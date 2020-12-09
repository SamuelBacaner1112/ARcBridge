// OutBMDialog.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "../../Src/ARcBridgeData/ARcBridgeData.h"
#include "../ARcBridgeInput/DlgProgress.h"
#include "arcbridgeoutbm.h"
#include "../../../../Engine/BaseEng/include/attcmn.h"


// COutBMDialog ��ȭ �����Դϴ�.

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

	// ����� ������ ���� ������ �Ǵ��ϱ� ���� ////
	m_bFocusOther	= FALSE;
	m_pBMTreeInfoCur	= NULL;
	m_pBMTreeInfoOther	= NULL;
	m_pTreeCtrl = NULL;
}

COutBMDialog::~COutBMDialog()
{
	AhTPADelete(&m_arrBMTreeInfo, (structBMTreeInfo*)0);		// �Ϲݼ���
	AhTPADelete(&m_arrBMTreeInfoCommon, (structBMTreeInfo*)0);	// �������
	AhTPADelete(&m_arrBMTreeInfoTogong, (structBMTreeInfo*)0);	// �������
	AhTPADelete(&m_arrBMTreeInfoRebar, (structBMTreeInfo*)0);	// ö�ټ���
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


// COutBMDialog �޽��� ó�����Դϴ�.


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
		// �Ϲݼ���
		AhTPADelete(&m_pArrBMAll[cm], (CBMData*)0);			// ����ǥ(��)
		AhTPADelete(&m_pArrBMStringAll[cm], (CBMSumString*)0);
	}

	// �������
	AhTPADelete(&m_pArrBMTogong, (CBMData*)0);			// ������� ����ǥ(��)
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

	// ��, ���� ����ǥ
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


	// ��, ���� ����ǥ
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
			// ö�ٸ�ΰ�������                      
			// ö�ټ����� Merge�Ҷ� ���� �ٲٱ� ������ �����͸� �������� �ȵȴ�.
			pBridge->MakeRebarList(&pArrRebar, -1, TRUE, TRUE, 0, FALSE);

			if(m_pStd->m_bRefCellBMSum)	str.Format(_T("%s ���� ö�ټ��� ����ǥ"), pBridge->m_strBridgeName);
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
				// ö�ٸ�ΰ�������                      
				// ö�ټ����� Merge�Ҷ� ���� �ٲٱ� ������ �����͸� �������� �ȵȴ�.
				pBridge->MakeRebarList(&pArrRebar, -1, TRUE, TRUE, 0, FALSE);

				strBridgeName.Format("%s", m_pStd->m_pARcBridgeDataStd->m_strLineDirName);				

				sBri = pBridge->m_strBridgeName;
				sDir = m_pStd->m_pARcBridgeDataStd->m_strLineDirName;

				if(m_pStd->m_bRefCellBMSum)	sName.Format(_T(" %s ���� (%s) ö�ټ��� ����ǥ"), sBri, sDir);
				else											sName = _T("");

				pOutBMUp->m_rebarSepBridgeSum.AddRebar(&pArrRebar, strBridgeName, sName);

				AhTPADelete(&pArrRebar2, (CReBar*)0);
				// ö�ٸ�ΰ�������                      
				// ö�ټ����� Merge�Ҷ� ���� �ٲٱ� ������ �����͸� �������� �ȵȴ�.
				pBridgeDown->MakeRebarList(&pArrRebar2, -1, TRUE, TRUE, 0, FALSE);

				strBridgeName.Format("%s", m_pStd->m_pARcBridgeDataStd->m_strLineRevDirName);

				sName = _T(""), sBri = _T(""), sDir = _T("");
				sBri = pBridgeDown->m_strBridgeName; 
				sDir = m_pStd->m_pARcBridgeDataStd->m_strLineRevDirName;
				
				if(m_pStd->m_bRefCellBMSum)	sName.Format(_T(" %s ���� (%s) ö�ټ��� ����ǥ"), sBri, sDir);
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

