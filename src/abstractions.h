#if !defined ARGENT_ABSTRACTIONS
#define ARGENT_ABSTRACTIONS


#include <argent/core.h>


typedef char aga_string;


extern agc_erno
aga_string_new(aga_string **ctx, const char *cstr);


inline agc_erno
aga_string_copy(aga_string **ctx, const aga_string *src)
{
        return aga_string_new (ctx, src);
}


extern agc_erno
aga_string_len(const aga_string *ctx, size_t *len);



extern agc_erno
aga_string_sz(const aga_string *ctx, size_t *sz);


extern agc_erno
aga_string_cmp(const aga_string *ctx, const aga_string *rhs, int *cmp);


extern agc_erno
aga_string_add(const aga_string **ctx, const aga_string *add);


extern agc_erno
aga_string_find(const aga_string *ctx, const aga_string *what, size_t *loc);


extern agc_erno
aga_string_replace(aga_string **ctx, const aga_string *what,
                   const aga_string *with);


extern agc_erno
aga_string_replace_first(aga_string **ctx, const aga_string *what,
                          const aga_string *with);


#endif /* !defined ARGENT_ABSTRACTIONS */

