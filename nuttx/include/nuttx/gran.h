/****************************************************************************
 * include/nuttx/gran.h
 * General purpose granule memory allocator.
 *
 *   Copyright (C) 2009 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

#ifndef __INCLUDE_NUTTX_GRAN_H
#define __INCLUDE_NUTTX_GRAN_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <sys/types.h>
#include <stdint.h>

#ifdef CONFIG_GRAN

/****************************************************************************
 * Pre-Processor Definitions
 ****************************************************************************/
/* Configuration ************************************************************/
/* CONFIG_GRAN - Enable granual allocator support
 * CONFIG_GRAN_SINGLE - Select if there is only one instance of the
 *   granule allocator (i.e., gran_initialize will be called only once.
 *   In this case, (1) there are a few optimizations that can can be done
 *   and (2) the GRAN_HANDLE is not needed.
 * CONFIG_DEBUG_GRAN - Just like CONFIG_DEBUG_MM, but only generates ouput
 *   from the gran allocation logic.
 */

/****************************************************************************
 * Public Types
 ****************************************************************************/

#ifndef CONFIG_GRAN_SINGLE
typedef FAR void *GRAN_HANDLE;
#endif

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

#ifdef __cplusplus
#define EXTERN extern "C"
extern "C" {
#else
#define EXTERN extern
#endif

/****************************************************************************
 * Name: gran_initialize
 *
 * Description:
 *   Set up one granule allocator instance.  Allocations will be aligned to
 *   the granule size; allocations will be in units of the granule size.
 *   Larger granules will give better performance and less overhead but more
 *   losses of memory due to alignment and quantization waste.
 *
 *   NOTE: The current implementation also restricts the maximum allocation
 *   size to 32 granaules.  That restriction could be eliminated with some
 *   additional coding effort.
 *
 * Input Parameters:
 *   heapstart - Start of the granule allocation heap
 *   heapsize  - Size of heap in bytes
 *   log2gran  - Log base 2 of the size of one granule.  0->1 byte,
 *               1->2 bytes, 2->4 bytes, 3-> 8bytes, etc.
 *
 * Returned Value:
 *   On success, a non-NULL handle is returned that may be used with other
 *   granual allocator interfaces.
 *
 ****************************************************************************/

#ifdef CONFIG_GRAN_SINGLE
EXTERN int gran_initialize(FAR void *heapstart, size_t heapsize,
                           uint8_t log2gran);
#else
EXTERN GRAN_HANDLE gran_initialize(FAR void *heapstart, size_t heapsize,
                                   uint8_t log2gran);
#endif

/****************************************************************************
 * Name: gran_alloc
 *
 * Description:
 *   Allocate memory from the granule heap.
 *
 *   NOTE: The current implementation also restricts the maximum allocation
 *   size to 32 granaules.  That restriction could be eliminated with some
 *   additional coding effort.
 *
 * Input Parameters:
 *   handle - The handle previously returned by gran_initialize
 *   size   - The size of the memory region to allocate.
 *
 * Returned Value:
 *   On success, either a non-NULL pointer to the allocated memory (if
 *   CONFIG_GRAN_SINGLE) or zero  (if !CONFIG_GRAN_SINGLE) is returned.
 *
 ****************************************************************************/

#ifdef CONFIG_GRAN_SINGLE
EXTERN FAR void *gran_alloc(size_t size);
#else
EXTERN FAR void *gran_alloc(GRAN_HANDLE handle, size_t size);
#endif

/****************************************************************************
 * Name: gran_free
 *
 * Description:
 *   Return memory to the granule heap.
 *
 * Input Parameters:
 *   handle - The handle previously returned by gran_initialize
 *   memory - A pointer to memory previoiusly allocated by gran_alloc.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

#ifdef CONFIG_GRAN_SINGLE
EXTERN void gran_free(FAR void *memory, size_t size);
#else
EXTERN void gran_free(GRAN_HANDLE handle, FAR void *memory, size_t size);
#endif

#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif /* CONFIG_GRAN */
#endif /* __INCLUDE_NUTTX_GRAN_H */
