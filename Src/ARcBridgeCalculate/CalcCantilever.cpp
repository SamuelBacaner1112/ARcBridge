#include "stdafx.h"
#include "ARcBridgeCalculate.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define COLLISION_V750		7500.
#define COLLISION_V250		2500.
#define COLLISION_V60		60
#define COLLISION_Rstd		200
#define COLLISION_H1		1000
#define COLLISION_UNIT_H	10000	// N/mm

#define CENTRIFUGAL_TANK_V 40
#define CENTRIFUGAL_HEIGHT_B 1800
#define CENTRIFUGAL_HEIGHT_L 487.5

#define N_CROWD_LOAD	0.005

CCalcCantilever::CCalcCantilever(void)
{
	m_bCollisionVehicle[0] = TRUE;
	m_bCollisionVehicle[1] = TRUE;

	// ARoad와 달리 좌우 캔틸레버 검토만 하므로 vector를 사용하지 않는다.
	for (long iLR = iLEFT; iLR <= iRIGHT; ++iLR)
	{
		m_pCantileverCheck[iLR] = new CCantileverCheckPosition;
	}
}

CCalcCantilever::~CCalcCantilever(void)
{
	for (long iLR = iLEFT; iLR <= iRIGHT; ++iLR)
	{
		SAFE_DELETE(m_pCantileverCheck[iLR]);
	}
}

CCantileverCheckPosition *CCalcCantilever::GetCheckPosition(HGBOOL bLeft)
{
	return m_pCantileverCheck[bLeft ? 0 : 1];
}

void CCalcCantilever::CalculateAll()
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;

	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	m_dUnitWeightSteelConc = pData->GetValueUnitType(toTon(pData->m_pBasicConcInfo->m_UWeightSteelConcrete), UNIT_CGS_TONF_M3, pData->m_szTonf_M3);

	MakeCantileverCheckPosition();
	CalculateCheckPosition();
}

void CCalcCantilever::CalculateCheckPosition()
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;

	for(long ix = 0; ix < 2; ix++)
	{
		CCantileverCheckPosition *pCheckPos = m_pCantileverCheck[ix];

		// 하중조합 그룹별 최대모멘트 설정
		pCheckPos->SetMomentByGroup(pBri->m_plsdMngForCantilever);

		SetConcBasicCheckPosition(pCheckPos, ix==0);

		// 슬래브 두께
		double dThick = pBri->m_dCantilH1[ix];
		pCheckPos->CalculateSlabThick(dThick);

		// 배력철근 검토 : 철근정보 설정후 계산
		pCheckPos->CalculateSupportRebar();

		// 처짐검토
		double dSpanLength = GetLengthSupport(ix==0);
		if(dSpanLength > 0)
		{
			pCheckPos->CalculateDeflection(dSpanLength);
		}
	}
}

void CCalcCantilever::MakeCantileverCheckPosition()
{
	for(long iLR = iLEFT; iLR <= iRIGHT; iLR++)
	{
		ClearVectorPtr(m_vArea[eSlabDC][iLR]);
		ClearVectorPtr(m_vArea[eSlabDW][iLR]);

		if( GetValidCheckCantilever(iLR==iLEFT) == FALSE ) continue;

		CCantileverCheckPosition *pCheckPos = new CCantileverCheckPosition;

		MakeAreaConc(iLR == iLEFT);

 		pCheckPos->SetName(iLR == iLEFT ? _T("좌측 켄틸레버") : _T("우측 켄틸레버"));
 
  		MakeListLoadCantilever(pCheckPos, iLR==iLEFT);
 		MakeListCombo(pCheckPos);
 
 		m_pCantileverCheck[iLR] = pCheckPos;
	}

}

HGBOOL CCalcCantilever::GetValidCheckCantilever(HGBOOL bLeft)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	return pBri->m_bCantilSlab[bLeft==TRUE ? 0 : 1];
}

// 방호벽(중분대),슬래브 콘크리트 영역 분할...
void CCalcCantilever::MakeAreaConc(BOOL bLeft)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	if( pBri == NULL ) return;

	long nLR = bLeft ? iLEFT : iRIGHT;
	CTwinVectorArray tvArr[2];
	// 방호벽
	GetTvarrayGuardWall(tvArr[0], bLeft);
	// 슬래브
	GetTvarrayCantilever(tvArr[1], bLeft);

	long nQtyHDan = pBri->GetQtyHDan();
	CGuardWallRC *pGW = pBri->GetGuardWallByHDan( bLeft==TRUE ? 0 : nQtyHDan-1 );
	HGBOOL bGuardWallDW = pGW->m_bDWload;

 	// AREA 분할....
 	CRebarPlacing rb;
 	for(long nS = 0; nS < 2; nS++) // 방호벽,슬래브
	{
		if(tvArr[nS].GetSize() == 0) continue;

		long ixLoadType = eSlabDC;
		if(nS == 0 && bGuardWallDW) 
			ixLoadType = eSlabDW;

		CDoubleArray dArrAngleSu;
		CTwinVectorArray tvArrResult;
		rb.SeperatorTwinVectorArea(tvArr[nS], tvArrResult, dArrAngleSu, 0.0, TRUE);

		long nIdx = 0;
		long nNum = m_vArea[ixLoadType][nLR].size() + 1;
		for(long i = 0; i < dArrAngleSu.GetSize(); i++)
		{
			long nCount	= (long)dArrAngleSu.GetAt(i);

			CTwinVectorAreaShape *pArea	= new CTwinVectorAreaShape(nNum++);
			for(long j = 0; j < nCount; j++)
			{
				pArea->AddTwinVector(tvArrResult.GetAt(nIdx++));
			}
			pArea->Calculate();
			double dArea = pArea->GetArea();
			if(Compare(dArea, 0.0, _T("=")))
			{
				nNum--;
				SAFE_DELETE(pArea);
				continue;
			}

			m_vArea[ixLoadType][nLR].push_back(pArea);
		}
	}
}

// 방호벽
void CCalcCantilever::GetTvarrayGuardWall(CTwinVectorArray &tvArr, BOOL bLeft)
{
	tvArr.RemoveAll();

	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	if( pBri == NULL ) return;

	long nQtyHDan = pBri->GetQtyHDan();
	long nSelect = bLeft==TRUE ? 0 : nQtyHDan-1;
	CGuardWallRC *pGW = pBri->GetGuardWallByHDan( nSelect );
	if(!pGW) return;

	CRebarPlacing rp;
	CTwinVectorArray tvArrResult;
	CDoubleArray dArrAngleSu;
	CTwinVector tvTemp, tv;
	CDoubleArray dDimHor;

	pGW->GetTvGuardWallBase(tvArr, FALSE, TRUE);
	long i=0; for(i=0; i<tvArr.GetSize(); i++)
	{
		tvTemp = tvArr.GetAt(i);
		if(tvTemp.GetLength() == 0)
		{
			tvArr.RemoveAt(i);
			i--;
		}
	}
	if(tvArr.GetSize() <= 0) return;

	// 우측인경우 우측 끝단을 0.0으로 이동시킨다.
	if(bLeft == FALSE)
	{
		tvArr.Move(CDPoint(-pGW->m_dWidhtHDan, 0));
	}


// 	tvTemp = tvArr.GetAt(0);
// 	tv.m_v1 = tvTemp.m_v1;
// 	tvArr.GetLast(tvTemp);
// 	tv.m_v2 = tvTemp.m_v2;
// 	tvArr.Add(tv);
// 
// 	for(i=0; i<tvArr.GetSize()-1; i++)
// 	{
// 		tv = tvArr.GetAt(i);
// 		double dHor = tv.GetHorLength();
// 		dDimHor.Add(dHor);
// 	}
// 	rp.SeperatorTwinVectorArea(tvArr, tvArrResult, dArrAngleSu);


// 	HGBOOL bRoadNotSlab(FALSE);
// 
// 	CTwinVectorArray tvArrResult;
// 	CTwinVector tvTemp, tv;
// 	CDoubleArray dDimVer;
// 	CDoubleArray dDimHor;
// 	CDoubleArray dArrAngleSu;
// 	
// 
// 	CString str = pBri->GetStringTypeHDan(nSelect, FALSE, bRoadNotSlab);
// 	if( str == "차도" )		// 보도
// 	{
// 		nSelect = bLeft==TRUE ? nSelect+1 : nSelect-1;
// 		pGW = pBri->GetGuardWallByHDan( nSelect );
// 		if(!pGW) return dArea;
// 	}
// 	else if( str == "보도" ) return dArea;
// 
// 	pGW->GetTvGuardWallBase(tvArr, FALSE);
// 	long i=0; for(i=0; i<tvArr.GetSize(); i++)
// 	{
// 		tvTemp = tvArr.GetAt(i);
// 		if(tvTemp.GetLength() == 0)
// 		{
// 			tvArr.RemoveAt(i);
// 			i--;
// 		}
// 	}
// 	if(tvArr.GetSize() <= 0) return 0;
// 
// 	tvTemp = tvArr.GetAt(0);
// 	tv.m_v1 = tvTemp.m_v1;
// 	tvArr.GetLast(tvTemp);
// 	tv.m_v2 = tvTemp.m_v2;
// 	tvArr.Add(tv);
// 
// 	for(i=0; i<tvArr.GetSize()-1; i++)
// 	{
// 		tv = tvArr.GetAt(i);
// 		double dHor = tv.GetHorLength();
// 		dDimHor.Add(dHor);
// 	}
// 	rp.SeperatorTwinVectorArea(tvArr, tvArrResult, dArrAngleSu);
// 	return rp.GetAreaTvArr(tvArr);

// 	CStringArray strArr;
// 	CDomyun pDom;
// 	dArea = m_pStd->m_pLoad->DrawDeadLoadSectionEtc(&pDom, pBri, pGW, strArr);
// 	return dArea;
}

// 캔틸레버
void CCalcCantilever::GetTvarrayCantilever(CTwinVectorArray &tvArr, BOOL bLeft)
{
	tvArr.RemoveAll();

	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	if( pBri == NULL ) return;

	long nPos = bLeft==TRUE ? 0 : 1;

//	HGDOUBLE dR = pBri->m_dRE2[nPos];
	HGDOUBLE dWidth = pBri->m_dCantilB[nPos];
	HGDOUBLE dHeight = pBri->m_dTS - pBri->m_dCantilH1[nPos] - pBri->m_dCantilH2[nPos];		// 직선 높이
	HGDOUBLE dH1 = pBri->m_dCantilH2[nPos];		// 변단면 높이
	HGINT32 nDir = bLeft? 1 : -1;

	CTwinVector tv;
	CDPoint xyStt(0,0);

	tv.SetFromPoint(xyStt, CDPoint(dWidth * nDir, 0));		// 외측 상단을 기준으로 한다.
	tvArr.Add(tv);

	tv.SetFromPoint(tv.m_v2, CDPoint(tv.m_v2.x, tv.m_v2.y - (dHeight + dH1)));
	tvArr.Add(tv);

	tv.SetFromPoint(tv.m_v2, CDPoint(tv.m_v2.x - dWidth * nDir, tv.m_v2.y + dH1));
	tvArr.Add(tv);

	tv.SetFromPoint(tv.m_v2, xyStt);
	tvArr.Add(tv);
}


// 켄틸레버 단위하중 생성
void CCalcCantilever::MakeListLoadCantilever(CCantileverCheckPosition *pCheckPosition, HGBOOL bLeft)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	if( pBri == NULL ) return;

	long nQtyHDan = pBri->GetQtyHDan();
	CGuardWallRC *pGW = pBri->GetGuardWallByHDan( bLeft==TRUE ? 0 : nQtyHDan-1 );
	HGBOOL bGuardWallDW = pGW->m_bDWload;

	// 방호벽,슬래브
	stCantileverLoadUnit *pLoad = GetUnitLoadConcreteDC(bLeft);
	pCheckPosition->AddLoad(pLoad);

	if(bGuardWallDW)
	{
		pLoad = GetUnitLoadConcreteDW(bLeft);
		pCheckPosition->AddLoad(pLoad);
	}

	// 포장하중
	pLoad = GetUnitLoadPavement(bLeft);
	pCheckPosition->AddLoad(pLoad);

	// 보도하중
	pLoad = GetUnitLoadFootWay(bLeft);
	pCheckPosition->AddLoad(pLoad);

	// 방음벽,난간
	pLoad = GetUnitLoadUpperStructure(bLeft, BANGUM);
	pCheckPosition->AddLoad(pLoad);
	//
	pLoad = GetUnitLoadUpperStructure(bLeft, NANGAN);
	pCheckPosition->AddLoad(pLoad);
 
	// 군중하중
	pLoad = GetUnitLoadCrowd(bLeft);
	pCheckPosition->AddLoad(pLoad);

 	//////////////////////////////////////////////////////////////////////////
	// 차량하중
	pLoad = GetUnitLoadVehicle(bLeft);
	pCheckPosition->AddLoad(pLoad);

	// 충돌하중
	pLoad = GetUnitLoadCollision(bLeft);
	pCheckPosition->AddLoad(pLoad);

	// 풍하중
	pLoad = GetUnitLoadWind(bLeft);
	pCheckPosition->AddLoad(pLoad);

	// 원심하중
	pLoad = GetUnitLoadCentrifugal(bLeft);
	pCheckPosition->AddLoad(pLoad);
}

stCantileverLoadUnit* CCalcCantilever::GetUnitLoadConcreteDC(HGBOOL bLeft)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	
	if( pBri != NULL )
	{
		stCantileverLoadUnit *pLoad = new stCantileverLoadUnit;
		pLoad->m_eLoadCaseKind = eLSD_LOADCASE_DC1;

		//DW하중이 있으면 따로 만들어준다.
		long nQtyHDan = pBri->GetQtyHDan();
		CGuardWallRC *pGW = pBri->GetGuardWallByHDan( bLeft==TRUE ? 0 : nQtyHDan-1 );
		HGBOOL bGuardWallDW = pGW->m_bDWload;

		double dDistCantil = GetDistCantilever(bLeft);
		double dDistConc = 0;
		
		CString strLoadName = bGuardWallDW ? STR_LOAD_SLAB : STR_LOAD_CONCRETE;
		pLoad->sName = strLoadName;

		double dTotalWeight = 0, dTotalWeightDistX = 0;
		
		long nLR = (bLeft) ? iLEFT : iRIGHT;
		long nSizeArea = m_vArea[eSlabDC][nLR].size();
		for(long i=0; i<nSizeArea; i++)
		{
			CTwinVectorAreaShape *pArea	= m_vArea[eSlabDC][nLR][i];
			double dArea = pArea->GetArea();
			if(Compare(dArea, 0.0, _T("=")))
				continue;
		
			double dWeight	= dArea * _1000MM * m_dUnitWeightSteelConc;
 			dDistConc = fabs(pArea->m_xyCen.x - dDistCantil * (bLeft? 1 : -1));
		
			dTotalWeight += dWeight;	
			dTotalWeightDistX += dWeight * dDistConc;
		}
		CDPoint xyCen(0,0);
		xyCen.x = dTotalWeightDistX/dTotalWeight;
		
		pLoad->m_xyLoadPosition = xyCen;
		pLoad->m_dForceVert =dTotalWeight;

		return pLoad;
	}

	return NULL;
}


