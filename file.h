#ifndef FILE_H_
#define FILE_H_

#include <string>

class File {
public:
	File(std::string &name);
	~File();

	std::string Read(int64_t offset, size_t nbyte);
	std::string ReadEntry(int64_t offset);
	int64_t Append(std::string &msg);
	int64_t Append(const char *buf, ssize_t bytes_written);

	std::string Name() {return name_;}

private:
	std::string name_;
	int fd_;
	int64_t offset_;
};

#endif //FILE_H_
