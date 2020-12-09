// DialogHDanLength.cpp : implementation file
//

#include "stdafx.h"
#include "arcbridgeinput.h"
#include "DialogHDanLength.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogHDanLength dialog


CDialogHDanLength::CDialogHDanLength(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogHDanLength::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogHDanLength)
	m_dAng = 0.0;
	m_dSta = 0.0;
	//}}AFX_DATA_INIT
	m_pIn	= NULL;
}


void CDialogHDanLength::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogHDanLength)
	DDX_Text(pDX, IDC_EDIT_ANG, m_dAng);
	DDX_Text(pDX, IDC_EDIT_STA, m_dSta);
	//}}AFX_DATA_MAP
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
}


BEGIN_MESSAGE_MAP(CDialogHDanLength, CDialog)
	//{{AFX_MSG_MAP(CDialogHDanLength)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, OnButtonApply)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogHDanLength message handlers

BOOL CDialogHDanLength::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogHDanLength::OnButtonApply() 
{
	// TODO: Add your control notification handler code here
	SetGridData();
	SetDataInit();
}


void CDialogHDanLength::SetGridData()
{
	CRcBridgeApp *pBri	= GetRcBridgeApp();
	if(!pBri) return;

	UpdateData(TRUE);


	m_Grid.SetGridDefault(2, 2, 1, 0);

	CString str	= _T("");

	CDPoint vAng	= ToDPointFrDegree(m_dAng);
	CDPoint A[30];
	pBri->GetXySlabUpperAct(m_dSta, vAng, A, FALSE, TRUE);
	long nQtyHDanAct = pBri->GetQtyHDanAct(m_dSta, vAng,FALSE, TRUE);

	long i = 0; for(i = 0; i < nQtyHDanAct; i++)
	{
		CDPoint xy1	= A[i];
		CDPoint xy2	= A[i+1];

		str.Format("L%d", i+1);
		m_Grid.AddCell(0, i, str);

		str.Format("%.3f", xy2.x-xy1.x);
		m_Grid.AddCell(1, i, str);

		str.Format("%.3f", xy2.y-xy1.y);
		m_Grid.AddCell(2, i, str);
	}

	long nRows	= 0;
	long nCols	= 0;
	m_Grid.GetCellCount(nRows, nCols);
	m_Grid.SetRows(nRows);
	m_Grid.SetCols(nCols);
	m_Grid.SetColumnWidthAll(70);
}

void CDialogHDanLength::SetDataInit()
{
	m_Grid.UpdateData(FALSE);
}

void CDialogHDanLength::SetARcStructureInputStd(CARcBridgeInputStd *pIn)
{
	m_pIn	= pIn;
}


CRcBridgeApp *CDialogHDanLength::GetRcBridgeApp()
{
	if(!m_pIn) return NULL;

	return m_pIn->GetRcBridge();
}

void CDialogHDanLength::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here
	SetGridData();
	SetDataInit();
}
