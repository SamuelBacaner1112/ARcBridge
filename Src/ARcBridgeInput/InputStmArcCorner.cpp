// InputStmArcCorner.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ARcBridgeInput.h"
#include "InputStmArcCorner.h"


// CInputStmArcCorner 대화 상자입니다.

IMPLEMENT_DYNAMIC(CInputStmArcCorner, CInputStmModelBase)

CInputStmArcCorner::CInputStmArcCorner(CWnd* pParent /*=NULL*/)
	: CInputStmModelBase(CInputStmArcCorner::IDD, pParent)
{

}

CInputStmArcCorner::~CInputStmArcCorner()
{
}

void CInputStmArcCorner::DoDataExchange(CDataExchange* pDX)
{
	CInputStmModelBase::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CInputStmArcCorner, CInputStmModelBase)
	ON_BN_CLICKED(IDC_BTN_CHECKMAIN, &CInputStmArcCorner::OnBnClickedBtnCheckmain)
	ON_BN_CLICKED(IDC_BTN_CHECKSHEAR, &CInputStmArcCorner::OnBnClickedBtnCheckshear)
END_MESSAGE_MAP()

// CInputStmArcCorner 메시지 처리기입니다.


void CInputStmArcCorner::OnPreInitDialog()
{
	CInputStmModelBase::OnPreInitDialog();
}

void CInputStmArcCorner::OnBnClickedBtnCheckmain()
{
	CDialogSTMArcCornerMain dlg;
	dlg.m_pIn = m_pIn;
	dlg.m_ixStmKey = m_ixStmKey;
	dlg.m_bUpper = IsStmUpperArcCorner(m_ixStmKey);
	if(dlg.DoModal() == IDOK)
	{
		if(dlg.m_bChange)
			AfxMessageBox(hggettext("철근배근이 수정되었습니다. STM해석을 다시 진행하시기 바랍니다."), MB_OK);
	}
}

void CInputStmArcCorner::OnBnClickedBtnCheckshear()
{
	CDialogSTMArcCornerShear dlg;
	dlg.m_pIn = m_pIn;
	dlg.m_ixStmKey = m_ixStmKey;
	dlg.m_bUpper = IsStmUpperArcCorner(m_ixStmKey);
	if(dlg.DoModal() == IDOK)
	{
		if(dlg.m_bChange)
			AfxMessageBox(hggettext("철근배근이 수정되었습니다. STM해석을 다시 진행하시기 바랍니다."), MB_OK);
	}
}

void CInputStmArcCorner::InitModelCombo( HGINT32 iDepth/*=0*/ )
{
	if(iDepth != 0) return;

	CRcBridgeApp *pBri = m_pIn->GetRcBridge();
	HGBOOL bLsd = pBri->m_pARcBridgeDataStd->IsLsdDesign();
	m_ComboModel.ResetContent();
	//	m_ComboCase.ResetContent();
	m_vKeyModel.clear();
	m_vKeyCase.clear();

	HGINT32 iKeyStt = STM_MODEL_CORNER_STT_UPPER_MINUS;
	HGINT32 iKeyEnd = STM_MODEL_CORNER_END_LOWER_PLUS;
	HGINT32 iAddCount = 0;

	HGINT32 bUpper = TRUE;

	HGINT32 icCase(m_bDesignLsd ? 3 : 1);
	for (HGINT32 ixCase = 0; ixCase < icCase; ++ixCase)
	{
		if( 0 < ixCase )
		{
			iKeyStt = ixCase==1 ? STM_MODEL_CORNER_STT_UPPER_MINUS_EXT_1 : STM_MODEL_CORNER_STT_UPPER_MINUS_EXT_2;
			iKeyEnd = ixCase==1 ? STM_MODEL_CORNER_END_LOWER_PLUS_EXT_1 : STM_MODEL_CORNER_END_LOWER_PLUS_EXT_2;
		}

		if( pBri->IsValidComboEnvType(static_cast<ETypeEnvComboLsd>(ixCase)) == FALSE ) continue;

		for(HGINT32 iKey=iKeyStt; iKey<=iKeyEnd; iKey++)
		{
			CStmModel *pModel = pBri->GetStmModelByKey(iKey);
			if(pModel == NULL) continue;

			CString strLoad =  bLsd ? pBri->m_strStmModelingCorner[iKey%2==0 ? 0 : 1][ixCase][IsStmUpperArcCorner(iKey) ? 0 : 1][IsStmOpenArcCorner(iKey) ? 1 : 0] : _T("");
			CString str = pBri->GetStringStmModelNameByKey(iKey);
			if( strLoad.IsEmpty() == FALSE )
				str += _T("-") + strLoad;

			m_vKeyModel.push_back(iKey);
			m_ComboModel.AddString(str);
			++iAddCount;
		}
	}

	m_nCurModel = 0;
	if(iAddCount > 0)
		m_ComboModel.SetCurSel(0);

	SetCurModelKey();
}

