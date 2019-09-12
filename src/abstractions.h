#if !defined ARGENT_ABSTRACTIONS
#define ARGENT_ABSTRACTIONS


#include <argent/core.h>


/**************************************************************************//**
 * @defgroup string AgA String Module
 * UTF-8 string management.
 *
 * UTF-8 is the de-facto character representation used nowadays across the globe
 * in computing contexts. In contrast to other Unicode schemes (such as UTF-16),
 * UTF-8 uses variable bytes sizes as required for Unicode code points, the
 * smallest being 1 byte. This makes it both efficient and backward compatible
 * with ASCII.
 *
 * The AgA String Module takes this into account, and uses UTF-8 as its encoding
 * scheme. This module provides the most important string manipulation routines,
 * and its interface is expected to grow over time.
 * @{
 */


/**
 * UTF-8 string.
 *
 * The aga_string type represents a UTF-8 string. An aga_string may be used
 * interchangably with a raw @c char string in most (but not all) cases. The
 * heap memory allocated to AgA string instances is managed internally by the
 * AgM memory pool through the Boehm Garbage Collector.
 */
typedef char aga_string;


/**
 * Create new string.
 *
 * The aga_string_new() function creates a new instance of an AgA string @p ctx
 * from a raw C string @p cstr. The raw string @p cstr may either be an ASCII
 * string or a UTF-8 string.
 *
 * @param ctx The contextual string instance.
 * @param cstr The raw C string.
 *
 * @return AGC_ERNO_NULL if successful.
 * @return AGC_ERNO_STRING if unsuccessful.
 *
 * @note Since AgA strings and raw C strings may be used interchangably in most
 * (but not all) cases, it is also possible to create a new AgA string using the
 * aga_string_copy() function.
 *
 * @warning Don't call the standard @c free() function on the string instance
 * created by this function; the heap memory allocated to the string is managed
 * internally by the AgM Library through the Boehm Garbage Collector.
 *
 * @see aga_string_copy()
 */
extern agc_erno
aga_string_new(aga_string **ctx, const char *cstr);


/**
 * Clone existing string.
 *
 * The aga_string_copy() function creates a string instance @p ctx as a copy of
 * an existing AgA string @p src.
 *
 * @param ctx The contextual string instance.
 * @param src The string to be cloned.
 *
 * @return AGC_ERNO_NULL if successful.
 * @return AGC_ERNO_STRING if unsuccessful.
 *
 * @note Since AgA strings and raw C strings may be used interchangably in most
 * (but not all) cases, it is also possible to create a copy of @p src by using
 * the aga_string_new() function.
 *
 * @warning Don't call the standard @c free() function on the string instance
 * created by this function; the heap memory allocated to the string is managed
 * internally by the AgM Library through the Boehm Garbage Collector.
 */
inline agc_erno
aga_string_copy(aga_string **ctx, const aga_string *src)
{
    return aga_string_new (ctx, src);
}


/**
 * Get string length.
 *
 * The aga_string_len() function computes the lexicographical length of an AgA
 * string @p ctx. The lexicographical length is the number of characters in @p
 * ctx, and this function correctly accounts for the variable size of UTF-8
 * characters.
 *
 * @param ctx The contextual string instance.
 * @param len The lexicographical length.
 *
 * @return AGC_ERNO_NULL if successful.
 * @return AGC_ERNO_STRING if unsuccessful.
 *
 * @warning There are as many bytes as there are characters only in ASCII
 * strings, and this is not necessarily true for UTF-8 strings. Use the
 * aga_string_sz() function instead if you need to compute the number of bytes
 * in @p ctx.
 *
 * @see aga_string_sz()
 */
extern agc_erno
aga_string_len(const aga_string *ctx, size_t *len);


