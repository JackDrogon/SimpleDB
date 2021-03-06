#pragma once

#include <string>

#include <sys/types.h>

// TODO EOF const
// TODO Error status
// TODO ReadAll
// TODO std::move

class File
{
public:
	File(const std::string &name);
	~File();

	ssize_t Read(const off_t offset, size_t nbytes, char *buf);
	std::string Read(const off_t offset, size_t nbytes);
	ssize_t Append(const std::string &msg);
	ssize_t Append(const char *buf, size_t bytes_written);
	int Flush();

	std::string Name() { return name_; }

private:
	const std::string name_;
	int fd_;
	int64_t offset_;
};
