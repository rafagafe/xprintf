
/*
<https://github.com/rafagafe/xprintf>

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
  Copyright (c) 2016-2018 Rafa Garcia <rafagarcia77@gmail.com>.
  Permission is hereby  granted, free of charge, to any  person obtaining a copy
  of this software and associated  documentation files (the "Software"), to deal
  in the Software  without restriction, including without  limitation the rights
  to  use, copy,  modify, merge,  publish, distribute,  sublicense, and/or  sell
  copies  of  the Software,  and  to  permit persons  to  whom  the Software  is
  furnished to do so, subject to the following conditions:
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
  THE SOFTWARE  IS PROVIDED "AS  IS", WITHOUT WARRANTY  OF ANY KIND,  EXPRESS OR
  IMPLIED,  INCLUDING BUT  NOT  LIMITED TO  THE  WARRANTIES OF  MERCHANTABILITY,
  FITNESS FOR  A PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN NO EVENT  SHALL THE
  AUTHORS  OR COPYRIGHT  HOLDERS  BE  LIABLE FOR  ANY  CLAIM,  DAMAGES OR  OTHER
  LIABILITY, WHETHER IN AN ACTION OF  CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE  OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.

*/

#ifndef XPRINTF_H
#define XPRINTF_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdarg.h>

/** @defgroup xprintf
  * @{ */

/** Holds a user defined output stream */
struct ostrm {
    /** Parameter to pass the the function. */
    void* p;
    /** Send a memory block ta a user defined stream.
      * @param p   Parameter.
      * @param src Source memory block.
      * @param len Length in bytes of the memory block. */
    void(*func)(void*p,void const*src,int len);
};

/** Print formatted data to a user defined stream. */
int xprintf( struct ostrm const* o, char const* fmt, ... ) __attribute__ ((format (printf, 2, 3)));

/** Print formatted data to a user defined stream. */
int xvprintf( struct ostrm const* o, char const* fmt, va_list va );

/** Print formatted data to a buffer. */
int xsprintf( char* buff, char const* fmt, ... ) __attribute__ ((format (printf, 2, 3)));

/** Print formatted data to a file. */
int xfprintf( FILE* fd, char const* fmt, ... ) __attribute__ ((format (printf, 2, 3)));

/** @ } */

#ifdef __cplusplus
}
#endif

#endif /* XPRINTF_H */