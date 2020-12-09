// Jijum.cpp: implementation of the CJijum class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Jijum.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CJijum::CJijum()
{
	m_nFlag	= 0;
	int i=0; for(i=0; i<2; i++)
	{
		m_dLengthUgan[i] = 0;				// 유간 거리
		m_dLengthSlabToShoe[i] = 0;			// 종방향 교량 받침 거리
	}
	m_vAngle = CDPoint(0, 0);				// 교량 받침 각도
	m_dStaJijum = 0;						// 지점 스테이션

	for(i=0; i<2; i++)
	{
		m_nQtyShoe[i] = 0;
		m_strJijumShoe[i] = _T("");					// 교량 받침 열의 이름
		int j=0; for(j=0; j<MAX_SHOE_COUNT_RC; j++)
		{
			m_dDistShoe[i][j] = 0.0;	// 중심 선형으로 부터의 교량 받침 법선 거리
			m_nDirShoe[i][j]  = 3;		// 교량 받침 방향
			m_dKh[i][j]	= 0;			// 탄성받침일 경우의 전단 스프링계수
			m_dKv[i][j]	= 0;			// 탄성받침일 경우의 압축 스프링계수
		}
	}

	for(i=0; i<2; i++)
	{
		m_dWidBlockOut[i] = 0;
		m_dHeightBlockOut[i] = 0;
	}
}

CJijum::~CJijum()
{

}

CJijum& CJijum::operator=(const CJijum& Obj)
{
	int i=0; for(i=0; i<2; i++)
	{
		m_dLengthUgan[i] = Obj.m_dLengthUgan[i];				// 유간 거리
		m_dLengthSlabToShoe[i] = Obj.m_dLengthSlabToShoe[i];	// 종방향 교량 받침 거리
	}
	m_vAngle = Obj.m_vAngle;				// 교량 받침 각도
	m_dStaJijum	= Obj.m_dStaJijum;			// 지점 스테이션

	for(i=0; i<2; i++)
	{
		m_nQtyShoe[i] = Obj.m_nQtyShoe[i];
		m_strJijumShoe[i] =Obj.m_strJijumShoe[i];						// 교량 받침 열의 이름
		int j=0; for(j=0; j<MAX_SHOE_COUNT_RC; j++)
		{
			m_dDistShoe[i][j] = Obj.m_dDistShoe[i][j];					// 중심 선형으로 부터의 교량 받침 법선 거리
			m_nDirShoe[i][j] = Obj.m_nDirShoe[i][j];					// 교량 받침 방향
			m_dKh[i][j]	= Obj.m_dKh[i][j];			// 탄성받침일 경우의 전단 스프링계수
			m_dKv[i][j]	= Obj.m_dKv[i][j];			// 탄성받침일 경우의 압축 스프링계수
		}
	}

	for(i=0; i<2; i++)
	{
		m_dWidBlockOut[i] = Obj.m_dWidBlockOut[i];
		m_dHeightBlockOut[i] = Obj.m_dHeightBlockOut[i];
	}

	return *this;
}

void CJijum::Serialize(CArchive &ar)
{
	if(ar.IsStoring())
	{
		int i=0; for(i=0; i<2; i++)
		{
			ar << m_dLengthUgan[i];				// 유간 거리
			ar << m_dLengthSlabToShoe[i];		// 종방향 교량 받침 거리
		}
		ar << m_vAngle;							// 교량 받침 각도
		ar << m_dStaJijum;						// 지점 스테이션

		for(i=0; i<2; i++)
		{
			ar << m_nQtyShoe[i];
			ar << m_strJijumShoe[i];			// 교량 받침 열의 이름
			// 이함수의 MAX_SHOE_COUNT_RC_OLD는 변경하면 안된다.
			// 애초에 CJijumData에 flag가 없어서 MAX_SHOE_COUNT_RC_OLD와 관련된 증가시 이부분은 그대로 둔다
			int j=0; for(j=0; j<MAX_SHOE_COUNT_RC_OLD; j++)
			{
				ar << m_dDistShoe[i][j];		// 중심 선형으로 부터의 교량 받침 법선 거리
				ar << m_nDirShoe[i][j];			// 교량 받침 방향
			}
		}
	}
	else
	{
		int i=0; for(i=0; i<2; i++)
		{
			ar >> m_dLengthUgan[i];				// 유간 거리
			ar >> m_dLengthSlabToShoe[i];		// 종방향 교량 받침 거리
		}
		ar >> m_vAngle;							// 교량 받침 각도
		ar >> m_dStaJijum;						// 지점 스테이션

		for(i=0; i<2; i++)
		{
			ar >> m_nQtyShoe[i];
			ar >> m_strJijumShoe[i];			// 교량 받침 열의 이름
			// 이함수의 MAX_SHOE_COUNT_RC_OLD는 변경하면 안된다.
			// 애초에 CJijumData에 flag가 없어서 MAX_SHOE_COUNT_RC_OLD와 관련된 증가시 이부분은 그대로 둔다
			int j=0; for(j=0; j<MAX_SHOE_COUNT_RC_OLD; j++)
			{
				ar >> m_dDistShoe[i][j];		// 중심 선형으로 부터의 교량 받침 법선 거리
				ar >> m_nDirShoe[i][j];			// 교량 받침 방향
			}
		}
	}
}

