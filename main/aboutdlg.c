/****************************************************************
 *
 *	Copyleft (C) 2003 Wang Chao Feng, No Rights Reserved
 *
 *	  All of the software may be reproduced or transmitted in any form or 
 *	  by any means, electronic or mechanical, for any purpose, without express
 *	  written permission of Wang Chao Feng. 
 *
 *	PURPOSE:
 *		
 *
 * $Revision: 1.2 $
 *
 *	MODIFICATION HISTORY
 * $Log: aboutdlg.c,v $
 * Revision 1.2  2005/09/07 11:26:53  wangcf
 * tcp/ip stack simulator daily backup
 *
 *
****************************************************************/

#include <global.h>
#include <assert.h>
#include <stdio.h>
#include "tcpipsim.h"

// function handler of about dialog
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND hWndVersion = NULL;
	char buf[512];
	int offset = 0;
	
	switch (message)
	{
	case WM_INITDIALOG:
		hWndVersion = GetDlgItem(hDlg, IDC_VERSION);
		assert(hWndVersion != NULL);
		offset += sprintf(buf+offset, "%s", szTitle);
		offset += sprintf(buf+offset, "\nVersion %d.%d", g_major_version, g_minor_version);
		offset += sprintf(buf+offset, "\n\n%s compiled by Wangcf", g_build_date);
		offset += sprintf(buf+offset, "\n\nBug report: wangcf@publicb.bta.net.cn");
		SetWindowText(hWndVersion, buf);
		return TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		break;
	}
	
	return FALSE;
}
