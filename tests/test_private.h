/*------------------------------------------------------------------
 * test_private.h - Internal test routines & library references
 *
 * 2012, Jonathan Toppins <jtoppins@users.sourceforge.net>
 * 2017  Reini Urban
 *
 * Copyright (c) 2012, 2013 by Cisco Systems, Inc
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *------------------------------------------------------------------
 */

#ifndef __TEST_PRIVATE_H__
#define __TEST_PRIVATE_H__

#include "config.h"

#ifdef __KERNEL__

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/ctype.h>

#include <linux/string.h>
#ifdef HAVE_C99
#define printf(...) printk(KERN_INFO __VA_ARGS__)
#endif

#else

/* for glibc use the GNU extensions: strcasestr */
#undef _GNU_SOURCE
#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>

#endif

/* ISO C++ forbids converting a string constant to 'char*' [-Wwrite-strings]
   rc = strtolowercase_s("test", len); */
#ifdef __cplusplus
# ifdef __clang
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wwrite-strings"
# elif defined(__GNUC__)
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wwrite-strings"
# endif
#endif

#ifdef DEBUG
#  if defined(HAVE_C99) && defined(__KERNEL__)
#    define debug_printf(...)  printk(KERN_DEBUG __VA_ARGS__)
#  else
#    define debug_printf printf
#  endif
#else
# ifdef HAVE_C99
#   define debug_printf(...)
# else
#   define debug_printf printf
# endif
#endif

#define ERR(n)                                     \
    if (rc != (n)) {                               \
        debug_printf("%s %u  Error rc=%d \n",     \
                     __FUNCTION__, __LINE__,  (int)rc); \
        errs++;                                    \
    }
#define ERRNO(n)                                   \
    if (errno != (n)) {                            \
        debug_printf("%s %u  Error errno=%d \n",  \
                     __FUNCTION__, __LINE__,  (int)errno); \
        errs++;                                    \
    }
#define ERREOF(n)                                  \
    ERR(EOF);                                      \
    if (errno != (n)) {                            \
        debug_printf("%s %u  Error errno=%d \n",  \
                     __FUNCTION__, __LINE__,  (int)errno); \
        errs++;                                    \
    }
#define NOERR()                                    \
    if (rc < 0) {                                  \
        debug_printf("%s %u  Error rc=%d \n",     \
                     __FUNCTION__, __LINE__,  (int)rc); \
        errs++;                                    \
    }
#define NOERRNULL()                                \
    if (rc <= 0) {                                 \
        debug_printf("%s %u  Error rc=%d \n",     \
                     __FUNCTION__, __LINE__,  (int)rc); \
        errs++;                                    \
    }
#define EXPNULL(str1)                              \
    if ((str1)[0] != '\0') {                       \
        debug_printf("%s %u  Expected null, got \"%s\" \n", \
                     __FUNCTION__, __LINE__, str1); \
        errs++;                                    \
    }
#define EXPSTR(str1, str2)                         \
    ind = strcmp(str1, str2);                      \
    if (ind != 0) {                                \
        debug_printf("%s %u  Expected \"%s\", got \"%s\" \n", \
                     __FUNCTION__, __LINE__,  str2, str1);  \
        errs++;                                    \
    }
#define WEXPNULL(str1)                             \
    if ((str1)[0] != L'\0') {                      \
        debug_printf("%s %u  Expected null, got L\"%ls\" \n", \
                     __FUNCTION__, __LINE__, str1); \
        errs++;                                    \
    }
#define WEXPSTR(str1, str2)                        \
    ind = wcscmp(str1, str2);                      \
    if (ind != 0) {                                \
        debug_printf("%s %u  Expected L\"%ls\", got L\"%ls\" \n", \
                     __FUNCTION__, __LINE__,  str2, str1);  \
        errs++;                                    \
    }
#define INDZERO()                                  \
    if (ind != 0) {                                \
        printf("%s %u  Error  ind=%d rc=%d \n",    \
               __FUNCTION__, __LINE__, (int)ind, rc);   \
        errs++;                                    \
    }
#define INDCMP(cmp)                                \
  if ((int)ind cmp) {                              \
        printf("%s %u  Error  ind=%d rc=%d \n",    \
               __FUNCTION__, __LINE__, (int)ind, rc);   \
        errs++;                                    \
    }
#define SUBNULL()                                  \
    if (sub) {                                     \
        printf("%s %u  Error  sub=\"%s\" rc=%d \n",    \
                     __FUNCTION__, __LINE__, sub, rc); \
        errs++;                                    \
    }
#define SUBNN()                                     \
    if (!sub) {                                     \
        printf("%s %u  Error  sub=null errno=%d \n",\
                     __FUNCTION__, __LINE__, errno); \
        errs++;                                     \
    }
#define PTREQ(str1, str2)                          \
    if (str1 != str2) {                            \
        debug_printf("%s %u  Expected \"%s\", got \"%s\" \n", \
                     __FUNCTION__, __LINE__,  str2, str1);  \
        errs++;                                    \
    }

#define SETLOCALE_C \
    lc_cat = setlocale(LC_CTYPE, "C")

#define SETLOCALE_UTF8                                  \
    lc_cat = setlocale(LC_CTYPE, "en_US.UTF-8");        \
    if (!lc_cat)                                        \
        lc_cat = setlocale(LC_CTYPE, "en_GB.UTF-8");    \
    if (!lc_cat)                                        \
        lc_cat = setlocale(LC_CTYPE, "en.UTF-8");       \
    if (!lc_cat)                                        \
        lc_cat = setlocale(LC_CTYPE, "POSIX.UTF-8");    \
    if (!lc_cat)                                        \
        lc_cat = setlocale(LC_CTYPE, "C.UTF-8");        \
    if (!lc_cat)                                        \
        lc_cat = setlocale(LC_CTYPE, "UTF-8");          \
    if (!lc_cat)                                        \
        lc_cat = setlocale(LC_CTYPE, "")

#ifdef HAVE_LANGINFO_H
#define SETLANG(l) lang = nl_langinfo(CODESET)
#else
#define SETLANG(l) lang = (l)
#endif

/* not fatal */
#define CHKLOCALE_C \
    debug_printf(__FILE__ ": set C locale "     \
                 "lc_cat=%s, codeset=%s\n", lc_cat, lang); \
    if ( !strcmp(lang, "C") ||                  \
         !strcmp(lang, "ASCII") ||              \
         !strcmp(lang, "ANSI_X3.4-1968") ||     \
         !strcmp(lang, "US-ASCII") )            \
        ; /* all fine */                        \
    else /* dont inspect the values */          \
        printf(__FILE__ ": cannot set C locale for test" \
               " (lc_cat=%s, codeset=%s)\n", lc_cat, lang)


#if !(defined(__STDC_WANT_LIB_EXT1__) && (__STDC_WANT_LIB_EXT1__ >= 1))
#  define NO_C11
#endif

#endif /* __TEST_PRIVATE_H__ */
