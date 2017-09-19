/*------------------------------------------------------------------
 * strnset_s.c
 *
 * September 2017, Reini Urban
 *
 * Copyright (c) 2017 by Reini Urban
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

#include "safeclib_private.h"

/**
 * @brief
 *    Sets maximal n characters of dest to a character value,
 *    but not the final NULL character.
 *    With SAFECLIB_STR_NULL_SLACK defined all elements following the
 *    terminating null character (if any) written in the
 *    array of dmax characters pointed to by dest are nulled.
 *
 * @remark EXTENSION TO
 *    * ISO/IEC TR 24731, Programming languages, environments
 *    and system software interfaces, Extensions to the C Library,
 *    Part I: Bounds-checking interfaces
 *    * Implemented in the Windows secure API as _strnset_s()
 *
 * @param[out]  dest    string that will be set.
 * @param[in]   dmax    restricted maximum length of dest
 * @param[in]   value   character value to write
 * @param[in]   n       number of characters to be written
 *
 * @pre dest shall not be a null pointer, and shall be null-terminated.
 * @pre dmax shall not be greater than RSIZE_MAX_STR.
 * @pre dmax shall not equal zero.
 * @pre n shall not be greater than dmax
 * @pre value shall not be greater than 255
 *
 * @retval  EOK         when successful
 * @retval  ESNULLP     when dest is NULL pointer
 * @retval  ESZEROL     when dmax = 0
 * @retval  ESLEMAX     when dmax > RSIZE_MAX_STR or value > 255
 * @retval  ESNOSPC     when n > dmax
 *
 * @see
 *    strzero_s(), strset_s(), wcsnset_s(), strispassword_s()
 *
 */
EXPORT errno_t
strnset_s (char *restrict dest, rsize_t dmax, int value, rsize_t n)
{
#ifdef SAFECLIB_STR_NULL_SLACK
    char * orig_dest;
#endif

    if (unlikely(dest == NULL)) {
        invoke_safe_str_constraint_handler("strnset_s: dest is null",
                   NULL, ESNULLP);
        return (ESNULLP);
    }

    if (unlikely(dmax == 0)) {
        invoke_safe_str_constraint_handler("strnset_s: dmax is 0",
                   NULL, ESZEROL);
        return (ESZEROL);
    }

    if (unlikely(dmax > RSIZE_MAX_STR || value > 255)) {
        invoke_safe_str_constraint_handler("strnset_s: dmax/value exceeds max",
                   NULL, ESLEMAX);
        return (ESLEMAX);
    }

    if (unlikely(n > dmax)) {
        invoke_safe_str_constraint_handler("strnset_s: n exceeds dmax",
                   NULL, ESNOSPC);
        return (ESNOSPC);
    }
        
#ifdef SAFECLIB_STR_NULL_SLACK
    orig_dest = dest;
#endif
    while (n && *dest) {
        *dest = (char)value;
        n--;
        dest++;
    }
#ifdef SAFECLIB_STR_NULL_SLACK
    /* null slack to clear any data */
    if (!*dest)
        memset(dest, 0, dmax-(dest-orig_dest));
#endif

    return (EOK);
}
