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
 * $Log: wpcap.c,v $
 * Revision 1.2  2005/09/10 16:58:33  wangcf
 * support configurate ip address dynamic
 *
 * Revision 1.1  2005/09/07 11:27:08  wangcf
 * tcp/ip stack simulator daily backup
 *
 *
****************************************************************/

#include <global.h>
#include "tcpipsim.h"
#include <pcap.h>

/* ‘ÿ»Îwpcapƒ£øÈ */
void load_wpcap()
{
	HINSTANCE hInst = NULL;

	hInst = LoadLibrary("wpcap");
	if (hInst == NULL)
	{
		return;
	}

	g_has_wpcap = TRUE;
	return;
}

void wpcap_send_pkt(pcap_t *pf, char *pkt, int len)
{
	if (pf == NULL)
	{
		return;
	}
	
	pcap_sendpacket(pf, pkt, len);

	return;
}