double CCalcCantilever::GetDistCantilever(HGBOOL bLeft)
{
	HGDOUBLE dDist(0);

// 	CTwinVectorArray tvArr;
// 	GetTvarrayCantilever(tvArr, bLeft);
// 
// 	dDist = abs(tvArr.GetXyCenter().x);

	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	dDist = pBri->m_dCantilB[bLeft? iLEFT : iRIGHT];

	return dDist;
}

stCantileverLoadUnit* CCalcCantilever::GetUnitLoadConcreteDW( HGBOOL bLeft )
{
// 	CRcBridgeRebar *pBri = m_pStd->m_pBri;
// 	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	long nLR = bLeft ? iLEFT : iRIGHT;
	long nSizeArea = m_vArea[eSlabDW][nLR].size();
	if(nSizeArea == 0) return NULL;

	stCantileverLoadUnit *pLoad = new stCantileverLoadUnit;
	pLoad->m_eLoadCaseKind = eLSD_LOADCASE_DW1;
	pLoad->sName =STR_LOAD_GUARDWALL;

	double dTotalWeight = 0, dTotalWeightDistX = 0;

	double dDistCantil = GetDistCantilever(bLeft);
	double dDistConc = 0;

	for(long i=0; i<nSizeArea; i++)
	{
		CTwinVectorAreaShape *pArea	= m_vArea[eSlabDW][nLR][i];
		double dArea = pArea->GetArea();
		if(Compare(dArea, 0.0, _T("=")))
			continue;

		double dWeight	= dArea * _1000MM * m_dUnitWeightSteelConc;
		dDistConc = fabs(pArea->m_xyCen.x - dDistCantil * (bLeft? 1 : -1));

		dTotalWeight += dWeight;	
		dTotalWeightDistX += dWeight * dDistConc;
	}
	CDPoint xyCen(0,0);
	xyCen.x = dTotalWeightDistX/dTotalWeight;

	pLoad->m_xyLoadPosition = xyCen;
	pLoad->m_dForceVert = dTotalWeight;

	return pLoad;
}

BOOL CCalcCantilever::IsSelectedLoad(ELSDLoadSelectKind eKind, BOOL bDeckSlab)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;

	CLsdManager *pLsdManager = pBri->m_plsdMngForCantilever;
	if(pLsdManager == NULL) return FALSE;

	CLsdDesignLoad *pDesignLoad = pLsdManager->GetDesignLoad(eKind);

	if(pDesignLoad == NULL || pDesignLoad->GetUse() == FALSE) return FALSE;

	return TRUE;
}

stCantileverLoadUnit* CCalcCantilever::GetUnitLoadFootWay(HGBOOL bLeft)
{
	if(IsSelectedLoad(eLSD_LOADSELECT_DW, TRUE) == FALSE) return NULL;

	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	CCheckCantileverData *pCheckData = pBri->m_pCheckCantilever;
	if( pCheckData == NULL ) return NULL;

	if( pCheckData->GetWorkingLoadData(bLeft)->GetBodoWeight().m_bExist == FALSE ) return NULL;
	 
 	CDPoint xyFootway = GetXyHDanByType(bLeft);
	double dLength = fabs(xyFootway.x - xyFootway.y);
	double dLengthFootWay = GetLengthSupport(bLeft, TRUE);
	if(Compare(dLength, 0., _T("=")) || Compare(dLengthFootWay, 0., _T("="))) return NULL;

	stCantileverLoadUnit *pLoad = new stCantileverLoadUnit;

	pLoad->m_eLoadCaseKind = eLSD_LOADCASE_DW1;
	pLoad->sName = STR_LOAD_FOOTWAY;

	double dLoadStt = GetValueUnitChange(pCheckData->GetWorkingLoadData(bLeft)->GetBodoWeight().m_dWeightStt, UNIT_CGS_TONF_M, pData->m_szTonfM);
	double dLoadEnd = GetValueUnitChange(pCheckData->GetWorkingLoadData(bLeft)->GetBodoWeight().m_dWeightEnd, UNIT_CGS_TONF_M, pData->m_szTonfM);
	dLoadEnd = dLoadStt - (dLoadStt - dLoadEnd) * dLength / dLengthFootWay;

	CDPoint xyCen(0,0);
	xyCen.x = fabs(xyFootway.x + xyFootway.y) / 2.;
	pLoad->m_xyLoadPosition = xyCen;
	pLoad->m_dForceVert = dLength * (dLoadStt + dLoadEnd) / 2.;

	return pLoad;
}

stCantileverLoadUnit* CCalcCantilever::GetUnitLoadCrowd(HGBOOL bLeft)
{
	if(IsSelectedLoad(eLSD_LOADSELECT_PL, TRUE) == FALSE) return NULL;

	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	CCheckCantileverData *pCheckData = pBri->m_pCheckCantilever;
	if( pCheckData == NULL ) return NULL;

	if( pCheckData->GetWorkingLoadData(bLeft)->GetWalkLoad() == FALSE ) return NULL;

	CDPoint xyFootway = GetXyHDanByType(bLeft); // x = 시작위치, y = 끝위치 (슬래브끝단기준)
	double dLength = fabs(xyFootway.x - xyFootway.y);
	if(Compare(dLength, 0., _T("="))) return NULL;

	stCantileverLoadUnit *pLoad = new stCantileverLoadUnit;

	pLoad->m_eLoadCaseKind = eLSD_LOADCASE_PL;
	pLoad->sName = STR_LOAD_CROWD;

	double dLoad = GetValueUnitChange(toTon(pCheckData->GetWorkingLoadData(bLeft)->GetVertF()), UNIT_CGS_TONF_M, pData->m_szTonf_M);

	CDPoint xyCen(0,0);
	xyCen.x = fabs(xyFootway.x + xyFootway.y) / 2.;

	double dVForce = dLength * dLoad * _1000MM;

	pLoad->m_xyLoadPosition = xyCen;
	pLoad->m_dForceVert = dVForce;

	return pLoad;
}

CDPoint CCalcCantilever::GetXyHDanByType(HGBOOL bLeft, long nHDanType)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;

	HGDOUBLE dLenCantil = pBri->m_dCantilB[bLeft==TRUE ? 0 : 1];

	CDPoint xyHDan(0,0); // 캔틸레버 검토위지
	HGDOUBLE dBaseX(0), dSttX(0);
	CTwinVectorArray tvArr;

	CRebarPlacing rp;
	long nQtyHDan = pBri->GetQtyHDan();
	double dLengthSum = 0, dLengthHDan(0);
	BOOL bFirst = TRUE;

	if( bLeft )
	{
		for (long nHDan = 0; nHDan < nQtyHDan; ++nHDan )
		{
			CGuardWallRC* pGW = pBri->GetGuardWallByHDan(nHDan);
			if( pGW == NULL ) continue;

			dLengthHDan = pBri->GetLengthHDan(nHDan);
			dLengthSum += dLengthHDan;

			if( pGW->m_nTypeGuardWall != nHDanType )
			{
				if(dLengthSum >= dLenCantil) 
					break;
				else
					continue;
			}
			if(bFirst)
			{
				xyHDan.x = dLengthSum - dLengthHDan - dLenCantil;
				bFirst = FALSE;
			}
			if(dLengthSum <= dLenCantil)
				xyHDan.y = dLengthSum - dLenCantil;
			else
				xyHDan.y = 0;
			if(dLengthSum >= dLenCantil) break;
		}
	}
	else
	{
		for(long nHDan = nQtyHDan - 1; nHDan >= 0; nHDan--)
		{
			CGuardWallRC* pGW = pBri->GetGuardWallByHDan(nHDan);
			if( pGW == NULL ) continue;

			dLengthHDan = pBri->GetLengthHDan(nHDan);
			dLengthSum += dLengthHDan;

			if(pGW->m_nTypeGuardWall != nHDanType) 
			{
				if(dLengthSum >= dLenCantil) 
					break;
				else
					continue;
			}
			if(bFirst)
			{
				xyHDan.x = dLenCantil - (dLengthSum - dLengthHDan);
				bFirst = FALSE;
			}
			if(dLengthSum <= dLenCantil)
				xyHDan.y = dLengthSum;
			else
				xyHDan.y = 0;
			if(dLengthSum >= dLenCantil) break;
		}

	}

	return xyHDan;
}

// 난간(0), 방음벽(1)
stCantileverLoadUnit* CCalcCantilever::GetUnitLoadUpperStructure(HGBOOL bLeft, long nType)
{
	if(IsSelectedLoad(eLSD_LOADSELECT_DW, TRUE) == FALSE) return NULL;

	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	CCheckCantileverData *pCheckData = pBri->m_pCheckCantilever;
	if( pCheckData == NULL ) return NULL;

	CWorkingLoad *pWorking = pCheckData->GetWorkingLoadData(bLeft);
	if( pWorking == NULL ) return NULL;

	if( pWorking->GetWalkLoad() == FALSE ) return NULL;

//	long nQtyHDan = pBri->GetQtyHDan();
//	long nG	= bLeft ? 0 : nQtyHDan-1;

	CTwinVectorArray tvArr;
	GetTvarrayCantilever(tvArr, bLeft);
	double dLenCantil = tvArr.GetLength() / 2.;

	stCantileverLoadUnit *pLoad = new stCantileverLoadUnit;

	pLoad->m_eLoadCaseKind = eLSD_LOADCASE_DW1;
	pLoad->sName =  nType == BANGUM ? STR_LOAD_BANGUM : STR_LOAD_NANGAN;

	double dVForce = GetValueUnitChange( pWorking->GetUserLoad(nType).m_dWeight, UNIT_CGS_TONF_M, pData-> m_szTonf_M);

	CDPoint xyCen(0,0);
	xyCen.x = dLenCantil - frM(pWorking->GetUserLoad(nType).m_dDist);

	pLoad->m_xyLoadPosition = xyCen;
	pLoad->m_dForceVert = dVForce;

	return pLoad;
}

stCantileverLoadUnit* CCalcCantilever::GetUnitLoadPavement(HGBOOL bLeft)
{
	if(IsSelectedLoad(eLSD_LOADSELECT_DW, TRUE) == FALSE) return NULL;

	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	HGDOUBLE dWeightPave(0.);		// 포장 단위중량
	HGDOUBLE dBaseX(0.);
	HGDOUBLE dCantileverW = pBri->m_dCantilB[bLeft==TRUE ? 0 : 1];

	CTwinVectorArray tvArr;
	long nQtyHDan = pBri->GetQtyHDan();

	// 보도가 없으면 포장하중
	for ( long nHDan = 0; nHDan<nQtyHDan; ++nHDan )
	{
		CGuardWallRC *pGW = pBri->GetGuardWallByHDan( nHDan );
		if( pGW == NULL ) continue;

		pGW->GetTvGuardWallBase(tvArr, FALSE);
		dBaseX += (tvArr.GetXyRight().x - tvArr.GetXyLast().x);

		CString str = pBri->GetStringTypeHDan(nHDan, FALSE, FALSE);
		if( str == "보도") return NULL;

		dWeightPave = pGW->m_dUnitWeightPave;

		if( dCantileverW < dBaseX )
			break;
	}

	CCheckCantileverData *pCheckData = pBri->m_pCheckCantilever;
	if( pCheckData == NULL ) return NULL;

	if( pCheckData->GetWorkingLoadData(bLeft)->GetBodoWeight().m_bExist == FALSE ) return NULL;

	stCantileverLoadUnit *pLoad = new stCantileverLoadUnit;

	pLoad->m_eLoadCaseKind = eLSD_LOADCASE_DW1;
	pLoad->sName = STR_LOAD_PAVEMENT;

//	long nHDanType = 0; // 없음(포장)
	
	// 보도가 아니면 무조건 차도로 인식하게 한다. - 기술부랑 협의함.
	CDPoint xyPave = GetXyHDanByType(bLeft); // x = 시작위치, y = 끝위치 (슬래브끝단기준)
	double dLength = fabs(xyPave.x - xyPave.y);
	double dThick = pBri->GetThickPave(FALSE);

	CDPoint xyCen(0,0);
	xyCen.x = fabs(xyPave.x + xyPave.y) / 2.;
	
	double dVForce = dLength * dThick * _1000MM * dWeightPave;

	pLoad->m_xyLoadPosition = xyCen;
	pLoad->m_dForceVert = dVForce;

	return pLoad;
}

stCantileverLoadUnit* CCalcCantilever::GetUnitLoadVehicle(HGBOOL bLeft)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	CCheckCantileverData *pCheckData = pBri->m_pCheckCantilever;
	if( pCheckData == NULL ) return NULL;

	long nLR = bLeft==TRUE ? iLEFT : iRIGHT;

	double dX = GetLengthSupport(bLeft);
	if(Compare(dX, 0.0, _T("<="))) return NULL;

 	stCantileverLoadUnit *pLoad = new stCantileverLoadUnit;
	

	pLoad->m_eLoadCaseKind = eLSD_LOADCASE_LL1;
	pLoad->sName = STR_LOAD_VEHICLE;

//	double dE = VEHICLE_FACTOR_E1 * dX + VEHICLE_FACTOR_E2;
	// 임시
	sVC[nLR].dPr = 100.;
	sVC[nLR].dMl = 200.;

