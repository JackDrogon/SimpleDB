#include "db/db_impl.h"

#include <_types/_uint64_t.h>
#include <memory>
#include <system_error>

#include <cstdlib>
#include <inttypes.h>
#include <sys/stat.h>

#include "db.h"
#include "file/record_writer.h"
#include "utils/coding.h"

std::unique_ptr<DB> DB::Open(std::string name)
{
	std::error_code ec;
	auto db = Open(std::move(name), ec);
	if (ec) {
		throw;
	}

	return db;
}

std::unique_ptr<DB> DB::Open(std::string name, std::error_code &ec) noexcept {
	return std::make_unique<DBImpl>(std::move(name));
}

DBImpl::DBImpl(std::string name)
    : name_(std::move(name)), cache_(1024), kv_index_()
{
	// TODO constructor error
	// TODO Add logger
	// TODO metrics
	// check error filesystem
	mkdir(name_.c_str(), 0755);
	record_writer_ =
	    std::make_unique<RecordWriter>(name_ + "/hello.1", 1 << 20);
	BuildEntryCache();
}

void DBImpl::BuildEntryCache()
{
	off_t offset = 0;
	uint64_t size;
	std::string key;

	for (;;) {
		auto count = record_writer_->Read(
		    offset, sizeof(size), reinterpret_cast<char *>(&size));
		if (count <= 0) {
			return;
		}

		size = DecodeFixed64(reinterpret_cast<char *>(&size));

		auto kv = record_writer_->Read(offset + 8, size);
		if (kv.empty()) {
			return;
		}

		DecodeKey(kv, key);
		kv_index_.insert(std::make_pair(key, KeyEntry{1, offset}));
		offset = offset + 8 + (size >> 1);
	}
}

std::string DBImpl::EncodeKV(const std::string &key, const std::string &value,
			     bool deleted)
{
	std::string buf;
	uint64_t size, key_size, value_size;

	// TotalLength = 8 + KeyLength + 8 + ValueLength
	// TotalLength(with deleted flag), KeyLength, Key, ValueLength, Value
	EncodeFixed64(reinterpret_cast<char *>(&size),
		      8 + key.size() + 8 + value.size());
	size = deleted ? size << 1 : (size << 1) | 1;
	EncodeFixed64(reinterpret_cast<char *>(&key_size), key.size());
	EncodeFixed64(reinterpret_cast<char *>(&value_size), value.size());

	buf.append(reinterpret_cast<const char *>(&size), 8)
	    .append(reinterpret_cast<const char *>(&key_size), 8)
	    .append(key)
	    .append(reinterpret_cast<const char *>(&value_size), 8)
	    .append(value);

	return buf;
}

bool DBImpl::DecodeKV(const std::string &kv, const std::string &key,
		      std::string &value)
{
	auto kv_data = kv.data();
	uint64_t key_size = DecodeFixed64(kv_data);

	if (!strncmp(key.data(), kv_data + 8, key_size)) {
		return false;
	}

	uint64_t value_size = DecodeFixed64(kv_data + 8 + key_size);
	value.clear();
	value.append(kv_data + 8 + key_size + 8, value_size);
	return true;
}

void DBImpl::DecodeKey(const std::string &kv, std::string &key)
{
	auto kv_data = kv.data();
	uint64_t key_size = DecodeFixed64(kv_data);
	key.clear();
	key.append(kv_data + 8, key_size);
}

// FIXME: Crc32
// Record
bool DBImpl::Put(const std::string &key, const std::string &value)
{
	cache_.Put(key, value);

	// fprintf(record_writer_, "%lld,%ld:%s,%ld:%s\n", total_length,
	// key.length(), key.data(), value.length(), value.data());
	std::string kv(EncodeKV(key, value)); // std::move NRVO
	auto offset = record_writer_->Append(kv);
	if (offset < 0) {
		return false;
	}

	kv_index_.insert(std::make_pair(key, KeyEntry{1, offset}));
	return true;
}

bool DBImpl::Get(const std::string &key, std::string &value)
{
	if (cache_.Get(key, value)) {
		return true;
	}

	auto entry = kv_index_.find(key);
	if (entry == kv_index_.end())
		return false;

	uint64_t size;
	record_writer_->Read(entry->second.offset, sizeof(size),
			     reinterpret_cast<char *>(&size));
	size = DecodeFixed64(reinterpret_cast<char *>(&size));
	if (!(size & 1)) {
		return false;
	}

	auto kv = record_writer_->Read(entry->second.offset + 8, size >> 1);
	auto result = DecodeKV(kv, key, value);
	if (result) {
		cache_.Put(key, value);
	}
	return result;
}

bool DBImpl::Delete(const std::string &key)
{
	cache_.Remove(key);
	kv_index_.erase(key);

	std::string kv(EncodeKV(key, std::string(), true));
	auto offset = record_writer_->Append(kv);

	if (offset < 0)
		return false;
	return true;
}
