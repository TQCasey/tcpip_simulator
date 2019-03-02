/****************************************************************
 *
 *	Copyleft (C) 2005 Wangcf, No Right Reserved
 *		E-mail: wangcf@publicb.bta.net.cn
 *
 * $Revision: 1.7 $
 *
 *	PURPOSE:
 *		Enter file description here
 *
 *	MODIFICATION HISTORY
 * $Log: tcpipsim.h,v $
 * Revision 1.7  2005/09/13 10:37:05  wangcf
 * release 0.4: add toolbar support
 *
 * Revision 1.6  2005/09/13 06:19:16  wangcf
 * tcp/ip stack simulator daily report
 *
 * Revision 1.5  2005/09/10 16:58:33  wangcf
 * support configurate ip address dynamic
 *
 * Revision 1.4  2005/09/08 10:26:50  wangcf
 * add interface configuration dialog
 *
 * Revision 1.3  2005/09/07 17:25:56  wangcf
 * 1) add netcard config dialog
 * 2) add bsd system & stack initialization function
 *
 * Revision 1.2  2005/09/07 11:26:53  wangcf
 * tcp/ip stack simulator daily backup
 *
 * Revision 1.1.1.1  2005/09/06 16:40:11  wangcf
 * initial version
 *
 *
****************************************************************/

#ifndef __TCPIPSIM_H__
#define __TCPIPSIM_H__

#include "resource.h"
#include <pcap.h>

#define MAX_LOADSTRING 	100
#define MAX_IFNAME_LEN		256
#define NSE					2
#define MAX_IA_NUM			16

#define ICON_CX				16
#define ICON_CY				16

struct ia_addr
{
	unsigned int ip;
	unsigned int mask;
};
typedef struct str_IfAddr
{
	int ia_num;
	struct ia_addr ia[MAX_IA_NUM];
}IfAddr_t;
extern IfAddr_t	ia_list[NSE];

/* section name of profile */
#define SECTION_COMMON		"common"
#define SECTION_PCAP		"pcap"

extern HINSTANCE hInst;
extern TCHAR szTitle[MAX_LOADSTRING];
extern TCHAR szWindowClass[MAX_LOADSTRING];

extern int g_major_version;
extern int g_minor_version;
extern char *g_build_date;

extern const char *g_prof_name;
extern int g_has_wpcap;
extern char g_if_name[MAX_IFNAME_LEN];

ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE hInstance, int nCmdShow);
LRESULT CALLBACK	WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK	About(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void load_wpcap();
void wpcap_send_pkt(pcap_t *pf, char *pkt, int len);
int sys_init();
void sys_done();
LRESULT CALLBACK NetcardConfig(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK InterfaceConfig(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ToolbarProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void bsd_init();
void bsd_done();
void net_init();
void net_done();
int ifaddr_add(int index, unsigned int ipaddr, unsigned int netmask);
int ifaddr_del(int index, unsigned int ipaddr);
void ifaddr_init();
void ifaddr_done();

int se_pcap_send(char *pkt, int len);
void ThreadSim(void *arg);
void se_pcap_cb(unsigned char *user, const struct pcap_pkthdr *phdr, const unsigned char *pdata);

#endif 