// 	sVC[nLR].dPr = GetValueUnitChange(pGradeCtl->GetLSDWeight(nLoadType, pGradeCtl->GetLSDLoadSize(nLoadType) - 1, nGrade) / 2., UNIT_CGS_KGF, "N");
// 	sVC[nLR].dMl = (sVC[nLR].dPr / dE * _1000MM) * dX * (1 + _dImpactLoad[1]);

	pLoad->m_xyLoadPosition = CDPoint(dX, 0);
	pLoad->m_dForceVert = sVC[nLR].dMl / dX;

	long nQtyHDan = pBri->GetQtyHDan();
	CGuardWallRC* pGw = pBri->GetGuardWallByHDan(bLeft==TRUE ? 0 : nQtyHDan-1);		// 끝단을 기준으로 한다.
	if( pGw == NULL ) return NULL;
	
	HGDOUBLE dTLen = pBri->GetLengthHDan(bLeft==TRUE ? 0 : nQtyHDan-1);
	
	if( pCheckData->m_bFloorTank || pCheckData->m_bFloorTrailer )
	{
		HGBOOL bGw = pGw->m_nTypeGuardWall==2 ? TRUE : FALSE;

		double dD1 = bGw ? 0 : (bLeft ? pGw->m_dW1 : dTLen - pGw->GetWidth());
		double dWidth = bGw ? 0 : pGw->GetWidth();
		double dTerm = pBri->m_dCantilB[bLeft==TRUE ? 0 : 1];

// 		long nType	= pFloor->GetGuardWallType();
// 		double dTLen = pDB->GetLengthHDanDom(pBx,pDB->GetNumHDanDomAtGuradFenceFrLeft(pBx,pDB->GetGuardFenceSu()-1));
// 
// 		double dD1 = nType == GUARDWALL_NONE_CHECK ? 0 : bLeft ? pGuard->m_D1 : dTLen - pGuard->GetWidth();
// 		double dWidth = nType == GUARDWALL_NONE_CHECK ? 0 : pGuard->GetWidth();
// 		double dSharp = m_pStd->GetSharpCantilever(pBx, bLeft);


		BOOL bNoDistOnNoGuard = FALSE;		// [횡방향 하중분배]-[활하중]-[연석없는 경우 단부적용] 옵션
		

		if( pCheckData->m_bFloorTank )
		{
//			sVC[nLR].DisFromWallTank = (dTLen==0 && pData->m_bNoDistOnNoGuard) ? 0 : frM(pData->m_dTankEdit2/2);
			// 임시 데이터
			double dTankEdit2 = 500;				// 탱크 바퀴간격

			sVC[nLR].DisFromWallTank = (dTLen==0 && bNoDistOnNoGuard ) ? 0 : frM(dTankEdit2/2);
 			sVC[nLR].dTankW = GetValueUnitChange(pData->m_dTank_WeightTank, UNIT_CGS_TONF, _T("N"));
 			sVC[nLR].dL2	= dTerm - (sVC[nLR].DisFromWallTank + dD1 + dWidth);	
			sVC[nLR].dP2	= sVC[nLR].dTankW / 2 / dTankEdit2;
			sVC[nLR].dMtank	= sVC[nLR].dP2 * sVC[nLR].dL2 * (1 + _dImpactLoad[2]);

			if(sVC[nLR].dMtank > sVC[nLR].dMl)
			{
				pLoad->m_xyLoadPosition = CDPoint(sVC[nLR].dL2, 0);
				pLoad->m_dForceVert = sVC[nLR].dP2 * (1 + _dImpactLoad[2]);
			}
		}

		if( pCheckData->m_bFloorTrailer )
		{
			double dTrailEdit5 = 700; // 트레일러 바퀴간격 - 횡방향 	측면

			sVC[nLR].DisFromWallTrailer = (dTLen==0 && bNoDistOnNoGuard) ? 0 : frM(dTrailEdit5/2);
			sVC[nLR].dTrailerW = GetValueUnitChange(max(pData->m_dTank_Ptrailer1, max(pData->m_dTank_Ptrailer2, pData->m_dTank_Ptrailer3)), UNIT_CGS_TONF, _T("N"));

			sVC[nLR].dL3	= dTerm - (sVC[nLR].DisFromWallTrailer + dD1 + dWidth);				
			sVC[nLR].dE3	= VEHICLE_FACTOR_E1 * sVC[nLR].dL3 + VEHICLE_FACTOR_E2;
			sVC[nLR].dP3	= sVC[nLR].dTrailerW * (1 + _dImpactLoad[2]) / sVC[nLR].dE3 * _1000MM;
			sVC[nLR].dMTrail = sVC[nLR].dP3 * sVC[nLR].dL3;

			if(sVC[nLR].dMTrail > sVC[nLR].dMtank && sVC[nLR].dMTrail > sVC[nLR].dMl)
			{
				pLoad->m_xyLoadPosition = CDPoint(sVC[nLR].dL3, 0);
				pLoad->m_dForceVert = sVC[nLR].dP3;
			}

// 			sVC[nLR].DisFromWallTrailer = (dTLen==0 && pData->m_bNoDistOnNoGuard) ? 0 : frM(pData->m_dTrailEdit5/2);
// 			sVC[nLR].dTrailerW = GetValueUnitChange(max(pData->m_dTrailP1, max(pData->m_dTrailP2, pData->m_dTrailP3)), UNIT_CGS_TONF, _T("N"));
// 
// 			sVC[nLR].dL3	= dTerm - dSharp / 2. - (sVC[nLR].DisFromWallTrailer + dD1 + dWidth);				
// 			sVC[nLR].dE3	= VEHICLE_FACTOR_E1 * sVC[nLR].dL3 + VEHICLE_FACTOR_E2;
// 			sVC[nLR].dP3	= sVC[nLR].dTrailerW * (1 + _dImpactLoad[2]) / sVC[nLR].dE3 * _1000MM;
// 			sVC[nLR].dMTrail = sVC[nLR].dP3 * sVC[nLR].dL3;
// 			if(sVC[nLR].dMTrail > sVC[nLR].dMtank && sVC[nLR].dMTrail > sVC[nLR].dMl)
// 			{
// 				pLoad->SetLoadPosition(CDPoint(sVC[nLR].dL3, 0));
// 				pLoad->SetForceVert(sVC[nLR].dP3);
// 			}

		}
	}

	return pLoad;
}

stCantileverLoadUnit* CCalcCantilever::GetUnitLoadCollision(HGBOOL bLeft)
{
	if(IsSelectedLoad(eLSD_LOADSELECT_CT, TRUE) == FALSE) return NULL;

	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	CCheckCantileverData *pCheckData = pBri->m_pCheckCantilever;
	if( pCheckData == NULL ) return NULL;

	CWorkingLoad *pWorkingLoad = pCheckData->GetWorkingLoadData(bLeft);
	if( pWorkingLoad == NULL ) return NULL;

	long nQtyHDan = pBri->GetQtyHDan();
	CGuardWallRC* pGw = pBri->GetGuardWallByHDan(bLeft==TRUE ? 0 : nQtyHDan-1);		// 끝단을 기준으로 한다.
	if( pGw == NULL ) return NULL;

	double dVelocity= pCheckData->m_dCo_V;
	double dLendSupport = GetLengthSupport(bLeft);
	if(Compare(dLendSupport, 0.0, _T("<="))) return NULL;
	double dR = pCheckData->m_dCenter_R[bLeft ? iLEFT : iRIGHT];
	double dHeight = COLLISION_H1 + pBri->GetThickPave() + pBri->m_dTS / 2; // 수평 충돌 높이

	double dHeightNangan = pWorkingLoad->GetUserLoad(NANGAN).m_bExist ? pWorkingLoad->GetUserLoad(NANGAN).m_dHeight : 0;
	double dGuardH	=  pGw->GetHeight() + pBri->m_dTS / 2 + dHeightNangan;
	double dTerm = pBri->m_dCantilB[bLeft==TRUE ? 0 : 1];

	long nLR = bLeft==TRUE ? iLEFT : iRIGHT;

	sCO[nLR].dH = GetValueUnitChange(pWorkingLoad->GetHoriF(), UNIT_CGS_KGF_M, pData->m_szTonf_M) * _1000MM;
	sCO[nLR].dV = GetValueUnitChange(pWorkingLoad->GetVertF(), UNIT_CGS_KGF_M, pData->m_szTonf_M) * _1000MM;

	stCantileverLoadUnit *pLoad = new stCantileverLoadUnit;

	pLoad->m_eLoadCaseKind = eLSD_LOADCASE_CT11;
	m_bCollisionVehicle[nLR] = TRUE;

	BOOL bExistFootway = pWorkingLoad->GetBodoWeight().m_bExist;
	if(bExistFootway && dLendSupport > 0) // 보도가 있고 차도가 있는 경우
	{		
		double LEN = pBri->GetLengthHDan(bLeft==TRUE ? 0 : nQtyHDan-1);
		double dDist = (dTerm - LEN) + (bLeft ? (pGw->m_dW5 + pGw->m_dW4) : (pGw->m_dW1 + pGw->m_dW2))  + pGw->m_dW3 / 2;
 //		double dMoment = sCO[nLR].dV * dDist + dHeight * sCO[nLR].dH;

		pLoad->sName = STR_LOAD_COLLISION_VEHICLE;
		pLoad->m_xyLoadPosition = CDPoint(dDist, dHeight);
		pLoad->m_dForceVert = sCO[nLR].dV;
		pLoad->m_dForceHori = sCO[nLR].dH;
	}
	else if(!bExistFootway) // 보도가 없고 방호벽 혹은 중분대
	{
		if( pGw->m_nTypeGuardWall == 2 || pGw->m_nTypeGuardWall == 3 )
		{
			sCO[nLR].dH = (dVelocity / COLLISION_V60) * (dVelocity / COLLISION_V60) * COLLISION_V750 + COLLISION_V250;
			double dMoment1	= sCO[nLR].dH * dHeight;
			double dMoment2	= COLLISION_UNIT_H * dGuardH; // 10N/mm

			pLoad->sName = STR_LOAD_COLLISION_VEHICLE;
			if(dMoment1 > dMoment2)
			{
				pLoad->m_xyLoadPosition = CDPoint(0, dHeight);
				pLoad->m_dForceHori = sCO[nLR].dH;
			}
			else
			{
				pLoad->m_xyLoadPosition = CDPoint(0, dGuardH);
				pLoad->m_dForceHori = COLLISION_UNIT_H;
			}
			if(dR > 0 && dR <= COLLISION_Rstd ) // 반경이 200M보다 작은 경우 2배
			{
				pLoad->m_dForceHori *= 2;
			}
		}
	}
	else  if(bExistFootway && dLendSupport <= 0) // 보도만 있는 경우
	{
		m_bCollisionVehicle[nLR] = FALSE;

		double dLoadPos = 0;
		if(pWorkingLoad->GetUserLoad(NANGAN).m_bExist)
		{
			dLoadPos = pWorkingLoad->GetUserLoad(BANGUM).m_dDist;
		}
		else if(pWorkingLoad->GetUserLoad(BANGUM).m_bExist)
		{
			dLoadPos = pWorkingLoad->GetUserLoad(BANGUM).m_dDist;
		}
		sCO[nLR].dLoadPos = dLoadPos;

		double dH = dGuardH + pWorkingLoad->GetUserLoad(NANGAN).m_dHeight;
		double LEN = pBri->GetLengthHDan(bLeft ? 0 : nQtyHDan-1);
		double dDist = (dTerm - LEN) + (bLeft ? (pGw->m_dW5+pGw->m_dW4) : (pGw->m_dW1+pGw->m_dW2)) + pGw->m_dW3 - sCO[nLR].dLoadPos;
 //		double dMoment = sCO[nLR].dH * dH + sCO[nLR].dV * dDist;

		pLoad->sName = STR_LOAD_COLLISION_CROWD;
		pLoad->m_xyLoadPosition = CDPoint(dDist, dH);
		pLoad->m_dForceVert = sCO[nLR].dV;
		pLoad->m_dForceHori = sCO[nLR].dH;
	}

	return pLoad;
}

stCantileverLoadUnit* CCalcCantilever::GetUnitLoadWind(HGBOOL bLeft)
{
	if(IsSelectedLoad(eLSD_LOADSELECT_WS, TRUE) == FALSE) return NULL;

	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	CCheckCantileverData *pCheckData = pBri->m_pCheckCantilever;
	if( pCheckData == NULL ) return NULL;

	CWorkingLoad *pWorkingLoad = pCheckData->GetWorkingLoadData(bLeft);
	if( pWorkingLoad == NULL ) return NULL;

	long nQtyHDan = pBri->GetQtyHDan();
	CGuardWallRC* pGw = pBri->GetGuardWallByHDan(bLeft==TRUE ? 0 : nQtyHDan-1);		// 끝단을 기준으로 한다.
	if( pGw == NULL ) return NULL;

	BOOL bBodo = (pWorkingLoad->GetBodoWeight().m_bExist && pWorkingLoad->GetBodoWeight().m_dHeight > 0);
	double dPaveT = bBodo ? frM(pWorkingLoad->GetBodoWeight().m_dHeight) : pBri->GetThickPave();	// 포장두께 
	double dSlabT =	pBri->m_dTS;	// 슬래브 기준 두께	
	double dWindStrength = GetValueUnitChange(pCheckData->m_dWindStrength, UNIT_CGS_TONF_M2, pData->m_szTonf_M2);	// 풍하중 강도

	double dHeight = pGw->GetHeight();;

	double dWindLoad = dWindStrength * _1000MM * (dHeight);  // N
	double dHeightLoad = (dHeight) / 2. + dPaveT + dSlabT / 2.; // N.mm

	stCantileverLoadUnit *pLoad = new stCantileverLoadUnit;

	pLoad->m_eLoadCaseKind = eLSD_LOADCASE_WS11;
	pLoad->sName = STR_LOAD_WIND;
	pLoad->m_xyLoadPosition = CDPoint(0, dHeightLoad);
	pLoad->m_dForceHori =dWindLoad;

	return pLoad;
}

stCantileverLoadUnit* CCalcCantilever::GetUnitLoadCentrifugal(HGBOOL bLeft)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	CCheckCantileverData *pCheckData = pBri->m_pCheckCantilever;
	if( pCheckData == NULL ) return NULL;

	CWorkingLoad *pWorkingLoad = pCheckData->GetWorkingLoadData(bLeft);
	if( pWorkingLoad == NULL ) return NULL;

	long nQtyHDan = pBri->GetQtyHDan();
	CGuardWallRC* pGw = pBri->GetGuardWallByHDan(bLeft==TRUE ? 0 : nQtyHDan-1);		// 끝단을 기준으로 한다.
	if( pGw == NULL ) return NULL;

	long nType = pGw->m_nTypeGuardWall;
	long nLR = bLeft==TRUE ? iLEFT : iRIGHT;

	double dR		= pCheckData->m_dCenter_R[bLeft ? iLEFT : iRIGHT];					// 설계반경 
 	double dV		= pCheckData->m_dCo_V;						// 설계속도 
 	double dVT		= CENTRIFUGAL_TANK_V;					// 탱크 or 트레일러 설계속도
	double dD1		= pGw ? pGw->m_dDistFrLeft : 0;			// 방호벽 바깥쪽 공간 - 횡단 슬래브 제원 - D1
	double dWidth	= nType != 2 ? 0 : pGw->GetWidth();	// 방호벽 폭 
	double dTerm = pBri->m_dCantilB[bLeft==TRUE ? 0 : 1];

	sCF[nLR].dL1 = GetLengthSupport(bLeft);// 계산지간 
	sCF[nLR].dL3 = 0;
	sCF[nLR].dE1 = VEHICLE_FACTOR_E1 * sCF[nLR].dL1 + VEHICLE_FACTOR_E2; // 윤하중 분포폭 
	sCF[nLR].dE3 = 0;	

	double dSlabT = pBri->m_dTS; // 슬래브 두께
	double dPaveT = pBri->GetThickPave();				// 포장두께
	sCF[nLR].dHeight = CENTRIFUGAL_HEIGHT_B + dSlabT / 2. + dPaveT;		// 작용높이

	if(pWorkingLoad->GetBodoWeight().m_bExist) return NULL;
	if(Compare(sCF[nLR].dL1, 0.0, _T("<=")) || Compare(dR, 0.0, _T("="))) return NULL;

 	HGDOUBLE dLSDWeight = GetValueUnitChange(m_pStd->m_pLoad->m_dPrApply[0],UNIT_CGS_TONF,pData->m_szTonf);
	HGDOUBLE dTankEdit1 = m_pStd->m_pLoad->m_dTank_TankLength;
	
	sCF[nLR].dPr1  = dLSDWeight;
	sCF[nLR].dCf1  = CalcCentriodLoad_CF(dV, dR);
	sCF[nLR].dPcf1 = sCF[nLR].dPr1 / sCF[nLR].dE1 * sCF[nLR].dCf1;
	sCF[nLR].dMcf1 = sCF[nLR].dPcf1 * sCF[nLR].dHeight;	

	// 탱크
	if(pCheckData->m_bFloorTank)
	{
		sCF[nLR].dPr2  = pData->m_dTank_WeightTank / 2 / dTankEdit1;
		sCF[nLR].dCf2  = CalcCentriodLoad_CF(dVT, dR);
		sCF[nLR].dPcf2 = sCF[nLR].dPr2 * sCF[nLR].dCf2;
		sCF[nLR].dMcf2 = sCF[nLR].dPcf2 * sCF[nLR].dHeight;	
	}

	// 트레일러
	if (pCheckData->m_bFloorTrailer)
	{
		sCF[nLR].dL3	  = dTerm - (CENTRIFUGAL_HEIGHT_L + dD1 + dWidth);
		sCF[nLR].dE3	  = VEHICLE_FACTOR_E1 * sCF[nLR].dL3 + VEHICLE_FACTOR_E2;
		sCF[nLR].dPr3  = GetValueUnitChange(MAX(pData->m_dTank_Ptrailer1, MAX(pData->m_dTank_Ptrailer2, pData->m_dTank_Ptrailer3)), UNIT_CGS_TONF, "N");
		sCF[nLR].dCf3  = CalcCentriodLoad_CF(dVT, dR);
		sCF[nLR].dPcf3 = sCF[nLR].dPr3 * sCF[nLR].dCf3;
		sCF[nLR].dMcf3 = sCF[nLR].dPcf3 * sCF[nLR].dHeight;
	}

	stCantileverLoadUnit *pLoad = new stCantileverLoadUnit;

	pLoad->m_eLoadCaseKind = eLSD_LOADCASE_CF;
	pLoad->sName = _T("원심하중");
	pLoad->m_xyLoadPosition = CDPoint(0, sCF[nLR].dHeight);
	pLoad->m_dForceHori = MAX(sCF[nLR].dPcf1, MAX(sCF[nLR].dPcf2, sCF[nLR].dPcf3));

	return pLoad;
}

