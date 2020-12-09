// ARcBridgeDoc.cpp : implementation of the CARcBridgeDoc class
//

#include "stdafx.h"
#include "ARcBridge.h"
#include "MainFrm.h"
#include "Splash.h"

#include "../Src/ARcBridgeInput/ARcBridgeInput.h"
#include "../Src/ARcBridgeDrawing/ARcBridgeDrawing.h"
#include "../Src/ARcBridgeDrawingOption/ARcBridgeDrawingOption.h"
#include "../Src/ARcBridgeOutBM/ARcBridgeOutBM.h"
#include "../Src/ARcBridgeOutDesign/ARcBridgeOutDesign.h"
#include "../Src/ARcBridgeCalculate/ARcBridgeCalculate.h"
#include "ARcBridgeDoc.h"
#include "HgBaseDraw.h"

#include "spromeps.h" // RainBow Lock Driver

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CARcBridgeDoc

IMPLEMENT_DYNCREATE(CARcBridgeDoc, CDocument)

BEGIN_MESSAGE_MAP(CARcBridgeDoc, CDocument)
	//{{AFX_MSG_MAP(CARcBridgeDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CARcBridgeDoc::m_bFirstDocument = FALSE;


/////////////////////////////////////////////////////////////////////////////
// CARcBridgeDoc construction/destruction

CARcBridgeDoc::CARcBridgeDoc()
{
	m_nActiveIDD = 0;
	m_bFileOpen = FALSE;
	m_nCurBriNoOfSettingDomyun	= 0;
	m_strCurFileOpenPathName = _T("");
	m_sFileSaveInfo = _T("");
	
	CreateMember();

	//==============================================================================================
	//
	//  LOCK CHECK MODULE - "Lock" �ʱ�ȭ (���������� ������ �� 1ȸ�� �ʱ�ȭ �˴ϴ�)
	//
	//  !! �Ʒ� ��õ� Lock �Լ�(RNBO_xxx)����, �ݵ�� ���� ȣ������� ������ �ּž� �մϴ� !!
	//
	//==============================================================================================
	CARcBridgeApp *pApp = (CARcBridgeApp*)AfxGetApp();
	if (!CARcBridgeDoc::m_bFirstDocument)	// ���� ������ ���� Lock�� üũ��
	{
		if(aIsLockOK() || LDK::aIsLogIn())	//SLLock_20150526
		{		
			if(LDK::aCheckLicenseHGKey())
			{			
				m_pARcBridgeDataStd->SetVersionFlag(LDK::aGetModuleNumber());
				
				CSplashWnd::SetLSD(m_pARcBridgeDataStd->IsLockRcBridgeLsdModule());
				CSplashWnd::ShowSplashScreen(AfxGetMainWnd());

				// CG: The following line was added by the Splash Screen component.
				CEnvRegist env;
				if(env.LoadEnvInt(CEnvRegist::SET_PRGOPT))
				{
					CARcBridgeApp *pApp	= (CARcBridgeApp*)AfxGetApp();
					CSplashWnd::ShowSplashScreen(AfxGetMainWnd());
				}

				// HCAD_SYMBOL dxf���� �о����
				CString szPath = AfxGetApp()->GetProfileString("Afx Directory", "Fixed Directory");
				CString strDxfName = _T("");
				strDxfName.Format("%s\\DXF\\HCAD_Symbol.dxf", szPath);
				if(CSplashWnd::c_pSplashWnd->GetSafeHwnd())
				{
					CHgBaseDrawStd::GetHCadSymbolDom()->SetProgress(&CSplashWnd::c_pSplashWnd->m_ProcessBar, NULL);
				}
				
				CHgBaseDrawStd::GetHCadSymbolDom()->DxfIn(strDxfName);

				//==========================================================================================
				//  !! ��� ���� �����... ���ð� �ʱ�ȭ !!
				//==========================================================================================
				CARcBridgeDoc::m_bFirstDocument = TRUE;
			}
		}
	}
	//==============================================================================================
	//
	//  LOCK CHECK MODULE ����
	//
	//==============================================================================================
}

CARcBridgeDoc::~CARcBridgeDoc()
{
	ClearMember();
}

BOOL CARcBridgeDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	m_nActiveIDD = 0;
	m_bFileOpen = FALSE;
	
	CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();
	if(pFrm)
	{
		CARcBridgeDataStd *pStd	= m_pARcBridgeDataStd;

		if(pFrm->m_ComboUnit.GetSafeHwnd() && IsWindow(pFrm->m_ComboUnit.m_hWnd))
		{
			pFrm->m_ComboUnit.SetCurSel(pStd->m_nTypeUnit);
			pStd->m_pBasicConcInfo->SetUnit(pStd->m_nTypeUnit);
		}
	}

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CARcBridgeDoc serialization

void CARcBridgeDoc::Serialize(CArchive& ar)
{
	// 4 : pFrm->m_paneShortcutDrawing.m_wndTreeDrawing
	// 4 : #27449 LockID, LockSerial
	UINT nFlag = 4;

	CString szSerial,vStr,sInfo = _T("");
	CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();

	if (ar.IsStoring())
	{
		ar << nFlag;
		ar << m_nActiveIDD;
		ar << m_nCurBriNoOfSettingDomyun;
		m_pARcBridgeDataStd->Serialize(ar);
		m_pARcBridgeInputStd->Serialize(ar);
		m_pARcBridgeDrawingOptionStd->Serialize(ar);
		m_pARcBridgeDrawingStd->Serialize(ar);
		m_pARcBridgeOutBMStd->Serialize(ar);	// �ø����߰�
		m_pARoadOptionStd->Serialize(ar);
		ar << szSerial;
		pFrm->m_paneShortcutDrawing.m_wndTreeDrawing.Serialize(ar);

		if(aGetLockSerial() != _T("0097"))  //�ѱ��� �������� ����
		{
			m_sFileSaveInfo = GetStringFileSaveInfo();
		}
 		ar << m_sFileSaveInfo;
 		ar << vStr;
	}
	else
	{
		m_bFileOpen = TRUE;
		ar >> nFlag;
		ar >> m_nActiveIDD;
		if(nFlag > 0)
			ar >> m_nCurBriNoOfSettingDomyun;
		m_pARcBridgeDataStd->Serialize(ar);
		m_pARcBridgeInputStd->Serialize(ar);
		m_pARcBridgeDrawingOptionStd->Serialize(ar);
		m_pARcBridgeDrawingStd->Serialize(ar);
		m_pARcBridgeOutBMStd->Serialize(ar);	// �ø����߰�
		if(nFlag > 1)
			m_pARoadOptionStd->Serialize(ar);
		AfxGetMainWnd()->SendMessage(WM_USER+8, -1);
		if(nFlag > 2)
			ar >> szSerial;
		if(nFlag > 3)
			pFrm->m_paneShortcutDrawing.m_wndTreeDrawing.Serialize(ar);

		if(nFlag > 3)//#27449
		{
 			ar >> m_sFileSaveInfo;
 			ar >> vStr;
		}

		AfxGetMainWnd()->SendMessage(WM_USER+1234, (WPARAM)m_pARcBridgeDataStd->GetLineApp());	
	}		
}

