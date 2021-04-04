# xprintf

[![Build Status](https://travis-ci.org/rafagafe/xprintf.svg?branch=master)](https://travis-ci.org/rafagafe/xprintf)

The xprintf module is an alternative implementation of the printf family of the standard library of C.

The main feature of xprintf is that it lets you __print in user defined output stream__. In this way the user can print directly on a serial port or on an RTOS queue.

* It supports almost all printf functionality except floating point.
* It is a very light implementation. It is suitable for embedded systems.
* It has a __low stack usage__ profile. It is suitable for multi-thread systems.
* Supports several compilation options to reduce program memory by eliminating features.

# Example 

We have a queue module with this interface:

```C

// File: queue.h

struct queue;

void queue_push( struct queue* queue, unsigned char byte );

```

We need print in this queue with a function like this:

```C

// File: qprintf.h

int qprintf( struct queue* queue, char const* fmt, ... );

```

We implement the `qprintf` function by using the `xvprintf` function:

```C

// File: qprintf.c

#include "xprintf.h"

/* Callback function for user defined output stream. */
static void print2queue( void* p, void const* src, int len ) {
    unsigned char* data = (unsigned char*)src;
    struct queue* queue = (struct queue*)p;
    for( int i = 0; i < len; ++i )
        queue_push( queue, data[i] );
}

/* Print formatted data to a queue. */
int qprintf( struct queue* queue, char const* fmt, ... ) {
    struct ostrm const ostrm = {
        .p    = (void*)queue,
        .func = print2queue
    };
    va_list va;
    va_start( va, fmt );
    int const rslt = xvprintf( &ostrm, fmt, va );
    va_end( va );
    return rslt;
}

```
