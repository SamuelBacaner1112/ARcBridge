// OutDesignDlg.cpp : implementation file
//

#include "stdafx.h"
#include "../../Src/ARcBridgeData/ARcBridgeData.h"
#include "../../Src/ARcBridgeCalculate/ARcBridgeCalculate.h"
#include "ARcbridgeOutDesign.h"
#include "../../../../Engine/BaseEng/include/attcmn.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COutDesignDlg dialog
// INSERT_ATESTCODE
HGLONG32 CStaticATest::m_bThoughtATest=FALSE;


COutDesignDlg::COutDesignDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COutDesignDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COutDesignDlg)
	m_nPrintType = -1;
	//}}AFX_DATA_INIT

	m_bOK = FALSE;
	m_pXL = NULL;
	m_bEarthQuake = FALSE;
	m_pStd = NULL;
	m_nPos = -1;
}


void COutDesignDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COutDesignDlg)
//	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDC_BUTTON_XLOPTION, m_btnOption);
	DDX_Control(pDX, IDC_BUTTON_VIEWWMF, m_btnViewWmf);
	DDX_Control(pDX, IDC_BUTTON_PRINTOUT, m_btnPrint);
	DDX_Control(pDX, IDC_BUTTON_OPEN, m_btnOpen);
	DDX_Control(pDX, IDC_TAB_DESIGN, m_ctlTab);
	DDX_Control(pDX, IDC_COMBO_ONE, m_ComboList);
	DDX_Control(pDX, IDC_TEXT_PROGRESS, m_sProgText);
	DDX_Control(pDX, IDC_COMBO_SELECT2, m_ComboSelect);
	DDX_Radio(pDX, IDC_RADIO_ONE, m_nPrintType);
	DDX_Control(pDX, IDC_PROGRESS_DESIGN, m_ctlProgress);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COutDesignDlg, CDialog)
	//{{AFX_MSG_MAP(COutDesignDlg)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_DESIGN, OnSelchangeTabType)
	ON_BN_CLICKED(IDC_BUTTON_PRINTOUT, OnPrint)
	ON_BN_CLICKED(IDC_RADIO_ALLTYPE, OnRadioAll)
	ON_BN_CLICKED(IDC_RADIO_ONE, OnRadioOne)
	ON_BN_CLICKED(IDC_RADIO_SELECT, OnRadioSelect)
	ON_BN_CLICKED(IDC_BUTTON_OPEN, OnButtonOpen)
	ON_BN_CLICKED(IDC_BUTTON_VIEWWMF, OnButtonViewwmf)
	ON_BN_CLICKED(IDC_BUTTON_XLOPTION, OnButtonOption)
	ON_WM_PAINT()
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_XLOUT_COMPLETE, OnXlOutComplete)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COutDesignDlg message handlers

void COutDesignDlg::OnSelchangeTabType(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	InitComboARcBridge();

	*pResult = 0;
}

void COutDesignDlg::OnPrint() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if(m_nPrintType==0 && m_ComboList.GetCurSel() < 0)
	{
		AfxMessageBox("����Ÿ�� �������� �ʽ��ϴ� !", MB_ICONSTOP);
		return;
	}		
	Begin();	
}

