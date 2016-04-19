#include "db.h"
#include "file.h"

#include <cstdlib>
#include <cstdint>

DB::DB(std::string name) :
	name_(name),
	write_file_(new File(name)),
	cache_(1024),
	kv_index_()
{
}

DB::~DB()
{
	delete write_file_;
}

std::string DB::EncodeKV(std::string &key, std::string &value)
{
	std::string buf;
	buf.append(sizeof)
}

// FIXME: Crc32
// Record
// TotalLength, KeyLength, Key, ValueLength, Value
bool DB::Put(std::string &key, std::string &value)
{
	std::uint64_t total_length = key.length() + value.length();
	cache_.put(key, value);

	// fprintf(write_file_, "%lld,%ld:%s,%ld:%s\n", total_length, key.length(), key.data(), value.length(), value.data());
	std::string kv(std::move(EncodeKV(key, value)));
	auto offset_ = write_file_->Append(kv);
	if (offset_ < 0) return false;

	return true;
}

bool DB::Get(std::string &key, std::string &value)
{
	if (cache_.get(key, value)) {
		return true;
	} else {
		KeyEntry &entry_ = kv_index_[key];
	}

	return false;
}
