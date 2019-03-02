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
 * $Revision: 1.9 $
 *
 *	MODIFICATION HISTORY
 * $Log: tcpipsim.c,v $
 * Revision 1.9  2005/09/13 10:37:05  wangcf
 * release 0.4: add toolbar support
 *
 * Revision 1.8  2005/09/13 06:19:16  wangcf
 * tcp/ip stack simulator daily report
 *
 * Revision 1.7  2005/09/11 05:01:42  wangcf
 * save/restore interface address configuration
 *
 * Revision 1.6  2005/09/10 16:58:33  wangcf
 * support configurate ip address dynamic
 *
 * Revision 1.5  2005/09/08 10:26:50  wangcf
 * add interface configuration dialog
 *
 * Revision 1.4  2005/09/07 17:25:56  wangcf
 * 1) add netcard config dialog
 * 2) add bsd system & stack initialization function
 *
 * Revision 1.3  2005/09/07 11:34:12  wangcf
 * tcp/ip stack simulator daily backup
 *
 * Revision 1.2  2005/09/07 11:26:53  wangcf
 * tcp/ip stack simulator daily backup
 *
 *
****************************************************************/

#include <global.h>
#include "tcpipsim.h"
#include "resource.h"
#include <commctrl.h>
#include <assert.h>
#include <process.h>
#include <pcap.h>
#include <remote-ext.h>

HINSTANCE hInst;								// current instance
HWND hWndToolbar = NULL;
TCHAR szTitle[MAX_LOADSTRING];				// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];		// The title bar text

const char *g_prof_name = "tcpipsim.ini";		/* 配置文件名 */
int g_has_wpcap = FALSE;						/* 是否安装pcap软件 */
char g_if_name[MAX_IFNAME_LEN];				/* 选定的网卡名 */

pcap_t *stack_pf = NULL;
int stack_go = FALSE;


int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	MSG msg;
	HACCEL hAccelTable;
	
	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_TCPIPSIM, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	
	// system & network initialization
	sys_init();
	net_init();
	ifaddr_init();
	
	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_TCPIPSIM);

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage is only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc		= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_TCPIPSIM);
	wcex.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCSTR)IDC_TCPIPSIM;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}


