//
//  ofxHammingCode.h
//
//  Created by ISHII 2bit on 2015/03/29.
//

#pragma once

#define BEGIN_NAMESPACE(name) namespace name {
#define END_NAMESPACE(name) };

#ifndef _UINT8_T
#define _UINT8_T
typedef unsigned char uint8_t;
#endif /* _UINT8_T */

#ifndef _UINT16_T
#define _UINT16_T
typedef unsigned short uint16_t;
#endif /* _UINT16_T */

BEGIN_NAMESPACE(ofxHammingCode)

BEGIN_NAMESPACE(H74)

/*
	H = [[1, 1, 0], [0, 1, 1], [1, 0, 1], [1, 1, 1], [1, 0, 0], [0, 1, 0], [0, 0, 1]]
	G = [[1, 0, 0, 0], [0, 1, 0, 0], [0, 0, 1, 0], [0, 0, 0, 1], [1, 0, 1, 1], [1, 1, 0, 1], [0, 1, 1, 1]]
 */

namespace {
    inline uint8_t first(uint16_t h) {
        return h >> 8;
    }

    inline uint8_t second(uint16_t h) {
        return h & 0xFF;
    }

    template <int n>
    inline uint8_t digit_sum(uint8_t c) {
        return ((c >> (n - 1)) & 1) + digit_sum<n - 1>(c);
    }

    template <>
    inline uint8_t digit_sum<1>(uint8_t c) {
        return c & 1;
    }

    template <int n>
    inline uint8_t binary_digit_sum(uint8_t c) {
        return 1 & digit_sum<n>(c);
    }

    uint8_t correct_bit[8] = {0, 1, 2, 32, 4, 16, 64, 8};

    inline uint8_t encode_half(uint8_t d) {
        return ((d & 15) << 3) | (binary_digit_sum<4>(d & 11) << 2) | (binary_digit_sum<4>(d & 13) << 1) | binary_digit_sum<3>(d & 7);
    }
    inline uint8_t check_data(uint8_t c) {
        return (binary_digit_sum<7>(c & 92) << 2) | (binary_digit_sum<7>(c & 106) << 1) | binary_digit_sum<7>(c & 57);
    }
};

inline uint16_t encode(uint8_t data) {
    return (encode_half(data >> 4) << 8) | encode_half(data & 15);
}

inline bool isCorrect(uint16_t h) {
    return (check_data(first(h)) | check_data(second(h))) == 0;
}

inline void correct(uint16_t &h) {
    h ^= (correct_bit[check_data(first(h))] << 8) | correct_bit[check_data(second(h))];
}

inline uint8_t decode(uint16_t h) {
    return ((first(h) & 120) << 1) | (second(h) >> 3);
}

END_NAMESPACE(H74)

BEGIN_NAMESPACE(H74SECDED)

/*
	H = [[1, 1, 0, 1], [0, 1, 1, 1], [1, 0, 1, 1], [1, 1, 1, 1], [1, 0, 0, 1], [0, 1, 0, 1], [0, 0, 1, 1], [0, 0, 0, 1]
	G = [[1, 0, 0, 0], [0, 1, 0, 0], [0, 0, 1, 0], [0, 0, 0, 1], [1, 0, 1, 1], [1, 1, 0, 1], [0, 1, 1, 1], [0, 0, 0, 0]]
 */

uint8_t correct_bit[8] = {1, 2, 4, 64, 8, 32, 128, 16};

namespace {
    inline uint8_t encode_half(uint8_t d) {
        return ((d & 15) << 4) | (H74::binary_digit_sum<4>(d & 11) << 3) | (H74::binary_digit_sum<4>(d & 13) << 2) | ((H74::binary_digit_sum<3>(d & 7)) << 1);
    }

    inline uint16_t add_parity(uint16_t data) {
        return data | (H74::binary_digit_sum<8>(H74::first(data)) << 8) | H74::binary_digit_sum<8>(H74::second(data));
    }

    inline uint8_t check_data(uint8_t c) {
        return (H74::binary_digit_sum<8>(c & 184) << 3) | (H74::binary_digit_sum<8>(c & 212) << 2) | (H74::binary_digit_sum<8>(c & 114) << 1) | H74::binary_digit_sum<8>(c & 255);
    }
};

inline uint16_t encode(uint8_t data) {
    return add_parity((encode_half(data >> 4) << 8) | encode_half(data & 15));
}

inline bool isCorrect(uint16_t h) {
    return (check_data(H74::first(h)) | check_data(H74::second(h))) == 0;
}

inline uint8_t decode(uint16_t h) {
    return (H74::first(h) & 240) | (H74::second(h) >> 4);
}

inline bool isCorrectable(uint16_t h) {
    return (check_data(H74::first(h)) & 1) || (check_data(H74::second(h)) & 1);
}

inline void correct(uint16_t &h) {
    uint8_t first_f = check_data(H74::first(h)), second_f = check_data(H74::second(h));
    if(first_f & 1) h ^= correct_bit[first_f >> 1] << 8;
    if(second_f & 1) h ^= correct_bit[second_f >> 1];
}

END_NAMESPACE(H74SECDED)

END_NAMESPACE(ofxHammingCode)