UINT COutDesignDlg::DoWork()
{
//	������ �ʴ� ���� ���μ��� ���̱�
	CString strName(_T(""));
	HWND hWnd = ::GetDesktopWindow();
	CWnd *pWnd = FromHandle(hWnd);
	while(1)
	{								
		if(hWnd)
		{			
			pWnd = FromHandle(hWnd);
			pWnd->GetWindowText(strName);
			if(strName.Find("Microsoft Excel")!=-1)
			{				
				if(!pWnd->IsWindowVisible())
				{
					DWORD pid;
					GetWindowThreadProcessId( pWnd->m_hWnd, &pid );
					HANDLE     hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
					TerminateProcess(hProcess, 0);
				}
			}
		}
		else 
		{
			break;
		}
		if(hWnd == ::GetDesktopWindow())
			hWnd = ::GetWindow(hWnd,GW_CHILD);
		else
			hWnd = ::GetWindow(hWnd,GW_HWNDNEXT);
		pWnd = FromHandle(hWnd);
	}

	CString str = _T("");
	
	m_bOK = FALSE;	

	GetDlgItem(IDC_BUTTON_PRINTOUT)->EnableWindow(FALSE);
	GetDlgItem(IDOK)->EnableWindow(FALSE);

	CARcBridgeDataStd *pData = m_pStd->m_pARcBridgeDataStd;
	CString sFileName = _T("");
	long nTotalBridge = m_ComboList.GetCount();

	CString sFileForm = pData->GetStringExcelFileForm();
	CString sExt = (sFileForm.Find(_T("xlsx")) != -1)? _T(".xlsx") : _T(".xls");

	if(m_nPrintType==0)
	{					
		long nBri = m_ComboList.GetCurSel();
		CRcBridgeApp  *pBri  = pData->GetBridge(nBri);

		if(LoadOutput(pBri))
		{
			CString sFileName = _T("");
			CString sHeader	= pData->GetFileNameForExcel(nBri);
			CString sFileNameSub = _T("");

			if(m_ctlTab.GetCurSel() == 0) sFileNameSub = _T("_��꼭");
			else if(m_ctlTab.GetCurSel() == 1) sFileNameSub = _T("_�������");
			else if(m_ctlTab.GetCurSel() == 2) sFileNameSub = _T("_���丮��Ʈ");

			sFileName.Format("%s%s", sHeader, sFileNameSub);
			
			// ���� ����
			CFileDialog dlg(FALSE, "*.xls", sFileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, sFileForm);
			dlg.m_ofn.lpstrTitle = "���� ��������";
			
			if(IDOK == dlg.DoModal())
			{				
				// INSERT_ATESTCODE	
				CAttSendMsg attObj(NULL,WM_ATT_MAINMSG,eMsgAppStartCalcDocumentAtEng,0,
					eMsgAppFinishCalcDocumentAtEng,0);

				CXLControl::GetWmfViewer().ClearAll();			
				m_szLastFileName = dlg.GetPathName();

				GetDlgItem(IDC_PROGRESS_DESIGN)->ShowWindow(TRUE);
				GetDlgItem(IDC_TEXT_PROGRESS)->ShowWindow(TRUE);

				MakeExcel(nBri);				
			}		
		}
		else
		{
			BOOL bMidasOutput = (pBri->m_nSelect2DFrameTool==SOLVE_MIDAS) ? TRUE : FALSE;
			CString str = _T("");
			str.Format("%s�� ���� 2DFrame �ؼ��� �Ǿ����� �ʽ��ϴ�.", pBri->m_strBridgeName);
			if(bMidasOutput) AfxMessageBox(str);
		}
	}
	if(m_nPrintType==1)
	{				
		long nCheckCount = 0;
		long nFirNum = -1;
		long i=0; for(i=0; i<nTotalBridge; i++)
		{
			if(m_ComboSelect.GetCheck(i)==TRUE) 
			{
				nCheckCount++;
				if(nCheckCount==1) nFirNum = i;
			}
		}

		if(nCheckCount<0 || nFirNum==-1) return 0;

		CRcBridgeApp  *pBri  = pData->GetBridge(nFirNum);
		CString sFileName = _T("");
		CString sHeader	= pData->GetFileNameForExcel(nFirNum);
		CString sFileNameSub = _T("");
		
		if(m_ctlTab.GetCurSel() == 0) sFileNameSub = _T("_��꼭");
		else if(m_ctlTab.GetCurSel() == 1) sFileNameSub = _T("_�������");
		else if(m_ctlTab.GetCurSel() == 2) sFileNameSub = _T("_���丮��Ʈ");
		sFileName.Format("%s%s", sHeader, sFileNameSub);

		CFileDialog dlg(FALSE, "*.xls", sFileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, sFileForm);
		dlg.m_ofn.lpstrTitle = "���� ��������";
	
		if(IDOK == dlg.DoModal())
		{
			// INSERT_ATESTCODE	
			CAttSendMsg attObj(NULL,WM_ATT_MAINMSG,eMsgAppStartCalcDocumentAtEng,0,
				eMsgAppFinishCalcDocumentAtEng,0);

			CString sFileName = dlg.GetPathName();

			GetDlgItem(IDC_PROGRESS_DESIGN)->ShowWindow(TRUE);
			GetDlgItem(IDC_TEXT_PROGRESS)->ShowWindow(TRUE);

			BOOL bFirst = TRUE;
			for(long nBri=0; nBri<nTotalBridge; nBri++)
			{
				if(m_ComboSelect.GetCheck(nBri)==FALSE) continue;

				CXLControl::GetWmfViewer().ClearAll();

				CRcBridgeApp  *pBri  = pData->GetBridge(nBri);				
				if(LoadOutput(pBri))
				{
					CString sFileName = _T("");
					
					if(bFirst)
					{
						m_szLastFileName = dlg.GetPathName();
						bFirst = FALSE;
					}
					else
					{
						CString sFilePath = dlg.GetPathName();
						if (!sFilePath.IsEmpty())
						{
							int index = sFilePath.ReverseFind(_T('\\'));
							if (index != -1)
							{
								sFilePath = sFilePath.Left(index);
							}
						}

						CString sHeader	= pData->GetFileNameForExcel(nBri);

						sFileName.Format("%s\\%s%s%s", sFilePath, sHeader, sFileNameSub, sExt);
						m_szLastFileName = sFileName;
					}
					
					MakeExcel(nBri);
				}
			}
		}		
	}
	if(m_nPrintType==2)
	{		
		CRcBridgeApp  *pBri  = pData->GetBridge(0);
		CString sFileName = _T("");
		CString sHeader	= pData->GetFileNameForExcel(0);
		CString sFileNameSub = _T("");
		
		if(m_ctlTab.GetCurSel() == 0) sFileNameSub = _T("_��꼭");
		else if(m_ctlTab.GetCurSel() == 1) sFileNameSub = _T("_�������");
		else if(m_ctlTab.GetCurSel() == 2) sFileNameSub = _T("_���丮��Ʈ");
		sFileName.Format("%s%s", sHeader, sFileNameSub);

		CFileDialog dlg(FALSE, "*.xls", sFileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, sFileForm);
		dlg.m_ofn.lpstrTitle = "���� ��������";

		if(IDOK == dlg.DoModal())
		{
			// INSERT_ATESTCODE	
			CAttSendMsg attObj(NULL,WM_ATT_MAINMSG,eMsgAppStartCalcDocumentAtEng,0,
				eMsgAppFinishCalcDocumentAtEng,0);

			CString sFileName = dlg.GetPathName();

			GetDlgItem(IDC_PROGRESS_DESIGN)->ShowWindow(TRUE);
			GetDlgItem(IDC_TEXT_PROGRESS)->ShowWindow(TRUE);

			BOOL bFirst = TRUE;
			for(long nBri=0; nBri<nTotalBridge; nBri++)
			{
				CRcBridgeApp  *pBri  = pData->GetBridge(nBri);
				if(m_ctlTab.GetCurSel() == 1 && pBri->m_nSeparBri == 2) continue;
				CXLControl::GetWmfViewer().ClearAll();

				if(LoadOutput(pBri))
				{
					CString sFileName = _T("");
					if(bFirst)
					{
						m_szLastFileName = dlg.GetPathName();
						
						bFirst = FALSE;
					}
					else
					{
// 						CString sFilePath = dlg.GetPathName();
// 						if (!sFilePath.IsEmpty())
// 						{
// 							int index = sFilePath.ReverseFind(_T('\\'));
// 							if (index != -1)
// 							{
// 								sFilePath = sFilePath.Left(index);
// 							}
// 						}
// 						sHeader	= pData->GetFileNameForExcel(nBri);
// 						sFileName.Format("%s\\%s%s.xls", sFilePath, sHeader, sFileNameSub);
						if(pData->m_nTypeFileNameForExcel == 1 || pData->m_nTypeFileNameForExcel == 4)
 							sHeader	= dlg.GetFolderPath() + _T("\\") + pData->GetFileNameForExcel(nBri);
						else
							sHeader	= pData->GetFileNameForExcel(nBri);

 						sFileName.Format("%s%s%s", sHeader, sFileNameSub, sExt);
						m_szLastFileName = sFileName;
					}
					
					MakeExcel(nBri);
				}
			}
		}		
	}		
	GetDlgItem(IDC_PROGRESS_DESIGN)->ShowWindow(FALSE);
	GetDlgItem(IDC_TEXT_PROGRESS)->ShowWindow(FALSE);	
	PostMessage(WM_XLOUT_COMPLETE,0,0);
	return 0;
}

