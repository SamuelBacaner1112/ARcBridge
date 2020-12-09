// InputGenShellAnalysis.cpp : implementation file
//

#include "stdafx.h"
#include "arcbridgeinput.h"
#include "../ARcBridgeCalculate/ARcBridgeCalculate.h"
//#include "InputGenShellAnalysis.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WM_USER_MIDASLOAD_COMPLETE WM_USER+1
/////////////////////////////////////////////////////////////////////////////
// CInputGenShellAnalysis dialog


CInputGenShellAnalysis::CInputGenShellAnalysis(CWnd* pParent /*=NULL*/)
	: CInputGenAnalysisBaseShell(CInputGenShellAnalysis::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInputGenShellAnalysis)
	m_bCheckAllBridge = FALSE;
	m_nRadioView	 = 3;
	m_bKorCode = TRUE;
	//}}AFX_DATA_INIT
	
	m_nTypeDesign = 5;
	
	m_cAxis = 'd';
}


void CInputGenShellAnalysis::DoDataExchange(CDataExchange* pDX)
{
	CInputGenAnalysisBaseShell::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputGenShellAnalysis)	
	DDX_Control(pDX, IDC_BUTTON_VIEW_SETTING, m_btnViewSetting);
	DDX_Check(pDX, IDC_CHECK_ALLBRIDGE, m_bCheckAllBridge);
	DDX_Check(pDX, IDC_CHECK_KOR, m_bKorCode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInputGenShellAnalysis, CInputGenAnalysisBaseShell)
	//{{AFX_MSG_MAP(CInputGenShellAnalysis)
	ON_BN_CLICKED(IDC_BUTTON_VIEW_CALC, OnButtonViewCalc)
	ON_BN_CLICKED(IDC_BUTTON_RUN_TOOL, OnButtonRunSap2000)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_OUTPUT, OnButtonLoadSap)
	ON_BN_CLICKED(IDC_BUTTON_MAKE, OnButtonMakeSap)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_CALC_OK, OnCalcOk)
	ON_MESSAGE(WM_USER_MIDASLOAD_COMPLETE, OnMidasLoadComplete)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputGenShellAnalysis message handlers

