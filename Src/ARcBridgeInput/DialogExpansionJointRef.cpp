// DialogExpansionJointRef.cpp : implementation file
//

#include "stdafx.h"
#include "ARcBridgeInput.h"
#include "../ARcBridgeData/ARcBridgeData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogExpansionJointRef dialog


CDialogExpansionJointRef::CDialogExpansionJointRef(CExpJointRefArr *pExpJointRefArr, CExpansionJointData* pExpData, CWnd* pParent /*=NULL*/)
	: CXTResizeDialog(CDialogExpansionJointRef::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogExpansionJointRef)
	m_pExpJointRefArr	= pExpJointRefArr;
	m_pExpData			= pExpData;
	//}}AFX_DATA_INIT
}


void CDialogExpansionJointRef::DoDataExchange(CDataExchange* pDX)
{
	CXTResizeDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogExpansionJointRef)
	DDX_Control(pDX, IDC_BUTTON_DEFAULT1, m_btnDefault);
	DDX_Control(pDX, IDC_BUTTON_DEL, m_btnDel);
	DDX_Control(pDX, IDC_BUTTON_USER_ADD, m_btnAdd);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogExpansionJointRef, CXTResizeDialog)
	//{{AFX_MSG_MAP(CDialogExpansionJointRef)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_USER_ADD, OnButtonUserAdd)
	ON_BN_CLICKED(IDC_BUTTON_DEL, OnButtonDel)
	ON_BN_CLICKED(IDC_BUTTON_DEFAULT1, OnButtonDefault)
	//}}AFX_MSG_MAP
	ON_NOTIFY(GVN_CELL_CHANGED_ROW, IDC_GRID, OnCellChangedRow)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogExpansionJointRef message handlers