void CCalcCantilever::MakeListCombo(CCantileverCheckPosition *pCheckPosition)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	CLsdManager *pLsdManager = pBri->m_plsdMngForCantilever;

	vector<stCantileverLoadUnit*> vLoad = pCheckPosition->GetLoads();

	long nSizeLoad = vLoad.size();
	long nEtaPos = 0;
	double dFactor = 0;
	for(long nComboKind = eLSD_COMBINE_LIMIT_I; nComboKind < eLSD_COMBINE_TOTAL_COUNT; nComboKind++)
	{
		ELSDLoadGroupKind eGroupKind = pLsdManager->GetLoadGroupKindByCombKind((ELSDCombKind)nComboKind);


		std::vector<CLsdLoadCase *> vecLoadCase = pLsdManager->GetLsdLoadCaseArray((ELSDCombKind)nComboKind, eLSD_LOADCASE_DIR_PER);

		long nSizeCombo = vecLoadCase.size();
		for(long nCombo = 0; nCombo < nSizeCombo; nCombo++)
		{
			CLsdLoadCase *pLoadCase = vecLoadCase[nCombo];
//			if(pLoadCase->GetSizeExtraValueArray() == 0 || pLoadCase->GetExtraValueArray()[0] == FALSE) continue;

			stCantileverCombo *pCombo = new stCantileverCombo;

			pCombo->m_sName = pLoadCase->GetName();
			pCombo->m_eStatus = eGroupKind;
			pCombo->m_dEta = pLsdManager->GetLsdAppliedEtaiValue(nEtaPos, ELSDCombKind(nComboKind), eLSD_LOADCASE_DIR_PER, nCombo);
			pCombo->m_strCombo = pBri->GetStringCombo((ELSDCombKind)nComboKind, nCombo, TRUE);
 
 			for(long ix = 0; ix < nSizeLoad; ix++)
 			{
				stCantileverLoadUnit *pLoad = vLoad[ix];

 				ELSDLoadCaseKind eLoadCaseKind = pLoad->m_eLoadCaseKind;
 				dFactor = pLoadCase->GetLoadFactor(eLoadCaseKind);
 				if(Compare(dFactor, 0., _T("=")) == FALSE)
 				{
 					stCantileverLoadUnit *pLoadAdd = new stCantileverLoadUnit(pLoad);
 					pLoadAdd->m_dFactor = dFactor;
 					pCombo->AddLoad(pLoadAdd);
 				}
 			}
 
 			pCombo->Calculate();
 			pCheckPosition->AddComboData(pCombo, eGroupKind);
		}
	}
}

void CCalcCantilever::WriteCantilever( CXLFormatText *pXLText, long nColPos, HGBOOL bLeft, long nIdxTitle, long nIdxSubTitle )
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	if( pBri == NULL ) return;

	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;
	
	CCheckCantileverData *pCheckData = pBri->m_pCheckCantilever;
	if( pCheckData == NULL ) return;

	CCantileverCheckPosition *pCheckPos = GetCheckPosition(bLeft);
	if( pCheckPos == NULL ) return;

	CWorkingLoad *pWorkingLoad = pCheckData->GetWorkingLoadData(bLeft);
	if( pWorkingLoad == NULL ) return;

	long nQtyHDan = pBri->GetQtyHDan();
	CGuardWallRC *pGW = pBri->GetGuardWallByHDan( bLeft==TRUE ? 0 : nQtyHDan-1 );
	if(!pGW) return;

	HGBOOL bGuardWallDW = pGW->m_bDWload;

	pXLText->GoPosition(NO_MOVE, 0);
	
	CString szSubTitle = (bLeft==TRUE) ? _T("좌측 캔틸레버부") : _T("우측 캔틸레버부");
	pXLText->AddFormatText(_T("$h$tb10%d.%d %s$n$n"), nIdxTitle, nIdxSubTitle, szSubTitle);
 
	CDomyun *pDom = new CDomyun;
	DrawGuardWall(pDom, pBri, pGW, bLeft );
	long nSizeRow = (long)(pDom->GetExtHeight()/150);

	pXLText->GoPosition(NO_MOVE, COL_POS_3);
	pXLText->InsertDomyun(pDom,m_pStd->m_pView,nSizeRow,CXLFormatText::INSERT_DOM_CURPOS | CXLFormatText::INSERT_DOM_MOVEROW);

	long nIdxBig = 1;
	long nIdxMiddle = 1;

	pXLText->GoPosition(NO_MOVE, 0);
	pXLText->AddFormatText(_T("$n$h$r%d) 하중산정$n"), nIdxBig++);

 	if(pCheckPos->IsLoad(eLSD_LOADCASE_DC1))//방호벽,슬래브
		WriteLoadConcrete(pXLText, nIdxMiddle, bLeft);

	if(pCheckPos->IsLoad(eLSD_LOADCASE_DW1))//포장하중
		WriteLoadPavement(pXLText, nIdxMiddle, bLeft);

	if(pCheckPos->IsLoad(eLSD_LOADCASE_DW1) && pWorkingLoad->GetBodoWeight().m_bExist)//보도하중
		WriteLoadFootWay(pXLText, nIdxMiddle, bLeft);

	if(pCheckPos->IsLoad(eLSD_LOADCASE_PL) && pWorkingLoad->GetWalkLoad())//군중하중
		WriteLoadCrowd(pXLText, nIdxMiddle, bLeft);


	if(pCheckPos->IsLoad(eLSD_LOADCASE_DW1))//방음벽, 난간
	{
		if(pWorkingLoad->GetUserLoad(BANGUM).m_bExist)
			WriteLoadUpperStructure(pXLText, nIdxMiddle, BANGUM, bLeft);
		if(pWorkingLoad->GetUserLoad(NANGAN).m_bExist)
			WriteLoadUpperStructure(pXLText, nIdxMiddle, NANGAN, bLeft);
	}

	if(pCheckPos->IsLoad(eLSD_LOADCASE_LL1))//차량하중
		WriteLoadVehicle(pXLText, nIdxMiddle, bLeft);

	if(pCheckPos->IsLoad(eLSD_LOADCASE_CT11))//충돌하중
	{
		WriteLoadCollision(pXLText, nIdxMiddle, bLeft);
	}

	if(pCheckPos->IsLoad(eLSD_LOADCASE_WS11))//풍하중
		WriteLoadWind(pXLText, nIdxMiddle, bLeft);

	if(pCheckPos->IsLoad(eLSD_LOADCASE_CF))//원심하중
		WriteLoadCentrifugal(pXLText, nIdxMiddle, bLeft);

	//2)하중조합
	nIdxMiddle = 1;
	pXLText->GoPosition(NO_MOVE, 0);
	pXLText->AddFormatText(_T("$n$h$r%d) 하중조합$n"), nIdxBig++);
	
	WriteCombineTotal(pXLText, nIdxMiddle, bLeft); //하중조합

	//3)바닥판 단면검토
	nIdxMiddle = 1;
	pXLText->GoPosition(NO_MOVE, 0);
	pXLText->AddFormatText(_T("$n$h$r%d) Cantilever 단면검토$n"), nIdxBig++);
	
	WriteMinThickSlab(pXLText, nIdxMiddle, bLeft);//바닥판 두께의 적정성 검토

	WriteRebarCheck(pXLText, nIdxMiddle, bLeft);// 단면검토(철근량검토)
	WriteRebarUsability(pXLText, nIdxMiddle, bLeft);

	WriteSupportRebar(pXLText, nIdxMiddle, bLeft);// 배력철근 검토
	
	//4)처짐검토
	pXLText->GoPosition(NO_MOVE, 0);
	pXLText->AddFormatText(_T("$n$h$r%d) Cantilever 처짐검토$n"), nIdxBig++);
	
	WriteDeflectionCheck(pXLText, bLeft);//처짐검토
}

double CCalcCantilever::DrawGuardWall(CDomyun *pDomP, CRcBridgeRebar *pBri, CGuardWallRC* pGW, HGBOOL bLeft )
{
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	double dArea = 0;
	CDomyun Dom(pDomP);
	CString strRet;

	Dom.SetScaleDim(12);
	Dom.SetTextHeight(6);
	Dom.SetDimExo(10);
	Dom.SetDimVLen(15);

	CRebarPlacing rb;
	CTwinVectorArray tvArr, tvArrResult;
	CTwinVector tvTemp, tv;
	CDoubleArray dArrAngleSu;
	CDoubleArray dDimVer;
	CDoubleArray dDimHor;

	long nPos = bLeft==TRUE ? 0 : 1;
	HGDOUBLE dCantilH = pBri->m_dTS - pBri->m_dCantilH1[nPos] - pBri->m_dCantilH2[nPos];		// 직선 높이
	dDimVer.Add(dCantilH);

	GetTvarrayGuardWall(tvArr, bLeft);
	if(tvArr.GetSize() <= 0) return 0;

	tvTemp = tvArr.GetAt(0);
	tv.m_v1 = tvTemp.m_v1;
	tvArr.GetLast(tvTemp);
	tv.m_v2 = tvTemp.m_v2;
	tvArr.Add(tv);

	long i=0; for(i=0; i<tvArr.GetSize()-1; i++)
	{
		tv = tvArr.GetAt(i);
		double dHor = tv.GetHorLength();
		dDimHor.Add(dHor);
	}
	rb.SeperatorTwinVectorArea(tvArr, tvArrResult, dArrAngleSu);

	// 도형으로 대입 /

	long nNum	= 1;
	long nIdx	= 0;
	AhTPADelete(&pBri->m_pArrAreaConc,	(CTwinVectorAreaShape*)0);
	for(i = 0; i < dArrAngleSu.GetSize(); i++)
	{
		CTwinVectorAreaShape *pArea = new CTwinVectorAreaShape;
		for(long j = 0; j < dArrAngleSu.GetAt(i); j++)
		{
			pArea->AddTwinVector(tvArrResult.GetAt(nIdx++));
		}
		pArea->Calculate();
		pArea->m_nNum	= nNum++;
		pBri->m_pArrAreaConc.Add(pArea);

		double dH = toM(pArea->m_dHeight);
		double dW1 = toM(pArea->m_dWidthLower);
		double dW2 = toM(pArea->m_dWidthUpper);

		HGBOOL bRound = pData->IsLsdDesign() == FALSE;
		if(pArea->m_nTypeShape == SHAPE_TV_RECTANGLE)
		{
			dArea += bRound ? Round(dH*dW1,3) : dH*dW1;
			strRet.Format("%s x %s$m+06$r=$r%s %s",
				pData->GetStrDoubleUnitChange(dH, UNIT_CGS_M, pData->m_szM), pData->GetStrDoubleUnitChange(dW1, UNIT_CGS_M, pData->m_szM),
				pData->GetStrDoubleUnitChange(dH*dW1, UNIT_CGS_M2, pData->m_szM2), pData->m_szM2);
		}
		else if(pArea->m_nTypeShape == SHAPE_TV_TRAPEZOID)
		{
			dArea += bRound ? Round((dW1+dW2)/2*dH,3) : (dW1+dW2)/2*dH;
			strRet.Format("(%s + %s)/2 x %s$m+06$r=$r%s %s",
				pData->GetStrDoubleUnitChange(dW1, UNIT_CGS_M, pData->m_szM), pData->GetStrDoubleUnitChange(dW2, UNIT_CGS_M, pData->m_szM),
				pData->GetStrDoubleUnitChange(dH, UNIT_CGS_M, pData->m_szM), pData->GetStrDoubleUnitChange((dW1+dW2)/2*dH, UNIT_CGS_M2, pData->m_szM2), pData->m_szM2);
		}
		else if(pArea->m_nTypeShape == SHAPE_TV_TRIANGLE)
		{
			dArea += bRound ? Round(dH*dW1/2,3) : dH*dW1/2;
			strRet.Format("(%s x %s)/2$m+06$r=$r%s %s", pData->GetStrDoubleUnitChange(dH, UNIT_CGS_M, pData->m_szM), 
				pData->GetStrDoubleUnitChange(dW1, UNIT_CGS_M, pData->m_szM), pData->GetStrDoubleUnitChange(dH*dW1/2, UNIT_CGS_M2, pData->m_szM2), pData->m_szM2);
		}
		else continue;

		dDimVer.Add(pArea->m_dHeight);
	}

	// 번호 붙여서 출력 modified by jaeho 2004.04.09
	CHgBaseDrawStd baseDraw;

	// 2 번째 방호벽 타입
// 	long nQtyHDan = pBri->GetQtyHDan();
// 	CGuardWallRC *pGWNext = pBri->GetGuardWallByHDan( bLeft==TRUE ? 1 : nQtyHDan-2 );
// 	if(!pGWNext) return dArea;
// 
// 	pGWNext->GetTvGuardWallBase(tvArr, FALSE);
// 	baseDraw.DrawTvArray(&Dom, tvArr);
	//////////////////////////////////////////////////////////////////////////

	CTwinVectorArray tvArrCantil;
	GetTvarrayCantilever(tvArrCantil, bLeft );
	baseDraw.DrawTvArray(&Dom, tvArrCantil);

	long nSize = pBri->m_pArrAreaConc.GetSize();
	for ( long nArr=0; nArr<nSize; ++nArr )
	{
		CTwinVectorAreaShape* pArea	= pBri->m_pArrAreaConc.GetAt(nArr);
		if( NULL == pArea ) continue;

		baseDraw.DrawTvArray(&Dom, pArea->m_tvArr);
	}

	// 캔틸레버
//	baseDraw.DrawAreaSectionNumber(&Dom, &pBri->m_pArrAreaConc, 1);

	double dDist = 0;
	CDRect rc = Dom.GetExtRect();
	CDPoint xyStt = rc.TopRight();

	Dom.SetDirection("RIGHT");
	Dom.DimMoveTo(xyStt.x, xyStt.y, 0, "");
	for(i=0; i<dDimVer.GetSize(); i++)
	{
		dDist = dDimVer.GetAt(i);
		Dom.DimLineTo( dDist, 0, COMMA(dDist));
	}

	Dom.SetDirection("BOTTOM");
	dDist = rc.Width();
	Dom.DimMoveTo(xyStt.x, xyStt.y, 0, "");
	Dom.DimLineTo( -dDist, 0, COMMA(dDist));

	long nSign = bLeft ? 1 : -1;
	Dom.SetDirection("TOP");
	if( bLeft )
		xyStt = rc.BottomLeft();
	else
		xyStt = rc.BottomRight();
	Dom.DimMoveTo(xyStt.x, xyStt.y, 0, "");

	dDist = pGW->m_dW1;
	if(dDist > 100) Dom.DimLineTo( dDist * nSign, 0, COMMA(dDist));
	else Dom.DimLineToExtend( dDist * nSign, 0, "", COMMA(dDist));

	dDist = pGW->m_dW2;
	if(dDist > 100) Dom.DimLineTo( dDist * nSign, 0, COMMA(dDist));
	else Dom.DimLineToExtend( dDist * nSign, 0, "", COMMA(dDist));

	dDist = pGW->m_dW3;
	if(dDist > 100) Dom.DimLineTo( dDist * nSign, 0, COMMA(dDist));
	else Dom.DimLineToExtend( dDist * nSign, 0, "", COMMA(dDist));

	dDist = pGW->m_dW4;
	if(dDist > 100) Dom.DimLineTo( dDist * nSign, 0, COMMA(dDist));
	else Dom.DimLineToExtend( dDist * nSign, 0, "", COMMA(dDist));

	dDist = pGW->m_dW5;
	if(dDist > 100) Dom.DimLineTo( dDist * nSign, 0, COMMA(dDist));
	else Dom.DimLineToExtend( dDist * nSign, 0, "", COMMA(dDist));

	dDist = pGW->m_dW6;
	if(dDist > 100) Dom.DimLineTo( dDist * nSign, 0, COMMA(dDist));
	else Dom.DimLineToExtend( dDist * nSign, 0, "", COMMA(dDist));

	dDist = pGW->m_dW7;
	if(dDist > 100) Dom.DimLineTo( dDist * nSign, 0, COMMA(dDist));
	else Dom.DimLineToExtend( dDist * nSign, 0, "", COMMA(dDist));

	*pDomP << Dom;
	return dArea;
}

