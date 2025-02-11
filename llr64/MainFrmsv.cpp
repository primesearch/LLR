// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Prime95.h"

#include "MainFrm.h"
#include <winreg.h>
//#include <pbt.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_SIZE()
	ON_WM_ENDSESSION()
	ON_WM_ACTIVATEAPP()
	//}}AFX_MSG_MAP
	ON_WM_SYSCOMMAND()
	// Global help commands
	ON_COMMAND(ID_HELP_FINDER, CFrameWnd::OnHelpFinder)
	ON_COMMAND(ID_HELP, CFrameWnd::OnHelp)
	ON_COMMAND(ID_CONTEXT_HELP, CFrameWnd::OnContextHelp)
	ON_COMMAND(ID_DEFAULT_HELP, CFrameWnd::OnHelpFinder)
	ON_COMMAND(IDM_TRAY_OPEN, OnTrayOpenWindow)
	ON_MESSAGE(USR_SERVICE_STOP, OnServiceStop)
	ON_MESSAGE(WM_POWERBROADCAST, OnPower)
	ON_MESSAGE(MYWM_TRAYMESSAGE, OnTrayMessage)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	
}

CMainFrame::~CMainFrame()
{
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFrameWnd::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers
LRESULT CMainFrame::OnPower(WPARAM uID, LPARAM uMouseMsg)
{
	if (uID == PBT_APMPOWERSTATUSCHANGE) {
		CHECK_BATTERY = 1;
		if (STOPPED_ON_BATTERY) {
			STOPPED_ON_BATTERY = 999;
			PostMessage (WM_COMMAND, IDM_CONTINUE, 0);
		}
	}
	return 0;
}

LRESULT CMainFrame::OnTrayMessage(WPARAM uID, LPARAM uMouseMsg)
{
	if (uID == 352 && uMouseMsg == WM_LBUTTONDBLCLK)
		if (IsWindowVisible())
			ShowWindow(FALSE);	// hide it
		else {
			ShowWindow(TRUE);	// show it
			SetForegroundWindow();
		}

	if (uID == 352 && uMouseMsg == WM_RBUTTONUP) {
		POINT	point;
		HMENU	hMenu, hSubMenu;

		GetCursorPos (&point);		// Get cursor location
		hMenu = LoadMenu (AfxGetApp()->m_hInstance,
				  MAKEINTRESOURCE (IDR_TRAYMENU));
		hSubMenu = GetSubMenu (hMenu, 0);
		if (!IsIconic ()) {
			MENUITEMINFO m;
			m.cbSize = sizeof (m);
			m.fMask = MIIM_STRING;
			m.fType = MFT_STRING;
			m.dwTypeData = "Minimize Window";
			m.cch = strlen (m.dwTypeData);
			SetMenuItemInfo (hSubMenu, IDM_TRAY_OPEN, FALSE, &m);
		}
		SetMenuDefaultItem (hSubMenu, IDM_TRAY_OPEN, FALSE);
		SetForegroundWindow ();	// Per KB Article Q135788
		TrackPopupMenu (hSubMenu,
			TPM_LEFTBUTTON | TPM_RIGHTBUTTON | TPM_LEFTALIGN,
			point.x, point.y, 0, m_hWnd, NULL);
		PostMessage (WM_NULL, 0, 0);// Per KB Article Q135788
		DestroyMenu (hMenu);
	}

	return 0;
}

void CMainFrame::OnTrayOpenWindow() 
{
	if (IsIconic ())
		PostMessage (WM_SYSCOMMAND, SC_RESTORE, 0L);
	else
		PostMessage (WM_SYSCOMMAND, SC_MINIMIZE, 0L);
}

LRESULT CMainFrame::OnServiceStop (WPARAM wParam, LPARAM lParam)
{
	if (TRAY_ICON) ((CPrime95App *)AfxGetApp())->TrayMessage (NIM_DELETE, NULL, 0);
	ShowWindow (FALSE);
	CloseHandle (g_hMutexInst);
	return (0);
}

BOOL CMainFrame::DestroyWindow() 
{
	if (TRAY_ICON) ((CPrime95App *)AfxGetApp())->TrayMessage (NIM_DELETE, NULL, 0);
	return CFrameWnd::DestroyWindow();
}

void CMainFrame::OnSize(UINT nType, int cx, int cy) 
{
	if (nType == SIZE_MINIMIZED) {
		if (TRAY_ICON) ShowWindow (FALSE);// hide it
		if (HIDE_ICON) ShowWindow (FALSE);
	}
	CFrameWnd::OnSize(nType, cx, cy);
}

LRESULT CMainFrame::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	WM_ENDSESSION_LPARAM = lParam;
	return CFrameWnd::WindowProc(message, wParam, lParam);
}

