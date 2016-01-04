/*
 * The MIT License (MIT)
 * Copyright (c) 2015 Peter Vanusanik <admin@en-circle.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy 
 * of this software and associated documentation files (the "Software"), to deal in 
 * the Software without restriction, including without limitation the rights to use, copy, 
 * modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
 * and to permit persons to whom the Software is furnished to do so, subject to the 
 * following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies 
 * or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS 
 * OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN 
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * _commons.h
 *  Created on: Jan 4, 2016
 *      Author: Peter Vanusanik
 *  Contents: 
 */

#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

extern void* malloc(size_t n);
extern void* calloc(size_t num, size_t n);
extern void* realloc(void* a, size_t n);
extern void  free(void* a);
extern int   strcmp(const char* a, const char* b);
extern void  memset(char* c, int v, size_t s);

#ifndef REGISTER_UINT_TYPE
#define REGISTER_UINT_TYPE uint64_t
#endif
typedef REGISTER_UINT_TYPE ruint_t;

#ifndef PHYSICAL_UINT_TYPE
#define PHYSICAL_UINT_TYPE uintptr_t
#endif
typedef PHYSICAL_UINT_TYPE puint_t;