// DlgCorner.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ARcBridgeInput.h"
#include "resource.h"


// CDlgCorner 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgCorner, CDialog)

CDlgCorner::CDlgCorner(CARcBridgeInputStd* pIn, CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCorner::IDD, pParent)
{
	m_dUpSlab_1Dan = 0.0;
	m_dUpSlab_2Dan = 0.0;
	m_dUpSlab_3Dan = 0.0;
	m_dLoSlab_1Dan = 0.0;
	m_dLoSlab_2Dan = 0.0;
	m_dLoSlab_3Dan = 0.0;

	m_dUpSlab_CornerVert = 0.0;
	m_dLoSlab_CornerVert = 0.0;
	m_bUpSlab_CornerCurve = TRUE;
	m_bLoSlab_CornerCurve = TRUE;

	m_pIn	= pIn;
}

CDlgCorner::~CDlgCorner()
{
}

void CDlgCorner::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_UPSLAB_1DAN, m_dUpSlab_1Dan);
	DDX_Text(pDX, IDC_EDIT_UPSLAB_2DAN, m_dUpSlab_2Dan);
	DDX_Text(pDX, IDC_EDIT_UPSLAB_3DAN, m_dUpSlab_3Dan);
	DDX_Text(pDX, IDC_EDIT_LOSLAB_1DAN, m_dLoSlab_1Dan);
	DDX_Text(pDX, IDC_EDIT_LOSLAB_2DAN, m_dLoSlab_2Dan);
	DDX_Text(pDX, IDC_EDIT_LOSLAB_3DAN, m_dLoSlab_3Dan);

	DDX_Text(pDX, IDC_EDIT_UPSLAB_CORVERT, m_dUpSlab_CornerVert); //ACUL-6077 상부슬래브 절곡
	DDX_Text(pDX, IDC_EDIT_LOSLAB_CORVERT, m_dLoSlab_CornerVert);  //하부 슬래브 절곡
	DDX_Check(pDX, IDC_CHECK_UPSLAB_CORVERT, m_bUpSlab_CornerCurve); //상부슬래브 반지림 적용
	DDX_Check(pDX, IDC_CHECK_LOSLAB_CORVERT, m_bLoSlab_CornerCurve); //하부 슬래브 반지름 적용
}


BEGIN_MESSAGE_MAP(CDlgCorner, CDialog)
	ON_BN_CLICKED(IDC_BUT_DEFAULT, OnButtonDefault)
	ON_BN_CLICKED(IDC_CHECK_UPSLAB_CORVERT, OnClickOption)
	ON_BN_CLICKED(IDC_CHECK_LOSLAB_CORVERT, OnClickOption)
END_MESSAGE_MAP()

// CDlgCorner 메시지 처리기입니다.


BOOL CDlgCorner::OnInitDialog() 
{
	CDialog::OnInitDialog();

	SetInitData();
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgCorner::OnButtonDefault() 
{
	CRcBridgeRebar *pBri	= m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	pBri->m_dDanCorner_UpperSlab[0] = 0.4;
	pBri->m_dDanCorner_UpperSlab[1] = 0.5;
	pBri->m_dDanCorner_UpperSlab[2] = 0.6;

	pBri->m_dDanCorner_LowerSlab[0] = 0.4;
	pBri->m_dDanCorner_LowerSlab[1] = 0.5;
	pBri->m_dDanCorner_LowerSlab[2] = 0.6;

	m_dUpSlab_CornerVert = 0.3;
	m_dLoSlab_CornerVert = 0.3;
	m_bUpSlab_CornerCurve = TRUE;
	m_bLoSlab_CornerCurve = TRUE;

	SetInitData();
	UpdateData(FALSE);	
}

void CDlgCorner::OnCancel() 
{
	// TODO: Add extra cleanup here

	CDialog::OnCancel();
}

void CDlgCorner::OnOK() 
{
	CRcBridgeRebar *pBri	= m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	UpdateData();
 
 	pBri->m_dDanCorner_UpperSlab[0] = m_dUpSlab_1Dan;
 	pBri->m_dDanCorner_UpperSlab[1] = m_dUpSlab_2Dan;
 	pBri->m_dDanCorner_UpperSlab[2] = m_dUpSlab_3Dan;
 	
 	pBri->m_dDanCorner_LowerSlab[0] = m_dLoSlab_1Dan;
 	pBri->m_dDanCorner_LowerSlab[1] = m_dLoSlab_2Dan;
 	pBri->m_dDanCorner_LowerSlab[2] = m_dLoSlab_3Dan;

	pBri->m_dDanCorner_UpperVert = m_dUpSlab_CornerVert ;
	pBri->m_dDanCorner_LowerVert =m_dLoSlab_CornerVert;
	pBri->m_bUpperSlab_CornerCurve = m_bUpSlab_CornerCurve;
	pBri->m_bLowerSlab_CornerCurve = m_bLoSlab_CornerCurve;

	CDialog::OnOK();
}



void CDlgCorner::SetInitData()
{
	CRcBridgeRebar *pBri	= m_pIn->GetRcBridgeRebar();
	if(!pBri) return;
	
 	m_dUpSlab_1Dan = pBri->m_dDanCorner_UpperSlab[0];
 	m_dUpSlab_2Dan = pBri->m_dDanCorner_UpperSlab[1];
 	m_dUpSlab_3Dan = pBri->m_dDanCorner_UpperSlab[2];
 		
 	m_dLoSlab_1Dan = pBri->m_dDanCorner_LowerSlab[0];
 	m_dLoSlab_2Dan = pBri->m_dDanCorner_LowerSlab[1];
 	m_dLoSlab_3Dan = pBri->m_dDanCorner_LowerSlab[2];

	m_dUpSlab_CornerVert = pBri->m_dDanCorner_UpperVert;
	m_dLoSlab_CornerVert = pBri->m_dDanCorner_LowerVert;
	m_bUpSlab_CornerCurve = pBri->m_bUpperSlab_CornerCurve;
	m_bLoSlab_CornerCurve = pBri->m_bLowerSlab_CornerCurve;

	GetDlgItem(IDC_EDIT_UPSLAB_CORVERT)->EnableWindow(m_bUpSlab_CornerCurve == FALSE);
	GetDlgItem(IDC_EDIT_LOSLAB_CORVERT)->EnableWindow(m_bLoSlab_CornerCurve == FALSE);
}

void CDlgCorner::OnClickOption()
{
	CDialog::UpdateData();
	GetDlgItem(IDC_EDIT_UPSLAB_CORVERT)->EnableWindow(m_bUpSlab_CornerCurve == FALSE);
	GetDlgItem(IDC_EDIT_LOSLAB_CORVERT)->EnableWindow(m_bLoSlab_CornerCurve == FALSE);
}
