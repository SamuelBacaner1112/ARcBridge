// OptimizeRebar.cpp : implementation file
//

#include "stdafx.h"
#include "arcbridgeinput.h"
#include "OptimizeRebar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptimizeRebar dialog


COptimizeRebar::COptimizeRebar(CRcBridgeData *pBri, CWnd* pParent /*=NULL*/)
	: CDialog(COptimizeRebar::IDD, pParent)
{
	//{{AFX_DATA_INIT(COptimizeRebar)
	m_bCheckRebar2Dan = FALSE;
	m_bCheckSpliteCycle = FALSE;
	m_szSafeRate = _T("");
	m_bCheckRebarMatch = FALSE;
	//}}AFX_DATA_INIT

	m_pBri = pBri;
}


void COptimizeRebar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptimizeRebar)
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_COMBO_DIA_MIN, m_cbDiaMin);
	DDX_Control(pDX, IDC_COMBO_DIA_MAX, m_cbDiaMax);
	DDX_Check(pDX, IDC_CHECK_REBAR_2DAN, m_bCheckRebar2Dan);
	DDX_Check(pDX, IDC_CHECK_SPLITE_CYCLE, m_bCheckSpliteCycle);
	DDX_Text(pDX, IDC_EDIT_SAFE_RATE, m_szSafeRate);
	DDX_Check(pDX, IDC_CHECK_REBAR_MATCH, m_bCheckRebarMatch);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptimizeRebar, CDialog)
	//{{AFX_MSG_MAP(COptimizeRebar)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptimizeRebar message handlers

BOOL COptimizeRebar::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here


	m_szSafeRate.Format("%.0f",m_pBri->m_dOptimizeSafeRate);
	m_bCheckRebar2Dan	= m_pBri->m_bOptimizeRebar2Dan;
	m_bCheckSpliteCycle	= m_pBri->m_bOptimizeRebarSplit;
	m_bCheckRebarMatch  = m_pBri->m_bOptimizeRebarMatch;

	m_strArr.RemoveAll();
	m_strArr.Add(m_pBri->m_pARcBridgeDataStd->GetStringDia(10));
	m_strArr.Add(m_pBri->m_pARcBridgeDataStd->GetStringDia(13));
	m_strArr.Add(m_pBri->m_pARcBridgeDataStd->GetStringDia(16));
	m_strArr.Add(m_pBri->m_pARcBridgeDataStd->GetStringDia(19));
	m_strArr.Add(m_pBri->m_pARcBridgeDataStd->GetStringDia(22));
	m_strArr.Add(m_pBri->m_pARcBridgeDataStd->GetStringDia(25));
	m_strArr.Add(m_pBri->m_pARcBridgeDataStd->GetStringDia(29));
	m_strArr.Add(m_pBri->m_pARcBridgeDataStd->GetStringDia(32));
	m_strArr.Add(m_pBri->m_pARcBridgeDataStd->GetStringDia(35));

	long nIdxDiaMax = -1;
	long nIdxDiaMin = -1;
	long i=0; for(i=0; i<m_strArr.GetSize(); i++)
	{
		double Dia = m_pBri->m_pARcBridgeDataStd->GetDia(m_strArr.GetAt(i));
		if(Dia==m_pBri->m_dOptimizeDiaMaximum) nIdxDiaMax = i;
		if(Dia==m_pBri->m_dOptimizeDiaMinimum) nIdxDiaMin = i;

		m_cbDiaMax.AddString(m_strArr.GetAt(i));
		m_cbDiaMin.AddString(m_strArr.GetAt(i));
	}
	m_cbDiaMin.SetCurSel(nIdxDiaMin);
	m_cbDiaMax.SetCurSel(nIdxDiaMax);

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COptimizeRebar::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);

	m_pBri->m_dOptimizeSafeRate   = atof(m_szSafeRate);
	m_pBri->m_bOptimizeRebar2Dan  = m_bCheckRebar2Dan;
	m_pBri->m_bOptimizeRebarSplit = m_bCheckSpliteCycle;
	m_pBri->m_bOptimizeRebarMatch = m_bCheckRebarMatch;

	long nIdxDiaMax = m_cbDiaMax.GetCurSel();
	if(nIdxDiaMax==0) m_pBri->m_dOptimizeDiaMaximum = 10;
	if(nIdxDiaMax==1) m_pBri->m_dOptimizeDiaMaximum = 13;
	if(nIdxDiaMax==2) m_pBri->m_dOptimizeDiaMaximum = 16;
	if(nIdxDiaMax==3) m_pBri->m_dOptimizeDiaMaximum = 19;
	if(nIdxDiaMax==4) m_pBri->m_dOptimizeDiaMaximum = 22;
	if(nIdxDiaMax==5) m_pBri->m_dOptimizeDiaMaximum = 25;
	if(nIdxDiaMax==6) m_pBri->m_dOptimizeDiaMaximum = 29;
	if(nIdxDiaMax==7) m_pBri->m_dOptimizeDiaMaximum = 32;
	if(nIdxDiaMax==8) m_pBri->m_dOptimizeDiaMaximum = 35;

	long nIdxDiaMin = m_cbDiaMin.GetCurSel();
	if(nIdxDiaMin==0) m_pBri->m_dOptimizeDiaMinimum = 10;
	if(nIdxDiaMin==1) m_pBri->m_dOptimizeDiaMinimum = 13;
	if(nIdxDiaMin==2) m_pBri->m_dOptimizeDiaMinimum = 16;
	if(nIdxDiaMin==3) m_pBri->m_dOptimizeDiaMinimum = 19;
	if(nIdxDiaMin==4) m_pBri->m_dOptimizeDiaMinimum = 22;
	if(nIdxDiaMin==5) m_pBri->m_dOptimizeDiaMinimum = 25;
	if(nIdxDiaMin==6) m_pBri->m_dOptimizeDiaMinimum = 29;
	if(nIdxDiaMin==7) m_pBri->m_dOptimizeDiaMinimum = 32;
	if(nIdxDiaMin==8) m_pBri->m_dOptimizeDiaMinimum = 35;
	
	CDialog::OnOK();
}
