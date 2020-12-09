// ARcBridge.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include <afxinet.h>
#include <direct.h> 
#include "ARcBridge.h"

#include "MainFrm.h"
#include "ARcBridgeDoc.h"
#include "ARcBridgeView.h"
#include "Splash.h"
#include "Shlwapi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define LOCK_FEATURE_ID 4500
const TCHAR szARcBridgeClass[] = _T("ARcBridgeClass");
/////////////////////////////////////////////////////////////////////////////
// CARcBridgeApp

BEGIN_MESSAGE_MAP(CARcBridgeApp, CWinApp)
	//{{AFX_MSG_MAP(CARcBridgeApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CARcBridgeApp construction

CARcBridgeApp::CARcBridgeApp(LPCTSTR lpszAppName)
{
//	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF ); //디버그에 메모리립이 생기면 프로그램이 종료되고 알려줄때 사용됨.

//	_CrtSetBreakAlloc(1231861); //10번 메모리가 할당될때 중단점을 설정함.


	if (lpszAppName != NULL)
		m_pszAppName = _tcsdup(lpszAppName);
	else
		m_pszAppName = NULL;

	m_strFileOpenPathName	= _T("");

	aLockInit(UNIQ_ID_ARCBRIDGE, SPLOCK_ARCBRIDGE_DID, SPADDR_SL_ARCBRIDGE, NULL, 68, BUILDDATE, "login_ARcBridge.swf");

	LDK::SLockInitInfo LockInfo = {
//		FID_ARCBRIDGE, 
//		HASP_INVALID_HANDLE_VALUE,
		"ARcBridge",
		UNIQ_ID_ARCBRIDGE,
		PID_ARCBRIDGE,
		LDK::aLoginDlgProc,		
		BUILDDATE,
		LICENSE_NULL};

	LDK::aLockInit(LockInfo); //LDK 초기화 함수

}

/////////////////////////////////////////////////////////////////////////////
// The one and only CARcBridgeApp object

CARcBridgeApp theApp("ARcBridge2013");

/////////////////////////////////////////////////////////////////////////////
// CARcBridgeApp initialization