// Special WM_ENDSESSION code from knowledge base article Q164166

static const TCHAR szAfxOldWndProc[] = _T("AfxOldWndProc423");
BOOL CALLBACK EnumProc (HWND hWnd, LPARAM lParam)
{
	//check for property and unsubclass if necessary
	WNDPROC oldWndProc = (WNDPROC)::GetProp (hWnd, szAfxOldWndProc);
	if (oldWndProc!=NULL) {
		SetWindowLong (hWnd, GWLP_WNDPROC, (DWORD) oldWndProc);
		RemoveProp (hWnd, szAfxOldWndProc);
	}
	return TRUE;
}

void CMainFrame::OnEndSession(BOOL bEnding) 
{

// If we are running as a service, then do not end the program at logoff
// Instead, just remove the icon from the system tray.

	if (NTSERVICENAME[0] ||
	    (WINDOWS95_SERVICE && WM_ENDSESSION_LPARAM && isWindows95 ())) {
		if (TRAY_ICON) {
			((CPrime95App *)AfxGetApp())->TrayMessage (NIM_DELETE, NULL, 0);
			WINDOWS95_TRAY_ADD = 1;
		}

// In addition a Windows NT service must take special actions.  MFC was
// not designed to be used in a NT service as it uses Global Atoms which are
// cleared at logoff.  This fix from knowledge base article Q164166 seems
// to fix the problem.

		if (NTSERVICENAME[0]) {
			DWORD	dwProcessId, dwThreadId;
			dwThreadId = GetWindowThreadProcessId (m_hWnd, &dwProcessId);
			EnumThreadWindows (dwThreadId, EnumProc, (LPARAM) dwThreadId);
		}
	}

// If we aren't running as a service, just do normal processing

	else
		CFrameWnd::OnEndSession (bEnding);
}

/* Handle all the details for running prime95 as a service */

#include <winsvc.h>
#define RSP_SIMPLE_SERVICE	1
#define RSP_UNREGISTER_SERVICE	0

