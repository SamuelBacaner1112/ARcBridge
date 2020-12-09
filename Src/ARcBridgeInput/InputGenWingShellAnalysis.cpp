// InputGenWingShellAnalysis.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "arcbridgeinput.h"
#include "../ARcBridgeCalculate/ARcBridgeCalculate.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WM_USER_MIDASLOAD_COMPLETE WM_USER+1
// CInputGenWingShellAnalysis 대화 상자입니다.

CInputGenWingShellAnalysis::CInputGenWingShellAnalysis(CWnd* pParent /*=NULL*/)
	: CInputGenAnalysisBaseShell(CInputGenWingShellAnalysis::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInputGenWingShellAnalysis)
	m_nRadioView	 = 1;
	//}}AFX_DATA_INIT
	
	m_nTypeDesign = 6;

	m_bLeft = FALSE;
	m_bStt = FALSE;
}

CInputGenWingShellAnalysis::~CInputGenWingShellAnalysis()
{
}

void CInputGenWingShellAnalysis::DoDataExchange(CDataExchange* pDX)
{
	CInputGenAnalysisBaseShell::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputGenWingShellAnalysis)
	DDX_Control(pDX, IDC_BUTTON_VIEW_SETTING, m_btnViewSetting);
	DDX_Control(pDX, IDC_COMBO_SELECT_WING, m_cbSelectWing);
	DDX_Control(pDX, IDC_COMBO_MESH_SIZE, m_ComboMeshSize);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInputGenWingShellAnalysis, CInputGenAnalysisBaseShell)
	//{{AFX_MSG_MAP(CInputGenWingShellAnalysis)
	ON_CBN_SELCHANGE(IDC_COMBO_SELECT_WING, OnSelchangeComboSelectWing)
	ON_BN_CLICKED(IDC_BUTTON_VIEW_CALC, OnButtonViewCalc)
	ON_BN_CLICKED(IDC_BUTTON_RUN_TOOL, OnButtonRunSap2000)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_OUTPUT, OnButtonLoadSap)
	ON_BN_CLICKED(IDC_BUTTON_MAKE, OnButtonMakeSap)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_CALC_OK, OnCalcOk)
	ON_MESSAGE(WM_USER_MIDASLOAD_COMPLETE, OnMidasLoadComplete)
	ON_CBN_SELCHANGE(IDC_COMBO_MESH_SIZE, OnSelchangeComboMeshSize)
END_MESSAGE_MAP()


// CInputGenWingShellAnalysis 메시지 처리기입니다.
void CInputGenWingShellAnalysis::OnPreInitDialog()
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
	SetResize(IDC_BUTTON_RUN_TOOL,  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_LOAD_OUTPUT,  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_MAKE,  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);	
	SetResize(IDC_BUTTON_BKCHANGE,  SZ_TOP_RIGHT, SZ_TOP_RIGHT);	
	SetResize(IDC_BUTTON_SAVEIMG,  SZ_TOP_RIGHT, SZ_TOP_RIGHT);		
	SetResize(IDC_COMBO_MESH_SIZE,  SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_STATIC_MESH_SIZE,  SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	
	InitComboBridge();
	InitWingComboCtrl();

	SetHyperHelp(m_pIn->GetManualPath());	
}

void CInputGenWingShellAnalysis::InitComboCtrl()
{
	m_btnRunTool.SetWindowText(_T("SAP 2000 자동 실행"));
	m_btnMakeData.SetWindowText(_T("SAP 2000 Input 생성"));
	m_btnLoadOutput.SetWindowText(_T("SAP 2000 Output 읽기"));

	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	if(pBri)
	{
		double dMeshSize = pBri->m_dMeshSizeWing[m_bStt ? iSTT : iEND][m_bLeft ? iLEFT : iRIGHT];
		long nIdx = (long)(dMeshSize - 200) / 50;
		m_ComboMeshSize.SetCurSel(nIdx);
	}

	SetLoadCaseCombo();
	SetLoadComboCombo();

	SetEnableControl();
}

void CInputGenWingShellAnalysis::OnSelchangeComboBridgeNo() 
{
	m_pIn->m_nCurrentBridge = m_cbBridgeNo.GetCurSel();

	InitWingComboCtrl();

	CInputGenAnalysisBaseShell::OnSelchangeComboBridgeNo();
}