CString CARcBridgeDoc::GetStringFileSaveInfo()
{
	CARcBridgeApp *pApp = (CARcBridgeApp*)AfxGetApp();

	CString sCompany = pApp->GetProfileString(_T("Settings"), _T("UserCompany"));
	CString sUserName = pApp->GetProfileString(_T("Settings"), _T("UserName"));

	CString sSeri = NOTRANS("");
	if (LDK::aIsLogIn())
		sSeri = LDK::aLookLockSerial(TRUE);
	else
		sSeri = aGetLockSerial();

	CString sBuildVersion = pApp->GetProfileString(_T("Settings"), _T("BuildVersion"));

	CString szReturn;
	szReturn.Format(_T("[%s, %s, %s, %s, %s]"), sCompany, sUserName, pApp->GetStringLockInfo(), sSeri, sBuildVersion);

	return szReturn;
}


/////////////////////////////////////////////////////////////////////////////
// CARcBridgeDoc diagnostics

#ifdef _DEBUG
void CARcBridgeDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CARcBridgeDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CARcBridgeDoc commands

void CARcBridgeDoc::DeleteContents() 
{
	ClearMember();
	CreateMember();
	
	CDocument::DeleteContents();
}

void CARcBridgeDoc::CreateMember()
{
	m_pARcBridgeDataStd	= new CARcBridgeDataStd;
	m_pARoadOptionStd	= new CARoadOptionStd(DRW_APIER);
	m_pARcBridgeInputStd = new CARcBridgeInputStd;
	m_pARcBridgeDrawingStd = new CARcBridgeDrawingStd;
	m_pARcBridgeDrawingOptionStd = new CARcBridgeDrawingOptionStd;
	m_pARcBridgeOutBMStd = new CARcBridgeOutBMStd(m_pARcBridgeDataStd);
	m_pARcBridgeOutDesignStd = new CARcBridgeOutDesignStd;
	m_pShareWithADManagerStd = new CShareWidthADManagerStd;
	m_pShareBridgeInfoStd	= new CShareBridgeInfoStd;

	m_pUbManager = new CStmUbManager;

	m_pARcBridgeDataStd->m_pDoc	= this;			
	m_pARcBridgeDataStd->SetVersionFlag(LDK::aGetModuleNumber());

	m_pARcBridgeInputStd->m_pDoc = this;
	m_pARcBridgeInputStd->m_pARcBridgeDataStd = m_pARcBridgeDataStd;
	m_pARcBridgeInputStd->m_pARoadOptionStd = m_pARoadOptionStd;

	m_pARcBridgeDrawingStd->m_pDoc = this;
	m_pARcBridgeDrawingStd->m_pOptStd = m_pARoadOptionStd;
	m_pARcBridgeDrawingStd->m_pARcBridgeDataStd	= m_pARcBridgeDataStd;
	m_pARcBridgeDrawingStd->m_pARcBridgeDrawingOptionStd = m_pARcBridgeDrawingOptionStd;
	m_pARcBridgeDrawingStd->m_pARoadOptionStd = m_pARoadOptionStd;
	m_pARcBridgeDrawingOptionStd->m_pDoc = this;

	m_pARcBridgeDrawingStd->GetDogak()->m_pNameBridge = &m_pARcBridgeDataStd->m_strTempName;
	m_pARcBridgeDrawingStd->GetDogak()->m_pNameProject = &m_pARcBridgeDataStd->m_strProjectName;	
	m_pARcBridgeDrawingStd->m_pDoc = this;

	m_pARcBridgeOutBMStd->m_pARcBridgeDataStd	= m_pARcBridgeDataStd;
	m_pARcBridgeOutBMStd->m_pARoadOptionStd		= m_pARoadOptionStd;
	m_pARcBridgeOutBMStd->m_pDoc = this;

	m_pARcBridgeOutDesignStd->m_pARcBridgeDataStd = m_pARcBridgeDataStd;
	m_pARcBridgeOutDesignStd->m_pARoadOptionStd = m_pARoadOptionStd;
	m_pARcBridgeOutDesignStd->m_pDoc = this;

	m_pShareWithADManagerStd->m_pARoadOptionStd	= m_pARoadOptionStd;
	m_pShareWithADManagerStd->m_pDrawStd	= (CDrawStd*)m_pARcBridgeDrawingStd;
	
	
	CHgBaseDrawStd::m_pARoadOptionStd = m_pARoadOptionStd;

	CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();
	//pFrm->m_paneShortcutDrawing.m_wndTreeDrawing.m_pStd	= m_pARcBridgeDrawingStd;

	m_pARoadOptionStd->SetSupportSimpleMarking(TRUE);
}

void CARcBridgeDoc::ClearMember()
{
	SAFE_DELETE(m_pARoadOptionStd);
	SAFE_DELETE(m_pARcBridgeInputStd);
	SAFE_DELETE(m_pARcBridgeDataStd);
	SAFE_DELETE(m_pARcBridgeDrawingStd);
	SAFE_DELETE(m_pARcBridgeDrawingOptionStd);
	SAFE_DELETE(m_pARcBridgeOutBMStd);
	SAFE_DELETE(m_pARcBridgeOutDesignStd);
	SAFE_DELETE(m_pShareWithADManagerStd);
	SAFE_DELETE(m_pShareBridgeInfoStd);

	SAFE_DELETE(m_pUbManager);
}

