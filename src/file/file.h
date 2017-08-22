#pragma once

#include <string>

// TODO EOF const
// TODO Error status
// TODO ReadAll

class File
{
public:
	File(const std::string &name);
	~File();

	ssize_t Read(const int64_t offset, size_t nbytes, char *buf);
	std::string Read(const int64_t offset, size_t nbytes);
	ssize_t Append(const std::string &msg);
	ssize_t Append(const char *buf, size_t bytes_written);

	std::string Name() { return name_; }

private:
	const std::string name_;
	int fd_;
	int64_t offset_;
};
