// InputGenFrameAnalysis.cpp : implementation file
//

#include "stdafx.h"
#include "arcbridgeinput.h"
#include "../ARcBridgeCalculate/ARcBridgeCalculate.h"
//#include "InputGenFrameAnalysis.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInputGenFrameAnalysis dialog


CInputGenFrameAnalysis::CInputGenFrameAnalysis(CWnd* pParent /*=NULL*/)
	: CInputGenAnalysisBase(CInputGenFrameAnalysis::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInputGenFrameAnalysis)
	m_bCheckAllBridge = FALSE;
	m_bCheckAutoClose = TRUE;
	m_nRadioForce    = 1;
	m_nRadioView	 = 1;
	m_bKorCode = TRUE;
	//}}AFX_DATA_INIT
}


void CInputGenFrameAnalysis::DoDataExchange(CDataExchange* pDX)
{
	CInputGenAnalysisBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputGenFrameAnalysis)
	DDX_Control(pDX, IDC_EDIT_DISPMAX, m_ctrlEditUserDispMax);
	DDX_Control(pDX, IDC_CHECK_USER_DISPMAX, m_ctrlCheckUserDispMax);
	DDX_Control(pDX, IDC_BUTTON_VIEW_SETTING, m_btnViewSetting);
	DDX_Control(pDX, IDC_COMBO_TOOLNAME, m_cmbToolName);
	DDX_Control(pDX, IDC_SLIDER_SCALE2, m_ctlDispSlider);
	DDX_Control(pDX, IDC_SLIDER_SCALE, m_ctlSlider);
	DDX_Control(pDX, IDC_TAB_DESIGN_TYPE, m_ctlTab);	
	DDX_Check(pDX, IDC_CHECK_ALLBRIDGE, m_bCheckAllBridge);
	DDX_Check(pDX, IDC_CHECK_AUTO_CLOSE, m_bCheckAutoClose);
	DDX_Radio(pDX, IDC_RADIO_V22, m_nRadioForce);
	DDX_Check(pDX, IDC_CHECK_KOR, m_bKorCode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInputGenFrameAnalysis, CInputGenAnalysisBase)
	//{{AFX_MSG_MAP(CInputGenFrameAnalysis)
	ON_BN_CLICKED(IDC_RADIO_V22, OnRadioV22)
	ON_BN_CLICKED(IDC_RADIO_TORTION, OnRadioTortion)		
	ON_BN_CLICKED(IDC_RADIO_AXIAL, OnRadioAxial)
	ON_BN_CLICKED(IDC_RADIO_DISP, OnRadioDisp)
	ON_BN_CLICKED(IDC_RADIO_M22, OnRadioM22)
	ON_BN_CLICKED(IDC_RADIO_M33, OnRadioM33)
	ON_BN_CLICKED(IDC_RADIO_REACTION, OnRadioReaction)
	ON_BN_CLICKED(IDC_BUTTON_RUN_TOOL, OnButtonRunSap2000)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_OUTPUT, OnButtonLoadSap)
	ON_BN_CLICKED(IDC_BUTTON_MAKE, OnButtonMakeSap)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_SCALE, OnReleasedcaptureSliderScale)
	ON_BN_CLICKED(IDC_RADIO_V33, OnRadioV33)
	ON_BN_CLICKED(IDC_BUTTON_VIEW_CALC, OnButtonViewCalc)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_DESIGN_TYPE, OnSelchangeTabDesignType)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB_DESIGN_TYPE, OnSelchangingTabDesignType)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_SCALE2, OnReleasedcaptureSliderScale2)
	ON_CBN_SELCHANGE(IDC_COMBO_TOOLNAME, OnSelchangeComboToolname)
	ON_BN_CLICKED(IDC_BUTTON_VIEW_SETTING, OnButtonViewSetting)
	ON_BN_CLICKED(IDC_CHECK_USER_DISPMAX, OnCheckUserDispmax)
	ON_EN_CHANGE(IDC_EDIT_DISPMAX, OnChangeEditDispmax)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_CALC_OK, OnCalcOk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputGenFrameAnalysis message handlers