void CCalcCantilever::WriteLoadConcrete(CXLFormatText *pXLText, long &nIdxMiddle, HGBOOL bLeft)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	CCantileverCheckPosition *pCheckPos = GetCheckPosition(bLeft);
	if(pCheckPos == NULL) return;

	double dDistCantilever = GetDistCantilever(bLeft);
	double dDistArea = 0;
	double dB = 1000.;

	CString strLoadName = STR_LOAD_CONCRETE;
	long nLR = bLeft ? iLEFT : iRIGHT;
	long nQtyHDan = pBri->GetQtyHDan();
	CGuardWallRC *pGW = pBri->GetGuardWallByHDan( bLeft==TRUE ? 0 : nQtyHDan-1 );
	HGBOOL bGuardWallDW = pGW->m_bDWload;

	for(long ixLoadType = eSlabDC; ixLoadType <= eSlabDW; ++ixLoadType)
	{
		if((ixLoadType == eSlabDW) && bGuardWallDW == FALSE) continue;
		if(bGuardWallDW)
			strLoadName = (ixLoadType == eSlabDC) ? STR_LOAD_SLAB : STR_LOAD_GUARDWALL;

		stCantileverLoadUnit *pUnitLoad = pCheckPos->GetLoad(strLoadName);
		if(pUnitLoad == NULL) continue;

		long icArea = m_vArea[ixLoadType][nLR].size();

		pXLText->GoPosition(NO_MOVE, 0);
		pXLText->AddFormatText(_T("$s+%02d$h$m+02(%d) %s (%s) $m+22$i(단위 m당)$n")
			, icArea+1, nIdxMiddle++, strLoadName, pBri->GetLoadCaseName(ixLoadType == eSlabDC ? eLSD_LOADCASE_DC1 : eLSD_LOADCASE_DW1));

		pXLText->GoPosition(NO_MOVE, COL_POS_2);
		pXLText->AddTable(icArea+1, 22, 1, 2);

		pXLText->AddFormatText(_T("$h$me1$cNo.$m+02$me9$c산   식 (체적 x 단위중량)$m+10$me2$c작용하중(N)$m+03$me3$c작용거리(mm)$m+04$me3$c모멘트(N·mm)$n"));

		for(long ix = 0; ix < icArea; ++ix)
		{
			CTwinVectorAreaShape *pArea = m_vArea[ixLoadType][nLR][ix];
			if(pArea == NULL) continue;

			long icArr = pArea->m_tvArr.GetSize();
			double dL = pArea->GetWidth();
			double dH = pArea->GetHeight();
			double dWeight = 0; //작용하중

			pXLText->AddFormatText(_T("$h$me1$c%d"), ix+1); //No.

			//산식
			if(icArr == 3) //삼각형
			{
				pXLText->AddFormatText(_T("$m+02$me9$e %4.f x %3.f x 1000 x %f x 1/2"), dL, dH, m_dUnitWeightSteelConc);
				dWeight = (dL * dH * dB * m_dUnitWeightSteelConc)/2;
			}
			else if(icArr == 4) //사각형
			{
				pXLText->AddFormatText(_T("$m+02$me9$e %4.f x %3.f x 1000 x %f"), dL, dH, m_dUnitWeightSteelConc);
				dWeight = dL * dH * dB * m_dUnitWeightSteelConc;
			}

			dDistArea = fabs(pArea->m_xyCen.x - dDistCantilever * (bLeft? 1 : -1));

			//작용하중
			pXLText->AddFormatText(_T("$m+10$me2$f[#,##0_ ]$i%s "), CommaC(dWeight));

			//작용거리
			pXLText->AddFormatText(_T("$m+03$me3$i$f[#,##0_ ]%s "), CommaC(dDistArea));

			//모멘트
			pXLText->AddFormatText(_T("$m+04$me3$i$f[#,##0_ ]%s $n"), CommaC(dWeight * dDistArea));
		}

		double dForceVert = pUnitLoad->m_dForceVert;
		double dMoment = pUnitLoad->GetMoment(FALSE);

		pXLText->AddFormatText(_T("$h"));
		pXLText->AddMergeFormatText(0, 11, _T("$c합 계"));
		pXLText->AddFormatText(_T("$h$m+12$me2$i$f[#,##0_ ]%s$m+03$me3$m+04$me3$c$i$f[#,##0_ ]%s$n"), CommaC(dForceVert), CommaC(dMoment));

		*pXLText += 1;
	}

	pXLText->GoPosition(NO_MOVE, 0);
}

void CCalcCantilever::WriteLoadPavement(CXLFormatText *pXLText, long &nIdxMiddle, HGBOOL bLeft)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	CCantileverCheckPosition *pCheckPos = GetCheckPosition(bLeft);
	if(pCheckPos == NULL) return;

	stCantileverLoadUnit *pUnitLoad = pCheckPos->GetLoad(STR_LOAD_PAVEMENT);
	if(pUnitLoad == NULL) return;

	long nQtyHDan = pBri->GetQtyHDan();
	CGuardWallRC *pGW = pBri->GetGuardWallByHDan( bLeft==TRUE ? 0 : nQtyHDan-1 );

//	long nHDanType = 0; // 없음(포장)
	CDPoint xyPave = GetXyHDanByType(bLeft); // x = 시작위치, y = 끝위치 (슬래브끝단기준)

	double dLength = fabs(xyPave.x - xyPave.y);
	double dThick = pBri->GetThickPave();

	double dLoadPosition = pUnitLoad->m_xyLoadPosition.x;
	double dMoment = pUnitLoad->GetMoment(FALSE);

	pXLText->AddFormatText(_T("$h$m+02(%d) %s (%s)$n"), nIdxMiddle++), pUnitLoad->sName, pBri->GetLoadCaseName(pUnitLoad->GetLoadCaseKind());
	pXLText->AddFormatText(_T("$h$m+03$cM`(p)$r$c'=$r%s x %s x %s x %f x %s $m+09$c'=$r%s N.m$n$n"), CommaC(dLength), CommaC(dThick), CommaC(_1000MM), pGW->m_dUnitWeightPave, CommaC(dLoadPosition), CommaC(dMoment));
}

void CCalcCantilever::WriteLoadFootWay(CXLFormatText *pXLText, long &nIdxMiddle, HGBOOL bLeft)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	CCantileverCheckPosition *pCheckPos = GetCheckPosition(bLeft);
	if(pCheckPos == NULL) return;

	stCantileverLoadUnit *pUnitLoad = pCheckPos->GetLoad(STR_LOAD_FOOTWAY);
	if(pUnitLoad == NULL) return;

	CCheckCantileverData *pCheckData = pBri->m_pCheckCantilever;
	if( pCheckData == NULL ) return;

	CWorkingLoad *pWorking = pCheckData->GetWorkingLoadData(bLeft);
	if( pWorking == NULL ) return;

	CDPoint xyFootway = GetXyHDanByType(bLeft); // x = 시작위치, y = 끝위치 (슬래브끝단기준)

	double dLengthLoad = fabs(xyFootway.x - xyFootway.y);
	double dLengthFootWay = GetLengthSupport(bLeft, TRUE);
	double dLoadStt = GetValueUnitChange( pWorking->GetBodoWeight().m_dWeightStt, UNIT_CGS_TONF_M2, pData->m_szTonf_M2) * _1000MM;
	double dLoadEnd = GetValueUnitChange(pWorking->GetBodoWeight().m_dWeightEnd, UNIT_CGS_TONF_M2, pData->m_szTonf_M2) * _1000MM;
	dLoadEnd = dLoadStt - (dLoadStt - dLoadEnd) * dLengthLoad / dLengthFootWay;

	double dLoadPosition = pUnitLoad->m_xyLoadPosition.x;
	double dMoment = pUnitLoad->GetMoment(FALSE);

	pXLText->AddFormatText(_T("$h$m+02(%d) %s (%s)$n"), nIdxMiddle++, pUnitLoad->sName, pBri->GetLoadCaseName(pUnitLoad->GetLoadCaseKind()));
	pXLText->AddFormatText(_T("$h$m+03$cM`(pd)$r$c'=$r%s x (%s + %s) / 2 x %s$m+09$c'=$r%s N.mm$n$n"), pData->GetStrDouble(dLengthLoad, UNIT_CGS_M), pData->GetStrDouble(dLoadStt, UNIT_CGS_TONF_M), pData->GetStrDouble(dLoadEnd, UNIT_CGS_TONF_M), pData->GetStrDouble(dLoadPosition, UNIT_CGS_M), pData->GetStrDouble(dMoment, UNIT_CGS_TONFM));
}

void CCalcCantilever::WriteLoadCrowd(CXLFormatText *pXLText, long &nIdxMiddle, HGBOOL bLeft)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;

	CCantileverCheckPosition *pCheckPos = GetCheckPosition(bLeft);
	if(pCheckPos == NULL) return;

	stCantileverLoadUnit *pUnitLoad = pCheckPos->GetLoad(STR_LOAD_CROWD);
	if(pUnitLoad == NULL) return;

	CDPoint xyFootway = GetXyHDanByType(bLeft); // x = 시작위치, y = 끝위치 (슬래브끝단기준)

	double dLoad = N_CROWD_LOAD * _1000MM;
	double dLength = fabs(xyFootway.x - xyFootway.y);
//	double dLoadPosition = pUnitLoad->m_xyLoadPosition.x;
	double dMoment = pUnitLoad->GetMoment(FALSE);

	pXLText->AddFormatText(_T("$h$m+02(%d) %s (%s)$n"), nIdxMiddle++, pUnitLoad->sName, pBri->GetLoadCaseName(pUnitLoad->GetLoadCaseKind()));
	pXLText->AddFormatText(_T("$h$m+03$cM`(f)$r$c=$r%s kN/m² x L² / 2  (보도 5.0 kN/m² 등분포하중 재하)$n"), CommaC(dLoad));
	pXLText->AddFormatText(_T("$h$m+04$c=$r%.3f x %s x %s² / 2$m+09$c'=$r%s N.mm$n$n"), N_CROWD_LOAD, CommaC(_1000MM), CommaC(dLength), CommaC(dMoment));
}

void CCalcCantilever::WriteLoadUpperStructure(CXLFormatText *pXLText, long &nIdxMiddle, long nType, HGBOOL bLeft)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	CCantileverCheckPosition *pCheckPos = GetCheckPosition(bLeft);
	if(pCheckPos == NULL) return;

	CString strLoadName = (nType == BANGUM) ? STR_LOAD_BANGUM : STR_LOAD_NANGAN;

	stCantileverLoadUnit *pUnitLoad = pCheckPos->GetLoad(strLoadName);
	if(pUnitLoad == NULL) return;

	CCheckCantileverData *pCheckData = pBri->m_pCheckCantilever;
	if( pCheckData == NULL ) return;

	CWorkingLoad *pWorking = pCheckData->GetWorkingLoadData(bLeft);
	if( pWorking == NULL ) return;

	double dVForce = pUnitLoad->m_dForceVert;
	double dLoadPosition = pUnitLoad->m_xyLoadPosition.x;
	double dMoment = pUnitLoad->GetMoment(FALSE);

	pXLText->AddFormatText(_T("$h$m+02(%d) %s (%s)$n"), nIdxMiddle++, pUnitLoad->sName, pBri->GetLoadCaseName(pUnitLoad->GetLoadCaseKind()));
	pXLText->AddFormatText(_T("$h$m+03$cM`(s)$r$c'=$r%s x %s$m+09$c'=$r%s N.mm$n$n"), CommaC(dVForce, 0, FALSE), CommaC(dLoadPosition), CommaC(dMoment, 0, FALSE));
}

