// InputStmFootingStartEnd.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ARcBridgeInput.h"
#include "InputStmFootingStartEnd.h"


// CInputStmFootingStartEnd 대화 상자입니다.

IMPLEMENT_DYNAMIC(CInputStmFootingStartEnd, CInputStmModelBase)

CInputStmFootingStartEnd::CInputStmFootingStartEnd(CWnd* pParent /*=NULL*/)
	: CInputStmModelBase(CInputStmFootingStartEnd::IDD, pParent)
{

}

CInputStmFootingStartEnd::~CInputStmFootingStartEnd()
{
}

void CInputStmFootingStartEnd::DoDataExchange(CDataExchange* pDX)
{
	CInputStmModelBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_STMCOMBO_CHECK, m_ComboCase);
}


BEGIN_MESSAGE_MAP(CInputStmFootingStartEnd, CInputStmModelBase)
	ON_BN_CLICKED(IDC_BTN_CHECKMAIN, &CInputStmFootingStartEnd::OnBnClickedBtnCheckmain)
	ON_BN_CLICKED(IDC_BTN_CHECKSHEAR, &CInputStmFootingStartEnd::OnBnClickedBtnCheckshear)
	ON_CBN_SELCHANGE(IDC_COMBO_STMCOMBO_CHECK, &CInputStmFootingStartEnd::OnCbnSelchangeComboStmcomboCheck)
END_MESSAGE_MAP()

// CInputStmFootingStartEnd 메시지 처리기입니다.

void CInputStmFootingStartEnd::OnPreInitDialog()
{
	CInputStmModelBase::OnPreInitDialog();
}

void CInputStmFootingStartEnd::OnBnClickedBtnCheckmain()
{
	CDialogSTMFootingMainStartEnd dlg;
	dlg.m_pIn = m_pIn;
	dlg.m_ixStmKey = m_ixStmKey;
	if(dlg.DoModal() == IDOK)
	{
		if(dlg.m_bChange)
			AfxMessageBox(hggettext("철근배근이 수정되었습니다. STM해석을 다시 진행하시기 바랍니다."), MB_OK);
	}
}

void CInputStmFootingStartEnd::OnBnClickedBtnCheckshear()
{
	CDialogSTMFootingShearStartEnd dlg;
	dlg.m_pIn = m_pIn;
	dlg.m_ixStmKey = m_ixStmKey;
	if(dlg.DoModal() == IDOK)
	{
		if(dlg.m_bChange)
			AfxMessageBox(hggettext("철근배근이 수정되었습니다. STM해석을 다시 진행하시기 바랍니다."), MB_OK);
	}
}

