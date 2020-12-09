// InputGenAnalysisBase.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "arcbridgeinput.h"

// CInputGenAnalysisBase 대화 상자입니다.
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CInputGenAnalysisBase, CInputDialogBase)

CInputGenAnalysisBase::CInputGenAnalysisBase(UINT nID, CWnd* pParent /*=NULL*/)
	: CInputDialogBase(nID, pParent)
{
	m_bCheckViewJoint = FALSE;
	m_bChceckViewElement  = FALSE;
	m_bCheckLoadType	  = FALSE;
	m_bCheckViewOutValue  = FALSE;
	m_bCheckViewRestraint = FALSE;	

	m_dSizeDataValue = 0.2;
	m_dSizeElement	 = 0.2;
	m_dSizeJoint	 = 0.2;

	m_bGL = TRUE;

	m_cAxis = 'y';

	m_nRadioView = -1;
}

CInputGenAnalysisBase::~CInputGenAnalysisBase()
{
}

void CInputGenAnalysisBase::DoDataExchange(CDataExchange* pDX)
{
	CInputDialogBase::DoDataExchange(pDX);
	
	DDX_Control(pDX, IDC_COMBO_LOADCASE, m_ComboLoadCase);
	DDX_Control(pDX, IDC_COMBO_LOADCOMBO, m_ComboLoadCombo);
	DDX_Control(pDX, IDC_COMBO_BRIDGE_NO, m_cbBridgeNo);
	DDX_Control(pDX, IDC_BUTTON_BKCHANGE, m_btnBkChange);
	DDX_Control(pDX, IDC_BUTTON_LOAD_OUTPUT, m_btnLoadOutput);
	DDX_Control(pDX, IDC_BUTTON_MAKE, m_btnMakeData);
	DDX_Control(pDX, IDC_BUTTON_SAVEIMG, m_btnSaveImage);
	DDX_Control(pDX, IDC_BUTTON_RUN_TOOL, m_btnRunTool);
	DDX_Control(pDX, IDC_BUTTON_VIEW_CALC, m_btnViewCalc);
	DDX_Check(pDX, IDC_CHECK_JOINT, m_bCheckViewJoint);
	DDX_Check(pDX, IDC_CHECK_ELEMENT, m_bChceckViewElement);
	DDX_Check(pDX, IDC_CHECK_LOADTYPE, m_bCheckLoadType);
	DDX_Check(pDX, IDC_CHECK_OUTVALUE, m_bCheckViewOutValue);
	DDX_Check(pDX, IDC_CHECK_RESTRAINT, m_bCheckViewRestraint);	
	DDX_Radio(pDX, IDC_RADIO_VIEW_YZ, m_nRadioView);
	DDX_Text(pDX, IDC_EDIT_DATASIZE, m_dSizeDataValue);
	DDX_Text(pDX, IDC_EDIT_ELEMENTNUM_SIZE, m_dSizeElement);
	DDX_Text(pDX, IDC_EDIT_JOINTNUM_SIZE, m_dSizeJoint);
}


BEGIN_MESSAGE_MAP(CInputGenAnalysisBase, CInputDialogBase)
	ON_CBN_SELCHANGE(IDC_COMBO_BRIDGE_NO, OnSelchangeComboBridgeNo)
	ON_CBN_SELCHANGE(IDC_COMBO_LOADCOMBO, OnSelchangeComboLoadcase)
	ON_CBN_SELCHANGE(IDC_COMBO_LOADCASE, OnSelchangeComboLoadtype)
	ON_CPN_XT_SELENDOK(IDC_BUTTON_BKCHANGE, OnSelEndOkBackClr)
	ON_EN_KILLFOCUS(IDC_EDIT_JOINTNUM_SIZE, OnKillfocusEditJointnumSize)
	ON_BN_CLICKED(IDC_RADIO_VIEW_3D, OnRadioView3d)
	ON_BN_CLICKED(IDC_RADIO_VIEW_XY, OnRadioViewXy)
	ON_BN_CLICKED(IDC_RADIO_VIEW_XZ, OnRadioViewXz)
	ON_BN_CLICKED(IDC_RADIO_VIEW_YZ, OnRadioViewYz)
	ON_BN_CLICKED(IDC_BUTTON_VIEW_SETTING, OnButtonViewSetting)
	ON_BN_CLICKED(IDC_BUTTON_SAVEIMG, OnButtonSaveImgage)
	ON_BN_CLICKED(IDC_CHECK_ELEMENT, OnCheckElement)
	ON_BN_CLICKED(IDC_CHECK_JOINT, OnCheckJoint)
	ON_BN_CLICKED(IDC_CHECK_LOADTYPE, OnCheckLoadtype)
	ON_BN_CLICKED(IDC_CHECK_OUTVALUE, OnCheckOutValue)
	ON_BN_CLICKED(IDC_CHECK_RESTRAINT, OnCheckRestraint)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// CInputGenAnalysisBase 메시지 처리기입니다.

void CInputGenAnalysisBase::OnSelchangeComboBridgeNo()
{
	SetDataInit();
	SetPaneTitle();

	DrawInputDomyunView();	
}