BOOL CARcBridgeApp::InitInstance()
{
	// CG: The following block was added by the Splash Screen component.
	{
		CCommandLineInfo cmdInfo;
		ParseCommandLine(cmdInfo);
		CSplashWnd::EnableSplashScreen(cmdInfo.m_bShowSplash);
	}
	CString errMsg = _T("오류가 발생했습니다.");
#ifdef _DEBUG
#else
	BEGIN_AEXCEPTION();
#endif 


	AfxEnableControlContainer();
	AfxGetModuleState()->m_dwVersion = 0x0601;
	AfxInitRichEdit();
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

// #ifdef _AFXDLL
// 	Enable3dControls();			// Call this when using MFC in a shared DLL
// #else
// 	Enable3dControlsStatic();	// Call this when linking to MFC statically
// #endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("HangilIT"));

	LoadStdProfileSettings(8);  // Load standard INI file options (including MRU)

	SetFixedDirectory();

	CHgModelRenderApi::LoadFont();	// 디폴트 폰트 읽기

// 	unsigned short int OldLockModule;
// 	if(!aCheckLock(HANGIL_ARCBRIDGE, SPADDR_MF_ARCBRIDGE, SPLOCK_ARCBRIDGE_RCODE, SPLOCK_ARCBRIDGE_RCODE, OldLockModule,TRUE,FALSE))
// 	{		
// 		if (!::aLockCheckResultAndNotice(NULL, FALSE))
// 			return FALSE;
// 	}
// 
// 	LDK::aSetLockSerial(aGetLockProductSerial());


	unsigned short int OldLockModule;
	if(aCheckLock(HANGIL_ARCBRIDGE, SPADDR_MF_ARCBRIDGE, SPLOCK_ARCBRIDGE_RCODE, SPLOCK_ARCBRIDGE_RCODE, OldLockModule,TRUE,FALSE))
	{		
		if (!::aLockCheckResultAndNotice(NULL, FALSE))
			return FALSE;

		LDK::aSetLockSerial(aGetLockProductSerial());
	}
	else if (LDK::aLockLType(LDK::eSLnRegistration) && !LDK::aLockLogin(LOCK_FEATURE_ID))	
	{
		if (!LDK::aLDKNoticeLock(LDK::aGetLockInfo(), LDK::aGetLockResult()))		
			return FALSE;		
	}	
	

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CARcBridgeDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CARcBridgeView));
	AddDocTemplate(pDocTemplate);
	
	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);	

	// INSERT_ATESTCODE argv 두번째 인자를 쓰기위해서
	if(cmdInfo.m_nShellCommand == CCommandLineInfo::FileOpen)
	{		
		HGINT32 iIndex= cmdInfo.m_strFileName.ReverseFind(_T('.'));
		CString strTmp = cmdInfo.m_strFileName.Mid(iIndex +1);
		strTmp.MakeLower();
		if(strTmp != _T("arb")) //확장자 .( 나중에 바뀔일잇으면 바꿔야함)
			cmdInfo.m_nShellCommand = CCommandLineInfo::FileNew;
	}


	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;	

	// INSERT_ATESTCODE
	ATEST_INST(m_lpCmdLine, m_pMainWnd);


	// 메인프레임 생성후 plus인지 아닌지에 따라 메인프레임 타이틀 설정 ///////////
	CString sPlus	= GetARcBridgeName();
	CFrameWnd *pFrame	= (CFrameWnd *)m_pMainWnd;
	pFrame->SetTitle(sPlus);
	pFrame->SetWindowText(sPlus);
	///////////////////////////////////////////////////////////////////////////////


	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);
	m_pMainWnd->UpdateWindow();

	//==============================================================================================
	//
	//  Lock Check Module
	//
	//==============================================================================================
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CDocument*  pDoc = pMainFrame->GetActiveDocument();

	//----------------------------------------------------------------------------------------------
	//  STEP1: Lock Check 실패시, 종료
	//----------------------------------------------------------------------------------------------
	if (LDK::aIsLogIn())//SLLock_20150526
	{
		if (!LDK::aLockCheckResultAndNotice(pDoc, FALSE)) // 2015. 07. 28. Software Lock 작업, 김형준	
			return FALSE;
	}
	else
	{
		if (!::aLockCheckResultAndNotice(pDoc, FALSE))
			return FALSE;
	}	

	//----------------------------------------------------------------------------------------------
	//  STEP2: Lock 검사 Timer 가동
	//----------------------------------------------------------------------------------------------
	pMainFrame->SetTimer(SPLOCK_TIMER_ID, SPLOCK_TIMER_INTERVAL, NULL);

	CString szRunTime;
	if(LDK::aCheckLicenseHGKey())	
	{
		if (LDK::aIsLogIn())//SLLock_20150526
		{
			LDK::aUpdateCheck();
		}
		else
		{
			aUpdateCheck("ARcBridge", atoi(GetProfileString("Program Option", "CheckVersion","")), szRunTime);
			if(!szRunTime.IsEmpty())	
				WriteProfileString("Settings", "LastRunDate",szRunTime);
		}
	}

	AfxBeginThread(aSendRunInfo,NULL);
	LDK::aCheckLoseLockAndDestoryKey();
	
	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles( );

	// Enable DDE Execute open
	EnableShellOpen( );
	RegisterShellFileTypes( TRUE );

	// 제품버전을 타이틀에 표시... ///////////
	CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();
	if(pFrm)
	{
		CFrameWnd *pFrameWnd	= (CFrameWnd *)m_pMainWnd;
		CString sTitle = _T("");
		sTitle.Format("ARcBridge %s",GetStringLockInfo());
		pFrameWnd->SetTitle(sTitle);
		pFrameWnd->SetWindowText(sTitle);
	}

	pMainFrame->UpdateLockInfo();

	if(!LDK::aCheckLicenseHGKey())
		pMainFrame->PostMessage(WM_COMMAND, ID_CREATE_LICENSE);
	