void CInputStmFootingStartEnd::InitModelCombo( HGINT32 iDepth/*=0*/ )
{
	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	
	m_ComboModel.ResetContent();
	m_ComboCase.ResetContent();
	m_vKeyModel.clear();
	m_vKeyCase.clear();

	HGINT32 ixKeySkip = m_bDesignLsd ? 15 : 4;
	HGINT32 iKeyStt = m_bDesignLsd ? STM_MODEL_FOOT_STT_MAX_AXIAL : STM_MODEL_FOOT_STT_MAXP;
	HGINT32 iKeyEnd = m_bDesignLsd ? STM_MODEL_FOOT_END_MIN_HORI_FORCE_EXT_2 : STM_MODEL_FOOT_END_EARTH;
	HGINT32 iAddCount = 0;
	CString str, str1, str2;
	for (HGINT32 ix = 0; ix < 2; ++ix)
	{
		for(HGINT32 iKey=iKeyStt; iKey<(iKeyStt + ixKeySkip); iKey++)
		{
			CStmModel *pModel = pBri->GetStmModelByKey(iKey);
			if(pModel == NULL) continue;

			str1 = pBri->GetStringStmModelNameByKey(iKey, 1);
			str2 = pBri->GetStringStmModelNameByKey(iKey, 2);
			str = str1 + str2;
			m_ComboModel.AddString(str);
			++iAddCount;
			break;
		}
		iKeyStt += ixKeySkip;
	}

	if(m_nCurModel >= iAddCount)
		m_nCurModel = 0;

 	if(iAddCount > 0 && iDepth == 0)
		m_ComboModel.SetCurSel(0);
	else if (iDepth == 1)
		m_ComboModel.SetCurSel(m_nCurModel);
	else if (iAddCount <= 0)
		return;

	m_ComboModel.GetLBText(m_nCurModel, str);
	HGBOOL bStt(FALSE);
	if (str.Find(_T("시점")) != -1)
	{
		iKeyStt = m_bDesignLsd ? STM_MODEL_FOOT_STT_MAX_AXIAL : STM_MODEL_FOOT_STT_MAXP;
		iKeyEnd = m_bDesignLsd ? STM_MODEL_FOOT_STT_MIN_HORI_FORCE : STM_MODEL_FOOT_STT_EARTH;
		bStt = TRUE;
	}
	else
	{
		iKeyStt = m_bDesignLsd ? STM_MODEL_FOOT_END_MAX_AXIAL : STM_MODEL_FOOT_END_MAXP;
		iKeyEnd = m_bDesignLsd ? STM_MODEL_FOOT_END_MIN_HORI_FORCE : STM_MODEL_FOOT_END_EARTH;
	}

	HGINT32 icCase(m_bDesignLsd ? 3 : 1);
	for (HGINT32 ixCase = 0; ixCase<icCase; ++ixCase)
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

			CString strLoad = m_bDesignLsd ? pBri->m_strStmModelingFooting[bStt ? 0 : 1][ixCase][iKey - iKeyStt] : _T("");
			CString str = pBri->GetStringStmModelNameByKey(iKey, 3);
			if( strLoad.IsEmpty() == FALSE )
				str += _T("-") + strLoad;

			m_ComboCase.AddString(str);
		}
	}
	
	m_nCurCase = 0;
	m_ComboCase.SetCurSel(m_nCurCase);

	SetCurModelKey();
}

void CInputStmFootingStartEnd::OnCbnSelchangeComboStmcomboCheck()
{
	m_nCurCase = m_ComboCase.GetCurSel();
	SetSelectModel();

	SetCurModelKey();
	SetDataInit();

	DrawInputDomyunView();
}

BOOL CInputStmFootingStartEnd::IsValid()
{
	HGBOOL bValid = FALSE;
	CRcBridgeApp *pBridge = m_pIn->GetRcBridge();
	if (!pBridge) return bValid;

	if(m_pIn->m_pARcBridgeDataStd->IsLockRcBridgeStmModule() == FALSE)
		return FALSE;
	if(m_pIn->m_pARcBridgeDataStd->m_nTypeKciCode < 2 && m_pIn->m_pARcBridgeDataStd->IsLsdDesign() == FALSE)
		return FALSE;

	for (HGINT32 ix = 0; ix < 2; ++ix)
	{
		for (HGINT32 jx = 0; jx < 3; ++jx)
		{
			for(HGINT32 iCombo=0; iCombo<5; ++iCombo)
			{
				if(pBridge->m_bApplyStmModelingFooting[ix][jx][iCombo])
				{
					bValid = TRUE;
					break;
				}
			}
		}
		if(bValid)
			break;
	}

	return bValid;
}

BOOL CInputStmFootingStartEnd::InitStmModel(CStmModel *pModel)
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

