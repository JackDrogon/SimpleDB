#pragma once

#include <algorithm>
#include <string>
#include <memory>
#include <system_error>

class DB
{
public:
	// TODO: nocopyable but moveable
	DB() = default;
	virtual ~DB() = default;

	virtual bool Put(const std::string &key, const std::string &value) = 0;
	virtual bool Get(const std::string &key, std::string &value) = 0;
	virtual bool Delete(const std::string &key) = 0;

public:
	static std::unique_ptr<DB> Open(std::string name);
	static std::unique_ptr<DB> Open(std::string name,
					std::error_code &ec) noexcept;
};
