#include "number.h"
#include <cstring>

//Negative number for the input --> the highest bit in the last byte for the sign
//Сonverting the numbers to the 256 number system

int2023_t from_int(int32_t i) {
    int2023_t number;
    size_t current_index = 0;
    //If the number is negative
    if (i < 0) {
        number.digits[kDigitsArrayLength - 1] = kSignOneBit;
        i *= -1;
    }

    while (i > 0) {
        number.digits[current_index++] += i % kDivisionMod;
        i /= kDivisionMod;
    }

    return number;
}

int2023_t from_string(const char* buff) {
    int2023_t result;
    bool sign_of_number = (buff[0] != '-');
    bool quot_of_div_exist;
    //Storing the current quotient of division
    char quotient_of_division[kSizeOfDivisionBuffer];
    char current_digit_in_quot;
    //Transferring the module of a number to an array
    (sign_of_number) ? strcpy(quotient_of_division, buff) : strcpy(quotient_of_division, &buff[1]);
    //Storing the current digit in a larger number system for digit carry calculations
    uint16_t digit_65536_number_system;
    uint16_t remainder;
    size_t index_in_quot_of_div;
    size_t index_in_result = 0;

    //At each iteration of the loop getting a digit in 256 number system
    while (strlen(quotient_of_division) && index_in_result < kDigitsArrayLength) {
        quot_of_div_exist = false;
        index_in_quot_of_div = 0;
        remainder = 0;

        //Getting a new quotient of division
        for (int i = 0; i < strlen(quotient_of_division); ++i) {
            digit_65536_number_system = remainder * 10 + quotient_of_division[i] - '0';
            remainder = digit_65536_number_system % kDivisionMod;
            //Converting the current digit of the quotient from division to char
            current_digit_in_quot = (char)(digit_65536_number_system / kDivisionMod + '0');
            if (current_digit_in_quot != '0') {
                quot_of_div_exist = true;
            }
            if (quot_of_div_exist) {
                quotient_of_division[index_in_quot_of_div++] = current_digit_in_quot;
            }
        }
        quotient_of_division[index_in_quot_of_div] = '\0';
        result.digits[index_in_result++] = remainder;
    }

    //If the number is negative
    if (!sign_of_number) {
        result.digits[kDigitsArrayLength - 1] += kSignOneBit;
    }

    return result;
}

int ModuleCompare(const int2023_t& a, const int2023_t& b) {
    //1 - first larger than second, 2 - second larger than first, 0 - equals
    bool sign_a = (a.digits[kDigitsArrayLength - 1] < kSignOneBit);
    bool sign_b = (b.digits[kDigitsArrayLength - 1] < kSignOneBit);
    if (a.digits[kDigitsArrayLength - 1] - !sign_a * kSignOneBit > b.digits[kDigitsArrayLength - 1] - !sign_b * kSignOneBit) {
        return 1;
    } else if (a.digits[kDigitsArrayLength - 1] - !sign_a * kSignOneBit < b.digits[kDigitsArrayLength - 1] - !sign_b * kSignOneBit) {
        return 2;
    }

    for (int i = kDigitsArrayLength-2; i >= 0; --i) {
        if (a.digits[i] > b.digits[i]) {
            return 1;
        } else if(a.digits[i] < b.digits[i]) {
            return 2;
        }
    }

    return 0;
}

int2023_t NumberInverse(int2023_t& number) {
    uint16_t digit_65536_number_system;
    uint8_t carry = 1;

    //Invert the bytes with respect to the maximum digit in 256 number system
    for (int i = 0; i < kDigitsArrayLength; ++i) {
        number.digits[i] = kDivisionMod - number.digits[i] - 1;
    }

    //+1
    for (int i = 0; i < kDigitsArrayLength && carry; ++i) {
        digit_65536_number_system = number.digits[i] + carry;
        number.digits[i] = digit_65536_number_system;
        carry = digit_65536_number_system / kDivisionMod;
    }

    return number;
}

