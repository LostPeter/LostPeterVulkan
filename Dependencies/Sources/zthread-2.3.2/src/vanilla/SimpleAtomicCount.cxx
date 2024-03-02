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

#ifndef __ZTATOMICCOUNTIMPL_H__
#define __ZTATOMICCOUNTIMPL_H__

#include "zthread/AtomicCount.h"
#include "zthread/Guard.h"
#include "../FastLock.h"

#include <assert.h>

namespace ZThread {

typedef struct atomic_count_t {

  FastLock lock;
  unsigned long count;
  
  atomic_count_t() : count(0) {}

} ATOMIC_COUNT;

AtomicCount::AtomicCount() {

  ATOMIC_COUNT* c = new ATOMIC_COUNT;
  _value = reinterpret_cast<void*>(c);
  
}

AtomicCount::~AtomicCount() {

  ATOMIC_COUNT* c = reinterpret_cast<ATOMIC_COUNT*>(_value);
  assert(c->count == 0);

  delete c;

}
  
//! Postfix decrement and return the current value
size_t AtomicCount::operator--(int) {

  ATOMIC_COUNT* c = reinterpret_cast<ATOMIC_COUNT*>(_value);
  
  Guard<FastLock> g(c->lock);
  return c->count--;

}
  
//! Postfix increment and return the current value
size_t AtomicCount::operator++(int) {

  ATOMIC_COUNT* c = reinterpret_cast<ATOMIC_COUNT*>(_value);
  
  Guard<FastLock> g(c->lock);
  return c->count++;

}

//! Prefix decrement and return the current value
size_t AtomicCount::operator--() {

  ATOMIC_COUNT* c = reinterpret_cast<ATOMIC_COUNT*>(_value);
  
  Guard<FastLock> g(c->lock);
  return --c->count;

}
  
//! Prefix increment and return the current value
size_t AtomicCount::operator++() {
  
  ATOMIC_COUNT* c = reinterpret_cast<ATOMIC_COUNT*>(_value);
  
  Guard<FastLock> g(c->lock);
  return ++c->count;

}

};

#endif // __ZTATOMICCOUNTIMPL_H__
