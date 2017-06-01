//
//  ofxHammingCode.h
//
//  Created by ISHII 2bit on 2015/03/29.
//

#pragma once

#include <cstdint>
#include <cstddef>

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

#ifndef _UINT32_T
#define _UINT32_T
typedef unsigned int uint32_t;
#endif /* _UINT32_T */

BEGIN_NAMESPACE(ofxHammingCode)

std::uint8_t bit_sum_8(std::uint8_t b) {
    b = ((b & 0xAA) >> 1) + (b & 0x55);
    b = ((b & 0xCC) >> 2) + (b & 0x33);
    b = ((b & 0xF0) >> 4) + (b & 0x0F);
    return b & 1;
}

std::uint8_t bit_sum_16(std::uint16_t b) {
    b = ((b & 0xAAAA) >> 1) + (b & 0x5555);
    b = ((b & 0xCCCC) >> 2) + (b & 0x3333);
    b = ((b & 0xF0F0) >> 4) + (b & 0x0F0F);
    b = ((b & 0xFF00) >> 8) + (b & 0x00FF);
    return b & 1;
}

std::uint8_t bit_sum_32(std::uint32_t b) {
    b = ((b & 0xAAAAAAAA) >>  1) + (b & 0x55555555);
    b = ((b & 0xCCCCCCCC) >>  2) + (b & 0x33333333);
    b = ((b & 0xF0F0F0F0) >>  4) + (b & 0x0F0F0F0F);
    b = ((b & 0xFF00FF00) >>  8) + (b & 0x00FF00FF);
    b = ((b & 0xFFFF0000) >> 16) + (b & 0x0000FFFF);
    return b & 1;
}

namespace H74 {

    /*
     G =
     1000 110
     0100 011
     0010 101
     0001 111
     
     H =
     1011 100
     1101 010
     0111 001
     */

    namespace {
        inline std::uint8_t first(std::uint16_t h) {
            return h >> 8;
        }

        inline std::uint8_t second(std::uint16_t h) {
            return h & 0xFF;
        }

        std::uint8_t correct_bit[8] = {0, 1, 2, 32, 4, 16, 64, 8};

        inline std::uint8_t encode_half(std::uint8_t d) {
            return ((d & 15) << 3)
            | (bit_sum_8(d & 11) << 2)
            | (bit_sum_8(d & 13) << 1)
            | bit_sum_8(d & 7);
        }
        inline std::uint8_t check_data(std::uint8_t c) {
            return (bit_sum_8(c & 92) << 2)
            | (bit_sum_8(c & 106) << 1)
            | bit_sum_8(c & 57);
        }
    };

    inline std::uint16_t encode(std::uint8_t data) {
        return (encode_half(data >> 4) << 8) | encode_half(data & 15);
    }

    inline std::uint8_t decode(std::uint16_t h) {
        return ((first(h) & 120) << 1) | (second(h) >> 3);
    }
    
    inline bool isCorrect(std::uint16_t h) {
        return (check_data(first(h)) | check_data(second(h))) == 0;
    }

    inline void correct(std::uint16_t &h) {
        h ^= (correct_bit[check_data(first(h))] << 8) | correct_bit[check_data(second(h))];
    }

    namespace SECDED {
        /*
         G =
         1000 110
         0100 011
         0010 101
         0001 111
         
         H =
         1011 1000
         1101 0100
         0111 0010
         1111 1111
         */
        
        std::uint8_t correct_bit[8] = {1, 2, 4, 64, 8, 32, 128, 16};
        
        namespace {
            inline std::uint16_t add_parity(std::uint16_t data) {
                return (data << 1)
                | (bit_sum_8(first(data)) << 8)
                | bit_sum_8(second(data));
            }
            
            inline std::uint8_t check_data(std::uint8_t c) {
                return (bit_sum_8(c & 184) << 3)
                | (bit_sum_8(c & 212) << 2)
                | (bit_sum_8(c & 114) << 1)
                | bit_sum_8(c & 255);
            }
        };
        
        inline std::uint16_t encode(std::uint8_t data) {
            return add_parity((encode_half(data >> 4) << 8) | encode_half(data & 15));
        }
        