#ifdef _DEBUG
#else
	END_AEXCEPTION(errMsg);
#endif
	
	// INSERT_ATESTCODE
	::PostMessage(AfxGetMainWnd()->GetSafeHwnd(), WM_ATT_MAINMSG,eMsgAppFinishInstance,0);
	return TRUE;
}

// App command to run the dialog
void CARcBridgeApp::OnAppAbout()
{
	CString sVersion = GetStringVersion();
	CString sLockInfo = GetStringLockInfo();		// ARcBridge는 프로그램 이름에 버전이 표현됨(Plus)
	CString sEtcInfo = GetStringEtcInfo();

	CDialogAbout	Dlg;
	Dlg.SetProgramName(GetARcBridgeName());
	Dlg.SetLockInfo(sLockInfo);
	Dlg.SetEtcInfo(_T(""));
	Dlg.SetVersion(sVersion);
	Dlg.DoModal();
}


CString CARcBridgeApp::GetStringVersion()
{
	CString sVersion = VERSION;
	// Master Lock일 경우 빌드넘버도 보여준다. 
	if(aGetLockID() == SPLOCK_MASTER_LID)
	{
		sVersion.Format("%s(%s)", VERSION, BUILDNUMBER);
	}

	return sVersion;
}

/////////////////////////////////////////////////////////////////////////////
// CARcBridgeApp message handlers

UINT CARcBridgeApp::GetMRUListSize()
{
	UINT nListSize = 0;
	UINT nExistSize = 0;
	if(!m_pRecentFileList)
		return nExistSize;

	CString strName = _T("");
	nListSize = m_pRecentFileList->GetSize();

	for(UINT i=0; i<nListSize; i++)
	{
		strName = GetMRUName(i, FALSE);

		if(strName!=_T(""))
			nExistSize++;
	}

	return nExistSize;
}

CString CARcBridgeApp::GetMRUName(UINT nIndex, BOOL bFullPath)
{
	CString strName = _T("");

	if(bFullPath)
	{
		strName = (*m_pRecentFileList)[nIndex];
	}
	else
	{
		m_pRecentFileList->m_nMaxDisplayLength = 0;
		m_pRecentFileList->GetDisplayName(strName, nIndex, _T(""), 0);
	}

	return strName;
}

void CARcBridgeApp::OnFileOpen() 
{	
	//31652 
	CString sFilter;
	sFilter.Format(_T("ARcBridge 파일 (*.arb)|*.arb|All files(*.*) | *.*||")); 

	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, sFilter);

	//CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("ARcBridge 파일 (*.arb)|*.arb||"));
	CString strPath = m_strFileOpenPathName == "" ? GetWorkDirectory() : m_strFileOpenPathName;
	dlg.m_ofn.lpstrInitialDir = strPath.GetBuffer(strPath.GetLength());

	if(dlg.DoModal()==IDOK)
	{
		CString strPathName = dlg.GetPathName();
		OpenDocumentFile(strPathName);		

//-		CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();
//-		CXTPShortcutBarItem* pSelItem = pFrm->m_wndShortcutBar.GetItem(1);
//-		pFrm->m_wndShortcutBar.SelectItem(pSelItem);

		DeleteWMF();
	}
}

void CARcBridgeApp::OnFileNew() 
{
	CWinApp::OnFileNew();
}