BOOL CARcBridgeDoc::DoSave(LPCTSTR pszPathName, BOOL bReplace)
{
	// ���̺��� /////////////////////////
	MakeBackup(pszPathName, FALSE);
	///////////////////////////////////////

	CString newName = pszPathName;
	BOOL bModified = IsModified();

	BOOL bSaveAs = FALSE;
	if (newName.IsEmpty())
		bSaveAs = TRUE;

	if (bSaveAs)
	{
		newName = m_strPathName;
		if (bReplace && newName.IsEmpty())
		{
			newName = (m_strTitle=="���� ����") ? ("�������") : m_strTitle;
			int iBad = newName.FindOneOf(_T("#%;/\\"));    // dubious filename
			if (iBad != -1)
				newName.ReleaseBuffer(iBad);

			// append the default suffix if there is one
			newName += ".arb";
		}
		CFileDialog dlg(FALSE, _T("arb"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("ARcBridge ���� (.arb)|*.arb||"));
		dlg.m_ofn.lpstrFile = newName.GetBuffer(_MAX_PATH);
		if (dlg.DoModal() != IDOK)
			return FALSE;
		CString szPathName = dlg.GetPathName();
		newName = szPathName;
	}

	BeginWaitCursor();
	if (!OnSaveDocument(newName))
	{
		if (pszPathName == NULL)
		{
			// be sure to delete the file
			try
			{
				CFile::Remove(newName);
			}
 			catch(...)
			{
				TRACE0("Warning: failed to delete file after failed SaveAs\n");
			}			
		}		
		EndWaitCursor();
		return FALSE;
	}
	EndWaitCursor();

	if(bReplace)
	{
		SetPathName(newName, TRUE);
		ASSERT(m_strPathName == newName);       // must be set
	}
	else
	{
		SetModifiedFlag(bModified);
	}

	CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();
	pFrm->m_paneShortcutStart.InitMRUList();

	pFrm->CreateSaveCompleteDlg();

	return TRUE;
}

BOOL CARcBridgeDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if(!LDK::aCheckLicenseHGKey()) 
		return 0;

	// ���¹�� /////////////////////////
	MakeBackup(lpszPathName, TRUE);
	///////////////////////////////////////

	// m_strCurFileOpenPathName ���� ���� �������ο� ������� ����ǰ� ���� //// 
	m_strCurFileOpenPathName	= lpszPathName;
	////////////////////////////////////////////////////////////////////////////

	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	CARcBridgeApp *pApp = (CARcBridgeApp*)AfxGetApp();
	pApp->m_strFileOpenPathName	= lpszPathName;
	

	CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();
	CARcBridgeDataStd *pStd	= m_pARcBridgeDataStd;

	// ������ ������ ����
	pFrm->m_ComboUnit.SetCurSel(pStd->m_nTypeUnit);
	pStd->m_pBasicConcInfo->SetUnit(pStd->m_nTypeUnit);

	// ���� ���� �Ŀ� plus������ �ƴϸ� plus����Ÿ�� ���� ��� ������ ����
	// ��¥�� Ÿ�̸ӷ� üũ�ϸ鼭 �����Ƿ� �ϴ� plus�� ����̶� ����� �� �ֵ��� �޽�����
	// ���ΰ� �׳� ����������.
	if(!CheckLockSupportBridgeType())
	{
		CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();
		OnNewDocument();
		pFrm->m_paneShortcutInput.InitTreeState();
		pFrm->m_paneShortcutDrawing.InitTree();

		return FALSE;
	}

	pFrm->InitBMTree();

	pFrm->UpdateLockInfo();

	return TRUE;
}

