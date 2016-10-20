#include "record_writer.h"

RecordWriter::RecordWriter(const std::string &name, const ssize_t file_size)
	: File(name), file_size_(file_size)
{
}
