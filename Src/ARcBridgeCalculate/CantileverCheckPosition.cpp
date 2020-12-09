#include "stdafx.h"
#include "CantileverCheckPosition.h"


CCantileverCheckPosition::CCantileverCheckPosition(void)
{
	m_sName = _T("");

	for (long ix=0; ix < eLSD_COMBINATION_COUNT; ++ix)
	{
		m_dMoment[ix] = 0.;
		m_sRebarCheck[ix] = _T("");
	}

	m_dThickSlab = 0;
	m_dThickSlabReqMin = 0;
	m_dThickSlabReqGroove = 0;

	m_dAsReq_Support = 0;
	m_dAsUse_Support = 0;
	m_dCtcMax_Support = 0;
	m_dCtcUse_Support = 0;

	m_dAsUse_EndSupport = 0;
	m_dAsUse_EndMain = 0;
	m_dAsReq_EndMain = 0;

	m_dtMin_SpanLength = 0;
	m_dtMin_Deflection = 0;
	m_dtmin_Div = 0;
}

CCantileverCheckPosition::~CCantileverCheckPosition(void)
{
	ClearVectorPtr(m_vLoad);
	for(long ix = 0; ix < eLSD_COMBINATION_COUNT; ix++)
	{
		ClearVectorPtr(m_vCombo[ix]);
	}
	ClearVectorPtr(m_vConc);
}

CString CCantileverCheckPosition::GetName() const
{
	return m_sName;
}

void CCantileverCheckPosition::SetName(CString strName)
{
	m_sName = strName;
}

double CCantileverCheckPosition::GetMoment(ELSDLoadGroupKind eGroup) const
{ 
	return m_dMoment[eGroup];
}

void CCantileverCheckPosition::AddLoad(stCantileverLoadUnit *pLoad )
{
	if(pLoad == NULL) return;

	m_vLoad.push_back(pLoad);
}

vector<stCantileverLoadUnit*> CCantileverCheckPosition::GetLoads()
{
	return m_vLoad;
}

void CCantileverCheckPosition::AddComboData(stCantileverCombo *pCombo, ELSDLoadGroupKind eGroupKind)
{
	m_vCombo[eGroupKind].push_back(pCombo);
}

BOOL CCantileverCheckPosition::IsLoad( ELSDLoadCaseKind ePos )
{
	vector<stCantileverLoadUnit*> m_vLoad = GetLoads();

	long icLoad = m_vLoad.size();
	for(long ix = 0; ix < icLoad; ++ix)
	{
		if(ePos == m_vLoad[ix]->GetLoadCaseKind())
			return TRUE;
	}

	return FALSE;
}

stCantileverLoadUnit* CCantileverCheckPosition::GetLoad( CString strName )
{
	vector<stCantileverLoadUnit*> m_vLoads = GetLoads();

	long icLoad = m_vLoads.size();
	for(long ix = 0; ix < icLoad; ++ix)
	{
		if( strName == m_vLoads[ix]->sName )
			return m_vLoads[ix];
	}

	return NULL;
}

BOOL CCantileverCheckPosition::IsSameLoadCase()
{
	BOOL bSameLoad = FALSE;

	vector<stCantileverLoadUnit*> vLoads = GetLoads();

	long icLoad = vLoads.size();
	for(long ix = 0; ix < icLoad-1; ++ix)
	{
		stCantileverLoadUnit *pLoad = vLoads[ix];
		if(pLoad == NULL) continue;
		stCantileverLoadUnit *pLoadNext = vLoads[ix+1];
		if(pLoadNext == NULL) continue;

		if(pLoad->GetLoadCaseKind() == pLoadNext->GetLoadCaseKind())
		{
			bSameLoad = TRUE;
			break;
		}
	}

	return bSameLoad;
}

double CCantileverCheckPosition::GetSumMomentLoadCase( ELSDLoadCaseKind eLoadCase )
{
	double dSumCase = 0;

	vector<stCantileverLoadUnit*> vLoads = GetLoads();

	long icLoad = vLoads.size();
	for(long ix = 0; ix < icLoad-1; ++ix)
	{
		stCantileverLoadUnit *pLoad = vLoads[ix];
		if(pLoad == NULL) continue;
		
		if(pLoad->GetLoadCaseKind() == eLoadCase)
		{
			dSumCase += pLoad->GetMoment(FALSE);
			break;
		}
	}

	return dSumCase;
}

long CCantileverCheckPosition::GetSizeCombo()
{
	long nSizeCombo = 0;
	for(long nStatus = 0; nStatus < eLSD_COMBINATION_COUNT; nStatus++)
	{
		nSizeCombo += GetSizeCombo(ELSDLoadGroupKind(nStatus));
	}

	return nSizeCombo;
}

long CCantileverCheckPosition::GetSizeCombo( ELSDLoadGroupKind eStatus )
{
	return m_vCombo[eStatus].size();
}

stCantileverCombo * CCantileverCheckPosition::GetCombo( ELSDLoadGroupKind eGroupKind, long nIdex )
{
	return GetVectorElement(m_vCombo[eGroupKind], nIdex);
}

