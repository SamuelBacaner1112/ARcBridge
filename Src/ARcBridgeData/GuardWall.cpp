// GuardWall.cpp: implementation of the CGuardWallRC class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GuardWall.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGuardWallRC::CGuardWallRC()
{
	m_nTypeGuardWall = 0;
	m_nNumberHDan = 0;
	m_bRailRoad = FALSE;
	m_bLeftGuardFence = TRUE;
	m_bSoundProof = FALSE;
	m_dDistFrLeft = 0;
	m_dW1 = 0;
	m_dW2 = 0;
	m_dW3 = 0;
	m_dW4 = 0;
	m_dW5 = 0;
	m_dH1 = 0;
	m_dH2 = 0;
	m_dH3 = 72;
	m_dR1 = 0;
	m_dR2 = 0;
	m_dR3 = 0;
	m_dC  = 0;	// ��� �����
	m_dW6 = 0;	// ������
	m_dW7 = 0;	// ������
	m_dH4 = 0;	// ������
	m_dH5 = 0;	// ������

	//Rebar
	m_dHR  = 0;
	m_dVR  = 0;
	m_dSR  = 0;
	m_dEC  = 0;
	m_ddc1 = 0;
	m_ddc2 = 0;
	m_ddc3 = 0;
	m_dF1  = 0;
	m_dF2  = 0;
	m_dF3  = 0;
	m_dE   = 0;
	m_dEH  = 0;
	m_dF   = 0;
	m_dFH  = 0;
	m_dL1  = 0;
	m_dCTC = 0;
	m_dWidhtHDan   = 0;
	m_dCrossF2 = 0;
	m_dCrossF3 = 0;
	m_dForce = 0;
	m_nUGugan = 0;
	m_nSGugan = 0;
	long i=0; for(i=0; i<GW_REBAR_GUGAN; i++)
	{
		m_nU[i] = 0;
		m_nS[i] = 0;
		m_dU[i] = 0;
		m_dS[i] = 0;
	}

	m_nTypePave				 = 0;
	m_nTypeInnerFill		 = 0;
	m_dUnitWeightPave		 = 1000;
	m_dUnitWeightInnerFill	 = 1000;
	m_dThickPaveFootway		 = 20;
	m_dSlopeUpper			 = -0.02;

	m_dFck = 0;
	m_dFy  = 0;
	m_dAggregate	= 19;				// ���� �ִ� ġ��

	m_bGWhalf = FALSE;
	m_nTypeRebar	= 0;				// ��� Ÿ�� (�⺻ : 0, ��ȣ���� ���ؼ��� �ٸ� Ÿ�� ����� ������ : 1)
	m_bDWload		= FALSE;

	m_sArrTypePave.RemoveAll();
	m_sArrTypePave.Add("������");
	m_sArrTypePave.Add("��ũ��Ʈ");
	m_sArrTypePave.Add("�ƽ���");

	m_sArrInnerFill.RemoveAll();
	m_sArrInnerFill.Add("��ũ��Ʈ");
	m_sArrInnerFill.Add("��Ż");
	m_sArrInnerFill.Add("��");
	
	for(i=0; i<GW_REBAR_COUNT; i++)
		m_pArrRebar.Add(new CRebar);
}

CGuardWallRC::~CGuardWallRC()
{
	AhTPADelete(&m_pArrRebar, (CRebar*)0);
	AhTPADelete(&m_pArrAreaConc, (CRebar*)0);
}

void CGuardWallRC::Serialize(CArchive& ar)
{
	// 15 : m_bDWload

	long nFlag = 15;
	if(ar.IsStoring())
	{
		ar << nFlag;

		ar << m_nTypeGuardWall;
		ar << m_nNumberHDan;
		ar << m_bRailRoad;
		ar << m_bLeftGuardFence;
		ar << m_dDistFrLeft;
		ar << m_ZzxyGW;
		ar << m_bSoundProof;
		ar << m_dW1;
		ar << m_dW2;
		ar << m_dW3;
		ar << m_dW4;
		ar << m_dW5;
		ar << m_dH1;
		ar << m_dH2;
		ar << m_dH3;
		ar << m_dR1;
		ar << m_dR2;
		ar << m_dR3;
		ar << m_dC;		// ��� �����
		ar << m_dW6;	// ������
		ar << m_dW7;	// ������
		ar << m_dH4;	// ������
		ar << m_dH5;	// ������

		// rebar
		ar << m_dHR;
		ar << m_dVR;
		ar << m_dSR;
		ar << m_dEC;
		ar << m_ddc1;
		ar << m_ddc2;
		ar << m_ddc3;
		ar << m_dF1;
		ar << m_dF2;
		ar << m_dF3;
		ar << m_dE;
		ar << m_dEH;
		ar << m_dF;
		ar << m_dFH;
		ar << m_dL1;
		ar << m_dCTC;
		ar << m_dSC;
		ar << m_dWidhtHDan;
		ar << m_dCrossF2;
		ar << m_dCrossF3;
		ar << m_dForce;
		ar << m_nTypePave;
		ar << m_nTypeInnerFill;
		ar << m_dUnitWeightPave;
		ar << m_dUnitWeightInnerFill;
		ar << m_dThickPaveFootway;
		ar << m_dSlopeUpper;
		ar << m_nUGugan;
		ar << m_nSGugan;
		long i=0; for(i=0; i<GW_REBAR_GUGAN; i++)
		{
			ar << m_nU[i];
			ar << m_nS[i];
			ar << m_dU[i];
			ar << m_dS[i];
		}
		AhTPASerial(ar, &m_pArrRebar, (CRebar*)0);
		ar << m_dFck;
		ar << m_dFy;
		ar << m_dAggregate;				// ���� �ִ� ġ��
		ar << m_bGWhalf;
		ar << m_nTypeRebar;				// ��� Ÿ�� (�⺻ : 0, ��ȣ���� ���ؼ��� �ٸ� Ÿ�� ����� ������ : 1)
		ar << m_bDWload;
	}
	else
	{
		ar >> nFlag;

		ar >> m_nTypeGuardWall;
		
		if(nFlag > 1)
		{
			ar >> m_nNumberHDan;
			ar >> m_bRailRoad;
		}
		
		if(nFlag > 2)
			ar >> m_bLeftGuardFence;
		
		ar >> m_dDistFrLeft;
		ar >> m_ZzxyGW;
		if(nFlag>10)
			ar >> m_bSoundProof;
		ar >> m_dW1;
		ar >> m_dW2;
		ar >> m_dW3;
		ar >> m_dW4;
		ar >> m_dW5;
		ar >> m_dH1;
		ar >> m_dH2;
		ar >> m_dH3;
		ar >> m_dR1;
		ar >> m_dR2;
		ar >> m_dR3;
		ar >> m_dC;	// ��� �����
		
		if(nFlag > 3)
		{
			ar >> m_dW6;	// ������
			ar >> m_dW7;	// ������
			ar >> m_dH4;	// ������
			ar >> m_dH5;	// ������
		}
		
		// Rebar_
		ar >> m_dHR;
		ar >> m_dVR;
		ar >> m_dSR;
		ar >> m_dEC;
		ar >> m_ddc1;
		ar >> m_ddc2;
		ar >> m_ddc3;
		ar >> m_dF1;
		ar >> m_dF2;
		ar >> m_dF3;
		ar >> m_dE;

		if(nFlag > 8)
			ar >> m_dEH;
		else
			m_dEH = 6;
		
		ar >> m_dF;
		if(nFlag > 8)
			ar >> m_dFH;
		else
			m_dFH = 0;
		ar >> m_dL1;
		ar >> m_dCTC;
		
		if(nFlag > 6)
			ar >> m_dSC;
		else
		{
			m_dE  = 6;
			m_dSC = 2;
		}
		
		ar >> m_dWidhtHDan;
		ar >> m_dCrossF2;
		ar >> m_dCrossF3;

		if(nFlag > 0)
			ar >> m_dForce;
		
		if(nFlag > 5)
		{
			ar >> m_nTypePave;
			ar >> m_nTypeInnerFill;
			ar >> m_dUnitWeightPave;
			ar >> m_dUnitWeightInnerFill;
			ar >> m_dThickPaveFootway;
			ar >> m_dSlopeUpper;
		}	

		if(nFlag > 7)
		{
			ar >> m_nUGugan;
			ar >> m_nSGugan;

			long i=0; for(i=0; i<GW_REBAR_GUGAN; i++)
			{
				ar >> m_nU[i];
				ar >> m_nS[i];
				ar >> m_dU[i];
				ar >> m_dS[i];
			}
		}
		else
		{
			m_nUGugan = 0;
			m_nSGugan = 0;

			long i=0; for(i=0; i<GW_REBAR_GUGAN; i++)
			{
				m_nU[i] = 0;
				m_nS[i] = 0;
				m_dU[i] = 0;
				m_dS[i] = 0;
			}
		}

		if(nFlag < 5 && !m_bRailRoad)		//���α��� ���, Ⱦ��Ÿ�� �¿� ������
		{
			if(m_nTypeGuardWall == 2)		//��ȣ��(��) �� ���
				m_nTypeGuardWall = HDAN_TYPE_ROAD_GUARDFENCE;
			else if(m_nTypeGuardWall == 3)	//�ߺд� �� ���
				m_nTypeGuardWall = HDAN_TYPE_ROAD_CENTERGUARD;
			else if(m_nTypeGuardWall == 4)	//�ߺд�1/2(��) �� ���
				m_nTypeGuardWall = HDAN_TYPE_ROAD_CENTERGUARDHALF;
			else if(m_nTypeGuardWall == 5)	//�ߺд�1/2(��) �� ���
				m_nTypeGuardWall = HDAN_TYPE_ROAD_CENTERGUARDHALF;
			else if(m_nTypeGuardWall == 6)	//���� �� ���
				m_nTypeGuardWall = HDAN_TYPE_ROAD_FOOTWAY;
			else if(m_nTypeGuardWall == 7)	//������ ��輮 �� ���
				m_nTypeGuardWall = HDAN_TYPE_ROAD_FOOTWAYBLOCK;
			else if(m_nTypeGuardWall == 8)	//������ ���� �� ���
				m_nTypeGuardWall = HDAN_TYPE_ROAD_SOUNDPROOF;
			else if(m_nTypeGuardWall == 9)	//�븷�̺� �� ���
				m_nTypeGuardWall = HDAN_TYPE_ROAD_RETAININGWALL;
			else if(m_nTypeGuardWall == 10)	//�������� �� ���
				m_nTypeGuardWall = HDAN_TYPE_ROAD_HANDRAIL;
		}
		if(nFlag < 6 && m_nTypeGuardWall == HDAN_TYPE_ROAD_FOOTWAY)//���� �� ���
			m_dH3 = 72;		
		
		AhTPASerial(ar, &m_pArrRebar, (CRebar*)0);
		long nSizeArr = m_pArrRebar.GetSize();
		long i=0; for(i=0; i<GW_REBAR_COUNT-nSizeArr; i++)
		{
			CRebar *pRB = new CRebar;
			m_pArrRebar.Add(pRB);
		}
		
		if(nFlag > 9)
		{
			ar >> m_dFck;
			ar >> m_dFy;
		}
		if(nFlag > 12)
			ar >> m_dAggregate;				// ���� �ִ� ġ��
		if(nFlag > 11)
			ar >> m_bGWhalf;
		if(nFlag > 13)
			ar >> m_nTypeRebar;				// ��� Ÿ�� (�⺻ : 0, ��ȣ���� ���ؼ��� �ٸ� Ÿ�� ����� ������ : 1)
		if(nFlag > 14)
			ar >> m_bDWload;
	}
}

