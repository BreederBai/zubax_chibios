/*
 * Copyright (c) 2016 Zubax, zubax.com
 * Distributed under the MIT License, available in the file LICENSE.
 * Author: Pavel Kirienko <pavel.kirienko@zubax.com>
 */

#pragma once

#include <zubax_chibios/os.hpp>
#include <cstdint>
#include <cassert>


namespace bootloader
{
/**
 * Error codes.
 * These are returned from functions in negated form, i.e. -10000 means error code 10000.
 */
static constexpr std::int16_t ErrOK                     = 0;
static constexpr std::int16_t ErrInvalidState           = 10001;

/**
 * This is used to verify integrity of the application and other data.
 * CRC-64-WE
 * Description: http://reveng.sourceforge.net/crc-catalogue/17plus.htm#crc.cat-bits.64
 * Initial value: 0xFFFFFFFFFFFFFFFF
 * Poly: 0x42F0E1EBA9EA3693
 * Reverse: no
 * Output xor: 0xFFFFFFFFFFFFFFFF
 * Check: 0x62EC59E3F1A4F00A
 */
class CRC64WE
{
    std::uint64_t crc_;

public:
    CRC64WE() : crc_(0xFFFFFFFFFFFFFFFFULL) { }

    void add(std::uint8_t byte)
    {
        static constexpr std::uint64_t Poly = 0x42F0E1EBA9EA3693;
        crc_ ^= std::uint64_t(byte) << 56;
        for (int i = 0; i < 8; i++)
        {
            crc_ = (crc_ & (std::uint64_t(1) << 63)) ? (crc_ << 1) ^ Poly : crc_ << 1;
        }
    }

    void add(const void* data, unsigned len)
    {
        auto bytes = static_cast<const std::uint8_t*>(data);
        assert(bytes != nullptr);
        while (len --> 0)
        {
            add(*bytes++);
        }
    }

    std::uint64_t get() const { return crc_ ^ 0xFFFFFFFFFFFFFFFFULL; }
};

}