void CInputGenWingShellAnalysis::DrawInputDomyunView(BOOL bZoomAll)
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

void CInputGenWingShellAnalysis::OnButtonRunSap2000() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	CARcBridgeCalcStd CalcStd(pBri);

	CalcStd.m_pShell->MakeSapData(m_bStt, m_bLeft, TRUE);

	m_pIn->RunSap2000(pBri,m_nTypeDesign, this);	
}

UINT CInputGenWingShellAnalysis::DoWork()
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

LRESULT CInputGenWingShellAnalysis::OnMidasLoadComplete(WPARAM, LPARAM)
{
	CFEMManage *pFEM = m_pIn->m_FemDraw.GetFEMManage();
	SetLoadComboCombo();
	SetLoadCaseCombo();						
	m_ComboLoadCombo.SetCurSel(pFEM->GetLoadCaseSize()-pFEM->m_nOutputCaseAddSize);
	UpdateData(FALSE);
	DrawInputDomyunView(TRUE);
	return 0;
}

void CInputGenWingShellAnalysis::OnButtonLoadSap() 
{
	// TODO: Add your control notification handler code here
	CString szOutExt;
	CString szComment;

	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	if(pBri->m_nSelect2DFrameTool==SOLVE_SAP_6AND7)
	{	
		szOutExt  = "*.out|*.*";
		szComment = "Sap2000 Output File(*.out)|*.out|모든파일 (*.*)|*.*||";
	} 
	else if(pBri->m_nSelect2DFrameTool==SOLVE_SAP_8 || pBri->m_nSelect2DFrameTool==SOLVE_SAP_9 || pBri->m_nSelect2DFrameTool==SOLVE_SAP_10)
	{
		szOutExt  = "*.mdb|*.*";
		szComment = "Sap2000 Output File(*.mdb)|*.mdb|모든파일 (*.*)|*.*||";
	}
	else if(pBri->m_nSelect2DFrameTool==SOLVE_MIDAS)
	{
		szOutExt  = "*.anl|*.*";
		szComment = "MIDAS Output File(*.anl)|*.anl|모든파일 (*.*)|*.*||";

	}

	CARcBridgeCalcStd CalcStd(pBri);
	CalcStd.m_pShell->MakeSapData(m_bStt, m_bLeft, TRUE);
	
	CString szPath = "";
	CFileDialog dlg(TRUE, szOutExt, "", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
								  szComment, m_pView);
	dlg.m_ofn.lpstrTitle = "SAP 출력파일 불러오기";
	dlg.m_ofn.lpstrInitialDir = m_pIn->GetPathDirectory();

	if (dlg.DoModal() == IDOK) 
		szPath = dlg.GetPathName();
	else 
		return;

	m_pView->Clear();

	CString szInExt = ".s2k";

	char	drive[_MAX_DRIVE];
	char	dir[_MAX_DIR];
	char	fname[_MAX_FNAME];
	char	ext[_MAX_EXT];		  	
	char	inputname[_MAX_EXT];	

	_SPLITPATH_S(szPath,drive,dir,fname,ext);
	_STRCPY_S(inputname, sizeof(inputname), drive);
	_STRCAT_S(inputname, sizeof(inputname), dir);
	_STRCAT_S(inputname, sizeof(inputname), fname);
	_STRCAT_S(inputname, sizeof(inputname), szInExt);
	CString szInputPath(inputname);

	CDlgProgress *pProgressDlg;
	pProgressDlg = new CDlgProgress;
	if(!pProgressDlg->GetSafeHwnd())
	{
		pProgressDlg->Create(IDD_DIALOG_HGPROGRESS, this);
	}

	CFEMManage *pFEM = m_pIn->m_FemDraw.GetFEMManage();

	if(pBri->m_nSelect2DFrameTool==SOLVE_SAP_6AND7 || 
		pBri->m_nSelect2DFrameTool==SOLVE_SAP_8 ||
		pBri->m_nSelect2DFrameTool==SOLVE_SAP_9 ||
		pBri->m_nSelect2DFrameTool==SOLVE_SAP_10)
	{
		try{			
			pProgressDlg->ShowWindow(SW_SHOW);
			pProgressDlg->CenterWindow();
			pProgressDlg->InitProgressData();

			CLoadSAP2000OutputShell LoadOut(pFEM);
			if(pBri->m_nSelect2DFrameTool==SOLVE_SAP_6AND7)
			{				
				if(LoadOut.LoadData(szPath))
				{
					m_pIn->m_FemDraw.SetFlag(FEMVIEW_ELEMENT_FORCE, TRUE);
					m_pIn->m_FemDraw.SetFlag(FEMVIEW_COLORINDEX, TRUE);
					SetLoadComboCombo();
					SetLoadCaseCombo();
					UpdateData(FALSE);
					DrawInputDomyunView(TRUE);
				}
			}
			else
			{
// 				pProgressDlg->ShowWindow(SW_SHOW);
// 				pProgressDlg->CenterWindow();
// 				pProgressDlg->InitProgressData();
// 
// 				CString szTitle = _T("");
// 				pProgressDlg->GetDlgItem(IDC_STATIC_HGMESSAGE2)->SetWindowText(szTitle);
// 				if(LoadOut.LoadSap2000Ver9MDB(szPath, &pProgressDlg->m_ctlProgress, pProgressDlg->GetDlgItem(IDC_STATIC_HGMESSAGE),FALSE))
// 				{
// 					m_pIn->m_FemDraw.SetFlag(FEMVIEW_ELEMENT_FORCE, TRUE);
// 					m_pIn->m_FemDraw.SetFlag(FEMVIEW_COLORINDEX, TRUE);
// 					SetLoadComboCombo();
// 					SetLoadCaseCombo();
// 					UpdateData(FALSE);
// 					DrawInputDomyunView(TRUE);
// 				}
			}
		}
		catch(...)
		{
			AfxMessageBox(szPath + "파일이 비정상적이거나 존재하지 않습니다.", MB_ICONSTOP);	
		}	
	}
	else if(pBri->m_nSelect2DFrameTool==SOLVE_MIDAS)
	{
		CLoadMidasOutput LoadOut(pFEM);
		try{
			if(LoadOut.LoadData(szPath))
			{
				m_pIn->m_FemDraw.SetFlag(FEMVIEW_ELEMENT_FORCE, TRUE);
				m_pIn->m_FemDraw.SetFlag(FEMVIEW_COLORINDEX, TRUE);
				SetLoadComboCombo();
				SetLoadCaseCombo();
				UpdateData(FALSE);
				DrawInputDomyunView(TRUE);
			}
		}
		catch(...)
		{
			AfxMessageBox(szPath + "파일이 비정상적이거나 존재하지 않습니다.", MB_ICONSTOP);	
		}	
	}


	delete pProgressDlg;

	pBri->SetWingWallShellOutput(m_bStt, m_bLeft);

	SetLoadCaseCombo();
	SetLoadComboCombo();
}

