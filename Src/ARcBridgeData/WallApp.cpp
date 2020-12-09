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

// 벽체 종방향 
BOOL CWallApp::GetTvFront(CTwinVectorArray &tvArrVertSection, BOOL bApplyAngle)
{
	m_tvArrFront.RemoveAll();
	if(!m_bIs) return TRUE;

	double dAngle	= bApplyAngle ? m_dAnglePath : 90;
	double rad		= sin(ToRadian(dAngle));
	CDPoint vAngUpper	= ToDPointFrDegree(m_dAngleUpperWall_Front);

	m_tvArrFront.m_sInfo = "내부벽체";
	CTwinVector tv;
	CTwinVector tmp;

	// 입력받은 EL에 기초 높이를 감안해서 그려준다.
	double dELB	= GetELBottomOfWall();

	// 수직형
	if(m_nType == WALL_TYPE_VER)
	{
		BOOL bBoxType				= m_tvLowerLimitOfMainRebar.GetLength() >0;

		tv.m_bApplyAngle	= bApplyAngle;
		tv.m_dAngle			= dAngle;
		tv.m_v1				= tv.m_v2 = CVector(m_xyOrg.x, dELB, 0);
		tv.m_v2.x			+= m_dW/2/rad;
		tv.m_sInfo			= "벽체하면우측";
		tv.val.valHor		= NULL;
		tv.val.valVer		= &m_dELB;
		tv.val.bRevMoveVer	= FALSE;
		m_tvArrFront.Add(tv);

		tv.m_bApplyAngle	= bApplyAngle;
		tv.m_dAngle			= 90;
		tv.m_v1				= tv.m_v2;
		tv.m_v2.y			= m_xyOrg.y + (bBoxType ? m_dDiffTop[1] : 0);
		tv.m_sInfo			= "벽체우측면";
		tv.val.valHor		= &m_dW;
		tv.val.valVer		= NULL;
		tv.val.bRevMoveHor	= FALSE;
		m_tvArrFront.Add(tv);

		tv.m_bApplyAngle	= bApplyAngle;
		tv.m_dAngle			= dAngle;
		tv.m_v1				= tv.m_v2 = CVector(m_xyOrg.x, dELB, 0);
		tv.m_v2.x			-= m_dW/2/rad;
		tv.m_sInfo			= "벽체하면좌측";
		tv.val.valHor		= NULL;
		tv.val.valVer		= &m_dELB;
		tv.val.bRevMoveVer	= FALSE;
		m_tvArrFront.Add(tv);

		tv.m_bApplyAngle	= bApplyAngle;
		tv.m_dAngle			= 90;
		tv.m_v1				= tv.m_v2;
		tv.m_v2.y			= m_xyOrg.y + (bBoxType ? m_dDiffTop[0] : 0);
		tv.m_sInfo			= "벽체좌측면";
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

		tv.m_sInfo			= "벽체상면";
		tv.val.valHor		= NULL;
		tv.val.valVer		= NULL;
		m_tvArrFront.Add(tv);
	}
	// 경사형
	else if(m_nType == WALL_TYPE_SLOPE)
	{
		double dAngle	= GetAngleWallByHor(bApplyAngle);
		CDPoint xyDir = ToDPointFrRadian(ToRadian(dAngle));
		CDPoint xyDirLower = CDPoint(1, 0);
		CDPoint xyCenLower(0, 0);
		CDPoint xySideUpper(0, 0), xySideLower(0, 0);

		// 벽체하면 각도
		// 기초에 경사가 없을때는 수평, 경사가 있을때는 경사에 맞춰서 각도 정해짐.
		// 그런데 각도처리 안하면 수평으로 나옴
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
		
		// 기둥하단 중심
		GetXyMatchLineAndLine(m_xyOrg, xyDir, CDPoint(m_xyOrg.x, dELB), CDPoint(1, 0), xyCenLower);

		tv.m_bApplyAngle	= bApplyAngle;
		tv.m_dAngle			= dAngle;
		tv.m_v1				= tv.m_v2 = xyCenLower;
		xySideUpper			= m_xyOrg;
		xySideLower			= xyCenLower;
		// 기둥 우측면(offset)
		Offset(xySideLower, xySideUpper, m_dW/2/rad);
		GetXyMatchLineAndLine(xyCenLower, xyDirLower, xySideUpper, xyDir, xySideLower);
		tv.m_v2				= xySideLower;
		tv.m_sInfo			= "벽체하면우측";
		tv.val.valHor		= NULL;
		tv.val.valVer		= &m_dELB;
		tv.val.bRevMoveVer	= FALSE;
		m_tvArrFront.Add(tv);

		tv.m_bApplyAngle	= bApplyAngle;
		tv.m_dAngle			= 90;
		tv.m_v1				= tv.m_v2;
		GetXyMatchLineAndLine(xySideLower, xyDir, m_xyOrg, vAngUpper, xySideUpper);
		tv.m_v2				= xySideUpper;
		tv.m_sInfo			= "벽체우측면";
		tv.val.valHor		= &m_dW;
		tv.val.valVer		= NULL;
		tv.val.bRevMoveHor	= FALSE;
		m_tvArrFront.Add(tv);

		tv.m_bApplyAngle	= bApplyAngle;
		tv.m_dAngle			= dAngle;
		tv.m_v1				= tv.m_v2 = xyCenLower;
		xySideUpper			= m_xyOrg;
		xySideLower			= xyCenLower;
		// 기둥 좌측면(offset)
		Offset(xySideLower, xySideUpper, -m_dW/2/rad);
		GetXyMatchLineAndLine(xyCenLower, -xyDirLower, xySideUpper, xyDir, xySideLower);
		tv.m_v2				= xySideLower;
		tv.m_sInfo			= "벽체하면좌측";
		tv.val.valHor		= NULL;
		tv.val.valVer		= &m_dELB;
		tv.val.bRevMoveVer	= FALSE;
		m_tvArrFront.Add(tv);

		tv.m_bApplyAngle	= bApplyAngle;
		tv.m_dAngle			= 90;
		tv.m_v1				= tv.m_v2;
		GetXyMatchLineAndLine(xySideLower, -xyDir, m_xyOrg, vAngUpper, xySideUpper);
		tv.m_v2				= xySideUpper;
		tv.m_sInfo			= "벽체좌측면";
		tv.val.valHor		= &m_dW;
		tv.val.valVer		= NULL;
		tv.val.bRevMoveHor	= TRUE;
		m_tvArrFront.Add(tv);

		tv.m_bApplyAngle	= bApplyAngle;
		tv.m_dAngle			= dAngle;
		tv.m_v1				= tv.m_v2;
		m_tvArrFront.GetTvByInfo("벽체우측면", tmp);
		tv.m_v2				= Compare(tmp.m_v1.y, tmp.m_v2.y, ">") ? tmp.m_v1 : tmp.m_v2;
		tv.m_sInfo			= "벽체상면";
		tv.val.valHor		= NULL;
		tv.val.valVer		= NULL;
		m_tvArrFront.Add(tv);
	}
	// V형
	else if(m_nType == WALL_TYPE_V)
	{
		CDPoint xyCenV		= CDPoint(m_xyOrg.x, m_xyOrg.y-m_dH);
		CDPoint xyDirRight	= ToDPointFrRadian(ToRadian(90-m_dAngleV/2));
		CDPoint xyDirLeft	= ToDPointFrRadian(ToRadian(90+m_dAngleV/2));
		CDPoint xyRightUpper(0, 0), xyLeftUpper(0, 0);
		CDPoint xyUpper(0, 0), xyLower(0, 0);
		// 오른쪽 팔 중심선 top
		GetXyMatchLineAndLine(xyCenV, xyDirRight, CDPoint(0, m_dELRT), CDPoint(1, 0), xyRightUpper);
		// 왼쪽 팔 중심선 top
		GetXyMatchLineAndLine(xyCenV, xyDirLeft, CDPoint(0, m_dELLT), CDPoint(1, 0), xyLeftUpper);

		tv.m_bApplyAngle	= bApplyAngle;
		tv.m_dAngle			= dAngle;
		tv.m_v1				= tv.m_v2 = CVector(m_xyOrg.x, dELB, 0);
		tv.m_v2.x			+= m_dW/2/rad;
		tv.m_sInfo			= "벽체하면우측";
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
		tv.m_sInfo			= "벽체우측면";
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
		tv.m_sInfo			= "벽체우측팔우측면";
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
		tv.m_sInfo			= "벽체우측팔상면";
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
		tv.m_sInfo			= "벽체우측팔좌측면";
		tv.val.valHor		= &m_dWR;
		tv.val.valVer		= NULL;
		tv.val.bRevMoveHor	= TRUE;
		m_tvArrFront.Add(tv);

		tv.m_bApplyAngle	= bApplyAngle;
		tv.m_dAngle			= dAngle;
		tv.m_v1				= tv.m_v2 = CVector(m_xyOrg.x, dELB, 0);
		tv.m_v2.x			-= m_dW/2/rad;
		tv.m_sInfo			= "벽체하면좌측";
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
		tv.m_sInfo			= "벽체좌측면";
		tv.val.valHor		= NULL;
		tv.val.valVer		= &m_dH;
		tv.val.bRevMoveVer	= TRUE;
		m_tvArrFront.Add(tv);

		tv.m_bApplyAngle	= bApplyAngle;
		tv.m_dAngle			= 90;
		tv.m_v1				= tv.m_v2;
		GetXyMatchLineAndLine(tv.m_v1, xyDirLeft, xyLeftUpper, CDPoint(-1, 0), xyUpper);
		tv.m_v2				= xyUpper;
		tv.m_sInfo			= "벽체좌측팔좌측면";
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
		tv.m_sInfo			= "벽체좌측팔상면";
		tv.val.valHor		= NULL;
		tv.val.valVer		= NULL;
		m_tvArrFront.Add(tv);

		tv.m_bApplyAngle	= bApplyAngle;
		tv.m_dAngle			= 90;
		tv.m_v1				= tv.m_v2;
		m_tvArrFront.GetTvByInfo("벽체우측팔좌측면", tmp);
		tv.m_v2				= tmp.m_v2;
		tv.m_sInfo			= "벽체좌측팔우측면";
		tv.val.valHor		= &m_dWL;
		tv.val.valVer		= NULL;
		tv.val.bRevMoveHor	= FALSE;
		m_tvArrFront.Add(tv);
	}

	// 기둥식일경우 기둥의 정면좌표도 한번 구해주라.. ㅎㅎ
	if(m_bTopIsColumn)
		m_Column.GetTvFront(TRUE, FALSE, ToDPointFrDegree(dAngle));

	// 정면에 대한 동기화 ////////////////////////////////////////////////////////////////
	if(m_nType == WALL_TYPE_SLOPE && !IsVertWall())
	{
		//CDPoint vAngJijum	= ToDPointFrDegree(m_dAnglePath);
		double dAngle	= GetAngleWallByHor(bApplyAngle);

		CDPoint xyWallCenter(0, 0);
		CDPoint xyDir = ToDPointFrRadian(ToRadian(180+dAngle));
		GetXyMatchLineAndLine(m_xyOrg, xyDir, CDPoint(m_xyOrg.x, dELB), CDPoint(1, 0), xyWallCenter);

		CDPoint xyDirSlope = m_footing.GetXyDirSlope(m_footing.m_nType == FOOTING_TYPE_LEFTSLOPE, bApplyAngle);
		//double dLenSlope = sqrt((m_footing.m_dFW/vAngJijum.y*m_footing.m_dFW/vAngJijum.y)+m_footing.m_dFH*m_footing.m_dFH);
		
		CTwinVector tvLeft	= m_tvArrFront.GetTvByInfo("벽체좌측면");
		CTwinVector tvRight	= m_tvArrFront.GetTvByInfo("벽체우측면");
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
		// 기초 형식을 왜바꾸지????
		// 일단 Box일때는 바꾸지 않도록 하자.
		if(m_footing.m_nType != FOOTING_TYPE_BOX)
			m_footing.m_nType = FOOTING_TYPE_BASIC_BOTH;
		m_footing.m_dML = m_dW/2;
		m_footing.m_dMR = m_dW/2;
		m_footing.m_dSta = m_xyOrg.x;
		m_footing.m_dEL = m_dELB + m_footing.GetHeight();
	}
	m_footing.m_dELDiff	= m_dELBDiff;

	// 하부 한계가 있다면 하부 한계까지만 그린다. 하부 한계보다 벽체가 밑에 있다면 완전히 없앤다. ㅡㅡ;
	if(m_tvArrLowerLimit_Front.GetSize() > 0)
	{
		long nIdxLeft	= m_tvArrFront.GetIdxByInfo("벽체좌측면"); 
		long nIdxRight	= m_tvArrFront.GetIdxByInfo("벽체우측면");
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
		nIdxLeft	= m_tvArrFront.GetIdxByInfo("벽체하면좌측");
		nIdxRight	= m_tvArrFront.GetIdxByInfo("벽체하면우측");
	
		tvLeft.m_v1	= xyMid;
		tvLeft.m_v2	= xyLeft;
		tvLeft.m_sInfo	= "벽체하면좌측";
		tvRight.m_v1	= xyMid;
		tvRight.m_v2	= xyRight;
		tvRight.m_sInfo	= "벽체하면우측";
		m_tvArrFront.SetAt(nIdxLeft, tvLeft);
		m_tvArrFront.SetAt(nIdxRight, tvRight);
	}


	// 정면 좌우측 아크형태
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

			strLeft.Format(_T("%d번째내측헌치좌측면"), ix+1);
			strRight.Format(_T("%d번째내측헌치우측면"), ix+1);
			
			tvLeftBottomSlab = tvArrVertSection.GetTvByInfo(strLeft);
			tvRightBottomSlab = tvArrVertSection.GetTvByInfo(strRight);

			tvArrLeftSlab.Add(tvLeftBottomSlab);
			tvArrRightSlab.Add(tvRightBottomSlab);
		}
		
		CTwinVector tvLeft	= m_tvArrFront.GetTvByInfo("벽체좌측면");
		CTwinVector tvRight	= m_tvArrFront.GetTvByInfo("벽체우측면");
		if(tvLeft.GetVerLength() > 0)
		{
			// 31720
			//라운드 접점 부위를 처리하기 위함 포인트 비교를 통하여 길이가 0에 가까운 것을 찾아서 포인트를 맞춰줌
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
			m_tvArrFront.SetTvByInfo("벽체좌측면", tvLeft);
		}

		if(tvRight.GetVerLength() > 0)
		{
			//{31720 
			//라운드 접점 부위를 처리하기 위함 포인트 비교를 통하여 길이가 0에 가까운 것을 찾아서 포인트를 맞춰줌
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
			m_tvArrFront.SetTvByInfo("벽체우측면", tvRight);
		}
	}
	//////////////////////////////////////////////////////////////// 정면에 대한 동기화 //

	
	return TRUE;
}

