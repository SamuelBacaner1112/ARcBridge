// ARcBridgeInputStd.cpp: implementation of the CARcBridgeInputStd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../ARcBridgeData/ARcBridgeData.h"
#include "../ARcBridgeCalculate/ARcBridgeCalculate.h"
#include "ARcBridgeInput.h"
#include "DialogHDanLength.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CARcBridgeInputStd::CARcBridgeInputStd()
{
	m_pDlgHDanLength	= new CDialogHDanLength;
	m_pDlgHDanLength->SetARcStructureInputStd(this);
	m_nFlag	= 0;
	m_strHelp = _T("");
	// �信�� ������ ���콺 �̺�Ʈ �������� ////////////
	m_strME.bViewMouseEvent = FALSE;
	m_strME.xy = CDPoint(0, 0);
	////////////////////////////////////////////////////

	m_bZoomAllOnInitDialog	= TRUE;
	m_pCurrentDialog	= NULL;
	m_nCurrentBridge	= 0;
	m_nCurrentSap		= 0;
	m_bViewAllDir	= TRUE;

	AddDialog(ID_INPUT_BASE1, &m_dlgProjectInfo, MENU_BASE);
	AddDialog(ID_INPUT_BASE2, &m_dlgDesignBasicdata, MENU_BASE);
	AddDialog(ID_INPUT_BASE3, &m_dlgLineInfo, MENU_BASE);
	AddDialog(ID_INPUT_BASE4, &m_dlgBridgeFormFactor, MENU_BASE);
	AddDialog(ID_INPUT_BASE5, &m_dlgGroundInfo, MENU_BASE);
	AddDialog(ID_INPUT_BASE6, &m_dlgCrossPosition, MENU_BASE);
	AddDialog(ID_INPUT_BASE7, &m_dlgSpanConstruct, MENU_BASE);
	AddDialog(ID_INPUT_BASE8, &m_dlgShoePosition, MENU_BASE);
	AddDialog(ID_INPUT_BASE9, &m_dlgCrossConstruct, MENU_BASE);
	AddDialog(ID_INPUT_GENERAL1, &m_dlgDesignVariable, MENU_GENERAL);
	AddDialog(ID_INPUT_GENERAL2, &m_dlgJongSection, MENU_GENERAL);
	AddDialog(ID_INPUT_GENERAL3, &m_dlgColumn, MENU_GENERAL);
	AddDialog(ID_INPUT_GENERAL7, &m_dlgPlane, MENU_GENERAL);
	AddDialog(ID_INPUT_GENERAL4, &m_dlgWingWall, MENU_GENERAL);
	AddDialog(ID_INPUT_GENERAL5, &m_dlgSodan, MENU_GENERAL);
	AddDialog(ID_INPUT_GENERAL6, &m_dlgTogong, MENU_GENERAL);
	AddDialog(ID_INPUT_GENERAL8, &m_dlgAps, MENU_GENERAL);
	AddDialog(ID_INPUT_GENERAL9, &m_dlgFooting, MENU_GENERAL);
/*	AddDialog(ID_INPUT_GENERAL10, &m_dlgHyunTa, MENU_GENERAL);*/
	AddDialog(ID_INPUT_GENERAL10, &m_dlgMonument, MENU_GENERAL);
	AddDialog(ID_INPUT_GENERAL14, &m_dlgLoad, MENU_GENERAL);
	AddDialog(ID_INPUT_GENERAL11, &m_dlgFrameAnalysis, MENU_GENERAL);
	AddDialog(ID_INPUT_GENERAL12, &m_dlgShellAnalysis, MENU_GENERAL);
	AddDialog(ID_INPUT_GENERAL13, &m_dlgWingShellAnalysis, MENU_GENERAL);
	AddDialog(ID_INPUT_REBAR1, &m_dlgRebarMain, MENU_REBAR);
	AddDialog(ID_INPUT_REBAR2, &m_dlgRebarMain2, MENU_REBAR);
	AddDialog(ID_INPUT_REBAR3, &m_dlgRebarMain3, MENU_REBAR);
	AddDialog(ID_INPUT_REBAR4, &m_dlgRebarSection, MENU_REBAR);
	AddDialog(ID_INPUT_REBAR10, &m_dlgRebarGagak, MENU_REBAR);
	AddDialog(ID_INPUT_REBAR5, &m_dlgRebarColumn, MENU_REBAR);
	AddDialog(ID_INPUT_REBAR6, &m_dlgRebarWingWall, MENU_REBAR);
	AddDialog(ID_INPUT_REBAR7, &m_dlgRebarAps, MENU_REBAR);
	AddDialog(ID_INPUT_REBAR8, &m_dlgRebarOthers, MENU_REBAR);
	AddDialog(ID_INPUT_REBAR9, &m_dlgRebarJewon, MENU_REBAR);
	// STM �ؼ�
	AddDialog(ID_INPUT_STM1, &m_dlgStmCondition, MENU_STM);
	AddDialog(ID_INPUT_STM2, &m_dlgStmArcCorner, MENU_STM);
	AddDialog(ID_INPUT_STM3, &m_dlgStmBracket, MENU_STM);
	AddDialog(ID_INPUT_STM4, &m_dlgStmFootingStartEnd, MENU_STM);
	AddDialog(ID_INPUT_STM5, &m_dlgStmFooringMid, MENU_STM);

	m_menuInput.LoadMenu(IDR_MENU_INPUT);

	m_bFlagScaleInputDialog	= TRUE;
	m_nLastTab_RebarMain	= 0;

	m_bShowComment = TRUE;
	m_bViewVectorInfo	= FALSE;
	m_nLeft				= 0;
	m_bPushMoveButton	= FALSE;

	m_pDlgAddRateSheet	= new CDlgAddRateSheet("ö�� ������ ����");
}

CARcBridgeInputStd::~CARcBridgeInputStd()
{
	delete m_pDlgHDanLength;
	delete m_pDlgAddRateSheet;
}	

void CARcBridgeInputStd::Serialize(CArchive& ar)
{
	// 3 : ���ҽ� ID ���� ���� 
	// 4 : �������� ���̾�α� �߰�
	m_nFlag = 4;
	int nMenuIDSize = 0;

	if(m_pCurrentDialog)
	{
		m_nSaveMenuID = m_pCurrentDialog->GetIDD();
	}

	if (ar.IsStoring())
	{
		ar << m_nFlag;
		ar << m_nSaveMenuID;
		nMenuIDSize = m_DialogList.GetSize();
		ar << nMenuIDSize;
		int i=0; for(i=0; i<nMenuIDSize; i++)
		{
			ar << m_DialogList[i]->GetIDD();
			ar << m_DialogList[i]->IsUsedFlag();
//			ar << m_DialogList[i]->IsErrorInclude();
		}
		ar << m_bShowComment;
		ar << m_nCurrentBridge;
		ar << m_bViewAllDir;
	}
	else
	{
		ar >> m_nFlag;
		ar >> m_nSaveMenuID;
		if(m_nFlag < 3)
			m_nSaveMenuID	= GetIDDInputDialogByFlag1(m_nSaveMenuID);
		ar >> nMenuIDSize;
		UINT nIDD;
		BOOL bUsedFlag;
		int i=0; for(i=0; i<nMenuIDSize; i++)
		{
			ar >> nIDD;
			if(m_nFlag < 3)
				nIDD	= GetIDDInputDialogByFlag1(nIDD);
			ar >> bUsedFlag;
			if(GetDialogByIDD(nIDD))
				GetDialogByIDD(nIDD)->SetUsedFlag(bUsedFlag);
//			GetDialogByIDD(nIDD)->SetErrorInclude(bValue);
		}
		ar >> m_bShowComment;
		if(m_nFlag > 0)
			ar >> m_nCurrentBridge;
		if(m_nFlag >= 2)
			ar >> m_bViewAllDir;

		AfxGetMainWnd()->SendMessage(WM_USER+8, -1);
	}

	if(ar.IsLoading())
	{
		if(m_nFlag < 3)
		{
			for(long bri = 0; bri < m_pARcBridgeDataStd->GetBridgeSize(); bri++)
			{
				CRcBridgeRebar *pBri	= m_pARcBridgeDataStd->GetBridgeRebar(bri);
				if(!pBri) continue;
				long i = 0; for(i = 0; i < pBri->m_pArrTypeDefault.GetSize(); i++)
				{
					CTypeDefault *pDefault	= pBri->m_pArrTypeDefault.GetAt(i);
					pDefault->m_nIDDialog	= GetIDDInputDialogByFlag1(pDefault->m_nIDDialog);
				}
			}
		}
		else if (m_nFlag < 4)
		{
			if (GetDialogByIDD(IDD_GEN_LOAD) && GetDialogByIDD(IDD_GEN_FRAME_ANLYSIS) && GetDialogByIDD(IDD_GEN_FRAME_ANLYSIS)->IsUsedFlag())
			{
				GetDialogByIDD(IDD_GEN_LOAD)->SetUsedFlag(TRUE);
			}
		}
	}
}