void CInputGenShellAnalysis::OnPreInitDialog()
{	
	m_btnBkChange.SetColor(m_pView->GetDomBkColor());
	
	SetResize(IDC_STATIC1,  SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_STATIC2,  SZ_TOP_LEFT, SZ_TOP_RIGHT);
	SetResize(IDC_STATIC3,  SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_STATIC4,  SZ_TOP_LEFT, SZ_TOP_RIGHT);
	SetResize(IDC_STATIC_JUNC,  SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_STATIC_ELEM,  SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_STATIC_DATA,  SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_STATIC_BACK_CLR,  SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_EDIT_JOINTNUM_SIZE,  SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_EDIT_ELEMENTNUM_SIZE,  SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_EDIT_DATASIZE,  SZ_TOP_RIGHT, SZ_TOP_RIGHT);

	SetResize(IDC_BUTTON_VIEW_CALC,  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_CHECK_ALLBRIDGE,  SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_BUTTON_RUN_TOOL,  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_LOAD_OUTPUT,  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_MAKE,  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_BKCHANGE,  SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_BUTTON_SAVEIMG,  SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	
	InitComboBridge();

	SetHyperHelp(m_pIn->GetManualPath());	
}

void CInputGenShellAnalysis::InitComboCtrl()
{
	CRcBridgeRebar *pBri = dynamic_cast<CRcBridgeRebar *>(m_pIn->GetRcBridge());
	long nTool = pBri->m_nSelect3DShellTool;
	if(nTool==0)
	{
		GetDlgItem(IDC_CHECK_KOR)->ShowWindow(SW_HIDE);
		m_btnRunTool.EnableWindow(FALSE);
		m_btnRunTool.SetWindowText(_T("SAP 2000 자동 실행"));
		m_btnMakeData.SetWindowText(_T("SAP 2000 Input 생성"));
		m_btnLoadOutput.SetWindowText(_T("SAP 2000 Output 읽기"));
	}
	else
	{
		GetDlgItem(IDC_CHECK_KOR)->ShowWindow(SW_SHOW);
		m_btnRunTool.EnableWindow(FALSE);
		m_btnRunTool.SetWindowText(_T("MIDAS Civil 자동 실행"));
		m_btnMakeData.SetWindowText(_T("MIDAS Civil Input 생성"));
		m_btnLoadOutput.SetWindowText(_T("MIDAS Civil Output 읽기"));
	}

	SetLoadCaseCombo();
	SetLoadComboCombo();

	SetEnableControl();
}

void CInputGenShellAnalysis::OnSelchangeComboBridgeNo() 
{
	m_pIn->m_nCurrentBridge = m_cbBridgeNo.GetCurSel();

	CInputGenAnalysisBaseShell::OnSelchangeComboBridgeNo();
}

void CInputGenShellAnalysis::DrawInputDomyunView(BOOL bZoomAll)
{
	UpdateData();

	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	if(!pBri) return;

	long nLoadCombo = m_ComboLoadCombo.GetCurSel();
	long nLoadCase  = m_ComboLoadCase.GetCurSel();

	CDomyun *pDom = m_pView->GetDomyun();
	pDom->ClearEtt(TRUE, FALSE);
	// 도면의 기본 영역을 잡기위한 포인트 설정 - 항상 호출해야함
	m_pIn->m_FemDraw.InsertDomyunPoint();			

	m_pIn->m_FemDraw.SetFlag(FEMVIEW_JOINT_NUMBER, m_bCheckViewJoint);
	m_pIn->m_FemDraw.SetFlag(FEMVIEW_ELEMENT_NUMBER, m_bChceckViewElement);
	m_pIn->m_FemDraw.SetFlag(FEMVIEW_RENDER_RESTRAINT, m_bCheckViewRestraint);
	m_pIn->m_FemDraw.SetFlag(FEMVIEW_ELEMENT_FORCEVALUE, m_bCheckViewOutValue);
	m_pIn->m_FemDraw.SetFlag(FEMVIEW_LOADCASE, m_bCheckLoadType);

	if(m_bCheckLoadType)	m_pIn->m_FemDraw.SetLoadCase(nLoadCase);	
	else					m_pIn->m_FemDraw.SetLoadCase(nLoadCombo);	

	m_pIn->m_FemDraw.SetViewShellForceType(m_nForceType);
	m_pIn->m_FemDraw.SetViewForceType(m_nForceTypeFrame);
	
	m_pView->SetAxis(m_cAxis);
	CInputGenAnalysisBaseShell::DrawInputDomyunView(bZoomAll);
}

void CInputGenShellAnalysis::OnButtonRunSap2000() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	CARcBridgeCalcStd CalcStd(pBri);
	CalcStd.m_pShell->MakeMidasData(TRUE);

	if(m_bCheckAllBridge)
	{
		m_pIn->RunSap2000All(this,m_nTypeDesign);	
	}
	else
	{
		CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
		m_pIn->RunSap2000(pBri,m_nTypeDesign, this);	
	}
}

UINT CInputGenShellAnalysis::DoWork()
{
	CFEMManage *pFEM = m_pIn->m_FemDraw.GetFEMManage();
	while(!m_MidasProgressDlg.m_ctlProgress.GetSafeHwnd());
	CLoadMidasOutput LoadOut(pFEM,&m_MidasProgressDlg.m_ctlProgress);
	m_MidasProgressDlg.m_ctlProgress.SetPos(0);
	try{
		if(LoadOut.LoadData(m_szInputPath))
		{
			m_pIn->m_FemDraw.SetFlag(FEMVIEW_ELEMENT_FORCE, TRUE);
			m_pIn->m_FemDraw.SetFlag(FEMVIEW_COLORINDEX, TRUE);			
			m_MidasProgressDlg.SendMessage(WM_COMMAND, IDOK);
			PostMessage(WM_USER_MIDASLOAD_COMPLETE,0,0);			
		}
	}
	catch(...)
	{
		AfxMessageBox(m_szInputPath + "파일이 비정상적이거나 존재하지 않습니다.", MB_ICONSTOP);	
		m_MidasProgressDlg.SendMessage(WM_COMMAND, IDOK);
	}	
		
	return 0;
}

LRESULT CInputGenShellAnalysis::OnMidasLoadComplete(WPARAM, LPARAM)
{
	CFEMManage *pFEM = m_pIn->m_FemDraw.GetFEMManage();
	SetLoadComboCombo();
	SetLoadCaseCombo();						
	m_ComboLoadCombo.SetCurSel(pFEM->GetLoadCaseSize()-pFEM->m_nOutputCaseAddSize);
	UpdateData(FALSE);
	DrawInputDomyunView(TRUE);
	return 0;
}

