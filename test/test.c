
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

#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>
#include <ctype.h>

#include "../xprintf.h"

static int check( int len1, char const* str1, int len2, char const* str2, int size ) {
    enum { margin = 15 };
    if ( len1 != len2 ) {
        puts( "FAILED" );
        printf( "%*s'%d'\n",   margin, "-Len 1:", len1 );
        printf( "%*s'%d'\n\n", margin, "-Len 2:", len2 );
        printf( "%*s'%.*s'\n",   margin, "-String 1:", len1, str1 );
        printf( "%*s'%.*s'\n\n", margin, "-String 2:", len2, str2 );
        return -1;
    }
    if ( 0 != strcmp( str1, str2 ) ) {
        puts( "FAILED" );
        printf( "%*s'%s'\n",   margin, "-String 1:", str1 );
        printf( "%*s'%s'\n\n", margin, "-String 2:", str2 );
        return -2;
    }
    if ( 0 != memcmp( str1, str2, size ) ) {
        puts( "FAILED" );
        printf( "%*s'%.*s'\n",   margin, "-Padding 1:", size - len1 - 1, str1 + len1 + 1 );
        printf( "%*s'%.*s'\n\n", margin, "-Padding 2:", size - len2 - 1, str2 + len2 + 1 );
        return -3;
    }
    puts( "PASSED" );
    return 0;
}

