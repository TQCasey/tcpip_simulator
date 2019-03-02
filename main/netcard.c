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
 * $Revision: 1.3 $
 *
 *	MODIFICATION HISTORY
 * $Log: netcard.c,v $
 * Revision 1.3  2005/09/13 06:19:16  wangcf
 * tcp/ip stack simulator daily report
 *
 * Revision 1.2  2005/09/11 05:01:42  wangcf
 * save/restore interface address configuration
 *
 * Revision 1.1  2005/09/07 17:26:07  wangcf
 * 1) add netcard config dialog
 * 2) add bsd system & stack initialization function
 *
 *
****************************************************************/

#include <global.h>
#include <assert.h>
#include "tcpipsim.h"
#include <pcap.h>
#include <profile.h>

LRESULT CALLBACK NetcardConfig(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND hNetcard = NULL;
	pcap_if_t *all_devs = NULL;
	pcap_if_t *dev = NULL;
	char err_buf[256];
	char *buf = NULL;
	int index = 0;
	int select_index = 0;

	switch (message)
	{
	case WM_INITDIALOG:
		hNetcard = GetDlgItem(hDlg, IDC_NETCARD);
		assert(hNetcard != NULL);
		
		/* 获取interface列表 */
		if (pcap_findalldevs(&all_devs, err_buf) == -1)
		{
			MessageBox(NULL, "Can't get interface list", szTitle, MB_OK | MB_ICONERROR);
			return FALSE;
		}
		if (all_devs == NULL)
		{
			MessageBox(NULL, "No interface found", szTitle, MB_OK | MB_ICONERROR);
			return FALSE;
		}

		/* 建立选择interface的下拉列表框 */
		for (dev = all_devs; dev != NULL; dev = dev->next)
		{
			index = SendMessage(hNetcard, CB_ADDSTRING, 0, (LPARAM)dev->description);
			if (!strcmp(g_if_name, dev->name))
			{
				select_index = index;
			}
			buf = malloc(strlen(dev->name)+1);
			if (buf != NULL)
			{
				strcpy(buf, dev->name);
				SendMessage(hNetcard, CB_SETITEMDATA, (WPARAM)index, (LPARAM)buf);
			}
		}

		pcap_freealldevs(all_devs);

		SendMessage(hNetcard, CB_SETCURSEL, select_index, 0);
		
		break;
	case WM_COMMAND:
		hNetcard = GetDlgItem(hDlg, IDC_NETCARD);
		assert(hNetcard != NULL);
		
		/* 选择一块新的网卡 */
		if (LOWORD(wParam) == IDOK)
		{
			select_index = SendMessage(hNetcard, CB_GETCURSEL, 0, 0);
			buf = (char *)SendMessage(hNetcard, CB_GETITEMDATA, select_index, 0);
			strcpy(g_if_name, buf);
			prof_printf(g_prof_name, SECTION_PCAP, "netcard", g_if_name);
		}

		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			/* 释放列表框的资源 */
			for (index=0; index<SendMessage(hNetcard, CB_GETCOUNT, 0, 0); index++)
			{
				buf = (char*)SendMessage(hNetcard, CB_GETITEMDATA, index, 0);
				if (buf!=NULL)
				{
					free(buf);
				}
			}
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		break;
	}

	return FALSE;
}