// ���������� Ÿ���� ������ üũ 
// �ƴ϶�� �޽����� ����ش�.
BOOL CARcBridgeDoc::CheckLockSupportBridgeType()
{
	CStringArray sArrTypeName;
	sArrTypeName.Add("���� ��౳");
	sArrTypeName.Add("���� ��౳");
	sArrTypeName.Add("V�� ��౳");
	sArrTypeName.Add("��米����౳");
	sArrTypeName.Add("�����걳");
	sArrTypeName.Add("��ġ��");
	sArrTypeName.Add("BOX�� ��౳");
	sArrTypeName.Add("��米����౳2");


	BOOL bOK	= TRUE;
	for(long i = 0; i < m_pARcBridgeDataStd->GetBridgeSize(); i++)
	{
		CRcBridgeData *pBri	= m_pARcBridgeDataStd->GetBridge(i);

		if(!m_pARcBridgeDataStd->IsSupportTypeStd(pBri->m_nTypeBridge))
		{
			bOK	= FALSE;
			CString sMes	= _T("");
			sMes.Format("%s Ÿ���� �������� �ʴ� Lock�Դϴ�. Lock�� ��ü �Ͻñ� �ٶ��ϴ�", sArrTypeName.GetAt(pBri->m_nTypeBridge));
			AfxMessageBox(sMes);
		}
	}
	return bOK;
}
void CARcBridgeDoc::ExportBridgeInfo()
{
	if(!m_pARcBridgeDataStd) return;
	CARcBridgeDataStd *pDataStd	= m_pARcBridgeDataStd;

	// �������� ������ ������ �ϳ��̻� �־�� ������ ���� �ʴ´�.
	BOOL bOK	= FALSE;
	BOOL bLsd = pDataStd->IsLsdDesign();

	// ���� ���� ����
	// ���� �ϳ��� Bridge 1��, seg 1��
	// �����걳, ���������� ���� ù��° ������ ���ؼ��� �������� ������.
	// ������, ���1Ÿ���� ���� ���� ������� �ܰ氣���� ���� ������
	CRcBridgeApp *pBriRC[2]	= {NULL, NULL};
	for(long bri = 0; bri < pDataStd->GetBridgeSize(); bri++)
	{
		pBriRC[0]	= pDataStd->GetBridge(bri);
		if(!pBriRC[0]) continue;
		pBriRC[1]	= pDataStd->GetBridge(pDataStd->GetIdxMatchBridge(bri));
		


		for(long updn = 0; updn < 2; updn++)
		{
			CRcBridgeApp *pBri	= pBriRC[updn];
			if(!pBri) continue;

			
			if(pBri->m_nTypeBridge != BRIDGE_TYPE_SLAB && pBri->m_nTypeBridge != BRIDGE_TYPE_PI && pBri->m_nTypeBridge != BRIDGE_TYPE_TRAP) continue;
			BOOL bMake1Span	= pBri->m_nTypeBridge != BRIDGE_TYPE_SLAB;
			bOK	= TRUE;

			m_pShareBridgeInfoStd->InitShareBridgeInfoStd(updn);

			// ������Ʈ ���� ////////////
			m_pShareBridgeInfoStd->SetProjectName(pDataStd->m_strProjectName);
			m_pShareBridgeInfoStd->SetOwnerName(pDataStd->m_strOwnerName);
			m_pShareBridgeInfoStd->SetConstructorName(pDataStd->m_strConstructorName);
			m_pShareBridgeInfoStd->SetSpotName(pDataStd->m_strSpotName);
			m_pShareBridgeInfoStd->SetEngName(pDataStd->m_strEngName);
			m_pShareBridgeInfoStd->SetLineDirName(pDataStd->m_strLineDirName);
			m_pShareBridgeInfoStd->SetLineRevDirName(pDataStd->m_strLineRevDirName);
			////////////////////////////

			// ������ �и�
			m_pShareBridgeInfoStd->SetLineCount(pBriRC[0] && pBriRC[1] ? 1 : 0);

			long nQtyJijum	= bMake1Span ? 2 : pBri->GetCountJijum();
			long nQtyJigan	= bMake1Span ? 1 : pBri->m_nQtyJigan;
			
			CConvertCommonBridge *pConvertCommonBridge	= m_pShareBridgeInfoStd->m_pConvertCommonBridge[updn];
			m_pShareBridgeInfoStd->m_bIsCommonBridge[updn]	= TRUE;
			pConvertCommonBridge->SetLineInfoApp(pBri->m_pLineApp);
			pConvertCommonBridge->SetLineBase(pBri->m_nLineBase);
			pConvertCommonBridge->SetStaBridge(pBri->m_dStationBridgeStt);
			pConvertCommonBridge->SetLenBridge(pBri->m_dLengthBridge);
			pConvertCommonBridge->SetTypeUpDown(pBri->m_nSeparBri == 1 || pBri->m_nSeparBri == 2 ? pBri->m_nSeparBri : 0);
			pConvertCommonBridge->InitJijumData(nQtyJijum);
			pConvertCommonBridge->InitSegData(1);
			pConvertCommonBridge->InitSpanData(0, nQtyJigan);
			
			long nJ = 0; for(nJ = 0; nJ < pConvertCommonBridge->m_pBri->m_nQtyJigan; nJ++)
			{
				long nJRcBridge	= bMake1Span ? (nJ == 0 ? 0 : pBri->GetCountJijum()-1) : nJ;
				long nJRcBridgeNext	= bMake1Span ? pBri->GetCountJijum()-1 : nJ;

				CJijum *pJ	= pBri->GetJijum(nJRcBridge);
				if(pJ)
					pConvertCommonBridge->InitBeamData(0, nJ, pJ->m_nQtyShoe[nJ == 0 ? 1 : 0], BRIDGE_SECTION_RCSLAB);
			}
			
			// jijum
			for(nJ = 0; nJ <= pConvertCommonBridge->m_pBri->m_nQtyJigan; nJ++)
			{
				CConvertJijumData *pConvertJijumData	= pConvertCommonBridge->GetConvertJijumData(nJ);
				long nJRcBridge	= bMake1Span ? (nJ == 0 ? 0 : pBri->GetCountJijum()-1) : nJ;
				long nJRcBridgeNext	= bMake1Span ? (nJ == 0 ? pBri->GetCountJijum()-1 : pBri->GetCountJijum()) : nJ + 1;

				CJijum *pJ	= pBri->GetJijum(nJRcBridge);
				CJijum *pJNext	= pBri->GetJijum(nJRcBridgeNext);
				if(!pConvertJijumData) continue;
				if(!pJ) continue;

				long nVerPos  = (nJ == 0) ? JIJUM_VPOS_END : JIJUM_VPOS_STT;
				// (AAB-6810) ���� ��ȣ�� ����Ҷ� pBri�� nJRcBridge��  pConvertCommonBridge�� nJ�� ����ؼ� �����͸� �����. �߸� ����ϰ� �ִ� �κ��� �־� ����.

				// �Ŵ��� ������ 1��, 1���� �Ŵ��� ��� ���� ���� ����.(Ȯ��������)
				if(1)//pBri->IsExtLine())
				{
					pConvertJijumData->SetQtyGirder(1, 1);
					pConvertJijumData->SetQtyBearing(pJ->m_nQtyShoe[0], pJ->m_nQtyShoe[1]);
					long nCountShoe	= max(pJ->m_nQtyShoe[0], pJ->m_nQtyShoe[1]);
					pConvertJijumData->SetDisGirderFrLine(0, 0, 0);
					pConvertJijumData->SetQtyBearingOfGirder(0, pJ->m_nQtyShoe[0], pJ->m_nQtyShoe[1]);
					for(long shoe = 0; shoe < nCountShoe; shoe++)
					{
						pConvertJijumData->SetXyBearing(0, shoe, pBri->GetXyShoe(nJRcBridge, 0, shoe), pBri->GetXyShoe(nJRcBridge, 1, shoe));
						pConvertJijumData->SetDistBearing(0, shoe, pJ->m_dDistShoe[0][shoe], pJ->m_dDistShoe[1][shoe]);
					}
					pConvertJijumData->m_pJ->m_nNumLineSlabLeft[nVerPos]	= pBri->m_nLineLeft;
					pConvertJijumData->m_pJ->m_nNumLineSlabRight[nVerPos]	= pBri->m_nLineRight;
					pConvertJijumData->m_pJ->m_nNumLineInfo[nVerPos][0]		= pBri->m_nLineBase;
					for(long g = 0; g < MAX_GIRDER_COUNT; g++)
						pConvertJijumData->m_pJ->m_dSlabThickUserAdd[nVerPos][g]	= pBri->GetHeightHunchByJijum(nJRcBridge);
					pConvertCommonBridge->m_pBri->m_bApllyUserThickShoeSeat	= TRUE;
				}
				else
				{
					pConvertJijumData->SetQtyGirder(pJ->m_nQtyShoe[0], pJ->m_nQtyShoe[1]);
					pConvertJijumData->SetQtyBearing(pJ->m_nQtyShoe[0], pJ->m_nQtyShoe[1]);
					long nCountShoe	= max(pJ->m_nQtyShoe[0], pJ->m_nQtyShoe[1]);
					for(long shoe = 0; shoe < nCountShoe; shoe++)
					{
						pConvertJijumData->SetDisGirderFrLine(shoe, pJ->m_dDistShoe[0][shoe], pJ->m_dDistShoe[1][shoe]);
						pConvertJijumData->SetXyBearing(0, shoe, pBri->GetXyShoe(nJRcBridge, 0, shoe), pBri->GetXyShoe(nJRcBridge, 1, shoe));
						pConvertJijumData->SetQtyBearingOfGirder(shoe, 1, 1);

						if(shoe == 0)
							pConvertJijumData->m_pJ->m_nNumLineSlabLeft[nVerPos]	= pBri->m_nLineLeft;
						else if(shoe == nCountShoe-1)
							pConvertJijumData->m_pJ->m_nNumLineSlabRight[nVerPos]	= pBri->m_nLineRight;

						pConvertJijumData->m_pJ->m_nNumLineInfo[nVerPos][shoe]	= pBri->m_nLineBase;
					}
				}

				// ���  
				for(long nVPos = 0; nVPos < 2; nVPos++)
				{
					for(long bearing = 0; bearing < pJ->m_nQtyShoe[nVPos]; bearing++)
					{
						CBearingData *pBearing	= pConvertJijumData->m_pJ->GetBearing(nVPos, bearing);
						if(!pBearing) continue;
						pBearing->m_nDir	= pJ->m_nDirShoe[nVPos][bearing];
						pBearing->m_LoadHori[0] = GetValueUnitChange(pBri->GetShoeHoriForce(nJRcBridge, bearing), pBri->m_pARcBridgeDataStd->m_szTonf, UNIT_CGS_TONF);
					}
				}

				pConvertJijumData->SetNameJijum(pJ->m_strJijumShoe[nJ == 0 ? 1 : 0]);
				pConvertJijumData->SetAngleJijum(pBri->GetAngleJijum(nJRcBridge).GetAngleDegree());
				pConvertJijumData->SetdLenUgan(pJ->m_dLengthUgan[0], pJ->m_dLengthUgan[1]);
				pConvertJijumData->SetTypeGirder(BRIDGE_SECTION_RCSLAB, BRIDGE_SECTION_RCSLAB);
				pConvertJijumData->SetLenSlabToGirder(0, 0);
				pConvertJijumData->SetLenGirderToShoe(pJ->m_dLengthSlabToShoe[0], pJ->m_dLengthSlabToShoe[1]);
				pConvertJijumData->SetStationJijum(pBri->GetStationOnJijum(nJRcBridge));
				if(nJ < pConvertCommonBridge->m_pBri->m_nQtyJigan && pJNext)
				{
					pConvertJijumData->SetLenShoeToShoeUseOnlyInput(pBri->GetStationOnJijum(nJRcBridgeNext) - pBri->GetStationOnJijum(nJRcBridge) - (pJ->m_dLengthUgan[1] + pJNext->m_dLengthUgan[0]) - (pJ->m_dLengthSlabToShoe[1] + pJNext->m_dLengthSlabToShoe[0]));
				}
				else
					pConvertJijumData->SetLenShoeToShoeUseOnlyInput(0);
				if(nJ == 0)
				{
					pConvertJijumData->SetTypeAbutPier(!pBri->IsOutWall(TRUE) ? TYPE_ABUT : TYPE_PIER);
					pConvertJijumData->SetTypeConnect(2);
				}
				else if(nJ == pConvertCommonBridge->m_pBri->m_nQtyJigan)
				{
					pConvertJijumData->SetTypeAbutPier(!pBri->IsOutWall(FALSE) ? TYPE_ABUT : TYPE_PIER);
					pConvertJijumData->SetTypeConnect(3);
				}
				else
				{
					CWallApp *pWall	= pBri->GetInWall(nJRcBridge-1);
					if(pWall)
						pConvertJijumData->SetTypeAbutPier(!pWall->m_bIs ? TYPE_PIER : TYPE_ABUT);
					else
						pConvertJijumData->SetTypeAbutPier(TYPE_PIER);
					pConvertJijumData->SetTypeConnect(4);
				}
				

				pConvertJijumData->SetDisSlabLeftFrLine(pBri->m_dWidthSlabLeft, pBri->m_dWidthSlabLeft);
				pConvertJijumData->SetDistSlabRightFrLine(pBri->m_dWidthSlabRight, pBri->m_dWidthSlabRight);
			}

			// seg
			CConvertSegData *pConvertSegData	= pConvertCommonBridge->GetConvertSegData(0);
			if(pConvertSegData)
			{
				pConvertSegData->SetBri(bri+updn);
				pConvertSegData->SetJijumStt(0);
				pConvertSegData->SetJijumEnd(nQtyJigan);
				pConvertSegData->SetNameSeg("ARcBridge");
				pConvertSegData->SetTypeGirder(BRIDGE_SECTION_RCSLAB);
			}


			
			CSegData *pSeg	= pConvertCommonBridge->m_pBri->GetSegData(0);
			if(pSeg)
			{
				// span
				for(long span = 0; span < pSeg->GetSizeSpan(); span++)
				{
					CConvertSpanData *pConvertSpanData	= pConvertCommonBridge->GetConvertSpanData(0, span);
					if(!pConvertSpanData) continue;
					CSpanData *pSpan	= pSeg->GetSpan(span);
					if(!pSpan) continue;

					// beam �ǰ��(rc������ beam������ �Է¾ȹ����Ƿ�)
					for(long beam = 0; beam < pSpan->GetSizeBeam(); beam++)
					{
						CBeamData *pBeam	= pSpan->GetBeamData(beam);
						if(!pBeam) continue;
						pBeam->m_pBeamMid->SetDataDefault(0);
						pBeam->m_pBeamEnd->SetDataDefault(1);
						pBeam->SetDataDefault();
					}
				}

				// hDan
				CConvertHDanData *pConvertHDanData	= pConvertCommonBridge->GetConvertHDanData(0);
				if(pConvertHDanData)
				{
					pConvertHDanData->SetQtyHDan(pBri->GetQtyHDan());
					pConvertHDanData->SetQtyRail(pBri->m_nQtyRail);
					pConvertHDanData->SetDisBaseElevation(pBri->m_dDisBaseElevation);
					pConvertHDanData->SetDisBaseElevationRL(pBri->m_dDisBaseElevationRL);
					pConvertHDanData->SetHeightDosangUnderTie(pBri->m_dHeightDosangUnderTie);
					pConvertHDanData->SetHeightRail(pBri->m_dHeightRail);
					pConvertHDanData->SetLevelFrRoadCenterToEdge(pBri->m_bLevelFrRoadCenterToEdge);
					pConvertHDanData->SetSlopeDosang(pBri->GetSlopeDosang());
					pConvertHDanData->SetThickPavement(pBri->GetThickPave());
					pConvertHDanData->SetThickSlabStd(pBri->m_dTS);

					CHDanData *pHDan	= pSeg->m_pHDan;
					long d = 0; for(d = 0; d < pHDan->m_nQtyHDan; d++)
					{
						pConvertHDanData->SetLengthHDan(d, pBri->GetLengthHDan(d));
						pConvertHDanData->SetLevelHDan(d, pBri->IsLevelHDanApplyLineSlope(d));
						pConvertHDanData->SetSlopeHDan(d, pBri->GetSlopeHDan(d));
						pConvertHDanData->SetTypeHDan(d, pBri->GetTypeHDan(d));
						pConvertHDanData->SetVariableLenHDan(d, pBri->IsVariableLenHDan(d, pBri->m_bUnderGroundRahmen));
					}

					for(d = 0; d < pHDan->m_nQtyRail; d++)
					{
						pConvertHDanData->SetLengthRail(d, pBri->GetLengthRailByLine(d));
						pConvertHDanData->SetLineNumber(d, pBri->m_nLineNumber[d]);
					}

					// ��ȣ��
					for(d = 0; d < pHDan->m_nQtyHDan; d++)
					{
						CGuardWallRC *pGWRC	= pBri->GetGuardWallByHDan(d);
						CGuardWall *pGW	= pConvertCommonBridge->m_pBri->GetGuardWallByHDan(0, d);
						
						pGW->m_dH1	= pGWRC->m_dH1;
						pGW->m_dH2	= pGWRC->m_dH2;
						pGW->m_dH3	= pGWRC->m_dH3;
						pGW->m_dW1	= pGWRC->m_dW1;
						pGW->m_dW2	= pGWRC->m_dW2;
						pGW->m_dW3	= pGWRC->m_dW3;
						pGW->m_dW4	= pGWRC->m_dW4;
						pGW->m_dW5	= pGWRC->m_dW5;
						pGW->m_dW6	= pGWRC->m_dW6;
						pGW->m_dW7	= pGWRC->m_dW7;
						pGW->m_dR1	= pGWRC->m_dR1;
						pGW->m_dR2	= pGWRC->m_dR2;
						pGW->m_dR3	= pGWRC->m_dR3;
						pGW->m_dC	= pGWRC->m_dC;
						pGW->m_dForce	= pGWRC->m_dForce;
						pGW->m_bLeftGuardFence	= pGWRC->m_bLeftGuardFence;
						pGW->m_bGWhalf	= pGWRC->m_bGWhalf;
						pGW->m_bSoundProof	= pGWRC->m_bSoundProof;
					}
				}
			}

			// ���¸� �ݷ�(3D �ݷ� ����)
			if(pBri->m_FemNormal.IsOutputLoaded() || pBri->m_FemShell.IsOutputLoaded())
			{
				CARcBridgeCalcStd CalcStd(static_cast<CRcBridgeRebar*>(pBri));
				double SlabWidth = bLsd? frM(CalcStd.GetSlabWidth()) : CalcStd.GetSlabWidth();

				long nJ = 0; for(nJ = 0; nJ < nQtyJijum; nJ++)
				{
					long nJRcBridge	= bMake1Span ? (nJ == 0 ? 0 : pBri->GetCountJijum()-1) : nJ;
					long nJRcBridgeNext	= bMake1Span ? pBri->GetCountJijum()-1 : nJ;

					CJijum *pJ	= pBri->GetJijum(nJRcBridge);
					if(!pJ) continue;

					

					long nV	= nJ == 0 ? 1 : 0;
					long nQtyShoe	= pJ->m_nQtyShoe[nV];
					for(long shoe = 0; shoe < nQtyShoe; shoe++)
					{
						BOOL b2DFrame		= pBri->IsShoeReaction2DApply(nJRcBridge, shoe);
						double dLength		= b2DFrame ? SlabWidth / (double)nQtyShoe : 1.00;

						double dLoadDead(0), dLoadDeadDW(0),dLoadLive1(0), dSubsid(0), dV1(0), dV2(0), dV3(0);
						BOOL bOdd			= nQtyShoe % 2 == 1;	// Ȧ������?
						long nAdd			= bOdd ? 1 : 0;

						if(bLsd)
						{
							dLoadDead	= pBri->GetShoeReactionLsd(nJRcBridge, shoe, eLSD_LOADCASE_DC1, b2DFrame, TRUE) * dLength;
							dLoadDead	+= pBri->GetShoeReactionLsd(nJRcBridge, shoe, eLSD_LOADCASE_DW1, b2DFrame, TRUE) * dLength;
							dLoadLive1	= pBri->GetShoeReactionLsd(nJRcBridge, shoe, eLSD_LOADCASE_LL1, b2DFrame, TRUE) * dLength;

							dV1    = pBri->GetShoeReactionLsd(nJRcBridge,shoe,eLSD_LOADCASE_TU21 ,b2DFrame,TRUE) * dLength;
							dV2    = pBri->GetShoeReactionLsd(nJRcBridge,shoe,eLSD_LOADCASE_TU22 ,b2DFrame,TRUE) * dLength;
							dV3    = pBri->GetShoeReactionLsd(nJRcBridge,shoe,eLSD_LOADCASE_SH,b2DFrame,TRUE) * dLength;

							dSubsid		= pBri->GetShoeReactionLsd(nJRcBridge,shoe,eLSD_LOADCASE_SD,b2DFrame,TRUE) * dLength;
						}
						else
						{
							dLoadDead	= pBri->GetShoeReactionUsd(nJRcBridge, shoe, BRIDGE_LOAD_DEAD, b2DFrame, TRUE) * dLength;
							dLoadLive1	= pBri->GetShoeReactionUsd(nJRcBridge, shoe, BRIDGE_LOAD_LIVE_FULL, b2DFrame, TRUE) * dLength;

							dV1    = pBri->GetShoeReactionUsd(nJRcBridge,shoe,BRIDGE_LOAD_TEMP1 ,b2DFrame,TRUE) * dLength;
							dV2    = pBri->GetShoeReactionUsd(nJRcBridge,shoe,BRIDGE_LOAD_TEMP2 ,b2DFrame,TRUE) * dLength;
							dV3    = pBri->GetShoeReactionUsd(nJRcBridge,shoe,BRIDGE_LOAD_SHRINK,b2DFrame,TRUE) * dLength;

							dSubsid		= pBri->GetShoeReactionUsd(nJRcBridge,shoe,BRIDGE_LOAD_SUBSID,b2DFrame,TRUE) * dLength;
						}
						
						double dLoadLive2	= (nQtyShoe / 2) + nAdd > shoe ? dLoadLive1 : 0;
						double dLoadLive3	= (nQtyShoe / 2) + nAdd <= shoe ? dLoadLive1 : 0;

						double dLoadMax	= dLoadDead + dLoadLive1;
						dLoadMax += max(dV1+dV3,dV2+dV3);
						dLoadMax += dSubsid;

						if(bLsd)
						{
							// N ==> Tonf
							dLoadDead /= 10000;
							dLoadLive1 /= 10000;
							dLoadLive2 /= 10000;
							dLoadLive3 /= 10000;
							dLoadMax /= 10000;
						}
						else
						{
							CFEMManage *pFEM = b2DFrame ? &pBri->m_FemNormal : &pBri->m_FemShell;

							if(pFEM->SYSTEM_VAR.m_szUnitType == "KN")
							{
								dLoadDead /= 10;
								dLoadLive1 /= 10;
								dLoadLive2 /= 10;
								dLoadLive3 /= 10;
								dLoadMax /= 10;
							}
						}

						// (AAB-6388)  �̰� ����? �� ���࿡�� ��������?
						// long nUpDn = 0;
						long nUpDn = updn;
						m_pShareBridgeInfoStd->SetBearingReAction(nJ, nV, shoe, dLoadDead, dLoadDeadDW, dLoadLive1, dLoadLive2, dLoadLive3, 0, 0, dLoadMax, nUpDn);
					}
				}
			}

			if(updn == 1) bri++;
		}

		// BasicConcInfo
		m_pShareBridgeInfoStd->SetBasicConcInfo(pDataStd->m_pBasicConcInfo);

		CString sTitle	= _T("");
		if(pBriRC[0])
			sTitle	= pBriRC[0]->m_strBridgeName;
		if(pBriRC[1])
			sTitle	= sTitle + ", " + pBriRC[1]->m_strBridgeName;

		m_pShareBridgeInfoStd->SetNameSoftWareExport("ARcBridge");
		m_pShareBridgeInfoStd->SetNameBridgeExport(sTitle);

		sTitle	= sTitle + "�� ���� ���� ���� ��������";
		if(!bOK) 
		{
			AfxMessageBox(sTitle + "�� �������� ������ ������ �ƴմϴ�.");
			return;
		}
		m_pShareBridgeInfoStd->Export(sTitle);
	}
}

