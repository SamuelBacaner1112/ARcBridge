// DialogMenuOutBM.cpp : implementation file
//

#include "stdafx.h"
#include "ARcBridge.h"
#include "MainFrm.h"
#include "ARcBridgeDoc.h"
#include "../src/ARcBridgeOutBM/ARcBridgeOutBM.h"
#include "DialogMenuOutBM.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogMenuOutBM dialog


CDialogMenuOutBM::CDialogMenuOutBM(CWnd* pParent /*=NULL*/)
	: CXTResizeDialog(CDialogMenuOutBM::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogMenuOutBM)
	//}}AFX_DATA_INIT

	m_colorBK	= RGB(253, 250, 255);
	m_nTabBMNormal = 0;
	m_bPushCtrlKey = FALSE;
}


void CDialogMenuOutBM::DoDataExchange(CDataExchange* pDX)
{
	CXTResizeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogMenuOutBM)
	DDX_Control(pDX, IDC_TAB_BM, m_ctlTab);
	DDX_Control(pDX, IDC_BUTTON_BM, m_btnBM);
	DDX_Control(pDX, IDC_TREE_BM, m_ctlTreeBM);
	DDX_Control(pDX, IDC_BUTTON_SELECT_REV, m_btnSelectRev);
	DDX_Control(pDX, IDC_BUTTON_SELECT_NONE, m_btnSelectNone);
	DDX_Control(pDX, IDC_BUTTON_SELECT_BMSUM, m_btnSelectBMSum);
	DDX_Control(pDX, IDC_BUTTON_SELECT_BM, m_btnSelectBM);
	DDX_Control(pDX, IDC_BUTTON_SELECT_ALL, m_btnSelectAll);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogMenuOutBM, CXTResizeDialog)
	//{{AFX_MSG_MAP(CDialogMenuOutBM)
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BUTTON_BM, OnButtonBm)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_ALL, OnButtonSelectAll)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_BM, OnButtonSelectBm)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_BMSUM, OnButtonSelectBmsum)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_NONE, OnButtonSelectNone)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_REV, OnButtonSelectRev)
	ON_NOTIFY(NM_CLICK, IDC_TREE_BM, OnClickTreeBm)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_BM, OnSelchangedTreeBm)
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_BM, OnSelchangeTabBm)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogMenuOutBM message handlers


BOOL CDialogMenuOutBM::OnInitDialog() 
{
	CXTResizeDialog::OnInitDialog();
	
	SetResize(IDC_TAB_BM, SZ_TOP_LEFT, SZ_TOP_RIGHT);
	SetResize(IDC_BUTTON_SELECT_ALL, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_SELECT_NONE, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_SELECT_REV, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_SELECT_BM, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_SELECT_BMSUM, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_TREE_BM, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);

	m_nTabBMNormal = 0;
	UpdateData(FALSE);

	InitTree();

	m_ctlTab.RemoveAllTabs();
	m_ctlTab.SendInitialUpdate(TRUE);

	m_ctlTab.ModifyStyle(0L, WS_CLIPCHILDREN | WS_CLIPSIBLINGS);

	m_ctlTab.InsertItem(0, hggettext("일반"));
	m_ctlTab.InsertItem(1, hggettext("공통"));
	m_ctlTab.InsertItem(2, hggettext("토공"));
	m_ctlTab.InsertItem(3, hggettext("철근"));
	m_ctlTab.SetCurSel(m_nTabBMNormal);

	return TRUE;
}

void CDialogMenuOutBM::OnDestroy()
{
	m_ctlTreeBM.DeleteAllItems();

	CXTResizeDialog::OnDestroy();
}

void CDialogMenuOutBM::OnButtonBm()
{
	CARcBridgeDoc* pDoc = (CARcBridgeDoc*)((CMainFrame*)AfxGetMainWnd())->GetActiveDocument();
	CARcBridgeOutBMStd *pBMStd = pDoc->m_pARcBridgeOutBMStd;
	if(pBMStd == NULL)
		return;

	UpdateData(TRUE);
	pBMStd->MakeBMRcBridgeArray();

	// TreeInfo가 있어야 사용자 수량이 적용가능하다. 먼저 만들어주고 다시 한번 적용해 준다.
	pBMStd->m_pOutBMDialog->InitBMTreeInfo(FALSE);
	if(pBMStd->m_pOutBMDialog->CreateBM())
	{
		pBMStd->m_pOutBMDialog->InitBMTreeInfo();

		UpdateData(FALSE);
		InitTree();
	}
}

void CDialogMenuOutBM::RunOnButtonBmByStruct()
{
	OnButtonBm();

	OnButtonSelectNone();
}

void CDialogMenuOutBM::InitTree()
{
	CARcBridgeDoc* pDoc = (CARcBridgeDoc*)((CMainFrame*)AfxGetMainWnd())->GetActiveDocument();
	CARcBridgeOutBMStd *pBMStd = pDoc->m_pARcBridgeOutBMStd;
	if(pBMStd == NULL)
		return;

	m_bInitingTree = TRUE;
	m_ctlTreeBM.SetBkColor(m_colorBK);
	m_ctlTreeBM.DeleteAllItems();

	pBMStd->m_pOutBMDialog->SetTreeCtrl(&m_ctlTreeBM);
	pBMStd->m_pOutBMDialog->InitTree(&m_ctlTreeBM, m_nTabBMNormal); 

	m_ctlTreeBM.SetItemHeight(18);
	m_bInitingTree = FALSE;
}

BOOL CDialogMenuOutBM::OnEraseBkgnd(CDC* pDC) 
{
	CRect rcBackground;
	GetClientRect(rcBackground);
	pDC->FillSolidRect(rcBackground, m_colorBK);
	return TRUE;
}

