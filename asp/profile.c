/****************************************************************
 *
 *	Copyleft (C) 2003 Wang Chao Feng, No Rights Reserved
 *
 *	  All of the software may be reproduced or transmitted in any form or 
 *	  by any means, electronic or mechanical, for any purpose, without express
 *	  written permission of Wang Chao Feng. 
 *
 *	PURPOSE:
 *		profile read/write support function
 *
 * $Revision: 1.2 $
 *
 *	MODIFICATION HISTORY
 * $Log: profile.c,v $
 * Revision 1.2  2005/09/08 10:26:50  wangcf
 * add interface configuration dialog
 *
 * Revision 1.2  2005/05/12 23:52:34  guest
 * 支持将配置保存到profile文件
 *
 * Revision 1.1.1.1  2005/05/08 05:57:57  guest
 * initial version
 *
 * Revision 1.2  2005/04/26 08:15:58  wangcf
 * 整理代码整理代码整理代码整理代码
 *
 * Revision 1.1.1.1  2004/06/24 07:12:14  wangcf
 * initial version
 *
 * Revision 1.1.1.1  2004/01/06 08:00:10  wangcf
 * initial version
 *
 *
****************************************************************/
#include <global.h>
#include "profile.h"
#include <stdio.h>
#include <assert.h>

#define TRUE	1
#define FALSE	0

/* get integer from profile */
INT32 prof_get_int(const INT8 *filename, const INT8 *section, const INT8 *key, INT32 def_value)
{
	INT8 line[PROF_MAX_LEN+1];
	FILE *pf = NULL;
	INT32 seclen = 0;
	INT32 secflag = FALSE;
	INT32 value = def_value;
	
	assert (filename!=NULL);
	assert (section!=NULL);
	assert (key!=NULL);
	assert (filename[0]!='\0');
	assert (section[0]!='\0');
	assert (key[0]!='\0');

	pf = fopen(filename, "r");
	if (NULL == pf)
	{	/* return default value if profile not exist */
		return def_value;
	}

	seclen = strlen(section);
	secflag = FALSE;
	while (NULL!=fgets(line, sizeof(line)-1, pf))
	{
		/* reached next section */
		if (secflag && line[0] == '[')
		{
			break;
		}

		/* correct section */
		if (line[0] == '[' && !memcmp(&line[1], section, seclen) && line[seclen+1] == ']')
		{
			secflag = TRUE;
			continue;
		}

		if (secflag)
		{
			if (!strncmp(line, key, strlen(key)) && line[strlen(key)] == '=')
			{
				/* find it */
				if (sscanf(&line[strlen(key)+1], "%d", &value)!=1)
				{
					value = def_value;
				}
				break;
			}
		}
	}

	fclose(pf);

	return value;
}

/* get string from profile */
INT32 prof_get_string(const INT8 *filename, const INT8 *section, const INT8 *key, 
	const INT8 *def_str, INT32 size, INT8 *buf)
{
	INT8 line[PROF_MAX_LEN+1];
	FILE *pf;
	INT32 seclen = 0;
	INT32 secflag = FALSE;
	
	assert (filename!=NULL);
	assert (section!=NULL);
	assert (key!=NULL);
	assert (filename[0]!='\0');
	assert (section[0]!='\0');
	assert (key[0]!='\0');

	memset(buf, 0, size);
	if (def_str!=NULL)
	{
		strncpy(buf, def_str, size);
	}
	
	pf = fopen(filename, "r");
	if (NULL == pf)
	{
		return 0;
	}

	seclen = strlen(section);
	secflag = FALSE;
	while (NULL!=fgets(line, sizeof(line), pf))
	{
		/* reached next section */
		if (secflag && line[0] == '[')
		{
			break;
		}

		/* find correct section */
		if (line[0] == '[' && !memcmp(&line[1], section, seclen) && line[seclen+1] == ']')
		{
			secflag = TRUE;
			continue;
		}

		if (secflag)
		{
			if (!strncmp(line, key, strlen(key)) && line[strlen(key)] == '=')
			{
				/* find it */
				strncpy(buf, &line[strlen(key)+1], size-1);
				buf[size-1] = '\0';
				buf[strlen(buf)-1] = '\0';
				break;
			}
		}
	}

	fclose(pf);

	return strlen(buf);
}

