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
 * $Revision: 1.2 $
 *
 *	MODIFICATION HISTORY
 * $Log: kern_init.c,v $
 * Revision 1.2  2005/09/10 16:58:33  wangcf
 * support configurate ip address dynamic
 *
 * Revision 1.1  2005/09/07 17:26:32  wangcf
 * 1) add netcard config dialog
 * 2) add bsd system & stack initialization function
 *
 *
****************************************************************/

//#include <glib.h>
//#include <string.h>
#include <sys/param.h>
#include <sys/domain.h>
#include <sys/malloc.h>
#include <sys/socket.h>
#include <net/if.h>
#include <net/route.h>
#include <sys/param.h>
#include <sys/callout.h>
#include <stand/stand.h>
#include <sys/sockio.h>
#include <netinet/in.h>
#include <netinet/in_var.h>
#include <sys/mbuf.h>
#include <stdio.h>
#include <string.h>
#include <pcap.h>
#include <netinet/if_ether.h>
#include <if_softeth.h>

void bsd_init()
{
	int i;

	/* freebsd系统初始化 */
	callout = (struct callout *)malloc(sizeof(struct callout)*ncallout, 0, 0);
	callfree = callout;
	for (i = 1; i < ncallout; i++)
		callout[i-1].c_next = &callout[i];
	callout[i-1].c_next = NULL;

	return;
}

void bsd_done()
{
	free(callout);
	
	return;
}

void net_init()
{
	int i;
	
	mbinit();
	ifinit();
	domaininit();
	
	loopattach(0);

	for (i=0; i<NSE; i++)
		seattach(i);
	
	return;
}

void net_done()
{
	mbdone();
	return;
}

int ifaddr_add(int index, unsigned int ipaddr, unsigned int netmask)
{
	char buf[10];
	struct in_aliasreq ifra;
	struct ifnet *ifp = NULL;
	
	sprintf(buf, "se%d", index);
	ifp = ifunit(buf);
	strcpy(ifra.ifra_name, buf);
	ifra.ifra_addr.sin_family = AF_INET;
	ifra.ifra_addr.sin_len = sizeof(struct sockaddr_in);
	ifra.ifra_addr.sin_addr.s_addr = htonl(ipaddr);
	ifra.ifra_mask.sin_family = AF_INET;
	ifra.ifra_mask.sin_len = sizeof(struct sockaddr_in);
	ifra.ifra_mask.sin_addr.s_addr = htonl(netmask);
	return in_control(NULL, SIOCAIFADDR, &ifra, ifp);
}

int ifaddr_del(int index, unsigned int ipaddr)
{
	char buf[10];
	struct in_aliasreq ifra;
	struct ifnet *ifp = NULL;
	
	sprintf(buf, "se%d", index);
	ifp = ifunit(buf);
	strcpy(ifra.ifra_name, buf);
	ifra.ifra_addr.sin_family = AF_INET;
	ifra.ifra_addr.sin_len = sizeof(struct sockaddr_in);
	ifra.ifra_addr.sin_addr.s_addr = htonl(ipaddr);
	return in_control(NULL, SIOCDIFADDR, &ifra, ifp);
}

void se_pcap_cb(char *user, const struct pcap_pkthdr *phdr, const char *pdata)
{
	register struct ether_header *eh = (struct ether_header *)pdata;
	int i;
	struct se_softc *sep = NULL;
	struct ifnet *recv_ifp = NULL;
	char bc_addr[6] = {(char)0xff, (char)0xff, (char)0xff, (char)0xff, (char)0xff, (char)0xff};
	
	struct mbuf *m;
	int len;
	
	/* 因为可能是从一块网卡发送到另外一块网卡，所以先判断目的地址是不是本地mac */
	for (i=0; i<NSE; i++)
	{
		sep = &se_softc[i];
		
		if (bcmp(bc_addr, eh->ether_dhost, 6) == 0)
		{	/* 广播包，不发给发送此报文的网卡 */
			if (bcmp(sep->se_addr, eh->ether_shost, 6) == 0)
				continue;
		}
		else
		{	/* 非广播包，根据mac发送 */
			if (bcmp(sep->se_addr, eh->ether_dhost, 6) != 0)
				continue;
		}
		
		recv_ifp = &sep->se_if;		
		
		MGETHDR(m, M_DONTWAIT, MT_DATA);
		if (m == NULL)
			continue;
		m->m_pkthdr.rcvif = recv_ifp;
		m->m_pkthdr.len = phdr->caplen-sizeof(struct ether_header);
		m->m_len = MHLEN;
		MCLGET(m, M_DONTWAIT);
		
		
#define min(a,b) (a<b?a:b)
		if (m->m_flags & M_EXT)
			m->m_len = len = min(phdr->caplen-sizeof(struct ether_header), MCLBYTES);
		else
			len = m->m_len;
		bcopy(pdata+sizeof(struct ether_header), mtod(m, caddr_t), (unsigned)len);
		
		// 调用if_ether的接收函数
		ether_input(recv_ifp, eh, m);
	}
	
	return;
}
