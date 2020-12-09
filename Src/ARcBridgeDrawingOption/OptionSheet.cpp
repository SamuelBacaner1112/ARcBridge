// OptionSheet.cpp : implementation file
//

#include "stdafx.h"
#include "../../ARcBridge/ARcBridgeDoc.h"
#include "../../Src/ARcBridgeData/ARcBridgeData.h"
#include "../ARcBridgeDrawing/ARcBridgeDrawingStd.h"
#include "arcbridgedrawingoption.h"
#include "OptionSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionSheet

IMPLEMENT_DYNAMIC(COptionSheet, CPropertySheet)

COptionSheet::COptionSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	m_bSetOK = TRUE;
}

COptionSheet::COptionSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage, BOOL bSlabStandardOutput)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	m_psh.dwFlags &= ~PSH_HASHELP;
	m_psh.dwFlags |= PSH_NOAPPLYNOW;	// 적용 버튼 없애기

	if(!bSlabStandardOutput)
	{
		AddPage(&m_pageOptionCommon);
		AddPage(&m_pageOptionVerticalPlane);
		AddPage(&m_pageOptionGeneral);
		AddPage(&m_pageOptionRebar);
		AddPage(&m_pageOptionAps);
		AddPage(&m_pageOptionCns);
	//	방호벽 상세도, 교명주받침 상세도 추가...
		AddPage(&m_pageOptionHyunTa);
		AddPage(&m_pageOptionOthers);		// 기타 도면 삭제..
		//AddPage(&m_pageOptionShoeArrange);
		AddPage(&m_pageOptionBoring);
	}
	else
	{
		AddPage(&m_pageOptionCommon);
		AddPage(&m_pageOptionSlabStandardOutput);
	}

	m_bSetOK = FALSE;
}

COptionSheet::~COptionSheet()
{
}

void COptionSheet::DoDataExchange(CDataExchange* pDX) 
{
	DDX_Control(pDX, IDC_OPT_BRIDGE_NO, m_cbBriNo);
	
	CPropertySheet::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(COptionSheet, CPropertySheet)
	//{{AFX_MSG_MAP(COptionSheet)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_OPT_SAVE, OnOptionSave)
	ON_BN_CLICKED(IDC_OPT_LOAD, OnOptionLoad)
	ON_CBN_SELCHANGE(IDC_OPT_BRIDGE_NO, OnSelchangeComboBridgeNo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionSheet message handlers

int COptionSheet::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CPropertySheet::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_pageOptionCommon.m_pDoc			= m_pDoc;
	m_pageOptionVerticalPlane.m_pDoc	= m_pDoc;
	m_pageOptionGeneral.m_pDoc			= m_pDoc;
	m_pageOptionRebar.m_pDoc			= m_pDoc;
	m_pageOptionAps.m_pDoc				= m_pDoc;
	m_pageOptionCns.m_pDoc				= m_pDoc;
	m_pageOptionOthers.m_pDoc			= m_pDoc;
	m_pageOptionShoeArrange.m_pDoc		= m_pDoc;
	m_pageOptionBoring.m_pDoc			= m_pDoc;
	m_pageOptionHyunTa.m_pDoc			= m_pDoc;
	m_pageOptionSlabStandardOutput.m_pDoc	= m_pDoc;

	return 0;
}

BOOL COptionSheet::OnInitDialog() 
{
	BOOL bResult = CPropertySheet::OnInitDialog();

	CARcBridgeDoc* pDoc = dynamic_cast<CARcBridgeDoc*>(m_pDoc);
	CARcBridgeDrawingOptionStd* pOpt = pDoc->m_pARcBridgeDrawingOptionStd;

	m_bSetOK = TRUE;

	CRect rect;
	GetClientRect(&rect);
	HFONT font;
	font = CreateFontSize("굴림", 12);

	m_cbBriNo.Create(WS_CHILD|WS_VISIBLE|CBS_DROPDOWNLIST, CRect(7, rect.bottom-25, 247, rect.bottom+500), this, IDC_OPT_BRIDGE_NO);
	m_cbBriNo.SendMessage(WM_SETFONT, (WPARAM)font, 0);
	InitComboCtrl();

	m_btnSave.Create(_T("저장"), WS_CHILD|WS_VISIBLE, CRect(255, rect.bottom-27, 315, rect.bottom-3), this, IDC_OPT_SAVE);
	m_btnSave.SendMessage(WM_SETFONT, (WPARAM)font, 0);
	m_btnLoad.Create(_T("불러오기"), WS_CHILD|WS_VISIBLE, CRect(317, rect.bottom-27, 377, rect.bottom-3), this, IDC_OPT_LOAD);
	m_btnLoad.SendMessage(WM_SETFONT, (WPARAM)font, 0);
	m_btnDefault.Create(_T("기본값"), WS_CHILD|WS_VISIBLE, CRect(379, rect.bottom-27, 439, rect.bottom-3), this, IDC_OPT_DEFAULT);
	m_btnDefault.SendMessage(WM_SETFONT, (WPARAM)font, 0);

	if(pDoc->m_nCurBriNoOfSettingDomyun< 0 || pDoc->m_nCurBriNoOfSettingDomyun >= m_cbBriNo.GetCount())
		pDoc->m_nCurBriNoOfSettingDomyun	= 0;
	m_cbBriNo.SetCurSel(pDoc->m_nCurBriNoOfSettingDomyun);
		

	return bResult;
}

BOOL COptionSheet::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message==WM_KEYDOWN && (pMsg->wParam==VK_RETURN || pMsg->wParam==VK_ESCAPE))
		return FALSE;

	CARcBridgeDoc* pDoc = dynamic_cast<CARcBridgeDoc*>(m_pDoc);
	CARcBridgeDataStd* pStd = pDoc->m_pARcBridgeDataStd;
	CARcBridgeDrawingOptionStd* pOpt = pDoc->m_pARcBridgeDrawingOptionStd;

	
	if(pOpt->m_nCurPropPage > -1 && pOpt->m_nCurPropPage < GetPageCount())
	{
		CPropertyPage *pPage	= GetPage(pOpt->m_nCurPropPage);
		if(pPage)
		{
		}
	}

	return CPropertySheet::PreTranslateMessage(pMsg);
}