/* write to profile */
INT32 prof_printf(const INT8 *filename, const INT8 *section, const INT8 *key, const INT8 *fmt, ...)
{
	INT32 n = 0;
	FILE *pf = NULL;
	INT32 offset = 0;
	INT8 *buf = NULL;
	INT8 line[PROF_MAX_LEN+1];
	INT32 sec_flag = FALSE;
	INT32 key_flag = FALSE;
	struct str_profsection profile;	
	struct str_profsection *tmpsec = NULL;
	struct str_profkey *tmpkey = NULL;
	struct str_profsection *cursec = NULL;
	struct str_profkey *curkey = NULL;
	INT8 name[PROF_MAX_LEN];
	INT8 sec_blank = FALSE;
	va_list args;
	int len = 0;

	assert (filename!=NULL);
	assert (section!=NULL);
	assert (key!=NULL);
	assert (filename[0]!='\0');
	assert (section[0]!='\0');
	assert (key[0]!='\0');

	memset(&profile, 0, sizeof(struct str_profsection));

	pf = fopen(filename, "r");
	if (pf == NULL)
		goto write_file;

	while(NULL!=fgets(line, sizeof(line), pf))
	{
		/* delete line feed */
		line[strlen(line)-1]='\0';
		
		if (line[0] == '[')
		{	/* section start, search section, if section not exist, create new section */
			for (n=1; line[n]!=']' && line[n]!='\0'; n++)
				name[n-1] = line[n];
			name[n-1] = '\0';
			cursec = &profile;
			while (cursec->next != NULL && strcmp(cursec->next->name, name))
				cursec = cursec->next;
			if (cursec->next == NULL)
			{
				tmpsec = (struct str_profsection *)malloc(sizeof(struct str_profsection));
				if (tmpsec == NULL)
				{
					goto ret;
				}
				memset(tmpsec, 0, sizeof(struct str_profsection));
				strcpy(tmpsec->name, name);
				cursec->next = tmpsec;
				cursec = tmpsec;
			}
			else
			{
				cursec = cursec->next;
			}
			continue;
		}

		if (cursec == NULL)
			goto ret;

		buf = strchr(line, '=');
		if (buf == NULL)
			continue;
		memcpy(name, line, buf-line);
		name[buf-line] = '\0';
		curkey = &cursec->firstkey;

		/* search key, if not exist, create new key */
		while (curkey->next!=NULL && strcmp(curkey->next->name, name))
			curkey = curkey->next;
		if (curkey->next == NULL)
		{
			tmpkey = (struct str_profkey *)malloc(sizeof(struct str_profkey));
			if (tmpkey == NULL)
				goto ret;
			memset(tmpkey, 0, sizeof(struct str_profkey));
			strcpy(tmpkey->name, name);
			curkey->next = tmpkey;
			curkey = tmpkey;
		}
		else
		{
			curkey = curkey->next;
		}

		/* update key value */
		strcpy(curkey->value, buf+1);
	}

	if (pf!=NULL)
	{
		fclose(pf);
		pf = NULL;
	}

write_file:
	/* find key to write; if not found, create new key */
	cursec = &profile;
	while (cursec->next != NULL && strcmp(cursec->next->name, section))
		cursec = cursec->next;
	if (cursec->next == NULL)
	{
		tmpsec = (struct str_profsection *)malloc(sizeof(struct str_profsection));
		if (tmpsec == NULL)
		{
			goto ret;
		}
		memset(tmpsec, 0, sizeof(struct str_profsection));
		strcpy(tmpsec->name, section);
		cursec->next = tmpsec;
		cursec = tmpsec;
	}
	else
	{
		cursec = cursec->next;
	}
	
	/* search key, if not exist, create new key */
	curkey = &cursec->firstkey;

	while (curkey->next!=NULL && strcmp(curkey->next->name, key))
		curkey = curkey->next;
	if (curkey->next == NULL)
	{
		tmpkey = (struct str_profkey *)malloc(sizeof(struct str_profkey));
		if (tmpkey == NULL)
			goto ret;
		memset(tmpkey, 0, sizeof(struct str_profkey));
		strcpy(tmpkey->name, key);
		curkey->next = tmpkey;
		curkey = tmpkey;
	}
	else
	{
		curkey = curkey->next;
	}
	/* update key value */
	va_start(args, fmt);
	len = vsprintf(line, fmt, args);
	va_end(args);
	assert(len<sizeof(line));
	strcpy(curkey->value, line);



	/* write profile to file */
	pf = fopen(filename, "w");
	if (pf == NULL)
	{
		goto ret;
	}
	
	for (cursec = &profile; cursec->next != NULL; cursec = &profile)
	{
		sec_blank = FALSE;
		for (curkey = &cursec->next->firstkey; curkey->next != NULL; curkey = &cursec->next->firstkey)
		{
			if (!sec_blank)
			{
				sec_blank = TRUE;
				fprintf(pf, "[%s]\n", cursec->next->name);
			}
			fprintf(pf, "%s=%s\n", curkey->next->name, curkey->next->value);
			tmpkey = curkey->next;
			curkey->next = tmpkey->next;
			free(tmpkey);
		}
		tmpsec = cursec->next;
		cursec->next = tmpsec->next;
		free(tmpsec);
	}

ret:

	if (pf!=NULL)
	{
		fclose(pf);
		pf = NULL;
	}
	return 0;
}

