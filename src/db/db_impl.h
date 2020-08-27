#pragma once

#include "db.h"

#include <memory>
#include <unordered_map>
#include <string>

#include <unistd.h>

#include "lru.h"

struct KeyEntry {
	uint32_t file_id;
	off_t offset;
};

class File;

// TODO: Add mertrics
// TODO: Add compact
// TODO: Add table cache

class DBImpl final : public DB
{
public:
	DBImpl(std::string name);
	~DBImpl() override = default;

	bool Put(const std::string &key, const std::string &value) override;
	bool Get(const std::string &key, std::string &value) override;
	bool Delete(const std::string &key) override;

private:
	std::string EncodeKV(const std::string &key, const std::string &value,
			     bool deleted = false);
	bool DecodeKV(const std::string &kv, const std::string &key,
		      std::string &value); // TODO: use status to replace bool
	// @DOC return value -> offset
	void DecodeKey(const std::string &kv, std::string &key);
	void BuildEntryCache();

	typedef LRU<std::string, std::string> Cache;
	using KVIndex = std::unordered_map<std::string, KeyEntry>;

	std::string name_;
	std::unique_ptr<File> record_writer_;
	Cache cache_;
	KVIndex kv_index_;
};