BOOL CDialogExpansionJointRef::OnInitDialog() 
{
	CXTResizeDialog::OnInitDialog();
	
	SetResize(IDOK,					SZ_BOTTOM_RIGHT,	SZ_BOTTOM_RIGHT);
	SetResize(IDCANCEL,				SZ_BOTTOM_RIGHT,	SZ_BOTTOM_RIGHT);
	SetResize(IDC_BUTTON_USER_ADD,	SZ_BOTTOM_LEFT,		SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_DEL,		SZ_BOTTOM_LEFT,		SZ_BOTTOM_LEFT);
	SetResize(IDC_GRID,				SZ_TOP_LEFT,		SZ_BOTTOM_RIGHT);

	m_Grid.InitGrid();

	SetGridData();
	MarkIndexSelect();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogExpansionJointRef::OnCellChangedRow(NMHDR* pNotifyStruct, LRESULT* result)
{
	NM_GRIDVIEW* pGridView = (NM_GRIDVIEW*)pNotifyStruct;	
	
	for(long nRow=2; nRow<m_Grid.GetRowCount(); nRow++)
	{
		for (long nCol=1; nCol<m_Grid.GetColumnCount(); nCol++)
		{
			m_Grid.SetItemBkColour(nRow, nCol, RGB_WHITE);
		}
	}

	for(long nCol=1; nCol<m_Grid.GetColumnCount(); nCol++)
	{
		m_Grid.SetItemBkColour(pGridView->iRow, nCol, RGB(0,255,0));
	}

	m_Grid.SetRedraw(TRUE, TRUE);
}


void CDialogExpansionJointRef::SetGridData()
{
	m_Grid.SetGridDefault(4, 7, 2, 1);

	m_Grid.AddCell(0, 0, "모델명\n");
	m_Grid.AddCell(1, 0, "모델명\n");
	m_Grid.AddCell(0, 1, "최대허용\n신축량");
	m_Grid.AddCell(1, 1, "최대허용\n신축량");
	m_Grid.AddCell(0, 2, "BLOCK OUT");
	m_Grid.AddCell(1, 2, "폭");
	m_Grid.AddCell(0, 3, "BLOCK OUT");
	m_Grid.AddCell(1, 3, "높이");
	m_Grid.AddCell(0, 4, "유간");
	m_Grid.AddCell(1, 4, "최소");
	m_Grid.AddCell(0, 5, "유간");
	m_Grid.AddCell(1, 5, "평균");
	m_Grid.AddCell(0, 6, "유간");
	m_Grid.AddCell(1, 6, "최대");

	CString sText;
	for (long nExp=0; nExp<m_pExpJointRefArr->GetSize(); nExp++)
	{
		CExpJointRef *pExpJointRef = m_pExpJointRefArr->GetExpJointRef(nExp);

		m_Grid.AddCell(nExp+2, 0, &pExpJointRef->m_szName);
		m_Grid.AddCell(nExp+2, 1, &pExpJointRef->m_dMaxAllowExpL); 
		m_Grid.AddCell(nExp+2, 2, &pExpJointRef->m_dBlockW); 
		m_Grid.AddCell(nExp+2, 3, &pExpJointRef->m_dBlockH); 
		m_Grid.AddCell(nExp+2, 4, &pExpJointRef->m_dUganMin);
		m_Grid.AddCell(nExp+2, 6, &pExpJointRef->m_dUganMax);
	}

	m_Grid.UpdateData(FALSE);
	m_Grid.SetColumnWidthAll(80);
	m_Grid.SetColumnWidth(0, 100);

	m_Grid.MergeGrid(0, 1, 0, m_Grid.GetColumnCount());

	m_Grid.SetRedraw(TRUE, TRUE);
}

// pBearing에 Data를 넣는다.
void CDialogExpansionJointRef::OnOK() 
{
	m_Grid.UpdateData(TRUE);

	SetExpJointRef();
	CXTResizeDialog::OnOK();
}

void CDialogExpansionJointRef::SetExpJointRef()
{
	long nSection				= m_pExpData->m_nPosition;
	long nExp					= max(m_Grid.GetFocusCell().row-2, 0);
	CExpJointRef *pExpJointRef	= m_pExpJointRefArr->GetExpJointRef(nExp);

	m_pExpData->m_strExpanJoint	= pExpJointRef->m_szName;
	m_pExpData->m_dMaxAllowExp	= pExpJointRef->m_dMaxAllowExpL;
	m_pExpData->m_dMaxLayingGap	= pExpJointRef->m_dUganMax;
	m_pExpData->m_dMinLayingGap	= pExpJointRef->m_dUganMin;

	if(nSection==BOTHSIDE_SECTION || nSection==PRE_SECTION)
	{
 		*m_pExpData->m_pBlockW[PRE_SECTION]		= pExpJointRef->m_dBlockW; 
 		*m_pExpData->m_pBlockH[PRE_SECTION]		= pExpJointRef->m_dBlockH;
	}
	if(nSection==BOTHSIDE_SECTION || nSection==POST_SECTION)
	{
 		*m_pExpData->m_pBlockW[POST_SECTION]	= pExpJointRef->m_dBlockW; 
 		*m_pExpData->m_pBlockH[POST_SECTION]	= pExpJointRef->m_dBlockH;
	}	
}

void CDialogExpansionJointRef::MarkIndexSelect()
{
	long nRowMark	= 0;
	for(long nx=0; nx<m_pExpJointRefArr->GetSize(); ++nx)
	{
		CExpJointRef *pExpJointRef	= m_pExpJointRefArr->GetExpJointRef(nx);
		if(m_pExpData->m_strExpanJoint != pExpJointRef->m_szName)
			continue;
		nRowMark = nx;
		break;
	}

	for(long nCol=1; nCol<m_Grid.GetColumnCount(); nCol++)
	{
		m_Grid.SetItemBkColour(nRowMark+2, nCol, RGB(0,255,0));
	}
	m_Grid.SetFocusCell(nRowMark+2, 1);
	m_Grid.SetRedraw(TRUE, TRUE);
}

void CDialogExpansionJointRef::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
}

void CDialogExpansionJointRef::OnButtonUserAdd() 
{
	// TODO: Add your control notification handler code here
	
}

void CDialogExpansionJointRef::OnButtonDel() 
{
	// TODO: Add your control notification handler code here
	
}

void CDialogExpansionJointRef::OnButtonDefault() 
{
	m_pExpJointRefArr->SetRefDefault();
	SetGridData();
}
