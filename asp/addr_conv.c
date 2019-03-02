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
 * $Log: addr_conv.c,v $
 * Revision 1.1.1.1  2005/09/06 16:40:11  wangcf
 * initial version
 *
 *
****************************************************************/

#include <global.h>

#include "addr_conv.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

/* convert binary mac address to string */
char *mac_ntoa(char *mac, char *buf)
{
	assert (buf!=NULL);
	assert (mac!=NULL);
	
	sprintf(buf, "%02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

	return buf;
}

/* converc mac address string to binary */
void mac_aton(char *buf, char *mac)
{
	u_int tmp_mac[6];

	assert (buf!=NULL);
	assert (mac!=NULL);

	memset(tmp_mac, 0, sizeof(tmp_mac));

	if (sscanf(buf, "%x:%x:%x:%x:%x:%x", tmp_mac, tmp_mac+1, tmp_mac+2, 
		tmp_mac+3, tmp_mac+4, tmp_mac+5) == 6)
	{
		mac[0] = (char)tmp_mac[0];
		mac[1] = (char)tmp_mac[1];
		mac[2] = (char)tmp_mac[2];
		mac[3] = (char)tmp_mac[3];
		mac[4] = (char)tmp_mac[4];
		mac[5] = (char)tmp_mac[5];
	}

	return;
}


/* convert ip address to dot seperated string */
char *ip_ntoa(u_int ipaddr, char *buf)
{
	assert (buf!=NULL);

	sprintf(buf, "%d.%d.%d.%d", (ipaddr>>24)&0xff, (ipaddr>>16)&0xff, (ipaddr>>8)&0xff, (ipaddr)&0xff);

	return buf;
}


/* convert dot seperated string to ip address */
u_int ip_aton(char *buf, u_int *ipaddr)
{
	u_int tmp_addr[4];
	u_int addr;

	assert (buf!=NULL);

	sscanf(buf, "%d.%d.%d.%d", tmp_addr, tmp_addr+1, tmp_addr+2, tmp_addr+3);
	addr = (tmp_addr[0]<<24)+(tmp_addr[1]<<16)+(tmp_addr[2]<<8)+(tmp_addr[3]);
	if (ipaddr!=NULL)
	{
		*ipaddr = addr;
	}

	return addr;
}
