#pragma once
#include <cinttypes>
#include <iostream>

const size_t kDigitsArrayLength = 253;
const size_t kDivisionMod = 256;
const size_t kSignOneBit = 128;
const size_t kSizeOfDivisionBuffer = 610;

struct int2023_t {
    uint8_t digits[kDigitsArrayLength] = {0};
};

static_assert(sizeof(int2023_t) <= 253, "Size of int2023_t must be no higher than 253 bytes");

int2023_t from_int(int32_t i);

int2023_t from_string(const char* buff);

int2023_t operator+(const int2023_t& lhs, const int2023_t& rhs);

int2023_t operator-(const int2023_t& lhs, const int2023_t& rhs);

int2023_t operator*(const int2023_t& lhs, const int2023_t& rhs);

int2023_t operator/(const int2023_t& lhs, const int2023_t& rhs);

int2023_t operator/(const int2023_t& number, long long divider);

void operator/=(int2023_t& number, long long divider);

void operator/=(int2023_t& lhs, const int2023_t& rhs);

bool operator==(const int2023_t& lhs, const int2023_t& rhs);

bool operator!=(const int2023_t& lhs, const int2023_t& rhs);

std::ostream& operator<<(std::ostream& stream, const int2023_t& value);