BOOL COutDesignDlg::MakeExcel(long nBri)
{
	long nTab = m_ctlTab.GetCurSel();
	m_pXL = new CXLControl;	

	CARcBridgeDataStd *pDataStd	= m_pStd->m_pARcBridgeDataStd;
	
	long TextSize  = (long)pDataStd->m_dFontSizePrint;
	CString szFont = pDataStd->m_szFontPrint;
	if(pDataStd->m_bExcelDefaultSetting)
	{
		TextSize = 11;
		szFont = "����";
	}


#ifdef _DEBUG
	m_pXL->SetXLVisible(TRUE);
#endif
	if( m_pXL->NewXL() == FALSE ) 
	{
		PostMessage(WM_XLOUT_COMPLETE,0,0);		
		CheckStandardFont(m_pXL, FALSE, szFont, TextSize, FALSE);
		delete m_pXL;
		m_pXL=NULL;
		return 0;
	}					
	if(!CheckStandardFont(m_pXL, TRUE, szFont, TextSize, FALSE)) return 0;
	m_ctlProgress.SetPos(0);
	CString sFileName = m_szLastFileName;
#ifndef _DEBUG
	try 
#endif
	{
		if(nTab == 0) m_pStd->m_pOutBridge->MakeExcel(m_pXL,nBri);
		else if(nTab == 1) m_pStd->m_pOutResister->MakeExcel(m_pXL,nBri);
	}
#ifndef _DEBUG
	catch(...)
	{
		AfxMessageBox("���� ����� ���� �߻� !\n�ѱ����Ƽ�� ���� �ٶ��ϴ�.",MB_OK | MB_ICONSTOP);
		CheckStandardFont(m_pXL, FALSE, szFont, TextSize, FALSE);
		m_pXL->QuitXL();
		delete m_pXL;
		m_pXL = NULL;
		return 1;
	}
#endif

	m_ctlProgress.SetPos(m_nPos);
	
	try 
	{
		m_pXL->SaveAs(sFileName);
		m_pXL->GetExcelFileNameForAnyVersion(m_szLastFileName);
	}
	catch(CException* theException)
	{
		TCHAR   szCause[255];
		CString strFormatted = "";
		theException->GetErrorMessage(szCause,255);
		
		strFormatted.Format("%s",szCause);
		strFormatted += "\n\n ���� ������ ������ �� ����Ͻʽÿ�.";
		long nResult = AfxMessageBox(strFormatted,MB_OK | MB_ICONSTOP);			
		
		if(nResult == IDOK)
		{
			m_ctlProgress.SetPos(0);
			
			theException->Delete();
			CheckStandardFont(m_pXL, FALSE, szFont, TextSize, FALSE);
			return 1;
		}
		CheckStandardFont(m_pXL, FALSE, szFont, TextSize, FALSE);
		m_pXL->QuitXL();
		delete m_pXL;
		m_pXL = NULL;
		return 0;
	}	
	CheckStandardFont(m_pXL, FALSE, szFont, TextSize, FALSE);
	m_pXL->QuitXL();
	delete m_pXL;
	m_pXL = NULL;
	m_bOK = TRUE;

	GetDlgItem(IDC_PROGRESS_DESIGN)->SetWindowText("");
	GetDlgItem(IDC_TEXT_PROGRESS)->SetWindowText("");

	return 1;
}