void CCalcCantilever::WriteLoadVehicle(CXLFormatText *pXLText, long &nIdxMiddle, HGBOOL bLeft)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	CCantileverCheckPosition *pCheckPos = GetCheckPosition(bLeft);
	if(pCheckPos == NULL) return;

	stCantileverLoadUnit *pUnitLoad = pCheckPos->GetLoad(STR_LOAD_VEHICLE);
	if(pUnitLoad == NULL) return;

	CCheckCantileverData *pCheckData = pBri->m_pCheckCantilever;
	if( pCheckData == NULL ) return;

	CWorkingLoad *pWorking = pCheckData->GetWorkingLoadData(bLeft);
	if( pWorking == NULL ) return;

	long nIdxSmall = 1;

	pXLText->AddFormatText(_T("$s+05$h$m+02(%d) %s (%s)$m+23$i[도.설(2015) 3.6.1]$n")
		, nIdxMiddle++, STR_LOAD_VEHICLE, pBri->GetLoadCaseName(pUnitLoad->GetLoadCaseKind()));
	pXLText->AddFormatText(_T("$h$m+03%s 충격하중계수 (IM)$m+22$i[도.설(2015) 3.7.1]$n"), GetNumberString(nIdxSmall++,NUM_CIRCLE) );

	pXLText->GoPosition(NO_MOVE, COL_POS_3);
	pXLText->AddFormatText(_T("$h"));
	pXLText->AddTable(2, 22, 1, 19);
	pXLText->AddFormatText(_T("$h$me7$c성분$m+06$me9$m+08$me4$m+05$me3$cIM$n"));
	pXLText->AddFormatText(_T("$h$me6$md1$c$j$c바닥판 신축이음장치를 제외한 모든 다른 부재$m+07$me7$c피로한계상태를 제외한 모든 한계상태$m+06$me5 $m+06$me3$c25%%$n"));
	pXLText->AddFormatText(_T("$h$m+07$me7$c피로한계상태$m+06$me5 $m+06$me3$c15%%$n"));

	pXLText->GoPosition(NO_MOVE, 0);

	*pXLText += 1;

	long nLR = bLeft == TRUE ? iLEFT : iRIGHT;

	double dEl = (double)VEHICLE_FACTOR_E1;
	double dE2 = (double)VEHICLE_FACTOR_E2;
	double dX = GetLengthSupport(bLeft);
	double dE = dEl * dX + dE2;

	pXLText->AddFormatText(_T("$h$m+03%s 윤하중 분포폭$m+22$i[도.설(2015) 4.6.2.5]$n"), GetNumberString(nIdxSmall++,NUM_CIRCLE));
	pXLText->AddFormatText(_T("$h$m+04'- 일반활하중 작용시$n"));
	pXLText->AddFormatText(_T("$h$m+05E$r$c'=$r%.1f x X + %s = %.1f x %.f + %s = %s mm$n"), dEl, CommaC(dE2), dEl, dX, CommaC(dE2), CommaC(dE));

	//탱크트레일러 작용시
	if(pCheckData->m_bFloorTrailer)
	{
		pXLText->AddFormatText(_T("$h$m+04'- 탱크트레일러하중 작용시$n"));
		pXLText->AddFormatText(_T("$h$m+05E$r$c'=$r%.1f x L + %s = %.1f x %s + %s = %s mm$n"), dEl, CommaC(dE2), dEl, CommaC(sVC[nLR].dL3), CommaC(dE2), CommaC(sVC[nLR].dE3));
	}
	*pXLText += 1;

	pXLText->AddFormatText(_T("$h$m+03%s 휨모멘트$n"), GetNumberString(nIdxSmall++,NUM_CIRCLE));
	pXLText->AddFormatText(_T("$h$m+04'- 일반활하중 작용시$n"));
	pXLText->AddFormatText(_T("$h$m+05M`(LL)$r$c'=$r(Pr / E) x X x (1 + IM) = (%s / %s x %s) x %s x (1 + %.3f)$n"), CommaC(sVC[nLR].dPr), CommaC(dE), CommaC(_1000MM), CommaC(dX), _dImpactLoad[1]);
	pXLText->AddFormatText(_T("$h$m+06$c'=$r%s N·m$n"), CommaC(sVC[nLR].dMl));

	if(pCheckData->m_bFloorTank)
	{
		HGDOUBLE m_dTankEdit1 = 100;		// 탱크 바퀴간격
		pXLText->AddFormatText(_T("$h$m+04'- 탱크궤도하중 작용시$m+21$i[방호공학 P48, 군용 충격계수 15%% 적용]$n"));
		pXLText->AddFormatText(_T("$h$m+05Pr$m+02$c'=$rW / 2 / L(탱크접지길이) = %s / 2 / %.3f = %s kN$n"), CommaC(sVC[nLR].dTankW), m_dTankEdit1, CommaC(sVC[nLR].dP2));
		pXLText->AddFormatText(_T("$h$m+05M`(LL.TK)$m+02$c'=$rPr x L x (1 + IMTL) = %s x %s x (1 + %.3f)$n"), CommaC(sVC[nLR].dP2), CommaC(sVC[nLR].dL2), _dImpactLoad[2]);
		pXLText->AddFormatText(_T("$h$m+07$c'=$r%s N·mm$n"), CommaC(sVC[nLR].dMtank));
	}
	if(pCheckData->m_bFloorTrailer)
	{
		pXLText->AddFormatText(_T("$h$m+04'- 탱크트레일러하중 작용시$n"));
		pXLText->AddFormatText(_T("$h$m+05M`(LL.TL)$m+02$c'=$r(Pr / E) x L x (1 + IMTL) = (%s / %s x %s) x %s x (1 + %.3f)$n"), CommaC(sVC[nLR].dTrailerW), CommaC(sVC[nLR].dE3), CommaC(_1000MM), CommaC(sVC[nLR].dL3), _dImpactLoad[2]);
		pXLText->AddFormatText(_T("$h$m+07$c'=$r%s N·mm$n"), CommaC(sVC[nLR].dMTrail));
	}

	*pXLText += 1;
}

void CCalcCantilever::WriteLoadWind(CXLFormatText *pXLText, long &nIdxMiddle, HGBOOL bLeft)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	CCantileverCheckPosition *pCheckPos = GetCheckPosition(bLeft);
	if(pCheckPos == NULL) return;

	stCantileverLoadUnit *pUnitLoad = pCheckPos->GetLoad(STR_LOAD_WIND);
	if(pUnitLoad == NULL) return;

	CCheckCantileverData *pCheckData = pBri->m_pCheckCantilever;
	if( pCheckData == NULL ) return;

	CWorkingLoad *pWorking = pCheckData->GetWorkingLoadData(bLeft);
	if( pWorking == NULL ) return;

	long nQtyHDan = pBri->GetQtyHDan();
	CGuardWallRC* pGw = pBri->GetGuardWallByHDan(bLeft==TRUE ? 0 : nQtyHDan-1);		// 끝단을 기준으로 한다.
	if( pGw == NULL ) return;


	BOOL bBodo = (pWorking->GetBodoWeight().m_bExist && pWorking->GetBodoWeight().m_dHeight > 0);
	double dPaveT = bBodo ? frM(pWorking->GetBodoWeight().m_dHeight) : pBri->GetThickPave();	// 포장두께 
	double dSlabT =	pBri->m_dTS;	// 슬래브 기준 두께	

	double dWindStrength = GetValueUnitChange(pCheckData->m_dWindStrength, UNIT_CGS_TONF_M2, pData->m_szTonf_M2);	// 풍하중 강도
	double dHeight = pGw->GetHeight();
	double dForceHori = pUnitLoad->m_dForceHori;

	pXLText->AddFormatText(_T("$h$m+02(%d) %s (%s)$m+23$i[도.설(2015) 3.13.2.1]$n")
		, nIdxMiddle++, pUnitLoad->sName, pBri->GetLoadCaseName(pUnitLoad->GetLoadCaseKind()));

	pXLText->AddFormatText(_T("$h$m+03Pw$r$c'=$r%s kN/m²x H = %.3f x %s x %s$n"), CommaC(dWindStrength * _1000MM), dWindStrength, CommaC(_1000MM), CommaC(dHeight, 0, FALSE));
	pXLText->AddFormatText(_T("$h$m+04$c'=$r%s N/m$n"), CommaC(pUnitLoad->m_dForceHori, 0, FALSE));

	pXLText->AddFormatText(_T("$h$m+03Mw$r$c'=$rPw x ( H / 2 + Pt + t/2 ) = %s x ( %s / 2 + %s + %s / 2 )$n"), CommaC(dForceHori, 0, FALSE), CommaC(dHeight, 0, FALSE), CommaC(dPaveT), CommaC(dSlabT));
	pXLText->AddFormatText(_T("$h$m+04$c'=$r%s N·mm$n$n"), CommaC(pUnitLoad->GetMoment(FALSE), 0, FALSE));
}

void CCalcCantilever::WriteLoadCollision(CXLFormatText *pXLText, long &nIdxMiddle, HGBOOL bLeft)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	CCantileverCheckPosition *pCheckPos = GetCheckPosition(bLeft);
	if(pCheckPos == NULL) return;

	long nLR = bLeft == TRUE ? iLEFT : iRIGHT;
	stCantileverLoadUnit *pUnitLoad = pCheckPos->GetLoad(m_bCollisionVehicle[nLR] ? STR_LOAD_COLLISION_VEHICLE : STR_LOAD_COLLISION_CROWD);
	if(pUnitLoad == NULL) return;

	CCheckCantileverData *pCheckData = pBri->m_pCheckCantilever;
	if( pCheckData == NULL ) return;

	CWorkingLoad *pWorking = pCheckData->GetWorkingLoadData(bLeft);
	if( pWorking == NULL ) return;

	long nQtyHDan = pBri->GetQtyHDan();
	CGuardWallRC* pGw = pBri->GetGuardWallByHDan(bLeft==TRUE ? 0 : nQtyHDan-1);		// 끝단을 기준으로 한다.
	if( pGw == NULL ) return;

	double dLendSupport = GetLengthSupport(bLeft);
	BOOL bExistFootway = pWorking->GetBodoWeight().m_bExist;
	double dHeightNangan = pWorking->GetUserLoad(NANGAN).m_bExist ? pWorking->GetUserLoad(NANGAN).m_dHeight : 0;
	double dGuardH	= pGw->GetHeight() + pBri->m_dTS / 2 + dHeightNangan;
// 	double dTerm = m_pStd->GetTermCantilever(pBx, bLeft);
// 	double dSharp = m_pStd->GetSharpCantilever(pBx, bLeft);

	double dTerm = pBri->m_dCantilB[bLeft==TRUE ? 0 : 1];

	long nIdxSmall = 1;

	if(bExistFootway && dLendSupport > 0) // 보도가 있고 차도가 있는 경우
	{
		pXLText->AddFormatText(_T("$h$m+02 계산서 타입 추가$n"));
	}
	else if(!bExistFootway) // 보도가 없고 방호벽 혹은 중분대
	{
		double dVelocity= pCheckData->m_dCo_V;
		double dHeight = COLLISION_H1 + pBri->GetThickPave() + pBri->m_dTS / 2; // 수평 충돌 높이

		double dR = pCheckData->m_dCenter_R[bLeft ? iLEFT : iRIGHT];
		double dH = sCO[nLR].dH;

		pXLText->AddFormatText(_T("$h$m+02(%d) %s (%s)$m+23$i[도.설.해(2008) 2.4.3.6]$n"), nIdxMiddle++, pUnitLoad->sName, pBri->GetLoadCaseName(pUnitLoad->GetLoadCaseKind()));
		pXLText->AddFormatText(_T("$h$m+03%s 수평충돌력  H가 노면상 1m 높이에 교축을 따라 1m 마다 작용하는 것으로 한다.$n"), GetNumberString(nIdxSmall++,NUM_CIRCLE));
		pXLText->AddFormatText(_T("$h$m+04H = (V / 60)² x %s + %s = (%s / 60)²x %s + %s = %s N$n"), CommaC(COLLISION_V750), CommaC(COLLISION_V250), CommaC(dVelocity), CommaC(COLLISION_V750), CommaC(COLLISION_V250), CommaC(dH));
		pXLText->AddFormatText(_T("$h$m+04작용 높이 h = %s mm$n"), CommaC(dHeight));
		pXLText->AddFormatText(_T("$h$m+04작용모멘트 M`(CT1) = H x h = %s x %s = %s N·mm$n$n"), CommaC(dH), CommaC(dHeight), CommaC(dH * dHeight));
		pXLText->AddFormatText(_T("$h$m+03%s 방호벽 상단에 횡방향으로 10 kN/m의 수평하중을 재하한다.$n"), GetNumberString(nIdxSmall++,NUM_CIRCLE));
		pXLText->AddFormatText(_T("$h$m+04작용 높이 h = %s mm$n"), CommaC(dGuardH));
		pXLText->AddFormatText(_T("$h$m+04작용모멘트 M`(CT2) = H x h = %s x %s = %s N·mm$n$n"), CommaC(COLLISION_UNIT_H), CommaC(dGuardH), CommaC(COLLISION_UNIT_H * dGuardH));

		if(dR > 0 && dR <= COLLISION_Rstd)
		{
			pXLText->AddFormatText(_T("$h$m+04곡률반지름 R = %s m ≤ %s m 이므로, ①과 ②의 값 중 큰 값의 2배를 적용한다.$n"),CommaC(dR), CommaC(COLLISION_Rstd));
		}
		else
		{
			pXLText->AddFormatText(_T("$h$m+04곡률반지름 R = %s m > %s m 이므로, ①과 ②의 값 중 큰 값을 적용한다.$n"),CommaC(dR), CommaC(COLLISION_Rstd));
		}

		pXLText->AddFormatText(_T("$h$m+04Mco = %s N·mm$n$n"), CommaC(pUnitLoad->GetMoment(FALSE)));
	}
	else  if(bExistFootway && dLendSupport <= 0) // 보도만 있는 경우
	{
		double dSharp = 200;

		double dH = dGuardH + pWorking->GetUserLoad(NANGAN).m_dHeight;
		double LEN = pBri->GetLengthHDan(bLeft==TRUE ? 0 : nQtyHDan-1);
		double dDist = (dTerm - dSharp/2 - LEN) + (bLeft ? (pGw->m_dW5+pGw->m_dW4) : (pGw->m_dW1+pGw->m_dW2)) + pGw->m_dW3 - sCO[nLR].dLoadPos;

		pXLText->AddFormatText(_T("$h$m+02(%d) %s (%s)$m+23$i[도.설.해(2008) 9.7.3]$n"), nIdxMiddle++, pUnitLoad->sName, pBri->GetLoadCaseName(pUnitLoad->GetLoadCaseKind()));
		pXLText->AddFormatText(_T("$h$m+03%s 수평충돌력 H와 수직력 V가 상단부와 난간 정상부에 작용하는 것으로 한다.$n"), GetNumberString(nIdxSmall++,NUM_CIRCLE));
		pXLText->AddFormatText(_T("$h$m+04수평 충돌력$m+04H $r$c'= $r$c$me1%s $r$rN$n"), CommaC(sCO[nLR].dH));
		pXLText->AddFormatText(_T("$h$m+04수직력$m+04V $r$c'= $r$c$me1%s $r$rN$n"), CommaC(sCO[nLR].dV));
		pXLText->AddFormatText(_T("$h$m+04작용 모멘트 Mct = %s x %s + %s x %s = %s N·mm$n"), CommaC(sCO[nLR].dH), CommaC(dH), CommaC(sCO[nLR].dV), CommaC(dDist), CommaC(pUnitLoad->GetMoment(FALSE)));
	}
	*pXLText += 1;
}

double CCalcCantilever::GetLengthSupport( BOOL bLeft, BOOL bFootWay/*=FALSE*/ )
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;

	double dLenCantilever = GetDistCantilever(bLeft);
	long nQtyHDan = pBri->GetQtyHDan();
	CGuardWallRC *pGW = pBri->GetGuardWallByHDan( bLeft==TRUE ? 0 : nQtyHDan-1 );
	
	long nFindType = bFootWay? HDAN_TYPE_ROAD_FOOTWAY : HDAN_TYPE_ROAD_NONE;

	if(bLeft)
	{
		for(long ix=0; ix<nQtyHDan; ++ix)
		{
			CGuardWallRC *pGW = pBri->GetGuardWallByHDan( bLeft==TRUE ? 0 : nQtyHDan-1 );
			if(pGW && pGW->m_nTypeGuardWall == nFindType)
			{
				break;
			}
			dLenCantilever -= pGW->GetWidth();
			if(dLenCantilever <= 0)
				break;
		}
	}
	else
	{
		for(long ix=nQtyHDan-1; ix<0; ix--)
		{
			CGuardWallRC *pGW = pBri->GetGuardWallByHDan( bLeft==TRUE ? 0 : nQtyHDan-1 );
			if(pGW && pGW->m_nTypeGuardWall == nFindType)
			{
				break;
			}
			dLenCantilever -= pGW->GetWidth();
			if(dLenCantilever <= 0)
				break;
		}
	}

	return MAX(dLenCantilever, 0);
}

