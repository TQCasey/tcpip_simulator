/****************************************************************
 *
 *	Copyleft (C) 2003 Wang Chao Feng, No Rights Reserved
 *
 *	  All of the software may be reproduced or transmitted in any form or 
 *	  by any means, electronic or mechanical, for any purpose, without express
 *	  written permission of Wang Chao Feng. 
 *
 *	PURPOSE:
 *		header of profile support
 *
 * $Revision: 1.2 $
 *
 *	MODIFICATION HISTORY
 * $Log: profile.h,v $
 * Revision 1.2  2005/09/08 10:26:50  wangcf
 * add interface configuration dialog
 *
 * Revision 1.1.1.1  2005/05/08 05:57:57  guest
 * initial version
 *
 * Revision 1.2  2005/04/26 08:16:46  wangcf
 * 整理代码整理代码整理代码整理代码
 *
 * Revision 1.1.1.1  2004/06/24 07:11:43  wangcf
 * initial version
 *
 * Revision 1.1.1.1  2004/01/06 08:00:10  wangcf
 * initial version
 *
 *
****************************************************************/

#ifndef __PROFILE_H
#define __PROFILE_H

#ifdef __CPLUSPLUS
extern "C"
{
#endif

#define PROF_MAX_LEN				256
#define PROF_TMPFILE_NAME_LEN		32
#define PROF_BUF_LEN				(32*1024)
	
#define INT32 int
#define INT8	char
	
struct str_profkey
{
	INT8	name[PROF_MAX_LEN];
	INT8	value[PROF_MAX_LEN];
	struct str_profkey *next;
};

struct str_profsection
{
	INT8	name[PROF_MAX_LEN];
	struct str_profsection *next;
	struct str_profkey firstkey;
};

INT32 prof_get_int(const INT8 *filename, const INT8 *section, const INT8 *key, INT32 def_value);
INT32 prof_get_string(const INT8 *filename, const INT8 *section, const INT8 *key, 
					  const INT8 *def_str, INT32 size, INT8 *buf);
INT32 prof_printf(const INT8 *filename, const INT8 *section, const INT8 *key, const INT8 *fmt, ...);


#ifdef __CPLUSPLUS
}
#endif

#endif
