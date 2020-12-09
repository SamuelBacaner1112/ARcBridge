// DlgOcc3D.cpp : implementation file
//

#include "stdafx.h"
#include "arcbridge.h"
#include "../Src/ARcBridgeData/ARcBridgeData.h"
#include "UbiARcBridgeData.h"
#include "DlgOcc3D.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgOcc3D dialog


CDlgOcc3D::CDlgOcc3D(CWnd* pParent /*=NULL*/)
	: CXTResizeDialog(CDlgOcc3D::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgOcc3D)
	m_nRadioKind3d = -1;
	//}}AFX_DATA_INIT
	m_pDlgOccView	= NULL;
	m_pUbiARcBridgeData	= NULL;
	m_pARcBridgeDataStd	= NULL;
}


void CDlgOcc3D::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgOcc3D)
	DDX_Control(pDX, IDC_DLGOCC_COMBO_REBAR_CREATEKIND, m_comRebarCreateKind);
	DDX_Control(pDX, IDC_DLGOCC_CHECK_REBAR, m_ctlRebarOption);
	DDX_Control(pDX, IDC_DLGOCC_BUTTON_3D, m_btnDraw3d);
	DDX_Control(pDX, IDC_DLGOCC_COMBO_RC, m_comRcBridge);
	DDX_Radio(pDX, IDC_DLGOCC_RADIO_ALL, m_nRadioKind3d);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgOcc3D, CXTResizeDialog)
	//{{AFX_MSG_MAP(CDlgOcc3D)
	ON_BN_CLICKED(IDC_DLGOCC_BUTTON_3D, OnDlgoccButtonDraw3d)
	ON_CBN_SELCHANGE(IDC_DLGOCC_COMBO_RC, OnSelchangeDlgoccComboRc)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_DLGOCC_RADIO_SELECT, OnDlgoccRadioSelectRc)
	ON_BN_CLICKED(IDC_DLGOCC_RADIO_ALL, OnDlgoccRadioAllRc)
	ON_WM_CANCELMODE()
	ON_CBN_SELCHANGE(IDC_DLGOCC_COMBO_REBAR_CREATEKIND, OnSelchangeComboRebarCreatekind)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgOcc3D message handlers

void CDlgOcc3D::OnDlgoccButtonDraw3d() 
{
	// TODO: Add your control notification handler code here
	if(m_pDlgOccView	== NULL)	return;
	if(m_pUbiARcBridgeData	== NULL)	return;
	if(m_pARcBridgeDataStd	== NULL)	return;
	if(m_pARcBridgeDataStd->GetBridgeSize()	== 0)	return;

	if(m_pUbiARcBridgeData->m_nDrawKind	== -1)	m_pUbiARcBridgeData->m_nDrawKind = 0;
	m_pUbiARcBridgeData->m_nDrawRebar	= m_ctlRebarOption.GetCheck();
	m_pUbiARcBridgeData->m_nDrawBridge	= m_comRcBridge.GetCurSel();
	m_pDlgOccView->Draw3D();
}

void CDlgOcc3D::OnSelchangeDlgoccComboRc() 
{
	// TODO: Add your control notification handler code here
	
}

void CDlgOcc3D::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CXTResizeDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	if(bShow==TRUE)
	{
		CRect rcWnd, rcDlg;
		GetParent()->GetWindowRect(&rcWnd);
		GetWindowRect(&rcDlg);
		this->SetWindowPos(NULL, rcWnd.right-rcDlg.Width()-7 ,rcWnd.bottom-rcDlg.Height()-30,rcDlg.Width(), rcDlg.Height(),SW_SHOW);

		InitControl();
	}
}

void CDlgOcc3D::OnDlgoccRadioAllRc() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_pUbiARcBridgeData->m_nDrawKind = 0;
}

void CDlgOcc3D::OnDlgoccRadioSelectRc() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_pUbiARcBridgeData->m_nDrawKind = 1;
}

void CDlgOcc3D::InitControl()
{
	m_ctlRebarOption.SetCheck(FALSE);
	m_nRadioKind3d	= 0;
	InitComboBridge();

	long i=0;
	long nCount	= m_comRebarCreateKind.GetCount();
	for(i=0;i<nCount;i++)
		m_comRebarCreateKind.DeleteString(0);

	m_comRebarCreateKind.AddString(_T("라인"));
	m_comRebarCreateKind.AddString(_T("실린더"));
//	m_comRebarCreateKind.AddString(_T("솔리드"));
	m_comRebarCreateKind.SetCurSel(0);

	UpdateData(FALSE);
}

void CDlgOcc3D::InitComboBridge(long nBri)
{
	long nCnt	= m_comRcBridge.GetCount();
	long i=0;
	for(i=0; i<nCnt; i++)
		m_comRcBridge.DeleteString(0);

	if(m_pARcBridgeDataStd==NULL)	return;
	CString str = _T("");
	for(i=0; i<m_pARcBridgeDataStd->GetBridgeSize(); i++)
	{
		CRcBridgeApp* pBri = m_pARcBridgeDataStd->GetBridge(i);
		str = m_pARcBridgeDataStd->GetStringBridgeOrder(i) + _T(" : ") + pBri->m_strBridgeName
			+ _T(" [") + pBri->GetStringBridgeType() + _T("]");
		m_comRcBridge.AddString(str);
	}
	m_comRcBridge.SetCurSel(0);
}

BOOL CDlgOcc3D::OnInitDialog() 
{
	CXTResizeDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetResize(IDC_DLGOCC_STATIC_3D, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_DLGOCC_STATIC_OPTION, SZ_TOP_LEFT, SZ_BOTTOM_RIGHT);

//	InitControl();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgOcc3D::OnCancelMode() 
{
	CXTResizeDialog::OnCancelMode();
	
	// TODO: Add your message handler code here
}

void CDlgOcc3D::OnSelchangeComboRebarCreatekind() 
{
	// TODO: Add your control notification handler code here
	//m_pDlgOccView->m_pOccView->m_pManage->m_nRebaCreateKind = m_comRebarCreateKind.GetCurSel() + 1;
}
