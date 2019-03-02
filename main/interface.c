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
 * $Log: interface.c,v $
 * Revision 1.3  2005/09/11 05:01:42  wangcf
 * save/restore interface address configuration
 *
 * Revision 1.2  2005/09/10 16:58:33  wangcf
 * support configurate ip address dynamic
 *
 * Revision 1.1  2005/09/08 10:27:02  wangcf
 * add interface configuration dialog
 *
 * Revision 1.2  2005/09/07 11:26:53  wangcf
 * tcp/ip stack simulator daily backup
 *
 *
****************************************************************/

#include <global.h>
#include <assert.h>
#include <stdio.h>
#include "tcpipsim.h"
#include <commctrl.h>
#include <addr_conv.h>
#include <profile.h>

IfAddr_t ia_list[NSE];
unsigned int new_ip, new_mask;

void IfSelChange(HWND hDlg);
LRESULT CALLBACK IpAddrCfg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

// function handler of about dialog
LRESULT CALLBACK InterfaceConfig(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND hIf = NULL;
	HWND hAddr = NULL;
	char buf[20];
	int if_index;
	LV_COLUMN lvc;
	LV_ITEM	item;
	int i, ia_index;
	int index;
	char sec[20], key[20];

	switch (message)
	{
	case WM_INITDIALOG:
		// 初始化interface列表
		hIf = GetDlgItem(hDlg, IDC_INTERFACE);
		assert(hIf!=NULL);
		for (if_index=0; if_index<NSE; if_index++)
		{
			sprintf(buf, "soft eth%d", if_index);
			SendMessage(hIf, CB_ADDSTRING, 0, (LPARAM)buf);
		}
		SendMessage(hIf, CB_SETCURSEL, 0, 0);
		
		// 初始化ip地址控件
		hAddr = GetDlgItem(hDlg, IDC_IPADDRESS);
		assert(hAddr!=NULL);
		lvc.mask = LVCF_TEXT;
		lvc.pszText = TEXT("IP Address");
		lvc.iSubItem = 0;
		SendMessage(hAddr, LVM_INSERTCOLUMN, 0, (LPARAM)&lvc);
		SendMessage(hAddr, LVM_SETCOLUMNWIDTH, 0, 120);
		lvc.pszText = TEXT("Netmask");
		SendMessage(hAddr, LVM_INSERTCOLUMN, 1, (LPARAM)&lvc);
		SendMessage(hAddr, LVM_SETCOLUMNWIDTH, 1, 120);
		
		IfSelChange(hDlg);
		return TRUE;

	case WM_COMMAND:
		hIf = GetDlgItem(hDlg, IDC_INTERFACE);
		assert(hIf!=NULL);
		if_index = SendMessage(hIf, CB_GETCURSEL, 0, 0);
		assert(if_index<NSE);
		hAddr = GetDlgItem(hDlg, IDC_IPADDRESS);
		assert(hAddr!=NULL);

		switch (HIWORD(wParam))
		{
		case CBN_SELCHANGE:
			IfSelChange(hDlg);
			return TRUE;
			break;
		}

		switch (LOWORD(wParam))
		{
		case IDC_ADDRADD:		// add ip address
			if (ia_list[if_index].ia_num>=MAX_IA_NUM)
			{
				MessageBox(hDlg, "Number of IP address exceeds limition", szTitle, MB_OK|MB_ICONEXCLAMATION);
				return TRUE;
			}
			new_ip = 0;
			new_mask = 0;
			if (DialogBox(hInst, (LPCTSTR)IDD_IPADDRADD, hDlg, (DLGPROC)IpAddrCfg)==IDOK)
			{
				// add address & mask to stack
				if (ifaddr_add(if_index, new_ip, new_mask)!=0)
				{
					MessageBox(hDlg, "Adding address error", szTitle, MB_OK|MB_ICONERROR);
					return TRUE;
				}

//				// check new address & mask
//				for (i=0; i<ia_list[if_index].ia_num; i++)
//				{
//					if ((ia_list[if_index].ia[i].ip&new_mask)==(new_ip&new_mask)
//						|| (ia_list[if_index].ia[i].ip&ia_list[if_index].ia[i].mask)==(new_ip&ia_list[if_index].ia[i].mask))
//					{
//						MessageBox(hDlg, "Duplicate IP address", szTitle, MB_OK|MB_ICONERROR);
//						return TRUE;
//					}
//				}

				// insert ip address in address list
				for (ia_index=0; ia_index<ia_list[if_index].ia_num; ia_index++)
				{
					if (ia_list[if_index].ia[ia_index].ip>=new_ip)
						break;
				}
				for (i=ia_list[if_index].ia_num; i>ia_index; i--)
				{
					memcpy(&ia_list[if_index].ia[i], &ia_list[if_index].ia[i-1], sizeof(struct ia_addr));
				}
				ia_list[if_index].ia[ia_index].ip = new_ip;
				ia_list[if_index].ia[ia_index].mask = new_mask;
				ia_list[if_index].ia_num++;

				// update profile
				sprintf(sec, "seth%d", if_index);
				for (i=ia_index; i<ia_list[if_index].ia_num; i++)
				{
					sprintf(key, "ipaddr%d", i);
					prof_printf(g_prof_name, sec, key, ip_ntoa(ia_list[if_index].ia[i].ip, buf));
					sprintf(key, "netmask%d", i);
					prof_printf(g_prof_name, sec, key, ip_ntoa(ia_list[if_index].ia[i].mask, buf));
				}

				// refresh display
				item.mask = LVIF_TEXT;
				item.iItem = 0;
				item.iSubItem = 0;
				ip_ntoa(new_ip, buf);
				item.pszText = buf;
				index = SendMessage(hAddr, LVM_INSERTITEM, 0, (LPARAM)&item);
				item.iSubItem++;
				ip_ntoa(new_mask, buf);
				SendMessage(hAddr, LVM_SETITEMTEXT, index, (LPARAM)&item);
			}
			return TRUE;
			break;
		case IDC_ADDRDEL:
			ia_index = SendMessage(hAddr, LVM_GETNEXTITEM, -1, MAKELPARAM((UINT)(LVNI_ALL|LVNI_SELECTED),0)) ;
			if (ia_index>=0)
			{
				assert(ia_index<ia_list[if_index].ia_num);
				if (ifaddr_del(if_index, ia_list[if_index].ia[ia_index].ip)!=0)
				{
					MessageBox(hDlg, "Deleting address error", szTitle, MB_OK|MB_ICONERROR);
					return TRUE;
				}
				for (i=ia_index+1; i<ia_list[if_index].ia_num; i++)
				{
					memcpy(&ia_list[if_index].ia[i-1], &ia_list[if_index].ia[i], sizeof(struct ia_addr));
				}
				ia_list[if_index].ia_num--;
				assert(ia_list[if_index].ia_num>=0);

				// update profile
				sprintf(sec, "seth%d", if_index);
				for (i=ia_index; i<ia_list[if_index].ia_num; i++)
				{
					sprintf(key, "ipaddr%d", i);
					prof_printf(g_prof_name, sec, key, ip_ntoa(ia_list[if_index].ia[i].ip, buf));
					sprintf(key, "netmask%d", i);
					prof_printf(g_prof_name, sec, key, ip_ntoa(ia_list[if_index].ia[i].mask, buf));
				}
				sprintf(key, "ipaddr%d", i);
				prof_printf(g_prof_name, sec, key, "");
				sprintf(key, "netmask%d", i);
				prof_printf(g_prof_name, sec, key, "");

				// update display
				SendMessage(hAddr, LVM_DELETEITEM, ia_index, 0);
			}
			return TRUE;
			break;
		case IDOK:
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
			break;
		}
	}
	
	return FALSE;
}