void CInputGenWingShellAnalysis::OnButtonMakeSap() 
{
	UpdateData();
	
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	m_pIn->MakeInputData(pBri, m_nTypeDesign, "", SOLVE_SAP_6AND7, FALSE);
}

void CInputGenWingShellAnalysis::SetLoadComboCombo()
{
	long nComboSelect = 0;
	m_ComboLoadCombo.ResetContent();
	CFEMManage *pFEM = m_pIn->m_FemDraw.GetFEMManage();
	if(!pFEM) return;

	CString sCombo = _T("");
	for(long n=0; n<pFEM->m_szLCName.GetSize(); n++)
	{
		sCombo = pFEM->m_szLCName[n];
		m_ComboLoadCombo.AddString(sCombo);
		if(pFEM->m_szLCName[n]=="COMB1")
		{
			nComboSelect = n;
		}
	}

	m_ComboLoadCombo.SetCurSel(nComboSelect);
	DrawInputDomyunView(TRUE);
}

void CInputGenWingShellAnalysis::SetLoadCaseCombo()
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

LRESULT CInputGenWingShellAnalysis::OnCalcOk(WPARAM wp, LPARAM)
{	
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	
	if(m_pIn->CompleteRunOfSap2000(pBri, m_nTypeDesign))
	{
		m_pIn->m_FemDraw.SetFlag(FEMVIEW_ELEMENT_FORCE, TRUE);
		m_pIn->m_FemDraw.SetFlag(FEMVIEW_COLORINDEX, TRUE);
		SetLoadComboCombo();
		SetLoadCaseCombo();

		pBri->SetWingWallShellOutput(m_bStt, m_bLeft);

		UpdateData(FALSE);
		DrawInputDomyunView(FALSE);
	}

	return 0;
}