void COutDesignDlg::OnRadioAll() 
{
	// TODO: Add your control notification handler code here
	UpdateData();	
	GetDlgItem(IDC_COMBO_ONE)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBO_SELECT2)->EnableWindow(FALSE);
}

void COutDesignDlg::OnRadioOne() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	GetDlgItem(IDC_COMBO_ONE)->EnableWindow(TRUE);
	GetDlgItem(IDC_COMBO_SELECT2)->EnableWindow(FALSE);
}

void COutDesignDlg::OnRadioSelect() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	GetDlgItem(IDC_COMBO_ONE)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBO_SELECT2)->EnableWindow(TRUE);
}

void COutDesignDlg::OnButtonOpen() 
{
	// TODO: Add your control notification handler code here
	ShellExecute(NULL,"open", m_szLastFileName ,NULL,NULL,SW_SHOW);	
}

void COutDesignDlg::OnButtonViewwmf() 
{
	// TODO: Add your control notification handler code here
	CXLControl::GetWmfViewer().CreateOrShowWindow(this,TRUE);	
}

void COutDesignDlg::OnButtonOption() 
{
	// TODO: Add your control notification handler code here
	int nTab   = m_ctlTab.GetCurSel();
	if(nTab==0)
	{
		m_pStd->m_pOutDesignOption->m_pXLOption  = m_pStd->m_pXLOption;
		m_pStd->m_pOutDesignOption->m_pEtcOption = m_pStd->m_pEtcOption;
		m_pStd->m_pOutDesignOption->m_pStmOption = m_pStd->m_pStmOption;
	}
	else if(nTab==1 || nTab==2)
	{
		m_pStd->m_pOutDesignOption->m_pXLOption  = m_pStd->m_pXLOption;
		m_pStd->m_pOutDesignOption->m_pEtcOption = m_pStd->m_pEtcOption;
	}
//////////////////////////////////////////////////////////////////////////
	CARcBridgeDataStd *pStd = m_pStd->m_pARcBridgeDataStd;

	if(m_pStd->m_pARcBridgeDataStd->m_bExcelDefaultSetting)
	{
		pStd->m_szFontPrint		    = "����ü";
		pStd->m_dFontSizePrint		= 9;
		pStd->m_dOffsetLeftPrint	= 1.9;
		pStd->m_dOffsetRightPrint	= 1.9;
		pStd->m_dOffsetTopPrint		= 2.5;
		pStd->m_dOffsetBottomPrint  = 2.5;
	}
	m_pStd->m_pOutDesignOption->m_XLOptionPage.m_bLsd = pStd->IsLsdDesign();
	m_pStd->m_pOutDesignOption->m_XLExelOptionPage.m_bUseDefault = pStd->m_bExcelDefaultSetting;
	m_pStd->m_pOutDesignOption->m_XLExelOptionPage.m_szFont      = pStd->m_szFontPrint;
	m_pStd->m_pOutDesignOption->m_XLExelOptionPage.m_nSizeFont   = (int)pStd->m_dFontSizePrint;
	m_pStd->m_pOutDesignOption->m_XLExelOptionPage.m_szLeft.Format("%.1f", pStd->m_dOffsetLeftPrint);
	m_pStd->m_pOutDesignOption->m_XLExelOptionPage.m_szRight.Format("%.1f", pStd->m_dOffsetRightPrint);
	m_pStd->m_pOutDesignOption->m_XLExelOptionPage.m_szTop.Format("%.1f", pStd->m_dOffsetTopPrint);
	m_pStd->m_pOutDesignOption->m_XLExelOptionPage.m_szBottom.Format("%.1f", pStd->m_dOffsetBottomPrint);
	m_pStd->m_pOutDesignOption->m_XLExelOptionPage.m_szPageRow.Format("%.0f", (double)pStd->m_nPageRow);

	if(m_pStd->m_pOutDesignOption->DoModal()==IDOK)
	{
		pStd->m_bExcelDefaultSetting = m_pStd->m_pOutDesignOption->m_XLExelOptionPage.m_bUseDefault;
		pStd->m_szFontPrint = m_pStd->m_pOutDesignOption->m_XLExelOptionPage.m_szFont;
		pStd->m_dFontSizePrint = m_pStd->m_pOutDesignOption->m_XLExelOptionPage.m_nSizeFont;
		pStd->m_dOffsetLeftPrint = atof(m_pStd->m_pOutDesignOption->m_XLExelOptionPage.m_szLeft);
		pStd->m_dOffsetRightPrint = atof(m_pStd->m_pOutDesignOption->m_XLExelOptionPage.m_szRight);
		pStd->m_dOffsetTopPrint = atof(m_pStd->m_pOutDesignOption->m_XLExelOptionPage.m_szTop);
		pStd->m_dOffsetBottomPrint = atof(m_pStd->m_pOutDesignOption->m_XLExelOptionPage.m_szBottom);
		pStd->m_nPageRow = (long)atof(m_pStd->m_pOutDesignOption->m_XLExelOptionPage.m_szPageRow);
	}
//////////////////////////////////////////////////////////////////////////
}

void COutDesignDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CDialog::OnPaint() for painting messages
}

void COutDesignDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	CDialog::OnShowWindow(bShow, nStatus);	
	
	if(bShow)
	{
		m_ComboSelect.SetInitListHandle();		
		switch(m_nPrintType)
		{		
		case 0: OnRadioOne();		break;			
		case 1: OnRadioSelect();	break;			
		case 2:	OnRadioAll();		break;
		}		
	}
}

void COutDesignDlg::InitComboARcBridge()
{
	m_ComboSelect.InitComboBox();
	m_ComboSelect.ResetContent();
	m_ComboList.ResetContent();
	int nTab = m_ctlTab.GetCurSel();
	CARcBridgeDataStd *pStd = m_pStd->m_pARcBridgeDataStd;

	GetDlgItem(IDC_BUTTON_XLOPTION)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO_ONE)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO_SELECT)->EnableWindow(TRUE);

	CString str;
	
	long nSize;
	nSize = pStd->GetBridgeSize();
	
	long i=0; for(i=0; i<nSize; i++)
	{
		CRcBridgeApp* pBri = pStd->GetBridge(i);
		str = pStd->GetStringBridgeOrder(i) + _T(" : ") + pBri->m_strBridgeName
			+ _T(" [") + pBri->GetStringBridgeType() + _T("]");
		m_ComboList.AddString(str);
		m_ComboSelect.AddString(str);
	}
	if(nTab==1 || nTab==2)
	{
		m_nPrintType = 2;
		GetDlgItem(IDC_BUTTON_XLOPTION)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_ONE)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_SELECT)->EnableWindow(FALSE);
	}

	if(m_nPrintType!=0) GetDlgItem(IDC_COMBO_ONE)->EnableWindow(FALSE);
	if(m_nPrintType!=1) GetDlgItem(IDC_COMBO_SELECT2)->EnableWindow(FALSE);
	
	m_ComboList.SetCurSel(m_pStd->m_nCurBridge);
	m_ComboSelect.SetCheck(0, TRUE);
	UpdateData(FALSE);
}