//
//   FUNCTION: InitInstance(HANDLE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;
	HMENU hMenu;
	INITCOMMONCONTROLSEX icex;
	HIMAGELIST hIml;
	HIMAGELIST hDisIml;
	TBBUTTON tbArray[5];
	int tbCmd[5] = {IDM_START, IDM_STOP, IDM_NETCARD, IDM_INTERFACE, IDM_EXIT,
	};
	int i;

	hInst = hInstance; // Store instance handle in our global variable

	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icex.dwICC = ICC_COOL_CLASSES|ICC_BAR_CLASSES;
	InitCommonControlsEx(&icex);

	// create toolbar
	hWndToolbar = CreateWindowEx(WS_EX_TOOLWINDOW, TOOLBARCLASSNAME, NULL, 
		WS_CHILD|WS_VISIBLE, 0, 0, 0, 0,
		hWnd, NULL, hInstance, NULL);
	if (hWndToolbar==NULL)
	{
		DestroyWindow(hWnd);
		return FALSE;
	}

	hIml = ImageList_Create(ICON_CX, ICON_CY, ILC_COLOR, 0, 5);
	ImageList_Add(hIml, LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_TOOLBAR)), NULL);
	SendMessage(hWndToolbar, TB_SETIMAGELIST, 0, (LPARAM)hIml);
	hDisIml = ImageList_Create(ICON_CX, ICON_CY, ILC_COLOR, 0, 5);
	ImageList_Add(hDisIml, LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_TOOLBARDIS)), NULL);
	SendMessage(hWndToolbar, TB_SETDISABLEDIMAGELIST, 0, (LPARAM)hDisIml);
	for (i=0; i<sizeof(tbArray)/sizeof(tbArray[0]); i++)
	{
		tbArray[i].iBitmap = i;
		tbArray[i].idCommand = tbCmd[i];
		tbArray[i].fsState = TBSTATE_ENABLED;
		tbArray[i].fsStyle = TBSTYLE_BUTTON;
		tbArray[i].dwData = 0;
		tbArray[i].iString = i;
	}
	SendMessage(hWndToolbar, TB_BUTTONSTRUCTSIZE, sizeof(TBBUTTON), 0);
	SendMessage(hWndToolbar, TB_ADDBUTTONS, (WPARAM)(sizeof(tbArray)/sizeof(TBBUTTON)),
		(LPARAM)tbArray);


	// set menu state
	hMenu = GetMenu(hWnd);
	assert(hMenu!=NULL);
	EnableMenuItem(hMenu, IDM_STOP, MF_BYCOMMAND|MF_GRAYED);
	SendMessage(hWndToolbar, TB_ENABLEBUTTON, IDM_STOP, MAKELONG(FALSE,0));
	
	ShowWindow(hWndToolbar, SW_SHOWMAXIMIZED);
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	HMENU hMenu;
	
	switch (message) 
	{
	case WM_PAINT:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam); 
		wmEvent = HIWORD(wParam); 
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_START:
			hMenu = GetMenu(hWnd);
			assert(hMenu!=NULL);
			EnableMenuItem(hMenu, IDM_START, MF_BYCOMMAND|MF_GRAYED);
			EnableMenuItem(hMenu, IDM_STOP, MF_BYCOMMAND|MF_ENABLED);
			SendMessage(hWndToolbar, TB_ENABLEBUTTON, IDM_START, MAKELONG(FALSE,0));
			SendMessage(hWndToolbar, TB_ENABLEBUTTON, IDM_STOP, MAKELONG(TRUE,0));
			stack_go = TRUE;
			_beginthread(ThreadSim, 64*1024, NULL);
			break;
		case IDM_STOP:
			hMenu = GetMenu(hWnd);
			assert(hMenu!=NULL);
			EnableMenuItem(hMenu, IDM_STOP, MF_BYCOMMAND|MF_GRAYED);
			EnableMenuItem(hMenu, IDM_START, MF_BYCOMMAND|MF_ENABLED);
			SendMessage(hWndToolbar, TB_ENABLEBUTTON, IDM_STOP, MAKELONG(FALSE,0));
			SendMessage(hWndToolbar, TB_ENABLEBUTTON, IDM_START, MAKELONG(TRUE,0));
			stack_go = FALSE;
			break;
		case IDM_ABOUT:
			DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case IDM_NETCARD:
			DialogBox(hInst, (LPCTSTR)IDD_NETCARD, hWnd, (DLGPROC)NetcardConfig);
			break;
		case IDM_INTERFACE:
			DialogBox(hInst, (LPCTSTR)IDD_INTERFACE, hWnd, (DLGPROC)InterfaceConfig);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_DESTROY:
		sys_done();
		ifaddr_done();
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}


int se_pcap_send(char *pkt, int len)
{
	if (stack_pf!=NULL)
	{
		wpcap_send_pkt(stack_pf, pkt, len);
	}
	return 0;	
}

void ThreadSim(void *arg)
{
	int in_pkts;
	char error_buf[PCAP_ERRBUF_SIZE];

	stack_pf = pcap_open(g_if_name, 2000, PCAP_OPENFLAG_PROMISCUOUS, 1, NULL, error_buf);
	if (stack_pf == NULL)
	{
		MessageBox(NULL, "pcap: open adapter error", szTitle, MB_OK|MB_ICONEXCLAMATION);
		goto ret;
	}

	while (stack_go)
	{
		in_pkts = pcap_dispatch(stack_pf, 1, se_pcap_cb, arg);
		if (in_pkts < 0)
		{
			MessageBox(NULL, "pcap: get packet error", szTitle, MB_OK|MB_ICONEXCLAMATION);
			break;
		}
		
	}

ret:
	if (stack_pf != NULL)
	{
		pcap_close(stack_pf);
		stack_pf = NULL;
	}
	return;
}