void CARcBridgeInputStd::InsertInputMenu(CXTPMenuBar* pMenuBar)
{
	CXTPControls* pControls = pMenuBar->GetControls();

	if(pControls->GetCount()>5)
		return;

	pMenuBar->LoadMenu(&m_menuInput, FALSE);
	int i=0; for(i=0; i<MENU_COUNT; i++)
	{
		CXTPControl* pControl = pControls->GetAt(i+5);
		pControls->MoveBefore(pControl, i+1);
	}
}

void CARcBridgeInputStd::MoveNextDialog()
{
	if(m_pCurrentDialog)
	{
		m_bPushMoveButton	= TRUE;
		CInputDialogBase* pDlg = GetDialogByOrder(TRUE);
		ShowDialog(pDlg, TRUE);
		m_bPushMoveButton	= FALSE;
	}
}

void CARcBridgeInputStd::MovePrevDialog()
{
	if(m_pCurrentDialog)
	{
		m_bPushMoveButton	 = TRUE;
		CInputDialogBase* pDlg = GetDialogByOrder(FALSE);
		ShowDialog(pDlg, FALSE);
		m_bPushMoveButton	= FALSE;
	}
}

void CARcBridgeInputStd::AddDialog(UINT nID, CInputDialogBase* pDlg, long nMenu)
{	
	m_IDDArray[nMenu].Add(pDlg->GetIDD());
	pDlg->SetParentStd(this);
	pDlg->SetMenuID(nID);
	m_DialogList.Add(pDlg);
}

BOOL CARcBridgeInputStd::IsEnableDialog(UINT nID)
{
	CInputDialogBase* pDlg = GetDialogByMenuID(nID);
	if(!pDlg) return FALSE;

	return pDlg->IsUsedFlag() && pDlg->IsValid();
}

CInputDialogBase* CARcBridgeInputStd::FindValidDlg(CInputDialogBase* pDlg, BOOL bNext)
{	
	BOOL bStart = FALSE;
	if(bNext)
	{		
		long n=0; for(n=0; n<m_DialogList.GetSize(); n++)
		{
			if(m_DialogList[n] == pDlg) bStart = TRUE;
			if(bStart && m_DialogList[n]->IsValid()) return m_DialogList[n];
		}
	}
	else
	{
		long n=0; for(n=m_DialogList.GetSize()-1; n>=0; n--)
		{
			if(m_DialogList[n] == pDlg) bStart = TRUE;
			if(bStart && m_DialogList[n]->IsValid()) return m_DialogList[n];
		}
	}

	return NULL;
}

CInputDialogBase* CARcBridgeInputStd::GetDialogByOrder(BOOL bNext)
{
	if(m_pCurrentDialog)
		m_nIDDLast	= m_pCurrentDialog->GetIDD();

	int i=0; for(i=0; i<m_DialogList.GetSize(); i++)
	{
		if(m_DialogList[i]==m_pCurrentDialog)
		{
			if(i==m_DialogList.GetSize()-1 && bNext)
				return NULL;

			if(i==0 && !bNext)
				return NULL;

			if(bNext)
			{
				return m_DialogList[i+1];
			}
			else
			{
				return m_DialogList[i-1];
			}
		}
	}

	return NULL;
}

CInputDialogBase* CARcBridgeInputStd::GetDialogByMenuID(UINT nMenuID)
{	
	for(int n=0; n<m_DialogList.GetSize(); n++)
	{
		if(m_DialogList[n]->GetMenuID()==nMenuID)
		{
			//if(m_DialogList[n]->GetIDD()==IDD_BASE9_CROSS_CONSTRUCT && m_pARcBridgeDataStd->IsRailDesign())
			//return m_DialogList[n+1];

			return m_DialogList[n];
		}
	}

	return NULL;
}

CInputDialogBase* CARcBridgeInputStd::GetDialogByIDD(UINT nIDD)
{
	for(int n=0; n<m_DialogList.GetSize(); n++)
	{
		if(m_DialogList[n]->GetIDD()==nIDD) return m_DialogList[n];
	}

	return NULL;	
}

UINT CARcBridgeInputStd::GetMenuID(CInputDialogBase* pDlg)
{
	for(int n=0; n<m_DialogList.GetSize(); n++)
	{
		if(	m_DialogList[n] == pDlg ) return m_DialogList[n]->GetMenuID();
	}

	return 0;
}

UINT CARcBridgeInputStd::ShowDialog(CInputDialogBase* pDlg, BOOL bNext)
{
	if(!HideDialog(FindValidDlg(pDlg, bNext))) return 0;
	
	pDlg = FindValidDlg(pDlg, bNext);
	if(pDlg)
	{
		SetCurrentDialog(pDlg);
		CheckZoomAllOnInitDialog();
	
		UINT nMenuID = pDlg->GetMenuID();
		AfxGetMainWnd()->SendMessage(WM_USER+2, nMenuID);

		return nMenuID;
	}
	else
	{
		AfxGetMainWnd()->SendMessage(WM_COMMAND, 88888);
	}

	return 0;
}

BOOL CARcBridgeInputStd::HideDialog(CInputDialogBase* pDlgNext)
{
	if(m_pCurrentDialog && pDlgNext)
	{
		// �����̵��� �Է�â�� ������ �Ѿ� �� �� �ִ� â�̶�� IsOKMoveOtherDlg�� �˻����� �ʴ´�.
		BOOL bCheckMoveOtherDlg	= pDlgNext ? pDlgNext->IsCheckMoveOtherDlg() : TRUE;

		if(m_pCurrentDialog->IsOKMoveOtherDlg() || !bCheckMoveOtherDlg)
		{
			m_pCurrentDialog->m_pView->SetGL(FALSE);
			m_pCurrentDialog->KillFocusDlg();
			m_pCurrentDialog = NULL;
		}
		else
			return FALSE;
	}

	return TRUE;
}

void CARcBridgeInputStd::SetCurrentDialog(CInputDialogBase* pDlg)
{
	UINT nIDD = pDlg->GetIDD();
	m_pCurrentDialog = GetDialogByIDD(nIDD);
	m_paneInputDialog.SetInputDialog(m_pCurrentDialog);
}

void CARcBridgeInputStd::GoCurrentDlg()
{
	if(m_nSaveMenuID != -1)
	{
		CInputDialogBase *pDlg = GetDialogByIDD(m_nSaveMenuID);
		if(pDlg)
		{
			pDlg = FindValidDlg(pDlg, TRUE);
			if(pDlg)
				AfxGetMainWnd()->SendMessage(WM_COMMAND,pDlg->GetMenuID(),0);	
			else
				AfxGetMainWnd()->SendMessage(WM_COMMAND,m_nSaveMenuID,0);
		}
		else
			AfxGetMainWnd()->SendMessage(WM_COMMAND,m_nSaveMenuID,0);	
	}
}

// ���� �Է����� ���� ����
CRcBridgeApp* CARcBridgeInputStd::GetRcBridge()
{
	if(m_nCurrentBridge == -1 && m_pARcBridgeDataStd->GetBridgeSize() > 0)
		m_nCurrentBridge	= 0;

	if(m_pARcBridgeDataStd->GetBridgeSize() <= m_nCurrentBridge)
		m_nCurrentBridge	= m_pARcBridgeDataStd->GetBridgeSize()-1;

	return m_pARcBridgeDataStd->GetBridge(m_nCurrentBridge);
}

// ���� �Է����� ������� ����
CRcBridgeRebar* CARcBridgeInputStd::GetRcBridgeRebar()
{
	if(m_nCurrentBridge == -1 && m_pARcBridgeDataStd->GetBridgeSize() > 0)
		m_nCurrentBridge	= 0;

	if(m_pARcBridgeDataStd->GetBridgeSize() <= m_nCurrentBridge)
		m_nCurrentBridge	= m_pARcBridgeDataStd->GetBridgeSize()-1;

	return m_pARcBridgeDataStd->GetBridgeRebar(m_nCurrentBridge);
}

CLineInfoApp* CARcBridgeInputStd::GetLineApp()
{
	return m_pARcBridgeDataStd->GetLineApp();
}

void CARcBridgeInputStd::SetComment(CString str, BOOL bWarring)
{
	m_strHelp = str;
	CString strHelp = GetComment();
	m_paneInputDialog.m_wndComment.SetDescString(strHelp, bWarring);
	if(m_paneInputDialog.m_wndComment.GetSafeHwnd())
		m_paneInputDialog.m_wndComment.Invalidate();
}