void CInputGenWingShellAnalysis::OnSelchangeComboLoadcase() 
{
	// TODO: Add your control notification handler code here
	DrawInputDomyunView(FALSE);	
}

void CInputGenWingShellAnalysis::OnSelchangeComboLoadtype() 
{
	// TODO: Add your control notification handler code here
	DrawInputDomyunView(FALSE);	
}

void CInputGenWingShellAnalysis::OnButtonViewCalc() 
{
	// TODO: Add your control notification handler code here
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();

	CFEMManage *pFEM = pBri->GetFemWingWall(m_bStt, m_bLeft);
	if(pFEM->IsOutputLoaded() == FALSE)
	{
		AfxMessageBox(_T("날개벽 해석후 다시 하시기 바랍니다."));
		return;
	}

	CInputCheckGrid Dlg(pBri,IDD_GEN_WING_SHELL_ANLYSIS);

	long nWing = 0;
	if(m_bStt && !m_bLeft) nWing = 1;
	if(!m_bStt && m_bLeft) nWing = 2; 
	if(!m_bStt && !m_bLeft) nWing = 3;
	Dlg.SetBriAndContent(pBri, IDD_GEN_WING_SHELL_ANLYSIS, nWing);

	Dlg.DoModal();
}

void CInputGenWingShellAnalysis::SetDataInit()
{
	CRcBridgeRebar *pBri = dynamic_cast<CRcBridgeRebar *>(m_pIn->GetRcBridge());

	CFEMManage *pFEM = pBri->GetFemWingWall(m_bStt, m_bLeft);

	if(!pFEM->IsOutputLoaded())
	{
		m_pIn->m_FemDraw.SetFlag(FEMVIEW_ELEMENT_FORCE, FALSE);
		m_pIn->m_FemDraw.SetFlag(FEMVIEW_COLORINDEX, FALSE);
		CARcBridgeCalcStd CalcStd(pBri);
		CalcStd.m_pShell->MakeSapData(m_bStt, m_bLeft, TRUE);
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

void CInputGenWingShellAnalysis::SetEnableControl()
{
	CRcBridgeRebar *pBri = dynamic_cast<CRcBridgeRebar *>(m_pIn->GetRcBridge());

	BOOL bEnable = (pBri->m_nSelectWingCalc == 0) ? FALSE : TRUE;

	GetDlgItem(IDC_BUTTON_RUN_TOOL)->EnableWindow(bEnable);
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
	GetDlgItem(IDC_RADIO_VIEW_3D)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO_VIEW_XY)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO_VIEW_XZ)->EnableWindow(bEnable);
	GetDlgItem(IDC_RADIO_VIEW_YZ)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO_V22)->EnableWindow(bEnable);
	GetDlgItem(IDC_RADIO_V33)->EnableWindow(bEnable);
	GetDlgItem(IDC_RADIO_M22)->EnableWindow(bEnable);
	GetDlgItem(IDC_RADIO_M33)->EnableWindow(bEnable);
	GetDlgItem(IDC_RADIO_AXIAL)->EnableWindow(bEnable);
	GetDlgItem(IDC_RADIO_TORTION)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_ELEMENT)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_JOINT)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_LOADTYPE)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_OUTVALUE)->EnableWindow(bEnable);
	GetDlgItem(IDC_CHECK_RESTRAINT)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_BKCHANGE)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_SAVEIMG)->EnableWindow(bEnable);
}

