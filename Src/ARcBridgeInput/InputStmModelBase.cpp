// InputStmModeBase.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ARcBridgeInput.h"
#include "InputStmModelBase.h"
#include "Tlhelp32.h"

#define _STMSOLVER_SAP_ASTM	0
#define _STMSOLVER_ASTM		1
#define _STMSOLVER_SAP2000		2

// CInputStmModelBase 대화 상자입니다.

IMPLEMENT_DYNAMIC(CInputStmModelBase, CInputDialogBase)

	CInputStmModelBase::CInputStmModelBase(UINT nID, CWnd* pParent /*=NULL*/)
	: CInputDialogBase(nID, pParent)
{
	m_pUbRenderer = NULL;
	m_bUbicon = TRUE;
	m_bCheckAllBridge = FALSE;
	m_bCheckAllModel = FALSE;
	m_bCheckAllCombo = FALSE;
	m_pModel = NULL;
	m_nCurModel = 0;
	m_nCurCase = 0;
	m_ixStmKey = -1;
	m_bCheckShowLoad = FALSE;
	m_bCheckShowStrutTie = FALSE;
	m_bCheckShowNodalZone = FALSE;
	m_bCheckShowStressFlow = FALSE;

	m_bRunSapGeneralModel = FALSE;
	m_bEnableStmControl = TRUE;
	m_bEndCalcOK = FALSE;
	m_bRunStad2DForSap2000 = FALSE;
	m_pDlgCheck = new CInputCheckGrid;
	m_pStress = new CStmModel;
}

CInputStmModelBase::~CInputStmModelBase()
{
	delete m_pDlgCheck;
	delete m_pStress;
}

void CInputStmModelBase::DoDataExchange(CDataExchange* pDX)
{
	CInputDialogBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_BRIDGE_NO, m_cbBridgeNo);
	DDX_Control(pDX, IDC_COMBO_LOADCASE, m_ComboLoadCase);
	DDX_Control(pDX, IDC_COMBO_STMCOMBO, m_ComboModel);
	DDX_Control(pDX, IDC_BUTTON_SOLVER, m_btRunSolver);
	DDX_Control(pDX, IDC_BUTTON_RUNSTRESSFLOW, m_btRunStressFlow);
	DDX_Control(pDX, IDC_BUTTON_INPUT, m_btInputSap);
	DDX_Control(pDX, IDC_BUTTON_OUTPUT, m_btOunputSap);
	DDX_Control(pDX, IDC_COMBO_DESIGN_TOOL, m_cbDesignTool);
	DDX_Check(pDX, IDC_CHECK_LOAD, m_bCheckShowLoad);
	DDX_Check(pDX, IDC_CHECK_STRUT_TIE, m_bCheckShowStrutTie);
	DDX_Check(pDX, IDC_CHECK_NODAL_ZONE, m_bCheckShowNodalZone);
	DDX_Check(pDX, IDC_CHECK_STRESS_FLOW, m_bCheckShowStressFlow);
	DDX_Check(pDX, IDC_CHECK_ALLBRIDGE, m_bCheckAllBridge);
	DDX_Check(pDX, IDC_CHECK_ALLCORNER, m_bCheckAllModel);
	DDX_Check(pDX, IDC_CHECK_ALLCOMBO, m_bCheckAllCombo);
}


BEGIN_MESSAGE_MAP(CInputStmModelBase, CInputDialogBase)
	ON_CBN_SELCHANGE(IDC_COMBO_BRIDGE_NO, OnSelchangeComboBridgeNo)
	ON_CBN_SELCHANGE(IDC_COMBO_STMCOMBO, &CInputStmModelBase::OnCbnSelchangeComboStmcombo)
	ON_BN_CLICKED(IDC_BTN_STMSETTING, &CInputStmModelBase::OnBnClickedBtnStmsetting)
	ON_BN_CLICKED(IDC_BTN_DEFAULTMODEL, &CInputStmModelBase::OnBnClickedBtnDefaultmodel)
	ON_BN_CLICKED(IDC_BTN_RENUM, &CInputStmModelBase::OnBnClickedBtnRenum)
	ON_CBN_SELCHANGE(IDC_COMBO_LOADCASE, &CInputStmModelBase::OnCbnSelchangeComboLoadcase)
	ON_BN_CLICKED(IDC_BTN_VIEWOPTION, &CInputStmModelBase::OnBnClickedBtnViewoption)
	ON_BN_CLICKED(IDC_BUTTON_RUNSTRESSFLOW, &CInputStmModelBase::OnBnClickedButtonStressFlow)
	ON_BN_CLICKED(IDC_BUTTON_SOLVER, &CInputStmModelBase::OnBnClickedButtonSolver)
	ON_BN_CLICKED(IDC_BUTTON_INPUT, &CInputStmModelBase::OnBnClickedButtonInput)
	ON_BN_CLICKED(IDC_BUTTON_OUTPUT, &CInputStmModelBase::OnBnClickedButtonOutput)
	ON_BN_CLICKED(IDC_BTN_CHECKDESIGN, &CInputStmModelBase::OnBnClickedBtnCheckdesign)
	ON_BN_CLICKED(IDC_BTN_CHECKMAIN, &CInputStmModelBase::OnBnClickedBtnCheckmain)
	ON_BN_CLICKED(IDC_BTN_CHECKSHEAR, &CInputStmModelBase::OnBnClickedBtnCheckshear)
	ON_CBN_SELCHANGE(IDC_COMBO_DESIGN_TOOL, &CInputStmModelBase::OnCbnSelchangeComboDesignTool)
	ON_BN_CLICKED(IDC_CHECK_LOAD, &CInputStmModelBase::OnBnClickedCheckLoadtype)
	ON_BN_CLICKED(IDC_CHECK_STRUT_TIE, &CInputStmModelBase::OnBnClickedCheckStrutTie)
	ON_BN_CLICKED(IDC_CHECK_NODAL_ZONE, &CInputStmModelBase::OnBnClickedCheckNodalZone)
	ON_BN_CLICKED(IDC_CHECK_STRESS_FLOW, &CInputStmModelBase::OnBnClickedCheckStressFlow)
	ON_MESSAGE(WM_CALC_OK, OnCalcOk)
	ON_MESSAGE(WM_CALC_STRESS_FLOW_OK, OnCalcOkStressFlow)
	ON_MESSAGE(WM_CALC_STAD2D_OK, OnCalcOkStad2D)

	ON_BN_CLICKED(IDC_CHECK_ALLBRIDGE, &CInputStmModelBase::OnBnClickedCheckAllbridge)
	ON_BN_CLICKED(IDC_CHECK_ALLCORNER, &CInputStmModelBase::OnBnClickedCheckAllModel)
	ON_BN_CLICKED(IDC_CHECK_ALLCOMBO, &CInputStmModelBase::OnBnClickedCheckAllcombo)
END_MESSAGE_MAP()


// CInputStmModelBase 메시지 처리기입니다.


