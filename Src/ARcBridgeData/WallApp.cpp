// WallApp.cpp: implementation of the CWallApp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WallApp.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWallApp::CWallApp()
{

}

CWallApp::~CWallApp()
{

}

// ��ü ������ 
BOOL CWallApp::GetTvFront(CTwinVectorArray &tvArrVertSection, BOOL bApplyAngle)
{
	m_tvArrFront.RemoveAll();
	if(!m_bIs) return TRUE;

	double dAngle	= bApplyAngle ? m_dAnglePath : 90;
	double rad		= sin(ToRadian(dAngle));
	CDPoint vAngUpper	= ToDPointFrDegree(m_dAngleUpperWall_Front);

	m_tvArrFront.m_sInfo = "���κ�ü";
	CTwinVector tv;
	CTwinVector tmp;

	// �Է¹��� EL�� ���� ���̸� �����ؼ� �׷��ش�.
	double dELB	= GetELBottomOfWall();

	// ������
	if(m_nType == WALL_TYPE_VER)
	{
		BOOL bBoxType				= m_tvLowerLimitOfMainRebar.GetLength() >0;

		tv.m_bApplyAngle	= bApplyAngle;
		tv.m_dAngle			= dAngle;
		tv.m_v1				= tv.m_v2 = CVector(m_xyOrg.x, dELB, 0);
		tv.m_v2.x			+= m_dW/2/rad;
		tv.m_sInfo			= "��ü�ϸ����";
		tv.val.valHor		= NULL;
		tv.val.valVer		= &m_dELB;
		tv.val.bRevMoveVer	= FALSE;
		m_tvArrFront.Add(tv);

		tv.m_bApplyAngle	= bApplyAngle;
		tv.m_dAngle			= 90;
		tv.m_v1				= tv.m_v2;
		tv.m_v2.y			= m_xyOrg.y + (bBoxType ? m_dDiffTop[1] : 0);
		tv.m_sInfo			= "��ü������";
		tv.val.valHor		= &m_dW;
		tv.val.valVer		= NULL;
		tv.val.bRevMoveHor	= FALSE;
		m_tvArrFront.Add(tv);

		tv.m_bApplyAngle	= bApplyAngle;
		tv.m_dAngle			= dAngle;
		tv.m_v1				= tv.m_v2 = CVector(m_xyOrg.x, dELB, 0);
		tv.m_v2.x			-= m_dW/2/rad;
		tv.m_sInfo			= "��ü�ϸ�����";
		tv.val.valHor		= NULL;
		tv.val.valVer		= &m_dELB;
		tv.val.bRevMoveVer	= FALSE;
		m_tvArrFront.Add(tv);

		tv.m_bApplyAngle	= bApplyAngle;
		tv.m_dAngle			= 90;
		tv.m_v1				= tv.m_v2;
		tv.m_v2.y			= m_xyOrg.y + (bBoxType ? m_dDiffTop[0] : 0);
		tv.m_sInfo			= "��ü������";
		tv.val.valHor		= &m_dW;
		tv.val.valVer		= NULL;
		tv.val.bRevMoveHor	= TRUE;
		m_tvArrFront.Add(tv);

		tv.m_bApplyAngle	= bApplyAngle;
		tv.m_dAngle			= dAngle;
		tv.m_v1				= tv.m_v2;
		tv.m_v2.x			+= m_dW/rad;
		if(bBoxType) 
			tv.m_v2.y			+= (m_dDiffTop[1] - m_dDiffTop[0]);

		tv.m_sInfo			= "��ü���";
		tv.val.valHor		= NULL;
		tv.val.valVer		= NULL;
		m_tvArrFront.Add(tv);
	}
	// �����
	else if(m_nType == WALL_TYPE_SLOPE)
	{
		double dAngle	= GetAngleWallByHor(bApplyAngle);
		CDPoint xyDir = ToDPointFrRadian(ToRadian(dAngle));
		CDPoint xyDirLower = CDPoint(1, 0);
		CDPoint xyCenLower(0, 0);
		CDPoint xySideUpper(0, 0), xySideLower(0, 0);

		// ��ü�ϸ� ����
		// ���ʿ� ��簡 �������� ����, ��簡 �������� ��翡 ���缭 ���� ������.
		// �׷��� ����ó�� ���ϸ� �������� ����
		if(m_bApplyAngleEnd)
		{
			if(Compare(dAngle, 90., "="))
			{
				xyDirLower = CDPoint(1, 0);
			}
			if(dAngle > 90)
			{
				xyDirLower = m_footing.GetXyDirSlope(TRUE, bApplyAngle);
			}
			else
			{
				xyDirLower = m_footing.GetXyDirSlope(FALSE, bApplyAngle);
			}
			
		}
		else
		{
			xyDirLower = CDPoint(1, 0);
		}
		
		// ����ϴ� �߽�
		GetXyMatchLineAndLine(m_xyOrg, xyDir, CDPoint(m_xyOrg.x, dELB), CDPoint(1, 0), xyCenLower);

		tv.m_bApplyAngle	= bApplyAngle;
		tv.m_dAngle			= dAngle;
		tv.m_v1				= tv.m_v2 = xyCenLower;
		xySideUpper			= m_xyOrg;
		xySideLower			= xyCenLower;
		// ��� ������(offset)
		Offset(xySideLower, xySideUpper, m_dW/2/rad);
		GetXyMatchLineAndLine(xyCenLower, xyDirLower, xySideUpper, xyDir, xySideLower);
		tv.m_v2				= xySideLower;
		tv.m_sInfo			= "��ü�ϸ����";
		tv.val.valHor		= NULL;
		tv.val.valVer		= &m_dELB;
		tv.val.bRevMoveVer	= FALSE;
		m_tvArrFront.Add(tv);

		tv.m_bApplyAngle	= bApplyAngle;
		tv.m_dAngle			= 90;
		tv.m_v1				= tv.m_v2;
		GetXyMatchLineAndLine(xySideLower, xyDir, m_xyOrg, vAngUpper, xySideUpper);
		tv.m_v2				= xySideUpper;
		tv.m_sInfo			= "��ü������";
		tv.val.valHor		= &m_dW;
		tv.val.valVer		= NULL;
		tv.val.bRevMoveHor	= FALSE;
		m_tvArrFront.Add(tv);

		tv.m_bApplyAngle	= bApplyAngle;
		tv.m_dAngle			= dAngle;
		tv.m_v1				= tv.m_v2 = xyCenLower;
		xySideUpper			= m_xyOrg;
		xySideLower			= xyCenLower;
		// ��� ������(offset)
		Offset(xySideLower, xySideUpper, -m_dW/2/rad);
		GetXyMatchLineAndLine(xyCenLower, -xyDirLower, xySideUpper, xyDir, xySideLower);
		tv.m_v2				= xySideLower;
		tv.m_sInfo			= "��ü�ϸ�����";
		tv.val.valHor		= NULL;
		tv.val.valVer		= &m_dELB;
		tv.val.bRevMoveVer	= FALSE;
		m_tvArrFront.Add(tv);

		tv.m_bApplyAngle	= bApplyAngle;
		tv.m_dAngle			= 90;
		tv.m_v1				= tv.m_v2;
		GetXyMatchLineAndLine(xySideLower, -xyDir, m_xyOrg, vAngUpper, xySideUpper);
		tv.m_v2				= xySideUpper;
		tv.m_sInfo			= "��ü������";
		tv.val.valHor		= &m_dW;
		tv.val.valVer		= NULL;
		tv.val.bRevMoveHor	= TRUE;
		m_tvArrFront.Add(tv);

		tv.m_bApplyAngle	= bApplyAngle;
		tv.m_dAngle			= dAngle;
		tv.m_v1				= tv.m_v2;
		m_tvArrFront.GetTvByInfo("��ü������", tmp);
		tv.m_v2				= Compare(tmp.m_v1.y, tmp.m_v2.y, ">") ? tmp.m_v1 : tmp.m_v2;
		tv.m_sInfo			= "��ü���";
		tv.val.valHor		= NULL;
		tv.val.valVer		= NULL;
		m_tvArrFront.Add(tv);
	}
	// V��
	else if(m_nType == WALL_TYPE_V)
	{
		CDPoint xyCenV		= CDPoint(m_xyOrg.x, m_xyOrg.y-m_dH);
		CDPoint xyDirRight	= ToDPointFrRadian(ToRadian(90-m_dAngleV/2));
		CDPoint xyDirLeft	= ToDPointFrRadian(ToRadian(90+m_dAngleV/2));
		CDPoint xyRightUpper(0, 0), xyLeftUpper(0, 0);
		CDPoint xyUpper(0, 0), xyLower(0, 0);
		// ������ �� �߽ɼ� top
		GetXyMatchLineAndLine(xyCenV, xyDirRight, CDPoint(0, m_dELRT), CDPoint(1, 0), xyRightUpper);
		// ���� �� �߽ɼ� top
		GetXyMatchLineAndLine(xyCenV, xyDirLeft, CDPoint(0, m_dELLT), CDPoint(1, 0), xyLeftUpper);

		tv.m_bApplyAngle	= bApplyAngle;
		tv.m_dAngle			= dAngle;
		tv.m_v1				= tv.m_v2 = CVector(m_xyOrg.x, dELB, 0);
		tv.m_v2.x			+= m_dW/2/rad;
		tv.m_sInfo			= "��ü�ϸ����";
		tv.val.valHor		= NULL;
		tv.val.valVer		= &m_dELB;
		tv.val.bRevMoveVer	= FALSE;
		m_tvArrFront.Add(tv);

		tv.m_bApplyAngle	= bApplyAngle;
		tv.m_dAngle			= 0;
		tv.m_v1				= tv.m_v2;
		xyUpper				= xyRightUpper; 
		xyLower				= xyCenV;
		Offset(xyLower, xyUpper, m_dWR/2/rad);
		GetXyMatchLineAndLine(xyUpper, -xyDirRight, tv.m_v1, CDPoint(0, 1), xyLower);
		tv.m_v2				= xyLower;
		tv.m_sInfo			= "��ü������";
		tv.val.valHor		= &m_dW;
		tv.val.valVer		= &m_dH;
		tv.val.bRevMoveHor	= FALSE;
		tv.val.bRevMoveVer	= TRUE;
		m_tvArrFront.Add(tv);

		tv.m_bApplyAngle	= bApplyAngle;
		tv.m_dAngle			= 90;
		tv.m_v1				= tv.m_v2;
		GetXyMatchLineAndLine(tv.m_v1, xyDirRight, xyRightUpper, CDPoint(1, 0), xyUpper);
		tv.m_v2				= xyUpper;
		tv.m_sInfo			= "��ü�����ȿ�����";
		tv.val.valHor		= &m_dWR;
		tv.val.valVer		= NULL;
		tv.val.bRevMoveHor	= FALSE;
		m_tvArrFront.Add(tv);

		tv.m_bApplyAngle	= bApplyAngle;
		tv.m_dAngle			= dAngle;
		tv.m_v1				= tv.m_v2;
		xyUpper				= xyRightUpper;
		xyLower				= xyCenV;
		Offset(xyLower, xyUpper, -m_dWR/2/rad);
		GetXyMatchLineAndLine(xyLower, xyDirRight, xyRightUpper, CDPoint(-1, 0), xyUpper);
		tv.m_v2				= xyUpper;
		tv.m_sInfo			= "��ü�����Ȼ��";
		tv.val.valHor		= NULL;
		tv.val.valVer		= NULL;
		m_tvArrFront.Add(tv);

		tv.m_bApplyAngle	= bApplyAngle;
		tv.m_dAngle			= 90;
		tv.m_v1				= tv.m_v2;
		xyUpper				= xyLeftUpper;
		xyLower				= xyCenV;
		Offset(xyLower, xyUpper, m_dWL/2/rad);
		GetXyMatchLineAndLine(xyUpper, -xyDirLeft, tv.m_v1, -xyDirRight, xyLower);
		tv.m_v2				= xyLower;
		tv.m_sInfo			= "��ü������������";
		tv.val.valHor		= &m_dWR;
		tv.val.valVer		= NULL;
		tv.val.bRevMoveHor	= TRUE;
		m_tvArrFront.Add(tv);

		tv.m_bApplyAngle	= bApplyAngle;
		tv.m_dAngle			= dAngle;
		tv.m_v1				= tv.m_v2 = CVector(m_xyOrg.x, dELB, 0);
		tv.m_v2.x			-= m_dW/2/rad;
		tv.m_sInfo			= "��ü�ϸ�����";
		tv.val.valHor		= NULL;
		tv.val.valVer		= &m_dELB;
		tv.val.bRevMoveVer	= FALSE;
		m_tvArrFront.Add(tv);

		tv.m_bApplyAngle	= bApplyAngle;
		tv.m_dAngle			= 90;
		tv.m_v1				= tv.m_v2;
		xyUpper				= xyLeftUpper; 
		xyLower				= xyCenV;
		Offset(xyLower, xyUpper, -m_dWL/2/rad);
		GetXyMatchLineAndLine(xyUpper, -xyDirLeft, tv.m_v1, CDPoint(0, 1), xyLower);
		tv.m_v2				= xyLower;
		tv.m_sInfo			= "��ü������";
		tv.val.valHor		= NULL;
		tv.val.valVer		= &m_dH;
		tv.val.bRevMoveVer	= TRUE;
		m_tvArrFront.Add(tv);

		tv.m_bApplyAngle	= bApplyAngle;
		tv.m_dAngle			= 90;
		tv.m_v1				= tv.m_v2;
		GetXyMatchLineAndLine(tv.m_v1, xyDirLeft, xyLeftUpper, CDPoint(-1, 0), xyUpper);
		tv.m_v2				= xyUpper;
		tv.m_sInfo			= "��ü������������";
		tv.val.valHor		= &m_dWL;
		tv.val.valVer		= NULL;
		tv.val.bRevMoveHor	= TRUE;
		m_tvArrFront.Add(tv);

		tv.m_bApplyAngle	= bApplyAngle;
		tv.m_dAngle			= dAngle;
		tv.m_v1				= tv.m_v2;
		xyUpper				= xyLeftUpper;
		xyLower				= xyCenV;
		Offset(xyLower, xyUpper, m_dWL/2/rad);
		GetXyMatchLineAndLine(xyLower, xyDirLeft, xyLeftUpper, CDPoint(1, 0), xyUpper);
		tv.m_v2				= xyUpper;
		tv.m_sInfo			= "��ü�����Ȼ��";
		tv.val.valHor		= NULL;
		tv.val.valVer		= NULL;
		m_tvArrFront.Add(tv);

		tv.m_bApplyAngle	= bApplyAngle;
		tv.m_dAngle			= 90;
		tv.m_v1				= tv.m_v2;
		m_tvArrFront.GetTvByInfo("��ü������������", tmp);
		tv.m_v2				= tmp.m_v2;
		tv.m_sInfo			= "��ü�����ȿ�����";
		tv.val.valHor		= &m_dWL;
		tv.val.valVer		= NULL;
		tv.val.bRevMoveHor	= FALSE;
		m_tvArrFront.Add(tv);
	}

	// ��ս��ϰ�� ����� ������ǥ�� �ѹ� �����ֶ�.. ����
	if(m_bTopIsColumn)
		m_Column.GetTvFront(TRUE, FALSE, ToDPointFrDegree(dAngle));

	// ���鿡 ���� ����ȭ ////////////////////////////////////////////////////////////////
	if(m_nType == WALL_TYPE_SLOPE && !IsVertWall())
	{
		//CDPoint vAngJijum	= ToDPointFrDegree(m_dAnglePath);
		double dAngle	= GetAngleWallByHor(bApplyAngle);

		CDPoint xyWallCenter(0, 0);
		CDPoint xyDir = ToDPointFrRadian(ToRadian(180+dAngle));
		GetXyMatchLineAndLine(m_xyOrg, xyDir, CDPoint(m_xyOrg.x, dELB), CDPoint(1, 0), xyWallCenter);

		CDPoint xyDirSlope = m_footing.GetXyDirSlope(m_footing.m_nType == FOOTING_TYPE_LEFTSLOPE, bApplyAngle);
		//double dLenSlope = sqrt((m_footing.m_dFW/vAngJijum.y*m_footing.m_dFW/vAngJijum.y)+m_footing.m_dFH*m_footing.m_dFH);
		
		CTwinVector tvLeft	= m_tvArrFront.GetTvByInfo("��ü������");
		CTwinVector tvRight	= m_tvArrFront.GetTvByInfo("��ü������");
		m_footing.m_dML	= ~(CDPoint(tvLeft.m_v1)-CDPoint(tvRight.m_v1))/2;
		m_footing.m_dMR	= m_footing.m_dML;

		if(m_footing.m_nType == FOOTING_TYPE_LEFTSLOPE)
			xyWallCenter	= xyWallCenter + (xyDirSlope * (m_footing.m_dMR + m_footing.m_dFWR));
		else
			xyWallCenter	= xyWallCenter + (xyDirSlope * (m_footing.m_dML + m_footing.m_dFWL));
		//xyWallCenter = xyWallCenter+(xyDirSlope*dLenSlope/2);
		m_footing.m_dSta  = xyWallCenter.x;
		m_footing.m_dEL = xyWallCenter.y + m_dELBDiff;

	}
	else
	{
		m_dAngle	= 90-GetAngleUpperSlab();
		m_footing.m_dFH	= m_footing.GetHeight();
		// ���� ������ �ֹٲ���????
		// �ϴ� Box�϶��� �ٲ��� �ʵ��� ����.
		if(m_footing.m_nType != FOOTING_TYPE_BOX)
			m_footing.m_nType = FOOTING_TYPE_BASIC_BOTH;
		m_footing.m_dML = m_dW/2;
		m_footing.m_dMR = m_dW/2;
		m_footing.m_dSta = m_xyOrg.x;
		m_footing.m_dEL = m_dELB + m_footing.GetHeight();
	}
	m_footing.m_dELDiff	= m_dELBDiff;

	// �Ϻ� �Ѱ谡 �ִٸ� �Ϻ� �Ѱ������ �׸���. �Ϻ� �Ѱ躸�� ��ü�� �ؿ� �ִٸ� ������ ���ش�. �Ѥ�;
	if(m_tvArrLowerLimit_Front.GetSize() > 0)
	{
		long nIdxLeft	= m_tvArrFront.GetIdxByInfo("��ü������"); 
		long nIdxRight	= m_tvArrFront.GetIdxByInfo("��ü������");
		CTwinVector tvLeft;
		CTwinVector tvRight;
		if(nIdxLeft > -1) tvLeft	= m_tvArrFront.GetAt(nIdxLeft);
		if(nIdxRight > -1) tvRight	= m_tvArrFront.GetAt(nIdxRight);
		CDPointArray xyArrMatch;
		CRebarPlacing rb;

		if(nIdxLeft > -1)
		{
			rb.GetXyMatchTvArrAndLine(m_tvArrLowerLimit_Front, tvLeft.m_v1, tvLeft.GetXyDir(), xyArrMatch);
			if(xyArrMatch.GetSize() > 0)
			{
				CDPoint xyMatch	= xyArrMatch.GetAt(0);
				if(xyMatch.y > tvLeft.GetXyTop().y)
				{
					tvLeft.m_v1	= tvLeft.m_v2;
				}
				else
				{
					tvLeft.m_v1	= xyMatch;
				}
				
				m_tvArrFront.SetAt(nIdxLeft, tvLeft);
			}
		}

		if(nIdxRight > -1)
		{
			rb.GetXyMatchTvArrAndLine(m_tvArrLowerLimit_Front, tvRight.m_v1, tvRight.GetXyDir(), xyArrMatch);
			if(xyArrMatch.GetSize() > 0)
			{
				CDPoint xyMatch	= xyArrMatch.GetAt(0);
				if(xyMatch.y > tvRight.GetXyTop().y)
				{
					tvRight.m_v1	= tvRight.m_v2;
				}
				else
				{
					tvRight.m_v1	= xyMatch;
				}
			}
			m_tvArrFront.SetAt(nIdxRight, tvRight);
		}

		CDPoint xyLeft	= tvLeft.GetXyBottom();
		CDPoint xyRight	= tvRight.GetXyBottom();
		CDPoint xyMid	= (xyLeft + xyRight)/2;
		nIdxLeft	= m_tvArrFront.GetIdxByInfo("��ü�ϸ�����");
		nIdxRight	= m_tvArrFront.GetIdxByInfo("��ü�ϸ����");
	
		tvLeft.m_v1	= xyMid;
		tvLeft.m_v2	= xyLeft;
		tvLeft.m_sInfo	= "��ü�ϸ�����";
		tvRight.m_v1	= xyMid;
		tvRight.m_v2	= xyRight;
		tvRight.m_sInfo	= "��ü�ϸ����";
		m_tvArrFront.SetAt(nIdxLeft, tvLeft);
		m_tvArrFront.SetAt(nIdxRight, tvRight);
	}


	// ���� �¿��� ��ũ����
	if(m_bSideIsArc)
	{
		//3172
		CTwinVectorArray tvArrLeftSlab, tvArrRightSlab;
		CTwinVector tvLeftBottomSlab, tvRightBottomSlab;

		tvArrLeftSlab.RemoveAll();
		tvArrRightSlab.RemoveAll();
		
		for (long ix=0; ix < m_nCountColumn; ix++)
		{
			CString strLeft = _T(""), strRight = _T("");

			strLeft.Format(_T("%d��°������ġ������"), ix+1);
			strRight.Format(_T("%d��°������ġ������"), ix+1);
			
			tvLeftBottomSlab = tvArrVertSection.GetTvByInfo(strLeft);
			tvRightBottomSlab = tvArrVertSection.GetTvByInfo(strRight);

			tvArrLeftSlab.Add(tvLeftBottomSlab);
			tvArrRightSlab.Add(tvRightBottomSlab);
		}
		
		CTwinVector tvLeft	= m_tvArrFront.GetTvByInfo("��ü������");
		CTwinVector tvRight	= m_tvArrFront.GetTvByInfo("��ü������");
		if(tvLeft.GetVerLength() > 0)
		{
			// 31720
			//���� ���� ������ ó���ϱ� ���� ����Ʈ �񱳸� ���Ͽ� ���̰� 0�� ����� ���� ã�Ƽ� ����Ʈ�� ������
			if(tvArrLeftSlab.GetSize() >0)
			{
				long nCount =0;
				for (long ix =0; ix< tvArrLeftSlab.GetSize(); ix++)
				{
					CTwinVector tv;
					tv = tvArrLeftSlab.GetAt(ix);

					if(ComparePt(tv.m_v2, tvLeft.m_v2, _T("="), 0.001))
						nCount++;
				}
				//}

				if(nCount == 0)
				{
					for (long jx=0; jx < tvArrLeftSlab.GetSize(); jx++)
					{
						CTwinVector tvTmp;
						tvTmp = tvArrLeftSlab.GetAt(jx);

						if(Compare(tvTmp.m_v2.x, tvLeft.m_v2.x, _T("="), 0.001))
						{
							tvLeft.m_v2  = tvTmp.m_v2;
							break;
						}
					}
				}
			}			

			tvLeft.m_v1.z	= -1;
			tvLeft.m_v2.z	= tvLeft.GetVerLength()/2;
			tvLeft.m_xyRCenter	= tvLeft.GetXyMid();
			m_tvArrFront.SetTvByInfo("��ü������", tvLeft);
		}

		if(tvRight.GetVerLength() > 0)
		{
			//{31720 
			//���� ���� ������ ó���ϱ� ���� ����Ʈ �񱳸� ���Ͽ� ���̰� 0�� ����� ���� ã�Ƽ� ����Ʈ�� ������
			if(tvArrRightSlab.GetSize() > 0)
			{
				long nCount =0;								
				for (long ix =0; ix< tvArrRightSlab.GetSize(); ix++)
				{
					CTwinVector tv;
					tv = tvArrRightSlab.GetAt(ix);

					if(ComparePt(tv.m_v2, tvRight.m_v2, _T("="), 0.001))
						nCount++;
				}

				if(nCount == 0)
				{
					for (long jx=0; jx < tvArrRightSlab.GetSize(); jx++)
					{
						CTwinVector tvTmp;
						tvTmp = tvArrRightSlab.GetAt(jx);

						if(Compare(tvTmp.m_v2.x, tvRight.m_v2.x, _T("="), 0.001))
						{
							tvRight.m_v2  = tvTmp.m_v2;
							break;
						}
					}
				}
			}	
			

			tvRight.m_v1.z	= 1;
			tvRight.m_v2.z	= -tvRight.GetVerLength()/2;
			tvRight.m_xyRCenter	= tvRight.GetXyMid();
			m_tvArrFront.SetTvByInfo("��ü������", tvRight);
		}
	}
	//////////////////////////////////////////////////////////////// ���鿡 ���� ����ȭ //

	
	return TRUE;
}

// ��ü�� ����� �߽� ��ġ ã��
// bLeft�� V���϶� ���
CDPoint CWallApp::GetXyWallTopCen(BOOL bLeft)
{
	CTwinVector tv;

	//31720
	CTwinVectorArray tvArrVertSection;
	tvArrVertSection.RemoveAll();

	GetTvFront(tvArrVertSection, FALSE);
	if(m_nType == WALL_TYPE_V)
	{
		if(bLeft) m_tvArrFront.GetTvByInfo("��ü�����Ȼ��", tv);
		else m_tvArrFront.GetTvByInfo("��ü�����Ȼ��", tv);
	}
	else
	{
		m_tvArrFront.GetTvByInfo("��ü���", tv);
	}

	CDPoint xy1 = tv.m_v1;
	CDPoint xy2 = tv.m_v2;

	return (xy1+xy2)/2;
}

// ��ü�ϴ��� �������� ������
BOOL CWallApp::IsLeftSlope()
{
	double dAngle	= GetAngleWallByHor(TRUE);
	if(dAngle > 90) return FALSE;
	else return TRUE;
}

double CWallApp::GetWidthWall()
{
	return m_dW;
}

// ��ü �ܸ� ��ǥ ����.
BOOL CWallApp::GetTvPlan()
{
	m_tvArrPlan.RemoveAll();
	if(IsRoundRectWall())
	{
		m_Column.GetTvPlan();
		m_tvArrPlan	= m_Column.m_tvArrPlan[iOUTTER][iUPPER];
	}
	else
	{
		// ��¥�� ������ �׸��� ������ �Ǵϱ�.. ���� ������ ����
		// ��ġ�����ǥ ���Ҷ� �̿��ϱ⵵ �ؾ� �ǹǷ� �������� ��쵵 ��ü ��� ��ǥ�� �ʿ��� ��찡 ����.
		//if(!m_bIs) return FALSE;

		CTwinVector tv;
		CDPoint xyDir = GetXyDirTop();
		tv = GetTvWallTopCen();

		if(m_bTopIsColumn && m_nType == WALL_TYPE_VER)
		{
			if(m_bRevDirPlacingColumn) 
			{
				xyDir		= xyDir * -1;
				GetSwap(tv.m_v1, tv.m_v2);
			}
			double dDist	= 0;
			CDPoint xyCen	= tv.m_v1;
			
			long i = 0; for(i = 0; i < m_nCountColumn; i++)
			{
				dDist += m_dDistColumn[i];
				m_Column.m_xyCen = tv.m_v1+dDist*xyDir;
				m_Column.GetTvPlan();
				m_tvArrPlan.AddFromTvArray(m_Column.m_tvArrPlan[iOUTTER][iUPPER]);

				if(i == 0)
					xyCen	= m_Column.m_xyCen;
			}
			if(!m_bRevDirPlacingColumn)
				m_Column.m_xyCen	= xyCen;
		}
		else
		{
			// V���϶��� ������ ������ ��� ����.
			long n = m_nType == WALL_TYPE_V ? 3 : 1;
			BOOL bVertWall	= IsVertWall();
			if(!bVertWall) n += 1;
			CString str = _T("");
			double dW = GetWidthFrontLower();

			CTwinVector tvLeftUpper, tvLeftLower;
			long i = 0; for(i = 0; i < n; i++)
			{
				if(i == 0) str = "��ü";
				else
				{
					if(!bVertWall)
					{
						tv	= GetTvWallTopCen(FALSE);
						str	= "��ü�ϸ�";
					}
					else
					{
						if(i == 1) 
						{
							tv = GetTvWallTopCenOfTypeV(TRUE);
							str = "��ü������";
							dW = m_dWL;
						}
						else if(i == 2) 
						{
							tv = GetTvWallTopCenOfTypeV(FALSE);
							str = "��ü������";
							dW = m_dWR;
						}
					}
				}

				CTwinVector tvLeft = tv;
				CTwinVector tvRight = tv;

				Offset(tvLeft.m_v1, tvLeft.m_v2, -dW/2);
				Offset(tvRight.m_v1, tvRight.m_v2, dW/2);

				// �������� �ƴҶ�
				if(!m_bRAngRight)
				{
					CDPoint xyDirStt = m_vAngRight;
					// ���� ����
					GetXyMatchSegAndLine(tvLeft.m_v2, tvLeft.m_v1, tv.m_v1, xyDirStt, tvLeft.m_v1);
					//������ ����
					GetXyMatchSegAndLine(tvRight.m_v2, tvRight.m_v1, tv.m_v1, xyDirStt, tvRight.m_v1);
				}

				// �������� �ƴҶ�
				if(!m_bRAngLeft)
				{
					CDPoint xyDirEnd = m_vAngLeft;
					// ��������
					GetXyMatchSegAndLine(tvLeft.m_v1, tvLeft.m_v2, tv.m_v2, xyDirEnd, tvLeft.m_v2);
					// ������ ����
					GetXyMatchSegAndLine(tvRight.m_v1, tvRight.m_v2, tv.m_v2, xyDirEnd, tvRight.m_v2);
				}

				tv.m_v1 = tvLeft.m_v1;
				tv.m_v2 = tvRight.m_v1;
				tv.m_sInfo = str+"����ϸ�";
				m_tvArrPlan.Add(tv);
				
				tv.m_v1 = tv.m_v2;
				tv.m_v2 = tvRight.m_v2;
				tv.m_sInfo = str+"������";
				m_tvArrPlan.Add(tv);

				tv.m_v1 = tv.m_v2;
				tv.m_v2 = tvLeft.m_v2;
				tv.m_sInfo = str+"�����";
				m_tvArrPlan.Add(tv);

				tv.m_v1 = tv.m_v2;
				tv.m_v2 = tvLeft.m_v1;
				tv.m_sInfo = str+"�������";
				m_tvArrPlan.Add(tv);

				if(!bVertWall)
				{
					if(i == 0)
						tvLeftUpper	 = tv;
					else 
						tvLeftLower	= tv;
				}
			}

			if(!bVertWall)
			{
				tv.m_v1	= tvLeftUpper.m_v1;
				tv.m_v2	= tvLeftLower.m_v1;
				tv.m_sInfo	= "";
				m_tvArrPlan.Add(tv);

				tv.m_v1	= tvLeftUpper.m_v2;
				tv.m_v2	= tvLeftLower.m_v2;
				tv.m_sInfo	= "";
				m_tvArrPlan.Add(tv);
			}
		}
	}

	// ��鿡 ���� ����ȭ ////////////////////////////////////////////////////////////////
	m_footing.m_tvPathPlan	= GetTvWallTopCen(FALSE);//m_tvPathPlan;

	// �����鿡 ���� ����ȭ
	CTwinVectorArray tvArrWallLeft;
	GetTvLeftSide(tvArrWallLeft, TRUE, FALSE);
	tvArrWallLeft.GetTvByInfo("��ü�ϸ�", m_footing.m_tvPathLeft);

	GetTvLeftSide(tvArrWallLeft, TRUE, FALSE, FALSE, FALSE);
	tvArrWallLeft.GetTvByInfo("��ü�ϸ�", m_footing.m_tvPathRight);

	m_footing.m_dCenterOfWall	= m_dCenterOfWall;
	
	// ���ʿ� ���������Ҷ� �������� �� Path�� ���ۺο� ������ ���ؼ��� ��������.
	m_tvArrPlan.GetTvByInfo("��ü����ϸ�", m_footing.m_tvRightAngStt);
	m_tvArrPlan.GetTvByInfo("��ü�����", m_footing.m_tvRightAngEnd);
	//////////////////////////////////////////////////////////////// ��鿡 ���� ����ȭ //

	return TRUE;
}

