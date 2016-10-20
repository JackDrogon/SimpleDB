#include "db.h"
#include "file.h"
#include "coding.h"

#include <cstdlib>
#include <cstdint>
#include <sys/stat.h>

DB::DB(std::string name) :
	name_(name),
	cache_(1024),
	kv_index_()
{
	// TODO constructor error
	// TODO Add logger
	// TODO metrics
	mkdir(name.c_str(), 0755);
	write_file_ = new File(name+"/hello.1");
	BuildEntryCache();
}

DB::~DB()
{
	delete write_file_;
}

void DB::BuildEntryCache()
{
	ssize_t offset = 0;
	uint64_t size;
	std::string key;
	for(;;) {
		write_file_->Read(offset, sizeof(size),
				reinterpret_cast<char *>(&size));
		size = DecodeFixed64(reinterpret_cast<char *>(&size));

		auto kv = write_file_->Read(offset+8, size);
		if (kv.empty()) return ;

		DecodeKey(kv, key);
		kv_index_.insert(std::make_pair(key, KeyEntry{1, offset}));
		offset = offset + 8 + (size >> 1);
	}
}

std::string DB::EncodeKV(const std::string &key, const std::string &value,
		bool deleted)
{
	std::string buf;
	uint64_t size, key_size, value_size;

	// TotalLength = 8 + KeyLength + 8 + ValueLength
	// TotalLength(with deleted flag), KeyLength, Key, ValueLength, Value
	EncodeFixed64(reinterpret_cast<char *>(&size), 8+key.size()+8+value.size());
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

bool DB::DecodeKV(const std::string &kv, const std::string &key, std::string &value)
{
	auto kv_data = kv.data();
	uint64_t key_size = DecodeFixed64(kv_data);

	if (!strncmp(key.data(), kv_data+8, key_size)) return false;

	uint64_t value_size = DecodeFixed64(kv_data+8+key_size);
	value.clear();
	value.append(kv_data+8+key_size+8, value_size);
	return true;
}

void DB::DecodeKey(const std::string &kv, std::string &key)
{
	auto kv_data = kv.data();
	uint64_t key_size = DecodeFixed64(kv_data);
	key.clear();
	key.append(kv_data+8, key_size);
}

// FIXME: Crc32
// Record
bool DB::Put(const std::string &key, const std::string &value)
{
	cache_.put(key, value);

	// fprintf(write_file_, "%lld,%ld:%s,%ld:%s\n", total_length, key.length(), key.data(), value.length(), value.data());
	std::string kv(EncodeKV(key, value)); // std::move NRVO
	auto offset = write_file_->Append(kv);
	if (offset < 0) return false;

	kv_index_.insert(std::make_pair(key, KeyEntry{1, offset}));

	return true;
}

bool DB::Get(const std::string &key, std::string &value)
{
	if (cache_.get(key, value)) {
		return true;
	} else {
		auto entry = kv_index_.find(key);
		if (entry == kv_index_.end()) return false;

		uint64_t size;
		write_file_->Read(entry->second.offset, sizeof(size),
				reinterpret_cast<char *>(&size));
		size = DecodeFixed64(reinterpret_cast<char *>(&size));
		if (!(size & 1)) return false;

		auto kv = write_file_->Read(entry->second.offset+8, size>>1);
		auto result = DecodeKV(kv, key, value);
		if (result) cache_.put(key, value);
		return result;
	}

	return false;
}

bool DB::Delete(const std::string &key)
{
	cache_.remove(key);
	kv_index_.erase(key);

	std::string kv(EncodeKV(key, std::string(), true));
	auto offset = write_file_->Append(kv);

	if (offset < 0) return false;
	return true;
}
