#include <stdint.h>

typedef uint8_t u8;   ///<  8-bit unsigned integer
typedef uint16_t u16; ///< 16-bit unsigned integer
typedef uint32_t u32; ///< 32-bit unsigned integer
typedef uint64_t u64; ///< 64-bit unsigned integer

typedef int8_t s8;   ///<  8-bit signed integer
typedef int16_t s16; ///< 16-bit signed integer
typedef int32_t s32; ///< 32-bit signed integer
typedef int64_t s64; ///< 64-bit signed integer

typedef volatile u8 vu8;   ///<  8-bit volatile unsigned integer.
typedef volatile u16 vu16; ///< 16-bit volatile unsigned integer.
typedef volatile u32 vu32; ///< 32-bit volatile unsigned integer.
typedef volatile u64 vu64; ///< 64-bit volatile unsigned integer.

typedef volatile s8 vs8;   ///<  8-bit volatile signed integer.
typedef volatile s16 vs16; ///< 16-bit volatile signed integer.
typedef volatile s32 vs32; ///< 32-bit volatile signed integer.
typedef volatile s64 vs64; ///< 64-bit volatile signed integer.

#define WRITEU8(addr, data) *(vu8 *)(addr) = data
#define WRITEU16(addr, data) *(vu16 *)(addr) = data
#define WRITEU32(addr, data) *(vu32 *)(addr) = data
#define WRITES64(addr, data) *(vs64 *)(addr) = data
#define READU8(addr) *(vu8 *)(addr)
#define READU16(addr) *(vu16 *)(addr)
#define READU32(addr) *(vu32 *)(addr)
#define READS64(addr) *(vs64 *)(addr)