void CInputGenFrameAnalysis::OnPreInitDialog()
{
	CRcBridgeRebar *pBri = dynamic_cast<CRcBridgeRebar *>(m_pIn->GetRcBridge());
	if(pBri ->m_nTypeBridge==BRIDGE_TYPE_ARCH) pBri->TrimRibByFootingUpper();
	pBri->InitSaveDistRigidEndSlab();
	pBri->InitSaveDistRigidEndWall();

	InitTabControl();
	m_btnBkChange.SetColor(m_pView->GetDomBkColor());

	SetControlUserDisp();

	SetResize(IDC_STATIC1,  SZ_TOP_LEFT, SZ_TOP_RIGHT);
	SetResize(IDC_STATIC2,  SZ_TOP_LEFT, SZ_TOP_RIGHT);
	SetResize(IDC_STATIC3,  SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_STATIC_JUNC,  SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_STATIC_ELEM,  SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_STATIC_DATA,  SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_STATIC_SCALE,  SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_STATIC_SCALE2,  SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_SLIDER_SCALE,  SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_SLIDER_SCALE2,  SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_STATIC_BACK_CLR,  SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_EDIT_JOINTNUM_SIZE,  SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_EDIT_ELEMENTNUM_SIZE,  SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_EDIT_DATASIZE,  SZ_TOP_RIGHT, SZ_TOP_RIGHT);

	SetResize(IDC_BUTTON_VIEW_CALC,  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_CHECK_ALLBRIDGE,  SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_CHECK_AUTO_CLOSE,  SZ_TOP_RIGHT, SZ_TOP_RIGHT);
	SetResize(IDC_BUTTON_RUN_TOOL,  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_LOAD_OUTPUT,  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_MAKE,  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);	
	SetResize(IDC_COMBO_TOOLNAME,  SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_BKCHANGE,  SZ_TOP_RIGHT, SZ_TOP_RIGHT);	
	SetResize(IDC_BUTTON_SAVEIMG,  SZ_TOP_RIGHT, SZ_TOP_RIGHT);	
	
	InitComboBridge();

	InitComboCtrlToolName();

	SetHyperHelp(m_pIn->GetManualPath());	
}

void CInputGenFrameAnalysis::InitComboCtrl()
{
	CRcBridgeRebar *pBri = dynamic_cast<CRcBridgeRebar *>(m_pIn->GetRcBridge());

	long nTool = pBri->m_nSelect2DFrameTool;
	if(nTool==SOLVE_SAP_6AND7 || nTool==SOLVE_SAP_8 || nTool==SOLVE_SAP_9 || nTool==SOLVE_SAP_10)
	{
		GetDlgItem(IDC_CHECK_KOR)->ShowWindow(SW_HIDE);
		if(nTool==SOLVE_SAP_6AND7) m_btnRunTool.EnableWindow(TRUE);
		else					   m_btnRunTool.EnableWindow(FALSE);
		m_btnRunTool.SetWindowText(_T("SAP 2000 자동 실행"));
		m_btnMakeData.SetWindowText(_T("SAP 2000 Input 생성"));
		m_btnLoadOutput.SetWindowText(_T("SAP 2000 Output 읽기"));
	}
	else if(nTool==SOLVE_MIDAS)
	{
		GetDlgItem(IDC_CHECK_KOR)->ShowWindow(SW_SHOW);
		m_btnRunTool.EnableWindow(FALSE);
		m_btnRunTool.SetWindowText(_T("MIDAS Civil 자동 실행"));
		m_btnMakeData.SetWindowText(_T("MIDAS Civil Input 생성"));
		m_btnLoadOutput.SetWindowText(_T("MIDAS Civil Output 읽기"));
	}
	else
	{
		GetDlgItem(IDC_CHECK_KOR)->ShowWindow(SW_HIDE);
		m_btnRunTool.EnableWindow(TRUE);
		m_btnRunTool.SetWindowText(_T("NeoMax3D 자동 실행"));
		m_btnMakeData.SetWindowText(_T("NeoMax3D 생성"));
		m_btnLoadOutput.SetWindowText(_T("NeoMax3D 읽기"));
	}

	CFEMManage *pFEM;
	long nSel = m_ctlTab.GetCurSel();
	if(nSel<0) nSel=0;
	switch(nSel)
	{
		case 0: pFEM = &pBri->m_FemNormal; break;
		case 1: pFEM = &pBri->m_FemEQ[0]; break;
		case 2: pFEM = &pBri->m_FemEQ[1]; break;
		case 3: pFEM = &pBri->m_FemEQ[2]; break;
		case 4: pFEM = &pBri->m_FemEQ[3]; break;
	}		

	m_pIn->m_FemDraw.ChangeFEM(pFEM);	

	m_ctlSlider.SetRange(0,10);
	m_ctlDispSlider.SetRange(0,500);		
	SetLoadComboCombo();
	SetLoadCaseCombo();
}

