
    /* you need to include the <argent/abstractions.h> header file */
#include <stdio.h>
#include <argent/abstractions.h>


int
main(void)
{
    const aga_string *S1, *S2;
    aga_string *s1, s2, s3, c1, c2;
    agc_size len, sz;
    agc_index loc;
    agc_int cmp;

AGC_TRY:
        /* this is how you could create two strings on the stack. */
    S1 = "Hello, world!";
    S2 = "Привет, мир!";

        /* this is now you could create two strings on the heap. */
    s1 = s2 = NULL;
    agc_try (aga_string_new (&s1, "Hello, world!");
    agc_try (aga_string_new (&s2, "Привет, мир!");

        /* this is how you would make copies of strings. */
    c1 = c2 = NULL;
    agc_try (aga_string_copy (&c1, s1));
    agc_try (aga_string_copy (&c2, "Привет, мир!"));

        /* this is how would determine the length and size of strings. */
    agc_try (aga_string_len (c1, &len));
    agc_try (aga_string_sz (c2, &sz));

        /* this is how you would compare two strings for equality. */A
    agc_try (aga_string_cmp (s1, S1, &cmp));
    if (!cmp)
        printf("s1 and S1 are equal!\n");

        /* this is how you would check if a string is lexicographically less
         * than another. */
    agc_try (aga_string_cmp (s2, S2, &cmp));
    if (cmp < 1)
        printf("s2 < S2\n");

        /* this is how you would check if a string is lexicographically greater
         * than another. */
    agc_try (aga_string_cmp (c1, c2, &cmp));
    if (cmp > 1)
        printf("c1 > c2\n");

        /* this is how you would add two strings. */
    s3 = NULL;
    agc_try (aga_string_add(s3, s2));
    agc_try (aga_string_add(s3, s1));

        /* this is how you would search for the first instance of a substring in
         * a string. */
    agc_try (aga_string_find (s3, s2, &loc));
    printf("s2 found in s3 at index %lu", loc);

        /* this is how you would replace the first instance of a substring in a
         * string. */
    agc_try (aga_string_replace_first (s3, s1, "replaced text"));

        /* this is how you would replace all instances of a substring in a
         * string. */
    agc_try (aga_string_replace (s3, s1, "foo"));

AGC_CATCH:
    agm_erno_log ();
    agm_error_log ("error in running string example");

AGC_FINALLY:
    return 0;
}

