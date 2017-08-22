#pragma once

#include "utils/lru.h"
#include <string>
#include <unordered_map>

struct KeyEntry {
	uint32_t file_id;
	ssize_t offset;
};

class File;

// TODO: Add mertrics
// TODO: Add compact
// TODO: Add table cache
class DB
{
public:
	DB(std::string name);
	~DB();
	bool Put(const std::string &key, const std::string &value);
	bool Get(const std::string &key, std::string &value);
	bool Delete(const std::string &key);

private:
	std::string EncodeKV(const std::string &key, const std::string &value,
			     bool deleted = false);
	bool DecodeKV(const std::string &kv, const std::string &key,
		      std::string &value); // TODO: use status to replace bool
	// @doc return value -> offset
	void DecodeKey(const std::string &kv, std::string &key);
	void BuildEntryCache();

	typedef LRU<std::string, std::string> Cache;
	typedef std::unordered_map<std::string, KeyEntry> KVIndex;

	std::string name_;
	File *record_writer_;
	Cache cache_;
	KVIndex kv_index_;
};