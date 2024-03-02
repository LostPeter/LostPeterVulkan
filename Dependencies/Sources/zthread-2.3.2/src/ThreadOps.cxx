/*
 * Copyright (c) 2005, Eric Crahen
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#ifndef __ZTTHREADOPSIMPLSELECT_CXX__
#define __ZTTHREADOPSIMPLSELECT_CXX__

#include "ThreadOps.h"

// This file will select an implementation for a ThreadOps based on
// what ThreadOps.h selects. This method is for selecting the 
// source files, to improve portability. Currently, the project is
// based on the autoconf tool-set, which doesn't support conditional
// compilation well. Additionally, this should make the library 
// easier to port since its working around conditional compilation
// by using C++ features and people won't have to fiddle around with
// their make tool as much to compile the source

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

// Check for sched_yield()

#if !defined(HAVE_SCHED_YIELD)
#  if defined(HAVE_UNISTD_H)
#    include <unistd.h>
#    if defined(_POSIX_PRIORITY_SCHEDULING)
#      define HAVE_SCHED_YIELD 1
#    endif
#  endif
#endif

#include ZT_THREADOPS_IMPLEMENTATION

#endif
