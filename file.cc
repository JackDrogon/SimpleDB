#include "file.h"

#include <memory>
#include <cstdlib>
#include <fcntl.h>
#include <unistd.h>

File::File(const std::string &name) :
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

ssize_t File::Read(const int64_t offset, size_t nbytes, char *buf)
{
	return pread(fd_, buf, nbytes, offset);
}

std::string File::Read(const int64_t offset, size_t nbytes)
{
	std::shared_ptr<char>buf(new char[nbytes+1]);
	Read(offset, nbytes, buf.get());

	return std::string(buf.get(), nbytes);
}

// std::string File::ReadEntry(int64_t offset)
// {
// 	uint64_t total_length = 0;
// 	pread(fd_, &total_length, sizeof total_length, offset);

// 	std::shared_ptr<char>buf(new char[total_length+1]);
// 	pread(fd_, buf.get(), static_cast<size_t>(total_length), offset+sizeof(total_length));

// 	return std::string(buf.get(), total_length);
// }

ssize_t File::Append(const char *buf, size_t count)
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

ssize_t File::Append(const std::string &msg)
{
	return Append(msg.data(), msg.size());
}