CGuardWallRC& CGuardWallRC::operator=(const CGuardWallRC& Obj)
{
	m_nTypeGuardWall = Obj.m_nTypeGuardWall;
	m_nNumberHDan = Obj.m_nNumberHDan;
	m_bRailRoad = Obj.m_bRailRoad;
	m_bLeftGuardFence = Obj.m_bLeftGuardFence;
	m_dDistFrLeft = Obj.m_dDistFrLeft;
	m_ZzxyGW = Obj.m_ZzxyGW;
	m_bSoundProof = Obj.m_bSoundProof;

	m_dFck = Obj.m_dFck;
	m_dFy  = Obj.m_dFy;
	m_dAggregate	= Obj.m_dAggregate;				// ���� �ִ� ġ��

	m_bGWhalf  = Obj.m_bGWhalf;
	m_nTypeRebar	= Obj.m_nTypeRebar;				// ��� Ÿ�� (�⺻ : 0, ��ȣ���� ���ؼ��� �ٸ� Ÿ�� ����� ������ : 1)

	m_dW1 = Obj.m_dW1;
	m_dW2 = Obj.m_dW2;
	m_dW3 = Obj.m_dW3;
	m_dW4 = Obj.m_dW4;
	m_dW5 = Obj.m_dW5;

	m_dH1 = Obj.m_dH1;
	m_dH2 = Obj.m_dH2;
	m_dH3 = Obj.m_dH3;

	m_dR1 = Obj.m_dR1;
	m_dR2 = Obj.m_dR2;
	m_dR3 = Obj.m_dR3;
	m_dC  = Obj.m_dC;	// ��� �����
	m_dW6 = Obj.m_dW6;	// ������
	m_dW7 = Obj.m_dW7;	// ������
	m_dH4 = Obj.m_dH4;	// ������
	m_dH5 = Obj.m_dH5;	// ������

	// rebar
	m_dHR  = Obj.m_dHR;
	m_dVR  = Obj.m_dVR;
	m_dSR  = Obj.m_dSR;
	m_dEC  = Obj.m_dEC;
	m_ddc1 = Obj.m_ddc1;
	m_ddc2 = Obj.m_ddc2;
	m_ddc3 = Obj.m_ddc3;
	m_dF1  = Obj.m_dF1;
	m_dF2  = Obj.m_dF2;
	m_dF3  = Obj.m_dF3;
	m_dE   = Obj.m_dE;
	m_dF   = Obj.m_dF;
	m_dL1  = Obj.m_dL1;
	m_dCTC = Obj.m_dCTC;
	m_dSC  = Obj.m_dSC;
	m_dWidhtHDan   = Obj.m_dWidhtHDan;
	m_dCrossF2 = Obj.m_dCrossF2;
	m_dCrossF3 = Obj.m_dCrossF3;
	m_dForce = Obj.m_dForce;
	m_nTypePave				 = Obj.m_nTypePave;
	m_nTypeInnerFill		 = Obj.m_nTypeInnerFill;
	m_dUnitWeightPave		 = Obj.m_dUnitWeightPave;
	m_dUnitWeightInnerFill	 = Obj.m_dUnitWeightInnerFill;
	m_dThickPaveFootway		 = Obj.m_dThickPaveFootway;
	m_dSlopeUpper			 = Obj.m_dSlopeUpper;
	AhTPACopy(&m_pArrRebar, &Obj.m_pArrRebar, (CRebar*)0);	// ö������
	AhTPACopy(&m_pArrAreaConc, &Obj.m_pArrAreaConc, (CTwinVectorAreaShape*)0);	// ��ũ��Ʈ ����

	m_bDWload				= Obj.m_bDWload;

	return *this;
}

// ��ȣ�� ö�� ��
BOOL CGuardWallRC::operator==(const CGuardWallRC& rhs)
{
	BOOL bCheck = FALSE;

	if(   ( this->m_dW1 == rhs.m_dW1 &&	// ���� ����
		    this->m_dW2 == rhs.m_dW2 &&
		    this->m_dW3 == rhs.m_dW3 &&
			this->m_dW4 == rhs.m_dW4 &&
		    this->m_dW5 == rhs.m_dW5 )	||	(	this->m_dW1 == rhs.m_dW5 &&	// �ٸ� ����
												this->m_dW2 == rhs.m_dW4 &&
												this->m_dW3 == rhs.m_dW3 &&
												this->m_dW4 == rhs.m_dW2 &&
												this->m_dW5 == rhs.m_dW1	 )
	  )
	  bCheck = TRUE;
	
	if(this->m_dW6 > 0)
	{
		if(bCheck && this->m_dH4 == rhs.m_dH4
			  && this->m_dH5 == rhs.m_dH5
			  && this->m_dW7 == rhs.m_dW7
			  && ( this->m_dW1+this->m_dW2+this->m_dW3+this->m_dW4+this->m_dW5-(this->m_dW6+this->m_dW7) == rhs.m_dW6 
			      ||  this->m_dW6 == rhs.m_dW1+rhs.m_dW2+rhs.m_dW3+rhs.m_dW4+rhs.m_dW5-(rhs.m_dW6+rhs.m_dW7) )
		 )	  bCheck = TRUE;
		else
			 bCheck = FALSE;
	}

	if(bCheck && (this->m_dH1+this->m_dH2+this->m_dH3) == (rhs.m_dH1+rhs.m_dH2+rhs.m_dH3)
			  && (long)this->m_dHR  == (long)rhs.m_dHR
			  && (long)this->m_dVR  == (long)rhs.m_dVR
			  && (long)this->m_dSR  == (long)rhs.m_dSR
			  && Round(this->m_dEC,5)  == Round(rhs.m_dEC,5)
			  && Round(this->m_dF1,5)  == Round(rhs.m_dF1,5)
			  && Round(this->m_dF2,5)  == Round(rhs.m_dF2,5)
			  && Round(this->m_dF3,5)  == Round(rhs.m_dF3,5)
			  && Round(this->m_dE,5)   == Round(rhs.m_dE,5)
			  && Round(this->m_dEH,5)  == Round(rhs.m_dEH,5)
			  && Round(this->m_dF,5)   == Round(rhs.m_dF,5)
			  && Round(this->m_dFH,5)  == Round(rhs.m_dFH,5)
			  && Round(this->m_dL1,5)  == Round(rhs.m_dL1,5)
			  && Round(this->m_dCTC,5) == Round(rhs.m_dCTC,5)
			  && Round(this->m_dSC,5)  == Round(rhs.m_dSC,5)
			  && this->m_dFck		   == rhs.m_dFck
			  && this->m_dFy		   == rhs.m_dFy
			  && this->m_dAggregate    == rhs.m_dAggregate				// ���� �ִ� ġ��
			  && this->m_bGWhalf	   == rhs.m_bGWhalf
			  && this->m_nTypeRebar    == rhs.m_nTypeRebar				// ��� Ÿ�� (�⺻ : 0, ��ȣ���� ���ؼ��� �ٸ� Ÿ�� ����� ������ : 1)
	  )	  bCheck = TRUE;
	else
		  bCheck = FALSE;

	if(this->m_nUGugan>0 && this->m_nSGugan>0)
	{
		long i=0; for(i=0; i<this->m_nUGugan; i++)
		{
			if(bCheck && this->m_nUGugan == rhs.m_nUGugan
					  && this->m_nU[i] == rhs.m_nU[i]
					  && this->m_dU[i] == rhs.m_dU[i]
			  )
			  bCheck = TRUE;
			else
			  bCheck = FALSE;
		}
		for(i=0; i<this->m_nSGugan; i++)
		{
			if(bCheck && this->m_nSGugan == rhs.m_nSGugan
					  && this->m_nS[i] == rhs.m_nS[i]
					  && this->m_dS[i] == rhs.m_dS[i]
			  )
			  bCheck = TRUE;
			else
			  bCheck = FALSE;
		}
	}
	return bCheck;
}

//<summary>
//��� : ��ȣ��, ����, ���� ���� ���� ��ǥ�� ����
//		 <��������>�� ���ٰ� ������. //�Է¿��� �������� ���� ���� �ʵ��� �� ��.
//		 ���ÿ� ���� ��ǥ�� ������ ��ǥ���� Ȯ���ϰ� ����� ��.
//</summary>
//<param name="A[9]">��ȯ ���� ���� ��ǥ��</param>
// void CGuardWallRC::GetXyGuardFenceBase(CDPoint A[9])
// {
// 	long n=0; for(n=0; n<9; n++){
// 		A[n].x = 0;
// 		A[n].y = 0;
// 	}
// 	if(IsTypeHDanRailFootwayBlock()) //����+����
// 	{
// 		if(m_dW4 >= m_dW1)
// 		{		
// 			A[0].x = 0;
// 			A[1].x = m_dDistFrLeft;
// 			A[2].x = A[1].x;
// 			A[3].x = m_dDistFrLeft + m_dW1;
// 			A[4].x = A[3].x;
// 			A[5].x = m_dDistFrLeft + m_dW1 + m_dW2;
// 			A[6].x = A[5].x;
// 			A[7].x = A[6].x;
// 			A[8].x = A[7].x;
// 
// 			A[0].y = 0;
// 			A[1].y = 0;
// 			A[2].y = m_dH1 + m_dH2;
// 			A[3].y = A[2].y;
// 			A[4].y = m_dH2;
// 			A[5].y = A[4].y;
// 			A[6].y = 0;
// 			A[7].y = 0;
// 			A[8].y = 0;
// 		}
// 		else
// 		{
// 			A[0].x = 0;
// 			A[1].x = m_dDistFrLeft;
// 			A[2].x = A[1].x;
// 			A[3].x = m_dDistFrLeft + m_dW1;
// 			A[4].x = A[3].x;
// 			A[5].x = m_dDistFrLeft + m_dW1 + m_dW2;
// 			A[6].x = A[5].x;
// 			A[7].x = A[6].x;
// 			A[8].x = A[7].x;
// 
// 			A[0].y = 0;
// 			A[1].y = 0;
// 			A[2].y = m_dH2;
// 			A[3].y = A[2].y;
// 			A[4].y = m_dH1 + m_dH2;
// 			A[5].y = A[4].y;
// 			A[6].y = 0;
// 			A[7].y = 0;
// 			A[8].y = 0;
// 		}
// 	}
// 	else if(IsTypeHDanRailTotalHole())	//��ű�, ��ű�+����
// 	{
// 		A[0].x = 0;
// 		A[1].x = m_dDistFrLeft;
// 		A[2].x = m_dDistFrLeft + m_dW1 + m_dW2 + m_dW3;
// 		A[3].x = m_dDistFrLeft + m_dW1 + m_dW2 + m_dW3;
// 		A[4].x = m_dDistFrLeft;
// 		A[5].x = 0;
// 		A[6].x = 0;
// 		A[7].x = 0;
// 		A[8].x = 0;
// 
// 		A[0].y = 0;
// 		A[1].y = 0;
// 		A[2].y = 0;
// 		A[3].y = m_dH1 + m_dH2 + m_dH3;
// 		A[4].y = m_dH1 + m_dH2 + m_dH3;
// 		A[5].y = 0;
// 		A[6].y = 0;
// 		A[7].y = 0;
// 		A[8].y = 0;
// 	}
// 	else
// 	{	
// 		A[0].x = 0;
// 		A[1].x = m_dDistFrLeft;
// 		A[2].x = A[1].x;
// 		A[3].x = m_dDistFrLeft + m_dW1;
// 		A[4].x = m_dDistFrLeft + m_dW1 + m_dW2;
// 		A[5].x = m_dDistFrLeft + m_dW1 + m_dW2 + m_dW3;
// 		A[6].x = m_dDistFrLeft + m_dW1 + m_dW2 + m_dW3 + m_dW4;
// 		A[7].x = m_dDistFrLeft + m_dW1 + m_dW2 + m_dW3 + m_dW4 + m_dW5;
// 		A[8].x = A[7].x;
// 
// 		A[0].y = 0;
// 		A[1].y = 0;
// 		A[2].y = m_dH1;
// 		A[3].y = m_dH1 + m_dH2;
// 		A[4].y = m_dH1 + m_dH2 + m_dH3;
// 		A[5].y = m_dH1 + m_dH2 + m_dH3;
// 		A[6].y = m_dH1 + m_dH2;
// 		A[7].y = m_dH1;
// 		A[8].y = 0;
// 	}
// }