// ���ü� ��������
void CARcBridgeDoc::SerializeATemp(CArchive &ar)
{
	CARcBridgeDataStd *pStd	= m_pARcBridgeDataStd;
	long nCountBri	= pStd->GetBridgeSize();

	long nFlag	= 0;
	CString sNameProgram	= _T("ARcBridge");
	if(ar.IsStoring())
	{
		// 1. ���α׷���
		ar << sNameProgram;

		ar << nFlag;

		// 2. ������Ʈ ����
		// ������Ʈ ���� ---------------------------------
		ar << pStd->m_strProjectName;		// �����
		ar << pStd->m_strOwnerName;		// ����ó
		ar << pStd->m_strConstructorName;	// �ð���
		ar << pStd->m_strSpotName;		// �����
		ar << pStd->m_strEngName;			// �����
		// --------------------------------------------------

		// 3. ������, 4. ������ Data and 5. ���ݰ�, 6. ��������
		ar << nCountBri;
		for(long bri = 0; bri < nCountBri; bri++)
		{
			CRcBridgeApp *pBri	= pStd->GetBridge(bri);
			if(!pBri) continue;

			long nCountJijum	= pBri->GetCountJijum();
			ar << nCountJijum;
			for(long jijum = 0; jijum < nCountJijum; jijum++)
			{
				SerializeStructInfoForATemp(ar, pBri, jijum);
			}
		}
	}
}