// ��ü�� �������� ��ġ�� �߽���ǥ ����
CTwinVector CWallApp::GetTvWallTopCenOfTypeV(BOOL bLeft)
{
	CTwinVector tv;
	CTwinVector tv1;
	CString str = bLeft ? "��ü�����Ȼ��" : "��ü�����Ȼ��";
	m_tvArrFront.GetTvByInfo(str, tv);
	m_tvArrFront.GetTvByInfo("��ü�����ȿ�����", tv1);

	// �߽ɰ� �� �Ȱ��� �Ÿ�
	double dOffset = tv1.m_v2.x - tv.GetXyMid().x;
	
	// ��ü �߽���ǥ���� �Ÿ��� offset
	CTwinVector tvPath = GetTvWallTopCen();
	CDPoint xy1 = tvPath.m_v1;
	CDPoint xy2 = tvPath.m_v2;
	Offset(xy1, xy2, dOffset);

	tv.m_v1 = xy1;
	tv.m_v2 = xy2;
	
	return tv;
}

CDPoint CWallApp::GetXyDirTop()
{
	return m_tvPathPlan.GetXyDir();
}

// ��� ���� �ǰ��
// �¿� ��Ī���� ���� ��.
// �⺻ ���� �ް� ���� �޾Ƽ� �ڵ����� ���� �� ��.
// BOOL bDefaultCountColumn : ��հ����� �ǰ�� ����..?
void CWallApp::SetDataDefaultDistColumn(BOOL bDefaultCountColumn)
{
	if(m_nCountColumn <= 0 && bDefaultCountColumn==FALSE) return; // bDefaultCountColumn�ε� ���Ͻ�Ű�� ��ü���� ������� ������ ��� ���� �ǰ���� �ȵ�.

	CTwinVector tvPath = GetTvWallTopCen();
	CRebarPlacing rb;
	CDPointArray xyArrDist;
	
	// ��հ��� �ǰ��
	// �⺻���� 3000�� �������� ��հ��� ã��
	if(bDefaultCountColumn)
	{
		m_nCountColumn = (long)(tvPath.GetLength() / 3000)-1;
	}

	long nCount = m_nCountColumn * 2;
	double dDist = tvPath.GetLength() / nCount;

	// ������ 100������ �ڸ�
	dDist = (long)(dDist / 100) * 100;

	// ù���� ���� ����
	double dOffsetStt = tvPath.GetLength() - (dDist * (nCount-1));
	if(dOffsetStt > dDist)
		dOffsetStt -= (dOffsetStt-dDist)/2;
	
	// ������ ������ �Է�
	m_dDistColumn[0] = dOffsetStt;
	long i = 0; for(i = 1; i < m_nCountColumn; i++)
		m_dDistColumn[i] = dDist*2;
}

// ��ü�� �߽ɼ��� ����
// ��ü ���� ���� ����
CTwinVector CWallApp::GetTvWallTopCen(BOOL bUpper)
{
	CTwinVector tv = bUpper ? m_tvPathPlan : m_tvPathPlanLower;
	CDPoint xyDir = GetXyDirTop();
	tv.m_v1 += xyDir * m_dMarginRight;
	tv.m_v2 -= xyDir * m_dMarginLeft;

	return tv;
}

// ���ʿ� ���� �ǰ��
BOOL CWallApp::SetDataDefaultShapeFooting()
{
	/*
	if(!m_footing.m_bIs) return TRUE;	// ���ʰ� ���� ����� ���ʰ� �ʱ�ȭ �ϰ� ����.
	
	double dW	= m_dW;
	
	// * ���� ������ ��(B)(0.6H <= B <= 0.9H) (RcBridge������ 1�� ����)
	// 1. ������ : 3.5 + 0.8 * (H - 5.0)
	// 2. ������ : 3.5 + 0.7 * (H - 5.0)
	double dB		= 3.5 + 0.8 * (dH - 5.0);
	if(dB < 0.6 * dH) dB	= 0.6 * dH;
	if(dB > 0.9 * dH) dB	= 0.9 * dH;

	// * ���� �ձ��� ���� (H : ��ü ����, B : ���� �ʺ�, tW : ��ü �ʺ�) - RcBridge������ 1�� ����
	// 1. ��Ż�鿡 ���밡 ������� : H/6
	// 2. �����Ǵ� ���� ���鿡 ������ ���� �ʰų�, ���ұ��ʷ� �ϴ� ��� : B//3

	// * ���� �ޱ��� ����
	// 1. B - (�ձ� + tW)

	// * ������ �β�(�ּ� 0.8)
	// 1. (B/5 - 0.1) >= 0.8
	// 2. (B < 5) �϶��� 0.8

	if(m_nType == WALL_TYPE_SLOPE)
	{
		if(IsLeftSlope())
		{
			m_footing.m_nType = FOOTING_TYPE_RIGHTSLOPE;
			m_footing.m_dFW		= dW;
			m_footing.m_dFH		= dW;
			m_footing.m_dFSLW	= dW;
			m_footing.m_dFSRH	= dW;
		}
		else
		{
			m_footing.m_nType = FOOTING_TYPE_LEFTSLOPE;
			m_footing.m_dFW		= dW;
			m_footing.m_dFH		= dW;
			m_footing.m_dFSRW	= dW;
			m_footing.m_dFSLH	= dW;
		}
	}
	else
	{
		m_footing.m_nType = FOOTING_TYPE_BASIC_BOTH;
		m_footing.m_dFWR	= Round(dH / 6, -3);
		m_footing.m_dFWL	= Round(dH / 6, -3);
		m_footing.m_dFH		= (dB < 5000) ? 800 : Round(dB/5-100);
	}

	// ���� �ǰ���� �ϸ� Ȯ����� �ǰ�ȵ� �ؾ� �ȴ�.
	BOOL bResult = m_footing.SetDataDefaultShapeExFooting();
	*/

	return TRUE;
}

// ��ü�� ���� ����ȭ�� ������
// ����ȭ�� �Ϸ��� ��ǥ�� ���� ������ �־�� �Ѵ�.
// ��ǥ���ϴ� �Լ��� ���� �ѹ� ȣ������ �ڿ� �� ���� ��...
void CWallApp::SyncWall(CTwinVectorArray &tvArrVertSection, BOOL bApplyAngle, double dAngle)
{
	m_dAnglePath	= dAngle;

	// ��ս��� �ƴ����� ���� ���̸� ����� ���庮ü�� �°� ���� �� ���´�.
	if(IsRoundRectWall())
	{
		SetColumnForRoundRectWall();
	}
	if(!m_footing.IsVirtualFooting())
	{
		if(IsVertWall())
		{
			if(m_footing.m_nType != FOOTING_TYPE_BASIC_LEFTSLOPE && m_footing.m_nType != FOOTING_TYPE_BASIC_RIGHTSLOPE && m_footing.m_nType != FOOTING_TYPE_BOX)
				m_footing.m_nType	= FOOTING_TYPE_BASIC_BOTH;
		}
		else
		{
			m_footing.m_nType = IsLeftSlope() ? FOOTING_TYPE_RIGHTSLOPE : FOOTING_TYPE_LEFTSLOPE;
		}
	}

	GetTvFront(tvArrVertSection, bApplyAngle);
	GetTvPlan();

	// ��ü�� ������ ���ʵ� �翬�� ����.
	if(!m_bIs) m_footing.m_bIs = FALSE;

	m_footing.SyncFooting(bApplyAngle, dAngle);

	// ��ġ�� ���� ���ؼ� ��������
	CheckOverLapLineBetweenSubObject();

	// ��ٰ��� ����ȭ ////////////////////////////////////////////////////////////
	m_footing.m_bSameOffsetPlaceTAndCBlock	= m_bSameOffsetPlaceTAndCBlock;
	m_footing.m_bPlacing23DanByStandardOffset	= m_bPlacing23DanByStandardOffset;
	// ���� ����Ŭ ��
	m_footing.m_nIdxCountCycleMainRebar	= m_nIdxCountCycleMainRebar;
	m_footing.m_nIdxCTC_MainRebar		= m_nIdxCTC_MainRebar;

	// ��ս��ϰ�� ��� ���̴� ���ϸ� CTC�� ������ ���� �ȴ�. �� ������ ��������.
	m_footing.m_tvArrReverseCTCBlock_Main_Side.RemoveAll();
	if(m_bTopIsColumn)
	{
		CTwinVectorArray tvArrSide, tvArrHidden;
		m_footing.GetTvLeftSide(tvArrSide, tvArrHidden, TRUE);
		
		CTwinVector tvRange;
		CHgBaseConcStd baseConc;
		double dCTC	= 0;//baseConc.GetValueRebarCTC(m_nIdxCTC_MainRebar);
		long i = 0; for(i = 0; i < m_nCountColumn-1; i++)
		{
			CDPoint xyCenPrev	= GetXyCenterColumn_Left(i);
			CDPoint xyLeftPrev	= CDPoint(xyCenPrev.x-m_Column.GetWidth(TRUE)/2-dCTC, xyCenPrev.y);
			CDPoint xyRightPrev	= CDPoint(xyCenPrev.x+m_Column.GetWidth(TRUE)/2+dCTC, xyCenPrev.y);

			CDPoint xyCenNext	= GetXyCenterColumn_Left(i+1);
			CDPoint xyLeftNext	= CDPoint(xyCenNext.x-m_Column.GetWidth(TRUE)/2-dCTC, xyCenNext.y);
			CDPoint xyRightNext	= CDPoint(xyCenNext.x+m_Column.GetWidth(TRUE)/2+dCTC, xyCenNext.y);

			tvRange.m_v1		= xyRightPrev;
			tvRange.m_v2		= xyLeftNext;
			m_footing.m_tvArrReverseCTCBlock_Main_Side.Add(tvRange);
		}
	}

	// ����ȯ��
	SyncWall_DesignData();
	CTwinVector tvLower							= m_footing.GetTvOffset_Front(0, FALSE, m_footing.IsVertDir());
	CRebarPlacing rb;
	m_Column.m_dCTCMainRebar	= (long)(m_bApplyAngleToMainRebarCTC ? m_dCTC_MainRebar/sin(ToRadian(m_dAnglePath)) : m_dCTC_MainRebar);
	// ����� ����Ŭ ������ �����Ƿ� ctc�� ������ �ؾߵ�.
	// ��, ���� ��ü�� �Էµ� ��ö�� ���̿� 2cycle�� �ڵ����� �־ ����ϹǷ� ctc�� ������ �ָ� �ȵ�.
	if(!IsRoundRectWall())
		m_Column.m_dCTCMainRebar	/= 2;
	m_Column.m_tvArrUpperLimitOfMainRebarFront.RemoveAll();
	m_Column.m_tvArrUpperLimitOfMainRebarFront.Add(m_tvUpperLimitOfMainRebar);
	m_Column.m_tvArrUpperLimitOfMainRebarSide	= m_tvArrLeftUpper;
	m_Column.m_tvArrLowerLimit_Front.RemoveAll();
	m_Column.m_tvArrLowerLimit_FrontRebar.RemoveAll();
	if(m_bTopIsColumn || IsRoundRectWall())
	{
		m_Column.m_tvArrLowerLimit_Front		= m_tvArrLowerLimit_Front;
		m_Column.m_tvArrLowerLimit_FrontRebar	= m_tvArrLowerLimit_FrontRebar;

		m_Column.m_dELB	= m_dELB;
		m_Column.m_dCH	= m_Column.m_dEL - m_Column.m_dELB - m_footing.GetHeight();
		// (38038) ������ GetTvFront(tvArrVertSection, bApplyAngle);�� �ؼ� ���� ����ؼ� ���ߴµ� ���⼭ ���������ϰ� �ٽ� ����?
//		m_Column.GetTvFront();
	}
	rb.MoveRebar(m_Column.m_tvArrUpperLimitOfMainRebarSide, CDPoint(0, -m_dCoverUpper_UpperSlab));
	m_Column.SyncColumn();
	m_Column.m_tvLowerLimitOfMainRebar			= (m_footing.m_bIs || m_footing.IsVirtualFooting())? tvLower : m_tvLowerLimitOfMainRebar;
	m_Column.m_footing							= m_footing;
	m_Column.m_footing.SyncFooting();

	//////////////////////////////////////////////////////////// ��ٰ��� ����ȭ //
}

// ��ü ���� ��ö�� ��ǥ ����(V�� ��ü ���)
// bArm : v���� ��� �Ⱥκ�����?(
// bLeftArm : �Ⱥκ��̶�� ���� ������?
// bLeftRebar : ���� ö������?
CTwinVector CWallApp::GetTvOffset_Front(long nCycle, long nDan, BOOL bArm, BOOL bLeftArm, BOOL bLeftRebar, BOOL bVertDir)
{
	CTwinVector tvWall;
	if(nCycle == -1) nCycle = GetCountCycleMainRebar()-1;
	if(nCycle < 0 || nCycle > GetCountCycleMainRebar()-1) return tvWall;
	if(nDan == -1) nDan = MAX_REBAR_DAN-1;
	if(nDan < 0 || nDan > MAX_REBAR_DAN-1) return tvWall;

	double dRad		= sin(ToRadian(bVertDir ? 90 : m_dAnglePath));
	double dCover	= bLeftRebar ? m_dCoverLeft[nDan]/dRad : -m_dCoverRight[nDan]/dRad;
	
	// V���̰� bArm�� ��츸 �ٸ��� ������ ����.
	if(bArm && m_nType == WALL_TYPE_V)
	{
		
	}
	else
	{
		CString sInfo	= bLeftRebar ? _T("��ü������") : _T("��ü������");
		if(m_tvArrFront.GetTvByInfo(sInfo, tvWall))
		{
			Offset(tvWall.m_v1, tvWall.m_v2, dCover);
		}
	}

	return tvWall;
}

// ��� ����Ŭ, ��� ���� rebar info cycle����� �Լ�
void CWallApp::MakeRebarInfoCycle(long nCycle, BOOL bLeft, long nDan, BOOL bInput, BOOL bMarkInitAtFirst, BOOL bVertDir)
{
	if(nCycle == -1) nCycle = GetCountCycleMainRebar()-1;
	if(nCycle < 0 || nCycle > GetCountCycleMainRebar()-1) return;
	if(nDan == -1) nDan = MAX_REBAR_DAN-1;
	if(nDan < 0 || nDan > MAX_REBAR_DAN-1) return;
	long left	= bLeft ? iLEFT : iRIGHT;

	CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC	= &m_pArrRCMain[nCycle][left][nDan];
	AhTPADelete(pArrRC, (CRebarInfoCycle*)0);
	if(m_bNotRebar) return;

	long nCycleReal	= nCycle;
	if(!m_bDetailInput23DanMainRebar)
	{
		if(nDan > 0)
			nCycle	= 1;
	}

// 	double dSettleOfFooting	 = 500;
	CDPoint xyDirLower	= bLeft ? CDPoint(-1, 0) : CDPoint(1, 0);
	if(m_tvLowerLimitOfMainRebar.GetLength() > 0)
		xyDirLower = m_tvLowerLimitOfMainRebar.GetXyDir(bLeft);

	long nCountDan	= GetDanMainRebar(nCycle, bLeft);
	if(nCountDan <= nDan) return;

	if(bMarkInitAtFirst)
		GetMarkRebarMain(bLeft ? MARK_LEFT : MARK_RIGHT, bInput, nCycle, TRUE, TRUE, _T(""), nDan);

	// �ϴ� ��ս��� rebar info cycle�� ������ ����
	if(m_bTopIsColumn) return;

	if(m_nType == WALL_TYPE_V)
	{
	}
	else
	{
		CTwinVector tvWall	= GetTvOffset_Front(nCycle, nDan, FALSE, FALSE, bLeft, bVertDir);
		//double sw			= bLeft ? -1 : 1;
		
		// ���� �ϸ�� ����(���ʰ� ������)
		if(m_footing.m_bIs || m_footing.IsVirtualFooting())
		{
			CDPoint xyMatch(0, 0);
			CTwinVector tvFooting = m_footing.GetTvOffset_Front(0, FALSE, bVertDir);
			tvFooting.m_v1.y	+= GAP_FOOTING_LOWER;
			tvFooting.m_v2.y	+= GAP_FOOTING_LOWER;
			if(GetXyMatchLineAndLine(tvWall.m_v1, tvWall.GetXyDir(TRUE), tvFooting.m_v1, tvFooting.GetXyDir(), xyMatch))
			{
				tvWall.m_v1	= xyMatch;
			}
		}
		// ���ʰ� ���ٸ� ��ü�Ϻ� �Ѱ踦 �̿��ؼ� ..
		else if(Compare(m_tvLowerLimitOfMainRebar.GetLength(), 0.0, ">"))
		{
			CDPoint xyMatch(0, 0);
			if(GetXyMatchLineAndLine(tvWall.m_v1, tvWall.GetXyDir(), m_tvLowerLimitOfMainRebar.m_v1, m_tvLowerLimitOfMainRebar.GetXyDir(), xyMatch))
			{
				tvWall.m_v1	= xyMatch;
			}
		}

		// ��ü ��� ����
		if(Compare(m_tvUpperLimitOfMainRebar.GetLength(), 0.0, ">"))
		{
			CDPoint xyMatch(0, 0);
			if(GetXyMatchLineAndLine(tvWall.m_v1, tvWall.GetXyDir(), m_tvUpperLimitOfMainRebar.m_v1, m_tvUpperLimitOfMainRebar.GetXyDir(), xyMatch))
			{
				tvWall.m_v2	= xyMatch;
			}
		}

		// ���ʰ� slope���ϴ� ��üö�� �ϴܺ� ���� ������ �������� ���ƾ� �ȴ�.
		/*
		if((m_footing.m_nType == FOOTING_TYPE_LEFTSLOPE && !bLeft) || (m_footing.m_nType == FOOTING_TYPE_RIGHTSLOPE && bLeft))
		{
			sw *= -1;
		}
		*/
	
		CHgBaseConcStd baseConc;

		
		long left		= bLeft ? iLEFT : iRIGHT;
		CDPoint xyStt	= CDPoint(tvWall.m_v1.x, tvWall.m_v1.y + m_footing.m_dCoverLower[0]);	// ������ġ
		CDPoint xyNextStt	= tvWall.m_v1;
		CDPoint xyDirWall	= tvWall.GetXyDir();

		double dLenJoint	= 0;
// 		double dLenJointLast	= 0;
		long nCountJoint	= nCycle == 0 || nCycle == 2 ? m_nCountJointMainRebar_Cycle1[left][m_bDetailInput23DanMainRebar ? nDan : 0] : m_nCountJointMainRebar_Cycle2[left][m_bDetailInput23DanMainRebar ? nDan : 0];

		for(long joint = 0; joint < nCountJoint+1; joint++)
		{
			double dist = GetDistMainRebar(nCycle, nDan, bLeft, joint, FALSE);
			double dLenJointLast	= dLenJoint;
			if(Compare(dist, 0.0, "=") && joint<nCountJoint) continue;
			if(nCountJoint == 0)
			{
				dist = tvWall.GetLength();
			}

			// ö�� ������ �����ڸ�ö�� ������ ���� ũ�� ////////////
			double dDia		= GetDiaMainRebar(nCycle, m_bDetailInput23DanMainRebar ? nDan : 0, bLeft, joint);
			double dDiaReal	= GetDiaMainRebar(nCycleReal, nDan, bLeft, joint);
			BOOL bExist		= IsExistMainRebar(nCycleReal, nDan, bLeft, joint);
			double dDiaNext	= GetDiaMainRebar(nCycle, m_bDetailInput23DanMainRebar ? nDan : 0, bLeft, joint+1);
			double dLenRound	= (joint == 0) ? GetLenRebarAtEdge(dDia, TRUE) : GetLenRebarAtEdge(dDia, FALSE);
			/////////////////////////////////////////////////////////////////

			pArrRC->Add(new CRebarInfoCycle);
			CRebarInfoCycle *pRC	= pArrRC->GetAt(pArrRC->GetSize()-1);

			// ����Ӽ�
			pRC->m_szMark		= bExist ? GetMarkRebarMain(bLeft ? MARK_LEFT : MARK_RIGHT, bInput, nCycle, FALSE, TRUE, _T(""), nDan) : "";
			pRC->m_nTypeRebar	= REBAR_TYPE_MAIN;
			pRC->m_nPosEnd		= bLeft ? REBARINFO_POS_INWALL_LEFT : REBARINFO_POS_INWALL_RIGHT;
			pRC->m_Dia			= dDia;
			pRC->m_bExist		= bExist;
			pRC->m_nRebarDan	= 0;
			
			// ������ġ�� �׻� ���ʿ��� �����Ѵ�.
			if(joint == 0)
			{
				dLenJoint = (nCountJoint == 0) ? 0 : GetValueJoint(pRC, dDiaNext);
				
				if(m_bRebarLowerSettle[left] == FALSE) // m_footing.m_bIs == FALSE && m_footing.IsVirtualFooting() == FALSE)//m_footing.m_nType == FOOTING_TYPE_BOX)
				{
					pRC->m_nPosStt	= bLeft ? REBARINFO_POS_INWALL_LEFT : REBARINFO_POS_INWALL_RIGHT;
					pRC->m_xyStt	= tvWall.m_v1;
					pRC->m_xyMid1	= pRC->m_xyStt + (xyDirWall * (dist + dLenJoint/2));
					pRC->m_xyMid2	= pRC->m_xyMid1;
					pRC->m_xyEnd	= pRC->m_xyMid2;
				}
				else
				{
					double dSettleOfFooting	 = 500;

					pRC->m_nPosStt	= REBARINFO_POS_FOOTING_LOWER;
					pRC->m_xyStt	= tvWall.m_v1 + xyDirLower * (dLenRound+dSettleOfFooting);//CDPoint(tvWall.m_v1.x + (dLenRound+dSettleOfFooting)*sw, tvWall.m_v1.y);
					GetXyTanOfArc(tvWall.m_v1, xyDirLower, tvWall.GetXyDir(), dLenRound, pRC->m_xyCen, pRC->m_xyMid1, pRC->m_xyMid2);
					pRC->m_xyEnd	= pRC->m_xyMid2 + (xyDirWall * (dist + dLenJoint/2));

					pRC->m_R_Mid	= dLenRound;
					pRC->m_bRevArc	= bLeft ? FALSE : TRUE;
				}
				
				pRC->m_Dia		= dDiaReal;
				
				if(joint == nCountJoint)
					pRC->m_xyEnd	= tvWall.m_v2;
				xyNextStt		= pRC->m_xyEnd - (dLenJoint * xyDirWall);
			}
			else
			{
				pRC->m_nPosStt	= pRC->m_nPosEnd;
				dLenJoint		= GetValueJoint(pRC, dDiaNext);
				pRC->m_Dia		= dDiaReal;

				pRC->m_xyStt	= xyNextStt;
				pRC->m_xyMid1	= pRC->m_xyStt + (xyDirWall * (dist + dLenJoint/2 + dLenJointLast/2));
				if(joint == nCountJoint)
					pRC->m_xyMid1	= tvWall.m_v2;
				pRC->m_xyMid2	= pRC->m_xyMid1;
				pRC->m_xyEnd	= pRC->m_xyMid1;
				pRC->m_xyCen	= pRC->m_xyMid1;
				pRC->m_R_Mid	= 0;
				xyNextStt		= pRC->m_xyMid1 - (dLenJoint * xyDirWall);
			}
		}
	}

	//2,3�� ö�� Ȯ����� ����
	if(nDan > 0 && m_bDetailInput23DanMainRebar == FALSE)
	{
		ApplyExtLength23DanMainRebar(pArrRC, GetLengthExt23DanRebar(bLeft), m_tvUpperLimitOfMainRebar);
	}

	// Not Exist�� rc ����
	/*
	if(!bInput)
		RemoveNotExistRebarInfoCycle(pArrRC);
		*/

	// sDescription ����
	SetStringDescriptionRebarInfoCycle(pArrRC, bLeft, nDan, nCycleReal);

	// ������ ö���� ���� ��ȭ ���� �ɼ� ��������
	if(pArrRC->GetSize() > 0)
	{
		CRebarInfoCycle *pRC	= pArrRC->GetAt(pArrRC->GetSize()-1);
		pRC->m_bVariableLength	= TRUE;
	}
}

// ö�� ���� ����
double CWallApp::GetDiaMainRebar(long nCycle, long nDan, BOOL bLeft, long nIdx)
{
	double dDia	= 0;
// 	long nSize	= 0;
	long left	= bLeft ? iLEFT : iRIGHT;
	

	if(nDan == 0 || m_bDetailInput23DanMainRebar)
	{
		long nSize	= m_dArrDiaRC[nCycle][left][nDan].GetSize();
		if(nIdx == -1) nIdx = nSize-1;

		if(nIdx < nSize)
			dDia	= m_dArrDiaRC[nCycle][left][nDan].GetAt(nIdx);
		else 
			dDia	= 0;
	}
	// 2�ܺ��ʹ� �ٸ� ������ ����Ǿ� �ִ�
	// 2���̻��� �׳� ���� �������� ������ ���еǾ� ����.
	// 2���� index 0�̰� 3���� 1�̴�.
	else if(nDan > 0)
	{
		if(nDan == -1) nDan = MAX_REBAR_DAN-1;
		if(nDan < 0) return 0;
		if(nDan > MAX_REBAR_DAN-1) return 0;
		if(nCycle == -1) nCycle = MAX_REBAR_CYCLE-1;
		if(nCycle < 0) return 0;
		if(nCycle > MAX_REBAR_CYCLE-1) return 0;

		CSetRebarDan *pRD	= &m_setRebarDan[left];
		if(pRD->m_nCountDan > nDan && GetCountCycleMainRebar() > nCycle)
		{
			dDia	= pRD->m_dDia[nCycle][nDan];
		}
		else 
			dDia	= 0;
	}

	return dDia;
}

// ö�� ���� ����
BOOL CWallApp::IsExistMainRebar(long nCycle, long nDan, BOOL bLeft, long nIdx)
{
	if(!m_bDetailInput23DanMainRebar && (nCycle == 0 || nCycle == 2)) return TRUE;

	// 4cycle�� �Է¹��� �����Ƿ� 2cycle���� ���
	if(nCycle == 3) 
		nCycle = 1;
	if(!m_bDetailInput23DanMainRebar)
	{
		nDan	= 0;
	}

	BOOL bExist	= FALSE;
	long nSize	= 0;
	long left	= bLeft ? iLEFT : iRIGHT;
	
	nSize	= m_dArrExistRC[nCycle][left][nDan].GetSize();
	if(nIdx == -1) nIdx = nSize-1;

	if(nIdx < nSize)
		bExist	= (HGBOOL)m_dArrExistRC[nCycle][left][nDan].GetAt(nIdx);
	else 
		bExist	= 0;

	return bExist;
}

// ö�� ��ġ �ܼ� ����
long CWallApp::GetDanMainRebar(long nCycle, BOOL bLeft)
{
	if(nCycle == -1) nCycle = GetCountCycleMainRebar()-1;
	if(nCycle < 0 || nCycle > GetCountCycleMainRebar()-1) return 0;

	long left	= bLeft ? iLEFT : iRIGHT;

	long nDan	= m_setRebarDan[left].m_nCountDan;

	if(m_bDetailInput23DanMainRebar)
	{
		nDan	= 1;
		long dan = 0; for(dan = 1; dan < MAX_REBAR_DAN; dan++)
		{
			BOOL bOKDia	= FALSE;
			BOOL bOKExist	= FALSE;
			long i = 0; for(i = 0; i < m_dArrDiaRC[nCycle][left][dan].GetSize(); i++)
			{
				if(m_dArrDiaRC[nCycle][left][dan].GetAt(i) > 0)
				{
					bOKDia	= TRUE;
					break;
				}
			}

			for(i = 0; i < m_dArrExistRC[nCycle][left][dan].GetSize(); i++)
			{
				if((BOOL)m_dArrExistRC[nCycle][left][dan].GetAt(i) == TRUE)
				{
					bOKExist	= TRUE;
					break;
				}
			}

			if(bOKDia && bOKExist)
			{
				nDan++;
			}
		}
	}
	return nDan;
}

// ����ö�� �����..
double CWallApp::GetLenRebarAtEdge(double dDia, BOOL bInFooting)
{
	if(!m_pBasicConcInfo) return 0;
	double dLen	= 0;
	if(m_nTypeCurveOfVertRebar == 0)		// �ڵ�
		dLen	= m_pBasicConcInfo->GetRebarInfo(dDia, BASIC_RADIUS);
	else if(m_nTypeCurveOfVertRebar == 1)	// �
		dLen	= m_pBasicConcInfo->GetRebarInfo(dDia, BASIC_RADIUS);
	else									// ����
		dLen	= 0;	

	if(bInFooting && m_nTypeCurveOfVertRebar == 0) 
	{
		if(Compare(dDia, 19.0, "<=")) dLen	= 0;
	}

	return dLen;
}