void CInputGenFrameAnalysis::SetDataInit()
{
	CRcBridgeRebar *pBri = dynamic_cast<CRcBridgeRebar *>(m_pIn->GetRcBridge());
	CFEMManage *pFEM;
	long nSel = m_ctlTab.GetCurSel();	
	if(nSel<0) nSel=0;
	switch(nSel)
	{
		case 0: pFEM = &pBri->m_FemNormal; break;
		case 1: pFEM = &pBri->m_FemEQ[0]; break;
		case 2: pFEM = &pBri->m_FemEQ[1]; break;
		case 3: pFEM = &pBri->m_FemEQ[2]; break;
		case 4: pFEM = &pBri->m_FemEQ[3]; break;
	}		
	if(!pFEM->IsOutputLoaded())
	{
		m_pIn->m_FemDraw.SetFlag(FEMVIEW_ELEMENT_FORCE, FALSE);
		m_pIn->m_FemDraw.SetFlag(FEMVIEW_COLORINDEX, FALSE);
		CARcBridgeCalcStd CalcStd(pBri);
		CalcStd.m_pFrame->MakeSapData(nSel,TRUE);
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

	InitComboCtrl();
	m_ctlSlider.SetPos(m_pIn->m_FemDraw.GetScaleGrade());
	m_ctlDispSlider.SetPos((int)m_pIn->m_FemDraw.GetDispScale());
	DrawInputDomyunView(TRUE);
}

void CInputGenFrameAnalysis::OnSelchangeComboBridgeNo() 
{
	m_pIn->m_nCurrentBridge = m_cbBridgeNo.GetCurSel();
	CRcBridgeApp *pBri	= m_pIn->GetRcBridge();
	pBri->InitSaveDistRigidEndSlab();
	pBri->InitSaveDistRigidEndWall();

	InitTabControl();
	SetControlUserDisp();
	
	CInputGenAnalysisBase::OnSelchangeComboBridgeNo();
}

void CInputGenFrameAnalysis::DrawInputDomyunView(BOOL bZoomAll)
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

//	m_pIn->m_FemDraw.SetRestraintScale(pBri->m_pARcBridgeDataStd->IsLsdDesign()? 1.0 : 1.0);
	m_pIn->m_FemDraw.SetFlag(FEMVIEW_JOINT_NUMBER, m_bCheckViewJoint);
	m_pIn->m_FemDraw.SetFlag(FEMVIEW_ELEMENT_NUMBER, m_bChceckViewElement);
	m_pIn->m_FemDraw.SetFlag(FEMVIEW_RENDER_RESTRAINT, m_bCheckViewRestraint);
	m_pIn->m_FemDraw.SetFlag(FEMVIEW_ELEMENT_FORCEVALUE, m_bCheckViewOutValue);
	m_pIn->m_FemDraw.SetFlag(FEMVIEW_LOADCASE, m_bCheckLoadType);

	if(m_bCheckLoadType)	m_pIn->m_FemDraw.SetLoadCase(nLoadCase);	
	else					m_pIn->m_FemDraw.SetLoadCase(nLoadCombo);	

	if(m_nRadioForce==6)
	{
		m_pIn->m_FemDraw.SetFlag(FEMVIEW_JOINT_DISPLACEMENT,TRUE);
		m_pIn->m_FemDraw.SetFlag(FEMVIEW_ELEMENT_DISPLACEMENT,TRUE);	
		m_pIn->m_FemDraw.SetFlag(FEMVIEW_REACTION,FALSE);				

		m_pIn->m_FemDraw.SetFlag(FEMVIEW_ELEMENT_FORCE,FALSE);
		m_pIn->m_FemDraw.SetFlag(FEMVIEW_ELEMENT_FORCEVALUE,FALSE);
	}
	else if(m_nRadioForce==7)
	{
		m_pIn->m_FemDraw.SetFlag(FEMVIEW_JOINT_DISPLACEMENT,FALSE);
		m_pIn->m_FemDraw.SetFlag(FEMVIEW_ELEMENT_DISPLACEMENT,FALSE);	
		m_pIn->m_FemDraw.SetFlag(FEMVIEW_REACTION,TRUE);
		m_pIn->m_FemDraw.SetViewForceType(m_nRadioForce);
		m_pIn->m_FemDraw.SetFlag(FEMVIEW_ELEMENT_FORCE,FALSE);
		m_pIn->m_FemDraw.SetFlag(FEMVIEW_ELEMENT_FORCEVALUE,FALSE);
	}
	else
	{
		m_pIn->m_FemDraw.SetFlag(FEMVIEW_JOINT_DISPLACEMENT,FALSE);
		m_pIn->m_FemDraw.SetFlag(FEMVIEW_ELEMENT_DISPLACEMENT,FALSE);	
		m_pIn->m_FemDraw.SetFlag(FEMVIEW_REACTION,FALSE);				
		CFEMManage *pFEM = m_pIn->m_FemDraw.GetFEMManage();
		if(pFEM && pFEM->IsOutputLoaded())
		{		
			m_pIn->m_FemDraw.SetFlag(FEMVIEW_ELEMENT_FORCE,TRUE);
		}
		m_pIn->m_FemDraw.SetViewForceType(m_nRadioForce);
	}	
	m_pView->SetAxis(m_cAxis);

	CInputGenAnalysisBase::DrawInputDomyunView(bZoomAll);
}

void CInputGenFrameAnalysis::OnRadioV22() 
{
	DrawInputDomyunView(TRUE);
}

void CInputGenFrameAnalysis::OnRadioTortion() 
{
	DrawInputDomyunView(TRUE);
}

void CInputGenFrameAnalysis::OnRadioAxial() 
{
	DrawInputDomyunView(TRUE);
}

void CInputGenFrameAnalysis::OnRadioDisp() 
{
	DrawInputDomyunView(TRUE);
}

