#ifndef DB_H_
#define DB_H_

#include "lru.h"
#include <string>
#include <unordered_map>

struct KeyEntry {
	uint32_t file_id;
	uint64_t offset;
};

class File;

class DB {
public:
	DB(std::string name);
	~DB();
	bool Put(std::string &key, std::string &value);
	bool Get(std::string &key, std::string &value);
	bool Delete(std::string &key);

private:
	std::string EncodeKV(std::string &key, std::string &value);
	void DecodeKV(std::string &kv, std::string &value);

	typedef LRU<std::string, std::string> Cache;
	typedef std::unordered_map<std::string, KeyEntry> KVIndex;

	std::string name_;
	File *write_file_;
	Cache cache_;
	KVIndex kv_index_;
};

#endif // DB_H_
