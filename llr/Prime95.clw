; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CSplitFrame
LastTemplate=CFrameWnd
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "prime95.h"
LastPage=0

ClassCount=15
Class1=CAffinityDlg
Class2=ContentsDlg
Class3=CCpuDlg
Class4=CEditDropFiles
Class5=CMainFrame
Class6=CPreferencesDlg
Class7=CPrime95App
Class8=CAboutDlg
Class9=CPrime95Doc
Class10=CPrime95View
Class11=CPriority
Class12=CTestDlg

ResourceCount=9
Class13=CScrollDisplay
Class14=CScrollSplit
Resource1=IDD_PREFERENCES (English (U.S.))
Resource2=IDR_TRAYMENU (English (U.S.))
Resource3=IDD_PRIORITY (English (U.S.))
Resource4=IDD_AFFINITY (English (U.S.))
Resource5=IDR_MAINFRAME (English (U.S.))
Resource6=IDD_ABOUTBOX (English (U.S.))
Resource7=IDD_TEST (English (U.S.))
Resource8=IDD_CPU (English (U.S.))
Class15=CSplitFrame
Resource9=IDD_CONTENTS (English (U.S.))

[CLS:CAffinityDlg]
Type=0
BaseClass=CDialog
HeaderFile=AffinityDlg.h
ImplementationFile=AffinityDlg.cpp

[CLS:ContentsDlg]
Type=0
BaseClass=CDialog
HeaderFile=ContentsDlg.h
ImplementationFile=ContentsDlg.cpp

[CLS:CCpuDlg]
Type=0
BaseClass=CDialog
HeaderFile=CpuDlg.h
ImplementationFile=CpuDlg.cpp

[CLS:CEditDropFiles]
Type=0
BaseClass=CEdit
HeaderFile=EditDropFiles.h
ImplementationFile=EditDropFiles.cpp

[CLS:CMainFrame]
Type=0
BaseClass=CFrameWnd
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp

[CLS:CPreferencesDlg]
Type=0
BaseClass=CDialog
HeaderFile=PreferencesDlg.h
ImplementationFile=PreferencesDlg.cpp

[CLS:CPrime95App]
Type=0
BaseClass=CWinApp
HeaderFile=Prime95.h
ImplementationFile=Prime95.cpp
LastObject=CPrime95App

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=Prime95.cpp
ImplementationFile=Prime95.cpp
LastObject=CAboutDlg

[CLS:CPrime95Doc]
Type=0
BaseClass=CDocument
HeaderFile=Prime95Doc.h
ImplementationFile=Prime95Doc.cpp

[CLS:CPrime95View]
Type=0
BaseClass=CView
HeaderFile=Prime95View.h
ImplementationFile=Prime95View.cpp

[CLS:CPriority]
Type=0
BaseClass=CDialog
HeaderFile=Priority.h
ImplementationFile=Priority.cpp

[CLS:CTestDlg]
Type=0
BaseClass=CDialog
HeaderFile=TestDlg.h
ImplementationFile=TestDlg.cpp

[DLG:IDD_AFFINITY]
Type=1
Class=CAffinityDlg

[DLG:IDD_CONTENTS]
Type=1
Class=ContentsDlg

[DLG:IDD_CPU]
Type=1
Class=CCpuDlg

[DLG:IDD_PREFERENCES]
Type=1
Class=CPreferencesDlg

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg

[DLG:IDD_PRIORITY]
Type=1
Class=CPriority

[DLG:IDD_TEST]
Type=1
Class=CTestDlg

[CLS:CScrollDisplay]
Type=0
HeaderFile=ScrollDisplay.h
ImplementationFile=ScrollDisplay.cpp
BaseClass=CScrollView
Filter=C
VirtualFilter=VWC
LastObject=CScrollDisplay

[CLS:CScrollSplit]
Type=0
HeaderFile=ScrollSplit.h
ImplementationFile=ScrollSplit.cpp
BaseClass=CFrameWnd
Filter=T
LastObject=CScrollSplit
VirtualFilter=fWC

