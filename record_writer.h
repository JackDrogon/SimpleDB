#pragma once

#include <string>
#include <cstdint>
#include "file.h"

class RecordWriter : public File {
public:
	// static std::string Next() { return to_string(file_num_+1); }
	RecordWriter(const std::string &name, const ssize_t file_size);
	~RecordWriter() { }
	std::string ReadRecord();
	void WriteRecord();

private:
	const ssize_t file_size_;
};
