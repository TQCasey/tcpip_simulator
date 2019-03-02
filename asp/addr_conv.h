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
 * $Log: addr_conv.h,v $
 * Revision 1.1.1.1  2005/09/06 16:40:11  wangcf
 * initial version
 *
 *
****************************************************************/

#ifndef __ADDR_CONV_H__

#define __ADDR_CONV_H__

#ifdef __CPLUSPLUS
extern "C"
{
#endif

#include <global.h>

char *mac_ntoa(char *mac, char *buf);
void mac_aton(char *buf, char *mac);
char *ip_ntoa(u_int ipaddr, char *buf);
u_int ip_aton(char *buf, u_int *ipaddr);


#ifdef __CPLUSPLUS
}
#endif

#endif