double CCalcCantilever::CalcCentriodLoad_CF( double V, double R )
{
	double CF = 0;
	double A = 0.79;

	CF = A * (V*V) / R;
	return CF;
}

BOOL CCalcCantilever::WriteLoadCentrifugal( CXLFormatText *pXLText, long &nIdxMiddle, HGBOOL bLeft )
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CARcBridgeDataStd *pData = pBri->m_pARcBridgeDataStd;

	CCantileverCheckPosition *pCheckPos = GetCheckPosition(bLeft);
	if(pCheckPos == NULL) return FALSE;

	long nLR = bLeft == TRUE ? iLEFT : iRIGHT;
	stCantileverLoadUnit *pUnitLoad = pCheckPos->GetLoad(STR_LOAD_CENTRIFUGAL);
	if(pUnitLoad == NULL) return FALSE;

	CCheckCantileverData *pCheckData = pBri->m_pCheckCantilever;
	if( pCheckData == NULL ) return FALSE;

	CWorkingLoad *pWorking = pCheckData->GetWorkingLoadData(bLeft);
	if( pWorking == NULL ) return FALSE;

	double dR		= pCheckData->m_dCenter_R[bLeft ? iLEFT : iRIGHT];					// 설계반경 
	double dV		= pCheckData->m_dCo_V;						// 설계속도 : km/hr
	double dV_sec	= pCheckData->m_dCo_V * 1000. / (60.0 * 60.0);						// 설계속도 : m/sec

	BOOL bTankOrTarailer = (pCheckData->m_bFloorTank || pCheckData->m_bFloorTrailer) ? TRUE : FALSE;

	pXLText->AddFormatText(_T("$h$m+02(%d) %s (%s)$m+23$i[도.설(2015) 3.18]$n"), nIdxMiddle++, pUnitLoad->sName, pBri->GetLoadCaseName(pUnitLoad->GetLoadCaseKind()));
	
	pXLText->AddFormatText(_T("$h$m+03※ 설계속도  V = %4.1f km/hr = %4.1f m/s $m+04(일반차량)$n"), dV, dV_sec);
	if(bTankOrTarailer)
	{
		double dVT		= CENTRIFUGAL_TANK_V;					// 탱크 or 트레일러 설계속도
		double dVT_sec	= CENTRIFUGAL_TANK_V * 1000. / (60.0 * 60.0);					// 탱크 or 트레일러 설계속도

		pXLText->AddFormatText(_T("$h$m+03※ 설계속도  V = %4.1f km/hr = %4.1f m/s $m+04(탱크, 탱크트레일러 차량)$n$n"), dVT, dVT_sec);
	}

	pXLText->AddFormatText(_T("$h$m+03- 일반활하중 작용시$n"));
	pXLText->AddFormatText(_T("$h$m+04C = 4 / 3 x  V² / (g x R) = 4 / 3 x 33.333² / (9.81 x %.1f) = %.3f$n"), dR, sCF[nLR].dCf1);
	pXLText->AddFormatText(_T("$h$m+04Pcf = (Pr/E) x C = (%.1f/%.1f) x %.3f = %.3f$n"), sCF[nLR].dPr1, sCF[nLR].dE1, sCF[nLR].dCf1, sCF[nLR].dPcf1);
	pXLText->AddFormatText(_T("$h$m+04Mcf = Pcf x %.1f = %.3f x %.1f = %.3f %s$n"), sCF[nLR].dHeight, sCF[nLR].dPcf1, sCF[nLR].dHeight, sCF[nLR].dMcf1, pData->m_szTonfM);

	if(pCheckData->m_bFloorTank)
	{
		pXLText->AddFormatText(_T("$h$m+03- 탱크궤도하중 작용시$n"));
		pXLText->AddFormatText(_T("$h$m+04C = 4 / 3 x  V² / (g x R) = 4 / 3 x 11.111² / (9.81 x %.1f) = %.3f$n"), dR, sCF[nLR].dCf2);
		pXLText->AddFormatText(_T("$h$m+04Pcf = Pr x C = %.1f x %.1f = %.3f N $n"), sCF[nLR].dPr2, sCF[nLR].dCf2, sCF[nLR].dPcf2);
		pXLText->AddFormatText(_T("$h$m+04Mcf = Pcf x %.1f = %.3f x %.1f = %.3f %s$n"), sCF[nLR].dHeight, sCF[nLR].dPcf2, sCF[nLR].dHeight, sCF[nLR].dMcf2, pData->m_szTonfM);
	}

	if (pCheckData->m_bFloorTrailer)
	{
		pXLText->AddFormatText(_T("$h$m+03- 탱크트레일러하작용시$n"));
		pXLText->AddFormatText(_T("$h$m+04C = 4 / 3 x  V² / (g x R) = 4 / 3 x 11.111² / (9.81 x %.1f) = %.3f$n"), dR, sCF[nLR].dCf3);
		pXLText->AddFormatText(_T("$h$m+04Pcf = Pr x C = %.1f x %.1f = %.3f N $n"), sCF[nLR].dPr3, sCF[nLR].dCf3, sCF[nLR].dPcf3);
		pXLText->AddFormatText(_T("$h$m+04Mcf = Pcf x %.1f = %.3f x %.1f = %.3f %s$n"), sCF[nLR].dHeight, sCF[nLR].dPcf3, sCF[nLR].dHeight, sCF[nLR].dMcf3, pData->m_szTonfM);
	}

	pXLText->AddFormatText(_T("$h$m+04 ㆍ원심하중에서 가장 큰 값인 Mcf = %.f N·m를 적용한다.$n$n"), pUnitLoad->GetMoment(FALSE));

	return TRUE;
}

void CCalcCantilever::WriteCombineTotal( CXLFormatText *pXLText, long &nIdxMiddle, HGBOOL bLeft )
{
	pXLText->OutXL();
	pXLText->GoPosition(NO_MOVE, 0);

	//에타집계표
	pXLText->AddFormatText(_T("$h$s+12"));
	pXLText->GoPosition(NO_MOVE,COL_POS_2);
	pXLText->AddFormatText(_T("$h(%d) η집계표$n"), nIdxMiddle++);
	m_pStd->m_pLoad->MakeTableOutValueEta_Lsd(pXLText, ePartUpperSlab, -1, FALSE);

	//하중계수
	pXLText->AddFormatText(_T("$h$s+17"));
	pXLText->GoPosition(NO_MOVE,COL_POS_2);
	pXLText->AddFormatText(_T("$h(%d) 하중계수$n"), nIdxMiddle++);

	////하중조합 엔진(캔틸레버)
	CLsdManager *pLsdManager = m_pStd->m_pBri->m_plsdMngForCantilever;
	if(pLsdManager)
	{
		long nTypeWrite			= LSD_COMB_DETAIL; //변수 생기면 연결시킬것
		ELSDLoadCaseDir eDir = eLSD_LOADCASE_DIR_PER;
		
		for(long ixComboType=0; ixComboType<eLSD_COMBINATION_FATIGUE; ++ixComboType)
		{
			long nIdxTitle_Trash = 0;
			pLsdManager->WriteTableLsdCombine_Detail(pXLText, eDir, ixComboType, nIdxTitle_Trash, nIdxTitle_Trash, COL_POS_2, _T(""), TRUE, static_cast<long>(eLSD_COMBINE_TOTAL_COUNT), 2);
		}
	}
	*pXLText += 1;

	//하중집계
	WriteTotalTable(pXLText, nIdxMiddle, bLeft);
	//하중조합
	WriteComboTable(pXLText, nIdxMiddle, bLeft);

	pXLText->GoPosition(NO_MOVE, 0);
}

void CCalcCantilever::WriteTotalTable( CXLFormatText *pXLText, long &nIdxMiddle, HGBOOL bLeft )
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;

	CCantileverCheckPosition *pCheckPos = GetCheckPosition(bLeft);
	if(pCheckPos == NULL) return;

	CString sTitle = _T(""), sQes = _T(""), sMu = _T(""), sDes = _T("");
	long nSizeLoad = 0;

	vector<stCantileverLoadUnit*> vLoad = pCheckPos->GetLoads();
	if(vLoad.size() == 0) return;


	nSizeLoad = vLoad.size();

	BOOL bSumLoad = pCheckPos->IsSameLoadCase(); //같은 하중 케이스가 있다면 합계를 따로 구해줘야한다.

	//Table 생성
	long nTotRow = nSizeLoad;
	long nTotCol = bSumLoad ? 20 : 16;

	pXLText->AddFormatText(_T("$h$s+17"));
	pXLText->GoPosition(NO_MOVE,COL_POS_2);
	pXLText->AddFormatText(_T("$h(%d) 하중집계$n"), nIdxMiddle++);
	
	pXLText->AddTable(nTotRow, nTotCol, 1, 1);

	stCantileverLoadUnit *pLoadNext = NULL;
	BOOL bFirst = TRUE;
	//title
	long nTitleLoadValCol = bSumLoad ? 8 : 4; //하중값 컬럼개수
	pXLText->AddFormatText(_T("$h$me8$c%s$m+09$me%d$c%s$m+%02d$me3$c%s$n$h"), hggettext("구분"), nTitleLoadValCol-1, hggettext("하중값 (N.mm)"), nTitleLoadValCol, hggettext("비고"));

	for(long ix = 0; ix < nSizeLoad; ix++)
	{
		stCantileverLoadUnit *pLoad = vLoad[ix];
		if(!pLoad) continue;

		CString sQesNext = _T("");
		if(ix < nSizeLoad -1)
		{
			pLoadNext = vLoad[ix+1];
			sQesNext = pBri->GetLoadCaseName(pLoadNext->GetLoadCaseKind());
		}

		sQes   = pBri->GetLoadCaseName(pLoad->GetLoadCaseKind());
		sTitle = pLoad->sName;
		sMu    = GetStrDouble( pLoad->GetMoment(FALSE), 0, TRUE, TRUE) ;
		sDes   = pLoad->m_sDescription;		

		if(bSumLoad)
			pXLText->AddFormatText(_T("$h$me2$c%s$m+03$me5$c%s$m+06$me3$i$f[#,##0_ ]%s$m+04$me3$c$m+04$me3$c%s"), sQes, sTitle, sMu, sDes);
		else
			pXLText->AddFormatText(_T("$h$me2$c%s$m+03$me5$c%s$m+06$me3$i$f[#,##0_ ]%s$m+04$me3$c%s"), sQes, sTitle, sMu, sDes);


		//같은 하중타입이 있을경우 합계를 구하기 위함
		if(bSumLoad)
		{
			if((sQes == sQesNext) && (ix < nSizeLoad -1)) //다음 하중과 LoadCase가 같다면 열을 머지한다. 합계도 구해놓는다.
			{
				if(bFirst)
				{
					double dSumCase = pCheckPos->GetSumMomentLoadCase(pLoad->GetLoadCaseKind());
					pXLText->AddFormatText(_T("$h$m+13$md1$i$f[#,##0_ ]%s"), CommaC(dSumCase));
				}
				else
				{
					pXLText->AddFormatText(_T("$h$m+13$md1$i$f[#,##0_ ]"));
				}
				bFirst = FALSE;
			}
			else
			{
				pXLText->AddFormatText(_T("$h$m+13$i$f[#,##0_ ]%s"), sMu);	
				bFirst = TRUE;
			}
		}

		*pXLText += 1;

	}

	pXLText->AddFormatText(_T("$h$n"));
}

void CCalcCantilever::WriteComboTable( CXLFormatText *pXLText, long &nIdxMiddle, HGBOOL bLeft )
{
	CCantileverCheckPosition *pCheckPos = GetCheckPosition(bLeft);
	if(pCheckPos == NULL) return;

	//////변수
	long nCntCol = 23;
	CString strCombo = _T(""), sMo = _T(""), str = _T("");

	//선택설계 사항 + 총 열 숫자
	long nSizeCombo = pCheckPos->GetSizeCombo();

	///Print
	pXLText->AddFormatText(_T("$h$s+17"));
	pXLText->GoPosition(NO_MOVE,COL_POS_2);
	pXLText->AddFormatText(_T("$h(%d) 하중조합 $m+23$i단위 m당 $n$h"), nIdxMiddle++);

	pXLText->AddTable(nSizeCombo, nCntCol, 1, 1);

	//Title (merge가 10개 이상일 경우 분리해서 머지 시켜야함)
	pXLText->AddMergeFormatText(0, 1, _T("$c명칭"));
	pXLText->AddFormatText(_T("$m+02"));

	pXLText->AddMergeFormatText(0, 17, _T("$c적 용 식"));
	pXLText->AddFormatText(_T("$m+18"));

	pXLText->AddMergeFormatText(0, 3, _T("$c합계 (N.mm)"));
	pXLText->AddFormatText(_T("$n$h"));

	for(long nStatus = 0; nStatus < eLSD_COMBINATION_COUNT; nStatus++)
	{
		nSizeCombo = pCheckPos->GetSizeCombo((ELSDLoadGroupKind)nStatus);
		for(long nCombo = 0; nCombo < nSizeCombo; nCombo++)
		{
			stCantileverCombo *pCombo = pCheckPos->GetCombo((ELSDLoadGroupKind)nStatus, nCombo);

			sMo = GetStrDouble(pCombo->m_dMoment, 0 , TRUE);
			strCombo.Format(_T("%.3f x (%s)"), pCombo->m_dEta, pCombo->m_strCombo);

			pXLText->AddMergeFormatText(0, 1, _T("$c%s"), pCombo->m_sName);
			pXLText->AddFormatText(_T("$m+02"));

			pXLText->AddMergeFormatText(0, 17, _T("%s$e"), strCombo);
			pXLText->AddFormatText(_T("$m+18"));

			pXLText->AddMergeFormatText(0, 3, _T("$i$f[#,##0_ ]%s"), sMo);
			pXLText->AddFormatText(_T("$n$h"));
		}
	}	
	if(pCheckPos->GetSizeCombo(eLSD_COMBINATION_LIMIT) > 0)
	{
		str.Format(_T("ㆍ 극한한계상태는 하중조합 중에서 가장 큰 Mu = %s N.mm 로 검토한다."), GetStrDouble(pCheckPos->GetMoment(eLSD_COMBINATION_LIMIT), 0, TRUE));
		pXLText->AddFormatText(_T("$h%s $n$h"), str);
	}
	if(pCheckPos->GetSizeCombo(eLSD_COMBINATION_LIMIT) > 0)
	{
		str.Format(_T("ㆍ 극단상황한계상태는 하중조합 중에서 가장 큰 Mu = %s N.mm 로 검토한다."), GetStrDouble(pCheckPos->GetMoment(eLSD_COMBINATION_EXTREME), 0, TRUE));
		pXLText->AddFormatText(_T("$h%s $n$h"), str);
	}
	if(pCheckPos->GetSizeCombo(eLSD_COMBINATION_SERVICE) > 0)
	{
		str.Format(_T("ㆍ 사용한계상태는 하중조합 중에서 가장 큰 Mu = %s N.mm 로 검토한다."), GetStrDouble(pCheckPos->GetMoment(eLSD_COMBINATION_SERVICE), 0, TRUE));
		pXLText->AddFormatText(_T("$h%s $n$h"), str);
	}

	pXLText->AddFormatText(_T("$h$n$h"));
}

