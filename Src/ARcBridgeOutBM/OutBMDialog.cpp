// OutBMDialog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "../../Src/ARcBridgeData/ARcBridgeData.h"
#include "../ARcBridgeInput/DlgProgress.h"
#include "arcbridgeoutbm.h"
#include "../../../../Engine/BaseEng/include/attcmn.h"


// COutBMDialog 대화 상자입니다.

IMPLEMENT_DYNAMIC(COutBMDialog, CXTResizeDialog)

COutBMDialog::COutBMDialog(CWnd* pParent /*=NULL*/)
	: CXTResizeDialog(COutBMDialog::IDD, pParent)
{
	m_bCancelPrint	= FALSE;
	m_bPrintingExcel = FALSE;
	m_bEnableOpen	= FALSE;
	m_pBaseBM = NULL;
	m_pStd = NULL;
	m_nKindBM = 0;
	m_nCountGrid = 1;
	m_bReadyOutBMDlg = FALSE;

	m_pDlgProgress	= new CDlgProgress;

	// 사용자 수량에 사용될 수량을 판단하기 위함 ////
	m_bFocusOther	= FALSE;
	m_pBMTreeInfoCur	= NULL;
	m_pBMTreeInfoOther	= NULL;
	m_pTreeCtrl = NULL;
}

COutBMDialog::~COutBMDialog()
{
	AhTPADelete(&m_arrBMTreeInfo, (structBMTreeInfo*)0);		// 일반수량
	AhTPADelete(&m_arrBMTreeInfoCommon, (structBMTreeInfo*)0);	// 공통수량
	AhTPADelete(&m_arrBMTreeInfoTogong, (structBMTreeInfo*)0);	// 토공수량
	AhTPADelete(&m_arrBMTreeInfoRebar, (structBMTreeInfo*)0);	// 철근수량
	ClearVectorPtr(m_pArrRebarTableAll);

	ClearBM();

	delete m_pDlgProgress;
}

void COutBMDialog::DoDataExchange(CDataExchange* pDX)
{
	CXTResizeDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_PRINT_BM, m_staticPrintBM);
	DDX_Control(pDX, IDC_COMBO_PREVIEW, m_comboPreview);
	DDX_Control(pDX, IDC_PROGRESS_PRINT_BM, m_ctlProgressPrintBM);
	DDX_Control(pDX, IDC_BUTTON_WMF_R, m_btnWmf);
	DDX_Control(pDX, IDC_BUTTON_USER_BM, m_btnUserBM);
	DDX_Control(pDX, IDC_BUTTON_PRINT_SELECT, m_btnPrintSelect);
	DDX_Control(pDX, IDC_BUTTON_OPTION_BM, m_btnOption);
	DDX_Control(pDX, IDC_BUTTON_KIND_SELECT, m_btnKindSelect);
	DDX_Control(pDX, IDC_BUTTON_PRINT_CANCEL, m_btnCancel);
	DDX_Radio(pDX, IDC_RADIO_VIEW_VERT, m_nRadioViewVert);
	DDX_GridControl(pDX, IDC_GRID_ORG, m_Grid);
	DDX_GridControl(pDX, IDC_GRID_OTHER, m_GridOther);
}


BEGIN_MESSAGE_MAP(COutBMDialog, CXTResizeDialog)
	ON_WM_SIZE()
	ON_CBN_SELCHANGE(IDC_COMBO_PREVIEW, OnSelchangeComboPreview)
	ON_BN_CLICKED(IDC_RADIO_VIEW_HOR, OnRadioViewHor)
	ON_BN_CLICKED(IDC_RADIO_VIEW_VERT, OnRadioViewVert)
	ON_BN_CLICKED(IDC_BUTTON_OPTION_BM, OnButtonOption)
	ON_BN_CLICKED(IDC_BUTTON_PRINT_SELECT, OnButtonPrintSelect)
	ON_BN_CLICKED(IDC_BUTTON_PRINT_CANCEL, OnButtonPrintCancel)
	ON_BN_CLICKED(IDC_BUTTON_WMF_R, OnButtonWmf)
	ON_BN_CLICKED(IDC_BUTTON_KIND_SELECT, OnButtonKindSelect)
	ON_BN_CLICKED(IDC_BUTTON_USER_BM, OnButtonUserBmInput)
	ON_BN_CLICKED(IDC_BUTTON_USER_BM2, OnButtonUserBmApply)
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()


// COutBMDialog 메시지 처리기입니다.


BOOL COutBMDialog::OnInitDialog() 
{
	CXTResizeDialog::OnInitDialog();

	if(!m_pStd) return FALSE;

	SetResize(IDC_STATIC_PREVIEW_SU, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_COMBO_PREVIEW, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_STATIC_VIEW_ALI, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_RADIO_VIEW_VERT, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_RADIO_VIEW_HOR, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_USER_BM, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_USER_BM2, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_KIND_SELECT, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_OPTION_BM, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_STATIC_OUTBM_VIEW_BOTTOM2, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_STATIC_OUTBM_VIEW_BOTTOM1, SZ_BOTTOM_LEFT, SZ_BOTTOM_RIGHT);


	SetResize(IDC_BUTTON_WMF_R, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_PRINT_SELECT, SZ_BOTTOM_LEFT, SZ_BOTTOM_LEFT);
	SetResize(IDC_BUTTON_PRINT_CANCEL, SZ_BOTTOM_RIGHT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_STATIC_PRINT_BM, SZ_BOTTOM_LEFT, SZ_BOTTOM_RIGHT);
	SetResize(IDC_PROGRESS_PRINT_BM, SZ_BOTTOM_LEFT, SZ_BOTTOM_RIGHT);

	InitControl();
	SetControl();

	m_bReadyOutBMDlg = TRUE;

	return TRUE;
}


void COutBMDialog::InitControl()
{
	m_comboPreview.SetCurSel(0);
	m_nCountGrid	= m_comboPreview.GetCurSel() + 1;
	m_nRadioViewVert	= 0;

	UpdateData(FALSE);
}


void COutBMDialog::SetControl()
{
	GetDlgItem(IDC_RADIO_VIEW_VERT)->EnableWindow(m_nCountGrid == 2);
	GetDlgItem(IDC_RADIO_VIEW_HOR)->EnableWindow(m_nCountGrid == 2);
}

void COutBMDialog::SetControl(BOOL bStartPrint)
{
	if(bStartPrint)
	{
		m_btnPrintSelect.EnableWindow(FALSE);
		m_btnCancel.EnableWindow(TRUE);
		m_btnUserBM.EnableWindow(FALSE);
		m_btnKindSelect.EnableWindow(FALSE);
		m_btnOption.EnableWindow(FALSE);
	}
	else
	{
		m_btnPrintSelect.EnableWindow(TRUE);
		m_btnUserBM.EnableWindow(TRUE);
		m_btnKindSelect.EnableWindow(TRUE);
		m_btnOption.EnableWindow(TRUE);
	}
}


void COutBMDialog::ClearBM()
{
	for(long cm = 0; cm < 2; cm++)
	{
		// 일반수량
		AhTPADelete(&m_pArrBMAll[cm], (CBMData*)0);			// 집계표(계)
		AhTPADelete(&m_pArrBMStringAll[cm], (CBMSumString*)0);
	}

	// 토공수량
	AhTPADelete(&m_pArrBMTogong, (CBMData*)0);			// 토공수량 집계표(계)
	AhTPADelete(&m_pArrBMStringTogong, (CBMSumString*)0);
}

void COutBMDialog::MergeBMStd()
{
	ClearBM();

	CHgBaseBMStd baseBM;
	DWORD	exceptionFlag = 0x00000000;

	for(HGINT32 ix = 0; ix < m_pStd->m_pArrOutBMNormal.GetSize(); ix++)
	{
		COutBM *pBMNor = m_pStd->m_pArrOutBMNormal.GetAt(ix);
		if(pBMNor)
			baseBM.MergeBMList(&m_pArrBMAll[0], &pBMNor->m_pArrBMSum, TRUE, exceptionFlag, FALSE);

		COutBM *pBMCom = m_pStd->m_pArrOutBMCommon.GetAt(ix);
		if(pBMCom)
			baseBM.MergeBMList(&m_pArrBMAll[1], &pBMCom->m_pArrBMSum, TRUE, exceptionFlag, FALSE);

		COutBMTogong *pBMTogong = m_pStd->m_pArrOutBMTogong.GetAt(ix);
		if(pBMTogong)
			baseBM.MergeBMList(&m_pArrBMTogong, &pBMTogong->m_pArrBMSum, TRUE, exceptionFlag, FALSE);
	}

	// 상, 하행 집계표
	for(HGINT32 ix = 0; ix < m_pStd->m_pArrOutBMNormal.GetSize(); ix++)
	{
		CRcBridgeApp *pBri = m_pStd->GetBridge(ix);
		if (pBri->m_nSeparBri != 1)
			continue;

		COutBM *pBMNorUp = m_pStd->m_pArrOutBMNormal.GetAt(ix);
		COutBM *pBMNorDown = m_pStd->m_pArrOutBMNormal.GetAt(ix+1);
		if(pBMNorUp && pBMNorDown)
		{
			baseBM.MergeBMList(&pBMNorUp->m_pArrBMSepBridgeSum, &pBMNorUp->m_pArrBMSum, TRUE, exceptionFlag, FALSE, TRUE);
			baseBM.MergeBMList(&pBMNorUp->m_pArrBMSepBridgeSum, &pBMNorDown->m_pArrBMSum, TRUE, exceptionFlag, FALSE, TRUE);
		}

		COutBM *pBMComUp = m_pStd->m_pArrOutBMCommon.GetAt(ix);
		COutBM *pBMComDown = m_pStd->m_pArrOutBMCommon.GetAt(ix+1);
		if(pBMComUp && pBMComDown)
		{
			baseBM.MergeBMList(&pBMComUp->m_pArrBMSepBridgeSum, &pBMComUp->m_pArrBMSum, TRUE, exceptionFlag, FALSE, TRUE);
			baseBM.MergeBMList(&pBMComUp->m_pArrBMSepBridgeSum, &pBMComDown->m_pArrBMSum, TRUE, exceptionFlag, FALSE, TRUE);
		}

		COutBMTogong *pBMTogongUp = m_pStd->m_pArrOutBMTogong.GetAt(ix);
		COutBMTogong *pBMTogongDown = m_pStd->m_pArrOutBMTogong.GetAt(ix+1);
		if(pBMTogongUp && pBMTogongDown)
		{
			baseBM.MergeBMList(&pBMTogongUp->m_pArrBMSepBridgeSum, &pBMTogongUp->m_pArrBMSum, TRUE, exceptionFlag, FALSE, TRUE);
			baseBM.MergeBMList(&pBMTogongUp->m_pArrBMSepBridgeSum, &pBMTogongDown->m_pArrBMSum, TRUE, exceptionFlag, FALSE, TRUE);
		}
	}
}

void COutBMDialog::MakeBMStringStd()
{
	CHgBaseBMStd baseBM;

	HGBOOL bFirst = TRUE;
	for(HGINT32 ix = 0; ix < m_pStd->m_pArrOutBMNormal.GetSize(); ix++)
	{
		CRcBridgeApp *pBri = m_pStd->GetBridge(ix);
		if (pBri->m_nSeparBri == 2)
			continue;

		COutBM *pBMNor = m_pStd->m_pArrOutBMNormal.GetAt(ix);
		if(pBMNor)
			baseBM.MakeBMSumString(&m_pArrBMStringAll[0], pBri->m_nSeparBri == 1 ? &pBMNor->m_pArrBMSepBridgeSum : &pBMNor->m_pArrBMSum, TRUE, bFirst);

		COutBM *pBMCom = m_pStd->m_pArrOutBMCommon.GetAt(ix);
		if(pBMCom)
			baseBM.MakeBMSumString(&m_pArrBMStringAll[1], pBri->m_nSeparBri == 1 ? &pBMCom->m_pArrBMSepBridgeSum : &pBMCom->m_pArrBMSum, TRUE, bFirst);

		COutBMTogong *pBMTogong = m_pStd->m_pArrOutBMTogong.GetAt(ix);
		if(pBMTogong)
			baseBM.MakeBMSumString(&m_pArrBMStringTogong, pBri->m_nSeparBri == 1 ? &pBMTogong->m_pArrBMSepBridgeSum : &pBMTogong->m_pArrBMSum, TRUE, bFirst);

		bFirst = FALSE;
	}

	baseBM.MakeBMSumString(&m_pArrBMStringAll[0], &m_pArrBMAll[0], TRUE, bFirst);
	baseBM.MakeBMSumString(&m_pArrBMStringAll[1], &m_pArrBMAll[1], TRUE, bFirst);
	baseBM.MakeBMSumString(&m_pArrBMStringTogong, &m_pArrBMTogong, TRUE, bFirst);


	// 상, 하행 집계표
	bFirst = TRUE;
	for(HGINT32 jx = 0; jx < m_pStd->m_pArrOutBMNormal.GetSize(); ++jx)
	{
		CRcBridgeApp *pBri = m_pStd->GetBridge(jx);
		if (pBri->m_nSeparBri != 1)
			continue;

		COutBM *pBMNorUp = m_pStd->m_pArrOutBMNormal.GetAt(jx);
		COutBM *pBMNorDown = m_pStd->m_pArrOutBMNormal.GetAt(jx+1);
		if(pBMNorUp && pBMNorDown)
		{
			baseBM.MakeBMSumString(&pBMNorUp->m_pArrBMStringSepBridgeSum, &pBMNorUp->m_pArrBMSum, TRUE, TRUE);					
			baseBM.MakeBMSumString(&pBMNorUp->m_pArrBMStringSepBridgeSum, &pBMNorDown->m_pArrBMSum, TRUE, FALSE);
			baseBM.MakeBMSumString(&pBMNorUp->m_pArrBMStringSepBridgeSum, &pBMNorUp->m_pArrBMSepBridgeSum, TRUE, FALSE);
		}

		COutBM *pBMComUp = m_pStd->m_pArrOutBMCommon.GetAt(jx);
		COutBM *pBMComDown = m_pStd->m_pArrOutBMCommon.GetAt(jx+1);
		if(pBMComUp && pBMComDown)
		{
			baseBM.MakeBMSumString(&pBMComUp->m_pArrBMStringSepBridgeSum, &pBMComUp->m_pArrBMSum, TRUE, TRUE);					
			baseBM.MakeBMSumString(&pBMComUp->m_pArrBMStringSepBridgeSum, &pBMComDown->m_pArrBMSum, TRUE, FALSE);
			baseBM.MakeBMSumString(&pBMComUp->m_pArrBMStringSepBridgeSum, &pBMComUp->m_pArrBMSepBridgeSum, TRUE, FALSE);
		}

		COutBMTogong *pBMTogongUp = m_pStd->m_pArrOutBMTogong.GetAt(jx);
		COutBMTogong *pBMTogongDown = m_pStd->m_pArrOutBMTogong.GetAt(jx+1);
		if(pBMTogongUp && pBMTogongDown)
		{
			baseBM.MakeBMSumString(&pBMTogongUp->m_pArrBMStringSepBridgeSum, &pBMTogongUp->m_pArrBMSum, TRUE, TRUE);
			baseBM.MakeBMSumString(&pBMTogongUp->m_pArrBMStringSepBridgeSum, &pBMTogongDown->m_pArrBMSum, TRUE, FALSE);
			baseBM.MakeBMSumString(&pBMTogongUp->m_pArrBMStringSepBridgeSum, &pBMTogongUp->m_pArrBMSepBridgeSum, TRUE, FALSE);
		}

		bFirst = FALSE;
	}
}

