#include "stdafx.h"
#include "CheckCantileverData.h"

//////////////////////////////////////////////////////////////////////////
CWorkingLoad::CWorkingLoad(void)
{
	// 방음벽, 난간
	for (long ix=0; ix<2; ++ix)
	{
		m_UserLoad[ix].m_bExist = FALSE;
		m_UserLoad[ix].m_dWeight = 0;
		m_UserLoad[ix].m_dHeight = 0;
		m_UserLoad[ix].m_dDist = 0;
		m_UserLoad[ix].m_bDw = FALSE;
	}

	m_BodoLoad.m_bExist = FALSE;
	m_BodoLoad.m_dWeightStt = 0;
	m_BodoLoad.m_dWeightEnd = 0;
	m_BodoLoad.m_dHeight	= 0;

	// 군중하중
	m_bWalkLoad = FALSE;	
	m_nWorkLoadType = 0;
	m_dHoriF	= 250;
	m_dVerF		= 100;
	m_nHorType	= 0;
}

CWorkingLoad::~CWorkingLoad(void)
{

}

void CWorkingLoad::Serialize(CArchive &ar)
{
	int nFlag = 0;

	if(ar.IsStoring())
	{
		ar << nFlag;
		for (long ix=0; ix<2; ++ix)
		{
			ar << m_UserLoad[ix].m_bExist;
			ar << m_UserLoad[ix].m_dWeight;
			ar << m_UserLoad[ix].m_dHeight;
			ar << m_UserLoad[ix].m_dDist;
			ar << m_UserLoad[ix].m_bDw;
		}

		ar << m_BodoLoad.m_bExist;
		ar << m_BodoLoad.m_dWeightStt;
		ar << m_BodoLoad.m_dWeightEnd;
		ar << m_BodoLoad.m_dHeight;

		ar << m_bWalkLoad;
		ar << m_dHoriF;
		ar << m_dVerF;
		ar << m_nHorType;
		ar << m_nWorkLoadType;
	}
	else
	{
		ar >> nFlag;

		for (long ix=0; ix<2; ++ix)
		{
			ar >> m_UserLoad[ix].m_bExist;
			ar >> m_UserLoad[ix].m_dWeight;
			ar >> m_UserLoad[ix].m_dHeight;
			ar >> m_UserLoad[ix].m_dDist;
			ar >> m_UserLoad[ix].m_bDw;
		}

		ar >> m_BodoLoad.m_bExist;
		ar >> m_BodoLoad.m_dWeightStt;
		ar >> m_BodoLoad.m_dWeightEnd;
		ar >> m_BodoLoad.m_dHeight;

		ar >> m_bWalkLoad;
		ar >> m_dHoriF;
		ar >> m_dVerF;
		ar >> m_nHorType;
		ar >> m_nWorkLoadType;
	}
}

CWorkingLoad& CWorkingLoad::operator=(const CWorkingLoad &Obj)
{
	if(this==&Obj)
		return *this;

	for (long ix=0; ix<2; ++ix)
	{
		m_UserLoad[ix].m_bExist = Obj.m_UserLoad[ix].m_bExist;
		m_UserLoad[ix].m_dWeight = Obj.m_UserLoad[ix].m_dWeight;
		m_UserLoad[ix].m_dHeight = Obj.m_UserLoad[ix].m_dHeight;
		m_UserLoad[ix].m_dDist = Obj.m_UserLoad[ix].m_dDist;
		m_UserLoad[ix].m_bDw = Obj.m_UserLoad[ix].m_bDw;
	}

	m_BodoLoad.m_bExist = Obj.m_BodoLoad.m_bExist;
	m_BodoLoad.m_dWeightStt = Obj.m_BodoLoad.m_dWeightStt;
	m_BodoLoad.m_dWeightEnd = Obj.m_BodoLoad.m_dWeightEnd;
	m_BodoLoad.m_dHeight = Obj.m_BodoLoad.m_dHeight;

	m_bWalkLoad = Obj.m_bWalkLoad;
	m_dHoriF = Obj.m_dHoriF;
	m_dVerF = Obj.m_dVerF;
	m_nHorType = Obj.m_nHorType;
	m_nWorkLoadType = Obj.m_nWorkLoadType;
	return *this;
}

_USER_LOAD &CWorkingLoad::GetUserLoad(long nType)
{
	return m_UserLoad[nType];
}

_BODO &CWorkingLoad::GetBodoWeight()
{
	return m_BodoLoad;
}