BOOL COutDesignDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_nPrintType = 0;

	m_ctlTab.InsertItem(0,"��ü�ؼ�");
	// (ARCBRIDGE-2687) �Ѱ���� �϶��� ������� ����� �������� �ʴ´�.
	if(m_pStd->m_pARcBridgeDataStd->IsLsdDesign() == FALSE)
		m_ctlTab.InsertItem(1,"�������");
	// m_ctlTab.InsertItem(2,"���丮��Ʈ"); // (ARCBRIDGE-2687) ��� ���뵵 ���� �Ǹ� �ִ�. ����..

	m_ctlTab.SetCurSel(0);
	InitComboARcBridge();
	InitOption();
	OnRadioOne();
	
	InitOptionItemCorner();

	m_nPos	= 10000;
	m_ctlProgress.SetRange(0,(short)m_nPos);
	GetDlgItem(IDC_PROGRESS_DESIGN)->ShowWindow(FALSE);
	GetDlgItem(IDC_TEXT_PROGRESS)->ShowWindow(FALSE);

	CXLControl::GetWmfViewer().CreateOrShowWindow(this,FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COutDesignDlg::InitOption()
{
	// ��꼭 ��� �ɼ�
	CARcBridgeDataStd *pStd = m_pStd->m_pARcBridgeDataStd;
	CRcBridgeApp *pBridge = pStd->GetBridge(m_pStd->m_nCurBridge);
	if (!pBridge) return;

	COptionItem *pOpt = m_pStd->m_pXLOption;	
	if(pOpt->GetOptionSize()>0) return;

	const HGBOOL bLsd = pStd->IsLsdDesign();

	pOpt->RemoveAllData();
	
	pOpt->AddItem("ǥ��");
	pOpt->AddItem("�����帧��");
	pOpt->AddItem("����");
	pOpt->AddItem("�ؼ���� ���");
	pOpt->AddItem("��������");
	pOpt->AddItem("������");
	pOpt->AddItem("�ܸ鰡��");
	pOpt->AddItem("�𵨸�");
	pOpt->AddItem("���߻���");
	pOpt->AddItem("��������");
	pOpt->AddItem("��������");
	pOpt->AddItem("�ܸ�� ����");
	pOpt->AddItem("�ܸ����");
	pOpt->AddItem("��뼺����");
	pOpt->AddItem("ó�� �� �Ƿΰ���");
	pOpt->AddItem("�찢�� �� ���ö�� ����", bLsd ? FALSE : TRUE, bLsd ? FALSE : TRUE);
	pOpt->AddItem("��,���(�߰���ü) ����");
	pOpt->AddItem("��ġ���� ����");
	pOpt->AddItem("���ʾ�������");
	pOpt->AddItem("���ʼ���");
	pOpt->AddItem("����������");
	pOpt->AddItem("���ӽ����� ����");
	pOpt->AddItem("���Ұ���");
	pOpt->AddItem("�ε�ħ�ϰ���");
	pOpt->AddItem("������ħ �ݷ»���");
	pOpt->AddItem("�������� ����");
	pOpt->AddItem("ĵƿ���� ����", FALSE, pBridge->IsCantileverDesign());
	pOpt->AddItem("STM �ؼ�-�찢��", pBridge->IsApplyStmCorner());
	pOpt->AddItem("STM �ؼ�-�����", pBridge->IsApplyStmBracket());
	pOpt->AddItem("STM �ؼ�-����������", pBridge->IsApplyStmFooting());
	pOpt->AddItem("STM �ؼ�-�߰�����", pBridge->IsApplyStmMidFooting());

	// ��Ÿ ��� �ɼ�
	COptionItem *pOptEtc = m_pStd->m_pEtcOption;
	if(pOptEtc->GetOptionSize()>0) return;
	pOptEtc->RemoveAllData();
	pOptEtc->AddItem("�ܸ鰡�� �𵵿� �����ֻ� ����", FALSE);	
	pOptEtc->AddItem("�ִ���ȿ�� ���� ��½� �� ����", FALSE);	

	// Stm ��� �ɼ�

	COptionItem *pOptStm = m_pStd->m_pStmOption;
		if(pOptStm->GetOptionSize()>0)
			return;
		
	pOptStm->RemoveAllData();
	pOptStm->AddItem(hggettext("��������"));
	pOptStm->AddItem(hggettext("�ܸ�����"));
	pOptStm->AddItem(hggettext("�ؼ��𵨸�"));
	pOptStm->AddItem(hggettext("���߻���"));
	pOptStm->AddItem(hggettext("�������ϵ�"));
	pOptStm->AddItem(hggettext("��������"));
	pOptStm->AddItem(hggettext("��Ʈ��/Ÿ�� �ܸ��"));
	pOptStm->AddItem(hggettext("ö�ٹ�ġ"));
	pOptStm->AddItem(hggettext("��Ʈ�� ���������� �ִ���ȿ�� ����"));
	pOptStm->AddItem(hggettext("��Ʈ�� ��������"));
	pOptStm->AddItem(hggettext("���������� ��������"));
	pOptStm->AddItem(hggettext("�տ���� ���� �ּ�ö�ٷ��� ���"), FALSE);
	//pOptStm->AddItem(hggettext("ö���� ����"), FALSE); ARCBRIDGE-2779
}

LRESULT COutDesignDlg::OnXlOutComplete(WPARAM, LPARAM)
{
	GetDlgItem(IDC_BUTTON_PRINTOUT)->EnableWindow(TRUE);
	GetDlgItem(IDOK)->EnableWindow(TRUE);
	Sleep(200);
	if(m_bOK) 
	{
		AfxMessageBox("����� �Ϸ��߽��ϴ�.",MB_OK,MB_ICONINFORMATION);		
		GetDlgItem(IDC_BUTTON_OPEN)->EnableWindow(TRUE);
		CXLControl::RemoveTmpFiles();		
	}	
	else
	{
		GetDlgItem(IDC_BUTTON_OPEN)->EnableWindow(FALSE);
	}
	return 0;
}

BOOL COutDesignDlg::LoadOutput(CRcBridgeApp *pBri)
{
	BOOL bMidasOutput = (pBri->m_nSelect2DFrameTool==SOLVE_MIDAS) ? TRUE : FALSE;

	CString szOutExt  = bMidasOutput ? "*.anl|*.*" : "*.out|*.*";
	CString szComment = bMidasOutput ? "MIDAS Output File(*.anl)|*.anl|������� (*.*)|*.*||" : "Sap2000 Output File(*.out)|*.out|������� (*.*)|*.*||";

	BOOL bEarthQuakeDesign = pBri->GetTypeDesingEQ() != 0 ? TRUE : FALSE;
	BOOL bLoadedAll = TRUE;

	long nCountOutput = pBri->GetCountEarthQuakeModel() + 1;
	long i=0; for(i=0; i<nCountOutput; i++)
	{
		CFEMManage *pFEM = &pBri->m_FemNormal;
		if(i > 0)
		{
			if(i == 1 && pBri->IsBoxType() == FALSE) continue;
			long nOffset = pBri->IsBoxType() ? 1 : 2;
			pFEM = pBri->GetFemManage(COMBO_EARTHQUAKE + i - nOffset);
		}
		
		if(pFEM->IsOutputLoaded()==FALSE)
		{
			bLoadedAll = FALSE;
			break;
		}
	}

	if(bLoadedAll)	return TRUE; // Output ��� �ε�� ���
	if(bMidasOutput)	return FALSE;

	// INSERT_ATESTCODE
	if(CStaticATest::m_bThoughtATest)
		return FALSE;

	if(AfxMessageBox("�𵨸� �ؼ������ �����ϴ�.\n����,�������� ��� ��������� �����ðڽ��ϱ� ?",MB_YESNO)==IDNO)
		return FALSE;


	CString szPath = "";
	CFileDialog dlg(TRUE, szOutExt, "", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
								  szComment, m_pStd->m_pView);
	dlg.m_ofn.lpstrTitle = bMidasOutput ? "MIDAS ������� �ҷ�����" : "SAP ������� �ҷ�����";
	if (dlg.DoModal() == IDOK) 
		szPath = dlg.GetPathName();
	else 
		return FALSE;

	for(i=0; i<nCountOutput; i++)
	{
		if(pBri->m_nSelectModelingCountEQ!=2 && i==3) break;
		if(pBri->IsRcSlab() && i==1) break;
		if(pBri->IsBoxType() && i == 3) break;
		if(pBri->IsBoxType() && pBri->m_nSelectModelingCountEQ !=2 && i == 2) break;
		
		CString szOutput = szPath;
		if(pBri->IsBoxType() == FALSE)
		{
			if(i==1)
				szOutput.Replace("_����","_��������");
			else if(i==2 && pBri->m_nSelectModelingCountEQ!=2)
				szOutput.Replace("_����","_���ϸ��");
			else if(i==2 && pBri->m_nSelectModelingCountEQ==2)
				szOutput.Replace("_����","_���ϸ��_�������");
			else if(i==3)
				szOutput.Replace("_����","_���ϸ��_�������");
		}
		else
		{
			if(i == 1)
			{
				if(pBri->m_nSelectModelingCountEQ == 0)
					szOutput.Replace("_����","_�ر�����");
				else
					szOutput.Replace("_����","_��ɼ���");
			}
			else if(i == 2)
			{
				szOutput.Replace("_����","_��ɼ���");
			}
		}

		CStdioFile Fout;
		long nReturn = Fout.Open(szOutput,CFile::modeRead);
		Fout.Close();

		if(nReturn==0) return FALSE;
	}

	// ������� �б�.....
	for(i=0; i<nCountOutput; i++)
	{
		if(pBri->m_nSelectModelingCountEQ!=2 && i==3) break;
		if(pBri->IsRcSlab() && i==1) break;
		if(pBri->IsBoxType() && i == 3) break;
		if(pBri->IsBoxType() && pBri->m_nSelectModelingCountEQ !=2 && i == 2) break;

		CFEMManage *pFEM = &pBri->m_FemNormal;
		CString szOutput = szPath;
		if(pBri->IsBoxType() == FALSE)
		{
			if(i==1)
			{
				szOutput.Replace("_����","_��������");
				pFEM = &pBri->m_FemEQ[0];
			}
			else if(i==2 && pBri->m_nSelectModelingCountEQ!=2)
			{
				szOutput.Replace("_����","_���ϸ��");
				pFEM = &pBri->m_FemEQ[1];
			}
			else if(i==2 && pBri->m_nSelectModelingCountEQ==2)
			{
				szOutput.Replace("_����","_���ϸ��_�������");
				pFEM = &pBri->m_FemEQ[1];
			}
			else if(i==3)
			{
				szOutput.Replace("_����","_���ϸ��_�������");
				pFEM = &pBri->m_FemEQ[2];
			}
		}
		else
		{
			if(i==1)
			{
				if(pBri->m_nSelectModelingCountEQ == 0)
					szOutput.Replace("_����","_�ر�����");
				else
					szOutput.Replace("_����","_��ɼ���");
				pFEM = &pBri->m_FemEQ[0];
			}
			else if(i==2 && pBri->m_nSelectModelingCountEQ == 2)
			{
				szOutput.Replace("_����","_��ɼ���");
				pFEM = &pBri->m_FemEQ[1];
			}

		}

		if(pFEM == NULL || pFEM->IsOutputLoaded()) continue;

		CARcBridgeCalcStd CalcStd(dynamic_cast<CRcBridgeRebar*>(pBri));
		CalcStd.m_pFrame->MakeSapData(i, TRUE);

		CFEMLoadSAP2000Output LoadOut(pFEM);
		LoadOut.LoadData(szOutput);
	}

	return TRUE;
}

// bInit TRUE : sFont, dSize�� ���� ��Ʈ�� ���� �Ǿ� �ִ��� üũ�ؼ�
// ������ ������ ����ڿ��� ���´�.
// bInit FALSE : ������ ������ �Ǿ����ٸ� ������ ��Ʈ�� �����Ѵ�.
BOOL COutDesignDlg::CheckStandardFont(CXLControl *pXL, BOOL bInit, CString sFont, double dSize, BOOL bView)
{
	CHgBaseBMStd baseBM;
	return baseBM.CheckStandardFont(pXL, bInit, sFont, dSize, bView);
}

void COutDesignDlg::InitOptionItemCorner()
{
	CARcBridgeDataStd *pStd = m_pStd->m_pARcBridgeDataStd;
	if (pStd == NULL)
		return;

	const HGBOOL bLsd = pStd->IsLsdDesign();
	if (bLsd == FALSE)
		return;

	COptionItem *pOpt = m_pStd->m_pXLOption;
	if (pOpt == NULL)
		return;

	HGINT32 ixFind = -1;
	for (HGINT32 ix = 0; ix < pOpt->m_strArrItem.GetCount(); ++ix)
	{
		if (pOpt->m_strArrItem.GetAt(ix) == "�찢�� �� ���ö�� ����")
		{
			ixFind = ix;
		}
	}
	
	if (ixFind < 0)
		return;
	
	pOpt->SetCheck(ixFind, FALSE);
	pOpt->m_dArrEnable.GetAt(ixFind) = FALSE;
}
