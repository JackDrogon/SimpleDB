#include "db.h"

#include <cstdlib>
#include <cstdint>

DB::DB(std::string name) :
	name_(name),
	write_file_(fopen(name.data(), "ae")),
	cache_(1024)
{
}

DB::~DB()
{
	fclose(write_file_);
}

// FIXME: Crc32
// Record
// TotalLength, KeyLength, Key, ValueLength, Value
bool DB::Put(std::string &key, std::string &value)
{
	std::uint64_t total_length = key.length() + value.length();
	cache_.put(key, value);

	fseek(write_file_, 0L, SEEK_END);
	fprintf(write_file_, "%lld,%ld:%s,%ld:%s\n", total_length, key.length(), key.data(), value.length(), value.data());

	return true;
}

bool DB::Get(std::string &key, std::string &value)
{
	if (cache_.get(key, value)) {
		return true;
	} else {
		fseek(write_file_, 0L, SEEK_SET);
		return false;
	}
}
