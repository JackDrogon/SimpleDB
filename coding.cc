#include "coding.h"
#include <cstring>

void EncodeFixed32(char* buf, uint32_t value)
{
	if (port::kLittleEndian) {
		memcpy(buf, &value, sizeof(value));
	} else {
		buf[0] = static_cast<char>(value & 0xff);
		buf[1] = static_cast<char>((value >> 8) & 0xff);
		buf[2] = static_cast<char>((value >> 16) & 0xff);
		buf[3] = static_cast<char>((value >> 24) & 0xff);
	}
}

void EncodeFixed64(char* buf, uint64_t value)
{
	if (port::kLittleEndian) {
		memcpy(buf, &value, sizeof(value));
	} else {
		buf[0] = static_cast<char>(value & 0xff);
		buf[1] = static_cast<char>((value >> 8) & 0xff);
		buf[2] = static_cast<char>((value >> 16) & 0xff);
		buf[3] = static_cast<char>((value >> 24) & 0xff);
		buf[4] = static_cast<char>((value >> 32) & 0xff);
		buf[5] = static_cast<char>((value >> 40) & 0xff);
		buf[6] = static_cast<char>((value >> 48) & 0xff);
		buf[7] = static_cast<char>((value >> 56) & 0xff);
	}
}


uint32_t DecodeFixed32(const char* ptr) {
  if (port::kLittleEndian) {
    // Load the raw bytes
    uint32_t result;
    memcpy(&result, ptr, sizeof(result));  // gcc optimizes this to a plain load
    return result;
  } else {
    return ((static_cast<uint32_t>(static_cast<unsigned char>(ptr[0])))
        | (static_cast<uint32_t>(static_cast<unsigned char>(ptr[1])) << 8)
        | (static_cast<uint32_t>(static_cast<unsigned char>(ptr[2])) << 16)
        | (static_cast<uint32_t>(static_cast<unsigned char>(ptr[3])) << 24));
  }
}

uint64_t DecodeFixed64(const char* ptr) {
  if (port::kLittleEndian) {
    // Load the raw bytes
    uint64_t result;
    memcpy(&result, ptr, sizeof(result));  // gcc optimizes this to a plain load
    return result;
  } else {
    uint64_t lo = DecodeFixed32(ptr);
    uint64_t hi = DecodeFixed32(ptr + 4);
    return (hi << 32) | lo;
  }
}