BOOL CInputStmArcCorner::IsValid()
{
	HGBOOL bValid = FALSE;
	CRcBridgeApp *pBridge = m_pIn->GetRcBridge();
	if (!pBridge) return bValid;

	HGBOOL bLsd = m_pIn->m_pARcBridgeDataStd->IsLsdDesign();
	if(m_pIn->m_pARcBridgeDataStd->IsLockRcBridgeStmModule() == FALSE)
		return FALSE;
	if(m_pIn->m_pARcBridgeDataStd->m_nTypeKciCode < 2 && bLsd == FALSE)
		return FALSE;

	for(HGINT32 ixStt=iSTT; ixStt<2; ++ixStt)
	{
		for(HGINT32 ixCase=0; ixCase<3; ++ixCase)
		{
			for(HGINT32 ixUD=iUPPER; ixUD<2; ++ixUD)
			{
				for(HGINT32 ixPM=0; ixPM<2; ++ixPM)
				{
					if(pBridge->m_bApplyStmModelingCorner[ixStt][bLsd ? ixCase : 0][ixUD][ixPM])
					{
						bValid = TRUE;
						break;
					}
				}

				if(bValid)
					break;
			}
		}

		if(bValid)
			break;
	}

	return bValid;
}

BOOL CInputStmArcCorner::InitStmModel(CStmModel *pModel)
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

void CInputStmArcCorner::MakeStmStressFlowData( CStmModel *pStm, HGINT32 nBri, HGINT32 ixKey )
{
	CRcBridgeRebar *pBridge = m_pIn->m_pARcBridgeDataStd->GetBridgeRebar(nBri);
	if (!pBridge) return;

	if(pStm->IsLock())
		return;

	CARcBridgeCalcStd CalcStd(pBridge);
	CalcStd.m_pStm->m_pCalcStmCorner->SetPosCorner(ixKey);
	CalcStd.m_pStm->m_pCalcStmCorner->MakeStmModelByTemplete(static_cast<CStmModelForArcbridge*>(pStm), TRUE);

	return;
}

void CInputStmArcCorner::MakeStmTrussData( HGINT32 nBri, HGINT32 ixKey, BOOL bSetLoadOnly )
{
	CRcBridgeRebar *pBridge = m_pIn->m_pARcBridgeDataStd->GetBridgeRebar(nBri);
	if (!pBridge) return;

	CStmModel *pStm = pBridge->GetStmModelByKey(ixKey);
	if(pStm == NULL)
		return;

	CARcBridgeCalcStd CalcStd(pBridge);
	CalcStd.m_pStm->m_pCalcStmCorner->SetPosCorner(ixKey);
	if(bSetLoadOnly)
	{
		CalcStd.m_pStm->m_pCalcStmCorner->ResetLoadAtModel(static_cast<CStmModelForArcbridge*>(pStm));
	}
	else
	{
		CalcStd.m_pStm->m_pCalcStmCorner->MakeStmModelByTemplete(static_cast<CStmModelForArcbridge*>(pStm), FALSE);
	}

	return;
}

HGBOOL CInputStmArcCorner::IsStmUpperArcCorner(HGBOOL iKey)
{
	HGBOOL bUpper(TRUE);
	switch(iKey)
	{
	case STM_MODEL_CORNER_STT_UPPER_MINUS:
	case STM_MODEL_CORNER_END_UPPER_MINUS:
	case STM_MODEL_CORNER_STT_UPPER_PLUS:
	case STM_MODEL_CORNER_END_UPPER_PLUS:
	case STM_MODEL_CORNER_STT_UPPER_MINUS_EXT_1:
	case STM_MODEL_CORNER_END_UPPER_MINUS_EXT_1:
	case STM_MODEL_CORNER_STT_UPPER_PLUS_EXT_1:
	case STM_MODEL_CORNER_END_UPPER_PLUS_EXT_1:
	case STM_MODEL_CORNER_STT_UPPER_MINUS_EXT_2:
	case STM_MODEL_CORNER_END_UPPER_MINUS_EXT_2:
	case STM_MODEL_CORNER_STT_UPPER_PLUS_EXT_2:
	case STM_MODEL_CORNER_END_UPPER_PLUS_EXT_2:
		bUpper = TRUE;
		break;
	default:
		bUpper = FALSE;
		break;
	}

	return bUpper;
}

HGBOOL CInputStmArcCorner::IsStmOpenArcCorner(HGBOOL iKey)
{
	HGBOOL bOpen(TRUE);

	if( STM_MODEL_CORNER_STT_UPPER_MINUS <= iKey && iKey <= STM_MODEL_CORNER_END_LOWER_MINUS ||
		STM_MODEL_CORNER_STT_UPPER_MINUS_EXT_1 <= iKey && iKey <= STM_MODEL_CORNER_END_LOWER_MINUS_EXT_1 ||
		STM_MODEL_CORNER_STT_UPPER_MINUS_EXT_2 <= iKey && iKey <= STM_MODEL_CORNER_END_LOWER_MINUS_EXT_2)
		bOpen = FALSE;
	else
		bOpen = TRUE;

	return bOpen;
}