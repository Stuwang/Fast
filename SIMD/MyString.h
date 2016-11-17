#ifndef MYSTRING_H__
#define MYSTRING_H__

#include <intrin.h>
#include <nmmintrin.h>
#include <emmintrin.h>
#include <immintrin.h>

#include <stdint.h>
#include <assert.h>

#ifdef __cplusplus
extern "C"{
#endif

  extern inline size_t Strlen(const char* p);

  extern inline char* Strcpy(char dest, const char* src);

  inline const char *SkipWhitespace_SIMD(const char* p) {
    // Fast return for single non-whitespace
    if ( *p == ' ' || *p == '\n' || *p == '\r' || *p == '\t' )
      ++p;
    else
      return p;

    // 16-byte align to the next boundary
    const char* nextAligned = reinterpret_cast<const char*>((reinterpret_cast<size_t>(p)+15) & static_cast<size_t>(~15));
    while ( p != nextAligned )
    if ( *p == ' ' || *p == '\n' || *p == '\r' || *p == '\t' )
      ++p;
    else
      return p;

    // The rest of string using SIMD
    static const char whitespace[16] = " \n\r\t";
    const __m128i w = _mm_loadu_si128(reinterpret_cast<const __m128i *>(&whitespace[0]));

    for ( ;; p += 16 ) {
      const __m128i s = _mm_load_si128(reinterpret_cast<const __m128i *>(p));
      const int r = _mm_cvtsi128_si32(_mm_cmpistrm(w, s, _SIDD_UBYTE_OPS | _SIDD_CMP_EQUAL_ANY | _SIDD_BIT_MASK | _SIDD_NEGATIVE_POLARITY));
      if ( r != 0 ) {   // some of characters is non-whitespace
        unsigned long offset;
        _BitScanForward(&offset, r);
        return p + offset;
      }
    }
  };

#ifdef __cplusplus
}
#endif

#endif //MYSTRING_H__