LRESULT CALLBACK IpAddrCfg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND hAddr, hMask;
	char buf[20], buf2[20];

	switch (message)
	{
	case WM_INITDIALOG:
		if (new_ip!=0)
		{
			hAddr = GetDlgItem(hDlg, IDC_ADDRESS);
			assert(hAddr!=NULL);
			ip_ntoa(new_ip, buf);
			SetWindowText(hAddr, buf);
		}
		if (new_mask!=0)
		{
			hMask = GetDlgItem(hDlg, IDC_NETMASK);
			assert(hMask!=NULL);
			ip_ntoa(new_mask, buf);
			SetWindowText(hMask, buf);
		}
		return TRUE;
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			hAddr = GetDlgItem(hDlg, IDC_ADDRESS);
			assert(hAddr!=NULL);
			GetWindowText(hAddr, buf, sizeof(buf));
			hMask = GetDlgItem(hDlg, IDC_NETMASK);
			assert(hAddr!=NULL);
			GetWindowText(hMask, buf2, sizeof(buf2));
			if (strlen(buf)>0 && strlen(buf2)>0)
			{
				ip_aton(buf, &new_ip);
				ip_aton(buf2, &new_mask);
				EndDialog(hDlg, IDOK);
			}
			else
			{
				MessageBox(hDlg, "Please input IP address and netmask", szTitle, MB_OK|MB_ICONERROR);
			}
			return TRUE;
			break;
		case IDCANCEL:
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
			break;
		}
	}

	return FALSE;
}

void IfSelChange(HWND hDlg)
{
	HWND hIf = GetDlgItem(hDlg, IDC_INTERFACE);
	HWND hIpAddr = GetDlgItem(hDlg, IDC_IPADDRESS);
	int if_index;
	LV_ITEM item;
	char buf[20];
	int i;

	assert(hIf!=NULL && hIpAddr!=NULL);

	// 得到当前网卡
	if_index = SendMessage(hIf, CB_GETCURSEL, 0, 0);
	assert(if_index<NSE);

	// 更新IP地址显示
	SendMessage(hIpAddr, LVM_DELETEALLITEMS, (WPARAM)0, 0);

	for (i=0; i<ia_list[if_index].ia_num; i++)
	{
		memset(&item, 0, sizeof(item));
		item.mask = LVIF_TEXT;
		item.iItem = i;
		item.iSubItem = 0;
		ip_ntoa(ia_list[if_index].ia[i].ip, buf);
		item.pszText = buf;
		SendMessage(hIpAddr, LVM_INSERTITEM, 0, (LPARAM)&item);
		item.iSubItem++;
		ip_ntoa(ia_list[if_index].ia[i].mask, buf);
		SendMessage(hIpAddr, LVM_SETITEMTEXT, i, (LPARAM)&item);
	}

	return;
}