void CCalcCantilever::WriteMinThickSlab( CXLFormatText *pXLText, long &nIdxMiddle, HGBOOL bLeft )
{
	CCantileverCheckPosition *pCheckPos = GetCheckPosition(bLeft);

	CString str = _T("");

	pXLText->AddFormatText(_T("$h$s+08"));
	//Title
	pXLText->GoPosition(NO_MOVE,COL_POS_2);
	pXLText->AddFormatText(_T("$h(%d) 바닥판 두께의 적정성 검토$n"), nIdxMiddle++);

	pXLText->GoPosition(NO_MOVE,COL_POS_3);
	CString strOK = pCheckPos->IsOkMinThick(FALSE) ? _T("O.K") : _T("N.G");
	str.Format(_T("① 홈 또는 마모방지층의 두께를 뺀 최소두께는 %.0f mm 보다 커야 한다."), pCheckPos->GetThickSlabRequire(FALSE));
	pXLText->AddFormatText(_T("$h%s $m+18∴ %s $n$h"), str, strOK);

	strOK = pCheckPos->IsOkMinThick(TRUE) ? _T("O.K") : _T("N.G");
	str.Format(_T("② 콘크리트 바닥판은 %.0fmm 보다 작아서는 안된다."), pCheckPos->GetThickSlabRequire(FALSE));
	pXLText->AddFormatText(_T("$h%s $m+18∴ %s $n$n$h"), str, strOK);

	*pXLText += 1;
}

void CCalcCantilever::WriteRebarCheck( CXLFormatText *pXLText, long &nIdxMiddle, HGBOOL bLeft )
{
	CCantileverCheckPosition *pCheckPos = GetCheckPosition(bLeft);

	pXLText->AddFormatText(_T("$h$s+40"));

	pXLText->GoPosition(NO_MOVE,COL_POS_2);
	pXLText->AddFormatText(_T("$h(%d) 철근량 검토$n"), nIdxMiddle++);

	// 단면검토
	long nSizeConc = pCheckPos->GetSizeConcData();
	for(long n = 0; n < nSizeConc; n++)
	{
		CConcBasic *pConc = pCheckPos->GetConcData(n);
		if(pConc == NULL) continue;

		long nStatus = pCheckPos->m_lArrStatus.GetAt(n);
		if(nStatus == eLSD_COMBINATION_SERVICE) continue;

		pXLText->GoPosition(NO_MOVE, COL_POS_3);
		pXLText->AddFormatText(_T("$h$r'- %s$n$h"), pConc->m_sTitle);

		CLsdBeam Beam(pConc, nStatus == 0 ? LSD_ULS_DESIGN : LSD_ESLS_DESIGN);
		Beam.MakeOutputDesignExcelFile(pXLText, COL_POS_3, FALSE, FALSE);
		pXLText->AddText(_T("$n"));
	}
}

void CCalcCantilever::WriteRebarUsability( CXLFormatText *pXLText, long &nIdxMiddle, HGBOOL bLeft )
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CCantileverCheckPosition *pCheckPos = GetCheckPosition(bLeft);

	pXLText->AddFormatText(_T("$h$s+10"));
	pXLText->GoPosition(NO_MOVE,COL_POS_2);
	pXLText->AddFormatText(_T("$h(%d)사용성 검토$n"), nIdxMiddle++);

	long nSizeConc = pCheckPos->GetSizeConcData();

	// 사용성검토
	for(long n = 0; n < nSizeConc; n++)
	{
		CConcBasic *pConc = pCheckPos->GetConcData(n);
		if(pConc == NULL) continue;

		long nStatus = pCheckPos->m_lArrStatus.GetAt(n);
		if(nStatus != eLSD_COMBINATION_SERVICE) continue;

		pXLText->GoPosition(NO_MOVE, COL_POS_2);
		pXLText->AddFormatText(_T("$m+03'- %s$n$h"), pConc->m_sTitle);

		if(pConc->m_strDesignDegree == _T("A"))
		{
			pXLText->AddFormatText(_T("$h$m+03$e설계등급 A는 비균열 상태 및 균열폭 한계상태 검토대상이 아니므로 균열 검토를 진행하지 않습니다.$n$h"));
			continue;
		}

		CString sDesign= _T(""), sKind=_T(""), sExtra = _T("");
		double dCrackWitdh = pBri->GetCrackLimitWidth_LSD(ePartUpperSlab, TRUE, sKind, sDesign);
		pXLText->AddFormatText(_T("$h$m+03※ 노출 환경에 따른 적용 설계 등급 : %s %s$n$h"), sDesign, sExtra);
		// 		if(sDesign == _T("A"))	
		// 			pXLText->AddFormatText(_T("$h$r설계등급 A는 비균열 상태 및 균열폭 한계상태 검토대상이 아니므로$n$h 균열 검토를 진행하지 않습니다.$n$h"));

		if(pBri->IsCheckPosEtaUseLoad(ePartUpperSlab) == FALSE)
		{
			pXLText->AddFormatText(_T("$h$m+03사용하중 조합에서 한계상태 검증을 위한 하중조합을 미검토하였으므로$n$h$m+03사용성 검토는 진행하지 않는다.$n$n$h"));
			continue;
		}

		if(sDesign != _T("A"))
			pXLText->AddFormatText(_T("$h$m+03$e설계등급 %s 이므로 균열폭 한계상태는 %s, 표면 한계균열폭은 %.1fmm를 적용.$n$n$h"), sDesign,sKind,dCrackWitdh );

		CLsdBeam Beam(pConc, LSD_ULS_DESIGN);
		Beam.MakeOutputUsabilityExcelFile(pXLText, COL_POS_3, FALSE);
		pXLText->AddText(_T("$n"));
	}
}

void CCalcCantilever::WriteSupportRebar( CXLFormatText *pXLText, long &nIdxMiddle, HGBOOL bLeft )
{
	CCantileverCheckPosition *pCheckPos = GetCheckPosition(bLeft);
	if(pCheckPos->GetSizeConcData() == 0) return;

	CString str = _T("");

	long nDan = 0;
	long nIndex = 0;
	CConcBasic *pConc = pCheckPos->GetConcData(nIndex);

	HGTCHAR cHD = GetCharRebarMark(pConc->m_Fy);
	double dFy = GetValueUnitChange(pConc->m_Fy, UNIT_CGS_KGF_CM2, _T("N/mm²"));
	double dAsUse = pCheckPos->GetAsUseSupport();
	double dAsReq = pCheckPos->GetAsReqSupport();
	double dCtcUse = pCheckPos->GetCtcUseSupport();
	double dCtcMax = pCheckPos->GetCtcMaxSupport();

	CString strOK1 = pCheckPos->IsOkSupportAs() ? _T("O.K") : _T("N.G");
	CString strOK2 = pCheckPos->IsOkSupportAs() ? _T("≥") : _T("＜");

	///Print
	pXLText->AddFormatText(_T("$h$s+10"));

	//Title
	pXLText->GoPosition(NO_MOVE,COL_POS_2);
	pXLText->AddFormatText(_T("$h(%d)배력철근 검토$n"), nIdxMiddle++);

	pXLText->GoPosition(NO_MOVE,COL_POS_3);
	CString strTitle = _T("철근 단면적 검토");
	pXLText->AddFormatText(_T("$h'- %s$n"), strTitle);

	str.Format(_T("As ≥ %.2f Ag / fy"), SUPPORT_REBAR_AREA_FACTOR);
	pXLText->AddFormatText(_T("$h$r%s$n"), str);

	pXLText->AddFormatText(_T("$h$r사용철근량$m+03$c=$r$e%.1f mm²$n"), dAsUse);
	pXLText->AddFormatText(_T("$h$r$m+04%c%.0f - CTC %.0f$n"), cHD, pConc->m_TSupportBarDia[nDan], pConc->m_TSupportBarCTC[nDan]);
	pXLText->AddFormatText(_T("$h$r→ As = %.1f mm² %s %.2f x %.1f x %.1f / %.1f = %.1f mm²  ∴ %s$n"), dAsUse, strOK2, SUPPORT_REBAR_AREA_FACTOR, pConc->m_Ho, pConc->m_Bo, dFy, dAsReq, strOK1);


	strTitle = _T("배력철근 간격 검토");
	pXLText->AddFormatText(_T("$n$h'- %s$n"), strTitle);
	strOK1 = pCheckPos->IsOkSupportCtc() ? _T("O.K") : _T("N.G");
	strOK2 = pCheckPos->IsOkSupportCtc() ? _T("≤") : _T("＞");
	str.Format(_T("S ≤ min(%.0fh, %.1f)"), SUPPORT_REBAR_CTC_FACTOR, SUPPORT_REBAR_CTC_MAX);
	pXLText->AddFormatText(_T("$h$r%s$n"), str);

	pXLText->AddFormatText(_T("$h$r→ S = %.1f mm %s min(%.0f x %.1f, %.1f) = %.1f mm  ∴ %s$n"), dCtcUse, strOK2, SUPPORT_REBAR_CTC_FACTOR, pConc->m_Ho, SUPPORT_REBAR_CTC_MAX, dCtcMax, strOK1);

	*pXLText += 1;
}

void CCalcCantilever::WriteDeflectionCheck( CXLFormatText *pXLText, HGBOOL bLeft )
{
	CCantileverCheckPosition *pCheckPos = GetCheckPosition(bLeft);

	CString str = _T("");

	BOOL bNeedCheck = pCheckPos->IsNeedDeflectionCheck();
	CString strOK1 = bNeedCheck ? _T("N.G") : _T("O.K");
	CString strOK2 = bNeedCheck ? _T("＞") : _T("≤");

	double dSpanLen = pCheckPos->GettMinSpanLength();
	double dMinThick = pCheckPos->GettMinDeflection();
	double dSlabThick = pCheckPos->GetThickSlab();
	double dDiv = pCheckPos->GettMinDivide();

	///Print
	pXLText->GoPosition(NO_MOVE,COL_POS_2);
	pXLText->AddFormatText(_T("$h'-처짐 계산을 하지 않는 경우의 휨부재의 최소 두께$n"));

	pXLText->AddFormatText(_T("$h$r t min$m+02$c=$r$eL / %.0f = %.1f / %.0f$m+05$c=$r%.1f mm$n"), dDiv, dSpanLen, dDiv, dMinThick);

	pXLText->AddFormatText(_T("$h$r t min$m+02$c=$r$e%.1f mm %s h = %.1f mm  ∴ %s$n"), dMinThick, strOK2, dSlabThick, strOK1);

	if(bNeedCheck)
	{
		pXLText->AddFormatText(_T("$h$r∴ 별도의 처짐검토는 필요.$n"));
	}
	else
	{
		pXLText->AddFormatText(_T("$h$r∴ 별도의 처짐검토는 필요 없음.$n"));
	}

	*pXLText += 1;
}

void CCalcCantilever::SetConcBasicCheckPosition( CCantileverCheckPosition *pCheckPosition, HGBOOL bLeft )
{
	pCheckPosition->ClearConcData();
	for(long iStatue = eLSD_COMBINATION_LIMIT; iStatue < eLSD_COMBINATION_COUNT; iStatue++)
	{
		if(pCheckPosition->GetSizeCombo((ELSDLoadGroupKind)iStatue) == 0) continue;

		ELSDLoadGroupKind eGroup = (ELSDLoadGroupKind)iStatue;
		CConcBasic *pConc = GetConcBasicData(bLeft, eGroup);
		if(pConc == NULL) continue;

		pConc->m_sTitle = pCheckPosition->GetStringRebarCheck(eGroup);
		double dMoment = GetValueUnitChange(pCheckPosition->GetMoment(eGroup), _T("N.mm"), UNIT_CGS_TONFM);
		if(eGroup == eLSD_COMBINATION_SERVICE)
		{
			pConc->m_MomentUse = dMoment;
			pConc->m_MomentCrack	= pConc->m_MomentUse;
		}
		else
			pConc->m_Moment = dMoment;

		pCheckPosition->AddConcData(pConc);
		pCheckPosition->m_lArrStatus.Add(iStatue);
	}
}

CConcBasic * CCalcCantilever::GetConcBasicData( HGBOOL bLeft, ELSDLoadGroupKind eGroupKind )
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;

	CConcBasic *pConc = new CConcBasic;

	pBri->SetConcDataBasic(pConc, ePartUpperSlab, TRUE, eGroupKind);

	pConc->m_sTitle	= bLeft? _T("좌측 캔틸레버부") : _T("우측 캔틸레버부");
	pConc->m_Bo		= frM(UNIT_METER);
	pConc->m_Ho		= pBri->m_dCantilH1[bLeft? iLEFT : iRIGHT]; // 캔틸레버두께

	pConc->m_nTypeSection = SECTION_SINGLE;
	pConc->m_dDeflectionCoefficient = 10.0;

	// 사용철근 집계
	for(long ix=0; ix<MAX_REBAR_DAN; ix++)
	{
		CDPoint xyInfo = pBri->GetInfoSupportRebar_UpperSlab(TRUE, 0, ix, 0);
		pConc->m_RbT1C_DIA[ix] = xyInfo.x; // 철근의 직경
		if(xyInfo.x>0)
		{
			pConc->m_RbT1C_dc[ix] = pBri->m_dCoverUpper_UpperSlab[ix];  // 철근의 중심거리 피복 + 100*단수(-1)
			pConc->m_RbT1C_B [ix] = pConc->m_Bo/xyInfo.y;  // 철근의 개수 (1000/CTC)
		}

		xyInfo = pBri->GetInfoSupportRebar_UpperSlab(FALSE, 0, ix, 0);
		pConc->m_RbC1C_DIA[ix] = xyInfo.x; // 철근의 직경
		if(xyInfo.x>0)
		{
			pConc->m_RbC1C_dc[ix] = pBri->m_dCoverLower_UpperSlab[ix];  // 철근의 중심거리 피복 + 100*단수(-1)
			pConc->m_RbC1C_B [ix] = pConc->m_Bo/xyInfo.y;  // 철근의 개수 (1000/CTC)
		}
	}

	for(long ix=0; ix<MAX_REBAR_DAN; ix++)
	{
		CDPoint xyInfo = pBri->GetInfoMainRebar_UpperSlab(TRUE, 0, 0, ix, 0);
		CDPoint xyInfo2 = pBri->GetInfoMainRebar_UpperSlab(TRUE, 0, 1, ix, 0);

		pConc->m_TSupportBarDia[ix] = xyInfo.x;
		pConc->m_TSupportBarCTC[ix] = (xyInfo2.x > 0)? (xyInfo.y / 2) : xyInfo.y;

		xyInfo = pBri->GetInfoMainRebar_UpperSlab(FALSE, 0, 0, ix, 0);
		xyInfo2 = pBri->GetInfoMainRebar_UpperSlab(FALSE, 0, 1, ix, 0);

		pConc->m_CSupportBarDia[ix] = xyInfo.x;
		pConc->m_CSupportBarCTC[ix] = (xyInfo2.x > 0)? (xyInfo.y / 2) : xyInfo.y;
	}

	return pConc;
}