void CInputStmModelBase::OnPreInitDialog()
{
	GetDlgItem(IDC_BUTTON_RECOMMAND)->SetWindowText(hggettext("기본값(&D)"));
	GetDlgItem(IDC_BUTTON_NEXT)->SetWindowText(hggettext("다음(&N)"));
	GetDlgItem(IDC_BUTTON_APPLY)->SetWindowText(hggettext("적용(&A)"));
	GetDlgItem(IDC_BUTTON_PREV)->SetWindowText(hggettext("이전(&P)"));

	SetResize(IDC_COMBO_BRIDGE_NO, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_SPIN_ARCBRIDGE, SZ_TOP_LEFT, SZ_TOP_LEFT);;
 	SetResize(IDC_COMBO_STMCOMBO, SZ_TOP_LEFT, SZ_TOP_LEFT);
	SetResize(IDC_SPIN_STMCOMBO, SZ_TOP_LEFT, SZ_TOP_LEFT);
 	SetResize(IDC_COMBO_LOADCASE, SZ_TOP_LEFT, SZ_TOP_LEFT);
 	SetResize(IDC_SPIN_LOADCASE, SZ_TOP_LEFT, SZ_TOP_LEFT);
 	SetResize(IDC_STATIC_CONDITON, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
 	SetResize(IDC_CHECK_ALLBRIDGE, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
 	SetResize(IDC_CHECK_ALLCORNER, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
 	SetResize(IDC_CHECK_ALLCOMBO, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
 	SetResize(IDC_COMBO_DESIGN_TOOL, SZ_TOP_RIGHT, SZ_TOP_RIGHT);
 	SetResize(IDC_BUTTON_SOLVER, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
 	SetResize(IDC_BUTTON_RUNSTRESSFLOW, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
 	SetResize(IDC_BUTTON_OUTPUT, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
 	SetResize(IDC_BUTTON_INPUT, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BTN_CHECKDESIGN, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BTN_CHECKMAIN, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BTN_CHECKSHEAR, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_STATIC_VIEWCONTROL, SZ_TOP_LEFT, SZ_TOP_LEFT);

 	m_pUbRenderer = m_pIn->GetUbRenderer();

	InitModelCombo(0);

	InitComboCtrlTools();
	SetSelectModel();
	SetLoadCaseCombo();
}

void CInputStmModelBase::OnSelchangeComboBridgeNo()
{
	m_pIn->m_nCurrentBridge = m_cbBridgeNo.GetCurSel();
	SetDataInit();
	SetPaneTitle();
	m_cbBridgeNo.SetFocus();

	InitModelCombo(0);
	SetSelectModel();
	SetLoadCaseCombo();

	DrawInputDomyunView();
}


void CInputStmModelBase::OnCbnSelchangeComboStmcombo()
{
	SetDataSave();
	m_nCurModel = m_ComboModel.GetCurSel();

	if(m_nIDD == IDD_STM_FOORING_STARTEND || m_nIDD == IDD_STM_FOORING_MID)
	{
		InitModelCombo(1);
	}
	
	SetCurModelKey();
	SetSelectModel();
	SetLoadCaseCombo();
	DrawInputDomyunView();
}


void CInputStmModelBase::OnBnClickedBtnStmsetting()
{
	CDialogSTMSetting dlg;
	dlg.m_pIn = m_pIn;
	dlg.m_ixStmKey = m_ixStmKey;
	
	if(dlg.DoModal() == IDOK)
	{

	}
}


void CInputStmModelBase::OnBnClickedBtnDefaultmodel()
{
	if (!m_pModel)
		return;

	if(AfxMessageBox(_T("STM 해석 모델링이 재성성됩니다. \n계속하시겠습니까?"), MB_YESNO | MB_ICONQUESTION) == IDYES)
	{
		if(AfxMessageBox(_T("모든하중조합에 대해 적용하시겠습니까?"), MB_YESNO | MB_ICONQUESTION) == IDYES)
		{
			CRcBridgeApp *pBri = m_pIn->GetRcBridge();
			if(pBri == NULL) return;

			long ixKeyStt(-1), ixKeyEnd(-1);
			if(m_ixStmKey >= STM_MODEL_BRACKET_STT && m_ixStmKey <= STM_MODEL_BRACKET_END)
			{
				ixKeyStt = STM_MODEL_BRACKET_STT;
				ixKeyEnd = STM_MODEL_BRACKET_END;
			}
			else if((m_ixStmKey >= STM_MODEL_CORNER_STT_UPPER_MINUS && m_ixStmKey <= STM_MODEL_CORNER_END_LOWER_PLUS) || (m_ixStmKey >= STM_MODEL_CORNER_STT_UPPER_MINUS_EXT_1 && m_ixStmKey <= STM_MODEL_CORNER_END_LOWER_PLUS_EXT_2))
			{
				// 우각부 모델의 Key값은 연속되지 않는다.
				ixKeyStt = STM_MODEL_CORNER_STT_UPPER_MINUS;
				ixKeyEnd = STM_MODEL_CORNER_END_LOWER_PLUS_EXT_2;
			}
			else if(m_ixStmKey >= STM_MODEL_FOOT_STT_MAXP && m_ixStmKey <= STM_MODEL_FOOT_END_EARTH)
			{
				ixKeyStt = STM_MODEL_FOOT_STT_MAXP;
				ixKeyEnd = STM_MODEL_FOOT_END_EARTH;
			}
			else if(m_ixStmKey >= STM_MODEL_MIDFOOT_HEAD && m_ixStmKey < STM_MODEL_CORNER_STT_UPPER_MINUS_EXT_1)
			{
				ixKeyStt = STM_MODEL_MIDFOOT_HEAD;
				ixKeyEnd = STM_MODEL_MIDFOOT_HEAD + pBri->GetCountInWall() * 4;
			}
			else if(m_ixStmKey >= STM_MODEL_FOOT_STT_MAX_AXIAL && m_ixStmKey <= STM_MODEL_FOOT_END_MIN_HORI_FORCE_EXT_2)
			{
				// (ARCBRIDGE-3250) 시종점 같이 초기화
				ixKeyStt = STM_MODEL_FOOT_STT_MAX_AXIAL;
				ixKeyEnd = STM_MODEL_FOOT_END_MIN_HORI_FORCE_EXT_2;
			}
			else if(m_ixStmKey >= STM_MODEL_FOOT_MID_MAX_AXIAL)
			{
				ixKeyStt = STM_MODEL_FOOT_MID_MAX_AXIAL;
				ixKeyEnd = ixKeyStt + pBri->GetCountInWall() * 15;
			}

			for(long ix=ixKeyStt; ix<=ixKeyEnd; ++ix)
			{
				// 우각부 모델의 Key값은 연속되지 않는다.
				if(ixKeyStt == STM_MODEL_CORNER_STT_UPPER_MINUS && ix > STM_MODEL_CORNER_END_LOWER_PLUS && ix < STM_MODEL_CORNER_STT_UPPER_MINUS_EXT_1)
					continue;

				CStmModelForArcbridge *pModel = static_cast<CStmModelForArcbridge*>(pBri->GetStmModelByKey(ix));
				if(pModel == NULL) continue;

				pModel->SetLock(FALSE);
				pModel->SetStmModifiedFlag(FALSE);
				if(InitStmModel(pModel) == FALSE)
				{
					continue;
				}
			}
		}
		else
		{
			m_pModel->SetLock(FALSE);
			m_pModel->SetStmModifiedFlag(FALSE);
			InitStmModel(m_pModel);
		}

		SetDataInit();
  		DrawInputDomyunView();
	}
}


void CInputStmModelBase::OnBnClickedBtnRenum()
{
	CStmUbManager *pMng = m_pUbRenderer->GetUbManager();
	if(pMng == NULL) return;

	if (!m_pModel)
		return;

	m_pModel->ReSetIdNodeAndElement(TRUE);

	ClearHistoryAndDeletedObject();

	pMng->UpdateStmRenderer();

	DrawInputDomyunView();
}


void CInputStmModelBase::OnCbnSelchangeComboLoadcase()
{
	DrawInputDomyunView();
}


void CInputStmModelBase::OnBnClickedBtnViewoption()
{
	CStmUbManager *pMng = m_pUbRenderer->GetUbManager();
	if(pMng == NULL) return;

	pMng->ShowDlgViewControl();
}


void CInputStmModelBase::OnBnClickedButtonStressFlow()
{
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;

	UpdateData();

	CString szPath = AfxGetApp()->GetProfileString("Afx Directory", "Fixed Directory");

	long nSizeBri = pStd->GetBridgeSize();
	long nBriStt = m_bCheckAllBridge ? 0 :  m_pIn->m_nCurrentBridge;
	long nBriEnd = m_bCheckAllBridge ? nSizeBri - 1 :  m_pIn->m_nCurrentBridge;

	long nCurrentBri = m_pIn->m_nCurrentBridge;
	long nCurrentKey = m_ixStmKey;

	for(long nBri = nBriStt; nBri <= nBriEnd; nBri++)
	{
		m_pIn->m_nCurrentBridge = nBri;
	
		// clear StressFlow
		CRcBridgeApp *pBri = pStd->GetBridge(nBri);

		std::vector<HGINT32> vKeys;
		if( pStd->IsLsdDesign() )
			GetModelKeysForAnalysis_Lsd(nBri, vKeys);
		else
			GetModelKeysForAnalysis(nBri, vKeys);

		long icModel = vKeys.size();

		for(HGINT32 ix=0; ix<icModel; ++ix)
		{
			m_ixStmKey = vKeys[ix];

			m_pStress->ClearStm(TRUE);
			m_pStress->ClearStressFlow();
			m_pStress->SetModelingType(eStmStressFlow);

			MakeStmStressFlowData(m_pStress, nBri, vKeys[ix]);
			SetStmAreaByInput(m_pStress);

			// clear, delete
			MakeStressFlowInputData(szPath);
			// 해석

			m_bEndCalcOK = FALSE;
			SetCurrentDirectory(szPath); // 현재 디렉토리를 설정
			m_CommandDlg.m_pMainWnd = this;
			m_CommandDlg.m_nTypeRun = 0;
			if(!m_CommandDlg.GetSafeHwnd()) 
			{
				m_CommandDlg.Create(this->GetParent(), this, 0);
			}
			CString szPos = pBri->m_strBridgeName + _T("_") + pBri->GetStringStmModelNameByKey(vKeys[ix]);
			CString szTitle = _T("");
			szTitle.Format(_T("주응력 흐름%s : pconvert Run..."), szPos);
			m_CommandDlg.SetTitle(szTitle);
			m_CommandDlg.SetWindowText(hggettext("pconvert 모듈 실행창"));
			m_CommandDlg.SetBtnText(hggettext("닫 기"));
			m_CommandDlg.CenterWindow();
			m_CommandDlg.ShowWindow(SW_SHOW);	
			m_CommandDlg.m_ctlList.ResetContent();
			m_CommandDlg.m_bAutoTerminal = TRUE;
			if(pBri->m_pARcBridgeDataStd->IsLsdDesign())
			{
				m_CommandDlg.AddCommand(STDIO_OPEN, szPath + _T("\\pconvertLsd.exe "));
			}
			else
			{
				m_CommandDlg.AddCommand(STDIO_OPEN, szPath + _T("\\pconvertUsd.exe "));
			}

			// 임시방편으로 처리
			while(m_bEndCalcOK==FALSE/* && m_CommandDlg.IsWindowVisible()*/)
			{
				AfxGetThread()->PumpMessage();
			}
			Sleep(1000);
			// 임시방편으로 처리
		}
	}
	
	m_pIn->m_nCurrentBridge = nCurrentBri;
	m_ixStmKey	= nCurrentKey;

	m_bCheckShowStressFlow = TRUE;
	UpdateData(FALSE);

	DrawInputDomyunView();
}


void CInputStmModelBase::OnBnClickedButtonSolver()
{
	CStmUbManager *pMng = m_pUbRenderer->GetUbManager();
	if(pMng == NULL) return;

	if(m_pModel == NULL) return;

	CStmDataConfig *pConfig = m_pModel->GetStmDataConfig();
	long nSelectMaxEffectiveWidth = pConfig->m_ixTypeCalcMaxWidth;

	UpdateData();

	HGBOOL bLock = m_pModel->IsLock();

	ClearHistoryAndDeletedObject();

 	long nSolverTool = GetSelected2DFrameTool();
	if(nSolverTool == _STMSOLVER_ASTM)
	{
		m_bRunStad2DForSap2000 = FALSE;
		RunStad2D();
		return;
	}

	m_bRunStad2DForSap2000 = FALSE;
	if(nSolverTool == _STMSOLVER_SAP_ASTM)
	{
		m_bRunStad2DForSap2000 = TRUE;
		RunStad2D();
	}

	if(nSelectMaxEffectiveWidth == 1 && nSolverTool == _STMSOLVER_SAP2000) //차원화법 이면서 Sap2000인경우.
	{
		m_bRunStad2DForSap2000 = TRUE;
		RunStad2D();
	}

	if(nSolverTool == _STMSOLVER_SAP2000 || nSolverTool == _STMSOLVER_SAP_ASTM)
	{
		m_pIn->m_sSapFileNameArray.RemoveAll();

		if(m_bCheckAllBridge)
		{
			if(!bLock || m_bRunStad2DForSap2000)
			{
				long nCntBri = m_pIn->m_pARcBridgeDataStd->GetBridgeSize();
				for(long nBri = 0; nBri < nCntBri; nBri++)
				{
					std::vector<HGINT32> vKeys;
					GetModelKeysForAnalysis(nBri, vKeys);

					long icModel = vKeys.size();

					for(HGINT32 ix=0; ix<icModel; ++ix)
					{
						MakeStmInputData(nBri, vKeys[ix], TRUE, !m_bRunStad2DForSap2000);
						
						pMng->UpdateStmRenderer();
					}
				}

				m_pIn->RunSap2000All(this, m_ixStmKey);
			}
		}
		else
		{
			CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
			if(pBri == NULL) return;

			if(!bLock || m_bRunStad2DForSap2000)
			{
				std::vector<HGINT32> vKeys;
				GetModelKeysForAnalysis(m_pIn->m_nCurrentBridge, vKeys);

				long icModel = vKeys.size();

				for(HGINT32 ix=0; ix<icModel; ++ix)
				{
					MakeStmInputData(m_pIn->m_nCurrentBridge, vKeys[ix], TRUE, !m_bRunStad2DForSap2000);

					pMng->UpdateStmRenderer();
				}

				if(icModel == 1)
					m_pIn->RunSap2000(pBri, m_ixStmKey,this);
				else
					m_pIn->RunSap2000All(this, m_ixStmKey);
			}
			pMng->UpdateStmRenderer();
		}
	}
}


void CInputStmModelBase::OnBnClickedButtonInput()
{
	UpdateData();
	if(m_pModel == NULL) return;

	CWaitCursor cursor;

	HGBOOL bLock = m_pModel->IsLock();
	HGBOOL bModified = m_pModel->IsStmModified();
	if(GetSelected2DFrameTool() != _STMSOLVER_ASTM)
	{
		if(!bLock || bModified)
		{
			m_pModel->SetModelStmToFemManager();
		}

		CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
		m_pIn->MakeInputData(pBri, m_ixStmKey, _T(""), SOLVE_SAP_6AND7);
	}
	else
	{
		MakeInputStad2D();
	}
}


void CInputStmModelBase::OnBnClickedButtonOutput()
{
	CStmUbManager *pMng = m_pUbRenderer->GetUbManager();
	if(pMng == NULL) return;

	if(m_pModel == NULL) return;

	long nSolverTool = GetSelected2DFrameTool();
	if(nSolverTool != _STMSOLVER_ASTM)
	{
		CFEMManage *pFem = m_pModel->GetFemManager();
		if(pFem == NULL) return;

		CString szOutExt;
		CString szComment;
		szOutExt  = _T("*.out|*.*");
		szComment.Format(_T("Sap2000 Output File(*.out)|*.out|%s (*.*)|*.*||"),hggettext("모든파일"));

		MakeStmInputData(m_pIn->m_nCurrentBridge, m_ixStmKey, FALSE, FALSE);
		pMng->UpdateStmRenderer();

		CString szPath;
		CFileDialog dlg(TRUE, szOutExt, _T(""), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szComment, m_pView);
		dlg.m_ofn.lpstrTitle = _T("SAP 결과파일 불러오기");
		dlg.m_ofn.lpstrInitialDir = m_pIn->GetPathDirectory();

		if (dlg.DoModal() == IDOK)
		{
			szPath = dlg.GetPathName();
		}
		else
		{
			return;
		}

		CDlgProgress *pProgressDlg = new CDlgProgress;
		if(!pProgressDlg->GetSafeHwnd())
		{
			pProgressDlg->Create(IDD_DIALOG_HGPROGRESS, this);
		}

		CString szInExt = _T(".s2k");

		HGTCHAR	drive[_MAX_DRIVE];
		HGTCHAR	dir[_MAX_DIR];
		HGTCHAR	fname[_MAX_FNAME];
		HGTCHAR	ext[_MAX_EXT];
		HGTCHAR	inputname[_MAX_EXT];

		::hgsplitpath(szPath, drive, _countof(drive), dir, _countof(dir), fname, _countof(fname), ext, _countof(ext));
		::hgstrcpy(inputname, _countof(inputname), drive);
		::hgstrcpy(inputname, _countof(inputname), dir);
		::hgstrcpy(inputname, _countof(inputname), fname);
		::hgstrcpy(inputname, _countof(inputname), szInExt);

		CString szInputPath(inputname);
		CString sMsg = _T("");

		try
		{
			pProgressDlg->ShowWindow(SW_SHOW);
			pProgressDlg->CenterWindow();
			pProgressDlg->InitProgressData();

			CFEMLoadSAP2000Output LoadOut(pFem);
			if(LoadOut.LoadData(szPath))
			{
	//			SetLoadComboCombo();
				SetLoadCaseCombo();
				UpdateData(FALSE);

				m_pModel->SetResultStmFromFemManager();
				m_pModel->GetFemManager()->m_bLoadResult = TRUE;

				DrawInputDomyunView(TRUE);
			}
		}
		catch(...)
		{
			sMsg.Format(hggettext("%s파일이 비정상적이거나 존재하지 않습니다."), szPath);
			AfxMessageBox(sMsg, MB_ICONSTOP);
		}

		SAFE_DELETE(pProgressDlg);
	}
	else
	{
		ReadOututStad2D();
	}

	SetLoadCaseCombo();
//	SetLoadComboCombo();

	m_bCheckShowStressFlow = FALSE;
	UpdateData(FALSE);

	DrawInputDomyunView();
}


void CInputStmModelBase::OnBnClickedBtnCheckdesign()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_pModel)
		return;

	if(m_pModel->GetFemManager()->IsOutputLoaded() == FALSE)
	{
		AfxMessageBox(_T("STM Frame 해석 후에 검토하여 주시기 바랍니다."));
		return;
	}

	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	m_pDlgCheck->SetBriAndContent(pBri,GetIDD(),0);
	m_pDlgCheck->SetStmModel(m_pModel);
	m_pDlgCheck->ShowAndCheck(this);
}


void CInputStmModelBase::OnBnClickedBtnCheckmain()
{
	CDialogSTMArcCornerMain dlg;
	dlg.m_pIn = m_pIn;
	dlg.m_ixStmKey = m_ixStmKey;
	if(dlg.DoModal() == IDOK)
	{
		if(dlg.m_bChange)
			AfxMessageBox(hggettext("철근배근이 수정되었습니다. STM해석을 다시 진행하시기 바랍니다."), MB_OK);
	}
}


void CInputStmModelBase::OnBnClickedBtnCheckshear()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CInputStmModelBase::OnCbnSelchangeComboDesignTool()
{
	if(AfxMessageBox(_T("모든 구조물의 해석결과가 초기화됩니다.\n계속 하시겠습니까 ?"), MB_YESNO) == IDNO)
	{
		long nSelect = m_cbDesignTool.GetCurSel();
		m_cbDesignTool.SetCurSel(nSelect == 0 ? 1 : 0);
		UpdateData(FALSE);

		return;
	}

	UpdateData(TRUE);

	SetSelected2DFrameTool(m_cbDesignTool.GetCurSel());

	InitComboCtrl();
	DrawInputDomyunView();
}

void CInputStmModelBase::SetDataSave()
{
	UpdateData();
}

void CInputStmModelBase::SetDataInit()
{
	InitComboCtrl();
	SetLoadCaseCombo();
}

void CInputStmModelBase::SetLoadCaseCombo()
{
	if (!m_pModel)
		return;

	m_ComboLoadCase.ResetContent();

	std::vector<CString> vstrLoadName;
	m_pModel->GetNameListLoadCondition(vstrLoadName);

	std::vector<CString>::iterator pos;
	for(pos = vstrLoadName.begin(); pos != vstrLoadName.end(); ++pos)
	{
		m_ComboLoadCase.AddString(*pos);
	}

	HGINT32 icStatic = m_pModel->GetCountLoadConditionStatic();
	HGINT32 ixSelect = icStatic == vstrLoadName.size() ? 0 : icStatic;
	m_ComboLoadCase.SetCurSel(ixSelect);
}

void CInputStmModelBase::InitComboCtrl()
{
	m_btRunSolver.SetWindowText(hggettext("해석"));
	m_btInputSap.SetWindowText(hggettext("Input"));
	m_btOunputSap.SetWindowText(hggettext("Output"));

	SetLoadCaseCombo();

	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;

	m_cbBridgeNo.ResetContent();

	CString str = _T("");
	HGINT32 nSize = pStd->GetBridgeSize();
	for(HGINT32 ix = 0; ix < nSize; ix++)
	{
		CRcBridgeApp *pBri = pStd->GetBridge(ix);
		str = pStd->GetStringBridgeOrder(ix) + _T(" : ") + pBri->m_strBridgeName
			+ _T(" [") + pBri->GetStringBridgeType() + _T("]");
		m_cbBridgeNo.AddString(str);
	}
	m_cbBridgeNo.SetCurSel(m_pIn->m_nCurrentBridge);
}

void CInputStmModelBase::ClearHistoryAndDeletedObject()
{
	CStmUbManager *pMng = m_pUbRenderer->GetUbManager();
	if(pMng == NULL) return;

	pMng->ClearHistory();
	pMng->UpdateStmRenderer();
	m_pModel->ClearDeletedObject();
}

void CInputStmModelBase::RunStad2D()
{
	CStmUbManager *pMng = m_pUbRenderer->GetUbManager();
	if(pMng == NULL) return;

	CStmDataConfig *pConfig = m_pModel->GetStmDataConfig();
	if(pConfig == NULL) return;

	CDlgProgress *pProgressDlg = new CDlgProgress;
	long nIdxProgress = 0;
	CString szPath = AfxGetApp()->GetProfileString("Afx Directory", "Fixed Directory");
	long nSolverTool = GetSelected2DFrameTool();
	long nSelectMaxEffectiveWidth = pConfig->m_ixTypeCalcMaxWidth;
	BOOL bHide = (nSelectMaxEffectiveWidth == 1) && (nSolverTool == _STMSOLVER_SAP2000);

	long nSizeBri = m_pIn->m_pARcBridgeDataStd->GetBridgeSize();
	long ixStt = m_bCheckAllBridge ? 0 :  m_pIn->m_nCurrentBridge;
	long ixEnd = m_bCheckAllBridge ? nSizeBri - 1 :  m_pIn->m_nCurrentBridge;

	long nCurrentBridge = m_pIn->m_nCurrentBridge;
	long nCurrentKey = m_ixStmKey;

	//구조물별 조합 총 개수
	long nTotal = (ixEnd - ixStt) + 1;
	
	double dOffsetProgress = 100. /  nTotal;
	if(!pProgressDlg->GetSafeHwnd())
	{
		pProgressDlg->Create(IDD_DIALOG_HGPROGRESS, this);
	}

	pProgressDlg->ShowWindow(bHide? SW_HIDE : SW_SHOW);
	pProgressDlg->CenterWindow();
	pProgressDlg->InitProgressData();

	for(long nBri = ixStt; nBri <= ixEnd; nBri++)
	{
		CRcBridgeRebar *pBri = m_pIn->m_pARcBridgeDataStd->GetBridgeRebar(nBri);
		if(pBri == NULL) continue;

		m_pIn->m_nCurrentBridge = nBri;

		std::vector<HGINT32> vKeys;
		GetModelKeysForAnalysis(nBri, vKeys);

		long icModel = vKeys.size();

		for(HGINT32 ix=0; ix<icModel; ++ix)
		{
			m_ixStmKey = vKeys[ix];

			CStmModel *pStm = pBri->GetStmModelByKey(m_ixStmKey);
			if(pStm == NULL) continue;

			MakeStmInputData(nBri, m_ixStmKey, TRUE, TRUE);


			m_bEndCalcOK = FALSE;
			SetCurrentDirectory(szPath); // 현재 디렉토리를 설정
			m_CommandDlg.m_pMainWnd = this;
			m_CommandDlg.m_nTypeRun = 1;
			if(!m_CommandDlg.GetSafeHwnd()) 
			{
				m_CommandDlg.Create(this->GetParent(), this, 1);
			}
			CString szPos = pBri->m_strBridgeName + _T("_") + pBri->GetStringStmModelNameByKey(m_ixStmKey);
			CString szTitle = _T("");
			szTitle.Format(_T("  %s : tconvert Run..."), szPos); 

			m_CommandDlg.SetTitle(szTitle);
			m_CommandDlg.SetWindowText(hggettext("tconvert 모듈 실행창"));
			m_CommandDlg.SetBtnText(hggettext("닫 기"));
			m_CommandDlg.CenterWindow();
			m_CommandDlg.ShowWindow(bHide? SW_HIDE : SW_SHOW);	
			m_CommandDlg.m_ctlList.ResetContent();
			m_CommandDlg.m_bAutoTerminal = TRUE;

			if(pBri->m_pARcBridgeDataStd->IsLsdDesign())
			{
				m_CommandDlg.AddCommand(STDIO_OPEN, szPath + _T("\\tconvertLsd.exe "));
			}
			else
			{
				m_CommandDlg.AddCommand(STDIO_OPEN, szPath + _T("\\tconvertUsd.exe "));
			}

			pProgressDlg->SetProgressPos(szTitle, (long)dOffsetProgress * nIdxProgress++);

			// 임시방편으로 처리
			while(m_bEndCalcOK==FALSE/* && m_CommandDlg.IsWindowVisible()*/)
			{
				AfxGetThread()->PumpMessage();
			}
			Sleep(1000);
			// 임시방편으로 처리
		}
	}

	pProgressDlg->SetProgressPos(_T("계산완료"), 100);
	SAFE_DELETE(pProgressDlg);

	pMng->UpdateStmRenderer();

	m_pIn->m_nCurrentBridge = nCurrentBridge;
	m_ixStmKey	= nCurrentKey;

	m_bCheckShowStressFlow = FALSE;
	UpdateData(FALSE);

	DrawInputDomyunView();
}

void CInputStmModelBase::InitComboCtrlTools()
{
	for(HGINT32 ix = 0; ix < m_cbDesignTool.GetCount(); ix++)
	{
		m_cbDesignTool.DeleteString(0);
	}
	m_cbDesignTool.ResetContent();

	m_cbDesignTool.AddString(_T("SAP+ASTM"));
	m_cbDesignTool.AddString(_T("ASTM 1.0"));
	m_cbDesignTool.AddString(SOLVE_SAP6AND7_STRING);

	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	if(pBri)
	{
		m_cbDesignTool.SetCurSel(pBri->m_ixSolverStmTool);
	}
}

void CInputStmModelBase::SetSelectModel()
{
	HGINT32 iKey = -1;

	if(GetDlgItem(IDC_COMBO_STMCOMBO_CHECK))
	{
		if(m_nCurCase >= 0 && m_nCurCase < static_cast<HGINT32>(m_vKeyCase.size()))
		{
			iKey = m_vKeyCase[m_nCurCase];
		}
	}
	else
	{
		HGINT32 iIndex = m_ComboModel.GetCurSel();
		if(iIndex >= 0 && iIndex < static_cast<HGINT32>(m_vKeyModel.size()))
		{
			iKey = m_vKeyModel[iIndex];
		}
	}

	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	m_pModel = pBri->GetStmModelByKey(iKey);

	InitStmModel(m_pModel);
	return;
}

void CInputStmModelBase::DrawInputDomyunView( BOOL bZoomAll/*=TRUE*/ )
{
	if(m_pView != NULL)
	{
		CDomyun *pDom = m_pView->GetDomyun();
		pDom->ClearEtt(TRUE, FALSE);
	}

	CStmUbManager *pMng = m_pUbRenderer->GetUbManager();
	if(pMng == NULL) return;

	if(m_pModel == NULL)
	{
		pMng->SetStmModel(NULL);
		CInputDialogBase::DrawInputDomyunView(bZoomAll);
		return;
	}

	CFEMManage *pFem = m_pModel->GetFemManager();
	if(pFem == NULL) return;

	CStmDataView *pDataView = m_pModel->GetStmDataView();
	if(pDataView == NULL) return;

	UpdateData();

	HGINT32 m_iResultType = 0;


	if(m_bCheckShowLoad)
	{
		HGINT32 ixLoadCase = m_ComboLoadCase.GetCurSel();
		pMng->SetIndexLoadConditionLoad(ixLoadCase);
	}

	HGINT32 icStatic = m_pModel->GetCountLoadConditionStatic();
	HGINT32 ixLoadCombo = m_ComboLoadCase.GetCurSel();
	if(ixLoadCombo >= icStatic)
	{
		ixLoadCombo = icStatic + (ixLoadCombo - icStatic) * 2;
	}
	pMng->SetIndexLoadConditionResult(ixLoadCombo);

	EStmTypeResult eTypeResult = static_cast<EStmTypeResult>(m_iResultType);
	pDataView->SetTypeResult(eTypeResult);

	HGBOOL bOutput(pFem->IsOutputLoaded());
	pDataView->SetShowOption(STM_SHOW_RESULT_TRUSS, bOutput);
	pDataView->SetShowOption(STM_SHOW_RESULT_TRUSS_VALUE, bOutput);
	pDataView->SetShowOption(STM_SHOW_LOAD, m_bCheckShowLoad);
	pDataView->SetShowOption(STM_SHOW_LOAD_VALUE, m_bCheckShowLoad);
	pDataView->SetShowOption(STM_SHOW_EFFECT_WIDTH, m_bCheckShowStrutTie);
	// (ARCBRIDGE-3110) 'Strut/Tie' 버튼에 같이 동작하도록 요청
	pDataView->SetShowOption(STM_SHOW_EFFECT_WIDTH_MAX, m_bCheckShowStrutTie);
	pDataView->SetShowOption(STM_SHOW_NODAL_ZONE, m_bCheckShowNodalZone);
	pDataView->SetShowOption(STM_SHOW_RESULT_STRESS_FLOW, m_bCheckShowStressFlow);
	pDataView->SetShowOption(STM_SHOW_MESH, m_bCheckShowStressFlow);

	CStmDlgViewControl *pViewControl = pMng->GetDlgViewControl();
	if(pViewControl != NULL)
	{
		pViewControl->SetDataInit();
	}

	CInputDialogBase::DrawInputDomyunView(bZoomAll);
}

BOOL CInputStmModelBase::InitStmModel(CStmModel *pModel)
{
	if(pModel == NULL) return FALSE;
	
	if(!pModel->IsLock() && m_nIDD != IDD_STM_BRACKET)
	{
		CRcBridgeRebar* pBri = m_pIn->GetRcBridgeRebar();
		if(pBri == NULL) return FALSE;

		CFEMManage *pFEM = pBri->GetFemManage(COMBO_ULTIMATE);

		if(pFEM == NULL || pFEM->IsOutputLoaded() == FALSE)
		{
			AfxMessageBox(_T("2D Frame 해석결과가 없으면 Model에 하중이 재하되지 않습니다.\n2D Frame 해석 수행 후 진행하시기 바랍니다."));
			return FALSE;
		}
	}

	if(pModel == m_pModel)
	{
		SetCurrentStmModel();
	}
	else
	{
		SetStmAreaByInput(pModel);
	}

	return TRUE;
}

BOOL CInputStmModelBase::SetCurrentStmModel()
{
	CStmUbManager *pMng = m_pUbRenderer->GetUbManager();
	if(pMng == NULL) return FALSE;

	pMng->SetViewBeginning(FALSE);

	pMng->SetStmModel(m_pModel);
	pMng->SetActiveStmModel(TRUE);
	m_pUbRenderer->SetAction(ACTION_STM_NORMAL);

	SetStmAreaByInput(m_pModel);

	m_pUbRenderer->ZoomAll();
	pMng->UpdateStmRenderer();

	CStmDataView *pDataView = m_pModel->GetStmDataView();
	if(pDataView != NULL)
	{
		CVector clrBlack(0, 0, 0), clrWhite(1, 1, 1);
		pDataView->SetColorSetBackground(eStmColorDark, clrBlack, clrBlack);
		pDataView->SetColorSetBackground(eStmColorLight, clrWhite, clrWhite);
	}

	m_pUbRenderer->SetViewPos(CHgModelRenderWnd::V_XZ);

	return TRUE;
}

void CInputStmModelBase::SetCurModelKey( )
{
	if(m_nIDD == IDD_STM_FOORING_STARTEND || m_nIDD == IDD_STM_FOORING_MID)
	{
		m_ixStmKey = (static_cast<HGINT32>(m_vKeyCase.size()) > m_nCurCase)? m_vKeyCase[m_nCurCase] : -1;
	}
	else
	{
		m_ixStmKey = (static_cast<HGINT32>(m_vKeyModel.size()) > m_nCurModel)? m_vKeyModel[m_nCurModel] : -1;
	}
}

void CInputStmModelBase::OnBnClickedCheckLoadtype()
{
	DrawInputDomyunView();
}

void CInputStmModelBase::OnBnClickedCheckStrutTie()
{
	DrawInputDomyunView();
}

void CInputStmModelBase::OnBnClickedCheckNodalZone()
{
	DrawInputDomyunView();
}

void CInputStmModelBase::OnBnClickedCheckStressFlow()
{
	DrawInputDomyunView();
}

void CInputStmModelBase::SetSelected2DFrameTool( long nSolverTool )
{
	// 모든 구조물을 같게 설정
	long nSizeBri = m_pIn->m_pARcBridgeDataStd->GetBridgeSize();
	for(long ix = 0; ix < nSizeBri; ix++)
	{
		CRcBridgeApp *pBri = m_pIn->m_pARcBridgeDataStd->GetBridge(ix);
		if(pBri == NULL) continue;

		pBri->m_ixSolverStmTool = nSolverTool;

		// 해석결과 초기화
		std::map<int, CStmModelForArcbridge*>::iterator mIter = pBri->m_mStmSolver.begin();
		for(mIter ; mIter !=  pBri->m_mStmSolver.end(); ++mIter)
		{
			mIter->second->SetLock(FALSE);
		}
	}
}

long CInputStmModelBase::GetSelected2DFrameTool()
{
	CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();
	if(pBri == NULL) return 0;

	return pBri->m_ixSolverStmTool;
}

void CInputStmModelBase::MakeStmInputData( HGINT32 ixBridge, HGINT32 ixDesignType, BOOL bRenumberModel, BOOL bRemakeModel )
{
	CStmUbManager *pMng = m_pUbRenderer->GetUbManager();
	if(pMng == NULL) return;

	CRcBridgeRebar *pBri = m_pIn->m_pARcBridgeDataStd->GetBridgeRebar(ixBridge);
	if(pBri == NULL) return;

	CStmModel *pStm = pBri->GetStmModelByKey(ixDesignType);
	if(pStm == NULL) return;

	CString szPath = AfxGetApp()->GetProfileString("Afx Directory", "Fixed Directory");
	m_pModel->SetLsdDesign(m_pIn->m_pARcBridgeDataStd->IsLsdDesign());		// Lsd 설정 추가

	//////////////////////////////////////////////////////////////////////////
	// 한계상태 단위추가
// 	CString strUnit(pBri->m_pARcBridgeDataStd->IsSI() ? _T("KN") : _T("TON"));
// 	if(pBri->m_pARcBridgeDataStd->IsLsdDesign())
// 	{
// 		strUnit = _T("N");
// 	}
// 
// 	SStmDof sDof;
// 	sDof.SetAll();
// 	if(pStm->GetPlane() == eStmPlaneYz)
// 	{
// 		sDof.ux = FALSE;
// 		sDof.ry = FALSE;
// 		sDof.rz = FALSE;
// 	}
// 	pStm->SetSystem(sDof, strUnit, pBri->m_pARcBridgeDataStd->IsLsdDesign() ? NOTRANS("mm") : NOTRANS("M"));
	//////////////////////////////////////////////////////////////////////////

	if(bRemakeModel && !pStm->IsLock() && !pStm->IsStmModified())
	{
		MakeStmTrussData(ixBridge, ixDesignType);
		SetStmAreaByInput(pStm);
	}

	if(pBri->m_ixSolverStmTool == _STMSOLVER_SAP2000 || (pBri->m_ixSolverStmTool == _STMSOLVER_SAP_ASTM && bRenumberModel == TRUE && bRemakeModel == FALSE))
	{
		CString szBridgeName = pBri->m_strBridgeName;
		szBridgeName.Replace(_T(" "), _T("_"));

		CString szFileName = m_pIn->m_pDoc->GetPathName();
		szFileName  = szFileName.Left(szFileName.ReverseFind('.'));
		szFileName += _T("_") + szBridgeName + _T("_");
		szFileName += pBri->GetStringStmModelNameByKey(ixDesignType) + _T(".s2k");
		pStm->SetModelStmToFemManager();
		CFEMManage *pFem = pStm->GetFemManager();
		if(pFem)
		{
			// X_Z평면 모델이므로 두방향만 잡아준다.
// 			pFem->SYSTEM_VAR.m_UX = 1;
// 			pFem->SYSTEM_VAR.m_UY = 0;
// 			pFem->SYSTEM_VAR.m_UZ = 1;
// 			pFem->SYSTEM_VAR.m_RX = 0;
// 			pFem->SYSTEM_VAR.m_RY = 1;
// 			pFem->SYSTEM_VAR.m_RZ = 0;
			pFem->SaveAsSap2000(szFileName);

			m_pIn->m_sSapFileNameArray.Add(szFileName);
		}

		return;
	}

	// clear,delete
	ClearFile(szPath + NOTRANS("\\OUTPUT.tcom??"));
	ClearFile(szPath + NOTRANS("\\OUTPUT.tdim??"));

	CStdioFile sFileTsap;
	if(sFileTsap.Open (szPath + _T("\\output.tsap") ,CFile::modeCreate | CFile::modeWrite ))
	{
		pStm->MakeInputStad2D(&sFileTsap);
		sFileTsap.Close();
	}
	CStdioFile sFileEpxy;
	if(sFileEpxy.Open (szPath + _T("\\output.epxy") ,CFile::modeCreate | CFile::modeWrite ))
	{
		pStm->MakeFileEpxy(&sFileEpxy);
		sFileEpxy.Close();
	}
	CStdioFile sFileEffr;
	if(sFileEffr.Open (szPath + _T("\\output.effstr") ,CFile::modeCreate | CFile::modeWrite ))
	{
		pStm->MakeFileEffStr(&sFileEffr);
		sFileEffr.Close();
	}
}

LRESULT CInputStmModelBase::OnCalcOk( WPARAM, LPARAM )
{
	if(m_bCheckAllBridge)
	{
		long nSizeBri = m_pIn->m_pARcBridgeDataStd->GetBridgeSize();
		for(long nBri=0; nBri < nSizeBri; nBri++)
		{
			CRcBridgeRebar *pBri = m_pIn->m_pARcBridgeDataStd->GetBridgeRebar(nBri);

			std::vector<HGINT32> vKeys;
			GetModelKeysForAnalysis(nBri, vKeys);

			long icModel = vKeys.size();

			for(HGINT32 ix=0; ix<icModel; ++ix)
			{
				if(m_pIn->CompleteRunOfSap2000(pBri, vKeys[ix]))
				{
					CStmModel *pStm = pBri->GetStmModelByKey(vKeys[ix]);
					if(pStm)
					{
						pStm->SetResultStmFromFemManager();
						CStmDesignStd StmDesign(pStm, NULL, NULL, TRUE);
						StmDesign.CalculateNodalZone();

// 						pStm->SetResultStmFromFemManager();
					}
				}
			}
		}
		
		SetLoadCaseCombo();

		UpdateData(FALSE);
//		SetControl();

	}
	else
	{
		CRcBridgeRebar *pBri = m_pIn->GetRcBridgeRebar();

		std::vector<HGINT32> vKeys;
		GetModelKeysForAnalysis(m_pIn->m_nCurrentBridge, vKeys);

		long icModel = vKeys.size();

		for(HGINT32 ix=0; ix<icModel; ++ix)
		{
			if(m_pIn->CompleteRunOfSap2000(pBri, vKeys[ix]))
			{
				CStmModel *pStm = pBri->GetStmModelByKey(vKeys[ix]);
				if(pStm)
				{
					pStm->SetResultStmFromFemManager();

					CStmDesignStd StmDesign(pStm, NULL, NULL, TRUE);
					StmDesign.CalculateNodalZone();

// 					pStm->SetResultStmFromFemManager();
				}
			}
		}
		
//		HGINT32 iDesignType = m_bRunSapGeneralModel ? m_pIn->m_nStepDesignCheckSap : m_iDesignType;
//		CompleteStmAnalysisAbutPier(pAbut, pPier, iDesignType, m_ixStmCombo);
	}

	m_bEndCalcOK = TRUE;
	AfxGetMainWnd()->SendMessage(WM_CALC_OK);

	DrawInputDomyunView();

	return 0;
}

LRESULT CInputStmModelBase::OnCalcOkStressFlow( WPARAM, LPARAM )
{
	long nBri = m_pIn->m_nCurrentBridge;
	
	CRcBridgeRebar *pBri = m_pIn->m_pARcBridgeDataStd->GetBridgeRebar(nBri);
	if(pBri == NULL) return 0;

	m_pIn->m_nCurrentBridge = nBri;

	// clear StressFlow
	CStmModel *pStm = pBri->GetStmModelByKey(m_ixStmKey); 
	if(pStm == NULL) return 0;

	CString szWorkDir = m_pIn->m_pDoc->GetPathName();
	szWorkDir = szWorkDir.Left(szWorkDir.GetLength() - 4);

	CString szStmPos = pBri->m_strBridgeName + _T("_") + pBri->GetStringStmModelNameByKey(m_ixStmKey);
	DeleteAndCopyOutput(_T("pcom"));
	DeleteAndCopyOutput(_T("psap"));

	m_bEndCalcOK = FALSE;

	CString szFileName = _T("");
	pStm->m_bQuadMesh = TRUE;
	pStm->dataQuad = m_pStress->dataQuad;

	szFileName.Format(_T("%s%s.pcom%02d"), szWorkDir, szStmPos, 1);
	pStm->ReadResultStressFlow(szFileName);
	
	m_bEndCalcOK = TRUE;

	return 0;
}

LRESULT CInputStmModelBase::OnCalcOkStad2D( WPARAM, LPARAM )
{
	long nBri = m_pIn->m_nCurrentBridge;
	CRcBridgeRebar *pBri = m_pIn->m_pARcBridgeDataStd->GetBridgeRebar(nBri);
	if(pBri == NULL) return 0;

	CStmModel *pStm = pBri->GetStmModelByKey(m_ixStmKey); 
	if(pStm == NULL) return 0;

	CString szWorkDir = m_pIn->m_pDoc->GetPathName();
	szWorkDir = szWorkDir.Left(szWorkDir.GetLength() - 4);

	DeleteAndCopyOutput(_T("tcom"));
	DeleteAndCopyOutput(_T("tdim"));
	DeleteAndCopyOutput(_T("tsap"));
	DeleteAndCopyOutput(_T("tdim_max"));

	CString szStmPos = pBri->m_strBridgeName + _T("_") + pBri->GetStringStmModelNameByKey(m_ixStmKey);
	CString szFileName = _T("");

	long nSizeCombo = pStm->GetCountLoadConditionCombo();

	long nSolverTool = GetSelected2DFrameTool();
	if(nSolverTool != _STMSOLVER_SAP2000)
	{
		// 구조해석 결과
		szFileName.Format(_T("%s%s.tcom%02d"), szWorkDir, szStmPos, nSizeCombo);

		CStdioFile sFileOut;
		if(sFileOut.Open (szFileName ,CFile::modeRead | CFile::typeText ))
		{
			pStm->ReadResultStad2D(&sFileOut);
			sFileOut.Close();
		}

		// 절점영역 계산 결과
		szFileName.Format(_T("%s%s.tdim%02d"), szWorkDir, szStmPos, nSizeCombo);

		CStdioFile sFileCalc;
		if(sFileCalc.Open (szFileName ,CFile::modeRead | CFile::typeText ))
		{
			pStm->ReadNodalZoneStad2D(&sFileCalc, FALSE);
			sFileCalc.Close();
		}

		pStm->GetFemManager()->m_bLoadResult = m_bRunStad2DForSap2000 ? FALSE : TRUE;
	}

	// 최대유효폭 계산 결과
	szFileName.Format(_T("%s%s.tdim%02d_max"), szWorkDir, szStmPos, nSizeCombo);

	CStdioFile sFileMax;
	if(sFileMax.Open (szFileName ,CFile::modeRead | CFile::typeText ))
	{
		pStm->ReadNodalZoneStad2D(&sFileMax, TRUE);
		sFileMax.Close();
	}

	m_bEndCalcOK = TRUE;

	return 0;
}

void CInputStmModelBase::DeleteAndCopyOutput( CString szExt )
{
	long nBri = m_pIn->m_nCurrentBridge;
	CRcBridgeRebar *pBri = m_pIn->m_pARcBridgeDataStd->GetBridgeRebar(nBri);
	if(pBri == NULL) return;

	CString szPath = AfxGetApp()->GetProfileString("Afx Directory", "Fixed Directory");

	CString szWorkDir = m_pIn->m_pDoc->GetPathName();
	szWorkDir = szWorkDir.Left(szWorkDir.GetLength() - 4);

	CString szSource = _T("");
	CString szTarget = _T("");
	CString szStmPos = pBri->m_strBridgeName + _T("_") + pBri->GetStringStmModelNameByKey(m_ixStmKey);
	CString szFind = _T("");

	CFileFind ff;
	// 기존 OUTPUT 삭제 : 작업폴더
	szFind.Format(_T("%s%s*.%s??"),szWorkDir, szStmPos, szExt);
	if(szExt == _T("tdim_max"))
	{
		szFind.Format(_T("%s%s*.%s??_max"),szWorkDir, szStmPos, _T("tdim"));
	}
	BOOL bFind = ff.FindFile(szFind);
	while(bFind)
	{
		bFind = ff.FindNextFile();
		if(ff.GetFileName() == _T(".") || ff.GetFileName() == _T("..")) continue;

		DeleteFile(ff.GetFilePath());
	}
	// 결과파일 복사 : 실행폴더 -> 작업폴더
	long nLenExt = (szExt == _T("tsap") || szExt == _T("psap")) ? 4 : 6;
	if(szExt == _T("tdim_max"))
	{
		nLenExt = 10;
	}
	szFind.Format(_T("%s\\OUTPUT.%s??"),szPath, szExt);
	if(szExt == _T("tdim_max"))
	{
		szFind.Format(_T("%s\\OUTPUT.%s??_max"),szPath, _T("tdim"));
	}
	bFind = ff.FindFile(szFind);
	while(bFind)
	{
		bFind = ff.FindNextFile();
		if(ff.GetFileName() == _T(".") || ff.GetFileName() == _T("..")) continue;

		szSource = ff.GetFilePath();
		szTarget.Format(_T("%s%s%s"), szWorkDir, szStmPos, szSource.Right(nLenExt + 1));

		CFile::Rename(szSource, szTarget);
	}
}

void CInputStmModelBase::MakeStressFlowInputData(CString szPath)
{
	// Clear
	ClearFile(szPath + NOTRANS("\\OUTPUT.pcom??"));
	
	// make, run
	CStdioFile sFileTemp;
	sFileTemp.Open (szPath + _T("\\INPUTP") ,CFile::modeCreate | CFile::modeWrite );
	sFileTemp.Close();
	sFileTemp.Open (szPath + _T("\\OUTPUT.fem") ,CFile::modeCreate | CFile::modeWrite );
	sFileTemp.Close();
	sFileTemp.Open (szPath + _T("\\OUTPUT.temp") ,CFile::modeCreate | CFile::modeWrite );
	sFileTemp.Close();

	// 해석
	CStdioFile sFilePsap;
	if(sFilePsap.Open (szPath + _T("\\output.psap") ,CFile::modeCreate | CFile::modeWrite ))
	{
		m_pStress->MakeInputStressFlowOrESO(&sFilePsap, FALSE);
		sFilePsap.Close();
	}
	CStdioFile sFileEpxy;
	if(sFileEpxy.Open (szPath + _T("\\output.epxy") ,CFile::modeCreate | CFile::modeWrite ))
	{
		m_pStress->MakeFileEpxy(&sFileEpxy);
		sFileEpxy.Close();
	}
}

void CInputStmModelBase::MakeInputStad2D()
{
	long nBri = m_pIn->m_nCurrentBridge;
	CRcBridgeRebar *pBri = m_pIn->m_pARcBridgeDataStd->GetBridgeRebar(nBri);
	if(pBri == NULL) return;

	CString szEdit = AfxGetApp()->GetProfileString(_T("Settings"), _T("Editor"));	
	if(szEdit == _T(""))
	{
		CString szPath = AfxGetApp()->GetProfileString("Afx Directory", "Fixed Directory");
		szEdit = szPath + _T("\\AEdit.exe");
	}
	CString szWorkDir = m_pIn->m_pDoc->GetPathName();
	szWorkDir = szWorkDir.Left(szWorkDir.GetLength() - 4);
	CString szStmPos = pBri->m_strBridgeName + _T("_") + pBri->GetStringStmModelNameByKey(m_ixStmKey);
	CString sFileName = szWorkDir + szStmPos + _T(".tsap");

	CString szExt;				
	CString szFileCommet;
	szExt = _T("*.tsap|*.*");
	szFileCommet.Format(_T("ASTM Input File(*.tsap)|*.tsap|%s (*.*)|*.*||"),hggettext("모든파일"));
	CFileDialog dlg(FALSE, szExt,sFileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFileCommet);
	dlg.m_ofn.lpstrTitle = _T("ASTM Input 생성");

	if(dlg.DoModal() == IDOK)
	{
		sFileName = dlg.GetPathName();
		CStdioFile sFiletsap;
		sFiletsap.Open (sFileName ,CFile::modeCreate | CFile::modeWrite );
		m_pModel->MakeInputStad2D(&sFiletsap);
		sFiletsap.Close();

		ShellExecute(NULL, NULL, szEdit, sFileName, NULL, SW_SHOWNORMAL);		
	}
}

void CInputStmModelBase::ReadOututStad2D()
{
	CStmUbManager *pMng = m_pUbRenderer->GetUbManager();
	if(pMng == NULL) return;

	CStmModel *pStm = pMng->GetStmModel();
	if(pStm == NULL) return;

	CString szOutExt;
	CString szComment;
	szOutExt  = _T("*.tcom*|*.*");
	szComment.Format(_T("ASTM Output File(*.tcom*)|*.out|%s (*.*)|*.*||"),hggettext("모든파일"));

	MakeStmInputData(m_pIn->m_nCurrentBridge, m_ixStmKey, FALSE, FALSE);
	pMng->UpdateStmRenderer();

	CString szFileName;
	CFileDialog dlg(TRUE, szOutExt, _T("*.tcom01"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szComment, m_pView);
	dlg.m_ofn.lpstrTitle = _T("ASTM 결과파일 불러오기");
	dlg.m_ofn.lpstrInitialDir = m_pIn->GetPathDirectory();

	if (dlg.DoModal() == IDOK)
	{
		szFileName = dlg.GetPathName();
	}
	else
	{
		return;
	}

	// 구조해석 결과
	CStdioFile sFileOut;
	if(sFileOut.Open (szFileName ,CFile::modeRead | CFile::typeText ))
	{
		pStm->ReadResultStad2D(&sFileOut);
		sFileOut.Close();
		m_pModel->GetFemManager()->m_bLoadResult = TRUE;	
	}

	// 절점영역 계산 결과
	CStdioFile sFileCalc;
	szFileName = szFileName.Left(szFileName.GetLength() - 5) + _T("dim01");
	if(sFileCalc.Open (szFileName ,CFile::modeRead | CFile::typeText ))
	{
		pStm->ReadNodalZoneStad2D(&sFileCalc, FALSE);
		sFileCalc.Close();
	}

	// 최대유효폭 계산 결과
	CStdioFile sFileMax;
	szFileName = szFileName.Left(szFileName.GetLength() - 5) + _T("dim01_max");
	if(sFileMax.Open (szFileName ,CFile::modeRead | CFile::typeText ))
	{
		pStm->ReadNodalZoneStad2D(&sFileMax, TRUE);
		sFileMax.Close();
	}
}

void CInputStmModelBase::SetStmAreaByInput( CStmModel *pStm )
{
	long nBri = m_pIn->m_nCurrentBridge;
	CRcBridgeRebar *pBri = m_pIn->m_pARcBridgeDataStd->GetBridgeRebar(nBri);
	if(pBri == NULL) return;

	if(pStm->GetCountPropertyArea(TRUE) == 0) return;

	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;

	long nIndex = 0;
	CStmObjArea *pArea = pStm->GetPropertyArea(nIndex, TRUE);
	if(pArea == NULL) return;

	CStmDataConfig *pConfig = m_pModel->GetStmDataConfig(); // 입력이 설정되어 있어야 하므로, m_pModel을 사용,
	if(pConfig == NULL) return;

	SStmAreaProperty *sProperty = &pArea->GetAreaProperty();
	
	sProperty->fck = pConfig->m_dFck;
	sProperty->fct = pConfig->m_dFt;
	sProperty->fy	= pConfig->m_dFy;
	sProperty->fvy	= pConfig->m_dFvy;
	
	CString strUnit = pStd->GetStringUnitType(UNIT_CGS_TONF_M2);
	HGDOUBLE dUC = GetValueUnitChange(1.0, _T("N/mm²"),strUnit);
	HGDOUBLE dG = pBri->GetGravity();

	CStmObjMaterial *pMatConc = pStm->GetMaterial(sProperty->id_materal_conc);
	if(pMatConc)
	{
		SStmPropMaterial sMatProperty = pMatConc->GetProperty();

		pMatConc->SetName(_T("A-CONC"));

		HGDOUBLE dW = toTon(pStd->m_pBasicConcInfo->m_UWeightConcrete);

		sMatProperty.elastic_modulus	= pConfig->m_dEc * dUC;
		sMatProperty.Fy					= 0;
		sMatProperty.IDES				= 'C';
		sMatProperty.mass_density		= dW/dG;
		sMatProperty.poisson_ratio		= pStd->m_RatePoasong;
		sMatProperty.thermal_coeff		= 0;
		sMatProperty.weight				= GetValueUnitChange(dW, UNIT_CGS_TONF_M3, pStd->m_szTonf_M3);
		pMatConc->SetProperty(sMatProperty);
	}

	CStmObjMaterial *pMatConc1 = pStm->GetMaterial(sProperty->id_materal_Rconc);
	if(pMatConc1)
	{
		SStmPropMaterial sMatProperty = pMatConc1->GetProperty();

		pMatConc1->SetName(_T("A-RCONC"));

		HGDOUBLE dW = toTon(pStd->m_pBasicConcInfo->m_UWeightSteelConcrete);

		sMatProperty.elastic_modulus	= pConfig->m_dEc * dUC;
		sMatProperty.Fy					= pConfig->m_dFck * dUC;
		sMatProperty.IDES				= 'C';
		sMatProperty.mass_density		= dW/dG;
		sMatProperty.poisson_ratio		= pStd->m_RatePoasong;
		sMatProperty.thermal_coeff		= 0;
		sMatProperty.weight				= GetValueUnitChange(dW, UNIT_CGS_TONF_M3, pStd->m_szTonf_M3);
		pMatConc1->SetProperty(sMatProperty);
	}

	CStmObjMaterial *pMatSteel = pStm->GetMaterial(sProperty->id_materal_steel);
	if(pMatSteel)
	{
		SStmPropMaterial sMatProperty = pMatSteel->GetProperty();

		pMatSteel->SetName(_T("A-STEEL"));

		HGDOUBLE dW = toTon(pStd->m_pBasicConcInfo->m_UWeightSteel);

		sMatProperty.elastic_modulus	= pConfig->m_dEs * dUC;
		sMatProperty.Fy					= pConfig->m_dFy * dUC;
		sMatProperty.IDES				= 'S';
		sMatProperty.mass_density		= dW/dG;
		sMatProperty.poisson_ratio		= pStd->m_RatePoasong;
		sMatProperty.thermal_coeff		= 0;
		sMatProperty.weight				= GetValueUnitChange(dW, UNIT_CGS_TONF_M3, pStd->m_szTonf_M3);
		pMatSteel->SetProperty(sMatProperty);
	}
}

void CInputStmModelBase::ClearFile( CString szFileName )
{
	CFileFind ff;
	BOOL bFind = ff.FindFile(szFileName);
	while(bFind)

	{
		bFind = ff.FindNextFile();
		if(ff.GetFileName() == NOTRANS(".") || ff.GetFileName() == NOTRANS("..")) continue;

		DeleteFile(ff.GetFilePath());
	}
}

void CInputStmModelBase::GetModelKeysForAnalysis( HGINT32 nBri, std::vector<HGINT32> &vKeys )
{
	vKeys.clear();
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp *pBri = pStd->GetBridge(nBri);
	if(pBri == NULL)
		return;

	// 전체 교량이 체크되어있으면 나머지는 모두다 출력 상태인걸로.
	// 전체 기초 출력이 체크되 있으면 마찬가지로 모든 케이스 다 출력상태인걸로.
	if(m_bCheckAllBridge == FALSE && m_bCheckAllModel == FALSE && (m_nIDD == IDD_STM_BRACKET || m_nIDD == IDD_STM_ARC_CORNER || m_bCheckAllCombo == FALSE))
	{
		vKeys.push_back(m_ixStmKey);
		return;
	}

	HGBOOL bLsd = pStd->IsLsdDesign();
	HGINT32 iKeyStt(0);
	HGINT32 iKeyEnd(0);
	HGINT32 iKeyBreak(0);
	HGINT32 iKeyContinue(0);

	if(m_nIDD == IDD_STM_BRACKET)
	{
		iKeyStt = STM_MODEL_BRACKET_STT;
		iKeyEnd = STM_MODEL_BRACKET_END;
	}
	else if(m_nIDD == IDD_STM_ARC_CORNER)
	{
		iKeyStt = STM_MODEL_CORNER_STT_UPPER_MINUS;
		iKeyBreak = STM_MODEL_CORNER_END_LOWER_PLUS;
		iKeyContinue = bLsd? STM_MODEL_CORNER_STT_UPPER_MINUS_EXT_1 : STM_MODEL_CORNER_STT_UPPER_MINUS;
		iKeyEnd = bLsd? STM_MODEL_CORNER_END_LOWER_PLUS_EXT_2 : STM_MODEL_CORNER_END_LOWER_PLUS;
	}
	else if(m_nIDD == IDD_STM_FOORING_STARTEND)
	{
		if(m_bCheckAllBridge || m_bCheckAllModel)
		{
			iKeyStt = bLsd? STM_MODEL_FOOT_STT_MAX_AXIAL : STM_MODEL_FOOT_STT_MAXP;
			iKeyEnd = bLsd? STM_MODEL_FOOT_END_MIN_HORI_FORCE_EXT_2 : STM_MODEL_FOOT_END_EARTH;
		}
		else
		{
			if(m_ixStmKey > bLsd? STM_MODEL_FOOT_STT_MIN_HORI_FORCE_EXT_2 : STM_MODEL_FOOT_STT_EARTH)
			{
				iKeyStt = bLsd? STM_MODEL_FOOT_END_MAX_AXIAL : STM_MODEL_FOOT_END_MAXP;
				iKeyEnd = bLsd? STM_MODEL_FOOT_END_MIN_HORI_FORCE_EXT_2 : STM_MODEL_FOOT_END_EARTH;
			}
			else
			{
				iKeyStt = bLsd? STM_MODEL_FOOT_STT_MAX_AXIAL : STM_MODEL_FOOT_STT_MAXP;
				iKeyEnd = bLsd? STM_MODEL_FOOT_STT_MIN_HORI_FORCE_EXT_2 : STM_MODEL_FOOT_STT_EARTH;
			}
		}
	}
	else if(m_nIDD == IDD_STM_FOORING_MID)
	{
		HGINT32 icFoot = pBri->GetCountInWall();
		if(m_bCheckAllModel)
		{
			iKeyStt = bLsd? STM_MODEL_FOOT_MID_MAX_AXIAL : STM_MODEL_MIDFOOT_HEAD;
			iKeyEnd = bLsd? (STM_MODEL_FOOT_MID_MAX_AXIAL * (3 * 5 * icFoot)) : (STM_MODEL_MIDFOOT_HEAD + (4 * icFoot));
		}
		else
		{
			HGINT32 ix= bLsd? ((m_ixStmKey - STM_MODEL_FOOT_MID_MAX_AXIAL) / (3 * 5)) : ((m_ixStmKey - STM_MODEL_MIDFOOT_HEAD) / 4);
			
			if(bLsd)
			{
				iKeyStt = STM_MODEL_FOOT_MID_MAX_AXIAL + ((3 * 5) * ix);
				iKeyEnd = STM_MODEL_FOOT_MID_MAX_AXIAL + ((3 * 5) * (ix + 1) - 1);
			}
			else
			{
				iKeyStt = STM_MODEL_MIDFOOT_HEAD + (4 * ix);
				iKeyEnd = STM_MODEL_MIDFOOT_HEAD + (4 * ix + 3);
			}
		}
	}

	for(HGINT32 iKey=iKeyStt; iKey<=iKeyEnd; iKey++)
	{
		// 한계상태 작업하면서 Key값이 연속되지 않는게 생겼다.
		if(iKeyBreak > iKeyStt && iKey > iKeyBreak && iKey < iKeyContinue)
			continue;

		CStmModel *pModel = pBri->GetStmModelByKey(iKey);
		if(pModel == NULL) continue;

		vKeys.push_back(iKey);
	}
}

void CInputStmModelBase::GetModelKeysForAnalysis_Lsd(HGINT32 nBri, std::vector<HGINT32> &vKeys)
{
	vKeys.clear();
	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
	CRcBridgeApp *pBri = pStd->GetBridge(nBri);
	if(pBri == NULL)
		return;

	// 전체 교량이 체크되어있으면 나머지는 모두다 출력 상태인걸로.
	// 전체 기초 출력이 체크되 있으면 마찬가지로 모든 케이스 다 출력상태인걸로.
	if(m_bCheckAllBridge == FALSE && m_bCheckAllModel == FALSE && (m_nIDD == IDD_STM_BRACKET || m_nIDD == IDD_STM_ARC_CORNER || m_bCheckAllCombo == FALSE))
	{
		vKeys.push_back(m_ixStmKey);
		return;
	}

	HGINT32 iKeyStt(0);
	HGINT32 iKeyEnd(0);

	if(m_nIDD == IDD_STM_BRACKET)
	{
		iKeyStt = STM_MODEL_BRACKET_STT;
		iKeyEnd = STM_MODEL_BRACKET_END;
	}
	else if(m_nIDD == IDD_STM_ARC_CORNER)
	{
		iKeyStt = STM_MODEL_CORNER_STT_UPPER_MINUS;
		iKeyEnd = STM_MODEL_CORNER_END_LOWER_PLUS;
	}
	else if(m_nIDD == IDD_STM_FOORING_STARTEND)
	{
		if(m_bCheckAllModel)
		{
			iKeyStt = STM_MODEL_FOOT_STT_MAX_AXIAL;
			iKeyEnd = STM_MODEL_FOOT_END_MIN_HORI_FORCE_EXT_2;
		}
		else
		{
			if(m_ixStmKey < STM_MODEL_FOOT_STT_MIN_HORI_FORCE_EXT_2)
			{
				iKeyStt = STM_MODEL_FOOT_STT_MAX_AXIAL;
				iKeyEnd = STM_MODEL_FOOT_STT_MIN_HORI_FORCE_EXT_2;
			}
			else
			{
				iKeyStt = STM_MODEL_FOOT_END_MAX_AXIAL;
				iKeyEnd = STM_MODEL_FOOT_END_MIN_HORI_FORCE_EXT_2;
			}
		}
	}
	else if(m_nIDD == IDD_STM_FOORING_MID)
	{
		HGINT32 icFoot = pBri->GetCountInWall();
		if(m_bCheckAllModel)
		{
			iKeyStt = STM_MODEL_FOOT_MID_MAX_AXIAL;
			iKeyEnd = STM_MODEL_FOOT_MID_MIN_HORI_FORCE_EXT_2 + (STM_MODEL_FOOT_LSD_COUNT * icFoot);
		}
		else
		{
			HGINT32 ix= (m_ixStmKey - STM_MODEL_FOOT_MID_MAX_AXIAL) / STM_MODEL_FOOT_LSD_COUNT;

			iKeyStt = STM_MODEL_FOOT_MID_MAX_AXIAL + (STM_MODEL_FOOT_LSD_COUNT * ix);
			iKeyEnd = STM_MODEL_FOOT_MID_MIN_HORI_FORCE_EXT_2 + (STM_MODEL_FOOT_LSD_COUNT * ix + 1);
		}
	}

	for(HGINT32 iKey=iKeyStt; iKey<=iKeyEnd; iKey++)
	{
		CStmModel *pModel = pBri->GetStmModelByKey(iKey);
		if(pModel == NULL) continue;

		vKeys.push_back(iKey);
	}

	// 우각부 추가 - 극한상황 1, 2
	if( m_nIDD == IDD_STM_ARC_CORNER )
	{
		for(HGINT32 iKey=STM_MODEL_CORNER_STT_UPPER_MINUS_EXT_1; iKey<=STM_MODEL_CORNER_END_LOWER_PLUS_EXT_2; iKey++)
		{
			CStmModel *pModel = pBri->GetStmModelByKey(iKey);
			if(pModel == NULL) continue;

			vKeys.push_back(iKey);
		}
	}
}

// void CInputStmModelBase::SetUnlock()
// {
// 	CARcBridgeDataStd* pStd = m_pIn->m_pARcBridgeDataStd;
// 
// 	long nSizeBri = pStd->GetBridgeSize();
// 	long nBriStt = m_bCheckAllBridge ? 0 :  m_pIn->m_nCurrentBridge;
// 	long nBriEnd = m_bCheckAllBridge ? nSizeBri - 1 :  m_pIn->m_nCurrentBridge;
// 
// //	long nCurrentBri = m_pIn->m_nCurrentBridge;
// // 	long nCurrentKey = m_ixStmKey;
// 
// 	for(long nBri = nBriStt; nBri <= nBriEnd; nBri++)
// 	{
// 		CRcBridgeApp *pBri = pStd->GetBridge(nBri);
// 		if(pBri == NULL)
// 			return;
// 
// 		std::vector<HGINT32> vKeys;
// 		GetModelKeysForAnalysis(nBri, vKeys);
// 
// 		long icModel = vKeys.size();
// 
// 		for(HGINT32 ix=0; ix<icModel; ++ix)
// 		{
// 			CStmModel *pStm = pBri->GetStmModelByKey(vKeys[ix]);
// 			if(pStm == NULL) continue;
// 
// 			pStm->SetLock(FALSE);
// 		}
// 	}
// }

void CInputStmModelBase::OnBnClickedCheckAllbridge()
{
	UpdateData(TRUE);
	GetDlgItem(IDC_CHECK_ALLCORNER)->EnableWindow(!m_bCheckAllBridge);
	GetDlgItem(IDC_CHECK_ALLCOMBO)->EnableWindow(!m_bCheckAllBridge);
	if(m_bCheckAllBridge)
	{
		m_bCheckAllModel = FALSE;
		m_bCheckAllCombo = FALSE;
		UpdateData(FALSE);
	}
}


void CInputStmModelBase::OnBnClickedCheckAllModel()
{
	UpdateData(TRUE);
	GetDlgItem(IDC_CHECK_ALLCOMBO)->EnableWindow(!m_bCheckAllModel);
	if(m_bCheckAllModel)
	{
		m_bCheckAllCombo = FALSE;
		UpdateData(FALSE);
	}
}


void CInputStmModelBase::OnBnClickedCheckAllcombo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CInputStmModelBase::SetUnlock()
{
	m_pModel->SetLock(FALSE);
}

void CInputStmModelBase::RecalculateStmLoadDistribution()
{
	MakeStmInputData(-1, m_ixStmKey, FALSE, TRUE);
}