BOOL& CWorkingLoad::GetWalkLoad()
{
	return m_bWalkLoad;
}

long &CWorkingLoad::GetWorkLoadType()
{
	return m_nWorkLoadType;
}

double &CWorkingLoad::GetHoriF()
{
	return m_dHoriF;
}

double &CWorkingLoad::GetVertF()
{
	return m_dVerF;
}

long &CWorkingLoad::GetHoriType()
{
	return m_nHorType;
}
//////////////////////////////////////////////////////////////////////////

CCheckCantileverData::CCheckCantileverData(void)
{
	m_dCoverUp = -1;
	m_dCoverDn = -1;
	m_dCoverSd = -1;

	//곡선반경
	for (long n=0; n<3; ++n)
	{
		m_dCenter_R[n] = 0;
	}
	m_dCo_V		= 0;
//	m_dCo_H		= 0;

	// 2. 원심하중
	m_nCent_HeightType	= 0;

	m_nSlabCentDesignMethod = 0;
	m_bWind				= FALSE;	// 바닥판 계산용
	m_bFloowWind		= TRUE;		// 바닥판  - 하중재하 고려사항 checkbox 추가 	2010_02_18 
	m_dWindStrength		= 0;
	m_bFloorTrailer		= FALSE;
	m_bFloorTank		= FALSE;
	m_bCentMugun		= FALSE;
	m_bMugun            = FALSE;
}


CCheckCantileverData::~CCheckCantileverData(void)
{
}

void CCheckCantileverData::Serialize(CArchive &ar)
{
	int nFlag = 2;		// 설계선택사항 추가, 2 : m_dCenter_R 데이터 변경 (좌,우,중앙)

	if(ar.IsStoring())
	{
		ar << nFlag;

		m_WorkingLoadData->Serialize(ar);

		for (long n=0; n<3; ++n)
		{
			ar << m_dCenter_R[n];
		}
		ar << m_dCo_V;
		ar << m_bCentMugun;
		ar << m_bMugun;
		ar << m_bFloorTank;
		ar << m_bFloorTrailer;
		ar << m_dWindStrength;
		ar << m_bWind;
		ar << m_bFloowWind;
		ar << m_nSlabCentDesignMethod;
		ar << m_nCent_HeightType;
		ar << m_dCoverUp;
		ar << m_dCoverDn;
		ar << m_dCoverSd;
	}
	else
	{
		ar >> nFlag;

		m_WorkingLoadData->Serialize(ar);

		if( nFlag > 0)
		{
			if( nFlag > 1 )
			{
				for (long n=0; n<3; ++n)
				{
					ar >> m_dCenter_R[n];
				}
			}
			else
				ar >> m_dCenter_R[0];
			ar >> m_dCo_V;
			ar >> m_bCentMugun;
			ar >> m_bMugun;
			ar >> m_bFloorTank;
			ar >> m_bFloorTrailer;
			ar >> m_dWindStrength;
			ar >> m_bWind;
			ar >> m_bFloowWind;
			ar >> m_nSlabCentDesignMethod;
			ar >> m_nCent_HeightType;
			ar >> m_dCoverUp;
			ar >> m_dCoverDn;
			ar >> m_dCoverSd;
		}
	}
}

CCheckCantileverData& CCheckCantileverData::operator =(const CCheckCantileverData &Obj)
{
	if(this==&Obj)
		return *this;

	for (long n=0; n<3; ++n)
	{
		m_dCenter_R[n] = Obj.m_dCenter_R[n];
	}
	
	m_dCo_V = Obj.m_dCo_V;
	m_bCentMugun = Obj.m_bCentMugun;
	m_bMugun = Obj.m_bMugun;
	m_bFloorTank = Obj.m_bFloorTank;
	m_bFloorTrailer = Obj.m_bFloorTrailer;

	m_dWindStrength = Obj.m_dWindStrength;
	m_bFloowWind = Obj.m_bFloowWind;
	m_nSlabCentDesignMethod = Obj.m_nSlabCentDesignMethod;
	m_nCent_HeightType = Obj.m_nCent_HeightType;
	m_dCoverUp = Obj.m_dCoverUp;
	m_dCoverDn = Obj.m_dCoverDn;
	m_dCoverSd = Obj.m_dCoverSd;
	return *this;
}

CWorkingLoad *CCheckCantileverData::GetWorkingLoadData(HGBOOL bLeft)
{
	return &m_WorkingLoadData[bLeft==TRUE ? 0 : 1];
}