void CARcBridgeApp::SetFixedDirectory()
{
	CWinApp* pApp = AfxGetApp();
	CString strSection = _T("Afx Directory");
	CString strFixedItem = _T("Fixed Directory");
	CString strWorkItem = _T("Work Directory");

	////////////////////////////////////////////////////////////////
	// Set AShop Directory(Fixed Directory) 
	// Only Write Current Path in Registry
	CString strFixedDir = pApp->GetProfileString(strSection, strFixedItem);
	BOOL bFirst = strFixedDir.GetLength()==0 ? TRUE : FALSE;
	if(bFirst)
	{
		char Buf[1024];
		::GetCurrentDirectory(1024, Buf);
		pApp->WriteProfileString(strSection, strFixedItem, Buf);
		strFixedDir = Buf;
	}

	//////////////////////////////////////////////////////////////////
	// Set Work Directory

	// Read and Work Directory
	// #include "direct.h"
	// int _chdir( const char_t *dirname );
	CString strWork = _T("");
	CString strRcWork = pApp->GetProfileString(strSection, strWorkItem);
	if(strRcWork==_T(""))
	{
		strRcWork = "C:\\ARcBridgeWork";
		pApp->WriteProfileString(strSection, strWorkItem, strRcWork);
/*
		_chdrive(3);
		TCHAR szPath[MAX_PATH] = {0,};
		SHGetSpecialFolderPath(NULL, szPath, CSIDL_PERSONAL, TRUE);
		CString str = szPath;
		_chdir(str);
		_mkdir(_T("내 설계"));
		strWork = str+_T("\\내 설계");
		_chdir(strWork);
		_mkdir(_T("ARcBridge"));
		strRcWork = strWork + _T("\\ARcBridge");
		pApp->WriteProfileString(strSection, strWorkItem, strRcWork);

		PathMakeSystemFolder(strWork);
		// Desktop.ini 파일 생성
		CFile file;
		if(file.Open(str+_T("\\내 설계\\Desktop.ini"), CFile::modeCreate|CFile::modeWrite))
		{
			CString strTitle = _T("[.ShellClassInfo]\r\n");
			CString strInfoTip = _T("InfoTip=교량 설계용 파일이 들어 있습니다.\r\n");
			CString strFixed = GetProfileString(strSection, strFixedItem);
			CString strIconFile = _T("IconFile=%SystemDrive%")+strFixed.Right(strFixed.GetLength()-2)+_T("\\work.ico\r\n");
			CString strIconIndex = _T("IconIndex=0\r\n");
			file.Write(strTitle, strTitle.GetLength());
			file.Write(strInfoTip, strInfoTip.GetLength());
			file.Write(strIconFile, strIconFile.GetLength());
			file.Write(strIconIndex, strIconIndex.GetLength());
			file.Close();
		}
		SetFileAttributes(strWork+_T("\\Desktop.ini"), FILE_ATTRIBUTE_HIDDEN|FILE_ATTRIBUTE_READONLY);
*/
	}
	_chdrive(3);

    if(IsRegFileExt()==FALSE)
	{
		CString strValue[4];
		strValue[0] = _T(".arb");
		strValue[1] = _T("ARcBridge File");
		strValue[2] = _T("ARcBridge 파일");
		strValue[3] = strFixedDir;
		SetRegFileExt(strValue);

		strValue[0] = _T(".dgk");
		strValue[1] = _T("Dogak File");
		strValue[2] = _T("도각 파일");
		strValue[3] = strFixedDir;
		SetRegFileExt(strValue, 1, FALSE);

		strValue[0] = _T(".lin");
		strValue[1] = _T("ALine File");
		strValue[2] = _T("ALine 파일");
		strValue[3] = strFixedDir;
		SetRegFileExt(strValue, 2, FALSE);

		strValue[0] = _T(".aro");
		strValue[1] = _T("ARcBridge Drawing Option File");
		strValue[2] = _T("ARcBridge 도면 옵션 파일");
		strValue[3] = strFixedDir;
		SetRegFileExt(strValue, 3, FALSE);

		strValue[0] = _T(".ard");
		strValue[1] = _T("ARcBridge Drawing Arrangement File");
		strValue[2] = _T("ARcBridge 도면 배치 파일");
		strValue[3] = strFixedDir;
		SetRegFileExt(strValue, 4, FALSE);

		strValue[0] = ".rub";
		strValue[1] = "ARcBridge User Defined BM File";
		strValue[2] = "ARcBridge 사용자 정의 수량 파일";
		strValue[3] = strFixedDir;
		SetRegFileExt(strValue, 5, FALSE);

		strValue[0] = ".opt";
		strValue[1] = "Drawing Format File";
		strValue[2] = "도면 형식 파일";
		strValue[3] = strFixedDir;
		SetRegFileExt(strValue, 6, FALSE);

		strValue[0] = ".rbd";
		strValue[1] = "ARcBridge Basic Data File";
		strValue[2] = "ARcBridge 기초 자료 파일";
		strValue[3] = strFixedDir;
		SetRegFileExt(strValue, 7, FALSE);
	}
}