BOOL CInputGenWingShellAnalysis::IsValid()
{
	BOOL bExistWing = FALSE;
	if(!m_pIn) return FALSE;
	if(!m_pIn->m_pARcBridgeDataStd) return FALSE;

	// 날개벽이 하나라도 있다면 TRUE
	long nCountBri	= m_pIn->m_pARcBridgeDataStd->GetBridgeSize();
	for(long bri = 0; bri < nCountBri; bri++)
	{
		CRcBridgeRebar *pBri = m_pIn->m_pARcBridgeDataStd->GetBridgeRebar(bri);
		if(pBri)
		{
			for(long ix=0; ix<4; ix++)
			{
				BOOL bStt = (ix < 2)? TRUE : FALSE;
				BOOL bLeft = (ix == 0 || ix == 2)? TRUE : FALSE;
				if(pBri->IsWingWall(bStt, bLeft) && pBri->m_nSelectWingCalc > 0) 
				{
					bExistWing = TRUE;
					break;
				}
			}
		}
		if(bExistWing) break;
	}

	return bExistWing;
}

void CInputGenWingShellAnalysis::InitWingComboCtrl()
{
	m_cbSelectWing.ResetContent();

	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	if(!pBri) return;

	BOOL bFirst = TRUE;
	for(long nStt=0; nStt<2; nStt++)
	{
		CString strSE = (nStt==0)? _T("시점") : _T("종점");
		for(long nLeft=0; nLeft<2; nLeft++)
		{
			CString strLR = (nLeft==0)? _T("좌측") : _T("우측");
			BOOL bWingWall = pBri->IsWingWall(nStt==0, nLeft==0);

			if(bWingWall)
			{
				CString str = _T("");
				str.Format(_T("%s%s 날개벽"), strSE, strLR);
				m_cbSelectWing.AddString(str);

				if(bFirst)
				{
					m_bStt = (nStt == 0)? TRUE : FALSE;
					m_bLeft = (nLeft == 0)? TRUE : FALSE;
					bFirst = FALSE;
				}
			}
		}
	}
	m_cbSelectWing.SetCurSel(0);
	m_nTypeDesign = 6;
	if(m_bStt && !m_bLeft) m_nTypeDesign = 7;
	if(!m_bStt && m_bLeft) m_nTypeDesign = 8;
	if(!m_bStt && !m_bLeft) m_nTypeDesign = 9;
}

void CInputGenWingShellAnalysis::OnSelchangeComboSelectWing()
{
	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	if(!pBri) return;

	long nSel = m_cbSelectWing.GetCurSel();
	long nIndex = -1;

	for(long nStt=0; nStt<2; nStt++)
	{
		for(long nLeft=0; nLeft<2; nLeft++)
		{
			BOOL bWingWall = pBri->IsWingWall(nStt==0, nLeft==0);
			if(bWingWall)
			{
				nIndex++;
				if(nIndex == nSel)
				{
					m_bStt = (nStt == 0)? TRUE : FALSE;
					m_bLeft = (nLeft == 0)? TRUE : FALSE;
					break;
				}
			}
		}
	}

	m_nTypeDesign = 6;
	if(m_bStt && !m_bLeft) m_nTypeDesign = 7;
	if(!m_bStt && m_bLeft) m_nTypeDesign = 8;
	if(!m_bStt && !m_bLeft) m_nTypeDesign = 9;

	SetDataInit();
}

void CInputGenWingShellAnalysis::OnSelchangeComboMeshSize()
{
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	if(AfxMessageBox(_T("날개벽 MESH를 재생성하는 경우 해석을 다시 수행하셔야 합니다.\n계속 하시겠습니까?"), MB_YESNO) != IDYES)
	{
		double dMeshSize = pBri->m_dMeshSizeWing[m_bStt ? iSTT : iEND][m_bLeft ? iLEFT : iRIGHT];
		long nIdx = (long)(dMeshSize - 200) / 50;
		m_ComboMeshSize.SetCurSel(nIdx);
		return;
	}

	UpdateData(TRUE);

	//	double dSize = 200;
	long nIdx = m_ComboMeshSize.GetCurSel();

	pBri->m_dMeshSizeWing[m_bStt ? iSTT : iEND][m_bLeft ? iLEFT : iRIGHT] = 200 + (50 * nIdx);

	CARcBridgeCalcStd CalcStd(pBri);
	CalcStd.m_pShell->MakeSapData(m_bStt, m_bLeft, TRUE);

	SetDataInit();
	DrawInputDomyunView(FALSE);
}