        inline std::uint8_t decode(std::uint16_t h) {
            return (first(h) & 240) | (second(h) >> 4);
        }
        
        inline bool isCorrect(std::uint16_t h) {
            return (check_data(first(h)) | check_data(second(h))) == 0;
        }
        
        inline bool isCorrectable(std::uint16_t h) {
            return (check_data(first(h)) & 1) || (check_data(second(h)) & 1);
        }
        
        inline void correct(std::uint16_t &h) {
            std::uint8_t first_f = check_data(first(h)), second_f = check_data(second(h));
            if(first_f & 1) h ^= correct_bit[first_f >> 1] << 8;
            if(second_f & 1) h ^= correct_bit[second_f >> 1];
        }
    };
};

namespace H3126 {
    /*
     G =
     1000 0000 0000 0000 0000 0000 0000 111
     0100 0000 0000 0000 0000 0000 0001 011
     0010 0000 0000 0000 0000 0000 0001 101
     0001 0000 0000 0000 0000 0000 0001 110
     0000 1000 0000 0000 0000 0000 0010 011
     0000 0100 0000 0000 0000 0000 0010 101
     0000 0010 0000 0000 0000 0000 0010 110
     0000 0001 0000 0000 0000 0000 0011 001
     0000 0000 1000 0000 0000 0000 0011 010
     0000 0000 0100 0000 0000 0000 0011 100
     0000 0000 0010 0000 0000 0000 0011 111
     0000 0000 0001 0000 0000 0000 0011 110
     0000 0000 0000 1000 0000 0000 0011 101
     0000 0000 0000 0100 0000 0000 0011 011
     0000 0000 0000 0010 0000 0000 0011 000
     0000 0000 0000 0001 0000 0000 0010 111
     0000 0000 0000 0000 1000 0000 0010 100
     0000 0000 0000 0000 0100 0000 0010 010
     0000 0000 0000 0000 0010 0000 0010 001
     0000 0000 0000 0000 0001 0000 0001 111
     0000 0000 0000 0000 0000 1000 0001 100
     0000 0000 0000 0000 0000 0100 0001 010
     0000 0000 0000 0000 0000 0010 0001 001
     0000 0000 0000 0000 0000 0001 0000 110
     0000 0000 0000 0000 0000 0000 1000 101
     0000 0000 0000 0000 0000 0000 0100 011
     
     H =
     1110 1101 0010 1101 0011 0010 1100 001
     1101 1010 1011 0101 0101 0101 0100 010
     1011 0110 0111 1001 1001 1001 1000 100
     0111 0001 1111 1110 0001 1110 0001 000
     0000 1111 1111 1111 1110 0000 0010 000
     */

    namespace {
        std::uint32_t correct_bit[32] = {
            0x0,
            0x10,       0x8,        0x10000, 	0x4,
            0x4000, 	0x400,      0x200000, 	0x2,
            0x2000, 	0x200,      0x400000, 	0x80,
            0x1000000, 	0x8000000, 	0x80000, 	0x1,
            0x1000, 	0x100,      0x800000, 	0x40,
            0x2000000, 	0x10000000, 0x40000, 	0x20,
            0x4000000, 	0x20000000, 0x20000, 	0x40000000,
            0x8000, 	0x800,      0x100000
        };
        
        inline std::uint32_t check_data(std::uint32_t c) {
            return (bit_sum_32(c & 0x76969961) << 4)
                | (bit_sum_32(c & 0x6d5aaaa2) << 3)
                | (bit_sum_32(c & 0x5b3cccc4) << 2)
                | (bit_sum_32(c & 0x38ff0f08) << 1)
                | bit_sum_32(c & 0x7fff010);
        }
    };

    inline std::uint32_t encode(std::uint32_t data) {
        return ((data & 0x3ffffff) << 5)
             | (bit_sum_32(data & 0x3fff80) << 4)
             | (bit_sum_32(data & 0x1c7f878) << 3)
             | (bit_sum_32(data & 0x2d9e666) << 2)
             | (bit_sum_32(data & 0x36ad555) << 1)
             | bit_sum_32(data & 0x3b4b4cb);
    }

    inline std::uint32_t decode(std::uint32_t h) {
        return ((h >> 5) & 0x3ffffff);
    }
    