//ö���� ��ű�, ��ű�+����
BOOL CGuardWallRC::IsTypeHDanRailTotalHole()
{
	if(IsTypeHDanRailHole() || IsTypeHDanRailHoleFootway())
		return TRUE;
	
	return FALSE;
}
//ö���� ��ű�
BOOL CGuardWallRC::IsTypeHDanRailHole()
{
	if(m_bRailRoad)
		if(m_nTypeGuardWall==HDAN_TYPE_RAIL_HOLE) return TRUE;
	
	return FALSE;
}
//ö���� ��ű�+����
BOOL CGuardWallRC::IsTypeHDanRailHoleFootway()
{
	if(m_bRailRoad)
		if(m_nTypeGuardWall==HDAN_TYPE_RAIL_HOLEFOOTWAY) return TRUE;
	
	return FALSE;
}
//<summary>
//	���α�		����ũ
//	ö����		����ũ
//	2005�� 6�� 7�� ����� <HDAN_TYPE_ROAD_DIKE>�� ����� <HDAN_TYPE_RAIL_DIKE>�� ������� �� ��
//</summary>
//<param name="nHDan"></param>
BOOL CGuardWallRC::IsTypeHDanRoadRailDike()
{
	if(m_nTypeGuardWall == HDAN_TYPE_ROAD_DIKE)			return TRUE;
	return FALSE;
}

//<summary>
//	���α�		����
//	ö����		����
//</summary>
//<param name="nHDan"></param>
BOOL CGuardWallRC::IsTypeHDanOnlyNone()
{
	if(m_bRailRoad && m_nTypeGuardWall == HDAN_TYPE_RAIL_NONE)			return TRUE;
	if(!m_bRailRoad && m_nTypeGuardWall == HDAN_TYPE_ROAD_NONE)			return TRUE;
	return FALSE;
}
//<summary>
//	���α�		��ȣ��
//</summary>
//<param name="nHDan"></param>
BOOL CGuardWallRC::IsTypeHDanRoadGuardFence()
{
	if(!m_bRailRoad && m_nTypeGuardWall == HDAN_TYPE_ROAD_GUARDFENCE)
		return TRUE;
	return FALSE;
}
//<summary>
//	���α�		�ߺд�(1/2�� �ƴ� ���)
//</summary>
//<param name="nHDan"></param>
BOOL CGuardWallRC::IsTypeHDanRoadCentralWallFull()
{
	if(!m_bRailRoad && m_nTypeGuardWall == HDAN_TYPE_ROAD_CENTERGUARD)
		return TRUE;
	return FALSE;
}
//<summary>
//	���α�		�ߺд�(1/2�� ���)
//</summary>
//<param name="nHDan"></param>
BOOL CGuardWallRC::IsTypeHDanRoadCentralWallHalf()
{
	if(!m_bRailRoad && m_nTypeGuardWall == HDAN_TYPE_ROAD_CENTERGUARDHALF)
		return TRUE;
	return FALSE;
}
//<summary>
//	���α�		����
//</summary>
BOOL CGuardWallRC::IsTypeHDanRoadFootway()
{
	if(!m_bRailRoad && m_nTypeGuardWall == HDAN_TYPE_ROAD_FOOTWAY)
		return TRUE;
	return FALSE;
}
//<summary>
//	���α�		������ ��輮
//</summary>
//<param name="nHDan"></param>
BOOL CGuardWallRC::IsTypeHDanRoadFootwayBlock()
{
	if(!m_bRailRoad && m_nTypeGuardWall == HDAN_TYPE_ROAD_FOOTWAYBLOCK)
		return TRUE;
	return FALSE;
}

//<summary>
//	���α�		������ ����
//</summary>
//<param name="nHDan"></param>
BOOL CGuardWallRC::IsTypeHDanRoadSoundProof()
{
	if(!m_bRailRoad && m_nTypeGuardWall == HDAN_TYPE_ROAD_SOUNDPROOF)
		return TRUE;
	return FALSE;
}
//<summary>
//	���α�		�븷�̺�
//</summary>
//<param name="nHDan"></param>
BOOL CGuardWallRC::IsTypeHDanRoadRetainingWall()
{
	if(!m_bRailRoad && m_nTypeGuardWall == HDAN_TYPE_ROAD_RETAININGWALL)
		return TRUE;
	return FALSE;
}
//<summary>
//	���α�		��������
//</summary>
//<param name="nHDan"></param>
BOOL CGuardWallRC::IsTypeHDanRoadHandRail()
{
	if(!m_bRailRoad && m_nTypeGuardWall == HDAN_TYPE_ROAD_HANDRAIL)
		return TRUE;
	return FALSE;
}
//<summary>
//	���α�		����ũ
//</summary>
//<param name="nHDan"></param>
BOOL CGuardWallRC::IsTypeHDanRoadDike()
{
	if(!m_bRailRoad && m_nTypeGuardWall == HDAN_TYPE_ROAD_DIKE)
		return TRUE;
	return FALSE;
}
//<summary>
//	���α�		��ȣ��, �ߺд�, �ߺд�1/2, ö������ ��� ��� TRUE
//</summary>
//<param name="nHDan"></param>
BOOL CGuardWallRC::IsTypeHDanRoadTotalGuardFenceAndCentralWall()
{
	if(IsTypeHDanRoadGuardFence() || IsTypeHDanRoadCentralWallFull() || IsTypeHDanRoadCentralWallHalf())
		return TRUE;
	return FALSE;
}
//<summary>
//	���α�		�ߺд�, �ߺд�1/2, ö������ ��� ��� TRUE
//</summary>
//<param name="nHDan"></param>
BOOL CGuardWallRC::IsTypeHDanRoadTotalCentralWall()
{
	if(IsTypeHDanRoadCentralWallFull() || IsTypeHDanRoadCentralWallHalf())
		return TRUE;
	return FALSE;
}
//<summary>
//	ö����		����
//</summary>
//<param name="nHDan"></param>
BOOL CGuardWallRC::IsTypeHDanRailGuardFence()
{
	if(m_bRailRoad && m_nTypeGuardWall == HDAN_TYPE_RAIL_GUARDFENCE)
		return TRUE;
	return FALSE;
}
//<summary>
//	ö���� 	�ڰ�����(ö��)
//</summary>
//<param name="nHDan"></param>
BOOL CGuardWallRC::IsTypeHDanRailDetectGravel()
{
	if(m_bRailRoad && m_nTypeGuardWall == HDAN_TYPE_RAIL_DETECTGRAVEL)
		return TRUE;
	return FALSE;
}
//<summary>
//	ö���� 	�ڰ�����(����)
//</summary>
//<param name="nHDan"></param>
BOOL CGuardWallRC::IsTypeHDanRailDetectGravel2()
{
	if(m_bRailRoad && m_nTypeGuardWall == HDAN_TYPE_RAIL_DETECTGRAVEL2)
		return TRUE;
	return FALSE;
}
//<summary>
//	ö����		��������
//</summary>
//<param name="nHDan"></param>
BOOL CGuardWallRC::IsTypeHDanRailFootway()
{
	if(m_bRailRoad && m_nTypeGuardWall == HDAN_TYPE_RAIL_FOOTWAY)
		return TRUE;
	return FALSE;
}
//<summary>
//	ö����		����+����
//</summary>
//<param name="nHDan"></param>
BOOL CGuardWallRC::IsTypeHDanRailFootwayBlock()
{
	if(m_bRailRoad && m_nTypeGuardWall == HDAN_TYPE_RAIL_FOOTWAYBLOCK)
		return TRUE;
	return FALSE;
}
//<summary>
//	ö����		����ĵ���
//</summary>
//<param name=""></param>
BOOL CGuardWallRC::IsTypeHDanRailDosangVer()
{
	if(m_bRailRoad && m_nTypeGuardWall == HDAN_TYPE_RAIL_DOSANGVER)
		return TRUE;
	return FALSE;
}
//<summary>
//	ö����		�ڰ�����
//</summary>
//<param name=""></param>
BOOL CGuardWallRC::IsTypeHDanRailDosangGravel()
{
	if(m_bRailRoad && m_nTypeGuardWall == HDAN_TYPE_RAIL_DOSANGGRAVEL)
		return TRUE;
	return FALSE;
}
//<summary>
//	ö����		����ĵ���	�ڰ�����
//</summary>
//<param name=""></param>
BOOL CGuardWallRC::IsTypeHDanRailTotalDosang()
{
	if(IsTypeHDanRailDosangVer() || IsTypeHDanRailDosangGravel())
		return TRUE;
	return FALSE;
}
//<summary>
// ���� nHDan �� ��ȣ�� Ÿ������ ����
//	���α�  ����, ���� �� ������ ��� ��, ö������ ��� ��� TRUE
//		��ȣ�� �� �ߺд�
//		������ ��輮
//		������ ����
//		�븷�̺�
//		��������
//</summary>
BOOL CGuardWallRC::IsTypeHDanRoadTotalGuardFence()
{
	if(IsTypeHDanRoadFootway() || IsTypeHDanOnlyNone())
		return FALSE;
	return TRUE;
}
//<summary>
// ���� nHDan �� ��ȣ�� Ÿ������ ����
//	���α� ���� �� ������ ��� ��, ö������ ��� ��� TRUE
//		��ȣ�� �� �ߺд�
//		������ ��輮
//		������ ����
//		�븷�̺�
//		��������
//</summary>
BOOL CGuardWallRC::IsTypeHDanRoadTotalGuardWall()
{
	if(IsTypeHDanOnlyNone())
		return FALSE;
	return TRUE;
}
//<summary>
//</summary>
//<param name=""></param>
BOOL CGuardWallRC::IsLeftGuardFence()
{
	return m_bLeftGuardFence;
}

