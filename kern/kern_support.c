#include <sys/param.h>

struct proc *
pfind(pid)
register pid_t pid;
{
	__asm {int 3};
	return NULL;
}

void
selwakeup(sip)
register struct selinfo *sip;
{
	__asm {int 3};
	return;	
}

void
gsignal(pgid, signum)
int pgid, signum;
{
	__asm {int 3};
	return;	
}

void
psignal(p, signum)
	register struct proc *p;
	register int signum;
{
	__asm {int 3};
	return;	
}

int
tsleep(ident, priority, wmesg, timo)
void *ident;
int priority, timo;
char *wmesg;
{
	__asm {int 3};
	return NULL;
}

void
wakeup(ident)
register void *ident;
{
	__asm {int 3};
	return;	
}