void CInputGenShellAnalysis::OnButtonLoadSap() 
{
	// TODO: Add your control notification handler code here
	CString szExt = "*.anl|*.*"; // "*.out|*.*"
	CString szComment = "MIDAS Output File(*.anl)|*.anl|모든파일 (*.*)|*.*||";// "Sap2000 Output File(*.out)|*.out|모든파일 (*.*)|*.*||"

	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	if(pBri->m_bUseShellUserOutput)
	{
		if(AfxMessageBox("3D PLATE 해석결과가 사용자 입력값으로 되어있습니다.      \n결과를 읽어오는 경우 사용자 입력은 적용되지 않습니다.      \n계속 하시겠습니까 ?",MB_YESNO)==IDNO)
		{
			return;
		}
		else
		{
			pBri->m_bUseShellUserOutput = FALSE;
		}
	}

	CARcBridgeCalcStd CalcStd(pBri);
	CalcStd.m_pShell->MakeMidasData(TRUE);
	
	CString szPath = "";
	CFileDialog dlg(TRUE, szExt, "", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
								  szComment, m_pView);
	dlg.m_ofn.lpstrTitle = "MIDAS 출력파일 불러오기";//"SAP 출력파일 불러오기";
	dlg.m_ofn.lpstrInitialDir = m_pIn->GetPathDirectory();

	if (dlg.DoModal() == IDOK) 
		szPath = dlg.GetPathName();
	else 
		return;

	//m_pView->Clear();
/*
	char	drive[_MAX_DRIVE];
	char	dir[_MAX_DIR];
	char	fname[_MAX_FNAME];
	char	ext[_MAX_EXT];		  	
	char	inputname[_MAX_EXT];		  	
	_splitpath(szPath,drive,dir,fname,ext);
	strcpy(inputname,drive);
	strcat(inputname,dir);
	strcat(inputname,fname);
	strcat(inputname,".s2k");
	CString szInputPath(inputname);
*/
	m_szInputPath = szPath;
	Begin();
	m_MidasProgressDlg.DoModal();	

	SetLoadCaseCombo();
	SetLoadComboCombo();
}

void CInputGenShellAnalysis::OnButtonMakeSap() 
{
	UpdateData();
	
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return;
	
	m_pIn->MakeInputData(pBri, m_nTypeDesign, "", SOLVE_MIDAS, m_bKorCode);
}

void CInputGenShellAnalysis::SetLoadComboCombo()
{
	long nComboSelect = 0;
	m_ComboLoadCombo.ResetContent();
	CFEMManage *pFEM = m_pIn->m_FemDraw.GetFEMManage();
	if(!pFEM) return;
	long n=0; for(n=0; n<pFEM->m_szLCName.GetSize(); n++)
	{
		m_ComboLoadCombo.AddString(pFEM->m_szLCName[n]);
		if(pFEM->m_szLCName[n]=="ENVULT MAX")
			nComboSelect = n;
	}
	m_ComboLoadCombo.SetCurSel(nComboSelect);
	DrawInputDomyunView(TRUE);
}

void CInputGenShellAnalysis::SetLoadCaseCombo()
{
	CFEMManage *pFEM = m_pIn->m_FemDraw.GetFEMManage();
	if(!pFEM) return;

	m_ComboLoadCase.ResetContent();	
	long n=0; for(n=0; n<pFEM->m_szLCName.GetSize() - pFEM->m_nOutputCaseAddSize; n++)
	{
		m_ComboLoadCase.AddString(pFEM->m_szLCName[n]);
	}
	m_ComboLoadCase.SetCurSel(0);
	DrawInputDomyunView(TRUE);
}

