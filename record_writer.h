#pragma once

#include <string>
#include <cstdint>

#include "file.h"
#include "log_format.h"

class WritableFile;

class RecordWriter : public File {
public:
	// static std::string Next() { return to_string(file_num_+1); }
	explicit RecordWriter(WritableFile *dest);
	RecordWriter(const std::string &name, const ssize_t file_size);
	~RecordWriter() { }
	std::string ReadRecord();
	void AppendRecord();

private:
	const ssize_t file_size_;

	WritableFile *dest_;
	int block_offset_; // Current offset in block

	// crc32c values for all supported record types.  These are
	// pre-computed to reduce the overhead of computing the crc of the
	// record type stored in the header.
	uint32_t type_crc_[kMaxRecordType + 1];

	Status EmitPhysicalRecord(RecordType type, const char *ptr,
				  size_t length);

	// No copying allowed
	Writer(const Writer &);
	void operator=(const Writer &);
};