// BOOL bApplyRebarRadius : ��ö�� �߽ɿ��� ö�ٱ�ȣ������ ����ؼ� ��ġ�� ������?
void CWallApp::GetXyRebarSupport_Front(CDPointArray &xyArr, BOOL bLeft, long nDan, BOOL bApplyRebarRadius, BOOL bVertDir)
{
	xyArr.RemoveAll();
	if(m_bNotRebar) return;

	if(nDan == -1) nDan = MAX_REBAR_DAN-1;
	if(nDan < 0 || nDan > MAX_REBAR_DAN-1) return;

	long left	= bLeft ? iLEFT : iRIGHT;
	// 2��, 3���� �Է��� ���� �ʰ� 1���� 3������ ���� �������ʹ� 1�� 2���� �ϳ��� 2,3�ܿ� ����.
	CPlaceRebarByBlock *pPlace	= &m_placeByBlock_SupportRebar[left][0];

	// ��ü �¿��� �Ǻ�
	double dRadRebar		= bLeft ? REBAR_SYMBOL_RADIUS : -REBAR_SYMBOL_RADIUS;
	if(!bApplyRebarRadius) 
		dRadRebar	= 0;
	
	CRebarInfoCycle *pRC	= GetRebarInfoCycle(0, nDan, bLeft, 0);
	if(!pRC) return;

	CDPoint vAng	= GetTvOffset_Front(0, 0, TRUE, TRUE, bLeft, bVertDir).GetXyDir();

	CTwinVector tvWall;
	tvWall.m_v1	= GetXySttRebarSupport_Front(nDan, bLeft, bVertDir);
	if(pRC->GetLengthEnd() > 0)
		tvWall.m_v2	= tvWall.m_v1 + (vAng * pRC->GetLengthEnd());
	else
		tvWall.m_v2	= tvWall.m_v1 + (vAng * 500);

	// ö�� ��ǥ ã��
	CRebarPlacing rb;
	rb.SettingRadius(dRadRebar, TRUE, FALSE);
	rb.GetXyDivideLine(xyArr, tvWall.m_v1, tvWall.m_v2, pPlace->m_xyArr, FALSE, FALSE, FALSE);

	// 2�ܺ��ʹ� ������ ��ö���� ��ٵ� ������ ���ö���� ��ٵȴ�. 
	if(nDan > 0)
	{
		long nCountOffset	= m_bSameOffsetPlaceTAndCBlock ? 1 : 2;	// 2�� ��ٽ� 1��ö�� ����� �ϳ��� �������?
		
		// ���� ������ �ƴҶ��� ǥ�ذ����� 1/2�� ��ٵ� ���� �ִ�.
		// �� ������ m_bPlacing23DanByStandardOffset �ɼ� ���� ������
		if(!m_bSameOffsetPlaceTAndCBlock)
		{
			if(m_bPlacing23DanByStandardOffset == FALSE)// || GetCountCycleMainRebar() > 1)
			{
				nCountOffset	= 1;
			}
		}	

		// 2��, 3���� �Է��� ���� �ʰ� 1���� nCountSame������ ���� �������ʹ� 1�� 2���� �ϳ��� 2,3�ܿ� ����.
		long nCountSame	= 0;
		CDPointArray xyArrTmp;
		xyArrTmp		= xyArr;
		long nSize		= xyArrTmp.GetSize();
		xyArr.RemoveAll();
		if(nSize > nCountSame)
		{
			long i = 0; for(i = 0; i < nCountSame; i++)
				xyArr.Add(xyArrTmp.GetAt(i));

			for(i = nCountSame; i < nSize-(nCountOffset-1); i+=nCountOffset)
			{
				xyArr.Add(xyArrTmp.GetAt(i));
			}
		}

		CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC	= &m_pArrRCMain[0][left][nDan];
		CHgBaseConcStd baseConcStd;
		baseConcStd.GetXyRangeOfRebarInfoCycleArray(xyArr, pArrRC, FALSE);

		xyArr.Sort(FALSE);
	}
}

CRebarInfoCycle* CWallApp::GetRebarInfoCycle(long nCycle, long nDan, BOOL bLeft, long nIdx)
{
	if(nCycle == -1) nCycle = GetCountCycleMainRebar()-1;
	if(nCycle < 0 || nCycle > GetCountCycleMainRebar()-1) return NULL;
	if(nDan == -1) nDan = MAX_REBAR_DAN-1;
	if(nDan < 0 || nDan > MAX_REBAR_DAN-1) return NULL;

	// �ʿ��� RebarInfoCycle �迭�� ã��
	long left	= bLeft ? iLEFT : iRIGHT;
	
	CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC = &m_pArrRCMain[nCycle][left][nDan];

	long nSize	= pArrRC->GetSize();
	if(nSize == 0) return NULL;
	if(nIdx == -1) nIdx = nSize-1;
	if(nIdx >= nSize) return NULL;
	if(nIdx < 0) return NULL;

	return pArrRC->GetAt(nIdx);
	/*
	// �����͸� �Ѱܵ� ������ ������ �����ؾ� �޸� ������ �������� ������ ���Ƽ� �̷��� ��.
	*pRC	= *pArrRC->GetAt(nIdx);

	return TRUE;
	*/
}

// long nVertDir : -1 : ���Ÿ�, 0 : �ڵ�(��������), 1 : ��Ÿ�
void CWallApp::GetTvRebarShear_Front(CTwinVectorArray &tvArr, BOOL bVertDir)
{
	tvArr.RemoveAll();
	if(m_bNotRebar) return;
	
	// ��ս��϶� ����ö���� ������.. 
	if(m_bTopIsColumn) return;

	// ��ν����� ���ϸ� 1��ö�ٿ� ����ö�� �����.
	// ��.. ����ö���� �Ǻ��� �������� ���� ��ü �������� ���۵Ǿ�� �ϹǷ� 1��ö�� ��ǥ ���Ҷ� �Ǻ��� �������� �ʰ� ����.
	CFootingApp *pFooting	= &m_footing;
	CTwinVector tvWall		= GetTvOffset_Front(0, 0, FALSE, FALSE, TRUE, bVertDir);	// ����ö���� ���� �Ǿ� �� ������ü ö��..
	CTwinVector tvLower;
	if((pFooting->m_bIs || pFooting->IsVirtualFooting()) && pFooting->m_tvArrFront.GetTvByInfo("�����ϸ�", tvLower))
	{
		GetXyMatchLineAndLine(tvWall.m_v2, tvWall.GetXyDir(TRUE), tvLower.m_v1, tvLower.GetXyDir(), tvWall.m_v1);
	}
	else if(m_tvArrLowerLimit_FrontRebar.GetSize() > 0)
	{
		CDPointArray xyArrMatch;
		CRebarPlacing rb;
		rb.GetXyMatchTvArrAndLine(m_tvArrLowerLimit_FrontRebar, tvWall.m_v1, tvWall.GetXyDir(), xyArrMatch);
		if(xyArrMatch.GetSize() > 0)
		{
			tvWall.m_v1	= xyArrMatch.GetAt(0);
//			tvWall.m_v1.y += m_dCoverLower_LowerSlab;
		}
	}

	// ����ö�� ��ġ�� ��ġ ã��.
	CRebarPlacing rb;
	CDPointArray xyArr;
	CPlaceRebarByBlock *pPlace	= &m_placeByBlock_ShearRebar;
	rb.GetXyDivideLine(xyArr, tvWall.m_v1, tvWall.m_v2, pPlace->m_xyArr, FALSE, FALSE, FALSE);

	// ��ġ�� �˾����� ���̸� �༭ ����ö���� ����
	double dAngle	= bVertDir ? 90 : m_dAnglePath;
	double rad		= sin(ToRadian(dAngle));
	double dLen	= (m_dW - (m_dCoverLeft[0] + m_dCoverRight[0]))/rad;

	CDPoint xyDirTan	= tvWall.GetXyDir().RotateInv90();
	rb.MakePointArrayToLineArray(xyArr, tvArr, xyDirTan, dLen, FALSE);
}

void CWallApp::GetTvRebarShear_Plane( CTwinVectorArray &tvArr )
{
	tvArr.RemoveAll();
	if(m_bNotRebar) return;

	// ��ս��϶� ����ö���� ������.. 
	if(m_bTopIsColumn) return;

	CRebarPlacing rb;
	CTwinVector tvCenter;
	CTwinVectorArray tvArrPlan;
	m_Column.GetTvRebarRound_Plan(tvArrPlan, 0);
	tvArrPlan = m_Column.m_tvArrPlan[iOUTTER][iLOWER];
	tvCenter = tvArrPlan.GetTvByInfo(_T("��մܸ������"));
	rb.Offset(tvCenter, -m_dCoverLeft[0], -m_dCoverLeft[0]);

	// ����ö�� ��ġ�� ��ġ ã��.
	CDPointArray xyArr;
	vector<double> y_shear;
	CDPointArray xyArrDirApplyScale;
	m_Column.GetXyRebarMain_Plan(xyArr, xyArrDirApplyScale, 0);
	long nCount = xyArr.GetSize();
	for(long ix=0; ix<nCount; ++ix)
	{
		double dY = xyArr.GetAt(ix).y;
		if(dY < tvCenter.m_v1.y || dY > tvCenter.m_v2.y)
			continue;
		y_shear.push_back(dY);
	}
	std::sort(y_shear.begin(), y_shear.end());
	vector<double>::iterator iend = std::unique(y_shear.begin(), y_shear.end());
	y_shear.erase(iend, y_shear.end());

	xyArr.RemoveAll();
	long nSize = y_shear.size();
	for(long ix=1; ix<nSize-1; ++ix)
	{
		if(ix%(m_nCountWrapMainRebar/2) != 0) continue;

		CDPoint xyPos = CDPoint(tvCenter.m_v1.x, y_shear[ix]);
		xyArr.Add(xyPos);
	}
	
	// ��ġ�� �˾����� ���̸� �༭ ����ö���� ����
	CTwinVector tv;
	double dLen	= m_dW - (m_dCoverLeft[0] + m_dCoverRight[0]);
	CDPoint xyDir = tvCenter.GetXyDir().Rotate90();
	rb.MakePointArrayToLineArray(xyArr, tvArr, xyDir, dLen, FALSE);
}

// ������ ö�� ���̷� �ǰ�� �ϰ�, ������ ������ �ǰ�� �� ������ �������� �������� ��� 
// ���� ö�ٿ� �����Ǵ� ��ǥ�� �ǰ�� �Ѵ�.
void CWallApp::SetDataDefaultRebarSupport_VertSection_InWall(BOOL bLeft)
{
	long left	= bLeft ? iLEFT : iRIGHT;
	
	// ���庮ü�� ������ ������ �׻� ���ƾ� �ǹǷ� �����ǰ�Ƚ� �����ǰ�� �ϵ��� �Ѵ�////
	if(IsRoundRectWall() && !bLeft)
		bLeft	= TRUE;
	/////////////////////////////////////////////////////////////////////////////////////////

	CPlaceRebarByBlock *pPlace	= &m_placeByBlock_SupportRebar[left][0];
	pPlace->m_xyArr.RemoveAll();
	if(m_bTopIsColumn) return;

	CHgBaseConcStd baseConc;
	double dCTC			= m_dCTC_SupportRebar;

	// ��ü ��ö��ö�� ����(��ü ��ö���� ����(���)�� �͸� ����.)
	CRebarInfoCycle *pRCStt	= GetRebarInfoCycle(0, 0, bLeft, 0),
					*pRCEnd	= GetRebarInfoCycle(0, 0, bLeft, -1);
	CRebarInfoCycle *pRCSttOther	= GetRebarInfoCycle(0, 0, !bLeft, 0);
	
	if(!pRCStt || !pRCEnd || !pRCSttOther) return;
	

	CTwinVector tvWall;
	if(ComparePt(pRCStt->m_xyStt, pRCEnd->m_xyStt, "="))	// rc�� ó���� ���� ���ٸ� �ϳ� ¥���̰� �ٸ��� ������ ��� ���
	{
		tvWall.m_v1	= pRCStt->m_xyMid2;
		tvWall.m_v2	= pRCStt->m_xyEnd;
	}
	else
	{
		tvWall.m_v1	= pRCStt->m_xyMid2;
		tvWall.m_v2	= pRCEnd->m_xyMid1;
	}
	if(m_bRebarLowerSettle[left] == FALSE) // !m_footing.m_bIs && !m_footing.IsVirtualFooting())
	{
		// ���ʰ� ���� ���·� �ϴ� ����κ��� ����.
		tvWall.m_v1	= pRCStt->m_xyStt;
	}

	// ��� �Ѱ谡 �ִٸ� �Ѱ������ ����
	CDPoint xyMatch(0, 0);
	if(GetXyMatchSegAndSeg(m_tvUpperLimitOfSupportRebar.m_v1, m_tvUpperLimitOfSupportRebar.m_v2, tvWall.m_v1, tvWall.m_v2, xyMatch))
	{
		tvWall.m_v2	= xyMatch;
	}
	

	// ���ʰ� �ִٸ� ���� ��� ö�ٿ��� �ڸ�
	if(m_footing.m_bIs || m_footing.IsVirtualFooting())
	{
		CTwinVector tvUpper	= m_footing.GetTvOffset_Front(0, TRUE, m_footing.IsVertDir());
		GetXyMatchLineAndLine(tvWall.m_v1, tvWall.GetXyDir(), tvUpper.m_v1, tvUpper.GetXyDir(), tvWall.m_v1);
	}
	else
	{
		GetXyMatchLineAndLine(tvWall.m_v1, tvWall.GetXyDir(), m_tvLowerLimitOfSupportRebar.m_v1, m_tvLowerLimitOfSupportRebar.GetXyDir(), tvWall.m_v1);
	}
	
	CDPoint xySttSupportRebar	= GetXySttRebarSupport_Front(0, bLeft, TRUE);
	if(bLeft)
	{
		double dLenWall		= tvWall.GetLength() - 200;
		double dRemain		= 0;
		long nCount			= 0;

		// ��üö�� ���� ��ܺ��� ���� ��� ö�ٱ��� ////////////////////////////////////
		CRebarInfoCycle *pRC	= GetRebarInfoCycle(0, 0, bLeft, 0);
		if(pRC)
		{
			if(m_bRebarLowerSettle[left]) //m_footing.m_bIs || m_footing.IsVirtualFooting())
			{
				CDPoint vAng	= (pRC->m_xyEnd - pRC->m_xyMid2).Unit();
				double dDist	= 0;
				CTwinVector tvLower	= m_tvArrFront.GetTvByInfo("��ü�ϸ����");
				CDPoint xyMatch(0, 0);
				GetXyMatchLineAndLine(tvLower.m_v1, tvLower.GetXyDir(), pRC->m_xyMid2, vAng, xyMatch);
				if(m_footing.m_bIs || m_footing.IsVirtualFooting())
				{
					xyMatch	= xyMatch + (vAng * -m_footing.m_dCoverUpper[0]);
				}
				else
				{
					xyMatch	= xyMatch + (vAng * 100);
				}

				dDist	= ~(xyMatch - xySttSupportRebar);
				pPlace->m_xyArr.Add(CDPoint(1, dDist));
				// ��üö�� ���� ��ܺ��� ���� ��� ö�ٱ��� ////////////////////////////////////
			}
			else
			{
				double dDist	= 0;
				CDPoint xyMatch(0, 0);
				CDPoint vAng	= (pRC->m_xyStt - pRC->m_xyMid1).Unit();
				GetXyMatchLineAndLine(m_tvLowerLimitOfSupportRebar.m_v1, m_tvLowerLimitOfSupportRebar.GetXyDir(), pRC->m_xyStt, vAng, xyMatch);

				xyMatch	= xyMatch - (vAng * 100);
				dDist	= ~(xyMatch - xySttSupportRebar);
				pPlace->m_xyArr.Add(CDPoint(1, dDist));
				// ��üö�� ���� ��ܺ��� ���� ��� ö�ٱ��� ////////////////////////////////////
			}
		}

		// ���� ��� ö�ٿ��� ��ν����� �ϸ�ö�ٱ��� 
		nCount	= (long)(dLenWall / dCTC);
		dRemain	= dLenWall - (dCTC * nCount);
		if(dRemain < MIN_LEN_BLOCK)
		{
			dRemain += dCTC;
			nCount --;
		}
		pPlace->m_xyArr.Add(CDPoint(nCount, dCTC));
		pPlace->m_xyArr.Add(CDPoint(1, dRemain));
	}
	else
	{
		CDPointArray xyArr;
		GetXyRebarSupport_Front(xyArr, TRUE, 0);
		if(xyArr.GetSize() > 0)
		{
			CDPoint xyDirTan	= tvWall.GetXyDir().RotateInv90();

			// ������ ����ö�� ��ǥ�� ���缭 �ǰ�� �ؾ� ��.
			CDPoint xyMatch;
			CDPointArray xyArrRight;
			xyArrRight.Add(xySttSupportRebar);

			long nSize	= xyArr.GetSize();
			long i = 0; for(i = 0; i < nSize; i++)	// ù��° ö���� ����(������ġ�� �ٸ� �� ����)
			{
				if(GetXyMatchLineAndLine(tvWall.m_v1, tvWall.GetXyDir(), xyArr[i], xyDirTan, xyMatch))
				{
					xyArrRight.Add(xyMatch);	
				}
			}

			CRebarPlacing rb;
			rb.ConvertPointArrayToOffsetInfo(xyArrRight, pPlace->m_xyArr, 2);
		}
	}

	pPlace->m_nCountBlock	= pPlace->m_xyArr.GetSize();
}


// ����ö�� �ǰ��
void CWallApp::SetDataDefaultRebarShear_VertSection()
{
	if(m_bTopIsColumn) return;

	CRebarPlacing rb;
	CPlaceRebarByBlock *pPlace	= &m_placeByBlock_ShearRebar;
	pPlace->m_xyArr.RemoveAll();
	pPlace->m_nCountBlock	= 0;

	// ������ü ���ö�ٿ��� ��ν������ϸ�� ���ʻ�� ���̿� �ִ� ö�ٿ� ��ġ��.
	CTwinVector tvWall		= GetTvOffset_Front(0, 0, FALSE, TRUE, TRUE, TRUE);
	CTwinVector tvLower;
	CFootingApp *pFooting	= &m_footing;
	if(pFooting->m_bIs || pFooting->IsVirtualFooting())
	{
		pFooting->m_tvArrFront.GetTvByInfo("�����ϸ�", tvLower);
		GetXyMatchLineAndLine(tvLower.m_v1, tvLower.GetXyDir(), tvWall.m_v1, tvWall.GetXyDir(), tvWall.m_v1);
	}
	else
	{
		CDPointArray xyArrMatch;
		rb.GetXyMatchTvArrAndLine(m_tvArrLowerLimit_FrontRebar, tvWall.m_v1, tvWall.GetXyDir(), xyArrMatch);
		if(xyArrMatch.GetSize() > 0)
		{
			tvWall.m_v1 = xyArrMatch.GetAt(0);
		}

	}

	

		
	CDPoint xyMatch(0, 0);
	if(m_tvUpperLimitOfSupportRebar.GetLength() > 0)
		GetXyMatchLineAndLine(m_tvUpperLimitOfSupportRebar.m_v1, m_tvUpperLimitOfSupportRebar.GetXyDir(), tvWall.m_v1, tvWall.GetXyDir(), xyMatch);
	else
		xyMatch	= m_xyOrg;


	CDPointArray xyArrSup;
	GetXyRebarSupport_Front(xyArrSup, TRUE, 0);

	// ��±��� 2���̻󸶴� ����ö���� ���� �ȴٸ� ���⼭ ����. ///////////////////
	long nOffset	= (long)(m_dCTC_ShearRebar / m_dCTC_SupportRebar);
	if(nOffset > 1)
	{
		// ������ �ٲ�鼭 ��߳��°��� �ִ��� ���纸��.
		long nIndex = -1;
		double dDistPrev = 0;
		CDPointArray xyArrTmp;
		for(long ix=0; ix<xyArrSup.GetSize()-(nOffset-1); ix++)
		{
			nIndex++;
			if(ix==0) continue;
			double dDist = ~(xyArrSup.GetAt(ix-1) - xyArrSup.GetAt(ix));
			if(nIndex % nOffset != 0)
			{
				dDistPrev = dDist;
				continue;
			}

			if(dDist != dDistPrev) ix++;

			xyArrTmp.Add(xyArrSup.GetAt(ix));
			dDistPrev = dDist;
		}

		xyArrSup.RemoveAll();
		xyArrSup	= xyArrTmp;
	}
	////////////////////////////////////////////////////////////////////////////////////


	// ���ö�ٿ� �����ϸ� ��ǥ�ϳ� �߰�����(����ö���� ���۵Ǵ� ��ġ�� ����)
	xyArrSup.InsertAt(0, CDPoint(tvWall.m_v1.x, tvWall.m_v1.y));

	// �����ȿ� ��� ���� ������ ���ö�� ������ �м��ؼ� ����ö�� �������� ġȯ�Ѵ�.
	rb.ConvertPointArrayToOffsetInfo(xyArrSup, pPlace->m_xyArr, 1);
	pPlace->m_nCountBlock	= pPlace->m_xyArr.GetSize();
}

// ��ü �����鵵..
// �����鵵 ��� ������ ���� ��ǥ�� ������ 0,0�� �������� �� �׷�����.����
// BOOL bGetWall : ��ս��̶� ��ü�� ��ǥ�� ���Ѵ�.(Ư���� �뵵)
// BOOL bIncludeFooting : ���ʸ� ������ ��ǥ�� ����(����ö�� �׸����� ����Ѵ�) - ��ǻ� ���� �ǹ� ����(���ʿ� ��ü�� �ʺ� �ٸ� ��찡 �ֱ� ������)
// BOOL bIncludeUpperSlab : ��ν����굵 ��������..(����� ��ü�ϰ�츸 ����)
// ���ǻ���
// ��ü�� ������ ������ ��� �������� �������׸���.
// ��, �ϴ��� �������� �׸�.
// BOOL bForRebar : ����� ���Ѱ���?(��ġ���� ��ü�� �ִ��� �⺻������ ��ν����� ���� �����ش�)
// BOOL bCorrectionEL : ��ġ�� ���� ��� ��翡 ���� ����� ���ϴ� EL ����.
void CWallApp::GetTvLeftSide(CTwinVectorArray &tvArr, BOOL bGetWall, BOOL bIncludeFooting, BOOL bIncludeUpperSlab, BOOL bLeft, long nDrawLeft, BOOL bForRebar, BOOL bVertDir, BOOL bOnlyViewFromLeft, BOOL bPattern, BOOL bCorrectionEL)
{
	tvArr.RemoveAll();


	CRebarPlacing rb;
	CDPoint vAngRot	= GetXyDirRotate_Front();
	CDPoint xyOrgTop	= GetXyOrgTopForFront(bLeft);

	CTwinVectorArray tvArrLeftUpper;
	tvArrLeftUpper	= m_tvArrLeftUpper;

	// �Է¹��� EL�� ���� ���̸� �����ؼ� �׷��ش�.
	double dELB	= GetXyOrgFootingLowerForSide(bLeft).y;
	if(!bIncludeFooting)
	{
		CTwinVector tvWall		= m_tvArrFront.GetTvByInfo(bLeft ? "��ü������" : "��ü������");
		tvWall	= rb.RotateTv(tvWall, xyOrgTop, vAngRot.GetAngleDegree());

		dELB = tvWall.GetXyBottom().y;

		if(bCorrectionEL)
		{
			// ���ϴ� EL�� �����Ѵٸ� ���� �߰���ġ�ϰ��̴�.
// 			double dHCorrection = 0;
			double dWidthWall = m_tvArrFront.GetHorLength();
			if(m_tvUpperLimitOfMainRebar.GetVerLength() != 0)
			{
				CDPoint vDir = m_tvUpperLimitOfMainRebar.GetXyDir();
				double dHCorrection = fabs(dWidthWall / vDir.x * vDir.y) / 2.;
//				dHCorrection = fabs(m_dDiffHLeftByCenter) / 2.;
				rb.MoveRebar(tvArrLeftUpper, CDPoint(0, dHCorrection));
			}
			
			dELB = m_dBottomElCorrect;
		}
	}

	double dHMove	= 0;
	if(!bForRebar)
	{
		double dH	= m_nTypeRebarFront == 0 ? (bLeft ? m_dThickHunchL : m_dThickHunchR) : m_dThickHunch;
		dHMove	= -(m_dThickUppeSlab+dH);
	}

	if(m_dDiffTop[bLeft? 0 : 1] != 0)
		dHMove += -m_dDiffTop[bLeft? 0 : 1];
	if(m_nTypeRebarFront == 0 && !bCorrectionEL)
		dHMove += bLeft ? m_dDiffHLeftByCenter : m_dDiffHRightByCenter;

	rb.MoveRebar(tvArrLeftUpper, CDPoint(0, dHMove));


	if(!bLeft && !bOnlyViewFromLeft)
	{
		rb.Mirror(tvArrLeftUpper, CDPoint(m_dCenterOfWall, 0), TRUE);
	}

	
	CTwinVector tv;
	CDPoint xyLeft(0, 0), xyRight(0, 0);
	long nSize	= tvArrLeftUpper.GetSize();
	// ��� ���� ��ǥ�������� �⺻ ������ �׸�.
	if(nSize == 0)
	{
		xyLeft = xyRight = CDPoint(0, m_xyOrg.y);
		xyRight.x += 10000;
	}
	else
	{
		xyLeft	= tvArrLeftUpper.GetAt(0).m_v1;
		xyRight	= tvArrLeftUpper.GetAt(nSize-1).m_v2;
	}


	// ��ս��϶� ����� ���鵵�� ������.
	if(m_bTopIsColumn && m_nType == WALL_TYPE_VER && !bGetWall)
	{
		for(long col = 0; col < m_nCountColumn; col++)
		{
			CColumnApp *pColumn	= GetColumn(col);
			pColumn->GetTvFront(bPattern, TRUE);
			tvArr.AddFromTvArray(pColumn->m_tvArrFront);
		}

		if(nDrawLeft != 0)
		{
			BOOL bLeftTrim	= nDrawLeft == -1 ? FALSE : TRUE;
			CTwinVector tvCenter	= GetTvCenter_Side(bLeft, 100);
			CRebarPlacing rb;
			rb.TrimTvArrayByLine(tvArr, tvCenter.m_v1, tvCenter.m_v2, bLeftTrim, TRUE, FALSE);
		}
		return;
	}


	// �����ϴܿ��� ���� ���� �������� �׸�.
	// ������ �����Ѱ����� ġ�� �׸�.
	CDPoint xyMatch(0, 0);
	GetXyMatchLineAndLine(xyLeft, CDPoint(0, -1), CDPoint(xyLeft.x, dELB), CDPoint(1, 0), xyMatch);
	tv.m_v1		= xyMatch;
	tv.m_v2		= xyLeft;
	tv.m_sInfo	= "��ü����";
	tvArr.Add(tv);

	if(nSize == 0)
	{
		tv.m_v1		= tv.m_v2;
		tv.m_v2		= xyRight;
		tv.m_sInfo	= "��ü���";
		tvArr.Add(tv);
	}
	else
	{
		long i = 0; for(i = 0; i < nSize-1; i++)
		{
			tv	= tvArrLeftUpper.GetAt(i);
			tv.m_sInfo	= "��ü���";
			tvArrLeftUpper.SetAt(i, tv);
		}

		tv	= tvArrLeftUpper.GetAt(nSize-1);
		tv.m_sInfo	= "��ü��鳡";
		tvArrLeftUpper.SetAt(nSize-1, tv);
		
		
		tvArr.AddFromTvArray(tvArrLeftUpper);
	}

	GetXyMatchLineAndLine(xyRight, CDPoint(0, -1), CDPoint(xyRight.x, dELB), CDPoint(-1, 0), xyMatch);
	tv.m_v1		= xyRight;
	tv.m_v2		= xyMatch;
	tv.m_sInfo	= "��ü����";
	tvArr.Add(tv);

	tv			= tvArr.GetAt(0);
	tv.m_v2		= tv.m_v1;
	tv.m_v1		= xyMatch;
	tv.m_sInfo	= "��ü�ϸ�";
	tvArr.Add(tv);


	// ��ν����긦 �����ϴ� ��ü�� ���(������ �׸��� �� ���� �� ����)
	if(bIncludeUpperSlab)
	{
		long left = 0; for(left = 0; left < 2; left++)
		{
			CDPoint xyMatch(0, 0);
			long nIdx	= tvArr.GetIdxByInfo(left == iLEFT ? "��ü����" : "��ü����");
			if(nIdx > -1)
			{
				CTwinVector tvWall	= tvArr.GetAt(nIdx);
				CDPointArray xyArrMatch;
				rb.GetXyMatchTvArrAndLine(tvArrLeftUpper, tvWall.m_v1, tvWall.GetXyDir(), xyArrMatch);
				if(xyArrMatch.GetSize() > 0)
				{
					if(tvWall.m_v1.y < tvWall.m_v2.y)
						tvWall.m_v2	= xyArrMatch.GetAt(0);
					tvArr.SetAt(nIdx, tvWall);
				}
			}
		}
	}


	if(nDrawLeft != 0)
	{
		BOOL bLeftTrim	= nDrawLeft == -1 ? FALSE : TRUE;
		CTwinVector tvCenter	= GetTvCenter_Side(bLeft, 0);
		rb.TrimTvArrayByLine(tvArr, tvCenter.m_v1, tvCenter.m_v2, bLeftTrim, TRUE, FALSE);
	}
}

// ��ü ������ �������� ������ �Ÿ������� ��ν����� ����� ��ǥ
// dXInLeft : �����鵵 ������ x��ǥ
CDPoint CWallApp::GetXyMatchUpperSlab(double dXInLeft, BOOL bBottomOfHunch)
{
	CDPoint xyMatch(0, 0);
	long nSize	= m_tvArrLeftUpper.GetSize();
	if(nSize == 0)
	{
		GetXyMatchLineAndLine(m_xyOrg, CDPoint(1, 0), CDPoint(dXInLeft, 0), CDPoint(0, 1), xyMatch);
	}
	else
	{
		CTwinVector tv;
		BOOL bFind		= FALSE;
		long i = 0; for(i = 0; i < nSize; i++)
		{
			tv	= m_tvArrLeftUpper.GetAt(i);

			if(GetXyMatchSegAndLine(tv.m_v1, tv.m_v2, CDPoint(dXInLeft, 0), CDPoint(0, 1), xyMatch))
			{
				bFind	= TRUE;
				break;
			}
		}

		// �� ã�� ��� ó���� �������� ������ �׳� ã��.
		if(!bFind)
		{
			CDPoint xyLeft	= m_tvArrLeftUpper.GetAt(0).m_v1;
			CDPoint xyRight	= m_tvArrLeftUpper.GetAt(nSize-1).m_v2;
			GetXyMatchLineAndLine(xyLeft, (xyRight-xyLeft).Unit(), CDPoint(dXInLeft, 0), CDPoint(0, 1), xyMatch);
		}
	}

	if(bBottomOfHunch)
	{
		xyMatch.y -= (m_dThickUppeSlab + m_dThickHunch);
	}

	return xyMatch;
}

// rebarinfocycle �迭 �ΰ��� ��ģ��.
// BOOL bInvert : ��ĥ �迭�� ����� ��ĥ��..?
// �� �Լ� ȣ��� �ݵ�� �޸� ���� �� �ٰ�.
void CWallApp::AddRebarInfoCycle(CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrDest, CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrSource, BOOL bInvert)
{
	if(!pArrDest || !pArrSource) return;

	long nSize = pArrSource->GetSize();
	if(bInvert)
	{
		long i = 0; for(i = nSize-1; i >= 0; i--)
		{
			CRebarInfoCycle *pRC	= pArrSource->GetAt(i);
			
			CRebarInfoCycle *pRCNew	= new CRebarInfoCycle;
			*pRCNew	= *pRC;
			InvertRebarInfoCycle(pRCNew);

			pArrDest->Add(pRCNew);
		}
	}
	else
	{
		long i = 0; for(i = 0; i < nSize; i++)
		{
			CRebarInfoCycle *pRC	= pArrSource->GetAt(i);

			CRebarInfoCycle *pRCNew	= new CRebarInfoCycle;
			*pRCNew	= *pRC;
			pArrDest->Add(pRCNew);
		}
	}
}

// rebarinfocycle�� �����´�.
void CWallApp::InvertRebarInfoCycle(CRebarInfoCycle *pRC)
{
	if(!pRC) return;

	if(pRC->m_nPosStt == pRC->m_nPosEnd)
	{
		GetSwap(pRC->m_xyStt, pRC->m_xyMid1);
	}
	else
	{

		GetSwap(pRC->m_xyStt, pRC->m_xyEnd);
		GetSwap(pRC->m_xyMid1, pRC->m_xyMid2);
		GetSwap(pRC->m_nPosStt, pRC->m_nPosEnd);
	}
}

BOOL CWallApp::IsApplyAngle()
{
	if(m_tvArrFront.GetSize() > 0)
	{
		return m_tvArrFront.GetAt(0).m_bApplyAngle;
	}
	else
		return FALSE;
}