void CDialogMenuOutBM::OnButtonSelectAll()
{
	CARcBridgeDoc* pDoc = (CARcBridgeDoc*)((CMainFrame*)AfxGetMainWnd())->GetActiveDocument();
	CARcBridgeOutBMStd *pBMStd = pDoc->m_pARcBridgeOutBMStd;
	if(pBMStd == NULL)
		return;

	pBMStd->m_pOutBMDialog->SetCheck(0, m_nTabBMNormal);
	InitTree();
}

void CDialogMenuOutBM::OnButtonSelectNone()
{
	CARcBridgeDoc* pDoc = (CARcBridgeDoc*)((CMainFrame*)AfxGetMainWnd())->GetActiveDocument();
	CARcBridgeOutBMStd *pBMStd = pDoc->m_pARcBridgeOutBMStd;
	if(pBMStd == NULL)
		return;

	pBMStd->m_pOutBMDialog->SetCheck(1, m_nTabBMNormal);
	InitTree();
}

void CDialogMenuOutBM::OnButtonSelectRev()
{
	CARcBridgeDoc* pDoc = (CARcBridgeDoc*)((CMainFrame*)AfxGetMainWnd())->GetActiveDocument();
	CARcBridgeOutBMStd *pBMStd = pDoc->m_pARcBridgeOutBMStd;
	if(pBMStd == NULL)
		return;

	pBMStd->m_pOutBMDialog->SetCheck(2, m_nTabBMNormal);
	InitTree();
}

void CDialogMenuOutBM::OnButtonSelectBm()
{
	CARcBridgeDoc* pDoc = (CARcBridgeDoc*)((CMainFrame*)AfxGetMainWnd())->GetActiveDocument();
	CARcBridgeOutBMStd *pBMStd = pDoc->m_pARcBridgeOutBMStd;
	if(pBMStd == NULL)
		return;

	pBMStd->m_pOutBMDialog->SetCheck(3, m_nTabBMNormal);
	InitTree();
}

void CDialogMenuOutBM::OnButtonSelectBmsum()
{
	CARcBridgeDoc* pDoc = (CARcBridgeDoc*)((CMainFrame*)AfxGetMainWnd())->GetActiveDocument();
	CARcBridgeOutBMStd *pBMStd = pDoc->m_pARcBridgeOutBMStd;
	if(pBMStd == NULL)
		return;

	pBMStd->m_pOutBMDialog->SetCheck(4, m_nTabBMNormal);
	InitTree();
}

void CDialogMenuOutBM::OnClickTreeBm(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here

	// 클릭한 위치가 체크박스 위치 인지 판단/////////
	SetCheckTreeItem(TVHT_ONITEMSTATEICON);

	*pResult = 0;
}

void CDialogMenuOutBM::OnSelchangedTreeBm(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if(m_bInitingTree) return;

	CARcBridgeDoc* pDoc = (CARcBridgeDoc*)((CMainFrame*)AfxGetMainWnd())->GetActiveDocument();
	CARcBridgeOutBMStd *pBMStd = pDoc->m_pARcBridgeOutBMStd;
	if(pBMStd == NULL)
		return;

	HTREEITEM hItem	= m_ctlTreeBM.GetSelectedItem();
	if(!hItem) return;

	void *p	= (void*)m_ctlTreeBM.GetItemData(hItem);
	pBMStd->m_pOutBMDialog->SetDataInit(p, m_bPushCtrlKey);
	
	//AfxGetMainWnd()->SendMessage(WM_USER+34);

	*pResult = 0;
}

BOOL CDialogMenuOutBM::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_CONTROL)
		m_bPushCtrlKey	= TRUE;
	else if(pMsg->message == WM_KEYUP && pMsg->wParam == VK_CONTROL)
		m_bPushCtrlKey	= FALSE;
		
	return CXTResizeDialog::PreTranslateMessage(pMsg);
}

HBRUSH CDialogMenuOutBM::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CXTResizeDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO: Change any attributes of the DC here
	pDC->SetBkMode(TRANSPARENT);

	// TODO: Return a different brush if the default is not desired
	return (HBRUSH)::GetStockObject(NULL_BRUSH);
	return hbr;
}

void CDialogMenuOutBM::OnSelchangeTabBm(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Add your control notification handler code here
	m_nTabBMNormal	= m_ctlTab.GetCurSel();
	UpdateData(TRUE);
	InitTree();	

	return;
}

void CDialogMenuOutBM::SetSelectBMTab(long nTab)
{
	m_ctlTab.SetCurSel(nTab);
	m_nTabBMNormal = m_ctlTab.GetCurSel();
	UpdateData(TRUE);
	InitTree();
}

void CDialogMenuOutBM::SetCheckTreeItem( UINT nHitTestInfo )
{
	// 클릭한 위치가 체크박스 위치 인지 판단/////////
	CPoint point;
	::GetCursorPos(&point);
	m_ctlTreeBM.ScreenToClient(&point);
	UINT nHitFlags = 0;
	HTREEITEM hItem = m_ctlTreeBM.HitTest(point, &nHitFlags);
	if (hItem == NULL) return;
	if (nHitFlags & nHitTestInfo)
	{
		CARcBridgeDoc* pDoc = (CARcBridgeDoc*)((CMainFrame*)AfxGetMainWnd())->GetActiveDocument();
		CARcBridgeOutBMStd *pBMStd = pDoc->m_pARcBridgeOutBMStd;
		if(pBMStd == NULL)
			return;

		// 
		pBMStd->m_pOutBMDialog->SetCheck(&m_ctlTreeBM, hItem, !m_ctlTreeBM.GetCheck(hItem));
	}
	//////////////////////////////////////////////////
}