// 벽체의 꼭대기 중심 위치 찾기
// bLeft는 V형일때 사용
CDPoint CWallApp::GetXyWallTopCen(BOOL bLeft)
{
	CTwinVector tv;

	//31720
	CTwinVectorArray tvArrVertSection;
	tvArrVertSection.RemoveAll();

	GetTvFront(tvArrVertSection, FALSE);
	if(m_nType == WALL_TYPE_V)
	{
		if(bLeft) m_tvArrFront.GetTvByInfo("벽체좌측팔상면", tv);
		else m_tvArrFront.GetTvByInfo("벽체우측팔상면", tv);
	}
	else
	{
		m_tvArrFront.GetTvByInfo("벽체상면", tv);
	}

	CDPoint xy1 = tv.m_v1;
	CDPoint xy2 = tv.m_v2;

	return (xy1+xy2)/2;
}

// 벽체하단이 왼쪽으로 갔는지
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

// 벽체 단면 좌표 구함.
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
		// 어짜피 없으면 그리지 않으면 되니까.. 구지 막지는 않음
		// 헌치평면좌표 구할때 이용하기도 해야 되므로 슬래브인 경우도 벽체 평면 좌표가 필요한 경우가 있음.
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
			// V형일때는 좌측팔 우측팔 모두 구함.
			long n = m_nType == WALL_TYPE_V ? 3 : 1;
			BOOL bVertWall	= IsVertWall();
			if(!bVertWall) n += 1;
			CString str = _T("");
			double dW = GetWidthFrontLower();

			CTwinVector tvLeftUpper, tvLeftLower;
			long i = 0; for(i = 0; i < n; i++)
			{
				if(i == 0) str = "벽체";
				else
				{
					if(!bVertWall)
					{
						tv	= GetTvWallTopCen(FALSE);
						str	= "벽체하면";
					}
					else
					{
						if(i == 1) 
						{
							tv = GetTvWallTopCenOfTypeV(TRUE);
							str = "벽체좌측팔";
							dW = m_dWL;
						}
						else if(i == 2) 
						{
							tv = GetTvWallTopCenOfTypeV(FALSE);
							str = "벽체우측팔";
							dW = m_dWR;
						}
					}
				}

				CTwinVector tvLeft = tv;
				CTwinVector tvRight = tv;

				Offset(tvLeft.m_v1, tvLeft.m_v2, -dW/2);
				Offset(tvRight.m_v1, tvRight.m_v2, dW/2);

				// 직각적용 아닐때
				if(!m_bRAngRight)
				{
					CDPoint xyDirStt = m_vAngRight;
					// 왼쪽 정리
					GetXyMatchSegAndLine(tvLeft.m_v2, tvLeft.m_v1, tv.m_v1, xyDirStt, tvLeft.m_v1);
					//오른쪽 정리
					GetXyMatchSegAndLine(tvRight.m_v2, tvRight.m_v1, tv.m_v1, xyDirStt, tvRight.m_v1);
				}

				// 직각적용 아닐때
				if(!m_bRAngLeft)
				{
					CDPoint xyDirEnd = m_vAngLeft;
					// 왼쪽정리
					GetXyMatchSegAndLine(tvLeft.m_v1, tvLeft.m_v2, tv.m_v2, xyDirEnd, tvLeft.m_v2);
					// 오른쪽 정리
					GetXyMatchSegAndLine(tvRight.m_v1, tvRight.m_v2, tv.m_v2, xyDirEnd, tvRight.m_v2);
				}

				tv.m_v1 = tvLeft.m_v1;
				tv.m_v2 = tvRight.m_v1;
				tv.m_sInfo = str+"평면하면";
				m_tvArrPlan.Add(tv);
				
				tv.m_v1 = tv.m_v2;
				tv.m_v2 = tvRight.m_v2;
				tv.m_sInfo = str+"평면우측";
				m_tvArrPlan.Add(tv);

				tv.m_v1 = tv.m_v2;
				tv.m_v2 = tvLeft.m_v2;
				tv.m_sInfo = str+"평면상면";
				m_tvArrPlan.Add(tv);

				tv.m_v1 = tv.m_v2;
				tv.m_v2 = tvLeft.m_v1;
				tv.m_sInfo = str+"평면좌측";
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

	// 평면에 대한 동기화 ////////////////////////////////////////////////////////////////
	m_footing.m_tvPathPlan	= GetTvWallTopCen(FALSE);//m_tvPathPlan;

	// 좌측면에 대한 동기화
	CTwinVectorArray tvArrWallLeft;
	GetTvLeftSide(tvArrWallLeft, TRUE, FALSE);
	tvArrWallLeft.GetTvByInfo("벽체하면", m_footing.m_tvPathLeft);

	GetTvLeftSide(tvArrWallLeft, TRUE, FALSE, FALSE, FALSE);
	tvArrWallLeft.GetTvByInfo("벽체하면", m_footing.m_tvPathRight);

	m_footing.m_dCenterOfWall	= m_dCenterOfWall;
	
	// 기초에 직각적용할때 기준으로 할 Path의 시작부와 끝부의 기준선을 정의해줌.
	m_tvArrPlan.GetTvByInfo("벽체평면하면", m_footing.m_tvRightAngStt);
	m_tvArrPlan.GetTvByInfo("벽체평면상면", m_footing.m_tvRightAngEnd);
	//////////////////////////////////////////////////////////////// 평면에 대한 동기화 //

	return TRUE;
}

// 벽체가 높여지는 위치의 중심좌표 리턴
CTwinVector CWallApp::GetTvWallTopCenOfTypeV(BOOL bLeft)
{
	CTwinVector tv;
	CTwinVector tv1;
	CString str = bLeft ? "벽체좌측팔상면" : "벽체우측팔상면";
	m_tvArrFront.GetTvByInfo(str, tv);
	m_tvArrFront.GetTvByInfo("벽체좌측팔우측면", tv1);

	// 중심과 각 팔과의 거리
	double dOffset = tv1.m_v2.x - tv.GetXyMid().x;
	
	// 벽체 중심좌표와의 거리로 offset
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

// 기둥 간격 권고안
// 좌우 대칭으로 나눠 줌.
// 기본 간격 받고 개수 받아서 자동으로 나누 어 줌.
// BOOL bDefaultCountColumn : 기둥개수로 권고안 할지..?
void CWallApp::SetDataDefaultDistColumn(BOOL bDefaultCountColumn)
{
	if(m_nCountColumn <= 0 && bDefaultCountColumn==FALSE) return; // bDefaultCountColumn인데 리턴시키면 벽체에서 기둥으로 변경한 경우 갯수 권고안이 안됩.

	CTwinVector tvPath = GetTvWallTopCen();
	CRebarPlacing rb;
	CDPointArray xyArrDist;
	
	// 기둥개수 권고안
	// 기본간격 3000을 기준으로 기둥개수 찾음
	if(bDefaultCountColumn)
	{
		m_nCountColumn = (long)(tvPath.GetLength() / 3000)-1;
	}

	long nCount = m_nCountColumn * 2;
	double dDist = tvPath.GetLength() / nCount;

	// 간격을 100단위로 자름
	dDist = (long)(dDist / 100) * 100;

	// 첫번재 간격 결정
	double dOffsetStt = tvPath.GetLength() - (dDist * (nCount-1));
	if(dOffsetStt > dDist)
		dOffsetStt -= (dOffsetStt-dDist)/2;
	
	// 결정된 간격을 입력
	m_dDistColumn[0] = dOffsetStt;
	long i = 0; for(i = 1; i < m_nCountColumn; i++)
		m_dDistColumn[i] = dDist*2;
}

// 벽체의 중심선을 리턴
// 벽체 양쪽 마진 감안
CTwinVector CWallApp::GetTvWallTopCen(BOOL bUpper)
{
	CTwinVector tv = bUpper ? m_tvPathPlan : m_tvPathPlanLower;
	CDPoint xyDir = GetXyDirTop();
	tv.m_v1 += xyDir * m_dMarginRight;
	tv.m_v2 -= xyDir * m_dMarginLeft;

	return tv;
}

// 기초에 대한 권고안
BOOL CWallApp::SetDataDefaultShapeFooting()
{
	/*
	if(!m_footing.m_bIs) return TRUE;	// 기초가 없는 경우라면 기초값 초기화 하고 리턴.
	
	double dW	= m_dW;
	
	// * 기초 저면의 폭(B)(0.6H <= B <= 0.9H) (RcBridge에서는 1번 적용)
	// 1. 고정단 : 3.5 + 0.8 * (H - 5.0)
	// 2. 가동단 : 3.5 + 0.7 * (H - 5.0)
	double dB		= 3.5 + 0.8 * (dH - 5.0);
	if(dB < 0.6 * dH) dB	= 0.6 * dH;
	if(dB > 0.9 * dH) dB	= 0.9 * dH;

	// * 기초 앞굽의 길이 (H : 전체 높이, B : 기초 너비, tW : 벽체 너비) - RcBridge에서는 1번 적용
	// 1. 비탈면에 교대가 높은경우 : H/6
	// 2. 평지또는 교대 전면에 제약을 받지 않거나, 말뚝기초로 하는 경우 : B//3

	// * 기초 뒷굽의 길이
	// 1. B - (앞굽 + tW)

	// * 기초의 두께(최소 0.8)
	// 1. (B/5 - 0.1) >= 0.8
	// 2. (B < 5) 일때는 0.8

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

	// 기초 권고안을 하면 확대기초 권고안도 해야 된다.
	BOOL bResult = m_footing.SetDataDefaultShapeExFooting();
	*/

	return TRUE;
}

// 벽체에 대해 동기화를 맞춰줌
// 동기화를 하려면 좌표가 먼저 구해져 있어야 한다.
// 좌표구하는 함수는 따로 한번 호출해준 뒤에 이 짓을 함...
void CWallApp::SyncWall(CTwinVectorArray &tvArrVertSection, BOOL bApplyAngle, double dAngle)
{
	m_dAnglePath	= dAngle;

	// 기둥식은 아니지만 라운드 식이면 기둥을 라운드벽체에 맞게 셋팅 해 놓는다.
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

	// 벽체가 없으면 기초도 당연히 없다.
	if(!m_bIs) m_footing.m_bIs = FALSE;

	m_footing.SyncFooting(bApplyAngle, dAngle);

	// 겹치는 선에 대해서 정의해줌
	CheckOverLapLineBetweenSubObject();

	// 배근관련 동기화 ////////////////////////////////////////////////////////////
	m_footing.m_bSameOffsetPlaceTAndCBlock	= m_bSameOffsetPlaceTAndCBlock;
	m_footing.m_bPlacing23DanByStandardOffset	= m_bPlacing23DanByStandardOffset;
	// 지원 사이클 수
	m_footing.m_nIdxCountCycleMainRebar	= m_nIdxCountCycleMainRebar;
	m_footing.m_nIdxCTC_MainRebar		= m_nIdxCTC_MainRebar;

	// 기둥식일경우 기둥 사이는 상하면 CTC가 뒤집어 져야 된다. 그 영역을 정의해줌.
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

	// 설계환경
	SyncWall_DesignData();
	CTwinVector tvLower							= m_footing.GetTvOffset_Front(0, FALSE, m_footing.IsVertDir());
	CRebarPlacing rb;
	m_Column.m_dCTCMainRebar	= (long)(m_bApplyAngleToMainRebarCTC ? m_dCTC_MainRebar/sin(ToRadian(m_dAnglePath)) : m_dCTC_MainRebar);
	// 기둥은 사이클 개념이 없으므로 ctc를 반으로 해야됨.
	// 단, 라운드 벽체는 입력된 주철근 사이에 2cycle을 자동으로 넣어서 배근하므로 ctc를 반으로 주면 안됨.
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
		// (38038) 위에서 GetTvFront(tvArrVertSection, bApplyAngle);를 해서 각도 고려해서 구했는데 여기서 각도무시하고 다시 만듬?
//		m_Column.GetTvFront();
	}
	rb.MoveRebar(m_Column.m_tvArrUpperLimitOfMainRebarSide, CDPoint(0, -m_dCoverUpper_UpperSlab));
	m_Column.SyncColumn();
	m_Column.m_tvLowerLimitOfMainRebar			= (m_footing.m_bIs || m_footing.IsVirtualFooting())? tvLower : m_tvLowerLimitOfMainRebar;
	m_Column.m_footing							= m_footing;
	m_Column.m_footing.SyncFooting();

	//////////////////////////////////////////////////////////// 배근관련 동기화 //
}

// 벽체 정면 주철근 좌표 리턴(V형 벽체 고려)
// bArm : v형의 경우 팔부분인지?(
// bLeftArm : 팔부분이라면 왼쪽 팔인지?
// bLeftRebar : 좌측 철근인지?
CTwinVector CWallApp::GetTvOffset_Front(long nCycle, long nDan, BOOL bArm, BOOL bLeftArm, BOOL bLeftRebar, BOOL bVertDir)
{
	CTwinVector tvWall;
	if(nCycle == -1) nCycle = GetCountCycleMainRebar()-1;
	if(nCycle < 0 || nCycle > GetCountCycleMainRebar()-1) return tvWall;
	if(nDan == -1) nDan = MAX_REBAR_DAN-1;
	if(nDan < 0 || nDan > MAX_REBAR_DAN-1) return tvWall;

	double dRad		= sin(ToRadian(bVertDir ? 90 : m_dAnglePath));
	double dCover	= bLeftRebar ? m_dCoverLeft[nDan]/dRad : -m_dCoverRight[nDan]/dRad;
	
	// V형이고 bArm인 경우만 다르고 나머진 같음.
	if(bArm && m_nType == WALL_TYPE_V)
	{
		
	}
	else
	{
		CString sInfo	= bLeftRebar ? _T("벽체좌측면") : _T("벽체우측면");
		if(m_tvArrFront.GetTvByInfo(sInfo, tvWall))
		{
			Offset(tvWall.m_v1, tvWall.m_v2, dCover);
		}
	}

	return tvWall;
}

