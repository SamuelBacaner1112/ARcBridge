// InputStmBracket.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ARcBridgeInput.h"
#include "InputStmBracket.h"

// CInputStmBracket 대화 상자입니다.

IMPLEMENT_DYNAMIC(CInputStmBracket, CInputStmModelBase)

CInputStmBracket::CInputStmBracket(CWnd* pParent /*=NULL*/)
	: CInputStmModelBase(CInputStmBracket::IDD, pParent)
{

}

CInputStmBracket::~CInputStmBracket()
{
}

void CInputStmBracket::DoDataExchange(CDataExchange* pDX)
{
	CInputStmModelBase::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CInputStmBracket, CInputStmModelBase)
	ON_BN_CLICKED(IDC_BTN_CHECKMAIN, &CInputStmBracket::OnBnClickedBtnCheckmain)
	ON_BN_CLICKED(IDC_BTN_CHECKSHEAR, &CInputStmBracket::OnBnClickedBtnCheckshear)
END_MESSAGE_MAP()


// CInputStmBracket 메시지 처리기입니다.


void CInputStmBracket::OnPreInitDialog()
{
	CInputStmModelBase::OnPreInitDialog();
}

void CInputStmBracket::OnBnClickedBtnCheckmain()
{
	CDialogSTMBracketMain dlg;
	dlg.m_pIn = m_pIn;
	dlg.m_ixStmKey = m_ixStmKey;
	if(dlg.DoModal() == IDOK)
	{
		if(dlg.m_bChange)
			AfxMessageBox(hggettext("철근배근이 수정되었습니다. STM해석을 다시 진행하시기 바랍니다."), MB_OK);
	}
}

void CInputStmBracket::OnBnClickedBtnCheckshear()
{
	CDialogSTMBracketShear dlg;
	dlg.m_pIn = m_pIn;
	dlg.m_ixStmKey = m_ixStmKey;
	if(dlg.DoModal() == IDOK)
	{
		if(dlg.m_bChange)
			AfxMessageBox(hggettext("철근배근이 수정되었습니다. STM해석을 다시 진행하시기 바랍니다."), MB_OK);
	}
}

void CInputStmBracket::InitModelCombo( HGINT32 iDepth/*=0*/ )
{
	if(iDepth != 0) return;

	CRcBridgeApp *pBri = m_pIn->GetRcBridge();

	m_ComboModel.ResetContent();
//	m_ComboCase.ResetContent();
	m_vKeyModel.clear();
	m_vKeyCase.clear();

	HGINT32 iKeyStt = STM_MODEL_BRACKET_STT;
	HGINT32 iKeyEnd = STM_MODEL_BRACKET_END;
	HGINT32 iAddCount = 0;

	for(HGINT32 iKey=iKeyStt; iKey<=iKeyEnd; iKey++)
	{
		CStmModel *pModel = pBri->GetStmModelByKey(iKey);
		if(pModel == NULL) continue;

		m_vKeyModel.push_back(iKey);

		CString strLoad = pBri->m_strStmModelingBracket[iKey - STM_MODEL_BRACKET_STT];
		CString str = pBri->GetStringStmModelNameByKey(iKey);
		if( strLoad.IsEmpty() == FALSE )
			str += _T("-") + strLoad;

		m_ComboModel.AddString(str);
		++iAddCount;
	}

	m_nCurModel = 0;
	if(iAddCount > 0)
	{
		m_ComboModel.SetCurSel(0);
	}

	SetCurModelKey();

//	CInputStmModelBase::InitModelCombo(iDepth);
}

BOOL CInputStmBracket::IsValid()
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
		if(pBridge->m_bApplyStmModelingBracket[ix])
		{
			bValid = TRUE;
			break;
		}
	}

	return bValid;
}

BOOL CInputStmBracket::InitStmModel(CStmModel *pModel)
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
		HGINT32 iKeyCur = pBridge->GetStmKeyByStmModel(pModel);
		if(iKeyCur > -1)
		{
			MakeStmTrussData(m_pIn->m_nCurrentBridge, iKeyCur, TRUE);
		}
	}

	return CInputStmModelBase::InitStmModel(pModel);
}

void CInputStmBracket::MakeStmStressFlowData( CStmModel *pStm, HGINT32 nBri, HGINT32 ixKey )
{
	CRcBridgeRebar *pBridge = m_pIn->m_pARcBridgeDataStd->GetBridgeRebar(nBri);
	if (!pBridge) return;

	if(pStm->IsLock())
		return;

	HGBOOL bStt = (ixKey == STM_MODEL_BRACKET_STT)? TRUE : FALSE;

	CARcBridgeCalcStd CalcStd(pBridge);
	CalcStd.m_pStm->m_pCalcStmBracket->SetPosStt(bStt);
	CalcStd.m_pStm->m_pCalcStmBracket->MakeStmModelByTemplete(pStm, TRUE);

	return;
}

void CInputStmBracket::MakeStmTrussData( HGINT32 nBri, HGINT32 ixKey, BOOL bSetLoadOnly )
{
	CRcBridgeRebar *pBridge = m_pIn->m_pARcBridgeDataStd->GetBridgeRebar(nBri);
	if (!pBridge) return;

	CStmModel *pStm = pBridge->GetStmModelByKey(ixKey);
	if(pStm == NULL)
		return;

	HGBOOL bStt = (ixKey == STM_MODEL_BRACKET_STT)? TRUE : FALSE;

	CARcBridgeCalcStd CalcStd(pBridge);
	CalcStd.m_pStm->m_pCalcStmBracket->SetPosStt(bStt);

	if(bSetLoadOnly)
	{
		CalcStd.m_pStm->m_pCalcStmBracket->ResetLoadAtModel(pStm);
	}
	else
	{
		CalcStd.m_pStm->m_pCalcStmBracket->MakeStmModelByTemplete(pStm, FALSE);
	}

	return;
}
