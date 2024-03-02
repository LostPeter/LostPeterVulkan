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

#ifndef ZTDEBUG

#ifndef NDEBUG
    #include <stdio.h>
    #include "zthread/Thread.h"

    #define ZTDEBUG(x) {if(Thread::s_isDebugPrint) printf(x);}
    #define ZTDEBUG_PARAM1(x, p1) {if(Thread::s_isDebugPrint) printf(x, p1);}
    #define ZTDEBUG_PARAM2(x, p1, p2) {if(Thread::s_isDebugPrint) printf(x, p1);}
#else
    #define ZTDEBUG(x) 
    #define ZTDEBUG_PARAM1(x, p1)
    #define ZTDEBUG_PARAM2(x, p1, p2) 
#endif

#endif