// Atemp�� ������ ���� ����
// 4. ������ Data(��ǥ(3D)) - ���� or mass, ����, ���/��ü
// 5. ���ݰ� - ���ü� ��� EL., ���ϼ���(�Էµ� ���ı� offset ����)
// 6. �������� - �ش� ������ ���� ����
// ��� ��ǥ�� �ð�������� ���ư�
void CARcBridgeDoc::SerializeStructInfoForATemp(CArchive &ar, CRcBridgeApp *pBri, long jijum)
{
	CRebarPlacing rb;

	CFootingApp *pFooting	= pBri->GetFootingByJijumNumber(jijum);
	CExFootingApp *pExFooting	= &pFooting->m_exFooting;
	pFooting->GetTvPlan();
	pFooting->GetTvFront();

	pExFooting->GetTvPlan();
	pExFooting->GetTvFront(FALSE, FALSE, TRUE);
	
	// ������ ����
	CString sStruct	= "ARcBridge";

	// ���� or mass 3D ��ǥ
	CTwinVectorArray tvArrExFooting3D;
	CTwinVectorArray tvArrPlan;
	tvArrPlan	= pExFooting->m_tvArrPlan;
	MakeCWTvArr(tvArrPlan);
	Make3DTvArr(tvArrPlan, pFooting->m_tvArrFront, tvArrExFooting3D);

	// ���� 3D ��ǥ
	CTwinVectorArray tvArrFooting3D;
	tvArrPlan	= pFooting->m_tvArrplan;
	MakeCWTvArr(tvArrPlan);
	Make3DTvArr(tvArrPlan, pFooting->m_tvArrFront, tvArrFooting3D);

	// ���/��ü
	CTwinVectorArray tvArrWall3D;
	CTwinVectorArray tvArrWall;
	CTwinVectorArray tvArrSec;
	CWallApp *pWall	= NULL;
	CWallApp wall;
	if(jijum == 0 || jijum == pBri->m_nQtyJigan)
	{
		pBri->MakeWallByOutWallOnlyShape(wall, jijum==0, TRUE, FALSE);
		pWall	= &wall;
	}
	else
	{
		pWall	= pBri->GetInWall(jijum-1);
	}

	pWall->GetTvPlan();
	tvArrWall	= pWall->m_tvArrPlan;

	//31720
	CTwinVectorArray tvArrVertSection;
	tvArrVertSection.AddFromTvArray(pBri->m_tvArrVertSection);

	pWall->GetTvFront(tvArrVertSection);
	tvArrSec	= pWall->m_tvArrFront;

	MakeCWTvArr(tvArrPlan);
	Make3DTvArr(tvArrWall, tvArrSec, tvArrWall3D);
	

	// ���ü� ��� EL
	CTwinVectorArray tvArrTupagi3D;
	tvArrTupagi3D	= pExFooting->m_tvArrPlan;
	MakeCWTvArr(tvArrTupagi3D);
	rb.GetOffsetTvArray(tvArrTupagi3D, pFooting->m_dDistOffsetDigging, FALSE);
	CTwinVector *pTv	= NULL;
	for(long i = 0; i < tvArrTupagi3D.GetSize(); i++)
	{
		pTv	= &tvArrTupagi3D.GetAt(i);
		pTv->m_v1.z	= pFooting->m_dEL_DiggingFirst;
		pTv->m_v2.z	= pFooting->m_dEL_DiggingFirst;
	}

	// ���ϼ���
	double dELWater	= pFooting->m_dEL_DiggingWater;

	// ��������
	if(!pFooting->m_pExBoring)
	{
//		double dStation = pBri->GetStationOnJijum(jijum);
//		CBoringDataEx *pBoring	= pBri->GetBoringDataExByStation(dStation);
//		pFooting->m_pExBoring	= pBoring;

		pFooting->m_pExBoring = pBri->GetBoringDataEx(jijum);
	}

	CBoringDataEx boringDataEx;
	if(pFooting->m_pExBoring)
		boringDataEx	= *pFooting->m_pExBoring;

	long nFlag	= 0;

	if(ar.IsStoring())
	{
		ar << nFlag;
		ar << sStruct;
		ar << pBri->m_strBridgeName;
		tvArrExFooting3D.Serialize(ar);
		tvArrFooting3D.Serialize(ar);
		tvArrWall3D.Serialize(ar);
		tvArrTupagi3D.Serialize(ar);

		// ���ϼ���
		ar << dELWater;

		// ���� ����
		boringDataEx.Serialize(ar);
	}
}