CString CARcBridgeInputStd::GetComment()
{
	return m_strHelp;
}

void CARcBridgeInputStd::SetScaleInputDialog(double dScale)	// ������ �����Լ�
{
	if(!GetCurrentDialog()) return;
	if(dScale < MIN_SCALE) dScale	= MIN_SCALE;
	if(dScale > MAX_SCALE) dScale	= MAX_SCALE;
		

	long nMenuID	= GetCurrentDialog()->GetMenuID();
	long nIdx		= GetIdxInputDialog();
	if(nIdx == -1)
		m_xyArrScaleInputDialog.Add(CDPoint(nMenuID, dScale));
	else
		m_xyArrScaleInputDialog.SetAt(nIdx, CDPoint(nMenuID, dScale));
}

double CARcBridgeInputStd::GetScaleInputDialog()				// ������ ���� �Լ�(������ �⺻�� ����)
{
	if(!GetCurrentDialog()) return RCBRI_DEFAULT_SCALE;

	long nIdx		= GetIdxInputDialog();
	if(nIdx == -1)
		SetScaleInputDialog(GetCurrentDialog()->m_dScaleDefault);
	nIdx	= GetIdxInputDialog();

	CDPoint xyMenu	= m_xyArrScaleInputDialog.GetAt(nIdx);
	return xyMenu.y;
}

double CARcBridgeInputStd::IncreaseScaleInputDialog(BOOL bBig)			// ���� �Է´��̾�α��� �������� �����Ű�� ����
{
	if(!GetCurrentDialog()) return RCBRI_DEFAULT_SCALE;
	
	double dGap		= GetCurrentDialog()->m_dScaleOffset;
	double dScale	= GetScaleInputDialog() + (bBig ? dGap * 3 : dGap);
	SetScaleInputDialog(dScale);
	
	return dScale;
}

double CARcBridgeInputStd::DecreaseScaleInputDialog(BOOL bBig)			// ���� �Է´��̾�α��� �������� ���ҽ�Ű�� ����
{
	if(!GetCurrentDialog()) return RCBRI_DEFAULT_SCALE;
	
	double dGap		= GetCurrentDialog()->m_dScaleOffset;
	double dScale	= GetScaleInputDialog() - (bBig ? dGap * 3 : dGap);
	SetScaleInputDialog(dScale);
	
	return dScale;
}


long CARcBridgeInputStd::GetIdxInputDialog()				// ���� �Է´��̾�α��� �������� ���� �Ǿ� �ִ� index ����
{
	if(!GetCurrentDialog()) return -1;

	long nMenuID	= GetCurrentDialog()->GetMenuID();
	long nIdx	= -1;
	CDPoint xyMenu(0, 0);
	long nSize	= m_xyArrScaleInputDialog.GetSize();
	long i = 0; for(i = 0; i < nSize; i++)
	{
		xyMenu	= m_xyArrScaleInputDialog.GetAt(i);
		if(long(xyMenu.x ) == nMenuID)
		{
			nIdx	= i;
			break;
		}
	}

	return nIdx;
}

// void CARcBridgeInputStd::SetEnableScaleInputDialog()			// ������ ���� Ȱ��ȭ
// {
// 	m_bFlagScaleInputDialog	= TRUE;
// }
// void CARcBridgeInputStd::SetDisableScaleInputDialog()			// ������ ���� ��Ȱ��ȭ
// {
// 	m_bFlagScaleInputDialog	= FALSE;
// }

BOOL CARcBridgeInputStd::IsScaleInputDialog()
{
	if(!GetCurrentDialog()) return FALSE;
	return m_bFlagScaleInputDialog && GetCurrentDialog()->m_dScaleOffset != 0;
}

// ���� �׸��忡 message�� ����Ѵ�.
// �׸��忡 ����� ������ ������ ��� �޽��� ��¿����� ����ϸ� ����.
void CARcBridgeInputStd::GridMessage(CGridCtrlMng *pGrid, CString strMessage)
{
	if(!pGrid) return;
		
	pGrid->SetGridDefault(1, 1, 1, 1, TRUE, TRUE);
	pGrid->SetCellSize(-1, -1);
	/*
	// �׸��忡 �̹��� ��� ////////////////////
	// �׸��尡 ���� �Ǹ鼭 ��Ʈ�ʵ� �Բ� ������Ƿ� delete�� �� �ʿ� ����.
	CBitmap *pBitmap = new CBitmap;
	pBitmap->LoadBitmap(IDB_BITMAP_BRITYPE1);
	pGrid->AddBitmap(0, 0, pBitmap);
	///////////////////////////////////////////
	*/
	CRect rect;
	pGrid->GetWindowRect(rect);

	pGrid->SetRowHeight(0, rect.Height()-2);
	pGrid->SetColumnWidth(0, rect.Width()-2);

	pGrid->AddCell(0, 0, strMessage);
	pGrid->UpdateData(FALSE);
}

CString CARcBridgeInputStd::GetNeoMaxInstallFolder(CWnd *pParent)
{
	CFileFind find;	
	CString szSapPath = AfxGetApp()->GetProfileString("Settings", "NeoMaxDirectory");			
	if(szSapPath.IsEmpty() || !find.FindFile(szSapPath))
	{
		CString szText = "<< NeoMax3D ���α׷��� ��ġ�� ���丮�� �����ϼ��� >>";
		CCustomFolderDialog FolderDlg("",0,pParent);		
		FolderDlg.m_bi.lpszTitle = szText.GetBuffer(0);
		if(FolderDlg.DoModal()==IDOK)
		{
			szSapPath = FolderDlg.GetPathName();
			AfxGetApp()->WriteProfileString("Settings", "NeoMaxDirectory",szSapPath);										
		}
		else
			return "";
	}
	return szSapPath;
}

CString CARcBridgeInputStd::GetSap200InstallFolder(CWnd *pParent)
{
	CFileFind find;	
	CString szSapPath = AfxGetApp()->GetProfileString("Settings", "Sap2000Directory");			
	if(szSapPath.IsEmpty() || !find.FindFile(szSapPath))
	{
		CString szText = "<< Sap2000 ���α׷��� ��ġ�� ���丮�� �����ϼ��� >>";
		CCustomFolderDialog FolderDlg("",0,pParent);		
		FolderDlg.m_bi.lpszTitle = szText.GetBuffer(0);
		if(FolderDlg.DoModal()==IDOK)
		{
			szSapPath = FolderDlg.GetPathName();
			AfxGetApp()->WriteProfileString("Settings", "Sap2000Directory",szSapPath);										
		}
		else
			return "";
	}
	return szSapPath;
}

void CARcBridgeInputStd::RunNeoMaxAll(CDialog *pDlg, long nDesignType, BOOL bAutoClose)
{
	CFileFind find;		
	
	CString szSapPath = GetNeoMaxInstallFolder(pDlg->GetParent());
	if(szSapPath.IsEmpty()) return;

	CStringArray sSapFileNameArray;
	long nSizeBridge = m_pARcBridgeDataStd->m_pRcBridgeArray.GetSize();
	long i=0; for(i=0; i<nSizeBridge; i++)
	{
		CRcBridgeRebar *pBri = m_pARcBridgeDataStd->m_pRcBridgeArray.GetAt(i);

		CString sSapFileName = Maket3dFile(pBri,nDesignType);
		sSapFileName.Replace(".s2k",".t3d");
		sSapFileNameArray.Add(sSapFileName);		
	}	
	if(sSapFileNameArray.GetSize()==0) return;
					
	CString szRunningDir;
	CString szRunningPath = m_pDoc->GetPathName();	
	GetDirectoryName(szRunningDir,szRunningPath);
	if( szRunningDir.IsEmpty() )
		return;
	::SetCurrentDirectory(szRunningDir);
	
	// �ٽ� ��� �ʱ�ȭ		
	m_SapDlg.m_pMainWnd = pDlg;
	m_SapDlg.m_bError = FALSE;
	m_SapDlg.m_bWarning = FALSE;
	if(!m_SapDlg.GetSafeHwnd()) m_SapDlg.Create(pDlg->GetParent(),pDlg);			
	m_SapDlg.SetWindowText("NeoMAX ��� ����â");		
	m_SapDlg.SetBtnText("�� ��");
	m_SapDlg.CenterWindow();
	m_SapDlg.ShowWindow(SW_SHOW);	
	m_SapDlg.m_ctlList.ResetContent();
	m_SapDlg.m_bShowCompleteMsg = FALSE;
	m_SapDlg.m_bAutoTerminal = bAutoClose;
	
	BOOL bFirst = TRUE;
	CString szRunFirst;
	long n=0; for(n=0; n<sSapFileNameArray.GetSize(); n++)
	{
		CString sSapFileName = sSapFileNameArray.GetAt(n);
		CString szSapResult = sSapFileName;							
		szSapResult.Replace(".t3d","");
		if(find.FindFile(sSapFileName))
		{
			if(bFirst)
			{
				m_SapDlg.AddCommand(STDIO_OPEN, szSapPath +  "\\neoMAX3D.exe " + sSapFileName + " /A " + szSapResult + " /K");				
				bFirst = FALSE;
			}
			else
				m_SapDlg.m_StrCmdList.AddTail(szSapPath +  "\\neoMAX3D.exe " + sSapFileName + " /A " + szSapResult + " /K");				
		}				
		else
		{
			AfxMessageBox(sSapFileName + " ������ �������� �ʽ��ϴ� !", MB_ICONSTOP);
			return;
		}						
	}
	// ����
	m_SapDlg.AddCommand(STDIO_OPEN, szRunFirst);							
}

