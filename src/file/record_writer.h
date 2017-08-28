#pragma once

#include <string>
#include <cstdint>
#include <inttypes.h>

#include "file.h"
#include "log_format.h"

class WritableFile;

class RecordWriter : public File
{
public:
	// static std::string Next() { return to_string(file_num_+1); }
	explicit RecordWriter(WritableFile *dest);
	RecordWriter(const std::string &name, const ssize_t file_size);
	~RecordWriter() {}

public:
	std::string ReadRecord();
	void AppendRecord(const std::string &record);

private:
	const ssize_t file_size_;

	File *dest_;
	int block_offset_; // Current offset in block

	// crc32c values for all supported record types.  These are
	// pre-computed to reduce the overhead of computing the crc of the
	// record type stored in the header.
	uint32_t type_crc_[kMaxRecordType + 1];

	int EmitPhysicalRecord(RecordType type, const char *ptr, size_t length);

	// No copying allowed
	RecordWriter(const RecordWriter &);
	void operator=(const RecordWriter &);
};
