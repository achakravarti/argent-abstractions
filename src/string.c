#include <string.h>
#include <argent/middleware.h>
#include "abstractions.h"


#define NULLCHAR_OFFSET 1


static inline bool
utf8_continuation(char c)
{
    return (c & 0xC0) == 0x80;
}


static inline size_t
utf8_strlen(const char *str)
{
    register size_t i = 0, len = 0;

    while (str[i]) {
        if (!utf8_continuation (str[i]))
            len++;

        i++;
    }

    return len;
}


static agc_erno
replace_first(aga_string **ctx, const aga_string *what,
              const aga_string *with)
{
AGC_TRY:
    ;const char *pos = strstr (*ctx, what);
    if (!pos)
        return aga_string_copy(ctx, what);

    const agc_size ctxsz = strlen (*ctx);
    const agc_size whatsz = strlen (what);
    const agc_size withsz = strlen (with);
    const agc_size diffsz = withsz - whatsz;

    aga_string *tmp = NULL;
    agc_try (agm_mpool_alloc ((void **) &tmp,
                              whatsz + diffsz + NULLCHAR_OFFSET));

    const agc_size shifts = pos - *ctx;
    memcpy (tmp, *ctx, shifts);
    memcpy (tmp + shifts, with, withsz);
    memcpy (tmp + shifts + withsz, pos + whatsz, ctxsz - shifts - whatsz);

    tmp [ctxsz + diffsz] = '\0';
    *ctx = tmp;

AGC_CATCH:
    agc_erno_set (AGC_ERNO_STRING);

AGC_FINALLY:
    return agc_erno_get ();
}




extern agc_erno
aga_string_new(aga_string **ctx, const char *cstr)
{
AGC_TRY:
    agc_assert_string (cstr);

    const agc_size sz = strlen (cstr) + NULLCHAR_OFFSET;
    agc_try (agm_mpool_alloc ((void **) ctx, sz));
    (void) strncpy (*ctx, cstr, sz);

AGC_CATCH:
    agc_erno_set (AGC_ERNO_STRING);

AGC_FINALLY:
    return agc_erno_get ();
}


extern inline agc_erno
aga_string_copy(aga_string **ctx, const aga_string *src);


extern agc_erno
aga_string_len(const aga_string *ctx, size_t *len)
{
AGC_TRY:
    agc_assert_handle (ctx && len);

    *len = utf8_strlen (ctx);

AGC_CATCH:
    agc_erno_set (AGC_ERNO_STRING);

AGC_FINALLY:
    return agc_erno_get ();
}


extern agc_erno
aga_string_sz(const aga_string *ctx, size_t *sz)
{
AGC_TRY:
    agc_assert_handle (ctx);

    *sz = strlen (ctx) + NULLCHAR_OFFSET;

AGC_CATCH:
    agc_erno_set (AGC_ERNO_STRING);

AGC_FINALLY:
    return agc_erno_get ();
}


extern agc_erno
aga_string_cmp(const aga_string *ctx, const aga_string *rhs, int *cmp)
{
AGC_TRY:
    agc_assert_handle (ctx && rhs && cmp);

    *cmp = strcmp (ctx, rhs);

AGC_CATCH:
    agc_erno_set (AGC_ERNO_STRING);

AGC_FINALLY:
    return agc_erno_get ();
}


extern agc_erno
aga_string_add(const aga_string **ctx, const aga_string *add)
{
AGC_TRY:
    agc_assert_handle (ctx && *ctx && add);

    const agc_size llen = strlen (*ctx) + NULLCHAR_OFFSET;
    const agc_size rlen = strlen (add) + NULLCHAR_OFFSET;

    aga_string *cat = NULL;
    agc_try (agm_mpool_alloc ((void **) &cat, llen + rlen));

    (void) strncpy (cat, *ctx, llen);
    *ctx = strncat (cat, add, rlen);

AGC_CATCH:
    agc_erno_set (AGC_ERNO_STRING);

AGC_FINALLY:
    return agc_erno_get ();
}


extern agc_erno
aga_string_find(const aga_string *ctx, const aga_string *what, size_t *loc)
{
AGC_TRY:
    agc_assert_handle (ctx && what && loc);

    const agc_size offset = 1;
    const aga_string *sub = strstr (ctx, what);
    *loc = sub ? (utf8_strlen (ctx) - utf8_strlen (sub) + offset) : 0;

AGC_CATCH:
        agc_erno_set (AGC_ERNO_STRING);

AGC_FINALLY:
        return agc_erno_get ();
}


extern agc_erno
aga_string_replace(aga_string **ctx, const aga_string *what,
                   const aga_string *with)
{
AGC_TRY:
    agc_assert_handle (ctx && *ctx && with);
    agc_assert_string (what);

    if (agc_likely (!strstr (with, what))) {
        agc_try (replace_first (ctx, what, with));

        while (strstr (*ctx, what))
            agc_try (replace_first (ctx, what, with));
    }

    const char placeholder[] = { 0x1, 0x0 };
    agc_try (replace_first (ctx, what, placeholder));
    while (strstr (*ctx, what))
        agc_try (replace_first (ctx, what, placeholder));

    agc_try (replace_first (ctx, placeholder, with));
    while (strstr (*ctx, placeholder))
        agc_try (replace_first (ctx, placeholder, with));

AGC_CATCH:
    agc_erno_set (AGC_ERNO_STRING);

AGC_FINALLY:
    return agc_erno_get ();
}


extern agc_erno
aga_string_replace_first(aga_string **ctx, const aga_string *what,
                         const aga_string *with)
{
AGC_TRY:
    agc_assert_handle (ctx && *ctx && what && with);

    return replace_first (ctx, what, with);

AGC_CATCH:
    agc_erno_set (AGC_ERNO_STRING);

AGC_FINALLY:
    return agc_erno_get ();
}


