#include "stdafx.h"

#include "CommandReDir.h"
#include "..\..\..\Engine\BaseEng\Src\AFem\ColorListBox.h"
#include "CommandResultDlg.h"
#include "..\..\..\Engine\BaseEng\include\attcmn.h"

//C:\Hangil\Trunk\Product\AAbutPierSTM\Src\AAbutPierInput
//C:\Hangil\Trunk\Engine\BaseEng\Src\AFem
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CCommandResultDlg::CCommandResultDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCommandResultDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCommandResultDlg)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT	
	m_bShowCompleteMsg = FALSE;
	m_bAutoTerminal	= FALSE;	// 자동으로 종료(닫기버튼 나오지 않음)
	m_szBtnText = hggettext("닫 기");
	m_pRedirect = new CCommandReDir(this);
}

CCommandResultDlg::~CCommandResultDlg()
{
	if (m_pRedirect)
		delete m_pRedirect;
}

void CCommandResultDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCommandResultDlg)
	DDX_Control(pDX, IDC_LIST_RESULT_COMMAN_RESULT, m_ctlList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCommandResultDlg, CDialog)
	//{{AFX_MSG_MAP(CCommandResultDlg)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_COPYDATA, OnCopyData)
	ON_MESSAGE(WM_STDIO_COMMAND, OnStdioCommand)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCommandResultDlg message handlers

void CCommandResultDlg::AddCommand(WPARAM wCommand, HGLPCTSTR lpszCommand)
{
	(CButton*) m_pButton = (CButton*)GetDlgItem(IDOK);
	if(wCommand==WM_CALC_END)
	{
		m_pButton->EnableWindow(TRUE);
		m_pButton->SetFocus();		
	}

	if(wCommand==STDIO_OPEN)
	{
		m_pButton->EnableWindow(FALSE);

		// INSERT_ATESTCODE
		CWnd* pMainWnd=AfxGetMainWnd();
		if(pMainWnd) pMainWnd->SendMessage(WM_ATT_MAINMSG,eMsgAppStartSap2000Dlg);
	}

	ASSERT(lpszCommand != NULL);	

	m_criticalSection.Lock();
	m_StdioCommand.AddTail(lpszCommand);	
	m_criticalSection.Unlock();		

	OnStdioCommand(wCommand, 0);
}

LRESULT CCommandResultDlg::OnStdioCommand(WPARAM wCommand, LPARAM lParam)
{
	CString strCommand;
	m_criticalSection.Lock();
	ASSERT(m_StdioCommand.GetCount() > 0);
	strCommand = m_StdioCommand.RemoveHead();    

	switch (wCommand)
	{
	case STDIO_OPEN:
		{
			if(!IsWindowVisible())
			{
				CenterWindow();
				ShowWindow(SW_HIDE);
			}

			CString strCmdLine;
			ASSERT(!strCommand.IsEmpty());
			GetCommandLine(AfxGetApp()->m_hInstance, strCmdLine, strCommand);
			if (m_pRedirect->StartChildProcess(strCmdLine, FALSE))
			{
				m_strCmdLine = strCmdLine;				
			}
			break;
		}
	case STDIO_CLOSE:
		{
			if (!strCommand.IsEmpty())
				m_pRedirect->WriteChildStdIn(strCommand);
			else
				m_pRedirect->TerminateChildProcess();			
			AddCommand(WM_CALC_END,_T(""));
		}			
		break;
	case STDIO_WRITE:
		m_pRedirect->WriteChildStdIn(strCommand);
		break;
	}
	m_criticalSection.Unlock();
	return 0;
}

LRESULT CCommandResultDlg::OnCopyData(WPARAM wParam, LPARAM lParam)
{
	COPYDATASTRUCT* pcds = (COPYDATASTRUCT*)lParam;
	CString szData = (HGLPCTSTR)pcds->lpData;	
	CStringArray strArr;
	SeperateStr(szData,strArr);

	switch (pcds->dwData)
	{		
	case SET_WINDOW_TEXT:
		{			
			if(szData==_T(">>End<<"))			
			{
				// INSERT_ATESTCODE
				CWnd* pMainWnd=AfxGetMainWnd();
				if(pMainWnd) pMainWnd->SendMessage(WM_ATT_MAINMSG,eMsgAppFinishSap2000Dlg, (LPARAM)m_hWnd);


				long nCount = m_ctlList.GetCount();	

				m_ctlList.InsertString (nCount,_T(""));
				m_ctlList.SetCurSel( nCount );
				m_ctlList.InsertString (nCount+1,_T("  A N A L Y S I S    C O M P L E T E   !"));
				m_ctlList.SetCurSel( nCount+1 );
				m_ctlList.InsertString (nCount+2,_T(""));
				m_ctlList.SetCurSel( nCount+2 );

				CString strCommandNext = m_StrCmdList.GetCount() > 0 ? m_StrCmdList.RemoveHead() : _T("");

				if(!strCommandNext.IsEmpty())
				{							
					AddCommand(STDIO_OPEN, strCommandNext);
				}
				else
				{
					CString strPrograme = m_nTypeRun == 0 ? _T(" pconvert Complete !") : _T(" tconvert Complete !");
					SetWindowText(m_strTitle + strPrograme);
					if(m_bAutoTerminal)
					{
						OnOK();
					}									
					if(m_bShowCompleteMsg)
					{
						AfxMessageBox(strPrograme, MB_ICONINFORMATION);										
					}
				}						
			}			
			break;
		}		
	case STDIO_WRITE_TEXT:
		{			
			long nCount = m_ctlList.GetCount();		
			m_ctlList.InsertString (nCount,szData);
			m_ctlList.SetCurSel( nCount );		
			break;
		}
	case STDERR_WRITE_TEXT:
		{
			for(long n=0; n<strArr.GetSize(); n++)
			{
				long nCount = m_ctlList.GetCount();
				if(strArr.GetAt(n) == _T("REPEAT"))
				{
					m_ctlList.DeleteString(nCount-1);
				}

				m_ctlList.InsertString (nCount,strArr.GetAt(n));
				m_ctlList.SetCurSel( nCount );								
			}
			break;
		}
	}
	return 0;
}

void CCommandResultDlg::OnOK() 
{	
	m_StrCmdList.RemoveAll();
	AddCommand(STDIO_CLOSE, hggettext("<프로그램 종료>"));

	UINT message = m_nTypeRun == 0 ? WM_CALC_STRESS_FLOW_OK : WM_CALC_STAD2D_OK;
	if(m_pMainWnd) 
		m_pMainWnd->PostMessage(message, 1,0);
	else
		GetParent()->PostMessage(message, 1,0);

	CDialog::OnOK();
}

BOOL CCommandResultDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();		

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CCommandResultDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);

	SetWindowText(m_strTitle.IsEmpty() ? hggettext("실행 결과창") : m_strTitle);
	GetDlgItem(IDOK)->SetWindowText(m_szBtnText);
}


void CCommandResultDlg::SetTitle(const CString &strTitle)
{
	m_strTitle = strTitle;
}