int2023_t operator+(const int2023_t& lhs, const int2023_t& rhs) {
    bool sign_lhs = (lhs.digits[kDigitsArrayLength - 1] < kSignOneBit);
    bool sign_rhs = (rhs.digits[kDigitsArrayLength - 1] < kSignOneBit);
    bool digit_transfer = false;
    uint16_t digit_65536_number_system;
    int2023_t a;
    int2023_t b;
    //Processing negative numbers
    (!sign_lhs) ? a = lhs, a.digits[kDigitsArrayLength - 1] -= kSignOneBit, NumberInverse(a) : a = lhs;
    (!sign_rhs) ? b = rhs, b.digits[kDigitsArrayLength - 1] -= kSignOneBit, NumberInverse(b) : b = rhs;

    //Adding numbers by column
    for (int i = 0; i < kDigitsArrayLength; ++i) {
        digit_65536_number_system = a.digits[i] + b.digits[i] + digit_transfer;
        digit_transfer = digit_65536_number_system / kDivisionMod;
        a.digits[i] += digit_65536_number_system - a.digits[i];
    }

    //If the result is negative
    if (!sign_lhs && !sign_rhs || !sign_lhs && ModuleCompare(lhs, rhs) == 1 ||
                                  !sign_rhs && ModuleCompare(lhs, rhs) == 2) {
        NumberInverse(a);
        a.digits[kDigitsArrayLength - 1] += kSignOneBit;
    }

    return a;
}

int2023_t operator-(const int2023_t& lhs, const int2023_t& rhs) {
    bool sign_lhs = (lhs.digits[kDigitsArrayLength - 1] < kSignOneBit);
    bool sign_rhs = (rhs.digits[kDigitsArrayLength - 1] < kSignOneBit);
    int2023_t a = lhs;
    int2023_t b = rhs;
    //Rationally realize subtraction by addition: a - b = a + (-b)
    if (sign_lhs && sign_rhs || !sign_lhs && sign_rhs) {
        b.digits[kDigitsArrayLength - 1] += kSignOneBit;
    } else {
        b.digits[kDigitsArrayLength - 1] -= kSignOneBit;
    }

    return a + b;
}

int2023_t operator*(const int2023_t& lhs, const int2023_t& rhs) {
    bool sign_lhs = (lhs.digits[kDigitsArrayLength - 1] < kSignOneBit);
    bool sign_rhs = (rhs.digits[kDigitsArrayLength - 1] < kSignOneBit);
    int2023_t result;
    int2023_t a = lhs;
    int2023_t b = rhs;
    a.digits[kDigitsArrayLength - 1] -= !sign_lhs * kSignOneBit;
    b.digits[kDigitsArrayLength - 1] -= !sign_rhs * kSignOneBit;
    uint16_t carry = 0;
    uint16_t digit_65536_number_system;
    size_t digit_transfer_index;

    //Multiplication of numbers by column
    for (int i = 0; i < kDigitsArrayLength; ++i) {
        for (int j = 0; j < kDigitsArrayLength - i; ++j) {
            digit_65536_number_system = a.digits[i] * b.digits[j] + result.digits[i + j] + carry;
            carry = digit_65536_number_system / kDivisionMod;
            result.digits[i + j] = digit_65536_number_system;
        }
        //Processing of number digit transfers
        digit_transfer_index = i + kDigitsArrayLength - 1;
        while (digit_transfer_index < kDigitsArrayLength && carry) {
            result.digits[digit_transfer_index] += carry;
            carry = result.digits[digit_transfer_index] / kDivisionMod;
            ++digit_transfer_index;
        }
    }

    //If the result is negative
    if (sign_lhs ^ sign_rhs && result != from_int(0)) {
        result.digits[kDigitsArrayLength - 1] += kSignOneBit;
    }

    return result;
}