void CInputGenFrameAnalysis::OnRadioM22() 
{
	DrawInputDomyunView(TRUE);
}

void CInputGenFrameAnalysis::OnRadioM33() 
{
	DrawInputDomyunView(TRUE);
}

void CInputGenFrameAnalysis::OnRadioReaction() 
{
	DrawInputDomyunView(TRUE);
}

void CInputGenFrameAnalysis::OnRadioV33() 
{
	DrawInputDomyunView(TRUE);	
}

void CInputGenFrameAnalysis::OnButtonRunSap2000() 
{
	// TODO: Add your control notification handler code here
	UpdateData();

	long nTab = m_ctlTab.GetCurSel();

	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();

	if(pBri->m_nTypeBridge==BRIDGE_TYPE_SLAB && nTab>0) return;

	CARcBridgeCalcStd CalcStd(pBri);
	CalcStd.m_pFrame->MakeSapData(nTab,TRUE);

	if(m_bCheckAllBridge)
	{
		if(pBri->m_nSelect2DFrameTool==SOLVE_SAP_6AND7)
			m_pIn->RunSap2000All(this, nTab, m_bCheckAutoClose);	
		else if(pBri->m_nSelect2DFrameTool==SOLVE_NEOMAX)
			m_pIn->RunNeoMaxAll(this, nTab, m_bCheckAutoClose);	
	}
	else
	{
		CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
		if(pBri->m_nSelect2DFrameTool==SOLVE_SAP_6AND7)
			m_pIn->RunSap2000(pBri, nTab, this, m_bCheckAutoClose);	
		else
			 if(pBri->m_nSelect2DFrameTool==SOLVE_NEOMAX)
			m_pIn->RunNeoMax(pBri, nTab, this, m_bCheckAutoClose);	
	}
}