void CARcBridgeInputStd::RunSap2000All(CDialog *pDlg, long nDesignType, BOOL bAutoClose)
{
	CFileFind find;		
	
	CString szSapPath = GetSap200InstallFolder(pDlg->GetParent());
	if(szSapPath.IsEmpty()) return;

	if(nDesignType < STM_MODEL_BRACKET_STT)
	{
		m_sSapFileNameArray.RemoveAll();

		long nSizeBridge = m_pARcBridgeDataStd->m_pRcBridgeArray.GetSize();
		long i=0; for(i=0; i<nSizeBridge; i++)
		{
			CRcBridgeRebar *pBri = m_pARcBridgeDataStd->m_pRcBridgeArray.GetAt(i);

			CARcBridgeCalcStd CalcStd(pBri);
			CalcStd.m_pFrame->MakeSapData(nDesignType,TRUE);
			CString sSapFileName = MakeS2kFile(pBri,nDesignType);

			m_sSapFileNameArray.Add(sSapFileName);		
		}
	}
	
	if(m_sSapFileNameArray.GetSize()==0) return;
	
	if(find.FindFile(szSapPath + "\\Sapre.exe"))
	{				
		CString szRunningDir;
		CString szRunningPath = m_pDoc->GetPathName();	
		GetDirectoryName(szRunningDir,szRunningPath);
		::SetCurrentDirectory(szRunningDir);
		
		// �ٽ� ��� �ʱ�ȭ		
		m_SapDlg.m_pMainWnd = pDlg;
		m_SapDlg.m_bError = FALSE;
		m_SapDlg.m_bWarning = FALSE;
		if(!m_SapDlg.GetSafeHwnd()) m_SapDlg.Create(pDlg->GetParent(),pDlg);			
		m_SapDlg.SetWindowText("SAP2000 ��� ����â");		
		m_SapDlg.SetBtnText("�� ��");
		m_SapDlg.CenterWindow();
		m_SapDlg.ShowWindow(SW_SHOW);	
		m_SapDlg.m_ctlList.ResetContent();
		m_SapDlg.m_bShowCompleteMsg = FALSE;
		m_SapDlg.m_bAutoTerminal = bAutoClose;
		
		BOOL bFirst = TRUE;
		CString szRunFirst;
		long n=0; for(n=0; n<m_sSapFileNameArray.GetSize(); n++)
		{
			CString sSapFileName = m_sSapFileNameArray.GetAt(n);
			CString szSapPre, szSapGo;					
			szSapPre = sSapFileName;
			szSapGo = sSapFileName;
			szSapPre = szSapPre.Mid(szRunningDir.GetLength()+1); 
			szSapGo = szSapGo.Mid(szRunningDir.GetLength()+1);
			szSapPre.Replace(" ","_");
			szSapGo.Replace(" ","_");
			szSapGo.Replace(".s2k","");
			if(find.FindFile(szSapPre))
			{
				if(bFirst)
				{
					szRunFirst = szSapPath + "\\Sapre.exe " + szSapPre;
					bFirst = FALSE;
				}
				else
					m_SapDlg.m_StrCmdList.AddTail(szSapPath + "\\Sapre.exe " + szSapPre);				
				
				m_SapDlg.m_StrCmdList.AddTail(szSapPath + "\\Sapgo.exe " + szSapGo);				
			}				
			else
			{
				AfxMessageBox(sSapFileName + " ������ �������� �ʽ��ϴ� !", MB_ICONSTOP);
				return;
			}						
		}
		// ����
		m_SapDlg.AddCommand(STDIO_OPEN, szRunFirst);				
	}
	else
	{		
		CSap200RuntimeDlg RuntimeDlg;
		RuntimeDlg.m_szMsg = szSapPath + "\\Sapre.exe" +" ������ ã�� �� �����ϴ�.\n\n�ڵ� ���縦 Ŭ���Ͻø� Sap2000 ��ġ�� ���丮 �Ϻο� ��ġ�� Utility ���丮���� Sapre.exe�� �ڵ����� �մϴ�.\n\n�ڵ����� �� �ٽ� �����Ͻñ� �ٶ��ϴ�.";
		RuntimeDlg.DoModal();		
		return;
	}		
}

void CARcBridgeInputStd::RunNeoMax(CRcBridgeRebar *pBri, long nDesignType, CDialog *pDlg, BOOL bAutoClose)
{
	CFileFind find;	
	
	CString szSapPath = GetNeoMaxInstallFolder(pDlg->GetParent());
	if(szSapPath.IsEmpty()) return;

	CString sSapFileName = Maket3dFile(pBri,nDesignType);
	sSapFileName.Replace(".s2k",".t3d");

	CString szRunningDir;
	CString szRunningPath = m_pDoc->GetPathName();	
	GetDirectoryName(szRunningDir,szRunningPath);
	::SetCurrentDirectory(szRunningDir);
	
	// �ٽ� ��� �ʱ�ȭ		
	m_SapDlg.m_pMainWnd = pDlg;
	m_SapDlg.m_bError = FALSE;
	m_SapDlg.m_bWarning = FALSE;
	if(!m_SapDlg.GetSafeHwnd()) m_SapDlg.Create(pDlg->GetParent(),pDlg);			
	m_SapDlg.SetWindowText("NeoMAX ��� ����â");
	m_SapDlg.SetBtnText("�� ��");
	m_SapDlg.CenterWindow();
	m_SapDlg.ShowWindow(SW_SHOW);	
	m_SapDlg.m_ctlList.ResetContent();
	m_SapDlg.m_bShowCompleteMsg = FALSE;
	m_SapDlg.m_bAutoTerminal = bAutoClose;
	CString szSapResult = sSapFileName;					
	szSapResult.Replace(".t3d","");
	if(find.FindFile(sSapFileName))
	{
		m_SapDlg.AddCommand(STDIO_OPEN, szSapPath +  "\\neoMAX3D.exe " + sSapFileName + " /A " + szSapResult + " /K");							
	}				
	else
	{
		AfxMessageBox(sSapFileName + " ������ �������� �ʽ��ϴ� !", MB_ICONSTOP);
		return;
	}								
}