//<summary>
//</summary>
//<param name="bUnderGroundRahmen">TRUE=���� ������ ���߶���̰� �������(����)�� ���(���θ� ��ȣ������ �������� ����)</param>
void CGuardWallRC::SetDataDefaultGuardWall(double thickPave, BOOL bUnderGroundRahmen)
{
	if(m_bRailRoad)
		SetDataDefaultGuardWallRail(thickPave, bUnderGroundRahmen);
	else
		SetDataDefaultGuardWallDoro(thickPave, bUnderGroundRahmen);
}
//<summary>
//</summary>
//<param name="bUnderGroundRahmen">TRUE=���� ������ ���߶���̰� �������(����)�� ���(���θ� ��ȣ������ �������� ����)</param>
void CGuardWallRC::SetDataDefaultGuardWallDoro(double thickPave, BOOL bUnderGroundRahmen)
{
	if(IsLeftGuardFence() && IsTypeHDanRoadGuardFence())	// ���� ��ȣ��
	{
		m_dDistFrLeft = 30;
		m_dW1 = 0;
		m_dW2 = 0;
		m_dW3 = 230;
		m_dW4 = bUnderGroundRahmen ? 190 : 70;
		m_dW5 = bUnderGroundRahmen ? 0 : 120;

		m_dH1 = bUnderGroundRahmen ? 0 : 125 + thickPave;
		m_dH2 = bUnderGroundRahmen ? 0 : 175;
		m_dH3 = bUnderGroundRahmen ? 1000 : 970;
		m_dR1 = 0;
		m_dR2 = 0;
		m_dR3 = 0;
		m_dC  = 0;
		m_dForce  = 0;
	}
	else if(!IsLeftGuardFence() && IsTypeHDanRoadGuardFence())	// ���� ��ȣ��
	{
		m_dDistFrLeft = 0;
		m_dW1 = bUnderGroundRahmen ? 0 : 120;
		m_dW2 = bUnderGroundRahmen ? 190 : 70;
		m_dW3 = 230;
		m_dW4 = 0;
		m_dW5 = 0;

		m_dH1 = bUnderGroundRahmen ? 0 : 125 + thickPave;
		m_dH2 = bUnderGroundRahmen ? 0 : 175;
		m_dH3 = bUnderGroundRahmen ? 1000 : 970;
		m_dR1 = 0;
		m_dR2 = 0;
		m_dR3 = 0;
		m_dC  = 0;
		m_dForce  = 0;
	}
	else if(IsTypeHDanRoadCentralWallFull())	// �ߺд�
	{
		m_dDistFrLeft = 0;
		m_dW1 = 125;
		m_dW2 = 105;
		m_dW3 = 150;
		m_dW4 = 105;
		m_dW5 = 125;
		m_dH1 = 75 + thickPave;
		m_dH2 = 175;
		m_dH3 = 1020;
		m_dR1 = 100;
		m_dR2 = 260;
		m_dR3 = 26;
		m_dC  = 30;
		m_dForce  = 0;
	}
	else if(IsTypeHDanRoadCentralWallHalf())	// �ߺд�1/2
	{
		m_dDistFrLeft = 0;
		m_dW1 = IsLeftGuardFence() ? 0 : 125;
		m_dW2 = IsLeftGuardFence() ? 0 : 85;
		m_dW3 = 110;
		m_dW4 = IsLeftGuardFence() ? 85 : 0;
		m_dW5 = IsLeftGuardFence() ? 125 : 0;

		m_dH1 = 75 + thickPave;
		m_dH2 = 175;
		m_dH3 = 1020;
		m_dR1 = 0;
		m_dR2 = 0;
		m_dR3 = 0;
		m_dC  = 0;
		m_dForce  = 0;
	
	}
	else if(IsTypeHDanRoadFootwayBlock())	// ������ ��輮
	{
		m_dDistFrLeft = 0;
		m_dH1 = 0;
		m_dH2 = 0;
		m_dH3 = 12 + thickPave;
		m_dW1 = 0;
		m_dW2 = 0;
		m_dW3 = 180;
		m_dW4 = 0;
		m_dW5 = 0;
		m_dR1 = 0;
		m_dR2 = 0;
		m_dR3 = 0;
		m_dC  = 0;
		m_dForce  = 0;
	}
	else if(IsTypeHDanRoadFootway())	// ����
	{
		m_dDistFrLeft = 0;
		m_dW1 = 0;
		m_dW2 = 0;
		m_dW3 = 300;
		m_dW4 = 0;
		m_dW5 = 0;

		m_dH1 = 0;
		m_dH2 = 0;
		m_dH3 = 77;
		m_dR1 = 0;
		m_dR2 = 0;
		m_dR3 = 0;
		m_dC  = 0;
		m_dForce  = 0;

		m_nTypePave				 = 0;
		m_nTypeInnerFill		 = 2;
		m_dUnitWeightPave		 = 2300;// tonf/m��
		m_dUnitWeightInnerFill	 = 2000;// tonf/m��
		m_dThickPaveFootway		 = 20;
		m_dSlopeUpper			 = m_bLeftGuardFence ? -0.02 : 0.02;
	}
	else if(IsTypeHDanRoadSoundProof())	// ������ ���� Sound Proof
	{
		m_dDistFrLeft = IsLeftGuardFence() ? 30 : 0;
		m_dW1 = 0;
		m_dW2 = 0;
		m_dW3 = 400;
		m_dW4 = 0;
		m_dW5 = 0;

		m_dH1 = 0;
		m_dH2 = 0;
		m_dH3 = 720 + thickPave;
		m_dR1 = 0;
		m_dR2 = 0;
		m_dR3 = 0;
		m_dC  = 0;
		m_dForce  = 0.1;
	}
	else if(IsTypeHDanRoadRetainingWall())	// �븷�̺� Retaining Wall
	{
		m_dDistFrLeft = IsLeftGuardFence() ? 30 : 0;
		m_dW1 = IsLeftGuardFence() ? 0 : 1200;
		m_dW2 = 0;
		m_dW3 = 400;
		m_dW4 = 0;
		m_dW5 = IsLeftGuardFence() ? 1100 : 0;

		m_dH1 = 220 + thickPave;
		m_dH2 = 0;
		m_dH3 = 1700;
		m_dR1 = 0;
		m_dR2 = 0;
		m_dR3 = 0;
		m_dC  = 0;
		m_dForce  = 0;
	}
	else if(IsTypeHDanRoadHandRail())	// �������� Handrail
	{
		m_dDistFrLeft = IsLeftGuardFence() ? 30 : 0;
		m_dW1 = IsLeftGuardFence() ? 0 : 120;
		m_dW2 = 0;
		m_dW3 = 300;
		m_dW4 = 0;
		m_dW5 = IsLeftGuardFence() ? 120 : 0;

		m_dH1 = 125 + thickPave;
		m_dH2 = 175;
		m_dH3 = 270;
		m_dR1 = 0;
		m_dR2 = 0;
		m_dR3 = 0;
		m_dC  = 0;
		m_dForce  = 0.1;
	}
	else if(IsTypeHDanRoadRailDike())	// ����, ö�� ����ũ
	{
		m_dDistFrLeft = 0;
		m_dW1 = 0;
		m_dW2 = IsLeftGuardFence() ? 0 : 135;
		m_dW3 = 115;
		m_dW4 = IsLeftGuardFence() ? 135 : 0;
		m_dW5 = 0;

		m_dH1 = 0;
		m_dH2 = 0;
		m_dH3 = 400 + thickPave+150;
		m_dR1 = 0;
		m_dR2 = 0;
		m_dR3 = 20;
		m_dC  = 0;
		m_dForce  = 0.1;
	}
}
//<summary>
//</summary>
//<param name="bUnderGroundRahmen">TRUE=���� ������ ���߶���̰� �������(����)�� ���(���θ� ��ȣ������ �������� ����)</param>
void CGuardWallRC::SetDataDefaultGuardWallRail(double thickPave, BOOL bUnderGroundRahmen)
{
	if(IsTypeHDanRailGuardFence())		//����
	{
		m_dDistFrLeft = 0;
		m_dW1 = IsLeftGuardFence() ? 0 : 50;
		m_dW2 = 0;
		m_dW3 = m_dWidhtHDan - 50;
		m_dW4 = 0;
		m_dW5 = IsLeftGuardFence() ? 50 : 0;
		
		m_dH1 = 600;
		m_dH2 = 0;
		m_dH3 = 300;	
		m_dR1 = 0;
		m_dR2 = 0;
		m_dR3 = 0;
		m_dC  = 0;
		m_dW6  = 0;
		m_dW7  = 0;
		m_dH4  = 0;
		m_dH5  = 0;
		m_dForce  = 0;
	}
	else if(IsTypeHDanRailDetectGravel())	//�ڰ�����
	{
		m_dDistFrLeft = 0;
		m_dW1 = 0;
		m_dW2 = IsLeftGuardFence() ? 0 : 200;
		m_dW4 = IsLeftGuardFence() ? 200 : 0;
		m_dW3 = IsLeftGuardFence() ? m_dWidhtHDan - m_dW4 : m_dWidhtHDan - m_dW2;
		m_dW5 = 0;
		
		m_dH1 = 0;
		m_dH2 = 0;
		m_dH3 = 800;	
		m_dR1 = 0;
		m_dR2 = 0;
		m_dR3 = 0;
		m_dC  = 0;
		m_dW6  = 0;
		m_dW7  = 0;
		m_dH4  = 0;
		m_dH5  = 0;
		m_dForce  = 0;
	}
	else if(IsTypeHDanRailDetectGravel2())	//�ڰ�����
	{
		m_dDistFrLeft = 0;
		m_dW1 = 0;
		m_dW2 = 0;
		m_dW3 = m_dWidhtHDan;
		m_dW4 = 0;
		m_dW5 = 0;
		
		m_dH1 = 0;
		m_dH2 = 0;
		m_dH3 = 300;	
		m_dR1 = 0;
		m_dR2 = 0;
		m_dR3 = 0;
		m_dC  = 0;
		m_dW6  = 0;
		m_dW7  = 0;
		m_dH4  = 0;
		m_dH5  = 0;
		m_dForce  = 0;
	}
	else if(IsTypeHDanRailFootway())	// ����
	{
		m_dDistFrLeft = 0;
		m_dW1 = 0;
		m_dW2 = 0;
		m_dW3 = m_dWidhtHDan;
		m_dW4 = 0;
		m_dW5 = 0;

		m_dH1 = 0;
		m_dH2 = 0;
		m_dH3 = 300;
		m_dR1 = 0;
		m_dR2 = 0;
		m_dR3 = 0;
		m_dC  = 0;
		m_dForce  = 0;

		m_nTypePave				 = 0;
		m_nTypeInnerFill		 = 0;
		m_dUnitWeightPave		 = 2300;// tonf/m��
		m_dUnitWeightInnerFill	 = 2000;// tonf/m��
		m_dThickPaveFootway		 = 50;
		m_dSlopeUpper			 = m_bLeftGuardFence ? -0.02 : 0.02;
	}
	else if(IsTypeHDanRailFootwayBlock())	//����+����
	{
		m_dDistFrLeft = 0;
		m_dW1 = IsLeftGuardFence() ? 0 : 600;
		m_dW2 = 0;
		m_dW3 = m_dWidhtHDan - 600;
		m_dW4 = 0;
		m_dW5 = IsLeftGuardFence() ? 600 : 0;
		m_dH1 = 350;
		m_dH2 = 0;
		m_dH3 = 300;	

		m_dW6 = IsLeftGuardFence() ? 550 : m_dWidhtHDan - 300 -150 - 400;
		m_dW7 = 300;
		m_dH4 = 70;
		m_dH5 = 200;

		m_dR1 = 0;
		m_dR2 = 0;
		m_dR3 = 0;
		m_dC  = 0;
		m_dForce  = 0;
	}
	else if(IsTypeHDanRailHole() || IsTypeHDanRailHoleFootway())	//��ű� �Ǵ� ��ű� + ����
	{
		m_dDistFrLeft = 0;
		m_dW1 = 0;
		m_dW2 = 0;
		m_dW3 = m_dWidhtHDan;	
		m_dW4 = 0;
		m_dW5 = 0;
		
		m_dH1 = 0;
		m_dH2 = 0;
		m_dH3 = 350;		

		m_dW6 = 150;
		m_dW7 = m_dWidhtHDan - 300;
		m_dH4 = 70;
		m_dH5 = 200;

		m_dR1 = 0;
		m_dR2 = 0;
		m_dR3 = 0;
		m_dC  = 0;
		m_dForce  = 0;
	}
	else if(IsTypeHDanRoadRailDike())	// ����, ö�� ����ũ
	{
		m_dDistFrLeft = 0;
		m_dW1 = 0;
		m_dW2 = IsLeftGuardFence() ? 0 : 135;
		m_dW3 = 115;
		m_dW4 = IsLeftGuardFence() ? 135 : 0;
		m_dW5 = 0;

		m_dH1 = 0;
		m_dH2 = 0;
		m_dH3 = 400 + thickPave+150;
		m_dR1 = 0;
		m_dR2 = 0;
		m_dR3 = 20;
		m_dC  = 0;
		m_dForce  = 0.1;
	}
}