LRESULT CInputGenShellAnalysis::OnCalcOk(WPARAM wp, LPARAM)
{	
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();

	if(m_bCheckAllBridge)
	{
		long nSizeBridge = m_pIn->m_pARcBridgeDataStd->GetBridgeSize();
		for(long nBridge=0; nBridge<nSizeBridge; nBridge++)
		{	
			CRcBridgeRebar *pBri = m_pIn->m_pARcBridgeDataStd->GetBridgeRebar(nBridge);
			m_pIn->CompleteRunOfSap2000(pBri, m_nTypeDesign);
		}
		SetLoadComboCombo();
		SetLoadCaseCombo();
		UpdateData(FALSE);
		DrawInputDomyunView(TRUE);
	}
	else
	{		
		if(m_pIn->CompleteRunOfSap2000(pBri, m_nTypeDesign))
		{
			m_pIn->m_FemDraw.SetFlag(FEMVIEW_ELEMENT_FORCE, TRUE);
			m_pIn->m_FemDraw.SetFlag(FEMVIEW_COLORINDEX, TRUE);
			SetLoadComboCombo();
			SetLoadCaseCombo();
			UpdateData(FALSE);
			DrawInputDomyunView(TRUE);
		}
	}

	return 0;
}

void CInputGenShellAnalysis::OnButtonViewCalc() 
{
	// TODO: Add your control notification handler code here
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	CInputCheckGrid Dlg(pBri,IDD_GEN_SHELL_ANLYSIS);

	Dlg.DoModal();
}

void CInputGenShellAnalysis::SetDataInit()
{
	CRcBridgeRebar *pBri = dynamic_cast<CRcBridgeRebar *>(m_pIn->GetRcBridge());
	CFEMManage *pFEM = &pBri->m_FemShell;
	if(!pFEM->IsOutputLoaded())
	{
		m_pIn->m_FemDraw.SetFlag(FEMVIEW_ELEMENT_FORCE, FALSE);
		m_pIn->m_FemDraw.SetFlag(FEMVIEW_COLORINDEX, FALSE);
		CARcBridgeCalcStd CalcStd(pBri);
		CalcStd.m_pShell->MakeMidasData(TRUE);
	}	
	else
	{
		m_pIn->m_FemDraw.SetFlag(FEMVIEW_ELEMENT_FORCE, TRUE);
		m_pIn->m_FemDraw.SetFlag(FEMVIEW_COLORINDEX, TRUE);
	}	

	if(m_bCheckLoadType)
		GetDlgItem(IDC_COMBO_LOADCASE)->EnableWindow(TRUE);
	else
		GetDlgItem(IDC_COMBO_LOADCASE)->EnableWindow(FALSE);
	
	m_pIn->m_FemDraw.ChangeFEM(pFEM);
	InitComboCtrl();
	
	switch(m_nForceType)
	{
	case FORCE_F11:		((CButton *)GetDlgItem(IDC_RADIO_FXX))->SetCheck(1); break;
	case FORCE_F22:		((CButton *)GetDlgItem(IDC_RADIO_FYY))->SetCheck(1); break;
	case FORCE_F12:		((CButton *)GetDlgItem(IDC_RADIO_FXY))->SetCheck(1); break;
	case FORCE_MAX:		((CButton *)GetDlgItem(IDC_RADIO_FMAX))->SetCheck(1); break;
	case FORCE_MIN:		((CButton *)GetDlgItem(IDC_RADIO_FMIN))->SetCheck(1); break;
	case MOMENT_M11:	((CButton *)GetDlgItem(IDC_RADIO_MXX))->SetCheck(1); break;
	case MOMENT_M22:	((CButton *)GetDlgItem(IDC_RADIO_MYY))->SetCheck(1); break;
	case MOMENT_M12:	((CButton *)GetDlgItem(IDC_RADIO_MXY))->SetCheck(1); break;
	case MOMENT_MAX:	((CButton *)GetDlgItem(IDC_RADIO_MMAX))->SetCheck(1); break;
	case MOMENT_MIN:	((CButton *)GetDlgItem(IDC_RADIO_MMIN))->SetCheck(1); break;
	case VER_13:		((CButton *)GetDlgItem(IDC_RADIO_VXX))->SetCheck(1); break;
	case VER_23:		((CButton *)GetDlgItem(IDC_RADIO_VYY))->SetCheck(1); break;
	}
	
	switch(m_nForceTypeFrame)
	{
	case ELE_SHEAR_2:		((CButton *)GetDlgItem(IDC_RADIO_V22))->SetCheck(1); break;
	case ELE_MOMENT2:		((CButton *)GetDlgItem(IDC_RADIO_M22))->SetCheck(1); break;
	case ELE_AXFORCE:		((CButton *)GetDlgItem(IDC_RADIO_AXIAL))->SetCheck(1); break;
	case ELE_SHEAR_3:		((CButton *)GetDlgItem(IDC_RADIO_V33))->SetCheck(1); break;
	case ELE_MOMENT3:		((CButton *)GetDlgItem(IDC_RADIO_M33))->SetCheck(1); break;
	case ELE_AXTORSN:		((CButton *)GetDlgItem(IDC_RADIO_TORTION))->SetCheck(1); break;
	}
	DrawInputDomyunView(TRUE);
}