// tvArr�� �ð� �������� ���ư����� �Ѵ�.
void CARcBridgeDoc::MakeCWTvArr(CTwinVectorArray &tvArr)
{
	if(tvArr.GetSize() < 2) return;
	tvArr.Sort();

	CTwinVector tv1	= tvArr.GetAt(0);
	CTwinVector tv2	= tvArr.GetAt(1);
	long nDir	= GetDirectionRotateByPoint(tv1.m_v1, tv1.m_v2, tv2.m_v2);

	if(nDir == 1)
	{
		CRebarPlacing rb;
		rb.ReverseRebar(tvArr);
	}
}

// ���, ����, ���� ��ǥ�� 3D��ǥ ���� ����
void CARcBridgeDoc::Make3DTvArr(CTwinVectorArray &tvArrPlan, CTwinVectorArray &tvArrFront, CTwinVectorArray &tvArr3D)
{
	tvArr3D.RemoveAll();

	CDRect rect	= tvArrFront.GetRect();

	for(long i = 0; i < tvArrPlan.GetSize(); i++)
	{
		CTwinVector *pTvL	= &tvArrPlan.GetAt(i);
		CTwinVector tvU	= *pTvL;
		pTvL->m_v1.z	= rect.bottom;
		pTvL->m_v2.z	= rect.bottom;
		tvU.m_v1.z	= rect.top;
		tvU.m_v2.z	= rect.top;

		tvArr3D.Add(tvU);
	}

	tvArrPlan.AddFromTvArray(tvArr3D);
	tvArr3D.RemoveAll();
	tvArr3D.AddFromTvArray(tvArrPlan);
}

