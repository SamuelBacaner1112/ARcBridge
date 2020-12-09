// GirderAndColumn.cpp: implementation of the CGirderAndColumn class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GirderAndColumn.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGirderAndColumn::CGirderAndColumn()
{
	m_nJijum  = -1;
	m_bGirder = FALSE;
	m_bColumn = FALSE;
	m_bWall	  = FALSE;
	
	long i=0; for(i=0; i<RCBRIDGE_GIR_CHECK; i++)
	{
		m_dSpanLengthGirder[i]	= 0;
		for(long ixTypeEnv=0; ixTypeEnv<RCBRIDGE_GIR_CHECK_ENV_LSD; ++ixTypeEnv)
		{
			m_dAxialForceTop[i][ixTypeEnv]		= 0;
			m_dAxialForceBot[i][ixTypeEnv]		= 0;

			for(long kk=0; kk<2; kk++)
			{
				m_GirMomentPlus[kk][i][ixTypeEnv]	= 0;
				m_GirMomentMinus[kk][i][ixTypeEnv]	= 0;
				m_GirShear[kk][i][ixTypeEnv]		= 0;
			}
		}

		m_szCalcLn[i] = _T("");
	}
	m_dDivOutBeamPlus	= 14.0;
	m_dDivOutBeamMinus	= 10.0;
	m_dDivInBeamPlus	= 16.0;
	m_dDivInBeamMinus	= 11.0;
	m_dFactorShear		= 1.15;

	m_dCtcColumn = 0;
	for(i=0; i<RCBRIDGE_COL_CHECK; i++)
	{
		m_dAxialBot[i]	= 0;
		m_dMomentBot[i]	= 0;
		m_dColAxial[i]	= 0;
		m_dColMoment[i]	= 0;
		m_nTypeDesign[i]= -1;
		m_nCombo[i]		= -1;
		pConcColumn[i] = new CConcBasic;
	}

	for (HGINT32 ix = 0; ix < 4; ++ix)
	{
		for (HGINT32 jx = 0; jx < RCBRIDGE_COL_CHECK_LSD; ++jx)
		{
			pConcColumnLsd[ix][jx] = new CConcBasic;
		}
	}
	
	for(long ixEnvType=0; ixEnvType < 3; ++ixEnvType)
	{
		pConcGirEnd[iUPPER][ixEnvType] = new CConcBasic;
		pConcGirMid[iUPPER][ixEnvType] = new CConcBasic;
		pConcGirEnd[iLOWER][ixEnvType] = new CConcBasic;
		pConcGirMid[iLOWER][ixEnvType] = new CConcBasic;
	}
}

CGirderAndColumn::~CGirderAndColumn()
{
	for(long ixEnvType=0; ixEnvType < 3; ++ixEnvType)
	{
		delete pConcGirEnd[iUPPER][ixEnvType];
		delete pConcGirMid[iUPPER][ixEnvType];
		delete pConcGirEnd[iLOWER][ixEnvType];
		delete pConcGirMid[iLOWER][ixEnvType];
	}
	for(long i=0; i<RCBRIDGE_COL_CHECK; i++)
	{
		delete pConcColumn[i];
	}

	for (HGINT32 ix = 0; ix < 4; ++ix)
	{
		for (HGINT32 jx = 0; jx < RCBRIDGE_COL_CHECK_LSD; ++jx)
		{
			SAFE_DELETE(pConcColumnLsd[ix][jx]);
		}
	}
}

void CGirderAndColumn::CalulateForce()
{
	//////////////////////////////////////////////////////////////////////////

	long i=0; for(i=0; i<RCBRIDGE_COL_CHECK; i++)
	{
		m_dColAxial[i]  = m_dAxialBot[i]*m_dCtcColumn;
		m_dColMoment[i] = m_dMomentBot[i]*m_dCtcColumn;
		m_dColShear[i]  = m_dShearBot[i]*m_dCtcColumn;
	}

	if(m_bGirder==FALSE) return;
	for(long ixTypeEnv=0; ixTypeEnv<RCBRIDGE_GIR_CHECK_ENV_LSD; ++ixTypeEnv)
	{	
		// 상부거더
		long nIdx = RCBRIDGE_GIR_POS_END; // 지점부
		double W = m_dAxialForceTop[nIdx][ixTypeEnv];
		double L = m_dSpanLengthGirder[nIdx];
		m_GirMomentPlus[iUPPER][nIdx][ixTypeEnv]  = W*pow(L,2)/m_dDivOutBeamPlus;
		m_GirMomentMinus[iUPPER][nIdx][ixTypeEnv] = W*pow(L,2)/m_dDivOutBeamMinus;
		m_GirShear[iUPPER][nIdx][ixTypeEnv]	   = m_dFactorShear*W*L/2.0 - 0.15*W*L;
		//
		nIdx = RCBRIDGE_GIR_POS_MID; // 중앙부
		W = m_dAxialForceTop[nIdx][ixTypeEnv];
		L = m_dSpanLengthGirder[nIdx];
		m_GirMomentPlus[iUPPER][nIdx][ixTypeEnv]  = W*pow(L,2)/m_dDivInBeamPlus;
		m_GirMomentMinus[iUPPER][nIdx][ixTypeEnv] = W*pow(L,2)/m_dDivInBeamMinus;
		m_GirShear[iUPPER][nIdx][ixTypeEnv]	   = m_dFactorShear*W*L/2.0 - 0.15*W*L;

		// 하부거더 
		nIdx = RCBRIDGE_GIR_POS_END; // 지점부
		W = m_dAxialForceBot[nIdx][ixTypeEnv];
		L = m_dSpanLengthGirder[nIdx];
		m_GirMomentPlus[iLOWER][nIdx][ixTypeEnv]  = W*pow(L,2)/m_dDivOutBeamPlus;
		m_GirMomentMinus[iLOWER][nIdx][ixTypeEnv] = W*pow(L,2)/m_dDivOutBeamMinus;
		m_GirShear[iLOWER][nIdx][ixTypeEnv]	   = m_dFactorShear*W*L/2.0 - 0.15*W*L;
		//
		nIdx = RCBRIDGE_GIR_POS_MID; // 중앙부
		W = m_dAxialForceBot[nIdx][ixTypeEnv];
		L = m_dSpanLengthGirder[nIdx];
		m_GirMomentPlus[iLOWER][nIdx][ixTypeEnv]  = W*pow(L,2)/m_dDivInBeamPlus;
		m_GirMomentMinus[iLOWER][nIdx][ixTypeEnv] = W*pow(L,2)/m_dDivInBeamMinus;
		m_GirShear[iLOWER][nIdx][ixTypeEnv]	   = m_dFactorShear*W*L/2.0 - 0.15*W*L;
	}
}
