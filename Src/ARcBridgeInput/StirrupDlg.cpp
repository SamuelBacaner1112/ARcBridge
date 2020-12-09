// StirrupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "arcbridgeinput.h"
#include "StirrupDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStirrupDlg dialog


CStirrupDlg::CStirrupDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStirrupDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStirrupDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pParent	= NULL;

	m_pGirderRB = NULL;
	m_pBri = NULL;
	m_nJ = -1;
}

CStirrupDlg::CStirrupDlg(CRcBridgeRebar *pBri, long nJ, CInputDialogBase* pParent)
	: CDialog(CStirrupDlg::IDD, pParent)
{
	if(pBri)
	{
		if(pBri->IsGirderRebar(nJ, TRUE))
		{
			m_pGirderRB		= &pBri->m_SlabGirderRebar[iUPPER][nJ];
			m_nJ			= nJ;
			m_pBri			= pBri;
			m_pParent		= pParent;
		}
		else
		{
			m_nJ			= -1;
			m_pBri			= NULL;
			m_pGirderRB		= NULL;
			m_pParent		= NULL;
		}
		
	}
	else
	{
		m_nJ			= -1;
		m_pBri			= NULL;
		m_pGirderRB		= NULL;
		m_pParent		= NULL;
	}
	
}

void CStirrupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStirrupDlg)
	DDX_Control(pDX, IDC_COMBO_STIRRUP, m_comboStirrup);
	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStirrupDlg, CDialog)
	//{{AFX_MSG_MAP(CStirrupDlg)
	ON_CBN_SELCHANGE(IDC_COMBO_STIRRUP, OnSelchangeComboStirrup)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED_DATA, IDC_GRID, OnCellChangedData)
	ON_NOTIFY(GVN_CELL_CHANGED, IDC_GRID, OnCellChanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStirrupDlg message handlers

void CStirrupDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CStirrupDlg::OnOK() 
{
	// TODO: Add extra validation here
	SetDataSave();
	
	CDialog::OnOK();
}

BOOL CStirrupDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_Grid.InitGrid();

	InitCombo();
	SetGridData();
	SetDataInit();
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CStirrupDlg::SetGridData()
{
	// 그리드 초기화
	m_Grid.SetGridDefault(2, 2, 1, 0);
	m_Grid.SetCellSize(20, 50);

	if(!m_pGirderRB) return;
	
	long row = 0, col = 0;
	CString str = _T(""), sHelp = _T("");
	CStringArray sArr;

	for(long strup = 0; strup < m_pGirderRB->m_nCountStirrup; strup++)
	{
		str.Format("A%d", strup+1);
		m_Grid.AddCell(row, col, str);
		m_Grid.AddCell(row+1, col++, &m_pGirderRB->m_xyDistStirrup[strup].x);

		str.Format("B%d", strup+1);
		m_Grid.AddCell(row, col, str);
		m_Grid.AddCell(row+1, col++, &m_pGirderRB->m_xyDistStirrup[strup].y);
	}
}

void CStirrupDlg::SetDataInit()
{
	m_Grid.UpdateData(FALSE);

	// 스트럽 설정
	if(m_pGirderRB)
	{
		long nIdx	= m_pGirderRB->m_nCountStirrup-1;
		if(nIdx > -1)
			m_comboStirrup.SetCurSel(nIdx);
	}
}

void CStirrupDlg::SetDataSave()
{
	m_Grid.UpdateData(TRUE);
}

void CStirrupDlg::InitCombo()
{
	CString str	= _T("");
	for(long strup = 0; strup < MAX_REBAR_STIRRUP; strup++)
	{
		str.Format("%d개", strup+1);
		m_comboStirrup.AddString(str);
	}
}

void CStirrupDlg::OnCellChanged(NMHDR* pNotifyStruct, LRESULT* result)
{
	structGridCell *pCell = m_Grid.GetCurrentCell();
	if(pCell)
	{
		//DrawInputDomyunView(FALSE);
		
	}
}

void CStirrupDlg::OnSelchangeComboStirrup() 
{
	// TODO: Add your control notification handler code here
	if(m_pGirderRB)
	{
		m_pGirderRB->m_nCountStirrup	= m_comboStirrup.GetCurSel()+1;
		SetDataDefault();
		SetGridData();
		SetDataInit();
	}
}

void CStirrupDlg::OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result)
{
	SetDataSave();
	if(m_pParent)
		m_pParent->DrawInputDomyunView(FALSE);
}


void CStirrupDlg::SetDataDefault()
{
	if(!m_pParent) return;

	if(m_pBri)
	{
		long nJ	= m_nJ;
		if(m_pBri->IsGirderRebar(nJ, TRUE))
		{
			m_pBri->SetDataDefaultRebarMainInGirder_Girder(nJ);
			m_pBri->SetDataDefaultRebarGirder(nJ, TRUE);
			if(m_pBri->IsBoxType())
			{
				m_pBri->SetDataDefaultRebarMainInGirder_Girder(nJ, TRUE, TRUE, TRUE, FALSE, FALSE);
				m_pBri->SetDataDefaultRebarGirder(nJ, TRUE, FALSE, TRUE, TRUE, TRUE, FALSE, FALSE, FALSE);
			}
		}
		m_pParent->DrawInputDomyunView(FALSE);
	}
}