// ������� �����
// 1. ���¹�� : �����Ҷ� ����(���������� ������ ���� copy) ���ϸ�.apr.bak1
// 2. ���̺��� : �����Ҷ� ����(�����ϱ� ���� copy) ���ϸ�.apr.bak2
// bOpen : TRUE - ���¹��, FALSE - ���̺���
BOOL CARcBridgeDoc::MakeBackup(CString sPathName, BOOL bOpen)
{
	BOOL bResult	= FALSE;
	if(sPathName == "") return bResult;

	CString sPathNameBackup	= sPathName;
	if(bOpen)
	{
		sPathNameBackup += ".bak1";
	}
	else
	{
		sPathNameBackup += ".bak2";
	}

	// ���Ϻ���
	CFileFind ff;
	if(ff.FindFile(sPathName))
	{
		//31652
		CEnvRegist reg;
		BOOL bBackUpByDate = reg.LoadEnvInt(CEnvRegist::MAKE_BACKUPFILE_DATE);

		if(bBackUpByDate && bOpen)
			bResult = ((CMSDIWinApp*)AfxGetApp())->MakeBackUpFileByDate((CDocument*)(this), sPathName, FALSE);
		else
			bResult	= CopyFile(sPathName, sPathNameBackup, FALSE);
	}

	return bResult;
}

void CARcBridgeDoc::ShowDlgHDanLength()
{
	m_pARcBridgeInputStd->ShowDlgHDanLength();
}

CStmUbManager * CARcBridgeDoc::GetUbManager()
{
	return m_pUbManager;
}