//<summary>
//</summary>
//<param name=""></param>
void CGuardWallRC::GetTvGuardWallBase(CTwinVectorArray& tvArr, BOOL bApplyRoundChamfer, BOOL bForBM, double dSlopeSlab)
{
	if(IsTypeHDanRoadFootway() || IsTypeHDanRailFootway() || IsTypeHDanRoadFootwayBlock())	// �����̸� + ��������輮
	{
		GetTvFootwayBase(tvArr, bApplyRoundChamfer, bForBM, dSlopeSlab);
		return;
	}
	tvArr.RemoveAll();

	double dD  = m_dDistFrLeft;
	double dH1 = m_dH1;
	double dH2 = m_dH2;
	double dH3 = m_dH3;
	double dW1 = m_dW1;
	double dW2 = m_dW2;
	double dW3 = m_dW3;
	double dW4 = m_dW4;
	double dW5 = m_dW5;
	double dR1 = m_dR1;
	double dR2 = m_dR2;
	double dR  = m_dR3;
	double dC  = m_dC;
	
	// �̷� ��� �� �ȵǳ�....�ϴ� �̷��� ����
	if(dH1>0 && dH2==0 && dH3==0)  
	{
		dH3 = dH1;
		dH1 = 0;
	}
	if(dH2>0 && dH1==0 && dH3==0)
	{
		dH3 = dH2;
		dH2 = 0;
	}
	// #37537�� ���� ���� by Ha, ���� ����� �ƴ� ��..
	if(dH3>0 && dH1==0 && dH2==0)
	{
		if(dW5 > 0 && dW4 == 0)
		{
			dW4 = dW5;
			dW5 = 0;
		}
		if(dW1 > 0 && dW2 == 0)
		{
			dW2 = dW1;
			dW1 = 0;
		}
	}

	if(bApplyRoundChamfer==FALSE || bForBM)
	{
		dR1 = 0;
		dR2 = 0;
		dR  = 0;
		dC  = 0;
	}

	CDPoint xyCen(0, 0), xy1(0, 0), xy2(0, 0);
	CDPoint xyCur(0, 0), xyPrev(0,0), xyNext(0, 0);

	CTwinVector tv;

	tv.m_v1 = CVector(0, 0, 0);	tv.m_v2 = CVector(dD, 0, 0);	// 0~1
	tv.m_sInfo	= "0";	
	tvArr.Add(tv);	//0
	xyPrev = CDPoint(dD, 0);				// 1�� ��ǥ
	xyCur  = CDPoint(dD, dH1);				// 2�� ��ǥ
	xyNext = (dH1 + dH2) > 0 ? CDPoint(dD+dW1, dH1+dH2) : CDPoint(dD, dH1+dH2);		// 3�� ��ǥ
	GetInSideTangentCircle(xyCur, xyPrev, xyNext, dR1, xyCen, xy1, xy2);	//xy2 = ����, �ð�ݴ���� ȸ��

	tv.m_v1 = xyPrev;	tv.m_v1.z = 0;	tv.m_v2 = xy2;	tv.m_v2.z = 0;	//1~2 ����
	tv.m_sInfo	= "1";
	tvArr.Add(tv);	//1
	tv.m_v1 = xy1;	tv.m_v1.z = -1;	tv.m_v2 = xy2;	tv.m_v2.z = dR1;	//2 ���� ~ 2 ����
	tvArr.Add(tv);	//2

	xyPrev = xy1;								//2 ����
	xyCur  = (dH1 + dH2) > 0 ? CDPoint(dD+dW1, dH1+dH2) : CDPoint(dD, dH1+dH2);			//3�� ��ǥ
	xyNext = CDPoint(xyCur.x+dW2, xyCur.y+dH3);	//4�� ��ǥ
	GetInSideTangentCircle(xyCur, xyNext, xyPrev, dR2, xyCen, xy1, xy2);	//xy1 = ����, �ð�ݴ���� ȸ��

	tv.m_v1 = xyPrev;	tv.m_v1.z = 0;	tv.m_v2 = xy1;	tv.m_v2.z = 0;	//2 ���� ~ 3����
	tv.m_sInfo = "2";
	tvArr.Add(tv);	//3
	tv.m_v1 = xy1;	tv.m_v1.z = -1;	tv.m_v2 = xy2;	tv.m_v2.z = dR2;	//3 ���� ~ 3����
	tvArr.Add(tv);	//4

	if(m_dR3 > 0)	//m_dR3 ���� "0"���� ũ�� ������ ���� ���̶�� �Ǵ���
	{
		xyPrev = xy2;								//3����
		xyCur  = CDPoint(dD+dW1+dW2, dH1+dH2+dH3);	//4�� ��ǥ
		xyNext = CDPoint(xyCur.x+dW3, xyCur.y);		//5�� ��ǥ
		GetInSideTangentCircle(xyCur, xyPrev, xyNext, dR, xyCen, xy1, xy2);	//xy2 = ����, �ð�ݴ���� ȸ��
		tv.m_v1 = xyPrev;	tv.m_v1.z = 0;	tv.m_v2 = xy2;	tv.m_v2.z = 0;	//3 ���� ~ 4 ����
		tv.m_sInfo = "3";
		tvArr.Add(tv);	//5
		tv.m_v1 = xy1;	tv.m_v1.z = -1;	tv.m_v2 = xy2;	tv.m_v2.z = dR;	//4 ���� ~ 4 ����
		tvArr.Add(tv);	//6

		xyPrev = xy1;									//4 ����
		xyCur  = CDPoint(dD+dW1+dW2+dW3, dH1+dH2+dH3);	//5�� ��ǥ
		xyNext = CDPoint(xyCur.x+dW4, xyCur.y-dH3);		//6�� ��ǥ
		GetInSideTangentCircle(xyCur, xyPrev, xyNext, dR, xyCen, xy1, xy2);	//xy2 = ����, �ð�ݴ���� ȸ��
		tv.m_v1 = xyPrev;	tv.m_v1.z = 0;	tv.m_v2 = xy2;	tv.m_v2.z = 0;	//4 ���� ~ 5 ����
		tv.m_sInfo = "4";
		tvArr.Add(tv);	//7
		tv.m_v1 = xy1;	tv.m_v1.z = -1;	tv.m_v2 = xy2;	tv.m_v2.z = dR;	//5 ���� ~ 5 ����
		tvArr.Add(tv);	//8
	}
	else
	{
		xyPrev = xy2;								//3 ����
		xyCur  = CDPoint(dD+dW1+dW2, dH1+dH2+dH3);	//4�� ��ǥ
		xyNext = CDPoint(xyCur.x+dW3, xyCur.y);		//5�� ��ǥ

		GetXyChamferSegAndSeg(xyPrev, xyCur, xyNext, xyCur, xy1, xy2, dC, dC);
		tv.m_v1 = xyPrev;	tv.m_v1.z = 0;	tv.m_v2 = xy1;	tv.m_v2.z = 0;	//3 ���� ~ 4 �� è��
		tv.m_sInfo = "3";
		tvArr.Add(tv);	//5
		tv.m_v1 = xy1;	tv.m_v1.z = 0;	tv.m_v2 = xy2;	tv.m_v2.z = 0;		//4 �� è�� ~ 4 �� è��
		tv.m_sInfo = "4";
		tvArr.Add(tv);	//6

		

		xyPrev = xy2;									//4 �� è��
		xyCur  = CDPoint(dD+dW1+dW2+dW3, dH1+dH2+dH3);	//5�� ��ǥ
		xyNext = CDPoint(xyCur.x+dW4, xyCur.y-dH3);		//6�� ��ǥ
		GetXyChamferSegAndSeg(xyPrev, xyCur, xyNext, xyCur, xy2, xy1, dC, dC);
		tv.m_v1 = xyPrev;	tv.m_v1.z = 0;	tv.m_v2 = xy2;	tv.m_v2.z = 0;	//4 �� è�� ~ 5 �� è��
		tv.m_sInfo = "5";
		tvArr.Add(tv);	//7
		tv.m_v2 = xy1;	tv.m_v1.z = 0;	tv.m_v1 = xy2;	tv.m_v2.z = 0;	//5 �� è�� ~ 5 �� è��
		tv.m_sInfo = "6";
		tvArr.Add(tv);	//8
	}

	xyPrev = xy1;									//5 ����
	xyCur  = CDPoint(dD+dW1+dW2+dW3+dW4, dH1+dH2);	//6�� ��ǥ
	xyNext = CDPoint(xyCur.x+dW5, xyCur.y-dH2);		//7�� ��ǥ
	GetInSideTangentCircle(xyCur, xyNext, xyPrev, dR2, xyCen, xy1, xy2);	//xy1 = ����, �ð�ݴ���� ȸ��
	tv.m_v1 = xyPrev;	tv.m_v1.z = 0;	tv.m_v2 = xy1;	tv.m_v2.z = 0;	//5���� ~ 6����
	tv.m_sInfo = "7";
	tvArr.Add(tv);	//9
	tv.m_v1 = xy1;	tv.m_v1.z = -1;	tv.m_v2 = xy2;	tv.m_v2.z = dR2;	//6���� ~ 6����
	tvArr.Add(tv);	//10

	xyPrev = xy2;									//6����
	xyCur  = CDPoint(dD+dW1+dW2+dW3+dW4+dW5, dH1);	//7�� ��ǥ
	xyNext = CDPoint(xyCur.x, 0);					//8�� ��ǥ
	GetInSideTangentCircle(xyCur, xyPrev, xyNext, dR1, xyCen, xy1, xy2);	//xy2 = ����, �ð�ݴ���� ȸ��
	tv.m_v1 = xyPrev;	tv.m_v1.z = 0;	tv.m_v2 = xy2;	tv.m_v2.z = 0;	//6 ���� ~ 7 ����
	tv.m_sInfo = "8";
	tvArr.Add(tv);	//11
	tv.m_v1 = xy1;	tv.m_v1.z = -1;	tv.m_v2 = xy2;	tv.m_v2.z = dR1;	//7 ���� ~ 7 ����
	tvArr.Add(tv);	//12
	tv.m_v1 = xy1;	tv.m_v1.z = 0;	tv.m_v2 = xyNext;	tv.m_v2.z = 0;	//7 ���� ~ 8 ����ǥ
	tv.m_sInfo = "9";
	tvArr.Add(tv);	//13

	if(IsTypeHDanRailFootwayBlock() || IsTypeHDanRailHole() || IsTypeHDanRailHoleFootway())
	{
		tv.m_v1 = CVector(m_dW6, m_dH4, 0);			tv.m_v2 = CVector(m_dW6, m_dH4+m_dH5, 0);	//Hole
		tv.m_sInfo = "HoleLine1";
		tvArr.Add(tv);	//14
		tv.m_v1 = CVector(m_dW6, m_dH4+m_dH5, 0);	tv.m_v2 = CVector(m_dW6+m_dW7, m_dH4+m_dH5, 0);	//Hole
		tv.m_sInfo = "HoleLine2";
		tvArr.Add(tv);	//15
		tv.m_v1 = CVector(m_dW6+m_dW7, m_dH4+m_dH5, 0);	tv.m_v2 = CVector(m_dW6+m_dW7, m_dH4, 0);	//Hole
		tv.m_sInfo = "HoleLine3";
		tvArr.Add(tv);	//16
		tv.m_v1 = CVector(m_dW6+m_dW7, m_dH4, 0);	tv.m_v2 = CVector(m_dW6, m_dH4, 0);	//Hole
		tv.m_sInfo = "HoleLine4";
		tvArr.Add(tv);	//17
		tv.m_v1 = CVector(m_dW6, m_dH4, 0);	tv.m_v2 = CVector(m_dW6+m_dW7, m_dH4+m_dH5, 0);	//Hole
		tv.m_sInfo = "HoleMark1";
		tvArr.Add(tv);	//18
		tv.m_v1 = CVector(m_dW6, m_dH4+m_dH5, 0);	tv.m_v2 = CVector(m_dW6+m_dW7, m_dH4, 0);	//Hole
		tv.m_sInfo = "HoleMark2";
		tvArr.Add(tv);	//19
	}
}
//<summary>
// ���� ��ǥ�� ����
// ������ ���Ͽ� ��ǥ ������ ��ȣ�� �Ϲݰ� �� ���� �Ͽ���.
//</summary>
//<param name=""></param>
// dSlopeSlab : ������ ��������� ����� ���ؼ� ���ڸ� �߰� (0,0)�� �����ϴ����� ����
void CGuardWallRC::GetTvFootwayBase(CTwinVectorArray& tvArr, BOOL bApplyRoundChamfer, BOOL bForBM, double dSlopeSlab)
{
	tvArr.RemoveAll();

	double dH3L  = m_dH3;
	double dH3R  = m_dH3;
	double dW3   = m_dWidhtHDan;
	double slopeGuard = IsTypeHDanRoadFootwayBlock() ? 0 : m_dSlopeUpper;
	double slopeSlab = dSlopeSlab; //m_bLeftGuardFence ? -dSlopeSlab : dSlopeSlab;
	double dDiffH_Guard	= 0;// ��ȣ�� ���迡 ���ؼ� ����� ������
	if(slopeGuard < 0)
	{
		dDiffH_Guard = -(slopeGuard*dW3);
		dH3L += dDiffH_Guard;
	}
	else
	{
		dDiffH_Guard = slopeGuard*dW3;
		dH3R += dDiffH_Guard;
	}

	// ������ ���迡 ���ؼ� ����� ������
	double dDiffH_Slab = slopeSlab*dW3;
	dH3R -= dDiffH_Slab;

	// (ARCBRIDGE-3459) ������ ���迡���� 
	// �������� m_dH�� �ǵ��� ����
// 	if(dH3L>dH3R)
// 	{
// 		dH3L += (m_dH3 - dH3R);
// 		dH3R = m_dH3;
// 	}
// 	else
// 	{
// 		dH3R += (m_dH3 - dH3L);
// 		dH3L = m_dH3;
// 	}

	CVector tvZero(0,0,0);
	CVector tvH3L(0, dH3L, 0);
	CVector tvH3R(dW3, dH3R+dDiffH_Slab, 0); // ������ (0,0) ������ (0,dDiffH_Slab)
	CVector tvEnd(dW3,dDiffH_Slab,0);


	CTwinVector tv;

	tv.m_v1 = tvZero;	tv.m_v2 = tvZero;	// 0~1
	tv.m_sInfo	= "0";	
	tvArr.Add(tv);	//0

	tv.m_v1 = tvZero;	tv.m_v2 = tvZero;	//1~2 ����
	tv.m_sInfo	= "1";
	tvArr.Add(tv);	//1
	tv.m_v1 = tvZero;	tv.m_v2 = tvZero;	//2 ���� ~ 2 ����
	tvArr.Add(tv);	//2
	tv.m_v1 = tvZero;	tv.m_v2 = tvZero;	//2 ���� ~ 3����
	tv.m_sInfo = "2";
	tvArr.Add(tv);	//3
	tv.m_v1 = tvZero;	tv.m_v2 = tvZero;	//3 ���� ~ 3����
	tvArr.Add(tv);	//4
	tv.m_v1 = tvZero;	tv.m_v2 = tvH3L;	//3 ���� ~ 4 ����
	tv.m_sInfo = "3";
	tvArr.Add(tv);	//5
	tv.m_v1 = tvH3L;	tv.m_v2 = tvH3L;	//4 ���� ~ 4 ����
	tvArr.Add(tv);	//6
	tv.m_v1 = tvH3L;	tv.m_v2 = tvH3R;	//4 ���� ~ 5 ����
	tv.m_sInfo = "4";
	tvArr.Add(tv);	//7
	tv.m_v1 = tvH3R;	tv.m_v2 = tvH3R;	//5 ���� ~ 5 ����
	tvArr.Add(tv);	//8
	tv.m_v1 = tvH3R;	tv.m_v2 = tvEnd;	//5���� ~ 6����
	tv.m_sInfo = "7";
	tvArr.Add(tv);	//9
	tv.m_v1 = tvEnd;	tv.m_v2 = tvEnd;	//6���� ~ 6����
	tvArr.Add(tv);	//10
	tv.m_v1 = tvEnd;	tv.m_v2 = tvEnd;	//6 ���� ~ 7 ����
	tv.m_sInfo = "8";
	tvArr.Add(tv);	//11
	tv.m_v1 = tvEnd;	tv.m_v2 = tvEnd;	//7 ���� ~ 7 ����
	tvArr.Add(tv);	//12
	tv.m_v1 = tvEnd;	tv.m_v2 = tvEnd;	//7 ���� ~ 8 ����ǥ
	tv.m_sInfo = "9";
	tvArr.Add(tv);	//13

	// ������ ��� ���� ǥ��
	if(IsTypeHDanRoadFootway() || IsTypeHDanRailFootway())
	{
		if(m_dThickPaveFootway>0 && !bForBM)
		{
			tv = tvArr.GetAt(7);
			long nTerm = (long)RoundUp(m_dThickPaveFootway/10,0);
			double dSpace = m_dThickPaveFootway/nTerm;
			long n=0; for(n=0; n<nTerm; n++)
			{
				tv.m_v1.y += dSpace;
				tv.m_v2.y += dSpace;
				tvArr.Add(tv);
			}
		}
	}

	if(m_bRailRoad)
	{
		return;
	}
}



