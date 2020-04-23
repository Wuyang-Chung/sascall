/*-
 * Copyright (c) 1999 Assar Westerlund
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * $FreeBSD$
 */

#include <sys/types.h>
#include <sys/module.h>
#include <sys/syscall.h>

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void foo(void *);

void foo(void *arg)
{

	printf("foo: %d\n", (int)arg);
	_exit(0);
}

int
main(int argc __unused, char **argv __unused)
{
	int modid, syscall_num;
	struct module_stat stat;
	int rc;

	stat.version = sizeof(stat);
	if ((modid = modfind("sys/afork")) == -1)
		err(1, "modfind");
	if (modstat(modid, &stat) != 0)
		err(1, "modstat");
	syscall_num = stat.data.intval;
	rc = syscall(syscall_num, foo, 1, 4096);
	printf("afork: %d\n", rc);
#if 0
	stat.version = sizeof(stat);
	if ((modid = modfind("sys/sinit")) == -1)
		err(1, "modfind");
	if (modstat(modid, &stat) != 0)
		err(1, "modstat");
	syscall_num = stat.data.intval;
	syscall(syscall_num); //, foo, NULL);

	stat.version = sizeof(stat);
	if ((modid = modfind("sys/sfork")) == -1)
		err(1, "modfind");
	if (modstat(modid, &stat) != 0)
		err(1, "modstat");
	syscall_num = stat.data.intval;
	syscall (syscall_num); //, foo, NULL);

	stat.version = sizeof(stat);
	if ((modid = modfind("sys/sexecve")) == -1)
		err(1, "modfind");
	if (modstat(modid, &stat) != 0)
		err(1, "modstat");
	syscall_num = stat.data.intval;
	syscall(syscall_num); //, foo, NULL);
#endif
	return 0;
}