BOOL CARcBridgeApp::IsRegFileExt() const
{
	HKEY hSecKey = HKEY_CLASSES_ROOT;
	HKEY nExtKey = NULL;
	CString strValue = _T("");
	DWORD dwType, dwCount, dw;

	if(RegCreateKeyEx(hSecKey, _T("ARcBridge File"), 0, REG_NONE,REG_OPTION_NON_VOLATILE,
		KEY_WRITE|KEY_READ, NULL, &nExtKey, &dw)==ERROR_SUCCESS)
	{
		LONG lResult = RegQueryValueEx(nExtKey, NULL,	NULL, &dwType, NULL, &dwCount);

		if(lResult==ERROR_SUCCESS)
		{
			lResult = RegQueryValueEx(nExtKey, NULL, NULL, &dwType,
				(LPBYTE)strValue.GetBuffer(dwCount/sizeof(TCHAR)), &dwCount);
			strValue.ReleaseBuffer();
		}
	}
	
	if(hSecKey!=NULL) RegCloseKey(hSecKey);

	if(strValue.GetLength()>0) return TRUE;
	return FALSE;
}

void CARcBridgeApp::SetRegFileExt(CString strValue[4], long nIcon/* =0 */, BOOL bOpen/* =TRUE */)
{
	CWinApp* pApp = AfxGetApp();
	HKEY nExtKey = NULL;
	HKEY nDefaultIconkey = NULL;
	HKEY nShellKey = NULL;
	HKEY nOpenKey = NULL;
	HKEY nCommandKey = NULL;
	HKEY nPrintKey = NULL;

	DWORD dw;
	if(RegCreateKeyEx(HKEY_CLASSES_ROOT, strValue[0], 0, REG_NONE,REG_OPTION_NON_VOLATILE,
		KEY_WRITE|KEY_READ, NULL, &nExtKey, &dw)==ERROR_SUCCESS)
	{
		LPCTSTR lpszValue = (LPCTSTR)strValue[1];
		RegSetValueEx(nExtKey, NULL, 0, REG_SZ, (LPBYTE)lpszValue, (lstrlen(lpszValue)+1)*sizeof(TCHAR));
	}
	if(nExtKey!=NULL) RegCloseKey(nExtKey);

	nExtKey = NULL;
	CString strIconKey = _T("");
	if(nIcon!=0) strIconKey.Format(_T(", %d"), nIcon);
	if(RegCreateKeyEx(HKEY_CLASSES_ROOT, strValue[1], 0, REG_NONE, REG_OPTION_NON_VOLATILE,
		KEY_WRITE|KEY_READ,NULL, &nExtKey, &dw)==ERROR_SUCCESS)
	{
		LPCTSTR lpszValue = (LPCTSTR)strValue[2];
		RegSetValueEx(nExtKey, NULL, 0, REG_SZ, (LPBYTE)lpszValue, (lstrlen(lpszValue)+1)*sizeof(TCHAR));

		if(RegCreateKeyEx(nExtKey, _T("DefaultIcon"), 0, REG_NONE, REG_OPTION_NON_VOLATILE,
			KEY_WRITE|KEY_READ, NULL, &nDefaultIconkey, &dw)==ERROR_SUCCESS)
		{
			CString strDir = GetFixedDirectory() + _T("\\ARcBridge.exe");
			strDir += strIconKey;
			LPCTSTR lpszValue = (LPCTSTR)strDir;
			RegSetValueEx(nDefaultIconkey, NULL, 0, REG_SZ, (LPBYTE)lpszValue, (lstrlen(lpszValue)+1)*sizeof(TCHAR));
		}
		if(nDefaultIconkey!=NULL) RegCloseKey(nDefaultIconkey);
		
		nDefaultIconkey = NULL;
		if(bOpen)
		{			
			if(strValue[3].GetLength()>0)
			{
				if(RegCreateKeyEx(nExtKey, _T("shell"), 0, REG_NONE, REG_OPTION_NON_VOLATILE,
					KEY_WRITE|KEY_READ, NULL, &nShellKey, &dw)==ERROR_SUCCESS)
				{
					if(RegCreateKeyEx(nShellKey, _T("open"), 0, REG_NONE, REG_OPTION_NON_VOLATILE,
						KEY_WRITE|KEY_READ, NULL, &nOpenKey, &dw)==ERROR_SUCCESS)
					{
						if(RegCreateKeyEx(nOpenKey, _T("command"), 0, REG_NONE, REG_OPTION_NON_VOLATILE,
							KEY_WRITE|KEY_READ, NULL, &nCommandKey, &dw)==ERROR_SUCCESS)
						{
							CString strPath(strValue[3]), strFomat;
							strFomat.Format(_T(".EXE\" %c%%1%c"), '"', '"');
							strPath = _T("\"") + strPath + (CString)_T("\\") + pApp->m_pszAppName + strFomat;
							LPCTSTR lpszValue = (LPCTSTR)strPath;
							RegSetValueEx(nCommandKey, NULL, 0, REG_SZ, (LPBYTE)lpszValue,
							  (lstrlen(lpszValue)+1)*sizeof(TCHAR));
						}
					}

					if(RegCreateKeyEx(nShellKey, _T("print"), 0, REG_NONE, REG_OPTION_NON_VOLATILE,
						KEY_WRITE|KEY_READ, NULL, &nPrintKey, &dw)==ERROR_SUCCESS)
					{
						if(RegCreateKeyEx(nPrintKey, _T("command"), 0, REG_NONE, REG_OPTION_NON_VOLATILE,
							KEY_WRITE|KEY_READ, NULL, &nCommandKey, &dw)==ERROR_SUCCESS)
						{
							CString strPath(strValue[3]), strFomat;
							strFomat.Format(_T(".EXE /P %c%%1%c"), '"', '"');
							strPath += (CString)_T("\\") + pApp->m_pszAppName + strFomat;
							LPCTSTR lpszValue = (LPCTSTR)strPath;
							RegSetValueEx(nCommandKey, NULL, 0, REG_SZ, (LPBYTE)lpszValue,
							  (lstrlen(lpszValue)+1)*sizeof(TCHAR));
						}
					}
				}
			}
		}
	}

	if (nExtKey!=NULL) RegCloseKey(nExtKey);
	if (nDefaultIconkey!=NULL) RegCloseKey(nDefaultIconkey);
	if (nShellKey!=NULL) RegCloseKey(nShellKey);
	if (nOpenKey!=NULL) RegCloseKey(nOpenKey);
	if (nCommandKey!=NULL) RegCloseKey(nCommandKey);
	if (nPrintKey!=NULL) RegCloseKey(nPrintKey);
}

