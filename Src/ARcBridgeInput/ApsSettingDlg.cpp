// ApsSettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "arcbridgeinput.h"
#include "ApsSettingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CApsSettingDlg dialog


CApsSettingDlg::CApsSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CApsSettingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CApsSettingDlg)
	//}}AFX_DATA_INIT
	m_pAps	= NULL;
}

CApsSettingDlg::CApsSettingDlg(CApsApp *pAps, CWnd* pParent /*=NULL*/)
	: CDialog(CApsSettingDlg::IDD, pParent)
{
	m_pAps = pAps;
}


void CApsSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CApsSettingDlg)
	DDX_Control(pDX, IDC_COMBO_NUM, m_comboNum);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_RECOMMOND, m_btnRecommand);
	DDX_GridControl(pDX, IDC_GRID, m_Grid);
	DDX_GridControl(pDX, IDC_GRID_SHEAR, m_GridShear);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CApsSettingDlg, CDialog)
	//{{AFX_MSG_MAP(CApsSettingDlg)
	ON_BN_CLICKED(IDC_RECOMMOND, OnRecommond)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CApsSettingDlg message handlers

BOOL CApsSettingDlg::OnInitDialog() 
{
	if(!m_pAps) return FALSE;
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	CString str = _T("");
	for(long wrap = 0; wrap < MAX_REBAR_WRAP; wrap++)
	{
		str.Format("%d", wrap+1);
		m_comboNum.AddString(str);
	}

	m_Grid.InitGrid(m_pAps->m_dFy);
	m_GridShear.InitGrid(m_pAps->m_dFyShear);
	SetGridData();
	SetDataInit();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CApsSettingDlg::OnRecommond() 
{
	// TODO: Add your control notification handler code here
	if(!m_pAps) return;
	m_pAps->SetDataDefaultCover();
	m_pAps->SetDataDefaultShearWarp();
	
	SetDataInit();
}

void CApsSettingDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CApsSettingDlg::OnOK() 
{
	// TODO: Add extra validation here
	SetDataSave();
	
	if(m_pAps->m_rebarShearDetail.m_nType==TYPE_SHEAR_SINGLE_HOOK)
	{
		AfxMessageBox(hggettext("한가닥스트럽 선택시 하면피복이 150mm 이상 적용되어야 합니다."), MB_ICONINFORMATION);
	}

	CDialog::OnOK();
}

void CApsSettingDlg::SetDataInit()
{
	if(!m_pAps) return;

	if(m_pAps->m_nCountWrapMainRebar > -1 && m_pAps->m_nCountWrapMainRebar < MAX_REBAR_WRAP)
		m_comboNum.SetCurSel(m_pAps->m_nCountWrapMainRebar-1);

	UpdateData(FALSE);

	m_Grid.UpdateData(FALSE);
	m_GridShear.UpdateData(FALSE);
	m_Grid.SetRedraw(TRUE, TRUE);
	m_GridShear.SetRedraw(TRUE, TRUE);
}

void CApsSettingDlg::SetDataSave()
{
	if(!m_pAps) return;
	UpdateData(TRUE);

	m_pAps->m_nCountWrapMainRebar	= m_comboNum.GetCurSel()+1;
	m_Grid.UpdateData(TRUE);
	m_GridShear.UpdateData(TRUE);
}

void CApsSettingDlg::SetGridData()
{
	// 그리드 초기화
	m_Grid.SetGridDefault(2, 2, 0, 1);
	m_Grid.SetCellSize(20, -1);

	if(!m_pAps) return;

	

	CDPointArray xyArr;	// 배경색 반전위치
	CStringArray sArr;
	CString str = _T("");
	CString strHelp = _T("");
	long row = 0;
	long col = 0;
	UINT nFormat = DT_CENTER | DT_VCENTER;
	UINT nLine = CELL_LINE_NO;

	m_Grid.AddCell(row, col, "상면"); m_Grid.AddCell(row++, col+1, &m_pAps->m_dCoverUpper);
	m_Grid.AddCell(row, col, "하면"); m_Grid.AddCell(row++, col+1, &m_pAps->m_dCoverLower);
	m_Grid.AddCell(row, col, "측면"); m_Grid.AddCell(row++, col+1, &m_pAps->m_dCoverSide);
	
	m_Grid.GetCellCount(row, col);
	m_Grid.SetRows(row);
	m_Grid.SetCols(col);
	m_Grid.SetColumnWidthAll(50);

	// 전단철근 설정
	CRebarShearDetail *pDetail	= &m_pAps->m_rebarShearDetail;

	row	= 0;
	col	= 0;

	m_GridShear.SetGridDefault(2, 2, 1, 0);
	m_GridShear.SetCellSize(20, -1);

	CHgBaseConcStd baseConc;
	sArr.RemoveAll();
	long i = 0; for(i = 0; i < MAX_REBAR_CTC; i++)
	{
		str.Format("%.0f", baseConc.GetValueRebarCTC(i));
		sArr.Add(str);
	}

	m_GridShear.AddCell(row, col, "CTC");
	m_GridShear.AddCell(row+1, col++, &m_pAps->m_dCtcShear, nFormat, nLine, "", CELL_TYPE_COMBO, sArr);


	m_GridShear.AddCell(row, col, "직경");
	m_GridShear.AddCell(row+1, col++, &pDetail->m_dDia, nFormat, nLine, "", CELL_TYPE_DIA);


	sArr.RemoveAll();
	for(i = 0; i < TYPE_SHEAR_SU; i++)
		sArr.Add(pDetail->GetStringType(i));
	m_GridShear.AddCell(row, col, "타입");
	m_GridShear.AddCell(row+1, col++, &pDetail->m_nType, nFormat, TRUE, nLine, "", CELL_TYPE_COMBO, sArr);
}


BOOL CApsSettingDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	m_Grid.ViewGridCellForCombo(pMsg);
	m_GridShear.ViewGridCellForCombo(pMsg);
	
	return CDialog::PreTranslateMessage(pMsg);
}
