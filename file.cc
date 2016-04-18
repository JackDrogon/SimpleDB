#include "file.h"

#include <memory>
#include <cstdlib>
#include <fcntl.h>
#include <unistd.h>

File::File(std::string &name) :
	name_(name)
{
	fd_ = open(name_.data(), O_RDWR | O_CREAT | O_APPEND, 0600);
	if (fd_ < 0) abort();

	offset_ = lseek(fd_, 0, SEEK_END);
}

File::~File()
{
	close(fd_);
}

std::string File::Read(int64_t offset, size_t nbyte)
{
	std::shared_ptr<char>buf(new char[nbyte+1]);
	pread(fd_, buf.get(), nbyte, offset);

	return std::string(buf.get(), nbyte);
}

std::string File::ReadEntry(int64_t offset)
{
	int64_t total_length = 0;
	pread(fd_, &total_length, sizeof total_length, offset);

	std::shared_ptr<char>buf(new char[total_length+1]);
	pread(fd_, buf.get(), total_length, offset+sizeof(total_length));

	return std::string(buf.get(), total_length);
}

int64_t File::Append(const char *buf, ssize_t count)
{
	ssize_t bytes_written = 0;

	while (count > 0) {
		bytes_written = write(fd_, buf, count);
		if (bytes_written > 0) {
			count -= bytes_written;
			buf += bytes_written;
		} else {
			return -1;
		}
	}

	return offset_ += bytes_written;
}

int64_t File::Append(std::string &msg)
{
	return Append(msg.data(), msg.size());
}