BOOL CARcBridgeApp::PreTranslateMessage(MSG* pMsg) 
{
	// CG: The following line was added by the Splash Screen component.
	if (m_pMainWnd == NULL)
	{
		return FALSE;
	}

	if(CSplashWnd::PreTranslateAppMessage(pMsg))
		return TRUE;

	return CWinApp::PreTranslateMessage(pMsg);
}

void CARcBridgeApp::DeleteWMF()
{
	CString strPath = AfxGetApp()->GetProfileString("Afx Directory", "Fixed Directory");
	SetCurrentDirectory(strPath);

	WIN32_FIND_DATA wfd;
	HANDLE hFind;
	BOOL bResult = TRUE;
	strPath += "\\*.wmf";
	
	hFind = FindFirstFile(strPath, &wfd);
	while(bResult)
	{
		BOOL bDelete = DeleteFile(wfd.cFileName);		
		bResult = FindNextFile(hFind,&wfd);
	}
	FindClose(hFind);
}

CString CARcBridgeApp::GetARcBridgeName()
{
	CString szProgramName = _T("ARcBridge");
//	szProgramName.Format(_T("ARcBridge %s"), GetStringLockInfo());
	return szProgramName;
}

CString CARcBridgeApp::GetAUpdateProgramInstallPath()
{
	CHAR pszPath[200]="";
	HKEY  hKey; 					
	CString szKey = "Software\\HangilIT\\AUpdate\\Afx Directory";	
	if (RegOpenKeyEx ( HKEY_CURRENT_USER, szKey, 0, KEY_ALL_ACCESS,&hKey ) == ERROR_SUCCESS)
	{		
		DWORD   dwDataSize = 500, dwDataType = REG_SZ;		
		CString szReadKey;						
		RegQueryValueEx(hKey,                 
						"Fixed Directory",                 
						0,                 
						&dwDataType,                 
						(unsigned char *)pszPath,                 
						&dwDataSize                 
						);              
		RegCloseKey (hKey); 		
	}
	else			
		return "";		
	return pszPath;

}

