/****************************************************************
 *
 *	Copyleft (C) 2003 Wang Chao Feng, No Rights Reserved
 *
 *	  All of the software may be reproduced or transmitted in any form or 
 *	  by any means, electronic or mechanical, for any purpose, without express
 *	  written permission of Wang Chao Feng. 
 *
 *	PURPOSE:
 *		main function of netsim
 *
 * $Revision: 1.4 $
 *
 *	MODIFICATION HISTORY
 * $Log: init.c,v $
 * Revision 1.4  2005/09/11 05:01:42  wangcf
 * save/restore interface address configuration
 *
 * Revision 1.3  2005/09/10 16:58:33  wangcf
 * support configurate ip address dynamic
 *
 * Revision 1.2  2005/09/07 17:25:56  wangcf
 * 1) add netcard config dialog
 * 2) add bsd system & stack initialization function
 *
 * Revision 1.1  2005/09/07 11:27:08  wangcf
 * tcp/ip stack simulator daily backup
 *
 *
****************************************************************/

#include <global.h>
#include <pcap.h>
#include <profile.h>
#include <addr_conv.h>
#include "tcpipsim.h"
#include <string.h>

/* 系统初始化函数 */
int sys_init()
{
	pcap_if_t *all_devs = NULL;
	pcap_if_t *dev = NULL;

	/* 初始化需要操作的接口名 */
	memset(g_if_name, 0, MAX_IFNAME_LEN);
	prof_get_string(g_prof_name, SECTION_PCAP, "netcard", NULL, MAX_IFNAME_LEN, g_if_name);

	load_wpcap();
	if (!g_has_wpcap)
	{
		MessageBox(NULL, "Unable to load WinPcap (wpcal.dll). Please install winpcap", 
			szTitle, MB_OK|MB_ICONERROR);
		return FALSE;
	}

	memset(ia_list, 0, sizeof(ia_list));

	bsd_init();

	return TRUE;
}

/* 退出系统时调用该函数释放动态分配的资源 */
void sys_done()
{
	bsd_done();

	return;
}

void ifaddr_init()
{
	int card_index;
	int addr_index;
	char sec[20], key[20];
	char addr[20], mask[20];

	for (card_index=0; card_index<NSE; card_index++)
	{
		sprintf(sec, "seth%d", card_index);
		for (addr_index=0; addr_index<MAX_IA_NUM; addr_index++)
		{
			sprintf(key, "ipaddr%d", addr_index);
			prof_get_string(g_prof_name, sec, key, "", sizeof(addr), addr);
			sprintf(key, "netmask%d", addr_index);
			prof_get_string(g_prof_name, sec, key, "", sizeof(mask), mask);
			if (strlen(addr)>0 && strlen(mask)>0)
			{
				if (ifaddr_add(card_index, ip_aton(addr, NULL), ip_aton(mask, NULL))==0)
				{
					ia_list[card_index].ia[addr_index].ip = ip_aton(addr, NULL);
					ia_list[card_index].ia[addr_index].mask = ip_aton(mask, NULL);
					ia_list[card_index].ia_num++;
				}
			}
		}
	}

	return;
}


void ifaddr_done()
{
	int card_index, addr_index;

	// delete address from stack
	for (card_index=0; card_index<NSE; card_index++)
	{
		for (addr_index=0; addr_index<ia_list[card_index].ia_num; addr_index++)
		{
			ifaddr_del(card_index, ia_list[card_index].ia[addr_index].ip);
		}
	}

	return;
}

