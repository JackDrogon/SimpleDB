#pragma once

#include <cstdint>
#include "port.h"

// Lower-level versions of Put... that write directly into a character buffer
// REQUIRES: dst has enough space for the value being written
extern void EncodeFixed32(char *dst, uint32_t value);
extern void EncodeFixed64(char *dst, uint64_t value);

// Lower-level versions of Get... that read directly from a character buffer
// without any bounds checking.
uint32_t DecodeFixed32(const char *ptr);
uint64_t DecodeFixed64(const char *ptr);