// 모든 사이클, 모든 단의 rebar info cycle만드는 함수
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

	// 일단 기둥식은 rebar info cycle를 만들지 않음
	if(m_bTopIsColumn) return;

	if(m_nType == WALL_TYPE_V)
	{
	}
	else
	{
		CTwinVector tvWall	= GetTvOffset_Front(nCycle, nDan, FALSE, FALSE, bLeft, bVertDir);
		//double sw			= bLeft ? -1 : 1;
		
		// 기초 하면과 교점(기초가 있으면)
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
		// 기초가 없다면 벽체하부 한계를 이용해서 ..
		else if(Compare(m_tvLowerLimitOfMainRebar.GetLength(), 0.0, ">"))
		{
			CDPoint xyMatch(0, 0);
			if(GetXyMatchLineAndLine(tvWall.m_v1, tvWall.GetXyDir(), m_tvLowerLimitOfMainRebar.m_v1, m_tvLowerLimitOfMainRebar.GetXyDir(), xyMatch))
			{
				tvWall.m_v1	= xyMatch;
			}
		}

		// 벽체 상부 교점
		if(Compare(m_tvUpperLimitOfMainRebar.GetLength(), 0.0, ">"))
		{
			CDPoint xyMatch(0, 0);
			if(GetXyMatchLineAndLine(tvWall.m_v1, tvWall.GetXyDir(), m_tvUpperLimitOfMainRebar.m_v1, m_tvUpperLimitOfMainRebar.GetXyDir(), xyMatch))
			{
				tvWall.m_v2	= xyMatch;
			}
		}

		// 기초가 slope형일댄 벽체철근 하단부 라운드 방향을 한쪽으로 몰아야 된다.
		/*
		if((m_footing.m_nType == FOOTING_TYPE_LEFTSLOPE && !bLeft) || (m_footing.m_nType == FOOTING_TYPE_RIGHTSLOPE && bLeft))
		{
			sw *= -1;
		}
		*/
	
		CHgBaseConcStd baseConc;

		
		long left		= bLeft ? iLEFT : iRIGHT;
		CDPoint xyStt	= CDPoint(tvWall.m_v1.x, tvWall.m_v1.y + m_footing.m_dCoverLower[0]);	// 시작위치
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

			// 철근 지름과 가장자리철근 모따기밑 라운드 크기 ////////////
			double dDia		= GetDiaMainRebar(nCycle, m_bDetailInput23DanMainRebar ? nDan : 0, bLeft, joint);
			double dDiaReal	= GetDiaMainRebar(nCycleReal, nDan, bLeft, joint);
			BOOL bExist		= IsExistMainRebar(nCycleReal, nDan, bLeft, joint);
			double dDiaNext	= GetDiaMainRebar(nCycle, m_bDetailInput23DanMainRebar ? nDan : 0, bLeft, joint+1);
			double dLenRound	= (joint == 0) ? GetLenRebarAtEdge(dDia, TRUE) : GetLenRebarAtEdge(dDia, FALSE);
			/////////////////////////////////////////////////////////////////

			pArrRC->Add(new CRebarInfoCycle);
			CRebarInfoCycle *pRC	= pArrRC->GetAt(pArrRC->GetSize()-1);

			// 공통속성
			pRC->m_szMark		= bExist ? GetMarkRebarMain(bLeft ? MARK_LEFT : MARK_RIGHT, bInput, nCycle, FALSE, TRUE, _T(""), nDan) : "";
			pRC->m_nTypeRebar	= REBAR_TYPE_MAIN;
			pRC->m_nPosEnd		= bLeft ? REBARINFO_POS_INWALL_LEFT : REBARINFO_POS_INWALL_RIGHT;
			pRC->m_Dia			= dDia;
			pRC->m_bExist		= bExist;
			pRC->m_nRebarDan	= 0;
			
			// 시작위치는 항상 기초에서 시작한다.
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

	//2,3단 철근 확장길이 적용
	if(nDan > 0 && m_bDetailInput23DanMainRebar == FALSE)
	{
		ApplyExtLength23DanMainRebar(pArrRC, GetLengthExt23DanRebar(bLeft), m_tvUpperLimitOfMainRebar);
	}

	// Not Exist인 rc 제거
	/*
	if(!bInput)
		RemoveNotExistRebarInfoCycle(pArrRC);
		*/

	// sDescription 설정
	SetStringDescriptionRebarInfoCycle(pArrRC, bLeft, nDan, nCycleReal);

	// 마지막 철근은 길이 변화 가능 옵션 설정해줌
	if(pArrRC->GetSize() > 0)
	{
		CRebarInfoCycle *pRC	= pArrRC->GetAt(pArrRC->GetSize()-1);
		pRC->m_bVariableLength	= TRUE;
	}
}

// 철근 지름 리턴
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
	// 2단부터는 다른 변수에 저장되어 있다
	// 2단이상은 그냥 내측 외측으로 지름이 구분되어 있음.
	// 2단이 index 0이고 3단이 1이다.
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