void CInputGenShellAnalysis::SetEnableControl()
{
	CRcBridgeRebar *pBri = dynamic_cast<CRcBridgeRebar *>(m_pIn->GetRcBridge());

	BOOL bEnable = (pBri->m_nSelect3DShellTool==1) ? TRUE : FALSE;

//	GetDlgItem(IDC_BUTTON_RUN_TOOL)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_MAKE)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_LOAD_OUTPUT)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_VIEW_CALC)->EnableWindow(bEnable);
	GetDlgItem(IDC_COMBO_LOADCOMBO)->EnableWindow(bEnable);
	GetDlgItem(IDC_COMBO_LOADCASE)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_JOINTNUM_SIZE)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_ELEMENTNUM_SIZE)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_DATASIZE)->EnableWindow(bEnable);
	GetDlgItem(IDC_RADIO_FXX)->EnableWindow(bEnable);
	GetDlgItem(IDC_RADIO_FYY)->EnableWindow(bEnable);
	GetDlgItem(IDC_RADIO_FXY)->EnableWindow(bEnable);
	GetDlgItem(IDC_RADIO_FMAX)->EnableWindow(bEnable);
	GetDlgItem(IDC_RADIO_FMIN)->EnableWindow(bEnable);
	GetDlgItem(IDC_RADIO_MXX)->EnableWindow(bEnable);
	GetDlgItem(IDC_RADIO_MYY)->EnableWindow(bEnable);
	GetDlgItem(IDC_RADIO_MXY)->EnableWindow(bEnable);
	GetDlgItem(IDC_RADIO_MMAX)->EnableWindow(bEnable);
	GetDlgItem(IDC_RADIO_MMIN)->EnableWindow(bEnable);
	GetDlgItem(IDC_RADIO_VXX)->EnableWindow(bEnable);
	GetDlgItem(IDC_RADIO_VYY)->EnableWindow(bEnable);
	GetDlgItem(IDC_RADIO_VIEW_3D)->EnableWindow(bEnable);
	GetDlgItem(IDC_RADIO_VIEW_XY)->EnableWindow(bEnable);
	GetDlgItem(IDC_RADIO_VIEW_XZ)->EnableWindow(bEnable);
	GetDlgItem(IDC_RADIO_VIEW_YZ)->EnableWindow(bEnable);
	GetDlgItem(IDC_RADIO_V22)->EnableWindow(bEnable);
	GetDlgItem(IDC_RADIO_V33)->EnableWindow(bEnable);
	GetDlgItem(IDC_RADIO_M22)->EnableWindow(bEnable);
	GetDlgItem(IDC_RADIO_M33)->EnableWindow(bEnable);
	GetDlgItem(IDC_RADIO_AXIAL)->EnableWindow(bEnable);
	GetDlgItem(IDC_RADIO_TORTION)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_ALLBRIDGE)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_ELEMENT)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_JOINT)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_LOADTYPE)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_OUTVALUE)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_RESTRAINT)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_BKCHANGE)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_SAVEIMG)->EnableWindow(bEnable);
}

BOOL CInputGenShellAnalysis::IsValid()
{
	BOOL bCalc3D = FALSE;
	if(!m_pIn) return FALSE;
	if(!m_pIn->m_pARcBridgeDataStd) return FALSE;

//	if(m_pIn->GetRcBridge()->m_nTypeBridge == BRIDGE_TYPE_ARCH) return FALSE;

	long nCountBri	= m_pIn->m_pARcBridgeDataStd->GetBridgeSize();
	for(long bri = 0; bri < nCountBri; bri++)
	{
		CRcBridgeRebar *pBri = m_pIn->m_pARcBridgeDataStd->GetBridgeRebar(bri);
		if(pBri)
		{
			if(pBri->m_nSelect3DShellTool!=2) 
			{
				bCalc3D	= TRUE;
				break;
			}
		}
	}

	return bCalc3D;
}