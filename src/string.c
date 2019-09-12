#include <string.h>
#include <argent/middleware.h>
#include "abstractions.h"


    /* size offset for terminating null character */
#define NULLCHAR_OFFSET ((ag_size) 1)


    /* checks if a character is a UTF-8 continuation character */
static inline agc_bool
utf8_continuation(agc_word_8 c)
{
        /* see https://gist.github.com/stanislaw/f62c36823242c4ffea1b */
    return (c & 0xC0) == 0x80;
}


    /* determines length of a string, accounting for the possibility that it may
     * contain UTF-8 characters */
static inline agc_size
utf8_strlen(const aga_string *str)
{
    register agc_size i, len;

        /* count number of characters in @p str, skipping UTF-8 continuations */
    i = len = (agc_size) 0;
    while (str[i]) {
        if (!utf8_continuation (str[i]))
            len++;
        i++;
    }

    return len;
}


    /* replaces first instance of a substring in a string; this code is adapted
     * from  https://gist.github.com/stanislaw/f62c36823242c4ffea1b */
static agc_erno
replace_first(aga_string **ctx, const aga_string *what,
              const aga_string *with)
{
    const char *pos;
    const agc_size ctxsz, whatsz, withsz, diffsz, shifts;
    aga_string *tmp;

AGC_TRY:
        /* break out if @p what doesn't exist in @p ctx */
    pos = strstr (*ctx, what);
    if (!pos)
        return AGC_ERNO_NULL;

        /* determine required sizes */
    ctxsz = strlen (*ctx);
    whatsz = strlen (what);
    withsz = strlen (with);
    diffsz = withsz - whatsz;

        /* create temporary buffer */
    tmp = NULL;
    agc_try (agm_mpool_alloc ((void **) &tmp,
                              whatsz + diffsz + NULLCHAR_OFFSET));

        /* copy required characters to temporary buffer, accounting for
         * substring replacement */
    shifts = pos - *ctx;
    memcpy (tmp, *ctx, shifts);
    memcpy (tmp + shifts, with, withsz);
    memcpy (tmp + shifts + withsz, pos + whatsz, ctxsz - shifts - whatsz);

        /* update @p ctx  */
    tmp [ctxsz + diffsz] = '\0';
    *ctx = tmp;

AGC_CATCH:
AGC_FINALLY:
        /* return current error code */
    return agc_erno_get ();
}


    /* sanitises a string pointer, accounting for it possibly being null or
     * empty */
static inline const aga_string *
str_sanitise(const aga_string *s)
{
    return s ? (*s ? s : "(empty)") : "(null)";
}


    /* sanitises a pointer to a string pointer, accounting  for it possibly
     * being null or empty */
static inline const aga_string *
str_sanitise_2(const aga_string **s)
{
    return s && *s ? (**s ? *s : "(empty)") : "(null)";
}


    /* implement aga_string_new() interface function */
extern agc_erno
aga_string_new(aga_string **ctx, const char *cstr)
{
    const agc_size sz;

AGC_TRY:
        /* check preconditions */
    agc_assert_handle (cstr);

        /* allocate memory for @p ctx and copy @p cstr into it */
    sz = strlen (cstr) + NULLCHAR_OFFSET;
    agc_try (agm_mpool_alloc ((void **) ctx, sz));
    (void) strncpy (*ctx, cstr, sz);

AGC_CATCH:
        /* log error details and update error code */
    agm_log_erno ();
    agm_log_error ("failed to create string \'%s\'", str_sanitise (cstr));
    agc_erno_set (AGC_ERNO_STRING);

AGC_FINALLY:
        /* return current error code */
    return agc_erno_get ();
}


    /* declare aga_string_copy() interface function as inline */
extern inline agc_erno
aga_string_copy(aga_string **ctx, const aga_string *src);


    /* implement aga_string_len() interface function */
extern agc_erno
aga_string_len(const aga_string *ctx, size_t *len)
{
AGC_TRY:
        /* check preconditions */
    agc_assert_handle (ctx && len);

        /* determine lexicographical length of @p ctx; we can't use the standard
         * strlen() function because UTF-8 characters are of variable size */
    *len = utf8_strlen (ctx);

AGC_CATCH:
        /* log error details and update error code */
    agm_log_erno ();
    agm_log_error ("failed to get length of string \'%s\'",
                   str_sanitise_2 (ctx));
    agc_erno_set (AGC_ERNO_STRING);

AGC_FINALLY:
        /* return current error code */
    return agc_erno_get ();
}


    /* implement aga_string_sz() interface function */
extern agc_erno
aga_string_sz(const aga_string *ctx, size_t *sz)
{
AGC_TRY:
        /* check preconditions */
    agc_assert_handle (ctx);

        /* determine size of @p ctx; we can use the standard strlen() function
         * in this case because we need to count the individual bytes */
    *sz = strlen (ctx) + NULLCHAR_OFFSET;

AGC_CATCH:
        /* log error details and update error code */
    agm_log_erno ();
    agm_log_error ("failed to get size of string \'%s\'", str_sanitise (ctx));
    agc_erno_set (AGC_ERNO_STRING);

AGC_FINALLY:
        /* return current error code */
    return agc_erno_get ();
}


    /* implement aga_string_cmp() interface function */