// ���� ���ö��(������ �������� �� ���)
// BOOL bCutAddRebar : �߰� ���� ö���� �ڸ���?(ö�� ��ũ���϶��� �� �߶�ߵ�)
void CWallApp::GetTvRebarSupport_Side(CTwinVectorArray &tvArr, CTwinVectorArray &tvArrAngle, CTwinVectorArray &tvArrAngleAdd, BOOL bLeftWall, long nDan, BOOL bLeft, BOOL bCutAddRebar, BOOL bCoverLocation)
{
	tvArr.RemoveAll();
	tvArrAngle.RemoveAll();
	tvArrAngleAdd.RemoveAll();
	if(m_bNotRebar) return;

	// ���ö�� ���� ����.(�����ϸ� ���� ���鼭 ���ϸ� ��)
	CDoubleArray dArrDist;
	GetDistRebarSupport_Front(dArrDist, bLeftWall, nDan, bCoverLocation);

	// ��ü �¿���
	BOOL bIncludeFooting	= TRUE;
	BOOL bIncludeUpperSlab	= m_nType == WALL_TYPE_SLOPE ? TRUE : FALSE;
	CTwinVectorArray tvArrSide;
	GetTvLeftSide(tvArrSide, FALSE, bIncludeFooting, bIncludeUpperSlab, bLeft, 0, TRUE);
	CTwinVector tvLeft	= tvArrSide.GetTvByInfo("��ü����"),
				tvRight	= tvArrSide.GetTvByInfo("��ü����");
	
	// �¿��� ��ǥ�� �Ʒ����� ���� ���ϵ��� ����.
	if(tvLeft.m_v1.y > tvLeft.m_v2.y) GetSwap(tvLeft.m_v1, tvLeft.m_v2);
	if(tvRight.m_v1.y > tvRight.m_v2.y) GetSwap(tvRight.m_v1, tvRight.m_v2);

	// ö�ٱ��� ����.
	CDPoint vAng	= ToDPointFrDegree(m_dAnglePath);
	double	dCoverFront	= GetCoverFront(TRUE, 0)/vAng.y,
			dCoverBack	= GetCoverFront(FALSE, 0)/vAng.y;
	double dLenHor		= fabs(tvRight.m_v1.x-tvLeft.m_v1.x)-dCoverFront-dCoverBack;

	// �����.
	CRebarPlacing rb;
	CDPoint xyStt		= CDPoint(tvLeft.m_v1.x+dCoverBack, tvLeft.m_v1.y);
	if(m_tvArrLowerLimit_FrontRebar.GetSize() > 0)
	{
		CTwinVector tvWallRebar	= GetTvOffset_Front(0, 0, FALSE, TRUE, bLeftWall, TRUE);
		CDPointArray xyArrMatch;
		rb.GetXyMatchTvArrAndLine(m_tvArrLowerLimit_FrontRebar, tvWallRebar.m_v1, CDPoint(0, -1), xyArrMatch);
		if(xyArrMatch.GetSize() > 0)
		{
			xyStt.y	= xyArrMatch.GetAt(0).y;
		}
	}
	
	
	/*
	// ��� ���� ��ġ�� ��ü ��ö�� ��� ���� ���� ����.
	CRebarInfoCycle *pRC	= GetRebarInfoCycle(0, 0, bLeftWall, 0);
	if(pRC)
	{
		CDPoint xyOrg		= GetXyOrgTopForFront(bLeft);
		double dAng			= GetXyDirRotate_Front().GetAngleDegree();
		CDPoint xyMid		= pRC->m_xyMid2;
		rb.RotateXy(xyMid, xyOrg, dAng);
		xyStt.y = xyMid.y;
	}
	*/
	

	// ���� ���� /////////
	xyStt.x -= m_dWidthExp[iLEFT];
	dLenHor += m_dWidthExp[iLEFT] + m_dWidthExp[iRIGHT];
	// ���� ���� /////////


	// ö���߿��� ��ü ��鿡 �ɸ��� ö���� ������ ��� �׷� ������.
	CTwinVectorArray tvArrUpper;
	tvArrUpper	= m_tvArrLeftUpperSlab;

	if(m_dDiffTop[bLeft? 0 : 1] != 0)
		rb.MoveRebar(tvArrUpper, CDPoint(0, -m_dDiffTop[bLeft? 0 : 1]));

	if(!bLeft)
		rb.Mirror(tvArrUpper, CDPoint(m_dCenterOfWall, 0), TRUE);

	long nSize	= dArrDist.GetSize();
	CTwinVector tv;
	tv.m_v1	= xyStt;
	tv.m_v2	= CVector(tv.m_v1.x + dLenHor, tv.m_v1.y, 0);
	CDPointArray xyArrMatch;


	// ������ ��� ö�� ���� ����.
	CTwinVectorArray tvArrRebar;
	tvArrRebar	= tvArrUpper;
	CDRect rect	= tvArrRebar.GetRect();
	rb.TrimTvArrayByLine(tvArrRebar, CDPoint(xyStt.x, rect.bottom), CDPoint(xyStt.x, rect.top), TRUE, FALSE, FALSE);
	rb.TrimTvArrayByLine(tvArrRebar, CDPoint(xyStt.x+dLenHor, rect.bottom), CDPoint(xyStt.x+dLenHor, rect.top), FALSE, FALSE, FALSE);

	double dTopLimit	= tvArrUpper.GetXyBottom().y - m_dCoverUpper_UpperSlab - m_dRangeSupportRebarForSameUpperSlab;
	double dMid	= GetCrossSectionCenterOfRoad(bLeft);

	CTwinVectorArray tvArrRebarOld;
	tvArrRebarOld	= tvArrRebar;
	long i = 0; for(i = 0; i < nSize; i++)
	{
		tv.m_v1.y	+= dArrDist.GetAt(i);
		tv.m_v2.y	= tv.m_v1.y;
		tv.m_sInfo.Format("%d", i+1);

		if(tv.m_v1.y < dTopLimit)
			tvArr.Add(tv);
		else
		{
			CDPoint xyMatchUpper	= CDPoint(0, 0);
			CDPoint xyMatchTv		= CDPoint(0, 0);

			rb.GetXyMatchTvArrAndLine(tvArrRebar, CDPoint(dMid, 0), CDPoint(0, 1), xyArrMatch);
			GetXyMatchLineAndLine(tv.m_v1, tv.GetXyDir(), CDPoint(dMid, 0), CDPoint(0, 1), xyMatchTv);
			if(xyArrMatch.GetSize() == 0)
			{
// 				CDPoint xyLeft	= tvArrRebar.GetXyLeft();
// 				CDPoint xyRight	= tvArrRebar.GetXyRight();
// 				if(fabs(dMid-xyLeft.x) < fabs(dMid-xyRight.x))
// 					xyMatchUpper	= xyLeft;
// 				else
// 					xyMatchUpper	= xyRight;

				// (ARCBRIDGE-3078) ���翡 ���� ��ġ�� ����Ϸ��� Match�Ǵ� ���� �׳� �Ű��ָ� �ȵǰ� ���弱���� ã����� �Ѵ�.
				CDPoint xy1	= tvArrRebar.GetXyFirst();
				CDPoint xy2	= tvArrRebar.GetXyLast();
				if(fabs(dMid-xy1.x) < fabs(dMid-xy2.x))
				{
					CTwinVector tvTemp;
					tvArrRebar.GetFirst(tvTemp);
					GetXyMatchLineAndLine(tvTemp.m_v1, tvTemp.GetXyDir(), CDPoint(dMid, 0), CDPoint(0, 1), xyMatchUpper);
				}
				else
				{
					CTwinVector tvTemp;
					tvArrRebar.GetLast(tvTemp);
					GetXyMatchLineAndLine(tvTemp.m_v1, tvTemp.GetXyDir(), CDPoint(dMid, 0), CDPoint(0, 1), xyMatchUpper);
				}
			}
			else
				xyMatchUpper	= xyArrMatch.GetAt(0);

			rb.MoveRebar(tvArrRebar, CDPoint(0, xyMatchTv.y - xyMatchUpper.y));
			long j = 0; for(j = 0; j < tvArrRebar.GetSize(); j++)
			{
				CTwinVector tmp	= tvArrRebar.GetAt(j);
				tmp.m_sInfo.Format("%d", i+1);
				tvArrRebar.SetAt(j, tmp);
			}
			if(tvArrAngle.GetSize() == 0)
				tvArrRebarOld	= tvArrRebar;
			tvArrAngle.AddFromTvArray(tvArrRebar);
			
		}
	}

	if(m_placeByBlock_SupportRebarAddSameUpperSlab.m_xyArr.GetSize() > 0)
	{
		tvArrRebar	= tvArrRebarOld;
		CDPoint xyR	= m_placeByBlock_SupportRebarAddSameUpperSlab.m_xyArr.GetAt(0);

		// �߰� ���� ö���� �ʿ��� �κи� ��󳽴�.
		if(bCutAddRebar)
		{
			double dXLeft	= tvArrRebar.GetXyLeft().x + m_dDistAddSupportRebarForSameUpperSlab[iLEFT];
			double dXRight	= tvArrRebar.GetXyRight().x - m_dDistAddSupportRebarForSameUpperSlab[iRIGHT];
			CDRect rect	= tvArrRebar.GetRect();

			if(m_dDistAddSupportRebarForSameUpperSlab[iLEFT] > 0)
			{
				rb.TrimTvArrayByLine(tvArrRebar, CDPoint(dXLeft, rect.bottom-1000), CDPoint(dXLeft, rect.top+1000), TRUE, FALSE, FALSE);
			}
			if(m_dDistAddSupportRebarForSameUpperSlab[iRIGHT] > 0)
			{
				rb.TrimTvArrayByLine(tvArrRebar, CDPoint(dXRight, rect.bottom-1000), CDPoint(dXRight, rect.top+1000), FALSE, FALSE, FALSE);
			}
		}
		if(tvArrAngle.GetSize() > 0 && (xyR.x*xyR.y > 0))
		{
			rb.MoveRebar(tvArrRebar, CDPoint(0, -xyR.x*xyR.y));
			long i = 0; for(i = 0; i < xyR.x; i++)
			{
				long j = 0; for(j = 0; j < tvArrRebar.GetSize(); j++)
				{
					CTwinVector tmp	= tvArrRebar.GetAt(j);
					tmp.m_sInfo.Format("%d", i+1);
					tvArrRebar.SetAt(j, tmp);
				}
				if(bCutAddRebar)
					tvArrAngleAdd.AddFromTvArray(tvArrRebar);
				else
					tvArrAngle.AddFromTvArray(tvArrRebar);
				rb.MoveRebar(tvArrRebar, CDPoint(0, xyR.y));
			}
		}
	}
}

// ���� ���ö�� ���� 
// ���� �ϸ鿡�� ������ ����
// ������ϰ�� �����Ÿ��� �ƴ϶� ���� �Ÿ��� �����ؾ� ��
// �׸��� ���⼭ ���ϴ� �Ÿ��� ���� ��ġ�� �ƴ϶�. ���� ��ٵ��� �׸��� ���� ���� ��ġ������ �Ÿ��� �ȴ�.
// ���� ���� �Ÿ��� �ɼǿ� ���� ���ݾ� �޶� �� �� �ִ�.
void CWallApp::GetDistRebarSupport_Front(CDoubleArray &dArr, BOOL bLeft, long nDan,BOOL bCoverLocation )
{
	// ���ö�� ��ǥ
	CDPointArray xyArr;
	// ���� �Ÿ��� ��Ȯ�ϰ� �ϱ� ���ؼ� ������ �¿������� �ű� ��ǥ�� ���Ѵ�.
	double dCoverLeft	= m_dCoverLeft[0];
	double dCoverRight	= m_dCoverRight[0];

	if(m_nTypeRebarFront == 0)
	{
		m_dCoverLeft[0]		= -REBAR_SYMBOL_RADIUS;
		m_dCoverRight[0]	= REBAR_SYMBOL_RADIUS;
	}
	else
	{
		m_dCoverLeft[0]		= m_dW/2 - REBAR_SYMBOL_RADIUS;
		m_dCoverRight[0]	= -m_dW/2 + REBAR_SYMBOL_RADIUS;
	}

	GetXyRebarSupport_Front(xyArr, bLeft, nDan, FALSE);

	m_dCoverLeft[0]		= dCoverLeft;
	m_dCoverRight[0]	= dCoverRight;

	
	
	// ���� �迭�� ��ȯ
	long nSize	= xyArr.GetSize();
	CDPoint xy1(0, 0), xy2(0, 0);
	long i = 0; for(i = 0; i < nSize-1; i++)
	{
		xy1	= xyArr.GetAt(i);
		xy2	= xyArr.GetAt(i+1);
		dArr.Add(~(xy2-xy1));
	}

	// ù��° ö���� ���� �ϸ������ �Ÿ��� ����Ѵ�.
	if(nSize > 0)
	{
		if(m_footing.m_bIs || m_footing.IsVirtualFooting())
		{
			// �����Ը�� ù��° ö���� �������� �����.
			CDPoint xy	= xyArr.GetAt(0);
			CTwinVector tvLower	= m_footing.m_tvArrFront.GetTvByInfo("�����ϸ�");
			CRebarPlacing rb;
			CDPoint xyOrgTop	= GetXyOrgTopForFront(bLeft);
			double dAngRot		= GetXyDirRotate_Front().GetAngleDegree();
			tvLower	= rb.RotateTv(tvLower, xyOrgTop, dAngRot);
			rb.RotateXy(xy, xyOrgTop, dAngRot);

			// �������� ������ ���¿��� ��ü�� �׸��� ���� X��ġ�� �̵�
			xy.x		= GetXyOrgTopForFront(bLeft).x;

			
			// ������ ã��
			CDPoint xyMatch(0, 0);
			GetXyMatchLineAndLine(tvLower.m_v1, tvLower.GetXyDir(), xy, CDPoint(0, -1), xyMatch);
			dArr.InsertAt(0, xy.y - xyMatch.y);
		}
		else if(m_tvArrLowerLimit_FrontRebar.GetSize() > 0)
		{
			CDPoint xy	= xyArr.GetAt(0);
			CDPointArray xyArrMatch;
			CRebarPlacing rb;
			rb.GetXyMatchTvArrAndLine(m_tvArrLowerLimit_FrontRebar, xy, CDPoint(0, -1), xyArrMatch);			
			if(xyArrMatch.GetSize() > 0)
			{
				CDPoint xyMatch	= xyArrMatch.GetAt(0);
				if(bCoverLocation)
					dArr.InsertAt(0,GetCoverUpper(FALSE,0,TRUE));
				else
					dArr.InsertAt(0, xy.y-xyMatch.y);				
			}
		}
	}
}

// ���� ��ö��(������ �������� �� ���)
// BOOL bIncludeRound : ����ö���� ���������� �ִ� ��ö�ٿ� ���ؼ��� ���Ƿ� ����� ��ö���� �������� ���� ��찡 �ִ�.
// nIdxRC	= 999 : ö�� ��ũ�� ���� �ε��� ������� ÷���� ������ ����.
void CWallApp::GetTvRebarMain_Side(CTwinVectorArray &tvArr, BOOL bLeftRebar, long nCycle, long nDan, long nIdxRC, CTwinVectorArray &tvArrJoint, BOOL bIncludeRound, BOOL bLeft, BOOL bForDim)
{
	tvArr.RemoveAll();
	tvArrJoint.RemoveAll();
	if(m_bNotRebar) return;
	const double dGapJoint	= 36;	// ��ö�ٰ� �������� ����
	BOOL bJoint	= FALSE;

	CRebarInfoCycle *pRCTmp	= GetRebarInfoCycle(nCycle==-1?0:nCycle, nDan, bLeftRebar, nIdxRC);
	CRebarInfoCycle *pRC	= NULL;


	if(!pRCTmp) 
	{
		if(nIdxRC == 999)
		{
			pRCTmp		= GetRebarInfoCycle(nCycle==-1?0:nCycle, nDan, bLeftRebar, 0);
			if(!pRCTmp) return;

			pRC	= new CRebarInfoCycle;
			*pRC	= *pRCTmp;

			CRebarInfoCycle *pRCLast	= GetRebarInfoCycle(nCycle==-1?0:nCycle, nDan, bLeftRebar, -1);
			pRC->m_xyEnd	= pRCLast->m_xyMid1;
		}
		else
			return;	// ö���� ������ ����
	}
	else
	{
		if(!pRCTmp->m_bExist && nIdxRC != 999) return;
		pRC	= new CRebarInfoCycle;
		*pRC	= *pRCTmp;
	}
	CRebarInfoCycle *pRCTmp1	= GetRebarInfoCycle(nCycle==-1?0:nCycle, nDan, bLeftRebar, nIdxRC+1);	// ������ ���ϱ� ���� ���� ö��
	CRebarInfoCycle cRCNext;
	if(pRCTmp1 != NULL && pRCTmp1->m_bExist)
	{
		bJoint = TRUE;
		cRCNext = *pRCTmp1;
	}
		
	CRebarPlacing rb;

	// rc�� �������� �������� �������� �������� �����.
	if(m_nType == WALL_TYPE_SLOPE)
	{
		double dAng		= GetXyDirRotate_Front().GetAngleDegree();
		CDPoint xyOrg	= GetXyOrgTopForFront(bLeftRebar);
		
		rb.RotateXy(pRC->m_xyStt, xyOrg, dAng);
		rb.RotateXy(pRC->m_xyMid1, xyOrg, dAng);
		rb.RotateXy(pRC->m_xyMid2, xyOrg, dAng);
		rb.RotateXy(pRC->m_xyEnd, xyOrg, dAng);
		rb.RotateXy(pRC->m_xyCen, xyOrg, dAng);
		rb.RotateXy(pRC->m_xyMidExt, xyOrg, dAng);
		rb.RotateXy(pRC->m_xyEndExt, xyOrg, dAng);
		rb.RotateXy(pRC->m_xyCenExt, xyOrg, dAng);
		

		// ���������� ö�ٵ� ȸ�� ������
		if(bJoint)
		{
			rb.RotateXy(cRCNext.m_xyStt, xyOrg, dAng);
			rb.RotateXy(cRCNext.m_xyMid1, xyOrg, dAng);
			rb.RotateXy(cRCNext.m_xyMid2, xyOrg, dAng);
			rb.RotateXy(cRCNext.m_xyEnd, xyOrg, dAng);
			rb.RotateXy(cRCNext.m_xyCen, xyOrg, dAng);
			rb.RotateXy(cRCNext.m_xyMidExt, xyOrg, dAng);
			rb.RotateXy(cRCNext.m_xyEndExt, xyOrg, dAng);
			rb.RotateXy(cRCNext.m_xyCenExt, xyOrg, dAng);
		}
	}


	// ö���� ��ν����� ���� ���� ���� ��ȭ ��ų��?
	BOOL bVariableLength	= pRC->m_bVariableLength;	// ���̰� ��ȭ�Ǵ� ö������?
	BOOL bVariablePos		= pRC->m_bVariablePos;	// ������ġ�� ��ȭ�Ǵ� ö������?
	BOOL bBoxType				= m_tvLowerLimitOfMainRebar.GetLength() >0;

	// ������ ���� ��츦 �Ǵ�.(������ö���� ���� ��� ����)
	if(nIdxRC == GetCountRebarInfoCycle(nCycle==-1?0:nCycle, nDan, bLeftRebar)-1)
	{
		bJoint			= FALSE;
	}

	BOOL bBoxTypeNotSlabJoint = (pRC->m_nPosStt == REBARINFO_POS_STTWALL_OUTTER && pRC->m_nPosEnd == REBARINFO_POS_ENDWALL_OUTTER) && bBoxType;


	// ��ö���� ���� ����(��������,���庮ü�ǰ�����δ³��߿����� ����) ////
	CTwinVectorArray tvArrSide;
	GetTvOffset_Side(tvArrSide, bLeft, 0);	// ���ܸ���� 2��ö���� �Ǻ��� 2���Ǻ��� ���� ������, ���鵵������ �׻� 1���Ǻ��� �����
	CTwinVector tvLeft	= tvArrSide.GetTvByInfo("��ü����"),
				tvRight	= tvArrSide.GetTvByInfo("��ü����");

	CTwinVector tvHorOrg;
	tvHorOrg.m_v1	= tvLeft.GetXyBottom();
	tvHorOrg.m_v2	= tvRight.GetXyBottom();
	// �纸����ġ �϶� ���� �κб��� ��ö���� ��ġ�ȴ�. �׷��ٶ� ������ ��ġ �� ġ������ ����Ѵ�
	if(m_dWidthOverRebarExp[iLEFT] > 0)
	{
		tvLeft.m_v1.x -= m_dWidthOverRebarExp[iLEFT];
		tvLeft.m_v2.x -= m_dWidthOverRebarExp[iLEFT];
	}
	if(m_dWidthOverRebarExp[iRIGHT] > 0)
	{
		tvRight.m_v1.x += m_dWidthOverRebarExp[iRIGHT];
		tvRight.m_v2.x += m_dWidthOverRebarExp[iRIGHT];
	}

	if(IsRoundRectWall())
	{
		double dCover	= GetCoverFront(TRUE, 0);
		tvLeft.m_v1.x += (m_dW/2 - dCover);
		tvLeft.m_v2.x += (m_dW/2 - dCover);
		tvRight.m_v1.x -= (m_dW/2 - dCover);
		tvRight.m_v2.x -= (m_dW/2 - dCover);
	}

	CTwinVector tvHor;
	tvHor.m_v1	= tvLeft.GetXyBottom();
	tvHor.m_v2	= tvRight.GetXyBottom();
	///////////////////////////////////////////////////////////////////////////////////
	

	// ���� ��ö�� ���� ���� ///////////////////////////////////////////////////////////////////
	CDPointArray xyArrOffset;
	GetOffsetInfoRebarMain_Plane_LineSection(xyArrOffset, TRUE, 0);
	CHgBaseConcStd baseConc;
	CDPointArray xyArr;
	
	// 1cycle�� ��ü����ο� ���� ö�� ���� ��Ŵ.
	if(IsRoundRectWall())
	{
		xyArr.RemoveAll();
		CDPointArray xyArrTmp, xyArrTmp2;

		// ������ ��ü�� ��ö�� ������ ���ؼ� �־�Z������,
		// ���߿� ��սĺ�ü�� ö�� ������ �Է¹޴� ������� ����Ǿ �Ʒ��� �ڵ带 �ּ�ó����.
		/*
		long nROld	= m_Column.m_nR[iOUTTER][nDan];
		if(!bIncludeRound)
			m_Column.m_nR[iOUTTER][nDan]	= 0;

		m_Column.m_dF1[iOUTTER][nDan]	= xyArrOffset[0].y;
		m_Column.m_nE[iOUTTER][nDan]	= xyArrOffset[1].x;
		m_Column.m_dF[iOUTTER][nDan]	= xyArrOffset[1].y;
		m_Column.m_dF2[iOUTTER][nDan]	= xyArrOffset[2].y;
		*/
		
		CDPointArray xyArrDirApplyScale;
		//m_Column.GetXyRebarMain_Plan(xyArrTmp, xyArrDirApplyScale, 0);
		GetXyRebarMain_Plan_For_Round_Wall(xyArrTmp, xyArrDirApplyScale, 0, nCycle>0);
		if(nCycle == -1)
		{
			GetXyRebarMain_Plan_For_Round_Wall(xyArrTmp2, xyArrDirApplyScale, 0, nCycle>0);
			
			long nSize = xyArrTmp2.GetSize();
			for(long ix=0; ix<nSize; ++ix)
				xyArrTmp.Add(xyArrTmp2.GetAt(ix));
		}

		CDPoint xyCen	= m_Column.m_xyCen;
		rb.RotateRebar(xyArrTmp, xyCen, 90);
		rb.GetXyAlonePoint(xyArrTmp, xyArr, TRUE, xyCen, bLeft ? LEFTUP_AREA|RIGHTUP_AREA : LEFTDOWN_AREA|RIGHTDOWN_AREA, TRUE, 1.0);
		rb.MoveRebar(xyArr, tvHor.GetXyMid() - xyCen);
		xyArr.Sort();

		//m_Column.m_nR[iOUTTER][nDan]	= nROld;
	}
	else
	{
		rb.GetXyDivideLine(xyArr, tvHor.m_v1, tvHor.m_v2, xyArrOffset, TRUE, FALSE, FALSE);
	}

	/*
	// 2cycle�� ��� 1cycle�� ���̿� �ϳ��� ��. ////////////////////
	if(IsRoundRectWall())
	{
		CDPointArray xyArrTmp;
		long nSize	= xyArr.GetSize();
		if(nCycle != -1)
		{
			long nStt	= nCycle == 0 || nCycle == 2 ? 0 : 1;
			long i = 0; for(i = nStt; i < nSize-1-nStt; i+=2)
				xyArrTmp.Add(xyArr.GetAt(i));
			// 1,3 cycle�̰� r�� Ȧ���̸� ���� �ϳ� ����.. �̰Ŵ� ���� ��ü�� ����
			if((nCycle == 0 || nCycle == 2) && m_Column.m_nR[iOUTTER][nDan] % 2 == 1 && bLeft)
			{
				xyArrTmp.Add(xyArr.GetAt(nSize-1));
			}

			xyArr.RemoveAll();
			xyArr	= xyArrTmp;
		}
	}
	else
	*/
	if(IsRoundRectWall() == FALSE)
	{
		// IsRoundRectWall()�϶���  ��鿡�� 1,2 Cycle�� ���� �������� ���⼭�� �������� �ʴ´�.
		if(nCycle == 1 || nCycle == 3 || nCycle == -1)
		{
			CDPointArray xyArrTmp;

			// ��/�Ϻ� ö��
			long nSize	= xyArr.GetSize();
			if(nCycle != -1)
			{
				long i = 0; for(i = 0; i < nSize-1; i++)
					xyArrTmp.Add((xyArr[i]+xyArr[i+1])/2);

				xyArr.RemoveAll();
				xyArr	= xyArrTmp;	
			}
			else
			{
				CDPoint xy1(0, 0), xy2(0, 0);
				long i = 0; for(i = 0; i < nSize-1; i++)
				{
					xy1	= xyArr[i];
					xy2	= xyArr[i+1];
					xyArr.Add((xyArr[i]+xyArr[i+1])/2);
				}

				xyArr.Sort(TRUE);
			}
		}
	}

	// 2cycle�� ��� 1cycle�� ���̿� �ϳ��� ��. ////////////////////
	// 4cycle���� �����Ǵ� ���
	// 1, 2cycle�ΰ�� ¦����°��, 3,4cycle�ΰ�� Ȧ����°�� ��°��ǥ�� ����� �ȴ�.
	if(GetCountCycleMainRebar() == 4 && nCycle != -1)
	{
		BOOL bDeleteOdd	= nCycle==2||nCycle==3 ? TRUE : FALSE;
		long i = 0; for(i = 0; i < xyArr.GetSize(); i++)
		{
			if(bDeleteOdd)
			{
				xyArr.RemoveAt(i);
				i--;
			}

			bDeleteOdd	= !bDeleteOdd;
		}
	}

	/*
	if(xyArr.GetSize() > 0 && nCycle != 0)
		xyArr.RemoveAt(xyArr.GetSize()-1);
		*/
	////////////////////////////////////////////////////////////////////

	// ġ���� ��ö���� �����ο� �� ��ö�ٱ��� ��� ������ְ� �׸������ ������������ ��ü ���� �ִ� �͸� ������ش�.
	if(bForDim == FALSE && (m_dWidthOverRebarExp[iLEFT] > 0 || m_dWidthOverRebarExp[iRIGHT] > 0))
	{
		CDPointArray xyArrTemp = xyArr;
		xyArr.RemoveAll();
		double dCover	= GetCoverFront(TRUE, 0);

		long nSize = xyArrTemp.GetSize();
		for(long ix=0; ix<nSize; ++ix)
		{
			CDPoint xyPos = xyArrTemp.GetAt(ix);
			if(xyPos.x > (tvHorOrg.m_v1.x - dCover) && xyPos.x < (tvHorOrg.m_v2.x + dCover))
			{
				xyArr.Add(xyPos);
			}
		}
	}

	// ��ü ����� ������ �ƴϹǷ� ��ü ��鿡 �ٵ��� �ؼ� ö�� ����
	CTwinVector tvMain;			// ���� ����ؾ� �Ǵ� ��ö��.(���������� ����� ����)
	CTwinVector tvMainNext;
	if(pRC->m_nPosStt == pRC->m_nPosEnd)
	{
		tvMain.m_v1	= pRC->m_xyStt;
		tvMain.m_v2	= pRC->m_xyMid1;
	}
	else
	{
		tvMain.m_v1	= pRC->m_xyStt;
		tvMain.m_v2	= pRC->m_xyMid1;
		if(nIdxRC == 0)
		{
			if(pRC->m_xyEnd.y > pRC->m_xyMid2.y)
			{
				tvMain.m_v1	= pRC->m_xyMid2;
				tvMain.m_v1.y -= pRC->m_R_Mid;
			}
		}
		/* ������ ��米���� ����� ������ ���迡 ������ �����Ƿ� ����ö�ٱ����� ��Ȯ�ϰ� �׸�.
		if(!IsVertWall())
		{
			if(pRC->m_xyEnd.y > pRC->m_xyMid2.y)
			{
				// ��米�� ��� �������� ���������� R�κ��� �� �Ʒ������� ���� �� ���� �����Ƿ�.. ///
				tvMain.m_v1	= pRC->m_xyMid2;
				if(tvMain.m_v1.y > pRC->m_xyCen.y - pRC->m_R_Mid)
					tvMain.m_v1.y = pRC->m_xyCen.y - pRC->m_R_Mid;
				// ��米�� ��� �������� ���������� R�κ��� �� �Ʒ������� ���� �� ���� �����Ƿ�.. ///

			}
		}
		*/


		if(pRC->m_bUseMidExt)
			tvMain.m_v2	= pRC->m_xyMidExt;
		else if(pRC->m_bUseEndExt)
			tvMain.m_v2	= pRC->m_xyEndExt;
		else
		{
			if(Compare(pRC->m_xyMid2.x, pRC->m_xyEnd.x, "="))
				tvMain.m_v2	= pRC->m_xyEnd;
			else
				tvMain.m_v2	= pRC->m_xyMid2;
		}

		if(bBoxType/*bBoxTypeNotSlabJoint*/)
		{
			double dMinLeft(0), dMaxRight(0), dMinEL(0), dMaxEL(0);
			CDPointArray xyArrPointRc;
			xyArrPointRc.Add(pRC->m_xyStt);
			xyArrPointRc.Add(pRC->m_xyEnd);
			xyArrPointRc.Add(pRC->m_xyEndExt);
			xyArrPointRc.Add(pRC->m_xyMid1);
			xyArrPointRc.Add(pRC->m_xyMid2);
			xyArrPointRc.Add(pRC->m_xyMid3);
			xyArrPointRc.Add(pRC->m_xyMid3);
			xyArrPointRc.Add(pRC->m_xyMidExt);

			for (long nx=0; nx< xyArrPointRc.GetSize(); nx++)
			{
				CDPoint xyCurr = xyArrPointRc.GetAt(nx);
				if(nx == 0)
				{
					dMinLeft		= xyCurr.x;
					dMaxRight		= xyCurr.x;
					dMinEL			= xyCurr.y;
					dMaxEL			= xyCurr.y;
					continue;
				}

				if(xyCurr.x ==0 && xyCurr.y == 0) continue;
				
				dMinLeft = min(dMinLeft, xyCurr.x);
				dMaxRight = max(dMaxRight, xyCurr.x);
				dMinEL = min(dMinEL, xyCurr.y);
				dMaxEL = max(dMaxEL, xyCurr.y);
			}

			tvMain.m_v1.x = !bLeft? dMinLeft : dMaxRight;
			tvMain.m_v1.y = dMinEL;
			tvMain.m_v2.x = tvMain.m_v1.x;
			tvMain.m_v2.y = dMaxEL;	
		}
	}

	CTwinVectorArray tvArrUpper, tvArrUpperEnd;
	tvArrSide.GetTvArrByInfo("��ü���", tvArrUpper, TRUE, TRUE);
	tvArrSide.GetTvArrByInfo("��ü��鳡", tvArrUpperEnd, TRUE, TRUE);
	tvArrUpper.AddFromTvArray(tvArrUpperEnd);



	if(bJoint)
	{
		if(cRCNext.m_nPosStt == cRCNext.m_nPosEnd)
		{
			tvMainNext.m_v1	= cRCNext.m_xyStt;
			tvMainNext.m_v2	= cRCNext.m_xyMid1;
		}
		else if(bBoxType/*bBoxTypeNotSlabJoint*/)
		{
			double dMinLeft(0), dMaxRight(0), dMinEL(0), dMaxEL(0);
			CDPointArray xyArrPointRcNext;
			xyArrPointRcNext.Add(cRCNext.m_xyStt);
			xyArrPointRcNext.Add(cRCNext.m_xyEnd);
			xyArrPointRcNext.Add(cRCNext.m_xyEndExt);
			xyArrPointRcNext.Add(cRCNext.m_xyMid1);
			xyArrPointRcNext.Add(cRCNext.m_xyMid2);
			xyArrPointRcNext.Add(cRCNext.m_xyMid3);
			xyArrPointRcNext.Add(cRCNext.m_xyMid3);
			xyArrPointRcNext.Add(cRCNext.m_xyMidExt);

			for (long nx=0; nx< xyArrPointRcNext.GetSize(); nx++)
			{
				CDPoint xyCurr = xyArrPointRcNext.GetAt(nx);
				if(nx == 0)
				{
					dMinLeft		= xyCurr.x;
					dMaxRight		= xyCurr.x;
					dMinEL			= xyCurr.y;
					dMaxEL			= xyCurr.y;
					continue;
				}

				if(xyCurr.x ==0 && xyCurr.y == 0) continue;

				dMinLeft = min(dMinLeft, xyCurr.x);
				dMaxRight = max(dMaxRight, xyCurr.x);
				dMinEL = min(dMinEL, xyCurr.y);
				dMaxEL = max(dMaxEL, xyCurr.y);
			}

			tvMainNext.m_v1.x = !bLeft? dMinLeft : dMaxRight;
			tvMainNext.m_v1.y = dMinEL;
			tvMainNext.m_v2.x = tvMain.m_v1.x;
			tvMainNext.m_v2.y = dMaxEL;	
		}
		else
		{
			tvMainNext.m_v1	= cRCNext.m_xyStt;
			if(cRCNext.m_bUseMidExt)
				tvMainNext.m_v2	= cRCNext.m_xyMidExt;
			else if(cRCNext.m_bUseEndExt)
				tvMainNext.m_v2	= cRCNext.m_xyEndExt;
			else
				tvMainNext.m_v2	= cRCNext.m_xyEnd;
		}
	}

	if(tvMain.m_v1.y > tvMain.m_v2.y) GetSwap(tvMain.m_v1, tvMain.m_v2);
	if(tvMainNext.m_v1.y > tvMainNext.m_v2.y) GetSwap(tvMainNext.m_v1, tvMainNext.m_v2);

	// ������ �ִ��� �ٽ� ����
	if(bJoint)
	{
		if(tvMainNext.m_v1.y < tvMain.m_v2.y)
			bJoint	= TRUE;
		else
			bJoint	= FALSE;
	}
	
	// ö���� ��ν����긦 �վ����� üũ�Ѵ�.
	// �� �վ��ٸ� ��翡 ���ؼ� ���鵵���� ��ν����긦 �հ� ���� ö���� �߶� �ش�.
	// ���߿� �۾���.(�ϴ��� ��ν����긦 �հ� ���� ö���� ������ �߶����
	BOOL bProjectedUpperSlab	= FALSE;

	long nSize		= xyArr.GetSize();
	CTwinVector tv;
	CDPoint xy(0, 0), xyMatch(0, 0);
	double dHDiff	= 0;
	double dLenRebar	= tvMain.m_v2.y - tvMain.m_v1.y;
	double dCenterOfRoad	= GetCrossSectionCenterOfRoad(bLeft);
	long i = 0; for(i = 0; i < nSize; i++)
	{
		xy	= xyArr.GetAt(i);
		if(bVariableLength || bVariablePos)
		{
			dHDiff	= GetDiffHeightByCenterOfRoad(tvArrUpper, xy.x, dCenterOfRoad);
		}
		
		tv.m_v1	= CVector(xy.x, tvMain.m_v1.y + (bVariablePos ? dHDiff : 0), 0);
		tv.m_v2	= CVector(xy.x, tv.m_v1.y + dLenRebar + (bVariableLength ? dHDiff : 0), 0);

		// Ȥ�ö� ö���� ��ü�� �հ� ������ �Ǵ� ��찡 �����Ƿ� ����� ��ü �������� �׸�.
		if(!bProjectedUpperSlab)
		{
			CDPointArray xyArrMatch;
			rb.GetXyMatchTvArrAndTv(tvArrUpper, tv, xyArrMatch, TRUE);
			if(xyArrMatch.GetSize() > 0)
			{
				tv.m_v2.y	= xyArrMatch.GetAt(0).y;
			}
		}

		tvArr.Add(tv);

		if(bJoint)
		{
			double dLenJoint	= tvMain.m_v2.y - tvMainNext.m_v1.y;
			tv.m_v1	= CVector(xy.x + dGapJoint, tv.m_v2.y - dLenJoint, 0);
			tv.m_v2	= CVector(xy.x + dGapJoint, tv.m_v1.y + dLenJoint, 0);

			tvArrJoint.Add(tv);
		}
	}

	if(pRC)
		delete pRC;
}