//<summary>
//	m_dR3>0 �ϰ�쿡�� TRUE
//</summary>
//<param name=""></param>
BOOL CGuardWallRC::IsRound()
{
	if(m_dR3 > 0)
		return TRUE;
	
	return FALSE;
}
//<summary>
// Ⱦ�ܿ��� ����, �����Ż �� ������ ����
//���α�  ����
//ö����  ����, ����
//</summary>
//<param name=""></param>
BOOL CGuardWallRC::IsPaveOnHDan()
{
	if(IsTypeHDanOnlyNone() || IsTypeHDanRailTotalDosang())
		return TRUE;
	
	return FALSE;
}

//<summary>
//	��ȣ���� ġ�� �׸�
//</summary>
//<param name="bInput"></param>
//<param name="bWriteLeft">��ȣ�� ���� ġ�� ǥ��</param>
//<param name="bWriteRigh">��ȣ�� ���� ġ�� ǥ��</param>
void CGuardWallRC::DimGuardWallBase(CDomyun* pDomP, BOOL bInput, BOOL bWriteLeft, BOOL bWriteRight)
{
	double dD  = m_dDistFrLeft;
	double dH1 = m_dH1;
	double dH2 = m_dH2;
	double dH3 = m_dH3;
	double dW1 = m_dW1;
	double dW2 = m_dW2;
	double dW3 = m_dW3;
	double dW4 = m_dW4;
	double dW5 = m_dW5;
	double dR1 = m_dR1;
	double dR2 = m_dR2;
	double dR3 = m_dR3;
	double dC  = m_dC;
	if(IsTypeHDanRoadFootway() || IsTypeHDanRailFootway())
		dW3 = m_dWidhtHDan;

	long nDimDan  = 0;
	double dDimX = 0;
	CString strUp = _T(""), strLow = _T("");

	CDomyun Dom(pDomP);

	if(bWriteLeft)		// ���� ġ�� ǥ��
	{
		Dom.SetDirection("LEFT");
		Dom.DimMoveTo(dDimX, 0, nDimDan);
		strUp = bInput ? _T("H1") : COMMA(dH1);
		strLow = bInput ? COMMA(dH1) : _T("");
		Dom.DimLineToExtend(dH1, nDimDan, strUp, strLow);
		strUp = bInput ? _T("H2") : COMMA(dH2);
		strLow = bInput ? COMMA(dH2) : _T("");
		Dom.DimLineToExtend(dH2, nDimDan, strUp, strLow);
		strUp = bInput ? _T("H3") : COMMA(dH3);
		strLow = bInput ? COMMA(dH3) : _T("");
		Dom.DimLineToExtend(dH3, nDimDan++, strUp, strLow);

		if((dH1 > 0 && dH2 > 0) || (dH2 > 0 && dH3 > 0) || (dH3 > 0 && dH1 > 0))//���� 2�� �̻��� "0"���� Ŭ ���
		{
			Dom.DimMoveTo(dDimX, 0, nDimDan);
			Dom.DimLineTo(dH1+dH2+dH3, nDimDan, COMMA(dH1+dH2+dH3));
		}
	}
	nDimDan = 0;
	if(bWriteRight)		// ���� ġ�� ǥ��
	{
		dDimX = dD + dW1+dW2+dW3+dW4+dW5;
		Dom.SetDirection("RIGHT");
		Dom.DimMoveTo(dDimX, 0, nDimDan);
		strUp = bInput ? _T("H1") : COMMA(dH1);
		strLow = bInput ? COMMA(dH1) : _T("");
		Dom.DimLineToExtend(dH1, nDimDan, strUp, strLow);
		strUp = bInput ? _T("H2") : COMMA(dH2);
		strLow = bInput ? COMMA(dH2) : _T("");
		Dom.DimLineToExtend(dH2, nDimDan, strUp, strLow);
		strUp = bInput ? _T("H3") : COMMA(dH3);
		strLow = bInput ? COMMA(dH3) : _T("");
		Dom.DimLineToExtend(dH3, nDimDan++, strUp, strLow);

		if((dH1 > 0 && dH2 > 0) || (dH2 > 0 && dH3 > 0) || (dH3 > 0 && dH1 > 0))//���� 2�� �̻��� "0"���� Ŭ ���
		{
			Dom.DimMoveTo(dDimX, 0, nDimDan);
			Dom.DimLineTo(dH1+dH2+dH3, nDimDan, COMMA(dH1+dH2+dH3));
		}
	}

	// ���
	nDimDan = 0;
	Dom.SetDirection("TOP");
	Dom.SetDimExo(10);

	dDimX = bInput ? 0 : dD;

	Dom.DimMoveTo(dDimX, dH1+dH2+dH3, nDimDan);
	if(bInput)
	{
		strUp = _T("D");
		strLow = COMMA(dD);
		Dom.DimLineToExtend(dD, nDimDan, strUp, strLow);
	}
	strUp = bInput ? _T("W1") : COMMA(dW1);
	strLow = bInput ? COMMA(dW1) : _T("");
	Dom.DimLineToExtend(dW1, nDimDan, strUp, strLow);
	strUp = bInput ? _T("W2") : COMMA(dW2);
	strLow = bInput ? COMMA(dW2) : _T("");
	Dom.DimLineToExtend(dW2, nDimDan, strUp, strLow);
	if(IsTypeHDanRoadFootway() || IsTypeHDanRailFootway())
	{
		strUp = COMMA(m_dWidhtHDan);
		strLow = _T("");
	}
	else
	{
		strUp = bInput ? _T("W3") : COMMA(dW3);
		strLow = bInput ? COMMA(dW3) : _T("");
	}
	Dom.DimLineToExtend(dW3, nDimDan, strUp, strLow);

	strUp = bInput ? _T("W4") : COMMA(dW4);
	strLow = bInput ? COMMA(dW4) : _T("");
	Dom.DimLineToExtend(dW4, nDimDan, strUp, strLow);
	strUp = bInput ? _T("W5") : COMMA(dW5);
	strLow = bInput ? COMMA(dW5) : _T("");
	Dom.DimLineToExtend(dW5, nDimDan, strUp, strLow);

	long nQtyDimen = 0;
	if(dW1 > 0)		nQtyDimen++;
	if(dW2 > 0)		nQtyDimen++;
	if(dW3 > 0)		nQtyDimen++;
	if(dW4 > 0)		nQtyDimen++;
	if(dW5 > 0)		nQtyDimen++;
	if(nQtyDimen >= 2)
	{
		Dom.DimMoveTo(dD, dH1+dH2+dH3, ++nDimDan);
		Dom.DimLineTo(dW1+dW2+dW3+dW4+dW5, nDimDan, COMMA(dW1+dW2+dW3+dW4+dW5));
	}
	if((!IsTypeHDanRoadFootway() || !IsTypeHDanRailFootway()) && bInput)
	{
		strUp = COMMA(m_dWidhtHDan);
		strLow = _T("");
		Dom.DimMoveTo(dDimX, dH1+dH2+dH3, ++nDimDan);
		Dom.DimLineTo(m_dWidhtHDan, nDimDan, COMMA(m_dWidhtHDan));
	}

	if(m_dW6>0 && m_dW7>0 && m_dH4>0 && m_dH5>0)
	{
		nDimDan = -1;
		Dom.SetDirection("LEFT");
		Dom.DimMoveTo(m_dW6, 0, nDimDan);
		strUp = bInput ? _T("H4") : COMMA(m_dH4);
		strLow = bInput ? COMMA(m_dH4) : _T("");
		Dom.DimLineToExtend(m_dH4, nDimDan, strUp, strLow);
		strUp = bInput ? _T("H5") : COMMA(m_dH5);
		strLow = bInput ? COMMA(m_dH5) : _T("");
		Dom.DimLineToExtend(m_dH5, nDimDan, strUp, strLow);

		Dom.SetDirection("BOTTOM");
		Dom.DimMoveTo(0, m_dH4, nDimDan);
		strUp = bInput ? _T("W6") : COMMA(m_dW6);
		strLow = bInput ? COMMA(m_dW6) : _T("");
		Dom.DimLineToExtend(m_dW6, nDimDan, strUp, strLow);
		strUp = bInput ? _T("W7") : COMMA(m_dW7);
		strLow = bInput ? COMMA(m_dW7) : _T("");
		Dom.DimLineToExtend(m_dW7, nDimDan, strUp, strLow);
	}

	// ����� �� ����
	CDPoint xyPrev(0, 0), xyCur(0, 0), xyNext(0, 0);
	CDPoint xyCen(0, 0), xy1(0, 0), xy2(0, 0);
	CDPoint xyR1(0, 0), xyR2(0, 0);
	double radAng1 = 0, radAng2 = 0;

	double widDim = bInput ? Dom.Always(5) : Dom.Always(15);
	if(dR1>0 && dW1>0 && dH1>0)
	{
		xyPrev = CDPoint(dD, 0);
		xyCur  = CDPoint(dD, dH1);
		xyNext = CDPoint(dD+dW1, dH1+dH2);
		GetInSideTangentCircle(xyCur, xyPrev, xyNext, dR1, xyCen, xy1, xy2);
		radAng1 = (xy1-xyCen).GetAngleRadian();
		radAng2 = (xy2-xyCen).GetAngleRadian();

		GetXyMatchLineAndArc(xyCen, (xy1+xy2)/2-xyCen, xyCen, dR1, radAng1, radAng2, xyR1, xyR2);
		strUp = bInput ? _T("R1") : _T("R=")+COMMA(dR1);
		strLow = bInput ? COMMA(dR1) : _T("");
		Dom.GiSiArrowExtend(xyR1.x, xyR1.y, Dom.Always(5), -widDim, 3, strUp, strLow);
	}

	if(dR2>0 && (dW1>0 || dH2>0) && (dW2>0 || dH3>0) && dW1+dW2>0)
	{
		xyPrev = CDPoint(dD, dH1);
		xyCur  = CDPoint(dD+dW1, dH1+dH2);
		xyNext = CDPoint(dD+dW1+dW2, dH1+dH2+dH3);
		GetInSideTangentCircle(xyCur, xyNext, xyPrev, dR2, xyCen, xy1, xy2);
		radAng1 = (xy1-xyCen).GetAngleRadian();
		radAng2 = (xy2-xyCen).GetAngleRadian();

		GetXyMatchLineAndArc(xyCen, (xy1+xy2)/2-xyCen, xyCen, dR2, radAng1, radAng2, xyR1, xyR2);
		strUp = bInput ? _T("R2") : _T("R=")+COMMA(dR2);
		strLow = bInput ? COMMA(dR2) : _T("");
		Dom.GiSiArrowExtend(xyR1.x, xyR1.y, Dom.Always(5), -widDim, 3, strUp, strLow);
	}
	if(IsRound())// R3 �Ǵ� C
	{
		if(dR3>0 && dW3>0 && dH3>0)
		{
			xyPrev = CDPoint(dD+dW1, dH1+dH2);
			xyCur  = CDPoint(dD+dW1+dW2, dH1+dH2+dH3);
			xyNext = CDPoint(dD+dW1+dW2+dW3, dH1+dH2+dH3);
			GetInSideTangentCircle(xyCur, xyPrev, xyNext, dR3, xyCen, xy1, xy2);
			radAng1 = (xy1-xyCen).GetAngleRadian();
			radAng2 = (xy2-xyCen).GetAngleRadian();

			GetXyMatchLineAndArc(xyCen, (xy1+xy2)/2-xyCen, xyCen, dR3, radAng1, radAng2, xyR1, xyR2);
			strUp = bInput ? _T("R") : _T("R=")+COMMA(dR3);
			strLow = bInput ? COMMA(dR3) : _T("");
			Dom.GiSiArrowExtend(xyR1.x, xyR1.y, Dom.Always(5), -widDim, 3, strUp, strLow);
		}
	}
	else
	{
		if(dC>0 && dW3>0 && dH3>0)
		{
			xyPrev = CDPoint(dD+dW1, dH1+dH2);
			xyCur  = CDPoint(dD+dW1+dW2, dH1+dH2+dH3);
			xyNext = CDPoint(dD+dW1+dW2+dW3, dH1+dH2+dH3);
			GetXyChamferSegAndSeg(xyPrev, xyCur, xyNext, xyCur, xy2, xy1, dC, dC);

			xyR1 = (xy1+xy2)/2;
			strUp = bInput ? _T("C X C") : COMMA(dC)+"X"+COMMA(dC);
			strLow = bInput ? COMMA(dC)+"X"+COMMA(dC) : _T("");
			Dom.GiSiArrowExtend(xyR1.x, xyR1.y, Dom.Always(5), -widDim, 3, strUp, strLow);
		}
	}
	if(dR1>0 && dW5>0 && dH1>0)
	{
		xyPrev = CDPoint(dD+dW1+dW2+dW3+dW4, dH1+dH2);
		xyCur  = CDPoint(dD+dW1+dW2+dW3+dW4+dW5, dH1);
		xyNext = CDPoint(dD+dW1+dW2+dW3+dW4+dW5, 0);
		GetInSideTangentCircle(xyCur, xyPrev, xyNext, dR1, xyCen, xy1, xy2);
		radAng1 = (xy1-xyCen).GetAngleRadian();
		radAng2 = (xy2-xyCen).GetAngleRadian();

		GetXyMatchLineAndArc(xyCen, (xy1+xy2)/2-xyCen, xyCen, dR1, radAng1, radAng2, xyR1, xyR2);
		strUp = bInput ? _T("R1") : _T("R=")+COMMA(dR1);
		strLow = bInput ? COMMA(dR1) : _T("");
		Dom.GiSiArrowExtend(xyR1.x, xyR1.y, Dom.Always(5), widDim, 1, strUp, strLow);
	}

	if(dR2>0 && (dW4>0 || dH3>0) && (dW5>0 || dH2>0) && dW4+dW5>0)
	{
		xyPrev = CDPoint(dD+dW1+dW2+dW3, dH1+dH2+dH3);
		xyCur  = CDPoint(dD+dW1+dW2+dW3+dW4, dH1+dH2);
		xyNext = CDPoint(dD+dW1+dW2+dW3+dW4+dW5, dH1);
		GetInSideTangentCircle(xyCur, xyNext, xyPrev, dR2, xyCen, xy1, xy2);
		radAng1 = (xy1-xyCen).GetAngleRadian();
		radAng2 = (xy2-xyCen).GetAngleRadian();

		GetXyMatchLineAndArc(xyCen, (xy1+xy2)/2-xyCen, xyCen, dR2, radAng1, radAng2, xyR1, xyR2);
		strUp = bInput ? _T("R2") : _T("R=")+COMMA(dR2);
		strLow = bInput ? COMMA(dR2) : _T("");
		Dom.GiSiArrowExtend(xyR1.x, xyR1.y, Dom.Always(5), widDim, 1, strUp, strLow);
	}
	if(IsRound())// R3 �Ǵ� C
	{
		if(dR3>0 && dW3>0 && dH3>0)
		{
			xyPrev = CDPoint(dD+dW1+dW2, dH1+dH2+dH3);
			xyCur  = CDPoint(dD+dW1+dW2+dW3, dH1+dH2+dH3);
			xyNext = CDPoint(dD+dW1+dW2+dW3+dW4, dH1+dH2);
			GetInSideTangentCircle(xyCur, xyPrev, xyNext, dR3, xyCen, xy1, xy2);
			radAng1 = (xy1-xyCen).GetAngleRadian();
			radAng2 = (xy2-xyCen).GetAngleRadian();

			GetXyMatchLineAndArc(xyCen, (xy1+xy2)/2-xyCen, xyCen, dR3, radAng1, radAng2, xyR1, xyR2);
			strUp = bInput ? _T("R") : _T("R=")+COMMA(dR3);
			strLow = bInput ? COMMA(dR3) : _T("");
			Dom.GiSiArrowExtend(xyR1.x, xyR1.y, Dom.Always(5), widDim, 1, strUp, strLow);
		}
	}
	else
	{
		if(dC>0 && dW3>0 && dH3>0)
		{
			xyPrev = CDPoint(dD+dW1+dW2, dH1+dH2+dH3);
			xyCur  = CDPoint(dD+dW1+dW2+dW3, dH1+dH2+dH3);
			xyNext = CDPoint(dD+dW1+dW2+dW3+dW4, dH1+dH2);
			GetXyChamferSegAndSeg(xyPrev, xyCur, xyNext, xyCur, xy2, xy1, dC, dC);

			xyR1 = (xy1+xy2)/2;
			strUp = bInput ? _T("C X C") : COMMA(dC)+"X"+COMMA(dC);
			strLow = bInput ? COMMA(dC)+"X"+COMMA(dC) : _T("");
			Dom.GiSiArrowExtend(xyR1.x, xyR1.y, Dom.Always(5), widDim, 1, strUp, strLow);
		}
	}

	if(IsTypeHDanRoadFootway() || IsTypeHDanRailFootway())
	{
		CTwinVectorArray tvArr;
		CTwinVector tv;
		GetTvGuardWallBase(tvArr, FALSE);
		tv = tvArr.GetAt(7);
		double dLen = (tv.m_v2.x-tv.m_v1.x)/4;
		GetXyMatchSegAndLine(tv.m_v1, tv.m_v2, CDPoint(tv.m_v2.x-dLen, 0), CDPoint(0, 1), xyCur);

		// ���� ���ü�
		Dom.SetDirection("TOP");
		strUp.Format(_T("(T=%.fmm)"), m_dThickPaveFootway);
		if(bWriteLeft)
			Dom.GiSiArrowExtend(xyCur.x, xyCur.y+m_dThickPaveFootway, Dom.Always(10), Dom.Always(-25), 3, "���� : "+GetStringPave(m_nTypePave), strUp);
		else
			Dom.GiSiArrowExtend(xyCur.x, xyCur.y+m_dThickPaveFootway, Dom.Always(10), Dom.Always(25), 1, "���� : "+GetStringPave(m_nTypePave), strUp);

		xyCur = tv.GetXyMid();
		xyCur.y += 20 + m_dThickPaveFootway;
		CDPoint vSlope = tv.GetXyDir();
	//	if(m_dSlopeUpper < 0)	vSlope *= -1;
		strUp.Format(_T("%.3f%%"), m_dSlopeUpper*100);
		ArrowDirectionForSlope(&Dom, xyCur, strUp, vSlope);// ��� ȭ��ǥ

		if(bWriteLeft)
		{
			xyCur = tv.m_v1;
			Dom.SetDirection("LEFT");
		}
		else
		{
			xyCur = tv.m_v2;
			Dom.SetDirection("RIGHT");
		}
//		Dom.SetDimExo(0);
//		Dom.SetDimObq(20);
		Dom.DimMoveTo(xyCur.x, xyCur.y, 0);
		strUp = bInput ? _T("TP") : COMMA(m_dThickPaveFootway);
		strLow = bInput ? COMMA(m_dThickPaveFootway) : _T("");
		Dom.DimLineToExtend(m_dThickPaveFootway, 0, strUp, strLow);// ���� �β� ġ��
	}

	*pDomP << Dom;
}