void CARcBridgeInputStd::RunSap2000(CRcBridgeRebar *pBri, long nDesignType, CDialog *pDlg, BOOL bAutoClose)
{
	CFileFind find;	
	
	CString szSapPath = GetSap200InstallFolder(pDlg->GetParent());
	if(szSapPath.IsEmpty()) return;

	CString sSapFileName(_T(""));
	if(nDesignType < STM_MODEL_BRACKET_STT)
	{
		sSapFileName= MakeS2kFile(pBri,nDesignType);
	}
	else
	{
		if(m_sSapFileNameArray.GetSize() > 0)
			sSapFileName = m_sSapFileNameArray.GetAt(0);
	}

	if(find.FindFile(szSapPath + "\\Sapre.exe"))
	{				
		CString szRunningDir;
		CString szRunningPath = m_pDoc->GetPathName();	
		GetDirectoryName(szRunningDir,szRunningPath);
		::SetCurrentDirectory(szRunningDir);
		
		// �ٽ� ��� �ʱ�ȭ		
		m_SapDlg.m_pMainWnd = pDlg;
		m_SapDlg.m_bError = FALSE;
		m_SapDlg.m_bWarning = FALSE;
		if(!m_SapDlg.GetSafeHwnd()) m_SapDlg.Create(pDlg->GetParent(),pDlg);			
		m_SapDlg.SetWindowText("SAP2000 ��� ����â");
		m_SapDlg.SetBtnText("�� ��");
		m_SapDlg.CenterWindow();
		m_SapDlg.ShowWindow(SW_SHOW);	
		m_SapDlg.m_ctlList.ResetContent();
		m_SapDlg.m_bShowCompleteMsg = FALSE;
		m_SapDlg.m_bAutoTerminal = bAutoClose;
		CString szSapPre, szSapGo;					
		szSapPre = sSapFileName;
		szSapGo = sSapFileName;
		szSapPre = szSapPre.Mid(szRunningDir.GetLength()+1); 
		szSapGo = szSapGo.Mid(szRunningDir.GetLength()+1);
		szSapPre.Replace(" ","_");
		szSapGo.Replace(" ","_");
		szSapGo.Replace(".s2k","");
		if(find.FindFile(szSapPre))
		{
			m_SapDlg.AddCommand(STDIO_OPEN, szSapPath + "\\Sapre.exe " + szSapPre);				
			m_SapDlg.m_StrCmdList.AddTail(szSapPath + "\\Sapgo.exe " + szSapGo);				
		}				
		else
		{
			AfxMessageBox(sSapFileName + " ������ �������� �ʽ��ϴ� !", MB_ICONSTOP);
			return;
		}						
	}
	else
	{		
		CSap200RuntimeDlg RuntimeDlg;
		RuntimeDlg.m_szMsg = szSapPath + "\\Sapre.exe" +" ������ ã�� �� �����ϴ�.\n\n�ڵ� ���縦 Ŭ���Ͻø� Sap2000 ��ġ�� ���丮 �Ϻο� ��ġ�� Utility ���丮���� Sapre.exe�� �ڵ����� �մϴ�.\n\n�ڵ����� �� �ٽ� �����Ͻñ� �ٶ��ϴ�.";
		RuntimeDlg.DoModal();		
		return;
	}		
}

BOOL CARcBridgeInputStd::CompleteRunOfNeoMax(CRcBridgeRebar *pBri, long nDesignType)
{
	// ���ʿ��� ���� ����	
	CString sFileName = m_pDoc->GetPathName();
	sFileName  = sFileName.Left(sFileName.ReverseFind('.'));	

	CString szBridgeName = pBri->m_strBridgeName;
	szBridgeName.Replace(" ","_");
	szBridgeName.Replace(".","_");
	if(nDesignType==0)
		sFileName += ("_" + szBridgeName + "_����");
	else if(nDesignType>0 && nDesignType<5)
	{
		CString strEq = pBri->GetNameStringEarthQuakeModel(nDesignType);
		sFileName += ("_" + szBridgeName + "_" + strEq);
	}
	else if(nDesignType==5)
		sFileName += ("_" + szBridgeName + "_PLATE");
	else if(nDesignType==6)
		sFileName += ("_" + szBridgeName + "_��������������");
	else if(nDesignType==7)
		sFileName += ("_" + szBridgeName + "_��������������");
	else if(nDesignType==8)
		sFileName += ("_" + szBridgeName + "_��������������");
	else if(nDesignType==9)
		sFileName += ("_" + szBridgeName + "_��������������");

	CFEMManage *pFEM;
	if(nDesignType==0) pFEM = &pBri->m_FemNormal;
	if(nDesignType==1) pFEM = &pBri->m_FemEQ[0];
	if(nDesignType==2) pFEM = &pBri->m_FemEQ[1];	
	if(nDesignType==3) pFEM = &pBri->m_FemEQ[2];
	if(nDesignType==4) pFEM = &pBri->m_FemEQ[3];
	if(nDesignType==5) pFEM = &pBri->m_FemShell;
	if(nDesignType==6) pFEM = &pBri->m_FemWingWall[iSTT][iLEFT];
	if(nDesignType==7) pFEM = &pBri->m_FemWingWall[iSTT][iRIGHT];
	if(nDesignType==8) pFEM = &pBri->m_FemWingWall[iEND][iLEFT];;
	if(nDesignType==9) pFEM = &pBri->m_FemWingWall[iEND][iRIGHT];

	CLoadNeoMaxOutput LoadSap(pFEM);
	try{
		LoadSap.LoadData(sFileName);
	}
	catch(...)
	{
		AfxMessageBox("����� ������������ ����Ǿ����ϴ� !", MB_ICONSTOP);
		return FALSE;
	}

	return TRUE;
}

BOOL CARcBridgeInputStd::CompleteRunOfSap2000(CRcBridgeRebar *pBri, long nDesignType)
{
	// ���ʿ��� ���� ����
	CString sFileName = m_pDoc->GetPathName();
	sFileName  = sFileName.Left(sFileName.ReverseFind('.'));	

	CString szBridgeName = pBri->m_strBridgeName;
	szBridgeName.Replace(" ","_");
	szBridgeName.Replace(".","_");

	if(pBri->IsBoxType() == FALSE)
	{

	}
	if(nDesignType==0)
		sFileName += ("_" + szBridgeName + "_����");
	else if(nDesignType>0 && nDesignType<5)
	{
		CString strEq = pBri->GetNameStringEarthQuakeModel(nDesignType);
		sFileName += ("_" + szBridgeName + "_" + strEq);
	}
	else if(nDesignType==5)
		sFileName += ("_" + szBridgeName + "_PLATE");
	else if(nDesignType==6)
		sFileName += ("_" + szBridgeName + "_��������������");
	else if(nDesignType==7)
		sFileName += ("_" + szBridgeName + "_��������������");
	else if(nDesignType==8)
		sFileName += ("_" + szBridgeName + "_��������������");
	else if(nDesignType==9)
		sFileName += ("_" + szBridgeName + "_��������������");
	else if(nDesignType>=STM_MODEL_BRACKET_STT)
		sFileName += (_T("_") + szBridgeName + _T("_") + pBri->GetStringStmModelNameByKey(nDesignType));
	
	char *Ext[] = { ".$2k", ".$OG", ".CMB", ".EKO", ".EV",  ".F11", ".F3",  ".GUU", ".H",   ".H11", ".H3", 
					".HSA", ".ID",  ".JCJ", ".JOB", ".K",   ".K11", ".K3",  ".L3",  ".LBL", ".LBM", ".LDS",
					".LIN", ".LIS", ".LOG", ".LTS", ".M11", ".M3",  ".MAS", ".MOD", ".MTL", ".P1S", ".P3",
					".P3X", ".PAR", ".PAT", ".PDE", ".PDF", ".PPD", ".SBK", ".SEC", ".SEL", ".T3",  ".TXT",
					".U",   ".U11", ".U3",  ".V",   ".V11", ".V3",  ".XMJ", ".XYZ", ".Y11", ".Y3",  ".Z11",
					".Z3",  ".SDB", ".HSC",	".F9",	".H1",	".H9",	".K9",	".LBK", ".M9",  ".NPR", ".P9",
					".RSI", ".U1",  ".U9",  ".V1",  ".V9",  ".Y9",  ".Z9",	".F1",	".K1",	".LFC",	"M1",
					".W", ".W1",  ".W3",  ".W9",  ".Z1",  ".CQC",  ".LRD",  ".BRI",
					".CSE", ".CSG",  ".CSJ",  ".CSP",  ".LBN",  ".BRR",  ".BRD",  ".BXU",  ".BXR",  ".BXP",
					".B", ".B1",  ".B11",  ".B3",  ".BRC",
					".SEV",	".Y1",	".M1",	".LCS"};
	long nCount = sizeof(Ext) / sizeof(Ext[0]);
	long n=0; for(n=0; n<nCount; n++)
	{
		try{CFile::Remove(sFileName+Ext[n]);} 
		catch(CFileException *pEx)
		{
			pEx->Delete();
		}
		catch(...){};
	}
	if(m_SapDlg.m_bError) return FALSE;

	CFEMManage *pFEM;
	if(nDesignType==0) pFEM = &pBri->m_FemNormal;
	if(nDesignType==1) pFEM = &pBri->m_FemEQ[0];
	if(nDesignType==2) pFEM = &pBri->m_FemEQ[1];	
	if(nDesignType==3) pFEM = &pBri->m_FemEQ[2];
	if(nDesignType==4) pFEM = &pBri->m_FemEQ[3];
	if(nDesignType==5) pFEM = &pBri->m_FemShell;
	if(nDesignType==6) pFEM = &pBri->m_FemWingWall[iSTT][iLEFT];
	if(nDesignType==7) pFEM = &pBri->m_FemWingWall[iSTT][iRIGHT];
	if(nDesignType==8) pFEM = &pBri->m_FemWingWall[iEND][iLEFT];;
	if(nDesignType==9) pFEM = &pBri->m_FemWingWall[iEND][iRIGHT];
	if(nDesignType>=STM_MODEL_BRACKET_STT)
	{
		CStmModel *pStm = pBri->GetStmModelByKey(nDesignType);
		if(pStm)
		{
			pFEM = pStm->GetFemManager();
		}
	}

	if(nDesignType >= 6 && nDesignType < STM_MODEL_BRACKET_STT)
	{
		// ������ Plate
		CLoadSAP2000OutputShell LoadSap(pFEM);
		try
		{
			LoadSap.LoadData(sFileName + ".Out");
		}
		catch(...)
		{
			AfxMessageBox("����� ������������ ����Ǿ����ϴ� !", MB_ICONSTOP);
			return FALSE;
		}
	}
	else
	{
		CFEMLoadSAP2000Output LoadSap(pFEM, nDesignType == 5);
		try
		{
			LoadSap.LoadData(sFileName + ".Out");
		}
		catch(...)
		{
			AfxMessageBox("����� ������������ ����Ǿ����ϴ� !", MB_ICONSTOP);
			return FALSE;
		}
	}

	return TRUE;
}