//#27449
//일반,Plus,문형
CString CARcBridgeApp::GetStringLockInfo()
{
	CString sLockInfo = _T("");

	CFrameWnd *pFrame = (CFrameWnd *)m_pMainWnd;
	if(pFrame == NULL) return sLockInfo;

	CARcBridgeDoc *pDoc	= (CARcBridgeDoc *)pFrame->GetActiveDocument();
	if(pDoc == NULL) return sLockInfo;

	CARcBridgeDataStd *pData = pDoc->m_pARcBridgeDataStd;

	switch (aGetLockID())
	{
 	case SPLOCK_MASTER_LID: 
		sLockInfo = _T("Master"); 
		break;
 	case SPLOCK_ARCBRIDGE_LID:
		if(pData->IsLockRcBridgePlus())
		{
			sLockInfo = _T("Plus");
		}
		else if(pData->IsLockRcBridgeBasic())
		{
			sLockInfo = _T("Basic");
		}
		else if(pData->IsLockRcBridgePortal())
		{
			sLockInfo = _T("문형");
		}
		if(pData->IsLockRcBridgeBoxModule())
		{
			if(pData->IsLockRcBridgeBox())
				sLockInfo = _T("Box교"); // 박스교만
			else
				sLockInfo += _T(",Box교"); // 박스모듈
		}

		break;
	}
	
	return sLockInfo;
}


CString CARcBridgeApp::GetStringEtcInfo()
{
	CString sEtcInfo = _T("");

	return sEtcInfo;
}

int CARcBridgeApp::ExitInstance()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	aLockExit();	
	LDK::aLockLogout();

	CHgModelRenderApi::UnloadFont();
	CStationFormatTextSingleTon::RemoveInstance();

	return CWinApp::ExitInstance();
}

CARcBridgeApp::~CARcBridgeApp()
{
	if(m_pszAppName != NULL)
		delete m_pszAppName;
}