void CInputGenAnalysisBase::OnSelchangeComboLoadcase() 
{
	// TODO: Add your control notification handler code here
	DrawInputDomyunView(TRUE);	
}

void CInputGenAnalysisBase::OnSelchangeComboLoadtype() 
{
	// TODO: Add your control notification handler code here
	DrawInputDomyunView(TRUE);	
}

void CInputGenAnalysisBase::OnKillfocusEditJointnumSize()
{

}

void CInputGenAnalysisBase::OnCheckLoadtype() 
{
	UpdateData();
	if(m_bCheckLoadType) 
	{
		GetDlgItem(IDC_COMBO_LOADCASE)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_LOADCOMBO)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_COMBO_LOADCASE)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_LOADCOMBO)->EnableWindow(TRUE);
	}	
	DrawInputDomyunView(TRUE);	
}

void CInputGenAnalysisBase::OnCheckElement() 
{
	DrawInputDomyunView(TRUE);		
}

void CInputGenAnalysisBase::OnCheckJoint() 
{
	DrawInputDomyunView(TRUE);			
}

void CInputGenAnalysisBase::OnCheckOutValue() 
{
	DrawInputDomyunView(TRUE);			
}

void CInputGenAnalysisBase::OnCheckRestraint() 
{
	DrawInputDomyunView(TRUE);
}

void CInputGenAnalysisBase::OnRadioView3d() 
{
	m_cAxis = 'd';
	DrawInputDomyunView(TRUE);
}

void CInputGenAnalysisBase::OnRadioViewXy() 
{
	m_cAxis = 'z';
	DrawInputDomyunView(TRUE);
}

void CInputGenAnalysisBase::OnRadioViewXz() 
{
	m_cAxis = 'y';
	DrawInputDomyunView(TRUE);			
}

void CInputGenAnalysisBase::OnRadioViewYz() 
{
	m_cAxis = 'x';
	DrawInputDomyunView(TRUE);			
}

void CInputGenAnalysisBase::OnButtonViewSetting() 
{
	// TODO: Add your control notification handler code here
	CPostViewOptionDlg Dlg(&m_pIn->m_FemDraw);
	if(Dlg.DoModal()==IDOK)
		DrawInputDomyunView();	
}

void CInputGenAnalysisBase::OnButtonSaveImgage() 
{
	// TODO: Add your control notification handler code here

	CDC *pDC;				
	CDC *pBitDC = new CDC;
	CRect rect;
	CBitmap bmp, *pOldBitmap;	
	CImage Image;

	CFileDialog dlg(FALSE, "", NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
		"JPG 파일 (*.jpg)|*.jpg|BMP 파일 (*.bmp)|*.bmp||", m_pView);
	dlg.m_ofn.lpstrTitle = "파일 저장";
	dlg.m_ofn.lpstrInitialDir = m_pIn->GetPathDirectory();

	if (dlg.DoModal() != IDOK) 
	{
		delete pBitDC;
		return;
	}

	ShowWindow(SW_HIDE);
	pDC = m_pView->GetDC();
	//m_pView->OnDrawMain(pDC);
	CString szFileName = dlg.GetPathName();
	m_pView->GetClientRect(&rect);
	pBitDC->CreateCompatibleDC(pDC);
	bmp.CreateCompatibleBitmap(pDC,rect.right,rect.bottom);				
	pOldBitmap = pBitDC->SelectObject(&bmp);	
	//m_pView->OnDrawMain(pDC);	// 이미지로 저장할 화면	
	pBitDC->BitBlt(0,0,rect.right,rect.bottom,pDC,0,0,SRCCOPY);	
	if(!Image.CopyFromBmp(&bmp))
	{
		AfxMessageBox("지원할수 없는 형식입니다 !");
		delete pBitDC;
		return;
	}	
	Image.SaveImage(szFileName.GetBuffer(szFileName.GetLength()));	
	pBitDC->SelectObject(pOldBitmap);

	ReleaseDC(pDC);
	delete pBitDC;	
	ShowWindow(SW_SHOW);
}

void CInputGenAnalysisBase::OnDestroy() 
{
	CInputDialogBase::OnDestroy();

	// TODO: Add your message handler code here
	m_pView->SetGL(FALSE);
}

void CInputGenAnalysisBase::InitComboBridge()
{
	int nSize = m_pIn->m_pARcBridgeDataStd->GetBridgeSize();
	CString str = _T("");
	int i=0; for(i=0; i<nSize; i++)
	{
		CRcBridgeApp* pBri = m_pIn->m_pARcBridgeDataStd->GetBridge(i);
		str = m_pIn->m_pARcBridgeDataStd->GetStringBridgeOrder(i) + _T(" : ") + pBri->m_strBridgeName
			+ _T(" [") + pBri->GetStringBridgeType() + _T("]");
		m_cbBridgeNo.AddString(str);
	}
	m_cbBridgeNo.SetCurSel(m_pIn->m_nCurrentBridge);
}

void CInputGenAnalysisBase::OnSelEndOkBackClr()
{
	COLORREF clr = m_btnBkChange.GetColor();
	m_pIn->m_FemDraw.SetBkColor(clr);
	GetView()->Invalidate(FALSE);
}