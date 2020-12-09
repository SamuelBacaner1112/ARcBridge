#include "stdafx.h"

#include "UserLoad.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CBridgeUserLoad::CBridgeUserLoad()
{
	m_nType		= 0;
	m_nIdxNo	= -1;		// "0" base
	m_Ux = m_Uy = m_Uz = m_Mx = m_My = m_Mz = 0;
	
	long i=0; for(i=0; i<2; i++)
	{
		m_dDist[i] = m_dLoadW2[i] = m_dLoadW3[i] = 0;
	}
}

CBridgeUserLoad::~CBridgeUserLoad()
{
}

void CBridgeUserLoad::Serialize (CArchive &ar)
{
	m_nFlag	= 0;
	if(ar.IsStoring())
	{
		ar << m_nType;
		ar << m_nIdxNo;		// "0" base
		ar << m_Ux;
		ar << m_Uy;
		ar << m_Uz;
		ar << m_Mx;
		ar << m_My;
		ar << m_Mz;
		
		long i=0; for(i=0; i<2; i++)
		{
			ar << m_dDist[i];	// 하중 시작위치,끝위치
			ar << m_dLoadW2[i];	// 하중크기, 시작,끝
			ar << m_dLoadW3[i];	// 하중크기, 시작,끝
		}
	}
	else
	{
		ar >> m_nType;
		ar >> m_nIdxNo;		// "0" base
		ar >> m_Ux;
		ar >> m_Uy;
		ar >> m_Uz;
		ar >> m_Mx;
		ar >> m_My;
		ar >> m_Mz;
		
		long i=0; for(i=0; i<2; i++)
		{
			ar >> m_dDist[i];	// 하중 시작위치,끝위치
			ar >> m_dLoadW2[i];	// 하중크기, 시작,끝
			ar >> m_dLoadW3[i];	// 하중크기, 시작,끝
		}
	}
}

CBridgeUserLoad& CBridgeUserLoad::operator=(const CBridgeUserLoad& Obj)
{
	m_nType = Obj.m_nType;
	m_nIdxNo = Obj.m_nIdxNo;		// "0" base
	m_Ux = Obj.m_Ux;
	m_Uy = Obj.m_Uy;
	m_Uz = Obj.m_Uz;
	m_Mx = Obj.m_Mx;
	m_My = Obj.m_My;
	m_Mz = Obj.m_Mz;

	for(long ix = 0; ix < 2; ix++)
	{
		m_dDist[ix] = Obj.m_dDist[ix];	// 하중 시작위치,끝위치
		m_dLoadW2[ix] = Obj.m_dLoadW2[ix];	// 하중크기, 시작,끝
		m_dLoadW3[ix] = Obj.m_dLoadW3[ix];	// 하중크기, 시작,끝
	}

	return *this;
}

CBridgeUserLoadSet::CBridgeUserLoadSet()
{
	m_bSelect = FALSE;
	m_szTitle = _T("");
	m_szComent = _T("");
	m_nTypeLoadCase = 0;
	m_sTypeLoadCase= _T("");
}

CBridgeUserLoadSet::~CBridgeUserLoadSet()
{
	AhTPADelete(&m_pArrUserLoad, (CBridgeUserLoadSet*)0);
}

void CBridgeUserLoadSet::Serialize (CArchive &ar)
{
	m_nFlag	= 2;
	if(ar.IsStoring())
	{
		ar << m_nFlag;

		ar << m_bSelect;
		ar << m_szTitle;
		ar << m_szComent;
		ar << m_nTypeLoadCase;
		ar << m_sTypeLoadCase;

		AhTPASerial(ar, &m_pArrUserLoad, (CBridgeUserLoad*)0);
	}
	else
	{
		ar >> m_nFlag;

		ar >> m_bSelect;
		ar >> m_szTitle;
		ar >> m_szComent;
		if(m_nFlag > 0)
		{
			ar >> m_nTypeLoadCase;
		}
		if(m_nFlag > 1)
		{
			ar >> m_sTypeLoadCase;
		}

		//m_pArrUserLoad.Serialize(ar);
		AhTPASerial(ar, &m_pArrUserLoad, (CBridgeUserLoad*)0);
	}
}

CBridgeUserLoadSet& CBridgeUserLoadSet::operator=(const CBridgeUserLoadSet& Obj)
{
	m_bSelect = Obj.m_bSelect;
	m_szTitle = Obj.m_szTitle;
	m_szComent = Obj.m_szComent;
	m_sTypeLoadCase = Obj.m_sTypeLoadCase;

	AhTPACopy(&m_pArrUserLoad, &Obj.m_pArrUserLoad, (CBridgeUserLoad*)0);

	return *this;
}