extern agc_erno
aga_string_cmp(const aga_string *lhs, const aga_string *rhs, agc_int *cmp)
{
AGC_TRY:
        /* check preconditions */
    agc_assert_handle (lhs && rhs && cmp);

        /* use the standard strcmp() function to compare @p lhs with @p rhs */
    *cmp = strcmp (lhs, rhs);

AGC_CATCH:
        /* log error details and update error code */
    agm_log_erno ();
    agm_log_error ("failed to compare string \'%s\' with \'%s\'",
                   str_sanitise (lhs), str_sanitise (rhs));
    agc_erno_set (AGC_ERNO_STRING);

AGC_FINALLY:
        /* return current error code */
    return agc_erno_get ();
}


    /* implement aga_string_add() interface function */
extern agc_erno
aga_string_add(aga_string **ctx, const aga_string *add)
{
    const agc_size llen, rlen;
    aga_string *cat;

AGC_TRY:
        /* check preconditions */
    agc_assert_handle (ctx && && add);

        /* determine sizes of @p ctx and @p add */
    lsz = strlen (*ctx) + NULLCHAR_OFFSET;
    rsz = strlen (add) + NULLCHAR_OFFSET;

        /* allocate memory for concatenated buffer */
    cat = NULL;
    agc_try (agm_mpool_alloc ((void **) &cat, lsz + rsz));

        /* concatenate @p ctx and @p add; @p ctx may be a handle to an
         * uninitialised string buffer */
    if (*ctx)
        (void) strncpy (cat, *ctx, lsz);
    *ctx = strncat (cat, add, rsz);

AGC_CATCH:
        /* log error details and update error code */
    agm_log_erno ();
    agm_log_error ("failed to add string \'%s\' to \'%s\'", str_sanitise (add),
                   str_sanitise_2 (*ctx));
    agc_erno_set (AGC_ERNO_STRING);

AGC_FINALLY:
        /* return current error code */
    return agc_erno_get ();
}


    /* implement aga_string_find() interface function */
extern agc_erno
aga_string_find(const aga_string *ctx, const aga_string *what, agc_index *loc)
{
    const agc_size offset = 1;
    const aga_string *sub;

AGC_TRY:
        /* check preconditions */
    agc_assert_handle (ctx && what && loc);

        /* determine 1-based lexicographical position index of @p what in @p
         * ctx; 0 indicates that @p what doesn't exist in @p ctx */
    sub = strstr (ctx, what);
    *loc = sub ? (utf8_strlen (ctx) - utf8_strlen (sub) + offset) : 0;

AGC_CATCH:
        /* log error details and update error code */
    agm_log_erno ();
    agm_log_error ("failed to find string \'%s\' in \'%s\')",
                   str_sanitise (ctx), str_sanitise (what));
    agc_erno_set (AGC_ERNO_STRING);

AGC_FINALLY:
        /* return current error code */
    return agc_erno_get ();
}


    /* implement aga_string_replace() interface function */
extern agc_erno
aga_string_replace(aga_string **ctx, const aga_string *what,
                   const aga_string *with)
{
    const char placeholder[] = { 0x1, 0x0 };

AGC_TRY:
        /* check preconditions */
    agc_assert_handle (ctx && *ctx && with);
    agc_assert_string (what);

        /* in the probable case the @p what is not a substring of @p with, make
         * repeated calls to the replace_first() helper function until all
         * instances of @p what in @p ctx have been replaced with @p with */
    if (agc_likely (!strstr (with, what))) {
        agc_try (replace_first (ctx, what, with));
        while (strstr (*ctx, what))
            agc_try (replace_first (ctx, what, with));
    }

        /* if we're here, then we have encountered the edge case of @p what
         * being a substring of @p with; directly calling replace_first() as
         * above would lead to an infinite loop, so we need to first replace all
         * instances of @p what with a placeholder string; this placeholder
         * string contains a single non-printing character, and so is guaranteed
         * never to be encountered in the context of aga_string usage */
    agc_try (replace_first (ctx, what, placeholder));
    while (strstr (*ctx, what))
        agc_try (replace_first (ctx, what, placeholder));

        /* now replace all instances of the placeholder string with @p with */
    agc_try (replace_first (ctx, placeholder, with));
    while (strstr (*ctx, placeholder))
        agc_try (replace_first (ctx, placeholder, with));

AGC_CATCH:
        /* log error details and update error code */
    agm_log_erno ();
    agm_log_error ("failed to replace string \'%s\' in \'%s\' with \'%s\'",
                   str_sanitise (what), str_sanitise_2 (*ctx),
                   str_sanitise (with));
    agc_erno_set (AGC_ERNO_STRING);

AGC_FINALLY:
        /* return current error code */
    return agc_erno_get ();
}


    /* implement aga_string_replace_first() interface function */
extern agc_erno
aga_string_replace_first(aga_string **ctx, const aga_string *what,
                         const aga_string *with)
{
AGC_TRY:
        /* check preconditions */
    agc_assert_handle (ctx && *ctx && what && with);

        /* use the replace_first() helper function to replace the first instance
         * of @p what in @p ctx with @p with */
    agc_try (replace_first (ctx, what, with))

AGC_CATCH:
        /* log error details and update error code */
    agm_log_erno ();
    agm_log_error ("failed to replace string \'%s\' in \'%s\' with \'%s\'",
                   str_sanitise (what), str_sanitise_2 (*ctx),
                   str_sanitise (with));
    agc_erno_set (AGC_ERNO_STRING);

AGC_FINALLY:
        /* return current error code */
    return agc_erno_get ();
}