// ��ü ���� ö�� ��ǥ��.
// bLeft : ���� ������ ����
// �ϸ��� �����ϸ��� ������.
void CWallApp::GetTvOffset_Side(CTwinVectorArray &tvArr, BOOL bLeft, long nDan)
{
	tvArr.RemoveAll();
	CRebarPlacing rb;
	double dCoverFront	= GetCoverFront(TRUE, nDan);
	double dCoverBack	= GetCoverFront(FALSE, nDan);
	double dCoverUpper	= GetCoverUpper(TRUE, 0, TRUE);
	double dCoverLower	= GetCoverUpper(FALSE, 0, TRUE);

	if(m_bApplyAngleToMainRebarCTC)
	{
		double rad	= sin(ToRadian(m_dAnglePath));
		dCoverFront	= dCoverFront / rad;
		dCoverBack	= dCoverBack / rad;
	}

	// ���ʿ� ��ü ���� ����.
	BOOL bIncludeFooting	= TRUE;
	BOOL bIncludeUpperSlab	= m_nType == WALL_TYPE_SLOPE ? TRUE : FALSE;
	CTwinVectorArray tvArrSide, tvArrFootingSide, tvArrHidden;
	GetTvLeftSide(tvArrSide, FALSE, bIncludeFooting, bIncludeUpperSlab, bLeft, 0, TRUE, FALSE);

	// �������� ����
	CTwinVector tvLeft	= tvArrSide.GetTvByInfo("��ü����"),
				tvRight	= tvArrSide.GetTvByInfo("��ü����"),
				tvLower	= tvArrSide.GetTvByInfo("��ü�ϸ�");
	CTwinVectorArray tvArrUpper;
	tvArrSide.GetTvArrByInfoRange("��ü���", "��ü��鳡", tvArrUpper);


	// ���� �������� offset
	if(tvLeft.m_v1.y > tvLeft.m_v2.y) GetSwap(tvLeft.m_v1, tvLeft.m_v2);
	if(tvRight.m_v1.y > tvRight.m_v2.y) GetSwap(tvRight.m_v1, tvRight.m_v2);
	if(tvLower.m_v1.x > tvLower.m_v2.x) GetSwap(tvLower.m_v1, tvLower.m_v2);
	Offset(tvLeft.m_v1, tvLeft.m_v2, dCoverBack);		// ��
	Offset(tvRight.m_v1, tvRight.m_v2, -dCoverFront);	// ��
	rb.GetOffsetTvArray(tvArrUpper, dCoverUpper);		// ��
	Offset(tvLower.m_v1, tvLower.m_v2, -dCoverLower);	// ��

	// offset �� �͵��� ������ ó��
	GetXyMatchLineAndLine(tvLeft.m_v2, tvLeft.GetXyDir(TRUE), tvLower.m_v1, tvLower.GetXyDir(), tvLeft.m_v1);
	tvLower.m_v1	= tvLeft.m_v1;
	GetXyMatchLineAndLine(tvRight.m_v2, tvRight.GetXyDir(TRUE), tvLower.m_v1, tvLower.GetXyDir(), tvRight.m_v1);
	tvLower.m_v2	= tvRight.m_v1;

	// tvLeft�� tvRight�� offset�ϰ� ���� tvUpper�� ���� �� �� ����(���� ������)
	// �׷� trim�� �� �ȵ�.. �ٿ�����
	CDPointArray xyArrMatch;
	rb.GetXyMatchTvArrAndLine(tvArrUpper, tvLeft.m_v1, tvLeft.GetXyDir(), xyArrMatch);
	if(xyArrMatch.GetSize() > 0)
		tvLeft.m_v2	= xyArrMatch.GetAt(0);
	rb.GetXyMatchTvArrAndLine(tvArrUpper, tvRight.m_v1, tvRight.GetXyDir(), xyArrMatch);
	if(xyArrMatch.GetSize() > 0)
		tvRight.m_v2	= xyArrMatch.GetAt(0);

	long nSize	= tvArrUpper.GetSize();
	if(nSize > 0)
	{
		rb.TrimTvArrayByLine(tvArrUpper, tvLeft.GetXyBottom(), tvLeft.GetXyTop(), TRUE, TRUE, FALSE);

		nSize	= tvArrUpper.GetSize();
		if(nSize > 0)
			rb.TrimTvArrayByLine(tvArrUpper, tvRight.GetXyBottom(), tvRight.GetXyTop(), FALSE, TRUE, FALSE);
	}
	tvArr.Add(tvLeft);
	tvArr.AddFromTvArray(tvArrUpper);
	tvArr.Add(tvRight);
	tvArr.Add(tvLower);
}

// ��ü ������� �߽� ��
// double dExtLen : ���Ʒ��� Ȯ��Ǵ� ����
CTwinVector CWallApp::GetTvCenter_Side(BOOL bLeft, double dExtLen)
{
	CTwinVectorArray tvArrSide;
	BOOL bIncludeFooting	= TRUE;
	BOOL bIncludeUpperSlab	= TRUE;
	GetTvLeftSide(tvArrSide, FALSE, bIncludeFooting, bIncludeUpperSlab, bLeft, 0, TRUE);


	CDRect rect	= tvArrSide.GetRect();
	CTwinVector tvLower	= tvArrSide.GetTvByInfo("��ü�ϸ�");
	CTwinVector tvCenter;
	tvCenter.m_v1	= CVector(tvLower.GetXyMid().x, rect.bottom, 0);
	tvCenter.m_v2	= CVector(tvLower.GetXyMid().x, rect.top, 0);

	// Ȯ����� ����.
	tvCenter.m_v1	= tvCenter.m_v1 + (tvCenter.GetXyDir(TRUE) * dExtLen);
	tvCenter.m_v2	= tvCenter.m_v2 + (tvCenter.GetXyDir() * dExtLen);

	return tvCenter;
}

// ���� �߽� ����
// ���� �ԷµȰ� �����Ƿ� ���� ���� ���� ���� �߽����� ģ��. 
// CTwinVector CWallApp::GetTvRoadCenter_Side(BOOL bLeft, double dExtLen)
// {
// 	// �߽ɼ��� ���ѵڿ� X��ǥ�� ã��.
// 	CTwinVector tvCenter	= GetTvCenter_Side(bLeft, dExtLen);
// 
// 	// x��ǥ ã��
// 	tvCenter.m_v1.x	= 0;
// 	tvCenter.m_v2.x	= 0;
// 	
// 	return tvCenter;
// }


// ��ö���� � ���� �Ǵ�(������ ���� ���� ����)
long CWallApp::GetCountRebarInfoCycle(long nCycle, long nDan, BOOL bLeft)
{
	if(nCycle == -1) nCycle = GetCountCycleMainRebar()-1;
	if(nCycle < 0 || nCycle > GetCountCycleMainRebar()-1) return FALSE;
	if(nDan == -1) nDan = MAX_REBAR_DAN-1;
	if(nDan < 0 || nDan > MAX_REBAR_DAN-1) return FALSE;

	// �ʿ��� RebarInfoCycle �迭�� ã��
	long left	= bLeft ? iLEFT : iRIGHT;
	
	CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC	= &m_pArrRCMain[nCycle][left][nDan];

	long nSize	= pArrRC->GetSize();
	return nSize;
}

// �����鿡�� ����� x��ǥ ����
CDPoint CWallApp::GetXyCenterColumn_Left(long nColumn)
{

	CTwinVector tv;
	CDPoint xyLeft(0, 0), xyRight(0, 0);
	long nSize	= m_tvArrLeftUpper.GetSize();
	// ��� ���� ��ǥ�������� �⺻ ������ �׸�.
	if(nSize == 0)
	{
		xyLeft = xyRight = CDPoint(0, m_xyOrg.y);
		xyRight.x += 10000;
	}
	else
	{
		xyLeft	= m_tvArrLeftUpper.GetAt(0).m_v1;
		xyRight	= m_tvArrLeftUpper.GetAt(nSize-1).m_v2;
	}

	// ��ս��϶� ����� ���鵵�� ������.
	CDPoint xyCen(0, 0);
	if(m_bTopIsColumn && m_nType == WALL_TYPE_VER)
	{
		double dLeft	= m_bRevDirPlacingColumn ? xyLeft.x : xyRight.x;
		double sw		= m_bRevDirPlacingColumn ? 1 : -1;
		CTwinVectorArray tvArrLeft;
	
		for(long col = 0; col < m_nCountColumn; col++)
		{
			dLeft	+= m_dDistColumn[col] * sw;
			xyCen	= GetXyMatchUpperSlab(dLeft);
			if(col == nColumn)
			{
				break;
			}
		}
	}

	return xyCen;
}

// v�� ��ü �� ����
double CWallApp::GetHeightArm(BOOL bLeft)
{
	if(m_nType != WALL_TYPE_V) return 0;

	CString sArm = bLeft ? _T("��ü�����Ȼ��") : _T("��ü�����Ȼ��");
	CTwinVector tvUpper	= m_tvArrFront.GetTvByInfo(sArm);
	CTwinVector tvLeft	= m_tvArrFront.GetTvByInfo("��ü������");

	return fabs(tvLeft.m_v1.y-tvUpper.GetXyMid().y);
}

// v�� ��ü �߽ɿ��� �ȱ��� �Ÿ�
double CWallApp::GetDistArm(BOOL bLeft)
{
	if(m_nType != WALL_TYPE_V) return 0;

	CString sArm = bLeft ? _T("��ü������������") : _T("��ü�����ȿ�����");
	CTwinVector tvArm	= m_tvArrFront.GetTvByInfo(sArm);
	
	return tvArm.GetHorLength();
}
// ��ü ��ö�� ö�� ����
void CWallApp::SetDataDefaultRebarJewonMain()
{
	CHgBaseConcStd baseConc;
	baseConc.ClearRebarJewon(&m_pArrRebarMain, TRUE);

	if(!m_bIs) return;
	if(m_bTopIsColumn) return;
	if(m_bNotRebar) return;
	if(!m_pBasicConcInfo) return;

	CRebarPlacing rb;

	long cycle(0), left(0), dan(0), nIdx(0), nSize(0);
	BOOL bLeft(TRUE);
	CString sCycle(_T("")), sLeft(_T(""));

	double dCover(0), dRebarSpace(0);
	double dLenRound(0);
	double dLenMax(0), dLenMin(0);

	CTwinVectorArray tvArr, tvArrJoint;
	CTwinVector tv;
	CRebarInfoCycle *pRC	= NULL;

	dRebarSpace	= m_dCTC_MainRebar/2;
	
	// ��ö��
	for(cycle = 0; cycle < GetCountCycleMainRebar(); cycle++)
	{  
		sCycle	= GetCircleText(cycle+1);
	
		for(left = 0; left < 2; left++)
		{
			bLeft	= left == iLEFT ? TRUE : FALSE;
			sLeft	= bLeft ? "����" : "����";
			dCover	= bLeft ? m_dCoverLeft[0] : m_dCoverRight[0];
			
			for(dan = 0; dan < MAX_REBAR_DAN; dan++)
			{
				for(nIdx = 0; nIdx < GetCountRebarInfoCycle(cycle, dan, bLeft); nIdx++)
				{
					pRC	= GetRebarInfoCycle(cycle, dan, bLeft, nIdx);
					if(!pRC) continue;
					if(!pRC->m_bExist) continue;
					GetTvRebarMain_Side(tvArr, bLeft, cycle, dan, nIdx, tvArrJoint, TRUE);

					// Round�������� ���� �ϴ� ���� 
					dLenRound	= 0;
					if(pRC->m_nPosStt != pRC->m_nPosEnd)
						dLenRound	= fabs(pRC->m_xyMid2.y - pRC->m_xyMid1.y);

					// ���󿡼� �ִ�� �ּ� ���̸� �Ǵ��ؼ� ö�� ������.
					dLenMax	= tvArr.GetLengthMax();
					dLenMin	= tvArr.GetLengthMin();
					nSize	= tvArr.GetSize();
					if(nSize > 0)
					{
						CRebar *pRB	= new CRebar;
						pRB->m_Fy			= m_Fy;
						pRB->m_sDescription	= pRC->m_sDescription;//.Format("�߰����� %s�� %d�� %s %d", sLeft, dan+1, sCycle, nIdx+1);
						pRB->m_sMark		= pRC->m_szMark;
						pRB->m_dDia			= pRC->m_Dia;
						if(pRC->m_nPosStt == pRC->m_nPosEnd)
						{
							pRB->m_dLengthMax_A	= dLenMax;
							pRB->m_dLengthMin_A	= dLenMin;
							pRB->m_nTypeRebar	= 1;
						}
						else
						{
							pRB->m_dLengthMax_A	= dLenMax - dLenRound;
							pRB->m_dLengthMin_A	= dLenMin - dLenRound;
							pRB->m_dLengthMax_B	= pRC->GetLengthMid(TRUE);//rb.GetArcLength(pRC->m_xyCen, pRC->m_xyMid1, pRC->m_xyMid2);
							pRB->m_dLengthMax_C	= fabs(pRC->m_xyMid1.x - pRC->m_xyStt.x);
							pRB->m_dLengthMin_C	= pRB->m_dLengthMax_C;
							pRB->m_nTypeRebar	= 5;
							pRB->m_R			= pRC->m_R_Mid;
						}
						pRB->m_bMainRebar	= TRUE;
						pRB->m_nEa			= tvArr.GetSize();
	
						baseConc.OptimizationRebar(pRB);	// �߰��ϱ� ���� ����ȭ �ѹ� ���ش�.
						SetRebarLsdOption(pRB->m_pOptionLsd, bLeft, TRUE, cycle, dan);
						m_pBasicConcInfo->SetLengthRebar(m_Fck, m_Fy, pRB, JOINT_TENS_NORMAL_B, dCover, dRebarSpace);
						pRB->m_dRateAdd	= MAKE_RATE;
						if(baseConc.SetRebarJewon(&m_pArrRebarMain, pRB->m_sDescription, pRB->m_sMark))
						{
							delete pRB;
						}
						else
						{
							m_pArrRebarMain.Add(pRB);
						}
					}
				}
			}
		}
	}
}

// ������ü ���鵵 ���� 2���̻��� ö���� ���ؼ� �߶󳻾� �Ǵ� ������ ������.
// bUpper�� TRUE�̸� ����, FALSE�̸� �Ʒ���
// ��, �ش� ������ ö���� ���ٴ°� ��������, null�� ����������.
void CWallApp::GetTvCuttingArea_Side(CTwinVectorArray &tvArr, BOOL bLeft, long nDan, BOOL bUpper)
{
	tvArr.RemoveAll();

	// �ش�  ������ ö���� �ִ��� �ľ��� ����.
	CRebarInfoCycle *pRC	= GetRebarInfoCycle(0, nDan, bLeft, bUpper ? -1 : 0);
	if(!pRC) return;
	
	// �ϸ��� ��� ��ü ö���� �ƴϸ� ���� �ʴ´�.
	if(!bUpper)
	{
		//if(pRC->m_nPosEnd != REBARINFO_POS_INWALL_LEFT && pRC->m_nPosEnd != REBARINFO_POS_INWALL_RIGHT) return;
	}

	CTwinVectorArray tvArrSide;
	GetTvLeftSide(tvArrSide);

	// ���� area ����� (�ɼǿ� ���� �ٲ� ���� ����, �ٲ� ��� tvArr�� �����ϰ� �ٲ��ָ� ��(��ũ�� ���� �����̸� � ����̵� ����))
	// ���η� ������ Ʈ����..
	// Rũ��� ��ü ����/16
	// ���� ���� ���̴� ��ü ����/2	
	CDRect rect	= tvArrSide.GetRect();
	double dDiff	= nDan == 1 ? rect.Width()/8 : rect.Width()/8*3;
	double dX	= bLeft ? rect.left + dDiff : rect.right - dDiff;
	CDPoint xyMatchU	= GetXyMatchUpperSlab(dX);
	CDPoint xyMatchL	= xyMatchU;
	CTwinVectorArray tvArrLeftFooting, tvArrHidden;
	m_footing.GetTvLeftSide(tvArrLeftFooting, tvArrHidden, FALSE);
	CTwinVector tvLower	= tvArrLeftFooting.GetTvByInfo("�����ϸ�");
	GetXyMatchLineAndLine(xyMatchL, CDPoint(0, -1), tvLower.m_v1, tvLower.GetXyDir(), xyMatchL);
	
	double dY	= bUpper ? xyMatchU.y : xyMatchL.y;
	double dR	= rect.Width()/15;
	double dH	= fabs(xyMatchU.y - xyMatchL.y)/4;

	// Ʈ���� �����.(Ʈ���� ���� �߽� �Ʒ����� ����)
	CTwinVector tv;
	tv.m_v1		= CVector(dX+dR, dY-dH/2, 0);
	tv.m_v2		= tv.m_v1 + CVector(0, dH, 0);
	tv.m_sInfo	= "��������";
	tvArr.Add(tv);

	tv.m_v1		= tv.m_v2;
	tv.m_v1.z	= 1;
	tv.m_v2		= tv.m_v1 + CVector(-dR*2, 0, dR);
	tv.m_sInfo	= bUpper ? "�׸�������" : "��ܾ�ũ";
	tvArr.Add(tv);

	tv.m_v1		= tv.m_v2;
	tv.m_v1.z	= 0;
	tv.m_v2		= tv.m_v1 + CVector(0, -dH, 0);
	tv.m_sInfo	= "��������";
	tvArr.Add(tv);

	tv.m_v1		= tv.m_v2;
	tv.m_v1.z	= 1;
	tv.m_v2		= tv.m_v1 + CVector(dR*2, 0, dR);
	tv.m_sInfo	= bUpper ? "�ϴܾ�ũ" : "�׸�������";
	tvArr.Add(tv);

	CRebarPlacing rb;
	rb.MoveRebar(tvArr, CDPoint(0, bUpper ? -dH/4 : dH/4));
}

// tvArr�� ���� ���� �����ؼ� �����ش�.
// ����θ� �������� �׸��� ���ؼ�..
void CWallApp::DivideTvArrayByRound(CTwinVectorArray &tvArr, CTwinVectorArray &tvArrRound, double dLenRound, BOOL bRoundAtStt)
{
	CTwinVector tv, tvSolid, tvHidden;
	long nSize	= tvArr.GetSize();

	long i = 0; for(i = 0; i < nSize; i++)
	{
		tv	= tvArr.GetAt(i);
		if(bRoundAtStt)
		{
			tvHidden.m_v1	= tv.m_v1;
			tvHidden.m_v2	= tv.m_v1 + (tv.GetXyDir()*dLenRound);
			tvSolid.m_v1	= tvHidden.m_v2;
			tvSolid.m_v2	= tv.m_v2;
		}
		else
		{
			tvHidden.m_v1	= tv.m_v2 + (tv.GetXyDir(TRUE) * dLenRound);
			tvHidden.m_v2	= tv.m_v2;
			tvSolid.m_v1	= tv.m_v1;
			tvSolid.m_v2	= tvHidden.m_v1;
		}
		
		tvArr.SetAt(i, tvSolid);
		tvArrRound.Add(tvHidden);
	}
}

void CWallApp::SetDataDefaultRebarInfoCycle(long nCycle, BOOL bOnlyCountArray, long nDan, BOOL bOnlyDiffJoint)
{
	if(nDan > 0 && !m_bDetailInput23DanMainRebar) return;

	long stt	= nCycle == -1 ? 0 : nCycle;
	long end	= nCycle == -1 ? GetCountCycleMainRebar()+1 : nCycle+1;

	if(stt < 0 || stt > GetCountCycleMainRebar()+1) return;
	if(end < 0 || end > GetCountCycleMainRebar()+1) return;

	long joint(0), left(0);
	double dMove	= 0;

	long cycle = 0; for(cycle = stt; cycle < end; cycle++)
	{
		BOOL bCycle34	= nCycle == 0 || nCycle == 1 ? FALSE : TRUE;

		// ��ü�� �Ÿ��� �����̳� ��� ���� ������ŭ�� ������ �ȴ�.
		for(left = 0; left < 2; left++)
		{
			long nCountJoint	= nCycle == 0 || nCycle == 2 ? m_nCountJointMainRebar_Cycle1[left][nDan] : m_nCountJointMainRebar_Cycle2[left][nDan];

			// ���������� ����� ��쿡�� �ǰ�� �ϵ��� �ϴ� ��� /////////
			if(bOnlyDiffJoint)
			{
				BOOL bDiff	= FALSE;
				long nCountJoint	= nCycle == 0 || nCycle == 2 ? m_nCountJointMainRebar_Cycle1[left][nDan] : m_nCountJointMainRebar_Cycle2[left][nDan];
				long nCountJointOld	= nCycle == 0 || nCycle == 2 ? m_nCountJointMainRebar_Cycle1_Old[left][nDan] : m_nCountJointMainRebar_Cycle2_Old[left][nDan];
				if(nCountJoint != nCountJointOld)
					bDiff	= TRUE;

				if(!bDiff) continue;
			}
			//////////////////////////////////////////////////////////////////

			double dHeight		= GetTvOffset_Front(cycle, nDan, FALSE, TRUE, TRUE, TRUE).GetVerLength();
			dHeight += (m_dThickUppeSlab + m_dThickHunch);
			if(m_footing.m_bIs == FALSE && m_footing.IsVirtualFooting() == FALSE)
			{
				// m_footing.m_nType == FOOTING_TYPE_BOX
				CTwinVector tvWall;
				tvWall = GetTvOffset_Front(cycle, nDan, FALSE, TRUE, TRUE, TRUE);
				GetXyMatchLineAndLine(tvWall.m_v1, tvWall.GetXyDir(), m_tvUpperLimitOfMainRebar.m_v1, m_tvUpperLimitOfMainRebar.GetXyDir(), tvWall.m_v1);
				GetXyMatchLineAndLine(tvWall.m_v1, tvWall.GetXyDir(), m_tvLowerLimitOfMainRebar.m_v1, m_tvLowerLimitOfMainRebar.GetXyDir(), tvWall.m_v2);
				dHeight = tvWall.GetVerLength();
			}

			CDoubleArray dArrDistRC;
			CDoubleArray dArrDiaRC;
			CDoubleArray dArrExistRC;
			dArrDistRC	= m_dArrDistRC[cycle][left][nDan];
			dArrDiaRC	= m_dArrDiaRC[cycle][left][nDan];
			dArrExistRC	= m_dArrExistRC[cycle][left][nDan];

			m_dArrDistRC[cycle][left][nDan].RemoveAll();
			m_dArrDiaRC[cycle][left][nDan].RemoveAll();
			m_dArrExistRC[cycle][left][nDan].RemoveAll();
			dMove	= GetDistMoveJointCycle13(cycle==2);

			// �Ÿ�/����
			if(!bCycle34)
			{
				for(joint = 0; joint < nCountJoint; joint++)
				{
					m_dArrDistRC[cycle][left][nDan].Add(Round(dHeight/(nCountJoint+1)+dMove, -1));
					m_dArrDiaRC[cycle][left][nDan].Add(22);
					m_dArrExistRC[cycle][left][nDan].Add((double)nDan == 0 ? TRUE : FALSE);
				}
				m_dArrDiaRC[cycle][left][nDan].Add(22);
				m_dArrExistRC[cycle][left][nDan].Add((double)nDan == 0 ? TRUE : FALSE);

				if(bOnlyCountArray)
				{
					long i = 0; for(i = 0; i < m_dArrDistRC[cycle][left][nDan].GetSize(); i++)
					{
						if(i < dArrDistRC.GetSize())
							m_dArrDistRC[cycle][left][nDan].SetAt(i, dArrDistRC.GetAt(i));
					}

					for(i = 0; i < m_dArrDiaRC[cycle][left][nDan].GetSize(); i++)
					{
						if(i < dArrDiaRC.GetSize())
							m_dArrDiaRC[cycle][left][nDan].SetAt(i, dArrDiaRC.GetAt(i));
					}

					for(i = 0; i < m_dArrExistRC[cycle][left][nDan].GetSize(); i++)
					{
						if(i < dArrExistRC.GetSize())
							m_dArrExistRC[cycle][left][nDan].SetAt(i, dArrExistRC.GetAt(i));
					}
				}
			}
			else
			{
				m_dArrDistRC[cycle][left][nDan]	= m_dArrDistRC[cycle == 2 ? 0 : 1][left][nDan];
				m_dArrDiaRC[cycle][left][nDan]	= m_dArrDiaRC[cycle == 2 ? 0 : 1][left][nDan];
				m_dArrExistRC[cycle][left][nDan]	= m_dArrExistRC[cycle == 2 ? 0 : 1][left][nDan];

				long i = 0; for(i = 0; i < m_dArrDistRC[cycle][left][nDan].GetSize(); i++)
					m_dArrDistRC[cycle][left][nDan].SetAt(i, m_dArrDistRC[cycle][left][nDan].GetAt(i)+(i == 0 ? dMove*2 : 0));
			}
		}
		
		m_setRebarDan[iLEFT].m_dDia[cycle][nDan]	= 19;
		m_setRebarDan[iRIGHT].m_dDia[cycle][nDan]	= 19;
	}
}

