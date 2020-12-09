// RcBridgeApp.cpp: implementation of the CRcBridgeApp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "arcbridgedata.h"
#include "RcBridgeApp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CRcBridgeApp::CRcBridgeApp()
{
}

CRcBridgeApp::~CRcBridgeApp()
{
}

// BOOL CRcBridgeApp::MarkRecommandRebarJewon(long nRebarPart)
// {
// 
// 	return TRUE;
// }

// 해당 sta에서의 사각 리턴
// 사용처 : 수량(거푸집) 구할때 벽체별로 적용해야 되는 사각이 다르기 때문임.
CDPoint CRcBridgeApp::GetAngleByStation(double dSta)
{
	CDPoint xyDir	= CDPoint(0, 1);
	long nJ			= GetJijumNumberByStation(dSta);
	if(nJ != -1)
	{
		xyDir	= GetAngleJijum(nJ);
		CLineInfo *pLine	= GetLineBase();

		double dStaJijum	= GetStationOnJijum(nJ);
		CDPoint vAngAz		= GetAngleAzimuthJijum(nJ);
		CDPoint vAngAzPer	= vAngAz.Rotate90();
		CDPoint xyCenJijum		= pLine->GetXyLine(dStaJijum);
		CDPoint xyCen			= pLine->GetXyLine(dSta);

		double dW	= dSta - dStaJijum;
		CDPoint xyMatch	= CDPoint(0, 0);
		if(GetXyMatchLineAndLine(xyCenJijum, vAngAzPer, xyCen, vAngAz, xyMatch))
		{
			dW	= ~(xyMatch - xyCenJijum);
			if(dW < 0)
				dW *= -1;
		}


		xyDir		= pLine->GetAngleByOffsetWidth(dStaJijum, dW, xyDir);
	}
	
	return xyDir;
}

BOOL CRcBridgeApp::IsBriInLine()
{
	CLineInfo* pLine = GetLineBase();

	double dBp = pLine->m_dStationBp;
	double dEp = pLine->GetStationEpLine();
	double dBriStt = m_dStationBridgeStt;
	double dBriEnd = dBriStt + m_dLengthBridge;

	if(dBriStt<dBp || dBriStt>dEp)
	{
		AfxMessageBox("교량의 시점이 선형 상에 존재하지 않습니다. 교량의 시점 Station을 다시 입력해 주십시오.  ");
		return FALSE;
	}
	if(dBriEnd<dBp || dBriEnd>dEp)
	{
		AfxMessageBox("교량의 종점이 선형 상에 존재하지 않습니다. 교량의 시점 Station이나 교량의 길이를 다시 입력해 주십시오.  ");
		return FALSE;
	}

	return TRUE;
}

// bLeft는 V형 벽체에서만 사용.
// 1. 벽체에 해당하는 헌치를 찾고
// 2. 헌치의 높이를 적용한 벽체의 높이를 찾고
// 3. V형의 경우 벽체의 왼쪽 오른쪽팔의 좌표를 찾아서
// 바로 리턴.
CDPoint CRcBridgeApp::GetXyInWallHunchBottom(long nIdxWall, BOOL bLeft, long nLeft, BOOL bVertDir, BOOL bUpper)
{
	if(nIdxWall >= GetCountInWall()) 
	{
		if(m_nQtyJigan-1 > nIdxWall)
		{
			KeepInWallCount();
		}
		else
			return CDPoint(0, 0);
	}
	CWallApp *pWall = GetInWall(nIdxWall);
	CDPoint xy(0, 0);

	// 벽체의 Org를 설정한뒤
	SetOrgInWall(nIdxWall, nLeft, bVertDir);

	// V형일때는 각 팔의 EL로 계산하고.
	if(pWall->m_nType == WALL_TYPE_V)
	{
		CDPoint xyMatch(0, 0);
		CDPoint xyDirLeft(0, 0), xyDirRight(0, 0);
		xyDirLeft = ToDPointFrRadian(ToRadian(90+pWall->m_dAngleV/2));
		xyDirRight = ToDPointFrRadian(ToRadian(90-pWall->m_dAngleV/2));

		if(bLeft)
		{
			// 벽체 왼쪽팔의 EL 구하기
			xy = GetXyMatchLineAndSlabeLowerToDist(-GetHeightHunch(GetIdxHunchInWall(nIdxWall, bLeft)), CDPoint(pWall->m_xyOrg.x, pWall->m_xyOrg.y-pWall->m_dH), xyDirLeft, -1, nLeft);
		}
		else
		{
			// 벽체 오른쪽 팔의 EL 구하기
			xy= GetXyMatchLineAndSlabeLowerToDist(-GetHeightHunch(GetIdxHunchInWall(nIdxWall, bLeft)), CDPoint(pWall->m_xyOrg.x, pWall->m_xyOrg.y-pWall->m_dH), xyDirRight, -1, nLeft);
		}
		
	}
	else	// 일반형일때는 벽체의 Org를 그대로 사용하면 됨
	{
		xy = pWall->m_xyOrg;

		if(bUpper == FALSE)
		{
			SyncBridgeBoxType(TRUE);
			if(m_nTypeJongdanForRebar != 0)
				xy.y = pWall->m_dELB;
			else
			{
				long nIdxHunch = GetIdxHunchInWall(nIdxWall,FALSE);
				xy.y = m_dELSB + m_dTS_Lower + m_dHHL[nIdxHunch][iLOWER] + m_dHBH[nIdxHunch][iLOWER];
			}
		}
	}

	return xy;
}

// 종단면도 좌표구함.
// 시점좌표 기준
// 박스교일때를 제외하고는 기초에 대해서는 따로 구하지 않음.
// 외냐하면 기초를 따로 분리 시켰기 때문임.
// long nLeft 기준선형 : -1 왼쪽, 0 중심, 1 오른쪽 
// bWingFront : 날개벽 정면도 그릴 때에만 쓰임. 2004.12.9 Goh 추가
// bApplyAngle : TRUE 사거리, FALSE 직거리로 그림.
// bHunchInclude : TRUE 좌우측 내측헌치 구함
// 직거리로 그릴때는 직방향 선형을 이용한다.
// 중요 : 좌우측 선형을 기준으로 그릴때는 직거리로 그리는거 지원 안함(필요도 없고 그릴 수도 없음)
// 좌우측 선형 기준으로 그릴때는 그냥 직선으로 그린다.(실제 종단선형과 다르게 나오기 때문)
// ***  이 함수는 너무 중요한 함수임 ***
// 현재 좌우측 선형으로 그릴때는 중간벽체가 이상하게 나옴. (실제 좌우측 선형으로 그릴때 중간벽체 필요 없으므로 일단 놔두지만, 바로 수정할 것임)
// 헌치길이를 구할때 외측벽체 헌치와 내측벽체 헌지길이 구하는 방법이 다르다.
// 외측벽체는 평면선형의 영향을 받고 내측벽체는 종단선형의 영향을 받는다.
BOOL CRcBridgeApp::GetTvVertSection(long nLeft, BOOL bApplyAngle, BOOL bWingFront, long nTypeJongdan, BOOL bHunchInclude)
{
	double dTSOld	= m_dTS;
	m_dTS	= m_dTSOld;

	m_bApplyAngleJongdan = bApplyAngle;

	m_nLeftCurVertSection	= nLeft;
	// 가끔씩 이 값이 엉뚱하게 들어간 경우가 잇어서 강제로 조정해줌.
	if(m_nTypeJongdanForRebar < 0 || m_nTypeJongdanForRebar > 2)
		m_nTypeJongdanForRebar	= 0;

	if(nTypeJongdan < 0 || nTypeJongdan > 2)
		nTypeJongdan	= m_nTypeJongdanForRebar;

	// 신축이음은 무조건 구해 놓음
	GetTvVertSectionFxJoint(TRUE, !bApplyAngle);
	GetTvVertSectionFxJoint(FALSE, !bApplyAngle);

	CTwinVector tv;

	m_tvArrVertSection.RemoveAll();
	m_tvArrBriSect.RemoveAll();
	m_tvArrVertSectionRib[iUPPER].RemoveAll();
	m_tvArrVertSectionRib[iLOWER].RemoveAll();
	m_tvArrVertSectionRibCen.RemoveAll();

	CDPoint xyOrgStt(0, 0), xyOrgEnd(0, 0);

	// 종단선형따라 그릴지? 직선으로 그릴지?
	BOOL bDrawJongdanLine	= (nLeft == 0) && nTypeJongdan == 2;

 	if(m_bSettingVertSection)
	{
		// 날개벽각도로 그리지 않고, 종단좌표가 셋팅되어 있다면 셋팅된거를 그대로 사용한다. ////////////
		long wing	= bWingFront ? 1 : 0;
		if(nLeft == -1)
		{
			if(bApplyAngle)
				m_tvArrVertSection	= m_tvArrVertSection_Ang_Left[nTypeJongdan][wing];
			else
				m_tvArrVertSection	= m_tvArrVertSection_Vert_Left[nTypeJongdan];
		}
		else if(nLeft == 0)
		{
			if(bApplyAngle)
				m_tvArrVertSection	= m_tvArrVertSection_Ang_Center[nTypeJongdan][wing];
			else
				m_tvArrVertSection	= m_tvArrVertSection_Vert_Center[nTypeJongdan];
		}
		else
		{
			if(bApplyAngle)
				m_tvArrVertSection	= m_tvArrVertSection_Ang_Right[nTypeJongdan][wing];
			else
				m_tvArrVertSection	= m_tvArrVertSection_Vert_Right[nTypeJongdan];
		}

		CTwinVector tv;
		CString str	= _T("");
		CStringArray sArr;
		sArr.Add("슬래브상면(외측벽체안쪽)");
		sArr.Add("종점벽체하부헌치");
		sArr.Add("종점벽체좌측면");
		sArr.Add("종점벽체헌치");
		str.Format("%d번째내측헌치우측슬래브하면", m_nCountInWallHunch);
		sArr.Add(str);
		sArr.Add("슬래브하면");
		sArr.Add("시점벽체헌치");
		sArr.Add("시점벽체우측면");
		sArr.Add("시점벽체하부헌치");
		if(m_nTypeBridge == BRIDGE_TYPE_BOX)
		{
			sArr.Add("하부슬래브상면");
			sArr.Add("하부슬래브하면(외측벽체안쪽)");

			sArr.Add("하부슬래브하면(시점벽체아래)");
			sArr.Add("시점부상방지저판하단");
			sArr.Add("시점부상방지저판바깥측면");
			sArr.Add("시점부상방지저판상면");

			sArr.Add("하부슬래브하면(종점벽체아래)");
			sArr.Add("종점부상방지저판하단");
			sArr.Add("종점부상방지저판바깥측면");
			sArr.Add("종점부상방지저판상면");
		}

		CTwinVectorArray tvArrTmp;
		tvArrTmp	= m_tvArrVertSection;
		tvArrTmp.GetTvArrByInfo(sArr, m_tvArrBriSect, TRUE);

		tv	= m_tvArrVertSection.GetTvByInfo("시점벽체하면");
		xyOrgStt	= tv.GetXyRight();
		if(m_nTypeBridge == BRIDGE_TYPE_BOX)
		{
			tv	= m_tvArrVertSection.GetTvByInfo("하부슬래브하면(시점벽체아래)");
			xyOrgStt	= tv.GetXyRight();
		}
		tv	= m_tvArrVertSection.GetTvByInfo("시점벽체우측면");
		xyOrgStt.x	= tv.m_v1.x;

		tv	= m_tvArrVertSection.GetTvByInfo("종점벽체하면");
		if(m_nTypeBridge == BRIDGE_TYPE_BOX)
			tv	= m_tvArrVertSection.GetTvByInfo("하부슬래브하면(종점벽체아래)");
		xyOrgEnd	= tv.GetXyLeft();
		if(m_nTypeBridge == BRIDGE_TYPE_BOX)
		{
			tv	= m_tvArrVertSection.GetTvByInfo("하부슬래브하면(종점벽체아래)");
			xyOrgEnd	= tv.GetXyLeft();
		}
		tv	= m_tvArrVertSection.GetTvByInfo("종점벽체좌측면");
		xyOrgEnd.x	= tv.m_v1.x;

		// 벽체 위치 셋팅
		for(long wall = 0; wall < GetCountInWall(); wall++)
		{
			SetOrgInWall(wall, nLeft, !bApplyAngle);
		}

		if(m_nTypeBridge == BRIDGE_TYPE_BOX)
			SyncBridgeBoxType(bApplyAngle, nTypeJongdan);
		////////////////
		////////////////////////////////////////////////////////////////////////////////////
	}
	else
	{
		SyncBridgeBoxType(bApplyAngle, nTypeJongdan);
		InitSaveOrgInWall();
		BOOL bLeftWing = nLeft==-1;
		double dAngStt = GetAngleWingWallBetweenWall(TRUE, !bLeftWing).GetAngleDegree();
		double dAngEnd = GetAngleWingWallBetweenWall(FALSE, bLeftWing).GetAngleDegree();

		// 날개벽이 측면부착일때는 Elevation만 사용하므로 실각도를 적용하지 않아도 된다.
		CWingWall *pWing = GetWingWall(TRUE, !bLeftWing);
		if(pWing)
		{
			if(pWing->m_nAttachPos == 1 && bWingFront) dAngStt	= 90;
		}

		pWing = GetWingWall(FALSE, bLeftWing);
		if(pWing)
		{
			if(pWing->m_nAttachPos == 1 && bWingFront) dAngEnd	= 90;
		}

		// 날개벽 각도가 0도이거나 180도이면 단면 생성이 불가능하므로 90도로
		if(Compare(dAngStt, 180.0, "=") || Compare(dAngStt, 0.0, "="))
		{
			dAngStt	= 90;
		}
		if(Compare(dAngEnd, 180.0, "=") || Compare(dAngEnd, 0.0, "="))
		{
			dAngEnd	= 90;
		}

		m_tvArrVertSection.m_sInfo = "교량";
		BOOL bVertDir	= !bApplyAngle;

		// 횡단면 입력에서 이미 정해 지는 값들 //////////////////////
		// 슬래브 두께.
		//	m_dBTHS	= m_dTS;
		//	m_dBTHE	= m_dTS;

		// 헌치 높이(2004.09.29 : 시종점,내측벽체헌치 모두 횡단면에서 입력받는 값으로 고정
		// 단, 신축이음부의 헌치의 경우 횡단면에서 입력받은 값으로 고정하지 않는다.
		// 헌치값 모두 따로 받는것으로 변경(2004.04.11) - 헌치부 R 입력과 관련해서 변경됨  /////////
		/*
		m_dHHE	= m_dHHS;
		long i = 0; for(i = 0; i < GetCountInWall(); i++)
		{
			if(!m_bExpJoint[i+1])
			{
				long nIdxHunch	= GetIdxHunchInWall(i, TRUE);
				if(nIdxHunch > -1)
					m_dHH[nIdxHunch] = m_dHHS;
				nIdxHunch		= GetIdxHunchInWall(i, FALSE);
				if(nIdxHunch > -1)
					m_dHH[nIdxHunch] = m_dHHS;
			}
		}
		*/
		/////////////////////////////////////////////////////////////


		// 교량 좌표 구할때마다 박스교일때는 시점측과 종점측의 el과 기초높이를 같게 해준다.
		if(m_nTypeBridge == BRIDGE_TYPE_BOX)
		{
//			m_dELEB = m_dELSB;
//			m_dFHE	= m_dFHS;
		}

		// 브라켓 상부 너비를 헌치너비와 항상 일치시킴 //////////////////////////////
		// 현재 상부너비와 헌치너비를 다르게 입력하는건 지원하지 않을 예정이기때문
		// 2005.03.28
		m_dBWS	= m_dBHWS;
		m_dBWE	= m_dBHWE;
		/////////////////////////////////////////////////////////////////////////////

		// 경사교가 아닐 경우 벽체 헌치 없음 . BOX형에도 하부 헌치 있음.
		if(m_nTypeBridge != BRIDGE_TYPE_TRAP && m_nTypeBridge != BRIDGE_TYPE_TRAP2 && m_nTypeBridge != BRIDGE_TYPE_ARCH&& m_nTypeBridge != BRIDGE_TYPE_BOX)
		{
			m_dWSHH	= 0;
			m_dWSHW	= 0;
			m_dWEHH	= 0;
			m_dWEHW = 0;
		}
			
		// 벽체 하면 EL에 기초 높이 적용
		double dELSB	= m_dELSB + m_footingStt.GetHeight();
		double dELEB	= m_dELEB + m_footingEnd.GetHeight() - GetDiffELSttAndJijumForVertSection(m_nQtyJigan, FALSE);
		if(m_nTypeBridge == BRIDGE_TYPE_BOX)
		{
			dELSB = m_dELSB;
			if(bVertDir || bWingFront)
			{
				// 교량 길이가 달라지기 때문에 하단 EL도 미세하게 차이가 나네...
				double dLengthBridge	= fabs(GetXySectionSttEnd(FALSE, nLeft, bVertDir).x-GetXySectionSttEnd(TRUE, nLeft, bVertDir).x);
				dELEB	= GetxyLowerSlabBottomByLenFrSttStation(dLengthBridge + m_dWE / tv.GetVDir().y, nLeft, bVertDir).y;
			}
			else
			{
				dELEB = GetXyLowerSlabBottomByOffsetFrStation(GetStationOnJijum(m_nQtyJigan), m_dWE, nLeft).y;
			}
		}

		if(nLeft == 0)
		{
			double dStaStt = GetStationOnJijum(0);
			double dStaEnd = GetStationOnJijum(m_nQtyJigan);
			double dStaWallOutStt = GetXySlabUpperByOffsetFrStation(dStaStt, -m_dWS, nLeft).x;
			double dStaWallOutEnd = GetXySlabUpperByOffsetFrStation(dStaEnd, m_dWE, nLeft).x;

			m_dWOutWallByLine[iSTT] = dStaStt - dStaWallOutStt;
			m_dWOutWallByLine[iEND] = dStaWallOutEnd - dStaEnd;
//			m_dWBracketByLine[iSTT] = ;
//			m_dWBracketByLine[iEND] = ;
		}

		// 시점부터 오른쪽으로 
		tv.m_v1 = GetXySectionSttEnd(TRUE, nLeft, bVertDir);

		if(IsBlockOut(TRUE))
		{
			double dW	= m_fxJointStt.m_dWidth;
			if(!bVertDir)
				dW /= GetAngleJijum(0).y;
			
			//#0030653 종구배가 있는경우 CTWinVector의 각도에 상관없이 x축만 이동하게되어 슬래브 두께가 다르게 적용되는 오류 발생
			if(m_nTypeBridge == BRIDGE_TYPE_SLAB)
			{
				tv.m_v1.x += dW;
			}
			else
			{
				CRebarPlacing rb;

				CTwinVector tvTemp = tv;
				tvTemp.m_v2 = GetXySectionSttEnd(FALSE, nLeft, bVertDir);
				

				CDPointArray xyArrMatch;
				CTwinVectorArray tvArr;

				tvArr.Add(tvTemp);
				rb.GetXyMatchTvArrAndLine(tvArr, CDPoint(tvTemp.m_v1.x + dW, tvTemp.m_v1.y), CDPoint(0, 1), xyArrMatch);

				if(xyArrMatch.GetSize() > 0)
				{
					tv.m_v1.x = xyArrMatch.GetAt(0).x;
					tv.m_v1.y = xyArrMatch.GetAt(0).y;
				}
				else
				{
					tv.m_v1.x += dW;
				}
			}
		}
		// 슬래브일때는 신축이음 고려해서 슬래브 상면을 그려준다.
		// 신축이음을 고려해서 간격을 넓히는건 마지막에 한다.
		if(m_nTypeBridge == BRIDGE_TYPE_SLAB)
		{
			for(long nJijum = 0; nJijum < m_nQtyJigan; nJijum++)
			{
				if(nJijum > 0) tv.m_v1 = GetXyInWallHunchBottom(nJijum-1, TRUE, nLeft, bVertDir);
				if(nJijum < m_nQtyJigan-1) tv.m_v2 = GetXyInWallHunchBottom(nJijum, TRUE, nLeft, bVertDir);
				else tv.m_v2 = GetXySectionSttEnd(FALSE, nLeft, bVertDir);

				// 신축이음 고려(시점 종점일땐 고려하지 않음 - GetXySectionSttEnd함수 자체가 시종점의 유간을 고려하기 때문임)
				CJijum* pJL = NULL;
				CJijum* pJR = NULL;
				pJL = GetJijum(nJijum);
				pJR = GetJijum(nJijum+1);
				if(m_bExpJoint[nJijum] && nJijum > 0) tv.m_v1.x += pJL->m_dLengthUgan[1];
				if(m_bExpJoint[nJijum+1] && nJijum < m_nQtyJigan-1) tv.m_v2.x -= pJR->m_dLengthUgan[0];
				
				tv.m_bApplyAngle	= bApplyAngle;
				tv.m_dAngle			= GetAngleJijum(nJijum, bVertDir).GetAngleDegree();
				tv.m_v1.y			= GetXySlabUpperByLenFrSttStation(tv.m_v1.x-GetXySectionSttEnd(TRUE, nLeft, bVertDir).x, nLeft, bVertDir).y;
				tv.m_v2.y			= GetXySlabUpperByLenFrSttStation(tv.m_v2.x-GetXySectionSttEnd(TRUE, nLeft, bVertDir).x, nLeft, bVertDir).y;
				tv.m_v1.z			= bDrawJongdanLine ? 5 : 0;
				tv.m_sInfo.Format("슬래브상면(%d번째지간)", nJijum+1);
				tv.val.valHor		= NULL;
				tv.val.valVer		= NULL;
				m_tvArrVertSection.Add(tv);
			}
		}
		else
		{
			tv.m_bApplyAngle	= bApplyAngle;
			tv.m_dAngle			= 90;
			tv.m_v2				= GetXySectionSttEnd(FALSE, nLeft, bVertDir);
			tv.m_v1.z			= bDrawJongdanLine ? 5 : 0;
			tv.m_sInfo			= "슬래브상면(외측벽체안쪽)";
			tv.val.valHor		= NULL;
			tv.val.valVer		= NULL;
			m_tvArrVertSection.Add(tv);
			m_tvArrBriSect.Add(tv);
		}
		
		if(IsOutWall(FALSE))
		{
			//InitSaveXySlabUpperByLenFrSttStation();
			double dLengthBridge	= fabs(GetXySectionSttEnd(FALSE, nLeft, bVertDir).x-GetXySectionSttEnd(TRUE, nLeft, bVertDir).x);
			if(bWingFront)
				dLengthBridge	= m_dLengthBridge;
			tv.m_bApplyAngle	= bApplyAngle;
			tv.m_dAngle			= bApplyAngle ? (bWingFront ? dAngEnd : GetAngleJijum(m_nQtyJigan, bVertDir).GetAngleDegree()) : 90;
			tv.m_v1				= tv.m_v2;
			if(bVertDir || bWingFront)
			{
				tv.m_v2			= GetXySlabUpperByLenFrSttStation(dLengthBridge+(m_dWE+m_dBHWE-m_dBWE)/tv.GetVDir().y, nLeft, bVertDir);
			}
			else
			{
				tv.m_v2			= GetXySlabUpperByOffsetFrStation(GetStationOnJijum(m_nQtyJigan), m_dWE+m_dBHWE-m_dBWE, nLeft);
				tv.m_dAngle		= GetAngleJijumByStaToSta(tv.m_v1.x, tv.m_v2.x, m_dWE+m_dBHWE-m_dBWE, nLeft);
			}
			tv.m_v1.z			= bDrawJongdanLine ? 5 : 0;
			tv.m_sInfo			= "슬래브상면(종점벽체위)";
			tv.val.valHor		= NULL;
			tv.val.valVer		= NULL;
			m_tvArrVertSection.Add(tv);

			tv.m_bApplyAngle	= bApplyAngle;
			tv.m_dAngle			= 90;
			tv.m_v1				= tv.m_v2;
			tv.m_v2.y			-= m_dBTHE;
			tv.m_sInfo			= "종점브라켓상부측면";
			tv.val.valHor		= &m_dBWE;
			tv.val.valVer		= NULL;
			tv.val.bRevMoveHor	= TRUE;
			m_tvArrVertSection.Add(tv);


			tv.m_bApplyAngle	= bApplyAngle;
			tv.m_dAngle			= bApplyAngle ? (bWingFront ? dAngEnd : GetAngleJijum(m_nQtyJigan, bVertDir).GetAngleDegree()) : 90;
			tv.m_v1				= tv.m_v2;
			if(bVertDir || bWingFront)
				tv.m_v2.x		+= m_dBWE/tv.GetVDir().y;
			else
			{
				tv.m_v2.x		= GetXySlabUpperByOffsetFrStation(GetStationOnJijum(m_nQtyJigan), m_dWE+m_dBHWE, nLeft).x;
				tv.m_dAngle		= GetAngleJijumByStaToSta(tv.m_v1.x, tv.m_v2.x, m_dBWE, nLeft);
			}
			tv.m_sInfo			= "종점브라켓상면";
			tv.val.valHor		= NULL;
			tv.val.valVer		= &m_dBTHE;
			tv.val.bRevMoveVer	= TRUE;
			m_tvArrVertSection.Add(tv);

			tv.m_bApplyAngle	= bApplyAngle;
			tv.m_dAngle			= 90;
			tv.m_v1				= tv.m_v2;
			tv.m_v2.y			-= (m_dBHE-m_dBHHE);
			tv.m_sInfo			= "종점브라켓측면";
			tv.val.valHor		= &m_dBHWE;
			tv.val.valVer		= &m_dBHE;
			tv.val.bRevMoveHor	= FALSE;
			tv.val.bRevMoveVer	= TRUE;
			m_tvArrVertSection.Add(tv);

			tv.m_bApplyAngle	= bApplyAngle;
			tv.m_dAngle			= bApplyAngle ? (bWingFront ? dAngEnd : GetAngleJijum(m_nQtyJigan, bVertDir).GetAngleDegree()) : 90;
			tv.m_v1				= tv.m_v2;
			if(bVertDir || bWingFront)
				tv.m_v2.x		-= m_dBHWE/tv.GetVDir().y; 
			else
			{
				tv.m_v2.x		= GetXySlabUpperByOffsetFrStation(GetStationOnJijum(m_nQtyJigan), m_dWE, nLeft).x;
				tv.m_dAngle		= GetAngleJijumByStaToSta(tv.m_v1.x, tv.m_v2.x, -m_dBHWE, nLeft);
			}
			tv.m_v2.y			-= m_dBHHE;
			tv.m_sInfo			= "종점브라켓헌치";
			tv.val.valHor		= &m_dBHWE;
			tv.val.valVer		= &m_dBHHE;
			tv.val.bRevMoveHor	= FALSE;
			tv.val.bRevMoveVer	= FALSE;
			m_tvArrVertSection.Add(tv);

			tv.m_bApplyAngle	= bApplyAngle;
			tv.m_dAngle			= 90;
			tv.m_v1				= tv.m_v2;
			tv.m_v2.y			= dELEB;
			if(m_nTypeBridge == BRIDGE_TYPE_BOX && m_bExistProtectionRiseFooting)
				tv.m_v2.y += (m_dPRF_H1 + m_dPRF_H2);
			tv.m_sInfo			= "종점벽체우측면";
			tv.val.valHor		= &m_dWE;
			tv.val.valVer		= NULL;
			tv.val.bRevMoveHor	= FALSE;
			m_tvArrVertSection.Add(tv);

			if(m_nTypeBridge != BRIDGE_TYPE_BOX)
			{
				tv.m_bApplyAngle	= bApplyAngle;
				tv.m_dAngle			= bApplyAngle ? (bWingFront ? dAngEnd : GetAngleJijum(m_nQtyJigan, bVertDir).GetAngleDegree()) : 90;
				tv.m_v1				= tv.m_v2;
				double dW			= m_nTypeBridge == BRIDGE_TYPE_TRAP2 || m_nTypeBridge == BRIDGE_TYPE_ARCH ? (m_dWE + m_dWEHW) : m_dWE;
				if(bVertDir || bWingFront)
					tv.m_v2.x		-= dW/tv.GetVDir().y;
				else
				{
					tv.m_v2.x		= GetXySlabUpperByOffsetFrStation(GetStationOnJijum(m_nQtyJigan), -m_dWEHW, nLeft).x;
					tv.m_dAngle		= GetAngleJijumByStaToSta(tv.m_v1.x, tv.m_v2.x, -dW, nLeft);
				}
				tv.m_sInfo			= "종점벽체하면";
				tv.val.valHor		= NULL;
				tv.val.valVer		= &m_dELEB;
				tv.val.bRevMoveVer	= FALSE;
				m_tvArrVertSection.Add(tv);		
			}
			else
			{
// 				if(bWingFront && bApplyAngle)
// 				{
// 					tv.m_bApplyAngle	= bApplyAngle;
// 					tv.m_dAngle			= dAngEnd;
// 					tv.m_v1				= tv.m_v2;
// 					double dW			= (m_dWE + m_dWEHW);
// 					if(bVertDir || bWingFront)
// 						tv.m_v2.x		-= dW/tv.GetVDir().y;
// 				}
// 				else
				{
					// Box 하단에 종단경사 적용하면 높이을 계산해 줘야한다.
					if(bVertDir || bWingFront)
					{
						tv.m_v2		= GetxyLowerSlabBottomByLenFrSttStation(dLengthBridge - m_dWEHW/tv.GetVDir().y, nLeft, bVertDir);
					}
					else
					{
						tv.m_v2		= GetXyLowerSlabBottomByOffsetFrStation(GetStationOnJijum(m_nQtyJigan), -m_dWEHW, nLeft);
					}
					tv.m_v2.y += m_dTS_Lower;
				}
			}

			xyOrgEnd	= tv.GetXyLeft();

			if(m_nTypeBridge == BRIDGE_TYPE_TRAP2 || m_nTypeBridge == BRIDGE_TYPE_ARCH || m_nTypeBridge == BRIDGE_TYPE_BOX)
			{
				tv.m_bApplyAngle	= bApplyAngle;
				tv.m_dAngle			= bApplyAngle ? (bWingFront ? dAngEnd : GetAngleJijum(m_nQtyJigan, bVertDir).GetAngleDegree()) : 90;
				xyOrgEnd.x += m_dWEHW/tv.GetVDir().y;
				tv.m_v1				= tv.m_v2;

				if(m_nTypeBridge == BRIDGE_TYPE_BOX)
				{
					if(bVertDir || bWingFront)
					{
						tv.m_v2		= GetxyLowerSlabBottomByLenFrSttStation(dLengthBridge, nLeft, bVertDir);
					}
					else
					{
						tv.m_v2 = GetXyLowerSlabBottomByOffsetFrStation(GetStationOnJijum(m_nQtyJigan), 0, nLeft);
					}
					tv.m_v2.y += (m_dTS_Lower + m_dWEHH);
				}
				else
				{
					if(bVertDir || bWingFront)
						tv.m_v2.x		+= m_dWEHW/tv.GetVDir().y;
					else
					{
						tv.m_v2.x		= GetXySlabUpperByOffsetFrStation(GetStationOnJijum(m_nQtyJigan), 0, nLeft).x;
						tv.m_dAngle		= GetAngleJijumByStaToSta(tv.m_v1.x, tv.m_v2.x, m_dWEHW, nLeft);
					}
					tv.m_v2.y			+= m_dWEHH;
				}
				
				tv.m_sInfo			= "종점벽체하부헌치";
				tv.val.valHor		= &m_dWEHW;
				tv.val.valVer		= &m_dWEHH;
				tv.val.bRevMoveHor	= TRUE;
				tv.val.bRevMoveVer	= FALSE;
				m_tvArrVertSection.Add(tv);
				m_tvArrBriSect.Add(tv);
			}

			tv.m_bApplyAngle	= bApplyAngle;
			tv.m_dAngle			= 90;
			tv.m_v1				= tv.m_v2;
			if(bVertDir)
				tv.m_v2			= GetXyMatchLineAndSlabUpper(tv.m_v1, CDPoint(0, 1), nLeft, -1, bVertDir);
			else
				tv.m_v2.y		= GetXySlabUpperByOffsetFrStation(GetStationOnJijum(m_nQtyJigan), 0, nLeft).y - m_dTS;
			tv.m_v2.x			= tv.m_v1.x;
			tv.m_v2.y			-= m_dHHE;
			tv.m_sInfo			= "종점벽체좌측면";
			tv.val.valHor		= NULL;
			tv.val.valVer		= NULL;
			m_tvArrVertSection.Add(tv);
			m_tvArrBriSect.Add(tv);

			tv.m_bApplyAngle	= bApplyAngle;
			tv.m_dAngle			= bApplyAngle ? (bWingFront ? dAngEnd : GetAngleJijum(m_nQtyJigan, bVertDir).GetAngleDegree()) : 90;
			tv.m_v1				= tv.m_v2;
			if(bVertDir || bWingFront)
			{
				tv.m_v2			= GetXyMatchLineAndSlabUpper(tv.m_v1, CDPoint(0, 1), nLeft, -1, bVertDir);
				tv.m_v2			= GetXySlabUpperByLenFrSttStation(dLengthBridge-m_dHWE/tv.GetVDir().y, nLeft, bVertDir);// (tv.m_v2.x, -m_dHWE/tv.GetVDir().y, nLeft, -1, bVertDir, FALSE);
				tv.m_v2.y		-= m_dTS;
			}
			else
			{
				// 왜 내측헌치와 외측벽체 헌지길이 구하는 방법을 다른걸 썻을까...
				// GetXyToDistFromSlabUpper() 함수는 종단경사에 영향을 받고
				// GetXySlabUpperByOffsetFrStation() 함수는 평면선형에 형향을받 
				//tv.m_v2			= GetXyToDistFromSlabUpper(GetStationOnJijum(m_nQtyJigan), -m_dHWE/tv.GetVDir().y, nLeft, -1, bVertDir, FALSE);
				tv.m_v2			= GetXySlabUpperByOffsetFrStation(GetStationOnJijum(m_nQtyJigan), -m_dHWE, nLeft);
				tv.m_v2.y		-= m_dTS;
				tv.m_dAngle		= GetAngleJijumByStaToSta(tv.m_v1.x, tv.m_v2.x, -m_dHWE, nLeft);
			}
			if(m_dHRE > 0 && m_nTypeRoundOfHunchEnd == 1)
			{
				tv.m_v1.z	= -1;
				tv.m_v2.z	= m_dHRE;
			}
			tv.m_sInfo			= "종점벽체헌치";
			tv.val.valHor		= &m_dHWE;
			tv.val.valVer		= &m_dHHE;
			tv.val.bRevMoveHor	= TRUE;
			tv.val.bRevMoveVer	= TRUE;
			m_tvArrVertSection.Add(tv);
			m_tvArrBriSect.Add(tv);
			tv.m_v1.z			= 0;
			tv.m_v2.z			= 0;
		}
		else
		{
			tv.m_bApplyAngle	= bApplyAngle;
			tv.m_dAngle			= 90;
			tv.m_v1				= tv.m_v2;
			CDPoint xy			= GetXyMatchLineAndSlabUpper(CDPoint(tv.m_v1.x-m_dWE, tv.m_v1.y), CDPoint(0, 1), nLeft, (m_dTS+m_dHHE), bVertDir);
			tv.m_v2.y			= xy.y;
			tv.m_sInfo			= "종점슬래브측면";
			tv.val.valHor		= NULL;
			tv.val.valVer		= NULL;
			m_tvArrVertSection.Add(tv);

			// 종점 슬래브 측면 구하고나서 마지막 선에 신축이음 적용
			if(IsBlockOut(FALSE))
			{
				CTwinVector tmp	= m_tvArrVertSection.GetAt(m_tvArrVertSection.GetSize()-2);
				double dW	= m_fxJointEnd.m_dWidth;
				if(!bVertDir)
					dW /= GetAngleJijum(m_nQtyJigan).y;
				
				//#0030653 종구배가 있는경우 CTWinVector의 각도에 상관없이 x축만 이동하게되어 슬래브 두께가 다르게 적용되는 오류 발생
				CRebarPlacing rb;

				CDPointArray xyArrMatch;
				CTwinVectorArray tvArr;

				CTwinVector tv;

				//z값에 따라 CRebarPlacing::GetXyMatchTvArrAndLine가 정상작동하지 않아 CDPoint값만 복사하여 교차점 검색
				tv.m_v1 = (CDPoint)tmp.m_v1;
				tv.m_v2 = (CDPoint)tmp.m_v2;

				tvArr.Add(tv);
				rb.GetXyMatchTvArrAndLine(tvArr, CDPoint(tmp.m_v2.x - dW, tmp.m_v2.y), CDPoint(0, 1), xyArrMatch);

				if(xyArrMatch.GetSize() > 0)
				{
					tmp.m_v2.x = xyArrMatch.GetAt(0).x;
					tmp.m_v2.y = xyArrMatch.GetAt(0).y;
				}
				else
				{
					tmp.m_v2.x -= dW;
				}
				
				m_tvArrVertSection.SetAt(m_tvArrVertSection.GetSize()-2, tmp);
			}

			tv.m_bApplyAngle	= bApplyAngle;
			tv.m_dAngle			= bApplyAngle ? GetAngleJijum(m_nQtyJigan, bVertDir).GetAngleDegree() : 90;
			tv.m_v1				= tv.m_v2;
			tv.m_v2				= GetXyMatchLineAndSlabUpper(CDPoint(tv.m_v1.x-m_dWE/tv.GetVDir().y, tv.m_v1.y), CDPoint(0, 1), nLeft, (m_dTS+m_dHHE), bVertDir);
			tv.m_sInfo			= "종점헌치하면";
			tv.val.valHor		= NULL;
			tv.val.valVer		= &m_dHHE;
			tv.val.bRevMoveVer	= TRUE;
			m_tvArrVertSection.Add(tv);

			tv.m_bApplyAngle	= bApplyAngle;
			tv.m_dAngle			= bApplyAngle ? GetAngleJijum(m_nQtyJigan, bVertDir).GetAngleDegree() : 90;
			tv.m_v1				= tv.m_v2;
			tv.m_v2				= GetXyMatchLineAndSlabUpper(tv.m_v1, CDPoint(0, 1), nLeft, -1, bVertDir);
			tv.m_v2				= GetXyToDistFromSlabUpper(tv.m_v2.x, -m_dHWE/tv.GetVDir().y, nLeft, -1, bVertDir, FALSE);

			//tv.m_v2				= GetXyMatchLineAndSlabUpper(CDPoint(tv.m_v1.x-m_dHWE, tv.m_v1.y), CDPoint(0, 1), nLeft, m_dTS, bVertDir);
			if(m_dHRE > 0 && m_nTypeRoundOfHunchEnd == 1)
			{
				tv.m_v1.z	= -1;
				tv.m_v2.z	= m_dHRE;
			}
			tv.m_sInfo			= "종점벽체헌치";
			tv.val.valHor		= &m_dHWE;
			tv.val.valVer		= &m_dHHE;
			tv.val.bRevMoveHor	= TRUE;
			tv.val.bRevMoveVer	= TRUE;
			m_tvArrVertSection.Add(tv);
			tv.m_v1.z			= 0;
			tv.m_v2.z			= 0;
		}

		// 좌우측 선형 기준으로 그릴때는 내측헌치를 그리지 않는다 (필요 없기 때문에) -> # ARCBRIDGE-3444 상부슬래브 하면 거푸집 수량 시 내측헌치 필요
		if(nLeft == 0 || (bHunchInclude && (nLeft == -1 || nLeft == +1)))
		{
			// 내부 헌치 제원들
 			long nIdxInWall = 0;
			BOOL bLeft = TRUE;
			
			// 슬래브일때만 신축이음 적용
			CJijum* pJ = NULL;
			double dUganLeft = 0;	// 현재 슬래브의 왼쪽 유간
			double dUganRight =	0;		// 현재 슬래브의 오른쪽 유간

			long i = 0; for(i = m_nCountInWallHunch-1; i >= 0; i--)
			{
				// 경사교에서는 수직으로 내려오는 헌치 없음 /////
				if(m_nTypeBridge == BRIDGE_TYPE_TRAP || m_nTypeBridge == BRIDGE_TYPE_TRAP2 || m_nTypeBridge == BRIDGE_TYPE_ARCH)
					m_dHBH[i][iUPPER]	= 0;
				/////////////////////////////////////////////////

				// 헌치 높이 좌우 동일하게 함.
				m_dHHR[i][iUPPER]	= m_dHHL[i][iUPPER];
		
				if(m_nTypeBridge == BRIDGE_TYPE_SLAB)
				{
					pJ = GetJijum(i+1);
					if(m_bExpJoint[i+1]) 
					{
						dUganLeft = pJ->m_dLengthUgan[0];
						dUganRight = pJ->m_dLengthUgan[1];
					}
					else
					{
						dUganLeft = 0;
						dUganRight = 0;
					}
				}

				// Round가 있는 경우 WL이나 WR은 적용 되지 않음
				CWallApp *pWall	= GetInWall(i);
				CDPoint vAngWall		= ToDPointFrDegree(pWall->GetAngleWallByHor(bApplyAngle));
				BOOL bVertWall	= pWall->IsVertWall();
				if(pWall && m_nTypeBridge != BRIDGE_TYPE_SLAB)
				{
					
					if(m_dHLR[i][iUPPER] > 0 || !bVertWall || m_nTypeBridge == BRIDGE_TYPE_ARCH)
						m_dWL[i][iUPPER]	= pWall->m_dW/2/vAngWall.y;
					if(m_dHRR[i][iUPPER] > 0 || !bVertWall || m_nTypeBridge == BRIDGE_TYPE_ARCH)
						m_dWR[i][iUPPER]	= pWall->m_dW/2/vAngWall.y;
				}

				// 헌치의 정보를 가져옴
				GetInfoHunch(i, nIdxInWall, bLeft);
				CDPoint xyHunchBottom(0, 0);
				double dAng = GetAngleJijum(i+1, bVertDir).GetAngleDegree();
				if(bWingFront)
				{
					if(i==m_nCountInWallHunch-1)
						dAng = dAngEnd==0 ? 90 : dAngEnd;
					if(i==0)
						dAng = dAngStt==0 ? 90 : dAngStt;
				}

				tv.m_bApplyAngle	= bApplyAngle;
				// bApplyAngle 가 FALS면 bVertDir이 TRUE가 되기 때문에 지점에서 선형에 의한 각도가 생길수 있기때문에 dAng을 무조건 줘야한다.
				tv.m_dAngle			= bApplyAngle ? dAng : 90;
				tv.m_v1				= tv.m_v2;
				tv.m_v1.z			= bDrawJongdanLine ? 5 : 0;
				
				CDPoint xyHunchBottomC	= GetXyInWallHunchBottom(nIdxInWall, bLeft, nLeft, bVertDir);
				CDPoint xyHunchBottomL(0, 0), xyHunchBottomR(0, 0);
				CDPoint vAngHunchLower	= GetVAngHunchLower(i, nLeft, bVertDir);
				GetXyHunchLowerSide(i, nLeft, bVertDir, xyHunchBottomL, xyHunchBottomR);
				
				double dHDiffL		= xyHunchBottomL.y - xyHunchBottomC.y;
				double dHDiffR		= xyHunchBottomR.y - xyHunchBottomC.y;
				xyHunchBottom		= CDPoint(xyHunchBottomR.x, xyHunchBottomR.y);
				//xyHunchBottom.y		-= m_dHBH[i];

				if(!bVertWall)
				{
					dHDiffL	= 0;
					dHDiffR	= 0;
				}
				
//				tv.m_v2				= GetXyMatchLineAndSlabUpper(xyHunchBottom, vAngWall, nLeft, -1, bVertDir);
				tv.m_v2				= GetXyToDistFromSlabUpper(xyHunchBottom.x, m_dHRW[i][iUPPER]/tv.GetVDir().y, nLeft, -1, bVertDir, FALSE);

				tv.m_sInfo.Format("%d번째내측헌치우측슬래브하면", i+1);
				tv.val.valHor		= NULL;
				tv.val.valVer		= NULL;
				tv.val.bRevMoveVer	= TRUE;
				m_tvArrVertSection.Add(tv);
				if(i==m_nCountInWallHunch-1)
					m_tvArrBriSect.Add(tv);


				tv.m_bApplyAngle	= bApplyAngle;
				tv.m_dAngle			= bApplyAngle ? dAng : 90;
				tv.m_v1				= tv.m_v2;
				tv.m_v2				= xyHunchBottom;
				tv.m_v2.y			+= (-dHDiffR + dHDiffL < 0) ? (-dHDiffR + dHDiffL) : 0;
				
				if(m_dHRR[i][iUPPER] > 0)
				{
					tv.m_v1.z		= -1;
					tv.m_v2.z		= m_dHRR[i][iUPPER];
				}

				//31720
				//아치교일경우에 접합부라운드 처리시 상부슬래브가 편경사인 경우 양쪽의 접합부 라운드를 상단까지 이어지게 하기 위해서 설정함
				if(m_nTypeBridge == BRIDGE_TYPE_ARCH && pWall->m_bSideIsArc)
				{
					double dLen = 10; //10으로 둔 이유는 헌치 길이가 설마 10 이하이지는 않겠지..
					double dLenHunchVert = tv.GetVerLength();
					double dLenHunchHori = tv.GetHorLength();

					if(dLenHunchVert > dLen && dLenHunchHori < dLen)
						tv.m_v2 = tv.m_v1;
				}
				//}

				tv.m_sInfo.Format("%d번째내측헌치우측면", i+1);
				tv.val.valHor		= &m_dHRW[i][iUPPER];
				tv.val.valVer		= &m_dHHL[i][iUPPER];
				tv.val.bRevMoveHor	= FALSE;
				tv.val.bRevMoveVer	= TRUE;
				m_tvArrVertSection.Add(tv);
				tv.m_v1.z			= 0;
				tv.m_v2.z			= 0;
				//Issue Number:26848
				/*if(bVertWall)
				{
					tv.m_v1	= tv.m_v2;
					tv.m_v2.y	= tv.m_v2.y + ((-dHDiffR + dHDiffL) < 0 ? (-dHDiffR + dHDiffL) : 0);
					tv.m_sInfo.Format("%d번째내측헌치우측보정면", i+1);
					tv.val.valHor	= NULL;
					tv.val.valVer	= NULL;
					m_tvArrVertSection.Add(tv);
				}
				*/

				tv.m_bApplyAngle	= FALSE;
				tv.m_dAngle			= 90;
				tv.m_v1				= tv.m_v2;
				tv.m_sInfo.Format("%d번째내측헌치우측면우측상부", i+1);
				tv.val.valHor		= NULL;
				tv.val.valVer		= NULL;
				m_tvArrVertSection.Add(tv);

				tv.m_bApplyAngle	= bApplyAngle;
				tv.m_dAngle			= 90;
				tv.m_v1				= tv.m_v2;
				tv.m_v2.y			-= m_dHBH[i][iUPPER];
				tv.m_sInfo.Format("%d번째내측헌치아래우측면", i+1);
				tv.val.valHor		= &m_dWR[i][iUPPER];
				tv.val.valVer		= NULL;
				tv.val.bRevMoveHor	= FALSE;
				m_tvArrVertSection.Add(tv);

				CVector oldVec;	// 신축이음 적용으로 틀려진 다음 위치 보관
				tv.m_bApplyAngle	= bApplyAngle;
				tv.m_dAngle			= bApplyAngle ? dAng : 90;
				tv.m_v1				= tv.m_v2;
				GetXyMatchLineAndLine(CDPoint(tv.m_v2.x-m_dWR[i][iUPPER]/tv.GetVDir().y, tv.m_v2.y), vAngWall, tv.m_v2, vAngHunchLower, tv.m_v2);

				//tv.m_v2				= tv.m_v2 + (vAngHunchLower * m_dWR[i]/tv.GetVDir().y);
				//tv.m_v2.x			-= (m_dWR[i]/tv.GetVDir().y);
				oldVec				= tv.m_v2;
				tv.m_v2.x			+= dUganRight/tv.GetVDir().y;	// 신축이음 적용
				if(bVertWall)
				{
					tv.m_v1.y		= min(min(xyHunchBottomC.y, xyHunchBottomL.y), xyHunchBottomR.y);
					tv.m_v2.y		= min(min(xyHunchBottomC.y, xyHunchBottomL.y), xyHunchBottomR.y);
				}
				tv.m_sInfo.Format("%d번째내측헌치아래면우측", i+1);
 				tv.val.valHor		= NULL;
				tv.val.valVer		= &m_dHBH[i][iUPPER];
				tv.val.bRevMoveVer	= TRUE;
				m_tvArrVertSection.Add(tv);
				
				// 신축이음에 의해서 세로로 생기는 선.
				if(m_bExpJoint[i+1])
				{
					tv.m_bApplyAngle	= bApplyAngle;
					tv.m_dAngle			= 90;
					tv.m_v1			= tv.m_v2;
					tv.m_v2			= GetXyMatchLineAndSlabUpper(tv.m_v2, CDPoint(0, 1), nLeft, 0, bVertDir);
					tv.val.valHor	= NULL;
					tv.val.valVer	= NULL;
					tv.m_sInfo.Format("%d번째내측헌치신축이음우측", i+1);
					m_tvArrVertSection.Add(tv);

					tv.m_bApplyAngle	= bApplyAngle;
					tv.m_dAngle			= bApplyAngle ? dAng : 90;
					tv.m_v1				= tv.m_v2;
					tv.m_v1.x			-= (dUganLeft+dUganRight)/tv.GetVDir().y;
					tv.m_v1				= GetXyMatchLineAndSlabUpper(tv.m_v1, CDPoint(0, 1), nLeft, 0, bVertDir);
					tv.m_v2				= oldVec;
					tv.m_v2.x			-= dUganLeft/tv.GetVDir().y;
					tv.val.valHor		= NULL;
					tv.val.valVer		= NULL;
					tv.m_sInfo.Format("%d번째내측헌치신축이음좌측", i+1);
					m_tvArrVertSection.Add(tv);
				}

				tv.m_bApplyAngle	= bApplyAngle;
				tv.m_dAngle			= bApplyAngle ? dAng : 90;
				tv.m_v1				= oldVec;
				tv.m_v2				= oldVec;
				GetXyMatchLineAndLine(CDPoint(tv.m_v2.x-m_dWL[i][iUPPER]/tv.GetVDir().y, tv.m_v2.y), vAngWall, tv.m_v2, vAngHunchLower, tv.m_v2);
				//tv.m_v2				= tv.m_v2 + (vAngHunchLower * m_dWL[i]/tv.GetVDir().y);
				//tv.m_v2.x			-= (m_dWL[i])/tv.GetVDir().y;
				tv.m_v1.x			-= dUganLeft/tv.GetVDir().y;	// 신축이음 적용
				if(bVertWall)
				{
					tv.m_v1.y		= min(min(xyHunchBottomC.y, xyHunchBottomL.y), xyHunchBottomR.y);
					tv.m_v2.y		= min(min(xyHunchBottomC.y, xyHunchBottomL.y), xyHunchBottomR.y);
				}
				tv.m_sInfo.Format("%d번째내측헌치아래면좌측", i+1);
				tv.val.valHor		= NULL;
				tv.val.valVer		= &m_dHBH[i][iUPPER];
				tv.val.bRevMoveVer	= TRUE;
				m_tvArrVertSection.Add(tv);

				tv.m_bApplyAngle	= bApplyAngle;
				tv.m_dAngle			= 90;
				tv.m_v1				= tv.m_v2;
				tv.m_v2.y			+= m_dHBH[i][iUPPER];
				tv.m_sInfo.Format("%d번째내측헌치아래좌측면", i+1);
				tv.val.valHor		= &m_dWL[i][iUPPER];
				tv.val.valVer		= NULL;
				tv.val.bRevMoveHor	= TRUE;
				m_tvArrVertSection.Add(tv);

				//Issue Number:26848
				// 좌측 보정..
// 				if(bVertWall)
// 				{
// 					tv.m_v1				= tv.m_v2;
// 					tv.m_v2				= GetXyToDistFromSlabUpper(tv.m_v1.x, 0, nLeft, m_dTS+m_dHHL[i], bVertDir, TRUE);
// 					tv.m_sInfo.Format("%d번째내측헌치좌측보정면", i+1);
// 					tv.val.valHor		= NULL;
// 					tv.val.valVer		= NULL;
// 					m_tvArrVertSection.Add(tv);
// 				}


				tv.m_bApplyAngle	= FALSE;
				tv.m_dAngle			= 90;
				tv.m_v1				= tv.m_v2;
				/*
				if(m_dHHL[i] < m_dHHR[i])
					tv.m_v2.y			+= (m_dHHR[i] - m_dHHL[i]);
					*/
				tv.m_sInfo.Format("%d번째내측헌치좌측면좌측상부", i+1);
				tv.val.valHor		= NULL;
				tv.val.valVer		= NULL;
				m_tvArrVertSection.Add(tv);

				tv.m_bApplyAngle	= bApplyAngle;
				tv.m_dAngle			= bApplyAngle ? dAng : 90;
				tv.m_v1				= tv.m_v2;
				CDPoint xyTemp(0,0), xyTemp2(0,0);
				xyTemp = GetXyMatchLineAndSlabUpper(tv.m_v1, vAngWall, nLeft, -1, bVertDir); 
				xyTemp = GetXyToDistFromSlabUpper(tv.m_v2.x, -m_dHLW[i][iUPPER]/tv.GetVDir().y, nLeft, -1, bVertDir, FALSE); 
				tv.m_v2				= GetXyMatchLineAndSlabUpper(tv.m_v1, vAngWall, nLeft, -1, bVertDir);

				tv.m_v2				= GetXyToDistFromSlabUpper(tv.m_v2.x, -m_dHLW[i][iUPPER]/tv.GetVDir().y, nLeft, -1, bVertDir, FALSE);
				if(m_dHLR[i][iUPPER] > 0)
				{
					tv.m_v1.z		= -1;
					tv.m_v2.z		= m_dHLR[i][iUPPER];
				}

				// (40269) 31720번 이슈에서 우측면만 처리하고 좌측면은 처리를 안했음 추가.
				//아치교일경우에 접합부라운드 처리시 상부슬래브가 편경사인 경우 양쪽의 접합부 라운드를 상단까지 이어지게 하기 위해서 설정함
				if(m_nTypeBridge == BRIDGE_TYPE_ARCH && pWall->m_bSideIsArc)
				{
					double dLen = 10; //10으로 둔 이유는 헌치 길이가 설마 10 이하이지는 않겠지..
					double dLenHunchVert = tv.GetVerLength();
					double dLenHunchHori = tv.GetHorLength();

					if(dLenHunchVert > dLen && dLenHunchHori < dLen)
						tv.m_v1 = tv.m_v2;
				}

				tv.m_sInfo.Format("%d번째내측헌치좌측면", i+1);
				tv.val.valHor		= &m_dHLW[i][iUPPER];
				tv.val.valVer		= &m_dHHL[i][iUPPER];
				tv.val.bRevMoveHor	= TRUE;
				tv.val.bRevMoveVer	= TRUE;
				m_tvArrVertSection.Add(tv);
				tv.m_v1.z			= 0;
				tv.m_v2.z			= 0;
			}
		}

		double dWS	= IsOutWall(TRUE) ? 0 : m_dWS;
		tv.m_bApplyAngle	= bApplyAngle;
		tv.m_dAngle			= 90;
		tv.m_v1				= tv.m_v2;
		tv.m_v1.z			= bDrawJongdanLine ? 5 : 0;
		if(bVertDir)
		{
			tv.m_v2				= GetXyMatchLineAndSlabUpper(GetXySectionSttEnd(TRUE, nLeft, bVertDir), CDPoint(0, -1), nLeft, -1, bVertDir);
			tv.m_v2				= GetXyToDistFromSlabUpper(tv.m_v2.x, (dWS+m_dHWS)/tv.GetVDir().y, nLeft, -1, bVertDir, FALSE);
		}
		else
		{
			tv.m_v2			= GetXySlabUpperByOffsetFrStation(GetXySectionSttEnd(TRUE, 0, FALSE).x, dWS+m_dHWS, nLeft);
			tv.m_v2.y		-= m_dTS;
			tv.m_dAngle		= 90;
		}
		tv.m_sInfo			 = "슬래브하면";
		tv.val.valHor		= NULL;
		tv.val.valVer		= NULL;
		tv.val.bRevMoveVer	= TRUE;
		m_tvArrVertSection.Add(tv);
		m_tvArrBriSect.Add(tv);

		tv.m_bApplyAngle	= bApplyAngle;
		tv.m_dAngle			= bApplyAngle ? (bWingFront ? dAngStt : GetAngleJijum(0).GetAngleDegree()) : 90;
		tv.m_v1				= tv.m_v2;
		if(bVertDir)
		{
			tv.m_v2				= GetXyMatchLineAndSlabUpper(GetXySectionSttEnd(TRUE, nLeft, bVertDir), CDPoint(0, 1), nLeft, -1, bVertDir);
			if(!IsOutWall(TRUE))
				tv.m_v2			= GetXyToDistFromSlabUpper(tv.m_v2.x, dWS/tv.GetVDir().y, nLeft, -1, bVertDir);
		}
		else
		{
			tv.m_v2			= GetXySlabUpperByOffsetFrStation(GetXySectionSttEnd(TRUE, 0, FALSE).x, 0, nLeft);
			tv.m_v2.y		-= m_dTS;
			if(!IsOutWall(TRUE))
				tv.m_v2			= GetXyToDistFromSlabUpper(tv.m_v2.x, dWS/tv.GetVDir().y, nLeft, -1, bVertDir);
		
			tv.m_dAngle		= GetAngleJijumByStaToSta(tv.m_v1.x, tv.m_v2.x, m_dHWS, nLeft);
		}
		tv.m_v2.y			-= IsOutWall(TRUE) ? m_dHHS : m_dHHS;
		if(m_dHRS > 0 && m_nTypeRoundOfHunchStt == 1)
		{
			tv.m_v1.z		= -1;
			tv.m_v2.z		= m_dHRS;
		}
		tv.m_sInfo			= "시점벽체헌치";
		tv.val.valHor		= &m_dHWS;
		tv.val.valVer		= &m_dHHS;
		tv.val.bRevMoveHor	= FALSE;
		tv.val.bRevMoveVer	= TRUE;
		m_tvArrVertSection.Add(tv);
		m_tvArrBriSect.Add(tv);
		tv.m_v1.z			= 0;
		tv.m_v2.z			= 0;


		if(IsOutWall(TRUE))
		{
			tv.m_bApplyAngle	= bApplyAngle;
			tv.m_dAngle			= 90;
			tv.m_v1				= tv.m_v2;
			if(m_nTypeBridge == BRIDGE_TYPE_TRAP2 || m_nTypeBridge == BRIDGE_TYPE_ARCH)
				tv.m_v2.y			= dELSB + m_dWSHH;
			else if(m_nTypeBridge == BRIDGE_TYPE_BOX)
				tv.m_v2.y			= GetElevationBoxBottom(0, 0, FALSE) + m_dTS_Lower +m_dWSHH;
			else
				tv.m_v2.y			= dELSB;
			tv.m_sInfo			= "시점벽체우측면";
			tv.val.valHor		= NULL;
			tv.val.valVer		= NULL;
			m_tvArrVertSection.Add(tv);
			m_tvArrBriSect.Add(tv);

			if(m_nTypeBridge == BRIDGE_TYPE_TRAP2 || m_nTypeBridge == BRIDGE_TYPE_ARCH || m_nTypeBridge == BRIDGE_TYPE_BOX)
			{
				tv.m_bApplyAngle	= bApplyAngle;
				tv.m_dAngle			= bApplyAngle ? (bWingFront ? dAngStt : GetAngleJijum(0).GetAngleDegree()) : 90;
				tv.m_v1				= tv.m_v2;
				
				if(m_nTypeBridge == BRIDGE_TYPE_BOX)
				{
					// Box 하단에 종단경사 적용하면 높이을 계산해 줘야한다.
					if(bVertDir || bWingFront)
					{
						tv.m_v2		= GetxyLowerSlabBottomByLenFrSttStation(m_dWSHW, nLeft, bVertDir);
					}
					else
					{
						tv.m_v2		= GetXyLowerSlabBottomByOffsetFrStation(GetStationOnJijum(0), m_dWSHW, nLeft);
					}
					tv.m_v2.y += m_dTS_Lower;
				}
				else
				{
					if(bVertDir)
						tv.m_v2.x			+= m_dWSHW;
					else
					{
						tv.m_v2.x		= GetXySlabUpperByOffsetFrStation(GetStationOnJijum(0), m_dWSHW, nLeft).x;
						tv.m_dAngle		= GetAngleJijumByStaToSta(tv.m_v1.x, tv.m_v2.x, m_dWSHW, nLeft);
					}
					tv.m_v2.y			-= m_dWSHH;
				}

				tv.m_sInfo			= "시점벽체하부헌치";
				tv.val.valHor		= &m_dWSHW;
				tv.val.valVer		= &m_dWSHH;
				tv.val.bRevMoveHor	= FALSE;
				tv.val.bRevMoveVer	= FALSE;
				m_tvArrVertSection.Add(tv);
				m_tvArrBriSect.Add(tv);
			}
			if(m_nTypeBridge != BRIDGE_TYPE_BOX)
			{
				tv.m_bApplyAngle	= bApplyAngle;
				tv.m_dAngle			= bApplyAngle ? (bWingFront ? dAngStt : GetAngleJijum(0).GetAngleDegree()) : 90;
				tv.m_v1				= tv.m_v2;
				double dW			= m_nTypeBridge == BRIDGE_TYPE_TRAP2 || m_nTypeBridge == BRIDGE_TYPE_ARCH || m_nTypeBridge == BRIDGE_TYPE_BOX ? m_dWS+m_dWSHW : m_dWS;
				if(bVertDir || bWingFront)
					tv.m_v2.x		-= dW/tv.GetVDir().y;
				else
				{
					// (ARCBRIDGE-3442) 벽체 두께에 Station을 적용하지 않는다. 벽체 위치 내에 곡선 선형인경우 오차가 있다.
					// 벽체 두께에 사각을 곱해서 계산한다.
					tv.m_v2.x		= GetXySlabUpperByOffsetFrStation(GetStationOnJijum(0), -m_dWS, nLeft).x;
					tv.m_dAngle		= GetAngleJijumByStaToSta(tv.m_v1.x, tv.m_v2.x, -dW, nLeft);
				}
				tv.m_sInfo			= "시점벽체하면";
				tv.val.valHor		= NULL;
				tv.val.valVer		= &m_dELSB;
				tv.val.bRevMoveVer	= FALSE;
				m_tvArrVertSection.Add(tv);	
			}
			else
			{
				if(bVertDir || bWingFront)
				{
					tv.m_v2		= GetxyLowerSlabBottomByLenFrSttStation(-m_dWS/tv.GetVDir().y, nLeft, bVertDir);
				}
				else
				{
					tv.m_v2 = GetXyLowerSlabBottomByOffsetFrStation(GetStationOnJijum(0), -m_dWS, nLeft);
				}
				
				if(m_bExistProtectionRiseFooting)
				{
					tv.m_v2.y += (m_dPRF_H1 + m_dPRF_H2);
				}
			}

			xyOrgStt = tv.GetXyRight();//.GetXyMid();
			if(m_nTypeBridge == BRIDGE_TYPE_TRAP2 || m_nTypeBridge == BRIDGE_TYPE_ARCH)
			{
				if(bVertDir || bWingFront)
					xyOrgStt.x -= (m_dWSHW/tv.GetVDir().y);
				else
					xyOrgStt.x	= GetXySlabUpperByOffsetFrStation(GetStationOnJijum(0), 0, nLeft).x;
			}

			tv.m_bApplyAngle	= bApplyAngle;
			tv.m_dAngle			= 90;
			tv.m_v1				= tv.m_v2;
			if(bVertDir || bWingFront)
			{
				tv.m_v2				= GetXySlabUpperByLenFrSttStation((-m_dWS-m_dBHWS+m_dBWS)/tv.GetVDir().y, nLeft, bVertDir);
				tv.m_v2.x			= GetXySectionSttEnd(TRUE, nLeft, bVertDir).x - m_dWS/tv.GetVDir().y;
			}
			else
			{
				tv.m_v2			= GetXySlabUpperByOffsetFrStation(GetStationOnJijum(0), -m_dWS-m_dBHWS+m_dBWS, nLeft);
			}
			tv.m_v2.x			= tv.m_v1.x;
			tv.m_v2.y			-= (m_dBTHS+m_dBHS);
			tv.m_sInfo			= "시점벽체좌측면";
			tv.val.valHor		= &m_dWS;
			tv.val.valVer		= NULL;
			tv.val.bRevMoveHor	= TRUE;
			m_tvArrVertSection.Add(tv);

			tv.m_bApplyAngle	= bApplyAngle;
			tv.m_dAngle			= bApplyAngle ? (bWingFront ? dAngStt : GetAngleJijum(0).GetAngleDegree()) : 90;
			tv.m_v1				= tv.m_v2;
			if(bVertDir || bWingFront)
				tv.m_v2.x			-= m_dBHWS/tv.GetVDir().y;
			else
			{
				tv.m_v2.x		= GetXySlabUpperByOffsetFrStation(GetStationOnJijum(0), -m_dWS-m_dBHWS, nLeft).x;
				tv.m_dAngle		= GetAngleJijumByStaToSta(tv.m_v1.x, tv.m_v2.x, -m_dBHWS, nLeft);
			}
			tv.m_v2.y			+= m_dBHHS;
			tv.m_sInfo			= "시점브라켓헌치";
			tv.val.valHor		= &m_dBHWS;
			tv.val.valVer		= &m_dBHHS;
			tv.val.bRevMoveHor	= TRUE;
			tv.val.bRevMoveVer	= FALSE;
			m_tvArrVertSection.Add(tv);

			tv.m_bApplyAngle	= bApplyAngle;
			tv.m_dAngle			= 90;
			tv.m_v1				= tv.m_v2;
			tv.m_v2.y			+= (m_dBHS-m_dBHHS);
			tv.m_sInfo			= "시점브라켓측면";
			tv.val.valHor		= &m_dBHWS;
			tv.val.valVer		= &m_dBHS;
			tv.val.bRevMoveHor	= TRUE;
			tv.val.bRevMoveVer	= TRUE;
			m_tvArrVertSection.Add(tv);

			tv.m_bApplyAngle	= bApplyAngle;
			tv.m_dAngle			= bApplyAngle ? (bWingFront ? dAngStt : GetAngleJijum(0).GetAngleDegree()) : 90;
			tv.m_v1				= tv.m_v2;
			if(bVertDir || bWingFront)
				tv.m_v2.x		+= m_dBWS/tv.GetVDir().y;
			else
			{
				tv.m_v2.x		= GetXySlabUpperByOffsetFrStation(GetStationOnJijum(0), -m_dWS-m_dBHWS+m_dBWS, nLeft).x;
				tv.m_dAngle		= GetAngleJijumByStaToSta(tv.m_v1.x, tv.m_v2.x, m_dBWS, nLeft);
			}
			tv.m_sInfo			= "시점브라켓상면";
			tv.val.valHor		= NULL;
			tv.val.valVer		= &m_dBTHS;
			tv.val.bRevMoveVer	= TRUE;
			m_tvArrVertSection.Add(tv);

			tv.m_bApplyAngle	= bApplyAngle;
			tv.m_dAngle			= 90;
			tv.m_v1				= tv.m_v2;
			tv.m_v2.y			+= m_dBTHS;
			tv.m_sInfo			= "시점브라켓상부측면";
			tv.val.valHor		= &m_dBWS;
			tv.val.valVer		= NULL;
			tv.val.bRevMoveHor	= FALSE;
			m_tvArrVertSection.Add(tv);

			tv.m_bApplyAngle	= bApplyAngle;
			tv.m_dAngle			= bApplyAngle ? (bWingFront ? dAngStt : GetAngleJijum(0).GetAngleDegree()) : 90;
			tv.m_v1				= tv.m_v2;
			tv.m_v2				= GetXySectionSttEnd(TRUE, nLeft, bVertDir);
			if(!bVertDir && !bWingFront)
			{
				tv.m_dAngle		= GetAngleJijumByStaToSta(tv.m_v1.x, tv.m_v2.x, m_dWS+m_dBHWS-m_dBWS, nLeft);
			}

			/*
			if(bVertDir)
			{
				tv.m_dAngle		= GetAngleJijumByStaToSta(tv.m_v1.x, tv.m_v2.x, m_dWS+m_dBHWS-m_dBWS, nLeft);
			}
			*/
			tv.m_v1.z			= bDrawJongdanLine ? 5 : 0;
			tv.m_sInfo			= "슬래브상면(시점벽체위)";
			tv.val.valHor		= NULL;
			tv.val.valVer		= NULL;
			m_tvArrVertSection.Add(tv);
		}
		else
		{
			tv.m_bApplyAngle	= bApplyAngle;
			tv.m_dAngle			= bApplyAngle ? (bWingFront ? dAngStt : GetAngleJijum(0).GetAngleDegree()) : 90;
			tv.m_v1				= tv.m_v2;
			tv.m_v2.x			-= m_dWS/tv.GetVDir().y;
			tv.m_sInfo			= "시점헌치하면";
			tv.val.valHor		= NULL;
			tv.val.valVer		= &m_dHHS;
			tv.val.bRevMoveVer	= TRUE;
			m_tvArrVertSection.Add(tv);		


			tv.m_bApplyAngle	= bApplyAngle;
			tv.m_dAngle			= bApplyAngle ? (bWingFront ? dAngStt : GetAngleJijum(0).GetAngleDegree()) : 90;
			tv.m_v1				= tv.m_v2;
			tv.m_v2				= GetXySectionSttEnd(TRUE, nLeft, bVertDir);
			tv.m_sInfo			= "시점슬래브측면";
			tv.val.valHor		= NULL;
			tv.val.valVer		= NULL;
			m_tvArrVertSection.Add(tv);
		}

		// 시점 헌치 R이 접선인 경우
		// 헌치길이가 0보다 클 경우 R이 존재 할 수 있다.
		if(m_nTypeRoundOfHunchStt == 0 && m_dHRS > 0)
		{
			long nIdx		= m_tvArrVertSection.GetIdxByInfo("슬래브하면");
			CTwinVector tv1	= m_tvArrVertSection.GetAt(nIdx);
			CTwinVector tv2	= m_tvArrVertSection.GetAt(nIdx+1);

			CDPoint xyMatchOrg(0, 0);
			if(GetXyMatchLineAndLine(tv1.m_v1, tv1.GetXyDir(), tv2.m_v1, tv2.GetXyDir(), xyMatchOrg))
			{
				CDPoint xyCen(0, 0);
				if(GetXyTanOfArc(xyMatchOrg, tv1.GetXyDir(TRUE), tv2.GetXyDir(), m_dHRS, xyCen, tv1.m_v2, tv2.m_v1))
				{
					m_tvArrVertSection.SetAt(nIdx, tv1);
					m_tvArrVertSection.SetAt(nIdx+1, tv2);

					CTwinVector tvR;
					tvR.m_v1	= tv1.m_v2;
					tvR.m_v2	= tv2.m_v1;
					tvR.m_v1.z	= -1;
					tvR.m_v2.z	= m_dHRS;
					tvR.val.valHor	= &m_dHRS;
					tvR.val.valVer	= &m_dHRS;
					tvR.val.bRevMoveVer	= TRUE;
					tvR.m_sInfo	= "시점벽체헌치R";
					
					m_tvArrVertSection.InsertAt(nIdx+1, tvR);
				}
			}
		}

		// 종점 헌치 R이 접선인 경우
		// 헌치길이가 0보다 클 경우 R이 존재 할 수 있다.
		if(m_nTypeRoundOfHunchEnd == 0 && m_dHRE > 0)
		{
			long nIdx	= m_tvArrVertSection.GetIdxByInfo("종점벽체헌치");
			CTwinVector tv1	= m_tvArrVertSection.GetAt(nIdx);
			CTwinVector tv2	= m_tvArrVertSection.GetAt(nIdx+1);

			// 시점 헌치에 R이 접선형태로 있을 경우 
			// 각 접점의 위치
			
			CDPoint xyMatchOrg(0, 0);
			if(GetXyMatchLineAndLine(tv1.m_v1, tv1.GetXyDir(), tv2.m_v1, tv2.GetXyDir(), xyMatchOrg))
			{
				CDPoint xyCen(0, 0);
				if(GetXyTanOfArc(xyMatchOrg, tv1.GetXyDir(TRUE), tv2.GetXyDir(), m_dHRE, xyCen, tv1.m_v2, tv2.m_v1))
				{
					m_tvArrVertSection.SetAt(nIdx, tv1);
					m_tvArrVertSection.SetAt(nIdx+1, tv2);

					CTwinVector tvR;
					tvR.m_v1	= tv1.m_v2;
					tvR.m_v2	= tv2.m_v1;
					tvR.m_v1.z	= -1;
					tvR.m_v2.z	= m_dHRS;
					tvR.val.valHor	= &m_dHRE;
					tvR.val.valVer	= &m_dHRE;
					tvR.val.bRevMoveHor	= TRUE;
					tvR.val.bRevMoveVer	= TRUE;
					tvR.m_sInfo	= "종점벽체헌치R";
					
					m_tvArrVertSection.InsertAt(nIdx+1, tvR);
				}
			}
		}

		// Box형인경우 하부슬래브에 대한것을 추가해 준다.
		if(m_nTypeBridge == BRIDGE_TYPE_BOX)
		{
			double dLengthBridge	= fabs(GetXySectionSttEnd(FALSE, nLeft, bVertDir).x-GetXySectionSttEnd(TRUE, nLeft, bVertDir).x);
			if(bWingFront)
				dLengthBridge	= m_dLengthBridge;

			double dAngStt1 = bApplyAngle ? (bWingFront ? dAngStt : GetAngleJijum(0, bVertDir).GetAngleDegree()) : 90;
			double dAngEnd1 = bApplyAngle ? (bWingFront ? dAngEnd : GetAngleJijum(m_nQtyJigan, bVertDir).GetAngleDegree()) : 90;

			CTwinVector tvBottom;
			tv.m_bApplyAngle	= bApplyAngle;
			tv.m_dAngle			= dAngStt1;
			if(bVertDir || bWingFront)
			{
				tv.m_v1		= GetxyLowerSlabBottomByLenFrSttStation(-m_dWS/tv.GetVDir().y, nLeft, bVertDir);
			}
			else
			{
				tv.m_v1		= GetXyLowerSlabBottomByOffsetFrStation(GetStationOnJijum(0), -m_dWS, nLeft);
			}
			tv.m_v2				= GetXyLowerSlabBottomByOffsetFrStation(GetStationOnJijum(0), 0, nLeft);
			tv.m_v1.z			= (bDrawJongdanLine && m_bSlopeLowerSlab) ? 5 : 0;
			tv.m_sInfo			= "하부슬래브하면(시점벽체아래)";
			tv.val.valHor		= NULL;
			tv.val.valVer		= NULL;
			m_tvArrVertSection.Add(tv);
			m_tvArrBriSect.Add(tv);
			tvBottom.m_v1 = tv.m_v1;

			tv.m_bApplyAngle	= bApplyAngle;
			tv.m_dAngle			= 90;
			tv.m_v1				= tv.m_v2;
			if(bVertDir || bWingFront)
			{
				tv.m_v2		= GetxyLowerSlabBottomByLenFrSttStation(dLengthBridge, nLeft, bVertDir);
			}
			else
			{
				tv.m_v2		= GetXyLowerSlabBottomByOffsetFrStation(GetStationOnJijum(m_nQtyJigan), 0, nLeft);
			}
			tv.m_v1.z			= (bDrawJongdanLine && m_bSlopeLowerSlab) ? 5 : 0;
			tv.m_sInfo			= "하부슬래브하면(외측벽체안쪽)";
			tv.val.valHor		= NULL;
			tv.val.valVer		= NULL;
			m_tvArrVertSection.Add(tv);
			m_tvArrBriSect.Add(tv);

			tv.m_bApplyAngle	= bApplyAngle;
			tv.m_dAngle			= dAngEnd1;
			tv.m_v1				= tv.m_v2;
			if(bVertDir || bWingFront)
			{
				tv.m_v2		= GetxyLowerSlabBottomByLenFrSttStation(dLengthBridge + m_dWE / tv.GetVDir().y, nLeft, bVertDir);
			}
			else
			{
				tv.m_v2		= GetXyLowerSlabBottomByOffsetFrStation(GetStationOnJijum(m_nQtyJigan), m_dWE, nLeft);
			}

			tv.m_v1.z			= (bDrawJongdanLine && m_bSlopeLowerSlab) ? 5 : 0;
			tv.m_sInfo			= "하부슬래브하면(종점벽체아래)";
			tv.val.valHor		= NULL;
			tv.val.valVer		= NULL;
			m_tvArrVertSection.Add(tv);
			m_tvArrBriSect.Add(tv);
			tvBottom.m_v2 = tv.m_v2;

			if(m_bExistProtectionRiseFooting)
			{
				tv.m_bApplyAngle	= bApplyAngle;
				tv.m_dAngle			= 90;
				tv.m_v1				= tvBottom.m_v1;
				tv.m_v1.y	+= (m_dPRF_H1 + m_dPRF_H2);
				tv.m_v2				= tvBottom.m_v1;
				tv.m_v1.z			= 0;
				tv.m_sInfo			= "시점벽체(부상방지저판)";
				tv.val.valHor		= NULL;
				tv.val.valVer		= NULL;
				m_tvArrVertSection.Add(tv);
				//m_tvArrBriSect.Add(tv);

				tv.m_bApplyAngle	= bApplyAngle;
				tv.m_dAngle			= 90;
				tv.m_v1				= tvBottom.m_v2;
				tv.m_v1.y	+= (m_dPRF_H1 + m_dPRF_H2);
				tv.m_v2				= tvBottom.m_v2;
				tv.m_v1.z			= 0;
				tv.m_sInfo			= "종점벽체(부상방지저판)";
				tv.val.valHor		= NULL;
				tv.val.valVer		= NULL;
				m_tvArrVertSection.Add(tv);
				//m_tvArrBriSect.Add(tv);

				CTwinVectorArray tvArrTemp;
				GetTvPRF_VertSection(tvArrTemp, TRUE,  nLeft, m_nTypeJongdanForRebar!=0, bApplyAngle, TRUE);
				GetTvPRF_VertSection(tvArrTemp, FALSE, nLeft, m_nTypeJongdanForRebar!=0, bApplyAngle, TRUE);
			}

			// 내측지점이 있던 없던 시점벽체 하부헌치 위치부터 다시 시작.
			if(bVertDir || bWingFront)
			{
				tv.m_v2		= GetxyLowerSlabBottomByLenFrSttStation(m_dWSHW / tv.GetVDir().y, nLeft, bVertDir);
			}
			else
			{
				tv.m_v2		= GetXyLowerSlabBottomByOffsetFrStation(GetStationOnJijum(0), m_dWSHW, nLeft);
			}
			tv.m_v2.y		+= m_dTS_Lower;

			// 좌우측 선형 기준으로 그릴때는 내측헌치를 그리지 않는다 (필요 없기 때문에)
			if(nLeft == 0 && m_nCountInWallHunch > 0)
			{
				
				// 내부 헌치 제원들
				long nIdxInWall = 0;
				BOOL bLeft = TRUE;

				long ix = 0; for(ix = 0; ix < m_nCountInWallHunch; ix++)
				{
					// 헌치 높이 좌우 동일하게 함.
					m_dHHR[ix][iLOWER]	= m_dHHL[ix][iLOWER];

					// Round가 있는 경우 WL이나 WR은 적용 되지 않음
					CWallApp *pWall	= GetInWall(ix);
					CDPoint vAngWall		= ToDPointFrDegree(pWall->GetAngleWallByHor(bApplyAngle));
					BOOL bVertWall	= pWall->IsVertWall();

					// 헌치의 정보를 가져옴
					GetInfoHunch(ix, nIdxInWall, bLeft);
					CDPoint xyHunchBottom(0, 0);
					double dAng = GetAngleJijum(ix+1, bVertDir).GetAngleDegree();
					if(bWingFront)
					{
						if(ix==m_nCountInWallHunch-1)
							dAng = dAngEnd==0 ? 90 : dAngEnd;
						if(ix==0)
							dAng = dAngStt==0 ? 90 : dAngStt;
					}

					tv.m_bApplyAngle	= bApplyAngle;
					tv.m_dAngle			= bApplyAngle ? dAng : 90;
					tv.m_v1				= tv.m_v2;
					tv.m_v1.z			= (bDrawJongdanLine && m_bSlopeLowerSlab) ? 5 : 0;

					CDPoint xyHunchBottomC	= GetXyInWallHunchBottom(nIdxInWall, bLeft, nLeft, bVertDir, FALSE);
					CDPoint xyHunchBottomL(0, 0), xyHunchBottomR(0, 0);
					CDPoint vAngHunchLower	= GetVAngHunchLower(ix, nLeft, bVertDir);
					GetXyHunchLowerSide(ix, nLeft, bVertDir, xyHunchBottomL, xyHunchBottomR, FALSE);

					double dHDiffL		= xyHunchBottomL.y - xyHunchBottomC.y;
					double dHDiffR		= xyHunchBottomR.y - xyHunchBottomC.y;
					xyHunchBottom		= CDPoint(xyHunchBottomL.x, xyHunchBottomL.y);
					//xyHunchBottom.y		-= m_dHBH[i];

					if(!bVertWall)
					{
						dHDiffL	= 0;
						dHDiffR	= 0;
					}
					
					tv.m_v2				= GetXyToDistFromLowerSlab(xyHunchBottomL.x, -m_dHLW[ix][iLOWER]/tv.GetVDir().y, nLeft, -1, bVertDir, FALSE);
					tv.m_sInfo.Format(_T("%d번째내측헌치좌측하부슬래브상면"), ix+1);
					tv.val.valHor		= NULL;
					tv.val.valVer		= NULL;
					tv.val.bRevMoveVer	= TRUE;
					m_tvArrVertSection.Add(tv);
					if(ix==0)
						m_tvArrBriSect.Add(tv);


					tv.m_bApplyAngle	= bApplyAngle;
					tv.m_dAngle			= bApplyAngle ? dAng : 90;
					tv.m_v1				= tv.m_v2;
					tv.m_v2				= xyHunchBottom;
					tv.m_v2.y			+= (dHDiffR - dHDiffL > 0) ? (dHDiffR - dHDiffL) : 0;

					tv.m_sInfo.Format("%d번째내측하부헌치좌측면", ix+1);
					tv.val.valHor		= &m_dHLW[ix][iLOWER];
					tv.val.valVer		= &m_dHHL[ix][iLOWER];
					tv.val.bRevMoveHor	= FALSE;
					tv.val.bRevMoveVer	= TRUE;
					m_tvArrVertSection.Add(tv);
					tv.m_v1.z			= 0;
					tv.m_v2.z			= 0;

					tv.m_bApplyAngle	= FALSE;
					tv.m_dAngle			= 90;
					tv.m_v1				= tv.m_v2;
					tv.m_sInfo.Format("%d번째내측하부헌치좌측면좌측하부", ix+1);
					tv.val.valHor		= NULL;
					tv.val.valVer		= NULL;
					m_tvArrVertSection.Add(tv);

					tv.m_bApplyAngle	= bApplyAngle;
					tv.m_dAngle			= 90;
					tv.m_v1				= tv.m_v2;
					tv.m_v2.y			+= m_dHBH[ix][iLOWER];
					tv.m_sInfo.Format("%d번째내측하부헌치위좌측면", ix+1);
					tv.val.valHor		= &m_dHBH[ix][iLOWER];
					tv.val.valVer		= NULL;
					tv.val.bRevMoveHor	= FALSE;
					m_tvArrVertSection.Add(tv);

					CVector oldVec;	// 신축이음 적용으로 틀려진 다음 위치 보관
					tv.m_bApplyAngle	= bApplyAngle;
					tv.m_dAngle			= bApplyAngle ? dAng : 90;
					tv.m_v1				= tv.m_v2;
					GetXyMatchLineAndLine(CDPoint(tv.m_v2.x+m_dWL[ix][iLOWER]/tv.GetVDir().y, tv.m_v2.y), vAngWall, tv.m_v2, vAngHunchLower, tv.m_v2);

					//tv.m_v2				= tv.m_v2 + (vAngHunchLower * m_dWR[i]/tv.GetVDir().y);
					//tv.m_v2.x			-= (m_dWR[i]/tv.GetVDir().y);
					oldVec				= tv.m_v2;
					if(bVertWall)
					{
						tv.m_v1.y		= max(max(xyHunchBottomC.y, xyHunchBottomL.y), xyHunchBottomR.y);
						tv.m_v2.y		= max(max(xyHunchBottomC.y, xyHunchBottomL.y), xyHunchBottomR.y);
					}
					tv.m_sInfo.Format("%d번째내측하부헌치윗면좌측", ix+1);
					tv.val.valHor		= NULL;
					tv.val.valVer		= &m_dHBH[ix][iLOWER];
					tv.val.bRevMoveVer	= TRUE;
					m_tvArrVertSection.Add(tv);

					tv.m_bApplyAngle	= bApplyAngle;
					tv.m_dAngle			= bApplyAngle ? dAng : 90;
					tv.m_v1				= oldVec;
					tv.m_v2				= oldVec;
					GetXyMatchLineAndLine(CDPoint(tv.m_v2.x+m_dWR[ix][iLOWER]/tv.GetVDir().y, tv.m_v2.y), vAngWall, tv.m_v2, vAngHunchLower, tv.m_v2);
					//tv.m_v2				= tv.m_v2 + (vAngHunchLower * m_dWL[i]/tv.GetVDir().y);
					//tv.m_v2.x			-= (m_dWL[i])/tv.GetVDir().y;
					if(bVertWall)
					{
						tv.m_v1.y		= max(max(xyHunchBottomC.y, xyHunchBottomL.y), xyHunchBottomR.y);
						tv.m_v2.y		= max(max(xyHunchBottomC.y, xyHunchBottomL.y), xyHunchBottomR.y);
					}
					tv.m_sInfo.Format("%d번째내측하부헌치윗면우측", ix+1);
					tv.val.valHor		= NULL;
					tv.val.valVer		= &m_dHBH[ix][iLOWER];
					tv.val.bRevMoveVer	= TRUE;
					m_tvArrVertSection.Add(tv);

					tv.m_bApplyAngle	= bApplyAngle;
					tv.m_dAngle			= 90;
					tv.m_v1				= tv.m_v2;
					tv.m_v2.y			-= m_dHBH[ix][iLOWER];
					tv.m_sInfo.Format("%d번째내측하부헌치위우측면", ix+1);
					tv.val.valHor		= &m_dWR[ix][iLOWER];
					tv.val.valVer		= NULL;
					tv.val.bRevMoveHor	= TRUE;
					m_tvArrVertSection.Add(tv);

					tv.m_bApplyAngle	= FALSE;
					tv.m_dAngle			= 90;
					tv.m_v1				= tv.m_v2;

					tv.m_sInfo.Format("%d번째내측하부헌치우측면우측하부", ix+1);
					tv.val.valHor		= NULL;
					tv.val.valVer		= NULL;
					m_tvArrVertSection.Add(tv);

					tv.m_bApplyAngle	= bApplyAngle;
					tv.m_dAngle			= bApplyAngle ? dAng : 90;
					tv.m_v1				= tv.m_v2;
					tv.m_v2				= GetXyMatchLineAndLowerSlab(tv.m_v1, vAngWall, nLeft, -1, bVertDir);
					tv.m_v2				= GetXyToDistFromLowerSlab(tv.m_v2.x, +m_dHRW[ix][iLOWER]/tv.GetVDir().y, nLeft, -1, bVertDir, FALSE);

					tv.m_sInfo.Format("%d번째내측하부헌치우측면", ix+1);
					tv.val.valHor		= &m_dHRW[ix][iLOWER];
					tv.val.valVer		= &m_dHHR[ix][iLOWER];
					tv.val.bRevMoveHor	= TRUE;
					tv.val.bRevMoveVer	= TRUE;
					m_tvArrVertSection.Add(tv);
					tv.m_v1.z			= 0;
					tv.m_v2.z			= 0;
				}
			}

			tv.m_bApplyAngle	= bApplyAngle;
			tv.m_dAngle			= 90;
			tv.m_v1				= tv.m_v2;
			tv.m_v1.z			= (bDrawJongdanLine && m_bSlopeLowerSlab) ? 5 : 0;

			if(bVertDir || bWingFront)
			{
				tv.m_v2		= GetxyLowerSlabBottomByLenFrSttStation(dLengthBridge - m_dWEHW / tv.GetVDir().y, nLeft, bVertDir);
			}
			else
			{
				tv.m_v2		= GetXyLowerSlabBottomByOffsetFrStation(GetStationOnJijum(m_nQtyJigan), -(m_dWEHW), nLeft);
			}
			tv.m_v2.y		+= m_dTS_Lower;
			tv.m_dAngle		= 90;

			tv.m_sInfo			 = "하부슬래브상면";
			tv.val.valHor		= NULL;
			tv.val.valVer		= NULL;
			tv.val.bRevMoveVer	= TRUE;
			m_tvArrVertSection.Add(tv);
			m_tvArrBriSect.Add(tv);
		}
	}


	// 아치교의 경우 리브에 대해서도 해준다.
	if(m_nTypeBridge == BRIDGE_TYPE_ARCH)
	{
		long upper = 0; for(upper = 0; upper < 2; upper++)
		{
			BOOL bUpper	= upper == iUPPER;
			m_pbRib[upper].m_dDistStt	= -m_pbRib[upper].m_dDistTotal/2;
			m_pbRib[upper].m_dDistEnd	= m_pbRib[upper].m_dDistTotal/2;
			AddFromParabola(m_tvArrVertSectionRib[upper], m_pbRib[upper]);
			long i = 0; for(i = 0; i < m_tvArrVertSectionRib[upper].GetSize(); i++)
			{
				tv	= m_tvArrVertSectionRib[upper].GetAt(i);
				tv.m_sInfo	= bUpper ? "상부리브" : "하부리브";
				m_tvArrVertSectionRib[upper].SetAt(i, tv);
			}
			//m_tvArrVertSectionRib[upper].AddFromArch(m_xyRibCrown[upper], m_dRibLenTotal[upper], m_dRibHeightTotal[upper], m_dRibFactorM[upper], m_dRibLenStt[upper], m_dRibLenEnd[upper], m_dRibLenArchLine[upper]);
		}

		m_pbRibCen.m_dDistStt	= -m_pbRibCen.m_dDistTotal/2;
		m_pbRibCen.m_dDistEnd	= m_pbRibCen.m_dDistTotal/2;
		AddFromParabola(m_tvArrVertSectionRibCen, m_pbRibCen);
		//m_tvArrVertSectionRibCen.AddFromArch(m_xyRibCrownCen, m_dRibLenTotalCen, m_dRibHeightTotalCen, m_dRibFactorMCen, m_dRibLenSttCen, m_dRibLenEndCen, m_dRibLenArchLineCen);
	}

	if(m_nTypeBridge == BRIDGE_TYPE_BOX)
	{
		GetTvBoxLowerExFootFront(nLeft, bApplyAngle);
		GetTvBoxLowerExFootPlan();
	}

	// 좌표를 구할때마다 기초의 스테이션과 EL을 정해준다.
	m_footingStt.m_dELDiff	= 0;//GetDiffELSttAndJijumForVertSection(0, TRUE);
	m_footingEnd.m_dELDiff	= GetDiffELSttAndJijumForVertSection(m_nQtyJigan, FALSE);
	xyOrgEnd.y += m_footingEnd.m_dELDiff;
	SetFootingStaAndEL(xyOrgStt, xyOrgEnd, bApplyAngle, nLeft);

	m_dTS	= dTSOld;

	return TRUE;
}

// 방호벽 및 보차도 경계석
//<param name="bRoadNotSlab">TRUE=지중라멘의 상부 노면(도로면, 철도면)이며 슬래브면(교면)아님
//							 FALSE=일반교량, 지중라멘의 슬래브면(교면)</param>
void CRcBridgeApp::GetTvGuardWall(CGuardWallRC* pGW, CTwinVectorArray& tvArr, double sta, CDPoint vAng, BOOL bApplyRoundChamfer, BOOL bRoadNotSlab, BOOL bExp, BOOL bForBM)
{
	if(!IsExp(sta))
		bExp	= FALSE;

	double dSlopeSlab = GetSlopeHDan(pGW->m_nNumberHDan);
	if(!m_bUnderGroundRahmen && m_bApplyLineSlope && IsLevelHDanApplyLineSlope(pGW->m_nNumberHDan))		// 31695
		dSlopeSlab = 0;
	pGW->GetTvGuardWallBase(tvArr, bApplyRoundChamfer, bForBM, dSlopeSlab);

	BOOL bHDanFootWay = (pGW->IsTypeHDanRoadFootway() || pGW->IsTypeHDanRoadFootwayBlock()) ? TRUE : FALSE;

	// vAngReal : 원곡선일때 사각만 적용하면 안 되므므로, 실제 길이를 계산해서 
	// 각 단마다 다르게 vangReal을 적용한다.
	CDPoint vAngReal	= vAng;
	double dLenVert	= GetLengthHDanAct(sta, CDPoint(0, 1), pGW->m_nNumberHDan, bRoadNotSlab);
	double dLenAng	= GetLengthHDanAct(sta, vAng, pGW->m_nNumberHDan, bRoadNotSlab);
	if(dLenAng > 0 && dLenVert > 0) vAngReal.y	= dLenVert / dLenAng;


	long n = 0; for(n = 0; n < tvArr.GetSize(); n++)
	{
		if(vAngReal.y==1.0) break;
		tvArr.GetAt(n).m_v1.x /= vAngReal.y;
		tvArr.GetAt(n).m_v2.x /= vAngReal.y;
	}

	CDPoint xyLeft = GetXySlabHDanLeft(sta, vAng, pGW->m_nNumberHDan,bRoadNotSlab, bExp);
	for(n = 0; n < tvArr.GetSize(); n++)
	{
		tvArr.GetAt(n).m_v1 += xyLeft;
		tvArr.GetAt(n).m_v2 += xyLeft;
	}

	double diffH = 0;

	// 가각일때는 가각용 slop를 미리 계산해놔야 한다.
	if(bExp)
	{
		SetXySlabUpperAct90_Gagak(sta,vAng);
	}

	CDPoint xyL(0,0),xyR(0,0);
	xyL = GetXySlabUpperActByDistAngle(sta, tvArr.GetAt(1).m_v1.x, vAng,bRoadNotSlab, bExp);
	xyR = GetXySlabUpperActByDistAngle(sta, tvArr.GetAt(13).m_v2.x,vAng,bRoadNotSlab, bExp);
	if(bHDanFootWay==FALSE)
	{
		if(xyL.y > xyR.y)
			diffH = xyLeft.y - xyL.y;
		else
			diffH = xyLeft.y - xyR.y;
	}

	if(pGW->IsTypeHDanRoadRailDike())
	{
		diffH += pGW->GetHeight()-150;
		for(n = 0; n < tvArr.GetSize(); n++)
		{
			tvArr.GetAt(n).m_v1.y -= diffH;
			tvArr.GetAt(n).m_v2.y -= diffH;
		}
		tvArr.GetAt(13).m_v2 = tvArr.GetAt(0).m_v2;
	}
	else
	{
		for(n = 0; n < tvArr.GetSize(); n++)
		{
			if(diffH==0.0) break;
			CTwinVector tv = tvArr.GetAt(n);
			tvArr.GetAt(n).m_v1.y -= diffH;
			tvArr.GetAt(n).m_v2.y -= diffH;
		}

		if(bHDanFootWay==FALSE) 
		{
			if(xyL.y > xyR.y)
			{
				tvArr.GetAt(13).m_v2 = xyR;
			}
			else
			{
				tvArr.GetAt(0).m_v2 = xyL;
				tvArr.GetAt(1).m_v1 = xyL;
			}
			tvArr.GetAt(0).m_v1 = xyLeft;
		}
	}

	if(bForBM)	// 페곡선으로 만들어야 수량에서 도형으로 분리할 수 있다.
	{
		long nStt = (long)pGW->m_dDistFrLeft == 0 ? 0 : 1;
		
		long nIdexLast(0);
		CTwinVectorArray tvArrTemp;
		long k=0; for(k=nStt; k<tvArr.GetSize(); k++)
		{
			tvArrTemp.Add(tvArr[k]);
			if(tvArr[k].m_sInfo.Find(_T("Hole")) == -1)
				nIdexLast = k-nStt;
		}
		
		CTwinVector tv;
		tv.m_v1 = tvArrTemp[nIdexLast].m_v2;
		tv.m_v2 = tvArrTemp[0].m_v1;
		tvArrTemp.Add(tv);

		tvArr.RemoveAll();
		for(k=0; k<tvArrTemp.GetSize(); k++)
		{
			tv = tvArrTemp[k];
			tvArr.Add(tvArrTemp[k]);
		}
	}
}

// 종단면에서의 시점좌표나 종점좌표를 리턴한다.
// nLeft = -1:좌, 0:중심, 1:우
// 포장높이를 고려해서 실제 선형보다 포장두께만큼 아래로 내려간 좌표가 리턴된다.
// 단 좌우측 벽체가 없는 경우 유간을 고려해서 실제 슬래브의 시점 종점 좌표를 리턴한다.
CDPoint CRcBridgeApp::GetXySectionSttEnd(BOOL bStt, int nLeft, BOOL bVertDir)
{
	CLineInfo* pLine = GetLineBase();

	double dStaStt = 0, dStaEnd = 0;
	double dStaStt2 = 0, dStaEnd2 = 0;
	CDPoint A[4], B[4];
	CDPoint A2[4], B2[4];

	GetXySss(BRIDGE_OFF_STT_STT, A);
	GetXySss(BRIDGE_OFF_END_END, B);
	GetXySss(BRIDGE_OFF_STT_SLAB, A2);
	GetXySss(BRIDGE_OFF_END_SLAB, B2);

	BOOL bLeft = nLeft==1 ? FALSE : TRUE;
	BOOL bSttOrg	= bStt;
	if(m_nTypeJongdanForRebar == 0)
		bStt = TRUE;

	CDPoint xy(0, 0);
	if(nLeft==-1)
	{
		pLine->GetStationMatchVerticalByXy(A[0], dStaStt);
		pLine->GetStationMatchVerticalByXy(B[0], dStaEnd);
		pLine->GetStationMatchVerticalByXy(A2[0], dStaStt2);
		pLine->GetStationMatchVerticalByXy(B2[0], dStaEnd2);
	}
	else if(nLeft==1)
	{
		pLine->GetStationMatchVerticalByXy(A[3], dStaStt);
		pLine->GetStationMatchVerticalByXy(B[3], dStaEnd);
		pLine->GetStationMatchVerticalByXy(A2[3], dStaStt2);
		pLine->GetStationMatchVerticalByXy(B2[3], dStaEnd2);
	}
	else
	{
		dStaStt	= GetStationByTarget(BRIDGE_OFF_STT_STT, 0, FALSE);
		dStaEnd	= GetStationByTarget(BRIDGE_OFF_END_END, 0, FALSE);
		dStaStt2	= GetStationByTarget(BRIDGE_OFF_STT_SLAB, 0, FALSE);
		dStaEnd2	= GetStationByTarget(BRIDGE_OFF_END_SLAB, 0, FALSE);

		if(!IsOutWall(TRUE))
			dStaStt	= dStaStt2;
		if(!IsOutWall(FALSE))
			dStaEnd	= dStaEnd2;
	}

	xy.x = bSttOrg ? dStaStt : dStaEnd;

	if(m_nTypeJongdanForRebar != 2)
	{
		CDPoint xy1	= CDPoint(dStaStt2, pLine->GetElevationPlanEl(dStaStt2));
		CDPoint xy2	= CDPoint(dStaEnd2, pLine->GetElevationPlanEl(m_nTypeJongdanForRebar == 0 ? dStaStt2 : dStaEnd2));
		GetXyMatchLineAndLine(xy1, (xy2-xy1).Unit(), CDPoint(xy.x, 0), CDPoint(0, 1), xy);
		xy.y -= GetThickPave(FALSE);

		// FL 고려해야함.
		if(m_dDisBaseElevation != 0)
		{
			double dStaCenter	= GetStationOnJijum(0, 0, FALSE);
			double dELByDis	= GetXySlabUpperActByDist(dStaCenter, 0, FALSE, TRUE, TRUE, TRUE).y;

			double dDisBaseElevationOld	= m_dDisBaseElevation;
			m_dDisBaseElevation	= 0;
			double dELBy0	= GetXySlabUpperActByDist(dStaCenter, 0, FALSE, TRUE, TRUE, TRUE).y;
			m_dDisBaseElevation	= dDisBaseElevationOld;
			xy.y += (dELByDis - dELBy0);
		}
	}
	else
	{
		if(nLeft==0)
		{
			xy.y	= GetXySlabUpperByLenFrSttStation(bStt ? (dStaStt-m_dStationBridgeStt) : (dStaEnd-m_dStationBridgeStt), 0, FALSE).y;
			//xy.y = bStt ? pLine->GetElevationPlanEl(dStaStt) : pLine->GetElevationPlanEl(dStaEnd);
			// 포장 두께 고려해서 아래로 내림
			//xy.y -= GetThickPave(FALSE);
		}
		else
			xy.y = GetElSlabEndAtBriEnd(bStt, bLeft);
	}
	
	CDPoint xyRet	= bVertDir ? GetXyVertSectionByDirVert(xy, bSttOrg ? 0 : m_nQtyJigan) : xy;
	if(!bVertDir && !bSttOrg)
	{
		xyRet.x	= dStaStt + GetLengthJigan(-1, nLeft);
		if(m_bExpJoint[0])
		{
			CJijum *pJ	= GetJijum(0);
			xyRet.x -= pJ->m_dLengthUgan[1];
		}

		if(m_bExpJoint[m_nQtyJigan])
		{
			CJijum *pJ	= GetJijum(m_nQtyJigan);
			xyRet.x -= pJ->m_dLengthUgan[0];
		}
	}
	return xyRet;
}

// BOX형에서 부상방지판을 포함한 
// 종단면에서의 시점좌표나 종점좌표를 리턴한다.
CDPoint CRcBridgeApp::GetXySectionSttEndIncludePRF(BOOL bStt)
{
	if(m_nTypeBridge != BRIDGE_TYPE_BOX) return CDPoint(0,0);

	CLineInfo* pLine = GetLineBase();
	CTwinVector tv;
	CDPoint xyStt(0,0), xyPRF(0,0);

	long nJijum = bStt ? 0 : GetCountJijum() - 1;

	double sw = bStt ? -1.0 : 1.0;
	double dStaJijum = GetStationOnJijum(nJijum);
	double dThickWall = bStt ? m_dWS : m_dWE;
	CDPoint vAngJijum = GetAngleJijum(nJijum);

	//벽체 기준점
	xyStt.x	= pLine->GetStationByOffsetWidth(dStaJijum, sw*dThickWall, vAngJijum);
	xyStt.y	= m_dELSB;
	if(bStt == FALSE && m_bSlopeLowerSlab== TRUE)
	{
		double dStation1 = pLine->GetStationByOffsetWidth(GetStationOnJijum(0), -m_dWS, GetAngleJijum(0));
		double dEL1 = pLine->GetElevationPlanEl(dStation1); // 시점기준위치 계획고
		double dEL2 = pLine->GetElevationPlanEl(xyStt.x);	// 종점기준위치 계획고
		xyStt.y = m_dELSB + (dEL2 - dEL1);
	}

	// 부상방지저판 끝단
	double dGiulJong = m_bSlopeLowerSlab ? pLine->GetJongDanGiul(xyStt.x) :0.0;
	if(m_bExistProtectionRiseFooting)
	{
		xyPRF.x = xyStt.x + sw * m_dPRF_W / vAngJijum.y;
		xyPRF.y = xyStt.y + sw * m_dPRF_W / vAngJijum.y * dGiulJong;
	}
	else
	{
		xyPRF.x = xyStt.x;
		xyPRF.y = xyStt.y;
	}

	return xyPRF;
}
// 시점측에서 dX만큼 떨어진 곳의 좌표
// long nLeft 기준선형 : -1 좌측,  0 중심, 1 우측
// 계획고가 아닌 슬래브 상단 리턴
// double dDistFromSttReal : 중심선형 기준 실제 거리
CDPoint CRcBridgeApp::GetXySlabUpperByLenFrSttStation(double dDistFromStt, long nLeft, BOOL bVertDir)
{
	CDPoint xy(0, 0);
	// 기존에 저장된 것이 있는지 체크 /////
	long i = 0; for(i = 0; i < m_pArrSaveXySlabUpperByLenFrSttStation.GetSize(); i++)
	{
		CSaveXySlabUpperByLenFrSttStation *pSave	= m_pArrSaveXySlabUpperByLenFrSttStation.GetAt(i);
		if(pSave->IsMatch(dDistFromStt, nLeft, bVertDir, m_nTypeJongdanForRebar))
		{
			pSave->GetValue(xy);			
			return xy;
		}
	}
	
	CLineInfo *pLineCR	= GetLineBase(bVertDir);
	CLineInfo *pLine	= GetLineLeftCenRight(nLeft, bVertDir);
	if(!pLine) return CDPoint(GetXySectionSttEnd(TRUE, nLeft, bVertDir).x+dDistFromStt, GetXySectionSttEnd(TRUE, nLeft, bVertDir).y);

	if(m_nTypeJongdanForRebar == 2 && !bVertDir) // 선형따라 그릴경우
	{
		double dStaCenter	= GetStationOnJijum(0, 0) + dDistFromStt;
		xy.x	= dStaCenter;
		xy.y	= GetXySlabUpperActByDist(dStaCenter, 0, FALSE, TRUE, TRUE, TRUE).y;// pLine->GetElevationPlanEl(m_dStationBridgeStt + dDistFromStt) - GetThickPave(FALSE);	// 계획고가 아니라 슬래브 상단 이므로 포장두께 빼 준다.

		if(nLeft != 0)
		{
			double dStaLeft	= GetStationByCenterStation(dStaCenter, nLeft);
			double dSta1	= GetStationOnJijum(0, nLeft);
			double dSta2	= dStaLeft;
			
			double dLengthFromStt	= GetLengthStaToSta(dSta1, dSta2, nLeft);
			xy.x	= GetXySectionSttEnd(TRUE, nLeft).x + dLengthFromStt;

			CDPoint xyPlaneLR	= GetXyMatchLineLRByCenterStation(dStaCenter, nLeft);
			pLineCR->GetStationMatchVerticalByXy(xyPlaneLR, dStaCenter);
			
			double dDist	= pLineCR->GetDisMatchVerticalByXy(xyPlaneLR);
			xy.y	= GetXySlabUpperActByDist(dStaCenter, dDist, FALSE, TRUE, TRUE, FALSE).y;
		}
	}
	else if(m_nTypeJongdanForRebar == 1) // 직선으로 그릴 경우(경사에 의해 계산)
	{
		CDPoint xyDir = CDPoint(GetXySectionSttEnd(FALSE, nLeft, bVertDir)-GetXySectionSttEnd(TRUE, nLeft, bVertDir)).Unit();

		// GetXySectionSttEnd 함수는 시점이 아닌 슬래브 시작점을 리턴하므로 슬래브교인 경우 시점에서의 유간 고려해줌
		xy = GetXySectionSttEnd(TRUE, nLeft, bVertDir);
		if(!IsOutWall(TRUE))
		{
			CJijum *pJ	= GetJijum(0);
			if(m_bExpJoint[0])
				xy.x -= pJ->m_dLengthUgan[1];
		}

		xy.x += dDistFromStt;
		GetXyMatchLineAndLine(xy, CDPoint(0, 1), GetXySectionSttEnd(TRUE, nLeft, bVertDir), xyDir, xy);
	}
	else // 수평으로 그릴 경우.
	{
		xy = GetXySectionSttEnd(TRUE, nLeft, bVertDir);
		
		if(!IsOutWall(TRUE))
		{
			CJijum *pJ	= GetJijum(0);
			if(m_bExpJoint[0])
				xy.x -= pJ->m_dLengthUgan[1];
		}
		xy.x += dDistFromStt;
	}

	// 기존에 저장된 것이 없으면 추가
	CSaveXySlabUpperByLenFrSttStation *pSave	= new CSaveXySlabUpperByLenFrSttStation;
	pSave->SetValue(dDistFromStt, nLeft, bVertDir, m_nTypeJongdanForRebar, xy);
	m_pArrSaveXySlabUpperByLenFrSttStation.Add(pSave);
	
	return xy;
}

// idx번째 헌치의 높이를 구함.
double CRcBridgeApp::GetHeightHunch(long nIdxHunch, BOOL bUpper)
{
	if(m_nCountInWallHunch <= nIdxHunch) return 0;

	return max(m_dHHL[nIdxHunch][bUpper? iUPPER:iLOWER], m_dHHR[nIdxHunch][bUpper? iUPPER:iLOWER])+m_dHBH[nIdxHunch][bUpper? iUPPER:iLOWER];
}

// nIdxWall의 왼쪽이나 오른쪽 헌치의 index를 찾는다.
// bLeft는 V형 벽체일 경우만 사용.
long CRcBridgeApp::GetIdxHunchInWall(long nIdxWall, BOOL bLeft)
{
	long nIdx = -1;
	for(long i = 0; i <= nIdxWall; i++)
	{
		CWallApp *pWall = GetInWall(i);
		if(pWall)
		{
			if(i == nIdxWall)
			{
				if(pWall->m_nType == WALL_TYPE_V && pWall->m_bIs)
				{
					if(bLeft) 
						nIdx++;
					else 
						nIdx += 2;
				}
				else
				{
					nIdx++;
				}
			}
			else
			{
				if(pWall->m_nType == WALL_TYPE_V && pWall->m_bIs)
					nIdx+=2;
				else 
					nIdx++;
			}
		}
	}

	return nIdx;
}

// 내부벽체의 Org를 지간에 기준해서 설정해준다.
// 1. 슬래브 중심을 기준으로 벽체의 중심을 설정한 다음
// 2. V형일경우 그냥 놔두고, 일반형일 경우 해당 헌치를 찾는다.
// 3. 해당 헌치를 찾았을 경우 헌치의 아래를 중심으로 Org의 높이를 맞춰준다.
// long nLeft : 기준이 되는 선형, -1 왼쪽, 0 중심, 1 오른쪽
// 
BOOL CRcBridgeApp::SetOrgInWall(long nIdxWall, long nLeft, BOOL bVertDir)
{
	if(GetCountInWall() <= nIdxWall) return FALSE;

	// 기존에 저장된 것이 있는지 체크 /////
	// 종단형식에 대한 옵션이없어서 정확한 데이타를 저장 할 수 없어서 막음.
	// 앞으로도 풀어서는 안됨.
	/*
	long i = 0; for(i = 0; i < m_pArrSaveOrgInWall.GetSize(); i++)
	{
		CSaveOrgInWall *pSave	= m_pArrSaveOrgInWall.GetAt(i);
		if(pSave->IsMatch(nIdxWall, nLeft, bVertDir))
		{
			pSave->GetValue(GetInWall(nIdxWall)->m_xyOrg);			
			return TRUE;
		}
	}
	*/

	
	CLineInfoApp *pLine = m_pLineApp;
	if(!pLine) return FALSE;
	
	CWallApp *pWall = GetInWall(nIdxWall);
	long nJ	= nIdxWall + 1;
	long nIdxHunch = GetIdxHunchInWall(nIdxWall);
	/// m_dTS의 경우 경사식에서 경사길이가 고려된 두께이기 때문에 org를 셋팅할때는 원래의 두께를 사용하도록 한다
	double dTS = m_dTSOld;	


	double dSta		= bVertDir ? GetStationOnJijum(nJ, nLeft, bVertDir) : GetXySlabUpperByLenFrSttStation(GetStationOnJijum(nJ)-GetStationOnJijum(0), nLeft).x;
	CDPoint xySta	= GetXyMatchLineAndSlabUpper(CDPoint(dSta, 0), CDPoint(0, 1), 0, dTS/2, FALSE);
	pWall->m_dAngleUpperSlab_Front	= GetAngleJongdan(dSta);
	CDPoint vAngWall	= ToDPointFrDegree(pWall->GetAngleWallByHor(!bVertDir));
	CDPoint vAng		= GetAngleJijum(nJ, bVertDir);
	double dWL	= m_dWL[nIdxHunch][iUPPER]/vAng.y;
	double dWR	= m_dWR[nIdxHunch][iUPPER]/vAng.y;

	CDPoint xyStaL	= GetXyMatchLineAndSlabUpper(CDPoint(dSta-dWL, 0), CDPoint(0, 1), 0, dTS/2, FALSE);
	CDPoint xyStaR	= GetXyMatchLineAndSlabUpper(CDPoint(dSta+dWR, 0), CDPoint(0, 1), 0, dTS/2, FALSE);

	if(0) //bVertDir
	{
		xySta	= GetXyVertSectionByDirVert(xySta, nIdxWall+1);
		xyStaL	= GetXyVertSectionByDirVert(xyStaL, nIdxWall+1);
		xyStaR	= GetXyVertSectionByDirVert(xyStaR, nIdxWall+1);
	}
	
	double dH	= 0;
	
	if(nIdxHunch > -1)
	{
		dH	= GetHeightHunch(nIdxHunch);
	}

	CDPoint xyLower	= GetXyMatchLineAndSlabUpper(xySta, pWall->IsVertWall() ? CDPoint(0, 1) : vAngWall, nLeft, dTS, bVertDir);
	double dTSHalfByAngle	= ~(xySta - xyLower);
			
	
	// 지점에 있는 벽체 xyOrg를 하부 슬래브로 이동
	double dDistToHunchBottom	= dTSHalfByAngle+dH;
	if(pWall->IsVertWall())
	{
		xyStaL	= xyStaL + (vAngWall * -dDistToHunchBottom);
		xyStaR	= xyStaR + (vAngWall * -dDistToHunchBottom);
		
		double dELBottom	= min(xyStaL.y, xyStaR.y);
		GetXyMatchLineAndLine(xySta, vAngWall, CDPoint(0, dELBottom), CDPoint(1, 0), pWall->m_xyOrg);

		CDPoint xyHunchBottomL	= GetXyToDistFromSlabUpper(pWall->m_xyOrg.x, -m_dWL[nIdxHunch][iUPPER]/vAng.y, nLeft, dTS+dH, bVertDir, TRUE);
		CDPoint xyHunchBottomR	= GetXyToDistFromSlabUpper(pWall->m_xyOrg.x, m_dWR[nIdxHunch][iUPPER]/vAng.y, nLeft, dTS+dH, bVertDir, TRUE);
		CDPoint xyHunchBottomest	= xyHunchBottomL.y < xyHunchBottomR.y ? xyHunchBottomL : xyHunchBottomR;
		if(xyHunchBottomest.y < pWall->m_xyOrg.y)
		{
			GetXyMatchLineAndLine(xyHunchBottomest, CDPoint(1, 0), pWall->m_xyOrg, vAngWall, pWall->m_xyOrg);
		}
	}
	else
	{
		pWall->m_xyOrg = xySta + (vAngWall * -dDistToHunchBottom);
	}
	
	// 기존에 저장된 것이 없으면 추가
	// 종단형식에 대한 옵션이없어서 정확한 데이타를 저장 할 수 없어서 막음.
	// 앞으로도 풀어서는 안됨.
	/*
	CSaveOrgInWall *pSave	= new CSaveOrgInWall;
	pSave->SetValue(nIdxWall, nLeft, bVertDir, pWall->m_xyOrg);
	m_pArrSaveOrgInWall.Add(pSave);
	*/

	return TRUE;
}

// 직선과 슬래브 하면과의 교차점 찾는 함수.
// long nLeft 기준 선형 : nLeft -1 좌측, nLeft 0 중심, nLeft 1 우측
// dOffset은 상부에서의 간격
// 계획고가 아닌 슬래브 상면 리턴
CDPoint CRcBridgeApp::GetXyMatchLineAndSlabUpper(CDPoint xy, CDPoint xyDir, long nLeft, double dOffset, BOOL bVertDir)
{
	CDPoint xyMatch(0, 0);
	
	// 기존에 저장된 것이 있는지 체크 /////
	long i = 0; for(i = 0; i < m_pArrSaveXyMatchLineAndSlabUpper.GetSize(); i++)
	{
		CSaveXyMatchLineAndSlabUpper *pSave	= m_pArrSaveXyMatchLineAndSlabUpper.GetAt(i);
		if(pSave->IsMatch(xy, xyDir, nLeft, dOffset, bVertDir, m_nTypeJongdanForRebar))
		{
			pSave->GetValue(xyMatch);			
			return xyMatch;
		}
	}

	dOffset = Compare(dOffset, -1.0, "=") ? m_dTS : dOffset;
	CLineInfo *pLine	= GetLineLeftCenRight(nLeft, bVertDir);
	if(m_nTypeJongdanForRebar == 2 && pLine)
	{
		double dTP	= GetThickPave(FALSE);
		if(nLeft == 0 && xyDir.x == 0)
		{
			xyMatch.x	= xy.x;
 			xyMatch.y	= GetXySlabUpperActByDist(xy.x, 0, FALSE, TRUE, TRUE, TRUE).y;
			xyMatch.y -= dOffset;
		}
		else
		{
			CLineInfo *pLineCR	= GetLineLeftCenRight(0, bVertDir);
			double dStaCenter	= xy.x;
			//pLineCR->GetStationMatchVerticalByXy(xy, dStaCenter);
 			double dELDiff		= 0;
			CTwinVectorArray tvArrCross;
			GetTvCrossSection(tvArrCross, dStaCenter, CDPoint(0, 1), FALSE, FALSE, FALSE, 0, FALSE, FALSE, FALSE, TRUE);
			double dELCenter	= pLineCR->GetElevationPlanEl(dStaCenter);
			double dELLeft		= dELCenter;
			double dELRight		= dELCenter;
			if(tvArrCross.GetSize() > 0)
			{
				dELLeft		= tvArrCross.GetAt(0).m_v1.y;
				dELRight	= tvArrCross.GetAt(tvArrCross.GetSize()-1).m_v2.y;
			}

			if(nLeft == -1)
				dELDiff	= dELCenter - dELLeft;
			else if(nLeft == 1)
				dELDiff	= dELCenter - dELRight;
			pLineCR->GetXyMatchByJongDanXyAndAngle(xy, xyDir, xyMatch, -(dOffset + dTP));
			xyMatch.y -= dELDiff;
		}
	}
	else
	{
		long nTypeJongdan	= m_nTypeJongdanForRebar;
		if(m_nTypeJongdanForRebar == 2)
		{
			m_nTypeJongdanForRebar	= 1;
		}
		CDPoint xyStt = GetXySectionSttEnd(TRUE, nLeft, bVertDir);
		CDPoint xyEnd = GetXySectionSttEnd(FALSE, nLeft, bVertDir);
		xyStt.y -= dOffset;
		xyEnd.y -= dOffset;
		
		GetXyMatchLineAndLine(xyStt, (xyEnd-xyStt).Unit(), xy, xyDir, xyMatch);

		m_nTypeJongdanForRebar	= nTypeJongdan;
	}

	// 기존에 저장된 것이 없으면 추가
	CSaveXyMatchLineAndSlabUpper *pSave	= new CSaveXyMatchLineAndSlabUpper;
	pSave->SetValue(xy, xyDir, nLeft, dOffset, bVertDir, m_nTypeJongdanForRebar, xyMatch);
	m_pArrSaveXyMatchLineAndSlabUpper.Add(pSave);

	return xyMatch;
}

// 직선과 하부슬래브 하면과의 교차점 찾는 함수.
// long nLeft 기준 선형 : nLeft -1 좌측, nLeft 0 중심, nLeft 1 우측
// dOffset은 하부에서의 간격
// 계획고가 아닌 슬래브 상면 리턴
CDPoint CRcBridgeApp::GetXyMatchLineAndLowerSlabLower(CDPoint xy, CDPoint xyDir, long nLeft, double dOffset, BOOL bVertDir)
{
	CDPoint xyMatch(0, 0);

	dOffset = Compare(dOffset, -1.0, "=") ? m_dTS_Lower : dOffset;
	CLineInfo *pLine	= GetLineLeftCenRight(nLeft, bVertDir);
	if(m_nTypeJongdanForRebar == 2 && pLine)
	{
		xyMatch.x	= xy.x;
		xyMatch.y	= GetXyLowerSlabBottomByOffsetFrStation(xy.x, 0, nLeft).y;
		xyMatch.y += dOffset;
		
	}
	else
	{
		long nTypeJongdan	= m_nTypeJongdanForRebar;
		if(m_nTypeJongdanForRebar == 2)
		{
			m_nTypeJongdanForRebar	= 1;
		}
		CDPoint xyStt = GetXySectionSttEnd(TRUE, nLeft, bVertDir);
		CDPoint xyEnd = GetXySectionSttEnd(FALSE, nLeft, bVertDir);

		xyStt.y = GetElevationBoxBottom(0, 0, FALSE, bVertDir);
		xyEnd.y = GetElevationBoxBottom(m_nQtyJigan, 0, FALSE, bVertDir);

		xyStt.y += dOffset;
		xyEnd.y += dOffset;

		GetXyMatchLineAndLine(xyStt, (xyEnd-xyStt).Unit(), xy, xyDir, xyMatch);

		m_nTypeJongdanForRebar	= nTypeJongdan;
	}

	return xyMatch;
}

// 슬래브 
// nLeft : 선형 -1 왼쪽 선형, 0 : 중심선형, 1 : 오른쪽 선형
// dOffset : 상부에서의 간격
// BOOL bDistIsHor : 수평으로 dDist만큼 나간 거리
// FALSE -> 종단길이로 dDist만큼 나간 거리
CDPoint CRcBridgeApp::GetXyToDistFromSlabUpper(double dStt, double dDist, long nLeft, double dOffset, BOOL bVertDir, BOOL bDistIsHor)
{
	dOffset = Compare(dOffset, -1.0, "=") ? m_dTS : dOffset;
	CDPoint xyOrg = GetXyMatchLineAndSlabUpper(CDPoint(dStt, 0), CDPoint(0, 1), nLeft, dOffset, bVertDir);
	CDPoint xyMatch(0, 0);
	CLineInfo *pLine	= GetLineLeftCenRight(nLeft, bVertDir);
	//CLineInfo *pLineCR	= GetLineLeftCenRight(0, bVertDir);

	if(m_nTypeJongdanForRebar == 2 && pLine)
	{
		if(bDistIsHor)
		{
			xyMatch.x	= dStt+dDist;
			xyMatch.y	= GetXySlabUpperByLenFrSttStation(dStt+dDist-m_dStationBridgeStt, nLeft, FALSE).y;// pLine->GetElevationPlanEl(dStt+dDist);
			xyMatch.y	-= dOffset;	// 계획고가 아닌 슬래브 상면 리턴
		}
		else
		{
			double dStaStt	= GetStationOnJijum(0, 0, FALSE);
			//CDPoint xyStt	= pLine->GetXyLine(dStaStt);
			//pLineCR->GetStationMatchVerticalByXy(xyStt, dStaStt);
			double dGiul	= pLine->GetJongDanGiul(dStt);
			double dRad		= atan(dGiul);
			CDPoint xyDir	= ToDPointFrRadian(dRad);
			CDPoint xyOrg	= GetXySlabUpperByLenFrSttStation(dStt-dStaStt, nLeft, FALSE);
			xyOrg	= xyOrg + (dDist * xyDir);
			xyMatch	= GetXySlabUpperByLenFrSttStation(xyOrg.x-dStaStt, nLeft, FALSE);
			xyMatch.y	-= dOffset;
		}
	}
	else
	{
		CDPoint xyStt = GetXySectionSttEnd(TRUE, nLeft, bVertDir);
		CDPoint xyEnd = GetXySectionSttEnd(FALSE, nLeft, bVertDir);
		CDPoint xyDir = (xyEnd-xyStt).Unit();
		
		if(bDistIsHor)
			GetXyMatchLineAndLine(xyOrg, xyDir, CDPoint(xyOrg.x+dDist, xyOrg.y), CDPoint(0, 1), xyMatch);
		else
			xyMatch = xyOrg + (dDist * xyDir);
	}

	return xyMatch;
}

CDPoint CRcBridgeApp::GetXyToDistFromSlabLower(double dStt, double dDist, long nLeft, double dOffset, BOOL bVertDir, BOOL bDistIsHor)
{
	dOffset = Compare(dOffset, -1.0, "=") ? m_dTS : dOffset;
	CDPoint xyOrg = GetXyMatchLineAndLowerSlab(CDPoint(dStt, 0), CDPoint(0, 1), nLeft, dOffset, bVertDir);
	CDPoint xyMatch(0, 0);
	CLineInfo *pLine	= GetLineLeftCenRight(nLeft, bVertDir);
	//CLineInfo *pLineCR	= GetLineLeftCenRight(0, bVertDir);

	if(m_nTypeJongdanForRebar == 2 && pLine)
	{
		if(bDistIsHor)
		{
			xyMatch.x	= dStt+dDist;
			xyMatch.y	= GetxyLowerSlabBottomByLenFrSttStation(dStt+dDist-m_dStationBridgeStt, nLeft, FALSE).y;// pLine->GetElevationPlanEl(dStt+dDist);
			xyMatch.y	-= dOffset;	// 계획고가 아닌 슬래브 상면 리턴
		}
		else
		{
			double dStaStt	= GetStationOnJijum(0, 0, FALSE);
			//CDPoint xyStt	= pLine->GetXyLine(dStaStt);
			//pLineCR->GetStationMatchVerticalByXy(xyStt, dStaStt);
			double dGiul	= pLine->GetJongDanGiul(dStt);
			double dRad		= atan(dGiul);
			CDPoint xyDir	= ToDPointFrRadian(dRad);
			CDPoint xyOrg	= GetxyLowerSlabBottomByLenFrSttStation(dStt-dStaStt, nLeft, FALSE);
			xyOrg	= xyOrg + (dDist * xyDir);
			xyMatch	= GetxyLowerSlabBottomByLenFrSttStation(xyOrg.x-dStaStt, nLeft, FALSE);
			xyMatch.y	-= dOffset;
		}
	}
	else
	{
		double dLengthBridge	= fabs(GetXySectionSttEnd(FALSE, nLeft, bVertDir).x-GetXySectionSttEnd(TRUE, nLeft, bVertDir).x);
		CDPoint xyStt= GetxyLowerSlabBottomByLenFrSttStation(0, nLeft, bVertDir);
		CDPoint xyEnd= GetxyLowerSlabBottomByLenFrSttStation(dLengthBridge, nLeft, bVertDir);
		CDPoint xyDir = (xyEnd-xyStt).Unit();

		if(bDistIsHor)
			GetXyMatchLineAndLine(xyOrg, xyDir, CDPoint(xyOrg.x+dDist, xyOrg.y), CDPoint(0, 1), xyMatch);
		else
			xyMatch = xyOrg + (dDist * xyDir);
	}

	return xyMatch;
}

// 직선과 슬래브하면이 dDistFromSlab만큼 떨어진 곳과의 교차점 찾기
CDPoint CRcBridgeApp::GetXyMatchLineAndSlabeLowerToDist(double dDistFromSlab, CDPoint xy, CDPoint xyDir, double dOffset, long nLeft)
{
	CDPoint xyMatch(0, 0);
	dOffset = Compare(dOffset, -1.0, "=") ? m_dTS : dOffset;
// 	if(m_nTypeJongdanForRebar == 2)
// 	{
		CDPoint xyStt = GetXySectionSttEnd(TRUE, nLeft);
		CDPoint xyEnd = GetXySectionSttEnd(FALSE, nLeft);
		Offset(xyStt, xyEnd, dOffset);

		xyStt.y += dDistFromSlab;
		xyEnd.y += dDistFromSlab;

		GetXyMatchLineAndLine(xy, xyDir, xyStt, (xyEnd-xyStt).Unit(), xyMatch);
// 	}
// 	else 
// 	{
// 		CDPoint xyStt = GetXySectionSttEnd(TRUE, nLeft);
// 		CDPoint xyEnd = GetXySectionSttEnd(FALSE, nLeft);
// 		Offset(xyStt, xyEnd, dOffset);
// 
// 		xyStt.y += dDistFromSlab;
// 		xyEnd.y += dDistFromSlab;
// 
// 		GetXyMatchLineAndLine(xy, xyDir, xyStt, (xyEnd-xyStt).Unit(), xyMatch);
// 	}

	return xyMatch;
}

// 헌치의 정보 파악
// 해당 벽체 없으면 0으로 셋팅
void CRcBridgeApp::GetInfoHunch(long nIdxHunch, long &nIdxInWall, BOOL &bLeft)
{
	long i = 0; for(i = 0; i < GetCountInWall(); i++)
	{
		if(nIdxHunch == GetIdxHunchInWall(i, TRUE))
		{
			nIdxInWall = i;
			bLeft = TRUE;
			return;
		}
		else if(nIdxHunch == GetIdxHunchInWall(i, FALSE))
		{
			nIdxInWall = i;
			bLeft = FALSE;
			return;
		}
	}

	nIdxInWall = 0;
	bLeft = TRUE;
}


// 지간수만큼 벽체를 만들어야 됨.
// 모자라면 추가하고, 남으면 지워버림
// 기존 데이타는 최대한 보존되도록 함.
BOOL CRcBridgeApp::KeepInWallCount()
{
	// 정확한 벽체개수를 세기위해 교량에 따른 벽체의 형식을 먼저 알아야 된다.
	SetDataDefaultInWallType();

	long nCountNeed = m_nQtyJigan-1;
	long nQtyHDan = GetCountInWall();
	if(nCountNeed == nQtyHDan) 
	{
		KeepHunchCount();
		return TRUE;
	}

	if(nCountNeed > nQtyHDan)
	{
		long n = nCountNeed - nQtyHDan;
		long i = 0; for(i = 0; i < n; i++)
		{
			MakeInWall();
			SetDataDefaultShapeInWall(GetCountInWall()-1);	// 새로 만드는 것들은 권고안 해줌.
		}
	}
	else
	{
		long n = nQtyHDan - nCountNeed;
		long i = 0; for(i = 0; i < n; i++)
			RemoveInWall(GetCountInWall()-1);
	}

	KeepHunchCount();
	return FALSE;
}

// 헌치의 개수를 내부 벽체에 맞게 유지해줌
// 남으면 초기화 하고
// 모자라면 필요한 갯수만 설정해줌. (값은 나중에)
BOOL CRcBridgeApp::KeepHunchCount()
{
	long nQtyHDan = m_nCountInWallHunch;
	long nCountNeed = GetCountInWall() > 0 ? GetIdxHunchInWall(GetCountInWall()-1, FALSE)+1 : 0;
	// 슬래브교일때는 내측벽체가 없으므로 따로 설정
	if(m_nTypeBridge == BRIDGE_TYPE_SLAB)
		nCountNeed	= m_nQtyJigan-1;
	if(nQtyHDan == nCountNeed) return TRUE;
	

	if(nQtyHDan > nCountNeed)
	{
		long i = 0; for(i = nCountNeed; i < nQtyHDan; i++)
			InitHunch(i);

		m_nCountInWallHunch = nCountNeed;
	}
	else
	{
		m_nCountInWallHunch = nCountNeed;
	}

	// 헌치의 높이가 0인것이 나오면 헌치만 권고안 해준다.
	long i = 0; for(i = 0; i < m_nCountInWallHunch; i++)
	{
		if(Compare(GetHeightHunch(i), 0.0, "=")) SetDataDefaultShapeHunch(i);
	}


	return FALSE;
}


// 형상 권고안 : 내측벽체
BOOL CRcBridgeApp::SetDataDefaultShapeInWall(long nIdxWall, BOOL bOnlyDefaultHunch, long nLeft)
{
	CWallApp *pWall = GetInWall(nIdxWall);
	if(!pWall) return FALSE;
	CLineInfoApp *pLine = m_pLineApp;
	if(!pLine) return FALSE;

	if(!bOnlyDefaultHunch)
	{
		// 교량타입에 따라서 내측벽체의 종류가 정해진다.
		if(m_nTypeBridge == BRIDGE_TYPE_PORTAL || m_nTypeBridge == BRIDGE_TYPE_PI || m_nTypeBridge == BRIDGE_TYPE_BOX || m_nTypeBridge == BRIDGE_TYPE_ARCH)
		{
			pWall->m_nType = WALL_TYPE_VER;
		}
		else if(m_nTypeBridge == BRIDGE_TYPE_TRAP || m_nTypeBridge == BRIDGE_TYPE_TRAP2)
		{
			pWall->m_nType = WALL_TYPE_SLOPE;
		}
		else if(m_nTypeBridge == BRIDGE_TYPE_V)
		{
			pWall->m_nType = WALL_TYPE_V;
		}
		else if(m_nTypeBridge == BRIDGE_TYPE_SLAB)
		{
			pWall->m_bIs = FALSE;
		}


		double dEL = GetXySectionSttEnd(TRUE, nLeft).y;
		double dELEarth = 0;
		if(pLine->GetElevationErathOnStation(GetStationOnJijum(nIdxWall+1, 0), dELEarth))
				pWall->m_dELB	= dELEarth - dDepthFromEL;
		else 
		{
			// 지반고를 찾지 못할경우 시점의 지반고를 기준으로 다시 한번 더 찾아 본다.(캬.. 이 배려..)
			if(pLine->GetElevationErathOnStation(m_dStationBridgeStt, dELEarth))
				pWall->m_dELB	= dELEarth - dDepthFromEL;
			else
				pWall->m_dELB	= dEL-5000;
		}
		double dH = GetXySectionSttEnd(TRUE).y - (pWall->m_dELB - dDepthFromEL);
		if(dH > dMaxHeight)		// 권고안시 교량의 최대 높이 적용
		{
			pWall->m_dELB	= dEL - dMaxHeight;
			dH = dMaxHeight;
		}
		if(dH < dMinHeight)	// 권고안시 교량의 최소 높이 적용
		{
			pWall->m_dELB	= dEL - dMinHeight;
			dH	= dMinHeight;
		}

		// 벽체의 두께(내측벽체)
		// 1. 0.035H^1.5 > 0.8

		if(pWall->m_bIs)
		{
			SetOrgInWall(nIdxWall, nLeft);
			if(m_nTypeBridge == BRIDGE_TYPE_ARCH)
			{
				if(nIdxWall+1 == GetJijumConnectRibAndSlab())
					pWall->m_dW	= Round(m_dLengthBridge/5, -3);
				else
					pWall->m_dW	= 700;
				pWall->m_dAngle	= 90;
			}
			else
			{
				pWall->m_dAngleV	= 70;
				pWall->m_dH			= 2000;
				pWall->m_dAngle		= 90;
				if(pWall->m_nType == WALL_TYPE_SLOPE)
				{
					double dAngleJongdan	= GetAngleJongdan(pWall->m_xyOrg.x);
					if(nIdxWall == 0)
						pWall->m_dAngle	= 60+dAngleJongdan;
					else if(nIdxWall == GetCountInWall()-1)
						pWall->m_dAngle	= 120+dAngleJongdan;
					else
						pWall->m_dAngle	= 90-dAngleJongdan;
				}
				pWall->m_dW			= Round(0.035*pow(toM(dH), 1.5)*1000, -2);
				if(pWall->m_dW < 800) pWall->m_dW	= 800;
				pWall->m_dWL		= Round(pWall->m_dW/1.5, -2);
				pWall->m_dWR		= Round(pWall->m_dW/1.5, -2);

				if(pWall->m_nType == WALL_TYPE_SLOPE)
				{
					// 경사형은 벽체 두께를 외측 벽체 권고안으로 한다.
					pWall->m_dW	= IsOutWall(TRUE) ? m_dWS : pWall->m_dW;
				}
			}
		}
		else
		{
			pWall->m_dW			= Round(0.035*pow(toM(dH), 1.5)*1000, -2);
			if(pWall->m_dW < 800) pWall->m_dW	= 800;
		}
	}

	long nIdxHunch = GetIdxHunchInWall(nIdxWall, TRUE);
	SetDataDefaultShapeHunch(nIdxHunch);

	if(pWall->m_nType == WALL_TYPE_V)
	{
		SetDataDefaultShapeHunch(nIdxHunch+1);
	}

	SetDatadefaultShapeInWallHunch(nIdxWall);

	// 기초 권고안도 같이 함.
	// 박스교일 경우 기초가 없다.
	if(!bOnlyDefaultHunch)
	{
		if(m_nTypeBridge == BRIDGE_TYPE_BOX || m_nTypeBridge == BRIDGE_TYPE_SLAB) 
		{
			pWall->m_footing.m_bIs = FALSE;
			pWall->m_footing.m_bVirtualFooting = FALSE;
			pWall->m_footing.m_exFooting.m_bIs = FALSE;

			// 기둥 배근을 위해서 기초에 피복만 넣어주자.
			// 엔진에서 기초가 없는데도 배근할때 기초의 피복을 쓰네..
			pWall->m_footing.m_dCoverLower[0] = m_dCoverLower_LowerSlab[0];
		}
	}

	return TRUE;
}

// 형상 권고안 : 교량 및 기초
BOOL CRcBridgeApp::SetDataDefaultShapeBridge(BOOL bSttWall, long nLeft)
{
	CLineInfoApp *pLine = m_pLineApp;
	if(!pLine) return FALSE;

	// 시종점 제원 권고안시 평면 제원 다시 구해 주도록 해야 됨.
	m_bSettingPlane	= FALSE;
	m_bNeedSettingPlane	= TRUE;

	// 외측벽체/기초 타입 결정
	SetDataDefaultOutWallType();

	if(!IsOutWall(bSttWall)) InitWall(bSttWall);
	
	if(bSttWall)
	{
		double dEL = 0;
		if(pLine->GetElevationErathOnStation(m_dStationBridgeStt, dEL))
			m_dELSB	= dEL-dDepthFromEL;
		else 
			m_dELSB = GetXySectionSttEnd(TRUE, nLeft).y-5000;
	
		if(!IsOutWall(bSttWall))
		{
			m_dELSB = GetXySectionSttEnd(TRUE, nLeft).y - m_dTS;
			m_dHHS	= 0;
		}
		else
		{
			double dH = GetXySectionSttEnd(TRUE, nLeft).y - m_dELSB;
			if(dH > dMaxHeight)		// 권고안시 교량의 최대 높이 적용
			{
				m_dELSB = GetXySectionSttEnd(TRUE, nLeft).y - dMaxHeight;
				dH = dMaxHeight;
			}
			if(dH < dMinHeight)	// 권고안시 교량의 최소 높이 적용
			{
				m_dELSB = GetXySectionSttEnd(TRUE, nLeft).y - dMinHeight;
				dH	= dMinHeight;
			}

			// 벽체의 두께(내측벽체)
			// 1. 0.035H^1.5 > 0.8m
			double dW	= 0.035 * pow(toM(dH), 1.5) * 1000;
			if(dW < 800) dW	= 800;

			m_dBHS	= dSizeBracket+dSizeBracket;
			m_dBTHS	= dDepthToBracket;
			m_dBWS	= dSizeBracket;
			m_dBHHS	= dSizeBracket;
			m_dBHWS	= dSizeBracket;
			m_dHHS	= dHeightHunch;
			m_dHWS	= m_nTypeBridge == BRIDGE_TYPE_SLAB ? m_dHHS * 5 : dWidthHunch;
			m_dWS	= Round(dW, -2);
			if(m_nTypeBridge == BRIDGE_TYPE_TRAP2 || m_nTypeBridge == BRIDGE_TYPE_ARCH)
			{
				m_dWSHH	= 900;
				m_dWSHW	= 300;
			}
			else if(m_nTypeBridge == BRIDGE_TYPE_BOX)
			{
				m_dWSHH	= m_dHHS;
				m_dWSHW	= m_dHWS;
			}
			else
			{
				m_dWSHH	= 0;
				m_dWSHW	= 0;
			}
		}
	}
	else
	{
		double dEL = 0;
		if(pLine->GetElevationErathOnStation(m_dStationBridgeStt+m_dLengthBridge, dEL))
			m_dELEB	= dEL-dDepthFromEL;
		else 
			m_dELEB = GetXySectionSttEnd(FALSE, nLeft).y-5000;
		
		if(!IsOutWall(FALSE))
		{
			m_dELEB = GetXySectionSttEnd(FALSE, nLeft).y - m_dTS;
			m_dHHE	= 0;
		}
		else
		{
			double dH = GetXySectionSttEnd(FALSE, nLeft).y - m_dELEB;
			if(dH > dMaxHeight)		// 권고안시 교량의 최대 높이 적용
			{
				m_dELEB = GetXySectionSttEnd(FALSE, nLeft).y - dMaxHeight;
				dH = dMaxHeight;
			}
			if(dH < dMinHeight)	// 권고안시 교량의 최소 높이 적용
			{
				m_dELEB = GetXySectionSttEnd(FALSE, nLeft).y - dMinHeight;
				dH	= dMinHeight;
			}

			// 벽체의 두께(내측벽체)
			// 1. 0.035H^1.5 > 0.8m
			double dW	= 0.035 * pow(toM(dH), 1.5) * 1000;
			if(dW < 800) dW	= 800;

			m_dBHE	= dSizeBracket+dSizeBracket;
			m_dBTHE	= dDepthToBracket;
			m_dBWE	= dSizeBracket;
			m_dBHHE	= dSizeBracket;
			m_dBHWE	= dSizeBracket;
			m_dHHE	= dHeightHunch;
			m_dHWE	= m_nTypeBridge == BRIDGE_TYPE_SLAB ? m_dHHE * 5 : dWidthHunch;
			m_dWE	= Round(dW, -2);
			if(m_nTypeBridge == BRIDGE_TYPE_TRAP2 || m_nTypeBridge == BRIDGE_TYPE_ARCH)
			{
				m_dWEHH	= 900;
				m_dWEHW	= 300;
			}
			else if(m_nTypeBridge == BRIDGE_TYPE_BOX)
			{
				m_dWEHH	= m_dHHE;
				m_dWEHW	= m_dHWE;
			}
			else
			{
				m_dWEHH	= 0;
				m_dWEHW	= 0;
			}
		}
	}

	// BLOCK OUT
	long nJijum = bSttWall ? 0 : m_nQtyJigan;
	if(m_bExpJoint[nJijum])
	{
		CExpansionJointData *pExp = m_pDesignExpanJoint->GetExpansionJointData(bSttWall ? 0 : 1);
		if(pExp)
		{
			CFlexibleJoint *pFx = bSttWall ? &m_fxJointStt : &m_fxJointEnd;

			pFx->m_dHeight = *pExp->m_pBlockH[bSttWall ? 1 : 0];
			pFx->m_dWidth = *pExp->m_pBlockW[bSttWall ? 1 : 0]; 
		}
	}

	m_bExistProtectionRiseFooting = FALSE;
	m_dPRF_W = 1000;						
	m_dPRF_H1 = 500;						
	m_dPRF_H2 = 500;						

	return TRUE;
}

// 외측벽체 타입 설정
void CRcBridgeApp::SetDataDefaultOutWallType()
{
	if(IsOutsideWall())
	{
		m_nTypeOutWall = 2;
		// 박스일때는 기초가 없고 대신 하부 슬래브 형식을 사용한다.
		if(m_nTypeBridge == BRIDGE_TYPE_BOX)
		{
			m_footingStt.m_bIs = FALSE;
			m_footingEnd.m_bIs = FALSE;
			m_footingBox.m_bIs = TRUE;
		}
		else
		{
			m_footingStt.m_bIs = TRUE;
			m_footingEnd.m_bIs = TRUE;
			m_footingBox.m_bIs = FALSE;
		}
	}
	else 
	{
		m_nTypeOutWall = 0;
		m_footingStt.m_bIs = FALSE;
		m_footingEnd.m_bIs = FALSE;
		m_footingBox.m_bIs = FALSE;
	}
}

// 내부벽체 타입을 설정해줌.
void CRcBridgeApp::SetDataDefaultInWallType()
{
	long nType = WALL_TYPE_VER;
	if(m_nTypeBridge == BRIDGE_TYPE_PORTAL || m_nTypeBridge == BRIDGE_TYPE_PI || m_nTypeBridge == BRIDGE_TYPE_BOX || m_nTypeBridge == BRIDGE_TYPE_ARCH)
		nType = WALL_TYPE_VER;
	else if(m_nTypeBridge == BRIDGE_TYPE_V)
		nType = WALL_TYPE_V;
	else if(m_nTypeBridge == BRIDGE_TYPE_TRAP || m_nTypeBridge == BRIDGE_TYPE_TRAP2)
		nType = WALL_TYPE_SLOPE;

	long i = 0; for(i = 0; i < GetCountInWall(); i++)
	{
		CWallApp *pWall = GetInWall(i);
		if(!pWall) continue;
		
		if(m_nTypeBridge == BRIDGE_TYPE_SLAB) 
		{
			pWall->m_bIs = FALSE;
			continue;
		}
		pWall->m_nType = nType;	

		//31720
		CTwinVectorArray tvArrVertSection;
		tvArrVertSection.AddFromTvArray(m_tvArrVertSection);

		// 벽체 타입 설정하면 싱크한번 해 줘야지..
		pWall->SyncWall(tvArrVertSection);
	}
}

// 교량의 지점 초기값 생성
void CRcBridgeApp::SetDataInitJijum()
{
	int nCountJijum = m_nQtyJigan + 1;
	MakeJijum(nCountJijum);
}

//<summary>
//교량에서 슈의 열수 총합을 구한다.
//
//</summary>
//<return></return>
int CRcBridgeApp::GetCountJijumShoe()
{
	int nCountJijumShoe = 0;
	int nCountJijum = GetCountJijum();

	int nJ=0; for(nJ=0; nJ<nCountJijum; nJ++)
	{
		CJijum* pJ = NULL;
		pJ = GetJijum(nJ);

		if(pJ->m_strJijumShoe[0]!=_T(""))
			nCountJijumShoe++;

		if(pJ->m_strJijumShoe[1]!=_T(""))
			nCountJijumShoe++;
	}

	return nCountJijumShoe;
}

//지점들중 슈의 최대값
long CRcBridgeApp::GetMaxShoe()
{
	CJijum* pJ = NULL;

	long nMax = -999;
	long nCountShoe = 0;
	long nCountJijum = GetCountJijum();

	for(long nJ=0; nJ<nCountJijum; nJ++)
	{
		pJ = GetJijum(nJ);

		for(long nLeft = 0; nLeft < 2; nLeft++) 
		{
			if(pJ->m_strJijumShoe[nLeft] != _T(""))
			{
				nCountShoe = pJ->m_nQtyShoe[nLeft];
				if(nCountShoe > nMax)
					nMax = nCountShoe;
			}
		}
	}

	return nMax;
}

CString CRcBridgeApp::GetStrShoeDirection(long Dir)
{
	CString str;
	switch(Dir)
	{
	case 0: str = "고정단";		break;
	case 1: str = "교축방향";	break;
	case 2: str = "직각방향";	break;
	case 3: str = "양방향";		break;		
	}

	return str;
}

//지점들중에서 슈가있는 지점 개수
long CRcBridgeApp::GetCountJijumBearing()
{
// 	CJijum* pJ = NULL;

	long nCount = 0;
	long nQtyJijum = GetCountJijum();

	for(long nJ = 0; nJ < nQtyJijum; nJ++)
	{
		CJijum* pJ = GetJijum(nJ);

		for(long nLeft = 0; nLeft < 2; nLeft++) 
		{
			if(pJ->m_nQtyShoe[nLeft] > 0)
			{
				nCount++;
			}
		}
	}

	return nCount;
}

void CRcBridgeApp::SetDataDefaultCountShoe()
{
	double dEndDist = 1500;
	double dStdSpace = 2400.0;
	
	long nCountJijum = GetCountJijum();
	for(long nJ=0; nJ<nCountJijum; nJ++)
	{
		CJijum* pJ = NULL;
		pJ = GetJijum(nJ);
		for(long nLeft=0; nLeft<2; nLeft++)
		{
			pJ->m_nQtyShoe[nLeft] = 0;
			if(pJ->m_strJijumShoe[nLeft]!=_T(""))
			{
				double dSta = GetStationShoeOnJijum(nJ, 0, nLeft);
				double dSlabWidth = GetWidthSlabAct(dSta, CDPoint(0, 1));
				long nCountShoe = (long)RoundUp((dSlabWidth-dEndDist*2)/dStdSpace, 0);
				pJ->m_nQtyShoe[nLeft] = nCountShoe;
			}
		}
	}
}

void CRcBridgeApp::SetDataDefaultGagak()
{
	m_dExpSttW[0] = 0;
	m_dExpSttH[0] = 0;
	m_dExpSttR[0] = 0;
	m_dExpEndW[0] = 0;
	m_dExpEndH[0] = 0;
	m_dExpEndR[0] = 0;
	m_dExpSttW[1] = 0;
	m_dExpSttH[1] = 0;
	m_dExpSttR[1] = 0;
	m_dExpEndW[1] = 0;
	m_dExpEndH[1] = 0;
	m_dExpEndR[1] = 0;
}

void CRcBridgeApp::SetDataDefaultExpWidth()
{
	m_dDisExtLineSttLeft	= 0;
	m_dDisExtLineSttRight	= 0;
	m_dDisExtLineEndLeft	= 0;
	m_dDisExtLineEndRight	= 0;
}

void CRcBridgeApp::MakeExpJoint()
{
	CDesignExpansionJoint *pDesignExp	= m_pDesignExpanJoint;
	pDesignExp->RemoveAll();

	for(long nSE=iSTT; nSE<=iEND; nSE++)
	{
		if(IsOutWall(nSE == iSTT)) continue;

		CExpansionJointData *pExpData = new CExpansionJointData;
		pDesignExp->InsertBridge(pExpData);

		SetExpJoint(hggettext("UCF-150S"), nSE==iSTT, TRUE);
	}
}

void CRcBridgeApp::SetExpJoint(CString szExpRefName, BOOL bStt, BOOL bRefSetting)
{
	CDesignExpansionJoint	*pDesignExp	= m_pDesignExpanJoint;
	CExpJointRef *pExpJointRef	= m_pARcBridgeDataStd->m_pExpJointRefArr->GetExpJointRef(szExpRefName);
	if(m_pARcBridgeDataStd->m_pExpJointRefArr->GetSize() == 0) return;

	long nExp = bStt ? iSTT : iEND;
	CExpansionJointData *pExpData = pDesignExp->GetExpansionJointData(nExp);
	if(pExpData == NULL) return;

	CJijum *pJ = GetJijum(bStt ? 0 : GetCountJijum() - 1);

	long nSection  = nExp == 0 ? POST_SECTION : PRE_SECTION;
	long nSection2 = nExp == 0 ? PRE_SECTION : POST_SECTION;

	pExpData->m_strName		= bStt ? _T("A1") : _T("A2");
	pExpData->m_bIsAbut		= m_nExpJointType[bStt ? iSTT : iEND] == TYPE_ABUT ? TRUE : FALSE;
	pExpData->m_nRegion		= NORMAL_REGION;
	pExpData->m_nTypeBridge[nSection]	= RC_BRIDGE;
	pExpData->m_nPosition		= nSection;

	if(bRefSetting)
	{
		pExpData->m_strExpanJoint	= szExpRefName;	
		pExpData->m_dMaxLayingGap	= pExpJointRef->m_dUganMax;
		pExpData->m_dMinLayingGap	= pExpJointRef->m_dUganMin;
		pExpData->m_dMaxAllowExp	= pExpJointRef->m_dMaxAllowExpL;	
	}

	pExpData->m_pBlockW[PRE_SECTION]	= &pJ->m_dWidBlockOut[PRE_SECTION];
	pExpData->m_pBlockH[PRE_SECTION]	= &pJ->m_dHeightBlockOut[PRE_SECTION];
	pExpData->m_pBlockW[POST_SECTION]	= &pJ->m_dWidBlockOut[POST_SECTION];
	pExpData->m_pBlockH[POST_SECTION]	= &pJ->m_dHeightBlockOut[POST_SECTION];

	long nVerPos = bStt ? 1 : 0;
	pExpData->m_nTypeBridge[PRE_SECTION] = RC_BRIDGE;
	pExpData->m_dDispEarthQuake[nSection]	= 0;
	pExpData->m_dEstablishLayingGap	 = pJ->m_dLengthUgan[nVerPos];
	pExpData->m_dHeightGirder[nSection]	= m_dTS;
	pExpData->m_dLengthGirder[nSection] = m_dLengthJigan[bStt ? 0 : m_nQtyJigan - 1]; // 고정점까지의 거리
	if(m_nTypeBridge == BRIDGE_TYPE_SLAB)
	{
		pExpData->m_dLengthGirder[nSection] = 0;
		if(bStt)
		{
			for(long ix = 0; ix < m_nQtyJigan; ix++)
			{
				CJijum *pJ = GetJijum(ix);
				if(pJ->IsFixJijum())
				{
					break;
				}
				pExpData->m_dLengthGirder[nSection] += m_dLengthJigan[ix];
			}
		}
		else
		{
			for(long ix = m_nQtyJigan; ix > 0; ix--)
			{
				CJijum *pJ = GetJijum(ix);
				if(pJ->IsFixJijum())
				{
					break;
				}
				pExpData->m_dLengthGirder[nSection] += m_dLengthJigan[ix - 1];
			}
		}
	}
	//
	if(bRefSetting)
	{
		pJ->m_dWidBlockOut[nSection]		= pExpJointRef->m_dBlockW;
		pJ->m_dHeightBlockOut[nSection]		= pExpJointRef->m_dBlockH;
	}
	pJ->m_dWidBlockOut[nSection2]		= 0;
	pJ->m_dHeightBlockOut[nSection2]	= 0;

	pDesignExp->SetExpansionJoint(nExp);
}

void CRcBridgeApp::SetDataDefaultDistShoe()
{
	CJijum* pJ = NULL;
	int nCountJijum = GetCountJijum();
	double dEndDist = 1500;
	
	int nJ=0; for(nJ=0; nJ<nCountJijum; nJ++)
	{
		pJ = GetJijum(nJ);
		for(int nLeft=0; nLeft<2; nLeft++)
		{
			if(pJ->m_strJijumShoe[nLeft]!=_T(""))
			{
				int nCountShoe = pJ->m_nQtyShoe[nLeft];
				if(nCountShoe<1) return;
				
				double dSta = GetStationShoeOnJijum(nJ, 0, nLeft);
				double dSlabWidth = GetWidthSlabAct(dSta, CDPoint(0, 1)) - dEndDist*2;
				double dLeftDist = GetDisSlabLeftAct(dSta, CDPoint(0, 1)) + dEndDist;
				double dSpace = dSlabWidth/(nCountShoe-1);

				for(int nRow=0; nRow<nCountShoe; nRow++)
					pJ->m_dDistShoe[nLeft][nRow] = Round(dSpace*nRow+dLeftDist, 0);
			}
		}
	}
}

// 교량 형식에 따른 신축 이음 및 지점 각도를 설정
void CRcBridgeApp::SetDataDefaultJijumAngle()
{
	BOOL bExpJointAbut = !IsOutsideWall();

	long nCountJijum = GetCountJijum();
	CDPoint vAng(0, 1);

	CString str = _T("");
	for(long nJ=0; nJ<nCountJijum; nJ++)
	{
		CJijum* pJ = NULL;
		pJ = GetJijum(nJ);
		pJ->SetAngJijum(vAng);

		if(nJ==0 || nJ==nCountJijum-1)
			m_bExpJoint[nJ] = bExpJointAbut;
		else
			m_bExpJoint[nJ] = FALSE;

		pJ->m_strJijumShoe[0] = _T("");
		pJ->m_strJijumShoe[1] = _T("");
		if(m_bExpJoint[nJ])
		{
			if(nJ!=0)
			{
				if(nJ==nCountJijum-1)
					str = _T("종점(A2)");
				else
					str.Format(_T("P%dL"), nJ);
				pJ->m_strJijumShoe[0] = str;
			}
			if(nJ!=nCountJijum-1)
			{
				if(nJ==0)
					str = _T("시점(A1)");
				else
					str.Format(_T("P%dR"), nJ);
				pJ->m_strJijumShoe[1] = str;
			}
		}
		else
		{
			if(nJ!=0)
				pJ->m_strJijumShoe[0] = _T("");
			if(nJ!=nCountJijum-1)
				pJ->m_strJijumShoe[1] = _T("");
		}
	}
}

// 지간 거리를 설정
void CRcBridgeApp::SetDataDefaultJijumLength()
{
	double dBriLength = m_dLengthBridge;
	CDPoint vAng(0, 1);

	int nCountJijum = GetCountJijum();
	
	int nJ=0; for(nJ=0; nJ<nCountJijum; nJ++)
	{
		CJijum* pJ = NULL;
		pJ = GetJijum(nJ);

		pJ->m_dLengthUgan[0] = 0.0;
		pJ->m_dLengthSlabToShoe[0] = 0.0;
		pJ->m_dLengthUgan[1] = 0.0;
		pJ->m_dLengthSlabToShoe[1] = 0.0;

		if(m_bExpJoint[nJ])
		{
			pJ->m_dLengthUgan[0] = 80.0;
			pJ->m_dLengthSlabToShoe[0] = 450.0;
			pJ->m_dLengthUgan[1] = 80.0;
			pJ->m_dLengthSlabToShoe[1] = 450.0;

			if(nJ==0)
			{
				pJ->m_dLengthUgan[0] = 0.0;
				pJ->m_dLengthSlabToShoe[0] = 0.0;
			}
			if(nJ==nCountJijum-1)
			{
				pJ->m_dLengthUgan[1] = 0.0;
				pJ->m_dLengthSlabToShoe[1] = 0.0;
			}
		}
	}

	double dUgan[2], dShoe[2];
	int nCountJigan = m_nQtyJigan;
	CJijum* pJL = NULL;
	CJijum* pJR = NULL;
	m_arrJijumLen.RemoveAll();

	for(nJ=0; nJ<nCountJigan; nJ++)
	{
		pJL = GetJijum(nJ);
		pJR = GetJijum(nJ+1);

		dUgan[0] = pJL->m_dLengthUgan[1];
		dShoe[0] = pJL->m_dLengthSlabToShoe[1];
		dUgan[1] = pJR->m_dLengthUgan[0];
		dShoe[1] = pJR->m_dLengthSlabToShoe[0];
		m_dLengthJigan[nJ] = dBriLength/nCountJigan - (dUgan[0]+dShoe[0]+dUgan[1]+dShoe[1]);

		if(dUgan[0]>0)
			m_arrJijumLen.Add(dUgan[0]);
		if(dShoe[0]>0)
			m_arrJijumLen.Add(dShoe[0]);
		if(m_dLengthJigan[nJ]>0)
			m_arrJijumLen.Add(m_dLengthJigan[nJ]);
		if(dShoe[1]>0)
			m_arrJijumLen.Add(dShoe[1]);
		if(dUgan[1]>0)
			m_arrJijumLen.Add(dUgan[1]);
	}
}

// 슈 열의 각도
//<summary>
//본선 기준 슈 열의 각도를 구함.
//RcBridge 에서는 슈 자동계산은 모두 옵셋으로 처리함.
//</summary>
//<param name="nJijum"></param>
//<param name="bPrev">TRUE = 지점 이전 슈, FALSE = 지점 이후 슈</param>
//<return></return>
CDPoint CRcBridgeApp::GetAngleJijumShoe(int nJijum, int bPrev)
{
	CJijum* pJ = GetJijum(nJijum);
	double staJijum = GetStationOnJijum(nJijum, 0);
	CDPoint vAngJijum = pJ->m_vAngle;
	double  dLenFrJijumToShoe = -(pJ->m_dLengthUgan[0] + pJ->m_dLengthSlabToShoe[0]);
	if(!bPrev) 
		dLenFrJijumToShoe = pJ->m_dLengthUgan[1] + pJ->m_dLengthSlabToShoe[1];

	return GetLineBase()->GetAngleByOffset(staJijum, staJijum+dLenFrJijumToShoe, vAngJijum);
}

// 슬래브 시점(유간고려한 실제 슬래브)에서 내측벽체 까지의 거리를 계산해서 리턴함.
// double CRcBridgeApp::GetDistInWall(long nIdxInWall, long nLeft, BOOL bVertDir)
// {
// 	if(nIdxInWall > m_nQtyJigan) return 0;
// 
// 	double dDist = 0;
// 	CDPoint vAng	= CDPoint(0, 1);
// 	long i = 0; for(i = 0; i < nIdxInWall+1; i++)
// 	{
// 		dDist	+= GetLengthJigan(i, nLeft, bVertDir);
// 	}
// 
// 	// 시작위치에 유간 있는지 고려해줌
// 	CJijum *pJ	= GetJijum(0);
// 	if(pJ)
// 	{
// 		vAng	= bVertDir ? pJ->m_vAngle : CDPoint(0, 1);
// 		dDist -= (pJ->m_dLengthUgan[1] * vAng.y);
// 	}
// 
// 	return dDist;
// }

CLineInfo *CRcBridgeApp::GetLineByNumber(long nLine)
{ 
	return m_pLineApp->GetLine(nLine); 
}

CLineInfo *CRcBridgeApp::GetLineBySlab(long nLeft)
{ 
	if(nLeft == 0)
		return GetLineByNumber(m_nLineBase); 
	if(nLeft == -1)
		return GetLineByNumber(m_nLineLeft); 

	return GetLineByNumber(m_nLineRight); 
}

// BOOL bForPlacingMainRebar : 주철근 배근을 위한 선형(즉 슬래브 중심)
CLineInfo *CRcBridgeApp::GetLineBase(BOOL bLineBaseVert, BOOL bForPlacingMainRebar)
{ 
	if(bLineBaseVert || bForPlacingMainRebar)
	{
		MakeLineBaseVert(bLineBaseVert);
		return m_pLineBaseVert;
	}
	else
	{
		return GetLineByNumber(m_nLineBase); 
	}
}
//TRUE=좌측선형, FALSE=우측선형
CLineInfo *CRcBridgeApp::GetLineLeftRight(BOOL bLeft)
{ 
	if(bLeft)
		return GetLineByNumber(m_nLineLeft); 
	return GetLineByNumber(m_nLineRight); 
}
//-1=좌측선형, 0=기준선형, 1=우측선형
CLineInfo *CRcBridgeApp::GetLineLeftCenRight(long nLeft,BOOL bLineBaseVert, BOOL bForPlacingMainRebar)
{ 
	if(nLeft==0)		//기준선형
		return GetLineBase(bLineBaseVert, bForPlacingMainRebar); 
	else if(nLeft==-1)	//좌측슬래브 선형
		return GetLineLeftRight(TRUE);

	return GetLineLeftRight(FALSE); //우측 슬래브 선형
}

//<summary>
//확폭 교량인지를 점검
//</summary>
//<return>FALSE = 확폭이 아님, TRUE = 확폭임</return>
BOOL CRcBridgeApp::IsExtLine()
{
	if(GetLineBase() == GetLineLeftRight(TRUE) && GetLineBase() == GetLineLeftRight(FALSE))
		return FALSE;
	return TRUE; 
}

// 외측 벽체가 있는지 점검(문형 라멘교나 박스교일 때에만 외측벽체가 있음)
BOOL CRcBridgeApp::IsOutsideWall()
{
	if(IsOutWall(TRUE) && IsOutWall(FALSE)) return TRUE;
	return FALSE;
}
//<summary>
//시종점부의 평면 좌우선 데이터를 구함.
//</summary>
//<param name="nTarget"></param>
//enum TARGET_IN_ONE_SLAB{
//	BRIDGE_OFF_STT_STT, BRIDGE_OFF_STT_SLAB, BRIDGE_OFF_STT_SHOE
//	BRIDGE_OFF_END_END, BRIDGE_OFF_END_SLAB, BRIDGE_OFF_END_SHOE};
//<param name="A"></param>
void CRcBridgeApp::GetXySss(TARGET_IN_ONE_SLAB nTarget, CDPoint* A, BOOL bExp)
{
	CLineInfo  *pLineLeft = GetLineLeftRight(TRUE);
	CLineInfo  *pLineRight = GetLineLeftRight(FALSE);

	BOOL bIncludeExp = (nTarget==BRIDGE_OFF_STT_SLAB || nTarget==BRIDGE_OFF_END_SLAB) ? bExp : FALSE;
	double staLeft = GetStationByTarget(nTarget, -1, bIncludeExp);
	double staRight = GetStationByTarget(nTarget, 1, bIncludeExp);
	CDPoint vAngJ(0, 0);
	
	// 가각관련.
	BOOL bStt = TRUE;
	double dExpHeightLeft  = 0;
	double dExpHeightRight = 0;
	double dExpStaLeft		= 0;
	double dExpStaRight		= 0;

	if(m_bIsExp && bExp)
	{
		if(nTarget==BRIDGE_OFF_STT_STT || nTarget==BRIDGE_OFF_STT_SLAB) 
		{
			bStt = TRUE;
			vAngJ = GetAngleAzimuthJijum(0);
			if(nTarget==BRIDGE_OFF_STT_STT)
			{
				dExpHeightLeft  = IsGagakForLine(TRUE, TRUE) ? m_dExpSttH[0] * (1 - (m_dWS/vAngJ.y)/m_dExpSttW[0]) : 0;
				dExpHeightRight = IsGagakForLine(TRUE, FALSE) ? m_dExpSttH[1] * (1 - (m_dWS/vAngJ.y)/m_dExpSttW[1]) : 0;

				dExpStaLeft		= (m_dExpSttH[0] - dExpHeightLeft) / tan(vAngJ.GetAngleRadian());
				dExpStaRight	= (m_dExpSttH[1] - dExpHeightRight) / tan(vAngJ.GetAngleRadian());
			}
			else
			{
				dExpHeightLeft  = m_dExpSttH[0];
				dExpHeightRight = m_dExpSttH[1];
			}
		}
		if(nTarget==BRIDGE_OFF_END_END || nTarget==BRIDGE_OFF_END_SLAB) 
		{
			bStt = FALSE;
			vAngJ = GetAngleAzimuthJijum(m_nQtyJigan);
			if(nTarget==BRIDGE_OFF_END_END)
			{
				dExpHeightLeft = IsGagakForLine(FALSE, TRUE) ? m_dExpEndH[0] * (1 - (m_dWE/vAngJ.y)/m_dExpEndW[0]) : 0;
				dExpHeightRight  = IsGagakForLine(FALSE, FALSE) ? m_dExpEndH[1] * (1 - (m_dWE/vAngJ.y)/m_dExpEndW[1]) : 0;
				
				dExpStaLeft		= (m_dExpEndH[0] - dExpHeightLeft) / tan(vAngJ.GetAngleRadian());
				dExpStaRight	= (m_dExpEndH[1] - dExpHeightRight) / tan(vAngJ.GetAngleRadian());
			}
			else
			{
				dExpHeightLeft  = m_dExpEndH[0];
				dExpHeightRight = m_dExpEndH[1];
			}
		}
	}

	BOOL bNotWall	= nTarget == BRIDGE_OFF_STT_SLAB || nTarget == BRIDGE_OFF_END_SLAB;
	if(m_bIsExp && bExp && (nTarget == BRIDGE_OFF_STT_STT || nTarget == BRIDGE_OFF_END_END))
	{
		A[0] = pLineLeft->GetXyLineDis(staLeft - dExpStaLeft,   m_dWidthSlabLeft  - dExpHeightLeft);
		A[3] = pLineRight->GetXyLineDis(staRight + dExpStaRight, m_dWidthSlabRight + dExpHeightRight);

		if(!bNotWall)
		{
			vAngJ = GetAngleAzimuthJijum((nTarget==BRIDGE_OFF_STT_STT)? 0 : m_nQtyJigan);
			A[0] = pLineLeft->GetXyLineDis(staLeft, m_dWidthSlabLeft);
			A[3] = pLineRight->GetXyLineDis(staRight, m_dWidthSlabRight);
			CTwinVectorArray tvArrPlane;
			GetTvPlaneOfExp(tvArrPlane, bStt, TRUE, -1);
			CRebarPlacing rb;
			
			BOOL bMatch = FALSE;
			CDPointArray xyArrMatch;
			rb.GetXyMatchTvArrAndLine(tvArrPlane, A[0], vAngJ, xyArrMatch);
			if(xyArrMatch.GetSize() > 0)
			{
				A[0]	= xyArrMatch.GetAt(0);
				bMatch	= TRUE;
			}
			if(!bMatch)
			{
				GetTvPlaneOfExp(tvArrPlane, !bStt, TRUE, -1);
				rb.GetXyMatchTvArrAndLine(tvArrPlane, A[0], vAngJ, xyArrMatch);
				if(xyArrMatch.GetSize() > 0) A[0]	= xyArrMatch.GetAt(0);
			}
			
			bMatch = FALSE;
			GetTvPlaneOfExp(tvArrPlane, bStt, FALSE, -1);
			rb.GetXyMatchTvArrAndLine(tvArrPlane, A[3], vAngJ, xyArrMatch);
			if(xyArrMatch.GetSize() > 0)
			{
				A[3]	= xyArrMatch.GetAt(0);
				bMatch	= TRUE;
			}
			if(!bMatch)
			{
				GetTvPlaneOfExp(tvArrPlane, !bStt, FALSE, -1);
				rb.GetXyMatchTvArrAndLine(tvArrPlane, A[3], vAngJ, xyArrMatch);
				if(xyArrMatch.GetSize() > 0) A[3]	= xyArrMatch.GetAt(0);
			}
		}
	}
	else
	{
		A[0] = pLineLeft->GetXyLineDis(staLeft - dExpStaLeft,   m_dWidthSlabLeft  - dExpHeightLeft);
		A[3] = pLineRight->GetXyLineDis(staRight + dExpStaRight, m_dWidthSlabRight + dExpHeightRight);
	}
	A[1] = A[0];	//좌측 꺽임 좌표(확폭)
	A[2] = A[3];	//우측 꺽임 좌표(확폭)

	if(!IsExtLine())	return;

//	CDPoint vAngLeft = GetAngleByTarget(nTarget, -1);
//	CDPoint vAngRight = GetAngleByTarget(nTarget, 1);
//	bStt = TRUE;
//	if(nTarget >= BRIDGE_OFF_END_END)//종점부 일 경우
//		bStt = FALSE;

	// 확폭 꺽임???? 이하 막아도 됨... 
/*	if		(bStt && m_dDisExtLineSttLeft == 0 && m_dDisExtLineSttRight == 0)	return;
	else if	(!bStt && m_dDisExtLineEndLeft == 0 && m_dDisExtLineEndRight == 0)	return;

	double staCR = GetStationByTarget(nTarget, 0, bIncludeExp);
	CDPoint vAngCR = GetAngleByTarget(nTarget, 0);

//	CDPoint vAngBangCR = pLineCR->GetAngleAzimuth(staCR);
//	CDPoint vAngBangLeft = pLineLeft->GetAngleAzimuth(staLeft);
//	CDPoint vAngBangRight = pLineRight->GetAngleAzimuth(staRight);
	CDPoint vAngBangCR = GetAngleByTarget(nTarget, 0);
	CDPoint vAngBangLeft = GetAngleByTarget(nTarget, 0);
	CDPoint vAngBangRight = GetAngleByTarget(nTarget, 0);
	CDPoint xyCR = pLineCR->GetXyLine(staCR);
	CDPoint xyLeft = A[0];
	CDPoint xyRight = A[3];

	if(bStt)
	{
		if(m_dDisExtLineSttLeft != 0)
			GetXyMatchLineAndLine(xyCR, vAngBangCR.Rotate(vAngCR), xyLeft, vAngBangLeft.Rotate(vAngLeft), A[1]);
		if(m_dDisExtLineSttRight != 0)
			GetXyMatchLineAndLine(xyCR, vAngBangCR.Rotate(vAngCR), xyRight, vAngBangRight.Rotate(vAngRight), A[2]);
	}
	else
	{
		if(m_dDisExtLineEndLeft != 0)
			GetXyMatchLineAndLine(xyCR, vAngBangCR.Rotate(vAngCR), xyLeft, vAngBangLeft.Rotate(vAngLeft), A[1]);
		if(m_dDisExtLineEndRight != 0)
			GetXyMatchLineAndLine(xyCR, vAngBangCR.Rotate(vAngCR), xyRight, vAngBangRight.Rotate(vAngRight), A[2]);
	}*/
}

double CRcBridgeApp::GetStationByTarget(TARGET_IN_ONE_SLAB nTarget, long nLeft, BOOL bIncludeExp)
{
	if(!GetJijum(0)) return 0;
	if(!GetJijum(m_nQtyJigan)) return 0;

	double dLenSttSlab = GetJijum(0)->m_dLengthUgan[1];
	double dLenEndSlab = -GetJijum(m_nQtyJigan)->m_dLengthUgan[0];
	double dLenSttShoe = dLenSttSlab + GetJijum(0)->m_dLengthSlabToShoe[1];
	double dLenEndShoe = dLenEndSlab - GetJijum(m_nQtyJigan)->m_dLengthSlabToShoe[0];
	switch(nTarget)
	{
	case BRIDGE_OFF_STT_STT:
		return GetStationByDis(TRUE, 0, nLeft,bIncludeExp);
	case BRIDGE_OFF_STT_SLAB:
	case BRIDGE_OFF_STT_SLABCOVER:
		{
			if(IsOutWall(TRUE))
			{
				double dCover = (nTarget == BRIDGE_OFF_STT_SLABCOVER) ? m_dCoverWallOut[0] : 0;
				double staOffset = GetLineBase()->GetStationByOffsetWidth(m_dStationBridgeStt, -m_dWS + dCover, GetJijum(0)->m_vAngle);
				dLenSttSlab = staOffset - m_dStationBridgeStt;
			}
			return GetStationByDis(TRUE, dLenSttSlab, nLeft,bIncludeExp);
		}
	case BRIDGE_OFF_STT_SHOE:
		return GetStationByDis(TRUE, dLenSttShoe, nLeft,FALSE);
	case BRIDGE_OFF_END_END:
		return GetStationByDis(FALSE, 0, nLeft,bIncludeExp);
	case BRIDGE_OFF_END_SLAB:
	case BRIDGE_OFF_END_SLABCOVER:
		{
			if(IsOutWall(FALSE))
			{
				double dCover = (nTarget == BRIDGE_OFF_STT_SLABCOVER) ? m_dCoverWallOut[0] : 0;
				double staOffset = GetLineBase()->GetStationByOffsetWidth(m_dStationBridgeStt + m_dLengthBridge, m_dWE - dCover, GetJijum(m_nQtyJigan)->m_vAngle);
				dLenEndSlab = staOffset - (m_dStationBridgeStt + m_dLengthBridge);
			}
			return GetStationByDis(FALSE, dLenEndSlab, nLeft,bIncludeExp);
		}
	case BRIDGE_OFF_END_SHOE:
		return GetStationByDis(FALSE, dLenEndShoe, nLeft,FALSE);
	}

	return 0;
}

//<summary>
//현재 시.종점에서 선형 중심 길이인 dis(항상 +) 스테이션 만큼 이동된 길이의 스테이션을 구함.
//</summary>
//<param name="bStt">시점.종점 기준을 구분</param>
//<param name="dLenMove">시.종점 스테이션에서 이격된 선형 거리(기준 선형 기준), [+]값은 시점, [-]값은 종점</param>
//<param name="nLeft">0=선형중심, -1=좌측슬래브 끝단, 1=우측슬래브 끝단</param>
//<return></return>
double CRcBridgeApp::GetStationByDis(BOOL bStt, double dLenMove, long nLeft, BOOL bIncludeExp)
{
	double	staCR = m_dStationBridgeStt;
	CDPoint vAngCR = GetJijum(0)->m_vAngle;
	if(bStt == FALSE)
	{
		staCR = m_dStationBridgeStt + m_dLengthBridge;
		vAngCR = GetJijum(m_nQtyJigan)->m_vAngle;
	}

	double disCenter = 0;
	double dExpHeightLeft  = 0;
	double dExpHeightRight = 0;
	if(bIncludeExp && bStt) 
	{
		dExpHeightLeft  = m_dExpSttH[0];
		dExpHeightRight = m_dExpSttH[1];
	}
	if(bIncludeExp && !bStt) 
	{
		dExpHeightLeft  = m_dExpEndH[0];
		dExpHeightRight = m_dExpEndH[1];
	}
	if(nLeft == -1)
		disCenter = m_dWidthSlabLeft - dExpHeightLeft;
	else if(nLeft == 1)
		disCenter = m_dWidthSlabRight + dExpHeightRight;
	if(nLeft == 0 || !IsExtLine())	//도로중심이거나 확폭이 아니면확폭 거더가 아니면
		return GetStationByDisOrdinaryLine(staCR, disCenter, vAngCR, dLenMove);


	double	dDisExtLine = 0;		//확폭 꺽임 길이
	if(nLeft == -1)
	{
		if(bStt)	dDisExtLine = m_dDisExtLineSttLeft;
		else		dDisExtLine = m_dDisExtLineEndLeft;
	}
	else if(nLeft == 1)
	{
		if(bStt)	dDisExtLine = m_dDisExtLineSttRight;
		else		dDisExtLine = m_dDisExtLineEndRight;
	}

	// GetStationByDisExtLineSimple 함수 마지막 인자 입력받는 슬래브 거리 외의 거리가 들어가야 됨
	if(dDisExtLine == 0)	//확폭 꺽임 길이가 0일 경우
	{
		double dExt	= nLeft == -1 ? -dExpHeightLeft : (nLeft == 1 ? dExpHeightRight : 0);
		return GetStationByDisExtLineSimple(staCR, vAngCR, dLenMove, nLeft, dExt);
	}

	return GetStationByDisExtLine(staCR, vAngCR, dLenMove, dDisExtLine, nLeft);
}

//<summary>
//일반선형 일 경우(확폭이 아닐 경우)
//</summary>
//<param name="staCR">도로중심 스테이션</param>
//<param name="disCenter">도로중심 이격된 거리</param>
//<param name="vAngCR">도로중심 각도</param>
//<param name="dLenMove">이동 스테이션</param>
//<return></return>
double CRcBridgeApp::GetStationByDisOrdinaryLine(double staCR, double disCenter, CDPoint vAngCR, double dLenMove)
{
	CDPoint vAngCRMove = GetLineBase()->GetAngleByOffset(staCR, staCR+dLenMove, vAngCR);

	return GetLineBase()->GetStationByScInLine(staCR+dLenMove, disCenter, vAngCRMove);
}

//<summary>
//확폭이고 확폭 꺽임 길이가 0일 경우, nLeft 는 "0"이 아님
//</summary>
//<param name="staCR">도로중심 스테이션</param>
//<param name="vAngCR">도로중심 각도</param>
//<param name="dLenMove">이동 스테이션</param>
//<return></return>
double CRcBridgeApp::GetStationByDisExtLineSimple(double staCR, CDPoint vAngCR, double dLenMove, long nLeft, double dExtDist)
{

	CLineInfo  *pLineCR = GetLineBase();
	CLineInfo  *pLine = GetLineBySlab(nLeft);

	double disCenter = 0;
	if(nLeft == -1)
		disCenter = m_dWidthSlabLeft;
	else if(nLeft == 1)
		disCenter = m_dWidthSlabRight;
	disCenter += dExtDist;

	double  staCRMove = staCR + dLenMove;
	CDPoint vAngCRMove = pLineCR->GetAngleByOffset(staCR, staCRMove, vAngCR);	//항상 옵셋

	CDPoint vAngBang = pLineCR->GetAngleAzimuth(staCRMove);//방위각
	CDPoint xyCRMove  = pLineCR->GetXyLine(staCRMove);

 	double  staReturn = staCR + dLenMove;

	// 선형이 같고 이동 거리가 0인경우 사각에 의해서 계산상 0.0001정도의 차이가 발생해서 ////
	// 여러군데 문제가 발생함.
	// 다라서 선형이 같고 이동거리가 0인경우 들어온 sta를 그대로 리턴하면 된다.
	// modified by jaeho
	if(disCenter == 0 && pLineCR == pLine) return staReturn;
	///////////////////////////////////////////////////////////////////////////////////////////

	CDPoint vAngMatch	= vAngBang.Rotate(vAngCRMove);
	if(!pLine->GetStationMatchByXyAndAngle(xyCRMove, vAngMatch, staReturn, disCenter)) return 0;

	return staReturn;
}

//<summary>l 
//확폭이고 확폭 꺽임 길이가 있을 경우
//</summary>
//<param name="staCR">도로중심 스테이션</param>
//<param name="vAngCR">도로중심 각도</param>
//<param name="dLenMove">이동 스테이션</param>
//<param name="dDisExtLine">확폭 이동거리</param>
//<return></return>
double CRcBridgeApp::GetStationByDisExtLine(double staCR, CDPoint vAngCR, double dLenMove, double dDisExtLine, long nLeft)
{
	CLineInfo  *pLineCR = GetLineBase();
	CLineInfo  *pLine = GetLineBySlab(nLeft);

	double disCenter = 0;
	if(nLeft == -1)
		disCenter = m_dWidthSlabLeft;
	else if(nLeft == 1)
		disCenter = m_dWidthSlabRight;

	CDPoint xyW = pLineCR->GetXyLineByLengthAndAngle(staCR, dDisExtLine, vAngCR);

	double staSlabLineCenter = 0;
	pLine->GetStationMatchVerticalByXy(xyW, staSlabLineCenter);	//0번 또는 마지막 거더의 스테이션
	CDPoint vAngSlab = CDPoint(0, 1);	//0번 또는 마지막 거더의 각도 (항상 90도로 만남)
	double staSlabMove = staSlabLineCenter + dLenMove;
	CDPoint vAngSlabMove = pLine->GetAngleByOffset(staSlabLineCenter, staSlabMove, vAngSlab);

	double staReturn = pLine->GetStationByScInLine(staSlabMove, disCenter, vAngSlabMove);

	return staReturn;
} 


CDPoint CRcBridgeApp::GetAngleByTarget(TARGET_IN_ONE_SLAB nTarget, long nLeft)
{
	if(!GetJijum(0)) return 0;
	if(!GetJijum(m_nQtyJigan)) return 0;

	double dLenSttSlab	= GetJijum(0)->m_dLengthUgan[1];
	double dLenEndSlab	= -GetJijum(m_nQtyJigan)->m_dLengthUgan[0];
	double dLenSttShoe	= dLenSttSlab + GetJijum(0)->m_dLengthSlabToShoe[1];
	double dLenEndShoe	= dLenEndSlab - GetJijum(m_nQtyJigan)->m_dLengthSlabToShoe[0];
	double dCover		= GetCoverSide_UpperSlab(0);
	BOOL bCover	= nTarget == BRIDGE_OFF_STT_SLABCOVER || nTarget == BRIDGE_OFF_END_SLABCOVER;

	switch(nTarget)
	{
	case BRIDGE_OFF_STT_STT:
		return GetAngleByDis(TRUE, 0, nLeft);
	case BRIDGE_OFF_STT_SLAB:
	case BRIDGE_OFF_STT_SLABCOVER:
		{
			if(IsOutWall(TRUE))
			{
				double staOffset = GetLineBase()->GetStationByOffsetWidth(m_dStationBridgeStt, bCover ? -m_dWS+dCover : -m_dWS, GetJijum(0)->m_vAngle);
				dLenSttSlab = staOffset - m_dStationBridgeStt;
			}
			else
			{
				double staOffset = GetLineBase()->GetStationByOffsetWidth(m_dStationBridgeStt+dLenSttSlab, dCover, GetJijum(0)->m_vAngle);
				dLenSttSlab		= staOffset - (m_dStationBridgeStt+dLenSttSlab);
			}
			return GetAngleByDis(TRUE, dLenSttSlab, nLeft);
		}
	case BRIDGE_OFF_STT_SHOE:
		return GetAngleByDis(TRUE, dLenSttShoe, nLeft);
	case BRIDGE_OFF_END_END:
		return GetAngleByDis(FALSE, 0, nLeft);
	case BRIDGE_OFF_END_SLAB:
	case BRIDGE_OFF_END_SLABCOVER:
		{
			if(IsOutWall(FALSE))
			{
				double staOffset = GetLineBase()->GetStationByOffsetWidth(m_dStationBridgeStt + m_dLengthBridge, bCover ? m_dWE - dCover : m_dWE, GetJijum(m_nQtyJigan)->m_vAngle);
				dLenEndSlab = staOffset - (m_dStationBridgeStt + m_dLengthBridge);
			}
			else
			{
				double staOffset = GetLineBase()->GetStationByOffsetWidth(m_dStationBridgeStt+m_dLengthBridge+dLenEndSlab, -dCover, GetJijum(m_nQtyJigan)->m_vAngle);
				dLenEndSlab	= staOffset - (m_dStationBridgeStt+m_dLengthBridge+dLenEndSlab);
			}
			return GetAngleByDis(FALSE, dLenEndSlab, nLeft);
		}
	case BRIDGE_OFF_END_SHOE:
		return GetAngleByDis(FALSE, dLenEndShoe, nLeft);
	}

	return CDPoint(0,1);
}

//<summary>
//현재 시.종점에서 선형 중심 길이인 dis(항상 +) 스테이션 만큼 이동된 길이의 각도를 구함.
//</summary>
//<param name="bStt">시점.종점 기준을 구분</param>
//<param name="dLenMove">시.종점 스테이션에서 이격된 선형 거리(기준 선형 기준), [+]값은 시점, [-]값은 종점</param>
//<return></return>
CDPoint CRcBridgeApp::GetAngleByDis(BOOL bStt, double dLenMove, long nLeft)
{
	if(!GetJijum(0)) return 0;
	
	double	staCR = m_dStationBridgeStt;
	CDPoint vAngCR = GetJijum(0)->m_vAngle;
	if(bStt == FALSE)
	{
		staCR = m_dStationBridgeStt + m_dLengthBridge;
		vAngCR = GetJijum(m_nQtyJigan)->m_vAngle;
	}

	double disCenter = 0;
	if(nLeft == -1)
		disCenter = m_dWidthSlabLeft;
	else if(nLeft == 1)
		disCenter = m_dWidthSlabRight;
	if(nLeft == 0 || !IsExtLine())	//도로중심이거나 확폭이 아니면확폭 거더가 아니면
		return GetAngleByDisOrdinaryLine(staCR, disCenter, vAngCR, dLenMove);


	double	dDisExtLine = 0;		//확폭 꺽임 길이
	if(nLeft == -1)
	{
		if(bStt)	dDisExtLine = m_dDisExtLineSttLeft;
		else		dDisExtLine = m_dDisExtLineEndLeft;
	}
	else if(nLeft == 1)
	{
		if(bStt)	dDisExtLine = m_dDisExtLineSttRight;
		else		dDisExtLine = m_dDisExtLineEndRight;
	}

	if(dDisExtLine == 0)	//확폭 꺽임 길이가 0일 경우
		return GetAngleByDisExtLineSimple(staCR, vAngCR, dLenMove, nLeft);

	return GetAngleByDisExtLine(staCR, vAngCR, dLenMove, dDisExtLine, nLeft);
}

//<summary>
//일반선형 일 경우(확폭이 아닐 경우)
//</summary>
//<param name="staCR">도로중심 스테이션</param>
//<param name="disCenter">도로중심 이격된 거리</param>
//<param name="vAngCR">도로중심 각도</param>
//<param name="dLenMove">이동 스테이션</param>
//<return></return>
CDPoint CRcBridgeApp::GetAngleByDisOrdinaryLine(double staCR, double disCenter, CDPoint vAngCR, double dLenMove)
{
	CDPoint vAngCRMove = GetLineBase()->GetAngleByOffset(staCR, staCR+dLenMove, vAngCR);

	return vAngCRMove;
}

//<summary>
//확폭이고 확폭 꺽임 길이가 0일 경우, nLeft 는 "0"이 아님
//</summary>
//<param name="staCR">도로중심 스테이션</param>
//<param name="vAngCR">도로중심 각도</param>
//<param name="dLenMove">이동 스테이션</param>
//<return></return>
CDPoint CRcBridgeApp::GetAngleByDisExtLineSimple(double staCR, CDPoint vAngCR, double dLenMove, long nLeft)
{
	CLineInfo  *pLineCR = GetLineBase();

	double  staCRMove = staCR + dLenMove;
	CDPoint vAngReturn = pLineCR->GetAngleByOffset(staCR, staCRMove, vAngCR);	//항상 옵셋
	return vAngReturn;
}

//<summary>
//확폭이고 확폭 꺽임 길이가 있을 경우
//</summary>
//<param name="staCR">도로중심 스테이션</param>
//<param name="vAngCR">도로중심 각도</param>
//<param name="dLenMove">이동 스테이션</param>
//<param name="dDisExtLine">확폭 이동거리</param>
//<return></return>
CDPoint CRcBridgeApp::GetAngleByDisExtLine(double staCR, CDPoint vAngCR, double dLenMove, double dDisExtLine, long nLeft)
{
	CLineInfo  *pLineCR = GetLineBase();
	CLineInfo  *pLine = GetLineBySlab(nLeft);

	double disCenter = 0;
	if(nLeft == -1)
		disCenter = m_dWidthSlabLeft;
	else if(nLeft == 1)
		disCenter = m_dWidthSlabRight;

	CDPoint xyW = pLineCR->GetXyLineByLengthAndAngle(staCR, dDisExtLine, vAngCR);

	double staSlabLineCenter = 0;
	pLine->GetStationMatchVerticalByXy(xyW, staSlabLineCenter);	//0번 또는 마지막 거더의 스테이션
	CDPoint vAngSlab = CDPoint(0, 1);	//0번 또는 마지막 거더의 각도 (항상 90도로 만남)
	double staSlabMove = staSlabLineCenter + dLenMove;
	CDPoint vAngSlabMove = pLine->GetAngleByOffset(staSlabLineCenter, staSlabMove, vAngSlab);

	double staReturn = pLine->GetStationByScInLine(staSlabMove, disCenter, vAngSlabMove);
	CDPoint vAngReturn = pLine->GetAngleByOffset(staSlabMove, staReturn, vAngSlabMove);
	return vAngReturn;
} 

double CRcBridgeApp::GetStationOnJijum(long nJijum)
{
	return GetStationOnJijum(nJijum,0,FALSE);
}

//<summary>
//해당 지점의 스테이션 구하기
//</summary>
//<param name="nJ">지점번호 0, 1, 2, 교대교각 구분없음, 교대시종점, 교각중앙부(유간기준선)</param>
//<param name="nSlabLeft">"-1" = 좌측 슬래브, "0" = 도로중심, "1" = 우측 슬래브</param>
//<return></return>
double CRcBridgeApp::GetStationOnJijum(long nJ, long nSlabLeft, BOOL bVertDir)
{
	if(nJ < 0 || nJ > m_nQtyJigan+1)	return 0;

	if(!bVertDir)
	{
		if(nJ == 0 || nJ == m_nQtyJigan)
		{
			double dSta	= (nJ == 0) ? GetStationByTarget(BRIDGE_OFF_STT_STT, nSlabLeft, FALSE) : GetStationByTarget(BRIDGE_OFF_END_END, nSlabLeft, FALSE);

			if(0)//nSlabLeft != 0)
			{
				CLineInfo *pLine	= GetLineLeftCenRight(nSlabLeft, bVertDir);
				CLineInfo *pLineCR	= GetLineLeftCenRight(0, bVertDir);
				CDPoint vAng		= GetAngleAzimuthJijum(nJ);
				CDPoint xyPlane		= pLine->GetXyLine(dSta);
				pLineCR->GetStationMatchByXyAndAngle(xyPlane, vAng, dSta, 0);
			}

			return dSta;
		}
	}

	//교각부 계산	항상 nJ가 "1"이상
	CDPoint vAngCR = GetJijum(nJ)->m_vAngle;
	double	staCR = m_dStationBridgeStt;//첫번째 교각
	long n = 0; for(n = 1; n <= nJ; n++)
	{
		staCR += GetJijum(n-1)->m_dLengthUgan[1] + GetJijum(n-1)->m_dLengthSlabToShoe[1]
			     + GetJijum(n)->m_dLengthSlabToShoe[0] + GetJijum(n)->m_dLengthUgan[0] + m_dLengthJigan[n-1];
	}
	if(vAngCR.GetAngleDegree()==90) return staCR;
	double staMatch = GetStationByDisExtLineSimple(staCR, vAngCR, 0, nSlabLeft);

	if(bVertDir)
	{
		if(nJ == 0) 
			staMatch	= GetStationByTarget(BRIDGE_OFF_STT_STT, nSlabLeft, FALSE);
		if(nJ == m_nQtyJigan)
			staMatch	= GetStationByTarget(BRIDGE_OFF_STT_STT, nSlabLeft, FALSE)+GetLengthBridge(FALSE);

//		CLineInfo *pLine	= GetLineBase();
// 		CDPoint xy	= pLine->GetXyLineByLengthAndAngle(staMatch, 0, CDPoint(0, 1));
		CDPoint xy	= GetXyVertSectionByDirVert(CDPoint(staMatch, 0), nJ);
		staMatch	= xy.x;
	}

	return staMatch;
}

// 지점과 지점 사이의 거리(지간 거리)
// BOOL bIncludeWall : 벽체 포함인지?
double CRcBridgeApp::GetLengthJigan(int nJigan, long nSlabLeft, BOOL bVertDir, BOOL bIncludeWall)
{
	if(nJigan >= m_nQtyJigan) return 0;
	int nJ1 = nJigan == -1 ? 0 : nJigan;
	int nJ2 = nJigan == -1 ? m_nQtyJigan : nJigan + 1;
	double dDist	= 0;
	double dSta1 = GetStationOnJijum(nJ1, nSlabLeft, bVertDir);
	double dSta2 = GetStationOnJijum(nJ2, nSlabLeft, bVertDir);

	if(0)//bVertDir)
	{
		CLineInfo *pLine	= GetLineBase();
		CDPoint xy1	= pLine->GetXyLineByLengthAndAngle(dSta1, 0, CDPoint(0, 1));
		CDPoint xy2	= pLine->GetXyLineByLengthAndAngle(dSta2, 0, CDPoint(0, 1));
		xy1		= GetXyVertSectionByDirVert(xy1, nJ1);
		xy2		= GetXyVertSectionByDirVert(xy2, nJ2);

		dDist	= ~(xy1-xy2);
	}
	else
	{
		
		if(nSlabLeft == 0)
		{
			dDist	= fabs(dSta2-dSta1);
		}
		else
		{
			double dWidth	= nSlabLeft == -1 ? m_dWidthSlabLeft : m_dWidthSlabRight;
			CLineInfo *pLine	= GetLineLeftCenRight(nSlabLeft, bVertDir);
			dDist	= pLine->GetLengthByDis(dSta1, dSta2, dWidth);
		}
	}
		
	if(!bIncludeWall)
	{
		CDPoint vAngStt	= GetAngleJijum(nJigan);
		CDPoint vAngEnd	= GetAngleJijum(nJigan+1);
		if(nJigan == 0)
		{
			if(nJigan != m_nQtyJigan-1)
			{
				CWallApp *pWall	= GetInWall(nJigan);
				if(pWall)
				{
					if(pWall->m_bIs)
						dDist -= (pWall->m_dW/vAngEnd.y)/2;
				}
			}
		}
		else if(nJigan == m_nQtyJigan-1)
		{
			CWallApp *pWall	= GetInWall(nJigan-1);
			if(pWall)
			{
				if(pWall->m_bIs)
					dDist -= (pWall->m_dW/vAngStt.y)/2;
			}
		}
		else
		{
			CWallApp *pWall	= GetInWall(nJigan-1);
			if(pWall)
			{
				if(pWall->m_bIs)
					dDist -= (pWall->m_dW/vAngStt.y)/2;
			}
			pWall	= GetInWall(nJigan);
			if(pWall)
			{
				if(pWall->m_bIs)
					dDist -= (pWall->m_dW/vAngStt.y)/2;
			}
		}
	}

	return dDist;
}

double CRcBridgeApp::GetStationShoeOnJijum(long nJ, long nSlabLeft, long nLeft, BOOL bVertDir)
{
	// 입력을 사거리로 받으므로, 직거리 위치 가져올땐 슈거리를 직거리로 변경해야 됨
	CDPoint vAng	= bVertDir ? GetAngleJijum(nJ) : CDPoint(0, 1);
	if(nLeft == 0)
	{
		return GetStationOnJijum(nJ, nSlabLeft, bVertDir) - (GetJijum(nJ)->m_dLengthSlabToShoe[0] + GetJijum(nJ)->m_dLengthUgan[0])*vAng.y;
	}

	return GetStationOnJijum(nJ, nSlabLeft, bVertDir) + (GetJijum(nJ)->m_dLengthSlabToShoe[1] + GetJijum(nJ)->m_dLengthUgan[1])*vAng.y;
}

CDPoint CRcBridgeApp::GetAngleShoeOnJijum(long nJ, long nSlabLeft, long nLeft)
{
	return GetLineBase()->GetAngleByOffset(GetStationOnJijum(nJ, nSlabLeft), GetStationShoeOnJijum(nJ, nSlabLeft, nLeft), GetJijum(nJ)->m_vAngle);
}

// 현재 교량의 타입에 맞게 갖가지 성분들을 변경한다
//BOOL bCheckAps : 접속슬래브는 브라켓 존재여부에 따라 있고 없고가 결정된다.
// 따라서 실제로 브라켓을 입력받기 전까진 브라켓여부를 모르므로 접속슬래브에 대해서 체크를 하지 말아야 한다.
void CRcBridgeApp::ModifyBridgeType(BOOL bCheckAps)
{
	// 문형
	if(m_nTypeBridge == BRIDGE_TYPE_PORTAL)
	{
		// 양쪽 벽체 가 모두 잇음
		m_nTypeOutWall = 2;
		m_footingStt.m_bIs = TRUE;
		m_footingStt.m_exFooting.m_bIs = TRUE;
		m_footingEnd.m_bIs = TRUE;
		m_footingEnd.m_exFooting.m_bIs = TRUE;
		m_footingBox.m_bIs = FALSE;
		m_footingBox.m_exFooting.m_bIs = FALSE;
		
		// 내측벽체 존재
		long i = 0; for(i = 0; i < GetCountInWall(); i++)
		{
			GetInWall(i)->m_bIs = TRUE;
			GetInWall(i)->m_footing.m_bIs = TRUE;
			GetInWall(i)->m_footing.m_exFooting.m_bIs = TRUE;
			GetInWall(i)->m_Column.m_bIs	= TRUE;
		}

	}

	// ㅠ형과 V형, 경사
	else if(m_nTypeBridge == BRIDGE_TYPE_PI || m_nTypeBridge == BRIDGE_TYPE_V || m_nTypeBridge == BRIDGE_TYPE_TRAP)
	{
		// 양측벽체 모두 없음
		m_nTypeOutWall = 0;
		m_footingStt.m_bIs = FALSE;
		m_footingStt.m_exFooting.m_bIs = FALSE;
		m_footingEnd.m_bIs = FALSE;
		m_footingEnd.m_exFooting.m_bIs = FALSE;
		m_footingBox.m_bIs = FALSE;
		m_footingBox.m_exFooting.m_bIs = FALSE;

		// 내측벽체 존재
		long i = 0; for(i = 0; i < GetCountInWall(); i++)
		{
			GetInWall(i)->m_bIs = TRUE;
			GetInWall(i)->m_footing.m_bIs = TRUE;
			GetInWall(i)->m_footing.m_exFooting.m_bIs = TRUE;
			GetInWall(i)->m_Column.m_bIs	= TRUE;
			if(m_nTypeBridge != BRIDGE_TYPE_PI)
				GetInWall(i)->m_bTopIsColumn	= FALSE;
		}
	}

	// 슬래브
	else if(m_nTypeBridge == BRIDGE_TYPE_SLAB)
	{
		// 양쪽 벽체가 모두 없음
		m_nTypeOutWall = 0;
		m_footingStt.m_bIs = FALSE;
		m_footingStt.m_exFooting.m_bIs = FALSE;
		m_footingEnd.m_bIs = FALSE;
		m_footingEnd.m_exFooting.m_bIs = FALSE;
		m_footingBox.m_bIs = FALSE;
		m_footingBox.m_exFooting.m_bIs = FALSE;

		// 내측벽체 없음
		long i = 0; for(i = 0; i < GetCountInWall(); i++)
		{
			GetInWall(i)->m_bIs				 = FALSE;
			GetInWall(i)->m_Column.m_bIs	= FALSE;
			GetInWall(i)->m_footing.m_bIs	= FALSE;
			GetInWall(i)->m_footing.m_exFooting.m_bIs = FALSE;
		}
	}

	// 경사교 2타입
	else if(m_nTypeBridge == BRIDGE_TYPE_TRAP2)
	{
		// 양측벽체 모두 있음
		m_nTypeOutWall = 2;
		m_footingStt.m_bIs = TRUE;
		m_footingStt.m_exFooting.m_bIs = TRUE;
		m_footingEnd.m_bIs = TRUE;
		m_footingEnd.m_exFooting.m_bIs = TRUE;
		m_footingBox.m_bIs = FALSE;
		m_footingBox.m_exFooting.m_bIs = FALSE;

		// 내측벽체 존재
		long i = 0; for(i = 0; i < GetCountInWall(); i++)
		{
			CWallApp *pWall	= GetInWall(i);
			if(pWall)
			{
				pWall->m_bIs = TRUE;
				pWall->m_footing.m_bIs = FALSE;
				pWall->m_footing.m_exFooting.m_bIs = FALSE;
				pWall->m_Column.m_bIs	= FALSE;
				pWall->m_bTopIsColumn	= FALSE;
			}
		}
	}

	// 박스교
	else if(m_nTypeBridge == BRIDGE_TYPE_BOX)
	{
		// 양쪽 벽체 가 모두 잇음
		m_nTypeOutWall = 2;

		// 시점측은 기초의 높이가 0이고 확대기초가 잇으며
		// 종점측은 기초도 확대기초도 없다
		m_footingStt.m_bIs = FALSE;
		m_footingStt.m_exFooting.m_bIs = FALSE;
		m_footingEnd.m_bIs = FALSE;
		m_footingEnd.m_exFooting.m_bIs = FALSE;
		m_footingBox.m_bIs = TRUE;
		m_footingBox.m_dFH = 0;
		m_footingBox.m_exFooting.m_bIs = TRUE;

		// 내측벽체 존재하지만 기초는 없음
		long i = 0; for(i = 0; i < GetCountInWall(); i++)
		{
			GetInWall(i)->m_bIs = TRUE;
			GetInWall(i)->m_footing.m_bIs = FALSE;
			GetInWall(i)->m_footing.m_exFooting.m_bIs = FALSE;
			// 기둥 배근을 위해서 기초에 피복만 넣어주자.
			// 엔진에서 기초가 없는데도 배근할때 기초의 피복을 쓰네..
			GetInWall(i)->m_footing.m_dCoverLower[0] = m_dCoverLower_LowerSlab[0];
//			GetInWall(i)->m_footing.m_nType = FOOTING_TYPE_BOX;
		}
	}

	// 아치교
	else if(m_nTypeBridge == BRIDGE_TYPE_ARCH)
	{
		m_nTypeOutWall	= 2;

		m_footingStt.m_bIs = TRUE;
		m_footingStt.m_exFooting.m_bIs = TRUE;
		m_footingEnd.m_bIs = TRUE;
		m_footingEnd.m_exFooting.m_bIs = TRUE;
		m_footingBox.m_bIs = FALSE;
		m_footingBox.m_exFooting.m_bIs = FALSE;
		// 내측벽체 존재
		long i = 0; for(i = 0; i < GetCountInWall(); i++)
		{
			CWallApp *pWall	= GetInWall(i);
			if(pWall)
			{
				pWall->m_bIs = TRUE;
				pWall->m_footing.m_bIs = FALSE;
				pWall->m_footing.m_exFooting.m_bIs = FALSE;
				pWall->m_footing.m_bVirtualFooting	= FALSE;
				pWall->m_Column.m_bIs	= TRUE;
				pWall->m_bTopIsColumn	= FALSE;
			}
		}
	}


	// 접속슬래브를 없애야 하는 경우
	if(bCheckAps)
	{
		long stt = 0;
		for(stt = 0; stt < 2; stt++)
		{
			BOOL bStt	= stt == iSTT;
			if(IsBracket(bStt)) continue;

			long left = 0; for(left = 0; left < 2; left++)
			{
				if(bStt)
					m_apsStt[left].m_bIs	= FALSE;
				else
					m_apsEnd[left].m_bIs	= FALSE;

				// 완충슬래브도 같이 없앰
				for(long cns = 0; cns < 2; cns++)
				{
					if(bStt)
						m_cnsStt[left][cns].m_bIs	= FALSE;
					else
						m_cnsEnd[left][cns].m_bIs	= FALSE;
				}
			}
		}

		// 완충슬래브 없애야 하는 경우
		for(stt = 0; stt < 2; stt++)
		{
			BOOL bStt	= stt == iSTT;

			long left = 0; for(left = 0; left < 2; left++)
			{
				CApsApp *pAps	= bStt ? &m_apsStt[left] : &m_apsEnd[left];
				if(!pAps->m_bIs)
				{
					if(bStt)
						m_nCountCnsStt[left]	= 0;
					else
						m_nCountCnsEnd[left]	= 0;
					// 완충슬래브도 같이 없앰
					for(long cns = 0; cns < 2; cns++)
					{
						if(bStt)
							m_cnsStt[left][cns].m_bIs	= FALSE;
						else
							m_cnsEnd[left][cns].m_bIs	= FALSE;
					}
				}
			}
		}
	}
	


	// 접속슬래브가 나눠진 경우..
	if(m_apsStt[iLEFT].m_bIs && m_bDivisionApsStt)
	{
		m_apsStt[iRIGHT].m_bIs	= TRUE;

		// 날개벽 각도 적용 활성화
		m_apsStt[iLEFT].m_bEnableApplyWingAngLeft	= TRUE;
		m_apsStt[iLEFT].m_bEnableApplyWingAngRight	= FALSE;
		m_apsStt[iRIGHT].m_bEnableApplyWingAngLeft	= FALSE;
		m_apsStt[iRIGHT].m_bEnableApplyWingAngRight	= TRUE;
	}
	else
	{
		m_apsStt[iRIGHT].m_bIs	= FALSE;
		// 날개벽 각도 적용 활성화
		m_apsStt[iLEFT].m_bEnableApplyWingAngLeft	= TRUE;
		m_apsStt[iLEFT].m_bEnableApplyWingAngRight	= TRUE;
		m_apsStt[iRIGHT].m_bEnableApplyWingAngLeft	= TRUE;
		m_apsStt[iRIGHT].m_bEnableApplyWingAngRight	= TRUE;
	}
	if(m_apsEnd[iLEFT].m_bIs && m_bDivisionApsEnd)
	{
		m_apsEnd[iRIGHT].m_bIs	= TRUE;

		// 날개벽 각도 적용 활성화
		m_apsEnd[iLEFT].m_bEnableApplyWingAngLeft	= TRUE;
		m_apsEnd[iLEFT].m_bEnableApplyWingAngRight	= FALSE;
		m_apsEnd[iRIGHT].m_bEnableApplyWingAngLeft	= FALSE;
		m_apsEnd[iRIGHT].m_bEnableApplyWingAngRight	= TRUE;
	}
	else
	{
		m_apsEnd[iRIGHT].m_bIs	= FALSE;

		// 날개벽 각도 적용 활성화
		m_apsEnd[iLEFT].m_bEnableApplyWingAngLeft	= TRUE;
		m_apsEnd[iLEFT].m_bEnableApplyWingAngRight	= TRUE;
		m_apsEnd[iRIGHT].m_bEnableApplyWingAngLeft	= TRUE;
		m_apsEnd[iRIGHT].m_bEnableApplyWingAngRight	= TRUE;
	}

	// 날개벽을 없애야 되는 경우
	if(!IsOutWall(TRUE))
	{
		m_pWingWallStt[0]->m_bExist	= FALSE;
		m_pWingWallStt[1]->m_bExist	= FALSE;
	}

	if(!IsOutWall(FALSE))
	{
		m_pWingWallEnd[0]->m_bExist	= FALSE;
		m_pWingWallEnd[1]->m_bExist	= FALSE;
	}
}

// 헌치 권고안
// nIdxWall 이 -1일경우 자동으로 찾아준다.
void CRcBridgeApp::SetDataDefaultShapeHunch(long nIdxHunch)
{
	if(m_nCountInWallHunch <= nIdxHunch) return;
	long nIdxWall = 0;
	BOOL bLeft = TRUE;
	GetInfoHunch(nIdxHunch, nIdxWall, bLeft);
	if(nIdxWall < 0) return;

	CWallApp *pWall = GetInWall(nIdxWall);

	// 초기화
	for(long upper=0; upper<2; upper++)
	{
		m_dHHL[nIdxHunch][upper]	= 0;
		m_dHHR[nIdxHunch][upper]	= 0;
		m_dHBH[nIdxHunch][upper]	= 0;
		m_dHLW[nIdxHunch][upper]	= 0;
		m_dHRW[nIdxHunch][upper]	= 0;
		m_dWL[nIdxHunch][upper]		= 0;
		m_dWR[nIdxHunch][upper]		= 0;
		m_dHLR[nIdxHunch][upper]	= 0;	// 내측헌치 왼쪽 반경
		m_dHRR[nIdxHunch][upper]	= 0;	// 내측헌치 오른쪽 반경
	}
		
	if(m_nTypeBridge == BRIDGE_TYPE_ARCH)
	{
		m_dHLW[nIdxHunch][iUPPER]	= 0;
		m_dHRW[nIdxHunch][iUPPER]	= 0;
		m_dHHL[nIdxHunch][iUPPER]	= 0;
		m_dHHR[nIdxHunch][iUPPER]	= 0;
		m_dHBH[nIdxHunch][iUPPER]	= 0;
		m_dWL[nIdxHunch][iUPPER]	= pWall->m_dW/2;
		m_dWR[nIdxHunch][iUPPER]	= pWall->m_dW/2;
	}
	else
	{
		for(long upper=0; upper<2; upper++)
		{
			m_dHLW[nIdxHunch][upper] = m_nTypeBridge == BRIDGE_TYPE_SLAB ? m_dHHS * 5 : dWidthHunch;
			m_dHRW[nIdxHunch][upper] = m_nTypeBridge == BRIDGE_TYPE_SLAB ? m_dHHS * 5 : dWidthHunch;
			m_dHHL[nIdxHunch][upper] = dHeightHunch;
			m_dHHR[nIdxHunch][upper] = dHeightHunch;
			m_dHBH[nIdxHunch][upper] = 0;
			m_dWL[nIdxHunch][upper] = pWall->m_dW/2;
			m_dWR[nIdxHunch][upper] = pWall->m_dW/2;
			if(!pWall->IsVertWall())
			{
				if(pWall->GetAngleWallByHor(TRUE) < 90)
				{
					m_dHLR[nIdxHunch][upper]	= pWall->m_dW;
					m_dHRR[nIdxHunch][upper]	= pWall->m_dW*2;
				}
				else
				{
					m_dHLR[nIdxHunch][upper]	= pWall->m_dW*2;
					m_dHRR[nIdxHunch][upper]	= pWall->m_dW;
				}
			}
		}
	}
}

// 형상권고안 메인
void CRcBridgeApp::SetDataDefaultShapeStd(long nLeft, BOOL bSyncAps)
{
	SyncBridge(nLeft, FALSE, bSyncAps);

	SetDataDefaultShapeBridge(TRUE);
	SetDataDefaultShapeBridge(FALSE);
	SetDataDefaultShapeFooting(&m_footingStt, m_dWS, m_dStationBridgeStt);
	SetDataDefaultShapeFooting(&m_footingEnd, m_dWE, m_dStationBridgeStt+m_dLengthBridge);
	SetDataDefaultPRF();
	long i = 0; for(i = 0; i < m_nCountInWallHunch; i++)
	{
		SetDataDefaultShapeHunch(i);
	}
	for(i = 0; i < GetCountInWall(); i++)
	{
		CWallApp *pWall	= GetInWall(i);
		
		if(pWall)
		{
			SetDataDefaultShapeInWall(i, FALSE, nLeft);
			SetDataDefaultShapeFooting(&pWall->m_footing, pWall->m_dW, GetXyOnJijum(i+1).x, i);
		}
	}
	SetDataDefaultShapeRib();

	if(m_nTypeBridge == BRIDGE_TYPE_BOX)
	{
		SetDataDefaultBoxLowerSlab();
	}
}

// 지간 좌표 구함.(평면)
//<summary>
//지점 좌우 슬래브 끝단의 좌표를 구함
//</summary>
//<param name="nJ"></param>
//<param name="dLenMove">지점에서 이동한 스테이션 거리</param>
//<return></return>
CTwinVector CRcBridgeApp::GetTvJijum(long nJ, double dLenMove)
{
	CTwinVector tv;
	if(nJ < 0 || nJ > m_nQtyJigan) return tv;	//시종점 교대는 그리지 않음.

	double  staLeft = GetStationOnJijum(nJ, -1);
	double  staRight = GetStationOnJijum(nJ, 1);
	if(1)//dLenMove != 0)
	{
		double staCR = GetStationOnJijum(nJ, 0);
		CDPoint vAngCR = GetJijum(nJ)->m_vAngle;
		staLeft = GetStationByDisExtLineSimple(staCR, vAngCR, dLenMove, -1);
		staRight = GetStationByDisExtLineSimple(staCR, vAngCR, dLenMove, 1);
	}

	CLineInfo *pLineLeft	= GetLineLeftRight(TRUE);
	CLineInfo *pLineRight	= GetLineLeftRight(FALSE);
	if(!pLineLeft || !pLineRight) return tv;

	tv.m_v1 = pLineLeft->GetXyLineDis(staLeft, m_dWidthSlabLeft);
	tv.m_v2 = pLineRight->GetXyLineDis(staRight, m_dWidthSlabRight);

	return tv;
}

void CRcBridgeApp::SetDataDefaultShapeTopInWall(long nIdxWall)
{
	CWallApp *pWall = GetInWall(nIdxWall);
	if(!pWall) return;

	if(pWall->m_bIs)
	{
		if(pWall->m_Column.m_bIs)
		{
			pWall->m_Column.m_dB[iOUTTER][iUPPER] = pWall->m_dW;
			pWall->m_Column.m_dH[iOUTTER][iUPPER] = 0;
			pWall->m_Column.m_dR[iOUTTER][iUPPER] = 0;
		}
	}

	return;
}

// 현재 교량의 속성에 맞게 벽체의 속성을 정해준다.
// long nLeft 교량 단면 그릴때 기준 선형
// nLeft -1 좌측, nLeft 0 중심, nLeft 1 우측
// 좌측과 우측은 내측벽체에 대해서는 최적화 되지 않음
// 단지 외측벽체와 슬래브에 대해서만 최적화 되어 있음.
void CRcBridgeApp::SyncBridge(long nLeft, BOOL bApplyAngle, BOOL bSyncAps, BOOL bApplyAngleAps, BOOL bForBMOfFooting)
{
//	if(m_dWidthSlabLeft==0 || m_dWidthSlabRight==0)
//		return;
	// 타입에 따른 속성
	ModifyBridgeType(bSyncAps);

	BOOL bSettingVertSection	= m_bSettingVertSection;

	GetTvVertSection(nLeft, bApplyAngle);
	if(!bSettingVertSection)
	{
		if(m_nTypeBridge == BRIDGE_TYPE_ARCH)
			m_tvArrVertSection_NonModify_ForArch = m_tvArrVertSectionRib[iUPPER];
	}
	// 교량의 기초 좌표 구함
	SetTvPathPlan();	
	m_footingStt.m_bLeftIsFrontGup	= TRUE;
	m_footingEnd.m_bLeftIsFrontGup	= FALSE;
	m_footingStt.m_dCenterOfWall	= GetMidCrossSection(!bApplyAngle);
	m_footingEnd.m_dCenterOfWall	= GetMidCrossSection(!bApplyAngle);
	m_footingStt.m_exFooting.m_Pile.m_bLeftIsOrgStt	= FALSE;
	m_footingEnd.m_exFooting.m_Pile.m_bLeftIsOrgStt	= TRUE;

	// 외측벽체 좌표가 구해지면
	// 외측벽체 좌표의 하면으로 m_footingStt와 m_footingEnd의 m_tvPathLeft를 설정해줘야 된다.
	CWallApp wall;
	CTwinVectorArray tvArrSide, tvArrHidden;
	CTwinVector tv;

	if(m_nTypeBridge == BRIDGE_TYPE_BOX)
	{
		SyncBridgeBoxType(bApplyAngle);
	}
	else
	{
		// 시점 기초
		SyncBridge_CrossSection_SttEndFooting(bApplyAngle);

		// (ARCBRIDGE-3442) 벽체 위치가 곡선 선형인경우 종단에 표현되는 벽체 두께와 기초에서의 벽체 두께가 같지 않아 벽체와 기초의 연결부가 틀어지고
		// 치수 위치도 조금씩 어긋난다. 지점 각도를 조금 조정해서 벽체 두께를 맞춰 준다.
		double dAngStt = ToDegree(asin(min(1.0, m_dWS / GetThickOutWallByLine(TRUE))));
		double dAngEnd = ToDegree(asin(min(1.0, m_dWE / GetThickOutWallByLine(FALSE))));
		if(GetAngleJijum(0).GetAngleDegree() > 90)
		{
			dAngStt = 180 - dAngStt;
		}
		if(GetAngleJijum(m_nQtyJigan).GetAngleDegree() > 90)
		{
			dAngEnd = 180 - dAngEnd;
		}

		m_footingStt.SyncFooting(bApplyAngle, dAngStt, FALSE, bForBMOfFooting);
		m_footingEnd.SyncFooting(bApplyAngle, dAngEnd, FALSE, bForBMOfFooting);
		TrimRibByFootingUpper();
		GetTvRibBlock();
	}

	// 접속슬래브 동기화(접속슬래브는 있을지 없을지 모를 수가 있다...) 그래서 옵션처리
	if(bSyncAps)
		SyncBridgeAps(nLeft, bApplyAngleAps);

	// 지점 path 기초와 벽체에 설정
	CRebarPlacing rb;
	long i = 0; for(i = 0; i < GetCountInWall(); i++)
	{
		CWallApp *pWall = GetInWall(i);
		if(pWall)
		{
			SyncWall(i+1, pWall, bApplyAngle);
		}
	}
	TrimRibByConnectUpperSlab();

	if(m_nTypeBridge == BRIDGE_TYPE_ARCH)
	{
		m_tvArrVertSection.AddFromTvArray(m_tvArrVertSectionRib[iUPPER]);
		m_tvArrVertSection.AddFromTvArray(m_tvArrVertSectionRib[iLOWER]);
	}

	// 겹치는 선들에 대해서 체크함
	CheckOverLapLineBetweenSubObject();

	// 기초에 고유 인덱스 넘겨줌(지점번호 아니고, 실제 존재하는 기초들 간의 인덱스)
	for(i = 0; i < GetCountFooting(); i++)
	{
		CFootingApp *pFooting	= GetFooting(i);
		if(pFooting)
		{
			pFooting->m_nIndexFooting	= i;
		}
	}
	// 
	SyncBridge_DesignData();
}
//<summary>
//도상 기본선을 그림. 꺽이지 않은, A[1] = A[0], A[4]=A[5]인 상태 
//</summary>
//<param name="A[6]"></param>
//<param name="nHDan">무조건 nHDan이 도상이라고 생각하고 그림</param>
//<param name="bRoadNotSlab">TRUE=지중라멘의 상부 노면(도로면, 철도면)이며 슬래브면(교면)아님
//							FALSE=일반교량, 지중라멘의 슬래브면(교면)</param>
void CRcBridgeApp::GetXyDosangBase(double sta, CDPoint vAng, long nHDan, CDPoint A[6], BOOL bRoadNotSlab)
{
	long n=0; for(n=0; n<6; n++){
		A[n].x = 0;
		A[n].y = 0;
	}

	double disDosangLeft = GetDisSlabLeftAct(sta, vAng, bRoadNotSlab);
	for(n = 0; n < nHDan; n++)
	{
		disDosangLeft += GetLengthHDanAct(sta, vAng, n, bRoadNotSlab);
	}
	double disDosangRigh = disDosangLeft + GetLengthHDanAct(sta, vAng, nHDan, bRoadNotSlab);
	double dWidthDosang = disDosangRigh - disDosangLeft;
	double disBaseLeft = disDosangLeft;
	double disBaseRigh = disDosangRigh;

	if(m_dDisBaseElevationRL == 0)	//R.L 기준이 아니면
	{
		double dEleLeft = GetXySlabUpperActByDistAngle(sta, disDosangLeft, vAng, bRoadNotSlab).y;
		double dEleRigh = GetXySlabUpperActByDistAngle(sta, disDosangRigh, vAng, bRoadNotSlab).y;

		double giulCant = m_pARcBridgeDataStd->GetLineApp()->GetGiulCant(sta, disDosangLeft);
		double heightCant = giulCant * dWidthDosang;	
		if(dEleLeft >= dEleRigh - heightCant)	//엘리베이션이 높은쪽이 기준, 좌측이 기준
		{
			A[2].y = dEleLeft + GetHeightDosangInput();
			A[3].y = A[2].y + fabs(heightCant);	//우측이 항상 높다고 가정함.
		}
		else
		{
			A[3].y = dEleRigh + GetHeightDosangInput();
			A[2].y = A[3].y + fabs(heightCant);	//좌측이 항상 높다고 가정함
		}	
		A[2].x = disDosangLeft;
		A[3].x = disDosangRigh;
		double giulDosang = 0;
		if(GetSlopeDosang() != 0)		giulDosang = 1/GetSlopeDosang();
		A[0]   = GetMatchPavePoint(sta, A[2], giulDosang,bRoadNotSlab);
		A[1] = A[0];

		A[5]   = GetMatchPavePoint(sta, A[3], -giulDosang,bRoadNotSlab);
		A[4] = A[5];
	}
	else
	{
		double disRailCen = 0;
		if(GetQtyRailNode() <= 0)	return;
		for(long nRail = 0; nRail < GetQtyRailNode(); nRail++)
		{
			disRailCen = GetLengthRailByLine(nRail);
			if(disRailCen > disDosangLeft && disRailCen < disDosangRigh)
				break;
		}
		disBaseLeft = disRailCen - m_pARcBridgeDataStd->GetLineApp()->m_dRailwayGauge/2;
		disBaseRigh = disRailCen + m_pARcBridgeDataStd->GetLineApp()->m_dRailwayGauge/2;

		double giulCant = m_pARcBridgeDataStd->GetLineApp()->GetGiulCant(sta, disBaseLeft);
		double heightRail = GetHeightRail()*cos(atan(giulCant));
		double eleRailLower = m_dDisBaseElevationRL - heightRail;
		if(giulCant > 0)	//엘리베이션이 낮은쪽이 기준
		{
			A[2].y = eleRailLower + (disDosangLeft - disBaseLeft) * giulCant+GetXySlabUpperActByDist(sta,0,bRoadNotSlab).y;
			A[3].y = A[2].y - (disDosangLeft - disDosangRigh) * giulCant;	//우측이 항상 높다고 가정함.
		}
		else
		{
			A[3].y = eleRailLower + (disDosangRigh - disBaseRigh) * giulCant+GetXySlabUpperActByDist(sta,0,bRoadNotSlab).y;
			A[2].y = A[3].y + (disDosangLeft - disDosangRigh) * giulCant;	//좌측이 항상 높다고 가정함
		}	
		A[2].x = disDosangLeft;
		A[3].x = disDosangRigh;
		double giulDosang = 0;
		if(m_dSlopeDosang != 0)		giulDosang = 1/m_dSlopeDosang;
		A[0]   = GetMatchPavePoint(sta, A[2], giulDosang, bRoadNotSlab);
		A[1] = A[0];

		A[5]   = GetMatchPavePoint(sta, A[3], -giulDosang, bRoadNotSlab);
		A[4] = A[5];
	}

}
//<summary>
//xy에서 slope 기울기를 가진 선분이 슬래브 하면(상면-80-250+50)과 교차함.
//포장 상면에서 밑으로 평행이동한 선들과의 접선을 구함.
//거더에서 기본 슬래브 두께선과 만나는 점들을 찾을때 사용
//</summary>
//<param name="*pBxGir">거더 pBx : 선형중심 pBx가 아닐수도 있음.</param>
//<param name="xy">슬래브 상의 원하는 위치</param>
//<param name="slope">xy 좌표에서 이루는 각도(수직으로 올라가면 => "0")</param>
//<param name="bLowBySlabHunch">슬래브 아래 면(기본두께 이하면)과의 교점을 구하지를 결정</param>
//<param name="bRoadNotSlab">TRUE=지중라멘의 상부 노면(도로면, 철도면)이며 슬래브면(교면)아님
//							FALSE=일반교량, 지중라멘의 슬래브면(교면)</param>
CDPoint CRcBridgeApp::GetMatchPavePoint(double staCR, CDPoint xy, double slope, BOOL bRoadNotSlab)
{
	CDPoint vAng = CDPoint(1, slope);
	if(slope == 0)	vAng = CDPoint(0, 1);//90도
	CDPoint xyResult(0,0);
	CDPoint A[30];
	GetXySlabUpperAct(staCR, CDPoint(0, 1), A,bRoadNotSlab);
	long nQtyHDanAct = GetQtyHDanAct(staCR,CDPoint(0,1),bRoadNotSlab);
	long n = 0; for(n = 0; n < nQtyHDanAct; n++)
		if(GetXyMatchSegAndLine(A[n],A[n+1],xy,vAng,xyResult))		break;

	return xyResult;
}
//<summary>
//도상 선을 그림. 인근의 방호벽, 도상과 만나는 선 고려하여 꺽임선 그림
//</summary>
//<param name="*pBx"></param>
//<param name="A[6]"></param>
//<param name="nHDan">무조건 nHDan이 도상이라고 생각하고 그림</param>
//<param name="bRoadNotSlab">TRUE=지중라멘의 상부 노면(도로면, 철도면)이며 슬래브면(교면)아님
//							FALSE=일반교량, 지중라멘의 슬래브면(교면)</param>
void CRcBridgeApp::GetXyDosang(double staCR, CDPoint vAng, long nHDan, CDPoint A[6], BOOL bRoadNotSlab)
{
	GetXyDosangBase(staCR, vAng, nHDan, A,bRoadNotSlab);

	long nQtyHDan = GetQtyHDan(bRoadNotSlab);
	CDPoint xyResult;
	CDPoint xyReal;
	double dis = 0;
	double disOld = 0;
	BOOL bMatch = FALSE;
	BOOL bMatchOverOne = FALSE;		//최소 1번이상 만났을 경우
	BOOL bMatchDosangUpper = FALSE;	//도상 윗면이 만나는 경우 (2번에서 3번 선)
	CDPoint B[9];
	CTwinVectorArray tvArr;
	long n = 0; for(n = 0; n < nHDan; n++)		//도상 좌측과 방호벽, 도상과 만나는 제원을 찾음
	{
		CGuardWallRC* pGW = GetGuardWallByHDan(n,bRoadNotSlab);
		if(!pGW) continue;
		if(pGW->IsTypeHDanRailTotalDosang())	//직결식도상, 자갈도상 이면
		{			
			GetXyDosangBase(staCR, vAng, n, B,bRoadNotSlab);
			bMatch = GetXyMatchSegAndSegArray(A[2], A[0], B, 6, xyReal);
			if(bMatch)
			{
				dis = ~(A[2] - xyReal);
				if(bMatchOverOne == FALSE)
				{
					disOld = dis;
					xyResult = xyReal;
					bMatchOverOne = TRUE;
				}
				if(dis < disOld)	//직선 거리가 짧으면
				{
					xyResult = xyReal;
					bMatchDosangUpper = FALSE;
				}
			}
			bMatch = GetXyMatchSegAndSegArray(A[2], A[3], B, 6, xyReal);//도상 윗면이 만나는 경우 (2번에서 3번 선)
			if(bMatch)
			{
				dis = ~(A[2] - xyReal);
				bMatchDosangUpper = TRUE;
				if(bMatchOverOne == FALSE)
				{
					disOld = dis;
					xyResult = xyReal;
					bMatchOverOne = TRUE;
				}
				if(dis < disOld)	//직선 거리가 짧으면
				{
					xyResult = xyReal;
				}
			}
		}
		else if(!pGW->IsTypeHDanOnlyNone())
		{
			GetTvGuardWall(pGW, tvArr, staCR, vAng, FALSE,bRoadNotSlab);
			bMatch = GetXyMatchSegAndSegTwinVector(A[2], A[0], tvArr, 14, xyReal);
			if(bMatch)
			{
				dis = ~(A[2] - xyReal);
				if(bMatchOverOne == FALSE)
				{
					disOld = dis;
					xyResult = xyReal;
					bMatchOverOne = TRUE;
				}
				if(dis < disOld)	//직선 거리가 짧으면
				{
					xyResult = xyReal;
				}
			}
		}
	}
	if(bMatchOverOne)
	{
		if(bMatchDosangUpper)
		{
			A[2] = xyResult;
			A[1] = A[2];		//도면 작성 표준을 위하여 [1]을 [0]으로 하지않과 [2]로 함
			A[0] = GetMatchPavePoint(staCR, A[2], 0,bRoadNotSlab);
		}
		else
		{
			A[1] = xyResult;
			A[0] = GetMatchPavePoint(staCR, A[1], 0,bRoadNotSlab);
		}

		bMatch = FALSE;
		bMatchOverOne = FALSE;
		bMatchDosangUpper = FALSE;
	}

	for(n = nHDan+1; n < nQtyHDan; n++)		//도상 우측과 방호벽, 도상과 만나는 제원을 찾음
	{
		if(n == nHDan)	continue;
		CGuardWallRC* pGW = GetGuardWallByHDan(n,bRoadNotSlab);
		if(!pGW)	continue;
		if(pGW->IsTypeHDanRailTotalDosang())	//직결식도상, 자갈도상 이면
		{
			GetXyDosangBase(staCR, vAng, n, B,bRoadNotSlab);
			bMatch = GetXyMatchSegAndSegArray(A[3], A[5], B, 6, xyReal);
			if(bMatch)
			{
				dis = ~(A[3] - xyReal);
				if(bMatchOverOne == FALSE)
				{
					disOld = dis;
					xyResult = xyReal;
					bMatchOverOne = TRUE;
				}
				if(dis < disOld)	//직선 거리가 짧으면
				{
					xyResult = xyReal;
					bMatchDosangUpper = FALSE;
				}
			}
			bMatch = GetXyMatchSegAndSegArray(A[2], A[3], B, 6, xyReal);	//도상 윗면이 만나는 경우 (2번에서 3번 선)
			if(bMatch)
			{
				dis = ~(A[3] - xyReal);
				bMatchDosangUpper = TRUE;
				if(bMatchOverOne == FALSE)
				{
					disOld = dis;
					xyResult = xyReal;
					bMatchOverOne = TRUE;
				}
				if(dis < disOld)	//직선 거리가 짧으면
				{
					xyResult = xyReal;
				}
			}
		}
		else if(!pGW->IsTypeHDanOnlyNone())	//방호벽 타입이면
		{
			GetTvGuardWall(pGW, tvArr, staCR, vAng, FALSE,bRoadNotSlab);
			bMatch = GetXyMatchSegAndSegTwinVector(A[3], A[5], tvArr, 14, xyReal);
			if(bMatch)
			{
				dis = ~(A[3] - xyReal);
				if(bMatchOverOne == FALSE)
				{
					disOld = dis;
					xyResult = xyReal;
					bMatchOverOne = TRUE;
				}
				if(dis < disOld)	//직선 거리가 짧으면
				{
					xyResult = xyReal;
				}
			}
		}
	}
	if(bMatchOverOne)
	{
		if(bMatchDosangUpper)
		{
			A[3] = xyResult;
			A[4] = A[3];		//도면 작성 표준을 위하여 [1]을 [0]으로 하지않과 [2]로 함
			A[5] = GetMatchPavePoint(staCR, A[3], 0,bRoadNotSlab);
		}
		else
		{
			A[4] = xyResult;
			A[5] = GetMatchPavePoint(staCR, A[4], 0,bRoadNotSlab);
		}
	}
}
double CRcBridgeApp::GetLengthRailByLineByCenLine(double sta, CDPoint vAng, long nRail)
{
	double disRail = GetLengthRailByLine(nRail);
	long nLineRail = GetRailLineNumber(nRail);
	if(nLineRail == 0)
		return disRail;

	CLineInfo *pLine = (static_cast<CLineInfoApp*>(GetLineBase()))->GetLine(nLineRail);
	double disLineToLine = pLine->GetDisMatchAngByTargetLine(pLine, sta, vAng);
	double disRailAct = disRail + disLineToLine;
	return disRailAct;
}
//도상은 지중라멘일 경우 슬래브면(교면)은 표현하지 않음.
BOOL CRcBridgeApp::IsMatchDosangByDis(double sta, CDPoint vAng, double disFrCenter, CDPoint &xyResult)
{
	CDPoint xy1 = CDPoint(disFrCenter, 10000000);
	CDPoint xy2 = CDPoint(disFrCenter, -10000000);
	CDPoint A[6]; 
	BOOL	bMatch = FALSE;
	long nQtyHDan = GetQtyHDan(m_bUnderGroundRahmen);	//지중라멘교일 경우에는 슬래브면이 아닌 철로면
	for(long nHDan = 0; nHDan < nQtyHDan; nHDan++)//지중라멘일 경우에는 철로 상면 기준으로만 찾음. 슬래브 면은 무시함
	{
		CGuardWallRC* pGW = NULL;
		pGW = GetGuardWallByHDan(nHDan, m_bUnderGroundRahmen);
		if(!pGW) continue;
		if(pGW->IsTypeHDanRailTotalDosang())	//직결식도상, 자갈도상 이면
		{
			GetXyDosang(sta, vAng, nHDan, A,m_bUnderGroundRahmen);// 도상의 포인트
			bMatch = GetXyMatchSegAndSegArray(xy1, xy2, A, 6, xyResult);
			if(bMatch)
				return bMatch;
		}
	}
	return FALSE;
}

//<summary>
//실제 슬래브 상면의 값을 구함.
//</summary>
//<param name="sta"></param>
//<param name="vAng"></param>
//<param name="A[30]"></param>
//<param name="bRoadNotSlab">TRUE=지중라멘의 상부 노면(도로면, 철도면)이며 슬래브면(교면)아님
//							 FALSE=일반교량, 지중라멘의 슬래브면(교면)</param>
void CRcBridgeApp::GetXySlabUpperAct(double sta, CDPoint vAng, CDPoint A[30], BOOL bRoadNotSlab, BOOL bExp)
{
	long n=0; for(n=0; n<30; n++)
	{
		A[n] = CDPoint(0,0);
		m_Aslope[n] = CDPoint(0,0);
	}
	m_nQtyHDanSlope = 0;
	m_dDisExpLeft = 0;
	m_dDisExpRight = 0;
	m_bSetGagakDist = FALSE;

	// 기존에 저장된 것이 있는지 체크 /////
// 	long i = 0; 
// 	for(i = 0; i < m_pArrSaveCrossSection.GetSize(); i++)
// 	{
// 		CSaveCrossSection *pSave	= m_pArrSaveCrossSection.GetAt(i);
// 		if(pSave->IsMatch(sta, vAng, bRoadNotSlab, bExp))
// 		{
// 			pSave->GetValue(A);			
// 			return;
// 		}
// 	}


	if(Compare(vAng.GetAngleDegree(), 90.0, "="))
	{
		GetXySlabUpperAct90(sta, A, bRoadNotSlab, bExp);
	}
	else
	{
		if(bExp)
		{
			SetXySlabUpperAct90_Gagak(sta,vAng);
		}
		GetXySlabUpperActAngle(sta, vAng, A, bRoadNotSlab, bExp);
	}
	m_bSetGagakDist = FALSE;

	// 기존에 저장된 것이 없으면 추가
	CSaveCrossSection *pSave	= new CSaveCrossSection;
	pSave->SetValue(sta, vAng, bRoadNotSlab, bExp, A);
	m_pArrSaveCrossSection.Add(pSave);
}

//<summary>
//실제 슬래브 상면의 값을 구함.
//</summary>
//<param name="sta"></param>
//<param name="vAng"></param>
//<param name="A[30]"></param>
//<param name="bRoadNotSlab">TRUE=지중라멘의 상부 노면(도로면, 철도면)이며 슬래브면(교면)아님
//							 FALSE=일반교량, 지중라멘의 슬래브면(교면)</param>
void CRcBridgeApp::GetXySlabUpperAct90(double sta, CDPoint A[30], BOOL bRoadNotSlab, BOOL bExp)
{

	CDPoint vAng = CDPoint(0,1);
	double disSlabLeft	= bExp ? GetDisSlabLeftActByGagak(sta, vAng, bRoadNotSlab) : GetDisSlabLeftAct(sta, vAng, bRoadNotSlab);
	double eleSlabLeft	= 0;

	BOOL bExpSkewGagak = (bExp && IsSkew() && m_nQtyHDanSlope>0) ? TRUE : FALSE;

	double x = disSlabLeft;
	double y = eleSlabLeft;

	if(bExpSkewGagak && m_bSetGagakDist) 
		x = m_dDisExpLeft;

	A[0].x = x;
	A[0].y = y;

	long nHDan = 0;		//입력한 상부 절점수
	long nHDanAct = 0;	//실제 상부 절점수. nHdan 과 같거나 1개 더 많을 수 있음.(선형중심이 슬래브내에 있을 경우)
	
	double heightFrSlabLeftToCen = 0;	//선형중심의 높이
	
	double staSc = sta;
	long nQtyHDan = GetQtyHDan(bRoadNotSlab);
	for(nHDan = 0; nHDan<nQtyHDan; nHDan++)
	{
		double slope = 0;	//좌측 슬래브끝단의 편경사
		slope = GetSlopeHDanByStation(staSc, nHDan, bRoadNotSlab);
		if(Compare(A[nHDanAct].x, 0.0, "<"))	slope = -slope; 
		double dLengthHDanAct = 0;
		dLengthHDanAct = GetLengthHDanAct(sta, vAng, nHDan, bRoadNotSlab, bExp);
		if(Compare(A[nHDanAct].x, 0.0, "<", DIST_MIN_CROSS_CENTER) && Compare(A[nHDanAct].x + dLengthHDanAct, 0.0, ">", DIST_MIN_CROSS_CENTER))	//선형중심을 가로지르는 횡단은 도로중심에서 분리하여 추가
		{
			double disFrZero = dLengthHDanAct + A[nHDanAct].x;
			A[nHDanAct+1].x = 0;
			if(bExpSkewGagak)
			{
				slope = GetSlopeHDanByDistGagak(A[nHDanAct+1].x);
			}
			A[nHDanAct+1].y = A[nHDanAct].y - A[nHDanAct].x*slope;
			heightFrSlabLeftToCen = A[nHDanAct+1].y;			// 선형중심의 높이
			if(nHDan != nQtyHDan && slope!=0.00 && !Compare(A[nHDanAct].x, 0.0, "="))	// LEVEL인 경우는 slope를 재계산할 필요 없음...
				slope = GetSlopeHDanByStation(staSc, nHDan+1, bRoadNotSlab, nHDan);
			A[nHDanAct+2].x = disFrZero;
			if(bExpSkewGagak)
			{
				slope = GetSlopeHDanByDistGagak(A[nHDanAct+2].x);
			}
			A[nHDanAct+2].y = A[nHDanAct+1].y + disFrZero*vAng.y * slope;

			nHDanAct++;
		}
		else
		{
			A[nHDanAct+1].x = A[nHDanAct].x + dLengthHDanAct;
			if(bExpSkewGagak)
			{
				slope = GetSlopeHDanByDistGagak(A[nHDanAct].x);
			}
			A[nHDanAct+1].y = A[nHDanAct].y + dLengthHDanAct*vAng.y*slope;
		}
		nHDanAct++;
	}

	//if(!(m_bUnderGroundRahmen && !bRoadNotSlab))
	// 지중라멘의 슬래브는 이 옵션을 안 먹었으나, 2007.10.30부로 먹음
	// 단, 도로중심기준 옵션은 지중라멘의 슬래브용 변수를 하나 더 만들어서 사용함
	BOOL bLevelFrRoadCenterToEdge	= m_bUnderGroundRahmen && !bRoadNotSlab ? m_bLevelFrRoadCenterToEdgeOfUnderGroundRahmenSlab : m_bLevelFrRoadCenterToEdge;
	double dDistRoadCenterLevelLeft  = m_dDistRoadCenterLevelLeft;
	double dDistRoadCenterLevelRight = m_dDistRoadCenterLevelRight;
	if(Compare(A[0].x, 0.0, _T(">="), 0.01))	//좌측 슬래브 끝단이 선형중심의 우측에 있을 경우, 레벨 여부(m_cLevelRoadCenterToSlabEdge)는 항상 레벨
	{
		if(bLevelFrRoadCenterToEdge==FALSE)
		{
			double slopePyun = m_bUnderGroundRahmen ? GetSlopeHDan(0) : GetLineBase()->GetPyungubaeRight(sta, TRUE);
			heightFrSlabLeftToCen = -(disSlabLeft-dDistRoadCenterLevelRight)*slopePyun;
		}
		else
			heightFrSlabLeftToCen = 0;
	}
	else if(Compare(A[nHDanAct].x, 0.0, _T("<="), 0.01))	//좌측 슬래브 끝단이 선형중심의 우측에 있을 경우, 레벨 여부(m_cLevelRoadCenterToSlabEdge)는 항상 레벨
	{
		if(bLevelFrRoadCenterToEdge==FALSE)
		{
			double slopePyun = m_bUnderGroundRahmen ? GetSlopeHDan(nHDanAct-1) : GetLineBase()->GetPyungubaeRight(sta, FALSE);
			double dDisSlabRight = GetDisSlabRighActByGagak(sta, vAng, bRoadNotSlab);
			heightFrSlabLeftToCen = A[nHDanAct].y+(dDisSlabRight+dDistRoadCenterLevelLeft)*slopePyun;
		}
		else
			heightFrSlabLeftToCen = A[nHDanAct].y;
	}
	else	//슬래브 좌우 사이에 선형중심이 있을 경우
	{
		CDPoint xyResult;
		BOOL bMatch = GetXyMatchSegAndSegArray(CDPoint(0, 1000000000), CDPoint(0, -1000000000), A, nHDanAct, xyResult);
		if(bMatch)
			heightFrSlabLeftToCen = xyResult.y;
	}
	
	long nQtyHDanAct = GetQtyHDanAct(sta, vAng,bRoadNotSlab,bExp);
	for(nHDanAct=0; nHDanAct < nQtyHDanAct+1; nHDanAct++)	//선형중심 위치의 높이를 "0"로 설정함.
		A[nHDanAct].y -= heightFrSlabLeftToCen;


	if(IsApplyDisBaseElevationFL())	//철도교량이면
	{
		if(m_dDisBaseElevation != 0)
		{
			CDPoint xyResult;
			BOOL bMatch = GetXyMatchSegAndSegArray(CDPoint(m_dDisBaseElevation, 1000000000), CDPoint(m_dDisBaseElevation, -1000000000), A, nQtyHDanAct+1, xyResult);
			if(bMatch)
				for(nHDanAct = 0; nHDanAct < nQtyHDanAct+1; nHDanAct++)
					A[nHDanAct].y -= xyResult.y;
		}
	}
	double elePlan = 0;
	for(nHDanAct = 0; nHDanAct <nQtyHDanAct+1; nHDanAct++)	//지중라멘이고 슬래브면(교면)일 경우에만 입력한 엘리베이션 사용 그리고 수량산출을 위해서 만들어졌을때도
	{
		staSc = GetLineBase()->GetStationByScInLine(sta, A[nHDanAct].x*vAng.y, vAng);
		if((m_bUnderGroundRahmen && !bRoadNotSlab) || m_bBridgeForBM == TRUE)
			elePlan = m_pLineApp->GetElevationPlanEl(staSc);
		else
			elePlan = m_pARcBridgeDataStd->GetLineApp()->GetElevationPlanEl(staSc);
		A[nHDanAct].y += (elePlan-GetThickPave(FALSE));
	}
}

//<summary>
//실제 슬래브 상면의 값을 구함.
//</summary>
//<param name="sta"></param>
//<param name="vAng"></param>
//<param name="A[30]"></param>
//<param name="bRoadNotSlab">TRUE=지중라멘의 상부 노면(도로면, 철도면)이며 슬래브면(교면)아님
//							 FALSE=일반교량, 지중라멘의 슬래브면(교면)</param>
void CRcBridgeApp::GetXySlabUpperActAngle(double sta, CDPoint vAng, CDPoint A[30], BOOL bRoadNotSlab, BOOL bExp)
{
	double disSlabLeft	= bExp ? GetDisSlabLeftActByGagak(sta, vAng, bRoadNotSlab) : GetDisSlabLeftAct(sta, vAng, bRoadNotSlab);

	A[0].x = disSlabLeft;
	A[0].y = GetXySlabUpperActByDistAngle(sta, A[0].x, vAng, bRoadNotSlab, bExp).y;

	long nQtyHDan = GetQtyHDan(bRoadNotSlab);
	long nHDan = 0;		//입력한 상부 절점수
	long nHDanAct = 0;	//실제 상부 절점수. nHdan 과 같거나 1개 더 많을 수 있음.(선형중심이 슬래브내에 있을 경우)
	CDPoint vAngReal	= vAng;
	for(nHDan = 0; nHDan<nQtyHDan; nHDan++)
	{
		double dLengthHDanAct = 0;
		dLengthHDanAct = GetLengthHDanAct(sta, vAng, nHDan, bRoadNotSlab, bExp, vAngReal);

		// 입력을 해도 소수점 세째자리 까지 이므로 비교도 그정도에서만...
		if(Compare(A[nHDanAct].x, 0.0, "<", DIST_MIN_CROSS_CENTER) && Compare(A[nHDanAct].x + dLengthHDanAct, 0.0, ">", DIST_MIN_CROSS_CENTER))	//선형중심을 가로지르는 횡단은 도로중심에서 분리하여 추가
		{
			double disFrZero = dLengthHDanAct + A[nHDanAct].x;
			A[nHDanAct+1] = GetXySlabUpperActByDist(sta, 0, bRoadNotSlab, bExp);//도로중심 위치
			A[nHDanAct+2] = GetXySlabUpperActByDistAngle(sta, disFrZero, vAng, bRoadNotSlab, bExp);

			nHDanAct++;
		}
		else
		{
			A[nHDanAct+1].x = A[nHDanAct].x + dLengthHDanAct;
			A[nHDanAct+1].y = GetXySlabUpperActByDistAngle(sta, A[nHDanAct+1].x, vAng, bRoadNotSlab, bExp).y;
		}
		nHDanAct++;
	}
}

// 횡단 좌표 중 가장 높은 y를 구함
//<param name="bRoadNotSlab">TRUE=지중라멘의 상부 노면(도로면, 철도면)이며 슬래브면(교면)아님
//							FALSE=일반교량, 지중라멘의 슬래브면(교면)</param>
double CRcBridgeApp::GetElMaxSlabUpper(double sta, CDPoint vAng, BOOL bRoadNotSlab)
{
	vector<double> el;
	CDPoint A[30];
	GetXySlabUpperAct(sta, vAng, A,bRoadNotSlab);

	long nQtyHDan = GetQtyHDanAct(sta,vAng,bRoadNotSlab);
	int i=0; for(i=0; i<=nQtyHDan; i++)
		el.push_back(A[i].y);

	return nQtyHDan>0 ? *max_element(el.begin(), el.end()) : 0;
}

// 횡단 좌표 중 가장 낮은 y를 구함
//<param name="bRoadNotSlab">TRUE=지중라멘의 상부 노면(도로면, 철도면)이며 슬래브면(교면)아님
//							FALSE=일반교량, 지중라멘의 슬래브면(교면)</param>
double CRcBridgeApp::GetElMinSlabUpper(double sta, CDPoint vAng, BOOL bRoadNotSlab)
{
	vector<double> el;
	CDPoint A[30];
	GetXySlabUpperAct(sta, vAng, A,bRoadNotSlab);

	long nQtyHDan = GetQtyHDanAct(sta,vAng,bRoadNotSlab);
	int i=0; for(i=0; i<=nQtyHDan; i++)
		el.push_back(A[i].y);

	return nQtyHDan>0 ? *min_element(el.begin(), el.end()) : 0;
}

// 슬래브 헌치의 상단 포인트
void CRcBridgeApp::GetPointArraySlabHunchAct(double sta, CDPoint vAng, CDPointArray& xyArr, BOOL bExp)
{
	double dDHL = m_dDH[0]/vAng.y;
	double dDHR = m_dDH[1]/vAng.y;

	xyArr.RemoveAll();
	GetPointArraySlabLowerAct(sta, vAng, xyArr, bExp);

	long nQtyHDan = xyArr.GetSize()-1;
	double dStt = xyArr.GetAt(0).x + dDHL;
	double dEnd = xyArr.GetAt(nQtyHDan).x - dDHR;
	CDPoint xyFir(0, 0), xySec(0, 0);
	CDPoint xyResult(0, 0);
	// 좌측단
	nQtyHDan	= xyArr.GetSize();
	int i=0; for(i=0; i<nQtyHDan-1; i++)
	{
		xyFir = xyArr.GetAt(i);
		xySec = xyArr.GetAt(i+1);
		if(GetXyMatchSegAndLine(xyFir, xySec, CDPoint(dStt, 0), CDPoint(0, 1), xyResult))
		{
			xyArr.SetAt(i, xyResult);
			break;
		}
		else
		{
			xyArr.RemoveAt(i);
			nQtyHDan = xyArr.GetSize();
			i--;
		}
	}

	// 우측단
	nQtyHDan = xyArr.GetSize();
	for(i=nQtyHDan-1; i>0; i--)
	{
		xyFir = xyArr.GetAt(i);
		xySec = xyArr.GetAt(i-1);
		if(GetXyMatchSegAndLine(xyFir, xySec, CDPoint(dEnd, 0), CDPoint(0, 1), xyResult))
		{
			xyArr.SetAt(i, xyResult);
			break;
		}
		else
		{
			xyArr.RemoveAt(i);
			nQtyHDan = xyArr.GetSize();
		}
	}
}

//기능 : 도로중심 좌측 레벨의 총길이
//<summary>
//각도에 따른 실제 값이 나옴.
//</summary>
//<param name="sta"></param>
//<param name="vAng"></param>
//<return></return>
//<param name="bRoadNotSlab">TRUE=지중라멘의 상부 노면(도로면, 철도면)이며 슬래브면(교면)아님
//							FALSE=일반교량, 지중라멘의 슬래브면(교면)</param>
// double CRcBridgeApp::GetDisSlabLeftLevelAct(double sta, CDPoint vAng, BOOL bRoadNotSlab)
// {
// 	double	disSlabLeft =  GetDisSlabLeftAct(sta, vAng,bRoadNotSlab);	//도로중심에서 슬래브 좌측까지의 길이
// 	if(disSlabLeft >= 0)	return 0;		//슬래브 좌측이 도로중심 우측에 있을때
// 
// 	double lenTotal = 0;
// 	double	dDisLevel	= 0;		//도로중심 좌측의 레벨 총 거리
// 	long nHDan=0; for(nHDan=0; nHDan<GetQtyHDan(bRoadNotSlab); nHDan++)
// 	{
// 		lenTotal += GetLengthHDanAct(sta, vAng, nHDan,bRoadNotSlab);
// 		if(IsLevelHDanApplyLineSlope(nHDan,bRoadNotSlab))		//Level 이면
// 		{
// 			if((lenTotal <= -disSlabLeft))
// 			{
// 				dDisLevel += GetLengthHDan(nHDan,bRoadNotSlab);
// 			}
// 			else if((lenTotal > -disSlabLeft) && (lenTotal - GetLengthHDanAct(sta, vAng, nHDan,bRoadNotSlab) < -disSlabLeft))//레벨 구간이 도로중심에 걸쳤을 때
// 			{
// 				dDisLevel += (-disSlabLeft - (lenTotal - GetLengthHDanAct(sta, vAng, nHDan,bRoadNotSlab)));
// 				break;
// 			}
// 		}
// 	}
// 	//////////////////////////////////////////////////////////////////////////
// 	double	widSlab = GetWidthSlabAct(sta,vAng);
// 	if(-disSlabLeft > widSlab)	//슬래브 우측이 도로중심 좌측에 있고 레벨 타입일 경우을 경우
// 	{
// 		if(m_bLevelFrRoadCenterToEdge)	//레벨 타입일 경우
// 		{
// 			dDisLevel += (-disSlabLeft - widSlab);
// 		}
// 		else
// 		{
// 			if(m_dDistRoadCenterLevelLeft > 0)
// 			{
// 				double disSlabRighToCen = (-disSlabLeft - widSlab);
// 				if(m_dDistRoadCenterLevelLeft < disSlabRighToCen)
// 					dDisLevel += m_dDistRoadCenterLevelLeft;
// 				else
// 					dDisLevel += disSlabRighToCen;	//레벨 타입이 아닐 경우라도 m_dDistRoadCenterLevelLeft 가 클 경우 레벨 타입과 동일하게 됨.
// 			}
// 		}
// 	}
// 	return fabs(dDisLevel);
// //	return dDisLevel;
// }

//<param name="bRoadNotSlab">TRUE=지중라멘의 상부 노면(도로면, 철도면)이며 슬래브면(교면)아님
//							FALSE=일반교량, 지중라멘의 슬래브면(교면)</param>
// double CRcBridgeApp::GetDisSlabRightLevelAct(double sta, CDPoint vAng, BOOL bRoadNotSlab)
// {
// 	CDPoint vAng90 = CDPoint(0, 1);
// 	double	width = GetWidthSlabAct(sta, vAng90,0,FALSE,FALSE,bRoadNotSlab);
// 	double	disSlabLeft =  GetDisSlabLeftAct(sta, vAng90,bRoadNotSlab);
// 	double	disSlabRight =  disSlabLeft + width;
// 	if(disSlabRight <= 0)	return 0;
// 	double	disSlabLeftLevel =  GetDisSlabLeftLevelAct(sta, vAng90,bRoadNotSlab);
// 
// 	double	dDisLevel	= 0;
// 	long nHDan=0; for(nHDan=0; nHDan<GetQtyHDan(bRoadNotSlab); nHDan++)
// 	{
// 		if(IsLevelHDanApplyLineSlope(nHDan))		//Level 이면
// 			dDisLevel += GetLengthHDan(nHDan,bRoadNotSlab);
// 	}
// 	return (dDisLevel - disSlabLeftLevel) / vAng.y;
// }
//<summary>
//목적 : 도로중심(확폭일 경우 본선) 스테이션에서 직각으로 올렸을때의 슬래브 끝단간 길이
//입력 : sta	도로중심(확폭일 경우 본선) 스테이션
//기능 : 슬래브 끝단에서 끝단까지의 거리
//</summary>
//<param name="sta"></param>
//<return></return>
// long nLeft : -1 중심에서 좌측까지 거리, 0 : 양 끝단간 거리, 1 : 중심에서 우측까지 거리
// BOOL bHunch : 헌치부 너비인지?(수량때문에 만듬)
// BOOL bVertDir : 직방향 중심선형용인지
//<param name="bRoadNotSlab">TRUE=지중라멘의 상부 노면(도로면, 철도면)이며 슬래브면(교면)아님
//							FALSE=일반교량, 지중라멘의 슬래브면(교면)</param>
// CDPoint vAngByApplyAngle : CDPoint(0, 0)이면 시점 각도 사용, 아니면 vAngByApplyAngle을 사용
double CRcBridgeApp::GetWidthSlabAct(double sta, CDPoint vAng, long nLeft, BOOL bHunch, BOOL bVertDir, BOOL bRoadNotSlab, CDPoint vAngByApplyAngle)
{
	double dSta	= sta;

	// 직방향 중심선형용이면
	double dDiff	= 0;
	CDPoint xyDirDiff	= vAng;
	if(bVertDir)
	{
		CDPoint vAngVert = vAng;
		if(ComparePt(vAngByApplyAngle, CDPoint(0, 0), "="))
			vAng = GetAngleJijum(0, FALSE);		//ToDPointFrDegree(GetAngleJijum(0).GetAngleDegree() - (90 - vAngVert.GetAngleDegree()));
		else
			vAng	= vAngByApplyAngle;

		CLineInfo *pLineBaseVert	= GetLineBase(TRUE);
		CLineInfo *pLineBase		= GetLineBase();
		CDPoint xyStaVert			= pLineBaseVert->GetXyLineByLengthAndAngle(sta, 0, vAngVert);
		
		pLineBase->GetStationMatchByXyAndAngle(xyStaVert, GetAngleAzimuthJijum(0), dSta);	// 이함수는 특이하게 방위각 이동 
		CDPoint xySta = pLineBase->GetXyLineByLengthAndAngle(dSta, 0, vAng);
		dDiff		= ~(xySta-xyStaVert);
		xyDirDiff	= (xyStaVert-xySta).Unit();
	}

	CDPoint xyL	= GetXySlabEdgeAct(dSta, vAng, TRUE,bRoadNotSlab);
	CDPoint xyR	= GetXySlabEdgeAct(dSta, vAng, FALSE,bRoadNotSlab);
	CDPoint xyC	= GetLineBase()->GetXyLine(dSta);

	if(bVertDir)
		xyC	= xyC + xyDirDiff * dDiff;

	double dWidth	= 0;

	// 좌측 너비 구하기
	if(nLeft==-1)
	{
		dWidth = ~(xyL-xyC);

		// 좌측너비가 -가 되는 경우도 있을 수 있다.
		// 사보강일땐 선형자체가 슬래브 중심이므로 -는 절대 없음
		if(Compare(~(xyR-xyC), ~(xyL-xyR)+~(xyL-xyC), "=") && !bVertDir)
			dWidth *= -1;
		
		if(bHunch)
			dWidth -= m_dDH[0];
	}
	// 우측 너비 구하기
	else if(nLeft==1)
	{
		dWidth = ~(xyR-xyC);

		// 우측너비가 -가 되는 경우도 있을 수 있다.
		// 사보강일땐 선형자체가 슬래브 중심이므로 -는 절대 없음
		//if(m_dWidthSlabRight < 0 && !bVertDir)
		// xyL과 xyC사이에 xyR이 있다면
		if(Compare(~(xyL-xyC), ~(xyL-xyR)+~(xyR-xyC), "=") && !bVertDir)
			dWidth *= -1;

		if(bHunch)
			dWidth -= m_dDH[1];
	}
	else
	{
		dWidth = ~(xyL-xyR);
		if(bHunch)
			dWidth -= (m_dDH[0] + m_dDH[1]);
	}

	return dWidth;
}


//<summary>
//
//</summary>
//<param name="nJ"></param>
//<param name="nLeft"></param>
//<param name="nRow"></param>
//<return></return>
CDPoint CRcBridgeApp::GetXyShoe(long nJ, long nLeft, long nRow)
{
	double sta = GetStationShoeOnJijum(nJ, 0, nLeft);	//도로중심의 슈 스테이션
	CJijum *pJ = GetJijum(nJ);
	CDPoint vAng = GetAngleShoeOnJijum(nJ, 0, nLeft);;	//도로중심의 슈 위치 지점각도

	double dDistShoe = pJ->m_dDistShoe[nLeft][nRow];
	double staSc = GetLineBase()->GetStationByScInLine(sta, dDistShoe, vAng);
	CDPoint xyShoe = GetLineBase()->GetXyLineDis(staSc, dDistShoe);
	if(!IsExtLine())	//확폭이 아니면
		return xyShoe;

	return xyShoe;
}

//<summary>
//
//</summary>
//<param name="nJ"></param>
//<param name="nLeft"></param>
//<param name="nRow"></param>
//<return></return>
CDPoint CRcBridgeApp::GetAngleShoe(long nJ, long nLeft, long nRow, long Dir, long nSlab, long FromJijum, long ToJijum)
{
	CLineInfo *pLine = GetLineBase();
	CJijum *pJ = GetJijum(nJ);

	double  staJijum  = GetStationShoeOnJijum(nJ, 0, nLeft);	//도로중심의 슈 스테이션
	CDPoint vAngJijum = GetAngleShoeOnJijum(nJ, 0, nLeft);;		//도로중심의 슈 위치 지점각도

	double dDistShoe = pJ->m_dDistShoe[nLeft][nRow];
	double staShoe   = pLine->GetStationByScInLine(staJijum, dDistShoe, vAngJijum);

	CDPoint vAngShoe = CDPoint(0,0);

	if(m_nTypeShoeAngle==0)
	{
		vAngShoe = pLine->GetAngleAzimuth(staShoe);
	}
	
	if(m_nTypeShoeAngle==1)
	{
		CDPoint vAngLine = pLine->GetAngleAzimuth(staShoe);
		vAngShoe = vAngJijum.Rotate90();
		vAngShoe = vAngShoe.Rotate(vAngLine);
	}
	
	if(m_nTypeShoeAngle==2)
	{
		long nJijumFix = 0;
		long nLeftFix  = 0;
		long nRowFix   = 0;

		CDPoint xyShoe = GetXyShoe(nJ,nLeft,nRow);
		GetIndexNearestFixShoe(xyShoe,nJijumFix,nLeftFix,nRowFix,FromJijum,ToJijum);
		CDPoint xyFix  = GetXyShoe(nJijumFix,nLeftFix,nRowFix);
		
		if(IsExistFixedJijum(nSlab))
			vAngShoe = (xyFix-xyShoe).Unit();
		else
			vAngShoe = pLine->GetAngleAzimuth(staShoe);
		
		if(Dir==2)		//symbol이 수직이면 자신이 가진 각도90도를 빼주어야 한다.
			vAngShoe = vAngShoe.Rotate90();
	}
	
	if(m_nTypeShoeAngle==3)
	{
		long nJijumFix = 0;
		long nLeftFix  = 0;
		long nRowFix   = 0;

		CDPoint xyShoe = GetXyShoe(nJ,nLeft,nRow);
		GetIndexNearestFixShoe(xyShoe,nJijumFix,nLeftFix,nRowFix,FromJijum,ToJijum);

		if(nJijumFix==nJ)
		{
			CDPoint TargetShoe = GetXyShoe(nJijumFix,nLeftFix,nRowFix);
			if(IsExistFixedJijum(nSlab))
				vAngShoe = (TargetShoe-xyShoe).Unit();
			else
				vAngShoe = pLine->GetAngleAzimuth(staShoe);
		}
		else
		{
			long nTargetJijum = nJijumFix;
			long nTargetLeft  = nLeftFix;

			CJijum* pTargetJ = NULL; 
			pTargetJ = GetJijum(nTargetJijum);
			
			long nTargetCountShoe = pTargetJ->m_nQtyShoe[nTargetLeft];
			
			if((nRow+1)<=nTargetCountShoe)
			{
				CDPoint TargetShoe = GetXyShoe(nJijumFix,nLeftFix,nRow);
				if(IsExistFixedJijum(nSlab))
					vAngShoe = (TargetShoe-xyShoe).Unit();
				else
					vAngShoe = pLine->GetAngleAzimuth(staShoe);
			}
			else
			{
				CDPoint TargetShoe = GetXyShoe(nJijumFix, nLeftFix, nTargetCountShoe-1);
				if(IsExistFixedJijum(nSlab))
					vAngShoe = (TargetShoe-xyShoe).Unit();
				else
					vAngShoe = pLine->GetAngleAzimuth(staShoe);
			}
		}

		if(Dir==2)		//symbol이 수직이면 자신이 가진 각도90도를 빼주어야 한다.
			vAngShoe = vAngShoe.Rotate90();
	}

	return vAngShoe;
}
//<param name="bRoadNotSlab">TRUE=지중라멘의 상부 노면(도로면, 철도면)이며 슬래브면(교면)아님
//							FALSE=일반교량, 지중라멘의 슬래브면(교면)</param>
BOOL CRcBridgeApp::IsLeftHDan(long nHDan, BOOL bRoadNotSlab)
{
	double dSta	= GetStationBridgeMid();
	CDPoint vAng = CDPoint(0, 1);


	// (ARCBRIDGE-1488) 이함수내용은 선형을 중심으로 선형 왼쪽에 있는지 판단하는 함수다.
	// 하지만 사용 하는 부분을 보면 슬래브 중심을 기준으로 좌우를 판단하는게 맞다.
	// XXXX 단 bRoadNotSlab가 TRUE이면 그냥 선형 좌우측으로 판단하자. XXXX
	CLineInfo *pLine	= GetLineBase();
	CDPoint xyLeft	= GetXySlabEdgeAct(dSta, vAng, TRUE, bRoadNotSlab);
	CDPoint xyRight	= GetXySlabEdgeAct(dSta, vAng, FALSE, bRoadNotSlab);
	double dDistLeft	= pLine->GetDisMatchVerticalByXy(xyLeft);
	double dDistRight	= pLine->GetDisMatchVerticalByXy(xyRight);
	double dDistMid = (dDistLeft + dDistRight) / 2;

	long n = 0; for(n = 0; n<nHDan; n++)
	{
		dDistLeft += GetLengthHDanAct(dSta, vAng, n,bRoadNotSlab);
	}

	// 이렇게 하면 선형중심을 기준으로
	// if(Compare(dDistLeft,0.0, "<"))	return TRUE;

	if(Compare(dDistLeft,dDistMid, "<"))	return TRUE;
	
	return FALSE;
}
//<summary>
// 도로 중심으로부터 임의의 거리만큼 떨어진 곳의 횡단 좌표(xy.x = 도로중심 0 일경우 거리, xy.y = 엘리베이션)
//                             xy.x == 항상 D
//</summary>
//<param name="sta">도로중심 스테이션</param>
//<param name="D">사방향 거리(선형에 직각 거리가 아님)</param>
//<param name="bRoadNotSlab">TRUE=지중라멘의 상부 노면(도로면, 철도면)이며 슬래브면(교면)아님
//							FALSE=일반교량, 지중라멘의 슬래브면(교면)</param>
//<param name="bExtendSlop"> 횡단을 벗어난 구간에 경사를 적용할 것인지, TRUE -> LEVEL로 처리</param>
// BOOL bUseRealEL : 횡단벗어난 경우 실제 EL사용할지? 
CDPoint CRcBridgeApp::GetXySlabUpperActByDist(double sta, double D, BOOL bRoadNotSlab, BOOL bExp, BOOL bExtendSlop, BOOL bUseRealEL)
{
	if(!IsExp(sta))
		bExp	= FALSE;

	CDPoint A[30];
	GetXySlabUpperAct90(sta, A, bRoadNotSlab, bExp);

	CLineInfo *pLine	= GetLineBase();
	CDPoint xy(0, 0);
	CDPoint xyResult(0, 0);
	long nQtyHDanAct = GetQtyHDanAct(sta,CDPoint(0,1),bRoadNotSlab);
	for(long nHDanAct=0; nHDanAct<nQtyHDanAct+1; nHDanAct++)
	{
		if(nHDanAct==0 && D < A[nHDanAct].x)
		{
			if(!bUseRealEL)
			{
				if(bExtendSlop)
				{
					GetXyMatchSegAndLine(A[nHDanAct], A[nHDanAct+1], CDPoint(D, 0), CDPoint(0, 1), xyResult);
					xy = xyResult;
				}
				else
					xy = A[nHDanAct];
			}
			else
			{
				double dTP	= GetThickPave();
				xy.x	= D;
				xy.y	= pLine->GetElevationPlanEl(sta) - dTP;
			}
			break;
		}
		if(nHDanAct==nQtyHDanAct-1 && D > A[nHDanAct+1].x)
		{
			if(!bUseRealEL)
			{
				if(bExtendSlop)
				{
					GetXyMatchSegAndLine(A[nHDanAct], A[nHDanAct+1], CDPoint(D, 0), CDPoint(0, 1), xyResult);
					xy = xyResult;
				}
				else
					xy = A[nHDanAct+1];
			}
			else
			{
				double dTP	= GetThickPave();
				xy.x	= D;
				xy.y	= pLine->GetElevationPlanEl(sta) - dTP;
			}
			break;
		}
		if(GetXyMatchSegAndLine(A[nHDanAct], A[nHDanAct+1], CDPoint(D, 0), CDPoint(0, 1), xyResult))
		{
			xy = xyResult;
			break;
		}
	}

	return xy;	//실제 길이로 리턴
}
//<summary>
// 도로 중심으로부터 임의의 거리만큼 떨어진 곳의 횡단 좌표(xy.x = 도로중심 0 일경우 거리, xy.y = 엘리베이션)
//                             xy.x == 항상 D
//</summary>
//<param name="sta">도로중심 스테이션</param>
//<param name="vAng">도로중심 각도</param>
//<param name="D">사방향 거리(선형에 직각 거리가 아님)</param>
//<param name="bRoadNotSlab">TRUE=지중라멘의 상부 노면(도로면, 철도면)이며 슬래브면(교면)아님
//							FALSE=일반교량, 지중라멘의 슬래브면(교면)</param>
CDPoint CRcBridgeApp::GetXySlabUpperActByDistAngle(double sta, double D, CDPoint vAng, BOOL bRoadNotSlab, BOOL bExp)
{
	if(vAng.y == 1 || D == 0)
		return GetXySlabUpperActByDist(sta, D, bRoadNotSlab, bExp);

	CDPoint xyDis = GetLineBase()->GetXyLineByLengthAndAngle(sta, D, vAng);
	double dist90 = GetLineBase()->GetDisMatchVerticalByXy(xyDis);
	double staSc = GetLineBase()->GetStationByScInLine(sta, dist90, vAng);
	CDPoint xyResult = GetXySlabUpperActByDist(staSc, dist90, bRoadNotSlab, bExp);
	CDPoint xyAngle = CDPoint(D, xyResult.y);	//사방향 거리로 리턴함.
	return xyAngle;	
}
// 좌우 종단면의 시점과 종점에서의 Elevation
double CRcBridgeApp::GetElSlabEndAtBriEnd(BOOL bStt, BOOL bLeft, BOOL bExp)
{
	double dEL	= 0;
	CDPoint A[30];

	double dSta		= GetStationByTarget(bStt ? BRIDGE_OFF_STT_STT : BRIDGE_OFF_END_END, 0, bExp);
	CDPoint vAng	= GetAngleByTarget(bStt ? BRIDGE_OFF_STT_STT : BRIDGE_OFF_END_END,  0);
	
	GetXySlabUpperAct(dSta, vAng, A, FALSE, bExp);
	long nQtyDan	= GetQtyHDanAct(dSta,vAng, FALSE);
	dEL	= bLeft ? A[0].y : A[nQtyDan].y;
	/*
	CLineInfo* pLine = GetLineBase();
	CDPoint A[4];
	double dDist = 0, dSta = 0;
	double dEl = 0;

	if(bStt)
		GetXySss(BRIDGE_OFF_STT_STT, A);
	else
		GetXySss(BRIDGE_OFF_END_END, A);

	CDPoint xy = bLeft ? A[0] : A[3];
	dDist = pLine->GetDisMatchVerticalByXy(xy);
	pLine->GetStationMatchVerticalByXy(xy, dSta);

	dEl = GetXySlabUpperActByDist(dSta, dDist).y;
	*/

	return dEL;
}

//[일반입력>>횡단위치 입력] 권고안
void CRcBridgeApp::SetDataDefaultCrossPosition()
{
	m_nLineBase = 0;
	m_nLineLeft = 0;
	m_nLineRight = 0;

	if(m_pARcBridgeDataStd->IsRailDesign())
	{
		if(m_nSeparBri==0)
		{
			m_dWidthSlabLeft = -5450;
			m_dWidthSlabRight = 5450;
		}
		else if(m_nSeparBri==2)
		{
			m_dWidthSlabLeft = -10910;
			m_dWidthSlabRight = -10;
		}
		else if(m_nSeparBri==1)
		{
			m_dWidthSlabLeft = 10;
			m_dWidthSlabRight = 10910;
		}
	}
	else
	{
		if(m_nSeparBri==0)
		{
			m_dWidthSlabLeft = -9750;
			m_dWidthSlabRight = 9750;
		}
		else if(m_nSeparBri==2)
		{
			m_dWidthSlabLeft = -9750;
			m_dWidthSlabRight = -10;
		}
		else if(m_nSeparBri==1)
		{
			m_dWidthSlabLeft = 10;
			m_dWidthSlabRight = 9750;
		}
	}

}

// 횡단 슬래브 제원 권고안
void CRcBridgeApp::SetDataDefaultHDanSlab()
{
	m_dTS = 800;
	m_dHHS = 400;
	m_dHeightDosangUnderTie = 300;
	m_dSlopeDosang = 1.8;
	m_dHeightRail = 145.6917;
	m_nSlabChemfer[0] = m_nSeparBri==1 ? 0 : 2;
	m_nSlabChemfer[1] = m_nSeparBri==2 ? 0 : 2;
	m_dDH[0] = m_nSeparBri==1 ? 0.0 : 165.0;
	m_dDH[1] = m_nSeparBri==2 ? 0.0 : 165.0;

	double widSlab = GetWidthSlabAct(GetStationBridgeMid(), CDPoint(0, 1), 0);
	double slope      = -0.02;
	long nQtyHDan = 0;
	long nHDan = 0;
	if(m_pARcBridgeDataStd->IsRailDesign())	//상하행 분리, 지중라멘 고려 안함.
	{
		m_bApplyLineSlope = FALSE;
		m_dTP  = 50.0;
		double widGuardFence = 1000;	//연석+보도 기본 너비
		double widDosang = 3400;	//도상 기본 너비
		double widNone = 500;		//없음 기본 너비
		double widTotalFence = widGuardFence*2+widNone*2;
		nQtyHDan  = (long)((widSlab-widTotalFence)/widDosang)+4;
		if(m_bUnderGroundRahmen)
			nQtyHDan = 4;
		SetQtyHDan(nQtyHDan);
		double dSpace = (widSlab-(widTotalFence+widDosang*(nQtyHDan-4)))/2;
		if(widSlab-widTotalFence < 3400)
			dSpace = (widSlab - widGuardFence*2)/2;

		double dLenHDan    = 0;
		long   nTypeGuardFence   = 0;
		for(nHDan=0; nHDan<nQtyHDan; nHDan++)
		{
			if(nHDan==0 || nHDan==nQtyHDan-1)
			{
				dLenHDan    = widGuardFence;
				nTypeGuardFence   = HDAN_TYPE_RAIL_FOOTWAYBLOCK;
			}
			else if(nHDan==1 || nHDan==nQtyHDan-2)
			{
				dLenHDan    = dSpace;
				nTypeGuardFence   = 0;
			}
			else
			{
				dLenHDan    = widDosang;
				nTypeGuardFence   = HDAN_TYPE_RAIL_DOSANGGRAVEL;
			}
			SetLengthHDan(nHDan, dLenHDan);
			SetSlopeHDan(nHDan, slope);
			SetTypeHDan(nHDan, nTypeGuardFence);
		}
	}
	else
	{
		m_bApplyLineSlope = TRUE;
		m_dTP  = 80.0;
		double widGuardFence = 450.0;
		double widNone = 3500.0;
		
		

		if(m_nSeparBri==0)	//상하행 분리가 아닌경우
		{
			double widFootwayBlock = 180.0;
			double widCenterGuardFull = 610.0;
			double widNoneTotal = widSlab - widGuardFence*2;
			double widFootway = 2000;
			if(m_bUnderGroundRahmen)
			{
				nQtyHDan = 3;
				widNone = widNoneTotal;
			}
			else
			{
				
				if(widNoneTotal<=widNone*2)
				{
					nQtyHDan = 4;
					widNone = widNoneTotal/2;
				}
				else if(widNoneTotal>widNone*2 && widNoneTotal<widNone*4+widFootwayBlock*2)
				{
					nQtyHDan = 5;
					widNone = (widNoneTotal-widCenterGuardFull)/2;
				}
				else
				{
					nQtyHDan = 9;
					widNone = (widNoneTotal-widCenterGuardFull-widFootway*2-widFootwayBlock*2)/2;
				}
			}
			SetQtyHDan(nQtyHDan);
			for(nHDan=0; nHDan<nQtyHDan; nHDan++)
			{
				m_bLevelHDan[nHDan]			= FALSE;
				m_dSlopeHDan[nHDan]			= slope;
				if(nHDan==0 || nHDan==nQtyHDan-1)
				{
					m_dLengthHDan[nHDan]		= widGuardFence;
					m_nTypeHDan[nHDan]			= HDAN_TYPE_ROAD_GUARDFENCE;
				}
				else if(nQtyHDan==3 && nHDan==1)
				{
					m_dLengthHDan[nHDan]		= widNone;
					m_nTypeHDan[nHDan]			= HDAN_TYPE_ROAD_NONE;
				}
				else if(nQtyHDan==4 && (nHDan==1 || nHDan==2))
				{
					m_dLengthHDan[nHDan]		= widNone;
					m_nTypeHDan[nHDan]			= HDAN_TYPE_ROAD_NONE;
				}
				else if(nQtyHDan==5 && (nHDan==1 || nHDan==3))
				{
					m_dLengthHDan[nHDan]		= widNone;
					m_nTypeHDan[nHDan]			= HDAN_TYPE_ROAD_NONE;
				}
				else if(nQtyHDan==5 && nHDan==2)
				{
					m_dLengthHDan[nHDan]		= widCenterGuardFull;
					m_nTypeHDan[nHDan]			= HDAN_TYPE_ROAD_CENTERGUARD;
				}
				else if(nQtyHDan==9 && (nHDan==1 || nHDan==7))
				{
					m_dLengthHDan[nHDan]		= widFootway;
					m_nTypeHDan[nHDan]			= HDAN_TYPE_ROAD_FOOTWAY;
				}
				else if(nQtyHDan==9 && (nHDan==2 || nHDan==6))
				{
					m_dLengthHDan[nHDan]		= widFootwayBlock;
					m_nTypeHDan[nHDan]			= HDAN_TYPE_ROAD_FOOTWAYBLOCK;
				}
				else if(nQtyHDan==9 && (nHDan==3 || nHDan==5))
				{
					m_dLengthHDan[nHDan]		= widNone;
					m_nTypeHDan[nHDan]			= HDAN_TYPE_ROAD_NONE;
				}
				else if(nQtyHDan==9 && nHDan==4)
				{
					m_dLengthHDan[nHDan]		= widCenterGuardFull;
					m_nTypeHDan[nHDan]			= HDAN_TYPE_ROAD_CENTERGUARD;
				}
			}
		}
		else	//상하행 분리인 경우
		{
			double widCenterGuardHalf = 320.0;
			double widNoneTotal = widSlab - widGuardFence - widCenterGuardHalf;
			if(m_bUnderGroundRahmen)
			{
				nQtyHDan = 2;
				widNoneTotal = widSlab - widGuardFence;
				widNone = widNoneTotal;
			}
			else
			{
				if(widNoneTotal<=widNone*2)
				{
					nQtyHDan = 3;
					widNone = widNoneTotal;
				}
				else
				{
					nQtyHDan = 5;
					widNone = widNoneTotal/(nQtyHDan-2);
				}
			}
			SetQtyHDan(nQtyHDan);
			if(m_nSeparBri==2)	//하행선
			{
				for(nHDan=0; nHDan<nQtyHDan; nHDan++)
				{
					m_bLevelHDan[nHDan]			= FALSE;
					m_dSlopeHDan[nHDan]			= slope;
					if(nHDan==0)
					{
						m_dLengthHDan[nHDan]		= widGuardFence;
						m_nTypeHDan[nHDan]			= HDAN_TYPE_ROAD_GUARDFENCE;
					}
					else if(nQtyHDan!=2 && nHDan==nQtyHDan-1)
					{
						m_dLengthHDan[nHDan]		= widCenterGuardHalf;
						m_nTypeHDan[nHDan]			= HDAN_TYPE_ROAD_CENTERGUARDHALF;
					}
					else
					{
						m_dLengthHDan[nHDan]		= widNone;
						m_nTypeHDan[nHDan]			= HDAN_TYPE_ROAD_NONE;
					}
				}
			}
			else if(m_nSeparBri==1)	//상행선
			{
				for(nHDan=0; nHDan<nQtyHDan; nHDan++)
				{
					m_bLevelHDan[nHDan]			= FALSE;
					m_dSlopeHDan[nHDan]			= slope;
					if(nQtyHDan!=2 && nHDan==0)
					{
						m_dLengthHDan[nHDan]		= widCenterGuardHalf;
						m_nTypeHDan[nHDan]			= HDAN_TYPE_ROAD_CENTERGUARDHALF;
					}
					else if(nHDan==nQtyHDan-1)
					{
						m_dLengthHDan[nHDan]		= widGuardFence;
						m_nTypeHDan[nHDan]			= HDAN_TYPE_ROAD_GUARDFENCE;
					}
					else
					{
						m_dLengthHDan[nHDan]		= widNone;
						m_nTypeHDan[nHDan]			= HDAN_TYPE_ROAD_NONE;
					}
				}
			}
		}
	}
	// 방호벽
	MakeGuardWall(nQtyHDan);
	
	for(nHDan=0; nHDan<nQtyHDan; nHDan++)
	{
		CGuardWallRC* pGW = NULL;
		pGW = GetGuardWallByHDan(nHDan);
		if(!pGW) continue;
		pGW->m_nTypeGuardWall = m_nTypeHDan[nHDan];
		pGW->m_nNumberHDan = nHDan;
		pGW->m_bRailRoad = m_pARcBridgeDataStd->IsRailDesign();
		pGW->m_bLeftGuardFence = IsLeftHDan(nHDan);
		pGW->m_dWidhtHDan = GetLengthHDan(nHDan);

		pGW->SetDataDefaultGuardWall(GetThickPave(FALSE), m_bUnderGroundRahmen);
	}
}
//지중라멘일 경우 도로횡단, 철도횡단 상면 횡단 권고안
void CRcBridgeApp::SetDataDefaultHDanRoad()
{
//	CLineInfo* pLine = m_pARcBridgeDataStd->GetLineApp();

	CDPoint vAng(0, 1);
	double sta = GetStationBridgeMid();
	double widSlab = GetWidthSlabAct(sta, vAng, 0,FALSE,FALSE,TRUE);
	long nQtyHDan = 0;
	if(m_pARcBridgeDataStd->IsRailDesign())
	{
		double widGuardFence = 1000;	//연석+보도 기본 너비
		double widDosang = 3400;	//도상 기본 너비
		double widNone = 500;		//없음 기본 너비
		double widTotalFence = widGuardFence*2+widNone*2;
		nQtyHDan  = (long)((widSlab-widTotalFence)/widDosang)+4;
		SetQtyHDanRoad(nQtyHDan);
		double dSpace = (widSlab-(widTotalFence+widDosang*(nQtyHDan-4)))/2;
		if(widSlab-widTotalFence < 3400)
			dSpace = (widSlab - widGuardFence*2)/2;

		double dLenHDan    = 0;
		double slope      = -0.02;
		long   nTypeGuardFence   = 0;
		long nHDan=0; for(nHDan=0; nHDan<nQtyHDan; nHDan++)
		{
			if(nHDan==0 || nHDan==nQtyHDan-1)
			{
				dLenHDan    = widGuardFence;
				nTypeGuardFence   = HDAN_TYPE_RAIL_FOOTWAYBLOCK;
			}
			else if(nHDan==1 || nHDan==nQtyHDan-2)
			{
				dLenHDan    = dSpace;
				nTypeGuardFence   = 0;
			}
			else
			{
				dLenHDan    = widDosang;
				nTypeGuardFence   = HDAN_TYPE_RAIL_DOSANGGRAVEL;
			}
			SetLengthHDanRoad(nHDan, dLenHDan);
			SetSlopeHDanRoad(nHDan, slope);
			SetTypeHDanRoad(nHDan, nTypeGuardFence);
		}
	}
	else
	{
//		double widDike	 = 250;
//		double widDikeRoad = 500;
		
		

		if(m_nSeparBri!=0)	//분리형 교량이면
		{
			double widCenterGuardHalf = 320.0;
			double widLaneAct = (widSlab - widCenterGuardHalf);
			nQtyHDan = 2;
			SetQtyHDanRoad(nQtyHDan);
			m_dLengthHDanRoad[0] = m_nSeparBri==1 ? widLaneAct : widCenterGuardHalf;
			m_dLengthHDanRoad[1] = m_nSeparBri==1 ? widCenterGuardHalf : widLaneAct;
			m_nTypeHDanRoad[0] = m_nSeparBri==1 ? HDAN_TYPE_ROAD_NONE : HDAN_TYPE_ROAD_CENTERGUARDHALF;
			m_nTypeHDanRoad[1] = m_nSeparBri==1 ? HDAN_TYPE_ROAD_CENTERGUARDHALF : HDAN_TYPE_ROAD_NONE;
		}
		else
		{
			double widCenterGuardFull = 610.0;
			double widLaneAct = (widSlab - widCenterGuardFull)/2;
			nQtyHDan = 3;
			SetQtyHDanRoad(nQtyHDan);

			double dSlopeSide	 = 0.04;
			long nHDan=0; for(nHDan=0; nHDan<nQtyHDan; nHDan++)
			{
				
				if(nHDan==0 || nHDan==(nQtyHDan-1))
				{
					m_dLengthHDanRoad[nHDan] = widLaneAct;
					m_bLevelHDanRoad[nHDan] = FALSE;
					m_dSlopeHDanRoad[nHDan]  = dSlopeSide;
					m_nTypeHDanRoad[nHDan]   = HDAN_TYPE_ROAD_NONE;
				}
				if(nHDan==1)
				{
					m_dLengthHDanRoad[nHDan] = widCenterGuardFull;
					m_bLevelHDanRoad[nHDan] = TRUE;
					m_dSlopeHDanRoad[nHDan]  = 0;
					m_nTypeHDanRoad[nHDan]   = HDAN_TYPE_ROAD_CENTERGUARD;
				}
			}		
		}
	}
	
	// 방호벽
	MakeGuardWall(nQtyHDan,TRUE);
	
	long nHDan=0; for(nHDan=0; nHDan<nQtyHDan; nHDan++)
	{
		CGuardWallRC* pGW = NULL;
		pGW = GetGuardWallByHDan(nHDan, TRUE);
		if(!pGW) continue;
		pGW->m_nTypeGuardWall = m_nTypeHDanRoad[nHDan];
		pGW->m_nNumberHDan = nHDan;
		pGW->m_bRailRoad = m_pARcBridgeDataStd->IsRailDesign();
		pGW->m_bLeftGuardFence = IsLeftHDan(nHDan,TRUE);
		pGW->m_dWidhtHDan = GetLengthHDan(nHDan, TRUE);

		pGW->SetDataDefaultGuardWall(GetThickPave(TRUE), TRUE);
	}
}

// 성토사면
void CRcBridgeApp::SetDataDefaultHDanNori()
{
	m_dUnderGroundNoriSlopeFirst	= 1.5;
	m_dUnderGroundNoriSlopeNext		= 1.8;
	m_dUnderGroundNoriMaxHeight		= 5000;
	m_dUnderGroundNoriWidth			= 1000;
	m_dUnderGroundNoriSolpeSodan	= 4.0;

}
// 포장제원
void CRcBridgeApp::SetDataDefaultHDanPave()
{
	
	m_dUnderGroundDesignLiveAlways	=  0;
	if(m_bUnderGroundRahmen)
	{
		m_nTypePavement					=  0;
		m_dUnderGroundHeightPavement	=  80;
	}
	else
	{
		m_nRoadMaterial = 0;
		m_dTP = 80;
		// m_dUnderGroundPaveHeightTosa_XXX	= 0;
		m_dUnderGroundPaveWeightTosa	= 2300;
	}

	// (ARCBRIDGE-2223) 아스콘 단위중량은 무조건 2300 주는걸로.  // m_pARcBridgeDataStd->IsSI() ? 2250 : 2300;
	m_dUnderGroundHeightMortar		= 50;
	m_pARcBridgeDataStd->m_pBasicConcInfo->m_UWeightAsphalt = 2300;
	m_pARcBridgeDataStd->m_pBasicConcInfo->m_UWeightCement	= 2150;
	m_bUnderGroundPaveDetail		= FALSE; 
	if(1/*m_bUnderGroundPaveDetail*/)		// 안쓰더라도 초기화은 해주는게...
	{
		m_dUnderGroundPaveHeightPyo		= 450;
		m_dUnderGroundPaveHeightGee		= 300;
		m_dUnderGroundPaveHeightMiddle	= 100;
		m_dUnderGroundPaveHeightBozo	= 300;
		m_dUnderGroundPaveHeightDong	= 150;
		m_dUnderGroundPaveHeightEps		= 1000;
		//double dHeighEct = m_dUnderGroundPaveHeightPyo + m_dUnderGroundPaveHeightGee + m_dUnderGroundPaveHeightMiddle
		//	+ m_dUnderGroundPaveHeightBozo + m_dUnderGroundPaveHeightDong + m_dUnderGroundPaveHeightEps;
		// m_dUnderGroundPaveHeightTosa_XXX	= MAX(m_dUnderGroundDesignHeight - dHeighEct, 0);

		m_dUnderGroundPaveWeightPyo		= 2300;
		m_dUnderGroundPaveWeightGee		= 2300;
		m_dUnderGroundPaveWeightMiddle	= 2000;
		m_dUnderGroundPaveWeightBozo	= 1900;
		m_dUnderGroundPaveWeightDong	= 1900;
		m_dUnderGroundPaveWeightEps		=  200;
		m_dUnderGroundPaveWeightTosa	= 2300;

		double dStationBridge    = GetStationOnJijum(0, 0);
		double dWidthBetweenCurb = toM(GetWidthTotalRoadWay(dStationBridge));
		m_dUnderGroundPaveWidthPyo		= dWidthBetweenCurb;
		m_dUnderGroundPaveWidthGee		= dWidthBetweenCurb;
		m_dUnderGroundPaveWidthMiddle	= dWidthBetweenCurb;
		m_dUnderGroundPaveWidthBozo		= dWidthBetweenCurb;
		m_dUnderGroundPaveWidthDong		= dWidthBetweenCurb;
		m_dUnderGroundPaveWidthEps		= dWidthBetweenCurb;
		m_dUnderGroundPaveWidthTosa		= toM(GetWidthSlabAct(dStationBridge, CDPoint(0, 1)));
	}
}
// 상재하중
void CRcBridgeApp::SetDataDefaultHDanLive()
{
	CLineInfoApp *pLinCen = m_pARcBridgeDataStd->m_pLineApp;
	CLineInfoApp *pLinBri = m_pLineApp;

	double dHeightStt   = toM(pLinCen->GetElevationPlanEl(m_dStationBridgeStt) - pLinBri->GetElevationPlanEl(m_dStationBridgeStt));
	double dHeightEnd   = toM(pLinCen->GetElevationPlanEl(m_dStationBridgeStt+m_dLengthBridge) - pLinBri->GetElevationPlanEl(m_dStationBridgeStt+m_dLengthBridge));
	double dHeightApply = Round(min(dHeightStt,dHeightEnd),1);

	m_dUnderGroundDesignHeight		=  frM(dHeightApply);
	//////////////////////////////////////////////////////////////////////////
}

void CRcBridgeApp::SetDataDefaultHDanRailLive()
{
	m_dRail_HeightTie				=  195;	// 침목높이
	m_dRail_LengthTie				= 2400;	// 침목길이
	m_dRail_WidthTie				=  300;	// 침목폭
	m_dRail_DistBetweenWheel		= frM(m_pARcBridgeDataStd->m_RailDistWheel);	// 동륜축 거리
	BOOL bSingleLane = (m_pARcBridgeDataStd->m_nTypeRailQty==0) ? TRUE : FALSE;

// (ARCBRIDGE-3294) 궤도 중심 간격은 레일위치 입력에서 간격을 계산 했기 때문에 기본값 하지 않는다.
//	m_dRail_DistBetweenRail			= bSingleLane ? 0 : 4100;	// 궤도 중심간 거리
	if(bSingleLane)
		m_dRail_DistBetweenRail = 0;
}

// 레일위치
void CRcBridgeApp::SetDataDefaultHDanRail()
{
	CDPoint vAng(0, 1);
	double dSta = GetStationBridgeMid();
	long nQtyDosang = 0;
	double lenHDan = 0;
	double disSlabLeft = GetDisSlabLeftAct(dSta, vAng,m_bUnderGroundRahmen);	//지중라멘교일 경우에는 슬래브면이 아닌 철로면
	long nQtyHDan = GetQtyHDan(m_bUnderGroundRahmen);	//지중라멘교일 경우에는 슬래브면이 아닌 철로면
	for(long nHDan = 0; nHDan < nQtyHDan; nHDan++)
	{
		CGuardWallRC *pGW = GetGuardWallByHDan(nHDan, m_bUnderGroundRahmen);
		if(!pGW) continue;
		if(pGW->IsTypeHDanRailTotalDosang())	//도상이면
		{
			SetRailLineNumber(nQtyDosang, m_nLineBase);
			SetLengthRailByLine(nQtyDosang, (disSlabLeft + lenHDan + GetLengthHDanAct(dSta, vAng, nHDan)/2));
			nQtyDosang++;
		}
		lenHDan += GetLengthHDanAct(dSta, vAng, nHDan,m_bUnderGroundRahmen);

	}
	SetQtyRailNode(nQtyDosang);	
}
// 상재하중
void CRcBridgeApp::SetDataDefaultHDanDistribute()
{
	m_dRail_LengthTie			= 2400;	// 침목길이
	m_dRail_HeightTie			= 195;	// 침목높이
	m_dUnderGroundHeightMortar	= 50;	// 방수몰탈
}
//지중라멘교 멤버 권고안: 슬래브상단EL, 좌측 노면 거리, 우측 노면 거리
void CRcBridgeApp::SetDataDefaultHDanUnderMember()
{
	if(!m_bUnderGroundRahmen) return;
	CLineInfoApp *pLineStd = m_pARcBridgeDataStd->m_pLineApp;
	double dStaCen = GetStationBridgeMid();
	double dELCen  = pLineStd->GetElevationPlanEl(dStaCen);
	m_dUnderGroundCenterELStt	 = toM(dELCen) - 2.000;
	m_dUnderGroundCenterELEnd	 = m_dUnderGroundCenterELStt;
	if(m_dWidthSlabLeft>=0)
	{
		m_dWidthRoadLeft  = m_dWidthSlabLeft;
		m_dWidthRoadRight = m_dWidthSlabRight - 4000;
	}
	else if(m_dWidthSlabRight<=0)
	{
		m_dWidthRoadLeft  = m_dWidthSlabLeft + 4000;
		m_dWidthRoadRight = m_dWidthSlabRight;
	}
	else
	{
		m_dWidthRoadLeft  = MIN(m_dWidthSlabLeft  + 4000,0);
		m_dWidthRoadRight = MAX(m_dWidthSlabRight - 4000,0);
	}
}

// 교량에 대한 기초의 위치를 지정.
// CDPoint xyOrgStt, CDPoint xyOrgEnd : 각 벽체의 하단 중심
void CRcBridgeApp::SetFootingStaAndEL(CDPoint xyOrgStt, CDPoint xyOrgEnd, BOOL bApplyAngle, long nLeft)
{
	// bApplyAngle에 대해서 여기서 해줄 일은 없음..
	// 기본 으로 할일만 하면 됨.

	// 시점측 기초
	m_footingStt.m_nType = m_nTypeBridge ==  BRIDGE_TYPE_TRAP2 || m_nTypeBridge ==  BRIDGE_TYPE_ARCH ? FOOTING_TYPE_BASIC_RIGHTSLOPE : FOOTING_TYPE_BASIC;
	m_footingStt.m_dML = m_dWS;
	m_footingStt.m_dMR = m_nTypeBridge == BRIDGE_TYPE_ARCH ? m_dWSHW : 0;
	m_footingStt.m_dSta = xyOrgStt.x;
	m_footingStt.m_dEL = xyOrgStt.y;
	
	// 박스교의 경우 시점기초가 종점기초 역활까지 한다.
	if(m_nTypeBridge == BRIDGE_TYPE_BOX)
	{
		//시작과 끝점은 부상방지판까지 포함해서 계산.
		CDPoint xyStt = GetXySectionSttEndIncludePRF(TRUE);
		CDPoint xyEnd = GetXySectionSttEndIncludePRF(FALSE);
		//CDPoint xyMid = (xyStt + xyEnd) / 2;

		double dLenBridgeVert = GetLengthBridge(TRUE);
		m_footingBox.m_dSta = xyOrgStt.x;
		m_footingBox.m_dEL = xyOrgStt.y;
		m_footingBox.m_nType = FOOTING_TYPE_BOX;
		m_footingBox.m_dML = dLenBridgeVert / 2 + m_dWS;		// (xyEnd.x - xyStt.x) / 2;		// (ARCBRIDGE-3549) 시점 사각이 있는경우 기초의 폭을 적용할때는 선형방향 교량길이가 아닌 직각방향 교량길이여야 한다.
		m_footingBox.m_dMR = dLenBridgeVert / 2 + m_dWE;		// m_footingBox.m_dML;
		m_footingBox.m_dFWL = 0;
		m_footingBox.m_dFWR = 0;
		m_footingBox.m_xyBottomCenter.x = m_dStationBridgeStt + dLenBridgeVert/2;// xyMid.x;
		m_footingBox.m_xyBottomCenter.y = min(xyStt.y, xyEnd.y);
	}

	// 종점측 기초
	m_footingEnd.m_nType = m_nTypeBridge ==  BRIDGE_TYPE_TRAP2 || m_nTypeBridge ==  BRIDGE_TYPE_ARCH ? FOOTING_TYPE_BASIC_LEFTSLOPE : FOOTING_TYPE_BASIC;
	m_footingEnd.m_dML = m_nTypeBridge == BRIDGE_TYPE_ARCH ? m_dWEHW : 0;
	m_footingEnd.m_dMR = m_dWE;
	m_footingEnd.m_dSta = xyOrgEnd.x;
	m_footingEnd.m_dEL = xyOrgEnd.y;
}

// 교량에 있는 기초에 대한 권고안.
BOOL CRcBridgeApp::SetDataDefaultShapeFooting(CFootingApp *pFooting, double dWidthWall, double dSta, long nIdxInWall)
{
	if(!pFooting->m_bIs) return TRUE;	// 기초가 없는 경우라면 기초값 초기화 하고 리턴.
	CLineInfoApp *pLine = m_pLineApp;
	if(!pLine) return FALSE;

	// 교량 높이 구하기.
	double dW			= dWidthWall;
	double dH			= 0;
	double dElEarth		= 0;
	if(pLine->GetElevationErathOnStation(dSta, dElEarth))
		dH		= GetXyMatchLineAndSlabUpper(CDPoint(dSta, 0), CDPoint(0, 1), 0, 0).y - (dElEarth - dDepthFromEL);
	else
		dH		= 5000;
	if(dH > dMaxHeight) dH	= dMaxHeight;
	if(dH < dMinHeight)	dH	= dMinHeight; // 권고안시 교량의 최소 높이 적용


	// * 기초 저면의 폭(B)(0.6H <= B <= 0.9H) (RcBridge에서는 1번 적용)
	// 1. 고정단 : 3.5 + 0.8 * (H - 5.0)
	// 2. 가동단 : 3.5 + 0.7 * (H - 5.0)
	double dB		= 3.5 + 0.8 * (dH - 5.0);
	if(dB < 0.6 * dH) dB	= 0.6 * dH;
	if(dB > 0.9 * dH) dB	= 0.9 * dH;
	dB	= Round(dB, -1);

	// * 기초 앞굽의 길이 (H : 전체 높이, B : 기초 너비, tW : 벽체 너비) - RcBridge에서는 1번 적용
	// 1. 비탈면에 교대가 높은경우 : H/6
	// 2. 평지또는 교대 전면에 제약을 받지 않거나, 말뚝기초로 하는 경우 : B//3

	// * 기초 뒷굽의 길이
	// 1. B - (앞굽 + tW)

	// * 기초의 두께(최소 0.8)
	// 1. (B/5 - 0.1) >= 0.8
	// 2. (B < 5) 일때는 0.8
	// 내측벽체용일때.
	if(nIdxInWall > -1)
	{
		CWallApp *pWall	= GetInWall(nIdxInWall);
		if(pWall)
		{
			if(pWall->m_nType == WALL_TYPE_SLOPE)
			{
				if(pWall->IsLeftSlope())
				{
					pFooting->m_nType = FOOTING_TYPE_RIGHTSLOPE;
					pFooting->m_dFW		= dW * 1.5;
					pFooting->m_dFH		= dW;
					pFooting->m_dFSLW	= dW * 1.5;
					pFooting->m_dFSRH	= dW;
				}
				else
				{
					pFooting->m_nType = FOOTING_TYPE_LEFTSLOPE;
					pFooting->m_dFW		= dW * 1.5;
					pFooting->m_dFH		= dW;
					pFooting->m_dFSRW	= dW * 1.5;
					pFooting->m_dFSLH	= dW;
				}
				pFooting->m_dFWR	= 500;
				pFooting->m_dFWL	= 500;
			}
			else
			{
				pFooting->m_nType = FOOTING_TYPE_BASIC_BOTH;
				pFooting->m_dFWR	= Round((dB - dW)/2, -2);
				pFooting->m_dFWL	= Round((dB - dW)/2, -2);
				pFooting->m_dFH		= (dB < 5000) ? 800 : Round(dB/5-100, -2);
			}
		}
	}
	// 외측벽체용일때
	else 
	{
		if(Compare(dSta, m_dStationBridgeStt, "="))
		{
			if(m_nTypeBridge == BRIDGE_TYPE_TRAP2 || m_nTypeBridge == BRIDGE_TYPE_ARCH)
			{
				pFooting->m_dFWL	= Round(dH / 6, -2);
				double dHW			= dB - pFooting->m_dFWL - dW;
				pFooting->m_dFWR	= Round(dHW * 0.65, -2);
				pFooting->m_dFERW	= 500;
				pFooting->m_dFSRW	= dHW - pFooting->m_dFWR - pFooting->m_dFERW;
				double dHF			= (dB < 5000) ? 800 : Round(dB/5-100, -2);
				pFooting->m_dFH		= Round(dHF * 0.6, -2);
				pFooting->m_dFSRH	= dHF	- pFooting->m_dFH;
			}
			else
			{
				pFooting->m_dFWR	= Round(dH / 6, -2);
				pFooting->m_dFWL	= Round(dB - (pFooting->m_dFWR + dW), -2);
				pFooting->m_dFH		= (dB < 5000) ? 800 : Round(dB/5-100, -2);
			}
		}
		else
		{
			if(m_nTypeBridge == BRIDGE_TYPE_TRAP2 || m_nTypeBridge == BRIDGE_TYPE_ARCH)
			{
				pFooting->m_dFWR	= Round(dH / 6, -2);
				double dHW			= dB - pFooting->m_dFWR - dW;
				pFooting->m_dFWL	= Round(dHW * 0.65, -2);
				pFooting->m_dFELW	= 500;
				pFooting->m_dFSLW	= dHW - pFooting->m_dFWL - pFooting->m_dFELW;
				double dHF			= (dB < 5000) ? 800 : Round(dB/5-100, -2);
				pFooting->m_dFH		= Round(dHF * 0.6, -2);
				pFooting->m_dFSLH	= dHF	- pFooting->m_dFH;
			}
			else
			{
				pFooting->m_dFWL	= Round(dH / 6, -2);
				pFooting->m_dFWR	= Round(dB - (pFooting->m_dFWL + dW), -2);
				pFooting->m_dFH		= (dB < 5000) ? 800 : Round(dB/5-100, -2);
			}
		}
	}
	
	if(pFooting->m_nType == FOOTING_TYPE_BASIC || pFooting->m_nType == FOOTING_TYPE_BASIC_BOTH)
	{
		pFooting->m_dFSLW	= 0;
		pFooting->m_dFSLH	= 0;
		pFooting->m_dFSRH	= 0;
		pFooting->m_dFSRW	= 0;
		pFooting->m_dFELW	= 0;
		pFooting->m_dFERW	= 0;
	}
	

	// 기초 권고안을 하면 확대기초 권고안도 해야 된다.
	pFooting->SetDataDefaultShapeExFooting();

	// 지반정보 권고안
	long nBoringIndex = GetBoringIndexByStation(pFooting->m_dSta);
	pFooting->SetBoringData(0, nBoringIndex);

	return TRUE;
}

// 시점측과 종점측 벽체에 평면상의 지점 path를 설정해 준다.
// 기초와 벽체가 뻗어나가는 방향임.
void CRcBridgeApp::SetTvPathPlan()
{
	CTwinVector tv = GetTvJijum(0, 0.0);
	GetSwap(tv.m_v1, tv.m_v2);
	m_footingStt.m_tvPathPlan = tv;

	tv = GetTvJijum(m_nQtyJigan, 0.0);
	GetSwap(tv.m_v1, tv.m_v2);
	m_footingEnd.m_tvPathPlan = tv;
}

// 각 지점의 끝부분에서 선형에 대한 접선의 각도
CDPoint CRcBridgeApp::GetAngleAzimuthTanOnJijum(long nJ, int nLeft)
{
	CLineInfo* pLine = GetLineBySlab(nLeft);

	CDPoint vAng(0, 0);
	if(pLine)
	{
		double dSta = GetStationOnJijum(nJ, nLeft);
		vAng = pLine->GetAngleAzimuth(dSta);
	}

	return vAng;
}

// 교량 각 지점의 방위각(사각 고려)
// BOOL bOnlyDirBySkew : 무조건 skew방향의 각도를 리턴함.
CDPoint CRcBridgeApp::GetAngleAzimuthJijum(int nJ, BOOL bOnlyDirBySkew)
{
	CDPoint vAng(0, 0);
	CDPoint A[4];
	CTwinVector tv;
	int nCountJijum = GetCountJijum();

	if(nJ==0)
		GetXySss(BRIDGE_OFF_STT_STT, A, FALSE);
	else if(nJ==nCountJijum-1)
		GetXySss(BRIDGE_OFF_END_END, A, FALSE);
	else
		tv = GetTvJijum(nJ, 0.0);

	if(nJ==0 || nJ==nCountJijum-1)
	{
		if(bOnlyDirBySkew)
			vAng = (A[1]-A[2]).Unit();
		else
			vAng = (A[2]-A[1]).Unit();
	}
	else
		vAng = (tv.m_v2-tv.m_v1).Unit();

	return vAng;
}

// 교량과 그에 딸려 있는 벽체나 기초와 겹치는 선에 대해서 체크함.
// 반드시 좌표를 모두 구한뒤에 해야 되므로, 다른데서는 함부로 호출하면 별 효과 없음.
void CRcBridgeApp::CheckOverLapLineBetweenSubObject()
{
	// 박스형이 아닐때 겹치는 선에 대해서 정의해줌
	if(m_nTypeBridge != BRIDGE_TYPE_BOX)
	{
		CTwinVector tv;
		long stt = 0; for(stt = 0; stt < 2; stt++)
		{
			BOOL bStt	= stt == iSTT;
			CFootingApp *pFooting = bStt ? &m_footingStt : &m_footingEnd;
			CString str = bStt ? "시점벽체하면" : "종점벽체하면";
			if(!pFooting->m_bIs) continue;
			if(m_tvArrVertSection.GetTvByInfo(str, tv))
			{
				CTwinVector tmpL, tmpR;
				pFooting->m_tvArrFront.GetTvByInfo("기초상면좌측마진", tmpL);
				tv.m_vecOverlap1 = tmpL.GetXyLeft();
				pFooting->m_tvArrFront.GetTvByInfo("기초상면우측마진", tmpR);
				tv.m_vecOverlap2 = tmpR.GetXyRight();
				tmpL.m_vecOverlap1 = tmpL.m_v1;
				tmpL.m_vecOverlap2 = tmpL.m_v2;
				tmpR.m_vecOverlap1 = tmpR.m_v1;
				tmpR.m_vecOverlap2 = tmpR.m_v2;

				m_tvArrVertSection.SetTvByInfo(str, tv);
				pFooting->m_tvArrFront.SetTvByInfo("기초상면좌측마진", tmpL);
				pFooting->m_tvArrFront.SetTvByInfo("기초상면우측마진", tmpR);
			}

			// 경사교2는 내측벽체와 겹치는 부분도 있을 수 있다.
			if(m_nTypeBridge == BRIDGE_TYPE_TRAP2)
			{
				long nCountInWall	= GetCountInWall();
				str	= bStt ? "기초우측경사" : "기초좌측경사";
				long nIdxInWall	= bStt ? (nCountInWall > 0 ? 0 : -1) : (nCountInWall > 1 ? nCountInWall-1 : -1);
				CWallApp *pWall	= nIdxInWall > -1 ? GetInWall(nIdxInWall) : NULL;
				if(pWall)
				{
					CTwinVector tvSlope	= pFooting->m_tvArrFront.GetTvByInfo(str);
					CTwinVector tvLowerL	= pWall->m_tvArrFront.GetTvByInfo("벽체하면좌측");
					CTwinVector tvLowerR	= pWall->m_tvArrFront.GetTvByInfo("벽체하면우측");
					tvLowerL.m_vecOverlap1	= tvLowerL.m_v1;
					tvLowerL.m_vecOverlap2	= tvSlope.m_v1;
					tvLowerR.m_vecOverlap1	= tvLowerR.m_v1;
					tvLowerR.m_vecOverlap2	= tvSlope.m_v2;
					tvSlope.m_vecOverlap1	= tvLowerL.m_v2;
					tvSlope.m_vecOverlap2	= tvLowerR.m_v2;

					pFooting->m_tvArrFront.SetTvByInfo(str, tvSlope);
					pWall->m_tvArrFront.SetTvByInfo("벽체하면좌측", tvLowerL);
					pWall->m_tvArrFront.SetTvByInfo("벽체하면우측", tvLowerR);
				}

				// 외측벽체 헌치와 겹치는 부분도 overlap 체크
				str	= bStt ? "기초상면우측" : "기초상면좌측";
				CString str2	= bStt ? "시점벽체하면" : "종점벽체하면";
				CTwinVector tvUpper	= pFooting->m_tvArrFront.GetTvByInfo(str);
				CTwinVector tvLower	= m_tvArrVertSection.GetTvByInfo(str2);

				tvUpper.m_vecOverlap1	= tvLower.GetXyLeft();
				tvUpper.m_vecOverlap2	= tvLower.GetXyRight();
				tvLower.m_vecOverlap1	= tvLower.m_v1;
				tvLower.m_vecOverlap2	= tvLower.m_v2;

				pFooting->m_tvArrFront.SetTvByInfo(str, tvUpper);
				m_tvArrVertSection.SetTvByInfo(str2, tvLower);
			}
		}
	}
	else
	{
		CTwinVector tv;
		long stt = 0; for(stt = 0; stt < 2; stt++)
		{
			BOOL bStt	= stt == iSTT;
			CString str = bStt ? "시점부상방지저판안쪽측면" : "종점부상방지저판안쪽측면";

			if(m_tvArrVertSection.GetTvByInfo(str, tv))
			{
				CString strF = bStt? "시점벽체(부상방지저판)" : "종점벽체(부상방지저판)";
				CTwinVector tmp;
				m_tvArrVertSection.GetTvByInfo(strF, tmp);
				tv.m_vecOverlap1 = tmp.m_v1;
				tv.m_vecOverlap2 = tmp.m_v2;
				tmp.m_vecOverlap1 = tmp.m_v1;
				tmp.m_vecOverlap2 = tmp.m_v2;

				m_tvArrVertSection.SetTvByInfo(str, tv);
				m_tvArrVertSection.SetTvByInfo(strF, tmp);
			}
		}
	}

	// 벽체들의 좌표도 구해주고 겹치는 선에 대해서도 정의해 줌
	long i = 0; for(i = 0; i < GetCountInWall(); i++)
	{
		CString str = _T("");
		
		CTwinVector tv;

		CWallApp *pWall = GetInWall(i);

		// 기둥식일때는 겹치는선 정의 안해도 됨. ㅋㅋ
		if(pWall && !pWall->m_bTopIsColumn)
		{
// 			long nIdxHunch = 0;
			for(long w = 0; w < 2; w++)
			{
				if(w == 1 && pWall->m_nType != WALL_TYPE_V) break;
				
				long nIdxHunch = GetIdxHunchInWall(i, (w==0));
				str.Format("%d", nIdxHunch+1);

				
				// 좌측 우측 한번씩 설정해줌.
				CTwinVector tmp;
				CString sWall = _T("벽체상면");
				if(pWall->m_nType == WALL_TYPE_V)
					sWall = w == 0 ? "벽체좌측팔상면" : "벽체우측팔상면";

				if(pWall->m_tvArrFront.GetTvByInfo(sWall, tmp))
				{
					// 좌측
					if(m_tvArrVertSection.GetTvByInfo(str+"번째내측헌치아래면좌측", tv))
					{
						tv.m_vecOverlap1 = tmp.GetXyLeft();
						tv.m_vecOverlap2 = tv.GetXyRight();
						tmp.m_vecOverlap1 = tv.m_v2;
						
						m_tvArrVertSection.SetTvByInfo(str+"번째내측헌치아래면좌측", tv);
					}

					// 우측
					if(m_tvArrVertSection.GetTvByInfo(str+"번째내측헌치아래면우측", tv))
					{
						tv.m_vecOverlap1 = tv.GetXyLeft();
						tv.m_vecOverlap2 = tmp.GetXyRight();
						tmp.m_vecOverlap2 = tv.m_v1;
						
						m_tvArrVertSection.SetTvByInfo(str+"번째내측헌치아래면우측", tv);
					}

					pWall->m_tvArrFront.SetTvByInfo(sWall, tmp);
				}
			}

			if(IsBoxType())
			{
				for(long w = 0; w < 2; w++)
				{
					CTwinVector tmp;
// 					CString sWall = _T("");
					CString sWall = (w == 0) ? _T("벽체하면좌측") : _T("벽체하면우측");
					if(pWall->m_tvArrFront.GetTvByInfo(sWall, tmp))
					{
						CString strLR = (w == 0)? _T("번째내측하부헌치윗면좌측") : _T("번째내측하부헌치윗면우측");
						if(m_tvArrVertSection.GetTvByInfo(str+strLR, tv))
						{
							tv.m_vecOverlap1 = tmp.GetXyLeft();
							tv.m_vecOverlap2 = tmp.GetXyRight();
							tmp.m_vecOverlap1 = tv.m_v1;
							tmp.m_vecOverlap2 = tv.m_v2;

							m_tvArrVertSection.SetTvByInfo(str+strLR, tv);
							pWall->m_tvArrFront.SetTvByInfo(sWall, tmp);
						}
					}
				}
			}
		}
	}
}

// 중심선형에서의 지점 좌표
CDPoint CRcBridgeApp::GetXyOnJijum(int nJ)
{
	double dSta = GetStationOnJijum(nJ, 0);

	CLineInfo* pLine = GetLineBase();
	CDPoint xy = pLine->GetXyLine(dSta);

	return xy;
}

double CRcBridgeApp::GetStationAtSlabEnd(BOOL bStt, int nLeft, double D)
{
	CLineInfo* pLine = GetLineBySlab(nLeft);
	CLineInfo* pLineL = GetLineLeftRight(TRUE);
	CLineInfo* pLineR = GetLineLeftRight(FALSE);

	CDPoint A[4];
	if(bStt)
		GetXySss(BRIDGE_OFF_STT_SLAB, A);
	else
		GetXySss(BRIDGE_OFF_END_SLAB, A);

	double staMatch = 0;
	if(pLine==pLineL)
	{
		int i=0; for(i=0; i<2; i++)
		{
			if(A[i]!=A[i+1])
			{
				pLine->GetStationMatchSegment(A[i], A[i+1], staMatch, D);
				break;
			}
		}
	}
	else if(pLine==pLineR)
	{
		int i=0; for(i=2; i>0; i--)
		{
			if(A[i]!=A[i+1])
			{
				pLine->GetStationMatchSegment(A[i], A[i+1], staMatch, D);
				break;
			}
		}
	}
	else
	{
		pLine->GetStationMatchSegment(A[1], A[2], staMatch, D);
	}

	// (ARCBRIDGE-2735) GetStationMatchSegment() 함수를 썼을때 교량이 선형을 벗어나 있다면 Station을 구하지 못한다.
	if(staMatch == 0)
	{
		pLine->GetStationMatchByXyAndAngle(A[0], (A[3] - A[0]).Unit(), staMatch, D);
	}

	return staMatch;
}

// BOOL bAscon : TRUE - 아크콘 권고안, FALSE - 콘크리트 권고안
void CRcBridgeApp::SetDataDefaultShapeAps(BOOL bStt, BOOL bLeft, BOOL bAscon)
{
	double LengthDefault = GetLengthDefaultAps(bStt);

	long left	= bLeft ? iLEFT : iRIGHT;
	CApsApp *pAps = bStt ? &m_apsStt[left] : &m_apsEnd[left];
	if(bAscon)
	{
		pAps->m_dL1	= LengthDefault;
		pAps->m_dL2	= 0;
		pAps->m_dL3	= 0;
		pAps->m_dH1	= 400;
		pAps->m_dH2	= 0;
	}
	else
	{
		pAps->m_dL1	= LengthDefault - 280 - 2000;
		pAps->m_dL2	= 2000;
		pAps->m_dL3	= 280;
		pAps->m_dH1	= 400;
		pAps->m_dH2	= 400;
	}		
	pAps->m_dAD1	= 300;
	pAps->m_dAD2	= 300;
	pAps->m_dSlope	= 4;
	pAps->m_dCtcDowelBar	= 400;
	pAps->m_bOrthoSlabEnd	= FALSE;
	pAps->m_dST	= 5;
	pAps->m_bApplyWingAng	= FALSE;
	if(bStt)
	{
		m_bBracketLengthStt = FALSE;
		m_dBrkLenStt[0] = 0.0;
		m_dBrkLenStt[1] = 0.0;
	}
	else
	{
		m_bBracketLengthEnd = FALSE;
		m_dBrkLenEnd[0] = 0.0;
		m_dBrkLenEnd[1] = 0.0;
	}

	// 완충슬래브도 같이 권고안 함
	double dFck	= bAscon ? 240 : 270;
	double dFy	= bAscon ? 3000 : 4000;
	if(m_pARcBridgeDataStd->IsLsdDesign())
	{
		dFck = GetValueFck(bStt? ePartApsStt : ePartApsEnd);
		dFy = GetValueFy(bStt? ePartApsStt : ePartApsEnd);
	}

	pAps->m_dFck	= dFck;
	pAps->m_dFy		= dFy;
	for(long cns = 0; cns < MAX_CNS; cns++)
	{
		SetDataDefaultShapeCns(bStt, left, cns, dFck, dFy);
	}
}


// 접속슬래브의 길이는 교대에서 교대뒷채움선 끝단에서 1.0m룰 더한 값으로 하고 최소 6.0m, 최대 10.0m를 적용
// 도로공사 표준도 (4.091, 2003년5월)
double CRcBridgeApp::GetLengthDefaultAps(BOOL bStt)
{
	// 브라켓 상면과 헌치의 너비 차이.. /////////////////////////////////////////
	CTwinVector tv;
	m_tvArrVertSection.GetTvByInfo(bStt ? "시점브라켓상면" : "종점브라켓상면", tv);
	double dBracketX = bStt ? tv.m_v2.x : tv.m_v1.x;
	m_tvArrVertSection.GetTvByInfo(bStt ? "시점브라켓헌치" : "종점브라켓헌치", tv);
	double dBracketHunchX = bStt ? tv.m_v1.x : tv.m_v2.x;
	double dDiffBracket = dBracketHunchX - dBracketX;
	// 브라켓 상면과 헌치의 너비 차이.. /////////////////////////////////////////

	double dHeightBridge = bStt ? GetXySectionSttEnd(TRUE).y-m_footingStt.m_dEL : GetXySectionSttEnd(FALSE).y-m_footingEnd.m_dEL;
	dHeightBridge -= bStt ? m_dBTHS : m_dBTHE;
	const double dGapBackFill	= 500; // 벽체뒷면~뒷채움시작점....
	double dGapSt	= 5;  // 스티로폴(Aps권고안적용)
	double dLengthDefault = (dHeightBridge + dGapBackFill + 1000) - dGapSt;
	dLengthDefault += bStt ? -dDiffBracket : dDiffBracket;

	CDPoint vAng = GetAngleJijum(bStt ? 0 : m_nQtyJigan);
	dLengthDefault = dLengthDefault/vAng.y;
	dLengthDefault = RoundUp(dLengthDefault/500.0,0)*500;

	if(dLengthDefault< 6000) dLengthDefault =  6000;
	if(dLengthDefault>10000) dLengthDefault = 10000;

	return dLengthDefault;
}

// 날개벽 상단 끝단의 평면 좌표
CDPoint CRcBridgeApp::GetXyPlaneWingWallEnd(BOOL bStt, BOOL bLeft, BOOL bInclBlockD)
{
	CWingWall* pWing = GetWingWall(bStt, bLeft);
	// 시점일 경우 교량 진행 방향의 좌우와 날개벽의 좌우가 반대임.
	if(bStt)
		bLeft = !bLeft;

	CLineInfo* pLine = GetLineLeftRight(bLeft);

	double se = bStt ? -1.0 : 1.0;
	double lr = bLeft ? -1.0 : 1.0;
	double dL1 = pWing->m_bExist ? pWing->m_dL1 : 0;
	double dL2 = pWing->m_bExist ? pWing->m_dL2 : 0;
	double dWidth = bInclBlockD ? dL1+dL2 : dL2;
	if(dL2==0)
		dWidth = dL1;
	double dSta = 0;
	CDPoint vAngWing = pWing->m_vAng;
	CDPoint vAng(0, 0);
	CDPoint A[4], xyStt(0, 0), xyEnd(0, 0);

	if(bStt)
		GetXySss(BRIDGE_OFF_STT_STT, A);
	else
		GetXySss(BRIDGE_OFF_END_END, A);

	xyStt = bLeft ? A[0] : A[3];

	pLine->GetStationMatchVerticalByXy(xyStt, dSta);
	vAng = pLine->GetAngleAzimuth(dSta);

	if(se*lr>0)
		vAng = vAng.RotateInv(vAngWing);
	else
		vAng = vAng.Rotate(vAngWing);

	xyEnd = xyStt + dWidth*vAng*se;

	return xyEnd;
}

// 날개벽 시점의 상하단 단면 좌표
CDPoint CRcBridgeApp::GetXySectionWingWallStt(BOOL bStt, BOOL bUpper, BOOL bLowHunch)
{
	CDPoint xyUpper(0, 0), xyLower(0, 0);
	CTwinVector tv;
	
	CTwinVectorArray tvArrVert;
	tvArrVert	= m_tvArrVertSection;
	BOOL bLeft	= m_nLeftCurVertSection == -1 ? (bStt ? FALSE : TRUE) : (bStt ? TRUE : FALSE);

	// (ARCBRIDGE-1496) Box형 교량의 하단은 조정할 필요가 없다.
	// ()
	if(bUpper || !IsBoxType() || m_bSlopeLowerSlab)
	{
		ModifyVertSectionForWing(tvArrVert, bStt, bLeft);
	}
	if(bStt)
	{
		tvArrVert.GetTvByInfo(_T("슬래브상면(시점벽체위)"), tv);
		xyUpper = tv.GetXyRight();
		
		if(m_nTypeBridge == BRIDGE_TYPE_TRAP2 || m_nTypeBridge == BRIDGE_TYPE_ARCH)
		{
			tvArrVert.GetTvByInfo(_T("시점벽체하부헌치"), tv);
			xyLower = (bLowHunch)? tv.m_v2 : CDPoint(tv.m_v1.x, tv.m_v2.y);
		}
		else if(m_nTypeBridge == BRIDGE_TYPE_BOX)
		{
			tvArrVert.GetTvByInfo(_T("하부슬래브하면(시점벽체아래)"), tv);
			xyLower = bLowHunch? tv.GetXyLeft() : tv.GetXyRight();
		}
		else
		{
			tvArrVert.GetTvByInfo(_T("시점벽체우측면"), tv);
			xyLower = tv.GetXyBottom();
		}		
	}
	else
	{
		tvArrVert.GetTvByInfo(_T("슬래브상면(종점벽체위)"), tv);
		xyUpper = tv.GetXyLeft();
		if(m_nTypeBridge == BRIDGE_TYPE_TRAP2 || m_nTypeBridge == BRIDGE_TYPE_ARCH)
		{
			tvArrVert.GetTvByInfo(_T("종점벽체하부헌치"), tv);
			xyLower = (bLowHunch)? tv.m_v1 : CDPoint(tv.m_v2.x, tv.m_v1.y);
		}
		else if(m_nTypeBridge == BRIDGE_TYPE_BOX)
		{
			tvArrVert.GetTvByInfo(_T("하부슬래브하면(종점벽체아래)"), tv);
			xyLower = bLowHunch? tv.GetXyRight() : tv.GetXyLeft();
		}
		else
		{
			tvArrVert.GetTvByInfo(_T("종점벽체좌측면"), tv);
			xyLower = tv.GetXyBottom();
		}
	}

	if(bUpper)
	{
		CWingWall *pWing = GetWingWall(bStt, bLeft);
		if(pWing)
		{
			xyUpper.y -= pWing->m_dH3;
		}
		
		return xyUpper;
	}
	return xyLower;
}

// 날개벽 끝단의 Elevation
double CRcBridgeApp::GetElWingWallEnd(BOOL bStt, BOOL bLeft)
{
	CLineInfo* pLine = GetLineBase();

	CDPoint xy = GetXyPlaneWingWallEnd(bStt, bLeft);
	double dDist = pLine->GetDisMatchVerticalByXy(xy);

	double dSta = 0;
	pLine->GetStationMatchVerticalByXy(xy, dSta);

	CDPoint xySlab = GetXySlabUpperActByDist(dSta, dDist, FALSE, TRUE, TRUE);

// 	CWingWall *pWing = GetWingWall(bStt, bLeft);
// 	if(pWing)
// 	{
// 		xySlab.y -= pWing->m_dH3;
// 	}

	return xySlab.y;
}

// 날개벽 시점의 높이
double CRcBridgeApp::GetHeightWingWall(BOOL bStt)
{
	CDPoint xyTop(0, 0), xyBot(0, 0);

	xyTop = GetXySectionWingWallStt(bStt, TRUE);
	xyBot = GetXySectionWingWallStt(bStt, FALSE);

	double dHeight = fabs(xyTop.y-xyBot.y);
	
	return dHeight;
}

// 날개벽 끝단의 피복
double CRcBridgeApp::GetCoverWingWallEnd(BOOL bStt, BOOL bLeft, BOOL bObq)
{
	if(!bObq)
		return m_dCoverWingEnd;

	CTwinVector tvEnd;
	CString strDir = bLeft ? _T("좌측") : _T("우측");
	GetTvWingWallPlane(bStt);
	m_tvArrPlaneWingWall.GetTvByInfo(strDir+_T("날개벽끝단"), tvEnd);

	CDPoint vWing = GetAngleAzimuthWingWall(bStt, bLeft);
	CDPoint vEnd = tvEnd.GetXyDir();
	CDPoint vBetEnd = vWing.BetweenAngle(vEnd);

	return (Compare(vBetEnd.y, 0.0, _T("="), 0.001))? 0 : m_dCoverWingEnd/fabs(vBetEnd.y);
}

void CRcBridgeApp::SetDataDefaultWingWallEl()
{
	if(!IsOutsideWall()) return;
	if(!m_bAutoWingEl) return;

	CWingWall* pWing = GetWingWall(TRUE, TRUE);
	pWing->m_dEL = Round(GetElWingWallEnd(TRUE, TRUE), 0);

	pWing = GetWingWall(TRUE, FALSE);
	pWing->m_dEL = Round(GetElWingWallEnd(TRUE, FALSE), 0);

	pWing = GetWingWall(FALSE, TRUE);
	pWing->m_dEL = Round(GetElWingWallEnd(FALSE, TRUE), 0);

	pWing = GetWingWall(FALSE, FALSE);
	pWing->m_dEL = Round(GetElWingWallEnd(FALSE, FALSE), 0);
}

void CRcBridgeApp::SetDataDefaultWingWallHeightSoil()
{
	if(!IsOutsideWall()) return;
	if(!m_bAutoWingHeightSoil) return;

	CWingWall* pWing = GetWingWall(TRUE, TRUE);
	pWing->m_dHeightSoil = Round(CalcUnderGroundDesignHeightForWingWall(TRUE, TRUE), 0);

	pWing = GetWingWall(TRUE, FALSE);
	pWing->m_dHeightSoil = Round(CalcUnderGroundDesignHeightForWingWall(TRUE, FALSE), 0);

	pWing = GetWingWall(FALSE, TRUE);
	pWing->m_dHeightSoil = Round(CalcUnderGroundDesignHeightForWingWall(FALSE, TRUE), 0);

	pWing = GetWingWall(FALSE, FALSE);
	pWing->m_dHeightSoil = Round(CalcUnderGroundDesignHeightForWingWall(FALSE, FALSE), 0);
}

void CRcBridgeApp::SetDataDefaultWingWall(BOOL bStt, BOOL bLeft, BOOL bFirst)
{
	CWingWall* pWing = GetWingWall(bStt, bLeft);

	if(bFirst || !pWing->IsValid())
	{
		pWing->m_dL2 = 6000.0;
	}

	if(pWing->m_bExist)
	{
		CDPoint vAngBet = GetAngleWingWallBetweenWall(bStt, bLeft);
		BOOL bHeel = FALSE; // 앞굽뒷굽 상관없이 날개벽 방향으로 구하므로 무조건 FALSE//(vAngBet.GetAngleDegree() < 180) ? FALSE : TRUE;
		
		// (ARCBRIDGE-2772) dDist1의 의도는 날개벽각도가 클때 외측길이가 아닌 내측 길이로 기본값을 해주려는 것으로 보인다.
		double dDist1 = (pWing->m_nAttachPos==0)? MAX(0, (vAngBet.GetAngleDegree() > 90)? fabs(pWing->m_dT2/vAngBet.y*vAngBet.x) : 0) : 0;
		
		pWing->m_bRAngEnd = TRUE;
		pWing->m_nDirHunch = 0;

		if(m_nTypeBridge == BRIDGE_TYPE_BOX)
		{
			pWing->m_dL1 = 6000.0;
			pWing->m_dL2 = 0;
		}
		else
		{
			pWing->m_dL1 = 3000.0;
			pWing->m_dL2 = GetLengthFootingWingWall(bStt, bLeft, FALSE, bHeel)-dDist1;
		}
		
		pWing->m_dEL = Round(GetElWingWallEnd(bStt, bLeft), 0);
		pWing->m_dHL = 1500.0;
		pWing->m_dH2 = 0.0;
		pWing->m_dH3 = 0.0;
		pWing->m_dSW = 1.0;
		pWing->m_dT1 = 500.0;
		pWing->m_dT2 = 500.0;
		pWing->m_dLT = 0.0;
		pWing->m_dSH = 2.0;
		pWing->m_dHuW1 = 300.0;
		pWing->m_dHuH1 = 300.0;
		pWing->m_dHuW2 = 300.0;
		pWing->m_dHuH2 = 300.0;
		pWing->m_bSWHor = FALSE;
	}
	else
		pWing->Clear();
}

// 날개벽 평면도 좌표
void CRcBridgeApp::GetTvWingWallPlane(BOOL bStt, BOOL bInclBracket, BOOL bUpper, BOOL bInclBlockD)
{
	CWingWall* pWingL = GetWingWall(bStt, TRUE);
	CWingWall* pWingR = GetWingWall(bStt, FALSE);
	CLineInfo* pLineL = GetLineLeftRight(!bStt);
	CLineInfo* pLineR = GetLineLeftRight(bStt);

	BOOL bBrkSideL = FALSE;
	BOOL bBrkSideR = FALSE;
	BOOL bGagakL = IsGagakForWing(bStt, TRUE);
	BOOL bGagakR = IsGagakForWing(bStt, FALSE);

	CDPoint A[4];
	if(bStt)
		GetXySss(BRIDGE_OFF_STT_STT, A);
	else
		GetXySss(BRIDGE_OFF_END_END, A);

	long nWingPosL = pWingL->m_nAttachPos;
	long nWingPosR = pWingR->m_nAttachPos;

	double dW = bStt ? m_dWS : m_dWE;		// 벽체 두께
	double dBW = bStt ? m_dBWS : m_dBWE;	// 브라켓 폭
	double dT1L = pWingL->m_dT1;
	double dT2L = pWingL->m_dT2;
	double dLTL = bUpper ? pWingL->m_dLT : 0.0;
	int nDirHunchL = dT1L==dT2L ? 1 : pWingL->m_nDirHunch;
	if(nDirHunchL==1)
	{
		dT1L = dT2L = bUpper ? pWingL->m_dT1 : pWingL->m_dT2;
		dLTL = 0.0;
	}
	if(nDirHunchL==2)
		dT1L = bUpper ? pWingL->m_dT1 : pWingL->m_dT2;
	double dDTL = dT2L - dT1L;
	double dSHL = pWingL->m_dSH;
	double dDSL = fabs(dDTL*dSHL);

	double dT1R = pWingR->m_dT1;
	double dT2R = pWingR->m_dT2;
	double dLTR = bUpper ? pWingR->m_dLT : 0.0;
	int nDirHunchR = dT1R==dT2R ? 1 : pWingR->m_nDirHunch;
	if(nDirHunchR==1)
	{
		dT1R = dT2R = bUpper ? pWingR->m_dT1 : pWingR->m_dT2;
		dLTR = 0.0;
	}
	if(nDirHunchR==2)
		dT1R = bUpper ? pWingR->m_dT1 : pWingR->m_dT2;
	double dDTR = dT2R - dT1R;
	double dSHR = pWingR->m_dSH;
	double dDSR = fabs(dDTR*dSHR);
	double dHuWL = pWingL->m_dHuW2;
	double dHuHL = pWingL->m_dHuH2;
	double dHuWR = pWingR->m_dHuW2;
	double dHuHR = pWingR->m_dHuH2;
	BOOL bRAngL = pWingL->m_bRAngEnd;
	BOOL bRAngR = pWingR->m_bRAngEnd;
	CDPoint vX(1, 0), vY(0, 1);

	// 슬래브 끝단 좌표(시종점 좌표가 아님)
	double nCountJijum = GetCountJijum();
	CJijum* pJ = bStt ? GetJijum(0) : GetJijum((int)(nCountJijum-1));
	CDPoint vAng = bStt ? GetAngleByTarget(BRIDGE_OFF_STT_SLAB, 0) : GetAngleByTarget(BRIDGE_OFF_END_SLAB, 0);
	double se = bStt ? -1.0 : 1.0;
	double dSta = GetStationByTarget(bStt ? BRIDGE_OFF_STT_SLAB : BRIDGE_OFF_END_SLAB, 0, FALSE);//bStt ? m_dStationBridgeStt : m_dStationBridgeStt+m_dLengthBridge;
	double dStaSlab = dSta;//dSta + dW*se/vAng.y;
	double dStaBrk = GetLineBase()->GetStationByOffsetWidth(dStaSlab, dBW*se, pJ->m_vAngle);//dSta + (dW+dBW)*se/vAng.y;

	// (ARCBRIDGE-3019) 곡선선형인경우 브라켓은 Station과 지점각도를 가지고 그리면 안된다. 벽체에서 offset해서 그려야한다.
	CDPoint xySlabL = (bGagakL)? GetXySlabEdgeActByGagak(dStaSlab, vAng, bStt ? FALSE : TRUE) : GetXySlabEdgeAct(dStaSlab, vAng, bStt ? FALSE : TRUE);
	CDPoint xySlabR = (bGagakR)? GetXySlabEdgeActByGagak(dStaSlab, vAng, bStt ? TRUE : FALSE) : GetXySlabEdgeAct(dStaSlab, vAng, bStt ? TRUE : FALSE);
	// 브라켓 좌표
	//CDPoint xyBrkL = GetXySlabEdgeAct(dStaBrk, vAng, bStt ? FALSE : TRUE);
	//CDPoint xyBrkR = GetXySlabEdgeAct(dStaBrk, vAng, bStt ? TRUE : FALSE);
	CDPoint xyBrkL = xySlabL;
	CDPoint xyBrkR = xySlabR;
	Offset(xyBrkL, xyBrkR, -dBW);

	// 외측 점 좌표
	CDPoint xyOutSttL = bStt ? A[3] : A[0];
	CDPoint xyOutEndL = GetXyPlaneWingWallEnd(bStt, TRUE, bInclBlockD);
	CDPoint xyOutSttR = bStt ? A[0] : A[3];
	CDPoint xyOutEndR = GetXyPlaneWingWallEnd(bStt, FALSE, bInclBlockD);

	// 각도 설정
	CDPoint vLineL = pLineL->GetAngleAzimuth(dStaBrk);
	CDPoint vLineR = pLineR->GetAngleAzimuth(dStaBrk);
	CDPoint vAngJijum = (xyOutSttR-xyOutSttL).Unit();	// 슬래브 끝단 각도
	CDPoint vAngWingL = (xyOutEndL-xyOutSttL).Unit();	// 좌측 날개벽 각도
	CDPoint vAngWingR = (xyOutEndR-xyOutSttR).Unit();	// 우측 날개벽 각도
	CDPoint	vAngEndL = bRAngL ? vAngWingL.Rotate90() : (nWingPosL==0)? vAngJijum:vLineL;	// 좌측 날개벽 단부 각도
	CDPoint	vAngEndR = bRAngR ? vAngWingR.Rotate90() : (nWingPosR==0)? vAngJijum:vLineR;	// 우측 날개벽 단부 각도

	// 내측 점 좌표
	CDPoint xyInSttL(0, 0), xyInEndL(0, 0), xyInSttR(0, 0), xyInEndR(0, 0), xyEndWallL(0, 0), xyEndWallR(0, 0);		// 시종점 좌표
	CDPoint xyInSttL1(0, 0), xyInSttL2(0, 0), xyInSttR1(0, 0), xyInSttR2(0, 0);	// 날개벽 시점 헌치 좌표
	CDPoint xyHunchL1(0, 0), xyHunchL2(0, 0), xyHunchR1(0, 0), xyHunchR2(0, 0);	// 날개벽 내부 헌치 좌표
	CDPoint xyTemp1(0, 0), xyTemp2(0, 0), xyTemp3(0, 0), xyTemp4(0, 0);

	// ...좌측
	if(pWingL->m_bExist && !ComparePt(xyOutSttL, xyOutEndL, _T("=")))
	{
		xyTemp1 = xyOutEndL;
		xyTemp2 = xyOutSttL;
		xyTemp3 = xyOutSttL;
		xyTemp4 = xyOutSttR;
		Offset(xyTemp1, xyTemp2, -dT2L);
		Offset(xyTemp3, xyTemp4, -dW);

		// 날개벽이 있다면 브라켓도 수정 modify by jaeho(20050609)////
		xyBrkL	= xyOutSttL;
		xyBrkR	= xyOutSttR;
		Offset(xyBrkL, xyBrkR, -(dW+dBW));
		///////////////////////////////////////////////////////////////

		// ...날개벽 시점 헌치
		if(nWingPosL == 0)
		{
			GetXyMatchLineAndLine(xyTemp1, vAngWingL, xyTemp4, vAngJijum, xyInSttL);
			GetXyChamferSegAndSeg(xyTemp1, xyInSttL, xyTemp4, xyInSttL, xyInSttL1, xyInSttL2, dHuWL, dHuHL);
			xyEndWallL = xyInSttL2;
		}
		else
		{
			GetXyMatchLineAndLine(xyTemp4, vAngJijum, xyOutSttL, vLineL, xyEndWallL);
			GetXyMatchLineAndLine(xyTemp1, vAngWingL, xyEndWallL, vLineL, xyInSttL);
			if(GetXyChamferSegAndSeg(xyTemp1, xyInSttL, xyEndWallL, xyInSttL, xyInSttL1, xyInSttL2, dHuWL, dHuHL))
			{
				if(Compare(~(xyInSttL - xyEndWallL), dHuHL, "<")) xyInSttL2 = xyEndWallL;
			}
			else
			{
				xyInSttL1	= xyEndWallL;
				xyInSttL2	= xyEndWallL;
			}
		}

		// ...단부
		xyTemp1 = xyOutEndL;
		xyTemp2 = xyOutSttL;
		Offset(xyTemp1, xyTemp2, -dT1L);
		GetXyMatchLineAndLine(xyTemp1, vAngWingL, xyOutEndL, vAngEndL, xyInEndL);

		xyTemp3 = xyOutEndL - dT1L*vAngWingL.Rotate90();
		xyTemp4 = nDirHunchL==1 ? xyInEndL : xyTemp3;

		// ...날개벽 내부 헌치
		xyHunchL1 = (bUpper)?xyTemp4 - dLTL*vAngWingL : xyInEndL;
		xyHunchL2 = (bUpper)? xyHunchL1 - dDSL*vAngWingL + dDTL*vAngWingL.RotateInv90() : xyInEndL;
	}
	else
	{
		// 날개벽이 없어도 가각이 들어갈때는 브라켓이 수정 되어야...
		if(bGagakL)
		{
			xyBrkL	= xyOutSttL;
			xyBrkR	= xyOutSttR;
			Offset(xyBrkL, xyBrkR, -(dW+dBW));
		}

		xyInSttL = xySlabL;
		xyInSttL1 = xySlabL;
		xyInSttL2 = xySlabL;
		xyInEndL = xySlabL;
		xyHunchL1 = xySlabL;
		xyHunchL2 = xySlabL;
		xyEndWallL = xySlabL;
		bBrkSideL = TRUE;
	}

	// ...우측
	if(pWingR->m_bExist && !ComparePt(xyOutSttR, xyOutEndR, _T("=")))
	{
		xyTemp1 = xyOutEndR;
		xyTemp2 = xyOutSttR;
		xyTemp3 = xyOutSttR;
		xyTemp4 = xyOutSttL;
		Offset(xyTemp1, xyTemp2, dT2R);
		Offset(xyTemp3, xyTemp4, dW);

		// 날개벽이 있다면 브라켓도 수정modify by jaeho(20050609)//
		xyBrkL	= xyOutSttL;
		xyBrkR	= xyOutSttR;
		Offset(xyBrkL, xyBrkR, -(dW+dBW));
		///////////////////////////////////////////////////////////

		// ...날개벽 시점 헌치
		if(nWingPosR == 0)
		{
			GetXyMatchLineAndLine(xyTemp1, vAngWingR, xyTemp4, vAngJijum, xyInSttR);
			GetXyChamferSegAndSeg(xyTemp1, xyInSttR, xyTemp4, xyInSttR, xyInSttR1, xyInSttR2, dHuWR, dHuHR);
			xyEndWallR = xyInSttR2;
		}
		else
		{
			GetXyMatchLineAndLine(xyTemp4, vAngJijum, xyOutSttR, vLineR, xyEndWallR);
			GetXyMatchLineAndLine(xyTemp1, vAngWingR, xyEndWallR, vLineR, xyInSttR);
			if(GetXyChamferSegAndSeg(xyTemp1, xyInSttR, xyEndWallR, xyInSttR, xyInSttR1, xyInSttR2, dHuWR, dHuHR))
			{
				if(Compare(~(xyInSttR - xyEndWallR), dHuHR, "<")) xyInSttR2 = xyEndWallR;
			}
			else
			{
				xyInSttR1	= xyEndWallR;
				xyInSttR2	= xyEndWallR;
			
			}
		}

		// ...단부
		xyTemp1 = xyOutEndR;
		xyTemp2 = xyOutSttR;
		Offset(xyTemp1, xyTemp2, dT1R);
		GetXyMatchLineAndLine(xyTemp1, vAngWingR, xyOutEndR, vAngEndR, xyInEndR);

		xyTemp3 = xyOutEndR + dT1R*vAngWingR.Rotate90();
		xyTemp4 = nDirHunchR==1 ? xyInEndR : xyTemp3;

		// ...날개벽 내부 헌치
		xyHunchR1 = (bUpper)? xyTemp4 - dLTR*vAngWingR : xyInEndR;
		xyHunchR2 = (bUpper)? xyHunchR1 - dDSR*vAngWingR + dDTR*vAngWingR.Rotate90() : xyInEndR;
	}
	else
	{
		// 날개벽이 없어도 가각이 들어갈때는 브라켓이 수정 되어야...
		if(bGagakR)
		{
			xyBrkL	= xyOutSttL;
			xyBrkR	= xyOutSttR;
			Offset(xyBrkL, xyBrkR, -(dW+dBW));
		}

		xyInSttR = xySlabR;
		xyInSttR1 = xySlabR;
		xyInSttR2 = xySlabR;
		xyInEndR = xySlabR;
		xyHunchR1 = xySlabR;
		xyHunchR2 = xySlabR;
		xyEndWallR = xySlabR;
		bBrkSideR = TRUE;
	}

	CTwinVector tv;
	m_tvArrPlaneWingWall.RemoveAll();

	// 외측선
	tv.m_v1 = xyOutSttL; tv.m_v2 = xyOutEndL;
	tv.m_sInfo = _T("좌측날개벽외측");
	m_tvArrPlaneWingWall.Add(tv);	// 좌측
	tv.m_sInfo = _T("슬래브끝단외측");
	tv.m_v1 = xyOutSttL; tv.m_v2 = xyOutSttR;
	m_tvArrPlaneWingWall.Add(tv);	// 슬래브 끝단
	tv.m_v1 = xyOutSttR; tv.m_v2 = xyOutEndR;
	tv.m_sInfo = _T("우측날개벽외측");
	m_tvArrPlaneWingWall.Add(tv);	// 우측

	// 내측선
	// ...좌측
	if(pWingL->m_bExist)
	{
		tv.m_v1 = xyOutEndL; tv.m_v2 = xyInEndL;
		tv.m_sInfo = _T("좌측날개벽끝단");
		m_tvArrPlaneWingWall.Add(tv);
	}
	tv.m_v1 = xyInEndL; tv.m_v2 = xyHunchL1;
	tv.m_sInfo = _T("좌측날개벽내부헌치전");
	m_tvArrPlaneWingWall.Add(tv);
	tv.m_v1 = xyHunchL1; tv.m_v2 = xyHunchL2;
	tv.m_sInfo = _T("좌측날개벽내부헌치");
	m_tvArrPlaneWingWall.Add(tv);
	tv.m_v1 = xyHunchL2; tv.m_v2 = xyInSttL1;
	tv.m_sInfo = _T("좌측날개벽내부헌치후");
	m_tvArrPlaneWingWall.Add(tv);
	tv.m_v1 = xyInSttL1; tv.m_v2 = xyInSttL2;
	tv.m_sInfo = _T("좌측날개벽시점헌치");
	m_tvArrPlaneWingWall.Add(tv);
	if(nWingPosL != 0)
	{
		tv.m_v1 = xyEndWallL; tv.m_v2 = xyInSttL2;
		tv.m_sInfo = _T("좌측벽체끝단");
		m_tvArrPlaneWingWall.Add(tv);
	}

	// ...우측
	if(pWingR->m_bExist)
	{
		tv.m_v1 = xyOutEndR; tv.m_v2 = xyInEndR;
		tv.m_sInfo = _T("우측날개벽끝단");
		m_tvArrPlaneWingWall.Add(tv);
	}
	tv.m_v1 = xyInEndR; tv.m_v2 = xyHunchR1;
	tv.m_sInfo = _T("우측날개벽내부헌치전");
	m_tvArrPlaneWingWall.Add(tv);
	tv.m_v1 = xyHunchR1; tv.m_v2 = xyHunchR2;
	tv.m_sInfo = _T("우측날개벽내부헌치");
	m_tvArrPlaneWingWall.Add(tv);
	tv.m_v1 = xyHunchR2; tv.m_v2 = xyInSttR1;
	tv.m_sInfo = _T("우측날개벽내부헌치후");
	m_tvArrPlaneWingWall.Add(tv);
	tv.m_v1 = xyInSttR1; tv.m_v2 = xyInSttR2;
	tv.m_sInfo = _T("우측날개벽시점헌치");
	m_tvArrPlaneWingWall.Add(tv);
	if(nWingPosR != 0)
	{
		tv.m_v1 = xyEndWallR; tv.m_v2 = xyInSttR2;
		tv.m_sInfo = _T("우측벽체끝단");
		m_tvArrPlaneWingWall.Add(tv);
	}

	// ...슬래브 끝단
	tv.m_v1 = xyEndWallL; tv.m_v2 = xyEndWallR;
	tv.m_sInfo = _T("슬래브끝단내측");
	m_tvArrPlaneWingWall.Add(tv);
	// ...브라켓
	CDPoint xyTemp(0, 0);

	if(bInclBracket)
	{
		if(nWingPosL != 0)
		{
			GetXyMatchLineAndLine(xyInSttL, vLineL, xyBrkR, vAngJijum, xyBrkL);
//			if(bStt) m_bBracketLengthStt = TRUE;
//			else m_bBracketLengthEnd = TRUE;
			bBrkSideL = TRUE;
		}
		else if(!GetXyMatchSegAndLine(xyHunchL2, xyInSttL1, xyBrkR, vAngJijum, xyBrkL))
		{
			if(!GetXyMatchSegAndLine(xyInSttL1, xyInSttL2, xyBrkR, vAngJijum, xyBrkL))
			{
				GetXyMatchLineAndLine(xyInSttL, vLineL, xyBrkR, vAngJijum, xyBrkL);
//				if(bStt) m_bBracketLengthStt = TRUE;
//				else m_bBracketLengthEnd = TRUE;
				bBrkSideL = TRUE;
			}
		}

		if(nWingPosR != 0)
		{
			GetXyMatchLineAndLine(xyInSttR, vLineR, xyBrkL, vAngJijum, xyBrkR);
//			if(bStt) m_bBracketLengthStt = TRUE;
//			else m_bBracketLengthEnd = TRUE;
			bBrkSideR = TRUE;
		}
		else if(!GetXyMatchSegAndLine(xyHunchR2, xyInSttR1, xyBrkL, vAngJijum, xyBrkR))
		{
			if(!GetXyMatchSegAndLine(xyInSttR1, xyInSttR2, xyBrkL, vAngJijum, xyBrkR))
			{
				GetXyMatchLineAndLine(xyInSttR, vLineR, xyBrkL, vAngJijum, xyBrkR);
//				if(bStt) m_bBracketLengthStt = TRUE;
//				else m_bBracketLengthEnd = TRUE;
				bBrkSideR = TRUE;
			}
		}
		tv.m_v1 = xyBrkL; tv.m_v2 = xyBrkR;
		tv.m_sInfo = _T("브라켓");
		m_tvArrPlaneWingWall.Add(tv);
	
		int nSize = m_tvArrPlaneWingWall.GetSize();
		BOOL bBrkLen = bStt ? m_bBracketLengthStt : m_bBracketLengthEnd;

		if(bBrkLen)
		{
			bBrkSideL = TRUE;
			bBrkSideR = TRUE;
			xyBrkL = GetXyPlaneBracket(bStt, TRUE);
			xyBrkR = GetXyPlaneBracket(bStt, FALSE);

			if(GetXyMatchSegAndSeg(xyHunchL2, xyInSttL1, xyBrkR, xyBrkL, xyTemp))
			{
				xyBrkL = xyTemp;
				bBrkSideL = FALSE;
			}
			if(GetXyMatchSegAndSeg(xyInSttL1, xyInSttL2, xyBrkR, xyBrkL, xyTemp))
			{
				xyBrkL = xyTemp;
				bBrkSideL = FALSE;
			}

			if(GetXyMatchSegAndSeg(xyHunchR2, xyInSttR1, xyBrkL, xyBrkR, xyTemp))
			{
				xyBrkR = xyTemp;
				bBrkSideR = FALSE;
			}
			if(GetXyMatchSegAndSeg(xyInSttR1, xyInSttR2, xyBrkR, xyBrkL, xyTemp))
			{
				xyBrkR = xyTemp;
				bBrkSideR = FALSE;
			}

			tv.m_v1 = xyBrkL; tv.m_v2 = xyBrkR;
			tv.m_sInfo = _T("브라켓");
			m_tvArrPlaneWingWall.SetAt(nSize-1, tv);
		}	
		
		CDPoint xyBrkEndL(0, 0), xyBrkEndR(0, 0);
		CTwinVector tvApsL, tvApsR;
		CApsApp* pAPS = NULL;
	
		pWingL->m_bAttachBraket = TRUE;
		pWingR->m_bAttachBraket = TRUE;

		if(bBrkSideL)
		{
			pAPS = bStt ? &m_apsStt[iLEFT] : &m_apsEnd[iLEFT];
			pAPS->GetTvPlan(TRUE);
			pAPS->m_tvArrPlan.GetTvByInfo(_T("접속슬래브좌측"), tvApsL);
			CDPoint vApsL = tvApsL.GetXyDir();
			
			if(!GetXyMatchSegAndLine(xyEndWallL, xyEndWallR, xyBrkL, vLineL, xyBrkEndL))
			{
				if(!GetXyMatchSegAndLine(xyHunchL2, xyInSttL1, xyBrkL, vLineL, xyBrkEndL))
					if(!GetXyMatchSegAndLine(xyInSttL1, xyInSttL2, xyBrkL, vLineL, xyBrkEndL))
						GetXyMatchLineAndLine(xyEndWallL, vAngJijum, xyBrkL, vLineL, xyBrkEndL);
			}
			tv.m_v1 = xyBrkL; tv.m_v2 = xyBrkEndL;
			tv.m_sInfo = _T("브라켓좌측끝단");
			m_tvArrPlaneWingWall.Add(tv);
			if(tv.GetLength() > 0) pWingL->m_bAttachBraket = FALSE;
		}
		
		if(bBrkSideR)
		{
			pAPS = bStt ? &m_apsStt[iRIGHT] : &m_apsEnd[iRIGHT];
			if(!pAPS->m_bIs)
				pAPS = bStt ? &m_apsStt[iLEFT] : &m_apsEnd[iLEFT];
			else
				pAPS->GetTvPlan(TRUE);
			pAPS->m_tvArrPlan.GetTvByInfo(_T("접속슬래브우측"), tvApsR);
			CDPoint vApsR = tvApsR.GetXyDir();

			if(!GetXyMatchSegAndLine(xyEndWallL, xyEndWallR, xyBrkR, vLineR, xyBrkEndR))
			{
				if(!GetXyMatchSegAndLine(xyHunchR2, xyInSttR1, xyBrkR, vLineR, xyBrkEndR))
					if(!GetXyMatchSegAndLine(xyInSttR1, xyInSttR2, xyBrkR, vLineR, xyBrkEndR))
						GetXyMatchLineAndLine(xyEndWallR, vAngJijum, xyBrkR, vLineR, xyBrkEndR);
			}
			tv.m_v1 = xyBrkR; tv.m_v2 = xyBrkEndR;
			tv.m_sInfo = _T("브라켓우측끝단");
			m_tvArrPlaneWingWall.Add(tv);
			if(tv.GetLength() > 0) pWingR->m_bAttachBraket = FALSE;
		}
	}
}

CDPoint CRcBridgeApp::GetTvArrayWingWallInitialMesh(CTwinVectorArray& tvArrWing, CTwinVectorArray& tvArrFix, CTwinVectorArray& tvArrSK, CDPoint &xyWingWallStt, CDPoint &xyWingWallEnd, CDPoint &xyWingFootStt, CDPoint &xyWingFootEnd, double &dDistThick, long nIndexArea, BOOL bStt, BOOL bLeft)
{
	tvArrWing.RemoveAll();
	tvArrFix.RemoveAll();
	tvArrSK.RemoveAll();

	CWingWall* pWing = GetWingWall(bStt, bLeft);
	if(pWing->m_nDirHunch == 0 && nIndexArea > 0) return CDPoint(0,0);

	long se = bStt ? -1 : 1;
	long nLeft = bLeft ? -se : se;
	long nOut = 1;

	GetTvVertSection_Std(nLeft, TRUE, TRUE, FALSE, 2);

	CDPoint vAng = GetAngleWingWallBetweenWall(bStt, bLeft);

	double dL1 = pWing->m_dL1;
	double dL2 = pWing->m_dL2;
	double dH1 = pWing->m_dHL;
	double dSW = pWing->m_dSW;
	double dBTH = bStt ? m_dBTHS : m_dBTHE;
	double dBH  = bStt ? m_dBHS : m_dBHE;
	double dBW  = bStt ? m_dBWS/vAng.y : m_dBWE/vAng.y;
	double dBHH = bStt ? m_dBHHS : m_dBHHE;
	double dBHW = bStt ? m_dBHWS/vAng.y : m_dBHWE/vAng.y;
	double dElEnd = pWing->GetEndEl(FALSE);
	double dW  = bStt ? m_dWS/vAng.y : m_dWE/vAng.y;

	if(pWing->m_nAttachPos == 1)
	{
		dW		= 0;
		dBW		= 0;
		dBHH	= 0;
		dBHW	= 0;
	}

	BOOL bExistBrackt = (dBTH > 0 && dBH > 0 && dBW > 0 && dBHH > 0 && dBHW > 0) ? TRUE : FALSE;

	CDPoint vX(1,0), vY(0,1);
	CDPoint xyWingStt = GetXySectionWingWallStt(bStt, TRUE);
	CDPoint xyWingEnd = CDPoint(xyWingStt.x + (dL1 + dL2)*se, dElEnd);
	if(pWing->m_nAttachPos == 0)
	{
		GetXyMatchSegAndLine(xyWingStt, xyWingEnd, CDPoint(xyWingStt.x + dW*se, xyWingStt.y), vY, xyWingStt);
	}
	CDPoint xyNoriStt = xyWingEnd - dH1 * vY;
	CDPoint xyNoriEnd = xyNoriStt;
	CDPoint xyFootStt = GetXySectionWingWallStt(bStt, FALSE) + dW*se*vX;
	xyNoriEnd.x += -dL1 * se;
	xyNoriEnd.y += -dL1 / dSW;
	if(pWing->m_nAttachPos == 0)
	{
		if(Compare(dL2, 0.0, _T("<=")))
		{
			GetXyMatchSegAndLine(xyNoriStt, xyNoriEnd, xyFootStt, vY, xyNoriEnd);
		}
	}
	else
	{
		if(Compare(dL2, 0.0, _T("<=")))
		{
			GetXyMatchSegAndLine(xyNoriStt, xyNoriEnd, xyWingStt, vY, xyNoriEnd);
		}
// 		else
// 		{
// 			GetXyMatchSegAndLine(xyNoriStt, xyNoriEnd, xyFootStt, vY, xyNoriEnd);
// 		}
	}

	double dLenFoot = min(GetLengthFootingWingWall(bStt, bLeft, FALSE, FALSE, nOut) - fabs(dW), fabs(xyFootStt.x - xyNoriEnd.x));
	CDPoint xyFootEnd = xyFootStt + dLenFoot * se * vX;
	CDPoint xyWingBot(0,0);
	if(IsBoxType())
	{
		if(m_bExistProtectionRiseFooting && dL2 > 0)
		{
			dLenFoot = m_bExistProtectionRiseFooting ? m_dPRF_W / vAng.y : 0;
			xyFootStt.y += m_dPRF_H1 + m_dPRF_H2;
			xyFootEnd = xyFootStt + dLenFoot * se * vX;
			xyFootEnd.y -= m_dPRF_H2;
			CDPoint xyMatch(0,0);
			xyWingBot = CDPoint(xyNoriEnd.x, xyFootEnd.y);
			if(GetXyMatchSegAndSeg(xyNoriEnd, xyWingBot, xyFootStt, xyFootEnd, xyMatch))
			{
				xyFootEnd = xyMatch;
				xyWingBot = xyMatch;
			}
		}
		else
		{
			xyFootStt.y += pWing->m_dH2;
			xyFootEnd = xyFootStt;
			CDPoint xyMatch(0,0);
			if(GetXyMatchSegAndLine(xyNoriStt, xyNoriEnd, xyFootEnd, CDPoint(1, 0), xyMatch))
			{
				xyNoriEnd = xyMatch;
			}

			
			xyWingBot = xyFootEnd;
		}
	}
	else
	{
		xyWingBot = CDPoint(xyNoriEnd.x, xyFootEnd.y);
	}
	xyWingFootStt = xyFootStt;
	xyWingFootEnd = xyFootEnd;

	//////////////////////////////////////////////////////////////////////////
	CTwinVector tv;

	tv.SetFromPoint(xyWingStt, xyWingEnd);
	tvArrWing.Add(tv);
	tv.SetFromPoint(xyWingEnd, xyNoriStt);
	tvArrWing.Add(tv);
	tv.SetFromPoint(xyNoriStt, xyNoriEnd);
	tvArrWing.Add(tv);
	if(Compare(dL2, 0.0, _T(">")))
	{
		if(ComparePt(xyNoriEnd, xyWingBot, _T("=")) == FALSE)
		{
			tv.SetFromPoint(xyNoriEnd, xyWingBot);
			tvArrWing.Add(tv);
		}
		if(ComparePt(xyWingBot, xyFootEnd, _T("=")) == FALSE)
		{
			tv.SetFromPoint(xyWingBot, xyFootEnd);
			tvArrWing.Add(tv);
		}
	}
	else if(pWing->m_bMatchBottom)
	{
		tv.SetFromPoint(xyNoriEnd, xyWingBot);
		tvArrWing.Add(tv);
	}
	

	CDPoint xyBrk1 = xyWingStt - dBTH * vY;
	CDPoint xyBrk2 = xyBrk1 + dBW * se * vX;
	CDPoint xyBrk3 = xyBrk2 - dBH * vY;
	CDPoint xyBrk4 = xyBrk3 - dBHW * se * vX - dBHH * vY;
	if(pWing->m_nAttachPos == 0)
	{
		if(bExistBrackt)
		{
			tv.SetFromPoint(xyWingStt, xyBrk1);
			tvArrFix.Add(tv);
			tv.SetFromPoint(xyBrk1, xyBrk2);
			tvArrFix.Add(tv);
			tv.SetFromPoint(xyBrk2, xyBrk3);
			tvArrFix.Add(tv);
			tv.SetFromPoint(xyBrk3, xyBrk4);
			tvArrFix.Add(tv);
		}
		else
		{
			xyBrk4 = xyWingStt;
		}
		if(pWing->m_bMatchBottom)
		{
			tv.SetFromPoint(xyBrk4, xyWingBot);
			tvArrFix.Add(tv);
		}
		else if(Compare(dL2, 0.0, _T("<=")))
		{
			tv.SetFromPoint(xyBrk4, xyNoriEnd);
			tvArrFix.Add(tv);
		}
		else
		{
			// 브래킷끝과 기초시작이 미세하게 어긋나는 경우 구속조건에 문제가 발생
			xyFootStt.x = xyBrk4.x;
			tv.SetFromPoint(xyBrk4, xyFootStt);
			tvArrFix.Add(tv);
			if(ComparePt(xyFootStt, xyFootEnd, _T("=")) == FALSE)
			{
				tv.SetFromPoint(xyFootStt, xyFootEnd);
				tvArrFix.Add(tv);
			}
		}
	}
	else
	{
		if(pWing->m_bMatchBottom)
		{
			// 측면부착일때는 브라켓이 없지. 상단까지 올려준다.
			tv.SetFromPoint(xyWingStt, xyWingBot);
			tvArrFix.Add(tv);
		}
		else if(Compare(dL2, 0.0, _T(">")))
		{
			xyFootStt.x = xyWingStt.x;
			tv.SetFromPoint(xyFootEnd, xyFootStt);
			tvArrFix.Add(tv);
			tv.SetFromPoint(xyWingStt, xyFootStt);
			tvArrFix.Add(tv);
		}
		else
		{
			tv.SetFromPoint(xyWingStt, xyNoriEnd);
			tvArrFix.Add(tv);
		}
	}

	CRebarPlacing rp;
	rp.RemoveMeanlessTwinVector(tvArrWing);
	rp.RemoveMeanlessTwinVector(tvArrFix);

	tvArrWing.AddFromTvArray(tvArrFix);
	tvArrWing.Sort();
	// 두께가 변하는 경우 구간별로 분리
	xyWingWallEnd = xyWingEnd;
	xyWingWallStt = xyWingStt;
	if(pWing->m_nDirHunch == 1)
	{
		CTwinVectorArray tvArrDiv[2];
		double dElevation = GetXySectionWingWallStt(bStt, TRUE).y - pWing->m_dLT - (pWing->m_dT2 - pWing->m_dT1) * pWing->m_dSH;
		long nSize = tvArrWing.GetSize();
		for(long ix = 0; ix < nSize; ix++)
		{
			tv = tvArrWing.GetAt(ix);
			// 중복구간 포함해서 Add
			double dMaxEle = MAX(tv.m_v1.y, tv.m_v2.y);
			double dMinEle = MIN(tv.m_v1.y, tv.m_v2.y);
			if(Compare(dMinEle,  dElevation, _T(">="), 0.1) && Compare(dMaxEle,  dElevation, _T(">="), 0.1))
			{
				tvArrDiv[iUPPER].Add(tv);
			}
			if(Compare(dMaxEle,  dElevation, _T("<="), 0.1) && Compare(dMinEle,  dElevation, _T("<"), 0.1))
			{
				tvArrDiv[iLOWER].Add(tv);
			}
			if(Compare(dElevation,  dMinEle, _T(">"), 0.1) && Compare(dElevation,  dMaxEle, _T("<"), 0.1))
			{
				CDPoint xyMatch(0,0);
				CTwinVector tvAdd;
				if(GetXyMatchSegAndLine(tv.m_v1, tv.m_v2, CDPoint(0,dElevation), vX, xyMatch))
				{
					if(tv.m_v1.y > tv.m_v2.y)
					{
						tvAdd.m_v1 = tv.m_v1;
						tvAdd.m_v2 = xyMatch;
						tvArrDiv[iUPPER].Add(tvAdd);
						tvAdd.m_v1 = xyMatch;
						tvAdd.m_v2 = tv.m_v2;
						tvArrDiv[iLOWER].Add(tvAdd);
					}
					if(tv.m_v1.y < tv.m_v2.y)
					{
						tvAdd.m_v1 = tv.m_v2;
						tvAdd.m_v2 = xyMatch;
						tvArrDiv[iUPPER].Add(tvAdd);
						tvAdd.m_v1 = xyMatch;
						tvAdd.m_v2 = tv.m_v1;
						tvArrDiv[iLOWER].Add(tvAdd);
					}
				}
			}
		}
		tvArrDiv[iUPPER].Sort();
		tvArrDiv[iLOWER].Sort();

		tvArrWing.RemoveAll();
		if(tvArrDiv[nIndexArea].GetSize() > 0)
		{
			tvArrWing.AddFromTvArray(tvArrDiv[nIndexArea]);
			CTwinVector tvSK;
			tvSK.m_v1 = tvArrDiv[nIndexArea].GetAt(0).m_v1;
			tvSK.m_v2 = tvArrDiv[nIndexArea].GetAt(tvArrDiv[nIndexArea].GetSize()-1).m_v2;
			tvArrSK.Add(tvSK);
			tvArrWing.Add(tvSK);
		}
	}
	else if(pWing->m_nDirHunch == 2)
	{
		dDistThick = xyWingEnd.x - (pWing->m_dLT + (pWing->m_dT2 - pWing->m_dT1) * pWing->m_dSH) * se;

		CTwinVectorArray tvArrDiv[2];
		long nSize = tvArrWing.GetSize();
		for(long ix = 0; ix < nSize; ix++)
		{
			tv = tvArrWing.GetAt(ix);
			// 중복구간 포함해서 Add
			double dMaxDist = MAX(tv.m_v1.x, tv.m_v2.x);
			double dMinDist = MIN(tv.m_v1.x, tv.m_v2.x);
			if(Compare(dMinDist,  dDistThick, _T(">=")) && Compare(dMaxDist,  dDistThick, _T(">=")))
			{
				tvArrDiv[iRIGHT].Add(tv);
			}
			if(Compare(dMaxDist,  dDistThick, _T("<=")) && Compare(dMinDist,  dDistThick, _T("<=")))
			{
				tvArrDiv[iLEFT].Add(tv);
			}
			if(Compare(dDistThick,  dMinDist, _T(">")) && Compare(dDistThick,  dMaxDist, _T("<")))
			{
				CDPoint xyMatch(0,0);
				CTwinVector tvAdd;
				if(GetXyMatchSegAndLine(tv.m_v1, tv.m_v2, CDPoint(dDistThick, 0), vY, xyMatch))
				{
					if(tv.m_v1.x > tv.m_v2.x)
					{
						tvAdd.m_v1 = tv.m_v1;
						tvAdd.m_v2 = xyMatch;
						tvArrDiv[iRIGHT].Add(tvAdd);
						tvAdd.m_v1 = xyMatch;
						tvAdd.m_v2 = tv.m_v2;
						tvArrDiv[iLEFT].Add(tvAdd);
					}
					if(tv.m_v1.x < tv.m_v2.x)
					{
						tvAdd.m_v1 = tv.m_v2;
						tvAdd.m_v2 = xyMatch;
						tvArrDiv[iRIGHT].Add(tvAdd);
						tvAdd.m_v1 = xyMatch;
						tvAdd.m_v2 = tv.m_v1;
						tvArrDiv[iLEFT].Add(tvAdd);
					}
				}
			}
		}
		tvArrDiv[iLEFT].Sort();
		tvArrDiv[iRIGHT].Sort();

		long nLR = nIndexArea;
		if(bStt)
		{
			nLR = nIndexArea == 0 ? iRIGHT : iLEFT;
		}

		tvArrWing.RemoveAll();
		if(tvArrDiv[nLR].GetSize() > 0)
		{
			tvArrWing.AddFromTvArray(tvArrDiv[nLR]);
			CTwinVector tvSK;
			tvSK.m_v1 = tvArrDiv[nLR].GetAt(0).m_v1;
			tvSK.m_v2 = tvArrDiv[nLR].GetAt(tvArrDiv[nLR].GetSize()-1).m_v2;
			tvArrSK.Add(tvSK);
			tvArrWing.Add(tvSK);
		}
	}

	tvArrWing.Sort();
	GetTvVertSection_Std(nLeft, TRUE, TRUE, TRUE, 2);

	return (pWing->m_nAttachPos == 0) ? xyBrk2 : CDPoint(0,0);
}


// BOOL bIncludeOutWall : 날개벽 좌표 구할때 외측벽체 좌표도 같이 구함(수량산출을 위해서)
// BOOL bOnlySolidLine : 모두 실선으로 만듬
// BOOL bForBM : 수량을 위한 영역(날개벽 정면 상단 선중 본체와 겹치는선 제거)
void CRcBridgeApp::GetTvWingWallFront(BOOL bStt, BOOL bLeft, int nOut, CTwinVectorArray &tvArr, BOOL bIncludeOutWall, BOOL bIncludeHunch, BOOL bOnlySolidLine, BOOL bForBM)
{
	CWingWall* pWing = GetWingWall(bStt, bLeft);
	CFootingApp * pFoot = GetFootingByJijumNumber(bStt? 0:GetCountJijum()-1);

	if(!pWing->m_bExist) return;
	if(!pFoot) return;
	
	CTwinVector tv, tvSlope, tvLow;
	CDPoint vX(1, 0), vY(0, 1);
	CDPoint xy1(0, 0), xy2(0, 0), xyTemp(0, 0);

	BOOL bMatchBottom = FALSE;
	BOOL bSide		= (pWing->m_nAttachPos == 0)? FALSE : TRUE;
	BOOL bBox = (m_nTypeBridge == BRIDGE_TYPE_BOX);

	double dL1 = pWing->m_dL1;
	double dL2 = pWing->m_dL2;
	double dHL = pWing->m_dHL;
	double dSW = pWing->m_dSW;
	double dHuH1 = pWing->m_dHuH1;
	double dHuW2 = pWing->m_dHuW2;
	double dElEnd = pWing->GetEndEl(FALSE);
	double se = bStt ? -1.0 : 1.0;
	double dFootEL = IsBoxType() ? (bStt ? m_dELSB : m_dELEB) : pFoot->m_dEL; //박스형일경우기초EL이 아니라 변수로가져옴..

	CDPoint vAng	= GetAngleWingWallBetweenWall(bStt, bLeft);// 수량산출용이라면 구지 날개벽 각도를 벌린상태를 그릴 필요가 없다.
	double dW = bStt ? -m_dWS/vAng.y : m_dWE/vAng.y;
	double dW2 = 0;
	double dT1 = pWing->m_dT1;
	double dT2 = pWing->m_dT2;
	double dSpareT = 0;
	
	if(bSide)
	{
		//double dAngle	= vAng.GetAngleDegree(); 
		//double dAngJijum = GetAngleJijum(bStt? 0 : m_nQtyJigan).GetAngleDegree();
		//double dAngA1 = (dAngle - dAngJijum) * (bLeft? -1 : 1);

		/*
		각도 계산이 이상해서 지움
		아마도 이 코드는 내가 예전에 수정하다가 넣은 코드 같음.(modified by jaeho 2009.06.15)
		if((bStt && bLeft) || (!bStt && !bLeft))
		{
			dAngJijum = 180 - dAngJijum;
			dAngA1	= (dAngle - dAngJijum) * (bLeft ? 1 : -1);
		}
		*/		

		dW = dW2 = 0.0;
//		dW2 = (nOut == 1)? 0.0 : ((dAngA1 == 0)? 0.0 : (dT2)/tan(ToRadian(dAngA1)));		

		//dSpareT = (nOut==0 && pWing->m_bRAngEnd) ? max(dT1, dT2)/tan(ToRadian(dAngA1)) : 0.0; //31447 실제 평면 좌표로 하기 위하여 탄젠트 삭제

		//{ 31447 
		CTwinVectorArray tvArrPlan;
		long nDan = 0;
		double dWingBakcLen = GetWingWallBackAndSideAttach(tvArrPlan, nOut, nDan, bLeft);		

		if(nOut==0)
			dSpareT = (dL1 + dL2 - dWingBakcLen);
		else
			dSpareT = 0;
		//}
	}
	else
	{
		dW2 = dW;
		dSpareT = (nOut==0 && pWing->m_bRAngEnd) ? max(dT1, dT2)/vAng.y*vAng.x : 0.0;
		if(nOut==0 && !pWing->m_bRAngEnd && pWing->m_nDirHunch==2) dL1 -= ((dT2-dT1)/vAng.y*vAng.x);
	}

	double dWidth = (dL1 + dL2 - dSpareT) * se;
	double dElBottom = dFootEL - pFoot->GetHeight();

//	double dLenFoot = (bStt)? pFoot->m_dFWL:pFoot->m_dFWR;
	double dLenFoot = GetLengthFootingWingWall(bStt, bLeft, FALSE, FALSE, nOut) - fabs(dW);
	
	CDPoint xyStt = GetXySectionWingWallStt(bStt, TRUE);
	CDPoint xyEnd = GetXySectionWingWallStt(bStt, FALSE);
	if(bBox)
	{
		double dSta1 = bStt? (m_dStationBridgeStt + m_dWS) : (m_dStationBridgeStt + GetLengthBridge(TRUE) + m_dWE);
		dElBottom = GetXyLowerSlabBottomByOffsetFrStation(dSta1, 0, (bLeft==bStt)? 1 : -1).y;
		dLenFoot = 0;

		if(m_bExistProtectionRiseFooting == FALSE)
		{
			dElBottom += pWing->m_dH2;
			dFootEL = dElBottom;
		}
	}

	xy1 = xyStt;
	xy2 = CDPoint(xy1.x+dWidth, dElEnd);
	tv.m_v1 = xy1; tv.m_v2 = xy2;
	tv.m_sInfo = _T("날개벽정면상단");

	if(bForBM && !bSide)
	{
		CTwinVector tvWall	= m_tvArrVertSection.GetTvByInfo(bStt ? "시점벽체좌측면" : "종점벽체우측면");
		CDPoint xyMatch(0, 0);
		if(GetXyMatchLineAndLine(tvWall.m_v1, tvWall.GetXyDir(), tv.m_v1, tv.GetXyDir(), xyMatch))
			tv.m_v1	= xyMatch;
	}
	tvArr.Add(tv);

	xy1 = xy2;
	xy2 = CDPoint(xy2.x, xy2.y-dHL);
	tv.m_v1 = xy1; tv.m_v2 = xy2;
	tv.m_sInfo = _T("날개벽정면끝단상부");
	tvArr.Add(tv);

	pWing->m_bMatchBottom = FALSE;
	double dLenTemp = (dL2 == 0)? (dL1 - dW*se) : dL1;
	xy1 = xy2;
	xy2 = CDPoint(xy2.x-dLenTemp*se, xy2.y-dLenTemp/dSW);

	CDPoint vAngXy12 = (xy2 - xy1).Unit();
	
	if(pWing->m_bSWHor)
	{

	}
	else if(GetXyMatchSegAndLine(xy1, xy2, CDPoint(xy2.x, dElBottom), vX, xyTemp))
	{
		xy2 = xyTemp;
		bMatchBottom =TRUE;
	}
	else if(dFootEL > xy2.y && nOut == 0 && pWing->m_bRAngEnd == TRUE)
	{
		double d1 = vAng.GetAngleDegree();
		double dTemp = ( Round(d1, 0) >= 180)? dT2 : dT2/tan(vAng.GetAngleRadian()); 
		CDPoint xyCheck(xy2.x + (dTemp)*se, dFootEL);
		BOOL bMat = GetXyMatchLineAndLine(xy1, (xy2 - xy1).Unit(), xyCheck, vY, xyTemp);
		xy2 = xyTemp;
		bMatchBottom =TRUE;
	}
	else if(ComparePt(vAngXy12, vX , _T("="), 0.3) && Compare(xy1.y, dElBottom, _T("="), 0.3))
	{
		double d1 = vAng.GetAngleDegree();
		double dTemp = ( Round(d1, 0) >= 180)? dT2 : dT2/tan(vAng.GetAngleRadian()); 
		CDPoint xyCheck(xy2.x + (dTemp)*se, dFootEL);
		BOOL bMat = GetXyMatchLineAndLine(xy1, (xy2 - xy1).Unit(), xyCheck, vY, xyTemp);
		xy2 = xyTemp;
		bMatchBottom =TRUE;
	}


	if(bBox && m_bExistProtectionRiseFooting==FALSE && bMatchBottom)
	{
		pWing->m_bMatchBottom = TRUE;
	}
	else if(dL2==0)
		GetXyMatchSegAndLine(xy1, xy2, CDPoint(xyEnd.x+dW, xyEnd.y), CDPoint(0, 1), xy2);

	tv.m_v1 = xy1; tv.m_v2 = xy2;
	tv.m_sInfo = _T("날개벽정면노리");
	tvArr.Add(tv);
	tvSlope = tv;
	
	if(bMatchBottom)
	{
		xy1 = xy2;
		xy2 = CDPoint(xyEnd.x+dW+dLenFoot*se, xy2.y);
		tv.m_v1 = xy1; tv.m_v2 = xy2;
		tv.m_sInfo = _T("날개벽꼬리하단");
		tvArr.Add(tv);

		xy1 = xy2;
		xy2 = CDPoint(xy2.x, xyEnd.y);
		tv.m_v1 = xy1; tv.m_v2 = xy2;
		tv.m_sInfo = _T("날개벽정면끝단하부");
		tvArr.Add(tv);
		
		xy1 = CDPoint(xy1.x, xyEnd.y);
		xy2 = CDPoint(xyEnd.x+dW, xyEnd.y);
		tv.m_v1 = xy1; tv.m_v2 = xy2; tv.m_v1.z = 5;
		if(tv.GetLength() > dLenFoot) tv.m_v1.x = tv.m_v2.x + dLenFoot*se;
		tv.m_sInfo = _T("날개벽정면하단");
		tvArr.Add(tv);
		
		//BOX형인 경우 하단헌치가 없다.
		if(!bBox)
		{
			tvLow = tv;
			tvLow.m_sInfo = _T("날개벽정면하단헌치");
		}
	}
	else
	{
		xy1 = xy2;
		tv.m_v1 = xy1; tv.m_v2 = xy2;
		tv.m_sInfo = _T("날개벽꼬리하단");
		tvArr.Add(tv);

		CTwinVector tvPRFSide, tvPRFTop;
		if(bBox)
		{
			CDPoint xyResult(0,0);
			CTwinVectorArray tvArrSect;
			CString strPRFSide = (bStt) ? _T("시점부상방지저판바깥측면") : _T("종점부상방지저판바깥측면");
			CString strPRFTop = (bStt) ? _T("시점부상방지저판상면") : _T("종점부상방지저판상면");

			tvArrSect = m_tvArrVertSection;
			tvArrSect.GetTvByInfo(strPRFSide, tvPRFSide);
			tvArrSect.GetTvByInfo(strPRFTop, tvPRFTop);
			
			//부상방지저판이 있으면 날개벽 하면은 저판 상면위치기준, 아니면 H2입력받은값 기준.
			if(m_bExistProtectionRiseFooting && !IsWingWallOnPRF(bStt, bLeft))
				xyEnd.y = tvPRFSide.m_v2.y;
			else if(m_bExistProtectionRiseFooting && IsWingWallOnPRF(bStt, bLeft))
			{
				GetXyMatchSegAndLine(tvPRFTop.m_v1, tvPRFTop.m_v2, tv.m_v1, CDPoint(0, -1), xyResult);
				xyEnd.y = xyResult.y;
			}
			else
			{
				CTwinVector tvLowerSlab;
				CString stsrLowerSlab = bStt? _T("하부슬래브하면(시점벽체아래)") : _T("하부슬래브하면(종점벽체아래)");
				tvArrSect.GetTvByInfo(stsrLowerSlab, tvLowerSlab);

				GetXyMatchSegAndLine(tvLowerSlab.m_v1, tvLowerSlab.m_v2, tv.m_v2, CDPoint(0, -1), xyResult);
				xyEnd.y = xyResult.y; 

			}
		}

		xy1 = xy2;
		xy2 = CDPoint(xy2.x, xyEnd.y);
		if(bBox && !m_bExistProtectionRiseFooting && pWing->m_dH2 > 0 && pWing->m_dL2 > 0)
		{
			xy2 = CDPoint(xy2.x, xyEnd.y + pWing->m_dH2);
		}
		tv.m_v1 = xy1; tv.m_v2 = xy2;
		tv.m_sInfo = _T("날개벽정면끝단하부");
		tvArr.Add(tv);
	
		xy1 = xy2;
		xy2 = CDPoint(xyEnd.x+dW, xyEnd.y);
		if(bBox && !m_bExistProtectionRiseFooting && pWing->m_dH2 > 0)
		{
			xy2 = CDPoint(xyEnd.x+dW, xyEnd.y + pWing->m_dH2);
		}
		tv.m_v1 = xy1; tv.m_v2 = xy2; tv.m_v1.z = 5;
		if(bBox) tv.m_v1.z = 0;
		if(bBox && m_bExistProtectionRiseFooting)
		{
			if(pWing->m_dL2 > 0)
			{
				//L2가 부상방지저판상면 위에있는경우
				if(pWing->m_dL2 <= m_dPRF_W + dW * se)
					tv.m_v2 = tv.m_v1;
				else
					tv.m_v2.x = tvPRFSide.m_v2.x;
			}
		}
		tv.m_sInfo = _T("날개벽정면하단");
		tvArr.Add(tv);

		//하단까지 했는데 H2가 있다면 날개벽정면하단 끝 아래 벽체를 그려준다.
		if(bBox && !m_bExistProtectionRiseFooting && pWing->m_dH2 > 0 && pWing->m_dL2 > 0)
		{
			xy1 = xy2;
			xy2 = CDPoint(xy1.x, xy1.y - pWing->m_dH2);
			tv.m_v1 = xy1; tv.m_v2 = xy2;
			tv.m_sInfo = _T("날개벽정면하단벽체");
			tvArr.Add(tv);
		}

		//BOX형인 경우 하단헌치가 없다. 밑에서 부상방지판 있을경우에는 여기에 넣어줌.
		if(!bBox)
		{
			tvLow = tv;
			if(tvLow.GetLength() > fabs(dLenFoot)) tvLow.m_v1.x = tvLow.m_v2.x + dLenFoot*se;
			tvLow.m_sInfo = _T("날개벽정면하단헌치");
		}

	}
	
	// 외측벽체 좌표 포함
	if(bIncludeOutWall)
	{
		CTwinVector tvWall, tvSlab;
		CTwinVectorArray tvArrSect;
		tvArrSect = m_tvArrVertSection;

		CDPoint xyBeg(0, 0), xyWall(0, 0), xySlab(0, 0);
		if(bStt)
		{
			tvArrSect.GetTvByInfo(_T("시점벽체좌측면"), tvWall);
			tvArrSect.GetTvByInfo(_T("슬래브상면(시점벽체위)"), tvSlab);
			xyBeg = tvSlab.m_v2;
			xyWall = tvWall.m_v1;
			xySlab = tvSlab.m_v1;
		}
		else
		{
			tvArrSect.GetTvByInfo(_T("종점벽체우측면"), tvWall);
			tvArrSect.GetTvByInfo(_T("슬래브상면(종점벽체위)"), tvSlab);
			xyBeg = tvSlab.m_v1;
			xyWall = tvWall.m_v2;
			xySlab = tvSlab.m_v2;
		}
		double dW	= dW2;
		double dBW	= (pWing->m_bAttachBraket && IsBracket(bStt))? (bStt ? m_dBWS : m_dBWE)/vAng.y : 0;
		double dBTH = (pWing->m_bAttachBraket && IsBracket(bStt))? (bStt ? m_dBTHS : m_dBTHE) : 0;
		double dBHH = (pWing->m_bAttachBraket && IsBracket(bStt))? (bStt ? m_dBHHS : m_dBHHE) : 0;
		double dBH	= (pWing->m_bAttachBraket && IsBracket(bStt))? (bStt ? m_dBHS : m_dBHE) : 0;
		double dBHS = dBH - dBHH;

		CTwinVector tvWallU, tvWallL, tvBrk1, tvBrk2, tvEdge[5];
		CDPoint xyMatch(0, 0);
		BOOL bMatch = GetXyMatchSegAndLine(xyStt, CDPoint(xyStt.x+dWidth, dElEnd), CDPoint(xyBeg.x+dW, xySlab.y), vY, xyMatch);
		tv.m_v1 = bMatch ? xyMatch : xyStt; tv.m_v2 = tv.m_v1 - dBTH*vY; tv.m_v1.z = 5;
		tv.m_sInfo	= "벽체";
		tvArr.Add(tv);
		tvWallU = tv;
		tvWallU.m_sInfo	= "벽체내측";
		tv.m_v1 = tv.m_v2;	tv.m_v2 = tv.m_v1 + dBW*vX*se;	tv.m_v1.z = 5;
		tv.m_sInfo	= "벽체브라켓상면";
		tvArr.Add(tv);
		tv.m_v1 = tv.m_v2;	tv.m_v2 = tv.m_v1 - dBHS*vY;	tv.m_v1.z = 5;
		tv.m_sInfo	= "벽체";
		tvArr.Add(tv);
		tvBrk1 = tv;
		tvBrk1.m_sInfo	= "벽체내측";
		tv.m_v1 = tv.m_v2;	tv.m_v2 = tv.m_v1 - dBW*vX*se - dBHH*vY;	tv.m_v1.z = 5;
		tv.m_sInfo	= "벽체";
		tvArr.Add(tv);
		tvBrk2 = tv;
		tvBrk2.m_sInfo	= "벽체내측";
		tv.m_v1 = tv.m_v2;
		if(pWing->m_bMatchBottom) tv.m_v2 = CDPoint(tv.m_v1.x, xyWall.y + pWing->m_dH2); //Box형에서 부상방지판이 없을때는 H2가 적용된다.
		else if(dL2 == 0) GetXyMatchSegAndLine(tvSlope.m_v1, tvSlope.m_v2, tv.m_v1, vY, tv.m_v2);
		else tv.m_v2 = CDPoint(tv.m_v1.x, xyWall.y);
		
		tv.m_v1.z = 5;
		tv.m_sInfo	= "벽체";
		tvArr.Add(tv);
		tvWallL = tv;
		tvWallL.m_sInfo	= "벽체내측";
	
		//Box 일경우 tvLow에다가 부상방지판 헌치 넣어주자.
		CString strPRFSide = bStt? _T("시점부상방지저판바깥측면") : _T("종점부상방지저판바깥측면");
		CString strPRFTop = bStt? _T("시점부상방지저판상면") : _T("종점부상방지저판상면");
		CTwinVector tvPRFSide, tvPRFTop;
		tvArrSect.GetTvByInfo(strPRFSide, tvPRFSide);
		tvArrSect.GetTvByInfo(strPRFTop, tvPRFTop);
		if(bBox && m_bExistProtectionRiseFooting)
		{
			if(pWing->m_dL2 > 0)
			{
				//L2가 있는데 부상방지판 위에 있는경우.
				//이경우에 L2하고 부상방지판상면하고 만나는점이 헌치 하단 시작점이다.
				if(pWing->m_dL2 <= m_dPRF_W + dW * se) 
				{
					CDPoint tvWall;
					tvWall = xyStt;
					tvWall.x += pWing->m_dL2 * se;
					if(GetXyMatchSegAndLine(tvPRFTop.m_v1, tvPRFTop.m_v2, tvWall, CDPoint(0,-1), xyMatch))
						tv.m_v1 = xyMatch;
				}
				else
					tv.m_v1 = tvPRFSide.m_v2;

				tvLow = tv;
			}
		}

		if(!bForBM && tvLow.GetLength() > 0)
		{
			tv = tvLow;				// 날개벽정면하단헌치
			tvArr.Add(tv);
		}

		if(bIncludeHunch)
		{
			CDPoint xyMatch(0, 0);
			double dSpaceH = dHuH1/3.0;
			double dSpaceW = dHuW2/3.0;
			tvEdge[0].m_v1 = tvLow.m_v2;
			tvEdge[1].m_v1 = tvWallL.m_v1;
			tvEdge[2].m_v1 = tvBrk2.m_v1;
			tvEdge[3].m_v1 = tvBrk1.m_v1;
			tvEdge[4].m_v1 = tvLow.m_v1;
			int i=0; for(i=0; i<3; i++)
			{
				Offset(tvLow.m_v1, tvLow.m_v2, dSpaceH*se);
				Offset(tvWallL.m_v1, tvWallL.m_v2, -dSpaceW*se);
				Offset(tvWallU.m_v1, tvWallU.m_v2, -dSpaceW*se);
				Offset(tvBrk1.m_v1, tvBrk1.m_v2, -dSpaceW*se);
				Offset(tvBrk2.m_v1, tvBrk2.m_v2, -dSpaceW*se);

				BOOL bMatch	= FALSE;
				if(dL2==0)
					bMatch	= GetXyMatchSegAndSeg(tvSlope.m_v1, tvSlope.m_v2, tvWallL.m_v1, tvWallL.m_v2, xyMatch);
				else
					bMatch	= GetXyMatchSegAndSeg(tvLow.m_v1, tvLow.m_v2, tvWallL.m_v1, tvWallL.m_v2, xyMatch);
				if(bMatch)
					tvLow.m_v2 = tvWallL.m_v2 = xyMatch;

				if(bBox && m_bExistProtectionRiseFooting)
				{
					CString strPRFSide = bStt ? _T("시점부상방지저판바깥측면") : _T("종점부상방지저판바깥측면");
					CTwinVector tvPRFSide;
					tvArrSect.GetTvByInfo(strPRFSide, tvPRFSide);
	
					//좌측벽체가 부상방지저판 상면에 있을경우
					if(pWing->m_dL2 <= m_dPRF_W + dW * se)
					{
						CDPoint tvWall;
						tvWall = xyStt;
						tvWall.x += pWing->m_dL2 * se;
						if(GetXyMatchSegAndLine(tvLow.m_v1, tvLow.m_v2, tvWall, CDPoint(0,-1), xyMatch))
							tvLow.m_v1 = xyMatch;
					}
					else
					{
						if(GetXyMatchSegAndLine(tvLow.m_v1, tvLow.m_v2, tvPRFSide.m_v1, CDPoint(0,1), xyMatch))
							tvLow.m_v1 = xyMatch;
					}
				}
				if(GetXyMatchSegAndLine(tvBrk2.m_v1, tvBrk2.m_v2, tvWallL.m_v1, vY, xyMatch))
					tvWallL.m_v1 = tvBrk2.m_v2 = xyMatch;
				GetXyMatchSegAndLine(tvBrk2.m_v1, tvBrk2.m_v2, tvBrk1.m_v1, vY, xyMatch);
				tvBrk1.m_v2 = tvBrk2.m_v1 = xyMatch;

				if(GetXyMatchSegAndLine(xyStt, CDPoint(xyStt.x+dWidth, dElEnd), tvWallU.m_v2, vY, xyMatch))
						tvWallU.m_v1	= xyMatch;

				tvWallU.m_v1.z = tvWallL.m_v1.z = tvLow.m_v1.z = 5;
				tvEdge[0].m_v2 = tvLow.m_v2;
				tvEdge[1].m_v2 = tvWallL.m_v1;
				tvEdge[2].m_v2 = tvBrk2.m_v1;
				tvEdge[3].m_v2 = tvBrk1.m_v1;
				tvEdge[4].m_v2 = tvLow.m_v1;

				tvArr.Add(tvWallU);
				tvArr.Add(tvBrk1);
				tvArr.Add(tvBrk2);
				tvArr.Add(tvWallL);

				if(dL2!=0) 
					tvArr.Add(tvLow);
			}

			// 브라켓헌치와 기초상단의 모서리 선
			for(i=0; i<5; i++)
			{
				if(i==0 && dL2==0) continue;
				if(bBox && i==4 && dL2==0) continue;

				tvEdge[i].m_v1.z = 5;
				tvArr.Add(tvEdge[i]);
			}

			double dLT = pWing->m_dLT;
			double dSH = pWing->m_dSH;
			// 세로 방향 변단면 헌치
			if(pWing->m_nDirHunch==1)
			{
				CTwinVectorArray tvArrHuStt, tvArrHuEnd;
				tvArrHuStt.Add(tvWallU);
				tvArrHuStt.Add(tvBrk1);
				tvArrHuStt.Add(tvBrk2);
				tvArrHuStt.Add(tvWallL);
				tvArr.GetTvByInfo(_T("날개벽정면끝단상부"), tv);
				tvArrHuEnd.Add(tv);
				tvArr.GetTvByInfo(_T("날개벽정면노리"), tv);
				tvArrHuEnd.Add(tv);
				tvArr.GetTvByInfo(_T("날개벽정면끝단하부"), tv);
				tvArrHuEnd.Add(tv);

				CDPoint xyHuStt1(xyStt.x, xyStt.y-dLT);
				CDPoint xyHuStt2(xyHuStt1.x, xyHuStt1.y-(dT2-dT1)*dSH);
				for(i=0; i<tvArrHuStt.GetSize(); i++)
				{
					tv = tvArrHuStt.GetAt(i);
					if(GetXyMatchSegAndLine(tv.m_v1, tv.m_v2, xyHuStt1, vX, xyHuStt1))
						break;
				}
				for(i=0; i<tvArrHuStt.GetSize(); i++)
				{
					tv = tvArrHuStt.GetAt(i);
					if(GetXyMatchSegAndLine(tv.m_v1, tv.m_v2, xyHuStt2, vX, xyHuStt2))
						break;
				}

				CDPoint xyHuEnd1(0, 0), xyHuEnd2(0, 0);
				for(i=0; i<tvArrHuEnd.GetSize(); i++)
				{
					tv = tvArrHuEnd.GetAt(i);
					if(GetXyMatchSegAndLine(tv.m_v1, tv.m_v2, xyHuStt1, vX, xyHuEnd1))
						break;
				}
				for(i=0; i<tvArrHuEnd.GetSize(); i++)
				{
					tv = tvArrHuEnd.GetAt(i);
					if(GetXyMatchSegAndLine(tv.m_v1, tv.m_v2, xyHuStt2, vX, xyHuEnd2))
						break;
				}

				tv.m_v1 = xyHuStt1;
				tv.m_v2 = xyHuEnd1;
				tv.m_v1.z = 5;
				tv.m_sInfo = _T("세로변단면헌치1");
				tvArr.Add(tv);
				tv.m_v1 = xyHuStt2;
				tv.m_v2 = xyHuEnd2;
				tv.m_v1.z = 5;
				tv.m_sInfo = _T("세로변단면헌치2");
				tvArr.Add(tv);
			}

			// 가로 방향 변단면 헌치
			if(pWing->m_nDirHunch==2)
			{
				// m_dLT는 세로방향 변단면 일때도 쓰이므로 여기서 바꿔준다.
				long iDir = (vAng.GetAngleDegree() > 90)? -1 :1;
				if(nOut==0 && !pWing->m_bRAngEnd) dLT += fabs((dT1)/vAng.y*vAng.x) * iDir;

				CTwinVector tvUpper, tvNori;
				tvArr.GetTvByInfo(_T("날개벽정면상단"), tvUpper);
				tvArr.GetTvByInfo(_T("날개벽정면노리"), tvNori);

				CDPoint xyHuStt1(tvUpper.m_v2.x-dLT*se, tvUpper.m_v2.y);
				CDPoint xyHuStt2(xyHuStt1.x-(dT2-dT1)*dSH*se, xyHuStt1.y);
				GetXyMatchSegAndLine(tvUpper.m_v1, tvUpper.m_v2, xyHuStt1, vY, xyHuStt1);
				GetXyMatchSegAndLine(tvUpper.m_v1, tvUpper.m_v2, xyHuStt2, vY, xyHuStt2);

				CDPoint xyHuEnd1(0, 0), xyHuEnd2(0, 0);
				GetXyMatchSegAndLine(tvNori.m_v1, tvNori.m_v2, xyHuStt1, vY, xyHuEnd1);
				GetXyMatchSegAndLine(tvNori.m_v1, tvNori.m_v2, xyHuStt2, vY, xyHuEnd2);

				tv.m_v1 = xyHuStt1;
				tv.m_v2 = xyHuEnd1;
				tv.m_v1.z = 5;
				tv.m_sInfo = _T("가로변단면헌치1");
				tvArr.Add(tv);
				tv.m_v1 = xyHuStt2;
				tv.m_v2 = xyHuEnd2;
				tv.m_v1.z = 5;
				tv.m_sInfo = _T("가로변단면헌치2");
				tvArr.Add(tv);
			}
		}
	}

	// 모두 실선으로 만들어야 되는 경우
	if(bOnlySolidLine)
	{
		long nSize	= tvArr.GetSize();
		long i = 0; for(i = 0; i < nSize; i++)
		{
			tv	= tvArr.GetAt(i);
			tv.m_v1.z	= 0;
			tv.m_v2.z	= 0;
			tvArr.SetAt(i, tv);
		}
	}

	// 날개벽정면상단과 날개벽정면하단은 외측벽체 선과 교차점을 찾아서 정확하게 일치시켜 줘야 된다.
	if(bForBM)
	{
		CRebarPlacing rb;
		rb.RemoveMinLengthLine(tvArr, 0.00001);
		CTwinVector tvWall	= tvArr.GetTvByInfo("벽체");
		CDPoint xyMatch(0, 0);
		long nIdx	= tvArr.GetIdxByInfo("날개벽정면상단");
		if(nIdx > -1)
		{
			tv	= tvArr.GetAt(nIdx);
			if(GetXyMatchLineAndLine(tv.m_v1, tv.GetXyDir(), tvWall.m_v1, tvWall.GetXyDir(), xyMatch))
			{
				tv.m_v1	= xyMatch;
				tvArr.SetAt(nIdx, tv);
			}
		}

		nIdx	= tvArr.GetIdxByInfo("날개벽정면하단");
		if(nIdx > -1)
		{
			tv	= tvArr.GetAt(nIdx);
			if(GetXyMatchLineAndLine(tv.m_v1, tv.GetXyDir(), tvWall.m_v1, tvWall.GetXyDir(), xyMatch))
			{
				tv.m_v2	= xyMatch;
				tvArr.SetAt(nIdx, tv);
			}
		}

		if(pWing->m_dL2 == 0)
		{
			nIdx	= tvArr.GetIdxByInfo("날개벽정면끝단하부");
			if(nIdx > -1)
			{
				tvArr.RemoveAt(nIdx);
			}

			nIdx	= tvArr.GetIdxByInfo("날개벽정면노리");
			long nIdx_Tail = tvArr.GetIdxByInfo("날개벽꼬리하단");

			long nIdxSel = nIdx_Tail > -1? nIdx_Tail : nIdx;

			if(nIdxSel > -1 )
			{
				tv	= tvArr.GetAt(nIdxSel);

				if(GetXyMatchLineAndLine(tv.m_v1, tv.GetXyDir(), tvWall.m_v1, tvWall.GetXyDir(), xyMatch))
				{
					tv.m_v2	= xyMatch;
					tvArr.SetAt(nIdxSel, tv);
				}
			}
		}
	}
}

void CRcBridgeApp::GetTvWingWallSection(BOOL bStt, BOOL bLeft, CTwinVectorArray &tvArr, double dSpare)
{
	CWingWall* pWing = GetWingWall(bStt, bLeft);

	if(!pWing->m_bExist) return;

	CTwinVector tv, tvFront;
	CDPoint xy1(0, 0), xy2(0, 0);
	CDPoint xyCut1(0, 0), xyCut2(0, 0);

	CTwinVectorArray tvArrGen;
	GetTvWingWallFront(bStt, bLeft, 1, tvArrGen);
	tvArrGen.GetTvByInfo(_T("날개벽정면끝단하부"), tvFront);
	double dL2 = pWing->m_dL2;
	double dT1 = pWing->m_dT1;
	double dT2 = pWing->m_dT2;
	double dLT = pWing->m_dLT;
	if(pWing->m_nDirHunch==2)
	{
		dT1 = dT2;
		dLT = 0.0;
	}
	double dDT = dT2-dT1;
	double dSH = pWing->m_dSH;
	double dDS = fabs(dDT*dSH);
	double dHuW = pWing->m_dHuW1;
	double dHuH = pWing->m_dHuH1;
	double lr = bLeft ? -1.0 : 1.0;

	double dHeight = GetHeightWingWall(bStt);
	double dCutLen = dT2 + dHuW + dSpare;
	double dFH = bStt ? (m_footingStt.m_dFH + m_footingStt.m_dFSRH) : (m_footingEnd.m_dFH + m_footingEnd.m_dFSLH);
	if(IsBoxType()) 
		dFH =  m_footingBox.m_dFSRH;

	CDPoint xyStt = GetXySectionWingWallStt(bStt, TRUE);
	CDPoint xyEnd = GetXySectionWingWallStt(bStt, FALSE);

	// 외측선
	xy1 = xyStt;
	xy2 = CDPoint(xy1.x, xyEnd.y-dFH);
	tv.m_v1 = xy1; tv.m_v2 = xy2;
	tv.m_sInfo = _T("날개벽단면외측");
	tvArr.Add(tv);

	xy1 = xy2;
	xy2 = CDPoint(xy1.x-dCutLen*lr, xy1.y);
	tv.m_v1 = xy1; tv.m_v2 = xy2;
	tv.m_sInfo = _T("날개벽단면하단");
	tvArr.Add(tv);

	// 내측선
	xy1 = xyStt;
	xy2 = CDPoint(xy1.x-dT1*lr, xy1.y);
	tv.m_v1 = xy1; tv.m_v2 = xy2;
	tv.m_sInfo = _T("날개벽단면상단");
	tvArr.Add(tv);

	xy1 = xy2;
	xy2 = CDPoint(xy1.x, xy1.y-dLT);
	tv.m_v1 = xy1; tv.m_v2 = xy2;
	tv.m_sInfo = _T("날개벽단면내측헌치전");
	tvArr.Add(tv);

	xy1 = xy2;
	xy2 = CDPoint(xy1.x-dDT*lr, xy1.y-dDS);
	tv.m_v1 = xy1; tv.m_v2 = xy2;
	tv.m_sInfo = _T("날개벽단면내측헌치");
	tvArr.Add(tv);

	xy1 = xy2;

	//박스형일때 부상방지가 있고 L2있으면  단면 날개벽은 방지판 상면위까지 
	if(m_nTypeBridge == BRIDGE_TYPE_BOX && m_bExistProtectionRiseFooting && pWing->m_dL2 > 0)
		xy2 = CDPoint(xy1.x, xy1.y-(dHeight-(dLT+dDS+dHuH)) + (m_dPRF_H1 + m_dPRF_H2));
	//날개벽에 H2가 있으면 헌치도 존재x , 부상방지판은 H2설치할수 없으니 당연히 제외.
	else if(m_nTypeBridge == BRIDGE_TYPE_BOX && pWing->m_dH2 > 0 && !m_bExistProtectionRiseFooting)
		xy2 = CDPoint(xy1.x, xy1.y-(dHeight-(dLT+dDS)) + pWing->m_dH2);
	else
		xy2 = CDPoint(xy1.x, xy1.y-(dHeight-(dLT+dDS+dHuH)));
	
	if(dL2==0)
		xy2 = CDPoint(xy1.x, tvFront.m_v1.y);
	tv.m_v1 = xy1; tv.m_v2 = xy2;
	tv.m_sInfo = _T("날개벽단면내측헌치후");
	tvArr.Add(tv);

	if(dL2==0)
	{
		xy1 = CDPoint(xy2.x+dT2*lr, xy2.y);
		xy2 = xy2;
		tv.m_v1 = xy1; tv.m_v2 = xy2;
		tv.m_sInfo = _T("날개벽단면노리끝");
		tvArr.Add(tv);
	}

	if(m_nTypeBridge == BRIDGE_TYPE_BOX && pWing->m_dH2 > 0 && !m_bExistProtectionRiseFooting)
	{
		xy1 = xy2;
		xy2 = CDPoint(xy1.x+dT1*lr, xy1.y);
		tv.m_v1 = xy1; tv.m_v2 = xy2;
		tv.m_sInfo = _T("날개벽단면기초");
		tvArr.Add(tv);
	}
	else
	{
		xy1 = xy2;
		xy2 = CDPoint(xy1.x-dHuW*lr, xy1.y-dHuH);
		if(dL2==0)
			xy1 = xy2 = CDPoint(xy1.x+dT2*lr, xyEnd.y);
		tv.m_v1 = xy1; tv.m_v2 = xy2;
		tv.m_sInfo = _T("날개벽단면기초헌치");
		tvArr.Add(tv);

		xy1 = xy2;
		xy2 = CDPoint(xyStt.x-dCutLen*lr, xy1.y);
		tv.m_v1 = xy1; tv.m_v2 = xy2;
		tv.m_sInfo = _T("날개벽단면기초");
		tvArr.Add(tv);
	}

	//이경우는 부상방지저판이 날개벽하고 떨어져있기때문에 따로그려준다.
	if(m_nTypeBridge == BRIDGE_TYPE_BOX && m_bExistProtectionRiseFooting && pWing->m_dL2 <= 0)
	{
		CDPoint xySttPRF = xyEnd + (m_dPRF_H1 + m_dPRF_H2) * CDPoint(0,1);
		tv.m_v1 = xySttPRF;
		tv.m_v2 = xySttPRF - dCutLen * CDPoint(1,0) * lr;
		tvArr.Add(tv);
	}
	
}

// A구간과 D구간의 경계선
void CRcBridgeApp::GetTvWingWallBoundAD(BOOL bStt, BOOL bLeft, CTwinVector &tv, BOOL bUpper)
{
	CWingWall* pWing = GetWingWall(bStt, bLeft);
	CLineInfo* pLine = GetLineLeftRight(bStt==bLeft);

	double dW = bStt ? m_dWS : m_dWE;		// 벽체 두께
	double dBW = bStt ? m_dBWS : m_dBWE;	// 브라켓 폭
	double dL1 = pWing->m_dL1;
	CDPoint vAng = GetAngleAzimuthWingWall(bStt, bLeft);
	double dT1 = pWing->m_dT1;
	double dT2 = pWing->m_dT2;
	BOOL bRAng = pWing->m_bRAngEnd;
	int nDirHunch = dT1==dT2 ? 1 : pWing->m_nDirHunch;
	int se	= (bStt)? -1 : 1;

	double dT = dT1;
	if(nDirHunch==1)
		dT = bUpper ? dT1 : dT2;
	else if(nDirHunch==2)
		dT = dT2;

	CDPoint A[4];
	if(bStt)
		GetXySss(BRIDGE_OFF_STT_STT, A);
	else
		GetXySss(BRIDGE_OFF_END_END, A);

	int nCountJijum = GetCountJijum();
	int nJ = bStt ? 0 : nCountJijum-1;
	int nWingPos = pWing->m_nAttachPos;
	CDPoint xyOutStt = bLeft ? (bStt ? A[3] : A[0]) : (bStt ? A[0] : A[3]);
	CDPoint xyOutEnd = GetXyPlaneWingWallEnd(bStt, bLeft);
	CDPoint vAngJijum = GetAngleAzimuthJijum(nJ);				// 슬래브 끝단 각도
	double dSta = bStt ? m_dStationBridgeStt : m_dStationBridgeStt+m_dLengthBridge;
	double dStaBrk = dSta + (dW+dBW)*se/vAng.y;
	CDPoint vAngWing = (xyOutEnd-xyOutStt).Unit();				// 좌측 날개벽 각도
	CDPoint vLine = pLine->GetAngleAzimuth(dStaBrk);
	CDPoint	vAngEnd = bRAng ? vAngWing.Rotate90() : (nWingPos==0)? vAngJijum:vLine;	// 좌측 날개벽 단부 각도

	double lr = bLeft ? -1.0 : 1.0;
	CDPoint xyInEnd(0, 0);
	CDPoint xyTemp = xyOutEnd;
	Offset(xyTemp, xyOutStt, dT*lr);
	GetXyMatchLineAndLine(xyTemp, vAngWing, xyOutEnd, vAngEnd, xyInEnd);

	tv.m_v1 = xyOutEnd - dL1*vAng;
	tv.m_v2 = xyInEnd - dL1*vAng;
}

// 날개벽의 방위각
CDPoint CRcBridgeApp::GetAngleAzimuthWingWall(BOOL bStt, BOOL bLeft)
{
	CDPoint A[4];
	if(bStt)
		GetXySss(BRIDGE_OFF_STT_STT, A);
	else
		GetXySss(BRIDGE_OFF_END_END, A);

	// 외측 점 좌표
	CDPoint xyOutSttL = bStt ? A[3] : A[0];
	CDPoint xyOutEndL = GetXyPlaneWingWallEnd(bStt, TRUE);
	CDPoint xyOutSttR = bStt ? A[0] : A[3];
	CDPoint xyOutEndR = GetXyPlaneWingWallEnd(bStt, FALSE);

	CDPoint vAng = bLeft ? (xyOutEndL-xyOutSttL).Unit() : (xyOutEndR-xyOutSttR).Unit();

	return vAng;
}

// 기초 직각 처리했을 때의 끝단 각도
CDPoint CRcBridgeApp::GetAngleRectFootingEnd(long nJ, BOOL bLeft)
{
	CFootingApp* pFooting = GetFootingByJijumNumber(nJ);
	CDPoint vAng(0, 0);

	if(pFooting)
	{
		CDPoint vAngTan = GetAngleAzimuthJijum(nJ);
		vAng = vAngTan.Rotate90();
	}

	return vAng;
}

// 기초 직각 처리했을 때의 끝단 여분 길이
double CRcBridgeApp::GetLengthFootingMargin(long nJ, BOOL bLeft)
{
	double dLength = 0.0;
	// 기초에 벽체가 두개 붙을때..
	BOOL bIsTwoWall = FALSE;
	long nIdxAddWall = 0;
	double dLenOld = 0;

	CTwinVector tvFooting, tvWing, tvWingAD, tvWall, tvCenter;
	CTwinVectorArray tvArrFooting, tvArrWing, tvArrWall;

	CDPoint vAngFooting(0, 0), vAngWing(0, 0), vAngWall(0, 0);
	CDPoint vAngJijum(0, 0), vAngBet(0, 0);
	CDPoint xyWingAD(0, 0), xyResult(0, 0);
	CString strWing = _T(""), strFooting = _T("");

	CFootingApp* pFooting = GetFootingByJijumNumber(nJ);
	if(nJ==0)
	{
		if(pFooting)
		{
			pFooting->GetTvPlan();
			tvArrFooting = pFooting->m_tvArrplan;

			// 기초끝단 각도
			strFooting = bLeft ? _T("좌측1") : _T("우측1");
			tvArrFooting.GetTvByInfo(_T("기초평면")+strFooting, tvFooting);
			vAngFooting = bLeft ? tvFooting.GetXyDir(FALSE) : tvFooting.GetXyDir();

			tvArrFooting.GetTvByInfo(_T("기초평면후면"), tvFooting);

			CWingWall* pWing = GetWingWall(TRUE, !bLeft);	// bLeft는 선형기준이므로 시점측에서는 반대로 해줘야.. 
			GetTvWingWallPlane(TRUE);
			tvArrWing = m_tvArrPlaneWingWall;

			// (ARCBRIDGE-3054) 날개벽 BC구간이 있을때 날개벽 하단에 기초를 맞추기 위해서다. L2가 0이라면 맞추지 않는다.
			if(pWing->m_bExist && pWing->m_dL2 > 0)
			{
				strWing = bLeft ? _T("우측") : _T("좌측");
				tvArrWing.GetTvByInfo(strWing+_T("날개벽외측"), tvWing);
				vAngWing = bLeft ? tvWing.GetXyDir() : tvWing.GetXyDir(FALSE);
				vAngBet = vAngWing.BetweenAngle(vAngFooting);

				if(vAngBet.y>0.0)
				{
					vAngJijum = GetAngleAzimuthJijum(nJ);
					GetTvWingWallBoundAD(TRUE, !bLeft, tvWingAD);
					GetXyMatchLineAndLine(tvWingAD.m_v1, vAngFooting, tvWing.m_v1, vAngJijum, xyResult);
					dLength = ~(tvWing.m_v1-xyResult);
				}
				else
					dLength = 0.0;

				if(IsGagakForWing(nJ==0, !bLeft))
				{
					CDPoint AIncGagak[4], AOrg[4];
					GetXySss(BRIDGE_OFF_STT_STT, AIncGagak, TRUE);
					GetXySss(BRIDGE_OFF_STT_STT, AOrg, FALSE);
					double dOverLen = bLeft? ~(AIncGagak[0] - AOrg[0]) : ~(AIncGagak[3] - AOrg[3]);
					dLength += dOverLen;
				}
			}
			else
			{
				tvArrWing.GetTvByInfo(_T("슬래브끝단외측"), tvWing);
				CDPoint xy1 = bLeft ? tvWing.m_v2 : tvWing.m_v1;
				tvArrWing.GetTvByInfo(_T("슬래브끝단내측"), tvWing);
				CDPoint xy2 = bLeft ? tvWing.m_v2 : tvWing.m_v1;

				vAngWing = (xy2-xy1).Unit();
				vAngBet = vAngWing.BetweenAngle(vAngFooting);

				if(vAngBet.y>0.0)
				{
					vAngJijum = GetAngleAzimuthJijum(nJ);
					GetXyMatchLineAndLine(xy2, vAngFooting, xy1, vAngJijum, xyResult);
					dLength = ~(xy1-xyResult);
				}
				else
					dLength = 0.0;

				if(IsGagakForWing(nJ==0, !bLeft))
				{			
					CDPoint AIncGagak[4], AOrg[4];
					GetXySss(BRIDGE_OFF_STT_STT, AIncGagak, TRUE);
					GetXySss(BRIDGE_OFF_STT_STT, AOrg, FALSE);
					double dOverLen = bLeft? ~(AIncGagak[0] - AOrg[0]) : ~(AIncGagak[3] - AOrg[3]);

					dLength += dOverLen;
				}
			}

			bIsTwoWall = (m_nTypeBridge == BRIDGE_TYPE_TRAP2 || m_nTypeBridge == BRIDGE_TYPE_ARCH)? TRUE : FALSE;
			if(bIsTwoWall )
			{
				tvArrWing.GetTvByInfo(_T("슬래브끝단외측"), tvWing);
				tvCenter = tvWing;				// 경사벽체를 고려할때 기준선을 유지하기 위해.
				tvCenter.m_v1 = (bLeft)? tvWing.m_v1 : tvWing.m_v2;
				tvCenter.m_v2 = (bLeft)? tvWing.m_v2 : tvWing.m_v1;
			}
			nIdxAddWall = nJ + 1;
			dLenOld = dLength;
		}
	}
	else if(nJ==m_nQtyJigan)
	{
		if(pFooting)
		{
			pFooting->GetTvPlan();
			tvArrFooting = pFooting->m_tvArrplan;

			// 기초끝단 각도
			strFooting = bLeft ? _T("좌측1") : _T("우측1");
			tvArrFooting.GetTvByInfo(_T("기초평면")+strFooting, tvFooting);
			vAngFooting = bLeft ? tvFooting.GetXyDir(FALSE) : tvFooting.GetXyDir();

			tvArrFooting.GetTvByInfo(_T("기초평면전면"), tvFooting);

			CWingWall* pWing = GetWingWall(FALSE, bLeft);
			GetTvWingWallPlane(FALSE);
			tvArrWing = m_tvArrPlaneWingWall;
			
			// (ARCBRIDGE-3054) 날개벽 BC구간이 있을때 날개벽 하단에 기초를 맞추기 위해서다. L2가 0이라면 맞추지 않는다.
			if(pWing->m_bExist && pWing->m_dL2 > 0)
			{
				strWing = bLeft ? _T("좌측") : _T("우측");
				tvArrWing.GetTvByInfo(strWing+_T("날개벽외측"), tvWing);
				vAngWing = tvWing.GetXyDir();
				vAngBet = vAngWing.BetweenAngle(vAngFooting);

				if(vAngBet.y>0.0)
				{
					vAngJijum = GetAngleAzimuthJijum(nJ);
					GetTvWingWallBoundAD(FALSE, bLeft, tvWingAD);

					GetXyMatchLineAndLine(tvWingAD.m_v1, vAngFooting, tvWing.m_v1, vAngJijum, xyResult);
					dLength = ~(tvWing.m_v1-xyResult);
				}
				else
					dLength = 0.0;

				if(IsGagakForWing(nJ==0, bLeft))
				{
					CDPoint AIncGagak[4], AOrg[4];
					GetXySss(BRIDGE_OFF_END_END, AIncGagak, TRUE);
					GetXySss(BRIDGE_OFF_END_END, AOrg, FALSE);
					double dOverLen = bLeft? ~(AIncGagak[0] - AOrg[0]) : ~(AIncGagak[3] - AOrg[3]);

					dLength += dOverLen;
				}
			}
			else
			{
				tvArrWing.GetTvByInfo(_T("슬래브끝단외측"), tvWing);
				CDPoint xy1 = bLeft ? tvWing.m_v1 : tvWing.m_v2;
				tvArrWing.GetTvByInfo(_T("슬래브끝단내측"), tvWing);
				CDPoint xy2 = bLeft ? tvWing.m_v1 : tvWing.m_v2;
				vAngWing = (xy2-xy1).Unit();
				vAngBet = vAngWing.BetweenAngle(vAngFooting);

				if(vAngBet.y>0.0)
				{
					vAngJijum = GetAngleAzimuthJijum(nJ);
					GetXyMatchLineAndLine(xy2, vAngFooting, xy1, vAngJijum, xyResult);
					dLength = ~(xy1-xyResult);
				}
				else
					dLength = 0.0;

				if(IsGagakForWing(nJ==0, bLeft))
				{
					CDPoint AIncGagak[4], AOrg[4];
					GetXySss(BRIDGE_OFF_END_END, AIncGagak, TRUE);
					GetXySss(BRIDGE_OFF_END_END, AOrg, FALSE);
					double dOverLen = bLeft? ~(AIncGagak[0] - AOrg[0]) : ~(AIncGagak[3] - AOrg[3]);
					
					dLength += dOverLen;
				}
			}
		}
		bIsTwoWall = (m_nTypeBridge == BRIDGE_TYPE_TRAP2 || m_nTypeBridge == BRIDGE_TYPE_ARCH)? TRUE : FALSE;
		if(bIsTwoWall)
		{
			tvArrWing.GetTvByInfo(_T("슬래브끝단외측"), tvWing);
			tvCenter = tvWing;				// 경사벽체를 고려할때 기준선을 유지하기 위해.
			tvCenter.m_v1 = (bLeft)? tvWing.m_v1 : tvWing.m_v2;
			tvCenter.m_v2 = (bLeft)? tvWing.m_v2 : tvWing.m_v1;
		}
		nIdxAddWall = nJ - 1;
		dLenOld = dLength;
	}
	
	if(!(nJ==0 || nJ==m_nQtyJigan) || bIsTwoWall)
	{
		if(pFooting)
		{
			pFooting->GetTvPlan();
			tvArrFooting = pFooting->m_tvArrplan;

			// 기초끝단 각도
			strFooting = bLeft ? _T("좌측1") : _T("우측1");
			tvArrFooting.GetTvByInfo(_T("기초평면")+strFooting, tvFooting);
			vAngFooting = bLeft ? tvFooting.GetXyDir() : tvFooting.GetXyDir(FALSE);

			int nW = (!bIsTwoWall)? nJ-1 : nIdxAddWall-1;
			CWallApp* pWall = GetInWall(nW);

			if(pWall)
			{
				pWall->GetTvPlan();
				tvArrWall = pWall->m_tvArrPlan;

				BOOL bIsVertWall = pWall->IsVertWall();
				CString strPos = (bIsVertWall)? _T("벽체") : _T("벽체하면");
				
				// 경사교각 에대한 추가 고려라면 중심선은 외측벽체에 그대로 있어야 한다.
				if(!bIsTwoWall) tvCenter = pWall->GetTvWallTopCen(bIsVertWall);
				CDPoint xyCenter = bLeft ? tvCenter.m_v2 : tvCenter.m_v1;

				tvArrWall.GetTvByInfo(strPos + "평면우측", tvWall);
				CDPoint xy1 = bLeft ? tvWall.m_v2 : tvWall.m_v1;
				tvArrWall.GetTvByInfo(strPos + "평면좌측", tvWall);
		
				CDPoint xy2 = bLeft ? tvWall.m_v1 : tvWall.m_v2;
				vAngWall = (xy2-xy1).Unit();
				vAngBet = vAngWall.BetweenAngle(vAngFooting);

				BOOL bStt = (bIsTwoWall)? (nJ == 0) : FALSE;
				vAngJijum = GetAngleAzimuthJijum(nJ);
				if((vAngBet.x>0.0 && !bIsTwoWall) || (((vAngBet.x>0.0 && bLeft) || (vAngBet.x>0.0 && !bLeft)) && bStt && bIsTwoWall))
				{
					GetXyMatchLineAndLine(xy1, vAngFooting, xyCenter, vAngJijum, xyResult);
					if(((tvCenter.m_v2.y-xyResult.y) < 0) == bLeft) dLength = ~(tvCenter.m_v2-xyResult);
				}
				else if((vAngBet.x<0.0 && !bIsTwoWall) || (((vAngBet.x<0.0 && bLeft) || (vAngBet.x<0.0 && !bLeft)) && !bStt && bIsTwoWall))
				{
					GetXyMatchLineAndLine(xy2, vAngFooting, xyCenter, vAngJijum, xyResult);
					if(((tvCenter.m_v2.y-xyResult.y) > 0) == !bLeft) dLength = ~(tvCenter.m_v1-xyResult);
				}
				else
					dLength = 0.0;
			}
		}
	}

	dLength = max(dLenOld, dLength);

	return dLength;
}

// 기초 끝단 형상을 날개벽에 맞춤
void CRcBridgeApp::SetFootingEndShape(long nJ)
{
	CFootingApp* pFooting = GetFootingByJijumNumber(nJ);
	if(pFooting)
	{
		pFooting->m_bRAngLeft = m_nSeparBri!=2;
		pFooting->m_bRAngRight = m_nSeparBri!=1;
		pFooting->m_exFooting.m_bRAngLeft = pFooting->m_bRAngLeft;
		pFooting->m_exFooting.m_bRAngRight = pFooting->m_bRAngRight;

		double dAngTanL = GetAngleAzimuthTanOnJijum(nJ, -1).GetAngleDegree();
		double dAngTanR = GetAngleAzimuthTanOnJijum(nJ, 1).GetAngleDegree();
		pFooting->m_vAngLeft = m_nSeparBri!=2 ? GetAngleRectFootingEnd(nJ, TRUE) : ToDPointFrRadian(ToRadian(dAngTanL));
		pFooting->m_vAngRight = m_nSeparBri!=1 ? GetAngleRectFootingEnd(nJ, FALSE) : ToDPointFrRadian(ToRadian(dAngTanR));

		if(nJ==0)
			GetTvWingWallPlane(TRUE);
		else
			GetTvWingWallPlane(FALSE);

		CTwinVector tv1, tv2;
		CTwinVectorArray tvArr;
		tvArr = m_tvArrPlaneWingWall;

		tvArr.GetTvByInfo(_T("좌측날개벽외측"), tv1);
		tvArr.GetTvByInfo(_T("우측날개벽외측"), tv2);
		pFooting->m_xyLWingEnd = nJ==0 ? tv2.m_v2 : tv1.m_v2;
		pFooting->m_xyRWingEnd = nJ==0 ? tv1.m_v2 : tv2.m_v2;
		pFooting->m_exFooting.m_xyLWingEnd = pFooting->m_xyLWingEnd;
		pFooting->m_exFooting.m_xyRWingEnd = pFooting->m_xyRWingEnd;

		BOOL bStt = nJ==0 ? TRUE : FALSE;
		BOOL bLeft = bStt ? FALSE : TRUE;
		CWingWall* pWingL = GetWingWall(bStt, bLeft);
		CWingWall* pWingR = GetWingWall(bStt, !bLeft);
		if(pFooting->m_bFootingEndToWing)
		{
			pFooting->m_dMarginLeft = pWingL->m_bExist ? 0.0 : GetLengthFootingMargin(nJ, TRUE);
			pFooting->m_dMarginRight = pWingR->m_bExist ? 0.0 : GetLengthFootingMargin(nJ, FALSE);
			pFooting->m_exFooting.m_dMarginLeft = pFooting->m_dMarginLeft;
			pFooting->m_exFooting.m_dMarginRight = pFooting->m_dMarginRight;
		}
		else
		{
			pFooting->m_dMarginLeft = GetLengthFootingMargin(nJ, TRUE);
			pFooting->m_dMarginRight = GetLengthFootingMargin(nJ, FALSE);
			pFooting->m_exFooting.m_dMarginLeft = pFooting->m_dMarginLeft;
			pFooting->m_exFooting.m_dMarginRight = pFooting->m_dMarginRight;
		}
	}
}

// 헌치 하면 좌표 구함
// 슬래브교에서 신축이음이 들어가면 코드가 조금 복잡해 지므로 일부러 함수 만들어 씀.
// 헌치하면 좌표는 거더철근을 구하기 위한거다
// 거더철근은 사거리로구하므로, 헌치하면 좌표는 항상 사거리로 구해 준다.
BOOL CRcBridgeApp::GetTvHunchLower(CTwinVector &tvLower, long nJ, BOOL bLeft, BOOL bUpperSlabGirder)
{
	CTwinVectorArray tvArrOld;
	tvArrOld	= m_tvArrVertSection;
	GetTvVertSection(0, TRUE) ;

	if(nJ == 0 || nJ == m_nQtyJigan)
	{
		CString sLower	= _T("");
		if(nJ == 0)
		{
			sLower	= IsHunch(nJ) ? "시점헌치하면" : "슬래브하면";
		}
		else
		{
			long nCountInHunch	= GetIdxHunchInWall(m_nQtyJigan, FALSE) + 1;
			if(IsHunch(nJ))
				sLower	= "종점헌치하면";
			else
			{
				if(m_nQtyJigan > 1)
					sLower.Format("%d번째내측헌치우측슬래브하면", nCountInHunch);
				else
					sLower	= "슬래브하면";
			}
		}

		if(!m_tvArrVertSection.GetTvByInfo(sLower, tvLower)) return FALSE;
		tvLower.Sort(TRUE);
	}
	else
	{
		long nIdxInWall	= nJ - 1;
		CTwinVector tv;
		long nIdxHunch	= GetIdxHunchInWall(nIdxInWall, TRUE);
		CString sLeft	= _T("");
		CString sRight	= _T("");
		if(nIdxHunch < 0) return FALSE;

		if(bUpperSlabGirder)
		{
			sLeft.Format("%d번째내측헌치아래면좌측", nIdxHunch+1);
			sRight.Format("%d번째내측헌치아래면우측", nIdxHunch+1);
		}
		else
		{
			sLeft.Format("%d번째내측하부헌치윗면좌측", nIdxHunch+1);
			sRight.Format("%d번째내측하부헌치윗면우측", nIdxHunch+1);
		}

		if(!m_tvArrVertSection.GetTvByInfo(sLeft, tv)) return FALSE;
		tvLower.m_v1	= tv.GetXyLeft();

		if(!m_tvArrVertSection.GetTvByInfo(sRight, tv)) return FALSE;
		tvLower.m_v2	= tv.GetXyRight();
	}

	m_tvArrVertSection	= tvArrOld;
	return TRUE;
}

BOOL CRcBridgeApp::IsFixedJijum(long nJijum, BOOL bSttShoe)
{
	CJijum* pJ = GetJijum(nJijum);

	long nDirStt  = bSttShoe ? 1 : 0;
	long nQtyShoe = pJ->m_nQtyShoe[nDirStt];

	long i=0; for(i=0; i<nQtyShoe; i++)
	{
		if(pJ->m_nDirShoe[nDirStt][i]==0)
			return TRUE;
	}

	return FALSE;
}

// nSlab==-1 : 교량전체에서 ...
BOOL CRcBridgeApp::IsExistFixedJijum(long nSlab)
{
	long nQtyJijum = GetCountJijum();
	long nJijumStt = (nSlab==-1) ? 0 : GetJijumNumberSlabStt(nSlab);
	long nJijumEnd = (nSlab==-1) ? nQtyJijum-1 : GetJijumNumberSlabEnd(nSlab);
	
	if(m_nTypeBridge>0 && m_nTypeBridge<4)	//슬래브가 없는 라멘교 일때(Type=1,2,3)
	{
		nJijumStt=0;
		nJijumEnd=nQtyJijum-1;
	}
	
	for(long nJ=nJijumStt; nJ<=nJijumEnd; nJ++)
	{
		if(m_nTypeBridge>0 && m_nTypeBridge<4)	//슬래브가 없는 라멘교 일때(Type=1,2,3)
		{
			if(nJ>nJijumStt && nJ<nJijumEnd) 
				continue;
		}
		if(nJ==nJijumStt)
		{
			if(IsFixedJijum(nJ,TRUE))  return TRUE;
				continue;
		}
		
		if(nJ!=nJijumStt && nJ!=nJijumEnd)
		{
			if(IsFixedJijum(nJ,TRUE))  return TRUE;
			if(IsFixedJijum(nJ,FALSE)) return TRUE;
		}

		if(nJ==nJijumEnd)
		{
			if(IsFixedJijum(nJ,FALSE)) return TRUE;
				break;
		}
	}
	return FALSE;
}

// 날개벽이 있는지
BOOL CRcBridgeApp::IsWingWall(BOOL bStt, BOOL bLeft)
{
	CWingWall* pWing = GetWingWall(bStt, bLeft);

	double dL1 = pWing->m_dL1;
	double dL2 = pWing->m_dL2;
	BOOL bExist = (pWing->m_bExist && (dL1!=0 || dL2!=0)) ? TRUE : FALSE;

	return bExist;
}

BOOL CRcBridgeApp::IsWingWall()
{
	if(IsWingWall(TRUE, TRUE)) return TRUE;
	if(IsWingWall(TRUE, FALSE)) return TRUE;
	if(IsWingWall(FALSE, TRUE)) return TRUE;
	if(IsWingWall(FALSE, FALSE)) return TRUE;

	return FALSE;
}

// 날개벽에 수직 변단면이 있는지
BOOL CRcBridgeApp::IsWingWallVariableSect(BOOL bStt, BOOL bLeft)
{
	CWingWall* pWing = GetWingWall(bStt, bLeft);

	double dT1 = pWing->m_dT1;
	double dT2 = pWing->m_dT2;
	double dLT = pWing->m_dLT;
	int nVar = pWing->m_nDirHunch;

	BOOL bVar = nVar==1 && dT1!=dT2 && dLT>0;

	return bVar;
}

// 날개벽에 수평 변단면이 있는지
// BOOL CRcBridgeApp::IsWingWallVariablePlane(BOOL bStt, BOOL bLeft)
// {
// 	CWingWall* pWing = GetWingWall(bStt, bLeft);
// 
// 	double dT1 = pWing->m_dT1;
// 	double dT2 = pWing->m_dT2;
// 	double dLT = pWing->m_dLT;
// 	int nVar = pWing->m_nDirHunch;
// 
// 	BOOL bVar = nVar==2 && dT1!=dT2 && dLT>0;
// 
// 	return bVar;
// }

void CRcBridgeApp::GetIndexNearestFixShoe(CDPoint xyShoe, long &nJijumFix, long &nLeftFix, long &nRowFix, long FromJijum, long ToJijum)
{
	CJijum* pJ = NULL;

	double dDistMin    = 0.0;
	long   nCountFix   = 0;
	
	for(long nJ=FromJijum; nJ<=ToJijum; nJ++)
	{
		pJ = GetJijum(nJ);
		for(long nLeft=0; nLeft<2; nLeft++)
		{
			if(IsFixedJijum(nJ,nLeft)==FALSE) continue;
			if(nJ==FromJijum && nLeft==0) continue;
			if(nJ==ToJijum && nLeft==1) break;

			long nCountShoe = pJ->m_nQtyShoe[nLeft];
			for(long nRow=0; nRow<nCountShoe; nRow++)
			{
				long Dir = pJ->m_nDirShoe[nLeft][nRow];
				if(Dir!=0) continue;
				
				CDPoint xyFixShoe = GetXyShoe(nJ,nLeft,nRow);
				double dDist = ~(xyShoe-xyFixShoe);
				
				if(nCountFix==0)
				{
					dDistMin  = dDist;
					nJijumFix = nJ;
					nLeftFix  = nLeft;
					nRowFix   = nRow;
				}
				else if(dDist<dDistMin)
				{
					dDistMin  = dDist;
					nJijumFix = nJ;
					nLeftFix  = nLeft;
					nRowFix   = nRow;
				}
				nCountFix++;
			}
		}
	}
}

// long CRcBridgeApp::GetQtySlab()
// {
// 	if(m_nTypeBridge!=BRIDGE_TYPE_SLAB) return 1;
// 
// 	long nQtyJijum = GetCountJijum();
// 	long nQtyExpJoint = 0;
// 
// 	for(long nJ=0; nJ<nQtyJijum; nJ++)
// 	{
// 		if(m_bExpJoint[nJ])  nQtyExpJoint++;
// 	}
// 
// 	return nQtyExpJoint-1;
// }

long CRcBridgeApp::GetJijumNumberSlabStt(long nSlab)
{
	if(m_nTypeBridge!=BRIDGE_TYPE_SLAB) return -1;

	long nQtyJijum = GetCountJijum();
	long nQtyExpJoint = 0;

	for(long nJ=0; nJ<nQtyJijum; nJ++)
	{
		if(m_bExpJoint[nJ])  nQtyExpJoint++;
		if(nSlab==nQtyExpJoint-1) return nJ;
	}

	return -1;
}

long CRcBridgeApp::GetJijumNumberSlabEnd(long nSlab)
{
	if(m_nTypeBridge!=BRIDGE_TYPE_SLAB) return -1;

	long nQtyJijum = GetCountJijum();
	long nQtyExpJoint = 0;

	for(long nJ=0; nJ<nQtyJijum; nJ++)
	{
		if(m_bExpJoint[nJ])  nQtyExpJoint++;
		if(nSlab+2==nQtyExpJoint) return nJ;
	}

	return -1;
}

CDPoint CRcBridgeApp::GetAngleWingWallBetweenWall(BOOL bStt, BOOL bLeft)
{
	int nCountJijum = GetCountJijum();
	int nJ = bStt ? 0 : nCountJijum-1;
	int se = bStt ? -1 : 1;
	int lr = bLeft ? -1 : 1;
	CDPoint vAngWall = se*lr>0 ? -GetAngleAzimuthJijum(nJ) : GetAngleAzimuthJijum(nJ);
	CDPoint vAngWing = GetAngleAzimuthWingWall(bStt, bLeft);

	CDPoint vAng = bLeft ? vAngWall.BetweenAngle(vAngWing) : vAngWing.BetweenAngle(vAngWall);

	double dAng	= vAng.GetAngleDegree();
	CWingWall *pWing	= GetWingWall(bStt, bLeft);

	if(pWing->m_nAttachPos == 0)
	{
		if(Compare(dAng, 0.0, "=") || Compare(dAng, 180.0, "="))
			vAng	= CDPoint(0, 1);
	}

	return vAng;
}

// int CRcBridgeApp::GetCountTotalWingWall()
// {
// 	long nQtyHDan = 0;
// 	CWingWall* pWing = GetWingWall(TRUE, FALSE);
// 	if(pWing->m_bExist)
// 		nQtyHDan++;
// 	pWing = GetWingWall(TRUE, FALSE);
// 	if(pWing->m_bExist)
// 		nQtyHDan++;
// 	pWing = GetWingWall(FALSE, TRUE);
// 	if(pWing->m_bExist)
// 		nQtyHDan++;
// 	pWing = GetWingWall(FALSE, FALSE);
// 	if(pWing->m_bExist)
// 		nQtyHDan++;
// 
// 	return nQtyHDan;
// }

// B구간의 높이 또는 EL을 구함
double CRcBridgeApp::GetLengthWingWallBlockBC(BOOL bStt, BOOL bLeft, BOOL bEL)
{
	CWingWall* pWing = GetWingWall(bStt, bLeft);
	if(!pWing) return FALSE;

	CTwinVectorArray tvArr;
	CTwinVector tv;

	GetTvWingWallFront(bStt, bLeft, 1, tvArr);

	tvArr.GetTvByInfo(_T("날개벽정면하단"), tv);
	double dHgtB	= ~(tv.m_v2-tv.m_v1);
	if(m_nTypeBridge == BRIDGE_TYPE_BOX && m_bExistProtectionRiseFooting && pWing->m_dL2 > 0)
		dHgtB += m_dPRF_W;
	
	dHgtB = bEL ? tv.m_v1.y+dHgtB : dHgtB;
	
	return dHgtB;
}

/**
	@brief
		날개벽 A구간이 존재하는지?
*/
BOOL CRcBridgeApp::IsExistWingWallBlockA(BOOL bStt, BOOL bLeft)
{
	CWingWall* pWing = GetWingWall(bStt, bLeft);
	if(!pWing) return FALSE;

	double dL2 = pWing->m_dL2;
	if(dL2 == 0) return FALSE;

	CTwinVectorArray tvArr;
	CTwinVector tv;
	
	GetTvWingWallFront(bStt, bLeft, 1, tvArr);
	
	tvArr.GetTvByInfo(_T("날개벽정면하단"), tv);
	double dHgtB	= ~(tv.m_v2-tv.m_v1);
	// 날개벽 끝단의 높이가 dHgtB보다 작으면 날개벽 끝단의 높이로 dHgtB를 대신함
	{
		CTwinVector tvEnd	= tvArr.GetTvByInfo("날개벽정면끝단하부");
		double dHEnd	= tvEnd.GetVerLength();
		
		if(dHEnd < dHgtB)
			return FALSE;
	}
		
	return TRUE;
}

// 완충슬래브 권고안
void CRcBridgeApp::SetDataDefaultShapeCns(BOOL bStt, BOOL bLeft, long nIdx, double dFck, double dFy)
{
	if(nIdx < 0 || nIdx >= MAX_CNS) return; 
	// 완충슬래브도 같이 권고안 함
	long left		= bLeft ? iLEFT : iRIGHT;
	CApsApp *pAps	= bStt ? &m_cnsStt[left][nIdx] : &m_cnsEnd[left][nIdx];
	pAps->m_dAD1			= nIdx == 0 ? 300 : 0;
	pAps->m_dAD2			= nIdx == 0 ? 300 : 0;
	pAps->m_dST				= 0;
	pAps->m_bApplyWingAng	= FALSE;
	pAps->m_bOrthoSlabEnd	= TRUE;
	pAps->m_dL1				= 5000;
	pAps->m_dFck			= dFck;
	pAps->m_dFy				= dFy;
	pAps->m_dFy				= dFy;
}

// 접속슬래브 동기화 양이 많아서 따로 뺌(syncbridge에서만 호출 가능)
void CRcBridgeApp::SyncBridgeAps(long nLeft, BOOL bApplyAngle)
{
	long left	= 0;

	CWingWall *pWingL;
	CWingWall *pWingR;
	// 접속슬래브 동기화 ////////////////////////////////////
	CTwinVectorArray tvArrWing;
	CTwinVectorArray tvArrWingPlane;
	CTwinVector tv;
	
	m_tvArrVertSection.GetTvByInfo("시점브라켓상면", tv);
	for(left = 0; left < 2; left++)
	{
		m_apsStt[left].m_xyOrg		= tv.GetXyRight();
		m_apsStt[left].m_dSW		= (tv.GetLength()-m_apsStt[left].m_dST)/2;

		/*
		// 브라켓이 없으면 접속슬래브도 없음
		if(!IsBracket(TRUE))
			m_apsStt[left].m_bIs	= FALSE;
			*/
	}

	if(m_bDivisionApsStt && m_apsStt[iLEFT].m_bIs)
		m_apsStt[iRIGHT].m_bIs	= TRUE;
	else
		m_apsStt[iRIGHT].m_bIs	= FALSE;
	// 시점측 접속슬래브
	pWingL = GetWingWall(TRUE, TRUE);
	pWingR = GetWingWall(TRUE, FALSE);
	GetTvWingWallPlane(TRUE, TRUE, TRUE);
	m_tvArrPlaneWingWall.GetTvByInfo("슬래브끝단내측", tv);
	tv.m_sInfo = "기준면";
	tvArrWing.Add(tv);


	if(pWingL->m_nAttachPos == 0)
	{
		m_tvArrPlaneWingWall.GetTvByInfo((pWingL->m_nAttachPos==0)? "좌측날개벽시점헌치":"좌측벽체끝단", tv);
		tv.m_sInfo = "좌측날개벽헌치";
		GetSwap(tv.m_v1, tv.m_v2);
		if(tv.GetLength() > 0) tvArrWing.Add(tv);

		m_tvArrPlaneWingWall.GetTvByInfo("좌측날개벽내부헌치후", tv);
		tv.m_sInfo = "좌측날개벽";
		GetSwap(tv.m_v1, tv.m_v2);
		if(tv.GetLength() > 0) tvArrWing.Add(tv);
	}

	if(pWingR->m_nAttachPos == 0)
	{
		m_tvArrPlaneWingWall.GetTvByInfo((pWingR->m_nAttachPos==0)? "우측날개벽시점헌치":"우측벽체끝단", tv);
		tv.m_sInfo = "우측날개벽헌치";
		GetSwap(tv.m_v1, tv.m_v2);
		if(tv.GetLength() > 0) tvArrWing.Add(tv);

		m_tvArrPlaneWingWall.GetTvByInfo("우측날개벽내부헌치후", tv);
		tv.m_sInfo = "우측날개벽";
		GetSwap(tv.m_v1, tv.m_v2);
		if(tv.GetLength() > 0) tvArrWing.Add(tv);
	}
	
	for(left = 0; left < 2; left++)
	{
		m_apsStt[left].m_xyDirPlane		= -GetAngleAzimuthTanOnJijum(0, 0);
		m_apsStt[left].m_tvArrOrg		= tvArrWing;
		m_apsStt[left].m_bOrgLeft		= FALSE;
		m_apsStt[left].m_pBasicConcInfo	= m_pARcBridgeDataStd->m_pBasicConcInfo;
		m_apsStt[left].m_bIsDBar_ForDowelBar	= GetDBar_ForDowelBar();
		m_apsStt[left].SyncAps(bApplyAngle);

		

		// 완충슬래브도 sync해줌
		for(long cns = 0; cns < MAX_CNS; cns++)
		{
			if(m_nCountCnsStt[left] <= cns)
				m_cnsStt[left][cns].m_bIs			= FALSE;
			else
				m_cnsStt[left][cns].m_bIs			= TRUE;
			m_cnsStt[left][cns].m_pBasicConcInfo	= m_pARcBridgeDataStd->m_pBasicConcInfo;
			m_cnsStt[left][cns].m_bIsDBar_ForDowelBar	= GetDBar_ForDowelBar();
		}
		m_apsStt[left].SetSyncApsByThis(&m_cnsStt[left][0], bApplyAngle);
		m_cnsStt[left][0].SetSyncApsByThis(&m_cnsStt[left][1], bApplyAngle);
	}

	// 종점측 접속슬래브
	pWingL = GetWingWall(FALSE, TRUE);
	pWingR = GetWingWall(FALSE, FALSE);
	tvArrWing.RemoveAll();
	m_tvArrVertSection.GetTvByInfo("종점브라켓상면", tv);
	for(left = 0; left < 2; left++)
	{
		m_apsEnd[left].m_xyOrg	= tv.GetXyLeft();
		m_apsEnd[left].m_dSW	= (tv.GetLength()-m_apsEnd[left].m_dST)/2;

		/*
		// 브라켓이 없으면 접속슬래브도 없음
		if(!IsBracket(FALSE))
			m_apsEnd[left].m_bIs	= FALSE;
			*/
	}

	if(m_bDivisionApsEnd && m_apsEnd[iLEFT].m_bIs)
		m_apsEnd[iRIGHT].m_bIs	= TRUE;
	else
		m_apsEnd[iRIGHT].m_bIs	= FALSE;


	GetTvWingWallPlane(FALSE, TRUE, TRUE);
	m_tvArrPlaneWingWall.GetTvByInfo("슬래브끝단내측", tv);
	tv.m_sInfo = "기준면";
	tvArrWing.Add(tv);

	if(pWingL->m_nAttachPos == 0)
	{
		m_tvArrPlaneWingWall.GetTvByInfo((pWingL->m_nAttachPos==0)? "좌측날개벽시점헌치":"좌측벽체끝단", tv);
		tv.m_sInfo = "좌측날개벽헌치";
		GetSwap(tv.m_v1, tv.m_v2);
		if(tv.GetLength() > 0) tvArrWing.Add(tv);

		m_tvArrPlaneWingWall.GetTvByInfo("좌측날개벽내부헌치후", tv);
		tv.m_sInfo = "좌측날개벽";
		GetSwap(tv.m_v1, tv.m_v2);
		if(tv.GetLength() > 0) tvArrWing.Add(tv);
	}

	if(pWingR->m_nAttachPos == 0)
	{
		m_tvArrPlaneWingWall.GetTvByInfo((pWingR->m_nAttachPos==0)? "우측날개벽시점헌치":"우측벽체끝단", tv);
		tv.m_sInfo = "우측날개벽헌치";
		GetSwap(tv.m_v1, tv.m_v2);
		if(tv.GetLength() > 0) tvArrWing.Add(tv);

		m_tvArrPlaneWingWall.GetTvByInfo("우측날개벽내부헌치후", tv);
		tv.m_sInfo = "우측날개벽";
		GetSwap(tv.m_v1, tv.m_v2);
		if(tv.GetLength() > 0) tvArrWing.Add(tv);
	}

	for(left = 0; left < 2; left++)
	{
		m_apsEnd[left].m_xyDirPlane	= GetAngleAzimuthTanOnJijum(m_nQtyJigan, 0);
		m_apsEnd[left].m_tvArrOrg	= tvArrWing;
		m_apsEnd[left].m_bOrgLeft	= TRUE;
		m_apsEnd[left].m_pBasicConcInfo	= m_pARcBridgeDataStd->m_pBasicConcInfo;
		m_apsEnd[left].m_bIsDBar_ForDowelBar	= GetDBar_ForDowelBar();
		m_apsEnd[left].SyncAps(bApplyAngle);

		// 완충슬래브도 sync해줌
		for(long cns = 0; cns < MAX_CNS; cns++)
		{
			if(m_nCountCnsEnd[left] <= cns)
				m_cnsEnd[left][cns].m_bIs	= FALSE;
			else
				m_cnsEnd[left][cns].m_bIs	= TRUE;
			m_cnsEnd[left][cns].m_pBasicConcInfo	= m_pARcBridgeDataStd->m_pBasicConcInfo;
			m_cnsEnd[left][cns].m_bIsDBar_ForDowelBar	= GetDBar_ForDowelBar();
		}
		m_apsEnd[left].SetSyncApsByThis(&m_cnsEnd[left][0], bApplyAngle);
		m_cnsEnd[left][0].SetSyncApsByThis(&m_cnsEnd[left][1], bApplyAngle);
	}
	////////////////////////////////////////////////////////////////

	for(left = 0; left < 2; left++)
	{
		m_apsStt[left].m_dFck	= GetValueFck(ePartApsStt);
		m_apsStt[left].m_dFy	= GetValueFy(ePartApsStt);
		m_apsStt[left].m_dFyShear	= GetValueFy(ePartApsStt, TRUE);

		m_apsEnd[left].m_dFck	= GetValueFck(ePartApsEnd);
		m_apsEnd[left].m_dFy	= GetValueFy(ePartApsEnd);
		m_apsEnd[left].m_dFyShear	= GetValueFy(ePartApsEnd, TRUE);

		for(long cns = 0; cns < MAX_CNS; cns++)
		{
			m_cnsStt[left][cns].m_dFck	= GetValueFck(ePartApsStt);
			m_cnsStt[left][cns].m_dFy	= GetValueFy(ePartApsStt);
			m_cnsStt[left][cns].m_dFyShear	= GetValueFy(ePartApsStt, TRUE);

			m_cnsEnd[left][cns].m_dFck	= GetValueFck(ePartApsEnd);
			m_cnsEnd[left][cns].m_dFy	= GetValueFy(ePartApsEnd);
			m_cnsEnd[left][cns].m_dFyShear	= GetValueFy(ePartApsEnd, TRUE);
		}
	}
}

// 현재 교량이 사각적용 된 형태인지 판단.(종단면)
BOOL CRcBridgeApp::IsApplyAngle()
{
	if(m_tvArrVertSection.GetSize() > 0)
	{
		return m_tvArrVertSection.GetAt(0).m_bApplyAngle;
	}
	else
		return FALSE;
}

CDPoint CRcBridgeApp::GetAngleJijum(long nJ, BOOL bVertDir)
{

	if(nJ == -1) nJ = m_nQtyJigan;
	if(nJ > m_nQtyJigan) nJ = m_nQtyJigan;
	if(nJ < 0) nJ = 0;

	CJijum *pJ	= GetJijum(nJ);
	CDPoint vAng	= pJ->m_vAngle;

	if(bVertDir)
	{
		double dAng	= 90 - (GetAngleAzimuthJijum(0).GetAngleDegree() - GetAngleAzimuthJijum(nJ).GetAngleDegree());
		vAng	= ToDPointFrDegree(dAng);
	}

	if(Compare(vAng.GetAngleDegree(), 90.0, "="))
		vAng	= CDPoint(0, 1);
	return vAng;
}

// sta에 따라 상부슬래브 두께가 달라질 수 있다.
// 헌치 때문에.
// 사거리 sta으로 찾음(슬래브 상면 sta임)
// double CRcBridgeApp::GetHeightUpperSlab_OldOrNotUse(double dSta, CDPoint vAng)
// {
// // 	return GetHeightUpperSlab(GetJijumNumberByStation(dSta), 0, vAng);
// 
// 	// 최종적으로 헌치를 구할때 사용한 종단좌표가 지금꺼와 같으면 싱크를 다시 하지 않고 그대로 사용한다.
// 	static CTwinVectorArray tvArrVertSectionLast;
// 	CDRect rectLast	= tvArrVertSectionLast.GetRect();
// 	CDRect rect		= m_tvArrVertSection.GetRect();
// 	BOOL bSameWithLast	= FALSE;
// 	if(Compare(rect.left, rectLast.left, "=") && Compare(rect.right, rectLast.right, "=") && 
// 		Compare(rect.top, rectLast.top, "=") && Compare(rect.bottom, rectLast.bottom, "="))
// 	{
// 		bSameWithLast	= FALSE;
// 	}
// 
// 	BOOL bRecalcVertSection	= FALSE;
// 
// 	CTwinVectorArray tvArrUpper;	// 슬래브 상면
// 	CTwinVectorArray tvArrLower;	// 슬래브 하면
// 	CTwinVectorArray tvArrVertSection;
// 	tvArrVertSection	= m_tvArrVertSection;
// 	long nCountInWall	= GetCountInWall();
// 	CTwinVectorArray *pTvArrFront	= NULL;
// 
// 	long nTypeJongOld = m_nTypeJongdanForRebar;
// 	
// 	if(!bSameWithLast)
// 	{
// 		if(m_nTypeJongdanForRebar != 2)
// 		{
// 			GetTvVertSection_Std(0, TRUE, FALSE, FALSE, 2);
// 			bRecalcVertSection	= TRUE;
// 		}
// 
// 		if(nCountInWall > 0)
// 			pTvArrFront	= new CTwinVectorArray[nCountInWall];
// 
// 		for(long wall = 0; wall < nCountInWall; wall++)
// 		{
// 			CWallApp *pWall = GetInWall(wall);
// 			if(pWall)
// 			{
// 				//31720
// 				CTwinVectorArray tvArrVertSection;
// 				tvArrVertSection.AddFromTvArray(m_tvArrVertSection);
// 
// 				pTvArrFront[wall]	= pWall->m_tvArrFront;
// 				pWall->GetTvFront(tvArrVertSection, TRUE);
// 				
// 				// 내측헌치 R에 대해서 구해줌(접선일 경우에만 구해주면 됨)
// 				ApplyRoundOfInWallHunch(wall, TRUE);
// 				ApplyRoundOfInWallHunch(wall, FALSE);
// 			}
// 		}
// 	}
// 
// 	tvArrVertSectionLast	= m_tvArrVertSection;
// 
// 	double dStt	= GetStationByTarget(BRIDGE_OFF_STT_SLAB, 0, TRUE);
// 	double dEnd = GetStationByTarget(BRIDGE_OFF_END_SLAB, 0, TRUE);
// 	if(Compare(dSta, dStt, "="))
// 		dSta	= dStt;
// 	if(Compare(dSta, dEnd, "="))
// 		dSta	= dEnd;
// 
// 	if(dSta < dStt || dSta > dEnd) 
// 	{
// 		if(!bSameWithLast)
// 		{
// 			if(bRecalcVertSection)
// 			{
// 				m_nTypeJongdanForRebar = nTypeJongOld;
// 				m_nTypeJongdanForRebarOld = nTypeJongOld;
// 				GetTvVertSection_Std(0, TRUE, FALSE, FALSE, nTypeJongOld);
// 			}
// 
// 			if(pTvArrFront)
// 				delete [] pTvArrFront;
// 		}
// 
// 		return 0;
// 	}
// 
// 	dStt	= GetStationByTarget(BRIDGE_OFF_STT_STT, 0, TRUE);
// 	dEnd	= GetStationByTarget(BRIDGE_OFF_END_END, 0, TRUE);
// 
// 
// 	double dH	= 0;
// 	// 시점과 종점은 외측벽체 때문에 제대로 판단이 안됨
// 	// 따라서 그냥 헌치두께를 더해줌.
// 	if(Compare(dSta, dStt, "<="))
// 	{
// 		if(!bSameWithLast)
// 		{
// 			if(bRecalcVertSection)
// 			{
// 				m_nTypeJongdanForRebar = nTypeJongOld;
// 				m_nTypeJongdanForRebarOld = nTypeJongOld;
// 				GetTvVertSection_Std(0, TRUE, FALSE, FALSE, nTypeJongOld);
// 			}
// 			if(pTvArrFront)
// 				delete [] pTvArrFront;
// 		}
// 
// 		return m_dTS + m_dHHS;
// 	}
// 	else if(Compare(dSta, dEnd, ">="))
// 	{
// 		if(!bSameWithLast)
// 		{
// 			if(bRecalcVertSection)
// 			{
// 				m_nTypeJongdanForRebar = nTypeJongOld;
// 				m_nTypeJongdanForRebarOld = nTypeJongOld;
// 				GetTvVertSection_Std(0, TRUE, FALSE, FALSE, nTypeJongOld);
// 			}
// 			if(pTvArrFront)
// 				delete [] pTvArrFront;
// 		}
// 
// 		return m_dTS + m_dHHE;
// 	}
// 
// 
// 	CRebarPlacing rb;
// 	
// 	// 해당 sta에서 vAng각도로 교차하는 모든걸 찾음
// 	double dTP	= GetThickPave();
// 	CDPointArray xyArrMatch;
// 	CLineInfo *pLine	= GetLineBase(0);
// 	CDPoint xyStt	= CDPoint(dSta, pLine->GetElevationPlanEl(dSta)-dTP);
// 	CDPoint xyTop	= xyStt;
// 	pLine->GetXyMatchByJongDanXyAndAngle(xyStt, vAng, xyTop, -dTP);
// 	CTwinVectorArray tvArrVertSectionTmp;
// 	// 리브는 빼고 검사하자
// 	long i = 0; for(i = 0; i < m_tvArrVertSection.GetSize(); i++)
// 	{
// 		CTwinVector tv	= m_tvArrVertSection.GetAt(i);
// 		if(tv.m_sInfo.Find("리브", 0) == -1)
// 			tvArrVertSectionTmp.Add(tv);
// 	}
// 
// 	rb.GetXyMatchTvArrAndLine(tvArrVertSectionTmp, xyStt, vAng, xyArrMatch);
// 	xyArrMatch.Add(xyTop);
// 	if(xyArrMatch.GetSize() > 0)
// 	{
// 		long nSize	= xyArrMatch.GetSize();
// 		if(nSize == 1)
// 		{
// 			CDPoint xy	= xyArrMatch.GetAt(0);
// 			dH	= ~(xy - xyStt);
// 		}
// 		else
// 		{
// 			long i = 0; for(i = 0; i < nSize-1; i++)
// 			{
// 				CDPoint xy	= xyArrMatch.GetAt(i);
// 				long j = 0; for(j = i + 1; j < nSize; j++)
// 				{
// 					CDPoint xy1	= xyArrMatch.GetAt(j);
// 					double dDist	= ~(xy-xy1);
// 
// 					if(dDist > dH)
// 					{
// 						dH = dDist;
// 					}
// 				}
// 			}
// 		}
// 	}
// 
// 	
// 	
// 	if(!bSameWithLast)
// 	{
// 		for(long wall = 0; wall < nCountInWall; wall++)
// 		{
// 			CWallApp *pWall	= GetInWall(wall);
// 			if(pWall)
// 			{
// 				pWall->m_tvArrFront	= pTvArrFront[wall];
// 			}
// 		}
// 		
// 		if(bRecalcVertSection)
// 		{
// 			m_nTypeJongdanForRebar = nTypeJongOld;
// 			m_nTypeJongdanForRebarOld = nTypeJongOld;
// 			GetTvVertSection_Std(0, TRUE, FALSE, FALSE, nTypeJongOld);
// 		}
// 
// 		if(pTvArrFront)
// 			delete [] pTvArrFront;
// 	}
// 
// 
// 	return dH;
// }

// 평면상의 시종점 교대 라인
// dOffset : offset 간격, use for 'General Drawing'!
// BOOL bExp : 가각적용할지?
void CRcBridgeApp::GetTvPlaneSttEndLine(CTwinVectorArray &tvArr, BOOL bStt, BOOL bSlab, double dDistMove, BOOL bExp)
{
	tvArr.RemoveAll();
	if(m_bSettingPlane && bSlab)
	{
		tvArr	= m_tvArrSttEndPlane[bStt ? iSTT : iEND][bExp ? iEXP : iNORMAL];
	}
	else
	{
		CDPoint A[4];
		TARGET_IN_ONE_SLAB nTarget	= BRIDGE_OFF_STT_SLAB;
		if(bSlab)
		{
			nTarget	 = bStt ? BRIDGE_OFF_STT_SLAB : BRIDGE_OFF_END_SLAB;
			GetXySss(nTarget, A, bExp);
		}
		else
		{
			nTarget	= bStt ? BRIDGE_OFF_STT_STT : BRIDGE_OFF_END_END;
			GetXySss(nTarget, A, bExp);
		}

		CTwinVector tv;
		long i = 0; for(i = 0; i < 4-1; i++)
		{
			tv.m_v1	= A[i];
			tv.m_v2	= A[i+1];
			tvArr.Add(tv);
		}

		if(0)//!bExp)
		{
			CDPoint vAng		= GetAngleJijum(bStt ? 0 : m_nQtyJigan);
			CLineInfoApp *pLine	= static_cast<CLineInfoApp*>(GetLineBase());
			double dSta			= bStt ? m_dStationBridgeStt : m_dStationBridgeStt+m_dLengthBridge;
			if(bSlab)
			{
				if(IsOutWall(bStt))
				{
					dSta	+= bStt ? -m_dWS : m_dWE;
				}
				else
				{
					long nJ	= bStt ? 0 : m_nQtyJigan;
					CJijum *pJ	= GetJijum(nJ);
					if(m_bExpJoint[nJ])
					{
						dSta += bStt ? pJ->m_dLengthUgan[1] : -pJ->m_dLengthUgan[0];
					}
				}
			}

			double dWidLeft		= GetWidthSlabAct(dSta, vAng, -1);
			double dWidRight	= GetWidthSlabAct(dSta, vAng, 1);

			CTwinVector tv;
			tv.m_v1	= pLine->GetXyLineByLengthAndAngle(dSta, -dWidLeft, vAng);
			tv.m_v2	= pLine->GetXyLineByLengthAndAngle(dSta, dWidRight, vAng);
					
			tvArr.RemoveAll();
			tvArr.Add(tv);
		}


		CString str	= bStt ? "시점" : "종점";
		for(i = 0; i < tvArr.GetSize(); i++)
		{
			tv	= tvArr.GetAt(i);
			tv.m_sInfo	= str+tv.m_sInfo;
			tvArr.SetAt(i, tv);
		}

		if(bSlab)
			m_tvArrSttEndPlane[bStt ? iSTT : iEND][bExp ? iEXP : iNORMAL]	= tvArr;
	}

	if(dDistMove != 0.0)
	{
		CLineInfo *pLine	= GetLineBase();
		CDPoint xyMatch(0, 0);

		double dSta	= 0;
		
		CTwinVector tv;
		long i = 0; for(i = 0; i < tvArr.GetSize(); i++)
		{
			double dDist	= 0;
			tv	= tvArr.GetAt(i);
			pLine->GetStationMatchVerticalByXy(tv.m_v1, dSta);
			dDist	= pLine->GetDisMatchVerticalByXy(tv.m_v1);

			dSta += dDistMove;
			tv.m_v1	= pLine->GetXyLineDis(dSta, dDist);

			pLine->GetStationMatchVerticalByXy(tv.m_v2, dSta);
			dDist	= pLine->GetDisMatchVerticalByXy(tv.m_v2);

			dSta += dDistMove;
			tv.m_v2	= pLine->GetXyLineDis(dSta, dDist);

			tvArr.SetAt(i, tv);
		}
	}
}

// 도면을 시점과 종점이 수평으로 되도록 그리려 할때
// 회전시켜야 되는 각도 리턴
CDPoint CRcBridgeApp::GetXyDirRotateDomByBridgeSttEndXy()
{
	CLineInfoApp *pLine = static_cast<CLineInfoApp*>(GetLineBase());

	CDPoint xyStt   = pLine->GetXyLine(m_dStationBridgeStt);
	CDPoint xyEnd   = pLine->GetXyLine(m_dStationBridgeStt + m_dLengthBridge);
	CDPoint vRotate = (xyEnd-xyStt).Unit().GetMirrorVert();

	return vRotate;
}

// 도로중심 리턴
double CRcBridgeApp::GetMidCrossSection(BOOL bVert)
{
	return GetDistMidOfSlab(m_dStationBridgeStt-m_dWS, bVert, TRUE);
}

// 이 함수를 고칠 경우 종단제원 입력에 있는 SyncWall도 고쳐야 됨.
void CRcBridgeApp::SyncWall(long nJijum, CWallApp *pWall, BOOL bApplyAngle, double dBaseStation)
{
	if(nJijum == GetJijumConnectRibAndSlab())
	{
		pWall->m_bNotRebar		= TRUE;
		pWall->m_bRoundRect		= FALSE;
		pWall->m_bTopIsColumn	= FALSE;
	}
	else
	{
		pWall->m_bNotRebar		= FALSE;
		pWall->m_bSideIsArc		= FALSE;
	}

	// 경사교2타입인 경우 중간벽체 첫번째와 마지막번째는 EL이 자동조정된다. ////
	// 단 level로 그릴때는 자동조정하지 않는다. (프레임해석용) - 배근은 level 지원하지 않음
	if((m_nTypeBridge == BRIDGE_TYPE_ARCH || m_nTypeBridge == BRIDGE_TYPE_TRAP2) && pWall)
	{
		if(nJijum == 1 || nJijum == m_nQtyJigan-1)
		{
			CFootingApp *pFooting	= nJijum == 1 ? &m_footingStt : &m_footingEnd;
			if(m_nTypeJongdanForRebar  != 0)
			{
				CTwinVector tvSlope	= pFooting->m_tvArrFront.GetTvByInfo(nJijum==1 ? "기초우측경사" : "기초좌측경사");
				CDPoint xyMatch(0, 0);
				if(GetXyMatchLineAndLine(tvSlope.m_v1, tvSlope.GetXyDir(), pWall->m_xyOrg, ToDPointFrDegree(pWall->GetAngleWallByHor(bApplyAngle)), xyMatch))
				{
					pWall->m_dELB	= xyMatch.y;
				}
			}
			pWall->m_footing = *pFooting;
			pWall->m_footing.m_bIs	= FALSE;
			pWall->m_footing.m_bVirtualFooting	= m_nTypeBridge == BRIDGE_TYPE_TRAP2 ? TRUE : FALSE;
			pWall->m_footing.m_exFooting.m_bIs	= FALSE;
		}
		else
		{
			pWall->m_footing.m_bIs	= (m_nTypeBridge == BRIDGE_TYPE_TRAP2 || nJijum == 0 || nJijum == m_nQtyJigan) ? TRUE : FALSE;
			pWall->m_footing.m_bVirtualFooting	= FALSE;
			pWall->m_footing.m_exFooting.m_bIs	= TRUE;
		}
	}
	else if(m_nTypeBridge == BRIDGE_TYPE_BOX)
	{
		// 기둥 배근을 위해서 기초에 피복만 넣어주자.
		// 엔진에서 기초가 없는데도 배근할때 기초의 피복을 쓰네..
		pWall->m_footing.m_dCoverLower[0] = m_dCoverLower_LowerSlab[0];
	}
	//////////////////////////////////////////////////////////////////////////////

	CTwinVector tv = GetTvJijum(nJijum, 0);
	GetSwap(tv.m_v1, tv.m_v2);

	pWall->m_nTypeRebarFront	= pWall->IsVertWall() && pWall->m_tvArrLowerLimit_FrontRebar.GetSize() == 0 ? 1 : 0;
	pWall->m_dDiffTop[0]	= nJijum == 0 ? 0 : GetDiffELSttAndJijumForVertSection(nJijum, FALSE);
	
	if(IsBoxType())
	{
		pWall->m_dELBDiff	= 0;
		// Box 타입은 좌측 우측도 따로 구해서 저장하자.
		pWall->m_dDiffTop[0]	= GetDiffELSttAndJijumForVertSection(nJijum, (nJijum==0)? FALSE : TRUE, FALSE, TRUE);
		pWall->m_dDiffTop[1]	= GetDiffELSttAndJijumForVertSection(nJijum, (nJijum==0)? TRUE : FALSE, FALSE, TRUE);
	}
	else
	{
		pWall->m_dELBDiff	= pWall->m_dDiffTop[0];
		pWall->m_dDiffTop[1]	= pWall->m_dDiffTop[0];
	}

	// 내측벽체는 헌치까지만 구한다.
	double dLenL	= GetDistCrossSectionAndInWall(nJijum-1, TRUE, TRUE);
	double dLenR	= GetDistCrossSectionAndInWall(nJijum-1, TRUE, FALSE);
	if(nJijum > 0 && nJijum < m_nQtyJigan && !pWall->m_bTopIsColumn)
	{
		tv.m_v2	= tv.m_v2 + tv.GetXyDir(TRUE) * dLenL;
		tv.m_v1	= tv.m_v1 + tv.GetXyDir() * dLenR;
	}

	// 벽체 양 끝부분의 각도와 path를 적용 ///////////////
	CDPoint vStt = GetAngleAzimuthTanOnJijum(nJijum, -1);
	CDPoint vEnd = GetAngleAzimuthTanOnJijum(nJijum, 1);

	pWall->m_tvPathPlan	= tv;
	pWall->m_tvPathPlanLower = tv;
	pWall->m_vAngLeft	= vStt;
	pWall->m_vAngRight	= vEnd;
	/////////////////////////////////////////////////////

	SyncWall_ColumnPlanJewon(pWall);

	pWall->m_Column.m_dAngPlan = pWall->GetXyDirTop().GetAngleDegree()+90;
	
	// 기둥객체에 EL 설정
	pWall->m_Column.m_dSta	= pWall->m_xyOrg.x;
	pWall->m_Column.m_xyCenInFront.x	= pWall->m_Column.m_dSta;
	pWall->m_Column.m_dEL	= pWall->m_xyOrg.y;
	pWall->m_Column.m_dELB	= pWall->m_dELB;
	pWall->m_Column.m_dELBDiff	= pWall->m_dELBDiff;
	pWall->m_Column.m_dDiffTop	= (pWall->m_dDiffTop[0] + pWall->m_dDiffTop[1]) / 2.;

	// 좌측면도 기준설정
	tv.m_v1.z = tv.m_v2.z = 0;
	
	double dSta	= pWall->m_xyOrg.x;
	CDPoint vAng = bApplyAngle ? GetAngleJijum(nJijum) : CDPoint(0, 1); 
	// 외측벽체는 기준 스테이션을 슬래브 끝으로 한다.
	if(nJijum == 0 || nJijum == m_nQtyJigan)
	{
		if(m_nTypeWallForRebarGen == 0)
		{
			dSta	= GetStationByTarget(nJijum ==0 ? BRIDGE_OFF_STT_SLAB : BRIDGE_OFF_END_SLAB, 0, TRUE);
			vAng	= GetAngleByTarget(nJijum == 0 ? BRIDGE_OFF_STT_SLAB : BRIDGE_OFF_END_SLAB, 0);
		}
		else if(m_nTypeWallForRebarGen == 1)
		{
			dSta	= GetStationByTarget(nJijum == 0 ? BRIDGE_OFF_STT_STT : BRIDGE_OFF_END_END, 0, TRUE);
			vAng	= GetAngleByTarget(nJijum == 0 ? BRIDGE_OFF_STT_STT : BRIDGE_OFF_END_END, 0);
		}
	}
	if(!bApplyAngle)
		vAng	= CDPoint(0, 1);


	// 중간벽체의 경우에만 아래값 넣어줌
	if(nJijum > 0 && nJijum < m_nQtyJigan)
	{
		pWall->m_dAngleUpperWall_Front	= GetVAngHunchLower(nJijum-1, 0, !bApplyAngle).GetAngleDegree();

		// 횡단면 관련 ////
		CDPoint vAng	= ToDPointFrDegree(pWall->GetAngleWallByHor(bApplyAngle));
		
		CDPoint xyOrg	= pWall->m_xyOrg;
		CDPoint xyOrgL	= CDPoint(pWall->m_xyOrg.x - pWall->m_dW/vAng.y/2, pWall->m_xyOrg.y);
		CDPoint xyOrgR	= CDPoint(pWall->m_xyOrg.x + pWall->m_dW/vAng.y/2, pWall->m_xyOrg.y);

		if(!pWall->IsVertWall())
		{
			dSta	= GetXyMatchLineAndSlabUpper(xyOrg, vAng, 0, 0, !bApplyAngle).x;
		}
		double dStaL	= GetXyMatchLineAndSlabUpper(xyOrgL, vAng, 0, 0, !bApplyAngle).x;
		double dStaR	= GetXyMatchLineAndSlabUpper(xyOrgR, vAng, 0, 0, !bApplyAngle).x;

		CLineInfo *pLine	= GetLineBase();
		pWall->m_dDiffHLeftByCenter	= pLine->GetElevationPlanEl(dStaL) - pLine->GetElevationPlanEl(dSta);		// 벽체상면 중심과 좌측과의 차이
		pWall->m_dDiffHRightByCenter	= pLine->GetElevationPlanEl(dStaR) - pLine->GetElevationPlanEl(dSta);		// 벽체상면 중심과 우측과의 차이

		pWall->m_dThickUppeSlab	= m_dTS;
		pWall->m_dThickLowerSlab = m_dTS_Lower;
		// 횡단면 관련 ////


		// 종단이 변경되었을때...
		SyncWall_JongdanLimitFront(pWall, bApplyAngle);
		// 종단면 일반도 그릴때 필요 //
	}

	// 횡단면이나 지간이 바꼈을때... //
	CRebarPlacing rb;

	if(IsBoxType())
	{
		// (ARCBRIDGE-3268) 내용을 확인 할수 없다.
		// 좌측면의 상단 좌표를 저장하는데 Sta는 좌측면이 아니다.
		// 기존타입들은 그냥 사용하고 있었으므로 BoxType인경우만 맞추자
		CDPoint vAng = bApplyAngle ? GetAngleJijum(nJijum) : CDPoint(0, 1); 
		// 외측벽체는 기준 스테이션을 슬래브 끝으로 한다.
		if(nJijum == 0)
		{
			dSta	= GetStationByTarget(BRIDGE_OFF_STT_SLAB, 0, TRUE);
		}
		else if(nJijum == m_nQtyJigan)
		{
			dSta	= GetStationByTarget(BRIDGE_OFF_END_END, 0, TRUE);
		}
		else
		{
			// 기둥식인 경우는 전면 배면이 따로 있는게 아니라 중앙부를 그리기 때문에 그냥 중심위치를...
			CDPoint vAng	= GetAngleJijum(nJijum, !bApplyAngle);
			if(pWall->m_bTopIsColumn)
				dSta	= pWall->m_xyOrg.x;
			else
				dSta	= pWall->m_xyOrg.x - pWall->m_dW / vAng.y / 2.;
		}
	}

	if(Compare(dBaseStation, dSta_Check_NotUse_Minus, _T("="), 0.01) == FALSE)
	{
		dSta = dBaseStation;
	}

	pWall->m_tvArrLeftUpper.RemoveAll();
	pWall->m_tvArrLeftUpperSlab.RemoveAll();
	GetTvCrossSection(pWall->m_tvArrLeftUpper    , dSta, vAng, FALSE, FALSE, FALSE, 0, FALSE, FALSE, FALSE, FALSE);
	GetTvCrossSection(pWall->m_tvArrLeftUpperSlab, dSta, vAng, FALSE, FALSE, FALSE, 0, FALSE, FALSE, FALSE, TRUE);

	// pWall->m_tvArrLeftUpperSlab와 같은 EL이어야 하므로 pWall->m_tvArrLeftUpper폭 만큼 Slab를 잘라서 사용하자
	// 가각인 경우 EL 맞지 않음.(2009.2.25)
	double dXLeft	= pWall->m_tvArrLeftUpper.GetXyLeft().x;
	double dXRight	= pWall->m_tvArrLeftUpper.GetXyRight().x;
	CDRect rectSlab	= pWall->m_tvArrLeftUpperSlab.GetRect();
	pWall->m_tvArrLeftUpper	= pWall->m_tvArrLeftUpperSlab;
	rb.TrimTvArrayByLine(pWall->m_tvArrLeftUpper, CDPoint(dXLeft, rectSlab.bottom - 10), CDPoint(dXLeft, rectSlab.top + 10), TRUE, FALSE, FALSE);
	rb.TrimTvArrayByLine(pWall->m_tvArrLeftUpper, CDPoint(dXRight, rectSlab.bottom - 10), CDPoint(dXRight, rectSlab.top + 10), FALSE, FALSE, FALSE);

	//double dELDiff	= GetDiffELSttAndJijumForVertSection(nJijum, m_nTypeWallForRebarGen==0?FALSE:TRUE, TRUE);
	/*
	if(m_nTypeJongdanForRebar == 1)
	{
		rb.MoveRebar(pWall->m_tvArrLeftUpper, CDPoint(0, dELDiff));
		rb.MoveRebar(pWall->m_tvArrLeftUpperSlab, CDPoint(0, dELDiff));
	}
	*/
	if(nJijum > 0 && nJijum < m_nQtyJigan && !pWall->m_bTopIsColumn)
	{
		CDRect rect	= pWall->m_tvArrLeftUpper.GetRect();
		CRebarPlacing rb;
		rb.TrimTvArrayByLine(pWall->m_tvArrLeftUpper, CDPoint(rect.left+dLenL, rect.bottom-10), CDPoint(rect.left+dLenL, rect.top+10), TRUE, FALSE, FALSE);
		rb.TrimTvArrayByLine(pWall->m_tvArrLeftUpper, CDPoint(rect.right-dLenR, rect.bottom-10), CDPoint(rect.right-dLenR, rect.top+10), FALSE, FALSE, FALSE);
	}

	pWall->m_dCenterOfWall = GetMidCrossSection(!bApplyAngle);

	//31720
	CTwinVectorArray tvArrVertSection;
	tvArrVertSection.AddFromTvArray(m_tvArrVertSection);

	pWall->SyncWall(tvArrVertSection, bApplyAngle, GetAngleJijum(nJijum).GetAngleDegree());
	// 좌측 벽체 좌표 구한뒤에 벽체 측면이 아크로 된거면 
	// 슬래브의 보정면과 합쳐 버린다. ㅋㅋ
	if(GetJijumConnectRibAndSlab() == nJijum && pWall->m_bSideIsArc)
	{
		CString str	= _T("");
		str.Format("%d번째내측헌치좌측보정면", nJijum);
		CTwinVector tv	= m_tvArrVertSection.GetTvByInfo(str);
		if(tv.GetVerLength() > 0)
		{
			CTwinVector tvLeft	= pWall->m_tvArrFront.GetTvByInfo("벽체좌측면");
			tvLeft.m_v2	= tv.GetXyTop();
			tvLeft.m_v2.z	= tvLeft.GetVerLength()/2;
			tvLeft.m_xyRCenter	= tvLeft.GetXyMid();
			pWall->m_tvArrFront.SetTvByInfo("벽체좌측면", tvLeft);

			tv.m_v1	= tv.GetXyTop();
			tv.m_v2	= tv.GetXyTop();
			m_tvArrVertSection.SetTvByInfo(str, tv);
		}

		str.Format("%d번째내측헌치우측보정면", nJijum);
		tv	= m_tvArrVertSection.GetTvByInfo(str);
		if(tv.GetVerLength() > 0)
		{
			CTwinVector tvRight	= pWall->m_tvArrFront.GetTvByInfo("벽체우측면");
			tvRight.m_v2	= tv.GetXyTop();
			tvRight.m_v2.z	= -tvRight.GetVerLength()/2;
			tvRight.m_xyRCenter	= tvRight.GetXyMid();
			pWall->m_tvArrFront.SetTvByInfo("벽체우측면", tvRight);

			tv.m_v1	= tv.GetXyTop();
			tv.m_v2	= tv.GetXyTop();
			m_tvArrVertSection.SetTvByInfo(str, tv);
		}
	}

	
	if(!pWall->IsVertWall())
	{
		CDPoint vAng	= ToDPointFrDegree(pWall->GetXyDirTop().GetAngleDegree() + 90 + (90 - pWall->m_dAnglePath));
		CTwinVector tvWallLower	= pWall->m_tvArrFront.GetTvByInfo("벽체하면좌측");
		CDPoint xyJijum	= GetXySlabUpperByOffsetFrStation(GetStationOnJijum(nJijum), 0, 0);
		double dDist	= xyJijum.x - tvWallLower.m_v1.x;
		pWall->m_tvPathPlanLower	= tv;
		pWall->m_tvPathPlanLower.m_v1 += vAng * dDist;
		pWall->m_tvPathPlanLower.m_v2 += vAng * dDist;
		pWall->GetTvPlan();
	}

	// 내측헌치 R에 대해서 구해줌(접선일 경우에만 구해주면 됨)
	ApplyRoundOfInWallHunch(nJijum-1, TRUE);
	ApplyRoundOfInWallHunch(nJijum-1, FALSE);

	if(nJijum == 0 || nJijum == m_nQtyJigan)
	{
		pWall->m_dThickUppeSlab	 = 0;
		pWall->m_dThickHunch	= 0;
		pWall->m_dThickHunchL	= 0;
		pWall->m_dThickHunchR	= 0;
		pWall->m_dThickLowerSlab = 0;
		pWall->m_dThichLowerHunch = 0;
	}
	else
	{
		SyncWall_HunchLength(nJijum, pWall, bApplyAngle);
		pWall->m_Column.m_tvArrLeftUpper	= pWall->m_tvArrLeftUpper;
		rb.MoveRebar(pWall->m_Column.m_tvArrLeftUpper, CDPoint(0, -m_dTS-pWall->m_dThickHunch));
	}

	if(pWall->m_tvArrLowerLimit_Front.GetSize() > 0 && !IsBoxType())
	{
		CDPointArray xyArrMatch;
		CRebarPlacing rb;
		double dSta	= GetStationOnJijum(nJijum);
		pWall->m_dELB	= m_pbRib[iUPPER].GetXyByDist(m_pbRib[iUPPER].GetDistByXPos(dSta)).y;
		pWall->m_Column.m_dELB	= pWall->m_dELB;
	}
}

CDPoint CRcBridgeApp::GetXyCenterWingWallPlane(BOOL bStt, BOOL bLeft)
{
	GetTvWingWallPlane(bStt);
	CString strDir = bLeft ? _T("좌측") : _T("우측");

	CTwinVector tv;
	m_tvArrPlaneWingWall.GetTvByInfo(strDir+_T("날개벽외측"), tv);

	return tv.GetXyMid();
}

void CRcBridgeApp::GetDimArrayShoeDist(CDPointArray& xyArr, int nJ, int nLeft)
{
	xyArr.RemoveAll();
	
	CJijum* pJ = GetJijum(nJ);

	CDPoint vAng(0, 1);
	double dSta = GetStationShoeOnJijum(nJ, 0, nLeft);
	double dSlabWidth = GetWidthSlabAct(dSta, vAng);
	double dLeftDist = GetDisSlabLeftAct(dSta, vAng);
	double dDist1 = dLeftDist;
	double dDist2 = 0;

	CDoubleArray dArr;
	int nCountShoe = pJ->m_nQtyShoe[nLeft];
	int i=0; for(i=0; i<nCountShoe; i++)
	{
		dDist2 = pJ->m_dDistShoe[nLeft][i];
		dArr.Add(dDist2-dDist1);
		dDist1 = dDist2;
	}
	dDist2 = dSlabWidth+dLeftDist;
	dArr.Add(dDist2-dDist1);

	m_pARcBridgeDataStd->GetPointArrayCountAndValue(xyArr, dArr);
}

CString CRcBridgeApp::GetStringBridgeType()
{
	CString str = _T("");

	if(m_nTypeBridge==BRIDGE_TYPE_PORTAL)	str = _T("문형 라멘교");
	if(m_nTypeBridge==BRIDGE_TYPE_PI)		str = _T("∏형 라멘교");
	if(m_nTypeBridge==BRIDGE_TYPE_V)		str = _T("V형 라멘교");
	if(m_nTypeBridge==BRIDGE_TYPE_TRAP)		str = _T("경사 교각 라멘교");
	if(m_nTypeBridge==BRIDGE_TYPE_TRAP2)	str = _T("경사 교각 라멘교2");
	if(m_nTypeBridge==BRIDGE_TYPE_SLAB)		str = _T("슬래브교");
	if(m_nTypeBridge==BRIDGE_TYPE_BOX)		str = _T("BOX형 라멘교");
	if(m_nTypeBridge==BRIDGE_TYPE_ARCH)		str = _T("아치형 라멘교");

	return str;
}

// 종단면 상단 포장면 좌표 구함.
void CRcBridgeApp::GetTvPave_VertSection(CTwinVectorArray &tvArr)
{
	tvArr.RemoveAll();
	if(Compare(GetThickPave(FALSE), 0.0, "=")) return;

	CTwinVectorArray tvArrUpper;	// 슬래브 상단 좌표들
	CTwinVector tvUpper;		

	// 슬래브 상면 좌표들 구함.
	if(m_nTypeBridge == BRIDGE_TYPE_SLAB)
	{
// 		CString strStt	= _T(""),
		CString	strEnd	= _T("");
		CString strStt	= "슬래브상면(1번째지간)";
		strEnd.Format("슬래브상면(%d번째지간)", m_nQtyJigan);
		m_tvArrVertSection.GetTvArrByInfoRange(strStt, strEnd, tvArrUpper);
	}
	else
	{
		if(IsOutWall(TRUE))
		{
			tvUpper	= m_tvArrVertSection.GetTvByInfo("슬래브상면(시점벽체위)");
			tvArrUpper.Add(tvUpper);
		}
		tvUpper	= m_tvArrVertSection.GetTvByInfo("슬래브상면(외측벽체안쪽)");
		tvArrUpper.Add(tvUpper);
		if(IsOutWall(FALSE))
		{
			tvUpper	= m_tvArrVertSection.GetTvByInfo("슬래브상면(종점벽체위)");
			tvArrUpper.Add(tvUpper);
		}
	}


	CRebarPlacing rb;
	// 포장면
	double thickPave		= GetThickPave(FALSE);
	long nTerm		= 20;
	double dSpace	= thickPave/nTerm;
	long i = 0; for(i = 0; i < nTerm; i++)
	{
		rb.MoveRebar(tvArrUpper, CDPoint(0, dSpace));
		tvArr.AddFromTvArray(tvArrUpper);
	}
}

//종단면 부상방지 저판 좌표 구함
void CRcBridgeApp::GetTvPRF_VertSection(CTwinVectorArray &tvArr, BOOL bStt, long nLeft, BOOL bVertDir, BOOL bApplyAngle, BOOL bAddTvArr)
{
	tvArr.RemoveAll();
	if(m_nTypeBridge != BRIDGE_TYPE_BOX) return;
	if(m_bExistProtectionRiseFooting == FALSE) return;
	
	CLineInfo* pLine = GetLineBase();
	CTwinVector tv;
	CDPoint xyStt(0,0), xyPRF(0,0);

	long nJijum = bStt ? 0 : GetCountJijum() - 1;

	double sw = bStt ? -1.0 : 1.0;
	CDPoint vAngJijum = bApplyAngle ? GetAngleJijum(nJijum) : CDPoint(0,1);

	CTwinVector tvPRFStt;
	if(bStt)
		tvPRFStt = m_tvArrVertSection.GetTvByInfo(_T("시점벽체(부상방지저판)"));
	else
		tvPRFStt = m_tvArrVertSection.GetTvByInfo(_T("종점벽체(부상방지저판)"));
	
	xyStt = tvPRFStt.m_v2;

	CString strInfo = _T("");
	CString strStt = _T("");
	bStt ? strStt = _T("시점") : strStt = _T("종점");

	// 부상방지저판 끝단
	double dGiulJong = (m_bSlopeLowerSlab && bVertDir) ? pLine->GetJongDanGiul(xyStt.x) :0.0;

	xyPRF.x = xyStt.x + sw * m_dPRF_W / vAngJijum.y;
	xyPRF.y = xyStt.y + sw * m_dPRF_W / vAngJijum.y * dGiulJong;
	
	tv.m_v1 = xyStt;
	tv.m_v2 = xyPRF;
	strInfo.Format(_T("%s부상방지저판하단"), strStt);
	tv.m_sInfo = strInfo;
	tvArr.Add(tv); //w
	if(bAddTvArr)
	{
		m_tvArrVertSection.Add(tv);
		m_tvArrBriSect.Add(tv);
	}

	tv.m_v1 = xyPRF;
	tv.m_v2 = xyPRF + m_dPRF_H1 * CDPoint(0,1);
	strInfo.Format(_T("%s부상방지저판바깥측면"), strStt);
	tv.m_sInfo = strInfo;
	tvArr.Add(tv); //H1
	if(bAddTvArr)
	{
		m_tvArrVertSection.Add(tv);
		m_tvArrBriSect.Add(tv);
	}

	tv.m_v1 = xyPRF + m_dPRF_H1 * CDPoint(0,1);
	tv.m_v2 = xyStt + (m_dPRF_H1 + m_dPRF_H2) * CDPoint(0,1);
	strInfo.Format(_T("%s부상방지저판상면"), strStt);
	tv.m_sInfo = strInfo;
	tvArr.Add(tv); //H2
	if(bAddTvArr)
	{
		m_tvArrVertSection.Add(tv);
		m_tvArrBriSect.Add(tv);
	}

	tv.m_v1 = xyStt + (m_dPRF_H1 + m_dPRF_H2) * CDPoint(0,1);
	tv.m_v2 = xyStt;
	strInfo.Format(_T("%s부상방지저판안쪽측면"), strStt);
	tv.m_sInfo = strInfo;
	tvArr.Add(tv);
	if(bAddTvArr)
	{
		m_tvArrVertSection.Add(tv);
	}
}

void CRcBridgeApp::GetTvCrossHunch(CTwinVectorArray& tvArr, double sta, CDPoint vAng, BOOL bDrawSlopPattern, long nDrawLeft, BOOL bMirror, BOOL bExp, CDPoint vAngWall, BOOL bCalHeightHunch, double dHeightHunch)
{
	if(!IsExp(sta))
		bExp	= FALSE;

	CTwinVector tv;

//	double dTH	= bCalHeightHunch ? GetHeightUpperSlab_OldOrNotUse(sta, vAngWall) - m_dTS : dHeightHunch;
	double dTH	= bCalHeightHunch ? GetHeightUpperSlab(GetJijumNumberByStation(sta), 0, vAngWall) - m_dTS : dHeightHunch;
	if(dTH <= 0) return;
	int nTerm	= bDrawSlopPattern ? 4 : 1;
	double dSpace = dTH/nTerm;
	CDPointArray xyArr;
	GetPointArraySlabHunchAct(sta, vAng, xyArr, bExp);
	long nQtyHDan = xyArr.GetSize();

	for(int n=1; n<nTerm+1; n++)
	{
		int i=0; for(i=0; i<nQtyHDan-1; i++)
		{
			tv.m_v1 = CDPoint(xyArr.GetAt(i).x, xyArr.GetAt(i).y-dSpace*n);
			tv.m_v2 = CDPoint(xyArr.GetAt(i+1).x, xyArr.GetAt(i+1).y-dSpace*n);
			tv.m_sInfo.Format("헌치하면%d", i+1);
			tvArr.Add(tv);
		}
	}

	tv.m_v1 = CDPoint(xyArr.GetAt(0).x, xyArr.GetAt(0).y);
	tv.m_v2 = CDPoint(xyArr.GetAt(0).x, xyArr.GetAt(0).y-dSpace*nTerm);
	tv.m_sInfo	= "헌치좌측";
	tvArr.Add(tv);	// 좌측단
	tv.m_v1 = CDPoint(xyArr.GetAt(nQtyHDan-1).x, xyArr.GetAt(nQtyHDan-1).y);
	tv.m_v2 = CDPoint(xyArr.GetAt(nQtyHDan-1).x, xyArr.GetAt(nQtyHDan-1).y-dSpace*nTerm);
	tv.m_sInfo	= "헌치우측";
	tvArr.Add(tv);	// 우측단

	// mirror함
	CRebarPlacing rb;
	if(bMirror)
	{
		CTwinVectorArray tvArrCross;
		GetTvCrossSection(tvArrCross, sta, vAng, FALSE, TRUE, FALSE, 0, FALSE);
		CDRect rect	= tvArrCross.GetRect();
		rb.Mirror(tvArr, CDPoint(rect.left, 0), TRUE);
		rb.MoveRebar(tvArr, CDPoint(rect.Width(), 0));
	}

	// nDrawLeft에 따라서 잘라냄
	if(nDrawLeft != 0)
	{
		BOOL bVert	= Compare(vAng.GetAngleDegree(), 90.0, "=");
		CRebarPlacing rb;
		CDRect rect	= tvArr.GetRect();
		double dMid	= GetMidCrossSection(bVert);
		BOOL bLeft	= nDrawLeft == -1 ? FALSE : TRUE;
		CDPoint xy1	= CDPoint(dMid, rect.bottom-100);
		CDPoint xy2	= CDPoint(dMid, rect.top+100);
		rb.TrimTvArrayByLine(tvArr, xy1, xy2, bLeft, TRUE, FALSE);
	}
}

void CRcBridgeApp::GetPointArraySlabLowerAct(double sta, CDPoint vAng, CDPointArray& xyArr, BOOL bExp)
{
	xyArr.RemoveAll();
	CDPoint A[30];

	// 슬래브 상단
	GetXySlabUpperAct(sta, vAng, A, FALSE, bExp);

	double dWidth = bExp ? GetWidthSlabActByGagak(sta, vAng) : GetWidthSlabAct(sta, vAng);
	double dDisLeft = bExp ? GetDisSlabLeftActByGagak(sta, vAng) : GetDisSlabLeftAct(sta, vAng);
	double dLeftFromCen = dDisLeft + m_dCantilB[0];
	double dRightFromCen = dWidth + dDisLeft - m_dCantilB[1];
	CDPoint xyLLower = GetXySlabUpperActByDistAngle(sta, dLeftFromCen, vAng, FALSE, bExp);
	CDPoint xyRLower = GetXySlabUpperActByDistAngle(sta, dRightFromCen, vAng, FALSE, bExp);
	xyLLower.y -= m_dTS;
	xyRLower.y -= m_dTS;
	long nLIndex = GetNumHDanByDist(sta, vAng, m_dCantilB[0]);
	long nRIndex = GetNumHDanByDist(sta, vAng, m_dCantilB[1], TRUE);
	long nQtyHDanActLower = nRIndex - nLIndex + 2;
	int i=0; for(i=0; i<nQtyHDanActLower+1; i++)
	{
		if(i==0)
			xyArr.Add(xyLLower);
		else if(i==nQtyHDanActLower)
			xyArr.Add(xyRLower);
		else
			xyArr.Add(CDPoint(A[i+nLIndex-1].x, A[i+nLIndex-1].y-m_dTS));
	}
}

// CTwinVector CRcBridgeApp::GetTvExpansionEnd(BOOL bStt, BOOL bLeft)
// {
// 	CTwinVector tv;
// 	CTwinVectorArray tvArr;
// 
// 	GetTvPlaneSttEndLine(tvArr, bStt, TRUE);
// 	int nSize = tvArr.GetSize();
// 
// 	CDPoint xyEnd = bLeft ? tvArr.GetAt(0).m_v1 : tvArr.GetAt(nSize-1).m_v2;	// 슬래브 끝단 꼭지점
// 	CDPoint vAng = (tvArr.GetAt(nSize-1).m_v2-tvArr.GetAt(0).m_v1).Unit();
// 
// 	int nLeft = !bLeft;
// 	int se = (int)(bStt ? -1.0 : 1.0);
// 	int lr = (int)(bLeft ? -1.0 : 1.0);
// 	double dDistSlab = bLeft ? m_dWidthSlabLeft : m_dWidthSlabRight;
// 	double dExpH = bStt ? m_dExpSttH[nLeft]*lr : m_dExpEndH[nLeft]*lr;
// 	double dExpW = bStt ? m_dExpSttW[nLeft] : -m_dExpEndW[nLeft];
// 	double dExpR = bStt ? m_dExpSttR[nLeft] : m_dExpEndR[nLeft];
// 	
// 	CLineInfo* pLine = GetLineBase();
// 	double dMatchSta = 0;
// 	pLine->GetStationMatchByXyAndAngle(xyEnd, vAng, dMatchSta, dDistSlab+dExpH);
// 	CDPoint xy1 = pLine->GetXyLineDis(dMatchSta, dDistSlab+dExpH);
// 
// 	pLine->GetStationMatchByXyAndAngle(xyEnd, vAng, dMatchSta, dDistSlab);
// 	double dExpWSta = pLine->GetStationByDisLength(dMatchSta, dExpW, dDistSlab);
// 	CDPoint xy2 = pLine->GetXyLineDis(dExpWSta, dDistSlab);
// 
// 	tv.m_v1 = se*lr>0 ? xy1 : xy2;
// 	tv.m_v2 = se*lr>0 ? xy2 : xy1;
// 	tv.m_v1.z = dExpR>0 ? 1 : 0;
// 	tv.m_v2.z = dExpR;
// 
// 	return tv;
// }

// 동바리 좌표 (브라켓부)
void CRcBridgeApp::GetTvPostBracket(CTwinVectorArray &tvArr, BOOL bStt)
{
	tvArr.RemoveAll();
	if(!IsOutWall(bStt)) return;

	CFootingApp *pFooting	= bStt ? &m_footingStt : &m_footingEnd;
	CString sFooting		= bStt ? _T("기초상면좌측") : _T("기초상면우측");
	CString sBracket		= bStt ? _T("시점브라켓헌치") : _T("종점브라켓헌치");	

	CTwinVector tvFooting	= pFooting->m_tvArrFront.GetTvByInfo(sFooting);
	CTwinVector tvBracket	= m_tvArrVertSection.GetTvByInfo(sBracket);
	if(Compare(tvBracket.GetHorLength(), 0.0, "<=")) return;

	double dYLower = 0;
	if(m_nTypePostBaseLine == 0 && IsOutWall(TRUE))	// 기초상단 기준
	{
		if(IsBoxType())
		{
			dYLower = m_dELSB;
		}
		else
		{
			dYLower = tvFooting.GetXyTop().y;
		}
	}
	if(m_nTypePostBaseLine == 1)	// 지반고 기준
	{
		CTwinVector tvLowerFooting	= pFooting->m_tvArrFront.GetTvByInfo("기초하면");
		GetLineBase()->GetElevationErathOnStation(tvLowerFooting.GetXyMid().x, dYLower);
	}
	if(m_nTypePostBaseLine == 2)	// 사용자 입력
	{
		dYLower = frM(m_dPostLineEL);
	}
	
	CTwinVector tv;
	tv.m_v1	= tvBracket.m_v1;
	tv.m_v2	= tvBracket.m_v2;
	tvArr.Add(tv);

	if(m_nTypePostBaseLine == 0 && IsBoxType() && m_bExistProtectionRiseFooting)
	{
		double dYU = m_dELSB + (m_dPRF_H1 + m_dPRF_H2);
		double dYL = dYU - (m_dPRF_H2 * tvBracket.GetHorLength() / m_dPRF_W);
		tv.m_v1	= tv.m_v2;
		tv.m_v2	= CDPoint(tvBracket.m_v2.x, bStt? dYL : dYU);
		tvArr.Add(tv);

		tv.m_v1	= tv.m_v2;
		tv.m_v2	= CDPoint(tvBracket.m_v1.x, bStt? dYU : dYL);
		tvArr.Add(tv);
	}
	else
	{
		tv.m_v1	= tv.m_v2;
		tv.m_v2	= CDPoint(tvBracket.m_v2.x, dYLower);
		tvArr.Add(tv);

		tv.m_v1	= tv.m_v2;
		tv.m_v2	= CDPoint(tvBracket.m_v1.x, dYLower);
		tvArr.Add(tv);
	}

	tv.m_v1	= tv.m_v2;
	tv.m_v2	= tvBracket.m_v1;
	tvArr.Add(tv);
}

// 각 지간별 동바리 좌표 구함
// tvArrUpper	: 육교용 동바리에서 상부 목재동바리
// tvArrLeft	: 육교용 동바리에서 좌측 강관동바리
// tvArrRight	: 육교용 동바리에서 우측 강관동바리
// tvArrMid		: 육교용에서 상,좌,우 동바리를 뺀 부분이나, 일반용에서 지간 전체
// tvArrArch	: 헌지가 R인경우
// tvArrMid2	: 아치교에서 지간 사이 동바리
BOOL CRcBridgeApp::GetTvPostJigan(CTwinVectorArray &tvArrUpper, CTwinVectorArray &tvArrLeft, CTwinVectorArray &tvArrRight, CTwinVectorArray &tvArrMid, CTwinVectorArray &tvArrArch, CTwinVectorArray &tvArrMid2, long nJigan, CDPoint &LeftEL, CDPoint &RightEL)
{
	CRebarPlacing rb;

	// 일단 지간을 전체를 구함.
	// 헌치와 상면만 구하고 나머지는 직접 구함.
	// 좌우측 헌치를 구한뒤 수직으로 내림.
	// 슬래브교도 어차피 헌치밑에 교각이 들어갈 것이므로 다른것과 똑같이 구하면 됨.	
	CTwinVector tvHunchL, tvHunchR, tvLowerHunchL, tvLowerHunchR;
	BOOL bBoxType = (m_nTypeBridge == BRIDGE_TYPE_BOX)? TRUE : FALSE;

	// 좌측 헌치
	if(nJigan == 0)
	{
		if(!m_tvArrVertSection.GetTvByInfo("시점벽체헌치", tvHunchL))
		{
			m_tvArrVertSection.GetTvByInfo("시점슬래브측면", tvHunchL);
			tvHunchL.m_v1	= tvHunchL.GetXyBottom();
			tvHunchL.m_v2	= tvHunchL.GetXyBottom();
		}
		if(bBoxType)
		{
			if(!m_tvArrVertSection.GetTvByInfo("시점벽체하부헌치", tvLowerHunchL))
			{
				CTwinVector tvWallRight =m_tvArrVertSection.GetTvByInfo(_T("시점벽체우측면"));
				tvLowerHunchL.m_v1 = tvLowerHunchL.m_v2 = tvWallRight.GetXyBottom();
			}
		}
	}
	else
	{
		long nIdxHunch	= GetIdxHunchInWall(nJigan-1, FALSE);
		CString sHunch	= _T("");
		sHunch.Format("%d번째내측헌치우측면", nIdxHunch+1);
		tvHunchL	= m_tvArrVertSection.GetTvByInfo(sHunch);
		
		if(tvHunchL.m_v1.z != 0 && tvHunchL.m_v2.z != 0)	// 헌치가 R 인 경우
			tvArrArch.Add(tvHunchL);

		if(bBoxType)
		{
			sHunch.Format("%d번째내측하부헌치우측면", nIdxHunch+1);
			if(!m_tvArrVertSection.GetTvByInfo(sHunch, tvLowerHunchL))
			{
				CWallApp *pWall = GetInWall(nJigan-1);
				CTwinVector tvWallRight = pWall->m_tvArrFront.GetTvByInfo(_T("벽체우측면"));
				tvLowerHunchL.m_v1 = tvLowerHunchL.m_v2 = tvWallRight.GetXyBottom();
			}
		}
	}

	// 우측 헌치
	if(nJigan == m_nQtyJigan-1)
	{
		if(!m_tvArrVertSection.GetTvByInfo("종점벽체헌치", tvHunchR))
		{
			m_tvArrVertSection.GetTvByInfo("종점슬래브측면", tvHunchR);
			tvHunchR.m_v1	= tvHunchR.GetXyBottom();
			tvHunchR.m_v2	= tvHunchR.GetXyBottom();
		}
		if(bBoxType)
		{
			if(!m_tvArrVertSection.GetTvByInfo("종점벽체하부헌치", tvLowerHunchR))
			{
				CTwinVector tvWallLeft =m_tvArrVertSection.GetTvByInfo(_T("종점벽체좌측면"));
				tvLowerHunchR.m_v1 = tvLowerHunchR.m_v2 = tvWallLeft.GetXyBottom();
			}
		}
	}
	else
	{
		long nIdxHunch	= GetIdxHunchInWall(nJigan, TRUE);
		CString sHunch	= _T("");
		sHunch.Format("%d번째내측헌치좌측면", nIdxHunch+1);
		tvHunchR	= m_tvArrVertSection.GetTvByInfo(sHunch);
		
		if(tvHunchR.m_v1.z != 0 && tvHunchR.m_v2.z != 0)	// 헌치가 R 인 경우
			tvArrArch.Add(tvHunchR);

		if(bBoxType)
		{
			sHunch.Format("%d번째내측하부헌치좌측면", nIdxHunch+1);
			if(!m_tvArrVertSection.GetTvByInfo(sHunch, tvLowerHunchR))
			{
				CWallApp *pWall = GetInWall(nJigan);
				CTwinVector tvWallLeft = pWall->m_tvArrFront.GetTvByInfo(_T("벽체좌측면"));
				tvLowerHunchL.m_v1 = tvLowerHunchL.m_v2 = tvWallLeft.GetXyBottom();
			}
		}
	}
	
	// 좌우측 하단 EL을 찾는다.
	BOOL bPostLine = FALSE;	// 동바리 기준이 기초 상단 이상일 경우 TRUE
	double dYLowerL=0, dYLowerR=0;
	CLineInfo *pLine = GetLineBase();

	// 좌측 하단 EL
	pLine->GetElevationErathOnStation(tvHunchL.GetXyLeft().x, dYLowerL);	// 지반고
	if(m_nTypePostBaseLine == 0 && bBoxType)
	{
		dYLowerL = tvLowerHunchL.GetXyBottom().y;
		LeftEL	 = CDPoint(m_tvArrVertSection.GetXyLeft().x, dYLowerL);
		bPostLine = TRUE;
	}
	else if(m_nTypePostBaseLine == 2)	// 사용자 입력
	{
		dYLowerL  = frM(m_dPostLineEL);
	}
	else if(nJigan == 0)
	{
		if(m_nTypePostBaseLine == 0 && IsOutWall(TRUE))	// 기초상단 기준
		{
			CTwinVector tvFooting;
			BOOL bTv = m_footingStt.m_tvArrFront.GetTvByInfo("기초상면우측", tvFooting);
			if(!bTv)	tvFooting = m_footingStt.m_tvArrFront.GetTvByInfo("기초상면");		// 경사교 일때
			dYLowerL = tvFooting.GetXyLeft().y;
			LeftEL	 = m_nTypeBridge == BRIDGE_TYPE_TRAP2 ? CDPoint(tvFooting.GetXyRight().x, dYLowerL) : CDPoint(tvFooting.GetXyLeft().x, dYLowerL);
		
			bPostLine = TRUE;
		}
	}
	else
	{
		CWallApp *pWall = GetInWall(nJigan-1);
		if(pWall)
		{
			if(m_nTypePostBaseLine == 0 && pWall->m_footing.m_bIs)	// 기초상단 기준
			{
				CTwinVector tvFooting;
				BOOL bTv = pWall->m_footing.m_tvArrFront.GetTvByInfo("기초상면우측", tvFooting);
				if(!bTv) tvFooting = pWall->m_footing.m_tvArrFront.GetTvByInfo("기초상면");		// 경사교 일때
				dYLowerL = tvFooting.GetXyLeft().y;
				LeftEL	 = CDPoint(tvFooting.GetXyLeft().x, dYLowerL);
				bPostLine = TRUE;
			}
			else if(m_nTypeBridge == BRIDGE_TYPE_TRAP2 && m_nTypePostBaseLine == 0 && !pWall->m_footing.m_bIs)	// 경사교2
			{
				CTwinVector tvFooting;
				BOOL bTv = m_footingEnd.m_tvArrFront.GetTvByInfo("기초상면우측", tvFooting);
				if(!bTv) tvFooting = m_footingEnd.m_tvArrFront.GetTvByInfo("기초상면");		// 경사교 일때
				dYLowerL = tvFooting.GetXyLeft().y;
				LeftEL	 = CDPoint(tvFooting.GetXyLeft().x, dYLowerL);
				bPostLine = TRUE;
			}
		}
	}
	
	// 우측 하단 EL
	pLine->GetElevationErathOnStation(tvHunchR.GetXyRight().x, dYLowerR); // 지반고
	if(m_nTypePostBaseLine == 0 && bBoxType)
	{
		dYLowerR = tvLowerHunchR.GetXyBottom().y;
		RightEL	 = CDPoint(m_tvArrVertSection.GetXyRight().x, dYLowerR);
		bPostLine = TRUE;
	}
	else if(nJigan == m_nQtyJigan-1)
	{
		if(m_nTypePostBaseLine == 0 && IsOutWall(FALSE))	// 기초상단 기준
		{
			CTwinVector tvFooting;
			BOOL bTv = m_footingEnd.m_tvArrFront.GetTvByInfo("기초상면좌측", tvFooting);
			if(!bTv) tvFooting = m_footingEnd.m_tvArrFront.GetTvByInfo("기초상면");		// 경사교 일때
			dYLowerR = tvFooting.GetXyRight().y;
			RightEL	 = m_nTypeBridge == BRIDGE_TYPE_TRAP2 ? CDPoint(tvFooting.GetXyLeft().x, dYLowerR) : CDPoint(tvFooting.GetXyRight().x, dYLowerR);
			bPostLine = TRUE;	// 기준선이 기초상단 위라면 간단하게 사각형 도형을 만들수 있다.
		}
		else if(m_nTypePostBaseLine == 2)	// 사용자 입력
		{
			dYLowerR  = frM(m_dPostLineEL);
		}
		
		if(!IsOutWall(FALSE))	// 경사교 일때 종점측에 벽체가 없으면 왼쪽 기초 상단으로 맞춘다.
			dYLowerR = dYLowerL;
	}
	else
	{
		CWallApp *pWall	= GetInWall(nJigan);
		if(pWall)
		{
			if(m_nTypePostBaseLine == 0 && pWall->m_footing.m_bIs)	// 기초상단 기준
			{
				CTwinVector tvFooting;
				BOOL bTv = pWall->m_footing.m_tvArrFront.GetTvByInfo("기초상면좌측", tvFooting);
				if(!bTv) tvFooting = pWall->m_footing.m_tvArrFront.GetTvByInfo("기초상면");		// 경사교 일때
				dYLowerR = tvFooting.GetXyRight().y;
				RightEL	 = CDPoint(tvFooting.GetXyRight().x, dYLowerR);
				bPostLine = TRUE;	// 기준선이 기초상단 위라면 간단하게 사각형 도형을 만들수 있다.
			}
			else if(m_nTypePostBaseLine == 2)	// 사용자 입력
			{
				dYLowerR  = frM(m_dPostLineEL);
			}
			else if(m_nTypeBridge == BRIDGE_TYPE_TRAP2 && m_nTypePostBaseLine == 0 && !pWall->m_footing.m_bIs)	// 경사교2		
			{
				CTwinVector tvFooting;
				BOOL bTv = pWall->m_footing.m_tvArrFront.GetTvByInfo("기초상면좌측", tvFooting);
				if(!bTv) tvFooting = pWall->m_footing.m_tvArrFront.GetTvByInfo("기초상면");		// 경사교 일때
				dYLowerR = tvFooting.GetXyRight().y;
				RightEL	 = CDPoint(tvFooting.GetXyRight().x, dYLowerR);
				bPostLine = TRUE;	// 기준선이 기초상단 위라면 간단하게 사각형 도형을 만들수 있다.
			}

			if(nJigan==0 && !IsOutWall(TRUE))	// 경사교 일때 시점측에 벽체가 없으면 오른쪽 기초 상단으로 맞춘다.
				dYLowerL = dYLowerR;
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 좌우측 헌치와 좌우측 EL을 구했으므로 지간 전체 영역을 구함.
	tvArrMid.RemoveAll();
	tvArrMid2.RemoveAll();
	CTwinVector tv;
	BOOL bFirst = FALSE;
	CDPoint xyFirst, xyResult;
		
	if(m_nTypeBridge == BRIDGE_TYPE_ARCH)
		GetRibPost(tvArrMid);			// 아치교에서 리브 아래부분 동바리 영역
	else
	{
		// 좌측
		if(m_nTypeBridge == BRIDGE_TYPE_TRAP || m_nTypeBridge == BRIDGE_TYPE_TRAP2)
		{
			CWallApp *pWall = GetInWall(nJigan-1);
			if(pWall)
			{
				CDPoint xyLine;
				if(bPostLine)
				{
					xyLine = CDPoint(tvHunchL.GetXyLeft().x, dYLowerL);
					CTwinVector tvWallRight = pWall->m_tvArrFront.GetTvByInfo(_T("벽체우측면"));
					GetXyMatchSegAndLine(tvWallRight.m_v1, tvWallRight.m_v2, xyLine, CDPoint(1,0), xyResult);
					
					bFirst  = TRUE;
					tv.m_v1 = xyFirst = xyResult;
					tv.m_v2	= tvHunchL.GetXyLeft();
					tv.m_sInfo	= "좌측면";
					tvArrMid.Add(tv);
				}
				else
				{
// 					BOOL bMatch = FALSE;

					CTwinVector tvFootingRight = pWall->m_footing.m_tvArrFront.GetTvByInfo(_T("기초우측면"));
					xyLine = CDPoint(tvFootingRight.m_v1.x, dYLowerR);
					BOOL bMatch = GetXyMatchSegAndLine(tvFootingRight.m_v1, tvFootingRight.m_v2, xyLine, CDPoint(1,0), xyResult);
					if(bMatch)
					{
						tv.m_v1 = xyResult;
						tv.m_v2 = tvFootingRight.GetXyTop();
						tv.m_sInfo	= "좌측면";
						tvArrMid.Add(tv);
						if(!bFirst)
						{
							bFirst  = TRUE;
							xyFirst = xyResult;
						}
					}

					if(nJigan%2 == 0)		// 홀수 지간은 기초 상면과는 상관없다
					{
						CTwinVector tvFootingUpper;
						BOOL bIs = pWall->m_footing.m_tvArrFront.GetTvByInfo(_T("기초상면"), tvFootingUpper);
						if(bIs && tvFootingUpper.GetXyTop().y > dYLowerL)
						{
							tv.m_v1	= tv.m_v2;
							tv.m_v2 = tvFootingUpper.GetXyLeft();
							tv.m_sInfo	= "좌측면";
							tvArrMid.Add(tv);
						}
					}

					CTwinVector tvFootingSlope = pWall->m_footing.m_tvArrFront.GetTvByInfo(_T("기초경사우측면"));
					xyLine = CDPoint(tvFootingSlope.GetXyBottom().x, dYLowerL);
					bMatch = GetXyMatchSegAndLine(tvFootingSlope.m_v1, tvFootingSlope.m_v2, xyLine, CDPoint(1,0), xyResult);
					BOOL bMatchSlope = bMatch;
					if(bMatch)
					{
						if(nJigan%2 == 0)
						{
							tv.m_v1 = tvFootingSlope.GetXyTop();
							tv.m_v2 = xyResult;
						}
						else
						{
							tv.m_v1 = xyResult;
							tv.m_v2 = tvFootingSlope.GetXyTop();
						}
						if(tvFootingSlope.GetXyTop().y > dYLowerL)
						{
							tv.m_sInfo	= "좌측면";
							tvArrMid.Add(tv);
							if(!bFirst)
							{
								bFirst  = TRUE;
								xyFirst = xyResult;
							}
						}
					}
					else
					{
						if(tvFootingSlope.GetXyTop().y > dYLowerL)
						{
							tv.m_v1	= tv.m_v2;
							tv.m_v2 = (nJigan%2 == 1) ? tvFootingSlope.GetXyTop() : tvFootingSlope.GetXyBottom();
							tv.m_sInfo	= "좌측면";
							tvArrMid.Add(tv);
						}
					}

					CTwinVector tvWallLeft = pWall->m_tvArrFront.GetTvByInfo(_T("벽체우측면"));
					xyLine = CDPoint(tvWallLeft.GetXyTop().x, dYLowerL);
					bMatch = GetXyMatchSegAndLine(tvWallLeft.m_v1, tvWallLeft.m_v2, xyLine, CDPoint(1,0), xyResult);
					if(bMatch)
					{
						if(bMatchSlope && nJigan%2 == 0)
						{
							tv.m_v1 = tv.m_v2;
							tv.m_v2 = xyResult;
							tv.m_sInfo	= "좌측면";
							tvArrMid.Add(tv);
						}

						tv.m_v1 = xyResult;
						tv.m_v2 = tvWallLeft.GetXyTop();
						if(!bFirst)
						{
//							bFirst  = TRUE;
							xyFirst = xyResult;
						}
					}
					else
					{
						tv.m_v1	= tv.m_v2;
						tv.m_v2 = tvWallLeft.GetXyTop();
					}
					tv.m_sInfo	= "좌측면";
					tvArrMid.Add(tv);
				}
			}
			else
			{
				BOOL bMatch = FALSE;
				CDPoint xyLine(0,0);
				CTwinVector tvLowerHunch;
				if(m_tvArrVertSection.GetTvByInfo("시점벽체하부헌치", tvLowerHunch))
				{
					if(tvLowerHunch.GetXyTop().y > dYLowerL)
					{
						xyLine = CDPoint(tvLowerHunch.GetXyTop().x, dYLowerL);
						bMatch = GetXyMatchSegAndLine(tvLowerHunch.m_v1, tvLowerHunch.m_v2, xyLine, CDPoint(1,0), xyResult);
					
// 						bFirst  = TRUE;
						tv.m_v1	= xyFirst = bMatch ? xyResult : CDPoint(tvLowerHunch.GetXyBottom().x, dYLowerL);
						tv.m_v2	= tvLowerHunch.GetXyLeft();
						tv.m_sInfo	= "좌측면";
						tvArrMid.Add(tv);

						tv.m_v1	= tv.m_v2;
						tv.m_v2	= tvHunchL.GetXyLeft();
						tv.m_sInfo	= "좌측면";
						tvArrMid.Add(tv);
					}
					else
					{
						xyLine = CDPoint(tvHunchL.GetXyTop().x-1, dYLowerL);	//  약간 빼준다..혹시 Match 안될까봐
						bMatch = GetXyMatchSegAndLine(tvHunchL.m_v1, tvHunchL.m_v2, xyLine, CDPoint(1,0), xyResult);
						
//						bFirst  = TRUE;
						tv.m_v1	= xyFirst = bMatch ? xyResult : CDPoint(tvHunchL.GetXyBottom().x, dYLowerL);
						tv.m_v2	= tvHunchL.GetXyLeft();
						tv.m_sInfo	= "좌측면";
						tvArrMid.Add(tv);
					}
				}
				else
				{
					tv.m_v1	= xyFirst = CDPoint(tvHunchL.GetXyLeft().x, dYLowerL);
					tv.m_v2	= tvHunchL.GetXyLeft();
					tv.m_sInfo	= "좌측면";
					tvArrMid.Add(tv);
				}
			}
		}
		else
		{
			tv.m_v1	= xyFirst = CDPoint(tvHunchL.GetXyLeft().x, dYLowerL);
			if(IsBoxType() && tvLowerHunchL.GetLength() > 0) tv.m_v1.y = tvLowerHunchL.GetXyTop().y;
			tv.m_v2	= m_nTypeBridge == BRIDGE_TYPE_SLAB ? tvHunchL.GetXyTop() : tvHunchL.GetXyLeft();
			tv.m_sInfo	= "좌측면";
			tvArrMid.Add(tv);
		}

		tv.m_v1	= tv.m_v2;
		tv.m_v2	= m_nTypeBridge == BRIDGE_TYPE_SLAB ? tvHunchL.GetXyTop() : tvHunchL.GetXyRight();
		tv.m_sInfo	= "좌측헌치";
		tvArrMid.Add(tv);

		tv.m_v1	= tv.m_v2;
		tv.m_v2	= tvHunchR.GetXyLeft();
		tv.m_sInfo	= "상면";
		tvArrMid.Add(tv);

		tv.m_v1	= tv.m_v2;
		tv.m_v2	= m_nTypeBridge == BRIDGE_TYPE_SLAB ? tvHunchR.GetXyTop() : tvHunchR.GetXyRight();
		tv.m_sInfo	= "우측헌치";
		tvArrMid.Add(tv);
		
		//우측
		if(m_nTypeBridge == BRIDGE_TYPE_TRAP || m_nTypeBridge == BRIDGE_TYPE_TRAP2)
		{
			CWallApp *pWall	= GetInWall(nJigan);
			if(pWall)
			{
				CDPoint xyLine;
				if(bPostLine)
				{
					CTwinVector tvWallLeft  = pWall->m_tvArrFront.GetTvByInfo(_T("벽체좌측면"));
					xyLine = CDPoint(tvWallLeft.m_v1.x, dYLowerR);
					GetXyMatchSegAndLine(tvWallLeft.m_v1, tvWallLeft.m_v2, xyLine, CDPoint(1,0), xyResult);
					tv.m_v1	= tv.m_v2;
					tv.m_v2 = xyResult; 
					tv.m_sInfo	= "우측면";
					tvArrMid.Add(tv);
				}
				else
				{
// 					BOOL bMatch = FALSE;

					CTwinVector tvWallLeft = pWall->m_tvArrFront.GetTvByInfo(_T("벽체좌측면"));
					xyLine = CDPoint(tvWallLeft.GetXyBottom().x, dYLowerR);
					BOOL bMatch = GetXyMatchSegAndLine(tvWallLeft.m_v1, tvWallLeft.m_v2, xyLine, CDPoint(1,0), xyResult);
					if(bMatch)
					{
						tv.m_v1	= tv.m_v2;
						tv.m_v2 = xyResult;
					}
					else
					{
						tv.m_v1	= tv.m_v2;
						tv.m_v2 = tvWallLeft.m_v1;
					}
					tv.m_sInfo	= "우측면";
					tvArrMid.Add(tv);

					CTwinVector tvFootingSlope = pWall->m_footing.m_tvArrFront.GetTvByInfo(_T("기초경사좌측면"));
					xyLine = CDPoint(tvFootingSlope.GetXyTop().x, dYLowerR);
					bMatch = GetXyMatchSegAndLine(tvFootingSlope.m_v1, tvFootingSlope.m_v2, xyLine, CDPoint(1,0), xyResult);
					if(bMatch)
					{
						if(nJigan%2 == 0)
						{
							tv.m_v1 = tv.m_v2;
							tv.m_v2 = xyResult;
							tv.m_sInfo	= "우측면";
							tvArrMid.Add(tv);
						}

						tv.m_v1 = tv.m_v2;
						tv.m_v2 = nJigan%2==0 ? tvFootingSlope.GetXyTop() : xyResult;
						tv.m_sInfo	= "우측면";
						if(tvFootingSlope.GetXyTop().y > dYLowerR)
							tvArrMid.Add(tv);
					}
					else
					{
						if(tvFootingSlope.GetXyTop().y > dYLowerR)
						{	
							tv.m_v1	= tv.m_v2;
							tv.m_v2 = (nJigan%2 == 1) ? tvFootingSlope.GetXyBottom() : tvFootingSlope.GetXyTop();
							tv.m_sInfo	= "우측면";
							tvArrMid.Add(tv);
						}
					}

					if(nJigan%2 == 0)
					{
						CTwinVector tvFootingUpper;
						BOOL bIs = pWall->m_footing.m_tvArrFront.GetTvByInfo(_T("기초상면"), tvFootingUpper);
						if(bIs && tvFootingUpper.GetXyTop().y > dYLowerR)
						{
							tv.m_v1	= tv.m_v2;
							tv.m_v2 = tvFootingUpper.GetXyLeft();
							tv.m_sInfo	= "우측면";
							tvArrMid.Add(tv);
						}
					}

					CTwinVector tvFootingLeft = pWall->m_footing.m_tvArrFront.GetTvByInfo(_T("기초좌측면"));
					xyLine = CDPoint(tvFootingLeft.m_v1.x, dYLowerR);
					bMatch = GetXyMatchSegAndLine(tvFootingLeft.m_v1, tvFootingLeft.m_v2, xyLine, CDPoint(1,0), xyResult);
					if(bMatch)
					{
						tv.m_v1	= tv.m_v2;
						tv.m_v2 = xyResult;
						tv.m_sInfo	= "우측면";
						tvArrMid.Add(tv);
					}
				}
			}
			else
			{
				BOOL bMatch = FALSE;
				CDPoint xyLine(0,0);
				CTwinVector tvLowerHunch;
				if(m_tvArrVertSection.GetTvByInfo("종점벽체하부헌치", tvLowerHunch))
				{
					if(tvLowerHunch.GetXyTop().y > dYLowerR)
					{
						tv.m_v1	= tv.m_v2;
						tv.m_v2	= tvLowerHunch.GetXyTop();
						tv.m_sInfo	= "좌측면";
						tvArrMid.Add(tv);
						
						xyLine = CDPoint(tvLowerHunch.GetXyBottom().x, dYLowerR);
						bMatch = GetXyMatchSegAndLine(tvLowerHunch.m_v1, tvLowerHunch.m_v2, xyLine, CDPoint(1,0), xyResult);

						tv.m_v1	= tv.m_v2;
						tv.m_v2	= bMatch ? CDPoint(xyResult.x, dYLowerR) : xyLine;
						tv.m_sInfo	= "좌측면";
						tvArrMid.Add(tv);
					}
					else
					{
						xyLine = CDPoint(tvLowerHunch.GetXyBottom().x-1, dYLowerR);		//  약깐 빼준다..혹시 Match 안될까봐
						bMatch = GetXyMatchSegAndLine(tv.m_v2, CDPoint(tv.m_v2.x, dYLowerR), xyLine, CDPoint(1,0), xyResult);

						tv.m_v1	= tv.m_v2;
						tv.m_v2	= bMatch ? xyResult : CDPoint(tv.m_v1.x, dYLowerR);
						tv.m_sInfo	= "좌측면";
						tvArrMid.Add(tv);
					}
				}
				else
				{
					tv.m_v1	= tv.m_v2;
					tv.m_v2	= CDPoint(tv.m_v1.x, dYLowerR);
					tv.m_sInfo	= "우측면";
					tvArrMid.Add(tv);
				}
			}
		}
		else
		{
			tv.m_v1	= tv.m_v2;
			tv.m_v2	= CDPoint(tv.m_v1.x, dYLowerR);
			if(IsBoxType() && tvLowerHunchR.GetLength() > 0) tv.m_v2.y = tvLowerHunchR.GetXyTop().y;
			tv.m_sInfo	= "우측면";
			tvArrMid.Add(tv);	
		}

		if(bBoxType)
		{
			if(bPostLine)
			{
				if(tvLowerHunchL.GetLength() > 0)
				{
					tv	= tvLowerHunchL;
					tv.m_sInfo	= "좌측면";
					swap(tv.m_v1, tv.m_v2);
					tvArrMid.InsertAt(0, tv);
				}

				if(tvLowerHunchR.GetLength() > 0)
				{
					tv	= tvLowerHunchR;
					tv.m_sInfo	= "우측면";
					swap(tv.m_v1, tv.m_v2);
					tvArrMid.Add(tv);
				}

				tv.m_v1	= tvLowerHunchL.m_v2;
				tv.m_v2	= tvLowerHunchR.m_v1;
				tv.m_sInfo	= "하면";
				tvArrMid.Add(tv);
			}
			else
			{
				tv.m_v1	= CDPoint(min(tv.m_v2.x, xyFirst.x), dYLowerL);
				tv.m_v2	= CDPoint(max(tv.m_v2.x, xyFirst.x), dYLowerR);
				tv.m_sInfo	= "하면";
				tvArrMid.Add(tv);
			}
		}
		else
		{
			tv.m_v1	= CDPoint(min(tv.m_v2.x, xyFirst.x), dYLowerL);
			tv.m_v2	= CDPoint(max(tv.m_v2.x, xyFirst.x), dYLowerR);
			tv.m_sInfo	= "하면";
			tvArrMid.Add(tv);
		}
	}
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if(m_nTypeBridge == BRIDGE_TYPE_ARCH)	// 아치교에서 지간 동바리.
		GetArchJiganPost(tvArrMid2, nJigan);
	
	if(m_nTypePost != TYPE_POST_BRIDGE) return TRUE;
	// 육교용이라면 좌,우,상면을 offset해서 강관과 목재 동바리로 나눈다.
	tvArrLeft.RemoveAll();
	tvArrUpper.RemoveAll();
	tvArrRight.RemoveAll();
	
	// 육교 동바리 라인 4개
	CTwinVector tvLower = tvArrMid.GetTvByInfo("하면");	// 하면은 한 라인이다.
	CTwinVectorArray tvArrOverPass;
	CDPoint xyMid = tvLower.GetXyMid();
	CTwinVector tvOverPass;
	tvOverPass.m_v1 = CDPoint(xyMid.x-4000, xyMid.y);
	tvOverPass.m_v2 = CDPoint(xyMid.x-4000, xyMid.y+4000);
	tvArrOverPass.Add(tvOverPass);
	tvOverPass.m_v1 = CDPoint(xyMid.x-4000, xyMid.y+4000);
	tvOverPass.m_v2 = CDPoint(xyMid.x+4000, xyMid.y+4000);
	tvArrOverPass.Add(tvOverPass);
	tvOverPass.m_v1 = CDPoint(xyMid.x+4000, xyMid.y+4000);
	tvOverPass.m_v2 = CDPoint(xyMid.x+4000, xyMid.y);
	tvArrOverPass.Add(tvOverPass);
	tvOverPass.m_v1 = CDPoint(xyMid.x+4000, xyMid.y);
	tvOverPass.m_v2 = CDPoint(xyMid.x-4000, xyMid.y);
	tvArrOverPass.Add(tvOverPass);

	// 육교용 동바리 설치 가능한지 검사	
	long nSize = tvArrMid.GetSize();
	long i=0; for(i=0; i<nSize-1; i++)	// 하면은 검사 하지 않음
	{
		CTwinVector tvOut = tvArrMid.GetAt(i);
		if(tvOut.m_sInfo == _T("하면")) continue;

		for(long j=0; j<tvArrOverPass.GetSize()-1; j++)		// 하면은 검사 하지 않음
		{
			CDPoint xyResult;
			CTwinVector tvIn = tvArrOverPass.GetAt(j);
			
			BOOL bMatch = GetXyMatchSegAndSeg(tvOut.m_v1,tvOut.m_v2,tvIn.m_v1,tvIn.m_v2,xyResult);
			if(bMatch) 
				return TRUE;		// 육교용 동바리를 설치 할 수 없다.
		}
	}
	
	// 상부와 육교용 동바리와의 교점을 찾기 위해 연장할 선
	const long nExtend = 1000000; 
	CTwinVector tvLeft = tvArrOverPass.GetAt(0);
	tvLeft.m_v1 = CDPoint(tvLeft.m_v1.x, tvLeft.m_v1.y-nExtend);
	tvLeft.m_v2 = CDPoint(tvLeft.m_v2.x, tvLeft.m_v2.y+nExtend);
	CTwinVector tvRight = tvArrOverPass.GetAt(2);
	tvRight.m_v1 = CDPoint(tvRight.m_v1.x, tvRight.m_v1.y+nExtend);
	tvRight.m_v2 = CDPoint(tvRight.m_v2.x, tvRight.m_v2.y-nExtend);
	
	// 각 영역을 찾는다.
	BOOL bLeft = FALSE;
	for(i=0; i<nSize; i++)
	{
		CDPoint xyResult;
		// Left
		CTwinVector tvOut = tvArrMid.GetAt(i);
		if(!bLeft)
		{
			BOOL bMatch = GetXyMatchSegAndSeg(tvOut.m_v1,tvOut.m_v2,tvLeft.m_v1,tvLeft.m_v2,xyResult);
			if(!bMatch)
				tvArrLeft.Add(tvOut);
			else
			{
				tv.m_v1 = tvOut.m_v1;
				tv.m_v2 = xyResult;
				tvArrLeft.Add(tv);
						
				tv.m_v1 = xyResult;
				tv.m_v2 = tvArrOverPass.GetAt(0).m_v1;
				tvArrLeft.Add(tv);
				tv.m_v1 = tv.m_v2;
				tv.m_v2 = tvArrLeft.GetAt(0).m_v1;
				tvArrLeft.Add(tv);

				bLeft = TRUE;
				
				// Upper
				tv.m_v1 = tvArrOverPass.GetAt(0).m_v2;
				tv.m_v2 = xyResult;
				tvArrUpper.Add(tv);
			}
		}
		if(!bLeft)	continue;

		BOOL bMatch = GetXyMatchSegAndSeg(tvOut.m_v1,tvOut.m_v2,tvRight.m_v1,tvRight.m_v2,xyResult);
		if(!bMatch)
		{
			tv.m_v1 = tv.m_v2;
			tv.m_v2 = tvOut.m_v2;
			tv.m_v1.z = 0;	tv.m_v2.z = 0;
			if(tv.m_v1.x != tv.m_v2.x && tv.m_v1.y != tv.m_v2.y)
				tvArrUpper.Add(tv);
		}
		else
		{
			tv.m_v1 = tv.m_v2;
			tv.m_v2 = xyResult;
			tv.m_v1.z = 0;	tv.m_v2.z = 0;
			tvArrUpper.Add(tv);

			tv.m_v1 = tv.m_v2;
			tv.m_v2 = tvArrOverPass.GetAt(1).m_v2;
			tv.m_v1.z = 0;	tv.m_v2.z = 0;
			tvArrUpper.Add(tv);
			
			tv.m_v1 = tv.m_v2;
			tv.m_v2 = tvArrOverPass.GetAt(1).m_v1;
			tv.m_v1.z = 0;	tv.m_v2.z = 0;
			tvArrUpper.Add(tv);

			// Right
			tv.m_v1 = xyResult;
			tv.m_v2 = tvOut.m_v2;
			tvArrRight.Add(tv);
			long k=0; for(k=i+1; k<nSize-1; k++)
			{
				tv = tvArrMid.GetAt(k);
				tvArrRight.Add(tv);
			}
			tv.m_v1 = tv.m_v2;
			tv.m_v2 = tvArrOverPass.GetAt(2).m_v2;
			tvArrRight.Add(tv);
			tv.m_v1 = tv.m_v2;
			tv.m_v2 = xyResult;
			tvArrRight.Add(tv);

			// Mid
			tvArrMid.RemoveAll();
			tvArrMid = tvArrOverPass;
			break;
		}
	}

	return FALSE;
}

// 아치교에서 지간 동바리
void CRcBridgeApp::GetArchJiganPost(CTwinVectorArray &tvArrMid2, long nJigan)
{
	BOOL bIs = FALSE, bIsHunch = FALSE, bMatch = FALSE;
	CTwinVector tv, tvTemp;
	CWallApp *pWall = NULL;
	CString sHunch	= _T("");
	long nIdxHunch = 0;
	CDPoint xyResult;

	long nSize = m_tvArrVertSectionRib[0].GetSize();
	if(nSize<=0) return;

	CTwinVector tvStt = m_tvArrVertSectionRib[0].GetAt(0);
	CTwinVector tvEnd = m_tvArrVertSectionRib[0].GetAt(nSize-1);

	if(nJigan==0)		// 시점
	{
		// 리브가 기초 경사면에 있을 경우
		bIs = m_footingStt.m_tvArrFront.GetTvByInfo("기초우측경사", tvTemp);
		bMatch = GetXyMatchSegAndSeg(tvStt.m_v1, tvStt.m_v2, tvTemp.m_v1, tvTemp.m_v2, xyResult);
		if(bIs && bMatch)
		{
			tv.m_v1 = xyResult;
			tv.m_v2 = tvTemp.GetXyLeft();
			tvArrMid2.Add(tv);

			bIs = m_footingStt.m_tvArrFront.GetTvByInfo("기초상면우측", tvTemp);
			if(bIs)	
			{
				tv.m_v1	= tv.m_v2;
				bIs = m_tvArrVertSection.GetTvByInfo(_T("시점벽체하부헌치"), tvTemp);
				if(bIs)
					tv.m_v2	= tvTemp.GetXyBottom();
				else
				{
					bIs = m_tvArrVertSection.GetTvByInfo(_T("시점벽체우측면"), tvTemp);
					if(bIs)
						tv.m_v2	= tvTemp.GetXyBottom();
				}				
				tvArrMid2.Add(tv);
			}
		}
		else		// 리브가 기초 상면에 있을 경우
		{
			bIs = m_footingStt.m_tvArrFront.GetTvByInfo("기초상면우측", tvTemp);
			bMatch = GetXyMatchSegAndSeg(tvStt.m_v1, tvStt.m_v2, tvTemp.m_v1, tvTemp.m_v2, xyResult);
			if(bIs && bMatch)
			{
				tv.m_v1	= xyResult;
				bIs = m_tvArrVertSection.GetTvByInfo(_T("시점벽체하부헌치"), tvTemp);
				if(bIs)
					tv.m_v2	= tvTemp.GetXyBottom();
				else
				{
					bIs = m_tvArrVertSection.GetTvByInfo(_T("시점벽체우측면"), tvTemp);
					if(bIs)
						tv.m_v2	= tvTemp.GetXyBottom();
				}				
				tvArrMid2.Add(tv);
			}
		}
		//
		bIs = m_tvArrVertSection.GetTvByInfo(_T("시점벽체하부헌치"), tvTemp);
		if(bIs)
		{
			tv.m_v1	= tv.m_v2;
			tv.m_v2	= tvTemp.GetXyTop();
			tvArrMid2.Add(tv);
		}
		bIs = m_tvArrVertSection.GetTvByInfo(_T("시점벽체우측면"), tvTemp);
		if(bIs)
		{
			tv.m_v1	= tv.m_v2;
			tv.m_v2	= tvTemp.GetXyTop();
			tvArrMid2.Add(tv);
		}
		bIs = m_tvArrVertSection.GetTvByInfo(_T("시점벽체헌치"), tvTemp);
		if(bIs)
		{
			tv.m_v1	= tv.m_v2;
			tv.m_v2	= tvTemp.GetXyRight();
			tvArrMid2.Add(tv);
		}
		pWall = GetInWall(nJigan);
		if(pWall)
		{
			nIdxHunch	= GetIdxHunchInWall(nJigan, TRUE);
			sHunch.Format(_T("%d번째내측헌치좌측면"), nIdxHunch+1);
			bIsHunch = m_tvArrVertSection.GetTvByInfo(sHunch, tvTemp);
			if(bIsHunch)
			{
				tv.m_v1	= tv.m_v2;
				tv.m_v2	= tvTemp.GetXyLeft();
				tvArrMid2.Add(tv);

				tv.m_v1	= tv.m_v2;
				tv.m_v2	= tvTemp.GetXyRight();
				tvArrMid2.Add(tv);
			}
			bIs = pWall->m_tvArrFront.GetTvByInfo(_T("벽체좌측면"), tvTemp);
			if(bIs)
			{
				if(!bIsHunch)
				{
					tv.m_v1	= tv.m_v2;
					tv.m_v2	= tvTemp.GetXyTop();
					tvArrMid2.Add(tv);
				}
				tv.m_v1	= tv.m_v2;
				tv.m_v2	= tvTemp.GetXyBottom();
				tvArrMid2.Add(tv);
			}
		}
	}
	else
	{
		pWall = GetInWall(nJigan-1);
		if(pWall)
		{
			bIs = pWall->m_tvArrFront.GetTvByInfo(_T("벽체우측면"), tvTemp);
			if(bIs)
			{
				tv.m_v1	= tvTemp.GetXyBottom();
				tv.m_v2	= tvTemp.GetXyTop();
				tvArrMid2.Add(tv);
			}
			nIdxHunch	= GetIdxHunchInWall(nJigan-1, FALSE);
			
			sHunch.Format(_T("%d번째내측헌치우측면"), nIdxHunch+1);
			bIsHunch = m_tvArrVertSection.GetTvByInfo(sHunch, tvTemp);
			if(bIsHunch)
			{
				tv.m_v1	= tv.m_v2;
				tv.m_v2	= tvTemp.GetXyRight();
				tvArrMid2.Add(tv);
			}
		}
		if(nJigan != m_nQtyJigan-1)
		{
			pWall = GetInWall(nJigan);
			if(pWall)
			{
				nIdxHunch	= GetIdxHunchInWall(nJigan, TRUE);
				sHunch.Format(_T("%d번째내측헌치좌측면"), nIdxHunch+1);
				bIsHunch = m_tvArrVertSection.GetTvByInfo(sHunch, tvTemp);
				if(bIsHunch)
				{
					tv.m_v1	= tv.m_v2;
					tv.m_v2	= tvTemp.GetXyLeft();
					tvArrMid2.Add(tv);

					tv.m_v1	= tv.m_v2;
					tv.m_v2	= tvTemp.GetXyRight();
					tvArrMid2.Add(tv);
				}
				bIs = pWall->m_tvArrFront.GetTvByInfo(_T("벽체좌측면"), tvTemp);
				if(bIs)
				{
					if(!bIsHunch)
					{
						tv.m_v1	= tv.m_v2;
						tv.m_v2	= tvTemp.GetXyTop();
						tvArrMid2.Add(tv);
					}
					tv.m_v1	= tv.m_v2;
					tv.m_v2	= tvTemp.GetXyBottom();
					tvArrMid2.Add(tv);
				}
			}
		}
		else		// 종점
		{
			bIsHunch = m_tvArrVertSection.GetTvByInfo(_T("종점벽체헌치"), tvTemp);
			if(bIsHunch)
			{
				tv.m_v1	= tv.m_v2;
				tv.m_v2	= tvTemp.GetXyLeft();
				tvArrMid2.Add(tv);

				tv.m_v1	= tv.m_v2;
				tv.m_v2	= tvTemp.GetXyRight();
				tvArrMid2.Add(tv);
			}
			bIs = m_tvArrVertSection.GetTvByInfo(_T("종점벽체좌측면"), tvTemp);
			if(bIs)
			{
				if(!bIsHunch)
				{
					tv.m_v1	= tv.m_v2;
					tv.m_v2	= tvTemp.GetXyTop();
					tvArrMid2.Add(tv);
				}
				tv.m_v1	= tv.m_v2;
				tv.m_v2	= tvTemp.GetXyBottom();
				tvArrMid2.Add(tv);
			}
			bIs = m_tvArrVertSection.GetTvByInfo(_T("종점벽체하부헌치"), tvTemp);
			if(bIs)
			{
				tv.m_v1	= tv.m_v2;
				tv.m_v2	= tvTemp.GetXyBottom();
				tvArrMid2.Add(tv);
			}
			// 리브가 기초 상면에 있을 경우
			bIs = m_footingEnd.m_tvArrFront.GetTvByInfo("기초상면좌측", tvTemp);
			bMatch = GetXyMatchSegAndSeg(tvEnd.m_v1, tvEnd.m_v2, tvTemp.m_v1, tvTemp.m_v2, xyResult);
			if(bIs && bMatch)
			{
				tv.m_v1 = tv.m_v2;
				tv.m_v2 = xyResult;
				tvArrMid2.Add(tv);
			}
			else	// 리브가 기초 경사면에 있을 경우
			{
				bIs = m_footingEnd.m_tvArrFront.GetTvByInfo("기초상면좌측", tvTemp);
				if(bIs)
				{
					tv.m_v1 = tv.m_v2;
					tv.m_v2 = tvTemp.GetXyLeft();
					tvArrMid2.Add(tv);
				}
				bIs = m_footingEnd.m_tvArrFront.GetTvByInfo("기초좌측경사", tvTemp);
				GetXyMatchSegAndSeg(tvEnd.m_v1, tvEnd.m_v2, tvTemp.m_v1, tvTemp.m_v2, xyResult);
				if(bIs)
				{
					tv.m_v1 = tv.m_v2;
					tv.m_v2 = xyResult;
					tvArrMid2.Add(tv);
				}
			}
		}
	}
	CRebarPlacing rb;
	rb.RemoveMinLengthLine(tvArrMid2, 0);
	tvArrMid2.Sort();

	//
	CTwinVector tvRib;
	BOOL bStt = FALSE, bEnd = FALSE;
	CDPoint xyResult1, xyResult2;
	CDPoint vStt = tvArrMid2.GetAt(0).m_v1;
	CDPoint vEnd = tvArrMid2.GetAt(tvArrMid2.GetSize()-1).m_v2;

	CTwinVectorArray tvArrSecRib;
	tvArrSecRib	= m_tvArrVertSectionRib[0];
	double dXLeft	= min(vStt.x, vEnd.x);
	double dXRight	= max(vStt.x, vEnd.x);
	CDRect rect	= tvArrSecRib.GetRect();
	rb.TrimTvArrayByLine(tvArrSecRib, CDPoint(dXLeft, rect.bottom-1000), CDPoint(dXLeft, rect.top+1000), TRUE, FALSE, FALSE);
	rb.TrimTvArrayByLine(tvArrSecRib, CDPoint(dXRight, rect.bottom-1000), CDPoint(dXRight, rect.top+1000), FALSE, FALSE, FALSE);

	tvArrMid2.AddFromTvArray(tvArrSecRib);
	long i = 0;
	for(i = 0; i < tvArrMid2.GetSize(); i++)
	{
		CTwinVector *pTv	= NULL;
		pTv	= &tvArrMid2.GetAt(i);
		pTv->m_v1.z	= 0;
		pTv->m_v2.z	= 0;
	}
	tvArrMid2.Sort();
	if(tvArrMid2.GetSize() > 0)
	{
		CTwinVector *pTvStt	= &tvArrMid2.GetAt(0);
		CTwinVector *pTvEnd	= &tvArrMid2.GetAt(tvArrMid2.GetSize()-1);
		pTvEnd->m_v2	= pTvStt->m_v1;
	}


	return;
	//
	nSize = m_tvArrVertSectionRib[0].GetSize();
	for(i=nSize-1; i>=0; i--)
	{
		tvRib = m_tvArrVertSectionRib[0].GetAt(i);
		bStt = GetXyMatchSegAndLine(tvRib.m_v1, tvRib.m_v2, vEnd, CDPoint(0,1), xyResult1);
		
		if(bStt || bEnd)
		{
			tvRib = m_tvArrVertSectionRib[0].GetAt(i);
			bEnd = GetXyMatchSegAndLine(tvRib.m_v1, tvRib.m_v2, vStt, CDPoint(0,1), xyResult2);
			if(bEnd)
			{
				tv.m_v1	= tv.m_v2;
				tv.m_v2	= xyResult2;
				tvArrMid2.Add(tv);
				break;
			}
			else
			{
				bEnd = TRUE;
				tv.m_v1	= bStt ? vEnd : tv.m_v2;
				tv.m_v2	= tvRib.GetXyLeft();
				tvArrMid2.Add(tv);
			}
		}
	}
}

// 아치교에서 리브 아래부분 동바리 영역
void CRcBridgeApp::GetRibPost(CTwinVectorArray &tvArrMid)
{
	long nSize = m_tvArrVertSectionRib[1].GetSize();
	if(nSize<=0) return;

	CTwinVector tvStt = m_tvArrVertSectionRib[1].GetAt(0);
	CTwinVector tvEnd = m_tvArrVertSectionRib[1].GetAt(nSize-1);

	tvArrMid	= m_tvArrVertSectionRib[1];

	if(m_nTypePostBaseLine == 1 || m_nTypePostBaseLine == 2)
	{
		double dELLower	= 0;
		if(m_nTypePostBaseLine == 1)	// 지반고
		{
			CLineInfo *pLine = GetLineBase();
			double dYLowerL(0), dYLowerR(0);
			pLine->GetElevationErathOnStation(tvStt.m_v1.x, dYLowerL);
			pLine->GetElevationErathOnStation(tvEnd.m_v2.x, dYLowerR);
			dELLower	= min(dYLowerL, dYLowerR);
		}
		else	// 사용자 입력
		{
			dELLower = frM(m_dPostLineEL);
		}


		CDRect rect	= tvArrMid.GetRect();
		CRebarPlacing rb;
		rb.TrimTvArrayByLine(tvArrMid, CDPoint(rect.left - 100, dELLower), CDPoint(rect.right + 100, dELLower), FALSE, FALSE, FALSE);

		nSize	= m_tvArrVertSectionRib[1].GetSize();
		if(nSize < 3) return;

		tvStt = m_tvArrVertSectionRib[1].GetAt(0);
		tvEnd = m_tvArrVertSectionRib[1].GetAt(nSize-1);

		tvArrMid.RemoveAll();

		if(tvStt.m_v1.z > dELLower)
		{
			CTwinVector tv;
			tv.m_v1	= CVector(tvStt.m_v1.x, dELLower, 0);
			tv.m_v2	= tvStt.m_v2;
			tv.m_v2.z	= 0;
			tvArrMid.Add(tv);
		}
		tvArrMid	= m_tvArrVertSectionRib[1];

		if(tvEnd.m_v2.z > dELLower)
		{
			CTwinVector tv;
			tv.m_v1	= tvEnd.m_v2;
			tv.m_v2	= CVector(tvEnd.m_v2.x, dELLower, 0);
			tv.m_v1.z	= 0;
			tvArrMid.Add(tv);
		}
	}

	

	nSize	= tvArrMid.GetSize();
	if(nSize < 3) return;
	tvStt	= tvArrMid.GetAt(0);
	tvEnd	= tvArrMid.GetAt(nSize-1);
	
	CTwinVector tv;
	tv.m_v1	= tvEnd.m_v2;
	tv.m_v2	= tvStt.m_v1;
	tv.m_v1.z	= 0;
	tv.m_v2.z	= 0;
	tvArrMid.Add(tv);
	return;
	
	// 기초상단 기준
// 	BOOL /*bTv=FALSE, */bMatch=FALSE;
	CTwinVector tvFooting;
	// 리브가 기초 상면에 있을 경우
	BOOL bTv = m_footingStt.m_tvArrFront.GetTvByInfo("기초상면우측", tvFooting);
	BOOL bMatch = GetXyMatchSegAndSeg(tvStt.m_v1, tvStt.m_v2, tvFooting.m_v1, tvFooting.m_v2);
	if(bTv && bMatch)
	{
		bTv = m_footingStt.m_tvArrFront.GetTvByInfo("기초우측확장상면", tvFooting);
		if(bTv)	
		{
			tv = CTwinVector(tvFooting.GetXyRight(), tvFooting.GetXyLeft());
			tvArrMid.Add(tv);
		}
		bTv = m_footingStt.m_tvArrFront.GetTvByInfo("기초우측경사면", tvFooting);
		if(bTv)	
		{
			tv = CTwinVector(tvFooting.GetXyRight(), tvFooting.GetXyLeft());
			tvArrMid.Add(tv);
		}
	
		tv.m_v1 = tvFooting.GetXyLeft();
		tv.m_v2 = m_tvArrVertSectionRib[1].GetAt(0).m_v1;
		tvArrMid.Add(tv);

	}
	// 리브가 기초 경사면에 있을 경우
	bTv = m_footingStt.m_tvArrFront.GetTvByInfo("기초우측경사", tvFooting);
	bMatch = GetXyMatchSegAndSeg(tvStt.m_v1, tvStt.m_v2, tvFooting.m_v1, tvFooting.m_v2);
	if(bTv && bMatch)
	{
		bTv = m_footingStt.m_tvArrFront.GetTvByInfo("기초우측확장상면", tvFooting);
		if(bTv)	
		{
			tv = CTwinVector(tvFooting.GetXyRight(), tvFooting.GetXyLeft());
			tvArrMid.Add(tv);
		}

		tv.m_v1 = tvFooting.GetXyLeft();
		tv.m_v2 = m_tvArrVertSectionRib[1].GetAt(0).m_v1;
		tvArrMid.Add(tv);	
	}
	
	long i=0; for(i=0; i<nSize; i++)
	{
		tv = m_tvArrVertSectionRib[1].GetAt(i);
		tvArrMid.Add(tv);
	}

	// 리브가 기초 상면에 있을 경우
	bTv = m_footingEnd.m_tvArrFront.GetTvByInfo("기초상면좌측", tvFooting);
	bMatch = GetXyMatchSegAndSeg(tvEnd.m_v1, tvEnd.m_v2, tvFooting.m_v1, tvFooting.m_v2);
	if(bTv && bMatch)
	{
		tv.m_v1 = m_tvArrVertSectionRib[1].GetAt(nSize-1).m_v2;
		tv.m_v2 = tvFooting.GetXyLeft();
		tvArrMid.Add(tv);

		bTv = m_footingEnd.m_tvArrFront.GetTvByInfo("기초좌측경사", tvFooting);
		if(bTv)	
		{
			tv = CTwinVector(tvFooting.GetXyRight(), tvFooting.GetXyLeft());
			tvArrMid.Add(tv);
		}
		bTv = m_footingEnd.m_tvArrFront.GetTvByInfo("기초좌측확장상면", tvFooting);
		if(bTv)	
		{
			tv = CTwinVector(tvFooting.GetXyRight(), tvFooting.GetXyLeft());
			tvArrMid.Add(tv);
		}
	}
	// 리브가 기초 경사면에 있을 경우
	bTv = m_footingEnd.m_tvArrFront.GetTvByInfo("기초좌측경사", tvFooting);
	bMatch = GetXyMatchSegAndSeg(tvEnd.m_v1, tvEnd.m_v2, tvFooting.m_v1, tvFooting.m_v2);
	if(bTv && bMatch)
	{
		tv.m_v1 = m_tvArrVertSectionRib[1].GetAt(nSize-1).m_v2;
		tv.m_v2 = tvFooting.GetXyLeft();
		tvArrMid.Add(tv);

		bTv = m_footingEnd.m_tvArrFront.GetTvByInfo("기초좌측확장상면", tvFooting);
		if(bTv)	
		{
			tv = CTwinVector(tvFooting.GetXyRight(), tvFooting.GetXyLeft());
			tvArrMid.Add(tv);
		}	
	}

	long nMidSize = tvArrMid.GetSize();
	tv.m_v1 = tvArrMid.GetAt(nMidSize-1).m_v2;
	tv.m_v2 = tvArrMid.GetAt(0).m_v1;
	tv.m_sInfo	= "하면";
	tvArrMid.Add(tv);

	// 지반고 || 사용자 입력
	if(m_nTypePostBaseLine == 1 || m_nTypePostBaseLine == 2)
	{
		
		CDPoint vStt, vEnd, vOrg;
		if(m_nTypePostBaseLine == 1)	// 지반고
		{
			double dYLowerL=0, dYLowerR=0;
			CLineInfo *pLine = GetLineBase();
			pLine->GetElevationErathOnStation(tvStt.m_v1.x, dYLowerL);
			pLine->GetElevationErathOnStation(tvEnd.m_v2.x, dYLowerR);
			vStt = CDPoint(tvStt.m_v1.x, dYLowerL);
			vEnd = CDPoint(tvEnd.m_v2.x, dYLowerR);
		}
		else	// 사용자 입력
		{
			vOrg = m_footingEnd.m_tvArrFront.GetXyLeft();
			vOrg = CDPoint(vOrg.x, frM(m_dPostLineEL));
		}

		CDPoint xyResult;
		CTwinVectorArray tvArr;
		BOOL bCheck = FALSE;
		long n=0; for(n=0; n<nMidSize; n++)
		{
			tv.m_v1 = tvArrMid.GetAt(n).m_v1;
			tv.m_v2 = tvArrMid.GetAt(n).m_v2;
			
			if(m_nTypePostBaseLine == 1)		// 지반고
				bMatch = GetXyMatchSegAndSeg(tv.m_v1, tv.m_v2, vStt, vEnd, xyResult);
			else	// 사용자 입력
				bMatch = GetXyMatchSegAndLine(tv.m_v1, tv.m_v2, vOrg, CDPoint(1,0), xyResult);

			if(bMatch)
			{
				bCheck = !bCheck;
				tv = bCheck ? CTwinVector(xyResult, tv.m_v2) : CTwinVector(tv.m_v1, xyResult);
				if(!bCheck)
					tvArr.Add(tv);
			}
			if(bCheck)
				tvArr.Add(tv);
		}

		tvArrMid.RemoveAll();
		tvArrMid = tvArr;
		nMidSize = tvArrMid.GetSize();
		if(nMidSize > 0)
		{
			tv.m_v1 = tvArrMid.GetAt(nMidSize-1).m_v2;
			tv.m_v2 = tvArrMid.GetAt(0).m_v1;
			tv.m_sInfo	= "하면";
			tvArrMid.Add(tv);
		}
	}
}

// 피복 기본값
// : 교량설계기준,한국도로공사 P45/29 참조
// : 단, 직경별로 피복값이 다른 경우도 있으나, 고려하지 않음(큰값으로 적용)
void CRcBridgeApp::SetDataDefaultCover()
{
	m_dCoverUpper_UpperSlab[0]	= 80;	// 상부슬래브 상면(지상노출시70)
	m_dCoverLower_UpperSlab[0]	= 60;	// 상부슬래브 하면
	m_dCoverSide_UpperSlab[0]	= 60;	// 상부슬래브 측면
	m_dCoverUpper_LowerSlab[0]	= 60;	// 상부슬래브 상면(지상노출시70)
	m_dCoverLower_LowerSlab[0]	= 80;	// 상부슬래브 하면
	m_dCoverWallOut[0]			= 80;	// 벽체외면
	m_dCoverWallIn[0]			= 80;	// 벽체 내면
	m_dCoverWallMid[0]			= 80;	// 중간벽체
	m_dCoverColumn[0]			= 80;	// 기둥
	m_dCoverWingOut[0]			= 50;	// 날개벽 외면(노출면50(70), 지중면 80) ()는 무늬거푸집 사용시)
	m_dCoverWingIn[0]			= 80;	// 날개벽 내면(노출면50(70), 지중면 80) ()는 무늬거푸집 사용시)
	m_dCoverWingUpper			= 80;	// 날개벽 상면(노출면50(70), 지중면 80) ()는 무늬거푸집 사용시) 
	m_dCoverWingEnd				= 80;	// 날개벽 끝단(노출면50(70), 지중면 80) ()는 무늬거푸집 사용시)
	m_dCoverUpper_Footing[0]	= 100;	// 기초 상면
	m_dCoverLower_Footing_Lean[0]	= 100;	// 기초 하면(직접기초시)
	m_dCoverLower_Footing_Pile[0]	= 150;	// 기초 하면(말뚝기초시)
	m_dCoverRib[iUPPER][0]		= m_dCoverLower_UpperSlab[0];
	m_dCoverRib[iLOWER][0]		= m_dCoverLower_UpperSlab[0];
	m_dCoverSide_Footing[0]		=100; // 기초 측면
}

// void CRcBridgeApp::SetDataDefaultRebarSymbol()
// {
// }

void CRcBridgeApp::SetDataDefaultBM(long nType)
{
	

	// 수량선택사항 -------------------------------------------------------
	if(nType == 0)
	{
		m_nFormType_ConcNormal				= 2;			// 거푸집 : 일반콘크리트
		m_nFormType_Footing					= 1;			// 거푸집 : 기초
		m_nFormType_WallFront				= 2;			// 거푸집 : 벽체 전면
		m_nFormType_WallBack				= 2;			// 거푸집 : 벽체 후면
		long stt(0), left(0);
		for(stt = 0; stt < 2; stt++)
		{
			for(left = 0; left < 2; left++)
			{
				m_nFormType_WingFront[stt][left]	= 2;	// 거푸집 : 날개벽 전면
				m_nFormType_WingBack[stt][left]		= 2;	// 거푸집 : 날개벽 배면
			}
		}
		m_nFormType_Aps						= 2;			// 거푸집 : 접속슬래브
		m_nFormType_MassConc				= 0;			// 거푸집 : mass 콘크리트
		m_nFormType_LeanConc				= 0;			// 거푸집 : 버림 콘크리트
	}
	else
	{
		m_bSelectBM_ConcCare_Main			= FALSE;		// 콘크리트양생 : 교대본체, 날개벽
		m_bSelectBM_ConcCare_Aps			= TRUE;			// 콘크리트양생 : 접속슬래브, 완충슬래브
		m_nSelectBM_ConcCare_Cal			= 0;			// 콘크리트양생 산출방법 ( 0 : 기초를 제외한 거푸집수량+상부수평면, 1 : 기초를 포함한 수평면)
		m_nTypeRcBridgeBlock				= 0;			// 교대보호블럭 설치
		m_dAngleRcBridgeBlock				= 15;			// 교대보호블럭 외측설치각도(도)
		m_bSelectBM_ApsCnsInclude			= TRUE;			// 수량집계에 접속슬래브, 완충슬래브 포함
		m_dPavementThick					= 400;			// 뒷채움시 콘크리트 포장두께
		m_dNosangThick						= 1000;			// 뒷채움시 노상두께
		m_nTypePost							= 0;			// 동바리 타입(육교용, 일반용)
		m_nTypeHeightWall					= 1;			// 수량 산출시 벽체 기준 높이(0 : 도로중심 높이, 1 : 평균높이((최대+최소)/2), 2 : 최대높이
		m_nTypeSurfaceProof					= !m_bUnderGroundRahmen || m_nTypeBridge == BRIDGE_TYPE_SLAB ? 0 : 1;			// 교면방수 타입(0 : 침투식, 1 : 쉬트방수) - 권고안 -> 토피없는라멘,슬래브교는 침투식, 지중라멘교는 쉬트방수
		m_nTypePostBaseLine					= 0;			// 동바리 기준선(0:기초 상단 1:지반선 2:사용자 입력) 
		m_dPostLineEL						= 0;			// 사용자 입력 동바리 기준선 EL(M)
		m_dThickPostUpper					= 1200;			// 육교용 동바리 상부 두께
		m_dThickPostSide					= 1200;			// 육교용 동바리 좌우측 두께
		m_dThickScaffold					= 1200;			// 비계 두께

		m_nDeductNearFormBM					= 0;			// 인접구조물 거푸집 수량 공제(0 : 안함, 1 : 좌측공제, 2 : 우측공제)
		m_nDeductNearTupagiBM				= 0;			// 인접구조물 터파기 수량 공제(0 : 안함, 1 : 좌측공제, 2 : 우측공제)

		m_dAggregate_Footing				= 25;			// 콘크리트 골재최대치수(기초)
		m_dAggregate_Main					= 25;			// 콘크리트 골재최대치수(본체)
		m_dAggregate_Aps					= 25;			// 콘크리트 골재최대치수(접속슬래브)
		m_dAggregate_Lean					= 40;			// 콘크리트 골재최대치수(버림)
		m_dAggregate_Mass					= 40;			// 콘크리트 골재최대치수(MASS)
		m_dAggregate_Hyunta					= 25;			// 콘크리트 골재최대치수(현장타설말뚝)
		m_dAggregate_Etc					= 19;			// 콘크리트 골재최대치수(방호벽등)

		m_dSlump_Footing					= 15;			// 콘크리트 Slump(기초)
		m_dSlump_Main						= 15;			// 콘크리트 Slump(본체)
		m_dSlump_Aps						= 8;			// 콘크리트 Slump(접속슬래브)
		m_dSlump_Lean						= 8;			// 콘크리트 Slump(버림)
		m_dSlump_Mass						= 8;			// 콘크리트 Slump(MASS)
		m_dSlump_Hyunta						= 8;			// 콘크리트 Slump(현장타설말뚝)
		m_dSlump_Etc						= 15;			// 콘크리트 Slump(방호벽등)


		m_fxJointStt.m_sName	= "UCF-50 S";
		long i = 0; for(i = 0; i < 2; i++)
		{
			m_fxJointStt.m_rb[i].m_dDia	= 0;
			m_fxJointStt.m_rb[i].m_dLengthTotal	= 0.12;
			m_fxJointStt.m_rb[0].m_nEa	= 1;
		}

		m_fxJointEnd.m_sName	= "UCF-80 S";
		for(i = 0; i < 2; i++)
		{
			m_fxJointEnd.m_rb[i].m_dDia	= 0;
			m_fxJointEnd.m_rb[i].m_dLengthTotal	= 0.12;
			m_fxJointEnd.m_rb[i].m_nEa	= 1;
		}

		m_nMonumenu						= 4;				// 교명주 받침 설치
		m_nDesPanel							= 2;				// 설명판 설치개수
		m_nNamePanel						= 2;				// 교명판 설치개수
		m_dTackCotting						= 1.5;			// 택 코팅 (ARCBRIDGE-1570) 2.0 -> 1.5 변경요청
		m_nTypePileMilkBM				= 0;
		m_bExcludeApsDowelRebar	= FALSE;
		m_dTimeConcForm					= 120;
		m_bApplyYongSu						= FALSE;
		m_dSurplusSoil_HyunTaPile		= 100;		
		m_dConcreteSoil_HyunTaPile	= 8;		
		m_dConcreteRock_HyunTaPile	= 14;

		InitAggregateAndSlump();
	}
	// ------------------------------------------------------ 수량선택사항
}


void CRcBridgeApp::SetDataDefaultCondition()
{
	m_dUnitWeigthBackFill		=  2.0;
	m_dUnitWeigthBackFillWater	=  1.0;
	m_dAngleFrictionBackFill	=  35;
	m_dLiveLoadAlways			= (m_pARcBridgeDataStd->IsSI() && m_pARcBridgeDataStd->IsRailDesign()) ? 3.50 :  1.00;
	m_dDeadLoadAlways			= (m_pARcBridgeDataStd->IsSI() && m_pARcBridgeDataStd->IsRailDesign()) ? 1.50 :  0.00;
	m_nSelectLiveLoadHori       = m_bUnderGroundRahmen ? 1 : 0;
	m_dLiveLoadFootway			= 0.500;
	m_dUnitWeightHeapSoil		= 2.0;
	m_dUnitWeightValidDepth		= 2.0;
	m_dValidDepthBaseGround		= 1.0;
	m_dEo_CalcWallSpringEQ		= 28*27; // N=27 <- 35 = 15 + √(15N)
	m_nTypeToapSafeCheck		= 0;
	m_SafeFactorOverturning[0]	=  2.0;
	m_SafeFactorOverturning[1]	=  1.5;
	m_SafeFactorSliding[0]		=  1.5;
	m_SafeFactorSliding[1]		=  1.2;
	m_nSelectModelingCountEQ	= IsBoxType() ? 0 : 2;
	m_nSelectPileCalcSpringEQ	= 0;
	m_nSelect2DFrameTool		= SOLVE_SAP_6AND7;
	m_nSelect3DShellTool		= 2;
	m_nSelect3DOutputValue		= 0;
	m_dCoffKh_Sei				= 100;

	m_dFckMassConc	= 210;
	m_dFckLeanConc	= 150;

	BOOL bLSD = m_pARcBridgeDataStd->IsLsdDesign();
	double uWC = (m_pARcBridgeDataStd->m_pBasicConcInfo->m_UWeightConcrete>0) ? m_pARcBridgeDataStd->m_pBasicConcInfo->m_UWeightConcrete/1000 : 2.350;

	for (long ix =0; ix < ePartCount; ix++)
	{
		m_Fck[ix] = bLSD? 270 : 240;
		m_Fy[ix]  = bLSD? 4000 : 3000;
		m_Fvy[ix] = bLSD? 4000 : 3000;
		m_Ec[ix] = GetValueEcConc(m_Fck[ix], uWC, m_pARcBridgeDataStd->IsSI(), m_pARcBridgeDataStd->m_nTypeKciCode); // 도로교설계기준 38페이지;
	}

// 	if (bLSD)
// 	{
// 		m_Fy[ePartWingWallStt]	= 270;
// 		m_Fy[ePartApsStt]		= 270;
// 		m_Fy[ePartWingWallEnd]	= 270;
// 		m_Fy[ePartApsEnd]		= 270;
// 
// 		m_Fvy[ePartWingWallStt]	= 270;
// 		m_Fvy[ePartApsStt]		= 270;
// 		m_Fvy[ePartWingWallEnd]	= 270;
// 		m_Fvy[ePartApsEnd]		= 270;
// 
// 		m_Ec[ePartWingWallStt]	= 270;
// 		m_Ec[ePartApsStt]		= 270;
// 		m_Ec[ePartWingWallEnd]	= 270;
// 		m_Ec[ePartApsEnd]		= 270;
// 	}

	m_bMaterialDetail = FALSE;
	m_dRateDryAndShrinkage = 0.00015;
	m_dCoefficientExpansion = 0.000010;
	m_bIsDBar_ForDowelBar = TRUE;
	m_nTypeDesingEQ = 1;
	m_nDegreeEarthQuake = 0;
	m_nRegionEarthQuake = 0;			
	m_dCoefficientGround = 1.0;			
	m_gValue = 9.810;					
	m_nTypeRigidFootingEQ = 0;			
	m_bApplyRigidWallEQRotate = FALSE;	
	m_nTypeJointPosFootingEQ = 0;		
	m_nTypePropertyPosFootingEQ = 0;	
	m_nTypeSoilPressureEQ = 0;			
	m_nTypeUpperWeightEQ = 0;		
	m_nSelectWingLoad_LS_ES = bLSD? 0 : 1;
}

//#define BRIDGE_LOAD_DEAD			0 // 고정하중
//#define BRIDGE_LOAD_LIVE_FULL		1 // 활하중
//#define BRIDGE_LOAD_TOAP_BOTH		2 // 수평토압,수평수압
//#define BRIDGE_LOAD_TEMP1			3 // 온도하중(+)
//#define BRIDGE_LOAD_TEMP2			4 // 온도하중(-)
//#define BRIDGE_LOAD_SHRINK		5 // 건조수축
//#define BRIDGE_LOAD_SUBSID		6 // 지점침하
//#define BRIDGE_LOAD_BASIC			7 // 기본하중의 개수
void CRcBridgeApp::SetDataDefaultCombineUltLoad()
{
	// 3D 적용, 지하수
	long i=0; for(i=0; i<MAX_RCBRIDGE_COMBO; i++)
	{
		m_bApplyCombo3D[COMBO_ULTIMATE][i] = (i <= 4) ? TRUE : FALSE;
	}

	SetCalcVertLoadFactor();
	// 하중조합 권고안
	if(m_pARcBridgeDataStd->m_nTypeConditionApply==DESIGN_CONDITION_CONCRETE)
	{
		if(m_pARcBridgeDataStd->m_nTypeKciCode == 0) // 2003
		{
			double dFactor[320] = {
				1.540,	1.540,	1.700,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.540,	1.540,	0.000,	1.700,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.540,	1.540,	1.700,	0.000,	0.000,	1.800,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.540,	1.540,	0.000,	1.700,	0.000,	1.800,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.540,	1.540,	1.700,	0.000,	0.000,	0.000,	1.800,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.540,	1.540,	0.000,	1.700,	0.000,	0.000,	1.800,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.540,	1.540,	0.000,	0.000,	0.000,	1.800,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.540,	1.540,	0.000,	0.000,	0.000,	0.000,	1.800,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.540,	1.540,	0.000,	0.000,	0.000,	0.000,	0.000,	1.500,	0.000,	1.500,	1.500,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.540,	1.540,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	1.500,	1.500,	1.500,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.155,	1.155,	1.275,	0.000,	0.000,	0.000,	0.000,	1.125,	0.000,	1.125,	1.125,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.155,	1.155,	1.275,	0.000,	0.000,	0.000,	0.000,	0.000,	1.125,	1.125,	1.125,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.155,	1.155,	0.000,	1.275,	0.000,	0.000,	0.000,	1.125,	0.000,	1.125,	1.125,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.155,	1.155,	0.000,	1.275,	0.000,	0.000,	0.000,	0.000,	1.125,	1.125,	1.125,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.540,	1.540,	1.700,	0.000,	0.000,	0.900,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.540,	1.540,	0.000,	1.700,	0.000,	0.900,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.540,	1.540,	1.700,	0.000,	0.000,	0.000,	0.900,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.540,	1.540,	0.000,	1.700,	0.000,	0.000,	0.900,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.540,	1.540,	0.000,	0.000,	0.000,	0.900,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.540,	1.540,	0.000,	0.000,	0.000,	0.000,	0.900,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000, 	0.000,	0.000
			};
			long nIdxFator = 0;
			long nSizeRow  = 20;
			m_MatComboUlt.Resize(nSizeRow,BRIDGE_LOAD_BASIC);
			for(long nRow=0; nRow<nSizeRow; nRow++)
			{
				for(long nCol=0; nCol<BRIDGE_LOAD_BASIC; nCol++)
				{
					m_MatComboUlt(nRow,nCol) = dFactor[nIdxFator++];
					if(m_bApplyLoad[nCol]==FALSE) m_MatComboUlt(nRow,nCol) = 0;
				}
			}
		}
		else if (m_pARcBridgeDataStd->m_nTypeKciCode == 1) // 2007
		{
			double dFactor[288] = {
				1.40, 	1.40, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 
				1.20, 	1.60,	1.60, 	0.00, 	0.00, 	1.60, 	0.00, 	1.20, 	0.00, 	1.20, 	1.20, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 
				1.20, 	1.60,	1.60, 	0.00, 	0.00, 	0.00, 	1.60, 	1.20, 	0.00, 	1.20, 	1.20, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 
				1.20, 	1.60,	1.60, 	0.00, 	0.00, 	1.60, 	0.00, 	0.00, 	1.20, 	1.20, 	1.20, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 
				1.20, 	1.60,	1.60, 	0.00, 	0.00, 	0.00, 	1.60, 	0.00, 	1.20, 	1.20, 	1.20, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 
				1.20, 	0.00, 	1.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 
				1.20, 	1.60,	1.60, 	0.00, 	0.00, 	0.80, 	0.00, 	1.20, 	0.00, 	1.20, 	1.20, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 
				1.20, 	1.60,	1.60, 	0.00, 	0.00, 	0.00, 	0.80, 	1.20, 	0.00, 	1.20, 	1.20, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 
				1.20, 	1.60,	1.60, 	0.00, 	0.00, 	0.80, 	0.00, 	0.00, 	1.20, 	1.20, 	1.20, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 
				1.20, 	1.60,	1.60, 	0.00, 	0.00, 	0.00, 	0.80, 	0.00, 	1.20, 	1.20, 	1.20, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 
				0.90, 	1.60,	0.00, 	0.00, 	0.00, 	1.60, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 
				0.90, 	1.60,	0.00, 	0.00, 	0.00, 	0.00, 	1.60, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 
				1.40, 	1.40, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	1.40, 
				1.20, 	1.60,	1.60, 	0.00, 	0.00, 	1.60, 	0.00, 	1.20, 	0.00, 	1.20, 	1.20, 	0.00, 	0.00, 	0.00, 	1.60, 	1.60,
				1.20, 	1.60,	1.60, 	0.00, 	0.00, 	1.60, 	0.00, 	0.00, 	1.20, 	1.20, 	1.20, 	0.00, 	0.00, 	0.00, 	1.60, 	1.60,
				1.20, 	1.60,	1.60, 	0.00, 	0.00, 	0.80, 	0.00, 	1.20, 	0.00, 	1.20, 	1.20, 	0.00, 	0.00, 	0.00, 	0.80, 	1.60,
				1.20, 	1.60,	1.60, 	0.00, 	0.00, 	0.80, 	0.00, 	0.00, 	1.20, 	1.20, 	1.20, 	0.00, 	0.00, 	0.00, 	0.80, 	1.60,
				0.90, 	1.60,	0.00, 	0.00, 	0.00, 	1.60, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	1.60, 	1.60
			};		
			long nIdxFator = 0;
			long nSizeRow  = 18;
			m_MatComboUlt.Resize(nSizeRow,BRIDGE_LOAD_BASIC);
			for(long nRow=0; nRow<nSizeRow; nRow++)
			{
				for(long nCol=0; nCol<BRIDGE_LOAD_BASIC; nCol++)
				{
					if(nCol == BRIDGE_LOAD_SOIL) // 연직토압
					{
						if(nRow != 0 && nRow != 5 && nRow != 12)
						{
							m_MatComboUlt(nRow,nCol) = dFactor[nIdxFator++] * m_dFactorComboVLoad;
						}
						else
						{
							m_MatComboUlt(nRow,nCol) = dFactor[nIdxFator++];
						}
					}
					else if(nCol == BRIDGE_LOAD_WATERVERT) // 연직수압
					{
						if(nRow > 13)
						{
							m_MatComboUlt(nRow,nCol) = dFactor[nIdxFator++] * m_dFactorComboVLoad;
						}
						else
						{
							m_MatComboUlt(nRow,nCol) = dFactor[nIdxFator++];
						}
					}
					else
					{
						m_MatComboUlt(nRow,nCol) = dFactor[nIdxFator++];
					}
					if(m_bApplyLoad[nCol]==FALSE) m_MatComboUlt(nRow,nCol) = 0;
				}
			}
		}
		else // 2012
		{
			double dFactor[352] = {
				1.40, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00,
				1.20, 	1.60,	1.60, 	0.00, 	0.00, 	1.60, 	0.00, 	1.20, 	0.00, 	1.20, 	1.20, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00,
				1.20, 	1.60,	1.60, 	0.00, 	0.00, 	0.00, 	1.60, 	1.20, 	0.00, 	1.20, 	1.20, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00,
				1.20, 	1.60,	1.60, 	0.00, 	0.00, 	1.60, 	0.00, 	0.00, 	1.20, 	1.20, 	1.20, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 
				1.20, 	1.60,	1.60, 	0.00, 	0.00, 	0.00, 	1.60, 	0.00, 	1.20, 	1.20, 	1.20, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 
				1.20, 	0.00, 	1.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 
				1.20, 	1.60,	1.60, 	0.00, 	0.00, 	0.80, 	0.00, 	1.20, 	0.00, 	1.20, 	1.20, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 
				1.20, 	1.60,	1.60, 	0.00, 	0.00, 	0.00, 	0.80, 	1.20, 	0.00, 	1.20, 	1.20, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 
				1.20, 	1.60,	1.60, 	0.00, 	0.00, 	0.80, 	0.00, 	0.00, 	1.20, 	1.20, 	1.20, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 
				1.20, 	1.60,	1.60, 	0.00, 	0.00, 	0.00, 	0.80, 	0.00, 	1.20, 	1.20, 	1.20, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 
				0.90, 	0.90,	0.00, 	0.00, 	0.00, 	1.60, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00,
				0.90, 	0.90,	0.00, 	0.00, 	0.00, 	0.00, 	1.60, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00,
				0.90, 	0.90,	0.00, 	0.00, 	0.00, 	0.80, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00,
				0.90, 	0.90,	0.00, 	0.00, 	0.00, 	0.00, 	0.80, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00,
//				1.40, 	1.40, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	1.40,
				1.20, 	1.60,	1.60, 	0.00, 	0.00, 	1.60, 	0.00, 	1.20, 	0.00, 	1.20, 	1.20, 	0.00, 	0.00, 	0.00, 	1.60, 	1.60,
				1.20, 	1.60,	1.60, 	0.00, 	0.00, 	1.60, 	0.00, 	0.00, 	1.20, 	1.20, 	1.20, 	0.00, 	0.00, 	0.00, 	1.60, 	1.60,
				1.20, 	1.60,	1.60, 	0.00, 	0.00, 	0.80, 	0.00, 	1.20, 	0.00, 	1.20, 	1.20, 	0.00, 	0.00, 	0.00, 	0.80, 	1.60,
				1.20, 	1.60,	1.60, 	0.00, 	0.00, 	0.80, 	0.00, 	0.00, 	1.20, 	1.20, 	1.20, 	0.00, 	0.00, 	0.00, 	0.80, 	1.60,
				0.90, 	0.90,	0.00, 	0.00, 	0.00, 	1.60, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	1.60, 	1.60,
				0.90, 	0.90,	0.00, 	0.00, 	0.00, 	0.80, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	1.60, 	1.60,
			};		
			long nIdxFator = 0;
			long nSizeRow  = 20;
			m_MatComboUlt.Resize(nSizeRow,BRIDGE_LOAD_BASIC);
			for(long nRow=0; nRow<nSizeRow; nRow++)
			{
				for(long nCol=0; nCol<BRIDGE_LOAD_BASIC; nCol++)
				{
					if(nCol == BRIDGE_LOAD_SOIL) // 연직토압
					{
						if(nRow != 0 && nRow != 5)//  && nRow != 14) // 배열에서 14번이 막혔음.
						{
							m_MatComboUlt(nRow,nCol) = dFactor[nIdxFator++] * m_dFactorComboVLoad;
						}
						else
						{
							m_MatComboUlt(nRow,nCol) = dFactor[nIdxFator++];
						}
					}
					else if(nCol == BRIDGE_LOAD_WATERVERT) // 연직수압
					{
						if(nRow > 14)// 15) // 배열에서 14번이 막혔음.
						{
							m_MatComboUlt(nRow,nCol) = dFactor[nIdxFator++] * m_dFactorComboVLoad;
						}
						else
						{
							m_MatComboUlt(nRow,nCol) = dFactor[nIdxFator++];
						}
					}
					else
					{
						m_MatComboUlt(nRow,nCol) = dFactor[nIdxFator++];
					}
					if(m_bApplyLoad[nCol]==FALSE) m_MatComboUlt(nRow,nCol) = 0;
				}
			}
		}
	}
	// 도로교 설계기준
	if(m_pARcBridgeDataStd->m_nTypeConditionApply==DESIGN_CONDITION_ROAD)
	{
		double dFactor[928] = {
			1.300,	1.500,	2.150,	0.000,	0.000,	1.700,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.300,	1.500,	0.000,	2.150,	0.000,	1.700,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.300,	1.500,	2.150,	0.000,	0.000,	0.000,	1.700,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.300,	1.500,	0.000,	2.150,	0.000,	0.000,	1.700,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.300,	1.500,	0.000,	0.000,	1.300,	1.700,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.300,	1.500,	0.000,	0.000,	1.300,	0.000,	1.700,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.300,	1.500,	0.000,	0.000,	0.000,	1.700,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.300,	1.500,	0.000,	0.000,	0.000,	0.000,	1.700,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.300,	1.500,	1.300,	0.000,	0.000,	1.700,	0.000,	1.300,	0.000,	1.300,	1.300,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.300,	1.500,	1.300,	0.000,	0.000,	1.700,	0.000,	0.000,	1.300,	1.300,	1.300,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.300,	1.500,	0.000,	1.300,	0.000,	1.700,	0.000,	1.300,	0.000,	1.300,	1.300,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.300,	1.500,	0.000,	1.300,	0.000,	1.700,	0.000,	0.000,	1.300,	1.300,	1.300,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.300,	1.500,	1.300,	0.000,	0.000,	0.000,	1.700,	1.300,	0.000,	1.300,	1.300,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.300,	1.500,	1.300,	0.000,	0.000,	0.000,	1.700,	0.000,	1.300,	1.300,	1.300,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.300,	1.500,	0.000,	1.300,	0.000,	0.000,	1.700,	1.300,	0.000,	1.300,	1.300,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.300,	1.500,	0.000,	1.300,	0.000,	0.000,	1.700,	0.000,	1.300,	1.300,	1.300,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.250,	1.450,	0.000,	0.000,	0.000,	1.650,	0.000,	1.250,	0.000,	1.250,	1.250,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.250,	1.450,	0.000,	0.000,	0.000,	1.650,	0.000,	0.000,	1.250,	1.250,	1.250,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.250,	1.450,	0.000,	0.000,	0.000,	0.000,	1.650,	1.250,	0.000,	1.250,	1.250,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.250,	1.450,	0.000,	0.000,	0.000,	0.000,	1.650,	0.000,	1.250,	1.250,	1.250,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.300,	1.500,	2.150,	0.000,	0.000,	0.650,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.300,	1.500,	0.000,	2.150,	0.000,	0.650,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.300,	1.500,	2.150,	0.000,	0.000,	0.000,	0.650,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.300,	1.500,	0.000,	2.150,	0.000,	0.000,	0.650,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.300,	1.500,	0.000,	0.000,	1.300,	0.650,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.300,	1.500,	0.000,	0.000,	1.300,	0.000,	0.650,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.300,	1.500,	0.000,	0.000,	0.000,	0.650,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.300,	1.500,	0.000,	0.000,	0.000,	0.000,	0.650,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.300,	1.500,	1.300,	0.000,	0.000,	0.650,	0.000,	1.300,	0.000,	1.300,	1.300,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.300,	1.500,	1.300,	0.000,	0.000,	0.650,	0.000,	0.000,	1.300,	1.300,	1.300,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.300,	1.500,	0.000,	1.300,	0.000,	0.650,	0.000,	1.300,	0.000,	1.300,	1.300,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.300,	1.500,	0.000,	1.300,	0.000,	0.650,	0.000,	0.000,	1.300,	1.300,	1.300,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.300,	1.500,	1.300,	0.000,	0.000,	0.000,	0.650,	1.300,	0.000,	1.300,	1.300,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.300,	1.500,	1.300,	0.000,	0.000,	0.000,	0.650,	0.000,	1.300,	1.300,	1.300,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.300,	1.500,	0.000,	1.300,	0.000,	0.000,	0.650,	1.300,	0.000,	1.300,	1.300,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.300,	1.500,	0.000,	1.300,	0.000,	0.000,	0.650,	0.000,	1.300,	1.300,	1.300,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.250,	1.450,	0.000,	0.000,	0.000,	0.650,	0.000,	1.250,	0.000,	1.250,	1.250,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.250,	1.450,	0.000,	0.000,	0.000,	0.650,	0.000,	0.000,	1.250,	1.250,	1.250,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.250,	1.450,	0.000,	0.000,	0.000,	0.000,	0.650,	1.250,	0.000,	1.250,	1.250,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.250,	1.450,	0.000,	0.000,	0.000,	0.000,	0.650,	0.000,	1.250,	1.250,	1.250,	0.000,	0.000,	0.000, 	0.000,	0.000,
			//  0        1        2        3        4        5        6        7        8        9       10       11       12       13       14       15 
			1.300,	1.500,	2.150,	0.000,	0.000,	1.700,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	1.300,	1.300, // 41
			1.300,	1.500,	0.000,	0.000,	0.000,	1.700,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	1.300,	1.300, // 42
			1.300,	1.500,	0.000,	0.000,	1.300,	1.700,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	1.300,	1.300, // 43
			1.300,	1.500,	1.300,	0.000,	0.000,	1.700,	0.000,	1.300,	0.000,	1.300,	0.000,	0.000,	0.000,	0.000,	1.300,	1.300, // 44
			1.300,	1.500,	1.300,	0.000,	0.000,	1.700,	0.000,	0.000,	1.300,	1.300,	0.000,	0.000,	0.000,	0.000,	1.300,	1.300, // 45
			1.300,	1.500,	0.000,	0.000,	0.000,	1.700,	0.000,	1.300,	0.000,	1.300,	0.000,	0.000,	0.000,	0.000,	1.300,	1.300, // 46
			1.300,	1.500,	0.000,	0.000,	0.000,	1.700,	0.000,	0.000,	1.300,	1.300,	0.000,	0.000,	0.000,	0.000,	1.300,	1.300, // 47
			1.250,	1.450,	0.000,	0.000,	0.000,	1.650,	0.000,	1.250,	0.000,	1.250,	0.000,	0.000,	0.000,	0.000,	1.300,	1.300, // 48
			1.250,	1.450,	0.000,	0.000,	0.000,	1.650,	0.000,	0.000,	1.250,	1.250,	0.000,	0.000,	0.000,	0.000,	1.300,	1.300, // 49
			1.300,	1.500,	2.150,	0.000,	0.000,	0.650,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	1.300,	1.300, // 50
			1.300,	1.500,	0.000,	0.000,	0.000,	0.650,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	1.300,	1.300, // 51
			1.300,	1.500,	0.000,	0.000,	1.300,	0.650,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	1.300,	1.300, // 52
			1.300,	1.500,	1.300,	0.000,	0.000,	0.650,	0.000,	1.300,	0.000,	1.300,	0.000,	0.000,	0.000,	0.000,	1.300,	1.300, // 53
			1.300,	1.500,	1.300,	0.000,	0.000,	0.650,	0.000,	0.000,	1.300,	1.300,	0.000,	0.000,	0.000,	0.000,	1.300,	1.300, // 54
			1.300,	1.500,	0.000,	0.000,	0.000,	0.650,	0.000,	1.300,	0.000,	1.300,	0.000,	0.000,	0.000,	0.000,	1.300,	1.300, // 55
			1.300,	1.500,	0.000,	0.000,	0.000,	0.650,	0.000,	0.000,	1.300,	1.300,	0.000,	0.000,	0.000,	0.000,	1.300,	1.300, // 56
			1.250,	1.450,	0.000,	0.000,	0.000,	0.650,	0.000,	1.250,	0.000,	1.250,	0.000,	0.000,	0.000,	0.000,	1.300,	1.300, // 57
			1.250,	1.450,	0.000,	0.000,	0.000,	0.650,	0.000,	0.000,	1.250,	1.250,	0.000,	0.000,	0.000,	0.000, 	1.300,	1.300  // 58	
		};
		long nIdxFator = 0;
		long nSizeRow  = 58;
		m_MatComboUlt.Resize(nSizeRow,BRIDGE_LOAD_BASIC);
		for(long nRow=0; nRow<nSizeRow; nRow++)
		{
			for(long nCol=0; nCol<BRIDGE_LOAD_BASIC; nCol++)
			{
				m_MatComboUlt(nRow,nCol) = dFactor[nIdxFator++];
				if(m_bApplyLoad[nCol]==FALSE) m_MatComboUlt(nRow,nCol) = 0;
			}
		}
	}
	// 철도교 설계기준
	if(m_pARcBridgeDataStd->m_nTypeConditionApply==DESIGN_CONDITION_RAIL || m_pARcBridgeDataStd->m_nTypeConditionApply==3)
	{
		if(m_pARcBridgeDataStd->IsRailDesign2017())
		{
			double dFactor[640] = {
				1.350,	1.350,	1.850,	0.000,	0.000,	1.600,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.350,	1.350,	1.850,	0.000,	0.000,	0.000,	1.600,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.600,	1.600,	1.600,	0.000,	0.000,	1.600,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.600,	1.600,	1.600,	0.000,	0.000,	0.000,	1.600,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.350,	1.350,	1.400,	0.000,	0.000,	1.600,	0.000,	1.350,	0.000,	1.350,	1.350,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.350,	1.350,	1.400,	0.000,	0.000,	1.600,	0.000,	0.000,	1.350,	1.350,	1.350,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.350,	1.350,	1.400,	0.000,	0.000,	0.000,	1.600,	1.350,	0.000,	1.350,	1.350,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.350,	1.350,	1.400,	0.000,	0.000,	0.000,	1.600,	0.000,	1.350,	1.350,	1.350,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.350,	1.350,	0.000,	0.000,	0.000,	1.600,	0.000,	1.350,	0.000,	1.350,	1.350,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.350,	1.350,	0.000,	0.000,	0.000,	1.600,	0.000,	0.000,	1.350,	1.350,	1.350,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.350,	1.350,	0.000,	0.000,	0.000,	0.000,	1.600,	1.350,	0.000,	1.350,	1.350,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.350,	1.350,	0.000,	0.000,	0.000,	0.000,	1.600,	0.000,	1.350,	1.350,	1.350,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.350,	1.350,	1.400,	0.000,	0.000,	1.600,	0.000,	1.350,	0.000,	1.350,	1.350,	1.400,	1.400,	0.000,	0.000,	0.000,
				1.350,	1.350,	1.400,	0.000,	0.000,	1.600,	0.000,	0.000,	1.350,	1.350,	1.350,	1.400,	1.400,	0.000,	0.000,	0.000,
				1.350,	1.350,	1.400,	0.000,	0.000,	0.000,	1.600,	1.350,	0.000,	1.350,	1.350,	1.400,	1.400,	0.000,	0.000,	0.000,
				1.350,	1.350,	1.400,	0.000,	0.000,	0.000,	1.600,	0.000,	1.350,	1.350,	1.350,	1.400,	1.400,	0.000,	0.000,	0.000,
				1.350,	1.350,	1.400,	0.000,	0.000,	1.600,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.350,	1.350,	1.400,	0.000,	0.000,	0.000,	1.600,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.200,	1.200,	0.000,	0.000,	0.000,	1.500,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.200,	1.200,	0.000,	0.000,	0.000,	0.000,	1.500,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.350,	1.350,	1.850,	0.000,	0.000,	0.600,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.350,	1.350,	1.850,	0.000,	0.000,	0.000,	0.600,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.600,	1.600,	1.600,	0.000,	0.000,	0.600,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.600,	1.600,	1.600,	0.000,	0.000,	0.000,	0.600,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.350,	1.350,	1.400,	0.000,	0.000,	0.600,	0.000,	1.350,	0.000,	1.350,	1.350,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.350,	1.350,	1.400,	0.000,	0.000,	0.600,	0.000,	0.000,	1.350,	1.350,	1.350,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.350,	1.350,	1.400,	0.000,	0.000,	0.000,	0.600,	1.350,	0.000,	1.350,	1.350,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.350,	1.350,	1.400,	0.000,	0.000,	0.000,	0.600,	0.000,	1.350,	1.350,	1.350,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.350,	1.350,	0.000,	0.000,	0.000,	0.600,	0.000,	1.350,	0.000,	1.350,	1.350,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.350,	1.350,	0.000,	0.000,	0.000,	0.600,	0.000,	0.000,	1.350,	1.350,	1.350,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.350,	1.350,	0.000,	0.000,	0.000,	0.000,	0.600,	1.350,	0.000,	1.350,	1.350,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.350,	1.350,	0.000,	0.000,	0.000,	0.000,	0.600,	0.000,	1.350,	1.350,	1.350,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.350,	1.350,	1.400,	0.000,	0.000,	0.600,	0.000,	1.350,	0.000,	1.350,	1.350,	1.400,	1.400,	0.000,	0.000,	0.000,
				1.350,	1.350,	1.400,	0.000,	0.000,	0.600,	0.000,	0.000,	1.350,	1.350,	1.350,	1.400,	1.400,	0.000,	0.000,	0.000,
				1.350,	1.350,	1.400,	0.000,	0.000,	0.000,	0.600,	1.350,	0.000,	1.350,	1.350,	1.400,	1.400,	0.000,	0.000,	0.000,
				1.350,	1.350,	1.400,	0.000,	0.000,	0.000,	0.600,	0.000,	1.350,	1.350,	1.350,	1.400,	1.400,	0.000,	0.000,	0.000,
				1.350,	1.350,	1.400,	0.000,	0.000,	0.600,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.350,	1.350,	1.400,	0.000,	0.000,	0.000,	0.600,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.200,	1.200,	0.000,	0.000,	0.000,	0.800,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.200,	1.200,	0.000,	0.000,	0.000,	0.000,	0.800,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000
			};
			long nIdxFator = 0;
			long nSizeRow  = 40;
			long nSizeRow2 = (IsBoxType()? 20 : 12);
			long nRow(0);
			m_MatComboUlt.Resize(nSizeRow + nSizeRow2,BRIDGE_LOAD_BASIC);
			for(nRow=0; nRow<nSizeRow; nRow++)
			{
				for(long nCol=0; nCol<BRIDGE_LOAD_BASIC; nCol++)
				{
					m_MatComboUlt(nRow,nCol) = dFactor[nIdxFator++];
					if(m_bApplyLoad[nCol]==FALSE) m_MatComboUlt(nRow,nCol) = 0;
				}
			}

			if(IsBoxType())
			{
				double dFactor2[320] = {	
					1.350,	1.350,	1.850,	0.000,	0.000,	1.600,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	1.400,	1.400,
					1.600,	1.600,	1.600,	0.000,	0.000,	1.600,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	1.600,	1.600,
					1.350,	1.350,	1.400,	0.000,	0.000,	1.600,	0.000,	1.350,	0.000,	1.350,	1.350,	0.000,	0.000,	0.000,	1.350,	1.350,
					1.350,	1.350,	1.400,	0.000,	0.000,	1.600,	0.000,	0.000,	1.350,	1.350,	1.350,	0.000,	0.000,	0.000,	1.350,	1.350,
					1.350,	1.350,	0.000,	0.000,	0.000,	1.600,	0.000,	1.350,	0.000,	1.350,	1.350,	0.000,	0.000,	0.000,	1.350,	1.350,
					1.350,	1.350,	0.000,	0.000,	0.000,	1.600,	0.000,	0.000,	1.350,	1.350,	1.350,	0.000,	0.000,	0.000,	1.350,	1.350,
					1.350,	1.350,	1.400,	0.000,	0.000,	1.600,	0.000,	1.350,	0.000,	1.350,	1.350,	1.400,	1.400,	0.000,	1.350,	1.350,
					1.350,	1.350,	1.400,	0.000,	0.000,	1.600,	0.000,	0.000,	1.350,	1.350,	1.350,	1.400,	1.400,	0.000,	1.350,	1.350,
					1.350,	1.350,	1.400,	0.000,	0.000,	1.600,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	1.350,	1.350,
					1.200,	1.200,	0.000,	0.000,	0.000,	1.500,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	1.200,	1.200,
					1.350,	1.350,	1.850,	0.000,	0.000,	0.600,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	1.400,	1.400,
					1.600,	1.600,	1.600,	0.000,	0.000,	0.600,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	1.600,	1.600,
					1.350,	1.350,	1.400,	0.000,	0.000,	0.600,	0.000,	1.350,	0.000,	1.350,	1.350,	0.000,	0.000,	0.000,	1.350,	1.350,
					1.350,	1.350,	1.400,	0.000,	0.000,	0.600,	0.000,	0.000,	1.350,	1.350,	1.350,	0.000,	0.000,	0.000,	1.350,	1.350,
					1.350,	1.350,	0.000,	0.000,	0.000,	0.600,	0.000,	1.350,	0.000,	1.350,	1.350,	0.000,	0.000,	0.000,	1.350,	1.350,
					1.350,	1.350,	0.000,	0.000,	0.000,	0.600,	0.000,	0.000,	1.350,	1.350,	1.350,	0.000,	0.000,	0.000,	1.350,	1.350,
					1.350,	1.350,	1.400,	0.000,	0.000,	0.600,	0.000,	1.350,	0.000,	1.350,	1.350,	1.400,	1.400,	0.000,	1.350,	1.350,
					1.350,	1.350,	1.400,	0.000,	0.000,	0.600,	0.000,	0.000,	1.350,	1.350,	1.350,	1.400,	1.400,	0.000,	1.350,	1.350,
					1.350,	1.350,	1.400,	0.000,	0.000,	0.600,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	1.350,	1.350,
					1.200,	1.200,	0.000,	0.000,	0.000,	0.800,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	1.200,	1.200
				};

				nIdxFator = 0;
				for(nRow; nRow<nSizeRow+nSizeRow2; nRow++)
				{
					for(long nCol=0; nCol<BRIDGE_LOAD_BASIC; nCol++)
					{
						m_MatComboUlt(nRow,nCol) = dFactor2[nIdxFator++];
						if(m_bApplyLoad[nCol]==FALSE) m_MatComboUlt(nRow,nCol) = 0;
					}
				}
			}
			else
			{
				double dFactor2[192] = {	
					1.350,	1.350,	1.400,	0.000,	0.000,	1.600,	0.000,	1.350,	0.000,	1.350,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
					1.350,	1.350,	1.400,	0.000,	0.000,	1.600,	0.000,	0.000,	1.350,	1.350,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
					1.350,	1.350,	0.000,	0.000,	0.000,	1.600,	0.000,	1.350,	0.000,	1.350,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
					1.350,	1.350,	0.000,	0.000,	0.000,	1.600,	0.000,	0.000,	1.350,	1.350,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
					1.350,	1.350,	1.400,	0.000,	0.000,	1.600,	0.000,	1.350,	0.000,	1.350,	0.000,	1.400,	1.400,	0.000,	0.000,	0.000,
					1.350,	1.350,	1.400,	0.000,	0.000,	1.600,	0.000,	0.000,	1.350,	1.350,	0.000,	1.400,	1.400,	0.000,	0.000,	0.000,
					1.350,	1.350,	1.400,	0.000,	0.000,	0.600,	0.000,	1.350,	0.000,	1.350,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
					1.350,	1.350,	1.400,	0.000,	0.000,	0.600,	0.000,	0.000,	1.350,	1.350,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
					1.350,	1.350,	0.000,	0.000,	0.000,	0.600,	0.000,	1.350,	0.000,	1.350,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
					1.350,	1.350,	0.000,	0.000,	0.000,	0.600,	0.000,	0.000,	1.350,	1.350,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
					1.350,	1.350,	1.400,	0.000,	0.000,	0.600,	0.000,	1.350,	0.000,	1.350,	0.000,	1.400,	1.400,	0.000,	0.000,	0.000,
					1.350,	1.350,	1.400,	0.000,	0.000,	0.600,	0.000,	0.000,	1.350,	1.350,	0.000,	1.400,	1.400,	0.000,	0.000,	0.000
				};

				nIdxFator = 0;
				for(nRow; nRow<nSizeRow+nSizeRow2; nRow++)
				{
					for(long nCol=0; nCol<BRIDGE_LOAD_BASIC; nCol++)
					{
						m_MatComboUlt(nRow,nCol) = dFactor2[nIdxFator++];
						if(m_bApplyLoad[nCol]==FALSE) m_MatComboUlt(nRow,nCol) = 0;
					}
				}
			}
		}
		else
		{
			double dFactor[1616] = {
				1.400,	1.400,	2.000,	0.000,	0.000,	1.700,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.400,	1.400,	0.000,	2.000,	0.000,	1.700,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.400,	1.400,	2.000,	0.000,	0.000,	0.000,	1.700,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.400,	1.400,	0.000,	2.000,	0.000,	0.000,	1.700,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.700,	1.400,	1.700,	0.000,	0.000,	1.700,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.700,	1.400,	0.000,	1.700,	0.000,	1.700,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.700,	1.400,	1.700,	0.000,	0.000,	0.000,	1.700,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.700,	1.400,	0.000,	1.700,	0.000,	0.000,	1.700,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.400,	1.400,	0.000,	0.000,	0.000,	1.700,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.400,	1.400,	0.000,	0.000,	0.000,	0.000,	1.700,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.400,	1.400,	1.400,	0.000,	0.000,	1.700,	0.000,	0.000,	0.000,	0.000,	0.000,	1.400,	1.400,	0.000,	0.000,	0.000,
				1.400,	1.400,	0.000,	1.400,	0.000,	1.700,	0.000,	0.000,	0.000,	0.000,	0.000,	1.400,	1.400,	0.000,	0.000,	0.000,
				1.400,	1.400,	1.400,	0.000,	0.000,	0.000,	1.700,	0.000,	0.000,	0.000,	0.000,	1.400,	1.400,	0.000,	0.000,	0.000,
				1.400,	1.400,	0.000,	1.400,	0.000,	0.000,	1.700,	0.000,	0.000,	0.000,	0.000,	1.400,	1.400,	0.000,	0.000,	0.000,
				1.400,	1.400,	1.400,	0.000,	0.000,	1.700,	0.000,	1.400,	0.000,	1.400,	1.400,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.400,	1.400,	1.400,	0.000,	0.000,	1.700,	0.000,	0.000,	1.400,	1.400,	1.400,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.400,	1.400,	0.000,	1.400,	0.000,	1.700,	0.000,	1.400,	0.000,	1.400,	1.400,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.400,	1.400,	0.000,	1.400,	0.000,	1.700,	0.000,	0.000,	1.400,	1.400,	1.400,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.400,	1.400,	1.400,	0.000,	0.000,	0.000,	1.700,	1.400,	0.000,	1.400,	1.400,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.400,	1.400,	1.400,	0.000,	0.000,	0.000,	1.700,	0.000,	1.400,	1.400,	1.400,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.400,	1.400,	0.000,	1.400,	0.000,	0.000,	1.700,	1.400,	0.000,	1.400,	1.400,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.400,	1.400,	0.000,	1.400,	0.000,	0.000,	1.700,	0.000,	1.400,	1.400,	1.400,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.400,	1.400,	0.000,	0.000,	0.000,	1.700,	0.000,	1.400,	0.000,	1.400,	1.400,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.400,	1.400,	0.000,	0.000,	0.000,	1.700,	0.000,	0.000,	1.400,	1.400,	1.400,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.400,	1.400,	0.000,	0.000,	0.000,	0.000,	1.700,	1.400,	0.000,	1.400,	1.400,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.400,	1.400,	0.000,	0.000,	0.000,	0.000,	1.700,	0.000,	1.400,	1.400,	1.400,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.400,	1.400,	1.400,	0.000,	0.000,	1.700,	0.000,	1.400,	0.000,	1.400,	1.400,	1.400,	1.400,	0.000,	0.000,	0.000,
				1.400,	1.400,	1.400,	0.000,	0.000,	1.700,	0.000,	0.000,	1.400,	1.400,	1.400,	1.400,	1.400,	0.000,	0.000,	0.000,
				1.400,	1.400,	0.000,	1.400,	0.000,	1.700,	0.000,	1.400,	0.000,	1.400,	1.400,	1.400,	1.400,	0.000,	0.000,	0.000,
				1.400,	1.400,	0.000,	1.400,	0.000,	1.700,	0.000,	0.000,	1.400,	1.400,	1.400,	1.400,	1.400,	0.000,	0.000,	0.000,
				1.400,	1.400,	1.400,	0.000,	0.000,	0.000,	1.700,	1.400,	0.000,	1.400,	1.400,	1.400,	1.400,	0.000,	0.000,	0.000,
				1.400,	1.400,	1.400,	0.000,	0.000,	0.000,	1.700,	0.000,	1.400,	1.400,	1.400,	1.400,	1.400,	0.000,	0.000,	0.000,
				1.400,	1.400,	0.000,	1.400,	0.000,	0.000,	1.700,	1.400,	0.000,	1.400,	1.400,	1.400,	1.400,	0.000,	0.000,	0.000,
				1.400,	1.400,	0.000,	1.400,	0.000,	0.000,	1.700,	0.000,	1.400,	1.400,	1.400,	1.400,	1.400,	0.000,	0.000,	0.000,
				1.400,	1.400,	2.000,	0.000,	0.000,	0.600,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.400,	1.400,	0.000,	2.000,	0.000,	0.600,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.400,	1.400,	2.000,	0.000,	0.000,	0.000,	0.600,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.400,	1.400,	0.000,	2.000,	0.000,	0.000,	0.600,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.700,	1.400,	1.700,	0.000,	0.000,	0.600,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.700,	1.400,	0.000,	1.700,	0.000,	0.600,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.700,	1.400,	1.700,	0.000,	0.000,	0.000,	0.600,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.700,	1.400,	0.000,	1.700,	0.000,	0.000,	0.600,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.400,	1.400,	0.000,	0.000,	0.000,	0.600,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.400,	1.400,	0.000,	0.000,	0.000,	0.000,	0.600,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.400,	1.400,	1.400,	0.000,	0.000,	0.600,	0.000,	0.000,	0.000,	0.000,	0.000,	1.400,	1.400,	0.000,	0.000,	0.000,
				1.400,	1.400,	0.000,	1.400,	0.000,	0.600,	0.000,	0.000,	0.000,	0.000,	0.000,	1.400,	1.400,	0.000,	0.000,	0.000,
				1.400,	1.400,	1.400,	0.000,	0.000,	0.000,	0.600,	0.000,	0.000,	0.000,	0.000,	1.400,	1.400,	0.000,	0.000,	0.000,
				1.400,	1.400,	0.000,	1.400,	0.000,	0.000,	0.600,	0.000,	0.000,	0.000,	0.000,	1.400,	1.400,	0.000,	0.000,	0.000,
				1.400,	1.400,	1.400,	0.000,	0.000,	0.600,	0.000,	1.400,	0.000,	1.400,	1.400,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.400,	1.400,	1.400,	0.000,	0.000,	0.600,	0.000,	0.000,	1.400,	1.400,	1.400,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.400,	1.400,	0.000,	1.400,	0.000,	0.600,	0.000,	1.400,	0.000,	1.400,	1.400,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.400,	1.400,	0.000,	1.400,	0.000,	0.600,	0.000,	0.000,	1.400,	1.400,	1.400,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.400,	1.400,	1.400,	0.000,	0.000,	0.000,	0.600,	1.400,	0.000,	1.400,	1.400,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.400,	1.400,	1.400,	0.000,	0.000,	0.000,	0.600,	0.000,	1.400,	1.400,	1.400,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.400,	1.400,	0.000,	1.400,	0.000,	0.000,	0.600,	1.400,	0.000,	1.400,	1.400,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.400,	1.400,	0.000,	1.400,	0.000,	0.000,	0.600,	0.000,	1.400,	1.400,	1.400,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.400,	1.400,	0.000,	0.000,	0.000,	0.600,	0.000,	1.400,	0.000,	1.400,	1.400,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.400,	1.400,	0.000,	0.000,	0.000,	0.600,	0.000,	0.000,	1.400,	1.400,	1.400,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.400,	1.400,	0.000,	0.000,	0.000,	0.000,	0.600,	1.400,	0.000,	1.400,	1.400,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.400,	1.400,	0.000,	0.000,	0.000,	0.000,	0.600,	0.000,	1.400,	1.400,	1.400,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.400,	1.400,	1.400,	0.000,	0.000,	0.600,	0.000,	1.400,	0.000,	1.400,	1.400,	1.400,	1.400,	0.000,	0.000,	0.000,
				1.400,	1.400,	1.400,	0.000,	0.000,	0.600,	0.000,	0.000,	1.400,	1.400,	1.400,	1.400,	1.400,	0.000,	0.000,	0.000,
				1.400,	1.400,	0.000,	1.400,	0.000,	0.600,	0.000,	1.400,	0.000,	1.400,	1.400,	1.400,	1.400,	0.000,	0.000,	0.000,
				1.400,	1.400,	0.000,	1.400,	0.000,	0.600,	0.000,	0.000,	1.400,	1.400,	1.400,	1.400,	1.400,	0.000,	0.000,	0.000,
				1.400,	1.400,	1.400,	0.000,	0.000,	0.000,	0.600,	1.400,	0.000,	1.400,	1.400,	1.400,	1.400,	0.000,	0.000,	0.000,
				1.400,	1.400,	1.400,	0.000,	0.000,	0.000,	0.600,	0.000,	1.400,	1.400,	1.400,	1.400,	1.400,	0.000,	0.000,	0.000,
				1.400,	1.400,	0.000,	1.400,	0.000,	0.000,	0.600,	1.400,	0.000,	1.400,	1.400,	1.400,	1.400,	0.000,	0.000,	0.000,
				1.400,	1.400,	0.000,	1.400,	0.000,	0.000,	0.600,	0.000,	1.400,	1.400,	1.400,	1.400,	1.400,	0.000, 	0.000,	0.000,
				//  0        1        2        3        4        5        6        7        8        9       10       11       12       13       14       15 
				1.400,   1.400,   2.000,   0.000,   0.000,  1.700,   0.000,  0.000,   0.000,  0.000,   0.000,   0.000,   0.000,   0.000,  1.400,  1.400,
				1.400,	1.400,    0.000,  0.000,    0.000,	 1.700,  0.000,  0.000,    0.000,  0.000, 	0.000,	 0.000,	 0.000,	  0.000, 1.400,  1.400,
				1.700,	1.400,	 1.700,	 0.000,	  0.000,  1.700,  0.000,  0.000,   0.000,  0.000, 	0.000,	 0.000,	 0.000,	  0.000, 1.700,  1.700,
				1.700,	1.400,	 0.000,	 0.000,	  0.000,  1.700,  0.000,  0.000,   0.000,  0.000, 	0.000,	 0.000,	 0.000,	  0.000, 1.700,  1.700,
				1.400,	1.400,	 1.400,	 0.000,	  0.000,  1.700,  0.000,  0.000,   0.000,  0.000, 	0.000,	 1.400,	 1.400,	  0.000, 1.400,  1.400,
				1.400,	1.400,	 0.000,	 0.000,	  0.000,  1.700,  0.000,  0.000,   0.000,  0.000, 	0.000,	 1.400,	 1.400,	  0.000, 1.400,  1.400,
				1.400,	1.400,	 1.400,	 0.000,	  0.000,  1.700,  0.000,  1.400,   0.000,  1.400, 	0.000,	 0.000,	 0.000,	  0.000, 1.400,  1.400,
				1.400,	1.400,	 1.400,	 0.000,	  0.000,  1.700,  0.000,  0.000,   1.400,  1.400, 	0.000,	 0.000,	 0.000,	  0.000, 1.400,  1.400,
				1.400,	1.400,	 0.000,	 0.000,	  0.000,  1.700,  0.000,  1.400,   0.000,  1.400, 	0.000,	 0.000,	 0.000,	  0.000, 1.400,  1.400,
				1.400,	1.400,	 0.000,	 0.000,	  0.000,  1.700,  0.000,  0.000,   1.400,  1.400, 	0.000,	 0.000,	 0.000,	  0.000, 1.400,  1.400,
				1.400,	1.400,	 0.000,	 0.000,	  0.000,  1.700,  0.000,  1.400,   0.000,  1.400, 	0.000,	 0.000,	 0.000,	  0.000, 1.400,  1.400,
				1.400,	1.400,	 0.000,	 0.000,	  0.000,  1.700,  0.000,  0.000,   1.400,  1.400, 	0.000,	 0.000,	 0.000,	  0.000, 1.400,  1.400,
				1.400,	1.400,	 1.400,	 0.000,	  0.000,  1.700,  0.000,  1.400,   0.000,  1.400, 	0.000,	 1.400,	 1.400,	  0.000, 1.400,  1.400,
				1.400,	1.400,	 1.400,	 0.000,	  0.000,  1.700,  0.000,  0.000,   1.400,  1.400, 	0.000,	 1.400,	 1.400,	  0.000, 1.400,  1.400,
				1.400,	1.400,	 0.000,	 0.000,	  0.000,  1.700,  0.000,  1.400,   0.000,  1.400, 	0.000,	 1.400,	 1.400,	  0.000, 1.400,  1.400,
				1.400,	1.400,	 0.000,	 0.000,	  0.000,  1.700,  0.000,  0.000,   1.400,  1.400, 	0.000,	 1.400,	 1.400,	  0.000, 1.400,  1.400,
				1.400,	1.400,	 2.000,	 0.000,	  0.000,  0.600,  0.000,  0.000,   0.000,  0.000, 	0.000,	 0.000,	 0.000,	  0.000, 1.400,  1.400,
				1.400,	1.400,	 0.000,	 0.000,	  0.000,  0.600,  0.000,  0.000,   0.000,  0.000, 	0.000,	 0.000,	 0.000,	  0.000, 1.400,  1.400,
				1.700,	1.400,	 1.700,	 0.000,	  0.000,  0.600,  0.000,  0.000,   0.000,  0.000, 	0.000,	 0.000,	 0.000,	  0.000, 1.700,  1.700,
				1.700,	1.400,	 0.000,	 0.000,	  0.000,  0.600,  0.000,  0.000,   0.000,  0.000, 	0.000,	 0.000,	 0.000,	  0.000, 1.700,  1.700,
				1.400,	1.400,	 0.000,	 0.000,	  0.000,  0.600,  0.000,  0.000,   0.000,  0.000, 	0.000,	 0.000,	 0.000,	  0.000, 1.400,  1.400,
				1.400,	1.400,	 1.400,	 0.000,	  0.000,  0.600,  0.000,  0.000,   0.000,  0.000, 	0.000,	 1.400,	 1.400,	  0.000, 1.400,  1.400,
				1.400,	1.400,	 0.000,	 0.000,	  0.000,  0.600,  0.000,  0.000,   0.000,  0.000, 	0.000,	 1.400,	 1.400,	  0.000, 1.400,  1.400,
				1.400,	1.400,	 1.400,	 0.000,	  0.000,  0.600,  0.000,  1.400,   0.000,  1.400, 	0.000,	 0.000,	 0.000,	  0.000, 1.400,  1.400,
				1.400,	1.400,	 1.400,	 0.000,	  0.000,  0.600,  0.000,  0.000,   1.400,  1.400, 	0.000,	 0.000,	 0.000,	  0.000, 1.400,  1.400,
				1.400,	1.400,	 0.000,	 0.000,	  0.000,  0.600,  0.000,  1.400,   0.000,  1.400, 	0.000,	 0.000,	 0.000,	  0.000, 1.400,  1.400,
				1.400,	1.400,	 0.000,	 0.000,	  0.000,  0.600,  0.000,  0.000,   1.400,  1.400, 	0.000,	 0.000,	 0.000,	  0.000, 1.400,  1.400,
				1.400,	1.400,	 0.000,	 0.000,	  0.000,  0.600,  0.000,  1.400,   0.000,  1.400, 	0.000,	 0.000,	 0.000,	  0.000, 1.400,  1.400,
				1.400,	1.400,	 0.000,	 0.000,	  0.000,  0.600,  0.000,  0.000,   1.400,  1.400, 	0.000,	 0.000,	 0.000,	  0.000, 1.400,  1.400,
				1.400,	1.400,	 1.400,	 0.000,	  0.000,  0.600,  0.000,  1.400,   0.000,  1.400, 	0.000,	 1.400,	 1.400,	  0.000, 1.400,  1.400,
				1.400,	1.400,	 1.400,	 0.000,	  0.000,  0.600,  0.000,  0.000,   1.400,  1.400, 	0.000,	 1.400,	 1.400,	  0.000, 1.400,  1.400,
				1.400,	1.400,	 0.000,	 0.000,	  0.000,  0.600,  0.000,  1.400,   0.000,  1.400, 	0.000,	 1.400,	 1.400,	  0.000, 1.400,  1.400,
				1.400,	1.400,	 0.000,	 0.000,	  0.000,  0.600,  0.000,  0.000,   1.400,  1.400, 	0.000,	 1.400,	 1.400,	  0.000, 1.400,  1.400
			};
			long nIdxFator = 0;
			long nSizeRow  = 101;
			m_MatComboUlt.Resize(nSizeRow,BRIDGE_LOAD_BASIC);
			for(long nRow=0; nRow<nSizeRow; nRow++)
			{
				for(long nCol=0; nCol<BRIDGE_LOAD_BASIC; nCol++)
				{
					m_MatComboUlt(nRow,nCol) = dFactor[nIdxFator++];
					if(m_bApplyLoad[nCol]==FALSE) m_MatComboUlt(nRow,nCol) = 0;
				}
			}
		}
	}

	for(long ix = 0; ix < MAX_RCBRIDGE_COMBO; ix++)
	{
		m_dFactorAnchor[COMBO_ULTIMATE][ix] = 1.0;
	}

	SetArrangeCombine(COMBO_ULTIMATE);

	long nSizeCombo = m_MatComboUlt.GetRow();
	for(long ix = 0; ix < nSizeCombo; ix++)
	{
		double dFactor = 0;
		dFactor = m_MatComboUlt(ix,BRIDGE_LOAD_WATERHORI);
		m_bApplyGroundWater[COMBO_ULTIMATE][ix] = (dFactor > 0) ? TRUE : FALSE;
	}
}

//	하중조합_권고안.xls 참조
void CRcBridgeApp::SetDataDefaultCombineUseLoad()
{
	// 3D 적용, 지하수
	long i=0; for(i=0; i<MAX_RCBRIDGE_COMBO; i++)
	{
		m_bApplyCombo3D[COMBO_SERVICE][i] = (i <= 4) ? TRUE : FALSE;
	}

	SetCalcVertLoadFactor();
	// 하중조합 권고안
	if(m_pARcBridgeDataStd->m_nTypeConditionApply==DESIGN_CONDITION_CONCRETE)
	{
		if(m_pARcBridgeDataStd->m_nTypeKciCode == 0) // 2003
		{
			double dFactor[320] = {
				1.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	0.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	1.000,	0.000,	0.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	0.000,	1.000,	0.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	1.000,	0.000,	0.000,	0.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	0.000,	1.000,	0.000,	0.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	0.000,	0.000,	0.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,	1.000,	0.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	1.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	1.000,	0.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,	1.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	0.000,	1.000,	0.000,	0.000,	0.000,	1.000,	0.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	0.000,	1.000,	0.000,	0.000,	0.000,	0.000,	1.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	1.000,	0.000,	0.000,	0.500,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	0.000,	1.000,	0.000,	0.500,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.500,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	0.000,	1.000,	0.000,	0.000,	0.500,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	0.000,	0.000,	0.000,	0.500,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.500,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000, 	0.000,	0.000
			};
			long nIdxFator = 0;
			long nSizeRow  = 20;
			m_MatComboUse.Resize(nSizeRow,BRIDGE_LOAD_BASIC);
			for(long nRow=0; nRow<nSizeRow; nRow++)
			{
				for(long nCol=0; nCol<BRIDGE_LOAD_BASIC; nCol++)
				{
					m_MatComboUse(nRow,nCol) = dFactor[nIdxFator++];
					if(m_bApplyLoad[nCol]==FALSE) m_MatComboUse(nRow,nCol) = 0;
				}
			}
		}
		else if (m_pARcBridgeDataStd->m_nTypeKciCode == 1)// 2007
		{
			double dFactor[288] = {
				1.00, 	1.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 
				1.00, 	1.00, 	1.00, 	0.00, 	0.00, 	1.00, 	0.00, 	1.00, 	0.00, 	1.00, 	1.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 
				1.00, 	1.00, 	1.00, 	0.00, 	0.00, 	0.00, 	1.00, 	1.00, 	0.00, 	1.00, 	1.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 
				1.00, 	1.00, 	1.00, 	0.00, 	0.00, 	1.00, 	0.00, 	0.00, 	1.00, 	1.00, 	1.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 
				1.00, 	1.00, 	1.00, 	0.00, 	0.00, 	0.00, 	1.00, 	0.00, 	1.00, 	1.00, 	1.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 
				1.00, 	0.00, 	1.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 
				1.00, 	1.00, 	1.00, 	0.00, 	0.00, 	0.50, 	0.00, 	1.00, 	0.00, 	1.00, 	1.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 
				1.00, 	1.00, 	1.00, 	0.00, 	0.00, 	0.00, 	0.50, 	1.00, 	0.00, 	1.00, 	1.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 
				1.00, 	1.00, 	1.00, 	0.00, 	0.00, 	0.50, 	0.00, 	0.00, 	1.00, 	1.00, 	1.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 
				1.00, 	1.00, 	1.00, 	0.00, 	0.00, 	0.00, 	0.50, 	0.00, 	1.00, 	1.00, 	1.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 
				1.00, 	1.00, 	0.00, 	0.00, 	0.00, 	1.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 
				1.00, 	1.00, 	0.00, 	0.00, 	0.00, 	0.00, 	1.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 
//				   0        1        2        3        4        5        6        7        8        9       10       11       12       13       14       15 
				1.00, 	1.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	1.00, 
				1.00, 	1.00, 	1.00, 	0.00, 	0.00, 	1.00, 	0.00, 	1.00, 	0.00, 	1.00, 	1.00, 	0.00, 	0.00, 	0.00, 	1.00, 	1.00, 
				1.00, 	1.00, 	1.00, 	0.00, 	0.00, 	1.00, 	0.00, 	0.00, 	1.00, 	1.00, 	1.00, 	0.00, 	0.00, 	0.00, 	1.00, 	1.00, 
				1.00, 	1.00, 	1.00, 	0.00, 	0.00, 	0.50, 	0.00, 	1.00, 	0.00, 	1.00, 	1.00, 	0.00, 	0.00, 	0.00, 	0.50, 	1.00, 
				1.00, 	1.00, 	1.00, 	0.00, 	0.00, 	0.50, 	0.00, 	0.00, 	1.00, 	1.00, 	1.00, 	0.00, 	0.00, 	0.00, 	0.50, 	1.00, 
				1.00, 	1.00, 	0.00, 	0.00, 	0.00, 	1.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	1.00, 	1.00
			};
			long nIdxFator = 0;
			long nSizeRow  = 18;
			m_MatComboUse.Resize(nSizeRow,BRIDGE_LOAD_BASIC);
			for(long nRow=0; nRow<nSizeRow; nRow++)
			{
				for(long nCol=0; nCol<BRIDGE_LOAD_BASIC; nCol++)
				{
					m_MatComboUse(nRow,nCol) = dFactor[nIdxFator++];
					if(m_bApplyLoad[nCol]==FALSE) m_MatComboUse(nRow,nCol) = 0;
				}
			}
		}
		else // 2012
		{
			double dFactor[352] = {
				1.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 
				1.00, 	1.00, 	1.00, 	0.00, 	0.00, 	1.00, 	0.00, 	1.00, 	0.00, 	1.00, 	1.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 
				1.00, 	1.00, 	1.00, 	0.00, 	0.00, 	0.00, 	1.00, 	1.00, 	0.00, 	1.00, 	1.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 
				1.00, 	1.00, 	1.00, 	0.00, 	0.00, 	1.00, 	0.00, 	0.00, 	1.00, 	1.00, 	1.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 
				1.00, 	1.00, 	1.00, 	0.00, 	0.00, 	0.00, 	1.00, 	0.00, 	1.00, 	1.00, 	1.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 
				1.00, 	0.00, 	1.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 
				1.00, 	1.00, 	1.00, 	0.00, 	0.00, 	0.50, 	0.00, 	1.00, 	0.00, 	1.00, 	1.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 
				1.00, 	1.00, 	1.00, 	0.00, 	0.00, 	0.00, 	0.50, 	1.00, 	0.00, 	1.00, 	1.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 
				1.00, 	1.00, 	1.00, 	0.00, 	0.00, 	0.50, 	0.00, 	0.00, 	1.00, 	1.00, 	1.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 
				1.00, 	1.00, 	1.00, 	0.00, 	0.00, 	0.00, 	0.50, 	0.00, 	1.00, 	1.00, 	1.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 
				1.00, 	1.00, 	0.00, 	0.00, 	0.00, 	1.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 
				1.00, 	1.00, 	0.00, 	0.00, 	0.00, 	0.00, 	1.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 
//				   0        1        2        3        4        5        6        7        8        9       10       11       12       13       14       15 
				1.00, 	1.00, 	0.00, 	0.00, 	0.00, 	0.50, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00,
				1.00, 	1.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.50, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00,
//				1.00, 	1.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00,	0.00, 	0.00, 	0.00, 	0.00, 	0.00,
				1.00, 	1.00, 	1.00, 	0.00, 	0.00, 	1.00, 	0.00, 	1.00, 	0.00, 	1.00, 	1.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00,
				1.00, 	1.00, 	1.00, 	0.00, 	0.00, 	1.00, 	0.00, 	0.00, 	1.00, 	1.00, 	1.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00,
				1.00, 	1.00, 	1.00, 	0.00, 	0.00, 	0.50, 	0.00, 	1.00, 	0.00, 	1.00, 	1.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00,
				1.00, 	1.00, 	1.00, 	0.00, 	0.00, 	0.50, 	0.00, 	0.00, 	1.00, 	1.00, 	1.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00,
				1.00, 	1.00, 	0.00, 	0.00, 	0.00, 	1.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00,
				1.00, 	1.00, 	0.00, 	0.00, 	0.00, 	0.50, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00, 	0.00,
			};
			long nIdxFator = 0;
			long nSizeRow  = 20;
			m_MatComboUse.Resize(nSizeRow,BRIDGE_LOAD_BASIC);
			for(long nRow=0; nRow<nSizeRow; nRow++)
			{
				for(long nCol=0; nCol<BRIDGE_LOAD_BASIC; nCol++)
				{
					m_MatComboUse(nRow,nCol) = dFactor[nIdxFator++];
					if(m_bApplyLoad[nCol]==FALSE) m_MatComboUse(nRow,nCol) = 0;
				}
			}
		}
	}
	// 도로교 설계기준
	if(m_pARcBridgeDataStd->m_nTypeConditionApply==DESIGN_CONDITION_ROAD)
	{
		double dFactor[928] = {
			1.000,	1.000,	1.000,	0.000,	0.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.000,	1.000,	0.000,	1.000,	0.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.000,	1.000,	1.000,	0.000,	0.000,	0.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.000,	1.000,	0.000,	1.000,	0.000,	0.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.000,	1.000,	0.000,	0.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.000,	1.000,	0.000,	0.000,	1.000,	0.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.000,	1.000,	0.000,	0.000,	0.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.000,	1.000,	1.000,	0.000,	0.000,	1.000,	0.000,	1.000,	0.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.000,	1.000,	1.000,	0.000,	0.000,	1.000,	0.000,	0.000,	1.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.000,	1.000,	0.000,	1.000,	0.000,	1.000,	0.000,	1.000,	0.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.000,	1.000,	0.000,	1.000,	0.000,	1.000,	0.000,	0.000,	1.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.000,	1.000,	1.000,	0.000,	0.000,	0.000,	1.000,	1.000,	0.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.000,	1.000,	1.000,	0.000,	0.000,	0.000,	1.000,	0.000,	1.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.000,	1.000,	0.000,	1.000,	0.000,	0.000,	1.000,	1.000,	0.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.000,	1.000,	0.000,	1.000,	0.000,	0.000,	1.000,	0.000,	1.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.000,	1.000,	0.000,	0.000,	0.000,	1.000,	0.000,	1.000,	0.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.000,	1.000,	0.000,	0.000,	0.000,	1.000,	0.000,	0.000,	1.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	1.000,	1.000,	0.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	1.000,	0.000,	1.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.000,	1.000,	1.000,	0.000,	0.000,	0.500,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.000,	1.000,	0.000,	1.000,	0.000,	0.500,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.500,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.000,	1.000,	0.000,	1.000,	0.000,	0.000,	0.500,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.000,	1.000,	0.000,	0.000,	1.000,	0.500,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.000,	1.000,	0.000,	0.000,	1.000,	0.000,	0.500,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.000,	1.000,	0.000,	0.000,	0.000,	0.500,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.500,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.000,	1.000,	1.000,	0.000,	0.000,	0.500,	0.000,	1.000,	0.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.000,	1.000,	1.000,	0.000,	0.000,	0.500,	0.000,	0.000,	1.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.000,	1.000,	0.000,	1.000,	0.000,	0.500,	0.000,	1.000,	0.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.000,	1.000,	0.000,	1.000,	0.000,	0.500,	0.000,	0.000,	1.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.500,	1.000,	0.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.500,	0.000,	1.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.000,	1.000,	0.000,	1.000,	0.000,	0.000,	0.500,	1.000,	0.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.000,	1.000,	0.000,	1.000,	0.000,	0.000,	0.500,	0.000,	1.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.000,	1.000,	0.000,	0.000,	0.000,	0.500,	0.000,	1.000,	0.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.000,	1.000,	0.000,	0.000,	0.000,	0.500,	0.000,	0.000,	1.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.500,	1.000,	0.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.500,	0.000,	1.000,	1.000,	1.000,	0.000,	0.000,	0.000, 	0.000,	0.000,
			//  0        1        2        3        4        5        6        7        8        9       10       11       12       13       14       15 
			1.000,	1.000,	1.000,	0.000,	0.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	1.000,	1.000, // 41
			1.000,	1.000,	0.000,	0.000,	0.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	1.000,	1.000, // 42
			1.000,	1.000,	0.000,	0.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	1.000,	1.000, // 43
			1.000,	1.000,	1.000,	0.000,	0.000,	1.000,	0.000,	1.000,	0.000,	1.000,	0.000,	0.000,	0.000,	0.000,	1.000,	1.000, // 44
			1.000,	1.000,	1.000,	0.000,	0.000,	1.000,	0.000,	0.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	1.000,	1.000, // 45
			1.000,	1.000,	0.000,	0.000,	0.000,	1.000,	0.000,	1.000,	0.000,	1.000,	0.000,	0.000,	0.000,	0.000,	1.000,	1.000, // 46
			1.000,	1.000,	0.000,	0.000,	0.000,	1.000,	0.000,	0.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	1.000,	1.000, // 47
			1.000,	1.000,	0.000,	0.000,	0.000,	1.000,	0.000,	1.000,	0.000,	1.000,	0.000,	0.000,	0.000,	0.000,	1.000,	1.000, // 48
			1.000,	1.000,	0.000,	0.000,	0.000,	1.000,	0.000,	0.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	1.000,	1.000, // 49
			1.000,	1.000,	1.000,	0.000,	0.000,	0.500,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	1.000,	1.000, // 50
			1.000,	1.000,	0.000,	0.000,	0.000,	0.500,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	1.000,	1.000, // 51
			1.000,	1.000,	0.000,	0.000,	1.000,	0.500,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	1.000,	1.000, // 52
			1.000,	1.000,	1.000,	0.000,	0.000,	0.500,	0.000,	1.000,	0.000,	1.000,	0.000,	0.000,	0.000,	0.000,	1.000,	1.000, // 53
			1.000,	1.000,	1.000,	0.000,	0.000,	0.500,	0.000,	0.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	1.000,	1.000, // 54
			1.000,	1.000,	0.000,	0.000,	0.000,	0.500,	0.000,	1.000,	0.000,	1.000,	0.000,	0.000,	0.000,	0.000,	1.000,	1.000, // 55
			1.000,	1.000,	0.000,	0.000,	0.000,	0.500,	0.000,	0.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	1.000,	1.000, // 56
			1.000,	1.000,	0.000,	0.000,	0.000,	0.500,	0.000,	1.000,	0.000,	1.000,	0.000,	0.000,	0.000,	0.000,	1.000,	1.000, // 57
			1.000,	1.000,	0.000,	0.000,	0.000,	0.500,	0.000,	0.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.000, 	1.000,	1.000  // 58	
		};
		long nIdxFator = 0;
		long nSizeRow  = 58;
		m_MatComboUse.Resize(nSizeRow,BRIDGE_LOAD_BASIC);
		for(long nRow=0; nRow<nSizeRow; nRow++)
		{
			for(long nCol=0; nCol<BRIDGE_LOAD_BASIC; nCol++)
			{
				m_MatComboUse(nRow,nCol) = dFactor[nIdxFator++];
				if(m_bApplyLoad[nCol]==FALSE) m_MatComboUse(nRow,nCol) = 0;
			}
		}
	}
	// 철도교 설계기준
	if(m_pARcBridgeDataStd->m_nTypeConditionApply==DESIGN_CONDITION_RAIL || m_pARcBridgeDataStd->m_nTypeConditionApply==3)
	{
		if(m_pARcBridgeDataStd->IsRailDesign2017())
		{
			double dFactor[512] = {
				1.000,	1.000,	1.000,	0.000,	0.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	1.000,	0.000,	0.000,	0.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	1.000,	0.000,	0.000,	1.000,	0.000,	1.000,	0.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	1.000,	0.000,	0.000,	1.000,	0.000,	0.000,	1.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	1.000,	0.000,	0.000,	0.000,	1.000,	1.000,	0.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	1.000,	0.000,	0.000,	0.000,	1.000,	0.000,	1.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	0.000,	0.000,	0.000,	1.000,	0.000,	1.000,	0.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	0.000,	0.000,	0.000,	1.000,	0.000,	0.000,	1.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	1.000,	1.000,	0.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	1.000,	0.000,	1.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	1.000,	0.000,	0.000,	1.000,	0.000,	1.000,	0.000,	1.000,	1.000,	1.000,	1.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	1.000,	0.000,	0.000,	1.000,	0.000,	0.000,	1.000,	1.000,	1.000,	1.000,	1.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	1.000,	0.000,	0.000,	0.000,	1.000,	1.000,	0.000,	1.000,	1.000,	1.000,	1.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	1.000,	0.000,	0.000,	0.000,	1.000,	0.000,	1.000,	1.000,	1.000,	1.000,	1.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	0.000,	0.000,	0.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	1.000,	0.000,	0.000,	0.500,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.500,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	1.000,	0.000,	0.000,	0.500,	0.000,	1.000,	0.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	1.000,	0.000,	0.000,	0.500,	0.000,	0.000,	1.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.500,	1.000,	0.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.500,	0.000,	1.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	0.000,	0.000,	0.000,	0.500,	0.000,	1.000,	0.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	0.000,	0.000,	0.000,	0.500,	0.000,	0.000,	1.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.500,	1.000,	0.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.500,	0.000,	1.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	1.000,	0.000,	0.000,	0.500,	0.000,	1.000,	0.000,	1.000,	1.000,	1.000,	1.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	1.000,	0.000,	0.000,	0.500,	0.000,	0.000,	1.000,	1.000,	1.000,	1.000,	1.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.500,	1.000,	0.000,	1.000,	1.000,	1.000,	1.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.500,	0.000,	1.000,	1.000,	1.000,	1.000,	1.000,	0.000,	0.000,	0.000,
				0.900,	0.900,	0.000,	0.000,	0.000,	0.500,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.900,	0.900,	0.000,	0.000,	0.000,	0.000,	0.500,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
			};
			long nIdxFator = 0;
			long nSizeRow  = 32;
			long nSizeRow2 = (IsBoxType()? 16 : 12);
			long nRow(0);
			m_MatComboUse.Resize(nSizeRow + nSizeRow2,BRIDGE_LOAD_BASIC);
			for(nRow=0; nRow<nSizeRow; nRow++)
			{
				for(long nCol=0; nCol<BRIDGE_LOAD_BASIC; nCol++)
				{
					m_MatComboUse(nRow,nCol) = dFactor[nIdxFator++];
					if(m_bApplyLoad[nCol]==FALSE) m_MatComboUse(nRow,nCol) = 0;
				}
			}

			if(IsBoxType())
			{
				double dFactor2[256] = {	
					1.000,	1.000,	1.000,	0.000,	0.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	1.000,	1.000,
					1.000,	1.000,	1.000,	0.000,	0.000,	1.000,	0.000,	1.000,	0.000,	1.000,	1.000,	0.000,	0.000,	0.000,	1.000,	1.000,
					1.000,	1.000,	1.000,	0.000,	0.000,	1.000,	0.000,	0.000,	1.000,	1.000,	1.000,	0.000,	0.000,	0.000,	1.000,	1.000,
					1.000,	1.000,	0.000,	0.000,	0.000,	1.000,	0.000,	1.000,	0.000,	1.000,	1.000,	0.000,	0.000,	0.000,	1.000,	1.000,
					1.000,	1.000,	0.000,	0.000,	0.000,	1.000,	0.000,	0.000,	1.000,	1.000,	1.000,	0.000,	0.000,	0.000,	1.000,	1.000,
					1.000,	1.000,	1.000,	0.000,	0.000,	1.000,	0.000,	1.000,	0.000,	1.000,	1.000,	1.000,	1.000,	0.000,	1.000,	1.000,
					1.000,	1.000,	1.000,	0.000,	0.000,	1.000,	0.000,	0.000,	1.000,	1.000,	1.000,	1.000,	1.000,	0.000,	1.000,	1.000,
					1.000,	1.000,	0.000,	0.000,	0.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	1.000,	1.000,
					1.000,	1.000,	1.000,	0.000,	0.000,	0.500,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	1.000,	1.000,
					1.000,	1.000,	1.000,	0.000,	0.000,	0.500,	0.000,	1.000,	0.000,	1.000,	1.000,	0.000,	0.000,	0.000,	1.000,	1.000,
					1.000,	1.000,	1.000,	0.000,	0.000,	0.500,	0.000,	0.000,	1.000,	1.000,	1.000,	0.000,	0.000,	0.000,	1.000,	1.000,
					1.000,	1.000,	0.000,	0.000,	0.000,	0.500,	0.000,	1.000,	0.000,	1.000,	1.000,	0.000,	0.000,	0.000,	1.000,	1.000,
					1.000,	1.000,	0.000,	0.000,	0.000,	0.500,	0.000,	0.000,	1.000,	1.000,	1.000,	0.000,	0.000,	0.000,	1.000,	1.000,
					1.000,	1.000,	1.000,	0.000,	0.000,	0.500,	0.000,	1.000,	0.000,	1.000,	1.000,	1.000,	1.000,	0.000,	1.000,	1.000,
					1.000,	1.000,	1.000,	0.000,	0.000,	0.500,	0.000,	0.000,	1.000,	1.000,	1.000,	1.000,	1.000,	0.000,	1.000,	1.000,
					0.900,	0.900,	0.000,	0.000,	0.000,	0.500,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	1.200,	1.200,
				};

				nIdxFator = 0;
				for(nRow; nRow<nSizeRow+nSizeRow2; nRow++)
				{
					for(long nCol=0; nCol<BRIDGE_LOAD_BASIC; nCol++)
					{
						m_MatComboUse(nRow,nCol) = dFactor2[nIdxFator++];
						if(m_bApplyLoad[nCol]==FALSE) m_MatComboUse(nRow,nCol) = 0;
					}
				}
			}
			else
			{
				double dFactor2[192] = {	
					1.000,	1.000,	1.000,	0.000,	0.000,	1.000,	0.000,	1.000,	0.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
					1.000,	1.000,	1.000,	0.000,	0.000,	1.000,	0.000,	0.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
					1.000,	1.000,	0.000,	0.000,	0.000,	1.000,	0.000,	1.000,	0.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
					1.000,	1.000,	0.000,	0.000,	0.000,	1.000,	0.000,	0.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
					1.000,	1.000,	1.000,	0.000,	0.000,	1.000,	0.000,	1.000,	0.000,	1.000,	0.000,	1.000,	1.000,	0.000,	0.000,	0.000,
					1.000,	1.000,	1.000,	0.000,	0.000,	1.000,	0.000,	0.000,	1.000,	1.000,	0.000,	1.000,	1.000,	0.000,	0.000,	0.000,
					1.000,	1.000,	1.000,	0.000,	0.000,	0.500,	0.000,	1.000,	0.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
					1.000,	1.000,	1.000,	0.000,	0.000,	0.500,	0.000,	0.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
					1.000,	1.000,	0.000,	0.000,	0.000,	0.500,	0.000,	1.000,	0.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
					1.000,	1.000,	0.000,	0.000,	0.000,	0.500,	0.000,	0.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
					1.000,	1.000,	1.000,	0.000,	0.000,	0.500,	0.000,	1.000,	0.000,	1.000,	0.000,	1.000,	1.000,	0.000,	0.000,	0.000,
					1.000,	1.000,	1.000,	0.000,	0.000,	0.500,	0.000,	0.000,	1.000,	1.000,	0.000,	1.000,	1.000,	0.000,	0.000,	0.000,
				};

				nIdxFator = 0;
				for(nRow; nRow<nSizeRow+nSizeRow2; nRow++)
				{
					for(long nCol=0; nCol<BRIDGE_LOAD_BASIC; nCol++)
					{
						m_MatComboUse(nRow,nCol) = dFactor2[nIdxFator++];
						if(m_bApplyLoad[nCol]==FALSE) m_MatComboUse(nRow,nCol) = 0;
					}
				}
			}
		}
		else
		{
			double dFactor[1616] = {
				1.000,	1.000,	1.000,	0.000,	0.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	0.000,	1.000,	0.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	1.000,	0.000,	0.000,	0.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	0.000,	1.000,	0.000,	0.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	1.000,	0.000,	0.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	0.000,	1.000,	0.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	1.000,	0.000,	0.000,	0.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	0.000,	1.000,	0.000,	0.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	0.000,	0.000,	0.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	1.000,	0.000,	0.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,	1.000,	1.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	0.000,	1.000,	0.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,	1.000,	1.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	1.000,	0.000,	0.000,	0.000,	1.000,	0.000,	0.000,	0.000,	0.000,	1.000,	1.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	0.000,	1.000,	0.000,	0.000,	1.000,	0.000,	0.000,	0.000,	0.000,	1.000,	1.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	1.000,	0.000,	0.000,	1.000,	0.000,	1.000,	0.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	1.000,	0.000,	0.000,	1.000,	0.000,	0.000,	1.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	0.000,	1.000,	0.000,	1.000,	0.000,	1.000,	0.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	0.000,	1.000,	0.000,	1.000,	0.000,	0.000,	1.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	1.000,	0.000,	0.000,	0.000,	1.000,	1.000,	0.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	1.000,	0.000,	0.000,	0.000,	1.000,	0.000,	1.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	0.000,	1.000,	0.000,	0.000,	1.000,	1.000,	0.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	0.000,	1.000,	0.000,	0.000,	1.000,	0.000,	1.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	0.000,	0.000,	0.000,	1.000,	0.000,	1.000,	0.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	0.000,	0.000,	0.000,	1.000,	0.000,	0.000,	1.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	1.000,	1.000,	0.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	1.000,	0.000,	1.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	1.000,	0.000,	0.000,	1.000,	0.000,	1.000,	0.000,	1.000,	1.000,	1.000,	1.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	1.000,	0.000,	0.000,	1.000,	0.000,	0.000,	1.000,	1.000,	1.000,	1.000,	1.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	0.000,	1.000,	0.000,	1.000,	0.000,	1.000,	0.000,	1.000,	1.000,	1.000,	1.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	0.000,	1.000,	0.000,	1.000,	0.000,	0.000,	1.000,	1.000,	1.000,	1.000,	1.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	1.000,	0.000,	0.000,	0.000,	1.000,	1.000,	0.000,	1.000,	1.000,	1.000,	1.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	1.000,	0.000,	0.000,	0.000,	1.000,	0.000,	1.000,	1.000,	1.000,	1.000,	1.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	0.000,	1.000,	0.000,	0.000,	1.000,	1.000,	0.000,	1.000,	1.000,	1.000,	1.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	0.000,	1.000,	0.000,	0.000,	1.000,	0.000,	1.000,	1.000,	1.000,	1.000,	1.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	1.000,	0.000,	0.000,	0.500,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	0.000,	1.000,	0.000,	0.500,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.500,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	0.000,	1.000,	0.000,	0.000,	0.500,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	1.000,	0.000,	0.000,	0.500,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	0.000,	1.000,	0.000,	0.500,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.500,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	0.000,	1.000,	0.000,	0.000,	0.500,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	0.000,	0.000,	0.000,	0.500,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.500,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	1.000,	0.000,	0.000,	0.500,	0.000,	0.000,	0.000,	0.000,	0.000,	1.000,	1.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	0.000,	1.000,	0.000,	0.500,	0.000,	0.000,	0.000,	0.000,	0.000,	1.000,	1.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.500,	0.000,	0.000,	0.000,	0.000,	1.000,	1.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	0.000,	1.000,	0.000,	0.000,	0.500,	0.000,	0.000,	0.000,	0.000,	1.000,	1.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	1.000,	0.000,	0.000,	0.500,	0.000,	1.000,	0.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	1.000,	0.000,	0.000,	0.500,	0.000,	0.000,	1.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	0.000,	1.000,	0.000,	0.500,	0.000,	1.000,	0.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	0.000,	1.000,	0.000,	0.500,	0.000,	0.000,	1.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.500,	1.000,	0.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.500,	0.000,	1.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	0.000,	1.000,	0.000,	0.000,	0.500,	1.000,	0.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	0.000,	1.000,	0.000,	0.000,	0.500,	0.000,	1.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	0.000,	0.000,	0.000,	0.500,	0.000,	1.000,	0.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	0.000,	0.000,	0.000,	0.500,	0.000,	0.000,	1.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.500,	1.000,	0.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.500,	0.000,	1.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	1.000,	0.000,	0.000,	0.500,	0.000,	1.000,	0.000,	1.000,	1.000,	1.000,	1.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	1.000,	0.000,	0.000,	0.500,	0.000,	0.000,	1.000,	1.000,	1.000,	1.000,	1.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	0.000,	1.000,	0.000,	0.500,	0.000,	1.000,	0.000,	1.000,	1.000,	1.000,	1.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	0.000,	1.000,	0.000,	0.500,	0.000,	0.000,	1.000,	1.000,	1.000,	1.000,	1.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.500,	1.000,	0.000,	1.000,	1.000,	1.000,	1.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	1.000,	0.000,	0.000,	0.000,	0.500,	0.000,	1.000,	1.000,	1.000,	1.000,	1.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	0.000,	1.000,	0.000,	0.000,	0.500,	1.000,	0.000,	1.000,	1.000,	1.000,	1.000,	0.000,	0.000,	0.000,
				1.000,	1.000,	0.000,	1.000,	0.000,	0.000,	0.500,	0.000,	1.000,	1.000,	1.000,	1.000,	1.000,	0.000, 	0.000,	0.000,
				//  0        1        2        3        4        5        6        7        8        9       10       11       12       13       14       15 
				1.000,   1.000,   1.000,   0.000,   0.000,  1.000,   0.000,  0.000,   0.000,  0.000,   0.000,   0.000,   0.000,   0.000,  1.000,  1.000,
				1.000,	1.000,    0.000,  0.000,    0.000,	 1.000,  0.000,  0.000,    0.000,  0.000, 	0.000,	 0.000,	 0.000,	  0.000,  1.000,  1.000,
				1.000,	1.000,	 1.000,	 0.000,	  0.000,  1.000,  0.000,  0.000,   0.000,  0.000, 	0.000,	 0.000,	 0.000,	  0.000,  1.000,  1.000,
				1.000,	1.000,	 0.000,	 0.000,	  0.000,  1.000,  0.000,  0.000,   0.000,  0.000, 	0.000,	 0.000,	 0.000,	  0.000,  1.000,  1.000,
				1.000,	1.000,	 1.000,	 0.000,	  0.000,  1.000,  0.000,  0.000,   0.000,  0.000, 	0.000,	 1.000,	 1.000,	  0.000,  1.000,  1.000,
				1.000,	1.000,	 0.000,	 0.000,	  0.000,  1.000,  0.000,  0.000,   0.000,  0.000, 	0.000,	 1.000,	 1.000,	  0.000,  1.000,  1.000,
				1.000,	1.000,	 1.000,	 0.000,	  0.000,  1.000,  0.000,  1.000,   0.000,  1.000, 	0.000,	 0.000,	 0.000,	  0.000,  1.000,  1.000,
				1.000,	1.000,	 1.000,	 0.000,	  0.000,  1.000,  0.000,  0.000,   1.000,  1.000, 	0.000,	 0.000,	 0.000,	  0.000,  1.000,  1.000,
				1.000,	1.000,	 0.000,	 0.000,	  0.000,  1.000,  0.000,  1.000,   0.000,  1.000, 	0.000,	 0.000,	 0.000,	  0.000,  1.000,  1.000,
				1.000,	1.000,	 0.000,	 0.000,	  0.000,  1.000,  0.000,  0.000,   1.000,  1.000, 	0.000,	 0.000,	 0.000,	  0.000,  1.000,  1.000,
				1.000,	1.000,	 0.000,	 0.000,	  0.000,  1.000,  0.000,  1.000,   0.000,  1.000, 	0.000,	 0.000,	 0.000,	  0.000,  1.000,  1.000,
				1.000,	1.000,	 0.000,	 0.000,	  0.000,  1.000,  0.000,  0.000,   1.000,  1.000, 	0.000,	 0.000,	 0.000,	  0.000,  1.000,  1.000,
				1.000,	1.000,	 1.000,	 0.000,	  0.000,  1.000,  0.000,  1.000,   0.000,  1.000, 	0.000,	 1.000,	 1.000,	  0.000,  1.000,  1.000,
				1.000,	1.000,	 1.000,	 0.000,	  0.000,  1.000,  0.000,  0.000,   1.000,  1.000, 	0.000,	 1.000,	 1.000,	  0.000,  1.000,  1.000,
				1.000,	1.000,	 0.000,	 0.000,	  0.000,  1.000,  0.000,  1.000,   0.000,  1.000, 	0.000,	 1.000,	 1.000,	  0.000,  1.000,  1.000,
				1.000,	1.000,	 0.000,	 0.000,	  0.000,  1.000,  0.000,  0.000,   1.000,  1.000, 	0.000,	 1.000,	 1.000,	  0.000,  1.000,  1.000,
				1.000,	1.000,	 1.000,	 0.000,	  0.000,  0.500,  0.000,  0.000,   0.000,  0.000, 	0.000,	 0.000,	 0.000,	  0.000,  1.000,  1.000,
				1.000,	1.000,	 0.000,	 0.000,	  0.000,  0.500,  0.000,  0.000,   0.000,  0.000, 	0.000,	 0.000,	 0.000,	  0.000,  1.000,  1.000,
				1.000,	1.000,	 1.000,	 0.000,	  0.000,  0.500,  0.000,  0.000,   0.000,  0.000, 	0.000,	 0.000,	 0.000,	  0.000,  1.000,  1.000,
				1.000,	1.000,	 0.000,	 0.000,	  0.000,  0.500,  0.000,  0.000,   0.000,  0.000, 	0.000,	 0.000,	 0.000,	  0.000,  1.000,  1.000,
				1.000,	1.000,	 0.000,	 0.000,	  0.000,  0.500,  0.000,  0.000,   0.000,  0.000, 	0.000,	 0.000,	 0.000,	  0.000,  1.000,  1.000,
				1.000,	1.000,	 1.000,	 0.000,	  0.000,  0.500,  0.000,  0.000,   0.000,  0.000, 	0.000,	 1.000,	 1.000,	  0.000,  1.000,  1.000,
				1.000,	1.000,	 0.000,	 0.000,	  0.000,  0.500,  0.000,  0.000,   0.000,  0.000, 	0.000,	 1.000,	 1.000,	  0.000,  1.000,  1.000,
				1.000,	1.000,	 1.000,	 0.000,	  0.000,  0.500,  0.000,  1.000,   0.000,  1.000, 	0.000,	 0.000,	 0.000,	  0.000,  1.000,  1.000,
				1.000,	1.000,	 1.000,	 0.000,	  0.000,  0.500,  0.000,  0.000,   1.000,  1.000, 	0.000,	 0.000,	 0.000,	  0.000,  1.000,  1.000,
				1.000,	1.000,	 0.000,	 0.000,	  0.000,  0.500,  0.000,  1.000,   0.000,  1.000, 	0.000,	 0.000,	 0.000,	  0.000,  1.000,  1.000,
				1.000,	1.000,	 0.000,	 0.000,	  0.000,  0.500,  0.000,  0.000,   1.000,  1.000, 	0.000,	 0.000,	 0.000,	  0.000,  1.000,  1.000,
				1.000,	1.000,	 0.000,	 0.000,	  0.000,  0.500,  0.000,  1.000,   0.000,  1.000, 	0.000,	 0.000,	 0.000,	  0.000,  1.000,  1.000,
				1.000,	1.000,	 0.000,	 0.000,	  0.000,  0.500,  0.000,  0.000,   1.000,  1.000, 	0.000,	 0.000,	 0.000,	  0.000,  1.000,  1.000,
				1.000,	1.000,	 1.000,	 0.000,	  0.000,  0.500,  0.000,  1.000,   0.000,  1.000, 	0.000,	 1.000,	 1.000,	  0.000,  1.000,  1.000,
				1.000,	1.000,	 1.000,	 0.000,	  0.000,  0.500,  0.000,  0.000,   1.000,  1.000, 	0.000,	 1.000,	 1.000,	  0.000,  1.000,  1.000,
				1.000,	1.000,	 0.000,	 0.000,	  0.000,  0.500,  0.000,  1.000,   0.000,  1.000, 	0.000,	 1.000,	 1.000,	  0.000,  1.000,  1.000,
				1.000,	1.000,	 0.000,	 0.000,	  0.000,  0.500,  0.000,  0.000,   1.000,  1.000, 	0.000,	 1.000,	 1.000,	  0.000,  1.000,  1.000
			};
			long nIdxFator = 0;
			long nSizeRow  = 101;
			m_MatComboUse.Resize(nSizeRow,BRIDGE_LOAD_BASIC);
			for(long nRow=0; nRow<nSizeRow; nRow++)
			{
				for(long nCol=0; nCol<BRIDGE_LOAD_BASIC; nCol++)
				{
					m_MatComboUse(nRow,nCol) = dFactor[nIdxFator++];
					if(m_bApplyLoad[nCol]==FALSE) m_MatComboUse(nRow,nCol) = 0;
				}
			}
		}
	}

	for(long ix = 0; ix < MAX_RCBRIDGE_COMBO; ix++)
	{
		m_dFactorAnchor[COMBO_SERVICE][ix] = 1.0;
	}

	SetArrangeCombine(COMBO_SERVICE);

	
	long nSizeCombo = m_MatComboUse.GetRow();
	for(long ix = 0; ix < nSizeCombo; ix++)
	{
		double dFactor = 0;
		dFactor = m_MatComboUse(ix,BRIDGE_LOAD_WATERHORI);
		m_bApplyGroundWater[COMBO_SERVICE][ix] = (dFactor > 0) ? TRUE : FALSE;
	}

}

void CRcBridgeApp::SetDataDefaultCombineEccLoad()
{
	// 3D 적용
	long i=0; for(i=0; i<MAX_RCBRIDGE_COMBO; i++)
	{
		if(i>4)
			m_bApplyCombo3D[COMBO_ECCENTRIC][i] = FALSE;
		else
			m_bApplyCombo3D[COMBO_ECCENTRIC][i] = TRUE;
	}
	SetCalcVertLoadFactor();
	// 하중조합 권고안
	if(m_pARcBridgeDataStd->m_nTypeConditionApply==DESIGN_CONDITION_CONCRETE)
	{
		if(m_pARcBridgeDataStd->m_nTypeKciCode == 1 || m_pARcBridgeDataStd->m_nTypeKciCode == 2)
		{
			m_MatComboEcc.Resize(0,0);
		}
		else
		{
			double dFactor[320] = {
				0.900,	0.900,	1.700,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.900,	0.900,	0.000,	1.700,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.900,	0.900,	1.700,	0.000,	0.000,	1.800,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.900,	0.900,	0.000,	1.700,	0.000,	1.800,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.900,	0.900,	1.700,	0.000,	0.000,	0.000,	1.800,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.900,	0.900,	0.000,	1.700,	0.000,	0.000,	1.800,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.900,	0.900,	0.000,	0.000,	0.000,	1.800,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.900,	0.900,	0.000,	0.000,	0.000,	0.000,	1.800,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.900,	0.900,	0.000,	0.000,	0.000,	0.000,	0.000,	1.500,	0.000,	1.500,	1.500,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.900,	0.900,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	1.500,	1.500,	1.500,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.900,	0.900,	1.275,	0.000,	0.000,	0.000,	0.000,	1.125,	0.000,	1.125,	1.125,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.900,	0.900,	1.275,	0.000,	0.000,	0.000,	0.000,	0.000,	1.125,	1.125,	1.125,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.900,	0.900,	0.000,	1.275,	0.000,	0.000,	0.000,	1.125,	0.000,	1.125,	1.125,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.900,	0.900,	0.000,	1.275,	0.000,	0.000,	0.000,	0.000,	1.125,	1.125,	1.125,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.900,	0.900,	1.700,	0.000,	0.000,	0.900,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.900,	0.900,	0.000,	1.700,	0.000,	0.900,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.900,	0.900,	1.700,	0.000,	0.000,	0.000,	0.900,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.900,	0.900,	0.000,	1.700,	0.000,	0.000,	0.900,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.900,	0.900,	0.000,	0.000,	0.000,	0.900,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.900,	0.900,	0.000,	0.000,	0.000,	0.000,	0.900,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000, 	0.000,	0.000
			};
			long nIdxFator = 0;
			long nSizeRow  = 20;
			m_MatComboEcc.Resize(nSizeRow,BRIDGE_LOAD_BASIC);
			for(long nRow=0; nRow<nSizeRow; nRow++)
			{
				for(long nCol=0; nCol<BRIDGE_LOAD_BASIC; nCol++)
				{
					m_MatComboEcc(nRow,nCol) = dFactor[nIdxFator++] * ((nCol==1)? m_dFactorComboVLoad : 1.0);
					if(m_bApplyLoad[nCol]==FALSE) m_MatComboEcc(nRow,nCol) = 0;
				}
			}
		}
	}
	// 도로교 설계기준
	if(m_pARcBridgeDataStd->m_nTypeConditionApply==DESIGN_CONDITION_ROAD)
	{
		double dFactor[928] = {
			1.000,	1.000,	2.150,	0.000,	0.000,	1.700,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.000,	1.000,	0.000,	2.150,	0.000,	1.700,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.000,	1.000,	2.150,	0.000,	0.000,	0.000,	1.700,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.000,	1.000,	0.000,	2.150,	0.000,	0.000,	1.700,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.000,	1.000,	0.000,	0.000,	1.300,	1.700,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.000,	1.000,	0.000,	0.000,	1.300,	0.000,	1.700,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.000,	1.000,	0.000,	0.000,	0.000,	1.700,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	1.700,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.000,	1.000,	1.300,	0.000,	0.000,	1.700,	0.000,	1.300,	0.000,	1.300,	1.300,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.000,	1.000,	1.300,	0.000,	0.000,	1.700,	0.000,	0.000,	1.300,	1.300,	1.300,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.000,	1.000,	0.000,	1.300,	0.000,	1.700,	0.000,	1.300,	0.000,	1.300,	1.300,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.000,	1.000,	0.000,	1.300,	0.000,	1.700,	0.000,	0.000,	1.300,	1.300,	1.300,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.000,	1.000,	1.300,	0.000,	0.000,	0.000,	1.700,	1.300,	0.000,	1.300,	1.300,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.000,	1.000,	1.300,	0.000,	0.000,	0.000,	1.700,	0.000,	1.300,	1.300,	1.300,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.000,	1.000,	0.000,	1.300,	0.000,	0.000,	1.700,	1.300,	0.000,	1.300,	1.300,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.000,	1.000,	0.000,	1.300,	0.000,	0.000,	1.700,	0.000,	1.300,	1.300,	1.300,	0.000,	0.000,	0.000,	0.000,	0.000,
			0.950,	0.950,	0.000,	0.000,	0.000,	1.650,	0.000,	1.250,	0.000,	1.250,	1.250,	0.000,	0.000,	0.000,	0.000,	0.000,
			0.950,	0.950,	0.000,	0.000,	0.000,	1.650,	0.000,	0.000,	1.250,	1.250,	1.250,	0.000,	0.000,	0.000,	0.000,	0.000,
			0.950,	0.950,	0.000,	0.000,	0.000,	0.000,	1.650,	1.250,	0.000,	1.250,	1.250,	0.000,	0.000,	0.000,	0.000,	0.000,
			0.950,	0.950,	0.000,	0.000,	0.000,	0.000,	1.650,	0.000,	1.250,	1.250,	1.250,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.000,	1.000,	2.150,	0.000,	0.000,	0.650,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.000,	1.000,	0.000,	2.150,	0.000,	0.650,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.000,	1.000,	2.150,	0.000,	0.000,	0.000,	0.650,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.000,	1.000,	0.000,	2.150,	0.000,	0.000,	0.650,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.000,	1.000,	0.000,	0.000,	1.300,	0.650,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.000,	1.000,	0.000,	0.000,	1.300,	0.000,	0.650,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.000,	1.000,	0.000,	0.000,	0.000,	0.650,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.650,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.000,	1.000,	1.300,	0.000,	0.000,	0.650,	0.000,	1.300,	0.000,	1.300,	1.300,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.000,	1.000,	1.300,	0.000,	0.000,	0.650,	0.000,	0.000,	1.300,	1.300,	1.300,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.000,	1.000,	0.000,	1.300,	0.000,	0.650,	0.000,	1.300,	0.000,	1.300,	1.300,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.000,	1.000,	0.000,	1.300,	0.000,	0.650,	0.000,	0.000,	1.300,	1.300,	1.300,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.000,	1.000,	1.300,	0.000,	0.000,	0.000,	0.650,	1.300,	0.000,	1.300,	1.300,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.000,	1.000,	1.300,	0.000,	0.000,	0.000,	0.650,	0.000,	1.300,	1.300,	1.300,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.000,	1.000,	0.000,	1.300,	0.000,	0.000,	0.650,	1.300,	0.000,	1.300,	1.300,	0.000,	0.000,	0.000,	0.000,	0.000,
			1.000,	1.000,	0.000,	1.300,	0.000,	0.000,	0.650,	0.000,	1.300,	1.300,	1.300,	0.000,	0.000,	0.000,	0.000,	0.000,
			0.950,	0.950,	0.000,	0.000,	0.000,	0.650,	0.000,	1.250,	0.000,	1.250,	1.250,	0.000,	0.000,	0.000,	0.000,	0.000,
			0.950,	0.950,	0.000,	0.000,	0.000,	0.650,	0.000,	0.000,	1.250,	1.250,	1.250,	0.000,	0.000,	0.000,	0.000,	0.000,
			0.950,	0.950,	0.000,	0.000,	0.000,	0.000,	0.650,	1.250,	0.000,	1.250,	1.250,	0.000,	0.000,	0.000,	0.000,	0.000,
			0.950,	0.950,	0.000,	0.000,	0.000,	0.000,	0.650,	0.000,	1.250,	1.250,	1.250,	0.000,	0.000,	0.000, 	0.000,	0.000,
			//  0        1        2        3        4        5        6        7        8        9       10       11       12       13       14       15 
			1.000,	1.000,	2.150,	0.000,	0.000,	1.700,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	1.300,	1.300, // 41
			1.000,	1.000,	0.000,	0.000,	0.000,	1.700,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	1.300,	1.300, // 42
			1.000,	1.000,	0.000,	0.000,	1.300,	1.700,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	1.300,	1.300, // 43
			1.000,	1.000,	1.300,	0.000,	0.000,	1.700,	0.000,	1.300,	0.000,	1.300,	0.000,	0.000,	0.000,	0.000,	1.300,	1.300, // 44
			1.000,	1.000,	1.300,	0.000,	0.000,	1.700,	0.000,	0.000,	1.300,	1.300,	0.000,	0.000,	0.000,	0.000,	1.300,	1.300, // 45
			1.000,	1.000,	0.000,	0.000,	0.000,	1.700,	0.000,	1.300,	0.000,	1.300,	0.000,	0.000,	0.000,	0.000,	1.300,	1.300, // 46
			1.000,	1.000,	0.000,	0.000,	0.000,	1.700,	0.000,	0.000,	1.300,	1.300,	0.000,	0.000,	0.000,	0.000,	1.300,	1.300, // 47
			0.950,	0.950,	0.000,	0.000,	0.000,	1.650,	0.000,	1.250,	0.000,	1.250,	0.000,	0.000,	0.000,	0.000,	1.300,	1.300, // 48
			0.950,	0.950,	0.000,	0.000,	0.000,	1.650,	0.000,	0.000,	1.250,	1.250,	0.000,	0.000,	0.000,	0.000,	1.300,	1.300, // 49
			1.000,	1.000,	2.150,	0.000,	0.000,	0.650,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	1.300,	1.300, // 50
			1.000,	1.000,	0.000,	0.000,	0.000,	0.650,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	1.300,	1.300, // 51
			1.000,	1.000,	0.000,	0.000,	1.300,	0.650,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	1.300,	1.300, // 52
			1.000,	1.000,	1.300,	0.000,	0.000,	0.650,	0.000,	1.300,	0.000,	1.300,	0.000,	0.000,	0.000,	0.000,	1.300,	1.300, // 53
			1.000,	1.000,	1.300,	0.000,	0.000,	0.650,	0.000,	0.000,	1.300,	1.300,	0.000,	0.000,	0.000,	0.000,	1.300,	1.300, // 54
			1.000,	1.000,	0.000,	0.000,	0.000,	0.650,	0.000,	1.300,	0.000,	1.300,	0.000,	0.000,	0.000,	0.000,	1.300,	1.300, // 55
			1.000,	1.000,	0.000,	0.000,	0.000,	0.650,	0.000,	0.000,	1.300,	1.300,	0.000,	0.000,	0.000,	0.000,	1.300,	1.300, // 56
			0.950,	0.950,	0.000,	0.000,	0.000,	0.650,	0.000,	1.250,	0.000,	1.250,	0.000,	0.000,	0.000,	0.000,	1.300,	1.300, // 57
			0.950,	0.950,	0.000,	0.000,	0.000,	0.650,	0.000,	0.000,	1.250,	1.250,	0.000,	0.000,	0.000,	0.000, 	1.300,	1.300  // 58	
		};
		long nIdxFator = 0;
		long nSizeRow  = 58;
		m_MatComboEcc.Resize(nSizeRow,BRIDGE_LOAD_BASIC);
		for(long nRow=0; nRow<nSizeRow; nRow++)
		{
			for(long nCol=0; nCol<BRIDGE_LOAD_BASIC; nCol++)
			{
				m_MatComboEcc(nRow,nCol) = dFactor[nIdxFator++];
				if(m_bApplyLoad[nCol]==FALSE) m_MatComboEcc(nRow,nCol) = 0;
			}
		}
	}
	// 철도교 설계기준
	if(m_pARcBridgeDataStd->m_nTypeConditionApply==DESIGN_CONDITION_RAIL || m_pARcBridgeDataStd->m_nTypeConditionApply==3)
	{
		if(m_pARcBridgeDataStd->IsRailDesign2017())
		{
			double dFactor[640] = {
				0.800,	0.800,	1.850,	0.000,	0.000,	1.600,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.800,	0.800,	1.850,	0.000,	0.000,	0.000,	1.600,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.800,	0.800,	1.600,	0.000,	0.000,	1.600,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.800,	0.800,	1.600,	0.000,	0.000,	0.000,	1.600,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.800,	0.800,	1.400,	0.000,	0.000,	1.600,	0.000,	1.350,	0.000,	1.350,	1.350,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.800,	0.800,	1.400,	0.000,	0.000,	1.600,	0.000,	0.000,	1.350,	1.350,	1.350,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.800,	0.800,	1.400,	0.000,	0.000,	0.000,	1.600,	1.350,	0.000,	1.350,	1.350,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.800,	0.800,	1.400,	0.000,	0.000,	0.000,	1.600,	0.000,	1.350,	1.350,	1.350,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.800,	0.800,	0.000,	0.000,	0.000,	1.600,	0.000,	1.350,	0.000,	1.350,	1.350,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.800,	0.800,	0.000,	0.000,	0.000,	1.600,	0.000,	0.000,	1.350,	1.350,	1.350,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.800,	0.800,	0.000,	0.000,	0.000,	0.000,	1.600,	1.350,	0.000,	1.350,	1.350,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.800,	0.800,	0.000,	0.000,	0.000,	0.000,	1.600,	0.000,	1.350,	1.350,	1.350,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.800,	0.800,	1.400,	0.000,	0.000,	1.600,	0.000,	1.350,	0.000,	1.350,	1.350,	1.400,	1.400,	0.000,	0.000,	0.000,
				0.800,	0.800,	1.400,	0.000,	0.000,	1.600,	0.000,	0.000,	1.350,	1.350,	1.350,	1.400,	1.400,	0.000,	0.000,	0.000,
				0.800,	0.800,	1.400,	0.000,	0.000,	0.000,	1.600,	1.350,	0.000,	1.350,	1.350,	1.400,	1.400,	0.000,	0.000,	0.000,
				0.800,	0.800,	1.400,	0.000,	0.000,	0.000,	1.600,	0.000,	1.350,	1.350,	1.350,	1.400,	1.400,	0.000,	0.000,	0.000,
				0.800,	0.800,	1.400,	0.000,	0.000,	1.600,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.800,	0.800,	1.400,	0.000,	0.000,	0.000,	1.600,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.900,	0.900,	0.000,	0.000,	0.000,	1.500,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.900,	0.900,	0.000,	0.000,	0.000,	0.000,	1.500,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.800,	0.800,	1.850,	0.000,	0.000,	0.600,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.800,	0.800,	1.850,	0.000,	0.000,	0.000,	0.600,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.800,	0.800,	1.600,	0.000,	0.000,	0.600,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.800,	0.800,	1.600,	0.000,	0.000,	0.000,	0.600,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.800,	0.800,	1.400,	0.000,	0.000,	0.600,	0.000,	1.350,	0.000,	1.350,	1.350,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.800,	0.800,	1.400,	0.000,	0.000,	0.600,	0.000,	0.000,	1.350,	1.350,	1.350,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.800,	0.800,	1.400,	0.000,	0.000,	0.000,	0.600,	1.350,	0.000,	1.350,	1.350,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.800,	0.800,	1.400,	0.000,	0.000,	0.000,	0.600,	0.000,	1.350,	1.350,	1.350,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.800,	0.800,	0.000,	0.000,	0.000,	0.600,	0.000,	1.350,	0.000,	1.350,	1.350,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.800,	0.800,	0.000,	0.000,	0.000,	0.600,	0.000,	0.000,	1.350,	1.350,	1.350,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.800,	0.800,	0.000,	0.000,	0.000,	0.000,	0.600,	1.350,	0.000,	1.350,	1.350,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.800,	0.800,	0.000,	0.000,	0.000,	0.000,	0.600,	0.000,	1.350,	1.350,	1.350,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.800,	0.800,	1.400,	0.000,	0.000,	0.600,	0.000,	1.350,	0.000,	1.350,	1.350,	1.400,	1.400,	0.000,	0.000,	0.000,
				0.800,	0.800,	1.400,	0.000,	0.000,	0.600,	0.000,	0.000,	1.350,	1.350,	1.350,	1.400,	1.400,	0.000,	0.000,	0.000,
				0.800,	0.800,	1.400,	0.000,	0.000,	0.000,	0.600,	1.350,	0.000,	1.350,	1.350,	1.400,	1.400,	0.000,	0.000,	0.000,
				0.800,	0.800,	1.400,	0.000,	0.000,	0.000,	0.600,	0.000,	1.350,	1.350,	1.350,	1.400,	1.400,	0.000,	0.000,	0.000,
				0.800,	0.800,	1.400,	0.000,	0.000,	0.600,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.800,	0.800,	1.400,	0.000,	0.000,	0.000,	0.600,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.900,	0.900,	0.000,	0.000,	0.000,	0.800,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.900,	0.900,	0.000,	0.000,	0.000,	0.000,	0.800,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
			};

			long nIdxFator = 0;
			long nSizeRow  = 40;
			long nSizeRow2 = (IsBoxType()? 20 : 12);
			m_MatComboEcc.Resize(nSizeRow+nSizeRow2,BRIDGE_LOAD_BASIC);
			long nRow(0);
			for(nRow; nRow<nSizeRow; nRow++)
			{
				for(long nCol=0; nCol<BRIDGE_LOAD_BASIC; nCol++)
				{
					m_MatComboEcc(nRow,nCol) = dFactor[nIdxFator++];
					if(m_bApplyLoad[nCol]==FALSE) m_MatComboEcc(nRow,nCol) = 0;
				}
			}

			if(IsBoxType())
			{
				double dFactor2[320] = {	
					0.800,	0.800,	1.850,	0.000,	0.000,	1.600,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	1.400,	1.400,
					0.800,	0.800,	1.600,	0.000,	0.000,	1.600,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	1.600,	1.600,
					0.800,	0.800,	1.400,	0.000,	0.000,	1.600,	0.000,	1.350,	0.000,	1.350,	1.350,	0.000,	0.000,	0.000,	1.350,	1.350,
					0.800,	0.800,	1.400,	0.000,	0.000,	1.600,	0.000,	0.000,	1.350,	1.350,	1.350,	0.000,	0.000,	0.000,	1.350,	1.350,
					0.800,	0.800,	0.000,	0.000,	0.000,	1.600,	0.000,	1.350,	0.000,	1.350,	1.350,	0.000,	0.000,	0.000,	1.350,	1.350,
					0.800,	0.800,	0.000,	0.000,	0.000,	1.600,	0.000,	0.000,	1.350,	1.350,	1.350,	0.000,	0.000,	0.000,	1.350,	1.350,
					0.800,	0.800,	1.400,	0.000,	0.000,	1.600,	0.000,	1.350,	0.000,	1.350,	1.350,	1.400,	1.400,	0.000,	1.350,	1.350,
					0.800,	0.800,	1.400,	0.000,	0.000,	1.600,	0.000,	0.000,	1.350,	1.350,	1.350,	1.400,	1.400,	0.000,	1.350,	1.350,
					0.800,	0.800,	1.400,	0.000,	0.000,	1.600,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	1.350,	1.350,
					0.900,	0.900,	0.000,	0.000,	0.000,	1.500,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	1.200,	1.200,
					0.800,	0.800,	1.850,	0.000,	0.000,	0.600,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	1.400,	1.400,
					0.800,	0.800,	1.600,	0.000,	0.000,	0.600,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	1.600,	1.600,
					0.800,	0.800,	1.400,	0.000,	0.000,	0.600,	0.000,	1.350,	0.000,	1.350,	1.350,	0.000,	0.000,	0.000,	1.350,	1.350,
					0.800,	0.800,	1.400,	0.000,	0.000,	0.600,	0.000,	0.000,	1.350,	1.350,	1.350,	0.000,	0.000,	0.000,	1.350,	1.350,
					0.800,	0.800,	0.000,	0.000,	0.000,	0.600,	0.000,	1.350,	0.000,	1.350,	1.350,	0.000,	0.000,	0.000,	1.350,	1.350,
					0.800,	0.800,	0.000,	0.000,	0.000,	0.600,	0.000,	0.000,	1.350,	1.350,	1.350,	0.000,	0.000,	0.000,	1.350,	1.350,
					0.800,	0.800,	1.400,	0.000,	0.000,	0.600,	0.000,	1.350,	0.000,	1.350,	1.350,	1.400,	1.400,	0.000,	1.350,	1.350,
					0.800,	0.800,	1.400,	0.000,	0.000,	0.600,	0.000,	0.000,	1.350,	1.350,	1.350,	1.400,	1.400,	0.000,	1.350,	1.350,
					0.800,	0.800,	1.400,	0.000,	0.000,	0.600,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	1.350,	1.350,
					0.900,	0.900,	0.000,	0.000,	0.000,	0.800,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	1.200,	1.200,
				};

				nIdxFator = 0;
				for(nRow; nRow<nSizeRow+nSizeRow2; nRow++)
				{
					for(long nCol=0; nCol<BRIDGE_LOAD_BASIC; nCol++)
					{
						m_MatComboEcc(nRow,nCol) = dFactor2[nIdxFator++];
						if(m_bApplyLoad[nCol]==FALSE) m_MatComboEcc(nRow,nCol) = 0;
					}
				}
			}
			else
			{
				double dFactor2[192] = {	
					0.800,	0.800,	1.400,	0.000,	0.000,	1.600,	0.000,	1.350,	0.000,	1.350,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
					0.800,	0.800,	1.400,	0.000,	0.000,	1.600,	0.000,	0.000,	1.350,	1.350,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
					0.800,	0.800,	0.000,	0.000,	0.000,	1.600,	0.000,	1.350,	0.000,	1.350,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
					0.800,	0.800,	0.000,	0.000,	0.000,	1.600,	0.000,	0.000,	1.350,	1.350,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
					0.800,	0.800,	1.400,	0.000,	0.000,	1.600,	0.000,	1.350,	0.000,	1.350,	0.000,	1.400,	1.400,	0.000,	0.000,	0.000,
					0.800,	0.800,	1.400,	0.000,	0.000,	1.600,	0.000,	0.000,	1.350,	1.350,	0.000,	1.400,	1.400,	0.000,	0.000,	0.000,
					0.800,	0.800,	1.400,	0.000,	0.000,	0.600,	0.000,	1.350,	0.000,	1.350,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
					0.800,	0.800,	1.400,	0.000,	0.000,	0.600,	0.000,	0.000,	1.350,	1.350,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
					0.800,	0.800,	0.000,	0.000,	0.000,	0.600,	0.000,	1.350,	0.000,	1.350,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
					0.800,	0.800,	0.000,	0.000,	0.000,	0.600,	0.000,	0.000,	1.350,	1.350,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
					0.800,	0.800,	1.400,	0.000,	0.000,	0.600,	0.000,	1.350,	0.000,	1.350,	0.000,	1.400,	1.400,	0.000,	0.000,	0.000,
					0.800,	0.800,	1.400,	0.000,	0.000,	0.600,	0.000,	0.000,	1.350,	1.350,	0.000,	1.400,	1.400,	0.000,	0.000,	0.000,
				};

				nIdxFator = 0;
				for(nRow; nRow<nSizeRow+nSizeRow2; nRow++)
				{
					for(long nCol=0; nCol<BRIDGE_LOAD_BASIC; nCol++)
					{
						m_MatComboEcc(nRow,nCol) = dFactor2[nIdxFator++];
						if(m_bApplyLoad[nCol]==FALSE) m_MatComboEcc(nRow,nCol) = 0;
					}
				}
			}
		}
		else
		{
			double dFactor[1616] = {
				0.800,	0.800,	2.000,	0.000,	0.000,	1.700,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.800,	0.800,	0.000,	2.000,	0.000,	1.700,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.800,	0.800,	2.000,	0.000,	0.000,	0.000,	1.700,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.800,	0.800,	0.000,	2.000,	0.000,	0.000,	1.700,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.800,	0.800,	1.700,	0.000,	0.000,	1.700,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.800,	0.800,	0.000,	1.700,	0.000,	1.700,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.800,	0.800,	1.700,	0.000,	0.000,	0.000,	1.700,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.800,	0.800,	0.000,	1.700,	0.000,	0.000,	1.700,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.800,	0.800,	0.000,	0.000,	0.000,	1.700,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.800,	0.800,	0.000,	0.000,	0.000,	0.000,	1.700,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.800,	0.800,	1.400,	0.000,	0.000,	1.700,	0.000,	0.000,	0.000,	0.000,	0.000,	1.400,	1.400,	0.000,	0.000,	0.000,
				0.800,	0.800,	0.000,	1.400,	0.000,	1.700,	0.000,	0.000,	0.000,	0.000,	0.000,	1.400,	1.400,	0.000,	0.000,	0.000,
				0.800,	0.800,	1.400,	0.000,	0.000,	0.000,	1.700,	0.000,	0.000,	0.000,	0.000,	1.400,	1.400,	0.000,	0.000,	0.000,
				0.800,	0.800,	0.000,	1.400,	0.000,	0.000,	1.700,	0.000,	0.000,	0.000,	0.000,	1.400,	1.400,	0.000,	0.000,	0.000,
				0.800,	0.800,	1.400,	0.000,	0.000,	1.700,	0.000,	1.400,	0.000,	1.400,	1.400,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.800,	0.800,	1.400,	0.000,	0.000,	1.700,	0.000,	0.000,	1.400,	1.400,	1.400,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.800,	0.800,	0.000,	1.400,	0.000,	1.700,	0.000,	1.400,	0.000,	1.400,	1.400,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.800,	0.800,	0.000,	1.400,	0.000,	1.700,	0.000,	0.000,	1.400,	1.400,	1.400,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.800,	0.800,	1.400,	0.000,	0.000,	0.000,	1.700,	1.400,	0.000,	1.400,	1.400,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.800,	0.800,	1.400,	0.000,	0.000,	0.000,	1.700,	0.000,	1.400,	1.400,	1.400,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.800,	0.800,	0.000,	1.400,	0.000,	0.000,	1.700,	1.400,	0.000,	1.400,	1.400,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.800,	0.800,	0.000,	1.400,	0.000,	0.000,	1.700,	0.000,	1.400,	1.400,	1.400,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.800,	0.800,	0.000,	0.000,	0.000,	1.700,	0.000,	1.400,	0.000,	1.400,	1.400,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.800,	0.800,	0.000,	0.000,	0.000,	1.700,	0.000,	0.000,	1.400,	1.400,	1.400,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.800,	0.800,	0.000,	0.000,	0.000,	0.000,	1.700,	1.400,	0.000,	1.400,	1.400,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.800,	0.800,	0.000,	0.000,	0.000,	0.000,	1.700,	0.000,	1.400,	1.400,	1.400,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.800,	0.800,	1.400,	0.000,	0.000,	1.700,	0.000,	1.400,	0.000,	1.400,	1.400,	1.400,	1.400,	0.000,	0.000,	0.000,
				0.800,	0.800,	1.400,	0.000,	0.000,	1.700,	0.000,	0.000,	1.400,	1.400,	1.400,	1.400,	1.400,	0.000,	0.000,	0.000,
				0.800,	0.800,	0.000,	1.400,	0.000,	1.700,	0.000,	1.400,	0.000,	1.400,	1.400,	1.400,	1.400,	0.000,	0.000,	0.000,
				0.800,	0.800,	0.000,	1.400,	0.000,	1.700,	0.000,	0.000,	1.400,	1.400,	1.400,	1.400,	1.400,	0.000,	0.000,	0.000,
				0.800,	0.800,	1.400,	0.000,	0.000,	0.000,	1.700,	1.400,	0.000,	1.400,	1.400,	1.400,	1.400,	0.000,	0.000,	0.000,
				0.800,	0.800,	1.400,	0.000,	0.000,	0.000,	1.700,	0.000,	1.400,	1.400,	1.400,	1.400,	1.400,	0.000,	0.000,	0.000,
				0.800,	0.800,	0.000,	1.400,	0.000,	0.000,	1.700,	1.400,	0.000,	1.400,	1.400,	1.400,	1.400,	0.000,	0.000,	0.000,
				0.800,	0.800,	0.000,	1.400,	0.000,	0.000,	1.700,	0.000,	1.400,	1.400,	1.400,	1.400,	1.400,	0.000,	0.000,	0.000,
				0.800,	0.800,	2.000,	0.000,	0.000,	0.600,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.800,	0.800,	0.000,	2.000,	0.000,	0.600,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.800,	0.800,	2.000,	0.000,	0.000,	0.000,	0.600,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.800,	0.800,	0.000,	2.000,	0.000,	0.000,	0.600,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.800,	0.800,	1.700,	0.000,	0.000,	0.600,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.800,	0.800,	0.000,	1.700,	0.000,	0.600,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.800,	0.800,	1.700,	0.000,	0.000,	0.000,	0.600,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.800,	0.800,	0.000,	1.700,	0.000,	0.000,	0.600,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.800,	0.800,	0.000,	0.000,	0.000,	0.600,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.800,	0.800,	0.000,	0.000,	0.000,	0.000,	0.600,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.800,	0.800,	1.400,	0.000,	0.000,	0.600,	0.000,	0.000,	0.000,	0.000,	0.000,	1.400,	1.400,	0.000,	0.000,	0.000,
				0.800,	0.800,	0.000,	1.400,	0.000,	0.600,	0.000,	0.000,	0.000,	0.000,	0.000,	1.400,	1.400,	0.000,	0.000,	0.000,
				0.800,	0.800,	1.400,	0.000,	0.000,	0.000,	0.600,	0.000,	0.000,	0.000,	0.000,	1.400,	1.400,	0.000,	0.000,	0.000,
				0.800,	0.800,	0.000,	1.400,	0.000,	0.000,	0.600,	0.000,	0.000,	0.000,	0.000,	1.400,	1.400,	0.000,	0.000,	0.000,
				0.800,	0.800,	1.400,	0.000,	0.000,	0.600,	0.000,	1.400,	0.000,	1.400,	1.400,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.800,	0.800,	1.400,	0.000,	0.000,	0.600,	0.000,	0.000,	1.400,	1.400,	1.400,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.800,	0.800,	0.000,	1.400,	0.000,	0.600,	0.000,	1.400,	0.000,	1.400,	1.400,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.800,	0.800,	0.000,	1.400,	0.000,	0.600,	0.000,	0.000,	1.400,	1.400,	1.400,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.800,	0.800,	1.400,	0.000,	0.000,	0.000,	0.600,	1.400,	0.000,	1.400,	1.400,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.800,	0.800,	1.400,	0.000,	0.000,	0.000,	0.600,	0.000,	1.400,	1.400,	1.400,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.800,	0.800,	0.000,	1.400,	0.000,	0.000,	0.600,	1.400,	0.000,	1.400,	1.400,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.800,	0.800,	0.000,	1.400,	0.000,	0.000,	0.600,	0.000,	1.400,	1.400,	1.400,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.800,	0.800,	0.000,	0.000,	0.000,	0.600,	0.000,	1.400,	0.000,	1.400,	1.400,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.800,	0.800,	0.000,	0.000,	0.000,	0.600,	0.000,	0.000,	1.400,	1.400,	1.400,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.800,	0.800,	0.000,	0.000,	0.000,	0.000,	0.600,	1.400,	0.000,	1.400,	1.400,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.800,	0.800,	0.000,	0.000,	0.000,	0.000,	0.600,	0.000,	1.400,	1.400,	1.400,	0.000,	0.000,	0.000,	0.000,	0.000,
				0.800,	0.800,	1.400,	0.000,	0.000,	0.600,	0.000,	1.400,	0.000,	1.400,	1.400,	1.400,	1.400,	0.000,	0.000,	0.000,
				0.800,	0.800,	1.400,	0.000,	0.000,	0.600,	0.000,	0.000,	1.400,	1.400,	1.400,	1.400,	1.400,	0.000,	0.000,	0.000,
				0.800,	0.800,	0.000,	1.400,	0.000,	0.600,	0.000,	1.400,	0.000,	1.400,	1.400,	1.400,	1.400,	0.000,	0.000,	0.000,
				0.800,	0.800,	0.000,	1.400,	0.000,	0.600,	0.000,	0.000,	1.400,	1.400,	1.400,	1.400,	1.400,	0.000,	0.000,	0.000,
				0.800,	0.800,	1.400,	0.000,	0.000,	0.000,	0.600,	1.400,	0.000,	1.400,	1.400,	1.400,	1.400,	0.000,	0.000,	0.000,
				0.800,	0.800,	1.400,	0.000,	0.000,	0.000,	0.600,	0.000,	1.400,	1.400,	1.400,	1.400,	1.400,	0.000,	0.000,	0.000,
				0.800,	0.800,	0.000,	1.400,	0.000,	0.000,	0.600,	1.400,	0.000,	1.400,	1.400,	1.400,	1.400,	0.000,	0.000,	0.000,
				0.800,	0.800,	0.000,	1.400,	0.000,	0.000,	0.600,	0.000,	1.400,	1.400,	1.400,	1.400,	1.400,	0.000, 	0.000,	0.000,
				//  0        1        2        3        4        5        6        7        8        9       10       11       12       13       14       15 
				0.800,   0.800,   2.000,   0.000,   0.000,  1.700,   0.000,  0.000,   0.000,  0.000,   0.000,   0.000,   0.000,   0.000,  1.400,  1.400,
				0.800,	0.800,    0.000,  0.000,    0.000,	 1.700,  0.000,  0.000,    0.000,  0.000, 	0.000,	 0.000,	 0.000,	  0.000, 1.400,  1.400,
				0.800,	0.800,	 1.700,	 0.000,	  0.000,  1.700,  0.000,  0.000,   0.000,  0.000, 	0.000,	 0.000,	 0.000,	  0.000, 1.700,  1.700,
				0.800,	0.800,	 0.000,	 0.000,	  0.000,  1.700,  0.000,  0.000,   0.000,  0.000, 	0.000,	 0.000,	 0.000,	  0.000, 1.700,  1.700,
				0.800,	0.800,	 1.400,	 0.000,	  0.000,  1.700,  0.000,  0.000,   0.000,  0.000, 	0.000,	 1.400,	 1.400,	  0.000, 1.400,  1.400,
				0.800,	0.800,	 0.000,	 0.000,	  0.000,  1.700,  0.000,  0.000,   0.000,  0.000, 	0.000,	 1.400,	 1.400,	  0.000, 1.400,  1.400,
				0.800,	0.800,	 1.400,	 0.000,	  0.000,  1.700,  0.000,  1.400,   0.000,  1.400, 	0.000,	 0.000,	 0.000,	  0.000, 1.400,  1.400,
				0.800,	0.800,	 1.400,	 0.000,	  0.000,  1.700,  0.000,  0.000,   1.400,  1.400, 	0.000,	 0.000,	 0.000,	  0.000, 1.400,  1.400,
				0.800,	0.800,	 0.000,	 0.000,	  0.000,  1.700,  0.000,  1.400,   0.000,  1.400, 	0.000,	 0.000,	 0.000,	  0.000, 1.400,  1.400,
				0.800,	0.800,	 0.000,	 0.000,	  0.000,  1.700,  0.000,  0.000,   1.400,  1.400, 	0.000,	 0.000,	 0.000,	  0.000, 1.400,  1.400,
				0.800,	0.800,	 0.000,	 0.000,	  0.000,  1.700,  0.000,  1.400,   0.000,  1.400, 	0.000,	 0.000,	 0.000,	  0.000, 1.400,  1.400,
				0.800,	0.800,	 0.000,	 0.000,	  0.000,  1.700,  0.000,  0.000,   1.400,  1.400, 	0.000,	 0.000,	 0.000,	  0.000, 1.400,  1.400,
				0.800,	0.800,	 1.400,	 0.000,	  0.000,  1.700,  0.000,  1.400,   0.000,  1.400, 	0.000,	 1.400,	 1.400,	  0.000, 1.400,  1.400,
				0.800,	0.800,	 1.400,	 0.000,	  0.000,  1.700,  0.000,  0.000,   1.400,  1.400, 	0.000,	 1.400,	 1.400,	  0.000, 1.400,  1.400,
				0.800,	0.800,	 0.000,	 0.000,	  0.000,  1.700,  0.000,  1.400,   0.000,  1.400, 	0.000,	 1.400,	 1.400,	  0.000, 1.400,  1.400,
				0.800,	0.800,	 0.000,	 0.000,	  0.000,  1.700,  0.000,  0.000,   1.400,  1.400, 	0.000,	 1.400,	 1.400,	  0.000, 1.400,  1.400,
				0.800,	0.800,	 2.000,	 0.000,	  0.000,  0.600,  0.000,  0.000,   0.000,  0.000, 	0.000,	 0.000,	 0.000,	  0.000, 1.400,  1.400,
				0.800,	0.800,	 0.000,	 0.000,	  0.000,  0.600,  0.000,  0.000,   0.000,  0.000, 	0.000,	 0.000,	 0.000,	  0.000, 1.400,  1.400,
				0.800,	0.800,	 1.700,	 0.000,	  0.000,  0.600,  0.000,  0.000,   0.000,  0.000, 	0.000,	 0.000,	 0.000,	  0.000, 1.700,  1.700,
				0.800,	0.800,	 0.000,	 0.000,	  0.000,  0.600,  0.000,  0.000,   0.000,  0.000, 	0.000,	 0.000,	 0.000,	  0.000, 1.700,  1.700,
				0.800,	0.800,	 0.000,	 0.000,	  0.000,  0.600,  0.000,  0.000,   0.000,  0.000, 	0.000,	 0.000,	 0.000,	  0.000, 1.400,  1.400,
				0.800,	0.800,	 1.400,	 0.000,	  0.000,  0.600,  0.000,  0.000,   0.000,  0.000, 	0.000,	 1.400,	 1.400,	  0.000, 1.400,  1.400,
				0.800,	0.800,	 0.000,	 0.000,	  0.000,  0.600,  0.000,  0.000,   0.000,  0.000, 	0.000,	 1.400,	 1.400,	  0.000, 1.400,  1.400,
				0.800,	0.800,	 1.400,	 0.000,	  0.000,  0.600,  0.000,  1.400,   0.000,  1.400, 	0.000,	 0.000,	 0.000,	  0.000, 1.400,  1.400,
				0.800,	0.800,	 1.400,	 0.000,	  0.000,  0.600,  0.000,  0.000,   1.400,  1.400, 	0.000,	 0.000,	 0.000,	  0.000, 1.400,  1.400,
				0.800,	0.800,	 0.000,	 0.000,	  0.000,  0.600,  0.000,  1.400,   0.000,  1.400, 	0.000,	 0.000,	 0.000,	  0.000, 1.400,  1.400,
				0.800,	0.800,	 0.000,	 0.000,	  0.000,  0.600,  0.000,  0.000,   1.400,  1.400, 	0.000,	 0.000,	 0.000,	  0.000, 1.400,  1.400,
				0.800,	0.800,	 0.000,	 0.000,	  0.000,  0.600,  0.000,  1.400,   0.000,  1.400, 	0.000,	 0.000,	 0.000,	  0.000, 1.400,  1.400,
				0.800,	0.800,	 0.000,	 0.000,	  0.000,  0.600,  0.000,  0.000,   1.400,  1.400, 	0.000,	 0.000,	 0.000,	  0.000, 1.400,  1.400,
				0.800,	0.800,	 1.400,	 0.000,	  0.000,  0.600,  0.000,  1.400,   0.000,  1.400, 	0.000,	 1.400,	 1.400,	  0.000, 1.400,  1.400,
				0.800,	0.800,	 1.400,	 0.000,	  0.000,  0.600,  0.000,  0.000,   1.400,  1.400, 	0.000,	 1.400,	 1.400,	  0.000, 1.400,  1.400,
				0.800,	0.800,	 0.000,	 0.000,	  0.000,  0.600,  0.000,  1.400,   0.000,  1.400, 	0.000,	 1.400,	 1.400,	  0.000, 1.400,  1.400,
				0.800,	0.800,	 0.000,	 0.000,	  0.000,  0.600,  0.000,  0.000,   1.400,  1.400, 	0.000,	 1.400,	 1.400,	  0.000, 1.400,  1.400
			};
			long nIdxFator = 0;
			long nSizeRow  = 101;
			m_MatComboEcc.Resize(nSizeRow,BRIDGE_LOAD_BASIC);
			for(long nRow=0; nRow<nSizeRow; nRow++)
			{
				for(long nCol=0; nCol<BRIDGE_LOAD_BASIC; nCol++)
				{
					m_MatComboEcc(nRow,nCol) = dFactor[nIdxFator++];
					if(m_bApplyLoad[nCol]==FALSE) m_MatComboEcc(nRow,nCol) = 0;
				}
			}
		}
	}

	for(long ix = 0; ix < MAX_RCBRIDGE_COMBO; ix++)
	{
		m_dFactorAnchor[COMBO_ECCENTRIC][ix] = 1.0;
	}

	SetArrangeCombine(COMBO_ECCENTRIC);

	long nSizeCombo = m_MatComboEcc.GetRow();
	for(long ix = 0; ix < nSizeCombo; ix++)
	{
		double dFactor = 0;
		dFactor = m_MatComboEcc(ix,BRIDGE_LOAD_WATERHORI);
		m_bApplyGroundWater[COMBO_ECCENTRIC][ix] = (dFactor > 0) ? TRUE : FALSE;
	}

}

void CRcBridgeApp::SetDataDefaultCombineEQLoad()
{
	// 3D 적용
	long i=0; for(i=0; i<MAX_RCBRIDGE_COMBO; i++)
	{
		m_bApplyCombo3D[COMBO_EARTHQUAKE][i] = FALSE;
	}

	SetCalcVertLoadFactor();
	// 하중조합 권고안
	if(m_pARcBridgeDataStd->m_nTypeConditionApply==DESIGN_CONDITION_CONCRETE)
	{
		if(m_pARcBridgeDataStd->m_nTypeKciCode == 0)
		{
			double dFactor[32] = {
				1.000,	1.000,	0.000,	0.000,	0.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	1.000,	0.000,	0.000,
				1.000,	1.000,	1.000,	0.000,	0.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	1.000,	0.000,	0.000,
			};
			long nIdxFator = 0;
			long nSizeRow = 2;
			m_MatComboEQ.Resize(nSizeRow,BRIDGE_LOAD_BASIC);
			for(long nRow=0; nRow<nSizeRow; nRow++)
			{
				for(long nCol=0; nCol<BRIDGE_LOAD_BASIC; nCol++)
				{
					m_MatComboEQ(nRow,nCol) = dFactor[nIdxFator++];
					if(m_bApplyLoad[nCol]==FALSE || m_bApplyLiveLoadEQ == FALSE && (nCol == BRIDGE_LOAD_LIVE_FULL || nCol == BRIDGE_LOAD_LIVE_ECC))
					{
						m_MatComboEQ(nRow,nCol) = 0;
					}
				}
			}
		}
		else
		{
			double dFactor[128] = {
				1.200,	1.200,	1.000,	0.000,	0.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	1.000,	0.000,	0.000,
				1.200,	1.200,	1.000,	0.000,	0.000,	0.500,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	1.000 ,	1.600,	1.600,
				0.900,	0.900,	0.000,	0.000,	0.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	1.000,	0.000,	0.000,
				0.900,	0.900,	0.000,	0.000,	0.000,	0.500,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	1.000,	0.000,	0.000,
				1.200,	1.200,	1.000,	0.000,	0.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	1.000,	0.000,	0.000,
				1.200,	1.200,	1.000,	0.000,	0.000,	0.500,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	1.000,	0.000,	0.000,
				0.900,	0.900,	0.000,	0.000,	0.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	1.000,	0.000,	0.000,
				0.900,	0.900,	0.000,	0.000,	0.000,	0.500,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	1.000,	0.000,	0.000,
			};
			long nIdxFator = 0;
			long nSizeRow = 8;
			m_MatComboEQ.Resize(nSizeRow,BRIDGE_LOAD_BASIC);
			for(long nRow=0; nRow<nSizeRow; nRow++)
			{
				for(long nCol=0; nCol<BRIDGE_LOAD_BASIC; nCol++)
				{
					m_MatComboEQ(nRow,nCol) = dFactor[nIdxFator++] * ((nCol==1 || nCol==15) ? m_dFactorComboVLoad : 1.0);
					if(m_bApplyLoad[nCol]==FALSE || m_bApplyLiveLoadEQ == FALSE && (nCol == BRIDGE_LOAD_LIVE_FULL || nCol == BRIDGE_LOAD_LIVE_ECC))
					{
						m_MatComboEQ(nRow,nCol) = 0;
					}
				}
			}
		}
	}
	// 도로교 설계기준
	if(m_pARcBridgeDataStd->m_nTypeConditionApply==DESIGN_CONDITION_ROAD)
	{
		double dFactor[48] = {
			1.000,	1.000,	0.000,	0.000,	0.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	1.000,	0.000,	0.000,
			1.000,	1.000,	1.000,	0.000,	0.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	1.000,	0.000,	0.000,
			1.000,	1.000,	0.000,	0.000,	0.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	1.000,	1.000,	1.000
		};
		long nIdxFator = 0;
		long nSizeRow = 3;
		m_MatComboEQ.Resize(nSizeRow,BRIDGE_LOAD_BASIC);
		for(long nRow=0; nRow<nSizeRow; nRow++)
		{
			for(long nCol=0; nCol<BRIDGE_LOAD_BASIC; nCol++)
			{
				m_MatComboEQ(nRow,nCol) = dFactor[nIdxFator++];
				if(m_bApplyLoad[nCol]==FALSE || m_bApplyLiveLoadEQ == FALSE && (nCol == BRIDGE_LOAD_LIVE_FULL || nCol == BRIDGE_LOAD_LIVE_ECC))
				{
					m_MatComboEQ(nRow,nCol) = 0;
				}
			}

			//편토압도 양토압 Factor를 사용하기때문에 양토압 Factor를 넣어준다.
			if(m_bApplyLoad[BRIDGE_LOAD_TOAP_ONE])
			{
				m_MatComboEQ(nRow, BRIDGE_LOAD_TOAP_BOTH) = dFactor[BRIDGE_LOAD_TOAP_BOTH];
			}
		}
	}
	// 철도교 설계기준
	if(m_pARcBridgeDataStd->m_nTypeConditionApply==DESIGN_CONDITION_RAIL || m_pARcBridgeDataStd->m_nTypeConditionApply==3)
	{
		if(m_pARcBridgeDataStd->IsRailDesign2017())
		{
			double dFactor[32] = {
				1.000,	1.000,	1.000,	0.000,	0.000,	0.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	1.000,	1.000,	1.000,
				1.000,	1.000,	0.000,	0.000,	0.000,	0.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	1.000,	1.000,	1.000
			};
			long nIdxFator = 0;
			long nSizeRow = 2;
			m_MatComboEQ.Resize(nSizeRow,BRIDGE_LOAD_BASIC);
			for(long nRow=0; nRow<nSizeRow; nRow++)
			{
				for(long nCol=0; nCol<BRIDGE_LOAD_BASIC; nCol++)
				{
					m_MatComboEQ(nRow,nCol) = dFactor[nIdxFator++];
					if(m_bApplyLoad[nCol]==FALSE || m_bApplyLiveLoadEQ == FALSE && (nCol == BRIDGE_LOAD_LIVE_FULL || nCol == BRIDGE_LOAD_LIVE_ECC))
					{
						m_MatComboEQ(nRow,nCol) = 0;
					}
				}
			}
		}
		else
		{
			double dFactor[48] = {
				1.000,	1.000,	0.000,	0.000,	0.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	1.000,	0.000,	0.000,
				1.000,	1.000,	1.000,	0.000,	0.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	1.000,	0.000,	0.000,
				1.000,	1.000,	0.000,	0.000,	0.000,	1.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	0.000,	1.000,	1.000,	1.000
			};
			long nIdxFator = 0;
			long nSizeRow = 3;
			m_MatComboEQ.Resize(nSizeRow,BRIDGE_LOAD_BASIC);
			for(long nRow=0; nRow<nSizeRow; nRow++)
			{
				for(long nCol=0; nCol<BRIDGE_LOAD_BASIC; nCol++)
				{
					m_MatComboEQ(nRow,nCol) = dFactor[nIdxFator++];
					if(m_bApplyLoad[nCol]==FALSE || m_bApplyLiveLoadEQ == FALSE && (nCol == BRIDGE_LOAD_LIVE_FULL || nCol == BRIDGE_LOAD_LIVE_ECC))
					{
						m_MatComboEQ(nRow,nCol) = 0;
					}
				}
			}
		}
	}

	for(long ix = 0; ix < COMBO_EARTHQUAKE; ix++)
	{
		m_dFactorAnchor[COMBO_EARTHQUAKE][ix] = 1.0;
	}

	SetArrangeCombine(COMBO_EARTHQUAKE);
	
	long nSizeCombo = m_MatComboEQ.GetRow();
	for(long ix = 0; ix < nSizeCombo; ix++)
	{
		double dFactor = 0;
		dFactor = m_MatComboEQ(ix,BRIDGE_LOAD_WATERHORI);
		m_bApplyGroundWater[COMBO_EARTHQUAKE][ix] = (dFactor > 0) ? TRUE : FALSE;
	}

}

void CRcBridgeApp::SetCalcVertLoadFactor()
{
	// 도로교일때는 kci2003으로 계산되도록 함(10281)
	// #37534 콘크리트 설계기준에만 적용
	if(m_pARcBridgeDataStd->m_nTypeConditionApply == DESIGN_CONDITION_CONCRETE && m_pARcBridgeDataStd->m_nTypeKciCode > 0 && m_bUnderGroundRahmen)
	{
		double dH = toM(m_dUnderGroundDesignHeight);
		if(dH < 2.0)
		{
			m_dFactorComboVLoad = 1.0;
		}
		else
		{
			double dFactor = 1.05 - (0.025 * dH);
			m_dFactorComboVLoad = Round(max(dFactor, 0.875), 3);
		}
	}
	else m_dFactorComboVLoad = 1.0;
}

/*
#define BRIDGE_LOAD_DEAD		 0 // 고정하중
#define BRIDGE_LOAD_SOIL		 1 // 연직토압
#define BRIDGE_LOAD_LIVE_FULL	 2 // 활하중 (만재)
#define BRIDGE_LOAD_LIVE_ECC	 3 // 활하중 (편재)
#define BRIDGE_LOAD_TANK		 4 // 탱크 및 특수 중차량하중
#define BRIDGE_LOAD_TOAP_BOTH	 5 // 양토압
#define BRIDGE_LOAD_TOAP_ONE	 6 // 편토압
#define BRIDGE_LOAD_TEMP1		 7 // 온도하중(+)
#define BRIDGE_LOAD_TEMP2		 8 // 온도하중(-)
#define BRIDGE_LOAD_SHRINK		 9 // 건조수축
#define BRIDGE_LOAD_SUBSID		10 // 지점침하
#define BRIDGE_LOAD_SB			11 // 시동 및 제동하중
#define BRIDGE_LOAD_LR			12 // 장대레일 종하중
#define BRIDGE_LOAD_EQ			13 // 지진하중
#define BRIDGE_LOAD_BASIC		14 // 기본하중의 개수

*/
void CRcBridgeApp::SetDataDefaultSelectLoad()
{
	// 콘크리트교
	if(m_pARcBridgeDataStd->m_nTypeConditionApply==DESIGN_CONDITION_CONCRETE)
	{
		m_bApplyLoad[BRIDGE_LOAD_DEAD]		= TRUE;
		m_bApplyLoad[BRIDGE_LOAD_SOIL]		= TRUE;
		m_bApplyLoad[BRIDGE_LOAD_LIVE_FULL]	= TRUE;
		m_bApplyLoad[BRIDGE_LOAD_LIVE_ECC]	= FALSE;
		m_bApplyLoad[BRIDGE_LOAD_TANK]		= FALSE;
		m_bApplyLoad[BRIDGE_LOAD_TOAP_BOTH]	= IsOutsideWall();
		m_bApplyLoad[BRIDGE_LOAD_TOAP_ONE]	= IsOutsideWall();
		m_bApplyLoad[BRIDGE_LOAD_TEMP1]		= TRUE;
		m_bApplyLoad[BRIDGE_LOAD_TEMP2]		= TRUE;
		m_bApplyLoad[BRIDGE_LOAD_SHRINK]	= TRUE;
		m_bApplyLoad[BRIDGE_LOAD_SB]		= FALSE;
		m_bApplyLoad[BRIDGE_LOAD_LR]		= FALSE;
		m_bApplyLoad[BRIDGE_LOAD_EQ]		= TRUE;
		m_bApplyLiveLoadEQ				= FALSE;
	}
	// 도로교
	if(m_pARcBridgeDataStd->m_nTypeConditionApply==DESIGN_CONDITION_ROAD)
	{
		m_bApplyLoad[BRIDGE_LOAD_DEAD]		= TRUE;
		m_bApplyLoad[BRIDGE_LOAD_SOIL]		= TRUE;
		m_bApplyLoad[BRIDGE_LOAD_LIVE_FULL]	= TRUE;
		m_bApplyLoad[BRIDGE_LOAD_LIVE_ECC]	= FALSE;
		m_bApplyLoad[BRIDGE_LOAD_TANK]		= FALSE;
		m_bApplyLoad[BRIDGE_LOAD_TOAP_BOTH]	= IsOutsideWall();
		m_bApplyLoad[BRIDGE_LOAD_TOAP_ONE]	= IsOutsideWall();
		m_bApplyLoad[BRIDGE_LOAD_TEMP1]		= TRUE;
		m_bApplyLoad[BRIDGE_LOAD_TEMP2]		= TRUE;
		m_bApplyLoad[BRIDGE_LOAD_SHRINK]	= TRUE;
		m_bApplyLoad[BRIDGE_LOAD_SB]		= FALSE;
		m_bApplyLoad[BRIDGE_LOAD_LR]		= FALSE;
		m_bApplyLoad[BRIDGE_LOAD_EQ]		= TRUE;
		m_bApplyLiveLoadEQ					= FALSE;
	}
	// 철도교
	if(m_pARcBridgeDataStd->m_nTypeConditionApply==DESIGN_CONDITION_RAIL || m_pARcBridgeDataStd->m_nTypeConditionApply==3)
	{
		m_bApplyLoad[BRIDGE_LOAD_DEAD]		= TRUE;
		m_bApplyLoad[BRIDGE_LOAD_SOIL]		= TRUE;
		m_bApplyLoad[BRIDGE_LOAD_LIVE_FULL]	= TRUE;
		m_bApplyLoad[BRIDGE_LOAD_LIVE_ECC]	= FALSE;
		m_bApplyLoad[BRIDGE_LOAD_TANK]		= FALSE;
		m_bApplyLoad[BRIDGE_LOAD_TOAP_BOTH]	= IsOutsideWall();
		m_bApplyLoad[BRIDGE_LOAD_TOAP_ONE]	= IsOutsideWall();
		m_bApplyLoad[BRIDGE_LOAD_TEMP1]		= TRUE;
		m_bApplyLoad[BRIDGE_LOAD_TEMP2]		= TRUE;
		m_bApplyLoad[BRIDGE_LOAD_SHRINK]	= TRUE;
		m_bApplyLoad[BRIDGE_LOAD_SB]		= TRUE;
		m_bApplyLoad[BRIDGE_LOAD_LR]		= TRUE;
		m_bApplyLoad[BRIDGE_LOAD_EQ]		= TRUE;
		m_bApplyLiveLoadEQ				= FALSE;
	}

	m_bApplyLoad[BRIDGE_LOAD_SUBSID]	= (IsRcSlab() == FALSE && IsBoxType() == FALSE) ? TRUE : FALSE;
	m_bApplyLoad[BRIDGE_LOAD_WATERHORI]		= (IsBoxType() && m_bApplyGroundWaterLevel) ? TRUE : FALSE;
	m_bApplyLoad[BRIDGE_LOAD_WATERVERT]	= (IsBoxType() && m_bApplyGroundWaterLevel) ? TRUE : FALSE;

	m_bApplyAnchorForce = (IsBoxType() && m_nTypeLower == BOXFOOTING_TYPE_ANCHOR) ? TRUE : FALSE;

	if(m_nTypeBridge == BRIDGE_TYPE_SLAB)
	{
		m_bApplyLoad[BRIDGE_LOAD_SOIL]	= FALSE;
	}

	SetCombineNormalDWORD();
	SetCombineEarthQuakeDWORD();
}

// 시종점 본체 비계 좌표
// 브라켓에서 필요한 브라켓,좌측헌치,우측헌치,좌측날개벽,우측날개벽을 골라내서 trim해서 외곽을 구한다.
void CRcBridgeApp::GetTvScaffoldPlane(CTwinVectorArray &tvArr, CTwinVectorArray &tvArrCen, BOOL bStt)
{
	double dWidthScaffold	= m_dThickScaffold;	// 비계 두께
	CWingWall *pWingL	= GetWingWall(bStt, TRUE);
	CWingWall *pWingR	= GetWingWall(bStt, FALSE);

	tvArr.RemoveAll();
	tvArrCen.RemoveAll();

	// 브라켓 길이 설정 옵션 적용시 비계 길이가 짧게 나올 수 있으므로
	// 비계 평면도 구할때는 브라켓 길이 설정옵선을 꺼 둔다.
	BOOL bBracketLengthStt	= m_bBracketLengthStt;
	BOOL bBracketLengthEnd	= m_bBracketLengthEnd;
	m_bBracketLengthStt	= FALSE;
	m_bBracketLengthEnd	= FALSE;

	GetTvWingWallPlane(bStt);

	// 브라켓 길이 설정 옵션 다시 살림
	m_bBracketLengthStt	= bBracketLengthStt;
	m_bBracketLengthEnd	= bBracketLengthEnd;

	CTwinVectorArray tvArrPlane;
	tvArrPlane	= m_tvArrPlaneWingWall;
	CString sStt	= bStt ? "시점" : "종점";

	// 브라켓을 비계두께만큼 옵셋한 위치에서 trim해서 비계 외곽 좌표를 구한다.
	// 단 날개벽이 없는 쪽은 날개벽 외측 선을 길게 뽑아내서 날개벽 내측 라인처럼 인식하도록 한뒤 작업함. 
	CTwinVector tvBracket		= tvArrPlane.GetTvByInfo("브라켓");
	CTwinVector tvBracket2		= tvBracket;
	CTwinVector tvCenter		= tvBracket;
	CTwinVector tvHunchLeft, tvHunchRight;
	CTwinVector tvWingLeft, tvWingRight;

	// offset한 브라켓 라인을 충분히 길게 준다.
	tvBracket2.m_sInfo	= "브라켓2";
	Offset(tvBracket2.m_v1, tvBracket2.m_v2, -dWidthScaffold);
	CTwinVector tvBracket2Tmp	= tvBracket2;
	double dLen	= tvBracket2.GetLength();
	tvBracket2.m_v1	= tvBracket2.m_v1 + tvBracket2.GetXyDir(TRUE) * dLen;
	tvBracket2.m_v2	= tvBracket2.m_v2 + tvBracket2.GetXyDir() * dLen;
	
	// 중심선을 길게 준다
	tvCenter.m_sInfo	= "중심선";
	Offset(tvCenter.m_v1, tvCenter.m_v2, -dWidthScaffold/2);
	CTwinVector tvCenterTmp	= tvCenter;
	dLen	= tvCenter.GetLength();
	tvCenter.m_v1	= tvCenter.m_v1 + tvCenter.GetXyDir(TRUE) * dLen;
	tvCenter.m_v2	= tvCenter.m_v2 + tvCenter.GetXyDir() * dLen;

	// 좌측 날개벽, 좌측 헌치
	if(!IsWingWall(bStt, TRUE) || pWingL->m_nAttachPos == 1)
	{
		CTwinVector tv;
		GetXyMatchLineAndLine(tvBracket2.m_v1, tvBracket2.GetXyDir(), tvBracket.m_v1, tvBracket.GetXyDir().Rotate90(), tv.m_v2);
		tv.m_v1			= tvBracket.m_v1;
		tvWingLeft		= tv;
		tvHunchRight	= CTwinVector(tv.m_v1, tv.m_v1);;
	}
	else
	{
		tvWingLeft	= tvArrPlane.GetTvByInfo("좌측날개벽내부헌치후");
		tvHunchLeft	= tvArrPlane.GetTvByInfo("좌측날개벽"+sStt+"헌치");

		CDPoint vAngMatch	= tvWingLeft.GetXyDir();
		GetXyMatchLineAndLine(tvWingLeft.m_v1, vAngMatch, tvBracket2.m_v1, tvBracket2.GetXyDir(), tvWingRight.m_v1);
	}

	// 우측 날개벽, 우측 헌치
	if(!IsWingWall(bStt, FALSE) || pWingR->m_nAttachPos == 1)
	{
		CTwinVector tv;
		GetXyMatchLineAndLine(tvBracket2.m_v1, tvBracket2.GetXyDir(), tvBracket.m_v2, tvBracket.GetXyDir().Rotate90(), tv.m_v2);
		tv.m_v1			= tvBracket.m_v2;
		tvWingRight		= tv;
		tvHunchRight	= CTwinVector(tv.m_v1, tv.m_v1);;
	}
	else
	{
		tvWingRight		= tvArrPlane.GetTvByInfo("우측날개벽내부헌치후");
		tvHunchRight	= tvArrPlane.GetTvByInfo("우측날개벽"+sStt+"헌치");
		CDPoint vAngMatch	= tvWingRight.GetXyDir();
		GetXyMatchLineAndLine(tvWingRight.m_v1, vAngMatch, tvBracket2.m_v1, tvBracket2.GetXyDir(), tvWingRight.m_v1);
	}

	tvWingLeft.m_sInfo		= "좌측날개벽";
	tvHunchLeft.m_sInfo		= "좌측헌치";
	tvWingRight.m_sInfo		= "우측날개벽";
	tvHunchRight.m_sInfo	= "우측헌치";

	tvArr.Add(tvWingLeft);
	if(tvHunchLeft.GetLength() > 0)
		tvArr.Add(tvHunchLeft);
	if(tvHunchRight.GetLength() > 0)
		tvArr.Add(tvHunchRight);
	tvArr.Add(tvWingRight);

	// 필요한 선을 모았으면 trim한다.
	CRebarPlacing rb;
	CDPointArray xyArrMatch;
	CVectorArray vecArr;
	tvArr.GetToVectorArray(vecArr);

	// 브라켓 후면에 대해서 교차점 정리
	rb.GetXyMatchSegAndVectorArrayPolygon(tvBracket2.m_v1, tvBracket2.m_v2, vecArr, xyArrMatch, TRUE);
	rb.TrimTvArrayByLine(tvArr, tvBracket2.m_v1, tvBracket2.m_v2, TRUE, TRUE, FALSE);
	rb.RemoveSamePoint(xyArrMatch, TRUE);
	if(xyArrMatch.GetSize() > 1)
	{
		tvBracket2.m_v1	= xyArrMatch.GetAt(0);
		tvBracket2.m_v2	= xyArrMatch.GetAt(1);
	}
	else
	{
		tvBracket2	= tvBracket2Tmp;
	}
	
	// 브라켓에 대해서 교차점 정리
	xyArrMatch.RemoveAll();
	rb.GetXyMatchSegAndVectorArrayPolygon(tvBracket.m_v1, tvBracket.m_v2, vecArr, xyArrMatch, TRUE);
	rb.TrimTvArrayByLine(tvArr, tvBracket.m_v1, tvBracket.m_v2, FALSE, TRUE, FALSE);
	rb.RemoveSamePoint(xyArrMatch, TRUE);
	if(xyArrMatch.GetSize() > 1)
	{
		tvBracket.m_v1	= xyArrMatch.GetAt(0);
		tvBracket.m_v2	= xyArrMatch.GetAt(1);
	}
	else
	{
//		tvBracket	= tvBracket2Tmp;
	}
	
	// 중심선에 대해서 교차점 정리
	xyArrMatch.RemoveAll();
	rb.GetXyMatchSegAndVectorArrayPolygon(tvCenter.m_v1, tvCenter.m_v2, vecArr, xyArrMatch, TRUE);
	rb.RemoveSamePoint(xyArrMatch, TRUE);
	if(xyArrMatch.GetSize() > 1)
	{
		tvCenter.m_v1	= xyArrMatch.GetAt(0);
		tvCenter.m_v2	= xyArrMatch.GetAt(1);
	}
	else
	{
		tvCenter	= tvCenterTmp;
	}

	// 정리된 선 갱신
	tvArr.Add(tvBracket2);
	tvArr.Add(tvBracket);
	tvArrCen.Add(tvCenter);

	BOOL bRectL	= FALSE;
	BOOL bRectR	= FALSE;
	if(!IsWingWall(bStt, TRUE) || (pWingL && pWingL->m_nAttachPos == 1))
	{
		bRectL	= TRUE;
	}
	if(!IsWingWall(bStt, FALSE) || (pWingR && pWingR->m_nAttachPos == 1))
	{
		bRectR	= TRUE;
	}
	if(bRectL && bRectR)
	{
		tvArr.RemoveAll();
		tvArr.Add(tvBracket);
		tvArr.Add(tvBracket2);
		CTwinVector tv;
		tv.m_v1	= tvBracket.m_v1;
		tv.m_v2	= tvBracket2.m_v1;
		tvArr.Add(tv);

		tv.m_v1	= tvBracket.m_v2;
		tv.m_v2	= tvBracket2.m_v2;
		tvArr.Add(tv);
	}
	
}

// 시종점 비계 단면 좌표 구함.
// 비계 높이를 판단하기 위해 사용함.
void CRcBridgeApp::GetTvScaffoldSection(CTwinVectorArray &tvArr, BOOL bStt)
{
	long nTypeBaseLine		= m_nTypeScaffoldBaseLine;
	double dWidthScaffold	= m_dThickScaffold;	// 비계 두께
	double dGapUpper		= 500;	// 상부에서 비계상면까지 간격
	tvArr.RemoveAll();

	// 시종점 상면/비계 설치되는 X위치 찾기. //////////////////////////////////////
	CString sSide	= _T("");
	if(IsOutWall(bStt))
	{
		sSide	= bStt ? "시점브라켓상부측면" : "종점브라켓상부측면";
	}
	else
	{
		return;
	}

	CTwinVector tvSide	= m_tvArrVertSection.GetTvByInfo(sSide);
	CDRect rect	= m_tvArrVertSection.GetRect();

	// 비계 하면 기준점 찾기 //////////////////////////////////////////////////////
	CFootingApp *pFooting	= bStt ? &m_footingStt : &m_footingEnd;
	CString sFooting		= bStt ? _T("기초상면좌측") : _T("기초상면우측");
	CTwinVector tvFooting	= pFooting->m_tvArrFront.GetTvByInfo(sFooting);
	
	double dYLower = 0;
	if(nTypeBaseLine == 0 && IsOutWall(TRUE))	// 기초상단 기준
	{
		if(IsBoxType())
			dYLower = m_dELSB;
		else
			dYLower = tvFooting.GetXyTop().y;
	}
	if(nTypeBaseLine == 1)	// 지반고 기준
	{
		if(IsBoxType())
			dYLower = m_dELSB;
		else
		{
			CTwinVector tvLowerFooting	= pFooting->m_tvArrFront.GetTvByInfo("기초하면");
			GetLineBase()->GetElevationErathOnStation(tvLowerFooting.GetXyMid().x, dYLower);
		}
	}
	if(nTypeBaseLine == 2)	// 사용자 입력
	{
		dYLower = frM(m_dScaffoldLineEL);
	}

	// 비계 만든다.
	CDRect rectScaffold;
	if(bStt)
	{
		rectScaffold.right	= rect.left;
		rectScaffold.left	= rect.left-dWidthScaffold;
	}
	else
	{
		rectScaffold.left	= rect.right;
		rectScaffold.right	= rect.right+dWidthScaffold;
	}

	rectScaffold.top	= tvSide.GetXyTop().y - dGapUpper;
	rectScaffold.bottom	= dYLower;
	tvArr.SetFromRect(rectScaffold);
}

// 날개벽 비계 평면 좌표
void CRcBridgeApp::GetTvScaffoldWingPlane(CTwinVectorArray &tvArrOut, CTwinVectorArray &tvArrIn, CTwinVectorArray &tvArrCenOut, CTwinVectorArray &tvArrCenIn, CTwinVectorArray &tvArrSideOut, CTwinVectorArray &tvArrSideIn, BOOL bStt, BOOL bLeft)
{
	double dWidthScaffold	= m_dThickScaffold;
	if(bLeft)
		dWidthScaffold	*= -1;
	tvArrOut.RemoveAll();
	tvArrIn.RemoveAll();
	tvArrCenOut.RemoveAll();
	tvArrCenIn.RemoveAll();
	tvArrSideOut.RemoveAll();
	tvArrSideIn.RemoveAll();
	if(!IsWingWall(bStt, bLeft)) return;

	// 날개벽 좌표 한번 구해주고..
	GetTvWingWallPlane(bStt);

	CRebarPlacing rb;
	CString sLeft	= bLeft ? "좌측" : "우측";
	CString sStt	= "시점";
	
	// 날개벽 내측면 ///////////////////////////////////////////////////
	// 구해진 선들을 날개벽 끝단에서 날개벽 각도의 수직으로 trim하기 위한 기준선 
	CTwinVectorArray tvArrMain, tvArrMainCen;
	GetTvScaffoldPlane(tvArrMain, tvArrMainCen, bStt);
	CTwinVector tvOut		= m_tvArrPlaneWingWall.GetTvByInfo(sLeft+"날개벽외측");
	CTwinVector tvEndIn		= m_tvArrPlaneWingWall.GetTvByInfo(sLeft+"날개벽끝단");
	CTwinVector tvBracket	= tvArrMain.GetTvByInfo("브라켓2");
	// 교차점을 확실히 찾기 위해 선을 좀 길게 만듬.
	tvEndIn.m_v1	= tvEndIn.m_v2;//+(tvOut.GetXyDir().Rotate90() * fabs(dWidthScaffold*2));
	tvEndIn.m_v2	= tvEndIn.m_v2+(tvOut.GetXyDir().Rotate90() * dWidthScaffold);
	tvBracket.m_v1	= tvBracket.m_v1+(tvBracket.GetXyDir(TRUE) * fabs(dWidthScaffold*2));
	tvBracket.m_v2	= tvBracket.m_v2+(tvBracket.GetXyDir() * fabs(dWidthScaffold*2));

	// 날개벽 내측 구함 .
	CTwinVectorArray tvArrIn1, tvArrIn2;
	tvArrIn1.Add(m_tvArrPlaneWingWall.GetTvByInfo(sLeft+"날개벽내부헌치전"));
	tvArrIn1.Add(m_tvArrPlaneWingWall.GetTvByInfo(sLeft+"날개벽내부헌치"));
	tvArrIn1.Add(m_tvArrPlaneWingWall.GetTvByInfo(sLeft+"날개벽내부헌치후"));
	//tvArrIn1.Add(m_tvArrPlaneWingWall.GetTvByInfo(sLeft+"날개벽"+sStt+"헌치"));

	tvArrIn2	= tvArrIn1;
	tvArrCenIn	= tvArrIn1;
	rb.GetOffsetTvArray(tvArrIn2, dWidthScaffold);
	rb.GetOffsetTvArray(tvArrCenIn, dWidthScaffold/2);

	CDPointArray xyArrMatch;
	CVectorArray vecArr;
	tvArrIn.AddFromTvArray(tvArrIn1);
	tvArrIn.AddFromTvArray(tvArrIn2);
	tvArrIn.GetToVectorArray(vecArr);

	// 브라켓2 왼쪽을 트림(트림하면서 교차점도 찾음)
	CTwinVector tvMatchBracket	= tvBracket;
	rb.GetXyMatchSegAndVectorArrayPolygon(tvMatchBracket.m_v1, tvMatchBracket.m_v2, vecArr, xyArrMatch, TRUE);
	if(xyArrMatch.GetSize() > 1)
	{
		tvMatchBracket.m_v1	= xyArrMatch.GetAt(0);
		tvMatchBracket.m_v2	= xyArrMatch.GetAt(1);

		rb.TrimTvArrayByLine(tvArrIn1, tvBracket.m_v1, tvBracket.m_v2, FALSE, TRUE, FALSE);
		rb.TrimTvArrayByLine(tvArrIn2, tvBracket.m_v1, tvBracket.m_v2, FALSE, TRUE, FALSE);
		rb.TrimTvArrayByLine(tvArrCenIn, tvBracket.m_v1, tvBracket.m_v2, FALSE, TRUE, FALSE);
	}
	// 브라켓과 교차점이 없는 경우(날개벽 각도가 너무 큰 경우임)
	// 날개벽 헌치에서 브라켓에 직각으로 설치
	else 
	{
		CTwinVector tvHunch	= m_tvArrPlaneWingWall.GetTvByInfo(sLeft+"날개벽내부헌치후"/*sLeft+"날개벽"+sStt+"헌치"*/);
		tvHunch.m_v1	= tvHunch.m_v2 + (tvBracket.GetXyDir().Rotate90() * fabs(dWidthScaffold));
		tvMatchBracket	= tvHunch;
		tvBracket		= tvHunch;
		tvBracket.m_v1	= tvBracket.m_v1+(tvBracket.GetXyDir(TRUE) * fabs(dWidthScaffold*2));
		tvBracket.m_v2	= tvBracket.m_v2+(tvBracket.GetXyDir() * fabs(dWidthScaffold*2));

		rb.TrimTvArrayByLine(tvArrIn1, tvBracket.m_v1, tvBracket.m_v2, bLeft, TRUE, FALSE);
		rb.TrimTvArrayByLine(tvArrIn2, tvBracket.m_v1, tvBracket.m_v2, bLeft, TRUE, FALSE);
		rb.TrimTvArrayByLine(tvArrCenIn, tvBracket.m_v1, tvBracket.m_v2, bLeft, TRUE, FALSE);
	}

	// 날개벽 끝단에서 왼쪽을 트림(트림하면서 교차점도 찾음)
	CTwinVector tvMatchEndIn	= tvEndIn;
	/*
	rb.GetXyMatchSegAndVectorArrayPolygon(tvMatchEndIn.m_v1, tvMatchEndIn.m_v2, vecArr, xyArrMatch, TRUE);
	if(xyArrMatch.GetSize() > 1)
	{
		tvMatchEndIn.m_v1	= xyArrMatch.GetAt(0);
		tvMatchEndIn.m_v2	= xyArrMatch.GetAt(1);
	}

	rb.TrimTvArrayByLine(tvArrIn1, tvEndIn.m_v1, tvEndIn.m_v2, TRUE, TRUE, FALSE);
	rb.TrimTvArrayByLine(tvArrIn2, tvEndIn.m_v1, tvEndIn.m_v2, TRUE, TRUE, FALSE);
	rb.TrimTvArrayByLine(tvArrCenIn, tvEndIn.m_v1, tvEndIn.m_v2, TRUE, TRUE, FALSE);
	*/
	
	// 수정된 좌표를 보관
	tvArrIn.RemoveAll();
	long nSize	= tvArrIn1.GetSize();
	long nSize2	= tvArrIn2.GetSize();
	if(nSize > 0 && nSize2 > 0)
	{
		CTwinVector tv;
		tvArrIn.AddFromTvArray(tvArrIn1);
		tvArrIn.AddFromTvArray(tvArrIn2);
		tvArrIn.Add(tvMatchBracket);
		tvArrIn.Add(tvMatchEndIn);
	}
			
	// 날개벽 외측면 //////////////////////////////////////////////////////////
	// 날개벽 외측면은 항상 1자이기 때문에 그냥 옵셋해서 사각형 구하면 됨.
	tvOut		= m_tvArrPlaneWingWall.GetTvByInfo(sLeft+"날개벽외측");
	CTwinVector tvOut2		= tvOut;
	CTwinVector tvCenOut	= tvOut;
	Offset(tvOut2.m_v1, tvOut2.m_v2, dWidthScaffold);
	Offset(tvCenOut.m_v1, tvCenOut.m_v2, dWidthScaffold/2);
	
	CTwinVector tvEnd	= CTwinVector(tvOut.m_v1, tvOut2.m_v1);
	CTwinVector tvStt	= CTwinVector(tvOut.m_v2, tvOut2.m_v2);

	if(m_nTypeScaffoldSideLocation == SCOFFOLD_WING_CON)
	{
		long nPM = ((bStt && !bLeft) || (!bStt && bLeft)) ? 1 : -1;
		double dOffset = 0;

		CTwinVectorArray tvArrSideWing;
		CTwinVector tvSideEnd;
		m_tvArrPlaneWingWall.GetTvArrByInfo(sLeft, tvArrSideWing, FALSE, FALSE);
		tvSideEnd = tvArrSideWing.GetTvByInfo(_T("끝단"), FALSE);

		if(tvArrSideWing.GetSize() > 0 && tvSideEnd.GetLength() > 0)
		{
			CTwinVector tvSideOut, tvSideIn, tvE, tvF, tvSideCen;
			tvSideOut = tvSideEnd;
									
			tvSideOut.m_v1 = tvSideOut.m_v1 + (tvSideOut.GetXyDir(TRUE) * fabs(dWidthScaffold));
			tvSideOut.m_v2 = tvSideOut.m_v2 + (tvSideOut.GetXyDir() * fabs(dWidthScaffold));
			tvSideOut.m_sInfo = _T("날개벽측면_비계측면내측");
			tvSideIn = tvSideCen = tvSideOut;
			tvE.m_v1 = tvSideOut.m_v1;
			tvF.m_v2 = tvSideOut.m_v2;
			tvArrSideOut.Add(tvSideOut);

			dOffset = fabs(dWidthScaffold) * nPM;
			rb.Offset(tvSideIn,  dOffset, dOffset);
			tvSideIn.m_sInfo = _T("날개벽측면_비계측면외측");
			tvE.m_v2 = tvSideIn.m_v1;
			tvF.m_v1 = tvSideIn.m_v2;
			swap(tvSideIn.m_v1, tvSideIn.m_v2);
			tvArrSideOut.Add(tvSideIn);

			tvE.m_sInfo = _T("날개벽측면_비계끝면");
			tvArrSideOut.Add(tvE);
			
			tvE.m_sInfo = _T("날개벽측면_비계시작면");
			tvArrSideOut.Add(tvF);

			rb.Offset(tvSideCen,  dOffset/2, dOffset/2);
			tvSideCen.m_sInfo = _T("날개벽측면_비계중심");
			tvArrSideIn.Add(tvSideCen);
		}
	}

	// 보관
	tvArrOut.Add(tvEnd);
	tvArrOut.Add(tvOut);
	tvArrOut.Add(tvStt);
	tvArrOut.Add(tvOut2);

	tvArrCenOut.Add(tvCenOut);
}

long CRcBridgeApp::GetSizeCombo(long nTypeDesign, BOOL bRealSize)
{
	long nSizeCombo = 0;

	if(nTypeDesign==COMBO_SERVICE)    nSizeCombo = m_MatComboUse.GetRow();
	if(nTypeDesign==COMBO_ULTIMATE)   nSizeCombo = m_MatComboUlt.GetRow();
	if(nTypeDesign==COMBO_ECCENTRIC)  nSizeCombo = m_MatComboEcc.GetRow();
	if(nTypeDesign==COMBO_EARTHQUAKE) nSizeCombo = m_MatComboEQ.GetRow();
	if(nTypeDesign==COMBO_EARTHQUAKE2) nSizeCombo = m_MatComboEQ.GetRow();

	if(bRealSize == FALSE || nTypeDesign == COMBO_EARTHQUAKE || nTypeDesign == COMBO_EARTHQUAKE2)
	{
		return nSizeCombo;
	}

	// CARcBridgeDesignFrame의 m_nCountSubsidence와 같게 설정이 되어야 한다.
	long nCountSubsidence = GetCountSubsidence();
	
	BOOL bEnvToapOne = m_bSelectEnvToapOne;
	BOOL bEnvSubsid = (nCountSubsidence > 1 && m_bSelectEnvSubsid) ? TRUE : FALSE;

	long nSizeRealCombo = 0;
	long nCountSubsid = MAX(1, nCountSubsidence);
	for(long nComboSubsid = 0; nComboSubsid < nCountSubsid; nComboSubsid++)
	{
		if(bEnvSubsid && nComboSubsid == 1) break;

		for(long nComboToapOne = 0; nComboToapOne < 2; nComboToapOne++)
		{
			if(bEnvToapOne && nComboToapOne == 1) break;
			for(long ix = 0; ix < nSizeCombo; ix++)
			{
				DWORD dWCombo = GetLoadCombo(ix, nTypeDesign);
				if(nComboSubsid > 0 && (dWCombo&LOAD_CASE_SUBSID)==FALSE) continue;
				if(nComboToapOne > 0 && (dWCombo&LOAD_CASE_TOAP_ONE)==FALSE) continue;

				nSizeRealCombo++;
			}
		}
	}


	return nSizeRealCombo;
}

long CRcBridgeApp::GetBaseIndexCombo( long nTypeDesign, long IndexComboReal )
{
	if(nTypeDesign == COMBO_EARTHQUAKE || nTypeDesign == COMBO_EARTHQUAKE2)
	{
		return IndexComboReal;
	}

	long nIndex = 0;

	long nSizeCombo = 0;
	if(nTypeDesign==COMBO_SERVICE)    nSizeCombo = m_MatComboUse.GetRow();
	if(nTypeDesign==COMBO_ULTIMATE)   nSizeCombo = m_MatComboUlt.GetRow();
	if(nTypeDesign==COMBO_ECCENTRIC)  nSizeCombo = m_MatComboEcc.GetRow();
	
	// CARcBridgeDesignFrame의 m_nCountSubsidence와 같게 설정이 되어야 한다.
	long nCountSubsidence = GetCountSubsidence();

	BOOL bEnvToapOne = m_bSelectEnvToapOne;
	BOOL bEnvSubsid = (nCountSubsidence > 1 && m_bSelectEnvSubsid) ? TRUE : FALSE;

	long nSizeRealCombo = 0;
	long nCountSubsid = MAX(1, nCountSubsidence);
	for(long nComboSubsid = 0; nComboSubsid < nCountSubsid; nComboSubsid++)
	{
		if(bEnvSubsid && nComboSubsid == 1) break;

		for(long nComboToapOne = 0; nComboToapOne < 2; nComboToapOne++)
		{
			if(bEnvToapOne && nComboToapOne == 1) break;
			for(long nComboLR = 0; nComboLR < 2; ++nComboLR)
			{
				for(long ix = 0; ix < nSizeCombo; ix++)
				{
					DWORD dWCombo = GetLoadCombo(ix, nTypeDesign);
					if(nComboSubsid > 0 && (dWCombo&LOAD_CASE_SUBSID)==FALSE) continue;
					if(nComboToapOne > 0 && (dWCombo&LOAD_CASE_TOAP_ONE)==FALSE) continue;
					if(nComboLR > 0 && (dWCombo&LOAD_CASE_LR)==FALSE) continue;

					if(nIndex == IndexComboReal)
					{
						return ix;
					}
					nIndex++;
				}
			}
		}
	}

	return IndexComboReal;
}

// nTypeCombo = 0 : 사용하중
// nTypeCombo = 1 : 계수하중
// nTypeCombo = 2 : 최대편심
// nTypeCombo = 3 : 지진하중
DWORD CRcBridgeApp::GetLoadCombo(long nCombo, long nTypeCombo)
{
	DWORD dWCase = 0x00;

	if(nTypeCombo<0 || nTypeCombo>3) return 0x00;

	CMatrix MatCombo;
	if(nTypeCombo==LOAD_COMBINE_USE) MatCombo = m_MatComboUse;
	if(nTypeCombo==LOAD_COMBINE_ULT) MatCombo = m_MatComboUlt;
	if(nTypeCombo==LOAD_COMBINE_ECC) MatCombo = m_MatComboEcc;
	if(nTypeCombo==LOAD_COMBINE_EQ)  MatCombo = m_MatComboEQ;

	if(nCombo>=MatCombo.GetRow() || nCombo<0) return dWCase;

	BOOL bApplyToap = (m_bApplyLoad[BRIDGE_LOAD_TOAP_BOTH] || m_bApplyLoad[BRIDGE_LOAD_TOAP_ONE]) ? TRUE : FALSE;

	// 하중이 선택되어 있고 하중계수값이 있는 경우에 
	if(m_bApplyLoad[BRIDGE_LOAD_DEAD]		&& MatCombo(nCombo,BRIDGE_LOAD_DEAD)>0)			dWCase |= LOAD_CASE_DEAD;
	if(m_bApplyLoad[BRIDGE_LOAD_SOIL]		&& MatCombo(nCombo,BRIDGE_LOAD_SOIL)>0)			dWCase |= LOAD_CASE_SOIL;
	if(m_bApplyLoad[BRIDGE_LOAD_LIVE_FULL]	&& MatCombo(nCombo,BRIDGE_LOAD_LIVE_FULL)>0)	dWCase |= LOAD_CASE_LIVE_FULL;
	if(m_bApplyLoad[BRIDGE_LOAD_LIVE_ECC]	&& MatCombo(nCombo,BRIDGE_LOAD_LIVE_ECC)>0)		dWCase |= LOAD_CASE_LIVE_ECC;
	if(m_bApplyLoad[BRIDGE_LOAD_TANK]		&& MatCombo(nCombo,BRIDGE_LOAD_TANK)>0)			dWCase |= LOAD_CASE_TANK;
	if(bApplyToap	&& MatCombo(nCombo,BRIDGE_LOAD_TOAP_BOTH)>0)	dWCase |= LOAD_CASE_TOAP_BOTH;
	if(m_bApplyLoad[BRIDGE_LOAD_TOAP_ONE]	&& MatCombo(nCombo,BRIDGE_LOAD_TOAP_ONE)>0)		dWCase |= LOAD_CASE_TOAP_ONE;
	if(m_bApplyLoad[BRIDGE_LOAD_TEMP1]		&& MatCombo(nCombo,BRIDGE_LOAD_TEMP1)>0)		dWCase |= LOAD_CASE_TEMP1;	
	if(m_bApplyLoad[BRIDGE_LOAD_TEMP2]		&& MatCombo(nCombo,BRIDGE_LOAD_TEMP2)>0)		dWCase |= LOAD_CASE_TEMP2;
	if(m_bApplyLoad[BRIDGE_LOAD_SHRINK]		&& MatCombo(nCombo,BRIDGE_LOAD_SHRINK)>0)		dWCase |= LOAD_CASE_SHRINK;
	if(m_bApplyLoad[BRIDGE_LOAD_SUBSID]		&& MatCombo(nCombo,BRIDGE_LOAD_SUBSID)>0)		dWCase |= LOAD_CASE_SUBSID;
	if(m_bApplyLoad[BRIDGE_LOAD_SB]			&& MatCombo(nCombo,BRIDGE_LOAD_SB)>0)			dWCase |= LOAD_CASE_SB;
	if(m_bApplyLoad[BRIDGE_LOAD_LR]			&& MatCombo(nCombo,BRIDGE_LOAD_LR)>0)			dWCase |= LOAD_CASE_LR;
	if(m_bApplyLoad[BRIDGE_LOAD_EQ]			&& MatCombo(nCombo,BRIDGE_LOAD_EQ)>0)			dWCase |= LOAD_CASE_EQ;
	if(m_bApplyLoad[BRIDGE_LOAD_WATERHORI]	&& MatCombo(nCombo,BRIDGE_LOAD_WATERHORI)>0)			dWCase |= LOAD_CASE_WATERHORI;
	if(m_bApplyLoad[BRIDGE_LOAD_WATERVERT]	&& MatCombo(nCombo,BRIDGE_LOAD_WATERVERT)>0)			dWCase |= LOAD_CASE_WATERVERT;

	return dWCase;
}

CBoringDataEx* CRcBridgeApp::GetBoringDataExByStation(double dStation)
{
	CBoringDataEx *pBoringEx = NULL;

	long nSizeBoring = m_pARcBridgeDataStd->m_pArrBoringEx.GetSize();

	double dDist = 0;
	long i=0; for(i=0; i<nSizeBoring; i++)
	{
		CBoringDataEx *pBoringCur = m_pARcBridgeDataStd->m_pArrBoringEx.GetAt(i);

		double dDistCur	= fabs(dStation-frM(pBoringCur->m_dStation));
		if(i==0)
		{
			dDist = fabs(dStation-frM(pBoringCur->m_dStation));
			pBoringEx = pBoringCur;
			continue;
		}
		if(dDist>dDistCur)
		{
			dDist = fabs(dStation-frM(pBoringCur->m_dStation));
			pBoringEx = pBoringCur;
		}
	}

	return pBoringEx;
}


long CRcBridgeApp::GetBoringIndexByStation( double dStation )
{
	long nIdxBoring = 0;

	long nSizeBoring = m_pARcBridgeDataStd->m_pArrBoringEx.GetSize();

	double dDist = 0;
	for(long ixBoring = 0; ixBoring < nSizeBoring; ixBoring++)
	{
		CBoringDataEx *pBoringCur = m_pARcBridgeDataStd->m_pArrBoringEx.GetAt(ixBoring);

		double dDistCur	= fabs(dStation-frM(pBoringCur->m_dStation));
		if(ixBoring==0)
		{
			dDist = fabs(dStation-frM(pBoringCur->m_dStation));
			nIdxBoring = ixBoring;
			continue;
		}

		if(dDist>dDistCur)
		{
			dDist = fabs(dStation-frM(pBoringCur->m_dStation));
			nIdxBoring = ixBoring;
		}
	}

	return nIdxBoring;
}

CBoringDataEx* CRcBridgeApp::GetBoringDataExByJijumNumber(long nJijum)
{
//	long nLine = 0;
	
	CBoringDataEx *pBoringEx = GetBoringDataEx(nJijum);

// 	double dStation = GetStationOnJijum(nJijum,nLine);
// 
// 	CBoringDataEx *pBoringEx = GetBoringDataExByStation(dStation);
	return pBoringEx;
}

double CRcBridgeApp::GetLengthFootingWingWall(BOOL bStt, BOOL bLeft, BOOL bExFoot, BOOL bHeel, int nOut)
{
	if(m_nTypeBridge == BRIDGE_TYPE_BOX)
		return 0;

	CFootingApp* pFooting = bStt ? &m_footingStt : &m_footingEnd;
	CWingWall *pWing = GetWingWall(bStt, bLeft);
	pFooting->GetTvPlan();

	GetTvWingWallPlane(bStt);
	CTwinVectorArray tvArrFoot, tvArrWing;
	tvArrFoot = bExFoot ? pFooting->m_exFooting.m_tvArrPlan : pFooting->m_tvArrplan;
	tvArrWing = m_tvArrPlaneWingWall;

	BOOL bSide = (pWing->m_nAttachPos == 0)? FALSE : TRUE;
	CTwinVector tvWing, tvSlabEnd, tvFoot;
	CString strDir = bLeft ? _T("좌측") : _T("우측");
	CDPoint xyWing(0, 0);
	tvArrWing.GetTvByInfo(_T("슬래브끝단외측"), tvSlabEnd);
	if(nOut==1)
	{
		tvArrWing.GetTvByInfo(strDir+_T("날개벽외측"), tvWing);
		
	}
	else
	{
		tvArrWing.GetTvByInfo(strDir+_T("날개벽내부헌치후"), tvWing);
		if(bSide)
		{
			CTwinVector tvSlabEndIn;
			tvArrWing.GetTvByInfo(_T("슬래브끝단내측"), tvSlabEndIn);
			GetXyMatchSegAndLine(bLeft? tvSlabEndIn.m_v1:tvSlabEndIn.m_v2, bLeft? tvSlabEnd.m_v1:tvSlabEnd.m_v2, tvWing.m_v1, tvWing.GetXyDir(), tvWing.m_v2);
		}
		else
		{
			GetXyMatchLineAndLine(tvWing.m_v1, tvWing.GetXyDir(), tvSlabEnd.m_v2, tvSlabEnd.GetXyDir(), tvWing.m_v2);
		}
		
		swap(tvWing.m_v1, tvWing.m_v2);
	}
	xyWing = tvWing.m_v1;

	CDPoint xyTemp(0,0);
	CDPoint vWing = tvWing.GetXyDir();
	CDPoint vSlab = tvSlabEnd.GetXyDir();
	CDPoint vWingBet = vWing.BetweenAngle(vSlab);
	CDPoint vFoot(0, 0), vBet(0, 0), xyMatch(0, 0), xyPos(0, 0);

	double dLenTemp = 0;
	BOOL bParallel = ((long)Round(vWingBet.GetAngleDegree(), 0)%180 == 0)||bSide? TRUE : FALSE;
	BOOL bMatch = FALSE;
	BOOL bLeftDir = FALSE;
	int nSize = tvArrFoot.GetSize();

	//#29307 날개벽과 기초와의 평면의 교점을 찾을때 종점일 경우에는 nSize i의 순서대로 
	//호출시 종점의 경우에는 기초 tvArr가 순서가 시점과 반대이기 때문에 순서를 맞춰주기 위해
	//Reverse Sorting을 함
	if(!bStt)
	{
		CRebarPlacing rb;
		rb.ReverseRebar(tvArrFoot);
	}

	BOOL bNotMatcht = TRUE;
	int i=0; for(i=0; i<nSize; i++)
	{
		tvFoot = tvArrFoot.GetAt(i);
		vFoot = tvFoot.GetXyDir();
		vBet = vFoot.BetweenAngle(vWing);
		if(Round(vBet.y, 8)==0.0) continue;

		// (ARCBRIDGE-1497) GetXyMatchSegAndLine()함수를 사용하던것을 GetXyMatchSegAndLineOneDir()로 변경
		// ★★★★★★★★ line의 반대방향에 있는게 TRUE로 나오기 때문에 방향을 -로 넣어준다.
		bMatch = GetXyMatchSegAndLineOneDir(tvFoot.m_v1, tvFoot.m_v2, tvWing.m_v1, vWing * (bHeel? 1:-1), xyPos);
		if(bMatch)
		{
			bNotMatcht = FALSE;
			xyMatch = xyPos;
			if(bParallel)
			{
				if(~(xyWing-xyMatch) < ~(xyWing-xyTemp) || xyTemp==CDPoint(0, 0))
					xyTemp = xyMatch;
				continue;
			}

//			xyTemp = xyMatch;
			if(ComparePt(xyMatch, xyWing, "=", 0.5))
			{
				CDPoint vAngW = GetAngleAzimuthWingWall(bStt, bLeft);

				CDPoint xyMatTemp = xyMatch + 100 * vAngW * (bHeel==bLeft? -1:1);
				bLeftDir = !bHeel;
				dLenTemp = 0;
				continue;
			}
			else
				bLeftDir = GetDirPointByLine(xyMatch, tvSlabEnd.m_v2, tvSlabEnd.m_v1);

			if(bHeel==bLeftDir || (ComparePt(xyMatch, xyWing, "=") && bSide)) 
			{
				break;
			}
		}
		else if(bParallel && i==nSize-1 && xyTemp==CDPoint(0, 0)) return dLenTemp;
	}

	double dRet = (bParallel)? ~(xyWing-xyTemp) : ~(xyWing-xyMatch);
	if(bNotMatcht)
		dRet = 0;

	return dRet;
}

CDPoint CRcBridgeApp::GetXyPlaneBracket(BOOL bStt, BOOL bLeft)
{
	CApsApp* pAPS = NULL;
	CLineInfo *pLine	= GetLineBase();
	CWingWall *pWing	= GetWingWall(bStt, bLeft);

	double dBrkLen = 0;
	if(bStt)
	{
		pAPS = bLeft ? &m_apsStt[iLEFT] : &m_apsStt[iRIGHT];
		if(!pAPS->m_bIs)
			pAPS = &m_apsStt[iLEFT];
		dBrkLen = bLeft ? (pAPS->m_dAD1-m_dBrkLenStt[0]) : (pAPS->m_dAD2-m_dBrkLenStt[1]);
	}
	else
	{
		pAPS = bLeft ? &m_apsEnd[iLEFT] : &m_apsEnd[iRIGHT];
		if(!pAPS->m_bIs)
			pAPS = &m_apsEnd[iLEFT];
		dBrkLen = bLeft ? (pAPS->m_dAD1-m_dBrkLenEnd[0]) : (pAPS->m_dAD2-m_dBrkLenEnd[1]);
	}
	
	CDPoint xy(0, 0);
	double lr = bLeft ? -1.0 : 1.0;
	double dSta = bStt ? m_dStationBridgeStt : m_dStationBridgeStt+m_dLengthBridge;
	double se = bStt ? -1.0 : 1.0;
	double dW = bStt ? m_dWS : m_dWE;		// 벽체 두께
	double dStaSlab = dSta + dW*se/GetAngleJijum(bStt? 0:GetCountJijum()-1).y;

	CTwinVector tvBrk, tvHun, tvTemp;
	CTwinVectorArray tvArrWing;

	tvArrWing = m_tvArrPlaneWingWall;

	CString strDir = bLeft ? _T("좌측") : _T("우측");
	CString str = strDir + ((pWing->m_nAttachPos==0)? "날개벽시점헌치":"벽체끝단");
//	tvArrWing.GetTvByInfo(strDir+_T("날개벽시점헌치"), tvHun);
	tvArrWing.GetTvByInfo(str, tvHun);
	tvArrWing.GetTvByInfo(_T("브라켓"), tvBrk);

	CDPoint vLine = pLine->GetAngleAzimuth(dStaSlab);
	CDPoint vAng = bStt? GetAngleAzimuthJijum(0):GetAngleAzimuthJijum(GetCountJijum()-1);

	CDPoint xyTemp(0, 0);
	tvTemp = tvHun;
	GetXyMatchLineAndLine(tvTemp.m_v1, vLine, tvBrk.m_v1, vAng, xyTemp);
	if(ComparePt(tvTemp.m_v1, xyTemp, _T("=")))
		tvTemp.m_v2 = tvTemp.m_v1 + vLine * 1000*se;
	else
		tvTemp.m_v2 = xyTemp;

	Offset(tvTemp.m_v1, tvTemp.m_v2, dBrkLen*-lr);

	GetXyMatchLineAndLine(tvTemp.m_v1, vLine, tvBrk.m_v1, vAng, xy);

	return xy;
}

// 첫번째로 나오는 기둥식벽체의 인덱스 리턴
// 없으면 -1 리턴
// 없다면 그냥 첫번째 벽체를 리턴
// 그런데 내측벽체도 없다면 그냥
// long CRcBridgeApp::GetIdxInWallOfFirstColumnType()
// {
// 	long nCountInWall	= GetCountInWall();
// 	long nIdxInWall		= -1;
// 	
// 	long i = 0; for(i = 0; i < nCountInWall; i++)
// 	{
// 		CWallApp *pWall	= GetInWall(i);
// 		if(pWall)
// 		{
// 			if(pWall->m_bTopIsColumn)
// 			{
// 				nIdxInWall	= i;
// 				break;
// 			}
// 		}
// 	}
// 
// 	return nIdxInWall;	
// }

// BOOL bCrossSection : 횡단면
// BOOL bMergeLower : 왼쪽이격, 기초폭, 오른쪽이격 선을 하나로 합쳐서 하면으로 만든다
// BOOL bIncludeUpper : 윗 뚜껑 덮을 지..
// BOOL bForDraw : 그리기 용인지(기초 중심이 아닌 지점 기준으로 그림)
void CRcBridgeApp::GetTvTogongLine(CTwinVectorArray& tvArr, long nJijum, BOOL bCrossSection, BOOL bMergeLower, BOOL bIncludeUpper, BOOL bForDraw, long nDeductLeft)
{
	tvArr.RemoveAll();
	int nCnt = GetCountJijum();
	if (nCnt <= nJijum) return;
// 	if(bCrossSection)
// 		SyncBridge(0, TRUE);
// 	else
// 		SyncBridge(0, FALSE);
//	HGBOOL bLSD = m_pARcBridgeDataStd->IsLsdDesign();
// 	long nSlab	= 0;
// 	double dStation = GetStationOnJijum(nJijum, nSlab);
	CFootingApp *pFooting	= GetFootingByJijumNumber(nJijum);
//	CBoringDataEx *pBoring	= GetBoringDataExByStation(dStation);
	CBoringDataEx *pBoring	= GetBoringDataEx(nJijum);
	
	GetTvTogongLine(tvArr, pFooting, pBoring, bIncludeUpper, bCrossSection, bMergeLower, bForDraw, nDeductLeft, nJijum);
}

void CRcBridgeApp::GetSlopeTogong(CTwinVectorArray& tvArr, double dX2, double dY2, CFootingApp* pFooting, CBoringDataEx *pBoring, BOOL bLeft)
{
	int iDer;
	CDPoint cpOld(dX2, dY2);
	CTwinVector tv;

	double dDepth=pFooting->m_dEL_DiggingFirst - dY2;
	
	if(bLeft) iDer = -1;
	else iDer = 1;

	double dH=0;
	
	if(frM(pBoring->m_EL) <= dY2)
	{
		
		dH = pFooting->m_dEL_DiggingFirst-dY2;
		dX2 += dH *  pFooting->m_dDiggingSlopeTosa * iDer;
		dY2 += dH;
		tv.m_v1 = cpOld;
		tv.m_v2 = CDPoint(dX2, dY2);
		tv.m_sInfo = "토사"; 
		cpOld = CDPoint(dX2, dY2);
		if(bLeft) tvArr.InsertAt(0, &tv);
		else tvArr.Add(&tv);
	}
	else
	{
		double dEltop;
		CString strName;
		while(dDepth)
		{
			CBoringData *pBoringData = NULL;
			pBoringData = pBoring->GetBoringByElevation((dY2+1)/1000);
			if(/*pBoringData == NULL && */frM(pBoring->m_EL)>dY2)
			{
				dEltop = frM(pBoringData->m_dELStart);
				if(dEltop <= dY2)
				{
					dEltop = frM(pBoring->m_EL);
				}
				double dS=1;
				switch(pBoringData->GetBoringType())
				{
					case TOSA:
						dS = pFooting->m_dDiggingSlopeTosa;
						strName = _T("토사");
						break;
					case PUNGWHAAM:
						dS = pFooting->m_dDiggingSlopePungwhaam;
						strName = _T("풍화암");
						break;
					case YUNAM:
						dS = pFooting->m_dDiggingSlopeYunam;
						strName = _T("연암");
						break;
					case GYUNGAM:
						dS = pFooting->m_dDiggingSlopeGyungam;
						strName = _T("경암");
						break;
				}
				
				dH = ((pFooting->m_dEL_DiggingFirst > dEltop)? dEltop:pFooting->m_dEL_DiggingFirst);
				dX2 += (dH - dY2)*dS*iDer;
				
				dY2 = dH;
			}
			else if(/*pBoringData == NULL && */pBoring->m_EL<=dY2)
			{
				dH = pFooting->m_dEL_DiggingFirst-dY2;
				dX2 += dH *  pFooting->m_dDiggingSlopeTosa * iDer;
				dY2 += dH;
			}
			
			tv.m_v1 = cpOld;
			tv.m_v2 = CDPoint(dX2, dY2);
			tv.m_sInfo = strName;
			cpOld = CDPoint(dX2, dY2);
			if(bLeft) tvArr.InsertAt(0, &tv);
			else tvArr.Add(&tv);

			dDepth = pFooting->m_dEL_DiggingFirst - dY2;
		}
	}
}


// 도로횡단의 좌측끝(성토사면이 내려가는 위치)의 EL을 산정한다.
// 도로중심위치에서 구간이 설정되지않은 경우 잘못된 값이 산정됨
double CRcBridgeApp::GetElevationRoadSectionLeft(double dStation)
{
	CLineInfo *pLineRoad = m_pARcBridgeDataStd->GetLineApp();		// 도로선형
	double dEle = 0;
	double dELCenter = pLineRoad->GetElevationPlanEl(dStation);		// 도로중심의 ELEVATION

	// 도로중심이 도로횡단의 좌측에 있는 경우
	if(m_dWidthRoadLeft>0)
	{
		return dELCenter;
	}

	// 도로중심이 도로횡단의 좌측에 있는 경우
	if(m_dWidthRoadRight<0)
	{
		dEle = dELCenter;
		long i=0; for(i=m_nQtyHDanRoad-1; i>=0; i--)
		{
			dEle += m_dLengthHDanRoad[i] * m_dSlopeHDanRoad[i];
		}
		return dEle;
	}

	// 도로중심이 도로횡단내에 있는 경우
	double dEleDiff  = 0;
	double dDistRoad = m_dWidthRoadLeft;
	long i=0; for(i=0; i<m_nQtyHDanRoad; i++)
	{
		dDistRoad += m_dLengthHDanRoad[i];
		if(dDistRoad>0) break;
		dEleDiff  -= m_dLengthHDanRoad[i] * m_dSlopeHDanRoad[i];
	}
	dEle = dELCenter + dEleDiff;
	return dEle;
}
// double CRcBridgeApp::GetElevationRoadSectionRight(double dStation)
// {
// 	double dEle = GetElevationRoadSectionLeft(dStation);
// 	double dDistRoad = m_dWidthRoadLeft;
// 	long i=0; for(i=0; i<m_nQtyHDanRoad; i++)
// 	{
// 		dDistRoad += m_dLengthHDanRoad[i];
// 		if(dDistRoad<=0)
// 			dEle -= m_dLengthHDanRoad[i] * m_dSlopeHDanRoad[i];
// 		else
// 			dEle += m_dLengthHDanRoad[i] * m_dSlopeHDanRoad[i];
// 	}
// 
// 	return dEle;
// }
// 시종점에 벽체가 없는 경우 임시로 보여줄 교대의 일부분 좌표
void CRcBridgeApp::GetTvVertSectionAbut(CTwinVectorArray &tvArr, BOOL bStt)
{
	tvArr.RemoveAll();
	if(IsOutWall(bStt)) return;

	CTwinVector tv;
	double sw	= bStt ? 1 : -1;

	double Gap	= 0;
	double WH	= 2000;
	double SL	= 1000;
	double PH	= m_dTS + GetThickPave(FALSE);
	double AW	= 500;
	double AH	= 880;
	double BWU	= 300;	// 브라켓 상면
	double BW	= 800;	// 브라켓 슬래브 너비
	double BH	= 1200;	// 브라켓 측면 높이
	double BSH	= 800;
	double HS	= 400;	// 슈 높이
	tv	= m_tvArrVertSection.GetTvByInfo(bStt ? "시점슬래브측면" : "종점슬래브측면");
	CDPoint xyOrg	= tv.GetXyBottom();
	xyOrg.x	= bStt ? m_dStationBridgeStt : m_dStationBridgeStt+m_dLengthBridge;
	xyOrg.y -= Gap;

	tv.m_v1	= CVector(xyOrg.x, xyOrg.y-HS, 0);
	tv.m_v2	= CVector(xyOrg.x+SL*sw, xyOrg.y-HS, 0);
	tv.m_sInfo	= "슈받침부";
	tvArr.Add(tv);

	tv.m_v1	= tv.m_v2;
	tv.m_v2	= CVector(tv.m_v1.x, tv.m_v1.y - WH, 0);
	tv.m_sInfo	= "교대내측면";
	tvArr.Add(tv);

	tv.m_v1	= CVector(xyOrg.x, xyOrg.y-HS, 0);
	tv.m_v2	= CVector(xyOrg.x, xyOrg.y + PH, 0);
	tv.m_sInfo	= "슈받침부상부측면";
	tvArr.Add(tv);

	tv.m_v1	= tv.m_v2;
	tv.m_v2	= CVector(tv.m_v1.x-AW*sw, tv.m_v1.y, 0);
	tv.m_sInfo	= "교대최상단";
	tvArr.Add(tv);

	tv.m_v1	= tv.m_v2;
	tv.m_v2	= CVector(tv.m_v1.x, tv.m_v2.y - AH, 0);
	tv.m_sInfo	= "브라켓상부측면";
	tvArr.Add(tv);

	tv.m_v1	= tv.m_v2;
	tv.m_v2	= CVector(tv.m_v1.x-BWU*sw, tv.m_v2.y, 0);
	tv.m_sInfo	= "브라켓상면";
	tvArr.Add(tv);

	tv.m_v1	= tv.m_v2;
	tv.m_v2	= CVector(tv.m_v1.x, tv.m_v1.y-BH, 0);
	tv.m_sInfo	= "브라켓측면";
	tvArr.Add(tv);

	tv.m_v1	= tv.m_v2;
	tv.m_v2	= CVector(tv.m_v1.x+BW*sw, tv.m_v1.y-BSH, 0);
	tv.m_sInfo	= "브라켓하면";
	tvArr.Add(tv);

	tv.m_v1	= tv.m_v2;
	tv.m_v2	= CVector(tv.m_v1.x, xyOrg.y-WH-HS, 0);
	tv.m_sInfo	= "교대외측면";
	tvArr.Add(tv);
}

void CRcBridgeApp::GetTvWingSlopeLine(CTwinVectorArray& tvArr, BOOL bStt, BOOL bLeft, double dEl)
{
	tvArr.RemoveAll();

	CTwinVector tv;
	 
	int iDir = (bStt)? 1:-1;
	
	CWingWall* pWingWall = GetWingWall(bStt, bLeft);
	if(!pWingWall->m_bExist) return;
	
	
	double dtempX, dtempY;
	CDPoint cpStt = GetXySectionWingWallStt(bStt, TRUE);
	CDPoint cpEnd;

	cpEnd.x = cpStt.x - ((pWingWall->m_dL1+pWingWall->m_dL2)*iDir);
	cpEnd.y = pWingWall->GetEndEl(FALSE);

	tv.m_v1.x = cpEnd.x + (pWingWall->m_dDistNoriStart * iDir);
	tv.m_v1.y = cpEnd.y;

	BOOL bRun = TRUE;
	BOOL bFrontSodan = FALSE;
	int nCntDan = 0;
	while(bRun)
	{
		nCntDan++;
		CString strName;
		if(nCntDan%2 && nCntDan == 1) strName = _T("1단");
		else if(nCntDan%2 && nCntDan != 1) strName = _T("2단이상");
		else strName = _T("소단");

		double dSlope = (nCntDan == 1)? pWingWall->m_dSlopeNoriFirst:pWingWall->m_dSlopeNoriNext;
		if(dSlope==0) break;
		if(nCntDan%2)
		{
			dtempY = (tv.m_v1.y - pWingWall->m_dHeightMaxSodan > dEl) ? pWingWall->m_dHeightMaxSodan : (tv.m_v1.y - dEl);
			dtempX = dtempY * dSlope;
		}
		else
		{
			dtempY = 0;
			dtempX = pWingWall->m_dWidthSodan;
			if(bFrontSodan)
			{
				dtempX = pWingWall->m_dWidthSodanWallFront;
				strName = _T("벽체전면소단");
				bFrontSodan = FALSE;
			}
		}
		double dX1, dY1;
		dX1 = tv.m_v1.x + dtempX*iDir;
		dY1 = tv.m_v1.y - dtempY;
		
		double dLeft = (bStt) ? tv.m_v1.x : dX1;
		double dRight = (bStt) ? dX1 : tv.m_v1.x;

		if(dLeft<cpStt.x && dRight>cpStt.x)
		{
			if(pWingWall->m_bRetainWallFront)		// 옹벽설치 체크
			{
				dX1 = cpStt.x;
				if(nCntDan%2)
					dtempY = (fabs(tv.m_v1.x - cpStt.x)/dSlope);
				else 
					dtempY = 0;
				dY1  = tv.m_v1.y - dtempY;
				bRun = FALSE;
			}
			else if((pWingWall->m_dWidthSodanWallFront>0) && nCntDan%2)		// 벽체전면 소단
			{
				dX1		= cpStt.x;
				dtempY	= (fabs(tv.m_v1.x - cpStt.x)/dSlope);
				dY1		= tv.m_v1.y - dtempY;
				bFrontSodan = TRUE;
			}
		}

		tv.m_v2.x = dX1;
		tv.m_v2.y = dY1;
		tv.m_sInfo = strName;
		tvArr.Add(&tv);
		
		tv.m_v1 = tv.m_v2;

		if(tv.m_v2.y <= dEl) bRun = FALSE;
	}
	
}

// 평면 브라켓 외곽
// BOOL bIncludeBracket : 브라켓 외측면
// BOOL bIncludeOutWallOutter : 벽체 외측면 
// BOOL bIncludeFRP : 하부슬래브 부상방지저판
void CRcBridgeApp::GetTvPlaneOutWall(CTwinVectorArray &tvArr, BOOL bStt, BOOL bIncludeBracket, BOOL bIncludeOutWallOutter, BOOL bIncludePRF)
{
	tvArr.RemoveAll();
	if(!IsOutWall(bStt)) return;

	CTwinVectorArray tvArrPlane;
	GetTvRebarOut_Plane_UpperSlabStd(tvArrPlane, -1, TRUE);
	CTwinVectorArray tvArrJijum;
	CTwinVector tvJijum;

	CString sStt	= bStt ? "시점" : "종점";
	long i = 0; for(i = 0; i < tvArrPlane.GetSize(); i++)
	{
		if(tvArrPlane.GetAt(i).m_sInfo == sStt)
			tvArrJijum.Add(tvArrPlane.GetAt(i));
	}
	if(tvArrJijum.GetSize() == 0) return;

	tvJijum.m_v1	= !bStt ? tvArrJijum.GetAt(0).m_v1 : tvArrJijum.GetAt(tvArrJijum.GetSize()-1).m_v2;
	tvJijum.m_v2	= !bStt ? tvArrJijum.GetAt(tvArrJijum.GetSize()-1).m_v2 : tvArrJijum.GetAt(0).m_v1;
	tvJijum.m_sInfo	= "슬래브끝단외측";	// 단부 철근 배근을 위해서 필요.
	tvArr.Add(tvJijum);


	if(bIncludeBracket)
	{
		GetTvWingWallPlane(bStt, TRUE, TRUE);
		
		CTwinVector tv	= m_tvArrPlaneWingWall.GetTvByInfo("브라켓");
		tv.m_sInfo	= "브라켓외측";
		GetSwap(tv.m_v1, tv.m_v2);
		tvArr.Add(tv);
	}

	if(bIncludeOutWallOutter)
	{
		CTwinVector tv	= tvJijum;
		tv.m_sInfo	= "벽체외측";
		GetSwap(tv.m_v1, tv.m_v2);
		tvArr.Add(tv);
	}

	if(bIncludePRF)
	{
		CTwinVector tv	= tvJijum;
		Offset(tv.m_v1, tv.m_v2, bStt ? +m_dPRF_W : -m_dPRF_W);
		tv.m_sInfo	= "부상방지저판";
		GetSwap(tv.m_v1, tv.m_v2);
		tvArr.Add(tv);
	}

	CTwinVector tv	= tvJijum;
	Offset(tv.m_v1, tv.m_v2, bStt ? -m_dWS : m_dWE);
	tv.m_sInfo	= "벽체내측";
	GetSwap(tv.m_v1, tv.m_v2);
	tvArr.Add(tv);
}

// 스테이션에서 가장 가까중 지점번호 찾기....
long CRcBridgeApp::GetJijumNumberByStation(double dSta)
{
	long nJijumCur   = 0;
	long nJijumMin   = 0;
	double dStaJijum = GetStationOnJijum(nJijumCur);
	double dDistMin  = fabs(dStaJijum-dSta);

	for(nJijumCur=1; nJijumCur<=m_nQtyJigan; nJijumCur++)
	{
		dStaJijum = GetStationOnJijum(nJijumCur);
		if(fabs(dStaJijum-dSta)<dDistMin)
		{
			dDistMin  = fabs(dStaJijum-dSta);
			nJijumMin = nJijumCur;
		}
	}

	return nJijumMin;
}

// 스테이션으로 해당하는 지점 번호 찾아내기 : 벽체두께를 고려하여 가까운 벽체를 찾는다...
long CRcBridgeApp::GetJijumWallByStation(double dSta)
{
	long nJijum		= -1;
	double dStaStt	= m_dStationBridgeStt;
	double dStaEnd	= m_dStationBridgeStt + m_dLengthBridge;
	BOOL bFind		= FALSE;
	// 시점벽체에 해당 하는지?
	if(IsOutWall(TRUE))
	{
		if(dStaStt-m_dWS-m_dBHWS <= dSta && dStaStt+m_dHWS >= dSta)
		{
			nJijum	= 0;
			bFind	= TRUE;
		}
	}
	else
	{
		long nJ		= 0;
		CJijum *pJ	= GetJijum(nJ);
		if(pJ)
		{
			if(dStaStt - pJ->m_dLengthUgan[0] <= dSta && dStaStt + pJ->m_dLengthUgan[1] >= dSta)
			{
				nJijum	= nJ;
				bFind	= TRUE;
			}
		}
	}

	// 내측 벽체에 해당 하는지
	if(!bFind)
	{
		long nCountInWall	= GetCountInWall();
		for(long wall = 0; wall < nCountInWall; wall++)
		{
			CWallApp *pWall	= GetInWall(wall);
			BOOL bIsWall	= FALSE;
			if(pWall)
			{
				if(pWall->m_bIs)
					bIsWall	= TRUE;
			}
			
			long nJ			= wall+1;
			if(bIsWall)
			{
				CTwinVector tvLeft	= pWall->m_tvArrFront.GetTvByInfo("벽체좌측");
				CTwinVector tvRight	= pWall->m_tvArrFront.GetTvByInfo("벽체우측");
				if(tvLeft.GetXyTop().x >= dSta && tvRight.GetXyTop().x <= dSta)
				{
					nJijum	= nJ;
					bFind	= TRUE;
				}
			}
			else
			{
				double dStaCur	= GetStationOnJijum(nJ, 0);
				CJijum *pJ	= GetJijum(nJ);
				if(pJ)
				{
					if(dSta >= dStaCur-pJ->m_dLengthUgan[0] && dSta <= dStaCur+pJ->m_dLengthUgan[1])
					{
						nJijum	= nJ;
						bFind	= TRUE;
					}
				}
			}

			if(bFind) break;
		}
	}



	// 종점벽체에 해당하는지?
	if(!bFind)
	{
		if(IsOutWall(FALSE))
		{
			if(dStaEnd-m_dHWE <= dSta && dStaEnd+m_dWE+m_dBHWE >= dSta)
			{
				nJijum	= m_nQtyJigan;
				bFind	= TRUE;
			}
		}
		else
		{
			long nJ		= m_nQtyJigan;
			CJijum *pJ	= GetJijum(nJ);
			if(pJ)
			{
				if(dStaEnd - pJ->m_dLengthUgan[0] <= dSta && dStaEnd + pJ->m_dLengthUgan[1] >= dSta)
				{
					nJijum	= nJ;
					bFind	= TRUE;
				}
			}
		}
	}


	// 각 벽체를 기준으로 검사해서 못 찾았다면 가장 가까운 지점을 찾는다.
	if(!bFind)
	{
		double dMinDist	= 0;
		long i = 0; for(i = 0; i <= m_nQtyJigan; i++)
		{
			if(i == 0)
			{
				nJijum		= i;
				dMinDist	= fabs(GetStationOnJijum(i, 0) - dSta);
			}
			else
			{
				if(fabs(GetStationOnJijum(i, 0) - dSta) < dMinDist)
				{
					nJijum		= i;
					dMinDist	= fabs(GetStationOnJijum(i, 0) - dSta);
				}
			}
		}
	}


	return nJijum;
}


void CRcBridgeApp::SetCombineNormalDWORD()
{
	m_dWLoadNormal = 0x00;
	if(m_bApplyLoad[BRIDGE_LOAD_DEAD])		m_dWLoadNormal |= LOAD_CASE_DEAD;
	if(m_bApplyLoad[BRIDGE_LOAD_SOIL])		m_dWLoadNormal |= LOAD_CASE_SOIL;
	if(m_bApplyLoad[BRIDGE_LOAD_LIVE_FULL])	m_dWLoadNormal |= LOAD_CASE_LIVE_FULL;
	if(m_bApplyLoad[BRIDGE_LOAD_LIVE_ECC])	m_dWLoadNormal |= LOAD_CASE_LIVE_ECC;
	if(m_bApplyLoad[BRIDGE_LOAD_TANK])		m_dWLoadNormal |= LOAD_CASE_TANK;
	if(m_bApplyLoad[BRIDGE_LOAD_TOAP_BOTH])	m_dWLoadNormal |= LOAD_CASE_TOAP_BOTH;
	if(m_bApplyLoad[BRIDGE_LOAD_TOAP_ONE])	m_dWLoadNormal |= LOAD_CASE_TOAP_ONE;
	if(m_bApplyLoad[BRIDGE_LOAD_TEMP1])		m_dWLoadNormal |= LOAD_CASE_TEMP1;
	if(m_bApplyLoad[BRIDGE_LOAD_TEMP2])		m_dWLoadNormal |= LOAD_CASE_TEMP2;
	if(m_bApplyLoad[BRIDGE_LOAD_SHRINK])	m_dWLoadNormal |= LOAD_CASE_SHRINK;
	if(m_bApplyLoad[BRIDGE_LOAD_SUBSID])	m_dWLoadNormal |= LOAD_CASE_SUBSID;
	if(m_bApplyLoad[BRIDGE_LOAD_SB])		m_dWLoadNormal |= LOAD_CASE_SB;
	if(m_bApplyLoad[BRIDGE_LOAD_LR])		m_dWLoadNormal |= LOAD_CASE_LR;
	if(m_bApplyLoad[BRIDGE_LOAD_WATERHORI])		m_dWLoadNormal |= LOAD_CASE_WATERHORI;
	if(m_bApplyLoad[BRIDGE_LOAD_WATERVERT])	m_dWLoadNormal |= LOAD_CASE_WATERVERT;
}

void CRcBridgeApp::SetCombineEarthQuakeDWORD()
{
	m_dWLoadEQ = 0x00;
	if(m_bApplyLoad[BRIDGE_LOAD_DEAD])		m_dWLoadEQ |= LOAD_CASE_DEAD;
	if(m_bApplyLoad[BRIDGE_LOAD_SOIL])		m_dWLoadEQ |= LOAD_CASE_SOIL;
	if(m_bApplyLiveLoadEQ)
	{
		if(m_bApplyLoad[BRIDGE_LOAD_LIVE_FULL])	m_dWLoadEQ |= LOAD_CASE_LIVE_FULL;
		if(m_bApplyLoad[BRIDGE_LOAD_LIVE_ECC])	m_dWLoadEQ |= LOAD_CASE_LIVE_ECC;
	}
	if(m_bApplyLoad[BRIDGE_LOAD_TOAP_BOTH] || m_bApplyLoad[BRIDGE_LOAD_TOAP_ONE])	m_dWLoadEQ |= LOAD_CASE_TOAP_BOTH;
	if(m_bApplyLoad[BRIDGE_LOAD_EQ])		m_dWLoadEQ |= LOAD_CASE_EQ;
	if(m_bApplyLoad[BRIDGE_LOAD_WATERVERT])	m_dWLoadEQ |= LOAD_CASE_WATERVERT;
	if(m_bApplyLoad[BRIDGE_LOAD_WATERHORI])	m_dWLoadEQ |= LOAD_CASE_WATERHORI;
}

// 상부슬래브 평면 좌표를 가져옴
// nIdxPart는 원하는 부분의 인덱스임
// 각 부분의 종류에 맞게 잘 offset을 해서 가져와야 됨.
// void CRcBridgeApp::GetTvPlaneSlabByPart(CTwinVectorArray &tvArr, long nIdxPart)
// {
// 	tvArr.RemoveAll();
// 
// 
// 	// 시점에서의 시작위치로 좌측선형을 따라갈지 우측선형을 따라갈지 결정한다.
// 	CDPoint A[30];
// 	GetXySlabUpperAct(m_dStationBridgeStt, CDPoint(0, 1), A);
// 	long nQtyHDan = GetQtyHDanAct(m_dStationBridgeStt, CDPoint(0, 1));
// 	if(nQtyHDan <= nIdxPart) return;
// 
// 	double dDist	= A[nIdxPart].x;
// 	long nLeft		= dDist == 0 ? 0 : (dDist < 0 ? -1 : 1);
// 	CLineInfo *pLine	= GetLineLeftCenRight(nLeft);
// 	if(!pLine) return;
// 
// 	// 해당 선형의 모든 sta에서의 구간의 위치를 가져옴
// // 	long nCountSta	= pLine->GetQtyIp();
// // 	long i = 0; for(i = 0; i < nCountSta; i++)
// // 	{
// // 		CLineSplit *pSp	= pLine->GetIPObj(i);
// // 		
// // 	} 
// }


void CRcBridgeApp::MakeLineBaseVert(BOOL bVert)
{
	CLineInfoApp *pLine	= static_cast<CLineInfoApp*>(GetLineBase());
	*m_pLineBaseVert	= *pLine;
	m_pLineBaseVert->ReSetSearchIp();

	CTwinVectorArray tvArrCenter;
	CTwinVector tvCenter;
	GetTvCenterPlane_UpperSlab(tvArrCenter, FALSE, FALSE, 0, bVert);
	tvCenter	= tvArrCenter.GetAt(0);

	double dExt	= 10000;
	tvCenter.m_v1	= tvCenter.m_v1 + tvCenter.GetXyDir(TRUE) * dExt;
	tvCenter.m_v2	= tvCenter.m_v2 + tvCenter.GetXyDir() * dExt;

	// 만들어진 중심선으로 선형을 새로 만듬
	m_pLineBaseVert->m_xyBp.x	= tvCenter.m_v1.x;
	m_pLineBaseVert->m_xyBp.y	= tvCenter.m_v1.y;
	

	m_pLineBaseVert->SetQtyIp(0);
	m_pLineBaseVert->m_dStationBp	= -dExt;

	m_pLineBaseVert->m_xyEp.x	= tvCenter.m_v2.x;
	m_pLineBaseVert->m_xyEp.y	= tvCenter.m_v2.y;


	CString strErrorMsg = _T("선형 데이터가 올바르지 않습니다.  ");
	m_pLineBaseVert->SetDataIp(strErrorMsg);
}

// 평면도상의 외곽철근좌표들
// 시계방향으로 모든 라인을 연결
// 슬래브교일 경우에는 시점 종점을 기준으로 하면 되지만, 외측벽체가 있는 경우.. 브라켓 이전까지 연장해서 
// 배근해야 된다.
// BOOL bExp : 가각적용된 외각인지??
// long nUpper : 중심선형을 기준으로 iUPPER : 상, iLOWER : 하. 2 : 모두
// BOOL bCutEnd : 가각 끝단 자르기 적용된 좌표인지?(가각 주철근 배근시 적용 안된걸 기준으로 배치 해야 되기 때문에 옵션을 둠)
void CRcBridgeApp::GetTvRebarOut_Plane_UpperSlab(CTwinVectorArray &tvArr, long nDan, BOOL bExp, long nLeft, BOOL bVert, BOOL bCutEnd)
{
	tvArr.RemoveAll();
	long nExp	= bExp ? iEXP : iNORMAL;

	CRebarPlacing rb;

	tvArr.RemoveAll();
	CLineInfoApp *pLineR	= static_cast<CLineInfoApp*>(GetLineLeftRight(FALSE));
	CLineInfoApp *pLineL	= static_cast<CLineInfoApp*>(GetLineLeftRight(TRUE));
	if(!pLineR || !pLineL) return;

	// RebarPlacing의 Offset은 선형을 모두 커버할 수 없으므로  ///////////////////////////
	// 선형에 대해서 만큼은 실제 교량 폭과 길이를 피복만큼 그려서 그리는 방법을 사용함.
	// 가각이 없는 부분에 대해서는 offset은 되지만 교차점 정리는 안됨.(선형이기 때문)
	// 어짜피 가각이 없는 경우는 현재로선 offset이 필요 없기 때문에.. 차후에 작업함.
	double dCover	= 0;//nDan > -1 ? GetCoverSide_UpperSlab(nDan) : 0;
	double dOldWidthSlabLeft	= m_dWidthSlabLeft;
	double dOldWidthSlabRight	= m_dWidthSlabRight;
	////////////////////////////////////////////////////////////////////////////////////////////

	CJijum *pJStt	= GetJijum(0);
	CJijum *pJEnd	= GetJijum(m_nQtyJigan);
	CDPoint vAngStt	= pJStt->m_vAngle;
	CDPoint vAngEnd	= pJEnd->m_vAngle;
	double dWS		= m_dWS/vAngStt.y;
	double dWE		= m_dWE/vAngEnd.y;

	// 가각의 R 입력방법이 추가됨(기존 접선방식에 직선의 끝을연결한 R타입으로 변경)
	double dExpSttR[2]	= {m_bExpSttRoundIsTan[0] ? m_dExpSttR[0] : 0, m_bExpSttRoundIsTan[1] ? m_dExpSttR[1] : 0};
	double dExpEndR[2]	= {m_bExpEndRoundIsTan[0] ? m_dExpEndR[0] : 0, m_bExpEndRoundIsTan[1] ? m_dExpEndR[1] : 0};

	// 시점측
	CTwinVectorArray tvArrStt;
	GetTvPlaneSttEndLine(tvArrStt, TRUE, TRUE, 0, bExp);

	// 종점측
	CTwinVectorArray tvArrEnd;
	GetTvPlaneSttEndLine(tvArrEnd, FALSE, TRUE, 0, bExp);

	CTwinVector tvStt;
	tvStt.m_v1	= tvArrStt.GetAt(0).m_v1;
	tvStt.m_sInfo	= tvArrStt.GetAt(0).m_sInfo;
	tvStt.m_v2	= tvArrStt.GetAt(tvArrStt.GetSize()-1).m_v2;
	tvArrStt.RemoveAll();
	tvArrStt.Add(tvStt);

	CTwinVector tvEnd;
	tvEnd.m_v1	= tvArrEnd.GetAt(0).m_v1;
	tvEnd.m_sInfo	= tvArrEnd.GetAt(0).m_sInfo;
	tvEnd.m_v2	= tvArrEnd.GetAt(tvArrEnd.GetSize()-1).m_v2;
	tvArrEnd.RemoveAll();
	tvArrEnd.Add(tvEnd);

	// 우측
	CTwinVectorArray tvArrRight;
	CTwinVectorArray tvArrRightS, tvArrRightE;	// 가각부 시종점
	
	double staSttRight = GetStationByTarget(BRIDGE_OFF_STT_SLAB, 1, bExp);
	double staEndRight = GetStationByTarget(BRIDGE_OFF_END_SLAB, 1, bExp);
	if(IsOutWall(TRUE)) staSttRight -= dWS;
	if(IsOutWall(FALSE)) staEndRight += dWE;

	if(bExp && m_bIsExp)
	{
		CDPoint slabA[4],EndA[4];
		GetXySss(BRIDGE_OFF_STT_SLAB,slabA);
		GetXySss(BRIDGE_OFF_END_SLAB,EndA);

		// 슬래브 우측라인
		BOOL bSttGagak = (m_dExpSttW[1]>0 && m_dExpSttH[1] && m_bIsExp) ? TRUE : FALSE;
		BOOL bEndGagak = (m_dExpEndW[1]>0 && m_dExpEndH[1] && m_bIsExp) ? TRUE : FALSE;
		double staStt = GetStationByTarget(BRIDGE_OFF_STT_SLAB,0,FALSE);// + (bSttGagak ? m_dExpSttW[1] : 0);
		double staEnd = GetStationByTarget(BRIDGE_OFF_END_SLAB,0,FALSE);// - (bEndGagak ? m_dExpEndW[1] : 0);
		CDPoint vAngStt	= GetAngleByTarget(BRIDGE_OFF_STT_SLAB, 1);
		CDPoint vAngEnd	= GetAngleByTarget(BRIDGE_OFF_END_SLAB, 1);
		CDPoint xyStt	= GetXySlabEdgeAct(staStt, vAngStt, FALSE);
		CDPoint xyEnd	= GetXySlabEdgeAct(staEnd, vAngEnd, FALSE);
		pLineR->GetStationMatchVerticalByXy(xyStt, staStt);
		pLineR->GetStationMatchVerticalByXy(xyEnd, staEnd);
		staStt	= pLineR->GetStationByDisLength(staStt, bSttGagak ? m_dExpSttW[1] : 0, m_dWidthSlabRight);
		staEnd	= pLineR->GetStationByDisLength(staEnd, bEndGagak ? -m_dExpEndW[1] : 0, m_dWidthSlabRight);
		//staStt += bSttGagak ? m_dExpSttW[1] : 0;
		//staEnd -= bEndGagak ? m_dExpEndW[1] : 0;

		CDPoint xyW = pLineR->GetXyLineDis(staStt,m_dWidthSlabRight);
		CDPoint xySlabSttEnd = slabA[3];
		CDPoint xySegEnd1 = pLineR->GetXyLineDis(staEnd,m_dWidthSlabRight);
		CDPoint xySegEnd2 = EndA[3];
		CDPoint xyCenStt(0,0),xySegStt(0,0),xyCenEnd(0,0),xySegEnd(0,0);
		if(bSttGagak)
			pLineR->GetXyForRc(m_dWidthSlabRight,dExpSttR[1],xyW,xySlabSttEnd,TRUE ,FALSE, staStt,xyCenStt,xySegStt);
		if(bEndGagak)
			pLineR->GetXyForRc(m_dWidthSlabRight,dExpEndR[1],xySegEnd1,xySegEnd2,FALSE,FALSE, staEnd,xyCenEnd,xySegEnd);
		//
		if(bSttGagak)
		{
			CTwinVector tv;
			
			if(dExpSttR[1]>0)
			{
				CDPoint xyArcEnd = pLineR->GetXyLineDis(staStt,m_dWidthSlabRight);
				double dAngStt = (xySegStt-xyCenStt).Unit().GetAngleDegree();
				double dAngEnd = (xyArcEnd-xyCenStt).Unit().GetAngleDegree();
				tv.m_v1	= xyCenStt + ToDPointFrDegree(dAngStt) * dExpSttR[1];
				tv.m_v2	= xyCenStt + ToDPointFrDegree(dAngEnd) * dExpSttR[1];
				tv.m_v1.z	= 1;
				tv.m_v2.z	= -dExpSttR[1];
				tv.m_sInfo	= "(시점우측가각부)";
				tvArrRightS.Add(tv);

				// 직선부와 곡선부를 정확하게 연결해줌.
				xySegStt	= tv.m_v1;
			}

			tv.m_v1	= xySegStt;
			tv.m_v2	= xySlabSttEnd;
			tv.m_v1.z	= 0;
			tv.m_v2.z	= 0;
			tv.m_sInfo	= "(시점우측가각부)";
			tvArrRightS.Add(tv);


			// 끝단 자르기인 경우
			if(m_bExpCutStt[1] && bCutEnd)
			{
				CDPoint xy		= GetLineBase()->GetXyLine(m_dStationBridgeStt);
				CDPoint vAng	= GetAngleAzimuthJijum(0);
				CDPoint xyMatch(0, 0);
				CDPointArray xyArrMatch;
				rb.GetXyMatchTvArrAndLine(tvArrRightS, xy, vAng, xyArrMatch);
				if(xyArrMatch.GetSize() > 0)
				{
					xyMatch	= xyArrMatch.GetAt(0);

					CTwinVector tvStt;
					tvStt.m_v1	= tvArrStt.GetAt(0).m_v1;
					tvStt.m_v2	= tvArrStt.GetAt(tvArrStt.GetSize()-1).m_v2;

					//todo 33302 라멘교일 경우에만 선형에 맞게 커팅선을 만듦
					CDPoint xyDirCut;

					if(m_nTypeBridge < 4)
						 xyDirCut = pLineR->GetAngleAzimuth(m_dStationBridgeStt);
					else
						xyDirCut	=  GetAngleCutGagakEnd(TRUE, FALSE);
					
					CDPoint xyMatch2(xyMatch);
					GetXyMatchLineAndLine(tvStt.m_v1, tvStt.GetXyDir(), xyMatch, xyDirCut, xyMatch2);


					CTwinVector tvNew;
					tvNew.m_v1	= xyMatch;
					tvNew.m_v2	= xyMatch2;
					tvNew.m_v1	+= tvNew.GetXyDir(TRUE) * 1000;
					tvNew.m_v2	+= tvNew.GetXyDir(FALSE) * 1000;
					tvNew.m_sInfo.Format("%s컷트", tv.m_sInfo);
					CTwinVectorArray tvArrNew;
					tvArrNew.Add(tvNew);

					CTwinVectorArray tvArrTmp;
					tvArrTmp	= tvArrRightS;
					rb.TrimTvArrayByLine(tvArrRightS, tvNew.m_v1, tvNew.m_v2, TRUE, TRUE, FALSE);
					tvNew.m_v1	+= tvNew.GetXyDir(TRUE) * -1000;
					tvNew.m_v2	+= tvNew.GetXyDir(FALSE) * -1000;
					tvArrRightS.Add(tvNew);
				}
			}
		}

		m_dWidthSlabLeft	+= dCover;
		m_dWidthSlabRight	-= dCover;
		tvArrRight.AddFromLineInfo(pLineR, staStt, staEnd, m_dWidthSlabRight);
		m_dWidthSlabLeft	= dOldWidthSlabLeft;
		m_dWidthSlabRight	= dOldWidthSlabRight;
		
		if(bEndGagak)
		{
			CTwinVector tv;

			tv.m_v1	= xySegEnd2;
			tv.m_v2	= xySegEnd;
			tv.m_v1.z	= 0;
			tv.m_v2.z	= 0;
			tv.m_sInfo	= "(종점우측가각부)";
			tvArrRightE.Add(tv);
			
			if(dExpEndR[1]>0)
			{
				CDPoint xyArcStt = pLineR->GetXyLineDis(staEnd,m_dWidthSlabRight);
				double dAngStt = (xyArcStt-xyCenEnd).Unit().GetAngleDegree();
				double dAngEnd = (xySegEnd-xyCenEnd).Unit().GetAngleDegree();
				tv.m_v1	= xyCenEnd + ToDPointFrDegree(dAngEnd) * dExpEndR[1];
				tv.m_v2	= xyCenEnd + ToDPointFrDegree(dAngStt) * dExpEndR[1];
				tv.m_v1.z	= -1;
				tv.m_v2.z	= dExpEndR[1];
				tv.m_sInfo	= "(종점우측가각부)";
				tvArrRightE.Add(tv);

				// 직선부와 곡선부를 정확하게 연결해줌.
				CTwinVector tmp	= tvArrRightE.GetAt(0);
				tmp.m_v2.x	= tv.m_v1.x;
				tmp.m_v2.y	= tv.m_v1.y;
				tvArrRightE.SetAt(0, tmp);
			}


			// 끝단 자르기인 경우
			if(m_bExpCutEnd[1] && bCutEnd)
			{
				CDPoint xy		= GetLineBase()->GetXyLine(m_dStationBridgeStt+m_dLengthBridge);
				CDPoint vAng	= GetAngleAzimuthJijum(m_nQtyJigan);
				CDPoint xyMatch(0, 0);
				CDPointArray xyArrMatch;
				rb.GetXyMatchTvArrAndLine(tvArrRightE, xy, vAng, xyArrMatch);
				if(xyArrMatch.GetSize() > 0)
				{
					xyMatch	= xyArrMatch.GetAt(0);
					CTwinVector tvEnd;
					tvEnd.m_v1	= tvArrEnd.GetAt(0).m_v1;
					tvEnd.m_v2	= tvArrEnd.GetAt(tvArrEnd.GetSize()-1).m_v2;

					//todo 33302 라멘교일 경우에만 선형에 맞게 커팅선을 만듦
					CDPoint xyDirCut;

					if(m_nTypeBridge < 4)
						xyDirCut = pLineR->GetAngleAzimuth(m_dStationBridgeStt+m_dLengthBridge);
					else
						xyDirCut	=  GetAngleCutGagakEnd(TRUE, FALSE);

					CDPoint xyMatch2(xyMatch);
					GetXyMatchLineAndLine(tvEnd.m_v1, tvEnd.GetXyDir(), xyMatch, xyDirCut, xyMatch2);

					CTwinVector tvNew;
					tvNew.m_v1	= xyMatch2;
					tvNew.m_v2	= xyMatch;
					tvNew.m_v1	+= tvNew.GetXyDir(TRUE) * 1000;
					tvNew.m_v2	+= tvNew.GetXyDir(FALSE) * 1000;
					tvNew.m_sInfo.Format("%s컷트", tv.m_sInfo);
					CTwinVectorArray tvArrNew;
					tvArrNew.Add(tvNew);

					CTwinVectorArray tvArrTmp;
					tvArrTmp	= tvArrRightE;
					rb.TrimTvArrayByLine(tvArrRightE, tvNew.m_v1, tvNew.m_v2, TRUE, TRUE, FALSE);
					tvNew.m_v1	+= tvNew.GetXyDir(TRUE) * -1000;
					tvNew.m_v2	+= tvNew.GetXyDir(FALSE) * -1000;
					tvArrRightE.InsertAt(0, tvNew);
				}
			}
		}
	}
	else
	{
		m_dWidthSlabLeft	+= dCover;
		m_dWidthSlabRight	-= dCover;
		CTwinVector tvStt	= tvArrStt.GetAt(0);
		CTwinVector tvEnd	= tvArrEnd.GetAt(0);
		tvStt.m_v1	= tvArrStt[0].m_v1;
		tvStt.m_v2	= tvArrStt[tvArrStt.GetSize()-1].m_v2;
		tvEnd.m_v1	= tvArrEnd[0].m_v1;
		tvEnd.m_v2	= tvArrEnd[tvArrEnd.GetSize()-1].m_v2;

		pLineR->GetStationMatchVerticalByXy(tvStt.m_v2, staSttRight);
		pLineR->GetStationMatchVerticalByXy(tvEnd.m_v2, staEndRight);
		tvArrRight.AddFromLineInfo(pLineR, staSttRight, staEndRight, m_dWidthSlabRight);

		CDPoint xy	= pLineR->GetXyLineDis(staSttRight, m_dWidthSlabRight);
		// 선형의 곡선 방향에 따라서 tv의 시종점이 뒤집어진 경우가 있다. 체크해서 돌려준다.
		if(tvArrRight.GetSize() > 0 && ~(xy - tvArrRight.GetAt(0).m_v1) > ~(xy - tvArrRight.GetAt(tvArrRight.GetSize()-1).m_v2))
		{
			rb.ReverseRebar(tvArrRight);
		}

		// 시종점과 우측을 정확하게 연결함 //
		CTwinVector tv	= tvArrStt[tvArrStt.GetSize()-1];
		//tv.m_v2.x	= tvArrRight[0].m_v1.x;
		//tv.m_v2.y	= tvArrRight[0].m_v1.y;
		tv.m_v2.x	= xy.x;
		tv.m_v2.y	= xy.y;
		tvArrStt.SetAt(tvArrStt.GetSize()-1, tv);

		tv	= tvArrEnd[tvArrEnd.GetSize()-1];
		xy	= pLineR->GetXyLineDis(staEndRight, m_dWidthSlabRight);
		tv.m_v2.x	= xy.x;
		tv.m_v2.y	= xy.y;
		//tv.m_v2.x	= tvArrRight[tvArrRight.GetSize()-1].m_v2.x;
		//tv.m_v2.y	= tvArrRight[tvArrRight.GetSize()-1].m_v2.y;
		tvArrEnd.SetAt(tvArrEnd.GetSize()-1, tv);
		////////////////////////////////////
		
		m_dWidthSlabLeft	= dOldWidthSlabLeft;
		m_dWidthSlabRight	= dOldWidthSlabRight;
	}

	// 좌측
	CTwinVectorArray tvArrLeft;
	CTwinVectorArray tvArrLeftS, tvArrLeftE;
	double staSttLeft = GetStationByTarget(BRIDGE_OFF_STT_SLAB, -1, bExp);
	double staEndLeft = GetStationByTarget(BRIDGE_OFF_END_SLAB, -1, bExp);
	if(IsOutWall(TRUE)) staSttLeft -= dWS;
	if(IsOutWall(FALSE)) staEndLeft += dWE;
	if(bExp && m_bIsExp)	// 평면 그리는 코드 그대로 따다가 수정함.
	{
		CDPoint slabA[4],EndA[4];
		GetXySss(BRIDGE_OFF_STT_SLAB,slabA);
		GetXySss(BRIDGE_OFF_END_SLAB,EndA);

		// 슬래브 좌측라인
		BOOL bSttGagak = (m_dExpSttW[0]>0 && m_dExpSttH[0] && m_bIsExp) ? TRUE : FALSE;
		BOOL bEndGagak = (m_dExpEndW[0]>0 && m_dExpEndH[0] && m_bIsExp) ? TRUE : FALSE;
		double staStt = GetStationByTarget(BRIDGE_OFF_STT_SLAB,0,FALSE);// + (bSttGagak ? m_dExpSttW[0] : 0);
		double staEnd = GetStationByTarget(BRIDGE_OFF_END_SLAB,0,FALSE);// - (bEndGagak ? m_dExpEndW[0] : 0);

		CDPoint vAngStt	= GetAngleByTarget(BRIDGE_OFF_STT_SLAB, -1);
		CDPoint vAngEnd	= GetAngleByTarget(BRIDGE_OFF_END_SLAB, -1);
		CDPoint xyStt	= GetXySlabEdgeAct(staStt, vAngStt, TRUE);
		CDPoint xyEnd	= GetXySlabEdgeAct(staEnd, vAngEnd, TRUE);
		pLineL->GetStationMatchVerticalByXy(xyStt, staStt);
		pLineL->GetStationMatchVerticalByXy(xyEnd, staEnd);
		staStt	= pLineL->GetStationByDisLength(staStt, bSttGagak ? m_dExpSttW[0] : 0, m_dWidthSlabLeft);
		staEnd	= pLineL->GetStationByDisLength(staEnd, bEndGagak ? -m_dExpEndW[0] : 0, m_dWidthSlabLeft);
		//staStt += bSttGagak ? m_dExpSttW[0] : 0;
		//staEnd -= bEndGagak ? m_dExpEndW[0] : 0;

		CDPoint xyW = pLineL->GetXyLineDis(staStt,m_dWidthSlabLeft);
		CDPoint xySlabSttEnd = slabA[0];
		CDPoint xySegEnd1 = pLineL->GetXyLineDis(staEnd,m_dWidthSlabLeft);
		CDPoint xySegEnd2 = EndA[0];
		CDPoint xyCenStt(0,0),xySegStt(0,0),xyCenEnd(0,0),xySegEnd(0,0);
		if(bSttGagak)
			pLineL->GetXyForRc(m_dWidthSlabLeft,dExpSttR[0],xyW,xySlabSttEnd,TRUE ,TRUE, staStt,xyCenStt,xySegStt);
		if(bEndGagak)
			pLineL->GetXyForRc(m_dWidthSlabLeft,dExpEndR[0],xySegEnd1,xySegEnd2,FALSE,TRUE, staEnd,xyCenEnd,xySegEnd);

		if(bSttGagak)
		{
			CTwinVector tv;

			tv.m_v1	= xySlabSttEnd;
			tv.m_v2	= xySegStt;
			tv.m_v1.z	= 0;
			tv.m_v2.z	= 0;
			tv.m_sInfo	= "(시점좌측가각부)";
			tvArrLeftS.Add(tv);

			if(dExpSttR[0]>0)
			{
				CDPoint xyArcEnd = pLineL->GetXyLineDis(staStt,m_dWidthSlabLeft);
				double dAngStt = (xySegStt-xyCenStt).Unit().GetAngleDegree();
				double dAngEnd = (xyArcEnd-xyCenStt).Unit().GetAngleDegree();
				if(dAngEnd<dAngStt)
					dAngEnd += 360;
				tv.m_v1	= xyCenStt + ToDPointFrDegree(dAngStt) * dExpSttR[0];
				tv.m_v2	= xyCenStt + ToDPointFrDegree(dAngEnd) * dExpSttR[0];
				tv.m_v1.z	= -1;
				tv.m_v2.z	= dExpSttR[0];
				tv.m_sInfo	= "(시점좌측가각부)";
				tvArrLeftS.Add(tv);

				// 직선부를 정확하게 곡선부와 연결해줌.
				CTwinVector tmp	= tvArrLeftS.GetAt(0);
				tmp.m_v2.x	= tv.m_v1.x;
				tmp.m_v2.y	= tv.m_v1.y;
				tvArrLeftS.SetAt(0, tmp);
			}

				// 끝단 자르기인 경우
			if(m_bExpCutStt[0] && bCutEnd)
			{
				CDPoint xy		= GetLineBase()->GetXyLine(m_dStationBridgeStt);
				CDPoint vAng	= GetAngleAzimuthJijum(0);
				CDPoint xyMatch(0, 0);
				CDPointArray xyArrMatch;
				rb.GetXyMatchTvArrAndLine(tvArrLeftS, xy, vAng, xyArrMatch);
				if(xyArrMatch.GetSize() > 0)
				{
					xyMatch	= xyArrMatch.GetAt(0);
					CTwinVector tvStt;
					tvStt.m_v1	= tvArrStt.GetAt(0).m_v1;
					tvStt.m_v2	= tvArrStt.GetAt(tvArrEnd.GetSize()-1).m_v2;

					CDPoint xyDirCut(1,0);
					if(m_nTypeBridge < 4)
						xyDirCut = pLineL->GetAngleAzimuth(m_dStationBridgeStt);
					else
						xyDirCut	=  GetAngleCutGagakEnd(TRUE, TRUE);

					CDPoint xyMatch2(xyMatch);
					GetXyMatchLineAndLine(tvStt.m_v1, tvStt.GetXyDir(), xyMatch, xyDirCut, xyMatch2);

					CTwinVector tvNew;
					tvNew.m_v1	= xyMatch2;
					tvNew.m_v2	= xyMatch;
					tvNew.m_v1	+= tvNew.GetXyDir(TRUE) * 1000;
					tvNew.m_v2	+= tvNew.GetXyDir(FALSE) * 1000;
					tvNew.m_sInfo.Format("%s컷트", tv.m_sInfo);
					CTwinVectorArray tvArrNew;
					tvArrNew.Add(tvNew);

					CTwinVectorArray tvArrTmp;
					tvArrTmp	= tvArrRightS;
					rb.TrimTvArrayByLine(tvArrLeftS, tvNew.m_v1, tvNew.m_v2, TRUE, TRUE, FALSE);
					tvNew.m_v1	+= tvNew.GetXyDir(TRUE) * -1000;
					tvNew.m_v2	+= tvNew.GetXyDir(FALSE) * -1000;
					tvArrLeftS.InsertAt(0, tvNew);
				}
			}
		}

		//
		m_dWidthSlabLeft	+= dCover;
		m_dWidthSlabRight	-= dCover;
		tvArrLeft.AddFromLineInfo(pLineL, staStt, staEnd, m_dWidthSlabLeft);
		m_dWidthSlabLeft	= dOldWidthSlabLeft;
		m_dWidthSlabRight	= dOldWidthSlabRight;
		
		if(bEndGagak)
		{
			CTwinVector tv;

			if(dExpEndR[0]>0)
			{
				CDPoint xyArcStt = pLineL->GetXyLineDis(staEnd,m_dWidthSlabLeft);
				double dAngStt = (xyArcStt-xyCenEnd).Unit().GetAngleDegree();
				double dAngEnd = (xySegEnd-xyCenEnd).Unit().GetAngleDegree();
				if(dAngEnd<dAngStt)
					dAngEnd += 360;
				tv.m_v1	= xyCenEnd + ToDPointFrDegree(dAngStt) * dExpEndR[0];
				tv.m_v2	= xyCenEnd + ToDPointFrDegree(dAngEnd) * dExpEndR[0];
				tv.m_v1.z	= -1;
				tv.m_v2.z	= dExpEndR[0];
				tv.m_sInfo	= "(종점좌측가각부)";
				tvArrLeftE.Add(tv);

				// 직선부와 곡선부를 정확하게 연결해줌
				xySegEnd	= tv.m_v2;
			}

			tv.m_v1	= xySegEnd;
			tv.m_v2	= xySegEnd2;
			tv.m_v1.z	= 0;
			tv.m_v2.z	= 0;
			tv.m_sInfo	= "(종점좌측가각부)";
			tvArrLeftE.Add(tv);


			// 끝단 자르기인 경우
			if(m_bExpCutEnd[0] && bCutEnd)
			{
				CDPoint xy		= GetLineBase()->GetXyLine(m_dStationBridgeStt+m_dLengthBridge);
				CDPoint vAng	= GetAngleAzimuthJijum(m_nQtyJigan);
				CDPoint xyMatch(0, 0);
				CDPointArray xyArrMatch;
				rb.GetXyMatchTvArrAndLine(tvArrLeftE, xy, vAng, xyArrMatch);
				if(xyArrMatch.GetSize() > 0)
				{
					xyMatch	= xyArrMatch.GetAt(0);
					CTwinVector tvEnd;
					tvEnd.m_v1	= tvArrEnd.GetAt(0).m_v1;
					tvEnd.m_v2	= tvArrEnd.GetAt(tvArrEnd.GetSize()-1).m_v2;
					CDPoint xyDirCut(1,0);
					if(m_nTypeBridge < 4)
						xyDirCut = pLineL->GetAngleAzimuth(m_dStationBridgeStt+m_dLengthBridge);
					else
						xyDirCut	=  GetAngleCutGagakEnd(FALSE, TRUE);

					CDPoint xyMatch2(xyMatch);
					GetXyMatchLineAndLine(tvEnd.m_v1, tvEnd.GetXyDir(), xyMatch, xyDirCut, xyMatch2);

					CTwinVector tvNew;
					tvNew.m_v1	= xyMatch2;
					tvNew.m_v2	= xyMatch;
					tvNew.m_v1	+= tvNew.GetXyDir(TRUE) * 1000;
					tvNew.m_v2	+= tvNew.GetXyDir(FALSE) * 1000;
					tvNew.m_sInfo.Format("%s컷트", tv.m_sInfo);
					CTwinVectorArray tvArrNew;
					tvArrNew.Add(tvNew);

					CTwinVectorArray tvArrTmp;
					tvArrTmp	= tvArrRightE;
					rb.TrimTvArrayByLine(tvArrLeftE, tvNew.m_v1, tvNew.m_v2, FALSE, TRUE, FALSE);
					tvNew.m_v1	+= tvNew.GetXyDir(TRUE) * -1000;
					tvNew.m_v2	+= tvNew.GetXyDir(FALSE) * -1000;
					tvArrLeftE.InsertAt(0, tvNew);
				}
			}
		}
	}
	else
	{
		m_dWidthSlabLeft	+= dCover;
		m_dWidthSlabRight	-= dCover;
		CTwinVector tvStt	= tvArrStt.GetAt(0);
		CTwinVector tvEnd	= tvArrEnd.GetAt(0);
		tvStt.m_v1	= tvArrStt[0].m_v1;
		tvStt.m_v2	= tvArrStt[tvArrStt.GetSize()-1].m_v2;
		tvEnd.m_v1	= tvArrEnd[0].m_v1;
		tvEnd.m_v2	= tvArrEnd[tvArrEnd.GetSize()-1].m_v2;

		pLineL->GetStationMatchVerticalByXy(tvStt.m_v1, staSttLeft);
		pLineL->GetStationMatchVerticalByXy(tvEnd.m_v1, staEndLeft);

		tvArrLeft.AddFromLineInfo(pLineL, staSttLeft, staEndLeft, m_dWidthSlabLeft);

		// 선형의 곡선 방향에 따라서 tv의 시종점이 뒤집어진 경우가 있다. 체크해서 돌려준다.
		CDPoint xy	= pLineL->GetXyLineDis(staSttLeft, m_dWidthSlabLeft);
		if(tvArrLeft.GetSize() > 0 && ~(xy - tvArrLeft.GetAt(0).m_v1) > ~(xy - tvArrLeft.GetAt(tvArrLeft.GetSize()-1).m_v2))
		{
			rb.ReverseRebar(tvArrLeft);
		}

		// 시종점과 좌측을 정확하게 연결함 //
		CTwinVector tv	= tvArrStt[0];
		tv.m_v1.x	= xy.x;
		tv.m_v1.y	= xy.y;
		//tv.m_v1.x	= tvArrLeft[0].m_v1.x;
		//tv.m_v1.y	= tvArrLeft[0].m_v1.y;
		tvArrStt.SetAt(0, tv);

		tv	= tvArrEnd[0];
		xy	= pLineL->GetXyLineDis(staEndLeft, m_dWidthSlabLeft);
		tv.m_v1.x	= xy.x;
		tv.m_v1.y	= xy.y;
		//tv.m_v1.x	= tvArrLeft[tvArrLeft.GetSize()-1].m_v2.x;
		//tv.m_v1.y	= tvArrLeft[tvArrLeft.GetSize()-1].m_v2.y;
		tvArrEnd.SetAt(0, tv);
		////////////////////////////////////

		m_dWidthSlabLeft	= dOldWidthSlabLeft;
		m_dWidthSlabRight	= dOldWidthSlabRight;
	}

	// 가각 끝단 자르기 옵션 적용
	if(bExp && bCutEnd)
	{
		long stt = 0; for(stt = 0; stt < 2; stt++)
		{
			BOOL bStt	= stt == iSTT;
			CTwinVectorArray *pTvArr	= bStt ? &tvArrStt : &tvArrEnd;
			long left = 0; for(left = 0; left < 2; left++)
			{
				BOOL bLeft	= left == 0;
				BOOL bCut	= bStt ? m_bExpCutStt[left] : m_bExpCutEnd[left];
				if(!bCut) continue;

				CTwinVectorArray tvArr;
				tvArr	= bStt ? (bLeft ? tvArrLeftS : tvArrRightS) : (bLeft ? tvArrLeftE : tvArrRightE);
				CTwinVector tvNew;
				long i = 0; for(i = 0; i < tvArr.GetSize(); i++)
				{
					tvNew	= tvArr.GetAt(i);
					if(tvNew.m_sInfo.Find("컷트", 0) != -1)
					{	
						CTwinVector tvStt	= pTvArr->GetAt(0);
						GetXyMatchLineAndLine(tvStt.m_v1, tvStt.GetXyDir(), tvNew.m_v1, tvNew.GetXyDir(), bLeft ? tvStt.m_v1 : tvStt.m_v2);
						pTvArr->SetAt(0, tvStt);
						break;
					}
				}
			}
		}
	}

	// 가각 R 타입이 접선방식이 아닌 경우에 대한 적용
	if(!m_bExpSttRoundIsTan[0] && m_dExpSttR[0] > 0)
	{
		CTwinVector tv;
		if(tvArrLeftS.GetTvByInfo("(시점좌측가각부)", tv))
		{
			tv.m_v1.z	= -1;
			tv.m_v2.z	= m_dExpSttR[0];
			tvArrLeftS.SetTvByInfo("(시점좌측가각부)", tv);
		}
	}
	if(!m_bExpSttRoundIsTan[1] && m_dExpSttR[1] > 0)
	{
		CTwinVector tv;
		if(tvArrRightS.GetTvByInfo("(시점우측가각부)", tv))
		{
			tv.m_v1.z	= -1;
			tv.m_v2.z	= m_dExpSttR[1];
			tvArrRightS.SetTvByInfo("(시점우측가각부)", tv);
		}
	}
	if(!m_bExpEndRoundIsTan[0] && m_dExpEndR[0] > 0)
	{
		CTwinVector tv;
		if(tvArrLeftE.GetTvByInfo("(종점좌측가각부)", tv))
		{
			tv.m_v1.z	= -1;
			tv.m_v2.z	= m_dExpEndR[0];
			tvArrLeftE.SetTvByInfo("(종점좌측가각부)", tv);
		}
	}
	if(!m_bExpEndRoundIsTan[1] && m_dExpEndR[1] > 0)
	{
		CTwinVector tv;
		if(tvArrRightE.GetTvByInfo("(종점우측가각부)", tv))
		{
			tv.m_v1.z	= -1;
			tv.m_v2.z	= m_dExpEndR[1];
			tvArrRightE.SetTvByInfo("(종점우측가각부)", tv);
		}
	}



	tvArrRight.AddFromTvArray(tvArrRightS);
	tvArrRight.AddFromTvArray(tvArrRightE);
	tvArrLeft.AddFromTvArray(tvArrLeftS);
	tvArrLeft.AddFromTvArray(tvArrLeftE);


	rb.RemoveMinLengthLine(tvArrRight, 0.0);
	rb.RemoveMinLengthLine(tvArrLeft, 0.0);

	// 합침 (순서에 맞게)
	rb.ReverseRebar(tvArrStt);


	// 적절한 이름/피복 부여
	CTwinVector tv;
	long i = 0; for(i = 0; i < tvArrStt.GetSize(); i++)
	{
		tv	= tvArrStt.GetAt(i);
		if(nDan != -1)
			tv.m_sInfoHor.Format("cover:%f", nDan == 9 ? m_dCoverWallOut[0]-0.01 : m_dCoverWallOut[nDan]);
		tvArrStt.SetAt(i, tv);
	}
	for(i = 0; i < tvArrEnd.GetSize(); i++)
	{
		tv	= tvArrEnd.GetAt(i);
		if(nDan != -1)
			tv.m_sInfoHor.Format("cover:%f", nDan == 9 ? m_dCoverWallOut[0]-0.01 : m_dCoverWallOut[nDan]);
		tvArrEnd.SetAt(i, tv);
	}

	for(i = 0; i < tvArrLeft.GetSize(); i++)
	{
		tv	= tvArrLeft.GetAt(i);
		tv.m_sInfo	= "좌측" + tv.m_sInfo;
		if(nDan != -1)
			tv.m_sInfoHor.Format("cover:%f", nDan == 9 ? GetCoverSide_UpperSlab(0)-0.01 : GetCoverSide_UpperSlab(nDan));
		tvArrLeft.SetAt(i, tv);
	}
	for(i = 0; i < tvArrRight.GetSize(); i++)
	{
		tv	= tvArrRight.GetAt(i);
		tv.m_sInfo	= "우측" + tv.m_sInfo;
		if(nDan != -1)
			tv.m_sInfoHor.Format("cover:%f", nDan == 9 ? GetCoverSide_UpperSlab(0)-0.01 : GetCoverSide_UpperSlab(nDan));
		tvArrRight.SetAt(i, tv);
	}

	// 중심선형을 기준으로 잘라낸다.
	// 속도개선을 위해서 trim함수 사용하지 않음
	if(nLeft != 2)
	{
		CTwinVectorArray tvArrCenter;
		GetTvCenterPlane_UpperSlab(tvArrCenter, TRUE, FALSE, 0, bVert);
		CTwinVector tv	= tvArrCenter.GetAt(0);
		tv.m_sInfoHor.Format("cover:%f", 0.0);
		tvArrCenter.SetAt(0, tv);
		
		BOOL bLeft	= nLeft == iLEFT;
		tv	= tvArrStt.GetAt(0);
		if(!bLeft)
			tv.m_v2	= tvArrCenter.GetAt(0).m_v1;
		else
			tv.m_v1	= tvArrCenter.GetAt(0).m_v1;

		tvArrStt.RemoveAll();
		tvArrStt.Add(tv);

		tv	= tvArrEnd.GetAt(0);
		if(!bLeft)
			tv.m_v1	= tvArrCenter.GetAt(tvArrCenter.GetSize()-1).m_v2;
		else
			tv.m_v2	= tvArrCenter.GetAt(tvArrCenter.GetSize()-1).m_v2;

		tvArrEnd.RemoveAll();
		tvArrEnd.Add(tv);


		if(bLeft)
		{
			tvArrRight.RemoveAll();
			tvArrRight.AddFromTvArray(tvArrCenter);
		}
		else
		{
			tvArrLeft.RemoveAll();
			tvArrLeft.AddFromTvArray(tvArrCenter);
			rb.ReverseRebar(tvArrLeft);
		}
	}


	tvArr.AddFromTvArray(tvArrStt);
	tvArr.AddFromTvArray(tvArrRight);
	tvArr.AddFromTvArray(tvArrEnd);
	tvArr.AddFromTvArray(tvArrLeft);

	if(!m_bSettingPlane)
	{
		m_tvArrLeftRightPlane[iRIGHT][nExp]	= tvArrRight;
		m_tvArrLeftRightPlane[iLEFT][nExp]	= tvArrLeft;
	}

	// 가각에서는 어쩔수 없이 피복을 하나만 쓰더라도 시종점과 Side의 피복을 없애면 안되지
	// (ARCBRIDGE-2631) 가각에서는 왜 피복을 하나밖에 쓸수가 없었는지 확인 할수가 없음.
	// 테스트 했을때 가각을 넣어도 Offset에는 문제가 없다. 입력에 따라 다른 경우에는 나올수도 있고 Offset 함수가 수정되면서 오류가 없어졌을수도 있다. 
	// 일단은 피복을 사용할수 있도록 열어준다.
	//if(FALSE)
	// (ARCBRIDGE-2796) 이슈에서 슬래브 끝단과 측면 피복을 달리 했을때 offset이 안되는 경우가 발생했다. 꺽이는 위치에서 부재가 짧은 경우 offset을 못하는듯하다.
	// rb.GetOffsetTvArray()함수에서 짧은 부재 빼는 부분을 수정했다.
	if(nDan > -1 && (bExp && m_bIsExp) == TRUE)
	{
		dCover	= nDan == 9 ? GetCoverSide_UpperSlab(0)-0.01 : GetCoverSide_UpperSlab(nDan);
		Sort(tvArr, 0.5);

		for(long ix = 0; ix < tvArr.GetSize(); ix++)
		{
			tvArr.GetPAt(ix)->m_sInfoHor	= _T("");
		}

		////#0029125 - 옵셋을 하면서 문제를 발생
		if(bExp == TRUE && bCutEnd == FALSE)
		{
			GetOffsetTvRebarOut_Plane_UpperSlab(tvArr, dCover, dExpSttR[0], dExpSttR[1], dExpEndR[0], dExpEndR[1]);
		}
		else
		{
			rb.GetOffsetTvArray(tvArr, dCover);
		}
	}
	else
	{
		rb.GetOffsetTvArray(tvArr, 0);
	}

	// offset을 위해서 변경했던 값 복원.
	m_dWidthSlabLeft	= dOldWidthSlabLeft;
	m_dWidthSlabRight	= dOldWidthSlabRight;
}

// 시점리턴 
double CRcBridgeApp::GetStationBridgeStt(BOOL bLineBaseVert)
{
	double dSta	= m_dStationBridgeStt;
	if(bLineBaseVert)
	{
		MakeLineBaseVert();
		dSta	= 0;
	}
	
	return dSta;
}

double CRcBridgeApp::GetLengthBridge(BOOL bLineBaseVert)
{
	double dLen	= m_dLengthBridge;
	
	if(bLineBaseVert)
	{
		MakeLineBaseVert();
		CTwinVectorArray tvArrCenter;
		CTwinVector tvCenter;
		GetTvCenterPlane_UpperSlab(tvArrCenter, FALSE, FALSE, 0, TRUE);
		tvCenter	= tvArrCenter.GetAt(0);
		dLen	= tvCenter.GetLength();
	}
	
	return dLen;
}

// 중심선 리턴
// 주철근 배치가 0번타입이면 선형을 리턴하면되지만.
// 주철근배치가 1이나 2번타입이면 선형을 리턴하지 않고, 시점에 직각되는 방향으로 뻗은 선을 리턴해야 된다.
void CRcBridgeApp::GetTvCenterPlane_UpperSlab(CTwinVectorArray &tvArr, BOOL bIncludeOutWall, BOOL bIncludeBracket, double dLenExt, BOOL bVert)
{
	tvArr.RemoveAll();
	CLineInfoApp *pLine	= static_cast<CLineInfoApp*>(GetLineBase());
	if(!pLine) return;
	double dMidDist		= GetDistMidOfSlab(m_dStationBridgeStt, TRUE);
	double dMidDistAng		= GetDistMidOfSlab(m_dStationBridgeStt, FALSE);

	// 아름다운 도면을 위해 브라켓 너비를 고려 하지 않을 수 없다.
	double dLenExtStt	= dLenExt;// + (bOnlySlab ? m_dWS : m_dWS + m_dBWS);
	double dLenExtEnd	= dLenExt;// + (bOnlySlab ? m_dWE : m_dWE + m_dBWE);
	if(bIncludeOutWall)
	{
		dLenExtStt	+= m_dWS / GetAngleJijum(0).y;
		dLenExtEnd	+= m_dWE / GetAngleJijum(m_nQtyJigan).y;
	}

	if(bIncludeBracket)
	{
		dLenExtStt	+= m_dBWS / GetAngleJijum(0).y;
		dLenExtEnd	+= m_dBWE / GetAngleJijum(m_nQtyJigan).y;
	}


	if(!bVert)
	{
		double dStaStt	= m_dStationBridgeStt-dLenExtStt;
		double dStaEnd	= m_dStationBridgeStt+m_dLengthBridge+dLenExtEnd;
		double dAngStt	= GetAngleJijum(0).GetAngleDegree();
		double dAngEnd	= GetAngleJijum(m_nQtyJigan).GetAngleDegree();

		double sw	= 1;
		if(dAngStt > 90)
		{
			sw	= 1;
			dAngStt = 180 - dAngStt;
		}
		else
			sw	= -1;
		dStaStt	= dStaStt + ((dMidDist)/tan(ToRadian(dAngStt))*sw);
		if(dAngEnd > 90)
		{
			sw	= 1;
			dAngEnd = 180 - dAngEnd;
		}
		else
			sw	= -1;
		dStaEnd	= dStaEnd + ((dMidDist)/tan(ToRadian(dAngEnd))*sw);

		tvArr.AddFromLineInfo(pLine, dStaStt, dStaEnd, dMidDist);

		// (ARCBRIDGE-3482) 선형의 곡선 방향에 따라서 tv의 시종점이 뒤집어진 경우가 있다. 체크해서 돌려준다.
		CDPoint xy	= pLine->GetXyLineDis(dStaStt, dMidDist);
		if(tvArr.GetSize() > 0 && ~(xy - tvArr.GetAt(0).m_v1) > ~(xy - tvArr.GetAt(tvArr.GetSize()-1).m_v2))
		{
			CRebarPlacing rb;
			rb.ReverseRebar(tvArr);
		}
	}
	else
	{
		CRebarPlacing rb;
		CDPoint xyMatchStt(0, 0), xyMatchEnd(0, 0);
		double dStaMid	= m_dStationBridgeStt + m_dLengthBridge/2;
		CDPoint vAng	= GetAngleJijum(0);
		CDPoint xyMid	= pLine->GetXyLineByLengthAndAngle(dStaMid, dMidDistAng, vAng);
		CDPoint xyDir	= GetAngleAzimuthJijum(0).Rotate90();

		// 일단 시종점라인에서 교차점을 찾아 보고
		CTwinVectorArray tvArrStt, tvArrEnd;
		CDPointArray xyArrStt, xyArrEnd;
		GetTvPlaneSttEndLine(tvArrStt, TRUE, FALSE);
		GetTvPlaneSttEndLine(tvArrEnd, FALSE, FALSE);
		
		rb.GetXyMatchTvArrAndLine(tvArrStt, xyMid, xyDir, xyArrStt);
		rb.GetXyMatchTvArrAndLine(tvArrEnd, xyMid, xyDir, xyArrEnd);
		rb.RemoveSamePoint(xyArrStt, TRUE);
		rb.RemoveSamePoint(xyArrEnd, TRUE);

		// 못 찾으면 첫번째 선을 가상으로 뻗어서라도 구함 
		if(xyArrStt.GetSize() == 0 && tvArrStt.GetSize() > 1)
		{
			CDPoint xyMatch(0, 0);
			CTwinVector tvStt	= tvArrStt.GetAt(1);
			if(GetXyMatchLineAndLine(tvStt.m_v1, tvStt.GetXyDir(), xyMid, xyDir, xyMatch))
			{
				xyArrStt.Add(xyMatch);
			}
		}
		if(xyArrEnd.GetSize() == 0 && tvArrEnd.GetSize() > 1)
		{
			CDPoint xyMatch(0, 0);
			CTwinVector tvEnd	= tvArrEnd.GetAt(1);
			if(GetXyMatchLineAndLine(tvEnd.m_v1, tvEnd.GetXyDir(), xyMid, xyDir, xyMatch))
			{
				xyArrEnd.Add(xyMatch);
			}
		}

		if(xyArrStt.GetSize() == 1 && xyArrEnd.GetSize() == 1)
		{
			CTwinVector tv;
			tv.m_v1	= xyArrStt.GetAt(0);
			tv.m_v2	= xyArrEnd.GetAt(0);
			tv.m_v1	+= tv.GetXyDir(TRUE) * dLenExtStt;
			tv.m_v2	+= tv.GetXyDir(FALSE) * dLenExtEnd;
			tvArr.Add(tv);
		}
		else	
		{
			// 정상적으로 못 찾으면 전체 외곽에서 찾는다.
			CTwinVectorArray tvArrPlane;
			GetTvRebarOut_Plane_UpperSlabStd(tvArrPlane, -1);

			CDPointArray xyArrMatch;
			rb.GetXyMatchTvArrAndLine(tvArrPlane, xyMid, xyDir, xyArrMatch);

			// 전체외곽에서 정상적으로 찾은건 교차점이 딱 2개인 경우이다.
			rb.RemoveSamePoint(xyArrMatch, TRUE);
			if(xyArrMatch.GetSize() == 2)
			{
				CTwinVector tv;
				tv.m_v1	= xyArrMatch.GetAt(0);
				tv.m_v2	= xyArrMatch.GetAt(1);

				// 시종점을 가려준다.
				double dSta1	= 0;
				double dSta2	= 0;
				pLine->GetStationMatchByXyAndAngle(tv.m_v1, CDPoint(0, 1), dSta1, dMidDist);
				pLine->GetStationMatchByXyAndAngle(tv.m_v2, CDPoint(0, 1), dSta2, dMidDist);
				if(dSta1 > dSta2)
					GetSwap(tv.m_v1, tv.m_v2);

				tv.m_v1	+= tv.GetXyDir(TRUE) * dLenExtStt;
				tv.m_v2	+= tv.GetXyDir(FALSE) * dLenExtEnd;
				tvArr.Add(tv);
			}
			else
			{
				// 그래도 없으면 그냥 선형으로 중심선을 한다.
				tvArr.AddFromLineInfo(pLine, m_dStationBridgeStt-dLenExtStt, m_dStationBridgeStt+m_dLengthBridge+dLenExtEnd, dMidDist);

				// (ARCBRIDGE-3482) 선형의 곡선 방향에 따라서 tv의 시종점이 뒤집어진 경우가 있다. 체크해서 돌려준다.
				CDPoint xy	= pLine->GetXyLineDis(m_dStationBridgeStt-dLenExtStt, dMidDist);
				if(tvArr.GetSize() > 0 && ~(xy - tvArr.GetAt(0).m_v1) > ~(xy - tvArr.GetAt(tvArr.GetSize()-1).m_v2))
				{
					CRebarPlacing rb;
					rb.ReverseRebar(tvArr);
				}
			}
		}
	}
}

// 선형에 따른 종단면에 대응하는 
// 직방향위치를 구해줌.
// 해당 위치의 스테이션에서 nJijum의 각도로 뻗어서 교차되는 중심선과의 교차점과 중심선 시작점과의 거리를
// 구해서 x좌표를 조정해서 리턴함.
CDPoint CRcBridgeApp::GetXyVertSectionByDirVert(CDPoint xy, long nJijum)
{
	CLineInfo *pLine	= GetLineBase();
	if(!pLine) return CDPoint(0, 0);

	CTwinVectorArray tvArrCenter;
	CTwinVector tvCenter;
	GetTvCenterPlane_UpperSlab(tvArrCenter, FALSE, FALSE, 0, TRUE);
	tvCenter	= tvArrCenter.GetAt(0);

	// 평면상에서의 좌표
	CDPoint vAngJijum	= GetAngleAzimuthJijum(nJijum);
	CDPoint xyPlane	= pLine->GetXyLineByLengthAndAngle(xy.x, 0, CDPoint(0, 1));
	CDPoint xyMatch	= xyPlane;
	GetXyMatchLineAndLine(xyPlane, vAngJijum, tvCenter.m_v1, tvCenter.GetXyDir(), xyMatch);

	// 다시 종단면 상의 좌표로 구함
	double dDist	= ~(xyMatch-tvCenter.m_v1);
	CDPoint xyVert	= CDPoint(m_dStationBridgeStt + dDist, xy.y);

	return xyVert;
}

// 스테이션 별 총 차도 폭을 구한다.
//지중라멘교일 경우에는 슬래브면이 아닌 도로면
double CRcBridgeApp::GetWidthTotalRoadWay(double sta)
{
	long nQtyHDan = GetQtyHDan(m_bUnderGroundRahmen);	//지중라멘교일 경우에는 슬래브면이 아닌 도로면

	double dWidth = 0;
	long nHDan=0; for(nHDan=0; nHDan<nQtyHDan; nHDan++)
	{
		CGuardWallRC* pGW = GetGuardWallByHDan(nHDan,m_bUnderGroundRahmen);
		if(!pGW) continue;
		if(pGW->IsTypeHDanOnlyNone())
			dWidth += GetLengthHDanAct(sta, CDPoint(0, 1), nHDan,m_bUnderGroundRahmen);
	}

	return dWidth;
}

double CRcBridgeApp::GetWidthTotalRoadWay()
{
	double sta = GetStationBridgeMid();

	return GetWidthTotalRoadWay(sta);
}

// 내측벽체의 헌치 
// 내측벽체 헌치는 사거리적용된 종단면을 사용해야 된다.
void CRcBridgeApp::GetTvPlaneInWallHunch(long nIdxInWall, CTwinVectorArray &tvArrLeft, CTwinVectorArray &tvArrRight, BOOL bUpperSlab)
{
	tvArrLeft.RemoveAll();
	tvArrRight.RemoveAll();
	CWallApp *pWall	= GetInWall(nIdxInWall);
	if(!pWall) return;
	CTwinVectorArray tvArrVertSection;
	tvArrVertSection	= m_tvArrVertSection;
	GetTvVertSection(0, TRUE);

	CLineInfo *pLine	= GetLineBase();
	long nJ	= nIdxInWall+1;
	CDPoint vAng	= GetAngleJijum(nJ);
	double dSta		= GetStationOnJijum(nJ);
	double dWidthL	= 0;
	double dWidthR	= 0;
	long left = 0; for(left = 0; left < 2; left++)
	{
		BOOL bLeft	= left == iLEFT;

		// 우측인데 벽체가 V형이 아니면 우측헌치는 없음
		if(!bLeft && pWall->m_nType != WALL_TYPE_V) continue;

		long nIdxHunch	= GetIdxHunchInWall(nIdxInWall, bLeft);
		CString sHunchL	= _T(""), sHunchR = _T("");
		if(bUpperSlab)
		{
			sHunchL.Format("%d번째내측헌치좌측면", nIdxHunch+1);
			sHunchR.Format("%d번째내측헌치우측면", nIdxHunch+1);
		}
		else
		{
			sHunchL.Format("%d번째내측하부헌치좌측면", nIdxHunch+1);
			sHunchR.Format("%d번째내측하부헌치우측면", nIdxHunch+1);
		}
		
		CTwinVector tvLeft	= m_tvArrVertSection.GetTvByInfo(sHunchL);
		CTwinVector tvRight	= m_tvArrVertSection.GetTvByInfo(sHunchR);
		tvLeft.m_v1.z	= 0;
		tvLeft.m_v2.z	= 0;
		tvRight.m_v1.z	= 0;
		tvRight.m_v2.z	= 0;

		// 평면좌표 만든다.
		double dStaL	= tvLeft.GetXyLeft().x;
		double dStaR	= tvRight.GetXyRight().x;
		CDPoint vAngL	= pLine->GetAngleByOffsetWidth(dSta, dStaL-dSta, vAng);
		CDPoint vAngR	= pLine->GetAngleByOffsetWidth(dSta, dStaR-dSta, vAng);

		// 헌치 좌측선
		dWidthL	= GetWidthSlabAct(dStaL, vAngL, -1, TRUE);
		dWidthR	= GetWidthSlabAct(dStaL, vAngL, 1, TRUE);
		tvLeft.m_v1	= pLine->GetXyLineByLengthAndAngle(dStaL, -dWidthL, vAngL);
		tvLeft.m_v2	= pLine->GetXyLineByLengthAndAngle(dStaL, dWidthR, vAngL);
		tvLeft.m_sInfo	= sHunchL;
		if(bLeft) tvArrLeft.Add(tvLeft);
		else tvArrRight.Add(tvLeft);

		// 헌치 우측선
		dWidthL	= GetWidthSlabAct(dStaR, vAngR, -1, TRUE);
		dWidthR	= GetWidthSlabAct(dStaR, vAngR, 1, TRUE);
		tvRight.m_v1	= pLine->GetXyLineByLengthAndAngle(dStaR, -dWidthL, vAngR);
		tvRight.m_v2	= pLine->GetXyLineByLengthAndAngle(dStaR, dWidthR, vAngR);
		tvRight.m_sInfo	= sHunchR;
		if(bLeft) tvArrLeft.Add(tvRight);
		else tvArrRight.Add(tvRight);

		// 헌치 후면(선형의 좌측)
		CTwinVector tvBack;
		tvBack.m_v1	= tvLeft.m_v2;
		tvBack.m_v2	= tvRight.m_v2;
		tvBack.m_sInfo	= "헌치후면";
		if(bLeft) tvArrLeft.Add(tvBack);
		else tvArrRight.Add(tvBack);

		// 헌치 전면(선형의 우측)
		CTwinVector tvFront;
		tvFront.m_v1	= tvLeft.m_v1;
		tvFront.m_v2	= tvRight.m_v1;
		tvFront.m_sInfo	= "헌치전면";
		if(bLeft) tvArrLeft.Add(tvFront);
		else tvArrRight.Add(tvFront);
	}

	m_tvArrVertSection	= tvArrVertSection;
}

BOOL CRcBridgeApp::IsExsitHyunTa()
{
	BOOL bHyunTa = FALSE;
	int nCntFoot = GetCountFooting();
	CFootingApp *pFooting;
	CExPileApp *pExPile = NULL;

	for( int i=0; i<nCntFoot; i++)
	{
		pFooting = GetFooting(i);
		if(pFooting == NULL) continue;
		if(pFooting->m_bIs) pExPile = &pFooting->m_exFooting.m_Pile;
		if(pExPile->m_bIs && pExPile->m_nType == PILE_TYPE_HYUNJANG) bHyunTa = TRUE;
	}

	return bHyunTa;
}


double CRcBridgeApp::GetHeightModelJijum(long nJijum)
{
	double dHeightJijum = 0;

	// 속도가 너무 느려진다. 종단 타입을 먼저 체크해서 다를때만...
	long nTypeJongOld(-1);
	long nTypeJongdanUseModel = (m_nTypeBridge==BRIDGE_TYPE_ARCH) ? 1 : 0;
	if(m_nTypeJongdanForRebar != nTypeJongdanUseModel)
	{
		nTypeJongOld	= m_nTypeJongdanForRebar;
		m_nTypeJongdanForRebar = nTypeJongdanUseModel;
		SyncBridge(0, FALSE, FALSE, FALSE);
		//	GetTvVertSection_Std(0, TRUE, FALSE, FALSE, (m_nTypeBridge==BRIDGE_TYPE_ARCH)? 1 : 0);
	}

	long nJijumCheck = (IsBoxType() && m_bSlopeLowerSlab) ? 0 : nJijum;
	long nLine = 0;
	double staJijum = GetStationOnJijum(nJijumCheck,nLine);

	if(nJijumCheck == 0)
	{
		CDPoint vAngStt = GetAngleJijum(0);
		staJijum -= m_dWS/vAngStt.y/2;
	}
	else if(nJijumCheck == m_nQtyJigan)
	{
		CDPoint vAngEnd = GetAngleJijum(m_nQtyJigan);
		staJijum += m_dWE/vAngEnd.y/2;
	}

	double EL_Plan  = 0;
	
	if(m_nSelectModelingHeight==0)
	{
		EL_Plan  = GetLineBase()->GetElevationPlanEl(staJijum);
	}
	else 
	{
		EL_Plan  = GetElMaxSlabUpper(staJijum, GetAngleJijum(nJijumCheck)) + GetThickPave(FALSE);
	}

	
	if(IsBoxType() == FALSE)
	{
		double EL_Base  = 0;
		if(nJijumCheck == 0) 
		{
			EL_Base = m_dELSB + m_footingStt.GetHeight(TRUE);
		}
		else if(nJijumCheck == m_nQtyJigan) 
		{
			EL_Base = m_dELEB + m_footingEnd.GetHeight(TRUE);
		}
		else 
		{
			BOOL bArch = (m_nTypeBridge == BRIDGE_TYPE_ARCH);
			if(bArch)
			{
				double dDist = m_pbRibCen.GetDistByXPos(staJijum);
				CDPoint xyPos = m_pbRibCen.GetXyByDist(dDist);
				EL_Base = xyPos.y;
			}
			else
			{
				CWallApp *pWall = GetInWall(nJijumCheck-1);
				EL_Base = pWall->GetELBottomOfWall() + pWall->m_dELBDiff;
			}
		}
		dHeightJijum = EL_Plan - GetThickPave(FALSE) - m_dTS/2 - EL_Base;
	}
	else
	{
		CDPoint vAngStt = GetAngleJijum(0);
		double dElBase = GetLineBase()->GetElevationPlanEl(GetStationOnJijum(0, nLine) - m_dWS/vAngStt.y);
		double dHeightStt = dElBase - m_dELSB - GetThickPave(FALSE) - m_dTS/2. - m_dTS_Lower/2.; // 하면EL 입력위치에서의 높이

		if(m_bSlopeLowerSlab == FALSE)
			dHeightJijum = dHeightStt + EL_Plan - dElBase;
		else
			dHeightJijum = dHeightStt;
	}

	if(nTypeJongOld != -1)
	{
		m_nTypeJongdanForRebar = nTypeJongOld;
		SyncBridge(0, FALSE, FALSE, FALSE);
		// GetTvVertSection_Std(0, TRUE, FALSE, TRUE);
	}

	return dHeightJijum;
}

double CRcBridgeApp::GetLengthModelSpan(long nSpan)
{
	double dLengthSpan = 0;
	CDPoint vAngStt = GetAngleJijum(0);
	CDPoint vAngEnd = GetAngleJijum(m_nQtyJigan);

	if(nSpan==-1)
	{
		long i=0; for(i=0; i<m_nQtyJigan; i++)
		{
			dLengthSpan += GetLengthJigan(i);
		}
		if(IsOutsideWall())
		{
			dLengthSpan += m_dWS/vAngStt.y/2;
			dLengthSpan += m_dWE/vAngEnd.y/2;
		}
	}
	else
	{
		dLengthSpan = GetLengthJigan(nSpan);
		if(nSpan==0 && IsOutsideWall())
		{
			dLengthSpan += m_dWS/vAngStt.y/2;
		}
		if(nSpan==m_nQtyJigan-1 && IsOutsideWall())
		{
			dLengthSpan += m_dWE/vAngEnd.y/2;
		}
		if(nSpan==0 && ( IsPiRahmen() || IsRcSlab() ) )
		{
			CJijum *pJ = GetJijum(nSpan);
			dLengthSpan -= pJ->m_dLengthUgan[1];
		}
		if(nSpan==m_nQtyJigan-1 && ( IsPiRahmen() || IsRcSlab() ) )
		{
			CJijum *pJ = GetJijum(nSpan+1);
			dLengthSpan -= pJ->m_dLengthUgan[0];
		}
	}

	return dLengthSpan;
}

long CRcBridgeApp::GetNumberElementSlabSpanSttCheck(long nSpan, long nTypeDesign, BOOL bShearCheck, BOOL bGirderCalc, BOOL bUpperSlab, double dDistShearPos, double &dDistStt)
{
	CFEMManage *pFEM = GetFemManage(nTypeDesign);
	if(pFEM == NULL) return -1;

	BOOL bLsd = m_pARcBridgeDataStd->IsLsdDesign();
	long nElement = -1;
	long nJijumConnect = GetJijumConnectRibAndSlab();

	// 무한강성이 아닌 경우는 강역에 해당하는 절점이 생성되지 않는다.
	// (ARCBRIDGE-3196) endoffset을 적용했어도 강역길이보다 짧은 부재가 있는경우 제외 시켜 줘야 한다.
	double dDistRigid = (bGirderCalc || m_nSelectRigidModeling == 0) ? 0 :  toM(GetDistRigidEndSlab(nSpan,FALSE,bUpperSlab));
	double dDistChcek = 0;
	// 강역길이가 5mm 이하일 경우 부재가 추가되지 않음으로 여기서도 제외시켜줘야한다.
	if(dDistRigid <= 0.005) dDistRigid = 0;

	if(bLsd)
	{
		dDistRigid = frM(dDistRigid);
	}

	long i=0; for(i=0; i<nSpan; i++)
	{
		dDistChcek += bLsd? GetLengthModelSpan(i) : toM(GetLengthModelSpan(i));
	}
	if(nSpan==0 && (IsRcSlab() || IsPiRahmen()))
	{
		CDPoint vAng = GetAngleJijum(0);
		CJijum *pJ = GetJijum(nSpan);
		dDistChcek = bLsd? pJ->m_dLengthSlabToShoe[1] : toM(pJ->m_dLengthSlabToShoe[1]);
	}
	else if(m_nTypeBridge==BRIDGE_TYPE_ARCH && nSpan==nJijumConnect)
	{
		double WT = GetThickWall(nJijumConnect, TRUE)/2;
		dDistChcek += bLsd? WT : toM(WT);
	}
	else
	{
		if(!bShearCheck && m_nSelectForceCheckPos==0)
			dDistChcek += (m_nSelectRigidModeling == 0)? 0.0 : dDistRigid;
		else
		{
			long nIdxHunchRight = GetIdxHunchInWall(nSpan,FALSE);
			// 경사교각일때는 벽체기울기 적용해야 하므로 별개로 처리
			if((m_nTypeBridge==BRIDGE_TYPE_TRAP || m_nTypeBridge==BRIDGE_TYPE_TRAP2) && nSpan>0)
			{
				CDPoint vAngInWall = GetAngleJijum(nSpan);
				double HWR = toM(Round(m_dHRW[nIdxHunchRight][iUPPER]/vAngInWall.y,0));
				dDistChcek += bLsd? frM(HWR) : HWR;
			}
			else
			{
				BOOL bRcSlab   = (m_nTypeBridge==BRIDGE_TYPE_SLAB) ? TRUE : FALSE;
//				BOOL bTrapType = (m_nTypeBridge==BRIDGE_TYPE_TRAP || m_nTypeBridge==BRIDGE_TYPE_TRAP2) ? TRUE : FALSE;
				double WT = GetThickWall(nSpan, FALSE);
				double WR = Round(m_dWL[nIdxHunchRight][iUPPER],0);
				
				CDPoint vAng = GetAngleJijum(nSpan);
				if(!Compare(WT/2,WR,"=") && !bRcSlab)
				{
					if(bLsd)
						dDistChcek += (nSpan==0)? WT/2/vAng.y : WT/2;
					else
						dDistChcek += (nSpan==0)? toM(WT/2)/vAng.y : toM(WT/2);
				}
				else
				{
					if(bLsd)
						dDistChcek += (nSpan==0)? WR/vAng.y : WR;
					else
						dDistChcek += (nSpan==0)? toM(WR)/vAng.y : toM(WR);
				}
			}
		}
	}

	// 벽체연단에서의 거리를 더한다.
	if(bShearCheck)
		dDistChcek += bLsd? dDistShearPos : toM(dDistShearPos);

	long nSizeElement = pFEM->GetElementSize();
	for(long nEle=0; nEle<nSizeElement; nEle++)
	{
		CElement *pEle = pFEM->GetElement((USHORT)nEle);
		if(bUpperSlab && pEle->m_nFlag != ELEMENT_UPPERSLAB) continue;
		if(!bUpperSlab && pEle->m_nFlag != ELEMENT_LOWERSLAB) continue;

		CJoint *pJnt = pFEM->GetJoint(pEle->m_nJStt);
		if(Compare(pJnt->m_vPointOrigin.x, dDistChcek, _T("="), 0.006))
		{
			nElement = nEle;
			dDistStt = 0;
			break;
		}
		else if((m_nSelectRigidModeling == 2 || (bShearCheck && dDistShearPos > 0)) && pJnt->m_vPointOrigin.x < dDistChcek)
		{
			CJoint *pJntEnd = pFEM->GetJoint(pEle->m_nJEnd);
			if(pJntEnd && pJntEnd->m_vPointOrigin.x > dDistChcek)
			{
				dDistStt = (bShearCheck && dDistShearPos > 0)? dDistChcek - pJnt->m_vPointOrigin.x : 0;
				return nEle;
			}
		}
	}

	return nElement;
}

long CRcBridgeApp::GetNumberElementSlabSpanEndCheck(long nSpan, long nTypeDesign, BOOL bShearCheck, BOOL bGirderCalc, BOOL bUpperSlab, double dDistShearPos, double &dDistEnd)
{
	CFEMManage *pFEM = GetFemManage(nTypeDesign);
	if(pFEM == NULL) return -1;

	BOOL bLsd = m_pARcBridgeDataStd->IsLsdDesign();
	long nElement = -1;
	long nJijumConnect = GetJijumConnectRibAndSlab();

	// 무한강성이 아닌 경우는 강역에 해당하는 절점이 생성되지 않는다.
	// (ARCBRIDGE-3196) endoffset을 적용했어도 강역길이보다 짧은 부재가 있는경우 제외 시켜 줘야 한다.
	double dDistRigid = (bGirderCalc || m_nSelectRigidModeling == 0) ? 0 : toM(GetDistRigidEndSlab(nSpan+1,TRUE,bUpperSlab));
	double dDistChcek = 0;
	// 강역길이가 5mm 이하일 경우 부재가 추가되지 않음으로 여기서도 제외시켜줘야한다.
	if(dDistRigid <= 0.005) dDistRigid = 0;

	if(bLsd)
	{
		dDistRigid = frM(dDistRigid);
	}

	long i=0; for(i=0; i<=nSpan; i++)
	{
		dDistChcek += bLsd? GetLengthModelSpan(i) : toM(GetLengthModelSpan(i));
	}
	if(nSpan==m_nQtyJigan-1 && (IsRcSlab() || IsPiRahmen()))
	{
		CDPoint vAng = GetAngleJijum(nSpan+1);
		CJijum *pJ = GetJijum(nSpan+1);
		dDistChcek -= bLsd? pJ->m_dLengthSlabToShoe[0] : toM(pJ->m_dLengthSlabToShoe[0]);
	}
	else if(m_nTypeBridge==BRIDGE_TYPE_ARCH && nSpan==nJijumConnect-1)
	{
		double WT = GetThickWall(nJijumConnect, TRUE)/2;
		dDistChcek -= bLsd? WT : toM(WT);
	}
	else
	{
		if(!bShearCheck && m_nSelectForceCheckPos==0)
			dDistChcek -= (m_nSelectRigidModeling == 0)? 0.0 : dDistRigid;
		else
		{
			long nIdxHunchLeft  = GetIdxHunchInWall(nSpan,TRUE);
			if((m_nTypeBridge==BRIDGE_TYPE_TRAP || m_nTypeBridge==BRIDGE_TYPE_TRAP2) && nSpan<m_nQtyJigan-1)
			{
				CDPoint vAngInWall = GetAngleJijum(nSpan);
				double HWL = toM(Round(m_dHLW[nIdxHunchLeft][iUPPER]/vAngInWall.y,0));
				dDistChcek -= bLsd? frM(HWL) : HWL;
			}
			else
			{
				BOOL bRcSlab   = (m_nTypeBridge==BRIDGE_TYPE_SLAB) ? TRUE : FALSE;
//				BOOL bTrapType = (m_nTypeBridge==BRIDGE_TYPE_TRAP || m_nTypeBridge==BRIDGE_TYPE_TRAP2) ? TRUE : FALSE;
				double WT = GetThickWall(nSpan+1, FALSE);
				double WL = Round(m_dWL[nIdxHunchLeft][iUPPER],0);

				CDPoint vAng = GetAngleJijum(nSpan+1);
				if(!Compare(WT/2,WL,"=") && !bRcSlab)
				{
					if(bLsd)
						dDistChcek -= (nSpan==m_nQtyJigan-1)? (WT/2/vAng.y) : (WT/2);
					else
						dDistChcek -= (nSpan==m_nQtyJigan-1)? toM(WT/2)/vAng.y : toM(WT/2);
				}
				else
				{
					if(bLsd)
						dDistChcek -= (nSpan==m_nQtyJigan-1)? (WL/vAng.y) : WL;
					else
						dDistChcek -= (nSpan==m_nQtyJigan-1)? toM(WL)/vAng.y : toM(WL);
				}
			}
		}
	}

	// 벽체연단에서의 거리를 더한다.
	if(bShearCheck)
		dDistChcek -= bLsd? dDistShearPos : toM(dDistShearPos);

	long nSizeElement = pFEM->GetElementSize();
	for(long nEle=0; nEle<nSizeElement; nEle++)
	{
		CElement *pEle = pFEM->GetElement((USHORT)nEle);
		if(bUpperSlab && pEle->m_nFlag != ELEMENT_UPPERSLAB) continue;
		if(!bUpperSlab && pEle->m_nFlag != ELEMENT_LOWERSLAB) continue;

		CJoint *pJnt = pFEM->GetJoint(pEle->m_nJEnd);
		if(Compare(pJnt->m_vPointOrigin.x,dDistChcek,"=", bLsd? 6 : 0.006))
		{
			nElement = nEle;
			dDistEnd = 0;
			break;
		}
		else if((m_nSelectRigidModeling == 2 || (bShearCheck && dDistShearPos > 0)) && pJnt->m_vPointOrigin.x > dDistChcek)
		{
			CJoint *pJntStt = pFEM->GetJoint(pEle->m_nJStt);
			if(pJntStt && pJntStt->m_vPointOrigin.x < dDistChcek)
			{
				dDistEnd = (bShearCheck && dDistShearPos > 0)? pJnt->m_vPointOrigin.x - dDistChcek : 0;
				return nEle;
			}
		}
	}

	return nElement;
}

// nIdxPile위치를 부재력을 설정하고 절점을 리턴
long CRcBridgeApp::GetOutValueForPileDesign_Usd(long nTypeCombo, long nIdxPile, double &dMoment, double &dShear, double &dAxial, double &dDispX, double &dDispZ, double &dDispR)
{
	dMoment = dAxial = dShear = dDispX = dDispZ = dDispR = 0;

	if(IsBoxType() == FALSE) return -1;

	CFEMManage *pFEM = &m_FemNormal;
	if(nTypeCombo == COMBO_EARTHQUAKE || nTypeCombo == COMBO_EARTHQUAKE2)
	{
		pFEM = nTypeCombo == COMBO_EARTHQUAKE ? &m_FemEQ[0] : &m_FemEQ[1];
	}
	if(pFEM->IsOutputLoaded()==FALSE) return -1;

	CString szCombo(_T(""));
	if(nTypeCombo==COMBO_SERVICE)	  szCombo = "ENVUSE MAX";
	if(nTypeCombo==COMBO_ULTIMATE)    szCombo = "ENVULT MAX";
	if(nTypeCombo==COMBO_ECCENTRIC)   szCombo = "ENVECC MAX";
	if(nTypeCombo==COMBO_EARTHQUAKE)  szCombo = "ENV MAX";
	if(nTypeCombo==COMBO_EARTHQUAKE2) szCombo = "ENV MAX";

	long idx = GetOutValueForPileDesign(pFEM, szCombo, nIdxPile, dMoment, dShear, dAxial, dDispX, dDispZ, dDispR);
	
	return idx;
}

long CRcBridgeApp::GetOutValueForPileDesign_Lsd(long nTypeCombo, long nIdxPile, double &dMoment, double &dShear, double &dAxial, double &dDispX, double &dDispZ, double &dDispR)
{
	dMoment = dAxial = dShear = dDispX = dDispZ = dDispR = 0;

	if(IsBoxType() == FALSE) return -1;

	CFEMManage *pFEM = &m_FemNormal;
	BOOL bEarthQuake = (nTypeCombo == eEnvExtremeI || nTypeCombo == eEnvExtremeII);
	if(bEarthQuake)
	{
		pFEM = &m_FemEQ[0];
	}
	if(pFEM->IsOutputLoaded()==FALSE) return -1;

	CString szCombo(_T(""));
	
	ETypeEnvComboLsd eTypeEnv = (ETypeEnvComboLsd)nTypeCombo;
	ELSDCombKind eComboKind = GetComboKindByEnvType(eTypeEnv, ePartLowerSlab, CHECK_POS_CENTER);

	szCombo = GetStringComboEnv_Lsd(eComboKind, ePartLowerSlab, TRUE);
	long nLoadCase = pFEM->GetLoadCaseByString(szCombo);
	if(nLoadCase < 0)
	{
		szCombo += _T(" MAX");
	}
	
	long idx = GetOutValueForPileDesign(pFEM, szCombo, nIdxPile, dMoment, dShear, dAxial, dDispX, dDispZ, dDispR);

	if(bEarthQuake && m_nSelectModelingCountEQ == 2)
	{
		pFEM = &m_FemEQ[1];
		if(pFEM->IsOutputLoaded()==TRUE)
		{
			double dMoment2(0), dShear2(0), dAxial2(0), dDispX2(0), dDispZ2(0), dDispR2(0);
			GetOutValueForPileDesign(pFEM, szCombo, nIdxPile, dMoment2, dShear2, dAxial2, dDispX2, dDispZ2, dDispR2);

			if(fabs(dMoment) < fabs(dMoment2))	dMoment = dMoment2;
			if(fabs(dShear) < fabs(dShear2))	dShear = dShear2;
			if(fabs(dAxial) < fabs(dAxial2))	dAxial = dAxial2;
			if(fabs(dDispX) < fabs(dDispX2))	dDispX = dDispX2;
			if(fabs(dDispZ) < fabs(dDispZ2))	dDispZ = dDispZ2;
			if(fabs(dDispR) < fabs(dDispR2))	dDispR = dDispR2;
		}
	}

	return idx;
}

long CRcBridgeApp::GetOutValueForPileDesign( CFEMManage *pFEM, CString szCombo,long nIdxPile, double &dMoment, double &dShear, double &dAxial, double &dDispX, double &dDispZ, double &dDispR )
{
	long nJijum = 0;
	CFootingApp *pFooting = GetFootingByJijumNumber(nJijum);

	BOOL bLsd = m_pARcBridgeDataStd->IsLsdDesign();
	double dApplyWidth = 1.0;
	if(pFEM != &m_FemNormal)
	{
		CDPoint vAng = CDPoint(0,1);
		double Station   = GetStationOnJijum(0, 0);
		dApplyWidth = bLsd? GetWidthSlabAct(Station,vAng) : toM(GetWidthSlabAct(Station,vAng));	// 슬래브폭원(교량폭,벽체폭)
	}

	long nLoadCase = pFEM->GetLoadCaseByString(szCombo);
	if(nLoadCase < 0) return -1;

	long nSizeJoint = pFEM->GetJointSize();
	for(long ix = 0; ix < nSizeJoint; ix++)
	{
		CJoint *pJoint = pFEM->GetJoint((USHORT)ix);
		if(pJoint->m_LongAttrib[LONG_0_JOINT_TYPE] != JOINT_LOWERSLAB) continue;
		if(pJoint->m_BoolAttrib[BOOL_0_JOINT_SPRING] == FALSE) continue;

		// 하부슬래브 시작위치는 벽체 스프링과 공유가 되어 말뚝개수와 안 맞을 수 있다. 말뚝없이 스프링이 존재 => sta을 비교
		double dStationPile = GetStationPileOrAnchor(pFooting, nIdxPile, FALSE);
		if(Compare(pJoint->m_vPointReal.x, dStationPile, _T("="), MIN_JOINT_DIST)) 
		{
			BOOL bSpringForce = (pJoint->IsFix()==FALSE && (m_nSelect2DFrameTool==SOLVE_SAP_6AND7 || m_nSelect2DFrameTool==SOLVE_SAP_8)) ? TRUE : FALSE;

			for(long nC = nLoadCase; nC <= nLoadCase+1; nC++)
			{
				CResultForce *pResult = pJoint->GetResultForce(nC);
				if(pResult == NULL) continue;

				if(bSpringForce)
				{
					dMoment = MAX(fabs(pResult->m_SpringMoment.y / dApplyWidth), dMoment);
					dAxial  = MAX(fabs(pResult->m_SpringForce.z / dApplyWidth), dAxial);
					dShear  = MAX(fabs(pResult->m_SpringForce.x / dApplyWidth), dShear);
				}
				else
				{
					dMoment = MAX(fabs(pResult->m_Moment.y / dApplyWidth), dMoment);
					dAxial  = MAX(fabs(pResult->m_Force.z / dApplyWidth), dAxial);
					dShear  = MAX(fabs(pResult->m_Force.x / dApplyWidth), dShear);
				}
				
				dDispX  = MAX(fabs(pResult->m_Disp.x), dDispX);
				dDispZ  = MAX(fabs(pResult->m_Disp.z), dDispZ);
				dDispR  = MAX(fabs(pResult->m_Rotate.y), dDispR);
			}

			return ix;
		}
	}

	return -1;
}

long CRcBridgeApp::GetOutValueForFootingDesign(long nJijum, long nTypeCheck, double &dMoment, double &dShear, double &dAxial, BOOL bUseForce, long nJijum_Add, long nJijumReal, BOOL bNoneImpectCase)
{
	CFEMManage *pFEM = (nTypeCheck==FOOTING_CHECK_EQ) ? &m_FemEQ[1] : &m_FemNormal;
	if(pFEM->IsOutputLoaded()==FALSE) return -1;

	long nComboFind		 = 0;
	long nTypeDesignStt = bUseForce ? COMBO_SERVICE : COMBO_ULTIMATE;
	if(nTypeCheck==FOOTING_CHECK_EQ) nTypeDesignStt = COMBO_EARTHQUAKE;
	long nTypeDesignEnd = nTypeDesignStt;
	if(nTypeCheck==FOOTING_CHECK_EQ && m_nSelectModelingCountEQ==2) 
	{
		nTypeDesignEnd = COMBO_EARTHQUAKE2;
	}

	long nSizeCombo = bUseForce ? m_MatComboUse.GetRow() : m_MatComboUlt.GetRow();
	CString szComboHead = bUseForce ? "SV" : "UL";
	if(nTypeCheck==FOOTING_CHECK_EQ) 
	{
		szComboHead = "EQ";
		nSizeCombo = m_MatComboEQ.GetRow();
	}
	if(nSizeCombo <= 0) return -1;
	CString szComboAdd = (bNoneImpectCase && nTypeCheck!=FOOTING_CHECK_EQ) ? _T("-S") : _T("");
	
	double dCurE	 = 0;
	double dMaxE	 = 0;
	double dCurValue = 0;
	double dMaxValue = 0;
	double dMinValue = 0;

	// bMinFind를 실제 지점번호로 판단.. 아치에서 바꿔서 온 경우 잘못됨
	long nJijumMinCheck = (nJijumReal==-1) ? nJijum : nJijumReal;
	BOOL bMinFind   = (nTypeCheck==FOOTING_CHECK_AXIAL || 
		               (nJijumMinCheck<m_nQtyJigan  && nTypeCheck==FOOTING_CHECK_MOMENT_PLUS) || 
					   (nJijumMinCheck==m_nQtyJigan && nTypeCheck==FOOTING_CHECK_MOMENT_MINUS) ) ? FALSE : TRUE;
	BOOL bFirst   = TRUE;
	// 축력최대시   : 최대축력   + 최대모멘트(절대값)의 조합으로
	// 모멘트최대시 : 최대모멘트 + 최대축력
	CResultForce *pResultMax = NULL;
	CResultForce *pResultMin = NULL;
	BOOL bSpringForce = FALSE;
	for(long nTypeDesign=nTypeDesignStt; nTypeDesign<=nTypeDesignEnd; nTypeDesign++)
	{
		if(nTypeDesign == COMBO_EARTHQUAKE2)
		{
			pFEM = &m_FemEQ[2];
		}
		long nElement = GetNumberElementWallEndCheck(nJijum,nTypeDesign,FALSE,FALSE);
		if(nTypeCheck==FOOTING_CHECK_EQ && GetTypeJointPosFootingEQ()==1)
		{
			if(m_nTypeBridge == BRIDGE_TYPE_ARCH && nJijum == m_nQtyJigan+1) nElement--;
			else nElement++;
		}
		CString szCombo = _T("");
		CElement *pEle = pFEM->GetElement((USHORT)nElement);
		if(!pEle) continue;

		CJoint *pJoint, *pJoint_Add;
		if(nJijum == m_nQtyJigan+1) pJoint= pFEM->GetJoint(pEle->m_nJStt);		// Arch리브의 시점측은 지점즉이 부재의 시작부분이다.
		else pJoint= pFEM->GetJoint(pEle->m_nJEnd);
//////////////// TRAP2, ARCH형의 기초는 연결된 지점의 반력을 고려해서 최대값을 찾는다.
		if(nJijum_Add > -1)
		{
			nElement = GetNumberElementWallEndCheck(nJijum_Add,nTypeDesign,FALSE,FALSE);
			if(nTypeCheck==FOOTING_CHECK_EQ && GetTypeJointPosFootingEQ()==1)
			{
				if(m_nTypeBridge == BRIDGE_TYPE_ARCH && nJijum_Add == m_nQtyJigan+1) nElement--;
				else nElement++;
			}
			if(nElement < 0) return 0;
			pEle = pFEM->GetElement((USHORT)nElement);
			if(nJijum_Add == m_nQtyJigan+1) pJoint_Add= pFEM->GetJoint(pEle->m_nJStt);		// Arch리브의 시점측은 지점즉이 부재의 시작부분이다.
			else pJoint_Add= pFEM->GetJoint(pEle->m_nJEnd);
		}
//////////////// 
		// SAP인경우만 스프링반력을 적용, MIDAS는 지점반력에서 모두 표현됨...
		CFootingApp *pFoot = GetFootingByJijumNumber(nJijum);
		double dJ = 0;
		if(nTypeCheck!=FOOTING_CHECK_EQ || m_nSelectModelingCountEQ!=2 || m_nSelectFootForce==0) dJ = 0;
		else
		{
			if(pFoot)
			{
				dJ = (GetTypeJointPosFootingEQ()==1) ? toM(pFoot->GetHeight()/2) : toM(pFoot->GetHeight());
			}
			else
			{
				// 기초가 없다면 경사교2 타입이다.
				CWallApp *pWall	= GetInWall(nJijum-1);
				if(pWall)
				{
					pFoot	= nJijum <= m_nQtyJigan/2 ? &m_footingStt : &m_footingEnd;
					CTwinVector tvLowerLeft	= pWall->m_tvArrFront.GetTvByInfo("벽체하면좌측");
					dJ	= tvLowerLeft.m_v1.y - (pFoot->m_dEL - pFoot->GetHeight());
				}
			}
		}
		
		bSpringForce = (pJoint->IsFix()==FALSE && (m_nSelect2DFrameTool==SOLVE_SAP_6AND7 || m_nSelect2DFrameTool==SOLVE_SAP_8)) ? TRUE : FALSE;
		nSizeCombo = GetSizeCombo(nTypeDesign, TRUE);
		for(long nCombo=0; nCombo<nSizeCombo; nCombo++)
		{
			for(long nMin=0; nMin<2; nMin++)
			{
				szCombo.Format("%s%02d%s %s",szComboHead,nCombo+1,szComboAdd,(nMin==0)?"MAX":"MIN");
				long nLoadCase = pFEM->GetLoadCaseByString(szCombo);
				if(nLoadCase==-1) 
				{
					szCombo.Format("%s%02d%s",szComboHead,nCombo+1,szComboAdd);
					nLoadCase = pFEM->GetLoadCaseByString(szCombo);
					if(nLoadCase==-1) continue;
				}

				CResultForce *pResult = pJoint->GetResultForce(nLoadCase);
				if(pResult == NULL) continue;

				dCurValue = (nTypeCheck==FOOTING_CHECK_AXIAL) ? pResult->m_Force.z : pResult->m_Moment.y;
				dCurE	= (pResult->m_Moment.y + pResult->m_Force.x * dJ)/pResult->m_Force.z;
				if(bSpringForce)
				{
					dCurValue = pResult->m_SpringMoment.y;
					dCurE	= (pResult->m_SpringMoment.y + pResult->m_SpringForce.x * dJ)/pResult->m_SpringForce.z;
				}
			////////////////
				if(nJijum_Add > -1)
				{
					CResultForce *pResult_Add = pJoint_Add->GetResultForce(nLoadCase);
					
					if(bSpringForce)
					{
						dCurValue += pResult_Add->m_SpringMoment.y;
						dCurE	+= (pResult_Add->m_SpringMoment.y + pResult_Add->m_SpringForce.x * dJ)/pResult_Add->m_SpringForce.z;
					}
					else
					{
						dCurValue += (nTypeCheck==FOOTING_CHECK_AXIAL) ? pResult_Add->m_Force.z : pResult_Add->m_Moment.y;
						dCurE	+= (pResult_Add->m_Moment.y + pResult_Add->m_Force.x * dJ)/pResult_Add->m_Force.z;
					}
				}
			////////////////
				if(nTypeCheck == FOOTING_CHECK_EQ)
				{
					if(bFirst || fabs(dCurE) > fabs(dMaxE))
					{
						dMaxE  = dCurE;
						nComboFind = nCombo;
						pResultMin = pResult;
						pResultMax = pResult;
						if(szCombo.Find("MAX")!=-1)	pResultMin = pJoint->GetResultForce(nLoadCase+1);
						if(szCombo.Find("MIN")!=-1)	pResultMax = pJoint->GetResultForce(nLoadCase-1);
					}
				}
				else
				{
					if(bFirst || (bMinFind && dCurValue<dMinValue))
					{
						dMinValue  = dCurValue;
						nComboFind = nCombo;
						pResultMin = pResult;
						pResultMax = pResult;
						if(szCombo.Find("MAX")!=-1)	pResultMin = pJoint->GetResultForce(nLoadCase+1);
						if(szCombo.Find("MIN")!=-1)	pResultMax = pJoint->GetResultForce(nLoadCase-1);
					}
					if(bFirst || (!bMinFind && dCurValue>dMaxValue))
					{
						dMaxValue  = dCurValue;
						nComboFind = nCombo;
						pResultMin = pResult;
						pResultMax = pResult;
						if(szCombo.Find("MAX")!=-1)	pResultMin = pJoint->GetResultForce(nLoadCase+1);
						if(szCombo.Find("MIN")!=-1)	pResultMax = pJoint->GetResultForce(nLoadCase-1);
					}
				}
				
				bFirst = FALSE;
			}
		}
	}

	if(pResultMin==NULL || pResultMax==NULL) return -1;
	
	double Station   = GetStationOnJijum(0,0);
	double SlabWidth = toM(GetWidthSlabAct(Station,CDPoint(0,1)));	// 슬래브폭원(교량폭,벽체폭)
	double dDivide   = (nTypeCheck==FOOTING_CHECK_EQ) ? SlabWidth : 1.00;
	// 종점기초는 기준점이 반대쪽이이므로 방향을 반대로 돌려준다.
	// 경사기초2는 끝에서 두번째 지점도. 아치교도 적용
	// (ARCBRIDGE-1532) 경사기초1의 끝에서 두번째 지점도 반대로 적용해줘야 한다.
	double dSwValue  = (nJijum<m_nQtyJigan) ? -1.0 : 1.0;
	if(((m_nTypeBridge==BRIDGE_TYPE_TRAP2 || m_nTypeBridge==BRIDGE_TYPE_TRAP) && nJijum==m_nQtyJigan-1))
		dSwValue  = 1.0;
	if(m_nTypeBridge==BRIDGE_TYPE_ARCH && nJijum>m_nQtyJigan)
	{
		dSwValue = (nJijum == m_nQtyJigan+1)? -1.0 : 1.0;
	}

	if(m_nSelectFootingReactionForce == 0)
	{
		// Case별로 집계
		BOOL bResultMax = TRUE;
		if(nTypeCheck==FOOTING_CHECK_AXIAL)
		{
			double dValueMax = bSpringForce? fabs(pResultMax->m_SpringForce.z) : fabs(pResultMax->m_Force.z);
			double dValueMin = bSpringForce? fabs(pResultMin->m_SpringForce.z) : fabs(pResultMin->m_Force.z);

			if(Compare(fabs(dValueMax), fabs(dValueMin), _T("="), 0.1))
			{
				double dValueMoMax = bSpringForce? fabs(pResultMax->m_SpringMoment.y) : fabs(pResultMax->m_Moment.y);
				double dValueMoMin = bSpringForce? fabs(pResultMin->m_SpringMoment.y) : fabs(pResultMin->m_Moment.y);

				bResultMax = (dValueMoMax > dValueMoMin)? TRUE : FALSE;
			}
			else
			{
				bResultMax = (dValueMax > dValueMin)? TRUE : FALSE;
			}
		}
		else
		{
			if(bSpringForce) bResultMax = (pResultMax->m_SpringMoment.y > pResultMin->m_SpringMoment.y)? TRUE : FALSE;
			else bResultMax = (pResultMax->m_Moment.y > pResultMin->m_Moment.y)? TRUE : FALSE;

			if(bMinFind) bResultMax = !bResultMax;
		}

		if(bSpringForce)
		{
			dAxial  = bResultMax? pResultMax->m_SpringForce.z : pResultMin->m_SpringForce.z;
			dShear  = bResultMax? pResultMax->m_SpringForce.x : pResultMin->m_SpringForce.x;
			dMoment = bResultMax? pResultMax->m_SpringMoment.y : pResultMin->m_SpringMoment.y;
		}
		else
		{
			dAxial  = bResultMax? pResultMax->m_Force.z : pResultMin->m_Force.z;
			dShear  = bResultMax? pResultMax->m_Force.x : pResultMin->m_Force.x;
			dMoment = bResultMax? pResultMax->m_Moment.y : pResultMin->m_Moment.y;
		}
	}
	else if(m_nSelectFootingReactionForce == 1)
	{
		// 부호별로 집계
		BOOL bResultMax = TRUE;
		if(nTypeCheck==FOOTING_CHECK_AXIAL)
		{
			if(bSpringForce) bResultMax = (fabs(pResultMax->m_SpringForce.z) > fabs(pResultMin->m_SpringForce.z))? TRUE : FALSE;
			else bResultMax = (fabs(pResultMax->m_Force.z) > fabs(pResultMin->m_Force.z))? TRUE : FALSE;
		}
		else
		{
			if(bSpringForce) bResultMax = (pResultMax->m_SpringMoment.y > pResultMin->m_SpringMoment.y)? TRUE : FALSE;
			else bResultMax = (pResultMax->m_Moment.y > pResultMin->m_Moment.y)? TRUE : FALSE;

			if(bMinFind) bResultMax = !bResultMax;
		}

		// Case별로 집계방법과 동일하게 Max, min을 찾고 수평력만 모멘트와 같은 부호로 찾아주자.

		if(bSpringForce)
		{
			dAxial  = bResultMax? pResultMax->m_SpringForce.z : pResultMin->m_SpringForce.z;
			dMoment = bResultMax? pResultMax->m_SpringMoment.y : pResultMin->m_SpringMoment.y;
			if(dMoment > 0) dShear  = max(pResultMax->m_SpringForce.x, pResultMin->m_SpringForce.x);
			else dShear  = min(pResultMax->m_SpringForce.x, pResultMin->m_SpringForce.x);
		}
		else
		{
			dAxial  = bResultMax? pResultMax->m_Force.z : pResultMin->m_Force.z;
			dMoment = bResultMax? pResultMax->m_Moment.y : pResultMin->m_Moment.y;
			if(dMoment > 0) dShear  = max(pResultMax->m_Force.x, pResultMin->m_Force.x);
			else dShear  = min(pResultMax->m_Force.x, pResultMin->m_Force.x);
		}
	}
	else if(m_nSelectFootingReactionForce == 2 || m_nSelectFootingReactionForce == 3)
	{
		// MAX,MIN의 절대값이 큰 것들을 조합해서 사용
		if(bSpringForce)
		{
			if(nTypeCheck==FOOTING_CHECK_AXIAL || m_nSelectFootingReactionForce == 3)
				dAxial  = max(fabs(pResultMax->m_SpringForce.z),fabs(pResultMin->m_SpringForce.z));
			else
				dAxial  = min(fabs(pResultMax->m_SpringForce.z),fabs(pResultMin->m_SpringForce.z));
			dShear  = pResultMax->m_SpringForce.x;
			if(fabs(pResultMax->m_SpringForce.x)<fabs(pResultMin->m_SpringForce.x))
				dShear  = pResultMin->m_SpringForce.x;
			dMoment = pResultMax->m_SpringMoment.y;
			if(pResultMax!=pResultMin)
			{
				if(nTypeCheck==FOOTING_CHECK_AXIAL)
				{
					if(fabs(pResultMax->m_SpringMoment.y)<fabs(pResultMin->m_SpringMoment.y))
						dMoment = pResultMin->m_SpringMoment.y;
				}
				else if(bMinFind && pResultMax->m_SpringMoment.y > pResultMin->m_SpringMoment.y)
				{
					dMoment = pResultMin->m_SpringMoment.y;
				}
			}
		}
		else
		{
			if(nTypeCheck==FOOTING_CHECK_AXIAL || m_nSelectFootingReactionForce == 3)
				dAxial  = max(fabs(pResultMax->m_Force.z),fabs(pResultMin->m_Force.z));
			else
				dAxial  = min(fabs(pResultMax->m_Force.z),fabs(pResultMin->m_Force.z));
			dShear  = pResultMax->m_Force.x;
			if(fabs(pResultMax->m_Force.x)<fabs(pResultMin->m_Force.x))
				dShear  = pResultMin->m_Force.x;
			dMoment = pResultMax->m_Moment.y;
			if(pResultMax!=pResultMin)
			{
				if(nTypeCheck==FOOTING_CHECK_AXIAL)
				{
					if(fabs(pResultMax->m_Moment.y)<fabs(pResultMin->m_Moment.y))
					{
						dMoment = pResultMin->m_Moment.y;
					}
				}
				else if(bMinFind && pResultMax->m_Moment.y > pResultMin->m_Moment.y)
				{
					dMoment = pResultMin->m_Moment.y;
				}
			}
		}
	}

	dMoment = dMoment*dSwValue/dDivide;
	dShear  = dShear*dSwValue/dDivide;
	dAxial  = dAxial/dDivide;

	return nComboFind;
}

// nPos : 0 좌측, 1 중앙, 2 우측
double CRcBridgeApp::GetOutValueSlabEQ(long nSpan, long nPos, long nTypeForce, BOOL bUpperSlab, ETypeEnvComboLsd eEnvTypeLsd, double dDist)
{
	double dEqRet(0);
	long nSizeEq = GetCountEarthQuakeModel();
	for(long ix=0; ix<nSizeEq; ++ix)
	{
		double dEQ1 = GetOutValueSlab(nSpan, nPos, COMBO_EARTHQUAKE + ix, nTypeForce, TRUE, FALSE, bUpperSlab, eEnvTypeLsd, dDist);
		if(ix == 0)
			dEqRet = dEQ1;
		else
		{
			if(nTypeForce==ELE_MOMENT2 && nPos!=CHECK_POS_CENTER)
				dEqRet = bUpperSlab ? min(dEQ1,dEqRet) : max(dEQ1,dEqRet);
			else
				dEqRet = bUpperSlab ? max(dEQ1,dEqRet) : min(dEQ1,dEqRet);
		}
	}

	return dEqRet;
}

double CRcBridgeApp::GetOutValuePRFEQ( BOOL bStt, long nTypeForce, ETypeEnvComboLsd eEnvTypeLsd/*=eEnvLimit*/ )
{
	double dEqRet(0);
	long nSizeEq = GetCountEarthQuakeModel();
	for(long ix=0; ix<nSizeEq; ++ix)
	{
		double dEQ1 = GetOutValuePRF(bStt, COMBO_EARTHQUAKE+ix, nTypeForce, eEnvTypeLsd);
		if(ix == 0)
			dEqRet = dEQ1;
		else
		{
			if(nTypeForce==ELE_MOMENT2)
				dEqRet = min(dEQ1,dEqRet);
			else
				dEqRet = max(dEQ1,dEqRet);
		}
	}

	return dEqRet;
}

// 거더 작용력을 구하기 위한 함수다. 슬래브 단면검토에 쓰이는게 아니다.
double CRcBridgeApp::GetOutValueShearSlabOnJijum(long nJijum,  long nPos, long nTypeDesign, BOOL bUpperSlab, ETypeEnvComboLsd eEnvTypeLsd)
{
	CFEMManage *pFEM = GetFemManage(nTypeDesign);
	if(pFEM == NULL || pFEM->IsOutputLoaded()==FALSE) return 0;
	if(IsUseModelCombo(nTypeDesign) == FALSE) return 0;

	double dValueMax  = 0;
	double dValueMin  = 0;

	long nSpanLeft    = nJijum-1;
	long nSpanRight   = nJijum;
	long nIdxEleLeft  = (nSpanLeft==-1) ? GetNumberElementSlabSpanSttCheck(nSpanRight, nTypeDesign, FALSE, TRUE, bUpperSlab) - 1
		                             : GetNumberElementSlabSpanEndCheck(nSpanLeft, nTypeDesign, FALSE, TRUE, bUpperSlab);
	long nIdxEleRight = nIdxEleLeft + 1;
	if(nIdxEleLeft==-1 || nIdxEleRight==-1) return 0;

	CString szCombo = GetStringComboEnv(nTypeDesign, eEnvTypeLsd, bUpperSlab? ePartUpperSlab : ePartLowerSlab, nPos);

	long nLoadCase  = pFEM->GetLoadCaseIndex(szCombo);	
	if(nLoadCase<0) return 0;

	CElement *pEle = NULL;
	CResultForceElement *pResultMax = NULL;
	CResultForceElement *pResultMin = NULL;

	// 하나의 부재에 대하여 부재력을 산출한다...
	long nIdxEle = (nPos==CHECK_POS_LEFT) ? nIdxEleLeft : nIdxEleRight;
	pEle = pFEM->GetElement((USHORT)nIdxEle);
	if(pEle == NULL) return 0;

	pResultMax = pEle->GetResultForce(nLoadCase);
	pResultMin = pEle->GetResultForce(nLoadCase+1);

	long nPoint  = (nPos==CHECK_POS_RIGHT) ? 0 : pResultMax->m_ResultForceElement.GetSize()-1;
	dValueMax  = pResultMax->m_ResultForceElement[nPoint]->m_dForceOnElement[ELE_SHEAR_2];
	dValueMin  = pResultMin->m_ResultForceElement[nPoint]->m_dForceOnElement[ELE_SHEAR_2];

	double dValueReturn = max(fabs(dValueMax),fabs(dValueMin));

	double Station   = GetStationOnJijum(0,0);
	double SlabWidth = toM(GetWidthSlabAct(Station,CDPoint(0,1)));	// 슬래브폭원(교량폭,벽체폭)
	double dDivide = (nTypeDesign>=COMBO_EARTHQUAKE) ? SlabWidth : 1.00;
	return dValueReturn/dDivide;
}

// nPos : 0 좌측, 1 중앙, 2 우측
double CRcBridgeApp::GetOutValueSlab(long nSpan, long nPos, long nTypeDesign, long nTypeForce, BOOL bOutSide, BOOL bGirderCalc, BOOL bUpperSlab, ETypeEnvComboLsd eEnvTypeLsd, double dDist)
{
	if(nTypeForce!=ELE_MOMENT2 && nTypeForce!=ELE_SHEAR_2) return 0;
//	부재 중앙부 전단력은 무조건 0으로 했는데 값 옵션 적용 하기로...
	if(!m_bCalcShearCenter && nPos==CHECK_POS_CENTER && nTypeForce==ELE_SHEAR_2) return 0;
	if(IsUseModelCombo(nTypeDesign) == FALSE) return 0;

	CFEMManage *pFEM = GetFemManage(nTypeDesign);
	if(pFEM == NULL || pFEM->IsOutputLoaded() == FALSE) return 0;

	double dOutputMax = 0;
	double dOutputMin = 0;
	double dValueMax  = 0;
	double dValueMin  = 0;

	BOOL bShearFoce = nTypeForce==ELE_SHEAR_2;
	double dDistStt(0), dDistEnd(0);

	long nIdxEleStt = GetNumberElementSlabSpanSttCheck(nSpan, nTypeDesign, bShearFoce, bGirderCalc, bUpperSlab, dDist, dDistStt);
	long nIdxEleEnd = GetNumberElementSlabSpanEndCheck(nSpan, nTypeDesign, bShearFoce, bGirderCalc, bUpperSlab, dDist, dDistEnd);
	if(nIdxEleStt==-1 || nIdxEleEnd==-1) return dOutputMax;

	CString szCombo = GetStringComboEnv(nTypeDesign, eEnvTypeLsd, bUpperSlab? ePartUpperSlab : ePartLowerSlab, nPos);
	long nLoadCase  = pFEM->GetLoadCaseIndex(szCombo);	
	if(nLoadCase<0) return dOutputMax;

	CElement *pEle = NULL;
	CResultForceElement *pResultMax = NULL;
	CResultForceElement *pResultMin = NULL;
	// 하나의 부재에 대하여 부재력을 산출한다...
	BOOL bFirst = TRUE;
	if(nPos==CHECK_POS_LEFT || nPos==CHECK_POS_RIGHT)
	{
		long nIdxEle = (nPos==0) ? nIdxEleStt : nIdxEleEnd;
		BOOL bSttPos = (nPos==0) ? TRUE : FALSE;
		pEle = pFEM->GetElement((USHORT)nIdxEle);
		double dDist = (nPos==0) ? dDistStt : dDistEnd;

		if(bShearFoce && dDist > 0)
		{
			// DistShear가 부재길이보다 큰경우 처리해준다.
			for(long ix=1; ix<=(nIdxEleEnd-nIdxEleStt); ++ix)
			{
				double dLenEle = pFEM->GetLengthElement(pEle);
				if(dDist >= dLenEle)
				{
					pEle = pFEM->GetElement((USHORT)(nIdxEle + ix * (nPos==CHECK_POS_LEFT? 1 : -1)));
					dDist -= dLenEle;
					if(pEle == NULL) return dOutputMax;
				}
				else
				{
					break;
				}
			}
		}

		pResultMax = pEle->GetResultForce(nLoadCase);
		pResultMin = pEle->GetResultForce(nLoadCase+1);

		if(bShearFoce && dDist > 0)
		{
			// 부재 끝에서 떨어진 위치에서 보간법으로 계산을 해준다.
			long nSizePoint = pResultMax->m_ResultForceElement.GetSize()-1;
			double dLenUnit = pFEM->GetLengthElement(pEle) / nSizePoint;
			long nPoint = (long)(dDist / dLenUnit);
			long nPoint1 = (nPos == 0) ? nPoint : nSizePoint-nPoint;
			long nPoint2 = (nPos == 0) ? nPoint1+1 : nPoint1-1;
			double dDistExt = dDist - dLenUnit * nPoint;
			double dValue1 = pResultMax->m_ResultForceElement[nPoint1]->m_dForceOnElement[nTypeForce];
			double dValue2 = pResultMax->m_ResultForceElement[nPoint2]->m_dForceOnElement[nTypeForce];
			dValueMax  = dValue1 + (dValue2-dValue1) * (dDistExt / dLenUnit);

			dValue1 = pResultMin->m_ResultForceElement[nPoint1]->m_dForceOnElement[nTypeForce];
			dValue2 = pResultMin->m_ResultForceElement[nPoint2]->m_dForceOnElement[nTypeForce];
			dValueMin  = dValue1 + (dValue2-dValue1) * (dDistExt / dLenUnit);
		}
		else
		{
			long nPoint  = bSttPos ? 0 : pResultMax->m_ResultForceElement.GetSize()-1;
			dValueMax  = pResultMax->m_ResultForceElement[nPoint]->m_dForceOnElement[nTypeForce];
			dValueMin  = pResultMin->m_ResultForceElement[nPoint]->m_dForceOnElement[nTypeForce];
		}
		dOutputMax = bFirst ? dValueMax : max(dValueMax,dOutputMax);
		dOutputMin = bFirst ? dValueMin : min(dValueMin,dOutputMin);
		bFirst = FALSE;
	}
	else
	{
		double dMaxM = 0;
		for(long nEle=nIdxEleStt; nEle<=nIdxEleEnd; nEle++)
		{
			pEle = pFEM->GetElement((USHORT)nEle);
			pResultMax = pEle->GetResultForce(nLoadCase);
			pResultMin = pEle->GetResultForce(nLoadCase+1);

			long nSizePoint = pResultMax->m_ResultForceElement.GetSize();
			long i=0; for(i=0; i<nSizePoint; i++)
			{
				if(nTypeForce == ELE_MOMENT2)
				{
					dValueMax  = pResultMax->m_ResultForceElement[i]->m_dForceOnElement[nTypeForce];
					dValueMin  = pResultMin->m_ResultForceElement[i]->m_dForceOnElement[nTypeForce];
					dOutputMax = bFirst ? dValueMax : max(dValueMax,dOutputMax);
					dOutputMin = bFirst ? dValueMin : min(dValueMin,dOutputMin);
					bFirst = FALSE;
				}
				else
				{
					double dM = pResultMax->m_ResultForceElement[i]->m_dForceOnElement[ELE_MOMENT2];

					if(bFirst || fabs(dMaxM) < fabs(dM))
					{
						dMaxM = dM;
						dValueMax  = pResultMax->m_ResultForceElement[i]->m_dForceOnElement[nTypeForce];
						dValueMin  = pResultMin->m_ResultForceElement[i]->m_dForceOnElement[nTypeForce];
					}
				}
			}
		}
	}

	double dValueReturn = dOutputMax;
	if(nTypeForce==ELE_MOMENT2 && nPos!=CHECK_POS_CENTER)
	{
		dValueReturn = (bOutSide) ? dOutputMin : dOutputMax;
	}
	if(bUpperSlab == FALSE) // 모멘트방향이 반대로 적용
	{
		dValueReturn = dOutputMin;
		if(nTypeForce==ELE_MOMENT2 && nPos!=CHECK_POS_CENTER)
		{
			dValueReturn = (bOutSide) ? dOutputMax : dOutputMin;
		}
	}
	if(nTypeForce==ELE_SHEAR_2)
	{
		dValueReturn = max(fabs(dOutputMax),fabs(dOutputMin));
	}

	BOOL bLsd = m_pARcBridgeDataStd->IsLsdDesign();
	double Station   = GetStationOnJijum(0,0);
	double SlabWidth = bLsd?  GetWidthSlabAct(Station,CDPoint(0,1)) : toM(GetWidthSlabAct(Station,CDPoint(0,1)));	// 슬래브폭원(교량폭,벽체폭)
	double dDivide = (nTypeDesign>=COMBO_EARTHQUAKE) ? SlabWidth : 1.00;
	return dValueReturn / dDivide;
}

double CRcBridgeApp::GetOutValuePRF( BOOL bStt, long nTypeDesign, long nTypeForce, ETypeEnvComboLsd eEnvTypeLsd )
{
	if(nTypeForce!=ELE_MOMENT2 && nTypeForce!=ELE_SHEAR_2) return 0;
	
	CFEMManage *pFEM = GetFemManage(nTypeDesign);
	if(pFEM == NULL || pFEM->IsOutputLoaded() == FALSE) return 0;

	double dOutputMax = 0;
	double dOutputMin = 0;

	long nIdxEle = GetNumberElementPRFCheck(nTypeDesign, bStt);
	
	CString szCombo = GetStringComboEnv(nTypeDesign, eEnvTypeLsd, ePartLowerSlab, bStt ? CHECK_POS_LEFT : CHECK_POS_RIGHT);
	long nLoadCase  = pFEM->GetLoadCaseIndex(szCombo);	
	if(nLoadCase<0) return dOutputMax;

	CElement *pEle = pFEM->GetElement((USHORT)nIdxEle);	
	
	CResultForceElement *pResultMax = pEle->GetResultForce(nLoadCase);
	CResultForceElement *pResultMin = pEle->GetResultForce(nLoadCase+1);
	
	long nPoint  = bStt ? pResultMax->m_ResultForceElement.GetSize()-1 : 0;
	dOutputMax  = pResultMax->m_ResultForceElement[nPoint]->m_dForceOnElement[nTypeForce];
	dOutputMin  = pResultMin->m_ResultForceElement[nPoint]->m_dForceOnElement[nTypeForce];
		
	double dValueReturn = dOutputMin;
	
	if(nTypeForce==ELE_SHEAR_2)
		dValueReturn = max(fabs(dOutputMax),fabs(dOutputMin));

	BOOL bLsd = m_pARcBridgeDataStd->IsLsdDesign();
	double Station   = GetStationOnJijum(0,0);
	double SlabWidth = bLsd?  GetWidthSlabAct(Station,CDPoint(0,1)) : toM(GetWidthSlabAct(Station,CDPoint(0,1)));	// 슬래브폭원(교량폭,벽체폭)
	double dDivide = (nTypeDesign>=COMBO_EARTHQUAKE) ? SlabWidth : 1.00;
	return dValueReturn / dDivide;
}

CJoint* CRcBridgeApp::GetJointSlabOnJijum(CFEMManage *pFEM, long nJijum, BOOL bUpperSlab)
{
	if(IsBoxType() == FALSE && bUpperSlab == FALSE)
		return NULL;

	long nSizeJnt = pFEM->GetJointSize();
	for(long i=0; i<nSizeJnt; i++)
	{
		CJoint *pJoint = NULL;
		pJoint = pFEM->GetJoint((USHORT)i);

		if(pJoint->m_LongAttrib[LONG_0_JOINT_TYPE] != JOINT_UPPERSLAB && pJoint->m_LongAttrib[LONG_0_JOINT_TYPE] != JOINT_LOWERSLAB) continue;

		// 상부슬래브
		if(bUpperSlab && pJoint->m_LongAttrib[LONG_0_JOINT_TYPE] == JOINT_UPPERSLAB && pJoint->m_BoolAttrib[BOOL_1_JOINT_JIJUM_CONNECT] && pJoint->m_LongAttrib[LONG_1_JOINT_JIJUMNO] == nJijum)
		{
			return pJoint;
		}
		// 하부슬래브
		if(bUpperSlab && pJoint->m_LongAttrib[LONG_0_JOINT_TYPE] == JOINT_LOWERSLAB && pJoint->m_BoolAttrib[BOOL_1_JOINT_JIJUM_CONNECT] && pJoint->m_LongAttrib[LONG_1_JOINT_JIJUMNO] == nJijum)
		{
			return pJoint;
		}
	}


	return NULL;
}


long CRcBridgeApp::GetNumberElementWallSttCheck(long nJijum, long nTypeDesign, BOOL bShearCheck, BOOL bGirderCalc, double dDistShearPos, double &dDistStt)
{
	CFEMManage *pFEM = GetFemManage(nTypeDesign);
	if(pFEM == NULL) return -1;

	BOOL bLsd = m_pARcBridgeDataStd->IsLsdDesign();
	BOOL bArch = (m_nTypeBridge == BRIDGE_TYPE_ARCH)? TRUE : FALSE;
	long nElement = -1;
//	GetTvVertSection_Std(0, TRUE, FALSE, FALSE, 0);

	if(bArch && nJijum > m_nQtyJigan)
	{
		long nSizeEle	= pFEM->GetElementSize();
		
		long i=0; for(i=0; i<nSizeEle; i++)
		{
			CElement *pEle = pFEM->GetElement((USHORT)i);
			CJoint   *pJnt = pFEM->GetJoint(pEle->m_nJEnd);
			
			if(pJnt->m_LongAttrib[1]<nJijum) continue;
			if(pJnt->m_LongAttrib[1]==nJijum && (pJnt->m_BoolAttrib[BOOL_2_JOINT_JIJUM_FIX] || pJnt->m_BoolAttrib[BOOL_0_JOINT_SPRING]))
			{
				nElement  = i;
				return nElement;
			}
		}
		return nElement;
	}
	CJoint *pJonitStt = GetJointSlabOnJijum(pFEM, nJijum, TRUE);

	double dDistRigid   = bLsd? GetDistRigidEndWall(nJijum) : toM(GetDistRigidEndWall(nJijum));

	if(m_nSelectRigidModeling == 0 && m_nSelectForceCheckPos==0)
	{
		// (ARCBRIDGE-2368) 강역 적용 안하고 최대 부재력 선택 했을때 부재 끝단을 적용한다.
		dDistRigid = 0.0;
	}

	double dHeightChcek = bGirderCalc ? pJonitStt->m_vPointOrigin.z : pJonitStt->m_vPointOrigin.z - dDistRigid;
	if(m_nSelectForceCheckPos==1 && bGirderCalc==FALSE)
	{
		dHeightChcek = pJonitStt->m_vPointOrigin.z - (bLsd? m_dTS : toM(m_dTS))/2; 
	}
	// ARCH교는 모델링 상부가 레벨이 아니므로 지점 마다의 El차를 고려해준다.
	// (ARCBRIDGE-2669) pJointStt가 각 지점의 위치기 때문에 시점EL과의 차이를 고려할 필요가 없다.
	double dElDiff = 0;
	if(FALSE)//(bArch)
	{
		CLineInfo *pLine = GetLineBase();
		double dElStt	 = pLine->GetElevationPlanEl(GetStationOnJijum(0)-m_dWS/2);
		double dDist	 = (nJijum==0)? (-m_dWS/2) : ((nJijum==m_nQtyJigan)? m_dWE/2 : 0);
		double dElCheck	 = pLine->GetElevationPlanEl(GetStationOnJijum(nJijum) + dDist);
		dElDiff	 = bLsd? (dElStt - dElCheck) : toM(dElStt - dElCheck);
	}

	// 슬래브연단에서의 거리를 더한다.
	if(bShearCheck)
		dHeightChcek -= bLsd? dDistShearPos : toM(dDistShearPos);

	long nSizeElement = pFEM->GetElementSize();
	for(long nEle=0; nEle<nSizeElement; nEle++)
	{
		CElement *pEle = pFEM->GetElement((USHORT)nEle);
		CJoint   *pJnt = pFEM->GetJoint(pEle->m_nJStt);
		if(pEle->m_nFlag==ELEMENT_UPPERSLAB) continue;	// SLAB
		if(pEle->m_nFlag==ELEMENT_LOWERSLAB) continue;	// SLAB
		if(pJnt->m_LongAttrib[1]!=nJijum) continue; // 지점번호

		if(Compare(pJnt->m_vPointOrigin.z,dHeightChcek-dElDiff,"=", bLsd? 5 : 0.005))
		{
			nElement = nEle;
			dDistStt = 0;
			break;
		}
		else if((m_nSelectRigidModeling == 2 || (bShearCheck && dDistShearPos > 0)) && pJnt->m_vPointOrigin.z > dHeightChcek-dElDiff) // End Offset으로 처리한 경우 부재사이에 검토점이 위치
		{
			CJoint *pJntEnd = pFEM->GetJoint(pEle->m_nJEnd);
			if(pJntEnd && pJntEnd->m_vPointOrigin.z < dHeightChcek-dElDiff)
			{
				dDistStt = (bShearCheck && dDistShearPos > 0)? pJnt->m_vPointOrigin.z - dHeightChcek : 0;
				return nEle;
			}
		}
	}

//	GetTvVertSection_Std(0, TRUE, FALSE, TRUE);
	return nElement;
}

long CRcBridgeApp::GetNumberElementWallEndCheck(long nJijum, long nTypeDesign, BOOL bShearCheck, BOOL bGirderCalc, double dDistShearPos, double &dDistEnd)
{
	CFEMManage *pFEM = GetFemManage(nTypeDesign);
	if(pFEM == NULL) return -1;

	BOOL bLsd = m_pARcBridgeDataStd->IsLsdDesign();
	BOOL bArch = (m_nTypeBridge == BRIDGE_TYPE_ARCH) ? TRUE : FALSE;
	long nElement = -1;
//	종단타입을 함수내에서 또 바꿔줄경우 선형타입을 복구할수 없다.. 최대한 필요한데에서만 바꾸자.
//	GetTvVertSection_Std(0, TRUE, FALSE, FALSE, bArch? 1:0);

	if(bArch && nJijum > m_nQtyJigan)
	{
		if(nJijum == m_nQtyJigan+1) nElement = GetNumberElementArchRibSpanSttCheck(0, nTypeDesign);
		else if(nJijum == m_nQtyJigan+2) nElement = GetNumberElementArchRibSpanEndCheck(m_nQtyJigan-1, nTypeDesign);
		else nElement = -1;
		
//		GetTvVertSection_Std(0, TRUE, FALSE, TRUE);
		return nElement;
	}
	else if(bArch && (nJijum>0 && nJijum<m_nQtyJigan))
	{
		
		
		long nSizeEle = pFEM->GetElementSize();
		long i=0; for(i=0; i<nSizeEle; i++)
		{
			CElement *pEle = NULL;
			pEle = pFEM->GetElement((USHORT)i);

			if(pEle->m_nFlag == ELEMENT_MIDWALL)
			{
				CJoint *pJntTemp = pFEM->GetJoint(pEle->m_nJEnd);
				if(pJntTemp->m_LongAttrib[LONG_0_JOINT_TYPE]==JOINT_ARCHRIB && pJntTemp->m_LongAttrib[LONG_1_JOINT_JIJUMNO]==nJijum)
				{
					nElement = i; break;
				}
			}
		}
//		GetTvVertSection_Std(0, TRUE, FALSE, TRUE);
		return nElement;
	}

	if(pFEM->GetJointSize() == 0) return nElement;

	CJoint *pJonitStt = GetJointSlabOnJijum(pFEM, nJijum, TRUE);

	double dHeightJijum = bLsd? GetHeightModelJijum(nJijum) : toM(GetHeightModelJijum(nJijum));
	if(m_nTypeBridge == BRIDGE_TYPE_TRAP2)
	{
		if(nJijum==1)dHeightJijum = bLsd? GetHeightModelJijum(0) :  toM(GetHeightModelJijum(0));
		else if(nJijum==m_nQtyJigan-1) dHeightJijum = bLsd? GetHeightModelJijum(m_nQtyJigan) : toM(GetHeightModelJijum(m_nQtyJigan)); 
	}

	double dDistRigid   = bLsd? GetDistRigidEndWall(nJijum, FALSE) : toM(GetDistRigidEndWall(nJijum, FALSE));
	if(m_nSelectRigidModeling == 0 && m_nSelectForceCheckPos==0)
	{
		// (ARCBRIDGE-2368) 강역 적용 안하고 최대 부재력 선택 했을때 부재 끝단을 적용한다.
		dDistRigid = 0.0;
	}

	double dHeightChcek = pJonitStt->m_vPointOrigin.z - dHeightJijum + dDistRigid;
	if(m_nSelectForceCheckPos==1 && bGirderCalc==FALSE && IsBoxType())
	{
		dHeightChcek = pJonitStt->m_vPointOrigin.z - dHeightJijum + (bLsd? m_dTS_Lower : toM(m_dTS_Lower))/2;
	}

	// ARCH교는 모델링 상부가 레벨이 아니므로 지점 마다의 El차를 고려해준다.
	// (40269) 아치고 종점에만 적용되겠지만 종점상단 절점 높이와 종점벽체 높이를 구했기 때문에 필요가 없어보인다.
	double dElDiff = 0;
// 	if(bArch)
// 	{
// 		CLineInfo *pLine = GetLineBase();
// 		double dElStt	 = pLine->GetElevationPlanEl(GetStationOnJijum(0)-m_dWS/2);
// 		double dDist	 = (nJijum==0)? (-m_dWS/2) : ((nJijum==m_nQtyJigan)? m_dWE/2 : 0);
// 		double dElCheck	 = pLine->GetElevationPlanEl(GetStationOnJijum(nJijum) + dDist);
// 		dElDiff	 = toM(dElStt - dElCheck);
// 	}

	// 기초연단에서의 거리를 더한다.
	if(bShearCheck)
		dHeightChcek += bLsd? dDistShearPos : toM(dDistShearPos);

	long nSizeElement = pFEM->GetElementSize();
	for(long nEle=0; nEle<nSizeElement; nEle++)
	{
		CElement *pEle = pFEM->GetElement((USHORT)nEle);
		CJoint   *pJnt = pFEM->GetJoint(pEle->m_nJEnd);
		if(pEle->m_nFlag==ELEMENT_UPPERSLAB) continue;
		if(pEle->m_nFlag==ELEMENT_LOWERSLAB) continue;
		if(pJnt->m_LongAttrib[1]!=nJijum) continue; // 지점번호

		//if(Compare(Round(pJnt->m_vPointOrigin.z, 3), Round(dHeightChcek-dElDiff, 3),"="))
		if(Compare(pJnt->m_vPointOrigin.z, dHeightChcek-dElDiff, "=", bLsd? 1 : .001))
		{
			nElement = nEle;
			dDistEnd = 0;
			break;
		}
		else if((m_nSelectRigidModeling == 2 || (bShearCheck && dDistShearPos > 0)) && pJnt->m_vPointOrigin.z < dHeightChcek-dElDiff) // End Offset으로 처리한 경우 부재사이에 검토점이 위치
		{
			CJoint *pJntStt = pFEM->GetJoint(pEle->m_nJStt);
			if(pJntStt && pJntStt->m_vPointOrigin.z > dHeightChcek-dElDiff)
			{
				dDistEnd = (bShearCheck && dDistShearPos > 0)? dHeightChcek - pJnt->m_vPointOrigin.z : 0;
				return nEle;
			}
		}
	}

//	GetTvVertSection_Std(0, TRUE, FALSE, TRUE);
	return nElement;
}

double CRcBridgeApp::GetMomentCorner(BOOL bSttWall, BOOL bUse, BOOL bOutSide, BOOL bUpperSlab, ETypeEnvComboLsd eEnvTypeLsd)
{
	CFEMManage *pFEM = &m_FemNormal;
	if(pFEM->IsOutputLoaded()==FALSE) return 0;

	double dMoment = 0;
	
	BOOL bLsd = m_pARcBridgeDataStd->IsLsdDesign();
	long nTypeDesign(COMBO_ULTIMATE);
	if(m_pARcBridgeDataStd->IsLsdDesign())
	{
		nTypeDesign = (eEnvTypeLsd == eEnvExtremeI || eEnvTypeLsd == eEnvExtremeII)? COMBO_EARTHQUAKE:COMBO_ULTIMATE;
	}
	else
	{
		nTypeDesign = bUse? COMBO_SERVICE:COMBO_ULTIMATE;
	}

	double dMomentSlab(0), dMomentWall(0);
	if(bLsd && (eEnvTypeLsd == eEnvExtremeI || eEnvTypeLsd == eEnvExtremeII))
	{
		dMomentSlab = GetOutValueSlabEQ(bSttWall? 0:m_nQtyJigan-1, bSttWall? CHECK_POS_LEFT:CHECK_POS_RIGHT,
			ELE_MOMENT2, bUpperSlab, eEnvTypeLsd);
		dMomentWall = GetOutValueWallEQ(bSttWall? 0:m_nQtyJigan, bUpperSlab ? CHECK_POS_TOP : CHECK_POS_BOTTOM,
			ELE_MOMENT2, FALSE, bOutSide,eEnvTypeLsd);
	}
	else
	{
		dMomentSlab = GetOutValueSlab(bSttWall? 0:m_nQtyJigan-1, bSttWall? CHECK_POS_LEFT:CHECK_POS_RIGHT,
			nTypeDesign, ELE_MOMENT2, bOutSide, FALSE, bUpperSlab,eEnvTypeLsd);
		dMomentWall = GetOutValueWall(bSttWall? 0:m_nQtyJigan, bUpperSlab ? CHECK_POS_TOP : CHECK_POS_BOTTOM,
			nTypeDesign, ELE_MOMENT2, FALSE, bOutSide,eEnvTypeLsd);
	}

	// 내측검토인 경우에 부모멘트 삭제... 
	if(bUpperSlab)
	{
		if(bOutSide==FALSE && dMomentSlab<0)  dMomentSlab = 0;
	}
	else
	{
		if(bOutSide==FALSE && dMomentSlab>0)  dMomentSlab = 0;
	}
	if(bOutSide==FALSE && dMomentWall<0)  dMomentWall = 0;

	dMoment = max(fabs(dMomentSlab), fabs(dMomentWall));
	
	
/*	
	CString szCombo = (bUse)? "ENVUSE MAX" : "ENVULT MAX";
	if(!bSttWall)
	{
		long nSizeEle = pFEM->GetElementSize();
		long i=0; for(i=0; i<nSizeEle; i++)
		{
			CElement *pEle = pFEM->GetElement(i);
			if(pEle->m_nFlag!=0) // ELEMENT_SLAB 0
			{
				nElement =  pEle->m_nIndex-1;
				break;
			}
		}
	}
	long nLoadCase  = pFEM->GetLoadCaseIndex(szCombo);	
	if(nLoadCase<0) return dMoment;
	CElement *pEle = pFEM->GetElement(nElement);
	CResultForceElement *pResultMax = pEle->GetResultForce(nLoadCase);
	CResultForceElement *pResultMin = pEle->GetResultForce(nLoadCase+1);
	long nPoint = bSttWall ? 0 : pResultMax->m_ResultForceElement.GetSize()-1;
	double dValueMax = pResultMax->m_ResultForceElement[nPoint]->m_dForceOnElement[ELE_MOMENT2];
	double dValueMin = pResultMin->m_ResultForceElement[nPoint]->m_dForceOnElement[ELE_MOMENT2];
	dMoment = max(fabs(dValueMax),fabs(dValueMin));
*/
	return dMoment;
}

double CRcBridgeApp::GetOutValueWallEQ(long nJijum, long nPos, long nTypeForce, BOOL bGirderCalc, BOOL bOutSide, ETypeEnvComboLsd eEnvTypeLsd, double dDist)
{
	double dEqRet(0);
	long nSizeEq = GetCountEarthQuakeModel();
	for(long ix=0; ix<nSizeEq; ++ix)
	{
		double dEQ1 = GetOutValueWall(nJijum,nPos,COMBO_EARTHQUAKE+ix,nTypeForce,bGirderCalc,bOutSide,eEnvTypeLsd, dDist);
		if(ix == 0)
			dEqRet = dEQ1;
		else
		{
			// 외측벽체의 모멘트
			if(nTypeForce==ELE_MOMENT2 && (nJijum==0 || nJijum==m_nQtyJigan))
			{
				if(nPos==CHECK_POS_TOP)		dEqRet = min(dEQ1,dEqRet);
				if(nPos==CHECK_POS_CENTER)	dEqRet = bOutSide ? min(dEQ1,dEqRet) : max(dEQ1,dEqRet);
				if(nPos==CHECK_POS_BOTTOM)	dEqRet = bOutSide ? min(dEQ1,dEqRet) : max(dEQ1,dEqRet);
			}
			else if(nTypeForce==ELE_SHEAR_2 || nTypeForce==ELE_AXFORCE || (nJijum>0 && nJijum<m_nQtyJigan))
			{
				// 전단력,축력,중간벽체(ALL)

				if((nJijum==0 || nJijum==m_nQtyJigan) && nTypeForce==ELE_AXFORCE && nPos==CHECK_POS_CENTER)
				{
					dEqRet = fabs(max(dEQ1,dEqRet));
				}
				else
				{
					dEqRet = max(fabs(dEQ1),fabs(dEqRet));
				}
			}
			else
			{
				dEqRet = max(dEQ1,dEqRet);
			}
		}
	}

	return dEqRet;
}

double CRcBridgeApp::GetOutValueWall(long nJijum, long nPos, long nTypeDesign, long nTypeForce, BOOL bGirderCalc, BOOL bOutSide, ETypeEnvComboLsd eEnvTypeLsd, double dDist)
{
	if(nTypeForce!=ELE_MOMENT2 && nTypeForce!=ELE_SHEAR_2 && nTypeForce!=ELE_AXFORCE) return 0;
//	부재 중앙부 전단력은 무조건 0으로 했는데 값 옵션 적용 하기로...
	if(!m_bCalcShearCenter && nPos==CHECK_POS_CENTER && nTypeForce==ELE_SHEAR_2) return 0;
	if(IsUseModelCombo(nTypeDesign) == FALSE) return 0;
	
	CFEMManage *pFEM = GetFemManage(nTypeDesign);
	if(pFEM == NULL || pFEM->IsOutputLoaded()==FALSE) return 0;

	double dOutputMax = 0;
	double dOutputMin = 0;

	BOOL bChange = FALSE;
	BOOL bArch = m_nTypeBridge == BRIDGE_TYPE_ARCH;
	long nTypeJongOld = 0;
	if(m_nTypeJongdanForRebar != (bArch? 1 : 0))
	{
		nTypeJongOld = m_nTypeJongdanForRebar;
		GetTvVertSection_Std(0, TRUE, FALSE, FALSE, bArch? 1 : 0, FALSE, TRUE);
		bChange = TRUE;
	}

	BOOL bShearFoce = (nTypeForce == ELE_SHEAR_2)? TRUE : FALSE;
	double dDistStt(0), dDistEnd(0);
	long nIdxEleStt = GetNumberElementWallSttCheck(nJijum,nTypeDesign,bShearFoce,bGirderCalc, dDist, dDistStt);
	long nIdxEleEnd = GetNumberElementWallEndCheck(nJijum,nTypeDesign,bShearFoce,bGirderCalc, dDist, dDistEnd);
	if(nIdxEleStt < 0 || nIdxEleEnd < 0) return 0;

	EPartOfBridge ePart(ePartInWall);
	if(nJijum == 0)
		ePart = ePartOutWallStt;
	else if(nJijum == m_nQtyJigan)
		ePart = ePartOutWallEnd;

	CString szCombo = GetStringComboEnv(nTypeDesign, eEnvTypeLsd, ePart, nPos);

	long nLoadCase  = pFEM->GetLoadCaseIndex(szCombo);	
	if(nLoadCase<0) 
	{
		if(bChange)
		{
			m_nTypeJongdanForRebar = nTypeJongOld;
			m_nTypeJongdanForRebarOld = nTypeJongOld;
			GetTvVertSection_Std(0, TRUE, FALSE, FALSE, nTypeJongOld, FALSE, TRUE);
		}

		return dOutputMax;
	}

	CElement *pEle = NULL;
	CResultForceElement *pResultMax = NULL;
	CResultForceElement *pResultMin = NULL;
	// 하나의 부재에 대하여 부재력을 산출한다...
	if(nPos==CHECK_POS_TOP || nPos==CHECK_POS_BOTTOM)
	{
		long nIdxEle = (nPos==0) ? nIdxEleStt : nIdxEleEnd;
		BOOL bSttPos = (nPos==0) ? TRUE : FALSE;
		pEle = pFEM->GetElement((USHORT)nIdxEle);
		if(pEle == NULL) return dOutputMax;
		double dDist = (nPos==0) ? dDistStt : dDistEnd;

		if(bShearFoce && dDist > 0)
		{
			// DistShear가 부재길이보다 큰경우 처리해준다.
			for(long ix=1; ix<(nIdxEleEnd-nIdxEleStt); ++ix)
			{
				double dLenEle = pFEM->GetLengthElement(pEle);
				if(dDist >= dLenEle)
				{
					pEle = pFEM->GetElement((USHORT)(nIdxEle + ix * (nPos==CHECK_POS_LEFT? 1 : -1)));
					dDist -= dLenEle;
					if(pEle == NULL) return dOutputMax;
				}
				else
				{
					break;
				}
			}
		}

		pResultMax = pEle->GetResultForce(nLoadCase);
		pResultMin = pEle->GetResultForce(nLoadCase+1);
		if(pResultMax == NULL || pResultMin == NULL) return dOutputMax;

		if(bShearFoce && dDist > 0)
		{
			// 부재 끝에서 떨어진 위치에서 보간법으로 계산을 해준다.
			long nSizePoint = pResultMax->m_ResultForceElement.GetSize()-1;
			double dLenUnit = pFEM->GetLengthElement(pEle) / nSizePoint;
			long nPoint = (long)(dDist / dLenUnit);
			long nPoint1 = (nPos == 0) ? nPoint : nSizePoint-nPoint;
			long nPoint2 = (nPos == 0) ? nPoint1+1 : nPoint1-1;
			double dDistExt = dDist - dLenUnit * nPoint;
			double dValue1 = pResultMax->m_ResultForceElement[nPoint1]->m_dForceOnElement[nTypeForce];
			double dValue2 = pResultMax->m_ResultForceElement[nPoint2]->m_dForceOnElement[nTypeForce];
			dOutputMax  = dValue1 + (dValue2-dValue1) * (dDistExt / dLenUnit);

			dValue1 = pResultMin->m_ResultForceElement[nPoint1]->m_dForceOnElement[nTypeForce];
			dValue2 = pResultMin->m_ResultForceElement[nPoint2]->m_dForceOnElement[nTypeForce];
			dOutputMin  = dValue1 + (dValue2-dValue1) * (dDistExt / dLenUnit);
		}
		else
		{
			long nPoint  = bSttPos ? 0 : pResultMax->m_ResultForceElement.GetSize()-1;
			dOutputMax   = pResultMax->m_ResultForceElement[nPoint]->m_dForceOnElement[nTypeForce];
			dOutputMin   = pResultMin->m_ResultForceElement[nPoint]->m_dForceOnElement[nTypeForce];
		}
	}
	else
	{
		// 벽체중앙부인 경우 벽체의 중앙위치를 찾아서 부재력을 구한다... (2005.06.13)
		long nEleMid=-1,nPointMid=-1;
		GeMidElementAndPointOnWall(nTypeDesign,nJijum,nEleMid,nPointMid, eEnvTypeLsd);
		pEle = pFEM->GetElement((USHORT)nEleMid);
		if(pEle == NULL) return dOutputMax;

		pResultMax  = pEle->GetResultForce(nLoadCase);
		pResultMin  = pEle->GetResultForce(nLoadCase+1);
		dOutputMax  = pResultMax->m_ResultForceElement[nPointMid]->m_dForceOnElement[nTypeForce];
		dOutputMin  = pResultMin->m_ResultForceElement[nPointMid]->m_dForceOnElement[nTypeForce];
	}

	double dValueReturn = dOutputMax;
	// 외측벽체의 모멘트
	if(nTypeForce==ELE_MOMENT2 && (nJijum==0 || nJijum==m_nQtyJigan))
	{
		// max(,),min(,)적용은 마이다스의 경우 (+),(-)의 구분이 없이 출력됨
		// ex) (-)모멘트만 있는 경우 max,min 모두 (-)로 출력.....
		if(nPos==CHECK_POS_TOP)		dValueReturn = bOutSide ? MIN(dOutputMin,0) : MAX(dOutputMax,0);
		if(nPos==CHECK_POS_CENTER)	dValueReturn = bOutSide ? MIN(dOutputMin,0) : MAX(dOutputMax,0);
		if(nPos==CHECK_POS_BOTTOM)	dValueReturn = bOutSide ? MIN(dOutputMin,0) : MAX(dOutputMax,0);
	}
	// 전단력,축력,중간벽체(ALL)
	if(nTypeForce==ELE_SHEAR_2 || nTypeForce==ELE_AXFORCE || (nJijum>0 && nJijum<m_nQtyJigan))
	{
		// (ARCBRIDGE-1719) 외측벽체를 벽체로 검토 할때는 적용하지 않는다.
		if((nJijum==0 || nJijum==m_nQtyJigan) && nTypeForce==ELE_AXFORCE && nPos==CHECK_POS_CENTER && m_nSelectWallSectionDesign != 1)
		{
			dValueReturn = fabs(dOutputMax);
		}
		else
		{
			dValueReturn = MAX(fabs(dOutputMax),fabs(dOutputMin));
		}
	}

	if(bChange)
	{
		m_nTypeJongdanForRebar = nTypeJongOld;
		m_nTypeJongdanForRebarOld = nTypeJongOld;
		GetTvVertSection_Std(0, TRUE, FALSE, FALSE, nTypeJongOld, FALSE, TRUE);
	}

	BOOL bLsd = m_pARcBridgeDataStd->IsLsdDesign();
	double Station   = GetStationOnJijum(0,0);
	double SlabWidth = bLsd? GetWidthSlabAct(Station,CDPoint(0,1)) : toM(GetWidthSlabAct(Station,CDPoint(0,1)));	// 슬래브폭원(교량폭,벽체폭)
	double dDivide = (nTypeDesign>=COMBO_EARTHQUAKE) ? SlabWidth : 1.00;
	return dValueReturn/dDivide;
}

// 기초와 보링데이타만으로 터파기 라인 구하기.
// BOOL bIncludeUpper : 윗 뚜껑 덮기
// BOOL bCrossSection : 횡단면
// BOOL bMergeLower : 왼쪽이격, 기초폭, 오른쪽이격 선을 하나로 합쳐서 하면으로 만든다
void CRcBridgeApp::GetTvTogongLine(CTwinVectorArray &tvArr, CFootingApp *pFooting, CBoringDataEx *pExBoring, BOOL bIncludeUpper, BOOL bCrossSection, BOOL bMergeLower, BOOL bForDraw, long nDeductLeft, long nJijum)
{
	tvArr.RemoveAll();
	if(!pFooting) return;
	if(!pExBoring) return;
	pFooting->m_pExBoring	= pExBoring;

	double dExtWidCrossSec = 0;
	if(bCrossSection && m_bIsExp && nJijum > -1 && bForDraw == FALSE)
	{
		if(nJijum == 0 || nJijum == m_nQtyJigan)
		{
			BOOL bStt = nJijum == 0? TRUE : FALSE;

			GetTvWingWallPlane(bStt);

			CTwinVector tvSlab;
			m_tvArrPlaneWingWall.GetTvByInfo("슬래브끝단외측", tvSlab);		

			dExtWidCrossSec = tvSlab.GetLength() > 0? tvSlab.GetLength() : 0;					
		}		
	}
	
	pFooting->GetTvTogongLine(tvArr, bIncludeUpper, bCrossSection, bMergeLower, bForDraw, nDeductLeft, TRUE, dExtWidCrossSec);	
}

// 소단라인을 참조해서 성토라인을 구한다
// bBoho 보호블럭 라인을 구함(성토에서 600만큼 더 들어간 선)
void CRcBridgeApp::GetTvSungtoLine(CTwinVectorArray &tvArr, BOOL bStt, BOOL bLeft, BOOL bBoho)
{
	CWingWall *pWing = GetWingWall(bStt,bLeft);
	tvArr.RemoveAll();
	

	CTwinVectorArray tvArrOldVert;
	tvArrOldVert	= m_tvArrVertSection;
	int se = bStt ? -1 : 1;
	int nLeft = bLeft ? -1*se : 1*se;	// 시점일 경우 교량 진행 방향의 좌우와 날개벽의 좌우가 반대임.
	GetTvVertSection(nLeft, TRUE, FALSE);

	CTwinVectorArray tvArrSodan;
	GetTvWingSlopeLine(tvArrSodan, bStt, bLeft, frM(pWing->m_dElSodanBottom));

	// 시점보다 왼쪽에 있거나 
	// 종점보다 오른쪽에 있으면 
	// 성토수량이 완전히 없는 것이다.
	double dSta	= GetXySectionWingWallStt(bStt, TRUE).x;//GetStationOnJijum(bStt ? 0 : m_nQtyJigan, nLeft, FALSE);
	CDRect rect	= tvArrSodan.GetRect();
	if(bStt && dSta > rect.right)
		return;
	if(!bStt && dSta < rect.left)
		return;


	// 외측벽체를 기준으로 잘라줘야 됨
	CRebarPlacing rb;
	rb.TrimTvArrayByLine(tvArrSodan, CDPoint(dSta, rect.bottom-100), CDPoint(dSta, rect.top+100), bStt ? TRUE : FALSE, TRUE, FALSE);

	// 보호블럭인 경우 600 더 들어감
	long nSize	= tvArrSodan.GetSize();
	if(bBoho && nSize > 0)
	{
		const double dDepth	= 600;
		CTwinVector tv	= tvArrSodan.GetAt(nSize-1);
		CDPoint xyMatch(0, 0);
		if(GetXyMatchLineAndLine(tv.m_v1, tv.GetXyDir(), CDPoint(tv.m_v2.x, tv.m_v2.y - dDepth), CDPoint(1, 0), xyMatch))
		{
			tv.m_v2	= xyMatch;
			tvArrSodan.SetAt(nSize-1, tv);
		}
	}

	tvArr	= tvArrSodan;

	m_tvArrVertSection	= tvArrOldVert;
}

// long nForUse : 0 원래 뒷채움 선, 1 : 되메우기용, 2 : 노상용, 3 : 노체용
// 1(되메우기용) : 하부에서 지반선까지
// 2(노상용) : 노상영역(하부한계 지반선)
// 3(노체용) : 노체영역(하부한계 지반선)
// 4(뒷채움수량산출용) : 원래 뒷채움선에서 포장부분 뺀거.
CTwinVector CRcBridgeApp::GetTvBackFill(BOOL bStt, long nForUse, BOOL bBack)
{
	CTwinVector tv;

	CLineInfoApp* pLine = NULL;
	if(IsBoxType())
		pLine = m_pLineApp;
	else
		pLine = m_pARcBridgeDataStd->GetLineApp();
	if(!pLine) return tv;

	CFootingApp* pFooting = NULL;
	//박스교의 경우 시점기초가 종점기초 역활까지 한다.
	if(m_nTypeBridge != BRIDGE_TYPE_BOX)
		pFooting	= bStt ? &m_footingStt : &m_footingEnd;
	else 
		pFooting	= &m_footingBox;
	
	if(!pFooting) return tv;

	double dx, dy = 0, dh, dv, dEl, dElEarth;
	BOOL bFindEL	= TRUE;
	if(bStt)
	{
		if(bBack)
		{
			CTwinVector tv	= m_tvArrVertSection.GetTvByInfo(_T("시점벽체좌측면"));
			dx = tv.m_v1.x - pFooting->m_dDistOffsetBackFill;
		}
		else
		{
			dx = pFooting->m_dSta - pFooting->m_dML - pFooting->m_dDistOffsetBackFill;
			if(m_nTypeBridge == BRIDGE_TYPE_BOX) //BOX일경우 좌측면에서 m_dDistOffsetBackFill 만큼만 뺴주면됨.
			{
				tv	= m_tvArrVertSection.GetTvByInfo(_T("시점벽체좌측면"));
				dx = tv.m_v1.x - pFooting->m_dDistOffsetBackFill;

			}
		}

		dy = pFooting->m_dEL;
		dEl = pLine->GetElevationPlanEl(m_dStationBridgeStt);

		dh = dEl - dy - GetThickPave(FALSE);
		
		//함형경우 뒷채움선이 부상방지판과 겹치는지 확인한다.
		if(m_nTypeBridge == BRIDGE_TYPE_BOX && m_bExistProtectionRiseFooting)
		{
			CDPoint xyResult;
			CDPoint xyDb = CDPoint(dx, dy);
			CTwinVectorArray tvArr;

			GetTvPRF_VertSection(tvArr, bStt, 0, m_bSlopeLowerSlab);
			CTwinVector tvPRF = tvArr.GetTvByInfo(_T("시점부상방지저판상면"));

			BOOL bMatch = GetXyMatchSegAndLine(tvPRF.m_v1, tvPRF.m_v2, xyDb, CDPoint(0,1), xyResult);
			if(bMatch)
			{
				dy = xyResult.y;
				dh = dEl - dy - GetThickPave(FALSE);
			}
		}
		dv = dh * pFooting->m_dSlopeBackFill;

		tv.m_v1	= CVector(dx, dy, 0);
		tv.m_v2	= CVector(dx-dv, dy+dh, 0);

		bFindEL	= pLine->GetElevationErathOnStation(m_dStationBridgeStt, dElEarth);
	}
	else
	{
		if(bBack)
		{
			CTwinVector tv	= m_tvArrVertSection.GetTvByInfo(_T("종점벽체우측면"));
			dx = tv.m_v1.x + pFooting->m_dDistOffsetBackFill;
		}			
		else
		{
			dx = pFooting->m_dSta + pFooting->m_dMR + pFooting->m_dDistOffsetBackFill;
			if(m_nTypeBridge == BRIDGE_TYPE_BOX) //BOX일경우 우측면에서 m_dDistOffsetBackFill 만큼만 더해주면됨.
			{
				tv	= m_tvArrVertSection.GetTvByInfo(_T("종점벽체우측면"));
				dx = tv.m_v1.x + pFooting->m_dDistOffsetBackFill;
			}
		}
		dy = pFooting->m_dEL;
		dEl = pLine->GetElevationPlanEl(m_dStationBridgeStt+m_dLengthBridge);
		dh = dEl - dy - GetThickPave(FALSE);

		//함형경우 뒷채움선이 부상방지판과 겹치는지 확인한다.
		if(m_nTypeBridge == BRIDGE_TYPE_BOX && m_bExistProtectionRiseFooting)
		{
			CDPoint xyResult;
			CDPoint xyDb = CDPoint(dx, dy);
			CTwinVectorArray tvArr;

			GetTvPRF_VertSection(tvArr, bStt, 0, m_bSlopeLowerSlab);
			CTwinVector tvPRF = tvArr.GetTvByInfo(_T("종점부상방지저판상면"));

			BOOL bMatch = GetXyMatchSegAndLine(tvPRF.m_v1, tvPRF.m_v2, xyDb, CDPoint(0,1), xyResult);
			if(bMatch)
			{
				dy = xyResult.y;
				dh = dEl - dy - GetThickPave(FALSE);
			}
		}

		dv = dh * pFooting->m_dSlopeBackFill;
		tv.m_v1	= CVector(dx, dy, 0);
		tv.m_v2	= CVector(dx+dv, dy+dh, 0);

		bFindEL	= pLine->GetElevationErathOnStation(m_dStationBridgeStt+m_dLengthBridge, dElEarth);
	}

	// 용도에 따라서 위 아래를 절단해줌
	// 1. 되메우기용이면 상단을 지반선기준으로 잘라준다.
	//    단, 지반이 뒷채움하부보다 아래에 있으면 뒷채움선을 길이 0으로 만들고, 
	//    지반이 뒷채움 상부보다 위에 있으면 뒷채움선은 그대로 둔다.
	if(nForUse == 1)	
	{
		long nJ	= bStt ? 0 : m_nQtyJigan;
		CTwinVectorArray tvArrTogong;
		GetTvTogongLine(tvArrTogong, nJ, FALSE, FALSE, TRUE);
		CTwinVector tvUpper;
		tvArrTogong.GetTvByInfo("상면", tvUpper);

		CDPoint xyMatch(0, 0);
		GetXyMatchLineAndLine(tv.m_v1, tv.GetXyDir(), tvUpper.m_v1, CDPoint(1, 0), xyMatch);

		if(tv.m_v1.y > xyMatch.y) tv.m_v2 = tv.m_v1;
		else if(tv.m_v2.y < xyMatch.y) tv	= tv;
		else tv.m_v2	= xyMatch;
	}
	// 2. 노상이나 노체용이면 노상 노체 영역에 맞게 자른뒤 하부를 지반기준으로 맞춘다.
	//    단, 지반이 자른 영역 하부보다 아래에 있으면 그냥 두고, 
	//    지반이 자른 영역 상부보다 위에 있으면 뒷채움선 길이를 0으로 만든다.
	else if(nForUse == 2 || nForUse == 3)
	{
		// 노상이나 노체의 상하부 Y 위치 
		CDPoint xyMatchU(0, 0), xyMatchL(0, 0);
		double dYUpper	= GetTvBackFillHorLine(bStt, nForUse-1).m_v1.y;
		double dYLower	= dYUpper;
		if(nForUse == 2)
			dYLower	= GetTvBackFillHorLine(bStt, 2).m_v1.y;
		else 
		{
			CTwinVectorArray tvArrTypagi;
			CTwinVector tvUpper;
			GetTvTogongLine(tvArrTypagi, bStt ? 0 : m_nQtyJigan, FALSE, TRUE, TRUE);
			if(tvArrTypagi.GetTvByInfo("상면", tvUpper))
			{
				dYLower	= tvUpper.m_v1.y;
			}
		}

		// 상하부 위치에 맞게 뒷채움선 자름
		GetXyMatchLineAndLine(tv.m_v1, tv.GetXyDir(), CDPoint(0, dYUpper), CDPoint(1, 0), xyMatchU);
		GetXyMatchLineAndLine(tv.m_v1, tv.GetXyDir(), CDPoint(0, dYLower), CDPoint(1, 0), xyMatchL);

		tv.m_v1	= xyMatchL;
		tv.m_v2	= xyMatchU;

		// 지반선 고려해서 뒷채움선 수정
		if(bFindEL)
		{
			BOOL bUseEL = Compare(pFooting->m_dEL_DiggingFirst, dElEarth, _T("="),0.1)? TRUE : FALSE;
			double dELMatch = bUseEL==FALSE? pFooting->m_dEL_DiggingFirst : dElEarth;

			CDPoint xyMatch(0, 0);
			GetXyMatchLineAndLine(tv.m_v1, tv.GetXyDir(), CDPoint(0, dELMatch), CDPoint(1, 0), xyMatch);
			if(Compare(tv.m_v1.y, xyMatch.y, ">")) tv = tv;
			else if(Compare(tv.m_v2.y, xyMatch.y, "<")) tv.m_v1 = tv.m_v2;
			else tv.m_v1 = xyMatch;
		}

	}
	else if(nForUse == 4)
	{
		GetXyMatchLineAndLine(tv.m_v1, tv.GetXyDir(), CDPoint(tv.m_v2.x, tv.m_v2.y-pFooting->m_dHeightPavement), CDPoint(1, 0), tv.m_v2);
	}

	tv.m_sInfo	= "뒷채움선";

	return tv;
}

// 뒷채움 수량중 날개벽 공제해야 될 영역만 골라냄
// long nForUse : 0 원래 뒷채움 선, 1 : 되메우기용, 2 : 노상용, 3 : 노체용
// 1(되메우기용) : 하부에서 지반선까지
// 2(노상용) : 노상영역(하부한계 지반선)
// 3(노체용) : 노체영역(하부한계 지반선)
void CRcBridgeApp::GetTvBackFillDeductOfWing(CTwinVectorArray &tvArr, BOOL bStt, BOOL bLeft, long nForUse)
{
	tvArr.RemoveAll();
	if(!IsWingWall(bStt, bLeft)) return;

	// 뒷채움수량 공제용인데
	// 날개벽이 측면에 부착되어 있으면 날개벽을 공제할 필요가 없다.
	CWingWall *pWing	= GetWingWall(bStt, bLeft);
	if(pWing->m_nAttachPos == 1) return;

	// 날개벽과 공제토 라인 구함
	CTwinVectorArray tvArrWing;
	GetTvWingWallFront(bStt, bLeft, 1, tvArrWing, TRUE, FALSE, TRUE, TRUE);

	// 뒷채움선으로 뒷채움 영역 구함(사다리꼴)
	CTwinVector tvBackFill	= GetTvBackFill(bStt, nForUse, TRUE);
	CTwinVector tvInner	= m_tvArrVertSection.GetTvByInfo(bStt ? "시점벽체좌측면" : "종점벽체우측면");
	double dXOrg	= tvInner.m_v1.x;//bStt ? m_dStationBridgeStt : m_dStationBridgeStt+m_dLengthBridge;

	// 교집합 구하는 함수가 선이 겹치는 경우에 대해서 정상적으로 찾지 못 하므로 100만큼 여유를 줘서 크게 만든다.
	tvBackFill.m_v1	= tvBackFill.m_v1 + tvBackFill.GetXyDir(TRUE) * 100;

	CTwinVectorArray tvArrBackFill;
	CTwinVector tv;
	tv.m_v1	= tvBackFill.m_v1;
	tv.m_v2	= CVector(dXOrg, tv.m_v1.y, 0);
	tv.m_v2 = tv.m_v2 + tv.GetXyDir() * 100;
	tv.m_sInfo	= "하면";
	tvArrBackFill.Add(tv);

	tv.m_v1	= tv.m_v2;
	tv.m_v2	= CVector(tv.m_v1.x, tv.m_v1.y + tvBackFill.GetVerLength(), 0);
	tv.m_sInfo	= "벽체면";
	tvArrBackFill.Add(tv);

	tv.m_v1	= tv.m_v2;
	tv.m_v2	= tvBackFill.m_v2;
	tv.m_sInfo	= "상면";
	tvArrBackFill.Add(tv);

	tvBackFill.m_sInfo	= "뒷채움선";
	tvArrBackFill.Add(tvBackFill);

	// 날개벽과의 교집합 영역을 구함
	CRebarPlacing rb;
	rb.SubTractTvArrAndTvArr(tvArrWing, tvArrBackFill, tvArr);
	tvArr.Sort();
}

void CRcBridgeApp::GetTvReFillDeductOfWing(CTwinVectorArray &tvArr, BOOL bStt, BOOL bLeft)
{
	tvArr.RemoveAll();
	if(!IsWingWall(bStt, bLeft)) return;

	// 날개벽과 공제토 라인 구함
	CTwinVectorArray tvArrWing;
	GetTvWingWallFront(bStt, bLeft, 1, tvArrWing, TRUE, FALSE, TRUE, TRUE);

	long nJ = bStt ? 0 : m_nQtyJigan;
	CTwinVectorArray tvArrTypagi;
	GetTvTogongLine(tvArrTypagi, nJ, FALSE, TRUE, TRUE);

	// 날개벽과의 교집합 영역을 구함
	CRebarPlacing rb;
	rb.SubTractTvArrAndTvArr(tvArrWing, tvArrTypagi, tvArr);
	tvArr.Sort();
}

void CRcBridgeApp::GetTvElEarthLine(CTwinVectorArray &tvArr, double dStationStt, double dStationEnd, BOOL bBridgeZone)
{
	tvArr.RemoveAll();
	CLineInfoApp *pLine = m_pARcBridgeDataStd->GetLineApp();
	if(!pLine) return;

	long nLine	= 0;
	CLineInfo *pL = pLine->GetLine(nLine);
	double dStaSttJongDan = dStationStt;
	double dStaEndJongDan = dStationEnd;
	if(bBridgeZone)
	{
		double dMarginLeft = IsOutWall(TRUE) ? m_footingStt.m_dFWL + m_dWS + 1000 : 1000;
		double dMarginRight = IsOutWall(FALSE) ? m_footingEnd.m_dFWR + m_dWE + 1000 : 1000;
		dStaSttJongDan = m_dStationBridgeStt - dMarginLeft;
		dStaEndJongDan = m_dStationBridgeStt + m_dLengthBridge + dMarginRight;
	}

	double dEleStt(0), dEleEnd(0);
	long nSizeV	= pL->m_VEarthLine.GetSize();
	if(nSizeV > 0)
	{
		// 그려야할 시종점에 지반정보가 없다면 지반정보 있는 만큼만 골라서 그리자
		if(!pL->GetElevationErathOnStation(dStaSttJongDan,dEleStt))
			dStaSttJongDan	= pL->m_VEarthLine.GetAt(0)->dSta;
		
		// 그려야할 시종점에 지반정보가 없다면 지반정보 있는 만큼만 골라서 그리자
		if(!pL->GetElevationErathOnStation(dStaEndJongDan,dEleEnd))
			dStaEndJongDan	= pL->m_VEarthLine.GetAt(nSizeV-1)->dSta-1;	// -1한 이유 : 마지막 sta와 같으니까 없는걸로 판단하는거 같음.
	}
	
	CTwinVector tv;
	if(pL->m_VEarthLine.GetSize())
	{			
		if(pLine->GetLine(nLine)->GetElevationErathOnStation(dStaSttJongDan,dEleStt) &&
		   pLine->GetLine(nLine)->GetElevationErathOnStation(dStaEndJongDan,dEleEnd))
		{
			tv.m_v1	= CVector(dStaSttJongDan, dEleStt, 0);

			long i = 0; for(i = 0; i < pL->m_VEarthLine.GetSize(); i++)	
			{
				if(pL->m_VEarthLine.GetAt(i)->dSta < dStaSttJongDan) continue;
				if(pL->m_VEarthLine.GetAt(i)->dSta > dStaEndJongDan) break;
				tv.m_v2	= CVector(pL->m_VEarthLine.GetAt(i)->dSta, pL->m_VEarthLine.GetAt(i)->dEle, 0);
				tvArr.Add(tv);

				tv.m_v1	= tv.m_v2;
			}

			tv.m_v2	= CVector(dStaEndJongDan, dEleEnd, 0);
			tvArr.Add(tv);
		}
	}
}

// 뒷채움영역의 수평 구분선 리턴
// nType - 0 : 포장층 상단 (최상단)
// nType - 1 : 노상 상단 
// nType - 2 : 노체 상단
CTwinVector CRcBridgeApp::GetTvBackFillHorLine(BOOL bStt, long nType)
{
	CTwinVector tv;

	CLineInfoApp *pLine = (m_bBridgeForBM)? m_pLineApp : m_pARcBridgeDataStd->GetLineApp();
	if(!pLine) return tv;
	
	double dx(0), dy(0);
	double dLen	= bStt ? -3000 : 3000;
	double dGap	= bStt ? -(500+m_dBWS) : (500+m_dBWE);
	CFootingApp *pFooting	= NULL;
	double se = 1;

	if(m_nTypeBridge == BRIDGE_TYPE_BOX)
		pFooting = &m_footingBox;
	else if(bStt)
		pFooting = &m_footingStt;
	else
	{
		pFooting = &m_footingEnd;
		se = -1;
	}

	double dSta = (bStt)? m_dStationBridgeStt : (m_dStationBridgeStt+m_dLengthBridge);
	CTwinVector tvBackFill	= (bStt)? GetTvBackFill(TRUE, FALSE ? 4 : 0) : GetTvBackFill(FALSE);
	
	dx = tvBackFill.m_v1.x + (pFooting->m_dDistOffsetBackFill * se);
	dy = (m_bUnderGroundRahmen)?  pLine->GetElevationPlanEl(dSta) : pLine->GetElevationPlanEl(dSta) - GetThickPave(FALSE);
	dx += dGap;

	if(nType == 0)
	{
		tv.m_v1	= CVector(dx, dy, 0);
		tv.m_v2	= CVector(dx+dLen, tv.m_v1.y, 0);
	}
	else if(nType == 1)
	{
		tv.m_v1	= CVector(dx, dy-pFooting->m_dHeightPavement, 0);
		tv.m_v2	= CVector(dx+dLen, tv.m_v1.y, 0);
	}
	else if(nType == 2)
	{
		tv.m_v1	= CVector(dx, dy-pFooting->m_dHeightPavement-pFooting->m_dHeightNosang, 0);
		tv.m_v2	= CVector(dx+dLen, tv.m_v1.y, 0);
	}

	return tv;
}
//실제 횡단 개수, 입력한 횡단개수에서 선형중심이 슬래브 사이(좌측 슬래브 끝단과 우측 슬래브 끝단 사이)에 있을 경우 1개 더 추가됨.
//<param name="bRoadNotSlab">TRUE=지중라멘의 상부 노면(도로면, 철도면)이며 슬래브면(교면)아님
//							FALSE=일반교량, 지중라멘의 슬래브면(교면)</param>
long CRcBridgeApp::GetQtyHDanAct(double staCR, CDPoint vAng, BOOL bRoadNotSlab, BOOL bExp)
{
	double width		= bExp ? GetWidthSlabActByGagak(staCR, CDPoint(0, 1),0,bRoadNotSlab) : GetWidthSlabAct(staCR, CDPoint(0, 1), 0,FALSE,FALSE,bRoadNotSlab);
	double disSlabLeft	= bExp ? GetDisSlabLeftActByGagak(staCR, CDPoint(0,1),bRoadNotSlab) : GetDisSlabLeftAct(staCR, CDPoint(0, 1),bRoadNotSlab);
	

	CDPoint vAngSerch = bExp ? vAng : CDPoint(0,1);
	// bExp인 경우는 실제 폭원으로 계산
	if(Compare(disSlabLeft, 0.0, "<", DIST_MIN_CROSS_CENTER) && Compare((disSlabLeft + width), 0.0, ">", DIST_MIN_CROSS_CENTER))	//선형중심이 슬래브 사이에 있을 경우
	{
		// 단, 횡단이 선형중심에 정확히 걸리도록 입력되어 있다면 1개 더 추가되면 안된다.
		long nQtyHDan	= GetQtyHDan(bRoadNotSlab);
		BOOL bAdd = TRUE;
		double dDistLeft = bExp ? GetDisSlabLeftActByGagak(staCR, vAng, bRoadNotSlab) : GetDisSlabLeftAct(staCR, CDPoint(0, 1), bRoadNotSlab);
		for(long i = 0; i < nQtyHDan; i++)
		{
			double dLengthHDanAct = 0;
			dLengthHDanAct = GetLengthHDanAct(staCR, vAngSerch, i, bRoadNotSlab, bExp, vAngSerch);
			dDistLeft += dLengthHDanAct;
			if(Compare(dDistLeft, 0.0, "=", DIST_MIN_CROSS_CENTER))
				bAdd	= FALSE;
		}
		return bAdd ? GetQtyHDan(bRoadNotSlab)+1 : GetQtyHDan(bRoadNotSlab); 
	}

	return GetQtyHDan(bRoadNotSlab);
	
}


//입력 : staCR	선형중심 스테이션
//		 nQtyHDan	횡단 슬래브 제원입력에서의 번호, 해당 번호에 레벨 여부, 방호벽 종류, 가변구간 여부가 있음.
//기능 : 입력에서의 번호로 슬래브 상면 절점의 번호를 구함.
//		 실제 슬래브 상면을 그리거나, Elevation, 높이관련 경우에 사용
//<param name="bRoadNotSlab">TRUE=지중라멘의 상부 노면(도로면, 철도면)이며 슬래브면(교면)아님
//							FALSE=일반교량, 지중라멘의 슬래브면(교면)</param>
long CRcBridgeApp::GetNumSlabNodeAct(double staCR, CDPoint vAng, long nHDan, BOOL bRoadNotSlab, BOOL bExp)
{
	CDPoint vAngSerch = bExp ? vAng : CDPoint(0,1);
	double width		= bExp ? GetWidthSlabActByGagak(staCR, vAngSerch, 0) : GetWidthSlabAct(staCR, vAngSerch,0,FALSE,FALSE,bRoadNotSlab);
	double disSlabLeft	= bExp ? GetDisSlabLeftActByGagak(staCR, vAngSerch, bRoadNotSlab) : GetDisSlabLeftAct(staCR, vAngSerch,bRoadNotSlab);
	if(Compare(disSlabLeft, 0.0, ">=", DIST_MIN_CROSS_CENTER) || Compare(disSlabLeft + width, 0.0, "<=", DIST_MIN_CROSS_CENTER))	//선형중심이 슬래브 사이에 없을 경우
	{
		return nHDan; 
	}

	for(long nHDanAct = 0; nHDanAct<nHDan+1; nHDanAct++)
	{
		double dLengthHDan = GetLengthHDanAct(staCR, vAngSerch, nHDanAct,bRoadNotSlab, bExp);	//선형중심 꺽임 길이가 적용안된 길이 계산, 입력 횡단개소수 적용 nHDanAct
		if(Compare(disSlabLeft, 0.0, "<", DIST_MIN_CROSS_CENTER) && Compare(disSlabLeft + dLengthHDan, 0.0, ">", DIST_MIN_CROSS_CENTER))
		{
			if(nHDan > nHDanAct)
				return nHDan + 1;
		}
		disSlabLeft += dLengthHDan;
	}
	return nHDan;
}
void CRcBridgeApp::GetTvPostWing(CTwinVectorArray &tvArr, BOOL bStt, BOOL bLeft, long nTypeBaseLine)
{
	tvArr.RemoveAll();
	if(!IsWingWall(bStt, bLeft)) return;

	CTwinVectorArray tvArrWing;
	GetTvWingWallFront(bStt, bLeft, 1, tvArrWing, FALSE, FALSE, TRUE, FALSE);
	
	CFootingApp *pFooting	= bStt ? &m_footingStt : &m_footingEnd;
	CString sFooting		= bStt ? _T("기초상면좌측") : _T("기초상면우측");
	
	CTwinVector tvFooting	= pFooting->m_tvArrFront.GetTvByInfo(sFooting);
	CTwinVector tvSlope	= tvArrWing.GetTvByInfo("날개벽정면노리");
	if(Compare(tvSlope.GetHorLength(), 0.0, "<=")) return;

	double dYLower = 0;
	if(m_nTypePostBaseLine == 0 && IsOutWall(TRUE))	// 기초상단 기준
	{
		if(IsBoxType())
			dYLower = m_dELSB + m_dPRF_H1;
		else
			dYLower = tvFooting.GetXyTop().y;
	}
	if(nTypeBaseLine == 1)	// 지반고 기준
	{
		if(IsBoxType())
			dYLower = m_dELSB;
		else
		{
			CTwinVector tvLowerFooting	= pFooting->m_tvArrFront.GetTvByInfo("기초하면");
			GetLineBase()->GetElevationErathOnStation(tvLowerFooting.GetXyMid().x, dYLower);
		}
	}
	if(nTypeBaseLine == 2)	// 사용자 입력
	{
		dYLower = frM(m_dPostLineEL);
	}

	// (41420)동바리 산정위치가 기준위치 보다 낮다면 동바리가 들어가지 않아야 한다.
	if(tvSlope.m_v1.y > dYLower)
	{
		CTwinVector tv;
		tv.m_v1	= tvSlope.m_v1;
		tv.m_v2	= tvSlope.m_v2;
		tvArr.Add(tv);

		tv.m_v1	= tv.m_v2;
		tv.m_v2	= CVector(tvSlope.m_v2.x, dYLower, 0);
		tvArr.Add(tv);

		tv.m_v1	= tv.m_v2;
		tv.m_v2	= CVector(tvSlope.m_v1.x, dYLower, 0);
		tvArr.Add(tv);

		tv.m_v1	= tv.m_v2;
		tv.m_v2	= tvSlope.m_v1;
		tvArr.Add(tv);
	}
}


// 말뚝 길이 권고안
// 보링중 암이 나오는 시작높이부터 근입깊이만큼 더 내려감
// 암이 없다면 마지막 마지막 지반에서 근입깊이 더 내려감
// 보링이 아무것도 없다면 0
// 보링이 위에 있어서 길이가 마이너스가 나오면 0
void CRcBridgeApp::SetDataDefaultPileLength(long nJ)
{
	CBoringDataEx *pBoringEx	= GetBoringDataExByJijumNumber(nJ);
	if(!pBoringEx) return;

	CFootingApp *pFooting	= GetFootingByJijumNumber(nJ);
	if(!pFooting) return;
	if(!pFooting->m_bIs) return;
	if(pFooting->m_exFooting.m_nType != EXFOOTING_TYPE_PILE) return;
	CExPileApp *pPile	= &pFooting->m_exFooting.m_Pile;

	double dYLowerOfFooting	= pFooting->m_dEL - pFooting->GetHeight();
	double dYSttAm			= dYLowerOfFooting;
	
	// 암이 있는 시작 위치를 찾음
	long i = 0; for(i = 0; i < pBoringEx->m_pArrBoring.GetSize(); i++)
	{
		CBoringData *pBoring	= pBoringEx->m_pArrBoring.GetAt(i);
		if(pBoring->GetBoringType() != TOSA || i == pBoringEx->m_pArrBoring.GetSize()-1)
		{
			if(i > 0)
				pBoring	= pBoringEx->m_pArrBoring.GetAt(i-1);

// 			dYSttAm	= pBoringEx->m_EL - pBoring->m_dELStart - pBoring->m_dDepth;
			dYSttAm	= pBoringEx->m_EL - pBoring->m_dDepth;
			break;
		}
	}


	// 최소 5M로 설정
	double dLen	= MAX(5000,(dYLowerOfFooting - frM(dYSttAm)) + pPile->m_dDf + 100);
	if(dLen < 0) dLen = 0;

	// 말뚝에 길이 넣어줌
	pPile->m_dLength	= dLen;
	for(i = 0; i < pPile->m_pArrPile.GetSize(); i++)
	{
		CPileAttr *pPileAttr = pPile->m_pArrPile.GetAt(i);
		pPileAttr->len = pPile->m_dLength;
	}
}

// bPilePos : 말뚝의 근입위치, FALSE인 경우 기초의 하단위치에서 찾는다...
CBoringData* CRcBridgeApp::GetBoringDataJijumBase(long nJijum, BOOL bPilePos)
{
	CBoringDataEx *pBorEx = GetBoringDataExByJijumNumber(nJijum);
	CFootingApp *pFooting = GetFootingByJijumNumber(nJijum);
	if(pBorEx == NULL || pFooting == NULL) return NULL;

	double dEL = toM(pFooting->m_dEL - pFooting->GetHeight());
	BOOL bMass = pFooting->m_exFooting.m_nType == EXFOOTING_TYPE_MASS;

	if(IsBoxType())
	{
		if(bMass)
			dEL = toM(m_dELSB - pFooting->m_exFooting.GetMaxHeight());
		else
			dEL = toM(m_dELSB);
	}
	else if(bMass)
	{
		dEL -= toM(pFooting->m_exFooting.GetMaxHeight());
	}
	else if(bPilePos && pFooting->m_exFooting.m_nType == EXFOOTING_TYPE_PILE)
	{
		dEL -= toM(pFooting->m_exFooting.m_Pile.m_dLength);
	}

	CBoringData *pBoring = pBorEx->GetBoringByElevation(dEL);

	return pBoring;
}

// 슬래브교의 각 지점별 교각 좌표
void CRcBridgeApp::GetTvVertSectionPier(CTwinVectorArray &tvArr, long nJ)
{
	tvArr.RemoveAll();
	if(nJ == 0 || nJ >= m_nQtyJigan) return;

	long nIdxInWall	= nJ-1;
	CWallApp *pWall	= GetInWall(nIdxInWall);
	if(pWall)
	{
		if(pWall->m_bIs) return;
	}


	CTwinVector tv;
	CDPoint xyOrg(0, 0);
	double WCop	= 1000;	// 두부보 너비
	double HCop	= 1000;	// 두부보 높이
	double HS	= 400;	// 슈 높이
	double WCol	= 800;	// 기둥 너비
	double HCol	= 1200;	// 기둥 높이
	long nIdxHunch	= GetIdxHunchInWall(nIdxInWall, TRUE);
	CString sHunch	= _T("");
	sHunch.Format("%d번째내측헌치아래면좌측", nIdxHunch+1);
	if(!m_tvArrVertSection.GetTvByInfo(sHunch, tv)) return;
	WCop	= tv.GetHorLength() * 2;
	WCol	= WCop - 200;

	xyOrg	= tv.GetXyRight();
	
	tv.m_v1	= CVector(xyOrg.x-WCop/2, xyOrg.y-HS, 0);
	tv.m_v2	= CVector(xyOrg.x+WCop/2, xyOrg.y-HS, 0);
	tv.m_sInfo	= "슈받침부";
	tvArr.Add(tv);

	tv.m_v1	= tv.m_v2;
	tv.m_v2	= CVector(tv.m_v1.x, xyOrg.y-HS-HCop, 0);
	tv.m_sInfo	= "두부보우측면";
	tvArr.Add(tv);

	tv.m_v1	= tv.m_v2;
	tv.m_v2	= CVector(tv.m_v1.x-(WCop-WCol)/2, xyOrg.y-HS-HCop, 0);
	tv.m_sInfo	= "두부보하면우측";
	tvArr.Add(tv);

	tv.m_v1	= tv.m_v2;
	tv.m_v2	= CVector(tv.m_v1.x, xyOrg.y-HS-HCop-HCol, 0);
	tv.m_sInfo	= "기둥우측면";
	tvArr.Add(tv);


	tv.m_v1	= CVector(xyOrg.x-WCop/2, xyOrg.y-HS, 0);
	tv.m_v2	= CVector(tv.m_v1.x, xyOrg.y-HS-HCop, 0);
	tv.m_sInfo	= "두부보좌측면";
	tvArr.Add(tv);

	tv.m_v1	= tv.m_v2;
	tv.m_v2	= CVector(tv.m_v1.x+(WCop-WCol)/2, xyOrg.y-HS-HCop, 0);
	tv.m_sInfo	= "두부보하면좌측";
	tvArr.Add(tv);

	tv.m_v1	= tv.m_v2;
	tv.m_v2	= CVector(tv.m_v1.x, xyOrg.y-HS-HCop-HCol, 0);
	tv.m_sInfo	= "기둥좌측면";
	tvArr.Add(tv);


}


// 내측벽체 헌치 자동 권고안
// 내측헌치를 벽체의 너비에 맞게 맞춰주는 역활만 함
void CRcBridgeApp::SetDatadefaultShapeInWallHunch(long nIdxInWall)
{
	long nIdxHunchL	= GetIdxHunchInWall(nIdxInWall, TRUE);
	long nIdxHunchR	= GetIdxHunchInWall(nIdxInWall, FALSE);

	CWallApp *pWall	= GetInWall(nIdxInWall);
	if(!pWall) return;
	if(!pWall->m_bIs) return;

	if(nIdxHunchL > -1)
	{
		CString sUpper	= pWall->m_nType == WALL_TYPE_V ? "벽체좌측팔상면" : "벽체상면";
		CTwinVector tvUpper	= pWall->m_tvArrFront.GetTvByInfo(sUpper);
		double dW	= tvUpper.GetHorLength(FALSE);
		m_dWL[nIdxHunchL][iUPPER]	= Round(dW/2, 0);
		m_dWR[nIdxHunchL][iUPPER]	= Round(dW/2, 0);
	}

	if(nIdxHunchR > -1 && nIdxHunchL != nIdxHunchR && pWall->m_nType == WALL_TYPE_V)
	{
		CString sUpper	= "벽체우측팔상면";
		CTwinVector tvUpper	= pWall->m_tvArrFront.GetTvByInfo(sUpper);
		double dW	= tvUpper.GetHorLength(FALSE);
		m_dWL[nIdxHunchR][iUPPER]	= Round(dW/2, 0);
		m_dWR[nIdxHunchR][iUPPER]	= Round(dW/2, 0);
	}
}

// 브라켓 있는지 검사.
BOOL CRcBridgeApp::IsBracket(BOOL bStt)
{
	BOOL bOK	= FALSE;
	if(bStt)
	{
		if(Compare(m_dBHWS, 0.0, ">") && Compare(m_dBHS, 0.0, ">")) bOK = TRUE;
	}
	else
	{
		if(Compare(m_dBHWE, 0.0, ">") && Compare(m_dBHE, 0.0, ">")) bOK = TRUE;
	}

	return bOK;
}

// sta에서 슬래브 중심 dist 구함.
double CRcBridgeApp::GetDistMidOfSlab(double dSta, BOOL bVert, BOOL bBaseWidth)
{
	
	long nJ	= GetJijumNumberByStation(dSta);
// 	CDPoint vAng	= bVert ? CDPoint(0, 1) : GetJijum(nJ)->m_vAngle;
	CDPoint vAng	= GetJijum(nJ)->m_vAngle;

	// (ARCBRIDGE-3533) 확폭일때 입력 스테이션에서의 거리와 지점에서의 중심 거리는 상당히 차이가 있다. 입력 위치로 한다.
	// 단, 각도는 특정할수 없으므로 지점 각도를 사용한다.
	CTwinVector tvStt;
	if(bBaseWidth)
	{
		// (ARCBRIDGE-3554) 확폭배근을 위해서는 가각 및 확폭이 포함된 폭의 중심이 필요하지만 도면을 그리는데는 기본폭의 중심을 적용한다.
		// tvStt	= GetTvJijum(nJ, 0);
		tvStt.m_v1 = GetXySlabEdgeAct(dSta, vAng, TRUE);
		tvStt.m_v2 = GetXySlabEdgeAct(dSta, vAng, FALSE);
	}
	else
	{
		tvStt.m_v1 = GetXySlabEdgeActByGagak(dSta, vAng, TRUE);
		tvStt.m_v2 = GetXySlabEdgeActByGagak(dSta, vAng, FALSE);
	}
	CDPoint xyMid	= tvStt.GetXyMid();

	CLineInfo *pLineBase	= GetLineBase();
	if(!pLineBase) return 0;

	CDPoint xyLine(0, 0);
	// dSta	= GetStationOnJijum(nJ);
	xyLine	= pLineBase->GetXyLine(dSta);
	
	double dDist	= ~(xyMid - xyLine);
	// -인지 판단
	double dDistTmp	= 0;
	dDistTmp	= pLineBase->GetDisMatchVerticalByXy(xyMid);
	if(dDistTmp < 0)
		dDist	= -dDist;

	if(bVert)
	{
		dDist *= vAng.y;
	}

	return dDist;



	/*

	double dWidthLeft	= GetWidthSlabAct(dSta, vAng, -1, FALSE, FALSE);
	double dWidthRight	= GetWidthSlabAct(dSta, vAng, 1, FALSE, FALSE);
	//dWidthLeft	= m_dWidthSlabLeft < 0 ? dWidthLeft * -1 : dWidthLeft;
	//dWidthRight	= m_dWidthSlabRight < 0 ? dWidthRight * -1 : dWidthRight;
	double dMidDist		= (-dWidthLeft+dWidthRight)/2;

	return dMidDist;
	*/
}

void CRcBridgeApp::MakeWallByOutWallOnlyShape(CWallApp &wall, BOOL bStt, BOOL bApplyAngle, BOOL bDefaultSupAndShear, double dStation)
{
	if(!IsOutWall(bStt)) return;

	long nJijum	= bStt ? 0 : m_nQtyJigan;
	CFootingApp *pFooting	= bStt ? &m_footingStt : &m_footingEnd;

	// 형상 convert
	// 일반도는 이렇게 해주면 되지만 배근도는 전면과 배면의 폭이 다른 경우가 있기때문에 Station 인자로 받음
	double dSta	= dStation;
	if(Compare(dStation, dSta_Check_NotUse_Minus, _T("="), 0.01))
	{
		dSta	= GetStationByTarget(m_nTypeWallForRebarGen == 0 ? (bStt ? BRIDGE_OFF_STT_SLAB : BRIDGE_OFF_END_SLAB) : (bStt ? BRIDGE_OFF_STT_STT : BRIDGE_OFF_END_END), 0, TRUE);
		if(bDefaultSupAndShear)
		{
			dSta	= GetStationByTarget((bStt ? BRIDGE_OFF_STT_STT : BRIDGE_OFF_END_END), 0, TRUE);
			dSta	+= bStt ? -m_dWS/2 : m_dWE/2;
		}
	}
	
	wall.m_bIs		= TRUE;
	wall.m_nType	= WALL_TYPE_VER;
	if(IsBoxType())
		wall.m_dELB		= GetXyToDistFromLowerSlab(dSta, 0, 0, 0, FALSE).y;
	else
		wall.m_dELB		= pFooting->m_dEL-pFooting->GetHeight();
	wall.m_xyOrg	= GetXyToDistFromSlabUpper(dSta, 0, 0, -1, FALSE);	
	wall.m_dAngleUpperSlab_Front	= GetAngleJongdan(wall.m_xyOrg.x);
	wall.m_dW		= bStt ? m_dWS : m_dWE;
	wall.m_nIdxCountCycleMainRebar	= m_nIdxCountCycleMainRebar;
	wall.m_dCoverUpper_UpperSlab	= m_dCoverUpper_UpperSlab[0];
	wall.m_nTypeRebarFront			= 0;
	long dan = 0; for(dan = 0; dan < MAX_REBAR_DAN; dan++)
	{
		wall.m_dCoverLeft[dan]	= bStt ? m_dCoverWallOut[dan] : m_dCoverWallIn[dan];	// 벽체 왼쪽 피복
		wall.m_dCoverRight[dan]	= bStt ? m_dCoverWallIn[dan] : m_dCoverWallOut[dan];	// 벽체 오른쪽 피복
		wall.m_dCoverFrontBack[dan]	= GetCoverSide_UpperSlab(dan);
	}
	wall.m_dCoverLower_LowerSlab			= m_dCoverLower_LowerSlab[0];


	wall.m_footing	= *pFooting;
	//wall.m_footing.m_dFH	= pFooting->GetHeight(TRUE);
	
	SyncWall(nJijum, &wall, bApplyAngle, dSta);

	if(IsBoxType())
	{
// 		CTwinVector tvLower;
// 		tvLower = GetTvOffset_LowerSlab(0, FALSE, -1, -1, bVertDir);
// 		wall.m_tvArrLowerLimit_FrontRebar.RemoveAll();
// 		wall.m_tvLowerLimitOfMainRebar		= tvLower;
// 		tvLower.m_v1.y -= m_dCoverLower_LowerSlab[0];
// 		tvLower.m_v2.y -= m_dCoverLower_LowerSlab[0];
// 		wall.m_tvArrLowerLimit_FrontRebar.Add(tvLower);
// 
// 		tvLower = GetTvOffset_LowerSlab(-1, TRUE, -1, -1, bVertDir);
// 		wall.m_tvLowerLimitOfSupportRebar		= tvLower;
// 
// 		wall.m_dCoverLower_LowerSlab			= m_dCoverLower_LowerSlab[0];
	}
	else
	{
		CTwinVector tvPathLeft	= wall.m_footing.m_tvPathLeft;
		CTwinVector tvPathLeftEx	= wall.m_footing.m_tvPathLeft;
		CTwinVector tvPathRightEx	= wall.m_footing.m_tvPathRight;
		wall.m_footing	= *pFooting;	// SyncWall은 내측벽체용이라서 시종점 벽체용일경우 기초가 변경될 수 있다. 따라서 함수 통과후 다시 기초를 설정해줌
		wall.m_footing.m_tvPathLeft = tvPathLeft;
		wall.m_footing.m_exFooting.m_tvPathLeft		= tvPathLeftEx;
		wall.m_footing.m_exFooting.m_tvPathRight	= tvPathRightEx;
	}
}


// 상부슬래브 폭이 가장 작은 스테이션 리턴
// 주철근 배근시 기준이 됨.(단, 사보강배치일 경우 사방향 철근이 깔끔하게 들어가려면 벨 폭이 큰 쪽을 기준으로 들어가야 된다.)
// 나머지는 가각 부 배근이 될 예정임.
// long nLeft : 좌우측만 따로 검사 해야 될 때도 있다.(확폭인데, 스테이션별로 좌우폭 비율이 전부 틀릴땔를 위함)
// BOOL bSlabEnd : 사각이 있는경우 중심선형위치의 Station이 아닌 슬래브 끝단위치의 스테이션
double CRcBridgeApp::GetStationSmallestWidthOfUpperSlab(BOOL bMax, long nLeft, BOOL bSlabEnd)
{	
	CLineInfo *pLine	= GetLineBase();
	if(!pLine) return m_dStationBridgeStt;

	double dSta			= m_dStationBridgeStt;
	double dStaFind		= m_dStationBridgeStt;
	double dWMin(0), dWMax(0);
	if(bMax)
	{
		for(long nJ = 0; nJ <= m_nQtyJigan; nJ++)
		{
			dSta	= GetStationOnJijum(nJ);
			if(nJ == 0) 
				dSta	= GetStationByTarget(BRIDGE_OFF_STT_SLAB, 0, TRUE);
			else if(nJ == m_nQtyJigan)
				dSta	= GetStationByTarget(BRIDGE_OFF_END_SLAB, 0, TRUE);
			if(nJ == 0)
			{
				dWMax	= GetWidthSlabActByGagak(dSta, CDPoint(0, 1), nLeft);
				dStaFind	= dSta;
			}
			else
			{
				double dW	= GetWidthSlabActByGagak(dSta, CDPoint(0, 1), nLeft);
				if(dWMax < dW)
				{
					dWMax	= dW;
					dStaFind	= dSta;
				}

				if(nJ == m_nQtyJigan)
					dSta	= GetStationOnJijum(nJ-1);
				else
					dSta	= dSta + GetLengthJigan(nJ-1)/2;

				dW	= GetWidthSlabActByGagak(dSta, CDPoint(0, 1), nLeft, FALSE);
				if(dWMax < dW)
				{
					dWMax	= dW;
					dStaFind	 = dSta;
				}
			}
		}
	}
	else
	{
		double dW	= 0;
		for(long nJ = 0; nJ <= m_nQtyJigan; nJ++)
		{
			dSta	= GetStationOnJijum(nJ);
			if(nJ == 0) 
				dSta	= GetStationByTarget(BRIDGE_OFF_STT_SLAB, 0, TRUE);
			else if(nJ == m_nQtyJigan)
				dSta	= GetStationByTarget(BRIDGE_OFF_END_SLAB, 0, TRUE);
			if(nJ == 0)
			{
				dWMin	= GetWidthSlabActByGagak(dSta, CDPoint(0, 1), nLeft);
				dStaFind	= dSta;

				dSta += GetLengthJigan(nJ) / 2;
			}
			else
			{
				dW	= GetWidthSlabActByGagak(dSta, CDPoint(0, 1), nLeft);
				if(dWMin > dW)
				{
					dWMin	= dW;
					dStaFind	= dSta;
				}

				if(nJ == m_nQtyJigan)
					dSta	= GetStationOnJijum(nJ);
				else
					dSta	= dSta + GetLengthJigan(nJ-1)/2;

			}

			dW	= GetWidthSlabActByGagak(dSta, CDPoint(0, 1), nLeft, FALSE);
			if(dWMin > dW)
			{
				dWMin	= dW;
				dStaFind	 = dSta;
			}
		}
	}

	// (ARCBRIDGE-3019) 최대 최소 폭이 사각이 큰 확폭 인경우 시종점 Station 밖에서 생기게되면 철근 배치 구간을 벗어나게 된다. 교량 끝단 Station을 고려해주자.
	// (RCBRIDGE-3533) 철근방향에 따라서 적용이 다르다. 주철근이 직각방향일때는 기준위치가 다르기때문에 중심선형 Station을 적용하는게 맞다.
	if(bSlabEnd)
	{
		for(long nSe=0; nSe<2; ++nSe)
		{
			BOOL bStt = (nSe == 0) ? TRUE : FALSE;
			TARGET_IN_ONE_SLAB eTarget = bStt ? BRIDGE_OFF_STT_SLABCOVER : BRIDGE_OFF_END_SLABCOVER;

			for(long nLr=0; nLr<2; ++nLr)
			{
				BOOL bLeft = (nLr == 0) ? TRUE : FALSE;

				dSta = GetStationByTarget(eTarget, 0, FALSE);
				CDPoint vAng = GetAngleByTarget(eTarget, 0);
				CDPoint xyEdge = GetXySlabEdgeAct(dSta, vAng, bLeft);
				if(GetLineBase()->GetStationMatchVerticalByXy(xyEdge, dSta))
				{
					double dW	= GetWidthSlabActByGagak(dSta, CDPoint(0, 1), nLeft, FALSE);
					if(bMax)
					{
						if(dWMax < dW)
						{
							dWMax	= dW;
							dStaFind	 = dSta;
						}
					}
					else
					{
						if(dWMin > dW)
						{
							dWMin	= dW;
							dStaFind	 = dSta;
						}
					}
				}
			}
		}
	}

	return dStaFind;
}

long CRcBridgeApp::GetShoePosByStation(double dStation)
{	
	if(IsBoxType()) return -1;
	long nJijum = GetJijumNumberByStation(dStation);

	CJijum *pJ = GetJijum(nJijum);

	if(pJ->m_nQtyShoe[0]>0)
	{
		double dStationJijum = GetStationOnJijum(nJijum);
		double dStationShoe  = dStationJijum - (pJ->m_dLengthUgan[0] + pJ->m_dLengthSlabToShoe[0]);
		if(Compare(dStation,dStationShoe,"="))
			return 0;
	}
	if(pJ->m_nQtyShoe[1]>0)
	{
		double dStationJijum = GetStationOnJijum(nJijum);
		double dStationShoe  = dStationJijum + (pJ->m_dLengthUgan[1] + pJ->m_dLengthSlabToShoe[1]);
		if(Compare(dStation,dStationShoe,"="))
			return 1;
	}

	return -1;
}

// bLeft : 도로중심을 기준으로 좌.우
double CRcBridgeApp::GetDistColumnNext(long nJijum, long nColumn, BOOL bLeft)
{
	CWallApp   *pWall = GetInWall(nJijum-1);

	if(bLeft) return pWall->m_dDistColumn[nColumn];

	CDPoint vAng = GetAngleJijum(nJijum);
	double dStation   = GetStationOnJijum(nJijum);

	double dSlabWidth = GetWidthSlabAct(dStation,vAng);
	double dDistRight = 0;

	if(nColumn < pWall->m_nCountColumn-1)
	{
		dDistRight = pWall->m_dDistColumn[nColumn+1];
	}
	else
	{
		double dDistSum = 0;
		long i=0; for(i=0; i<pWall->m_nCountColumn; i++)
		{
			dDistSum += pWall->m_dDistColumn[i];
		}
		dDistRight = dSlabWidth - dDistSum;
	}

	return dDistRight;
}

double CRcBridgeApp::GetGirderHeight(long nJijum, BOOL bUpperSlab)
{
	if(bUpperSlab == FALSE && IsBoxType() == FALSE) return 0;

	double dSlabThick = bUpperSlab ? m_dTS : m_dTS_Lower;
	double dHeightL	= GetThickHunch(nJijum, TRUE, TRUE, bUpperSlab) + dSlabThick;
	double dHeightR	= GetThickHunch(nJijum, FALSE, TRUE, bUpperSlab) + dSlabThick;

	return min(dHeightL, dHeightR);
}


// long CRcBridgeApp::GetOutValueSlabChange(long nJigan, double &Mmax, double &Mmin, double &Md)
// {
// 	CFEMManage *pFEM = &m_FemNormal;
// 	CElement *pEle =  NULL;
// 	
// 	long nIndexStt = GetNumberElementSlabSpanSttCheck(nJigan, 0, FALSE, FALSE, TRUE);
// 	long nIndexEnd = GetNumberElementSlabSpanEndCheck(nJigan, 0, FALSE, FALSE, TRUE);
// 	if(nIndexStt==-1 && nIndexEnd==-1) return 0;
// 	
// 	CString szComboLFmax = "LF MAX";
// 	CString szComboLFmin = "LF MIN";
// 	CString szComboD = "D";
// 	
// 	long nLoadCaseLFmax  = pFEM->GetLoadCaseIndex(szComboLFmax);
// 	long nLoadCaseLFmin  = pFEM->GetLoadCaseIndex(szComboLFmin);
// 	long nLoadCaseD  = pFEM->GetLoadCaseIndex(szComboD);
// 	if(nLoadCaseLFmax<0 || nLoadCaseLFmin<0 || nLoadCaseD<0) return 0;
// 	
// 	CResultForceElement *pResultMax = NULL;
// 	CResultForceElement *pResultMin = NULL;
// 	CResultForceElement *pResultD	= NULL;
// 	
// 	double dValueMax = 0;
// 	double dValueMin = 0;
// 	double dOutputMax = 0;
// 	double dOutputMin = 0;
// 	double dOutputD	  = 0;
// 	
// 	long nPos = 0;
// 	BOOL bFirst = TRUE;
// 	for(long nEle=nIndexStt; nEle<=nIndexEnd; nEle++)
// 	{
// 		pEle = pFEM->GetElement((USHORT)nEle);
// 		pResultMax = pEle->GetResultForce(nLoadCaseLFmax);
// 		pResultMin = pEle->GetResultForce(nLoadCaseLFmin);
// 		pResultD = pEle->GetResultForce(nLoadCaseD);
// 		
// 		long nSizePoint = pResultMax->m_ResultForceElement.GetSize();
// 		long i=0; for(i=0; i<nSizePoint; i++)
// 		{
// 			dValueMax  = pResultMax->m_ResultForceElement[i]->m_dForceOnElement[ELE_MOMENT2];
// 			dValueMin  = pResultMin->m_ResultForceElement[i]->m_dForceOnElement[ELE_MOMENT2];
// 			if(bFirst || (dValueMax-dValueMin)>(dOutputMax-dOutputMin))
// 			{
// 				dOutputMax = dValueMax;
// 				dOutputMin = dValueMin;
// 				dOutputD	= pResultD->m_ResultForceElement[i]->m_dForceOnElement[ELE_MOMENT2];
// 				bFirst = FALSE;
// 				
// 				if(fabs(dValueMax) > fabs(dValueMin)) nPos = 1;
// 				else nPos = (nEle > nIndexStt+(nIndexEnd-nIndexStt)/2)? 2:0;
// 			}
// 		}
// 	}
// 
// 	Mmax = dOutputMax;
// 	Mmin = dOutputMin;
// 	Md	= dOutputD;
// 
// 	return nPos;
// }

long CRcBridgeApp::GetOutValueSlabMaxForFatigue(long nJigan, double &Mmax, double &Mmin)
{
	CFEMManage *pFEM = &m_FemNormal;
	CElement *pEle =  NULL;
	
	long nIndexStt = GetNumberElementSlabSpanSttCheck(nJigan, 0, FALSE, FALSE, TRUE);
	long nIndexEnd = GetNumberElementSlabSpanEndCheck(nJigan, 0, FALSE, FALSE, TRUE);
	if(nIndexStt==-1 && nIndexEnd==-1) return 0;
	
	CString szLoadLive	 = "LF";
	CString szComboLFmax = "LF MAX";
	CString szComboLFmin = "LF MIN";
	
	long nLoadCaseLFmax  = pFEM->GetLoadCaseIndex(szComboLFmax);
	long nLoadCaseLFmin  = pFEM->GetLoadCaseIndex(szComboLFmin);
	long nLoadCaseLF	 = pFEM->GetLoadCaseIndex(szLoadLive);
	
	if(m_bUnderGroundRahmen && !m_bCalcOpen && nLoadCaseLF<0) return 0;
	else if(!m_bUnderGroundRahmen && (nLoadCaseLFmax<0 || nLoadCaseLFmin<0)) return 0;
	
	CResultForceElement *pResultMax = NULL;
	CResultForceElement *pResultMin = NULL;
	
	double dValueMax = 0;
	double dValueMin = 0;
	double dOutputMax = 0;
	double dOutputMin = 0;
	
	long nPos = 0;
	BOOL bFirst = TRUE;
	for(long nEle=nIndexStt; nEle<=nIndexEnd; nEle++)
	{
		pEle = pFEM->GetElement((USHORT)nEle);
		if(pEle==NULL) continue;
		if(m_bUnderGroundRahmen && !m_bCalcOpen)
		{
			pResultMax = pEle->GetResultForce(nLoadCaseLF);
		}
		else
		{
			pResultMax = pEle->GetResultForce(nLoadCaseLFmax);
			pResultMin = pEle->GetResultForce(nLoadCaseLFmin);
		}
		
		long nSizePoint = pResultMax->m_ResultForceElement.GetSize();
		long i=0; for(i=0; i<nSizePoint; i++)
		{
			if(m_bUnderGroundRahmen && !m_bCalcOpen)
			{
				dValueMax  = pResultMax->m_ResultForceElement[i]->m_dForceOnElement[ELE_MOMENT2];
				dValueMin  = 0;
			}
			else
			{
				dValueMax  = pResultMax->m_ResultForceElement[i]->m_dForceOnElement[ELE_MOMENT2];
				dValueMin  = pResultMin->m_ResultForceElement[i]->m_dForceOnElement[ELE_MOMENT2];
			}
			
			if(bFirst || dOutputMax < dValueMax)
			{
				dOutputMax = dValueMax;
				dOutputMin = dValueMin;
				bFirst = FALSE;

				if(fabs(dValueMax) > fabs(dValueMin)) nPos = 1;
				else nPos = (nEle > nIndexStt+(nIndexEnd-nIndexStt)/2)? 2:0;
			}
		}
	}
	
	Mmax = dOutputMax;
	Mmin = dOutputMin;
	
	return nPos;
}

//<summary>
// 슬래브 외곽선과 만나는 좌표를 구함. - 가각선은 무시됨.
//</summary>
//<param name="sta">도로중심 스테이션</param>
//<param name="vAng">도로중심 각도</param>
//<param name="bLeft"> TRUE = 좌측 슬래브 외곽선, FALSE = 우측 슬래브 외곽선</param>
//<param name="bRoadNotSlab">TRUE=지중라멘의 상부 노면(도로면, 철도면)이며 슬래브면(교면)아님
//							FALSE=일반교량, 지중라멘의 슬래브면(교면)</param>
CDPoint CRcBridgeApp::GetXySlabEdgeAct(double sta, CDPoint vAng, BOOL bLeft, BOOL bRoadNotSlab)
{
	if(bRoadNotSlab)
	{
		if(bLeft)	//좌측이면
		{
			double  staLeft = GetStationByDisExtLineSimple(sta, vAng, 0, -1);
			return GetLineLeftRight(bLeft)->GetXyLineDis(staLeft, m_dWidthRoadLeft);
		}
		double  staRight = GetStationByDisExtLineSimple(sta, vAng, 0, 1);
		return GetLineLeftRight(bLeft)->GetXyLineDis(staRight, m_dWidthRoadRight);
	}

	if(bLeft)	//좌측이면
	{
		double  staLeft = GetStationByDisExtLineSimple(sta, vAng, 0, -1);
		return GetLineLeftRight(bLeft)->GetXyLineDis(staLeft, m_dWidthSlabLeft);
	}
	double  staRight = GetStationByDisExtLineSimple(sta, vAng, 0, 1);
	return GetLineLeftRight(bLeft)->GetXyLineDis(staRight, m_dWidthSlabRight);
}

//<summary>
// 슬래브 외곽선과 만나는 좌표를 구함. - 가각선 구간에 있으면 가각선과 만나는 좌표 구함
//</summary>
//<param name="sta">도로중심 스테이션</param>
//<param name="vAng">도로중심 각도</param>
//<param name="bLeft"> TRUE = 좌측 슬래브 외곽선, FALSE = 우측 슬래브 외곽선</param>
CDPoint CRcBridgeApp::GetXySlabEdgeActByGagak(double sta, CDPoint vAng, BOOL bLeft, BOOL bRoadNotSlab)
{
	CDPoint xyResult;
	if(IsMatchGagak(sta, vAng, bLeft, TRUE, bRoadNotSlab, xyResult))	//시점 가각
		return xyResult;
	if(IsMatchGagak(sta, vAng, bLeft, FALSE, bRoadNotSlab, xyResult))	//종점 가각
		return xyResult;

	xyResult	= GetXySlabEdgeAct(sta, vAng, bLeft, bRoadNotSlab);

	// 가각과 만나진 않지만 시종점과 만나는지 검사해서 만난 곳이 기본폭보다 크면 시종점의 교차점을 리턴
	if(Compare(vAng.GetAngleDegree(), 90.0, "="))
	{
		CLineInfo *pLine	= GetLineBase();
		CTwinVector tvStt, tvEnd;
		CDPoint xyBase	= pLine->GetXyLine(sta);
		CDPoint vAngAz	= pLine->GetAngleAzimuth(sta).Rotate90();

		GetTvSttEndOfSlab(tvStt, tvEnd);
		CDPoint xyMatch(0, 0);

		if(GetXyMatchSegAndLine(tvStt.m_v1, tvStt.m_v2, xyBase, vAngAz, xyMatch))
		{
			if(bLeft)
			{
				if(pLine->GetDisMatchVerticalByXy(xyResult) > pLine->GetDisMatchVerticalByXy(xyMatch))
					return xyMatch;
			}
			else
			{
				if(pLine->GetDisMatchVerticalByXy(xyResult) < pLine->GetDisMatchVerticalByXy(xyMatch))
					return xyMatch;
			}
		}

		if(GetXyMatchSegAndLine(tvEnd.m_v1, tvEnd.m_v2, xyBase, vAngAz, xyMatch))
		{
			if(bLeft)
			{
				if(pLine->GetDisMatchVerticalByXy(xyResult) > pLine->GetDisMatchVerticalByXy(xyMatch))
					return xyMatch;
			}
			else
			{
				if(pLine->GetDisMatchVerticalByXy(xyResult) < pLine->GetDisMatchVerticalByXy(xyMatch))
					return xyMatch;
			}
		}
	}

	return xyResult;
}

//<summary>
// 가각과 만나는지 점검하고 만나는 경우 TRUE 값과 Match 좌표 xyResult 를 리턴함.
//</summary>
//<param name="sta">도로중심 스테이션</param>
//<param name="vAng">도로중심 각도</param>
//<param name="bLeft"> TRUE = 좌측 가각부, FALSE = 우측 가각부</param>
//<param name="bStt">  TRUE = 시점 가각부, FALSE = 종점 가각부</param>
BOOL CRcBridgeApp::IsMatchGagak(double sta, CDPoint vAng, BOOL bLeft, BOOL bStt, BOOL bRoadNotSlab, CDPoint &xyResult)
{
	BOOL bMatch = FALSE;

	if (bRoadNotSlab)
	{
		return bMatch;
	}

	if(!m_bSettingPlane)
		SettingPlane();

	if(m_bSettingPlane)
	{
		CTwinVectorArray tvArrExp;
		GetTvPlaneOfExp(tvArrExp, bStt, bLeft);
		if(tvArrExp.GetSize() == 0) return FALSE;

		CDPoint xyLine = GetLineBase()->GetXyLine(sta);
		CDPoint vAngAbs = GetLineBase()->GetAngleAzimuth(sta).Rotate(vAng);	//전체 좌표계의 각도

		CRebarPlacing rb;
		CDPointArray xyArrMatch;
		rb.GetXyMatchTvArrAndLine(tvArrExp, xyLine, vAngAbs, xyArrMatch);
		if(xyArrMatch.GetSize() > 0)
		{
			xyResult	= xyArrMatch.GetAt(0);
			bMatch	= TRUE;
		}

		return bMatch;
	}
	

	double W = 0;
	double H = 0;
	double R = 0;
	CDPoint slabA[4];
	double staSlabW = 0;
	double disSlab = 0;
	CDPoint xySlabSttEnd;
	CDPoint xyCenStt(0,0),xySegStt(0,0);
	CLineInfo *pLine = GetLineLeftRight(bLeft);
	if(bStt && bLeft)
	{
		W = m_dExpSttW[0];
		H = m_dExpSttW[0];
		if(W == 0 || H == 0)
			return bMatch;
		R = m_dExpSttR[0];
		disSlab = m_dWidthSlabLeft;
		GetXySss(BRIDGE_OFF_STT_SLAB,slabA);
		staSlabW = GetStationByTarget(BRIDGE_OFF_STT_SLAB,-1,FALSE) + W;
		xySlabSttEnd = slabA[0];
	}
	else if(bStt && !bLeft)
	{
		W = m_dExpSttW[1];
		H = m_dExpSttW[1];
		if(W == 0 || H == 0)
			return bMatch;
		R = m_dExpSttR[1];
		disSlab = m_dWidthSlabRight;
		GetXySss(BRIDGE_OFF_STT_SLAB,slabA);
		staSlabW = GetStationByTarget(BRIDGE_OFF_STT_SLAB,1,FALSE) + W;
		xySlabSttEnd = slabA[3];
	}
	else if(!bStt && bLeft)
	{
		W = m_dExpEndW[0];
		H = m_dExpEndW[0];
		if(W == 0 || H == 0)
			return bMatch;
		R = m_dExpEndR[0];
		disSlab = m_dWidthSlabLeft;
		GetXySss(BRIDGE_OFF_END_SLAB,slabA);
		staSlabW = GetStationByTarget(BRIDGE_OFF_END_SLAB,-1,FALSE) - W;
		xySlabSttEnd = slabA[0];
	}
	else if(!bStt && !bLeft)
	{
		W = m_dExpEndW[1];
		H = m_dExpEndW[1];
		if(W == 0 || H == 0)
			return bMatch;
		R = m_dExpEndR[1];
		disSlab = m_dWidthSlabRight;
		GetXySss(BRIDGE_OFF_END_SLAB,slabA);
		staSlabW = GetStationByTarget(BRIDGE_OFF_END_SLAB,1,FALSE) - W;
		xySlabSttEnd = slabA[3];
	}
	CDPoint xyW = pLine->GetXyLineDis(staSlabW,disSlab);
	CDPoint xyLine = GetLineBase()->GetXyLine(sta);
	CDPoint vAngAbs = GetLineBase()->GetAngleAzimuth(sta).Rotate(vAng);	//전체 좌표계의 각도
	
	pLine->GetXyForRc(disSlab,R,xyW,xySlabSttEnd,bStt, bLeft, staSlabW,xyCenStt,xySegStt);
	bMatch = GetXyMatchSegAndLine(xyW, xySlabSttEnd, xyLine, vAngAbs, xyResult);
	if(R>0)
	{
		CDPoint xyArcEnd = pLine->GetXyLineDis(staSlabW,disSlab);
		CDPoint xyResult1, xyResult2;
		if(GetDirectionRotateByPoint(xySegStt, xyArcEnd, xyCenStt)==-1)	// 시계방향
		{
			CDPoint xyArcEndPre = xyArcEnd;
			xyArcEnd = xySegStt;
			xySegStt = xyArcEndPre;
		}
		double dAngStt = (xySegStt-xyCenStt).Unit().GetAngleDegree();
		double dAngEnd = (xyArcEnd-xyCenStt).Unit().GetAngleDegree();
			
		long nMatch = GetXyMatchLineAndArc(xyLine, vAngAbs, xyCenStt, R, ToRadian(dAngStt), ToRadian(dAngEnd), xyResult1, xyResult2);
		if(nMatch == 1)
		{
			xyResult = xyResult1;
			bMatch = TRUE;
		}
		else if(nMatch == 2)
		{
			if(~(xyLine-xyResult1) < ~(xyLine-xyResult2))
				xyResult = xyResult1;
			else
				xyResult = xyResult2;
			bMatch = TRUE;
		}
		return bMatch;
	}

	return bMatch;
}

// 슬래브 끝단에서 dist만큼 떨어진 거리 다음의 횡단 인덱스
long CRcBridgeApp::GetNumHDanByDist(double sta, CDPoint vAng, double dist, BOOL bReverse)
{
	dist = Round(dist, 3);

	CDPoint A[30];
	GetXySlabUpperAct(sta, vAng, A);
	long nQtyHDan = GetQtyHDanAct(sta,vAng);
	double dXL = Round(A[0].x+dist, 3);
	double dXR = Round(A[nQtyHDan].x-dist, 3);

	int nIndex = 0;
	double dFir = 0, dSec = 0;
	if(bReverse)
	{
		int i=0; for(i=nQtyHDan; i>0; i--)
		{
			dFir = Round(A[i].x, 3);
			dSec = Round(A[i-1].x, 3);
			if(i==nQtyHDan && dXR>dFir)
			{
				nIndex = i;
				break;
			}
			else if((i==1 && dXR<dSec) || (dXR<=dFir && dXR>dSec))
			{
				nIndex = i-1;
				break;
			}
		}
	}
	else
	{
		int i=0; for(i=0; i<nQtyHDan; i++)
		{
			dFir = Round(A[i].x, 3);
			dSec = Round(A[i+1].x, 3);
			if(i==0 && dXL<dFir)
			{
				nIndex = i;
				break;
			}
			else if((i==nQtyHDan-1 && dXL>dSec) || (dXL>=dFir && dXL<dSec))
			{
				nIndex = i+1;
				break;
			}
		}
	}

	return nIndex;
}
//<summary>
//도로중심에서 nHDan 좌측까지의 거리
//도로중심의 좌측에 있으면 [-] 값을 리턴함.
//</summary>
//<param name="sta">도로중심 스테이션</param>
//<param name="nHDan">횡단 번호</param>
//<param name="bRoadNotSlab">TRUE=지중라멘의 상부 노면(도로면, 철도면)이며 슬래브면(교면)아님
//							FALSE=일반교량, 지중라멘의 슬래브면(교면)</param>
double CRcBridgeApp::GetDisSlabFrCenToHDanLeft(double sta, long nHDan, BOOL bRoadNotSlab)
{
	CDPoint vAng = CDPoint(0,1); //항상 90도 기준으로 거리를 산출할 것
	double disSlabLeft = GetDisSlabLeftAct(sta, vAng,bRoadNotSlab);
	double lenHDan = 0;
	long n=0; for(n=0; n<nHDan; n++)
	{
		lenHDan += GetLengthHDanAct(sta, vAng, n,bRoadNotSlab);
	}
	
	double dDist	= disSlabLeft + lenHDan;
	if(Compare(dDist, 0.0, "="))
		dDist	= 0;
	return dDist;
}
//<summary>
//좌측 슬래브 끝단과 만나는 실제 거리(각도 적용된 거리)를 구함. - 가각선은 무시됨.
//도로중심의 좌측에 있으면 [-] 값을 리턴함.
//</summary>
//<param name="sta">도로중심 스테이션</param>
//<param name="vAng">도로중심 각도</param>
//<param name="bRoadNotSlab">TRUE=지중라멘의 상부 노면(도로면, 철도면)이며 슬래브면(교면)아님
//							FALSE=일반교량, 지중라멘의 슬래브면(교면)</param>
double CRcBridgeApp::GetDisSlabLeftAct(double sta, CDPoint vAng, BOOL bRoadNotSlab)
{
	CDPoint vAngAbs = GetLineBase()->GetAngleAzimuth(sta).Rotate(vAng);	//전체 좌표계의 각도
	CDPoint xy = GetLineBase()->GetXyLine(sta);
	CDPoint xyL  = GetXySlabEdgeAct(sta, vAng, TRUE,bRoadNotSlab);

	return vAngAbs ^ (xy - xyL);
}

//<summary>
//좌측 슬래브 끝단과 만나는 실제 거리(각도 적용된 거리)를 구함. - 가각선 구간에 있으면 가각선과 만나는 좌표 구함
//도로중심의 좌측에 있으면 [-] 값을 리턴함.
//</summary>
//<param name="sta">도로중심 스테이션</param>
//<param name="vAng">도로중심 각도</param>
double CRcBridgeApp::GetDisSlabLeftActByGagak(double sta, CDPoint vAng, BOOL bRoadNotSlab)
{
	CDPoint vAngAbs = GetLineBase()->GetAngleAzimuth(sta).Rotate(vAng);	//전체 좌표계의 각도
	CDPoint xy = GetLineBase()->GetXyLine(sta);
	CDPoint xyL  = GetXySlabEdgeActByGagak(sta, vAng, TRUE, bRoadNotSlab);

	double dLen	= ~(xy - xyL);
	dLen	= Compare(dLen, 0.0, "=") ? 0 : vAngAbs ^ (xy - xyL);
	return dLen;
}

//<summary>
//우측 슬래브 끝단과 만나는 실제 거리(각도 적용된 거리)를 구함. - 가각선은 무시됨.
//도로중심의 좌측에 있으면 [-] 값을 리턴함.
//</summary>
//<param name="sta">도로중심 스테이션</param>
//<param name="vAng">도로중심 각도</param>
//<param name="bRoadNotSlab">TRUE=지중라멘의 상부 노면(도로면, 철도면)이며 슬래브면(교면)아님
//							FALSE=일반교량, 지중라멘의 슬래브면(교면)</param>
double CRcBridgeApp::GetDisSlabRightAct(double sta, CDPoint vAng, BOOL bRoadNotSlab)
{
	CDPoint vAngAbs = GetLineBase()->GetAngleAzimuth(sta).Rotate(vAng);	//전체 좌표계의 각도
	CDPoint xy = GetLineBase()->GetXyLine(sta);
	CDPoint xyR  = GetXySlabEdgeAct(sta, vAng, FALSE, bRoadNotSlab);

	return vAngAbs ^ (xy - xyR);
}


//<summary>
//우측 슬래브 끝단과 만나는 실제 거리(각도 적용된 거리)를 구함. - 가각선 구간에 있으면 가각선과 만나는 좌표 구함
//도로중심의 좌측에 있으면 [-] 값을 리턴함.
//</summary>
//<param name="sta">도로중심 스테이션</param>
//<param name="vAng">도로중심 각도</param>
double CRcBridgeApp::GetDisSlabRighActByGagak(double sta, CDPoint vAng, BOOL bRoadNotSlab)
{
	CDPoint vAngAbs = GetLineBase()->GetAngleAzimuth(sta).Rotate(vAng);	//전체 좌표계의 각도
	CDPoint xy = GetLineBase()->GetXyLine(sta);
	CDPoint xyR  = GetXySlabEdgeActByGagak(sta, vAng, FALSE, bRoadNotSlab);

	return vAngAbs ^ (xy - xyR);
}
//<summary>
//가각이 적용된, 각도가 적용된 실제 너비를 구함.
//</summary>
//<param name="sta">도로중심 스테이션</param>
//<param name="vAng">도로중심 각도</param>
double CRcBridgeApp::GetWidthSlabActByGagak(double sta, CDPoint vAng, long nLeft, BOOL bRoadNotSlab)
{
	double dWidth	= 0;
	CDPoint xyL  = GetXySlabEdgeActByGagak(sta, vAng, TRUE, bRoadNotSlab);
	CDPoint xyR  = GetXySlabEdgeActByGagak(sta, vAng, FALSE, bRoadNotSlab);
	CDPoint x = GetLineBase()->GetXyLine(sta);
	if(nLeft == 0)
	{
		dWidth	= ~(xyL - xyR);
	}
	else
	{
		CDPoint xyC	= GetLineBase()->GetXyLine(sta);
		dWidth		= nLeft == -1 ? ~(xyL - xyC) : ~(xyR - xyC);

		// 좌측인데 dist가 +이거나, 우측인데a dist가 -이면 dWidth는 -가 됨
		if(nLeft == -1)
		{
			if(GetLineBase()->GetDisMatchVerticalByXy(xyL) > 0)
				dWidth *= -1;
		}
		else
		{
			if(GetLineBase()->GetDisMatchVerticalByXy(xyR) < 0)
				dWidth *= -1;
		}
	}
	
	return dWidth;
}


//#define FORCE_F11			 0
//#define FORCE_F22			 1
//#define FORCE_F12			 2
//#define FORCE_MAX			 3
//#define FORCE_MIN			 4
//#define FORCE_ANGLE		 5
//#define MOMENT_M11		 6
//#define MOMENT_M22		 7
//#define MOMENT_M12		 8
//#define MOMENT_MAX		 9
//#define MOMENT_MIN		10
//#define MOMENT_ANGLE		11
//#define VER_13			12
//#define VER_23			13
//#define VER_MAX			14
//#define VER_ANGLE			15
// 사용자 입력인 경우 col - M,V,P,M,V (계수하중,사용하중)
// 사용자 입력인 경우 row - 지간별로 3개씩 
double CRcBridgeApp::GetOutValueSlabShell(long nSpan,  long nPos, long nTypeDesign, long nTypeForce, BOOL bUpperSlab, ETypeEnvComboLsd eEnvTypeLsd)
{
	if(nTypeForce!=ELE_MOMENT2 && nTypeForce!=ELE_SHEAR_2) return 0;
	if(nPos==CHECK_POS_CENTER  && nTypeForce==ELE_SHEAR_2) return 0;

	if(m_bUseShellUserOutput)
	{
		BOOL bLsd = m_pARcBridgeDataStd->IsLsdDesign();
		long nSizeRow = m_MatShellSlab.GetRow();
		long nSizeCol = m_MatShellSlab.GetCol();
		if(nSizeCol==0 || nSizeRow==0) return 0;
		long nRow = nSpan*2 + nPos;
		if(!bUpperSlab)
		{
			nRow += m_nQtyJigan * 2 + 1;
		}
		long nCol = 0;
		if(bLsd)
		{
			long nColBase = 0;
			for(long ix=0; ix<eCountEnvTypeLsd; ++ix)
			{
				ETypeEnvComboLsd eEnvType = (ETypeEnvComboLsd)ix;
				if(IsValidComboEnvType(eEnvType) == FALSE)
					continue;

				if(eEnvType == eEnvTypeLsd)
				{
					break;
				}
				nColBase += 3;
			}
			if(nTypeForce==ELE_MOMENT2) nCol = nColBase;
			if(nTypeForce==ELE_SHEAR_2) nCol = nColBase + 1;
			if(nTypeForce==ELE_AXFORCE) nCol = nColBase + 2; // 실제로는 값이 없음...
		}
		else
		{
			if(nTypeDesign==COMBO_SERVICE)
			{
				if(nTypeForce==ELE_MOMENT2) nCol = 3;
				if(nTypeForce==ELE_SHEAR_2) nCol = 4;
			}
			else
			{
				if(nTypeForce==ELE_MOMENT2) nCol = 0;
				if(nTypeForce==ELE_SHEAR_2) nCol = 1;
				if(nTypeForce==ELE_AXFORCE) nCol = 2; // 실제로는 값이 없음...
			}
		}
		
		if(nRow>=nSizeRow || nCol>=nSizeCol) return 0;
		return m_MatShellSlab(nRow,nCol);
	}

	CFEMManage *pFEM = &m_FemShell;
	if(nTypeDesign==COMBO_EARTHQUAKE2 || nTypeDesign==COMBO_EARTHQUAKE) return 0;
	if(pFEM->IsOutputLoaded()==FALSE) return 0;

	long nShellForce = (nTypeForce==ELE_MOMENT2) ? MOMENT_M11 : VER_13;

	double dOutputMax = 0;
	double dOutputMin = 0;
	double dValueMax  = 0;
	double dValueMin  = 0;
	double dValueEleMax[5] = {0,0,0,0,0};
	double dValueEleMin[5] = {0,0,0,0,0};

	CString szCombo = GetStringComboEnv(nTypeDesign, eEnvTypeLsd, bUpperSlab? ePartUpperSlab : ePartLowerSlab, nPos);

	BOOL bFirstCHeck = TRUE;

	long nLoadCase  = pFEM->GetLoadCaseIndex(szCombo);
	if(nLoadCase==-1) return 0;
	
	long idShell = 0;
	long nSizeShell = pFEM->GetShellSize();
	long nIndexNear = -1;
	long nJointNear[2] = {0,0};
	for(long nShell=0; nShell<nSizeShell; nShell++)
	{
		CShell *pShell  = pFEM->GetShell((USHORT)nShell);
		CShell *pShell2 = NULL; // 끝단부재력 구하는 경우에 입접한 부재의 값도 같이 비교...

		if(bUpperSlab && pShell->m_nAttrib!=SHELL_ELEMENT_UPPERSLAB) break;
		else if(bUpperSlab == FALSE && pShell->m_nAttrib!=SHELL_ELEMENT_LOWERSLAB) continue;

		if(pShell->m_LongAttrib[LONGSHELL_1_JIJUMNO]!=nSpan) continue;
		if(nPos==CHECK_POS_LEFT)
		{
			if(pShell->m_BoolAttrib[BOOLSHELL_0_CHECKPOS_STT]==FALSE) continue;
			nIndexNear = GetIndexShellNearByJointNumber(pShell,pShell->m_nJ[1],pShell->m_nJ[2]);
			pShell2 = (nIndexNear>=0) ? pFEM->GetShell((USHORT)nIndexNear) : NULL; // 좌측에 붙은 부재..
		}
		else if(nPos==CHECK_POS_CENTER)
		{
			if(pShell->m_BoolAttrib[BOOLSHELL_0_CHECKPOS_STT]) continue;
			if(pShell->m_BoolAttrib[BOOLSHELL_1_CHECKPOS_END]) continue;
		}
		else if(nPos==CHECK_POS_RIGHT)
		{
			if(pShell->m_BoolAttrib[BOOLSHELL_1_CHECKPOS_END]==FALSE) continue;
			nIndexNear = GetIndexShellNearByJointNumber(pShell,pShell->m_nJ[0],pShell->m_nJ[3]);
			pShell2 = (nIndexNear>=0) ? pFEM->GetShell((USHORT)nIndexNear) : NULL; // 우측에 붙은 부재..
		}

		BOOL bEmptyS1 = FALSE;
		BOOL bEmptyS2 = TRUE;
		for(long nR=0; nR<5; nR++)
		{
			if(pShell->m_ResultForce[nR].GetSize()==0)
				bEmptyS1 = TRUE;

			// (ARCBRIDGE-2635) 인접부재는 확인 하지 않기로 하자. 
// 			if(pShell2)
// 			{
// 				if(pShell2->m_ResultForce[nR].GetSize()==0)
// 					bEmptyS2 = TRUE;
// 			}
		}
		if(bEmptyS1) continue;

		dValueEleMax[0] = pShell->m_ResultForce[0].GetAt(nLoadCase)->m_dForce[nShellForce];
		dValueEleMax[1] = pShell->m_ResultForce[1].GetAt(nLoadCase)->m_dForce[nShellForce];
		dValueEleMax[2] = pShell->m_ResultForce[2].GetAt(nLoadCase)->m_dForce[nShellForce];
		dValueEleMax[3] = pShell->m_ResultForce[3].GetAt(nLoadCase)->m_dForce[nShellForce];
		dValueEleMax[4] = pShell->m_ResultForce[4].GetAt(nLoadCase)->m_dForce[nShellForce]; // cent
		//
		dValueEleMin[0] = pShell->m_ResultForce[0].GetAt(nLoadCase+1)->m_dForce[nShellForce];
		dValueEleMin[1] = pShell->m_ResultForce[1].GetAt(nLoadCase+1)->m_dForce[nShellForce];
		dValueEleMin[2] = pShell->m_ResultForce[2].GetAt(nLoadCase+1)->m_dForce[nShellForce];
		dValueEleMin[3] = pShell->m_ResultForce[3].GetAt(nLoadCase+1)->m_dForce[nShellForce];
		dValueEleMin[4] = pShell->m_ResultForce[4].GetAt(nLoadCase+1)->m_dForce[nShellForce]; // cent

		if(m_nSelect3DOutputValue==0) // 평균값
		{
			if(pShell2 && !bEmptyS2)
			{
				dValueEleMax[0] = pShell2->m_ResultForce[0].GetAt(nLoadCase)->m_dForce[nShellForce];
				dValueEleMax[1] = pShell2->m_ResultForce[1].GetAt(nLoadCase)->m_dForce[nShellForce];
				dValueEleMax[2] = pShell2->m_ResultForce[2].GetAt(nLoadCase)->m_dForce[nShellForce];
				dValueEleMax[3] = pShell2->m_ResultForce[3].GetAt(nLoadCase)->m_dForce[nShellForce];
				//
				dValueEleMin[0] = pShell2->m_ResultForce[0].GetAt(nLoadCase+1)->m_dForce[nShellForce];
				dValueEleMin[1] = pShell2->m_ResultForce[1].GetAt(nLoadCase+1)->m_dForce[nShellForce];
				dValueEleMin[2] = pShell2->m_ResultForce[2].GetAt(nLoadCase+1)->m_dForce[nShellForce];
				dValueEleMin[3] = pShell2->m_ResultForce[3].GetAt(nLoadCase+1)->m_dForce[nShellForce];
				
				// 삼각형부재일경우 값이 있는 세점을 평균한다.
				if(pShell2->m_nJ[2]==pShell2->m_nJ[3])
				{
					dValueMax = (dValueEleMax[0]+dValueEleMax[1]+dValueEleMax[2]+dValueEleMax[3])/3.0;
					dValueMin = (dValueEleMin[0]+dValueEleMin[1]+dValueEleMin[2]+dValueEleMin[3])/3.0;
				}
				else
				{
					dValueMax = (dValueEleMax[0]+dValueEleMax[1]+dValueEleMax[2]+dValueEleMax[3])/4.0;
					dValueMin = (dValueEleMin[0]+dValueEleMin[1]+dValueEleMin[2]+dValueEleMin[3])/4.0;
				}
			}
			else
			{
				// 삼각형부재일경우 값이 있는 세점을 평균한다.
				if(pShell->m_nJ[2]==pShell->m_nJ[3])
				{
					dValueMax = (dValueEleMax[0]+dValueEleMax[1]+dValueEleMax[2]+dValueEleMax[3])/3.0;
					dValueMin = (dValueEleMin[0]+dValueEleMin[1]+dValueEleMin[2]+dValueEleMin[3])/3.0;
				}
				else
				{
					dValueMax = (dValueEleMax[0]+dValueEleMax[1]+dValueEleMax[2]+dValueEleMax[3])/4.0;
					dValueMin = (dValueEleMin[0]+dValueEleMin[1]+dValueEleMin[2]+dValueEleMin[3])/4.0;
				}
			}
			
		}
		else if(m_nSelect3DOutputValue==1) // 최대값
		{
			dValueMax = max(dValueEleMax[0],max(dValueEleMax[1],max(dValueEleMax[2],dValueEleMax[3])));
			dValueMin = min(dValueEleMin[0],min(dValueEleMin[1],min(dValueEleMin[2],dValueEleMin[3])));
			if(pShell2 && !bEmptyS2)
			{
				nJointNear[0] = (nPos==CHECK_POS_LEFT) ? 0 : 1;
				nJointNear[1] = (nPos==CHECK_POS_LEFT) ? 3 : 2;
				double dValueEleMaxNear[2] = {0,0};
				double dValueEleMinNear[2] = {0,0};
				dValueEleMaxNear[0] = pShell2->m_ResultForce[nJointNear[0]].GetAt(nLoadCase)->m_dForce[nShellForce];
				dValueEleMaxNear[1] = pShell2->m_ResultForce[nJointNear[1]].GetAt(nLoadCase)->m_dForce[nShellForce];
				dValueEleMinNear[0] = pShell2->m_ResultForce[nJointNear[0]].GetAt(nLoadCase+1)->m_dForce[nShellForce];
				dValueEleMinNear[1] = pShell2->m_ResultForce[nJointNear[1]].GetAt(nLoadCase+1)->m_dForce[nShellForce];
				dValueMax = max(dValueMax,max(dValueEleMaxNear[0],dValueEleMaxNear[1]));
				dValueMin = min(dValueMin,min(dValueEleMinNear[0],dValueEleMinNear[1]));
			}
		}
		else if(m_nSelect3DOutputValue==2) // cent값
		{
			if(pShell2 && !bEmptyS2)
			{
				dValueEleMax[4] = pShell2->m_ResultForce[4].GetAt(nLoadCase)->m_dForce[nShellForce];  // cent
				dValueEleMin[4] = pShell2->m_ResultForce[4].GetAt(nLoadCase+1)->m_dForce[nShellForce]; // cent
			}
			dValueMax = dValueEleMax[4];
			dValueMin = dValueEleMin[4];
		}


		if(bFirstCHeck || dOutputMax<dValueMax)
		{
			idShell = pShell->m_nIndex;
			dOutputMax = dValueMax;
		}
		if(bFirstCHeck || dOutputMin>dValueMin)	dOutputMin = dValueMin;

		bFirstCHeck = FALSE;
	}

	// 하부 슬래브는 모멘트방향이 반대로 적용
	double dValueReturn = bUpperSlab? dOutputMax : dOutputMin;
	if(nTypeForce==ELE_MOMENT2 && nPos!=CHECK_POS_CENTER)
	{
		dValueReturn = bUpperSlab? dOutputMin : dOutputMax;
	}
	if(nTypeForce==ELE_SHEAR_2)
	{
		dValueReturn = max(fabs(dOutputMax),fabs(dOutputMin));
	}

	return dValueReturn;
}

double CRcBridgeApp::GetOutValuePRFShell( BOOL bStt, long nTypeDesign, long nTypeForce, ETypeEnvComboLsd eEnvTypeLsd/*=eEnvLimit*/ )
{
	if(nTypeForce!=ELE_MOMENT2 && nTypeForce!=ELE_SHEAR_2) return 0;
	
	BOOL bLsd = m_pARcBridgeDataStd->IsLsdDesign();
	long nPos = bStt ? CHECK_POS_LEFT : CHECK_POS_RIGHT;

	if(m_bUseShellUserOutput)
	{
		long nSizeRow = m_MatShellPRF.GetRow();
		long nSizeCol = m_MatShellPRF.GetCol();

		long nRow = bStt ? 0 : 1;
		long nCol = 0;
		if(bLsd)
		{
			long nColBase = 0;
			for(long ix=0; ix<eCountEnvTypeLsd; ++ix)
			{
				ETypeEnvComboLsd eEnvType = (ETypeEnvComboLsd)ix;
				if(IsValidComboEnvType(eEnvType) == FALSE)
					continue;

				if(eEnvType == eEnvTypeLsd)
				{
					break;
				}
				nColBase += 3;
			}
			if(nTypeForce==ELE_MOMENT2) nCol = nColBase;
			if(nTypeForce==ELE_SHEAR_2) nCol = nColBase + 1;
			if(nTypeForce==ELE_AXFORCE) nCol = nColBase + 2; // 실제로는 값이 없음...
		}
		else
		{
			if(nTypeDesign==COMBO_SERVICE)
			{
				if(nTypeForce==ELE_MOMENT2) nCol = 3;
				if(nTypeForce==ELE_SHEAR_2) nCol = 4;
			}
			else
			{
				if(nTypeForce==ELE_MOMENT2) nCol = 0;
				if(nTypeForce==ELE_SHEAR_2) nCol = 1;
				if(nTypeForce==ELE_AXFORCE) nCol = 2; // 실제로는 값이 없음...
			}
		}

		if(nRow>=nSizeRow || nCol>=nSizeCol) return 0;

		return m_MatShellPRF(nRow,nCol);
	}

	CFEMManage *pFEM = &m_FemShell;
	if(nTypeDesign==COMBO_EARTHQUAKE2 || nTypeDesign==COMBO_EARTHQUAKE) return 0;
	if(pFEM->IsOutputLoaded()==FALSE) return 0;

	long nShellForce = (nTypeForce==ELE_MOMENT2) ? MOMENT_M11 : VER_13;

	double dOutputMax = 0;
	double dOutputMin = 0;
	double dValueMax  = 0;
	double dValueMin  = 0;
	double dValueEleMax[5] = {0,0,0,0,0};
	double dValueEleMin[5] = {0,0,0,0,0};

	CString szCombo = GetStringComboEnv(nTypeDesign, eEnvTypeLsd, ePartLowerSlab, nPos);

	BOOL bFirstCHeck = TRUE;

	long nLoadCase  = pFEM->GetLoadCaseIndex(szCombo);
	if(nLoadCase==-1) return 0;

	long idShell = 0;
	long nSizeShell = pFEM->GetShellSize();
	long nIndexNear = -1;
	long nJointNear[2] = {0,0};
	for(long nShell=0; nShell<nSizeShell; nShell++)
	{
		CShell *pShell  = pFEM->GetShell((USHORT)nShell);
		CShell *pShell2 = NULL; // 끝단부재력 구하는 경우에 입접한 부재의 값도 같이 비교...

		if(pShell->m_nAttrib != SHELL_ELEMENT_LOWERSLAB) continue;
		if(pShell->m_LongAttrib[LONGSHELL_1_JIJUMNO] >= 0) continue;

		if(bStt && pShell->m_LongAttrib[LONGSHELL_1_JIJUMNO] != -1) continue;
		if(bStt == FALSE && pShell->m_LongAttrib[LONGSHELL_1_JIJUMNO] == -1) continue;


		BOOL bEmptyS1 = FALSE;
		for(long nR=0; nR<5; nR++)
		{
			if(pShell->m_ResultForce[nR].GetSize()==0)
				bEmptyS1 = TRUE;
		}
		if(bEmptyS1) continue;

		dValueEleMax[0] = pShell->m_ResultForce[0].GetAt(nLoadCase)->m_dForce[nShellForce];
		dValueEleMax[1] = pShell->m_ResultForce[1].GetAt(nLoadCase)->m_dForce[nShellForce];
		dValueEleMax[2] = pShell->m_ResultForce[2].GetAt(nLoadCase)->m_dForce[nShellForce];
		dValueEleMax[3] = pShell->m_ResultForce[3].GetAt(nLoadCase)->m_dForce[nShellForce];
		dValueEleMax[4] = pShell->m_ResultForce[4].GetAt(nLoadCase)->m_dForce[nShellForce]; // cent
		//
		dValueEleMin[0] = pShell->m_ResultForce[0].GetAt(nLoadCase+1)->m_dForce[nShellForce];
		dValueEleMin[1] = pShell->m_ResultForce[1].GetAt(nLoadCase+1)->m_dForce[nShellForce];
		dValueEleMin[2] = pShell->m_ResultForce[2].GetAt(nLoadCase+1)->m_dForce[nShellForce];
		dValueEleMin[3] = pShell->m_ResultForce[3].GetAt(nLoadCase+1)->m_dForce[nShellForce];
		dValueEleMin[4] = pShell->m_ResultForce[4].GetAt(nLoadCase+1)->m_dForce[nShellForce]; // cent

		if(m_nSelect3DOutputValue==0) // 평균값
		{
			// 삼각형부재일경우 값이 있는 세점을 평균한다.
			if(pShell->m_nJ[2]==pShell->m_nJ[3])
			{
				dValueMax = (dValueEleMax[0]+dValueEleMax[1]+dValueEleMax[2]+dValueEleMax[3])/3.0;
				dValueMin = (dValueEleMin[0]+dValueEleMin[1]+dValueEleMin[2]+dValueEleMin[3])/3.0;
			}
			else
			{
				dValueMax = (dValueEleMax[0]+dValueEleMax[1]+dValueEleMax[2]+dValueEleMax[3])/4.0;
				dValueMin = (dValueEleMin[0]+dValueEleMin[1]+dValueEleMin[2]+dValueEleMin[3])/4.0;
			}
		}
		else if(m_nSelect3DOutputValue==1) // 최대값
		{
			dValueMax = max(dValueEleMax[0],max(dValueEleMax[1],max(dValueEleMax[2],dValueEleMax[3])));
			dValueMin = min(dValueEleMin[0],min(dValueEleMin[1],min(dValueEleMin[2],dValueEleMin[3])));			
		}
		else if(m_nSelect3DOutputValue==2) // cent값
		{			
			dValueMax = dValueEleMax[4];
			dValueMin = dValueEleMin[4];
		}
		
		if(bFirstCHeck || dOutputMax<dValueMax)
		{
			idShell = pShell->m_nIndex;
			dOutputMax = dValueMax;
		}
		if(bFirstCHeck || dOutputMin>dValueMin)	dOutputMin = dValueMin;

		bFirstCHeck = FALSE;
	}

	double dValueReturn = dOutputMin;	
	if(nTypeForce==ELE_SHEAR_2)
	{
		dValueReturn = max(fabs(dOutputMax),fabs(dOutputMin));
	}

	return dValueReturn;
}

// 사용자 입력인 경우 col - M,V,P,M,V (계수하중,사용하중)
// 사용자 입력인 경우 row - 기둥 Index)
double CRcBridgeApp::GetOutValueWallShellColumn(long nJijum, long nColumn, long nPos, long nTypeDesign, long nTypeForce, BOOL bGirderCalc, BOOL bOutSide, ETypeEnvComboLsd eEnvTypeLsd)
{
	CFEMManage *pFEM = &m_FemShell;
	if(nTypeDesign==COMBO_EARTHQUAKE2 || nTypeDesign==COMBO_EARTHQUAKE) return 0;

	long nTypeColumn = GetTypeColumnByJijum(nJijum);
	if(nTypeColumn==-1) return 0;

	if(m_bUseShellUserOutput)
	{
		BOOL bLsd = m_pARcBridgeDataStd->IsLsdDesign();
		CMatrix Mat = m_MatShellWallMid[nJijum-1];
		long nSizeRow = Mat.GetRow();
		long nSizeCol = Mat.GetCol();
		if(nSizeCol==0 || nSizeRow==0) return 0;
		long nRow = nColumn;
		long nCol = 0;
		if(bLsd)
		{
			long nColBase = 0;
			for(long ix=0; ix<eCountEnvTypeLsd; ++ix)
			{
				ETypeEnvComboLsd eEnvType = (ETypeEnvComboLsd)ix;
				if(IsValidComboEnvType(eEnvType) == FALSE)
					continue;

				if(eEnvType == eEnvTypeLsd)
				{
					break;
				}
				nColBase += 3;
			}
			if(nTypeForce==ELE_MOMENT2) nCol = nColBase;
			if(nTypeForce==ELE_SHEAR_2) nCol = nColBase + 1;
			if(nTypeForce==ELE_AXFORCE) nCol = nColBase + 2;
		}
		else
		{
			if(nTypeDesign==COMBO_SERVICE)
			{
				if(nTypeForce==ELE_MOMENT2) nCol = 3;
				if(nTypeForce==ELE_SHEAR_2) nCol = 4;
			}
			else
			{
				if(nTypeForce==ELE_MOMENT2) nCol = 0;
				if(nTypeForce==ELE_SHEAR_2) nCol = 1;
				if(nTypeForce==ELE_AXFORCE) nCol = 2;
			}
		}
		if(nRow>=nSizeRow || nCol>=nSizeCol) return 0;
		return Mat(nRow,nCol);
	}
	//////////////////////////////////////////////////////////////////////////
	if(pFEM->IsOutputLoaded()==FALSE) return 0;

	double dOutputMax = 0;
	double dOutputMin = 0;
	long nIdxElement  = -1;

	CElement *pEle = NULL;
	
	POSITION pos = pFEM->m_ElementList.GetHeadPosition();
	while(pos)
	{		
		pEle = (CElement *)pFEM->m_ElementList.GetNext(pos);
		long nEle = pEle->m_nIndex;				
		if(pEle->m_LongAttrib[LONG_1_ELEMENT_JIJUMNO]==nJijum && 
		   pEle->m_LongAttrib[LONG_2_ELEMENT_COLUMN_INDEX]==nColumn)
		{
			nIdxElement = nEle;
			break;
		}
	}
	if(nIdxElement==-1) return 0;

	CString szCombo = GetStringComboEnv(nTypeDesign, eEnvTypeLsd, ePartInWall, nPos);

	long nLoadCase  = pFEM->GetLoadCaseIndex(szCombo);	
	if(nLoadCase<0) return dOutputMax;

	CResultForceElement *pResultMax = NULL;
	CResultForceElement *pResultMin = NULL;
	// 하나의 부재에 대하여 부재력을 산출한다...
	if(nPos==CHECK_POS_TOP || nPos==CHECK_POS_BOTTOM)
	{
		BOOL bSttPos = (nPos==0) ? TRUE : FALSE;
		pEle = pFEM->GetElement((USHORT)nIdxElement);
		pResultMax = pEle->GetResultForce(nLoadCase);
		pResultMin = pEle->GetResultForce(nLoadCase+1);

		if(pResultMax && pResultMin)
		{
			long nPoint  = bSttPos ? 0 : pResultMax->m_ResultForceElement.GetSize()-1;
			dOutputMax   = pResultMax->m_ResultForceElement[nPoint]->m_dForceOnElement[nTypeForce];
			dOutputMin   = pResultMin->m_ResultForceElement[nPoint]->m_dForceOnElement[nTypeForce];
		}
	}

	double dValueReturn = max(fabs(dOutputMax),fabs(dOutputMin));

//	double Station   = GetStationOnJijum(0,0);
//	double SlabWidth = toM(GetWidthSlabAct(Station,CDPoint(0,1)));	// 슬래브폭원(교량폭,벽체폭)
//	double dDivide = (nTypeDesign>=COMBO_EARTHQUAKE) ? SlabWidth : 1.00;
//	return dValueReturn/dDivide;

	return dValueReturn;
}

// nIdxArea : A,B,C,D,A'
double CRcBridgeApp::GetOutValueWingWallShell(long nIdxArea, BOOL bStt, BOOL bLeft, BOOL bMoment, BOOL bUse, BOOL bJudge)
{
	CFEMManage *pFEM = &m_FemWingWall[bStt ? 0 : 1][bLeft ? 0 : 1];

	long nShellForce = (nIdxArea != 2) ? (bMoment ? MOMENT_M11 : VER_13) : (bMoment ? MOMENT_M22 : VER_23); // nIdxArea = 2 (C구간)
	long nCheckArea = nIdxArea == 2 ? nIdxArea - 1 : nIdxArea; // C구간은 B구간과 같은 Shell, 모멘트 방향만 다르게 설정

	double dValue  = 0;
	double dVal[5] = {0,0,0,0,0};

	CString szCombo = bUse ? (bJudge? _T("COMB3") : _T("COMB2")) : _T("COMB1");

	long nLoadCase  = pFEM->GetLoadCaseIndex(szCombo);
	if(nLoadCase==-1) return 0;

	long nCountMaxValue = m_nApplyWingWallCalcValueCount + 1;
	CDoubleArray dArrMax;
	long nSizeShell = pFEM->GetShellSize();
	double dReturnValue = 0;
	for(long nShell=0; nShell<nSizeShell; nShell++)
	{
		CShell *pShell = pFEM->GetShell((USHORT)nShell);
		if(pShell->m_LongAttrib[LONGSHELL_0_AREA_POS] != nCheckArea) continue;

		// (ARCBRIDGE-3396) +/- 에따라 집계가 잘못되고 있다. 절대값 처리.
		dVal[0] = fabs(pShell->m_ResultForce[0].GetAt(nLoadCase)->m_dForce[nShellForce]);
		dVal[1] = fabs(pShell->m_ResultForce[1].GetAt(nLoadCase)->m_dForce[nShellForce]);
		dVal[2] = fabs(pShell->m_ResultForce[2].GetAt(nLoadCase)->m_dForce[nShellForce]);
		dVal[3] = fabs(pShell->m_ResultForce[3].GetAt(nLoadCase)->m_dForce[nShellForce]);
		dVal[4] = fabs(pShell->m_ResultForce[4].GetAt(nLoadCase)->m_dForce[nShellForce]);

		if(m_nSelect3DOutputValue==0)
		{
			dValue = (dVal[0] + dVal[1] + dVal[2] + dVal[3]) / 4.0;
		}
		else if(m_nSelect3DOutputValue==1)
		{
			dValue = max(dVal[0],max(dVal[1],max(dVal[2],dVal[3])));
		}
		else if(m_nSelect3DOutputValue==2) // cent값
		{
			if(Compare(dVal[4], 0.0, _T("="))) 
				dValue = (dVal[0] + dVal[1] + dVal[2] + dVal[3]) / 4.0;
			else
				dValue = dVal[4];
		}

		if(nCountMaxValue > 1)
			dArrMax.Add(dValue);
		else
			dReturnValue = MAX(fabs(dValue), dReturnValue);
	}

	if(nCountMaxValue > 1 && dArrMax.GetSize() > 0)
	{
		dArrMax.Sort(FALSE);
		double dValueSum = 0;
		double dValueShell = 0;
		for(long ix = 0; ix < nCountMaxValue; ix++)
		{
			dValueShell = dArrMax.GetAt(ix);
			dValueSum += dValueShell;
		}

		dReturnValue = dValueSum / nCountMaxValue;
	}

	return dReturnValue;
}

void CRcBridgeApp::SetWingWallShellOutput(BOOL bStt, BOOL bLeft)
{
	CWingWall *pWing = GetWingWall(bStt, bLeft);
	if(pWing->m_bUseUserOutput3D) return;

	CFEMManage *pFEM = &m_FemWingWall[bStt ? 0 : 1][bLeft ? 0 : 1];
	if(pFEM->IsOutputLoaded() == FALSE) return;

	for(long nArea = 0; nArea < 5; nArea++) // A,B,C,D,A'
	{
		pWing->m_dMomentUltPlate[nArea] = GetOutValueWingWallShell(nArea, bStt, bLeft, TRUE, FALSE);
		pWing->m_dMomentUsePlate[nArea] = GetOutValueWingWallShell(nArea, bStt, bLeft, TRUE, TRUE);
		pWing->m_dMomentUseJudge_Plate[nArea] = GetOutValueWingWallShell(nArea, bStt, bLeft, TRUE, TRUE, TRUE);
		pWing->m_dShearUltPlate[nArea] = GetOutValueWingWallShell(nArea, bStt, bLeft, FALSE, FALSE);
		pWing->m_dShearUsePlate[nArea] = GetOutValueWingWallShell(nArea, bStt, bLeft, FALSE, TRUE);
	}
}


// 사용자 입력인 경우 col - M,V,P,M,V (계수하중,사용하중)
// 사용자 입력인 경우 row - 벽체상단,중앙부(배/전),하단부(배/전)
// 사용자 입력인 경우 row - 벽체하단 : 중간지점
// bOutSide = TRUE : 배면
double CRcBridgeApp::GetOutValueWallShell(long nJijum, long nPos, long nTypeDesign, long nTypeForce, BOOL bGirderCalc, BOOL bOutSide, ETypeEnvComboLsd eEnvTypeLsd)
{
	if(nTypeForce!=ELE_MOMENT2 && nTypeForce!=ELE_SHEAR_2) return 0;
	if(nPos==CHECK_POS_CENTER && nTypeForce==ELE_SHEAR_2) return 0;

	long nTypeColumn = GetTypeColumnByJijum(nJijum);
	if(nTypeColumn!=-1)
	{
		long nColumn = 0;
		return GetOutValueWallShellColumn(nJijum, nColumn, nPos, nTypeDesign, nTypeForce, bGirderCalc, bOutSide, eEnvTypeLsd);
	}

	if(m_bUseShellUserOutput)
	{
		BOOL bLsd = m_pARcBridgeDataStd->IsLsdDesign();
		CMatrix Mat;
		if(nJijum==0)
		{
			Mat = m_MatShellWallStt;
		}
		else if(nJijum==m_nQtyJigan)
		{
			Mat = m_MatShellWallEnd;
		}
		else
		{
			Mat = m_MatShellWallMid[nJijum-1];
		}
		long nSizeRow = Mat.GetRow();
		long nSizeCol = Mat.GetCol();
		if(nSizeCol==0 || nSizeRow==0) return 0;
		long nRow = 0;
		long nCol = 0;
		if(nJijum==0 || nJijum==m_nQtyJigan)
		{
			if(nPos==CHECK_POS_CENTER && bOutSide==TRUE)   nRow = 1;
			if(nPos==CHECK_POS_CENTER && bOutSide==FALSE)  nRow = 2;
			if(nPos==CHECK_POS_BOTTOM && bOutSide==TRUE)   nRow = 3;
			if(nPos==CHECK_POS_BOTTOM && bOutSide==FALSE)  nRow = 4;
		}
		if(bLsd)
		{
			long nColBase = 0;
			for(long ix=0; ix<eCountEnvTypeLsd; ++ix)
			{
				ETypeEnvComboLsd eEnvType = (ETypeEnvComboLsd)ix;
				if(IsValidComboEnvType(eEnvType) == FALSE)
					continue;

				if(eEnvType == eEnvTypeLsd)
				{
					break;
				}
				nColBase += 3;
			}
			if(nTypeForce==ELE_MOMENT2) nCol = nColBase;
			if(nTypeForce==ELE_SHEAR_2) nCol = nColBase + 1;
			if(nTypeForce==ELE_AXFORCE) nCol = nColBase + 2; // 실제로는 값이 없음...
		}
		else
		{
			if(nTypeDesign==COMBO_SERVICE)
			{
				if(nTypeForce==ELE_MOMENT2) nCol = 3;
				if(nTypeForce==ELE_SHEAR_2) nCol = 4;
			}
			else
			{
				if(nTypeForce==ELE_MOMENT2) nCol = 0;
				if(nTypeForce==ELE_SHEAR_2) nCol = 1;
				if(nTypeForce==ELE_AXFORCE) nCol = 2;
			}
		}
		
		if(nRow>=nSizeRow || nCol>=nSizeCol) return 0;
		return Mat(nRow,nCol);
	}
	//////////////////////////////////////////////////////////////////////////
	CFEMManage *pFEM = &m_FemShell;
	if(nTypeDesign==COMBO_EARTHQUAKE2 || nTypeDesign==COMBO_EARTHQUAKE) return 0;
	if(pFEM->IsOutputLoaded()==FALSE) return 0;

	long nShellForce = (nTypeForce==ELE_MOMENT2) ? MOMENT_M11 : VER_13;

	double dOutputMax = 0;
	double dOutputMin = 0;
	double dValueMax  = 0;
	double dValueMin  = 0;
	double dValueEleMax[5] = {0,0,0,0,0};
	double dValueEleMin[5] = {0,0,0,0,0};

	EPartOfBridge ePart(ePartInWall);
	if(nJijum == 0)
		ePart = ePartOutWallStt;
	else if(nJijum == m_nQtyJigan)
		ePart = ePartOutWallEnd;

	CString szCombo = GetStringComboEnv(nTypeDesign, eEnvTypeLsd, ePart, nPos);
	
	BOOL bFirstCHeck = TRUE;

	long nLoadCase  = pFEM->GetLoadCaseIndex(szCombo);
	if(nLoadCase==-1) return 0;

	long nSizeShell = pFEM->GetShellSize();
	long nJointNear[2] = {0,0};
	long nIndexNear = -1;
	for(long nShell=0; nShell<nSizeShell; nShell++)
	{
		CShell *pShell = pFEM->GetShell((USHORT)nShell);
		CShell *pShell2 = NULL; // 끝단부재력 구하는 경우에 입접한 부재의 값도 같이 비교...

		if(pShell->m_nAttrib==SHELL_ELEMENT_UPPERSLAB || pShell->m_nAttrib==SHELL_ELEMENT_LOWERSLAB) continue;
		if(pShell->m_LongAttrib[LONGSHELL_1_JIJUMNO]!=nJijum) continue;
		if(nPos==CHECK_POS_TOP)
		{
			if(pShell->m_BoolAttrib[BOOLSHELL_0_CHECKPOS_STT]==FALSE) continue;
			if(m_nSelect3DOutputValue==1)
			{
				nIndexNear = GetIndexShellNearByJointNumber(pShell,pShell->m_nJ[0],pShell->m_nJ[1]);
				pShell2 = (nIndexNear>=0) ? pFEM->GetShell((USHORT)nIndexNear) : NULL; // 위에 붙은 부재..
			}
		}
		if(nPos==CHECK_POS_CENTER && pShell->m_BoolAttrib[BOOLSHELL_2_CHECKPOS_MID]==FALSE) continue;
		if(nPos==CHECK_POS_BOTTOM && pShell->m_BoolAttrib[BOOLSHELL_1_CHECKPOS_END]==FALSE) continue;
		
		dValueEleMax[0] = pShell->m_ResultForce[0].GetAt(nLoadCase)->m_dForce[nShellForce];
		dValueEleMax[1] = pShell->m_ResultForce[1].GetAt(nLoadCase)->m_dForce[nShellForce];
		dValueEleMax[2] = pShell->m_ResultForce[2].GetAt(nLoadCase)->m_dForce[nShellForce];
		dValueEleMax[3] = pShell->m_ResultForce[3].GetAt(nLoadCase)->m_dForce[nShellForce];
		dValueEleMax[4] = pShell->m_ResultForce[4].GetAt(nLoadCase)->m_dForce[nShellForce];
		//
		dValueEleMin[0] = pShell->m_ResultForce[0].GetAt(nLoadCase+1)->m_dForce[nShellForce];
		dValueEleMin[1] = pShell->m_ResultForce[1].GetAt(nLoadCase+1)->m_dForce[nShellForce];
		dValueEleMin[2] = pShell->m_ResultForce[2].GetAt(nLoadCase+1)->m_dForce[nShellForce];
		dValueEleMin[3] = pShell->m_ResultForce[3].GetAt(nLoadCase+1)->m_dForce[nShellForce];
		dValueEleMin[4] = pShell->m_ResultForce[4].GetAt(nLoadCase+1)->m_dForce[nShellForce];

		if(m_nSelect3DOutputValue==0)
		{
			dValueMax = (dValueEleMax[0]+dValueEleMax[1]+dValueEleMax[2]+dValueEleMax[3])/4.0;
			dValueMin = (dValueEleMin[0]+dValueEleMin[1]+dValueEleMin[2]+dValueEleMin[3])/4.0;
		}
		else if(m_nSelect3DOutputValue==1)
		{
			dValueMax = max(dValueEleMax[0],max(dValueEleMax[1],max(dValueEleMax[2],dValueEleMax[3])));
			dValueMin = min(dValueEleMin[0],min(dValueEleMin[1],min(dValueEleMin[2],dValueEleMin[3])));
			if(pShell2)
			{
				nJointNear[0] = 3;
				nJointNear[1] = 2;
				double dValueEleMaxNear[2] = {0,0};
				double dValueEleMinNear[2] = {0,0};
				dValueEleMaxNear[0] = pShell2->m_ResultForce[nJointNear[0]].GetAt(nLoadCase)->m_dForce[nShellForce];
				dValueEleMaxNear[1] = pShell2->m_ResultForce[nJointNear[1]].GetAt(nLoadCase)->m_dForce[nShellForce];
				dValueEleMinNear[0] = pShell2->m_ResultForce[nJointNear[0]].GetAt(nLoadCase+1)->m_dForce[nShellForce];
				dValueEleMinNear[1] = pShell2->m_ResultForce[nJointNear[1]].GetAt(nLoadCase+1)->m_dForce[nShellForce];
				dValueMax = max(dValueMax,max(dValueEleMaxNear[0],dValueEleMaxNear[1]));
				dValueMin = min(dValueMin,max(dValueEleMinNear[0],dValueEleMinNear[1]));
			}
		}
		else if(m_nSelect3DOutputValue==2) // cent값
		{
			if(pShell2)
			{
				dValueEleMax[4] = pShell2->m_ResultForce[4].GetAt(nLoadCase)->m_dForce[nShellForce];  // cent
				dValueEleMin[4] = pShell2->m_ResultForce[4].GetAt(nLoadCase+1)->m_dForce[nShellForce]; // cent
			}
			dValueMax = dValueEleMax[4];
			dValueMin = dValueEleMin[4];
		}

		if(bFirstCHeck || dOutputMax<dValueMax)	dOutputMax = dValueMax;
		if(bFirstCHeck || dOutputMin>dValueMin)	dOutputMin = dValueMin;

		bFirstCHeck = FALSE;
	}

	double dValueReturn = dOutputMax;
	// 외측벽체의 모멘트
	if(nTypeForce==ELE_MOMENT2 && (nJijum==0 || nJijum==m_nQtyJigan))
	{
		if(nPos==CHECK_POS_TOP)		dValueReturn = dOutputMin;
		if(nPos==CHECK_POS_CENTER)	dValueReturn = bOutSide ? dOutputMin : dOutputMax;
		if(nPos==CHECK_POS_BOTTOM)	dValueReturn = bOutSide ? dOutputMin : dOutputMax;
	}
	// 전단력,축력,중간벽체(ALL)
	if(nTypeForce==ELE_SHEAR_2 || nTypeForce==ELE_AXFORCE || (nJijum>0 && nJijum<m_nQtyJigan))
	{
		if((nJijum==0 || nJijum==m_nQtyJigan) && nTypeForce==ELE_AXFORCE && nPos==CHECK_POS_CENTER)
		{
			dValueReturn = fabs(dOutputMax);
		}
		else
		{
			dValueReturn = max(fabs(dOutputMax),fabs(dOutputMin));
		}
	}

	return dValueReturn;
}

BOOL CRcBridgeApp::IsCalc3D(long nTypeLoad)
{
	// (ARCBRIDGE-3481) 해석을 선택 하지 않으면 사용자입력 값도 적용하지 않도록 변경
	if(m_nSelect3DShellTool == 2)
		return FALSE;

	if(m_pARcBridgeDataStd->IsLsdDesign())
		return TRUE;

	BOOL b3D = FALSE;
	long nSizeRow = 0;
	if(nTypeLoad == COMBO_SERVICE) nSizeRow = m_MatComboUse.GetRow();
	else if(nTypeLoad == COMBO_ULTIMATE) nSizeRow = m_MatComboUlt.GetRow();
	else if(nTypeLoad == COMBO_ECCENTRIC) nSizeRow = m_MatComboEcc.GetRow();
	else if(nTypeLoad == COMBO_EARTHQUAKE) nSizeRow = m_MatComboEQ.GetRow();

	for(long nRow=0; nRow<nSizeRow; nRow++)
	{
		if(m_bApplyCombo3D[nTypeLoad][nRow]) b3D = TRUE;
	}

	return b3D;
}

// 가각부 주철근 각도 리턴
// R이 없는 상태에서의 가각의 각도로 들어감.
CDPoint CRcBridgeApp::GetXyDirExpForRebar(BOOL bStt, BOOL bLeft)
{
	double dOldExpSttR[2]	= { m_dExpSttR[0], m_dExpSttR[1] };
	double dOldExpEndR[2]	= { m_dExpEndR[0], m_dExpEndR[1] };

	m_dExpSttR[0]	= 0;
	m_dExpSttR[1]	= 0;
	m_dExpEndR[0]	= 0;
	m_dExpEndR[1]	= 0;

	CTwinVectorArray tvArrPlane;
	GetTvRebarOut_Plane_UpperSlabStd(tvArrPlane, 0, TRUE, 2, TRUE, FALSE);
	
	CTwinVector tv, tvExp;
	CString sInfo	= _T("");
	sInfo.Format("%s%s가각부", bStt ? "시점" : "종점", bLeft ? "좌측" : "우측");
	long i = 0; for(i = 0; i < tvArrPlane.GetSize(); i++)
	{
		tv	= tvArrPlane.GetAt(i);
		if(tv.m_sInfo.Find(sInfo, 0) != -1)
		{
			tvExp = tv;
			break;
		}
	}

	m_dExpSttR[0] = dOldExpSttR[0];
	m_dExpSttR[1] = dOldExpSttR[1];
	m_dExpEndR[0] = dOldExpEndR[0];
	m_dExpEndR[1] = dOldExpEndR[1];

	return tvExp.GetXyDir(bStt&&!bLeft || !bStt&&bLeft);
}

//<param name="bRoadNotSlab">TRUE=지중라멘의 상부 노면(도로면, 철도면)이며 슬래브면(교면)아님
//							FALSE=일반교량, 지중라멘의 슬래브면(교면)</param>
CDPoint CRcBridgeApp::GetXySlabHDanLeft(double sta, CDPoint vAng, long nHDan, BOOL bRoadNotSlab, BOOL bExp)
{
	CDPoint A[30];
	GetXySlabUpperAct(sta, vAng, A, bRoadNotSlab, bExp);
	long nHDanAct = GetNumSlabNodeAct(sta, vAng, nHDan, bRoadNotSlab, bExp);
	return A[nHDanAct];
}

//<param name="bRoadNotSlab">TRUE=지중라멘의 상부 노면(도로면, 철도면)이며 슬래브면(교면)아님
//							FALSE=일반교량, 지중라멘의 슬래브면(교면)</param>
CString CRcBridgeApp::GetStringTypeHDan(long nHDan, BOOL bWriteLeftRight, BOOL bRoadNotSlab)
{
	BOOL bRail = m_pARcBridgeDataStd->IsRailDesign();
	CString strType = GetStringTypeHDanByNum(GetTypeHDan(nHDan,bRoadNotSlab), bRail);

	CString strLeftRight = "";
	if(bWriteLeftRight)
	{
		if(IsLeftHDan(nHDan,bRoadNotSlab))
			strLeftRight = "(좌)";
		else
			strLeftRight = "(우)";
	}
	return strType + strLeftRight;
}

//<param name="bRoadNotSlab">TRUE=지중라멘의 상부 노면(도로면, 철도면)이며 슬래브면(교면)아님
//							FALSE=일반교량, 지중라멘의 슬래브면(교면)</param>
void CRcBridgeApp::SetTypeHDanByString(long nHDan, BOOL bWriteLeftRight, CString strType, BOOL bRoadNotSlab)
{

	CString str = strType;

	if(bWriteLeftRight)
	{
		if(strType =="방호벽(좌)")				str = "방호벽";
		else if(strType =="방호벽(우)")			str = "방호벽";
		else if(strType =="중분대1/2(좌)")		str = "중분대1/2";
		else if(strType =="중분대1/2(우)")		str = "중분대1/2";

	}
	BOOL bRail = m_pARcBridgeDataStd->IsRailDesign();
	long nHDanType = GetNumTypeHDanByString(str, bRail);
	if(bRoadNotSlab)
		SetTypeHDanRoad(nHDan, nHDanType);
	else
		SetTypeHDan(nHDan, nHDanType);

}
// 하중조합중에 같은 조합을 찾아서 삭제한다.
void CRcBridgeApp::SetArrangeCombine(long nTypeCombo)
{
	long nSizeCombo = 0;
	long nSizeRow   = 0;
	long nSizeCol   = BRIDGE_LOAD_BASIC;

	if(nTypeCombo==COMBO_SERVICE)	 nSizeRow = m_MatComboUse.GetRow();
	if(nTypeCombo==COMBO_ULTIMATE)	 nSizeRow = m_MatComboUlt.GetRow();
	if(nTypeCombo==COMBO_ECCENTRIC)  nSizeRow = m_MatComboEcc.GetRow();
	if(nTypeCombo==COMBO_EARTHQUAKE) nSizeRow = m_MatComboEQ.GetRow();

	CMatrix MatCombo(nSizeRow,nSizeCol);
	if(nTypeCombo==COMBO_SERVICE)	 { MatCombo = m_MatComboUse;  m_MatComboUse.Resize(0,0); }
	if(nTypeCombo==COMBO_ULTIMATE)	 { MatCombo = m_MatComboUlt;  m_MatComboUlt.Resize(0,0); }
	if(nTypeCombo==COMBO_ECCENTRIC)  { MatCombo = m_MatComboEcc;  m_MatComboEcc.Resize(0,0); }
	if(nTypeCombo==COMBO_EARTHQUAKE) { MatCombo = m_MatComboEQ;   m_MatComboEQ.Resize(0,0);  }


	CDoubleArray dArrComboIdx;
	// 자신의 조합이 이전에 같은 것이 없을 경우에만 적용
	long i=0; for(i=0; i<nSizeRow; i++)
	{
		BOOL bAdd = TRUE;
		for(long j=0; j<i; j++)
		{
			BOOL bSameCombo = IsSameMatrixRow(MatCombo,i,j);
			if(bSameCombo)
			{
				bAdd = FALSE;
				break;
			}
		}

		// (ARCBRIDGE-2646) 수평토압이 없고 연직토압만 있는 조합은 제거_XXXX
		// (ARCBRIDGE-2684) 편토압이 0일때 양토압 무시하고 중복조합 체크 하도록.
		//if(bAdd && MatCombo(i, BRIDGE_LOAD_SOIL) > 0 && MatCombo(i, BRIDGE_LOAD_TOAP_BOTH) == 0 && MatCombo(i, BRIDGE_LOAD_TOAP_ONE) == 0)
		//	bAdd = FALSE;

		if(bAdd)
		{
			dArrComboIdx.Add(i);
		}
	}

	nSizeCombo = dArrComboIdx.GetSize();
	CMatrix MatApply(nSizeCombo,nSizeCol);
	for(i=0; i<nSizeCombo; i++)
	{
		long nIdxCombo = (long)dArrComboIdx.GetAt(i);
		for(long j=0; j<nSizeCol; j++)
		{
			MatApply(i,j) = MatCombo(nIdxCombo,j);
		}
	}

	if(nTypeCombo==COMBO_SERVICE)	 m_MatComboUse = MatApply;
	if(nTypeCombo==COMBO_ULTIMATE)	 m_MatComboUlt = MatApply;
	if(nTypeCombo==COMBO_ECCENTRIC)  m_MatComboEcc = MatApply;
	if(nTypeCombo==COMBO_EARTHQUAKE) m_MatComboEQ  = MatApply;
}

//<summary>
//	일반교량 또는 지중라멘의 슬래브면에 대한 레벨 여부를 구함
//		지중라멘일 경우 - 입력 값 사용
//		지중라멘 아닐 경우 - 횡경사 선형 편경사 적용(m_bApplyLineSlope) 에 의하여 결정
//				m_bApplyLineSlope = TRUE 일 경우 레벨 입력값 사용
//				m_bApplyLineSlope = FALSE 일 경우 횡단 경사 입력값이 "0"인지 검토
//	지중라멘의 상부 노면(도로면, 철도면)에 대한 레벨 여부를 구함
//		횡경사 선형 편경사 적용(m_bApplyLineSlope) 에 의하여 결정
//				m_bApplyLineSlope = TRUE 일 경우 레벨 입력값 사용
//				m_bApplyLineSlope = FALSE 일 경우 횡단 경사 입력값이 "0"인지 검토
//</summary>
//<param name="nHDan"></param>
//<param name="bRoadNotSlab">TRUE=지중라멘의 상부 노면(도로면, 철도면)이며 슬래브면(교면)아님
//							FALSE=일반교량, 지중라멘의 슬래브면(교면)</param>
BOOL CRcBridgeApp::IsLevelHDanApplyLineSlope(long nHDan, BOOL bRoadNotSlab) const
{
	if(m_bApplyLineSlope)
	{
		if(bRoadNotSlab)//지중라멘교일 경우에는 슬래브면이 아닌 도로면
			return m_bLevelHDanRoad[nHDan];
		else
			return m_bLevelHDan[nHDan];
	}
	else
	{
		if(bRoadNotSlab)//지중라멘교일 경우에는 슬래브면이 아닌 도로면
		{
			if(m_dSlopeHDanRoad[nHDan] == 0)
				return TRUE;
		}
		else
		{
			if(m_dSlopeHDan[nHDan] == 0)
				return TRUE;
		}
	}

	return FALSE;
}
//<summary>
//  sta 에서의 nHDan 좌측 기준으로 한 경사를 구함.
//	일반교량 또는 지중라멘의 슬래브면에 대한 횡경사값을 구함
//		지중라멘일 경우 - 입력 값 사용
//		철도교일 경우 - 입력 값 사용(횡단 구간별 경사 있음)
//		도로교일 경우 - 선형 입력값 사용
//</summary>
//<param name="sta"></param>
//<param name="nHDan"></param>
//<param name="bRoadNotSlab">TRUE=지중라멘의 상부 노면(도로면, 철도면)이며 슬래브면(교면)아님
//							FALSE=일반교량, 지중라멘의 슬래브면(교면)</param>
// long nHDanCheckLevel : 레벨 체크를 하는 단(도로중심에걸칠때 경사는 다음단을 검사하고, 레벨은 이번단을 검사함)
double CRcBridgeApp::GetSlopeHDanByStation(double sta, long nHDan, BOOL bRoadNotSlab, long nHDanCheckLevel)
{
	if(nHDanCheckLevel == -1) nHDanCheckLevel = nHDan;
	if(IsLevelHDanApplyLineSlope(nHDanCheckLevel,bRoadNotSlab))	//입력값이 "0"이면 다른 사항에 관계없이 레벨로 처리
		return 0;

	if(m_bApplyLineSlope && ((m_bUnderGroundRahmen && bRoadNotSlab) || !m_bUnderGroundRahmen))
	{
		double disFrCenToHDan = GetDisSlabFrCenToHDanLeft(sta, nHDan,bRoadNotSlab);
//		double disSlabFrLeft = GetDisSlabLeftActByGagak(sta, CDPoint(0,1), bRoadNotSlab);
//		double disFrCenToHDan = disSlabFrLeft - disSlabFrLeftToHDan;

		return	GetLineBase()->GetPyungubae(sta, disFrCenToHDan);
	}
	else
	{
		return GetSlopeHDan(nHDan, bRoadNotSlab);
	}
}

BOOL CRcBridgeApp::IsSameMatrixRow(CMatrix Mat, long nRow1, long nRow2)
{
	long nSizeRow = Mat.GetRow();
	long nSizeCol = Mat.GetCol();

	if(nRow1>=nSizeRow || nRow2>=nSizeRow) return FALSE;
	
	//if(bAdd && MatCombo(i, BRIDGE_LOAD_SOIL) > 0 && MatCombo(i, BRIDGE_LOAD_TOAP_BOTH) == 0 && MatCombo(i, BRIDGE_LOAD_TOAP_ONE) == 0)

	long i=0; for(i=0; i<nSizeCol; i++)
	{
		double dValue1 = Mat(nRow1,i);
		double dValue2 = Mat(nRow2,i);

		double dValue11 = Mat(nRow1,0);
		double dValue12 = Mat(nRow1,1);
		double dValue13 = Mat(nRow1,2);
		double dValue14 = Mat(nRow1,3);
		double dValue15 = Mat(nRow1,4);
		double dValue16 = Mat(nRow1,5);

		// (ARCBRIDGE-2684) 편토압이 0이고 양토압도 0이면 무시하고 중복조합 체크 하도록.
		if(i == BRIDGE_LOAD_TOAP_BOTH && Mat(nRow1,BRIDGE_LOAD_TOAP_ONE) == 0 && dValue1 == 0)
			continue;

		if(Compare(dValue1,dValue2,"=")==FALSE) return FALSE;
	}

	return TRUE;
}

//<summary>
//횡단 슬래브, 벽체, 기초, 보도, 슬래브 하면, 방호벽 그리기
//</summary>
//<param name="sta">기준선형 스테이션</param>
//<param name="vAng">기준선형 각도</param>
//<param name="bInclFootway">보도 그리기</param>
//<param name="bInclSlabLower">슬래브 하면 그리기</param>
//<param name="bInclGuardWall">방호벽 그리기</param>
//<param name="nDrawLeft"></param>
//<param name="bMirror"></param>
//<param name="bInclPave">포장 그리기</param>
//<param name="bRoadNotSlab">TRUE=지중라멘의 상부 노면(도로면, 철도면)이며 슬래브면(교면)아님
//						     FALSE=일반교량, 지중라멘의 슬래브면(교면)</param>
// BOOL bOnlyRect : 속도 개선을 위해서 좌우측 폭만 결정해서 리턴한다.
// BOOL bInclNotch : Notch도 tvArr에 포함.(좌우모두)
void CRcBridgeApp::GetTvCrossSection(CTwinVectorArray& tvArr, double sta, CDPoint vAng, BOOL bInclFootway, BOOL bInclSlabLower, 
									 BOOL bInclGuardWall, long nDrawLeft, BOOL bMirror, BOOL bIncludePave, BOOL bRoadNotSlab, BOOL bExp, BOOL bOnlyRect, BOOL bInclNotch)
{
	if(!IsExp(sta))
		bExp	= FALSE;

	tvArr.RemoveAll();

	CTwinVectorArray tvArrGW;
	CTwinVector tv;

	long nQtyHDan = GetQtyHDan(bRoadNotSlab);
	long nQtyHDanAct = GetQtyHDanAct(sta,vAng,bRoadNotSlab,bExp);
	CDPoint A[30], B[30], C[30];

	if(bOnlyRect)
	{
		double disSlabLeft	= bExp ? GetDisSlabLeftActByGagak(sta, vAng, bRoadNotSlab) : GetDisSlabLeftAct(sta, vAng, bRoadNotSlab);
		double dW			= bExp ? GetWidthSlabActByGagak(sta, vAng, 0, bRoadNotSlab) : GetWidthSlabAct(sta, vAng, 0, FALSE, FALSE, bRoadNotSlab);
		tv.m_v1	= CVector(disSlabLeft, 0, 0);
		tv.m_v2	= tv.m_v1 + CVector(dW, 0, 0);
		tvArr.Add(tv);

		return;
	}

	// 슬래브 상단
	GetXySlabUpperAct(sta, vAng, A,bRoadNotSlab, bExp);
	long nHDan = 0;
	long nHDanAct = 0;
	CTwinVector tvOld;
	for(nHDan=0; nHDan<nQtyHDan; nHDan++)
	{
		nHDanAct = GetNumSlabNodeAct(sta, vAng, nHDan,bRoadNotSlab, bExp);
		tv.m_v1 = A[nHDanAct];	tv.m_v1.z = 0;	tv.m_v2 = A[nHDanAct+1];	tv.m_v2.z = 0;
		tv.m_sInfo.Format("슬래브상면 횡단%d",nHDan+1);
		if(!ComparePt(tv.m_v1, tvOld.m_v1, "=") || !ComparePt(tv.m_v2, tvOld.m_v2, "=") || nHDan == 0)
			tvArr.Add(tv);
		tvOld	= tv;

		if((A[nHDanAct+1].x-A[nHDanAct].x)!=GetLengthHDanAct(sta, vAng, nHDan,bRoadNotSlab, bExp) && nHDanAct+1 < nQtyHDanAct)
		{
			nHDanAct++;
			tv.m_v1 = A[nHDanAct];	tv.m_v1.z = 0;	tv.m_v2 = A[nHDanAct+1];	tv.m_v2.z = 0;
			tv.m_sInfo = "";
			if(!ComparePt(tv.m_v1, tvOld.m_v1, "=") || !ComparePt(tv.m_v2, tvOld.m_v2, "="))
				tvArr.Add(tv);
			tvOld	= tv;
		}
	}

	nQtyHDanAct = GetQtyHDanAct(sta,vAng,bRoadNotSlab,bExp);
	if(bInclSlabLower && !bRoadNotSlab)
	{
		CDPointArray xyArr;
		GetPointArraySlabLowerAct(sta, vAng, xyArr, bExp);

		// 슬래브 하단
		for(nHDanAct=0; nHDanAct<nQtyHDanAct+1; nHDanAct++)
			B[nHDanAct] = CDPoint(A[nHDanAct].x, A[nHDanAct].y-m_dTS);

		long nQtyHDanActLower = xyArr.GetSize();
		for(nHDanAct=0; nHDanAct<nQtyHDanActLower; nHDanAct++)
			C[nHDanAct] = xyArr.GetAt(nHDanAct);

		CDPoint xyCen(0, 0), xy1(0, 0), xy2(0, 0);
		// 좌측
		long nIdx = 1;
		CDPoint xyPrevL1 = C[1];
		CDPoint xyCurL1 = C[0];
		CDPoint xyNextL2 = A[0];
		if(m_nSlabChemfer[0]<2) // 슬래브 끝단 모따기 형태(0:없음, 1:직선, 2:곡선)
		{
			CDPoint xyNextL1 = m_bCantilSlab[0] ? CDPoint(C[0].x, C[0].y+m_dCantilH1[0]) : A[0];
			if(m_bCantilSlab[0])
			{
				CDPoint xyPrevL2 = xyNextL1;
				CDPoint xyCurL2 = CDPoint(B[0].x, B[0].y+m_dCantilH1[0]+m_dCantilH2[0]);
				GetXyChamferSegAndSeg(xyPrevL2, xyCurL2, xyNextL2, xyCurL2, xy1, xy2, m_dTCW2[0], m_dTCH2[0]);
				tv.m_v1 = A[0];	tv.m_v1.z = 0;	tv.m_v2 = xy2;	tv.m_v2.z = 0;
				tv.m_sInfo.Format("슬래브좌측면 횡단%d", nIdx++);
				tvArr.Add(tv);
				tv.m_v1 = xy2;	tv.m_v1.z = 0;	tv.m_v2 = xy1;	tv.m_v2.z = 0;
				tv.m_sInfo = _T("좌측슬래브모따기2");
				tvArr.Add(tv);
				tv.m_v1 = xy1;	tv.m_v1.z = 0;	tv.m_v2 = xyNextL1;	tv.m_v2.z = 0;
				tv.m_sInfo.Format("슬래브좌측면 횡단%d", nIdx++);
				tvArr.Add(tv);
			}
			if(xyCurL1.x+m_dTCW1[0]>xyPrevL1.x)
			{
				// 모따기 값이 첫번째구간을 벗어나는 경우 다음구간에서 구하고 C[1]을 헌치 끝에 맞춘다...
				GetXyChamferSegAndSeg(C[2], C[1], xyNextL1, xyCurL1, xy1, xy2, m_dTCW1[0], m_dTCH1[0]);
				C[1] = xy1;
			}
			else
			{
				GetXyChamferSegAndSeg(xyPrevL1, xyCurL1, xyNextL1, xyCurL1, xy1, xy2, m_dTCW1[0], m_dTCH1[0]);
			}
			tv.m_v1 = xyNextL1;	tv.m_v1.z = 0;	tv.m_v2 = xy2;	tv.m_v2.z = 0;
			tv.m_sInfo = _T("슬래브좌측면 횡단0");
			tvArr.Add(tv);
			tv.m_v1 = xy2;	tv.m_v1.z = 0;	tv.m_v2 = xy1;	tv.m_v2.z = 0;
			tv.m_sInfo = _T("좌측슬래브모따기1");
			tvArr.Add(tv);
			C[0] = xy1;
		}
		else
		{
			CDPoint xyNextL1 = m_bCantilSlab[0] ?  CDPoint(C[0].x, C[0].y+m_dCantilH1[0]) : A[0];
			if(m_bCantilSlab[0])
			{
				CDPoint xyPrevL2 = xyNextL1;
				CDPoint xyCurL2 = CDPoint(B[0].x, B[0].y+m_dCantilH1[0]+m_dCantilH2[0]);
				GetInSideTangentCircle(xyCurL2, xyPrevL2, xyNextL2, m_dRE2[0], xyCen, xy1, xy2);
				tv.m_v1 = A[0];	tv.m_v1.z = 0;	tv.m_v2 = xy1;	tv.m_v2.z = 0;
				tv.m_sInfo.Format("슬래브좌측면 횡단%d", nIdx++);
				tvArr.Add(tv);
				tv.m_v1 = xy1;	tv.m_v1.z = -1;	tv.m_v2 = xy2;	tv.m_v2.z = m_dRE2[0];
				tv.m_sInfo = _T("좌측슬래브모따기2");
				tvArr.Add(tv);
				tv.m_v1 = xy2;	tv.m_v1.z = 0;	tv.m_v2 = xyNextL1;	tv.m_v2.z = 0;
				tv.m_sInfo.Format("슬래브좌측면 횡단%d", nIdx++);
				tvArr.Add(tv);
			}

			GetInSideTangentCircle(xyCurL1, xyPrevL1, xyNextL1, m_dRE1[0], xyCen, xy1, xy2);
			if(xyCurL1.x+m_dRE1[0]>xyPrevL1.x)
			{
				// 모따기 값이 첫번째구간을 벗어나는 경우 다음구간에서 구하고 C[1]을 헌치 끝에 맞춘다...
				CDPoint xyCurL1Tmp	= xyCurL1;
				GetXyMatchSegAndLine(C[1], C[2], xyCurL1, (xyNextL1-xyCurL1).Unit(),  xyCurL1Tmp);
				GetInSideTangentCircle(xyCurL1Tmp, C[2], xyNextL1, m_dRE1[0], xyCen, xy1, xy2);
				C[1] = xy2;
			}
			else
			{
				GetInSideTangentCircle(xyCurL1, xyPrevL1, xyNextL1, m_dRE1[0], xyCen, xy1, xy2);
				if(xy2.x > xyPrevL1.x)
					C[1]	= xy2;
			}
			tv.m_v1 = xyNextL1;	tv.m_v1.z = 0;	tv.m_v2 = xy1;	tv.m_v2.z = 0;
			tv.m_sInfo = _T("슬래브좌측면 횡단0");
			tvArr.Add(tv);
			tv.m_v1 = xy1;	tv.m_v1.z = -1;	tv.m_v2 = xy2;	tv.m_v2.z = m_dRE1[0];
			tv.m_sInfo = _T("좌측슬래브모따기1");
			tvArr.Add(tv);
			C[0] = xy2;
		}

		// 우측
		nIdx = 1;
		CDPoint xyPrevR2 = A[nQtyHDanAct];
		CDPoint xyCurR1 = C[nQtyHDanActLower-1];
		CDPoint xyNextR1 = C[nQtyHDanActLower-2];
		if(m_nSlabChemfer[1]<2)
		{
			CDPoint xyPrevR1 = m_bCantilSlab[1] ?  CDPoint(C[nQtyHDanActLower-1].x, C[nQtyHDanActLower-1].y+m_dCantilH1[1]) : A[nQtyHDanAct];
			if(m_bCantilSlab[1])
			{
				CDPoint xyNextR2 = xyPrevR1;
				CDPoint xyCurR2 = CDPoint(B[nQtyHDanAct].x, B[nQtyHDanAct].y+m_dCantilH1[1]+m_dCantilH2[1]);
				GetXyChamferSegAndSeg(xyPrevR2, xyCurR2, xyNextR2, xyCurR2, xy1, xy2, m_dTCW2[1], m_dTCH2[1]);
				tv.m_v1 = A[nQtyHDanAct];	tv.m_v1.z = 0;	tv.m_v2 = xy1;	tv.m_v2.z = 0;
				tv.m_sInfo.Format("슬래브우측면 횡단%d", nIdx++);
				tvArr.Add(tv);
				tv.m_v1 = xy1;	tv.m_v1.z = 0;	tv.m_v2 = xy2;	tv.m_v2.z = 0;
				tv.m_sInfo = _T("우측슬래브모따기2");
				tvArr.Add(tv);
				tv.m_v1 = xy2;	tv.m_v1.z = 0;	tv.m_v2 = xyPrevR1;	tv.m_v2.z = 0;
				tv.m_sInfo.Format("슬래브우측면 횡단%d", nIdx++);
				tvArr.Add(tv);
			}
			if(xyCurR1.x-m_dTCW1[1]<xyNextR1.x)
			{
				// 모따기 값이 첫번째구간을 벗어나는 경우 다음구간에서 구하고 C[1]을 헌치 끝에 맞춘다...
				GetXyChamferSegAndSeg(xyPrevR1, xyCurR1, C[nQtyHDanActLower-3], xyNextR1, xy1, xy2, m_dTCH1[1], m_dTCW1[1]);
				C[nQtyHDanActLower-2] = xy2;
			}
			else
			{
				GetXyChamferSegAndSeg(xyPrevR1, xyCurR1, xyNextR1, xyCurR1, xy1, xy2, m_dTCH1[1], m_dTCW1[1]);
				
			}
			tv.m_v1 = xyPrevR1;	tv.m_v1.z = 0;	tv.m_v2 = xy1;	tv.m_v2.z = 0;
			tv.m_sInfo = _T("슬래브우측면 횡단0");
			tvArr.Add(tv);
			tv.m_v1 = xy1;	tv.m_v1.z = 0;	tv.m_v2 = xy2;	tv.m_v2.z = 0;
			tv.m_sInfo = _T("우측슬래브모따기1");
			tvArr.Add(tv);
			C[nQtyHDanActLower-1] = xy2;
		}
		else
		{
			CDPoint xyPrevR1 = m_bCantilSlab[1] ?  CDPoint(C[nQtyHDanActLower-1].x, C[nQtyHDanActLower-1].y+m_dCantilH1[1]) : A[nQtyHDanAct];
			if(m_bCantilSlab[1])
			{
				CDPoint xyNextR2 = xyPrevR1;
				CDPoint xyCurR2 = CDPoint(B[nQtyHDanAct].x, B[nQtyHDanAct].y+m_dCantilH1[1]+m_dCantilH2[1]);
				GetInSideTangentCircle(xyCurR2, xyPrevR2, xyNextR2, m_dRE2[1], xyCen, xy1, xy2);
				tv.m_v1 = A[nQtyHDanAct];	tv.m_v1.z = 0;	tv.m_v2 = xy2;	tv.m_v2.z = 0;
				tv.m_sInfo.Format("슬래브우측면 횡단%d", nIdx++);
				tvArr.Add(tv);
				tv.m_v1 = xy2;	tv.m_v1.z = 1;	tv.m_v2 = xy1;	tv.m_v2.z = -m_dRE2[1];
				tv.m_sInfo = _T("우측슬래브모따기2");
				tvArr.Add(tv);
				tv.m_v1 = xy1;	tv.m_v1.z = 0;	tv.m_v2 = xyPrevR1;	tv.m_v2.z = 0;
				tv.m_sInfo.Format("슬래브우측면 횡단%d", nIdx++);
				tvArr.Add(tv);
			}
			if(xyCurR1.x-m_dRE1[1]<xyNextR1.x)
			{
				// 모따기 값이 첫번째구간을 벗어나는 경우 다음구간에서 구하고 C[1]을 헌치 끝에 맞춘다...
				CDPoint xyCurR1Tmp	= xyCurR1;
				GetXyMatchSegAndLine(C[nQtyHDanActLower-2], C[nQtyHDanActLower-3], xyCurR1, (xyPrevR1-xyCurR1).Unit(), xyCurR1Tmp);
				GetInSideTangentCircle(xyCurR1Tmp, xyPrevR1, C[nQtyHDanActLower-3], m_dRE1[1], xyCen, xy1, xy2);
				C[nQtyHDanActLower-2] = xy1;
			}
			else
			{
				GetInSideTangentCircle(xyCurR1, xyPrevR1, xyNextR1, m_dRE1[1], xyCen, xy1, xy2);
				if(xy1.x < xyNextR1.x)
					C[nQtyHDanActLower-2] = xy1;

			}
			tv.m_v1 = xyPrevR1;	tv.m_v1.z = 0;	tv.m_v2 = xy2;	tv.m_v2.z = 0;
			tv.m_sInfo = _T("슬래브우측면 횡단0");
			tvArr.Add(tv);
			tv.m_v1 = xy2;	tv.m_v1.z = 1;	tv.m_v2 = xy1;	tv.m_v2.z = -m_dRE1[1];
			tv.m_sInfo = _T("우측슬래브모따기1");
			tvArr.Add(tv);
			C[nQtyHDanActLower-1] = xy1;
		}

		for(nHDan = 0; nHDan < nQtyHDanActLower-1; nHDan++)
		{
			tv.m_v1	= C[nHDan];
			tv.m_v2	= C[nHDan+1];
			tv.m_v1.z	= 0;
			tv.m_v2.z	= 0;
			if(tv.GetLength() == 0) continue;
			tv.m_sInfo.Format("슬래브하부수평면 횡단%d", nHDan+1);
			tvArr.Add(tv);
		}

		// 2006.08.16 added by jaeho
		if(bInclNotch)
		{
			double dWidthNotch		= m_dNotchWidth1;
			double dHeightNotch		= m_dNotchHeight;

			CTwinVectorArray tvArrLower;
			long i = 0; for(i = 0; i < tvArr.GetSize(); i++)
			{
				tv	= tvArr.GetAt(i);
				if(tv.m_sInfo.Find("슬래브하부수평면 횡단", 0) != -1)
					tvArrLower.Add(tv);
			}

			
			if(tvArrLower.GetSize() > 0)
			{
				CRebarPlacing rb;
				CDPointArray xyArrMatch;
				CTwinVectorArray tvArrNotch;;

				double dDistFromSide	= 50;
				// 왼쪽 노치 
				CTwinVector tvStt	= tvArrLower.GetAt(0);
				rb.GetXyMatchTvArrAndLine(tvArrLower, tvStt.GetXyLeft()+CDPoint(dDistFromSide, 0), CDPoint(0, 1), xyArrMatch);
				if(xyArrMatch.GetSize() > 0)
				{
					tv.m_v1	= xyArrMatch.GetAt(0);
					tv.m_v2	= tv.m_v1 + CVector(0, dHeightNotch, 0);
					tv.m_sInfo	= "노치(좌)";
					tvArrNotch.Add(tv);
					
					tv.m_v1	= tv.m_v2;
					tv.m_v2	= tv.m_v1 + CVector(dWidthNotch, 0, 0);
					tv.m_sInfo	= "노치(좌)";
					tvArrNotch.Add(tv);

					tv.m_v1	= tv.m_v2;
					rb.GetXyMatchTvArrAndLine(tvArrLower, tv.m_v1, CDPoint(0, 1), xyArrMatch);
					if(xyArrMatch.GetSize() > 0)
					{
						tv.m_v2	= xyArrMatch.GetAt(0);
						tv.m_sInfo	= "노치(좌)";
						tvArrNotch.Add(tv);

						// 노치가 정상적으로 생성 된 경우 횡단면 하면을 판다.
						CDRect rect	= tvArrNotch.GetRect();
						CTwinVectorArray tvArrCut;
						tvArrCut.SetFromRect(rect);

						rb.GetTvArrayByCrossTvArrayArea(tvArr, tvArrCut, FALSE);
						tvArr.AddFromTvArray(tvArrNotch);
					}
				}

				// 오른쪽 노치
				tvArrNotch.RemoveAll();
				CTwinVector tvEnd	= tvArrLower.GetAt(tvArrLower.GetSize()-1);
				rb.GetXyMatchTvArrAndLine(tvArrLower, tvEnd.GetXyRight() + CDPoint(-dDistFromSide, 0), CDPoint(0, 1), xyArrMatch);
				if(xyArrMatch.GetSize() > 0)
				{
					tv.m_v1	= xyArrMatch.GetAt(0);
					tv.m_v2	= tv.m_v1 + CVector(0, dHeightNotch, 0);
					tv.m_sInfo	= "노치(우)";
					tvArr.Add(tv);
					tvArrNotch.Add(tv);
					
					tv.m_v1	= tv.m_v2;
					tv.m_v2	= tv.m_v1 + CVector(-dWidthNotch, 0, 0);
					tv.m_sInfo	= "노치(우)";
					tvArr.Add(tv);
					tvArrNotch.Add(tv);

					tv.m_v1	= tv.m_v2;
					rb.GetXyMatchTvArrAndLine(tvArrLower, tv.m_v1, CDPoint(0, 1), xyArrMatch);
					if(xyArrMatch.GetSize() > 0)
					{
						tv.m_v2	= xyArrMatch.GetAt(0);
						tv.m_sInfo	= "노치(우)";
						tvArr.Add(tv);
						tvArrNotch.Add(tv);

						// 노치가 정상적으로 생성 된 경우 횡단면 하면을 판다.
						CDRect rect	= tvArrNotch.GetRect();
						CTwinVectorArray tvArrCut;
						tvArrCut.SetFromRect(rect);

						rb.GetTvArrayByCrossTvArrayArea(tvArr, tvArrCut, FALSE);
						tvArr.AddFromTvArray(tvArrNotch);
					}
				}
			}
		}
	}

	// 포장면
	BOOL bRail = m_pARcBridgeDataStd->IsRailDesign();	//hgh 함수 제작 할 것
	double thickPave = GetThickPave(bRoadNotSlab);
	if(thickPave>0 && bIncludePave && !(m_bUnderGroundRahmen && bRail))
	{
		long nTerm = (long)thickPave/15;
		double dSpace = (m_bUnderGroundRahmen && bRoadNotSlab) ? -thickPave/nTerm : thickPave/nTerm;
		for(nHDan=0; nHDan<nQtyHDan; nHDan++)
		{
			CGuardWallRC* pGW = GetGuardWallByHDan(nHDan,bRoadNotSlab);
			if(pGW == NULL) continue;
			if(pGW->IsPaveOnHDan())
			{
				for(int n=1; n<nTerm+1; n++)
				{
					nHDanAct = GetNumSlabNodeAct(sta, vAng, nHDan,bRoadNotSlab, bExp);
					tv.m_v1 = CDPoint(A[nHDanAct].x, A[nHDanAct].y+dSpace*n);
					tv.m_v2 = CDPoint(A[nHDanAct+1].x, A[nHDanAct+1].y+dSpace*n);
					tv.m_sInfo = "";
					tvArr.Add(tv);
					if(Compare(A[nHDanAct+1].x-A[nHDanAct].x,GetLengthHDanAct(sta, vAng, nHDan,bRoadNotSlab, bExp),"=")==FALSE)
					{
						nHDanAct++;
						tv.m_v1 = CDPoint(A[nHDanAct].x, A[nHDanAct].y+dSpace*n);
						tv.m_v2 = CDPoint(A[nHDanAct+1].x, A[nHDanAct+1].y+dSpace*n);
						tvArr.Add(tv);
					}
				}
			}
		}
	}

	// 방호벽 및 보도
	if(bInclGuardWall)
	{
		if(m_pARcBridgeDataStd->IsRailDesign())
		{
			for(nHDan=0; nHDan<nQtyHDan; nHDan++)
			{
				CGuardWallRC* pGW = GetGuardWallByHDan(nHDan,bRoadNotSlab);
				if(!pGW) continue;

				if(pGW->IsTypeHDanRoadTotalGuardFence() && !pGW->IsTypeHDanRailTotalDosang())	//방호벽 종류, 직결식도상, 자갈도상이 아니면
				{
					GetTvGuardWall(pGW, tvArrGW, sta, vAng,TRUE,bRoadNotSlab, bExp);
					tvArr.AddFromTvArray(tvArrGW);
				}
			}
		}
		else
		{
			for(nHDan=0; nHDan<nQtyHDan; nHDan++)
			{
				CGuardWallRC* pGW = GetGuardWallByHDan(nHDan,bRoadNotSlab);
				if(!pGW) continue;

				// 보도 및 보차도 경계석 제외
				if(pGW->IsTypeHDanRoadFootway() || pGW->IsTypeHDanRoadFootwayBlock()) continue;
				
				if(pGW != NULL && !pGW->IsTypeHDanOnlyNone())
				{
					GetTvGuardWall(pGW, tvArrGW, sta, vAng,TRUE,bRoadNotSlab, bExp);
					tvArr.AddFromTvArray(tvArrGW);
				}
			}
		}
	}
	
	// 보도
	if(bInclFootway)
	{
		for(nHDan=0; nHDan<nQtyHDan; nHDan++)
		{
			CGuardWallRC* pGW = GetGuardWallByHDan(nHDan,bRoadNotSlab);
			if(!pGW) continue;

			if(pGW->IsTypeHDanRoadFootway() || pGW->IsTypeHDanRoadFootwayBlock())
			{
				GetTvGuardWall(pGW, tvArrGW, sta, vAng, TRUE, bRoadNotSlab, bExp);
				tvArr.AddFromTvArray(tvArrGW);
			}
		}
	}

	// mirror함
	CRebarPlacing rb;
	if(bMirror)
	{
		CDRect rect	= tvArr.GetRect();
		rb.Mirror(tvArr, CDPoint(rect.left, 0), TRUE);
		rb.MoveRebar(tvArr, CDPoint(rect.Width(), 0));
	}
	
	// nDrawLeft에 따라서 잘라냄
	if(nDrawLeft != 0)
	{
		BOOL bVert	= Compare(vAng.GetAngleDegree(), 90.0, "=");
		CDRect rect	= tvArr.GetRect();
		double dMid	= GetMidCrossSection(bVert);
		BOOL bLeft	= nDrawLeft == -1 ? FALSE : TRUE;
		CDPoint xy1	= CDPoint(dMid, rect.bottom-100);
		CDPoint xy2	= CDPoint(dMid, rect.top+100);
		rb.TrimTvArrayByLine(tvArr, xy1, xy2, bLeft, TRUE, FALSE);
	}
}

// 실제 슬래브의 시종점 리턴
void CRcBridgeApp::GetTvSttEndOfSlab(CTwinVector &tvStt, CTwinVector &tvEnd, BOOL bExp)
{
	CTwinVectorArray tvArrPlane;
	CTwinVectorArray tvArrStt, tvArrEnd;
	GetTvRebarOut_Plane_UpperSlabStd(tvArrPlane, -1, bExp);

	CTwinVector tv;
	long i = 0; for(i = 0; i < tvArrPlane.GetSize(); i++)
	{
		tv	= tvArrPlane.GetAt(i);
		if(tv.m_sInfo == "시점") tvArrStt.Add(tv);
		else if(tv.m_sInfo == "종점") tvArrEnd.Add(tv);
	}

	if(tvArrStt.GetSize() > 0)
	{
		tvStt.m_v1	= tvArrStt.GetAt(0).m_v1;
		tvStt.m_v2	= tvArrStt.GetAt(tvArrStt.GetSize()-1).m_v2;
	}

	if(tvArrEnd.GetSize() > 0)
	{
		tvEnd.m_v1	= tvArrEnd.GetAt(tvArrEnd.GetSize()-1).m_v2;
		tvEnd.m_v2	= tvArrEnd.GetAt(0).m_v1;
	}
}
void CRcBridgeApp::GeMidElementAndPointOnWall(long nTypeDesign, long nJijum, long &nElement, long &nPoint, ETypeEnvComboLsd eEnvTypeLsd)
{
	CFEMManage *pFEM = GetFemManage(nTypeDesign);
	if(pFEM == NULL) return;
	
	long nIdxEleStt = GetNumberElementWallSttCheck(nJijum,nTypeDesign,FALSE,FALSE);
	long nIdxEleEnd = GetNumberElementWallEndCheck(nJijum,nTypeDesign,FALSE,FALSE);

	CElement *pEleStt = pFEM->GetElement((USHORT)nIdxEleStt);
	CElement *pEleEnd = pFEM->GetElement((USHORT)nIdxEleEnd);

	CString szCombo = GetStringComboEnv(nTypeDesign, eEnvTypeLsd, (nJijum==0)? ePartOutWallStt : ePartOutWallEnd, CHECK_POS_CENTER);
	
	long nLoadCase  = pFEM->GetLoadCaseIndex(szCombo);	
	if(nLoadCase<0) return;

	CJoint   *pJntStt = pFEM->GetJoint(pEleStt->m_nJStt);
	CJoint   *pJntEnd = pFEM->GetJoint(pEleEnd->m_nJEnd);
	double dCheckHeight = (pJntStt->m_vPointOrigin.z + pJntEnd->m_vPointOrigin.z)/2;
	double dNearHeight  = pJntStt->m_vPointOrigin.z - pJntEnd->m_vPointOrigin.z;

	for(long nEle=nIdxEleStt; nEle<=nIdxEleEnd; nEle++)
	{
		CElement *pEle = pFEM->GetElement((USHORT)nEle);
		CResultForceElement *pResult = pEle->GetResultForce(nLoadCase);
		if(pResult == NULL) continue;

		long nSizePoint = pResult->m_ResultForceElement.GetSize();

		CJoint *pJntSttCur = pFEM->GetJoint(pEle->m_nJStt);
		CJoint *pJntEndCur = pFEM->GetJoint(pEle->m_nJEnd);
		double dHeightPos = pJntStt->m_vPointOrigin.z;
		long i=0; for(i=0; i<nSizePoint; i++)
		{
			dHeightPos = pJntSttCur->m_vPointOrigin.z - (pJntSttCur->m_vPointOrigin.z-pJntEndCur->m_vPointOrigin.z)*(double)(i/(double)(nSizePoint));
			if(dNearHeight>fabs(dCheckHeight-dHeightPos))
			{
				dNearHeight = fabs(dCheckHeight-dHeightPos);
				nElement = nEle;
				nPoint   = i;
			}
		}
	}
}
void CRcBridgeApp::GetTvCrossSectionUnderNori(CTwinVectorArray& tvArr, double sta, CDPoint vAng, BOOL bLeft)
{
	tvArr.RemoveAll();
	if(!m_bUnderGroundRahmen)	return;
	if(m_dUnderGroundNoriMaxHeight==0 || m_dUnderGroundNoriSlopeFirst==0)	return;
	CTwinVectorArray tvArrRoad, tvArrSlab, tvArrNori;
	GetTvCrossSection(tvArrRoad, sta, vAng, FALSE, FALSE, FALSE, 0, FALSE, FALSE,TRUE);	//도로면 좌표만 가지고 옴.
	CDPoint xyRoadLeft = tvArrRoad.GetAt(0).m_v1;						//노면 좌측 끝단
	CDPoint xyRoadRigh = tvArrRoad.GetAt(tvArrRoad.GetSize()-1).m_v2;	//노면 우측 끝단
	CDPoint xyRoadEnd = bLeft ? xyRoadLeft : xyRoadRigh;
	double minusByLeft = bLeft ? -1 : 1;
	CTwinVector tv, tv1, tv2, tv3;
	CDPoint xy1(0,0),xy2(0,0),xyResult(0,0);

	if(m_nSeparBri == 1 && !bLeft)	// 상행인 경우 우측을 그리지 않음(분리교량이 아닌 경우는 모두 그림)
	{
		GetTvCrossSection(tvArrSlab, sta, vAng, FALSE, FALSE, FALSE, 0, FALSE, FALSE, FALSE);	//슬래브면 좌표만 가지고 옴.
		
		tv1.m_v1 = xyRoadRigh;
		tv1.m_v2 = tvArrSlab.GetAt(tvArrSlab.GetSize()-1).m_v2;	//슬래브 우측 끝단
		tvArr.Add(tv1);
		return;
	}
	else if(m_nSeparBri == 2 && bLeft)		// 하행인 경우 좌측을 그리지 않음(분리교량이 아닌 경우는 모두 그림)
	{
		GetTvCrossSection(tvArrSlab, sta, vAng, FALSE, FALSE, FALSE, 0, FALSE, FALSE, FALSE);	//슬래브면 좌표만 가지고 옴.
		
		tv1.m_v1 = xyRoadLeft;
		tv1.m_v2 = tvArrSlab.GetAt(0).m_v1;	//슬래브 우측 끝단
		tvArr.Add(tv1);
		return;
	}

	long nHDan=0; for(nHDan=0; nHDan<GetQtyHDan(FALSE); nHDan++)
	{
		CGuardWallRC* pGW = GetGuardWallByHDan(nHDan, FALSE);
		if(!pGW) continue;
		if(!pGW->IsTypeHDanOnlyNone())
		{
			CTwinVectorArray tvArrGW;
			xy1 = xyRoadEnd;
			xy2.y = xy1.y - m_dUnderGroundNoriMaxHeight;		// 1단 경사부
			xy2.x = xy1.x + m_dUnderGroundNoriMaxHeight*m_dUnderGroundNoriSlopeFirst*minusByLeft;
			GetTvGuardWall(pGW, tvArrGW, sta, vAng, FALSE);
			if(GetXyMatchSegAndSegTwinVector(xy1, xy2, tvArrGW, 14, xyResult))
			{
				tv1.m_v1 = xy1;
				tv1.m_v2 = xyResult;
				tvArr.Add(tv1);
				return;
			}
			tv1.m_v1 = xy1;
			tv1.m_v2 = xy2;
			xy1 = xy2;
			xy2.x = xy1.x + m_dUnderGroundNoriWidth*minusByLeft;	//소단 폭 부분
			xy2.y = xy1.y - m_dUnderGroundNoriWidth*m_dUnderGroundNoriSolpeSodan/100;
			if(GetXyMatchSegAndSegTwinVector(xy1, xy2, tvArrGW, 14, xyResult))
			{
				tvArr.Add(tv1);
				tv2.m_v1 = xy1;
				tv2.m_v2 = xyResult;
				tvArr.Add(tv2);
				return;
			}
			tv2.m_v1 = xy1;
			tv2.m_v2 = xy2;
			xy1 = xy2;
			xy2.y = xy1.y - m_dUnderGroundNoriMaxHeight;			// 2단 경사부
			xy2.x = xy1.x + m_dUnderGroundNoriMaxHeight*m_dUnderGroundNoriSlopeNext*minusByLeft;
			if(GetXyMatchSegAndSegTwinVector(xy1, xy2, tvArrGW, 14, xyResult))
			{
				tvArr.Add(tv1);
				tvArr.Add(tv2);
				tv3.m_v1 = xy1;
				tv3.m_v2 = xyResult;
				tvArr.Add(tv3);
				return;
			}
		}
	}

	
	tvArr.RemoveAll();
	xy1 = xyRoadEnd;
	xy2.y = xy1.y - m_dUnderGroundNoriMaxHeight;		// 1단 경사부
	xy2.x = xy1.x + m_dUnderGroundNoriMaxHeight*m_dUnderGroundNoriSlopeFirst*minusByLeft;
	//슬래브면과 노리선이 만나는 점을 찾음.	
	GetTvCrossSection(tvArrSlab, sta, vAng, FALSE, FALSE, FALSE, 0, FALSE, FALSE, FALSE);	//슬래브면 좌표만 가지고 옴.
	long nQtyArrSlab = tvArrSlab.GetSize();
	if(GetXyMatchSegAndSegTwinVector(xy1, xy2, tvArrSlab, nQtyArrSlab, xyResult))
	{
		tv1.m_v1 = xy1;
		tv1.m_v2 = xyResult;
		tvArr.Add(tv1);
		return;
	}
	tv1.m_v1 = xy1;
	tv1.m_v2 = xy2;
	tvArr.Add(tv1);

	CDRect rectSlab	= tvArrSlab.GetRect();
	while(1)
	{
		xy1 = xy2;
		xy2.x = xy1.x + m_dUnderGroundNoriWidth*minusByLeft;
		xy2.y = xy1.y - m_dUnderGroundNoriWidth*m_dUnderGroundNoriSolpeSodan/100;
		if(GetXyMatchSegAndSegTwinVector(xy1, xy2, tvArrSlab, nQtyArrSlab, xyResult))
		{
			tv1.m_v1 = xy1;
			tv1.m_v2 = xyResult;
			tvArr.Add(tv1);
			return;
		}
		tv1.m_v1 = xy1;
		tv1.m_v2 = xy2;
		tvArr.Add(tv1);
		xy1 = xy2;
		xy2.y = xy1.y - m_dUnderGroundNoriMaxHeight;
		xy2.x = xy1.x + m_dUnderGroundNoriMaxHeight*m_dUnderGroundNoriSlopeNext*minusByLeft;
		if(GetXyMatchSegAndSegTwinVector(xy1, xy2, tvArrSlab, nQtyArrSlab, xyResult))
		{
			tv1.m_v1 = xy1;
			tv1.m_v2 = xyResult;
			tvArr.Add(tv1);
			return;
		}

		// 교차점을 못 찾은 경우 선이 횡단 아래로 내려가지 않은 경우 다시 한번 찾기를 하자.
		if(rectSlab.bottom > xy2.y) break;
		
		tv1.m_v1	= xy1;
		tv1.m_v2	= xy2;
		tvArr.Add(tv1);
	}
}
//<summary>
//도상 기준 높이를 구함
//  Rail Level 기준이 아닐 경우 : 기준 높이는 입력 높이이며, 기준 위치는 도상 상면 끝단
//  Rail Level 기준일 경우		: 기준 높이는 계산으로 구하며, 기준 위치는 레일 위치
//</summary>
//<param name="nHDan"></param>
// double CRcBridgeApp::GetHeightDosang(double sta, CDPoint vAng, long nHDan)
// {
// 	if(m_dDisBaseElevationRL == 0)
// 		return GetHeightDosangInput();
// 
// 	CGuardWallRC* pGW = GetGuardWallByHDan(nHDan,m_bUnderGroundRahmen);
// 	if(!pGW) return 0;
// 	if(pGW->IsTypeHDanRailTotalDosang())
// 	{
// 		CDPoint A[6]; // 도상상단의 포인트
// 		GetXyDosang(sta, vAng, nHDan, A,m_bUnderGroundRahmen);
// 
// 		double giulDosang = 0;
// 		double heightL = ~(GetMatchPavePoint(sta, A[2], giulDosang, FALSE) - A[2]);
// 		double heightR = ~(GetMatchPavePoint(sta, A[3], giulDosang, FALSE) - A[3]);
// 		return min(heightL, heightR);
// 	}
// 	return 0;
// }
//기능 : 현재 도상의 평균 높이를 구한다
// double CRcBridgeApp::GetHeightDosangAvg(double sta, CDPoint vAng, long nHDan)
// {
// 	CGuardWallRC* pGW = GetGuardWallByHDan(nHDan,m_bUnderGroundRahmen);
// 	if(!pGW) return 0;
// 	if(pGW->IsTypeHDanRailTotalDosang())
// 	{
// 		CDPoint A[6]; // 도상상단의 포인트
// 		GetXyDosang(sta, vAng, nHDan, A,m_bUnderGroundRahmen);
// 
// 		double giulDosang = 0;
// 		double heightL = ~(GetMatchPavePoint(sta, A[2], giulDosang, FALSE) - A[2]);
// 		double heightR = ~(GetMatchPavePoint(sta, A[3], giulDosang, FALSE) - A[3]);
// 		return (heightL+heightR)/2;
// 	}
// 	return 0;
// }

// 외측벽체 가각부 좌표 구함.(횡단면도)
// bStt : 시종점
// bLeft : 좌우측
// bFront : 전면 배면
void CRcBridgeApp::GetTvCrossSection_OutWall_Exp(CTwinVectorArray &tvArr, BOOL bStt, BOOL bLeftWall, BOOL bInner, BOOL bLeft)
{
	tvArr.RemoveAll();
	CTwinVectorArray tvArrCross;
	GetTvCrossSection_OutWall(tvArrCross, bStt, bLeft);

	// 가각부로 인해 확장된 너비
	double dWidthExp	= GetWidthDiffSlabActOrgAndGagak(bStt, bLeftWall, bInner);//GetWidthSlabActByGagak(dSta, vAng, nLeft) - GetWidthSlabAct(dSta, vAng, nLeft);



	// 실제 벽체 측면을 포함해서 좌표 생성
	CTwinVector tvLeft, tvRight, tvLower;
	if(bLeftWall == bLeft)
	{
		tvRight	= tvArrCross.GetTvByInfo("벽체좌측");
		tvRight.m_sInfo	= "벽체우측";
		tvLeft.m_v1	= CVector(tvRight.m_v1.x - dWidthExp, tvRight.m_v1.y, tvRight.m_v1.z);
		tvLeft.m_v2	= CVector(tvRight.m_v2.x - dWidthExp, tvRight.m_v2.y, tvRight.m_v2.z);
		tvLeft.m_sInfo	= "벽체좌측";
	}
	else
	{
		tvLeft	= tvArrCross.GetTvByInfo("벽체우측");
		tvLeft.m_sInfo	= "벽체좌측";
		tvRight.m_v1	= CVector(tvLeft.m_v1.x + dWidthExp, tvLeft.m_v1.y, tvLeft.m_v1.z);
		tvRight.m_v2	= CVector(tvLeft.m_v2.x + dWidthExp, tvLeft.m_v2.y, tvLeft.m_v2.z);
		tvRight.m_sInfo	= "벽체우측";
	}

	tvLower.m_v1	= tvLeft.GetXyBottom();
	tvLower.m_v2	= tvRight.GetXyBottom();
	tvLower.m_sInfo	= "벽체하면";

	// 상부슬래브 상면
	CTwinVectorArray tvArrUpper, tvArrHunch;
	CRebarPlacing rb;
	double dSta	= GetStationByTarget(bStt ? (bInner ? BRIDGE_OFF_STT_STT : BRIDGE_OFF_STT_SLAB) : (bInner ? BRIDGE_OFF_END_END : BRIDGE_OFF_END_SLAB), 0, FALSE);
	CDPoint vAng	= GetAngleByTarget(bStt ? (bInner ? BRIDGE_OFF_STT_STT : BRIDGE_OFF_STT_SLAB) : (bInner ? BRIDGE_OFF_END_END : BRIDGE_OFF_END_SLAB), 0);
	GetTvCrossSection(tvArrUpper, dSta, vAng, FALSE, FALSE, FALSE, 0, FALSE, FALSE, FALSE, TRUE);
	GetTvCrossHunch(tvArrHunch, dSta, vAng, FALSE, 0, FALSE, TRUE);

	if(!bStt)
	{
		double dELDiff	= GetDiffELSttAndJijumForVertSection(m_nQtyJigan, FALSE);
		if(dELDiff > 0)
		{
			rb.MoveRebar(tvArrUpper, CDPoint(0, -dELDiff));
			rb.MoveRebar(tvArrHunch, CDPoint(0, -dELDiff));
		}
	}

	if(!bLeft)
	{
		double dMid	= GetMidCrossSection(FALSE);
		rb.Mirror(tvArrUpper, CDPoint(dMid, 0), TRUE);
		rb.Mirror(tvArrHunch, CDPoint(dMid, 0), TRUE);
	}

	CDPointArray xyArrMatch, xyArrMatchHunch;
	rb.GetXyMatchTvArrAndLine(tvArrUpper, tvLeft.m_v1, tvLeft.GetXyDir(), xyArrMatch);
	rb.GetXyMatchTvArrAndLine(tvArrHunch, tvLeft.m_v1, tvLeft.GetXyDir(), xyArrMatchHunch);
	if(xyArrMatch.GetSize() > 0)
		tvLeft.m_v2	= xyArrMatch.GetAt(0);
	if(xyArrMatchHunch.GetSize() > 0)
		tvLeft.m_v2	= xyArrMatchHunch.GetAt(0);

	rb.GetXyMatchTvArrAndLine(tvArrUpper, tvRight.m_v1, tvRight.GetXyDir(), xyArrMatch);
	rb.GetXyMatchTvArrAndLine(tvArrHunch, tvRight.m_v1, tvRight.GetXyDir(), xyArrMatchHunch);
	if(xyArrMatch.GetSize() > 0)
		tvRight.m_v1	= xyArrMatch.GetAt(0);
	if(xyArrMatchHunch.GetSize() > 0)
		tvRight.m_v1	= xyArrMatchHunch.GetAt(0);

	

	tvArr.Add(tvLeft);
	tvArr.Add(tvLower);
	tvArr.Add(tvRight);
}

// 외측벽체 횡단면 좌표
void CRcBridgeApp::GetTvCrossSection_OutWall(CTwinVectorArray &tvArr, BOOL bStt, BOOL bLeft)
{
	tvArr.RemoveAll();

	CWallApp wall;
	MakeWallByOutWallOnlyShape(wall, bStt, TRUE);

	wall.GetTvLeftSide(tvArr, FALSE, FALSE, FALSE, bLeft, 0, FALSE, FALSE);
}

BOOL CRcBridgeApp::IsWingWallTailBottom(BOOL bStt, BOOL bLeft)
{
	BOOL bIsEx = FALSE;
	CTwinVectorArray tvArr;
	GetTvWingWallFront(bStt, bLeft, TRUE, tvArr);

	CTwinVector tv;
	tvArr.GetTvByInfo(_T("날개벽꼬리하단"), tv);

	if(tv.GetLength() > 0) bIsEx = TRUE;

	return bIsEx;
}

double CRcBridgeApp::GetWidthDiffSlabActOrgAndGagak(BOOL bStt, BOOL bLeft, BOOL bInner)
{
	long nJ			= bStt ? 0 : m_nQtyJigan;
	double dSta		= GetStationByTarget(bStt ? (bInner ? BRIDGE_OFF_STT_STT : BRIDGE_OFF_STT_SLAB) : (bInner ? BRIDGE_OFF_END_END : BRIDGE_OFF_END_SLAB), 0, FALSE);//bStt ? m_dStationBridgeStt : m_dStationBridgeStt + m_dLengthBridge;
	
	if(!m_bIsExp) return 0;

	if(!IsOutWall(bStt))
	{
		if(m_bExpJoint[nJ])
		{
			CJijum *pJ	= GetJijum(nJ);
			dSta += (bStt ? pJ->m_dLengthUgan[1] : -pJ->m_dLengthUgan[0]);
		}
	}

	CDPoint vAng	= GetAngleByTarget(bStt ? (bInner ? BRIDGE_OFF_STT_STT : BRIDGE_OFF_STT_SLAB) : (bInner ? BRIDGE_OFF_END_END : BRIDGE_OFF_END_SLAB), 0);
	long nLeft	= bLeft ? -1 : 1;

	// 가각부로 인해 확장된 너비
	double dWidthExp	= GetWidthSlabActByGagak(dSta, vAng, nLeft) - GetWidthSlabAct(dSta, vAng, nLeft);

	return dWidthExp;
}

// 모델링중심을 기준으로한 거리를 구한다...
double CRcBridgeApp::GetDistRigidEndSlab(long nJijum, BOOL bSttPos, BOOL bUpperSlab)
{
	double DistRigid = 0;

	// 기존에 저장된 것이 있는지 체크 /////
	long i = 0; for(i = 0; i < m_pArrSaveDistRigidEndSlab.GetSize(); i++)
	{
		CSaveDistRigidEndSlab *pSave	= m_pArrSaveDistRigidEndSlab.GetAt(i);
		if(pSave->IsMatch(nJijum, bSttPos, bUpperSlab))
		{
			pSave->GetValue(DistRigid);			
			return MAX(DistRigid, 0);
		}
	}
	
	if(nJijum==0 && bSttPos) return 0;
	if(nJijum==m_nQtyJigan && !bSttPos) return 0;
	if(IsRcSlab()) return 0;
	if(IsPiRahmen() && (nJijum==0 || nJijum==GetCountJijum()-1)) return 0;
	// 아치형 중간지점은 강역을 설정할 수 없음...
	if(IsArchType() && nJijum==GetJijumConnectRibAndSlab()) return 0;


//	여기서 호출하면 너무 자주 호출되서 인지 문제가 생긴다.
//	GetTvVertSection_Std(0, FALSE, FALSE, FALSE, 0);
	long nTypJongdanOrg = m_nTypeJongdanForRebar;
	BOOL bApplyAngleJongdanOld = m_bApplyAngleJongdan;
	CTwinVectorArray tvArrVertSectionOrg;
	tvArrVertSectionOrg	= m_tvArrVertSection;
	if(m_nTypeJongdanForRebar != 0 || m_bApplyAngleJongdan)
	{
		m_nTypeJongdanForRebar = 0;
		GetTvVertSection(0, FALSE, FALSE);

		ApplyRoundOfInWallHunch(nJijum-1, TRUE);
		ApplyRoundOfInWallHunch(nJijum-1, FALSE);
	}

	// (ARCBRIDGE-3464) 현재 종단 형상이 m_nTypeJongdanForRebar 뿐만 아니라 bApplyAngle 도 FALSE인 상태여야 한다.

	CDPoint vAng(0, 1);
	double dAng = 90;
//	if(nJijum == 0 || nJijum == m_nQtyJigan)
//	{
//		dAng = 90;
//		vAng = CDPoint(0, 1);
//	}
//	else
//	{
//		CWallApp *pWall = GetInWall(nJijum-1);
//		dAng = pWall->m_dAngle;
//		vAng = ToDPointFrDegree(dAng);
//		if(!bSttPos) dAng = 180 - dAng;
//	}
	

	double SlabThick  = bUpperSlab? m_dTS : m_dTS_Lower;
	double WallThick  = GetThickWall(nJijum)/vAng.y;
	double HunchThick = 0;
	double HunchWidth = 0;

	long nUD = bUpperSlab ? iUPPER : iLOWER;
	BOOL bRoundHunch = IsBoxType() ? FALSE : IsRoundHunch(nJijum, bSttPos, bUpperSlab);
	if(bRoundHunch)
	{
		HunchThick = GetCalcLenRHunch(nJijum, bSttPos) * vAng.y;
		HunchWidth = GetCalcLenRHunch(nJijum, bSttPos);
	}
	else
	{
		if(nJijum==0) 
			HunchThick = (bUpperSlab? m_dHHS : m_dWSHH)*vAng.y;
		else if(nJijum==GetCountJijum()-1) 
			HunchThick = (bUpperSlab? m_dHHE : m_dWEHH)*vAng.y;
		else 
			HunchThick = (bSttPos)? m_dHHL[nJijum-1][nUD]*vAng.y:m_dHHR[nJijum-1][nUD]*vAng.y;
		
		HunchWidth = GetWidthHunch(nJijum, bSttPos, bUpperSlab);
	}

	double dAngleSlab = GetAnglHunchWithSlabOrWall(nJijum, bSttPos, TRUE, bUpperSlab);
	if(Compare(HunchThick,0.0,"=") || Compare(HunchWidth,0.0,"="))
	{
		DistRigid = WallThick/2 - 0.25*SlabThick;
	}
	else if(dAngleSlab<25) // 도로교 설계기준 4.12.3 (다) 변단면의 경우 (2)
	{
		if(m_bSelectRigidCalcHunch || (nJijum==0 || nJijum==m_nQtyJigan))
			DistRigid = WallThick/2 - 0.25*(SlabThick+HunchThick);
		else
			DistRigid = WallThick/2 - 0.25*SlabThick;
	}
	else if(dAngleSlab<60)
	{
		double dLenMinus = fabs(SlabThick/2/tan(ToRadian(dAngleSlab)));
		DistRigid = WallThick/2 + SlabThick/2/tan(ToRadian(dAng)) + HunchWidth -  dLenMinus;

		DistRigid = max(WallThick/2,DistRigid);
	}
	else
	{
		DistRigid = WallThick/2 + (SlabThick/2/(tan(ToRadian(dAng))) + HunchWidth) - 0.25*SlabThick;
	}

	if(nTypJongdanOrg != 0 || bApplyAngleJongdanOld)
	{
		m_nTypeJongdanForRebar	= nTypJongdanOrg;
		m_tvArrVertSection	= tvArrVertSectionOrg;
		GetTvVertSection(0, bApplyAngleJongdanOld, FALSE);
	}

	DistRigid	= Round(DistRigid, 0);

	// 기존에 저장된 것이 없으면 추가
	CSaveDistRigidEndSlab *pSave	= new CSaveDistRigidEndSlab;
	pSave->SetValue(nJijum, bSttPos, bUpperSlab, DistRigid);
	m_pArrSaveDistRigidEndSlab.Add(pSave);
	
	return MAX(DistRigid, 0);
}

double CRcBridgeApp::GetDistRigidEndWall(long nJijum, BOOL bUpper)
{
	if(nJijum < 0)
		return 0;

	double dRetHR = 0;
	if(bUpper == FALSE && IsBoxType() == FALSE) return 0;

	// 기존에 저장된 것이 있는지 체크 /////
	for(long i = 0; i < m_pArrSaveDistRigidEndWall.GetSize(); i++)
	{
		CSaveDistRigidEndWall *pSave	= m_pArrSaveDistRigidEndWall.GetAt(i);
		if(pSave->IsMatch(nJijum, bUpper))
		{
			pSave->GetValue(dRetHR);			
			return MAX(dRetHR, 0);
		}
	}

	double HeightRigid = 0;

	BOOL bSttHunch = (nJijum==0) ? FALSE : TRUE;

	CDPoint vAng(0,1);
	double dAng = 90;
//	if(nJijum == 0 || nJijum == m_nQtyJigan)
//	{
//		dAng = 90;
//		vAng = CDPoint(0, 1);
//	}
//	else
//	{
//		CWallApp *pWall = GetInWall(nJijum-1);
//		dAng = pWall->m_dAngle;
//		vAng = ToDPointFrDegree(dAng);
//	}

	double SlabThick  = (bUpper? m_dTS : m_dTS_Lower)/vAng.y;
	double WallThick  = GetThickWall(nJijum);
	double HunchThick = 0;
	double HunchWidth = 0;

	for(long nPos=0; nPos<2; nPos++)
	{
		if((nJijum==0 && nPos==0) || (nJijum==m_nQtyJigan && nPos==1)) continue;
		bSttHunch = (nPos==0)? TRUE : FALSE;

		if(IsRoundHunch(nJijum, (nPos==0), bUpper))
		{
			HunchThick = GetCalcLenRHunch(nJijum, bSttHunch) * vAng.y;
			HunchWidth = GetCalcLenRHunch(nJijum, bSttHunch);
		}
		else
		{
			if(nJijum==0 || nJijum==GetCountJijum()-1) HunchThick = (nJijum==0)? (bUpper? m_dHHS : m_dWSHH) : (bUpper? m_dHHE : m_dWEHH);
			else HunchThick = (nPos==0)?  m_dHHL[nJijum-1][bUpper? iUPPER:iLOWER]*vAng.y : m_dHHR[nJijum-1][bUpper? iUPPER:iLOWER]*vAng.y;

			HunchWidth = GetWidthHunch(nJijum,bSttHunch, bUpper);
		}
		
		if(nPos==1) dAng = 180 - dAng;
		double dAngleWall = GetAnglHunchWithSlabOrWall(nJijum, nPos==0, FALSE, bUpper);
		
		// 벽체경사가있을때 벽체에 수직으로 중심까지의 거리고려
		double dLenAdd = (dAng == 90)? 0.0 : (WallThick/2)/tan(ToRadian(dAng));

		if(Compare(HunchThick,0.0,"=") || Compare(HunchWidth,0.0,"="))
		{
			HeightRigid = SlabThick/2 - 0.25*WallThick;
		}
		else if(dAngleWall<25)
		{
			HeightRigid = 0.25*WallThick;
		}
		else if(Compare(dAngleWall, 60.0,"<"))
		{
			HeightRigid = SlabThick/2 + dLenAdd + HunchThick/vAng.y - (WallThick/2/tan(ToRadian(dAngleWall)));
			HeightRigid = max(SlabThick/2,HeightRigid);
		}
		else
		{
			HeightRigid = SlabThick/2 + dLenAdd + HunchThick - 0.25*WallThick;			
		}

		dRetHR = max(dRetHR, HeightRigid);
	}
	
	dRetHR = Round(dRetHR, 0);

	// 기존에 저장된 것이 없으면 추가
	CSaveDistRigidEndWall *pSave	= new CSaveDistRigidEndWall;
	pSave->SetValue(nJijum, dRetHR, bUpper);
	m_pArrSaveDistRigidEndWall.Add(pSave);
	return MAX(dRetHR, 0);
}

BOOL CRcBridgeApp::IsRigidEndLeftHunch(long nJijum, BOOL bUpper)
{
	BOOL bIsLeft = FALSE;
	double HeightRigid = 0;
	
	BOOL bSttHunch = (nJijum==0) ? FALSE : TRUE;
	long nUpper = bUpper? iUPPER : iLOWER;
	
	CDPoint vAng(0, 0);
	double dAng = 0;
	if(nJijum == 0 || nJijum == m_nQtyJigan)
	{
		dAng = 90;
		vAng = CDPoint(0, 1);
		bIsLeft = (nJijum == 0)? FALSE : TRUE;
		return bIsLeft;
	}
	else
	{
		CWallApp *pWall = GetInWall(nJijum-1);
		dAng = pWall->m_dAngle;
		vAng = ToDPointFrDegree(dAng);
	}
	
	double SlabThick  = m_dTS/vAng.y;
	double WallThick  = GetThickWall(nJijum);
	double HunchThick = 0;
	double HunchWidth = 0;
	
	double dRetHR = 0;
	for(long nPos=0; nPos<2; nPos++)
	{
		if((nJijum==0 && nPos==0) || (nJijum==m_nQtyJigan && nPos==1)) continue;
		bSttHunch = (nPos==0)? TRUE : FALSE;
		
		if(IsRoundHunch(nJijum, (nPos==0), bUpper))
		{
			HunchThick = GetCalcLenRHunch(nJijum, bSttHunch) * vAng.y;
			HunchWidth = GetCalcLenRHunch(nJijum, bSttHunch);
		}
		else
		{
			if(nJijum==0 || nJijum==GetCountJijum()-1) HunchThick = (nJijum==0)? (bUpper? m_dHHS : m_dWSHH) : (bUpper? m_dHHE : m_dWEHH);
			else HunchThick = (nPos==0)?  m_dHHL[nJijum-1][nUpper]*vAng.y : m_dHHR[nJijum-1][nUpper]*vAng.y;
			
			HunchWidth = GetWidthHunch(nJijum,bSttHunch, bUpper);
		}
		
		if(nPos==1) dAng = 180 - dAng;
		double dAngleWall = GetAnglHunchWithSlabOrWall(nJijum, nPos==0, FALSE, bUpper);
		
		// 벽체경사가있을때 벽체에 수직으로 중심까지의 거리고려
		double dLenAdd = (dAng == 90)? 0.0 : (WallThick/2)/tan(ToRadian(dAng));
		
		if(Compare(HunchThick,0.0,"=") || Compare(HunchWidth,0.0,"="))
		{
			HeightRigid = 0.25*WallThick;
		}
		else if(dAngleWall<25)
		{
			HeightRigid = 0.25*WallThick;
		}
		else if(dAngleWall<60)
		{
			HeightRigid = SlabThick/2 + dLenAdd + HunchThick/vAng.y - (WallThick/2/tan(ToRadian(dAngleWall)));
			HeightRigid = max(SlabThick/2,HeightRigid);
		}
		else
		{
			HeightRigid = SlabThick/2 + dLenAdd + HunchThick - 0.25*WallThick;			
		}
		
		bIsLeft = (dRetHR >= HeightRigid)? TRUE : FALSE;
		dRetHR = max(dRetHR, HeightRigid);
	}
	
	
	return bIsLeft;
}

double CRcBridgeApp::GetWidthHunch(long nJijum, BOOL bSttPos, BOOL bUpper)
{
	if(nJijum==0) 
	{
		return bSttPos ? 0 : (bUpper?  m_dHWS : m_dWSHW);
	}
	else if(nJijum==m_nQtyJigan)
	{
		return !bSttPos ? 0 : (bUpper?  m_dHWE : m_dWEHW);
	}
	else
	{
		long nIdxInWall = nJijum - 1;
		long nIdxHunchLeft  = GetIdxHunchInWall(nIdxInWall,TRUE);

		double HWL = m_dHLW[nIdxHunchLeft][bUpper? iUPPER:iLOWER];
		double HWR = m_dHRW[nIdxHunchLeft][bUpper? iUPPER:iLOWER];

		return bSttPos ? HWL : HWR;
	}
	return 0;
}

double CRcBridgeApp::GetThickWall(long nJijum, BOOL bAngleApplay)
{
	double dThick = 0;
	if(nJijum==0 && IsOutsideWall())
		dThick = m_dWS;
	else if(nJijum==m_nQtyJigan && IsOutsideWall())
		dThick = m_dWE;
	else if(IsOutsideWall()==FALSE && (nJijum==0 || nJijum==m_nQtyJigan))
		return 0;
	else
	{
		CWallApp *pWall = GetInWall(nJijum-1);
		if(pWall)
			dThick = Round(pWall->m_dW, 0);
	}

	if(bAngleApplay)
	{
		CDPoint vAngInWall = GetAngleJijum(nJijum);
		dThick = Round(dThick/vAngInWall.y, 0);
	}
	
	return dThick;
}


// 실제 슬래브의 양 끝 sta
// double CRcBridgeApp::GetStationSttEndByRealSlabZz(BOOL bStt)
// {
// 	long nJ	= bStt ? 0 : m_nQtyJigan;
// 	CDPoint vAng	= GetAngleJijum(nJ);
// 	double dSta		= GetStationOnJijum(nJ);
// 
// 	if(IsOutWall(bStt))
// 	{
// 		dSta += (bStt ? -m_dWS/vAng.y : m_dWE/vAng.y);
// 	}
// 	else
// 	{
// 		CJijum *pJ	= GetJijum(nJ);
// 		if(pJ && m_bExpJoint[nJ])
// 		{
// 			dSta += (bStt ? pJ->m_dLengthUgan[1] : -pJ->m_dLengthUgan[0]);
// 		}
// 	}
// 
// 	return dSta;
// }
//<summary>
// 현재 지점기준 앞, 뒤의 유간을 구함.
//    벽체일 경우 벽체 두께에 해당하는 스테이션 차를 구함.
//	  벽체일 경우 시점부 유간은 항상 [-], 종점부 유간도 항상 [-]. m_dLengthUgan[nStt] 사용을 종점부는 일반적응로 [-]하여 사용하여서 이렇게 처리함.
//</summary>
//<param name="nJ">지점번호</param>
//<param name="nStt">현재 지점의 앞 유간, 뒤 유간을 선택, 시점일 경우 nStt=0, 종점일 경우 nStt=1 만 있음.</param>
//<return></return>
double CRcBridgeApp::GetLengthUgan(long nJ, long nStt)
{
	long nQtyJijum = GetCountJijum();
	if(nJ==0 && IsOutWall(TRUE))
	{
		double staOffset = GetLineBase()->GetStationByOffsetWidth(m_dStationBridgeStt, -m_dWS, GetJijum(nJ)->m_vAngle);
		return staOffset - m_dStationBridgeStt;
	}
	if(nJ==nQtyJijum-1 && IsOutWall(FALSE))
	{
		double staOffset = GetLineBase()->GetStationByOffsetWidth(m_dStationBridgeStt + m_dLengthBridge, m_dWE, GetJijum(nJ)->m_vAngle);
		return -(staOffset - (m_dStationBridgeStt + m_dLengthBridge));
	}
	return GetJijum(nJ)->m_dLengthUgan[nStt];
}
// 좌우측 리턴 
void CRcBridgeApp::GetTvSidePlaneOfSlab(CTwinVectorArray &tvArr, BOOL bLeft, BOOL bExp)
{
	tvArr.RemoveAll();

	CString str	= bLeft ? "좌측" : "우측";

	CTwinVectorArray tvArrPlane;
	GetTvRebarOut_Plane_UpperSlabStd(tvArrPlane, -1, bExp);

	CTwinVector tv;
	long nSize	= tvArrPlane.GetSize();
	long i = 0; for(i = 0; i < nSize; i++)
	{
		tv	= tvArrPlane.GetAt(i);
		if(tv.m_sInfo.Find(str) != -1)
			tvArr.Add(tv);
	}
}


// ENV의 수평력만 산정하여 return
double CRcBridgeApp::GetShoeHoriForce(long nJijum, long nShoe)
{
	double dHoriForce = 0;

	long nV = nJijum == 0 ? 1 : 0;
	long nQtyShoe = GetJijum(nJijum)->m_nQtyShoe[nV];
	if(nQtyShoe == 0) return dHoriForce;

	CString szNameEnv(_T(""));
	if(m_pARcBridgeDataStd->IsLsdDesign())
	{
		szNameEnv = GetStringComboEnv_Lsd(eLSD_COMBINE_EXTREME_I, ePartUpperSlab, TRUE);
	}
	else
	{
		szNameEnv = _T("ENV");
	}

	long nCheckCase = GetCountEarthQuakeModel();
	for(long nCheck = 0; nCheck < nCheckCase; nCheck++)
	{
		CFEMManage *pFEM = &m_FemEQ[nCheck + 1];
		if(pFEM->IsOutputLoaded()==FALSE) continue;

		for(long nCaseEnv = 0; nCaseEnv < 2; nCaseEnv++)
		{
			CString szLoad = szNameEnv + ((nCaseEnv == 0) ? _T(" MAX") : _T(" MIN"));
			long nLoadCase = pFEM->GetLoadCaseByString(szLoad);
			if(nLoadCase==-1) continue;

			long nSizeJoint = pFEM->GetJointSize();
			for(long ix = 0; ix < nSizeJoint; ix++)
			{
				CJoint *pJoint = pFEM->GetJoint((USHORT)ix);
				if(pJoint->m_BoolAttrib[BOOL_4_JOINT_SHOE_POS] == FALSE) continue;
				if(pJoint->m_LongAttrib[LONG_1_JOINT_JIJUMNO] != nJijum) continue;
				CResultForce *pResult = pJoint->GetResultForce(nLoadCase);
				if(pResult == NULL) continue;

				dHoriForce = max(dHoriForce, m_bUsePadShoe ? fabs(pResult->m_SpringForce.x) : fabs(pResult->m_Force.x));
				break;
			}	

		}
	}

	if(nShoe != -1)
	{
		dHoriForce /= nQtyShoe;
	}

	return dHoriForce;
}


// nJijum	 = 지점번호
// nShoe	 = 교량받침번호, 전열+후열
// nTypeLoad = #define BRIDGE_LOAD_DEAD		 0 // 고정하중 참조 ARcBridgeDataStd
// b2DFrame  = TRUE - 2D 해석결과, FALSE - 3D 해석결과
double CRcBridgeApp::GetShoeReactionUsd(long nJijum, long nShoe, long nTypeLoad, BOOL b2DFrame, BOOL bUnitMeter)
{
	CFEMManage *pFEM = b2DFrame ? &m_FemNormal : &m_FemShell;
	if(pFEM->IsOutputLoaded()==FALSE) return 0;

	//	if(b2DFrame==FALSE) return 0; // 임시....

	double dReaction = 0;

	CString szLoad = m_pARcBridgeDataStd->GetStringLoadCase(nTypeLoad);
	long nLoadCase = pFEM->GetLoadCaseByString(szLoad);
	if(nLoadCase==-1) // 활하중 및 지점침하는 COMBO로 찾아야 함.....
	{
		szLoad += " MAX";
		nLoadCase = pFEM->GetLoadCaseByString(szLoad);
	}
	// SU LoadCase는 값이 없으므로 무조건 SU MAX에서 읽어온다.
	if(szLoad == "SU")
	{
		szLoad += " MAX";
		nLoadCase = pFEM->GetLoadCaseByString(szLoad);
	}

	dReaction = GetShoeReaction(nJijum, nShoe, nLoadCase, b2DFrame, bUnitMeter);

	// 활하중일때 보도 활하중 추가.
	if(nTypeLoad == BRIDGE_LOAD_LIVE_FULL)
	{
		szLoad = "FW";
		long nLoadCase = pFEM->GetLoadCaseByString(szLoad);

		dReaction += GetShoeReaction(nJijum, nShoe, nLoadCase, b2DFrame, bUnitMeter);
	}
	
	return dReaction;
}

double CRcBridgeApp::GetShoeReactionLsd(long nJijum, long nShoe, long nTypeLoad, BOOL b2DFrame, BOOL bUnitMeter)
{
	CFEMManage *pFEM = b2DFrame ? &m_FemNormal : &m_FemShell;
	if(pFEM->IsOutputLoaded()==FALSE) return 0;

	double dReaction = 0;

	CString szLoad(_T(""));
	szLoad = m_plsdManager->GetStringLoadCaseLsd((ELSDLoadCaseKind)nTypeLoad, TRUE);

	if(nTypeLoad == eLSD_LOADCASE_LL1 || nTypeLoad == eLSD_LOADCASE_SD)
	{
		szLoad += " MAX";
	}

	long ixLoadCase = pFEM->GetLoadCaseByString(szLoad);
	if(ixLoadCase > -1)
	{
		dReaction = GetShoeReaction(nJijum, nShoe, ixLoadCase, b2DFrame, bUnitMeter);
	}

	// 활하중일때 보도 활하중 추가.
	if(nTypeLoad == eLSD_LOADCASE_LL1)
	{
		szLoad = m_plsdManager->GetStringLoadCaseLsd(eLSD_LOADCASE_PL, TRUE);

		ixLoadCase = pFEM->GetLoadCaseByString(szLoad);
		if(ixLoadCase > -1)
		{
			dReaction += GetShoeReaction(nJijum, nShoe, ixLoadCase, b2DFrame, bUnitMeter);
		}
	}
		
	return dReaction;
}

double CRcBridgeApp::GetShoeReaction(long nJijum, long nShoe, long ixLoadCase, BOOL b2DFrame, BOOL bUnitMeter)
{
	CFEMManage *pFEM = b2DFrame ? &m_FemNormal : &m_FemShell;
	if(pFEM->IsOutputLoaded()==FALSE) return 0;

	BOOL bLsd = m_pARcBridgeDataStd->IsLsdDesign();
	double dReaction = 0;
	double dSlabWidth = 1.00;
	if(bUnitMeter == FALSE)
	{
		double dSta = GetStationOnJijum(nJijum, 0);
		dSlabWidth = bLsd? GetWidthSlabAct(dSta, CDPoint(0, 1)) : toM(GetWidthSlabAct(dSta, CDPoint(0, 1)));
	}
	
	long nSizeJoint = pFEM->GetJointSize();
	long i=0; for(i=0; i<nSizeJoint; i++)
	{
		if(ixLoadCase==-1) break;

		//ARCBRIDGE-3113
		//Midias Output인경우 Spring을 따로 분리하지 않는다.
		//탄성받침인 경우에도 m_Force를 사용.
		//CLoadMidasOutput::ReadReaction(CArchive &ar), CLoadMidasOutput::ConvertToFEMManage()
		BOOL bMidas = (m_nSelect2DFrameTool==SOLVE_MIDAS);

		CJoint *pJoint = pFEM->GetJoint((USHORT)i);
		if(b2DFrame)
		{
			if(pJoint->m_BoolAttrib[BOOL_4_JOINT_SHOE_POS] && 
			   pJoint->m_LongAttrib[LONG_1_JOINT_JIJUMNO]==nJijum)
			{
				CResultForce *pResult = pJoint->GetResultForce(ixLoadCase);
				if(pResult == NULL) continue;
				dReaction = (bMidas==FALSE && m_bUsePadShoe) ? pResult->m_SpringForce.z*dSlabWidth : pResult->m_Force.z*dSlabWidth;
				break;
			}
		}
		else
		{
			if(pJoint->m_LongAttrib[LONGSHELL_1_JIJUMNO]==nJijum && 
			   pJoint->m_LongAttrib[LONGSHELL_2_SHOE_POS]==nShoe)
			{
				CResultForce *pResult = pJoint->GetResultForce(ixLoadCase);
				if(pResult == NULL) continue;
				dReaction = /*m_bUsePadShoe ? pResult->m_SpringForce.z : */pResult->m_Force.z;
				break;
			}
		}
	}

	return dReaction;
}

BOOL CRcBridgeApp::IsShoeReaction2DApply(long nJijum, long nShoe)
{
	if(m_FemShell.IsOutputLoaded() == FALSE) return TRUE;

	BOOL bLsd = m_pARcBridgeDataStd->IsLsdDesign();
	double dV1=0, dV2=0, dV3=0;
	double SlabWidth = bLsd? (m_dWidthSlabRight - m_dWidthSlabLeft) : toM(m_dWidthSlabRight - m_dWidthSlabLeft);

	long nQtyShoeJijum  = 0;
	CJijum *pJijum = GetJijum(nJijum);
	nQtyShoeJijum += pJijum->m_nQtyShoe[0];
	nQtyShoeJijum += pJijum->m_nQtyShoe[1];

	if(nQtyShoeJijum==0) return TRUE;

	//////////////////////////////////////////////////////////////////////////
	double dReaction2D = 0;
	double dReaction3D = 0;

	if(bLsd)
	{
		dV1 = GetShoeReactionLsd(nJijum,-1,eLSD_LOADCASE_TU21 ,TRUE,TRUE);
		dV2 = GetShoeReactionLsd(nJijum,-1,eLSD_LOADCASE_TU22,TRUE,TRUE);
		dV3 = GetShoeReactionLsd(nJijum,-1,eLSD_LOADCASE_SH,TRUE,TRUE);
		//
		dReaction2D += GetShoeReactionLsd(nJijum,-1,eLSD_LOADCASE_DC1,TRUE,TRUE);
		dReaction2D += GetShoeReactionLsd(nJijum,-1,eLSD_LOADCASE_DW1,TRUE,TRUE);
		dReaction2D += GetShoeReactionLsd(nJijum,-1,eLSD_LOADCASE_LL1,TRUE,TRUE);
		dReaction2D += max(dV1+dV3,dV2+dV3);
		dReaction2D += GetShoeReactionLsd(nJijum,-1,eLSD_LOADCASE_SD,TRUE,TRUE);
		dReaction2D *= (SlabWidth/(double)nQtyShoeJijum);

		//////////////////////////////////////////////////////////////////////////
		dV1 = GetShoeReactionLsd(nJijum,nShoe,eLSD_LOADCASE_TU21 ,FALSE,TRUE);
		dV2 = GetShoeReactionLsd(nJijum,nShoe,eLSD_LOADCASE_TU22 ,FALSE,TRUE);
		dV3 = GetShoeReactionLsd(nJijum,nShoe,eLSD_LOADCASE_SH,FALSE,TRUE);
		//
		dReaction3D += GetShoeReactionLsd(nJijum,nShoe,eLSD_LOADCASE_DC1,FALSE,TRUE);
		dReaction3D += GetShoeReactionLsd(nJijum,nShoe,eLSD_LOADCASE_DW1,FALSE,TRUE);
		dReaction3D += GetShoeReactionLsd(nJijum,nShoe,eLSD_LOADCASE_LL1,FALSE,TRUE);
		dReaction3D += max(dV1+dV3,dV2+dV3);
		dReaction3D += GetShoeReactionLsd(nJijum,nShoe,eLSD_LOADCASE_SD,FALSE,TRUE);
	}
	else
	{
		dV1 = GetShoeReactionUsd(nJijum,-1,BRIDGE_LOAD_TEMP1 ,TRUE,TRUE);
		dV2 = GetShoeReactionUsd(nJijum,-1,BRIDGE_LOAD_TEMP2 ,TRUE,TRUE);
		dV3 = GetShoeReactionUsd(nJijum,-1,BRIDGE_LOAD_SHRINK,TRUE,TRUE);
		//
		dReaction2D += GetShoeReactionUsd(nJijum,-1,BRIDGE_LOAD_DEAD,TRUE,TRUE);
		dReaction2D += GetShoeReactionUsd(nJijum,-1,BRIDGE_LOAD_LIVE_FULL,TRUE,TRUE);
		dReaction2D += max(dV1+dV3,dV2+dV3);
		dReaction2D += GetShoeReactionUsd(nJijum,-1,BRIDGE_LOAD_SUBSID,TRUE,TRUE);
		dReaction2D *= (SlabWidth/(double)nQtyShoeJijum);

		//////////////////////////////////////////////////////////////////////////
		dV1 = GetShoeReactionUsd(nJijum,nShoe,BRIDGE_LOAD_TEMP1 ,FALSE,TRUE);
		dV2 = GetShoeReactionUsd(nJijum,nShoe,BRIDGE_LOAD_TEMP2 ,FALSE,TRUE);
		dV3 = GetShoeReactionUsd(nJijum,nShoe,BRIDGE_LOAD_SHRINK,FALSE,TRUE);
		//
		dReaction3D += GetShoeReactionUsd(nJijum,nShoe,BRIDGE_LOAD_DEAD,FALSE,TRUE);
		dReaction3D += GetShoeReactionUsd(nJijum,nShoe,BRIDGE_LOAD_LIVE_FULL,FALSE,TRUE);
		dReaction3D += max(dV1+dV3,dV2+dV3);
		dReaction3D += GetShoeReactionUsd(nJijum,nShoe,BRIDGE_LOAD_SUBSID,FALSE,TRUE);
	}
	

	//////////////////////////////////////////////////////////////////////////
	BOOL bApply2D = (dReaction2D>=dReaction3D) ? TRUE : FALSE;

	return bApply2D;

}
//<summary>
// 현재 스테이션, 각도에서 nHDan 구간의 길이를 구함
//</summary>
//<param name="sta"></param>
//<param name="vAng"></param>
//<param name="nHDan"></param>
//<param name="bRoadNotSlab">TRUE=지중라멘 도로(철도) 노면, FALSE=슬래브면</param>
//<return></return>
// vAng : 원곡선의 경우 수정된 비율을 넣어준다.
double CRcBridgeApp::GetLengthHDanAct(double sta, CDPoint vAng, long nHDan, BOOL bRoadNotSlab, BOOL bExp, CDPoint &vAngReal)
{
	// 원곡선이고 사거리에 가각이나 확폭이면 원래방법으로 구할수 없다.
	// 새로운 방법으로 구한다.
	if(!m_bIsIncludeOnlyLine && !Compare(vAng.GetAngleDegree(), 90.0, "=") && (m_bIsExp || IsExtLine()))
	{
		double dLenRet	= GetLengthHDan(nHDan,bRoadNotSlab);
		
		// 실제사거리와의 비를 따짐.
		double dDistPrevFromLeft	= 0;
		double dDistCurFromLeft		= 0;
		long i = 0; for(i = 0; i <= nHDan; i++)
		{
			if(i <= nHDan)
				dDistCurFromLeft += GetLengthHDan(i, bRoadNotSlab);
			if(i <= nHDan-1)
				dDistPrevFromLeft += GetLengthHDan(i, bRoadNotSlab);
		}
		
		// 입력받는 위치에서의 실제사거리와 비를 따져야 됨.
		double dStaCheck	= sta;
		if(IsExtLine())
			dStaCheck	= GetStationBridgeMid();

		double dWidthLeft	= bExp ? GetWidthSlabActByGagak(dStaCheck, CDPoint(0, 1), -1, bRoadNotSlab) : GetWidthSlabAct(dStaCheck, CDPoint(0, 1), -1, FALSE, FALSE, bRoadNotSlab);
		dDistPrevFromLeft	= -dWidthLeft + dDistPrevFromLeft;
		dDistCurFromLeft	= -dWidthLeft + dDistCurFromLeft;
		
		CLineInfo *pLine	= GetLineBase();
		double dStaPrev	= pLine->GetStationByScInLine(dStaCheck, dDistPrevFromLeft, vAng, 0);
		double dStaCur	= pLine->GetStationByScInLine(dStaCheck, dDistCurFromLeft, vAng, 0);
		CDPoint xyPrev	= pLine->GetXyLineDis(dStaPrev, dDistPrevFromLeft);
		CDPoint xyCur	= pLine->GetXyLineDis(dStaCur, dDistCurFromLeft);

		double dLen	= GetLengthHDan(nHDan, bRoadNotSlab);
		double dLenAng	= ~(xyPrev - xyCur);
		
		double dRate	= (dLenAng > 0)? dLen / dLenAng : 1.;

		// 변화구간 적용
		if(IsVariableLenHDan(nHDan, bRoadNotSlab) && (bExp || IsExtLine()))
		{
			long nCount	= GetCountVariableLenHDan(bRoadNotSlab);
			if(nCount > 0)
			{
				double dStaMid = GetStationBridgeMid(); // 횡단입력 기준....
				
				// 입력받은 거리
				// 입력받은 직거리를 중심선형기준으로 사거리로 만들었을때의 거리임
				// 가변구간은 실제로 적용된 사거리를 제외한 거리로 적용해야 하기 때문임
				// 이 거리는 슬래브 좌우측과 정확히 일치하지 않을 수 있음(확폭일때만)
				double disBaseLeft = GetDisSlabLeftAct(dStaMid, CDPoint(0, 1), bRoadNotSlab);
				double disBaseRight = GetDisSlabRightAct(dStaMid, CDPoint(0, 1), bRoadNotSlab);

				// (ARCBRIDGE-1610) 여기서 dStaMid를 사용한다면 곡선선형일경우에 지점 각도의 차이에 의해서 길이가 잘못 계산된다.
				double dStaBaseLeft	= pLine->GetStationByScInLine(dStaCheck, disBaseLeft, vAng, 0);
				double dStaBaseRight	= pLine->GetStationByScInLine(dStaCheck, disBaseRight, vAng, 0);
				CDPoint xyBaseLeft	= pLine->GetXyLineDis(dStaBaseLeft, disBaseLeft);
				CDPoint xyBaseRight	= pLine->GetXyLineDis(dStaBaseRight, disBaseRight);
				CDPoint xyBaseCen	= pLine->GetXyLineDis(dStaCheck, 0);
				if(disBaseLeft < 0)
					disBaseLeft	= ~(xyBaseLeft - xyBaseCen) * -1;
				else
					disBaseLeft	= ~(xyBaseLeft - xyBaseCen);
				if(disBaseRight < 0)
					disBaseRight	= ~(xyBaseRight - xyBaseCen) * -1;
				else
					disBaseRight	= ~(xyBaseRight - xyBaseCen);

				// 실제 거리(d
				double disStaLeft = GetDisSlabLeftActByGagak(sta, vAng, bRoadNotSlab);
				double disStaRight = GetDisSlabRighActByGagak(sta, vAng, bRoadNotSlab);

				if(nCount == 1) // 가변구간에 의해 차이가 나는 길이를 모두 몰아주면된다.
				{
					//dLenAng	= dLenAng + widVariables;
					double widVariables = (disBaseLeft - disStaLeft) + (disStaRight - disBaseRight);
					dLenAng = dLenAng + widVariables;
				}
				// 가변인 경우 도로중심에 횡단이 끊어져 있어야 하고 사용자가 가변구간을 체크해야 한다.
				else if(IsLeftHDan(nHDan,bRoadNotSlab))
				{
					double widVariables = disBaseLeft - disStaLeft;
					long nCountLeft = GetCountVariableHDanLeft(bRoadNotSlab);
					
					dLenAng	= dLenAng + widVariables/nCountLeft;
				}
				else
				{
					double widVariables = disStaRight - disBaseRight;
					long nCountRight = nCount - GetCountVariableHDanLeft(bRoadNotSlab);
					
					dLenAng	= dLenAng + widVariables/nCountRight;
				}

				dRate	= dLen / dLenAng;
			}
		}


		return dLenRet / dRate;
	}

	if(!IsExp(sta))
		bExp	= FALSE;

	long nCount	= GetCountVariableLenHDan(bRoadNotSlab);
	double dRate	= vAng.y;
	vAngReal	= vAng;

	// 사거리이고 원곡선이라면 그냥 사거리 적용하면 절대 안됨. ㅡㅡ; /////////////////////
	// 가각과 확폭일때도 물론 적용하면 안되지만, 현재로선 방법이 없어서 원래대로 함.
	if(!m_bIsIncludeOnlyLine)
	{
		if(!Compare(vAng.GetAngleDegree(), 90.0, "=") && !m_bIsExp && !IsExtLine()) 
		{
			// 실제로 이곳에서는 가변구간이 필요가 없다
			nCount	= 0;

			double dDistPrevFromLeft	= 0;
			double dDistCurFromLeft		= 0;
			long i = 0; for(i = 0; i <= nHDan; i++)
			{
				if(i <= nHDan)
					dDistCurFromLeft += GetLengthHDan(i, bRoadNotSlab);
				if(i <= nHDan-1)
					dDistPrevFromLeft += GetLengthHDan(i, bRoadNotSlab);
			}

			double dWidthLeft	= bExp ? GetWidthSlabActByGagak(sta, CDPoint(0, 1), -1, bRoadNotSlab) : GetWidthSlabAct(sta, CDPoint(0, 1), -1, FALSE, FALSE, bRoadNotSlab);
			dDistPrevFromLeft	= -dWidthLeft + dDistPrevFromLeft;
			dDistCurFromLeft	= -dWidthLeft + dDistCurFromLeft;

			CLineInfo *pLine	= GetLineBase();
			double dStaPrev	= pLine->GetStationByScInLine(sta, dDistPrevFromLeft, vAng, 0);
			double dStaCur	= pLine->GetStationByScInLine(sta, dDistCurFromLeft, vAng, 0);
			CDPoint xyPrev	= pLine->GetXyLineDis(dStaPrev, dDistPrevFromLeft);
			CDPoint xyCur	= pLine->GetXyLineDis(dStaCur, dDistCurFromLeft);
			double dLen	= GetLengthHDan(nHDan, bRoadNotSlab);
			double dLenAng	= ~(xyPrev - xyCur);

			dRate	= (dLenAng > 0)? dLen / dLenAng : 1;
			vAngReal.y	= dRate;
			vAng.y	= dRate;
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////

	// 가변구간 사용자 체크
	if(nCount > 0)
	{
		if(IsVariableLenHDan(nHDan, bRoadNotSlab))
		{   if(bExp)
			{
				double dStaMid = GetStationBridgeMid(); // 횡단입력 기준....
				double disBaseLeft = GetDisSlabLeftAct(dStaMid, vAng, bRoadNotSlab);
				double disBaseRight = GetDisSlabRightAct(dStaMid, vAng, bRoadNotSlab);
				double disStaLeft = GetDisSlabLeftActByGagak(sta, vAng, bRoadNotSlab);
				double disStaRight = GetDisSlabRighActByGagak(sta, vAng, bRoadNotSlab);
 				if(m_bSetGagakDist && Compare(vAng.GetAngleDegree(),90.0,"="))
 				{
 					disStaLeft = m_dDisExpLeft;
 					disStaRight = m_dDisExpRight;
 				}

				// 가변인 경우 도로중심에 횡단이 끊어져 있어야 하고 사용자가 가변구간을 체크해야 한다.
				if(IsLeftHDan(nHDan,bRoadNotSlab))
				{
					double widVariables = disBaseLeft - disStaLeft;

					double dLenHDan = GetLengthHDan(nHDan, bRoadNotSlab)/dRate;
					long nCountLeft = GetCountVariableHDanLeft(bRoadNotSlab);

					return dLenHDan + widVariables/nCountLeft;
				}
				else
				{
					double widVariables = disStaRight - disBaseRight;
					
					double dLenHDan = GetLengthHDan(nHDan, bRoadNotSlab)/dRate;
					long nCountRight = nCount - GetCountVariableHDanLeft(bRoadNotSlab);
					
					return dLenHDan + widVariables/nCountRight;

				}
			}
			else
			{
				double disLeftBase = bExp ? GetDisSlabLeftActByGagak(sta, vAng, bRoadNotSlab) : GetDisSlabLeftAct(sta, vAng, bRoadNotSlab); //GetStationBridgeMid() 가 입력 기준임
				double disRighBase = bExp ? GetDisSlabRighActByGagak(sta, vAng, bRoadNotSlab) : GetDisSlabRightAct(sta, vAng, bRoadNotSlab);//GetStationBridgeMid() 가 입력 기준임
				
				double wid	= disRighBase - disLeftBase;
				double disHDanTotal	= 0;
				long n=0; for(n=0; n<GetQtyHDan(bRoadNotSlab); n++)
					disHDanTotal += GetLengthHDan(n, bRoadNotSlab)/dRate;

				return (GetLengthHDan(nHDan, bRoadNotSlab)/dRate + (wid-disHDanTotal)/nCount);
			}
		}
	}
	else
	{
		// 가변구간 자동 분배
		if(nHDan==GetNumHdanVariableLenAutoLeft(bRoadNotSlab))
		{
			double disLeftBase = bExp ? GetDisSlabLeftActByGagak(GetStationBridgeMid(), CDPoint(0,1), bRoadNotSlab) : GetDisSlabLeftAct(GetStationBridgeMid(), CDPoint(0, 1), bRoadNotSlab); //GetStationBridgeMid() 가 입력 기준임
			double disLeftSta = bExp ? GetDisSlabLeftActByGagak(sta, CDPoint(0,1), bRoadNotSlab) : GetDisSlabLeftAct(sta, CDPoint(0,1), bRoadNotSlab);
			double disRighBase = bExp ? GetDisSlabRighActByGagak(GetStationBridgeMid(), CDPoint(0,1), bRoadNotSlab) : GetDisSlabRightAct(GetStationBridgeMid(), CDPoint(0,1), bRoadNotSlab);//GetStationBridgeMid() 가 입력 기준임
			
			double wid = disRighBase-disLeftBase;
			double disHDanTotal = 0;
			long n=0; for(n=0; n<GetQtyHDan(bRoadNotSlab); n++)
				disHDanTotal += GetLengthHDan(n, bRoadNotSlab);

			return (GetLengthHDan(nHDan, bRoadNotSlab)-(disLeftSta-disLeftBase)+(wid-disHDanTotal)) / dRate;
		}
		else if(nHDan==GetNumHdanVariableLenAutoRigh(bRoadNotSlab))
		{
			double disLeftBase = bExp ? GetDisSlabLeftActByGagak(GetStationBridgeMid(), CDPoint(0,1), bRoadNotSlab) : GetDisSlabLeftAct(GetStationBridgeMid(), CDPoint(0, 1), bRoadNotSlab);//GetStationBridgeMid() 가 입력 기준임
			double disRighBase = bExp ? GetDisSlabRighActByGagak(GetStationBridgeMid(), CDPoint(0,1), bRoadNotSlab) : GetDisSlabRightAct(GetStationBridgeMid(), CDPoint(0, 1), bRoadNotSlab);//GetStationBridgeMid() 가 입력 기준임
			double disRighSta = bExp ? GetDisSlabRighActByGagak(sta, CDPoint(0,1), bRoadNotSlab) : GetDisSlabRightAct(sta, CDPoint(0, 1), bRoadNotSlab);
			double wid = 0;
			double disHDanTotal = 0;
			if(disLeftBase > 0)	//좌측이 선형 우측에 있을 경우에만, 좌측에 가변구간이 있으면 좌측에서만 처리함.(전체 너비이기 때문에)
			{
				wid = disRighBase-disLeftBase;
				long n=0; for(n=0; n<GetQtyHDan(bRoadNotSlab); n++)
					disHDanTotal += GetLengthHDan(n, bRoadNotSlab);
			}
			return (GetLengthHDan(nHDan, bRoadNotSlab)+(disRighSta-disRighBase)+(wid-disHDanTotal)) / dRate;
		}
	}

	//가변 구간 아닐 경우 
	return GetLengthHDan(nHDan,bRoadNotSlab) / dRate;
}
//<summary>
// 도로중심 좌측 슬래브 중에서 가변구간 번호(nHDan)를 구함
//</summary>
//<param name="bRoadNotSlab">TRUE=지중라멘 도로(철도) 노면, FALSE=슬래브면</param>
//<return></return>
long CRcBridgeApp::GetNumHdanVariableLenAutoLeft(BOOL bRoadNotSlab)
{
	CDPoint vAng = CDPoint(0, 1);
	double disLeft = GetDisSlabLeftAct(GetStationBridgeMid(), vAng, bRoadNotSlab);
	if(disLeft > 0)	return -1;	//좌측 끝단이 선형 우측에 있을 경우에는 좌측에는 자동 가변 구간 없음.

	long nQtyHDan = GetQtyHDan(bRoadNotSlab);
	double disFrHDanLeft = disLeft;
	long n = 0; for(n = 0; n<nQtyHDan; n++)
	{
		disFrHDanLeft += GetLengthHDan(n,bRoadNotSlab);	//입력한 횡단
		if(disFrHDanLeft >= 0)
		{
			if(n<1)
			{
				return 0;
			}
			else
			{
				return n-1;
			}
		}
	}
	//슬래브 우측 끝단이 선형 좌측에 있는 경우
	return (long)(nQtyHDan/2);
}
//<summary>
// 도로중심 우측 슬래브 중에서 가변구간 번호(nHDan)를 구함
//</summary>
//<param name="bRoadNotSlab">TRUE=지중라멘 도로(철도) 노면, FALSE=슬래브면</param>
//<return></return>
long CRcBridgeApp::GetNumHdanVariableLenAutoRigh(BOOL bRoadNotSlab)
{
	CDPoint vAng = CDPoint(0, 1);
	double disRigh = GetDisSlabRightAct(GetStationBridgeMid(), vAng, bRoadNotSlab);
	if(disRigh < 0)	return -1;	//좌측 끝단이 선형 우측에 있을 경우에는 좌측에는 자동 가변 구간 없음.

	long nQtyHDan = GetQtyHDan(bRoadNotSlab);
	double disFrHDanRigh = disRigh;
	long n = 0; for(n = nQtyHDan-1; n>=0; n--)
	{
		disFrHDanRigh -= GetLengthHDan(n,bRoadNotSlab);	//입력한 횡단
		if(disFrHDanRigh <= 0)
		{
			if(n>nQtyHDan-2)
			{
				return nQtyHDan-1;
			}
			else
			{
				return n+1;
			}
		}
	}
	//슬래브 좌측 끝단이 선형 우측에 있는 경우
	return (long)(nQtyHDan/2)+1;
}

//<summary>
// nHDan이 가변구간인지를 점검
//</summary>
//<param name="bRoadNotSlab">TRUE=지중라멘 도로(철도) 노면, FALSE=슬래브면</param>
//<return></return>
// BOOL CRcBridgeApp::IsVariableLenHdanAuto(long nHDan, BOOL bRoadNotSlab)
// {
// 	if(nHDan == GetNumHdanVariableLenAutoLeft(bRoadNotSlab))
// 		return TRUE;
// 	if(nHDan == GetNumHdanVariableLenAutoRigh(bRoadNotSlab))
// 		return TRUE;
// 
// 	return FALSE;
// }

//<summary>
// 기준선형 sta, vAng로 좌우측 선형중심까지의 길이를 구함
//</summary>
//<param name="sta">기준선형 스테이션</param>
//<param name="vAng">기준선형 각도</param>
//<param name="bLeft">TRUE=좌측선형, FALSE=우측선형</param>
//<return></return>
double CRcBridgeApp::GetDistLineLeftOrRight(double sta, CDPoint vAng, BOOL bLeft)
{
	if(GetLineBase() == GetLineLeftRight(bLeft))	return 0;
	double staMatch = GetStationMatchLineLeftOrRight(sta, vAng, bLeft);
	CDPoint xy = GetLineLeftRight(bLeft)->GetXyLine(staMatch);
	double dis = GetLineBase()->GetDisMatchVerticalByXy(xy);

	return dis;
}
//<summary>
// 기준선형 sta, vAng로 만나는 좌우측 선형중심 스테이션을 구함
//</summary>
//<param name="sta">기준선형 스테이션</param>
//<param name="vAng">기준선형 각도</param>
//<param name="bLeft">TRUE=좌측선형, FALSE=우측선형</param>
//<return></return>
double CRcBridgeApp::GetStationMatchLineLeftOrRight(double sta, CDPoint vAng, BOOL bLeft)
{
	if(GetLineBase() == GetLineLeftRight(bLeft))	return sta;

	CDPoint xyC = GetLineBase()->GetXyLine(sta);
	CDPoint vAngAbs = GetLineBase()->GetAngleAzimuth(sta).Rotate(vAng);
	double staMatch = 0;
	GetLineLeftRight(bLeft)->GetStationMatchByXyAndAngle(xyC, vAngAbs, staMatch);

	return staMatch;
}

//<summary>
// 기준선형 sta, vAng로 만나는 좌우측 선형중심 각도를 구함
//</summary>
//<param name="sta">기준선형 스테이션</param>
//<param name="vAng">기준선형 각도</param>
//<param name="bLeft">TRUE=좌측선형, FALSE=우측선형</param>
//<return></return>
CDPoint CRcBridgeApp::GetAngMatchLineLeftOrRight(double sta, CDPoint vAng, BOOL bLeft)
{
	if(GetLineBase() == GetLineLeftRight(bLeft))	return vAng;

	CDPoint xyC = GetLineBase()->GetXyLine(sta);
	CDPoint vAngAbs = GetLineBase()->GetAngleAzimuth(sta).Rotate(vAng);
	double staMatch = 0;
	GetLineLeftRight(bLeft)->GetStationMatchByXyAndAngle(xyC, vAngAbs, staMatch);

	CDPoint vAngResult = GetLineLeftRight(bLeft)->GetAngleAzimuth(staMatch).BetweenAngle(vAngAbs).Unit();
	return vAngResult;
}
//<summary>
// 기준선형 sta, vAng로 만나는 좌우측 선형에서 슬래브 끝단까지의 너비를 구함
//</summary>
//<param name="sta">기준선형 스테이션</param>
//<param name="vAng">기준선형 각도</param>
//<param name="bLeft">TRUE=좌측선형, FALSE=우측선형</param>
//<return></return>
double CRcBridgeApp::GetWidthSlabFrLineLeftOrRight(double sta, CDPoint vAng, BOOL bLeft)
{
	double widSlab = bLeft ? m_dWidthSlabLeft : m_dWidthSlabRight;
	if(GetLineBase() == GetLineLeftRight(bLeft))
		return widSlab/vAng.y;

	CDPoint vAngMatch = GetAngMatchLineLeftOrRight(sta, vAng, bLeft);
	return widSlab/vAngMatch.y;
}
//<summary>
// 기준선형 sta, vAng로 만나는 좌우측 선형에서 슬래브 끝단까지의 너비를 구함
//</summary>
//<param name="sta">기준선형 스테이션</param>
//<param name="vAng">기준선형 각도</param>
//<param name="bLeft">TRUE=좌측선형, FALSE=우측선형</param>
//<return></return>
double CRcBridgeApp::GetStationBridgeMid()
{
	return m_dStationBridgeStt + m_dLengthBridge/2;
}


// double CRcBridgeApp::GetDiffELSttAndJijumForVertSectionBySta(double dSta, BOOL bApplyByJongdanSlope)
// {
// 	CLineInfo *pLine	= GetLineBase();
// 	double dStaStt	= GetStationByTarget(BRIDGE_OFF_STT_SLAB, 0, TRUE);
// 
// 	double dELStt	= pLine->GetElevationPlanEl(dStaStt);
// 	double dEL		= pLine->GetElevationPlanEl(dSta);
// 
// 	return dEL - dELStt;
// }


// 종단면을 level로 그릴 때 슬래브 끝단과 지점에서의 기초 el을 구하기 위함.
// BOOL bApplyByJongdanSlope 이 TRUE이면 
// 단 경사로 그릴때, nJ가 중간벽체 이면 일반도의 el과 구조도 옵션에서의 el과의 차이를 리턴함
// BOOL bInWallLR 내측벽체의 중심이아닌 좌우측 선택 bInner == TRUE 면 좌측
double CRcBridgeApp::GetDiffELSttAndJijumForVertSection(long nJ, BOOL bInner, BOOL bApplyByJongdanSlope, BOOL bInWallLR)
{
	// Box타입이고 하단이 수평인경우 EL의 이동이 없지..
	// 하단이 수평이 아닌경우도 Box타입은 종단면 위치 구할때마다 Sync를 맞추기 때문에 종단타입에 따른 높이차를 구하지 않는다.
	// 벽체구조도를 그릴때 Level로 그리려면 벽체 일반도를 이동해 줘야 하기 때문에 필요하다. 일단 구해주고 쓰는데서 수정해야한다.
	// if(IsBoxType() /*&& !m_bSlopeLowerSlab*/) return 0;
	if(m_nTypeJongdanForRebar == 1 && nJ > 0 && nJ < m_nQtyJigan)
	{
		double dSta	= GetStationOnJijum(nJ);
		CDPoint xy	= GetXyMatchLineAndSlabUpper(CDPoint(dSta, 0), CDPoint(0, 1), 0, -GetThickPave());
		CLineInfo *pLine	= GetLineBase();
		double dEl	= pLine->GetElevationPlanEl(dSta);
		return dEl - xy.y;
	}

	if(m_nTypeJongdanForRebar != 0) return 0;
	if(nJ > m_nQtyJigan) return 0;

	CLineInfo *pLine	= GetLineBase();
	double dStaStt	= GetStationByTarget(BRIDGE_OFF_STT_SLAB, 0, TRUE);
	double dStaEnd	= GetStationOnJijum(nJ);

	if(nJ == 0)
		dStaEnd	= GetStationByTarget(bInner ? BRIDGE_OFF_STT_STT : BRIDGE_OFF_STT_SLAB, 0, TRUE);
	else if(nJ == m_nQtyJigan)
		dStaEnd	= GetStationByTarget(bInner ? BRIDGE_OFF_END_END : BRIDGE_OFF_END_SLAB, 0, TRUE);
	else if(bInWallLR)
	{
		CWallApp *pWall = GetInWall(nJ-1);
		if(pWall)
		{
			CDPoint vAng	= ToDPointFrDegree(pWall->GetAngleWallByHor(TRUE));
			dStaEnd += pWall->m_dW / vAng.y / 2. * (bInner? -1 : 1);		// bInner면 좌측
		}
	}

	double dELStt	= pLine->GetElevationPlanEl(dStaStt);
	double dELEnd	= pLine->GetElevationPlanEl(dStaEnd);

	return dELEnd - dELStt;
}


// 말뚝의 N값 권고안.(보링 정보가 있어야 되므로 여기서 권고안 함)
// void CRcBridgeApp::SetDataDefaultPileN(long nJ)
// {
// 	CBoringDataEx *pBoringEx	= GetBoringDataExByJijumNumber(nJ);
// 	if(!pBoringEx) return;
// 
// 	CFootingApp *pFooting	= GetFootingByJijumNumber(nJ);
// 	if(!pFooting) return;
// 	if(!pFooting->m_bIs) return;
// 	if(pFooting->m_exFooting.m_nType != EXFOOTING_TYPE_PILE) return;
// 
// 	CExPileApp *pPile		= &pFooting->m_exFooting.m_Pile;
// 	double dELFootingLower	= pFooting->m_dEL - pFooting->GetHeight();
// 	double dELPileLower		= dELFootingLower - pPile->m_dLength + 200;
// 
// 	// 기초 하면과 같은 el의 boring과 말뚝 하단과 같은 el의 boring을 찾음
// 	CBoringData *pBoringFootingLower	= NULL;
// 	CBoringData *pBoringPileLower		= NULL;
// 	
// 	long i = 0; for(i = 0; i < pBoringEx->m_pArrBoring.GetSize(); i++)
// 	{
// 		double dELTop	= 0;
// 		double dELBottom	= 0;
// 
// 		CBoringData *pBoring	= pBoringEx->m_pArrBoring.GetAt(i);
// 		dELTop	= frM(pBoringEx->m_EL - pBoring->m_dELStart);
// 		dELBottom	= frM(pBoringEx->m_EL - pBoring->m_dELStart - pBoring->m_dDepth);
// 
// 		if(!pBoringFootingLower)
// 		{
// 			if(dELTop >= dELFootingLower && dELBottom <= dELFootingLower)
// 			{
// 				pBoringFootingLower	= pBoring;
// 			}	
// 		}
// 
// 		if(!pBoringPileLower)
// 		{
// 			if(dELTop >= dELPileLower && dELBottom <= dELPileLower)
// 			{
// 				pBoringPileLower = pBoring;
// 			}
// 		}
// 
// 		if(pBoringFootingLower && pBoringPileLower) break;
// 	}
// 
// 	if(pBoringFootingLower)
// 		pPile->m_dBeta_N	= pBoringFootingLower->m_nNValue;
// 
// 	if(pBoringPileLower)
// 		pPile->m_dEdge_N	= pBoringPileLower->m_nNValue;
// }

// 옵션에 따라서 선형 검색 타입을 적용함.
void CRcBridgeApp::SetSearchIP()
{
	CLineInfo *pLine	= GetLineBase();
	if(m_nQtyJigan < 1 || GetCountJijum() < 2 || GetCountJijum() < m_nQtyJigan)
	{
		if(pLine)
			pLine->ReSetSearchIp();
		
		return;
	}

	if(m_bQuickSearchLine)
	{
		if(pLine)
		{
			pLine->ReSetSearchIp();
			double dStaSttL	= GetStationByTarget(BRIDGE_OFF_STT_SLAB, -1, TRUE);
			double dStaSttC	= GetStationByTarget(BRIDGE_OFF_STT_SLAB, 0, TRUE);
			double dStaSttR	= GetStationByTarget(BRIDGE_OFF_STT_SLAB, 1, TRUE);
			double dStaStt	= min(dStaSttC, min(dStaSttL, dStaSttR));
			double dStaEnd	= max(dStaSttC, max(dStaSttL, dStaSttR));

			if(IsOutWall(TRUE))
			{
				CDPoint vAng	= GetAngleJijum(0);
				dStaStt -= m_footingStt.GetWidth() / vAng.y;
			}

			if(IsOutWall(FALSE))
			{
				CDPoint vAng	= GetAngleJijum(m_nQtyJigan);
				dStaEnd += m_footingEnd.GetWidth() / vAng.y;
			}

			dStaStt -= 50000;
			dStaEnd += 50000;

			double dStaBp	= pLine->m_dStationBp;
			double dStaEp	= pLine->GetStationEpLine();
//			m_pLineApp->GetStationMatchVerticalByXy(m_pLineApp->m_xyBp, dStaBp);
//			m_pLineApp->GetStationMatchVerticalByXy(m_pLineApp->m_xyEp, dStaEp);

			if(dStaStt < dStaBp) dStaStt = dStaBp;
			if(dStaEnd > dStaEp) dStaEnd = dStaEp;

			pLine->SetSearchIpBySta(dStaStt,dStaEnd,TRUE);
			if(!IsValidStationOfBri())
			{
				pLine->ReSetSearchIp();
			}
		}
	}
	else
	{
		if(pLine)
		{
			pLine->ReSetSearchIp();
		}
	}
}

// 현 교량의 스테이션이 선형내에 깔끔하게 포함 되어 있는지 체크한다.
BOOL CRcBridgeApp::IsValidStationOfBri()
{
	CLineInfo *pLine	= GetLineBase();

	// 체크할 범위(정확하진 않지면 차차 여러 경우를 체크해줘야 됨)
	double dStaStt	= GetStationByTarget(BRIDGE_OFF_STT_SLAB, 0, TRUE);
	double dStaEnd	= GetStationByTarget(BRIDGE_OFF_END_SLAB, 0, TRUE);
	CDPoint vAngStt	= GetAngleByTarget(BRIDGE_OFF_STT_SLAB, 0);
	CDPoint vAngEnd	= GetAngleByTarget(BRIDGE_OFF_END_SLAB, 0);

	CDPoint xySttL	= GetXySlabEdgeActByGagak(dStaStt, vAngStt, TRUE);
	CDPoint xySttR	= GetXySlabEdgeActByGagak(dStaStt, vAngStt, FALSE);
	CDPoint xyEndL	= GetXySlabEdgeActByGagak(dStaEnd, vAngEnd, TRUE);
	CDPoint xyEndR	= GetXySlabEdgeActByGagak(dStaEnd, vAngEnd, FALSE);
	
	double dStaSttL(0), dStaSttR(0), dStaEndL(0), dStaEndR(0);
	pLine->GetStationMatchVerticalByXy(xySttL, dStaSttL);
	pLine->GetStationMatchVerticalByXy(xySttR, dStaSttR);
	pLine->GetStationMatchVerticalByXy(xyEndL, dStaEndL);
	pLine->GetStationMatchVerticalByXy(xyEndR, dStaEndR);

	double dStaSttOfBri	= min(dStaSttL, dStaSttR);
	double dStaEndOfBri	= min(dStaEndL, dStaEndR);
	if(IsBracket(TRUE))
		dStaSttOfBri	-= m_dBHWS*2;
	if(IsBracket(FALSE))
		dStaEndOfBri	+= m_dBHWE*2; 

	long stt = 0; for(stt = 0; stt < 2; stt++)
	{
		BOOL bStt	= stt == iSTT;
		if(IsOutWall(bStt))
		{
			double dSta	= dStaSttOfBri;
			GetTvWingWallPlane(bStt, FALSE, FALSE, TRUE);

			CTwinVector tv;
			long i = 0; for(i = 0; i < m_tvArrPlaneWingWall.GetSize(); i++)
			{
				tv	= m_tvArrPlaneWingWall.GetAt(i);
				BOOL bLeft	= TRUE;
				CDPoint xy(0, 0);

				if(tv.m_sInfo=="좌측날개벽외측") 
				{
					bLeft	= TRUE;
					xy	= tv.m_v2;
				}
				else if(tv.m_sInfo=="우측날개벽외측") 
				{
					bLeft	= FALSE;
					xy	= tv.m_v2;
				}
				else
					continue;
	

				if(IsWingWall(bStt, bLeft))
				{
					if(pLine->GetStationMatchVerticalByXy(xy, dSta))
					{
						if(bStt)
							dStaSttOfBri	= min(dStaSttOfBri, dSta);
						else
							dStaEndOfBri	= max(dStaEndOfBri, dSta);
					}
					else
					{
						return FALSE;
					}
				}
			}
		}
	}


	// 평면 선형 검사
	// 선형의 시작과 끝 sta
	double dStaSttOfLine	= pLine->m_dStationBp;
	double dStaEndOfLine	= pLine->GetStationEpLine();
	if(dStaSttOfBri < dStaSttOfLine || dStaEndOfBri > dStaEndOfLine) return FALSE;

	// 종단 선형 검사
	// 종단은 딱 교량의 시종점만 검사를 하면 된다.
	if(m_bUnderGroundRahmen==FALSE)
	{
		dStaSttOfLine	= pLine->m_VIPData[0][0];
		dStaEndOfLine	= pLine->m_VIPData[pLine->m_VIPsu-1][0];
		if(dStaSttOfBri < dStaSttOfLine || dStaEndOfBri > dStaEndOfLine) return FALSE;
	}

	// 편경사 고려
	long left = 0; for(left = 0; left < 2; left++)
	{
		BOOL bLeft	= left == iLEFT;
		CTypedPtrArray <CObArray,CSlop*> *pSlope	= bLeft ? &pLine->m_pSlopLeft : &pLine->m_pSlopRight;

		if(pSlope->GetSize() == 0) return FALSE;	

		dStaSttOfLine	= pSlope->GetAt(0)->m_dStationPyungubae;
		dStaEndOfLine	= pSlope->GetAt(pSlope->GetSize()-1)->m_dStationPyungubae;
		if(dStaSttOfBri < dStaSttOfLine || dStaEndOfBri > dStaEndOfLine) return FALSE;
	}


	return TRUE;
}

BOOL CRcBridgeApp::IsSkew()
{
	BOOL bSkew = FALSE;
	long i=0; for(i=0; i<m_nQtyJigan; i++)
	{
		CDPoint vDir = GetAngleJijum(i);
		double d1 = vDir.GetAngleDegree();
		if(!Compare(d1, 90.0, "="))
		{
			bSkew = TRUE;
			break;
		}
	}

	return bSkew;
}
// 내측벽체헌치에 대해서 라운드 적용함
// 단, 접선인 경우에만 여기서 적용함
void CRcBridgeApp::ApplyRoundOfInWallHunch(long nIdxInWall, BOOL bLeft)
{
	long left	= bLeft ? iLEFT : iRIGHT;
	if(m_nTypeRoundOfHunchInWall[nIdxInWall][left] != 0) return;
	if(bLeft && Compare(m_dHLR[nIdxInWall][iUPPER], 0.0, "=")) return;
	if(!bLeft && Compare(m_dHRR[nIdxInWall][iUPPER], 0.0, "=")) return;
	if(m_nTypeBridge == BRIDGE_TYPE_BOX) return;

	// 접선방식이 아니면
				
	CRebarPlacing rb;
	CWallApp *pWall	= GetInWall(nIdxInWall);
	if(pWall)
	{
		if(pWall->m_bIs)
		{
			// 번째내측헌치 우측이나 좌측면에 해당하는 tv 모두 제거
			CString sHunch	= _T("");
			CString sLeft	= bLeft ? "좌측" : "우측";
			sHunch.Format("%d번째내측헌치", nIdxInWall+1);

			CTwinVector tv;
			long i = 0; for(i = 0; i < m_tvArrVertSection.GetSize(); i++)
			{
				tv	= m_tvArrVertSection.GetAt(i);
				if(tv.m_sInfo.Find(sHunch, 0) != -1 && tv.m_sInfo.Find(sLeft, 0) != -1 && 
					tv.m_sInfo.Find("우측슬래브하면", 0) == -1 && tv.m_sInfo.Find("헌치아래면"+sLeft, 0) == -1)
				{
					//m_tvArrVertSection.RemoveAt(i);
					// i--;
				}
			}
			
			CTwinVector tvUpperWall	= pWall->m_tvArrFront.GetTvByInfo("벽체상면");

			CTwinVector tv1, tv2;
			CString sInfo1(""), sInfo2("");
			double dR	= bLeft ? m_dHLR[nIdxInWall][iUPPER] : m_dHRR[nIdxInWall][iUPPER];
			CDPoint xyMatchOrg(0, 0), xyCen(0, 0);
			if(bLeft)
			{
				sInfo1	= "벽체좌측면";
				tv1	= pWall->m_tvArrFront.GetTvByInfo(sInfo1);
				if(nIdxInWall == 0)
					sInfo2	= "슬래브하면";
				else
					sInfo2.Format("%d번째내측헌치우측슬래브하면", nIdxInWall);
				tv2	= m_tvArrVertSection.GetTvByInfo(sInfo2);

				if(GetXyMatchLineAndLine(tv1.m_v1, tv1.GetXyDir(), tv2.m_v1, tv2.GetXyDir(), xyMatchOrg))
				{
					GetXyTanOfArc(xyMatchOrg, tv1.GetXyDir(TRUE), tv2.GetXyDir(), dR, xyCen, tv1.m_v2, tv2.m_v1);
				}
				
				pWall->m_tvArrFront.SetTvByInfo(sInfo1, tv1);
				m_tvArrVertSection.SetTvByInfo(sInfo2, tv2);
				CTwinVector tvR;
				tvR.m_v1	= tv1.m_v2;
				tvR.m_v2	= tv2.m_v1;
				tvR.m_v1.z	= -1;
				tvR.m_v2.z	= dR;
				xyCen	= rb.GetXyCircleCenter(tvR.m_v1, tvR.m_v2, fabs(tvR.m_v2.z), tvR.m_v1.z==-1);
				tvR.m_xyRCenter = xyCen;
				double dLen = tvR.GetLength();
				tvR.m_sInfo.Format("%d번째내측헌치좌측면", nIdxInWall+1);
				long nIdx	= m_tvArrVertSection.GetIdxByInfo(sInfo2)-1;
				if(nIdx > -1)
				{
					// (ARCBRIDGE-3465) InsertAt()을 해주면 같은 이름의 선이 계속 추가 되기 때문에 오류가 발생한다. 같은 이름이 있다면 일단 제거하고 추가. 
					long nFind = m_tvArrVertSection.GetIdxByInfo(tvR.m_sInfo);
					if(nFind > -1)
						m_tvArrVertSection.RemoveAt(nFind, 1);

					m_tvArrVertSection.InsertAt(nIdx, tvR);
				}

				tvUpperWall.m_v1	= tv1.m_v2;
				pWall->m_tvArrFront.SetTvByInfo("벽체상면", tvUpperWall);
			}
			else
			{
				sInfo1.Format("%d번째내측헌치우측슬래브하면", nIdxInWall+1);
				tv1	= m_tvArrVertSection.GetTvByInfo(sInfo1);
				sInfo2	= "벽체우측면";
				tv2	= pWall->m_tvArrFront.GetTvByInfo(sInfo2);
				
				if(GetXyMatchLineAndLine(tv1.m_v1, tv1.GetXyDir(), tv2.m_v1, tv2.GetXyDir(), xyMatchOrg))
				{
					GetXyTanOfArc(xyMatchOrg, tv1.GetXyDir(TRUE), tv2.GetXyDir(TRUE), dR, xyCen, tv1.m_v2, tv2.m_v2);
				}
				
				m_tvArrVertSection.SetTvByInfo(sInfo1, tv1);
				CTwinVector tvR;
				tvR.m_v1	= tv1.m_v2;
				tvR.m_v2	= tv2.m_v2;
				tvR.m_v1.z	= -1;
				tvR.m_v2.z	= dR;
				xyCen	= rb.GetXyCircleCenter(tvR.m_v1, tvR.m_v2, fabs(tvR.m_v2.z), tvR.m_v1.z==-1);
				tvR.m_xyRCenter = xyCen;
				tvR.m_sInfo.Format("%d번째내측헌치우측면", nIdxInWall+1);
				long nIdx	= m_tvArrVertSection.GetIdxByInfo(sInfo1)+1;
				if(nIdx > 0)
				{
					// (ARCBRIDGE-3465) InsertAt()을 해주면 같은 이름의 선이 계속 추가 되기 때문에 오류가 발생한다. 같은 이름이 있다면 일단 제거하고 추가. 
					long nFind = m_tvArrVertSection.GetIdxByInfo(tvR.m_sInfo);
					if(nFind > -1)
						m_tvArrVertSection.RemoveAt(nFind, 1);

					m_tvArrVertSection.InsertAt(nIdx, tvR);
				}
				pWall->m_tvArrFront.SetTvByInfo(sInfo2, tv2);

				tvUpperWall.m_v2	= tv2.m_v2;
				pWall->m_tvArrFront.SetTvByInfo("벽체상면", tvUpperWall);
			}

			// 벽체 상면과 헌치아래면좌우측을 맞춰줌
			CString sInfoL	= _T("");
			CString sInfoR	= _T("");
			sInfoL.Format("%d번째내측헌치아래면좌측", nIdxInWall+1);
			sInfoR.Format("%d번째내측헌치아래면우측", nIdxInWall+1);
			CTwinVector tvLeft	= m_tvArrVertSection.GetTvByInfo(sInfoL);
			CTwinVector tvRight	= m_tvArrVertSection.GetTvByInfo(sInfoR);

			tvRight.m_v1	= tvUpperWall.m_v2;
			tvRight.m_v2	= tvUpperWall.GetXyMid();
			tvLeft.m_v1		= tvRight.m_v2;
			tvLeft.m_v2		= tvUpperWall.m_v1;
			m_tvArrVertSection.SetTvByInfo(sInfoL, tvLeft);
			m_tvArrVertSection.SetTvByInfo(sInfoR, tvRight);
		}
		else
		{
			CTwinVector tv1, tv2;
			CString sInfo1(""), sInfo2("");
			double dR	= bLeft ? m_dHLR[nIdxInWall][iUPPER] : m_dHRR[nIdxInWall][iUPPER];
			CDPoint xyMatchOrg(0, 0), xyCen(0, 0);
			long nIdx	= 0;
			if(bLeft)
			{
				sInfo1.Format("%d번째내측헌치좌측면", nIdxInWall+1);
				
				if(nIdxInWall == 0)
					sInfo2	= "슬래브하면";
				else
					sInfo2.Format("%d번째내측헌치우측슬래브하면", nIdxInWall);
			}
			else
			{
				sInfo1.Format("%d번째내측헌치우측슬래브하면", nIdxInWall+1);
				sInfo2.Format("%d번째내측헌치우측면", nIdxInWall+1);
			}
			tv1	= m_tvArrVertSection.GetTvByInfo(sInfo1);
			tv2	= m_tvArrVertSection.GetTvByInfo(sInfo2);
			tv1.m_v1.z	= 0;
			tv1.m_v2.z	= 0;
			tv2.m_v1.z	= 0;
			tv2.m_v2.z	= 0;
			
			if(GetXyMatchLineAndLine(tv1.m_v1, tv1.GetXyDir(), tv2.m_v1, tv2.GetXyDir(), xyMatchOrg))
			{
				GetXyTanOfArc(xyMatchOrg, tv1.GetXyDir(TRUE), tv2.GetXyDir(), dR, xyCen, tv1.m_v2, tv2.m_v1);
			}

			m_tvArrVertSection.SetTvByInfo(sInfo1, tv1);
			m_tvArrVertSection.SetTvByInfo(sInfo2, tv2);
			CTwinVector tvR;
			tvR.m_v1	= tv1.m_v2;
			tvR.m_v2	= tv2.m_v1;
			tvR.m_v1.z	= -1;
			tvR.m_v2.z	= dR;
			xyCen	= rb.GetXyCircleCenter(tvR.m_v1, tvR.m_v2, fabs(tvR.m_v2.z), tvR.m_v1.z==-1);
			tvR.m_xyRCenter = xyCen;
			tvR.m_sInfo.Format("%d번째내측헌치좌측면R", nIdxInWall+1);
			nIdx	= m_tvArrVertSection.GetIdxByInfo(sInfo1)+1;
			if(nIdx > -1)
			{
				// (ARCBRIDGE-3465) InsertAt()을 해주면 같은 이름의 선이 계속 추가 되기 때문에 오류가 발생한다. 같은 이름이 있다면 일단 제거하고 추가. 
				long nFind = m_tvArrVertSection.GetIdxByInfo(tvR.m_sInfo);
				if(nFind > -1)
					m_tvArrVertSection.RemoveAt(nFind, 1);
				
				m_tvArrVertSection.InsertAt(nIdx, tvR);
			}
		}
	}
}

double CRcBridgeApp::GetAngleJongdan(double dSta)
{
	CLineInfo *pLine	= GetLineBase();
	return ToDegree(atan(pLine->GetJongDanGiul(dSta)));	
}
long CRcBridgeApp::GetIndexShellNearByJointNumber(CShell *pShell, long nIdxJoint1, long nIdxJoint2)
{
	CFEMManage *pFEM = &m_FemShell;

	long nSizeShell  = pFEM->GetShellSize();
	for(long nShell=0; nShell<nSizeShell; nShell++)
	{
		CShell *pShell2 = pFEM->GetShell((USHORT)nShell);

		if(pShell==pShell2) continue;

		if(pShell2->m_nAttrib==SHELL_ELEMENT_UPPERSLAB)
		{
			if(nIdxJoint1==pShell2->m_nJ[0] && nIdxJoint2==pShell2->m_nJ[3]) return nShell;
			if(nIdxJoint1==pShell2->m_nJ[1] && nIdxJoint2==pShell2->m_nJ[2]) return nShell;
		}
		else
		{
			if(nIdxJoint1==pShell2->m_nJ[3] && nIdxJoint2==pShell2->m_nJ[2]) return nShell;
			if(nIdxJoint1==pShell2->m_nJ[0] && nIdxJoint2==pShell2->m_nJ[1]) return nShell;
		}
	}

	return -1;
}

// 헌치 하면 각도
CDPoint CRcBridgeApp::GetVAngHunchLower(long nIdxHunch, long nLeft, BOOL bVertDir)
{
	long nIdxInWall	= nIdxHunch;
	CWallApp *pWall	= GetInWall(nIdxInWall);
	
	CDPoint vAngHunchLower	= CDPoint(1, 0);
	if(!Compare(pWall->GetAngleWallByHor(!bVertDir), 90.0, "="))
	{
		CDPoint xyHunchBottomL(0, 0), xyHunchBottomR(0, 0);
		GetXyHunchLowerSide(nIdxHunch, nLeft, bVertDir, xyHunchBottomL, xyHunchBottomR);

		vAngHunchLower	= (xyHunchBottomL - xyHunchBottomR).Unit();
	}

	return vAngHunchLower;
}

void CRcBridgeApp::GetXyHunchLowerSide(long nIdxHunch, long nLeft, BOOL bVertDir, CDPoint &xyHunchLowerL, CDPoint &xyHunchLowerR, BOOL bUpper)
{
	CDPoint vAng	= GetAngleJijum(nIdxHunch+1, bVertDir);
	// (38563) bVertDir = !bApplyAngle 로 사용하고 있는데 bVert가 TRUE인경우 Angle이 적용되지 않아야 전체 단면과 맞다
	// 이걸 어째야 하나... 일단 Angle 사용하지 않는걸로..
	if(bVertDir)
	{
		vAng = CDPoint(0, 1);
	}

	long nIdxInWall	= nIdxHunch;
	CWallApp *pWall	= GetInWall(nIdxInWall);
	CDPoint vAngWall	= ToDPointFrDegree(pWall->GetAngleWallByHor(!bVertDir));

	CDPoint xyHunchBottomC	= GetXyInWallHunchBottom(nIdxInWall, TRUE, nLeft, bVertDir, bUpper);

	if(bUpper)
	{
		xyHunchLowerL	= GetXyMatchLineAndSlabUpper(CDPoint(xyHunchBottomC.x-m_dWL[nIdxHunch][iUPPER]/vAng.y, xyHunchBottomC.y), vAngWall, nLeft, m_dTS, bVertDir);
		xyHunchLowerR	= GetXyMatchLineAndSlabUpper(CDPoint(xyHunchBottomC.x+m_dWR[nIdxHunch][iUPPER]/vAng.y, xyHunchBottomC.y), vAngWall, nLeft, m_dTS, bVertDir);
		xyHunchLowerL	= xyHunchLowerL + vAngWall * -m_dHHL[nIdxHunch][iUPPER];
		xyHunchLowerR	= xyHunchLowerR + vAngWall * -m_dHHR[nIdxHunch][iUPPER];
	}
	else
	{
		xyHunchLowerL	= GetXyMatchLineAndLowerSlab(CDPoint(xyHunchBottomC.x-m_dWL[nIdxHunch][iLOWER]/vAng.y, xyHunchBottomC.y), vAngWall, nLeft, m_dTS_Lower, bVertDir);
		xyHunchLowerR	= GetXyMatchLineAndLowerSlab(CDPoint(xyHunchBottomC.x+m_dWR[nIdxHunch][iLOWER]/vAng.y, xyHunchBottomC.y), vAngWall, nLeft, m_dTS_Lower, bVertDir);
		xyHunchLowerL	= xyHunchLowerL + vAngWall * m_dHHL[nIdxHunch][iLOWER];
		xyHunchLowerR	= xyHunchLowerR + vAngWall * m_dHHR[nIdxHunch][iLOWER];
	}
}

void CRcBridgeApp::SyncWall_HunchLength(long nJijum, CWallApp *pWall, BOOL bApplyAngle)
{
	CDPoint vAng	= ToDPointFrDegree(pWall->GetAngleWallByHor(bApplyAngle));
	
	//CDPoint xyOrg	= pWall->m_xyOrg;
	//CDPoint xyOrgL	= CDPoint(pWall->m_xyOrg.x - pWall->m_dW/vAng.y/2, pWall->m_xyOrg.y);
	//CDPoint xyOrgR	= CDPoint(pWall->m_xyOrg.x + pWall->m_dW/vAng.y/2, pWall->m_xyOrg.y);

	//double dSta		= GetXyMatchLineAndSlabUpper(xyOrg, vAng, 0, 0, !bApplyAngle).x;
	//double dStaL	= GetXyMatchLineAndSlabUpper(xyOrgL, vAng, 0, 0, !bApplyAngle).x;
	//double dStaR	= GetXyMatchLineAndSlabUpper(xyOrgR, vAng, 0, 0, !bApplyAngle).x;

	/*
	pWall->m_dThickHunch	= GetHeightUpperSlab_OldOrNotUse(nJdSta, vAng) - m_dTS;
	pWall->m_dThickHunchL	= GetHeightUpperSlab_OldOrNotUse(dStaL, vAng) - m_dTS;
	pWall->m_dThickHunchR	= GetHeightUpperSlab_OldOrNotUse(dStaR, vAng) - m_dTS;
	*/
	pWall->m_dThickHunch	= GetHeightUpperSlab(nJijum, 0, vAng) - m_dTS;
	pWall->m_dThickHunchL	= GetHeightUpperSlab(nJijum, -1, vAng) - m_dTS;
	pWall->m_dThickHunchR	= GetHeightUpperSlab(nJijum, 1, vAng) - m_dTS;

}

void CRcBridgeApp::SyncWall_DesignData(CWallApp *pWall)
{
	if(!pWall) return;

	m_pARcBridgeDataStd->m_pBasicConcInfo->SetLenghtRebarMax(m_pARcBridgeDataStd->m_dLengthRebarMax);
	pWall->m_pBasicConcInfo			= m_pARcBridgeDataStd->m_pBasicConcInfo;
	pWall->m_dCoverUpper_UpperSlab	= m_dCoverUpper_UpperSlab[0];
	pWall->m_Fck					= GetValueFck(ePartInWall);
	pWall->m_Fy						= GetValueFy(ePartInWall, FALSE);
	pWall->m_FyShear				= GetValueFy(ePartInWall, TRUE);
	pWall->m_dLengthRebarMax		= m_pARcBridgeDataStd->m_dLengthRebarMax;
	pWall->m_nIdxCountCycleMainRebar	= m_nIdxCountCycleMainRebar;
	long dan = 0; for(dan = 0; dan < MAX_REBAR_DAN; dan++)
	{
		pWall->m_dCoverLeft[dan]	= m_dCoverWallMid[dan];	// 벽체 왼쪽 피복
		pWall->m_dCoverRight[dan]	= m_dCoverWallMid[dan];	// 벽체 오른쪽 피복
	}

	pWall->m_Column.m_pBasicConcInfo	= m_pARcBridgeDataStd->m_pBasicConcInfo;
	pWall->m_footing.m_pBasicConcInfo	= m_pARcBridgeDataStd->m_pBasicConcInfo;
}

void CRcBridgeApp::SyncWall_JongdanLimitFront(CWallApp *pWall, BOOL bApplyAngle)
{
	if(!pWall) return;

	pWall->m_tvArrUpperLimit_Front.RemoveAll();

	CTwinVector tv;
	tv.m_v1	= GetXySectionSttEnd(TRUE, 0, !bApplyAngle);
	tv.m_v2	= GetXySectionSttEnd(FALSE, 0, !bApplyAngle);
	pWall->m_tvArrUpperLimit_Front.Add(tv);

	if(m_nTypeBridge == BRIDGE_TYPE_ARCH)
	{
		pWall->m_tvArrLowerLimit_Front.RemoveAll();
		pWall->m_tvArrLowerLimit_FrontRebar.RemoveAll();

		pWall->m_tvArrLowerLimit_Front	= m_tvArrVertSectionRib[iUPPER];
		pWall->m_tvArrLowerLimit_FrontRebar	= m_tvArrVertSectionRib[iLOWER];
	}
}

void CRcBridgeApp::SyncWall_ColumnPlanJewon(CWallApp *pWall)
{
	if(!pWall) return;

	if(pWall->m_bTopIsColumn)
	{
		if(pWall->m_Column.m_nType == COL_TYPE_CIRCLE)
		{
			pWall->m_Column.m_dB[iOUTTER][iUPPER]	= pWall->m_dW;
			pWall->m_Column.m_dR[iOUTTER][iUPPER]	= pWall->m_dW/2;
			pWall->m_Column.m_dH[iOUTTER][iUPPER]	= pWall->m_dW;
		}
		else if(pWall->m_Column.m_nType == COL_TYPE_RECTANGLE)
		{
			pWall->m_Column.m_dB[iOUTTER][iUPPER]	= pWall->m_dW;
			pWall->m_Column.m_dR[iOUTTER][iUPPER]	= 0;
		}
		else if(pWall->m_Column.m_nType == COL_TYPE_TRACK)
		{
			// 트랙형은 B값을 자동 지정해 준다.
			double dW	= pWall->GetWidthWall();
			pWall->m_Column.m_dB[iOUTTER][iUPPER]	= dW;

			// 가로 값 체크
			if(pWall->m_Column.m_dB[iOUTTER][iUPPER] < pWall->m_Column.m_dR[iOUTTER][iUPPER] * 2)
			{
				pWall->m_Column.m_dR[iOUTTER][iUPPER] = pWall->m_Column.m_dB[iOUTTER][iUPPER] / 2;
			}

			// 세로 값 체크
			if(pWall->m_Column.m_dH[iOUTTER][iUPPER] < pWall->m_Column.m_dR[iOUTTER][iUPPER] * 2)
			{
				pWall->m_Column.m_dR[iOUTTER][iUPPER] = pWall->m_Column.m_dH[iOUTTER][iUPPER] / 2;
			}

		}
		else if(pWall->m_Column.m_nType == COL_TYPE_POLY)
		{
			pWall->m_Column.m_dR[iOUTTER][iUPPER]	= pWall->m_dW/2;
		}
	}
}

void CRcBridgeApp::SyncBridge_CrossSection_SttEndFooting(BOOL bApplyAngle)
{
	CWallApp wall;
	CTwinVectorArray tvArrSide;
	CTwinVector tv;

	MakeWallByOutWallOnlyShape(wall, TRUE, bApplyAngle);	// 벽체는 무조건 사거리로 만듬
	wall.GetTvLeftSide(tvArrSide, TRUE, FALSE, FALSE, TRUE, 0);
	tvArrSide.GetTvByInfo("벽체하면", tv);
	m_footingStt.m_tvPathLeft	= tv;
	m_footingBox.m_tvPathLeft = tv;
	m_footingBox.m_exFooting.m_tvPathLeft = tv;

	wall.GetTvLeftSide(tvArrSide, TRUE, FALSE, FALSE, FALSE, 0);
	tvArrSide.GetTvByInfo("벽체하면", tv);
	m_footingStt.m_tvPathRight	= tv;
	m_footingBox.m_tvPathRight = tv;
	m_footingBox.m_exFooting.m_tvPathRight = tv;

	// 종점 기초
	MakeWallByOutWallOnlyShape(wall, FALSE, bApplyAngle);
	wall.GetTvLeftSide(tvArrSide, TRUE, FALSE, FALSE, TRUE, 0);
	tvArrSide.GetTvByInfo("벽체하면", tv);
	m_footingEnd.m_tvPathLeft	= tv;

	wall.GetTvLeftSide(tvArrSide, TRUE, FALSE, FALSE, FALSE, 0);
	tvArrSide.GetTvByInfo("벽체하면", tv);
	m_footingEnd.m_tvPathRight	= tv;
}

// 종단타입을 복구하는데 문제가 있으니 복구를 안해도 되는 시점에서 자주 초기화 해주자.
void CRcBridgeApp::ResetFlagForRecoverVertSection()
{
	m_bFirstCalledGetTvVertSection_Std	= FALSE;
	m_bSecondCalledGetTvVertSection_Std	= FALSE;

	m_nTypeJongdanForRebarOldOfOld	= m_nTypeJongdanForRebar;
	m_nTypeJongdanForRebarOld	= m_nTypeJongdanForRebar;
	m_nTypeJongdanParaOld	= m_nTypeJongdanForRebar;
}

// 전체적으로 종단면 좌표만 구하는 통합 함수
// syncBridge 대신 속도개선용으로 사용함.
// BOOL bForBMOfFooting : 기초를 수량 산출용으로 구함(기초 측면을 기초중심 기준으로 구함)
void CRcBridgeApp::GetTvVertSection_Std(long nLeft, BOOL bApplyAngle, BOOL bWingFront, BOOL bRecovery, long nTypeJongdan, BOOL bForBMOfFooting, BOOL bNoneRecover)
{
	// GetTvVertSection_Std가 중복호출되는것을 방지.
	if(bRecovery && bNoneRecover == FALSE)
	{
		// 중복호출후의 복구인 경우 최초호출상태로 되돌림
		if(m_bSecondCalledGetTvVertSection_Std)
		{
			m_bFirstCalledGetTvVertSection_Std	= TRUE;
			m_bSecondCalledGetTvVertSection_Std	= FALSE;

			m_nTypeJongdanForRebarOld = m_nTypeJongdanForRebarOldOfOld;
			m_tvArrVertSectionOld	= m_tvArrVertSectionOldOfOld;
			m_nLeftCurVertSectionOld	= m_nLeftCurVertSectionOldOfOld;

			for(long wall = 0; wall < GetCountInWall(); wall++)
			{
				CWallApp *pWall	= GetInWall(wall);
				if(pWall)
				{
					pWall->m_xyOrg	= m_xyArrOrdOldOfOld.GetSize() > wall ? m_xyArrOrdOldOfOld.GetAt(wall) : CDPoint(0, 0);
					pWall->m_Column.m_dEL	= m_dArrElOldOfOld.GetSize() > wall ? m_dArrElOldOfOld.GetAt(wall) : 0;
				}
			}

			GetTvVertSection_Std(m_nLeftParaOld, m_bApplyAngleParaOld, m_bWingFrontParaOld, FALSE, m_nTypeJongdanParaOld, m_bForBMOfFootingParaOld, TRUE);
			return;
		}

		m_bFirstCalledGetTvVertSection_Std	= FALSE;
		m_bSecondCalledGetTvVertSection_Std	= FALSE;
	}
	else if(bNoneRecover == FALSE)
	{
		// 최초호출
		if(!m_bFirstCalledGetTvVertSection_Std && !m_bSecondCalledGetTvVertSection_Std)
		{
			m_bFirstCalledGetTvVertSection_Std	= TRUE;
			m_bSecondCalledGetTvVertSection_Std	= FALSE;

			// 이미 호출되어 있는 상태라면 복구시키고 진행
			// 최초 호출된 상태를 저장해서 그쪽으로 복구한다.
			// 몇번이 중복되서 호출되던 복구는 최초 호출된 상태로 복구되도록
			m_nLeftParaOld	= nLeft;
			m_bApplyAngleParaOld	= bApplyAngle;
			m_bWingFrontParaOld	= bWingFront;
			m_nTypeJongdanParaOld	= nTypeJongdan;
			m_bForBMOfFootingParaOld	= bForBMOfFooting;

			m_nTypeJongdanForRebarOldOfOld	= m_nTypeJongdanForRebar;
			m_tvArrVertSectionOldOfOld	= m_tvArrVertSection;
			m_nLeftCurVertSectionOldOfOld	= m_nLeftCurVertSection;
			m_xyArrOrdOldOfOld.RemoveAll();
			m_dArrElOldOfOld.RemoveAll();
			for(long wall = 0; wall < GetCountInWall(); wall++)
			{
				CWallApp *pWall	= GetInWall(wall);
				if(pWall)
				{
					m_xyArrOrdOldOfOld.Add(pWall->m_xyOrgOld);
					m_dArrElOldOfOld.Add(pWall->m_Column.m_dELOld);
				}
			}
		}
		// 중복호출
		else
		{
			m_bFirstCalledGetTvVertSection_Std	= TRUE;
			m_bSecondCalledGetTvVertSection_Std	= TRUE;
		}
	}

	if(!bRecovery)
	{
		// nTypeJongdan 이 -1 일이라도 Old에 현재 상태는 넣어 줘야 한다. 복구를 하려고 할테니까
		if(!bNoneRecover)
		{
			m_nTypeJongdanForRebarOld	= m_nTypeJongdanForRebar;
			m_tvArrVertSectionOld		= m_tvArrVertSection;
		}

		// -1일때는 여기서 현재 저장된 것을 바꿔줄 필요가 없는것이지.
		if(nTypeJongdan != -1)
		{
			m_nTypeJongdanForRebar	= nTypeJongdan;
			m_nLeftCurVertSectionOld	= m_nLeftCurVertSection;

			for(long wall = 0; wall < GetCountInWall(); wall++)
			{
				CWallApp *pWall	= GetInWall(wall);
				if(pWall)
				{
					pWall->m_xyOrgOld	= pWall->m_xyOrg;
					pWall->m_Column.m_dELOld	= pWall->m_Column.m_dEL;
				}
			}
		}
	}

	if(bRecovery)
	{
		m_nTypeJongdanForRebar		= m_nTypeJongdanForRebarOld;
		m_nLeftCurVertSection		= m_nLeftCurVertSectionOld;
		m_tvArrVertSection			= m_tvArrVertSectionOld;
		m_footingStt.m_dELDiff	= 0;//GetDiffELSttAndJijumForVertSection(0, TRUE);
		m_footingEnd.m_dELDiff	= GetDiffELSttAndJijumForVertSection(m_nQtyJigan, FALSE);

		for(long wall = 0; wall < GetCountInWall(); wall++)
		{
			CWallApp *pWall	= GetInWall(wall);
			if(pWall)
			{
				pWall->m_xyOrg	= pWall->m_xyOrgOld;
				pWall->m_Column.m_dEL	= pWall->m_Column.m_dELOld;
			}
		}
	}
	else
	{
		// 싱크를 꼭 필요한 거만 해주자.. 속도개선!!! /////
		GetTvVertSection(nLeft, bApplyAngle, bWingFront);
		if(m_nTypeBridge == BRIDGE_TYPE_ARCH)
			m_tvArrVertSectionRib[iUPPER]	= m_tvArrVertSection_NonModify_ForArch;
	}
	
	GetTvVertSectionFxJoint(TRUE, !bApplyAngle);
	GetTvVertSectionFxJoint(FALSE, !bApplyAngle);
	BOOL bLeftWing = nLeft==-1;
	double dAngStt = GetAngleWingWallBetweenWall(TRUE, !bLeftWing).GetAngleDegree();
	double dAngEnd = GetAngleWingWallBetweenWall(FALSE, bLeftWing).GetAngleDegree();
	if(Compare(dAngStt, 0.0, "=")) dAngStt	= 90;
	if(Compare(dAngEnd, 0.0, "=")) dAngEnd	= 90;

	
	SyncBridge_CrossSection_SttEndFooting(bApplyAngle);
	double dAngSttForLine = ToDegree(asin(min(1.0, m_dWS / GetThickOutWallByLine(TRUE))));
	double dAngEndForLine = ToDegree(asin(min(1.0, m_dWE / GetThickOutWallByLine(FALSE))));
	if(GetAngleJijum(0).GetAngleDegree() > 90)
	{
		dAngSttForLine = 180 - dAngSttForLine;
	}
	if(GetAngleJijum(m_nQtyJigan).GetAngleDegree() > 90)
	{
		dAngEndForLine = 180 - dAngEndForLine;
	}

	// (ARCBRIDGE-3442) 벽체 위치가 곡선 선형인경우 종단에 표현되는 벽체 두께와 기초에서의 벽체 두께가 같지 않아 벽체와 기초의 연결부가 틀어지고
	// 치수 위치도 조금씩 어긋난다. 지점 각도를 조금 조정해서 벽체 두께를 맞춰 준다.
	m_footingStt.SyncFooting(bApplyAngle || bWingFront, bWingFront ? dAngStt : dAngSttForLine, FALSE, bForBMOfFooting);
	m_footingEnd.SyncFooting(bApplyAngle || bWingFront, bWingFront ? dAngEnd : dAngEndForLine, FALSE, bForBMOfFooting);
	TrimRibByFootingUpper();
	GetTvRibBlock();

	long i = 0; for(i = 0; i < GetCountInWall(); i++)
	{
		CWallApp *pWall = GetInWall(i);
		if(pWall)
		{
			SyncWall(i+1, pWall, bApplyAngle);
		}
	}
	TrimRibByConnectUpperSlab();
	if(m_nTypeBridge == BRIDGE_TYPE_ARCH)
	{
		m_tvArrVertSection.AddFromTvArray(m_tvArrVertSectionRib[iUPPER]);
		m_tvArrVertSection.AddFromTvArray(m_tvArrVertSectionRib[iLOWER]);
	}

	if(IsOutWall(TRUE))
	{
		m_footingStt.GetTvFront(bApplyAngle);
		m_footingStt.m_exFooting.GetTvFront(bApplyAngle);
	}
	if(IsOutWall(FALSE))
	{
		m_footingEnd.GetTvFront(bApplyAngle);
		m_footingEnd.m_exFooting.GetTvFront(bApplyAngle);
	}

	// 선형 좌우측 기준 교량 그릴때는 내측 헌치를 만들지 않으므로..
	// 아치교일 때는 적용되면 안 됨.(종단도  경사형태만 지원되므로 통과할 필요 없음)
	if(nLeft == 0 && m_nTypeBridge != BRIDGE_TYPE_ARCH)
	{
		long nCountWall	= GetCountInWall();
		for(long wall = 0; wall < nCountWall; wall++)
		{
			CWallApp *pWall	= GetInWall(wall);
			if(!pWall) continue;
			if(!pWall->m_bIs) continue;

			//31720
			CTwinVectorArray tvArrVertSection;
			tvArrVertSection.AddFromTvArray(m_tvArrVertSection);

			SyncWall_JongdanLimitFront(pWall, bApplyAngle);
			pWall->m_dDiffTop[0]	= GetDiffELSttAndJijumForVertSection(wall+1, TRUE);
			if(IsBoxType())
			{
				pWall->m_dELBDiff	= 0;
				// Box 타입은 좌측 우측도 따로 구해서 저장하자.
				pWall->m_dDiffTop[0]	= GetDiffELSttAndJijumForVertSection(wall+1, TRUE, FALSE, TRUE);
				pWall->m_dDiffTop[1]	= GetDiffELSttAndJijumForVertSection(wall+1, FALSE, FALSE, TRUE);
			}
			else
			{
				pWall->m_dELBDiff	= pWall->m_dDiffTop[0];
				pWall->m_dDiffTop[1]	= pWall->m_dDiffTop[0];
			}
			pWall->GetTvFront(tvArrVertSection, bApplyAngle);
			

			// 내측헌치 R에 대해서 구해줌(접선일 경우에만 구해주면 됨)
			ApplyRoundOfInWallHunch(wall, TRUE);
			ApplyRoundOfInWallHunch(wall, FALSE);

			if(pWall->m_footing.m_bIs)
			{
				pWall->m_footing.GetTvFront(bApplyAngle);
				pWall->m_footing.m_exFooting.GetTvFront(bApplyAngle);
			}

			// 내측 벽체 좌표를 구했으면 리브와 내측벽체의 관계를 규명하자!!!
			/*
			if(m_nTypeBridge == BRIDGE_TYPE_ARCH)
			{
				CRebarPlacing rb;
				GetTvRibBlock();
				m_tvArrVertSection.AddFromTvArray(m_tvArrVertSectionRib[iUPPER]);
				m_tvArrVertSection.AddFromTvArray(m_tvArrVertSectionRib[iLOWER]);
			}
			*/

			pWall->CheckOverLapLineBetweenSubObject();
		}
	}

	// 겹치는 선들에 대해서 체크함
	CheckOverLapLineBetweenSubObject();		
}

void CRcBridgeApp::SyncBridge_DesignData()
{
	m_footingStt.m_Fck	= GetValueFck(ePartFootStt);
	m_footingEnd.m_Fck	= GetValueFck(ePartFootEnd);;
	m_footingBox.m_Fck	= GetValueFck(ePartLowerSlab);
	m_footingStt.m_Fy	= GetValueFy(ePartFootStt, FALSE);
	m_footingStt.m_FyShear	= GetValueFy(ePartFootStt, TRUE);
	m_footingEnd.m_Fy	= GetValueFy(ePartFootEnd, FALSE);
	m_footingEnd.m_FyShear	= GetValueFy(ePartFootEnd, TRUE);
	m_footingBox.m_Fy	= GetValueFy(ePartLowerSlab, FALSE);
	m_footingBox.m_FyShear	= GetValueFy(ePartLowerSlab, TRUE);
	m_footingStt.m_pBasicConcInfo	= m_footingEnd.m_pBasicConcInfo	= m_footingBox.m_pBasicConcInfo	= m_pARcBridgeDataStd->m_pBasicConcInfo;

	long i = 0; for(i = 0; i < GetCountInWall(); i++)
	{
		CWallApp *pWall = GetInWall(i);
		if(pWall)
		{
			SyncWall_DesignData(pWall);
			pWall->SyncWall_DesignData();

			pWall->m_footing.m_Fck	= GetValueFck(ePartInFoot);
			pWall->m_footing.m_Fy	= GetValueFy(ePartInFoot, FALSE);
			pWall->m_footing.m_FyShear	= GetValueFy(ePartInFoot, TRUE);
		}
	}

	if(m_pARcBridgeDataStd->IsLsdDesign())
	{
		for(long ixStt=0; ixStt<2; ++ixStt)
		{
			BOOL bStt = (ixStt == 0);
			for(long ixL=0; ixL<2; ++ixL)
			{
				CApsApp *pAps = bStt ? &m_apsStt[ixL] : &m_apsEnd[ixL];

				pAps->m_dFck = GetValueFck(bStt? ePartApsStt : ePartApsEnd);
				pAps->m_dFy = GetValueFy(bStt? ePartApsStt : ePartApsEnd);
			}
		}

		BOOL bKds2019 = m_pARcBridgeDataStd->IsDesignKDS2019() || m_pARcBridgeDataStd->IsRailDesignKDS2018();
		m_plsdManager->SetDesignCondition(bKds2019 ? LSD_DESIGN_CONDITION_KDS_2019 : LSD_DESIGN_CONDITION_LSD_2015);
	}
}

// 헌치 높이 구함.
// sta로 구하는 함수가 있지만 시간이 매우 오래 걸림.
// 깔끔하게 지점을 구하는 이 함수를 사용하면 매우 빠른 동작으로 헌치높이를구할 수 있다.
double CRcBridgeApp::GetHeightUpperSlab(long nJ, long nLeft, CDPoint vAng)
{
	double dH	= 0;
	CLineInfo *pLine	= GetLineBase(FALSE);
	double dTP	= GetThickPave(FALSE);
	
	if(nJ == 0 || nJ == m_nQtyJigan)
	{
		// 시종점일땐는 어짜피 수직으로만 구하므로 각도고려 하지 않고 구한다.
		dH	= m_dTS + GetHeightHunchByJijum(nJ);
	}
	else
	{
		CWallApp *pWall	= GetInWall(nJ-1);
		if(pWall->m_bIs)
		{
			CTwinVector tv;
			CDPoint xyOrg(0, 0);
			if(nLeft == -1 || nLeft == 1)
			{
				tv	= pWall->m_tvArrFront.GetTvByInfo(nLeft == -1 ? "벽체좌측면" : "벽체우측면");
				xyOrg	= tv.GetXyTop();
			}
			else
			{
				tv	= pWall->m_tvArrFront.GetTvByInfo("벽체상면");
				xyOrg	= tv.GetXyMid();
			}
			
			CDPoint xyMatch(0, 0);
			if(m_nTypeJongdanForRebar == 2)
				pLine->GetXyMatchByJongDanXyAndAngle(xyOrg, vAng, xyMatch, -dTP);
			else
				xyMatch = GetXyMatchLineAndSlabUpper(xyOrg, vAng, 0, 0, FALSE);

			dH	= ~(xyMatch - xyOrg);
		}
		else
		{
			CDPoint xyOrg	= GetXyInWallHunchBottom(nJ-1, nLeft==-1, 0, FALSE);
			if(nLeft == 0)
				xyOrg.x	= GetStationOnJijum(nJ);

			CDPoint xyMatch(0, 0);
			pLine->GetXyMatchByJongDanXyAndAngle(xyOrg, vAng, xyMatch, -dTP);

			dH	= ~(xyMatch - xyOrg);
		}
	}

	return dH;
}

double CRcBridgeApp::GetHeightLowerSlab(long nJ, long nLeft, CDPoint vAng)
{
	double dH	= 0;
	if(nJ == 0 || nJ == m_nQtyJigan || !m_bSlopeLowerSlab)
	{
		// 시종점일땐는 어짜피 수직으로만 구하므로 각도고려 하지 않고 구한다.
		// 하부 슬래브에 종단경사가 적용되지 않았을때도
		dH	= m_dTS_Lower + GetHeightHunchByJijum(nJ, FALSE);
	}
	else
	{
		CWallApp *pWall	= GetInWall(nJ-1);
		if(pWall->m_bIs)
		{
			CTwinVector tv;
			CDPoint xyOrg(0, 0);
			if(nLeft == -1 || nLeft == 1)
			{
				tv	= pWall->m_tvArrFront.GetTvByInfo(nLeft == -1 ? "벽체좌측면" : "벽체우측면");
				xyOrg	= tv.GetXyBottom();
			}
			else
			{
				tv	= pWall->m_tvArrFront.GetTvByInfo("벽체하면좌측");
				xyOrg	= tv.m_v1;
			}

			CDPoint xyMatch(0, 0);
			xyMatch = GetXyMatchLineAndLowerSlab(xyOrg, vAng, 0, 0, FALSE);

			dH	= ~(xyOrg - xyMatch);
		}
		else
		{
			CDPoint xyOrg	= GetXyInWallHunchBottom(nJ-1, nLeft==-1, 0, FALSE, FALSE);
			if(nLeft == 0)
				xyOrg.x	= GetStationOnJijum(nJ);

			CDPoint xyMatch(0, 0);
			xyMatch = GetXyMatchLineAndLowerSlab(xyOrg, vAng, 0, 0, FALSE);

			dH	= ~(xyOrg - xyMatch);
		}
	}

	return dH;
}

// 중심 sta에 해당하는 좌우측 선형의 xy좌표 구함.
CDPoint CRcBridgeApp::GetXyMatchLineLRByCenterStation(double dSta, long nLeft)
{
	CLineInfo *pLineCR	= GetLineBase();
	if(nLeft == 0) return pLineCR->GetXyLine(dSta);
	
	CDPoint vAng		= GetAngleByStation(dSta);
	CDPoint xyPlane		= GetXySlabEdgeAct(dSta, vAng, nLeft==-1);
	CDPoint xyPlaneR		= GetXySlabEdgeAct(dSta, vAng, FALSE);

	return xyPlane;
}


// 중심 sta에 해당하는 좌우측 끝 단의 sta 구하기.(매우 중요한 함수임)
double CRcBridgeApp::GetStationByCenterStation(double dSta, long nLeft)
{
	if(nLeft == 0) return dSta;

	CLineInfo *pLine	= GetLineLeftCenRight(nLeft);
	CDPoint xyPlaneLR	= GetXyMatchLineLRByCenterStation(dSta, nLeft);
	double dStaLR		= dSta;
	pLine->GetStationMatchVerticalByXy(xyPlaneLR, dStaLR);

	return dStaLR;
}

// sta1과 sta2사이의 실제 거리를 리턴함.
// sta1, 2는 해당 선형에 대한 sta
double CRcBridgeApp::GetLengthStaToSta(double dSta1, double dSta2, long nLeft)
{
	double sw	= 1;
	if(dSta2 < dSta1)
	{
		GetSwap(dSta1, dSta2);
		sw	= -1;
	}

	if(nLeft == 0) return (dSta2 - dSta1) * sw;

	CLineInfo *pLine	= GetLineLeftCenRight(nLeft);
	double dWidth		= nLeft == -1 ? m_dWidthSlabLeft : m_dWidthSlabRight;

	double dLen	= pLine->GetLengthByDis(dSta1, dSta2, dWidth) * sw;

	return dLen;
}

/**
	@brief
		슬래브의 실길이
*/
double CRcBridgeApp::GetLengthSlabReal(long nLeft	/**< -1:슬래브의 좌측, 0:슬래브의중심(도로중심이아님), 1:우측(슬래브의우측)*/
									   )
{
	if(nLeft != 0)
	{
		CTwinVectorArray tvArr;
		GetTvSidePlaneOfSlab(tvArr, nLeft == -1, FALSE);

		return tvArr.GetLength();
	}
	else 
	{
		double dLenLeft	= GetLengthSlabReal(-1);
		double dLenRight	= GetLengthSlabReal(1);

		return (dLenLeft + dLenRight)/2;
	}

	return 0;
}

// sta1에서 sta2까지 중심선형에 해당하는 지점 각도를 좌우측 선형에 맞게 리턴
double CRcBridgeApp::GetAngleJijumByStaToSta(double dSta1, double dSta2, double dDistOrg, long nLeft)
{
	if(dSta1 > dSta2)
	{
		GetSwap(dSta1, dSta2);
		dDistOrg *= -1;
	}
	
	double dDist	= GetLengthStaToSta(dSta1, dSta2, nLeft);
	if(dDist == 0 || dDistOrg == 0) return 90;

	double dAng	= 90;
	if(dDistOrg / dDist > 1 || dDistOrg / dDist < -1)
	{
		double ret	= asin(min(1.0, dDist / dDistOrg));
		dAng		= ToDegree(ret);
	}
	else
	{
		double ret	= asin(min(1.0, dDistOrg / dDist));
		dAng		= ToDegree(ret);
	}
	
	return dAng;
}

// dSta는 현재 중심선형을 기준으로 구해진 sta인데
// 좌우측 선형을 기준으로 그릴경우, 중심선형의 어느 점에 대응하는 sta인지 찾아 준다.
// 미완성 함수
// double CRcBridgeApp::GetStationCenterByCenterStationForLR(double dSta, long nLeft)
// {
// 	if(nLeft == 0) return dSta;
// 
// 	return 0;
// }

// 사거리로 그릴때만 사용하는 함수
// 중심선형 sta에서 dOffset만큼 offset한 곳의 슬래브 상면 좌표 리턴
CDPoint CRcBridgeApp::GetXySlabUpperByOffsetFrStation(double dSta, double dOffset, long nLeft)
{
	CLineInfo *pLine	= GetLineBase();
	double dStaRet		= pLine->GetStationByOffsetWidth(dSta, dOffset, GetAngleByStation(dSta));

	double dDistFrStt	= dStaRet - m_dStationBridgeStt;
	CDPoint xyRet		= GetXySlabUpperByLenFrSttStation(dDistFrStt, nLeft);
	
	return xyRet;
}

// 날개벽 방향으로 좌우측 구분됨.
BOOL CRcBridgeApp::IsGagakForWing(BOOL bStt, BOOL bLeft)
{
//	BOOL bIsGagak = FALSE;
	long nLeft = (bStt == bLeft)? 1 : 0;

	double dExpW = (bStt)? m_dExpSttW[nLeft] : m_dExpEndW[nLeft];
	double dExpH = (bStt)? m_dExpSttH[nLeft] : m_dExpEndH[nLeft];
	
	BOOL bIsGagak = (dExpW > 0 && dExpH > 0 && m_bIsExp) ? TRUE : FALSE;

	return bIsGagak;
}

CDPoint CRcBridgeApp::GetDefaultAngleWingWall(BOOL bStt, BOOL bLeft)
{
	CWingWall *pWing = GetWingWall(bStt, bLeft);
	if(pWing == NULL) CDPoint(1, 0);

	CDPoint vAng(1, 0);
	CDPoint vAngJ = GetAngleJijum((bStt)? 0 : m_nQtyJigan);
	CDPoint vAngRL = (bLeft)? vAngJ : ToDPointFrDegree(180 - vAngJ.GetAngleDegree());
	BOOL bSide = (pWing->m_nAttachPos==0)? FALSE : TRUE;

	if(bSide)
	{
		vAng = vAngRL;
	}
	else
	{
		if(IsGagakForWing(bStt, bLeft))
		{
			long nLeft = (bStt == bLeft)? 1 : 0;
			
			double dExpW = (bStt)? m_dExpSttW[nLeft] : m_dExpEndW[nLeft];
			double dExpH = (bStt)? m_dExpSttH[nLeft] : m_dExpEndH[nLeft];
			double dWtot = (dExpH / tan(vAngRL.GetAngleRadian())) + dExpW;

			double dAngB = Compare(dWtot, 0.0, _T("=")) ? 0 : atan(dExpH / dWtot);
			vAng = ToDPointFrRadian(dAngB);
		}
		else
		{
			vAng = CDPoint(1, 0);
		}
	}
	
	// 날개벽에 대한 종단면을 다시 구하도록
	m_bSettingVertSection = FALSE;

	return vAng;
}

//부상방지판 상면위에 날개벽이 있는지 구하는 함수.
BOOL CRcBridgeApp::IsWingWallOnPRF(BOOL bStt, BOOL bLeft)
{
	CWingWall *pWing = GetWingWall(bStt, bLeft);
	if(pWing == NULL) return FALSE;
	if(m_nTypeBridge != BRIDGE_TYPE_BOX) return FALSE;

	CDPoint vAng	= GetAngleWingWallBetweenWall(bStt, bLeft);
	double dW = bStt ? m_dWS/vAng.y : m_dWE/vAng.y;

	if(pWing->m_dL2 <= m_dPRF_W + dW)
		return TRUE;

	return FALSE;
}

void CRcBridgeApp::SetDataDefaultPRF()
{
	m_dPRF_W = 1000;
	m_dPRF_H1 = m_dTS_Lower;
	m_dPRF_H2 = 0;
}
// 아치교의 경우 rib에 대해서 권고안 해줌
void CRcBridgeApp::SetDataDefaultShapeRib()
{
	if(m_nTypeBridge == BRIDGE_TYPE_ARCH)
	{
		double dDiffUpperSlabAndArchRib	= 100;	// 아치리브와 슬래브 떨어진 거리
		CLineInfo *pLine	= GetLineBase();
		double dStaMid	= m_dStationBridgeStt + m_dLengthBridge/2;
		double dTP		= GetThickPave();
		double dTS		= m_dTS;
		double dEL		= pLine->GetElevationPlanEl(dStaMid) - dTP - dTS - dDiffUpperSlabAndArchRib;
		
		m_pbRib[iUPPER].m_xyCrown		= CDPoint(dStaMid, dEL);
		m_pbRib[iUPPER].m_dDistTotal	= m_dLengthBridge;
		m_pbRib[iUPPER].m_dHeightTotal	= m_pbRib[iUPPER].m_xyCrown.y - min(m_dELSB, m_dELEB);
		m_pbRib[iUPPER].m_dDistStt		= -m_pbRib[iUPPER].m_dDistTotal/2;
		m_pbRib[iUPPER].m_dDistEnd		= m_pbRib[iUPPER].m_dDistTotal/2;
		m_pbRib[iUPPER].m_dFactorM		= 2;
		m_pbRib[iUPPER].m_dLenArchLine	= 500;

		// 크라운부 두께 (교량전장/50)
		m_pbRib[iLOWER]					= m_pbRib[iUPPER];
		m_pbRib[iLOWER].m_dDistTotal	-= 2000;
		m_pbRib[iLOWER].m_xyCrown		= m_pbRib[iLOWER].m_xyCrown + CDPoint(0, -m_dLengthBridge/50);
		m_pbRib[iLOWER].m_dFactorM		= 2;
		m_pbRib[iLOWER].m_dLenArchLine	= 500;

		// 크라운부 두께 (교량전장/100)
		m_pbRibCen						= m_pbRib[iUPPER];
		m_pbRibCen.m_dDistTotal			-= 1000;
		m_pbRibCen.m_xyCrown			= m_pbRibCen.m_xyCrown + CDPoint(0, -m_dLengthBridge/100);
		m_pbRibCen.m_dFactorM			= 2;
		m_pbRibCen.m_dLenArchLine		= 500;

	}
	else
	{
		long upper = 0; for(upper = 0; upper < 2; upper++)
		{
			m_pbRib[upper].Init();
		}
		m_pbRibCen.Init();
	}
}

// 리브를 시종점 기초상단에 맞춰서 자르기
// 반드시 기초싱크 끝난뒤 해야 됨.	
void CRcBridgeApp::TrimRibByFootingUpper()
{
	// 기초를 구하면 기초와 리브와 교차점 찾아버림(기초 상면만 검사)
	if(m_nTypeBridge != BRIDGE_TYPE_ARCH) return;

	CRebarPlacing rb;
	CDPoint xyMatch(0, 0);
	CDPointArray xyArrMatch;
	CTwinVectorArray tvArrUpper;
	long stt = 0; for(stt = 0; stt < 2; stt++)
	{
		BOOL bStt	= stt == iSTT;
		CFootingApp *pFooting	= bStt ? &m_footingStt : &m_footingEnd;
		pFooting->GetTvOffset_Front(-1, TRUE, tvArrUpper, FALSE);
			
		CTwinVector tv;
		long upper = 0; for(upper = 0; upper < 3; upper++)
		{
			CTwinVectorArray *pTvArr	= NULL;
			CParabola *pPb	= NULL;
			switch(upper)
			{
				case iUPPER: 
					pTvArr = &m_tvArrVertSectionRib[iUPPER]; 
					pPb	= &m_pbRib[iUPPER];
					break;
				case iLOWER: 
					pTvArr = &m_tvArrVertSectionRib[iLOWER]; 
					pPb	= &m_pbRib[iLOWER];
					break;
				case 2: 
					pTvArr = &m_tvArrVertSectionRibCen; 
					pPb	= &m_pbRibCen;
					break;
			}
			if(pTvArr->GetSize() == 0) continue;

			long i = 0; for(i = 0; i < pTvArr->GetSize(); i++)
			{
				tv	= pTvArr->GetAt(i);
				rb.GetXyMatchTvArrAndTv(tvArrUpper, tv, xyArrMatch, TRUE);
				if(xyArrMatch.GetSize() > 0) break;
			}

			if(xyArrMatch.GetSize() > 0)
			{
				xyMatch	= xyArrMatch.GetAt(0);
				if(bStt)
				{
					tv.m_v1	= xyMatch;
					pTvArr->SetAt(i, tv);
					long j = 0; for(j = 0; j < i; j++)
						pTvArr->RemoveAt(0);
				}
				else
				{
					tv.m_v2	= xyMatch;
					pTvArr->SetAt(i, tv);
					long nSize	= pTvArr->GetSize();
					long j = 0; for(j = i+1; j < nSize; j++)
						pTvArr->RemoveAt(i+1);
				}
			}
			else
			{
				if(bStt)
				{
					CTwinVector tvSlope	= m_footingStt.m_tvArrFront.GetTvByInfo("기초우측경사");
					CDPoint xyMatch	= pPb->GetXyMatchXyAndAngle(tvSlope.m_v1, tvSlope.GetXyDir());
					CDPoint xyStt	= pTvArr->GetAt(0).m_v1;
					if(pPb->GetDistByXPos(xyMatch.x) < pPb->GetDistByXPos(xyStt.x))
					{
						double dDist	= pPb->GetDistByXPos(xyMatch.x);
//						pPb->m_dDistStt	= dDist;
						pTvArr->RemoveAll();
						AddFromParabola(*pTvArr, (*pPb), dDist, pPb->m_dDistEnd);
					}
				}
				else
				{
					
					CTwinVector tvSlope	= m_footingEnd.m_tvArrFront.GetTvByInfo("기초좌측경사");
					CDPoint xyMatch	= pPb->GetXyMatchXyAndAngle(tvSlope.m_v1, tvSlope.GetXyDir());
					CDPoint xyEnd	= pTvArr->GetAt(pTvArr->GetSize()-1).m_v2;
					if(pPb->GetDistByXPos(xyMatch.x) > pPb->GetDistByXPos(xyEnd.x))
					{
						double dDist	= pPb->GetDistByXPos(xyMatch.x);
//						pPb->m_dDistEnd	= dDist;
						pTvArr->RemoveAll();
						AddFromParabola(*pTvArr, (*pPb), dDist, pPb->m_dDistEnd);
					}
				}

				if(upper == iUPPER || upper == iLOWER)
				{
					long i = 0; for(i = 0; i < pTvArr->GetSize(); i++)
					{
						tv	= pTvArr->GetAt(i);
						if(tv.m_sInfo.Find(_T("리브블럭")) != -1) continue;
						tv.m_sInfo	= upper==iUPPER ? "상부리브" : "하부리브";
						pTvArr->SetAt(i, tv);
					}	
				}
			}
		}
	}
}

// 리브 블럭 생성
void CRcBridgeApp::GetTvRibBlock()
{
	CTwinVector tvUpper, tvLeft, tvRight;
	CDPointArray xyArrMatch;
	CDPoint xyMid(0, 0);
	CRebarPlacing rb;
	CTwinVectorArray tvArrVertSectionRib;
	tvArrVertSectionRib	= m_tvArrVertSectionRib[iUPPER];

	// 리브 블럭이 생성되어있는 상태라면 또하지 않는다. 그위에 블럭높이를 더해 버린다.
	CTwinVectorArray tvArrFind;
	tvArrVertSectionRib.GetTvArrByInfo(_T("리브블럭"), tvArrFind, FALSE, FALSE);
	if(tvArrFind.GetSize() > 0) return;

	long i = 0; for(i = 0; i < m_nQtyJigan-1; i++)
	{
		double dW	= m_dRibBlockW[i];
		double dH	= m_dRibBlockH[i];

		if(Compare(dW, 0.0, "=") && Compare(dH, 0.0, "=")) continue;
		long nJ	= i+1;
		double dSta	= GetStationOnJijum(nJ);

		rb.GetXyMatchTvArrAndLine(tvArrVertSectionRib, CDPoint(dSta, 0), CDPoint(0, 1), xyArrMatch);
		if(xyArrMatch.GetSize() > 0)
		{
			xyMid	= xyArrMatch.GetAt(0);
			tvUpper.m_v1	= CVector(xyMid.x-dW/2, xyMid.y+dH, 0);
			tvUpper.m_v2	= tvUpper.m_v1 + CDPoint(dW, 0);
			tvUpper.m_sInfo.Format("리브블럭%d상면", i+1);
			
			rb.GetXyMatchTvArrAndLine(tvArrVertSectionRib, tvUpper.m_v1, CDPoint(0, -1), xyArrMatch);
			if(xyArrMatch.GetSize() > 0)
			{
				xyMid	= xyArrMatch.GetAt(0);
				tvLeft.m_v1	= tvUpper.m_v1;
				tvLeft.m_v2	= xyMid;
				tvLeft.m_sInfo.Format("리브블럭%d좌측면", i+1);
			}
			else continue;

			rb.GetXyMatchTvArrAndLine(tvArrVertSectionRib, tvUpper.m_v2, CDPoint(0, -1), xyArrMatch);
			if(xyArrMatch.GetSize() > 0)
			{
				xyMid	= xyArrMatch.GetAt(0);
				tvRight.m_v1	= tvUpper.m_v2;
				tvRight.m_v2	= xyMid;
				tvRight.m_sInfo.Format("리브블럭%d우측면", i+1);
			}
			else continue;

			// union함수를 써도 되지만, 그렇게 속도문제가 있어서 수작업함(수작업이 더 쉬울 수도 있음)
			// 상, 좌, 우 다 구했으면 리브에 블럭만큼 구멍 뚫기
			rb.GetXyMatchTvArrAndTv(tvArrVertSectionRib, tvUpper, xyArrMatch, TRUE);
			if(xyArrMatch.GetSize() > 0)
			{
				xyMid	= xyArrMatch.GetAt(0);
				if(tvLeft.m_v2.y > tvLeft.m_v1.y) tvUpper.m_v1	= xyMid;
				else tvUpper.m_v2	= xyMid;
			}
			
			rb.GetTvArrInRange(m_tvArrVertSectionRib[iUPPER], tvUpper, TRUE);


			// 리브 구멍 뚫은뒤 좌우측중 필요한거만 넣는다.
			if(tvLeft.m_v2.y < tvLeft.m_v1.y)
				m_tvArrVertSectionRib[iUPPER].Add(tvLeft);

			if(tvUpper.GetLength() > 0)
				m_tvArrVertSectionRib[iUPPER].Add(tvUpper);

			if(tvRight.m_v2.y < tvRight.m_v1.y)
				m_tvArrVertSectionRib[iUPPER].Add(tvRight);
		}
	}

	m_tvArrVertSectionRib[iUPPER].Sort();
}
// 종거더 입력시 사용하는 스테이션
double CRcBridgeApp::GetStationForInputJongGirder(BOOL bStt, BOOL bLeft, BOOL bGagak)
{
	double dSta	= m_dStationBridgeStt;
	if(bGagak)
		dSta	= GetStationByTarget(bStt ? BRIDGE_OFF_STT_STT : BRIDGE_OFF_END_END, 0, TRUE);
	else
		dSta	= GetStationByTarget(BRIDGE_OFF_STT_STT, 0, TRUE);

	return dSta;
}

// 
BOOL CRcBridgeApp::GetTvGirderLowerJong(CTwinVectorArray &tvArrLower, BOOL bStt, BOOL bLeft, BOOL bGagak)
{
	tvArrLower.RemoveAll();

	double dSta	= GetStationForInputJongGirder(bStt, bLeft, bGagak);
	CDPoint vAng	= CDPoint(0, 1);
	GetTvCrossSection(tvArrLower, dSta, vAng, FALSE, FALSE, FALSE, 0, FALSE, FALSE, FALSE, bGagak);
	if(tvArrLower.GetSize() == 0) return FALSE;

	CRebarPlacing rb;
	rb.MoveRebar(tvArrLower, CDPoint(0, -m_dTS));

	return TRUE;
}

void CRcBridgeApp::GetTvPlaneOfExp(CTwinVectorArray &tvArr, BOOL bStt, BOOL bLeft, long nDan)
{
	tvArr.RemoveAll();
	if(!IsGagakForLine(bStt, bLeft)) return;

	BOOL bCutEnd = (bStt)? m_bExpCutStt[bLeft? 0:1] : m_bExpCutEnd[bLeft? 0:1];
	CTwinVectorArray tvArrPlane;
	GetTvRebarOut_Plane_UpperSlabStd(tvArrPlane, nDan, TRUE,2,TRUE, bCutEnd);

	CTwinVector tv;
	CString str	= _T("");
	str.Format("%s%s가각부", bStt ? "시점" : "종점", bLeft ? "좌측" : "우측");
	long i = 0; for(i = 0; i < tvArrPlane.GetSize(); i++)
	{
		tv	= tvArrPlane.GetAt(i);
		if(tv.m_sInfo.Find(str, 0) != -1)
			tvArr.Add(tv);
	}
}

// 평면 좌표 셋팅
void CRcBridgeApp::SettingPlane()
{
	m_bSettingPlane	= FALSE;

	CTwinVectorArray tvArrPlane;
	GetTvRebarOut_Plane_UpperSlab(tvArrPlane, -1, TRUE);
	GetTvRebarOut_Plane_UpperSlab(tvArrPlane, -1, FALSE);
	
	m_bSettingPlane	= TRUE;
	m_bNeedSettingPlane	= FALSE;
}

//BOOL bForExpRebar
void CRcBridgeApp::GetTvRebarOut_Plane_UpperSlabStd(CTwinVectorArray &tvArr, long nDan, BOOL bExp, long nLeft, BOOL bVert, BOOL bCutEnd, BOOL bForExpRebar)
{
	tvArr.RemoveAll();
	if(nDan != -1)
	{
		GetTvRebarOut_Plane_UpperSlab(tvArr, nDan, bExp, nLeft, bVert, bCutEnd);
		return;
	}

	if(!m_bSettingPlane)
		SettingPlane();

	long nExp	= bExp ? iEXP : iNORMAL;
	CRebarPlacing rb;
	CTwinVectorArray tvArrStt;
	CTwinVectorArray tvArrEnd;
	CTwinVectorArray tvArrLeft;
	CTwinVectorArray tvArrRight;
	
	tvArrStt	= m_tvArrSttEndPlane[iSTT][nExp];
	tvArrEnd	= m_tvArrSttEndPlane[iEND][nExp];
	tvArrLeft	= m_tvArrLeftRightPlane[iLEFT][nExp];
	tvArrRight	= m_tvArrLeftRightPlane[iRIGHT][nExp];

	// 가각 끝단 자르기 옵션 적용
	if(bExp && bCutEnd)
	{
		long stt = 0; for(stt = 0; stt < 2; stt++)
		{
			BOOL bStt	= stt == iSTT;
			CString sStt	= bStt ? "시점" : "종점";
			CTwinVectorArray *pTvArr	= bStt ? &tvArrStt : &tvArrEnd;
			long left = 0; for(left = 0; left < 2; left++)
			{
				BOOL bLeft	= left == 0;
				BOOL bCut	= bStt ? m_bExpCutStt[left] : m_bExpCutEnd[left];
				
				if(!bCut) continue;

				CTwinVectorArray tvArr;
				tvArr	= bLeft ? tvArrLeft : tvArrRight;
				CTwinVector tvNew;
				long i = 0; for(i = 0; i < tvArr.GetSize(); i++)
				{
					tvNew	= tvArr.GetAt(i);
					if(tvNew.m_sInfo.Find("컷트", 0) != -1 && tvNew.m_sInfo.Find(sStt, 0) != -1)
					{	
						long j = 0; for(j = 0; j < pTvArr->GetSize(); j++)
						{
							CTwinVector tvStt	= pTvArr->GetAt(j);
							if(Compare(tvStt.GetLength(), 0.0, ">"))
							{
								GetXyMatchLineAndLine(tvStt.m_v1, tvStt.GetXyDir(), tvNew.m_v1, tvNew.GetXyDir(), bLeft ? tvStt.m_v1 : tvStt.m_v2);
								pTvArr->RemoveAll();
								pTvArr->Add(tvStt);
								break;
							}
						}
						break;
					}
				}
			}
		}
	}

	rb.ReverseRebar(tvArrStt);

	// 속도개선을 위해서 trim함수 사용하지 않음
	if(nLeft != 2)
	{
		CTwinVectorArray tvArrCenter;
		GetTvCenterPlane_UpperSlab(tvArrCenter, TRUE, FALSE, 0, bVert);
		CTwinVector tv	= tvArrCenter.GetAt(0);
		tv.m_sInfoHor.Format("cover:%f", 0);
		tvArrCenter.SetAt(0, tv);
		
		BOOL bLeft	= nLeft == iLEFT;
		tv.m_v1	= tvArrStt.GetAt(0).m_v1;
		tv.m_v2	= tvArrStt.GetAt(tvArrStt.GetSize()-1).m_v2;
		if(!bLeft)
			tv.m_v2	= tvArrCenter.GetAt(0).m_v1;
		else
			tv.m_v1	= tvArrCenter.GetAt(0).m_v1;

		tvArrStt.RemoveAll();
		tvArrStt.Add(tv);

		tv.m_v1	= tvArrEnd.GetAt(0).m_v1;
		tv.m_v2	= tvArrEnd.GetAt(tvArrEnd.GetSize()-1).m_v2;

		if(!bLeft)
			tv.m_v1	= tvArrCenter.GetAt(tvArrCenter.GetSize()-1).m_v2;
		else
			tv.m_v2	= tvArrCenter.GetAt(tvArrCenter.GetSize()-1).m_v2;

		tvArrEnd.RemoveAll();
		tvArrEnd.Add(tv);


		if(bLeft)
		{
			tvArrRight.RemoveAll();
			tvArrRight.AddFromTvArray(tvArrCenter);
		}
		else
		{
			tvArrLeft.RemoveAll();
			tvArrLeft.AddFromTvArray(tvArrCenter);
			rb.ReverseRebar(tvArrLeft);
		}
	}

	tvArr.AddFromTvArray(tvArrStt);
	tvArr.AddFromTvArray(tvArrRight);
	tvArr.AddFromTvArray(tvArrEnd);
	tvArr.AddFromTvArray(tvArrLeft);

}

// 내측벽체가 횡단면에서 들어가는 거리
// 즉.. 헌치부 거리
double CRcBridgeApp::GetDistCrossSectionAndInWall(long nIdxWall, BOOL bApplyAngle, BOOL bLeft)
{
	CWallApp *pWall	= GetInWall(nIdxWall);
	if(!pWall) return 0;

	long nJ	= nIdxWall + 1;
	CDPoint vAng = bApplyAngle ? GetAngleJijum(nJ) : CDPoint(0, 1); 
	double dLenL	= 0;
	double dLenR	= 0;
	if(nJ > 0 && nJ < m_nQtyJigan && !pWall->m_bTopIsColumn)
	{
		dLenL	= (m_dDH[0] + m_dCantilB[0])/vAng.y;
		dLenR	= (m_dDH[1] + m_dCantilB[1])/vAng.y;
	}

	return bLeft ? dLenL : dLenR;
}

// 이 교량이 완전하게 직선구간에 포함 되어 잇는지 판단한다.
BOOL CRcBridgeApp::IsIncludeOnlyLineInBridge()
{
	CLineInfo *pLineL	= GetLineLeftRight(TRUE);
	CLineInfo *pLineR	= GetLineLeftRight(FALSE);
	CLineInfo *pLine	= GetLineBase();

	double dStaMin	= min(GetStationByTarget(BRIDGE_OFF_STT_SLAB, -1, TRUE), GetStationByTarget(BRIDGE_OFF_STT_SLAB, 1, TRUE));
	double dStaMax	= max(GetStationByTarget(BRIDGE_OFF_END_SLAB, -1, TRUE), GetStationByTarget(BRIDGE_OFF_END_SLAB, 1, TRUE));

	BOOL bOnlyLine	= TRUE;
	if(pLineL->GetRadiusMin(dStaMin, dStaMax) > 0) 
	{
		bOnlyLine = FALSE;
		return bOnlyLine;
	}

	if(pLineR->GetRadiusMin(dStaMin, dStaMax) > 0) 
	{
		bOnlyLine = FALSE;
		return bOnlyLine;
	}

	if(pLine->GetRadiusMin(dStaMin, dStaMax) > 0) 
	{
		bOnlyLine = FALSE;
		return bOnlyLine;
	}

	return bOnlyLine;
}

double CRcBridgeApp::GetCalcLenRHunch(long nJijum, BOOL bSttPos, BOOL bInside)
{
	double dLenHun = 0;

	CRebarPlacing rb;

	CTwinVector tv;
	CTwinVectorArray *pTvArr = &m_tvArrVertSection;
	CDPoint vAngWall(0, 1);
		
	CString str	= _T("");

	if(nJijum == 0 || nJijum == m_nQtyJigan)
	{
		pTvArr->GetTvByInfo((nJijum==0)? "시점벽체헌치" : "종점벽체헌치", tv);
		if(tv.GetLength() < 100.0)
			pTvArr->GetTvByInfo((nJijum==0)? "시점벽체헌치R" : "종점벽체헌치R", tv);
	}
	else
	{
		CWallApp *pWall = GetInWall(nJijum-1);
		vAngWall = ToDPointFrDegree(pWall->m_dAngle);

		long nIdxInWall	= nJijum - 1;
		long nIdxHunch	= GetIdxHunchInWall(nIdxInWall, bSttPos);

		str.Format("%d번째내측헌치", nIdxHunch+1);
		if(bSttPos) pTvArr->GetTvByInfo(str+"좌측면", tv);
		else pTvArr->GetTvByInfo(str+"우측면", tv);
	}

	CDPoint xyCen	= rb.GetXyCircleCenter(tv.m_v1, tv.m_v2, tv.m_v2.z, tv.m_v1.z == -1);
	CDPoint xyMatch(0, 0), xyHun1(0, 0), xyHun2(0, 0), xyCross(0, 0);

	double dAngStt	= (tv.m_v1 - xyCen).GetAngleDegree();
	double dAngEnd	= (tv.m_v2 - xyCen).GetAngleDegree();
	double dAngMid	= (dAngEnd+dAngStt)/2;

	CDPointArray xyArrMatch;
	rb.GetXyMatchLineAndArc2(xyCen, ToDPointFrDegree(dAngMid), tv.m_v1, tv.m_v2, tv.m_v2.z, tv.m_v1.z==-1, TRUE, xyArrMatch, TRUE);
	if(xyArrMatch.GetSize() > 0)
	{
		xyMatch	= xyArrMatch.GetAt(0);
	}

	xyCross	= GetXyMatchLineAndSlabeLowerToDist(0, bSttPos? tv.m_v1 : tv.m_v2, vAngWall);
	xyHun1	= GetXyMatchLineAndSlabeLowerToDist(0, xyMatch, ToDPointFrDegree(dAngMid+90));
	GetXyMatchLineAndLine(bSttPos? tv.m_v1 : tv.m_v2, vAngWall, xyMatch, ToDPointFrDegree(dAngMid+90), xyHun2);

	if(bInside)
	{
// 		dLenHun = ~(xyCross - xyHun1);
		dLenHun = ~(xyCross - xyHun2);
	}
	else
	{
		dLenHun = ~(xyCross - tv.m_v1);
	}

	return dLenHun;
}

double CRcBridgeApp::GetAnglHunchWithSlabOrWall(long nJijum, BOOL bSttPos, BOOL bSlab, BOOL bUpper)
{
	if(nJijum==0 && bSttPos) return 0;
	if(nJijum==m_nQtyJigan && !bSttPos) return 0;
	if(IsRcSlab()) return 0;
	if(IsPiRahmen() && (nJijum==0 || nJijum==GetCountJijum()-1)) return 0;
	
	CDPoint vAng(0, 0);
	double dAng = 0;
	if(nJijum == 0 || nJijum == m_nQtyJigan)
	{
		dAng = 90;
		vAng = CDPoint(0, 1);
	}
	else
	{
		CWallApp *pWall = GetInWall(nJijum-1);
		dAng = pWall->m_dAngle;
		vAng = ToDPointFrDegree(dAng);
		if(!bSttPos) dAng = 180 - dAng;
	}
	
	double HunchThick = 0;
	double HunchWidth = 0;
	
	BOOL bRoundHunch = IsBoxType() ? FALSE : IsRoundHunch(nJijum, bSttPos, bUpper);
	long nUD = bUpper ? iUPPER : iLOWER;
	if(bRoundHunch)
	{
		HunchThick = GetCalcLenRHunch(nJijum, bSttPos) * vAng.y;
		HunchWidth = GetCalcLenRHunch(nJijum, bSttPos);
	}
	else
	{
		if(nJijum==0) HunchThick = (bUpper? m_dHHS : m_dWSHH)*vAng.y;
		else if(nJijum==GetCountJijum()-1) HunchThick = (bUpper? m_dHHE : m_dWEHH)*vAng.y;
		else HunchThick = (bSttPos)? m_dHHL[nJijum-1][nUD]*vAng.y:m_dHHR[nJijum-1][nUD]*vAng.y;
		
		HunchWidth = GetWidthHunch(nJijum,bSttPos, bUpper);
	}
	
	double dLenWidth = (dAng==90)? HunchWidth : (HunchWidth-(HunchThick/tan(ToRadian(dAng))));
	double dAngleSlab = ToDegree(atan(HunchThick/dLenWidth));
	double dAngleWall = 180 - dAng - dAngleSlab;
	
	return bSlab? dAngleSlab : dAngleWall;
}

// Area of Polygon
double CRcBridgeApp::GetPolygonArea(CDPointArray vArr, long nNum)
{
	long i=0;
	double dArea = 0;

	if(vArr.GetSize() < nNum)
		nNum = vArr.GetSize();

	CRebarPlacing rb;
	double dY	= rb.GetXyEdgePoint(vArr, DOWNRIGHT_AREA).y;
	double dX	= rb.GetXyEdgePoint(vArr, LEFTDOWN_AREA).x;
	rb.MoveRebar(vArr, CDPoint(-dX, -dY));
	
	for(i=0; i<nNum; i++)
	{
		long j = (i+1) % nNum;
		double dAreaX	= vArr[i].x * vArr[j].y;
		double dAreaY	= vArr[i].y * vArr[j].x;
		dArea += dAreaX;
		dArea -= dAreaY;
	}
	dArea /= 2.0;

	rb.MoveRebar(vArr, CDPoint(dX, dY));

	return(dArea<0 ? -dArea : dArea);
}

// Centroid of Polygon
CDPoint CRcBridgeApp::GetCentroid(CDPointArray vP, long nNum)
{
	long	i=0;
	double	dCx = 0, dCy = 0, dFactor = 0;
	double	dArea = GetPolygonArea(vP, nNum);
	CDPoint xyResult(0,0);

	CRebarPlacing rb;
	double dY	= rb.GetXyEdgePoint(vP, DOWNRIGHT_AREA).y;
	double dX	= rb.GetXyEdgePoint(vP, LEFTDOWN_AREA).x;
	rb.MoveRebar(vP, CDPoint(-dX, -dY));

	for(i=0; i<nNum; i++)
	{
		long j = (i + 1) % nNum;
		dFactor = ( (vP.GetAt(i).x * vP.GetAt(j).y) - (vP.GetAt(j).x * vP.GetAt(i).y) );
		dCx += (vP.GetAt(i).x + vP.GetAt(j).x) * dFactor;
		dCy += (vP.GetAt(i).y + vP.GetAt(j).y) * dFactor;
	}

	dArea *= 6.0;
	dFactor = 1 / dArea;
	dCx *= dFactor;
	dCy *= dFactor;
	xyResult.x = dCx;
	xyResult.y = dCy;

	xyResult.x += dX;
	xyResult.y += dY;
	rb.MoveRebar(vP, CDPoint(dX, dY));

	return xyResult;
}

// 리브와 상부슬래브가 연결된 지점 리턴
// 우선은 무조건 연결된거만 지원하므로 옵션에 관계없이 상단리브 최고점의 스테이션의 지점을 리턴하면 된다.
long CRcBridgeApp::GetJijumConnectRibAndSlab()
{
	if(m_nTypeBridge != BRIDGE_TYPE_ARCH) return -1;

	return GetJijumNumberByStation(m_pbRib[iUPPER].m_xyCrown.x);
}

long CRcBridgeApp::GetNumberElementArchRibSpanSttCheck(long nSpan, long nTypeDesign, BOOL bCrown)
{
	CFEMManage *pFEM = GetFemManage(nTypeDesign);
	if(pFEM == NULL) return -1;

	long nJijumConnect = GetJijumConnectRibAndSlab();
	long nElement = -1;
	double dDistChcek = 0;
	
	if(nSpan == 0)
	{
		CDPoint vAngStt = GetAngleJijum(0);
		
		CTwinVector tv = m_tvArrVertSectionRibCen.GetAt(0);
		dDistChcek = toM(tv.m_v1.x - (m_dStationBridgeStt-m_dWS/vAngStt.y/2));
	}
	else
	{
		long i=0; for(i=0; i<nSpan; i++)
		{
			dDistChcek += toM(GetLengthModelSpan(i));
		}
		
		if(nSpan == nJijumConnect && bCrown == FALSE)
		{
			double WT = GetThickWall(nJijumConnect)/2;
			dDistChcek += toM(WT);
		}
	}

	long nSizeElement = pFEM->GetElementSize();
	for(long nEle=0; nEle<nSizeElement; nEle++)
	{
		CElement *pEle = pFEM->GetElement((USHORT)nEle);
		if(pEle->m_nFlag != ELEMENT_ARCHRIB) continue;
		CJoint   *pJnt = pFEM->GetJoint(pEle->m_nJStt);
		if(Compare(pJnt->m_vPointOrigin.x,dDistChcek,"="))
		{
			nElement = nEle;
			break;
		}
	}

	return nElement;
}

long CRcBridgeApp::GetNumberElementArchRibSpanEndCheck(long nSpan, long nTypeDesign, BOOL bCrown)
{
	CFEMManage *pFEM = GetFemManage(nTypeDesign);
	if(pFEM == NULL) return -1;
	
	long nJijumConnect = GetJijumConnectRibAndSlab();
	long nElement = -1;
	double dDistChcek = 0;

	if(nSpan == m_nQtyJigan-1)
	{
		CDPoint vAngStt = GetAngleJijum(0);
		
		CTwinVector tv = m_tvArrVertSectionRibCen.GetAt(m_tvArrVertSectionRibCen.GetSize()-1);
		dDistChcek = toM(tv.m_v2.x - (m_dStationBridgeStt-m_dWS/vAngStt.y/2));
	}
	else
	{
		long i=0; for(i=0; i<=nSpan; i++)
		{
			dDistChcek += toM(GetLengthModelSpan(i));
		}
		if(nSpan == nJijumConnect-1 && bCrown == FALSE)
		{
			double WT = GetThickWall(nJijumConnect)/2;
			dDistChcek -= toM(WT);
		}
	}
	
	long nSizeElement = pFEM->GetElementSize();
	for(long nEle=0; nEle<nSizeElement; nEle++)
	{
		CElement *pEle = pFEM->GetElement((USHORT)nEle);
		if(pEle->m_nFlag != ELEMENT_ARCHRIB) continue;
		CJoint   *pJnt = pFEM->GetJoint(pEle->m_nJEnd);
		if(Compare(pJnt->m_vPointOrigin.x,dDistChcek,"="))
		{
			nElement = nEle;
			break;
		}
	}

	return nElement;
}

// 선형 방향으로 판단
BOOL CRcBridgeApp::IsGagakForLine(BOOL bStt, BOOL bLeft)
{
	BOOL bIsGagak = FALSE;
	long nLeft = bLeft ? 0 : 1;

	double dExpW = (bStt)? m_dExpSttW[nLeft] : m_dExpEndW[nLeft];
	double dExpH = (bStt)? m_dExpSttH[nLeft] : m_dExpEndH[nLeft];
	
	bIsGagak = (dExpW > 0 && dExpH > 0 && m_bIsExp) ? TRUE : FALSE;

	return bIsGagak;
}

// 아치리브 평면 일반도(크라운 기준)
void CRcBridgeApp::GetTvPlaneArchRib(CTwinVectorArray &tvArrPlane, BOOL bUpper, long nDrawLeft)
{
	tvArrPlane.RemoveAll();
	if(m_nTypeBridge != BRIDGE_TYPE_ARCH) return;
	CLineInfo *pLine	= GetLineBase();
	if(!pLine) return;

	long upper	= bUpper ? iUPPER : iLOWER;
	CTwinVector tv;
	CTwinVectorArray *pTvArrArchRib	= &m_tvArrVertSectionRib[upper];
	long nSize	= pTvArrArchRib->GetSize();
	if(nSize == 0) return;

	CParabola *pbRib	= &m_pbRib[upper];
	double xDisFir	= pbRib->GetDistByXPos(pTvArrArchRib->GetAt(0).m_v1.x);
	double xDisLast	= pbRib->GetDistByXPos(pTvArrArchRib->GetAt(nSize-1).m_v1.x);

	double dLenLeft		= pbRib->GetLenByDistToDist(xDisFir, 0);
	double dLenRight	= pbRib->GetLenByDistToDist(0, xDisLast);

	tv.m_v1.x	= -pLine->m_dStationBp + pbRib->m_xyCrown.x - dLenLeft;
	tv.m_v1.y	= GetWidthArchRib(-1);
	tv.m_v1.z	= 0;
	tv.m_v2.x	= -pLine->m_dStationBp + pbRib->m_xyCrown.x + dLenRight;
	tv.m_v2.y	= tv.m_v1.y;
	tv.m_v2.z	= 0;
	tv.m_sInfo	= "아치리브평면좌측";
	tvArrPlane.Add(tv);

	tv.m_v1		= tv.m_v2;
	tv.m_v2.y	-= GetWidthArchRib(0);
	tv.m_sInfo	= "아치리브평면종점";
	tvArrPlane.Add(tv);
	
	tv.m_v1		= tv.m_v2;
	tv.m_v2.x	-= (dLenLeft + dLenRight);
	tv.m_sInfo	= "아치리브평면우측";
	tvArrPlane.Add(tv);

	tv.m_v1		= tv.m_v2;
	tv.m_v2.y	+= GetWidthArchRib(0);
	tv.m_sInfo	= "아치리브평면시점";
	tvArrPlane.Add(tv);

	if(nDrawLeft == -1)
	{
		CTwinVector tvStt	= tvArrPlane.GetTvByInfo("아치리브평면시점");
		CTwinVector tvEnd	= tvArrPlane.GetTvByInfo("아치리브평면종점");
		CTwinVector tvRight	= tvArrPlane.GetTvByInfo("아치리브평면우측");
		tvRight.m_v1	= tvEnd.GetXyMid();
		tvRight.m_v2	= tvStt.GetXyMid();
		tvEnd.m_v2	= tvEnd.GetXyMid();
		tvStt.m_v1	= tvStt.GetXyMid();

		tvArrPlane.SetTvByInfo("아치리브평면시점", tvStt);
		tvArrPlane.SetTvByInfo("아치리브평면종점", tvEnd);
		tvArrPlane.SetTvByInfo("아치리브평면우측", tvRight);
	}
	else if(nDrawLeft == 1)
	{
		CTwinVector tvStt	= tvArrPlane.GetTvByInfo("아치리브평면시점");
		CTwinVector tvEnd	= tvArrPlane.GetTvByInfo("아치리브평면종점");
		CTwinVector tvLeft	= tvArrPlane.GetTvByInfo("아치리브평면좌측");
		tvLeft.m_v1	= tvStt.GetXyMid();
		tvLeft.m_v2	= tvEnd.GetXyMid();
		tvEnd.m_v1	= tvEnd.GetXyMid();
		tvStt.m_v2	= tvStt.GetXyMid();

		tvArrPlane.SetTvByInfo("아치리브평면시점", tvStt);
		tvArrPlane.SetTvByInfo("아치리브평면종점", tvEnd);
		tvArrPlane.SetTvByInfo("아치리브평면좌측", tvLeft);
	}


}

// 아치리브 폭
double CRcBridgeApp::GetWidthArchRib(long nLeft)
{
	double dW	= GetWidthSlabAct(m_pbRib[iUPPER].m_xyCrown.x, CDPoint(0, 1), nLeft, TRUE);
	
	if(nLeft == -1 || nLeft == 0)
	{
		if(m_bCantilSlab[0])
			dW -= m_dCantilB[0];
	}
	
	if(nLeft == 1 || nLeft == 0)
	{
		if(m_bCantilSlab[1])
			dW -= m_dCantilB[1];
	}

	return dW;
}

double CRcBridgeApp::GetCoverSideArchRib(long nDan)
{
	return m_dCoverRib[iUPPER][nDan];
}

double CRcBridgeApp::GetStationRibBySpan(long nSpan, long nPos, BOOL bCrown)
{
	double dSta = 0;
	
	CTwinVectorArray tvArrCenter;
	tvArrCenter = m_tvArrVertSectionRibCen;
	
	long nJijumConnect = GetJijumConnectRibAndSlab();
	CDPoint vAng = GetAngleJijum(0);
	CTwinVector tv = tvArrCenter.GetAt(0);
	double dStaBase = m_dStationBridgeStt - m_dWS/2/vAng.y;
	double dStaStt = tv.m_v1.x;
	tv = tvArrCenter.GetAt(tvArrCenter.GetSize()-1);
	double dStaEnd = tv.m_v2.x;
	
	double dStaSpanStt = 0;
	double dStaSpanEnd = 0;
	
	dStaSpanEnd = dStaBase;

	long nReal = 0;
	long n=0; for(n=0; n<nSpan+1; n++)
	{
		dStaSpanStt = dStaSpanEnd;
		//if(n==nJijumConnect) continue;
		dStaSpanEnd += GetLengthModelSpan(nReal);
		nReal++;
	}
	
	if(nSpan == nJijumConnect-1 && bCrown == FALSE)
	{
		long WT  = (long)(GetThickWall(nJijumConnect)/2);
		dStaSpanEnd -= WT;
	}
	else if(nSpan == nJijumConnect && bCrown == FALSE)
	{
		long WT  = (long)(GetThickWall(nJijumConnect)/2);
		dStaSpanStt += WT;
	}

	// 기존에는 지점연결부도 지간으로 고려하여 계산하고 있었다
	// 그때당시의 Station을 구하는 조건임
/*	else if(nSpan == nJijumConnect)
	{
		long WT  = GetThickWall(nJijumConnect)/2;
		dStaSpanStt -= WT;
		dStaSpanEnd += WT;

		if(nPos == CHECK_POS_CENTER)
		{
			dSta = m_pbRibCen.m_xyCrown.x;
			return dSta;
		}
	}*/
	
	if(nSpan == 0) dStaSpanStt = dStaStt;
	else if(nSpan == m_nQtyJigan-1) dStaSpanEnd = dStaEnd;
	
	switch(nPos)
	{
	case CHECK_POS_LEFT: dSta = dStaSpanStt; break;
	case CHECK_POS_CENTER: dSta = (dStaSpanStt+dStaSpanEnd)/2; break;
	case CHECK_POS_RIGHT: dSta = dStaSpanEnd; break;
	}
	
	return dSta;
}

// 아치리브 횡단면도 _ 상부리브 기준
// 횡단의 우측이 평면에서는 -1(좌측)이 됨
// BOOL bVerticalSection : 수직으로 자른 단면인지
void CRcBridgeApp::GetTvCrossSection_ArchRib(CTwinVectorArray &tvArr, double dXDisOfUpper, long nDrawLeft, BOOL bVerticalSection, double dHeightBlock)
{
	tvArr.RemoveAll();
	if(m_nTypeBridge != BRIDGE_TYPE_ARCH) return;

	double dThick		= GetThickArchRib(dXDisOfUpper, FALSE, bVerticalSection) + dHeightBlock;
	double dELMid		= m_pbRib[iUPPER].GetXyByDist(dXDisOfUpper).y + dHeightBlock - dThick/2;
	GetTvSection_ArchRib(tvArr, CDPoint(0, dELMid), dThick, nDrawLeft);
}

double CRcBridgeApp::GetThickArchRib(double dXDisOfUpper, BOOL bCenter, BOOL bVerticalSection)
{
	if(m_nTypeBridge != BRIDGE_TYPE_ARCH) return 0;
	double dThick = 0;

	if(bCenter)
	{
		CDPoint xyCen		= m_pbRibCen.GetXyByDist(dXDisOfUpper);
		CDPoint xyDirPer	= bVerticalSection ? CDPoint(0, -1) : m_pbRibCen.GetXyDirPerByDist(dXDisOfUpper, TRUE);
		CDPoint xyUpper		= m_pbRib[iUPPER].GetXyMatchXyAndAngle(xyCen, xyDirPer);
		CDPoint xyLower		= m_pbRib[iLOWER].GetXyMatchXyAndAngle(xyCen, xyDirPer);
		dThick		= ~(xyUpper - xyLower);
	}
	else
	{
		CDPoint xyUpper		= m_pbRib[iUPPER].GetXyByDist(dXDisOfUpper);
		CDPoint xyDirPer	= bVerticalSection ? CDPoint(0, -1) : m_pbRib[iUPPER].GetXyDirPerByDist(dXDisOfUpper, TRUE);
		CDPoint xyLower		= m_pbRib[iLOWER].GetXyMatchXyAndAngle(xyUpper, xyDirPer);
		dThick		= ~(xyUpper - xyLower);
	}

	return dThick;
}

// 상부슬래브와 연결된 리브 지점 트림
void CRcBridgeApp::TrimRibByConnectUpperSlab()
{
	long nJ	= GetJijumConnectRibAndSlab();
	double dXMatchLeft	= 0;
	double dXMatchRight	= 0;
	if(nJ > -1 && m_bConnectRibAndUpperSlab)
	{
		CWallApp *pWall	= GetInWall(nJ-1);
		if(!pWall) return;
		if(!pWall->m_bIs) return;

		CTwinVector tvLeft	= pWall->m_tvArrFront.GetTvByInfo("벽체좌측면");
		CTwinVector tvRight	= pWall->m_tvArrFront.GetTvByInfo("벽체우측면");
		dXMatchLeft		= tvLeft.GetXyLeft().x;
		dXMatchRight	= tvRight.GetXyRight().x;
	}
	else	// 이경우 상부슬래브와 직접 연결됨
	{
		CDPointArray xyArrMatch;
		long i = 0; for(i = 0; i < m_tvArrVertSectionRib[iUPPER].GetSize(); i++)
		{
			CTwinVector tv	= m_tvArrVertSectionRib[iUPPER].GetAt(i);
			CDPoint xyMatch	= GetXyMatchLineAndSlabUpper(tv.m_v1, tv.GetXyDir(), 0, m_dTS, FALSE);
			CDPoint xy1 = tv.m_v1;
			CDPoint xy2	= tv.m_v2;
			if(Compare(tv.GetLength(), ~(xy1-xyMatch)+~(xy2-xyMatch), ">="))
			{
				xyArrMatch.Add(xyMatch);
			}
		}

		if(xyArrMatch.GetSize() > 1)
		{
			xyArrMatch.Sort(TRUE);
			dXMatchLeft	= xyArrMatch.GetAt(0).x;
			dXMatchRight	= xyArrMatch.GetAt(1).x;
		}
	}

	if(dXMatchLeft != dXMatchRight)
	{
		CTwinVectorArray tvArrLeft, tvArrRight;
		tvArrLeft	= m_tvArrVertSectionRib[iUPPER];
		tvArrRight	= m_tvArrVertSectionRib[iUPPER];

		CDRect rect	= tvArrLeft.GetRect();
		
		CRebarPlacing rb;
		CDPoint xy1	= CDPoint(dXMatchLeft, rect.bottom);
		CDPoint xy2	= CDPoint(dXMatchLeft, rect.top);
		rb.TrimTvArrayByLine(tvArrLeft, xy1, xy2, FALSE, TRUE, FALSE);

		xy1	= CDPoint(dXMatchRight, rect.bottom);
		xy2	= CDPoint(dXMatchRight, rect.top);
		rb.TrimTvArrayByLine(tvArrRight, xy1, xy2, TRUE, TRUE, FALSE);

		m_tvArrVertSectionRib[iUPPER].RemoveAll();
		m_tvArrVertSectionRib[iUPPER].AddFromTvArray(tvArrLeft);
		m_tvArrVertSectionRib[iUPPER].AddFromTvArray(tvArrRight);

		// 상부슬래브 하면도 잘라내야 됨
		CString strLeft		= _T("");
		CString strRight	= _T("");
		if(nJ == 1)
			strLeft	= "슬래브하면";
		else
			strLeft.Format("%d번째내측헌치우측슬래브하면", nJ-1);
		strRight.Format("%d번째내측헌치우측슬래브하면", nJ);

		CTwinVector tvLeft	= m_tvArrVertSection.GetTvByInfo(strLeft);
		CTwinVector tvRight	= m_tvArrVertSection.GetTvByInfo(strRight);
		tvLeft.m_v1	= GetXyMatchLineAndSlabUpper(CDPoint(dXMatchLeft, 0), CDPoint(0, 1), 0, m_dTS, FALSE);
		tvRight.m_v2	= GetXyMatchLineAndSlabUpper(CDPoint(dXMatchRight, 0), CDPoint(0, 1), 0, m_dTS, FALSE);

		m_tvArrVertSection.SetTvByInfo(strLeft, tvLeft);
		m_tvArrVertSection.SetTvByInfo(strRight, tvRight);
	}
}

// 아치리브 횡단면도
void CRcBridgeApp::GetTvCrossSection_ArchRib(CTwinVectorArray &tvArr, CTwinVectorArray &tvArrPattern, BOOL bUpper, long nDrawLeft)
{
	tvArr.RemoveAll();
	tvArrPattern.RemoveAll();

	long upper	= bUpper ? iUPPER : iLOWER;
	CParabola *pPb	= &m_pbRib[upper];
	CTwinVectorArray *pTvArr	= &m_tvArrVertSectionRib[upper];

	double dWidthL	= GetWidthArchRib(-1);
	double dWidth	= GetWidthArchRib(0);
	double dMid		= GetMidCrossSection();
	double dHeight	= pPb->m_xyCrown.y - pTvArr->GetRect().bottom;

	if(nDrawLeft != 0)
		dWidth	= dWidth/2;

	CTwinVector tv;
	tv.m_v1	= CVector(dMid + (nDrawLeft == 1 ? 0 : -dMid - dWidthL), pPb->m_xyCrown.y, 0);
	tv.m_v2	= tv.m_v1 + CVector(dWidth, 0, 0);
	tv.m_sInfo	= "상면";
	tvArr.Add(tv);

	tv.m_v1	= tv.m_v2;
	tv.m_v2	= tv.m_v1 + CVector(0, -dHeight, 0);
	tv.m_sInfo	= "우측면";
	tvArr.Add(tv);

	tv.m_v1	= tv.m_v2;
	tv.m_v2 = tv.m_v1 + CVector(-dWidth, 0, 0);
	tv.m_sInfo	= "하면";
	tvArr.Add(tv);

	tv.m_v1	= tv.m_v2;
	tv.m_v2 = tv.m_v1 + CVector(0, dHeight, 0);
	tv.m_sInfo	= "좌측면";
	tvArr.Add(tv);

	// 패턴
	long nCountPattern	= 10;
	double dLen	= pPb->GetLenByDistToDist(pPb->GetDistByXPos(pTvArr->GetRect().left), 0);
	double dOffset	= dLen / nCountPattern;

	double dXStt	= pPb->GetDistByXPos(pTvArr->GetRect().left);
	tv	= tvArr.GetTvByInfo("하면");

	long i = 0; for(i = 0; i < nCountPattern; i++)
	{
		dXStt += dOffset;
		CDPoint xy	= pPb->GetXyByDist(dXStt);
		tv.m_v1.y	= xy.y;
		tv.m_v2.y	= xy.y;
		tv.m_sInfo	= "패턴";
		tvArrPattern.Add(tv);
	}
}

// 아치리브 단면도 그림
void CRcBridgeApp::GetTvSection_ArchRib(CTwinVectorArray &tvArr, CDPoint xyCen, double dThick, long nDrawLeft)
{
	tvArr.RemoveAll();

	double dWidth	= GetWidthArchRib(0);
	double dWidthL	= GetWidthArchRib(-1);
	double dWidthR	= GetWidthArchRib(1);

	CTwinVector tv;
	tv.m_v1	= CVector(nDrawLeft == -1 ? xyCen.x : xyCen.x-dWidthL, xyCen.y+dThick/2, 0);
	tv.m_v2	= CVector(nDrawLeft == 1 ? xyCen.x : xyCen.x-dWidthL + dWidth, xyCen.y+dThick/2, 0);
	tv.m_sInfo	= "아치리브횡단상면";
	tvArr.Add(tv);

	tv.m_v1	= tv.m_v2;
	tv.m_v2.y -= dThick;
	tv.m_sInfo	= "아치리브횡단우측면";
	tvArr.Add(tv);

	tv.m_v1	= tv.m_v2;
	tv.m_v2.x -= nDrawLeft == 0 ? dWidth : dWidthR;
	tv.m_sInfo	= "아치리브횡단하면";
	tvArr.Add(tv);

	tv.m_v1	= tv.m_v2;
	tv.m_v2.y += dThick;
	tv.m_sInfo	= "아치리브횡단좌측면";
	tvArr.Add(tv);
}

void CRcBridgeApp::GetTvPlane_Section_ArchRib(CTwinVectorArray &tvArr, BOOL bStt)
{
	tvArr.RemoveAll();
	if(m_nTypeBridge != BRIDGE_TYPE_ARCH) return;
	CFootingApp *pFooting	= bStt ? &m_footingStt : &m_footingEnd;
	
	if(pFooting->m_nType == FOOTING_TYPE_BASIC_RIGHTSLOPE)
	{
		CTwinVector tvSlope;
		if(!pFooting->m_tvArrFront.GetTvByInfo("기초우측경사", tvSlope))
		{
			if(!pFooting->m_tvArrFront.GetTvByInfo("기초상면우측", tvSlope))
				return;
		}
		double dDist	= tvSlope.GetXyLeft().x - m_tvArrVertSectionRib[iUPPER].GetAt(0).m_v1.x;
		double dThick	= m_tvArrVertSectionRib[iLOWER].GetAt(0).m_v1.x - m_tvArrVertSectionRib[iUPPER].GetAt(0).m_v1.x;

		CTwinVector tvPlaneSlope;
		if(!pFooting->m_tvArrplan.GetTvByInfo("경사평면", tvPlaneSlope))
		{
			if(!pFooting->m_tvArrplan.GetTvByInfo("기초평면전면", tvPlaneSlope))
				return;
		}
		CDPoint xyCen	= tvPlaneSlope.GetXyMid() + tvPlaneSlope.GetXyDir().RotateInv90() * (dDist-dThick/2);

		double dWidthL	= GetWidthArchRib(-1);
		double dWidthR	= GetWidthArchRib(1);
		xyCen	= xyCen + tvPlaneSlope.GetXyDir() * (dWidthL - dWidthR)/2;


		GetTvSection_ArchRib(tvArr, xyCen, dThick, 0);
		
		CRebarPlacing rb;
		rb.RotateTvArr(tvArr, xyCen, tvPlaneSlope.GetXyDir().GetAngleDegree());
	}
	else
	{
		long nSizeU	= m_tvArrVertSectionRib[iUPPER].GetSize();
		long nSizeL	= m_tvArrVertSectionRib[iLOWER].GetSize();

		CTwinVector tvSlope;
		if(!pFooting->m_tvArrFront.GetTvByInfo("기초좌측경사", tvSlope))
		{
			if(!pFooting->m_tvArrFront.GetTvByInfo("기초상면좌측", tvSlope))
				return;
		}
		double dDist	= tvSlope.GetXyRight().x - m_tvArrVertSectionRib[iUPPER].GetAt(nSizeU-1).m_v2.x;
		double dThick	= m_tvArrVertSectionRib[iLOWER].GetAt(nSizeL-1).m_v2.x - m_tvArrVertSectionRib[iUPPER].GetAt(nSizeU-1).m_v2.x;

		CTwinVector tvPlaneSlope;
		if(!pFooting->m_tvArrplan.GetTvByInfo("경사평면", tvPlaneSlope))
		{
			if(!pFooting->m_tvArrplan.GetTvByInfo("기초평면후면", tvPlaneSlope))
				return;
		}
		CDPoint xyCen	= tvPlaneSlope.GetXyMid() + tvPlaneSlope.GetXyDir().RotateInv90() * (dDist-dThick/2);

		double dWidthL	= GetWidthArchRib(-1);
		double dWidthR	= GetWidthArchRib(1);
		xyCen	= xyCen + tvPlaneSlope.GetXyDir() * (dWidthL - dWidthR)/2;

		GetTvSection_ArchRib(tvArr, xyCen, dThick, 0);

		CRebarPlacing rb;
		rb.RotateTvArr(tvArr, xyCen, tvPlaneSlope.GetXyDir().GetAngleDegree());
	}
}

// 아치리브 정면도
void CRcBridgeApp::GetTvSection_Front_ArchRib(CTwinVectorArray &tvArr, BOOL bStt)
{
	tvArr.RemoveAll();
	if(m_nTypeBridge != BRIDGE_TYPE_ARCH) return;

	CFootingApp *pFooting	= bStt ? &m_footingStt : &m_footingEnd;

	CTwinVectorArray tvArrFront, tvArrFrontHidden;
	pFooting->GetTvLeftSide(tvArrFront, tvArrFrontHidden, FALSE, 0, bStt ? FALSE : TRUE);
	
	// 기초 하면에서 떨어진 거리
	CTwinVector tvLower	= pFooting->m_tvArrFront.GetTvByInfo("기초하면");
	CTwinVector tvLowerSide	= tvArrFront.GetTvByInfo("기초하면");
	long nSizeU	= m_tvArrVertSectionRib[iUPPER].GetSize();
	long nSizeL	= m_tvArrVertSectionRib[iLOWER].GetSize();
	double dDist	= 0;
	double dThick	= 0;
	CDPoint xyCen(0, 0);
	if(pFooting->m_nType == FOOTING_TYPE_BASIC_RIGHTSLOPE)
	{
		dDist	= m_tvArrVertSectionRib[iUPPER].GetAt(0).m_v1.y - tvLower.m_v1.y;
		dThick	= m_tvArrVertSectionRib[iUPPER].GetAt(0).m_v1.y - m_tvArrVertSectionRib[iLOWER].GetAt(0).m_v1.y;
		xyCen	= tvLowerSide.GetXyMid() + CDPoint(0, 1) * (dDist - dThick/2);
	}
	else if(pFooting->m_nType == FOOTING_TYPE_BASIC_LEFTSLOPE)
	{
		dDist	= m_tvArrVertSectionRib[iUPPER].GetAt(nSizeU-1).m_v2.y - tvLower.m_v1.y;
		dThick	= m_tvArrVertSectionRib[iUPPER].GetAt(nSizeU-1).m_v2.y - m_tvArrVertSectionRib[iLOWER].GetAt(nSizeL-1).m_v2.y;
		xyCen	= tvLowerSide.GetXyMid() + CDPoint(0, 1) * (dDist - dThick/2);
	}


	double dWidthL	= GetWidthArchRib(-1);
	double dWidthR	= GetWidthArchRib(1);
	xyCen	= xyCen + tvLowerSide.GetXyDir(TRUE) * (dWidthL - dWidthR)/2;

	GetTvSection_ArchRib(tvArr, xyCen, dThick, 0);
}

void CRcBridgeApp::SetDataInitPaveDetailWidth()
{
	if(m_bUnderGroundRahmen)
	{
		double dStationBridge    = GetStationOnJijum(0, 0);
		double dWidthBetweenCurb = toM(GetWidthTotalRoadWay(dStationBridge));
		
		if(m_dUnderGroundPaveHeightPyo>0 && m_dUnderGroundPaveWidthPyo<=0)
			m_dUnderGroundPaveWidthPyo = dWidthBetweenCurb;
		if(m_dUnderGroundPaveHeightGee>0 && m_dUnderGroundPaveWidthGee<=0)
			m_dUnderGroundPaveWidthGee = dWidthBetweenCurb;
		if(m_dUnderGroundPaveHeightMiddle>0 && m_dUnderGroundPaveWidthMiddle<=0)
			m_dUnderGroundPaveWidthMiddle = dWidthBetweenCurb;
		if(m_dUnderGroundPaveHeightBozo>0 && m_dUnderGroundPaveWidthBozo<=0)
			m_dUnderGroundPaveWidthBozo = dWidthBetweenCurb;
		if(m_dUnderGroundPaveHeightDong>0 && m_dUnderGroundPaveWidthDong<=0)
			m_dUnderGroundPaveWidthDong = dWidthBetweenCurb;
		if(m_dUnderGroundPaveHeightEps>0 && m_dUnderGroundPaveWidthEps<=0)
			m_dUnderGroundPaveWidthEps = dWidthBetweenCurb;
		if(GetUnderGroundPaveHeightTosa()>0 && m_dUnderGroundPaveWidthTosa<=0)
			m_dUnderGroundPaveWidthTosa = dWidthBetweenCurb;
	}
}

double CRcBridgeApp::GetThickArchRibForCalc(long nPos, BOOL bReal)			// CHECK_POS_LEFT, CHECK_POS_CENTER, CHECK_POS_RIGHT
{
	double dThick = 0;
	
	double dThickCrown = Round(GetThickArchRib(0, TRUE), -1);
	double dDistStt = fabs(m_tvArrVertSectionRibCen.GetAt(0).m_v1.x - m_pbRibCen.m_xyCrown.x);
	double dDistEnd = fabs(m_tvArrVertSectionRibCen.GetAt(m_tvArrVertSectionRibCen.GetSize()-1).m_v2.x - m_pbRibCen.m_xyCrown.x);
	
	double dThickStt = Round(GetThickArchRib(dDistStt, TRUE), -1);
	double dThickEnd = Round(GetThickArchRib(dDistEnd, TRUE), -1);
	
	if(((dThickStt/dThickCrown)<1.5 || (dThickEnd/dThickCrown)<1.5) && !bReal)
	{
		dThick = dThickCrown;
	}
	else
	{
		switch(nPos)
		{
		case CHECK_POS_LEFT:
			dThick = Round(dThickStt, -1); break;
		case CHECK_POS_CENTER:
			dThick = Round(dThickCrown, -1); break;
		case CHECK_POS_RIGHT:
			dThick = Round(dThickEnd, -1); break;
		default:
			dThick = 0;
		}
	}
	
	return dThick;
}

double CRcBridgeApp::GetOutValueArchRibShell(long nSpan,  long nPos, long nTypeDesign, long nTypeForce)
{
	if(nTypeForce!=ELE_MOMENT2 && nTypeForce!=ELE_SHEAR_2) return 0;
	if(nPos==CHECK_POS_CENTER  && nTypeForce==ELE_SHEAR_2) return 0;

	if(m_bUseShellUserOutput)
	{
		long nSizeRow = m_MatShellSlab.GetRow();
		long nSizeCol = m_MatShellSlab.GetCol();
		if(nSizeCol==0 || nSizeRow==0) return 0;
		long nRow = nSpan*2 + nPos;
		long nCol = 0;
		if(nTypeDesign==COMBO_SERVICE)
		{
			if(nTypeForce==ELE_MOMENT2) nCol = 3;
			if(nTypeForce==ELE_SHEAR_2) nCol = 4;
		}
		else
		{
			if(nTypeForce==ELE_MOMENT2) nCol = 0;
			if(nTypeForce==ELE_SHEAR_2) nCol = 1;
			if(nTypeForce==ELE_AXFORCE) nCol = 2; // 실제로는 값이 없음...
		}
		if(nRow>=nSizeRow || nCol>=nSizeCol) return 0;
		return m_MatShellSlab(nRow,nCol);
	}

	CFEMManage *pFEM = &m_FemShell;
	if(nTypeDesign==COMBO_EARTHQUAKE2 || nTypeDesign==COMBO_EARTHQUAKE) return 0;
	if(pFEM->IsOutputLoaded()==FALSE) return 0;

	long nShellForce = (nTypeForce==ELE_MOMENT2) ? MOMENT_M11 : VER_13;

	double dOutputMax = 0;
	double dOutputMin = 0;
	double dValueMax  = 0;
	double dValueMin  = 0;
	double dValueEleMax[5] = {0,0,0,0,0};
	double dValueEleMin[5] = {0,0,0,0,0};

	CString szCombo = "ENVUSE MAX";
	if(nTypeDesign==COMBO_ULTIMATE)    szCombo = "ENVULT MAX";
	if(nTypeDesign==COMBO_ECCENTRIC)   szCombo = "ENVECC MAX";

	BOOL bFirstCHeck = TRUE;

	long nLoadCase  = pFEM->GetLoadCaseIndex(szCombo);
	if(nLoadCase==-1) return 0;

	long nSizeShell = pFEM->GetShellSize();
	long nIndexNear = -1;
	long nJointNear[2] = {0,0};
	for(long nShell=0; nShell<nSizeShell; nShell++)
	{
		CShell *pShell  = pFEM->GetShell((USHORT)nShell);
		CShell *pShell2 = NULL; // 끝단부재력 구하는 경우에 입접한 부재의 값도 같이 비교...

		if(pShell->m_nAttrib!=SHELL_ELEMENT_ARCHRIB) continue;
		if(pShell->m_LongAttrib[LONGSHELL_1_JIJUMNO]!=nSpan) continue;
		if(nPos==CHECK_POS_LEFT)
		{
			if(pShell->m_BoolAttrib[BOOLSHELL_0_CHECKPOS_STT]==FALSE) continue;
			nIndexNear = GetIndexShellNearByJointNumber(pShell,pShell->m_nJ[1],pShell->m_nJ[2]);
			pShell2 = (nIndexNear>=0) ? pFEM->GetShell((USHORT)nIndexNear) : NULL; // 좌측에 붙은 부재..
		}
		else if(nPos==CHECK_POS_CENTER)
		{
			if(pShell->m_BoolAttrib[BOOLSHELL_0_CHECKPOS_STT]) continue;
			if(pShell->m_BoolAttrib[BOOLSHELL_1_CHECKPOS_END]) continue;
		}
		else if(nPos==CHECK_POS_RIGHT)
		{
			if(pShell->m_BoolAttrib[BOOLSHELL_1_CHECKPOS_END]==FALSE) continue;
			nIndexNear = GetIndexShellNearByJointNumber(pShell,pShell->m_nJ[0],pShell->m_nJ[3]);
			pShell2 = (nIndexNear>=0) ? pFEM->GetShell((USHORT)nIndexNear) : NULL; // 우측에 붙은 부재..
		}
		dValueEleMax[0] = pShell->m_ResultForce[0].GetAt(nLoadCase)->m_dForce[nShellForce];
		dValueEleMax[1] = pShell->m_ResultForce[1].GetAt(nLoadCase)->m_dForce[nShellForce];
		dValueEleMax[2] = pShell->m_ResultForce[2].GetAt(nLoadCase)->m_dForce[nShellForce];
		dValueEleMax[3] = pShell->m_ResultForce[3].GetAt(nLoadCase)->m_dForce[nShellForce];
		dValueEleMax[4] = pShell->m_ResultForce[4].GetAt(nLoadCase)->m_dForce[nShellForce]; // cent
		//
		dValueEleMin[0] = pShell->m_ResultForce[0].GetAt(nLoadCase+1)->m_dForce[nShellForce];
		dValueEleMin[1] = pShell->m_ResultForce[1].GetAt(nLoadCase+1)->m_dForce[nShellForce];
		dValueEleMin[2] = pShell->m_ResultForce[2].GetAt(nLoadCase+1)->m_dForce[nShellForce];
		dValueEleMin[3] = pShell->m_ResultForce[3].GetAt(nLoadCase+1)->m_dForce[nShellForce];
		dValueEleMin[4] = pShell->m_ResultForce[4].GetAt(nLoadCase+1)->m_dForce[nShellForce]; // cent

		if(m_nSelect3DOutputValue==0) // 평균값
		{
			if(pShell2)
			{
				dValueEleMax[0] = pShell2->m_ResultForce[0].GetAt(nLoadCase)->m_dForce[nShellForce];
				dValueEleMax[1] = pShell2->m_ResultForce[1].GetAt(nLoadCase)->m_dForce[nShellForce];
				dValueEleMax[2] = pShell2->m_ResultForce[2].GetAt(nLoadCase)->m_dForce[nShellForce];
				dValueEleMax[3] = pShell2->m_ResultForce[3].GetAt(nLoadCase)->m_dForce[nShellForce];
				//
				dValueEleMin[0] = pShell2->m_ResultForce[0].GetAt(nLoadCase+1)->m_dForce[nShellForce];
				dValueEleMin[1] = pShell2->m_ResultForce[1].GetAt(nLoadCase+1)->m_dForce[nShellForce];
				dValueEleMin[2] = pShell2->m_ResultForce[2].GetAt(nLoadCase+1)->m_dForce[nShellForce];
				dValueEleMin[3] = pShell2->m_ResultForce[3].GetAt(nLoadCase+1)->m_dForce[nShellForce];
			}
			dValueMax = (dValueEleMax[0]+dValueEleMax[1]+dValueEleMax[2]+dValueEleMax[3])/4.0;
			dValueMin = (dValueEleMin[0]+dValueEleMin[1]+dValueEleMin[2]+dValueEleMin[3])/4.0;
		}
		else if(m_nSelect3DOutputValue==1) // 최대값
		{
			dValueMax = max(dValueEleMax[0],max(dValueEleMax[1],max(dValueEleMax[2],dValueEleMax[3])));
			dValueMin = min(dValueEleMin[0],min(dValueEleMin[1],min(dValueEleMin[2],dValueEleMin[3])));
			if(pShell2)
			{
				nJointNear[0] = (nPos==CHECK_POS_LEFT) ? 0 : 1;
				nJointNear[1] = (nPos==CHECK_POS_LEFT) ? 3 : 2;
				double dValueEleMaxNear[2] = {0,0};
				double dValueEleMinNear[2] = {0,0};
				dValueEleMaxNear[0] = pShell2->m_ResultForce[nJointNear[0]].GetAt(nLoadCase)->m_dForce[nShellForce];
				dValueEleMaxNear[1] = pShell2->m_ResultForce[nJointNear[1]].GetAt(nLoadCase)->m_dForce[nShellForce];
				dValueEleMinNear[0] = pShell2->m_ResultForce[nJointNear[0]].GetAt(nLoadCase+1)->m_dForce[nShellForce];
				dValueEleMinNear[1] = pShell2->m_ResultForce[nJointNear[1]].GetAt(nLoadCase+1)->m_dForce[nShellForce];
				dValueMax = max(dValueMax,max(dValueEleMaxNear[0],dValueEleMaxNear[1]));
				dValueMin = min(dValueMin,max(dValueEleMinNear[0],dValueEleMinNear[1]));
			}
		}
		else if(m_nSelect3DOutputValue==2) // cent값
		{
			if(pShell2)
			{
				dValueEleMax[4] = pShell2->m_ResultForce[4].GetAt(nLoadCase)->m_dForce[nShellForce];  // cent
				dValueEleMin[4] = pShell2->m_ResultForce[4].GetAt(nLoadCase+1)->m_dForce[nShellForce]; // cent
			}
			dValueMax = dValueEleMax[4];
			dValueMin = dValueEleMin[4];
		}


		if(bFirstCHeck || dOutputMax<dValueMax)	dOutputMax = dValueMax;
		if(bFirstCHeck || dOutputMin>dValueMin)	dOutputMin = dValueMin;

		bFirstCHeck = FALSE;
	}

	double dValueReturn = dOutputMax;
	if(nTypeForce==ELE_MOMENT2 && nPos!=CHECK_POS_CENTER)
	{
		dValueReturn = dOutputMin;
	}
	if(nTypeForce==ELE_SHEAR_2)
	{
		dValueReturn = max(fabs(dOutputMax),fabs(dOutputMin));
	}

	return dValueReturn;
}

// 상부 슬래브 피복 리턴
// bApplyAngle : 실제로 각도에 따라서 적용된 피복
double CRcBridgeApp::GetCoverSide_UpperSlab(long nDan, double dSta, CDPoint vAng, BOOL bApplyAngle, BOOL bLeft)
{
	if(nDan < 0 || nDan > MAX_REBAR_DAN-1) return 0;
	double dCover	= m_dCoverSide_UpperSlab[nDan];	// 상부슬래브 측면[nDan];
	
	if(bApplyAngle)
	{
		CLineInfo *pLineSide	= GetLineLeftRight(bLeft);
		CLineInfo *pLineC		= GetLineBase();

		double dW	= bLeft ? m_dWidthSlabLeft : m_dWidthSlabRight;
		CDPoint xySide	= GetXySlabEdgeAct(dSta, vAng, bLeft);
		double dSideSta(0);
		pLineSide->GetStationMatchByXyAndAngle(xySide, CDPoint(0, 1), dSideSta, dW);

		// 중심선에서 vAng로의 방위각
		double dAng		= pLineC->GetAngleAzimuth(dSta).Rotate90().GetAngleDegree() - (90 - vAng.GetAngleDegree());
		vAng	= ToDPointFrDegree(dAng);
		double dAngLine	= pLineSide->GetAngleAzimuth(dSideSta).RotateInv90().GetAngleDegree();
		vAng			= ToDPointFrDegree(dAngLine + (dAng + 180 - dAngLine));

		double dStaRebar(0);
		pLineSide->GetStationMatchByXyAndAngle(xySide, vAng, dSideSta, dW);
		pLineSide->GetStationMatchByXyAndAngle(xySide, vAng, dStaRebar, bLeft ? dW + dCover : dW - dCover);
		CDPoint xyRebar	= pLineSide->GetXyLineDis(dStaRebar, bLeft ? dW + dCover : dW - dCover);
		dCover	= ~(xySide - xyRebar);
	}

	return dCover;
}

// 가각 끝단 자르기 적용 되었는지?
BOOL CRcBridgeApp::IsCutEndOfExp(BOOL bStt, BOOL bLeft)
{
	if(!IsGagakForLine(bStt, bLeft)) return FALSE;

	long left	= bLeft ? 0 : 1;
	return bStt ? m_bExpCutStt[left] : m_bExpCutEnd[left];
}

// 신축이음 종단좌표
void CRcBridgeApp::GetTvVertSectionFxJoint(BOOL bStt, BOOL bVertDir)
{
	CFlexibleJoint *pFx	= bStt ? &m_fxJointStt : &m_fxJointEnd;
	pFx->m_tvArrSection.RemoveAll();
	if(!IsBlockOut(bStt)) return;
	double dTP	= GetThickPave(FALSE);

	// 종단선형따라 그릴지? 직선으로 그릴지?
	BOOL bDrawJongdanLine	= m_nTypeJongdanForRebar == 2;
	CString sStt	= bStt ? "시점" : "종점";
	long nJ	= bStt ? 0 : m_nQtyJigan;
	BOOL bApplyAngle	= !bVertDir;
	double dLengthBridge	= fabs(GetXySectionSttEnd(FALSE, 0, bVertDir).x-GetXySectionSttEnd(TRUE, 0, bVertDir).x);
	

	CTwinVector tv;
	tv.m_bApplyAngle	= bApplyAngle;
	tv.m_dAngle			= bApplyAngle ? GetAngleJijum(nJ, bVertDir).GetAngleDegree() : 90;
	tv.m_v1				= GetXySectionSttEnd(bStt, 0, bVertDir);
	if(!bStt)
	{
		if(bVertDir)
		{
			tv.m_v1		= GetXySlabUpperByLenFrSttStation(dLengthBridge-pFx->m_dWidth/tv.GetVDir().y, 0, bVertDir);
		}
		else
		{
			tv.m_v1		= GetXySlabUpperByOffsetFrStation(GetXySectionSttEnd(FALSE, 0, bVertDir).x, -pFx->m_dWidth, 0);
		}
	}

	if(bVertDir)
		tv.m_v2			= GetXySlabUpperByLenFrSttStation((bStt ? pFx->m_dWidth : dLengthBridge)/tv.GetVDir().y, 0, bVertDir);
	else
	{
		tv.m_v2			= GetXySlabUpperByOffsetFrStation(GetXySectionSttEnd(bStt, 0, bVertDir).x, bStt ? pFx->m_dWidth : 0, 0);
		tv.m_dAngle		= GetAngleJijumByStaToSta(tv.m_v1.x, tv.m_v2.x, pFx->m_dWidth, 0);
	}

	tv.m_v1.y			+= dTP;
	tv.m_v2.y			+= dTP;
	tv.m_v1.z			= bDrawJongdanLine ? 5 : 0;
	tv.m_sInfo			= sStt + "신축이음상면";
	tv.val.valHor		= NULL;
	tv.val.valVer		= NULL;
	pFx->m_tvArrSection.Add(tv);

	tv.m_dAngle	= 90;
	tv.m_v1	= tv.m_v2;
	tv.m_v2	= tv.m_v1 + CDPoint(0, -pFx->m_dHeight);
	tv.m_v1.z	= 0;
	tv.m_sInfo	= sStt + "신축이음우측면";
	pFx->m_tvArrSection.Add(tv);

	tv.m_dAngle	= pFx->m_tvArrSection.GetAt(0).m_dAngle;
	tv.m_v1	= tv.m_v2;
	tv.m_v2	= pFx->m_tvArrSection.GetAt(0).m_v1 + CDPoint(0, -pFx->m_dHeight);
	tv.m_v1.z			= bDrawJongdanLine ? 5 : 0;
	tv.m_sInfo	= sStt + "신축이음하면";
	pFx->m_tvArrSection.Add(tv);

	tv.m_dAngle	= 90;
	tv.m_v1	= tv.m_v2;
	tv.m_v2	= pFx->m_tvArrSection.GetAt(0).m_v1;
	tv.m_v1.z	= 0;
	tv.m_sInfo	= sStt + "신축이음좌측면";
	pFx->m_tvArrSection.Add(tv);

}

// 시점과 종점이 각도가 같은지?
BOOL CRcBridgeApp::IsSameAngleSttAndEnd()
{
	double dAngStt	= GetAngleAzimuthJijum(0).GetAngleDegree();
	double dAngEnd	= GetAngleAzimuthJijum(m_nQtyJigan).GetAngleDegree();

	return Compare(dAngStt, dAngEnd, "=");
}

// 지간의 직거리 표현이 가능한지?
BOOL CRcBridgeApp::IsEnableDisplayVertLengthOfJigan()
{
	return IsSameAngleSttAndEnd() && IsIncludeOnlyLineInBridge();
}
// CString CRcBridgeApp::GetStringSelect2DFrameTool()
// {
// 	CString str = _T("");
// 
// 	switch(m_nSelect2DFrameTool)
// 	{
// 	case 0:
// 		str = _T(" Sap 2000 7"); break;
// 	case 1:
// 		str = _T(" Sap 2000 8"); break;
// 	case 2:
// 		str = _T(" Sap 2000 9"); break;
// 	case 3:
// 		str = _T(" Sap 2000 10"); break;
// 	case 4:
// 		str = _T(" MIDAS Civil"); break;
// 	case 5:
// 		str = _T(" NeoMax3D"); break;
// 	}
// 
// 	return str;
// }

// 가각이 있는지?
// 가각이 없는데 가각그리는 옵션으로 그리면 이상하게 나옴 ㅠㅠ
BOOL CRcBridgeApp::IsExp(double dSta)
{
	if(!m_bIsExp) return FALSE;

	double dStaStt	= GetStationBridgeStt(FALSE);
	double dStaEnd	= GetStationBridgeStt(FALSE) + m_dLengthBridge;

	if(fabs(dStaStt - dSta) < fabs(dStaEnd - dSta))
	{
		if(IsGagakForLine(TRUE, TRUE) || IsGagakForLine(TRUE, FALSE))
			return TRUE;
	}
	else
	{
		if(IsGagakForLine(FALSE, TRUE) || IsGagakForLine(FALSE, FALSE))
			return TRUE;

	}

	return FALSE;
}

BOOL CRcBridgeApp::IsExpRound(BOOL bStt, BOOL bLeft)
{
	BOOL bRet = FALSE;
	long nLeft = bLeft? 0 : 1;

	double dR = bStt? m_dExpSttR[nLeft] : m_dExpEndR[nLeft];

	bRet = (dR > 0)? TRUE : FALSE;

	return bRet;
}

void CRcBridgeApp::ModifyVertSectionForWing(CTwinVectorArray &tvArr, BOOL bStt, BOOL bLeft)
{
	double dELDiff = 0;
	if(IsBoxType() && m_bSlopeLowerSlab)
	{
		double dSta1 = bStt? (m_dStationBridgeStt + m_dWS) : (m_dStationBridgeStt + GetLengthBridge(TRUE) + m_dWE);
		double dEL = GetXyLowerSlabBottomByOffsetFrStation(dSta1, 0, (bLeft==bStt)? 1 : -1).y;

		CTwinVector tvLowerOfLowerSlab;

		if(!tvArr.GetTvByInfo("하부슬래브하면(외측벽체안쪽)", tvLowerOfLowerSlab)) return;
		double dELCur	= bStt ? tvLowerOfLowerSlab.m_v1.y : tvLowerOfLowerSlab.m_v2.y;
		dELDiff	= dEL - dELCur;
	}
	else
	{
		// 시점에서는 교량의 Left와 날개벽의 Left가 반대다.
		if(bStt)
			bLeft	= !bLeft;

		double dEL	= GetElSlabEndAtBriEnd(bStt, bLeft, TRUE);

		CTwinVector tvUpperOfWall;

		if(!tvArr.GetTvByInfo("슬래브상면(외측벽체안쪽)", tvUpperOfWall)) return;
		double dELCur	= bStt ? tvUpperOfWall.m_v1.y : tvUpperOfWall.m_v2.y;
		dELDiff	= dEL - dELCur;
	}

	CRebarPlacing rb;
	rb.MoveRebar(tvArr, CDPoint(0, dELDiff));

	// 벽체 조정
	CTwinVector tv	= tvArr.GetTvByInfo("시점벽체좌측면");
	tv.m_v1.y -= dELDiff;
	tvArr.SetTvByInfo("시점벽체좌측면", tv);

	tv	= tvArr.GetTvByInfo("시점벽체우측면");
	tv.m_v2.y -= dELDiff;
	tvArr.SetTvByInfo("시점벽체우측면", tv);

	tv	= tvArr.GetTvByInfo("시점벽체하부헌치");
	tv.m_v1.y -= dELDiff;
	tv.m_v2.y -= dELDiff;
	tvArr.SetTvByInfo("시점벽체하부헌치", tv);

	tv	= tvArr.GetTvByInfo("시점벽체하면");
	tv.m_v1.y -= dELDiff;
	tv.m_v2.y -= dELDiff;
	tvArr.SetTvByInfo("시점벽체하면", tv);

	tv	= tvArr.GetTvByInfo("종점벽체좌측면");
	tv.m_v1.y -= dELDiff;
	tvArr.SetTvByInfo("종점벽체좌측면", tv);
	
	tv	= tvArr.GetTvByInfo("종점벽체우측면");
	tv.m_v2.y -= dELDiff;
	tvArr.SetTvByInfo("종점벽체우측면", tv);

	tv	= tvArr.GetTvByInfo("종점벽체하부헌치");
	tv.m_v1.y -= dELDiff;
	tv.m_v2.y -= dELDiff;
	tvArr.SetTvByInfo("종점벽체하부헌치", tv);

	tv	= tvArr.GetTvByInfo("종점벽체하면");
	tv.m_v1.y -= dELDiff;
	tv.m_v2.y -= dELDiff;
	tvArr.SetTvByInfo("종점벽체하면", tv);
}

// 중분대, 중분대1/2이 있는지 검사하고 있는 것 중의 최대 높이 구함.
double CRcBridgeApp::GetMaxHeightCentralWall()
{
	double dHeight	= 0;

	long nHDan	= GetQtyHDan();
	long i = 0; for(i = 0; i < nHDan; i++)
	{
		CGuardWallRC *pGW	= GetGuardWallByHDan(i);
		if(!pGW) continue;
		if(pGW->IsTypeHDanRoadCentralWallFull() || pGW->IsTypeHDanRoadCentralWallHalf())
		{
			dHeight	= max(pGW->GetHeight(), dHeight);
		}

	}

	return dHeight;
}

// 슬래브가 도로중심의 오른쪽에 있는지
// 슬래브 배근시 예외 상황이 발생해서 이 경우 다른 조치를 취하기 위함.
BOOL CRcBridgeApp::IsRightFromRoadCenter()
{	
	return m_dWidthSlabLeft > 0 && m_dWidthSlabRight > 0;
}

// 가각 주철근 배근을 위해서 평면도를 수정함.
// 가각이 있고 & R이 있고 & 가각 끝단을 절단하고 & 접선방식이 아는 경우에만 해당됨
void CRcBridgeApp::ModifyPlaneForGagakRebarMain(CTwinVectorArray &tvArr)
{	
	CString str	= _T("");
	CTwinVector tv;
	long stt = 0; for(stt = 0; stt < 2; stt++)
	{
		BOOL bStt	= stt == iSTT;
		CString sStt	= bStt ? "시점" : "종점";
		CTwinVectorArray tvArrStt;
		long i = 0; for(i = 0; i < tvArr.GetSize(); i++)
		{
			if(tvArr.GetAt(i).m_sInfo == sStt)
			{
				tvArrStt.Add(tvArr.GetAt(i));
			}
		}
		if(tvArrStt.GetSize() == 0) continue;

		CTwinVector tvStt	= tvArrStt.GetAt(0);
		

		long left = 0; for(left = 0; left < 2; left++)
		{
			BOOL bLeft	= left == iLEFT;
			CString sLeft	= bLeft ? "좌측" : "우측";

			if(!IsGagakForLine(bStt, bLeft)) continue;
			if(bStt)
			{
				if(Compare(m_dExpSttR[bLeft ? 0 : 1], 0.0, "<=")) continue;
				if(m_bExpSttRoundIsTan[bLeft ? 0 : 1]) continue;
				if(!m_bExpCutStt[bLeft ? 0 : 1]) continue;
			}
			else
			{
				if(Compare(m_dExpEndR[bLeft ? 0 : 1], 0.0, "<=")) continue;
				if(m_bExpEndRoundIsTan[bLeft ? 0 : 1]) continue;
				if(!m_bExpCutEnd[bLeft ? 0 : 1]) continue;
			}

			str.Format("%s(%s%s가각부)", sLeft, sStt, sLeft);
			if(!tvArr.GetTvByInfo(str, tv)) continue;

			CDPoint xyDirTan(0 ,1);
			CDPoint xyMatch(0, 0);
			// tvStt의 길이가 1000000이 넘어가면 곡선인 경우 거의 평행하게 교차점을 찾아서 그런것임..
			// 다시 복구해서 직선각도로 뻗어서 교차점 찾자(단 이렇게 하면 가각부 배근이 비는현상 발생함..이는 어쩔 수 없음)
			CTwinVector tvSttOld	= tvStt;
			if(bStt)
			{
				if(bLeft)
				{
					xyDirTan	= (tv.m_v1 - tv.GetXyCircleCenter()).Unit().RotateInv90();
					GetXyMatchLineAndLine(tvStt.m_v2, tvStt.GetXyDir(), tv.m_v1, xyDirTan, tvStt.m_v2);

					if(tvStt.GetLength() > 1000000)
					{
						xyDirTan	= (tv.m_v1 - tv.m_v2).Unit();//(tv.m_v1 - tv.GetXyCircleCenter()).Unit().RotateInv90();
						GetXyMatchLineAndLine(tvStt.m_v2, tvStt.GetXyDir(), tv.m_v1, xyDirTan, tvStt.m_v2);
					}
				}
				else
				{
					xyDirTan	= (tv.m_v2 - tv.GetXyCircleCenter()).Unit().Rotate90();
					GetXyMatchLineAndLine(tvStt.m_v1, tvStt.GetXyDir(), tv.m_v2, xyDirTan, tvStt.m_v1);

					if(tvStt.GetLength() > 1000000)
					{
						xyDirTan	= (tv.m_v1 - tv.m_v2).Unit();//(tv.m_v2 - tv.GetXyCircleCenter()).Unit().Rotate90();
						GetXyMatchLineAndLine(tvStt.m_v1, tvStt.GetXyDir(), tv.m_v2, xyDirTan, tvStt.m_v1);
					}
				}
			}
			else
			{
				if(bLeft)
				{
					xyDirTan	= (tv.m_v2 - tv.GetXyCircleCenter()).Unit().Rotate90();
					GetXyMatchLineAndLine(tvStt.m_v1, tvStt.GetXyDir(), tv.m_v2, xyDirTan, tvStt.m_v1);

					if(tvStt.GetLength() > 1000000)
					{
						xyDirTan	= (tv.m_v1 - tv.m_v2).Unit();//(tv.m_v2 - tv.GetXyCircleCenter()).Unit().Rotate90();
						GetXyMatchLineAndLine(tvStt.m_v1, tvStt.GetXyDir(), tv.m_v2, xyDirTan, tvStt.m_v1);
					}
				}
				else
				{
					xyDirTan	= (tv.m_v1 - tv.GetXyCircleCenter()).Unit().RotateInv90();
					GetXyMatchLineAndLine(tvStt.m_v2, tvStt.GetXyDir(), tv.m_v1, xyDirTan, tvStt.m_v2);

					if(tvStt.GetLength() > 1000000)
					{
						xyDirTan	= (tv.m_v1 - tv.m_v2).Unit();//(tv.m_v1 - tv.GetXyCircleCenter()).Unit().RotateInv90();
						GetXyMatchLineAndLine(tvStt.m_v2, tvStt.GetXyDir(), tv.m_v1, xyDirTan, tvStt.m_v2);
					}
				}
			}

			// 시종점 업데이트
			tvArr.SetTvByInfo(sStt, tvStt);
		}
	}
}

// 지간의 거리
// 양쪽 벽체 사이의 간격
double CRcBridgeApp::GetLengthBetweenWall(long nJigan, BOOL bVertDir)
{
	double dStt	= 0;
	double dEnd	= 0;

	long nIdxWallStt	= nJigan-1;
	long nIdxWallEnd	= nJigan;
	CWallApp *pWallStt	= GetInWall(nIdxWallStt);
	CWallApp *pWallEnd	= GetInWall(nIdxWallEnd);

	dStt	= GetStationOnJijum(nJigan, 0, bVertDir);
	dEnd	= GetStationOnJijum(nJigan+1, 0, bVertDir);
	CDPoint vAngStt	= GetAngleJijum(nIdxWallStt+1, bVertDir);
	CDPoint vAngEnd	= GetAngleJijum(nIdxWallEnd+1, bVertDir);


	if(pWallStt)
	{
		if(pWallStt->m_nType == WALL_TYPE_V)
		{
			CTwinVector tv;
			if(pWallStt->m_tvArrFront.GetTvByInfo("벽체우측팔상면", tv))
				dStt	= tv.m_v1.x;
		}
		else if(m_nTypeBridge == BRIDGE_TYPE_SLAB)
		{
			long nIdxHunch	= GetIdxHunchInWall(nIdxWallStt, FALSE);
			if(nIdxHunch > -1)
			{
				CTwinVector tv;
				CString str = _T("");
				str.Format("%d번째내측헌치아래우측면", nIdxHunch+1);
				if(m_tvArrVertSection.GetTvByInfo(str, tv))
					dStt	= tv.m_v1.x;
			}
		}
		else
		{
			dStt += pWallStt->GetWidthWall()/vAngStt.y/2;
		}

	}
	else
	{
		if(!IsOutWall(TRUE))
		{
			CTwinVector tv;
			m_tvArrVertSection.GetTvByInfo("시점슬래브측면", tv);
			dStt	= tv.m_v2.x;
		}
	}

	if(pWallEnd)
	{
		if(pWallEnd->m_nType == WALL_TYPE_V)
		{
			CTwinVector tv;
			pWallEnd->m_tvArrFront.GetTvByInfo("벽체좌측팔상면", tv);
			dEnd	= tv.m_v1.x;
		}
		else if(m_nTypeBridge == BRIDGE_TYPE_SLAB)
		{
			long nIdxHunch	= GetIdxHunchInWall(nIdxWallEnd, TRUE);
			if(nIdxHunch > -1)
			{
				CTwinVector tv;
				CString str = _T("");
				str.Format("%d번째내측헌치아래좌측면", nIdxHunch+1);
				if(m_tvArrVertSection.GetTvByInfo(str, tv))
					dEnd	= tv.m_v2.x;
			}
		}
		else
		{
			dEnd -= pWallEnd->GetWidthWall()/vAngEnd.y/2;
		}
	}
	else
	{
		if(!IsOutWall(FALSE))
		{
			CTwinVector tv;
			m_tvArrVertSection.GetTvByInfo("종점슬래브측면", tv);
			dEnd	= tv.m_v1.x;
		}
	}
	

	return dEnd-dStt;
}

// long nPos : 0 : 교량길이만, 1 : 방호벽 추가 길이만, 2 : 합친거
long CRcBridgeApp::GetGuardWallTotalLength(CGuardWallRC *pGW, long nPos)
{
	CLineInfo* pLineLeft = GetLineLeftRight(TRUE);
	CLineInfo* pLineRight = GetLineLeftRight(FALSE);
	CLineInfo* pLineBase = GetLineBase();
	CJijum* pJL = GetJijum(0);

	CDPoint vAng = CDPoint(0,1);
	double sta = GetStationOnJijum(0,0);
	if(m_nTypeBridge!=BRIDGE_TYPE_PORTAL)
		sta += pJL->m_dLengthUgan[1];	

	long   nHDanCur	= 0;	// 구하고자 하는 방호벽의 구간
	long   nHDanNode = GetQtyHDan();
	double dMid = 0;
	long i=0; for(i=0; i<nHDanNode; i++)
	{
		CGuardWallRC* pGW2 = GetGuardWallByHDan(i);
		if(!pGW2) continue;
		if(pGW2->m_nNumberHDan == pGW->m_nNumberHDan) 
		{
			nHDanCur = i;
			break;
		}
		dMid += GetLengthHDanAct(sta, vAng, i);
	}
	dMid = dMid + pGW->GetWidth()/2;

	double dLeft  = m_dWidthSlabLeft + pGW->GetWidth()/2;
	double dRight = m_dWidthSlabRight - pGW->GetWidth()/2;
	double dBase  = dMid + GetDisSlabLeftAct(sta, vAng);

	long nBriLength = 0;
	if(nHDanCur == 0)
	{
		double staResultSL = GetStationAtSlabEnd(TRUE, -1, dLeft);
		double staResultEL = GetStationAtSlabEnd(FALSE, -1, dLeft);
		if(nPos == 0 || nPos == 2)
		{
			nBriLength = (long)(Round(pLineLeft->GetLengthByDis(staResultSL,staResultEL,dLeft),0)); 
		}

		if(nPos == 1 || nPos == 2)
		{
			nBriLength += (long)frM(m_dAddLenghForGuardWall[0][1] + m_dAddLenghForGuardWall[1][0]);
		}
	}
	else if(nHDanCur == nHDanNode-1)
	{
		double staResultSR = GetStationAtSlabEnd(TRUE, 1, dRight);
		double staResultER = GetStationAtSlabEnd(FALSE, 1, dRight);
		if(nPos == 0 || nPos == 2)
		{
			nBriLength = (long)(Round(pLineRight->GetLengthByDis(staResultSR,staResultER,dRight),0)); 
		}

		if(nPos == 1 || nPos == 2)
		{
			nBriLength += (long)frM(m_dAddLenghForGuardWall[0][0] + m_dAddLenghForGuardWall[1][1]);
		}
	}
	else
	{
		double staResultSB = GetStationAtSlabEnd(TRUE, 0, dBase);
		double staResultEB = GetStationAtSlabEnd(FALSE, 0, dBase);

		if(nPos == 0 || nPos == 2)
		{
			nBriLength = (long)(Round(pLineBase->GetLengthByDis(staResultSB,staResultEB,dRight),0)); 
		}
	}

	return nBriLength;
}


// 가각 끝단 자르기 할때 자르는 각도
CDPoint CRcBridgeApp::GetAngleCutGagakEnd(BOOL bStt, BOOL bLeft)
{
	CDPoint xyDirCut	=  CDPoint(0, 1);
	if(bStt)
		xyDirCut	=  ToDPointFrDegree(GetAngleAzimuthJijum(m_nQtyJigan).GetAngleDegree() + (90 - GetAngleJijum(m_nQtyJigan).GetAngleDegree()) - 90);
	else
		xyDirCut	= ToDPointFrDegree(GetAngleAzimuthJijum(0).GetAngleDegree() + (90 - GetAngleJijum(0).GetAngleDegree()) + 90);

	return xyDirCut;
}

double CRcBridgeApp::GetLengthBracket(BOOL bStt)
{
	CTwinVectorArray tvArrPlane;
	GetTvPlaneOutWall(tvArrPlane, bStt, TRUE, FALSE);
	CTwinVector tvPath	= tvArrPlane.GetTvByInfo("브라켓외측");

	return tvPath.GetLength();
}

double CRcBridgeApp::GetLengthBFooting(BOOL bStt)
{
	CTwinVectorArray tvArrPlane;
	GetTvPlaneOutWall(tvArrPlane, bStt, FALSE, FALSE, TRUE);
	CTwinVector tvPath	= tvArrPlane.GetTvByInfo("부상방지저판");

	return tvPath.GetLength();
}

void CRcBridgeApp::GetTvVertSectionBracket(CTwinVectorArray &tvArr, BOOL bStt)
{
	tvArr.RemoveAll();

	CTwinVector tvInner;
	CString sStt	= bStt ? "시점브라켓" : "종점브라켓";
	CTwinVector tv	= m_tvArrVertSection.GetTvByInfo(sStt + "헌치");
	tvInner.m_v1	= bStt ? tv.m_v1 : tv.m_v2;
	tvArr.Add(tv);

	tv	= m_tvArrVertSection.GetTvByInfo(sStt + "측면");
	tvArr.Add(tv);

	tv	= m_tvArrVertSection.GetTvByInfo(sStt + "상면");
	tvInner.m_v2	= bStt ? tv.m_v2 : tv.m_v1;
	tvArr.Add(tv);

	tvArr.Add(tvInner);

	tvArr.Sort();
}

void CRcBridgeApp::GetLenJijumForDim(CDoubleArray &dArr)
{
	dArr.RemoveAll();

	long i=0; for(i=0; i<m_nQtyJigan; i++)
	{
		CJijum *pJL = GetJijum(i);
		CJijum *pJR = GetJijum(i+1);
		if(m_bExpJoint[i] || (!IsOutWall(TRUE) && i==0))
		{
			dArr.Add(pJL->m_dLengthUgan[1]);
			dArr.Add(pJL->m_dLengthSlabToShoe[1]);
		}
		dArr.Add(m_dLengthJigan[i]);
		if(m_bExpJoint[i+1] || (!IsOutWall(FALSE) && i == m_nQtyJigan-1))
		{
			dArr.Add(pJR->m_dLengthSlabToShoe[0]);
			dArr.Add(pJR->m_dLengthUgan[0]);
		}
	}
}

/**
	@brief
		- 가각시 slop만 따로 계산한다.
		- 선형중심 좌측은 -1을 곱해서 적용한다(일반적인 경우와 동일)
*/
void CRcBridgeApp::SetXySlabUpperAct90_Gagak(double sta, CDPoint vAng)
{
	// vAng 는 sta에서의 각도기 때문에 시좀점의경우 선형과의 각도가 지점각도와는 다를수 있다.
	// 그런데 이함수는 지점 각도를 사용하기 때문에 맞지 않다.
	//if(IsSkew()==FALSE) return;

	m_bSetGagakDist = FALSE;

	CLineInfo *pLineCR = GetLineBase();

	long nQtyHDan = GetQtyHDan(FALSE);
	long nHDan=0;
	double slope=0;

	double LenSlabLeft	= GetDisSlabLeftActByGagak(sta, vAng, FALSE);
	CDPoint xyDis = pLineCR->GetXyLineByLengthAndAngle(sta, LenSlabLeft, vAng);
	double disSlab = pLineCR->GetDisMatchVerticalByXy(xyDis);

	m_nQtyHDanSlope = 0;

	m_Aslope[0].x = disSlab; // 슬래브 좌측 거리
	slope	= GetSlopeHDanByStation(sta,nHDan); // 좌측끝단의 구배

	if(Compare(disSlab, 0.0, "<")) slope *= -1;
	m_Aslope[0].y = slope;

	for(nHDan = 0; nHDan<nQtyHDan; nHDan++)
	{
		double dLengthHDanAct = GetLengthHDanAct(sta, vAng, nHDan, FALSE, TRUE, vAng);
		if(Compare(LenSlabLeft, 0.0, "<", DIST_MIN_CROSS_CENTER) && Compare(LenSlabLeft + dLengthHDanAct, 0.0, ">", DIST_MIN_CROSS_CENTER))
		{
			double disFrZero = dLengthHDanAct + m_Aslope[m_nQtyHDanSlope].x;
			m_Aslope[m_nQtyHDanSlope+1].x = 0;
			m_Aslope[m_nQtyHDanSlope+1].y = -GetSlopeHDanByStation(sta,nHDan);

			xyDis = pLineCR->GetXyLineByLengthAndAngle(sta, disFrZero, vAng);
			disSlab = pLineCR->GetDisMatchVerticalByXy(xyDis);
			m_Aslope[m_nQtyHDanSlope+2].x = disSlab;
			m_Aslope[m_nQtyHDanSlope+2].y = GetSlopeHDanByStation(sta, (nHDan+1<nQtyHDan)? nHDan+1 : nHDan);
			// (39627)중심위치에 걸리지 않는경우 추가해주는데 nHDan을 같은단의 구배를 사용하면 안된다.
			// -구간에서 시작된 단이기 때문에 +구간으로 넘어왔기때문에 추가 되었다면 다음단의 구배를 사용하자.
			m_nQtyHDanSlope++;
			
			LenSlabLeft += dLengthHDanAct;
		}
		else
		{
			LenSlabLeft += dLengthHDanAct;

			xyDis = pLineCR->GetXyLineByLengthAndAngle(sta, LenSlabLeft, vAng);
			disSlab = Round(pLineCR->GetDisMatchVerticalByXy(xyDis),5);
			slope = GetSlopeHDanByStation(sta,nHDan);
			// slope는 nHDan구간에 해당하는 slope이다. => disSlab까지의 구배
			// dLengthHDanAct의 구간에 적용되는 구배이므로 "0.0"인 경우를 포함에서 구배를 *-1 한다.
			if(Compare(disSlab, 0.0, "<")) slope *= -1;
			if(Compare(disSlab, 0.0, "=")) slope *= -1;
			m_Aslope[m_nQtyHDanSlope+1].x = disSlab;
			m_Aslope[m_nQtyHDanSlope+1].y = slope;
		}
		m_nQtyHDanSlope++;
	}
	m_dDisExpLeft = m_Aslope[0].x;
	m_dDisExpRight = m_Aslope[m_nQtyHDanSlope].x;
	m_bSetGagakDist = TRUE;
}

double CRcBridgeApp::GetSlopeHDanByDistGagak(double dDist90)
{
	double dSlope = 0;

	for(long nHDan = 0; nHDan<m_nQtyHDanSlope+1; nHDan++)
	{
		double dDistSlope = m_Aslope[nHDan].x;
		if(Round(dDist90,5)<Round(dDistSlope,5))
		{
			return m_Aslope[nHDan].y;
		}
	}

	dSlope = m_Aslope[m_nQtyHDanSlope].y;

	return dSlope;
}

long CRcBridgeApp::GetCountVariableHDanLeft(BOOL bRoadNotSlab)
{
	long nCount	= 0;
	long nQtyHDan = GetQtyHDan(bRoadNotSlab);
	long i = 0; for(i = 0; i < nQtyHDan; i++)
	{
		if(IsVariableLenHDan(i, bRoadNotSlab) && IsLeftHDan(i,bRoadNotSlab))
			nCount++;
	}
	
	return nCount;
}


BOOL CRcBridgeApp::IsConnect(CTwinVectorArray &tvArr)
{
	if(tvArr.GetSize() == 0) return FALSE;

	BOOL bIsConnect = TRUE;

	CTwinVector tv1, tv2;

	for(long ix = 0; ix < tvArr.GetSize() - 1; ix ++)
	{
		tv1 = tvArr.GetAt(ix);
		tv2 = tvArr.GetAt(ix+1);

		if(ComparePt(tv1.m_v2, tv2.m_v1, "=", 0.00001) == FALSE)
		{
			bIsConnect = FALSE;
		}
	}

	return bIsConnect;
}


//가각부의 CTwinVectorArray의 길이와 위치의 문제로 인하여 offset이 되지 않게 된다. 때문에 가각부의 CTwinVectorArray를 한개로 병합하여 offset이 되도록 만들어 준다.
////#0029125 - 옵셋을 하면서 문제를 발생 
//GetTvRebarOut_Plane_UpperSlab()의 함수에서 bExp == TRUE, bCutEnd == FALSE인 경우에만 특화되어지게 만든 함수다.
void CRcBridgeApp::GetOffsetTvRebarOut_Plane_UpperSlab(CTwinVectorArray &tvArrPlan, double dCover, double dExpSttLeftR, double dExpSttRightR, double dExpEndLeftR, double dExpEndRightR)
{
	CTwinVectorArray tvArr, tvArrLine, tvArrTmp;

	CRebarPlacing rb;

	CTwinVector tvStt, tvEnd, tv1, tv2;

	tvArr.GetTvByInfo(_T("시점"), tvStt);
	tvArr.GetTvByInfo(_T("종점"), tvEnd);

// 	BOOL bIsConnect = TRUE; //CTwinVectorArray의 개체가 모두 연결된 상태인지 판별

	//우측(시점우측가각부)
	tvArr = tvArrPlan;
	tvArr.GetTvArrByInfo(_T("우측(시점우측가각부)"), tvArrLine, FALSE, TRUE);

	tvArrLine.Add(tvStt);

	rb.GetOffsetTvArray(tvArrLine, dCover, FALSE);

	tvArrLine.Sort();
	
	BOOL bIsConnect = IsConnect(tvArrLine);
	
	if(bIsConnect == FALSE && dExpSttRightR > 0) //offset이 정상적으로 되어 있지 않다.
	{
		tvArr.GetTvArrByInfo(_T("우측(시점우측가각부)"), tvArrLine, TRUE, TRUE);
		
		if(tvArrLine.GetSize() == 2)
		{
			tvArrLine.Sort();

			tv1 = tvArrLine.GetAt(0);
			tv2 = tvArrLine.GetAt(1);
			double dSttR[2] = {0, 0};
			dSttR[0] = tv1.GetLength();
			dSttR[1] = tv2.GetLength();

			if(dSttR[0] < dSttR[1])
			{
				tv2.m_v1 = (CDPoint)tv1.m_v1;

				tvArrLine.SetAt(1, tv2);

				tvArrLine.RemoveAt(0);

				tvArr.AddFromTvArray(tvArrLine);						
			}
			else
			{
				tv1.m_v2 = (CDPoint)tv2.m_v2;

				tvArrLine.SetAt(0, tv1);

				tvArrLine.RemoveAt(1);

				tvArr.AddFromTvArray(tvArrLine);
			}
		}
	}


	//우측(종점우측가각부)
	tvArr.GetTvArrByInfo(_T("우측(종점우측가각부)"), tvArrLine, FALSE, TRUE);

	tvArrLine.Add(tvEnd);

	rb.GetOffsetTvArray(tvArrLine, dCover, FALSE);

	tvArrLine.Sort();

	bIsConnect = IsConnect(tvArrLine);

	if(bIsConnect == FALSE && dExpEndRightR > 0) //offset이 정상적으로 되어 있지 않다.
	{
		tvArr.GetTvArrByInfo(_T("우측(종점우측가각부)"), tvArrLine, TRUE, TRUE);

		if(tvArrLine.GetSize() == 2)
		{
			tvArrLine.Sort();

			tv1 = tvArrLine.GetAt(0);
			tv2 = tvArrLine.GetAt(1);
			double dEndR[2] = {0, 0};
			dEndR[0] = tv1.GetLength();
			dEndR[1] = tv2.GetLength();

			if(dEndR[0] < dEndR[1])
			{
				tv2.m_v1 = (CDPoint)tv1.m_v1;

				tvArrLine.SetAt(1, tv2);

				tvArrLine.RemoveAt(0);

				tvArr.AddFromTvArray(tvArrLine);						
			}
			else
			{
				tv1.m_v2 = (CDPoint)tv2.m_v2;

				tvArrLine.SetAt(0, tv1);

				tvArrLine.RemoveAt(1);

				tvArr.AddFromTvArray(tvArrLine);
			}
		}
	}


	//좌측(시점좌측가각부)
	tvArr.GetTvArrByInfo(_T("좌측(시점좌측가각부)"), tvArrLine, FALSE, TRUE);

	tvArrLine.Add(tvStt);

	rb.GetOffsetTvArray(tvArrLine, dCover, FALSE);

	tvArrLine.Sort();

	bIsConnect = IsConnect(tvArrLine);

	if(bIsConnect == FALSE && dExpSttLeftR > 0) //offset이 정상적으로 되어 있지 않다.
	{
		tvArr.GetTvArrByInfo(_T("좌측(시점좌측가각부)"), tvArrLine, TRUE, TRUE);

		if(tvArrLine.GetSize() == 2)
		{
			tvArrLine.Sort();

			tv1 = tvArrLine.GetAt(0);
			tv2 = tvArrLine.GetAt(1);
			double dSttL[2] = {0, 0};
			dSttL[0] = tv1.GetLength();
			dSttL[1] = tv2.GetLength();

			if(dSttL[0] < dSttL[1])
			{
				tv2.m_v1 = (CDPoint)tv1.m_v1;

				tvArrLine.SetAt(1, tv2);

				tvArrLine.RemoveAt(0);

				tvArr.AddFromTvArray(tvArrLine);						
			}
			else
			{
				tv1.m_v2 = (CDPoint)tv2.m_v2;

				tvArrLine.SetAt(0, tv1);

				tvArrLine.RemoveAt(1);

				tvArr.AddFromTvArray(tvArrLine);
			}
		}
	}


	//좌측(종점좌측가각부)
	tvArr.GetTvArrByInfo(_T("좌측(종점좌측가각부)"), tvArrLine, FALSE, TRUE);

	tvArrLine.Add(tvEnd);

	rb.GetOffsetTvArray(tvArrLine, dCover, FALSE);

	tvArrLine.Sort();

	bIsConnect = IsConnect(tvArrLine);

	if(bIsConnect == FALSE && dExpEndLeftR > 0) //offset이 정상적으로 되어 있지 않다.
	{
		tvArr.GetTvArrByInfo(_T("좌측(종점좌측가각부)"), tvArrLine, TRUE, TRUE);

		if(tvArrLine.GetSize() == 2)
		{
			tvArrLine.Sort();

			tv1 = tvArrLine.GetAt(0);
			tv2 = tvArrLine.GetAt(1);
			double dEndL[2] = {0, 0};
			dEndL[0] = tv1.GetLength();
			dEndL[1] = tv2.GetLength();

			if(dEndL[0] < dEndL[1])
			{
				tv2.m_v1 = (CDPoint)tv1.m_v1;

				tvArrLine.SetAt(1, tv2);

				tvArrLine.RemoveAt(0);

				tvArr.AddFromTvArray(tvArrLine);						
			}
			else
			{
				tv1.m_v2 = (CDPoint)tv2.m_v2;

				tvArrLine.SetAt(0, tv1);

				tvArrLine.RemoveAt(1);

				tvArr.AddFromTvArray(tvArrLine);
			}
		}
	}


	tvArr.Sort(0.1);
	rb.GetOffsetTvArray(tvArr, dCover, FALSE);

	tvArrPlan = tvArr;
}


//31447  리턴값: 배면의 헌치를 제외한 수평철근의 실길이, tvArr 지점의 평면도 상의 총 날개벽
double CRcBridgeApp::GetWingWallBackAndSideAttach(CTwinVectorArray & tvArr,long nOut, long nDan, BOOL bLeft)
{
	if(nOut != 0) return 0;
	tvArr.RemoveAll();

	double dLenBakcTotal = 0;
	CTwinVector tvIn, tvOut, tv, tvWingHunchInner, tvResult;
	CDPoint xyMatch(0,0);
		
	tvArr.AddFromTvArray(m_tvArrPlaneWingWall);

	tvIn	 = tvArr.GetTvByInfo("슬래브끝단내측");
	tvOut = tvArr.GetTvByInfo("슬래브끝단외측");

	if(bLeft)
		tvWingHunchInner = tvArr.GetTvByInfo("좌측날개벽내부헌치후");
	else
		tvWingHunchInner = tvArr.GetTvByInfo("우측날개벽내부헌치후");
	
	if(bLeft)
	{
		tv.m_v1 = tvIn.m_v1;
		tv.m_v2 = tvOut.m_v1;
	}
	else
	{
		tv.m_v1 = tvIn.m_v2;
		tv.m_v2 = tvOut.m_v2;
	}
	
	GetXyMatchSegAndLine(tv.m_v1, tv.m_v2, tvWingHunchInner.m_v1, tvWingHunchInner.GetXyDir(), xyMatch);

	tvResult.m_v1 = tvWingHunchInner.m_v1;
	tvResult.m_v2 = xyMatch;

	dLenBakcTotal = tvResult.GetLength();

	return dLenBakcTotal;
}

//31447 
//날개벽 평면도 상의 철근 길이를 tvArr에 넣어줌 bUpper는 보통 TRUE로 설정 bUpper는 보통 TRUE로 설정
void CRcBridgeApp::GetWingWallbackAndSideAttachRebar(CTwinVectorArray & tvArr,long nOut, long nDan, BOOL bStt , BOOL bUpper, BOOL bLeft)
{
	tvArr.RemoveAll();

	CARcBridgeDataStd *pStd	= m_pARcBridgeDataStd;
	CWingWall* pWing = GetWingWall(bStt, bLeft);

	if(!pWing->m_bExist) return;

	CTwinVectorArray tvArrGen;
	CTwinVector tvOut, tvEnd, tvHuPrev, tvHuNext, tvHunch, tvSlab, tvFoot;

	CTwinVector tvSlabNoOffset;	

	GetTvWingWallPlane(bStt, TRUE, bUpper, bUpper);
	tvArrGen = m_tvArrPlaneWingWall;

	CString strDir = bLeft ? _T("좌측") : _T("우측");
	tvArrGen.GetTvByInfo(strDir+_T("날개벽외측"), tvOut);
	tvArrGen.GetTvByInfo(strDir+_T("날개벽끝단"), tvEnd);
	tvArrGen.GetTvByInfo(strDir+_T("날개벽내부헌치전"), tvHuPrev);
	tvArrGen.GetTvByInfo(strDir+_T("날개벽내부헌치"), tvHunch);
	tvArrGen.GetTvByInfo(strDir+_T("날개벽내부헌치후"), tvHuNext);
	tvArrGen.GetTvByInfo(_T("슬래브끝단외측"), tvSlab);
	tvSlabNoOffset	= tvSlab;

	BOOL bSide		= (pWing->m_nAttachPos == 0 )? FALSE : TRUE;
	double dCoverWingOut = m_dCoverWingOut[0];
	double dCoverWingIn = m_dCoverWingIn[0];
	double dCoverWingTop = m_dCoverWingUpper;
	double dCoverWingEnd = GetCoverWingWallEnd(bStt, bLeft, FALSE);
	double dCoverSlab = m_dCoverWallIn[0];
	double dT1 = pWing->m_dT1;
	double dT2 = pWing->m_dT2;
	double dLT = bUpper ? pWing->m_dLT : 0.0;
	double dDiaVar = pWing->m_dL1>0 ? pWing->m_dDiaMainD[0][nDan] : pWing->m_dDiaMainA[0][nDan];
	double dDiaIn = bUpper ? pWing->m_dDiaMainA[0][nDan] : pWing->m_dDiaMainB[0][nDan];
	double dDiaOut = bUpper ? pWing->m_dDiaMainA[1][nDan] : pWing->m_dDiaMainB[1][nDan];
	if(pWing->m_dL2==0)
	{
		dDiaVar = pWing->m_dDiaMainD[0][nDan];
		dDiaIn = pWing->m_dDiaMainD[0][nDan];
		dDiaOut = pWing->m_dDiaMainD[1][nDan];
	}
	double dCTCIn = pWing->m_dMainCTCA/2;
	double dCTCOut = pWing->m_dMainCTCA;
	double dSettleVar = pStd->GetValueSettle(dDiaVar, SETTLE_TENS_NORMAL, toCm(dCoverWingIn), toCm(dCTCIn));
	double dSettleOut = pStd->GetValueSettle(dDiaOut, SETTLE_TENS_NORMAL, toCm(dCoverSlab), toCm(dCTCOut));
	double dSettleIn = pStd->GetValueSettle(dDiaIn, SETTLE_TENS_NORMAL, toCm(dCoverSlab), toCm(dCTCIn));
	double lr = bLeft ? -1.0 : 1.0;
	CDPoint xyHu[3];

	//GetXyRebarWingWallVaryPlane(xyHu, bStt, bLeft); //이부분 수정 
	//함수를 호출하기 위해서는 캐스팅 해야 하기 때문에 귀찮아도 걍 여기다 함수 내용을 다 써준다...
	{
		CTwinVector tvOutTmp, tvHuPrevTmp, tvHuNextTmp, tvHunchTmp;

		tvOutTmp = tvOut;
		tvHuPrevTmp = tvHuPrev;
		tvHuNextTmp = tvHuNext;
		tvHunchTmp = tvHunch;

		Offset(tvOutTmp.m_v1, tvOutTmp.m_v2, -dCoverWingOut*lr);
		Offset(tvHuPrevTmp.m_v1, tvHuPrevTmp.m_v2, -dCoverWingIn*lr);
		Offset(tvHuNextTmp.m_v1, tvHuNextTmp.m_v2, -dCoverWingIn*lr);
		Offset(tvHunchTmp.m_v1, tvHunchTmp.m_v2, -dCoverWingIn*lr);

		CDPoint vOutTmp = tvOutTmp.GetXyDir();
		CDPoint vHuPrevTmp = tvHuPrevTmp.GetXyDir();
		CDPoint vHuNextTmp = tvHuNextTmp.GetXyDir();
		CDPoint vHuTmp = tvHunchTmp.GetXyDir();

		if(dT1<dT2)
		{
			GetXyMatchLineAndLine(tvHuNextTmp.m_v1, vHuNextTmp, tvHunchTmp.m_v2, vHuTmp, xyHu[0]);
			GetXyMatchLineAndLine(tvHuPrevTmp.m_v1, vHuPrevTmp, tvHunchTmp.m_v1, vHuTmp, xyHu[1]);
			GetXyMatchLineAndLine(tvOutTmp.m_v1, vOutTmp, tvHunchTmp.m_v1, vHuTmp, xyHu[2]);
		}
		else if(dT1>dT2)
		{
			GetXyMatchLineAndLine(tvHuPrevTmp.m_v1, vHuPrevTmp, tvHunchTmp.m_v2, vHuTmp, xyHu[0]);
			GetXyMatchLineAndLine(tvHuNextTmp.m_v1, vHuNextTmp, tvHunchTmp.m_v1, vHuTmp, xyHu[1]);
			GetXyMatchLineAndLine(tvOutTmp.m_v1, vOutTmp, tvHunchTmp.m_v1, vHuTmp, xyHu[2]);
		}
		else
			xyHu[0] = xyHu[1] = xyHu[2] = tvHuNextTmp.m_v1 + dCoverWingTop*vHuNextTmp;
	}
	

	int nDirHunch = dT1==dT2 ? 1 : pWing->m_nDirHunch;
	if(nDirHunch==1)
	{
		dT1 = dT2 = bUpper ? pWing->m_dT1 : pWing->m_dT2;
		dLT = 0.0;
	}

	Offset(tvSlab.m_v1, tvSlab.m_v2, -dCoverSlab);
	Offset(tvOut.m_v1, tvOut.m_v2, -dCoverWingOut*lr);
	Offset(tvEnd.m_v1, tvEnd.m_v2, -dCoverWingEnd*lr);
	Offset(tvHuNext.m_v1, tvHuNext.m_v2, -dCoverWingIn*lr);
	Offset(tvHunch.m_v1, tvHunch.m_v2, -dCoverWingIn*lr);
	Offset(tvHuPrev.m_v1, tvHuPrev.m_v2, -dCoverWingIn*lr);

	CDPoint vOut = tvOut.GetXyDir();
	CDPoint vEnd = tvEnd.GetXyDir();
	CDPoint vHuPrev = tvHuPrev.GetXyDir();
	CDPoint vHuNext = tvHuNext.GetXyDir();
	CDPoint vSlab = tvSlab.GetXyDir();

	CDPoint xySetOut(0, 0), xySetIn(0, 0);
	if(bSide)
	{
		CTwinVector tvSlabIn, tvWallSide;
		tvArrGen.GetTvByInfo(_T("슬래브끝단내측"), tvSlabIn);
		tvWallSide.m_v1 = (bLeft)? tvSlabNoOffset.m_v1 : tvSlabNoOffset.m_v2;
		tvWallSide.m_v2 = (bLeft)? tvSlabIn.m_v1 : tvSlabIn.m_v2;
		CDPoint vWallSide = tvWallSide.GetXyDir();

		GetXyMatchLineAndLine(tvOut.m_v1, vOut, tvWallSide.m_v1, vWallSide, tvOut.m_v1);
		GetXyMatchLineAndLine(tvHuNext.m_v2, vHuNext, tvWallSide.m_v1, vWallSide, tvHuNext.m_v2);
		GetXyMatchLineAndLine(tvOut.m_v1, vOut, tvEnd.m_v1, vEnd, tvEnd.m_v1);
		if(dLT==0)
			GetXyMatchLineAndLine(tvHuNext.m_v2, vHuNext, tvEnd.m_v1, vEnd, tvEnd.m_v2);
		else
			GetXyMatchLineAndLine(tvHuPrev.m_v2, vHuPrev, tvEnd.m_v1, vEnd, tvEnd.m_v2);

		xySetOut = tvOut.m_v1 - dSettleOut*vSlab*lr;
		xySetIn = tvHuNext.m_v2 - dSettleIn*vSlab*lr;
	}
	else
	{
		GetXyMatchLineAndLine(tvOut.m_v1, vOut, tvSlab.m_v1, vSlab, tvOut.m_v1);
		GetXyMatchLineAndLine(tvHuNext.m_v2, vHuNext, tvSlab.m_v1, vSlab, tvHuNext.m_v2);
		GetXyMatchLineAndLine(tvOut.m_v1, vOut, tvEnd.m_v1, vEnd, tvEnd.m_v1);
		if(dLT==0)
			GetXyMatchLineAndLine(tvHuNext.m_v2, vHuNext, tvEnd.m_v1, vEnd, tvEnd.m_v2);
		else
			GetXyMatchLineAndLine(tvHuPrev.m_v2, vHuPrev, tvEnd.m_v1, vEnd, tvEnd.m_v2);

		xySetOut = tvOut.m_v1 - dSettleOut*vSlab*lr;
		xySetIn = tvHuNext.m_v2 - dSettleIn*vSlab*lr;
	}

	if(!bUpper)
		xyHu[0] = xyHu[1] = xyHu[2] = tvEnd.m_v2;


	// 전면 수평 철근
	CTwinVector tv;

	//1
	tv.m_v1 = tvEnd.m_v1;
	tv.m_v2 = tvEnd.m_v2;
	tv.m_sInfo = _T("날개벽끝단");
	tvArr.Add(tv);

	//2
	tv.m_v1 = tvOut.m_v1;
	tv.m_v2 = tvEnd.m_v1;
	tv.m_sInfo = _T("날개벽전면수평철근1");
	tvArr.Add(tv);

	//3
	tv.m_v1 = tvOut.m_v1;
	tv.m_v2 = xySetOut;
	tv.m_sInfo = _T("날개벽전면정착장");
	tvArr.Add(tv);
	
	// 배면 수평 철근
	if(dT1<dT2)
	{
		//1
		tv.m_v1 = tvEnd.m_v2 ;
		tv.m_v2 = xyHu[1]+dSettleVar*vHuPrev ;
		tv.m_sInfo =_T("날개벽배면수평철근1");
		tvArr.Add(tv);

		//2
		tv.m_v1 = xyHu[2];
		tv.m_v2 = xyHu[0];
		tv.m_sInfo =_T("날개벽배면수평철근2");
		tvArr.Add(tv);

		//3
		tv.m_v1 = xyHu[0];
		tv.m_v2 = tvHuNext.m_v2;
		tv.m_sInfo =_T("날개벽배면수평철근3");
		tvArr.Add(tv);

	}
	else if(dT1>dT2)
	{
		//1
		tv.m_v1 =tvEnd.m_v2 ;
		tv.m_v2 = xyHu[0] ;
		tv.m_sInfo =_T("날개벽배면수평철근1");
		tvArr.Add(tv);

		//2
		tv.m_v1 =xyHu[0] ;
		tv.m_v2 = xyHu[2];
		tv.m_sInfo =_T("날개벽배면수평철근2");
		tvArr.Add(tv);

		//3
		tv.m_v1 = xyHu[1]-dSettleVar*vHuNext;
		tv.m_v2 =  tvHuNext.m_v2;
		tv.m_sInfo =_T("날개벽배면수평철근3");
		tvArr.Add(tv);
	}
	else
	{
		tv.m_v1 = tvEnd.m_v2;
		tv.m_v2 =   tvHuNext.m_v2;
		tv.m_sInfo =_T("날개벽배면수평철근1");
		tvArr.Add(tv);
	}
	
	tv.m_v1 = tvHuNext.m_v2;
	tv.m_v2 = xySetIn;
	tv.m_sInfo =_T("날개벽배면정착장");
	tvArr.Add(tv);
}

////평면도상의 수평철근 정착장을 정면도에 넣기 위해서 수평선상에서 본 값으로 보정시키는것 리턴값 : 보정된 정착장 길이 
//GetWingWallBackAndSideAttach()함수의 리턴값을 dWingBackLen로 받아와야함
double CRcBridgeApp::GetSettleRebarHoriRevision(CTwinVectorArray &tvArrSide,double dWingBackLen, long nOut, BOOL bStt, BOOL bLeft)
{
	CWingWall* pWing = GetWingWall(bStt, bLeft);

	double dSettleDistWing = 0,  dHoriDist =0;
	double dCoverWing = GetCoverWingWallEnd(bStt, bLeft);
	double dL1 = pWing->m_dL1;
	double dL2 = pWing->m_dL2;

	CTwinVector tvFrontTmp, tvFrontSettleTmp, tvBackTmp, tvBackSettleTmp , tvWingEnd;

	tvFrontTmp = tvArrSide.GetTvByInfo(_T("날개벽전면수평철근1"));
	tvFrontSettleTmp = tvArrSide.GetTvByInfo(_T("날개벽전면정착장"));
	tvBackTmp = tvArrSide.GetTvByInfo(_T("날개벽배면수평철근1"));
	tvBackSettleTmp = tvArrSide.GetTvByInfo(_T("날개벽배면정착장"));
	tvWingEnd = tvArrSide.GetTvByInfo(_T("날개벽끝단"));

	//전면과 배면에서 정착장의 길이를 정해줘야 한다 근데 구부러진것을 길이를 따라나가면서 봐야 하기 때문에;;;
	
	if(nOut == 1)
	{
		CTwinVector tvMatch;
		CDPoint xyMatchSettle(0,0);

		dHoriDist = dL1+dL2 - dCoverWing;

		double dSettleWing = tvFrontSettleTmp.GetLength();
		long nDir = -1;
		if(bStt && bLeft)
			nDir = 1;
		if(!bStt && !bLeft)
			nDir = -1;

		tvFrontTmp.m_v1 = tvFrontTmp.m_v1 + (tvFrontTmp.GetXyDir(TRUE) * dSettleWing ) * nDir;

		long nOffsetDir = bLeft? 1 : -1;

		Offset(tvWingEnd.m_v1, tvWingEnd.m_v2, nOffsetDir * dHoriDist);

		GetXyMatchSegAndLine(tvFrontTmp.m_v1, tvFrontTmp.m_v2, tvFrontSettleTmp.m_v2, tvWingEnd.GetXyDir(TRUE), xyMatchSettle);


		tvMatch.m_v1 = tvWingEnd.m_v1;
		tvMatch.m_v2 = xyMatchSettle;

		dSettleDistWing = tvMatch.GetLength();
	}
	else
	{
		CTwinVector tvMatch;
		CDPoint xyMatchSettle(0,0);

		dHoriDist = dWingBackLen - dCoverWing;

		double dSettleWing = tvBackSettleTmp.GetLength();

		long nDir = -1;
		if(bStt && bLeft)
			nDir = 1;
		if(!bStt && !bLeft)
			nDir = -1;

		tvBackTmp.m_v1 = tvBackTmp.m_v1 + (tvBackTmp.GetXyDir(TRUE) * dSettleWing ) * nDir;

		long nOffsetDir = bLeft? 1 : -1;

		Offset(tvWingEnd.m_v1, tvWingEnd.m_v2, nOffsetDir * dHoriDist);

		GetXyMatchSegAndLine(tvBackTmp.m_v1, tvBackTmp.m_v2, tvBackSettleTmp.m_v2, tvWingEnd.GetXyDir(TRUE), xyMatchSettle);


		tvMatch.m_v1 = tvWingEnd.m_v2;
		tvMatch.m_v2 = xyMatchSettle;

		dSettleDistWing = tvMatch.GetLength();

	}
	//}

	return dSettleDistWing;
}

void CRcBridgeApp::SetDataDefaultBoxLowerSlab()
{
	m_dTS_Lower		= 1300;					// 하부슬래브 두께
	m_nTypeLower	= 0;					// 기초 형식 (직접기초,MASS기초,말뚝기초,잡석기초,부력방지 Anchor)
	m_dThickCobbStone	= 1000;				// 잡석기초 두께
	m_bSlopeLowerSlab	= FALSE;				// 하부슬래브 종단적용 (FALSE), TRUE - 선형중심기준
	// 종단 적용을 FALSE로 해줄때는 구조도 종단 옵션을 항상 경사로 해줘야 한다...
	m_nTypeJongdanForRebar = 1;

	double dLenAll = GetLengthBridge(TRUE) + GetThickWall(m_nQtyJigan, TRUE) + GetThickWall(m_nQtyJigan, TRUE);
	m_footingBox.m_dML = dLenAll/ 2.;
	m_footingBox.m_dML = dLenAll/ 2.;
	m_footingBox.m_dFWL = 0;
	m_footingBox.m_dFWR = 0;

	m_footingBox.SetDataDefaultShapeExFooting();
}

double CRcBridgeApp::GetElevationBoxBottom( long nJijum, double dDist, BOOL bOutSide/*=TRUE*/, BOOL bVerOpt )
{
	double dElB = m_dELSB;
	if(m_bSlopeLowerSlab == TRUE && (m_nTypeJongdanForRebar == 2))
	{
		double dStaBase = GetStationByTarget(BRIDGE_OFF_STT_SLAB, 0, FALSE);
		double dElBase = m_pLineApp->GetElevationPlanEl(dStaBase);
		double dSta = GetStationOnJijum(nJijum);
		if(bOutSide)
		{
			if(nJijum == 0)
				dSta = dStaBase;
			else if(nJijum == m_nQtyJigan)
				dSta = GetStationByTarget(BRIDGE_OFF_END_SLAB, 0, FALSE);
		}

		double dEl = m_pLineApp->GetElevationPlanEl(dSta + dDist);

		dElB += (dEl - dElBase);
	}
	else if(m_bSlopeLowerSlab == TRUE && m_nTypeJongdanForRebar == 1)
	{
		double dStaBase = GetStationByTarget(BRIDGE_OFF_STT_SLAB, 0, FALSE);
		double dSta = GetStationOnJijum(nJijum);
		if(bOutSide)
		{
			if(nJijum == 0)
				dSta = dStaBase;
			else if(nJijum == m_nQtyJigan)
				dSta = GetStationByTarget(BRIDGE_OFF_END_SLAB, 0, FALSE);
		}

		double dLen = (dSta - dStaBase) + dDist;
		CDPoint xyDir = CDPoint(GetXySectionSttEnd(FALSE)-GetXySectionSttEnd(TRUE)).Unit();

		dElB +=  dLen * tan(xyDir.GetAngleRadian());
	}
	
	return dElB;
}

CDPoint CRcBridgeApp::GetxyLowerSlabBottomByLenFrSttStation( double dDistFromStt, long nLeft/*=0*/, BOOL bVertDir/*=FALSE*/ )
{
	// 하부슬래브는 편경사가 적용되지 않으므로 EL은 중심선형위치에서구해줘야 한다.
	// nLeft가 0이 아닌경우는 Station만 이동 시켜주고 중심선형위치에서 구한다.
	
	CDPoint xy = GetXySlabUpperByLenFrSttStation(dDistFromStt, nLeft, bVertDir);

	if(m_bSlopeLowerSlab == TRUE)
	{
		if(nLeft != 0)
		{
			double dStaLeft = GetStationByCenterStation(m_dStationBridgeStt + dDistFromStt, nLeft);
			dDistFromStt = (dStaLeft - m_dStationBridgeStt);

			xy.y = GetXySlabUpperByLenFrSttStation(dDistFromStt, 0, bVertDir).y;
		}

		double dStaBase = GetStationByTarget(BRIDGE_OFF_STT_SLAB, 0, FALSE);
		double dElBase = m_pLineApp->GetElevationPlanEl(dStaBase);
		double dHeight = dElBase - m_dELSB - GetThickPave(FALSE);
		xy.y -= dHeight;
	}
	else
	{
		xy.y = m_dELSB;
	}
	
	return xy;
}

void CRcBridgeApp::SyncBridgeBoxType(BOOL bApplyAngle, long nTypeJongdan)
{
	if(m_nTypeBridge != BRIDGE_TYPE_BOX) return;

	if(nTypeJongdan < 0 || nTypeJongdan > 2)
		nTypeJongdan	= m_nTypeJongdanForRebar;

	// 기둥 하단 EL은 종단옵션에 따라 바꿔줄필요없다. 나중에 옮겨준다. 하지만 종단경사에따른 좌우 높이 보정은 없어야한다.
	if(m_bSlopeLowerSlab/* && nTypeJongdan != 0*/)
	{
		m_dELEB = GetElevationBoxBottom(m_nQtyJigan, 0, TRUE, FALSE);
	}
	else
	{
		m_dELEB = m_dELSB;
	}

	CExFootingApp *pExFooting	= &m_footingBox.m_exFooting;
	if(m_nTypeLower == BOXFOOTING_TYPE_PILE)
	{
		pExFooting->m_Pile.m_bIs = TRUE;
		pExFooting->m_nType = EXFOOTING_TYPE_PILE;
	}
	else if(m_nTypeLower == BOXFOOTING_TYPE_MASS)
	{
		pExFooting->m_Pile.m_bIs = FALSE;
		pExFooting->m_nType = EXFOOTING_TYPE_MASS;
	}
	else
	{
		pExFooting->m_Pile.m_bIs = FALSE;
		pExFooting->m_nType = EXFOOTING_TYPE_LEAN;
	}

	// 중간지점부 기둥 하단 EL.
	for(long ix=0; ix<GetCountInWall(); ix++)
	{
		CWallApp *pWall = GetInWall(ix);
		if(!pWall) continue;

		//BOOL bLeft = TRUE;
		long nIdxInWall = ix;
		long nIdxHunchL = GetIdxHunchInWall(nIdxInWall, TRUE);
		long nIdxHunchR = GetIdxHunchInWall(nIdxInWall, FALSE);
		// GetInfoHunch(nIdxHunch, nIdxInWall, bLeft);
		
		if(m_bSlopeLowerSlab/* && nTypeJongdan != 0*/)
		{
			CDPoint vAngWall		= GetAngleJijum(nIdxInWall+1, !bApplyAngle);
			// 하부슬래브가 경사인경우 좌우측 헌치높이를 구해 높은쪽으로
			double dElHunchLeft = GetElevationBoxBottom(nIdxInWall+1, -m_dWL[nIdxHunchL][iLOWER]/vAngWall.y, FALSE, !bApplyAngle) + m_dTS_Lower + m_dHHL[nIdxHunchL][iLOWER];
			double dElHunchRight = GetElevationBoxBottom(nIdxInWall+1, m_dWR[nIdxHunchL][iLOWER]/vAngWall.y, FALSE, !bApplyAngle) + m_dTS_Lower + m_dHHR[nIdxHunchR][iLOWER];

			if(nTypeJongdan != 0)
				pWall->m_dELB = max(dElHunchLeft, dElHunchRight) + m_dHBH[nIdxHunchL][iLOWER];
			else
				pWall->m_dELB = (dElHunchLeft + dElHunchRight) / 2. + m_dHBH[nIdxHunchL][iLOWER];
		}
		else
		{
			pWall->m_dELB = (m_dELSB + m_dTS_Lower + m_dHHL[nIdxHunchL][iLOWER] + m_dHBH[nIdxHunchL][iLOWER]);
		}
	}
}

CDPoint CRcBridgeApp::GetXyLowerSlabBottomByOffsetFrStation( double dSta, double dOffset, long nLeft )
{
	// 하부슬래브는 편경사가 적용되지 않으므로 EL은 중심선형위치에서구해줘야 한다
	// 지점각도에 따라 EL이 달라진다면 Station을 보정해야 겠지만 Station을 받기 때문에 괜찮을꺼 같기도 하고...
	// nLeft가 0이 아닌경우는 Station만 이동 시켜주고 중심선형위치에서 구한다.
	// 횡단 편경사를 적용안하면 같은 Station에서의 EL은 같다.
	
	// x위치는 상부슬래브와 같아야 되니깐 nLeft를 그대로 주고
	// 위에서 구한 해당 Station에서 선형중심에서의 높이를 벽체높이만큼 내려서 y값을주면 된다.

	CDPoint xy = GetXySlabUpperByOffsetFrStation(dSta, dOffset, nLeft);
	if(m_bSlopeLowerSlab == TRUE && m_nTypeJongdanForRebar != 0)
	{
		if(nLeft != 0)
		{
			double dStaLeft = GetStationByCenterStation(dSta, nLeft);
			dSta += (dStaLeft - dSta);

			xy.y = GetXySlabUpperByOffsetFrStation(dSta, dOffset, 0).y;
		}

		double dStaBase = GetStationByTarget(BRIDGE_OFF_STT_SLAB, 0, FALSE);
		double dElBase = m_pLineApp->GetElevationPlanEl(dStaBase);
		double dHeight = dElBase - m_dELSB - GetThickPave(FALSE);

		xy.y -= dHeight;
	}
	else
	{
		xy.y = m_dELSB;
	}

	return xy;
}

CDPoint CRcBridgeApp::GetXyToDistFromLowerSlab( double dStt, double dDist, long nLeft/*=0*/, double dOffset/*=-1*/, BOOL bVertDir/*=FALSE*/, BOOL bDistIsHor/*=TRUE*/ )
{
	dOffset = Compare(dOffset, -1.0, "=") ? m_dTS_Lower : dOffset;
	CDPoint xyOrg = GetXyMatchLineAndLowerSlab(CDPoint(dStt, 0), CDPoint(0, 1), nLeft, dOffset, bVertDir);
	CDPoint xyMatch(0, 0);
	CLineInfo *pLine	= GetLineLeftCenRight(0, bVertDir);
	
	if(m_bSlopeLowerSlab == FALSE)
	{
		// 하단 경사가 없기 때문에 dDist 값만큼 이동만 시켜주면 OK
		xyMatch = xyOrg;
		xyMatch.x += dDist;
	}
	else if(m_nTypeJongdanForRebar == 2 && pLine)
	{
		if(bDistIsHor)
		{
			xyMatch.x	= dStt+dDist;
			xyMatch.y	= GetxyLowerSlabBottomByLenFrSttStation(dStt+dDist-m_dStationBridgeStt, nLeft, FALSE).y;// pLine->GetElevationPlanEl(dStt+dDist);
			xyMatch.y	+= dOffset;
		}
		else
		{
			double dStaStt	= GetStationOnJijum(0, 0, FALSE);
			//CDPoint xyStt	= pLine->GetXyLine(dStaStt);
			//pLineCR->GetStationMatchVerticalByXy(xyStt, dStaStt);
			double dGiul	= pLine->GetJongDanGiul(dStt);
			double dRad		= atan(dGiul);
			CDPoint xyDir	= ToDPointFrRadian(dRad);
			CDPoint xyOrg	= GetxyLowerSlabBottomByLenFrSttStation(dStt-dStaStt, nLeft, FALSE);
			xyOrg	= xyOrg + (dDist * xyDir);
			xyMatch	= GetxyLowerSlabBottomByLenFrSttStation(xyOrg.x-dStaStt, nLeft, FALSE);
			xyMatch.y	+= dOffset;
		}
	}
	else
	{
		CDPoint xyStt = GetXySectionSttEnd(TRUE, nLeft, bVertDir);
		CDPoint xyEnd = GetXySectionSttEnd(FALSE, nLeft, bVertDir);
		CDPoint xyDir = (xyEnd-xyStt).Unit();

		if(bDistIsHor)
			GetXyMatchLineAndLine(xyOrg, xyDir, CDPoint(xyOrg.x+dDist, xyOrg.y), CDPoint(0, 1), xyMatch);
		else
			xyMatch = xyOrg + (dDist * xyDir);
	}

	return xyMatch;
}

CDPoint CRcBridgeApp::GetXyMatchLineAndLowerSlab( CDPoint xy, CDPoint xyDir, long nLeft/*=0*/, double dOffset/*=-1*/, BOOL bVertDir/*=FALSE*/ )
{
	// 하부슬래브는 편경사가 적용되지 않으므로 EL은 중심선형위치에서구해줘야 한다.
	dOffset = Compare(dOffset, -1.0, "=") ? m_dTS_Lower : dOffset;

	// nLeft가 0이 아닌경우는 Station만 이동 시켜주고 중심선형위치에서 구한다.
	CDPoint xyRet = GetXyMatchLineAndSlabUpper(xy, xyDir, nLeft, 0, bVertDir);

	if(m_bSlopeLowerSlab == TRUE)
	{
		if(nLeft != 0)
		{
			double dStaLeft = GetStationByCenterStation(xy.x, nLeft);
			xy.x += (dStaLeft - xy.x);

			xyRet.y = GetXyMatchLineAndSlabUpper(xy, xyDir, 0, 0, bVertDir).y;
		}

		double dStaBase = GetStationByTarget(BRIDGE_OFF_STT_SLAB, 0, FALSE);
		double dElBase = m_pLineApp->GetElevationPlanEl(dStaBase);
		double dHeight = dElBase - m_dELSB - GetThickPave(FALSE);;
		xyRet.y -= (dHeight - dOffset);
	}
	else
	{
		xyRet.y = (m_dELSB + dOffset);
	}

	return xyRet;
}

// 상단 중심 기준
// BOOL bSide : 측면 기준으로 그리는지?
// BOOL bLeftSide : 측면 기준일때 좌측 기준인지?
// BOOL bForBM : 수량 산출용이면 확대기초 중심을 기준으로 구함(bSide일때)
// BOOL bMaxLengthForLeftSide : 측면도 그릴때 최대 길이로 그림
void CRcBridgeApp::GetTvBoxLowerExFootFront(long nLeft, BOOL bApplyAngle)
{
	CExFootingApp *pExFoot = &m_footingBox.m_exFooting;
	pExFoot->m_tvArrFront.RemoveAll();

	CTwinVector tv;
	CDPoint xyStt, xyEnd, xyDirStt, xyDirEnd;

	CTwinVectorArray tvArrFront, tvArrLower;
	m_tvArrVertSection.GetTvArrByInfo(_T("하부슬래브하면"), tvArrLower, FALSE, FALSE);

	if(m_bExistProtectionRiseFooting)
	{
		tv = m_tvArrVertSection.GetTvByInfo(_T("시점부상방지저판하단"));
		xyStt = tv.m_v2;
		xyDirStt = tv.GetXyDir();

		tv = m_tvArrVertSection.GetTvByInfo(_T("종점부상방지저판하단"));
		xyEnd = tv.m_v2;
		xyDirEnd = tv.GetXyDir();
	}
	else
	{
		tv = tvArrLower.GetTvByInfo(_T("하부슬래브하면(시점벽체아래)"));
		xyStt = tv.m_v1;
		xyDirStt = tv.GetXyDir(TRUE);

		tv = tvArrLower.GetTvByInfo(_T("하부슬래브하면(종점벽체아래)"));
		xyEnd = tv.m_v2;
		xyDirEnd = tv.GetXyDir();
	}

	// 종단선형따라 그릴지? 직선으로 그릴지?
	BOOL bDrawJongdanLine	= (nLeft == 0) && m_nTypeJongdanForRebar == 2;

	double dAngleStt	= bApplyAngle? GetAngleJijum(0).GetAngleDegree() : 90;
	double dAngleEnd	= bApplyAngle? GetAngleJijum(m_nQtyJigan).GetAngleDegree() : 90;
	double radStt		= sin(ToRadian(dAngleStt));
	double radEnd		= sin(ToRadian(dAngleEnd));

	double dEFWR	= pExFoot->m_dEFWR;
	double dEFWL	= pExFoot->m_dEFWL;
	
	double dWidth	= m_footingBox.GetWidth() + dEFWL/radStt + dEFWR/radEnd;

	pExFoot->m_tvArrFront.m_sInfo = NOTRANS("확대기초");	

	if(m_nTypeLower == LOWERSLAB_TYPE_LEAN || 
		m_nTypeLower == LOWERSLAB_TYPE_PILE || 
		m_nTypeLower == LOWERSLAB_TYPE_COBBSTONE || 
		m_nTypeLower == LOWERSLAB_TYPE_ANCHOR || 
		(!pExFoot->m_bFrontDanMass))
	{
		tv.m_bApplyAngle	= bApplyAngle;
		tv.m_dAngle			= 90;
		tv.m_v1				= xyStt;
		tv.m_v2				= xyEnd;
		tv.m_v1.z			= (m_bSlopeLowerSlab && bDrawJongdanLine) ? 5 : 0;
		tv.m_sInfo			= NOTRANS("확대기초상단");
		tv.val.valHor		= NULL;
		tv.val.valVer		= NULL;
		pExFoot->m_tvArrFront.Add(tv);

		tv.m_bApplyAngle	= bApplyAngle;
		tv.m_dAngle			= dAngleEnd;
		tv.m_v1				= tv.m_v2;
		tv.m_v2				= tv.m_v1 + (fabs(dEFWR/xyDirEnd.x)/radEnd * xyDirEnd);
		tv.m_sInfo			= NOTRANS("확대기초상단우측면");
		tv.val.valHor		= NULL;
		tv.val.valVer		= NULL;
		pExFoot->m_tvArrFront.Add(tv);

		tv.m_bApplyAngle	= bApplyAngle;
		tv.m_dAngle			= 90;
		tv.m_v1				= tv.m_v2;
		tv.m_v2.y			-= pExFoot->m_dEFHR;
		tv.m_sInfo			= NOTRANS("확대기초우측면");
		tv.val.valHor		= &pExFoot->m_dEFWR;
		tv.val.valVer		= NULL;
		tv.val.bRevMoveHor	= FALSE;
		pExFoot->m_tvArrFront.Add(tv);

		// 좌측 하면 위치 
		CDPoint xyPos = xyStt + (fabs(dEFWL/xyDirStt.x)/radStt * xyDirStt);
		xyPos.y -= pExFoot->m_dEFHR;

		tv.m_bApplyAngle	= bApplyAngle;
		tv.m_dAngle			= 90;
		tv.m_v1				= tv.m_v2;
		tv.m_v1.z			= (m_bSlopeLowerSlab && bDrawJongdanLine) ? 5 : 0;
		tv.m_v2				= xyPos;
		tv.m_sInfo			= NOTRANS("확대기초하면");
		tv.val.valHor		= NULL;
		tv.val.valVer		= &pExFoot->m_dEFHR;
		tv.val.bRevMoveVer	= TRUE;
		pExFoot->m_tvArrFront.Add(tv);

		tv.m_bApplyAngle	= bApplyAngle;
		tv.m_dAngle			= 90;
		tv.m_v1				= tv.m_v2;
		tv.m_v2.y			+= pExFoot->m_dEFHR;
		tv.m_sInfo			= NOTRANS("확대기초좌측면");
		tv.val.valHor		= &pExFoot->m_dEFWL;
		tv.val.valVer		= &pExFoot->m_dEFHR;
		tv.val.bRevMoveHor	= TRUE;
		pExFoot->m_tvArrFront.Add(tv);

		tv.m_bApplyAngle	= bApplyAngle;
		tv.m_dAngle			= dAngleStt;
		tv.m_v1				= tv.m_v2;
		tv.m_v2				+= (fabs(dEFWL/xyDirStt.x)/radStt * -xyDirStt);
		tv.m_sInfo			= NOTRANS("확대기초상단좌측면");
		tv.val.valHor		= NULL;
		tv.val.valVer		= NULL;
		pExFoot->m_tvArrFront.Add(tv);
	}
	else if(m_nTypeLower == LOWERSLAB_TYPE_MASS)
	{
		if(pExFoot->m_bLeftOrgOfMass)
		{
			tv.m_bApplyAngle	= bApplyAngle;
			tv.m_dAngle			= 90;
			tv.m_v1				= xyEnd;
			tv.m_v2				= xyStt;
			tv.m_v1.z			= (m_bSlopeLowerSlab && bDrawJongdanLine) ? 5 : 0;
			tv.m_sInfo			= NOTRANS("확대기초상단");
			tv.val.valHor		= NULL;
			tv.val.valVer		= NULL;
			pExFoot->m_tvArrFront.Add(tv);

			tv.m_bApplyAngle	= bApplyAngle;
			tv.m_dAngle			= dAngleEnd;
			tv.m_v1				= tv.m_v2;
			tv.m_v2				= tv.m_v1 + (fabs(dEFWL/xyDirStt.x) * xyDirStt);
			tv.m_sInfo			= NOTRANS("확대기초상단좌측면");
			tv.val.valHor		= NULL;
			tv.val.valVer		= NULL;
			pExFoot->m_tvArrFront.Add(tv);

			tv.m_bApplyAngle	= bApplyAngle;
			tv.m_dAngle			= 90;
			tv.m_v1				= tv.m_v2;
			tv.m_v2.y			-= pExFoot->m_dEFHR;
			tv.m_sInfo			= NOTRANS("확대기초좌측면");
			tv.val.valHor		= &pExFoot->m_dEFWR;
			tv.val.valVer		= NULL;
			tv.val.bRevMoveHor	= TRUE;
			pExFoot->m_tvArrFront.Add(tv);

			double dWidthDan = 0;
			if(pExFoot->m_nCountDan >= MAX_MASS_DAN) pExFoot->m_nCountDan = MAX_MASS_DAN-1;
			CString str;
			for(long i = 0; i < pExFoot->m_nCountDan; i++)
			{
				str.Format(NOTRANS("%d번째단"), i+1);
				tv.m_bApplyAngle	= bApplyAngle;
				tv.m_dAngle			= 90;
				tv.m_v1				= tv.m_v2;
				tv.m_v2.x			+= pExFoot->m_dEFW[i];
				if(i == 0)
					tv.m_v2.x += dEFWL/radStt;
				tv.m_sInfo			= NOTRANS("확대기초")+str+NOTRANS("하면");
				tv.val.valHor		= &pExFoot->m_dEFW[i];
				tv.val.valVer		= &pExFoot->m_dEFHR;
				tv.val.bRevMoveHor	= FALSE;
				tv.val.bRevMoveVer	= TRUE;
				pExFoot->m_tvArrFront.Add(tv);
				dWidthDan += pExFoot->m_dEFW[i];

				tv.m_bApplyAngle	= bApplyAngle;
				tv.m_dAngle			= 90;
				tv.m_v1				= tv.m_v2;
				tv.m_v2.x			+= pExFoot->m_dEFS[i];
				tv.m_v2.y			+= pExFoot->m_dEFH[i];
				tv.m_sInfo			= NOTRANS("확대기초")+str+NOTRANS("경사");
				tv.val.valHor		= &pExFoot->m_dEFS[i];
				tv.val.valVer		= &pExFoot->m_dEFH[i];
				tv.val.bRevMoveHor	= FALSE;
				tv.val.bRevMoveVer	= FALSE;
				pExFoot->m_tvArrFront.Add(tv);
				dWidthDan += pExFoot->m_dEFS[i];
			}

			tv.m_bApplyAngle	= bApplyAngle;
			tv.m_dAngle			= dAngleEnd;
			tv.m_v1				= tv.m_v2;
			if(pExFoot->m_nCountDan > 0)
				tv.m_v2.x			+= (dWidth-dWidthDan-dEFWL/radEnd);
			else
				tv.m_v2.x			+= (dWidth-dWidthDan);
			tv.m_sInfo			= NOTRANS("확대기초우측하면");
			tv.val.valHor		= NULL;
			tv.val.valVer		= &pExFoot->m_dEFHR;
			tv.val.bRevMoveVer	= TRUE;
			pExFoot->m_tvArrFront.Add(tv);

			tv.m_bApplyAngle	= bApplyAngle;
			tv.m_dAngle			= 90;
			tv.m_v1				= tv.m_v2;
			tv.m_v2				= xyEnd + (fabs(dEFWR/xyDirEnd.x)/radEnd * xyDirEnd);
			tv.m_sInfo			= NOTRANS("확대기초우측면");
			tv.val.valHor		= &pExFoot->m_dEFWR;
			tv.val.valVer		= NULL;
			tv.val.bRevMoveHor	= FALSE;
			pExFoot->m_tvArrFront.Add(tv);

			tv.m_bApplyAngle	= bApplyAngle;
			tv.m_dAngle			= dAngleEnd;
			tv.m_v1				= tv.m_v2;
			tv.m_v2				= xyEnd;
			tv.m_sInfo			= NOTRANS("확대기초상단우측면");
			tv.val.valHor		= NULL;
			tv.val.valVer		= NULL;
			pExFoot->m_tvArrFront.Add(tv);
		}
		else
		{
			tv.m_bApplyAngle	= bApplyAngle;
			tv.m_dAngle			= 90;
			tv.m_v1				= xyStt;
			tv.m_v2				= xyEnd;
			tv.m_v1.z			= (m_bSlopeLowerSlab && bDrawJongdanLine) ? 5 : 0;
			tv.m_sInfo			= NOTRANS("확대기초상단");
			tv.val.valHor		= NULL;
			tv.val.valVer		= NULL;
			pExFoot->m_tvArrFront.Add(tv);

			tv.m_bApplyAngle	= bApplyAngle;
			tv.m_dAngle			= dAngleEnd;
			tv.m_v1				= tv.m_v2;
			tv.m_v2				= tv.m_v1 + (fabs(dEFWR/xyDirEnd.x)/radEnd * xyDirEnd);
			tv.m_sInfo			= NOTRANS("확대기초상단우측면");
			tv.val.valHor		= NULL;
			tv.val.valVer		= NULL;
			pExFoot->m_tvArrFront.Add(tv);

			tv.m_bApplyAngle	= bApplyAngle;
			tv.m_dAngle			= 90;
			tv.m_v1				= tv.m_v2;
			tv.m_v2.y			-= pExFoot->m_dEFHR;
			tv.m_sInfo			= NOTRANS("확대기초우측면");
			tv.val.valHor		= &pExFoot->m_dEFWR;
			tv.val.valVer		= NULL;
			tv.val.bRevMoveHor	= FALSE;
			pExFoot->m_tvArrFront.Add(tv);

			double dWidthDan = 0;
			if(pExFoot->m_nCountDan >= MAX_MASS_DAN) pExFoot->m_nCountDan = MAX_MASS_DAN-1;
			CString str = _T("");
			for(long i = 0; i < pExFoot->m_nCountDan; i++)
			{
				str.Format(NOTRANS("%d번째단"), i+1);
				tv.m_bApplyAngle	= bApplyAngle;
				tv.m_dAngle			= 90;
				tv.m_v1				= tv.m_v2;
				tv.m_v2.x			-= pExFoot->m_dEFW[i];
				if(i == 0)
					tv.m_v2.x -= dEFWR/radEnd;
				tv.m_sInfo			= NOTRANS("확대기초")+str+NOTRANS("하면");
				tv.val.valHor		= &pExFoot->m_dEFW[i];
				tv.val.valVer		= &pExFoot->m_dEFHR;
				tv.val.bRevMoveHor	= TRUE;
				tv.val.bRevMoveVer	= TRUE;
				pExFoot->m_tvArrFront.Add(tv);
				dWidthDan += pExFoot->m_dEFW[i];

				tv.m_bApplyAngle	= bApplyAngle;
				tv.m_dAngle			= 90;
				tv.m_v1				= tv.m_v2;
				tv.m_v2.x			-= pExFoot->m_dEFS[i];
				tv.m_v2.y			+= pExFoot->m_dEFH[i];
				tv.m_sInfo			= NOTRANS("확대기초")+str+NOTRANS("경사");
				tv.val.valHor		= &pExFoot->m_dEFS[i];
				tv.val.valVer		= &pExFoot->m_dEFH[i];
				tv.val.bRevMoveHor	= TRUE;
				tv.val.bRevMoveVer	= FALSE;
				pExFoot->m_tvArrFront.Add(tv);
				dWidthDan += pExFoot->m_dEFS[i];
			}

			tv.m_bApplyAngle	= bApplyAngle;
			tv.m_dAngle			= dAngleStt;
			tv.m_v1				= tv.m_v2;
			if(pExFoot->m_nCountDan > 0)
				tv.m_v2.x			-= (dWidth-dWidthDan-dEFWR/radEnd);
			else
				tv.m_v2.x			-= (dWidth);
			tv.m_sInfo			= NOTRANS("확대기초좌측하면");
			tv.val.valHor		= NULL;
			tv.val.valVer		= &pExFoot->m_dEFHR;
			tv.val.bRevMoveVer	= TRUE;
			pExFoot->m_tvArrFront.Add(tv);

			tv.m_bApplyAngle	= bApplyAngle;
			tv.m_dAngle			= 90;
			tv.m_v1				= tv.m_v2;
			tv.m_v2				= xyStt + (fabs(dEFWL/xyDirStt.x)/radStt * xyDirStt);
			tv.m_sInfo			= NOTRANS("확대기초좌측면");
			tv.val.valHor		= &pExFoot->m_dEFWL;
			tv.val.valVer		= NULL;
			tv.val.bRevMoveHor	= TRUE;
			pExFoot->m_tvArrFront.Add(tv);

			tv.m_bApplyAngle	= bApplyAngle;
			tv.m_dAngle			= dAngleStt;
			tv.m_v1				= tv.m_v2;
			tv.m_v2				= xyStt;
			tv.m_sInfo			= NOTRANS("확대기초상단좌측면");
			tv.val.valHor		= NULL;
			tv.val.valVer		= NULL;
			pExFoot->m_tvArrFront.Add(tv);
		}
	}
	return;
}

void CRcBridgeApp::GetTvBoxLowerExFootPlan()
{
	CExFootingApp *pExFoot = &m_footingBox.m_exFooting;
	pExFoot->m_tvArrPlan.RemoveAll();

	if(m_footingBox.m_exFooting.m_bIs == FALSE) return;

	double dWStt = pExFoot->m_dEFWL;
	double dWEnd = pExFoot->m_dEFWR;
	double dWLeft = pExFoot->m_dEFWB;
	double dWRight = pExFoot->m_dEFWF;

	if(m_bExistProtectionRiseFooting)
	{
		dWStt += m_dPRF_W;
		dWEnd += m_dPRF_W;
	}

	CTwinVectorArray tvArrPlane;
	GetTvRebarOut_Plane_UpperSlabStd(tvArrPlane, -1, TRUE);

	CTwinVectorArray tvArrLeft, tvArrRight;
	CTwinVector tv;
	long i = 0; for(i = 0; i < tvArrPlane.GetSize(); i++)
	{
		tv	= tvArrPlane.GetAt(i);
		if(tv.m_sInfo.Find("좌측", 0) != -1) 
			tvArrLeft.Add(tv);
		else if(tv.m_sInfo.Find("우측", 0) != -1) 
			tvArrRight.Add(tv);
	}
	if(tvArrLeft.GetCount() == 0 || tvArrRight.GetCount() == 0) return;

	// Sort 되면서 방향이 바뀔수도 있기때문에 Sort 되기전의 첫번째 위치를 저장하자.
	CDPoint A[4];
	GetXySss(BRIDGE_OFF_STT_SLAB,A);
	CDPoint xyLeft = A[0];
	CDPoint xyRight = A[3]; 

	tvArrLeft.Sort();
	tvArrRight.Sort();

	CTwinVector tvStt, tvEnd;
	tvArrLeft.GetFirst(tv);
	tvStt.m_v1 = tv.m_v1;
	tvStt.m_v1.z = 0;
	tvArrRight.GetFirst(tv);
	tvStt.m_v2 = tv.m_v1;
	tvStt.m_v2.z = 0;

	tvArrLeft.GetLast(tv);
	tvEnd.m_v2 = tv.m_v2;
	tvEnd.m_v2.z = 0;
	tvArrRight.GetLast(tv);
	tvEnd.m_v1 = tv.m_v2;
	tvEnd.m_v1.z = 0;

	CRebarPlacing rb;
	if(~(tvStt.m_v1 - xyLeft) > ~(tvEnd.m_v2 - xyLeft))
	{
		swap(tvStt.m_v1, tvEnd.m_v2);
		rb.ReverseRebar(tvArrLeft);
	}
	if(~(tvStt.m_v2 - xyRight) > ~(tvEnd.m_v1 - xyRight))
	{
		swap(tvStt.m_v2, tvEnd.m_v1);
		rb.ReverseRebar(tvArrRight);
	}

	// 좌우측 끝단에 수평인 선을 추가해준다.
	CDPoint xyDirStt = GetAngleAzimuthTanOnJijum(0, 0);
	CDPoint xyDirEnd = GetAngleAzimuthTanOnJijum(m_nQtyJigan, 0);
	if(dWStt > 0)
	{
		tv.m_v2 = tvStt.m_v1;
		tv.m_v1 = tvStt.m_v1 - dWStt * xyDirStt;
		tvArrLeft.InsertAt(0, tv);
		tv.m_v2 = tvStt.m_v2;
		tv.m_v1 = tvStt.m_v2 - dWStt * xyDirStt;
		tvArrRight.InsertAt(0, tv);
	}
	if(dWEnd > 0)
	{
		tv.m_v1 = tvEnd.m_v2;
		tv.m_v2 = tvEnd.m_v2 + dWEnd * xyDirEnd;
		tvArrLeft.Add(tv);
		tv.m_v1 = tvEnd.m_v1;
		tv.m_v2 = tvEnd.m_v1 + dWEnd * xyDirEnd;
		tvArrRight.Add(tv);
	}

	rb.RemoveMergeParallelTwinVector(tvArrLeft);
	rb.RemoveMergeParallelTwinVector(tvArrRight);

	rb.GetOffsetTvArray(tvArrLeft, -dWLeft, -dWLeft, TRUE);
	rb.GetOffsetTvArray(tvArrRight, dWRight, dWRight, TRUE);
	rb.Offset(tvStt, dWStt, dWStt);
	rb.Offset(tvEnd, dWEnd, dWEnd);

	// 시/종, 좌/우의 선이 만나는 위치까지 연장해준다.
	tvArrLeft.GetFirst(tv);
	if(tv.m_v1.z == 0)
	{
		GetXyMatchLineAndLine(tvStt.m_v1, tvStt.GetXyDir(), tv.m_v1, tv.GetXyDir(), tvStt.m_v1);
		tv.m_v1 = tvStt.m_v1;
		tvArrLeft.SetAt(0, tv);
	}
	else
	{
		CDPoint xyDir = GetAngleAzimuthTanOnJijum(0, 0);
		GetXyMatchLineAndLine(tvStt.m_v1, tvStt.GetXyDir(), tv.m_v1, xyDir, tvStt.m_v1);
		tv.m_v2 = tv.m_v1;
		tv.m_v1 = tvStt.m_v1;
		tvArrLeft.InsertAt(0, tv);
	}

	tvArrRight.GetFirst(tv);
	if(tv.m_v1.z == 0)
	{
		GetXyMatchLineAndLine(tvStt.m_v1, tvStt.GetXyDir(), tv.m_v1, tv.GetXyDir(), tvStt.m_v2);
		tv.m_v1 = tvStt.m_v2;
		tvArrRight.SetAt(0, tv);
	}
	else
	{
		CDPoint xyDir = GetAngleAzimuthTanOnJijum(0, 0);
		GetXyMatchLineAndLine(tvStt.m_v1, tvStt.GetXyDir(), tv.m_v1, xyDir, tvStt.m_v2);
		tv.m_v2 = tv.m_v1;
		tv.m_v1 = tvStt.m_v2;
		tvArrRight.InsertAt(0, tv);
	}

	tvArrLeft.GetLast(tv);
	if(tv.m_v1.z == 0)
	{
		GetXyMatchLineAndLine(tvEnd.m_v1, tvEnd.GetXyDir(), tv.m_v1, tv.GetXyDir(), tvEnd.m_v2);
		tv.m_v2 = tvEnd.m_v2;
		tvArrLeft.SetAt(tvArrLeft.GetSize()-1, tv);
	}
	else
	{
		CDPoint xyDir = GetAngleAzimuthTanOnJijum(m_nQtyJigan, 0);
		GetXyMatchLineAndLine(tvEnd.m_v1, tvEnd.GetXyDir(), tv.m_v1, xyDir, tvEnd.m_v2);
		tv.m_v1 = tv.m_v2;
		tv.m_v2 = tvEnd.m_v2;
		tvArrLeft.Add(tv);
	}

	tvArrRight.GetLast(tv);
	if(tv.m_v1.z == 0)
	{
		GetXyMatchLineAndLine(tvEnd.m_v1, tvEnd.GetXyDir(), tv.m_v1, tv.GetXyDir(), tvEnd.m_v1);
		tv.m_v2 = tvEnd.m_v1;
		tvArrRight.SetAt(tvArrRight.GetSize()-1, tv);
	}
	else
	{
		CDPoint xyDir = GetAngleAzimuthTanOnJijum(m_nQtyJigan, 0);
		GetXyMatchLineAndLine(tvEnd.m_v1, tvEnd.GetXyDir(), tv.m_v1, xyDir, tvEnd.m_v1);
		tv.m_v1 = tv.m_v2;
		tv.m_v2 = tvEnd.m_v1;
		tvArrRight.Add(tv);
	}

	rb.ReverseRebar(tvArrLeft);
	pExFoot->m_tvArrPlan.Add(tvStt);
	pExFoot->m_tvArrPlan.AddFromTvArray(tvArrRight);
	pExFoot->m_tvArrPlan.Add(tvEnd);
	pExFoot->m_tvArrPlan.AddFromTvArray(tvArrLeft);

	return;
}

// 어스앵커 권고안
void CRcBridgeApp::SetDataDefaultEarthAnchor(CFootingApp *pFooting)
{
	if(pFooting == NULL || IsBoxType() == FALSE) return;
	if(m_nTypeLower != BOXFOOTING_TYPE_ANCHOR) return;

	CLineInfo *pLine	= GetLineBase();

	double dStation = GetStationBridgeStt(FALSE);
	double dWidth = m_dWidthSlabRight - m_dWidthSlabLeft;
	double dStationStt = pLine->GetStationByOffsetWidth(dStation, -m_dWS, GetAngleJijum(0));
	double dStationEnd = pLine->GetStationByOffsetWidth(dStation + m_dLengthBridge, +m_dWE, GetAngleJijum(GetCountJijum() -1));
//	double dDistStd = (m_dWidthSlabLeft + m_dWidthSlabRight) / 2.;
	double dDistStd = 0;
	double dLength = pLine->GetLengthByDis(dStationStt, dStationEnd, dDistStd);

	double dCtcJong	= 2500;
	double dCtcSide	= 2500;
	double dYunDanJong = 1000;
	double dYunDanSide = 1000;
	long nCountJong = (long)((dLength - dYunDanJong * 2) / dCtcJong) + 1;
	long nCountSide = (long)((dWidth - dYunDanSide * 2) / dCtcSide) + 1;
	
	dYunDanJong = (dLength - (nCountJong - 1) * dCtcJong) / 2.;
	dYunDanSide = (dWidth - (nCountSide - 1) * dCtcSide) / 2.;

	//////////////////////////////////////////////////////////////////////////
	m_dArrDistJongAnchor.RemoveAll();
	m_dArrLoadAnchor.RemoveAll();
	m_dArrLengthAnchor.RemoveAll();
	m_dArrDiaAnchor.RemoveAll();
	m_dArrEaAnchor.RemoveAll();
	for(long ix = 0; ix < RCBRIDGE_ANCHOR_MAX; ix++)
	{
		m_dArrDistHDanAnchor[ix].RemoveAll();
	}

	m_bAnchorSameValue = TRUE;
	m_nCountAnchor = nCountJong;

	double dAnchorLoad = 1000.0;
	double dAnchorLength = 8000.0;
	double dAnchorDia = 100.0;
	for(long ix = 0; ix < m_nCountAnchor; ix++)
	{
		m_dArrDistJongAnchor.Add(ix == 0 ? dYunDanJong : dCtcJong);
		m_dArrLoadAnchor.Add(dAnchorLoad);
		m_dArrLengthAnchor.Add(dAnchorLength);
		m_dArrDiaAnchor.Add(dAnchorDia);
		m_dArrEaAnchor.Add(nCountSide);

		for(long kk = 0; kk < nCountSide; kk++)
		{
			m_dArrDistHDanAnchor[ix].Add(kk == 0 ? dYunDanSide : dCtcSide);
		}
	}
}

// 말뚝배치 권고안
void CRcBridgeApp::SetDataDefaultPilePos(CFootingApp *pFooting)
{
	if(pFooting == NULL) return;
	if(IsBoxType() == FALSE)
	{
		if(pFooting->m_exFooting.m_nType == EXFOOTING_TYPE_PILE)
		{
			CExPileApp *pPile = &pFooting->m_exFooting.m_Pile;
			pPile->SetDataDefaultPos();
		}
		return;
	}

	CLineInfo *pLine	= GetLineBase();

	// 함형라멘교
	CExPileApp *pPile = &pFooting->m_exFooting.m_Pile;
	if(pPile == NULL) return;

	// 함형라멘교 말뚝배치 권고안
	// 1. 박스중심의 길이를 기준으로 배치
	// 2. 각도는 시점과 종점의 각도로 변해가도록 배치
	CDPoint vAng(0,1);
	double dStation = GetStationBridgeStt(FALSE);
	double dWidth = m_dWidthSlabRight - m_dWidthSlabLeft;
	double dStationStt = pLine->GetStationByOffsetWidth(dStation, -m_dWS, GetAngleJijum(0));
	double dStationEnd = pLine->GetStationByOffsetWidth(dStation + m_dLengthBridge, +m_dWE, GetAngleJijum(GetCountJijum() -1));
//	double dDistStd = (m_dWidthSlabLeft + m_dWidthSlabRight) / 2.;
	double dDistStd = 0;
	double dLength = pLine->GetLengthByDis(dStationStt, dStationEnd, dDistStd);

	double dWidthBase = 0;
	if(IsExtLine())
	{
		// 확폭일때 오류 수정. 확폭일때는 중앙 Sta를 기준으로 폭을 잡고 말뚝 종단 Station 마다 폭을 구해서 간격을 보정해준다.
		double dStationMid = dStation + GetLengthBridge(FALSE) / 2;
		CDPoint xyAng = GetAngleByStationForMesh(dStationMid);
		dWidth = GetWidthSlabAct(dStationMid, CDPoint(0, 1), 0);
		dWidthBase = GetWidthSlabAct(dStationMid, xyAng, 0);
	}

	double dCtcOrg	= 1500;
	double dCtcSide	= 1500;
	double dYundan = Round(MAX(650, 1.25 * pPile->m_dDia), 0);

	pPile->m_bIndvPileInput = FALSE;

	long nCountJong = (long)((dLength - dYundan - dYundan) / dCtcOrg) + 1;
	long nCountSide = (long)((dWidth - dYundan - dYundan) / dCtcSide) + 1;

	// 종방향 배치
	pPile->m_nCountOrg = nCountJong;
	AhTPAMake(nCountJong, &pPile->m_pArrOrgInfo,  (COrgInfo*)0);

	for(long ix = 0; ix < pPile->m_nCountOrg; ix++)
	{
		COrgInfo *pOrg =pPile-> m_pArrOrgInfo.GetAt(ix);
		if(pOrg == NULL) continue;

		if(ix == 0) 
			pOrg->dist = (dLength - (dCtcOrg * (pPile->m_nCountOrg - 1))) / 2;
		else 
			pOrg->dist = dCtcOrg;

		pOrg->countSide = nCountSide;
	}
	pPile->KeepOrgInfo();

	double dMargin = (dWidth - (dCtcSide * (nCountSide - 1))) / 2;
	// 횡방향 배치
	for(long ix = 0; ix < pPile->m_nCountOrg; ix++)
	{
		COrgInfo *pOrg =pPile-> m_pArrOrgInfo.GetAt(ix);
		if(pOrg == NULL) continue;

		double dCtcPile = dCtcSide;
		if(IsExtLine())
		{
			dStation += pOrg->dist;
			CDPoint xyAng = GetAngleByStationForMesh(dStation);
			double dWidthCur = GetWidthSlabAct(dStation, xyAng, 0);
			double dRatio = (dWidthCur - (dMargin/xyAng.y)*2) / (dWidthBase - (dMargin/xyAng.y)*2);
			dCtcPile = dCtcSide * dRatio;
		}

		for(long kk = 0; kk < nCountSide; kk++)
		{
			CPileAttr *pPileAttr = pPile->GetPileAttr(ix, kk);
			if(pPileAttr == NULL) 
			{
				pPileAttr = new CPileAttr;
				pPileAttr->nIdxOrg	= ix;
				pPileAttr->nIdxSide	= kk;
				pPile->m_pArrPile.Add(pPileAttr);
			}

			if(kk == 0) 
				pPileAttr->dist = dMargin;
			else 
				pPileAttr->dist = dCtcPile;

			pPileAttr->ang = 0;
			pPileAttr->len = 10000;
		}
	}

	SetXyPilePos(pFooting);

	pPile->KeepOrgInfo();
	pPile->KeepPile();
}

CDPoint CRcBridgeApp::GetXyPilePos(CFootingApp *pFooting, long nIndexOrg, long nIndexSide)
{
	CDPoint xyPile(0,0);
	if(pFooting == NULL || IsBoxType() == FALSE) return xyPile;

	BOOL bEarthAnchor = (IsBoxType() && m_nTypeLower == BOXFOOTING_TYPE_ANCHOR) ? TRUE : FALSE;

	CLineInfo *pLine	= GetLineBase();

	// 함형라멘교
	CExPileApp *pPile = &pFooting->m_exFooting.m_Pile;
	if(bEarthAnchor == FALSE && pPile == NULL) return xyPile;

	long nCountOrg = bEarthAnchor ? m_nCountAnchor : pPile->m_nCountOrg;
	long nCountSide = GetCountSidePileOrAnchor(pFooting, nIndexOrg, bEarthAnchor);
	if(nIndexOrg >= nCountOrg || nIndexSide >= nCountSide) return xyPile;

	double dStationPile = GetStationPileOrAnchor(pFooting, nIndexOrg, bEarthAnchor);

	// 지간내에 있는 말뚝을 설정
	//long nSpanPile = 0;
	for(long nSpan = 0; nSpan < m_nQtyJigan; nSpan++)
	{
		double dStation1 = GetStationOnJijum(nSpan);
		double dStation2 = GetStationOnJijum(nSpan + 1);
		if(nSpan == 0)
		{
			dStation1 = pLine->GetStationByOffsetWidth(dStation1, -m_dWS, GetAngleJijum(0));
		}
		if(nSpan == m_nQtyJigan - 1)
		{
			dStation2 = pLine->GetStationByOffsetWidth(dStation2, +m_dWE, GetAngleJijum(m_nQtyJigan));
		}
		if(Compare(dStationPile, dStation1, _T(">=")) && Compare(dStationPile, dStation2, _T("<=")))
		{
			//nSpanPile = nSpan;
			break;
		}
	}
//	long nIdxPileStt = GetIndexPileOrAnchorSttAtSpan(pFooting, nSpanPile, bEarthAnchor);
//	long nIdxPileEnd = GetIndexPileOrAnchorEndAtSpan(pFooting, nSpanPile, bEarthAnchor);
//	double dStationPileStt = GetStationPileOrAnchor(pFooting, nIdxPileStt, bEarthAnchor);
//	double dStationPileEnd = GetStationPileOrAnchor(pFooting, nIdxPileEnd, bEarthAnchor);
	// vAng
	CDPoint vAngPile = GetAngleByStationForMesh(dStationPile);
// 	CDPoint vAngPile(0,0);
// 	double dDegreeStt = GetAngleJijum(nSpanPile).GetAngleDegree();
// 	double dDegreeEnd = GetAngleJijum(nSpanPile + 1).GetAngleDegree();
// 	if(nIndexOrg == nIdxPileStt)
// 	{
// 		vAngPile = ToDPointFrDegree(dDegreeStt);
// 	}
// 	else if(nIndexOrg == nIdxPileEnd)
// 	{
// 		vAngPile = ToDPointFrDegree(dDegreeEnd);
// 	}
// 	else
// 	{
// 		vAngPile = ToDPointFrDegree(dDegreeStt + (dDegreeEnd - dDegreeStt) * (dStationPile - dStationPileStt) / (dStationPileEnd - dStationPileStt));
// 	}
	// dDistPile
	double dDistPile = GetDisSlabRightAct(dStationPile, CDPoint(0,1));
	for(long ix = 0; ix <= nIndexSide; ix++)
	{
		if(bEarthAnchor == FALSE)
		{
			CPileAttr *pPileAttr = pPile->GetPileAttr(nIndexOrg, ix);
			if(pPileAttr == NULL) continue;
			dDistPile -= pPileAttr->dist;
		}
		else
		{
			dDistPile -= m_dArrDistHDanAnchor[nIndexOrg].GetAt(ix);
		}
	}

	xyPile = pLine->GetXyLineDisByAng(dStationPile, dDistPile, vAngPile);

	return xyPile;
}

long CRcBridgeApp::GetIndexPileOrAnchorSttAtSpan(CFootingApp *pFooting, long nSpan, BOOL bEarthAnchor)
{
	if(nSpan < 0 || nSpan >= m_nQtyJigan) return -1;

	CLineInfo *pLine	= GetLineBase();
	CExPileApp *pPile = &pFooting->m_exFooting.m_Pile;

	long nCountOrg = bEarthAnchor ? m_nCountAnchor : pPile->m_nCountOrg;

	double dStationSpanStt = GetStationOnJijum(nSpan);
	double dStationSpanEnd = GetStationOnJijum(nSpan + 1);
	if(nSpan == 0)
	{
		dStationSpanStt = pLine->GetStationByOffsetWidth(dStationSpanStt, -m_dWS, GetAngleJijum(0));
	}
	if(nSpan == m_nQtyJigan - 1)
	{
		dStationSpanEnd = pLine->GetStationByOffsetWidth(dStationSpanEnd, +m_dWE, GetAngleJijum(m_nQtyJigan));
	}
	for(long ix = 0; ix < nCountOrg; ix++)
	{
		double dStationCheck = GetStationPileOrAnchor(pFooting, ix, bEarthAnchor);
		if(Compare(dStationCheck, dStationSpanStt, _T(">=")) && Compare(dStationCheck, dStationSpanEnd, _T("<")))
		{
			return ix;
		}
	}

	return -1;
}

long CRcBridgeApp::GetIndexPileOrAnchorEndAtSpan(CFootingApp *pFooting, long nSpan, BOOL bEarthAnchor)
{
	if(nSpan < 0 || nSpan >= m_nQtyJigan) return -1;

	CLineInfo *pLine	= GetLineBase();
	CExPileApp *pPile = &pFooting->m_exFooting.m_Pile;

	long nCountOrg = bEarthAnchor ? m_nCountAnchor : pPile->m_nCountOrg;

	long nIdxPileEnd = -1;
	double dStationSpanStt = GetStationOnJijum(nSpan);
	double dStationSpanEnd = GetStationOnJijum(nSpan + 1);
	if(nSpan == 0)
	{
		dStationSpanStt = pLine->GetStationByOffsetWidth(dStationSpanStt, -m_dWS, GetAngleJijum(0));
	}
	if(nSpan == m_nQtyJigan - 1)
	{
		dStationSpanEnd = pLine->GetStationByOffsetWidth(dStationSpanEnd, +m_dWE, GetAngleJijum(m_nQtyJigan));
	}
	
	for(long ix = 0; ix < nCountOrg; ix++)
	{
		double dStationCheck = GetStationPileOrAnchor(pFooting, ix, bEarthAnchor);
		if(Compare(dStationCheck, dStationSpanStt, _T(">")) && Compare(dStationCheck, dStationSpanEnd, _T("<=")))
		{
			nIdxPileEnd = ix;
		}
	}

	return nIdxPileEnd;
}


double CRcBridgeApp::GetStationPileOrAnchor(CFootingApp *pFooting, long nIndex, BOOL bAnchor)
{
	CLineInfo *pLine	= GetLineBase();

	double dStation = GetStationBridgeStt(FALSE);
	double dStationStt = pLine->GetStationByOffsetWidth(dStation, -m_dWS, GetAngleJijum(0));

	double dDistSum = 0;
	if(bAnchor == FALSE)
	{
		CExPileApp *pPile = &pFooting->m_exFooting.m_Pile;

		if(nIndex >= pPile->m_nCountOrg) return 0;

		for(long ix = 0; ix <= nIndex; ix++)
		{
			COrgInfo *pOrg = pPile-> m_pArrOrgInfo.GetAt(ix);
			if(pOrg == NULL) continue;
			dDistSum += pOrg->dist;
		}
	}
	else
	{
		if(nIndex >= m_nCountAnchor) return 0;

		for(long ix = 0; ix <= nIndex; ix++)
		{
			dDistSum += m_dArrDistJongAnchor.GetAt(ix);
		}
	}

	return dStationStt + dDistSum;
}

long CRcBridgeApp::GetCountSidePileOrAnchor(CFootingApp *pFooting, long nIndex, BOOL bAnchor)
{
	if(bAnchor == FALSE)
	{
		CExPileApp *pPile = &pFooting->m_exFooting.m_Pile;
		if(nIndex >= pPile->m_nCountOrg) return 0;

		COrgInfo *pOrg = pPile-> m_pArrOrgInfo.GetAt(nIndex);
		if(pOrg == NULL) return 0;

		return pOrg->countSide;
	}
	else
	{
		if(nIndex >= m_nCountAnchor) return 0;

		return (long)(m_dArrEaAnchor.GetAt(nIndex));
	}
}

// 말뚝위치 설정
void CRcBridgeApp::SetXyPilePos(CFootingApp *pFooting)
{
	if(pFooting == NULL || IsBoxType() == FALSE) return;

	// 함형라멘교
	CExPileApp *pPile = &pFooting->m_exFooting.m_Pile;
	if(pPile == NULL) return;

	for(long ix = 0; ix < pPile->m_nCountOrg; ix++)
	{
		COrgInfo *pOrg =pPile-> m_pArrOrgInfo.GetAt(ix);
		if(pOrg == NULL) continue;
		for(long kk = 0; kk < pOrg->countSide; kk++)
		{
			CPileAttr *pPileAttr = pPile->GetPileAttr(ix, kk);
			if(pPileAttr == NULL) continue;

			pPileAttr->xy = GetXyPilePos(pFooting, ix, kk);
		}
	}
}


// 기초모델링 권고안
void CRcBridgeApp::SetDataDefaultFootingModeling()
{
	m_nGroundFactorKV = 2;
	m_dGroundFactorKV = 0.67;
	m_nGroundFactorBv = 0;
	m_nEquivalentApplyKsb = 0;
	m_dEquivalentApplyKsb = 3;
	m_n2DGeneralSpringType = 0;
	m_n2DEQSpringType = 0;
	m_nVerticalFactor = 2;
	m_dSpringSpaceBottom = 1000;
	m_dSpringSpaceWall = 1000;
	m_bSpringSpaceIncludeWallTick = TRUE;
}

// nLeft = -1, nLeft = 1(우), nLeft = 0(모두)
void CRcBridgeApp::GetTvBoxLowerExFootSide( double dStation , CDPoint vAng, long nLeft)
{
	CLineInfoApp *pLine = m_pLineApp;
	if(!pLine) return;

	CExFootingApp *pExFoot = &m_footingBox.m_exFooting;
	
	CTwinVectorArray *pTvArr	= &pExFoot->m_tvArrSide;
	pTvArr->RemoveAll();

	// Center를 중심으로 좌우 넓이를 구한후 선형에서 구해준다.
	double dDistLeft = GetDisSlabLeftActByGagak(dStation, vAng);
	double dDistRight = GetDisSlabRighActByGagak(dStation, vAng);
//	double dEl_Plan = pLine->GetElevationPlanEl(dStation);
//	double dEL_Foot = dEl_Plan - GetThickPave() - GetHeightBridgeBox(dStation); // 버림콘크리트 상면
	// 선형에서 바로 구하면 종단 타입 선택시에 위치를 잘못구한다.
	double dEL_Foot = GetXyLowerSlabBottomByOffsetFrStation(dStation, 0, 0).y;

	CTwinVector tv;
	tv.m_v1 = pLine->GetXyLineByLengthAndAngle(dStation, dDistLeft, vAng);
	tv.m_v2 = pLine->GetXyLineByLengthAndAngle(dStation, dDistRight, vAng);
	pExFoot->m_tvPathPlan = tv;

	double dH = pExFoot->m_dEFHR;
	double dEFWR	= pExFoot->m_dEFWF;
	double dEFWL	= pExFoot->m_dEFWB;

	CDPoint xyLeft(dDistLeft, dEL_Foot), xyRight(dDistRight, dEL_Foot), vX(1,0), vY(0,1);
	
	pTvArr->m_sInfo = NOTRANS("확대기초");	
	
	if(nLeft == 0 || nLeft == 1)
	{
		// 우측상면, 우측 측면
		tv.m_sInfo	= NOTRANS("확대기초상단우측마진");
		tv.m_bApplyAngle	= FALSE;
		tv.m_v1	= (xyLeft + xyRight) / 2.;
		tv.m_v2	= xyRight;
		pTvArr->Add(tv);

		tv.m_sInfo = NOTRANS("확대기초상단우측면");
		tv.m_v1	= xyRight;
		tv.m_v2	= xyRight + dEFWR * vX;
		pTvArr->Add(tv);

		tv.m_sInfo = NOTRANS("확대기초우측면");
		tv.m_v1 = tv.m_v2;
		tv.m_v2 = tv.m_v1	- dH * vY;
		pTvArr->Add(tv);

		// 우측하면
		tv.m_sInfo = NOTRANS("확대기초상단우측면");
		tv.m_v1	= xyRight + dEFWR * vX - dH * vY;
		tv.m_v2	= xyRight - dH * vY;
		pTvArr->Add(tv);

		tv.m_sInfo	= NOTRANS("확대기초상단우측마진");
		tv.m_v1	= xyRight - dH * vY;
		tv.m_v2	= (xyLeft + xyRight) / 2. - dH * vY;
		pTvArr->Add(tv);
	}

	if(nLeft == 0 || nLeft == -1)
	{
		// 좌측하면
		tv.m_sInfo	= NOTRANS("확대기초상단좌측마진");
		tv.m_v1	= (xyLeft + xyRight) / 2. - dH * vY;
		tv.m_v2	= xyLeft - dH * vY;
		pTvArr->Add(tv);

		tv.m_sInfo = NOTRANS("확대기초상단좌측면");
		tv.m_v1	= xyLeft - dH * vY;
		tv.m_v2	= xyLeft - dEFWL * vX - dH * vY;
		pTvArr->Add(tv);

		// 좌측상면, 좌측 측면
		tv.m_sInfo = NOTRANS("확대기초좌측면");
		tv.m_v1 = xyLeft - dEFWL * vX - dH * vY;
		tv.m_v2 = xyLeft - dEFWL * vX;
		pTvArr->Add(tv);

		tv.m_sInfo = NOTRANS("확대기초상단좌측면");
		tv.m_v1	= xyLeft - dEFWL * vX;
		tv.m_v2	= xyLeft;
		pTvArr->Add(tv);

		tv.m_sInfo	= NOTRANS("확대기초상단좌측마진");
		tv.m_v1	= xyLeft;
		tv.m_v2	= (xyLeft + xyRight) / 2.;
		pTvArr->Add(tv);
	}
}

double CRcBridgeApp::GetHeightBridgeBox( double dStation )
{
	double dHeight = 0;

	if(m_bSlopeLowerSlab == TRUE)
	{
		// 하부가 종단경사를 따라간다면 
		double dStaBase = GetStationByTarget(BRIDGE_OFF_STT_SLAB, 0, FALSE);
		double dElBase = m_pLineApp->GetElevationPlanEl(dStaBase);
		dHeight = dElBase - m_dELSB - GetThickPave(FALSE);
	}
	else
	{
		double dElBase = m_pLineApp->GetElevationPlanEl(dStation);
		dHeight = dElBase - m_dELSB - GetThickPave(FALSE);
	}

	return dHeight;
}

double CRcBridgeApp::GetElevationFootingLower(double dStation, BOOL bUseSection)
{
	if(IsBoxType())
	{
		double dEl_Plan = m_pLineApp->GetElevationPlanEl(dStation);
		double dEL_Foot = dEl_Plan - GetThickPave() - GetHeightBridgeBox(dStation); // 버림콘크리트 상면

		return dEL_Foot;
	}
	else
	{
		long nJijum = GetJijumNumberByStation(dStation);
		CFootingApp* pFoot = GetFootingByJijumNumber(nJijum);
		if(!pFoot) return 0.0;

		// (ARCBRIDGE-3039) 기초 정면도에서 EL을 가져오면 계산과정에서 종단경사 수평으로도 되어있을때는 EL이 잘못 나온다.
		return bUseSection? pFoot->GetELFootingLower() : (pFoot->m_dEL - pFoot->GetHeight());
	}
}

CDPoint CRcBridgeApp::GetAngleByStationForMesh(double dStation, double dDist, BOOL bVert)
{
	CLineInfo *pLine = GetLineBase();
	CDPoint vAngSta = CDPoint(0,1);

	long nJijum1   = 0;
	long nJijum2   = 0;
	long nQtyJijum = GetCountJijum();

	long nJ=0; for(nJ=0; nJ<nQtyJijum; nJ++)
	{
		double dStaJijum = GetStationOnJijum(nJ);
		if(dDist != 0)
		{
			dStaJijum = pLine->GetStationByScInLine(dStaJijum, dDist, GetAngleJijum(nJ));
		}

		if(dStaJijum==dStation) return bVert? GetAngleJijum(nJ) : GetAngleAzimuthJijum(nJ, TRUE);
		if(dStaJijum>dStation) 
		{
			nJijum1 = nJ - 1;
			nJijum2 = nJ;
			break;
		}
	}
	if(nJ==nQtyJijum) nJijum1 = nJijum2 = nQtyJijum-1;
	if(nJijum1<0) nJijum1 = 0;
	if(nJijum2>=nQtyJijum) nJijum2 = nQtyJijum-1;
	if(nJijum1==nJijum2) return bVert? GetAngleJijum(nJijum1) : GetAngleAzimuthJijum(nJijum1, TRUE);

	double dStation1 = GetStationOnJijum(nJijum1);
	double dStation2 = GetStationOnJijum(nJijum2);
	double dRadAng1  = bVert? GetAngleJijum(nJijum1).GetAngleRadian() : GetAngleAzimuthJijum(nJijum1, FALSE).GetAngleRadian();
	double dRadAng2  = bVert? GetAngleJijum(nJijum2).GetAngleRadian() : GetAngleAzimuthJijum(nJijum2, FALSE).GetAngleRadian();

	if(dDist != 0)
	{
		dStation1 = pLine->GetStationByScInLine(dStation1, dDist, GetAngleJijum(nJijum1));
		dStation2 = pLine->GetStationByScInLine(dStation2, dDist, GetAngleJijum(nJijum2));
	}

	double dRadSta   = dRadAng1 + (dRadAng2-dRadAng1)*(dStation-dStation1)/(dStation2-dStation1);

	vAngSta = CDPoint(cos(dRadSta),sin(dRadSta));

	return vAngSta;
}

void CRcBridgeApp::GetTvCrossSectionLowerSlab( CTwinVectorArray& tvArr, double sta, CDPoint vAng /*= CDPoint(0, 1)*/, long nDrawLeft/*=0*/, BOOL bMirror/*=FALSE*/, BOOL bExp/*=FALSE*/ )
{
	if(!IsExp(sta))
		bExp	= FALSE;

	tvArr.RemoveAll();

	CTwinVector tv;

	double dWidth = bExp ? GetWidthSlabActByGagak(sta, vAng, 0) : GetWidthSlabAct(sta, vAng,0,FALSE,FALSE,FALSE);
	double disSlabLeft	= bExp ? GetDisSlabLeftActByGagak(sta, vAng, FALSE) : GetDisSlabLeftAct(sta, vAng, FALSE);
	double dElBottom = GetXyLowerSlabBottomByOffsetFrStation(sta, 0, 0).y;

	tv.m_v1 = CDPoint(disSlabLeft, dElBottom + m_dTS_Lower);
	tv.m_v2	= tv.m_v1 + CVector(dWidth, 0, 0);
	tv.m_sInfo = _T("하부슬래브횡단상면");
	tvArr.Add(tv);

	tv.m_v1 = tv.m_v2;
	tv.m_v2.y -= m_dTS_Lower;
	tv.m_sInfo = _T("하부슬래브횡단우측면");
	tvArr.Add(tv);

	tv.m_v1 = tv.m_v2;
	tv.m_v1.x -= dWidth;
	tv.m_sInfo = _T("하부슬래브횡단하면");
	tvArr.Add(tv);

	tv.m_v2 = tv.m_v1;
	tv.m_v1.y += m_dTS_Lower;
	tv.m_sInfo = _T("하부슬래브횡단좌측면");
	tvArr.Add(tv);

	// mirror함
	CRebarPlacing rb;
	if(bMirror)
	{
		CDRect rect	= tvArr.GetRect();
		rb.Mirror(tvArr, CDPoint(rect.left, 0), TRUE);
		rb.MoveRebar(tvArr, CDPoint(rect.Width(), 0));
	}

	// nDrawLeft에 따라서 잘라냄
	if(nDrawLeft != 0)
	{
		BOOL bVert	= Compare(vAng.GetAngleDegree(), 90.0, "=");
		CDRect rect	= tvArr.GetRect();
		double dMid	= GetMidCrossSection(bVert);
		BOOL bLeft	= nDrawLeft == -1 ? FALSE : TRUE;
		CDPoint xy1	= CDPoint(dMid, rect.bottom-100);
		CDPoint xy2	= CDPoint(dMid, rect.top+100);
		rb.TrimTvArrayByLine(tvArr, xy1, xy2, bLeft, TRUE, FALSE);
	}
}

void CRcBridgeApp::GetTvCrossLowerHunch( CTwinVectorArray& tvArr, double sta, CDPoint vAng, BOOL bDrawSlopPattern, long nDrawLeft, BOOL bMirror/*=FALSE*/, BOOL bExp/*=FALSE*/, CDPoint vAngWall/*=CDPoint(0, 1)*/, BOOL bCalHeightHunch/*=TRUE*/, double dHeightHunch/*=0*/ )
{
	if(!IsExp(sta))
		bExp	= FALSE;

	CTwinVector tv;

	//	double dTH	= bCalHeightHunch ? GetHeightUpperSlab_OldOrNotUse(sta, vAngWall) - m_dTS : dHeightHunch;
	double dTH	= bCalHeightHunch ? GetHeightLowerSlab(GetJijumNumberByStation(sta), 0, vAngWall) - m_dTS_Lower : dHeightHunch;
	if(dTH <= 0) return;
	int nTerm	= bDrawSlopPattern ? 4 : 1;
	double dSpace = dTH/nTerm;
	
	CTwinVectorArray tvArrLower;
	GetTvCrossSectionLowerSlab(tvArrLower, sta, vAng, 0, FALSE, bExp);
	CTwinVector tvBase = tvArrLower.GetTvByInfo(_T("하부슬래브횡단상면"));

	tv = tvBase;
	for(int n=1; n<nTerm+1; n++)
	{
		tv.m_v1.y += dSpace*n;
		tv.m_v2.y += dSpace*n;
		tv.m_sInfo = _T("헌치상면");
		tvArr.Add(tv);
	}

	tv.m_v1 = tv.m_v2 = tvBase.m_v1;
	tv.m_v2.y += dSpace*nTerm;
	tv.m_sInfo	= "헌치좌측";
	tvArr.Add(tv);	// 좌측단
	
	tv.m_v1 = tv.m_v2 = tvBase.m_v2;
	tv.m_v2.y += dSpace*nTerm;
	tv.m_sInfo	= "헌치우측";
	tvArr.Add(tv);	// 우측단

	// mirror함
	CRebarPlacing rb;
	if(bMirror)
	{
		CTwinVectorArray tvArrCross;
		GetTvCrossSection(tvArrCross, sta, vAng, FALSE, TRUE, FALSE, 0, FALSE);
		CDRect rect	= tvArrCross.GetRect();
		rb.Mirror(tvArr, CDPoint(rect.left, 0), TRUE);
		rb.MoveRebar(tvArr, CDPoint(rect.Width(), 0));
	}

	// nDrawLeft에 따라서 잘라냄
	if(nDrawLeft != 0)
	{
		BOOL bVert	= Compare(vAng.GetAngleDegree(), 90.0, "=");
		CRebarPlacing rb;
		CDRect rect	= tvArr.GetRect();
		double dMid	= GetMidCrossSection(bVert);
		BOOL bLeft	= nDrawLeft == -1 ? FALSE : TRUE;
		CDPoint xy1	= CDPoint(dMid, rect.bottom-100);
		CDPoint xy2	= CDPoint(dMid, rect.top+100);
		rb.TrimTvArrayByLine(tvArr, xy1, xy2, bLeft, TRUE, FALSE);
	}
}

CDPoint CRcBridgeApp::GetXyMatchLineAndArchRib( BOOL bUpper, CDPoint xy, CDPoint xyDir, long nLeft, double dOffset, BOOL bVertDir/*=FALSE*/ )
{
	CDPoint xyMatch(0, 0);

	CRebarPlacing rb;
	long nIndex = bUpper? iUPPER : iLOWER;

	CTwinVectorArray tvArr;
	tvArr = m_tvArrVertSectionRib[nIndex];

	if(Compare(dOffset, 0.0, _T("="), 0.1) == FALSE)
	{
		rb.GetOffsetTvArray(tvArr, bUpper? dOffset : - dOffset);
	}

	CDPointArray xyArrMatch;
	rb.GetXyMatchTvArrAndLine(tvArr, xy, xyDir, xyArrMatch);
	
	if(xyArrMatch.GetCount() > 0)
	{
		xyMatch = xyArrMatch.GetAt(0);
	}

	return xyMatch;
}

long CRcBridgeApp::GetCountSubsidence()
{
	long nCountSubsidence = 0;
	if(IsBoxType() == FALSE)
	{
		BOOL bLsd = m_pARcBridgeDataStd->IsLsdDesign();
		BOOL bApplyLoad = bLsd? m_plsdManager->IsSelectedLoadDesign(eLSD_LOADSELECT_SD) : m_bApplyLoad[BRIDGE_LOAD_SUBSID];

		BOOL bApplyType = (m_nTypeBridge==BRIDGE_TYPE_TRAP2 || m_nTypeBridge==BRIDGE_TYPE_ARCH) ? TRUE : FALSE;
		if(bApplyLoad && (m_nSelSubsideType==0 || !bApplyType))
		{
			if(m_nSelect2DFrameTool != SOLVE_MIDAS)
			{
				long nQtyJijum = IsArchType() ? 4 : GetCountJijum();
				// 각지점별로 1개씩 땡기고...
				long nJijum=0; for(nJijum=0; nJijum<nQtyJijum; nJijum++)
				{
					nCountSubsidence++;
				}
				// 2개씩 땡기고
				if(nQtyJijum>2)
				{
					long nJijum=0; for(nJijum=0; nJijum<nQtyJijum; nJijum++)
					{
						long i=0; for(i=nJijum+1; i<nQtyJijum; i++)
						{
							nCountSubsidence++;
						}
					}
				}
				// 3개씩 땡기고 : 앞3,뒤3,앞2+뒤1,앞1+뒤2
				if(nQtyJijum>3)
				{
					// 시점에서 3개
					nCountSubsidence++;
					// 종점에서 3개
					nCountSubsidence++;
					// 시점2개, 종점1개
					nCountSubsidence++;
					// 시점1개, 종점2개
					nCountSubsidence++;
					if(nQtyJijum>4)
					{
						// 시점3개 (P1부터)
						nCountSubsidence++;
						if(nQtyJijum>5)
						{
							// 종점3개 (A2이전까지)
							nCountSubsidence++;
						}
					}
				}
				if(nQtyJijum>5)
				{
					// 시점4개
					nCountSubsidence++;
					// 종점4개
					nCountSubsidence++;
				}
				// 5경간이상은 무시.............
			}
		}
		else if(bApplyLoad)
		{
			if(m_nSelect2DFrameTool!=SOLVE_MIDAS)
			{
				long nQtyJijum = IsArchType() ? 4 : GetCountJijum();
				long nJijum=0; for(nJijum=0; nJijum<nQtyJijum; nJijum++)
				{
					if(nJijum!=0 && nJijum!=nQtyJijum-2) continue;
					nCountSubsidence++;		
				}
			}
		}
	}

	return nCountSubsidence;
}

//Box형 교량에서 토공 구체공제를 위해서..
//바깥라인만 남겨둠..
void CRcBridgeApp::GetTvArrVertSectionOutLine(CTwinVectorArray &tvArrResult)
{
 	CTwinVector tv;
 	
 	long nSize = m_tvArrVertSection.GetSize();
 	if(nSize <= 0 || !IsBoxType()) return;
 
 	for(long ix = 0; ix < nSize; ix++)
 	{
 		tv = m_tvArrVertSection.GetAt(ix);
 		
		if(	tv.m_sInfo.Find(_T("헌치")) != -1
			|| tv.m_sInfo == _T("벽체좌측면")
			|| tv.m_sInfo == _T("벽체우측면")
			|| tv.m_sInfo == _T("시점벽체우측면")
			|| tv.m_sInfo == _T("종점벽체좌측면")
			|| tv.m_sInfo == _T("하부슬래브상면")
			|| tv.m_sInfo == _T("슬래브하면")
			)
		{
			if(tv.m_sInfo == _T("시점브라켓헌치") || tv.m_sInfo == _T("종점브라켓헌치"))
				tvArrResult.Add(tv);
			else
				continue;
		}
		else
			tvArrResult.Add(tv);
 	}
}

CStmModel* CRcBridgeApp::GetStmModelByKey( long iKey, HGBOOL bOutput )
{
	CStmModel *pModel = NULL;

	map<int, CStmModelForArcbridge*>::iterator iter = bOutput==TRUE ? m_mStmSolverOut.find(iKey) : m_mStmSolver.find(iKey);
	if( bOutput==TRUE ? (iter != m_mStmSolverOut.end()) : (iter != m_mStmSolver.end()) )
		pModel = iter->second;

	return pModel;
}

HGINT32 CRcBridgeApp::GetStmKeyByStmModel( CStmModel* pStm )
{
	HGINT32 ixKey(-1);
	map<int, CStmModelForArcbridge*>::iterator iter;
	for(iter = m_mStmSolver.begin(); iter != m_mStmSolver.end(); ++iter)
	{
		if(iter->second == pStm)
		{
			ixKey = iter->first;
			break;
		}
	}

	return ixKey;
}

CStmModel* CRcBridgeApp::GetStmModelBracket( BOOL bStt )
{
	HGINT32 iKey = GetKeyStmModel(eStmBracket, bStt);

	return 	GetStmModelByKey(iKey);
}

CStmModel* CRcBridgeApp::GetStmModelFoot( BOOL bStt, long iCase, long iIndex )
{
	HGINT32 iKey = GetKeyStmModel(eStmFoot, bStt, FALSE, iIndex, iCase);

	return 	GetStmModelByKey(iKey);
}

CStmModel* CRcBridgeApp::GetStmModelMidFoot( HGINT32 iInnerJ, long iCase )
{
	HGINT32 iKey = GetKeyStmModel(eStmMidFoot, FALSE, FALSE, iInnerJ-1, iCase);

	return 	GetStmModelByKey(iKey);
}

// CStmModel* CRcBridgeApp::GetStmModelCorner( BOOL bStt, BOOL bUpper, BOOL bPlus, long iExtreme )
// {
// 	// LSD - index로 극한한계 / 극단상황 1 구분
// 	HGINT32 iKey = GetKeyStmModel(eStmCorner, bStt, bUpper, iExtreme, bPlus? 0:1);
// 	
// 	return 	GetStmModelByKey(iKey);
// }

HGINT32 CRcBridgeApp::GetKeyStmModel( eStmModelPos ePos, BOOL bStt/*=TRUE*/, BOOL bUpper/*=TRUE*/, long iIndex/*=0*/, long iCase/*=0*/, long iMidType/*=0*/ )
{
	HGBOOL bLSD = m_pARcBridgeDataStd->IsLsdDesign();

	HGINT32 iKey = 0;

	switch (ePos)
	{
	case eStmBracket:
		iKey = bStt? STM_MODEL_BRACKET_STT : STM_MODEL_BRACKET_END;
		break;
	case eStmCorner:
		{
			if( bLSD && iIndex == 1)		// 한계상태 && 극단상황 1
				iKey = STM_MODEL_CORNER_STT_UPPER_MINUS_EXT_1;
			else if( bLSD && iIndex == 2 ) // 한계상태 && 극단상황 2
				iKey = STM_MODEL_CORNER_STT_UPPER_MINUS_EXT_2;
			else
				iKey = STM_MODEL_CORNER_STT_UPPER_MINUS;
			if(bStt == FALSE) iKey += 1;
			if(bUpper == FALSE) iKey += 2;
			if(iCase == 1) iKey += 4;
		}
		break;
	case eStmFoot:
		if( bLSD )
			iKey = (bStt? STM_MODEL_FOOT_STT_MAX_AXIAL : STM_MODEL_FOOT_END_MAX_AXIAL) + (iIndex * 5) + iCase;
		else
			iKey = (bStt? STM_MODEL_FOOT_STT_MAXP : STM_MODEL_FOOT_END_MAXP) + iCase;
		break;
	case eStmMidFoot:
		if( bLSD )
			iKey = STM_MODEL_FOOT_MID_MAX_AXIAL + (iMidType * 5) + (iIndex * 15) + iCase;
		else
			iKey = STM_MODEL_MIDFOOT_HEAD + (iIndex * 4) + iCase;
		break;
	default:
		break;
	}

	return iKey;
}

CString CRcBridgeApp::GetStringStmModelNameByKey( HGINT32 iKey, long iDepth/*=0*/ )
{
	CString sDepth1 = _T("");
	CString sDepth2 = _T("");
	CString sDepth3 = _T("");
	CString sDepth_Add = _T("");
	CString sDepth_Add2 = _T("");

	CString szArrPos[4] = { _T("시점상단"), _T("종점상단"), _T("시점하단"), _T("종점하단") };
	CString szArrCase[4] = { _T("Pmax"), _T("Mmax"), _T("Mmin"), _T("지진시") };
	CString szArrCaseLsd[5] = { _T("축력최대"), _T("부모멘트최대"), _T("정모멘트최대"), _T("수평력최대"), _T("수평력최소") };
	
	HGBOOL bLsd = m_pARcBridgeDataStd->IsLsdDesign();
	if(bLsd)
	{
		sDepth_Add = _T("극한한계");
	}

	switch (iKey)
	{
	case STM_MODEL_BRACKET_STT:
	case STM_MODEL_BRACKET_END:
		sDepth1 = (iKey == STM_MODEL_BRACKET_STT)? _T("시점") : _T("종점");
		sDepth2 = _T("브라켓");
		break;
	case STM_MODEL_CORNER_STT_UPPER_PLUS:
	case STM_MODEL_CORNER_END_UPPER_PLUS:
	case STM_MODEL_CORNER_STT_LOWER_PLUS:
	case STM_MODEL_CORNER_END_LOWER_PLUS:
	case STM_MODEL_CORNER_STT_UPPER_PLUS_EXT_1:
	case STM_MODEL_CORNER_END_UPPER_PLUS_EXT_1:
	case STM_MODEL_CORNER_STT_LOWER_PLUS_EXT_1:
	case STM_MODEL_CORNER_END_LOWER_PLUS_EXT_1:
	case STM_MODEL_CORNER_STT_UPPER_PLUS_EXT_2:
	case STM_MODEL_CORNER_END_UPPER_PLUS_EXT_2:
	case STM_MODEL_CORNER_STT_LOWER_PLUS_EXT_2:
	case STM_MODEL_CORNER_END_LOWER_PLUS_EXT_2:
		if( STM_MODEL_CORNER_STT_UPPER_PLUS_EXT_1 <= iKey && iKey <= STM_MODEL_CORNER_END_LOWER_PLUS_EXT_1 )
		{
			iKey -= (STM_MODEL_CORNER_STT_UPPER_PLUS_EXT_1 - STM_MODEL_CORNER_STT_UPPER_PLUS);		// 극단상황도 같이 사용하기 위해 키값을 변경해준다
			sDepth_Add = _T("극단상황Ⅰ");
		}
		else if( STM_MODEL_CORNER_STT_UPPER_PLUS_EXT_2 <= iKey && iKey <= STM_MODEL_CORNER_END_LOWER_PLUS_EXT_2 )
		{
			iKey -= (STM_MODEL_CORNER_STT_UPPER_PLUS_EXT_2 - STM_MODEL_CORNER_STT_UPPER_PLUS);		// 극단상황도 같이 사용하기 위해 키값을 변경해준다
			sDepth_Add = _T("극단상황Ⅱ");
		}
		sDepth1 = szArrPos[iKey - STM_MODEL_CORNER_STT_UPPER_PLUS];
		sDepth2 = _T("우각부");
		sDepth3 = _T("Open");
		break;
	case STM_MODEL_CORNER_STT_UPPER_MINUS:
	case STM_MODEL_CORNER_END_UPPER_MINUS:
	case STM_MODEL_CORNER_STT_LOWER_MINUS:
	case STM_MODEL_CORNER_END_LOWER_MINUS:
	case STM_MODEL_CORNER_STT_UPPER_MINUS_EXT_1:
	case STM_MODEL_CORNER_END_UPPER_MINUS_EXT_1:
	case STM_MODEL_CORNER_STT_LOWER_MINUS_EXT_1:
	case STM_MODEL_CORNER_END_LOWER_MINUS_EXT_1:
	case STM_MODEL_CORNER_STT_UPPER_MINUS_EXT_2:
	case STM_MODEL_CORNER_END_UPPER_MINUS_EXT_2:
	case STM_MODEL_CORNER_STT_LOWER_MINUS_EXT_2:
	case STM_MODEL_CORNER_END_LOWER_MINUS_EXT_2:
		if( STM_MODEL_CORNER_STT_UPPER_MINUS_EXT_1 <= iKey && iKey <= STM_MODEL_CORNER_END_LOWER_MINUS_EXT_1 )
		{
			iKey -= (STM_MODEL_CORNER_STT_UPPER_MINUS_EXT_1 - STM_MODEL_CORNER_STT_UPPER_MINUS);		// 극단상황도 같이 사용하기 위해 키값을 변경해준다.
			sDepth_Add = _T("극단상황Ⅰ");
		}
		else if( STM_MODEL_CORNER_STT_UPPER_MINUS_EXT_2 <= iKey && iKey <= STM_MODEL_CORNER_END_LOWER_MINUS_EXT_2 )
		{
			iKey -= (STM_MODEL_CORNER_STT_UPPER_MINUS_EXT_2 - STM_MODEL_CORNER_STT_UPPER_MINUS);		// 극단상황도 같이 사용하기 위해 키값을 변경해준다
			sDepth_Add = _T("극단상황Ⅱ");
		}
		sDepth1 = szArrPos[iKey - STM_MODEL_CORNER_STT_UPPER_MINUS];
		sDepth2 = _T("우각부");
		sDepth3 = _T("Close");
		break;
	case STM_MODEL_FOOT_STT_MAXP:
	case STM_MODEL_FOOT_STT_MAXM:
	case STM_MODEL_FOOT_STT_MINM:
	case STM_MODEL_FOOT_STT_EARTH:
	case	STM_MODEL_FOOT_STT_MAX_AXIAL:				// Lsd
	case	STM_MODEL_FOOT_STT_MAX_NEG_MOMENT:
	case	STM_MODEL_FOOT_STT_MAX_POS_MOMENT:	
	case	STM_MODEL_FOOT_STT_MAX_HORI_FORCE:	
	case	STM_MODEL_FOOT_STT_MIN_HORI_FORCE:	
	case	STM_MODEL_FOOT_STT_MAX_AXIAL_EXT_1:	
	case	STM_MODEL_FOOT_STT_MAX_NEG_MOMENT_EXT_1:
	case	STM_MODEL_FOOT_STT_MAX_POS_MOMENT_EXT_1:
	case	STM_MODEL_FOOT_STT_MAX_HORI_FORCE_EXT_1:
	case	STM_MODEL_FOOT_STT_MIN_HORI_FORCE_EXT_1:
	case	STM_MODEL_FOOT_STT_MAX_AXIAL_EXT_2:
	case	STM_MODEL_FOOT_STT_MAX_NEG_MOMENT_EXT_2:
	case	STM_MODEL_FOOT_STT_MAX_POS_MOMENT_EXT_2:
	case	STM_MODEL_FOOT_STT_MAX_HORI_FORCE_EXT_2:
	case	STM_MODEL_FOOT_STT_MIN_HORI_FORCE_EXT_2:
		{
			if( STM_MODEL_FOOT_STT_MAX_AXIAL_EXT_1 <= iKey && iKey <= STM_MODEL_FOOT_STT_MIN_HORI_FORCE_EXT_1 )
			{
				sDepth_Add = _T("극단상황Ⅰ");
			}
			else if( STM_MODEL_FOOT_STT_MAX_AXIAL_EXT_2 <= iKey && iKey <= STM_MODEL_FOOT_STT_MIN_HORI_FORCE_EXT_2 )
			{
				sDepth_Add = _T("극단상황Ⅱ");
			}
			sDepth1 = _T("시점");
			sDepth2 = _T("기초");
			if( bLsd )
				sDepth3 = szArrCaseLsd[(iKey - STM_MODEL_FOOT_STT_MAX_AXIAL)%5];
			else
				sDepth3 = szArrCase[iKey - STM_MODEL_FOOT_STT_MAXP];
		}
		break;
	case STM_MODEL_FOOT_END_MAXP:
	case STM_MODEL_FOOT_END_MAXM:
	case STM_MODEL_FOOT_END_MINM:
	case STM_MODEL_FOOT_END_EARTH:
	case	STM_MODEL_FOOT_END_MAX_AXIAL:				// Lsd
	case	STM_MODEL_FOOT_END_MAX_NEG_MOMENT:
	case	STM_MODEL_FOOT_END_MAX_POS_MOMENT:	
	case	STM_MODEL_FOOT_END_MAX_HORI_FORCE:	
	case	STM_MODEL_FOOT_END_MIN_HORI_FORCE:	
	case	STM_MODEL_FOOT_END_MAX_AXIAL_EXT_1:	
	case	STM_MODEL_FOOT_END_MAX_NEG_MOMENT_EXT_1:
	case	STM_MODEL_FOOT_END_MAX_POS_MOMENT_EXT_1:
	case	STM_MODEL_FOOT_END_MAX_HORI_FORCE_EXT_1:
	case	STM_MODEL_FOOT_END_MIN_HORI_FORCE_EXT_1:
	case	STM_MODEL_FOOT_END_MAX_AXIAL_EXT_2:
	case	STM_MODEL_FOOT_END_MAX_NEG_MOMENT_EXT_2:
	case	STM_MODEL_FOOT_END_MAX_POS_MOMENT_EXT_2:
	case	STM_MODEL_FOOT_END_MAX_HORI_FORCE_EXT_2:
	case	STM_MODEL_FOOT_END_MIN_HORI_FORCE_EXT_2:
		{
			if( STM_MODEL_FOOT_END_MAX_AXIAL_EXT_1 <= iKey && iKey <= STM_MODEL_FOOT_END_MIN_HORI_FORCE_EXT_1 )
			{
				sDepth_Add = _T("극단상황Ⅰ");
			}
			else if( STM_MODEL_FOOT_END_MAX_AXIAL_EXT_2 <= iKey && iKey <= STM_MODEL_FOOT_END_MIN_HORI_FORCE_EXT_2 )
			{
				sDepth_Add = _T("극단상황Ⅱ");
			}

			sDepth1 = _T("종점");
			sDepth2 = _T("기초");
			if( bLsd )
				sDepth3 = szArrCaseLsd[(iKey - STM_MODEL_FOOT_END_MAX_AXIAL)%5];
			else
				sDepth3 = szArrCase[iKey - STM_MODEL_FOOT_END_MAXP];
		}
		break;
	case STM_MODEL_MIDFOOT_HEAD:
	case	STM_MODEL_FOOT_MID_MAX_AXIAL:				// Lsd
	case	STM_MODEL_FOOT_MID_MAX_NEG_MOMENT:
	case	STM_MODEL_FOOT_MID_MAX_POS_MOMENT:	
	case	STM_MODEL_FOOT_MID_MAX_HORI_FORCE:	
	case	STM_MODEL_FOOT_MID_MIN_HORI_FORCE:	
	case	STM_MODEL_FOOT_MID_MAX_AXIAL_EXT_1:	
	case	STM_MODEL_FOOT_MID_MAX_NEG_MOMENT_EXT_1:
	case	STM_MODEL_FOOT_MID_MAX_POS_MOMENT_EXT_1:
	case	STM_MODEL_FOOT_MID_MAX_HORI_FORCE_EXT_1:
	case	STM_MODEL_FOOT_MID_MIN_HORI_FORCE_EXT_1:
	case	STM_MODEL_FOOT_MID_MAX_AXIAL_EXT_2:
	case	STM_MODEL_FOOT_MID_MAX_NEG_MOMENT_EXT_2:
	case	STM_MODEL_FOOT_MID_MAX_POS_MOMENT_EXT_2:
	case	STM_MODEL_FOOT_MID_MAX_HORI_FORCE_EXT_2:
	case	STM_MODEL_FOOT_MID_MIN_HORI_FORCE_EXT_2:
	default:
		{
			long iJijum = bLsd==TRUE ? ((iKey-STM_MODEL_FOOT_MID_MAX_AXIAL) / 15) : ((iKey - STM_MODEL_MIDFOOT_HEAD) / 4);
			long iCase = bLsd==TRUE ? ((iKey-STM_MODEL_FOOT_MID_MAX_AXIAL) % 5) : (iKey - STM_MODEL_MIDFOOT_HEAD) % 4;
			sDepth1.Format(_T("지점%d"), iJijum+1);
			sDepth2 = _T("기초");
			if( bLsd )
			{
				long nKeyIndex = (iKey-STM_MODEL_FOOT_MID_MAX_AXIAL) % 15;
				if(nKeyIndex >= 10)
					sDepth_Add = _T("극단상황Ⅱ");
				else if(nKeyIndex >= 5)
					sDepth_Add = _T("극단상황Ⅰ");
				sDepth3 = szArrCaseLsd[iCase];
			}
			else
				sDepth3 = szArrCase[iCase];
		}
		break;
	}

	CString szName(_T(""));
	if(iDepth == 0)
	{
		if(bLsd)
			szName = sDepth1 + sDepth2 + sDepth3 + _T("-") + sDepth_Add;
		else
			szName = sDepth1 + sDepth2 + sDepth3;
	}
	else
	{
		switch (iDepth)
		{
		case 1:
			szName = sDepth1;	break;
		case 2:
			szName = sDepth2;	break;
		case 3:
			szName = sDepth3;	break;
		case 4:
			if(iKey >= STM_MODEL_FOOT_STT_MAXP)
				szName = sDepth_Add + _T(" (") + sDepth3 + _T(")");
			else
				szName = sDepth_Add;
		default:
			break;
		}
	}

	return szName;
}

void CRcBridgeApp::MakeStmModelSyncAll( HGBOOL bOutput )
{
	// Frame 해석이 수행되었다면 하중을 구해서 하중이 0인 모델은 만들지 않는다.
	CFEMManage *pFem = GetFemManage(COMBO_ULTIMATE);

	HGBOOL bCheckedNoneLoadCase = FALSE;
	HGBOOL bCheckLoad = (pFem && pFem->IsOutputLoaded())? TRUE : FALSE;
	HGBOOL bLsd = m_pARcBridgeDataStd->IsLsdDesign();
	HGINT32 icCase = bLsd ? 3 : 1;
	// Bracket
	for(long ix=0; ix<2; ix++)
	{
		BOOL bSelect = bOutput ? TRUE : m_bApplyStmModelingBracket[ix];
		BOOL bExist = IsBracket(ix==iSTT);

		if(bExist && bCheckLoad)
		{
			bExist = IsAps(ix==iSTT, TRUE);
		}

		HGINT32 iKey = GetKeyStmModel(eStmBracket, ix==iSTT);
		SetStmModelByKey(iKey, (bSelect && bExist), bOutput);

		// 브라켓 하중조합 - 브라켓은 하중조합이 없으므로 공백으로 둔다
		if(bLsd == FALSE)
			m_strStmModelingBracket[ix] = _T("");
	}

	// Corner
	for(long nStt=0; nStt<2; nStt++)
	{
		HGDOUBLE dLoadValue(0.);
		for (long nCase=0; nCase<icCase; ++nCase)		// 극한상황
		{
			CString str[3] = { _T("ST"), _T("EX1"), _T("EX2") };

			for(long nUpper=0; nUpper<2; nUpper++)
			{
				BOOL bExist = IsOutWall(nStt==iSTT);
				if(nUpper == 1 && !IsBoxType())
				{
					bExist = FALSE;
				}

				for(long ix=0; ix<2; ix++)
				{
					// 우각부 - 하중조합
					 m_strStmModelingCorner[nStt][nCase][nUpper][ix] = str[nCase];

					BOOL bSelect = bOutput ? TRUE : m_bApplyStmModelingCorner[nStt][nCase][nUpper][ix];
 					if(bExist && bCheckLoad && bSelect)
 					{
 						dLoadValue = GetMomentCorner(nStt==iSTT, FALSE, ix==0, nUpper==iUPPER, nCase==0 ? eEnvLimit : eEnvExtremeI);
 						if(Compare(dLoadValue, 0.0, _T("="), 0.1))
						{
 							bCheckedNoneLoadCase = TRUE;
						}
 					}
					HGINT32 iKey = GetKeyStmModel(eStmCorner, nStt==iSTT, nUpper==iUPPER, nCase, ix);
					SetStmModelByKey(iKey, (bSelect && bExist), bOutput);
				}
			}
		}
	}

	// Foot
	HGINT32 icEnv = bLsd ? 5 : 4;
	for(long nStt=0; nStt<2; nStt++)				// 시점 종점
	{
		BOOL bExist = IsFooting(nStt==iSTT);
		HGINT32 ixKey = bLsd ? (nStt==0 ? STM_MODEL_FOOT_STT_MAX_AXIAL : STM_MODEL_FOOT_END_MAX_AXIAL) : (nStt==0 ? STM_MODEL_FOOT_STT_MAXP : STM_MODEL_FOOT_END_MAXP);

		
		for(long ix=0; ix<icCase; ix++)					// 극단상황 1,2,3
		{
			for(long jx=0; jx<icEnv; jx++)				// 요소 4개 - MAXP, MAXM, MINM, EARTH
			{
				// 하중조합
				ELSDCombKind eKindCombo = GetComboKindByEnvType((ETypeEnvComboLsd)ix, nStt==0 ? ePartFootStt : ePartFootEnd, CHECK_POS_CENTER);
			
				BOOL bSelect = m_bApplyStmModelingFooting[nStt][ix][jx];
				if(bExist && bCheckLoad)
				{
					HGDOUBLE dLoadValue(0), dTemp(0);
					if( bLsd )
					{
						long nIdxModel(0);
						long nCombo = GetOutValueForFootingDesign_Lsd((nStt==iSTT)? 0 : m_nQtyJigan, GetTypeEnvComboLsdFooting(ixKey+jx, nStt==0 ? 0 : GetCountJijum()), static_cast<EFootingCheckLsd>(jx), dLoadValue, dTemp, dTemp, nIdxModel);
						m_strStmModelingFooting[nStt][ix][jx] = GetStringComboEta_Lsd(eKindCombo, nStt==0 ? ePartFootStt : ePartFootEnd, nCombo + 1, TRUE, nIdxModel);
					}
					else
						GetOutValueForFootingDesign((nStt==iSTT)? 0 : m_nQtyJigan, jx, dLoadValue, dTemp, dTemp, FALSE);
					if(Compare(dLoadValue, 0.0, _T("="), 0.1) && bSelect)
						bCheckedNoneLoadCase = TRUE;
				}

				HGINT32 iKey = GetKeyStmModel(eStmFoot, nStt==iSTT, 0, ix, jx);
				SetStmModelByKey(iKey, bOutput ? TRUE : (bSelect && bExist), bOutput);
			}
		}
	}

	// MidFoot
	for(long nJijum=1; nJijum<MAX_JIJUM_COUNT_RC; nJijum++)
	{
		BOOL bExist = IsFootingJijum(nJijum);

		for(long ix=0; ix<icCase; ix++)
		{
			for (long iy = 0; iy < icEnv; ++iy)
			{
				// 하중조합
				ELSDCombKind eKindCombo = GetComboKindByEnvType((ETypeEnvComboLsd)ix, ePartInFoot, CHECK_POS_CENTER);
				
				BOOL bSelect = m_bApplyStmModelingMidFooting[nJijum-1][ix][iy];
				BOOL bValue = FALSE;
				if(bExist && bCheckLoad)
				{
					HGDOUBLE dLoadValue(0), dTemp(0);

					ETypeEnvComboLsd eType = eEnvLimit;

					switch( ix )
					{
					case 0:
						eType = eEnvLimit;
						break;
					case 1:
						eType = eEnvExtremeI;
						break;
					case 2:
						eType = eEnvExtremeII;
						break;
					}

					if( m_pARcBridgeDataStd->IsLsdDesign() )
					{
						long nIdxModel(0);
						long nCombo = GetOutValueForFootingDesign_Lsd(nJijum, eType, static_cast<EFootingCheckLsd>(iy), dLoadValue, dTemp, dTemp, nIdxModel);
						m_strStmModelingMidFooting[nJijum-1][ix][iy] = GetStringComboEta_Lsd(eKindCombo, ePartInFoot, nCombo + 1, TRUE, nIdxModel);
					}
					else
						GetOutValueForFootingDesign(nJijum, iy, dLoadValue, dTemp, dTemp, FALSE);

					bValue = (Compare(dLoadValue, 0.0, _T("="), 0.1))? FALSE : TRUE;
					if(bValue == FALSE && bSelect)
						bCheckedNoneLoadCase = TRUE;
				}

				HGINT32 iKey = GetKeyStmModel(eStmMidFoot, FALSE, 0, nJijum-1, iy, ix);
				SetStmModelByKey(iKey, bOutput ? TRUE : bSelect, bOutput);
			}
		}
	}

	// 모든 모델에 입력받은 CStmDataConfig를 복사해준다.
	// Lsd일때는 재료강도를 각각 받기 때문에 m_pStmSolverInput의 Config를 사용하지 않는다. 모델 생성할때만 반영한다.
	if(bLsd == FALSE)
	{
		CStmDataConfig *pConfigOrg = m_pStmSolverInput->GetStmDataConfig();

		map<int, CStmModelForArcbridge*>::iterator iter = bOutput ? m_mStmSolverOut.begin() : m_mStmSolver.begin();
		if( bOutput )
		{
			for(iter; iter!=m_mStmSolverOut.end(); ++iter)
			{
				CStmModelForArcbridge *pModel = iter->second;
				if( pModel == NULL ) continue;

				CStmDataConfig *pDataConfig = pModel->GetStmDataConfig();
				*pDataConfig = *pConfigOrg;
			}
		}
		else
		{
			for(iter; iter!=m_mStmSolver.end(); ++iter)
			{
				CStmModelForArcbridge *pModel = iter->second;

				CStmDataConfig *pDataConfig = pModel->GetStmDataConfig();
				*pDataConfig = *pConfigOrg;
			}
		}
	}

	if(bCheckedNoneLoadCase)
	{
		AfxMessageBox(_T("부재력이 0인 STM검토가 설정되었습니다.\n해당 모델은 검토가 필요치 않으며\n검토진행시 정상적인 모델작성 및 해석이 되지않을 수 있으므로\n체크해제후 진행하시기 바랍니다"), MB_OK | MB_ICONINFORMATION);
	}

	return;
}

void CRcBridgeApp::SetStmModelByKey( HGINT32 iKey, BOOL bMakeModel, HGBOOL bOutput )
{
	map<int, CStmModelForArcbridge*>::iterator iter = bOutput ? m_mStmSolverOut.find(iKey) : m_mStmSolver.find(iKey);
	BOOL bExistModel = bOutput ? (iter != m_mStmSolverOut.end()) : (iter != m_mStmSolver.end());
	
	if(bMakeModel != bExistModel)
	{
		if(bMakeModel)
		{
			CStmModelForArcbridge *pModel = new CStmModelForArcbridge;
			// pModel->m_pDataConfig
			if( bOutput )
				m_mStmSolverOut.insert(std::pair<int, CStmModelForArcbridge*>(iKey, pModel));
			else
				m_mStmSolver.insert(std::pair<int, CStmModelForArcbridge*>(iKey, pModel));
		}
		else
		{
			if( bOutput )
				m_mStmSolverOut.erase(iter);
			else
				m_mStmSolver.erase(iter);
		}
	}

	return;
}

double CRcBridgeApp::GetValueFck( EPartOfBridge ePos, BOOL bChangeUnit/*=FALSE*/ )
{
	double dFck(0);
	// 한계상태 설계가 적용되었을때는 각 구조물 별로 적용된다.
	if(m_pARcBridgeDataStd->IsLsdDesign())
	{
		if(bChangeUnit)
			dFck = GetValueUnitNmm(m_Fck[ePos], UNIT_CGS_KGF_CM2);
		else
			dFck = m_Fck[ePos];
	}
	else
	{
		if(ePos == ePartApsStt || ePos == ePartApsEnd)
		{
			dFck = (ePos == ePartApsStt)? m_apsStt[0].m_dFck : m_apsEnd[0].m_dFck;
			if(bChangeUnit)
				dFck = GetValueUnitChange(dFck, UNIT_CGS_KGF_CM2, m_pARcBridgeDataStd->m_szKgf_CM2);
		}
		else
		{
			if(bChangeUnit)
				dFck = GetValueUnitChange(m_pARcBridgeDataStd->m_Fck, UNIT_CGS_KGF_CM2, m_pARcBridgeDataStd->m_szKgf_CM2);
			else
				dFck = m_pARcBridgeDataStd->m_Fck;
		}		
	}

	return dFck;
}

double CRcBridgeApp::GetValueFy( EPartOfBridge ePos, BOOL bShearRebar, BOOL bChangeUnit/*=FALSE*/ )
{
	double dFy(0);
	// 한계상태 설계가 적용되었을때는 각 구조물 별로 적용된다.
	if(m_pARcBridgeDataStd->IsLsdDesign())
	{
		if(bChangeUnit)
			dFy = GetValueUnitNmm(bShearRebar? m_Fvy[ePos] : m_Fy[ePos], UNIT_CGS_KGF_CM2);
		else
			dFy = bShearRebar? m_Fvy[ePos] : m_Fy[ePos];
	}
	else
	{
		if(ePos == ePartApsStt || ePos == ePartApsEnd)
		{
			dFy = (ePos == ePartApsStt)? m_apsStt[0].m_dFy : m_apsEnd[0].m_dFy;
			if(bChangeUnit)
				dFy = GetValueUnitChange(dFy, UNIT_CGS_KGF_CM2, m_pARcBridgeDataStd->m_szKgf_CM2);
		}
		else
		{
			if(bChangeUnit)
				dFy = GetValueUnitChange(m_pARcBridgeDataStd->m_Fy, UNIT_CGS_KGF_CM2, m_pARcBridgeDataStd->m_szKgf_CM2);
			else
				dFy = m_pARcBridgeDataStd->m_Fy;
		}		
	}

	return dFy;
}

double CRcBridgeApp::GetValueEc( EPartOfBridge ePos, BOOL bChangeUnit/*=FALSE*/ )
{
	double dEc(0);
	// 한계상태 설계가 적용되었을때는 각 구조물 별로 적용된다.
	if(m_pARcBridgeDataStd->IsLsdDesign())
	{
		if(bChangeUnit)
			dEc = GetValueUnitNmm(m_Ec[ePos], UNIT_CGS_KGF_CM2);
		else
			dEc = m_Ec[ePos];
	}
	else
	{
		if(bChangeUnit)
		{
			// 단위변환을 한다면 CGS단위 일때도 Kgf/cm2을 사용하지 않고 Tonf/m2을 사용해야한다.
			dEc = GetValueUnitChange(m_pARcBridgeDataStd->m_Ec * 10, UNIT_CGS_TONF_M2, m_pARcBridgeDataStd->m_szTonf_M2);	// * 10 : Kgf/cm2 -> Tonf/m2;
		}
		else
			dEc = m_pARcBridgeDataStd->m_Ec;
	}

	return dEc;
}

void CRcBridgeApp::SetDataDefaultExposureRatingLsd()
{
	CLsdExposureRatingCtl *pExposureCtl = &(m_plsdEnvRating->GetLsdExposureRatingCtl());
	if(!pExposureCtl) return;

	// CRcBridgeRebar::SetExposureParameter() 에서의 순서와 맞춰야 한다.

	m_plsdEnvRating->SetDataDefaultExposureRating();
	
	pExposureCtl->AddMaterial(GetStringPart(ePartUpperSlab, TRUE),  FALSE, 0, 0, 0, 10, STR_EXPOSURE_SETTING_FERRO);
	pExposureCtl->AddMaterial(GetStringPart(ePartUpperSlab, FALSE), FALSE, 0, 0, 0, 10, STR_EXPOSURE_SETTING_FERRO);

	if(IsBoxType())
	{
		pExposureCtl->AddMaterial(GetStringPart(ePartLowerSlab, TRUE),  FALSE, 0, 0, 0, 10, STR_EXPOSURE_SETTING_FERRO);
		pExposureCtl->AddMaterial(GetStringPart(ePartLowerSlab, FALSE), FALSE, 0, 0, 0, 10, STR_EXPOSURE_SETTING_FERRO);
	}
	// 시점측
	if(IsOutWall(TRUE))
	{
		pExposureCtl->AddMaterial(GetStringPart(ePartOutWallStt, TRUE),  FALSE, 0, 0, 0, 10, STR_EXPOSURE_SETTING_FERRO);
		pExposureCtl->AddMaterial(GetStringPart(ePartOutWallStt, FALSE), FALSE, 0, 0, 0, 10, STR_EXPOSURE_SETTING_FERRO);

		if(!IsBoxType())
		{
			pExposureCtl->AddMaterial(GetStringPart(ePartFootStt, TRUE),  FALSE, 0, 0, 0, 10, STR_EXPOSURE_SETTING_FERRO);
			pExposureCtl->AddMaterial(GetStringPart(ePartFootStt, FALSE), FALSE, 0, 0, 0, 10, STR_EXPOSURE_SETTING_FERRO);
		}

		pExposureCtl->AddMaterial(GetStringPart(ePartWingWallStt, TRUE),  FALSE, 0, 0, 0, 10, STR_EXPOSURE_SETTING_FERRO);
		pExposureCtl->AddMaterial(GetStringPart(ePartWingWallStt, FALSE), FALSE, 0, 0, 0, 10, STR_EXPOSURE_SETTING_FERRO);
	}

	pExposureCtl->AddMaterial(GetStringPart(ePartApsStt, TRUE),  FALSE, 0, 0, 0, 10, STR_EXPOSURE_SETTING_FERRO);
	pExposureCtl->AddMaterial(GetStringPart(ePartApsStt, FALSE), FALSE, 0, 0, 0, 10, STR_EXPOSURE_SETTING_FERRO);

	// 종점측
	if(IsOutWall(FALSE))
	{
		pExposureCtl->AddMaterial(GetStringPart(ePartOutWallEnd, TRUE),  FALSE, 0, 0, 0, 10, STR_EXPOSURE_SETTING_FERRO);
		pExposureCtl->AddMaterial(GetStringPart(ePartOutWallEnd, FALSE), FALSE, 0, 0, 0, 10, STR_EXPOSURE_SETTING_FERRO);

		if(!IsBoxType())
		{
			pExposureCtl->AddMaterial(GetStringPart(ePartFootEnd, TRUE),  FALSE, 0, 0, 0, 10, STR_EXPOSURE_SETTING_FERRO);
			pExposureCtl->AddMaterial(GetStringPart(ePartFootEnd, FALSE), FALSE, 0, 0, 0, 10, STR_EXPOSURE_SETTING_FERRO);
		}

		pExposureCtl->AddMaterial(GetStringPart(ePartWingWallEnd, TRUE),  FALSE, 0, 0, 0, 10, STR_EXPOSURE_SETTING_FERRO);
		pExposureCtl->AddMaterial(GetStringPart(ePartWingWallEnd, FALSE), FALSE, 0, 0, 0, 10, STR_EXPOSURE_SETTING_FERRO);
	}

	pExposureCtl->AddMaterial(GetStringPart(ePartApsEnd, TRUE),  FALSE, 0, 0, 0, 10, STR_EXPOSURE_SETTING_FERRO);
	pExposureCtl->AddMaterial(GetStringPart(ePartApsEnd, FALSE), FALSE, 0, 0, 0, 10, STR_EXPOSURE_SETTING_FERRO);

	// 중간지점부
	if(m_nTypeBridge != BRIDGE_TYPE_SLAB && m_nQtyJigan > 1)
	{
		pExposureCtl->AddMaterial(GetStringPart(ePartInWall, TRUE), FALSE, 0, 0, 0, 10, STR_EXPOSURE_SETTING_FERRO);

		if(!IsBoxType())
		{
			pExposureCtl->AddMaterial(GetStringPart(ePartInFoot, TRUE),  FALSE, 0, 0, 0, 10, STR_EXPOSURE_SETTING_FERRO);
			pExposureCtl->AddMaterial(GetStringPart(ePartInFoot, FALSE), FALSE, 0, 0, 0, 10, STR_EXPOSURE_SETTING_FERRO);
		}
	}

	long nKeyUpper = pExposureCtl->GetIndexByMaterialTitle(GetStringPart(ePartUpperSlab, TRUE));
	pExposureCtl->SetRatingFlag(nKeyUpper, STR_EXPOSURERATING_EC4, TRUE);
	pExposureCtl->SetRatingFlag(nKeyUpper, STR_EXPOSURERATING_ED1, TRUE);
	pExposureCtl->SetRatingFlag(nKeyUpper, STR_EXPOSURERATING_EF3, TRUE);
	pExposureCtl->SetRatingFlag(nKeyUpper, STR_EXPOSURERATING_EA2, TRUE);

	nKeyUpper = pExposureCtl->GetIndexByMaterialTitle(GetStringPart(ePartUpperSlab, FALSE));
	pExposureCtl->SetRatingFlag(nKeyUpper, STR_EXPOSURERATING_EC4, TRUE);
	pExposureCtl->SetRatingFlag(nKeyUpper, STR_EXPOSURERATING_ED1, TRUE);
	pExposureCtl->SetRatingFlag(nKeyUpper, STR_EXPOSURERATING_EF3, TRUE);
	pExposureCtl->SetRatingFlag(nKeyUpper, STR_EXPOSURERATING_EA2, TRUE);

	if(m_nTypeBridge != BRIDGE_TYPE_SLAB)
	{
		for(long nStt=0; nStt<2; ++nStt)
		{
			EPartOfBridge ePart = (nStt == 0)? ePartOutWallStt : ePartOutWallEnd;

			nKeyUpper = pExposureCtl->GetIndexByMaterialTitle(GetStringPart(ePart, TRUE));
			pExposureCtl->SetRatingFlag(nKeyUpper, STR_EXPOSURERATING_EC4, TRUE);
			pExposureCtl->SetRatingFlag(nKeyUpper, STR_EXPOSURERATING_ED1, TRUE);
			pExposureCtl->SetRatingFlag(nKeyUpper, STR_EXPOSURERATING_EF1, TRUE);
			pExposureCtl->SetRatingFlag(nKeyUpper, STR_EXPOSURERATING_EA2, TRUE);

			nKeyUpper = pExposureCtl->GetIndexByMaterialTitle(GetStringPart(ePart, FALSE));
			pExposureCtl->SetRatingFlag(nKeyUpper, STR_EXPOSURERATING_EC2, TRUE);
			pExposureCtl->SetRatingFlag(nKeyUpper, STR_EXPOSURERATING_ED1, TRUE);
			pExposureCtl->SetRatingFlag(nKeyUpper, STR_EXPOSURERATING_EF1, TRUE);
			pExposureCtl->SetRatingFlag(nKeyUpper, STR_EXPOSURERATING_EA2, TRUE);
		}

		nKeyUpper = pExposureCtl->GetIndexByMaterialTitle(GetStringPart(ePartInWall, TRUE));
		pExposureCtl->SetRatingFlag(nKeyUpper, STR_EXPOSURERATING_EC4, TRUE);
		pExposureCtl->SetRatingFlag(nKeyUpper, STR_EXPOSURERATING_ED1, TRUE);
		pExposureCtl->SetRatingFlag(nKeyUpper, STR_EXPOSURERATING_EF1, TRUE);
		pExposureCtl->SetRatingFlag(nKeyUpper, STR_EXPOSURERATING_EA2, TRUE);
	}
	
	if(IsBoxType())
	{
		nKeyUpper = pExposureCtl->GetIndexByMaterialTitle(GetStringPart(ePartLowerSlab, TRUE));
		pExposureCtl->SetRatingFlag(nKeyUpper, STR_EXPOSURERATING_EC2, TRUE);
		pExposureCtl->SetRatingFlag(nKeyUpper, STR_EXPOSURERATING_ED1, TRUE);
		pExposureCtl->SetRatingFlag(nKeyUpper, STR_EXPOSURERATING_EF3, TRUE);
		pExposureCtl->SetRatingFlag(nKeyUpper, STR_EXPOSURERATING_EA2, TRUE);

		nKeyUpper = pExposureCtl->GetIndexByMaterialTitle(GetStringPart(ePartLowerSlab, FALSE));
		pExposureCtl->SetRatingFlag(nKeyUpper, STR_EXPOSURERATING_EC4, TRUE);
		pExposureCtl->SetRatingFlag(nKeyUpper, STR_EXPOSURERATING_ED1, TRUE);
		pExposureCtl->SetRatingFlag(nKeyUpper, STR_EXPOSURERATING_EF3, TRUE);
		pExposureCtl->SetRatingFlag(nKeyUpper, STR_EXPOSURERATING_EA2, TRUE);
	}
	else if(m_nTypeBridge != BRIDGE_TYPE_SLAB)
	{
		for(long nStt=0; nStt<2; ++nStt)
		{
			EPartOfBridge ePart = (nStt == 0)? ePartFootStt : ePartFootEnd;

			nKeyUpper = pExposureCtl->GetIndexByMaterialTitle(GetStringPart(ePart, TRUE));
			pExposureCtl->SetRatingFlag(nKeyUpper, STR_EXPOSURERATING_EC2, TRUE);
			pExposureCtl->SetRatingFlag(nKeyUpper, STR_EXPOSURERATING_ED1, TRUE);
			pExposureCtl->SetRatingFlag(nKeyUpper, STR_EXPOSURERATING_EF3, TRUE);
			pExposureCtl->SetRatingFlag(nKeyUpper, STR_EXPOSURERATING_EA2, TRUE);

			nKeyUpper = pExposureCtl->GetIndexByMaterialTitle(GetStringPart(ePart, FALSE));
			pExposureCtl->SetRatingFlag(nKeyUpper, STR_EXPOSURERATING_EC4, TRUE);
			pExposureCtl->SetRatingFlag(nKeyUpper, STR_EXPOSURERATING_ED1, TRUE);
			pExposureCtl->SetRatingFlag(nKeyUpper, STR_EXPOSURERATING_EF3, TRUE);
			pExposureCtl->SetRatingFlag(nKeyUpper, STR_EXPOSURERATING_EA2, TRUE);
		}

		nKeyUpper = pExposureCtl->GetIndexByMaterialTitle(GetStringPart(ePartInFoot, TRUE));
		pExposureCtl->SetRatingFlag(nKeyUpper, STR_EXPOSURERATING_EC2, TRUE);
		pExposureCtl->SetRatingFlag(nKeyUpper, STR_EXPOSURERATING_ED1, TRUE);
		pExposureCtl->SetRatingFlag(nKeyUpper, STR_EXPOSURERATING_EF3, TRUE);
		pExposureCtl->SetRatingFlag(nKeyUpper, STR_EXPOSURERATING_EA2, TRUE);

		nKeyUpper = pExposureCtl->GetIndexByMaterialTitle(GetStringPart(ePartInFoot, FALSE));
		pExposureCtl->SetRatingFlag(nKeyUpper, STR_EXPOSURERATING_EC4, TRUE);
		pExposureCtl->SetRatingFlag(nKeyUpper, STR_EXPOSURERATING_ED1, TRUE);
		pExposureCtl->SetRatingFlag(nKeyUpper, STR_EXPOSURERATING_EF1, TRUE);
		pExposureCtl->SetRatingFlag(nKeyUpper, STR_EXPOSURERATING_EA2, TRUE);
	}
	
	if(IsOutWall(TRUE) || IsOutWall(TRUE))
	{
		for(long nStt=0; nStt<2; ++nStt)
		{
			EPartOfBridge ePart = (nStt == 0)? ePartWingWallStt : ePartWingWallEnd;

			nKeyUpper = pExposureCtl->GetIndexByMaterialTitle(GetStringPart(ePart, TRUE));
			pExposureCtl->SetRatingFlag(nKeyUpper, STR_EXPOSURERATING_EC4, TRUE);
			pExposureCtl->SetRatingFlag(nKeyUpper, STR_EXPOSURERATING_ED1, TRUE);
			pExposureCtl->SetRatingFlag(nKeyUpper, STR_EXPOSURERATING_EF1, TRUE);
			pExposureCtl->SetRatingFlag(nKeyUpper, STR_EXPOSURERATING_EA2, TRUE);
			pExposureCtl->SetCheckReport(nKeyUpper, IsWingWall(nStt==0, TRUE) || IsWingWall(nStt==0, FALSE));

			nKeyUpper = pExposureCtl->GetIndexByMaterialTitle(GetStringPart(ePart, FALSE));
			pExposureCtl->SetRatingFlag(nKeyUpper, STR_EXPOSURERATING_EC2, TRUE);
			pExposureCtl->SetRatingFlag(nKeyUpper, STR_EXPOSURERATING_ED1, TRUE);
			pExposureCtl->SetRatingFlag(nKeyUpper, STR_EXPOSURERATING_EF1, TRUE);
			pExposureCtl->SetRatingFlag(nKeyUpper, STR_EXPOSURERATING_EA2, TRUE);
			pExposureCtl->SetCheckReport(nKeyUpper, IsWingWall(nStt==0, TRUE) || IsWingWall(nStt==0, FALSE));
		}
	}

	for(long nStt=0; nStt<2; ++nStt)
	{
		EPartOfBridge ePart = (nStt == 0)? ePartApsStt : ePartApsEnd;

		nKeyUpper = pExposureCtl->GetIndexByMaterialTitle(GetStringPart(ePart, TRUE));
		pExposureCtl->SetRatingFlag(nKeyUpper, STR_EXPOSURERATING_EC4, TRUE);
		pExposureCtl->SetRatingFlag(nKeyUpper, STR_EXPOSURERATING_ED1, TRUE);
		pExposureCtl->SetRatingFlag(nKeyUpper, STR_EXPOSURERATING_EF3, TRUE);
		pExposureCtl->SetRatingFlag(nKeyUpper, STR_EXPOSURERATING_EA2, TRUE);
		pExposureCtl->SetCheckReport(nKeyUpper, IsAps(nStt==0, TRUE) || IsAps(nStt==0, FALSE));

		nKeyUpper = pExposureCtl->GetIndexByMaterialTitle(GetStringPart(ePart, FALSE));
		pExposureCtl->SetRatingFlag(nKeyUpper, STR_EXPOSURERATING_EC4, TRUE);
		pExposureCtl->SetRatingFlag(nKeyUpper, STR_EXPOSURERATING_ED1, TRUE);
		pExposureCtl->SetRatingFlag(nKeyUpper, STR_EXPOSURERATING_EF3, TRUE);
		pExposureCtl->SetRatingFlag(nKeyUpper, STR_EXPOSURERATING_EA2, TRUE);
		pExposureCtl->SetCheckReport(nKeyUpper, IsAps(nStt==0, TRUE) || IsAps(nStt==0, FALSE));
	}
}

void CRcBridgeApp::SetDataDefaultDesignRatingLsd()
{
	CLsdExposureRatingCtl *pExposureCtl = &(m_plsdEnvRating->GetLsdExposureRatingCtl());
	if(!pExposureCtl) return;
	CLsdDesignRatingCtl *pDesignCtl = &(m_plsdEnvRating->GetLsdDesignRatingCtl());
	if(!pDesignCtl) return;

	long nSize = pExposureCtl->GetMaterialSize();
	m_plsdEnvRating->SetDataDefaultDesignRating();
	pDesignCtl->AddMaterialGrade(nSize, DESIGNRATING_RATECOUNT - 1);
}

void CRcBridgeApp::SetDataDefaultRebarFactorLsd()
{
	m_plsdRebar->GetOption().dn1[0] = 1.0;
	m_plsdRebar->GetOption().dn1[1] = 0.7;
	m_plsdRebar->GetOption().dn2 = 1.0;
	m_plsdRebar->GetOption().dRestraintStress = 0.;
	m_plsdRebar->GetOption().dOverRatio = 50.0;
	m_plsdRebar->GetOption().dAsRatio = 1.;
	m_plsdRebar->GetOption().dK_RebarPos[iOUTTER] = 0.00;
	m_plsdRebar->GetOption().dK_RebarPos[iINNER] = 0.00;
}

void CRcBridgeApp::SetDataDefaultRebarSettleLsd()
{
	m_plsdRebar->Init();
}

void CRcBridgeApp::SetDataDefaultRebarJointLsd()
{
	m_plsdRebar->Init();
}

void CRcBridgeApp::SetDataDefaultSelectLoadLsd(BOOL bForCantilever)
{
	if(bForCantilever)
		m_plsdMngForCantilever->SetDataDefaultSelectLoad();
	else
		m_plsdManager->SetDataDefaultSelectLoad();
}

void CRcBridgeApp::SetDataDefaultLoadFactor(BOOL bForCantilever)
{
	CLsdManagerBridge *pLsdMng = bForCantilever? m_plsdMngForCantilever : m_plsdManager;

	// (ARCBRIDGE-3232) 피로 검토 조합 제거 -> 피로활하중을 그대로 쓴다.
	// pLsdMng->SetDataDefaultLoadFactor(bForCantilever? FALSE : TRUE);	
	pLsdMng->SetDataDefaultLoadFactor(FALSE);	

	// 전체 check off
	map<ELSDCombKind, CLsdLoad *> vecLsdLoadFactor = pLsdMng->GetLoadArray();
	for(auto ite=vecLsdLoadFactor.begin(); ite!=vecLsdLoadFactor.end(); ++ite)
	{		
		(*ite).second->SetUse(FALSE);
	} 
	CTwinVectorArray tvArr;
	for(long ix=eLSD_COMBINE_LIMIT_I; ix<eLSD_COMBINE_TOTAL_COUNT; ix++)
	{
		CLsdLoad *pLsdLoad = pLsdMng->GetLoad(static_cast<ELSDCombKind> (ix));
		if(pLsdLoad == NULL) continue;
		CLsdUnitLoad *pUnitLoad = pLsdLoad->GetUnitLoad(eLSD_UNITLOAD_EV);
		std::vector<CLsdUnitLoadGroup *> vArrGroup = pLsdLoad->GetUnitGroupArray();
		if(pUnitLoad == NULL) continue;

		BOOL bType_rP = FALSE;
		for(auto ite=vArrGroup.begin(); ite!=vArrGroup.end(); ++ite)
		{		
			if((*ite)->GetOptionType() == eFactor_rP)
				bType_rP = TRUE;
				break;
		}

		if(bType_rP)
			pUnitLoad->SetSel(IsBoxType()? 2 : 3);
	}

	// (ARCBRIDGE-3069) EH설정 기본값 변경
	for(long ix=eLSD_COMBINE_LIMIT_I; ix<eLSD_COMBINE_TOTAL_COUNT; ix++)
	{
		CLsdLoad *pLsdLoad = pLsdMng->GetLoad(static_cast<ELSDCombKind> (ix));
		if(pLsdLoad == NULL) continue;
		CLsdUnitLoad *pUnitLoad = pLsdLoad->GetUnitLoad(eLSD_UNITLOAD_EH);
		std::vector<CLsdUnitLoadGroup *> vArrGroup = pLsdLoad->GetUnitGroupArray();
		if(pUnitLoad == NULL) continue;

		BOOL bType_rP = FALSE;
		for(auto ite=vArrGroup.begin(); ite!=vArrGroup.end(); ++ite)
		{		
			if((*ite)->GetOptionType() == eFactor_rP)
				bType_rP = TRUE;
			break;
		}

		// (ARCBRIDGE-3154) EH하중 극단상황한계상태Ι 일때는 주동토압으로 설정
		if(bType_rP)
			pUnitLoad->SetSel(ix == eLSD_COMBINE_EXTREME_I ? 0 : 1);
	}

	for(long ix=eLSD_COMBINE_LIMIT_I; ix<eLSD_COMBINE_TOTAL_COUNT; ix++)
	{
		CLsdLoad *pLsdLoad = pLsdMng->GetLoad(static_cast<ELSDCombKind> (ix));
		if(pLsdLoad == NULL) continue;
		CLsdUnitLoad *pUnitLoad = pLsdLoad->GetUnitLoad(eLSD_UNITLOAD_TG);
		if(pUnitLoad)
		{
			pUnitLoad->SetMax(0.5);
		}

		pUnitLoad = pLsdLoad->GetUnitLoad(eLSD_UNITLOAD_SD);
		if(pUnitLoad)
		{
			pUnitLoad->SetMax(1.0);
		}
	}

	// ARCBRIDGE-3440
	// PS, CR, SH 기본값 변경
	for(long ix=eLSD_COMBINE_LIMIT_I; ix<eLSD_COMBINE_TOTAL_COUNT; ix++)
	{
		CLsdLoad *pLsdLoad = pLsdMng->GetLoad(static_cast<ELSDCombKind> (ix));
		if(pLsdLoad == NULL) continue;

		CLsdUnitLoad *pUnitLoad = pLsdLoad->GetUnitLoad(eLSD_UNITLOAD_PS);
		if(pUnitLoad)
		{
			pUnitLoad->SetSel(1);
		}

		pUnitLoad = pLsdLoad->GetUnitLoad(eLSD_UNITLOAD_CR);
		if(pUnitLoad)
		{
			pUnitLoad->SetSel(1);
		}

		pUnitLoad = pLsdLoad->GetUnitLoad(eLSD_UNITLOAD_SH);
		if(pUnitLoad)
		{
			pUnitLoad->SetSel(1);
		}
	}

	// 설계등급에서 체크된 옵션
	if(bForCantilever)
	{
		(*vecLsdLoadFactor.find(eLSD_COMBINE_LIMIT_I)).second->SetUse(TRUE);
		(*vecLsdLoadFactor.find(eLSD_COMBINE_LIMIT_III)).second->SetUse(TRUE);
		(*vecLsdLoadFactor.find(eLSD_COMBINE_LIMIT_V)).second->SetUse(TRUE);
		(*vecLsdLoadFactor.find(eLSD_COMBINE_EXTREME_II)).second->SetUse(TRUE);
		(*vecLsdLoadFactor.find(eLSD_COMBINE_SERVICE_I)).second->SetUse(TRUE);
	}
	else
	{
		BOOL bCheck[5] = {FALSE,FALSE,FALSE,FALSE,FALSE};		// A~E 등급
		CLsdDesignRatingCtl * pDesignRatingCtl = &(m_plsdEnvRating->GetLsdDesignRatingCtl());
		for (long ix=0; ix<pDesignRatingCtl->GetMaterialGradeSize(); ++ix)
		{
			long nIndex = pDesignRatingCtl->GetMaterialGrade(ix);
			if(nIndex<0 || nIndex>4) continue;
			bCheck[nIndex] = TRUE;
		}

		// 기본 권고안
		(*vecLsdLoadFactor.find(eLSD_COMBINE_LIMIT_I)).second->SetUse(TRUE);
		(*vecLsdLoadFactor.find(eLSD_COMBINE_EXTREME_I)).second->SetUse(TRUE);
		(*vecLsdLoadFactor.find(eLSD_COMBINE_SERVICE_I)).second->SetUse(TRUE);
		// 설계등급에 의한 권고안
		if(bCheck[2] || bCheck[3])
		{
			(*vecLsdLoadFactor.find(eLSD_COMBINE_SERVICE_III)).second->SetUse(TRUE);
			(*vecLsdLoadFactor.find(eLSD_COMBINE_SERVICE_IV)).second->SetUse(TRUE);
		}
		if(bCheck[4])
		{
			(*vecLsdLoadFactor.find(eLSD_COMBINE_SERVICE_V)).second->SetUse(TRUE);
		}

		// (ARCBRIDGE-3232) 피로 검토 조합 제거 -> 피로활하중을 그대로 쓴다.
		// (*vecLsdLoadFactor.find(eLSD_COMBINE_STRESS)).second->SetUse(TRUE);
	}
}

void CRcBridgeApp::SetDataDefaultComboLSD( long nType, BOOL bForCantilever )
{
	ELSDLoadGroupKind eType = eLSD_COMBINATION_LIMIT;

	if(nType == 1)		eType = eLSD_COMBINATION_EXTREME;
	else if(nType == 2)	eType = eLSD_COMBINATION_SERVICE;
	else if(nType == 3)	eType = eLSD_COMBINATION_FATIGUE;

	if(bForCantilever)
	{
		m_plsdMngForCantilever->SetDataDefaultLimitCombineSet(eType, eLSD_LOAD_LOADING_DIR_BOTH, FALSE);
	}
	else
	{
		BOOL bBoxType = IsBoxType();
		CLsdDesignLoad *pLoadLH = m_plsdManager->GetDesignLoad(eLSD_LOADSELECT_EH);
		BOOL bSideLoad = FALSE;
		if(pLoadLH)
		{
			long nSeleOpt = m_plsdManager->GetDesignLoad(eLSD_LOADSELECT_EH)->GetIndexSelectOfType(bForCantilever ? eLSD_LOADCASE_DIR_PER : eLSD_LOADCASE_DIR_BRI);
			bSideLoad = (nSeleOpt == 0)? FALSE : TRUE;
		}

		for(long ixW=0; ixW<2; ++ixW)
		{
			BOOL bWater = (ixW == 1)? TRUE : FALSE;
			if(bWater && m_bApplyGroundWaterLevel == FALSE)
				continue;

			// 극한한계와 사용한계는 EH하중의 옵션에 따라 방향을 조합하고
			// 극단상황 에서는 '내진해석시 모델링방법' 옵션에따라 방향을 적용한다.
			for(long ixDir=0; ixDir<eLSD_LOAD_LOADING_DIR_COUNT; ++ixDir)
			{
				if(eType == eLSD_COMBINATION_EXTREME)
				{
					// 극단상황은 양방향 재하를 하지 않지만 조합 초기화를 위해 함수는 태워준다.
					// 함수에서 초기화하고 리턴한다.
					if(!bBoxType && m_nSelectModelingCountEQ == 0 && ixDir == eLSD_LOAD_LOADING_DIR_R)
						continue;
					if(!bBoxType && m_nSelectModelingCountEQ == 1 && ixDir == eLSD_LOAD_LOADING_DIR_L)
						continue;
				}
				else
				{
					if(ixDir == eLSD_LOAD_LOADING_DIR_L && bSideLoad == FALSE)
						break;
				}

				m_plsdManager->SetDataDefaultLimitCombineSet(eType, (ELSDLoadLoadingDir)ixDir, bWater);
			}
		}

		if(eType != eLSD_COMBINATION_EXTREME && eType != eLSD_COMBINATION_FATIGUE)
		{
			SetDataDefaultSelectComboForWingWallAndAps(eType);			
		}
	}		
}

void CRcBridgeApp::SetDataDefaultModifiedFactor( long nIdex, BOOL bForCantilever )
{
	CLsdManagerBridge *plsdManager = bForCantilever? m_plsdMngForCantilever : m_plsdManager;

	if(nIdex == -1)
		plsdManager->ClearLoadModified();

	double dEtaD[eLSD_COMBINE_TOTAL_COUNT]	= {1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00}; 
	double dEtaR[eLSD_COMBINE_TOTAL_COUNT]		= {1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00}; 
	double dEtaI[eLSD_COMBINE_TOTAL_COUNT]		= {1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00}; 

	CDoubleArray dArrFactor;

	for(long ixPart=0; ixPart<ePartCount; ++ixPart)
	{
		// 하중수정 계수는 전부다 나누지 않는다.
		if(IsUsePart((EPartOfBridge)ixPart, 0) == FALSE)
			continue;

		if(nIdex == -1 || nIdex == ixPart)
		{
			dArrFactor.RemoveAll();
			for (long ix=0; ix<eLSD_COMBINE_TOTAL_COUNT; ++ix)
			{
				dArrFactor.Add(dEtaD[ix]);
				dArrFactor.Add(dEtaR[ix]);
				dArrFactor.Add(dEtaI[ix]);
			}

			plsdManager->AddLoadModified(GetStringPartMerge((EPartOfBridge)ixPart), dArrFactor);
		}
	}
}

void CRcBridgeApp::SetDataDefaultLsdLoad()
{
	BOOL bLsd = m_pARcBridgeDataStd->IsLsdDesign();

	m_dDepthApplySubSid				= bLsd? 15 : 10;
	m_dLiveLoadAlways				= (m_pARcBridgeDataStd->IsSI() && m_pARcBridgeDataStd->IsRailDesign()) ? 3.50 :  1.00;
	m_dDeadLoadAlways				= (m_pARcBridgeDataStd->IsSI() && m_pARcBridgeDataStd->IsRailDesign()) ? 1.50 :  0.00;
	m_nSelectKindCoefficientKo		= 0;
	m_nSelectInteriorWaterPress		= 0;
	m_dHeightInteriorWaterPress		= 0;
	m_dLiveLoadFootway				= 0.500;
	m_dTemperatureChangeLoadGap		= bLsd? 20 : 5;
	m_dTemperatureSlopeT[0] = 23;
	m_dTemperatureSlopeT[1] = 6;
	m_dTemperatureSlopeT[2] = 0;
	m_nApplyPavement = 0;

	m_nLoadEffeciveW = 0;
	m_nLoadEquiStrip = 0;
	m_bLoadMultiRoadSurConfficient = FALSE;
	m_nLoadStandardLane = 0;

	m_bUnderGroundDesignHeight = TRUE;
	m_dUnderGroundDesignHeight = CalcUnderGroundDesignHeight();
	m_nUnderGroundDistribute = 1;
	m_bCorrectionFactor_LiveLoadAlways = bLsd;
	m_dCorrectionFactor_LiveLoadAlways = bLsd ? 1.33 : 1.00;
}

double CRcBridgeApp::GetHeightBridgeWall( BOOL bStt )
{
	double dSta = m_dStationBridgeStt;
	if(bStt == FALSE)
		dSta += m_dLengthBridge;

	double dELLowerFoot = GetElevationFootingLower(dSta, FALSE);
	double dELTopSlab = m_pLineApp->GetElevationPlanEl(dSta);
	
	return dELTopSlab - dELLowerFoot;
}

HGDOUBLE CRcBridgeApp::CalcUnderGroundDesignHeight()
{
	CARcBridgeDataStd *pStd	= m_pARcBridgeDataStd;
	CLineInfo* pLine = pStd->GetLineApp();
	
	const double dSttBri = m_dStationBridgeStt;
	const double dEndBri = dSttBri + m_dLengthBridge;

	CDPoint dpStt(dSttBri, frM(m_dUnderGroundCenterELStt));
	CDPoint dpEnd(dEndBri, frM(m_dUnderGroundCenterELEnd));
	HGINT32 nSign = m_dUnderGroundCenterELStt > m_dUnderGroundCenterELEnd ? -1 : 1;

	HGDOUBLE dDesignHeight = 0;
	for (HGDOUBLE dSta = dSttBri; dSta <= dEndBri; dSta += frM(1))
	{
		dSta = min(dEndBri, dSta);

		HGDOUBLE dH = tan((dpEnd - dpStt).GetAngleRadian()) * (dSta - dSttBri);
		HGDOUBLE dELPlan = pLine->GetElevationPlanEl(dSta);
		HGDOUBLE dGap = dELPlan - (frM(m_dUnderGroundCenterELStt) + (dH * nSign));

		dDesignHeight = max(dDesignHeight, dGap);
	}

	dDesignHeight = max(dDesignHeight, 0.);

	return dDesignHeight;
}

HGDOUBLE CRcBridgeApp::CalcUnderGroundDesignHeightForWingWall(BOOL bStt, BOOL bLeft)
{
	if(m_bUnderGroundRahmen == FALSE)
		return 0;

	// 시점 날개벽의 좌우측은 선형진행방향 기준 좌우측의 반대다.
	if(bStt)
		bLeft = !bLeft;

	double dStaJijum = bStt? m_dStationBridgeStt : (m_dStationBridgeStt + m_dLengthBridge);
	HGBOOL bExp = IsExp(dStaJijum);

	CTwinVectorArray tvArrNori, tvArrSlab;
	GetTvCrossSectionUnderNori(tvArrNori, dStaJijum, CDPoint(0, 1), bLeft);
	GetTvCrossSection(tvArrSlab, dStaJijum, CDPoint(0, 1), FALSE, FALSE, FALSE, bLeft? -1 : 1, FALSE, FALSE, FALSE, bExp);

	CRebarPlacing rb;
	CDPoint xyPosEnd = tvArrNori.GetXyBottom();

	double dHeight(0);
	CDPointArray xyArrMatch;
	rb.GetXyMatchTvArrAndLine(tvArrSlab, xyPosEnd, CDPoint(0, 1), xyArrMatch);
	if(xyArrMatch.GetSize() > 0)
	{
		CDPoint xyMatch = rb.GetXyEdgePoint(xyArrMatch, UPRIGHT_AREA);
		dHeight = xyPosEnd.y - xyMatch.y;
	}

	return dHeight;
}

void CRcBridgeApp::SetDataDefaultBaseGround(HGINT32 ixJijum, BOOL bInit)
{
	if( GetCountJijum() <= ixJijum ) return;

	HGINT32 nJijumStt = ixJijum;
	HGINT32 nJijumEnd = ixJijum;
	if( ixJijum == -1 )
	{
		nJijumEnd = GetCountJijum() - 1;
		nJijumStt = 0;
	}

	for (HGINT32 nJijum = nJijumStt; nJijum <= nJijumEnd; ++nJijum)
	{
		CFootingApp *pFooting = GetFootingByJijumNumber(nJijum); 
		CBoringData *pBoring = GetBoringDataJijumBase(nJijum);

		if (pFooting == NULL || pBoring == NULL)
			continue;

		if(bInit)
			pFooting->SetDataDefaultFootingGround();

		if(m_pARcBridgeDataStd->IsLsdDesign())
		{
			pFooting->m_nTypeBaseGround = pBoring->m_nTypeSoil;
			if(pFooting->m_nTypeBaseGround < 2)
			{
				pFooting->m_nTypeEquation_Qa = 0;
			}
		}
		else
		{
			pFooting->m_nTypeBaseGround = pBoring->GetBoringType();	// 타입
		}
		pFooting->m_C_BaseGround = pBoring->m_c;					// 점착력
		pFooting->m_dNValueBaseGround = pBoring->m_nNValue;		// 기초지반의 N치
		pFooting->m_dBoringEo = pBoring->m_dEo;					// 지반변형 계수
		pFooting->m_dBoringRatePoasong = pBoring->m_RatePoasong;	// 지반 포아송비
		if(bInit)
		{
			pFooting->m_dNValue_Eo_Hori = 28;
			pFooting->m_dNValue_Eo_Vert = 28;
			pFooting->m_dEo_Input_Hori = 0;
		}
	}
}


long CRcBridgeApp::GetOutValueForFootingLoadCase_Lsd( EFootingCheckLsd eCheck, BOOL bMinFind, long nLoadCaseMax, long nLoadCaseMin, CJoint *pJoint, double &dMoment, double &dShear, double &dAxial)
{
	dMoment = 0; dShear = 0; dAxial = 0;

	if(pJoint == NULL) return -1;

	CResultForce *pResultMax = nLoadCaseMax<0 ? NULL : pJoint->GetResultForce(nLoadCaseMax);
	CResultForce *pResultMin = nLoadCaseMin<0 ? NULL : pJoint->GetResultForce(nLoadCaseMin);
	
	if(pResultMax == NULL && pResultMin == NULL)
		return -1;
	else if(pResultMin == NULL)
		pResultMin = pResultMax;
	else if(pResultMax == NULL)
		pResultMax = pResultMin;

	// SAP인경우만 스프링반력을 적용, MIDAS는 지점반력에서 모두 표현됨...
	BOOL bSpringForce = (pJoint->IsFix()==FALSE && (m_nSelect2DFrameTool==SOLVE_SAP_6AND7 || m_nSelect2DFrameTool==SOLVE_SAP_8)) ? TRUE : FALSE;

	if(m_nSelectFootingReactionForce == 0)
	{
		// Case별로 집계
		BOOL bResultMax = TRUE;
		if(eCheck==eFootCheckAxial)
		{
			if(bSpringForce) bResultMax = (fabs(pResultMax->m_SpringForce.z) > fabs(pResultMin->m_SpringForce.z))? TRUE : FALSE;
			else bResultMax = (fabs(pResultMax->m_Force.z) > fabs(pResultMin->m_Force.z))? TRUE : FALSE;
		}
		else if(eCheck == eFootCheckHoriMax || eCheck == eFootCheckHoriMin)
		{
			if(bSpringForce) bResultMax = bMinFind == (pResultMax->m_SpringForce.x < pResultMin->m_SpringForce.x)? TRUE : FALSE;
			else bResultMax = bMinFind == (pResultMax->m_Force.x < pResultMin->m_Force.x)? TRUE : FALSE;
		}
		else
		{
			if(bSpringForce) bResultMax = (pResultMax->m_SpringMoment.y > pResultMin->m_SpringMoment.y)? TRUE : FALSE;
			else bResultMax = (pResultMax->m_Moment.y > pResultMin->m_Moment.y)? TRUE : FALSE;

			if(bMinFind) bResultMax = !bResultMax;
		}

		if(bSpringForce)
		{
			dAxial  = bResultMax? pResultMax->m_SpringForce.z : pResultMin->m_SpringForce.z;
			dShear  = bResultMax? pResultMax->m_SpringForce.x : pResultMin->m_SpringForce.x;
			dMoment = bResultMax? pResultMax->m_SpringMoment.y : pResultMin->m_SpringMoment.y;
		}
		else
		{
			dAxial  = bResultMax? pResultMax->m_Force.z : pResultMin->m_Force.z;
			dShear  = bResultMax? pResultMax->m_Force.x : pResultMin->m_Force.x;
			dMoment = bResultMax? pResultMax->m_Moment.y : pResultMin->m_Moment.y;
		}
	}
	else if(m_nSelectFootingReactionForce == 1)
	{
		// 부호별로 집계
		BOOL bResultMax = TRUE;
		if(eCheck==eFootCheckAxial)
		{
			if(bSpringForce) bResultMax = (fabs(pResultMax->m_SpringForce.z) > fabs(pResultMin->m_SpringForce.z))? TRUE : FALSE;
			else bResultMax = (fabs(pResultMax->m_Force.z) > fabs(pResultMin->m_Force.z))? TRUE : FALSE;
		}
		else if(eCheck == eFootCheckHoriMax || eCheck == eFootCheckHoriMin)
		{
			if(bSpringForce) bResultMax = bMinFind == (pResultMax->m_SpringForce.x < pResultMin->m_SpringForce.x)? TRUE : FALSE;
			else bResultMax = bMinFind == (pResultMax->m_Force.x < pResultMin->m_Force.x)? TRUE : FALSE;
		}
		else
		{
			if(bSpringForce) bResultMax = (pResultMax->m_SpringMoment.y > pResultMin->m_SpringMoment.y)? TRUE : FALSE;
			else bResultMax = (pResultMax->m_Moment.y > pResultMin->m_Moment.y)? TRUE : FALSE;

			if(bMinFind) bResultMax = !bResultMax;
		}

		// Case별로 집계방법과 동일하게 Max, min을 찾고 수평력만 모멘트와 같은 부호로 찾아주자.

		if(bSpringForce)
		{
			dAxial  = bResultMax? pResultMax->m_SpringForce.z : pResultMin->m_SpringForce.z;
			dMoment = bResultMax? pResultMax->m_SpringMoment.y : pResultMin->m_SpringMoment.y;
			if(dMoment > 0) dShear  = max(pResultMax->m_SpringForce.x, pResultMin->m_SpringForce.x);
			else dShear  = min(pResultMax->m_SpringForce.x, pResultMin->m_SpringForce.x);
		}
		else
		{
			dAxial  = bResultMax? pResultMax->m_Force.z : pResultMin->m_Force.z;
			dMoment = bResultMax? pResultMax->m_Moment.y : pResultMin->m_Moment.y;
			if(dMoment > 0) dShear  = max(pResultMax->m_Force.x, pResultMin->m_Force.x);
			else dShear  = min(pResultMax->m_Force.x, pResultMin->m_Force.x);
		}
	}
	else if(m_nSelectFootingReactionForce == 2 || m_nSelectFootingReactionForce == 3)
	{
		// MAX,MIN의 절대값이 큰 것들을 조합해서 사용
		// (ARCBRIDGE-2616, ARCBRIDGE-2034) 수평력은 모멘트 부호와 맞춰 줘야한다.
		if(bSpringForce)
		{
			// 축력최대 케이스를 제외하고는 작은값
			// m_nSelectFootingReactionForce == 3 일때는 큰값
			if(eCheck==eFootCheckAxial || m_nSelectFootingReactionForce == 3)
				dAxial  = max(fabs(pResultMax->m_SpringForce.z),fabs(pResultMin->m_SpringForce.z));
			else
				dAxial  = min(fabs(pResultMax->m_SpringForce.z),fabs(pResultMin->m_SpringForce.z));

			dShear  = pResultMax->m_SpringForce.x;
			dMoment = pResultMax->m_SpringMoment.y;

			if(pResultMax!=pResultMin)
			{
				if(eCheck==eFootCheckAxial)
				{
					if(fabs(pResultMax->m_SpringMoment.y)<fabs(pResultMin->m_SpringMoment.y))
						dMoment = pResultMin->m_SpringMoment.y;
				}
				else if(eCheck == eFootCheckHoriMax || eCheck == eFootCheckHoriMin)
				{
					if(bMinFind == (pResultMax->m_SpringForce.x > pResultMin->m_SpringForce.x))
					{
						dShear  = pResultMin->m_SpringForce.x;
						// 모멘트를 수평력부호에 맞춰준다. 수평력부호는 모멘트 부호와 같겠지...
						dMoment = pResultMin->m_SpringMoment.y;
					}
				}
				else if(bMinFind && pResultMax->m_SpringMoment.y > pResultMin->m_SpringMoment.y)
				{
					dMoment = pResultMin->m_SpringMoment.y;
				}

				if(eCheck != eFootCheckHoriMax && eCheck != eFootCheckHoriMin)
				{
					// 선택된 모멘트 부호와 맞춰준다.
					if(dMoment > 0 != (pResultMax->m_SpringForce.x > pResultMin->m_SpringForce.x))
						dShear  = pResultMin->m_SpringForce.x;
				}
			}
		}
		else
		{
			// 축력최대 케이스를 제외하고는 작은값
			if(eCheck==eFootCheckAxial || m_nSelectFootingReactionForce == 3)
				dAxial  = max(fabs(pResultMax->m_Force.z),fabs(pResultMin->m_Force.z));
			else
				dAxial  = min(fabs(pResultMax->m_Force.z),fabs(pResultMin->m_Force.z));

			dShear  = pResultMax->m_Force.x;
			dMoment = pResultMax->m_Moment.y;

			if(pResultMax!=pResultMin)
			{
				if(eCheck==eFootCheckAxial)
				{
					if(fabs(pResultMax->m_Moment.y)<fabs(pResultMin->m_Moment.y))
					{
						dMoment = pResultMin->m_Moment.y;
					}
				}
				else if(eCheck == eFootCheckHoriMax || eCheck == eFootCheckHoriMin)
				{
					if(bMinFind == (pResultMax->m_Force.x > pResultMin->m_Force.x))
					{
						dShear  = pResultMin->m_Force.x;
						// 모멘트를 수평력부호에 맞춰준다. 수평력부호는 모멘트 부호와 같겠지...
						dMoment = pResultMin->m_Moment.y;
					}
				}
				else if(bMinFind && pResultMax->m_Moment.y > pResultMin->m_Moment.y)
				{
					dMoment = pResultMin->m_Moment.y;
				}

				if(eCheck != eFootCheckHoriMax && eCheck != eFootCheckHoriMin)
				{
					// 선택된 모멘트 부호와 맞춰준다.
					if(dMoment > 0 != (pResultMax->m_Force.x > pResultMin->m_Force.x))
						dShear  = pResultMin->m_Force.x;
				}
			}
		}
	}

	return 0;
}

long CRcBridgeApp::GetOutValueForFootingDesign_Lsd( long nJijum, ETypeEnvComboLsd eEnvTypeLsd, EFootingCheckLsd eCheck, double &dMoment, double &dShear, double &dAxial, long &nIdxModel, long nJijum_Add/*=-1*/, long nJijumReal/*=-1*/ )
{
	long nTypeDesignStt(0), nTypeDesignEnd(0);
	long nSizeCombo(0);
	EPartOfBridge ePart(ePartFootEnd);
	ELSDCombKind eKindCombo(eLSD_COMBINE_LIMIT_I);
	BOOL bEarthQuake = (eEnvTypeLsd==eEnvExtremeI || eEnvTypeLsd==eEnvExtremeII);
	// (ARCBRIDGE-2784) BOX타입일때랑 독립기초일때랑 모델 인덱스가 다르다.
	long idxModelEq = IsBoxType()? 0 : 1;
	CFEMManage *pFEM = bEarthQuake ? &m_FemEQ[idxModelEq] : &m_FemNormal;

	if(bEarthQuake)
	{
		nTypeDesignStt = COMBO_EARTHQUAKE;
		// nTypeDesignEnd = (m_nSelectModelingCountEQ==2)? COMBO_EARTHQUAKE2 : nTypeDesignStt;
		nTypeDesignEnd = COMBO_EARTHQUAKE + GetCountEarthQuakeModel() - 1;
	}
	else
	{
		nTypeDesignStt = (eEnvTypeLsd==eEnvService || eEnvTypeLsd==eEnvService_Crack) ? COMBO_SERVICE : COMBO_ULTIMATE;
		nTypeDesignEnd = nTypeDesignStt;
	}

	if(nJijum == 0)
		ePart = ePartFootStt;
	else if(nJijum == m_nQtyJigan)
		ePart = ePartFootEnd;
	else
		ePart = ePartInFoot;
	eKindCombo = GetComboKindByEnvType(eEnvTypeLsd, ePart, CHECK_POS_CENTER);

	if(pFEM->IsOutputLoaded()==FALSE) return -1;
	
	long nComboFind(0);
	double dCurValue = 0;
	double dMaxValue = 0;
	double dMinValue = 0;
	double dCurValue_sub = 0;
	double dMaxMinValue_sub = 0;
	double dCaseM(0), dCaseS(0), dCaseA(0);
	CJoint *pJointFind(NULL);
	
	// bMinFind를 실제 지점번호로 판단.. 아치에서 바꿔서 온 경우 잘못됨
	long nJijumMinCheck = (nJijumReal==-1) ? nJijum : nJijumReal;
	
	BOOL bMinFind = (eCheck== eFootCheckAxial || 
		(eCheck== eFootCheckHoriMin && nJijumMinCheck == 0) ||
		(eCheck== eFootCheckHoriMax && nJijumMinCheck == m_nQtyJigan) ||
		(nJijumMinCheck<m_nQtyJigan  && eCheck==eFootCheckMomentPlus) || 
		(nJijumMinCheck==m_nQtyJigan && eCheck==eFootCheckMomentMinus) ) ? FALSE : TRUE;

	// 축력 최대일때는 모멘트가 큰 조합, 나머지는 축력이 작은 조합
	BOOL bMinFind_sub = (eCheck == eFootCheckAxial)? FALSE : TRUE;

	BOOL bFirst   = TRUE;
	// 축력최대시   : 최대축력   + 최대모멘트(절대값)의 조합으로
	// 모멘트최대시 : 최대모멘트 + 최대축력
	long nLoadCaseMax(0), nLoadCaseMin(0);
	for(long nTypeDesign=nTypeDesignStt; nTypeDesign<=nTypeDesignEnd; nTypeDesign++)
	{
		pFEM = GetFemManage(nTypeDesign);
		idxModelEq = IsBoxType() ? (nTypeDesign - COMBO_EARTHQUAKE) : (nTypeDesign - COMBO_EARTHQUAKE + 1);

		long nElement = GetNumberElementWallEndCheck(nJijum,nTypeDesign,FALSE,FALSE);
		if(bEarthQuake && GetTypeJointPosFootingEQ()==1)
		{
			if(m_nTypeBridge == BRIDGE_TYPE_ARCH && nJijum == m_nQtyJigan+1) nElement--;
			else nElement++;
		}
		CString szCombo = _T("");
		CElement *pEle = pFEM->GetElement((USHORT)nElement);
		if(!pEle) continue;

		CJoint *pJoint, *pJoint_Add;
		if(nJijum == m_nQtyJigan+1) pJoint= pFEM->GetJoint(pEle->m_nJStt);		// Arch리브의 시점측은 지점즉이 부재의 시작부분이다.
		else pJoint= pFEM->GetJoint(pEle->m_nJEnd);
		//////////////// TRAP2, ARCH형의 기초는 연결된 지점의 반력을 고려해서 최대값을 찾는다.
		if(nJijum_Add > -1)
		{
			nElement = GetNumberElementWallEndCheck(nJijum_Add,nTypeDesign,FALSE,FALSE);
			if(bEarthQuake && GetTypeJointPosFootingEQ()==1)
			{
				if(m_nTypeBridge == BRIDGE_TYPE_ARCH && nJijum_Add == m_nQtyJigan+1) nElement--;
				else nElement++;
			}
			if(nElement < 0) return 0;
			pEle = pFEM->GetElement((USHORT)nElement);
			if(nJijum_Add == m_nQtyJigan+1) pJoint_Add= pFEM->GetJoint(pEle->m_nJStt);		// Arch리브의 시점측은 지점즉이 부재의 시작부분이다.
			else pJoint_Add= pFEM->GetJoint(pEle->m_nJEnd);
		}
		
		CString szComboStt(_T(""));
		nSizeCombo = GetSizeComboByType(eEnvTypeLsd, ePart, szComboStt);
		long nLoadCase = pFEM->GetLoadCaseByString(szComboStt + _T(" MAX")) - 1;
		for(long nCombo=0; nCombo<nSizeCombo; nCombo++)
		{
			// (ARCBRIDGE-3321) m_nSelectFootingReactionForce 옵션에 따라 Max, Min 조합을 같이 확인 한다. 세번을 따로 반복할 필요 없다.
			//for(long nMin=0; nMin<3; nMin++)
			{
				// Max, Min Case를 조합해서 집계하는 경우 MinCase를 따로 하지 않는다.
				// if(m_nSelectFootingReactionForce > 0 && nMin > 1)
				//	continue;

				szCombo = GetStringComboEta_Lsd(eKindCombo, ePart, nCombo+1);
				
				BOOL bMaxMin(FALSE);
				nLoadCase = pFEM->GetLoadCaseByString(szCombo);
				if(nLoadCase < 0)
				{
					szCombo += _T(" MAX");
					nLoadCase = pFEM->GetLoadCaseByString(szCombo);
					bMaxMin = TRUE;
				}
				
				if(nLoadCase < 0) continue;
				
				long nLoadCase1 = nLoadCase;
				long nLoadCase2 = bMaxMin ? nLoadCase + 1 : -1;

				long nResult = GetOutValueForFootingLoadCase_Lsd(eCheck, bMinFind, nLoadCase1, nLoadCase2, pJoint, dCaseM, dCaseS, dCaseA);
				if(nResult == -1)
					continue;
				
				switch(eCheck)
				{
				case eFootCheckAxial:
					dCurValue = dCaseA;
					dCurValue_sub = dCaseM;	break;
				case eFootCheckHoriMax:
				case eFootCheckHoriMin:
					dCurValue = dCaseS;
					dCurValue_sub = dCaseA;	break;
				default:
					dCurValue = dCaseM;
					dCurValue_sub = dCaseA;	break;
				}
				////////////////
				if(nJijum_Add > -1)
				{
					long nResult = GetOutValueForFootingLoadCase_Lsd(eCheck, bMinFind, nLoadCase1, nLoadCase2, pJoint_Add, dCaseM, dCaseS, dCaseA);

					if(nResult != -1)
					{
						switch(eCheck)
						{
						case eFootCheckAxial:
							dCurValue += dCaseA;
							dCurValue_sub += dCaseM;	break;
						case eFootCheckHoriMax:
						case eFootCheckHoriMin:
							dCurValue += dCaseS;
							dCurValue_sub += dCaseA;	break;
						default:
							dCurValue += dCaseM;
							dCurValue_sub += dCaseA;	break;
						}
					}
				}
				////////////////

				if(bFirst || (bMinFind && Compare(dCurValue, dMinValue, _T("<"), 0.001)) || (bMinFind && Compare(dCurValue, dMinValue, _T("="), 0.001) && (bMinFind_sub == (fabs(dCurValue_sub) < fabs(dMaxMinValue_sub)))))
				{
					dMinValue  = dCurValue;
					dMaxMinValue_sub = dCurValue_sub;
					nComboFind = nCombo;

					pJointFind = pJoint;
					nLoadCaseMax = nLoadCase1;
					nLoadCaseMin = nLoadCase2;

					nIdxModel = bEarthQuake ? idxModelEq : 0;
				}
				if(bFirst || (!bMinFind && Compare(dCurValue, dMaxValue, _T(">"), 0.001)) || (!bMinFind && Compare(dCurValue, dMaxValue, _T("="), 0.001) && (bMinFind_sub == (fabs(dCurValue_sub) < fabs(dMaxMinValue_sub)))))
				{
					dMaxValue  = dCurValue;
					dMaxMinValue_sub = dCurValue_sub;
					nComboFind = nCombo;

					pJointFind = pJoint;
					nLoadCaseMax = nLoadCase1;
					nLoadCaseMin = nLoadCase2;
					
					nIdxModel = bEarthQuake ? idxModelEq : 0;
				}

				bFirst = FALSE;
			}
		}
	}

	double Station   = GetStationOnJijum(0,0);
	double SlabWidth = GetWidthSlabAct(Station,CDPoint(0,1));	// 슬래브폭원(교량폭,벽체폭)
	double dDivide   = (bEarthQuake) ? SlabWidth : 1.00;
	// 종점기초는 기준점이 반대쪽이이므로 방향을 반대로 돌려준다.
	// 경사기초2는 끝에서 두번째 지점도. 아치교도 적용
	// (ARCBRIDGE-1532) 경사기초1의 끝에서 두번째 지점도 반대로 적용해줘야 한다.
	double dSwValue  = (nJijum<m_nQtyJigan) ? -1.0 : 1.0;
	if(((m_nTypeBridge==BRIDGE_TYPE_TRAP2 || m_nTypeBridge==BRIDGE_TYPE_TRAP) && nJijum==m_nQtyJigan-1))
		dSwValue  = 1.0;
	if(m_nTypeBridge==BRIDGE_TYPE_ARCH && nJijum>m_nQtyJigan)
	{
		dSwValue = (nJijum == m_nQtyJigan+1)? -1.0 : 1.0;
	}

	GetOutValueForFootingLoadCase_Lsd(eCheck, bMinFind, nLoadCaseMax, nLoadCaseMin, pJointFind, dMoment, dShear, dAxial);

	dMoment = dMoment*dSwValue/dDivide;
	dShear  = dShear*dSwValue/dDivide;
	dAxial  = dAxial/dDivide;

	return nComboFind;
}

long CRcBridgeApp::GetOutValueForWallDesing_Lsd( long nJijum, ETypeEnvComboLsd eEnvTypeLsd, EWallCheckLsd eCheck, double &dMoment, double &dShear, double &dAxial, long &nIdxModel, BOOL bApplyCtc )
{
	long nTypeDesignStt(0), nTypeDesignEnd(0);
	BOOL bEarthQuake = (eEnvTypeLsd==eEnvExtremeI || eEnvTypeLsd==eEnvExtremeII);
	long idxModelEq = IsBoxType()? 0 : 1;
	CFEMManage *pFEM = bEarthQuake ? &m_FemEQ[idxModelEq] : &m_FemNormal;

	EPartOfBridge ePart(ePartFootEnd);
	if(nJijum == 0)
		ePart = ePartOutWallStt;
	else if(nJijum == m_nQtyJigan)
		ePart = ePartOutWallEnd;
	else
		ePart = ePartInWall;

	ELSDCombKind eKindCombo = GetComboKindByEnvType(eEnvTypeLsd, ePart, CHECK_POS_CENTER);

	if(bEarthQuake)
	{
		nTypeDesignStt = COMBO_EARTHQUAKE;
		// nTypeDesignEnd = (m_nSelectModelingCountEQ==2)? COMBO_EARTHQUAKE2 : nTypeDesignStt;
		nTypeDesignEnd = COMBO_EARTHQUAKE + GetCountEarthQuakeModel() - 1;
	}
	else
	{
		nTypeDesignStt = (eEnvTypeLsd==eEnvService || eEnvTypeLsd==eEnvService_Crack) ? COMBO_SERVICE : COMBO_ULTIMATE;
		nTypeDesignEnd = nTypeDesignStt;
	}

	dMoment	= 0; 
	dShear	= 0;
	dAxial	= 0;
	nIdxModel = 0;

	if(pFEM->IsOutputLoaded()==FALSE) return -1;

	CElement *pEle = NULL;
	CResultForceElement *pResultMax = NULL;
	CResultForceElement *pResultMin = NULL;

	CString szComboFirst(_T(""));
	long nSizeUlt = GetSizeComboByType(eEnvTypeLsd, ePart, szComboFirst);

	CString szCombo = _T("");
	
	double dAxialMax,dAxialMin,dMomentMax,dMomentMin,dShearMax,dShearMin;
	double dAxialCur,dMomentCur,dShearCur;
	long nComboFind(-1);
	long nSizeEle = pFEM->GetElementSize();

	long nFindEleType(0);
	if(nJijum == 0)
		nFindEleType = ELEMENT_STTWALL;
	else if(nJijum == m_nQtyJigan)
		nFindEleType = ELEMENT_ENDWALL;
	else
		nFindEleType = ELEMENT_MIDWALL;

	BOOL bMaxAxial	= (eCheck == eWallCheckAxialMax)? TRUE : FALSE;
	BOOL bMaxShear	= (eCheck == eWallCheckAxialMin)? FALSE : TRUE;

	for(long nTypeDesign=nTypeDesignStt; nTypeDesign<=nTypeDesignEnd; nTypeDesign++)
	{
		pFEM = GetFemManage(nTypeDesign);
		idxModelEq = IsBoxType() ? (nTypeDesign - COMBO_EARTHQUAKE) : (nTypeDesign - COMBO_EARTHQUAKE + 1);
		
		for(long ix=0; ix<nSizeUlt; ix++)
		{
			szCombo = GetStringComboEta_Lsd(eKindCombo, ePart, ix+1);

			long nLoadCase = pFEM->GetLoadCaseByString(szCombo);
			if(nLoadCase < 0)
			{
				szCombo += _T(" MAX");
				nLoadCase = pFEM->GetLoadCaseByString(szCombo);
				if(nLoadCase < 0) continue;
			}

			BOOL bFirst(TRUE);
			for(long ixEle=0; ixEle<nSizeEle; ++ixEle)
			{
				pEle = pFEM->GetElement((USHORT)ixEle);

				if(pEle->m_nFlag != nFindEleType)
					continue;
				
				if(pEle->m_LongAttrib[LONG_1_ELEMENT_JIJUMNO] != nJijum)
					continue;

				// (ARCBRIDGE-2833) 강역부재는 제외한다.
				double dLen = ~(pFEM->GetJoint(pEle->m_nJEnd)->GetPoint() - pFEM->GetJoint(pEle->m_nJStt)->GetPoint());
				if(pEle->m_dIoff + pEle->m_dJoff >= dLen)
					continue;

				pResultMax  = pEle->GetResultForce(nLoadCase);
				pResultMin  = pEle->GetResultForce(nLoadCase+1);
				if(pResultMax == NULL || pResultMin == NULL) continue;

				long icPoint = pResultMax->m_ResultForceElement.GetSize();
				for(long ixPoint=0; ixPoint<icPoint; ++ixPoint)
				{
					dAxialMax  = fabs(pResultMax->m_ResultForceElement[ixPoint]->m_dForceOnElement[ELE_AXFORCE]);
					dAxialMin  = fabs(pResultMin->m_ResultForceElement[ixPoint]->m_dForceOnElement[ELE_AXFORCE]);
					dMomentMax = fabs(pResultMax->m_ResultForceElement[ixPoint]->m_dForceOnElement[ELE_MOMENT2]);
					dMomentMin = fabs(pResultMin->m_ResultForceElement[ixPoint]->m_dForceOnElement[ELE_MOMENT2]);
					dShearMax = fabs(pResultMax->m_ResultForceElement[ixPoint]->m_dForceOnElement[ELE_SHEAR_2]);
					dShearMin = fabs(pResultMin->m_ResultForceElement[ixPoint]->m_dForceOnElement[ELE_SHEAR_2]);

					// (ARCBRIDGE-2764) BOX타입에서는 중간지점 기둥에 축력이 0이 나오는 경우가 있으므로 그럴땐 Max
					double dAxialMaxMin = (bMaxAxial || dAxialMax == 0)? MAX(dAxialMax, dAxialMin) : MIN(dAxialMax, dAxialMin);
					double dMomentMaxMin = MAX(dMomentMax, dMomentMin);
					double dShearMaxMin = bMaxShear? MAX(dShearMax, dShearMin) : MIN(dShearMax, dShearMin);

					if(bFirst)
					{
						bFirst = FALSE;
						dAxialCur	= dAxialMaxMin;
						dMomentCur	= dMomentMaxMin;
						dShearCur	= dShearMaxMin;
					}
					else
					{
						if(bMaxAxial == (dAxialMaxMin > dAxialCur))
							dAxialCur = dAxialMaxMin;
						if(dMomentMaxMin > dMomentCur)
							dMomentCur = dMomentMaxMin;
						if(bMaxShear == (dShearMaxMin > dShearCur))
							dShearCur = dShearMaxMin;
					}
				}
			}

			BOOL bApplyCase(FALSE);
			if(eCheck == eWallCheckAxialMax)
			{
				if(dAxialCur>dAxial)
				{
					bApplyCase = TRUE;
				}
			}
			else if(eCheck == eWallCheckMomentMax)
			{
				if(dMomentCur>dMoment)
				{
					bApplyCase = TRUE;
				}
			}
			else if(eCheck == eWallCheckEccMax)
			{
				double eCur = dMomentCur/dAxialCur;
				if(eCur>(dMoment/dAxial))
				{
					bApplyCase = TRUE;
				}
			}
			else if(eCheck == eWallCheckHoriMax)
			{
				if(dShearCur>dShear)
				{
					bApplyCase = TRUE;
				}
			}
			else if(eCheck == eWallCheckAxialMin)
			{
				if(dAxialCur<dAxial)
				{
					bApplyCase = TRUE;
				}
			}

			if(bApplyCase || nComboFind==-1)
			{
				dAxial  = dAxialCur;
				dMoment = dMomentCur;
				dShear  = dShearCur;
				nComboFind = ix;
				nIdxModel = bEarthQuake ? idxModelEq : 0;
			}
		}
	}

	double Station   = GetStationOnJijum(0,0);
	double SlabWidth = GetWidthSlabAct(Station,CDPoint(0,1));	// 슬래브폭원(교량폭,벽체폭)
	double dDivide   = (bEarthQuake) ? SlabWidth : 1.00;
	double dCTC		= bApplyCtc? frM(GetCtcColumn(nJijum)) : 1.0;

	dMoment = dMoment/dDivide * dCTC;
	dShear  = dShear/dDivide * dCTC;
	dAxial  = dAxial/dDivide * dCTC;

	return nComboFind;
}

HGBOOL CRcBridgeApp::GetMinMaxStation(HGDOUBLE &dMinSta, HGDOUBLE &dMaxSta)
{
	CLineInfo *pLine	= GetLineBase();
	if (pLine == NULL)
		return FALSE;

	CDPoint A[4], B[4];
	GetXySss(BRIDGE_OFF_STT_STT, A);
	GetXySss(BRIDGE_OFF_END_END, B);

	CDPoint vAngStt = pLine->GetAngleAzimuth(m_dStationBridgeStt).Rotate90();
	CDPoint vAngEnd = pLine->GetAngleAzimuth(m_dStationBridgeStt + m_dLengthBridge).Rotate90();

	HGDOUBLE dSta1 = 0;
	HGDOUBLE dSta2 = 0;
	if (pLine->GetStationMatchByXyAndAngle(A[0], vAngStt, dSta1) == FALSE)
	{
		return FALSE;
	}

	if (pLine->GetStationMatchByXyAndAngle(A[3], vAngStt, dSta2) == FALSE)
	{
		return FALSE;
	}

	dMinSta = min(dSta1, dSta2);

	if (pLine->GetStationMatchByXyAndAngle(B[0], vAngEnd, dSta1) == FALSE)
	{
		return FALSE;
	}

	if (pLine->GetStationMatchByXyAndAngle(B[3], vAngEnd, dSta2) == FALSE)
	{
		return FALSE;
	}

	dMaxSta = max(dSta1, dSta2);

	return TRUE;
}

HGBOOL CRcBridgeApp::GetTvArrayLaneForPlane(HGINT32 ixHDan, CTwinVectorArray &tvArr, HGBOOL bRoadNotSlab)
{
	HGDOUBLE dStaMin(0), dStaMax(0);
	if (GetMinMaxStation(dStaMin, dStaMax) == FALSE)
		return FALSE;

	const HGBOOL bExp = FALSE;
	HGBOOL bFirst = TRUE;
	CDPoint dpBefore;
	CDPoint vAng(0,1);
	for (HGDOUBLE dSta = dStaMin; dSta <= dStaMax + 1000; dSta += 1000)
	{
		if (dSta > dStaMax)
			dSta = dStaMax;

		const HGBOOL bExp = IsExp(dSta);
		
		double disLeftBase = bExp ? GetDisSlabLeftActByGagak(dSta, vAng, bRoadNotSlab) : GetDisSlabLeftAct(dSta, vAng, bRoadNotSlab);
		double disHDan = 0;
		for (HGINT32 ix = 0; ix < ixHDan; ++ix)
		{
			disHDan += GetLengthHDanAct(dSta, vAng, ix, bRoadNotSlab, bExp);
		}

		const HGDOUBLE dDist = disLeftBase + disHDan;
		const CDPoint dpPoint = GetXyLaneForPlane(dSta, dDist);
		
		if (bFirst)
		{
			bFirst = FALSE;
			dpBefore = dpPoint;
		}
		else
		{
			CTwinVector tv;
			tv.m_v1 = dpBefore;
			tv.m_v2 = dpPoint;
			
			tvArr.Add(tv);

			dpBefore = dpPoint;
		}

		if (dSta == dStaMax)
			break;
	}

	TrimLaneNoriForPlane(tvArr);
	
	return TRUE;
}

CDPoint CRcBridgeApp::GetXyLaneForPlane(HGDOUBLE dSta, HGDOUBLE dDist)
{
	CDPoint xy;
	CLineInfo *pLine	= GetLineBase();
	if (pLine == NULL)
		return xy;

	xy = pLine->GetXyLineDis(dSta, dDist);

	return xy;
}

ETypeEnvComboLsd CRcBridgeApp::GetTypeEnvComboLsdFooting(HGINT32 ixKey, long nJijum)
{
	long nCntJijum = GetCountJijum();
	if( m_pARcBridgeDataStd->IsLsdDesign() == FALSE || nCntJijum < nJijum) 
		return eEnvLimit;

	ETypeEnvComboLsd eEnvType = static_cast<ETypeEnvComboLsd>(GetLsdEnvType(ixKey));

	return eEnvType;

	// 시점, 종점, 중간지점
// 	if( (nJijum==0 && ixKey < STM_MODEL_FOOT_STT_MAX_AXIAL) || (nJijum==0 && STM_MODEL_FOOT_STT_MIN_HORI_FORCE_EXT_2 < ixKey) ||						
// 		(nJijum==nCntJijum-1 && ixKey < STM_MODEL_FOOT_END_MAX_AXIAL) || (nJijum==nCntJijum-1 && STM_MODEL_FOOT_END_MIN_HORI_FORCE_EXT_2 < ixKey))
// 		return eEnvLimit;
// 
// 	// 극한한계
// 	if( (nJijum==0 && (STM_MODEL_FOOT_STT_MAX_AXIAL <= ixKey && ixKey <= STM_MODEL_FOOT_STT_MIN_HORI_FORCE)) ||
// 		(nJijum==nCntJijum-1 && (STM_MODEL_FOOT_END_MAX_AXIAL <= ixKey && ixKey <= STM_MODEL_FOOT_END_MIN_HORI_FORCE)))
// 		return eEnvLimit;
// 	else if( (nJijum==0 && (STM_MODEL_FOOT_STT_MAX_AXIAL_EXT_1 <= ixKey && ixKey <= STM_MODEL_FOOT_STT_MIN_HORI_FORCE_EXT_1)) || 
// 		(nJijum==nCntJijum-1 && (STM_MODEL_FOOT_END_MAX_AXIAL_EXT_1 <= ixKey && ixKey <= STM_MODEL_FOOT_END_MIN_HORI_FORCE_EXT_1)) )
// 		return eEnvExtremeI;
// 	else if( (nJijum==0 && (STM_MODEL_FOOT_STT_MAX_AXIAL_EXT_2 <= ixKey && ixKey <= STM_MODEL_FOOT_STT_MIN_HORI_FORCE_EXT_2)) ||
// 		(nJijum==nCntJijum-1 && (STM_MODEL_FOOT_END_MAX_AXIAL_EXT_2 <= ixKey && ixKey <= STM_MODEL_FOOT_END_MIN_HORI_FORCE_EXT_2)))
// 		return eEnvExtremeII;
// 
// 	return eEnvLimit;
}

// 기둥이 부담하는 가장 큰 값을 사용
double CRcBridgeApp::GetCtcColumn(long nJijum)
{
	double dCtcColumn = 1.0;

	if(GetTypeColumnByJijum(nJijum)==-1)
		return dCtcColumn;

	CWallApp *pWall = GetInWall(nJijum-1);
	if(pWall == NULL) return dCtcColumn;

	//	double dDistLeft=0.,dDistRight=0.;
	long i=0; for(i=0; i<pWall->m_nCountColumn; i++)
	{
		double dDistLeft  = GetDistColumnNext(nJijum,i,TRUE);
		double dDistRight = GetDistColumnNext(nJijum,i,FALSE);
		if(i>0)
			dDistLeft *= 0.5;
		if(i<pWall->m_nCountColumn-1)
			dDistRight *= 0.5;
		if((dDistLeft+dDistRight) > dCtcColumn)
		{
			dCtcColumn = (dDistLeft+dDistRight);
		}
	}

	return toM(dCtcColumn);
}

void CRcBridgeApp::GetOutValueSlabMaxForFatigue_Lsd( long nJigan, long nPos, double &Mmax, double &Mmin )
{
	CFEMManage *pFEM = &m_FemNormal;
	CElement *pEle =  NULL;

	long nIndexStt = GetNumberElementSlabSpanSttCheck(nJigan, 0, FALSE, FALSE, TRUE);
	long nIndexEnd = GetNumberElementSlabSpanEndCheck(nJigan, 0, FALSE, FALSE, TRUE);
	if(nIndexStt==-1 && nIndexEnd==-1) return ;

	// (ARCBRIDGE-3232) 피로 검토 조합 제거 -> 피로활하중을 그대로 쓴다.
	// CString szCombo = GetStringComboEnv_Lsd(eLSD_COMBINE_STRESS, ePartUpperSlab);
	CString szCombo = _T("LLFG");
	long nLoadCase = pFEM->GetLoadCaseByString(szCombo);
	if(nLoadCase < 0)
	{
		szCombo += _T(" MAX");
	}

	long nLoadCaseMax  = pFEM->GetLoadCaseIndex(szCombo);
	long nLoadCaseMin  = nLoadCaseMax + 1;

	if( nLoadCaseMax < 0 || nLoadCaseMin < 0 ) return;
	
	CResultForceElement *pResultMax = NULL;
	CResultForceElement *pResultMin = NULL;

	double dValueMax = 0;
	double dValueMin = 0;
	double dOutputMax = 0;
	double dOutputMin = 0;

	if(nPos==CHECK_POS_LEFT || nPos==CHECK_POS_RIGHT)
	{
		long nIdxEle = (nPos==0) ? nIndexStt : nIndexEnd;
		BOOL bSttPos = (nPos==0) ? TRUE : FALSE;
		pEle = pFEM->GetElement((USHORT)nIdxEle);
		pResultMax = pEle->GetResultForce(nLoadCaseMax);
		pResultMin = pEle->GetResultForce(nLoadCaseMin);

		long nPoint  = bSttPos ? 0 : pResultMax->m_ResultForceElement.GetSize()-1;
		dValueMax  = pResultMax->m_ResultForceElement[nPoint]->m_dForceOnElement[ELE_MOMENT2];
		dValueMin  = pResultMin->m_ResultForceElement[nPoint]->m_dForceOnElement[ELE_MOMENT2];
		dOutputMax = dValueMax;
		dOutputMin = dValueMin;
	}
	else
	{
		BOOL bFirst = TRUE;
		for(long nEle=nIndexStt; nEle<=nIndexEnd; nEle++)
		{
			pEle = pFEM->GetElement((USHORT)nEle);
			pResultMax = pEle->GetResultForce(nLoadCaseMax);
			pResultMin = pEle->GetResultForce(nLoadCaseMin);

			long nSizePoint = pResultMax->m_ResultForceElement.GetSize();
			long i=0; for(i=0; i<nSizePoint; i++)
			{
				dValueMax  = pResultMax->m_ResultForceElement[i]->m_dForceOnElement[ELE_MOMENT2];
				dValueMin  = pResultMin->m_ResultForceElement[i]->m_dForceOnElement[ELE_MOMENT2];
				if(bFirst)
				{
					dOutputMax = dValueMax;
					dOutputMin = dValueMin;
					bFirst = FALSE;
				}
				else if(dValueMax > dOutputMax)
				{
					// 모멘트 최대가 나오는 위치에서 MIn을 집계하자.
					dOutputMax = dValueMax;
					dOutputMin = dValueMin;
				}
			}
		}
	}

	// 검토폭 1m 적용
	Mmax = dOutputMax * frM(UNIT_METER);
	Mmin = dOutputMin * frM(UNIT_METER);
}

HGBOOL CRcBridgeApp::GetTvArrayNoriOutLineForPlane(CTwinVectorArray &tvArr, HGBOOL bLeft)
{
	HGDOUBLE dStaMin(0), dStaMax(0);
	if (GetMinMaxStation(dStaMin, dStaMax) == FALSE)
		return FALSE;

	CVector vBefore;
	CVector vAng(0,1);
	
	for (HGDOUBLE dSta = dStaMin; dSta < dStaMax + 1000; dSta += 1000)
	{
		if (dSta > dStaMax)
			dSta = dStaMax;

		CTwinVectorArray tvArrNori;
		GetTvCrossSectionUnderNori(tvArrNori, dSta, vAng, bLeft);
		if (tvArrNori.GetCount() < 1)
			continue;

		HGDOUBLE dDist = tvArrNori.GetAt(tvArrNori.GetCount() - 1).m_v2.x;
		
		CVector vPos = GetXyLaneForPlane(dSta, dDist);
		
		if (dSta != dStaMin)
			tvArr.Add(CTwinVector(vBefore, vPos));			

		vBefore = vPos;

		if (dSta == dStaMax)
			break;
	}

	TrimLaneNoriForPlane(tvArr);

	return TRUE;
}

void CRcBridgeApp::TrimLaneNoriForPlane(CTwinVectorArray & tvArr)
{
	CDPoint A[4], B[4];

	GetXySss(BRIDGE_OFF_STT_STT, A);
	GetXySss(BRIDGE_OFF_END_END, B);

	CRebarPlacing rb;
	rb.TrimTvArrayByLine(tvArr, A[0], A[3], FALSE, FALSE, FALSE);
	rb.TrimTvArrayByLine(tvArr, B[0], B[3], TRUE, FALSE, FALSE);
}

double CRcBridgeApp::GetCoefficientAccelerateEQ(EGoalOfEarthQuake eGoalEq)
{
	// 지반 가속도 계수
	// 내진등급과 지진구역계수에 의한 (위험도계수 x 지진구역계수) 값
	double A		 = 0;
	double dCoDanger = GetFactorCoDanger(eGoalEq);
	double dCoEarthq = (GetRegionEarthQuake()==0) ? 0.11 : 0.07;

	A = dCoDanger * dCoEarthq;

	return A;
}

double CRcBridgeApp::GetCoefficientRegionEQ()
{
	double dCoEarthq = 0;
	if(GetDegreeEarthQuake() == 0) 
	{
		dCoEarthq = 0.11;
	}
	if(GetDegreeEarthQuake() == 1) 
	{
		dCoEarthq = 0.070;
	}

	return dCoEarthq;
}

double CRcBridgeApp::GetCoefficientDangerEQ()
{
	double dCoDanger = 0;
	if(GetDegreeEarthQuake() == 0) 
	{
		dCoDanger = 1.40;
	}
	if(GetDegreeEarthQuake() == 1) 
	{
		dCoDanger = 1.0;
	}

	return dCoDanger;
}

BOOL CRcBridgeApp::IsCantileverDesign()
{
	BOOL bLsd = m_pARcBridgeDataStd->IsLsdDesign();
	return bLsd && (m_bCantilSlab[0] == TRUE || m_bCantilSlab[1] == TRUE);
}

void CRcBridgeApp::SetDataDefaultLsdSupportRebar()
{
	m_bCheckSupportWallDryingShrinkage = TRUE;
	m_bCheckSupportWallMainRate = FALSE;
	m_bCheckSupportFootDryingShrinkage = TRUE;
	m_bCheckSupportSlabDryingShrinkage = TRUE;
	m_bCheckSupportSlabByReqRebar = FALSE;
	m_bCheckSupportSlabMainRate = FALSE;
	m_bCheckSupportSlabMainRateSpan = FALSE;
}

// 확폭에의한 사보강 배치를 위한 횡단폭 차이를 구함.
double CRcBridgeApp::GetMaxDiffWidthByExtLine( BOOL bLeft )
{
	double dWidthDiff = 0;
	if(GetLineBase() != GetLineLeftRight(bLeft))
	{
		long nLeft = bLeft? -1 : 1;
		CLineInfo *pLine	= GetLineBase();
		if(!pLine) return dWidthDiff;

		double dSta			= m_dStationBridgeStt;
		
		double dWMax		= 0;
		double dWMin		= 0;
		for(long nJ = 0; nJ <= m_nQtyJigan; nJ++)
		{
			dSta	= GetStationOnJijum(nJ);
			if(nJ == 0 || nJ == m_nQtyJigan) 
				dSta	= GetStationByTarget((nJ == 0)? BRIDGE_OFF_STT_SLAB : BRIDGE_OFF_END_SLAB, 0, TRUE);
			
			double dW	= GetWidthSlabActByGagak(dSta, CDPoint(0, 1), nLeft);
			if(nJ == 0 || dW > dWMax)
			{
				dWMax	= dW;
			}
			if(nJ == 0 || dW < dWMin)
			{
				dWMin	= dW;
			}
			// 지간의 중간에도 하나씩 찍어보자.
			if(nJ < m_nQtyJigan)
			{
				dSta	+= GetLengthJigan(nJ-1)/2;
				double dW2	= GetWidthSlabActByGagak(dSta, CDPoint(0, 1), nLeft);
				if(dW2 > dWMax)
				{
					dWMax	= dW2;
				}
				if(dW2 < dWMin)
				{
					dWMin	= dW2;
				}
			}
		}

		dWidthDiff = dWMax - dWMin;
	}

	return dWidthDiff;
}


//방호벽의 설치 유무 ( bRebar(철근): TRUE, 방호벽 일반: FALSE)
//한개라도 존재할시 : TRUE, 한개도 없을시 FALSE
BOOL CRcBridgeApp::IsExistGuardWall(BOOL bRebarType)
{
	BOOL bExist = FALSE;

	for (long nIdx =0; nIdx < GetQtyHDan(); nIdx++)
	{
		CGuardWallRC* pGW = GetGuardWallByHDan(nIdx);
		if(!pGW) continue;

		BOOL bRoadGuard = pGW->IsTypeHDanRoadTotalGuardWall();
		BOOL bNonRebar  = pGW->IsTypeHDanNonRebarConcrete();

		if(bRoadGuard == FALSE) 
			continue;

		if(bRebarType && bNonRebar)
			continue;

		bExist = TRUE;
		break;
	}


	return bExist;
}

HGDOUBLE CRcBridgeApp::GetUnderGroundPaveHeightTosa()
{
	double dHeigtAll = m_bUnderGroundDesignHeight ? CalcUnderGroundDesignHeight() : m_dUnderGroundDesignHeight;
	double dHeight = dHeigtAll - (m_dUnderGroundPaveHeightPyo + m_dUnderGroundPaveHeightGee + m_dUnderGroundPaveHeightMiddle + m_dUnderGroundPaveHeightBozo + m_dUnderGroundPaveHeightDong + m_dUnderGroundPaveHeightEps);

	return dHeight;
}

long CRcBridgeApp::GetNumberElementPRFCheck(long nTypeDesign, BOOL bStt)
{
	CFEMManage *pFEM = GetFemManage(nTypeDesign);
	if(pFEM == NULL) return -1;

	long nIdxSlabStt = 0, nIdxSlabEnd = 0;
	long nSizeEle = pFEM->GetElementSize();
	for(long i=0; i<nSizeEle; i++)
	{
		CElement *pEle = pFEM->GetElement((USHORT)i);
		if(pEle->m_nFlag==ELEMENT_LOWERSLAB)
		{
			nIdxSlabStt = i;
			break;
		}
	}
	for(long i=nSizeEle-1; i>=0; i--)
	{
		CElement *pEle = pFEM->GetElement((USHORT)i);
		if(pEle->m_nFlag==ELEMENT_LOWERSLAB)
		{
			nIdxSlabEnd = i;
			break;
		}
	}

	double dDistStt = pFEM->GetJoint((USHORT)nIdxSlabStt)->m_vPointOrigin.x;
	double dDistEnd = pFEM->GetJoint((USHORT)(nIdxSlabEnd + 1))->m_vPointOrigin.x;
	
	for(long nIdx = 0; nIdx < nSizeEle; nIdx++)
	{
		CElement *pE = pFEM->GetElement((USHORT)nIdx);
		if(pE->m_nFlag != ELEMENT_LOWERSLAB) continue;
		CJoint *pJntI = pFEM->GetJoint(pE->m_nJStt);
		CJoint *pJntJ = pFEM->GetJoint(pE->m_nJEnd);

		// 시점 부상방지 저판
		if(bStt && pJntI->m_vPointOrigin.x < dDistStt)
		{
			return nIdx;
		}
		else if(bStt == FALSE && pJntJ->m_vPointOrigin.x > dDistEnd)
		{
			return nIdx;
		}
	}

	return -1;
}

double CRcBridgeApp::GetFactorCoDanger( EGoalOfEarthQuake eGoalEq )
{
	// bFunction == TRUE, 함형라멘교 - 기능수행
	// bFunction == FALSE, 함형이 아니거나 붕괴방지

	double dCoDanger(0.0);
	if(eGoalEq == eGoalEQ_None || eGoalEq == eGoalEQ_Collapse)
	{
		switch(GetDegreeEarthQuake())
		{
		case 0:			// 내진 1등급교
			dCoDanger = 1.4;	break;
		case 1:			// 내진 2등급교
			dCoDanger = 1.0;	break;
		case 2:			// 내진 특등급교(2400년)
			dCoDanger = 2.0;	break;
		case 3:			// 내진 특등급교(4800년)
			dCoDanger = 2.6;	break;
		default:
			break;
		}
	}
	else if(eGoalEq == eGoalEQ_Function)
	{
		switch(GetDegreeEarthQuake())
		{
		case 0:			// 내진 1등급교
			dCoDanger = 0.57;	break;
		case 1:			// 내진 2등급교
			dCoDanger = 0.4;	break;
		case 2:			// 내진 특등급교(2400년)
		case 3:			// 내진 특등급교(4800년)
			dCoDanger = 0.73;	break;
		default:
			break;
		}
	}
	else if(eGoalEq == eGoalEQ_Immediately)
	{
		switch(GetDegreeEarthQuake())
		{
		case 0:			// 내진 1등급교
			dCoDanger = 0.73;	break;
		case 1:			// 내진 2등급교
			dCoDanger = 0.57;	break;
		case 2:			// 내진 특등급교(2400년)
		case 3:			// 내진 특등급교(4800년)
			dCoDanger = 1.00;	break;
		default:
			break;
		}
	}if(eGoalEq == eGoalEQ_LongTerm)
	{
		switch(GetDegreeEarthQuake())
		{
		case 0:			// 내진 1등급교
			dCoDanger = 1.00;	break;
		case 1:			// 내진 2등급교
			dCoDanger = 0.73;	break;
		case 2:			// 내진 특등급교(2400년)
		case 3:			// 내진 특등급교(4800년)
			dCoDanger = 1.40;	break;
		default:
			break;
		}
	}

	return dCoDanger;
}

BOOL CRcBridgeApp::IsUseModelCombo( long nModelCombo )
{
	BOOL bUse(TRUE);

	if(nModelCombo >= COMBO_EARTHQUAKE)
	{
		if(IsApplyEarthQuakeDesign() == FALSE)
			bUse = FALSE;
		else
		{
			long nSizeEq = GetCountEarthQuakeModel();
			bUse = nSizeEq > (nModelCombo - COMBO_EARTHQUAKE) ? TRUE : FALSE;
		}
	}

	return bUse;
}

void CRcBridgeApp::SyncJijumStation()
{
	for(long ix = 0; ix < m_nQtyJigan; ++ix)
	{
		CJijum *pJ = GetJijum(ix);
		pJ->m_dStaJijum = GetStationOnJijum(ix);
	}
}

BOOL CRcBridgeApp::IsApplyTankLoad()
{
	HGBOOL bLSD = m_pARcBridgeDataStd->IsLsdDesign();

	BOOL bApply(FALSE);
	if(bLSD)
	{
		CLsdDesignLoad *pDesignLoad = m_plsdManager->GetDesignLoad(eLSD_LOADSELECT_LL1);
		if(pDesignLoad)
		{
			long nSeleOpt = pDesignLoad->GetIndexSelectOfType(eLSD_LOADCASE_DIR_BRI);
			bApply = (m_plsdManager->IsSelectedLoadDesign(eLSD_LOADSELECT_LL1)==FALSE || nSeleOpt == 0)? FALSE : TRUE;
		}
	}
	else
	{
		bApply = (m_bApplyLoad[BRIDGE_LOAD_TANK]==TRUE);
	}

	return bApply;
}