double CCantileverCheckPosition::GetThickSlabRequire( BOOL bMinThick )
{
	return bMinThick ? m_dThickSlabReqMin : m_dThickSlabReqGroove;
}

BOOL CCantileverCheckPosition::IsOkMinThick( BOOL bMinThick )
{
	if(bMinThick)
	{
		return m_dThickSlab >= m_dThickSlabReqMin;
	}
	else
	{
		return m_dThickSlab >= m_dThickSlabReqGroove;
	}
}

void CCantileverCheckPosition::AddConcData( CConcBasic *pConc )
{
	if(pConc == NULL) return;

	m_vConc.push_back(pConc);
}

CConcBasic * CCantileverCheckPosition::GetConcData( long nIndex )
{
	if(nIndex < 0 || (long)m_vConc.size() <= nIndex) return NULL;

	return m_vConc[nIndex];
}

long CCantileverCheckPosition::GetSizeConcData()
{
	return m_vConc.size();
}

void CCantileverCheckPosition::SetMomentByGroup(CLsdManager *pLsdManager)
{
	CString szStatus[4] = { _T("극한한계상태"), _T("극단상황한계상태"), _T("사용한계상태"), _T("피로한계상태") };	

	for(long iStatus = eLSD_COMBINATION_LIMIT; iStatus < eLSD_COMBINATION_COUNT; iStatus++)
	{
		m_dMoment[iStatus] = 0;
		m_sRebarCheck[iStatus] = _T("");
		long nSizeCombo = GetSizeCombo((ELSDLoadGroupKind)iStatus);
		if(nSizeCombo == 0) continue;

		for(long nCombo = 0; nCombo < nSizeCombo; nCombo++)
		{
			stCantileverCombo *pCombo = GetCombo((ELSDLoadGroupKind)iStatus, nCombo);
			if(pCombo == NULL) continue;

			if(m_dMoment[iStatus] < pCombo->m_dMoment)
			{
				m_dMoment[iStatus] = pCombo->m_dMoment;
				m_sRebarCheck[iStatus].Format(_T("%s : %s"), szStatus[iStatus], pCombo->m_sName);
			}
		}
	}
}

CString CCantileverCheckPosition::GetStringRebarCheck( ELSDLoadGroupKind eGroupKind ) const
{
	return m_sRebarCheck[eGroupKind];
}

void CCantileverCheckPosition::ClearConcData()
{
	ClearVectorPtr(m_vConc);
}

void CCantileverCheckPosition::CalculateSlabThick(double dThick)
{
	m_dThickSlab = dThick;

	m_dThickSlabReqGroove = 240;
	m_dThickSlabReqMin = 220;
}

void CCantileverCheckPosition::CalculateSupportRebar()
{
	if(m_vConc.size() == 0) return;
	CConcBasic *pConc = m_vConc[0];

	// 철근량
	long nDan = 0;
	double dAg = pConc->m_Bo * pConc->m_Ho;
	double dFy = GetValueUnitChange(pConc->m_Fy, UNIT_CGS_KGF_CM2, _T("N/mm²"));

	m_dAsReq_Support = SUPPORT_REBAR_AREA_FACTOR * dAg / dFy;
	m_dAsUse_Support = frCm2(pConc->GetRebarAreaSupport(nDan, TRUE));

	// 간격
	m_dCtcMax_Support = MIN(SUPPORT_REBAR_CTC_FACTOR * pConc->m_Ho, SUPPORT_REBAR_CTC_MAX);
	m_dCtcUse_Support = pConc->m_TSupportBarCTC[nDan];
}

double CCantileverCheckPosition::GetAsReqSupport()
{
	return m_dAsReq_Support;
}

double CCantileverCheckPosition::GetAsUseSupport()
{
	return m_dAsUse_Support;
}

double CCantileverCheckPosition::GetCtcUseSupport()
{
	return m_dCtcUse_Support;
}

double CCantileverCheckPosition::GetCtcMaxSupport()
{
	return m_dCtcMax_Support;
}

BOOL CCantileverCheckPosition::IsOkSupportAs()
{
	return m_dAsUse_Support >= m_dAsReq_Support;
}

BOOL CCantileverCheckPosition::IsOkSupportCtc()
{
	return m_dCtcUse_Support <= m_dCtcMax_Support;
}

double CCantileverCheckPosition::GetThickSlab()
{
	return m_dThickSlab;
}

void CCantileverCheckPosition::CalculateDeflection( double dSpanLength )
{
	m_dtmin_Div = DEFELECTION_DIV_FACTOR_CAN;
	m_dtMin_SpanLength = dSpanLength;
	m_dtMin_Deflection = m_dtMin_SpanLength / m_dtmin_Div;
}

double CCantileverCheckPosition::GettMinDivide()
{
	return m_dtmin_Div;
}

double CCantileverCheckPosition::GettMinSpanLength()
{
	return m_dtMin_SpanLength;
}

double CCantileverCheckPosition::GettMinDeflection()
{
	return m_dtMin_Deflection;
}

BOOL CCantileverCheckPosition::IsNeedDeflectionCheck()
{
	return m_dThickSlab < m_dtMin_Deflection;
}