// ��ü ���ö�� �ǰ��.
void CWallApp::SetDataDefaultRebarJewonSupport(long nBaseNum)
{
	ClearRebarList(1);
	if(!m_bIs) return;
	if(m_bTopIsColumn) return;
	if(m_bNotRebar) return;
	if(!m_pBasicConcInfo) return;
	
	CHgBaseConcStd baseConc;
	CTwinVectorArray tvArr, tvArrAngle, tvArrAngleAdd;
	double dLenMax(0), dLenMin(0);

	double dCover(0);
	double dRebarSpace	= 0;
	if(nBaseNum != -1)
		GetMarkRebarSupport(0, FALSE, TRUE, nBaseNum);
	
	long left = 0; for(left = 0; left < 2; left++)
	{
		BOOL bLeft		= left == iLEFT;
		CString sLeft	= bLeft ? "����" : "����";

		// ���� ��ü�� ��� �¿��� ���о��� ���� ������ ö������ �����
		// ���� ������ ���ؼ��� ������ �ʿ䰡 ����
		if(IsRoundRectWall() && !bLeft) continue;	
		
		long dan = 0; for(dan = 0; dan < MAX_REBAR_DAN; dan++)
		{
			long nTypeRebar	= bLeft ? W_REBAR_S_L1+dan : W_REBAR_S_R1+dan;

			dCover		= bLeft ? m_dCoverLeft[dan] : m_dCoverRight[dan];
			dRebarSpace	= GetCTCRebarSupport(bLeft, dan, TRUE);
			CPlaceRebarByBlock *pPlace	= &m_placeByBlock_SupportRebar[left][dan];
			if(pPlace->m_dDia == 0) continue;

			
			// ���� ��ü�� ���
			if(IsRoundRectWall())
			{
				CRebar *pRB	= new CRebar;
				// ö�� ����
				CDPointArray xyArr;
				GetXyRebarSupport_Front(xyArr, TRUE, dan, FALSE, TRUE);

				CTwinVectorArray tvArrRebarPlane;
				m_Column.GetTvRebarRound_Plan(tvArrRebarPlane, dan);

				pRB->m_Fy			= m_Fy;
				pRB->m_sDescription.Format("�߰����� %d�� ���ö��", dan+1);
				pRB->m_sMark		= GetMarkRebarSupport(nTypeRebar+dan, FALSE, TRUE, -1);
				pRB->m_dDia			= pPlace->m_dDia;
				pRB->m_nTypeRebar	= 51;
				pRB->m_dLengthMax_A	= tvArrRebarPlane.GetTvByInfo("��մܸ�������").GetLength();
				double dR			= fabs(tvArrRebarPlane.GetTvByInfo("��մܸ�����������").m_v2.z);
				pRB->m_dLengthMax_B	= 2 * ConstPi * dR / 2;
				pRB->m_R			= dR;
				pRB->m_bMainRebar	= FALSE;
				pRB->m_nEa			= xyArr.GetSize();

				// ���� ���� ���� ���������� ���̸� ������ ����� �ѵ� * 2�� �Ѵ�.
				CRebar rbTmp;
				rbTmp	= *pRB;
				rbTmp.m_nTypeRebar	= 1;
				rbTmp.m_dLengthMax_A	= pRB->m_dLengthMax_A;

				SetRebarLsdOption(pRB->m_pOptionLsd, bLeft, FALSE, 0, dan);
				m_pBasicConcInfo->SetLengthRebar(m_Fck, m_Fy, &rbTmp, JOINT_TENS_NORMAL_B, dCover, dRebarSpace);

				pRB->m_nJoint	= rbTmp.m_nJoint * 2;
				pRB->m_dRateAdd	= MAKE_RATE;

				if(baseConc.SetRebarJewon(&m_pArrRebarSupport, pRB->m_sDescription, pRB->m_sMark))
				{
					delete pRB;
				}
				else
				{
					m_pArrRebarSupport.Add(pRB);
				}
			}
			else
			{
				GetTvRebarSupport_Side(tvArr, tvArrAngle, tvArrAngleAdd, bLeft, dan);

				// ���� ö��
				dLenMax	= tvArr.GetLengthMax();
				dLenMin	= tvArr.GetLengthMin();
				if(Compare(dLenMax, 0.0, "=")) continue;

				CRebar *pRB	= new CRebar;
				pRB->m_Fy			= m_Fy;
				pRB->m_sDescription.Format("�߰����� %s %d�� ���ö��", sLeft, dan+1);
				pRB->m_sMark		= GetMarkRebarSupport(nTypeRebar+dan, FALSE, TRUE, -1);
				pRB->m_dDia			= pPlace->m_dDia;
				pRB->m_dLengthMax_A	= dLenMax;
				pRB->m_dLengthMin_A	= dLenMin;
				pRB->m_nTypeRebar	= 1;
				pRB->m_bMainRebar	= FALSE;
				pRB->m_nEa			= tvArr.GetSize();
				baseConc.OptimizationRebar(pRB);	// ö�� ����ȭ

				SetRebarLsdOption(pRB->m_pOptionLsd, bLeft, FALSE, 0, dan);
				m_pBasicConcInfo->SetLengthRebar(m_Fck, m_Fy, pRB, JOINT_TENS_NORMAL_B, dCover, dRebarSpace);
				pRB->m_dRateAdd	= MAKE_RATE;

				if(baseConc.SetRebarJewon(&m_pArrRebarSupport, pRB->m_sDescription, pRB->m_sMark))
				{
					delete pRB;
				}
				else
				{
					m_pArrRebarSupport.Add(pRB);
				}

				// ���̴� ö��
				CString sMarkLast	= pRB->m_sMark;
				for(long add = 0; add < 2; add++)
				{
					CTwinVectorArray *pTvArr = add == 0 ? &tvArrAngle : &tvArrAngleAdd;
					long nCount	= 0;
					dLenMax	= 0;
					dLenMin	= 0;
					CString sInfo	= _T("");
					CTwinVector tv;
					CTwinVectorArray tvArrRebar;
					long i = 0; for(i = 0; i < pTvArr->GetSize(); i++)
					{
						tv	= pTvArr->GetAt(i);

						tvArrRebar.RemoveAll();
						tvArrRebar.Add(tv);
						long j = 0; for(j = i+1; j < pTvArr->GetSize(); j++)
						{
							if(tv.m_sInfo == pTvArr->GetAt(j).m_sInfo)
							{
								tvArrRebar.Add(pTvArr->GetAt(j));
								i++;
							}
							else
								break;
						}

						nCount++;
						if(dLenMax == 0 && dLenMin == 0)
						{
							dLenMax	= tvArrRebar.GetLength();
							dLenMin	= tvArrRebar.GetLength();
						}
						else
						{
							dLenMax	= max(dLenMax, tvArrRebar.GetLength());
							dLenMin	= min(dLenMin, tvArrRebar.GetLength());
						}
					}

					if(nCount > 0)
					{
						CString sMark	= sMarkLast;
						pRB	= new CRebar;
						pRB->m_Fy			= m_Fy;
						pRB->m_sDescription.Format("�߰����� %s %d�� ���ö��%d", sLeft, dan+1, 2+add);
						pRB->m_sMark.Format("%s-%d", sMark, 1+add);
						pRB->m_dDia			= pPlace->m_dDia;
						pRB->m_dLengthMax_A	= dLenMax;
						pRB->m_dLengthMin_A	= dLenMin;
						pRB->m_nTypeRebar	= 1;
						pRB->m_bMainRebar	= FALSE;
						pRB->m_nEa			= nCount;
						baseConc.OptimizationRebar(pRB);	// ö�� ����ȭ

						SetRebarLsdOption(pRB->m_pOptionLsd, bLeft, FALSE, 0, dan);
						m_pBasicConcInfo->SetLengthRebar(m_Fck, m_Fy, pRB, JOINT_TENS_NORMAL_B, dCover, dRebarSpace);
						pRB->m_dRateAdd	= MAKE_RATE;

						if(baseConc.SetRebarJewon(&m_pArrRebarSupport, pRB->m_sDescription, pRB->m_sMark))
						{
							delete pRB;
						}
						else
						{
							m_pArrRebarSupport.Add(pRB);
						}
					}
				}
			}

		}
	}
}

double CWallApp::GetCTCRebarSupport(BOOL bLeft, long nDan, BOOL bStd)
{
	long left	= bLeft ? iLEFT : iRIGHT;

	CPlaceRebarByBlock *pPlace	= &m_placeByBlock_SupportRebar[left][nDan];
	CDPointArray xyArr;
	xyArr	= pPlace->m_xyArr;
	if(xyArr.GetSize() == 0 || bStd)
	{
		CHgBaseConcStd baseConc;
		return m_dCTC_SupportRebar;
	}
	else
	{
		return pPlace->GetCTC();
	}
}

// ö�� �ʱ�ȭ
// long nType 
// 0 : ��ö��, 1 : ���ö��, 2 : ����ö��, 3 : ���ö��,
void CWallApp::ClearRebarList(long nType)
{
	CHgBaseConcStd baseConc;
	if(nType == 0)
	{}
	else if(nType == 1)
	{
		// ��� ö��
		baseConc.ClearRebarJewon(&m_pArrRebarSupport, TRUE);
	}
	else if(nType == 2)
	{
		// ���� ö��
		baseConc.ClearRebarJewon(&m_pArrRebarShear, TRUE);
	}
	else if(nType == 3)
	{
		// ���ö��
		baseConc.ClearRebarJewon(&m_pArrRebarColumn, TRUE);
	}
	
}

// ����ö�� ���鵵
void CWallApp::GetTvRebarShear_Side(CTwinVectorArray &tvArr, BOOL bLeftWall)
{
	tvArr.RemoveAll();
	if(m_bNotRebar) return;
	CRebarPlacing rb;

	// ���ӽ����� ���� ��ġ�� ��±ٰ� ��ö�� ó�� ö�ٰ��� �������̴�.
	CTwinVectorArray tvArrJoint;
	CTwinVectorArray tvArrMain, tvArrSup, tvArrSupAngle, tvArrSupAngleAdd;
	GetTvRebarMain_Side(tvArrMain, TRUE, 0, 0, 0, tvArrJoint, FALSE);
	GetTvRebarSupport_Side(tvArrSup, tvArrSupAngle, tvArrSupAngleAdd, TRUE, 0);

	// ����ö�� ��ġ ��ġ
	CTwinVectorArray tvArrShear;
	GetTvRebarShear_Front(tvArrShear);
	if(m_nType == WALL_TYPE_SLOPE)
	{
		CDPoint xyOrg	= GetXyOrgTopForFront(bLeftWall);
		double dAng		= GetXyDirRotate_Front().GetAngleDegree();
		rb.RotateTvArr(tvArrShear, xyOrg, dAng);
	}

	// ���� ��±��̳� ��ö���� ������ ����ö�� ����� �� �ȴ�.
	long nIdxSttMain	= 1;	// ��ö�� �� ��° ���� ��ġ�ϱ� ��������?
	if(tvArrMain.GetSize() < nIdxSttMain+1 || tvArrSup.GetSize() == 0) return;
	
	// ���ӽ����� ��ü �ܰ��� ���ؼ� ����ö���� ��ġ�� �ڿ� ����ö���� �� �� ��� ���ԵǴ°Ÿ� �츲.
	double dLenShear	= GetLenShear();		// ����ö�� ����
	CDPoint xyDirShear	= CDPoint(1, 0);

	// ����ö�� ��ġ ���� ��ġ
	CTwinVector tvMain	= tvArrMain.GetAt(nIdxSttMain);
	CTwinVector tvSup	= tvArrSup.GetAt(0);
	CDPoint xyStt		= CDPoint(0, 0);
	GetXyMatchLineAndLine(tvMain.m_v1, tvMain.GetXyDir(), tvSup.m_v1, tvSup.GetXyDir(), xyStt);

	//������������ ���� ��ġ�� ����..
	if(m_dWidthExp[iLEFT] > 0)
		xyStt.x -= m_dWidthExp[iLEFT];

	// ��ġ�Ѵ�.
	CTwinVector tv;
	CTwinVector tvLimit	= tvArrMain.GetAt(tvArrMain.GetSize()-1);
	double dRange	= (tvLimit.m_v1.x - tvMain.m_v1.x);	// ����ö�� ���� ����
	// �����϶��� ������ �׸����� ���ȿ��� �׸��Ƿ� �� ���̰�����, ��������ŭ ���ؼ� ����
	// ���踦 ��Ȯ�ϰ� �ϱ� ����
	if(m_dWidthExp[iLEFT] > 0)
		dRange += m_dWidthExp[iLEFT];
	if(m_dWidthExp[iRIGHT] > 0)
		dRange += m_dWidthExp[iRIGHT];
	//
	long nCount	= (long)(dRange / dLenShear);
	long nSize	= tvArrShear.GetSize();
	BOOL bAddAtFirst	= TRUE;
	long i = 0; for(i = 0; i < nSize; i++)
	{
		xyStt.y	= tvArrShear[i].m_v1.y;
		tv.m_v1	= xyStt;

		long nIdxLast	= bAddAtFirst ? nCount/2+1 : nCount/2;
		long j = 0; for(j = 0; j < nIdxLast; j++)
		{
			if(j == 0 && !bAddAtFirst)
				tv.m_v1 = tv.m_v1 + xyDirShear * dLenShear;
			tv.m_v2		= tv.m_v1 + xyDirShear * dLenShear;
			tv.m_sInfo	= "����ö��";

			tvArr.Add(tv);
			tv.m_v1		= tv.m_v1 + xyDirShear * (dLenShear*2);
		}
		bAddAtFirst	= !bAddAtFirst;
	}
}

// ����ö�� ���� 
double CWallApp::GetLenShear()
{
	double dLen	= 0;
	CHgBaseConcStd baseConc;

	// ǥ�ذ���/2 * ���δ� ����
	double dCTC	= m_bApplyAngleToMainRebarCTC ? m_dCTC_MainRebar/sin(ToRadian(m_dAnglePath)) : m_dCTC_MainRebar;

	

	dLen	= dCTC/2 * m_nCountWrapMainRebar;

	return dLen;
}

void CWallApp::SetDataDefaultRebarJewonShear(long nBaseNum)
{
	ClearRebarList(2);
	if(!m_bIs) return;
	if(m_bTopIsColumn) return;
	if(m_bNotRebar) return;
	if(!m_pBasicConcInfo) return;

	CHgBaseConcStd baseConc;
	CTwinVectorArray tvArr;
	double dLenMaxA(0), dLenMinA(0);
	double dLenMaxB(0), dLenMinB(0);
	
	double dCover(0);
	double dRebarSpace	= 0;
	if(nBaseNum != -1)
		GetMarkRebarShear(0, FALSE, TRUE, nBaseNum);
	
	dCover		= m_dCoverLeft[0];
	dRebarSpace	= GetCTCRebarShear();

	// ����ö�� ����
	GetTvRebarShear_Side(tvArr, TRUE);
	long nCount	= tvArr.GetSize();
	dLenMaxA	= tvArr.GetLengthMax();
	dLenMinA	= tvArr.GetLengthMin();

	// ����ö�� ����
	GetTvRebarShear_Front(tvArr);
	dLenMaxB	= tvArr.GetLengthMax();
	dLenMinB	= tvArr.GetLengthMin();

	// ������
	CPlaceRebarByBlock *pPlace	= &m_placeByBlock_ShearRebar;
	double dSettle	= pPlace->m_dDia <= 10 || pPlace->m_dDia >= 35 ? 0 : m_pBasicConcInfo->GetValueJoint(m_Fck, m_Fy, (long)pPlace->m_dDia, JOINT_TENS_NORMAL_B, dCover, dRebarSpace);
	double dDiaMainUpper	= GetDiaMainRebar(0, 0, TRUE, 0);
	double dDiaMainLower	= GetDiaMainRebar(0, 0, FALSE, 0);
	
	CRebar *pRB	= new CRebar;
	pRB->m_Fy			= m_Fy;
	pRB->m_sDescription.Format("�߰����� ����ö��");
	pRB->m_sMark		= GetMarkRebarShear(W_REBAR_V, FALSE, TRUE, -1);
	m_rebarShearDetail.SetRB(pRB, dDiaMainUpper, dDiaMainLower, dLenMaxA, dLenMaxB, dSettle);
	m_rebarShearDetail.SetMin(dLenMinA, dLenMinB);
	m_rebarShearDetail.SetDataDefaultRebarJewon();
	/*
	pRB->m_dDia			= pPlace->m_dDia;
	pRB->m_dLengthMax_A	= dLenMaxA + pRB->m_dDia + (dDiaMainUpper+dDiaMainLower)/2;
	pRB->m_dLengthMin_A	= 0;
	pRB->m_dLengthMax_B	= dLenMaxB;
	pRB->m_dLengthMin_B	= 0;
	pRB->m_dLengthMax_C	= dSettle;
	pRB->m_dLengthMin_C	= 0;
	pRB->m_nTypeRebar	= 16;
	*/
	pRB->m_bMainRebar	= FALSE;
	pRB->m_nEa			= nCount;
	baseConc.OptimizationRebar(pRB);	// ö�� ����ȭ
	m_pBasicConcInfo->SetLengthRebar(m_Fck, m_Fy, pRB, JOINT_TENS_NORMAL_B, dCover, dRebarSpace);
	pRB->m_dRateAdd	= MAKE_RATE;
	if(baseConc.SetRebarJewon(&m_pArrRebarShear, pRB->m_sDescription, pRB->m_sMark))
	{
		delete pRB;
	}
	else
	{
		m_pArrRebarShear.Add(pRB);
	}
}

double CWallApp::GetCTCRebarShear()
{
	CPlaceRebarByBlock *pPlace	= &m_placeByBlock_ShearRebar;
	CDPointArray xyArr;
	xyArr	= pPlace->m_xyArr;
	if(xyArr.GetSize() == 0)
	{
		CHgBaseConcStd baseConc;
		return m_dCTC_ShearRebar;
	}
	else
	{
		return pPlace->GetCTC();
	}
}

// ����ö�� �߶󳻴� ���� ���ϱ�
// 0 : �¿����� ���� �ݾ� �ڸ�
// 1 : �¿����� ���� ��(�߽ɿ��� �¿�� �̵��� ��ġ�� ����)
void CWallApp::GetTvCuttingArea_Side_Shear(CTwinVectorArray &tvArr, long nDrawLeft)
{
	tvArr.RemoveAll();

	// 0 : �¿����� ���� �ݾ� �ڸ�
	// 1 : �¿����� ���� ��(�߽ɿ��� �¿�� �̵��� ��ġ�� ����)
	const long nTypeCuttingShear	= 1;

	CRebarPlacing rb;

	if(nTypeCuttingShear == 0)
	{
		CTwinVector tvCenter	= GetTvCenter_Side(TRUE, 0);
		CTwinVector tv;
		if(nDrawLeft == -1 || nDrawLeft == 1)
		{
			BOOL bLeft	= nDrawLeft == 1;
			CTwinVectorArray tvArrSide;
			GetTvLeftSide(tvArrSide, FALSE, TRUE);
		
			rb.TrimTvArrayByLine(tvArrSide, tvCenter.m_v1, tvCenter.m_v2, bLeft, TRUE, FALSE);
			tvArrSide.Add(tvCenter);

			tvArr.AddFromTvArray(tvArrSide);
		}
	}
	else if(nTypeCuttingShear == 1)
	{
		// ��..
		CTwinVector tvCenter	= GetTvCenter_Side(TRUE, 0);
		CTwinVector tv;

		double dDivideCen	= 8.0; //���� 6.0
		double dDividePlan = 8.0; 

		double dR				= min(tvCenter.GetVerLength()/dDivideCen, m_tvPathPlan.GetLength()/dDividePlan);
		double sw				= nDrawLeft == -1 ? -1 : 1;
		if(m_dWidthExp[iLEFT] > 0)
			tvCenter.m_v1.x = tvCenter.m_v2.x -= m_dWidthExp[iLEFT];
		
		tv.m_v1	= CVector(tvCenter.GetXyMid().x, tvCenter.GetXyMid().y, -1);
		tv.m_v2	= CVector(tv.m_v1.x - dR*2, tv.m_v1.y, dR);
		tvArr.Add(tv);

		tv.m_v1	= CVector(tv.m_v2.x, tv.m_v2.y, -1);
		tv.m_v2	= CVector(tv.m_v1.x + dR*2, tv.m_v1.y, dR);
		tvArr.Add(tv);

		// ARCBRIDGE-2794 (������ �߶� ����Ҷ��� �����)
		if(m_nTypeOutputMainRebar_Side == 1)
		{
			CTwinVector tvCutLine;
			double dRCen = 0, dAngStt =0 , dAngEnd =0;
			GetInfoCuttingCircle_Side(TRUE, tvCutLine.m_v1, dRCen, dAngStt, dAngEnd);

			//Ŀ�ö��� ���� x�� ���� ū �ʰ� ���� EL ���� ���� 
			tvCutLine.m_v1.x += dRCen;
			tvCutLine.m_v2.x += dRCen;
			tvCutLine.m_v1.y = tvCenter.m_v1.y;
			tvCutLine.m_v2.y = tvCenter.m_v2.y;

			//���ͼ����� Ŀ�ü������� ���̸� ����
			double dDistCen = tvCenter.GetXyLeft().x - tvCutLine.GetXyLeft().x;
			dR = max(dR, dDistCen);
		}		
		

		CRebarPlacing rb;
		rb.MoveRebar(tvArr, CDPoint(sw, 0)*dR);
	}
}

// ��� ������ ����
// ����� �������ε�, ������ �ϳ� �ۿ� ���� ������ �ε����� ���� ����� �޸� �����ؼ� ��������
CColumnApp* CWallApp::GetColumn(long nIdx)
{
	CDPoint xyLeft(0, 0), xyRight(0, 0);
	long nSize	= m_tvArrLeftUpper.GetSize();
	// ��� ���� ��ǥ�������� �⺻ ������ �׸�.
	if(nSize == 0)
	{
		xyLeft = xyRight = CDPoint(0, m_xyOrg.y);
		xyRight.x += 10000;
	}
	else
	{
		xyLeft	= m_tvArrLeftUpper.GetAt(0).m_v1;
		xyRight	= m_tvArrLeftUpper.GetAt(nSize-1).m_v2;
	}

	double dLeft	= m_bRevDirPlacingColumn ? xyLeft.x : xyRight.x;
	double sw		= m_bRevDirPlacingColumn ? 1 : -1;
	CDPoint xyCen	= CDPoint(0, 0);
	CTwinVectorArray tvArrLeft;
	for(long col = 0; col < m_nCountColumn; col++)
	{
		dLeft	+= m_dDistColumn[col] * sw;
		xyCen	= GetXyMatchUpperSlab(dLeft);

		m_Column.m_xyCenInLeft	= xyCen;

		if(col == nIdx) break;
	}

	return &m_Column;
}

// ��� ö�� �ǰ��
// ��ս��� ��� �ٸ� ��ö���̳� ���ö�� ����ö���� ����, ��պ�ö�ٸ� ����
void CWallApp::SetDataDefaultRebarJewonColumn(long nBaseNum, long nJijum)
{
	ClearRebarList(3);
	if(!m_bIs) return;
	if(!m_bTopIsColumn) return;
	if(m_bNotRebar) return;
	
	if(nBaseNum != -1)
		GetMarkRebarColumn(0, FALSE, TRUE, nBaseNum);

	long nSizeLock = m_pArrRebarColumn.GetSize();
	BOOL bFirst	= TRUE;
	for(long col = 0; col < m_nCountColumn; col++)
	{
		CColumnApp *pColumn	= GetColumn(col);
		if(!pColumn) continue;

		pColumn->SetDataDefaultRebarJewonStd(nBaseNum);
		if(bFirst) bFirst	= FALSE;

		// ö���� �����ͼ� ��ħ.
		long i = 0; for(i = 0; i < pColumn->m_pArrRebar.GetSize(); i++)
		{
			CRebar *pRB	= new CRebar;
			*pRB	= *pColumn->m_pArrRebar.GetAt(i);
			pRB->m_sMark	= GetMarkRebarColumn(0, FALSE, TRUE, -1); 
			pRB->m_sDescription.Format("����%d %s", nJijum + 1, pRB->m_sDescription); 

			BOOL bExistLock = FALSE;
			for(long ix = 0; ix < nSizeLock; ix++)
			{
				CRebar *pRB_Lock = m_pArrRebarColumn.GetAt(ix);
				if(pRB_Lock->m_sDescription == pRB->m_sDescription)
				{
					bExistLock = TRUE;
					break;
				}
			}
			if(bExistLock == FALSE)
				m_pArrRebarColumn.Add(pRB);
			else
				delete pRB;
		}
	}

	// ����� �������� ������� ������� ����
	// ���� ������ ö���� ������ ��Ŵ.
	long i = 0; for(i = 0; i < m_pArrRebarColumn.GetSize()-1; i++)
	{
		long j = 0; for(j = i+1; j < m_pArrRebarColumn.GetSize(); j++)
		{
			CRebar *pRBi	= m_pArrRebarColumn.GetAt(i);
			CRebar *pRBj	= m_pArrRebarColumn.GetAt(j);

			if(pRBi->m_sDescription == pRBj->m_sDescription)
			{
				pRBi->m_dLengthMax_A	= (pRBi->m_dLengthMax_A*pRBi->m_nEa + pRBj->m_dLengthMax_A*pRBj->m_nEa) / (pRBi->m_nEa+pRBj->m_nEa);
				pRBi->m_dLengthMin_A	= (pRBi->m_dLengthMin_A*pRBi->m_nEa + pRBj->m_dLengthMin_A*pRBj->m_nEa) / (pRBi->m_nEa+pRBj->m_nEa);
				pRBi->m_dLengthMax_B	= (pRBi->m_dLengthMax_B*pRBi->m_nEa + pRBj->m_dLengthMax_B*pRBj->m_nEa) / (pRBi->m_nEa+pRBj->m_nEa);
				pRBi->m_dLengthMin_B	= (pRBi->m_dLengthMin_B*pRBi->m_nEa + pRBj->m_dLengthMin_B*pRBj->m_nEa) / (pRBi->m_nEa+pRBj->m_nEa);
				pRBi->m_dLengthMax_C	= (pRBi->m_dLengthMax_C*pRBi->m_nEa + pRBj->m_dLengthMax_C*pRBj->m_nEa) / (pRBi->m_nEa+pRBj->m_nEa);
				pRBi->m_dLengthMin_C	= (pRBi->m_dLengthMin_C*pRBi->m_nEa + pRBj->m_dLengthMin_C*pRBj->m_nEa) / (pRBi->m_nEa+pRBj->m_nEa);
				pRBi->m_dLengthMax_D	= (pRBi->m_dLengthMax_D*pRBi->m_nEa + pRBj->m_dLengthMax_D*pRBj->m_nEa) / (pRBi->m_nEa+pRBj->m_nEa);
				pRBi->m_dLengthMin_D	= (pRBi->m_dLengthMin_D*pRBi->m_nEa + pRBj->m_dLengthMin_D*pRBj->m_nEa) / (pRBi->m_nEa+pRBj->m_nEa);
				pRBi->m_nEa += pRBj->m_nEa;
				pRBi->m_dEa += pRBj->m_dEa;

				delete pRBj;
				m_pArrRebarColumn.RemoveAt(j);
				j--;
			}
		}
	}
}

// ��ü���� �׸��� �����ϸ�� ��ü�� �������� �������� �ؼ� �׸��µ�
// ����� ��ü�� ��� ��ü ������ �� ���� �ְ�, ������ �� ���� �ִ�.
// �׷��� �Լ��� �̿���
// ����� ��ü�϶��� ��ü�� �������� �������� ������ �ȹٷ� ������ �׷��� �Ǳ� ������.
CDPoint CWallApp::GetXyOrgFootingLowerForSide(BOOL bLeft)
{
	CDPoint xyOrg		= GetXyOrgTopForFront(bLeft);
	CRebarPlacing rb;
	CTwinVector tvLower	= m_footing.m_tvArrFront.GetTvByInfo("�����ϸ�");

	if(m_nType == WALL_TYPE_SLOPE)
		tvLower	= rb.RotateTv(tvLower, xyOrg, GetXyDirRotate_Front().GetAngleDegree());
	
	if(m_footing.m_bIs || m_footing.IsVirtualFooting())
	{
		GetXyMatchLineAndLine(xyOrg, CDPoint(0, -1), tvLower.m_v1, tvLower.GetXyDir(), xyOrg);
	}
	else if(m_tvArrLowerLimit_FrontRebar.GetSize() > 0)
	{
		CTwinVector tvWall	= GetTvOffset_Front(0, 0, FALSE, FALSE, bLeft, TRUE);

		CDPointArray xyArrMatch;
		CRebarPlacing rb;
		rb.GetXyMatchTvArrAndLine(m_tvArrLowerLimit_FrontRebar, xyOrg, tvWall.GetXyDir(), xyArrMatch);
		if(xyArrMatch.GetSize() > 0)
		{
			xyOrg.y	= xyArrMatch.GetAt(0).y;
		}
		else
		{
			// ������ ���� ��ã�Ҵٸ� �����ؼ� ���纸��.
			CTwinVector tvN;
			if(xyOrg.x < m_tvArrLowerLimit_FrontRebar.GetXyLeft().x)
			{
				tvN = m_tvArrLowerLimit_FrontRebar.GetAt(0);
			}
			else
			{
				m_tvArrLowerLimit_FrontRebar.GetLast(tvN);
			}
			CDPoint xyMatch;
			GetXyMatchLineAndLine(tvN.m_v1, tvN.GetXyDir(), xyOrg, tvWall.GetXyDir(), xyMatch);
			xyOrg.y = xyMatch.y;
		}
	}

	return xyOrg;
}

// ��ü �������� �������� �Լ�
// ö�� �����̳� ������ ���̸� ���� �����͵� ������, �����ϳ��� �迭�� ���ϴ� ��ŭ �� ���� ���� ��쳪
// �̻��� ���� �� ��츦 ����ؼ� �� �Լ��� ���ؼ� ���� �������̸� ���� ������ �Ѵ�.
// bAccumulation : index������ �������̸� �����ؼ� ������.
double CWallApp::GetDistMainRebar(long nCycle, long nDan, BOOL bLeft, long nIdx, BOOL bAccumulation)
{
	if(nCycle == -1) nCycle = GetCountCycleMainRebar()-1;
	if(nCycle < 0 || nCycle > GetCountCycleMainRebar()-1) return 0;

	double dDist	= 0;
	long nSize		= 0;
	long left		= bLeft ? iLEFT : iRIGHT;
	if(!m_bDetailInput23DanMainRebar)
		nDan	= 0;

	if(m_bTopIsColumn) return 0;

	nSize	= m_dArrDistRC[nCycle][left][nDan].GetSize();
	if(nIdx == -1) nIdx = nSize-1;
	if(nIdx < 0) return dDist;

	if(bAccumulation)
	{
		long i = 0; for(i = 0; i <= nIdx; i++)
		{
			if(i >= nSize) break;
			dDist	+= m_dArrDistRC[nCycle][left][nDan].GetAt(i);
		}
	}
	else
	{
		if(nIdx < nSize)
			dDist	= m_dArrDistRC[nCycle][left][nDan].GetAt(nIdx);
		else
			dDist	= 0;
	}

	return dDist;
}

void CWallApp::MakeJointMainRebar(CJointArray &arrJoint, long nCycle, BOOL bLeft, long nDan)
{
	arrJoint.RemoveAll();
	if(nCycle == -1) nCycle = GetCountCycleMainRebar()-1;
	if(nCycle < 0 || nCycle > GetCountCycleMainRebar()-1) return;

	// �ʿ��� RebarInfoCycle�� ã��
	long left	= bLeft ? iLEFT : iRIGHT;
	CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC = &m_pArrRCMain[nCycle][left][nDan];
	if(!pArrRC) return;

	// �����м�
	GetJointAtRebarInfoCycleArray(arrJoint, pArrRC);
}