void CJijum::SetAngJijum(CDPoint vAng)
{
	m_vAngle = vAng;
}

BOOL CJijum::IsFixJijum()
{
	for(long ix = iSTT; ix <= iEND; ix++)
	{
		for(long jx = 0; jx < m_nQtyShoe[ix]; jx++)
		{
			if(m_nDirShoe[ix][jx] == 0)
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}

// 기존의 Serialize가 flag가 없어서 부득이하게 flag있는 것으로 새로 만듬. ㅠㅠ
// SerializeNew로 시리얼을 옮기면서 MAX_SHOE_COUNT_RC 20 -> 30으로 조정하고
// 기존에 CRcBridgeData에서 따로 읽어오고 저장했던 Kh, Kv들을 여기서 저장하고 읽어옴.
// 완전히 새로운 함수기 때문에 Flag를 조정하지는 않았음.
// 이 함수로 들어오는 flag는 RcBridgeData에서 조정할 것임
void CJijum::SerializeNew(CArchive &ar)
{
	// 1 : MAX_SHOE_COUNT_RC : 30 -> 40
	// 2 : 블럭아웃 추가 (  BOX_TYPE )
	m_nFlag	= 2;

	if(ar.IsStoring())
	{
		ar << m_nFlag;

		int i=0; for(i=0; i<2; i++)
		{
			ar << m_dLengthUgan[i];				// 유간 거리
			ar << m_dLengthSlabToShoe[i];		// 종방향 교량 받침 거리
		}
		ar << m_vAngle;							// 교량 받침 각도
		ar << m_dStaJijum;						// 지점 스테이션

		for(i=0; i<2; i++)
		{
			ar << m_nQtyShoe[i];
			ar << m_strJijumShoe[i];			// 교량 받침 열의 이름
			int j=0; for(j=0; j<MAX_SHOE_COUNT_RC; j++)
			{
				ar << m_dDistShoe[i][j];		// 중심 선형으로 부터의 교량 받침 법선 거리
				ar << m_nDirShoe[i][j];			// 교량 받침 방향
			}
		}

		for(i=0; i<MAX_SHOE_COUNT_RC; i++)
		{
			ar << m_dKh[0][i];
			ar << m_dKh[1][i];
			ar << m_dKv[0][i];
			ar << m_dKv[1][i];
		}

		for(i=0; i<2; i++)
		{
			ar << m_dWidBlockOut[i];
			ar << m_dHeightBlockOut[i];
		}
	}
	else
	{
		ar >> m_nFlag;

		int i=0; for(i=0; i<2; i++)
		{
			ar >> m_dLengthUgan[i];				// 유간 거리
			ar >> m_dLengthSlabToShoe[i];		// 종방향 교량 받침 거리
		}
		ar >> m_vAngle;							// 교량 받침 각도
		ar >> m_dStaJijum;						// 지점 스테이션

		long nMaxShoeCount	= MAX_SHOE_COUNT_RC;
		if(m_nFlag < 1)
			nMaxShoeCount	= MAX_SHOE_COUNT_RC_20090716;

		for(i=0; i<2; i++)
		{
			ar >> m_nQtyShoe[i];
			ar >> m_strJijumShoe[i];			// 교량 받침 열의 이름
			int j=0; for(j=0; j<nMaxShoeCount; j++)
			{
				ar >> m_dDistShoe[i][j];		// 중심 선형으로 부터의 교량 받침 법선 거리
				ar >> m_nDirShoe[i][j];			// 교량 받침 방향
			}
		}

		for(i=0; i<nMaxShoeCount; i++)
		{
			ar >> m_dKh[0][i];
			ar >> m_dKh[1][i];
			ar >> m_dKv[0][i];
			ar >> m_dKv[1][i];
		}

		if(m_nFlag > 1)
		{
			for(i=0; i<2; i++)
			{
				ar >> m_dWidBlockOut[i];
				ar >> m_dHeightBlockOut[i];
			}
		}
	}
}