void CInputGenFrameAnalysis::OnButtonLoadSap() 
{
	// TODO: Add your control notification handler code here
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	CWaitCursor cursor;
	
	CString szOutExt;
	CString szComment;

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
	else if(pBri->m_nSelect2DFrameTool==SOLVE_NEOMAX)
	{
		szOutExt  = "*.$D?|*.*";
		szComment = "NeoMax3D Output File(*.$d?)|*.$d?|모든파일 (*.*)|*.*||";
	}

	long nTab = m_ctlTab.GetCurSel();

	CARcBridgeCalcStd CalcStd(pBri);
	CalcStd.m_pFrame->MakeSapData(nTab,TRUE);
	
	CString szPath = "";
	CFileDialog dlg(TRUE, szOutExt, "", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
								  szComment, m_pView);

	if(pBri->m_nSelect2DFrameTool==SOLVE_SAP_6AND7 || 
	   pBri->m_nSelect2DFrameTool==SOLVE_SAP_8 ||
	   pBri->m_nSelect2DFrameTool==SOLVE_SAP_9 ||
	   pBri->m_nSelect2DFrameTool==SOLVE_SAP_10)
		dlg.m_ofn.lpstrTitle = "SAP 출력파일 불러오기";
	else if(pBri->m_nSelect2DFrameTool==SOLVE_MIDAS)
		dlg.m_ofn.lpstrTitle = "MIDAS 출력파일 불러오기";
	else if(pBri->m_nSelect2DFrameTool==SOLVE_NEOMAX)
		dlg.m_ofn.lpstrTitle = "NeoMax3D 출력파일 불러오기";

	dlg.m_ofn.lpstrInitialDir = m_pIn->GetPathDirectory();

	if (dlg.DoModal() == IDOK) szPath = dlg.GetPathName();
	else return;

	m_pView->Clear();

	CString szInExt;
	if(pBri->m_nSelect2DFrameTool==SOLVE_SAP_6AND7)
		szInExt = ".s2k";
	else if(pBri->m_nSelect2DFrameTool==SOLVE_SAP_8 ||
		pBri->m_nSelect2DFrameTool==SOLVE_SAP_9 ||
		pBri->m_nSelect2DFrameTool==SOLVE_SAP_10)
	{
		CXLControl excel;
		double dVersion = excel.GetVersion();

		szInExt = (dVersion < 12) ? _T(".xls") : _T(".xlsx") ;
	}
	else if(pBri->m_nSelect2DFrameTool==SOLVE_MIDAS)
		szInExt = ".mct";
	else if(pBri->m_nSelect2DFrameTool==SOLVE_NEOMAX)
		szInExt = ".t3d";	

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
			BOOL bReadAllOutput = TRUE;
			long nSizeModel = pBri->GetCountEarthQuakeModel(FALSE) + 1; //	+1은 평상시
			for(long i=0; i<nSizeModel; i++)
			{
				CString szOutput = szPath;
				if(i > 0)
				{
					CString strEq = _T("_") + pBri->GetNameStringEarthQuakeModel(i);
					szOutput.Replace("_평상시", strEq);
				}
				else if(szOutput.Find(_T("평상시")) == -1)
				{
					bReadAllOutput = FALSE;
					break;
				}

				CFileFind ff;
				if(!ff.FindFile(szOutput)) bReadAllOutput = FALSE;
				if(!bReadAllOutput) break;
			}			

			if(bReadAllOutput && AfxMessageBox("평상시,지진시의 모든 결과파일을 읽으시겠습니까 ?",MB_YESNO)==IDYES)
			{
				if(!SOLVE_SAP_6AND7)
				{
					pProgressDlg->ShowWindow(SW_SHOW);
					pProgressDlg->CenterWindow();
					pProgressDlg->InitProgressData();
				}

				CFEMLoadSAP2000Output LoadOut(pFEM);			

				pProgressDlg->GetDlgItem(IDC_STATIC_HGMESSAGE2)->SetWindowText("평상시 Sap 2000 Output Loading...");
				BOOL bRen = pBri->m_nSelect2DFrameTool==SOLVE_SAP_6AND7 ? LoadOut.LoadData(szPath) : LoadOut.LoadSap2000Ver9MDB(szPath, &pProgressDlg->m_ctlProgress, pProgressDlg->GetDlgItem(IDC_STATIC_HGMESSAGE), NULL);
				if(bRen)
				{
					m_pIn->m_FemDraw.SetFlag(FEMVIEW_ELEMENT_FORCE, TRUE);
					m_pIn->m_FemDraw.SetFlag(FEMVIEW_COLORINDEX, TRUE);
					SetLoadComboCombo();
					SetLoadCaseCombo();
					UpdateData(FALSE);
					DrawInputDomyunView(TRUE);
				}
				long i=0; for(i=0; i<nSizeModel; i++)
				{
					CFEMManage *pFEM = &pBri->m_FemNormal;
					if(i > 0) pFEM = &pBri->m_FemEQ[i-1];

					CString szOutput = szPath;
					CString szTitle = _T("평상시 Sap 2000 Output Loading...");
					if(i > 0)
					{
						CString strEq = pBri->GetNameStringEarthQuakeModel(i);
						szOutput.Replace("_평상시", _T("_") + strEq);
						szTitle.Format(_T("%s Sap 2000 Output Loading..."), strEq);
					}

					if(pFEM->IsOutputLoaded()) continue;

					CARcBridgeCalcStd CalcStd(pBri);
					CalcStd.m_pFrame->MakeSapData(i,TRUE);

					CFEMLoadSAP2000Output LoadOut(pFEM);
					pProgressDlg->GetDlgItem(IDC_STATIC_HGMESSAGE2)->SetWindowText(szTitle);
					pBri->m_nSelect2DFrameTool==SOLVE_SAP_6AND7 ? LoadOut.LoadData(szOutput) : LoadOut.LoadSap2000Ver9MDB(szOutput, &pProgressDlg->m_ctlProgress, pProgressDlg->GetDlgItem(IDC_STATIC_HGMESSAGE), NULL);									
				}
			}
			else
			{
				pProgressDlg->ShowWindow(SW_SHOW);
				pProgressDlg->CenterWindow();
				pProgressDlg->InitProgressData();

				CFEMLoadSAP2000Output LoadOut(pFEM);
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
					pProgressDlg->ShowWindow(SW_SHOW);
					pProgressDlg->CenterWindow();
					pProgressDlg->InitProgressData();

					CString szTitle = _T("");
					pProgressDlg->GetDlgItem(IDC_STATIC_HGMESSAGE2)->SetWindowText(szTitle);
					if(LoadOut.LoadSap2000Ver9MDB(szPath, &pProgressDlg->m_ctlProgress, pProgressDlg->GetDlgItem(IDC_STATIC_HGMESSAGE),FALSE))
					{
						m_pIn->m_FemDraw.SetFlag(FEMVIEW_ELEMENT_FORCE, TRUE);
						m_pIn->m_FemDraw.SetFlag(FEMVIEW_COLORINDEX, TRUE);
						SetLoadComboCombo();
						SetLoadCaseCombo();
						UpdateData(FALSE);
						DrawInputDomyunView(TRUE);
					}
				}
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
	else if(pBri->m_nSelect2DFrameTool==SOLVE_NEOMAX)
	{
		try{			
			BOOL bReadAllOutput = TRUE;
			long nSizeModel = pBri->GetCountEarthQuakeModel(FALSE) + 1; //	+1은 평상시
			long i=0; for(i=0; i<nSizeModel; i++)
			{
				CString szOutput = szPath;
				if(i > 0)
				{
					CString strEq = pBri->GetNameStringEarthQuakeModel(i);
					szOutput.Replace("_평상시","_" + strEq);
				}
				
				CStdioFile Fout;
				long nReturn = Fout.Open(szOutput,CFile::modeRead);
				Fout.Close();
				if(nReturn==0) bReadAllOutput = FALSE;
				if(!bReadAllOutput) break;
			}

			if(bReadAllOutput && AfxMessageBox("평상시,지진시의 모든 결과파일을 읽으시겠습니까 ?",MB_YESNO)==IDYES)
			{
				CLoadNeoMaxOutput LoadOut(pFEM);
				if(LoadOut.LoadData(szPath))
				{
					m_pIn->m_FemDraw.SetFlag(FEMVIEW_ELEMENT_FORCE, TRUE);
					m_pIn->m_FemDraw.SetFlag(FEMVIEW_COLORINDEX, TRUE);
					SetLoadComboCombo();
					SetLoadCaseCombo();
					UpdateData(FALSE);
					DrawInputDomyunView(TRUE);
				}
				long i=0; for(i=0; i<nSizeModel; i++)
				{
					CFEMManage *pFEM = &pBri->m_FemNormal;
					CString szOutput = szPath;
					if(i > 0)
					{
						CString strEq = pBri->GetNameStringEarthQuakeModel(i);
						szOutput.Replace("_평상시","_" + strEq);
						pFEM = &pBri->m_FemEQ[i-1];
					}
					
					if(pFEM->IsOutputLoaded()) continue;

					CARcBridgeCalcStd CalcStd(pBri);
					CalcStd.m_pFrame->MakeSapData(i,TRUE);

					CLoadNeoMaxOutput LoadOut(pFEM);
					LoadOut.LoadData(szOutput);
				}
			}
			else
			{
				CLoadNeoMaxOutput LoadOut(pFEM);
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
		}
		catch(...)
		{
			AfxMessageBox(szPath + "파일이 비정상적이거나 존재하지 않습니다.", MB_ICONSTOP);	
		}	
	}

	delete pProgressDlg;
	SetLoadCaseCombo();
	SetLoadComboCombo();
}

void CInputGenFrameAnalysis::OnButtonMakeSap() 
{
	UpdateData();
	
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(!pBri) return;

	CWaitCursor cursor;
	
	long nTab = m_ctlTab.GetCurSel();	
	if(pBri->IsBoxType() == FALSE)
	{
		if((nTab==2 || nTab==3) && pBri->m_FemEQ[0].IsOutputLoaded()==FALSE && !pBri->m_bApplyUserDispMax)
		{
			AfxMessageBox("단위하중 해석결과를 읽으신 후에 작업하시기 바랍니다.");
			return;
		}
	}
	long nSololverType;
	switch(pBri->m_nSelect2DFrameTool)
	{
	case 0: nSololverType = SOLVE_SAP_6AND7;	break;
	case 1: nSololverType = SOLVE_SAP_8;		break;
	case 2: nSololverType = SOLVE_SAP_9;		break;
	case 3: nSololverType = SOLVE_SAP_10;		break;
	case 4: nSololverType = SOLVE_MIDAS;		break;
	case 5: nSololverType = SOLVE_NEOMAX;		break;
	}
	m_pIn->MakeInputData(pBri, nTab, "", nSololverType, m_bKorCode);
}

void CInputGenFrameAnalysis::OnReleasedcaptureSliderScale(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UpdateData();
	m_pIn->m_FemDraw.SetScaleGrade(m_ctlSlider.GetPos());
	DrawInputDomyunView(TRUE);		
	
	*pResult = 0;
}

void CInputGenFrameAnalysis::SetLoadComboCombo()
{
	CRcBridgeApp *pBri = m_pIn->GetRcBridge();


	long nComboSelect = 0;
	m_ComboLoadCombo.ResetContent();
	CFEMManage *pFEM = m_pIn->m_FemDraw.GetFEMManage();
	if(!pFEM) return;

	long nTab = m_ctlTab.GetCurSel();
	long n=0; for(n=0; n<pFEM->m_szLCName.GetSize(); n++)
	{
		m_ComboLoadCombo.AddString(pFEM->m_szLCName[n]);
		if(nTab==0)
		{
			if(pFEM->m_szLCName[n]=="ENVULT MAX")
				nComboSelect = n;
		}
		else if(nTab==1)
		{
			if(pBri->m_nSelect2DFrameTool==SOLVE_SAP_6AND7 ||
				pBri->m_nSelect2DFrameTool==SOLVE_SAP_8 ||
				pBri->m_nSelect2DFrameTool==SOLVE_SAP_9 ||
				pBri->m_nSelect2DFrameTool==SOLVE_SAP_10)
			{
				if(pFEM->m_szLCName[n]=="UNIT MAX")
					nComboSelect = n;
			}
			else
			{
				if(pFEM->m_szLCName[n]=="UNIT")
					nComboSelect = n;
			}
		}
		else
		{
			if(pFEM->m_szLCName[n]=="ENV MAX")
				nComboSelect = n;
		}
	}
	m_ComboLoadCombo.SetCurSel(nComboSelect);
	DrawInputDomyunView(TRUE);
}

void CInputGenFrameAnalysis::SetLoadCaseCombo()
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

LRESULT CInputGenFrameAnalysis::OnCalcOk(WPARAM wp, LPARAM)
{	
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();

	long nTab = m_ctlTab.GetCurSel();
	if(pBri->m_nSelect2DFrameTool==SOLVE_SAP_6AND7)
	{	
		if(m_bCheckAllBridge)
		{
			long nSizeBridge = m_pIn->m_pARcBridgeDataStd->GetBridgeSize();
			for(long nBridge=0; nBridge<nSizeBridge; nBridge++)
			{	
				CRcBridgeRebar *pBri = m_pIn->m_pARcBridgeDataStd->GetBridgeRebar(nBridge);
				m_pIn->CompleteRunOfSap2000(pBri, nTab);
			}

			if(m_ctlTab.GetCurSel()!=m_ctlTab.GetItemCount()-1 && !m_pIn->m_SapDlg.m_bError)
			{
				UpdateData(FALSE);
				DrawInputDomyunView(TRUE);
				m_ctlTab.SetCurSel(m_ctlTab.GetCurSel()+1);	
				PostMessage(WM_COMMAND,IDC_BUTTON_RUN_TOOL);
			}
			SetLoadComboCombo();
			SetLoadCaseCombo();
			UpdateData(FALSE);
			DrawInputDomyunView(TRUE);
		}
		else
		{		
			if(m_pIn->CompleteRunOfSap2000(pBri, nTab))
			{
				m_pIn->m_FemDraw.SetFlag(FEMVIEW_ELEMENT_FORCE, TRUE);
				m_pIn->m_FemDraw.SetFlag(FEMVIEW_COLORINDEX, TRUE);
				SetLoadComboCombo();
				SetLoadCaseCombo();
				UpdateData(FALSE);
				DrawInputDomyunView(TRUE);
			}
			if(m_ctlTab.GetCurSel()!=m_ctlTab.GetItemCount()-1 && !m_pIn->m_SapDlg.m_bError)
			{
				UpdateData(FALSE);
				DrawInputDomyunView(TRUE);
				m_ctlTab.SetCurSel(m_ctlTab.GetCurSel()+1);	
				PostMessage(WM_COMMAND,IDC_BUTTON_RUN_TOOL);
			}
		}
	}
	else if(pBri->m_nSelect2DFrameTool==SOLVE_NEOMAX)
	{
		if(m_bCheckAllBridge)
		{
			long nSizeBridge = m_pIn->m_pARcBridgeDataStd->GetBridgeSize();
			for(long nBridge=0; nBridge<nSizeBridge; nBridge++)
			{	
				CRcBridgeRebar *pBri = m_pIn->m_pARcBridgeDataStd->GetBridgeRebar(nBridge);
				m_pIn->CompleteRunOfNeoMax(pBri, nTab);
			}
			SetLoadComboCombo();
			SetLoadCaseCombo();
			UpdateData(FALSE);
			DrawInputDomyunView(TRUE);
		}
		else
		{		
			if(m_pIn->CompleteRunOfNeoMax(pBri, nTab))
			{
				m_pIn->m_FemDraw.SetFlag(FEMVIEW_ELEMENT_FORCE, TRUE);
				m_pIn->m_FemDraw.SetFlag(FEMVIEW_COLORINDEX, TRUE);
				SetLoadComboCombo();
				SetLoadCaseCombo();
				UpdateData(FALSE);
				DrawInputDomyunView(TRUE);
			}
			if(m_ctlTab.GetCurSel()!=m_ctlTab.GetItemCount()-1 && !m_pIn->m_SapDlg.m_bError)
			{
				UpdateData(FALSE);
				DrawInputDomyunView(TRUE);
				m_ctlTab.SetCurSel(m_ctlTab.GetCurSel()+1);	
				PostMessage(WM_COMMAND,IDC_BUTTON_RUN_TOOL);
			}
		}
	}
	return 0;
}

void CInputGenFrameAnalysis::OnButtonViewCalc() 
{
	// TODO: Add your control notification handler code here
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	CInputCheckGrid Dlg(pBri,IDD_GEN_FRAME_ANLYSIS);

	Dlg.DoModal();
}

void CInputGenFrameAnalysis::OnSelchangeTabDesignType(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UpdateData(TRUE);
	SetDataInit();
	SetLoadCaseCombo();
	SetLoadComboCombo();
	m_pIn->m_nCurrentSap = m_ctlTab.GetCurSel();
	DrawInputDomyunView(TRUE);

	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();

	if(pBri->IsBoxType())
	{
		m_ctrlCheckUserDispMax.ShowWindow(SW_HIDE);
		m_ctrlEditUserDispMax.ShowWindow(SW_HIDE);
	}
	else
	{
		m_ctrlCheckUserDispMax.ShowWindow((m_pIn->m_nCurrentSap == 1) ? SW_SHOW : SW_HIDE);
		m_ctrlEditUserDispMax.ShowWindow((m_pIn->m_nCurrentSap == 1) ? SW_SHOW : SW_HIDE);
	}
	
	*pResult = 0;
}

void CInputGenFrameAnalysis::OnSelchangingTabDesignType(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}


void CInputGenFrameAnalysis::OnReleasedcaptureSliderScale2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UpdateData();	
	m_pIn->m_FemDraw.SetDispScale(m_ctlDispSlider.GetPos());
	DrawInputDomyunView(TRUE);	
	*pResult = 0;
}
void CInputGenFrameAnalysis::InitTabControl()
{
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	m_ctlTab.DeleteAllItems();
	
	long nTabInsert = 0;
	m_ctlTab.InsertItem(nTabInsert++," 평상시 ");

	CString strTapName(_T(""));
	long nSizeEq = pBri->GetCountEarthQuakeModel(FALSE);
	for(long ix=0; ix<nSizeEq; ++ix)
	{
		CString strEq = pBri->GetNameStringEarthQuakeModel(ix+1);
		strTapName.Format(_T("지진시 : %s"), strEq);
		m_ctlTab.InsertItem(nTabInsert++, strTapName);
	}

	m_ctlTab.SetCurSel(0);
}