// RcBridgeRebar���� �����ؿ� �ڵ��� (��������)
// rebarinfocycle �迭���� ���� ������ �����Ѵ�.
void CWallApp::GetJointAtRebarInfoCycleArray(CJointArray &arrJoint, CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC)
{
	arrJoint.RemoveAll();
	if(!pArrRC) return;

	long nSize	= pArrRC->GetSize();
	for(long rc = 0; rc < nSize-1; rc++)
	{
		CRebarInfoCycle *pRC1	= pArrRC->GetAt(rc);
		CRebarInfoCycle *pRC2	= pArrRC->GetAt(rc+1);
		CRebarJoint joint;
		
		if(GetJointAtRebarInfoCycle(pRC1, pRC2, joint))
		{
			// ������ �����ʿ� �׷����� �Ǵ� ��츦 �Ǵ��ؼ� �������ش�.
			if(pRC1->m_nPosEnd == REBARINFO_POS_INWALL_LEFT)
				joint.m_bDrawUpper	= FALSE;
			else 
				joint.m_bDrawUpper	= TRUE;
			
			arrJoint.Add(joint);
		}
	}
}

// RcBridgeRebar���� �����ؿ� �ڵ��� (��������)
// �� ö���� ���ؼ� ������ �մ��� �Ǵ�.
// �Ϲ������� �� rebarinfocycle�� ���ؼ� ������ ã���� ������, ��ν����� ö���� ��� ������ 
// ���󰡱� ������ �Ϲ����� ������� ã�Ⱑ ��ƴ�.
// �׷��� ��ν����� ö�ٸ� �ٸ� ������� ã�´�.
BOOL CWallApp::GetJointAtRebarInfoCycle(CRebarInfoCycle *pRC1, CRebarInfoCycle *pRC2, CRebarJoint &joint)
{
	if(!pRC1 || !pRC2) return FALSE;
	if(!pRC1->m_bExist || !pRC2->m_bExist)
	{
		joint.m_dLength	= 0;
		return FALSE;
	}

	BOOL bJoint		= FALSE;
	CDPointArray xyArrLine1, xyArrLine2;	
	CRebarPlacing rb;
	
	// rebarinfocycle Ư���� ���� ���ؾ� �� ���� �̾Ƽ� ���Ѵ�.
	// 1. �� ö���� ��� ���۰� ���� pos�� ���� ö���϶�.
	if(pRC1->m_nPosStt == pRC1->m_nPosEnd && pRC2->m_nPosStt == pRC2->m_nPosEnd)
	{
		xyArrLine1.Add(pRC1->m_xyStt);
		xyArrLine1.Add(pRC1->m_xyMid1);

		xyArrLine2.Add(pRC2->m_xyStt);
		xyArrLine2.Add(pRC2->m_xyMid1);

		// ���� �ִ��� �Ǵ�.
		bJoint = rb.CheckOverlapLine(xyArrLine1, xyArrLine2, TRUE, TRUE);
	}
	// 2. ù��° ö���� ���۰� ���� ���� �ι�° ö���� �ٸ���
	else if(pRC1->m_nPosStt == pRC1->m_nPosEnd && pRC2->m_nPosStt != pRC2->m_nPosEnd)
	{
		xyArrLine1.Add(pRC1->m_xyStt);
		xyArrLine1.Add(pRC1->m_xyMid1);

		xyArrLine2.Add(pRC2->m_xyStt);
		xyArrLine2.Add(pRC2->m_xyMid1);

		// ���� �ִ��� �Ǵ�.
		bJoint	= rb.CheckOverlapLine(xyArrLine1, xyArrLine2, TRUE, TRUE);

		// ��ġ�� �ʾҴٸ� �ѹ� �� �˻���.
		if(!bJoint)
		{
			xyArrLine1.Add(pRC1->m_xyStt);
			xyArrLine1.Add(pRC1->m_xyMid1);

			xyArrLine2.Add(pRC2->m_xyMid2);
			xyArrLine2.Add(pRC2->m_xyEnd);
			
			// ���� �ִ��� �Ǵ�.
			bJoint	= rb.CheckOverlapLine(xyArrLine1, xyArrLine2, TRUE, TRUE);
		}
	}
	// 3. ù��° ö���� ���۰� ���� �ٸ��� �ι�° ö���� ������.
	else if(pRC1->m_nPosStt != pRC1->m_nPosEnd && pRC2->m_nPosStt == pRC2->m_nPosEnd)
	{
		xyArrLine1.Add(pRC1->m_xyMid2);
		xyArrLine1.Add(pRC1->m_xyEnd);

		xyArrLine2.Add(pRC2->m_xyStt);
		xyArrLine2.Add(pRC2->m_xyMid1);

		// ���� �ִ��� �Ǵ�.
		bJoint	= rb.CheckOverlapLine(xyArrLine1, xyArrLine2, TRUE, TRUE);

		// ��ġ�� �ʾҴٸ� �ѹ� �� �˻���.
		if(!bJoint)
		{
			xyArrLine1.RemoveAll();
			xyArrLine1.Add(pRC1->m_xyMid2);
			xyArrLine1.Add(pRC1->m_xyEnd);

			xyArrLine2.RemoveAll();
			xyArrLine2.Add(pRC2->m_xyStt);
			xyArrLine2.Add(pRC2->m_xyMid1);
			
			// ���� �ִ��� �Ǵ�.
			bJoint	= rb.CheckOverlapLine(xyArrLine1, xyArrLine2, TRUE, TRUE);
		}
	}
	// 4. ��ö�ٸ�� ���۰� ���� �ٸ���.
	else
	{
		xyArrLine1.Add(pRC1->m_xyStt);
		xyArrLine1.Add(pRC1->m_xyMid1);

		xyArrLine2.Add(pRC2->m_xyStt);
		xyArrLine2.Add(pRC2->m_xyMid1);

		// ���� �ִ��� �Ǵ�.
		bJoint	= rb.CheckOverlapLine(xyArrLine1, xyArrLine2, TRUE, TRUE);

		// ��ġ�� �ʾҴٸ� �ѹ� �� �˻���.
		if(!bJoint)
		{
			xyArrLine1.RemoveAll();
			xyArrLine1.Add(pRC1->m_xyStt);
			xyArrLine1.Add(pRC1->m_xyMid1);

			xyArrLine2.RemoveAll();
			xyArrLine2.Add(pRC2->m_xyMid2);
			xyArrLine2.Add(pRC2->m_xyEnd);
			
			// ���� �ִ��� �Ǵ�.
			bJoint	= rb.CheckOverlapLine(xyArrLine1, xyArrLine2, TRUE, TRUE);
		}

		// ��ġ�� �ʾҴٸ� �ѹ� �� �˻���.
		if(!bJoint)
		{
			xyArrLine1.RemoveAll();
			xyArrLine1.Add(pRC1->m_xyMid2);
			xyArrLine1.Add(pRC1->m_xyEnd);

			xyArrLine2.RemoveAll();
			xyArrLine2.Add(pRC2->m_xyStt);
			xyArrLine2.Add(pRC2->m_xyMid1);
			
			// ���� �ִ��� �Ǵ�.
			bJoint	= rb.CheckOverlapLine(xyArrLine1, xyArrLine2, TRUE, TRUE);
		}

		// ��ġ�� �ʾҴٸ� �ѹ� �� �˻���.
		if(!bJoint)
		{
			xyArrLine1.RemoveAll();
			xyArrLine1.Add(pRC1->m_xyMid2);
			xyArrLine1.Add(pRC1->m_xyEnd);

			xyArrLine2.RemoveAll();
			xyArrLine2.Add(pRC2->m_xyMid2);
			xyArrLine2.Add(pRC2->m_xyEnd);
			
			// ���� �ִ��� �Ǵ�.
			bJoint	= rb.CheckOverlapLine(xyArrLine1, xyArrLine2, TRUE, TRUE);
		}
	}


	// �˻簡 �������� ������ �ִ� ��� ������ ������ �����
	if(bJoint)
	{
		// �������ִ� ���� xyArrLine1�� ���� �ȴ�.
		if(xyArrLine1.GetSize() > 1)
		{
			CDPoint xy1	= xyArrLine1.GetAt(0);
			CDPoint xy2	= xyArrLine1.GetAt(1);

			joint.m_xyPos		= (xy2+xy1)/2;
			joint.m_dLength		= ~(xy2-xy1);
			joint.m_xyDir		= (xy2-xy1).Unit();
		}
	}

	return bJoint;
}

void CWallApp::SetStringDescriptionRebarInfoCycle(CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC, BOOL bLeft, long nDan, long nCycle)
{
	if(!pArrRC) return;

	CString sLeft	= bLeft ? "����" : "����";
	CString sCycle	= GetCircleText(nCycle+1);
	long nIdx		= 1;

	long nSize	= pArrRC->GetSize();

	long i = 0; for(i = 0; i < nSize; i++)
	{
		CRebarInfoCycle *pRC	= pArrRC->GetAt(i);
		if(!pRC->m_bExist) continue;

		pRC->m_sDescription.Format("�߰����� %s�� %d�� %s %d", sLeft, nDan+1, sCycle, nIdx++);
	}
}

// cycle3 ��ٹ�Ŀ� ���� ���� �̵� �Ÿ� ����
// ��� -> 
// 0 : �����Է�
// 1 : 1cycle�� ���� ��ġ
// 2 : �������� �������� 1/2 �̵�
// 3 : �������� �������� 1/2 �̵�
// 4 : �������� �������� ��ŭ �̵�
// 5 : �������� �������� ��ŭ �̵�
double CWallApp::GetDistMoveJointCycle13(BOOL bCycle3)
{
	if(GetCountCycleMainRebar() < 4) return 0;

	double dLenJoint	= 0;
	double dDist	= GetDistMoveJointByTypeDefaultCycle3(m_nTypeDefaultCycle3, dLenJoint);

	if(!bCycle3)
		dDist	= 0;

	return dDist;
}

// Ÿ�Կ� ���� �̵� �Ǿ�� �ϴ� �Ÿ� ����
// 0 : �����Է�
// 1 : 1cycle�� ���� ��ġ
// 2 : �������� �������� 1/2 �̵�
// 3 : �������� �������� 1/2 �̵�
// 4 : �������� �������� ��ŭ �̵�
// 5 : �������� �������� ��ŭ �̵�
double CWallApp::GetDistMoveJointByTypeDefaultCycle3(long nTypeDefaultCycle3, double dLenJoint)
{
	if(GetCountCycleMainRebar() < 4) return 0;

	if(nTypeDefaultCycle3 == 0) return 0;
	if(nTypeDefaultCycle3 == 1) return 0;
	if(nTypeDefaultCycle3 == 2) return dLenJoint/4;
	if(nTypeDefaultCycle3 == 3) return -dLenJoint/4;
	if(nTypeDefaultCycle3 == 4) return dLenJoint/2;
	if(nTypeDefaultCycle3 == 5) return -dLenJoint/2;

	return 0;
}

double CWallApp::GetValueJoint(CRebarInfoCycle *pRCCur, double dDiaNext)
{
	if(!m_pBasicConcInfo) return 0;
	if(!pRCCur) return 0;
	if(Compare(pRCCur->m_Dia, 0.0, "<=") && Compare(dDiaNext, 0.0, "<=")) return 0; 

	double dCover	= 0;
	double dSpace	= m_dCTC_MainRebar/2;
	BOOL bLeft = TRUE;

	if(pRCCur->m_nPosEnd == REBARINFO_POS_INWALL_LEFT )
	{
		dCover	= m_dCoverLeft[0];
		bLeft = TRUE;
	}
	else if(pRCCur->m_nPosEnd == REBARINFO_POS_INWALL_RIGHT)
	{
		dCover	= m_dCoverRight[0];
		bLeft = FALSE;
	}

	long nType			= JOINT_TENS_NORMAL_B;
	double dLenJoint	= 0;
	if(m_pBasicConcInfo->IsDesignLsd())
	{
		SRebarEnvOption SLsdOption;
		SetRebarLsdOption(&SLsdOption, bLeft, TRUE, 0, 0);
		dLenJoint	= m_pBasicConcInfo->GetValueJoint(max(pRCCur->m_Dia, dDiaNext), &SLsdOption);
	}
	else
	{
		dLenJoint	= m_pBasicConcInfo->GetValueJoint(m_Fck, m_Fy, max(pRCCur->m_Dia, dDiaNext), nType, dCover, dSpace);
	}
		
	return dLenJoint;
}

// ���鵵 ��½� ������ ����(ū��)
// double &dAngStt, double &dAngEnd : �׸��� ��ũ�� �׷��� �ǹǷ� ���������� ������.
void CWallApp::GetInfoCuttingCircle_Side(BOOL bLeft, CDPoint &xyCen, double &dR, double &dAngStt, double &dAngEnd)
{
	CTwinVectorArray tvArrSide;
	GetTvLeftSide(tvArrSide, FALSE, FALSE, FALSE, TRUE, bLeft ? -1 : 1, TRUE);

	CDRect rect	= tvArrSide.GetRect();
	double dDivide_L	= 2.0; //1.2
	double dDiviede_R	= 2.0; // ..
	
	dR	= rect.Width() * 10;
	xyCen.x	= bLeft ? rect.left - dR + rect.Width()/dDivide_L : rect.right + dR - rect.Width()/dDiviede_R;
	xyCen.y	= rect.top - fabs(rect.Height())/2;

	double dLenExt	= 1000;
	double dHU	= fabs(rect.Height())/2 + dLenExt;
	double dHL	= fabs(rect.Height())/2 + dLenExt + m_footing.GetHeight();

	double dL	= sqrt(dR*dR - dHU*dHU);
	double dAng1	= 90 - ToDegree(atan(dL/dHU));
	double dAng2	= 90 - ToDegree(atan(dL/dHL));

	if(bLeft)
	{
		dAngStt	= -dAng2;
		dAngEnd	= dAng1;
	}
	else
	{
		dAngStt	= 180 - dAng1;
		dAngEnd	= 180 + dAng2;
	}

}

// ���� ��ü�� ���� ����� ���� ��ü�Ŀ� �´� �������� ������ �д�.
// �ʿ��Ҷ� �������
void CWallApp::SetColumnForRoundRectWall()
{
	m_Column.m_nType	= COL_TYPE_TRACK;
	m_Column.m_xyCen	= m_tvPathPlan.GetXyMid();
	m_Column.m_dH[iOUTTER][iUPPER]		= m_tvPathPlan.GetLength();
	m_Column.m_dR[iOUTTER][iUPPER]		= m_dW/2;
	m_Column.m_dB[iOUTTER][iUPPER]		= m_dW;
	CDRect rect	= m_tvArrLeftUpper.GetRect();
	m_Column.m_xyCenInLeft	= CDPoint(rect.left+rect.Width()/2, rect.top);

	for(long nDan=0; nDan<MAX_REBAR_DAN; ++nDan)
	{
		if(GetCountRebarInfoCycle(0, nDan, TRUE) == FALSE)
			m_Column.m_dDiaMain[iOUTTER][nDan] = 0;
		else
			m_Column.m_dDiaMain[iOUTTER][nDan] = GetDiaMainRebar(0, nDan, TRUE, 0);
		
	}

	m_Column.GetTvPlan();
}

// ��ü ��ö�� ���������� ���� �������� ����
// ������� ��� ���������� ���ؼ��� ����.
void CWallApp::GetOffsetInfoRebarMain_Plane_LineSection(CDPointArray &xyArrOffsetInfo, BOOL bLeft, long nDan)
{
	xyArrOffsetInfo.RemoveAll();

	// ��ö���� ���� ����(��������,���庮ü�ǰ�����δ³��߿����� ����)
	CTwinVectorArray tvArrSide;
	GetTvOffset_Side(tvArrSide, bLeft, 0);
	CTwinVector tvLeft	= tvArrSide.GetTvByInfo("��ü����"),
				tvRight	= tvArrSide.GetTvByInfo("��ü����");
	if(IsRoundRectWall())
	{
		double dCover	= GetCoverFront(TRUE, 0);
		tvLeft.m_v1.x += (m_dW/2 - dCover);
		tvLeft.m_v2.x += (m_dW/2 - dCover);
		tvRight.m_v1.x -= (m_dW/2 - dCover);
		tvRight.m_v2.x -= (m_dW/2 - dCover);
	}

	// �纸����ġ �϶� ���� �κб��� ��ö���� ��ġ�ȴ�. �׷��ٶ� ������ ��ġ �� ġ������ ����Ѵ�
	if(m_dWidthOverRebarExp[iLEFT] > 0)
	{
		double dCover	= GetCoverFront(TRUE, 0);
		tvLeft.m_v1.x -= m_dWidthOverRebarExp[iLEFT] - dCover;
		tvLeft.m_v2.x -= m_dWidthOverRebarExp[iLEFT] - dCover;
	}
	if(m_dWidthOverRebarExp[iRIGHT] > 0)
	{
		double dCover	= GetCoverFront(TRUE, 0);
		tvRight.m_v1.x += m_dWidthOverRebarExp[iRIGHT] - dCover;
		tvRight.m_v2.x += m_dWidthOverRebarExp[iRIGHT] - dCover;
	}

	CTwinVector tvHor;
	tvHor.m_v1	= tvLeft.GetXyBottom();
	tvHor.m_v2	= tvRight.GetXyBottom();
	
	
	// ���� ��ö�� ���� ���� ///////////////////////////////////////////////////////////////////
	double dCtc			= m_bApplyAngleToMainRebarCTC ? m_dCTC_MainRebar/sin(ToRadian(m_dAnglePath)) : m_dCTC_MainRebar;
	double dLen			= tvHor.GetHorLength();
	long nCount			= (long)((dLen-(dCtc*2)) / dCtc);
	double dRemain		= (dLen-(dCtc*2)) - (dCtc * nCount);

	xyArrOffsetInfo.Add(CDPoint(1, dRemain/2 + dCtc));
	xyArrOffsetInfo.Add(CDPoint(nCount, dCtc));
	xyArrOffsetInfo.Add(CDPoint(1, dRemain/2 + dCtc));
}

// ���� ��ü ���� ���� ����?
BOOL CWallApp::IsEnableRoundRectWall()
{
	if(m_nType == WALL_TYPE_VER && !m_bTopIsColumn) return TRUE;


	return FALSE;
}

BOOL CWallApp::IsRoundRectWall()
{
	if(IsEnableRoundRectWall() && m_bRoundRect) return TRUE;

	return FALSE;
}

// ù��°�� ������ ����� ������ ���ʳ����� �Ÿ� ����
double CWallApp::GetDistFromFootingToColumn(BOOL bSttColumn)
{
	if(!m_bTopIsColumn) return 0;

	CDPoint xyFootingEnd	= bSttColumn ? m_footing.m_tvPathPlan.m_v1 : m_footing.m_tvPathPlan.m_v2;
	CDPoint xyColumnEnd		= bSttColumn ? m_tvPathPlan.m_v1 : m_tvPathPlan.m_v2;
	CDPoint xyDirCol		= m_tvPathPlan.GetXyDir();
	double dH				= m_Column.GetWidth(TRUE);
	if(!bSttColumn)
	{
		xyColumnEnd	= xyColumnEnd + xyDirCol * (m_dDistColumn[0] - dH/2);
	}
	else
	{
		double dDistTot	= 0;
		long i = 0; for(i = 0; i < m_nCountColumn; i++)
			dDistTot += m_dDistColumn[i];

		xyColumnEnd	= xyColumnEnd + xyDirCol * (m_tvPathPlan.GetLength() - (dDistTot + dH/2));
	}

	double dDist	= ~(xyFootingEnd - xyColumnEnd);
	dDist += (bSttColumn ? m_footing.m_dMarginRight : m_footing.m_dMarginLeft);

	return dDist;
}

BOOL CWallApp::SetDiaMainRebar(long nCycle, long nDan, BOOL bLeft, long nIdx, double dDia, BOOL bMax)
{
// 	long nSize	= 0;
	long left	= bLeft ? iLEFT : iRIGHT;
	
	if(nDan == 0)
	{
		long nSize	= m_dArrDiaRC[nCycle][left][0].GetSize();
		if(nIdx == -1) nIdx = nSize-1;

		if(nIdx < nSize)
		{
			if(bMax == FALSE || m_dArrDiaRC[nCycle][left][0].GetAt(nIdx) < dDia)
				m_dArrDiaRC[nCycle][left][0].SetAt(nIdx, dDia);
			else
				return FALSE;
		}
		else 
			return FALSE;
	}
	// 2�ܺ��ʹ� �ٸ� ������ ����Ǿ� �ִ�
	// 2���̻��� �׳� ���� �������� ������ ���еǾ� ����.
	// 2���� index 0�̰� 3���� 1�̴�.
	else if(nDan > 0)
	{
		if(nDan == -1) nDan = MAX_REBAR_DAN-1;
		if(nDan < 0) return 0;
		if(nDan > MAX_REBAR_DAN-1) return 0;
		if(nCycle == -1) nCycle = MAX_REBAR_CYCLE-1;
		if(nCycle < 0) return 0;
		if(nCycle > MAX_REBAR_CYCLE-1) return 0;

		CSetRebarDan *pRD	= &m_setRebarDan[left];
		if(dDia > 0 && GetCountCycleMainRebar() > nCycle)
		{
			if(bMax == FALSE || pRD->m_dDia[nCycle][nDan] < dDia)
			{
				pRD->m_dDia[nCycle][nDan]	= dDia;
				if(pRD->m_nCountDan < nDan+1)
					pRD->m_nCountDan	= nDan+1;
			}
		}
		else 
			return FALSE;
	}

	return TRUE;
}

// ��ü �Ϻ� EL
double CWallApp::GetELBottomOfWall()
{
	if(m_footing.IsVirtualFooting()) return m_dELB - m_dELBDiff;

	// �Է¹��� EL�� ���� ���̸� �����ؼ� �׷��ش�.
	double dELB = 0;
	if(!m_footing.m_bIs && !m_footing.IsVirtualFooting())
		dELB = m_dELB - m_dELBDiff;
	else
		dELB = m_dELB + m_footing.GetHeight() - m_dELBDiff;

	if(m_footing.m_nType == FOOTING_TYPE_LEFTSLOPE || m_footing.m_nType == FOOTING_TYPE_RIGHTSLOPE)
	{
		BOOL bLeft	= m_footing.m_nType == FOOTING_TYPE_LEFTSLOPE;
		CDPoint vAng	= m_footing.GetXyDirSlope(bLeft, FALSE);
		CDPoint xy1	= CDPoint(0, 0);
		double dLenToCen	= bLeft ? m_footing.m_dFWR + m_footing.m_dMR : m_footing.m_dFWL + m_footing.m_dML;
		CDPoint xy2	= xy1 + (vAng * -dLenToCen);
		double dHFU	= xy1.y - xy2.y;

		dELB	= m_dELB + m_footing.GetHeight() - dHFU - m_dELBDiff;
	}

	return dELB;

}

double CWallApp::GetDiffHeightByCenterOfRoad(CTwinVectorArray &tvArr, double dX, double dOrg)
{
	CDPoint xyMatchOrg		= GetXyMatchXAndTvArr(dOrg, tvArr);
	CDPoint xyMatchTarget	= GetXyMatchXAndTvArr(dX, tvArr);

	return xyMatchTarget.y - xyMatchOrg.y;
}

CDPoint CWallApp::GetXyMatchXAndTvArr(double dX, CTwinVectorArray &tvArr)
{
	CDPoint xyMatch(0, 0);
	CDPointArray xyArrMatch;
	CRebarPlacing rb;

	rb.GetXyMatchTvArrAndLine(tvArr, CDPoint(dX, 0), CDPoint(0, 1), xyArrMatch);
	if(xyArrMatch.GetSize() == 0)
	{
		CTwinVector tvLeft, tvRight;
		tvLeft	= tvArr.GetAt(0);
		tvRight	= tvArr.GetAt(tvArr.GetSize()-1);
		if(tvLeft.GetXyMid().x > tvRight.GetXyMid().x)
			GetSwap(tvLeft, tvRight);

		CDPoint xyLeft	= tvLeft.GetXyLeft();
		CDPoint xyRight	= tvRight.GetXyRight();
		CTwinVector tv;
		if(fabs(xyLeft.x - dX) < fabs(xyRight.x - dX))
		{
			GetXyMatchLineAndLine(tvLeft.m_v1, tvLeft.GetXyDir(), CDPoint(dX, 0), CDPoint(0, 1), xyMatch);
		}
		else
		{
			GetXyMatchLineAndLine(tvRight.m_v1, tvRight.GetXyDir(), CDPoint(dX, 0), CDPoint(0, 1), xyMatch);
		}
	}
	else
		xyMatch	= xyArrMatch.GetAt(0);

	return xyMatch;
}

void CWallApp::RemoveNotExistRebarInfoCycle(CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC)
{
	if(!pArrRC) return;

	long i = 0; for(i = 0; i < pArrRC->GetSize(); i++)
	{
		CRebarInfoCycle *pRC	= pArrRC->GetAt(i);
		if(!pRC->m_bExist)
		{
			delete pRC;
			pArrRC->RemoveAt(i);
			i--;
		}
	}
}


// ��ü 2,3�� ö�� Ȯ����� ����
void CWallApp::ApplyExtLength23DanMainRebar(CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC, double dLengthExt23DanRebar, CTwinVector tvRebarUpperSlab)
{
	if(!pArrRC) return;

	long nSize	= pArrRC->GetSize();
	long i = 0; for(i = 0; i < nSize; i++)
	{
		CRebarInfoCycle *pRCPrev	= i > 0 ? pArrRC->GetAt(i-1) : NULL;
		CRebarInfoCycle *pRC		= pArrRC->GetAt(i);
		CRebarInfoCycle *pRCNext	= i+1 < nSize ? pArrRC->GetAt(i+1) : NULL;
		CRebarJoint joint;

		if(pRCPrev)
		{
			if(!pRCPrev->m_bExist)
				pRCPrev	= NULL;
		}
		if(pRCNext)
		{
			if(!pRCNext->m_bExist)
				pRCNext	= NULL;
		}
		if(!pRC->m_bExist) continue;


		// ���� ö�ٰ��� ���� �˻�
		if(!GetJointAtRebarInfoCycle(pRCPrev, pRC, joint))
		{
			if(pRC->m_nPosStt == pRC->m_nPosEnd)
			{
				CDPoint xyDir	= (pRC->m_xyMid1 - pRC->m_xyStt).Unit();
				pRC->m_xyStt	= pRC->m_xyStt + (xyDir * -dLengthExt23DanRebar);
			}
		}

		// ���� ö�ٰ��� ���� �˻�
		if(!GetJointAtRebarInfoCycle(pRC, pRCNext, joint))
		{
			if(pRC->m_nPosStt == pRC->m_nPosEnd)
			{
				CDPoint xyDir	= (pRC->m_xyMid1 - pRC->m_xyStt).Unit();
				pRC->m_xyMid1	= pRC->m_xyMid1 + (xyDir * dLengthExt23DanRebar);
				pRC->m_xyMid2	= pRC->m_xyMid1;
				pRC->m_xyEnd	= pRC->m_xyMid1;
			}
			else
			{
				CDPoint xyDir	= (pRC->m_xyEnd - pRC->m_xyMid2).Unit();
				pRC->m_xyEnd	= pRC->m_xyEnd + (xyDir * dLengthExt23DanRebar);
			}
		}

		// ö�� üũ�� ������ ö���� ��ν����� �Ǻ��� �հ� �������� �˻��ؾ� �ȴ�.
		if(i == nSize-1)
		{
			CDPoint xyMatch(0, 0);

			if(pRC->m_nPosStt == pRC->m_nPosEnd)
			{
				if(GetXyMatchSegAndLine(pRC->m_xyStt, pRC->m_xyMid1, tvRebarUpperSlab.m_v1, tvRebarUpperSlab.GetXyDir(), xyMatch))
				{
					pRC->m_xyMid1	= xyMatch;
					pRC->m_xyMid2	= xyMatch;
					pRC->m_xyEnd	= xyMatch;
				}
			}
			else
			{
				if(GetXyMatchSegAndLine(pRC->m_xyEnd, pRC->m_xyMid2, tvRebarUpperSlab.m_v1, tvRebarUpperSlab.GetXyDir(), xyMatch))
				{
					pRC->m_xyEnd	= xyMatch;
				}
			}
		}
	}
}

double CWallApp::GetLengthExt23DanRebar(BOOL bLeft)
{
	long left	= bLeft ? iLEFT : iRIGHT;

	return m_setRebarDan[left].m_dLengthExt23DanRebar;
}

// CTwinVector CWallApp::GetTvCenterOfRoad_Side(BOOL bLeft, double dExtLen)
// {
// 	CTwinVectorArray tvArrSide;
// 	BOOL bIncludeFooting	= TRUE;
// 	BOOL bIncludeUpperSlab	= TRUE;
// 	GetTvLeftSide(tvArrSide, FALSE, bIncludeFooting, bIncludeUpperSlab, bLeft, 0, TRUE);
// 
// 
// 	CDRect rect	= tvArrSide.GetRect();
// 	CTwinVector tvLower	= tvArrSide.GetTvByInfo("��ü�ϸ�");
// 	CTwinVector tvCenter;
// 	
// 	tvCenter.m_v1	= CVector(m_dCenterOfWall, rect.bottom, 0);
// 	tvCenter.m_v2	= CVector(m_dCenterOfWall, rect.top, 0);
// 
// 	// Ȯ����� ����.
// 	tvCenter.m_v1	= tvCenter.m_v1 + (tvCenter.GetXyDir(TRUE) * dExtLen);
// 	tvCenter.m_v2	= tvCenter.m_v2 + (tvCenter.GetXyDir() * dExtLen);
// 
// 	return tvCenter;
// }

// ��±� ����Ҷ� ������ġ
// ����ö�ٵ� �ش��.
// �ϸ� ���̰� �ٸ� ��� ���� ���� �������� ��
CDPoint CWallApp::GetXySttRebarSupport_Front(long nDan, BOOL bLeft, BOOL bVertDir)
{
	CDPoint xyStt	= GetTvOffset_Front(0, 0, TRUE, TRUE, bLeft, bVertDir).m_v1;

	CRebarInfoCycle *pRC	= GetRebarInfoCycle(0, nDan, bLeft, 0);
	if(!pRC) return xyStt;
	CRebarInfoCycle *pRC1	= GetRebarInfoCycle(0, 0, bLeft, 0);
	//CRebarInfoCycle *pRC2	= GetRebarInfoCycle(0, 0, !bLeft, 0);
	//double dYMin	= pRC2 ? min(pRC1->m_xyMid2.y, pRC2->m_xyMid2.y) : pRC1->m_xyMid2.y;
	// ���� �¿����� ���� ���� �������� ������, ������ ��ü�� ���� ��ġ���� ���� ����� �ϴ� �ٶ���
	// ������ ��ü�� CWallApp�� ���� ����� ��� ������ �߻���
	// ���� �߰����� ��ü�� ���� ��ġ���� ����ϵ��� ��.
	if(m_bRebarLowerSettle[bLeft? iLEFT : iRIGHT]) // m_footing.m_bIs || m_footing.IsVirtualFooting())
	{
		// Box Ÿ���϶��� Round�� ������ ��ġ�� ���ö���� �����ϱ� ������ ö�� ���ϴ� ���� �������� ��ġ ������
		// �Ϲ� ���� ������ Round�κп� ö���� ���� �ʱ� ������ �� ��ġ�� ��� ������ġ�� �ȴ�.
		if(pRC->m_xyMid1.y <  pRC->m_xyMid2.y)
		{
			double dYMin	= (m_footing.m_bIs || m_footing.IsVirtualFooting())? pRC1->m_xyMid2.y : pRC1->m_xyMid1.y;

			CDPoint vAng	= GetTvOffset_Front(0, 0, TRUE, TRUE, bLeft, bVertDir).GetXyDir();
			GetXyMatchLineAndLine(CDPoint(pRC1->m_xyMid1.x, dYMin), vAng.Rotate90(), pRC->m_xyMid2, vAng, xyStt);
		}
		else
		{
			double dYMin	= (m_footing.m_bIs || m_footing.IsVirtualFooting())? pRC1->m_xyMid2.y : pRC1->m_xyMid2.y;

			CDPoint vAng	= GetTvOffset_Front(0, 0, TRUE, TRUE, bLeft, bVertDir).GetXyDir();
			GetXyMatchLineAndLine(CDPoint(pRC1->m_xyMid2.x, dYMin), vAng.Rotate90(), pRC->m_xyMid1, vAng, xyStt);
		}
	}
	else
	{
		CDPoint vAng(0, 1);
//		CDPoint xyLeft, xyRight;
//		GetXyMatchLineAndLine(pRC1->m_xyStt, vAng, m_tvLowerLimitOfSupportRebar.m_v1, m_tvLowerLimitOfSupportRebar.GetXyDir(), xyLeft);
//		GetXyMatchLineAndLine(pRC2->m_xyStt, vAng, m_tvLowerLimitOfSupportRebar.m_v1, m_tvLowerLimitOfSupportRebar.GetXyDir(), xyRight);

		xyStt = pRC->m_xyStt;
	}

	return xyStt;
}