//bReplaceBM: �������� �� �����̳� �ɼǿ� ���� ������ ��� : TRUE
HGBOOL COutBMDialog::CreateBM(HGBOOL bOnlyReplaceString, HGBOOL bNewCreateBM)
{
	if(m_bPrintingExcel)
	{
		AfxMessageBox(_T("���� ��� �߿��� �������谡 �Ұ����մϴ�."));
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

	// �ݵ�� ȣ���Ͽ� �������� �ɼǿ� ���� ����� �� �ֵ��� �Ѵ�. 
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

		sMsg = hggettext("�Ϲݼ��� ������......");
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
	
		str.Format(hggettext("�Ϲݼ��� �� ���� ��....."));
		m_pDlgProgress->SetProgressPos(str, (long)(dOffsetProgress * nIdxProgress++));
		if(!bOnlyReplaceString)
		{
			pBMNor->SetInsertDomyunStd();
			pBMCom->SetInsertDomyunStd();
		}

		////////////////////////////////
		sMsg = hggettext("������� ������......");
		str.Format(_T("%s %s"), pBridge->m_strBridgeName, sMsg);
		m_pDlgProgress->SetProgressPos(str, (long)dOffsetProgress * nIdxProgress++);
		if(bOnlyReplaceString == FALSE)
			pBMTogong->CalculateBM();
		else
			pBMTogong->ClearBM(TRUE);

		InsertUserBmDataTreeinfo(2);

		pBMTogong->MergeBMStd();
		pBMTogong->MakeBMStringStd();

		str.Format(hggettext("������� �� ���� ��....."));
		m_pDlgProgress->SetProgressPos(str, (long)(dOffsetProgress * nIdxProgress++));
		if(!bOnlyReplaceString)
			pBMTogong->SetInsertDomyunStd();
	}

	str = hggettext("�Ϲݼ���, ������� ����ǥ ������.......");
	m_pDlgProgress->SetProgressPos(str, (long)dOffsetProgress * nIdxProgress++);
	MergeBMStd();
	MakeBMStringStd();

	str = hggettext("ö������ǥ ������.......");
	m_pDlgProgress->SetProgressPos(str, (long)dOffsetProgress * nIdxProgress++);
	if(bOnlyReplaceString == FALSE)
		MakeBMRebarTable();

	m_pDlgProgress->SetProgressPos("���� ���� �Ϸ�", 100);

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

	// 0 - �Ϲݼ���, 1 - �������, 2 - �������, 3 - ö�ټ���
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
	// ����� ���� ����
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
		case 0 : sKindBM = hggettext("�Ϲݼ���"); break;
		case 1 : sKindBM = hggettext("�������"); break;
		case 2 : sKindBM = hggettext("�������"); break;
		case 3 : sKindBM = hggettext("ö�ټ���"); break;
	}

	BOOL bBM = (nKindBM != 3) ? TRUE : FALSE;
	if(bBM)
	{
		// �� ���� ����ǥ //////////////////
		pBMTreeInfo				= new structBMTreeInfo;
		pBMTreeInfo->sName.Format(hggettext("%s �Ѱ�����ǥ"), sKindBM);
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

	// (ARCBRIDGE-1689) ������ �и������̰� �����ΰ�� ö�� �Ѱ�����ǥ ���
	if(bBM == FALSE && m_pArrRebarTableAll.empty() == FALSE)
	{
		CString sName(_T(""));
		// ������ ö�� �Ѱ� ���� ����ǥ //////////////////
		pBMTreeInfo				= new structBMTreeInfo;
		pBMTreeInfo->sName.Format(hggettext("%s �Ѱ�����ǥ"), sKindBM);
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

	// ���� ����ǥ
	long nIdxParentTot  = 0;
	for(HGINT32 ix = 0; ix < nCountBridge; ix++)
	{
		CRcBridgeApp *pBridge = pDataStd->GetBridge(ix);
		if(pBridge == NULL)
			continue;

		// �Ϲݼ������� �����������
		COutBM *pOutBM = (nKindBM == 1) ? m_pStd->m_pArrOutBMCommon.GetAt(ix) : m_pStd->m_pArrOutBMNormal.GetAt(ix);
		if(pOutBM == NULL)
			continue;

		double dStaSt = pBridge->GetStationAtSlabEnd(TRUE,0);

		if (pBridge->m_nSeparBri != 2)
		{
			pBMTreeInfo				= new structBMTreeInfo;
			pBMTreeInfo->sName.Format(hggettext("%s ���� %s ����ǥ"), pBridge->m_strBridgeName, sKindBM);
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

			//{ARCBRIDGE-2769  ������ ������ ����ǥ�� �������� ���ؼ� ����
			COutBM *pOutBMParent = pOutBM;
			if(ix > 0)
				pOutBMParent = (nKindBM == 1) ? m_pStd->m_pArrOutBMCommon.GetAt(ix-1) : m_pStd->m_pArrOutBMNormal.GetAt(ix-1);
			//}

			CString sBriDir = pDataStd->GetStringBridgeDir(ix);

			pBMTreeInfo				= new structBMTreeInfo;
			pBMTreeInfo->sName.Format(hggettext(" %s ���� (%s) %s ����ǥ"), pBridge->m_strBridgeName, sBriDir, sKindBM);
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
			
			if(pBridge->m_nSeparBri == 2) //������ ��� ������ ���Ͽ� ����
				nIdxParentTot--;
		}
	

		long nIdxOfParent	= 0;
		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// ��ü ����ǥ
		pBMTreeInfo				= new structBMTreeInfo;
		pBMTreeInfo->sName.Format(hggettext("%s ��ü %s ����ǥ"), pBridge->m_strBridgeName, sKindBM);
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

		// ��ü ����ٰ�
		// ö�ټ�������ǥ������ ����ٰſ� �ش��ϴ� ö�ټ�������ǥ�� �ʿ� ����
		if(bBM)
		{
			pBMTreeInfo				= new structBMTreeInfo;
			pBMTreeInfo->sName.Format(hggettext("%s ��ü ����ٰ�"), pBridge->m_strBridgeName);
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
		// ��ȣ�� ����ǥ
		BOOL bExistGuardWall = pBridge->IsExistGuardWall(bBM == FALSE);
		BOOL bMoument		= pOutBM->m_pArrBMStringMonument.GetSize() > 0? TRUE : FALSE;
		BOOL bExistGuardTot	= bExistGuardWall || bMoument? TRUE : FALSE;
		
		if(bExistGuardTot)
		{
			pBMTreeInfo				= new structBMTreeInfo;
			pBMTreeInfo->sName.Format(hggettext("%s ��ȣ�� %s ����ǥ"), pBridge->m_strBridgeName, sKindBM);
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

		// ��ȣ�� ����ٰ�
		// ö�ټ�������ǥ������ ����ٰſ� �ش��ϴ� ö�ټ�������ǥ�� �ʿ� ����
		long nIdxGuideWallSum = 0;
		if(bBM)
		{
			for(HGINT32 dan = 0; dan < pBridge->GetQtyHDan(); dan++)
			{
				CGuardWallRC *pGW = pBridge->GetGuardWallByHDan(dan);
				if(pGW == NULL || pGW->IsTypeHDanRoadTotalGuardWall() == FALSE)
					continue;

				pBMTreeInfo				= new structBMTreeInfo;
				pBMTreeInfo->sName.Format(hggettext("%s %s(L%d) ����ٰ�"), pBridge->m_strBridgeName, pBridge->GetGuardWallName(pGW, TRUE), dan + 1);
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
		// �����ִ� ���� ����ǥ�� ���� ��ȣ���� ���ԵǾ� ����ȴ�. 

		// ������ ����ٰ�
		// ö�ټ�������ǥ������ ����ٰſ� �ش��ϴ� ö�ټ�������ǥ�� �ʿ� ����
		if(bBM)
		{
			if(pOutBM->m_pArrBMStringMonument.GetSize() > 0)
			{
				pBMTreeInfo				= new structBMTreeInfo;
				pBMTreeInfo->sName.Format(hggettext("%s ������ ����ٰ�"), pBridge->m_strBridgeName);
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
		// ������ ����ǥ
		//�������� �Ѱ��� ��ġ�� �ȵǾ� ������ �ƹ��� ����ǥ�� ���� �ʿ䰡 ����
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
			//Ʈ���޴��� �����ϱ� ������ ö�� ���̺��� Ʈ����Ʈ �̸��� ���⼭ �ۼ��� �ȴ�.
			pBMTreeInfo				= new structBMTreeInfo;
			pBMTreeInfo->sName.Format(hggettext("%s ������ %s ����ǥ"), pBridge->m_strBridgeName, sKindBM);
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

			// ������ ����ٰ�
			// ö�ټ�������ǥ������ ����ٰſ� �ش��ϴ� ö�ټ�������ǥ�� �ʿ� ����
			if(bBM)
			{
				HGINT32 nIdxOfParentWing = 0;
				for(HGINT32 stt = 0; stt < 2; stt++)
				{
					HGBOOL bStt = (stt == 0) ? TRUE : FALSE;
					CString sStt = bStt ? hggettext("����") : hggettext("����");
					for(HGINT32 left = 0; left < 2; left++)
					{
						HGBOOL bLeft = (left == 0) ? TRUE : FALSE;
						CString sLeft = bLeft ? hggettext("(����)") : hggettext("(����)");
						if(pBridge->IsWingWall(bStt, bLeft) == FALSE)
							continue;

						pBMTreeInfo				= new structBMTreeInfo;
						pBMTreeInfo->sName.Format(hggettext("%s %s ������%s ����ٰ�"), pBridge->m_strBridgeName, sStt, sLeft);
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
		// ���ӽ����� ����ǥ
		//���ӽ����갡 �ƿ� ���� ��� ö�� ����ǥ�� ������ �ʴ´�.
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
			pBMTreeInfo->sName.Format(hggettext("%s ���ӽ����� %s ����ǥ"), pBridge->m_strBridgeName, sKindBM);
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


		// ���ӽ����� ����ٰ�
		// ö�ټ�������ǥ������ ����ٰſ� �ش��ϴ� ö�ټ�������ǥ�� �ʿ� ����
		if(bBM)
		{
			HGINT32 nIdxOfParentAps = 0;
			for(HGINT32 stt = 0; stt < 2; stt++)
			{
				HGBOOL bStt = (stt == 0) ? TRUE : FALSE;
				CString sStt = bStt ? hggettext("����") : hggettext("����");
				for(HGINT32 left = 0; left < 2; left++)
				{
					HGBOOL bLeft = (left == 0) ? TRUE : FALSE;
					CString sLeft = bLeft ? hggettext("(����)") : hggettext("(����)");
					if(bLeft && pBridge->IsAps(bStt, !bLeft) == FALSE)
						sLeft = _T("");

					if(pBridge->IsAps(bStt, bLeft) == FALSE)
						continue;

					pBMTreeInfo				= new structBMTreeInfo;
					pBMTreeInfo->sName.Format(hggettext("%s %s ���ӽ�����%s ����ٰ�"), pBridge->m_strBridgeName, sStt, sLeft);
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
		// ���潽���� ����ǥ
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
			pBMTreeInfo->sName.Format(hggettext("%s ���潽���� %s ����ǥ"), pBridge->m_strBridgeName, sKindBM);
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

		// ���潽���� ����ٰ�
		// ö�ټ�������ǥ������ ����ٰſ� �ش��ϴ� ö�ټ�������ǥ�� �ʿ� ����
		if(bBM)
		{
			HGINT32 nIdxOfParentCns = 0;
			for(HGINT32 stt = 0; stt < 2; stt++)
			{
				HGBOOL bStt = (stt == 0) ? TRUE : FALSE;
				CString sStt = bStt ? hggettext("����") : hggettext("����");
				for(HGINT32 left = 0; left < 2; left++)
				{
					HGBOOL bLeft = (left == 0) ? TRUE : FALSE;
					CString sLeft = bLeft ? hggettext("����") : hggettext("����");

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
							pBMTreeInfo->sName.Format(hggettext("%s %s ���潽���� ����ٰ�"), pBridge->m_strBridgeName, sStt, sLeft, cns + 1);
						else
							pBMTreeInfo->sName.Format(hggettext("%s %s ���潽����(%s%d) ����ٰ�"), pBridge->m_strBridgeName, sStt, sLeft, cns + 1);
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
		// ��ȣ�� ����ǥ
		BOOL bBoho = FALSE;
		for(long stt = 0; stt < 2; stt++)
		{
			BOOL bStt	= stt == iSTT;
			if(pBridge->m_nTypeRcBridgeBlock == 0) continue;

			// ��ȣ�� ������ ���Ѵ�..
			CTwinVectorArray tvArrBoho;
			pBridge->GetTvSungtoLine(tvArrBoho, bStt, TRUE, TRUE);

			long nSize	= tvArrBoho.GetSize();
			if(nSize == 0) continue;

			bBoho = TRUE;
		}


		if(bBoho)
		{
			pBMTreeInfo				= new structBMTreeInfo;
			pBMTreeInfo->sName.Format(hggettext("%s ��ȣ�� %s ����ǥ"), pBridge->m_strBridgeName, sKindBM);
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
		// ��ȣ�� ����ٰ�
		// ö�ټ�������ǥ������ ����ٰſ� �ش��ϴ� ö�ټ�������ǥ�� �ʿ� ����
		if(bBM && bBoho)
		{
			for(HGINT32 stt = 0; stt < 2; stt++)
			{
				pBMTreeInfo				= new structBMTreeInfo;
				pBMTreeInfo->sName.Format(hggettext("%s %s ��ȣ�� ����ٰ�"), pBridge->m_strBridgeName, (stt == 0) ? hggettext("������") : hggettext("������"));
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
		// ����Ÿ�� ����ǥ
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
			pBMTreeInfo->sName.Format(hggettext("%s ����Ÿ�� %s ����ǥ"), pBridge->m_strBridgeName, sKindBM);
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

	CString sKindBM = hggettext("�������");

	structBMTreeInfo *pBMTreeInfo = new structBMTreeInfo;
	pBMTreeInfo->sName.Format("%s %s", sKindBM, _T(" �Ѱ�����ǥ"));
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
			pBMTreeInfo->sName.Format("%s ���� %s ����ǥ", pBridge->m_strBridgeName, sKindBM);
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
			pBMTreeInfo->sName.Format("%s ���� (%s) %s ����ǥ", pBridge->m_strBridgeName, pBridge->m_nSeparBri == 1 ? pDataStd->m_strLineDirName : pDataStd->m_strLineRevDirName, sKindBM);
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
			pBMTreeInfo->sName.Format("%s ��� ����ǥ", sKindBM);
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
					sJijum = _T("������౳ ����");
				else if(jx == 0)
					sJijum = _T("������");
				else if(jx == pBridge->m_nQtyJigan)
					sJijum = _T("������");
				else 
					sJijum.Format("�߰�����%d", jx);


				// ��� ����ٰ�
				pBMTreeInfo				= new structBMTreeInfo;
				pBMTreeInfo->sName.Format(_T("%s ��� ����ٰ�"), sJijum);
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

				// ��ȣ�� ������ ���Ѵ�..
				CTwinVectorArray tvArrBoho;
				pBridge->GetTvSungtoLine(tvArrBoho, bStt, TRUE, TRUE);

				long nSize	= tvArrBoho.GetSize();
				if(nSize == 0) continue;

				bBoho = TRUE;
			}

			if(bBoho)
			{
				pBMTreeInfo				= new structBMTreeInfo;
				pBMTreeInfo->sName.Format("%s ��ȣ�� ����ǥ", sKindBM);
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
					CString sJijum	= (stt == 0) ? _T("������") : _T("������");

					// ��ȣ�� ����ٰ�
					pBMTreeInfo				= new structBMTreeInfo;
					pBMTreeInfo->sName.Format(_T("%s ��ȣ�� ����ٰ�"), sJijum);
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

	// Ʈ�������� Ʈ���� ����
	pTreeCtrl->DeleteAllItems();

	HTREEITEM hItemParent	= NULL;
	AddItem(pTreeCtrl, NULL, TRUE);	// �ʱ�ȭ��
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

	// ���� �̸�
	CString sFileName = m_pStd->m_pDoc->GetPathName();
	sFileName = sFileName.Left(sFileName.GetLength() - 4);
	sFileName += _T("_");

	CString sKindBM = _T("");
	switch(m_nKindBM)
	{
	case 0: sKindBM = _T("�Ϲݼ��� ���⼭"); break;
	case 1: sKindBM = _T("������� ���⼭"); break;
	case 2: sKindBM = _T("������� ���⼭"); break;
	case 3: sKindBM = _T("ö������ǥ"); break;
	}
	sFileName += sKindBM;

	CString sFileForm = pDataStd->GetStringExcelFileForm();

	CFileDialog dlg(FALSE, "*.xls", sFileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, sFileForm);
	dlg.m_ofn.lpstrTitle = "���� ��������";

	if(dlg.DoModal() != IDOK) 
	{
		SetControl(FALSE);
		SetFocus();
		m_bPrintingExcel	= FALSE;
		return;
	}

	CXLControl::GetWmfViewer().ClearAll();

	m_btnCancel.SetWindowText("��� ���");
	m_bEnableOpen	= FALSE;
	m_pBaseBM->m_bRefCellBMSum = m_pStd->m_bRefCellBMSum;
	//////////////////////////////////////////

	// INSERT_ATESTCODE	
	CAttSendMsg attObj(NULL,WM_ATT_MAINMSG,eMsgAppStartBMAtEng,0,eMsgAppFinishBMAtEng,0);




	// ��Ʈ ����
	sFileName = dlg.GetPathName();
	CStringArray	sArrSheet;
	CString strBM = _T("");
	CString sName = _T("");
	long i = 0; for(i = 0; i < pArrBMTreeInfo->GetSize(); i++)
	{
		structBMTreeInfo *pBMTreeInfo = pArrBMTreeInfo->GetAt(i);
		if(pBMTreeInfo->bSelect)
		{
			// () ö������ǥ���� ǥ���� ����.
			if(pBMTreeInfo->nType != 2)
			{
				if(pBMTreeInfo->nLevel == 0)		// �Ѱ�����ǥ
				{
					sName = _T("ǥ��");
					sArrSheet.Add(sName);
				}
				else if(pBMTreeInfo->nLevel == 1)	// ���� ����ǥ
				{
					sName.Format(_T("%s ǥ��"), pBMTreeInfo->sName);
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

		sPrintBM.Format("%s �����...", sSheetName);
		m_staticPrintBM.SetWindowText(sPrintBM);

		HGBOOL bStt = (pBMTreeInfo->sName.Find("����") > -1) ? TRUE : FALSE;
		HGBOOL bLeft = (pBMTreeInfo->sName.Find("(��)") > -1) || (pBMTreeInfo->sName.Find("(����)") > -1)? FALSE : TRUE;
		

		if(pBMTreeInfo->nType == 0)		// ����ٰ�
		{
			m_pBaseBM->PrintBMListToExcel(sSheetName, pBMTreeInfo->pBMString, pBMTreeInfo->pBMSumStringParent, pBMTreeInfo->nIdxOfParent, -1, sTitleLeft);
			
			if(pBMTreeInfo->nTypeBM == 2)	// ���
			{
				if(pBMTreeInfo->pOutBM && sSheetName.Find(_T("���")) > -1)
				{
					COutBMTogong *pTogong = dynamic_cast<COutBMTogong*>(pBMTreeInfo->pOutBM);
					pTogong->DrawFirstPageTogong(m_pBaseBM, pBMTreeInfo->nIdxOfParent);
				}
				else if(pBMTreeInfo->pOutBM && sSheetName.Find(_T("��ȣ��")) > -1)
				{
					COutBMTogong *pTogong = dynamic_cast<COutBMTogong*>(pBMTreeInfo->pOutBM);
					pTogong->DrawFirstPageBoho(m_pBaseBM, bStt);
				}
			}
			else
			{
				if(pBMTreeInfo->pOutBM && sSheetName.Find(_T("��ü")) > -1)
				{
					COutBM *pBM = dynamic_cast<COutBM*>(pBMTreeInfo->pOutBM);
					pBM->DrawFirstPageMain(m_pBaseBM, pBMTreeInfo->nTypeBM);
				}
				else if(pBMTreeInfo->pOutBM && sSheetName.Find(_T("(L")) > -1)	// ��ȣ��
				{
					long nIndexNum = sSheetName.Find(_T("(L"));
					CString szNum = sSheetName.Mid(nIndexNum+2, 2);
					szNum.Replace(_T(")"), _T(""));
					long nHdan = atoi(szNum) - 1;
					CGuardWallRC *pGW = pBridge->GetGuardWallByHDan(nHdan);
					if(pGW && pGW->IsTypeHDanRoadTotalGuardWall())
					{
						COutBM *pBM = dynamic_cast<COutBM*>(pBMTreeInfo->pOutBM);
						pBM->DrawFirstPageGuardWall(m_pBaseBM, nHdan, 0);	// ������ nIdx�� �ǹ̾���
					}
				}
				else if(pBMTreeInfo->pOutBM && sSheetName.Find(_T("������")) > -1)
				{
					COutBM *pBM = dynamic_cast<COutBM*>(pBMTreeInfo->pOutBM);
					pBM->DrawFirstPageWing(m_pBaseBM, bStt, bLeft);
				}
				else if(pBMTreeInfo->pOutBM && sSheetName.Find(_T("���ӽ�����")) > -1)
				{
					COutBM *pBM = dynamic_cast<COutBM*>(pBMTreeInfo->pOutBM);
					pBM->DrawFirstPageAps(m_pBaseBM, bStt, bLeft);
				}
				else if(pBMTreeInfo->pOutBM && sSheetName.Find(_T("������")) > -1)
				{
					COutBM *pBM = dynamic_cast<COutBM*>(pBMTreeInfo->pOutBM);
					pBM->DrawFirstPageMonument(m_pBaseBM);
				}
			}
		}
		else if(pBMTreeInfo->nType == 1)	// ����ǥ
		{
			m_pBaseBM->PrintBMSumListToExcel(sSheetName, pBMTreeInfo->pBMSumString, 2, sArrSheet, TRUE, pBMTreeInfo->pBMSumStringParent, pBMTreeInfo->nIdxOfParent);
			PrintSumTitleToExcel(m_pBaseBM->m_pXL, pBMTreeInfo);

			if(pBMTreeInfo->nLevel == 0)		// �Ѱ�����ǥ
			{
				sSheetName = sArrSheet.GetAt(nIdxSheet--);
				CString sTitle = _T("�Ѱ� ") + sKindBM + _T(" ����ǥ");
				m_pBaseBM->MakeExcelMainPage(sSheetName, sTitle);
			}
			else if(pBMTreeInfo->nLevel == 1)	// ��������ǥ
			{
				sSheetName = sArrSheet.GetAt(nIdxSheet--);
				CRcBridgeRebar *pBridge = pDataStd->GetBridgeRebar(pBMTreeInfo->nIdxBridge);
				MakeExcelCoverPage(sSheetName, pBridge);
			}
		}
		else if(pBMTreeInfo->nType == 2)	// ö������ǥ
		{
			pBMTreeInfo->pBMRebarTable->SetTitle(pBMTreeInfo->sName);
			pBMTreeInfo->pBMRebarTable->PrintToExcel(m_pBaseBM->m_pXL, sSheetName, TRUE);
		}

		SetProgress(FALSE);
	}


	// ���� �ݱ�
	m_pBaseBM->CloseExcel();
	SetProgress(TRUE);
	m_staticPrintBM.SetWindowText(_T("��¿Ϸ�!!"));

	// ���� ��� �Ŀ��� ������� ����� �̸� ����
	m_btnCancel.SetWindowText(_T("������� ����"));
	m_bEnableOpen = TRUE;
	m_bPrintingExcel = FALSE;
	SetControl(FALSE);
}


void COutBMDialog::MakeExcelCoverPage(CString sSheetName, CRcBridgeRebar *pBri)
{
	CXLControl *pXL = m_pBaseBM->m_pXL;

	if(!pXL) return;
	// ����� ��Ʈ�� ã�Ƽ� Ȱ��ȭ �Ѵ�.
	if(!pXL->SetActiveSheet(sSheetName)) return;

	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	CXLFormatText::SetExcelFileName("HgExcelBase.dll");
	CXLFormatText XLText(pXL);

	// format ����
	pXL->SetCellWidth(XL_COLSTT,XL_COLEND,XL_COLWIDTH);
	pXL->SetCellHeight(XL_ROWSTT_BM,XL_ROWEND_BM,XFL_ROWHEIGHT);
	pXL->SetFonts(XL_ROWSTT_BM,XL_COLSTT,XL_ROWEND_BM,XL_COLEND,XL_TEXTSIZE,XL_SHEET_FONT,1,FALSE);
	pXL->SetVerAlign(XL_ROWSTT_BM,XL_COLSTT,XL_ROWEND_BM,XL_COLEND,2); // 2 = CENTER
	pXL->SetHoriAlign(XL_ROWSTT_BM,XL_COLSTT,XL_ROWEND_BM,XL_COLEND,TA_LEFT);

	// ���
	long nRowSheet = 4;
	CString str, strTemp;
	str.Format("%s �������⼭",pBri->m_strBridgeName);

	pXL->SetSheetName(sSheetName);
	pXL->SetMergeCell(nRowSheet,XFL_COLSTT,nRowSheet+3,XFL_COLEND);
	// �簢��
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
	XLText.AddFormatText("$h$t$tb13�� �� �� �� ġ$m+06$c$tb13:$r$tb13 %s$n", str);

	if(pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
		str.Format("%s", pBri->GetStringBridgeType());
	else
		str.Format("%d �氣 %s", pBri->m_nQtyJigan, pBri->GetStringBridgeType());
	XLText.AddFormatText("$h$t$tb13�� �� �� �� ��$m+06$c$tb13:$r$tb13 %s$n", str);

	if(pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
		str.Format("L = %.3f m", toM(pBri->m_dLengthBridge));
	else
		str.Format("L = %.3f m(%s)", toM(pBri->m_dLengthBridge), pBri->GetStringBridgeSpan());
	XLText.AddFormatText("$h$t$tb13�� �� �� �� ��$m+06$c$tb13:$r$tb13 %s$n", str);

	long nLine = 0;
	long nSpan = 0;
	CDPoint vAng = CDPoint(0,1);
	double dStationBridge = pBri->GetStationOnJijum(nSpan,nLine);
	str.Format("%.3f m", toM(pBri->GetWidthSlabAct(dStationBridge,vAng)));
	XLText.AddFormatText("$h$t$tb13�� Ⱦ �� �� ��$m+06$c$tb13:$r$tb13 %s$n", str);

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
		str.Format("%.3f��", vAng.GetAngleDegree());
	}
	else
	{
		str = _T("");
		long i=0; for(i=0; i<pBri->GetCountJijum(); i++)
		{
			vAng	= pBri->GetAngleJijum(i);
			if(i==0) strTemp.Format("����:%.3f��", vAng.GetAngleDegree());
			else if(i==pBri->m_nQtyJigan) strTemp.Format(", ����:%.3f��", vAng.GetAngleDegree());
			else strTemp.Format(", ����%d:%.3f��", i+1, vAng.GetAngleDegree());

			str += strTemp;
		}
	}
	XLText.AddFormatText("$h$t$tb13�� SKEW  �� ��$m+06$c$tb13:$r$tb13 %s$n", str);

	str = pData->GetStringFootTypeAll(pBri);
	XLText.AddFormatText("$h$t$tb13�� �� �� �� ��$m+06$c$tb13:$r$tb13 %s$n", str);

	if(pData->m_nTypeConditionApply < DESIGN_CONDITION_RAIL)
		str.Format("%d ���", pData->m_nDegreeBridge+1);
	else
		str.Format("%d �޼�", pData->m_nDegreeRail+1);
	XLText.AddFormatText("$h$t$tb13�� �� �� �� ��$m+06$c$tb13:$r$tb13 %s$n", str);

	str.Format("%s ���", (pBri->GetDegreeEarthQuake()==0)? "I":"II");
	XLText.AddFormatText("$h$t$tb13�� �� �� �� ��$m+06$c$tb13:$r$tb13 %s$n", str);
}


void COutBMDialog::PrintSumTitleToExcel(CXLControl *pXL, structBMTreeInfo *pBMTreeInfo)
{
	if(!pXL || !pBMTreeInfo)	return;

	long nRow = 0;

	// Ÿ��Ʋ ����
	CString	strTable = pBMTreeInfo->sName == _T("") ? _T("����ǥ") : pBMTreeInfo->sName;
	pXL->SetXL(nRow++, 0, strTable);

	// ���� Ÿ��Ʋ ����
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


// üũ�� ������
// nType = 0 : ��ü ����
// nType = 1 : ��ü ���� ����
// nType = 2 : ����
// nType = 3 : ����ٰŸ� ���� 
// nType = 4 : ����ǥ�� ����
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
		case 0 : pBMTreeInfo->bSelect	= TRUE; break;	// ��ü ����
		case 1 : pBMTreeInfo->bSelect	= FALSE; break;	// ��ü ���� ����
		case 2 : pBMTreeInfo->bSelect	= !pBMTreeInfo->bSelect; break;	// ����
		case 3 : pBMTreeInfo->bSelect	= pBMTreeInfo->nType == 0; break;	// ����ٰŸ� ����
		case 4 : pBMTreeInfo->bSelect	= pBMTreeInfo->nType == 1; break;	// ����ǥ�� ����
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


	if(pBMTreeInfo->nType == 1)	// ����ǥ
	{
		if(pBMTreeInfo->pBMSumString)
		{
			m_pBaseBM->PrintBMSumListToGrid(pGrid, pBMTreeInfo->pBMSumString, 1);
			if(pBMTreeInfo->pBMSumString->GetSize() > 0)
			{
				// ����ǥ Ÿ��Ʋ ����
				CStringArray sArrTitleOfSum;
				GetStringSumTitle(sArrTitleOfSum, pBMTreeInfo);

				long i = 0; for(i = 0; i < sArrTitleOfSum.GetSize(); i++)
					pGrid->SetTextMatrix(0, i, sArrTitleOfSum.GetAt(i));
				pGrid->SetMergeCol(0, 0, 2);
			}
		}
	}
	else if(pBMTreeInfo->nType == 0)	// ����ٰ�
	{
		m_pBaseBM->PrintBMListToGrid(pGrid, pBMTreeInfo->pBMString);
	}
	else if(pBMTreeInfo->nType == 2)	// ö������ǥ
	{
		if(pBMTreeInfo->pBMRebarTable)
		{
			pBMTreeInfo->pBMRebarTable->SetTitle(pBMTreeInfo->sName);
			pBMTreeInfo->pBMRebarTable->SetDataInit(pGrid);
		}		
	}
	else if(pBMTreeInfo->nType == -1)		// �������� ����� �� �˺� ������ ���� �� ������
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

	// �Ѱ�����ǥ���� nIdxBridge�� ���õǾ����� �ʴ´�. 
	if(pBMTreeInfo->nIdxBridge > -1)
		pBridge = pDataStd->GetBridge(pBMTreeInfo->nIdxBridge);

	HGINT32 nKindBM = 0;
	CString sBM	= _T("�Ϲݼ���");
	if(sNameSheetOrTab.Find("�Ϲݼ���", 0) != -1)
	{
		nKindBM = 0;
		sBM	= " �Ϲݼ��� ";
	}
	else if(sNameSheetOrTab.Find("�������", 0) != -1)
	{
		nKindBM = 1;
		sBM	= " ������� ";
	}
	else if(sNameSheetOrTab.Find("�������", 0) != -1)
	{
		nKindBM = 2;
		sBM	= " ������� ";
	}

	sArrTitle.RemoveAll();

	sArrTitle.Add("����");
	sArrTitle.Add("����");
	sArrTitle.Add("����");
	sArrTitle.Add("����");

	CString str = _T("");
	if(sNameSheetOrTab.Find(hggettext("�Ѱ�����ǥ")) != -1)
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
	else if(sNameSheetOrTab.Find(hggettext("����") + sBM + hggettext("����ǥ")) != -1 || 
		sNameSheetOrTab.Find(hggettext("����") + " (" + pDataStd->m_strLineDirName + ")" + sBM + hggettext("����ǥ")) != -1 ||
		sNameSheetOrTab.Find(hggettext("����") + " (" + pDataStd->m_strLineRevDirName + ")" + sBM + hggettext("����ǥ")) != -1)
	{
		// ����� �ƴҶ�
		if (pBridge->m_nSeparBri == 1 && (sNameSheetOrTab.Find(pDataStd->m_strLineDirName) < 0 && sNameSheetOrTab.Find(pDataStd->m_strLineRevDirName)))
		{
			sArrTitle.Add(pDataStd->m_strLineDirName);
			sArrTitle.Add(pDataStd->m_strLineRevDirName);
		}
		else if(sBM.Find(hggettext("�������"), 0) == -1)
		{
			COutBM *pOutBM = (nKindBM == 1) ? m_pStd->m_pArrOutBMCommon.GetAt(pBMTreeInfo->nIdxBridge) : m_pStd->m_pArrOutBMNormal.GetAt(pBMTreeInfo->nIdxBridge);

			if(pOutBM->m_pArrBMStringBridgeSum.GetSize() > 0)
				sArrTitle.Add(hggettext("��ü"));
			if(pOutBM->m_pArrBMStringGuardWallSum.GetSize() > 0)
				sArrTitle.Add(hggettext("��ȣ��"));
			if(pOutBM->m_pArrBMStringWingSum.GetSize() > 0)
				sArrTitle.Add(hggettext("������"));
			if(pOutBM->m_pArrBMStringApsSum.GetSize() > 0)
				sArrTitle.Add(hggettext("���ӽ�����"));
			if(pOutBM->m_pArrBMStringCnsSum.GetSize() > 0)
				sArrTitle.Add(hggettext("���潽����"));
			if(pOutBM->m_pArrBMStringBohoSum.GetSize() > 0)
				sArrTitle.Add(hggettext("��ȣ��"));
		}
		else
		{
			COutBMTogong *pOutBM = m_pStd->m_pArrOutBMTogong.GetAt(pBMTreeInfo->nIdxBridge);

			if(pOutBM->m_pArrBMStringTogongSum.GetSize() > 0)
				sArrTitle.Add(hggettext("���"));
			if(pOutBM->m_pArrBMStringBohoSum.GetSize() > 0)
				sArrTitle.Add(hggettext("��ȣ��"));
		}
	}
	else if(sNameSheetOrTab.Find(hggettext("��ü") + sBM + hggettext("����ǥ")) != -1)
	{
		sArrTitle.Add(hggettext("��ü"));
	}
	else if(sNameSheetOrTab.Find(hggettext("��ȣ��") + sBM + hggettext("����ǥ")) != -1)
	{
		// ������ �߰����� �ʰ� ���ǿ� �´� ��쿡�� add�� �ϰ� �ִ�. 
		// ������ �߰��Ϸ��� COutBM::MergeBMStd(), COutBM::MakeBMStringStd() ���� ������ ���� ���� �ɷ����� �ʰ� ������ Merge�ؾ� �Ѵ�. 
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
			sArrTitle.Add(_T("������"));
	}
	else if(sNameSheetOrTab.Find(hggettext("������") + sBM + hggettext("����ǥ")) != -1)
	{
		// ������ �߰����� �ʰ� ���ǿ� �´� ��쿡�� add�� �ϰ� �ִ�. 
		// ������ �߰��Ϸ��� COutBM::MergeBMStd(), COutBM::MakeBMStringStd() ���� ������ ���� ���� �ɷ����� �ʰ� ������ Merge�ؾ� �Ѵ�. 
		for(HGINT32 stt = 0; stt < 2; stt++)
		{
			HGBOOL bStt = (stt == 0) ? TRUE : FALSE;
			CString sStt = bStt ? hggettext("����") : hggettext("����");
			for(HGINT32 left = 0; left < 2; left++)
			{
				HGBOOL bLeft = (left == 0) ? TRUE : FALSE;
				CString sLeft = bLeft ? hggettext("����") : hggettext("����");

				if(pBridge->IsWingWall(bStt, bLeft) == FALSE)
					continue;

				str.Format(_T("%s %s"), sStt, sLeft);
				sArrTitle.Add(str);
			}
		}
	}
	else if(sNameSheetOrTab.Find(hggettext("���ӽ�����") + sBM + hggettext("����ǥ")) != -1)
	{
		// ������ �߰����� �ʰ� ���ǿ� �´� ��쿡�� add�� �ϰ� �ִ�. 
		// ������ �߰��Ϸ��� COutBM::MergeBMStd(), COutBM::MakeBMStringStd() ���� ������ ���� ���� �ɷ����� �ʰ� ������ Merge�ؾ� �Ѵ�. 
		for(HGINT32 stt = 0; stt < 2; stt++)
		{
			HGBOOL bStt = (stt == 0) ? TRUE : FALSE;
			CString sStt = bStt ? hggettext("����") : hggettext("����");
			for(HGINT32 left = 0; left < 2; left++)
			{
				HGBOOL bLeft = (left == 0) ? TRUE : FALSE;
				CString sLeft = bLeft ? hggettext("����") : hggettext("����");

				if(pBridge->IsAps(bStt, bLeft) == FALSE)
					continue;

				// �����ε� ������ ���� ��쿡�� �����̶� ���ڿ��� ������� �ʴ´�. 
				if(bLeft && pBridge->IsAps(bStt, !bLeft) == FALSE)
					str = sStt;
				else
					str.Format(_T("%s %s"), sStt, sLeft);

				sArrTitle.Add(str);
			}
		}
	}
	else if(sNameSheetOrTab.Find(hggettext("���潽����") + sBM + hggettext("����ǥ")) != -1)
	{
		// ������ �߰����� �ʰ� ���ǿ� �´� ��쿡�� add�� �ϰ� �ִ�. 
		// ������ �߰��Ϸ��� COutBM::MergeBMStd(), COutBM::MakeBMStringStd() ���� ������ ���� ���� �ɷ����� �ʰ� ������ Merge�ؾ� �Ѵ�. 
			for(HGINT32 stt = 0; stt < 2; stt++)
			{
				HGBOOL bStt = (stt == 0) ? TRUE : FALSE;
				CString sStt = bStt ? hggettext("����") : hggettext("����");
				for(HGINT32 left = 0; left < 2; left++)
				{
					HGBOOL bLeft = (left == 0) ? TRUE : FALSE;
					CString sLeft = bLeft ? hggettext("����") : hggettext("����");

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
	else if(sNameSheetOrTab.Find(hggettext("��ȣ��") + sBM + hggettext("����ǥ")) != -1)
	{
		sArrTitle.Add(hggettext("��ȣ��"));
	}
	else if(sNameSheetOrTab.Find(_T("��� ����ǥ")) > -1)		// ���
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
				str = _T("������౳ ����");
			else if(jx == 0)
				str = _T("������");
			else if(jx == pBridge->m_nQtyJigan)
				str = _T("������");
			else 
				str.Format("�߰�����%d", jx);

			sArrTitle.Add(str);
		}
	}
	else if(sNameSheetOrTab.Find(_T("��ȣ�� ����ǥ")) > -1)	// ���
	{
		sArrTitle.Add(_T("������"));
		sArrTitle.Add(_T("������"));
	}

	sArrTitle.Add(_T("��"));
	sArrTitle.Add(_T("���"));
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
		if(AfxMessageBox(hggettext("�������⼭ ����� ���� �Ͻðڽ��ϱ�?"), MB_YESNO | MB_ICONQUESTION)==IDYES)
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
		if(str.Find(_T("����ٰ�")) != -1)
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

			//stInfo.mClassIndex.insert(std::make_pair(99, _T("�ϴܿ� �߰�")));
			
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