void COutBMDialog::MakeBMRebarTable()
{
	CTypedPtrArray <CObArray, CRebar*> pArrRebar;
	CTypedPtrArray <CObArray, CRebar*> pArrRebar2;

	ClearVectorPtr(m_pArrRebarTableAll);


	CBMRebarTable *pRebarTableAll(NULL);

	pRebarTableAll = new CBMRebarTable;
	pRebarTableAll->InitRebar();
	pRebarTableAll->SetTypeUnit(m_pStd->m_pARcBridgeDataStd->m_nTypeUnit);
	pRebarTableAll->SetBasicConcInfo(m_pStd->m_pARcBridgeDataStd->m_pBasicConcInfo);
	
	
	CString str = _T("");
	CString strBridgeName;
	CString sName = _T(""), sBri = _T(""), sDir = _T("");


	for(HGINT32 ix = 0; ix < m_pStd->m_pArrOutBMNormal.GetSize(); ix++)
	{
		CRcBridgeRebar *pBridge = m_pStd->m_pARcBridgeDataStd->GetBridgeRebar(ix);
		if(pBridge == NULL)
			continue;

		if (pBridge->m_nSeparBri == 2)
			continue;

		if(pRebarTableAll)
		{
			AhTPADelete(&pArrRebar, (CReBar*)0);
			// 철근모두가져오기                      
			// 철근수량을 Merge할때 값을 바꾸기 때문에 포인터만 가져오면 안된다.
			pBridge->MakeRebarList(&pArrRebar, -1, TRUE, TRUE, 0, FALSE);

			if(m_pStd->m_bRefCellBMSum)	str.Format(_T("%s 교량 철근수량 집계표"), pBridge->m_strBridgeName);
			else											str = _T("");

			if (pBridge->m_nSeparBri == 1)
			{
				CRcBridgeRebar *pBridgeDown = m_pStd->m_pARcBridgeDataStd->GetBridgeRebar(ix+1);
				if (pBridgeDown)
				{
					pBridgeDown->MakeRebarList(&pArrRebar, -1, TRUE, TRUE, 0, FALSE);
				}
			}

			pRebarTableAll->AddRebar(&pArrRebar, pBridge->m_strBridgeName, str);
		}

		if (pBridge->m_nSeparBri == 1)
		{
			COutBM *pOutBMUp = m_pStd->m_pArrOutBMNormal.GetAt(ix);
			CRcBridgeRebar *pBridgeDown = m_pStd->m_pARcBridgeDataStd->GetBridgeRebar(ix+1);
			if (pBridgeDown)
			{
				pOutBMUp->m_rebarSepBridgeSum.InitRebar();
				pOutBMUp->m_rebarSepBridgeSum.SetTypeUnit(m_pStd->m_pARcBridgeDataStd->m_nTypeUnit);
				pOutBMUp->m_rebarSepBridgeSum.SetBasicConcInfo(m_pStd->m_pARcBridgeDataStd->m_pBasicConcInfo);
				
				AhTPADelete(&pArrRebar, (CReBar*)0);
				// 철근모두가져오기                      
				// 철근수량을 Merge할때 값을 바꾸기 때문에 포인터만 가져오면 안된다.
				pBridge->MakeRebarList(&pArrRebar, -1, TRUE, TRUE, 0, FALSE);

				strBridgeName.Format("%s", m_pStd->m_pARcBridgeDataStd->m_strLineDirName);				

				sBri = pBridge->m_strBridgeName;
				sDir = m_pStd->m_pARcBridgeDataStd->m_strLineDirName;

				if(m_pStd->m_bRefCellBMSum)	sName.Format(_T(" %s 교량 (%s) 철근수량 집계표"), sBri, sDir);
				else											sName = _T("");

				pOutBMUp->m_rebarSepBridgeSum.AddRebar(&pArrRebar, strBridgeName, sName);

				AhTPADelete(&pArrRebar2, (CReBar*)0);
				// 철근모두가져오기                      
				// 철근수량을 Merge할때 값을 바꾸기 때문에 포인터만 가져오면 안된다.
				pBridgeDown->MakeRebarList(&pArrRebar2, -1, TRUE, TRUE, 0, FALSE);

				strBridgeName.Format("%s", m_pStd->m_pARcBridgeDataStd->m_strLineRevDirName);

				sName = _T(""), sBri = _T(""), sDir = _T("");
				sBri = pBridgeDown->m_strBridgeName; 
				sDir = m_pStd->m_pARcBridgeDataStd->m_strLineRevDirName;
				
				if(m_pStd->m_bRefCellBMSum)	sName.Format(_T(" %s 교량 (%s) 철근수량 집계표"), sBri, sDir);
				else											sName = _T("");

				pOutBMUp->m_rebarSepBridgeSum.AddRebar(&pArrRebar2, strBridgeName, sName);
				pOutBMUp->m_rebarSepBridgeSum.FinishAddRebar();				
			}			
		}
	}

	pRebarTableAll->FinishAddRebar();
	m_pArrRebarTableAll.push_back(pRebarTableAll);

	AhTPADelete(&pArrRebar, (CReBar*)0);
	AhTPADelete(&pArrRebar2, (CReBar*)0);
}

//bReplaceBM: 수량산출 후 공종이나 옵션에 의한 재계산일 경우 : TRUE
HGBOOL COutBMDialog::CreateBM(HGBOOL bOnlyReplaceString, HGBOOL bNewCreateBM)
{
	if(m_bPrintingExcel)
	{
		AfxMessageBox(_T("엑셀 출력 중에는 수량집계가 불가능합니다."));
		return FALSE;
	}

	if(m_pDlgProgress)
	{
		delete m_pDlgProgress;
		m_pDlgProgress = NULL;
	}

	m_pDlgProgress	= new CDlgProgress;
	if(!m_pDlgProgress->GetSafeHwnd())
	{
		m_pDlgProgress->Create(IDD_DIALOG_HGPROGRESS, this);
	}
	m_pDlgProgress->ShowWindow(SW_SHOW);
	m_pDlgProgress->CenterWindow();
	m_pDlgProgress->InitProgressData();
	m_pDlgProgress->SetProgressPos("", 1);

	CXLControl::GetWmfViewer().CreateOrShowWindow(this, FALSE);

	// 반드시 호출하여 수량산출 옵션에 따라 산출될 수 있도록 한다. 
	m_pStd->CheckCalBM();

	CARcBridgeDataStd *pStd	= m_pStd->m_pARcBridgeDataStd;
	HGINT32 nSize = pStd->GetBridgeSize();
	HGINT32 nCountTotal = nSize * 4 + 1;
	HGDOUBLE dOffsetProgress = 100 / nCountTotal;
	HGINT32 nIdxProgress = 1;
	CString sMsg = _T(""), str = _T("");

	for(HGINT32 ix = 0; ix < pStd->GetBridgeSize(); ix++)
	{
		CRcBridgeApp *pBridge = pStd->GetBridge(ix);
		if(pBridge == NULL)
		{
			nIdxProgress += 10;
			continue;
		}

		COutBM *pBMNor = m_pStd->m_pArrOutBMNormal.GetAt(ix);
		COutBM *pBMCom = m_pStd->m_pArrOutBMCommon.GetAt(ix);
		COutBMTogong *pBMTogong = m_pStd->m_pArrOutBMTogong.GetAt(ix);

		if(pBMNor == NULL || pBMCom == NULL || pBMTogong == NULL)
			continue;

		sMsg = hggettext("일반수량 산출중......");
		str.Format(_T("%s %s"), pBridge->m_strBridgeName, sMsg);
		m_pDlgProgress->SetProgressPos(str, (long)(dOffsetProgress * nIdxProgress++));
		if(bOnlyReplaceString == FALSE)
		{
			pBMNor->CalculateBM(bNewCreateBM);
			pBMCom->CalculateBM();
		}
		else
		{
			pBMNor->ClearBM(TRUE);
			pBMCom->ClearBM(TRUE);
		}

		InsertUserBmDataTreeinfo(0);
		InsertUserBmDataTreeinfo(1);
		
		pBMNor->MergeBMStd();
		pBMNor->MakeBMStringStd();
		pBMCom->MergeBMStd();
		pBMCom->MakeBMStringStd();
	
		str.Format(hggettext("일반수량 삽도 생성 중....."));
		m_pDlgProgress->SetProgressPos(str, (long)(dOffsetProgress * nIdxProgress++));
		if(!bOnlyReplaceString)
		{
			pBMNor->SetInsertDomyunStd();
			pBMCom->SetInsertDomyunStd();
		}

		////////////////////////////////
		sMsg = hggettext("토공수량 산출중......");
		str.Format(_T("%s %s"), pBridge->m_strBridgeName, sMsg);
		m_pDlgProgress->SetProgressPos(str, (long)dOffsetProgress * nIdxProgress++);
		if(bOnlyReplaceString == FALSE)
			pBMTogong->CalculateBM();
		else
			pBMTogong->ClearBM(TRUE);

		InsertUserBmDataTreeinfo(2);

		pBMTogong->MergeBMStd();
		pBMTogong->MakeBMStringStd();

		str.Format(hggettext("토공수량 삽도 생성 중....."));
		m_pDlgProgress->SetProgressPos(str, (long)(dOffsetProgress * nIdxProgress++));
		if(!bOnlyReplaceString)
			pBMTogong->SetInsertDomyunStd();
	}

	str = hggettext("일반수량, 토공수량 집계표 생성중.......");
	m_pDlgProgress->SetProgressPos(str, (long)dOffsetProgress * nIdxProgress++);
	MergeBMStd();
	MakeBMStringStd();

	str = hggettext("철근집계표 생성중.......");
	m_pDlgProgress->SetProgressPos(str, (long)dOffsetProgress * nIdxProgress++);
	if(bOnlyReplaceString == FALSE)
		MakeBMRebarTable();

	m_pDlgProgress->SetProgressPos("수량 산출 완료", 100);

	if(m_pDlgProgress)
	{
		delete m_pDlgProgress;
		m_pDlgProgress = NULL;
	}

	return TRUE;
}


HTREEITEM COutBMDialog::AddItem(CTreeCtrl *pTreeCtrl, structBMTreeInfo *pBMTreeInfo, BOOL bOnlyInit)
{
	if(!pTreeCtrl) return NULL;

	static long nLevelLast	= 0;
	static HTREEITEM hItemLast	= NULL;
	if(bOnlyInit)
	{
		nLevelLast	= 0;
		hItemLast	= NULL;
		return NULL;
	}

	if(pBMTreeInfo->nLevel == 0)
	{
		hItemLast	= pTreeCtrl->InsertItem(pBMTreeInfo->sName, 0, 0);
	}
	else
	{
		long i = 0; for(i = 0; i < nLevelLast - pBMTreeInfo->nLevel + 1; i++)
		{
			hItemLast	= pTreeCtrl->GetParentItem(hItemLast);
			pTreeCtrl->Expand(hItemLast, TVE_EXPAND);
		}

		hItemLast	= pTreeCtrl->InsertItem(pBMTreeInfo->sName, hItemLast, 0);
	}

	pTreeCtrl->SetItemData(hItemLast, (DWORD)pBMTreeInfo);
	pTreeCtrl->SetCheck(hItemLast, pBMTreeInfo->bSelect);

	nLevelLast	= pBMTreeInfo->nLevel;

	return hItemLast;
}

void COutBMDialog::InitBMTreeInfo(BOOL bErase)
{
	AhTPADelete(&m_arrBMTreeInfo,	(structBMTreeInfo*)0);
	AhTPADelete(&m_arrBMTreeInfoCommon,	(structBMTreeInfo*)0);
	AhTPADelete(&m_arrBMTreeInfoTogong,	(structBMTreeInfo*)0);
	AhTPADelete(&m_arrBMTreeInfoRebar,	(structBMTreeInfo*)0);

	// 0 - 일반수량, 1 - 공통수량, 2 - 토공수량, 3 - 철근수량
	for(long kind = 0; kind < 4; kind++)
	{
		InitBMTreeInfoByKind(kind);
	}

	if(bErase)
	{
		EraseBMTreeInfoNonData(&m_arrBMTreeInfo);
		EraseBMTreeInfoNonData(&m_arrBMTreeInfoCommon);
		EraseBMTreeInfoNonData(&m_arrBMTreeInfoTogong);
		EraseBMTreeInfoNonData(&m_arrBMTreeInfoRebar);
	}
}

