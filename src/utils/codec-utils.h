/**
 * This work copyright Chao Sun(qq:296449610) and licensed under
 * a Creative Commons Attribution 3.0 Unported License(https://creativecommons.org/licenses/by/3.0/).
 */

#ifndef FLYINGKV_UTILS_CODEC_UTILS_H
#define FLYINGKV_UTILS_CODEC_UTILS_H

#include <cstdint>

#include "../common/common-def.h"

#if BYTE_ORDER == BIG_ENDIAN
#define ByteOrderUtils utils::BigEndianCodecUtils
#elif BYTE_ORDER == LITTLE_ENDIAN
#define ByteOrderUtils utils::LittleEndianCodecUtils
#endif

namespace flyingkv {
namespace utils {
class LittleEndianCodecUtils {
PUBLIC
    // encode
    static void WriteUInt16(uchar *buf, uint16_t n);
    static void WriteUInt32(uchar *buf, uint32_t n);
    static void WriteUInt64(uchar *buf, uint64_t n);

    // decode
    static uint16_t ReadUInt16(uchar *buf);
    static uint32_t ReadUInt32(uchar *buf);
    static uint64_t ReadUInt64(uchar *buf);
}; // class LittleEndianCodecUtils

class BigEndianCodecUtils {
PUBLIC
    // encode
    static void WriteUInt16(uchar *buf, uint16_t n);
    static void WriteUInt32(uchar *buf, uint32_t n);
    static void WriteUInt64(uchar *buf, uint64_t n);

    // decode
    static uint16_t ReadUInt16(uchar *buf);
    static uint32_t ReadUInt32(uchar *buf);
    static uint64_t ReadUInt64(uchar *buf);
}; // class LittleEndianCodecUtils

}  // namespace utils
}  // namespace flyingkv

#endif //FLYINGKV_UTILS_CODEC_UTILS_H