void CInputStmFootingStartEnd::MakeStmStressFlowData( CStmModel *pStm, HGINT32 nBri, HGINT32 ixKey )
{
	CRcBridgeRebar *pBridge = m_pIn->m_pARcBridgeDataStd->GetBridgeRebar(nBri);
	if (!pBridge) return;

	CRcBridgeApp *pBri = m_pIn->GetRcBridge();

	if(static_cast<HGINT32>(m_vKeyCase.size()) <= m_nCurCase) return;

	HGBOOL bStt = m_bDesignLsd ? ((ixKey < STM_MODEL_FOOT_END_MAX_AXIAL) ? TRUE : FALSE) : ((ixKey < STM_MODEL_FOOT_END_MAXP) ? TRUE : FALSE);
	eTypeLoadFoot eTypeLoad = (eTypeLoadFoot)(ixKey - (bStt? STM_MODEL_FOOT_STT_MAXP : STM_MODEL_FOOT_END_MAXP));
	EFootingCheckLsd eTypeLoadLsd = (EFootingCheckLsd)((ixKey - (bStt? STM_MODEL_FOOT_STT_MAX_AXIAL : STM_MODEL_FOOT_END_MAX_AXIAL))%eCountFootCheck);
	ETypeEnvComboLsd eTypeEnv =  pBri->GetTypeEnvComboLsdFooting(ixKey, bStt==TRUE ? 0 : pBri->GetCountJijum() );

	CARcBridgeCalcStd CalcStd(pBridge);
	CalcStd.m_pStm->m_pCalcStmFoot->SetPosFooting(bStt);
	if( m_bDesignLsd )
	{
		HGBOOL bEarthQuake = (bStt && STM_MODEL_FOOT_STT_MIN_HORI_FORCE < ixKey) || (!bStt && STM_MODEL_FOOT_END_MIN_HORI_FORCE < ixKey) ? TRUE : FALSE;
		CalcStd.m_pStm->m_pCalcStmFoot->SetTypeLoadCaseLsd(eTypeLoadLsd);
		CalcStd.m_pStm->m_pCalcStmFoot->SetEarthQuakeLsd(bEarthQuake);
	}
	else
		CalcStd.m_pStm->m_pCalcStmFoot->SetTypeLoadCase(eTypeLoad);
	CalcStd.m_pStm->m_pCalcStmFoot->MakeStmModelByTemplete(pStm, TRUE, eTypeEnv);

	CInputStmModelBase::InitStmModel(pStm);

	return;
}

void CInputStmFootingStartEnd::MakeStmTrussData( HGINT32 nBri, HGINT32 ixKey, BOOL bSetLoadOnly )
{
	CRcBridgeRebar *pBridge = m_pIn->m_pARcBridgeDataStd->GetBridgeRebar(nBri);
	if (!pBridge) return;

	CStmModel *pStm = pBridge->GetStmModelByKey(ixKey);
	if(pStm == NULL)
		return;

	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	
	HGBOOL bStt = m_bDesignLsd ? ((ixKey < STM_MODEL_FOOT_END_MAX_AXIAL) ? TRUE : FALSE) : ((ixKey < STM_MODEL_FOOT_END_MAXP) ? TRUE : FALSE);
	eTypeLoadFoot eTypeLoad = (eTypeLoadFoot)(ixKey - (bStt? STM_MODEL_FOOT_STT_MAXP : STM_MODEL_FOOT_END_MAXP));
	EFootingCheckLsd eTypeLoadLsd = (EFootingCheckLsd)((ixKey - (bStt? STM_MODEL_FOOT_STT_MAX_AXIAL : STM_MODEL_FOOT_END_MAX_AXIAL))%eCountFootCheck);
	ETypeEnvComboLsd eTypeEnv = pBri->GetTypeEnvComboLsdFooting(ixKey, bStt==TRUE ? 0 : pBri->GetCountJijum());

	CARcBridgeCalcStd CalcStd(pBridge);
	CalcStd.m_pStm->m_pCalcStmFoot->SetPosFooting(bStt);
	if( m_bDesignLsd )
	{
		HGBOOL bEarthQuake = (bStt && STM_MODEL_FOOT_STT_MIN_HORI_FORCE < ixKey) || (!bStt && STM_MODEL_FOOT_END_MIN_HORI_FORCE < ixKey) ? TRUE : FALSE;

		CalcStd.m_pStm->m_pCalcStmFoot->SetTypeLoadCaseLsd(eTypeLoadLsd);
		CalcStd.m_pStm->m_pCalcStmFoot->SetEarthQuakeLsd(bEarthQuake);
	}
	else
		CalcStd.m_pStm->m_pCalcStmFoot->SetTypeLoadCase(eTypeLoad);

	if(bSetLoadOnly)
	{
		CalcStd.m_pStm->m_pCalcStmFoot->ResetLoadAtModel(pStm, eTypeEnv);
	}
	else
	{
		CalcStd.m_pStm->m_pCalcStmFoot->MakeStmModelByTemplete(pStm, FALSE, eTypeEnv);
	}

	return;
}