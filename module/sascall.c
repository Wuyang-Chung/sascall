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

#include <sys/param.h>
#include <sys/proc.h>
#include <sys/module.h>
#include <sys/sysproto.h>
#include <sys/sysent.h>
#include <sys/kernel.h>
#include <sys/systm.h>
#include <sys/unistd.h>	

//============================
static int sexecve(struct thread *td, void *arg);
struct	proc *sinitproc;

struct sinit_arg {
	char *fname;
};

/*
 * The function for implementing sinit
 */
static int
sinit(struct thread *td, void *arg)
{
#if 1
	printf("sinit: Hello\n");
	return (0);
#else
	struct sinit_arg *uap = arg;
	struct fork_req fr;
	int error;

	if (sinitproc != NULL)
		return ESUCCESS;

	bzero(&fr, sizeof(fr));
	fr.fr_flags = RFFDG | RFPROC | RFSTOPPED;
	fr.fr_procp = &sinitproc; //wyc the created process is returned to initproc
	error = fork1(td, &fr);
	if (error)
		return error;

	path = malloc();
	copy arg to path;
	td = FIRST_THREAD_IN_PROC(sinitproc);
	cpu_fork_kthread_handler(td, start_sinit, path);
	
	thread_lock(td);
	TD_SET_CAN_RUN(td);
	sched_add(td, SRQ_BORING);
	thread_unlock(td);
	
	return (ESUCCESS);
#endif
}

#if 0
static void
start_sinit(char *fname)
{
	struct thread *td;

	td = PCPU_GET(pc_curthread);
	sexecve;
	free(fname);
	delete the thread 
}
#endif

struct sfork_arg {
	int (*func)(void *);
	void *arg;
};

/*
 * The function for implementing the system call.
 */
static int
sfork(struct thread *td, void *arg)
{
#if 1
	printf("sfork: Hello\n");
	return (0);
#else
	struct sfork_arg *uap = arg;
	struct fork_req fr;
	int error, pid;

	bzero(&fr, sizeof(fr));
	fr.fr_flags = RFPROC | RFSAS | RFMEMSHARE;
	fr.fr_pidp = &pid;
	fr.fr_func = uap->func;
	fr.fr_arg = uap->arg;
	error = fork1(td, &fr);
	if (error == 0) {
		td->td_retval[0] = pid;	//wyc return value to the parent
		td->td_retval[1] = 0;
	}
	return (error);
#endif
}

struct sexecve_arg {
	char *fname;
};

/*
 * The function for implementing sexecve
 */
static int
sexecve(struct thread *td, void *arg)
{
#if 1
	printf("sexecve: Hello\n");
	return (0);
#else
	struct sexecve_arg *uap = arg;
#endif
}

//============================

/*
 * The `sysent' for the new system call
 */
static struct sysent sinit_sysent = {
	.sy_narg = 0, //1,
	.sy_call = sinit,
};

/*
 * The offset in sysent where the system call is allocated.
 */
static int sinit_offset = NO_SYSCALL;

/*
 * The function called at load/unload.
 */
static int
sinit_load(struct module *module, int cmd, void *arg)
{
	int error = 0;

	switch (cmd) {
	case MOD_LOAD :
		printf("%s loaded at %d\n", __func__, sinit_offset);
		break;
	case MOD_UNLOAD :
		printf("%s unloaded from %d\n", __func__, sinit_offset);
		break;
	default :
		error = EOPNOTSUPP;
		break;
	}
	return (error);
}

SYSCALL_MODULE(sinit, &sinit_offset, &sinit_sysent, sinit_load, NULL);

/*
 * The `sysent' for the new system call
 */
static struct sysent sfork_sysent = {
	.sy_narg = 0, //2,
	.sy_call = sfork,
};

/*
 * The offset in sysent where the system call is allocated.
 */
static int sfork_offset = NO_SYSCALL;

/*
 * The function called at load/unload.
 */
static int
sfork_load(struct module *module, int cmd, void *arg)
{
	int error = 0;

	switch (cmd) {
	case MOD_LOAD :
		printf("%s loaded at %d\n", __func__, sfork_offset);
		break;
	case MOD_UNLOAD :
		printf("%s unloaded from %d\n", __func__, sfork_offset);
		break;
	default :
		error = EOPNOTSUPP;
		break;
	}
	return (error);
}

SYSCALL_MODULE(sfork, &sfork_offset, &sfork_sysent, sfork_load, NULL);

/*
 * The `sysent' for the new system call
 */
static struct sysent sexecve_sysent = {
	.sy_narg = 0, //1,
	.sy_call = sexecve,
};

/*
 * The offset in sysent where the system call is allocated.
 */
static int sexecve_offset = NO_SYSCALL;

/*
 * The function called at load/unload.
 */
static int
sexecve_load(struct module *module, int cmd, void *arg)
{
	int error = 0;

	switch (cmd) {
	case MOD_LOAD :
		printf("%s loaded at %d\n", __func__, sexecve_offset);
		break;
	case MOD_UNLOAD :
		printf("%s unloaded from %d\n", __func__, sexecve_offset);
		break;
	default :
		error = EOPNOTSUPP;
		break;
	}
	return (error);
}

SYSCALL_MODULE(sexecve, &sexecve_offset, &sexecve_sysent, sexecve_load, NULL);

//============================
struct afork_arg {
	void (*func)(void *);
	void *arg;
};

/*
 * The function for implementing the system call.
 */
static int
afork(struct thread *td, void *arg)
{
	struct afork_arg *uap = arg;
	struct fork_req fr;
	int error, pid;

	bzero(&fr, sizeof(fr));
	fr.fr_flags = RFFDG | RFPROC | RFMEMSHARE;
	fr.fr_pidp = &pid;
	fr.fr_func = uap->func;
	fr.fr_arg = uap->arg;
	error = fork1(td, &fr);
	if (error == 0) {
		td->td_retval[0] = pid;	//wyc return value to the parent
		td->td_retval[1] = 0;
	}

	return (error);
}

/*
 * The `sysent' for the new system call
 */
static struct sysent afork_sysent = {
	.sy_narg = 2,
	.sy_call = afork,
};

/*
 * The offset in sysent where the system call is allocated.
 */
static int afork_offset = NO_SYSCALL;

/*
 * The function called at load/unload.
 */
static int
afork_load(struct module *module, int cmd, void *arg)
{
	int error = 0;

	switch (cmd) {
	case MOD_LOAD :
		printf("%s loaded at %d\n", __func__, afork_offset);
		break;
	case MOD_UNLOAD :
		printf("%s unloaded from %d\n", __func__, afork_offset);
		break;
	default :
		error = EOPNOTSUPP;
		break;
	}
	return (error);
}

SYSCALL_MODULE(afork, &afork_offset, &afork_sysent, afork_load, NULL);