CString CARcBridgeInputStd::MakeS2kFile(CRcBridgeRebar *pBri, long nDesignType)
{
	CString szPath = AfxGetApp()->GetProfileString("Afx Directory", "Fixed Directory");			

	CString sFilePathName = m_pDoc->GetPathName();
	CString sFileTitle = m_pDoc->GetTitle();
	CString sFileName;

	sFileName  = sFilePathName.Left(sFilePathName.ReverseFind('\\'));	
	long nFind = sFileTitle.ReverseFind('.');
	sFileTitle = nFind == -1 ? sFileTitle : sFileTitle.Left(nFind);	
	sFileTitle.Replace(" ","_");
	sFileTitle.Replace(".","_");

	sFileName += _T('\\') + sFileTitle;

	CString szBridgeName = pBri->m_strBridgeName;
	szBridgeName.Replace(" ","_");
	szBridgeName.Replace(".","_");
	if(nDesignType==0)
		sFileName += ("_" + szBridgeName + "_����");
	else if(nDesignType>0 && nDesignType<5)
	{
		CString strEq = pBri->GetNameStringEarthQuakeModel(nDesignType);
		sFileName += ("_" + szBridgeName + "_" + strEq);
	}
	else if(nDesignType==5)
		sFileName += ("_" + szBridgeName + "_PLATE");
	else if(nDesignType==6)
		sFileName += ("_" + szBridgeName + "_��������������");
	else if(nDesignType==7)
		sFileName += ("_" + szBridgeName + "_��������������");
	else if(nDesignType==8)
		sFileName += ("_" + szBridgeName + "_��������������");
	else if(nDesignType==9)
		sFileName += ("_" + szBridgeName + "_��������������");

	CFileFind find;			
	CString sSapFileName = sFileName + ".s2k";	
	MakeInputData(pBri, nDesignType, sSapFileName, SOLVE_SAP_6AND7);

	return sSapFileName;
}

CString CARcBridgeInputStd::Maket3dFile(CRcBridgeRebar *pBri, long nDesignType)
{
	CString szPath = AfxGetApp()->GetProfileString("Afx Directory", "Fixed Directory");			

	CString sFileName = m_pDoc->GetPathName();
	sFileName  = sFileName.Left(sFileName.ReverseFind('.'));	

	CString szBridgeName = pBri->m_strBridgeName;
	szBridgeName.Replace(" ","_");
	szBridgeName.Replace(".","_");
	if(nDesignType==0)
		sFileName += ("_" + szBridgeName + "_����");
	else if(nDesignType>0 && nDesignType<5)
	{
		CString strEq = pBri->GetNameStringEarthQuakeModel(nDesignType);
		sFileName += ("_" + szBridgeName + "_" + strEq);
	}
	else if(nDesignType==5)
		sFileName += ("_" + szBridgeName + "_PLATE");
	else if(nDesignType==6)
		sFileName += ("_" + szBridgeName + "_��������������");
	else if(nDesignType==7)
		sFileName += ("_" + szBridgeName + "_��������������");
	else if(nDesignType==8)
		sFileName += ("_" + szBridgeName + "_��������������");
	else if(nDesignType==9)
		sFileName += ("_" + szBridgeName + "_��������������");

	CFileFind find;			
	CString sSapFileName = sFileName + ".t3d";	
	MakeInputData(pBri, nDesignType, sSapFileName, SOLVE_NEOMAX);

	return sSapFileName;
}

CString CARcBridgeInputStd::GetSolveName(long nSolveType)
{
	switch(nSolveType)
	{
	case SOLVE_SAP_6AND7:	return SOLVE_SAP6AND7_STRING;
	case SOLVE_SAP_8:		return SOLVE_SAP8_STRING;
	case SOLVE_SAP_9:		return SOLVE_SAP9_STRING;
	case SOLVE_SAP_10:		return SOLVE_SAP10_STRING;
	case SOLVE_MIDAS:		return SOLVE_MIDAS_STRING;
	case SOLVE_NEOMAX:		return SOLVE_NEOMAX_STRING;
	}
	return "";
}