long CGuardWallRC::GetNumPave(CString strName)
{
	long nSize = m_sArrTypePave.GetSize();
	CString str = _T("");
	long i=0; for(i=0; i<nSize; i++)
	{
		str = m_sArrTypePave.GetAt(i);
		if(str==strName) return i;
	}

	return -1;
}

long CGuardWallRC::GetNumInnerFill(CString strName)
{
	long nSize = m_sArrInnerFill.GetSize();
	CString str = _T("");
	long i=0; for(i=0; i<nSize; i++)
	{
		str = m_sArrInnerFill.GetAt(i);
		if(str==strName) return i;
	}

	return -1;
}

CString CGuardWallRC::GetStringInnerFill(long nIndex)
{
	if(nIndex<0 || nIndex>=m_sArrInnerFill.GetSize()) return _T("");

	return m_sArrInnerFill.GetAt(nIndex);
}

CString CGuardWallRC::GetStringPave(long nIndex)
{
	if(nIndex<0 || nIndex>=m_sArrTypePave.GetSize()) return _T("");

	return m_sArrTypePave.GetAt(nIndex);
}
// ����, ��ȣ���� ��� ��縦 ǥ��(ȭ��ǥ �� �ؽ�Ʈ)
//xy   : ȭ��ǥ�� ����
//vDir : ���� 
void CGuardWallRC::ArrowDirectionForSlope(CDomyun *pDomP, CDPoint xy, CString str, CDPoint vDir)
{
	CDomyun Dom(pDomP);

	double dTextLenght = Dom.GetTextWidth(str,1);
	if(vDir.GetAngleDegree()<90 || vDir.GetAngleDegree()>270)//ȭ��ǥ�� ������ �����϶�
	{
		CDPoint xyArrow = xy + vDir*(dTextLenght + Dom.Always(2));

		Dom.SetTextAlignHorz(TA_CENTER);
		CDPoint xyText =  xy + vDir*Dom.Always(1)+vDir.Rotate90()*Dom.Always(1);
		xyText += vDir * dTextLenght / 2;
		Dom.TextOut(xyText, str);
		Dom.Rotate(xyText, vDir);
		*pDomP << Dom;

		Dom.LineTo(xy, xyArrow);
		CDPoint xy1 = xyArrow+vDir.Rotate90()*Dom.Always(0.5);
		CDPoint xy2 = xyArrow+vDir*Dom.Always(3);
		CDPoint xy3 = xyArrow+vDir.RotateInv90()*Dom.Always(0.5);
		Dom.Solid(xy1.x, xy1.y, xy2.x, xy2.y, xy3.x, xy3.y, xyArrow.x, xyArrow.y);
	}
	else
	{
		CDPoint xyArrow = xy + vDir*(dTextLenght + Dom.Always(2));

		Dom.SetTextAlignHorz(TA_CENTER);
		CDPoint xyText =  xy + vDir*(dTextLenght+Dom.Always(1))+vDir.RotateInv90()*Dom.Always(1);
		xyText += vDir * -dTextLenght / 2;
		Dom.TextOut(xyText, str);
		double dAng = vDir.GetAngleDegree()-180;
		Dom.Rotate(xyText, ToDPointFrRadian(ToRadian(dAng)));
		*pDomP << Dom;

		Dom.LineTo(xy, xyArrow);
		CDPoint xy1 = xyArrow+vDir.RotateInv90()*Dom.Always(0.5);
		CDPoint xy2 = xyArrow+vDir*Dom.Always(3);
		CDPoint xy3 = xyArrow+vDir.Rotate90()*Dom.Always(0.5);
		Dom.Solid(xy1.x, xy1.y, xy2.x, xy2.y, xy3.x, xy3.y, xyArrow.x, xyArrow.y);
	}
	*pDomP << Dom;
}


CString CGuardWallRC::GetStringDia(double dDia)
{
	CString sFy = GetCharRebarMark(m_dFy);
	if(dDia==0) return _T("");
	
	return sFy+Comma(dDia);
}

// ���� ��ũ��Ʈ ����?
// ����, ������ ��輮
BOOL CGuardWallRC::IsTypeHDanNonRebarConcrete()
{
	if(m_bRailRoad)
	{
		// (ARCBRIDGE-3243) ������ ��輮�� ��� ����. //ö�����϶��� �����ϵ��� ���� 
		if(m_nTypeGuardWall == HDAN_TYPE_ROAD_FOOTWAY) 
			return TRUE;		
	}
	else
	{
		if(m_nTypeGuardWall == HDAN_TYPE_ROAD_FOOTWAY || m_nTypeGuardWall == HDAN_TYPE_RAIL_FOOTWAYBLOCK ||
			m_nTypeGuardWall == HDAN_TYPE_ROAD_FOOTWAYBLOCK) return TRUE;		
	}

	return FALSE;
}

// ��Ǫ���� ���� �ֵ�����?
// ��ȣ��, �ߺд�, ������ ��輮
BOOL CGuardWallRC::IsForm()
{
	return !IsTypeHDanNonRebarConcrete();
}