[MNU:IDR_TRAYMENU (English (U.S.))]
Type=1
Class=?
Command1=IDM_TRAY_OPEN
Command2=ID_APP_EXIT
CommandCount=2

[MNU:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=IDM_TEST
Command2=IDM_CONTINUE
Command3=IDM_STOP
Command4=ID_APP_EXIT
Command5=IDM_CPU
Command6=IDM_PREFERENCES
Command7=IDM_TRAY
Command8=IDM_HIDE
Command9=IDM_SERVICE
Command10=IDM_ERRCHK
Command11=IDM_PRIORITY
Command12=IDM_AFFINITY
Command13=ID_HELP_CONTENTS
Command14=ID_APP_ABOUT
CommandCount=14

[ACL:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=IDM_STOP
Command2=ID_HELP
Command3=ID_CONTEXT_HELP
CommandCount=3

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=?
ControlCount=9
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352

[DLG:IDD_TEST (English (U.S.))]
Type=1
Class=?
ControlCount=16
Control1=IDC_WORK_PGEN,button,1342373897
Control2=IDC_WORK_EXPR,button,1342177289
Control3=IDC_WORK_EXPRFILE,button,1342177289
Control4=IDC_PGEN_INPUT,edit,1350631552
Control5=IDC_PGEN_OUTPUT,edit,1350631552
Control6=IDC_PGEN_LINE,edit,1350631424
Control7=IDC_EXPR,edit,1350631620
Control8=IDC_EXPRFILE,edit,1350631552
Control9=IDC_EXPRFILE_LINE,edit,1350631424
Control10=IDOK,button,1342242817
Control11=IDCANCEL,button,1342242816
Control12=IDS_PGEN_INPUT,static,1342308352
Control13=IDS_PGEN_LINE,static,1342308352
Control14=IDS_EXPRFILE,static,1342308352
Control15=IDS_EXPRFILE_LINE,static,1342308352
Control16=IDS_PGEN_OUTPUT,static,1342308352

[DLG:IDD_CPU (English (U.S.))]
Type=1
Class=?
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,button,1342177287
Control4=IDC_CPU_INFO,static,1342308352

[DLG:IDD_PREFERENCES (English (U.S.))]
Type=1
Class=?
ControlCount=11
Control1=IDC_P,edit,1350631424
Control2=IDC_R_ITER,edit,1350631424
Control3=IDC_DISK,edit,1350631424
Control4=IDC_CUMULATIVE,button,1342242819
Control5=IDC_BACKUP,button,1342242819
Control6=IDC_BATTERY,button,1342242819
Control7=IDOK,button,1342242817
Control8=IDCANCEL,button,1342242816
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC,static,1342308352
Control11=IDC_STATIC,static,1342308352

[DLG:IDD_PRIORITY (English (U.S.))]
Type=1
Class=?
ControlCount=7
Control1=IDC_PRIORITY,edit,1350631552
Control2=IDOK,button,1342242817
Control3=IDCANCEL,button,1342242816
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352

[DLG:IDD_AFFINITY (English (U.S.))]
Type=1
Class=?
ControlCount=7
Control1=IDC_ALL_CPUS,button,1342242819
Control2=IDC_AFFINITY,edit,1350631552
Control3=IDOK,button,1342242817
Control4=IDCANCEL,button,1342242816
Control5=65535,static,1342308352
Control6=65535,static,1342308352
Control7=IDC_AFFINITY_TEXT,static,1342308352

[DLG:IDD_CONTENTS (English (U.S.))]
Type=1
Class=?
ControlCount=2
Control1=IDOK,button,1342242817
Control2=IDC_HELP_TEXT,edit,1352730628

[CLS:CSplitFrame]
Type=0
HeaderFile=SplitFrame.h
ImplementationFile=SplitFrame.cpp
BaseClass=CFrameWnd
Filter=T
LastObject=CSplitFrame
VirtualFilter=fWC