void CARcBridgeInputStd::MakeInputData(CRcBridgeRebar *pBri, long nDesignType, CString szOpenFileName,long nSolveType, BOOL bKorCode)
{
	CString szEdit = AfxGetApp()->GetProfileString("Settings", "Editor");
	CString sFilePathName = m_pDoc->GetPathName();
	CString sFileTitle = m_pDoc->GetTitle();
	CString sFileName;
	
	sFileName  = sFilePathName.Left(sFilePathName.ReverseFind('\\'));
	long nFind = sFileTitle.ReverseFind('.');
	sFileTitle = nFind == -1 ? sFileTitle : sFileTitle.Left(nFind);	
	sFileTitle.Replace(" ","_");
	sFileTitle.Replace(".","_");
	
	sFileName += _T('\\') + sFileTitle;
	
	CString szBridgeName = pBri->m_strBridgeName;
	szBridgeName.Replace(" ","_");
	szBridgeName.Replace(".","_");
	CFEMManage *pFem = NULL;
	CString szName;
	if(nDesignType==0)
	{
		sFileName += ("_" + szBridgeName + "_����");
		pFem = &pBri->m_FemNormal;
		pFem->m_bApplyKoreaCode = bKorCode;
		pFem->m_szTitle.RemoveAll();
		szName.Format("ARcBridge %s ����Ÿ���� : ����\n",GetSolveName(nSolveType));		
		pFem->m_szTitle.Add(szName);
	}
	else if(nDesignType>0 && nDesignType<5)
	{
		CString strEq = pBri->GetNameStringEarthQuakeModel(nDesignType);
		sFileName += ("_" + szBridgeName + "_" + strEq);
		pFem = &pBri->m_FemEQ[nDesignType-1];
		pFem->m_bApplyKoreaCode = bKorCode;
		pFem->m_szTitle.RemoveAll();
		szName.Format("ARcBridge %s ����Ÿ���� : ������ %s\n",GetSolveName(nSolveType), strEq);
		pFem->m_szTitle.Add(szName);
	}
	else if(nDesignType==5)
	{
		sFileName += ("_" + szBridgeName + "_PLATE");
		pFem = &pBri->m_FemShell;
		pFem->m_bApplyKoreaCode = bKorCode;
		pFem->m_szTitle.RemoveAll();
		szName.Format("ARcBridge %s ����Ÿ���� : 3D PLATE\n",GetSolveName(nSolveType));
		pFem->m_szTitle.Add(szName);
	}
	else if(nDesignType==6)
	{
		sFileName += ("_" + szBridgeName + "_��������������");
		pFem = &pBri->m_FemWingWall[iSTT][iLEFT];
		pFem->m_bApplyKoreaCode = bKorCode;
		pFem->m_szTitle.RemoveAll();
		szName.Format("ARcBridge %s ����Ÿ���� : ��������������\n",GetSolveName(nSolveType));
		pFem->m_szTitle.Add(szName);
	}
	else if(nDesignType==7)
	{
		sFileName += ("_" + szBridgeName + "_��������������");
		pFem = &pBri->m_FemWingWall[iSTT][iRIGHT];
		pFem->m_bApplyKoreaCode = bKorCode;
		pFem->m_szTitle.RemoveAll();
		szName.Format("ARcBridge %s ����Ÿ���� : ��������������\n",GetSolveName(nSolveType));
		pFem->m_szTitle.Add(szName);
	}
	else if(nDesignType==8)
	{
		sFileName += ("_" + szBridgeName + "_��������������");
		pFem = &pBri->m_FemWingWall[iEND][iLEFT];
		pFem->m_bApplyKoreaCode = bKorCode;
		pFem->m_szTitle.RemoveAll();
		szName.Format("ARcBridge %s ����Ÿ���� : ��������������\n",GetSolveName(nSolveType));
		pFem->m_szTitle.Add(szName);
	}
	else if(nDesignType==9)
	{
		sFileName += ("_" + szBridgeName + "_��������������");
		pFem = &pBri->m_FemWingWall[iEND][iRIGHT];
		pFem->m_bApplyKoreaCode = bKorCode;
		pFem->m_szTitle.RemoveAll();
		szName.Format("ARcBridge %s ����Ÿ���� : ��������������\n",GetSolveName(nSolveType));
		pFem->m_szTitle.Add(szName);
	}
	else if(nDesignType>=STM_MODEL_BRACKET_STT)
	{
		sFileName += ("_" + szBridgeName + pBri->GetStringStmModelNameByKey(nDesignType));
		pFem = pBri->GetStmModelByKey(nDesignType)->GetFemManager();
		pFem->m_bApplyKoreaCode = bKorCode;
		pFem->m_szTitle.RemoveAll();
		szName.Format("ARcBridge STM ����Ÿ���� : %s\n", pBri->GetStringStmModelNameByKey(nDesignType));	
		pFem->m_szTitle.Add(szName);
	}

	if(szOpenFileName.IsEmpty())
	{
		CString szExt;				
		CString szFileCommet;
		if(nSolveType==SOLVE_SAP_6AND7) 
		{
			szExt = "*.s2k|*.*";
			szFileCommet = "Sap2000 Input File(*.s2k)|*.s2k|������� (*.*)|*.*||";
		}
		else if(nSolveType==SOLVE_SAP_8 || nSolveType==SOLVE_SAP_9 || nSolveType==SOLVE_SAP_10) 
		{
			CXLControl excel;
			double dVersion = excel.GetVersion();

			szExt = (dVersion < 12) ? _T("*.xls|*.*") : _T("*.xlsx|*.*");
			if(dVersion < 12)
				szFileCommet.Format(_T("Sap2000 Input File(*.xls)|*.xls|%s (*.*)|*.*||"),hggettext("�������"));
			else
				szFileCommet.Format(_T("Sap2000 Input File(*.xlsx)|*.xlsx|%s (*.*)|*.*||"),hggettext("�������"));
		}
		else if(nSolveType==SOLVE_MIDAS)
		{
			szExt = "*.mct|*.*";
			szFileCommet = "MIDAS Input File(*.mct)|*.mct|������� (*.*)|*.*||";
		}
		else if(nSolveType==SOLVE_NEOMAX)
		{
			szExt = "*.t3d|*.*";
			szFileCommet = "NEOMAX3D Input File(*.t3d)|*.t3d|������� (*.*)|*.*||";
		}
		CFileDialog dlg(FALSE, szExt,sFileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
							   szFileCommet);		
		dlg.m_ofn.lpstrTitle = (nSolveType>=SOLVE_SAP_6AND7 && nSolveType<=SOLVE_SAP_10) ? "SAP Input ����" : (nSolveType==SOLVE_MIDAS) ? "MIDAS Input ����" : "NEOMAX3D Input ����";
		if (dlg.DoModal() != IDOK) return;
		sFileName = dlg.GetPathName();
	}	
	else sFileName = szOpenFileName;

	pFem->m_szTitle.Add("TITLE : " + pBri->m_strBridgeName + "\n");
	if(nSolveType==SOLVE_SAP_6AND7)		pFem->SaveAsSap2000(sFileName);
	else if(nSolveType==SOLVE_SAP_8 || nSolveType==SOLVE_SAP_9 || nSolveType==SOLVE_SAP_10)		
	{
		long nVer = nSolveType==SOLVE_SAP_8 ? 8 : nSolveType==SOLVE_SAP_9 ? 9 : 10;
		CString szPath = AfxGetApp()->GetProfileString("Afx Directory", "Fixed Directory");			
		pFem->SaveAsSap2000_V8V9V10(szPath,sFileName, nVer, NULL, NULL, NULL);
	}
	else if(nSolveType==SOLVE_MIDAS)	
	{
		pFem->m_szMidasVersion = pBri->m_nSelectMidasVer == 0 ? MIDAS_VER_4DOT3 : MIDAS_VER_8DOT6;
		pFem->SaveAsMidas(sFileName);
	}
	else if(nSolveType==SOLVE_NEOMAX)	pFem->SaveAsNeoMax3D(sFileName);

//////////////////////////////////////////////////////////////////////////
	if((nSolveType==SOLVE_SAP_6AND7 || nSolveType==SOLVE_NEOMAX) && szOpenFileName.IsEmpty())
	{
//		ShellExecute(NULL,NULL,szEdit,"\"\tNEW" + sFileName + "\"",NULL,SW_SHOWNORMAL);		
		ShellExecute(NULL,NULL,szEdit,sFileName,NULL,SW_SHOWNORMAL);		
	}
	else if(nSolveType==SOLVE_MIDAS && szOpenFileName.IsEmpty())
	{
		ShellExecute(NULL,"open", sFileName, NULL, NULL,SW_SHOWNORMAL);			
	}
	// OUTPUT
}

void CARcBridgeInputStd::GetDirectoryName(CString &strDirName, const CString& strPathName)
{
  char drive[_MAX_DRIVE], dir[_MAX_DIR],
       fname[_MAX_FNAME], ext[_MAX_EXT];

  _SPLITPATH_S(strPathName, drive, dir, fname, ext);
  strDirName = drive;
  strDirName += dir;

  if(strDirName[strDirName.GetLength()-1]=='\\')
	strDirName = strDirName.Left(strDirName.GetLength()-1);
}
BOOL CARcBridgeInputStd::IsViewVectorInfo()
{
	BOOL bView	= FALSE;
#ifdef _DEBUG
	bView = m_bViewVectorInfo;
// #else
// 	bView = FALSE;
#endif

	return bView;
}

// ���ڿ� �迭���� ���ڿ� ����
// �ε����� �̻��� ��� �״°� ���� �ϱ� ����.
CString CARcBridgeInputStd::GetStringArray(CStringArray &sArr, long nIdx)
{
	if(nIdx < 0 || nIdx >= sArr.GetSize()) return _T("");
	return sArr.GetAt(nIdx);
}

// ���� �׷��� ���鿡 dMargin��ŭ ������ ���� ����
void CARcBridgeInputStd::DrawMarginPoint(CDomyun *pDomP, double dRatio)
{
	pDomP->SetCalcExtRect();
	CDRect rect	= pDomP->GetExtRect();

	CDomyun Dom(pDomP);

	Dom.SetLineColor(-1);
	Dom.Point(rect.left-rect.Width()/dRatio,rect.bottom+rect.Height()/dRatio);
	Dom.Point(rect.right+rect.Width()/dRatio,rect.top-rect.Height()/dRatio);

	*pDomP << Dom;
}

long CARcBridgeInputStd::GetLeft()
{
	long nLeft	= 0;
#ifdef _DEBUG
	nLeft = m_nLeft;
#else
	nLeft = 0;
#endif

	return nLeft;
}

// ó�� �Է�â�� �㶧 �� ���� �ؾ� �ϴ��� üũ�ϴ� �κ�
void CARcBridgeInputStd::CheckZoomAllOnInitDialog()
{
	m_bZoomAllOnInitDialog	= TRUE;

	CInputDialogBase *pDlg	= GetCurrentDialog();
	if(!pDlg)
	{
		return;
	}

	// ��ö�� 1,2����Ŭ �̵��� ���� ����
	if(m_nIDDLast == m_dlgRebarMain.GetIDD() && pDlg->GetIDD() == m_dlgRebarMain2.GetIDD())
		m_bZoomAllOnInitDialog	= FALSE;
	if(m_nIDDLast == m_dlgRebarMain2.GetIDD() && pDlg->GetIDD() == m_dlgRebarMain.GetIDD())
		m_bZoomAllOnInitDialog	= FALSE;
}

// �Է�â Ȱ�� ��Ȱ�� ����
void CARcBridgeInputStd::SetDialogEnable()
{
	long nCount	= m_DialogList.GetSize();
	long i = 0; for(i = 0; i < nCount; i++)
	{
		CInputDialogBase *pDlg	= m_DialogList.GetAt(i);
		if(!pDlg->IsValid())
			pDlg->SetUsedFlag(FALSE);
	}

	AfxGetMainWnd()->SendMessage(WM_USER+8, -1);
	AfxGetMainWnd()->SendMessage(WM_USER+13);
}

// void CARcBridgeInputStd::StartSpeedTest()
// {
// 	GetLocalTime(&m_Time);
// }