void Service95 ()
{
	char	pathname[256];
	char	regkey[20];		/* Win9x registry name */
	SC_HANDLE schSCManager = 0;
	SC_HANDLE schService = 0;
	HKEY	hkey = 0;
	DWORD	rc, disposition;

/* In Windows 95/98/Me, call RegisterServiceProcess in the Kernel */
/* This will prevent prime95 from terminating on logoff. */

	if (isWindows95()) {
		HMODULE	hlib;
		DWORD (__stdcall *proc)(DWORD, DWORD);

		hlib = LoadLibrary ("KERNEL32.DLL");
		if (!hlib) {
			OutputStr ("Unable to load KERNEL32.DLL\n");
			goto done;
		}
		proc = (DWORD (__stdcall *)(DWORD, DWORD))
			GetProcAddress (hlib, "RegisterServiceProcess");
		if (proc == NULL)
			OutputStr ("Unable to find RegisterServiceProcess\n");
		else {
			if (WINDOWS95_SERVICE)
				rc = (*proc) (NULL, RSP_SIMPLE_SERVICE);
			else
				rc = (*proc) (NULL, RSP_UNREGISTER_SERVICE);
			if (!rc)
				OutputStr ("RegisterServiceProcess failed\n");
		}
		FreeLibrary (hlib);
	}

/* Now we deal with making the registry entries correct for proper starting */
/* or not starting of the service. */

/* Get pathname of executable */

	GetModuleFileName(NULL, pathname, sizeof(pathname));

/* In Win95/98/Me, we create a registry entry for each -A command line value */
/* We used to do this in WinNT/2000/XP, but now we just delete these old */
/* registry entries. */

	if (WINDOWS95_A_SWITCH < 0)
		strcpy (regkey, "LLR");
	else
		sprintf (regkey, "LLR-%d", WINDOWS95_A_SWITCH);

// For Windows NT/2000/XP we call the service control manager to
// maintain the services database

	if (! isWindows95 ()) {
		char	servicename[32];
		char	displayname[32];

// Create the Windows NT service name and display name

		IniGetString (INI_FILE, "ServiceName", servicename,
			      sizeof (servicename), NULL);
		if (servicename[0] == 0) {
			if (WINDOWS95_A_SWITCH < 0)
				strcpy (servicename, "LLR Service");
			else
				sprintf (servicename, "LLR Service-%d",
					 WINDOWS95_A_SWITCH);
		}

		IniGetString (INI_FILE, "DisplayName", displayname,
			      sizeof (displayname), servicename);

// Open connection to the service control manager

		schSCManager = OpenSCManager (
				NULL,		// machine (NULL == local)
				NULL,		// database (NULL == default)
				SC_MANAGER_ALL_ACCESS);	// access required
		if (! schSCManager) {
			OutputStr ("Couldn't open service control manager.\n");
			goto done;
		}

// Create the service entry

		if (WINDOWS95_SERVICE) {
			schService = CreateService (
				schSCManager,		// SCManager database
				servicename,	        // name of service
				displayname,		// display name
				SERVICE_ALL_ACCESS,	// desired access
				SERVICE_INTERACTIVE_PROCESS |
				SERVICE_WIN32_OWN_PROCESS,  // service type
				SERVICE_AUTO_START,	// start type
				SERVICE_ERROR_NORMAL,	// error control type
				pathname,		// service's binary
				NULL,			// no load ordering
				NULL,			// no tag identifier
				NULL,			// no dependencies
				NULL,			// LocalSystem account
				NULL);			// no password
			if (!schService) {
				if (GetLastError () != ERROR_SERVICE_EXISTS)
					OutputStr ("Error creating service.\n");
				goto done;
			}

// Set description for Win2K and later
			if (isWindows2000 ()) {
				SERVICE_DESCRIPTION svc_desc;
				svc_desc.lpDescription =
					"k*2^n-1 Prime Tests";
				ChangeServiceConfig2 (
					schService,
					SERVICE_CONFIG_DESCRIPTION,
					&svc_desc);
			}
		}

// Remove the service entry

		else {
			schService = OpenService (
					schSCManager,
					servicename,
					SERVICE_ALL_ACCESS);
			if (!schService) {
				if (GetLastError () != ERROR_SERVICE_DOES_NOT_EXIST)
					OutputStr ("Error opening service.\n");
				goto done;
			}
			if (! DeleteService (schService)) {
				OutputStr ("Error deleting service.\n");
				goto done;
			}
		}
	}

// For Windows 95/98/Me create a RunServices entry

	else {
		if (RegCreateKeyEx (
				HKEY_LOCAL_MACHINE,
				"Software\\Microsoft\\Windows\\CurrentVersion\\RunServices",
				0,
				NULL,
				REG_OPTION_NON_VOLATILE,
				KEY_ALL_ACCESS,
				NULL,
				&hkey,
				&disposition) != ERROR_SUCCESS) {
			OutputStr ("Can't create registry key.\n");
			goto done;
		}

/* Now create or delete an entry for prime95 */

		if (WINDOWS95_SERVICE) {
			rc = RegSetValueEx (hkey, regkey, 0, REG_SZ,
				(BYTE *) pathname, strlen (pathname) + 1);
			if (rc != ERROR_SUCCESS) {
				OutputStr ("Can't write registry value.\n");
				goto done;
			}
		} else {
			rc = RegDeleteValue (hkey, regkey);
			if (rc != ERROR_SUCCESS && rc != ERROR_FILE_NOT_FOUND){
				OutputStr ("Can't delete registry entry.\n");
				goto done;
			}
		}
		RegCloseKey (hkey);
		hkey = 0;
	}

// Cleanup and return

done:	if (schService) CloseServiceHandle (schService);
	if (schSCManager) CloseServiceHandle (schSCManager);
	if (hkey) RegCloseKey (hkey);
}


void CMainFrame::OnActivateApp(BOOL bActive, DWORD hTask) 
{
#if (_MSC_VER < 1600)
	CFrameWnd::OnActivateApp(bActive, (HTASK__ *)hTask);
#else
	CFrameWnd::OnActivateApp(bActive, hTask);
#endif
}

// Override the Upper Right X to do a minimize instead of a close.
// This has become common practice for tray applications.
// But default ExitOnX option to one. J.P. 08/12/04

void CMainFrame::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == SC_CLOSE &&
	    ! IniGetInt (INI_FILE, "ExitOnX", 1) &&
	    (TRAY_ICON || HIDE_ICON)) {
		SendMessage (WM_SYSCOMMAND, SC_MINIMIZE, 0);
		return;
	}
	CFrameWnd::OnSysCommand (nID, lParam);
}

BOOL CMainFrame::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle , const RECT& rect , CWnd* pParentWnd , LPCTSTR lpszMenuName , DWORD dwExStyle , CCreateContext* pContext)
{
	BOOL	retval;
	retval = CFrameWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, lpszMenuName, dwExStyle, pContext);
//	MAINFRAME_HWND = m_hWnd;
	return (retval);
}