/**
 * Get string size.
 *
 * The aga_string_sz() function computes the size in bytes of an AgA string @p
 * ctx, @b including its terminating null character.
 *
 * @param ctx The contextual string instance.
 * @param xz The size in bytes.
 *
 * @return AGC_ERNO_NULL if successful.
 * @return AGC_ERNO_STRING if unsuccessful.
 *
 * @warning There are as many bytes as there are characters only in ASCII
 * strings, and this is not necessarily true for UTF-8 strings. Use the
 * aga_string_len() function instead if you need to compute the lexicographical
 * length of @p ctx.
 *
 * @see pcr_string_len()
 */
extern agc_erno
aga_string_sz(const aga_string *ctx, size_t *sz);


/**
 * Compare two strings.
 *
 * The aga_string_cmp() function performs a lexicographical comparison of two
 * aga_string instances @p lhs and @p rhs. This function sets @p cmp to one of
 * the same values returned by the standard @c strcmp() function to indicate the
 * comparison result.
 *
 * @param lhs The left hand side string to compare.
 * @param rhs The right hand side string to compare.
 * @param ex The exception stack.
 *
 * @return AGC_ERNO_NULL if successful.
 * @return AGC_ERNO_STRING if unsuccessful.
 *
 * @note The lexicographical comparison is case sensitive, so "HELLO" < "hello".
 */
extern agc_erno
aga_string_cmp(const aga_string *lhs, const aga_string *rhs, int *cmp);


/**
 * Concatenate two strings.
 *
 * The aga_string_add() function concatenates @p add on to a contextual string
 * instace @p ctx.
 *
 * @param ctx The contextual string instance.
 * @param add The string to add.
 *
 * @return AGC_ERNO_NULL if successful.
 * @return AGC_ERNO_STRING if unsuccessful.
 *
 * @warning Don't call the standard @c free() function on the string instance
 * created by this function; the heap memory allocated to the string is managed
 * internally by the AgM Library through the Boehm Garbage Collector.
 */
extern agc_erno
aga_string_add(aga_string **ctx, const aga_string *add);


/**
 * Search for substring.
 *
 * The aga_string_find() function searches for the first instance of a substring
 * @p what in a string @p ctx, and sets @p loc to the lexicographical 1-based
 * index where the substring @p what was first found. In case @p what is not
 * found, then @p loc is set to 0.
 *
 * @param ctx The contextual string instance to search in.
 * @param what The substring to find.
 * @param loc The 1-based index of @p needle in @p ctx.
 *
 * @return AGC_ERNO_NULL if successful.
 * @return AGC_ERNO_STRING if unsuccessful.
 */
extern agc_erno
aga_string_find(const aga_string *ctx, const aga_string *what, size_t *loc);


/**
 * Replace all instances of substring.
 *
 * The aga_string_replace() function replaces @b all instances of a substring @p
 * ctx in a string @p ctx with another substring @p with. The original string @p
 * ctx is overwritten if @p what is found, otherwise it is unaffected.
 *
 * @param ctx The string to search in.
 * @param what The substring to replace.
 * @param with The replacement string.
 *
 * @return The replaced string.
 *
 * @warning Don't call the standard @c free() function on the string instance
 * created by this function; the heap memory allocated to the string is managed
 * internally by the AgM Library through the Boehm Garbage Collector.
 */
extern agc_erno
aga_string_replace(aga_string **ctx, const aga_string *what,
                   const aga_string *with);


/**
 * Replace first instance of substring.
 *
 * The aga_string_replace_first() function replaces the first instace of a
 * substring @p what in a string @p ctx with another substring @p with. The
 * original string @p ctx is overwritten if @p what is found, otherwise it is
 * unaffected.
 *
 * @param ctx The string to search in.
 * @param what The substring to replace.
 * @param with The replacement string.
 *
 * @return AGC_ERNO_NULL if successful.
 * @return AGC_ERNO_STRING if unsuccessful.
 *
 * @warning Don't call the standard @c free() function on the string instance
 * created by this function; the heap memory allocated to the string is managed
 * internally by the AgM Library through the Boehm Garbage Collector.
 *
 * @see aga_string_replace()
 */
extern agc_erno
aga_string_replace_first(aga_string **ctx, const aga_string *what,
                          const aga_string *with);


#endif /* !defined ARGENT_ABSTRACTIONS */

