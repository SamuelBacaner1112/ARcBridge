// InputStmFootingMid.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ARcBridgeInput.h"
#include "InputStmFootingMid.h"


// CInputStmFootingMid 대화 상자입니다.

IMPLEMENT_DYNAMIC(CInputStmFootingMid, CInputStmModelBase)

CInputStmFootingMid::CInputStmFootingMid(CWnd* pParent /*=NULL*/)
	: CInputStmModelBase(CInputStmFootingMid::IDD, pParent)
{

}

CInputStmFootingMid::~CInputStmFootingMid()
{
}

void CInputStmFootingMid::DoDataExchange(CDataExchange* pDX)
{
	CInputStmModelBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_STMCOMBO_CHECK, m_ComboCase);
}


BEGIN_MESSAGE_MAP(CInputStmFootingMid, CInputStmModelBase)
	ON_BN_CLICKED(IDC_BTN_CHECKMAIN, &CInputStmFootingMid::OnBnClickedBtnCheckmain)
	ON_CBN_SELCHANGE(IDC_COMBO_STMCOMBO_CHECK, &CInputStmFootingMid::OnCbnSelchangeComboStmcomboCheck)
END_MESSAGE_MAP()


// CInputStmFootingMid 메시지 처리기입니다.

void CInputStmFootingMid::OnPreInitDialog()
{
	CInputStmModelBase::OnPreInitDialog();
}

void CInputStmFootingMid::OnBnClickedBtnCheckmain()
{
	CDialogSTMFootingMainMid dlg;
	dlg.m_pIn = m_pIn;
	dlg.m_nCurIdx = m_nCurModel;
	if(dlg.DoModal() == IDOK)
	{
		if(dlg.m_bChange)
			AfxMessageBox(hggettext("철근배근이 수정되었습니다. STM해석을 다시 진행하시기 바랍니다."), MB_OK);
	}
}

void CInputStmFootingMid::OnBnClickedBtnCheckshear()
{
	CDialogSTMFootingShearMid dlg;
	dlg.m_pIn = m_pIn;
	dlg.m_nCurIdx = m_nCurModel;
	if(dlg.DoModal() == IDOK)
	{
		if(dlg.m_bChange)
			AfxMessageBox(hggettext("철근배근이 수정되었습니다. STM해석을 다시 진행하시기 바랍니다."), MB_OK);
	}
}

void CInputStmFootingMid::InitModelCombo( HGINT32 iDepth/*=0*/ )
{
	CRcBridgeApp *pBri = m_pIn->GetRcBridge();

	m_ComboModel.ResetContent();
	m_ComboCase.ResetContent();
	m_vKeyModel.clear();
	m_vKeyCase.clear();

	HGINT32 iKeyStt = m_bDesignLsd ? STM_MODEL_FOOT_MID_MAX_AXIAL : STM_MODEL_MIDFOOT_HEAD;
	HGINT32 iAddCount = 0;
	CString str, str1, str2;
	HGBOOL bFlag = FALSE;

	HGINT32 icKeyUnit = m_bDesignLsd ? 14 : 3;
	for (HGINT32 ix = 0; ix < MAX_JIJUM_COUNT_RC; ++ix)
	{
		iKeyStt = (m_bDesignLsd ? STM_MODEL_FOOT_MID_MAX_AXIAL : STM_MODEL_MIDFOOT_HEAD) + ix * icKeyUnit;
		bFlag = FALSE;
		for(HGINT32 iKey=iKeyStt; iKey<=(iKeyStt + icKeyUnit); iKey++)
		{
			CStmModel *pModel = pBri->GetStmModelByKey(iKey);
			if(pModel == NULL) continue;

			if (bFlag) continue;

			str1 = pBri->GetStringStmModelNameByKey(iKey, 1);
			str2 = pBri->GetStringStmModelNameByKey(iKey, 2);
			str = str1 + str2;
			m_ComboModel.AddString(str);
			++iAddCount;
			m_vKeyModel.push_back(ix);
			bFlag = TRUE;
		}
	}

	if(iAddCount > 0 && iDepth == 0)
		m_ComboModel.SetCurSel(0);
	else if (iDepth == 1)
		m_ComboModel.SetCurSel(m_nCurModel);
	else if (iAddCount <= 0)
		return;

	

  	iKeyStt = m_bDesignLsd ? (STM_MODEL_FOOT_MID_MAX_AXIAL + 15 * m_vKeyModel[m_nCurModel]) : (STM_MODEL_MIDFOOT_HEAD + 4 * m_vKeyModel[m_nCurModel]);
 	HGINT32 iKeyEnd = iKeyStt + STM_MODEL_FOOT_LSD_COUNT - 1;

	HGINT32 icCase( m_bDesignLsd ? 3 : 1);
	for (HGINT32 ixCase=0; ixCase<icCase; ++ixCase)
	{
		if( 0 < ixCase )
		{
			iKeyStt = iKeyEnd+1;
			iKeyEnd = iKeyStt + STM_MODEL_FOOT_LSD_COUNT - 1;
		}

		if( pBri->IsValidComboEnvType(static_cast<ETypeEnvComboLsd>(ixCase)) == FALSE ) continue;

		for(HGINT32 iKey=iKeyStt; iKey<=iKeyEnd; iKey++)
		{
			CStmModel *pModel = pBri->GetStmModelByKey(iKey);
			if(pModel == NULL) continue;

			m_vKeyCase.push_back(iKey);

			CString strLoad = m_bDesignLsd ? pBri->m_strStmModelingMidFooting[m_nCurModel][ixCase][iKey - iKeyStt] : _T("");
			CString str = pBri->GetStringStmModelNameByKey(iKey,3);
			if( strLoad.IsEmpty() == FALSE )
				str += _T("-") + strLoad;
			
			m_ComboCase.AddString(str);
		}
	}
	m_nCurCase = 0;
	m_ComboCase.SetCurSel(m_nCurCase);

	SetCurModelKey();
}