int2023_t operator/(const int2023_t& lhs, const int2023_t& rhs) {
    bool sign_lhs = (lhs.digits[kDigitsArrayLength - 1] < kSignOneBit);
    bool sign_rhs = (rhs.digits[kDigitsArrayLength - 1] < kSignOneBit);
    int2023_t result;
    int2023_t a = lhs;
    int2023_t b = rhs;
    a.digits[kDigitsArrayLength - 1] -= !sign_lhs * kSignOneBit;
    b.digits[kDigitsArrayLength - 1] -= !sign_rhs * kSignOneBit;
    //Processing division by 1 or by itself
    if (a == b || b == from_int(1)) {
        if (a == b) {
            a = from_int(1);
        }
        if (sign_rhs ^ sign_lhs) {
            a.digits[kDigitsArrayLength - 1] += kSignOneBit;
        }

        return a;
    }
    int2023_t left_border = from_int(0);
    int2023_t right_border = a + from_int(1);

    //Searching by binary search for the quotient of division
    while (ModuleCompare(right_border - left_border, from_int(1)) == 1) {
        result = (left_border + right_border) / 2;
        if (ModuleCompare(result * b, a) == 1) {
            right_border = result;
        } else if (ModuleCompare(result * b, a) == 2) {
            left_border = result;
        }
    }

    //If the result is negative
    if (sign_lhs ^ sign_rhs && left_border != from_int(0)) {
        left_border.digits[kDigitsArrayLength - 1] += kSignOneBit;
    }

    return left_border;
}

int2023_t operator/(const int2023_t& number, long long divider) {
    int2023_t result = number;
    bool sign_lhs = (number.digits[kDigitsArrayLength - 1] < kSignOneBit);
    bool sign_divider = (divider >= 0);
    if (!sign_divider) {
        divider *= -1;
    }
    result.digits[kDigitsArrayLength - 1] -= !sign_lhs * kSignOneBit;
    unsigned long long current_digit = result.digits[kDigitsArrayLength - 1];

    //Column division of a long number by a short number
    for (int i = kDigitsArrayLength - 1; i >= 0; --i) {
        result.digits[i] = current_digit / divider;
        if (i) {
            current_digit = (current_digit % divider) * kDivisionMod + result.digits[i - 1];
        }
    }
    
    //If the result is negative
    if (sign_divider ^ sign_lhs) {
        result.digits[kDigitsArrayLength - 1] += kSignOneBit;
    }

    return result;

}

void operator/=(int2023_t& number, long long divider) {
    number = number / divider;
}

void operator/=(int2023_t& lhs, const int2023_t& rhs) {
    lhs = lhs / rhs;
}

bool operator==(const int2023_t& lhs, const int2023_t& rhs) {
    for (int i = 0; i < kDigitsArrayLength; ++i) {
        if (lhs.digits[i] != rhs.digits[i]) {
            return false;
        }
    }

    return true;
}

bool operator!=(const int2023_t& lhs, const int2023_t& rhs) {
    return !(lhs == rhs);
}

std::ostream& operator<<(std::ostream& stream, const int2023_t& value) {
    //Output a number in 256 number system
    bool sign_value = (value.digits[kDigitsArrayLength-1] < kSignOneBit);
    bool print_flag;
    if (!sign_value) {
        stream << "-";
        if (value.digits[kDigitsArrayLength-1] > kSignOneBit) {
            stream << "(" << static_cast<int16_t> (value.digits[kDigitsArrayLength-1] - kSignOneBit) << ")";
            print_flag = true;
        }
    } else if(value.digits[kDigitsArrayLength-1]) {
        stream << "(" << static_cast<int16_t> (value.digits[kDigitsArrayLength-1]) << ")";
        print_flag = true;
    }

    for (int i = kDigitsArrayLength - 2; i >= 0; --i) {
        if (print_flag) {
            stream << "(" << static_cast<int16_t> (value.digits[i]) << ")";
        } else if (value.digits[i] != 0){
            print_flag = true;
            stream << "(" << static_cast<int16_t> (value.digits[i]) << ")";
        }
    }
    if (!print_flag) {
        stream << "(0)";
    }

    return stream;
}