int main( void ) {

    enum {
        verbose = 0,
        wval    = 16,
        wfmt    = 10,
        wrslt   = 8
    };

    int total = 0;
    int passed = 0;

    printf( "\n%-*s%-*s%s\n", wfmt, "Format", wval, "Value", "Result" );
    for( int i = 0; i < wfmt + wval + wrslt; ++i )
        putc( '-', stdout );
    putc('\n', stdout );

    char str1[ 128 ];
    char str2[ sizeof str1 ];

    {
        static char const* const data [] = { "abcdefg" };
        static char const* const fmt [] = {
            "Text: %s", "%10s", "%-10s", "%.2s", "%.10s", "%10.2s", "%10.2s"
        };
        for( int i = 0; i < sizeof fmt / sizeof *fmt; ++i ) {
            for( int j = 0; j < sizeof data / sizeof *data; ++j ) {
                memset( str1, 'R', sizeof str1 );
                memset( str2, 'R', sizeof str2 );
                printf( "%-*s%-*s", wfmt, fmt[i], wval, data[j] );
                int len1 =  sprintf( str1, fmt[i], data[j] );
                int len2 = xsprintf( str2, fmt[i], data[j] );
                int err = check( len1, str1, len2, str2, sizeof str1 );
                passed += 0 == err;
                ++total;
            }
        }
    }

    {
        memset( str1, 'R', sizeof str1 );
        memset( str2, 'R', sizeof str2 );
        static char const fmt [] = "Hi %s%n.";
        static char const data [] = "Perry";
        printf( "%-*s%-*s", wfmt, fmt, wval, data );
        int n1 = -1;
        int len1 =  sprintf( str1, fmt, data, &n1 );
        int n2 = -1;
        int len2 = xsprintf( str2, fmt, data, &n2 );
        int err = 0;
        if ( n1 != n2 || 0 > n1 ) {
            puts( "FAILED" );
            err = 1;
        }
        else
            err = check( len1, str1, len2, str2, sizeof str1 );
        passed += 0 == err;
        ++total;
    }

    {
        static int const data [] = { -12345, 12345, 0, 98, -98 };
        static char const* const fmt [] = {
            "%d", "%7d", "%07d", "%-7d", "%+d", "%.4d", "%.4d", "%+5.4d"
        };
        for( int i = 0; i < sizeof fmt / sizeof *fmt; ++i ) {
            for( int j = 0; j < sizeof data / sizeof *data; ++j ) {
                memset( str1, 'R', sizeof str1 );
                memset( str2, 'R', sizeof str2 );
                printf( "%-*s%-*d", wfmt, fmt[i], wval, data[j] );
                int len1 =  sprintf( str1, fmt[i], data[j] );
                int len2 = xsprintf( str2, fmt[i], data[j] );
                int err = check( len1, str1, len2, str2, sizeof str1 );
                passed += 0 == err;
                ++total;
            }
        }
    }

    {
        static size_t const data [] = { 12345, 0, 98 };
        static char const* const fmt [] = {
            "%zd", "%7zd", "%07zd", "%-7zd", "%+zd", "%.4zd", "%.4zd", "%+5.4zd"
        };
        for( int i = 0; i < sizeof fmt / sizeof *fmt; ++i ) {
            for( int j = 0; j < sizeof data / sizeof *data; ++j ) {
                memset( str1, 'R', sizeof str1 );
                memset( str2, 'R', sizeof str2 );
                printf( "%-*s%-*ld", wfmt, fmt[i], wval, data[j] );
                int len1 =  sprintf( str1, fmt[i], data[j] );
                int len2 = xsprintf( str2, fmt[i], data[j] );
                int err = check( len1, str1, len2, str2, sizeof str1 );
                passed += 0 == err;
                ++total;
            }
        }
    }

    {
        static long int const data [] = { -12345, 12345, 0, 98, -98 };
        static char const* const fmt [] = {
            "%ld", "%7ld", "%07ld", "%-7ld", "%+ld", "%.4ld", "%.4ld", "%+5.4ld"
        };
        for( int i = 0; i < sizeof fmt / sizeof *fmt; ++i ) {
            for( int j = 0; j < sizeof data / sizeof *data; ++j ) {
                memset( str1, 'R', sizeof str1 );
                memset( str2, 'R', sizeof str2 );
                printf( "%-*s%-*ld", wfmt, fmt[i], wval, data[j] );
                int len1 =  sprintf( str1, fmt[i], data[j] );
                int len2 = xsprintf( str2, fmt[i], data[j] );
                int err = check( len1, str1, len2, str2, sizeof str1 );
                passed += 0 == err;
                ++total;
            }
        }
    }

    {
        static long long int const data [] = { -12345, 12345, 0, 98, -98 };
        static char const* const fmt [] = {
            "%lld", "%7lld", "%07lld", "%-7lld", "%+lld", "%.4lld", "%.4lld", "%+5.4lld"
        };
        for( int i = 0; i < sizeof fmt / sizeof *fmt; ++i ) {
            for( int j = 0; j < sizeof data / sizeof *data; ++j ) {
                memset( str1, 'R', sizeof str1 );
                memset( str2, 'R', sizeof str2 );
                printf( "%-*s%-*lld", wfmt, fmt[i], wval, data[j] );
                int len1 =  sprintf( str1, fmt[i], data[j] );
                int len2 = xsprintf( str2, fmt[i], data[j] );
                int err = check( len1, str1, len2, str2, sizeof str1 );
                passed += 0 == err;
                ++total;
            }
        }
    }

    {
        static unsigned int const data [] = { 12345, 0, 98 };
        static char const* const fmt [] = {
            "%u", "%7u", "%07u", "%-7u", "%+u", "%.4u", "%.4u", "%+5.4u"
        };
        for( int i = 0; i < sizeof fmt / sizeof *fmt; ++i ) {
            for( int j = 0; j < sizeof data / sizeof *data; ++j ) {
                memset( str1, 'R', sizeof str1 );
                memset( str2, 'R', sizeof str2 );
                printf( "%-*s%-*u", wfmt, fmt[i], wval, data[j] );
                int len1 =  sprintf( str1, fmt[i], data[j] );
                int len2 = xsprintf( str2, fmt[i], data[j] );
                int err = check( len1, str1, len2, str2, sizeof str1 );
                passed += 0 == err;
                ++total;
            }
        }
    }

    {
        static size_t const data [] = { 12345, 0, 98 };
        static char const* const fmt [] = {
            "%zu", "%7zu", "%07zu", "%-7zu", "%+zu", "%.4zu", "%.4zu", "%+5.4zu"
        };
        for( int i = 0; i < sizeof fmt / sizeof *fmt; ++i ) {
            for( int j = 0; j < sizeof data / sizeof *data; ++j ) {
                memset( str1, 'R', sizeof str1 );
                memset( str2, 'R', sizeof str2 );
                printf( "%-*s%-*lu", wfmt, fmt[i], wval, data[j] );
                int len1 =  sprintf( str1, fmt[i], data[j] );
                int len2 = xsprintf( str2, fmt[i], data[j] );
                int err = check( len1, str1, len2, str2, sizeof str1 );
                passed += 0 == err;
                ++total;
            }
        }
    }

    {
        static unsigned long int const data [] = { 12345, 0, 98 };
        static char const* const fmt [] = {
            "%lu", "%7lu", "%07lu", "%-7lu", "%+lu", "%.4lu", "%.4lu", "%+5.4lu"
        };
        for( int i = 0; i < sizeof fmt / sizeof *fmt; ++i ) {
            for( int j = 0; j < sizeof data / sizeof *data; ++j ) {
                memset( str1, 'R', sizeof str1 );
                memset( str2, 'R', sizeof str2 );
                printf( "%-*s%-*lu", wfmt, fmt[i], wval, data[j] );
                int len1 =  sprintf( str1, fmt[i], data[j] );
                int len2 = xsprintf( str2, fmt[i], data[j] );
                int err = check( len1, str1, len2, str2, sizeof str1 );
                passed += 0 == err;
                ++total;
            }
        }
    }

    {
        static unsigned long long int const data [] = { 12345, 0, 98 };
        static char const* const fmt [] = {
            "%llu", "%7llu", "%07llu", "%-7llu", "%+llu", "%.4llu", "%.4llu", "%+5.4llu"
        };
        for( int i = 0; i < sizeof fmt / sizeof *fmt; ++i ) {
            for( int j = 0; j < sizeof data / sizeof *data; ++j ) {
                memset( str1, 'R', sizeof str1 );
                memset( str2, 'R', sizeof str2 );
                printf( "%-*s%-*llu", wfmt, fmt[i], wval, data[j] );
                int len1 =  sprintf( str1, fmt[i], data[j] );
                int len2 = xsprintf( str2, fmt[i], data[j] );
                int err = check( len1, str1, len2, str2, sizeof str1 );
                passed += 0 == err;
                ++total;
            }
        }
    }

    {
        static unsigned int const data [] = { 12345, 0, 98, 1 };
        static char const* const fmt [] = {
            "%X", "%x", "%#x", "%7x", "%#7x", "%07x", "%-7x", "%+x", "%.4x", "%.4x", "%+5.4x"
        };
        for( int i = 0; i < sizeof fmt / sizeof *fmt; ++i ) {
            for( int j = 0; j < sizeof data / sizeof *data; ++j ) {
                memset( str1, 'R', sizeof str1 );
                memset( str2, 'R', sizeof str2 );
                printf( "%-*s%-*u", wfmt, fmt[i], wval, data[j] );
                int len1 =  sprintf( str1, fmt[i], data[j] );
                int len2 = xsprintf( str2, fmt[i], data[j] );
                int err = check( len1, str1, len2, str2, sizeof str1 );
                passed += 0 == err;
                ++total;
            }
        }
    }

    {
        static size_t const data [] = { 12345, 0, 98, 1 };
        static char const* const fmt [] = {
            "%zX", "%zx", "%#zx", "%7zx", "%07zx", "%-7zx", "%+zx", "%.4zx", "%.4zx", "%+5.4zx"
        };
        for( int i = 0; i < sizeof fmt / sizeof *fmt; ++i ) {
            for( int j = 0; j < sizeof data / sizeof *data; ++j ) {
                memset( str1, 'R', sizeof str1 );
                memset( str2, 'R', sizeof str2 );
                printf( "%-*s%-*lu", wfmt, fmt[i], wval, data[j] );
                int len1 =  sprintf( str1, fmt[i], data[j] );
                int len2 = xsprintf( str2, fmt[i], data[j] );
                int err = check( len1, str1, len2, str2, sizeof str1 );
                passed += 0 == err;
                ++total;
            }
        }
    }

    {
        static unsigned long int const data [] = { 12345, 0, 98, 1 };
        static char const* const fmt [] = {
            "%lX", "%lx", "%#lx", "%7lx", "%07lx", "%-7lx", "%+lx", "%.4lx", "%.4lx", "%+5.4lx"
        };
        for( int i = 0; i < sizeof fmt / sizeof *fmt; ++i ) {
            for( int j = 0; j < sizeof data / sizeof *data; ++j ) {
                memset( str1, 'R', sizeof str1 );
                memset( str2, 'R', sizeof str2 );
                printf( "%-*s%-*lu", wfmt, fmt[i], wval, data[j] );
                int len1 =  sprintf( str1, fmt[i], data[j] );
                int len2 = xsprintf( str2, fmt[i], data[j] );
                int err = check( len1, str1, len2, str2, sizeof str1 );
                passed += 0 == err;
                ++total;
            }
        }
    }

    {
        static unsigned long long int const data [] = { 12345, 0, 98, 1 };
        static char const* const fmt [] = {
            "%llX", "%llx", "%#llx", "%7llx", "%07llx", "%-7llx", "%+llx", "%.4llx", "%.4llx", "%+5.4llx"
        };
        for( int i = 0; i < sizeof fmt / sizeof *fmt; ++i ) {
            for( int j = 0; j < sizeof data / sizeof *data; ++j ) {
                memset( str1, 'R', sizeof str1 );
                memset( str2, 'R', sizeof str2 );
                printf( "%-*s%-*llu", wfmt, fmt[i], wval, data[j] );
                int len1 =  sprintf( str1, fmt[i], data[j] );
                int len2 = xsprintf( str2, fmt[i], data[j] );
                int err = check( len1, str1, len2, str2, sizeof str1 );
                passed += 0 == err;
                ++total;
            }
        }
    }

    {
        static unsigned char const data [] = { 'a' };
        static char const* const fmt [] = {
            "%c", "%10c", "%-10c", "%.2c", "%.10c", "%10.2c"
        };
        for( int i = 0; i < sizeof fmt / sizeof *fmt; ++i ) {
            for( int j = 0; j < sizeof data / sizeof *data; ++j ) {
                memset( str1, 'R', sizeof str1 );
                memset( str2, 'R', sizeof str2 );
                printf( "%-*s%-*c", wfmt, fmt[i], wval, data[j] );
                int len1 =  sprintf( str1, fmt[i], data[j] );
                int len2 = xsprintf( str2, fmt[i], data[j] );
                int err = check( len1, str1, len2, str2, sizeof str1 );
                passed += 0 == err;
                ++total;
            }
        }
    }

    {
        static char const array [] = "array";
        static int const arraylen = sizeof array - 1;
        static char const* const fmt[] = { "%.*s", "%*s", "%-*s" };
        for( int i = 0; i < sizeof fmt / sizeof *fmt; ++i ) {
            memset( str1, 'R', sizeof str1 );
            memset( str2, 'R', sizeof str2 );
            printf( "%-*s%-*.*s", wfmt, fmt[i], wval, arraylen, array );
            int len1 =  sprintf( str1, fmt[i], arraylen, array );
            int len2 = xsprintf( str2, fmt[i], arraylen, array );
            int err = check( len1, str1, len2, str2, sizeof str1 );
            passed += 0 == err;
            ++total;
        }
    }

    {
        static void* const data [] = { (void*)12345, (void*)0, (void*)98, (void*)1 };
        static char const* const fmt [] = {
            "%p", "%#p", "%7p", "%07p", "%-7p", "%.4p", "%.4p"
        };
        for( int i = 0; i < sizeof fmt / sizeof *fmt; ++i ) {
            for( int j = 0; j < sizeof data / sizeof *data; ++j ) {
                memset( str1, 'R', sizeof str1 );
                memset( str2, 'R', sizeof str2 );
                printf( "%-*s%-*p", wfmt, fmt[i], wval, data[j] );
                int len1 =  sprintf( str1, fmt[i], data[j] );
                int len2 = xsprintf( str2, fmt[i], data[j] );
                int err = check( len1, str1, len2, str2, sizeof str1 );
                passed += 0 == err;
                ++total;
            }
        }
    }

    printf( "\nTest passed: [ %d / %d ]\n", passed, total );
    int failed = total - passed;
    printf( "\n%s%s\n\n", "Test suit result: ", failed ? "FAILED" : "PASSED" );
    return failed;
}
