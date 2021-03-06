/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * User memory access support for Hexagon
 *
 * Copyright (c) 2010-2011, The Linux Foundation. All rights reserved.
 */

#ifndef _ASM_UACCESS_H
#define _ASM_UACCESS_H
/*
 * User space memory access functions
 */
#include <asm/sections.h>

/*
 * access_ok: - Checks if a user space pointer is valid
 * @addr: User space pointer to start of block to check
 * @size: Size of block to check
 *
 * Context: User context only. This function may sleep if pagefaults are
 *          enabled.
 *
 * Checks if a pointer to a block of memory in user space is valid.
 *
 * Returns true (nonzero) if the memory block *may* be valid, false (zero)
 * if it is definitely invalid.
 *
 * User address space in Hexagon, like x86, goes to 0xbfffffff, so the
 * simple MSB-based tests used by MIPS won't work.  Some further
 * optimization is probably possible here, but for now, keep it
 * reasonably simple and not *too* slow.  After all, we've got the
 * MMU for backup.
 */

#define __access_ok(addr, size) \
	((get_fs().seg == KERNEL_DS.seg) || \
	(((unsigned long)addr < get_fs().seg) && \
	  (unsigned long)size < (get_fs().seg - (unsigned long)addr)))

/*
 * When a kernel-mode page fault is taken, the faulting instruction
 * address is checked against a table of exception_table_entries.
 * Each entry is a tuple of the address of an instruction that may
 * be authorized to fault, and the address at which execution should
 * be resumed instead of the faulting instruction, so as to effect
 * a workaround.
 */

/*  Assembly somewhat optimized copy routines  */
unsigned long raw_copy_from_user(void *to, const void __user *from,
				     unsigned long n);
unsigned long raw_copy_to_user(void __user *to, const void *from,
				   unsigned long n);
#define INLINE_COPY_FROM_USER
#define INLINE_COPY_TO_USER

__kernel_size_t __clear_user_hexagon(void __user *dest, unsigned long count);
#define __clear_user(a, s) __clear_user_hexagon((a), (s))

#include <asm-generic/uaccess.h>


#endif