    inline bool isCorrect(std::uint32_t h) {
        return check_data(h) == 0;
    }

    inline void correct(std::uint32_t &h) {
        h ^= correct_bit[check_data(h)];
    }

    namespace SECDED {
        /*
         G =
         1000 0000 0000 0000 0000 0000 0000 111
         0100 0000 0000 0000 0000 0000 0001 011
         0010 0000 0000 0000 0000 0000 0001 101
         0001 0000 0000 0000 0000 0000 0001 110
         0000 1000 0000 0000 0000 0000 0010 011
         0000 0100 0000 0000 0000 0000 0010 101
         0000 0010 0000 0000 0000 0000 0010 110
         0000 0001 0000 0000 0000 0000 0011 001
         0000 0000 1000 0000 0000 0000 0011 010
         0000 0000 0100 0000 0000 0000 0011 100
         0000 0000 0010 0000 0000 0000 0011 111
         0000 0000 0001 0000 0000 0000 0011 110
         0000 0000 0000 1000 0000 0000 0011 101
         0000 0000 0000 0100 0000 0000 0011 011
         0000 0000 0000 0010 0000 0000 0011 000
         0000 0000 0000 0001 0000 0000 0010 111
         0000 0000 0000 0000 1000 0000 0010 100
         0000 0000 0000 0000 0100 0000 0010 010
         0000 0000 0000 0000 0010 0000 0010 001
         0000 0000 0000 0000 0001 0000 0001 111
         0000 0000 0000 0000 0000 1000 0001 100
         0000 0000 0000 0000 0000 0100 0001 010
         0000 0000 0000 0000 0000 0010 0001 001
         0000 0000 0000 0000 0000 0001 0000 110
         0000 0000 0000 0000 0000 0000 1000 101
         0000 0000 0000 0000 0000 0000 0100 011
         
         H =
         1110 1101 0010 1101 0011 0010 1100 0010 = 0xed2d32c2
         1101 1010 1011 0101 0101 0101 0100 0100 = 0xdab55544
         1011 0110 0111 1001 1001 1001 1000 1000 = 0xb6799988
         0111 0001 1111 1110 0001 1110 0001 0000 = 0x71fe1e10
         0000 1111 1111 1111 1110 0000 0010 0000 = 0xfffe020
         1111 1111 1111 1111 1111 1111 1111 1111 = 0xffffffff
         */
        
        namespace {
            std::uint32_t correct_bit[32] = {
                0x1,
                0x20,       0x10,       0x20000,    0x8,
                0x8000,     0x800,      0x400000,   0x4,
                0x4000,     0x400,      0x800000,   0x100,
                0x2000000,  0x10000000, 0x100000,   0x2,
                0x2000,     0x200,      0x1000000,  0x80,
                0x4000000,  0x20000000, 0x80000,    0x40,
                0x8000000,  0x40000000, 0x40000,    0x80000000,
                0x10000,    0x1000,     0x200000
            };
            
            inline std::uint32_t add_parity(std::uint32_t data) {
                return (data << 1) | bit_sum_32(data);
            }
            
            inline std::uint32_t check_data(std::uint32_t c) {
                return (bit_sum_32(c & 0xed2d32c2) << 5)
                | (bit_sum_32(c & 0xdab55544) << 4)
                | (bit_sum_32(c & 0xb6799988) << 3)
                | (bit_sum_32(c & 0x71fe1e10) << 2)
                | (bit_sum_32(c & 0xfffe020) << 1)
                | (bit_sum_32(c & 0xffffffff));
            }
        };
        
        inline std::uint32_t encode(std::uint32_t data) {
            return add_parity(H3126::encode(data));
        }
        
        inline std::uint32_t decode(std::uint32_t h) {
            return ((h >> 6) & 0x3ffffff);
        }

        inline bool isCorrect(std::uint32_t h) {
            return check_data(h) == 0;
        }
        
        inline bool isCorrectable(std::uint32_t h) {
            return check_data(h) & 1;
        }
        
        inline void correct(std::uint32_t &h) {
            std::uint32_t f = check_data(h);
            if(f & 1) h ^= correct_bit[f >> 1];
        }
    };
};

END_NAMESPACE(ofxHammingCode)