// �ɼǿ� ���� ��ü�� ��� ������
// ��ν����꿡 �����Ǵ� ��ġ�� ������
// �������� �Ϲݵ� �׸��� �� ���� �������� �������� �����.
CDPoint CWallApp::GetXyOrgTopForFront(BOOL bLeft)
{
	// ������ �Ǵ� ��ġ ����
	CTwinVector tvWall	= m_tvArrFront.GetTvByInfo(bLeft ? "��ü������" : "��ü������");
	CDPoint xyOrg	= m_nTypeRebarFront == 0 ? (tvWall.m_v2) : m_xyOrg;

	CRebarPlacing rb;
	CDPointArray xyArrMatch;
	rb.GetXyMatchTvArrAndLine(m_tvArrUpperLimit_Front, xyOrg, tvWall.GetXyDir(), xyArrMatch);
	if(xyArrMatch.GetSize() > 0)
		xyOrg	= xyArrMatch.GetAt(0);

	return xyOrg;
}

// ��ü�� �������� ����� ���� ȸ���ؾ� �Ǵ� ����
CDPoint CWallApp::GetXyDirRotate_Front()
{
	if(m_nType == WALL_TYPE_VER) return CDPoint(1, 0);
	CTwinVector tvWall	= m_tvArrFront.GetTvByInfo("��ü������");

	return ToDPointFrDegree(90 - tvWall.GetXyDir().GetAngleDegree());
}

// ���� ��±� �ǰ��
// ��±� �ǰ�Ƚ� ��ü�� ����ؾ� �ǹǷ� ���⿡�� �ǰ�� ����
void CWallApp::SetDataDefaultRebarSupportFront_Footing(BOOL bUpper)
{
	long upper			= bUpper ? iUPPER : iLOWER;
	CPlaceRebarByBlock *pPlace	= &m_footing.m_placeByBlock_Support_Front[upper][0];
	pPlace->m_xyArr.RemoveAll();
	if(!m_footing.m_bIs) return;

	// ���� ����� ������ ���Ÿ��� �Ѵ�
	CTwinVectorArray tvArrFrontOld;
	tvArrFrontOld	= m_footing.m_tvArrFront;
	m_footing.GetTvFront(FALSE);

	CHgBaseConcStd baseConc;
	double dCTC			= m_footing.m_dCTC_SupportRebar;

	// (ARCBRIDGE-1659) ��ö�� �������� ��ġ�϶� ���� ��� ���ش�.
	
	HGBOOL bApplyAngle = (m_footing.IsVertDir() == FALSE);
	double dAngApply	= m_footing.m_dAngle;

	// ���� ��±� ���� ///////////////////////////////////////////////////////////////////
	CTwinVectorArray tvArrRebar;
	double dCtcUpper	= dCTC;		// ����� ǥ�ذ���
	double dCtcLower	= dCTC/2;	// �ϸ��� ǥ�ذ���/2
	

	// ������౸�� ���� �����ϰ�� ��� �ϸ� ��� dCTC�� �����ϰ� ������ �ϳ��� �Ѵ�.
	if(m_footing.m_bSameOffsetPlaceTAndCBlock)
	{
		dCtcUpper		= dCTC;
		dCtcLower		= dCTC;
	}

	// ����� �ǰ�Ƚ� ��ü�� ��� �Ѵ�.
	// ����� ���ʴ� �׳� ��.
	if(bUpper && (m_footing.m_nType == FOOTING_TYPE_BASIC || m_footing.m_nType == FOOTING_TYPE_BASIC_BOTH ||
		m_footing.m_nType == FOOTING_TYPE_BASIC_LEFTSLOPE || m_footing.m_nType == FOOTING_TYPE_BASIC_RIGHTSLOPE))
	{
		// ��� ���Ҷ��� �ϸ��� ����¥���� ����¥������ �˾ƾ� �ȴ�.
		CPlaceRebarByBlock *pPlaceLower	= &m_footing.m_placeByBlock_Support_Front[iLOWER][0];
		double dRemainLower	= pPlaceLower->m_xyArr.GetSize() > 0 ? pPlaceLower->m_xyArr.GetAt(0).y : 0;

		double dWLeft	= m_footing.m_dFWL + m_dCoverLeft[0] - m_footing.GetCoverSide(TRUE, 0);
		double dWRight	= m_footing.m_dFWR + m_dCoverRight[0] - m_footing.GetCoverSide(FALSE, 0);
		if(m_footing.m_nType == FOOTING_TYPE_BASIC_LEFTSLOPE)
		{
			dWLeft	= m_footing.m_dFWL + m_footing.m_dFSLW + m_footing.m_dFELW;
			dWRight	= m_footing.m_dFWR;
		}
		else if(m_footing.m_nType == FOOTING_TYPE_BASIC_RIGHTSLOPE)
		{
			dWLeft	= m_footing.m_dFWL;
			dWRight	= m_footing.m_dFWR + m_footing.m_dFSRW + m_footing.m_dFERW;
		}

		double dWWall	= m_dW - m_dCoverLeft[0] - m_dCoverRight[0];

		if(bApplyAngle)
		{
			dWLeft = dWLeft / sin(ToRadian(dAngApply));
			dWRight = dWRight / sin(ToRadian(dAngApply));
			dWWall = dWWall / sin(ToRadian(dAngApply));
		}

		long i = 0; for(i = 0; i < 3; i++)
		{
			double dLen			= 0;
			switch(i)
			{
				case 0: dLen	= dWLeft; break;
				case 1: dLen	= dWWall; break;
				case 2: dLen	= dWRight; break;
			}
			if(dLen <= 0) continue;
			dCTC	= bUpper ? dCtcUpper : dCtcLower;
			long nCount			= (long)((dLen-(dCtcLower*2)) / dCTC);
			double dRemain		= (dLen-(dCtcLower*2)) - (dCTC * nCount);
			// �ϸ��϶��� nCount�� ����϶��� 2�谡 �Ǵ��� üũ�ؾ� �ȴ�.
			if(!bUpper && !Compare(dCtcLower, dCtcUpper, "="))
			{
				long nCountUpper	= (long)((dLen-(dCtcLower*2)) / dCtcUpper);
				if(nCountUpper*2 != nCount)
				{
					dRemain	+= dCtcLower;
					nCount -= 1;
				}
			}



			if(i == 0)
			{
				double dDiff	= dRemainLower - (dRemain/2 + dCtcLower);
				pPlace->m_xyArr.Add(CDPoint(1, dRemainLower));
				pPlace->m_xyArr.Add(CDPoint(nCount, dCTC));
				pPlace->m_xyArr.Add(CDPoint(1, dRemain/2 + dCtcLower - dDiff));
			}
			else if(i == 1)
			{
				pPlace->m_xyArr.Add(CDPoint(1, dWWall));
			}
			else 
			{
				double dDiff	= dRemainLower - (dRemain/2 + dCtcLower);
				pPlace->m_xyArr.Add(CDPoint(1, dRemain/2 + dCtcLower - dDiff));
				pPlace->m_xyArr.Add(CDPoint(nCount, dCTC));
				pPlace->m_xyArr.Add(CDPoint(1, dRemainLower));
			}
		}
	}
	else
	{
		double dLen			= m_footing.GetWidth() - m_footing.GetCoverSide(TRUE, 0) * 2;
		if(bApplyAngle)
			dLen = dLen / sin(ToRadian(dAngApply));

		dCTC	= bUpper ? dCtcUpper : dCtcLower;
		long nCount			= (long)((dLen-(dCtcLower*2)) / dCTC);
		double dRemain		= (dLen-(dCtcLower*2)) - (dCTC * nCount);
		// �ϸ��϶��� nCount�� ����϶��� 2�谡 �Ǵ��� üũ�ؾ� �ȴ�.
		if(!bUpper && !Compare(dCtcLower, dCtcUpper, "="))
		{
			long nCountUpper	= (long)((dLen-(dCtcLower*2)) / dCtcUpper);
			if(nCountUpper*2 != nCount)
			{
				dRemain	+= dCtcLower;
				nCount -= 1;
			}
		}


		pPlace->m_xyArr.Add(CDPoint(1, dRemain/2 + dCtcLower));
		pPlace->m_xyArr.Add(CDPoint(nCount, dCTC));
		pPlace->m_xyArr.Add(CDPoint(1, dRemain/2 + dCtcLower));
	}

	pPlace->m_nCountBlock	= pPlace->m_xyArr.GetSize();

	m_footing.m_tvArrFront	= tvArrFrontOld;
}

// ����ö�� �ǰ��.
// ��� ���ö�� 1���� ������ ����Ѵ�.
void CWallApp::SetDataDefaultRebarShearFront_Footing(BOOL bFront)
{
	CPlaceRebarByBlock *pPlace	= bFront ? &m_footing.m_placeByBlock_Shear_Front : &m_footing.m_placeByBlock_Shear_Side;
	pPlace->m_xyArr.RemoveAll();
	pPlace->m_nCountBlock	= 0;

	// �Ϻι��ö�� 1���� ����.
	CDPoint xy(0, 0);
	CDPointArray xyArr;
	if(bFront)
	{
		m_footing.GetXyRebarSupport_Front(xyArr, TRUE, 0);

		// ��ü ��ö�ٿ� ���� ����� �ΰ��� ����.	
		CTwinVector tvWall[2]	= {GetTvOffset_Front(0, 0, FALSE, TRUE, TRUE, TRUE), GetTvOffset_Front(0, 0, FALSE, TRUE, FALSE, TRUE)};

		long left = 0; for(left = 0; left < 2; left++)
		{
			BOOL bLeft	= left == iLEFT;

			double dX	= bLeft ? tvWall[0].m_v1.x + m_dCoverLeft[0]: tvWall[1].m_v1.x - m_dCoverRight[0];
			double dMin	= 0;
			long nIdx	= -1;
// 			double dDist	= 0;
			long i = 0; for(i = 0; i < xyArr.GetSize(); i++)
			{
				double dDist	= fabs(dX - xyArr.GetAt(i).x);
				if(i == 0)
				{
					nIdx	= 0;
					dMin	= dDist;
				}
				else
				{
					if(dMin > dDist)
					{
						nIdx	= i;
						dMin	= dDist;
					}
				}
			}
			if(nIdx > -1)
			{
				xyArr.RemoveAt(nIdx);

				// (ARCBRIDGE-2781) ��ü�� �ٰ����ɷ� �¿� �ϳ����� ���� �Ǵµ� �̰� ����??? ���ܿ��� ��ü���� ���� �߰����� ���� �ִ�.
				// ������ �ִ� ��� �ޱ��� 1���� ���� �ݸ� ����.
				/*if((bLeft == m_footing.m_bLeftIsFrontGup))
				{
					if(bLeft)
					{
						if(nIdx > 1)
						{
							long nIdxMin	= m_footing.m_nType == FOOTING_TYPE_BASIC_BOTH ? 0 : nIdx / 2;

							if(nIdx > 0 && m_bPlacing23DanByStandardOffset)
							{
								xyArr.RemoveAt(nIdxMin);
							}


							long l = 0;
							for(l = nIdxMin; l > 0; l-=2)
							{
								xyArr.RemoveAt(l);
							}


						}
					}
					else
					{
						if(nIdx + 1 < xyArr.GetSize())
						{
							long nCountRemoveMax	= m_footing.m_nType == FOOTING_TYPE_BASIC_BOTH ? (xyArr.GetSize() - nIdx)/2 : (xyArr.GetSize() - nIdx) / 4;
							//long nCountRemove		= 0;
							for(long l = nIdx + nCountRemoveMax + 2; l < xyArr.GetSize()-1; l++)
							{
								xyArr.RemoveAt(l);
								//nCountRemove++;
								//if(nCountRemove >= nCountRemoveMax)
								//	break;
							}
						}
					}
				}*/
			}
		}
	}
	else
		m_footing.GetXyRebarMain_Side(xyArr, -1, FALSE, 0);


	// ���ö�� ù��°�� ������ �߰��ϰ�
	// ������ �ι�°, ������ ����° ö�ٺ��� warp�������� �ϳ��� �����ϰ�
	// ������ ö���� �׻� ��.
	long nCountOffset	= bFront ? 1 : m_footing.m_nCountWrapMainRebar;
	long nIdxStt	= bFront ? 1 : 2;
	CDPointArray xyArrSelect;
	long nSize	= xyArr.GetSize();
	long i = 0; for(i = 0; i < nSize; i++)
	{
		xy	= xyArr.GetAt(i);
		if(i == 0)
		{
			xyArrSelect.Add(xy);
		}
		else if(i == nSize-1 || i == nIdxStt-1)
		{
			continue;
		}
		else
		{
			if(((i-nIdxStt) % nCountOffset) == 0)
				xyArrSelect.Add(xy);
		}
	}

	// ��±��� 2���̻󸶴� ����ö���� ���� �ȴٸ� ���⼭ ����. ///////////////////
	long nOffset	= (long)(m_footing.m_dCTC_ShearRebar / m_footing.m_dCTC_SupportRebar);
	if(nOffset > 1)
	{
		CDPointArray xyArrTmp;

		long i = 0; for(i = 0; i < xyArrSelect.GetSize()-(nOffset-1); i+=nOffset)
		{
			xyArrTmp.Add(xyArrSelect.GetAt(i));
		}
		xyArrSelect.RemoveAll();
		xyArrSelect	= xyArrTmp;
	}
	////////////////////////////////////////////////////////////////////////////////////

	// �ʿ��Ѱ͸� ��� ���ö�� ������ �м��ؼ� ����ö�� �������� ġȯ�Ѵ�.
	CRebarPlacing rb;
	rb.ConvertPointArrayToOffsetInfo(xyArrSelect, pPlace->m_xyArr, 0);
	pPlace->m_nCountBlock	= pPlace->m_xyArr.GetSize();
}

// ��ü�� �������� �� �ִ���?
BOOL CWallApp::IsVertWall()
{
	if(!m_bIs) return TRUE;
	if(m_nType != WALL_TYPE_SLOPE) return TRUE;

	double dAngleUpperSlab	= GetAngleUpperSlab();
	if(Compare(m_dAngle + dAngleUpperSlab, 90.0, "=")) return TRUE;

	return FALSE;
}

double CWallApp::GetAngleUpperSlab(BOOL bApplyAngle)
{
	double dAng	= GetAngleByAngle(m_dAngleUpperSlab_Front, 90);// ? m_dAnglePath : 90);
	return dAng;
}

// ���� ���ؼ� ��ü�� ������ �ִ� ����(��ν����� ����� ������)
double CWallApp::GetAngleWallByHor(BOOL bApplyAngle)
{
	if(!m_bIs || IsVertWall())
	{
		return 90;
	}

	double dAng	= GetAngleByAngle(m_dAngle, bApplyAngle ? m_dAnglePath : 90);
	double dAngUp	= GetAngleUpperSlab(bApplyAngle);
	return dAng + dAngUp;
}

CFootingApp* CWallApp::GetFootingByRotateToVert(BOOL bLeft)
{
	CTwinVector tvWall	= m_tvArrFront.GetTvByInfo(bLeft ? "��ü������" : "��ü������");
	CTwinVector tvLowerOfWall		= m_tvArrFront.GetTvByInfo("��ü�ϸ����");
	CTwinVector tvLowerOfFooting	= m_footing.m_tvArrFront.GetTvByInfo("�����ϸ�");
	CDPoint xyOrg	= GetXyOrgTopForFront(bLeft);
	CDPoint xyDir	= tvWall.GetXyDir();

	CDPoint xyMatchOfWall(0, 0), xyMatchOfFooting(0, 0);
	GetXyMatchLineAndLine(xyOrg, xyDir, tvLowerOfWall.m_v1, tvLowerOfWall.GetXyDir(), xyMatchOfWall);
	GetXyMatchLineAndLine(xyOrg, xyDir, tvLowerOfFooting.m_v1, tvLowerOfFooting.GetXyDir(), xyMatchOfFooting);
	double dHeight	= ~(xyMatchOfFooting - xyMatchOfWall);
	double dELB	= GetXyOrgFootingLowerForSide(bLeft).y;

	m_footingByRotateToVert	= m_footing;
	m_footingByRotateToVert.m_dEL	= dELB + dHeight;
	m_footingByRotateToVert.m_dFH	= dHeight;
	m_footingByRotateToVert.m_nType	= FOOTING_TYPE_BASIC;
	m_footingByRotateToVert.m_bIs	= m_footing.m_bIs;
	m_footingByRotateToVert.m_bVirtualFooting	= m_footing.m_bVirtualFooting;

	CTwinVectorArray tvArrLeftSide;
	GetTvLeftSide(tvArrLeftSide, FALSE, FALSE, FALSE, TRUE, 0, TRUE, TRUE, TRUE, FALSE);
	m_footingByRotateToVert.m_tvPathLeft	= tvArrLeftSide.GetTvByInfo("��ü�ϸ�");
	GetTvLeftSide(tvArrLeftSide, FALSE, FALSE, FALSE, FALSE, 0, TRUE, TRUE, TRUE, FALSE);
	m_footingByRotateToVert.m_tvPathRight	= tvArrLeftSide.GetTvByInfo("��ü�ϸ�");

	m_footingByRotateToVert.SyncFooting();

	return &m_footingByRotateToVert;
}

double CWallApp::GetWidthFrontLower()
{
	CTwinVectorArray tvArrFront;
	tvArrFront	= m_tvArrFront;

	//31720
	CTwinVectorArray tvArrVertSection;
	tvArrVertSection.RemoveAll();

	GetTvFront(tvArrVertSection, FALSE);
	CTwinVector tvLeft	= m_tvArrFront.GetTvByInfo("��ü������");
	CTwinVector tvRight	= m_tvArrFront.GetTvByInfo("��ü������");
	CDPoint xyMatch(0, 0);

	// ��ü�� Round �ΰ�� GetxyDir�� ���� ������ �߸� ���´�.
	CDPoint xyDirRight = (tvRight.m_v2 - tvRight.m_v1).Unit();
	GetXyMatchLineAndLine(tvLeft.m_v1, CDPoint(1, 0), tvRight.m_v1, xyDirRight, xyMatch);

	m_tvArrFront	= tvArrFront;

	return fabs(tvLeft.m_v1.x - xyMatch.x);
}

void CWallApp::CheckOverLapLineBetweenSubObject()
{
	CTwinVector tv;
	CTwinVector tmp;
	if(m_footing.IsVirtualFooting() && m_tvArrLowerLimit_Front.GetSize() == 0) return;

	if(m_tvArrFront.GetTvByInfo("��ü�ϸ�����", tv))
	{
		if(m_tvArrLowerLimit_Front.GetSize() > 0)
		{
			tv.m_vecOverlap1	= tv.m_v1;
			tv.m_vecOverlap2	= tv.m_v2;
			m_tvArrFront.SetTvByInfo("��ü�ϸ�����", tv);
		}
		else
		{
			if(m_footing.m_tvArrFront.GetTvByInfo("���ʻ����������", tmp))
			{
				tv.m_vecOverlap1 = tmp.m_v1;
				tv.m_vecOverlap2 = tmp.m_v2;

				tmp.m_vecOverlap1 = tv.m_v1;
				tmp.m_vecOverlap2 = tv.m_v2;

				m_tvArrFront.SetTvByInfo("��ü�ϸ�����", tv);
				m_footing.m_tvArrFront.SetTvByInfo("���ʻ����������", tmp);
			}
			else if(m_footing.m_tvArrFront.GetTvByInfo("���ʰ����������", tmp))
			{
				tv.m_vecOverlap1 = tmp.m_v1;
				tv.m_vecOverlap2 = tmp.m_v2;

				tmp.m_vecOverlap1 = tv.m_v1;
				tmp.m_vecOverlap2 = tv.m_v2;

				m_tvArrFront.SetTvByInfo("��ü�ϸ�����", tv);
				m_footing.m_tvArrFront.SetTvByInfo("���ʰ����������", tmp);
			}
		}
	}

	if(m_tvArrFront.GetTvByInfo("��ü�ϸ����", tv))
	{
		if(m_tvArrLowerLimit_Front.GetSize() > 0)
		{
			tv.m_vecOverlap1	= tv.m_v1;
			tv.m_vecOverlap2	= tv.m_v2;
			m_tvArrFront.SetTvByInfo("��ü�ϸ����", tv);
		}
		else
		{
			if(m_footing.m_tvArrFront.GetTvByInfo("���ʻ���������", tmp))
			{
				tv.m_vecOverlap1 = tmp.m_v1;
				tv.m_vecOverlap2 = tmp.m_v2;

				tmp.m_vecOverlap1 = tv.m_v1;
				tmp.m_vecOverlap2 = tv.m_v2;

				m_tvArrFront.SetTvByInfo("��ü�ϸ����", tv);
				m_footing.m_tvArrFront.SetTvByInfo("���ʻ���������", tmp);
			}
			else if(m_footing.m_tvArrFront.GetTvByInfo("���ʰ���������", tmp))
			{
				tv.m_vecOverlap1 = tmp.m_v1;
				tv.m_vecOverlap2 = tmp.m_v2;

				tmp.m_vecOverlap1 = tv.m_v1;
				tmp.m_vecOverlap2 = tv.m_v2;

				m_tvArrFront.SetTvByInfo("��ü�ϸ����", tv);
				m_footing.m_tvArrFront.SetTvByInfo("���ʰ���������", tmp);
			}
		}
	}
}

void CWallApp::SyncWall_DesignData()
{
	// �������� ���ʿ� ��ü�� ���� �����ٰ� �� ������ �ƴ϶�..
	//m_footing.m_Fck	= m_Fck;
	//m_footing.m_Fy	= m_Fy;
	//m_footing.m_FyShear	= m_FyShear;
	//m_footing.m_dLengthRebarMax	= m_dLengthRebarMax;

	// ���
	m_Column.m_Fck	= m_Fck;
	m_Column.m_Fy	= m_Fy;
	m_Column.m_FyShear	= m_FyShear;
	m_Column.m_dLengthRebarMax	= m_dLengthRebarMax;
}

// ��ν����� ó�� ���� ��±� ���� �ǰ��
// ��ν����� �Ǻ� ö���� ���� ���� �κ��� �������� ��.
// ���� m_dRangeSupportRebarForSameUpperSlab�� 0�̾ �����ߵ� ö���� �� ����
void CWallApp::SetDataDefaultRangeSupportRebarForSameUpperSlab()
{
	m_dRangeSupportRebarForSameUpperSlab	= 0;
	m_placeByBlock_SupportRebarAddSameUpperSlab.m_xyArr.RemoveAll();
	m_placeByBlock_SupportRebarAddSameUpperSlab.m_xyArr.Add(CDPoint(0, 0));
	m_placeByBlock_SupportRebarAddSameUpperSlab.m_nCountBlock	= 1;
	long left = 0; for(left = 0; left < 2; left++)
	{
		m_dDistAddSupportRebarForSameUpperSlab[left]	= 0;	// ��ν����� ó�� ���� �߰��Ǵ� ��±��� �¿��� �Ÿ�
	}
}

double CWallApp::GetLength()
{
	CTwinVector tvLeft	= m_tvArrFront.GetTvByInfo("��ü������");
	CTwinVector tvRight	= m_tvArrFront.GetTvByInfo("��ü������");

	CDPoint xyMidUpper	= (tvLeft.m_v2 + tvRight.m_v2)/2;
	CDPoint xyMidLower	= (tvLeft.m_v1 + tvRight.m_v1)/2;

	return ~(xyMidUpper - xyMidLower);
}

double CWallApp::GetColumnMainRebarDia(long nCycle, long nDan, BOOL bLeft)
{
	double dRebarDia = 0;

	if(m_bTopIsColumn == TRUE)
	{
		dRebarDia = m_Column.m_dDiaMain[iOUTTER][nDan];
	}
	else
	{
		if(m_dArrDiaRC[nCycle][bLeft == TRUE ? iLEFT : iRIGHT][nDan].GetSize() > 0)
			dRebarDia = m_dArrDiaRC[nCycle][bLeft == TRUE ? iLEFT : iRIGHT][nDan].GetAt(0);
	}

	return dRebarDia;
}

void CWallApp::SetRebarLsdOption( SRebarEnvOption *pSLsdOption, BOOL bLeft, BOOL bMain, long nCycle, long nDan )
{
	if(m_pBasicConcInfo->IsDesignLsd() == FALSE)
		return;

	*pSLsdOption = *m_pBasicConcInfo->GetLsdRebarOpt();

	double dCtcMain = m_dCTC_MainRebar / 2;
	double dCtcSupp = GetCTCRebarSupport(bLeft, nDan, TRUE);

	double dDia(0);
	if(bMain)
	{
		CPlaceRebarByBlock *pPlace	= &m_placeByBlock_SupportRebar[bLeft? iLEFT:iRIGHT][nDan];

		dDia = pPlace->m_dDia;
	}
	else
	{
		// ��ü ��� �߿��� ���� ū �������� ��������.
		 long nSize = m_dArrDiaRC[nCycle][bLeft? iLEFT:iRIGHT][nDan].GetSize();

		for(long ix=0; ix<nSize; ++ix)
		{
			double dDiaCur = m_dArrDiaRC[nCycle][bLeft? iLEFT:iRIGHT][nDan].GetAt(ix);

			if(dDiaCur > dDia)
				dDia = dDiaCur;
		}
	}
	double dCover = bLeft? m_dCoverLeft[nDan] : m_dCoverRight[nDan];

	pSLsdOption->dFy = GetValueUnitNmm(m_Fy, _T("kgf/cm��"));
	pSLsdOption->dFck = GetValueUnitNmm(m_Fck, _T("kgf/cm��"));
	pSLsdOption->nRebarType = eStraitRebar;
	pSLsdOption->dCover = dCover;
	pSLsdOption->nHorDia	= (ERebarDia)m_pBasicConcInfo->GetIndexDia(dDia);

	pSLsdOption->ePosType = eRebarPosTension;
	pSLsdOption->dCheckCTC	= bMain? dCtcMain : dCtcSupp;
	pSLsdOption->dHorCTC	= bMain? dCtcSupp : dCtcMain;
	pSLsdOption->bUpperRebar = FALSE;
	pSLsdOption->dK = m_pBasicConcInfo->GetLsdRebarOpt()->dK_RebarPos[bMain ? iOUTTER : iINNER];
}

void CWallApp::GetXyRebarMain_Plan_For_Round_Wall( CDPointArray &xyArr, CDPointArray &xyArrDirApplyScale, long nDan, BOOL b2Cycle )
{
	if(b2Cycle)
	{
		// 2Cycle ö���� ���ٸ� 
		double dRebarDia = GetColumnMainRebarDia(1, nDan, TRUE);
		if(dRebarDia <= 0 || GetDanMainRebar(1, TRUE) <= nDan)
		{
			xyArr.RemoveAll();
			return;
		}

		long nR_org = m_Column.m_nR[iOUTTER][nDan];
		m_Column.m_nR[iOUTTER][nDan] = nR_org * 2 + 1;
		m_Column.GetXyRebarMain_Plan(xyArr, xyArrDirApplyScale, nDan);
		m_Column.m_nR[iOUTTER][nDan] = nR_org;

		// R ������ �ִ� ö���� �ϳ� �ɷ� �ϳ� ���� ���������� �ִ� ö���� ��δ� ���� ���̿� �ϳ����� �־��ش�.
		CTwinVectorArray tvArr;
		m_Column.GetTvPlan(FALSE);
		tvArr = m_Column.m_tvArrPlan[iOUTTER][iUPPER];
		CTwinVector tvLeft = tvArr.GetTvByInfo(_T("��մܸ�������"));
		CTwinVector tvRight = tvArr.GetTvByInfo(_T("��մܸ������"));
		double dTop = MAX(tvLeft.GetXyTop().y, tvRight.GetXyTop().y);
		double dBottom = MIN(tvLeft.GetXyBottom().y, tvRight.GetXyBottom().y);
		double dCenter = tvArr.GetXyCenter().x;

		CDPointArray xyArrUp, xyArrDn, xyArrLeft, xyArrRight;
		long nSize = xyArr.GetSize();
		for(long ix=0; ix<nSize; ++ix)
		{
			CDPoint xyPos = xyArr.GetAt(ix);
			if(Compare(xyPos.y, dTop, _T(">"), 1.0))
			{
				// ���� �����
				xyArrUp.Add(xyPos);
			}
			else if(Compare(xyPos.y, dBottom, _T("<"), 1.0))
			{
				// �Ʒ��� �����
				xyArrDn.Add(xyPos);
			}
			else
			{
				if(xyPos.x > dCenter)
					xyArrRight.Add(xyPos);
				else
					xyArrLeft.Add(xyPos);
			}
		}

		xyArr.RemoveAll();
		xyArrUp.Sort(TRUE);
		xyArrDn.Sort(TRUE);
		xyArrRight.Sort(FALSE);
		xyArrLeft.Sort(FALSE);

		nSize = xyArrUp.GetSize();
		for(long ix=0; ix<nSize; ++ ix)
		{
			if(ix%2 == 0)
				xyArr.Add(xyArrUp.GetAt(ix));
		}

		nSize = xyArrDn.GetSize();
		for(long ix=0; ix<nSize; ++ ix)
		{
			if(ix%2 == 0)
				xyArr.Add(xyArrDn.GetAt(ix));
		}

		nSize = xyArrRight.GetSize();
		for(long ix=0; ix<nSize-1; ++ ix)
		{
			CDPoint xy1 = xyArrRight.GetAt(ix);
			CDPoint xy2 = xyArrRight.GetAt(ix+1);
			
			xyArr.Add((xy1 + xy2)/2);
		}

		nSize = xyArrLeft.GetSize();
		for(long ix=0; ix<nSize-1; ++ ix)
		{
			CDPoint xy1 = xyArrLeft.GetAt(ix);
			CDPoint xy2 = xyArrLeft.GetAt(ix+1);

			xyArr.Add((xy1 + xy2)/2);
		}

	}
	else
	{
		m_Column.GetXyRebarMain_Plan(xyArr, xyArrDirApplyScale, nDan);
	}
}