void COptionSheet::InitComboCtrl()
{
	CARcBridgeDoc* pDoc = dynamic_cast<CARcBridgeDoc*>(m_pDoc);
	CARcBridgeDataStd* pStd = pDoc->m_pARcBridgeDataStd;
	CARcBridgeDrawingOptionStd* pOpt = pDoc->m_pARcBridgeDrawingOptionStd;

	int nSize = pStd->GetBridgeSize();

	CString str = _T("");
	for(int i=0; i<nSize; i++)
	{
		CRcBridgeApp* pBri = pStd->GetBridge(i);
		str = pStd->GetStringBridgeOrder(i) + _T(" : ") + pBri->m_strBridgeName
			+ _T(" [") + pBri->GetStringBridgeType() + _T("]");
		m_cbBriNo.AddString(str);
	}
	m_cbBriNo.SetCurSel(0);

	BOOL bShow = pOpt->m_nCurPropPage==0 ? FALSE : TRUE;
	m_cbBriNo.ShowWindow(bShow);
}

void COptionSheet::OnOptionSave()
{
	CARcBridgeDoc* pDoc = dynamic_cast<CARcBridgeDoc*>(m_pDoc);
	CARcBridgeDrawingOptionStd* pOpt = pDoc->m_pARcBridgeDrawingOptionStd;

	CFileDialog dlg(FALSE, _T("aro"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("ARcBridge 도면 옵션 파일 (*.aro)|*.aro||"));
	dlg.m_ofn.lpstrTitle = "도면 옵션 저장";
	if(dlg.DoModal()==IDOK)
	{
		UpdateData();
		CFile file;
		if(!file.Open(dlg.GetPathName(), CFile::modeWrite | CFile::modeCreate)) return;
		CArchive ar(&file, CArchive::store);
		pOpt->Serialize(ar);
		ar.Close();
		file.Close();
	}
}

void COptionSheet::OnOptionLoad()
{
	CARcBridgeDoc* pDoc = dynamic_cast<CARcBridgeDoc*>(m_pDoc);
	CARcBridgeDrawingStd* pStd = pDoc->m_pARcBridgeDrawingStd;
	CARcBridgeDrawingOptionStd* pOpt = pDoc->m_pARcBridgeDrawingOptionStd;

	CFileDialog dlg(TRUE, _T("aro"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("ARcBridge 도면 옵션 파일 (*.aro)|*.aro||"));
	dlg.m_ofn.lpstrTitle = "도면 옵션 불러오기";
	if(dlg.DoModal()==IDOK)
	{
		CFile file;
		if(!file.Open(dlg.GetPathName(), CFile::modeRead)) return;
		CArchive ar(&file, CArchive::load);
		pStd->GetDogak()->Initialize(TRUE, pOpt->m_strDgkName, pOpt->m_strDomNumHead);				
		pOpt->Serialize(ar);
		ar.Close();
		file.Close();
		pStd->GetDrawPageMng()->SetPageModifyAll();
		pStd->SetModifiedAll();
		EndDialog(0);
	}
}

void COptionSheet::OnSelchangeComboBridgeNo() 
{
	CARcBridgeDoc* pDoc = dynamic_cast<CARcBridgeDoc*>(m_pDoc);
	CARcBridgeDataStd* pStd = pDoc->m_pARcBridgeDataStd;
	CARcBridgeDrawingOptionStd* pOpt = pDoc->m_pARcBridgeDrawingOptionStd;	
	
	pDoc->m_nCurBriNoOfSettingDomyun	= m_cbBriNo.GetCurSel();
	
	for(long n=0; n<GetPageCount(); n++)
	{
		if(GetPage(n) && GetPage(n)->GetSafeHwnd())
		{	
			(dynamic_cast<COptionBase *>(GetPage(n)))->SetDataSave();
			(dynamic_cast<COptionBase *>(GetPage(n)))->m_nCurBri = pDoc->m_nCurBriNoOfSettingDomyun;
			(dynamic_cast<COptionBase *>(GetPage(n)))->m_Tree.DeleteAllItems();			
			(dynamic_cast<COptionBase *>(GetPage(n)))->InitGrid();
			(dynamic_cast<COptionBase *>(GetPage(n)))->InitTree();
		}
	}
	
}
