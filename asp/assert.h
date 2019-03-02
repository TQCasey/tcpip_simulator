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
 * $Log: assert.h,v $
 * Revision 1.1.1.1  2005/09/06 16:40:11  wangcf
 * initial version
 *
 *
****************************************************************/

#ifndef __ASSERT_H__
#define __ASSERT_H__

#include <global.h>

#define MAX_ASSSTR_LEN		512

void __assert(const char *file, int line, const char *failedexpr);

#ifdef NDEBUG
#define	assert(e)	((void)0)
#else
#define	assert(e)	((e) ? (void)0 : __assert(__FILE__, __LINE__, NULL))
#endif

#endif