// 철근 지름 리턴
BOOL CWallApp::IsExistMainRebar(long nCycle, long nDan, BOOL bLeft, long nIdx)
{
	if(!m_bDetailInput23DanMainRebar && (nCycle == 0 || nCycle == 2)) return TRUE;

	// 4cycle은 입력받지 않으므로 2cycle껄로 사용
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

// 철근 배치 단수 리턴
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

// 수직철근 절곡부..
double CWallApp::GetLenRebarAtEdge(double dDia, BOOL bInFooting)
{
	if(!m_pBasicConcInfo) return 0;
	double dLen	= 0;
	if(m_nTypeCurveOfVertRebar == 0)		// 자동
		dLen	= m_pBasicConcInfo->GetRebarInfo(dDia, BASIC_RADIUS);
	else if(m_nTypeCurveOfVertRebar == 1)	// 곡선
		dLen	= m_pBasicConcInfo->GetRebarInfo(dDia, BASIC_RADIUS);
	else									// 직선
		dLen	= 0;	

	if(bInFooting && m_nTypeCurveOfVertRebar == 0) 
	{
		if(Compare(dDia, 19.0, "<=")) dLen	= 0;
	}

	return dLen;
}

// BOOL bApplyRebarRadius : 주철근 중심에서 철근기호지름을 고려해서 위치를 구할지?
void CWallApp::GetXyRebarSupport_Front(CDPointArray &xyArr, BOOL bLeft, long nDan, BOOL bApplyRebarRadius, BOOL bVertDir)
{
	xyArr.RemoveAll();
	if(m_bNotRebar) return;

	if(nDan == -1) nDan = MAX_REBAR_DAN-1;
	if(nDan < 0 || nDan > MAX_REBAR_DAN-1) return;

	long left	= bLeft ? iLEFT : iRIGHT;
	// 2단, 3단은 입력을 받지 않고 1단의 3개까지 같고 다음부터는 1단 2개에 하나씩 2,3단에 들어간다.
	CPlaceRebarByBlock *pPlace	= &m_placeByBlock_SupportRebar[left][0];

	// 벽체 좌우측 피복
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

	// 철근 좌표 찾기
	CRebarPlacing rb;
	rb.SettingRadius(dRadRebar, TRUE, FALSE);
	rb.GetXyDivideLine(xyArr, tvWall.m_v1, tvWall.m_v2, pPlace->m_xyArr, FALSE, FALSE, FALSE);

	// 2단부터는 실제로 주철근이 배근된 곳에만 배력철근이 배근된다. 
	if(nDan > 0)
	{
		long nCountOffset	= m_bSameOffsetPlaceTAndCBlock ? 1 : 2;	// 2단 배근시 1단철근 몇개마다 하나씩 배근할지?
		
		// 동일 간격이 아닐때는 표준간격의 1/2씩 배근된 곳이 있다.
		// 그 곳에는 m_bPlacing23DanByStandardOffset 옵션 적용 가능함
		if(!m_bSameOffsetPlaceTAndCBlock)
		{
			if(m_bPlacing23DanByStandardOffset == FALSE)// || GetCountCycleMainRebar() > 1)
			{
				nCountOffset	= 1;
			}
		}	

		// 2단, 3단은 입력을 받지 않고 1단의 nCountSame개까지 같고 다음부터는 1단 2개에 하나씩 2,3단에 들어간다.
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

	// 필요한 RebarInfoCycle 배열을 찾음
	long left	= bLeft ? iLEFT : iRIGHT;
	
	CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC = &m_pArrRCMain[nCycle][left][nDan];

	long nSize	= pArrRC->GetSize();
	if(nSize == 0) return NULL;
	if(nIdx == -1) nIdx = nSize-1;
	if(nIdx >= nSize) return NULL;
	if(nIdx < 0) return NULL;

	return pArrRC->GetAt(nIdx);
	/*
	// 포인터를 넘겨도 되지만 별개로 관리해야 메모리 관리가 어지럽지 않을꺼 같아서 이렇게 함.
	*pRC	= *pArrRC->GetAt(nIdx);

	return TRUE;
	*/
}

// long nVertDir : -1 : 직거리, 0 : 자동(원래설정), 1 : 사거리
void CWallApp::GetTvRebarShear_Front(CTwinVectorArray &tvArr, BOOL bVertDir)
{
	tvArr.RemoveAll();
	if(m_bNotRebar) return;
	
	// 기둥식일땐 전단철근이 없지요.. 
	if(m_bTopIsColumn) return;

	// 상부슬래브 상하면 1단철근에 전단철근 배근함.
	// 단.. 전단철근이 피복을 적용하지 않은 벽체 외측부터 시작되어야 하므로 1단철근 좌표 구할때 피복을 적용하지 않고 구함.
	CFootingApp *pFooting	= &m_footing;
	CTwinVector tvWall		= GetTvOffset_Front(0, 0, FALSE, FALSE, TRUE, bVertDir);	// 전단철근의 길이 되어 줄 외측벽체 철근..
	CTwinVector tvLower;
	if((pFooting->m_bIs || pFooting->IsVirtualFooting()) && pFooting->m_tvArrFront.GetTvByInfo("기초하면", tvLower))
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

	// 전단철근 배치할 위치 찾음.
	CRebarPlacing rb;
	CDPointArray xyArr;
	CPlaceRebarByBlock *pPlace	= &m_placeByBlock_ShearRebar;
	rb.GetXyDivideLine(xyArr, tvWall.m_v1, tvWall.m_v2, pPlace->m_xyArr, FALSE, FALSE, FALSE);

	// 위치를 알았으면 길이를 줘서 전단철근을 만듬
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

	// 기둥식일땐 전단철근이 없지요.. 
	if(m_bTopIsColumn) return;

	CRebarPlacing rb;
	CTwinVector tvCenter;
	CTwinVectorArray tvArrPlan;
	m_Column.GetTvRebarRound_Plan(tvArrPlan, 0);
	tvArrPlan = m_Column.m_tvArrPlan[iOUTTER][iLOWER];
	tvCenter = tvArrPlan.GetTvByInfo(_T("기둥단면우측면"));
	rb.Offset(tvCenter, -m_dCoverLeft[0], -m_dCoverLeft[0]);

	// 전단철근 배치할 위치 찾음.
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
	
	// 위치를 알았으면 길이를 줘서 전단철근을 만듬
	CTwinVector tv;
	double dLen	= m_dW - (m_dCoverLeft[0] + m_dCoverRight[0]);
	CDPoint xyDir = tvCenter.GetXyDir().Rotate90();
	rb.MakePointArrayToLineArray(xyArr, tvArr, xyDir, dLen, FALSE);
}

// 좌측은 철근 길이로 권고안 하고, 우측은 좌측을 권고안 한 다음에 우측으로 수직으로 뻗어서 
// 우측 철근에 교차되는 좌표로 권고안 한다.
void CWallApp::SetDataDefaultRebarSupport_VertSection_InWall(BOOL bLeft)
{
	long left	= bLeft ? iLEFT : iRIGHT;
	
	// 라운드벽체는 우측이 좌측과 항상 같아야 되므로 우측권고안시 좌측권고안 하듯이 한다////
	if(IsRoundRectWall() && !bLeft)
		bLeft	= TRUE;
	/////////////////////////////////////////////////////////////////////////////////////////

	CPlaceRebarByBlock *pPlace	= &m_placeByBlock_SupportRebar[left][0];
	pPlace->m_xyArr.RemoveAll();
	if(m_bTopIsColumn) return;

	CHgBaseConcStd baseConc;
	double dCTC			= m_dCTC_SupportRebar;

	// 벽체 주철근철근 길이(벽체 주철근중 수직(경사)인 것만 구함.)
	CRebarInfoCycle *pRCStt	= GetRebarInfoCycle(0, 0, bLeft, 0),
					*pRCEnd	= GetRebarInfoCycle(0, 0, bLeft, -1);
	CRebarInfoCycle *pRCSttOther	= GetRebarInfoCycle(0, 0, !bLeft, 0);
	
	if(!pRCStt || !pRCEnd || !pRCSttOther) return;
	

	CTwinVector tvWall;
	if(ComparePt(pRCStt->m_xyStt, pRCEnd->m_xyStt, "="))	// rc가 처음과 끝이 같다면 하나 짜리이고 다르면 여러개 라는 얘기
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
		// 기초가 없는 형태로 하단 라운드부분이 없다.
		tvWall.m_v1	= pRCStt->m_xyStt;
	}

	// 상부 한계가 있다면 한계까지만 만듬
	CDPoint xyMatch(0, 0);
	if(GetXyMatchSegAndSeg(m_tvUpperLimitOfSupportRebar.m_v1, m_tvUpperLimitOfSupportRebar.m_v2, tvWall.m_v1, tvWall.m_v2, xyMatch))
	{
		tvWall.m_v2	= xyMatch;
	}
	

	// 기초가 있다면 기초 상면 철근에서 자름
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

		// 벽체철근 라운드 상단부터 기초 상면 철근까지 ////////////////////////////////////
		CRebarInfoCycle *pRC	= GetRebarInfoCycle(0, 0, bLeft, 0);
		if(pRC)
		{
			if(m_bRebarLowerSettle[left]) //m_footing.m_bIs || m_footing.IsVirtualFooting())
			{
				CDPoint vAng	= (pRC->m_xyEnd - pRC->m_xyMid2).Unit();
				double dDist	= 0;
				CTwinVector tvLower	= m_tvArrFront.GetTvByInfo("벽체하면우측");
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
				// 벽체철근 라운드 상단부터 기초 상면 철근까지 ////////////////////////////////////
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
				// 벽체철근 라운드 상단부터 기초 상면 철근까지 ////////////////////////////////////
			}
		}

		// 기초 상면 철근에서 상부슬래브 하면철근까지 
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

			// 우측은 좌측철근 좌표에 맞춰서 권고안 해야 됨.
			CDPoint xyMatch;
			CDPointArray xyArrRight;
			xyArrRight.Add(xySttSupportRebar);

			long nSize	= xyArr.GetSize();
			long i = 0; for(i = 0; i < nSize; i++)	// 첫번째 철근은 뺀다(시작위치는 다를 수 있음)
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


// 전단철근 권고안
void CWallApp::SetDataDefaultRebarShear_VertSection()
{
	if(m_bTopIsColumn) return;

	CRebarPlacing rb;
	CPlaceRebarByBlock *pPlace	= &m_placeByBlock_ShearRebar;
	pPlace->m_xyArr.RemoveAll();
	pPlace->m_nCountBlock	= 0;

	// 외측벽체 배력철근에서 상부슬래브하면과 기초상면 사이에 있는 철근에 배치됨.
	CTwinVector tvWall		= GetTvOffset_Front(0, 0, FALSE, TRUE, TRUE, TRUE);
	CTwinVector tvLower;
	CFootingApp *pFooting	= &m_footing;
	if(pFooting->m_bIs || pFooting->IsVirtualFooting())
	{
		pFooting->m_tvArrFront.GetTvByInfo("기초하면", tvLower);
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

	// 배력근의 2개이상마다 전단철근이 들어가야 된다면 여기서 뺀다. ///////////////////
	long nOffset	= (long)(m_dCTC_ShearRebar / m_dCTC_SupportRebar);
	if(nOffset > 1)
	{
		// 간격이 바뀌면서 어긋나는것을 최대한 맞춰보자.
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


	// 배력철근에 기초하면 좌표하나 추가해줌(전단철근이 시작되는 위치기 때문)
	xyArrSup.InsertAt(0, CDPoint(tvWall.m_v1.x, tvWall.m_v1.y));

	// 범위안에 드는 것을 제거한 배력철근 간격을 분석해서 전단철근 간격으로 치환한다.
	rb.ConvertPointArrayToOffsetInfo(xyArrSup, pPlace->m_xyArr, 1);
	pPlace->m_nCountBlock	= pPlace->m_xyArr.GetSize();
}

// 벽체 좌측면도..
// 좌측면도 상부 슬래브 기준 좌표가 없으면 0,0을 기준으로 걍 그려버림.ㅋㅋ
// BOOL bGetWall : 기둥식이라도 벽체의 좌표를 구한다.(특정한 용도)
// BOOL bIncludeFooting : 기초를 포함한 좌표를 구함(전단철근 그릴때나 사용한다) - 사실상 별로 의미 없음(기초와 벽체의 너비가 다른 경우가 있기 때문임)
// BOOL bIncludeUpperSlab : 상부슬래브도 포함할지..(경사형 벽체일경우만 사용됨)
// 주의사항
// 벽체가 기울어진 형태일 경우 수직으로 세워서그린다.
// 단, 하단을 기준으로 그림.
// BOOL bForRebar : 배근을 위한건지?(헌치까지 벽체가 있더라도 기본적으로 상부스래브 까지 뻗어준다)
// BOOL bCorrectionEL : 헌치가 없는 경우 경사에 의해 생기는 상하단 EL 보정.
void CWallApp::GetTvLeftSide(CTwinVectorArray &tvArr, BOOL bGetWall, BOOL bIncludeFooting, BOOL bIncludeUpperSlab, BOOL bLeft, long nDrawLeft, BOOL bForRebar, BOOL bVertDir, BOOL bOnlyViewFromLeft, BOOL bPattern, BOOL bCorrectionEL)
{
	tvArr.RemoveAll();


	CRebarPlacing rb;
	CDPoint vAngRot	= GetXyDirRotate_Front();
	CDPoint xyOrgTop	= GetXyOrgTopForFront(bLeft);

	CTwinVectorArray tvArrLeftUpper;
	tvArrLeftUpper	= m_tvArrLeftUpper;

	// 입력받은 EL에 기초 높이를 감안해서 그려준다.
	double dELB	= GetXyOrgFootingLowerForSide(bLeft).y;
	if(!bIncludeFooting)
	{
		CTwinVector tvWall		= m_tvArrFront.GetTvByInfo(bLeft ? "벽체좌측면" : "벽체우측면");
		tvWall	= rb.RotateTv(tvWall, xyOrgTop, vAngRot.GetAngleDegree());

		dELB = tvWall.GetXyBottom().y;

		if(bCorrectionEL)
		{
			// 상하단 EL을 보정한다면 지점 중간위치일것이다.
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
	// 상부 기준 좌표가없으면 기본 값으로 그림.
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


	// 기둥식일때 기둥의 측면도를 모은다.
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


	// 좌측하단에서 위로 가는 방향으로 그림.
	// 지반은 수평한것으로 치고 그림.
	CDPoint xyMatch(0, 0);
	GetXyMatchLineAndLine(xyLeft, CDPoint(0, -1), CDPoint(xyLeft.x, dELB), CDPoint(1, 0), xyMatch);
	tv.m_v1		= xyMatch;
	tv.m_v2		= xyLeft;
	tv.m_sInfo	= "벽체좌측";
	tvArr.Add(tv);

	if(nSize == 0)
	{
		tv.m_v1		= tv.m_v2;
		tv.m_v2		= xyRight;
		tv.m_sInfo	= "벽체상면";
		tvArr.Add(tv);
	}
	else
	{
		long i = 0; for(i = 0; i < nSize-1; i++)
		{
			tv	= tvArrLeftUpper.GetAt(i);
			tv.m_sInfo	= "벽체상면";
			tvArrLeftUpper.SetAt(i, tv);
		}

		tv	= tvArrLeftUpper.GetAt(nSize-1);
		tv.m_sInfo	= "벽체상면끝";
		tvArrLeftUpper.SetAt(nSize-1, tv);
		
		
		tvArr.AddFromTvArray(tvArrLeftUpper);
	}

	GetXyMatchLineAndLine(xyRight, CDPoint(0, -1), CDPoint(xyRight.x, dELB), CDPoint(-1, 0), xyMatch);
	tv.m_v1		= xyRight;
	tv.m_v2		= xyMatch;
	tv.m_sInfo	= "벽체우측";
	tvArr.Add(tv);

	tv			= tvArr.GetAt(0);
	tv.m_v2		= tv.m_v1;
	tv.m_v1		= xyMatch;
	tv.m_sInfo	= "벽체하면";
	tvArr.Add(tv);


	// 상부슬래브를 포함하는 벽체일 경우(실제로 그리면 안 맞을 수 있음)
	if(bIncludeUpperSlab)
	{
		long left = 0; for(left = 0; left < 2; left++)
		{
			CDPoint xyMatch(0, 0);
			long nIdx	= tvArr.GetIdxByInfo(left == iLEFT ? "벽체좌측" : "벽체우측");
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

// 벽체 좌측면 좌측에서 떨어진 거리에서의 상부슬래브 꼭대기 좌표
// dXInLeft : 좌측면도 에서의 x좌표
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

		// 못 찾은 경우 처음과 끝점과의 교점을 그냥 찾음.
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

// rebarinfocycle 배열 두개를 합친다.
// BOOL bInvert : 합칠 배열을 뒤집어서 합칠지..?
// 이 함수 호출시 반드시 메모리 해제 해 줄것.
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

// rebarinfocycle를 뒤집는다.
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

// 측면 배력철근(무조건 좌측에서 본 모양)
// BOOL bCutAddRebar : 추가 꺽임 철근을 자를지?(철근 마크용일때는 안 잘라야됨)
void CWallApp::GetTvRebarSupport_Side(CTwinVectorArray &tvArr, CTwinVectorArray &tvArrAngle, CTwinVectorArray &tvArrAngleAdd, BOOL bLeftWall, long nDan, BOOL bLeft, BOOL bCutAddRebar, BOOL bCoverLocation)
{
	tvArr.RemoveAll();
	tvArrAngle.RemoveAll();
	tvArrAngleAdd.RemoveAll();
	if(m_bNotRebar) return;

	// 배력철근 간격 구함.(기초하면 부터 띄우면서 구하면 됨)
	CDoubleArray dArrDist;
	GetDistRebarSupport_Front(dArrDist, bLeftWall, nDan, bCoverLocation);

	// 벽체 좌우측
	BOOL bIncludeFooting	= TRUE;
	BOOL bIncludeUpperSlab	= m_nType == WALL_TYPE_SLOPE ? TRUE : FALSE;
	CTwinVectorArray tvArrSide;
	GetTvLeftSide(tvArrSide, FALSE, bIncludeFooting, bIncludeUpperSlab, bLeft, 0, TRUE);
	CTwinVector tvLeft	= tvArrSide.GetTvByInfo("벽체좌측"),
				tvRight	= tvArrSide.GetTvByInfo("벽체우측");
	
	// 좌우측 좌표를 아래에서 위로 항하도록 수정.
	if(tvLeft.m_v1.y > tvLeft.m_v2.y) GetSwap(tvLeft.m_v1, tvLeft.m_v2);
	if(tvRight.m_v1.y > tvRight.m_v2.y) GetSwap(tvRight.m_v1, tvRight.m_v2);

	// 철근길이 구함.
	CDPoint vAng	= ToDPointFrDegree(m_dAnglePath);
	double	dCoverFront	= GetCoverFront(TRUE, 0)/vAng.y,
			dCoverBack	= GetCoverFront(FALSE, 0)/vAng.y;
	double dLenHor		= fabs(tvRight.m_v1.x-tvLeft.m_v1.x)-dCoverFront-dCoverBack;

	// 배근함.
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
	// 배근 시작 위치는 벽체 주철근 상단 라운드 부터 시작.
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
	

	// 가각 지원 /////////
	xyStt.x -= m_dWidthExp[iLEFT];
	dLenHor += m_dWidthExp[iLEFT] + m_dWidthExp[iRIGHT];
	// 가각 지원 /////////


	// 철근중에서 벽체 상면에 걸리는 철근은 무조건 사로 그려 버린다.
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


	// 슬래브 모양 철근 영역 구함.
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

				// (ARCBRIDGE-3078) 편경사에 의한 위치를 고려하려면 Match되는 점을 그냥 옮겨주면 안되고 연장선에서 찾아줘야 한다.
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

		// 추가 꺽임 철근은 필요한 부분만 골라낸다.
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

// 측면 배력철근 간격 
// 기초 하면에서 부터의 간격
// 경사형일경우 수직거리가 아니라 실제 거리를 리턴해야 됨
// 그리고 여기서 구하는 거리는 실제 위치가 아니라. 측면 배근도를 그리기 위한 기준 위치에서의 거리가 된다.
// 따라서 시작 거리가 옵션에 따라서 조금씩 달라 질 수 있다.
void CWallApp::GetDistRebarSupport_Front(CDoubleArray &dArr, BOOL bLeft, long nDan,BOOL bCoverLocation )
{
	// 배력철근 좌표
	CDPointArray xyArr;
	// 실제 거리를 정확하게 하기 위해서 구조물 좌우측으로 옮긴 좌표를 구한다.
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

	
	
	// 간격 배열로 변환
	long nSize	= xyArr.GetSize();
	CDPoint xy1(0, 0), xy2(0, 0);
	long i = 0; for(i = 0; i < nSize-1; i++)
	{
		xy1	= xyArr.GetAt(i);
		xy2	= xyArr.GetAt(i+1);
		dArr.Add(~(xy2-xy1));
	}

	// 첫번째 철근은 기초 하면부터의 거리를 사용한다.
	if(nSize > 0)
	{
		if(m_footing.m_bIs || m_footing.IsVirtualFooting())
		{
			// 기초함면과 첫번째 철근을 수직으로 세운다.
			CDPoint xy	= xyArr.GetAt(0);
			CTwinVector tvLower	= m_footing.m_tvArrFront.GetTvByInfo("기초하면");
			CRebarPlacing rb;
			CDPoint xyOrgTop	= GetXyOrgTopForFront(bLeft);
			double dAngRot		= GetXyDirRotate_Front().GetAngleDegree();
			tvLower	= rb.RotateTv(tvLower, xyOrgTop, dAngRot);
			rb.RotateXy(xy, xyOrgTop, dAngRot);

			// 수직으로 세워진 상태에서 벽체를 그리는 기준 X위치로 이동
			xy.x		= GetXyOrgTopForFront(bLeft).x;

			
			// 교차점 찾음
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

// 측면 주철근(무조건 좌측에서 본 모양)
// BOOL bIncludeRound : 전단철근은 직선구간에 있는 주철근에 대해서만 들어가므로 라운드부 주철근은 포함하지 않을 경우가 있다.
// nIdxRC	= 999 : 철근 마크를 위해 인덱스 관계없이 첨부터 끝까지 구함.
void CWallApp::GetTvRebarMain_Side(CTwinVectorArray &tvArr, BOOL bLeftRebar, long nCycle, long nDan, long nIdxRC, CTwinVectorArray &tvArrJoint, BOOL bIncludeRound, BOOL bLeft, BOOL bForDim)
{
	tvArr.RemoveAll();
	tvArrJoint.RemoveAll();
	if(m_bNotRebar) return;
	const double dGapJoint	= 36;	// 주철근과 이음간의 간격
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
			return;	// 철근이 없으면 리턴
	}
	else
	{
		if(!pRCTmp->m_bExist && nIdxRC != 999) return;
		pRC	= new CRebarInfoCycle;
		*pRC	= *pRCTmp;
	}
	CRebarInfoCycle *pRCTmp1	= GetRebarInfoCycle(nCycle==-1?0:nCycle, nDan, bLeftRebar, nIdxRC+1);	// 이음을 구하기 위한 다음 철근
	CRebarInfoCycle cRCNext;
	if(pRCTmp1 != NULL && pRCTmp1->m_bExist)
	{
		bJoint = TRUE;
		cRCNext = *pRCTmp1;
	}
		
	CRebarPlacing rb;

	// rc를 슬래브상면 기준점을 기준으로 수직으로 세운다.
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
		

		// 이음을위한 철근도 회전 시켜줌
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


	// 철근을 상부슬래브 높이 차에 따라 변화 시킬지?
	BOOL bVariableLength	= pRC->m_bVariableLength;	// 높이가 변화되는 철근인지?
	BOOL bVariablePos		= pRC->m_bVariablePos;	// 시작위치가 변화되는 철근인지?
	BOOL bBoxType				= m_tvLowerLimitOfMainRebar.GetLength() >0;

	// 이음이 없는 경우를 판단.(마지막철근은 이음 고려 안함)
	if(nIdxRC == GetCountRebarInfoCycle(nCycle==-1?0:nCycle, nDan, bLeftRebar)-1)
	{
		bJoint			= FALSE;
	}

	BOOL bBoxTypeNotSlabJoint = (pRC->m_nPosStt == REBARINFO_POS_STTWALL_OUTTER && pRC->m_nPosEnd == REBARINFO_POS_ENDWALL_OUTTER) && bBoxType;


	// 주철근이 들어가는 범위(수직구간,라운드벽체의경우라운드부는나중에따로 구함) ////
	CTwinVectorArray tvArrSide;
	GetTvOffset_Side(tvArrSide, bLeft, 0);	// 종단면상의 2단철근은 피복이 2단피복이 적용 되지만, 측면도에서는 항상 1단피복이 적용됨
	CTwinVector tvLeft	= tvArrSide.GetTvByInfo("벽체좌측"),
				tvRight	= tvArrSide.GetTvByInfo("벽체우측");

	CTwinVector tvHorOrg;
	tvHorOrg.m_v1	= tvLeft.GetXyBottom();
	tvHorOrg.m_v2	= tvRight.GetXyBottom();
	// 사보강배치 일때 가각 부분까지 주철근이 배치된다. 그려줄땐 빼더라도 배치 및 치수에는 고려한다
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
	

	// 측면 주철근 간격 구함 ///////////////////////////////////////////////////////////////////
	CDPointArray xyArrOffset;
	GetOffsetInfoRebarMain_Plane_LineSection(xyArrOffset, TRUE, 0);
	CHgBaseConcStd baseConc;
	CDPointArray xyArr;
	
	// 1cycle에 벽체라운드부에 들어가는 철근 포함 시킴.
	if(IsRoundRectWall())
	{
		xyArr.RemoveAll();
		CDPointArray xyArrTmp, xyArrTmp2;

		// 원래는 벽체의 주철근 간격을 구해서 넣어줫었으나,
		// 나중에 기둥식벽체의 철근 간격을 입력받는 방법으로 변경되어서 아래의 코드를 주석처리함.
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
	// 2cycle인 경우 1cycle의 사이에 하나씩 들어감. ////////////////////
	if(IsRoundRectWall())
	{
		CDPointArray xyArrTmp;
		long nSize	= xyArr.GetSize();
		if(nCycle != -1)
		{
			long nStt	= nCycle == 0 || nCycle == 2 ? 0 : 1;
			long i = 0; for(i = nStt; i < nSize-1-nStt; i+=2)
				xyArrTmp.Add(xyArr.GetAt(i));
			// 1,3 cycle이고 r이 홀수이면 끝에 하나 남음.. 이거는 좌측 벽체로 편입
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
		// IsRoundRectWall()일때는  평면에서 1,2 Cycle을 따로 가져오고 여기서는 조정하지 않는다.
		if(nCycle == 1 || nCycle == 3 || nCycle == -1)
		{
			CDPointArray xyArrTmp;

			// 상/하부 철근
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

	// 2cycle인 경우 1cycle의 사이에 하나씩 들어감. ////////////////////
	// 4cycle까지 지원되는 경우
	// 1, 2cycle인경우 짝수번째를, 3,4cycle인경우 홀수번째를 번째좌표를 지우면 된다.
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

	// 치수용 주철근은 가각부에 들어간 주철근까지 모두 만들어주고 그리기용은 가각을제외한 벽체 내에 있는 것만 만들어준다.
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

	// 벽체 상면은 직선이 아니므로 벽체 상면에 붙도록 해서 철근 생성
	CTwinVector tvMain;			// 현재 배근해야 되는 주철근.(높이정보만 사용할 것임)
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
		/* 원래는 경사교까지 고려를 했으나 집계에 문제가 있으므로 직선철근까지라도 정확하게 그림.
		if(!IsVertWall())
		{
			if(pRC->m_xyEnd.y > pRC->m_xyMid2.y)
			{
				// 경사교의 경우 수직으로 세워놓으면 R부분이 더 아래쪽으로 내려 갈 수도 있으므로.. ///
				tvMain.m_v1	= pRC->m_xyMid2;
				if(tvMain.m_v1.y > pRC->m_xyCen.y - pRC->m_R_Mid)
					tvMain.m_v1.y = pRC->m_xyCen.y - pRC->m_R_Mid;
				// 경사교의 경우 수직으로 세워놓으면 R부분이 더 아래쪽으로 내려 갈 수도 있으므로.. ///

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
	tvArrSide.GetTvArrByInfo("벽체상면", tvArrUpper, TRUE, TRUE);
	tvArrSide.GetTvArrByInfo("벽체상면끝", tvArrUpperEnd, TRUE, TRUE);
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

	// 이음이 있는지 다시 점검
	if(bJoint)
	{
		if(tvMainNext.m_v1.y < tvMain.m_v2.y)
			bJoint	= TRUE;
		else
			bJoint	= FALSE;
	}
	
	// 철근이 상부슬래브를 뚫었는지 체크한다.
	// 안 뚫었다면 경사에 의해서 측면도에서 상부슬래브를 뚫고 가는 철근을 잘라 준다.
	// 나중에 작업함.(일단은 상부슬래브를 뚫고 가는 철근은 무조건 잘라버림
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

		// 혹시라도 철근이 벽체를 뚫고 나가게 되는 경우가 있으므로 상면은 벽체 끝까지만 그림.
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

// 벽체 측면 철근 좌표들.
// bLeft : 현재 좌측만 지원
// 하면은 기초하면을 기준함.
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

	// 기초와 벽체 측면 구함.
	BOOL bIncludeFooting	= TRUE;
	BOOL bIncludeUpperSlab	= m_nType == WALL_TYPE_SLOPE ? TRUE : FALSE;
	CTwinVectorArray tvArrSide, tvArrFootingSide, tvArrHidden;
	GetTvLeftSide(tvArrSide, FALSE, bIncludeFooting, bIncludeUpperSlab, bLeft, 0, TRUE, FALSE);

	// 부위별로 구분
	CTwinVector tvLeft	= tvArrSide.GetTvByInfo("벽체좌측"),
				tvRight	= tvArrSide.GetTvByInfo("벽체우측"),
				tvLower	= tvArrSide.GetTvByInfo("벽체하면");
	CTwinVectorArray tvArrUpper;
	tvArrSide.GetTvArrByInfoRange("벽체상면", "벽체상면끝", tvArrUpper);


	// 각가 부위들을 offset
	if(tvLeft.m_v1.y > tvLeft.m_v2.y) GetSwap(tvLeft.m_v1, tvLeft.m_v2);
	if(tvRight.m_v1.y > tvRight.m_v2.y) GetSwap(tvRight.m_v1, tvRight.m_v2);
	if(tvLower.m_v1.x > tvLower.m_v2.x) GetSwap(tvLower.m_v1, tvLower.m_v2);
	Offset(tvLeft.m_v1, tvLeft.m_v2, dCoverBack);		// 좌
	Offset(tvRight.m_v1, tvRight.m_v2, -dCoverFront);	// 우
	rb.GetOffsetTvArray(tvArrUpper, dCoverUpper);		// 상
	Offset(tvLower.m_v1, tvLower.m_v2, -dCoverLower);	// 하

	// offset 된 것들을 교차점 처리
	GetXyMatchLineAndLine(tvLeft.m_v2, tvLeft.GetXyDir(TRUE), tvLower.m_v1, tvLower.GetXyDir(), tvLeft.m_v1);
	tvLower.m_v1	= tvLeft.m_v1;
	GetXyMatchLineAndLine(tvRight.m_v2, tvRight.GetXyDir(TRUE), tvLower.m_v1, tvLower.GetXyDir(), tvRight.m_v1);
	tvLower.m_v2	= tvRight.m_v1;

	// tvLeft와 tvRight는 offset하고 나면 tvUpper와 떨어 질 수 있음(편경사 때문에)
	// 그럼 trim이 잘 안됨.. 붙여주자
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

// 벽체 측면기준 중심 선
// double dExtLen : 위아래로 확장되는 길이
CTwinVector CWallApp::GetTvCenter_Side(BOOL bLeft, double dExtLen)
{
	CTwinVectorArray tvArrSide;
	BOOL bIncludeFooting	= TRUE;
	BOOL bIncludeUpperSlab	= TRUE;
	GetTvLeftSide(tvArrSide, FALSE, bIncludeFooting, bIncludeUpperSlab, bLeft, 0, TRUE);


	CDRect rect	= tvArrSide.GetRect();
	CTwinVector tvLower	= tvArrSide.GetTvByInfo("벽체하면");
	CTwinVector tvCenter;
	tvCenter.m_v1	= CVector(tvLower.GetXyMid().x, rect.bottom, 0);
	tvCenter.m_v2	= CVector(tvLower.GetXyMid().x, rect.top, 0);

	// 확장길이 적용.
	tvCenter.m_v1	= tvCenter.m_v1 + (tvCenter.GetXyDir(TRUE) * dExtLen);
	tvCenter.m_v2	= tvCenter.m_v2 + (tvCenter.GetXyDir() * dExtLen);

	return tvCenter;
}

// 도로 중심 리턴
// 현재 입력된게 없으므로 가장 높은 곳을 도로 중심으로 친다. 
// CTwinVector CWallApp::GetTvRoadCenter_Side(BOOL bLeft, double dExtLen)
// {
// 	// 중심선을 구한뒤에 X좌표를 찾음.
// 	CTwinVector tvCenter	= GetTvCenter_Side(bLeft, dExtLen);
// 
// 	// x좌표 찾음
// 	tvCenter.m_v1.x	= 0;
// 	tvCenter.m_v2.x	= 0;
// 	
// 	return tvCenter;
// }


// 주철근이 몇개 인지 판단(이음이 있을 수도 있음)
long CWallApp::GetCountRebarInfoCycle(long nCycle, long nDan, BOOL bLeft)
{
	if(nCycle == -1) nCycle = GetCountCycleMainRebar()-1;
	if(nCycle < 0 || nCycle > GetCountCycleMainRebar()-1) return FALSE;
	if(nDan == -1) nDan = MAX_REBAR_DAN-1;
	if(nDan < 0 || nDan > MAX_REBAR_DAN-1) return FALSE;

	// 필요한 RebarInfoCycle 배열을 찾음
	long left	= bLeft ? iLEFT : iRIGHT;
	
	CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC	= &m_pArrRCMain[nCycle][left][nDan];

	long nSize	= pArrRC->GetSize();
	return nSize;
}

// 좌측면에서 기둥의 x좌표 리턴
CDPoint CWallApp::GetXyCenterColumn_Left(long nColumn)
{

	CTwinVector tv;
	CDPoint xyLeft(0, 0), xyRight(0, 0);
	long nSize	= m_tvArrLeftUpper.GetSize();
	// 상부 기준 좌표가없으면 기본 값으로 그림.
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

	// 기둥식일때 기둥의 측면도를 모은다.
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

// v형 벽체 팔 높이
double CWallApp::GetHeightArm(BOOL bLeft)
{
	if(m_nType != WALL_TYPE_V) return 0;

	CString sArm = bLeft ? _T("벽체좌측팔상면") : _T("벽체우측팔상면");
	CTwinVector tvUpper	= m_tvArrFront.GetTvByInfo(sArm);
	CTwinVector tvLeft	= m_tvArrFront.GetTvByInfo("벽체좌측면");

	return fabs(tvLeft.m_v1.y-tvUpper.GetXyMid().y);
}

// v형 벽체 중심에서 팔까지 거리
double CWallApp::GetDistArm(BOOL bLeft)
{
	if(m_nType != WALL_TYPE_V) return 0;

	CString sArm = bLeft ? _T("벽체좌측팔좌측면") : _T("벽체우측팔우측면");
	CTwinVector tvArm	= m_tvArrFront.GetTvByInfo(sArm);
	
	return tvArm.GetHorLength();
}
// 벽체 주철근 철근 집계
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
	
	// 주철근
	for(cycle = 0; cycle < GetCountCycleMainRebar(); cycle++)
	{  
		sCycle	= GetCircleText(cycle+1);
	
		for(left = 0; left < 2; left++)
		{
			bLeft	= left == iLEFT ? TRUE : FALSE;
			sLeft	= bLeft ? "좌측" : "우측";
			dCover	= bLeft ? m_dCoverLeft[0] : m_dCoverRight[0];
			
			for(dan = 0; dan < MAX_REBAR_DAN; dan++)
			{
				for(nIdx = 0; nIdx < GetCountRebarInfoCycle(cycle, dan, bLeft); nIdx++)
				{
					pRC	= GetRebarInfoCycle(cycle, dan, bLeft, nIdx);
					if(!pRC) continue;
					if(!pRC->m_bExist) continue;
					GetTvRebarMain_Side(tvArr, bLeft, cycle, dan, nIdx, tvArrJoint, TRUE);

					// Round반지름이 차지 하는 길이 
					dLenRound	= 0;
					if(pRC->m_nPosStt != pRC->m_nPosEnd)
						dLenRound	= fabs(pRC->m_xyMid2.y - pRC->m_xyMid1.y);

					// 평면상에서 최대와 최소 길이를 판단해서 철근 집계함.
					dLenMax	= tvArr.GetLengthMax();
					dLenMin	= tvArr.GetLengthMin();
					nSize	= tvArr.GetSize();
					if(nSize > 0)
					{
						CRebar *pRB	= new CRebar;
						pRB->m_Fy			= m_Fy;
						pRB->m_sDescription	= pRC->m_sDescription;//.Format("중간지점 %s면 %d단 %s %d", sLeft, dan+1, sCycle, nIdx+1);
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
	
						baseConc.OptimizationRebar(pRB);	// 추가하기 전에 최적화 한번 해준다.
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

// 외측벽체 측면도 에서 2단이상의 철근을 위해서 잘라내야 되는 영역을 정의함.
// bUpper가 TRUE이면 위쪽, FALSE이면 아래쪽
// 단, 해당 부위에 철근이 없다는게 밝혀지만, null로 보내버린다.
void CWallApp::GetTvCuttingArea_Side(CTwinVectorArray &tvArr, BOOL bLeft, long nDan, BOOL bUpper)
{
	tvArr.RemoveAll();

	// 해당  부위에 철근이 있는지 파악해 본다.
	CRebarInfoCycle *pRC	= GetRebarInfoCycle(0, nDan, bLeft, bUpper ? -1 : 0);
	if(!pRC) return;
	
	// 하면일 경우 벽체 철근이 아니면 파지 않는다.
	if(!bUpper)
	{
		//if(pRC->m_nPosEnd != REBARINFO_POS_INWALL_LEFT && pRC->m_nPosEnd != REBARINFO_POS_INWALL_RIGHT) return;
	}

	CTwinVectorArray tvArrSide;
	GetTvLeftSide(tvArrSide);

	// 현재 area 모양은 (옵션에 따라 바꿀 수도 있음, 바꿀 경우 tvArr만 적당하게 바꿔주면 됨(아크와 선의 조합이면 어떤 모양이든 가능))
	// 세로로 세워진 트랙형..
	// R크기는 벽체 길이/16
	// 세로 직선 길이는 벽체 높이/2	
	CDRect rect	= tvArrSide.GetRect();
	double dDiff	= nDan == 1 ? rect.Width()/8 : rect.Width()/8*3;
	double dX	= bLeft ? rect.left + dDiff : rect.right - dDiff;
	CDPoint xyMatchU	= GetXyMatchUpperSlab(dX);
	CDPoint xyMatchL	= xyMatchU;
	CTwinVectorArray tvArrLeftFooting, tvArrHidden;
	m_footing.GetTvLeftSide(tvArrLeftFooting, tvArrHidden, FALSE);
	CTwinVector tvLower	= tvArrLeftFooting.GetTvByInfo("기초하면");
	GetXyMatchLineAndLine(xyMatchL, CDPoint(0, -1), tvLower.m_v1, tvLower.GetXyDir(), xyMatchL);
	
	double dY	= bUpper ? xyMatchU.y : xyMatchL.y;
	double dR	= rect.Width()/15;
	double dH	= fabs(xyMatchU.y - xyMatchL.y)/4;

	// 트랙형 만든다.(트랙의 우측 중심 아래에서 시작)
	CTwinVector tv;
	tv.m_v1		= CVector(dX+dR, dY-dH/2, 0);
	tv.m_v2		= tv.m_v1 + CVector(0, dH, 0);
	tv.m_sInfo	= "좌측세로";
	tvArr.Add(tv);

	tv.m_v1		= tv.m_v2;
	tv.m_v1.z	= 1;
	tv.m_v2		= tv.m_v1 + CVector(-dR*2, 0, dR);
	tv.m_sInfo	= bUpper ? "그릴때제거" : "상단아크";
	tvArr.Add(tv);

	tv.m_v1		= tv.m_v2;
	tv.m_v1.z	= 0;
	tv.m_v2		= tv.m_v1 + CVector(0, -dH, 0);
	tv.m_sInfo	= "우측세로";
	tvArr.Add(tv);

	tv.m_v1		= tv.m_v2;
	tv.m_v1.z	= 1;
	tv.m_v2		= tv.m_v1 + CVector(dR*2, 0, dR);
	tv.m_sInfo	= bUpper ? "하단아크" : "그릴때제거";
	tvArr.Add(tv);

	CRebarPlacing rb;
	rb.MoveRebar(tvArr, CDPoint(0, bUpper ? -dH/4 : dH/4));
}

// tvArr을 라운드 길이 적용해서 나눠준다.
// 라운드부를 점선으로 그리기 위해서..
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

		// 벽체는 거리나 지름이나 모두 이음 개수만큼만 받으면 된다.
		for(left = 0; left < 2; left++)
		{
			long nCountJoint	= nCycle == 0 || nCycle == 2 ? m_nCountJointMainRebar_Cycle1[left][nDan] : m_nCountJointMainRebar_Cycle2[left][nDan];

			// 이음개수가 변경된 경우에만 권고안 하도록 하는 경우 /////////
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

			// 거리/지름
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

// 벽체 배력철근 권고안.
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
		CString sLeft	= bLeft ? "좌측" : "우측";

		// 라운드 벽체인 경우 좌우측 구분없이 라운드 형태의 철근으로 집계됨
		// 따라서 우측에 대해서는 집계할 필요가 없음
		if(IsRoundRectWall() && !bLeft) continue;	
		
		long dan = 0; for(dan = 0; dan < MAX_REBAR_DAN; dan++)
		{
			long nTypeRebar	= bLeft ? W_REBAR_S_L1+dan : W_REBAR_S_R1+dan;

			dCover		= bLeft ? m_dCoverLeft[dan] : m_dCoverRight[dan];
			dRebarSpace	= GetCTCRebarSupport(bLeft, dan, TRUE);
			CPlaceRebarByBlock *pPlace	= &m_placeByBlock_SupportRebar[left][dan];
			if(pPlace->m_dDia == 0) continue;

			
			// 라운드 벽체인 경우
			if(IsRoundRectWall())
			{
				CRebar *pRB	= new CRebar;
				// 철근 개수
				CDPointArray xyArr;
				GetXyRebarSupport_Front(xyArr, TRUE, dan, FALSE, TRUE);

				CTwinVectorArray tvArrRebarPlane;
				m_Column.GetTvRebarRound_Plan(tvArrRebarPlane, dan);

				pRB->m_Fy			= m_Fy;
				pRB->m_sDescription.Format("중간지점 %d단 배력철근", dan+1);
				pRB->m_sMark		= GetMarkRebarSupport(nTypeRebar+dan, FALSE, TRUE, -1);
				pRB->m_dDia			= pPlace->m_dDia;
				pRB->m_nTypeRebar	= 51;
				pRB->m_dLengthMax_A	= tvArrRebarPlane.GetTvByInfo("기둥단면좌측면").GetLength();
				double dR			= fabs(tvArrRebarPlane.GetTvByInfo("기둥단면좌측상면라운드").m_v2.z);
				pRB->m_dLengthMax_B	= 2 * ConstPi * dR / 2;
				pRB->m_R			= dR;
				pRB->m_bMainRebar	= FALSE;
				pRB->m_nEa			= xyArr.GetSize();

				// 이음 개수 계산시 직선구간의 길이만 가지고 계산을 한뒤 * 2를 한다.
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

				// 직선 철근
				dLenMax	= tvArr.GetLengthMax();
				dLenMin	= tvArr.GetLengthMin();
				if(Compare(dLenMax, 0.0, "=")) continue;

				CRebar *pRB	= new CRebar;
				pRB->m_Fy			= m_Fy;
				pRB->m_sDescription.Format("중간지점 %s %d단 배력철근", sLeft, dan+1);
				pRB->m_sMark		= GetMarkRebarSupport(nTypeRebar+dan, FALSE, TRUE, -1);
				pRB->m_dDia			= pPlace->m_dDia;
				pRB->m_dLengthMax_A	= dLenMax;
				pRB->m_dLengthMin_A	= dLenMin;
				pRB->m_nTypeRebar	= 1;
				pRB->m_bMainRebar	= FALSE;
				pRB->m_nEa			= tvArr.GetSize();
				baseConc.OptimizationRebar(pRB);	// 철근 최적화

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

				// 꺽이는 철근
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
						pRB->m_sDescription.Format("중간지점 %s %d단 배력철근%d", sLeft, dan+1, 2+add);
						pRB->m_sMark.Format("%s-%d", sMark, 1+add);
						pRB->m_dDia			= pPlace->m_dDia;
						pRB->m_dLengthMax_A	= dLenMax;
						pRB->m_dLengthMin_A	= dLenMin;
						pRB->m_nTypeRebar	= 1;
						pRB->m_bMainRebar	= FALSE;
						pRB->m_nEa			= nCount;
						baseConc.OptimizationRebar(pRB);	// 철근 최적화

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

// 철근 초기화
// long nType 
// 0 : 주철근, 1 : 배력철근, 2 : 전단철근, 3 : 기둥철근,
void CWallApp::ClearRebarList(long nType)
{
	CHgBaseConcStd baseConc;
	if(nType == 0)
	{}
	else if(nType == 1)
	{
		// 배력 철근
		baseConc.ClearRebarJewon(&m_pArrRebarSupport, TRUE);
	}
	else if(nType == 2)
	{
		// 전단 철근
		baseConc.ClearRebarJewon(&m_pArrRebarShear, TRUE);
	}
	else if(nType == 3)
	{
		// 기둥철근
		baseConc.ClearRebarJewon(&m_pArrRebarColumn, TRUE);
	}
	
}

// 전단철근 측면도
void CWallApp::GetTvRebarShear_Side(CTwinVectorArray &tvArr, BOOL bLeftWall)
{
	tvArr.RemoveAll();
	if(m_bNotRebar) return;
	CRebarPlacing rb;

	// 접속슬래브 시작 위치는 배력근과 주철근 처음 철근간의 교차점이다.
	CTwinVectorArray tvArrJoint;
	CTwinVectorArray tvArrMain, tvArrSup, tvArrSupAngle, tvArrSupAngleAdd;
	GetTvRebarMain_Side(tvArrMain, TRUE, 0, 0, 0, tvArrJoint, FALSE);
	GetTvRebarSupport_Side(tvArrSup, tvArrSupAngle, tvArrSupAngleAdd, TRUE, 0);

	// 전단철근 배치 위치
	CTwinVectorArray tvArrShear;
	GetTvRebarShear_Front(tvArrShear);
	if(m_nType == WALL_TYPE_SLOPE)
	{
		CDPoint xyOrg	= GetXyOrgTopForFront(bLeftWall);
		double dAng		= GetXyDirRotate_Front().GetAngleDegree();
		rb.RotateTvArr(tvArrShear, xyOrg, dAng);
	}

	// 현재 배력근이나 주철근이 없으면 전단철근 배근이 안 된다.
	long nIdxSttMain	= 1;	// 주철근 몇 번째 부터 배치하기 시작할지?
	if(tvArrMain.GetSize() < nIdxSttMain+1 || tvArrSup.GetSize() == 0) return;
	
	// 접속슬래브 전체 외곽을 구해서 전단철근을 배치한 뒤에 전단철근의 두 점 모두 포함되는거만 살림.
	double dLenShear	= GetLenShear();		// 전단철근 길이
	CDPoint xyDirShear	= CDPoint(1, 0);

	// 전단철근 배치 시작 위치
	CTwinVector tvMain	= tvArrMain.GetAt(nIdxSttMain);
	CTwinVector tvSup	= tvArrSup.GetAt(0);
	CDPoint xyStt		= CDPoint(0, 0);
	GetXyMatchLineAndLine(tvMain.m_v1, tvMain.GetXyDir(), tvSup.m_v1, tvSup.GetXyDir(), xyStt);

	//가각부있으면 시작 위치가 변경..
	if(m_dWidthExp[iLEFT] > 0)
		xyStt.x -= m_dWidthExp[iLEFT];

	// 배치한다.
	CTwinVector tv;
	CTwinVector tvLimit	= tvArrMain.GetAt(tvArrMain.GetSize()-1);
	double dRange	= (tvLimit.m_v1.x - tvMain.m_v1.x);	// 전단철근 들어가는 범위
	// 가각일때는 실제로 그릴때는 원안에만 그리므로 안 보이겠지만, 가각폭만큼 더해서 구함
	// 집계를 정확하게 하기 위해
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
			tv.m_sInfo	= "전단철근";

			tvArr.Add(tv);
			tv.m_v1		= tv.m_v1 + xyDirShear * (dLenShear*2);
		}
		bAddAtFirst	= !bAddAtFirst;
	}
}

// 전단철근 길이 
double CWallApp::GetLenShear()
{
	double dLen	= 0;
	CHgBaseConcStd baseConc;

	// 표준간격/2 * 감싸는 개수
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

	// 전단철근 길이
	GetTvRebarShear_Side(tvArr, TRUE);
	long nCount	= tvArr.GetSize();
	dLenMaxA	= tvArr.GetLengthMax();
	dLenMinA	= tvArr.GetLengthMin();

	// 전단철근 높이
	GetTvRebarShear_Front(tvArr);
	dLenMaxB	= tvArr.GetLengthMax();
	dLenMinB	= tvArr.GetLengthMin();

	// 정착장
	CPlaceRebarByBlock *pPlace	= &m_placeByBlock_ShearRebar;
	double dSettle	= pPlace->m_dDia <= 10 || pPlace->m_dDia >= 35 ? 0 : m_pBasicConcInfo->GetValueJoint(m_Fck, m_Fy, (long)pPlace->m_dDia, JOINT_TENS_NORMAL_B, dCover, dRebarSpace);
	double dDiaMainUpper	= GetDiaMainRebar(0, 0, TRUE, 0);
	double dDiaMainLower	= GetDiaMainRebar(0, 0, FALSE, 0);
	
	CRebar *pRB	= new CRebar;
	pRB->m_Fy			= m_Fy;
	pRB->m_sDescription.Format("중간지점 전단철근");
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
	baseConc.OptimizationRebar(pRB);	// 철근 최적화
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

// 전단철근 잘라내는 영역 구하기
// 0 : 좌우측에 따라 반씩 자름
// 1 : 좌우측에 따라 원(중심에서 좌우로 이동한 위치에 있음)
void CWallApp::GetTvCuttingArea_Side_Shear(CTwinVectorArray &tvArr, long nDrawLeft)
{
	tvArr.RemoveAll();

	// 0 : 좌우측에 따라 반씩 자름
	// 1 : 좌우측에 따라 원(중심에서 좌우로 이동한 위치에 있음)
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
		// 원..
		CTwinVector tvCenter	= GetTvCenter_Side(TRUE, 0);
		CTwinVector tv;

		double dDivideCen	= 8.0; //기존 6.0
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

		// ARCBRIDGE-2794 (도면을 잘라서 사용할때만 사용함)
		if(m_nTypeOutputMainRebar_Side == 1)
		{
			CTwinVector tvCutLine;
			double dRCen = 0, dAngStt =0 , dAngEnd =0;
			GetInfoCuttingCircle_Side(TRUE, tvCutLine.m_v1, dRCen, dAngStt, dAngEnd);

			//커팅라인 제일 x의 값이 큰 쪽과 상하 EL 생성 해줌 
			tvCutLine.m_v1.x += dRCen;
			tvCutLine.m_v2.x += dRCen;
			tvCutLine.m_v1.y = tvCenter.m_v1.y;
			tvCutLine.m_v2.y = tvCenter.m_v2.y;

			//센터선부터 커팅선까지의 길이를 구함
			double dDistCen = tvCenter.GetXyLeft().x - tvCutLine.GetXyLeft().x;
			dR = max(dR, dDistCen);
		}		
		

		CRebarPlacing rb;
		rb.MoveRebar(tvArr, CDPoint(sw, 0)*dR);
	}
}

// 기둥 포인터 리턴
// 기둥은 여러개인데, 실제론 하나 밖에 없기 때문에 인덱스에 따라 기둥을 달리 셋팅해서 리턴해줌
CColumnApp* CWallApp::GetColumn(long nIdx)
{
	CDPoint xyLeft(0, 0), xyRight(0, 0);
	long nSize	= m_tvArrLeftUpper.GetSize();
	// 상부 기준 좌표가없으면 기본 값으로 그림.
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

// 기둥 철근 권고안
// 기둥식인 경우 다른 주철근이나 배력철근 전단철근이 없고, 기둥부철근만 있음
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

		// 철근을 가져와서 합침.
		long i = 0; for(i = 0; i < pColumn->m_pArrRebar.GetSize(); i++)
		{
			CRebar *pRB	= new CRebar;
			*pRB	= *pColumn->m_pArrRebar.GetAt(i);
			pRB->m_sMark	= GetMarkRebarColumn(0, FALSE, TRUE, -1); 
			pRB->m_sDescription.Format("지점%d %s", nJijum + 1, pRB->m_sDescription); 

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

	// 기둥이 여러개라서 순서대로 집계되지 않음
	// 같은 종류의 철근은 머지를 시킴.
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

// 벽체측면 그릴때 기초하면과 벽체의 교차점을 기준으로 해서 그리는데
// 경사형 벽체의 경우 벽체 좌측이 될 수도 있고, 우측이 될 수도 있다.
// 그래서 함수를 이용함
// 경사형 벽체일때는 벽체를 기준점을 기준으로 돌려서 똑바로 세워서 그려야 되기 때문임.
CDPoint CWallApp::GetXyOrgFootingLowerForSide(BOOL bLeft)
{
	CDPoint xyOrg		= GetXyOrgTopForFront(bLeft);
	CRebarPlacing rb;
	CTwinVector tvLower	= m_footing.m_tvArrFront.GetTvByInfo("기초하면");

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
			// 만나는 점을 못찾았다면 연장해서 맞춰보자.
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

// 벽체 이음길이 가져오는 함수
// 철근 지름이나 이음의 길이를 직접 가져와도 되지만, 만해하나라도 배열이 원하는 만큼 덜 생성 됐을 경우나
// 이상한 값이 들어간 경우를 대비해서 이 함수를 통해서 직접 이음길이를 가져 오도록 한다.
// bAccumulation : index까지의 이음길이를 축적해서 가져옴.
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

	// 필요한 RebarInfoCycle를 찾음
	long left	= bLeft ? iLEFT : iRIGHT;
	CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC = &m_pArrRCMain[nCycle][left][nDan];
	if(!pArrRC) return;

	// 정보분석
	GetJointAtRebarInfoCycleArray(arrJoint, pArrRC);
}


// RcBridgeRebar에서 복사해온 코드임 (완전같음)
// rebarinfocycle 배열에서 이음 정보를 추출한다.
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
			// 이음이 오른쪽에 그려져야 되는 경우를 판단해서 보정해준다.
			if(pRC1->m_nPosEnd == REBARINFO_POS_INWALL_LEFT)
				joint.m_bDrawUpper	= FALSE;
			else 
				joint.m_bDrawUpper	= TRUE;
			
			arrJoint.Add(joint);
		}
	}
}

// RcBridgeRebar에서 복사해온 코드임 (완전같음)
// 두 철근을 비교해서 이음이 잇는지 판단.
// 일반적으로 두 rebarinfocycle를 비교해서 이음을 찾으면 되지만, 상부슬래브 철근의 경우 선형을 
// 따라가기 때문에 일반적인 방법으로 찾기가 어렵다.
// 그래서 상부슬래브 철근만 다른 방법으로 찾는다.
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
	
	// rebarinfocycle 특성에 따라 비교해야 될 선을 뽑아서 비교한다.
	// 1. 두 철근이 모두 시작과 끝의 pos가 같은 철근일때.
	if(pRC1->m_nPosStt == pRC1->m_nPosEnd && pRC2->m_nPosStt == pRC2->m_nPosEnd)
	{
		xyArrLine1.Add(pRC1->m_xyStt);
		xyArrLine1.Add(pRC1->m_xyMid1);

		xyArrLine2.Add(pRC2->m_xyStt);
		xyArrLine2.Add(pRC2->m_xyMid1);

		// 겹쳐 있는지 판단.
		bJoint = rb.CheckOverlapLine(xyArrLine1, xyArrLine2, TRUE, TRUE);
	}
	// 2. 첫번째 철근은 시작과 끝이 같고 두번째 철근은 다를때
	else if(pRC1->m_nPosStt == pRC1->m_nPosEnd && pRC2->m_nPosStt != pRC2->m_nPosEnd)
	{
		xyArrLine1.Add(pRC1->m_xyStt);
		xyArrLine1.Add(pRC1->m_xyMid1);

		xyArrLine2.Add(pRC2->m_xyStt);
		xyArrLine2.Add(pRC2->m_xyMid1);

		// 겹쳐 있는지 판단.
		bJoint	= rb.CheckOverlapLine(xyArrLine1, xyArrLine2, TRUE, TRUE);

		// 겹치지 않았다면 한번 더 검사함.
		if(!bJoint)
		{
			xyArrLine1.Add(pRC1->m_xyStt);
			xyArrLine1.Add(pRC1->m_xyMid1);

			xyArrLine2.Add(pRC2->m_xyMid2);
			xyArrLine2.Add(pRC2->m_xyEnd);
			
			// 겹쳐 있는지 판단.
			bJoint	= rb.CheckOverlapLine(xyArrLine1, xyArrLine2, TRUE, TRUE);
		}
	}
	// 3. 첫번째 철근은 시작과 끝이 다르고 두번째 철근은 같을때.
	else if(pRC1->m_nPosStt != pRC1->m_nPosEnd && pRC2->m_nPosStt == pRC2->m_nPosEnd)
	{
		xyArrLine1.Add(pRC1->m_xyMid2);
		xyArrLine1.Add(pRC1->m_xyEnd);

		xyArrLine2.Add(pRC2->m_xyStt);
		xyArrLine2.Add(pRC2->m_xyMid1);

		// 겹쳐 있는지 판단.
		bJoint	= rb.CheckOverlapLine(xyArrLine1, xyArrLine2, TRUE, TRUE);

		// 겹치지 않았다면 한번 더 검사함.
		if(!bJoint)
		{
			xyArrLine1.RemoveAll();
			xyArrLine1.Add(pRC1->m_xyMid2);
			xyArrLine1.Add(pRC1->m_xyEnd);

			xyArrLine2.RemoveAll();
			xyArrLine2.Add(pRC2->m_xyStt);
			xyArrLine2.Add(pRC2->m_xyMid1);
			
			// 겹쳐 있는지 판단.
			bJoint	= rb.CheckOverlapLine(xyArrLine1, xyArrLine2, TRUE, TRUE);
		}
	}
	// 4. 두철근모두 시작과 끝이 다를때.
	else
	{
		xyArrLine1.Add(pRC1->m_xyStt);
		xyArrLine1.Add(pRC1->m_xyMid1);

		xyArrLine2.Add(pRC2->m_xyStt);
		xyArrLine2.Add(pRC2->m_xyMid1);

		// 겹쳐 있는지 판단.
		bJoint	= rb.CheckOverlapLine(xyArrLine1, xyArrLine2, TRUE, TRUE);

		// 겹치지 않았다면 한번 더 검사함.
		if(!bJoint)
		{
			xyArrLine1.RemoveAll();
			xyArrLine1.Add(pRC1->m_xyStt);
			xyArrLine1.Add(pRC1->m_xyMid1);

			xyArrLine2.RemoveAll();
			xyArrLine2.Add(pRC2->m_xyMid2);
			xyArrLine2.Add(pRC2->m_xyEnd);
			
			// 겹쳐 있는지 판단.
			bJoint	= rb.CheckOverlapLine(xyArrLine1, xyArrLine2, TRUE, TRUE);
		}

		// 겹치지 않았다면 한번 더 검사함.
		if(!bJoint)
		{
			xyArrLine1.RemoveAll();
			xyArrLine1.Add(pRC1->m_xyMid2);
			xyArrLine1.Add(pRC1->m_xyEnd);

			xyArrLine2.RemoveAll();
			xyArrLine2.Add(pRC2->m_xyStt);
			xyArrLine2.Add(pRC2->m_xyMid1);
			
			// 겹쳐 있는지 판단.
			bJoint	= rb.CheckOverlapLine(xyArrLine1, xyArrLine2, TRUE, TRUE);
		}

		// 겹치지 않았다면 한번 더 검사함.
		if(!bJoint)
		{
			xyArrLine1.RemoveAll();
			xyArrLine1.Add(pRC1->m_xyMid2);
			xyArrLine1.Add(pRC1->m_xyEnd);

			xyArrLine2.RemoveAll();
			xyArrLine2.Add(pRC2->m_xyMid2);
			xyArrLine2.Add(pRC2->m_xyEnd);
			
			// 겹쳐 있는지 판단.
			bJoint	= rb.CheckOverlapLine(xyArrLine1, xyArrLine2, TRUE, TRUE);
		}
	}


	// 검사가 끝났으면 이음이 있는 경우 이음의 정보를 기록함
	if(bJoint)
	{
		// 겹쳐져있는 선은 xyArrLine1에 남게 된다.
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

	CString sLeft	= bLeft ? "좌측" : "우측";
	CString sCycle	= GetCircleText(nCycle+1);
	long nIdx		= 1;

	long nSize	= pArrRC->GetSize();

	long i = 0; for(i = 0; i < nSize; i++)
	{
		CRebarInfoCycle *pRC	= pArrRC->GetAt(i);
		if(!pRC->m_bExist) continue;

		pRC->m_sDescription.Format("중간지점 %s면 %d단 %s %d", sLeft, nDan+1, sCycle, nIdx++);
	}
}

// cycle3 배근방식에 따른 이음 이동 거리 리턴
// 방식 -> 
// 0 : 수동입력
// 1 : 1cycle과 같은 위치
// 2 : 우측으로 이음길이 1/2 이동
// 3 : 좌측으로 이음길이 1/2 이동
// 4 : 우측으로 이음길이 만큼 이동
// 5 : 좌측으로 이음길이 만큼 이동
double CWallApp::GetDistMoveJointCycle13(BOOL bCycle3)
{
	if(GetCountCycleMainRebar() < 4) return 0;

	double dLenJoint	= 0;
	double dDist	= GetDistMoveJointByTypeDefaultCycle3(m_nTypeDefaultCycle3, dLenJoint);

	if(!bCycle3)
		dDist	= 0;

	return dDist;
}

// 타입에 따라서 이동 되어야 하는 거리 리턴
// 0 : 수동입력
// 1 : 1cycle과 같은 위치
// 2 : 우측으로 이음길이 1/2 이동
// 3 : 좌측으로 이음길이 1/2 이동
// 4 : 우측으로 이음길이 만큼 이동
// 5 : 좌측으로 이음길이 만큼 이동
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

// 측면도 출력시 컷팅할 정보(큰원)
// double &dAngStt, double &dAngEnd : 그릴때 아크를 그려야 되므로 각도정보도 리턴함.
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

// 라운드 벽체를 위해 기둥을 라운드 벽체식에 맞는 형식으로 셋팅해 둔다.
// 필요할때 사용하자
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

// 벽체 주철근 직선구간에 대한 간격정보 리턴
// 라운드식의 경우 직선구간에 대해서만 구함.
void CWallApp::GetOffsetInfoRebarMain_Plane_LineSection(CDPointArray &xyArrOffsetInfo, BOOL bLeft, long nDan)
{
	xyArrOffsetInfo.RemoveAll();

	// 주철근이 들어가는 범위(수직구간,라운드벽체의경우라운드부는나중에따로 구함)
	CTwinVectorArray tvArrSide;
	GetTvOffset_Side(tvArrSide, bLeft, 0);
	CTwinVector tvLeft	= tvArrSide.GetTvByInfo("벽체좌측"),
				tvRight	= tvArrSide.GetTvByInfo("벽체우측");
	if(IsRoundRectWall())
	{
		double dCover	= GetCoverFront(TRUE, 0);
		tvLeft.m_v1.x += (m_dW/2 - dCover);
		tvLeft.m_v2.x += (m_dW/2 - dCover);
		tvRight.m_v1.x -= (m_dW/2 - dCover);
		tvRight.m_v2.x -= (m_dW/2 - dCover);
	}

	// 사보강배치 일때 가각 부분까지 주철근이 배치된다. 그려줄땐 빼더라도 배치 및 치수에는 고려한다
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
	
	
	// 측면 주철근 간격 구함 ///////////////////////////////////////////////////////////////////
	double dCtc			= m_bApplyAngleToMainRebarCTC ? m_dCTC_MainRebar/sin(ToRadian(m_dAnglePath)) : m_dCTC_MainRebar;
	double dLen			= tvHor.GetHorLength();
	long nCount			= (long)((dLen-(dCtc*2)) / dCtc);
	double dRemain		= (dLen-(dCtc*2)) - (dCtc * nCount);

	xyArrOffsetInfo.Add(CDPoint(1, dRemain/2 + dCtc));
	xyArrOffsetInfo.Add(CDPoint(nCount, dCtc));
	xyArrOffsetInfo.Add(CDPoint(1, dRemain/2 + dCtc));
}

// 라운드 벽체 지원 가능 한지?
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

// 첫번째나 마지막 기둥의 끝에서 기초끝까지 거리 리턴
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
	// 2단부터는 다른 변수에 저장되어 있다
	// 2단이상은 그냥 내측 외측으로 지름이 구분되어 있음.
	// 2단이 index 0이고 3단이 1이다.
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

// 벽체 하부 EL
double CWallApp::GetELBottomOfWall()
{
	if(m_footing.IsVirtualFooting()) return m_dELB - m_dELBDiff;

	// 입력받은 EL에 기초 높이를 감안해서 그려준다.
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


// 벽체 2,3단 철근 확장길이 적용
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


		// 이전 철근과의 이음 검사
		if(!GetJointAtRebarInfoCycle(pRCPrev, pRC, joint))
		{
			if(pRC->m_nPosStt == pRC->m_nPosEnd)
			{
				CDPoint xyDir	= (pRC->m_xyMid1 - pRC->m_xyStt).Unit();
				pRC->m_xyStt	= pRC->m_xyStt + (xyDir * -dLengthExt23DanRebar);
			}
		}

		// 다음 철근과의 이음 검사
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

		// 철근 체크후 마지막 철근은 상부슬래브 피복을 뚫고 나가는지 검사해야 된다.
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
// 	CTwinVector tvLower	= tvArrSide.GetTvByInfo("벽체하면");
// 	CTwinVector tvCenter;
// 	
// 	tvCenter.m_v1	= CVector(m_dCenterOfWall, rect.bottom, 0);
// 	tvCenter.m_v2	= CVector(m_dCenterOfWall, rect.top, 0);
// 
// 	// 확장길이 적용.
// 	tvCenter.m_v1	= tvCenter.m_v1 + (tvCenter.GetXyDir(TRUE) * dExtLen);
// 	tvCenter.m_v2	= tvCenter.m_v2 + (tvCenter.GetXyDir() * dExtLen);
// 
// 	return tvCenter;
// }

// 배력근 배근할때 시작위치
// 전단철근도 해당됨.
// 하면 높이가 다를 경우 낮은 쪽을 기준으로 함
CDPoint CWallApp::GetXySttRebarSupport_Front(long nDan, BOOL bLeft, BOOL bVertDir)
{
	CDPoint xyStt	= GetTvOffset_Front(0, 0, TRUE, TRUE, bLeft, bVertDir).m_v1;

	CRebarInfoCycle *pRC	= GetRebarInfoCycle(0, nDan, bLeft, 0);
	if(!pRC) return xyStt;
	CRebarInfoCycle *pRC1	= GetRebarInfoCycle(0, 0, bLeft, 0);
	//CRebarInfoCycle *pRC2	= GetRebarInfoCycle(0, 0, !bLeft, 0);
	//double dYMin	= pRC2 ? min(pRC1->m_xyMid2.y, pRC2->m_xyMid2.y) : pRC1->m_xyMid2.y;
	// 원래 좌우측중 낮은 곳을 기준으로 했으나, 시종점 벽체는 실제 위치에서 부터 배근을 하는 바람에
	// 시종점 벽체를 CWallApp로 만들어서 사용할 경우 오류가 발생함
	// 따라서 중간지점 벽체도 실제 위치에서 배근하도록 함.
	if(m_bRebarLowerSettle[bLeft? iLEFT : iRIGHT]) // m_footing.m_bIs || m_footing.IsVirtualFooting())
	{
		// Box 타입일때는 Round가 끝나는 위치에 배력철근이 들어가야하기 때문에 철근 최하단 부터 간격으로 배치 하지만
		// 일반 기초 에서는 Round부분에 철근이 들어가지 않기 때문에 그 위치가 배근 시작위치가 된다.
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

// 옵션에 따른 벽체의 상부 기준점
// 상부슬래브에 교차되는 위치를 리턴함
// 구조도나 일반도 그릴때 이 점을 기준으로 수직으로 세운다.
CDPoint CWallApp::GetXyOrgTopForFront(BOOL bLeft)
{
	// 기준이 되는 위치 선정
	CTwinVector tvWall	= m_tvArrFront.GetTvByInfo(bLeft ? "벽체좌측면" : "벽체우측면");
	CDPoint xyOrg	= m_nTypeRebarFront == 0 ? (tvWall.m_v2) : m_xyOrg;

	CRebarPlacing rb;
	CDPointArray xyArrMatch;
	rb.GetXyMatchTvArrAndLine(m_tvArrUpperLimit_Front, xyOrg, tvWall.GetXyDir(), xyArrMatch);
	if(xyArrMatch.GetSize() > 0)
		xyOrg	= xyArrMatch.GetAt(0);

	return xyOrg;
}

// 벽체를 수직으로 세우기 위해 회전해야 되는 각도
CDPoint CWallApp::GetXyDirRotate_Front()
{
	if(m_nType == WALL_TYPE_VER) return CDPoint(1, 0);
	CTwinVector tvWall	= m_tvArrFront.GetTvByInfo("벽체좌측면");

	return ToDPointFrDegree(90 - tvWall.GetXyDir().GetAngleDegree());
}

// 기초 배력근 권고안
// 배력근 권고안시 벽체를 고려해야 되므로 여기에서 권고안 해줌
void CWallApp::SetDataDefaultRebarSupportFront_Footing(BOOL bUpper)
{
	long upper			= bUpper ? iUPPER : iLOWER;
	CPlaceRebarByBlock *pPlace	= &m_footing.m_placeByBlock_Support_Front[upper][0];
	pPlace->m_xyArr.RemoveAll();
	if(!m_footing.m_bIs) return;

	// 기초 배근은 무조건 직거리로 한다
	CTwinVectorArray tvArrFrontOld;
	tvArrFrontOld	= m_footing.m_tvArrFront;
	m_footing.GetTvFront(FALSE);

	CHgBaseConcStd baseConc;
	double dCTC			= m_footing.m_dCTC_SupportRebar;

	// (ARCBRIDGE-1659) 주철근 선형방향 배치일때 각도 고려 해준다.
	
	HGBOOL bApplyAngle = (m_footing.IsVertDir() == FALSE);
	double dAngApply	= m_footing.m_dAngle;

	// 기초 배력근 간격 ///////////////////////////////////////////////////////////////////
	CTwinVectorArray tvArrRebar;
	double dCtcUpper	= dCTC;		// 상면은 표준간격
	double dCtcLower	= dCTC/2;	// 하면은 표준간격/2
	

	// 인장압축구간 동일 적용일경우 상면 하면 모두 dCTC로 설정하고 구간을 하나로 한다.
	if(m_footing.m_bSameOffsetPlaceTAndCBlock)
	{
		dCtcUpper		= dCTC;
		dCtcLower		= dCTC;
	}

	// 상면은 권고안시 벽체를 고려 한다.
	// 경사형 기초는 그냥 함.
	if(bUpper && (m_footing.m_nType == FOOTING_TYPE_BASIC || m_footing.m_nType == FOOTING_TYPE_BASIC_BOTH ||
		m_footing.m_nType == FOOTING_TYPE_BASIC_LEFTSLOPE || m_footing.m_nType == FOOTING_TYPE_BASIC_RIGHTSLOPE))
	{
		// 상면 구할때는 하면의 좌측짜투리 우측짜투리를 알아야 된다.
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
			// 하면일때는 nCount가 상면일때의 2배가 되는지 체크해야 된다.
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
		// 하면일때는 nCount가 상면일때의 2배가 되는지 체크해야 된다.
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

// 전단철근 권고안.
// 상면 배력철근 1단을 가지고 배근한다.
void CWallApp::SetDataDefaultRebarShearFront_Footing(BOOL bFront)
{
	CPlaceRebarByBlock *pPlace	= bFront ? &m_footing.m_placeByBlock_Shear_Front : &m_footing.m_placeByBlock_Shear_Side;
	pPlace->m_xyArr.RemoveAll();
	pPlace->m_nCountBlock	= 0;

	// 하부배력철근 1단을 구함.
	CDPoint xy(0, 0);
	CDPointArray xyArr;
	if(bFront)
	{
		m_footing.GetXyRebarSupport_Front(xyArr, TRUE, 0);

		// 벽체 주철근에 가장 가까운 두개는 뺀다.	
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

				// (ARCBRIDGE-2781) 벽체에 붙가까운걸로 좌우 하나씩만 빼면 되는데 이건 뭐지??? 끝단에서 벽체까지 에서 중간에서 빼고 있다.
				// 구분이 있는 경우 뒷굽은 1개씩 빼되 반만 뺀다.
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


	// 배력철근 첫번째는 무조건 추가하고
	// 정면은 두번째, 측면은 세번째 철근부터 warp개수마다 하나씩 보관하고
	// 마지막 철근은 항상 뺌.
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

	// 배력근의 2개이상마다 전단철근이 들어가야 된다면 여기서 뺀다. ///////////////////
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

	// 필요한것만 골라낸 배력철근 간격을 분석해서 전단철근 간격으로 치환한다.
	CRebarPlacing rb;
	rb.ConvertPointArrayToOffsetInfo(xyArrSelect, pPlace->m_xyArr, 0);
	pPlace->m_nCountBlock	= pPlace->m_xyArr.GetSize();
}

// 벽체가 수직으로 서 있는지?
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

// 수평에 대해서 벽체가 세워져 있는 각도(상부슬래브 고려한 각도임)
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
	CTwinVector tvWall	= m_tvArrFront.GetTvByInfo(bLeft ? "벽체좌측면" : "벽체우측면");
	CTwinVector tvLowerOfWall		= m_tvArrFront.GetTvByInfo("벽체하면우측");
	CTwinVector tvLowerOfFooting	= m_footing.m_tvArrFront.GetTvByInfo("기초하면");
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
	m_footingByRotateToVert.m_tvPathLeft	= tvArrLeftSide.GetTvByInfo("벽체하면");
	GetTvLeftSide(tvArrLeftSide, FALSE, FALSE, FALSE, FALSE, 0, TRUE, TRUE, TRUE, FALSE);
	m_footingByRotateToVert.m_tvPathRight	= tvArrLeftSide.GetTvByInfo("벽체하면");

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
	CTwinVector tvLeft	= m_tvArrFront.GetTvByInfo("벽체좌측면");
	CTwinVector tvRight	= m_tvArrFront.GetTvByInfo("벽체우측면");
	CDPoint xyMatch(0, 0);

	// 벽체가 Round 인경우 GetxyDir을 쓰면 방향이 잘못 나온다.
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

	if(m_tvArrFront.GetTvByInfo("벽체하면좌측", tv))
	{
		if(m_tvArrLowerLimit_Front.GetSize() > 0)
		{
			tv.m_vecOverlap1	= tv.m_v1;
			tv.m_vecOverlap2	= tv.m_v2;
			m_tvArrFront.SetTvByInfo("벽체하면좌측", tv);
		}
		else
		{
			if(m_footing.m_tvArrFront.GetTvByInfo("기초상면좌측마진", tmp))
			{
				tv.m_vecOverlap1 = tmp.m_v1;
				tv.m_vecOverlap2 = tmp.m_v2;

				tmp.m_vecOverlap1 = tv.m_v1;
				tmp.m_vecOverlap2 = tv.m_v2;

				m_tvArrFront.SetTvByInfo("벽체하면좌측", tv);
				m_footing.m_tvArrFront.SetTvByInfo("기초상면좌측마진", tmp);
			}
			else if(m_footing.m_tvArrFront.GetTvByInfo("기초경사좌측마진", tmp))
			{
				tv.m_vecOverlap1 = tmp.m_v1;
				tv.m_vecOverlap2 = tmp.m_v2;

				tmp.m_vecOverlap1 = tv.m_v1;
				tmp.m_vecOverlap2 = tv.m_v2;

				m_tvArrFront.SetTvByInfo("벽체하면좌측", tv);
				m_footing.m_tvArrFront.SetTvByInfo("기초경사좌측마진", tmp);
			}
		}
	}

	if(m_tvArrFront.GetTvByInfo("벽체하면우측", tv))
	{
		if(m_tvArrLowerLimit_Front.GetSize() > 0)
		{
			tv.m_vecOverlap1	= tv.m_v1;
			tv.m_vecOverlap2	= tv.m_v2;
			m_tvArrFront.SetTvByInfo("벽체하면우측", tv);
		}
		else
		{
			if(m_footing.m_tvArrFront.GetTvByInfo("기초상면우측마진", tmp))
			{
				tv.m_vecOverlap1 = tmp.m_v1;
				tv.m_vecOverlap2 = tmp.m_v2;

				tmp.m_vecOverlap1 = tv.m_v1;
				tmp.m_vecOverlap2 = tv.m_v2;

				m_tvArrFront.SetTvByInfo("벽체하면우측", tv);
				m_footing.m_tvArrFront.SetTvByInfo("기초상면우측마진", tmp);
			}
			else if(m_footing.m_tvArrFront.GetTvByInfo("기초경사우측마진", tmp))
			{
				tv.m_vecOverlap1 = tmp.m_v1;
				tv.m_vecOverlap2 = tmp.m_v2;

				tmp.m_vecOverlap1 = tv.m_v1;
				tmp.m_vecOverlap2 = tv.m_v2;

				m_tvArrFront.SetTvByInfo("벽체하면우측", tv);
				m_footing.m_tvArrFront.SetTvByInfo("기초경사우측마진", tmp);
			}
		}
	}
}

void CWallApp::SyncWall_DesignData()
{
	// 옛날에는 기초와 벽체가 같이 쓰였다고 함 지금은 아니라서..
	//m_footing.m_Fck	= m_Fck;
	//m_footing.m_Fy	= m_Fy;
	//m_footing.m_FyShear	= m_FyShear;
	//m_footing.m_dLengthRebarMax	= m_dLengthRebarMax;

	// 기둥
	m_Column.m_Fck	= m_Fck;
	m_Column.m_Fy	= m_Fy;
	m_Column.m_FyShear	= m_FyShear;
	m_Column.m_dLengthRebarMax	= m_dLengthRebarMax;
}

// 상부슬래브 처럼 꺽을 배력근 범위 권고안
// 상부슬래브 피복 철근중 가장 낮은 부분을 기준으로 함.
// 따라서 m_dRangeSupportRebarForSameUpperSlab이 0이어도 꺾여야될 철근은 다 꺾임
void CWallApp::SetDataDefaultRangeSupportRebarForSameUpperSlab()
{
	m_dRangeSupportRebarForSameUpperSlab	= 0;
	m_placeByBlock_SupportRebarAddSameUpperSlab.m_xyArr.RemoveAll();
	m_placeByBlock_SupportRebarAddSameUpperSlab.m_xyArr.Add(CDPoint(0, 0));
	m_placeByBlock_SupportRebarAddSameUpperSlab.m_nCountBlock	= 1;
	long left = 0; for(left = 0; left < 2; left++)
	{
		m_dDistAddSupportRebarForSameUpperSlab[left]	= 0;	// 상부슬래브 처럼 꺽을 추가되는 배력근의 좌우측 거리
	}
}

double CWallApp::GetLength()
{
	CTwinVector tvLeft	= m_tvArrFront.GetTvByInfo("벽체좌측면");
	CTwinVector tvRight	= m_tvArrFront.GetTvByInfo("벽체우측면");

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
		// 전체 배근 중에서 가장 큰 직경으로 적용하자.
		 long nSize = m_dArrDiaRC[nCycle][bLeft? iLEFT:iRIGHT][nDan].GetSize();

		for(long ix=0; ix<nSize; ++ix)
		{
			double dDiaCur = m_dArrDiaRC[nCycle][bLeft? iLEFT:iRIGHT][nDan].GetAt(ix);

			if(dDiaCur > dDia)
				dDia = dDiaCur;
		}
	}
	double dCover = bLeft? m_dCoverLeft[nDan] : m_dCoverRight[nDan];

	pSLsdOption->dFy = GetValueUnitNmm(m_Fy, _T("kgf/cm²"));
	pSLsdOption->dFck = GetValueUnitNmm(m_Fck, _T("kgf/cm²"));
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
		// 2Cycle 철근이 없다면 
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

		// R 구간에 있는 철근은 하나 걸러 하나 빼고 직선구간에 있는 철근은 모두다 빼고 사이에 하나씩만 넣어준다.
		CTwinVectorArray tvArr;
		m_Column.GetTvPlan(FALSE);
		tvArr = m_Column.m_tvArrPlan[iOUTTER][iUPPER];
		CTwinVector tvLeft = tvArr.GetTvByInfo(_T("기둥단면좌측면"));
		CTwinVector tvRight = tvArr.GetTvByInfo(_T("기둥단면우측면"));
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
				// 윗쪽 라운드부
				xyArrUp.Add(xyPos);
			}
			else if(Compare(xyPos.y, dBottom, _T("<"), 1.0))
			{
				// 아랫쪽 라운드부
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
