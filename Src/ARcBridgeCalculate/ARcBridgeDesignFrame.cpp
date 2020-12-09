// ARcBridgeDesignFrame.cpp: implementation of the CARcBridgeDesignFrame class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../ARcBridgeData/ARcBridgeData.h"
#include "../ARcBridgeDBDraw/ARcBridgeDBDraw.h"
#include "ARcBridgeCalculate.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CARcBridgeDesignFrame::CARcBridgeDesignFrame()
{
	m_nCountSubsidence = 1;
}

CARcBridgeDesignFrame::~CARcBridgeDesignFrame()
{
//	AhTPADelete(&m_pListSlab,(CJointFrame*)0);
//	int i=0; for(i=0; i<MAX_JIJUM_COUNT_RC; i++)
//	{
//		AhTPADelete(&m_pListWall[i],(CJointFrame*)0);
//	}
}

void CARcBridgeDesignFrame::MakeSapDataJoint(CFEMManage *pFEM, BOOL bEarthQuake)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	CLineInfo *pLine = pBri->GetLineBase();

	BOOL bLsd = pData->IsLsdDesign();
	BOOL bArch = (pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)? TRUE : FALSE;
//	if(m_vArrSlab[iUPPER].GetSize()==0) MakeXyCoordinates(bEarthQuake);
	MakeXyCoordinates(bEarthQuake);
	SetSpringForce(bEarthQuake);
	if(m_dArrThickSlab.GetSize()==0) 
	{
		MakeArrayThick();
	}

	double dElevationMax = 0;
	long nJijumConnect = pBri->GetJijumConnectRibAndSlab();	// ARCH교일경우 리브와 슬래브가 붙은 지점

	BOOL bNLLink = (bEarthQuake && pBri->m_n2DEQSpringType == 0) || (!bEarthQuake &&  pBri->m_n2DGeneralSpringType == 0) ? TRUE : FALSE;
	// 절점배열 설정
	// 1) 절점생성
	// 슬래브
	long nIdxJoint = 0;
	for(long iUD = iUPPER; iUD <= iLOWER; iUD++)
	{
		if(iUD == iLOWER && pBri->IsBoxType() == FALSE) break;

		long nShoePos  = 0;
		long nSizeSlab = m_vArrSlab[iUD].GetSize(); 
		long nIdxSpring = 0;
		for(long i=0; i<nSizeSlab; i++)
		{
			CVector vPoint = m_vArrSlab[iUD].GetAt(i);
			CJoint *pJnt = pFEM->AddJoint((USHORT)(nIdxJoint++), CVector(vPoint.x,0,vPoint.z));
			pJnt->m_LongAttrib[LONG_0_JOINT_TYPE]  = iUD == iUPPER ? JOINT_UPPERSLAB : JOINT_LOWERSLAB;
			pJnt->m_LongAttrib[LONG_2_JOINT_SECTION_NO] = GetIndexSection(vPoint.y, TRUE) + 1;
			pJnt->m_LongAttrib[LONG_3_JOINT_HUNCHPOS]   = -1;

			dElevationMax = MAX(dElevationMax, fabs(pJnt->m_vPoint.z));

			if(pBri->IsBoxType() && iUD == iLOWER)
			{
				CDPoint xyJoint = CDPoint(vPoint.x, vPoint.z);
				BOOL bPileBase = pBri->IsPileBase(0) ? TRUE : FALSE;
				nIdxSpring = m_xyArrSpringLower.GetIndex(xyJoint, MIN_JOINT_DIST);
				if(nIdxSpring != -1)
				{
					pJnt->m_BoolAttrib[BOOL_0_JOINT_SPRING] = TRUE;

					long nCase = bEarthQuake ? 1 : 0;
					CVector vForce = m_vArrForceLower[nCase].GetAt(nIdxSpring);

					pJnt->m_vSpring.x  = bEarthQuake || bPileBase ? vForce.x : 0;
					pJnt->m_vSpring.z  = bNLLink && bPileBase == FALSE ? 0 : vForce.z; // 말뚝기초인 경우 저판은 스프링으로 처리
					pJnt->m_vSpringR.y = vForce.y;
				}
			}
			else if(iUD == iUPPER)
			{
				nShoePos = pBri->GetShoePosByStation(vPoint.x);
				if(nShoePos != -1)
				{
					long nJijum = pBri->GetJijumNumberByStation(vPoint.x);
					CJijum *pJijum = pBri->GetJijum(nJijum);
					if(pBri->IsRcSlab() || pBri->IsWall(nJijum)==FALSE)
					{
						if(pBri->m_bUsePadShoe)
						{
							double Kv = GetShoeKvValue(nJijum, bEarthQuake == FALSE);
							double Kh = GetShoeKhValue(nJijum, bEarthQuake == FALSE);
							pJnt->m_vSpring.x = Kh;
							pJnt->m_vSpring.y = Kh;
							pJnt->m_vSpring.z = Kv;
						}
						else
						{
							pJnt->m_UX = pJijum->IsFixJijum() ? 1 : 0;
							pJnt->m_UY =  1;
							pJnt->m_UZ =  1;
						}
						pJnt->m_LongAttrib[LONG_1_JOINT_JIJUMNO]  = nJijum;
						pJnt->m_BoolAttrib[BOOL_4_JOINT_SHOE_POS] = TRUE;
					}
				}
			}
		}
	}

	if(pBri->IsBoxType() && pBri->IsPileBase(-1) == FALSE && bEarthQuake == FALSE)
	{
		long nIndexLowerMid = GetIndexJointLowerSlabMid(pFEM);
		CJoint *pJoint = pFEM->GetJoint((unsigned short)nIndexLowerMid);
		pJoint->m_BoolAttrib[BOOL_0_JOINT_SPRING] = TRUE;
		pJoint->m_vSpring.x  = bLsd? 1e+07 : 1e+010;
	}

	// 벽체
	BOOL bFootInclude = (bEarthQuake &&  pBri->IsBoxType() == FALSE && pBri->GetTypeJointPosFootingEQ()==1) ? TRUE : FALSE;
	double TopHeight = m_vArrSlab[iUPPER].GetAt(0).z;
	for(long nJijum=0; nJijum<=pBri->m_nQtyJigan; nJijum++)
	{
		long nSizeWall = m_vArrWall[nJijum].GetSize();
		double tanVal =  0;
		if(nJijum>0 && nJijum<pBri->m_nQtyJigan)
		{
			CWallApp *pWall = pBri->GetInWall(nJijum-1);
			double dRadian = ToRadian(pWall->GetAngleWallByHor());
			tanVal = tan(dRadian);
		}
		if(nSizeWall<=0) continue;
		for(long i=0; i<nSizeWall; i++)
		{
			CVector vPoint = m_vArrWall[nJijum].GetAt(i);
			if(tanVal != 0) vPoint.x -= (TopHeight - vPoint.z) / tanVal;
			CJoint *pJnt = pFEM->AddJoint((USHORT)(nIdxJoint++), CVector(vPoint.x,0,vPoint.z));
			pJnt->m_LongAttrib[LONG_0_JOINT_TYPE]  = (nJijum==0 || nJijum==pBri->m_nQtyJigan) ? JOINT_OUTWALL : JOINT_MIDWALL;
			pJnt->m_LongAttrib[LONG_1_JOINT_JIJUMNO]	  =  nJijum;
			pJnt->m_LongAttrib[LONG_2_JOINT_SECTION_NO] = GetIndexSection(vPoint.y,FALSE) + 1;
			dElevationMax = MAX(dElevationMax, fabs(pJnt->m_vPoint.z));

			// 모델링의 하단점....
			// 아치교의 중간지점은 고정이나 스프링이 아니다.
			if(bArch && (nJijum>0 && nJijum<pBri->m_nQtyJigan)) continue;
			if(pBri->IsBoxType())
			{
				if(i==nSizeWall-3)// && pBri->GetTypeColumnByJijum(nJijum)!=-1) // 기둥의 하단점...
				{
					pJnt->m_BoolAttrib[BOOL_3_JOINT_COLUMN_END] = TRUE; // 내측벽체, 기둥위치
				}
			}
			else
			{
				if(i == nSizeWall-1)
				{
					if(bEarthQuake)
					{
						if(pBri->GetTypeRigidFootingEQ() == 2)
						{
							pJnt->m_BoolAttrib[BOOL_2_JOINT_JIJUM_FIX] = TRUE;	 // 기초하단을 FIX로
							pJnt->m_UX = pJnt->m_UY = pJnt->m_UZ = pJnt->m_RX = pJnt->m_RY = pJnt->m_RZ = 1;
						}
						else
						{
							pJnt->m_BoolAttrib[BOOL_0_JOINT_SPRING] = TRUE; // 기초하단을 SPRING로
						}
					}
					else
					{
						pJnt->m_BoolAttrib[BOOL_2_JOINT_JIJUM_FIX] = TRUE;	 // 기초하단을 FIX로
						pJnt->m_UX = pJnt->m_UY = pJnt->m_UZ = pJnt->m_RX = pJnt->m_RY = pJnt->m_RZ = 1;
					}
					if(pBri->GetTypeColumnByJijum(nJijum)!=-1 && !bFootInclude) // 기둥의 하단점...
					{
						pJnt->m_BoolAttrib[BOOL_3_JOINT_COLUMN_END] = TRUE;
					}
				}
				if(i==nSizeWall-2 && bFootInclude && pBri->GetTypeColumnByJijum(nJijum)!=-1) // 기둥의 하단점...
				{
					pJnt->m_BoolAttrib[BOOL_3_JOINT_COLUMN_END] = TRUE;
				}
			}
		}
	}

	// 벽체위치의 스프링
	if(pBri->IsBoxType() && bEarthQuake)
	{
		long nSize = pFEM->GetJointSize();
		for(long ix = 0; ix < nSize; ix++)
		{
			CJoint *pJnt = pFEM->GetJoint((unsigned short)ix);
			if(pJnt->m_LongAttrib[LONG_0_JOINT_TYPE]  == JOINT_MIDWALL) continue;
			if(pJnt->m_LongAttrib[LONG_0_JOINT_TYPE]  == JOINT_NLLINK) continue;

			CDPoint xyJoint = CDPoint(pJnt->m_vPoint.x, pJnt->m_vPoint.z);
			for(long iLR = iLEFT; iLR <= iRIGHT; iLR++)
			{
				long nIndexSpring = m_xyArrSpringWall[iLR].GetIndex(xyJoint, MIN_JOINT_DIST);
				if(nIndexSpring != -1)
				{
					pJnt->m_BoolAttrib[BOOL_0_JOINT_SPRING] = TRUE;
					CVector vForce = m_vArrForceWall[iLR].GetAt(nIndexSpring);
					pJnt->m_vSpring.x  = bNLLink ? 0 : vForce.x;
					if(pJnt->m_LongAttrib[LONG_0_JOINT_TYPE] != JOINT_LOWERSLAB)
					{
						pJnt->m_vSpring.z  = vForce.z;
					}
				}
			}
		}
	}

	// 2) 슬래브와 하부 연결점의 생성
	CDPoint vAngStt = pBri->GetAngleJijum(0);
	CDPoint vAngEnd = pBri->GetAngleJijum(pBri->m_nQtyJigan);
	long nSizeJnt = pFEM->GetJointSize();
	for(long nJijum = 0; nJijum <= pBri->m_nQtyJigan; nJijum++)
	{
		long nLine = 0;
		double staJijum = pBri->GetStationOnJijum(nJijum,nLine);
		// 시종점벽체가 없는 경우
		if(nJijum==0 && pBri->m_dWS==0) continue;	
		if(nJijum==pBri->m_nQtyJigan && pBri->m_dWE==0) continue;
		if(nJijum == nJijumConnect) continue;

		// 시종점벽체에 대한 설정
		if(nJijum==0 || nJijum==pBri->m_nQtyJigan)
		{
			staJijum = (nJijum==0) ? staJijum - pBri->m_dWS/vAngStt.y/2 : staJijum + pBri->m_dWE/vAngEnd.y/2;
			for(long i=0; i<nSizeJnt; i++)
			{
				CJoint *pJnt = pFEM->GetJoint((USHORT)i);
				// 상부슬래브
				if(pJnt->m_LongAttrib[LONG_0_JOINT_TYPE] == JOINT_UPPERSLAB && Compare(staJijum, pJnt->m_vPoint.x, _T("=")))
				{
					pJnt->m_BoolAttrib[BOOL_1_JOINT_JIJUM_CONNECT] = TRUE;
					pJnt->m_LongAttrib[LONG_1_JOINT_JIJUMNO]		= nJijum;
					if(pBri->IsBoxType() == FALSE)
					{
						if(pBri->GetTypeColumnByJijum(nJijum) !=-1 && !bFootInclude) // 기둥의 하단점...
						{
							pJnt->m_BoolAttrib[BOOL_3_JOINT_COLUMN_END] = TRUE;
						}
						break;
					}
				}
				// 하부슬래브
				if(pJnt->m_LongAttrib[LONG_0_JOINT_TYPE] == JOINT_LOWERSLAB && Compare(staJijum, pJnt->m_vPoint.x, _T("=")))
				{
					pJnt->m_BoolAttrib[BOOL_1_JOINT_JIJUM_CONNECT] = TRUE;
					pJnt->m_LongAttrib[LONG_1_JOINT_JIJUMNO] = nJijum;
					break;
				}
			}
			continue;
		}
		// 중간벽체에 대한 설정
		long nIdxInWall = nJijum - 1;
		long nIdxHunchLeft  = pBri->GetIdxHunchInWall(nIdxInWall,TRUE);
		long nIdxHunchRight = pBri->GetIdxHunchInWall(nIdxInWall,FALSE);

		CWallApp *pWall = pBri->GetInWall(nIdxInWall);
		staJijum = pBri->GetStationOnJijum(nJijum,nLine) - pWall->GetDistArm(TRUE);
		for(long i=0; i<nSizeJnt; i++)
		{
			CJoint *pJnt = pFEM->GetJoint((USHORT)i);
			// 상부슬래브
			if(pJnt->m_LongAttrib[LONG_0_JOINT_TYPE] == JOINT_UPPERSLAB && Compare(staJijum, pJnt->m_vPoint.x, _T("=")))
			{
				pJnt->m_BoolAttrib[BOOL_1_JOINT_JIJUM_CONNECT] = TRUE;
				pJnt->m_LongAttrib[LONG_1_JOINT_JIJUMNO] = nJijum;
				if(pBri->IsBoxType() == FALSE)
				{
					break;
				}
			}
			// 하부슬래브
			if(pJnt->m_LongAttrib[LONG_0_JOINT_TYPE] == JOINT_LOWERSLAB && Compare(staJijum, pJnt->m_vPoint.x, _T("=")))
			{
				pJnt->m_BoolAttrib[BOOL_1_JOINT_JIJUM_CONNECT] = TRUE;
				pJnt->m_LongAttrib[LONG_1_JOINT_JIJUMNO] = nJijum;
				break;
			}
		}
		// 중간벽체에 HUNCH가 2개인 경우 - V형
		if(nIdxHunchLeft != nIdxHunchRight)
		{
			staJijum = pBri->GetStationOnJijum(nJijum,nLine) + pWall->GetDistArm(FALSE);
			for(long i=0; i<nSizeJnt; i++)
			{
				CJoint *pJnt = pFEM->GetJoint((USHORT)i);
				if(Compare(staJijum,pJnt->m_vPoint.x,"="))
				{
					pJnt->m_BoolAttrib[BOOL_1_JOINT_JIJUM_CONNECT] = TRUE;
					pJnt->m_LongAttrib[LONG_1_JOINT_JIJUMNO] = nJijum;
					break;
				}
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// 함형라멘교 NLLINK 절점
	if(pBri->IsBoxType() && ( (bEarthQuake && pBri->m_n2DEQSpringType==0) || !bEarthQuake && pBri->m_n2DGeneralSpringType==0))
	{
		CVector vLink(0,0,0);
		// 하부슬래브
		if(pBri->IsPileBase(-1) == FALSE)
		{
			nSizeJnt = pFEM->GetJointSize();
			for(long ix = 0; ix < nSizeJnt; ix++)
			{
				CJoint *pJnt = pFEM->GetJoint((USHORT)ix);
				if(pJnt->m_LongAttrib[LONG_0_JOINT_TYPE] != JOINT_LOWERSLAB) continue;
				if(pJnt->m_BoolAttrib[BOOL_0_JOINT_SPRING] == FALSE) continue;

				vLink = pJnt->m_vPoint;
				CDPoint xyJoint = CDPoint(vLink.x, vLink.z);

				long nIndexSpring = m_xyArrSpringLower.GetIndex(xyJoint, MIN_JOINT_DIST);
				if(nIndexSpring != -1)
				{
					xyJoint.y -= 500;
					CJoint *pJNLLink = pFEM->AddJoint((USHORT)(nIdxJoint++), CVector(xyJoint.x,0,xyJoint.y));
					pJNLLink->m_UX = pJNLLink->m_UY = pJNLLink->m_UZ = 1;
					pJNLLink->m_RX = pJNLLink->m_RY = pJNLLink->m_RZ = 1;
					pJNLLink->m_BoolAttrib[BOOL_4_JOINT_SHOE_POS] = TRUE; // NLLINK 지점
					pJNLLink->m_LongAttrib[LONG_3_JOINT_HUNCHPOS] = ix;	 // NLLINK 연결 절점
					pJNLLink->m_LongAttrib[LONG_4_INDEX_SPRING] = nIndexSpring;
				}
			}
		}
		if(bEarthQuake)
		{
			// 좌우벽체
			for(long nLR = iLEFT; nLR <= iRIGHT; nLR++)
			{
				for(long ix = 0; ix < nSizeJnt; ix++)
				{
					CJoint *pJnt = pFEM->GetJoint((USHORT)ix);
					if(pJnt->m_BoolAttrib[BOOL_0_JOINT_SPRING] == FALSE) continue;

					vLink = pJnt->m_vPoint;
					CDPoint xyJoint = CDPoint(vLink.x, vLink.z);

					long nIndexSpring = m_xyArrSpringWall[nLR].GetIndex(xyJoint, MIN_JOINT_DIST);
					if(nIndexSpring != -1)
					{
						xyJoint.x -= nLR==iLEFT ? 500 : -500;
						CJoint *pJNLLink = pFEM->AddJoint((USHORT)(nIdxJoint++), CVector(xyJoint.x,0,xyJoint.y));
						pJNLLink->m_UX = pJNLLink->m_UY = pJNLLink->m_UZ = 1;
						pJNLLink->m_RX = pJNLLink->m_RY = pJNLLink->m_RZ = 1;
						pJNLLink->m_BoolAttrib[BOOL_4_JOINT_SHOE_POS] = TRUE; // NLLINK 지점
						pJNLLink->m_LongAttrib[LONG_3_JOINT_HUNCHPOS] = ix;	 // NLLINK 연결 절점
						pJNLLink->m_LongAttrib[LONG_4_INDEX_SPRING] = nIndexSpring;
					}
				}
			}
		}
	}

	// 3) 아치교일경우 리브 절점 추가
	if(pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH && m_vArrArchRib.GetSize() > 0)
	{
		long nSizeArchRib = m_vArrArchRib.GetSize();

		for(long i=0; i<nSizeArchRib; i++)
		{
			CVector vPoint = m_vArrArchRib.GetAt(i);
			CJoint *pJnt = pFEM->AddJoint((USHORT)(nIdxJoint++),CVector(vPoint.x,0,vPoint.z));
			pJnt->m_LongAttrib[LONG_0_JOINT_TYPE]  = JOINT_ARCHRIB;	
			pJnt->m_LongAttrib[LONG_2_JOINT_SECTION_NO] = GetIndexSection(vPoint.y, FALSE, TRUE) + 1;
			pJnt->m_LongAttrib[LONG_1_JOINT_JIJUMNO]	= (i==0)? pBri->m_nQtyJigan+1 : pBri->m_nQtyJigan+2;

			long nJ=0; for(nJ=0; nJ<pBri->m_nQtyJigan; nJ++)
			{
				if(m_vMatchArchAndInWall[nJ] == vPoint)
				{
					pJnt->m_LongAttrib[LONG_1_JOINT_JIJUMNO] = nJ;
					pJnt->m_BoolAttrib[BOOL_1_JOINT_JIJUM_CONNECT] = TRUE;
				}
			}

			if(i==0 || i==nSizeArchRib-1)
			{
				if(bEarthQuake)
				{
					if(pBri->GetTypeRigidFootingEQ()==2)
					{
						pJnt->m_BoolAttrib[BOOL_2_JOINT_JIJUM_FIX] = TRUE;	 // 기초하단을 FIX로
						pJnt->m_UX = pJnt->m_UY = pJnt->m_UZ = pJnt->m_RX = pJnt->m_RY = pJnt->m_RZ = 1;
					}
					else
					{
						pJnt->m_BoolAttrib[BOOL_0_JOINT_SPRING] = TRUE; // 기초하단을 SPRING로
					}
				}
				else
				{
					pJnt->m_BoolAttrib[BOOL_2_JOINT_JIJUM_FIX] = TRUE;	 // 기초하단을 FIX로
					pJnt->m_UX = pJnt->m_UY = pJnt->m_UZ = pJnt->m_RX = pJnt->m_RY = pJnt->m_RZ = 1;
				}
			}
		}
	}
	// 4) 절점의 좌표를 교량시점이 (0,dHeightMaxWall)이 되도록 조정, M단위로 변경	
	BOOL bUpperSlope = (pBri->IsBoxType() && pBri->m_bSlopeLowerSlab == FALSE) ? TRUE : FALSE; // 실제 EL을 설정
	// 실제의 Elevation과 station으로 저장 => m_vPointReal
	double dElBase = pLine->GetElevationPlanEl(pBri->GetStationOnJijum(0,0) - pBri->m_dWS/pBri->GetAngleJijum(0).y/2.) - pBri->GetThickPave() - pBri->m_dTS / 2.;
	nSizeJnt = pFEM->GetJointSize();
	for(long i=0; i<nSizeJnt; i++)
	{
		CJoint *pJnt = pFEM->GetJoint((USHORT)i);
		pJnt->m_vPointReal = pJnt->m_vPoint;
		if(bUpperSlope == FALSE)
		{
			pJnt->m_vPointReal.z += dElBase;
		}
	}

	CVector xyFirst = pFEM->GetJoint(0)->m_vPoint;
	for(long i=0; i<nSizeJnt; i++)
	{
		CJoint *pJnt = pFEM->GetJoint((USHORT)i);

		pJnt->m_vPoint.x  -= xyFirst.x;
		if(bUpperSlope)
			pJnt->m_vPoint.z  -= pBri->m_dELSB + pBri->m_dTS_Lower / 2.;
		else
			pJnt->m_vPoint.z  += dElevationMax;

		if(bLsd == FALSE)
		{
			pJnt->m_vPoint.x   = toM(pJnt->m_vPoint.x);
			pJnt->m_vPoint.z   = toM(pJnt->m_vPoint.z);
		}
		pJnt->m_vPointOrigin = pJnt->m_vPoint; // 길이변경전의 절점좌표를 저장, 직교이거나 경사지간을 적용한 경우는 의미가 없음/..
	}	

	//////////////////////////////////////////////////////////////////////////
	// 모델링을 모두 한 후에 사교에 대한 고려를 저장....
	// 1차모델링은 스테이션을 가지고 좌표를 설정
	// 사교인 경우 적용지간을 다시 산정하여 조정한다...
	// 지진시에는 사교에 대한 적용을 하지 않는다..?????? (2007.02.01)
	if(bEarthQuake==FALSE)
		ReCalculateCoordinateBySkew(pFEM);
	pFEM->RecalcExeRect();
}

void CARcBridgeDesignFrame::MakeSapDataElement(CFEMManage *pFEM, BOOL bEarthQuake)
{
	CRcBridgeRebar	  *pBri  = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	long nSizeJnt = pFEM->GetJointSize();
	if(nSizeJnt==0) return;

	BOOL bLsd = pData->IsLsdDesign();
	BOOL bUseLowerSlab = IsUseMaterialLowerSlab();
	BOOL bUseEndWall = IsUseMaterialEndWall();
	BOOL bUseInWall = IsUseMaterialInnerWall();

	BOOL bArch = (pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)? TRUE : FALSE;
	long nJijumConnect = pBri->GetJijumConnectRibAndSlab();
	long nIdxElement = 0;
	CString szName  = _T("");

	// 슬래브 부재 생성
	for(long iUD = iUPPER; iUD <= iLOWER; iUD++)
	{
		CString szHead(_T(""));
		if(bUseLowerSlab)
		{
			szHead = (iUD == iUPPER)? _T("U") : _T("L");
		}

		for(long i=0; i<nSizeJnt-1; i++)
		{
			CJoint *pJnt1 = pFEM->GetJoint((USHORT)i);
			CJoint *pJnt2 = pFEM->GetJoint((USHORT)(i+1));

			long nJointType1 = pJnt1->m_LongAttrib[LONG_0_JOINT_TYPE];
			long nJointType2 = pJnt2->m_LongAttrib[LONG_0_JOINT_TYPE];

			if(nJointType1 != nJointType2) continue;
			if(iUD == iUPPER && nJointType2 != JOINT_UPPERSLAB) break;
			if(iUD == iLOWER && nJointType1 != JOINT_LOWERSLAB) continue;

			CElement *pEle = pFEM->AddElement((USHORT)(nIdxElement++), pJnt1->m_nIdx,pJnt2->m_nIdx);	
			pEle->m_nFlag	= iUD == iUPPER ? ELEMENT_UPPERSLAB : ELEMENT_LOWERSLAB; 
			double dRigidEle = GetDistRigidElement(pFEM,pEle);
			szName = (dRigidEle > 0 && pBri->m_nSelectRigidModeling == 1) ? _T("RS") : (szHead + _T("SB"));
			if(dRigidEle != 0 && pBri->m_nSelectRigidModeling == 2) // End Offset
			{
				if(dRigidEle > 0)
					pEle->m_dIoff = bLsd? frM(dRigidEle) : dRigidEle;
				else
					pEle->m_dJoff = bLsd? frM(fabs(dRigidEle)) : fabs(dRigidEle);
				pEle->m_dRigid = 1.0;
			}
			pEle->m_szFrameSectionName.Format("%s%02d%02d", szName, pJnt1->m_LongAttrib[LONG_2_JOINT_SECTION_NO], pJnt2->m_LongAttrib[LONG_2_JOINT_SECTION_NO]);
		}
	}

	// 벽체 부재 생성
	BOOL bFootInclude = (bEarthQuake &&  pBri->IsBoxType() == FALSE && pBri->GetTypeJointPosFootingEQ()==1) ? TRUE : FALSE;
	long nIdxColumn = 1;
	long nJijum=0; for(nJijum=0; nJijum<=pBri->m_nQtyJigan; nJijum++)
	{
		long nSizeJoint = pFEM->GetJointSize();
		if(nSizeJoint==0) continue;
		if(nJijum == nJijumConnect) continue;

		CString szHead = bUseInWall? _T("IN") : _T("");

		long nIndexFlag = ELEMENT_MIDWALL;
		double dAngleEle = 0;
		if(nJijum == 0)
		{
			nIndexFlag = ELEMENT_STTWALL;
			dAngleEle = 180;
			szHead = (bUseEndWall)? _T("S") : _T("");
		}
		else if(nJijum == pBri->m_nQtyJigan)
		{
			nIndexFlag = ELEMENT_ENDWALL;
			szHead = (bUseEndWall)? _T("E") : _T("");
		}

		CJoint *pJntI = GetJointJijumOnSlab(pFEM, nJijum, TRUE);	// I단 절점
		CJoint *pJntJ = NULL;	// J단 절점
		for(long i=0; i<nSizeJnt; i++)
		{
			pJntJ = pFEM->GetJoint((USHORT)i);
			long nJointType = pJntJ->m_LongAttrib[LONG_0_JOINT_TYPE];
			long nJoitJijum = pJntJ->m_LongAttrib[LONG_1_JOINT_JIJUMNO];
			if(nJointType==JOINT_UPPERSLAB || nJoitJijum < nJijum) continue;
			if(nJointType==JOINT_LOWERSLAB || nJoitJijum < nJijum) continue;
			if(nJointType!=JOINT_UPPERSLAB && nJoitJijum > nJijum)
			{
				if(!bArch) 
					break;
				else if(bArch && (nJointType!=JOINT_ARCHRIB || nJoitJijum != nJijum)) 
					continue;
			}

			CElement *pEle = pFEM->AddElement((USHORT)(nIdxElement++),pJntI->m_nIdx,pJntJ->m_nIdx);
			pEle->m_LongAttrib[LONG_1_ELEMENT_JIJUMNO] = nJijum;
			long nIdxThick1 = pJntI->m_LongAttrib[LONG_2_JOINT_SECTION_NO];
			long nIdxThick2 = pJntJ->m_LongAttrib[LONG_2_JOINT_SECTION_NO];
			if(pJntI->m_LongAttrib[LONG_0_JOINT_TYPE] == JOINT_UPPERSLAB)		// 슬래브와 벽체의 접합부
			{
				double dThickWall = m_pStd->GetThickWallByDist(nJijum,0);
				nIdxThick1 = GetIndexSection(dThickWall,FALSE) + 1;
			}
			if(pJntJ->m_LongAttrib[LONG_0_JOINT_TYPE]==JOINT_ARCHRIB)	// 아치리브와 수직재의 접합부
			{

				nIdxThick2 = nIdxThick1;
			}
			pEle->m_nFlag	= nIndexFlag; 
			pEle->m_dAng  = dAngleEle;
			double dRigidEle = GetDistRigidElement(pFEM,pEle); 
			szName = (dRigidEle != 0 && pBri->m_nSelectRigidModeling == 1) ? "RW" : (szHead + "WL");
			if(dRigidEle != 0 && pBri->m_nSelectRigidModeling == 2) // End Offset
			{
				if(dRigidEle > 0)
					pEle->m_dIoff = fabs(bLsd? frM(dRigidEle) : dRigidEle);
				else
					pEle->m_dJoff = fabs(bLsd? frM(dRigidEle) : dRigidEle);
				pEle->m_dRigid = 1.0;
			}
			pEle->m_szFrameSectionName.Format("%s%02d%02d",szName,nIdxThick1,nIdxThick2);
			// 기둥부재인 경우
			if(pBri->GetTypeColumnByJijum(nJijum)!=-1 && pJntJ->m_BoolAttrib[BOOL_3_JOINT_COLUMN_END]) 
			{
				szName = "COLU";
				pEle->m_szFrameSectionName.Format("%s%02d",szName,nIdxColumn++);
			}
			// 기초부재인 경우 : 지진시
			if(bFootInclude && bEarthQuake && (pJntJ->m_BoolAttrib[BOOL_2_JOINT_JIJUM_FIX] || pJntJ->m_BoolAttrib[BOOL_0_JOINT_SPRING]))
			{
				long nIdxFameSect = nJijum+1;
				if(pBri->m_nTypeBridge==BRIDGE_TYPE_TRAP2 || pBri->m_nTypeBridge==BRIDGE_TYPE_ARCH)
				{
					long nQtyFoot = pBri->GetCountFooting();
					if(nJijum <= 1) nIdxFameSect = 1;
					else if(nJijum >= pBri->m_nQtyJigan-1) nIdxFameSect = nQtyFoot;
					else nIdxFameSect = nJijum;
				}
				szName = "FOOT";
				pEle->m_szFrameSectionName.Format("%s%02d",szName,nIdxFameSect);
			}

			if(nJijum>0 && nJijum<pBri->m_nQtyJigan && pBri->m_nTypeBridge==BRIDGE_TYPE_V && nJointType==JOINT_UPPERSLAB)
			{
				pJntI = GetJointJijumOnSlab(pFEM,nJijum,FALSE);
				CElement *pEle = pFEM->AddElement((USHORT)(nIdxElement++),pJntI->m_nIdx,pJntJ->m_nIdx);	
				pEle->m_nFlag = ELEMENT_MIDWALL; 	
				double dRigidEle = GetDistRigidElement(pFEM,pEle); 
				szName = (dRigidEle > 0 && pBri->m_nSelectRigidModeling == 1) ? "RW" : (szHead + "WL");
				if(dRigidEle > 0 && pBri->m_nSelectRigidModeling == 2) // End Offset
				{
					pEle->m_dIoff = bLsd? frM(dRigidEle) : dRigidEle;
					pEle->m_dRigid = 1.0;
				}
				pEle->m_szFrameSectionName.Format("%s%02d%02d",szName,pJntI->m_LongAttrib[LONG_2_JOINT_SECTION_NO],pJntJ->m_LongAttrib[LONG_2_JOINT_SECTION_NO]);
			}
			pJntI = pJntJ;
		}
		if(pBri->IsBoxType())
		{
			pJntJ = GetJointJijumOnSlab(pFEM, nJijum, TRUE, FALSE, FALSE);	// 하부슬래브 절점

			double dThickWall = m_pStd->GetThickWallByDist(nJijum, 0, TRUE);

			CElement *pEle = pFEM->AddElement((USHORT)(nIdxElement++),pJntI->m_nIdx,pJntJ->m_nIdx);
			pEle->m_LongAttrib[LONG_1_ELEMENT_JIJUMNO] = nJijum;
			long nIdxThick1 = pJntI->m_LongAttrib[LONG_2_JOINT_SECTION_NO];
			long nIdxThick2 = GetIndexSection(dThickWall,FALSE) + 1;
			pEle->m_nFlag	= nIndexFlag; 
			pEle->m_dAng  = dAngleEle;
			double dRigidEle = GetDistRigidElement(pFEM,pEle); 
			szName = (dRigidEle != 0 && pBri->m_nSelectRigidModeling == 1) ? "RW" : (szHead + "WL");
			if(dRigidEle != 0 && pBri->m_nSelectRigidModeling == 2) // End Offset
			{
				pEle->m_dJoff = fabs(bLsd? frM(dRigidEle) : dRigidEle);
				pEle->m_dRigid = 1.0;
			}
			pEle->m_szFrameSectionName.Format("%s%02d%02d",szName,nIdxThick1,nIdxThick2);
		}
	}

	if(bArch)
	{
		// 아크리브 부재 생성
		//BOOL bFirst = TRUE;
		long i=0; for(i=0; i<nSizeJnt-1; i++)
		{
			CJoint *pJnt1 = pFEM->GetJoint((USHORT)i);
			CJoint *pJnt2 = pFEM->GetJoint((USHORT)(i+1));

			long nJointType = pJnt1->m_LongAttrib[LONG_0_JOINT_TYPE];
			if(nJointType!=JOINT_ARCHRIB) continue;

			CElement *pEle;
			//			if(bFirst)
			//			{
			//				pEle = pFEM->AddElement(nIdxElement++, pJnt2->m_nIdx, pJnt1->m_nIdx);
			//				bFirst = FALSE;
			//			}
			//			else 
			pEle = pFEM->AddElement((USHORT)(nIdxElement++), pJnt1->m_nIdx, pJnt2->m_nIdx);

			pEle->m_nFlag	= ELEMENT_ARCHRIB; 		
			szName = "AR";
			pEle->m_szFrameSectionName.Format("%s%02d%02d",szName,pJnt1->m_LongAttrib[LONG_2_JOINT_SECTION_NO],pJnt2->m_LongAttrib[LONG_2_JOINT_SECTION_NO]);

			if(bFootInclude && bEarthQuake)
			{
				long nIdxFameSect = 0;
				if(pJnt1->m_BoolAttrib[BOOL_2_JOINT_JIJUM_FIX] || pJnt1->m_BoolAttrib[BOOL_0_JOINT_SPRING])
				{
					nIdxFameSect = 1;
					szName = "FOOT";
					pEle->m_szFrameSectionName.Format("%s%02d",szName,nIdxFameSect);
				}
				else if(pJnt2->m_BoolAttrib[BOOL_2_JOINT_JIJUM_FIX] || pJnt2->m_BoolAttrib[BOOL_0_JOINT_SPRING])
				{
					nIdxFameSect = 2;
					szName = "FOOT";
					pEle->m_szFrameSectionName.Format("%s%02d",szName,nIdxFameSect);
				}
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	// 부재생성후 사용안한 단면특성을 삭제...
	long nSizeElement = pFEM->GetElementSize();
	POSITION pos = pFEM->m_FrameSectionList.GetHeadPosition();
	while(pos)
	{
		BOOL bDeleteSection = TRUE;
		CFrameSection *pFrm = (CFrameSection *)pFEM->m_FrameSectionList.GetNext(pos);
		if(pFrm->m_szName.Find("SLAB")!=-1) continue;
		if(pFrm->m_szName.Find("WALL")!=-1) continue;
		if(pFrm->m_szName.Find("ARCH")!=-1) continue;
		if(pFrm->m_szName.Find("RGSB")!=-1) continue;
		if(pFrm->m_szName.Find("RGWL")!=-1) continue;
		long j=0; for(j=0; j<nSizeElement; j++)
		{
			CElement *pEle = pFEM->GetElement((USHORT)j);
			if(pFrm->m_szName==pEle->m_szFrameSectionName)
			{
				bDeleteSection = FALSE;
				break;
			}
		}
		if(bDeleteSection)
		{
			pFEM->DeleteFrameSection(pFrm->m_szName);
		}
	}
}

// 1. 슬래브의 부재를 설정, 중심선형의 STATION거리로 설정한다....
// 2. 벽체의 부재를 설정
// 3. ELEVATION은 무시하고 LEVEL로 모델링
void CARcBridgeDesignFrame::MakeXyCoordinates(BOOL bEarthQuake)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CLineInfo *pLine = pBri->GetLineBase();

	CDPointArray xyArrUpperHunch;
	m_vArrSlab[iUPPER].RemoveAll();
	m_vArrSlab[iLOWER].RemoveAll();
	long i=0; for(i=0; i<MAX_JIJUM_COUNT_RC; i++)
	{
		m_vArrWall[i].RemoveAll();
		m_vMatchArchAndInWall[i].SetNull();
	}
	m_vArrArchRib.RemoveAll();

	BOOL bArch	= (pBri->m_nTypeBridge==BRIDGE_TYPE_ARCH) ? TRUE : FALSE;
	long nJijumConnect = pBri->GetJijumConnectRibAndSlab();	// ARCH교일경우 리브와 슬래브가 붙은 지점

	// 시종점 벽체 두께에만 각도의 값을 적용하여 계산....
	// -> 모델링 전체길이를 맞추기위한 것이며 
	// -> 기타 부분의 벽체두께(시종점 포함),헌치,강역등은 모두 직거리의 값을 적용한다...
	CDPoint vAngStt = pBri->GetAngleJijum(0);
	CDPoint vAngEnd = pBri->GetAngleJijum(pBri->m_nQtyJigan);
	double dWS = pBri->m_dWS/vAngStt.y;
	double dWE = pBri->m_dWE/vAngEnd.y;

	long nLine = 0;
	long nJijum = 0; 
	double staJijum   = 0;
	double dElevation = 0; // LEVEL로 계산
	double dTS_Lower = pBri->IsBoxType() ? pBri->m_dTS_Lower : 0;
	double dElBase = pLine->GetElevationPlanEl(pBri->GetStationOnJijum(0,nLine) - dWS/2.); // 모델링 시점에서의 계획고
	//////////////////////////////////////////////////////////////////////////
	// 상부슬래브에 대한 설정
	// 1) 지점의 중심STATION 
	// vPoint.y : 두께를 저장한다....
	CVector vPoint(0, pBri->m_dTS + pBri->GetThickHunch(0,FALSE, FALSE), dElevation);
	for(nJijum=0; nJijum<=pBri->m_nQtyJigan; nJijum++)
	{
		staJijum = pBri->GetStationOnJijum(nJijum,nLine);
		vPoint.x = staJijum;
		vPoint.y = m_pStd->GetThickSlabByDist(nJijum, 0 ,TRUE);
		vPoint.z = dElevation;

		if(pBri->IsWall(nJijum) == FALSE)
		{
			CJijum *pJ = pBri->GetJijum(nJijum);
			if(nJijum == 0)
				vPoint.x += pJ->m_dLengthUgan[1];
			else if(nJijum == pBri->m_nQtyJigan)
				vPoint.x -= pJ->m_dLengthUgan[0];
		}
		m_vArrSlab[iUPPER].Add(vPoint);
		if(pBri->IsBoxType())
		{
			vPoint.y = m_pStd->GetThickSlabByDist(nJijum, 0 ,FALSE);
			vPoint.z = dElevation - pBri->GetHeightModelJijum(nJijum);
			m_vArrSlab[iLOWER].Add(vPoint);
		}
	}

	// 2) 벽체내측선,중심선(시종점) : 벽체두께,헌치부
	// 시종점벽체
	MakeXyCoordinates_SlabNode(TRUE, bEarthQuake);
	MakeXyCoordinates_SlabNode(FALSE, bEarthQuake);

	// 강역구간에 대한 입력
	if(pBri->m_nSelectRigidModeling == 1)
	{
		MakeXyCoordinates_SlabRigid(TRUE);
		MakeXyCoordinates_SlabRigid(FALSE);
	}

	// 중복되는 점을 삭제하고 X좌표를 기준으로 정렬...
	RemoveAndSortXyCoordinates_Slab(TRUE);
	RemoveAndSortXyCoordinates_Slab(FALSE);
	//
	double dUT = pBri->GetThickPave() + pBri->m_dTS / 2.;
	BOOL bUpperSlope = (pBri->IsBoxType() && pBri->m_bSlopeLowerSlab == FALSE) ? TRUE : FALSE; // 실제 EL을 설정
	if(bUpperSlope)
	{
		long nSize = m_vArrSlab[iUPPER].GetSize();
		for(long ix = 0; ix < nSize; ix++)
		{
			CVector vPos = m_vArrSlab[iUPPER].GetAt(ix);

			if(pBri->m_nSelectModelingHeight==0)
			{
				vPos.z  = pLine->GetElevationPlanEl(vPos.x) - dUT;
			}
			else 
			{
				vPos.z  = pBri->GetElMaxSlabUpper(vPos.x, CDPoint(0, 1)) + pBri->GetThickPave(FALSE) - dUT;
			}
			m_vArrSlab[iUPPER].SetAt(ix, vPos);
		}
		nSize = m_vArrSlab[iLOWER].GetSize();
		for(long ix = 0; ix < nSize; ix++)
		{
			CVector vPos = m_vArrSlab[iLOWER].GetAt(ix);
			vPos.z = pBri->m_dELSB + dTS_Lower / 2.;
			m_vArrSlab[iLOWER].SetAt(ix, vPos);
		}
		nSize = m_xyArrSpringLower.GetSize();
		for(long ix = 0; ix < nSize; ix++)
		{
			CDPoint xy = m_xyArrSpringLower.GetAt(ix);
			xy.y = pBri->m_dELSB + dTS_Lower / 2.;
			m_xyArrSpringLower.SetAt(ix, xy);
		}
	}


	//////////////////////////////////////////////////////////////////////////
	// 3) 벽체에 대한 설정
	double SlabThick   = pBri->m_dTS;
	double HeightJijum = 0;
	for(nJijum=0; nJijum<=pBri->m_nQtyJigan; nJijum++)
	{
		if(pBri->m_nTypeBridge==BRIDGE_TYPE_SLAB) break;
		if(pBri->IsWall(nJijum)==FALSE)	continue;
		if(nJijum == nJijumConnect) continue;

		long nIdxInWall = nJijum - 1;

		double HunchThickU = pBri->GetThickHunch(nJijum, TRUE, TRUE, TRUE);
		double HunchThickL = pBri->IsBoxType() ? pBri->GetThickHunch(nJijum, TRUE, TRUE, FALSE) : 0.0;

		staJijum = pBri->GetStationOnJijum(nJijum,nLine);
		if(nJijum==0) staJijum -= dWS/2.0;
		if(nJijum==pBri->m_nQtyJigan) staJijum += dWE/2.0;

		HeightJijum = pBri->GetHeightModelJijum(nJijum);
		if(pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP2)
		{
			if(nJijum==1) 
				HeightJijum = pBri->GetHeightModelJijum(0);
			else if(nJijum==pBri->m_nQtyJigan-1) 
				HeightJijum = pBri->GetHeightModelJijum(pBri->m_nQtyJigan); 
		}

		long nIdxHunchLeft  = pBri->GetIdxHunchInWall(nIdxInWall,TRUE);
		long nIdxHunchRight = pBri->GetIdxHunchInWall(nIdxInWall,FALSE);

		vPoint.x = staJijum;
		double dEl_XX = pBri->m_nSelectModelingHeight == 0 ? pLine->GetElevationPlanEl(vPoint.x) : (pBri->GetElMaxSlabUpper(vPoint.x, CDPoint(0, 1)) + pBri->GetThickPave(FALSE));
		dElevation = (bArch) ? dEl_XX - dElBase : 0;
		if(bUpperSlope)
		{
			dElevation = dEl_XX - dUT;
		}
		if(nIdxHunchLeft!=nIdxHunchRight) // V형
		{
			CWallApp *pWall = pBri->GetInWall(nIdxInWall);
			double dHeightArm = pWall->GetHeightArm(TRUE);
			vPoint.y = pBri->GetThickWall(nJijum);
			vPoint.z = dElevation - SlabThick/2.0 - HunchThickU - dHeightArm;
			m_vArrWall[nJijum].Add(vPoint);
		}
		else
		{
			vPoint.y = m_pStd->GetThickWallByDist(nJijum,SlabThick/2.0);
			vPoint.z = dElevation - SlabThick/2.0;
			m_vArrWall[nJijum].Add(vPoint);
			if(HunchThickU>0)
			{
				vPoint.y = pBri->GetThickWall(nJijum);
				vPoint.z = dElevation - SlabThick/2.0 - HunchThickU;
				m_vArrWall[nJijum].Add(vPoint);
			}
		}

		if(nJijum==0 || nJijum==pBri->m_nQtyJigan)
		{
// 			double dDivide = 4;
			long nDivideStt = 0;
			if(pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP2 || pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
			{
				double dHunchHeight = (nJijum==0)? pBri->m_dWSHH : pBri->m_dWEHH;

				vPoint.z = dElevation - HeightJijum;
				vPoint.y = m_pStd->GetThickWallByDist(nJijum, 0, TRUE);
				m_vArrWall[nJijum].Add(vPoint);
				if(bEarthQuake)
				{
					double CountDivi = (long)RoundUp(dHunchHeight/1000.0,0);
					double dLenUnit  = Round(dHunchHeight/CountDivi,0);
					for(long i=1; i<(long)CountDivi; i++)
					{
						vPoint.y = m_pStd->GetThickWallByDist(nJijum, dLenUnit*i, TRUE);
						vPoint.z = dElevation - (HeightJijum - dTS_Lower/2.) + dLenUnit*i;
						m_vArrWall[nJijum].Add(vPoint);
					}
				}

				// 헌치가 없는경우 절점이 하나 더생기는 문제가 발생
				// nDivideStt변수를 사용하여 걸러내도록 수정
				HeightJijum -= dHunchHeight;
				if(dHunchHeight == 0) nDivideStt = 1;
			}
			vPoint.y = pBri->GetThickWall(nJijum);
			if(bEarthQuake)
			{
				if(pBri->IsBoxType() == FALSE)
				{
					double dHeightWall = HeightJijum - (SlabThick + dTS_Lower)/2.- HunchThickU - HunchThickL;
					double CountDivi = (long)RoundUp(dHeightWall/1000.0,0);
					double dLenUnit  = Round(dHeightWall/CountDivi,0);
					for(long i = 1; i < (long)CountDivi; i++)
					{
						vPoint.z = dElevation - (SlabThick/2.0 + HunchThickU) - dLenUnit*i;
						m_vArrWall[nJijum].Add(vPoint);
					}
				}
				if(nDivideStt == 0)
				{
					vPoint.z = dElevation - (HeightJijum - dTS_Lower/2.);
					m_vArrWall[nJijum].Add(vPoint);
				}
			}
			else
			{
				double dDivide = 4;

				for(long i = nDivideStt; i < (long)dDivide; i++)
				{
					vPoint.z = dElevation - (HeightJijum - dTS_Lower/2. - HunchThickL) + i * (HeightJijum - SlabThick/2. - dTS_Lower/2. - HunchThickU - HunchThickL) / dDivide;
					m_vArrWall[nJijum].Add(vPoint);
				}
			}
		}
		else
		{
			vPoint.y = pBri->GetThickWall(nJijum);
			vPoint.z = dElevation - HeightJijum + dTS_Lower/2. + HunchThickL;
			if(bArch == FALSE)
			{
				m_vArrWall[nJijum].Add(vPoint);
			}
		}

		if(pBri->IsBoxType())
		{
			vPoint.y = pBri->GetThickWall(nJijum);
			vPoint.z = dElevation - HeightJijum + dTS_Lower/2.0;
			m_vArrWall[nJijum].Add(vPoint);
		}

		// 벽체 스프링
		if(bEarthQuake && pBri->IsBoxType() && (nJijum == 0 || nJijum == pBri->m_nQtyJigan))
		{
			long nLR = nJijum == 0 ? iLEFT : iRIGHT;
			double dCountSpring = Round(HeightJijum / pBri->m_dSpringSpaceWall, 0);
			double dCtcSpring = HeightJijum / dCountSpring;

			m_xyArrSpringWall[nLR].RemoveAll();
			m_xyArrSpringWall[nLR].Add(CDPoint(vPoint.x, dElevation));
			for(long kk = 1; kk < (long)(dCountSpring); kk++)
			{
				vPoint.z = dElevation - dCtcSpring * kk;
				m_vArrWall[nJijum].Add(vPoint);
				m_xyArrSpringWall[nLR].Add(CDPoint(vPoint.x, vPoint.z));
			}
			m_xyArrSpringWall[nLR].Add(CDPoint(vPoint.x, dElevation - HeightJijum));
		}

		// 지하수위
		if(pBri->IsBoxType() && pBri->m_bApplyGroundWaterLevel && (nJijum == 0 || nJijum == pBri->m_nQtyJigan))
		{
			if(pBri->m_dGroundWaterLevel < dElevation && pBri->m_dGroundWaterLevel > dElevation - HeightJijum)
			{
				vPoint.y = m_pStd->GetThickWallByDist(nJijum,dElevation-pBri->m_dGroundWaterLevel);
				vPoint.z = pBri->m_dGroundWaterLevel;
				m_vArrWall[nJijum].Add(vPoint);
			}
		}

		// 강역구간에 대한 입력
		if(pBri->m_nSelectRigidModeling == 1)
		{
			for(long nUD = iUPPER; nUD <= iLOWER; nUD++)
			{
				if(nUD == iLOWER && pBri->IsBoxType() == FALSE) break;

				double HeightRigid = pBri->GetDistRigidEndWall(nJijum, nUD == iUPPER);
				double dEleRigid = nUD == iUPPER ? dElevation - HeightRigid : dElevation - HeightJijum + HeightRigid;
				if(!Compare(HeightRigid,0.0,"="))
				{
					long nSizeWall = m_vArrWall[nJijum].GetSize();
					long j=0; for(j=0; j<nSizeWall; j++)
					{
						CVector vWall = m_vArrWall[nJijum].GetAt(j);
						if(Compare(vWall.z, dEleRigid, _T("=")))
						{
							break;
						}
					}
					if(j==nSizeWall)
					{
						vPoint.y = m_pStd->GetThickWallByDist(nJijum, HeightRigid, nUD == iLOWER);
						vPoint.z = dEleRigid;
						m_vArrWall[nJijum].Add(vPoint);
					}
				}
			}
		}

		// 기초의 도심에서 모델링을 하는 경우 절점을 추가한다...
		if(bEarthQuake && pBri->IsBoxType() == FALSE && pBri->GetTypeJointPosFootingEQ()==1)
		{
			HeightJijum = pBri->GetHeightModelJijum(nJijum);

			CFootingApp *pFooting = pBri->GetFootingByJijumNumber(nJijum);
			if((pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP2) && (nJijum==1 || nJijum==pBri->m_nQtyJigan-1))
			{
				pFooting = pBri->GetFootingByJijumNumber((nJijum==1)? 0 : pBri->m_nQtyJigan);
				double HeightJijumAdd = pBri->GetHeightModelJijum((nJijum==1)? 0 : pBri->m_nQtyJigan);
				double FH = pFooting->GetHeight();
				vPoint.y = pBri->GetThickWall(nJijum);
				vPoint.z = dElevation - HeightJijumAdd - FH/2.0;
				m_vArrWall[nJijum].Add(vPoint);
			}
			else if(pFooting)
			{
				double FH = pFooting->GetHeight();
				vPoint.y = pBri->GetThickWall(nJijum);
				vPoint.z = dElevation - HeightJijum - FH/2.0;
				m_vArrWall[nJijum].Add(vPoint);
			}
		}

		m_vArrWall[nJijum].RemoveSameValue();
		m_vArrWall[nJijum].Sort(2, FALSE); // z
	}

	//////////////////////////////////////////////////////////////////////////
	// 3) ARCH교 일경우 리브에 대한 설정
	if(bArch)
	{
		MakeXyCoordinates_ArchRib(bEarthQuake);
	}
}

// 지반반력을 스프링 위치에 저장
void CARcBridgeDesignFrame::SetSpringForce(BOOL bEarthQuake)
{
	long nCase = bEarthQuake ? MODULUS_FOUNDATION_EQ : MODULUS_FOUNDATION_NORMAL;
	m_vArrForceLower[nCase].RemoveAll();
	for(long nLR = iLEFT; nLR <= iRIGHT; nLR++)
	{
		m_vArrForceWall[nLR].RemoveAll();
	}

	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	if(pBri->IsBoxType() == FALSE) return;

	// 말뚝 하중case는 따로 쓴다.
	long nCasePile = nCase;
	CARcBridgeDataStd  *pData = pBri->m_pARcBridgeDataStd;
	if(pData->IsLsdDesign())
	{
		nCasePile = bEarthQuake? 2 : 1;			// 사용, 극한, 극단
	}

	long nIdxBoring = 0;
	// 하부슬래브
	BOOL bLsd = pData->IsLsdDesign();
	BOOL bPileBase = pBri->IsPileBase(0) ? TRUE : FALSE;
	double dSlabWidth = bEarthQuake? 1.0 : (bLsd? frM(m_pStd->GetSlabWidth(-1)) : m_pStd->GetSlabWidth(-1));
	double dApplyWidth = bEarthQuake? (bLsd? frM(m_pStd->GetSlabWidth(-1)) : m_pStd->GetSlabWidth(-1)) : 1.0;

	
	long nSizeSpring = m_xyArrSpringLower.GetSize();
	for(long ix = 0; ix < nSizeSpring; ix++)
	{
		CDPoint xyJoint = m_xyArrSpringLower.GetAt(ix);
		double dUZ = 0, dUX = 0, dRY = 0;
		if(bPileBase)
		{
			CFootingApp *pFooting = &pBri->m_footingBox;
			if(ix < pFooting->m_exFooting.m_Pile.m_nCountOrg) 
			{
				COrgInfo *pOrg = pFooting->m_exFooting.m_Pile.m_pArrOrgInfo.GetAt(ix);

				CDesignPile *pCalcPile = m_pStd->GetDesignPile(0);
				dUZ = GetValueUnitChange(pCalcPile->m_Kv, UNIT_CGS_TONF_M, pData->m_szTonf_M) * pOrg->countSide / dSlabWidth;
				dUX = GetValueUnitChange(pCalcPile->m_K1_Fix[nCasePile], UNIT_CGS_TONF_M, pData->m_szTonf_M) * pOrg->countSide / dSlabWidth;
				dRY = GetValueUnitChange(pCalcPile->m_K4_Fix[nCasePile], UNIT_CGS_TONFM, pData->m_szTonfM) * pOrg->countSide / dSlabWidth;
			}
		}
		else
		{
			double dSpringSpace = 0;
			long nSizeSpring = m_xyArrSpringLower.GetSize();
			if(ix < nSizeSpring)
			{
				if(ix < nSizeSpring - 1)
				{
					if(bLsd)
						dSpringSpace += (m_xyArrSpringLower.GetAt(ix + 1).x - m_xyArrSpringLower.GetAt(ix).x) / 2.;
					else
						dSpringSpace += toM(m_xyArrSpringLower.GetAt(ix + 1).x - m_xyArrSpringLower.GetAt(ix).x) / 2.;
				}
				if(ix > 0)
				{
					if(bLsd)
						dSpringSpace += (m_xyArrSpringLower.GetAt(ix).x - m_xyArrSpringLower.GetAt(ix - 1).x) / 2.;
					else
						dSpringSpace += toM(m_xyArrSpringLower.GetAt(ix).x - m_xyArrSpringLower.GetAt(ix - 1).x) / 2.;
				}

				if(pBri->m_bSpringSpaceIncludeWallTick && pBri->m_bExistProtectionRiseFooting == FALSE)
				{
					if(ix == 0)
						dSpringSpace += bLsd ? (pBri->m_dWS / 2.) : toM(pBri->m_dWS / 2.);
					if(ix == nSizeSpring - 1)
						dSpringSpace += bLsd ? (pBri->m_dWE / 2.) : toM(pBri->m_dWE / 2.);
				}
			}
			dUX = m_pStd->m_pModulusFoundation->m_Ks[MODULUS_POS_LOWERSLAB][nCase][nIdxBoring] * dSpringSpace * dApplyWidth;
			dUZ = m_pStd->m_pModulusFoundation->m_KvBL[MODULUS_POS_LOWERSLAB][nCase][nIdxBoring] * dSpringSpace * dApplyWidth;
		}
		dUX = Round(dUX, 3);
		dRY = Round(dRY, 3);
		dUZ = Round(dUZ, 3);
		CVector vForce(dUX, dRY, dUZ);
		m_vArrForceLower[nCase].Add(vForce);
	}

	if(bEarthQuake == FALSE) return;

	CLineInfo *pLine = pBri->GetLineBase();
	BOOL bUpperSlope = (pBri->IsBoxType() && pBri->m_bSlopeLowerSlab == FALSE) ? TRUE : FALSE;
	double dElBaseAdd = bUpperSlope ? 0 : pLine->GetElevationPlanEl(pBri->GetStationOnJijum(0,0) - pBri->m_dWS/pBri->GetAngleJijum(0).y/2.) - pBri->GetThickPave() - pBri->m_dTS / 2.;
	
	// 좌우측 벽체
	double dElevationSpring = 0;
	for(long iLR = iLEFT; iLR <= iRIGHT; iLR++)
	{
		long nPos = (iLR == iLEFT)? MODULUS_POS_STTWALL : MODULUS_POS_ENDWALL;
		long nSizeSpring = m_xyArrSpringWall[iLR].GetSize();
		for(long ix = 0; ix < nSizeSpring; ix++)
		{
			double dSpringSpace = 0;
			if(ix < m_xyArrSpringWall[iLR].GetSize() - 1)
			{
				if(bLsd)
					dSpringSpace += (m_xyArrSpringWall[iLR].GetAt(ix).y - m_xyArrSpringWall[iLR].GetAt(ix + 1).y) / 2.;
				else
					dSpringSpace += toM(m_xyArrSpringWall[iLR].GetAt(ix).y - m_xyArrSpringWall[iLR].GetAt(ix + 1).y) / 2.;
			}
			if(ix > 0)
			{
				if(bLsd)
					dSpringSpace += (m_xyArrSpringWall[iLR].GetAt(ix - 1).y - m_xyArrSpringWall[iLR].GetAt(ix).y) / 2.;
				else
					dSpringSpace += toM(m_xyArrSpringWall[iLR].GetAt(ix - 1).y - m_xyArrSpringWall[iLR].GetAt(ix).y) / 2.;
			}

			//벽체는 정확히 걸리는 위치는 윗쪽 지반정보를 적용하기 위해 0.001 더해주자.
			dElevationSpring = toM(m_xyArrSpringWall[iLR].GetAt(ix).y + dElBaseAdd);
			nIdxBoring = m_pStd->m_pModulusFoundation->GetIndexBoringByElevation(dElevationSpring+0.001);

			double dUZ = m_pStd->m_pModulusFoundation->m_Ks[nPos][nCase][nIdxBoring] * dSpringSpace * dApplyWidth;
			double dUX = m_pStd->m_pModulusFoundation->m_KvBL[nPos][nCase][nIdxBoring] * dSpringSpace * dApplyWidth;
			double dRY = 0;
			dUX = Round(dUX, 3);
			dUZ = Round(dUZ, 3);
			CVector vForce(dUX, dRY, dUZ);
			m_vArrForceWall[iLR].Add(vForce);
		}
	}
}


void CARcBridgeDesignFrame::MakeXyCoordinates_ArchRib(BOOL bEarthQuake)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;

	if(pBri->m_nTypeBridge != BRIDGE_TYPE_ARCH) return;


	CTwinVectorArray tvArrRibCen;
	CTwinVector tv;
	tvArrRibCen = pBri->m_tvArrVertSectionRibCen;

	CDPointArray xyArrRib;
	CRebarPlacing rb;
	CVector vPoint(0,0,0);

	if(tvArrRibCen.GetSize() == 0) return;

	CLineInfo *pLine = pBri->GetLineBase();

	CDPoint vAngStt = pBri->GetAngleJijum(0);
	CDPoint vAngEnd = pBri->GetAngleJijum(pBri->m_nQtyJigan);
	double dWS = pBri->m_dWS/vAngStt.y;
	double dWE = pBri->m_dWE/vAngEnd.y;

	long nLine = 0;
	long nJijumConnect = pBri->GetJijumConnectRibAndSlab();	// ARCH교일경우 리브와 슬래브가 붙은 지점

	double dElBase = pLine->GetElevationPlanEl(pBri->GetStationOnJijum(0,nLine) - dWS/2.0) - (pBri->GetThickPave() + pBri->m_dTS/2.0);
	double dElDiff = dElBase;
	// 리브 시점
	tv = tvArrRibCen.GetAt(0);
	vPoint.x = tv.m_v1.x;
	vPoint.y = m_pStd->GetThickArchRib(vPoint.x);
	vPoint.z = tv.m_v1.y - dElDiff;
	m_vArrArchRib.Add(vPoint);
	xyArrRib.Add(CDPoint(vPoint.x, vPoint.z));

	// 내측벽체지점
	CDPointArray poArr;
	CDPoint po1;
	long nCntInwall = pBri->GetCountInWall();
	for(long n=0; n<nCntInwall; n++)
	{
		long nJijum = (pBri->IsOutsideWall())? n+1 : n;
		double staJijum = pBri->GetStationOnJijum(nJijum,nLine);

		rb.GetXyMatchTvArrAndLine(tvArrRibCen, CDPoint(staJijum, dElBase), CDPoint(0, 1), poArr);
		if(poArr.GetSize() > 0)
		{
			po1 = poArr.GetAt(0);
			vPoint.x = po1.x;
			vPoint.y = m_pStd->GetThickArchRib(vPoint.x);
			vPoint.z = po1.y - dElDiff;
			m_vArrArchRib.Add(vPoint);
			xyArrRib.Add(CDPoint(vPoint.x, vPoint.z));
			m_vMatchArchAndInWall[nJijum] = vPoint;
		}

		if(nJijum == nJijumConnect)
		{
			double WT = pBri->GetThickWall(nJijum)/2;

			// 좌측
			rb.GetXyMatchTvArrAndLine(tvArrRibCen, CDPoint(staJijum-WT, dElBase), CDPoint(0, 1), poArr);
			if(poArr.GetSize() > 0)
			{
				po1 = poArr.GetAt(0);
				vPoint.x = po1.x;
				vPoint.y = m_pStd->GetThickArchRib(vPoint.x);
				vPoint.z = po1.y - dElDiff;
				m_vArrArchRib.Add(vPoint);
				xyArrRib.Add(CDPoint(vPoint.x, vPoint.z));
				m_vMatchArchAndInWall[nJijum] = vPoint;
			}
			// 우측
			rb.GetXyMatchTvArrAndLine(tvArrRibCen, CDPoint(staJijum+WT, dElBase), CDPoint(0, 1), poArr);
			if(poArr.GetSize() > 0)
			{
				po1 = poArr.GetAt(0);
				vPoint.x = po1.x;
				vPoint.y = m_pStd->GetThickArchRib(vPoint.x);
				vPoint.z = po1.y - dElDiff;
				m_vArrArchRib.Add(vPoint);
				xyArrRib.Add(CDPoint(vPoint.x, vPoint.z));
				m_vMatchArchAndInWall[nJijum] = vPoint;
			}
		}
	}

	// 리브 종점
	tv = tvArrRibCen.GetAt(tvArrRibCen.GetSize()-1);
	vPoint.x = tv.m_v2.x;
	vPoint.y = m_pStd->GetThickArchRib(vPoint.x);
	vPoint.z = tv.m_v2.y - dElDiff;
	m_vArrArchRib.Add(vPoint);
	xyArrRib.Add(CDPoint(vPoint.x, vPoint.z));

	// 절점사이를 4등분하는 절점 추가
	xyArrRib.Sort(TRUE,TRUE);
	long nSizeRib = xyArrRib.GetSize();
	for(long i=0; i < nSizeRib-1; i++)
	{
		double staRibJoint1 = 0;
		double staRibJoint2 = 0;

		staRibJoint1 = xyArrRib.GetAt(i).x;
		staRibJoint2 = xyArrRib.GetAt(i+1).x;
		long j=0; for(j=1; j<4; j++)
		{
			CDPoint xyJoint(staRibJoint1 + j*(staRibJoint2-staRibJoint1)/4, dElBase);
			rb.GetXyMatchTvArrAndLine(tvArrRibCen, xyJoint, CDPoint(0, 1), poArr);
			if(poArr.GetSize() > 0)
			{
				po1 = poArr.GetAt(0);
				vPoint.x = po1.x;
				vPoint.y = m_pStd->GetThickArchRib(vPoint.x);
				vPoint.z = po1.y - dElDiff;
				m_vArrArchRib.Add(vPoint);
			}
		}
	}

	if(bEarthQuake && pBri->GetTypeJointPosFootingEQ()==1)
	{
		// 시점기초
		CFootingApp *pFooting = pBri->GetFootingByJijumNumber(0);
		double HeightJijumAdd = pBri->GetHeightModelJijum(0);
		double FH = pFooting->GetHeight();
		tv = tvArrRibCen.GetAt(0);

		double dStaTemp = pBri->GetStationOnJijum(0,nLine) - dWS/2;
		double dElevation = (pLine->GetElevationPlanEl(dStaTemp)-(pBri->GetThickPave()+pBri->m_dTS/2)) - dElBase;

		CDPoint vDir  = tv.GetXyDir();
		vPoint.y = m_pStd->GetThickArchRib(tv.m_v1.x);
		vPoint.z = dElevation - HeightJijumAdd - FH/2;
		vPoint.x = tv.m_v1.x + (vPoint.z - tv.m_v1.y + dElBase)*(tv.GetXyDir().x / tv.GetXyDir().y);
		m_vArrArchRib.Add(vPoint);

		// 종점기초
		pFooting = pBri->GetFootingByJijumNumber(pBri->m_nQtyJigan);
		HeightJijumAdd = pBri->GetHeightModelJijum(pBri->m_nQtyJigan);
		FH = pFooting->GetHeight();
		tv = tvArrRibCen.GetAt(tvArrRibCen.GetSize()-1);

		dStaTemp = pBri->GetStationOnJijum(pBri->m_nQtyJigan,nLine) + dWE/2;
		dElevation = (pLine->GetElevationPlanEl(dStaTemp)-(pBri->GetThickPave()+pBri->m_dTS/2)) - dElBase;

		vPoint.y = m_pStd->GetThickArchRib(tv.m_v2.x);
		vPoint.z = dElevation - HeightJijumAdd - FH/2;
		vPoint.x = tv.m_v2.x + (vPoint.z - tv.m_v2.y + dElBase)*(tv.GetXyDir().x / tv.GetXyDir().y);
		m_vArrArchRib.Add(vPoint);
	}

	m_vArrArchRib.Sort(0,TRUE);
}

void CARcBridgeDesignFrame::RemoveAndSortXyCoordinates_Slab(BOOL bUpperSlab)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CLineInfo *pLine = pBri->GetLineBase();

	if(bUpperSlab == FALSE && pBri->IsBoxType() == FALSE) return;

	long nUD = bUpperSlab ? iUPPER : iLOWER;
	long nLine = 0;

	CDPoint vAngStt = pBri->GetAngleJijum(0);
	double dWS = pBri->m_dWS/vAngStt.y;

	double dTS_Lower = pBri->IsBoxType() ? pBri->m_dTS_Lower : 0;
	double dElBase = pLine->GetElevationPlanEl(pBri->GetStationOnJijum(0, nLine) - dWS/2.) - (pBri->GetThickPave() + pBri->m_dTS/2. + dTS_Lower/2.);

	CVectorArray vArrTemp;
	CVector vTemp, vPoint;
	// S30476 : Sort를 먼저하게 되면 중복되는 점 검사시 교량받침 정위치가 지워질 가능성이 있음
	// m_vArrSlab[nUD].Sort(0,TRUE);			
	double dMinTerm = MIN_JOINT_DIST;
	long nSizePoint = m_vArrSlab[nUD].GetSize();
	for(long i=0; i<nSizePoint; i++)
	{
		CVector vPoint = m_vArrSlab[nUD].GetAt(i);
		long nSizeTemp = vArrTemp.GetSize();
		long j=0; for(j=0; j<nSizeTemp; j++)
		{
			vTemp = vArrTemp.GetAt(j);
			if(fabs(vTemp.x - vPoint.x) <= dMinTerm && fabs(vTemp.y - vPoint.y) <= dMinTerm) 
			{
				break;
			}
		}
		if(j == nSizeTemp)
		{
			vArrTemp.Add(vPoint);
		}
	}

	m_vArrSlab[nUD].RemoveAll();
	nSizePoint = vArrTemp.GetSize();
	for(long i=0; i<nSizePoint; i++)
	{
		vPoint = vArrTemp.GetAt(i);
		if(pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
		{
			vPoint.z = (pLine->GetElevationPlanEl(vPoint.x) - (pBri->GetThickPave() + pBri->m_dTS/2.0)) - dElBase;
		}
		m_vArrSlab[nUD].Add(vPoint);
	}
	m_vArrSlab[nUD].RemoveSameValue(MIN_JOINT_DIST);
	m_vArrSlab[nUD].Sort(0, TRUE);
}


void CARcBridgeDesignFrame::MakeXyCoordinates_SlabRigid(BOOL bUpperSlab)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;

	if(bUpperSlab == FALSE && pBri->IsBoxType() == FALSE) return;
	if(pBri->m_nSelectRigidModeling == 0) return;

	long nUD = bUpperSlab ? iUPPER : iLOWER;

	long nJijumConnect = pBri->GetJijumConnectRibAndSlab();	// ARCH교일경우 리브와 슬래브가 붙은 지점
	BOOL bRcSlab = (pBri->m_nTypeBridge==BRIDGE_TYPE_SLAB) ? TRUE : FALSE;

	long nLine = 0;
	double staJijum = pBri->GetStationOnJijum(0, nLine);
	double dElevation = 0; // LEVEL로 계산

	long nJijum = 0;

	CDPoint vAngStt = pBri->GetAngleJijum(0);
	CDPoint vAngEnd = pBri->GetAngleJijum(pBri->m_nQtyJigan);
	double dWS = pBri->m_dWS/vAngStt.y;
	double dWE = pBri->m_dWE/vAngEnd.y;

	CVector vPoint(0,0);
	vPoint.z = bUpperSlab ? dElevation : dElevation - pBri->GetHeightModelJijum(nJijum);

	CDPoint xyRigid(0,0);

	for(long nJijum=0; nJijum<pBri->m_nQtyJigan+1; nJijum++)
	{
		if(bRcSlab) break;
		if(pBri->IsWall(nJijum)==FALSE)	continue;
		if(nJijum == nJijumConnect) continue;

		staJijum = pBri->GetStationOnJijum(nJijum,nLine);
		if(nJijum==0) staJijum -= dWS/2;
		if(nJijum==pBri->m_nQtyJigan) staJijum += dWE/2.0;

		double DistRigidStt = pBri->GetDistRigidEndSlab(nJijum,TRUE, bUpperSlab);
		double DistRigidEnd = pBri->GetDistRigidEndSlab(nJijum,FALSE, bUpperSlab);
		if(!Compare(DistRigidStt,0.0,"=", MIN_JOINT_DIST))
		{
			xyRigid.x = staJijum - DistRigidStt;
			long nSizeSlab = m_vArrSlab[nUD].GetSize();
			long j=0; for(j=0; j<nSizeSlab; j++)
			{
				CVector vSlab = m_vArrSlab[nUD].GetAt(j);
				if(Compare(vSlab.x,xyRigid.x,"="))
					break;
				// 짧은 부재가 생성되는 경우 기존의 절점을 강역절점으로 교체함 (5mm이하의 부재)
				if(fabs(vSlab.x-xyRigid.x)<=5) 
				{
					vPoint.x = xyRigid.x;
					vPoint.y = m_pStd->GetThickSlabByDist(nJijum, -DistRigidStt, bUpperSlab);
					m_vArrSlab[nUD].SetAt(j,vPoint);
					break;
				}
			}
			if(j==nSizeSlab)
			{
				vPoint.x = xyRigid.x;
				vPoint.y = m_pStd->GetThickSlabByDist(nJijum, -DistRigidStt, bUpperSlab);
				m_vArrSlab[nUD].Add(vPoint);
			}
		}
		if(!Compare(DistRigidEnd,0.0,"=", MIN_JOINT_DIST))
		{
			xyRigid.x = staJijum + DistRigidEnd;
			long nSizeSlab = m_vArrSlab[nUD].GetSize();
			long j=0; for(j=0; j<nSizeSlab; j++)
			{
				CVector vSlab = m_vArrSlab[nUD].GetAt(j);
				if(Compare(vSlab.x,xyRigid.x,"="))
					break;
				// 짧은 부재가 생성되는 경우 기존의 절점을 강역절점으로 교체함 (5mm이하의 부재)
				if(fabs(vSlab.x-xyRigid.x)<=MIN_JOINT_DIST) 
				{
					vPoint.x = xyRigid.x;
					vPoint.y = m_pStd->GetThickSlabByDist(nJijum, DistRigidEnd, bUpperSlab);
					m_vArrSlab[nUD].SetAt(j,vPoint);
					break;
				}
			}
			if(j==nSizeSlab)
			{
				vPoint.x = xyRigid.x;
				vPoint.y = m_pStd->GetThickSlabByDist(nJijum, DistRigidEnd, bUpperSlab);
				m_vArrSlab[nUD].Add(vPoint);
			}
		}
	}
}


void CARcBridgeDesignFrame::MakeXyCoordinates_SlabNode(BOOL bUpperSlab, BOOL bEarthQuake)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;

	if(bUpperSlab == FALSE && pBri->IsBoxType() == FALSE) return;

	long nUD = bUpperSlab ? iUPPER : iLOWER;

	long nLine = 0;
	double staJijum = pBri->GetStationOnJijum(0, nLine);
	double dElevation = 0; // LEVEL로 계산

	long nJijum = 0;
	long nJijumConnect = pBri->GetJijumConnectRibAndSlab();	// ARCH교일경우 리브와 슬래브가 붙은 지점

	CDPoint vAngStt = pBri->GetAngleJijum(0);
	CDPoint vAngEnd = pBri->GetAngleJijum(pBri->m_nQtyJigan);
	double dWS = pBri->m_dWS/vAngStt.y;
	double dWE = pBri->m_dWE/vAngEnd.y;
	double dHWS = bUpperSlab ? pBri->m_dHWS/vAngStt.y : pBri->m_dWSHW/vAngStt.y;
	double dHWE = bUpperSlab ? pBri->m_dHWE/vAngEnd.y : pBri->m_dWEHW/vAngEnd.y;

	CVector vPoint(0,0);
	vPoint.z = bUpperSlab ? dElevation : dElevation - pBri->GetHeightModelJijum(nJijum);
	CDPointArray xyArrHunch, xyArrJijum;
	if(bUpperSlab == FALSE && pBri->IsBoxType())
	{
		// Spring 절점을 위해서 지점위치만 저장
		for(long ix = 0; ix < m_vArrSlab[iLOWER].GetSize(); ix++)
		{
			CVector vPos = m_vArrSlab[iLOWER].GetAt(ix);
			xyArrJijum.Add(CDPoint(vPos.x, vPos.z));
		}
	}

	if(pBri->IsWall(0))
	{
		vPoint.x = staJijum - dWS/2.0;
		vPoint.y = m_pStd->GetThickSlabByDist(nJijum, -dWS/2.0, bUpperSlab);
		m_vArrSlab[nUD].Add(vPoint);
		xyArrHunch.Add(CDPoint(staJijum + dHWS ,dElevation));
	}
	else
	{
		CJijum *pJ = pBri->GetJijum(0);
		vPoint.x = staJijum + (pJ->m_dLengthUgan[1] + pJ->m_dLengthSlabToShoe[1]);
		vPoint.y = m_pStd->GetThickSlabByDist(nJijum, (pJ->m_dLengthUgan[1]+pJ->m_dLengthSlabToShoe[1]), bUpperSlab);
		m_vArrSlab[nUD].Add(vPoint);
		if(dWS>0)
		{
			vPoint.x = staJijum + dWS;
			vPoint.y = m_pStd->GetThickSlabByDist(nJijum, dWS, bUpperSlab);
			m_vArrSlab[nUD].Add(vPoint);
		}
		if(dHWS>0)
		{
			xyArrHunch.Add(CDPoint(staJijum + dWS + dHWS ,dElevation));
		}
		else
		{
			xyArrHunch.Add(CDPoint(staJijum + (pJ->m_dLengthUgan[1] + pJ->m_dLengthSlabToShoe[1])/vAngStt.y ,dElevation));
		}
	}
	// 종점
	nJijum = pBri->m_nQtyJigan;
	staJijum = pBri->GetStationOnJijum(pBri->m_nQtyJigan,nLine);
	if(pBri->IsWall(pBri->m_nQtyJigan))
	{
		vPoint.x = staJijum + dWE/2.0;
		vPoint.y = m_pStd->GetThickSlabByDist(nJijum, dWE/2.0, bUpperSlab);
		m_vArrSlab[nUD].Add(vPoint);
		xyArrHunch.Add(CDPoint(staJijum - dHWE ,dElevation));
	}
	else
	{
		CJijum *pJ = pBri->GetJijum(pBri->m_nQtyJigan);
		vPoint.x = staJijum - (pJ->m_dLengthUgan[0] + pJ->m_dLengthSlabToShoe[0]);
		vPoint.y = m_pStd->GetThickSlabByDist(nJijum, (-pJ->m_dLengthUgan[0]-pJ->m_dLengthSlabToShoe[0]), bUpperSlab);
		m_vArrSlab[nUD].Add(vPoint);
		if(dWE>0)
		{
			vPoint.x = staJijum - dWE;
			vPoint.y = m_pStd->GetThickSlabByDist(nJijum, -dWE, bUpperSlab);
			m_vArrSlab[nUD].Add(vPoint);
		}
		if(dHWE>0)
			xyArrHunch.Add(CDPoint(staJijum - dWE-dHWE ,dElevation));
		else
			xyArrHunch.Add(CDPoint(staJijum - (pJ->m_dLengthUgan[0] + pJ->m_dLengthSlabToShoe[0])/vAngEnd.y ,dElevation));
	}

	//////////////////////////////////////////////////////////////////////////
	// 내측벽체
	// 내측 벽체 두께에는 각도적용 안함.
	BOOL bRcSlab   = (pBri->m_nTypeBridge==BRIDGE_TYPE_SLAB) ? TRUE : FALSE;
	BOOL bTrapType = (pBri->m_nTypeBridge==BRIDGE_TYPE_TRAP || pBri->m_nTypeBridge==BRIDGE_TYPE_TRAP2) ? TRUE : FALSE;

	for(nJijum=1; nJijum<pBri->m_nQtyJigan; nJijum++)
	{
		long nIdxInWall = nJijum - 1;
		long nIdxHunchLeft  = pBri->GetIdxHunchInWall(nIdxInWall,TRUE);
		long nIdxHunchRight = pBri->GetIdxHunchInWall(nIdxInWall,FALSE);

		CWallApp *pWall = pBri->GetInWall(nIdxInWall);
		staJijum = pBri->GetStationOnJijum(nJijum,nLine) - pWall->GetDistArm(TRUE);

		CDPoint vAngInWall = pBri->GetAngleJijum(nJijum);
		double WT  = Round(pWall->m_dW, 0);	// 벽체의 두께
		double WL  = bTrapType ? 0 : Round(pBri->m_dWL[nIdxHunchLeft][nUD],0);
		double WR  = bTrapType ? 0 : Round(pBri->m_dWR[nIdxHunchLeft][nUD],0);
		double HWL = Round(pBri->m_dHLW[nIdxHunchLeft][nUD]/vAngInWall.y,0);
		double HWR = Round(pBri->m_dHRW[nIdxHunchRight][nUD]/vAngInWall.y,0);

		vPoint.x = staJijum - WL;
		vPoint.y = (nJijum == nJijumConnect)? pBri->m_dTS : m_pStd->GetThickSlabByDist(nJijum, -WL, bUpperSlab);
		m_vArrSlab[nUD].Add(vPoint);
		if(nJijum == nJijumConnect)			// ARCH리브와 슬래브 접합구간을 3등분한점
		{
			double sta1 = staJijum - WT/2.0;
			double sta2 = staJijum;
			long j=0; for(j=1; j<4; j++)
			{
				vPoint.x = sta1 + j*(sta2-sta1)/4.0;
				vPoint.y = bUpperSlab? pBri->m_dTS : pBri->m_dTS_Lower;
				m_vArrSlab[nUD].Add(vPoint);
			}
		}
		else if(!Compare(WT/2,WL,"=") && !bRcSlab && !bTrapType)
		{
			vPoint.x = staJijum - WT/2.0;
			vPoint.y = m_pStd->GetThickSlabByDist(nJijum, -WT/2.0, bUpperSlab);
			m_vArrSlab[nUD].Add(vPoint);
		}
		xyArrHunch.Add(CDPoint(staJijum-WL-HWL,dElevation));
		//
		vPoint.x = staJijum + WR;
		vPoint.y = m_pStd->GetThickSlabByDist(nJijum, WR, bUpperSlab);
		m_vArrSlab[nUD].Add(vPoint);
		if(nJijum == nJijumConnect)			// ARCH리브와 슬래브 접합구간을 3등분한점
		{
			double sta1 = staJijum;
			double sta2 = staJijum + WT/2.0;
			long j=0; for(j=1; j<4; j++)
			{
				vPoint.x = sta1 + j*(sta2-sta1)/4;
				vPoint.y = bUpperSlab? pBri->m_dTS : pBri->m_dTS_Lower;;
				m_vArrSlab[nUD].Add(vPoint);
			}
		}
		else if(!Compare(WT/2,WR,"=") && !bRcSlab && !bTrapType)
		{
			vPoint.x = staJijum + WT/2.0;
			vPoint.y = m_pStd->GetThickSlabByDist(nJijum, WT/2.0, bUpperSlab);
			m_vArrSlab[nUD].Add(vPoint);
		}
		xyArrHunch.Add(CDPoint(staJijum+WR+HWR,dElevation));
	}

	//////////////////////////////////////////////////////////////////////////
	long nSizeHunch = xyArrHunch.GetSize();
	xyArrHunch.Sort(TRUE,TRUE);
	for(long ix = 0; ix < nSizeHunch; ix++)
	{
		vPoint.x = xyArrHunch.GetAt(ix).x;
		vPoint.y = bUpperSlab? pBri->m_dTS : pBri->m_dTS_Lower;;
		m_vArrSlab[nUD].Add(vPoint);
	}

	//////////////////////////////////////////////////////////////////////////
	if(bUpperSlab)
	{
		// HUNCH사이를 4등분하는 절점을 넣음
		for(long ix=0; ix < nSizeHunch-1; ix += 2)
		{
			double staHunch1 = 0, staHunch2 = 0;

			staHunch1 = xyArrHunch.GetAt(ix).x;
			staHunch2 = xyArrHunch.GetAt(ix + 1).x;
			for(long kk = 1; kk < 4; kk++)
			{
				vPoint.x = staHunch1 + kk * (staHunch2 - staHunch1) / 4.0;
				vPoint.y = pBri->m_dTS;
				m_vArrSlab[nUD].Add(vPoint);
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// 함형라멘교 하부슬래브 절점
	if(bUpperSlab || pBri->IsBoxType() == FALSE) return;

	long nSizeJijum = xyArrJijum.GetSize();
	double dCountSpring = 0;
	double dCtcSpring = 0;
	m_xyArrSpringLower.RemoveAll();

	BOOL bPileBase = pBri->IsPileBase(0);
	CExPileApp *pPile = &pBri->m_footingBox.m_exFooting.m_Pile;

	// 부상방지판
	double dPRFW = pBri->m_bExistProtectionRiseFooting ? pBri->m_dPRF_W : 0;
	if(dPRFW > 0)
	{
		// 시점
		double dLength = dWS / 2. + dPRFW / vAngStt.y;
		vPoint.x = xyArrJijum.GetAt(0).x - dWS / 2.;
		vPoint.y = m_pStd->GetThickSlabByDist(nJijum, 0 , FALSE);
		if(bPileBase && pPile)
		{
			vPoint.x -= dLength;
			m_vArrSlab[nUD].Add(vPoint);
		}
		else
		{
			dCountSpring = Round(dLength / pBri->m_dSpringSpaceBottom, 0);
			dCtcSpring = dLength / dCountSpring;
			for(long kk = 1; kk <= (long)(dCountSpring); kk++)
			{
				vPoint.x -= dCtcSpring;
				m_vArrSlab[nUD].Add(vPoint);
				m_xyArrSpringLower.Add(CDPoint(vPoint.x, vPoint.z));
			}
		}
		// 종점
		dLength = dWE / 2. + dPRFW / vAngEnd.y;
		vPoint.x = xyArrJijum.GetAt(nSizeJijum - 1).x + dWE / 2.;
		vPoint.y = m_pStd->GetThickSlabByDist(nSizeJijum - 1, 0 , FALSE);
		if(bPileBase && pPile)
		{
			vPoint.x += dLength;
			m_vArrSlab[nUD].Add(vPoint);
		}
		else
		{
			dCountSpring = Round(dLength / pBri->m_dSpringSpaceBottom, 0);
			dCtcSpring = dLength / dCountSpring;
			for(long kk = 1; kk <= (long)(dCountSpring); kk++)
			{
				vPoint.x += dCtcSpring;
				m_vArrSlab[nUD].Add(vPoint);
				m_xyArrSpringLower.Add(CDPoint(vPoint.x, vPoint.z));
			}
		}
	}

	// Spring 절점
	m_dArrThickSpring.RemoveAll();
	if(bPileBase && pPile)
	{
		CLineInfo *pLine	= pBri->GetLineBase();
		double dStation = pBri->GetStationBridgeStt(FALSE);
		double dStationPile = pLine->GetStationByOffsetWidth(dStation, -pBri->m_dWS, pBri->GetAngleJijum(0));
		vPoint.x = dStationPile;
		for(long ix = 0; ix < pPile->m_nCountOrg; ix++)
		{
			COrgInfo *pOrg =pPile-> m_pArrOrgInfo.GetAt(ix);
			if(pOrg == NULL) continue;
			// ARCBRIDGE-2793
			// 입력위치를 더하고 지점번호를 찾아야함....(x) -> (한개씩 덜 나옴)
			vPoint.x += pOrg->dist;
			long nJ = pBri->GetJijumNumberByStation(vPoint.x);
			dStation = pBri->GetStationOnJijum(nJ);
			vPoint.y = m_pStd->GetThickSlabByDist(nJ, vPoint.x - dStation, FALSE);

			m_vArrSlab[nUD].Add(vPoint);
			m_xyArrSpringLower.Add(CDPoint(vPoint.x, vPoint.z));
			m_dArrThickSpring.Add(vPoint.y);
		}		
	}
	else
	{
		for(long ix = 0; ix < nSizeJijum - 1; ix++)
		{
			double dStation1 = xyArrJijum.GetAt(ix).x;
			double dStation2 = xyArrJijum.GetAt(ix + 1).x;
			if(ix == 0) // 시점
				dStation1 -= (dWS / 2.);
			if(ix == nSizeJijum - 2) // 종점
				dStation2 += (dWE / 2.); 

			dCountSpring = Round((dStation2 - dStation1) / pBri->m_dSpringSpaceBottom, 0);
			dCtcSpring = (dStation2 - dStation1) / dCountSpring;
			for(long kk = 0; kk <= (long)(dCountSpring); kk++)
			{
				vPoint.x = dStation1 + dCtcSpring * kk;
				if(kk > 0 && kk < (long)(dCountSpring))
				{
					nJijum = pBri->GetJijumNumberByStation(dStation1 + dCtcSpring * kk);
					vPoint.y = m_pStd->GetThickSlabByDist(nJijum, nJijum == ix ? dCtcSpring * kk : dStation1 - dStation2 + dCtcSpring * kk, FALSE);
					m_vArrSlab[nUD].Add(vPoint);
					m_dArrThickSpring.Add(vPoint.y);
				}
				if(m_xyArrSpringLower.GetIndex(CDPoint(vPoint.x, vPoint.z), MIN_JOINT_DIST) == -1)
				{
					m_xyArrSpringLower.Add(CDPoint(vPoint.x, vPoint.z));
				}
			}
		}
	}

	m_dArrThickSpring.RemoveSameValue();
	m_xyArrSpringLower.Sort(TRUE, TRUE); // x,오름차순
}


// CJoint* CARcBridgeDesignFrame::GetJointWallTop(CFEMManage *pFEM, long nJijum)
// {
// 	CRcBridgeRebar *pBri = m_pStd->m_pBri;
// 	if(nJijum>0 && nJijum<pBri->m_nQtyJigan) return NULL;
// 
// 	long nSizeJnt = pFEM->GetJointSize();
// 	if(nSizeJnt==0) return NULL;
// 
// 	long i=0; for(i=0; i<nSizeJnt; i++)
// 	{
// 		CJoint *pJnt = pFEM->GetJoint((USHORT)i);
// 		if(pJnt->m_LongAttrib[LONG_0_JOINT_TYPE]==JOINT_UPPERSLAB) continue;
// 		if(pJnt->m_LongAttrib[LONG_1_JOINT_JIJUMNO]==nJijum) return pJnt;
// 	}
// 
// 	return NULL;
// }

CJoint* CARcBridgeDesignFrame::GetJointWallHunchEnd(CFEMManage *pFEM, long nJijum)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	if(nJijum>0 && nJijum<pBri->m_nQtyJigan) return NULL;

	BOOL bLsd = pBri->m_pARcBridgeDataStd->IsLsdDesign();
	long nSizeJnt = pFEM->GetJointSize();
	if(nSizeJnt==0) return NULL;

	long nJointTop = (nJijum == pBri->m_nQtyJigan)? GetNumberJointSlabEnd(pFEM) : 0;
	double dHH = (nJijum == 0)? pBri->m_dHHS : pBri->m_dHHE;
	double dThick = bLsd? (pBri->m_dTS/2 + dHH) : toM(pBri->m_dTS/2 + dHH);
	double CheckPosZ = pFEM->GetJoint((unsigned short)nJointTop)->m_vPointOrigin.z - dThick;

	long i=0; for(i=0; i<nSizeJnt; i++)
	{
		CJoint *pJnt = pFEM->GetJoint((USHORT)i);
		if(pJnt->m_LongAttrib[LONG_0_JOINT_TYPE]==JOINT_UPPERSLAB) continue;
		if(pJnt->m_LongAttrib[LONG_1_JOINT_JIJUMNO]!=nJijum)		 continue;
		if(Compare(pJnt->m_vPointOrigin.z,CheckPosZ,"=", 0.001)) return pJnt;
	}

	return NULL;
}

// bBottomJoint : 벽체절점중 가장 아래것을 리턴 , 함형라멘교가 아닌 경우만 유효
CJoint* CARcBridgeDesignFrame::GetJointJijumOnSlab(CFEMManage *pFEM, long nJijum, BOOL bFirst, BOOL bBottomJoint, BOOL bUpperSlab)
{
	CJoint *pJnt = NULL;

	long nSizeJnt = pFEM->GetJointSize();

	if(bBottomJoint)
	{
		for(long i = nSizeJnt-1; i>=0; i--)
		{
			pJnt = pFEM->GetJoint((USHORT)i);
			if(pJnt->m_LongAttrib[LONG_1_JOINT_JIJUMNO] == nJijum)
			{
				return pJnt;
			}
		}
	}
	else
	{
		if(bUpperSlab)
		{
			long nCount = 0;

			for(long i=0; i<nSizeJnt; i++)
			{
				pJnt = pFEM->GetJoint((USHORT)i);
				if(bFirst && pJnt->m_LongAttrib[LONG_1_JOINT_JIJUMNO]==nJijum && nCount==0)
				{
					return pJnt;
				}
				if(!bFirst && pJnt->m_LongAttrib[LONG_1_JOINT_JIJUMNO]==nJijum && nCount==1)
				{
					return pJnt;
				}
				if(pJnt->m_LongAttrib[LONG_1_JOINT_JIJUMNO]==nJijum) nCount++;
			}
		}
		else
		{
			for(long i=0; i<nSizeJnt; i++)
			{
				pJnt = pFEM->GetJoint((USHORT)i);
				if(pJnt->m_LongAttrib[LONG_0_JOINT_TYPE] != JOINT_LOWERSLAB) continue;
				if(pJnt->m_LongAttrib[LONG_1_JOINT_JIJUMNO] == nJijum)
				{
					return pJnt;
				}
			}
		}
	}

	return pJnt;
}


void CARcBridgeDesignFrame::MakeSapDataMaterial(CFEMManage *pFEM, BOOL bEarthQuake)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd   *pData = pBri->m_pARcBridgeDataStd;

	BOOL bLsd = pData->IsLsdDesign();
	CString str = _T("");

	double Ec = pBri->m_pARcBridgeDataStd->m_Ec*10; // kgf/cm2 -> tonf/m2
	double Wc = toTon(pBri->m_pARcBridgeDataStd->m_pBasicConcInfo->m_UWeightSteelConcrete);
	double gV = pBri->GetGravity();
	double dT = 0;
	double dU = bLsd? pData->m_RatePoasong : 0.15;
	double dA = pBri->GetCoefficientExpansion();//0.00001;  AAB-6066

	gV = GetValueUnitChange(gV,UNIT_CGS_M,pData->m_szM);
	Ec = GetValueUnitChange(Ec,UNIT_CGS_TONF_M2,pData->m_szTonf_M2);
	Wc = GetValueUnitChange(Wc,UNIT_CGS_TONF_M3,pData->m_szTonf_M3);

	if(bLsd)
	{
		CString szName(_T(""));

		// 슬래브, 시점벽체, 종점벽체, 내측벽체
		for (long ix =0; ix < ePartCount; ix++)
		{
			BOOL bUse(FALSE);
			switch (ix)
			{
			case ePartUpperSlab:
				bUse = TRUE; break;
			case ePartOutWallStt:
			case ePartOutWallEnd:
				bUse = pBri->IsOutsideWall()? TRUE : FALSE; break;
			case ePartInWall:
				bUse = (pBri->GetCountInWall() > 0)? TRUE : FALSE; break;
			case ePartLowerSlab:
				bUse = pBri->IsBoxType()? TRUE : FALSE; break;
			case ePartFootStt:
			case ePartFootEnd:
				bUse = (pBri->GetTypeJointPosFootingEQ() == 1 && bEarthQuake)? TRUE : FALSE;
			default:
				break;
			}
			if(bUse == FALSE)
				continue;

			Ec = pBri->GetValueEc((EPartOfBridge)ix, TRUE);
			BOOL bFind(FALSE);
			long icMat = pFEM->GetMaterialSize();
			POSITION possub = pFEM->m_MaterialList.GetHeadPosition();
			while(possub)
			{
				CMaterial *pMat = (CMaterial *)pFEM->m_MaterialList.GetNext(possub);
				if(Compare(pMat->m_dE, Ec, _T("="), 0.001))
				{
					bFind = TRUE;
					break;
				}
			}
			if(bFind)
				continue;

			szName.Format(_T("CONC%d"), icMat+1);
			
			pFEM->AddMaterial(szName,MAT_TYPE_CONC, Ec,Wc,'C',Wc/gV, dT, dU, dA);
		}
	}
	else
	{
		pFEM->AddMaterial("CONC",MAT_TYPE_CONC, Ec,Wc,'C',Wc/gV, dT, dU, dA);	
	}
}

void CARcBridgeDesignFrame::MakeSapDataFrameSection(CFEMManage *pFEM, BOOL bEarthQuake)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	long nSizeJnt = pFEM->GetJointSize();
	if(nSizeJnt==0) return;

	BOOL bLsd = pData->IsLsdDesign();
	BOOL bRcSlab  = (pBri->m_nTypeBridge==BRIDGE_TYPE_SLAB) ? TRUE : FALSE;
	BOOL bArch	  = (pBri->m_nTypeBridge==BRIDGE_TYPE_ARCH) ? TRUE : FALSE;

	double dWidth = bEarthQuake ? m_pStd->GetSlabWidth() : UNIT_METER;
	double dWidthArchRib = bEarthQuake ? toM(pBri->GetWidthArchRib(0)) : UNIT_METER;
	if(bLsd)
	{
		dWidth = bEarthQuake? frM(dWidth) : 1;
		dWidthArchRib = frM(dWidthArchRib);
	}

	//////////////////////////////////////////////////////////////////////////
	// 절점별 단면특성을 설정
	// 일반단면
	// LSD에서 구조물강도를 분리하면서 구조물위치별로 따로 적용되는지 판단해야 한다.
	BOOL bUseLowerSlab = IsUseMaterialLowerSlab();
	BOOL bUseEndWall = IsUseMaterialEndWall();
	BOOL bUseInWall = IsUseMaterialInnerWall();

	CString szNameMat(_T("CONC"));
	CString szSection = _T("");
	double dThick = 0;
	long nSizeSlab = m_dArrThickSlab.GetSize();
	long i=0; for(i=0; i<nSizeSlab; i++)
	{
		dThick = m_dArrThickSlab.GetAt(i);
		szSection.Format("SLAB%02d",i+1);

		if(bLsd)
		{
			dThick = frM(dThick);
			szNameMat = GetMaterialNameByValue(pFEM, pBri->GetValueEc(ePartUpperSlab, TRUE));
			if(bUseLowerSlab)
			{
				szSection.Format("USLAB%02d",i+1);
			}
		}
		pFEM->AddFrameSectionOfRectangle(szSection,szNameMat,dWidth, dThick);		
	}

	if(bUseLowerSlab)
	{
		long i=0; for(i=0; i<nSizeSlab; i++)
		{
			dThick = frM(m_dArrThickSlab.GetAt(i));
			szSection.Format("LSLAB%02d",i+1);
			szNameMat = GetMaterialNameByValue(pFEM, pBri->GetValueEc(ePartLowerSlab, TRUE));
			
			pFEM->AddFrameSectionOfRectangle(szSection,szNameMat,dWidth, dThick);		
		}	
	}

	long nSizeWall = m_dArrThickWall.GetSize();
	for(i=0; i<nSizeWall; i++)
	{
		dThick = m_dArrThickWall.GetAt(i);
		szSection.Format("WALL%02d",i+1);

		if(bLsd)
		{
			dThick = frM(dThick);
			szNameMat = GetMaterialNameByValue(pFEM, pBri->GetValueEc(ePartOutWallStt, TRUE));
			if(bUseEndWall)
			{
				szSection.Format("SWALL%02d",i+1);
			}
		}
		pFEM->AddFrameSectionOfRectangle(szSection,szNameMat,dWidth, dThick);				
	}
	if(bUseEndWall)
	{
		for(i=0; i<nSizeWall; i++)
		{
			dThick = frM(m_dArrThickWall.GetAt(i));
			szSection.Format("EWALL%02d",i+1);
			szNameMat = GetMaterialNameByValue(pFEM, pBri->GetValueEc(ePartOutWallEnd, TRUE));

			pFEM->AddFrameSectionOfRectangle(szSection,szNameMat,dWidth, dThick);				
		}
	}
	if(bUseInWall)
	{
		for(i=0; i<nSizeWall; i++)
		{
			dThick = frM(m_dArrThickWall.GetAt(i));
			szSection.Format("INWALL%02d",i+1);
			szNameMat = GetMaterialNameByValue(pFEM, pBri->GetValueEc(ePartInWall, TRUE));

			pFEM->AddFrameSectionOfRectangle(szSection,szNameMat,dWidth, dThick);		
		}		
	}

	if(bArch)
	{
		long nSizeArch = m_dArrThickArchRib.GetSize();
		for(i=0; i<nSizeArch; i++)
		{
			dThick = m_dArrThickArchRib.GetAt(i);
			szSection.Format("ARCH%02d",i+1);
			pFEM->AddFrameSectionOfRectangle(szSection,szNameMat,dWidthArchRib, dThick);				
		}
	}
	// 강역단면 : 일반단면과 동일하며 무한강성으로 처리....
	if(pBri->m_nSelectRigidModeling == 1)
	{
		double dIxyRigid = bLsd? (UNLIMITED_RIGIDITY * pow(10.0, bEarthQuake? 12 : 9))  : UNLIMITED_RIGIDITY;
		if(!bRcSlab)
		{
			nSizeSlab = m_dArrThickSlab.GetSize();
			for(i=0; i<nSizeSlab; i++)
			{
				szSection.Format("RGSB%02d",i+1);
				double H  = bLsd? frM(m_dArrThickSlab.GetAt(i)) : m_dArrThickSlab.GetAt(i);
				if(pBri->m_nSelectRigidModeling != 0)
					pFEM->AddFrameSection(szSection,szNameMat,H*dWidth,dIxyRigid,dIxyRigid,dIxyRigid);
				else
					pFEM->AddFrameSectionOfRectangle(szSection,szNameMat,dWidth, m_dArrThickSlab.GetAt(i));		
			}
			nSizeWall = m_dArrThickWall.GetSize();
			for(i=0; i<nSizeWall; i++)
			{
				szSection.Format("RGWL%02d",i+1);
				double H  = bLsd? frM(m_dArrThickWall.GetAt(i)) : m_dArrThickWall.GetAt(i);
				if(pBri->m_nSelectRigidModeling != 0)
					pFEM->AddFrameSection(szSection,szNameMat,H*dWidth,dIxyRigid,dIxyRigid,dIxyRigid);
				else
					pFEM->AddFrameSectionOfRectangle(szSection,szNameMat,dWidth, m_dArrThickWall.GetAt(i));				
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// 부재별로 변단면에 해당하는 단면특성을 입력(I단~J단)
	CString szSectionStt = _T("");
	CString szSectionEnd = _T("");
	long sttEIVAR = 2;
	long endEIVAR = 1;
	for(long nCase=0; nCase<2; nCase++) // 0-일반단면,1-강역단면
	{
		if(nCase==1 && bRcSlab) break;
		if(nCase == 1 && pBri->m_nSelectRigidModeling != 1) break;

		CDPointArray xyArrThick;
		CString szSlab = (nCase==0) ? "SLAB" : "RGSB";
		CString szSS   = (nCase==0) ? "SB"   : "RS";
		CString szWall = (nCase==0) ? "WALL" : "RGWL";
		CString szWL   = (nCase==0) ? "WL"   : "RW";
		CString szArch = (nCase==0) ? "ARCH" : "RGAR";
		CString szAR   = (nCase==0) ? "AR"	 : "RA";

		CDPoint xySection(0,0);
		// Slab 부재============================
		for(long nUD=0; nUD<2; ++nUD)
		{
			// ARCBRIDGE-3108
			// 슬래브 두께도 상.하 슬래브 합쳐서 사용되고 있으며,
			// nCase==1(강역)인 경우는 접두어가 없어서 오류발생..
			if(nCase==0 && (nUD==0 || bUseLowerSlab))
			{
				xyArrThick.RemoveAll();
			}

			BOOL bUpperSlab = (nUD == 0)? TRUE : FALSE;
			
			for(i=0; i<nSizeJnt-1; i++)
			{
				CJoint *pJnt1 = pFEM->GetJoint((USHORT)i);
				CJoint *pJnt2 = pFEM->GetJoint((USHORT)(i+1));

				long nJointType = pJnt2->m_LongAttrib[LONG_0_JOINT_TYPE];
				if(bUpperSlab && nJointType != JOINT_UPPERSLAB) continue;
				if(!bUpperSlab && nJointType != JOINT_LOWERSLAB) continue;

				CString szHead(_T(""));
				if(bUseLowerSlab && nCase == 0)
				{
					szHead = (bUpperSlab == FALSE)? _T("L") : _T("U");
				}

				xySection.x = pJnt1->m_LongAttrib[LONG_2_JOINT_SECTION_NO];
				xySection.y = pJnt2->m_LongAttrib[LONG_2_JOINT_SECTION_NO];
				if(xyArrThick.GetIndex(xySection) == -1)
				{
					xyArrThick.Add(xySection);

					szSection.Format("%s%s%02d%02d",szHead,szSS,(long)xySection.x,(long)xySection.y);
					szSectionStt.Format("%s%s%02d",szHead,szSlab,(long)xySection.x);
					szSectionEnd.Format("%s%s%02d",szHead,szSlab,(long)xySection.y);
					pFEM->AddFrameSectionOfTaper(szSection,szSectionStt,szSectionEnd,sttEIVAR,endEIVAR);
				}
			}
		}
		//////////////////////////////////////////////////////////////////////////
		// 벽체 부재 ============================================================
		xyArrThick.RemoveAll();
		CDPointArray xyArrThickTemp;
		for(long nJijum=0; nJijum<=pBri->m_nQtyJigan; nJijum++)
		{
			if(nJijum == pBri->m_nQtyJigan)
			{
				if(bUseEndWall)
					xyArrThick.RemoveAll();
				else if(bUseInWall)
					xyArrThick = xyArrThickTemp;
			}
			else if(nJijum == 1)
			{
				if(bUseInWall)
				{
					xyArrThickTemp = xyArrThick;
					xyArrThick.RemoveAll();
				}
			}

			CJoint *pJntI = GetJointJijumOnSlab(pFEM, nJijum,TRUE);	// I단 절점
			CJoint *pJntJ = NULL;	// J단 절점
			long i=0; for(i=0; i<nSizeJnt; i++)
			{
				pJntJ = pFEM->GetJoint((USHORT)i);
				long nJointType = pJntJ->m_LongAttrib[LONG_0_JOINT_TYPE];
				long nJoitJijum = pJntJ->m_LongAttrib[LONG_1_JOINT_JIJUMNO];

				if(nJoitJijum < nJijum) continue;
				if(nJointType == JOINT_UPPERSLAB || nJointType == JOINT_LOWERSLAB) continue;
				if(bArch == FALSE && nJointType != JOINT_UPPERSLAB && nJoitJijum > nJijum)
				{
					break;
				}

				CString szHead(_T(""));
				if(nCase == 0 && (bUseEndWall || bUseInWall))
				{
					szHead = bUseEndWall? _T("S") : _T("");
					if(nJijum == pBri->m_nQtyJigan && bUseEndWall)
						szHead = _T("E");
					else if(bUseInWall && (nJijum > 0 && nJijum < pBri->m_nQtyJigan))
						szHead = _T("IN");
				}

				xySection.x = pJntI->m_LongAttrib[LONG_2_JOINT_SECTION_NO];
				xySection.y = pJntJ->m_LongAttrib[LONG_2_JOINT_SECTION_NO];
				if(pJntI->m_LongAttrib[LONG_0_JOINT_TYPE]==JOINT_UPPERSLAB) // 벽체상단부재
				{
					double dThickWall = m_pStd->GetThickWallByDist(nJijum,0);
					long nIdxThick = GetIndexSection(dThickWall,FALSE) + 1;
					xySection.x = nIdxThick;
				}

				if(xyArrThick.GetIndex(xySection) == -1)
				{
					xyArrThick.Add(xySection);
					szSection.Format("%s%s%02d%02d",szHead,szWL,(long)xySection.x,(long)xySection.y);
					szSectionStt.Format("%s%s%02d",szHead,szWall,(long)xySection.x);
					szSectionEnd.Format("%s%s%02d",szHead,szWall,(long)xySection.y);
					pFEM->AddFrameSectionOfTaper(szSection,szSectionStt,szSectionEnd,sttEIVAR,endEIVAR);
				}
				pJntI = pJntJ;
			}
			if(pBri->IsBoxType())
			{
//				pJntI = GetJointJijumOnSlab(pFEM, nJijum,TRUE);	// I단 절점
				pJntJ = GetJointJijumOnSlab(pFEM, nJijum, TRUE, FALSE, FALSE);	// 하부슬래브 절점

				CString szHead = bUseInWall? (_T("IN")) : _T("");

				double dThickWall = m_pStd->GetThickWallByDist(nJijum, 0, TRUE);
				long nIdxThick = GetIndexSection(dThickWall,FALSE) + 1;
				xySection.x = xySection.y;
				xySection.y = nIdxThick;
				if(xyArrThick.GetIndex(xySection) == -1)
				{
					xyArrThick.Add(xySection);
					szSection.Format("%s%s%02d%02d",szHead,szWL,(long)xySection.x,(long)xySection.y);
					szSectionStt.Format("%s%s%02d",szHead,szWall,(long)xySection.x);
					szSectionEnd.Format("%S%s%02d",szHead,szWall,(long)xySection.y);
					pFEM->AddFrameSectionOfTaper(szSection,szSectionStt,szSectionEnd,sttEIVAR,endEIVAR);
				}
			}

		}
		// ARCH RIB 부재=================================================================
		xyArrThick.RemoveAll();
		for(i=0; i<nSizeJnt-1; i++)
		{
			CJoint *pJnt1 = pFEM->GetJoint((USHORT)i);
			CJoint *pJnt2 = pFEM->GetJoint((USHORT)(i+1));

			long nJointType = pJnt2->m_LongAttrib[LONG_0_JOINT_TYPE];
			if(nJointType!=JOINT_ARCHRIB) continue;
			xySection.x = pJnt1->m_LongAttrib[LONG_2_JOINT_SECTION_NO];
			xySection.y = pJnt2->m_LongAttrib[LONG_2_JOINT_SECTION_NO];
			if(xyArrThick.GetIndex(xySection) == -1)
			{
				xyArrThick.Add(xySection);
				szSection.Format("%s%02d%02d",szAR,(long)xySection.x,(long)xySection.y);
				szSectionStt.Format("%s%02d",szArch,(long)xySection.x);
				szSectionEnd.Format("%s%02d",szArch,(long)xySection.y);
				pFEM->AddFrameSectionOfTaper(szSection,szSectionStt,szSectionEnd,sttEIVAR,endEIVAR);
			}
		}

	}
	//////////////////////////////////////////////////////////////////////////
	// 기둥단면
	long nSizeCol = m_dArrColumnArea.GetSize();
	double dWidthSlab = bEarthQuake ? (bLsd ? frM(m_pStd->GetSlabWidth()) : m_pStd->GetSlabWidth()) : 1.0;
	double dUnitM = bLsd ? frM(UNIT_METER) : UNIT_METER;
	for(i=0; i<nSizeCol; i++)
	{
		double Area = GetValueUnitChange(m_dArrColumnArea.GetAt(i), UNIT_CGS_M2, pData->m_szM2) / dUnitM * dWidthSlab;
		double I33  = GetValueUnitChange(m_dArrColumnI33.GetAt(i), UNIT_CGS_M4, pData->m_szM4) / dUnitM * dWidthSlab;
		double I22  = GetValueUnitChange(m_dArrColumnI22.GetAt(i), UNIT_CGS_M4, pData->m_szM4) / dUnitM * dWidthSlab;
		double J    = 0;
		szSection.Format("COLU%02d",i+1);

		szNameMat = GetMaterialNameByValue(pFEM, pBri->GetValueEc(ePartInWall, TRUE));

		pFEM->AddFrameSection(szSection,szNameMat,Area,I33,I22,J);
	}
	//////////////////////////////////////////////////////////////////////////
	// 기초단면
	if(bEarthQuake &&  pBri->IsBoxType() == FALSE && pBri->GetTypeJointPosFootingEQ()==1)
	{
		long nIdxFoot   = 0;
		long nIdxColumn = 0;
		long nJijum=0; for(nJijum=0; nJijum<=pBri->m_nQtyJigan; nJijum++)
		{
			if(pBri->m_nTypeBridge==BRIDGE_TYPE_SLAB) break;
			if(pBri->IsWall(nJijum)==FALSE)	continue;
			if(pBri->m_nTypeBridge==BRIDGE_TYPE_TRAP2 &&  (nJijum==1 || nJijum==pBri->m_nQtyJigan-1)) continue;
			if(pBri->m_nTypeBridge==BRIDGE_TYPE_ARCH  && !(nJijum==0 || nJijum==pBri->m_nQtyJigan))   continue;

			EPartOfBridge ePart = ePartInFoot;
			if(nJijum == 0) ePart = ePartFootStt;
			else if(nJijum == pBri->m_nQtyJigan) ePart = ePartFootEnd;

			szNameMat = GetMaterialNameByValue(pFEM, pBri->GetValueEc(ePart, TRUE));

			CFootingApp *pFooting = pBri->GetFootingByJijumNumber(nJijum);
			double FW = bLsd ? pFooting->GetWidth() : toM(pFooting->GetWidth());
			nIdxFoot++;
			if(pBri->GetTypePropertyPosFootingEQ()==0)
			{
				if(pBri->m_nTypeBridge==BRIDGE_TYPE_TRAP2 || pBri->m_nTypeBridge==BRIDGE_TYPE_ARCH)
					szSection.Format("FOOT%02d",nIdxFoot);
				else
					szSection.Format("FOOT%02d", nJijum+1);
				pFEM->AddFrameSectionOfRectangle(szSection,szNameMat,dWidth,FW);
			}
			else
			{
				if(pBri->m_nTypeBridge==BRIDGE_TYPE_TRAP2 || pBri->m_nTypeBridge==BRIDGE_TYPE_ARCH)
					szSection.Format("FOOT%02d",nIdxFoot);
				else
					szSection.Format("FOOT%02d", nJijum+1);
				if(pBri->GetTypeColumnByJijum(nJijum)==-1)
				{
					FW = bLsd ? pBri->GetThickWall(nJijum) : toM(pBri->GetThickWall(nJijum));
					pFEM->AddFrameSectionOfRectangle(szSection,szNameMat,dWidth,FW);
				}
				else
				{
					double Area = m_dArrColumnArea.GetAt(nIdxColumn)*dWidthSlab;
					double I33  = m_dArrColumnI33.GetAt(nIdxColumn)*dWidthSlab;
					double I22  = m_dArrColumnI22.GetAt(nIdxColumn)*dWidthSlab;
					double J    = 0;
					pFEM->AddFrameSection(szSection,szNameMat,Area,I33,I22,J);
					nIdxColumn++;
				}
			}
		}
	}
}

void CARcBridgeDesignFrame::MakeSapDataLoadLiveFootWay(CFEMManage *pFEM, long &nLoadCaseIndex, BOOL bEarthQuake)
{
	CRcBridgeRebar		*pBri  = m_pStd->m_pBri;
	CARcBridgeCalcLoad	*pLoad = m_pStd->m_pLoad;
	CARcBridgeDataStd   *pData = pBri->m_pARcBridgeDataStd;

	if(pLoad->m_dLiveLoadFootway <=0) return;

	BOOL bLsd = pData->IsLsdDesign();
	CString strLoadName(_T(""));
	CString strLoadMark(_T(""));
	
	if(bLsd)
	{
		strLoadName = pBri->m_plsdManager->GetStringLoadCaseLsd(eLSD_LOADCASE_PL, FALSE);
		strLoadMark  = pBri->m_plsdManager->GetStringLoadCaseLsd(eLSD_LOADCASE_PL, TRUE);	
	}
	else
	{
		strLoadName	= _T("보도활하중");
		strLoadMark	= _T("FW");
	}

	double dWidth = bEarthQuake ? (bLsd? frM(m_pStd->GetSlabWidth()) : m_pStd->GetSlabWidth()) : 1.0;

	long nIdxSlabStt = GetNumberElementSlabStt(pFEM, TRUE);
	long nIdxSlabEnd = GetNumberElementSlabEnd(pFEM, TRUE);

	pFEM->MakeElementLoadForce(nLoadCaseIndex+1);
	pFEM->m_szLCName.SetAt(nLoadCaseIndex, strLoadMark);
	pFEM->m_szLCComment.SetAt(nLoadCaseIndex, strLoadName);

	double dLoadValue = GetValueUnitChange(pLoad->m_dLiveLoadFootway,UNIT_CGS_TONF_M2,pData->m_szTonf_M2) * dWidth;
	for(long nIdx=nIdxSlabStt; nIdx<=nIdxSlabEnd; nIdx++)
	{
		CElement *pE = pFEM->GetElement((USHORT)nIdx);
		CLoadForce *pELoad = pE->GetLoadForce(nLoadCaseIndex);
		pELoad->m_szLoadTitle = strLoadMark;
		pELoad->SetLoadForce(0, 1.0, CVector(0,0,-dLoadValue), CVector(0,0,-dLoadValue));
	}		
	++nLoadCaseIndex;
}

void CARcBridgeDesignFrame::MakeSapDataLoadLiveAlways(CFEMManage *pFEM, long &nLoadCaseIndex, BOOL bEarthQuake, BOOL bApplyImpact)
{
	CRcBridgeRebar		*pBri  = m_pStd->m_pBri;
	CARcBridgeCalcLoad	*pLoad = m_pStd->m_pLoad;
	CARcBridgeDataStd   *pData = pBri->m_pARcBridgeDataStd;

// 	CString strLoad = _T("");
//	long nLoadCase = BRIDGE_LOAD_LIVE_FULL;

	BOOL bLsd = pData->IsLsdDesign();
	CString strLoadName(_T(""));
	CString strLoadMark(_T(""));

	double dWidth = bEarthQuake ? (bLsd? frM(m_pStd->GetSlabWidth()) : m_pStd->GetSlabWidth()) : 1.0;
	BOOL bAddCoefficientImpact = (pData->IsRailDesign2017() && pData->m_nTypeFootingSafetyApplyCoefficientImpact == 1) ? TRUE : FALSE;

	if(bLsd)
	{
		strLoadName = pBri->m_plsdManager->GetStringLoadCaseLsd(eLSD_LOADCASE_LL1, FALSE);
		strLoadMark  = pBri->m_plsdManager->GetStringLoadCaseLsd(eLSD_LOADCASE_LL1, TRUE);	
	}
	else
	{
		strLoadName	= _T("상재활하중");
		strLoadMark	= pData->GetStringLoadCase(BRIDGE_LOAD_LIVE_FULL);
		if(bApplyImpact == FALSE)
			strLoadMark += _T("-S");
	}

	long nIdxSlabStt = GetNumberElementSlabStt(pFEM, TRUE);
	long nIdxSlabEnd = GetNumberElementSlabEnd(pFEM, TRUE);
	long nSizeEle    = pFEM->GetElementSize();

	pFEM->MakeElementLoadForce(nLoadCaseIndex+1);
	pFEM->m_szLCName.SetAt(nLoadCaseIndex, strLoadMark);
	pFEM->m_szLCComment.SetAt(nLoadCaseIndex, strLoadName);

	// 슬래브
	double dLoadValue = GetValueUnitChange(pLoad->m_dLiveLoaUR_Pvl,UNIT_CGS_TONF_M2,pData->m_szTonf_M2) * dWidth;
	if(bAddCoefficientImpact && bApplyImpact)
		dLoadValue *= (1+pLoad->m_dCoefficientImpact[0]);
	for(long nIdx=nIdxSlabStt; nIdx<=nIdxSlabEnd; nIdx++)
	{
		CElement *pE = pFEM->GetElement((USHORT)nIdx);
		CLoadForce *pELoad = pE->GetLoadForce(nLoadCaseIndex);
		pELoad->m_szLoadTitle = strLoadMark;
		pELoad->SetLoadForce(0, 1.0, CVector(0,0,-dLoadValue), CVector(0,0,-dLoadValue));
	}

	dLoadValue = GetValueUnitChange(pLoad->m_dLiveLoaUR_Phl,UNIT_CGS_TONF_M2,pData->m_szTonf_M2) * dWidth;
	if(bAddCoefficientImpact && bApplyImpact)
		dLoadValue *= (1+pLoad->m_dCoefficientImpact[0]);
	// 측벽에 작용하는 상재하중
	if(Compare(dLoadValue, 0.0, _T(">")))
	{
		// 시점측벽체
		long i=0; for(i=0; i<nSizeEle; i++)
		{
			CElement *pE = pFEM->GetElement((USHORT)i);			
			if(pE->m_nFlag!=ELEMENT_STTWALL) continue;
			CLoadForce *pELoad = pE->GetLoadForce(nLoadCaseIndex);
			pELoad->m_szLoadTitle = strLoadMark;
			pELoad->SetLoadForce(0, 1.0, CVector(dLoadValue,0,0), CVector(dLoadValue,0,0));			
		}
		// 종점측벽체
		for(i=0; i<nSizeEle; i++)
		{
			CElement *pE = pFEM->GetElement((USHORT)i);			
			if(pE->m_nFlag!=ELEMENT_ENDWALL) continue;
			CLoadForce *pELoad = pE->GetLoadForce(nLoadCaseIndex);
			pELoad->m_szLoadTitle = strLoadMark;
			pELoad->SetLoadForce(0, 1.0, CVector(-dLoadValue,0,0), CVector(-dLoadValue,0,0));
		}
	}

	++nLoadCaseIndex;

	if(bApplyImpact && bAddCoefficientImpact)
	{
		// 충격계수가 없는 걸로 한번더 
		MakeSapDataLoadLiveAlways(pFEM, nLoadCaseIndex, bEarthQuake, FALSE);
	}
}

void CARcBridgeDesignFrame::MakeSapDataLoadAnchor(CFEMManage *pFEM, long &nLoadCaseIndex)
{
	CRcBridgeRebar		*pBri  = m_pStd->m_pBri;
	CARcBridgeDataStd   *pData = pBri->m_pARcBridgeDataStd;

	if(pBri->m_bApplyAnchorForce == FALSE) return;
	if(pBri->m_nTypeLower != BOXFOOTING_TYPE_ANCHOR) return;

// 	CString strLoad = _T("");
	long nSizeEle    = pFEM->GetElementSize();

	pFEM->MakeElementLoadForce(nLoadCaseIndex+1);

	CString strLoad   = _T("AN");
	pFEM->m_szLCName.SetAt(nLoadCaseIndex, strLoad);
	pFEM->m_szLCComment.SetAt(nLoadCaseIndex, "부력방지앵커");		

	// 하부슬래브
	CDoubleArray dArrDistAnchor, dArrFroceAnchor;
	for(long nIdx = 0; nIdx < nSizeEle; nIdx++)
	{
		CElement *pE = pFEM->GetElement((USHORT)nIdx);
		if(pE->m_nFlag != ELEMENT_LOWERSLAB) continue;

		GetArrayAnchorForceAndDist(pFEM, pE, dArrFroceAnchor, dArrDistAnchor, FALSE);

		for(long ix = 0; ix < dArrFroceAnchor.GetSize(); ix++)
		{
			CLoadForce *pELoad = pE->GetLoadForce(nLoadCaseIndex);
			pELoad->m_szLoadTitle = strLoad;
			pELoad->SetDistForce(dArrDistAnchor.GetAt(ix), CVector(0, 0, -dArrFroceAnchor.GetAt(ix)), _T(""), FALSE);
		}
	}
	++nLoadCaseIndex;
}


void CARcBridgeDesignFrame::MakeSapDataLoadWater(CFEMManage *pFEM, long &nLoadCaseIndex, BOOL bEarthQuake)
{
	CRcBridgeRebar		*pBri  = m_pStd->m_pBri;
	CARcBridgeCalcLoad	*pLoad = m_pStd->m_pLoad;
	CARcBridgeDataStd   *pData = pBri->m_pARcBridgeDataStd;
	CLineInfo *pLine = pBri->GetLineBase();

	BOOL bLsd = pData->IsLsdDesign();
	BOOL bApplyLoadVert(FALSE);
	BOOL bApplyLoadHori(FALSE);
	CString strLoadName(_T(""));
	CString strLoadMark(_T(""));
	if(bLsd)
	{
		bApplyLoadVert = pBri->m_plsdManager->IsSelectedLoadDesign(eLSD_LOADSELECT_EV);
		bApplyLoadHori = pBri->m_plsdManager->IsSelectedLoadDesign(eLSD_LOADSELECT_WA);
	}
	else
	{
		bApplyLoadVert = pBri->m_bApplyLoad[BRIDGE_LOAD_WATERVERT];
		bApplyLoadHori = pBri->m_bApplyLoad[BRIDGE_LOAD_WATERHORI];
	}
	
	long nIdxSlabStt = GetNumberElementSlabStt(pFEM, TRUE);
	long nIdxSlabEnd = GetNumberElementSlabEnd(pFEM, TRUE);
	long nSizeEle    = pFEM->GetElementSize();

	double dWidth = bEarthQuake ? (bLsd? frM(m_pStd->GetSlabWidth()) : m_pStd->GetSlabWidth()) : 1.0;

	// 수평수압
	if(bApplyLoadHori && (pBri->m_bApplyGroundWaterLevel || bEarthQuake))
	{
		if(bLsd)
		{
			strLoadName = pBri->m_plsdManager->GetStringLoadCaseLsd(eLSD_LOADCASE_WA1, FALSE);
			strLoadMark  = pBri->m_plsdManager->GetStringLoadCaseLsd(eLSD_LOADCASE_WA1, TRUE);
		}
		else
		{
			strLoadName = _T("수평수압");
			strLoadMark = pData->GetStringLoadCase(BRIDGE_LOAD_WATERHORI);
		}
		
		double dUT = pBri->GetThickPave() + pBri->m_dTS / 2.;
		double dEle_Left  = toM(pLine->GetElevationPlanEl(m_pStd->m_pFrame->m_vArrSlab[iUPPER].GetAt(0).x) - dUT);
		double dEle_Right = pBri->m_bSlopeLowerSlab ? dEle_Left : toM(pLine->GetElevationPlanEl(m_pStd->m_pFrame->m_vArrSlab[iUPPER].GetAt(m_pStd->m_pFrame->m_vArrSlab[iUPPER].GetSize()-1).x) - dUT);
		if(bLsd)
		{
			dEle_Left = frM(dEle_Left);
			dEle_Right = frM(dEle_Right);
		}

		pFEM->MakeElementLoadForce(nLoadCaseIndex+1);
		pFEM->m_szLCName.SetAt(nLoadCaseIndex, strLoadMark);
		pFEM->m_szLCComment.SetAt(nLoadCaseIndex, strLoadName);

		double dWaterLoadUp = 0;
		double dWaterLoadDn = 0;
		for(long iSE = iSTT; iSE <= iEND; iSE++)
		{
			double dUpperEL = (iSE == iSTT) ? dEle_Left : dEle_Right;
			double dUpperStt = (iSE == iSTT) ? pFEM->GetJoint((USHORT)nIdxSlabStt)->m_vPointOrigin.z : pFEM->GetJoint((USHORT)(nIdxSlabEnd + 1))->m_vPointOrigin.z;

			double dEL_Stt = 0, dEL_End = 0;

			double sw = (iSE == iSTT) ? 1.0 : -1.0;
			long i=0; for(i=0; i<nSizeEle; i++)
			{
				CElement *pE = pFEM->GetElement((USHORT)i);	
				if(iSE == iSTT && pE->m_nFlag != ELEMENT_STTWALL) continue;
				if(iSE == iEND && pE->m_nFlag != ELEMENT_ENDWALL) continue;

				CLoadForce *pELoad = pE->GetLoadForce(nLoadCaseIndex);
				CJoint *pJntI = pFEM->GetJoint(pE->m_nJStt);
				CJoint *pJntJ = pFEM->GetJoint(pE->m_nJEnd);

				dEL_Stt = dUpperEL - (dUpperStt - pJntI->m_vPointOrigin.z);
				dEL_End = dUpperEL - (dUpperStt - pJntJ->m_vPointOrigin.z);
				if(bLsd)
				{
					dEL_Stt = toM(dEL_Stt);
					dEL_End = toM(dEL_End);
				}

				dWaterLoadUp = GetValueUnitChange(pLoad->GetLoadWaterPressure(dEL_Stt) * dWidth,UNIT_CGS_TONF_M2,pData->m_szTonf_M2);
				dWaterLoadDn = GetValueUnitChange(pLoad->GetLoadWaterPressure(dEL_End) * dWidth,UNIT_CGS_TONF_M2,pData->m_szTonf_M2);
				if(dWaterLoadDn <= 0) continue;

				pELoad->m_szLoadTitle = strLoadMark;
				pELoad->SetLoadForce(0, 1.0, CVector(dWaterLoadUp * sw,0,0), CVector(dWaterLoadDn * sw,0,0));		
			}
		}

		++nLoadCaseIndex;
	}

	// 연직수압
	if(bApplyLoadVert && (pBri->m_bApplyGroundWaterLevel || bEarthQuake))
	{
		if(bLsd)
		{
			strLoadName = pBri->m_plsdManager->GetStringLoadCaseLsd(eLSD_LOADCASE_EVVW, FALSE);
			strLoadMark  = pBri->m_plsdManager->GetStringLoadCaseLsd(eLSD_LOADCASE_EVVW, TRUE);
		}
		else
		{
			strLoadName = _T("연직수압");
			strLoadMark = pData->GetStringLoadCase(BRIDGE_LOAD_WATERVERT);
		}

		pFEM->MakeElementLoadForce(nLoadCaseIndex+1);
		pFEM->m_szLCName.SetAt(nLoadCaseIndex, strLoadMark);
		pFEM->m_szLCComment.SetAt(nLoadCaseIndex, strLoadName);		

		// Lsd에서는 부력을 별도의 하중으로 분리
		if(bLsd == FALSE)
		{
			// 하부슬래브
			for(long nIdx = 0; nIdx < nSizeEle; nIdx++)
			{
				CElement *pE = pFEM->GetElement((USHORT)nIdx);
				if(pE->m_nFlag != ELEMENT_LOWERSLAB) continue;

				CLoadForce *pELoad = pE->GetLoadForce(nLoadCaseIndex);
				pELoad->m_szLoadTitle = strLoadMark;
				pELoad->SetLoadForce(0, 1.0, CVector(0, 0, pLoad->m_dLoadBuoyancy * dWidth), CVector(0, 0, pLoad->m_dLoadBuoyancy * dWidth));	
			}
		}
		
		// 부상방지판
		double dDistStt = pFEM->GetJoint((USHORT)nIdxSlabStt)->m_vPointOrigin.x;
		double dDistEnd = pFEM->GetJoint((USHORT)(nIdxSlabEnd + 1))->m_vPointOrigin.x;
		for(long nIdx = 0; nIdx < nSizeEle; nIdx++)
		{
			CElement *pE = pFEM->GetElement((USHORT)nIdx);
			if(pE->m_nFlag != ELEMENT_LOWERSLAB) continue;
			CJoint *pJntI = pFEM->GetJoint(pE->m_nJStt);
			CJoint *pJntJ = pFEM->GetJoint(pE->m_nJEnd);

			if(pJntI->m_vPointOrigin.x < dDistStt || pJntJ->m_vPointOrigin.x > dDistEnd)
			{
				CLoadForce *pELoad = pE->GetLoadForce(nLoadCaseIndex);
				pELoad->m_szLoadTitle = strLoadMark;
				pELoad->SetLoadForce(0, 1.0, CVector(0, 0, -GetValueUnitChange(pLoad->m_dLoadWaterVert * dWidth,UNIT_CGS_TONF_M2,pData->m_szTonf_M2)), CVector(0, 0, -GetValueUnitChange(pLoad->m_dLoadWaterVert * dWidth,UNIT_CGS_TONF_M2,pData->m_szTonf_M2)));	
			}
		}
		++nLoadCaseIndex;
	}

	if(pBri->m_bApplyGroundWaterLevel && bLsd && pBri->m_plsdManager->IsSelectedLoadDesign(eLSD_LOADSELECT_BP) && pLoad->m_dLoadBuoyancy > 0)
	{
		// 부력 - 강도설계에서는 연직수압에 포함
		strLoadName = pBri->m_plsdManager->GetStringLoadCaseLsd(eLSD_LOADCASE_BP, FALSE);
		strLoadMark  = pBri->m_plsdManager->GetStringLoadCaseLsd(eLSD_LOADCASE_BP, TRUE);

		pFEM->MakeElementLoadForce(nLoadCaseIndex+1);
		pFEM->m_szLCName.SetAt(nLoadCaseIndex, strLoadMark);
		pFEM->m_szLCComment.SetAt(nLoadCaseIndex, strLoadName);

		double dLoadValueBuoyancy = GetValueUnitChange(pLoad->m_dLoadBuoyancy * dWidth, UNIT_CGS_TONF_M2, pData->m_szTonf_M2);

		// 하부슬래브
		for(long nIdx = 0; nIdx < nSizeEle; nIdx++)
		{
			CElement *pE = pFEM->GetElement((USHORT)nIdx);
			if(pE->m_nFlag != ELEMENT_LOWERSLAB) continue;

			CLoadForce *pELoad = pE->GetLoadForce(nLoadCaseIndex);
			pELoad->m_szLoadTitle = strLoadMark;
			pELoad->SetLoadForce(0, 1.0, CVector(0, 0, dLoadValueBuoyancy), CVector(0, 0, dLoadValueBuoyancy));	
		}

		++nLoadCaseIndex;
	}

	// 내부 수압
	if(pBri->IsBoxType() && pBri->m_dHeightInteriorWaterPress > 0)
	{
		strLoadName = _T("");
		strLoadMark = _T("");
		if(bLsd)
		{
			ELSDLoadCaseKind eLoadCase(eLSD_LOADCASE_EVI);
			ELSDLoadSelectKind eLoadSelect(eLSD_LOADSELECT_EV);

			switch (pBri->m_nSelectInteriorWaterPress)
			{
			case 0:
				eLoadCase = eLSD_LOADCASE_EVI;
				eLoadSelect = eLSD_LOADSELECT_EV;
				break;
			case 1:
				eLoadCase = eLSD_LOADCASE_BPI;
				eLoadSelect = eLSD_LOADSELECT_BP;
				break;
			case 2:
				eLoadCase = eLSD_LOADCASE_WA21;
				eLoadSelect = eLSD_LOADSELECT_WA;
				break;
			}

			if(pBri->m_plsdManager->IsSelectedLoadDesign(eLoadSelect))
			{
				strLoadName = pBri->m_plsdManager->GetStringLoadCaseLsd(eLoadCase, FALSE);
				strLoadMark  = pBri->m_plsdManager->GetStringLoadCaseLsd(eLoadCase, TRUE);
			}
		}
		else
		{
			strLoadName = _T("내부수압");
			strLoadMark = _T("IW");
		}

		if(strLoadMark.IsEmpty()==FALSE && strLoadName.IsEmpty()==FALSE)
		{
			pFEM->MakeElementLoadForce(nLoadCaseIndex+1);
			pFEM->m_szLCName.SetAt(nLoadCaseIndex, strLoadMark);
			pFEM->m_szLCComment.SetAt(nLoadCaseIndex, strLoadName);

			double dUW_Water = GetValueUnitChange(toTon(pData->m_pBasicConcInfo->m_UWeightWater), UNIT_CGS_TONF_M3, pData->m_szTonf_M3);
			double dHeightWater = bLsd ? pBri->m_dHeightInteriorWaterPress : toM(pBri->m_dHeightInteriorWaterPress);

			double dDistStt = pFEM->GetJoint((USHORT)nIdxSlabStt)->m_vPointOrigin.x;
			double dDistEnd = pFEM->GetJoint((USHORT)(nIdxSlabEnd + 1))->m_vPointOrigin.x;
			// 하부슬래브
			for(long nIdx = 0; nIdx < nSizeEle; nIdx++)
			{
				CElement *pE = pFEM->GetElement((USHORT)nIdx);
				if(pE->m_nFlag != ELEMENT_LOWERSLAB) continue;
				CJoint *pJntI = pFEM->GetJoint(pE->m_nJStt);
				CJoint *pJntJ = pFEM->GetJoint(pE->m_nJEnd);

				// 부상방지 저판 제외
				if(pJntI->m_vPointOrigin.x >= dDistStt && pJntJ->m_vPointOrigin.x <= dDistEnd)
				{
					CLoadForce *pELoad = pE->GetLoadForce(nLoadCaseIndex);
					pELoad->m_szLoadTitle = strLoadMark;
					pELoad->SetLoadForce(0, 1.0, CVector(0, 0, dUW_Water * dWidth * dHeightWater * -1), CVector(0, 0, dUW_Water * dWidth * dHeightWater * -1));	
				}
			}

			++nLoadCaseIndex;
		}
	}
}


void CARcBridgeDesignFrame::MakeSapDataLoadNormal(CFEMManage *pFEM)
{
	CRcBridgeRebar		*pBri  = m_pStd->m_pBri;
	CARcBridgeCalcLoad	*pLoad = m_pStd->m_pLoad;
	CARcBridgeDataStd   *pData = pBri->m_pARcBridgeDataStd;

	//////////////////////////////////////////////////////////////////////////
// 	double dSelfWeight = 1.0;
	double dLoadValue  = GetValueUnitChange(pLoad->m_DeadLoadDistribution[0],UNIT_CGS_TONF,pData->m_szTonf);
	long nIdxSlabStt = GetNumberElementSlabStt(pFEM);
	long nIdxSlabEnd = GetNumberElementSlabEnd(pFEM);
	long nSizeEle    = pFEM->GetElementSize();

	long nLoadCaseIndex = 0;

	BOOL bOutWall = pBri->IsOutsideWall();

	long nLoadCase   = BRIDGE_LOAD_DEAD;
	CString strLoad  = pData->GetStringLoadCase(nLoadCase);
	if(pBri->m_bApplyLoad[BRIDGE_LOAD_DEAD])
	{
		double dSelfWeight = 1.0;

		pFEM->MakeElementLoadForce(nLoadCaseIndex+1);

		pFEM->m_szLCName.SetAt(nLoadCaseIndex, strLoad);
		pFEM->m_szLCComment.SetAt(nLoadCaseIndex, "구체자중 및 사하중");
		pFEM->m_dSelfWeight.SetAt(nLoadCaseIndex,dSelfWeight);	
		for(long nIdx=nIdxSlabStt; nIdx<=nIdxSlabEnd; nIdx++)
		{
			if(dLoadValue==0) break;
			CElement *pE = pFEM->GetElement((USHORT)nIdx);
			CLoadForce *pELoad = pE->GetLoadForce(nLoadCaseIndex);
			pELoad->m_szLoadTitle = strLoad;
			pELoad->SetLoadForce(0, 1.0, CVector(0,0,-dLoadValue), CVector(0,0,-dLoadValue));
		}		
		++nLoadCaseIndex;
	}

	nLoadCase  = BRIDGE_LOAD_SOIL;
	dLoadValue = GetValueUnitChange(pLoad->m_dSoilPressureVert,UNIT_CGS_TONF,pData->m_szTonf);
	if(pBri->m_bApplyLoad[nLoadCase] && (Compare(dLoadValue,0.0,">") || (pBri->IsBoxType() && pBri->m_bExistProtectionRiseFooting)))
	{

		pFEM->MakeElementLoadForce(nLoadCaseIndex+1);

		strLoad   = pData->GetStringLoadCase(nLoadCase);				
		pFEM->m_szLCName.SetAt(nLoadCaseIndex, strLoad);
		pFEM->m_szLCComment.SetAt(nLoadCaseIndex, "연직토압");		

		if(Compare(dLoadValue,0.0,">"))
		{
			for(long nIdx=nIdxSlabStt; nIdx<=nIdxSlabEnd; nIdx++)
			{
				CElement *pE = pFEM->GetElement((USHORT)nIdx);
				CLoadForce *pELoad = pE->GetLoadForce(nLoadCaseIndex);
				pELoad->m_szLoadTitle = strLoad;
				pELoad->SetLoadForce(0, 1.0, CVector(0,0,-dLoadValue), CVector(0,0,-dLoadValue));
			}
		}
		if(pBri->IsBoxType() && pBri->m_bExistProtectionRiseFooting)
		{
			// 부상방지 저판에 작용하는 연직토압
			double dHStt = pFEM->GetJoint(0)->m_vPointOrigin.z;
			double dHEnd = dHStt + m_pStd->GetElDiffModelingEnd();
			double dDistStt = pFEM->GetJoint((USHORT)nIdxSlabStt)->m_vPointOrigin.x;
			double dDistEnd = pFEM->GetJoint((USHORT)(nIdxSlabEnd + 1))->m_vPointOrigin.x;
			for(long nIdx = 0; nIdx < nSizeEle; nIdx++)
			{
				CElement *pE = pFEM->GetElement((USHORT)nIdx);
				if(pE->m_nFlag != ELEMENT_LOWERSLAB) continue;
				CJoint *pJntI = pFEM->GetJoint(pE->m_nJStt);
				CJoint *pJntJ = pFEM->GetJoint(pE->m_nJEnd);

				// 시점 부상방지 저판
				if(pJntI->m_vPointOrigin.x < dDistStt)
				{
					double dVertSoil = GetValueUnitChange(pLoad->GetLoadSoilPressure(dHStt - pJntI->m_vPointOrigin.z, TRUE, FALSE, FALSE, eGoalEQ_None, TRUE),UNIT_CGS_TONF,pData->m_szTonf);
					
					CElement *pE = pFEM->GetElement((USHORT)nIdx);
					CLoadForce *pELoad = pE->GetLoadForce(nLoadCaseIndex);
					pELoad->m_szLoadTitle = strLoad;
					pELoad->SetLoadForce(0, 1.0, CVector(0,0,-dVertSoil), CVector(0,0,-dVertSoil));
				}
				else if(pJntJ->m_vPointOrigin.x > dDistEnd)
				{
					double dVertSoil = GetValueUnitChange(pLoad->GetLoadSoilPressure(dHEnd - pJntI->m_vPointOrigin.z, TRUE, FALSE, FALSE, eGoalEQ_None, TRUE),UNIT_CGS_TONF,pData->m_szTonf);
					
					CElement *pE = pFEM->GetElement((USHORT)nIdx);
					CLoadForce *pELoad = pE->GetLoadForce(nLoadCaseIndex);
					pELoad->m_szLoadTitle = strLoad;
					pELoad->SetLoadForce(0, 1.0, CVector(0,0,-dVertSoil), CVector(0,0,-dVertSoil));
				}
			}
		}
		
		++nLoadCaseIndex;
	}

	//////////////////////////////////////////////////////////////////////////
	nLoadCase = BRIDGE_LOAD_TOAP_BOTH;
	if(pBri->m_bApplyLoad[nLoadCase] && bOutWall)
	{
		for(long nCase = 0; nCase < 2; nCase++)
		{
			if(nCase == 1 && (pBri->IsBoxType() == FALSE || pBri->m_bApplyGroundWaterLevel == FALSE)) 
			{
				break;
			}

			BOOL bWater = nCase == 1 ? TRUE : FALSE; // 지하수가 있는 경우
			pFEM->MakeElementLoadForce(nLoadCaseIndex+1);

			strLoad.Format(_T("%s%s"), pData->GetStringLoadCase(nLoadCase), bWater ? _T("W") : _T("")); 
			pFEM->m_szLCName.SetAt(nLoadCaseIndex, strLoad);
			pFEM->m_szLCComment.SetAt(nLoadCaseIndex, bWater ? _T("수평토압(지하수)") : _T("수평토압(양토압)"));

			double dToapUp = 0;
			double dToapDn = 0;
			for(long iSE = iSTT; iSE <= iEND; iSE++)
			{
				double sw = (iSE == iSTT) ? 1.0 : -1.0;
				double dHStt = (iSE == iSTT) ? pFEM->GetJoint(0)->m_vPointOrigin.z : pFEM->GetJoint(0)->m_vPointOrigin.z + m_pStd->GetElDiffModelingEnd();

				long i=0; for(i=0; i<nSizeEle; i++)
				{
					CElement *pE = pFEM->GetElement((USHORT)i);	
					if(iSE == iSTT && pE->m_nFlag != ELEMENT_STTWALL) continue;
					if(iSE == iEND && pE->m_nFlag != ELEMENT_ENDWALL) continue;

					CLoadForce *pELoad = pE->GetLoadForce(nLoadCaseIndex);
					CJoint *pJntI = pFEM->GetJoint(pE->m_nJStt);
					CJoint *pJntJ = pFEM->GetJoint(pE->m_nJEnd);

					dToapUp = pLoad->GetLoadSoilPressure(dHStt - pJntI->m_vPointOrigin.z, iSE == iSTT, bWater);
					dToapDn = pLoad->GetLoadSoilPressure(dHStt - pJntJ->m_vPointOrigin.z, iSE == iSTT, bWater);
					dToapUp = GetValueUnitChange(dToapUp,UNIT_CGS_TONF,pData->m_szTonf) * sw;
					dToapDn = GetValueUnitChange(dToapDn,UNIT_CGS_TONF,pData->m_szTonf) * sw;

					pELoad->m_szLoadTitle = strLoad;
					pELoad->SetLoadForce(0, 1.0, CVector(dToapUp,0,0), CVector(dToapDn,0,0));		
				}
			}

			++nLoadCaseIndex;
		}
	}

	nLoadCase = BRIDGE_LOAD_TOAP_ONE;
	if(pBri->m_bApplyLoad[nLoadCase] && bOutWall)
	{
		// 시점측벽체
		pFEM->MakeElementLoadForce(nLoadCaseIndex+1);
		strLoad   = pData->GetStringLoadCase(nLoadCase) + "L";
		pFEM->m_szLCName.SetAt(nLoadCaseIndex, strLoad);
		pFEM->m_szLCComment.SetAt(nLoadCaseIndex, "수평토압(시점벽체)");

		CJoint *pJntStt = pFEM->GetJoint(0);
		double dToapUp = 0;
		double dToapDn = 0;
		long i=0; for(i=0; i<nSizeEle; i++)
		{
			CElement *pE = pFEM->GetElement((USHORT)i);			
			if(pE->m_nFlag!=ELEMENT_STTWALL) continue;
			CLoadForce *pELoad = pE->GetLoadForce(nLoadCaseIndex);
			CJoint *pJntI = pFEM->GetJoint(pE->m_nJStt);
			CJoint *pJntJ = pFEM->GetJoint(pE->m_nJEnd);
			dToapUp = pLoad->GetLoadSoilPressure(pJntStt->m_vPointOrigin.z - pJntI->m_vPointOrigin.z);
			dToapDn = pLoad->GetLoadSoilPressure(pJntStt->m_vPointOrigin.z - pJntJ->m_vPointOrigin.z);			
			dToapUp = GetValueUnitChange(dToapUp,UNIT_CGS_TONF,pData->m_szTonf);
			dToapDn = GetValueUnitChange(dToapDn,UNIT_CGS_TONF,pData->m_szTonf);
			pELoad->m_szLoadTitle = strLoad;
			pELoad->SetLoadForce(0, 1.0, CVector(dToapUp,0,0), CVector(dToapDn,0,0));			
		}
		++nLoadCaseIndex;

		// 종점측 벽체
		pJntStt = pFEM->GetJoint(m_vArrSlab[iUPPER].GetSize() - 1);
		pFEM->MakeElementLoadForce(nLoadCaseIndex+1);
		strLoad   = pData->GetStringLoadCase(nLoadCase) + "R";
		pFEM->m_szLCName.SetAt(nLoadCaseIndex, strLoad);
		pFEM->m_szLCComment.SetAt(nLoadCaseIndex, "수평토압(종점벽체)");

		for(i=0; i<nSizeEle; i++)
		{
			CElement *pE = pFEM->GetElement((USHORT)i);			
			if(pE->m_nFlag!=ELEMENT_ENDWALL) continue;
			CLoadForce *pELoad = pE->GetLoadForce(nLoadCaseIndex);
			CJoint *pJntI = pFEM->GetJoint(pE->m_nJStt);
			CJoint *pJntJ = pFEM->GetJoint(pE->m_nJEnd);
			dToapUp = pLoad->GetLoadSoilPressure(pJntStt->m_vPointOrigin.z - pJntI->m_vPointOrigin.z);
			dToapDn = pLoad->GetLoadSoilPressure(pJntStt->m_vPointOrigin.z - pJntJ->m_vPointOrigin.z);
			dToapUp = GetValueUnitChange(dToapUp,UNIT_CGS_TONF,pData->m_szTonf);
			dToapDn = GetValueUnitChange(dToapDn,UNIT_CGS_TONF,pData->m_szTonf);
			pELoad->m_szLoadTitle = strLoad;
			pELoad->SetLoadForce(0, 1.0, CVector(-dToapUp,0,0), CVector(-dToapDn,0,0));
		}
		++nLoadCaseIndex;
	}

	//////////////////////////////////////////////////////////////////////////
	MakeSapDataLoadTemperature(pFEM, nLoadCaseIndex);
	//////////////////////////////////////////////////////////////////////////
	nLoadCase = BRIDGE_LOAD_SHRINK;
	if(pBri->m_bApplyLoad[nLoadCase])
	{
		pFEM->MakeElementLoadForce(nLoadCaseIndex+1);

		strLoad   = pData->GetStringLoadCase(nLoadCase);		
		pFEM->m_szLCName.SetAt(nLoadCaseIndex, strLoad);
		pFEM->m_szLCComment.SetAt(nLoadCaseIndex, "건조수축(-)");		

		if(pBri->m_bSelectShrinkLoadAddWall)
		{
			// 전체 재하
			for(long nIdx=0; nIdx<nSizeEle; nIdx++)
			{
				CElement *pE = pFEM->GetElement((USHORT)nIdx);
				CLoadForce *pELoad = pE->GetLoadForce(nLoadCaseIndex);
				pELoad->m_szLoadTitle = strLoad;
				pELoad->SetShrink(-pLoad->m_dLoadDryAndShrinkage,0,0);
			}
		}
		else
		{
			// Slab에만 재하
			for(long nIdx=nIdxSlabStt; nIdx<=nIdxSlabEnd; nIdx++)
			{
				CElement *pE = pFEM->GetElement((USHORT)nIdx);
				CLoadForce *pELoad = pE->GetLoadForce(nLoadCaseIndex);
				pELoad->m_szLoadTitle = strLoad;
				pELoad->SetShrink(-pLoad->m_dLoadDryAndShrinkage,0,0);
			}
		}
		++nLoadCaseIndex;		
	}

	//////////////////////////////////////////////////////////////////////////
	if(pBri->IsBoxType())
	{
		// 수평수압
		// 연직수압
		MakeSapDataLoadWater(pFEM, nLoadCaseIndex, FALSE);
		// 앵커
		MakeSapDataLoadAnchor(pFEM, nLoadCaseIndex);
	}

	//////////////////////////////////////////////////////////////////////////
	MakeSapDataLoadSubsid(pFEM, nLoadCaseIndex);
	
	//////////////////////////////////////////////////////////////////////////
	nLoadCase  = BRIDGE_LOAD_LR;
	dLoadValue = GetValueUnitChange(pLoad->m_dRail_LoadLR,UNIT_CGS_TONF,pData->m_szTonf);
	double dLoadValueMoment = GetValueUnitChange(pLoad->m_dRail_MomentLR,UNIT_CGS_TONF,pData->m_szTonf);
	if(pBri->m_bApplyLoad[nLoadCase] && Compare(dLoadValue,0.0,">"))
	{
		// () 역방향 장대레일 종하중 추가.
		for(long nDir=0; nDir<2; ++nDir)
		{
			BOOL bRev = (nDir == 0)? FALSE : TRUE;
			pFEM->MakeElementLoadForce(nLoadCaseIndex+1);

			long nFactor = 1;
			strLoad   = pData->GetStringLoadCase(nLoadCase);
			if(bRev)
			{
				strLoad = strLoad + _T("2");
				nFactor = -1;
			}
			pFEM->m_szLCName.SetAt(nLoadCaseIndex, strLoad);
			pFEM->m_szLCComment.SetAt(nLoadCaseIndex, "장대레일 종하중");

			for(long nIdx=nIdxSlabStt; nIdx<=nIdxSlabEnd; nIdx++)
			{
				CElement *pE = pFEM->GetElement((USHORT)nIdx);
				CLoadForce *pELoad = pE->GetLoadForce(nLoadCaseIndex);
				pELoad->m_szLoadTitle = strLoad;
				pELoad->SetLoadForce(0, 1.0, CVector(dLoadValue * nFactor,0,0), CVector(dLoadValue * nFactor,0,0), CVector(0,dLoadValueMoment * nFactor,0), CVector(0,dLoadValueMoment * nFactor,0));
			}		
			++nLoadCaseIndex;		
		}
	}

	//////////////////////////////////////////////////////////////////////////
	nLoadCase  = BRIDGE_LOAD_SB;
	dLoadValue = GetValueUnitChange(pLoad->m_dRail_LoadSB,UNIT_CGS_TONF,pData->m_szTonf);
	dLoadValueMoment = GetValueUnitChange(pLoad->m_dRail_MomentSB,UNIT_CGS_TONF,pData->m_szTonf);
	if(pBri->m_bApplyLoad[nLoadCase] && Compare(dLoadValue,0.0,">"))
	{
		pFEM->MakeElementLoadForce(nLoadCaseIndex+1);


		strLoad   = pData->GetStringLoadCase(nLoadCase);		
		pFEM->m_szLCName.SetAt(nLoadCaseIndex, strLoad);
		pFEM->m_szLCComment.SetAt(nLoadCaseIndex, "시동 및 제동하중");

		for(long nIdx=nIdxSlabStt; nIdx<=nIdxSlabEnd; nIdx++)
		{
			CElement *pE = pFEM->GetElement((USHORT)nIdx);
			CLoadForce *pELoad = pE->GetLoadForce(nLoadCaseIndex);
			pELoad->m_szLoadTitle = strLoad;
			pELoad->SetLoadForce(0, 1.0, CVector(dLoadValue,0,0), CVector(dLoadValue,0,0), CVector(0,dLoadValueMoment,0), CVector(0,dLoadValueMoment,0));
		}		
		++nLoadCaseIndex;		
	}

	//////////////////////////////////////////////////////////////////////////
	nLoadCase = BRIDGE_LOAD_LIVE_FULL;
	if(pBri->m_bApplyLoad[nLoadCase] || pBri->m_bApplyLoad[BRIDGE_LOAD_TANK])
	{
		// 보도활하중
		MakeSapDataLoadLiveFootWay(pFEM, nLoadCaseIndex, FALSE);

		// 활하중 //////////////////////////////////////////////////////////////////////////
		MakeSapDataLoadLiveLoadAll(pFEM, nLoadCaseIndex, FALSE);
	}
}

// D,L,H : 고정하중,활하중,수평토압
// G1,G2,G3 : 온도하중(+),온도하중(-),건조수축(-)
// S : 지점침하
void CARcBridgeDesignFrame::MakeSapDataCombineNormal(CFEMManage *pFEM, BOOL bEarthQuake, BOOL b3DPlate)
{
	CRcBridgeRebar		*pBri  = m_pStd->m_pBri;
	CARcBridgeCalcLoad	*pLoad = m_pStd->m_pLoad;
	CARcBridgeDataStd   *pData = pBri->m_pARcBridgeDataStd;

	CString str = _T(""), sHead = _T(""), strLoad = _T("");

	long nLoadCase = 0;
	long nSizeComboUse = pBri->m_MatComboUse.GetRow();
	long nSizeComboUlt = pBri->m_MatComboUlt.GetRow();
	long nSizeComboEcc = pBri->m_MatComboEcc.GetRow();

	BOOL bLSD = pBri->m_pARcBridgeDataStd->IsLsdDesign();
	BOOL bBOX = pBri->IsBoxType();

	BOOL bEnvToapOne = pBri->m_bSelectEnvToapOne;
	BOOL bEnvSubsid = pBri->m_bSelectEnvSubsid || b3DPlate ? TRUE : FALSE;

	MakeSapDataCombineLiveLoad(pFEM, FALSE);
	MakeSapDataCombineSoilPressure(pFEM);
	MakeSapDataCombineSubsid(pFEM, b3DPlate);

	BOOL bHistoryCombo = (m_bHistory[0] && b3DPlate == FALSE) ? TRUE : FALSE;
	//////////////////////////////////////////////////////////////////////////
	// 사용하중 조합 - 0
	// 계수하중 조합 - 1
	// 최대편심 조합 - 2;
	long nSizeComboEnv[4] = {0, 0, 0, 0};
	if(b3DPlate == FALSE)
	{
		pBri->m_lArrComboUse.RemoveAll();
		pBri->m_lArrComboUlt.RemoveAll();
		pBri->m_lArrComboEcc.RemoveAll();
	}

	long nCountComboCase = (!bEarthQuake && pData->IsRailDesign2017() && pData->m_nTypeFootingSafetyApplyCoefficientImpact == 1)? 4 : 3;
	for(long nComboCase=0; nComboCase<nCountComboCase; nComboCase++)
	{
		long nSizeCombo  = nSizeComboUse;
		CMatrix MatCombo = pBri->m_MatComboUse;
		if(nComboCase==1) { nSizeCombo = nSizeComboUlt;  MatCombo = pBri->m_MatComboUlt;}
		if(nComboCase==2) { nSizeCombo = nSizeComboEcc;  MatCombo = pBri->m_MatComboEcc;}

		long nIdxCombo = 0;
		long nCountSubsid = MAX(1, m_nCountSubsidence);
		for(long nComboSubsid = 0; nComboSubsid < nCountSubsid; nComboSubsid++)
		{
			if(bEnvSubsid && nComboSubsid == 1) break;

			for(long nComboToapOne = 0; nComboToapOne < 2; nComboToapOne++)
			{
				if(bEnvToapOne && nComboToapOne == 1) break;

				for(long nComboLR = 0; nComboLR < 2; ++nComboLR)
				{
					for(long i=0; i<nSizeCombo; i++)
					{
						DWORD dWCombo = pBri->GetLoadCombo(i,nComboCase == 3 ? 0 : nComboCase);

						if(nComboSubsid > 0 && (dWCombo&LOAD_CASE_SUBSID)==FALSE) continue;
						if(nComboToapOne > 0 && (dWCombo&LOAD_CASE_TOAP_ONE)==FALSE) continue;
						if(nComboLR > 0 && (dWCombo&LOAD_CASE_LR)==FALSE) continue;

						sHead = _T("SV");
						if(nComboCase==1) sHead = _T("UL");
						if(nComboCase==2)	sHead = _T("UE");
						str.Format("%s%02d",sHead, nIdxCombo + 1);
						if(nComboCase == 3)
							str += _T("-S");

						// SHELL 조합은 선택된 것만...
						if(bEarthQuake && pBri->m_bApplyCombo3D[nComboCase][i]==FALSE) continue;

						if(bHistoryCombo)
						{
							strLoad.Format(_T("H%s%02d%s"), sHead, i+1, nComboToapOne > 0 ? _T("R") : _T(""));
							pFEM->AddCombo(str,strLoad, 0, 3, 1, FALSE);
						}

						if(dWCombo&LOAD_CASE_DEAD && !bHistoryCombo)
						{
							nLoadCase = BRIDGE_LOAD_DEAD;
							strLoad = pData->GetStringLoadCase(nLoadCase);
							pFEM->AddCombo(str,strLoad,0,0,MatCombo(i,nLoadCase),FALSE);
							if(bLSD == FALSE && bBOX && pBri->m_dHeightInteriorWaterPress > 0)
							{
								strLoad = _T("IW");
								pFEM->AddCombo(str,strLoad,0,0,MatCombo(i,nLoadCase),FALSE);
							}
						}
						if(dWCombo&LOAD_CASE_SOIL && Compare(pLoad->m_dSoilPressureVert,0.0,">"))
						{
							nLoadCase = BRIDGE_LOAD_SOIL;
							strLoad = pData->GetStringLoadCase(nLoadCase);
							pFEM->AddCombo(str,strLoad,0,0,MatCombo(i,nLoadCase),FALSE);								
						}
						if(dWCombo&LOAD_CASE_LIVE_FULL)
						{
							nLoadCase = BRIDGE_LOAD_LIVE_FULL;
							strLoad = pData->GetStringLoadCase(nLoadCase);
							if(nComboCase == 3)
								strLoad += _T("-S");

							if(pBri->m_bUnderGroundRahmen && !pBri->m_bCalcOpen)
								pFEM->AddCombo(str,strLoad,0,0,MatCombo(i,nLoadCase),FALSE);	
							else if(pLoad->m_bExistLane)
								pFEM->AddCombo(str,strLoad,0,1,MatCombo(i,nLoadCase),FALSE);	
							if(pLoad->m_dLiveLoadFootway>0 &&!bHistoryCombo)
							{
								pFEM->AddCombo(str,"FW",0,0,MatCombo(i,nLoadCase),FALSE);				
							}
						}
						if(dWCombo&LOAD_CASE_TANK && (!pBri->m_bUnderGroundRahmen || pBri->m_bCalcOpen))
						{
							nLoadCase = BRIDGE_LOAD_TANK;
							strLoad = pData->GetStringLoadCase(nLoadCase);
							pFEM->AddCombo(str,strLoad,0,1,MatCombo(i,nLoadCase),FALSE);	
						}
						if(dWCombo&LOAD_CASE_TOAP_BOTH && !bHistoryCombo)
						{
							nLoadCase = BRIDGE_LOAD_TOAP_BOTH;
							strLoad = pData->GetStringLoadCase(nLoadCase);
							pFEM->AddCombo(str,strLoad,0,0,MatCombo(i,nLoadCase),FALSE);				
						}
						if(dWCombo&LOAD_CASE_TOAP_ONE)
						{
							nLoadCase = BRIDGE_LOAD_TOAP_ONE;
							if(bEnvToapOne)
							{
								strLoad = pData->GetStringLoadCase(nLoadCase);
								pFEM->AddCombo(str,strLoad,0,1,MatCombo(i,nLoadCase),FALSE);				
							}
							else if(bHistoryCombo == FALSE)
							{
								strLoad.Format(_T("%s%s"), pData->GetStringLoadCase(nLoadCase), nComboToapOne == 0 ? _T("L") : _T("R"));
								pFEM->AddCombo(str,strLoad,0,0,MatCombo(i,nLoadCase),FALSE);				
							}
						}
						if(dWCombo&LOAD_CASE_TEMP1 && !bHistoryCombo)
						{
							nLoadCase = BRIDGE_LOAD_TEMP1;
							strLoad = pData->GetStringLoadCase(nLoadCase);
							pFEM->AddCombo(str,strLoad,0,0,MatCombo(i,nLoadCase),FALSE);				
						}
						if(dWCombo&LOAD_CASE_TEMP2 && !bHistoryCombo)
						{
							nLoadCase = BRIDGE_LOAD_TEMP2;
							strLoad = pData->GetStringLoadCase(nLoadCase);
							pFEM->AddCombo(str,strLoad,0,0,MatCombo(i,nLoadCase),FALSE);				
						}
						if(dWCombo&LOAD_CASE_SHRINK && !bHistoryCombo)
						{
							nLoadCase = BRIDGE_LOAD_SHRINK;
							strLoad = pData->GetStringLoadCase(nLoadCase);
							pFEM->AddCombo(str,strLoad,0,0,MatCombo(i,nLoadCase),FALSE);				
						}
						if(dWCombo&LOAD_CASE_SUBSID)
						{
							nLoadCase = BRIDGE_LOAD_SUBSID;
							if(bEnvSubsid)
							{
								strLoad = pData->GetStringLoadCase(nLoadCase);
								if((pBri->m_nSelect2DFrameTool!=SOLVE_MIDAS) && b3DPlate==FALSE)
									pFEM->AddCombo(str,strLoad,0,1,MatCombo(i,nLoadCase),FALSE);
								// 지점 침하는 마이다스와 호환을 위하여 EnvType 을 2로 수정
								else
									pFEM->AddCombo(str,strLoad,0,2,MatCombo(i,nLoadCase),FALSE);
							}
							else
							{
								if(nCountSubsid > 1)
									strLoad.Format(_T("%s%d"), pData->GetStringLoadCase(nLoadCase), nComboSubsid + 1);
								else
									strLoad = pData->GetStringLoadCase(nLoadCase);
								pFEM->AddCombo(str,strLoad,0,0,MatCombo(i,nLoadCase),FALSE);				
							}
						}
						if(dWCombo&LOAD_CASE_LR && Compare(pLoad->m_dRail_LoadLR,0.0,">") && !bHistoryCombo)
						{
							nLoadCase = BRIDGE_LOAD_LR;
							strLoad = pData->GetStringLoadCase(nLoadCase);
							if(nComboLR == 1)
							{
								strLoad = strLoad + _T("2");
							}
							pFEM->AddCombo(str,strLoad,0,0,MatCombo(i,nLoadCase),FALSE);				
						}
						if(dWCombo&LOAD_CASE_SB && Compare(pLoad->m_dRail_LoadSB,0.0,">") && !bHistoryCombo)
						{
							nLoadCase = BRIDGE_LOAD_SB;
							strLoad = pData->GetStringLoadCase(nLoadCase);
							pFEM->AddCombo(str,strLoad,0,0,MatCombo(i,nLoadCase),FALSE);				
						}
						if(pBri->IsBoxType() && pBri->m_bApplyGroundWaterLevel && !bHistoryCombo)
						{
							if(dWCombo&LOAD_CASE_WATERHORI && pBri->m_bApplyLoad[BRIDGE_LOAD_WATERHORI])
							{
								nLoadCase = BRIDGE_LOAD_WATERHORI;
								strLoad = pData->GetStringLoadCase(nLoadCase);
								pFEM->AddCombo(str,strLoad,0,0,MatCombo(i,nLoadCase),FALSE);				
							}
							if(dWCombo&LOAD_CASE_WATERVERT && pBri->m_bApplyLoad[BRIDGE_LOAD_WATERVERT])
							{
								nLoadCase = BRIDGE_LOAD_WATERVERT;
								strLoad = pData->GetStringLoadCase(nLoadCase);
								pFEM->AddCombo(str,strLoad,0,0,MatCombo(i,nLoadCase),FALSE);				
							}
						}
						if(pBri->m_nTypeLower == BOXFOOTING_TYPE_ANCHOR && pBri->m_bApplyAnchorForce && !bHistoryCombo)
						{
							strLoad = _T("AN");
							pFEM->AddCombo(str,strLoad,0,0,pBri->m_dFactorAnchor[nComboCase][i],FALSE);				
						}

						if(b3DPlate == FALSE)
						{
							if(nComboCase==0)
							{
								pBri->m_lArrComboUse.Add(i); // 옵션에 따라 입력하중조합과 해석데이타에 적용된 조합수가 다를 수 있음. <- 편토압,지점침하
							}
							if(nComboCase==1) 
							{
								pBri->m_lArrComboUlt.Add(i);
							}
							if(nComboCase==2) 
							{
								pBri->m_lArrComboEcc.Add(i);
							}
						}
						nIdxCombo++;
						nSizeComboEnv[nComboCase]++;
					}
				}
			}
		}
	}

	for(long i=0; i<nSizeComboEnv[0]; i++)
	{		
		// SHELL 조합은 선택된 것만...
		if(bEarthQuake && pBri->m_bApplyCombo3D[COMBO_SERVICE][i]==FALSE) continue;

		str.Format("SV%02d",i+1);
		pFEM->AddCombo("ENVUSE",str,1,1,1.0,FALSE);						
	}
//  (ARCBRIDGE-3103) 기초검토는 개별조합을 사용하니까 Env조합은 만들지 않는다.
// 	if(!bEarthQuake && pData->IsRailDesign2017() && pData->m_nTypeFootingSafetyApplyCoefficientImpact == 1)
// 	{
// 		for(long i=0; i<nSizeComboEnv[0]; i++)
// 		{		
// 			str.Format("SV%02d-S",i+1);
// 			pFEM->AddCombo("ENVUSE-S",str,1,1,1.0,FALSE);						
// 		}
// 	}

	for(long i=0; i<nSizeComboEnv[1]; i++)
	{
		// SHELL 조합은 선택된 것만...
		if(bEarthQuake && pBri->m_bApplyCombo3D[COMBO_ULTIMATE][i]==FALSE) continue;

		str.Format("UL%02d",i+1);
		pFEM->AddCombo("ENVULT",str,1,1,1.0,FALSE);			
	}	
	for(long i=0; i<nSizeComboEnv[2]; i++)
	{
		// SHELL 조합은 선택된 것만...
		if(bEarthQuake && pBri->m_bApplyCombo3D[COMBO_ECCENTRIC][i]==FALSE) continue;

		str.Format("UE%02d",i+1);		
		pFEM->AddCombo("ENVECC",str,1,1,1.0,FALSE);			
	}
}

long CARcBridgeDesignFrame::GetNumberElementSlabStt(CFEMManage *pFEM, BOOL bUpperSlab)
{
	long nSizeEle = pFEM->GetElementSize();
	long nFlag = bUpperSlab ? ELEMENT_UPPERSLAB : ELEMENT_LOWERSLAB;
	for(long i=0; i<nSizeEle; i++)
	{
		CElement *pEle = pFEM->GetElement((USHORT)i);
		if(pEle->m_nFlag==nFlag) return pEle->m_nIndex;
	}

	return -1;
}

long CARcBridgeDesignFrame::GetNumberElementSlabEnd(CFEMManage *pFEM, BOOL bUpperSlab)
{
	// 슬래브부재가 끝나고 벽체부제가 생기는 가정.....
	long nSizeEle = pFEM->GetElementSize();
	if(m_pStd->m_pBri->m_nTypeBridge==BRIDGE_TYPE_SLAB) return nSizeEle-1;

	long nFlag = bUpperSlab ? ELEMENT_UPPERSLAB : ELEMENT_LOWERSLAB;
	for(long i=nSizeEle-1; i>=0; i--)
	{
		CElement *pEle = pFEM->GetElement((USHORT)i);
		if(pEle->m_nFlag==nFlag) 
			return pEle->m_nIndex;
	}

	return -1;
}

long CARcBridgeDesignFrame::GetNumberJointSlabStt(CFEMManage *pFEM, BOOL bUpperSlab)
{
	long nFlag = bUpperSlab ? JOINT_UPPERSLAB : JOINT_LOWERSLAB;
	long nSizeJoint = pFEM->GetJointSize();
	long i=0; for(i=0; i<nSizeJoint; i++)
	{
		CJoint *pJnt = pFEM->GetJoint((USHORT)i);
		if(pJnt->m_LongAttrib[LONG_0_JOINT_TYPE]==nFlag) return pJnt->m_nIdx;
	}

	return -1;
}

long CARcBridgeDesignFrame::GetNumberJointSlabEnd(CFEMManage *pFEM, BOOL bUpperSlab)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;

	// 슬래브부재가 끝나고 벽체부제가 생기는 가정.....
	long nFlag = bUpperSlab ? JOINT_UPPERSLAB : JOINT_LOWERSLAB;
	long nSizeJoint = pFEM->GetJointSize();
	if(pBri->IsRcSlab())
	{
		CJoint *pJnt = pFEM->GetJoint((USHORT)nSizeJoint-1);
		return pJnt->m_nIdx;
	}

	for(long ix = nSizeJoint-1; ix >= 0; ix--)
	{
		CJoint *pJnt = pFEM->GetJoint((USHORT)ix);
		if(pJnt->m_LongAttrib[LONG_0_JOINT_TYPE] == nFlag) 
		{
			return pJnt->m_nIdx;
		}
	}

	return -1;
}


// RC SLAB나 파이형라멘의 시종점부는 교량받침위치를 리턴..
long CARcBridgeDesignFrame::GetNumberJointJijumFix(CFEMManage *pFEM, long nJijum)
{
	BOOL bExitsWall = m_pStd->m_pBri->IsWall(nJijum);

	long nSizeJoint = pFEM->GetJointSize();
	long nJijumIdx = nJijum;
	if(m_pStd->m_pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
	{
		switch(nJijum)
		{
		case 0: nJijumIdx = 0; break;
		case 1: nJijumIdx = m_pStd->m_pBri->m_nQtyJigan+1; break;
		case 2: nJijumIdx = m_pStd->m_pBri->m_nQtyJigan+2; break;
		case 3: nJijumIdx = m_pStd->m_pBri->m_nQtyJigan; break;
		}
	}

	long i=0; for(i=0; i<nSizeJoint; i++)
	{
		CJoint *pJnt = pFEM->GetJoint((USHORT)i);
		if(pJnt->IsFix() && pJnt->m_LongAttrib[LONG_1_JOINT_JIJUMNO]==nJijumIdx)
		{
			return pJnt->m_nIdx;
		}
		if(bExitsWall==FALSE)
		{
			if(pJnt->m_LongAttrib[LONG_1_JOINT_JIJUMNO]==nJijumIdx && IsRestraintJoint(pJnt)) return pJnt->m_nIdx;
		}
	}

	return -1;
}

long CARcBridgeDesignFrame::GetNumberJointJijumSpring(CFEMManage *pFEM, long nJijum)
{
	long nSizeJoint = pFEM->GetJointSize();

	long nJijumIdx = nJijum;
	if(m_pStd->m_pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
	{
		switch(nJijum)
		{
		case 0: nJijumIdx = 0; break;
		case 1: nJijumIdx = m_pStd->m_pBri->m_nQtyJigan+1; break;
		case 2: nJijumIdx = m_pStd->m_pBri->m_nQtyJigan+2; break;
		case 3: nJijumIdx = m_pStd->m_pBri->m_nQtyJigan; break;
		}
	}
	long i=0; for(i=0; i<nSizeJoint; i++)
	{
		CJoint *pJnt = pFEM->GetJoint((USHORT)i);
		if(!pJnt->m_vSpringR.IsNull() && !pJnt->m_vSpring.IsNull() && pJnt->m_LongAttrib[LONG_1_JOINT_JIJUMNO]==nJijumIdx) return pJnt->m_nIdx;
	}

	return -1;
}

void CARcBridgeDesignFrame::MakeSapDataSpringEQ(CFEMManage *pFEM, BOOL bLeftWallToap)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd   *pData = pBri->m_pARcBridgeDataStd;
	CARcBridgeCalcEarthQuake *pLoadEQ = m_pStd->m_pLoadEQ;

	long nSizeJoint = pFEM->GetJointSize();
	long nJijum = 0;

	//////////////////////////////////////////////////////////////////////////
	// FOOTING
	double U1,U2,U3,R1,R2,R3;
	double mF = (pBri->m_nTypeBridge==BRIDGE_TYPE_TRAP2) ? 0.5 : 1.0; // ?????
	long i=0; for(i=0; i<nSizeJoint; i++)
	{
		CJoint *pJnt = pFEM->GetJoint((USHORT)i);
		if(pJnt->m_BoolAttrib[BOOL_0_JOINT_SPRING]==FALSE) continue;
		nJijum = pJnt->m_LongAttrib[LONG_1_JOINT_JIJUMNO];

		CRigidityFooting *pRigid = pLoadEQ->GetRigidityFooting(nJijum);
		if(pRigid==NULL && pBri->m_nTypeBridge==BRIDGE_TYPE_TRAP2)
		{
			if(nJijum==1) pRigid = pLoadEQ->GetRigidityFooting(nJijum-1);
			if(nJijum==pBri->m_nQtyJigan-1) pRigid = pLoadEQ->GetRigidityFooting(nJijum+1);
		}
		if(pRigid==NULL && pBri->m_nTypeBridge==BRIDGE_TYPE_ARCH)
		{
			if(nJijum==pBri->m_nQtyJigan+1) pRigid = pLoadEQ->GetRigidityFooting(0);
			if(nJijum==pBri->m_nQtyJigan+2) pRigid = pLoadEQ->GetRigidityFooting(pBri->m_nQtyJigan);
		}
		if(pRigid==NULL) continue;

		BOOL bCombine = pRigid->m_bCombineValue;
		U1 = bCombine ? pRigid->m_CombK[RIGIDITY_DISPX]   : pRigid->m_ValueK[RIGIDITY_DISPX];
		U2 = bCombine ? pRigid->m_CombK[RIGIDITY_DISPY]   : pRigid->m_ValueK[RIGIDITY_DISPY];
		U3 = bCombine ? pRigid->m_CombK[RIGIDITY_DISPZ]   : pRigid->m_ValueK[RIGIDITY_DISPZ];
		R1 = bCombine ? pRigid->m_CombK[RIGIDITY_ROTATEX] : pRigid->m_ValueK[RIGIDITY_ROTATEX];
		R2 = bCombine ? pRigid->m_CombK[RIGIDITY_ROTATEY] : pRigid->m_ValueK[RIGIDITY_ROTATEY];
		R3 = bCombine ? pRigid->m_CombK[RIGIDITY_ROTATEZ] : pRigid->m_ValueK[RIGIDITY_ROTATEZ];
		pJnt->m_vSpring.x  = GetValueUnitChange(U1,UNIT_CGS_TONF_M,pData->m_szTonf_M)*mF;
		pJnt->m_vSpring.y  = GetValueUnitChange(U2,UNIT_CGS_TONF_M,pData->m_szTonf_M)*mF;
		pJnt->m_vSpring.z  = GetValueUnitChange(U3,UNIT_CGS_TONF_M,pData->m_szTonf_M)*mF;
		pJnt->m_vSpringR.x = GetValueUnitChange(R1,UNIT_CGS_TONFM,pData->m_szTonfM)*mF;
		pJnt->m_vSpringR.y = GetValueUnitChange(R2,UNIT_CGS_TONFM,pData->m_szTonfM)*mF;
		pJnt->m_vSpringR.z = GetValueUnitChange(R3,UNIT_CGS_TONFM,pData->m_szTonfM)*mF;
	}
	if(pBri->IsFooting(bLeftWallToap)==FALSE) return;
	if(pBri->GetTypeRigidFootingEQ()==0) return; // 기초와 벽체의 강성을 조합... => 벽체스프링 없음

	//////////////////////////////////////////////////////////////////////////
	// 벽체
	nJijum = bLeftWallToap ? pBri->m_nQtyJigan : 0;

	CJoint *pJointSlab   = GetJointJijumOnSlab(pFEM,nJijum,TRUE);
	CJoint *pJointHunch  = GetJointWallHunchEnd(pFEM,nJijum);
	CJoint *pJointSpring = (pBri->GetTypeRigidFootingEQ()==2) ? GetJointJijumFix(pFEM,nJijum) : GetJointJijumSpring(pFEM,nJijum);

	if(!pJointSlab || !pJointHunch || !pJointSpring) return;

	// 모델링상단
	double Len1=0,Len2=0;
	double Ks = GetKsValueJointWall(pFEM,nJijum,pJointSlab->m_nIdx,Len1,Len2);
	pJointSlab->m_vSpring.x = GetValueUnitChange(Ks,UNIT_CGS_TONF_M,pData->m_szTonf_M);
	if(pBri->GetApplyRigidWallEQRotate())
	{
		double Kr = GetKrValueJointWall(pFEM,nJijum,pJointSlab->m_nIdx,Len1,Len2);
		pJointSlab->m_vSpringR.y = GetValueUnitChange(Kr,UNIT_CGS_TONFM,pData->m_szTonfM);
	}

	for(i=pJointHunch->m_nIdx; i<pJointSpring->m_nIdx; i++)
	{
		CJoint *pJnt = pFEM->GetJoint((USHORT)i);
		Ks = GetKsValueJointWall(pFEM,nJijum,i,Len1,Len2);
		pJnt->m_vSpring.x = GetValueUnitChange(Ks,UNIT_CGS_TONF_M,pData->m_szTonf_M);
		if(pBri->GetApplyRigidWallEQRotate())
		{
			double Kr = GetKrValueJointWall(pFEM,nJijum,i,Len1,Len2);
			pJnt->m_vSpringR.y = GetValueUnitChange(Kr,UNIT_CGS_TONFM,pData->m_szTonfM);
		}
	}
}

void CARcBridgeDesignFrame::MakeSapDataLoadEQ_BoxType(CFEMManage *pFEM, EGoalOfEarthQuake eGoalEq)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeCalcLoad *pLoad = m_pStd->m_pLoad;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	if(pBri->IsBoxType() == FALSE) return;

	long nLoadCaseIndex = 0;
	
	CString strLoad = _T("");

	long nSizeEle    = pFEM->GetElementSize();
	long nIdxSlabStt = GetNumberElementSlabStt(pFEM, TRUE);
	long nIdxSlabEnd = GetNumberElementSlabEnd(pFEM, TRUE);
	double dApplyWidth = m_pStd->GetSlabWidth(-1);

	CARcBridgeCalcEarthQuake *pLoadEQ = m_pStd->m_pLoadEQ;

	// 자중
	if(pBri->m_bApplyLoad[BRIDGE_LOAD_DEAD])
	{
		double dLoadValue = GetValueUnitChange(pLoadEQ->m_DeadLoadDistribution+pLoadEQ->m_DeadLoadSoil,UNIT_CGS_TONF,pData->m_szTonf);
		strLoad    = pData->GetStringLoadCase(EARTHQUAKE_LOAD_DEAD,TRUE);
		pFEM->MakeElementLoadForce(nLoadCaseIndex+1);
		pFEM->m_szLCName.SetAt(nLoadCaseIndex, strLoad);
		pFEM->m_szLCComment.SetAt(nLoadCaseIndex, "구체자중 및 사하중");

		double dSelfWeight = 1.0;
		pFEM->m_dSelfWeight.SetAt(nLoadCaseIndex,dSelfWeight);	
		for(long nIdx=nIdxSlabStt; nIdx<=nIdxSlabEnd; nIdx++)
		{
			CElement *pE = pFEM->GetElement((USHORT)nIdx);
			CLoadForce *pELoad = pE->GetLoadForce(nLoadCaseIndex);
			pELoad->m_szLoadTitle = strLoad;
			pELoad->SetLoadForce(0, 1.0, CVector(0,0,-dLoadValue), CVector(0,0,-dLoadValue));
		}		
		++nLoadCaseIndex;
	}

	// 구체관성력
	if(pBri->m_bApplyLoad[BRIDGE_LOAD_EQ])
	{
		MakeSapDataEQLoadInertialForce(pFEM, eGoalEq, nLoadCaseIndex);
	}
	// (40669) 양토압과 편도압이 모두 없을때만 안들어가도록
	if(pBri->m_bApplyLoad[BRIDGE_LOAD_TOAP_BOTH] || pBri->m_bApplyLoad[BRIDGE_LOAD_TOAP_ONE])
	{
		for(long nLR = iLEFT; nLR <= iRIGHT; nLR++)
		{
			strLoad.Format(_T("%s%s"), pData->GetStringLoadCase(EARTHQUAKE_LOAD_TOAP, TRUE), nLR==iLEFT ? _T("L") : _T("R"));

			pFEM->MakeElementLoadForce(nLoadCaseIndex+1);
			pFEM->m_szLCName.SetAt(nLoadCaseIndex, strLoad);
			pFEM->m_szLCComment.SetAt(nLoadCaseIndex, _T("수평토압"));

			double dToapUp = 0;
			double dToapDn = 0;
			for(long iSE = iSTT; iSE <= iEND; iSE++)
			{
				double sw = (iSE == iSTT) ? 1.0 : -1.0;
				double dHStt = (iSE == iSTT) ? pFEM->GetJoint(0)->m_vPointOrigin.z : pFEM->GetJoint(0)->m_vPointOrigin.z + m_pStd->GetElDiffModelingEnd();

				long i=0; for(i=0; i<nSizeEle; i++)
				{
					CElement *pE = pFEM->GetElement((USHORT)i);	
					if(iSE == iSTT && (pE->m_nFlag != ELEMENT_STTWALL || nLR != iLEFT)) continue;
					if(iSE == iEND && (pE->m_nFlag != ELEMENT_ENDWALL || nLR != iRIGHT)) continue;

					CLoadForce *pELoad = pE->GetLoadForce(nLoadCaseIndex);
					CJoint *pJntI = pFEM->GetJoint(pE->m_nJStt);
					CJoint *pJntJ = pFEM->GetJoint(pE->m_nJEnd);

					dToapUp = pLoad->GetLoadSoilPressure(dHStt - pJntI->m_vPointOrigin.z, iSE == iSTT, FALSE, TRUE, eGoalEq) * dApplyWidth;
					dToapDn = pLoad->GetLoadSoilPressure(dHStt - pJntJ->m_vPointOrigin.z, iSE == iSTT, FALSE, TRUE, eGoalEq) * dApplyWidth;
					dToapUp = GetValueUnitChange(dToapUp,UNIT_CGS_TONF,pData->m_szTonf) * sw;
					dToapDn = GetValueUnitChange(dToapDn,UNIT_CGS_TONF,pData->m_szTonf) * sw;

					pELoad->m_szLoadTitle = strLoad;
					pELoad->SetLoadForce(0, 1.0, CVector(dToapUp,0,0), CVector(dToapDn,0,0));		
				}
			}

			++nLoadCaseIndex;	
		}
	}

	if(pBri->m_bApplyLiveLoadEQ)
	{
		MakeSapDataLoadLiveFootWay(pFEM, nLoadCaseIndex, TRUE);
	}

	// 수평수압
	// 연직수압
	MakeSapDataLoadWater(pFEM, nLoadCaseIndex, TRUE);
	// 앵커
	MakeSapDataLoadAnchor(pFEM, nLoadCaseIndex);

	if(pBri->m_bApplyLiveLoadEQ && (pBri->m_bUnderGroundRahmen == FALSE || pBri->m_bCalcOpen))
	{
		MakeSapDataLoadLiveLoadAll(pFEM, nLoadCaseIndex, TRUE);
	}
}


void CARcBridgeDesignFrame::MakeSapDataLoadEQ(CFEMManage *pFEM, BOOL bLeftWallToap)
{
	CRcBridgeRebar		*pBri  = m_pStd->m_pBri;
	CARcBridgeDataStd   *pData = pBri->m_pARcBridgeDataStd;

	if(pBri->IsBoxType()) return;

	long nLoadCaseIndex = 0;

//	double dSelfWeight = 1.0;
	double dLoadValue  = 1.0;

	long nIdxSlabStt = GetNumberElementSlabStt(pFEM);
	long nIdxSlabEnd = GetNumberElementSlabEnd(pFEM);
	CString strLoad(_T(""));
	
	CARcBridgeCalcEarthQuake *pLoadEQ = m_pStd->m_pLoadEQ;

	if(pBri->m_bApplyLoad[BRIDGE_LOAD_DEAD])
	{
		dLoadValue = GetValueUnitChange(pLoadEQ->m_DeadLoadDistribution+pLoadEQ->m_DeadLoadSoil,UNIT_CGS_TONF,pData->m_szTonf);
		strLoad    = pData->GetStringLoadCase(EARTHQUAKE_LOAD_DEAD,TRUE);
		pFEM->MakeElementLoadForce(nLoadCaseIndex+1);
		pFEM->m_szLCName.SetAt(nLoadCaseIndex, strLoad);
		pFEM->m_szLCComment.SetAt(nLoadCaseIndex, "구체자중 및 사하중");

		double dSelfWeight = 1.0;
		pFEM->m_dSelfWeight.SetAt(nLoadCaseIndex,dSelfWeight);	
		for(long nIdx=nIdxSlabStt; nIdx<=nIdxSlabEnd; nIdx++)
		{
			CElement *pE = pFEM->GetElement((USHORT)nIdx);
			CLoadForce *pELoad = pE->GetLoadForce(nLoadCaseIndex);
			pELoad->m_szLoadTitle = strLoad;
			pELoad->SetLoadForce(0, 1.0, CVector(0,0,-dLoadValue), CVector(0,0,-dLoadValue));
		}		
		++nLoadCaseIndex;
	}
	if(pBri->m_bApplyLoad[BRIDGE_LOAD_SOIL])
	{
		// 연직토압은 고정하중에 같이..
	}

	// 활하중 (지중라멘교 : 상재하중)
	if(pBri->m_bApplyLiveLoadEQ)
	{
		if(pBri->m_bUnderGroundRahmen && !pBri->m_bCalcOpen)
		{
			MakeSapDataLoadLiveAlways(pFEM, nLoadCaseIndex, TRUE);
		}
		else
		{
			MakeSapDataLoadLiveLoadAll(pFEM, nLoadCaseIndex, TRUE);
		}
		MakeSapDataLoadLiveFootWay(pFEM, nLoadCaseIndex, TRUE);
	}
	//
	if(pBri->m_bApplyLoad[BRIDGE_LOAD_EQ])
	{
		dLoadValue = GetValueUnitChange(pLoadEQ->m_Pe[CALC_DIR_X],UNIT_CGS_TONF_M,pData->m_szTonf_M);
		strLoad    = pData->GetStringLoadCase(EARTHQUAKE_LOAD_TRAP_X,TRUE);
		pFEM->MakeElementLoadForce(nLoadCaseIndex+1);
		pFEM->m_szLCName.SetAt(nLoadCaseIndex, strLoad);
		pFEM->m_szLCComment.SetAt(nLoadCaseIndex, "종방향 지진하중");
		for(long nIdx=nIdxSlabStt; nIdx<=nIdxSlabEnd; nIdx++)
		{
			CElement *pE = pFEM->GetElement((USHORT)nIdx);
			CLoadForce *pELoad = pE->GetLoadForce(nLoadCaseIndex);
			pELoad->m_szLoadTitle = strLoad;
			if(bLeftWallToap)
				pELoad->SetLoadForce(0, 1.0, CVector(dLoadValue,0,0), CVector(dLoadValue,0,0));
			else
				pELoad->SetLoadForce(0, 1.0, CVector(-dLoadValue,0,0), CVector(-dLoadValue,0,0));
		}		
		++nLoadCaseIndex;
	}
	//
	if((pBri->m_bApplyLoad[BRIDGE_LOAD_TOAP_BOTH] || pBri->m_bApplyLoad[BRIDGE_LOAD_TOAP_ONE]) && pBri->IsOutsideWall())
	{
		MakeSapDataEQLoadHoriSoilPress(pFEM, bLeftWallToap, nLoadCaseIndex);
	}
}

void CARcBridgeDesignFrame::MakeSapDataCombineEQ(CFEMManage *pFEM, BOOL bUnitLoad)
{
	CRcBridgeRebar		*pBri  = m_pStd->m_pBri;
	CARcBridgeCalcLoad	*pLoad = m_pStd->m_pLoad;
	CARcBridgeDataStd   *pData = pBri->m_pARcBridgeDataStd;

	CString strCombo = _T(""),strLoad = _T(""); 

	if(bUnitLoad)
	{
		pFEM->AddCombo("UNIT","LOAD-X",0,0,1.0,FALSE);
	}
	else
	{
		if(pBri->m_bApplyLiveLoadEQ && (pBri->m_bUnderGroundRahmen == FALSE || pBri->m_bCalcOpen))
		{
			MakeSapDataCombineLiveLoad(pFEM, TRUE);
		}

		BOOL bComboLR = pBri->IsBoxType() ? TRUE : FALSE;
		long nLoadCase   = 0;
		CMatrix MatCombo = pBri->m_MatComboEQ;
		long nSizeCombo  = pBri->m_MatComboEQ.GetRow();
		long nIndexCombo = 0;
		for(long nLR=iLEFT; nLR<=iRIGHT; nLR++)
		{
			if(bComboLR == FALSE && nLR == iRIGHT) break;

			for(long i=0; i<nSizeCombo; i++)
			{
				DWORD dWCombo = pBri->GetLoadCombo(i,LOAD_COMBINE_EQ);
				strCombo.Format("EQ%02d",nIndexCombo+1);
				if(m_bHistory[1])
				{
					strLoad.Format(_T("H%s"), strCombo);
					pFEM->AddCombo(strCombo,strLoad, 0, 3, 1, FALSE);
					nIndexCombo++;
					continue;
				}

				if(dWCombo&LOAD_CASE_DEAD)
				{
					nLoadCase = BRIDGE_LOAD_DEAD;
					strLoad = pData->GetStringLoadCase(EARTHQUAKE_LOAD_DEAD,TRUE);
					pFEM->AddCombo(strCombo,strLoad,0,0,MatCombo(i,nLoadCase),FALSE);
					if(pData->IsLsdDesign() == FALSE && pBri->IsBoxType() && pBri->m_dHeightInteriorWaterPress > 0)
					{
						strLoad = _T("IW");
						pFEM->AddCombo(strCombo,strLoad,0,0,MatCombo(i,nLoadCase),FALSE);
					}
				}
				if(dWCombo&LOAD_CASE_SOIL)
				{
				}
				if(dWCombo&LOAD_CASE_LIVE_FULL && pBri->m_bApplyLiveLoadEQ)
				{
					if(pBri->m_bUnderGroundRahmen && !pBri->m_bCalcOpen)
					{
						nLoadCase = BRIDGE_LOAD_LIVE_FULL;
						strLoad = pData->GetStringLoadCase(nLoadCase);
						pFEM->AddCombo(strCombo,strLoad,0,0,MatCombo(i,nLoadCase),FALSE);	
					}
					else
					{
						nLoadCase = BRIDGE_LOAD_LIVE_FULL;
						strLoad = pData->GetStringLoadCase(nLoadCase);
						pFEM->AddCombo(strCombo,strLoad,0,1,MatCombo(i,nLoadCase),FALSE);	
					}
					if(pLoad->m_dLiveLoadFootway>0)
					{
						pFEM->AddCombo(strCombo,"FW",0,0,MatCombo(i,nLoadCase),FALSE);				
					}
				}

				if(pBri->IsBoxType())
				{
					if(dWCombo&LOAD_CASE_EQ)
					{
						nLoadCase = BRIDGE_LOAD_EQ;
						if(bComboLR)
							strLoad.Format(_T("%s%s"), pData->GetStringLoadCase(EARTHQUAKE_LOAD_INERTIAL, TRUE), nLR==iLEFT ? _T("L") : _T("R"));
						else
							strLoad = pData->GetStringLoadCase(EARTHQUAKE_LOAD_INERTIAL, TRUE);
						pFEM->AddCombo(strCombo,strLoad,0,0,MatCombo(i,nLoadCase), FALSE);
					}
					if(dWCombo&LOAD_CASE_TOAP_BOTH)
					{
						nLoadCase = BRIDGE_LOAD_TOAP_BOTH;
						if(bComboLR)
							strLoad.Format(_T("%s%s"), pData->GetStringLoadCase(EARTHQUAKE_LOAD_TOAP, TRUE), nLR==iLEFT ? _T("L") : _T("R"));
						else
							strLoad = pData->GetStringLoadCase(EARTHQUAKE_LOAD_TOAP, TRUE);
						pFEM->AddCombo(strCombo,strLoad,0,0,MatCombo(i,nLoadCase),FALSE);
					}
					if(dWCombo&LOAD_CASE_WATERHORI)
					{
						nLoadCase = BRIDGE_LOAD_WATERHORI;
						strLoad = pData->GetStringLoadCase(BRIDGE_LOAD_WATERHORI, FALSE);
						pFEM->AddCombo(strCombo,strLoad,0,0,MatCombo(i,nLoadCase),FALSE);
					}
					if(dWCombo&LOAD_CASE_WATERVERT)
					{
						nLoadCase = BRIDGE_LOAD_WATERVERT;
						strLoad = pData->GetStringLoadCase(BRIDGE_LOAD_WATERVERT, FALSE);
						pFEM->AddCombo(strCombo,strLoad,0,0,MatCombo(i,nLoadCase),FALSE);
					}
					if(pBri->m_nTypeLower == BOXFOOTING_TYPE_ANCHOR && pBri->m_bApplyAnchorForce)
					{
						strLoad = _T("AN"); 
						pFEM->AddCombo(strCombo,strLoad,0,0,pBri->m_dFactorAnchor[COMBO_EARTHQUAKE][i],FALSE);				
					}
				}
				else
				{
					if(dWCombo&LOAD_CASE_EQ)
					{
						nLoadCase = BRIDGE_LOAD_EQ;
						strLoad = pData->GetStringLoadCase(EARTHQUAKE_LOAD_TRAP_X,TRUE);
						pFEM->AddCombo(strCombo,strLoad,0,0,MatCombo(i,nLoadCase),FALSE);
					}
					if((dWCombo&LOAD_CASE_TOAP_BOTH || dWCombo&LOAD_CASE_TOAP_ONE) && pBri->IsOutsideWall())
					{
						nLoadCase = BRIDGE_LOAD_TOAP_BOTH;
						strLoad = pData->GetStringLoadCase(EARTHQUAKE_LOAD_TOAP,TRUE);
						pFEM->AddCombo(strCombo,strLoad,0,0,MatCombo(i,nLoadCase),FALSE);
					}
				}
				nIndexCombo++;
			}
		}
		//
		strCombo = "ENV";
		nSizeCombo = bComboLR ? nSizeCombo * 2 : nSizeCombo;
		for(long i=0; i<nSizeCombo; i++)
		{
			strLoad.Format("EQ%02d",i+1);
			pFEM->AddCombo(strCombo,strLoad,1,1,1.00,FALSE);
		}
	}
}

double CARcBridgeDesignFrame::GetLengthBetweenJoint(CFEMManage *pFEM, long nJoint1, long nJoint2)
{
	CJoint *pJnt1 = pFEM->GetJoint((USHORT)nJoint1);
	CJoint *pJnt2 = pFEM->GetJoint((USHORT)nJoint2);

	return ~(pJnt1->m_vPoint - pJnt2->m_vPoint);
}

// 기초하단이 스프링이거나 고정인 경우
CElement* CARcBridgeDesignFrame::GetElementWallFix(CFEMManage *pFEM, long nJijum)
{
	POSITION pos = pFEM->m_ElementList.GetHeadPosition();
	while(pos)
	{
		CElement *pEle = static_cast<CElement *>(pFEM->m_ElementList.GetNext(pos));
		CJoint *pJntI = pFEM->GetJoint(pEle->m_nJStt);
		CJoint *pJntJ = pFEM->GetJoint(pEle->m_nJEnd);

		if(m_pStd->m_pBri->IsBoxType() == FALSE)
		{
			BOOL bFixI = FALSE;
			BOOL bFixJ = FALSE;
			if(pJntI->m_BoolAttrib[BOOL_0_JOINT_SPRING] || pJntI->m_BoolAttrib[BOOL_2_JOINT_JIJUM_FIX]) bFixI = TRUE;
			if(pJntJ->m_BoolAttrib[BOOL_0_JOINT_SPRING] || pJntJ->m_BoolAttrib[BOOL_2_JOINT_JIJUM_FIX]) bFixJ = TRUE;

			if(bFixI && pJntI->m_LongAttrib[LONG_1_JOINT_JIJUMNO]==nJijum) return pEle;
			if(bFixJ && pJntJ->m_LongAttrib[LONG_1_JOINT_JIJUMNO]==nJijum) return pEle;
		}
		else
		{
			if(pJntJ->m_BoolAttrib[BOOL_3_JOINT_COLUMN_END] && pJntJ->m_LongAttrib[LONG_1_JOINT_JIJUMNO]==nJijum) return pEle;
		}
	}

	return NULL;
}

CElement* CARcBridgeDesignFrame::GetElementOnJointIdx(CFEMManage *pFEM, long nJIdx, BOOL bStt)
{
	POSITION pos = pFEM->m_ElementList.GetHeadPosition();
	while(pos)
	{
		CElement *pEle = static_cast<CElement *>(pFEM->m_ElementList.GetNext(pos));	
		if( bStt && pEle->m_nJStt == nJIdx) return pEle;
		if(!bStt && pEle->m_nJEnd == nJIdx) return pEle;
		if(pEle->m_nFlag == ELEMENT_ARCHRIB && (pEle->m_nJStt == nJIdx || pEle->m_nJEnd == nJIdx))
			return pEle;
	}
	return NULL;
}

void CARcBridgeDesignFrame::MakeSapData(long nTypeDesign, BOOL bRemoveAllData)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;

	BOOL bEarthQuake = (nTypeDesign==0) ? FALSE : TRUE;
	BOOL bArch = (pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH);
	long nTypeOld	= pBri->m_nTypeJongdanForRebar;
	pBri->GetTvVertSection_Std(0, TRUE, FALSE, FALSE, bArch? 1 : 0);
	pBri->SyncBridge();
	pBri->TrimRibByFootingUpper();

	MakeXyCoordinates(bEarthQuake);
	MakeArrayThick();

	if(nTypeDesign==0)
		MakeSapDataNormal(bRemoveAllData);
	else
		MakeSapDataEQ(nTypeDesign, bRemoveAllData);

	pBri->m_nTypeJongdanForRebar = nTypeOld;
	pBri->m_nTypeJongdanForRebarOld	= nTypeOld;

	pBri->GetTvVertSection_Std(0, TRUE, FALSE, TRUE, bArch? 1 : 0);
	pBri->SyncBridge();
	pBri->m_bFirstCalledGetTvVertSection_Std	= FALSE;
	pBri->m_bSecondCalledGetTvVertSection_Std	= FALSE;
}

void CARcBridgeDesignFrame::MakeSapDataNormal(BOOL bRemoveAllData)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	if(pBri==NULL) return;

	BOOL bLsd = pBri->m_pARcBridgeDataStd->IsLsdDesign();
	CFEMManage *pFEM = &pBri->m_FemNormal;

	if(bRemoveAllData)
	{
		pFEM->RemoveAllData();
	}
	pFEM->m_eDecimal = bLsd? CFEMManage::eDec6 : CFEMManage::eDec3;

	m_pStd->CalculateCofficient();
	m_pStd->m_pLoad->CalculateAll();
	if(pBri->IsBoxType())
	{
		m_pStd->m_pModulusFoundation->CalculateAll();
		m_pStd->m_pFooting->CalculateAll();
	}

	if(bLsd)
	{
		pFEM->SYSTEM_VAR.m_szUnitType = "N";
		pFEM->SYSTEM_VAR.m_szLengthType = "MM";
	}
	else
	{
		pFEM->SYSTEM_VAR.m_szUnitType = pBri->m_pARcBridgeDataStd->IsSI() ? "KN" : "TON";
		if(pBri->m_nSelect2DFrameTool==SOLVE_MIDAS && !pBri->m_pARcBridgeDataStd->IsSI())
		{
			pFEM->SYSTEM_VAR.m_szUnitType = "TONF";
		}
	}

	MakeSapDataJoint(pFEM, FALSE);
	MakeSapDataMaterial(pFEM, FALSE);
	MakeSapDataFrameSection(pFEM,FALSE);
	MakeSapDataConstraints(pFEM);
	MakeSapDataElement(pFEM,FALSE);

	MakeSapDataNLProperty(pFEM, FALSE);
	MakeSapDataNLLink(pFEM, FALSE);

	m_bHistory[0] = FALSE;

	// History 만들때도 하중을 확인 하려면 하중면저. LSD일때만
	if(bLsd == FALSE)
	{
		MakeSapDataMode(pFEM, FALSE);
		MakeSapDataHistoryNormal(pFEM);
	}

	if(IsAvailableJijumFix(pFEM))
	{	
		if(bLsd)
		{
			MakeSapDataLoadNormal_Lsd(pFEM);

			MakeSapDataMode_Lsd(pFEM, FALSE);
			MakeSapDataHistoryNormal_Lsd(pFEM);
			
			MakeSapDataCombineNormal_Lsd(pFEM,FALSE,FALSE);
			MakeSapDataCombineEnv_Lsd(pFEM, FALSE, FALSE);
			MakeSapDataCombineEtaCase_Lsd(pFEM, FALSE, FALSE);
		}
		else
		{
			MakeSapDataLoadNormal(pFEM);
			MakeSapDataCombineNormal(pFEM,FALSE,FALSE);
		}
	}
	else
	{
		CString str = _T("");
		if(pBri->IsOutsideWall()) 
			str = _T("지점 입력 오류");
		else 
			str = _T("시종점에 교량받침 입력을 확인하세요");

		AfxMessageBox(str, MB_OK);
		return;
	}
}

// nTypeDesign - 0 평상시
// nTypeDesign - 1 지진시 : 단위하중	/ 붕괴 혹은 기능
// nTypeDesign - 2 지진시 : 단일모드	/ 기능
// nTypeDesign - 3 지진시 : 단일모드 (좌우측에 각각 토압을 작용시키는 경우)
void CARcBridgeDesignFrame::MakeSapDataEQ(long nTypeDesign, BOOL bRemoveAllData)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	if(pBri==NULL) return;
	if(nTypeDesign==0) return;
	if(nTypeDesign>=5) return;
	if(pBri->GetCountEarthQuakeModel(FALSE) < nTypeDesign) return;
	
	BOOL bLsd = pBri->m_pARcBridgeDataStd->IsLsdDesign();
	EGoalOfEarthQuake eGoalEq = pBri->GetEarthQuakeGoalByIndex(nTypeDesign-1);
	
	CFEMManage *pFEM = &pBri->m_FemEQ[nTypeDesign-1];
	
	if(bRemoveAllData)
	{
		pFEM->RemoveAllData();
	}
	pFEM->m_eDecimal = bLsd? CFEMManage::eDec6 : CFEMManage::eDec3;

	m_pStd->CalculateCofficient();
	if(pBri->IsBoxType())
	{
		m_pStd->m_pLoad->CalculateAll();
	}
	m_pStd->m_pLoadEQ->CalculateAll();
	if(pBri->IsBoxType())
	{
		m_pStd->m_pModulusFoundation->CalculateAll();
		m_pStd->m_pFooting->CalculateAll();
	}

	if(bLsd)
	{
		pFEM->SYSTEM_VAR.m_szUnitType = "N";
		pFEM->SYSTEM_VAR.m_szLengthType = "MM";
	}
	else
	{
		pFEM->SYSTEM_VAR.m_szUnitType = pBri->m_pARcBridgeDataStd->IsSI() ? "KN" : "TON";
		if(pBri->m_nSelect2DFrameTool==SOLVE_MIDAS && !pBri->m_pARcBridgeDataStd->IsSI())
		{
			pFEM->SYSTEM_VAR.m_szUnitType = "TONF";
		}
	}

	BOOL bUnitLoad  = (nTypeDesign == 1 && pBri->IsBoxType() == FALSE) ? TRUE : FALSE;
	BOOL bLeftWallToap = TRUE;
	if(pBri->m_nSelectModelingCountEQ==1) bLeftWallToap = FALSE;
	if(pBri->m_nSelectModelingCountEQ==2 && nTypeDesign==3) bLeftWallToap = FALSE;

	MakeSapDataJoint(pFEM, TRUE);
	//	MakeSapDataNLLinkEQ(pFEM);
	MakeSapDataSpringEQ(pFEM,bLeftWallToap);
	MakeSapDataMaterial(pFEM, TRUE);
	MakeSapDataFrameSection(pFEM,TRUE);
	MakeSapDataElement(pFEM,TRUE);	
	MakeSapDataConstraints(pFEM);

	MakeSapDataNLProperty(pFEM, TRUE);
	MakeSapDataNLLink(pFEM, TRUE);

	m_bHistory[1] = FALSE;
	
	if(bLsd)
	{
		// History 만들때도 하중을 확인 하려면 하중면저. LSD일때만
		if(pBri->IsBoxType())
			MakeSapDataLoadEQ_BoxType_Lsd(pFEM, eGoalEq, nTypeDesign);
		else
		{
			if(bUnitLoad)
				MakeSapDataUnitLoadEQ(pFEM);
			else
				MakeSapDataLoadEQ_Lsd(pFEM,bLeftWallToap);
		}

		MakeSapDataMode_Lsd(pFEM, TRUE);
		MakeSapDataHistoryEarthquake_Lsd(pFEM);

		MakeSapDataCombineEQ_Lsd(pFEM,bUnitLoad, bLeftWallToap, nTypeDesign);

		if(bUnitLoad == FALSE)
		{
			MakeSapDataCombineEnv_Lsd(pFEM, TRUE, FALSE);
			MakeSapDataCombineEtaCase_Lsd(pFEM, TRUE, FALSE);
		}
	}
	else
	{
		MakeSapDataMode(pFEM, TRUE);
		MakeSapDataHistoryEarthquake(pFEM);
		if(pBri->IsBoxType())
			MakeSapDataLoadEQ_BoxType(pFEM, eGoalEq);
		else
		{
			if(bUnitLoad)
				MakeSapDataUnitLoadEQ(pFEM);
			else
				MakeSapDataLoadEQ(pFEM,bLeftWallToap);
		}
		MakeSapDataCombineEQ(pFEM,bUnitLoad);
	}
}


void CARcBridgeDesignFrame::MakeArrayThick()
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	if(pBri==NULL) return;

	long nJijumConnect = pBri->GetJijumConnectRibAndSlab();
	BOOL bRcSlab  = (pBri->m_nTypeBridge==BRIDGE_TYPE_SLAB) ? TRUE : FALSE;
	BOOL bArch	  = (pBri->m_nTypeBridge==BRIDGE_TYPE_ARCH) ? TRUE : FALSE;
	BOOL bOutWall = pBri->IsOutsideWall();

	m_dArrThickSlab.RemoveAll();
	m_dArrThickWall.RemoveAll();
	m_dArrThickArchRib.RemoveAll();

	//////////////////////////////////////////////////////////////////////////
	// 슬래브 두께
	m_dArrThickSlab.Add(pBri->m_dTS);
	if(pBri->IsBoxType())
	{
		m_dArrThickSlab.Add(pBri->m_dTS_Lower);
	}

	if(pBri->m_bSelectHunchModeling)
	{
		if(bRcSlab || pBri->IsPiRahmen())
		{
			for(long nJijum=0; nJijum<=pBri->m_nQtyJigan; nJijum++)
			{
				double dThickHunch1 = pBri->GetThickHunch(nJijum,TRUE,FALSE);
				double dThickHunch2 = pBri->GetThickHunch(nJijum,FALSE,FALSE);
				if(dThickHunch1>0)
				{
					long nSizeThick = m_dArrThickSlab.GetSize();
					long i=0; for(i=0; i<nSizeThick; i++)
					{
						double dThcick1 = m_dArrThickSlab.GetAt(i);
						if(Compare(dThcick1,pBri->m_dTS+dThickHunch1,"=")) break;
					}
					if(i==nSizeThick)
						m_dArrThickSlab.Add(pBri->m_dTS+dThickHunch1);
				}
				if(dThickHunch2>0 && dThickHunch1!=dThickHunch2)
				{
					long nSizeThick = m_dArrThickSlab.GetSize();
					long i=0; for(i=0; i<nSizeThick; i++)
					{
						double dThcick1 = m_dArrThickSlab.GetAt(i);
						if(Compare(dThcick1,pBri->m_dTS+dThickHunch2,"=")) break;
					}
					if(i==nSizeThick)
						m_dArrThickSlab.Add(pBri->m_dTS+dThickHunch2);
				}
			}
		}
		else
		{
			for(long nUD = iUPPER; nUD <= iLOWER; nUD++)
			{
				if(nUD == iLOWER && pBri->IsBoxType() == FALSE) break;

				for(long nJijum=0; nJijum<=pBri->m_nQtyJigan; nJijum++)
				{
					double ThickSlab = m_pStd->GetThickSlabByDist(nJijum, 0, nUD == iUPPER);
					long nSizeThick = m_dArrThickSlab.GetSize();
					long ix = 0;
					for(ix=0; ix<nSizeThick; ix++)
					{
						double dThick = m_dArrThickSlab.GetAt(ix);
						if(Compare(dThick, ThickSlab,"="))
						{
							break;
						}
					}
					if(ix==nSizeThick) 
					{
						m_dArrThickSlab.Add(ThickSlab);
					}
				}
			}
		}


		// 강역구간 두께 - 무한강성 적용 일때만
		if(pBri->m_nSelectRigidModeling == 1)
		{
			for(long nJijum=0; nJijum<=pBri->m_nQtyJigan; nJijum++)
			{
				if(bRcSlab) break;
				if(pBri->IsOutsideWall()==FALSE && (nJijum==0 || nJijum==pBri->m_nQtyJigan))
				{
					continue;
				}
				for(long nUD = iUPPER; nUD <= iLOWER; nUD++)
				{
					if(nUD == iLOWER && pBri->IsBoxType() == FALSE) break;

					double DistRigidStt = pBri->GetDistRigidEndSlab(nJijum, TRUE , nUD == iUPPER);
					double DistRigidEnd = pBri->GetDistRigidEndSlab(nJijum, FALSE, nUD == iUPPER);
					if(!Compare(DistRigidStt,0.0,"="))
					{
						double ThickSlab = m_pStd->GetThickSlabByDist(nJijum, -DistRigidStt, nUD == iUPPER);
						long nSizeThick = m_dArrThickSlab.GetSize();
						long i=0; for(i=0; i<nSizeThick; i++)
						{
							double dThick = m_dArrThickSlab.GetAt(i);
							if(Compare(dThick,ThickSlab,"="))
								break;
						}
						if(i==nSizeThick) 
						{
							m_dArrThickSlab.Add(ThickSlab);
						}
					}
					if(!Compare(DistRigidEnd,0.0,"="))
					{
						double ThickSlab = m_pStd->GetThickSlabByDist(nJijum, DistRigidEnd, nUD == iUPPER);
						long nSizeThick = m_dArrThickSlab.GetSize();
						long i=0; for(i=0; i<nSizeThick; i++)
						{
							double dThick = m_dArrThickSlab.GetAt(i);
							if(Compare(dThick,ThickSlab,"="))
								break;
						}
						if(i==nSizeThick) 
						{
							m_dArrThickSlab.Add(ThickSlab);
						}
					}
				}
			}
		}
	}

	// 벽체두께
	for(long nJijum=0; nJijum<=pBri->m_nQtyJigan; nJijum++)
	{
		if(bRcSlab) break;
		if(!bOutWall && (nJijum==0 || nJijum==pBri->m_nQtyJigan)) continue;
		if(nJijum == nJijumConnect) continue;

		double dThickWall	  = pBri->GetThickWall(nJijum);
		// 벽체두께
		long nSizeThick = m_dArrThickWall.GetSize();
		long i=0; for(i=0; i<nSizeThick; i++)
		{
			double dThick = m_dArrThickWall.GetAt(i);
			if(Compare(dThick,dThickWall,"="))
				break;
		}
		if(i==nSizeThick) 
		{
			m_dArrThickWall.Add(dThickWall);
		}
		if(pBri->m_bSelectHunchModeling == FALSE)
		{
			continue;
		}
		for(long iUD = iUPPER; iUD <= iLOWER; iUD++)
		{
			if(iUD == iLOWER && pBri->IsBoxType() == FALSE) break;
			double dWidthHunchStt = pBri->GetWidthHunch(nJijum, TRUE, iUD == iUPPER);
			double dWidthHunchEnd = pBri->GetWidthHunch(nJijum, FALSE, iUD == iUPPER);
			double dThickHunchStt = pBri->GetThickHunch(nJijum, TRUE, FALSE, iUD == iUPPER);
			double dThickHunchEnd = pBri->GetThickHunch(nJijum, FALSE, FALSE, iUD == iUPPER);

			if(dWidthHunchStt > dThickHunchStt/3) dWidthHunchStt = dThickHunchStt/3;
			if(dWidthHunchEnd > dThickHunchEnd/3) dWidthHunchEnd = dThickHunchEnd/3;

			// 벽체두께 + 헌치
			nSizeThick = m_dArrThickWall.GetSize();
			for(i=0; i<nSizeThick; i++)
			{
				double dThick = m_dArrThickWall.GetAt(i);
				if(Compare(dThick, dThickWall + dWidthHunchStt + dWidthHunchEnd, "="))
					break;
			}
			if(i==nSizeThick) m_dArrThickWall.Add(dThickWall+dWidthHunchStt+dWidthHunchEnd);

			// 강역위치 - 무한강성 적용 일때만
			if(pBri->m_nSelectRigidModeling == 1)
			{
				double HeightRigid = pBri->GetDistRigidEndWall(nJijum, iUD == iUPPER);
				if(!Compare(HeightRigid,0.0,"="))
				{
					double ThickRigid  = m_pStd->GetThickWallByDist(nJijum, HeightRigid, iUD == iLOWER);
					nSizeThick = m_dArrThickWall.GetSize();
					for(i=0; i<nSizeThick; i++)
					{
						double dThick = m_dArrThickWall.GetAt(i);
						if(Compare(dThick,ThickRigid,"="))
							break;
					}
					if(i==nSizeThick) m_dArrThickWall.Add(ThickRigid);
				}
			}
		}

		// 경사교각2 형식일때 하부 헌치고려 두께
		if((pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP2 || pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH) && (nJijum==0 || nJijum==pBri->m_nQtyJigan))
		{
			double dHunchHeight = (nJijum==0)? pBri->m_dWSHH : pBri->m_dWEHH;
//			double dAddTick = 0;
			double CountDivi = (long)RoundUp(dHunchHeight/1000.0,0);
			double dLenUnit  = Round(dHunchHeight/CountDivi,0);
			long i=0; for(i=0; i<(long)CountDivi; i++)
			{
				double dAddTick = m_pStd->GetThickWallByDist(nJijum,dLenUnit*i, TRUE);

				nSizeThick = m_dArrThickWall.GetSize();
				long j=0; for(j=0; j<nSizeThick; j++)
				{
					double dThick = m_dArrThickWall.GetAt(j);
					if(Compare(dThick,dAddTick,"="))
						break;
				}
				if(j==nSizeThick) m_dArrThickWall.Add(dAddTick);
			}
		}

		// 지하수위 위치의 벽체두께
		if(pBri->IsBoxType() && pBri->m_bApplyGroundWaterLevel && (nJijum == 0 || nJijum == pBri->m_nQtyJigan))
		{
			// double dSta = pBri->GetStationByTarget(nJijum == 0 ? BRIDGE_OFF_STT_SLAB : BRIDGE_OFF_END_SLAB, 0, FALSE);
			double dSta = pBri->GetStationOnJijum(nJijum,0);
			if(nJijum==0)
			{
				CDPoint vAng = pBri->GetAngleJijum(0);
				dSta -= (pBri->m_dWS/vAng.y)/2.0;
			}
			if(nJijum==pBri->m_nQtyJigan) 
			{
				CDPoint vAng = pBri->GetAngleJijum(pBri->m_nQtyJigan);
				dSta += (pBri->m_dWE/vAng.y)/2.0;
			}

			double dEL_Top = pBri->m_nSelectModelingHeight == 0 ? pBri->GetLineBase()->GetElevationPlanEl(dSta) : (pBri->GetElMaxSlabUpper(dSta, CDPoint(0, 1)) + pBri->GetThickPave(FALSE));
			double dElevation = dEL_Top - (pBri->GetThickPave() + pBri->m_dTS / 2.);
			double dWaterTick = m_pStd->GetThickWallByDist(nJijum,dElevation-pBri->m_dGroundWaterLevel);
			m_dArrThickWall.Add(dWaterTick);
		}
	}
	//////////////////////////////////////////////////////////////////////////
	long nSizeThick = m_dArrThickWall.GetSize();
	long i=0; for(i=0; i<nSizeThick; i++)
	{
		double dThick = toM(m_dArrThickWall.GetAt(i));
		m_dArrThickWall.SetAt(i,dThick);
	}
	//
	nSizeThick = m_dArrThickSpring.GetSize();
	for(long ix = 0; ix < nSizeThick; ix++)
	{
		m_dArrThickSlab.Add(m_dArrThickSpring.GetAt(ix));
	}
	nSizeThick = m_dArrThickSlab.GetSize();
	for(i=0; i<nSizeThick; i++)
	{
		double dThick = toM(m_dArrThickSlab.GetAt(i));
		m_dArrThickSlab.SetAt(i,dThick);
	}

	m_dArrThickSlab.RemoveSameValue();
	m_dArrThickWall.RemoveSameValue();
	m_dArrThickSlab.Sort();
	m_dArrThickWall.Sort();

	//////////////////////////////////////////////////////////////////////////
	// 기둥에 대한 설정... : 각 지점의 기둥에 대하여 설정 (동일한 것도 모두ㅜ,,,,)
	m_dArrColumnArea.RemoveAll();
	m_dArrColumnI33.RemoveAll();
	m_dArrColumnI22.RemoveAll();
	m_sArrColumnDes.RemoveAll();
	double ColArea = 0;
	double ColI33  = 0;
	double ColI22  = 0;
	double dWidthSlab = m_pStd->GetSlabWidth();
	double dEaColumn  = 0;
	CString szColumn = _T("");
	for(long nJijum=0; nJijum<=pBri->m_nQtyJigan; nJijum++)
	{
		if(pBri->GetTypeColumnByJijum(nJijum)==-1) continue;

		CWallApp   *pWall = pBri->GetInWall(nJijum-1);
		CColumnApp *pCol  = &pWall->m_Column;
		ColArea = pCol->GetColumnArea();
		ColI33  = pCol->GetColumnI33_I22(TRUE);
		ColI22  = pCol->GetColumnI33_I22(FALSE);
		dEaColumn  = pWall->m_nCountColumn;
		m_dArrColumnArea.Add(ColArea*dEaColumn/dWidthSlab);
		m_dArrColumnI33.Add(ColI33*dEaColumn/dWidthSlab);
		m_dArrColumnI22.Add(ColI22*dEaColumn/dWidthSlab);
		//
		double B = toM(pCol->m_dH[iOUTTER][iUPPER]);
		double H = toM(pCol->m_dB[iOUTTER][iUPPER]);
		double R = toM(pCol->m_dR[iOUTTER][iUPPER]);
		if(pCol->m_nType==COL_TYPE_CIRCLE)
			szColumn.Format("원형기둥 (D=%.3f)",B);
		else if(pCol->m_nType==COL_TYPE_RECTANGLE)
			szColumn.Format("사각기둥 (%.3fx%.3f)",B,H);
		else if(pCol->m_nType==COL_TYPE_TRACK)
		{
			B = B - R*2;
			H = H - R*2;
			if(B>0 && H>0)
				szColumn.Format("트랙형 (%.2fx%.2f,R=%.2f)",B+R*2,H+R*2,R);
			else if(H==0)
				szColumn.Format("트랙형 (B=%.2f,R=%.2f)",B+R*2,R);
			else
				szColumn.Format("트랙형 (H=%.2f,R=%.2f)",H+R*2,R);
		}
		else 
		{
			pCol->GetTvPlan();
			CTwinVector tv = pCol->m_tvArrPlan[iOUTTER][iUPPER].GetAt(0);
			double Length = toM(tv.GetLength());
			szColumn.Format("%d각형 (B=%.3f)",pCol->m_nAngSu,Length);
		}
		m_sArrColumnDes.Add(szColumn);
	}

	///////////////////////////////////////////////////////////////////////
	// 아치교일경우 아치리브의 두께..
	if(bArch)
	{
		CVector vPoint;
		long nSizeRib = m_vArrArchRib.GetSize();
		long n=0; for(n=0; n<nSizeRib; n++)
		{
			vPoint = m_vArrArchRib.GetAt(n);
			double dThickRib = m_pStd->GetThickArchRib(vPoint.x);

			if(dThickRib>0)
			{
				long nSizeThick = m_dArrThickArchRib.GetSize();
				long i=0; for(i=0; i<nSizeThick; i++)
				{
					double dThcick1 = m_dArrThickArchRib.GetAt(i);
					if(Compare(dThcick1, toM(dThickRib), "=")) break;
				}
				if(i==nSizeThick)
					m_dArrThickArchRib.Add(toM(dThickRib));
			}
		}

		m_dArrThickArchRib.Sort();
	}
}

long CARcBridgeDesignFrame::GetIndexSection(double dThick, BOOL bSlab, BOOL bArch)
{
	long nIndex = -1;
	long nSize = bArch? m_dArrThickArchRib.GetSize() : (bSlab ? m_dArrThickSlab.GetSize() : m_dArrThickWall.GetSize());

	double dThickArr = 0;
	double dThickCheck = Round(toM(dThick), 3);
	long i=0; for(i=0; i<nSize; i++)
	{
		if(bArch) 
			dThickArr = m_dArrThickArchRib.GetAt(i);
		else 
			dThickArr = bSlab ? m_dArrThickSlab.GetAt(i) : m_dArrThickWall.GetAt(i);
		dThickArr = Round(dThickArr, 3);
		if(Compare(dThickCheck, dThickArr, "="))
			return i;
	}

	return nIndex;
}

double CARcBridgeDesignFrame::GetDistRigidElement(CFEMManage *pFEM, CElement *pElement)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	long nCountJijum = pBri->GetCountJijum();
	if(pBri->m_nSelectRigidModeling == 0) return 0;

	BOOL bLsd = pBri->m_pARcBridgeDataStd->IsLsdDesign();
// 	CJoint *pJointSlab = NULL;
	CDPoint xyStt(0,0), xyEnd(0,0);
	//double dLengthElement = 0;
	double dLengthOffset = 0;
	if(pElement->m_nFlag == ELEMENT_UPPERSLAB || pElement->m_nFlag == ELEMENT_LOWERSLAB)
	{
		BOOL bUpperSlab = (pElement->m_nFlag == ELEMENT_UPPERSLAB);
		for(long nJijum=0; nJijum<nCountJijum; nJijum++)
		{
			// 아치형 중간지점은 강역을 설정할 수 없음...
			if(pBri->IsArchType() && nJijum==pBri->GetJijumConnectRibAndSlab()) continue;

			CJoint *pJntJijum = GetJointJijumOnSlab(pFEM,nJijum,TRUE);
			CDPoint vJnt = bLsd? toM(pJntJijum->m_vPointOrigin) : pJntJijum->m_vPointOrigin; 
			double RigidStt = vJnt.x - toM(pBri->GetDistRigidEndSlab(nJijum,TRUE,bUpperSlab));
			double RigidEnd = vJnt.x + toM(pBri->GetDistRigidEndSlab(nJijum,FALSE,bUpperSlab));
			CJoint *pJntStt = pFEM->GetJoint(pElement->m_nJStt);
			CJoint *pJntEnd = pFEM->GetJoint(pElement->m_nJEnd);
			xyStt = CDPoint(pJntStt->m_vPointOrigin.x, pJntStt->m_vPointOrigin.z);
			xyEnd = CDPoint(pJntEnd->m_vPointOrigin.x, pJntEnd->m_vPointOrigin.z);
			if(bLsd)
			{
				xyStt = toM(xyStt);
				xyEnd = toM(xyEnd);
			}

			if(Compare(xyStt.x , RigidEnd,_T(">"))) continue;
			if(Compare(xyEnd.x , RigidStt,_T("<"))) continue;

			//			dLengthElement = ~(xyStt - xyEnd) * 0.985;
			dLengthOffset = fabs(max(RigidStt, xyStt.x) - min(RigidEnd, xyEnd.x));
			// 			return min(dLengthElement, dLengthOffset);

			dLengthOffset *= Compare(xyStt.x , RigidStt,_T("<")) ? -1 : 1;		// (ARCBRIDGE-3187) 지점에서 떨어진 부재의 좌우측을 알기 위해서 +/-부호 적용

			return Round(dLengthOffset,3);
		}
	}
	else
	{
		for(long iUD = iUPPER; iUD <= iLOWER; iUD++)
		{
			if(iUD == iLOWER && pBri->IsBoxType() == FALSE) break;

			double sUD = iUD == iUPPER ? +1.0 : -1.0;

			// 아치교의 경우는 종경사를 고려해야 한다.
			CJoint *pJntStt = pFEM->GetJoint(pElement->m_nJStt);
			CJoint *pJntEnd = pFEM->GetJoint(pElement->m_nJEnd);
			double RigidStt = bLsd? toM(pFEM->GetJoint(0)->m_vPointOrigin.z) : pFEM->GetJoint(0)->m_vPointOrigin.z;
			double DistRigid = toM(pBri->GetDistRigidEndWall(pJntEnd->m_LongAttrib[LONG_1_JOINT_JIJUMNO], iUD == iUPPER));
			double RigidEnd = RigidStt - DistRigid;

			double dElDiff = 0;
			if(pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
			{
				long nJijum = pJntEnd->m_LongAttrib[LONG_1_JOINT_JIJUMNO];
				CLineInfo *pLine = pBri->GetLineBase();
				double dElStt	 = pLine->GetElevationPlanEl(pBri->GetStationOnJijum(0)-pBri->m_dWS/2);
				double dDist	 = (nJijum==0)? (-pBri->m_dWS/2) : ((nJijum==pBri->m_nQtyJigan)? pBri->m_dWE/2 : 0);
				double dElCheck	 = pLine->GetElevationPlanEl(pBri->GetStationOnJijum(nJijum) + dDist);
				dElDiff	 = toM(dElStt - dElCheck);
			}
			else if(pBri->IsBoxType())
			{
				BOOL bUpper = iUD == iUPPER ? TRUE : FALSE;

				CJoint *pJointSlab = NULL;
				pJointSlab = GetJointJijumOnSlab(pFEM, pJntEnd->m_LongAttrib[LONG_1_JOINT_JIJUMNO], TRUE, FALSE, bUpper);
				CVector vJnt = bLsd? toM(pJointSlab->m_vPointOrigin) : pJointSlab->m_vPointOrigin; 
				RigidStt = bUpper ? vJnt.z : vJnt.z + DistRigid;
				RigidEnd = bUpper ? RigidStt - DistRigid : vJnt.z;
			}

			xyStt = CDPoint(pJntStt->m_vPointOrigin.x, pJntStt->m_vPointOrigin.z);
			xyEnd = CDPoint(pJntEnd->m_vPointOrigin.x, pJntEnd->m_vPointOrigin.z);
			if(bLsd)
			{
				xyStt = toM(xyStt);
				xyEnd = toM(xyEnd);
			}

			if(Compare(xyStt.y , RigidStt,_T(">")) && Compare(xyEnd.y , RigidStt,_T(">"))) continue;
			if(Compare(xyStt.y , RigidEnd,_T("<")) && Compare(xyEnd.y , RigidEnd,_T("<"))) continue;


			//			dLengthElement = ~(xyStt - xyEnd) * 0.985;
			dLengthOffset = fabs(min(xyStt.y,RigidStt) - max(xyEnd.y,RigidEnd-dElDiff));
			// 			return min(dLengthElement, dLengthOffset);
			return Round(dLengthOffset*sUD,3);
		}
	}


	return 0.0;
}

void CARcBridgeDesignFrame::WriteModelingNormal(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long nIndexMiddle)
{
	CFEMManage *pFEM = &pBri->m_FemNormal;

	long nIndexSmall = 1;
	XLText->GoPosition(NO_MOVE,COL_POS_1);
	XLText->AddFormatText("$n$h$a%d.%d 평상시(2D Frame)$n",nIndexTitle,nIndexMiddle);
	XLText->AddFormatText("$h %d)$r해석 모델링$n$n$h$r",nIndexSmall++);

	//////////////////////////////////////////////////////////////////////////
	// 그림
	long nSizeRow = 15;
	CDomyun *pDom = new CDomyun;
	pDom->SetCWnd(m_pStd->m_pView);
	pDom->SetScaleDim(100);
	m_pStd->m_pARoadOptionStd->SetEnvType(pDom,HCAD_DIML);
	pDom->SetTextHeight(pDom->GetTextHeight(FALSE)*1.5);
	m_pStd->DrawDiagramModeling(pDom,pFEM);
	m_pStd->DimModelingSlab(pDom,pFEM,TRUE,TRUE,TRUE,TRUE,TRUE);
	m_pStd->DimModelingSlab(pDom,pFEM,TRUE,TRUE,TRUE,TRUE,FALSE);
	m_pStd->DimModelingWall(pDom,pFEM,-1,TRUE,TRUE,TRUE,TRUE);
	if(pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH) m_pStd->DimModelingArchRib(pDom,pFEM,TRUE,TRUE,TRUE,TRUE);
	m_pStd->DrawModelingSpringAndFix(pDom,pFEM);
	pDom->SetCalcExtRect();
	CDRect rect = pDom->GetExtRect();
	m_pStd->m_pARoadOptionStd->SetEnvType(pDom,HCAD_HIDD);
	pDom->Point(rect.left-pDom->Always(5),rect.bottom+pDom->Always(5));
	XLText->InsertDomyun(pDom,m_pStd->m_pView,nSizeRow,CXLFormatText::INSERT_DOM_CENTER | CXLFormatText::INSERT_DOM_MOVEROW);
	*XLText += 1;
	//////////////////////////////////////////////////////////////////////////
	if(pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
	{
		double dThickCrown = pBri->GetThickArchRib(0, TRUE);
		double dThickSp = max(m_pStd->m_pArchRib->GetThickArchRib(0, CHECK_POS_LEFT), m_pStd->m_pArchRib->GetThickArchRib(pBri->m_nQtyJigan, CHECK_POS_RIGHT));
		BOOL bOK = ((dThickSp/dThickCrown) > 1.5);
		CString strComp = (bOK)? ">" : "<" ;
		XLText->AddFormatText("$h$c-$r(스프링깅 H / 크라운 H) = (%.2f / %.2f) = %.2f$m+14$c%s$m+02$e1.5$n", dThickSp, dThickCrown, dThickSp/dThickCrown, strComp);
		XLText->AddFormatText("$h$r$c∴$r스프링깅부의 두께는 %.2f m로 적용한다.(교량의 계획과 설계 p479,반석기술)$n", bOK? dThickSp:dThickCrown);
	}

	XLText->AddFormatText("$h %d)$r절점 좌표$n",nIndexSmall++);
	MakeTableXZ(XLText, pBri, 0);			// nTyep : 0 = 평상시
	*XLText += 1;

	int nCntSlab = m_dArrThickSlab.GetSize();
	int nCntWall = m_dArrThickWall.GetSize();
	int nCntRow = nCntSlab + nCntWall +1;
	XLText->AddFormatText("$s+%02d$h %d)$r단면 특성$n",nCntRow+1,nIndexSmall++);
	MakeTableSectAffection(XLText, pBri, 0);
	*XLText += 1;

	if(pBri->m_nTypeBridge != BRIDGE_TYPE_SLAB && pBri->m_nSelectRigidModeling != 0)
	{
		XLText->AddFormatText("$s+10$h %d)$r강역구간 설정$n",nIndexSmall++);
		WriteRigidEnd(XLText, pBri, 0);
	}
}

void CARcBridgeDesignFrame::WriteModelingCoefficienOfPile(CXLFormatText *XLText, CRcBridgeRebar *pBri)
{
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	CFEMManage *pFEM = pBri->GetFemManage(COMBO_EARTHQUAKE);
	if(pFEM == NULL) return;
	if(m_pStd->m_pArrPileCalc.GetSize() == 0) return;


	XLText->GoPosition(NO_MOVE, 2);

	CDesignPile *pCalcPile = m_pStd->m_pArrPileCalc.GetAt(0);
	pCalcPile->CalculateCondition();
	pCalcPile->CalculatePileAttribute();

	double dScaleDim = 100;
	CDomyun *pDom = new CDomyun;
	pDom->SetCWnd(m_pStd->m_pView);
	pDom->SetScaleDim(dScaleDim);
	pDom->SetLTScale(dScaleDim * 5);

	CFootingApp *pFooting = pBri->GetFooting(0);
	CARcBridgeDBStandard DBStd(pBri, m_pStd->m_pARoadOptionStd, pData);

	long nTypeJongdanForRebarOrg = pBri->m_nTypeJongdanForRebar;
	pBri->m_nTypeJongdanForRebar = 2;
	pBri->GetTvVertSection_Std(0, TRUE, FALSE, FALSE);
	DBStd.m_pDrawVertSection->DrawVertSection(pDom, TRUE, &pBri->m_tvArrVertSection);
	DBStd.m_pDrawExFooting->DrawFront(pDom, &pFooting->m_exFooting, FALSE);
	long nCount = pBri->GetCountInWall();
	for(long ix = 0; ix < nCount; ix++)
	{
		CWallApp *pWall = pBri->GetInWall(ix);
		if(pWall)
			DBStd.m_pDrawWall->DrawFront(pDom, pWall, TRUE);
	}
	DBStd.m_pDrawVertSection->DrawExFootingBoxType(pDom, TRUE);
	DBStd.m_pDrawVertSection->DimVerSectionBoxSideStt(pDom, 0, FALSE);
	DBStd.m_pDrawVertSection->DimVerSectionBoxSideEnd(pDom, 0, FALSE);
	DBStd.m_pDrawVertSection->DimVerSectionHunch(pDom, 0, TRUE, FALSE, FALSE);
	DBStd.m_pDrawVertSection->DimVerSectionHunch(pDom, 0, FALSE, FALSE, FALSE);
	DBStd.m_pDrawExFooting->DimFrontLowerBox(pDom, pBri, 0, FALSE);
	DBStd.m_pDrawVertSection->DimVertSectionLowerSlab(pDom, 0, TRUE, FALSE);

	pBri->m_nTypeJongdanForRebar = nTypeJongdanForRebarOrg;

	*pCalcPile->m_pDomPlan << *pDom;
	XLText->OutXL();

	long nPos = COL_POS_1;
	long nRow = XLText->GetRowPostion();
	XLText->GoPosition(nRow, nPos);
	XLText->AddFormatText("$s+07$h1) 설계조건$n");
	XLText->GoPosition(nRow+1, nPos);
	nRow = pCalcPile->WritePileCondition(XLText->GetExcelControl(),nRow+1,0,TRUE,0);

	XLText->GoPosition(nRow+1, nPos);
	XLText->OutXL();
	nRow = XLText->GetRowPostion();
	nRow = XL_LINES_PER_PAGE*((long)(nRow/XL_LINES_PER_PAGE)+1);
	XLText->GoPosition(nRow, nPos);
	XLText->AddFormatText("$h2) 말뚝의 특성치 계산$n");
	nRow = pCalcPile->WritePileAttribute(XLText->GetExcelControl(),nRow+1,0,TRUE,0,1,TRUE);

	double dWidthSlab = m_pStd->GetSlabWidth();
	XLText->GoPosition(nRow+1, nPos);
	XLText->OutXL();
	XLText->AddFormatText("$s+40$h3) 말뚝의 적용 스프링계수 : 교량폭원 = %.3f m$n$n",dWidthSlab);

	// 29920
	XLText->GoPosition(NO_MOVE,nPos);
	XLText->AddTable(4, 11, 1, 0);
	XLText->AddText("$h$me3$c구 분$m+04$me3$c산출식$m+04$me3$c단 위$n");
	XLText->AddText("$h$me3$cK1$m+04$me3$c4EIβ³$m+04$me3$ckN/m$n");
	XLText->AddText("$h$me3$cK2$m+04$me3$c2EIβ²$m+04$me3$ckN/rad$n");
	XLText->AddText("$h$me3$cK3$m+04$me3$c2EIβ²$m+04$me3$ckN/rad$n");
	XLText->AddText("$h$me3$cK4$m+04$me3$c2EIβ$m+04$me3$ckN.m/rad$n$n");

	XLText->AddFormatText("$h%s 평상시$n", GetNumberString(1, NUM_CIRCLE));
	MakeTablePileSpringFactor(XLText, pBri, FALSE);
	if(pBri->IsApplyEarthQuakeDesign())
	{
		XLText->AddFormatText("$h%s 지진시$n", GetNumberString(2, NUM_CIRCLE));
		MakeTablePileSpringFactor(XLText, pBri, TRUE);

	}

	pBri->GetTvVertSection_Std(0, TRUE, FALSE, TRUE);
}

void CARcBridgeDesignFrame::MakeTablePileSpringFactor(CXLFormatText *pXLText, CRcBridgeRebar *pBri, BOOL bEarthquake)
{
	CFootingApp *pFooting = &pBri->m_footingBox;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	CDesignPile *pCalcPile = m_pStd->m_pArrPileCalc.GetAt(0);
	
	BOOL bLsd = pData->IsLsdDesign();
	CString szTitle(_T("")), szCal(_T(""));

	CLongArray lArrEa;
	for(long ix = 0; ix < pFooting->m_exFooting.m_Pile.m_nCountOrg; ix++)
	{
		COrgInfo *pOrg = pFooting->m_exFooting.m_Pile.m_pArrOrgInfo.GetAt(ix);
		long nSize = lArrEa.GetSize();
		long kk = 0;
		for(kk=0; kk<nSize; kk++)
		{
			long nEa = lArrEa.GetAt(kk);
			if(pOrg->countSide == nEa)
			{
				break;
			}
		}
		if(kk == nSize)
		{
			lArrEa.Add(pOrg->countSide);
		}
	}

	// 지진시에는 전폭으로 적용해야 하니까 나눠주는 교량폭을 1로
	double dWidthSlab = bEarthquake? 1.0 : m_pStd->GetSlabWidth();

	long nType = 3;
	double dVal = 0;
	pXLText->GoPosition(NO_MOVE, COL_POS_1);
	pXLText->AddTable(nType * lArrEa.GetSize(), 19, 1, 0);
	pXLText->AddText("$h$me4$c구 분$m+05$me4$c스프링 정수$m+05$me2$c말뚝개수$m+03$me3$c적용 스프링계수$m+04$me2$c단위$n");

	long nT = bEarthquake ? (bLsd? PILE_CASE_EXTREME : PILE_CASE_EARTH) : PILE_CASE_NORMAL;

	for(long kk = 0; kk < lArrEa.GetSize(); kk++)
	{
		long nEaPile = lArrEa.GetAt(kk);

		long n = 0;
		for(n = 0; n < nType; n++)
		{
			pXLText->GoPosition(NO_MOVE, COL_POS_1);
			if(n==0) szTitle = "연직방향 스프링계수";
			else if(n==1) szTitle = "수평방향 스프링계수";
			else if(n==2) szTitle = "회전 스프링계수";

			if(n==2) szCal = pData->m_szTonfM + "/rad";
			else	 szCal = pData->m_szTonf_M;

			if(n==0) dVal = GetValueUnitChange(pCalcPile->m_Kv, UNIT_CGS_TONF_M, pData->m_szTonf_M);
			else if(n==1) dVal = GetValueUnitChange(pCalcPile->m_K1_Fix[nT], UNIT_CGS_TONF_M, pData->m_szTonf_M);
			else if(n==2) dVal = GetValueUnitChange(pCalcPile->m_K4_Fix[nT], UNIT_CGS_TONFM, pData->m_szTonfM);

			pXLText->AddFormatText("$h$me4$c%s$m+05$me4$c%s$m+05$me2$c$%d$m+03$me3$c%s$m+04$c$me2%s$n",
				szTitle, pData->GetStrDouble(dVal, UNIT_CGS_TONF_M), nEaPile, pData->GetStrDouble(dVal*nEaPile/dWidthSlab, UNIT_CGS_TONF_M), szCal);
		}
	}

	pXLText->AddText("$n");
}


void CARcBridgeDesignFrame::WriteModelingCoefficienOfSubgradeReaction(CXLFormatText *XLText, CRcBridgeRebar *pBri, BOOL bEarthQuake)
{
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	CFEMManage *pFEM = pBri->GetFemManage(bEarthQuake ? COMBO_EARTHQUAKE : COMBO_SERVICE);
	if(pFEM == NULL) return;

	BOOL bLsd = pData->IsLsdDesign();
	XLText->GoPosition(NO_MOVE,COL_POS_1);

	XLText->GoPosition(NO_MOVE, 2);
	XLText->AddOutLine(10,19);			

	XLText->AddText("$a   < 연직방향 지반반력계수 >$n$n");		
	XLText->AddText("$m+13$p[Kv_1_SI]$n");
	XLText->AddText("$h   - 사질토층이 우세한 경우$n$n");
	XLText->AddText("$m+13$p[Kv_2_SI]$n");
	XLText->AddText("$h   - 사질토와 점성토의 혼합층인 경우$n$n");	
	XLText->AddText("$m+13$p[Kv_3_SI]$n");
	XLText->AddText("$h   - 점성토층이 우세한 경우$n$n");
	XLText->AddText("'α = Eo 의 산정방법에 대한 보정계수$n");
	XLText->AddText("'Eo = 지반의 탄성계수$n$n");

	//
	long nVerticalFactor = pBri->m_nVerticalFactor;
	if(nVerticalFactor == 0)
		XLText->AddText("$h$r$p[Kv_1_SI]$n$n$n");
	else if(nVerticalFactor == 1)
		XLText->AddText("$h$r$p[Kv_3_SI]$n$n$n");
	else 
		XLText->AddText("$h$r$p[Kv_2_SI]$n$n$n");

	XLText->AddFormatText("$h$r여기서,$r$r$r$p[Kvo_3_SI]$n$nEo : 지반탄성계수 (%s)$n$n",pData->m_szKgf_CM2);

	long nJijum = 0;
	long nCase = bEarthQuake ? MODULUS_FOUNDATION_EQ : MODULUS_FOUNDATION_NORMAL;
	CModulusFoundation *pMF = m_pStd->m_pModulusFoundation;
	CFootingApp *pFooting = &pBri->m_footingBox;
	CBoringDataEx *pBoringEx = pBri->GetBoringDataExByJijumNumber(nJijum);
	CConcSymbol sym;

	double alpha = pMF->m_dAlpah[nCase];
	CString str = _T("");
	CString szUnit = pData->IsSI() ? _T("kN/m²") : UNIT_CGS_KGF_CM2;
	if(bLsd)
	{
		szUnit = pData->m_szTonf_M2;
	}

	long nSizeCalc = (bEarthQuake) ? pMF->m_nQtyCalc : 1;

	XLText->AddFormatText("$h'(1)$r 지반반력계수 산정$n");

	// (ARCBRIDGE-3609) 위치마다 따로 찍는다.
/*	if(nSizeCalc <= 1)
	{
		CBoringData *pBoring = pBri->GetBoringDataJijumBase(nJijum);
		double NValue = pFooting->m_dNValueBaseGround;
		double Eo = (pFooting->m_dEo_Input_Vert > 0)? pFooting->m_dEo_Input_Vert > 0 : (pFooting->m_dNValue_Eo_Vert * NValue);
		double dEo(0);
		if(bLsd)
			dEo = GetValueUnitChange(Eo * 10, UNIT_CGS_TONF_M2, szUnit);
		else
			dEo = GetValueUnitChange(Eo, UNIT_CGS_KGF_CM2, szUnit);

		XLText->AddFormatText("$h$c$me1α$m+02$c=$m+01$f0%.0f$m+01$n", alpha);
		if(pFooting->m_dEo_Input_Vert > 0)
		{
			if(bLsd)
				dEo = GetValueUnitChange(pFooting->m_dEo_Input_Vert * 10, UNIT_CGS_TONF_M2, szUnit);
			else
				dEo = GetValueUnitChange(pFooting->m_dEo_Input_Vert, UNIT_CGS_KGF_CM2, szUnit);
			XLText->AddFormatText("$h$c$me1Eo$m+02$c=$m+01$me2$i$f2%.2f$m+03%s$n",dEo,pData->m_szKgf_CM2);
		}
		else
		{
			double dFactor(0);
			if(bLsd)
				dFactor = GetValueUnitChange(pFooting->m_dNValue_Eo_Vert * 10, UNIT_CGS_TONF_M2, szUnit);
			else
				dFactor = GetValueUnitChange(pFooting->m_dNValue_Eo_Vert, UNIT_CGS_KGF_CM2, szUnit);

			if(bLsd)
				str.Format("%.3f x N = %.3f x %.0f = %.4f %s",dFactor,dFactor,NValue,dEo,szUnit);
			else
				str.Format("%.0f x N = %.0f x %.0f = %.3f %s",dFactor,dFactor,NValue,dEo,szUnit);
			XLText->AddFormatText("$h$c$me1Eo$m+02$c=$m+01%s$n",str);
		}
	}
*/
	//////////////////////////////////////////////////////////////////////////
	BOOL bTerzaghiKv = (pBri->m_nGroundFactorKV == 0 || pBri->m_nGroundFactorKV == 1) ? TRUE : FALSE;
	BOOL bHoriSpring = (bEarthQuake && pBri->m_nEquivalentApplyKsb == 0) ? TRUE : FALSE;
	BOOL bNLLInk = (bEarthQuake && pBri->m_n2DEQSpringType == 0  || bEarthQuake==FALSE && pBri->m_n2DGeneralSpringType == 0) ? TRUE : FALSE;

	CString szPow = _T("( -3 / 4 )");
	CString szLBase = bLsd? _T("300") : _T("0.3");
	if(pBri->m_nVerticalFactor==0)
	{
		szPow = "( -1 / 2 )";
	}
	else if(pBri->m_nVerticalFactor==1)
	{
		szPow = "-1";
	}

	BOOL bCalcBoring = (bEarthQuake && pMF->m_lArrIndex.GetSize() > 0) ? TRUE : FALSE;

	long nSizePos = bEarthQuake ? MODULUS_POS_MAX : 1;
	for(long px=0; px<nSizePos; px++)
	{
		switch (px)
		{
		case MODULUS_POS_LOWERSLAB:
			str = hggettext("하부슬래브"); break;
		case MODULUS_POS_STTWALL:
			str = hggettext("시점벽체"); break;
		case MODULUS_POS_ENDWALL:
			str = hggettext("종점벽체"); break;
		default:
			str = _T("");
		}
		XLText->AddFormatText("$h'▶$r %s$n", str);

		double L = pMF->m_L[px];
		double B = pMF->m_B[px];
		double Bv = pMF->m_Bv[px][nCase];

		nSizeCalc = (bEarthQuake && px > MODULUS_POS_LOWERSLAB) ? pMF->m_nQtyCalc : 1;

		for(long nCalc = 0; nCalc < nSizeCalc; nCalc++)
		{
			long nIdxBoring = bCalcBoring ? pMF->m_lArrIndex.GetAt(nCalc) : 0;
			CBoringData *pBoring = bCalcBoring ? pBoringEx->GetBoringByIndex(nIdxBoring) : pBri->GetBoringDataJijumBase(nJijum);

			double Kvo  = pMF->m_Kvo[px][nCase][nCalc];
			double KvBL = pMF->m_KvBL[px][nCase][nCalc];
			double KvBB = pMF->m_KvBB[px][nCase][nCalc];
			double Ks   = pMF->m_Ks[px][nCase][nCalc];
			double NValue = px == MODULUS_POS_LOWERSLAB ? pFooting->m_dNValueBaseGround : pBoring->m_nNValue;
			double Eo = pFooting->m_dNValue_Eo_Vert * NValue;
			double dEo = GetValueUnitChange(Eo, UNIT_CGS_KGF_CM2, szUnit);
			if(bLsd)
				dEo = GetValueUnitChange(Eo * 10, UNIT_CGS_TONF_M2, szUnit);
			if(pFooting->m_dEo_Input_Vert > 0)
			{
				dEo = GetValueUnitChange(pFooting->m_dEo_Input_Vert, UNIT_CGS_KGF_CM2, szUnit);
				if(bLsd)
					dEo = GetValueUnitChange(pFooting->m_dEo_Input_Vert * 10, UNIT_CGS_TONF_M2, szUnit);
			}

			if(nSizeCalc > 1)
			{
				XLText->AddFormatText("$h %s 토층 %d : %s$n", GetNumberString(nCalc + 1, NUM_CIRCLE), nCalc + 1, sym.m_strSoilName[pBoring->m_nNameSoil]);
			}
			if(TRUE) // (nSizeCalc > 1)		// (ARCBRIDGE-3609) 위치마다 따로 다 찍는다.
			{
				XLText->AddFormatText("$h$c$me1α$m+02$c=$m+01$f0%.0f$m+01$n", alpha);
				if(pFooting->m_dEo_Input_Vert > 0)
				{
					XLText->AddFormatText("$h$c$me1Eo$m+02$c=$m+01$me2$i$f%d%s$m+03%s$n", bLsd? 4 : 2, pData->GetStrDouble(dEo, UNIT_CGS_TONF_M2, FALSE, 2),pData->m_szKgf_CM2);
				}
				else
				{
					double dFactor = GetValueUnitChange(pFooting->m_dNValue_Eo_Vert, UNIT_CGS_KGF_CM2, _T("kN/m²"));
					if(bLsd)
						dFactor = GetValueUnitChange(pFooting->m_dNValue_Eo_Vert* 10, UNIT_CGS_TONF_M2, szUnit);

					str.Format("%s x N = %s x %.0f = %s %s",
						pData->GetStrDouble(dFactor, UNIT_CGS_TONF_M2, FALSE, 0), pData->GetStrDouble(dFactor, UNIT_CGS_TONF_M2, FALSE, 0), NValue,
						pData->GetStrDouble(dEo, UNIT_CGS_TONF_M2),szUnit);
					XLText->AddFormatText("$h$c$me1$me1Eo$m+02$c=$m+01%s$n",str);
				}
			}
			XLText->AddFormatText("$h' - 연직방향 지반반력계수$n");
			if(L < B)
			{
				XLText->AddFormatText("$h$c$me1Bv$m+02$c=$m+01√( B × L )$m+03$c=$m+01√( %s × %s )$m+06$c=$m+01$c$me2$f%d%s$m+03%s  (L < B)$n", 
					pData->GetStrDouble(B, UNIT_CGS_M), pData->GetStrDouble(L, UNIT_CGS_M), bLsd? 0 : 3, pData->GetStrDouble(Bv, UNIT_CGS_M), pData->m_szM);
				XLText->AddFormatText("$h$c$me1Kvo$m+02$c=$r1 / %s x %.3f x %s$m+10$c=$m+01$c$me2%s$m+03%s$n", 
					szLBase, alpha, pData->GetStrDouble(dEo, UNIT_CGS_TONF_M2), pData->GetStrDouble(Kvo, UNIT_CGS_TONF_M2, TRUE, -1, TRUE), szUnit);
				XLText->AddFormatText("$h$c$me1Kv$m+02$c=$r%s x (%s / %s) ^ %s$m+10$c=$m+01$c$me2%s$m+03%s$n", 
					pData->GetStrDouble(Kvo, UNIT_CGS_TONF_M2, TRUE), pData->GetStrDouble(Bv, UNIT_CGS_M), szLBase, szPow, pData->GetStrDouble(KvBL, UNIT_CGS_TONF_M2, TRUE, -1, TRUE), szUnit);
			}
			else
			{
				if(bTerzaghiKv)
				{
					if(pBri->m_nGroundFactorKV == 0) // 보정식 사용
					{
						XLText->AddText("$h$r$p[Kv_Terz]$n$n$n$n");
					}
					XLText->AddFormatText("$h$i$me1Bv$m+02$c=$m+01√( B × B )$m+03$c=$m+01√( %s × %s )$m+06$c=$m+01$c$me2%s$m+03%s$n", 
						pData->GetStrDouble(B, UNIT_CGS_M), pData->GetStrDouble(B, UNIT_CGS_M), pData->GetStrDouble(Bv, UNIT_CGS_M, FALSE, -1, TRUE), pData->m_szM);
					XLText->AddFormatText("$h$i$me1Kvo$m+02$c=$r1 / %s x %.3f x %s$m+10$c=$m+01$c$me2%s$m+03%s$n", 
						szLBase, alpha, pData->GetStrDouble(dEo, UNIT_CGS_TONF_M2), pData->GetStrDouble(Kvo, UNIT_CGS_TONF_M2, TRUE, -1, TRUE), szUnit);
					XLText->AddFormatText("$h$i$me1Kv(BxB)$m+02$c=$r%s x (%s / %s) ^ %s$m+10$c=$m+01$c$me2%s$m+03%s$n", 
						pData->GetStrDouble(Kvo, UNIT_CGS_TONF_M2, TRUE), pData->GetStrDouble(B, UNIT_CGS_M), szLBase, szPow, pData->GetStrDouble(KvBB, UNIT_CGS_TONF_M, TRUE, -1, TRUE), szUnit);
					XLText->AddFormatText("$h$i$me1Kv(BxL)$m+02$c=$r%.3f x Kv(BxB) = %.3f x %s$m+10$c=$m+01$c$me2%s$m+03%s$n", 
						pMF->m_TerzaghiFactor, pMF->m_TerzaghiFactor, pData->GetStrDouble(KvBB, UNIT_CGS_TONF_M), pData->GetStrDouble(KvBL, UNIT_CGS_TONF_M, TRUE, -1, TRUE), szUnit);
				}
				else
				{
					XLText->AddFormatText("$h$c$me1Bv$m+02$c=$m+01√( B × B )$m+03$c=$m+01√( %s × %s )$m+06$c=$m+01$c$me2%s$m+03%s  (L > B)$n", 
						pData->GetStrDouble(B, UNIT_CGS_M), pData->GetStrDouble(B, UNIT_CGS_M), pData->GetStrDouble(Bv, UNIT_CGS_M, FALSE, -1, TRUE), pData->m_szM);
					XLText->AddFormatText("$h$c$me1Kvo$m+02$c=$r1 / %s x %.3f x %s$m+10$c=$m+01$c$me2%s$m+03%s$n", 
						szLBase, alpha, pData->GetStrDouble(dEo, UNIT_CGS_TONF_M2), pData->GetStrDouble(Kvo, UNIT_CGS_TONF_M2, TRUE, -1, TRUE), szUnit);
					XLText->AddFormatText("$h$c$me1Kv$m+02$c=$r%s x (%s / %s) ^ %s$m+10$c=$m+01$c$me2%s$m+03%s$n", 
						pData->GetStrDouble(Kvo, UNIT_CGS_TONF_M2, TRUE), pData->GetStrDouble(Bv, UNIT_CGS_M), szLBase, szPow, pData->GetStrDouble(KvBL, UNIT_CGS_TONF_M2, TRUE, -1, TRUE), szUnit);
				}
			}
			if(bHoriSpring)
			{
				XLText->AddFormatText("$h' - 전단 지반반력계수$n");
				XLText->AddFormatText("$h$c$me1Ksb$m+02$c=$rλ × Kv = 1 / %.1f × %s$m+10$c=$m+01$c$me2%s$m+03%s$n", 
					pBri->m_dEquivalentApplyKsb, pData->GetStrDouble(KvBL, UNIT_CGS_TONF_M2, TRUE), pData->GetStrDouble(Ks, UNIT_CGS_TONF_M2, TRUE, -1, TRUE), szUnit);
			}
			XLText->AddText("$n");
		}

		if(px == MODULUS_POS_LOWERSLAB)
		{

		}
	}

	//////////////////////////////////////////////////////////////////////////
	// 기초 TABLE
	

	long nSize = m_xyArrSpringLower.GetSize();
	CDoubleArray dArrDistSpring;
	double dDistSpring = 0;
	for(long ix = 0; ix < nSize; ix++)
	{
		dDistSpring = 0;
		if(ix < nSize - 1)
		{
			if(bLsd)
				dDistSpring += (m_xyArrSpringLower.GetAt(ix + 1).x - m_xyArrSpringLower.GetAt(ix).x) / 2.;
			else
				dDistSpring += toM(m_xyArrSpringLower.GetAt(ix + 1).x - m_xyArrSpringLower.GetAt(ix).x) / 2.;
		}
		if(ix > 0)
		{
			if(bLsd)
				dDistSpring += (m_xyArrSpringLower.GetAt(ix).x - m_xyArrSpringLower.GetAt(ix - 1).x) / 2.;
			else
				dDistSpring += toM(m_xyArrSpringLower.GetAt(ix).x - m_xyArrSpringLower.GetAt(ix - 1).x) / 2.;
		}
		if(pBri->m_bSpringSpaceIncludeWallTick && pBri->m_bExistProtectionRiseFooting == FALSE)
		{
			if(ix == 0)
				dDistSpring += bLsd ? (pBri->m_dWS / 2.) : toM(pBri->m_dWS / 2.);
			if(ix == nSize - 1)
				dDistSpring += bLsd ? (pBri->m_dWE / 2.) : toM(pBri->m_dWE / 2.);
		}

		long nSizeArr = dArrDistSpring.GetSize();
		BOOL bExistDist = FALSE;
		for(long kk = 0; kk < nSizeArr; kk++)
		{
			double dDist = dArrDistSpring.GetAt(kk);
			if(Compare(dDistSpring, dDist, _T("=")))
			{
				bExistDist = TRUE;
				break;
			}
		}
		if(bExistDist == FALSE)
		{
			dArrDistSpring.Add(dDistSpring);
		}
	}

	long nIdxCalc = 0;
	double KvBL = pMF->m_KvBL[MODULUS_POS_LOWERSLAB][nCase][nIdxCalc];
	double Ks   = pMF->m_Ks[MODULUS_POS_LOWERSLAB][nCase][nIdxCalc];

	long nSizeArr = dArrDistSpring.GetSize();

	double dApplyWidth = bEarthQuake? (bLsd? frM(m_pStd->GetSlabWidth(-1)) : m_pStd->GetSlabWidth(-1)) : 1.0;
	BOOL bHoriUnLimited = (pBri->IsPileBase(-1) == FALSE && bEarthQuake == FALSE) ? TRUE : FALSE;

	long nSizeTable = bHoriSpring ? nSizeArr * 2 + bHoriUnLimited : nSizeArr + bHoriUnLimited;
	XLText->AddFormatText("$h'(2)$r 지반반력계수 : 하부 슬래브$n");
	XLText->GoPosition(NO_MOVE, 2);
	XLText->AddTable(nSizeTable, 23, 1, 0);
	char *szTitle[] = { "지반반력계수", "계   산   식", "해당 절점" };
	int nWidthTitle[] = { 3, 13, 8 };
	for(long nCol = 0 ; nCol < 3; nCol++)
	{
		str = szTitle[nCol];
		XLText->AddMergeFormatText(0, nWidthTitle[nCol]-1, "$c%s", str);
		XLText->AddFormatText("$m+%02d", nWidthTitle[nCol]);
	}

	double dSpringZ = 0, dSpringS = 0;
	CString szJointMatch = _T("");
	for(long ix = 0; ix < nSizeArr; ix++)
	{
		szJointMatch = _T("");
		dDistSpring = dArrDistSpring.GetAt(ix);
		dSpringZ = Round(pMF->m_KvBL[MODULUS_POS_LOWERSLAB][nCase][nIdxCalc] * dDistSpring * dApplyWidth, bLsd? 6 : 3);
		dSpringS = Round(pMF->m_Ks[MODULUS_POS_LOWERSLAB][nCase][nIdxCalc] * dDistSpring * dApplyWidth, bLsd? 6 : 3);

// 		szJointMatch = _T("");
		long nIdxStt = GetNumberJointSlabStt(pFEM, FALSE);
		long nIdxEnd = GetNumberJointSlabEnd(pFEM, FALSE);
		CLongArray lArrJoint;
		for(long kk = nIdxStt; kk <= nIdxEnd; kk++)
		{
			CJoint *pJnt = pFEM->GetJoint((unsigned short)kk);
			if(pJnt->m_LongAttrib[LONG_0_JOINT_TYPE] != JOINT_LOWERSLAB) continue;
			if(bNLLInk == FALSE)
			{
				if(Compare(pJnt->m_vSpring.z, 0.0, _T("<="))) continue;
				if(Compare(pJnt->m_vSpring.z, dSpringZ, _T("="), 0.001))
				{
					lArrJoint.Add(kk + 1);
				}
			}
			else
			{
				CNullLLink *pLink = pFEM->GetNLLinkByJointIndex((unsigned short)kk);
				if(pLink == NULL) continue;
				CNLProp *pProp = pFEM->GetNLPropByName(pLink->m_szNLProp);
				if(pProp == NULL) continue;
				if(Compare(pProp->m_vecData[0].m_dKE, 0.0, _T("<="), 0.001)) continue;
				if(Compare(pProp->m_vecData[0].m_dKE, dSpringZ, _T("="), 0.001))
				{
					lArrJoint.Add(kk + 1);
				}
			}
		}
		szJointMatch = GetStringApplyJoint(lArrJoint);

		for(long nS = 0; nS < 2; nS++)
		{
			if(nS == 1 && bHoriSpring == FALSE) break;

			XLText->AddText("$n");
			XLText->GoPosition(NO_MOVE, 2);
			XLText->AddMergeFormatText(0, nWidthTitle[0]-1,nS == 0 ? "$cKv%d" : "$cKsb%d",ix + 1);
			XLText->AddFormatText("$m+%02d",nWidthTitle[0]);
			if(bEarthQuake)
			{
				XLText->AddMergeFormatText(0, nWidthTitle[1]-1,"$c%s ⅹ %s x %s  = %s %s", 
					pData->GetStrDouble(dDistSpring, UNIT_CGS_M, FALSE, -1, FALSE, 6), pData->GetStrDouble(nS == 0 ? KvBL : Ks, UNIT_CGS_TONF_M2, TRUE, -1, FALSE, 11), 
					pData->GetStrDouble(dApplyWidth, UNIT_CGS_M, FALSE, -1, FALSE, 6), pData->GetStrDouble(nS == 0 ? dSpringZ : dSpringS, UNIT_CGS_TONF_M, TRUE, -1, FALSE, 11), pData->m_szTonf_M);
			}
			else
			{
				XLText->AddMergeFormatText(0, nWidthTitle[1]-1,"$c%s ⅹ %s  = %s %s", 
					pData->GetStrDouble(dDistSpring, UNIT_CGS_M, FALSE, -1, FALSE, 6), pData->GetStrDouble(nS == 0 ? KvBL : Ks, UNIT_CGS_TONF_M2, TRUE, -1, FALSE, 11),
					pData->GetStrDouble(nS == 0 ? dSpringZ : dSpringS, UNIT_CGS_TONF_M, TRUE, -1, FALSE, 11), pData->m_szTonf_M);
			}
			XLText->AddFormatText("$m+%02d", nWidthTitle[1]);

			XLText->AddMergeFormatText(0,nWidthTitle[2]-1,"$c%s", szJointMatch);
			XLText->AddFormatText("$m+%02d",nWidthTitle[2]);
		}
	}

	if(bHoriUnLimited)
	{
		XLText->AddText("$n");
		XLText->GoPosition(NO_MOVE, 2);
		XLText->AddMergeFormatText(0, nWidthTitle[0]-1,"$cKh");
		XLText->AddFormatText("$m+%02d",nWidthTitle[0]);

		XLText->AddMergeFormatText(0, nWidthTitle[1]-1,"$e   1E%d %s   (무한강성)", 
			bLsd? 7 : 10, pData->m_szTonf_M2);
		XLText->AddFormatText("$m+%02d", nWidthTitle[1]);

		long nMid = GetIndexJointLowerSlabMid(pFEM);
		szJointMatch.Format(_T("%d"), nMid + 1);
		XLText->AddMergeFormatText(0,nWidthTitle[2]-1,"$c%s", szJointMatch);
		XLText->AddFormatText("$m+%02d",nWidthTitle[2]);
		XLText->AddText("$n");
	}
	XLText->AddText("$n");

	//////////////////////////////////////////////////////////////////////////
	// 벽체 TABLE
	if(bEarthQuake == FALSE) return;

	nSizeCalc = pMF->m_nQtyCalc;
	long nSizeJoint = pFEM->GetJointSize();

	for(long nLR=iLEFT; nLR <= iRIGHT; nLR++)
	{
		CLongArray lArrSpringWall;
		BOOL bStt = (nLR == iLEFT);

		double dPosXCen = (m_xyArrSpringWall[iLEFT].GetAt(0).x + m_xyArrSpringWall[iRIGHT].GetAt(0).x) / 2.;

		for(long ix = 0; ix < nSizeJoint; ix++)
		{
			CJoint *pJnt = pFEM->GetJoint((unsigned short)ix);
			if(pJnt->m_LongAttrib[LONG_0_JOINT_TYPE]  == JOINT_MIDWALL) continue;
			if(pJnt->m_LongAttrib[LONG_0_JOINT_TYPE]  == JOINT_NLLINK) continue;
			if(pJnt->m_LongAttrib[LONG_0_JOINT_TYPE]  == JOINT_UPPERSLAB) continue;
			if(pJnt->m_LongAttrib[LONG_0_JOINT_TYPE]  == JOINT_LOWERSLAB) continue;

			if(bStt && pJnt->m_vPointReal.x > dPosXCen) continue;
			if(!bStt && pJnt->m_vPointReal.x < dPosXCen) continue;

			CDPoint xyJoint = CDPoint(pJnt->m_vPoint.x, pJnt->m_vPoint.z);
			if(pJnt->m_BoolAttrib[BOOL_0_JOINT_SPRING])
			{
				lArrSpringWall.Add(ix);
			}
		}

		XLText->AddText("$n");
		XLText->AddFormatText("$h'(3)$r 지반반력계수 : %s벽체$n", bStt? _T("시점") : _T("종점"));

		CLineInfo *pLine = pBri->GetLineBase();
		BOOL bUpperSlope = (pBri->IsBoxType() && pBri->m_bSlopeLowerSlab == FALSE) ? TRUE : FALSE;
		double dElBaseAdd = bUpperSlope ? 0 : pLine->GetElevationPlanEl(pBri->GetStationOnJijum(0,0) - pBri->m_dWS/pBri->GetAngleJijum(0).y/2.) - pBri->GetThickPave() - pBri->m_dTS / 2.;

		long nTableIndex(0);
		double dElevationBase = toM(pBri->m_dELSB + pBri->m_dTS_Lower / 2.);
		double dElevationTop = dElevationBase + toM(max(pBri->GetHeightModelJijum(0), pBri->GetHeightModelJijum(pBri->m_nQtyJigan)));
		for(long nCalc = 0; nCalc < nSizeCalc; nCalc++)
		{
			double dElevationBoringTop = bCalcBoring ? pMF->m_dArrElevationTop.GetAt(nCalc) : dElevationTop;
			double dElevationBoringBot = bCalcBoring ? pMF->m_dArrElevationBottom.GetAt(nCalc) : dElevationBase;

			if(bLsd)
			{
				dElevationBoringTop = frM(dElevationBoringTop);
				dElevationBoringBot = frM(dElevationBoringBot);
			}

			dArrDistSpring.RemoveAll();
			
			long nPos = (nLR==iLEFT)? MODULUS_POS_STTWALL : MODULUS_POS_ENDWALL;
			long nSize = m_xyArrSpringWall[nLR].GetSize();
			double dDistSpring = 0;
			for(long ix = 0; ix < nSize; ix++)
			{
				dDistSpring = 0;
				double dElevationSpring = bLsd? (m_xyArrSpringWall[nLR].GetAt(ix).y + dElBaseAdd) : toM(m_xyArrSpringWall[nLR].GetAt(ix).y + dElBaseAdd);
				// (ARCBRIDGE-1709) 벽체스프링은 스프링위치가 지반정보 경계위치와 같다면 위에지반정보를 적용하자. 
				if(dElevationSpring >= dElevationBoringTop) continue;
				if(dElevationSpring < dElevationBoringBot) continue;

				if(ix < nSize - 1)
				{
					if(bLsd)
						dDistSpring += (dElevationSpring - (m_xyArrSpringWall[nLR].GetAt(ix + 1).y)) / 2.;
					else
						dDistSpring += (dElevationSpring - toM(m_xyArrSpringWall[nLR].GetAt(ix + 1).y)) / 2.;
				}
				if(ix > 0)
				{
					if(bLsd)
						dDistSpring += ((m_xyArrSpringWall[nLR].GetAt(ix - 1).y) - dElevationSpring) / 2.;
					else
						dDistSpring += (toM(m_xyArrSpringWall[nLR].GetAt(ix - 1).y) - dElevationSpring) / 2.;
				}

				long nSizeArr = dArrDistSpring.GetSize();
				BOOL bExistDist = FALSE;
				for(long kk = 0; kk < nSizeArr; kk++)
				{
					double dDist = dArrDistSpring.GetAt(kk);
					if(Compare(dDistSpring, dDist, _T("=")))
					{
						bExistDist = TRUE;
						break;
					}
				}
				if(bExistDist == FALSE)
				{
					dArrDistSpring.Add(dDistSpring);
				}
			}
			
			nSizeArr = dArrDistSpring.GetSize();
			if(nSizeArr == 0) continue;

			nSizeTable = bHoriSpring ? nSizeArr * 2 : nSizeArr;
			if(nTableIndex > 0)
			{
				XLText->AddText("$n");
			}
			++nTableIndex;

			XLText->GoPosition(NO_MOVE, 2);
			XLText->AddTable(nSizeTable, 23, 1, 0);
			for(long nCol = 0 ; nCol < 3; nCol++)
			{
				str = szTitle[nCol];
				if(nSizeCalc > 1 && nCol == 0)
				{
					str.Format(_T("토층 %d"), nCalc +1);
				}
				XLText->AddMergeFormatText(0, nWidthTitle[nCol]-1, "$c%s", str);
				XLText->AddFormatText("$m+%02d", nWidthTitle[nCol]);
			}

			KvBL = pMF->m_KvBL[nPos][nCase][nCalc];
			Ks   = pMF->m_Ks[nPos][nCase][nCalc];
			for(long ix = 0; ix < nSizeArr; ix++)
			{
				//szJointMatch = _T("");
				dDistSpring = dArrDistSpring.GetAt(ix);

				dSpringZ = Round(KvBL * dDistSpring * dApplyWidth, 3);
				dSpringS = Round(Ks * dDistSpring * dApplyWidth, 3);

				szJointMatch = _T("");
				CLongArray lArrJoint;
				long nSizeJoint = lArrSpringWall.GetSize();
				for(long kk = 0; kk < nSizeJoint; kk++)
				{
					long nIdx = lArrSpringWall.GetAt(kk);
					CJoint *pJnt = pFEM->GetJoint((unsigned short)nIdx);
					if(toM(pJnt->m_vPointReal.z) >= dElevationBoringTop) continue;
					if(toM(pJnt->m_vPointReal.z) < dElevationBoringBot) continue;

					if(bNLLInk == FALSE)
					{
						if(Compare(pJnt->m_vSpring.x, 0.0, _T("<="))) continue;
						if(Compare(pJnt->m_vSpring.x, dSpringZ, _T("=")))
						{
							lArrJoint.Add(pJnt->m_nIdx + 1);
						}
					}
					else
					{
						CNullLLink *pLink = pFEM->GetNLLinkByJointIndex((unsigned short)nIdx);
						if(pLink == NULL) continue;
						CNLProp *pProp = pFEM->GetNLPropByName(pLink->m_szNLProp);
						if(pProp == NULL) continue;
						if(Compare(pProp->m_vecData[0].m_dKE, 0.0, _T("<="))) continue;
						if(Compare(pProp->m_vecData[0].m_dKE, dSpringZ, _T("=")))
						{
							lArrJoint.Add(pJnt->m_nIdx + 1);
						}
					}
				}
				if(nTableIndex == 1)
				{
					if(bStt)
					{
						long nIdxSpring = m_xyArrSpringWall[iLEFT].GetSize() - 1;
						double dDistEnd = toM((m_xyArrSpringWall[iLEFT].GetAt(nIdxSpring - 1).y - m_xyArrSpringWall[iLEFT].GetAt(nIdxSpring).y) / 2.);
						if(Compare(dDistEnd ,dDistSpring, _T("=")))
							lArrJoint.Add(GetNumberJointSlabStt(pFEM, FALSE) + 1);
					}
					else
					{
						long nIdxSpring = m_xyArrSpringWall[iRIGHT].GetSize() - 1;
						double dDistEnd = toM((m_xyArrSpringWall[iRIGHT].GetAt(nIdxSpring - 1).y - m_xyArrSpringWall[iRIGHT].GetAt(nIdxSpring).y) / 2.);
						if(Compare(dDistEnd ,dDistSpring, _T("=")))
							lArrJoint.Add(GetNumberJointSlabEnd(pFEM, FALSE) + 1);
					}
				}
				if(nCalc == nSizeCalc - 1)
				{
					if(bStt)
					{
						long nIdxSpring = 0;
						double dDistEnd = toM((m_xyArrSpringWall[iLEFT].GetAt(nIdxSpring).y - m_xyArrSpringWall[iLEFT].GetAt(nIdxSpring + 1).y) / 2.);
						if(Compare(dDistEnd ,dDistSpring, _T("=")))
							lArrJoint.Add(GetNumberJointSlabStt(pFEM, TRUE) + 1);
					}
					else
					{
						long nIdxSpring = 0;
						double  dDistEnd = toM((m_xyArrSpringWall[iRIGHT].GetAt(nIdxSpring).y - m_xyArrSpringWall[iRIGHT].GetAt(nIdxSpring + 1).y) / 2.);
						if(Compare(dDistEnd ,dDistSpring, _T("=")))
							lArrJoint.Add(GetNumberJointSlabEnd(pFEM, TRUE) + 1);
					}
				}
				szJointMatch = GetStringApplyJoint(lArrJoint);

				for(long nS = 0; nS < 2; nS++)
				{
					if(nS == 1 && bHoriSpring == FALSE) break;

					XLText->AddText("$n");
					XLText->GoPosition(NO_MOVE, 2);
					XLText->AddMergeFormatText(0, nWidthTitle[0]-1,nS == 0 ? "$cKh%d" : "$cKss%d",ix + 1);
					XLText->AddFormatText("$m+%02d",nWidthTitle[0]);

					XLText->AddMergeFormatText(0, nWidthTitle[1]-1,"$c%s ⅹ %s x %s  = %s %s", 
						pData->GetStrDouble(dDistSpring, UNIT_CGS_M), pData->GetStrDouble(nS == 0 ? KvBL : Ks, UNIT_CGS_TONF_M2, FALSE, -1, FALSE, 11), 
						pData->GetStrDouble(dApplyWidth, UNIT_CGS_M), pData->GetStrDouble(nS == 0 ? dSpringZ : dSpringS, UNIT_CGS_TONF_M, FALSE, -1, FALSE, 11), pData->m_szTonf_M);
					XLText->AddFormatText("$m+%02d", nWidthTitle[1]);

					XLText->AddMergeFormatText(0,nWidthTitle[2]-1,"$c%s", szJointMatch);
					XLText->AddFormatText("$m+%02d",nWidthTitle[2]);
				}
			}
			XLText->AddText("$n");
		}
	}
}


void CARcBridgeDesignFrame::WriteModelingEQ(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long nIndexMiddle)
{
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	CFEMManage *pFEM = pBri->GetFemManage(COMBO_EARTHQUAKE);
	if(pFEM == NULL) return;

	long nIndexSmall = 1;
//	long nIndexTiny  = 1;
	XLText->GoPosition(NO_MOVE,COL_POS_1);
	XLText->AddFormatText("$h$e$a%d.%d 지진시(2D Frame)$n",nIndexTitle,nIndexMiddle);
	XLText->AddFormatText("$h %d)$r해석 모델링$n$n",nIndexSmall++);
	//////////////////////////////////////////////////////////////////////////
	// 그림
	long nSizeRow = 15;
	CDomyun *pDom = new CDomyun;
	pDom->SetCWnd(m_pStd->m_pView);
	pDom->SetScaleDim(100);
	m_pStd->m_pARoadOptionStd->SetEnvType(pDom,HCAD_DIML);
	pDom->SetTextHeight(pDom->GetTextHeight(FALSE)*1.5);
	m_pStd->DrawDiagramModeling(pDom,pFEM);
	m_pStd->DimModelingSlab(pDom,pFEM,TRUE,TRUE,TRUE,FALSE,TRUE);
	m_pStd->DimModelingSlab(pDom,pFEM,TRUE,TRUE,TRUE,FALSE,FALSE);
	m_pStd->DimModelingWall(pDom,pFEM,-1,TRUE,TRUE,TRUE);
	if(pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH) m_pStd->DimModelingArchRib(pDom,pFEM,TRUE,TRUE,TRUE,TRUE);
	m_pStd->DrawModelingSpringAndFix(pDom,pFEM);
	pDom->SetCalcExtRect();
	CDRect rect = pDom->GetExtRect();
	m_pStd->m_pARoadOptionStd->SetEnvType(pDom,HCAD_HIDD);
	pDom->Point(rect.left-pDom->Always(5),rect.bottom+pDom->Always(5));
	XLText->InsertDomyun(pDom,m_pStd->m_pView,nSizeRow,CXLFormatText::INSERT_DOM_CENTER | CXLFormatText::INSERT_DOM_MOVEROW);
	*XLText += 1;

	//////////////////////////////////////////////////////////////////////////
	XLText->AddFormatText("$s+14$h %d)$r절점 좌표$n",nIndexSmall++);
	MakeTableXZ(XLText, pBri, 1);			// nTyep : 1 = 지진시
	*XLText += 1;

	if(pBri->IsBoxType() == FALSE)
	{
		long nIndexTiny  = 1;
		int nCntRow = 1 + pBri->GetCountJijum();
		if(pBri->IsOutsideWall()==FALSE) nCntRow -= 2;
		XLText->GoPosition(NO_MOVE,COL_POS_1);
		XLText->AddFormatText("$s+%02d %d)$r스프링강성 : 『 6) SPRING 계수 산정 』참조$n",nCntRow+1,nIndexSmall++);
		if(pBri->GetTypeRigidFootingEQ()==2)
		{
			XLText->AddFormatText("$h$r%s$r기초 스프링강성 : 안전측으로하여 무한강성으로 한다.$n",GetNumberString(nIndexTiny++,NUM_CIRCLE));
			*XLText += 1;
		}
		else
		{
			XLText->AddFormatText("$h$r%s$r기초 스프링강성$n",GetNumberString(nIndexTiny++,NUM_CIRCLE));
			MakeTableSpringValueFooting(XLText, pBri);
		}
		XLText->GoPosition(NO_MOVE,COL_POS_1);
		if(pBri->IsOutsideWall() && pBri->GetTypeRigidFootingEQ()!=0)
		{
			nCntRow = 1 + GetCountSpringWall(pBri);
			XLText->AddFormatText("$s+%02d$h$r%s$r벽체 스프링강성$n",nCntRow+1,GetNumberString(nIndexTiny++,NUM_CIRCLE));
			MakeTableSpringValueWall(XLText, pBri);
		}
		*XLText += 1;
	}

	int nCntSlab = m_dArrThickSlab.GetSize();
	int nCntWall = m_dArrThickWall.GetSize();
	int nCntRow = nCntSlab + nCntWall +1;
	XLText->GoPosition(NO_MOVE,COL_POS_1);
	XLText->AddFormatText("$s+%02d$h %d)$r단면 특성$n",nCntRow+1,nIndexSmall++);
	MakeTableSectAffection(XLText, pBri, 1);

	XLText->AddFormatText("$s+04$h %d)$r강역구간 설정 : 평상시 모델링과 동일$n",nIndexSmall++);
	*XLText += 1;

	if(pBri->IsBoxType() == FALSE)
	{
		m_pStd->m_pLoadEQ->WriteCalcFootingSpring(XLText,pBri,nIndexSmall);
	}
}

void CARcBridgeDesignFrame::WriteModeling3DShell(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nIndexTitle, long nIndexMiddle)
{
	XLText->GoPosition(NO_MOVE,COL_POS_1);
	XLText->AddFormatText("$h$e$a%d.%d 3D Plate$n",nIndexTitle,nIndexMiddle);
}

// nType-> 0: 평상시(Normal), 1: 지진시(EQ)
void CARcBridgeDesignFrame::MakeTableXZ(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nType)
{
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	CFEMManage *pFEM;
	if(nType == 0) pFEM = &pBri->m_FemNormal;
	else if(nType == 1) pFEM = &pBri->m_FemEQ[0];

	int nCntRow = 10;
	int nCntCol = 2;

	int nCnt = pFEM->GetJointSize();

	if(nCnt <10) 
		nCntCol = 4*3;
	else 
		nCntCol = (long)(RoundUp(nCnt/(double)nCntRow, 0)*3); 

	if(nCntCol > 4*3) nCntCol = 4*3;

	nCntRow = (long)(RoundUp(nCnt/(nCntCol/(double)3), 0));

	XLText->AddTable(nCntRow, 24-1, 1, 0);
	//////////////////////////////////////////////////////////////////////////
	// Title
	int nWidthJoin = 1;
	int nWidth = 0;
	if(nCntCol/3 == 2) nWidthJoin = 4;
	else if(nCntCol/3 == 3) nWidthJoin = 3;
	else if(nCntCol/3 == 4) nWidthJoin = 2;

	char* Title[] = { "절점", "X", "Z" };

	int i = 0; for(i = 0; i<nCntCol; i++)
	{
		if( nCntCol/3 == 3 && i%3 == 0) nWidth = nWidthJoin-1;
		else nWidth = nWidthJoin;

		XLText->AddMergeFormatText(0,nWidth-1,"$c%s", Title[i%3]);
		XLText->AddFormatText("$m+0%d",nWidth);
	}
	XLText->AddText("$n$h$r");

	//////////////////////////////////////////////////////////////////////////
	// Data
	CJoint *pJoint;
	CVector cvXY;
	CString str = _T("");
	long nData = 0;

	for( i=1; i<=nCntRow; i++)
	{
		int j=0; for(j=0; j<nCntCol; j++)
		{
			if( nCntCol/3 == 3 && j%3 == 0) nWidth = nWidthJoin-1;
			else nWidth = nWidthJoin;

			nData = nCntRow*(j/3)+i;
			if(nData <= nCnt) pJoint = pFEM->GetJoint((USHORT)(nData-1));

			cvXY = pJoint->GetPoint();

			if(nData > nCnt) str = _T("");
			else if(j%3 == 0) str.Format("%d", nData);
			else if(j%3 == 1) str = pData->GetStrDouble(cvXY.x, UNIT_CGS_M, FALSE, -1, TRUE);
			else if(j%3 == 2) str = pData->GetStrDouble(cvXY.z, UNIT_CGS_M, FALSE, -1, TRUE);

			if(j%3 == 0) XLText->AddMergeFormatText(0,nWidth-1,"$b034$c%s", str);
			else XLText->AddMergeFormatText(0,nWidth-1,"$c%s", str);
			XLText->AddFormatText("$m+0%d",nWidth);
		}
		XLText->AddText("$n$h$r");
	}
}

void CARcBridgeDesignFrame::MakeTableSectAffection(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nType)
{
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	CFEMManage *pFEM = NULL;
	if(nType == 0) pFEM = &pBri->m_FemNormal;
	else if(nType == 1) pFEM = &pBri->m_FemEQ[0];

	BOOL bLsd = pData->IsLsdDesign();
	long nCntSlab = m_dArrThickSlab.GetSize();
	long nCntWall = m_dArrThickWall.GetSize();
	long nCntColu = m_dArrColumnArea.GetSize();
	long nCntArchRib = m_dArrThickArchRib.GetSize();
	long nCntRow  = nCntSlab + nCntWall + nCntColu + nCntArchRib + 1;
	long nCntCol  = 7;

	XLText->AddTable(nCntRow-1, 24-1, 1, 5);
	//////////////////////////////////////////////////////////////////////////
	// Title
	int nWidthJoin[] = { 5, 3, 3, 3, 3, 7 };

	char* Title[] = { "구 분", "B(m)", "H(m)", "A(m²)", "I(m⁴)", "적용절점" };
	if(bLsd)
	{
		Title[1] = _T("B(mm)");
		Title[2] = _T("H(mm)");
		Title[3] = _T("A(mm²)");
		Title[4] = _T("I(mm⁴)");
	}

	int i = 0; for(i = 0; i<nCntCol-1; i++)
	{
		XLText->AddMergeFormatText(0,nWidthJoin[i]-1,"$c%s", Title[i]);
		XLText->AddFormatText("$m+0%d",nWidthJoin[i]);
	}
	XLText->AddText("$n$h$r");

	//////////////////////////////////////////////////////////////////////////
	// Data
	CString str = _T("");
	CString strOpt = _T("");
	int nIndexSub = 0;
	int nWidthJoin1[] = { 3, 2, 3, 3, 3, 3, 7 };

	double dB = 0;
	double dH = 0;
	double dA = 0;
	double dI = 0;
	double dWidthSlab = (nType == 1) ? (bLsd? frM(m_pStd->GetSlabWidth()) : m_pStd->GetSlabWidth()) : 1.0;
	double dUnitM = bLsd ? frM(UNIT_METER) : UNIT_METER;
	long nSectTpye = 0;

	for( i=1; i<nCntRow; i++)
	{
		int j=0; for(j=0; j<nCntCol; j++)
		{
			if(i <= nCntSlab)
			{
				nIndexSub = i;
				dB = (nType==1) ? m_pStd->GetSlabWidth() : UNIT_METER;
				dH = m_dArrThickSlab.GetAt(nIndexSub-1);
				nSectTpye = 0;
				str = _T("슬래브");
			}
			else if(i <= (nCntSlab+nCntWall))
			{
				nIndexSub = i-nCntSlab;
				dB = (nType==1) ? m_pStd->GetSlabWidth() : UNIT_METER;
				dH = m_dArrThickWall.GetAt(nIndexSub-1);
				nSectTpye = 1;
				str = _T("벽 체");
			}
			else if(i <= (nCntSlab+nCntWall+nCntColu))
			{
				nIndexSub = i - nCntSlab - nCntWall;
				nSectTpye = 2;
				str = _T("기 둥");
			}
			else
			{
				nIndexSub = i - nCntSlab - nCntWall - nCntColu;
				dB = (nType==1) ? toM(pBri->GetWidthArchRib(0)) : UNIT_METER;
				dH = m_dArrThickArchRib.GetAt(nIndexSub-1);
				nSectTpye = 3;
				str = _T("아치리브");
			}

			if(bLsd)
			{
				dB = (nType==1)? frM(dB) : dB;
				dH = frM(dH);
			}

			dA = dB * dH;
			dI = dB * pow(dH, 3) / 12;
			if(nSectTpye==2)
			{
				dB = 0;
				dH = 0;
				dA = GetValueUnitChange(m_dArrColumnArea.GetAt(nIndexSub-1), UNIT_CGS_M2, pData->m_szM2) / dUnitM * dWidthSlab;
				dI = GetValueUnitChange(m_dArrColumnI33.GetAt(nIndexSub-1), UNIT_CGS_M4, pData->m_szM4)/ dUnitM * dWidthSlab;
			}

			if(j == 1) str.Format("%s", GetNumberString(nIndexSub, NUM_CIRCLE));
			if(j == 2) str = pData->GetStrDouble(dB, UNIT_CGS_M, FALSE, -1, TRUE);
			if(j == 3) str = pData->GetStrDouble(dH, UNIT_CGS_M, FALSE, -1, TRUE);
			if(j == 4) str = pData->GetStrDouble(dA, UNIT_CGS_M2, FALSE, -1, TRUE);
			if(j == 5) str = pData->GetStrDouble(dI, UNIT_CGS_M4, FALSE, -1, TRUE);
			if(j == 6) str = GetStringApplyJoint(pFEM, nSectTpye, nIndexSub);

			if((j==2 || j==3) && nSectTpye==2) str = _T("-");
			if(j==6 && nSectTpye==2) str = m_sArrColumnDes.GetAt(nIndexSub-1);

			if(i == 1 && j == 0) strOpt.Format("$md%d", nCntSlab-1);
			else if(i == nCntSlab+1 && j == 0 && nSectTpye==1) strOpt.Format("$md%d", nCntWall-1);
			else if(i == nCntSlab+nCntWall+1 && j == 0 && nSectTpye==2) strOpt.Format("$md%d", nCntColu-1);
			else if(i == nCntSlab+nCntWall+nCntColu+1 && j == 0 && nSectTpye==3) strOpt.Format("$md%d", nCntArchRib-1);
			else strOpt = _T("");

			XLText->AddMergeFormatText(0,nWidthJoin1[j]-1,"$c%s%s", str,strOpt);
			XLText->AddFormatText("$m+0%d",nWidthJoin1[j]);
		}
		XLText->AddText("$n$h$r");
	}
	*XLText += 1;
}

void CARcBridgeDesignFrame::MakeTableSpringValueFooting(CXLFormatText *XLText, CRcBridgeRebar *pBri)
{
	CFEMManage *pFEM = &pBri->m_FemEQ[0];
	CARcBridgeDataStd   *pData = pBri->m_pARcBridgeDataStd;

	char *Title[] = { "위치 / 절점","kδx","kδy","kδz","kθx","kθy","kθz"};
	int nWidthJoint1[] = {6, 3, 3, 3, 3, 3, 3};
	int nWidthJoint2[] = {3, 3, 3, 3, 3, 3, 3, 3};
	long nSizeRow = pBri->GetCountJijum() + 1;
	long nSizeCol = 8;
	if(pBri->IsOutsideWall()==FALSE) nSizeRow -= 2;

	XLText->GoPosition(NO_MOVE,COL_POS_2);
	XLText->AddTable(nSizeRow-1,24-1,1,2);
	long j=0; for(j=0; j<nSizeCol-1; j++)
	{
		XLText->AddMergeFormatText(0,nWidthJoint1[j]-1,"$c%s",Title[j]);
		XLText->AddFormatText("$m+0%d",nWidthJoint1[j]);
	}
	long nJijumStt = pBri->IsOutsideWall() ? 0 : 1;
	long i=0; for(i=1; i<nSizeRow; i++)
	{
		long nJijum = nJijumStt + i - 1;
		CString str = _T("");
		CRigidityFooting *pRigid = m_pStd->m_pLoadEQ->GetRigidityFootingByJijumNumber(nJijum);
		for(j=0; j<nSizeCol; j++)
		{
			if(j==0)
			{
				if(nJijum==0) str = "$e 시점기초";
				else if(nJijum==pBri->m_nQtyJigan) str = "$e 종점기초";
				else str.Format("$e 중간지점 %d",nJijum);
			}
			else if(j==1)
			{
				long nJoint = GetNumberJointJijumSpring(pFEM,nJijum);
				str.Format("%d",nJoint+1);
			}
			else if(pRigid)
			{
				double dCombK(0);
				if(j-2 < RIGIDITY_ROTATEX)
					dCombK = GetValueUnitChange(pRigid->m_CombK[j-2], UNIT_CGS_TONF_M, pData->m_szTonf_M);
				else
					dCombK = GetValueUnitChange(pRigid->m_CombK[j-2], UNIT_CGS_TONFM, pData->m_szTonfM);

				str = pData->GetStrDouble(dCombK, (j-2 < RIGIDITY_ROTATEX)? UNIT_CGS_TONF_M : UNIT_CGS_TONFM, FALSE, 0, FALSE, -1, FALSE);
				str.Format("%.0f",dCombK);
			}

			if(j==0)	XLText->AddText("$n$h");
			XLText->AddMergeFormatText(0,nWidthJoint2[j]-1,"$c%s",str);
			XLText->AddFormatText("$m+0%d",nWidthJoint2[j]);
		}
	}
	*XLText += 1;
}

void CARcBridgeDesignFrame::MakeTableSpringValueWall(CXLFormatText *XLText, CRcBridgeRebar *pBri)
{
	CARcBridgeDataStd   *pData = pBri->m_pARcBridgeDataStd;
	CFEMManage *pFEM = &pBri->m_FemEQ[0];

	char *Title[] = { "절점","산출식","kδx","비고"};
	int nWidthJoint[] = {4, 9, 5, 6};
	long nSizeRow = 0;
	long nSizeCol = 4;

	CString str = _T("");

	BOOL bSpringLeft  = (pBri->m_nSelectModelingCountEQ==1 || pBri->m_nSelectModelingCountEQ==2) ? TRUE : FALSE;
	BOOL bSpringRight = (pBri->m_nSelectModelingCountEQ==0 || pBri->m_nSelectModelingCountEQ==2) ? TRUE : FALSE;
	CStringArray sArrCalc;
	CDoubleArray dArrKsx;
	CDoubleArray dArrJoint;

	for(long nCalc=0; nCalc<2; nCalc++)
	{
		if(nCalc==0 && !bSpringLeft) continue;
		if(nCalc==1 && !bSpringRight) continue;

		long nJijum = (nCalc==0) ? 0 : pBri->m_nQtyJigan;
		CJoint *pJointSlab  = GetJointJijumOnSlab(pFEM,nJijum,TRUE);
		CJoint *pJointHunch = GetJointWallHunchEnd(pFEM,nJijum);
		CJoint *pJointSpring = (pBri->GetTypeRigidFootingEQ()==2) ? GetJointJijumFix(pFEM,nJijum) : GetJointJijumSpring(pFEM,nJijum);

		CRigidityFooting *pRigid = m_pStd->m_pLoadEQ->m_pListRighty.GetAt((nCalc==0) ? 0 : m_pStd->m_pLoadEQ->m_pListRighty.GetSize()-1);
		// (ARCBRIDGE-2663) 단위는 KN/m 가 맞지만 한계상태 일때 길이단위가 들어가기 때문에 1/1000이 되어야 한다. 강제로 KN/m2으로 단위변환한다.
		double KsBackFill = GetValueUnitChange(pRigid->m_KsBackFill,UNIT_CGS_TONF_M2,pData->m_szTonf_M2);

		// 모델링상단
		double Len1=0,Len2=0;
		double Ks = GetKsValueJointWall(pFEM,nJijum,pJointSlab->m_nIdx,Len1,Len2);
		Ks = GetValueUnitChange(Ks,UNIT_CGS_TONF_M,pData->m_szTonf_M);
		dArrJoint.Add(pJointSlab->m_nIdx);
		dArrKsx.Add(Ks);
		str.Format("%s x (%s + %s) / 2", pData->GetStrDouble(KsBackFill, UNIT_CGS_TONF_M)
			, pData->GetStrDoubleUnitChange(Len1, UNIT_CGS_M, pData->m_szM), pData->GetStrDoubleUnitChange(Len2, UNIT_CGS_M, pData->m_szM));
		sArrCalc.Add(str);

		long i=0; for(i=pJointHunch->m_nIdx; i<pJointSpring->m_nIdx; i++)
		{
			CJoint *pJnt = pFEM->GetJoint((USHORT)i);
			Ks = GetKsValueJointWall(pFEM,nJijum,i,Len1,Len2);
			Ks = GetValueUnitChange(Ks,UNIT_CGS_TONF_M,pData->m_szTonf_M);
			dArrJoint.Add(pJnt->m_nIdx);
			dArrKsx.Add(Ks);
			str.Format("%s x (%s + %s) / 2", pData->GetStrDouble(KsBackFill, UNIT_CGS_TONF_M)
				, pData->GetStrDoubleUnitChange(Len1, UNIT_CGS_M, pData->m_szM), pData->GetStrDoubleUnitChange(Len2, UNIT_CGS_M, pData->m_szM));
			sArrCalc.Add(str);
		}
	}
	nSizeRow = 1 + dArrKsx.GetSize();

	XLText->GoPosition(NO_MOVE,COL_POS_2);
	XLText->AddTable(nSizeRow-1,24-1,1,2);
	long j=0; for(j=0; j<nSizeCol; j++)
	{
		XLText->AddMergeFormatText(0,nWidthJoint[j]-1,"$c%s",Title[j]);
		XLText->AddFormatText("$m+0%d",nWidthJoint[j]);
	}

	long i=0; for(i=1; i<nSizeRow; i++)
	{
		long nIndex = i - 1;
		for(j=0; j<nSizeCol; j++)
		{
			if(j==0) str.Format("%.0f",dArrJoint.GetAt(nIndex)+1);
			if(j==1) str = sArrCalc.GetAt(nIndex);
			if(j==2) str.Format("%.0f",dArrKsx.GetAt(nIndex));
			if(j==3) str = _T("");

			if(j==0)	XLText->AddText("$n$h");
			XLText->AddMergeFormatText(0,nWidthJoint[j]-1,"$c%s",str);
			XLText->AddFormatText("$m+0%d",nWidthJoint[j]);
		}
	}
	*XLText += 1;
}

CString CARcBridgeDesignFrame::GetStringApplyJoint(CFEMManage *pFEM, long nType, long nNum)
{
	CJoint *pJoint;
	CLongArray lArrAppJoint;
	lArrAppJoint.RemoveAll();
	long nJointType = 0;
	BOOL bIncludeJoint = FALSE;

	long nCntJoint = pFEM->GetJointSize();
	long i=0; for(i=0; i<nCntJoint; i++)
	{
		pJoint = pFEM->GetJoint((USHORT)i);
		if(pJoint->m_BoolAttrib[BOOL_3_JOINT_COLUMN_END]) continue;
		nJointType = pJoint->m_LongAttrib[LONG_0_JOINT_TYPE];

		if(nType == 0) bIncludeJoint = (nJointType==JOINT_UPPERSLAB || nJointType==JOINT_LOWERSLAB);
		else if(nType == 1) bIncludeJoint = (nJointType == JOINT_OUTWALL || nJointType == JOINT_MIDWALL);
		else if(nType == 3) bIncludeJoint = (nJointType==JOINT_ARCHRIB);
		else bIncludeJoint = FALSE;

		if(bIncludeJoint && (nNum == pJoint->m_LongAttrib[LONG_2_JOINT_SECTION_NO]))
		{
			lArrAppJoint.Add(pJoint->m_nIdx+1);
		}
	}

	if(nType == 1)
	{
		nCntJoint = m_pStd->m_pBri->GetCountJijum();
		for(i=0; i<nCntJoint; i++)
		{
			double dThick  =  m_pStd->GetThickWallByDist(i, 0);
			long nSectNum = GetIndexSection(dThick, FALSE) + 1;

			if(nNum == nSectNum)
			{
				pJoint = GetJointJijumOnSlab(pFEM, i, TRUE);
				if(pJoint==NULL) continue;
				if(pJoint->m_BoolAttrib[BOOL_3_JOINT_COLUMN_END]) continue;
				lArrAppJoint.Add(pJoint->m_nIdx+1);
			}
		}
	}


	CString strRet = GetStringApplyJoint(lArrAppJoint);

	return strRet;
}

CString CARcBridgeDesignFrame::GetStringApplyJoint(CLongArray lArr)
{
	lArr.Sort();
	long nCntJoint = lArr.GetSize();

	CString strRet = _T("");
//	nTemp = 0;
	long nTemp2 = 0;
	long nTempOld = 0;

	CString strTemp = _T("");
	for(long i=0; i<nCntJoint; i++)
	{
		long nTemp = lArr.GetAt(i);
		if(i==0) strTemp.Format("%d", nTemp);
		else if(i==nCntJoint-1 && (strTemp == "~" || strTemp =="")) strTemp.Format("%d", nTemp);
		else if(i==nCntJoint-1) strTemp.Format(", %d", nTemp);
		else if(nTemp == nTempOld+1 && (strTemp == "~" || strTemp ==""))
		{
			nTemp2 = lArr.GetAt(i+1);
			if(nTemp2 == nTemp+1) strTemp = _T("");
			else strTemp.Format("%d", nTemp);
		}
		else if(nTemp == nTempOld+1)
		{
			nTemp2 = lArr.GetAt(i+1);
			if(nTemp2 == nTemp+1) strTemp = _T("~");
			else strTemp.Format(", %d", nTemp);
		}
		else strTemp.Format(", %d", nTemp);

		strRet += strTemp;
		nTempOld = nTemp;
	}

	return strRet;
}


void CARcBridgeDesignFrame::WriteRigidEnd(CXLFormatText *XLText, CRcBridgeRebar *pBri, long nType)
{
	long nIndexTiny  = 1;

	XLText->GoPosition(NO_MOVE,COL_POS_2);
	XLText->AddFormatText("$h기둥과 보의 절점부에 특히 큰 헌치가 있는 경우나 보부재 또는 기둥부재의 부재두께가 매우 큰 $n경우에는 강역의 영향을 고려한다. 『도로교설계기준 2008』4.12.3,(3)$n", "ⓐ");
	XLText->AddFormatText("$h부재가 그 축선에 대해 25˚이상 경사진 헌치를 갖는 경우에는 부재두께가 1.5배가 되는 점에서부터 $n절점까지로 한다. 다만, 헌치의 경사가 60˚이상의 경우는 헌치의 시점에서 부재두께의 1/4 안쪽 $n점에서부터 절점까지로 한다.『도로교설계기준 2008』4.12.3,(3)$n", "ⓑ");

	double dHH = 0;
	double dHW = 0;
	double dAngleSlab = 0;
	double dAngleWall = 0;
	BOOL bSlabBridge = (pBri->m_nTypeBridge == BRIDGE_TYPE_SLAB)? TRUE : FALSE;
	BOOL bPiRahmen	= (pBri->IsPiRahmen()) ? TRUE : FALSE;
	long nIndex = 0;

	CString str = _T("");

	for(long iUD = 0; iUD <= iLOWER; iUD++)
	{
		if(iUD == iLOWER && pBri->IsBoxType() == FALSE) break;

		BOOL bOneSect = FALSE;
		if(iUD == iUPPER && pBri->m_dWS == pBri->m_dWE && pBri->m_dHWS == pBri->m_dHWE && pBri->m_dHHS == pBri->m_dHHE)
			bOneSect = TRUE;
		if(iUD == iLOWER && pBri->m_dWS == pBri->m_dWE && pBri->m_dWSHW == pBri->m_dWEHW && pBri->m_dWSHH == pBri->m_dWEHH)
			bOneSect = TRUE;

		long nCnt = (bOneSect) ? 1 : 2;
		if(bSlabBridge || bPiRahmen) nCnt = 0;

		int i=0; for(i=0; i<nCnt; i++)
		{
			if(i==0 && bOneSect) str = "시종점 벽체";
			else if(i==0) str = "시점부 벽체";
			else if(i==1) str = "종점부 벽체";
			if(pBri->IsBoxType())
			{
				str += iUD == iUPPER ? _T(" : 상부슬래브") : _T(" : 하부슬래브");
			}
			XLText->AddFormatText("$n$h$s+12$h%s$r%s$n$h$r", GetNumberString(nIndexTiny++,NUM_CIRCLE), str);

			if(i==0) 
				nIndex = 0;
			else 
				nIndex = pBri->GetCountJijum()-1;
			CDomyun *pDom = new CDomyun;

			DrawRigidEnd(pDom, pBri, nIndex, iUD == iUPPER);
			XLText->InsertDomyun(pDom,m_pStd->m_pView,10,CXLFormatText::INSERT_DOM_CURPOS | CXLFormatText::INSERT_DOM_MOVEROW);
			XLText->PushPosition();

			if(i == 0)
			{
				dHH = iUD == iUPPER ? pBri->m_dHHS : pBri->m_dWSHH;
				dHW = iUD == iUPPER ? pBri->m_dHWS : pBri->m_dWSHW;
			}
			else
			{
				dHH = iUD == iUPPER ? pBri->m_dHHE : pBri->m_dWEHH;
				dHW = iUD == iUPPER ? pBri->m_dHWE : pBri->m_dWEHW;
			}

			XLText->AddFormatText("$u$u$u$u$u$u$u$t$t$r$r");
			if(dHH > 0 && dHW > 0)
			{
				dAngleSlab = ToDegree(atan(dHH/dHW));
				dAngleWall = 90 - dAngleSlab;
				str.Format("α = %.3f˚", dAngleSlab);
				XLText->AddFormatText("$r$e%s$n$l", str);
				str.Format("β = %.3f˚", dAngleWall);
				XLText->AddFormatText("$r$e%s$n", str);
			}
			else
				XLText->AddFormatText("$n$n");

			XLText->AddFormatText("$l$l$r$e  강역 계산결과$n");
			XLText->AddFormatText("$r$e슬래브 : %.3f$n$l", pBri->GetDistRigidEndSlab(nIndex, nIndex!=0, iUD == iUPPER));
			XLText->AddFormatText("$r$e벽  체 : %.3f$n$l", pBri->GetDistRigidEndWall(nIndex, iUD == iUPPER));
			XLText->AddFormatText("$n$n$n$h");
			XLText->PopPosition();
		}

		nCnt = (bSlabBridge) ? 0 : pBri->GetCountInWall();
		long nJijumConnect = pBri->GetJijumConnectRibAndSlab();
		BOOL bSame = FALSE;
		for(i=0; i<nCnt; i++)
		{
			if(i+1 == nJijumConnect) continue;
			if(i<0)
			{
				int j=0; for(j=0; j<i; j++)
				{
					if(pBri->m_dHLW[i][iUD]==pBri->m_dHLW[j][iUD] && pBri->m_dHRW[i][iUD]==pBri->m_dHRW[j][iUD])
					{
						bSame = TRUE;
						break;
					}
					else bSame = FALSE;
				}
			}

			if(!bSame)
			{
				str.Format("중간지점부 %d", i+1);
				if(pBri->IsBoxType())
				{
					str += iUD == iUPPER ? _T(" : 상부슬래브") : _T(" : 하부슬래브");
				}
				XLText->AddFormatText("$n$h$s+12$h%s$r%s$n$h$r", GetNumberString(nIndexTiny++,NUM_CIRCLE), str);

				nIndex = i+1;
				CDomyun *pDom = new CDomyun;

				if((pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP || pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP2))
					DrawRigidEndSlopeInWall(pDom, pBri, nIndex);
				else 
					DrawRigidEnd(pDom, pBri, nIndex, iUD == iUPPER);
				XLText->InsertDomyun(pDom,m_pStd->m_pView,10,CXLFormatText::INSERT_DOM_CURPOS | CXLFormatText::INSERT_DOM_MOVEROW);
				XLText->PushPosition();

				dHH = max(pBri->m_dHHL[i][iUD], pBri->m_dHHR[i][iUD]);	// modified by jaeho(2005.04.12)
				dHW = pBri->m_dHLW[i][iUD];
				dAngleSlab = pBri->GetAnglHunchWithSlabOrWall(nIndex, TRUE, TRUE, iUD==iUPPER);
				dAngleWall = pBri->GetAnglHunchWithSlabOrWall(nIndex, TRUE, FALSE, iUD==iUPPER);

				XLText->AddFormatText("$u$u$u$u$u$u$u$u$u$t$t$r$r");

				if(dHH>0 && dHW>0)
				{
					str.Format("α = %.3f˚", dAngleSlab);
					XLText->AddFormatText("$r$e  좌측$r$r%s$n$l", str);
					str.Format("β = %.3f˚", dAngleWall);
					XLText->AddFormatText("$r$e%s$n$l$l", str);
				}
				else *XLText += 2;

				dHW = pBri->m_dHRW[i][iUD];
				dAngleSlab = pBri->GetAnglHunchWithSlabOrWall(nIndex, FALSE, TRUE, iUD==iUPPER);
				dAngleWall = pBri->GetAnglHunchWithSlabOrWall(nIndex, FALSE, FALSE, iUD==iUPPER);
				if(dHH>0 && dHW>0)
				{
					str.Format("α = %.3f˚", dAngleSlab);
					XLText->AddFormatText("$l$r$e  우측$r$r%s$n$l", str);
					str.Format("β = %.3f˚", dAngleWall);
					XLText->AddFormatText("$r$e%s$n", str);
				}
				else *XLText += 2;

				XLText->AddFormatText("$l$l$l$r$e  강역 계산결과$n", str);
				XLText->AddFormatText("$r$e좌측 : %.3f$n$l", pBri->GetDistRigidEndSlab(nIndex, TRUE, iUD == iUPPER));
				XLText->AddFormatText("$r$e우측 : %.3f$n$l", pBri->GetDistRigidEndSlab(nIndex, FALSE, iUD == iUPPER));
				XLText->AddFormatText("$r$e벽체 : %.3f$n$l", pBri->GetDistRigidEndWall(nIndex, iUD == iUPPER));

				XLText->AddFormatText("$n$n$h");
				XLText->PopPosition();
			}
		}
	}

	if(pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
	{
		XLText->AddFormatText("$n$h$s+12$h%s$r%s$n", GetNumberString(nIndexTiny++,NUM_CIRCLE), "아치리브");
		XLText->AddFormatText("$h$r아치축에 대한 라멘구조의 영향은 아직 명확한 규명이 되어있지 않으므로 강역을 고려하여$n");
		XLText->AddFormatText("$h$r경제성을 확보하기 보다는 강역을 무시하여 아치리브에 가장 불리한 단면력을 유도하도록$n");
		XLText->AddFormatText("$h$r한다. (교량계획과 설계 P474 참조)$n");
	}
}

void CARcBridgeDesignFrame::DrawRigidEnd(CDomyun *pDomP, CRcBridgeRebar *pBri, long nIndex,BOOL bUpperSlab)
{
	CConcSymbol Sym(m_pStd->m_pARoadOptionStd);
	CDomyun Dom(pDomP);

	CString strMark = _T("");
	CDPoint xyStt(0, 0);
	CDPoint xyTemp(0, 0);
	CTwinVector tvHunch;

	double dD1 = 0;
	double dD2 = 0;
	double dD3 = 0;
	double dHH = 0;
	double dHW = 0;

	//	double dRateLS = m_pStd->m_pLoad->m_dReCalcRateSpan;
	double dRigidLenSlab = 0;
	double dRigidLenWall = 0;
	CDRect rc;

	Dom.SetScaleDim(100);
	Dom.SetLTScale(100);

	double ul = bUpperSlab ? 1.0 : -1.0;
	if(nIndex == 0)
	{
		dD1 = bUpperSlab ? pBri->m_dTS : pBri->m_dTS_Lower;
		dD2 = pBri->m_dWS;
		dHH = bUpperSlab ? pBri->m_dHHS : pBri->m_dWSHH;
		dHW = bUpperSlab ? pBri->m_dHWS : pBri->m_dWSHW;

		if(dHH == 0 || dHW == 0)
		{
			dHH = 0;
			dHW = 0;
		}
		dD3 = dD1 + dHH;

		double dLenX = dD2*3;
		double dLenY = dD1*3;

		// 중심선
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CENT);
		Dom.MoveTo(xyStt.x, xyStt.y);
		Dom.LineTo(xyStt.x+dLenX, xyStt.y);
		Dom.MoveTo(xyStt.x, xyStt.y);
		Dom.LineTo(xyStt.x, xyStt.y-dLenY*ul);

		// 외곽선
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
		xyTemp.x = xyStt.x + dLenX;
		xyTemp.y = xyStt.y + dD1/2 * ul;
		Dom.MoveTo(xyTemp.x, xyTemp.y);
		xyTemp.x -= (dLenX+dD2/2);
		Dom.LineTo(xyTemp.x, xyTemp.y);
		xyTemp.y -= (dLenY+dD1/2) * ul; 
		Dom.LineTo(xyTemp.x, xyTemp.y);
		xyTemp.x += dD2;
		Dom.MoveTo(xyTemp.x, xyTemp.y);
		xyTemp.y += (dLenY-dD1/2-dHH) * ul;
		tvHunch.m_v2 = xyTemp;
		Dom.LineTo(xyTemp.x, xyTemp.y);
		xyTemp.x += dHW;
		xyTemp.y += dHH * ul;
		tvHunch.m_v1 = xyTemp;
		Dom.LineTo(xyTemp.x, xyTemp.y);
		xyTemp.x += (dLenX-dD1/2-dHW);
		Dom.LineTo(xyTemp.x, xyTemp.y);

		// 강역
		dRigidLenSlab = pBri->GetDistRigidEndSlab(nIndex, FALSE, bUpperSlab);
		dRigidLenWall = pBri->GetDistRigidEndWall(nIndex, bUpperSlab);
		rc.left = xyStt.x;
		rc.top = xyStt.y + 20 * ul;
		rc.right = xyStt.x + dRigidLenSlab;
		rc.bottom = rc.top - 40 * ul;
		Dom.Rectangle(rc);

		rc.left = xyStt.x - 20;
		rc.top = xyStt.y;
		rc.right = rc.left + 40;
		rc.bottom = xyStt.y - dRigidLenWall * ul;
		Dom.Rectangle(rc);

		// Dim
		Dom.SetDimVLen(5);
		Dom.SetDimExo(2);

		double dAngleSlab = (dHH>0 && dHW>0)? ToDegree(atan(dHH/dHW)) : 0;
		double dAngleWall = 90 - dAngleSlab;

		strMark = _T("I = ∞");
		Dom.GiSiArrowExt(rc.left, rc.top, dD1/2+200, -500, bUpperSlab ? 3 : 5, strMark);

		xyTemp.x = xyStt.x-dD2/2;
		xyTemp.y = xyStt.y-dLenY*ul;
		Dom.SetDirection(bUpperSlab ? "BOTTOM" : "TOP");
		Dom.DimMoveTo(xyTemp.x, xyTemp.y, 0, "");
		strMark.Format("=%s", COMMA(dD2));
		Dom.DimLineToExtend(dD2, 0, "D₂", strMark);
		Dom.DimLineToExtend(dHW, 0, COMMA(dHW), "");

		xyTemp.x = xyStt.x+dLenX;
		xyTemp.y = tvHunch.m_v2.y;
		Dom.SetDirection("RIGHT");

		if(dAngleSlab > 0 && dAngleSlab < 25)
		{
			Dom.DimMoveTo(xyTemp.x, xyTemp.y, 0, "");
			strMark.Format("=%s", COMMA(dD3));
			Dom.DimLineToExtend(dD3*ul, 0, "D₃", strMark);
		}
		else
		{
			if(bUpperSlab)
			{
				Dom.DimMoveTo(xyTemp.x, xyTemp.y, 0, COMMA(dHH));
				Dom.DimMoveTo(xyTemp.x, xyTemp.y, 0);
				Dom.DimLineToExtend(dHH, 0, "", "", TRUE);
				strMark.Format("=%s", COMMA(dD1));
				Dom.DimLineToExtend(dD1, 0, "D₁", strMark);
			}
			else
			{
				Dom.DimMoveTo(xyTemp.x, xyTemp.y + (dHH + dD1)*ul, 0);
				strMark.Format("=%s", COMMA(dD1));
				Dom.DimLineToExtend(dD1, 0, "D₁", strMark);
				Dom.DimLineToOver(dHH, 0, COMMA(dHH));
			}

		}

		if(dAngleSlab > 60)
		{
			xyTemp = xyStt;
			Dom.SetDirection(bUpperSlab ? "TOP" : "BOTTOM");
			Dom.DimMoveTo(xyTemp.x, xyTemp.y+(dD1/2)*ul, 0, "");
			strMark.Format("=%s", COMMA(dRigidLenSlab));
			Dom.DimLineToOverExtend(dRigidLenSlab, 0, "0.25 D₁", strMark);
		}
		else if(dAngleSlab < 25)
		{
			xyTemp = xyStt;
			Dom.SetDirection(bUpperSlab ? "TOP" : "BOTTOM");
			Dom.DimMoveTo(xyTemp.x+dRigidLenSlab, xyTemp.y+(dD1/2)*ul, 0, "");
			strMark.Format("=%s", COMMA(dD2/2-dRigidLenSlab));
			Dom.DimLineToOverExtend(dD2/2-dRigidLenSlab, 0, "0.25 D₃", strMark);
		}
		else
		{
			Dom.SetDimObq(45);
			Dom.SetDimExo(0);			
			Dom.SetDirection("RIGHT");
			xyTemp.x = dRigidLenSlab;
			xyTemp.y = dD1/2*ul;
			Dom.DimMoveTo(xyTemp.x, xyTemp.y, 0, "");
			Dom.DimLineToExtend((-1.5*dD1)*ul, 0, "", "1.5 D₁");
			//Dom.SetDimObq(0);
			//Dom.SetDimExo(2);			
		}

		if(dAngleWall > 60)
		{
			xyTemp = xyStt;
			Dom.SetDirection("LEFT");
			Dom.DimMoveTo(xyTemp.x-dD2/2, -dRigidLenWall*ul, 0, "");
			strMark.Format("=%s", COMMA(dD1/2+dHH-dRigidLenWall));
			Dom.DimLineToOverExtend(-(dD1/2+dHH-dRigidLenWall)*ul, 0, "0.25 D₂", strMark);
		}
		else
		{
			Dom.SetDimObq(45);
			Dom.SetDimExo(0);			
			Dom.SetDirection("BOTTOM");
			xyTemp.x = -dD2/2;
			xyTemp.y = -dRigidLenWall * ul;
			Dom.DimMoveTo(xyTemp.x, xyTemp.y, 0, "");
			Dom.DimLineToExtend(1.5*dD2, 0, "", "1.5 D₂");
			Dom.SetDimObq(0);
			Dom.SetDimExo(2);
		}

		if(dHW > 0 && dHH >0)
		{
			xyTemp = tvHunch.m_v1;
			Dom.MoveTo(xyTemp.x, xyTemp.y);
			xyTemp.x -= 500;
			Dom.LineTo(xyTemp.x, xyTemp.y);
			xyTemp.y -= 150;
			Dom.TextOut(xyTemp.x, xyTemp.y, "α");

			xyTemp = tvHunch.m_v2;
			Dom.MoveTo(xyTemp.x, xyTemp.y);
			xyTemp.y += 300*ul;
			Dom.LineTo(xyTemp.x, xyTemp.y);
			xyTemp.x += 150;
			xyTemp.y -= 200*ul;
			Dom.TextOut(xyTemp.x, xyTemp.y, "β");
		}
	}
	else if(nIndex == pBri->GetCountJijum()-1)
	{
		dD1 = (bUpperSlab) ? pBri->m_dTS : pBri->m_dTS_Lower;
		dD2 = pBri->m_dWE;
		dHH = bUpperSlab ? pBri->m_dHHE : pBri->m_dWEHH;
		dHW = bUpperSlab ? pBri->m_dHWE : pBri->m_dWEHW;
		if(dHH == 0 || dHW == 0)
		{
			dHH = 0;
			dHW = 0;
		}
		dD3 = dD1 + dHH;

		double dLenX = dD2*3;
		double dLenY = dD1*3;

		// 중심선
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CENT);
		Dom.MoveTo(xyStt.x, xyStt.y);
		Dom.LineTo(xyStt.x-dLenX, xyStt.y);
		Dom.MoveTo(xyStt.x, xyStt.y);
		Dom.LineTo(xyStt.x, xyStt.y-dLenY * ul);

		// 외곽선
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);

		xyTemp.x = xyStt.x - dLenX;
		xyTemp.y = xyStt.y + dD1/2 * ul;
		Dom.MoveTo(xyTemp.x, xyTemp.y);
		xyTemp.x += (dLenX+dD2/2);
		Dom.LineTo(xyTemp.x, xyTemp.y);
		xyTemp.y -= (dLenY+dD1/2) * ul; 
		Dom.LineTo(xyTemp.x, xyTemp.y);
		xyTemp.x -= dD2;
		Dom.MoveTo(xyTemp.x, xyTemp.y);
		xyTemp.y += (dLenY-dD1/2-dHH) * ul;
		tvHunch.m_v2 = xyTemp;
		Dom.LineTo(xyTemp.x, xyTemp.y);
		xyTemp.x -= dHW;
		xyTemp.y += dHH * ul;
		tvHunch.m_v1 = xyTemp;
		Dom.LineTo(xyTemp.x, xyTemp.y);
		xyTemp.x -= (dLenX-dD1/2-dHW);
		Dom.LineTo(xyTemp.x, xyTemp.y);

		// 강역
		dRigidLenSlab = pBri->GetDistRigidEndSlab(nIndex, TRUE, bUpperSlab);
		dRigidLenWall = pBri->GetDistRigidEndWall(nIndex, bUpperSlab);
		rc.left = xyStt.x;
		rc.top = xyStt.y + 20 * ul;
		rc.right = xyStt.x - dRigidLenSlab;
		rc.bottom = rc.top - 40 * ul;
		Dom.Rectangle(rc);

		rc.left = xyStt.x - 20;
		rc.top = xyStt.y;
		rc.right = rc.left + 40;
		rc.bottom = xyStt.y - dRigidLenWall * ul;
		Dom.Rectangle(rc);

		// Dim
		Dom.SetDimVLen(5);
		Dom.SetDimExo(2);

		double dAngleSlab = (dHH>0 && dHW>0)? ToDegree(atan(dHH/dHW)) : 0;
		double dAngleWall = 90 - dAngleSlab;

		strMark = _T("I = ∞");
		Dom.GiSiArrowExt(rc.left, rc.top, dD1/2+100, 500, bUpperSlab ? 1 : 7, strMark);

		xyTemp.x = xyStt.x+dD2/2;
		xyTemp.y = xyStt.y-dLenY * ul;
		Dom.SetDirection(bUpperSlab ? "BOTTOM" : "TOP");
		Dom.DimMoveTo(xyTemp.x, xyTemp.y, 0, "");
		Dom.DimLineToExtend(-dD2, 0, "D₂", COMMA(dD2));
		Dom.DimLineToExtend(-dHW, 0, COMMA(dHW), "");

		xyTemp.x = xyStt.x-dLenX;
		xyTemp.y = tvHunch.m_v2.y;
		Dom.SetDirection("LEFT");
		if(dAngleSlab > 0 && dAngleSlab < 25)
		{
			Dom.DimMoveTo(xyTemp.x, xyTemp.y, 0, "");
			strMark.Format("=%s", COMMA(dD3));
			Dom.DimLineToExtend(dD3 * ul, 0, "D₃", strMark);
		}
		else
		{
			Dom.DimMoveTo(xyTemp.x, xyTemp.y, 0, "");
			Dom.DimLineToExtend(dHH * ul, 0, COMMA(dHH), "");
			Dom.DimLineToExtend(dD1 * ul, 0, "D₁", COMMA(dD1));
		}

		if(dAngleSlab > 60)
		{
			xyTemp = xyStt;
			Dom.SetDirection(bUpperSlab ? "TOP" : "BOTTOM");
			Dom.DimMoveTo(xyTemp.x, xyTemp.y+(dD1/2)*ul, 0, "");
			strMark.Format("=%s", COMMA(dRigidLenSlab));
			Dom.DimLineToOverExtend(dRigidLenSlab, 0, "0.25 D₁", strMark);
		}
		else if(dAngleSlab < 25)
		{
			xyTemp = xyStt;
			Dom.SetDirection(bUpperSlab ? "TOP" : "BOTTOM");
			Dom.DimMoveTo(xyTemp.x-dRigidLenSlab, xyTemp.y+(dD1/2)*ul, 0, "");
			strMark.Format("=%s", COMMA(dD2/2-dRigidLenSlab));
			Dom.DimLineToOverExtend(-dD2/2+dRigidLenSlab, 0, "0.25 D₃", strMark);
		}
		else
		{
			Dom.SetDimObq(45);
			Dom.SetDimExo(0);			
			Dom.SetDirection("LEFT");
			xyTemp.x = -dRigidLenSlab;
			xyTemp.y = (dD1/2) * ul;
			Dom.DimMoveTo(xyTemp.x, xyTemp.y, 0, "");
			Dom.DimLineToExtend(-1.5*dD1*ul, 0, "", "1.5 D₁");
			Dom.SetDimObq(0);
			Dom.SetDimExo(2);			
		}

//		if(dAngleWall > 25 || dAngleWall > 60)
		
		if(dAngleWall <= 25 || dAngleWall == 90)
		{
			xyTemp = xyStt;
			Dom.SetDirection("RIGHT");
			Dom.DimMoveTo(xyTemp.x+dD2/2, -dRigidLenWall*ul, 0, "");
			strMark.Format("=%s", COMMA(dD1/2+dHH-dRigidLenWall));
			Dom.DimLineToOverExtend(-(dD1/2+dHH-dRigidLenWall)*ul, 0, "0.25 D₂", strMark);
		}
		else
		{
			Dom.SetDimObq(45);
			Dom.SetDimExo(0);			
			Dom.SetDirection(bUpperSlab ? "BOTTOM" : "TOP");
			xyTemp.x = +dD2/2;
			xyTemp.y = -dRigidLenWall * ul;
			Dom.DimMoveTo(xyTemp.x, xyTemp.y, 0, "");
			Dom.DimLineToExtend(-1.5*dD2, 0, "", "1.5 D₂");
			Dom.SetDimObq(0);
			Dom.SetDimExo(2);
		}

		if(dHW > 0)
		{
			xyTemp = tvHunch.m_v1;
			Dom.MoveTo(xyTemp.x, xyTemp.y);
			xyTemp.x += 500;
			Dom.LineTo(xyTemp.x, xyTemp.y);
			xyTemp.x -= 100;
			xyTemp.y -= 150;
			Dom.TextOut(xyTemp.x, xyTemp.y, "α");

			xyTemp = tvHunch.m_v2;
			Dom.MoveTo(xyTemp.x, xyTemp.y);
			xyTemp.y += 300;
			Dom.LineTo(xyTemp.x, xyTemp.y);
			xyTemp.x -= 100;
			xyTemp.y -= 200;
			Dom.TextOut(xyTemp.x, xyTemp.y, "β");
		}
	}
	else
	{
		long nUD = bUpperSlab ? iUPPER : iLOWER;
		dD1 = bUpperSlab ? pBri->m_dTS : pBri->m_dTS_Lower;
		dHH = max(pBri->m_dHHL[nIndex-1][nUD], pBri->m_dHHR[nIndex-1][nUD]);	// modified by jaeho(2005.04.12)
		dD3 = dD1 + dHH;
		double dWL = pBri->GetThickWall(nIndex)/2;
		double dWR = pBri->GetThickWall(nIndex)/2;
		double dHLW = pBri->m_dHLW[nIndex-1][nUD];
		double dHRW = pBri->m_dHRW[nIndex-1][nUD];
		if(dHLW == 0 && dHRW == 0)
		{
			dHH = 0;
		}
		double dLenX = (dWL+dWR)*2;
		double dLenY = dD1*3;

		CTwinVector tvLH, tvRH;

		// 중심선
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CENT);
		Dom.MoveTo(xyStt.x-dLenX, xyStt.y);
		Dom.LineTo(xyStt.x+dLenX, xyStt.y);
		Dom.MoveTo(xyStt.x, xyStt.y);
		Dom.LineTo(xyStt.x, xyStt.y-dLenY*ul);

		// 외곽선
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);

		xyTemp.x = xyStt.x - dLenX;
		xyTemp.y = xyStt.y + dD1/2 * ul;
		Dom.MoveTo(xyTemp.x, xyTemp.y);
		xyTemp.x += (dLenX*2);
		Dom.LineTo(xyTemp.x, xyTemp.y);
		xyTemp.y -= (dD1) * ul;
		Dom.MoveTo(xyTemp.x, xyTemp.y);
		xyTemp.x -= (dLenX-(dHRW+dWR));
		tvRH.m_v1 = xyTemp;
		Dom.LineTo(xyTemp.x, xyTemp.y);
		xyTemp.x -= dHRW;
		xyTemp.y -= dHH * ul;
		tvRH.m_v2 = xyTemp;
		Dom.LineTo(xyTemp.x, xyTemp.y);
		xyTemp.y = xyStt.y-dLenY * ul;
		Dom.LineTo(xyTemp.x, xyTemp.y);
		xyTemp.x -= (dWR+dWL);
		Dom.MoveTo(xyTemp.x, xyTemp.y);
		xyTemp.y += (dLenY-(dD1/2+dHH)) * ul;
		tvLH.m_v2 = xyTemp;
		Dom.LineTo(xyTemp.x, xyTemp.y);
		xyTemp.x -= dHLW;
		xyTemp.y += dHH * ul;
		tvLH.m_v1 = xyTemp;
		Dom.LineTo(xyTemp.x, xyTemp.y);
		xyTemp.x -= (dLenX-dWL-dHLW);
		Dom.LineTo(xyTemp.x, xyTemp.y);

		// 강역
		double dRigidLenSlabR = pBri->GetDistRigidEndSlab(nIndex, FALSE, bUpperSlab);
		double dRigidLenSlabL = pBri->GetDistRigidEndSlab(nIndex, TRUE, bUpperSlab);
		dRigidLenWall = pBri->GetDistRigidEndWall(nIndex, bUpperSlab);

		rc.left = xyStt.x - 20;
		rc.top = xyStt.y;
		rc.right = rc.left + 40;
		rc.bottom = xyStt.y - dRigidLenWall * ul;
		Dom.Rectangle(rc);

		rc.left = xyStt.x - dRigidLenSlabL;
		rc.top = xyStt.y + 20 * ul;
		rc.right = xyStt.x + dRigidLenSlabR;
		rc.bottom = rc.top - 40 * ul;
		Dom.Rectangle(rc);

		// Dim
		Dom.SetDimVLen(5);
		Dom.SetDimExo(2);

		strMark = _T("I = ∞");
		Dom.GiSiArrowExt(rc.right, rc.top, dD1/2+100, 500, bUpperSlab ? 1 : 7, strMark);

		Dom.SetDirection(bUpperSlab ? "BOTTOM" : "TOP");
		xyTemp.x = tvLH.m_v1.x;
		xyTemp.y = xyStt.y - dLenY * ul;
		Dom.DimMoveTo(xyTemp.x, xyTemp.y, 0);
		Dom.DimLineTo(dHLW, 0, COMMA(dHLW));
		strMark.Format("=%s", COMMA(dWL+dWR));
		Dom.DimLineToExtend(dWL+dWR, 0, "D₂", strMark);
		Dom.DimLineTo(dHRW, 0, COMMA(dHRW));

		double dAngleSlab = (dHH>0 && dHLW>0)? ToDegree(atan(dHH/dHLW)) : 0;
		double dAngleWall = 90 - dAngleSlab;

		xyTemp.x = xyStt.x-dLenX;
		xyTemp.y = tvLH.m_v2.y;
		Dom.SetDirection("LEFT");
		if(dAngleSlab > 0 && dAngleSlab < 25)
		{
			Dom.DimMoveTo(xyTemp.x, xyTemp.y, 0, "");
			strMark.Format("=%s", COMMA(dD3));
			Dom.DimLineToExtend(dD3 * ul, 0, "D₃", strMark);
		}
		else
		{
			if(bUpperSlab)
			{
				Dom.DimMoveTo(xyTemp.x, xyTemp.y, 0, COMMA(dHH));
				Dom.DimLineToExtend(dHH, 0, "", "");
				strMark.Format("=%s", COMMA(dD1));
				Dom.DimLineToExtend(dD1, 0, "D₁", strMark);
			}
			else
			{
				Dom.DimMoveTo(xyTemp.x, xyTemp.y + (dHH + dD1)*ul, 0);
				strMark.Format("=%s", COMMA(dD1));
				Dom.DimLineToExtend(dD1, 0, "D₁", strMark);
				Dom.DimLineToOver(dHH, 0, COMMA(dHH));
			}
		}

		if(dHLW > 0)
		{
			xyTemp = tvLH.m_v1;
			Dom.MoveTo(xyTemp.x, xyTemp.y);
			xyTemp.x += 500;
			Dom.LineTo(xyTemp.x, xyTemp.y);
			xyTemp.y -= 150;
			Dom.TextOut(xyTemp.x, xyTemp.y, "α");

			xyTemp = tvLH.m_v2;
			Dom.MoveTo(xyTemp.x, xyTemp.y);
			xyTemp.y += 300;
			Dom.LineTo(xyTemp.x, xyTemp.y);
			xyTemp.x -= 100;
			xyTemp.y -= 200;
			Dom.TextOut(xyTemp.x, xyTemp.y, "β");

			xyTemp = tvRH.m_v1;
			Dom.MoveTo(xyTemp.x, xyTemp.y);
			xyTemp.x -= 500;
			Dom.LineTo(xyTemp.x, xyTemp.y);
			xyTemp.y -= 150;
			Dom.TextOut(xyTemp.x, xyTemp.y, "α");

			xyTemp = tvRH.m_v2;
			Dom.MoveTo(xyTemp.x, xyTemp.y);
			xyTemp.y += 300;
			Dom.LineTo(xyTemp.x, xyTemp.y);
			xyTemp.x += 100;
			xyTemp.y -= 200;
			Dom.TextOut(xyTemp.x, xyTemp.y, "β");
		}

		if(dAngleSlab == 0 || dAngleSlab > 60)
		{
			xyTemp = xyStt;
			Dom.SetDirection(bUpperSlab ? "TOP" : "BOTTOM");
			Dom.DimMoveTo(xyTemp.x-dWL, xyTemp.y+(dD1/2) * ul, 0, "0.25 D₁");
			Dom.DimLineTo(dWL-dRigidLenSlabL, 0, "");

			Dom.DimMoveTo(xyTemp.x+dRigidLenSlabR, xyTemp.y+(dD1/2) * ul, 0, "");
			strMark.Format("=%s", COMMA(dWR-dRigidLenSlabR));
			Dom.DimLineToOverExtend(dWR-dRigidLenSlabR, 0, "0.25 D₁", strMark);
		}
		else if(dAngleSlab <= 25)
		{
			xyTemp = xyStt;
			Dom.SetDirection(bUpperSlab ? "TOP" : "BOTTOM");
			Dom.DimMoveTo(xyTemp.x-dWL, xyTemp.y+(dD1/2)*ul, 0, "0.25 D₃");
			Dom.DimLineTo(dWL-dRigidLenSlabL, 0, "");

			Dom.DimMoveTo(xyTemp.x+dRigidLenSlabR, xyTemp.y+(dD1/2)*ul, 0, "");
			strMark.Format("=%s", COMMA(dWR-dRigidLenSlabR));
			Dom.DimLineToOverExtend(dWR-dRigidLenSlabR, 0, "0.25 D₃", strMark);
		}
		else
		{
			Dom.SetDimObq(45);
			Dom.SetDimExo(0);			
			Dom.SetDirection(bUpperSlab ? "TOP" : "BOTTOM");
			xyTemp.x = -dRigidLenSlabL;
			xyTemp.y = (dD1/2) * ul;
			Dom.DimMoveTo(xyTemp.x, xyTemp.y, 0, "");
			Dom.DimLineToExtend(1.5*dD1, 0, "", "1.5 D₁");
			Dom.SetDimObq(0);
			Dom.SetDimExo(2);			
		}

		if(dAngleWall > 60)
		{
			xyTemp = xyStt;
			Dom.SetDirection("RIGHT");
			Dom.DimMoveTo(xyStt.x+dLenX, -dRigidLenWall * ul, 0, "");
			strMark.Format("=%s", COMMA(dD1/2+dHH-dRigidLenWall));
			Dom.DimLineToOverExtend(-(dD1/2+dHH-dRigidLenWall) * ul, 0, "0.25 D₂", strMark, bUpperSlab ? 1 : 2);
		}
		else
		{
			Dom.SetDimObq(45);
			Dom.SetDimExo(0);			
			Dom.SetDirection("RIGHT");
			xyTemp = xyStt;
			xyTemp.x = +dWR;
			xyTemp.y = -dRigidLenWall * ul;
			Dom.DimMoveTo(xyTemp.x, xyTemp.y, 0, "");
			Dom.DimLineToExtend(-1.5*(dWL+dWR) * ul, 0, "", "1.5 D₂");
			Dom.SetDimObq(0);
			Dom.SetDimExo(2);
		}
	}
	*pDomP << Dom;
}

// nIndexJoint : Zero Base;
// 모델링상단과 헌치끝점사이에는 스프링을 두지않는다...
double CARcBridgeDesignFrame::GetKsValueJointWall(CFEMManage *pFEM, long nJijum, long nIndexJoint, double &dLenPrev, double &dLenNext)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	BOOL bLsd = pData->IsLsdDesign();
	long nIdxJijum = (nJijum==0) ? nJijum : m_pStd->m_pLoadEQ->m_pListRighty.GetSize()-1;
	CRigidityFooting *pRigid = m_pStd->m_pLoadEQ->m_pListRighty.GetAt(nIdxJijum);

	double Ks = pRigid->m_KsBackFill;

	CJoint *pJointSlab   = GetJointJijumOnSlab(pFEM,nJijum,TRUE);
	CJoint *pJointSpring = (pBri->GetTypeRigidFootingEQ()==2) ? GetJointJijumFix(pFEM,nJijum) : GetJointJijumSpring(pFEM,nJijum);
	CJoint *pJointHunch  = GetJointWallHunchEnd(pFEM,nJijum);

	double KsJoint  = 0;
	if(nIndexJoint!=pJointSlab->m_nIdx && nIndexJoint<pJointHunch->m_nIdx) return KsJoint;
	if(nIndexJoint==pJointSpring->m_nIdx) return KsJoint;

	if(nIndexJoint==pJointSlab->m_nIdx)
	{
		dLenPrev = toM(pBri->m_dTS);
		dLenNext = toM(pBri->m_dTS/2 + pBri->m_dHHS);
	}
	else if(nIndexJoint==pJointHunch->m_nIdx)
	{
		dLenPrev = toM(pBri->m_dTS/2 + pBri->m_dHHS);
		dLenNext = pFEM->GetLengthBetweenJoint(nIndexJoint,nIndexJoint+1);
		if(bLsd)
		{
			dLenNext = toM(dLenNext);
		}
	}
	else if(nIndexJoint==pJointSpring->m_nIdx-1)
	{
		dLenPrev = pFEM->GetLengthBetweenJoint(nIndexJoint,nIndexJoint-1);
		dLenNext = pFEM->GetLengthBetweenJoint(nIndexJoint,nIndexJoint+1)*2;
		if(bLsd)
		{
			dLenPrev = toM(dLenPrev);
			dLenNext = toM(dLenNext);
		}
	}
	else
	{
		dLenPrev = pFEM->GetLengthBetweenJoint(nIndexJoint,nIndexJoint-1);
		dLenNext = pFEM->GetLengthBetweenJoint(nIndexJoint,nIndexJoint+1);
		if(bLsd)
		{
			dLenPrev = toM(dLenPrev);
			dLenNext = toM(dLenNext);
		}
	}

	KsJoint = Ks * (dLenPrev+dLenNext)/2;

	return KsJoint;
}

double CARcBridgeDesignFrame::GetKrValueJointWall(CFEMManage *pFEM, long nJijum, long nIndexJoint, double &dLenPrev, double &dLenNext)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	BOOL bLsd = pData->IsLsdDesign();
	long nIdxJijum = (nJijum==0) ? nJijum : m_pStd->m_pLoadEQ->m_pListRighty.GetSize()-1;
	CRigidityFooting *pRigid = m_pStd->m_pLoadEQ->m_pListRighty.GetAt(nIdxJijum);

	double Kr = pRigid->m_KrBackFill;

	CJoint *pJointSlab   = GetJointJijumOnSlab(pFEM,nJijum,TRUE);
	CJoint *pJointSpring = (pBri->GetTypeRigidFootingEQ()==2) ? GetJointJijumFix(pFEM,nJijum) : GetJointJijumSpring(pFEM,nJijum);
	CJoint *pJointHunch  = GetJointWallHunchEnd(pFEM,nJijum);

	double KrJoint  = 0;
	if(nIndexJoint!=pJointSlab->m_nIdx && nIndexJoint<pJointHunch->m_nIdx) return KrJoint;
	if(nIndexJoint==pJointSpring->m_nIdx) return KrJoint;

	if(nIndexJoint==pJointSlab->m_nIdx)
	{
		dLenPrev = toM(pBri->m_dTS);
		dLenNext = toM(pBri->m_dTS/2 + pBri->m_dHHS);
	}
	else if(nIndexJoint==pJointHunch->m_nIdx)
	{
		dLenPrev = toM(pBri->m_dTS/2 + pBri->m_dHHS);
		dLenNext = pFEM->GetLengthBetweenJoint(nIndexJoint,nIndexJoint+1);
		if(bLsd)
		{
			dLenNext = toM(dLenNext);
		}
	}
	else if(nIndexJoint==pJointSpring->m_nIdx-1)
	{
		dLenPrev = pFEM->GetLengthBetweenJoint(nIndexJoint,nIndexJoint-1);
		dLenNext = pFEM->GetLengthBetweenJoint(nIndexJoint,nIndexJoint+1)*2;
		if(bLsd)
		{
			dLenPrev = toM(dLenPrev);
			dLenNext = toM(dLenNext);
		}
	}
	else
	{
		dLenPrev = pFEM->GetLengthBetweenJoint(nIndexJoint,nIndexJoint-1);
		dLenNext = pFEM->GetLengthBetweenJoint(nIndexJoint,nIndexJoint+1);
		if(bLsd)
		{
			dLenPrev = toM(dLenPrev);
			dLenNext = toM(dLenNext);
		}
	}

	KrJoint = Kr * (dLenPrev+dLenNext)/2;

	return KrJoint;
}

CJoint* CARcBridgeDesignFrame::GetJointJijumSpring(CFEMManage *pFEM, long nJijum)
{
	long nSizeJnt = pFEM->GetJointSize();

	long i=0; for(i=0; i<nSizeJnt; i++)
	{
		CJoint *pJnt = pFEM->GetJoint((USHORT)i);
		if(pJnt->m_BoolAttrib[BOOL_0_JOINT_SPRING] && 
			pJnt->m_LongAttrib[LONG_1_JOINT_JIJUMNO]==nJijum) return pJnt;	
	}
	return NULL;
}

CJoint* CARcBridgeDesignFrame::GetJointJijumFix(CFEMManage *pFEM, long nJijum)
{
//	CJoint *pJnt = NULL;

	long nSizeJnt = pFEM->GetJointSize();
	long nJijumIdx = nJijum;

	if(m_pStd->m_pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
	{
		switch(nJijum)
		{
		case 0: nJijumIdx = 0; break;
		case 1: nJijumIdx = m_pStd->m_pBri->m_nQtyJigan+1; break;
		case 2: nJijumIdx = m_pStd->m_pBri->m_nQtyJigan+2; break;
		case 3: nJijumIdx = m_pStd->m_pBri->m_nQtyJigan; break;
		}
	}

	long i=0; for(i=0; i<nSizeJnt; i++)
	{
		CJoint *pJnt = pFEM->GetJoint((USHORT)i);
		if(pJnt->m_BoolAttrib[BOOL_2_JOINT_JIJUM_FIX] && 
			pJnt->m_LongAttrib[LONG_1_JOINT_JIJUMNO]==nJijumIdx) return pJnt;	
	}
	return NULL;
}

long CARcBridgeDesignFrame::GetCountSpringWall(CRcBridgeRebar *pBri)
{
	long nQtySpring = 0;

	CARcBridgeDataStd   *pData = pBri->m_pARcBridgeDataStd;
	CFEMManage *pFEM = &pBri->m_FemEQ[0];

	BOOL bSpringLeft  = (pBri->m_nSelectModelingCountEQ==1 || pBri->m_nSelectModelingCountEQ==2) ? TRUE : FALSE;
	BOOL bSpringRight = (pBri->m_nSelectModelingCountEQ==0 || pBri->m_nSelectModelingCountEQ==2) ? TRUE : FALSE;

	for(long nCalc=0; nCalc<2; nCalc++)
	{
		if(nCalc==0 && !bSpringLeft) continue;
		if(nCalc==1 && !bSpringRight) continue;

		long nJijum = (nCalc==0) ? 0 : pBri->m_nQtyJigan;
		CJoint *pJointHunch = GetJointWallHunchEnd(pFEM,nJijum);
		CJoint *pJointSpring = (pBri->GetTypeRigidFootingEQ()==2) ? GetJointJijumFix(pFEM,nJijum) : GetJointJijumSpring(pFEM,nJijum);

		nQtySpring = 1 + (pJointSpring->m_nIdx - pJointHunch->m_nIdx);

	}

	return nQtySpring;
}

BOOL CARcBridgeDesignFrame::IsRestraintJoint(CJoint *pJoint)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;

	if(pBri->m_bUsePadShoe)
	{
		if(pJoint->m_vSpring.x > 0) return TRUE;
		if(pJoint->m_vSpring.y > 0) return TRUE;
		if(pJoint->m_vSpring.z > 0) return TRUE;
	}
	else
	{
		if(pJoint->m_UX==1) return TRUE;
		if(pJoint->m_UY==1) return TRUE;
		if(pJoint->m_UZ==1) return TRUE;
	}

	return FALSE;
}


// 각 지간별로 같은 비율을 적용하여 줄인다...
// 헌치부의 절점을 기준으로하여 줄임....
void CARcBridgeDesignFrame::ReCalculateCoordinateBySkew(CFEMManage *pFEM)
{
	if(Compare(m_pStd->m_pLoad->m_dReCalcRateSpan,1.00,"="))
		m_pStd->m_pLoad->CalculateSpanLength();

	if(m_pStd->m_pLoad->m_bReCalcSpanLength==FALSE) return;

	double dRateLS = m_pStd->m_pLoad->m_dReCalcRateSpan;

	long nSizeJoint = pFEM->GetJointSize();

	long i=0; for(i=0; i<nSizeJoint; i++)
	{
		CJoint *pJnt = pFEM->GetJoint((USHORT)i);

		pJnt->m_vPoint.x = pJnt->m_vPointOrigin.x*dRateLS;
	}
}

double CARcBridgeDesignFrame::GetShoeKvValue(long nJijum, BOOL bUnitMeter)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	double Kv = 0;

	CJijum *pJ = pBri->GetJijum(nJijum);

	long nVerPos  = (nJijum==0) ? 1 : 0;
	long nQtyShoe = pJ->m_nQtyShoe[nVerPos];

	long i=0; for(i=0; i<nQtyShoe; i++)
	{
		Kv += pJ->m_dKv[nVerPos][i];
	}

	double dWidthSlab = bUnitMeter ? m_pStd->GetSlabWidth() : UNIT_METER; // 1.000 UNIT_METER

	// bUnitMeter가 아닐때는 전폭에 대한 값이므로 단위가 tonf/m2이 아니라 tonf/m가 된다.
	return GetValueUnitChange(Kv/dWidthSlab, bUnitMeter ? UNIT_CGS_TONF_M2 : UNIT_CGS_TONF_M, bUnitMeter ? pData->m_szTonf_M2 : pData->m_szTonf_M);
} 

double CARcBridgeDesignFrame::GetShoeKhValue(long nJijum, BOOL bUnitMeter)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	double Kh = 0;

	CJijum *pJ = pBri->GetJijum(nJijum);

	long nVerPos  = (nJijum==0) ? 1 : 0;
	long nQtyShoe = pJ->m_nQtyShoe[nVerPos];

	long i=0; for(i=0; i<nQtyShoe; i++)
	{
		Kh += pJ->m_dKh[nVerPos][i];
	}

	double dWidthSlab = bUnitMeter ? m_pStd->GetSlabWidth() : UNIT_METER; // 1.000 UNIT_METER

	// bUnitMeter가 아닐때는 전폭에 대한 값이므로 단위가 tonf/m2이 아니라 tonf/m가 된다.
	return GetValueUnitChange(Kh/dWidthSlab, bUnitMeter ? UNIT_CGS_TONF_M2 : UNIT_CGS_TONF_M, bUnitMeter ? pData->m_szTonf_M2 : pData->m_szTonf_M);
}

/*
void CARcBridgeDesignFrame::MakeSapDataNLLinkEQ(CFEMManage *pFEM)
{
CRcBridgeRebar	  *pBri  = m_pStd->m_pBri;
CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

long nDOF=0,nOpen=0;

CString strName=_T("");
long nSizeJoint = pFEM->GetJointSize();
long i=0; for(i=0; i<nSizeJoint; i++)
{
CJoint *pJoint = pFEM->GetJoint((USHORT)i);
if(pJoint->m_BoolAttrib[BOOL_4_JOINT_SHOE_POS])
{
long nJijum = pJoint->m_LongAttrib[LONG_1_JOINT_JIJUMNO];
double Kv = GetShoeKvValue(nJijum, TRUE);
double Kh = GetShoeKhValue(nJijum, TRUE);
if(Compare(Kv,0.0,"=") && Compare(Kh,0.0,"=")) continue; // POT BEARING
// 탄성받침인 경우
strName.Format("NP%02d",nJijum+1);
pFEM->AddNLProp(strName,0,0,0,0,0,nDOF,Kv,0,0,nOpen);
}
}
}
*/

void CARcBridgeDesignFrame::DrawRigidEndSlopeInWall(CDomyun *pDomP, CRcBridgeRebar *pBri, long nIndex)
{
	CConcSymbol Sym(m_pStd->m_pARoadOptionStd);
	CDomyun Dom(pDomP);

	CString strMark = _T("");
	CDPoint xyStt(0, 0);
	CDPoint xyTemp(0, 0);
	CTwinVector tvHunch;

// 	double dD1 = 0;
// 	double dD3L = 0;
// 	double dD3R = 0;
	double dRateLS = m_pStd->m_pLoad->m_dReCalcRateSpan;
//	double dRigidLenWall = 0;
	CDRect rc;

	Dom.SetScaleDim(100);
	Dom.SetLTScale(100);

	if(/*pBri->m_dHLR[nIndex-1] == 0 && pBri->m_dHRR[nIndex-1] ==0*/1)
	{
		// 경사교각 일경우 경사 적용
		CWallApp *pWall = pBri->GetInWall(nIndex-1);
		double dAng = pWall->GetAngleWallByHor();
		CDPoint vAng = ToDPointFrDegree(dAng-180);

		double dD1 = pBri->m_dTS;

		double dWL = pBri->m_dWL[nIndex-1][iUPPER]/dRateLS;
		double dWR = pBri->m_dWR[nIndex-1][iUPPER]/dRateLS;
		double dHLW = (pBri->IsRoundHunch(nIndex, TRUE, TRUE)) ? pBri->GetCalcLenRHunch(nIndex, TRUE) : pBri->m_dHLW[nIndex-1][iUPPER];
		double dHRW = (pBri->IsRoundHunch(nIndex, FALSE, TRUE)) ? pBri->GetCalcLenRHunch(nIndex, FALSE) : pBri->m_dHRW[nIndex-1][iUPPER];
		double dHLH = (pBri->IsRoundHunch(nIndex, TRUE, TRUE)) ? pBri->GetCalcLenRHunch(nIndex, TRUE) : pBri->m_dHHL[nIndex-1][iUPPER];
		double dHRH = (pBri->IsRoundHunch(nIndex, FALSE, TRUE)) ? pBri->GetCalcLenRHunch(nIndex, FALSE) : pBri->m_dHHR[nIndex-1][iUPPER];
		double dD3L = dD1 + fabs(dHLH*vAng.y);
		double dD3R = dD1 + fabs(dHRH*vAng.y);

		double dLenX = (dWL+dWR)*3;
		double dLenY = dD1*4;

		CTwinVector tvLH, tvRH, tvCenSlab, tvCenWall, tvTemp1, tvTemp2;
		CDPoint xyRet(0, 0);

		// 중심선
		tvCenSlab.m_v1 = CDPoint(xyStt.x-dLenX, xyStt.y);
		tvCenSlab.m_v2 = CDPoint(xyStt.x+dLenX, xyStt.y);
		tvCenWall.m_v1 = xyStt;
		tvCenWall.m_v2 = CDPoint(xyStt.x-dLenY*(vAng.x/vAng.y), xyStt.y-dLenY);
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CENT);
		Dom.LineTo(tvCenSlab.m_v1, tvCenSlab.m_v2);
		Dom.LineTo(tvCenWall.m_v1, tvCenWall.m_v2);

		// 외곽선
		m_pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
		tvTemp1 = tvCenSlab;
		tvTemp2 = tvCenWall;
		Offset(tvTemp1.m_v1, tvTemp1.m_v2, -dD1/2);
		Dom.LineTo(tvTemp1.m_v1, tvTemp1.m_v2);
		// 왼쪽
		Offset(tvTemp1.m_v1, tvTemp1.m_v2, dD1);
		Offset(tvTemp2.m_v1, tvTemp2.m_v2, dWL);
		GetXyMatchSegAndSeg(tvTemp1.m_v1, tvTemp1.m_v2, tvTemp2.m_v1, tvTemp2.m_v2, xyRet);
		tvLH.m_v1 = CDPoint(xyRet.x-dHLW, xyRet.y);
		tvLH.m_v2 = xyRet + vAng * dHLH;
		Dom.LineTo(tvTemp1.m_v1, tvLH.m_v1);
		Dom.LineTo(tvLH.m_v1, tvLH.m_v2);
		Dom.LineTo(tvLH.m_v2, tvTemp2.m_v2);

		if(pBri->m_dHLR[nIndex-1][iUPPER] > 0)
		{
			CTwinVector tvArc;
			tvArc.m_v1 = xyRet + pBri->GetCalcLenRHunch(nIndex, TRUE, FALSE) * vAng;
			tvArc.m_v2 = xyRet + pBri->GetCalcLenRHunch(nIndex, TRUE, FALSE) * CDPoint(-1, 0);

			CRebarPlacing rb;
			CDPoint xyCen	= rb.GetXyCircleCenter(tvArc.m_v1, tvArc.m_v2, pBri->m_dHLR[nIndex-1][iUPPER], TRUE);
			//double dArcAng = (tvArc.m_v2 - xyCen).GetAngleDegree()-(tvArc.m_v1 - xyCen).GetAngleDegree();
			//			if(fabs(dArcAng) > 180) 
			//			{
			//				xyTemp = tvArc.m_v1;
			//				tvArc.m_v1 = tvArc.m_v2;
			//				tvArc.m_v2 = xyTemp;
			//			}
			double dAngStt	= (tvArc.m_v1-xyCen).GetAngleDegree();
			double dAngEnd	= (tvArc.m_v2-xyCen).GetAngleDegree();
			if(dAngEnd < dAngStt)
				dAngEnd+=360;
			Dom.Arc(xyCen, pBri->m_dHLR[nIndex-1][iUPPER], dAngStt, dAngEnd-dAngStt);
		}
		// 오른쪽
		tvTemp2 = tvCenWall;
		Offset(tvTemp2.m_v1, tvTemp2.m_v2, -dWR);
		GetXyMatchSegAndSeg(tvTemp1.m_v1, tvTemp1.m_v2, tvTemp2.m_v1, tvTemp2.m_v2, xyRet);
		tvRH.m_v1 = CDPoint(xyRet.x+dHRW, xyRet.y);
		tvRH.m_v2 = xyRet + vAng * dHRH;
		Dom.LineTo(tvTemp1.m_v2, tvRH.m_v1);
		Dom.LineTo(tvRH.m_v1, tvRH.m_v2);
		Dom.LineTo(tvRH.m_v2, tvTemp2.m_v2);
		if(pBri->m_dHRR[nIndex-1] > 0)
		{
			CTwinVector tvArc;
			tvArc.m_v1 = xyRet + pBri->GetCalcLenRHunch(nIndex, FALSE, FALSE) * vAng;
			tvArc.m_v2 = xyRet + pBri->GetCalcLenRHunch(nIndex, FALSE, FALSE) * CDPoint(1, 0);
			CRebarPlacing rb;
			CDPoint xyCen	= rb.GetXyCircleCenter(tvArc.m_v1, tvArc.m_v2, pBri->m_dHRR[nIndex-1][iUPPER], FALSE);
			Dom.Arc(xyCen, pBri->m_dHRR[nIndex-1][iUPPER], (tvArc.m_v1 - xyCen).GetAngleDegree(), (tvArc.m_v2 - xyCen).GetAngleDegree()-(tvArc.m_v1 - xyCen).GetAngleDegree());
		}

		// 강역
// 		BOOL bIsLeftWall = FALSE;
		double dRigidLenSlabR = pBri->GetDistRigidEndSlab(nIndex, FALSE, TRUE)*dRateLS;
		double dRigidLenSlabL = pBri->GetDistRigidEndSlab(nIndex, TRUE, TRUE)*dRateLS;
		double dRigidLenWall = pBri->GetDistRigidEndWall(nIndex, TRUE);
		BOOL bIsLeftWall = (dAng == 90)? FALSE : pBri->IsRigidEndLeftHunch(nIndex);

		tvTemp1.m_v1 = xyStt;
		tvTemp1.m_v2 = xyStt + dRigidLenWall * vAng;
		Dom.LineTo(tvTemp1.m_v1, tvTemp1.m_v2);
		Offset(tvTemp1.m_v1, tvTemp1.m_v2, -20);
		Dom.LineTo(tvTemp1.m_v1, tvTemp1.m_v2);
		Offset(tvTemp1.m_v1, tvTemp1.m_v2, 40);
		Dom.LineTo(tvTemp1.m_v1, tvTemp1.m_v2);

		rc.left = xyStt.x - dRigidLenSlabL;
		rc.top = xyStt.y + 20;
		rc.right = xyStt.x + dRigidLenSlabR;
		rc.bottom = rc.top - 40;
		Dom.Rectangle(rc);

		// Dim
		Dom.SetDimVLen(5);
		Dom.SetDimExo(2);

		strMark = _T("I = ∞");
		Dom.GiSiArrowExt(rc.right, rc.top, dD1/2+100, 500,  1, strMark);

		*pDomP << Dom;
		Dom.SetDirection("BOTTOM");
		tvTemp1 = tvCenWall;
		Offset(tvTemp1.m_v1, tvTemp1.m_v2, dWL);
		Dom.DimMoveTo(tvTemp1.m_v2.x, tvTemp1.m_v2.y, 0);
		strMark.Format("=%s", COMMA(dWL+dWR));
		Dom.DimLineToExtend(dWL+dWR, 0, "D₂", strMark);
		Dom.Rotate(tvTemp1.m_v2, vAng.Rotate90());
		*pDomP << Dom;

		double dAngleSlabL = pBri->GetAnglHunchWithSlabOrWall(nIndex, TRUE, TRUE);
		double dAngleSlabR = pBri->GetAnglHunchWithSlabOrWall(nIndex, FALSE, TRUE);
		double dAngleWall = pBri->GetAnglHunchWithSlabOrWall(nIndex, bIsLeftWall, FALSE);


		if(dAngleSlabL < 25)
		{
			xyTemp.x = xyStt.x-dLenX;
			xyTemp.y = tvLH.m_v2.y;

			Dom.SetDirection("LEFT");
			Dom.DimMoveTo(xyTemp.x, xyTemp.y, 0, "");
			strMark.Format("=%s", COMMA(dD3L));
			Dom.DimLineToExtend(dD3L, 0, "D₃", strMark);

			xyTemp.x = xyStt.x+dLenX;
			xyTemp.y = tvRH.m_v2.y;
			Dom.SetDirection("RIGHT");
			Dom.DimMoveTo(xyTemp.x, xyTemp.y, 0, "");
			strMark.Format("=%s", COMMA(dD3R));
			Dom.DimLineToExtend(dD3R, 0, "D₃", strMark);
		}
		else
		{
			xyTemp.x = xyStt.x-dLenX;
			xyTemp.y = tvLH.m_v2.y;

			Dom.SetDirection("LEFT");
			Dom.DimMoveTo(xyTemp.x, xyTemp.y, 0, COMMA(fabs(dHLH*vAng.y)));
			Dom.DimLineToExtend(fabs(dHLH*vAng.y), 0, "", "");
			strMark.Format("=%s", COMMA(dD1));
			Dom.DimLineToExtend(dD1, 0, "D₁", strMark);

			xyTemp.x = xyStt.x+dLenX;
			xyTemp.y = tvRH.m_v2.y;
			Dom.SetDirection("RIGHT");
			Dom.DimMoveTo(xyTemp.x, xyTemp.y, 0, COMMA(fabs(dHRH*vAng.y)));
			Dom.DimLineToExtend(fabs(dHRH*vAng.y), 0, "", "");
			strMark.Format("=%s", COMMA(dD1));
			Dom.DimLineToExtend(dD1, 0, "D₁", strMark);
		}

		if(dHLW > 0)
		{
			xyTemp = tvLH.m_v1;
			Dom.MoveTo(xyTemp.x, xyTemp.y);
			xyTemp.x += 500;
			Dom.LineTo(xyTemp.x, xyTemp.y);
			xyTemp.y -= 150;
			Dom.TextOut(xyTemp.x, xyTemp.y, "α");

			xyTemp = tvLH.m_v2;
			Dom.MoveTo(xyTemp.x, xyTemp.y);
			xyTemp -= 300 * vAng;
			Dom.LineTo(xyTemp.x, xyTemp.y);
			xyTemp.x -= 100;
			xyTemp.y -= 200;
			Dom.TextOut(xyTemp.x, xyTemp.y, "β");

			xyTemp = tvRH.m_v1;
			Dom.MoveTo(xyTemp.x, xyTemp.y);
			xyTemp.x -= 500;
			Dom.LineTo(xyTemp.x, xyTemp.y);
			xyTemp.y -= 150;
			Dom.TextOut(xyTemp.x, xyTemp.y, "α");

			xyTemp = tvRH.m_v2;
			Dom.MoveTo(xyTemp.x, xyTemp.y);
			xyTemp -= 300 * vAng;
			Dom.LineTo(xyTemp.x, xyTemp.y);
			xyTemp.x += 100;
			xyTemp.y -= 200;
			Dom.TextOut(xyTemp.x, xyTemp.y, "β");

		}

		// Left
		if(dAngleSlabL >= 60)
		{
			xyTemp = xyStt;
			Dom.SetDirection("TOP");
			Dom.DimMoveTo(xyTemp.x-dRigidLenSlabL, xyTemp.y+dD1/2, 0, "");
			strMark.Format("=%s", COMMA(0.25 * dD1));
			Dom.DimLineToOverExtend(-0.25 * dD1, 0, "0.25 D₁", strMark);
		}
		else if(dAngleSlabL < 25)
		{
			xyTemp = xyStt;
			Dom.SetDirection("TOP");
			Dom.DimMoveTo(xyTemp.x-dWL, xyTemp.y+pBri->m_dTS/2, 0, "0.25 D₃");
			Dom.DimLineTo(dWL-dRigidLenSlabL, 0, "");
		}
		else
		{
			xyTemp = xyStt;
			Dom.SetDimObq(45);
			Dom.SetDimExo(0);			
			Dom.SetDirection("LEFT");
			xyTemp.x = -dRigidLenSlabL;
			xyTemp.y = pBri->m_dTS/2;
			Dom.DimMoveTo(xyTemp.x, xyTemp.y, 0, "");
			Dom.DimLineToExtend(-1.5*dD1, 0, "", "1.5 D₁");
			Dom.SetDimObq(0);
			Dom.SetDimExo(2);			
		}
		// Right
		if(dAngleSlabR >= 60)
		{
			xyTemp = xyStt;
			Dom.SetDirection("TOP");
			Dom.DimMoveTo(xyTemp.x+dRigidLenSlabR, xyTemp.y+dD1/2, 0, "");
			strMark.Format("=%s", COMMA(0.25 * dD1));
			Dom.DimLineToOverExtend(0.25 * dD1, 0, "0.25 D₁", strMark);
		}
		else if(dAngleSlabR < 25)
		{
			xyTemp = xyStt;
			Dom.SetDirection("TOP");
			Dom.DimMoveTo(xyTemp.x+dRigidLenSlabR, xyTemp.y+dD1/2, 0, "");
			strMark.Format("=%s", COMMA(dWR-dRigidLenSlabR));
			Dom.DimLineToOverExtend(dWR-dRigidLenSlabR, 0, "0.25 D₃", strMark);
		}
		else
		{
			xyTemp = xyStt;
			Dom.SetDimObq(45);
			Dom.SetDimExo(0);			
			Dom.SetDirection("RIGHT");
			xyTemp.x = +dRigidLenSlabR;
			xyTemp.y = dD1/2;
			Dom.DimMoveTo(xyTemp.x, xyTemp.y, 0, "");
			Dom.DimLineToExtend(-1.5*dD1, 0, "", "1.5 D₁");
			Dom.SetDimObq(0);
			Dom.SetDimExo(2);			
		}

		if(dAngleWall >= 60)
		{
			*pDomP << Dom;
			double dWallTick = pBri->GetThickWall(nIndex);
			xyTemp = xyStt + dRigidLenWall*vAng;
			Dom.SetDimExo(10);
			Dom.SetDirection((dAng >90)? "LEFT" : "RIGHT");
			Dom.DimMoveTo(xyTemp.x, xyTemp.y, 0, "");
			strMark.Format("=%s", COMMA(0.25*dWallTick));
			Dom.DimLineToOverExtend(-(0.25*dWallTick), 0, "0.25 D₂", strMark);
			Dom.Rotate(xyTemp, vAng.Rotate90());
		}
		else if(dAngleWall < 25)
		{
			*pDomP << Dom;
			double dDistLen = bIsLeftWall? 0.25*dD3L : 0.25*dD3R;
			xyTemp = xyStt + dRigidLenWall*vAng;
			Dom.SetDirection((dAng >90)? "LEFT" : "RIGHT");
			Dom.DimMoveTo(xyTemp.x, xyTemp.y, 0, "0.25 D₃");
			Dom.DimLineTo(dDistLen, 0, "");
			Dom.Rotate(xyTemp, vAng.Rotate90());
		}
		else
		{
			*pDomP << Dom;
			long nDir = bIsLeftWall? 1 : -1;
			double dHalfWall = bIsLeftWall? dWR : dWL;

			Dom.SetDimObq(45);
			Dom.SetDimExo(0);			
			Dom.SetDirection("BOTTOM");
			xyTemp = xyStt + dRigidLenWall*vAng + nDir*dHalfWall*vAng.Rotate90();
			Dom.DimMoveTo(xyTemp.x, xyTemp.y, 0, "");
			Dom.DimLineToExtend(-1.5*(dWL+dWR)*nDir, 0, "", "1.5 D₂");
			Dom.SetDimObq(0);
			Dom.SetDimExo(2);
			Dom.Rotate(xyTemp, vAng.Rotate90());
		}
	}

	*pDomP << Dom;
}

// CJoint* CARcBridgeDesignFrame::GetJointMatchArchRib(CFEMManage *pFEM, long nJijum)
// {
// // 	CJoint *pJnt = NULL;
// 
// 	long nSizeJnt = pFEM->GetJointSize();
// 
// 	long i=0; for(i=0; i<nSizeJnt; i++)
// 	{
// 		CJoint *pJnt = pFEM->GetJoint((USHORT)i);
// 
// 		if(pJnt->m_nFlag == JOINT_ARCHRIB && pJnt->m_LongAttrib[LONG_1_JOINT_JIJUMNO]==nJijum)
// 		{
// 			return pJnt;
// 		}
// 	}
// 	return NULL;
// }

// 아치교일때 중간 수직재 하단 Element
CElement* CARcBridgeDesignFrame::GetElementWallMatchArchRib(CFEMManage *pFEM, long nJijum)
{
	POSITION pos = pFEM->m_ElementList.GetHeadPosition();
	while(pos)
	{
		CElement *pEle = static_cast<CElement*>(pFEM->m_ElementList.GetNext(pos));
		CJoint *pJntI = pFEM->GetJoint(pEle->m_nJStt);
		CJoint *pJntJ = pFEM->GetJoint(pEle->m_nJEnd);

		if(pJntI->m_LongAttrib[LONG_0_JOINT_TYPE] == JOINT_MIDWALL && pJntJ->m_LongAttrib[LONG_0_JOINT_TYPE] == JOINT_ARCHRIB)
		{
			if(pJntI->m_LongAttrib[LONG_1_JOINT_JIJUMNO]==nJijum && pJntJ->m_LongAttrib[LONG_1_JOINT_JIJUMNO]==nJijum)
				return pEle;
		}
	}

	return NULL;
}

BOOL CARcBridgeDesignFrame::IsAvailableJijumFix(CFEMManage *pFEM)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;

	if(pBri->IsBoxType()) return TRUE; // Spring, NLLink

	BOOL bArch	  = (pBri->m_nTypeBridge==BRIDGE_TYPE_ARCH) ? TRUE : FALSE;
	long nQtyJijum  = (bArch)? 4 :  pBri->GetCountJijum();

	if(pBri->m_nTypeBridge == BRIDGE_TYPE_SLAB && pBri->m_bUsePadShoe) return TRUE;

	long nJijum = 0; 
	for(nJijum = 0; nJijum<nQtyJijum; nJijum++)
	{
		if(pBri->m_bUsePadShoe && pBri->IsWall(nJijum) == FALSE)
		{
			continue;
		}

		long nIdxFix = GetNumberJointJijumFix(pFEM, nJijum);
		if(nIdxFix == -1) 
		{
			return FALSE;
		}
	}

	return TRUE;
}

void CARcBridgeDesignFrame::MakeSapDataNLProperty(CFEMManage *pFEM, BOOL bEarthQuake)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	if(pBri->IsBoxType() == FALSE) return;
	if(bEarthQuake == FALSE && pBri->m_n2DGeneralSpringType == 1) return;
	if(bEarthQuake == TRUE && pBri->m_n2DEQSpringType == 1) return;
	if(pBri->IsPileBase(-1) && bEarthQuake == FALSE) return; // 말뚝기초인 경우 저판은 스프링으로 처리

	long nCase = bEarthQuake ? MODULUS_FOUNDATION_EQ : MODULUS_FOUNDATION_NORMAL;
	long nDOF = 0, nOpen = 0, nType = 0;
	double dM = 0.00001, dMR1 = 0.00001, dMR2 = 0.00001, dMR3 = 0.00001, dCE = 0.9999;

	double dKE = 0, dK = 0;

	// 하부슬래브
	CString strName=_T("LS");
	long nSize = m_vArrForceLower[nCase].GetSize();
	CDPointArray xyArrProp;
	CDPoint xyProp(0,0);
	for(long ix = 0; ix < nSize; ix++)
	{
		xyProp.x = m_vArrForceLower[nCase].GetAt(ix).x;
		xyProp.y = m_vArrForceLower[nCase].GetAt(ix).z;
		if(xyArrProp.GetIndex(xyProp) == -1)
		{
			xyArrProp.Add(xyProp);
		}
	}

	long nSizeProp = xyArrProp.GetSize();
	for(long ix = 0; ix < nSizeProp; ix++)
	{
		dKE = xyArrProp.GetAt(ix).y;
		dK  = dKE;
		strName.Format(_T("LS%02d"), ix + 1);
		pFEM->AddNLProp(strName, nType, dM, dMR1, dMR2, dMR3, nDOF, dKE, dCE, dK, nOpen);
	}

	// 벽체
	if(bEarthQuake == FALSE) return;

	for(long iLR = iLEFT; iLR <= iRIGHT; iLR++)
	{
		nSize = m_vArrForceWall[iLR].GetSize();
		CDPointArray xyArrProp;
		CDPoint xyProp(0,0);
		for(long ix = 0; ix < nSize; ix++)
		{
			xyProp.x = m_vArrForceWall[iLR].GetAt(ix).x;
			xyProp.y = m_vArrForceWall[iLR].GetAt(ix).z;
			if(xyArrProp.GetIndex(xyProp) == -1)
			{
				xyArrProp.Add(xyProp);
			}
		}

		long nSizeProp = xyArrProp.GetSize();
		for(long ix = 0; ix < nSizeProp; ix++)
		{
			dKE = xyArrProp.GetAt(ix).x;
			dK  = dKE;
			strName.Format(_T("%s%02d"), iLR == iLEFT ? _T("LL") : _T("LR"), ix + 1);
			pFEM->AddNLProp(strName, nType, dM, dMR1, dMR2, dMR3, nDOF, dKE, dCE, dK, nOpen);
		}
	}
}

void CARcBridgeDesignFrame::MakeSapDataMode(CFEMManage *pFEM, BOOL bEarthQuake)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	if(pBri->IsBoxType() == FALSE) return;
	if(bEarthQuake == FALSE && pBri->m_n2DGeneralSpringType == 1) return;
	if(bEarthQuake == TRUE && pBri->m_n2DEQSpringType == 1) return;

	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	// MODE
	long nTypeMode  = 0; // RITZ
	long nCountMode = 100;

	pFEM->m_arrMode.RemoveAll();
	pFEM->AddMode(nTypeMode, nCountMode, 0, 0, 0, _T("*"));
	if(bEarthQuake == FALSE)
	{
		for(long nLoad = 0; nLoad < BRIDGE_LOAD_BASIC; nLoad++)
		{
			if(pBri->m_bApplyLoad[nLoad] == FALSE) continue;
			if(nLoad == BRIDGE_LOAD_LIVE_FULL)  continue;
			if(nLoad == BRIDGE_LOAD_LIVE_ECC)  continue;
			if(nLoad == BRIDGE_LOAD_EQ) continue;
			if(nLoad == BRIDGE_LOAD_TANK) continue;
			if(nLoad == BRIDGE_LOAD_SOIL && m_pStd->m_pLoad->m_dSoilPressureVert <= 0) continue;
			if(nLoad == BRIDGE_LOAD_TOAP_ONE)
			{
				if(pBri->m_bSelectEnvToapOne)
				{
					continue;
				}
				else
				{
					CString sLoad = pData->GetStringLoadCase(nLoad, FALSE);
					pFEM->m_arrMode.Add(sLoad + _T("L"));
					pFEM->m_arrMode.Add(sLoad + _T("R"));
					continue;
				}
			}
			if(pBri->m_bApplyGroundWaterLevel == FALSE && (nLoad == BRIDGE_LOAD_WATERHORI || nLoad == BRIDGE_LOAD_WATERVERT))
				continue;

			pFEM->m_arrMode.Add(pData->GetStringLoadCase(nLoad, FALSE));
		}
		if(m_pStd->m_pLoad->m_dLiveLoadFootway > 0)
		{
			pFEM->m_arrMode.Add(_T("FW"));
		}
		if(pData->IsLsdDesign() == FALSE && pBri->IsBoxType() && pBri->m_dHeightInteriorWaterPress > 0)
		{
			pFEM->m_arrMode.Add(_T("IW"));
		}
	}
	else
	{
		CString strLoad = _T("");
		if(pBri->m_bApplyLoad[BRIDGE_LOAD_DEAD])
		{
			pFEM->m_arrMode.Add(pData->GetStringLoadCase(BRIDGE_LOAD_DEAD, TRUE));
			if(pData->IsLsdDesign() == FALSE && pBri->IsBoxType() && pBri->m_dHeightInteriorWaterPress > 0)
			{
				pFEM->m_arrMode.Add(_T("IW"));
			}
		}
		if(pBri->m_bApplyLoad[BRIDGE_LOAD_EQ])
		{
			strLoad.Format(_T("%s%s"), pData->GetStringLoadCase(EARTHQUAKE_LOAD_INERTIAL, TRUE), _T("L"));
			pFEM->m_arrMode.Add(strLoad);
			strLoad.Format(_T("%s%s"), pData->GetStringLoadCase(EARTHQUAKE_LOAD_INERTIAL, TRUE), _T("R"));
			pFEM->m_arrMode.Add(strLoad);
		}
		if(pBri->m_bApplyLoad[BRIDGE_LOAD_TOAP_BOTH] || (pBri->IsBoxType() && pBri->m_bApplyLoad[BRIDGE_LOAD_TOAP_ONE]))
		{
			strLoad.Format(_T("%s%s"), pData->GetStringLoadCase(EARTHQUAKE_LOAD_TOAP, TRUE), _T("L"));
			pFEM->m_arrMode.Add(strLoad);
			strLoad.Format(_T("%s%s"), pData->GetStringLoadCase(EARTHQUAKE_LOAD_TOAP, TRUE), _T("R"));
			pFEM->m_arrMode.Add(strLoad);
		}
		if(pBri->m_bApplyLoad[BRIDGE_LOAD_WATERVERT])
		{
			pFEM->m_arrMode.Add(pData->GetStringLoadCase(BRIDGE_LOAD_WATERVERT, FALSE));
		}
		if(pBri->m_bApplyLoad[BRIDGE_LOAD_WATERHORI])
		{
			pFEM->m_arrMode.Add(pData->GetStringLoadCase(BRIDGE_LOAD_WATERHORI, FALSE));
		}
	}

	if(pBri->m_nTypeLower == BOXFOOTING_TYPE_ANCHOR && pBri->m_bApplyAnchorForce)
	{
		pFEM->m_arrMode.Add(_T("AN"));
	}
}

void CARcBridgeDesignFrame::MakeSapDataHistoryEarthquake(CFEMManage *pFEM)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	if(pBri->IsBoxType() == FALSE) return;
	if(pBri->m_n2DEQSpringType == 1) return;

	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	CARcBridgeCalcLoad *pLoad = m_pStd->m_pLoad;

	m_bHistory[1] = TRUE;

	CString strHist = _T(""),strLoad = _T("");
	long nTypeHistory = 0; // LOAD
	long nStep = 100;

	CMatrix MatCombo = pBri->m_MatComboEQ;
	long nSizeCombo  = pBri->m_MatComboEQ.GetRow();
	long nIndexCombo = 0;
	for(long nLR=iLEFT; nLR<=iRIGHT; nLR++)
	{
		for(long ix=0; ix<nSizeCombo; ix++)
		{
			DWORD dWCombo = pBri->GetLoadCombo(ix,LOAD_COMBINE_EQ);
			strHist.Format("HEQ%02d",nIndexCombo+1);

			pFEM->AddHistory(strHist, _T(""), nTypeHistory, nStep, 1.0, 0.99, TRUE);
			if(dWCombo&LOAD_CASE_DEAD)
			{
				strLoad = pData->GetStringLoadCase(BRIDGE_LOAD_DEAD, TRUE);
				pFEM->AddHistory_Load(strHist, strLoad, _T(""), MatCombo(ix,BRIDGE_LOAD_DEAD));
				if(pData->IsLsdDesign() == FALSE && pBri->IsBoxType() && pBri->m_dHeightInteriorWaterPress > 0)
				{
					strLoad = _T("IW");
					pFEM->AddHistory_Load(strHist, strLoad, _T(""), MatCombo(ix,BRIDGE_LOAD_DEAD));
				}
			}
			if(dWCombo&LOAD_CASE_EQ)
			{
				strLoad.Format(_T("%s%s"), pData->GetStringLoadCase(EARTHQUAKE_LOAD_INERTIAL, TRUE), nLR==iLEFT ? _T("L") : _T("R"));
				pFEM->AddHistory_Load(strHist, strLoad, _T(""), MatCombo(ix,BRIDGE_LOAD_EQ));
			}
			if(dWCombo&LOAD_CASE_LIVE_FULL && pBri->m_bApplyLiveLoadEQ && pLoad->m_dLiveLoadFootway>0)
			{
				pFEM->AddHistory_Load(strHist, _T("FW"), _T(""), MatCombo(ix,BRIDGE_LOAD_LIVE_FULL));
			}
			if(dWCombo&LOAD_CASE_TOAP_BOTH)
			{
				strLoad.Format(_T("%s%s"), pData->GetStringLoadCase(EARTHQUAKE_LOAD_TOAP, TRUE), nLR==iLEFT ? _T("L") : _T("R"));
				pFEM->AddHistory_Load(strHist, strLoad, _T(""), MatCombo(ix,BRIDGE_LOAD_TOAP_BOTH));
			}
			if(dWCombo&LOAD_CASE_WATERHORI)
			{
				strLoad = pData->GetStringLoadCase(BRIDGE_LOAD_WATERHORI);
				pFEM->AddHistory_Load(strHist, strLoad, _T(""), MatCombo(ix,BRIDGE_LOAD_WATERHORI));
			}
			if(dWCombo&LOAD_CASE_WATERVERT)
			{
				strLoad = pData->GetStringLoadCase(BRIDGE_LOAD_WATERVERT);
				pFEM->AddHistory_Load(strHist, strLoad, _T(""), MatCombo(ix,BRIDGE_LOAD_WATERVERT));
			}
			if(pBri->m_nTypeLower == BOXFOOTING_TYPE_ANCHOR && pBri->m_bApplyAnchorForce)
			{
				strLoad = _T("AN");
				pFEM->AddHistory_Load(strHist, strLoad, _T(""), pBri->m_dFactorAnchor[COMBO_EARTHQUAKE][ix]);
			}
			nIndexCombo++;
		}
	}
}

void CARcBridgeDesignFrame::MakeSapDataHistoryNormal(CFEMManage *pFEM)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	if(pBri->IsBoxType() == FALSE) return;
	if(pBri->m_n2DGeneralSpringType == 1) return;

	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	CARcBridgeCalcLoad *pLoad = m_pStd->m_pLoad;

	m_bHistory[0] = TRUE;
	BOOL bEnvToapOne = pBri->m_bSelectEnvToapOne;

	long nSizeComboUse = pBri->m_MatComboUse.GetRow();
	long nSizeComboUlt = pBri->m_MatComboUlt.GetRow();
	long nSizeComboEcc = pBri->m_MatComboEcc.GetRow();

	CString str = _T(""),strLoad = _T("");
	long nTypeHistory = 0; // LOAD
	long nStep = 100;
	CString szLR = _T("");

	for(long nComboCase=0; nComboCase<3; nComboCase++)
	{
		long nSizeCombo  = nSizeComboUse;
		CMatrix MatCombo = pBri->m_MatComboUse;
		if(nComboCase==1) { nSizeCombo = nSizeComboUlt;  MatCombo = pBri->m_MatComboUlt;}
		if(nComboCase==2) { nSizeCombo = nSizeComboEcc;  MatCombo = pBri->m_MatComboEcc;}

		for(long nComboToapOne = 0; nComboToapOne < 2; nComboToapOne++)
		{
			if(bEnvToapOne && nComboToapOne == 1) break;

			szLR = (nComboToapOne > 0) ? _T("R") : _T("");

			for(long ix=0; ix<nSizeCombo; ix++)
			{
				DWORD dWCombo = pBri->GetLoadCombo(ix, nComboCase);
				if(nComboToapOne > 0 && (dWCombo&LOAD_CASE_TOAP_ONE)==FALSE) continue;

				str.Format("HSV%02d%s",ix + 1, szLR);
				if(nComboCase==1) str.Format("HUL%02d%s",ix + 1, szLR);
				if(nComboCase==2)	str.Format("HUE%02d%s",ix + 1, szLR);

				pFEM->AddHistory(str, _T(""), nTypeHistory, nStep, 1.0, 0.99, TRUE);

				if(dWCombo&LOAD_CASE_DEAD)
				{
					strLoad = pData->GetStringLoadCase(BRIDGE_LOAD_DEAD);
					pFEM->AddHistory_Load(str, strLoad, _T(""), MatCombo(ix,BRIDGE_LOAD_DEAD));
					if(pData->IsLsdDesign() == FALSE && pBri->IsBoxType() && pBri->m_dHeightInteriorWaterPress > 0)
					{
						strLoad = _T("IW");
						pFEM->AddHistory_Load(str, strLoad, _T(""), MatCombo(ix,BRIDGE_LOAD_DEAD));
					}
				}
				if(dWCombo&LOAD_CASE_LIVE_FULL && pLoad->m_dLiveLoadFootway>0)
				{
					pFEM->AddHistory_Load(str, _T("FW"), _T(""), MatCombo(ix,BRIDGE_LOAD_LIVE_FULL));
				}
				if(dWCombo&LOAD_CASE_TOAP_BOTH)
				{
					strLoad = pData->GetStringLoadCase(BRIDGE_LOAD_TOAP_BOTH);
					pFEM->AddHistory_Load(str, strLoad, _T(""), MatCombo(ix,BRIDGE_LOAD_TOAP_BOTH));
				}
				if(dWCombo&LOAD_CASE_TOAP_ONE && !bEnvToapOne)
				{
					strLoad.Format(_T("%s%s"), pData->GetStringLoadCase(BRIDGE_LOAD_TOAP_ONE), nComboToapOne == 0 ? _T("L") : _T("R"));
					pFEM->AddHistory_Load(str, strLoad, _T(""), MatCombo(ix,BRIDGE_LOAD_TOAP_ONE));
				}
				if(dWCombo&LOAD_CASE_TEMP1)
				{
					strLoad = pData->GetStringLoadCase(BRIDGE_LOAD_TEMP1);
					pFEM->AddHistory_Load(str, strLoad, _T(""), MatCombo(ix,BRIDGE_LOAD_TEMP1));
				}
				if(dWCombo&LOAD_CASE_TEMP2)
				{
					strLoad = pData->GetStringLoadCase(BRIDGE_LOAD_TEMP2);
					pFEM->AddHistory_Load(str, strLoad, _T(""), MatCombo(ix,BRIDGE_LOAD_TEMP2));
				}
				if(dWCombo&LOAD_CASE_SHRINK)
				{
					strLoad = pData->GetStringLoadCase(BRIDGE_LOAD_SHRINK);
					pFEM->AddHistory_Load(str, strLoad, _T(""), MatCombo(ix,BRIDGE_LOAD_SHRINK));
				}
				if(dWCombo&LOAD_CASE_LR && Compare(pLoad->m_dRail_LoadLR,0.0,">"))
				{
					strLoad = pData->GetStringLoadCase(BRIDGE_LOAD_LR);
					pFEM->AddHistory_Load(str, strLoad, _T(""), MatCombo(ix,BRIDGE_LOAD_LR));
				}
				if(dWCombo&LOAD_CASE_SB && Compare(pLoad->m_dRail_LoadSB,0.0,">"))
				{
					strLoad = pData->GetStringLoadCase(BRIDGE_LOAD_SB);
					pFEM->AddHistory_Load(str, strLoad, _T(""), MatCombo(ix,BRIDGE_LOAD_SB));
				}
				if(pBri->m_bApplyGroundWaterLevel)
				{
					if(dWCombo&LOAD_CASE_WATERHORI && pBri->m_bApplyLoad[BRIDGE_LOAD_WATERHORI])
					{
						strLoad = pData->GetStringLoadCase(BRIDGE_LOAD_WATERHORI);
						pFEM->AddHistory_Load(str, strLoad, _T(""), MatCombo(ix,BRIDGE_LOAD_WATERHORI));
					}
					if(dWCombo&LOAD_CASE_WATERVERT && pBri->m_bApplyLoad[BRIDGE_LOAD_WATERVERT])
					{
						strLoad = pData->GetStringLoadCase(BRIDGE_LOAD_WATERVERT);
						pFEM->AddHistory_Load(str, strLoad, _T(""), MatCombo(ix,BRIDGE_LOAD_WATERVERT));
					}
				}
				if(pBri->m_nTypeLower == BOXFOOTING_TYPE_ANCHOR && pBri->m_bApplyAnchorForce)
				{
					strLoad = _T("AN");
					pFEM->AddHistory_Load(str, strLoad, _T(""), pBri->m_dFactorAnchor[nComboCase][ix]);
				}
			}
		}
	}
}

void CARcBridgeDesignFrame::MakeSapDataNLLink(CFEMManage *pFEM, BOOL bEarthQuake)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	if(pBri->IsBoxType() == FALSE) return;
	if(bEarthQuake == FALSE && pBri->m_n2DGeneralSpringType == 1) return;
	if(bEarthQuake == TRUE && pBri->m_n2DEQSpringType == 1) return;

	CString strName=_T("LS");
	
	long nCase = bEarthQuake ? MODULUS_FOUNDATION_EQ : MODULUS_FOUNDATION_NORMAL;

	// 하부슬래브
	long nSize = m_vArrForceLower[nCase].GetSize();
	CDPointArray xyArrPropSlab, xyArrPropWall[2];
	CDPoint xyProp(0,0);
	for(long ix = 0; ix < nSize; ix++)
	{
		xyProp.x = m_vArrForceLower[nCase].GetAt(ix).x;
		xyProp.y = m_vArrForceLower[nCase].GetAt(ix).z;
		if(xyArrPropSlab.GetIndex(xyProp) == -1)
		{
			xyArrPropSlab.Add(xyProp);
		}
	}
	// 벽체
	for(long nLR = iLEFT; nLR <= iRIGHT; nLR++)
	{
		nSize = m_vArrForceWall[nLR].GetSize();
		for(long ix = 0; ix < nSize; ix++)
		{
			xyProp.x = m_vArrForceWall[nLR].GetAt(ix).x;
			xyProp.y = m_vArrForceWall[nLR].GetAt(ix).z;
			if(xyArrPropWall[nLR].GetIndex(xyProp) == -1)
			{
				xyArrPropWall[nLR].Add(xyProp);
			}
		}
	}

	double dAngle = 0;
	long nIndexProp = 0;
	long nSizeJoint = pFEM->GetJointSize();
	long iNode = 0, jNode = 0;
	for(long ix = 0; ix < nSizeJoint; ix++)
	{
		CJoint *pJnt = pFEM->GetJoint((USHORT)ix);
		if(pJnt->m_BoolAttrib[BOOL_4_JOINT_SHOE_POS] == TRUE)
		{
			iNode = pJnt->m_LongAttrib[LONG_3_JOINT_HUNCHPOS];
			jNode = ix;
			CVector vI = pFEM->GetJoint((unsigned short)iNode)->m_vPoint;
			CVector vJ = pFEM->GetJoint((unsigned short)jNode)->m_vPoint;
			// 하부슬래브
			if(Compare(vI.x, vJ.x, _T("=")))
			{
				xyProp.x = m_vArrForceLower[nCase].GetAt(pJnt->m_LongAttrib[LONG_4_INDEX_SPRING]).x;
				xyProp.y = m_vArrForceLower[nCase].GetAt(pJnt->m_LongAttrib[LONG_4_INDEX_SPRING]).z;
				nIndexProp = xyArrPropSlab.GetIndex(xyProp);
				if(nIndexProp != -1)
				{
					strName.Format(_T("LS%02d"), nIndexProp + 1);
					pFEM->AddNLLink((USHORT)iNode, (USHORT)jNode, strName, dAngle);
				}
			}
			// 좌측벽체
			if(Compare(vI.x, vJ.x, _T(">")))
			{
				xyProp.x = m_vArrForceWall[iLEFT].GetAt(pJnt->m_LongAttrib[LONG_4_INDEX_SPRING]).x;
				xyProp.y = m_vArrForceWall[iLEFT].GetAt(pJnt->m_LongAttrib[LONG_4_INDEX_SPRING]).z;
				nIndexProp = xyArrPropWall[iLEFT].GetIndex(xyProp);
				if(nIndexProp != -1)
				{
					strName.Format(_T("LL%02d"), nIndexProp + 1);
					pFEM->AddNLLink((USHORT)iNode, (USHORT)jNode, strName, dAngle);
				}
			}
			// 우측벽체
			if(Compare(vI.x, vJ.x, _T("<")))
			{
				xyProp.x = m_vArrForceWall[iRIGHT].GetAt(pJnt->m_LongAttrib[LONG_4_INDEX_SPRING]).x;
				xyProp.y = m_vArrForceWall[iRIGHT].GetAt(pJnt->m_LongAttrib[LONG_4_INDEX_SPRING]).z;
				nIndexProp = xyArrPropWall[iRIGHT].GetIndex(xyProp);
				if(nIndexProp != -1)
				{
					strName.Format(_T("LR%02d"), nIndexProp + 1);
					pFEM->AddNLLink((USHORT)iNode, (USHORT)jNode, strName, dAngle);
				}
			}
		}
	}
}

void CARcBridgeDesignFrame::MakeSapDataConstraints(CFEMManage *pFEM)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	
	if(pBri->m_nTypeBridge != BRIDGE_TYPE_ARCH) return;
	long nJijumConnect = pBri->GetJijumConnectRibAndSlab();	// ARCH교일경우 리브와 슬래브가 붙은 지점
	double dW = pBri->GetThickWall(nJijumConnect)/2;
	double dStaJijum = pBri->GetStationOnJijum(nJijumConnect) - (pBri->m_dStationBridgeStt - pBri->m_dWS/2);
	
	long nSizeJnt = pFEM->GetJointSize();
	CString strName = _T("");
	CString strType = _T("EQUAL");
	long nIdxName = 0;
	
	long i=0; for(i=0; i<nSizeJnt-1; i++)
	{
		CJoint *pJnt1 = pFEM->GetJoint((USHORT)i);
		
		long nJointType = pJnt1->m_LongAttrib[LONG_0_JOINT_TYPE];
		if(nJointType!=JOINT_UPPERSLAB) break;
		
		if(pJnt1->m_vPointOrigin.x>=toM(dStaJijum-dW) && pJnt1->m_vPointOrigin.x<=toM(dStaJijum+dW))
		{
			long j=0; for(j=0; j<nSizeJnt-1; j++)
			{
				CJoint *pJnt2 = pFEM->GetJoint((USHORT)j);
				
				long nJointType = pJnt2->m_LongAttrib[LONG_0_JOINT_TYPE];
				if(nJointType!=JOINT_ARCHRIB) continue;
				
				if(pJnt1->m_vPointOrigin.x == pJnt2->m_vPointOrigin.x)
				{
					nIdxName++;
					strName.Format("EQUAL%d", nIdxName);
					pFEM->AddConstraints(strName,strType, 1, 0, 1, 0, 1, 0, pJnt1->m_nIdx+1, pJnt2->m_nIdx+1);
					break;
				}
			}
		}
	}	
}

long CARcBridgeDesignFrame::GetIndexJointLowerSlabMid(CFEMManage *pFEM)
{
	long nIndex = -1;

	double dXstt = 0, dXend = 0;

	long nSizeJoint = pFEM->GetJointSize();
	for(long ix = 0; ix < nSizeJoint; ix++)
	{
		CJoint *pJnt = pFEM->GetJoint((unsigned short)ix);
		if(pJnt->m_LongAttrib[LONG_0_JOINT_TYPE] != JOINT_LOWERSLAB) continue;

		if(dXstt == 0 && dXend == 0)
		{
			dXstt = pJnt->m_vPoint.x;
		}
		dXend = pJnt->m_vPoint.x;
	}

	double dXMid = (dXstt + dXend) / 2.;
	double dXMin = 100000000;
	for(long ix = 0; ix < nSizeJoint; ix++)
	{
		CJoint *pJnt = pFEM->GetJoint((unsigned short)ix);
		if(pJnt->m_LongAttrib[LONG_0_JOINT_TYPE] != JOINT_LOWERSLAB) continue;
		
		if(fabs(dXMid - pJnt->m_vPoint.x) < dXMin)
		{
			dXMin = fabs(dXMid - pJnt->m_vPoint.x);
			nIndex = ix;
		}
	}

	return nIndex;
}

void CARcBridgeDesignFrame::GetArrayAnchorForceAndDist(CFEMManage *pFEM, CElement *pElement, CDoubleArray &dArrAnchorForce, CDoubleArray &dArrDistAnchor, BOOL bEarthQauake)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	if(pBri->IsBoxType() == FALSE) return ;
	if(pBri->m_nTypeLower != BOXFOOTING_TYPE_ANCHOR) return;
	if(pBri->m_bApplyAnchorForce == FALSE) return;

	CLineInfo *pLine = pBri->GetLineBase();
	double dWidth = bEarthQauake ? 1.0 : m_pStd->GetSlabWidth();
	double dStation = pBri->GetStationBridgeStt(FALSE);
	double dStationAnchor = pLine->GetStationByOffsetWidth(dStation, -pBri->m_dWS, pBri->GetAngleJijum(0));

	CJoint *pJstt = pFEM->GetJoint(pElement->m_nJStt);
	CJoint *pJend = pFEM->GetJoint(pElement->m_nJEnd);

	dArrAnchorForce.RemoveAll();
	dArrDistAnchor.RemoveAll();
	double dAnchorForce = 0, dDistAnchor = 0;
	for(long ix = 0; ix < pBri->m_nCountAnchor; ix++)
	{
		dStationAnchor += pBri->m_dArrDistJongAnchor.GetAt(ix);

		if(Compare(pJstt->m_vPointReal.x, dStationAnchor, _T("<=")) && Compare(pJend->m_vPointReal.x, dStationAnchor, _T(">=")))
		{
			dAnchorForce = GetValueUnitChange(pBri->m_dArrLoadAnchor.GetAt(ix) * pBri->m_dArrEaAnchor.GetAt(ix) / dWidth, UNIT_CGS_TONF, pData->m_szTonf);
			dDistAnchor = toM(dStationAnchor - pJstt->m_vPointReal.x);

			dArrAnchorForce.Add(dAnchorForce);
			dArrDistAnchor.Add(dDistAnchor);
		}
	}
}

double CARcBridgeDesignFrame::GetThickElementAvg( CFEMManage *pFEM, CElement *pElement )
{
	double dThick(1.0);
	if(pFEM == NULL || pElement == NULL) return dThick;

	CJoint *pJStt = pFEM->GetJoint(pElement->m_nJStt);
	CJoint *pJEnd = pFEM->GetJoint(pElement->m_nJEnd);
	if(pJStt == NULL || pJEnd == NULL) return dThick;

	BOOL bArch = (pElement->m_nFlag == ELEMENT_ARCHRIB)? TRUE : FALSE;
	BOOL bSlab = bArch? FALSE : ((pElement->m_nFlag == ELEMENT_UPPERSLAB || pElement->m_nFlag == ELEMENT_LOWERSLAB)? TRUE : FALSE);

	long ixSectStt = pJStt->m_LongAttrib[LONG_2_JOINT_SECTION_NO] - 1;
	long ixSectEnd = pJEnd->m_LongAttrib[LONG_2_JOINT_SECTION_NO] - 1;

	long nSize = bArch? m_dArrThickArchRib.GetSize() : (bSlab ? m_dArrThickSlab.GetSize() : m_dArrThickWall.GetSize());
	if(ixSectStt < 0 || ixSectEnd < 0 || ixSectStt >= nSize || ixSectEnd >= nSize) return dThick;

	double dThickStt(0), dThickEnd(0);
	if(bArch)
	{
		dThickStt = m_dArrThickArchRib.GetAt(ixSectStt);
		dThickEnd = m_dArrThickArchRib.GetAt(ixSectEnd);
	}
	else if(bSlab)
	{
		dThickStt = m_dArrThickSlab.GetAt(ixSectStt);
		dThickEnd = m_dArrThickSlab.GetAt(ixSectEnd);
	}
	else
	{
		dThickStt = m_dArrThickWall.GetAt(ixSectStt);
		dThickEnd = m_dArrThickWall.GetAt(ixSectEnd);
	}
	dThick = (dThickStt + dThickEnd) / 2.0;

	return dThick;
}

CString CARcBridgeDesignFrame::GetMaterialNameByValue( CFEMManage *pFEM, double dEc )
{
	CString szName(_T(""));

	POSITION possub = pFEM->m_MaterialList.GetHeadPosition();
	while(possub)
	{
		CMaterial *pMat = (CMaterial *)pFEM->m_MaterialList.GetNext(possub);
		if(Compare(pMat->m_dE, dEc, _T("="), 0.001))
		{
			szName = pMat->m_szName;
			break;
		}
	}

	return szName;
}

BOOL CARcBridgeDesignFrame::IsUseMaterialLowerSlab()
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	BOOL bLsd = pData->IsLsdDesign();
	BOOL bUse = FALSE;

	if(bLsd && pBri->IsBoxType())
	{
		bUse = Compare(pBri->GetValueEc(ePartUpperSlab), pBri->GetValueEc(ePartLowerSlab), _T("="), 0.001)? FALSE : TRUE;
	}
	
	return bUse;
}

BOOL CARcBridgeDesignFrame::IsUseMaterialEndWall()
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	BOOL bLsd = pData->IsLsdDesign();
	BOOL bUse = FALSE;

	if(bLsd && pBri->IsOutsideWall())
	{
		bUse = Compare(pBri->GetValueEc(ePartOutWallStt), pBri->GetValueEc(ePartOutWallEnd), _T("="), 0.001)? FALSE : TRUE;
	}

	return bUse;
}

BOOL CARcBridgeDesignFrame::IsUseMaterialInnerWall()
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	BOOL bLsd = pData->IsLsdDesign();
	BOOL bUse = FALSE;

	if(bLsd && pBri->GetCountInWall() > 0)
	{
		// 시종점 벽체를 다르게 쓰고 있다면 내측벽체는 무조건 분리해서 쓰자.
		if(IsUseMaterialEndWall())
		{
			bUse = TRUE;
		}
		else
		{
			bUse = Compare(pBri->GetValueEc(ePartOutWallStt), pBri->GetValueEc(ePartInWall), _T("="), 0.001)? FALSE : TRUE;
		}
	}

	return bUse;
}

void CARcBridgeDesignFrame::MakeSapDataLoadNormal_Lsd( CFEMManage *pFEM )
{
	CRcBridgeRebar		*pBri  = m_pStd->m_pBri;
	CARcBridgeCalcLoad	*pLoad = m_pStd->m_pLoad;
	CARcBridgeDataStd   *pData = pBri->m_pARcBridgeDataStd;

	//////////////////////////////////////////////////////////////////////////
// 	double dSelfWeight = 1.0;
	long nIdxSlabStt = GetNumberElementSlabStt(pFEM);
	long nIdxSlabEnd = GetNumberElementSlabEnd(pFEM);
	long nSizeEle    = pFEM->GetElementSize();

	long nLoadCaseIndex = 0;

	CString strLoadName(_T(""));
	CString strLoadMark(_T(""));

	// 고정하중 DC / DW
	// 연직토압 EV
	MakeSapDataLoadDead_Lsd(pFEM, nLoadCaseIndex, FALSE);

	// 수평토압 EH
	MakeSapDataLoadHoriSoilPress_Lsd(pFEM, nLoadCaseIndex, FALSE);

	// 활하중 LL
	MakeSapDataLoadLiveLoadAll(pFEM, nLoadCaseIndex, FALSE);

	// 보도활하중
	if(pBri->m_plsdManager->IsSelectedLoadDesign(eLSD_LOADSELECT_PL))
	{
		MakeSapDataLoadLiveFootWay(pFEM, nLoadCaseIndex, FALSE);
	}

	// 상재활하중 LS
	if(pBri->m_plsdManager->IsSelectedLoadDesign(eLSD_LOADSELECT_LS))
	{
		MakeSapDataLoadLS_Lsd(pFEM, nLoadCaseIndex, FALSE);
	}

	//////////////////////////////////////////////////////////////////////////
	if(pBri->IsBoxType())
	{
		// 수평수압
		// 연직수압
		MakeSapDataLoadWater(pFEM, nLoadCaseIndex, FALSE);
	}

	// 온도하중
	MakeSapDataLoadTemperature(pFEM, nLoadCaseIndex);

	// 건조수축 SH
	if(pBri->m_plsdManager->IsSelectedLoadDesign(eLSD_LOADSELECT_SH))
	{
		strLoadName = pBri->m_plsdManager->GetStringLoadCaseLsd(eLSD_LOADCASE_SH, FALSE);
		strLoadMark  = pBri->m_plsdManager->GetStringLoadCaseLsd(eLSD_LOADCASE_SH, TRUE);

		pFEM->MakeElementLoadForce(nLoadCaseIndex+1);
		pFEM->m_szLCName.SetAt(nLoadCaseIndex, strLoadMark);
		pFEM->m_szLCComment.SetAt(nLoadCaseIndex, strLoadName);		

		if(pBri->m_bSelectShrinkLoadAddWall)
		{
			// 전체 재하
			for(long nIdx=0; nIdx<nSizeEle; nIdx++)
			{
				CElement *pE = pFEM->GetElement((USHORT)nIdx);
				CLoadForce *pELoad = pE->GetLoadForce(nLoadCaseIndex);
				pELoad->m_szLoadTitle = strLoadMark;
				pELoad->SetShrink(-pLoad->m_dLoadDryAndShrinkage,0,0);
			}
		}
		else
		{
			// Slab에만 재하
			for(long nIdx=nIdxSlabStt; nIdx<=nIdxSlabEnd; nIdx++)
			{
				CElement *pE = pFEM->GetElement((USHORT)nIdx);
				CLoadForce *pELoad = pE->GetLoadForce(nLoadCaseIndex);
				pELoad->m_szLoadTitle = strLoadMark;
				pELoad->SetShrink(-pLoad->m_dLoadDryAndShrinkage,0,0);
			}
		}
		++nLoadCaseIndex;		
	}

	// 상재토하중 ES
	if(pBri->m_plsdManager->IsSelectedLoadDesign(eLSD_LOADSELECT_ES))
	{
		MakeSapDataLoadES_Lsd(pFEM, nLoadCaseIndex, FALSE);
	}

	//////////////////////////////////////////////////////////////////////////
	MakeSapDataLoadSubsid(pFEM, nLoadCaseIndex);
	MakeSapDataLoadUserLoad(pFEM, nLoadCaseIndex, FALSE);
}

void CARcBridgeDesignFrame::MakeSapDataLoadSubsid( CFEMManage *pFEM, long &nLoadCaseIndex )
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd   *pData = pBri->m_pARcBridgeDataStd;
	CARcBridgeCalcLoad	*pLoad = m_pStd->m_pLoad;

	if(pBri->IsBoxType())
		return;

	BOOL bLsd = pData->IsLsdDesign();
	BOOL bApplyLoad(FALSE);
	CString strLoadName(_T(""));
	CString strLoadMark(_T(""));
	BOOL bApplyType = (pBri->m_nTypeBridge==BRIDGE_TYPE_TRAP2 || pBri->m_nTypeBridge==BRIDGE_TYPE_ARCH)? TRUE : FALSE;
	BOOL bArch	  = (pBri->m_nTypeBridge==BRIDGE_TYPE_ARCH) ? TRUE : FALSE;
	double dSubsidenceDisp = bLsd? frM(pLoad->m_dLoadSubsidence) : pLoad->m_dLoadSubsidence;

	if(bLsd)
	{
		bApplyLoad = pBri->m_plsdManager->IsSelectedLoadDesign(eLSD_LOADSELECT_SD);
		strLoadName = _T("지점침하");//pBri->m_plsdManager->GetStringLoadCaseLsd(eLSD_LOADCASE_SD1, FALSE);
		strLoadMark  = _T("SD");//pBri->m_plsdManager->GetStringLoadCaseLsd(eLSD_LOADCASE_SD1, TRUE);
	}
	else
	{
		bApplyLoad = pBri->m_bApplyLoad[BRIDGE_LOAD_SUBSID];
		strLoadName	= _T("지점침하");
		strLoadMark	= pData->GetStringLoadCase(BRIDGE_LOAD_SUBSID);
	}

	if(bApplyLoad && (pBri->m_nSelSubsideType==0 || !bApplyType))
	{
		BOOL bArchStt = FALSE;
		if(pBri->m_nSelect2DFrameTool!=SOLVE_MIDAS)
		{
			m_nCountSubsidence = 0;
			long nCountLoad = 1;
			long nQtyJijum  = (bArch)? 4 :  pBri->GetCountJijum();
			// 각지점별로 1개씩 땡기고...
			long nJijum=0; for(nJijum=0; nJijum<nQtyJijum; nJijum++)
			{
				bArchStt = (bArch && nJijum==1)? TRUE : FALSE;
				pFEM->MakeElementLoadForce(nLoadCaseIndex+1);
				pFEM->m_szLCName.SetAt(nLoadCaseIndex, strLoadMark+COMMA(nCountLoad++));
				pFEM->m_szLCComment.SetAt(nLoadCaseIndex, strLoadName+COMMA(nCountLoad-1));

				long nIdxFix = GetNumberJointJijumFix(pFEM, nJijum);
				if(nIdxFix == -1) continue;
				CElement *pE = GetElementOnJointIdx(pFEM, nIdxFix, bArchStt?  TRUE:FALSE);
				if(pE)
				{
					CLoadForce *pELoad = pE->GetLoadForce(nLoadCaseIndex);
					pELoad->SetDiplacementForce(bArchStt? 0.0 : 1.0,CVector(0,0,-dSubsidenceDisp));	
					++nLoadCaseIndex;
					m_nCountSubsidence++;
				}
			}
			// 2개씩 땡기고
			if(nQtyJijum>2)
			{
				long nJijum=0; for(nJijum=0; nJijum<nQtyJijum; nJijum++)
				{
					long i=0; for(i=nJijum+1; i<nQtyJijum; i++)
					{
						pFEM->MakeElementLoadForce(nLoadCaseIndex+1);
						pFEM->m_szLCName.SetAt(nLoadCaseIndex, strLoadMark+COMMA(nCountLoad++));
						pFEM->m_szLCComment.SetAt(nLoadCaseIndex, strLoadName+COMMA(nCountLoad-1));

						long nIdxFix1 = GetNumberJointJijumFix(pFEM, nJijum);
						long nIdxFix2 = GetNumberJointJijumFix(pFEM, i);
						if(nIdxFix1 == -1) continue;
						if(nIdxFix2 == -1) continue;

						CElement *pE1 = GetElementOnJointIdx(pFEM, nIdxFix1, (bArch && nJijum==1)?  TRUE:FALSE);
						CElement *pE2 = GetElementOnJointIdx(pFEM, nIdxFix2, (bArch && i==1)?  TRUE:FALSE);
						CLoadForce *pELoad1 = pE1->GetLoadForce(nLoadCaseIndex);
						CLoadForce *pELoad2 = pE2->GetLoadForce(nLoadCaseIndex);
						pELoad1->SetDiplacementForce((bArch && nJijum==1)? 0.0 : 1.0,CVector(0,0,-dSubsidenceDisp));
						pELoad2->SetDiplacementForce((bArch && i==1)? 0.0:1.0,CVector(0,0,-dSubsidenceDisp));
						++nLoadCaseIndex;
						m_nCountSubsidence++;
					}
				}
			}
			// 3개씩 땡기고 : 앞3,뒤3,앞2+뒤1,앞1+뒤2
			if(nQtyJijum>3)
			{
				// 시점에서 3개
				pFEM->MakeElementLoadForce(nLoadCaseIndex+1);
				pFEM->m_szLCName.SetAt(nLoadCaseIndex, strLoadMark+COMMA(nCountLoad++));
				pFEM->m_szLCComment.SetAt(nLoadCaseIndex, strLoadName+COMMA(nCountLoad-1));

				long nIdxFix1 = GetNumberJointJijumFix(pFEM, 0);
				long nIdxFix2 = GetNumberJointJijumFix(pFEM, 1);
				long nIdxFix3 = GetNumberJointJijumFix(pFEM, 2);

				if(nIdxFix1 != -1 && nIdxFix2 != -1 && nIdxFix3 != -1)
				{
					CElement *pE1 = GetElementOnJointIdx(pFEM, nIdxFix1, FALSE);
					CElement *pE2 = GetElementOnJointIdx(pFEM, nIdxFix2, (bArch)? TRUE:FALSE);
					CElement *pE3 = GetElementOnJointIdx(pFEM, nIdxFix3, FALSE);
					CLoadForce *pELoad1 = pE1->GetLoadForce(nLoadCaseIndex);
					CLoadForce *pELoad2 = pE2->GetLoadForce(nLoadCaseIndex);
					CLoadForce *pELoad3 = pE3->GetLoadForce(nLoadCaseIndex);
					pELoad1->SetDiplacementForce(1.0,CVector(0,0,-dSubsidenceDisp));
					pELoad2->SetDiplacementForce((bArch)? 0.0:1.0,CVector(0,0,-dSubsidenceDisp));
					pELoad3->SetDiplacementForce(1.0,CVector(0,0,-dSubsidenceDisp));
					++nLoadCaseIndex;
					m_nCountSubsidence++;
				}

				// 종점에서 3개
				pFEM->MakeElementLoadForce(nLoadCaseIndex+1);
				pFEM->m_szLCName.SetAt(nLoadCaseIndex, strLoadMark+COMMA(nCountLoad++));
				pFEM->m_szLCComment.SetAt(nLoadCaseIndex, strLoadName+COMMA(nCountLoad-1));
				nIdxFix1 = GetNumberJointJijumFix(pFEM, nQtyJijum-1);
				nIdxFix2 = GetNumberJointJijumFix(pFEM, nQtyJijum-2);
				nIdxFix3 = GetNumberJointJijumFix(pFEM, nQtyJijum-3);
				if(nIdxFix1 != -1 && nIdxFix2 != -1 && nIdxFix3 != -1)
				{
					CElement *pE1 = GetElementOnJointIdx(pFEM, nIdxFix1, FALSE);
					CElement *pE2 = GetElementOnJointIdx(pFEM, nIdxFix2, FALSE);
					CElement *pE3 = GetElementOnJointIdx(pFEM, nIdxFix3, (bArch)? TRUE : FALSE);
					CLoadForce *pELoad1 = pE1->GetLoadForce(nLoadCaseIndex);
					CLoadForce *pELoad2 = pE2->GetLoadForce(nLoadCaseIndex);
					CLoadForce *pELoad3 = pE3->GetLoadForce(nLoadCaseIndex);
					pELoad1->SetDiplacementForce(1.0,CVector(0,0,-dSubsidenceDisp));
					pELoad2->SetDiplacementForce(1.0,CVector(0,0,-dSubsidenceDisp));
					pELoad3->SetDiplacementForce((bArch)? 0.0:1.0,CVector(0,0,-dSubsidenceDisp));
					++nLoadCaseIndex;
					m_nCountSubsidence++;
				}

				// 시점2개, 종점1개
				pFEM->MakeElementLoadForce(nLoadCaseIndex+1);
				pFEM->m_szLCName.SetAt(nLoadCaseIndex, strLoadMark+COMMA(nCountLoad++));
				pFEM->m_szLCComment.SetAt(nLoadCaseIndex, strLoadName+COMMA(nCountLoad-1));
				nIdxFix1 = GetNumberJointJijumFix(pFEM, 0);
				nIdxFix2 = GetNumberJointJijumFix(pFEM, 1);
				nIdxFix3 = GetNumberJointJijumFix(pFEM, nQtyJijum-1);
				if(nIdxFix1 != -1 && nIdxFix2 != -1 && nIdxFix3 != -1)
				{
					CElement *pE1 = GetElementOnJointIdx(pFEM, nIdxFix1, FALSE);
					CElement *pE2 = GetElementOnJointIdx(pFEM, nIdxFix2, (bArch)? TRUE : FALSE);
					CElement *pE3 = GetElementOnJointIdx(pFEM, nIdxFix3, FALSE);
					CLoadForce *pELoad1 = pE1->GetLoadForce(nLoadCaseIndex);
					CLoadForce *pELoad2 = pE2->GetLoadForce(nLoadCaseIndex);
					CLoadForce *pELoad3 = pE3->GetLoadForce(nLoadCaseIndex);
					pELoad1->SetDiplacementForce(1.0,CVector(0,0,-dSubsidenceDisp));
					pELoad2->SetDiplacementForce((bArch)? 0.0:1.0,CVector(0,0,-dSubsidenceDisp));
					pELoad3->SetDiplacementForce(1.0,CVector(0,0,-dSubsidenceDisp));
					++nLoadCaseIndex;
					m_nCountSubsidence++;
				}

				// 시점1개, 종점2개
				pFEM->MakeElementLoadForce(nLoadCaseIndex+1);
				pFEM->m_szLCName.SetAt(nLoadCaseIndex, strLoadMark+COMMA(nCountLoad++));
				pFEM->m_szLCComment.SetAt(nLoadCaseIndex, strLoadName+COMMA(nCountLoad-1));
				nIdxFix1 = GetNumberJointJijumFix(pFEM, 0);
				nIdxFix2 = GetNumberJointJijumFix(pFEM, nQtyJijum-2);
				nIdxFix3 = GetNumberJointJijumFix(pFEM, nQtyJijum-1);
				if(nIdxFix1 != -1 && nIdxFix2 != -1 && nIdxFix3 != -1)
				{
					CElement *pE1 = GetElementOnJointIdx(pFEM, nIdxFix1, FALSE);
					CElement *pE2 = GetElementOnJointIdx(pFEM, nIdxFix2, FALSE);
					CElement *pE3 = GetElementOnJointIdx(pFEM, nIdxFix3, FALSE);
					CLoadForce *pELoad1 = pE1->GetLoadForce(nLoadCaseIndex);
					CLoadForce *pELoad2 = pE2->GetLoadForce(nLoadCaseIndex);
					CLoadForce *pELoad3 = pE3->GetLoadForce(nLoadCaseIndex);
					pELoad1->SetDiplacementForce(1.0,CVector(0,0,-dSubsidenceDisp));
					pELoad2->SetDiplacementForce(1.0,CVector(0,0,-dSubsidenceDisp));
					pELoad3->SetDiplacementForce(1.0,CVector(0,0,-dSubsidenceDisp));
					++nLoadCaseIndex;
					m_nCountSubsidence++;
				}
				if(nQtyJijum>4)
				{
					// 시점3개 (P1부터)
					pFEM->MakeElementLoadForce(nLoadCaseIndex+1);
					pFEM->m_szLCName.SetAt(nLoadCaseIndex, strLoadMark+COMMA(nCountLoad++));
					pFEM->m_szLCComment.SetAt(nLoadCaseIndex, strLoadName+COMMA(nCountLoad-1));
					nIdxFix1 = GetNumberJointJijumFix(pFEM, 1);
					nIdxFix2 = GetNumberJointJijumFix(pFEM, 2);
					nIdxFix3 = GetNumberJointJijumFix(pFEM, 3);
					if(nIdxFix1 != -1 && nIdxFix2 != -1 && nIdxFix3 != -1)
					{
						CElement *pE1 = GetElementOnJointIdx(pFEM, nIdxFix1, (bArch)? TRUE : FALSE);
						CElement *pE2 = GetElementOnJointIdx(pFEM, nIdxFix2, FALSE);
						CElement *pE3 = GetElementOnJointIdx(pFEM, nIdxFix3, FALSE);
						CLoadForce *pELoad1 = pE1->GetLoadForce(nLoadCaseIndex);
						CLoadForce *pELoad2 = pE2->GetLoadForce(nLoadCaseIndex);
						CLoadForce *pELoad3 = pE3->GetLoadForce(nLoadCaseIndex);
						pELoad1->SetDiplacementForce((bArch)? 0.0:1.0,CVector(0,0,-dSubsidenceDisp));
						pELoad2->SetDiplacementForce(1.0,CVector(0,0,-dSubsidenceDisp));
						pELoad3->SetDiplacementForce(1.0,CVector(0,0,-dSubsidenceDisp));
						++nLoadCaseIndex;
						m_nCountSubsidence++;
					}

					if(nQtyJijum>5)
					{
						// 종점3개 (A2이전까지)
						pFEM->MakeElementLoadForce(nLoadCaseIndex+1);
						pFEM->m_szLCName.SetAt(nLoadCaseIndex, strLoadMark+COMMA(nCountLoad++));
						pFEM->m_szLCComment.SetAt(nLoadCaseIndex, strLoadName+COMMA(nCountLoad-1));
						nIdxFix1 = GetNumberJointJijumFix(pFEM, nQtyJijum-4);
						nIdxFix2 = GetNumberJointJijumFix(pFEM, nQtyJijum-3);
						nIdxFix3 = GetNumberJointJijumFix(pFEM, nQtyJijum-2);
						if(nIdxFix1 != -1 && nIdxFix2 != -1 && nIdxFix3 != -1)
						{
							CElement *pE1 = GetElementOnJointIdx(pFEM, nIdxFix1, FALSE);
							CElement *pE2 = GetElementOnJointIdx(pFEM, nIdxFix2, (bArch)? TRUE : FALSE);
							CElement *pE3 = GetElementOnJointIdx(pFEM, nIdxFix3, FALSE);
							CLoadForce *pELoad1 = pE1->GetLoadForce(nLoadCaseIndex);
							CLoadForce *pELoad2 = pE2->GetLoadForce(nLoadCaseIndex);
							CLoadForce *pELoad3 = pE3->GetLoadForce(nLoadCaseIndex);
							pELoad1->SetDiplacementForce(1.0,CVector(0,0,-dSubsidenceDisp));
							pELoad2->SetDiplacementForce((bArch)? 0.0:1.0,CVector(0,0,-dSubsidenceDisp));
							pELoad3->SetDiplacementForce(1.0,CVector(0,0,-dSubsidenceDisp));
							++nLoadCaseIndex;
							m_nCountSubsidence++;
						}
					}
				}
			}
			if(nQtyJijum>5)
			{
				// 시점4개
				pFEM->MakeElementLoadForce(nLoadCaseIndex+1);
				pFEM->m_szLCName.SetAt(nLoadCaseIndex, strLoadMark+COMMA(nCountLoad++));
				pFEM->m_szLCComment.SetAt(nLoadCaseIndex, strLoadName+COMMA(nCountLoad-1));
				long nIdxFix1 = GetNumberJointJijumFix(pFEM, 0);
				long nIdxFix2 = GetNumberJointJijumFix(pFEM, 1);
				long nIdxFix3 = GetNumberJointJijumFix(pFEM, 2);
				long nIdxFix4 = GetNumberJointJijumFix(pFEM, 3);
				if(nIdxFix1 != -1 && nIdxFix2 != -1 && nIdxFix3 != -1 && nIdxFix4 != -1)
				{
					CElement *pE1 = GetElementOnJointIdx(pFEM, nIdxFix1, FALSE);
					CElement *pE2 = GetElementOnJointIdx(pFEM, nIdxFix2, (bArch)? TRUE : FALSE);
					CElement *pE3 = GetElementOnJointIdx(pFEM, nIdxFix3, FALSE);
					CElement *pE4 = GetElementOnJointIdx(pFEM, nIdxFix4, FALSE);
					CLoadForce *pELoad1 = pE1->GetLoadForce(nLoadCaseIndex);
					CLoadForce *pELoad2 = pE2->GetLoadForce(nLoadCaseIndex);
					CLoadForce *pELoad3 = pE3->GetLoadForce(nLoadCaseIndex);
					CLoadForce *pELoad4 = pE4->GetLoadForce(nLoadCaseIndex);
					pELoad1->SetDiplacementForce(1.0,CVector(0,0,-dSubsidenceDisp));
					pELoad2->SetDiplacementForce(1.0,CVector(0,0,-dSubsidenceDisp));
					pELoad3->SetDiplacementForce(1.0,CVector(0,0,-dSubsidenceDisp));
					pELoad4->SetDiplacementForce(1.0,CVector(0,0,-dSubsidenceDisp));
					++nLoadCaseIndex;
					m_nCountSubsidence++;
				}

				// 종점4개
				pFEM->MakeElementLoadForce(nLoadCaseIndex+1);
				pFEM->m_szLCName.SetAt(nLoadCaseIndex, strLoadMark+COMMA(nCountLoad++));
				pFEM->m_szLCComment.SetAt(nLoadCaseIndex, strLoadName+COMMA(nCountLoad-1));
				nIdxFix1 = GetNumberJointJijumFix(pFEM, nQtyJijum-4);
				nIdxFix2 = GetNumberJointJijumFix(pFEM, nQtyJijum-3);
				nIdxFix3 = GetNumberJointJijumFix(pFEM, nQtyJijum-2);
				nIdxFix4 = GetNumberJointJijumFix(pFEM, nQtyJijum-1);
				if(nIdxFix1 != -1 && nIdxFix2 != -1 && nIdxFix3 != -1 && nIdxFix4 != -1)
				{
					CElement *pE1 = GetElementOnJointIdx(pFEM, nIdxFix1, FALSE);
					CElement *pE2 = GetElementOnJointIdx(pFEM, nIdxFix2, (bArch)? TRUE : FALSE);
					CElement *pE3 = GetElementOnJointIdx(pFEM, nIdxFix3, FALSE);
					CElement *pE4 = GetElementOnJointIdx(pFEM, nIdxFix4, FALSE);
					CLoadForce *pELoad1 = pE1->GetLoadForce(nLoadCaseIndex);
					CLoadForce *pELoad2 = pE2->GetLoadForce(nLoadCaseIndex);
					CLoadForce *pELoad3 = pE3->GetLoadForce(nLoadCaseIndex);
					CLoadForce *pELoad4 = pE4->GetLoadForce(nLoadCaseIndex);
					pELoad1->SetDiplacementForce(1.0,CVector(0,0,-dSubsidenceDisp));
					pELoad2->SetDiplacementForce(1.0,CVector(0,0,-dSubsidenceDisp));
					pELoad3->SetDiplacementForce(1.0,CVector(0,0,-dSubsidenceDisp));
					pELoad4->SetDiplacementForce(1.0,CVector(0,0,-dSubsidenceDisp));
					++nLoadCaseIndex;
					m_nCountSubsidence++;
				}
			}
			// 5경간이상은 무시.............
		}
		if(pBri->m_nSelect2DFrameTool==SOLVE_MIDAS)
		{
			// (ARCBRIDGE-2570) SMLDCASE 를 사용 하기 때문에 ST LoadCase 추가하지 않는다.
			//pFEM->MakeElementLoadForce(nLoadCaseIndex+1);
			//pFEM->m_szLCName.SetAt(nLoadCaseIndex, strLoadMark);
			//pFEM->m_szLCComment.SetAt(nLoadCaseIndex, strLoadName);

			CString szName,szFixIdx;
			long nSubStt = -1;
			long nSubEnd = -1;
			long nIdxJointFix = 0;
			long nQtyJijum = bArch? 4 : pBri->m_nQtyJigan+1;
			long nJijum=0; for(nJijum=0; nJijum<nQtyJijum; nJijum++)
			{
				// (ARCBRIDGE-2570) 기초가 없는 지점에도 지점침하 적용
				//if(nJijum==0 && pBri->IsFooting(TRUE)==FALSE) continue;
				//if(nJijum==pBri->m_nQtyJigan && pBri->IsFooting(FALSE)==FALSE) continue;

				nIdxJointFix = GetNumberJointJijumFix(pFEM,nJijum);
				if(nIdxJointFix == -1) continue;

				szName.Format("WALL%d",nJijum+1);
				szFixIdx.Format("%d",nIdxJointFix+1);
				pFEM->SetSettlementForce(szName,-dSubsidenceDisp,szFixIdx);
				if(nSubStt==-1) nSubStt = nJijum;
				nSubEnd = nJijum;
			}
			pFEM->SetSettleCase(strLoadMark,1,nSubEnd-nSubStt,1.00); // (34913) nSubEnd-nSubStt+1 => nSubEnd-nSubStt로 변경
			// ++nLoadCaseIndex;
		}
	}
	else if(bApplyLoad)
	{
		m_nCountSubsidence = 0;
		long nQtyJijum  = (bArch)? 4 :  pBri->GetCountJijum();

		if(pBri->m_nSelect2DFrameTool!=SOLVE_MIDAS)
		{
			long nCountLoad = 1;
			long nJijum=0; for(nJijum=0; nJijum<nQtyJijum; nJijum++)
			{
				if(nJijum!=0 && nJijum!=nQtyJijum-2) continue;

				pFEM->MakeElementLoadForce(nLoadCaseIndex+1);
				pFEM->m_szLCName.SetAt(nLoadCaseIndex, strLoadMark+COMMA(nCountLoad++));
				pFEM->m_szLCComment.SetAt(nLoadCaseIndex, strLoadName+COMMA(nCountLoad-1));

				BOOL bArchStt = (nJijum==0 && pBri->m_nTypeBridge==BRIDGE_TYPE_ARCH);
				long nIdxFix1 = GetNumberJointJijumFix(pFEM, nJijum);
				long nIdxFix2 = GetNumberJointJijumFix(pFEM, nJijum+1);
				CElement *pE1 = GetElementOnJointIdx(pFEM, nIdxFix1, FALSE);
				CElement *pE2 = GetElementOnJointIdx(pFEM, nIdxFix2, bArchStt?  TRUE:FALSE);
				CLoadForce *pELoad1 = pE1->GetLoadForce(nLoadCaseIndex);
				CLoadForce *pELoad2 = pE2->GetLoadForce(nLoadCaseIndex);
				pELoad1->SetDiplacementForce(1.0,CVector(0,0,-dSubsidenceDisp));
				pELoad2->SetDiplacementForce(bArchStt? 0.0 : 1.0,CVector(0,0,-dSubsidenceDisp));
				++nLoadCaseIndex;
				m_nCountSubsidence++;		
			}
		}
		else if(pBri->m_nSelect2DFrameTool==SOLVE_MIDAS)
		{
			pFEM->MakeElementLoadForce(nLoadCaseIndex+1);	
			pFEM->m_szLCName.SetAt(nLoadCaseIndex, strLoadMark);
			pFEM->m_szLCComment.SetAt(nLoadCaseIndex, strLoadName);

			CString szName,szFixIdx;
			long nSubStt = -1;
			long nIdxJointFix1 = 0;
			long nIdxJointFix2 = 0;
			long nIdxName = 0;
			long nJijum=0; for(nJijum=0; nJijum<nQtyJijum; nJijum++)
			{
				if(nJijum!=0 && nJijum!=nQtyJijum-2) continue;

				nIdxJointFix1 = GetNumberJointJijumFix(pFEM,nJijum);
				nIdxJointFix2 = GetNumberJointJijumFix(pFEM,nJijum+1);

				szName.Format("FOOT%d", ++nIdxName);
				szFixIdx.Format("%d %d",nIdxJointFix1+1, nIdxJointFix2+1);
				pFEM->SetSettlementForce(szName,-dSubsidenceDisp,szFixIdx);
				if(nSubStt==-1) nSubStt = nJijum;
			}
			pFEM->SetSettleCase(strLoadMark,1,nIdxName,1.00);
			++nLoadCaseIndex;
		}
	}
}

void CARcBridgeDesignFrame::MakeSapDataLoadLiveLoadAll( CFEMManage *pFEM, long &nLoadCaseIndex, BOOL bEarthQuake )
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd   *pData = pBri->m_pARcBridgeDataStd;
	CARcBridgeCalcLoad	*pLoad = m_pStd->m_pLoad;

	BOOL bLsd = pData->IsLsdDesign();
	BOOL bApplyLoad(FALSE);
	BOOL bApplyLoadHeavy = pBri->IsApplyTankLoad();
	if(bLsd)
	{
		bApplyLoad = pBri->m_plsdManager->IsSelectedLoadDesign(eLSD_LOADSELECT_LL1);
	}
	else
		bApplyLoad = pBri->m_bApplyLoad[BRIDGE_LOAD_LIVE_FULL];

	double dWidth = bEarthQuake ? (bLsd? frM(m_pStd->GetSlabWidth()) : m_pStd->GetSlabWidth()) : 1.0;
		
	if(bApplyLoad == FALSE && bApplyLoadHeavy == FALSE)
		return;

	long nIdxSlabStt = GetNumberElementSlabStt(pFEM);
	long nIdxSlabEnd = GetNumberElementSlabEnd(pFEM);

	// 활하중 //////////////////////////////////////////////////////////////////////////		
	if(pBri->m_bUnderGroundRahmen && !pBri->m_bCalcOpen)// 지중라멘교 : 상재하중
	{
		MakeSapDataLoadLiveAlways(pFEM, nLoadCaseIndex, bEarthQuake);
	}
	else if(pLoad->m_bExistLane) // 차선하중
	{
		/////////////////////////////////// 중요 !!! = 마이다스는 레인에다가 충격계수를 주므로서 실제 Vehicle 에서 Load 에 충격계수 곱하는걸 빼야합 !! ///////////////////////
		for(long nIdx=nIdxSlabStt; nIdx<=nIdxSlabEnd; nIdx++)
			pFEM->AddLane("SLAB",nIdx+1,0);	

		if(pBri->m_pARcBridgeDataStd->IsRailDesign())
		{			
			//				double B = pLoad->m_dRail_WidthDistribute;
			double B = pLoad->m_bSingleLane ? pLoad->m_dRail_WidthDistribute : pLoad->m_dRail_WidthDistribute + toM(pLoad->m_dRail_DistBetweenRail);
			//				double I = (pBri->m_nSelect2DFrameTool==SOLVE_MIDAS) ? pLoad->m_dCoefficientImpact : 0;
			double I = (pData->m_nTypeFootingSafetyApplyCoefficientImpact == 1) ? 0 : pLoad->m_dCoefficientImpact[0];
			double N = (pBri->m_pARcBridgeDataStd->m_nTypeRailQty==0) ? 1 : 2;			

			CVehicle *pVehicle = NULL;

			if(pData->IsRailDesign2017())
			{
				double Pd = Round(GetValueUnitChange(pData->m_RailLoadWheel,UNIT_CGS_TONF,pData->m_szTonf)*N*(1+I)/B*dWidth,3);
				double dW = Round(GetValueUnitChange(pData->m_DistributionLoadFront,UNIT_CGS_TONF_M,pData->m_szTonf_M)*N*(1+I)/B*dWidth,3);

				pVehicle = pFEM->AddVehicleRail("LRAIL", 0, dW, 0.0, 0.0, Pd);
				pVehicle = pFEM->AddVehicleRail("LRAIL", 0, dW, pData->m_RailDistWheel, pData->m_RailDistWheelMid, Pd);
				pVehicle = pFEM->AddVehicleRail("LRAIL", 0, dW, 0.0, 0.0, 0.0);	
			}
			else
			{
				long nSizeWheel  = pBri->m_pARcBridgeDataStd->m_dArrLoadTrainWheel.GetSize();
				long i=0; for(i=0; i<nSizeWheel; i++)
				{
					long nType = (long)pBri->m_pARcBridgeDataStd->m_dArrTypeLoadTrainWheel.GetAt(i);
					double Load = pBri->m_pARcBridgeDataStd->m_dArrLoadTrainWheel.GetAt(i);
					Load = GetValueUnitChange(Load,UNIT_CGS_TONF,pData->m_szTonf);
					if(i==nSizeWheel-1)
					{
						if(nType==2)						
							pVehicle = pFEM->AddVehicleDB("LRAIL", 1, Round(Load*N*(1+I)/B*dWidth,3), 0.0, 0.0);					
						else											
							pVehicle = pFEM->AddVehicleDB("LRAIL", 0, Round(Load*N*(1+I)/B*dWidth,3), 0.0, 0.0);					
					}
					else
					{
						double D = pBri->m_pARcBridgeDataStd->m_dArrDistTrainWheel.GetAt(i);
						if(nType==2)					
							pVehicle = pFEM->AddVehicleDB("LRAIL", 1, Round(Load*N*(1+I)/B*dWidth,3), D, D);
						else					
							pVehicle = pFEM->AddVehicleDB("LRAIL", 0, Round(Load*N*(1+I)/B*dWidth,3), D, D);																
					}				
				}
			}
			if(pVehicle)
			{
				pVehicle->m_bTrain = TRUE;
			}
			
			pFEM->AddVehicleClass("VECL1", "LRAIL");
			long nRF = (pBri->m_nSelect2DFrameTool==SOLVE_MIDAS) ? 4 : 1; // ARCBRIDGE-3341,ARCBRIDGE-3361
			double dRF[6] = {1,};
			/////////////////////////////////// 중요 !!! = Scale Factor 모르겠음 ! ///////////////////////
			if(!pData->IsRailDesign2017() || pData->m_nTypeFootingSafetyApplyCoefficientImpact == 0)
			{
				pFEM->AddMovingLoad("MOVE1",nRF,dRF,"VECL1","SLAB",1,1,1);
			}
			else
			{
				// (ARCBRIDGE-3103) 기초 안정검토에 충격계수를 적용하지 않을때는 하중에 충격계수가 포함되어있지 않다.
				// 이동하중 조합에 직접 충격계수를 Factor로 주고 Factor가 1인 충격계수가 적용되지 않는 조합도 추가한다.
				pFEM->AddMovingLoad("MOVE1",nRF,dRF,"VECL1","SLAB", 1 + pLoad->m_dCoefficientImpact[0], 1, 1);
				pFEM->AddMovingLoad("MOVE1-S",nRF,dRF,"VECL1","SLAB", 1, 1, 1);
			}
		}
		else
		{			
			if(bApplyLoad)
			{
				// MIDAS INPUT작성시에도 활하중에 직접 넣는다.
				//double I   = (pBri->m_nSelect2DFrameTool==SOLVE_MIDAS) ? pLoad->m_dCoefficientImpact : 0;
				double I	= 0;
				double Pf  = GetValueUnitChange(pLoad->m_dPfApply[0]*dWidth/(1+I),UNIT_CGS_TONF_M,pData->m_szTonf_M);
				double Pr  = GetValueUnitChange(pLoad->m_dPrApply[0]*dWidth/(1+I),UNIT_CGS_TONF_M,pData->m_szTonf_M);
				double Dtb = GetValueUnitChange(pLoad->m_dDistributionApply*dWidth/(1+I),UNIT_CGS_TONF_M,pData->m_szTonf_M);
				double Pm  = GetValueUnitChange(pLoad->m_dPmApply[0]*dWidth/(1+I),UNIT_CGS_TONF_M,pData->m_szTonf_M);
				double Ps  = GetValueUnitChange(pLoad->m_dPsApply[0]*dWidth/(1+I),UNIT_CGS_TONF_M,pData->m_szTonf_M);
				double Pmt = GetValueUnitChange(pLoad->m_dPmApplyTruck[0]*dWidth/(1+I),UNIT_CGS_TONF_M,pData->m_szTonf_M);
				double wl = GetValueUnitChange(pLoad->m_dLaneLoad*dWidth,UNIT_CGS_TONF_M2,pData->m_szTonf_M2);

				if(bLsd)
				{
					double dDist1 = 3600;
					double dDist2 = 1200;
					double dDist3 = 7200;

					if(pBri->m_nSelect2DFrameTool!=SOLVE_MIDAS)
					{

						pFEM->AddVehicleDB("KLTRG",0,Pf,0,0);
						pFEM->AddVehicleDB("KLTRG",0,Pmt,dDist1,dDist1);
						pFEM->AddVehicleDB("KLTRG",0,Pmt,dDist2,dDist2);
						pFEM->AddVehicleDB("KLTRG",0,Pr,dDist3,dDist3);

						pFEM->AddVehicleDB("KLLAN",0,Pf * 0.75,0,0);
						pFEM->AddVehicleDB("KLLAN",0,Pmt * 0.75,dDist1,dDist1);
						pFEM->AddVehicleDB("KLLAN",0,Pmt * 0.75,dDist2,dDist2);
						pFEM->AddVehicleDB("KLLAN",0,Pr * 0.75,dDist3,dDist3);
						pFEM->AddVehicleDB("KLLAN",2,wl,0,0);
					}
					else
					{
						pFEM->AddVehicleDB("KLTRG",0,Pf,dDist1,dDist1);
						pFEM->AddVehicleDB("KLTRG",0,Pmt,dDist2,dDist2);
						pFEM->AddVehicleDB("KLTRG",0,Pmt,dDist3,dDist3);
						pFEM->AddVehicleDB("KLTRG",0,Pr,dDist3,dDist3);

						pFEM->AddVehicleDB("KLLAN",1,wl,0,0);
						pFEM->AddVehicleDB("KLLAN",0,Pf * 0.75,dDist1,dDist1);
						pFEM->AddVehicleDB("KLLAN",0,Pmt * 0.75,dDist2,dDist2);
						pFEM->AddVehicleDB("KLLAN",0,Pmt * 0.75,dDist3,dDist3);
						pFEM->AddVehicleDB("KLLAN",0,Pr * 0.75,dDist3,dDist3);
					}

					pFEM->AddVehicleClass("KLTRG", "KLTRG");
					pFEM->AddVehicleClass("KLLAN", "KLLAN");

					long nRF = (pBri->m_nSelect2DFrameTool==SOLVE_MIDAS) ? 4 : 1; // ARCBRIDGE-3341,ARCBRIDGE-3361
					double dRF[6] = {1.0, 1.0, 1.0, 1.0, };
					// 하중에 이미 안전율이 고려되어있으므로 안전율 제거..
					pFEM->AddMovingLoad("KLTRG",nRF,dRF,"KLTRG","SLAB",1,1,1);
					pFEM->AddMovingLoad("KLLAN",nRF,dRF,"KLLAN","SLAB",1,1,1);

					// (ARCBRIDGE-3232) 피로 검토 조합 제거 -> 피로활하중을 그대로 쓴다. 조합이 없으므로 무조건 출력
					if(!bEarthQuake)	// && pBri->m_plsdManager->IsUsedComboKind(eLSD_COMBINE_STRESS))
					{
						Pf  = GetValueUnitChange(pLoad->m_dPfApply[1]*dWidth/(1+I),UNIT_CGS_TONF_M,pData->m_szTonf_M);
						Pr  = GetValueUnitChange(pLoad->m_dPrApply[1]*dWidth/(1+I),UNIT_CGS_TONF_M,pData->m_szTonf_M);
						Pmt = GetValueUnitChange(pLoad->m_dPmApplyTruck[1]*dWidth/(1+I),UNIT_CGS_TONF_M,pData->m_szTonf_M);

						if(pBri->m_nSelect2DFrameTool!=SOLVE_MIDAS)
						{
							pFEM->AddVehicleDB("KLTFG",0,Pf,0,0);
							pFEM->AddVehicleDB("KLTFG",0,Pmt,dDist1,dDist1);
							pFEM->AddVehicleDB("KLTFG",0,Pmt,dDist2,dDist2);
							pFEM->AddVehicleDB("KLTFG",0,Pr,dDist3,dDist3);
						}
						else
						{
							pFEM->AddVehicleDB("KLTFG",0,Pf,dDist1,dDist1);
							pFEM->AddVehicleDB("KLTFG",0,Pmt,dDist2,dDist2);
							pFEM->AddVehicleDB("KLTFG",0,Pmt,dDist3,dDist3);
							pFEM->AddVehicleDB("KLTFG",0,Pr,dDist3,dDist3);
						}

						pFEM->AddVehicleClass("KLTFG", "KLTFG");

						pFEM->AddMovingLoad("KLTFG",nRF,dRF,"KLTFG","SLAB",1,1,1);
					}
				}
				else
				{
					double dDist1 = 4.2;
					double dDist2 = 9.0;

					if(pBri->m_nSelect2DFrameTool!=SOLVE_MIDAS)
					{

						pFEM->AddVehicleDB("DB",0,Pf,0,0);
						pFEM->AddVehicleDB("DB",0,Pr,dDist1,dDist1);
						pFEM->AddVehicleDB("DB",0,Pr,dDist1,dDist2);
					}
					else
					{
						pFEM->AddVehicleDB("DB",0,Pf,dDist1,dDist1);
						pFEM->AddVehicleDB("DB",0,Pr,dDist1,dDist1);
						pFEM->AddVehicleDB("DB",0,Pr,dDist2,dDist2);
					}
					pFEM->AddVehicleDL("DL",0,Dtb,Pm,Ps);

					pFEM->AddVehicleClass("DB", "DB");
					pFEM->AddVehicleClass("DL", "DL");			

					long nRF = (pBri->m_nSelect2DFrameTool==SOLVE_MIDAS) ? 4 : 1; // ARCBRIDGE-3341,ARCBRIDGE-3361
					double dRF[6] = {1.0, 1.0, 1.0, 1.0, };
					// 하중에 이미 안전율이 고려되어있으므로 안전율 제거..
					pFEM->AddMovingLoad("DB",nRF,dRF,"DB","SLAB",1,1,1);
					pFEM->AddMovingLoad("DL",nRF,dRF,"DL","SLAB",1,1,1);
				}
			}
			if(bApplyLoadHeavy)
			{
				// MIDAS INPUT작성시에도 활하중에 직접 넣는다.
				// double I    = (pBri->m_nSelect2DFrameTool==SOLVE_MIDAS) ? pLoad->m_dTank_i : 0;
				double I    = 0;
				double Ptr1 = GetValueUnitChange(pLoad->m_dTank_Ptrailer1*dWidth/(1+I),UNIT_CGS_TONFM,pData->m_szTonf_M);
				double Ptr2 = GetValueUnitChange(pLoad->m_dTank_Ptrailer2*dWidth/(1+I),UNIT_CGS_TONF_M,pData->m_szTonf_M);
				double Ptr3 = GetValueUnitChange(pLoad->m_dTank_Ptrailer3*dWidth/(1+I),UNIT_CGS_TONF_M,pData->m_szTonf_M);

				double dTL1 = bLsd? frM(pLoad->m_dTank_TrailerL1) : pLoad->m_dTank_TrailerL1;
				double dTL2 = bLsd? frM(pLoad->m_dTank_TrailerL2) : pLoad->m_dTank_TrailerL2;
				double dTL3 = bLsd? frM(pLoad->m_dTank_TrailerL3) : pLoad->m_dTank_TrailerL3;
				double dTL4 = bLsd? frM(pLoad->m_dTank_TrailerL4) : pLoad->m_dTank_TrailerL4;
				
				if(pBri->m_nSelect2DFrameTool!=SOLVE_MIDAS)
				{
					pFEM->AddVehicleDB("TRAILOR",0,Ptr1,0,0);
					pFEM->AddVehicleDB("TRAILOR",0,Ptr2,dTL1,dTL1);
					pFEM->AddVehicleDB("TRAILOR",0,Ptr2,dTL2,dTL2);
					pFEM->AddVehicleDB("TRAILOR",0,Ptr3,dTL3,dTL3);
					pFEM->AddVehicleDB("TRAILOR",0,Ptr3,dTL4,dTL4);
				}
				else
				{
					double dTL5 = bLsd? frM(9.0) : 9.0;
					pFEM->AddVehicleDB("TRAILOR",0,Ptr1,dTL1,dTL1);
					pFEM->AddVehicleDB("TRAILOR",0,Ptr2,dTL2,dTL2);
					pFEM->AddVehicleDB("TRAILOR",0,Ptr2,dTL3,dTL3);
					pFEM->AddVehicleDB("TRAILOR",0,Ptr3,dTL4,dTL4);
					pFEM->AddVehicleDB("TRAILOR",0,Ptr3,dTL5,dTL5);
				}
				// (35533) MIDAS도 분포이동하중 된다고 그냥 넣어 달라고~
				double Ptk  = GetValueUnitChange(pLoad->m_dTank_Ptank*dWidth,UNIT_CGS_TONF_M2,pData->m_szTonf_M2);
				double dPDist = bLsd? frM(pLoad->m_dTank_TankLength) : pLoad->m_dTank_TankLength;
				pFEM->AddVehicleDB("TANK",0,Ptk,dPDist,dPDist);

				/*double Ptk  = GetValueUnitChange(pLoad->m_dTank_Ptank/(1+I)*pLoad->m_dTank_TankLength,UNIT_CGS_TONF,pData->m_szTonf);
				double dDiv = 10; // MIDAS는 분포이동하중이 재하되지 않으므로 10개로 나눠서 재하....
				double dPDist = pLoad->m_dTank_TankLength/dDiv;
				long i=0; for(i=0; i<=(long)dDiv; i++)
				{
				if(pBri->m_nSelect2DFrameTool!=SOLVE_MIDAS)
				{
				if(i==0)
				pFEM->AddVehicleDB("TANK",0,Ptk/dDiv*0.5,0,0);
				else if(i==1)
				pFEM->AddVehicleDB("TANK",0,Ptk/dDiv*0.5,dPDist*0.5,dPDist*0.5);
				else if(i==(long)dDiv)
				pFEM->AddVehicleDB("TANK",0,Ptk/dDiv,dPDist*0.5,dPDist*0.5);		
				else
				pFEM->AddVehicleDB("TANK",0,Ptk/dDiv,dPDist,dPDist);
				}
				else
				{
				if(i==0)
				pFEM->AddVehicleDB("TANK",0,Ptk/dDiv*0.5,dPDist*0.5,dPDist*0.5);
				else if(i==1)
				pFEM->AddVehicleDB("TANK",0,Ptk/dDiv*0.5,dPDist*0.5,dPDist*0.5);
				else if(i==(long)dDiv)
				pFEM->AddVehicleDB("TANK",0,Ptk/dDiv,0,0);		
				else
				pFEM->AddVehicleDB("TANK",0,Ptk/dDiv,dPDist,dPDist);
				}
				}*/
				//
				pFEM->AddVehicleClass("TRAILOR", "TRAILOR");
				pFEM->AddVehicleClass("TANK"   , "TANK");			

				double dRF[6] = {1,};
				//					pFEM->AddMovingLoad("TRAILOR",1,dRF,"TRAILOR","SLAB",1+pLoad->m_dTank_i,1,1);
				//					pFEM->AddMovingLoad("TANK"   ,1,dRF,"TANK"   ,"SLAB",1+pLoad->m_dTank_i,1,1);
				pFEM->AddMovingLoad("TRAILOR",1,dRF,"TRAILOR","SLAB",1,1,1);
				pFEM->AddMovingLoad("TANK"   ,1,dRF,"TANK"   ,"SLAB",1,1,1);
			}
		}
	}
}

void CARcBridgeDesignFrame::MakeSapDataLoadTemperature( CFEMManage *pFEM, long &nLoadCaseIndex )
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd   *pData = pBri->m_pARcBridgeDataStd;
	CARcBridgeCalcLoad	*pLoad = m_pStd->m_pLoad;

	BOOL bLsd = pData->IsLsdDesign();
	BOOL bApplyLoadPlus(FALSE);
	BOOL bApplyLoadMinus(FALSE);
	if(bLsd)
	{
		bApplyLoadPlus = pBri->m_plsdManager->IsSelectedLoadDesign(eLSD_LOADSELECT_TU);
		bApplyLoadMinus = pBri->m_plsdManager->IsSelectedLoadDesign(eLSD_LOADSELECT_TU);
	}
	else
	{
		bApplyLoadPlus = pBri->m_bApplyLoad[BRIDGE_LOAD_TEMP1];
		bApplyLoadMinus = pBri->m_bApplyLoad[BRIDGE_LOAD_TEMP2];
	}

	CString strLoadName(_T(""));
	CString strLoadMark(_T(""));
	long nSizeEle    = pFEM->GetElementSize();

	if(bApplyLoadPlus)
	{
		if(bLsd)
		{
			strLoadName = pBri->m_plsdManager->GetStringLoadCaseLsd(eLSD_LOADCASE_TU21, FALSE);
			strLoadMark = pBri->m_plsdManager->GetStringLoadCaseLsd(eLSD_LOADCASE_TU21, TRUE);
		}
		else
		{
			strLoadName = _T("온도하중(+)");
			strLoadMark = pData->GetStringLoadCase(BRIDGE_LOAD_TEMP1);
		}
		pFEM->MakeElementLoadForce(nLoadCaseIndex+1);
		pFEM->m_szLCName.SetAt(nLoadCaseIndex, strLoadMark);
		pFEM->m_szLCComment.SetAt(nLoadCaseIndex, strLoadName);

		for(long nIdx=0; nIdx<nSizeEle; nIdx++)
		{
			CElement *pE = pFEM->GetElement((USHORT)nIdx);
			CLoadForce *pELoad = pE->GetLoadForce(nLoadCaseIndex);
			pELoad->m_szLoadTitle = strLoadMark;

			// (ARCBRIDGE-1716) 부재 두께를 고려 해서 온도하중을 넣어 준다. 부재두께는 1m를 적용 한다.
			double dThick = bLsd? frM(GetThickElementAvg(pFEM, pE)) : GetThickElementAvg(pFEM, pE);
			double dRatioThick = 1.0 / dThick;

			if(pE->m_nFlag==ELEMENT_UPPERSLAB)
			{
				if(bLsd)
				{
					pELoad->SetTemperature(pLoad->m_dLoadTempertureAxial,0,0,TRUE);
				}
				else
				{
					// sap2000에서는 상하면 온도차에 슬래브두께 고려 안함.
					if(pBri->m_nSelectTempLoadSlab == 0)
						pELoad->SetTemperature(pLoad->m_dLoadTempertureAxial,Round(pLoad->m_dLoadTempertureUpDown * dRatioThick,3),0,TRUE);
					else if(pBri->m_nSelectTempLoadSlab == 1)
						pELoad->SetTemperature(pLoad->m_dLoadTempertureAxial,0,0,TRUE);
					else if(pBri->m_nSelectTempLoadSlab == 2)
						pELoad->SetTemperature(0,Round(pLoad->m_dLoadTempertureUpDown * dRatioThick,3),0,TRUE);
				}
			}
			else if(pBri->m_bSelectTempLoadAddWall)
				pELoad->SetTemperature(pLoad->m_dLoadTempertureAxial,0,0,TRUE);
		}
		++nLoadCaseIndex;		
	}
	
	if(bApplyLoadMinus)
	{
		if(bLsd)
		{
			strLoadName = pBri->m_plsdManager->GetStringLoadCaseLsd(eLSD_LOADCASE_TU22, FALSE);
			strLoadMark = pBri->m_plsdManager->GetStringLoadCaseLsd(eLSD_LOADCASE_TU22, TRUE);
		}
		else
		{
			strLoadName = _T("온도하중(-)");
			strLoadMark = pData->GetStringLoadCase(BRIDGE_LOAD_TEMP2);
		}

		pFEM->MakeElementLoadForce(nLoadCaseIndex+1);
		pFEM->m_szLCName.SetAt(nLoadCaseIndex, strLoadMark);
		pFEM->m_szLCComment.SetAt(nLoadCaseIndex, strLoadName);

		for(long nIdx=0; nIdx<nSizeEle; nIdx++)
		{
			CElement *pE = pFEM->GetElement((USHORT)nIdx);
			CLoadForce *pELoad = pE->GetLoadForce(nLoadCaseIndex);
			pELoad->m_szLoadTitle = strLoadMark;

			double dThick = bLsd? frM(GetThickElementAvg(pFEM, pE)) : GetThickElementAvg(pFEM, pE);
			double dRatioThick = 1.0 / dThick;

			if(pE->m_nFlag==ELEMENT_UPPERSLAB)
			{
				if(bLsd)
				{
					pELoad->SetTemperature(-pLoad->m_dLoadTempertureAxial,0,0,TRUE);
				}
				else
				{
					if(pBri->m_nSelectTempLoadSlab == 0)
						pELoad->SetTemperature(-pLoad->m_dLoadTempertureAxial,Round(-pLoad->m_dLoadTempertureUpDown * dRatioThick,3),0,TRUE);
					else if(pBri->m_nSelectTempLoadSlab == 1)
						pELoad->SetTemperature(-pLoad->m_dLoadTempertureAxial,0,0,TRUE);
					else if(pBri->m_nSelectTempLoadSlab == 2)
						pELoad->SetTemperature(0,Round(-pLoad->m_dLoadTempertureUpDown * dRatioThick,3),0,TRUE);
				}
			}
			else if(pBri->m_bSelectTempLoadAddWall)
				pELoad->SetTemperature(-pLoad->m_dLoadTempertureAxial,0,0,TRUE);
		}
		++nLoadCaseIndex;		
	}

	// TG하중
	if(bLsd && pBri->m_plsdManager->IsSelectedLoadDesign(eLSD_LOADSELECT_TG))
	{
		for(long ix=0; ix<2; ++ix)
		{
			BOOL bPlus = (ix == 0);
			strLoadName = pBri->m_plsdManager->GetStringLoadCaseLsd(bPlus? eLSD_LOADCASE_TGP : eLSD_LOADCASE_TGM, FALSE);
			strLoadMark = pBri->m_plsdManager->GetStringLoadCaseLsd(bPlus? eLSD_LOADCASE_TGP : eLSD_LOADCASE_TGM, TRUE);

			pFEM->MakeElementLoadForce(nLoadCaseIndex+1);
			pFEM->m_szLCName.SetAt(nLoadCaseIndex, strLoadMark);
			pFEM->m_szLCComment.SetAt(nLoadCaseIndex, strLoadName);

			double dValueTG = pLoad->m_dLoadTgValueT[ix];

			for(long nIdx=0; nIdx<nSizeEle; nIdx++)
			{
				CElement *pE = pFEM->GetElement((USHORT)nIdx);
				CLoadForce *pELoad = pE->GetLoadForce(nLoadCaseIndex);
				pELoad->m_szLoadTitle = strLoadMark;

				// (ARCBRIDGE-1716) 부재 두께를 고려 해서 온도하중을 넣어 준다. 부재두께는 1m를 적용 한다.
				double dThick = bLsd? frM(GetThickElementAvg(pFEM, pE)) : GetThickElementAvg(pFEM, pE);
				double dRatioThick = 1.0 / dThick;

				if(pE->m_nFlag==ELEMENT_UPPERSLAB)
				{
					pELoad->SetTemperature(0.0, dValueTG * dRatioThick, 0,TRUE);
				}
			}
			++nLoadCaseIndex;	
		}
	}
}

void CARcBridgeDesignFrame::MakeSapDataCombineNormal_Lsd( CFEMManage *pFEM, BOOL bEarthQuake, BOOL b3DPlate )
{
	CRcBridgeRebar		*pBri  = m_pStd->m_pBri;
	CARcBridgeCalcLoad	*pLoad = m_pStd->m_pLoad;
	CARcBridgeDataStd   *pData = pBri->m_pARcBridgeDataStd;

	BOOL bEnvSubsid = pBri->m_bSelectEnvSubsid || b3DPlate ? TRUE : FALSE;
	BOOL bSap2000 = (b3DPlate==FALSE && pBri->m_nSelect2DFrameTool!=SOLVE_MIDAS) ? TRUE : FALSE;

	BOOL bHistoryCombo = (m_bHistory[0] && b3DPlate == FALSE) ? TRUE : FALSE;
	BOOL bApplyLoadLive = pBri->m_plsdManager->IsSelectedLoadDesign(eLSD_LOADSELECT_LL1);

	MakeSapDataCombineLiveLoad(pFEM, FALSE);
	MakeSapDataCombineSoilPressure(pFEM);
	MakeSapDataCombineSubsid(pFEM, b3DPlate);

	CString str(_T("")), strLoad(_T(""));
	double dFactor(0);
	
	for(long nL=0; nL<eLSD_COMBINE_TOTAL_COUNT; nL++)
	{	
		ELSDCombKind eKind = static_cast<ELSDCombKind>(nL);

		if(eKind == eLSD_COMBINE_EXTREME_I || eKind == eLSD_COMBINE_EXTREME_II)
			continue;
		
		std::vector<CLsdLoadCase *> LCArray = pBri->m_plsdManager->GetLsdLoadCaseArray(eKind,eLSD_LOADCASE_DIR_BRI);
		for(auto ite=LCArray.begin(); ite!=LCArray.end(); ++ite)
		{
			str = (*ite)->GetName();

			// SHELL 조합은 선택된 것만...
			//if(bEarthQuake && pBri->m_bApplyCombo3D[nComboCase][i]==FALSE) continue;
			if(bHistoryCombo)
			{
				strLoad = _T("H") + str;
				pFEM->AddCombo(str,strLoad, 0, 3, 1, FALSE);
			}

			for(long n=0; n<eLSD_LOADCASE_SIZE; ++n)
			{
				ELSDLoadCaseKind eCase = static_cast<ELSDLoadCaseKind>(n);
				if(pBri->m_plsdManager->IsSafeValidLoad(eCase) == FALSE)
					continue;
				
				strLoad = pBri->m_plsdManager->GetDesignNameByIndex(eCase);

				long nType(0);
				if(eCase == eLSD_LOADCASE_LL1 || eCase == eLSD_LOADCASE_LL2)
				{
					nType = (pLoad->m_bExistLane && bApplyLoadLive && (!pBri->m_bUnderGroundRahmen || pBri->m_bCalcOpen))? 1 : 0;

					// 피로한계상태 활하중은 따로 준다.
					if(nType == 1 && eCase == eLSD_LOADCASE_LL1 && eKind == eLSD_COMBINE_STRESS)
					{
						nType = 1;
						strLoad = _T("LLFG");
					}
				}
				else if(eCase >= eLSD_LOADCASE_SD1 && eCase <= eLSD_LOADCASE_SD1)
				{
					// (ARCBRIDGE-3465) Midas 일때도 SMLDCASE로 들어가기 때문에 하나의 조합으로 들어간다. SD1~ 이아니라 그냥 SD로 들어간다.
					nType = (bEnvSubsid /*&& bSap2000*/) ? 1 : 0;
					if(nType == 1)
					{
						strLoad = _T("SD");
					}

					if(b3DPlate || pBri->m_nSelect2DFrameTool == SOLVE_MIDAS)
						nType = 2;
				}

				if(bHistoryCombo && nType == 0)
					continue;

				if(nType == 0)
				{	
					if(pFEM->GetLoadCaseIndex(strLoad) == -1)
						continue;
				}
				
				// (ARCBRIDGE-2865) 자기 조합에 있는 계수를 그냥 쓰면 되는데 m_plsdManager->GetLsdLoadCaseFactor()쓸 이유가 없다. 
				// 오히려 지하수위에의한 조합때문에 (*ite)->GetIndexCombo()의 인덱스가 m_plsdManager의 조합 배열 인덱스가 맞지 않는다.
				// dFactor = pBri->m_plsdManager->GetLsdLoadCaseFactor(eKind, eLSD_LOADCASE_DIR_BRI, (*ite)->GetIndexCombo(), eCase); (*ite)->GetLoadFactor(eCase);
				dFactor = (*ite)->GetLoadFactor(eCase);
				if(dFactor != 0)
				{
					pFEM->AddCombo(str,strLoad,0,nType,dFactor,FALSE);
				}
			}

			if(b3DPlate == FALSE)
			{
				// 사용자하중
				long nSizeUserLoad = pBri->m_vpUserLoadSet[0].size();
				for(long n=0; n<nSizeUserLoad; ++n)
				{
					CBridgeUserLoadSet *pLoadSet = pBri->m_vpUserLoadSet[0][n];

					if(pLoadSet->m_pArrUserLoad.GetSize() > 0)
					{
						long nIndexELSDLoadCaseKind = 0;
						CString sLoadCase = pLoadSet->m_sTypeLoadCase;
						for (long iC=0; iC<eLSD_LOADCASE_SIZE; ++iC)
						{
							if(sLoadCase==pBri->m_plsdManager->GetDesignNameByIndex(static_cast<ELSDLoadCaseKind>(iC)))
							{
								nIndexELSDLoadCaseKind = iC;
								break;
							}
						}

						ELSDLoadCaseKind eCase = static_cast<ELSDLoadCaseKind>(nIndexELSDLoadCaseKind);
						if(eCase!=-1)
						{
							strLoad.Format(_T("USER%d"), n+1);
							dFactor = (*ite)->GetLoadFactor(eCase);
							if(dFactor != 0)
							{
								pFEM->AddCombo(str,strLoad,0,0,dFactor,FALSE);
							}
						}
					}
				}
			}
		}
	}
}

void CARcBridgeDesignFrame::MakeSapDataCombineLiveLoad( CFEMManage *pFEM, BOOL bEarthQuake )
{
	CRcBridgeRebar		*pBri  = m_pStd->m_pBri;
	CARcBridgeCalcLoad	*pLoad = m_pStd->m_pLoad;
	CARcBridgeDataStd   *pData = pBri->m_pARcBridgeDataStd;

	BOOL bLsd = pData->IsLsdDesign();
	BOOL bApplyLoadLive(FALSE);
	BOOL bApplyLoadHeavy = pBri->IsApplyTankLoad();
	CString str(_T(""));

	if(bLsd)
		bApplyLoadLive = pBri->m_plsdManager->IsSelectedLoadDesign(eLSD_LOADSELECT_LL1);
	else
		bApplyLoadLive = pBri->m_bApplyLoad[BRIDGE_LOAD_LIVE_FULL];
		
	if(pLoad->m_bExistLane && bApplyLoadLive && (!pBri->m_bUnderGroundRahmen || pBri->m_bCalcOpen))
	{	
		if(pBri->m_pARcBridgeDataStd->IsRailDesign())
		{		
			pFEM->AddCombo(pData->GetStringLoadCase(BRIDGE_LOAD_LIVE_FULL), "MOVE1",1,0,1.0,TRUE);
			if(!bEarthQuake && pData->IsRailDesign2017() && pData->m_nTypeFootingSafetyApplyCoefficientImpact == 1)
			{
				pFEM->AddCombo(pData->GetStringLoadCase(BRIDGE_LOAD_LIVE_FULL) + _T("-S"), "MOVE1-S",1,0,1.0,TRUE);
			}
		}
		else
		{
			if(bLsd)
				str = pBri->m_plsdManager->GetStringLoadCaseLsd(eLSD_LOADCASE_LL1, TRUE);
			else
				str = pData->GetStringLoadCase(BRIDGE_LOAD_LIVE_FULL);

			pFEM->AddCombo(str, bLsd? _T("KLTRG") : _T("DB"),1,0,1.0,TRUE);
			pFEM->AddCombo(str, bLsd? _T("KLLAN") : _T("DL"),1,0,1.0,TRUE);

			// (ARCBRIDGE-3232) 피로 검토 조합 제거 -> 피로활하중을 그대로 쓴다. 조합이 없으므로 무조건 출력
			if(bLsd && !bEarthQuake)	// && pBri->m_plsdManager->IsUsedComboKind(eLSD_COMBINE_STRESS))
			{
				pFEM->AddCombo(str + _T("FG"), _T("KLTFG"),1,0,1.0,TRUE);
			}
		}
	}
	if(bApplyLoadHeavy && (!pBri->m_bUnderGroundRahmen || pBri->m_bCalcOpen))
	{	
		if(bLsd)
			str = pBri->m_plsdManager->GetStringLoadCaseLsd(eLSD_LOADCASE_LL2, TRUE);
		else
			str = pData->GetStringLoadCase(BRIDGE_LOAD_TANK);

		pFEM->AddCombo(str, "TRAILOR",1,0,1.0,TRUE);
		pFEM->AddCombo(str, "TANK",1,0,1.0,TRUE);
	}
}

void CARcBridgeDesignFrame::MakeSapDataCombineSubsid( CFEMManage *pFEM, BOOL b3DPlate )
{
	CRcBridgeRebar		*pBri  = m_pStd->m_pBri;
	CARcBridgeDataStd   *pData = pBri->m_pARcBridgeDataStd;

	BOOL bLsd = pData->IsLsdDesign();
	BOOL bApplyLoad(FALSE);
	BOOL bEnvSubsid = pBri->m_bSelectEnvSubsid || b3DPlate ? TRUE : FALSE;
	CString str(_T(""));

	if(bLsd)
		bApplyLoad = pBri->m_plsdManager->IsSelectedLoadDesign(eLSD_LOADSELECT_SD);
	else
		bApplyLoad = pBri->m_bApplyLoad[BRIDGE_LOAD_SUBSID];
		
	// SAP2000일 경우의 지점침하...
	BOOL bSap2000 = (b3DPlate==FALSE && pBri->m_nSelect2DFrameTool!=SOLVE_MIDAS) ? TRUE : FALSE;
	if(bSap2000 && bApplyLoad && bEnvSubsid)
	{
		if(bLsd)
			str = _T("SD");//pBri->m_plsdManager->GetStringLoadCaseLsd(eLSD_LOADCASE_SD, TRUE);
		else
			str = pData->GetStringLoadCase(BRIDGE_LOAD_SUBSID);

		CString strLoad = _T("");
		// 각지점별로 땡기고...
		long i=0; for(i=0; i<m_nCountSubsidence; i++)
		{
			strLoad.Format("%s%d",str,i+1);
			pFEM->AddCombo(str,strLoad,1,0,1.0,FALSE);
		}
	}
}

void CARcBridgeDesignFrame::MakeSapDataCombineSoilPressure( CFEMManage *pFEM )
{
	CRcBridgeRebar		*pBri  = m_pStd->m_pBri;
	CARcBridgeDataStd   *pData = pBri->m_pARcBridgeDataStd;

	BOOL bLsd = pData->IsLsdDesign();
	BOOL bEnvToapOne = pBri->m_bSelectEnvToapOne;
	CString str(_T(""));

	// LSD가 아닌경우만 좌우측 토압을 콤보로 만든다.
	if(bLsd) return;

	if(bEnvToapOne && pBri->m_bApplyLoad[BRIDGE_LOAD_TOAP_ONE])
	{	
		str = pData->GetStringLoadCase(BRIDGE_LOAD_TOAP_ONE);
		pFEM->AddCombo(str, str+"L",1,0,1.0,FALSE);
		pFEM->AddCombo(str, str+"R",1,0,1.0,FALSE);
	}
}

void CARcBridgeDesignFrame::MakeSapDataHistoryNormal_Lsd( CFEMManage *pFEM )
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	if(pBri->IsBoxType() == FALSE) return;
	if(pBri->m_n2DGeneralSpringType == 1) return;

	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	CARcBridgeCalcLoad *pLoad = m_pStd->m_pLoad;

	m_bHistory[0] = TRUE;
	
	CString str = _T(""),strLoad = _T("");
	long nTypeHistory = 0; // LOAD
	long nStep = 100;
	CString szLR = _T("");
	double dFactor(0);

	for(long nL=0; nL<eLSD_COMBINE_TOTAL_COUNT; nL++)
	{	
		ELSDCombKind eKind = static_cast<ELSDCombKind>(nL);

		if(eKind == eLSD_COMBINE_EXTREME_I || eKind == eLSD_COMBINE_EXTREME_II)
			continue;

		std::vector<CLsdLoadCase *> LCArray = pBri->m_plsdManager->GetLsdLoadCaseArray(eKind,eLSD_LOADCASE_DIR_BRI);
		for(auto ite=LCArray.begin(); ite!=LCArray.end(); ++ite)
		{
			str = _T("H") + (*ite)->GetName();

			pFEM->AddHistory(str, _T(""), nTypeHistory, nStep, 1.0, 0.99, TRUE);

			for(long n=0; n<eLSD_LOADCASE_SIZE; ++n)
			{
				ELSDLoadCaseKind eCase = static_cast<ELSDLoadCaseKind>(n);
				if(pBri->m_plsdManager->IsSafeValidLoad(eCase) == FALSE)
					continue;

				strLoad = pBri->m_plsdManager->GetDesignNameByIndex(eCase);
				if(pFEM->GetLoadCaseIndex(strLoad) == -1)
					continue;

				if(pLoad->m_bExistLane && (eCase == eLSD_LOADCASE_LL1 || eCase == eLSD_LOADCASE_LL2))
					continue;
				else if(eCase == eLSD_LOADCASE_SD)
					continue;

				if(pBri->m_plsdManager->IsValidColumn(eKind, eLSD_LOADCASE_DIR_BRI, eCase) == FALSE)
					continue;

				// (ARCBRIDGE-2865) 
				// dFactor = pBri->m_plsdManager->GetLsdLoadCaseFactor(eKind, eLSD_LOADCASE_DIR_BRI, (*ite)->GetIndexCombo(), eCase);
				dFactor = (*ite)->GetLoadFactor(eCase);
				if(dFactor != 0)
				{
					pFEM->AddHistory_Load(str,strLoad,_T(""),dFactor);
				}
			}
		}
	}
}

void CARcBridgeDesignFrame::MakeSapDataMode_Lsd( CFEMManage *pFEM, BOOL bEarthQuake )
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	if(pBri->IsBoxType() == FALSE) return;
	if(bEarthQuake == FALSE && pBri->m_n2DGeneralSpringType == 1) return;
	if(bEarthQuake == TRUE && pBri->m_n2DEQSpringType == 1) return;

	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	// MODE
	long nTypeMode  = 0; // RITZ
	long nCountMode = 100;

	pFEM->m_arrMode.RemoveAll();
	pFEM->AddMode(nTypeMode, nCountMode, 0, 0, 0, _T("*"));

	for(long n=0; n<eLSD_LOADCASE_SIZE; ++n)
	{
		ELSDLoadCaseKind eCase = static_cast<ELSDLoadCaseKind>(n);
		if(pBri->m_plsdManager->IsSafeValidLoad(eCase) == FALSE)
			continue;

		CString strLoad = pBri->m_plsdManager->GetDesignNameByIndex(eCase);
		if(pFEM->GetLoadCaseIndex(strLoad) == -1)
			continue;

		ELSDLoadSelectKind eSelect = pBri->m_plsdManager->GetLoadSelectKindFromLoadCaseKind(eCase);
		if(bEarthQuake)
		{
			if(eSelect == eLSD_LOADSELECT_LL1)
				continue;
			// ARCBRIDGE-2755
		}
		else
		{
			//BOOL bApply = (eSelect == eLSD_LOADSELECT_DC || eSelect == eLSD_LOADSELECT_DW || eSelect == eLSD_LOADSELECT_EH || eSelect == eLSD_LOADSELECT_EV || eSelect == eLSD_LOADSELECT_WA);
			//if(bApply == FALSE)
			//	continue;

			// (ARCBRIDGE-3197) Mode에 위의 하중만 들어가도록 되어있었는데 모든 하중 다들어가도록 변경
			if(eSelect == eLSD_LOADSELECT_LL1)
				continue;
		}

		pFEM->m_arrMode.Add(strLoad);
	}
}

void CARcBridgeDesignFrame::MakeSapDataLoadEQ_Lsd( CFEMManage *pFEM, BOOL bLeftWallToap )
{
	CRcBridgeRebar		*pBri  = m_pStd->m_pBri;
	CARcBridgeDataStd   *pData = pBri->m_pARcBridgeDataStd;

	if(pBri->IsBoxType()) return;

	long nLoadCaseIndex = 0;

	//	double dSelfWeight = 1.0;
	
	long nIdxSlabStt = GetNumberElementSlabStt(pFEM);
	long nIdxSlabEnd = GetNumberElementSlabEnd(pFEM);

	CString strLoadName(_T(""));
	CString strLoadMark(_T(""));
	CARcBridgeCalcEarthQuake *pLoadEQ = m_pStd->m_pLoadEQ;

	// 고정하중 DC / DW
	// 연직토압 EV
	MakeSapDataLoadDead_Lsd(pFEM, nLoadCaseIndex, TRUE);

	// 활하중 (지중라멘교 : 상재하중)
	// 지중라멘일때 상재 활하중만 태워줬었는데 이동하중도 태워 주도록 변경
	if(pBri->m_plsdManager->IsUsedLoadCaseAtComboination(eLSD_COMBINATION_EXTREME, eLSD_LOADCASE_LL1))
	{
		MakeSapDataLoadLiveLoadAll(pFEM, nLoadCaseIndex, TRUE);
	}
	if(pBri->m_plsdManager->IsUsedLoadCaseAtComboination(eLSD_COMBINATION_EXTREME, eLSD_LOADCASE_LS))
	{
		MakeSapDataLoadLS_Lsd(pFEM, nLoadCaseIndex, TRUE);
	}
	if(pBri->m_plsdManager->IsUsedLoadCaseAtComboination(eLSD_COMBINATION_EXTREME, eLSD_LOADCASE_ES))
	{
		MakeSapDataLoadES_Lsd(pFEM, nLoadCaseIndex, TRUE);
	}
	if(pBri->m_plsdManager->IsUsedLoadCaseAtComboination(eLSD_COMBINATION_EXTREME, eLSD_LOADCASE_PL))
	{
		MakeSapDataLoadLiveFootWay(pFEM, nLoadCaseIndex, TRUE);
	}

	// 지진하중
	if(pBri->m_plsdManager->IsSelectedLoadDesign(eLSD_LOADSELECT_EQ))
	{
		double dLoadValue = GetValueUnitChange(pLoadEQ->m_Pe[CALC_DIR_X],UNIT_CGS_TONF_M,pData->m_szTonf_M);

		strLoadName = pBri->m_plsdManager->GetStringLoadCaseLsd(bLeftWallToap? eLSD_LOADCASE_EQ11 : eLSD_LOADCASE_EQ12, FALSE);
		strLoadMark  = pBri->m_plsdManager->GetStringLoadCaseLsd(bLeftWallToap? eLSD_LOADCASE_EQ11 : eLSD_LOADCASE_EQ12, TRUE);

		pFEM->MakeElementLoadForce(nLoadCaseIndex+1);
		pFEM->m_szLCName.SetAt(nLoadCaseIndex, strLoadMark);
		pFEM->m_szLCComment.SetAt(nLoadCaseIndex, strLoadName);
		for(long nIdx=nIdxSlabStt; nIdx<=nIdxSlabEnd; nIdx++)
		{
			CElement *pE = pFEM->GetElement((USHORT)nIdx);
			CLoadForce *pELoad = pE->GetLoadForce(nLoadCaseIndex);
			pELoad->m_szLoadTitle = strLoadMark;
			if(bLeftWallToap)
				pELoad->SetLoadForce(0, 1.0, CVector(dLoadValue,0,0), CVector(dLoadValue,0,0));
			else
				pELoad->SetLoadForce(0, 1.0, CVector(-dLoadValue,0,0), CVector(-dLoadValue,0,0));
		}		
		++nLoadCaseIndex;
	}

	if(pBri->m_plsdManager->IsSelectedLoadDesign(eLSD_LOADSELECT_EH) && pBri->IsOutsideWall())
	{
		if(pBri->m_plsdManager->IsUsedLoadCaseAtComboination(eLSD_COMBINATION_EXTREME, eLSD_LOADCASE_EH))
		{
			MakeSapDataEQLoadHoriSoilPress(pFEM, bLeftWallToap, nLoadCaseIndex);
		}
	}

	MakeSapDataLoadUserLoad(pFEM, nLoadCaseIndex, TRUE);
}

void CARcBridgeDesignFrame::MakeSapDataUnitLoadEQ( CFEMManage *pFEM )
{
	CRcBridgeRebar		*pBri  = m_pStd->m_pBri;
	CARcBridgeDataStd   *pData = pBri->m_pARcBridgeDataStd;

	if(pBri->IsBoxType()) return;

	long nLoadCaseIndex = 0;

	long nIdxSlabStt = GetNumberElementSlabStt(pFEM);
	long nIdxSlabEnd = GetNumberElementSlabEnd(pFEM);
	CString strLoad  = _T("LOAD-X");
	
	double dLoadValue = (pBri->m_nSelectModelingCountEQ==1) ? -1.0 : 1.0;

	pFEM->MakeElementLoadForce(nLoadCaseIndex+1);
	pFEM->m_szLCName.SetAt(nLoadCaseIndex, strLoad);
	pFEM->m_szLCComment.SetAt(nLoadCaseIndex, "종방향 단위하중");
	for(long nIdx=nIdxSlabStt; nIdx<=nIdxSlabEnd; nIdx++)
	{
		CElement *pE = pFEM->GetElement((USHORT)nIdx);
		CLoadForce *pELoad = pE->GetLoadForce(nLoadCaseIndex);
		pELoad->m_szLoadTitle = strLoad;
		pELoad->SetLoadForce(0, 1.0, CVector(dLoadValue,0,0), CVector(dLoadValue,0,0));
	}
}

void CARcBridgeDesignFrame::MakeSapDataEQLoadHoriSoilPress( CFEMManage *pFEM, BOOL bLeftWallToap, long &nLoadCaseIndex )
{
	CRcBridgeRebar		*pBri  = m_pStd->m_pBri;
	CARcBridgeDataStd   *pData = pBri->m_pARcBridgeDataStd;

	BOOL bLsd = pData->IsLsdDesign();
	CString strLoadName(_T(""));
	CString strLoadMark(_T(""));
	CARcBridgeCalcEarthQuake *pLoadEQ = m_pStd->m_pLoadEQ;
	double dLoadValue(0.0);

	if(bLsd)
	{
		strLoadName = pBri->m_plsdManager->GetStringLoadCaseLsd(bLeftWallToap? eLSD_LOADCASE_EH : eLSD_LOADCASE_EH2, FALSE);
		strLoadMark = pBri->m_plsdManager->GetStringLoadCaseLsd(bLeftWallToap? eLSD_LOADCASE_EH : eLSD_LOADCASE_EH2, TRUE);
	}
	else
	{
		strLoadName = _T("지진시 수평토압");
		strLoadMark = pData->GetStringLoadCase(EARTHQUAKE_LOAD_TOAP, TRUE);
	}

	long nJijum = bLeftWallToap ? 0 : pBri->m_nQtyJigan;
	long nIdxToap = bLeftWallToap ? 0 : 1;
	if(pBri->GetTypeSoilPressureEQ()==0)
	{
		CElement *pEle = GetElementWallFix(pFEM,nJijum);
		double dLengthEle  = pFEM->GetLengthElement(pEle);
		double dHeightToap = bLsd? frM(pLoadEQ->m_HeightToap[nIdxToap]) : pLoadEQ->m_HeightToap[nIdxToap];
		double dRDist = 1.0 - dHeightToap/dLengthEle;
		if(dHeightToap>dLengthEle)
		{
			double dHeightSum = dLengthEle;
			long nIdxEleFix = pEle->m_nIndex-1;
			for(long nEle=nIdxEleFix; nEle>=0; nEle--)
			{
				pEle = pFEM->GetElement((USHORT)nEle);
				dLengthEle  = pFEM->GetLengthElement(pEle);
				dHeightSum += pFEM->GetLengthElement(pEle);
				if(dHeightSum>=dHeightToap)
				{
					dRDist = 1.0 - (dHeightToap-(dHeightSum-dLengthEle))/dLengthEle;
					break;
				}
			}
		}
		dLoadValue = GetValueUnitChange(pLoadEQ->m_ToapEQ[nIdxToap] ,UNIT_CGS_TONF,pData->m_szTonf) * (bLeftWallToap? 1 : -1);

		pFEM->MakeElementLoadForce(nLoadCaseIndex+1);
		pFEM->m_szLCName.SetAt(nLoadCaseIndex, strLoadMark);
		pFEM->m_szLCComment.SetAt(nLoadCaseIndex, strLoadName);
		CLoadForce *pELoad = pEle->GetLoadForce(nLoadCaseIndex);
		pELoad->m_szLoadTitle = strLoadMark;
		pELoad->SetDistForce(dRDist,CVector(dLoadValue,0,0));
	}
	else
	{
		long i=0; for(i=0; i<2; i++)
		{
			// i == 0 : 평상시 토압 Pa
			// i == 1 : 지진시 토압 ΔPae
			CElement *pEle = GetElementWallFix(pFEM,nJijum);
			double dHeightToap(0);
			if(i==0)
				dHeightToap = bLsd? frM(pLoadEQ->m_HeightWall[nIdxToap]/3) : (pLoadEQ->m_HeightWall[nIdxToap]/3);
			else
				dHeightToap = bLsd? frM(pLoadEQ->m_HeightWall[nIdxToap]*0.6) : (pLoadEQ->m_HeightWall[nIdxToap]*0.6);
			double dLengthEle  = pFEM->GetLengthElement(pEle);
			double dRDist = 1.0 - dHeightToap/dLengthEle;
			if(dHeightToap>dLengthEle)
			{
				double dHeightSum = dLengthEle;
				long nIdxEleFix = pEle->m_nIndex-1;
				for(long nEle=nIdxEleFix; nEle>=0; nEle--)
				{
					pEle = pFEM->GetElement((USHORT)nEle);
					dLengthEle  = pFEM->GetLengthElement(pEle);
					dHeightSum += pFEM->GetLengthElement(pEle);
					if(dHeightSum>=dHeightToap)
					{
						dRDist = 1.0 - (dHeightToap-(dHeightSum-dLengthEle))/dLengthEle;
						break;
					}
				}
			}
			dLoadValue = (i==0) ? pLoadEQ->m_ToapNormal[nIdxToap] : pLoadEQ->m_ToapEQ[nIdxToap] - pLoadEQ->m_ToapNormal[nIdxToap];
			dLoadValue = GetValueUnitChange(dLoadValue ,UNIT_CGS_TONF,pData->m_szTonf) * (bLeftWallToap? 1 : -1);
			
			if(i == 0)
				pFEM->MakeElementLoadForce(nLoadCaseIndex+1);

			pFEM->m_szLCName.SetAt(nLoadCaseIndex, strLoadMark);
			pFEM->m_szLCComment.SetAt(nLoadCaseIndex, strLoadName);
			CLoadForce *pELoad = pEle->GetLoadForce(nLoadCaseIndex);
			pELoad->m_szLoadTitle = strLoadMark;
			pELoad->SetDistForce(dRDist,CVector(dLoadValue,0,0));
		}
	}
}

void CARcBridgeDesignFrame::MakeSapDataLoadEQ_BoxType_Lsd( CFEMManage *pFEM, EGoalOfEarthQuake eGoalEq, long nTypeDesign )
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
// 	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	if(pBri->IsBoxType() == FALSE) return;

	long nLoadCaseIndex = 0;
	
	// 고정하중 DC / DW
	// 연직토압 EV
	MakeSapDataLoadDead_Lsd(pFEM, nLoadCaseIndex, TRUE);

	// 구체관성력
	if(pBri->m_plsdManager->IsSelectedLoadDesign(eLSD_LOADSELECT_EQ))
	{
		MakeSapDataEQLoadInertialForce(pFEM, eGoalEq, nLoadCaseIndex);
	}

	// (40669) 양토압과 편도압이 모두 없을때만 안들어가도록
	MakeSapDataLoadHoriSoilPress_Lsd(pFEM, nLoadCaseIndex, TRUE, eGoalEq);

	// 수평수압
	// 연직수압
	MakeSapDataLoadWater(pFEM, nLoadCaseIndex, TRUE);

	// 활하중 (지중라멘교 : 상재하중)
	// 지중라멘일때 상재 활하중만 태워줬었는데 이동하중도 태워 주도록 변경
	if(pBri->m_plsdManager->IsUsedLoadCaseAtComboination(eLSD_COMBINATION_EXTREME, eLSD_LOADCASE_LL1))
	{
		MakeSapDataLoadLiveLoadAll(pFEM, nLoadCaseIndex, TRUE);
	}
	if(pBri->m_plsdManager->IsUsedLoadCaseAtComboination(eLSD_COMBINATION_EXTREME, eLSD_LOADCASE_LS))
	{
		MakeSapDataLoadLS_Lsd(pFEM, nLoadCaseIndex, TRUE);
	}
	if(pBri->m_plsdManager->IsUsedLoadCaseAtComboination(eLSD_COMBINATION_EXTREME, eLSD_LOADCASE_ES))
	{
		MakeSapDataLoadES_Lsd(pFEM, nLoadCaseIndex, TRUE);
	}
	if(pBri->m_plsdManager->IsUsedLoadCaseAtComboination(eLSD_COMBINATION_EXTREME, eLSD_LOADCASE_PL))
	{
		MakeSapDataLoadLiveFootWay(pFEM, nLoadCaseIndex, TRUE);
	}
	
	MakeSapDataLoadUserLoad(pFEM, nLoadCaseIndex, TRUE, nTypeDesign);
}

void CARcBridgeDesignFrame::MakeSapDataLoadDead_Lsd( CFEMManage *pFEM, long &nLoadCaseIndex, BOOL bEarthQuake )
{
	CRcBridgeRebar		*pBri  = m_pStd->m_pBri;
	CARcBridgeDataStd   *pData = pBri->m_pARcBridgeDataStd;

	CARcBridgeCalcLoad	*pLoad = m_pStd->m_pLoad;
	CARcBridgeCalcEarthQuake *pLoadEQ = m_pStd->m_pLoadEQ;

	double dLoadValue(0);

	long nIdxSlabStt = GetNumberElementSlabStt(pFEM);
	long nIdxSlabEnd = GetNumberElementSlabEnd(pFEM);

	CString strLoadName(_T(""));
	CString strLoadMark(_T(""));

	// 고정하중 DC / DW
	for(long ix=0; ix<2; ++ix)
	{
		BOOL bDW = (ix==0)? FALSE : TRUE;
		ELSDLoadSelectKind eLoadSelect = bDW? eLSD_LOADSELECT_DW : eLSD_LOADSELECT_DC;

		if(bEarthQuake)
			dLoadValue  = GetValueUnitChange(pLoadEQ->m_DeadLoadDistributionLsd[bDW? 1 : 0],UNIT_CGS_TONF_M,pData->m_szTonf_M);
		else
			dLoadValue  = GetValueUnitChange(pLoad->m_DeadLoadDistribution[bDW? 1 : 0],UNIT_CGS_TONF_M2,pData->m_szTonf_M2);

		if(bDW && dLoadValue <= 0)
			continue;

		if(pBri->m_plsdManager->IsSelectedLoadDesign(eLoadSelect))
		{
			strLoadName = pBri->m_plsdManager->GetStringSelectLoadLsd(eLoadSelect, FALSE);
			strLoadMark  = pBri->m_plsdManager->GetStringSelectLoadLsd(eLoadSelect, TRUE);

			pFEM->MakeElementLoadForce(nLoadCaseIndex+1);

			pFEM->m_szLCName.SetAt(nLoadCaseIndex, strLoadMark);
			pFEM->m_szLCComment.SetAt(nLoadCaseIndex, strLoadName);
			if(bDW == FALSE)
			{
				double dSelfWeight = 1.0;
				pFEM->m_dSelfWeight.SetAt(nLoadCaseIndex,dSelfWeight);	
			}
			for(long nIdx=nIdxSlabStt; nIdx<=nIdxSlabEnd; nIdx++)
			{
				if(dLoadValue==0) break;
				CElement *pE = pFEM->GetElement((USHORT)nIdx);
				CLoadForce *pELoad = pE->GetLoadForce(nLoadCaseIndex);
				pELoad->m_szLoadTitle = strLoadMark;
				pELoad->SetLoadForce(0, 1.0, CVector(0,0,-dLoadValue), CVector(0,0,-dLoadValue));
			}
			++nLoadCaseIndex;
		}
	}

	// 연직토압 EV
	if(pBri->m_plsdManager->IsSelectedLoadDesign(eLSD_LOADSELECT_EV))
	{
		BOOL bUnderGroundWater = pBri->m_bApplyGroundWaterLevel;
		double dWidth = bEarthQuake ? frM(m_pStd->GetSlabWidth()) : 1.0;

		for(long ixW=0; ixW<2; ++ixW)
		{
			BOOL bWater = (ixW == 1)? TRUE : FALSE;
			if(bWater && !bUnderGroundWater) continue;

			strLoadName = pBri->m_plsdManager->GetStringLoadCaseLsd(bWater? eLSD_LOADCASE_EVW : eLSD_LOADCASE_EV, FALSE);
			strLoadMark  = pBri->m_plsdManager->GetStringLoadCaseLsd(bWater? eLSD_LOADCASE_EVW : eLSD_LOADCASE_EV, TRUE);

			if(bEarthQuake)
				dLoadValue  = GetValueUnitChange(pLoadEQ->m_DeadLoadSoil	,UNIT_CGS_TONF_M,pData->m_szTonf_M);	// m_DeadLoadSoil에는 이미 dWidth가 적용되어있다.
			else
				dLoadValue  = GetValueUnitChange(pLoad->m_dSoilPressureVert	,UNIT_CGS_TONF_M2,pData->m_szTonf_M2);

			if(dLoadValue > 0)
			{
				pFEM->MakeElementLoadForce(nLoadCaseIndex+1);

				pFEM->m_szLCName.SetAt(nLoadCaseIndex, strLoadMark);
				pFEM->m_szLCComment.SetAt(nLoadCaseIndex, strLoadName);

				for(long nIdx=nIdxSlabStt; nIdx<=nIdxSlabEnd; nIdx++)
				{
					if(dLoadValue==0) break;
					CElement *pE = pFEM->GetElement((USHORT)nIdx);
					CLoadForce *pELoad = pE->GetLoadForce(nLoadCaseIndex);
					pELoad->m_szLoadTitle = strLoadMark;
					pELoad->SetLoadForce(0, 1.0, CVector(0,0,-dLoadValue), CVector(0,0,-dLoadValue));
				}

				++nLoadCaseIndex;
			}

			if(pBri->IsBoxType() && pBri->m_bExistProtectionRiseFooting)
			{
				if(dLoadValue <= 0)
				{
					pFEM->MakeElementLoadForce(nLoadCaseIndex+1);

					pFEM->m_szLCName.SetAt(nLoadCaseIndex, strLoadMark);
					pFEM->m_szLCComment.SetAt(nLoadCaseIndex, strLoadName);
				}

				// 부상방지 저판에 작용하는 연직토압
				double dHStt = pFEM->GetJoint(0)->m_vPointReal.z;
				double dHEnd = dHStt + frM(m_pStd->GetElDiffModelingEnd());
				double dDistStt = pFEM->GetJoint((USHORT)nIdxSlabStt)->m_vPointOrigin.x;
				double dDistEnd = pFEM->GetJoint((USHORT)(nIdxSlabEnd + 1))->m_vPointOrigin.x;

				double nSizeEle = pFEM->GetElementSize();
				for(long nIdx = 0; nIdx < nSizeEle; nIdx++)
				{
					CElement *pE = pFEM->GetElement((USHORT)nIdx);
					if(pE->m_nFlag != ELEMENT_LOWERSLAB) continue;
					CJoint *pJntI = pFEM->GetJoint(pE->m_nJStt);
					CJoint *pJntJ = pFEM->GetJoint(pE->m_nJEnd);

					// 시점 부상방지 저판
					if(pJntI->m_vPointOrigin.x < dDistStt)
					{
						double dVertSoil = GetValueUnitChange(pLoad->GetSoilPressureForProtectionRiseFooting(TRUE, bWater, FALSE, TRUE), UNIT_CGS_TONF_M2, pData->m_szTonf_M2);
						if(bWater)
						{
							dVertSoil += GetValueUnitChange(pLoad->GetSoilPressureForProtectionRiseFooting(TRUE, bWater, TRUE, FALSE), UNIT_CGS_TONF_M2, pData->m_szTonf_M2);
						}
						if(dVertSoil==0) continue;

						CElement *pE = pFEM->GetElement((USHORT)nIdx);
						CLoadForce *pELoad = pE->GetLoadForce(nLoadCaseIndex);
						pELoad->m_szLoadTitle = strLoadMark;
						pELoad->SetLoadForce(0, 1.0, CVector(0,0,-dVertSoil * dWidth), CVector(0,0,-dVertSoil * dWidth));
					}
					else if(pJntJ->m_vPointOrigin.x > dDistEnd)
					{
						double dVertSoil = GetValueUnitChange(pLoad->GetSoilPressureForProtectionRiseFooting(FALSE, bWater, FALSE, TRUE), UNIT_CGS_TONF_M2, pData->m_szTonf_M2);
						if(bWater)
						{
							dVertSoil += GetValueUnitChange(pLoad->GetSoilPressureForProtectionRiseFooting(FALSE, bWater, TRUE, FALSE), UNIT_CGS_TONF_M2, pData->m_szTonf_M2);
						}
						if(dVertSoil==0) continue;
						
						CElement *pE = pFEM->GetElement((USHORT)nIdx);
						CLoadForce *pELoad = pE->GetLoadForce(nLoadCaseIndex);
						pELoad->m_szLoadTitle = strLoadMark;
						pELoad->SetLoadForce(0, 1.0, CVector(0,0,-dVertSoil * dWidth), CVector(0,0,-dVertSoil * dWidth));
					}
				}

				++nLoadCaseIndex;
			}
		}
	}
}

void CARcBridgeDesignFrame::MakeSapDataEQLoadInertialForce( CFEMManage *pFEM, EGoalOfEarthQuake eGoalEq, long &nLoadCaseIndex )
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	double dLoadValue  = 1.0;
	CString strLoadName(_T(""));
	CString strLoadMark(_T(""));

	BOOL bLsd = pData->IsLsdDesign();
	long nSizeEle    = pFEM->GetElementSize();
	long nIdxSlabStt = GetNumberElementSlabStt(pFEM, TRUE);
	long nIdxSlabEnd = GetNumberElementSlabEnd(pFEM, TRUE);

	CARcBridgeCalcEarthQuake *pLoadEQ = m_pStd->m_pLoadEQ;

	for(long nLR = iLEFT; nLR <= iRIGHT; nLR++)
	{
		BOOL bLeft = (nLR == iLEFT)? TRUE : FALSE;

		if(bLsd)
		{
			strLoadName = pBri->m_plsdManager->GetStringLoadCaseLsd(bLeft? eLSD_LOADCASE_EQ21 : eLSD_LOADCASE_EQ22, FALSE);
			strLoadMark  = pBri->m_plsdManager->GetStringLoadCaseLsd(bLeft? eLSD_LOADCASE_EQ21 : eLSD_LOADCASE_EQ22, TRUE);
		}
		else
		{
			strLoadMark.Format(_T("%s%s"), pData->GetStringLoadCase(EARTHQUAKE_LOAD_INERTIAL, TRUE), nLR==iLEFT ? _T("L") : _T("R"));
			strLoadName = _T("구체 관성력");
		}

		pFEM->MakeElementLoadForce(nLoadCaseIndex + 1);
		pFEM->m_szLCName.SetAt(nLoadCaseIndex, strLoadMark);
		pFEM->m_szLCComment.SetAt(nLoadCaseIndex, strLoadName);

		double dSW = nLR == iLEFT ? +1.0 : -1.0;
		// 슬래브
		for(long iUD = iUPPER; iUD <= iLOWER; iUD++)
		{
			dLoadValue = GetValueUnitChange(pLoadEQ->m_InertialForce_Slab[eGoalEq][iUD],UNIT_CGS_TONF_M,pData->m_szTonf_M) * dSW;
			nIdxSlabStt = GetNumberElementSlabStt(pFEM, iUD == iUPPER);
			nIdxSlabEnd = GetNumberElementSlabEnd(pFEM, iUD == iUPPER);
			for(long nIdx=nIdxSlabStt; nIdx<=nIdxSlabEnd; nIdx++)
			{
				CElement *pE = pFEM->GetElement((USHORT)nIdx);
				CLoadForce *pELoad = pE->GetLoadForce(nLoadCaseIndex);
				pELoad->m_szLoadTitle = strLoadMark;
				pELoad->SetLoadForce(0, 1.0, CVector(dLoadValue, 0, 0), CVector(dLoadValue, 0, 0));
			}
		}
		// 벽체
		for(long iSE = iSTT; iSE <= iEND; iSE++)
		{
			long i=0; for(i=0; i<nSizeEle; i++)
			{
				CElement *pE = pFEM->GetElement((USHORT)i);	
				if(iSE == iSTT && pE->m_nFlag != ELEMENT_STTWALL) continue;
				if(iSE == iEND && pE->m_nFlag != ELEMENT_ENDWALL) continue;

				CLoadForce *pELoad = pE->GetLoadForce(nLoadCaseIndex);
				dLoadValue = GetValueUnitChange(pLoadEQ->m_InertialForce_Wall[eGoalEq][iSE],UNIT_CGS_TONF_M,pData->m_szTonf_M) * dSW;
				pELoad->m_szLoadTitle = strLoadMark;
				pELoad->SetLoadForce(0, 1.0, CVector(dLoadValue, 0, 0), CVector(dLoadValue, 0, 0));
			}
		}
		++nLoadCaseIndex;
	}
}

void CARcBridgeDesignFrame::MakeSapDataLoadHoriSoilPress_Lsd( CFEMManage *pFEM, long &nLoadCaseIndex, BOOL bEarthQuake, EGoalOfEarthQuake eGoalEq )
{
	CRcBridgeRebar		*pBri  = m_pStd->m_pBri;
	CARcBridgeCalcLoad	*pLoad = m_pStd->m_pLoad;
	CARcBridgeDataStd   *pData = pBri->m_pARcBridgeDataStd;

	//////////////////////////////////////////////////////////////////////////
	long nSizeEle    = pFEM->GetElementSize();

	BOOL bOutWall = pBri->IsOutsideWall();
	BOOL bUnderGroundWater = pBri->m_bApplyGroundWaterLevel;

	CString szUnitCGS = bEarthQuake? UNIT_CGS_TONF_M : UNIT_CGS_TONF_M2;
	CString szUnitApply = bEarthQuake? pData->m_szTonf_M : pData->m_szTonf_M2;
	double dWidth = bEarthQuake? m_pStd->GetSlabWidth(-1) : 1;

	CString strLoadName(_T(""));
	CString strLoadMark(_T(""));

	if(bOutWall == FALSE)
		return;
	if(pBri->m_plsdManager->IsSelectedLoadDesign(eLSD_LOADSELECT_EH) == FALSE)
		return;

	for(long ixW=0; ixW<2; ++ixW)
	{
		BOOL bWater = (ixW == 1)? TRUE : FALSE;
		if(bWater && !bUnderGroundWater) continue;

		for(long ixDir=0; ixDir<2; ++ixDir)
		{
			BOOL bLeft = (ixDir == iLEFT);
			ELSDLoadCaseKind eLoadCase(eLSD_LOADCASE_EH);

			if(bWater)
				eLoadCase = bLeft? eLSD_LOADCASE_EHW1 : eLSD_LOADCASE_EHW2;
			else
				eLoadCase = bLeft? eLSD_LOADCASE_EH : eLSD_LOADCASE_EH2;

			strLoadName = pBri->m_plsdManager->GetStringLoadCaseLsd(eLoadCase, FALSE);
			strLoadMark  = pBri->m_plsdManager->GetStringLoadCaseLsd(eLoadCase, TRUE);

			double dHStt = bLeft ? pFEM->GetJoint(0)->m_vPointOrigin.z : pFEM->GetJoint(0)->m_vPointOrigin.z + frM(m_pStd->GetElDiffModelingEnd());

			pFEM->MakeElementLoadForce(nLoadCaseIndex+1);
			pFEM->m_szLCName.SetAt(nLoadCaseIndex, strLoadMark);
			pFEM->m_szLCComment.SetAt(nLoadCaseIndex, strLoadName);

			long i=0; for(i=0; i<nSizeEle; i++)
			{
				CElement *pE = pFEM->GetElement((USHORT)i);	
				if(bLeft && pE->m_nFlag != ELEMENT_STTWALL) continue;
				if(!bLeft && pE->m_nFlag != ELEMENT_ENDWALL) continue;

				CLoadForce *pELoad = pE->GetLoadForce(nLoadCaseIndex);
				CJoint *pJntI = pFEM->GetJoint(pE->m_nJStt);
				CJoint *pJntJ = pFEM->GetJoint(pE->m_nJEnd);

				double dToapUp = pLoad->GetLoadSoilPressure(toM(dHStt - pJntI->m_vPointOrigin.z), bLeft, bWater, bEarthQuake, eGoalEq) * dWidth;
				double dToapDn = pLoad->GetLoadSoilPressure(toM(dHStt - pJntJ->m_vPointOrigin.z), bLeft, bWater, bEarthQuake, eGoalEq) * dWidth;
				dToapUp = GetValueUnitChange(dToapUp, szUnitCGS, szUnitApply) * (bLeft? 1 : -1);
				dToapDn = GetValueUnitChange(dToapDn, szUnitCGS, szUnitApply) * (bLeft? 1 : -1);

				pELoad->m_szLoadTitle = strLoadMark;
				pELoad->SetLoadForce(0, 1.0, CVector(dToapUp,0,0), CVector(dToapDn,0,0));		
			}

			++nLoadCaseIndex;
		}
	}
}

void CARcBridgeDesignFrame::MakeSapDataCombineEQ_Lsd( CFEMManage *pFEM, BOOL bUnitLoad, BOOL bLeftWallToap, long nTypeDesign )
{
	CRcBridgeRebar		*pBri  = m_pStd->m_pBri;
//	CARcBridgeDataStd   *pData = pBri->m_pARcBridgeDataStd;

	CString strCombo = _T(""),strLoad = _T(""); 
	CString str(_T(""));

	if(bUnitLoad)
	{
		pFEM->AddCombo("UNIT","LOAD-X",0,0,1.0,FALSE);
	}
	else
	{
		long nKind = 1;

		if(pBri->IsBoxType())
		{
			nKind = pBri->GetEarthQuakeGoalByIndex(nTypeDesign-1) + 1;
		}
		
		BOOL bComboLR = pBri->IsBoxType() ? TRUE : FALSE;

		if(pBri->m_plsdManager->IsUsedLoadCaseAtComboination(eLSD_COMBINATION_EXTREME, eLSD_LOADCASE_LL1))
		{
			MakeSapDataCombineLiveLoad(pFEM, TRUE);
		}

		for(long nL=eLSD_COMBINE_EXTREME_I; nL<=eLSD_COMBINE_EXTREME_II; nL++)
		{	
			ELSDCombKind eKind = static_cast<ELSDCombKind>(nL);

			std::vector<CLsdLoadCase *> LCArray = pBri->m_plsdManager->GetLsdLoadCaseArray(eKind,eLSD_LOADCASE_DIR_BRI);
			for(auto ite=LCArray.begin(); ite!=LCArray.end(); ++ite)
			{
				str = (*ite)->GetName();

				if(bComboLR == FALSE)
				{
					if(str.Find(bLeftWallToap? _T("NR") : _T("NL")) != -1)
						continue;
					if(str.Find(bLeftWallToap? _T("WR") : _T("WL")) != -1)
						continue;
				}

				// SHELL 조합은 선택된 것만...
				//if(bEarthQuake && pBri->m_bApplyCombo3D[nComboCase][i]==FALSE) continue;
				if(m_bHistory[1])
				{
					strLoad = _T("H") + str;
					pFEM->AddCombo(str,strLoad, 0, 3, 1, FALSE);
				}

				for(long n=0; n<eLSD_LOADCASE_SIZE; ++n)
				{
					ELSDLoadCaseKind eCase = static_cast<ELSDLoadCaseKind>(n);
					if(pBri->m_plsdManager->IsSafeValidLoad(eCase) == FALSE)
						continue;

					if(pBri->m_plsdManager->IsValidColumn(eKind, eLSD_LOADCASE_DIR_BRI, eCase) == FALSE)
						continue;

					strLoad = pBri->m_plsdManager->GetDesignNameByIndex(eCase);
					long nType = 0;
					if(eCase == eLSD_LOADCASE_LL1 || eCase == eLSD_LOADCASE_LL2)
					{
						nType = (!pBri->m_bUnderGroundRahmen || pBri->m_bCalcOpen)? 1 : 0;
					}
					else
					{
						if(pFEM->GetLoadCaseIndex(strLoad) == -1)
							continue;
					}

					if(m_bHistory[1] && nType == 0)
						continue;
					
					double dFactor = (*ite)->GetLoadFactor(eCase);
					if(dFactor != 0)
					{
						pFEM->AddCombo(str,strLoad,0,nType,dFactor,FALSE);
					}
				}

				// 사용자하중
				long nSizeUserLoad = pBri->m_vpUserLoadSet[nKind].size();
				for(long n=0; n<nSizeUserLoad; ++n)
				{
					CBridgeUserLoadSet *pLoadSet = pBri->m_vpUserLoadSet[nKind][n];

					if(pLoadSet->m_pArrUserLoad.GetSize() > 0)
					{
						long nIndexELSDLoadCaseKind = 0;
						CString sLoadCase = pLoadSet->m_sTypeLoadCase;
						for (long iC=0; iC<eLSD_LOADCASE_SIZE; ++iC)
						{
							if(sLoadCase==pBri->m_plsdManager->GetDesignNameByIndex(static_cast<ELSDLoadCaseKind>(iC)))
							{
								nIndexELSDLoadCaseKind = iC;
								break;
							}
						}

						ELSDLoadCaseKind eCase = static_cast<ELSDLoadCaseKind>(nIndexELSDLoadCaseKind);
						if(eCase!=-1)
						{
							strLoad.Format(_T("USER%d"), n+1);
							double dFactor = (*ite)->GetLoadFactor(eCase);
							if(dFactor != 0)
							{
								pFEM->AddCombo(str,strLoad,0,0,dFactor,FALSE);
							}
						}
					}
				}
			}
		}
	}
}

void CARcBridgeDesignFrame::MakeSapDataHistoryEarthquake_Lsd( CFEMManage *pFEM )
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	if(pBri->IsBoxType() == FALSE) return;
	if(pBri->m_n2DEQSpringType == 1) return;

	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	
	m_bHistory[1] = TRUE;

	CString str(_T("")), strLoad(_T(""));
	long nTypeHistory = 0; // LOAD
	long nStep = 100;
	double dFactor(0);

	for(long nL=eLSD_COMBINE_EXTREME_I; nL<=eLSD_COMBINE_EXTREME_II; nL++)
	{	
		ELSDCombKind eKind = static_cast<ELSDCombKind>(nL);

		std::vector<CLsdLoadCase *> LCArray = pBri->m_plsdManager->GetLsdLoadCaseArray(eKind,eLSD_LOADCASE_DIR_BRI);
		for(auto ite=LCArray.begin(); ite!=LCArray.end(); ++ite)
		{
			str = _T("H") + (*ite)->GetName();

			pFEM->AddHistory(str, _T(""), nTypeHistory, nStep, 1.0, 0.99, TRUE);

			for(long n=0; n<eLSD_LOADCASE_SIZE; ++n)
			{
				ELSDLoadCaseKind eCase = static_cast<ELSDLoadCaseKind>(n);
				if(pBri->m_plsdManager->IsSafeValidLoad(eCase) == FALSE)
					continue;

				if(pBri->m_plsdManager->IsValidColumn(eKind, eLSD_LOADCASE_DIR_BRI, eCase) == FALSE)
					continue;

				strLoad = pBri->m_plsdManager->GetDesignNameByIndex(eCase);
				if(pFEM->GetLoadCaseIndex(strLoad) == -1)
					continue;

				dFactor = (*ite)->GetLoadFactor(eCase);
				if(dFactor != 0)
				{
					pFEM->AddHistory_Load(str,strLoad,_T(""),dFactor);
				}
			}
		}
	}
}

void CARcBridgeDesignFrame::MakeSapDataLoadLS_Lsd( CFEMManage *pFEM, long &nLoadCaseIndex, BOOL bEarthQuake )
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeCalcLoad	*pLoad = m_pStd->m_pLoad;
	CARcBridgeDataStd   *pData = pBri->m_pARcBridgeDataStd;

	long nSizeElement = pFEM->GetElementSize();
	double dWidth = bEarthQuake ? frM(m_pStd->GetSlabWidth()) : 1.0;

	for(long ixDir=0; ixDir<2; ++ixDir)
	{
		BOOL bLeft = (ixDir == iLEFT);
		ELSDLoadCaseKind eLoadCase(eLSD_LOADCASE_LS);
		eLoadCase = bLeft? eLSD_LOADCASE_LS : eLSD_LOADCASE_LS2;

		CString strLoadName = pBri->m_plsdManager->GetStringLoadCaseLsd(eLoadCase, FALSE);
		CString strLoadMark  = pBri->m_plsdManager->GetStringLoadCaseLsd(eLoadCase, TRUE);

		double dLoadValue = GetValueUnitChange(pLoad->m_dLoadLsApply[ixDir]	,UNIT_CGS_TONF_M2,pData->m_szTonf_M2) * (bLeft? 1: -1) * dWidth;
		if(dLoadValue == 0)
			continue;

		pFEM->MakeElementLoadForce(nLoadCaseIndex+1);
		pFEM->m_szLCName.SetAt(nLoadCaseIndex, strLoadMark);
		pFEM->m_szLCComment.SetAt(nLoadCaseIndex, strLoadName);

		long i=0; for(i=0; i<nSizeElement; i++)
		{
			CElement *pE = pFEM->GetElement((USHORT)i);	
			if(bLeft && pE->m_nFlag != ELEMENT_STTWALL) continue;
			if(!bLeft && pE->m_nFlag != ELEMENT_ENDWALL) continue;

			CLoadForce *pELoad = pE->GetLoadForce(nLoadCaseIndex);
			pELoad->m_szLoadTitle = strLoadMark;
			pELoad->SetLoadForce(0, 1.0, CVector(dLoadValue,0,0), CVector(dLoadValue,0,0));
		}

		++nLoadCaseIndex;
	}
}

void CARcBridgeDesignFrame::MakeSapDataLoadES_Lsd( CFEMManage *pFEM, long &nLoadCaseIndex, BOOL bEarthQuake )
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeCalcLoad	*pLoad = m_pStd->m_pLoad;
	CARcBridgeDataStd   *pData = pBri->m_pARcBridgeDataStd;

	long nSizeElement = pFEM->GetElementSize();
	double dWidth = bEarthQuake ? frM(m_pStd->GetSlabWidth()) : 1.0;

	for(long ixDir=0; ixDir<2; ++ixDir)
	{
		BOOL bLeft = (ixDir == iLEFT);

		CString strLoadName = pBri->m_plsdManager->GetStringLoadCaseLsd(bLeft? eLSD_LOADCASE_ES : eLSD_LOADCASE_ES2, FALSE);
		CString strLoadMark  = pBri->m_plsdManager->GetStringLoadCaseLsd(bLeft? eLSD_LOADCASE_ES : eLSD_LOADCASE_ES2, TRUE);

		double dLoadValue = GetValueUnitChange(pLoad->GetLoadLiveAlwaysHori()	,UNIT_CGS_TONF_M2,pData->m_szTonf_M2) * (bLeft? 1: -1) * dWidth;
		if(dLoadValue == 0)
			continue;

		pFEM->MakeElementLoadForce(nLoadCaseIndex+1);
		pFEM->m_szLCName.SetAt(nLoadCaseIndex, strLoadMark);
		pFEM->m_szLCComment.SetAt(nLoadCaseIndex, strLoadName);

		long i=0; for(i=0; i<nSizeElement; i++)
		{
			CElement *pE = pFEM->GetElement((USHORT)i);	
			if(bLeft && pE->m_nFlag != ELEMENT_STTWALL) continue;
			if(!bLeft && pE->m_nFlag != ELEMENT_ENDWALL) continue;

			CLoadForce *pELoad = pE->GetLoadForce(nLoadCaseIndex);
			pELoad->m_szLoadTitle = strLoadMark;
			pELoad->SetLoadForce(0, 1.0, CVector(dLoadValue,0,0), CVector(dLoadValue,0,0));
		}

		++nLoadCaseIndex;
	}
}

void CARcBridgeDesignFrame::MakeSapDataCombineEnv_Lsd( CFEMManage *pFEM, BOOL bEarthQuake, BOOL b3DPlate )
{
	CRcBridgeRebar		*pBri  = m_pStd->m_pBri;
	
	CString str(_T("")), strEnv(_T("")), strDescription(_T(""));
	double dFactor(0);

	// 하중수정계수 종류에 따라 ENV 조합을 만든다.
	for(long ixPart=0; ixPart<ePartCount; ++ixPart)
	{
		if(pBri->m_bEtaCombinEach == FALSE && ixPart > 0)
			break;

		if(pBri->IsValidBridgePart((EPartOfBridge)ixPart) == FALSE)
			continue;
		
		if(pBri->IsUsePart((EPartOfBridge)ixPart, 1) == FALSE)
			continue;

		CStringArray szArrComboService;
		for(long nL=0; nL<eLSD_COMBINE_TOTAL_COUNT; nL++)
		{	
			ELSDCombKind eKind = static_cast<ELSDCombKind>(nL);
			if(eKind == eLSD_COMBINE_STRESS && ixPart != ePartUpperSlab)
				continue;

			strEnv = pBri->GetStringComboEnv_Lsd(eKind, (EPartOfBridge)ixPart);

			BOOL bExtreame = (eKind == eLSD_COMBINE_EXTREME_I || eKind == eLSD_COMBINE_EXTREME_II);
			if(bEarthQuake != bExtreame)
				continue;

			strDescription = pBri->GetStringComboEnv_Lsd(eKind, (EPartOfBridge)ixPart, FALSE);

			std::vector<CLsdLoadCase *> LCArray = pBri->m_plsdManager->GetLsdLoadCaseArray(eKind,eLSD_LOADCASE_DIR_BRI);
			for(auto ite=LCArray.begin(); ite!=LCArray.end(); ++ite)
			{
				str = (*ite)->GetName();
				// 해당 조합이 있는지 확인을....
				if(pFEM->GetComboByName(str) == NULL)
					continue;

				if(eKind >= eLSD_COMBINE_SERVICE_I && eKind <= eLSD_COMBINE_SERVICE_V)
				{
					szArrComboService.Add(str);
				}

				if(eKind == eLSD_COMBINE_SERVICE_II || eKind == eLSD_COMBINE_SERVICE_IV)
					continue;

				if(eKind == eLSD_COMBINE_SERVICE_III)
				{
					// 사용하중조합 Ⅲ + Ⅳ
					if((*ite)->GetLsdLoadComboKind() != eLSD_COMBINE_SERVICE_III && (*ite)->GetLsdLoadComboKind() != eLSD_COMBINE_SERVICE_IV)
						continue;
				}
				else
				{
					if((*ite)->GetLsdLoadComboKind() != eKind)
						continue;
				}

				dFactor = pBri->m_plsdManager->GetValueLoadModifiedFactor(eKind, ixPart, (*ite));
				pFEM->AddCombo(strEnv,str,1,1,dFactor,FALSE, strDescription);
			}

			// (ARCBRIDGE-3458) 균열검토 조합은 사용한계상태 모든 조함을 포함하도록 변경
// 			if(eKind == eLSD_COMBINE_SERVICE_I || eKind == eLSD_COMBINE_SERVICE_III || eKind == eLSD_COMBINE_SERVICE_V)
// 			{
// 				if(pFEM->GetComboByName(strEnv) != NULL)
// 				{
// 					szArrComboService.Add(strEnv);
// 				}
// 			}
		}
		if(szArrComboService.GetSize() > 0)
		{
			// 사용한계상태 균열판정용 조합을 만든다.
			CString strEnvCrack = pBri->GetStringComboEnv_Lsd(eLSD_COMBINE_SERVICE_I, (EPartOfBridge)ixPart, TRUE, TRUE);
			for(long ix=0; ix<szArrComboService.GetSize(); ++ix)
			{
				pFEM->AddCombo(strEnvCrack, szArrComboService.GetAt(ix),1,1,1,FALSE, _T("사용한계 균열검토"));
			}
		}
	}
}

void CARcBridgeDesignFrame::MakeSapDataCombineEtaCase_Lsd( CFEMManage *pFEM, BOOL bEarthQuake, BOOL b3DPlate )
{
	CRcBridgeRebar		*pBri  = m_pStd->m_pBri;

	CString str(_T("")), strEtaCombo(_T("")), strDescription1(_T("")), strDescription2(_T(""));;
	double dFactor(0);

	long nCountComboPart = 0;
	// 하중수정계수 종류에 따라 ENV 조합을 만든다.
	for(long ixPart=0; ixPart<ePartCount; ++ixPart)
	{
		if(pBri->m_bEtaCombinEach == FALSE && nCountComboPart > 0)
			break;

		if(pBri->IsUsePart((EPartOfBridge)ixPart, 2) == FALSE)
			continue;

		long nIndexCombo(1);
		for(long nL=0; nL<eLSD_COMBINE_TOTAL_COUNT; nL++)
		{	
			ELSDCombKind eKind = static_cast<ELSDCombKind>(nL);

			BOOL bExtreame = (eKind == eLSD_COMBINE_EXTREME_I || eKind == eLSD_COMBINE_EXTREME_II);
			if(bEarthQuake != bExtreame)
				continue;

			if(eKind == eLSD_COMBINE_SERVICE_IV || eKind == eLSD_COMBINE_STRESS)
				continue;

			if(eKind == eLSD_COMBINE_EXTREME_I || eKind == eLSD_COMBINE_EXTREME_II || eKind >= eLSD_COMBINE_SERVICE_I)
				nIndexCombo = 1;

			std::vector<CLsdLoadCase *> LCArray = pBri->m_plsdManager->GetLsdLoadCaseArray(eKind,eLSD_LOADCASE_DIR_BRI);
			for(auto ite=LCArray.begin(); ite!=LCArray.end(); ++ite)
			{
				if(eKind == eLSD_COMBINE_SERVICE_III)
				{
					// 사용하중조합 Ⅲ + Ⅳ
					if((*ite)->GetLsdLoadComboKind() != eLSD_COMBINE_SERVICE_III && (*ite)->GetLsdLoadComboKind() != eLSD_COMBINE_SERVICE_IV)
						continue;
				}
				else
				{
					if((*ite)->GetLsdLoadComboKind() != eKind)
						continue;
				}

				str = (*ite)->GetName();
				// 해당 조합이 있는지 확인을....
				if(pFEM->GetComboByName(str) == NULL)
					continue;

				dFactor = pBri->m_plsdManager->GetValueLoadModifiedFactor(eKind, ixPart, (*ite));
				strEtaCombo = pBri->GetStringComboEta_Lsd(eKind, (EPartOfBridge)ixPart, nIndexCombo);

				if(nIndexCombo == 1)
				{
					strDescription1 = pBri->m_plsdManager->GetCombineNameByIndex(eKind);
					strDescription2 = pBri->GetStringPartMerge((EPartOfBridge)ixPart);

					pFEM->AddCombo(strEtaCombo,str,0,1,dFactor,FALSE, strDescription1 + _T(" ") + strDescription2);
				}
				else
				{
					pFEM->AddCombo(strEtaCombo,str,0,1,dFactor,FALSE);
				}

				++nIndexCombo;
			}
		}

		++nCountComboPart;
	}
}

void CARcBridgeDesignFrame::MakeSapDataLoadUserLoad( CFEMManage *pFEM, long &nLoadCaseIndex, BOOL bEarthQuake, long nTypeDesing )
{
	CRcBridgeRebar		*pBri  = m_pStd->m_pBri;
	CARcBridgeDataStd   *pData = pBri->m_pARcBridgeDataStd;
	long nType = 0;
	if(bEarthQuake)
	{
		if(pBri->IsBoxType())
		{
			nType = pBri->GetEarthQuakeGoalByIndex(nTypeDesing-1) + 1;
		}
		else
		{
			nType = 1;
		}
	}

	BOOL bLsd = pData->IsLsdDesign();
	long nSizeUser = pBri->m_vpUserLoadSet[nType].size();
	for(long nIdx=0; nIdx<nSizeUser; nIdx++)
	{
		long nSizeLoad = pBri->m_vpUserLoadSet[nType][nIdx]->m_pArrUserLoad.GetSize();

		if(nSizeLoad > 0)
		{
			CString szName = _T("USER") + COMMA(nIdx+1);
			CString szDescription  = pBri->m_vpUserLoadSet[nType][nIdx]->m_szTitle;

			pFEM->MakeElementLoadForce(nLoadCaseIndex+1);
			pFEM->m_szLCName.SetAt(nLoadCaseIndex, szName);
			pFEM->m_szLCComment.SetAt(nLoadCaseIndex, szDescription);

			for(long ix=0; ix<nSizeLoad; ++ix)
			{
				CBridgeUserLoad *pUserLoad = pBri->m_vpUserLoadSet[nType][nIdx]->m_pArrUserLoad.GetAt(ix);
				if(pUserLoad->m_nIdxNo <= 0) continue;

				if(pUserLoad->m_nType == USER_LOAD_JOINT_V || pUserLoad->m_nType == USER_LOAD_JOINT_M)
				{
					CJoint *pJoint = pFEM->GetJoint(pUserLoad->m_nIdxNo - 1);
					if(pJoint == NULL) continue;
					
					CLoadForceJoint *pJointLoad = pJoint->GetLoadForce(nLoadCaseIndex);
					if(pJointLoad == NULL) continue;

					CVector vForce, vMoment;
					vForce.x	= GetValueUnitChange(pUserLoad->m_Ux, UNIT_CGS_TONF, pData->m_szTonf);
					vForce.y	= GetValueUnitChange(pUserLoad->m_Uy, UNIT_CGS_TONF, pData->m_szTonf);
					vForce.z	= GetValueUnitChange(pUserLoad->m_Uz, UNIT_CGS_TONF, pData->m_szTonf);
					// sap데이터 생성시 x축 모멘트를 -로 바꿔주기에 입력값을 맞추려고...
					vMoment.x	= -1 * GetValueUnitChange(pUserLoad->m_Mx, UNIT_CGS_TONFM, pData->m_szTonfM);
					vMoment.y	= GetValueUnitChange(pUserLoad->m_My, UNIT_CGS_TONFM, pData->m_szTonfM);
					vMoment.z	= GetValueUnitChange(pUserLoad->m_Mz, UNIT_CGS_TONFM, pData->m_szTonfM);

					pJointLoad->SetLoadForce(szName, vForce, vMoment);
				}
				else
				{
					CElement *pE = pFEM->GetElement(pUserLoad->m_nIdxNo - 1);
					if(pE == NULL) continue;
					CLoadForce *pELoad = pE->GetLoadForce(nLoadCaseIndex);
					if(pELoad == NULL) continue;

					if(pUserLoad->m_nType == USER_LOAD_ELEMENT_V)
					{
						CVector vForce;
						vForce.x	= GetValueUnitChange(pUserLoad->m_Ux, UNIT_CGS_TONF, pData->m_szTonf);
						vForce.y	= GetValueUnitChange(pUserLoad->m_Uy, UNIT_CGS_TONF, pData->m_szTonf);
						vForce.z	= GetValueUnitChange(pUserLoad->m_Uz, UNIT_CGS_TONF, pData->m_szTonf);

						pELoad->m_szLoadTitle = szName;
						pELoad->SetDistForce(bLsd ? frM(pUserLoad->m_dDist[0]) : pUserLoad->m_dDist[0], vForce, _T(""), FALSE);
					}
					else
					{
						CVector vForce1, vForce2;
						double dW1 =GetValueUnitChange(pUserLoad->m_dLoadW2[0], UNIT_CGS_TONF_M, pData->m_szTonf_M);
						double dW2 =GetValueUnitChange(pUserLoad->m_dLoadW2[1], UNIT_CGS_TONF_M, pData->m_szTonf_M);

						vForce1.x = (pUserLoad->m_nType == USER_LOAD_ELEMENT_WX)? dW1 : 0;
						vForce1.y = (pUserLoad->m_nType == USER_LOAD_ELEMENT_WY)? dW1 : 0;
						vForce1.z = (pUserLoad->m_nType == USER_LOAD_ELEMENT_WZ)? dW1 : 0;
						vForce2.x = (pUserLoad->m_nType == USER_LOAD_ELEMENT_WX)? dW2 : 0;
						vForce2.y = (pUserLoad->m_nType == USER_LOAD_ELEMENT_WY)? dW2 : 0;
						vForce2.z = (pUserLoad->m_nType == USER_LOAD_ELEMENT_WZ)? dW2 : 0;

						pELoad->m_szLoadTitle = szName;
						pELoad->SetLoadForce(pUserLoad->m_dDist[0], pUserLoad->m_dDist[1], vForce1, vForce2);
					}
				}
			}

			++nLoadCaseIndex;
		}
	}
}