void CInputStmFootingMid::OnCbnSelchangeComboStmcomboCheck()
{
	m_nCurCase = m_ComboCase.GetCurSel();
	SetSelectModel();

	SetCurModelKey();
	SetLoadCaseCombo();
	DrawInputDomyunView();
}

BOOL CInputStmFootingMid::IsValid()
{
	HGBOOL bValid = FALSE;
	CRcBridgeApp *pBridge = m_pIn->GetRcBridge();
	if (!pBridge) return bValid;

	if(m_pIn->m_pARcBridgeDataStd->IsLockRcBridgeStmModule() == FALSE)
		return FALSE;
	if(m_pIn->m_pARcBridgeDataStd->m_nTypeKciCode < 2 && m_pIn->m_pARcBridgeDataStd->IsLsdDesign() == FALSE)
		return FALSE;

	HGINT32 nCountInwall = pBridge->GetCountInWall();

	for (HGINT32 ix = 0; ix < nCountInwall; ++ix)
	{
		for(HGINT32 jx = 0; jx < 3; ++jx)
		{
			for(HGINT32 iCombo=0; iCombo<5; ++iCombo)
			{
				if(pBridge->m_bApplyStmModelingMidFooting[ix][jx][iCombo])
				{
					bValid = TRUE;
					break;
				}
			}
			if(bValid)
				break;
		}
	}

	return bValid;
}

BOOL CInputStmFootingMid::InitStmModel(CStmModel *pModel)
{
	CRcBridgeRebar *pBridge = m_pIn->GetRcBridgeRebar();
	if (!pBridge) return FALSE;

	if(pModel && !pModel->IsLock() && !pModel->IsStmModified())
	{
		HGINT32 iKeyCur = pBridge->GetStmKeyByStmModel(pModel);
		if(iKeyCur > -1)
		{
			MakeStmTrussData(m_pIn->m_nCurrentBridge, iKeyCur);
		}
	}
	else if(pModel && !pModel->IsLock())
	{
		// TODO : 해석이 되있는것도 아니고 모델을 새로 만들지도 않았다면 하중만 다시 태워줘보자
		if(pBridge->m_FemNormal.IsOutputLoaded())
		{
			HGINT32 iKeyCur = pBridge->GetStmKeyByStmModel(pModel);
			if(iKeyCur > -1)
			{
				MakeStmTrussData(m_pIn->m_nCurrentBridge, iKeyCur, TRUE);
			}
		}
	}

	return CInputStmModelBase::InitStmModel(pModel);
}