// void CARcBridgeInputStd::EndSpeedTest()
// {
// 	SYSTEMTIME endTime;
// 	GetLocalTime(&endTime);
// 
// 	SYSTEMTIME time;
// 	time.wMinute	= endTime.wMinute - m_Time.wMinute;
// 	time.wSecond	= endTime.wSecond - m_Time.wSecond;
// 	time.wMilliseconds	= endTime.wMilliseconds - m_Time.wMilliseconds;
// 
// 	if(time.wMilliseconds < 0)
// 	{
// 		time.wSecond -= 1;
// 		time.wMilliseconds += 1000;
// 	}
// 
// 	if(time.wSecond < 0)
// 	{
// 		time.wMinute -= 1;
// 		time.wSecond += 60;
// 	}
// 
// 	CString sTime	= _T("");
// 	sTime.Format("%d�� %d�� %d", time.wMinute, time.wSecond, time.wMilliseconds);
// 
// 	AfxMessageBox(sTime);
// }

BOOL CARcBridgeInputStd::CheckStation()
{
	CARcBridgeDataStd* pStd = m_pARcBridgeDataStd;
	long nCountBri	= pStd->GetBridgeSize();

	CStringArray sArrBridge;
	long i = 0; for(i = 0; i < nCountBri; i++)
	{
		CRcBridgeRebar *pBri	= pStd->GetBridgeRebar(i);
		if(pBri)
		{
			if(!pBri->IsValidStationOfBri())
			{
				sArrBridge.Add(pBri->m_strBridgeName);
			}

			// ���� üũ�ø��� ������ �������� �̷�� ������ �˻��Ѵ�.
			pBri->m_bIsIncludeOnlyLine	= pBri->IsIncludeOnlyLineInBridge();
		}
	}

	if(sArrBridge.GetSize() > 0)
	{
		CString str	= _T("");
		long i = 0; for(i = 0; i < sArrBridge.GetSize(); i++)
		{
			str += "["+sArrBridge.GetAt(i)+"]";
		}

		str = str + " ������ Station�� ������ ���� �� �� �ֽ��ϴ�.";
		SetComment(str, TRUE);
	}
	else
		SetComment("");



	return sArrBridge.GetSize() == 0;
}

// ��� ���� �ʿ��ϵ��� ����
void CARcBridgeInputStd::SettingNeedPlane()
{
	long nCountBridge	= m_pARcBridgeDataStd->GetBridgeSize();
	for(long bri = 0; bri < nCountBridge; bri++)
	{
		CRcBridgeApp *pBri	= m_pARcBridgeDataStd->GetBridge(bri);
		pBri->m_bSettingPlane	= FALSE;
		pBri->m_bNeedSettingPlane	= TRUE;
	}
}

// Ȯ���� �����ο� ���� ��� �޽���
void CARcBridgeInputStd::MessageExpAndGagak(BOOL bGrid, CGridCtrlMng *pGrid, long nRow, long nCol)
{
	BOOL bOK	= FALSE;
	long nCountBridge	= m_pARcBridgeDataStd->GetBridgeSize();
	for(long bri = 0; bri < nCountBridge; bri++)
	{
		CRcBridgeApp *pBri	= m_pARcBridgeDataStd->GetBridge(bri);
		if(!pBri) continue;

		if(pBri->IsExtLine())
		{
			bOK	= TRUE;
			break;
		}

		if(pBri->m_bIsExp)
		{
			bOK	= TRUE;
			break;
		}
	}

	if(bOK)
	{
		CString sMessage	= "Ȯ����(������)�� ���ؼ��� ����(�Ϲݵ�, ��ٵ�)�� �����ϰ� ������,\n��꼭, ������ ���ؼ��� �������� ���Ǵ� ���� �����Ͻñ� �ٶ��ϴ�.";
		if(bGrid)
		{
			if(pGrid)
			{
				pGrid->ShowHelpToolTip(nRow, nCol, sMessage);
			}
		}
		else
		{
			AfxMessageBox(sMessage);
		}
	}
}

// ���� ���̰� �������̸� �Ѿ ��� �޽��� ���
// void CARcBridgeInputStd::MessageGagakLength(CGridCtrlMng *pGrid, long nRow, long nCol)
// {
// 
// }

void CARcBridgeInputStd::ShowDlgHDanLength()
{
	if(!m_pDlgHDanLength->GetSafeHwnd())
	{
		m_pDlgHDanLength->Create(IDD_DIALOG_HDAN_LENGTH, NULL);
	}

	m_pDlgHDanLength->ShowWindow(SW_SHOW);
}


UINT CARcBridgeInputStd::GetIDDInputDialogByFlag1(UINT nIDDOld)
{
	if(nIDDOld == 7002)	return IDD_BASE1_PROJECTINFO;
	if(nIDDOld == 7005)	return IDD_BASE2_DESIGN_BASICDATA;
	if(nIDDOld == 7019)	return IDD_BASE3_LINEINFO;
	if(nIDDOld == 7021)	return IDD_BASE4_BRIDGE_FORMFACTOR;
	if(nIDDOld == 7029)	return IDD_BASE6_CROSS_POSITION;
	if(nIDDOld == 7030)	return IDD_BASE7_SPAN_CONSTRUCT;
	if(nIDDOld == 7050)	return IDD_BASE8_SHOE_POSITION;
	if(nIDDOld == 7051)	return IDD_BASE9_CROSS_CONSTRUCT;
	if(nIDDOld == 7054)	return IDD_BASE5_GROUNDINFO;
	if(nIDDOld == 7020)	return IDD_GEN_DESIGN_VARIABLE;
	if(nIDDOld == 7022)	return IDD_GEN_JONG_SECTION;
	if(nIDDOld == 7052)	return IDD_GEN_COLUMN;
	if(nIDDOld == 7053)	return IDD_GEN_FOOTING;
	if(nIDDOld == 7210)	return IDD_GEN_APS;
	if(nIDDOld == 7213)	return IDD_GEN_WINGWALL;
	if(nIDDOld == 7215)	return IDD_GEN_PLANE;
	if(nIDDOld == 7413)	return IDD_GEN_HYUNTA;		// ����Ÿ���� ���ʺο��� �˾����� ����
	if(nIDDOld == 8094)	return IDD_GEN_SODAN;
	if(nIDDOld == 8153)	return IDD_GEN_FRAME_ANLYSIS;
	if(nIDDOld == 8154)	return IDD_GEN_SHELL_ANLYSIS;
	if(nIDDOld == 8155)	return IDD_GEN_EARTHQUAKE_ANLYSIS;
	if(nIDDOld == 8158)	return IDD_GEN_TOGONG;
	if(nIDDOld == 8198)	return IDD_GEN_MONUMENT;
	if(nIDDOld == 7217)	return IDD_REBAR_MAIN;
	if(nIDDOld == 8083)	return IDD_REBAR_SECTION;
	if(nIDDOld == 8086)	return IDD_REBAR_COLUMN;
	if(nIDDOld == 8087)	return IDD_REBAR_WINGWALL;
	if(nIDDOld == 8088)	return IDD_REBAR_OTHERS;
	if(nIDDOld == 8090)	return IDD_REBAR_JEWON;
	if(nIDDOld == 8092)	return IDD_REBAR_APS;  
	if(nIDDOld == 8161)	return IDD_REBAR_MAIN2;
	if(nIDDOld == 8163)	return IDD_REBAR_MAIN3;
	if(nIDDOld == 8199)	return IDD_REBAR_MAIN4;
	if(nIDDOld == 8199)	return IDD_REBAR_GAGAK;

	return nIDDOld;
}

CString CARcBridgeInputStd::GetManualPath() const
{
	CString strPath = AfxGetApp()->GetProfileString("Afx Directory", "Fixed Directory");

	strPath += "\\";
	strPath += "ARcBridge_manual.pdf";

	return strPath;
}

CStmUbRenderer * CARcBridgeInputStd::GetUbRenderer()
{
	return m_pUbRenderer;
}

void CARcBridgeInputStd::SetUbRenderer( CStmUbRenderer *pUbRenderer )
{
	m_pUbRenderer = pUbRenderer;
}

CStmDataConfig * CARcBridgeInputStd::GetStmDataConfig( HGINT32 ixModelKey )
{
	CRcBridgeApp *pBridge = m_pARcBridgeDataStd->GetBridge(m_nCurrentBridge);
	if(!pBridge) return NULL;

	CStmModel* pModel = pBridge->GetStmModelByKey(ixModelKey);
	if(!pModel) return NULL;

	CStmDataConfig *pStmDataConfig = pModel->GetStmDataConfig();
	if(!pStmDataConfig) return NULL;

	return pStmDataConfig;
}

CString CARcBridgeInputStd::GetPathDirectory()
{
	CString szDir(_T(""));

	if(m_pDoc)
	{
		CString szPath = m_pDoc->GetPathName();
		GetDirectoryName(szDir, szPath);
	}

	return szDir;
}
