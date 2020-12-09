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
		m_dLengthUgan[i] = 0;				// ���� �Ÿ�
		m_dLengthSlabToShoe[i] = 0;			// ������ ���� ��ħ �Ÿ�
	}
	m_vAngle = CDPoint(0, 0);				// ���� ��ħ ����
	m_dStaJijum = 0;						// ���� �����̼�

	for(i=0; i<2; i++)
	{
		m_nQtyShoe[i] = 0;
		m_strJijumShoe[i] = _T("");					// ���� ��ħ ���� �̸�
		int j=0; for(j=0; j<MAX_SHOE_COUNT_RC; j++)
		{
			m_dDistShoe[i][j] = 0.0;	// �߽� �������� ������ ���� ��ħ ���� �Ÿ�
			m_nDirShoe[i][j]  = 3;		// ���� ��ħ ����
			m_dKh[i][j]	= 0;			// ź����ħ�� ����� ���� ���������
			m_dKv[i][j]	= 0;			// ź����ħ�� ����� ���� ���������
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
		m_dLengthUgan[i] = Obj.m_dLengthUgan[i];				// ���� �Ÿ�
		m_dLengthSlabToShoe[i] = Obj.m_dLengthSlabToShoe[i];	// ������ ���� ��ħ �Ÿ�
	}
	m_vAngle = Obj.m_vAngle;				// ���� ��ħ ����
	m_dStaJijum	= Obj.m_dStaJijum;			// ���� �����̼�

	for(i=0; i<2; i++)
	{
		m_nQtyShoe[i] = Obj.m_nQtyShoe[i];
		m_strJijumShoe[i] =Obj.m_strJijumShoe[i];						// ���� ��ħ ���� �̸�
		int j=0; for(j=0; j<MAX_SHOE_COUNT_RC; j++)
		{
			m_dDistShoe[i][j] = Obj.m_dDistShoe[i][j];					// �߽� �������� ������ ���� ��ħ ���� �Ÿ�
			m_nDirShoe[i][j] = Obj.m_nDirShoe[i][j];					// ���� ��ħ ����
			m_dKh[i][j]	= Obj.m_dKh[i][j];			// ź����ħ�� ����� ���� ���������
			m_dKv[i][j]	= Obj.m_dKv[i][j];			// ź����ħ�� ����� ���� ���������
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
			ar << m_dLengthUgan[i];				// ���� �Ÿ�
			ar << m_dLengthSlabToShoe[i];		// ������ ���� ��ħ �Ÿ�
		}
		ar << m_vAngle;							// ���� ��ħ ����
		ar << m_dStaJijum;						// ���� �����̼�

		for(i=0; i<2; i++)
		{
			ar << m_nQtyShoe[i];
			ar << m_strJijumShoe[i];			// ���� ��ħ ���� �̸�
			// ���Լ��� MAX_SHOE_COUNT_RC_OLD�� �����ϸ� �ȵȴ�.
			// ���ʿ� CJijumData�� flag�� ��� MAX_SHOE_COUNT_RC_OLD�� ���õ� ������ �̺κ��� �״�� �д�
			int j=0; for(j=0; j<MAX_SHOE_COUNT_RC_OLD; j++)
			{
				ar << m_dDistShoe[i][j];		// �߽� �������� ������ ���� ��ħ ���� �Ÿ�
				ar << m_nDirShoe[i][j];			// ���� ��ħ ����
			}
		}
	}
	else
	{
		int i=0; for(i=0; i<2; i++)
		{
			ar >> m_dLengthUgan[i];				// ���� �Ÿ�
			ar >> m_dLengthSlabToShoe[i];		// ������ ���� ��ħ �Ÿ�
		}
		ar >> m_vAngle;							// ���� ��ħ ����
		ar >> m_dStaJijum;						// ���� �����̼�

		for(i=0; i<2; i++)
		{
			ar >> m_nQtyShoe[i];
			ar >> m_strJijumShoe[i];			// ���� ��ħ ���� �̸�
			// ���Լ��� MAX_SHOE_COUNT_RC_OLD�� �����ϸ� �ȵȴ�.
			// ���ʿ� CJijumData�� flag�� ��� MAX_SHOE_COUNT_RC_OLD�� ���õ� ������ �̺κ��� �״�� �д�
			int j=0; for(j=0; j<MAX_SHOE_COUNT_RC_OLD; j++)
			{
				ar >> m_dDistShoe[i][j];		// �߽� �������� ������ ���� ��ħ ���� �Ÿ�
				ar >> m_nDirShoe[i][j];			// ���� ��ħ ����
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

// ������ Serialize�� flag�� ��� �ε����ϰ� flag�ִ� ������ ���� ����. �Ф�
// SerializeNew�� �ø����� �ű�鼭 MAX_SHOE_COUNT_RC 20 -> 30���� �����ϰ�
// ������ CRcBridgeData���� ���� �о���� �����ߴ� Kh, Kv���� ���⼭ �����ϰ� �о��.
// ������ ���ο� �Լ��� ������ Flag�� ���������� �ʾ���.
// �� �Լ��� ������ flag�� RcBridgeData���� ������ ����
void CJijum::SerializeNew(CArchive &ar)
{
	// 1 : MAX_SHOE_COUNT_RC : 30 -> 40
	// 2 : ���ƿ� �߰� (  BOX_TYPE )
	m_nFlag	= 2;

	if(ar.IsStoring())
	{
		ar << m_nFlag;

		int i=0; for(i=0; i<2; i++)
		{
			ar << m_dLengthUgan[i];				// ���� �Ÿ�
			ar << m_dLengthSlabToShoe[i];		// ������ ���� ��ħ �Ÿ�
		}
		ar << m_vAngle;							// ���� ��ħ ����
		ar << m_dStaJijum;						// ���� �����̼�

		for(i=0; i<2; i++)
		{
			ar << m_nQtyShoe[i];
			ar << m_strJijumShoe[i];			// ���� ��ħ ���� �̸�
			int j=0; for(j=0; j<MAX_SHOE_COUNT_RC; j++)
			{
				ar << m_dDistShoe[i][j];		// �߽� �������� ������ ���� ��ħ ���� �Ÿ�
				ar << m_nDirShoe[i][j];			// ���� ��ħ ����
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
			ar >> m_dLengthUgan[i];				// ���� �Ÿ�
			ar >> m_dLengthSlabToShoe[i];		// ������ ���� ��ħ �Ÿ�
		}
		ar >> m_vAngle;							// ���� ��ħ ����
		ar >> m_dStaJijum;						// ���� �����̼�

		long nMaxShoeCount	= MAX_SHOE_COUNT_RC;
		if(m_nFlag < 1)
			nMaxShoeCount	= MAX_SHOE_COUNT_RC_20090716;

		for(i=0; i<2; i++)
		{
			ar >> m_nQtyShoe[i];
			ar >> m_strJijumShoe[i];			// ���� ��ħ ���� �̸�
			int j=0; for(j=0; j<nMaxShoeCount; j++)
			{
				ar >> m_dDistShoe[i][j];		// �߽� �������� ������ ���� ��ħ ���� �Ÿ�
				ar >> m_nDirShoe[i][j];			// ���� ��ħ ����
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