void CInputStmFootingMid::MakeStmStressFlowData( CStmModel *pStm, HGINT32 nBri, HGINT32 ixKey )
{
	CRcBridgeRebar *pBridge = m_pIn->m_pARcBridgeDataStd->GetBridgeRebar(nBri);
	if (!pBridge) return;

	if(static_cast<HGINT32>(m_vKeyCase.size()) <= m_nCurCase) return;

	CRcBridgeApp *pBri = m_pIn->GetRcBridge();

	HGINT32 iInex = m_bDesignLsd ? (ixKey - STM_MODEL_FOOT_MID_MAX_AXIAL) : (ixKey - STM_MODEL_MIDFOOT_HEAD);
	HGINT32 nJijum = m_bDesignLsd ? (iInex / 15 + 1) : (iInex / STM_MODEL_FOOT_COUNT + 1);
	eTypeLoadFoot eTypeLoad = (eTypeLoadFoot)(iInex%STM_MODEL_FOOT_COUNT);
	EFootingCheckLsd eTypeLoadLsd = (EFootingCheckLsd)(iInex%STM_MODEL_FOOT_LSD_COUNT);
	ETypeEnvComboLsd eTypeEnv =  pBri->GetTypeEnvComboLsdFooting(ixKey, nJijum );

	CARcBridgeCalcStd CalcStd(pBridge);
	CalcStd.m_pStm->m_pCalcStmMidFoot->SetPosFooting(nJijum);
	if( m_bDesignLsd )
	{
		HGBOOL bEarthQuake = (ixKey < STM_MODEL_FOOT_MID_MIN_HORI_FORCE) ? TRUE : FALSE;
		CalcStd.m_pStm->m_pCalcStmMidFoot->SetTypeLoadCaseLsd(eTypeLoadLsd);
		CalcStd.m_pStm->m_pCalcStmMidFoot->SetEarthQuakeLsd(bEarthQuake);
	}
	else
		CalcStd.m_pStm->m_pCalcStmMidFoot->SetTypeLoadCase(eTypeLoad);
	CalcStd.m_pStm->m_pCalcStmMidFoot->MakeStmModelByTemplete(pStm, TRUE, eTypeEnv);
}

void CInputStmFootingMid::MakeStmTrussData( HGINT32 nBri, HGINT32 ixKey, BOOL bSetLoadOnly )
{
	CRcBridgeRebar *pBridge = m_pIn->m_pARcBridgeDataStd->GetBridgeRebar(nBri);
	if (!pBridge) return;

	CStmModel *pStm = pBridge->GetStmModelByKey(ixKey);
	if(pStm == NULL)
		return;

	CRcBridgeApp *pBri = m_pIn->GetRcBridge();

	HGINT32 iInex = m_bDesignLsd ? (ixKey - STM_MODEL_FOOT_MID_MAX_AXIAL) : (ixKey - STM_MODEL_MIDFOOT_HEAD);
	HGINT32 nJijum = m_bDesignLsd ? (iInex / 15 + 1) : (iInex / STM_MODEL_FOOT_COUNT + 1);
	eTypeLoadFoot eTypeLoad = (eTypeLoadFoot)(iInex%STM_MODEL_FOOT_COUNT);
	EFootingCheckLsd eTypeLoadLsd = (EFootingCheckLsd)(iInex%STM_MODEL_FOOT_LSD_COUNT);
	ETypeEnvComboLsd eTypeEnv = pBri->GetTypeEnvComboLsdFooting(ixKey, nJijum);

	CARcBridgeCalcStd CalcStd(pBridge);
	CalcStd.m_pStm->m_pCalcStmMidFoot->SetPosFooting(nJijum);
	if( m_bDesignLsd )
	{
		HGBOOL bEarthQuake = (eTypeEnv == eEnvExtremeI || eTypeEnv == eEnvExtremeII) ? TRUE : FALSE;

		CalcStd.m_pStm->m_pCalcStmMidFoot->SetTypeLoadCaseLsd(eTypeLoadLsd);
		CalcStd.m_pStm->m_pCalcStmMidFoot->SetEarthQuakeLsd(bEarthQuake);
	}
	else
		CalcStd.m_pStm->m_pCalcStmMidFoot->SetTypeLoadCase(eTypeLoad);

	if(bSetLoadOnly)
	{
		CalcStd.m_pStm->m_pCalcStmMidFoot->ResetLoadAtModel(pStm, eTypeEnv);
	}
	else
	{
		CalcStd.m_pStm->m_pCalcStmMidFoot->MakeStmModelByTemplete(pStm, FALSE, eTypeEnv);
	}

	return;
}