BOOL CInputGenFrameAnalysis::IsValid()
{
	return TRUE;
}

void CInputGenFrameAnalysis::OnSelchangeComboToolname() 
{
	// TODO: Add your control notification handler code here
	CRcBridgeRebar *pBri = dynamic_cast<CRcBridgeRebar *>(m_pIn->GetRcBridge());
	if(!pBri) return;

	// 해석툴 선택이 바뀌었다면 데이터 초기화
	if(pBri->m_nSelect2DFrameTool != m_cmbToolName.GetCurSel())
	{
		pBri->m_FemNormal.RemoveAllData();
		long i=0; for(i=0; i<4; i++)
			pBri->m_FemEQ[i].RemoveAllData();
	}
		
	pBri->m_nSelect2DFrameTool	= m_cmbToolName.GetCurSel();

	if(pBri->m_nSelect2DFrameTool == SOLVE_MIDAS)
	{
		pBri->m_bSelectEnvSubsid = TRUE;

		// (40867) 선택된 해석툴이 Midas이고 지반모델링이 NLLINK라면 Spring으로 바꿔준다.
		pBri->m_n2DGeneralSpringType = 1;
		pBri->m_n2DEQSpringType = 1;
	}

	SetDataInit();
}

void CInputGenFrameAnalysis::InitComboCtrlToolName()
{
	CRcBridgeRebar *pBri = dynamic_cast<CRcBridgeRebar *>(m_pIn->GetRcBridge());
	if(!pBri) return;

	long i = 0; for(i = 0; i < m_cmbToolName.GetCount(); i++)
		m_cmbToolName.DeleteString(0);

	m_cmbToolName.AddString("SAP 2000 7");
	m_cmbToolName.AddString("SAP 2000 8");
	m_cmbToolName.AddString("SAP 2000 9");
	m_cmbToolName.AddString("SAP 2000 10");
	m_cmbToolName.AddString("MIDAS Civil");
	
	if(pBri->m_nSelect2DFrameTool > -1)
		m_cmbToolName.SetCurSel(pBri->m_nSelect2DFrameTool);
}

