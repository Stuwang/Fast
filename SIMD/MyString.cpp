#include <MyString.h>

#ifdef _MSC_VER 
# define Len(x) 
#else  defined(__GNUC__)
# define Len(x) __builtin_ctz(x)
#endif

static inline size_t Strlen16(const char* p){
  __m128i xmm0 = _mm_setzero_si128();
  __m128i xmm1;
  int mask;
  xmm1 = _mm_load_si128((__m128i *)p);
  xmm1 = _mm_cmpeq_epi8(xmm1, xmm0);
  if ( (mask = _mm_movemask_epi8(xmm1)) == 0 ){
    return sizeof(__m128i);
  } else{
    unsigned long pos;
    _BitScanForward(&pos, mask);
    return pos;
  }

}

static inline size_t Strlen32(const char* p){
  __m256i xmm0 = _mm256_setzero_si256();
  __m256i xmm1;
  int mask;
  xmm1 = _mm256_load_si256((__m256i *)p);
  xmm1 = _mm256_cmpeq_epi8(xmm1, xmm0);
  if ( (mask = _mm256_movemask_epi8(xmm1)) == 0 ){
    return sizeof(__m256i);
  } else{
    unsigned long pos;
    _BitScanForward(&pos, mask);
    return pos;
  }
}

size_t Strlen(const char* p){

  assert(p);

  size_t len = 0;
  while ( (((intptr_t)p) & (sizeof(__m128i) - 1)) != 0 ){
    if ( *p++ == 0 ){
      return len;
    }
    ++len;
  }

  if ( (((intptr_t)p) & (sizeof(__m256i) - 1)) != 0 ){
    rsize_t l = Strlen16(p);
    if ( l == sizeof(__m128i) ){
      len += sizeof(__m128i);
      p += sizeof(__m128i);
    } else{
      return len + l;
    }
  }

  for ( ;; ){
    size_t l = Strlen32(p);
    if ( l == sizeof(__m256i) ){
      len += sizeof(__m256i);
      p += sizeof(__m256i);
    } else{
      return len + l;
    }
  }
};

static inline void Strcpy16(char * dest, const char* src){

}

static inline void Strcpy64(char * dest, const char* src){

}

char* Strcpy(char *dest, const char* src){
  assert(dest);
  assert(src);

  if ( src == dest )
    return dest;

  char *p = dest;

  while ( (((intptr_t)p) & (sizeof(__m128i) - 1)) != 0 ){
    *p++ = *src++;
  }


  return dest;

};