void COutBMDialog::InitBMTreeInfoByKind(long nKindBM)
{
	// 토공은 따로 뺀다
	if(nKindBM == 2)
	{
		InitBMTreeInfoByKindTogong();
		return;
	}

	HGINT32 cm = (nKindBM == 1) ? 1 : 0;
	HGBOOL bCommon = (nKindBM == 1) ? TRUE : FALSE;

	structBMTreeInfo *pBMTreeInfo	= NULL;

	CTypedPtrArray <CObArray, structBMTreeInfo*> *pArrBMTreeInfo	= NULL;
	switch(nKindBM)
	{
	case 0 : pArrBMTreeInfo	= &m_arrBMTreeInfo; break;
	case 1 : pArrBMTreeInfo	= &m_arrBMTreeInfoCommon; break;
	case 2 : pArrBMTreeInfo	= &m_arrBMTreeInfoTogong; break;
	case 3 : pArrBMTreeInfo	= &m_arrBMTreeInfoRebar; break;
	}
	if(!pArrBMTreeInfo) return;


	CARcBridgeDataStd *pDataStd	= m_pStd->m_pARcBridgeDataStd;
	long nCountBridge	= pDataStd->GetBridgeSize();
	if(nCountBridge == 0) return;

	CString sKindBM	= _T("");
	switch(nKindBM)
	{
		case 0 : sKindBM = hggettext("일반수량"); break;
		case 1 : sKindBM = hggettext("공통수량"); break;
		case 2 : sKindBM = hggettext("토공수량"); break;
		case 3 : sKindBM = hggettext("철근수량"); break;
	}

	BOOL bBM = (nKindBM != 3) ? TRUE : FALSE;
	if(bBM)
	{
		// 총 수량 집계표 //////////////////
		pBMTreeInfo				= new structBMTreeInfo;
		pBMTreeInfo->sName.Format(hggettext("%s 총괄집계표"), sKindBM);
		pBMTreeInfo->nLevel		= 0;
		pBMTreeInfo->nType		= bBM ? 1 : 2;
		pBMTreeInfo->pBMString	= NULL;
		pBMTreeInfo->pBMSumString	= &m_pArrBMStringAll[cm];
		pBMTreeInfo->pBMRebarTable	= NULL;
		pBMTreeInfo->pViewBM	= m_pStd->m_pViewBM;
		pBMTreeInfo->pOutBM		= NULL;
		pBMTreeInfo->pBMSumStringParent	= NULL;
		pBMTreeInfo->nIdxOfParent	= -1;
		pBMTreeInfo->bSelect	= TRUE;
		pBMTreeInfo->nTypeBM	= nKindBM;
		pBMTreeInfo->pArrBM		= NULL;
		pBMTreeInfo->nIdxBridge = -1;
		pArrBMTreeInfo->Add(pBMTreeInfo);	
	}

	// (ARCBRIDGE-1689) 상하행 분리교량이고 상행인경우 철근 총괄집계표 출력
	if(bBM == FALSE && m_pArrRebarTableAll.empty() == FALSE)
	{
		CString sName(_T(""));
		// 교량별 철근 총괄 수량 집계표 //////////////////
		pBMTreeInfo				= new structBMTreeInfo;
		pBMTreeInfo->sName.Format(hggettext("%s 총괄집계표"), sKindBM);
		pBMTreeInfo->nLevel		= 0;
		pBMTreeInfo->nType		= 2;
		pBMTreeInfo->pBMString	= NULL;
		pBMTreeInfo->pBMSumString	= NULL;
		pBMTreeInfo->pBMRebarTable	= m_pArrRebarTableAll[0];
		pBMTreeInfo->pViewBM	= m_pStd->m_pViewBM;
		pBMTreeInfo->pOutBM		= NULL;
		pBMTreeInfo->pBMSumStringParent	= NULL;
		pBMTreeInfo->nIdxOfParent	= -1;
		pBMTreeInfo->bSelect	= TRUE;
		pBMTreeInfo->nTypeBM	= nKindBM;
		pBMTreeInfo->pArrBM		= NULL;
		pBMTreeInfo->nIdxBridge = -1;
		pArrBMTreeInfo->Add(pBMTreeInfo);
	}

	// 교량 집계표
	long nIdxParentTot  = 0;
	for(HGINT32 ix = 0; ix < nCountBridge; ix++)
	{
		CRcBridgeApp *pBridge = pDataStd->GetBridge(ix);
		if(pBridge == NULL)
			continue;

		// 일반수량인지 공통수량인지
		COutBM *pOutBM = (nKindBM == 1) ? m_pStd->m_pArrOutBMCommon.GetAt(ix) : m_pStd->m_pArrOutBMNormal.GetAt(ix);
		if(pOutBM == NULL)
			continue;

		double dStaSt = pBridge->GetStationAtSlabEnd(TRUE,0);

		if (pBridge->m_nSeparBri != 2)
		{
			pBMTreeInfo				= new structBMTreeInfo;
			pBMTreeInfo->sName.Format(hggettext("%s 교량 %s 집계표"), pBridge->m_strBridgeName, sKindBM);
			pBMTreeInfo->nLevel		= 1;
			pBMTreeInfo->nType		= bBM ? 1 : 2;
			pBMTreeInfo->pBMString	= NULL;
			pBMTreeInfo->pBMSumString	=  bBM ? (pBridge->m_nSeparBri == 1 ? &pOutBM->m_pArrBMStringSepBridgeSum : &pOutBM->m_pArrBMStringAllSum) : NULL;
			pBMTreeInfo->pBMRebarTable	= !bBM ? (pBridge->m_nSeparBri == 1 ? &pOutBM->m_rebarSepBridgeSum : &pOutBM->m_rebarTableAll) : NULL;
			pBMTreeInfo->pViewBM	= m_pStd->m_pViewBM;
			pBMTreeInfo->pOutBM		= NULL;
			pBMTreeInfo->pBMSumStringParent	= &m_pArrBMStringAll[cm];
			pBMTreeInfo->nIdxOfParent	= nIdxParentTot; //ix;
			pBMTreeInfo->bSelect	= TRUE;
			pBMTreeInfo->nTypeBM	= nKindBM;
			pBMTreeInfo->pArrBM		= NULL;
			pBMTreeInfo->nIdxBridge = ix;
			pArrBMTreeInfo->Add(pBMTreeInfo);
		}

		HGINT32 iAddLevel = 0;
		if (pBridge->m_nSeparBri == 1 || pBridge->m_nSeparBri == 2)
		{
			iAddLevel = 1;

			//{ARCBRIDGE-2769  상위의 교량의 집계표의 셀참조를 위해서 만듦
			COutBM *pOutBMParent = pOutBM;
			if(ix > 0)
				pOutBMParent = (nKindBM == 1) ? m_pStd->m_pArrOutBMCommon.GetAt(ix-1) : m_pStd->m_pArrOutBMNormal.GetAt(ix-1);
			//}

			CString sBriDir = pDataStd->GetStringBridgeDir(ix);

			pBMTreeInfo				= new structBMTreeInfo;
			pBMTreeInfo->sName.Format(hggettext(" %s 교량 (%s) %s 집계표"), pBridge->m_strBridgeName, sBriDir, sKindBM);
			pBMTreeInfo->nLevel		= 2;
			pBMTreeInfo->nType		= bBM ? 1 : 2;
			pBMTreeInfo->pBMString	= NULL;
			pBMTreeInfo->pBMSumString	= bBM ? &pOutBM->m_pArrBMStringAllSum : NULL;
			pBMTreeInfo->pBMRebarTable	= !bBM ? &pOutBM->m_rebarTableAll : NULL;
			pBMTreeInfo->pViewBM	= m_pStd->m_pViewBM;
			pBMTreeInfo->pOutBM		= NULL;
			pBMTreeInfo->pBMSumStringParent	= bBM ? (pBridge->m_nSeparBri > 0 ? &pOutBMParent->m_pArrBMStringSepBridgeSum : &pOutBM->m_pArrBMStringAllSum) : NULL; 
			pBMTreeInfo->nIdxOfParent	= nIdxParentTot; //ix;
			pBMTreeInfo->bSelect	= TRUE;
			pBMTreeInfo->nTypeBM	= nKindBM;
			pBMTreeInfo->pArrBM		= NULL;
			pBMTreeInfo->nIdxBridge = ix;
			pArrBMTreeInfo->Add(pBMTreeInfo);
			
			if(pBridge->m_nSeparBri == 2) //하행일 경우 다음을 위하여 빼줌
				nIdxParentTot--;
		}
	

		long nIdxOfParent	= 0;
		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// 본체 집계표
		pBMTreeInfo				= new structBMTreeInfo;
		pBMTreeInfo->sName.Format(hggettext("%s 본체 %s 집계표"), pBridge->m_strBridgeName, sKindBM);
		pBMTreeInfo->nLevel		= 2 + iAddLevel;
		pBMTreeInfo->nType		= bBM ? 1 : 2;
		pBMTreeInfo->pBMString	= NULL;
		pBMTreeInfo->pBMSumString	= bBM ? &pOutBM->m_pArrBMStringBridgeSum : NULL;
		pBMTreeInfo->pBMRebarTable	= !bBM ? &pOutBM->m_rebarTableBridgeSum : NULL;
		pBMTreeInfo->pViewBM	= m_pStd->m_pViewBM;
		pBMTreeInfo->pOutBM		= pOutBM;
		pBMTreeInfo->pBMSumStringParent	= &pOutBM->m_pArrBMStringAllSum;
		pBMTreeInfo->nIdxOfParent	= nIdxOfParent++;
		pBMTreeInfo->bSelect	= TRUE;
		pBMTreeInfo->nTypeBM	= nKindBM;
		pBMTreeInfo->pArrBM		= NULL;
		pBMTreeInfo->nIdxBridge = ix;
		pArrBMTreeInfo->Add(pBMTreeInfo);	

		// 본체 산출근거
		// 철근수량집계표에서는 산출근거에 해당하는 철근수량집계표가 필요 없음
		if(bBM)
		{
			pBMTreeInfo				= new structBMTreeInfo;
			pBMTreeInfo->sName.Format(hggettext("%s 본체 산출근거"), pBridge->m_strBridgeName);
			pBMTreeInfo->nLevel		= 3 + iAddLevel;
			pBMTreeInfo->nType		= 0;
			pBMTreeInfo->pBMString	= &pOutBM->m_pArrBMStringBridge;
			pBMTreeInfo->pBMSumString	= NULL;
			pBMTreeInfo->pBMRebarTable	= NULL;
			pBMTreeInfo->pViewBM	= m_pStd->m_pViewBM;
			pBMTreeInfo->pOutBM		= pOutBM;
			pBMTreeInfo->pBMSumStringParent	= &pOutBM->m_pArrBMStringBridgeSum;
			pBMTreeInfo->nIdxOfParent	= 0;
			pBMTreeInfo->bSelect	= TRUE;
			pBMTreeInfo->nTypeBM	= nKindBM;
			pBMTreeInfo->pArrBM		= &pOutBM->m_pArrBMBridgeStd;
			pBMTreeInfo->nIdxBridge = ix;
			pArrBMTreeInfo->Add(pBMTreeInfo);				
		}
		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// 방호벽 집계표
		BOOL bExistGuardWall = pBridge->IsExistGuardWall(bBM == FALSE);
		BOOL bMoument		= pOutBM->m_pArrBMStringMonument.GetSize() > 0? TRUE : FALSE;
		BOOL bExistGuardTot	= bExistGuardWall || bMoument? TRUE : FALSE;
		
		if(bExistGuardTot)
		{
			pBMTreeInfo				= new structBMTreeInfo;
			pBMTreeInfo->sName.Format(hggettext("%s 방호벽 %s 집계표"), pBridge->m_strBridgeName, sKindBM);
			pBMTreeInfo->nLevel		= 2 + iAddLevel;
			pBMTreeInfo->nType		= bBM ? 1 : 2;
			pBMTreeInfo->pBMString	= NULL;
			pBMTreeInfo->pBMSumString	= bBM ? &pOutBM->m_pArrBMStringGuardWallSum : NULL;
			pBMTreeInfo->pBMRebarTable	= !bBM ? &pOutBM->m_rebarTableGuardWallSum : NULL;
			pBMTreeInfo->pViewBM	= m_pStd->m_pViewBM;
			pBMTreeInfo->pOutBM		= pOutBM;
			pBMTreeInfo->pBMSumStringParent	= &pOutBM->m_pArrBMStringAllSum;
			pBMTreeInfo->nIdxOfParent	= nIdxOfParent;
			pBMTreeInfo->bSelect	= TRUE;
			pBMTreeInfo->nTypeBM	= nKindBM;
			pBMTreeInfo->pArrBM		= NULL;
			pBMTreeInfo->nIdxBridge = ix;
			pArrBMTreeInfo->Add(pBMTreeInfo);	
		}

		// 방호벽 산출근거
		// 철근수량집계표에서는 산출근거에 해당하는 철근수량집계표가 필요 없음
		long nIdxGuideWallSum = 0;
		if(bBM)
		{
			for(HGINT32 dan = 0; dan < pBridge->GetQtyHDan(); dan++)
			{
				CGuardWallRC *pGW = pBridge->GetGuardWallByHDan(dan);
				if(pGW == NULL || pGW->IsTypeHDanRoadTotalGuardWall() == FALSE)
					continue;

				pBMTreeInfo				= new structBMTreeInfo;
				pBMTreeInfo->sName.Format(hggettext("%s %s(L%d) 산출근거"), pBridge->m_strBridgeName, pBridge->GetGuardWallName(pGW, TRUE), dan + 1);
				pBMTreeInfo->nLevel		= 3 + iAddLevel;
				pBMTreeInfo->nType		= 0;
				pBMTreeInfo->pBMString	= &pOutBM->m_pArrBMStringGuardWall[dan];
				pBMTreeInfo->pBMSumString	= NULL;
				pBMTreeInfo->pBMRebarTable	= NULL;
				pBMTreeInfo->pViewBM	= m_pStd->m_pViewBM;
				pBMTreeInfo->pOutBM		= pOutBM;
				pBMTreeInfo->pBMSumStringParent	= &pOutBM->m_pArrBMStringGuardWallSum;
				pBMTreeInfo->nIdxOfParent	= nIdxGuideWallSum++;
				pBMTreeInfo->bSelect	= TRUE;
				pBMTreeInfo->nTypeBM	= nKindBM;
				pBMTreeInfo->pArrBM		= &pOutBM->m_pArrBMGuardWallStd[dan];
				pBMTreeInfo->nIdxBridge = ix;
				pArrBMTreeInfo->Add(pBMTreeInfo);	
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// 교명주는 따로 집계표가 없고 방호벽에 포함되어 집계된다. 

		// 교명주 산출근거
		// 철근수량집계표에서는 산출근거에 해당하는 철근수량집계표가 필요 없음
		if(bBM)
		{
			if(pOutBM->m_pArrBMStringMonument.GetSize() > 0)
			{
				pBMTreeInfo				= new structBMTreeInfo;
				pBMTreeInfo->sName.Format(hggettext("%s 교명주 산출근거"), pBridge->m_strBridgeName);
				pBMTreeInfo->nLevel		= 3 + iAddLevel;
				pBMTreeInfo->nType		= 0;
				pBMTreeInfo->pBMString	= &pOutBM->m_pArrBMStringMonument;
				pBMTreeInfo->pBMSumString	= NULL;
				pBMTreeInfo->pBMRebarTable	= NULL;
				pBMTreeInfo->pViewBM	= m_pStd->m_pViewBM;
				pBMTreeInfo->pOutBM		= pOutBM;
				pBMTreeInfo->pBMSumStringParent	= &pOutBM->m_pArrBMStringGuardWallSum;
				pBMTreeInfo->nIdxOfParent	= nIdxGuideWallSum++;
				pBMTreeInfo->bSelect	= TRUE;
				pBMTreeInfo->nTypeBM	= nKindBM;
				pBMTreeInfo->pArrBM		= &pOutBM->m_pArrBMMonumentStd;
				pBMTreeInfo->nIdxBridge = ix;
				pArrBMTreeInfo->Add(pBMTreeInfo);	
			}
		}
		if(bExistGuardTot)
			nIdxOfParent++;


		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// 날개벽 집계표
		//날개벽이 한개라도 설치가 안되어 있을시 아무런 집계표를 만들 필요가 없다
		long nCntWing = 0;
		for(HGINT32 stt = 0; stt < 2; stt++)
		{
			HGBOOL bStt = (stt == 0) ? TRUE : FALSE;
			for(HGINT32 left = 0; left < 2; left++)
			{
				HGBOOL bLeft = (left == 0) ? TRUE : FALSE;
				if(pBridge->IsWingWall(bStt, bLeft) == FALSE)
					continue;
				nCntWing++;
			}
		}

		if(nCntWing > 0)
		{
			//트리메뉴를 구성하기 때문에 철근 테이블의 트리시트 이름도 여기서 작성이 된다.
			pBMTreeInfo				= new structBMTreeInfo;
			pBMTreeInfo->sName.Format(hggettext("%s 날개벽 %s 집계표"), pBridge->m_strBridgeName, sKindBM);
			pBMTreeInfo->nLevel		= 2 + iAddLevel;
			pBMTreeInfo->nType		= bBM ? 1 : 2;
			pBMTreeInfo->pBMString	= NULL;
			pBMTreeInfo->pBMSumString	= bBM ? &pOutBM->m_pArrBMStringWingSum : NULL;
			pBMTreeInfo->pBMRebarTable	= !bBM ? &pOutBM->m_rebarTableWingSum : NULL;
			pBMTreeInfo->pViewBM	= m_pStd->m_pViewBM;
			pBMTreeInfo->pOutBM		= pOutBM;
			pBMTreeInfo->pBMSumStringParent	= &pOutBM->m_pArrBMStringAllSum;
			pBMTreeInfo->nIdxOfParent	= nIdxOfParent++;
			pBMTreeInfo->bSelect	= TRUE;
			pBMTreeInfo->nTypeBM	= nKindBM;
			pBMTreeInfo->pArrBM		= NULL;
			pBMTreeInfo->nIdxBridge = ix;
			pArrBMTreeInfo->Add(pBMTreeInfo);	

			// 날개벽 산출근거
			// 철근수량집계표에서는 산출근거에 해당하는 철근수량집계표가 필요 없음
			if(bBM)
			{
				HGINT32 nIdxOfParentWing = 0;
				for(HGINT32 stt = 0; stt < 2; stt++)
				{
					HGBOOL bStt = (stt == 0) ? TRUE : FALSE;
					CString sStt = bStt ? hggettext("시점") : hggettext("종점");
					for(HGINT32 left = 0; left < 2; left++)
					{
						HGBOOL bLeft = (left == 0) ? TRUE : FALSE;
						CString sLeft = bLeft ? hggettext("(좌측)") : hggettext("(우측)");
						if(pBridge->IsWingWall(bStt, bLeft) == FALSE)
							continue;

						pBMTreeInfo				= new structBMTreeInfo;
						pBMTreeInfo->sName.Format(hggettext("%s %s 날개벽%s 산출근거"), pBridge->m_strBridgeName, sStt, sLeft);
						pBMTreeInfo->nLevel		= 3 + iAddLevel;
						pBMTreeInfo->nType		= 0;
						pBMTreeInfo->pBMString	= &pOutBM->m_pArrBMStringWing[stt][left];
						pBMTreeInfo->pBMSumString	= NULL;
						pBMTreeInfo->pBMRebarTable	= NULL;
						pBMTreeInfo->pViewBM	= m_pStd->m_pViewBM;
						pBMTreeInfo->pOutBM		= pOutBM;
						pBMTreeInfo->pBMSumStringParent	= &pOutBM->m_pArrBMStringWingSum;
						pBMTreeInfo->nIdxOfParent	= nIdxOfParentWing++;
						pBMTreeInfo->bSelect	= TRUE;
						pBMTreeInfo->nTypeBM	= nKindBM;
						pBMTreeInfo->pArrBM		= &pOutBM->m_pArrBMWingStd[stt][left];
						pBMTreeInfo->nIdxBridge = ix;
						pArrBMTreeInfo->Add(pBMTreeInfo);	
					}
				}
			}
		}			
				
		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// 접속슬래브 집계표
		//접속슬래브가 아예 없는 경우 철근 집계표는 구하지 않는다.
		long nCntAps = 0;
		for(HGINT32 stt = 0; stt < 2; stt++)
		{
			HGBOOL bStt = (stt == 0) ? TRUE : FALSE;
			for(HGINT32 left = 0; left < 2; left++)
			{
				HGBOOL bLeft = (left == 0) ? TRUE : FALSE;
				if(pBridge->IsAps(bStt, bLeft) == FALSE)
					continue;

				nCntAps++;						
			}
		}

		BOOL bApsInstall = nCntAps > 0? TRUE : FALSE;
		if(bApsInstall)
		{
			pBMTreeInfo				= new structBMTreeInfo;
			pBMTreeInfo->sName.Format(hggettext("%s 접속슬래브 %s 집계표"), pBridge->m_strBridgeName, sKindBM);
			pBMTreeInfo->nLevel		= 2 + iAddLevel;
			pBMTreeInfo->nType		= bBM ? 1 : 2;
			pBMTreeInfo->pBMString	= NULL;
			pBMTreeInfo->pBMSumString	= bBM ? &pOutBM->m_pArrBMStringApsSum : NULL;
			pBMTreeInfo->pBMRebarTable	= !bBM ? &pOutBM->m_rebarTableApsSum : NULL;
			pBMTreeInfo->pViewBM	= m_pStd->m_pViewBM;
			pBMTreeInfo->pOutBM		= pOutBM;
			pBMTreeInfo->pBMSumStringParent	= &pOutBM->m_pArrBMStringAllSum;
			pBMTreeInfo->nIdxOfParent	= nIdxOfParent++;
			pBMTreeInfo->bSelect	= TRUE;
			pBMTreeInfo->nTypeBM	= nKindBM;
			pBMTreeInfo->pArrBM		= NULL;
			pBMTreeInfo->nIdxBridge = ix;
			pArrBMTreeInfo->Add(pBMTreeInfo);	
		}


		// 접속슬래브 산출근거
		// 철근수량집계표에서는 산출근거에 해당하는 철근수량집계표가 필요 없음
		if(bBM)
		{
			HGINT32 nIdxOfParentAps = 0;
			for(HGINT32 stt = 0; stt < 2; stt++)
			{
				HGBOOL bStt = (stt == 0) ? TRUE : FALSE;
				CString sStt = bStt ? hggettext("시점") : hggettext("종점");
				for(HGINT32 left = 0; left < 2; left++)
				{
					HGBOOL bLeft = (left == 0) ? TRUE : FALSE;
					CString sLeft = bLeft ? hggettext("(좌측)") : hggettext("(우측)");
					if(bLeft && pBridge->IsAps(bStt, !bLeft) == FALSE)
						sLeft = _T("");

					if(pBridge->IsAps(bStt, bLeft) == FALSE)
						continue;

					pBMTreeInfo				= new structBMTreeInfo;
					pBMTreeInfo->sName.Format(hggettext("%s %s 접속슬래브%s 산출근거"), pBridge->m_strBridgeName, sStt, sLeft);
					pBMTreeInfo->nLevel		= 3 + iAddLevel;
					pBMTreeInfo->nType		= 0;
					pBMTreeInfo->pBMString	= &pOutBM->m_pArrBMStringAps[stt][left];
					pBMTreeInfo->pBMSumString	= NULL;
					pBMTreeInfo->pBMRebarTable	= NULL;
					pBMTreeInfo->pViewBM	= m_pStd->m_pViewBM;
					pBMTreeInfo->pOutBM		= pOutBM;
					pBMTreeInfo->pBMSumStringParent	= &pOutBM->m_pArrBMStringApsSum;
					pBMTreeInfo->nIdxOfParent	= nIdxOfParentAps++;
					pBMTreeInfo->bSelect	= TRUE;
					pBMTreeInfo->nTypeBM	= nKindBM;
					pBMTreeInfo->pArrBM		= &pOutBM->m_pArrBMApsStd[stt][left];
					pBMTreeInfo->nIdxBridge = ix;
					pArrBMTreeInfo->Add(pBMTreeInfo);	
				}
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// 완충슬래브 집계표
		BOOL bCns = FALSE;
		for (long stt =0; stt <= iEND; stt++)
		{
			BOOL bStt = stt == 0;
			for (long left =0; left <= iRIGHT; left++)
			{
				BOOL bLeft = left == 0;

				for(long cns = 0; cns < 2; cns++)
				{
					CApsApp *pCns	= bStt ? &pBridge->m_cnsStt[left][cns] : &pBridge->m_cnsEnd[left][cns];
					if(!pCns->m_bIs) continue;

					bCns = TRUE;
				}
			}
		}

		if(bCns)
		{
			pBMTreeInfo				= new structBMTreeInfo;
			pBMTreeInfo->sName.Format(hggettext("%s 완충슬래브 %s 집계표"), pBridge->m_strBridgeName, sKindBM);
			pBMTreeInfo->nLevel		= 2 + iAddLevel;
			pBMTreeInfo->nType		= bBM ? 1 : 2;
			pBMTreeInfo->pBMString	= NULL;
			pBMTreeInfo->pBMSumString	= bBM ? &pOutBM->m_pArrBMStringCnsSum : NULL;
			pBMTreeInfo->pBMRebarTable	= !bBM ? &pOutBM->m_rebarTableCnsSum : NULL;
			pBMTreeInfo->pViewBM	= m_pStd->m_pViewBM;
			pBMTreeInfo->pOutBM		= pOutBM;
			pBMTreeInfo->pBMSumStringParent	= &pOutBM->m_pArrBMStringAllSum;
			pBMTreeInfo->nIdxOfParent	= nIdxOfParent++;
			pBMTreeInfo->bSelect	= TRUE;
			pBMTreeInfo->nTypeBM	= nKindBM;
			pBMTreeInfo->pArrBM		= NULL;
			pBMTreeInfo->nIdxBridge = ix;
			pArrBMTreeInfo->Add(pBMTreeInfo);	
		}				

		// 완충슬래브 산출근거
		// 철근수량집계표에서는 산출근거에 해당하는 철근수량집계표가 필요 없음
		if(bBM)
		{
			HGINT32 nIdxOfParentCns = 0;
			for(HGINT32 stt = 0; stt < 2; stt++)
			{
				HGBOOL bStt = (stt == 0) ? TRUE : FALSE;
				CString sStt = bStt ? hggettext("시점") : hggettext("종점");
				for(HGINT32 left = 0; left < 2; left++)
				{
					HGBOOL bLeft = (left == 0) ? TRUE : FALSE;
					CString sLeft = bLeft ? hggettext("좌측") : hggettext("우측");

					if(bLeft && pBridge->IsCns(bStt, !bLeft, 0) == FALSE && pBridge->IsCns(bStt, !bLeft, 1) == FALSE)
						sLeft = _T("");

					for(HGINT32 cns = 0; cns < 2; cns++)
					{
						CString sIdx = (cns == 0) ? _T("1") : _T("2");
						if(pBridge->IsCns(bStt, bLeft, cns) == FALSE)
							continue;

						if(cns == 0 && pBridge->IsCns(bStt, bLeft, cns + 1) == FALSE)
							sIdx = _T("");

						pBMTreeInfo				= new structBMTreeInfo;
						if(sLeft.IsEmpty() && sIdx.IsEmpty())
							pBMTreeInfo->sName.Format(hggettext("%s %s 완충슬래브 산출근거"), pBridge->m_strBridgeName, sStt, sLeft, cns + 1);
						else
							pBMTreeInfo->sName.Format(hggettext("%s %s 완충슬래브(%s%d) 산출근거"), pBridge->m_strBridgeName, sStt, sLeft, cns + 1);
						pBMTreeInfo->nLevel		= 3 + iAddLevel;
						pBMTreeInfo->nType		= 0;
						pBMTreeInfo->pBMString	= &pOutBM->m_pArrBMStringCns[stt][left][cns];
						pBMTreeInfo->pBMSumString	= NULL;
						pBMTreeInfo->pBMRebarTable	= NULL;
						pBMTreeInfo->pViewBM	= m_pStd->m_pViewBM;
						pBMTreeInfo->pOutBM		= pOutBM;
						pBMTreeInfo->pBMSumStringParent	= &pOutBM->m_pArrBMStringCnsSum;
						pBMTreeInfo->nIdxOfParent	= nIdxOfParentCns++;
						pBMTreeInfo->bSelect	= TRUE;
						pBMTreeInfo->nTypeBM	= nKindBM;
						pBMTreeInfo->pArrBM		= &pOutBM->m_pArrBMCnsStd[stt][left][cns];
						pBMTreeInfo->nIdxBridge = ix;
						pArrBMTreeInfo->Add(pBMTreeInfo);	
					}
				}
			}
		}

		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// 보호블럭 집계표
		BOOL bBoho = FALSE;
		for(long stt = 0; stt < 2; stt++)
		{
			BOOL bStt	= stt == iSTT;
			if(pBridge->m_nTypeRcBridgeBlock == 0) continue;

			// 보호블럭 라인을 구한다..
			CTwinVectorArray tvArrBoho;
			pBridge->GetTvSungtoLine(tvArrBoho, bStt, TRUE, TRUE);

			long nSize	= tvArrBoho.GetSize();
			if(nSize == 0) continue;

			bBoho = TRUE;
		}


		if(bBoho)
		{
			pBMTreeInfo				= new structBMTreeInfo;
			pBMTreeInfo->sName.Format(hggettext("%s 보호블럭 %s 집계표"), pBridge->m_strBridgeName, sKindBM);
			pBMTreeInfo->nLevel		= 2 + iAddLevel;
			pBMTreeInfo->nType		= bBM ? 1 : 2;
			pBMTreeInfo->pBMString	= NULL;
			pBMTreeInfo->pBMSumString	= bBM ? &pOutBM->m_pArrBMStringBohoSum : NULL;
			pBMTreeInfo->pBMRebarTable	= !bBM ? &pOutBM->m_rebarTableBohoSum : NULL;
			pBMTreeInfo->pViewBM	= m_pStd->m_pViewBM;
			pBMTreeInfo->pOutBM		= pOutBM;
			pBMTreeInfo->pBMSumStringParent	= &pOutBM->m_pArrBMStringAllSum;
			pBMTreeInfo->nIdxOfParent	= nIdxOfParent++;
			pBMTreeInfo->bSelect	= TRUE;
			pBMTreeInfo->nTypeBM	= nKindBM;
			pBMTreeInfo->pArrBM		= NULL;
			pBMTreeInfo->nIdxBridge = ix;
			pArrBMTreeInfo->Add(pBMTreeInfo);	

		}
		// 보호블럭 산출근거
		// 철근수량집계표에서는 산출근거에 해당하는 철근수량집계표가 필요 없음
		if(bBM && bBoho)
		{
			for(HGINT32 stt = 0; stt < 2; stt++)
			{
				pBMTreeInfo				= new structBMTreeInfo;
				pBMTreeInfo->sName.Format(hggettext("%s %s 보호블럭 산출근거"), pBridge->m_strBridgeName, (stt == 0) ? hggettext("시점측") : hggettext("종점측"));
				pBMTreeInfo->nLevel		= 3 + iAddLevel;
				pBMTreeInfo->nType		= 0;
				pBMTreeInfo->pBMString	= &pOutBM->m_pArrBMStringBoho[stt];
				pBMTreeInfo->pBMSumString	= NULL;
				pBMTreeInfo->pBMRebarTable	= NULL;
				pBMTreeInfo->pViewBM	= m_pStd->m_pViewBM;
				pBMTreeInfo->pOutBM		= pOutBM;
				pBMTreeInfo->pBMSumStringParent	= &pOutBM->m_pArrBMStringBohoSum;
				pBMTreeInfo->nIdxOfParent	= stt;
				pBMTreeInfo->bSelect	= TRUE;
				pBMTreeInfo->nTypeBM	= nKindBM;
				pBMTreeInfo->pArrBM		= &pOutBM->m_pArrBMBohoStd[stt];
				pBMTreeInfo->nIdxBridge = ix;
				pArrBMTreeInfo->Add(pBMTreeInfo);	
			}
		}


		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// 현장타설 집계표
		BOOL bHyunTa = FALSE;
		for (long ifooting =0; ifooting < pBridge->GetCountFooting(); ifooting++)
		{
			CFootingApp *pFooting = pBridge->GetFooting(ifooting);
			if(pFooting == NULL) continue;

			CExPileApp *pPile = &pFooting->m_exFooting.m_Pile;
			if(pPile == NULL) continue;

			if(pPile->m_nType != PILE_TYPE_HYUNJANG)
				continue;

			bHyunTa = TRUE;
		}

		BOOL bApplyHyunTa = ((nKindBM == 0 || nKindBM == 1) == FALSE);	//ARCBRIDGE-3613
		if(bHyunTa && bApplyHyunTa)
		{
			pBMTreeInfo				= new structBMTreeInfo;
			pBMTreeInfo->sName.Format(hggettext("%s 현장타설 %s 집계표"), pBridge->m_strBridgeName, sKindBM);
			pBMTreeInfo->nLevel		= 2 + iAddLevel;
			pBMTreeInfo->nType		= bBM ? 1 : 2;
			pBMTreeInfo->pBMString	= NULL;
			pBMTreeInfo->pBMSumString	= NULL;
			pBMTreeInfo->pBMRebarTable	= !bBM ? &pOutBM->m_rebarTableHuyntaSum : NULL;
			pBMTreeInfo->pViewBM	= m_pStd->m_pViewBM;
			pBMTreeInfo->pOutBM		= pOutBM;
			pBMTreeInfo->pBMSumStringParent	= &pOutBM->m_pArrBMStringAllSum;
			pBMTreeInfo->nIdxOfParent	= nIdxOfParent++;
			pBMTreeInfo->bSelect	= TRUE;
			pBMTreeInfo->nTypeBM	= nKindBM;
			pBMTreeInfo->pArrBM		= NULL;
			pBMTreeInfo->nIdxBridge = ix;
			pArrBMTreeInfo->Add(pBMTreeInfo);	
		}
	    nIdxParentTot++;
	}
}


void COutBMDialog::InitBMTreeInfoByKindTogong()
{
	CTypedPtrArray <CObArray, structBMTreeInfo*> *pArrBMTreeInfo	= &m_arrBMTreeInfoTogong;
	if(!pArrBMTreeInfo)	return;

	CARcBridgeDataStd *pDataStd	= m_pStd->m_pARcBridgeDataStd;

	CString sKindBM = hggettext("토공수량");

	structBMTreeInfo *pBMTreeInfo = new structBMTreeInfo;
	pBMTreeInfo->sName.Format("%s %s", sKindBM, _T(" 총괄집계표"));
	pBMTreeInfo->nLevel = 0;
	pBMTreeInfo->nType = 1;
	pBMTreeInfo->pBMString	= NULL;
	pBMTreeInfo->pBMSumString	= &m_pArrBMStringTogong;
	pBMTreeInfo->pBMRebarTable	= NULL;
	pBMTreeInfo->pViewBM	= m_pStd->m_pViewBM;
	pBMTreeInfo->pOutBM		= NULL;
	pBMTreeInfo->pBMSumStringParent	= NULL;
	pBMTreeInfo->nIdxOfParent	= -1;
	pBMTreeInfo->bSelect	= TRUE;
	pBMTreeInfo->nTypeBM	= 2;
	pBMTreeInfo->pArrBM		= NULL;
	pBMTreeInfo->nIdxBridge = -1;
	pArrBMTreeInfo->Add(pBMTreeInfo);


	for(HGINT32 ix = 0; ix < m_pStd->m_pArrOutBMTogong.GetSize(); ix++)
	{
		CRcBridgeApp *pBridge = m_pStd->m_pARcBridgeDataStd->GetBridge(ix);
		if(pBridge == NULL)
			continue;

		COutBMTogong *pBMTogong = m_pStd->m_pArrOutBMTogong.GetAt(ix);
		if(pBMTogong == NULL)
			continue;

		if (pBridge->m_nSeparBri != 2)
		{
			pBMTreeInfo				= new structBMTreeInfo;
			pBMTreeInfo->sName.Format("%s 교량 %s 집계표", pBridge->m_strBridgeName, sKindBM);
			pBMTreeInfo->nLevel		= 1;
			pBMTreeInfo->nType		= 1;
			pBMTreeInfo->pBMString	= NULL;
			pBMTreeInfo->pBMSumString	= pBridge->m_nSeparBri == 1 ? &pBMTogong->m_pArrBMStringSepBridgeSum : &pBMTogong->m_pArrBMStringAllSum;
			pBMTreeInfo->pBMRebarTable	= NULL;
			pBMTreeInfo->pViewBM	= m_pStd->m_pViewBM;
			pBMTreeInfo->pOutBM		= NULL;
			pBMTreeInfo->pBMSumStringParent	=  &m_pArrBMStringTogong;
			pBMTreeInfo->nIdxOfParent	= ix;
			pBMTreeInfo->bSelect	= TRUE;
			pBMTreeInfo->nTypeBM	= 2;
			pBMTreeInfo->pArrBM		= NULL;
			pBMTreeInfo->nIdxBridge = ix;
			pArrBMTreeInfo->Add(pBMTreeInfo);
		}

		HGINT32 iAddLevel = 0;
		if (pBridge->m_nSeparBri == 1 || pBridge->m_nSeparBri == 2)
		{
			iAddLevel = 1;

			pBMTreeInfo				= new structBMTreeInfo;
			pBMTreeInfo->sName.Format("%s 교량 (%s) %s 집계표", pBridge->m_strBridgeName, pBridge->m_nSeparBri == 1 ? pDataStd->m_strLineDirName : pDataStd->m_strLineRevDirName, sKindBM);
			pBMTreeInfo->nLevel		= 2;
			pBMTreeInfo->nType		= 1;
			pBMTreeInfo->pBMString	= NULL;
			pBMTreeInfo->pBMSumString	= &pBMTogong->m_pArrBMStringAllSum;
			pBMTreeInfo->pBMRebarTable	= NULL;
			pBMTreeInfo->pViewBM	= m_pStd->m_pViewBM;
			pBMTreeInfo->pOutBM		= NULL;
			pBMTreeInfo->pBMSumStringParent	=  &m_pArrBMStringTogong;
			pBMTreeInfo->nIdxOfParent	= ix;
			pBMTreeInfo->bSelect	= TRUE;
			pBMTreeInfo->nTypeBM	= 2;
			pBMTreeInfo->pArrBM		= NULL;
			pBMTreeInfo->nIdxBridge = ix;
			pArrBMTreeInfo->Add(pBMTreeInfo);
		}

			long nIdxOfParent	= 0;
			pBMTreeInfo				= new structBMTreeInfo;
			pBMTreeInfo->sName.Format("%s 토공 집계표", sKindBM);
			pBMTreeInfo->nLevel		= 2 + iAddLevel;
			pBMTreeInfo->nType		= 1;
			pBMTreeInfo->pBMString	= NULL;
			pBMTreeInfo->pBMSumString	= &pBMTogong->m_pArrBMStringTogongSum;
			pBMTreeInfo->pBMRebarTable	= NULL;
			pBMTreeInfo->pViewBM	= m_pStd->m_pViewBM;
			pBMTreeInfo->pOutBM		= NULL;
			pBMTreeInfo->pBMSumStringParent	=  &pBMTogong->m_pArrBMStringAllSum;
			pBMTreeInfo->nIdxOfParent	= nIdxOfParent++;
			pBMTreeInfo->bSelect	= TRUE;
			pBMTreeInfo->nTypeBM	= 2;
			pBMTreeInfo->pArrBM		= NULL;
			pBMTreeInfo->nIdxBridge = ix;
			pArrBMTreeInfo->Add(pBMTreeInfo);

			for(HGINT32 jx = 0; jx <= pBridge->m_nQtyJigan; jx++)
			{
				if(pBridge->IsBoxType()) 
				{
					if(jx > 0) 
						continue;
				}

				CString sJijum	= _T("");
				if(pBridge->IsBoxType()) 
					sJijum = _T("함형라멘교 기초");
				else if(jx == 0)
					sJijum = _T("시점측");
				else if(jx == pBridge->m_nQtyJigan)
					sJijum = _T("종점측");
				else 
					sJijum.Format("중간지점%d", jx);


				// 토공 산출근거
				pBMTreeInfo				= new structBMTreeInfo;
				pBMTreeInfo->sName.Format(_T("%s 토공 산출근거"), sJijum);
				pBMTreeInfo->nLevel		= 3 + iAddLevel;
				pBMTreeInfo->nType		= 0;
				pBMTreeInfo->pBMString	= &pBMTogong->m_pArrBMStringTogong[jx];
				pBMTreeInfo->pBMSumString	= NULL;
				pBMTreeInfo->pBMRebarTable	= NULL;
				pBMTreeInfo->pViewBM	= m_pStd->m_pViewBM;
				pBMTreeInfo->pOutBM		= pBMTogong;
				pBMTreeInfo->pBMSumStringParent	= &pBMTogong->m_pArrBMStringTogongSum;
				pBMTreeInfo->nIdxOfParent	= jx;
				pBMTreeInfo->bSelect	= TRUE;
				pBMTreeInfo->nTypeBM	= 2;
				pBMTreeInfo->pArrBM		= &pBMTogong->m_pArrBMTogongStd[jx];
				pBMTreeInfo->nIdxBridge = ix;
				pArrBMTreeInfo->Add(pBMTreeInfo);
			}


			///////////////////////////////////////////////////////////////////////////////////////////////////
			BOOL bBoho = FALSE;
			for(long stt = 0; stt < 2; stt++)
			{
				BOOL bStt	= stt == iSTT;
				if(pBridge->m_nTypeRcBridgeBlock == 0) continue;

				// 보호블럭 라인을 구한다..
				CTwinVectorArray tvArrBoho;
				pBridge->GetTvSungtoLine(tvArrBoho, bStt, TRUE, TRUE);

				long nSize	= tvArrBoho.GetSize();
				if(nSize == 0) continue;

				bBoho = TRUE;
			}

			if(bBoho)
			{
				pBMTreeInfo				= new structBMTreeInfo;
				pBMTreeInfo->sName.Format("%s 보호블럭 집계표", sKindBM);
				pBMTreeInfo->nLevel		= 2 + iAddLevel;
				pBMTreeInfo->nType		= 1;
				pBMTreeInfo->pBMString	= NULL;
				pBMTreeInfo->pBMSumString	= &pBMTogong->m_pArrBMStringBohoSum;
				pBMTreeInfo->pBMRebarTable	= NULL;
				pBMTreeInfo->pViewBM	= m_pStd->m_pViewBM;
				pBMTreeInfo->pOutBM		= NULL;
				pBMTreeInfo->pBMSumStringParent	=  &pBMTogong->m_pArrBMStringAllSum;
				pBMTreeInfo->nIdxOfParent	= nIdxOfParent++;
				pBMTreeInfo->bSelect	= TRUE;
				pBMTreeInfo->nTypeBM	= 2;
				pBMTreeInfo->pArrBM		= NULL;
				pBMTreeInfo->nIdxBridge = ix;
				pArrBMTreeInfo->Add(pBMTreeInfo);

				for(HGINT32 stt = 0; stt < 2; stt++)
				{
					CString sJijum	= (stt == 0) ? _T("시점측") : _T("종점측");

					// 보호블럭 산출근거
					pBMTreeInfo				= new structBMTreeInfo;
					pBMTreeInfo->sName.Format(_T("%s 보호블럭 산출근거"), sJijum);
					pBMTreeInfo->nLevel		= 3 + iAddLevel;
					pBMTreeInfo->nType		= 0;
					pBMTreeInfo->pBMString	= &pBMTogong->m_pArrBMStringBoho[stt];
					pBMTreeInfo->pBMSumString	= NULL;
					pBMTreeInfo->pBMRebarTable	= NULL;
					pBMTreeInfo->pViewBM	= m_pStd->m_pViewBM;
					pBMTreeInfo->pOutBM		= pBMTogong;
					pBMTreeInfo->pBMSumStringParent	= &pBMTogong->m_pArrBMStringBohoSum;
					pBMTreeInfo->nIdxOfParent	= stt;
					pBMTreeInfo->bSelect	= TRUE;
					pBMTreeInfo->nTypeBM	= 2;
					pBMTreeInfo->pArrBM		= &pBMTogong->m_pArrBMBohoStd[stt];
					pBMTreeInfo->nIdxBridge = ix;
					pArrBMTreeInfo->Add(pBMTreeInfo);
				}
			}		
	}
}


void COutBMDialog::SetTreeCtrl(CTreeCtrl *pTreeCtrl)
{
	m_pTreeCtrl = pTreeCtrl;
}

void COutBMDialog::InitTree(CTreeCtrl *pTreeCtrl, long nKindBM)
{
	if(!pTreeCtrl)	
		pTreeCtrl = m_pTreeCtrl;

	m_nKindBM = nKindBM;

	CTypedPtrArray <CObArray, structBMTreeInfo*> *pArrBMTreeInfo	= NULL;
	switch(nKindBM)
	{
	case 0 : pArrBMTreeInfo	= &m_arrBMTreeInfo; break;
	case 1 : pArrBMTreeInfo	= &m_arrBMTreeInfoCommon; break;
	case 2 : pArrBMTreeInfo	= &m_arrBMTreeInfoTogong; break;
	case 3 : pArrBMTreeInfo	= &m_arrBMTreeInfoRebar; break;
	}
	if(!pArrBMTreeInfo) return;

	// 트리내용을 트리에 적용
	pTreeCtrl->DeleteAllItems();

	HTREEITEM hItemParent	= NULL;
	AddItem(pTreeCtrl, NULL, TRUE);	// 초기화만
	if(pArrBMTreeInfo->GetSize() > 0)
		hItemParent	= AddItem(pTreeCtrl, pArrBMTreeInfo->GetAt(0));

	for(long i = 1; i < pArrBMTreeInfo->GetSize(); i++)
	{
		structBMTreeInfo *pBMTreeInfo	= pArrBMTreeInfo->GetAt(i);
		hItemParent	= pTreeCtrl->GetParentItem(AddItem(pTreeCtrl, pBMTreeInfo));
		if(hItemParent)
			pTreeCtrl->Expand(hItemParent, TVE_EXPAND);
	}
}


void COutBMDialog::EraseBMTreeInfoNonData(CTypedPtrArray <CObArray, structBMTreeInfo*> *pArrBMTreeInfo)
{
	if(!pArrBMTreeInfo) return;

	BOOL bErase	= FALSE;
	long i = 0; for(i = 0; i < pArrBMTreeInfo->GetSize(); i++)
	{
		structBMTreeInfo *pBMTreeInfo	= pArrBMTreeInfo->GetAt(i);
		bErase	= FALSE;
		if(pBMTreeInfo->nType == 0)
		{ 
			if(pBMTreeInfo->pBMString) bErase	= pBMTreeInfo->pBMString->GetSize() < 2;
			else bErase	= TRUE;
		}
		else if(pBMTreeInfo->nType == 1)
		{
			if(pBMTreeInfo->pBMSumString) bErase	= pBMTreeInfo->pBMSumString->GetSize() < 2;
			else bErase	= TRUE;
		}
		else if(pBMTreeInfo->nType == 2)
		{
			if(pBMTreeInfo->pBMRebarTable) bErase	= pBMTreeInfo->pBMRebarTable->GetCountArrRebar(TRUE) < 1;
			else bErase	= TRUE;
		}

		if(bErase)
		{
			delete pBMTreeInfo;
			pArrBMTreeInfo->RemoveAt(i);
			i--;
		}
	}
}


void COutBMDialog::PrintToExcel(CTypedPtrArray <CObArray, structBMTreeInfo*> *pArrBMTreeInfo)
{
	if(!pArrBMTreeInfo)	return;

	CARcBridgeDataStd *pDataStd	= m_pStd->m_pARcBridgeDataStd;
	if(pDataStd == NULL)
		return;

	m_pBaseBM->m_bRefCellBMSum = m_pStd->m_bRefCellBMSum;
	m_bCancelPrint = FALSE;
	m_bPrintingExcel = TRUE;

	// 파일 이름
	CString sFileName = m_pStd->m_pDoc->GetPathName();
	sFileName = sFileName.Left(sFileName.GetLength() - 4);
	sFileName += _T("_");

	CString sKindBM = _T("");
	switch(m_nKindBM)
	{
	case 0: sKindBM = _T("일반수량 산출서"); break;
	case 1: sKindBM = _T("공통수량 산출서"); break;
	case 2: sKindBM = _T("토공수량 산출서"); break;
	case 3: sKindBM = _T("철근집계표"); break;
	}
	sFileName += sKindBM;

	CString sFileForm = pDataStd->GetStringExcelFileForm();

	CFileDialog dlg(FALSE, "*.xls", sFileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, sFileForm);
	dlg.m_ofn.lpstrTitle = "엑셀 파일저장";

	if(dlg.DoModal() != IDOK) 
	{
		SetControl(FALSE);
		SetFocus();
		m_bPrintingExcel	= FALSE;
		return;
	}

	CXLControl::GetWmfViewer().ClearAll();

	m_btnCancel.SetWindowText("출력 취소");
	m_bEnableOpen	= FALSE;
	m_pBaseBM->m_bRefCellBMSum = m_pStd->m_bRefCellBMSum;
	//////////////////////////////////////////

	// INSERT_ATESTCODE	
	CAttSendMsg attObj(NULL,WM_ATT_MAINMSG,eMsgAppStartBMAtEng,0,eMsgAppFinishBMAtEng,0);




	// 시트 생성
	sFileName = dlg.GetPathName();
	CStringArray	sArrSheet;
	CString strBM = _T("");
	CString sName = _T("");
	long i = 0; for(i = 0; i < pArrBMTreeInfo->GetSize(); i++)
	{
		structBMTreeInfo *pBMTreeInfo = pArrBMTreeInfo->GetAt(i);
		if(pBMTreeInfo->bSelect)
		{
			// () 철근집계표에는 표지가 없다.
			if(pBMTreeInfo->nType != 2)
			{
				if(pBMTreeInfo->nLevel == 0)		// 총괄집계표
				{
					sName = _T("표지");
					sArrSheet.Add(sName);
				}
				else if(pBMTreeInfo->nLevel == 1)	// 교량 집계표
				{
					sName.Format(_T("%s 표지"), pBMTreeInfo->sName);
					sArrSheet.Add(sName);
				}
			}

			sArrSheet.Add(pBMTreeInfo->sName);
		}
	}

	BOOL bVisible = FALSE;
#ifdef _DEBUG
	bVisible = TRUE;
#endif

	m_pBaseBM->CreateExcel(sFileName, sArrSheet, bVisible);


	long nCountSheet = sArrSheet.GetSize();
	SetProgress(TRUE, nCountSheet);

	CString sTitleLeft = pDataStd->m_strProjectName;
	CString sPrintBM = _T("");
	CString sSheetName = _T("");

	long nIdxSheet = sArrSheet.GetSize() - 1;
	for(i = pArrBMTreeInfo->GetSize() - 1; i >= 0; i--)
	{
		structBMTreeInfo *pBMTreeInfo = pArrBMTreeInfo->GetAt(i);
		if(!pBMTreeInfo || !pBMTreeInfo->bSelect)	continue;
		if(m_bCancelPrint)	break;

		CRcBridgeApp *pBridge = NULL;
		if(pBMTreeInfo->nIdxBridge > -1)
			pBridge = pDataStd->GetBridge(pBMTreeInfo->nIdxBridge);

		sSheetName = sArrSheet.GetAt(nIdxSheet--);

		sPrintBM.Format("%s 출력중...", sSheetName);
		m_staticPrintBM.SetWindowText(sPrintBM);

		HGBOOL bStt = (pBMTreeInfo->sName.Find("시점") > -1) ? TRUE : FALSE;
		HGBOOL bLeft = (pBMTreeInfo->sName.Find("(우)") > -1) || (pBMTreeInfo->sName.Find("(우측)") > -1)? FALSE : TRUE;
		

		if(pBMTreeInfo->nType == 0)		// 산출근거
		{
			m_pBaseBM->PrintBMListToExcel(sSheetName, pBMTreeInfo->pBMString, pBMTreeInfo->pBMSumStringParent, pBMTreeInfo->nIdxOfParent, -1, sTitleLeft);
			
			if(pBMTreeInfo->nTypeBM == 2)	// 토공
			{
				if(pBMTreeInfo->pOutBM && sSheetName.Find(_T("토공")) > -1)
				{
					COutBMTogong *pTogong = dynamic_cast<COutBMTogong*>(pBMTreeInfo->pOutBM);
					pTogong->DrawFirstPageTogong(m_pBaseBM, pBMTreeInfo->nIdxOfParent);
				}
				else if(pBMTreeInfo->pOutBM && sSheetName.Find(_T("보호블럭")) > -1)
				{
					COutBMTogong *pTogong = dynamic_cast<COutBMTogong*>(pBMTreeInfo->pOutBM);
					pTogong->DrawFirstPageBoho(m_pBaseBM, bStt);
				}
			}
			else
			{
				if(pBMTreeInfo->pOutBM && sSheetName.Find(_T("본체")) > -1)
				{
					COutBM *pBM = dynamic_cast<COutBM*>(pBMTreeInfo->pOutBM);
					pBM->DrawFirstPageMain(m_pBaseBM, pBMTreeInfo->nTypeBM);
				}
				else if(pBMTreeInfo->pOutBM && sSheetName.Find(_T("(L")) > -1)	// 방호벽
				{
					long nIndexNum = sSheetName.Find(_T("(L"));
					CString szNum = sSheetName.Mid(nIndexNum+2, 2);
					szNum.Replace(_T(")"), _T(""));
					long nHdan = atoi(szNum) - 1;
					CGuardWallRC *pGW = pBridge->GetGuardWallByHDan(nHdan);
					if(pGW && pGW->IsTypeHDanRoadTotalGuardWall())
					{
						COutBM *pBM = dynamic_cast<COutBM*>(pBMTreeInfo->pOutBM);
						pBM->DrawFirstPageGuardWall(m_pBaseBM, nHdan, 0);	// 마지막 nIdx값 의미없음
					}
				}
				else if(pBMTreeInfo->pOutBM && sSheetName.Find(_T("날개벽")) > -1)
				{
					COutBM *pBM = dynamic_cast<COutBM*>(pBMTreeInfo->pOutBM);
					pBM->DrawFirstPageWing(m_pBaseBM, bStt, bLeft);
				}
				else if(pBMTreeInfo->pOutBM && sSheetName.Find(_T("접속슬래브")) > -1)
				{
					COutBM *pBM = dynamic_cast<COutBM*>(pBMTreeInfo->pOutBM);
					pBM->DrawFirstPageAps(m_pBaseBM, bStt, bLeft);
				}
				else if(pBMTreeInfo->pOutBM && sSheetName.Find(_T("교명주")) > -1)
				{
					COutBM *pBM = dynamic_cast<COutBM*>(pBMTreeInfo->pOutBM);
					pBM->DrawFirstPageMonument(m_pBaseBM);
				}
			}
		}
		else if(pBMTreeInfo->nType == 1)	// 집계표
		{
			m_pBaseBM->PrintBMSumListToExcel(sSheetName, pBMTreeInfo->pBMSumString, 2, sArrSheet, TRUE, pBMTreeInfo->pBMSumStringParent, pBMTreeInfo->nIdxOfParent);
			PrintSumTitleToExcel(m_pBaseBM->m_pXL, pBMTreeInfo);

			if(pBMTreeInfo->nLevel == 0)		// 총괄집계표
			{
				sSheetName = sArrSheet.GetAt(nIdxSheet--);
				CString sTitle = _T("총괄 ") + sKindBM + _T(" 집계표");
				m_pBaseBM->MakeExcelMainPage(sSheetName, sTitle);
			}
			else if(pBMTreeInfo->nLevel == 1)	// 교량집계표
			{
				sSheetName = sArrSheet.GetAt(nIdxSheet--);
				CRcBridgeRebar *pBridge = pDataStd->GetBridgeRebar(pBMTreeInfo->nIdxBridge);
				MakeExcelCoverPage(sSheetName, pBridge);
			}
		}
		else if(pBMTreeInfo->nType == 2)	// 철근집계표
		{
			pBMTreeInfo->pBMRebarTable->SetTitle(pBMTreeInfo->sName);
			pBMTreeInfo->pBMRebarTable->PrintToExcel(m_pBaseBM->m_pXL, sSheetName, TRUE);
		}

		SetProgress(FALSE);
	}


	// 엑셀 닫기
	m_pBaseBM->CloseExcel();
	SetProgress(TRUE);
	m_staticPrintBM.SetWindowText(_T("출력완료!!"));

	// 엑셀 출력 후에는 출력파일 열기로 이름 변경
	m_btnCancel.SetWindowText(_T("출력파일 열기"));
	m_bEnableOpen = TRUE;
	m_bPrintingExcel = FALSE;
	SetControl(FALSE);
}


void COutBMDialog::MakeExcelCoverPage(CString sSheetName, CRcBridgeRebar *pBri)
{
	CXLControl *pXL = m_pBaseBM->m_pXL;

	if(!pXL) return;
	// 사용할 시트를 찾아서 활성화 한다.
	if(!pXL->SetActiveSheet(sSheetName)) return;

	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	CXLFormatText::SetExcelFileName("HgExcelBase.dll");
	CXLFormatText XLText(pXL);

	// format 설정
	pXL->SetCellWidth(XL_COLSTT,XL_COLEND,XL_COLWIDTH);
	pXL->SetCellHeight(XL_ROWSTT_BM,XL_ROWEND_BM,XFL_ROWHEIGHT);
	pXL->SetFonts(XL_ROWSTT_BM,XL_COLSTT,XL_ROWEND_BM,XL_COLEND,XL_TEXTSIZE,XL_SHEET_FONT,1,FALSE);
	pXL->SetVerAlign(XL_ROWSTT_BM,XL_COLSTT,XL_ROWEND_BM,XL_COLEND,2); // 2 = CENTER
	pXL->SetHoriAlign(XL_ROWSTT_BM,XL_COLSTT,XL_ROWEND_BM,XL_COLEND,TA_LEFT);

	// 출력
	long nRowSheet = 4;
	CString str, strTemp;
	str.Format("%s 수량산출서",pBri->m_strBridgeName);

	pXL->SetSheetName(sSheetName);
	pXL->SetMergeCell(nRowSheet,XFL_COLSTT,nRowSheet+3,XFL_COLEND);
	// 사각형
	XLText.GoPosition(nRowSheet+4,3);
	XLText.AddFormatText("$h$p[Rectangle]$n");
	//
	pXL->SetFonts(nRowSheet,XFL_COLSTT,XFL_TEXTSIZE*3,XFL_SHEET_FONT);
	pXL->SetHoriAlign(nRowSheet,XFL_COLSTT,nRowSheet+3,XFL_COLEND,TA_CENTER);
	pXL->SetXL(nRowSheet,XFL_COLSTT,str);	

	double dTemp1 = 0;
	double dTemp2 = 0;

	XLText.GoPosition(nRowSheet+10,XFL_COLSTT);
	dTemp1 = (long)toM(pBri->m_dStationBridgeStt)/1000;
	dTemp2 = toM(pBri->m_dStationBridgeStt)-dTemp1*1000;
	str.Format("STA. %d + %.3f", (long)dTemp1, dTemp2);
	XLText.AddFormatText("$h$t$tb13▣ 교 량 위 치$m+06$c$tb13:$r$tb13 %s$n", str);

	if(pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
		str.Format("%s", pBri->GetStringBridgeType());
	else
		str.Format("%d 경간 %s", pBri->m_nQtyJigan, pBri->GetStringBridgeType());
	XLText.AddFormatText("$h$t$tb13▣ 교 량 형 식$m+06$c$tb13:$r$tb13 %s$n", str);

	if(pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
		str.Format("L = %.3f m", toM(pBri->m_dLengthBridge));
	else
		str.Format("L = %.3f m(%s)", toM(pBri->m_dLengthBridge), pBri->GetStringBridgeSpan());
	XLText.AddFormatText("$h$t$tb13▣ 지 간 구 성$m+06$c$tb13:$r$tb13 %s$n", str);

	long nLine = 0;
	long nSpan = 0;
	CDPoint vAng = CDPoint(0,1);
	double dStationBridge = pBri->GetStationOnJijum(nSpan,nLine);
	str.Format("%.3f m", toM(pBri->GetWidthSlabAct(dStationBridge,vAng)));
	XLText.AddFormatText("$h$t$tb13▣ 횡 단 폭 원$m+06$c$tb13:$r$tb13 %s$n", str);

	BOOL bAll = TRUE;
	long i=0; for(i=0; i<pBri->GetCountJijum()-1; i++)
	{
		vAng	= pBri->GetAngleJijum(i);
		dTemp1	= vAng.GetAngleDegree();
		vAng	= pBri->GetAngleJijum(i+1);
		dTemp2	= vAng.GetAngleDegree();

		if(dTemp1 != dTemp2) bAll = FALSE;
	}

	if(bAll)
	{
		vAng	= pBri->GetAngleJijum(i);
		str.Format("%.3f˚", vAng.GetAngleDegree());
	}
	else
	{
		str = _T("");
		long i=0; for(i=0; i<pBri->GetCountJijum(); i++)
		{
			vAng	= pBri->GetAngleJijum(i);
			if(i==0) strTemp.Format("시점:%.3f˚", vAng.GetAngleDegree());
			else if(i==pBri->m_nQtyJigan) strTemp.Format(", 종점:%.3f˚", vAng.GetAngleDegree());
			else strTemp.Format(", 지점%d:%.3f˚", i+1, vAng.GetAngleDegree());

			str += strTemp;
		}
	}
	XLText.AddFormatText("$h$t$tb13▣ SKEW  각 도$m+06$c$tb13:$r$tb13 %s$n", str);

	str = pData->GetStringFootTypeAll(pBri);
	XLText.AddFormatText("$h$t$tb13▣ 기 초 형 식$m+06$c$tb13:$r$tb13 %s$n", str);

	if(pData->m_nTypeConditionApply < DESIGN_CONDITION_RAIL)
		str.Format("%d 등급", pData->m_nDegreeBridge+1);
	else
		str.Format("%d 급선", pData->m_nDegreeRail+1);
	XLText.AddFormatText("$h$t$tb13▣ 교 량 등 급$m+06$c$tb13:$r$tb13 %s$n", str);

	str.Format("%s 등급", (pBri->GetDegreeEarthQuake()==0)? "I":"II");
	XLText.AddFormatText("$h$t$tb13▣ 내 진 등 급$m+06$c$tb13:$r$tb13 %s$n", str);
}


void COutBMDialog::PrintSumTitleToExcel(CXLControl *pXL, structBMTreeInfo *pBMTreeInfo)
{
	if(!pXL || !pBMTreeInfo)	return;

	long nRow = 0;

	// 타이틀 쓰기
	CString	strTable = pBMTreeInfo->sName == _T("") ? _T("집계표") : pBMTreeInfo->sName;
	pXL->SetXL(nRow++, 0, strTable);

	// 가로 타이틀 쓰기
	CStringArray	sArrTitleOfSum;
	GetStringSumTitle(sArrTitleOfSum, pBMTreeInfo);
	long i = 0;
	for(i = 0; i < sArrTitleOfSum.GetSize(); i++)
	{
		CString str = sArrTitleOfSum.GetAt(i);
		pXL->SetXL(nRow, i, str);
	}

	pXL->SetMergeCell(0, 0, 0, sArrTitleOfSum.GetSize() - 1);
	pXL->SetMergeCell(1, 0, 1, 2);
}

void COutBMDialog::SetCheck(CTreeCtrl *pTreeCtrl, HTREEITEM hItem, BOOL bCheck)
{
	if(!pTreeCtrl) return;

	structBMTreeInfo *pBMTreeInfo	= (structBMTreeInfo*)pTreeCtrl->GetItemData(hItem);
	if(!pBMTreeInfo) return;
	pBMTreeInfo->bSelect	= bCheck;
}


// 체크를 설정함
// nType = 0 : 전체 선택
// nType = 1 : 전체 선택 해제
// nType = 2 : 반전
// nType = 3 : 산출근거만 선택 
// nType = 4 : 집계표만 선택
void COutBMDialog::SetCheck(long nType, long nKindBM)
{
	CTypedPtrArray <CObArray, structBMTreeInfo*> *pArrBMTreeInfo	= NULL;
	switch(nKindBM)
	{
	case 0 : pArrBMTreeInfo	= &m_arrBMTreeInfo; break;
	case 1 : pArrBMTreeInfo	= &m_arrBMTreeInfoCommon; break;
	case 2 : pArrBMTreeInfo	= &m_arrBMTreeInfoTogong; break;
	case 3 : pArrBMTreeInfo	= &m_arrBMTreeInfoRebar; break;
	}
	if(!pArrBMTreeInfo) return;

	long i = 0; for(i = 0; i < pArrBMTreeInfo->GetSize(); i++)
	{
		structBMTreeInfo *pBMTreeInfo	= pArrBMTreeInfo->GetAt(i);
		switch(nType)
		{
		case 0 : pBMTreeInfo->bSelect	= TRUE; break;	// 전체 선택
		case 1 : pBMTreeInfo->bSelect	= FALSE; break;	// 전체 선택 해제
		case 2 : pBMTreeInfo->bSelect	= !pBMTreeInfo->bSelect; break;	// 반전
		case 3 : pBMTreeInfo->bSelect	= pBMTreeInfo->nType == 0; break;	// 산출근거만 선택
		case 4 : pBMTreeInfo->bSelect	= pBMTreeInfo->nType == 1; break;	// 집계표만 선택
		}
	}
}

UINT COutBMDialog::DoWork()
{
	CTypedPtrArray <CObArray, structBMTreeInfo*> *pArrBMTreeInfo	= NULL;

	switch(m_nKindBM)
	{
	case 0: pArrBMTreeInfo = &m_arrBMTreeInfo;			break;
	case 1: pArrBMTreeInfo = &m_arrBMTreeInfoCommon;	break;
	case 2: pArrBMTreeInfo = &m_arrBMTreeInfoTogong;	break;
	case 3: pArrBMTreeInfo = &m_arrBMTreeInfoRebar;		break;
	}

	if(!pArrBMTreeInfo)	return 0;

	PrintToExcel(pArrBMTreeInfo);

	return 0;
}


void COutBMDialog::SetDataInit(void *p, BOOL bOtherGrid)
{
	structBMTreeInfo *pBMTreeInfo	= (structBMTreeInfo*)p;
	if(!pBMTreeInfo) return;

	CGridCtrlMng *pGrid	= bOtherGrid ? &m_GridOther : &m_Grid;
	if(bOtherGrid)
	{
		m_pBMTreeInfoOther	= pBMTreeInfo;
		m_bFocusOther	= TRUE;
	}
	else
	{
		m_pBMTreeInfoCur	= pBMTreeInfo;
		m_bFocusOther	= FALSE;
	}


	if(pBMTreeInfo->nType == 1)	// 집계표
	{
		if(pBMTreeInfo->pBMSumString)
		{
			m_pBaseBM->PrintBMSumListToGrid(pGrid, pBMTreeInfo->pBMSumString, 1);
			if(pBMTreeInfo->pBMSumString->GetSize() > 0)
			{
				// 집계표 타이틀 쓰기
				CStringArray sArrTitleOfSum;
				GetStringSumTitle(sArrTitleOfSum, pBMTreeInfo);

				long i = 0; for(i = 0; i < sArrTitleOfSum.GetSize(); i++)
					pGrid->SetTextMatrix(0, i, sArrTitleOfSum.GetAt(i));
				pGrid->SetMergeCol(0, 0, 2);
			}
		}
	}
	else if(pBMTreeInfo->nType == 0)	// 산출근거
	{
		m_pBaseBM->PrintBMListToGrid(pGrid, pBMTreeInfo->pBMString);
	}
	else if(pBMTreeInfo->nType == 2)	// 철근집계표
	{
		if(pBMTreeInfo->pBMRebarTable)
		{
			pBMTreeInfo->pBMRebarTable->SetTitle(pBMTreeInfo->sName);
			pBMTreeInfo->pBMRebarTable->SetDataInit(pGrid);
		}		
	}
	else if(pBMTreeInfo->nType == -1)		// 단위미터 출력일 때 옹벽 구분을 위한 빈 아이템
	{
		pGrid->InitGrid();
		pGrid->SetGridDefault(0, 0, 0, 0);
		pGrid->DeleteAllItems();
	}
}

void COutBMDialog::GetStringSumTitle(CStringArray &sArrTitle, structBMTreeInfo *pBMTreeInfo)
{
	CARcBridgeDataStd *pDataStd	= m_pStd->m_pARcBridgeDataStd;

	CString sNameSheetOrTab = pBMTreeInfo->sName;
	CRcBridgeApp *pBridge = NULL;

	// 총괄집계표에는 nIdxBridge가 셋팅되어있지 않는다. 
	if(pBMTreeInfo->nIdxBridge > -1)
		pBridge = pDataStd->GetBridge(pBMTreeInfo->nIdxBridge);

	HGINT32 nKindBM = 0;
	CString sBM	= _T("일반수량");
	if(sNameSheetOrTab.Find("일반수량", 0) != -1)
	{
		nKindBM = 0;
		sBM	= " 일반수량 ";
	}
	else if(sNameSheetOrTab.Find("공통수량", 0) != -1)
	{
		nKindBM = 1;
		sBM	= " 공통수량 ";
	}
	else if(sNameSheetOrTab.Find("토공수량", 0) != -1)
	{
		nKindBM = 2;
		sBM	= " 토공수량 ";
	}

	sArrTitle.RemoveAll();

	sArrTitle.Add("공종");
	sArrTitle.Add("공종");
	sArrTitle.Add("공종");
	sArrTitle.Add("단위");

	CString str = _T("");
	if(sNameSheetOrTab.Find(hggettext("총괄집계표")) != -1)
	{
		HGINT32 nSize = pDataStd->GetBridgeSize();
		for(HGINT32 ix = 0; ix < nSize; ix++)
		{
			CRcBridgeApp *pBri = pDataStd->GetBridge(ix);
			if (pBri == NULL)
				continue;

			if (pBri->m_nSeparBri == 2)
				continue;

			str.Format(pBri->m_strBridgeName);
			sArrTitle.Add(str);
		}
	}
	else if(sNameSheetOrTab.Find(hggettext("교량") + sBM + hggettext("집계표")) != -1 || 
		sNameSheetOrTab.Find(hggettext("교량") + " (" + pDataStd->m_strLineDirName + ")" + sBM + hggettext("집계표")) != -1 ||
		sNameSheetOrTab.Find(hggettext("교량") + " (" + pDataStd->m_strLineRevDirName + ")" + sBM + hggettext("집계표")) != -1)
	{
		// 토공이 아닐때
		if (pBridge->m_nSeparBri == 1 && (sNameSheetOrTab.Find(pDataStd->m_strLineDirName) < 0 && sNameSheetOrTab.Find(pDataStd->m_strLineRevDirName)))
		{
			sArrTitle.Add(pDataStd->m_strLineDirName);
			sArrTitle.Add(pDataStd->m_strLineRevDirName);
		}
		else if(sBM.Find(hggettext("토공수량"), 0) == -1)
		{
			COutBM *pOutBM = (nKindBM == 1) ? m_pStd->m_pArrOutBMCommon.GetAt(pBMTreeInfo->nIdxBridge) : m_pStd->m_pArrOutBMNormal.GetAt(pBMTreeInfo->nIdxBridge);

			if(pOutBM->m_pArrBMStringBridgeSum.GetSize() > 0)
				sArrTitle.Add(hggettext("본체"));
			if(pOutBM->m_pArrBMStringGuardWallSum.GetSize() > 0)
				sArrTitle.Add(hggettext("방호벽"));
			if(pOutBM->m_pArrBMStringWingSum.GetSize() > 0)
				sArrTitle.Add(hggettext("날개벽"));
			if(pOutBM->m_pArrBMStringApsSum.GetSize() > 0)
				sArrTitle.Add(hggettext("접속슬래브"));
			if(pOutBM->m_pArrBMStringCnsSum.GetSize() > 0)
				sArrTitle.Add(hggettext("완충슬래브"));
			if(pOutBM->m_pArrBMStringBohoSum.GetSize() > 0)
				sArrTitle.Add(hggettext("보호블럭"));
		}
		else
		{
			COutBMTogong *pOutBM = m_pStd->m_pArrOutBMTogong.GetAt(pBMTreeInfo->nIdxBridge);

			if(pOutBM->m_pArrBMStringTogongSum.GetSize() > 0)
				sArrTitle.Add(hggettext("토공"));
			if(pOutBM->m_pArrBMStringBohoSum.GetSize() > 0)
				sArrTitle.Add(hggettext("보호블럭"));
		}
	}
	else if(sNameSheetOrTab.Find(hggettext("본체") + sBM + hggettext("집계표")) != -1)
	{
		sArrTitle.Add(hggettext("본체"));
	}
	else if(sNameSheetOrTab.Find(hggettext("방호벽") + sBM + hggettext("집계표")) != -1)
	{
		// 무조건 추가하지 않고 조건에 맞는 경우에만 add를 하고 있다. 
		// 무조건 추가하려면 COutBM::MergeBMStd(), COutBM::MakeBMStringStd() 에서 갯수가 없는 것을 걸러내지 않고 무조건 Merge해야 한다. 
		for(HGINT32 dan = 0; dan < pBridge->GetQtyHDan(); dan++)
		{
			CGuardWallRC *pGW = pBridge->GetGuardWallByHDan(dan);
			if(pGW == NULL || pGW->IsTypeHDanRoadTotalGuardWall() == FALSE)
				continue;

			str.Format(_T("%s(L%d)"), pBridge->GetGuardWallName(pGW), dan + 1);
			sArrTitle.Add(str);
		}

		COutBM *pOutBM = (nKindBM == 1) ? m_pStd->m_pArrOutBMCommon.GetAt(pBMTreeInfo->nIdxBridge) : m_pStd->m_pArrOutBMNormal.GetAt(pBMTreeInfo->nIdxBridge);
		if(pOutBM->m_pArrBMStringMonument.GetSize() > 0)
			sArrTitle.Add(_T("교명주"));
	}
	else if(sNameSheetOrTab.Find(hggettext("날개벽") + sBM + hggettext("집계표")) != -1)
	{
		// 무조건 추가하지 않고 조건에 맞는 경우에만 add를 하고 있다. 
		// 무조건 추가하려면 COutBM::MergeBMStd(), COutBM::MakeBMStringStd() 에서 갯수가 없는 것을 걸러내지 않고 무조건 Merge해야 한다. 
		for(HGINT32 stt = 0; stt < 2; stt++)
		{
			HGBOOL bStt = (stt == 0) ? TRUE : FALSE;
			CString sStt = bStt ? hggettext("시점") : hggettext("종점");
			for(HGINT32 left = 0; left < 2; left++)
			{
				HGBOOL bLeft = (left == 0) ? TRUE : FALSE;
				CString sLeft = bLeft ? hggettext("좌측") : hggettext("우측");

				if(pBridge->IsWingWall(bStt, bLeft) == FALSE)
					continue;

				str.Format(_T("%s %s"), sStt, sLeft);
				sArrTitle.Add(str);
			}
		}
	}
	else if(sNameSheetOrTab.Find(hggettext("접속슬래브") + sBM + hggettext("집계표")) != -1)
	{
		// 무조건 추가하지 않고 조건에 맞는 경우에만 add를 하고 있다. 
		// 무조건 추가하려면 COutBM::MergeBMStd(), COutBM::MakeBMStringStd() 에서 갯수가 없는 것을 걸러내지 않고 무조건 Merge해야 한다. 
		for(HGINT32 stt = 0; stt < 2; stt++)
		{
			HGBOOL bStt = (stt == 0) ? TRUE : FALSE;
			CString sStt = bStt ? hggettext("시점") : hggettext("종점");
			for(HGINT32 left = 0; left < 2; left++)
			{
				HGBOOL bLeft = (left == 0) ? TRUE : FALSE;
				CString sLeft = bLeft ? hggettext("좌측") : hggettext("우측");

				if(pBridge->IsAps(bStt, bLeft) == FALSE)
					continue;

				// 좌측인데 우측이 없는 경우에는 좌측이란 문자열을 사용하지 않는다. 
				if(bLeft && pBridge->IsAps(bStt, !bLeft) == FALSE)
					str = sStt;
				else
					str.Format(_T("%s %s"), sStt, sLeft);

				sArrTitle.Add(str);
			}
		}
	}
	else if(sNameSheetOrTab.Find(hggettext("완충슬래브") + sBM + hggettext("집계표")) != -1)
	{
		// 무조건 추가하지 않고 조건에 맞는 경우에만 add를 하고 있다. 
		// 무조건 추가하려면 COutBM::MergeBMStd(), COutBM::MakeBMStringStd() 에서 갯수가 없는 것을 걸러내지 않고 무조건 Merge해야 한다. 
			for(HGINT32 stt = 0; stt < 2; stt++)
			{
				HGBOOL bStt = (stt == 0) ? TRUE : FALSE;
				CString sStt = bStt ? hggettext("시점") : hggettext("종점");
				for(HGINT32 left = 0; left < 2; left++)
				{
					HGBOOL bLeft = (left == 0) ? TRUE : FALSE;
					CString sLeft = bLeft ? hggettext("좌측") : hggettext("우측");

					if(bLeft && pBridge->IsCns(bStt, !bLeft, 0) == FALSE && pBridge->IsCns(bStt, !bLeft, 1) == FALSE)
						sLeft = _T("");

					for(HGINT32 cns = 0; cns < 2; cns++)
					{
						CString sIdx = (cns == 0) ? _T("1") : _T("2");
						if(pBridge->IsCns(bStt, bLeft, cns) == FALSE)
							continue;

						if(cns == 0 && pBridge->IsCns(bStt, bLeft, cns + 1) == FALSE)
							sIdx = _T("");

						str.Format(_T("%s %s%s"), sStt, sLeft, sIdx);
						sArrTitle.Add(str);
					}
				}
			}
	}
	else if(sNameSheetOrTab.Find(hggettext("보호블럭") + sBM + hggettext("집계표")) != -1)
	{
		sArrTitle.Add(hggettext("보호블럭"));
	}
	else if(sNameSheetOrTab.Find(_T("토공 집계표")) > -1)		// 토공
	{
		for(HGINT32 jx = 0; jx <= pBridge->m_nQtyJigan; jx++)
		{
			if(pBridge->IsBoxType()) 
			{
				if(jx > 0) 
					continue;
			}
			else if(pBridge->IsFootingJijum(jx) == FALSE)
				continue;

			if(pBridge->IsBoxType()) 
				str = _T("함형라멘교 기초");
			else if(jx == 0)
				str = _T("시점측");
			else if(jx == pBridge->m_nQtyJigan)
				str = _T("종점측");
			else 
				str.Format("중간지점%d", jx);

			sArrTitle.Add(str);
		}
	}
	else if(sNameSheetOrTab.Find(_T("보호블럭 집계표")) > -1)	// 토공
	{
		sArrTitle.Add(_T("시점측"));
		sArrTitle.Add(_T("종점측"));
	}

	sArrTitle.Add(_T("계"));
	sArrTitle.Add(_T("비고"));
}


void COutBMDialog::OnSelchangeComboPreview() 
{
	m_nCountGrid	= m_comboPreview.GetCurSel() + 1;
	SetControl();
	SettingGrid();
}


void COutBMDialog::SettingGrid()
{
	if(!m_bReadyOutBMDlg)	return;

	long nCountGrid = m_nCountGrid;
	BOOL bAliVert = m_nRadioViewVert == 0 ? TRUE : FALSE;
	CXTPClientRect rc(this);
	CRect rcGroup;
	GetDlgItem(IDC_STATIC_OUTBM_VIEW_BOTTOM1)->GetClientRect(rcGroup);
	long nHeightGroup	= rcGroup.Height();
	rc.bottom -= nHeightGroup;

	if(nCountGrid == 1)
	{
		m_Grid.MoveWindow(rc);

		m_GridOther.ShowWindow(FALSE);
	}
	else
	{
		CRect rcOther;
		if(bAliVert)
		{
			rcOther.left	= rc.left + rc.Width()/2;
			rcOther.right	= rc.right;
			rcOther.top		= rc.top;
			rcOther.bottom	= rc.bottom;

			rc.right	= rcOther.left;
		}
		else
		{
			rcOther.top		= rc.top + rc.Height()/2;
			rcOther.bottom	= rc.bottom;
			rcOther.left	= rc.left;
			rcOther.right	= rc.right;

			rc.bottom			= rcOther.top;
		}

		m_Grid.MoveWindow(rc);
		m_GridOther.MoveWindow(rcOther);
		m_GridOther.ShowWindow(TRUE);
	}
}


void COutBMDialog::OnRadioViewHor() 
{
	UpdateData(TRUE);
	SettingGrid();
}

void COutBMDialog::OnRadioViewVert() 
{
	UpdateData(TRUE);
	SettingGrid();
}

void COutBMDialog::OnButtonKindSelect() 
{
	CDialogBMSelectKind dlg;
	CViewBM *pViewBM	= NULL;
	switch(m_nKindBM)
	{
	case 0: pViewBM	= m_pStd->m_pViewBM; break;
	case 1: pViewBM	= m_pStd->m_pViewBMCommon; break;
	case 2: pViewBM	= m_pStd->m_pViewBMTogong; break;
	default: break;

	}

	if(pViewBM)
	{
		dlg.SetViewBM(pViewBM);
		if(dlg.DoModal() == IDOK)
		{
			CreateBM(FALSE, FALSE);
			if(m_pDlgProgress && m_pDlgProgress->GetSafeHwnd())
				m_pDlgProgress->ShowWindow(SW_HIDE);
		}
	}

	InitBMTreeInfo();
	InitTree(NULL, m_nKindBM);
}

void COutBMDialog::OnButtonUserBmInput() 
{
	CARcBridgeDataStd *pDataStd	= m_pStd->m_pARcBridgeDataStd;

	CDialogUserBmInput dlg;
	dlg.SetData(pDataStd->m_pUserBmData, m_nKindBM);
	dlg.DoModal();
}

void COutBMDialog::OnButtonUserBmApply()
{
	CARcBridgeDataStd *pDataStd	= m_pStd->m_pARcBridgeDataStd;

	std::vector<structPageIndexInfo> vArrTreeInfo;
	GetTreeInfoArray(vArrTreeInfo, m_nKindBM);

	CDialogUserBmApply dlg2;
	dlg2.SetData(pDataStd->m_pUserBmData, m_nKindBM);
	dlg2.SetDataTreeItemInfo(vArrTreeInfo);
	if(dlg2.DoModal() == IDOK)
	{
		CreateBM();
		InitBMTreeInfo();
		InitTree(NULL, m_nKindBM);
	}
}

void COutBMDialog::OnButtonWmf() 
{
	CXLControl::GetWmfViewer().CreateOrShowWindow(this,TRUE);
}


void COutBMDialog::OnButtonPrintCancel() 
{
	if(m_bEnableOpen)
		ShellExecute(NULL, "open", m_pBaseBM->m_sFileName, NULL, NULL, SW_SHOW);
	else
	{
		if(AfxMessageBox(hggettext("수량산출서 출력을 중지 하시겠습니까?"), MB_YESNO | MB_ICONQUESTION)==IDYES)
			m_bCancelPrint	= TRUE;
	}
}

void COutBMDialog::OnButtonOption() 
{
	CDialogBMOption dlg;
	dlg.SetOutBMStd(m_pStd);
	dlg.DoModal();
}

BOOL COutBMDialog::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	SetFocus();

	return CXTResizeDialog::OnSetCursor(pWnd, nHitTest, message);
}


void COutBMDialog::OnSize(UINT nType, int cx, int cy) 
{
	CXTResizeDialog::OnSize(nType, cx, cy);

	SettingGrid();
}

void COutBMDialog::OnButtonPrintSelect()
{
	Begin();	
}


void COutBMDialog::SetProgress(BOOL bInit, long nCountTotal)
{
	const long nMax		= 10000;
	static long nTotal	= 0;
	static long nIdx	= 0;
	if(bInit)
	{
		nTotal	= nCountTotal;
		nIdx	= 0;
		m_ctlProgressPrintBM.SetRange(0, nMax);
		m_ctlProgressPrintBM.SetPos(0);
	}
	else
	{
		m_ctlProgressPrintBM.SetPos(((nIdx+1) * nMax) / nTotal);
		nIdx++;
	}
}

void COutBMDialog::GetTreeInfoArray(std::vector<structPageIndexInfo> &vArrTreeInfo, long nType )
{
	vArrTreeInfo.clear();

	CTypedPtrArray <CObArray, structBMTreeInfo*> *pArrBMTreeInfo;
	if(nType == 0)
	{
		pArrBMTreeInfo = &m_arrBMTreeInfo;
	}
	else if (nType == 1)
	{
		pArrBMTreeInfo = &m_arrBMTreeInfoCommon;
	}
	else if (nType == 2)
	{
		pArrBMTreeInfo = &m_arrBMTreeInfoTogong;
	}
	else if (nType == 3)
	{
		pArrBMTreeInfo = &m_arrBMTreeInfoRebar;
	}

	long nCount = pArrBMTreeInfo->GetSize();
	for(long ix=0; ix<nCount; ++ix)
	{
		structBMTreeInfo *pStTreeInfo = pArrBMTreeInfo->GetAt(ix);
		CString str = pStTreeInfo->sName;
		if(str.Find(_T("산출근거")) != -1)
		{
			structPageIndexInfo stInfo;
			stInfo.szPageName = str;

			long nCntBm = pStTreeInfo->pArrBM->GetSize();
			for(long jx=0; jx<nCntBm; ++jx)
			{
				CBMData *pBm = pStTreeInfo->pArrBM->GetAt(jx);
				if(pBm->m_bIsUserBM) continue;

				std::map<int, CString>::iterator mIter = stInfo.mClassIndex.find(pBm->m_nIdxBigClass);
				if(mIter == stInfo.mClassIndex.end())
				{
					mIter = stInfo.mClassIndex.begin();
					for(mIter; mIter!=stInfo.mClassIndex.end(); mIter++)
					{
						if(mIter->first > pBm->m_nIdxBigClass)
							break;
					}
	
					stInfo.mClassIndex.insert(std::make_pair(pBm->m_nIdxBigClass, pBm->m_sBigClass));
				}
			}

			//stInfo.mClassIndex.insert(std::make_pair(99, _T("하단에 추가")));
			
			vArrTreeInfo.push_back(stInfo);
		}
	}
}

void COutBMDialog::InsertUserBmDataTreeinfo( long nType )
{
	structBMTreeInfo *pBMTreeInfo	= NULL;

	CTypedPtrArray <CObArray, structBMTreeInfo*> *pArrBMTreeInfo	= NULL;
	switch(nType)
	{
	case 0 : pArrBMTreeInfo	= &m_arrBMTreeInfo; break;
	case 1 : pArrBMTreeInfo	= &m_arrBMTreeInfoCommon; break;
	case 2 : pArrBMTreeInfo	= &m_arrBMTreeInfoTogong; break;
	case 3 : pArrBMTreeInfo	= &m_arrBMTreeInfoRebar; break;
	}
	if(!pArrBMTreeInfo) return;

	CARcBridgeDataStd *pDataStd	= m_pStd->m_pARcBridgeDataStd;
	CUserBmData *pUserBm = pDataStd->m_pUserBmData;

	long i = 0; for(i = 0; i < pArrBMTreeInfo->GetSize(); i++)
	{
		structBMTreeInfo *pBMTreeInfo	= pArrBMTreeInfo->GetAt(i);

		CUserBmApplyData *pApplyData = pUserBm->GetUserBmApplyData(pBMTreeInfo->sName, nType);
		if(pApplyData == NULL) continue;

		pUserBm->CopyUserBmToRealData(pApplyData, pBMTreeInfo->pArrBM);
	}
}