void CInputGenFrameAnalysis::OnCheckUserDispmax() 
{
	CRcBridgeRebar *pBri = dynamic_cast<CRcBridgeRebar *>(m_pIn->GetRcBridge());
	if(!pBri) return;

	pBri->m_bApplyUserDispMax = m_ctrlCheckUserDispMax.GetCheck();
	m_ctrlEditUserDispMax.EnableWindow(pBri->m_bApplyUserDispMax);
}

void CInputGenFrameAnalysis::OnChangeEditDispmax() 
{
	CRcBridgeRebar *pBri = dynamic_cast<CRcBridgeRebar *>(m_pIn->GetRcBridge());
	if(!pBri) return;
	
	CString szValue = _T("");
	m_ctrlEditUserDispMax.GetWindowText(szValue);
	pBri->m_dUnitLoadDispMaxUser = atof(szValue);
}

void CInputGenFrameAnalysis::SetControlUserDisp()
{
	CRcBridgeRebar *pBri = dynamic_cast<CRcBridgeRebar *>(m_pIn->GetRcBridge());
	if(!pBri) return;
	
	CString szDisp = _T("");
	szDisp.Format("%f", pBri->m_dUnitLoadDispMaxUser);
	m_ctrlCheckUserDispMax.SetCheck(pBri->m_bApplyUserDispMax);
	m_ctrlEditUserDispMax.SetWindowText(szDisp);
	m_ctrlEditUserDispMax.EnableWindow(pBri->m_bApplyUserDispMax);
	m_ctrlCheckUserDispMax.ShowWindow(SW_HIDE);
	m_ctrlEditUserDispMax.ShowWindow(SW_HIDE);
}