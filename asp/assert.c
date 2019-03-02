/****************************************************************
 *
 *	Copyleft (C) 2005 Wangcf, No Right Reserved
 *		E-mail: wangcf@publicb.bta.net.cn
 *
 * $Revision: 1.1.1.1 $
 *
 *	PURPOSE:
 *		Enter file description here
 *
 *	MODIFICATION HISTORY
 * $Log: assert.c,v $
 * Revision 1.1.1.1  2005/09/06 16:40:11  wangcf
 * initial version
 *
 *
****************************************************************/

#include "assert.h"
#include <global.h>
#include <stdio.h>

void __assert(const char *file, int line, const char *failedexpr)
{
	char buf[MAX_ASSSTR_LEN];
	
	if (failedexpr == NULL)
	{
		_snprintf(buf, sizeof(buf)-1, "assertion \"%s\" failed: file \"%s\", line %d\n",  failedexpr, file, line);
	}
	else
	{
		_snprintf(buf, sizeof(buf)-1, "assertion failed: file \"%s\", line %d\n",  file, line);
	}

	buf[sizeof(buf)-1] = '\0';
	MessageBox(NULL, buf, "Error", MB_OK | MB_ICONERROR);
	
	__asm {int 3};
	return;
}
