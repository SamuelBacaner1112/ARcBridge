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

	// ARoad�� �޸� �¿� ĵƿ���� ���丸 �ϹǷ� vector�� ������� �ʴ´�.
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

		// �������� �׷캰 �ִ���Ʈ ����
		pCheckPos->SetMomentByGroup(pBri->m_plsdMngForCantilever);

		SetConcBasicCheckPosition(pCheckPos, ix==0);

		// ������ �β�
		double dThick = pBri->m_dCantilH1[ix];
		pCheckPos->CalculateSlabThick(dThick);

		// ���ö�� ���� : ö������ ������ ���
		pCheckPos->CalculateSupportRebar();

		// ó������
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

 		pCheckPos->SetName(iLR == iLEFT ? _T("���� ��ƿ����") : _T("���� ��ƿ����"));
 
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

// ��ȣ��(�ߺд�),������ ��ũ��Ʈ ���� ����...
void CCalcCantilever::MakeAreaConc(BOOL bLeft)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	if( pBri == NULL ) return;

	long nLR = bLeft ? iLEFT : iRIGHT;
	CTwinVectorArray tvArr[2];
	// ��ȣ��
	GetTvarrayGuardWall(tvArr[0], bLeft);
	// ������
	GetTvarrayCantilever(tvArr[1], bLeft);

	long nQtyHDan = pBri->GetQtyHDan();
	CGuardWallRC *pGW = pBri->GetGuardWallByHDan( bLeft==TRUE ? 0 : nQtyHDan-1 );
	HGBOOL bGuardWallDW = pGW->m_bDWload;

 	// AREA ����....
 	CRebarPlacing rb;
 	for(long nS = 0; nS < 2; nS++) // ��ȣ��,������
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

// ��ȣ��
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

	// �����ΰ�� ���� ������ 0.0���� �̵���Ų��.
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
// 	if( str == "����" )		// ����
// 	{
// 		nSelect = bLeft==TRUE ? nSelect+1 : nSelect-1;
// 		pGW = pBri->GetGuardWallByHDan( nSelect );
// 		if(!pGW) return dArea;
// 	}
// 	else if( str == "����" ) return dArea;
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

// ĵƿ����
void CCalcCantilever::GetTvarrayCantilever(CTwinVectorArray &tvArr, BOOL bLeft)
{
	tvArr.RemoveAll();

	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	if( pBri == NULL ) return;

	long nPos = bLeft==TRUE ? 0 : 1;

//	HGDOUBLE dR = pBri->m_dRE2[nPos];
	HGDOUBLE dWidth = pBri->m_dCantilB[nPos];
	HGDOUBLE dHeight = pBri->m_dTS - pBri->m_dCantilH1[nPos] - pBri->m_dCantilH2[nPos];		// ���� ����
	HGDOUBLE dH1 = pBri->m_dCantilH2[nPos];		// ���ܸ� ����
	HGINT32 nDir = bLeft? 1 : -1;

	CTwinVector tv;
	CDPoint xyStt(0,0);

	tv.SetFromPoint(xyStt, CDPoint(dWidth * nDir, 0));		// ���� ����� �������� �Ѵ�.
	tvArr.Add(tv);

	tv.SetFromPoint(tv.m_v2, CDPoint(tv.m_v2.x, tv.m_v2.y - (dHeight + dH1)));
	tvArr.Add(tv);

	tv.SetFromPoint(tv.m_v2, CDPoint(tv.m_v2.x - dWidth * nDir, tv.m_v2.y + dH1));
	tvArr.Add(tv);

	tv.SetFromPoint(tv.m_v2, xyStt);
	tvArr.Add(tv);
}


// ��ƿ���� �������� ����
void CCalcCantilever::MakeListLoadCantilever(CCantileverCheckPosition *pCheckPosition, HGBOOL bLeft)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	if( pBri == NULL ) return;

	long nQtyHDan = pBri->GetQtyHDan();
	CGuardWallRC *pGW = pBri->GetGuardWallByHDan( bLeft==TRUE ? 0 : nQtyHDan-1 );
	HGBOOL bGuardWallDW = pGW->m_bDWload;

	// ��ȣ��,������
	stCantileverLoadUnit *pLoad = GetUnitLoadConcreteDC(bLeft);
	pCheckPosition->AddLoad(pLoad);

	if(bGuardWallDW)
	{
		pLoad = GetUnitLoadConcreteDW(bLeft);
		pCheckPosition->AddLoad(pLoad);
	}

	// ��������
	pLoad = GetUnitLoadPavement(bLeft);
	pCheckPosition->AddLoad(pLoad);

	// ��������
	pLoad = GetUnitLoadFootWay(bLeft);
	pCheckPosition->AddLoad(pLoad);

	// ������,����
	pLoad = GetUnitLoadUpperStructure(bLeft, BANGUM);
	pCheckPosition->AddLoad(pLoad);
	//
	pLoad = GetUnitLoadUpperStructure(bLeft, NANGAN);
	pCheckPosition->AddLoad(pLoad);
 
	// ��������
	pLoad = GetUnitLoadCrowd(bLeft);
	pCheckPosition->AddLoad(pLoad);

 	//////////////////////////////////////////////////////////////////////////
	// ��������
	pLoad = GetUnitLoadVehicle(bLeft);
	pCheckPosition->AddLoad(pLoad);

	// �浹����
	pLoad = GetUnitLoadCollision(bLeft);
	pCheckPosition->AddLoad(pLoad);

	// ǳ����
	pLoad = GetUnitLoadWind(bLeft);
	pCheckPosition->AddLoad(pLoad);

	// ��������
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

		//DW������ ������ ���� ������ش�.
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

	CDPoint xyFootway = GetXyHDanByType(bLeft); // x = ������ġ, y = ����ġ (�����곡�ܱ���)
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

	CDPoint xyHDan(0,0); // ĵƿ���� ��������
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

// ����(0), ������(1)
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

	HGDOUBLE dWeightPave(0.);		// ���� �����߷�
	HGDOUBLE dBaseX(0.);
	HGDOUBLE dCantileverW = pBri->m_dCantilB[bLeft==TRUE ? 0 : 1];

	CTwinVectorArray tvArr;
	long nQtyHDan = pBri->GetQtyHDan();

	// ������ ������ ��������
	for ( long nHDan = 0; nHDan<nQtyHDan; ++nHDan )
	{
		CGuardWallRC *pGW = pBri->GetGuardWallByHDan( nHDan );
		if( pGW == NULL ) continue;

		pGW->GetTvGuardWallBase(tvArr, FALSE);
		dBaseX += (tvArr.GetXyRight().x - tvArr.GetXyLast().x);

		CString str = pBri->GetStringTypeHDan(nHDan, FALSE, FALSE);
		if( str == "����") return NULL;

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

//	long nHDanType = 0; // ����(����)
	
	// ������ �ƴϸ� ������ ������ �ν��ϰ� �Ѵ�. - ����ζ� ������.
	CDPoint xyPave = GetXyHDanByType(bLeft); // x = ������ġ, y = ����ġ (�����곡�ܱ���)
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
	// �ӽ�
	sVC[nLR].dPr = 100.;
	sVC[nLR].dMl = 200.;

// 	sVC[nLR].dPr = GetValueUnitChange(pGradeCtl->GetLSDWeight(nLoadType, pGradeCtl->GetLSDLoadSize(nLoadType) - 1, nGrade) / 2., UNIT_CGS_KGF, "N");
// 	sVC[nLR].dMl = (sVC[nLR].dPr / dE * _1000MM) * dX * (1 + _dImpactLoad[1]);

	pLoad->m_xyLoadPosition = CDPoint(dX, 0);
	pLoad->m_dForceVert = sVC[nLR].dMl / dX;

	long nQtyHDan = pBri->GetQtyHDan();
	CGuardWallRC* pGw = pBri->GetGuardWallByHDan(bLeft==TRUE ? 0 : nQtyHDan-1);		// ������ �������� �Ѵ�.
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


		BOOL bNoDistOnNoGuard = FALSE;		// [Ⱦ���� ���ߺй�]-[Ȱ����]-[�������� ��� �ܺ�����] �ɼ�
		

		if( pCheckData->m_bFloorTank )
		{
//			sVC[nLR].DisFromWallTank = (dTLen==0 && pData->m_bNoDistOnNoGuard) ? 0 : frM(pData->m_dTankEdit2/2);
			// �ӽ� ������
			double dTankEdit2 = 500;				// ��ũ ��������

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
			double dTrailEdit5 = 700; // Ʈ���Ϸ� �������� - Ⱦ���� 	����

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
	CGuardWallRC* pGw = pBri->GetGuardWallByHDan(bLeft==TRUE ? 0 : nQtyHDan-1);		// ������ �������� �Ѵ�.
	if( pGw == NULL ) return NULL;

	double dVelocity= pCheckData->m_dCo_V;
	double dLendSupport = GetLengthSupport(bLeft);
	if(Compare(dLendSupport, 0.0, _T("<="))) return NULL;
	double dR = pCheckData->m_dCenter_R[bLeft ? iLEFT : iRIGHT];
	double dHeight = COLLISION_H1 + pBri->GetThickPave() + pBri->m_dTS / 2; // ���� �浹 ����

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
	if(bExistFootway && dLendSupport > 0) // ������ �ְ� ������ �ִ� ���
	{		
		double LEN = pBri->GetLengthHDan(bLeft==TRUE ? 0 : nQtyHDan-1);
		double dDist = (dTerm - LEN) + (bLeft ? (pGw->m_dW5 + pGw->m_dW4) : (pGw->m_dW1 + pGw->m_dW2))  + pGw->m_dW3 / 2;
 //		double dMoment = sCO[nLR].dV * dDist + dHeight * sCO[nLR].dH;

		pLoad->sName = STR_LOAD_COLLISION_VEHICLE;
		pLoad->m_xyLoadPosition = CDPoint(dDist, dHeight);
		pLoad->m_dForceVert = sCO[nLR].dV;
		pLoad->m_dForceHori = sCO[nLR].dH;
	}
	else if(!bExistFootway) // ������ ���� ��ȣ�� Ȥ�� �ߺд�
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
			if(dR > 0 && dR <= COLLISION_Rstd ) // �ݰ��� 200M���� ���� ��� 2��
			{
				pLoad->m_dForceHori *= 2;
			}
		}
	}
	else  if(bExistFootway && dLendSupport <= 0) // ������ �ִ� ���
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
	CGuardWallRC* pGw = pBri->GetGuardWallByHDan(bLeft==TRUE ? 0 : nQtyHDan-1);		// ������ �������� �Ѵ�.
	if( pGw == NULL ) return NULL;

	BOOL bBodo = (pWorkingLoad->GetBodoWeight().m_bExist && pWorkingLoad->GetBodoWeight().m_dHeight > 0);
	double dPaveT = bBodo ? frM(pWorkingLoad->GetBodoWeight().m_dHeight) : pBri->GetThickPave();	// ����β� 
	double dSlabT =	pBri->m_dTS;	// ������ ���� �β�	
	double dWindStrength = GetValueUnitChange(pCheckData->m_dWindStrength, UNIT_CGS_TONF_M2, pData->m_szTonf_M2);	// ǳ���� ����

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
	CGuardWallRC* pGw = pBri->GetGuardWallByHDan(bLeft==TRUE ? 0 : nQtyHDan-1);		// ������ �������� �Ѵ�.
	if( pGw == NULL ) return NULL;

	long nType = pGw->m_nTypeGuardWall;
	long nLR = bLeft==TRUE ? iLEFT : iRIGHT;

	double dR		= pCheckData->m_dCenter_R[bLeft ? iLEFT : iRIGHT];					// ����ݰ� 
 	double dV		= pCheckData->m_dCo_V;						// ����ӵ� 
 	double dVT		= CENTRIFUGAL_TANK_V;					// ��ũ or Ʈ���Ϸ� ����ӵ�
	double dD1		= pGw ? pGw->m_dDistFrLeft : 0;			// ��ȣ�� �ٱ��� ���� - Ⱦ�� ������ ���� - D1
	double dWidth	= nType != 2 ? 0 : pGw->GetWidth();	// ��ȣ�� �� 
	double dTerm = pBri->m_dCantilB[bLeft==TRUE ? 0 : 1];

	sCF[nLR].dL1 = GetLengthSupport(bLeft);// ������� 
	sCF[nLR].dL3 = 0;
	sCF[nLR].dE1 = VEHICLE_FACTOR_E1 * sCF[nLR].dL1 + VEHICLE_FACTOR_E2; // ������ ������ 
	sCF[nLR].dE3 = 0;	

	double dSlabT = pBri->m_dTS; // ������ �β�
	double dPaveT = pBri->GetThickPave();				// ����β�
	sCF[nLR].dHeight = CENTRIFUGAL_HEIGHT_B + dSlabT / 2. + dPaveT;		// �ۿ����

	if(pWorkingLoad->GetBodoWeight().m_bExist) return NULL;
	if(Compare(sCF[nLR].dL1, 0.0, _T("<=")) || Compare(dR, 0.0, _T("="))) return NULL;

 	HGDOUBLE dLSDWeight = GetValueUnitChange(m_pStd->m_pLoad->m_dPrApply[0],UNIT_CGS_TONF,pData->m_szTonf);
	HGDOUBLE dTankEdit1 = m_pStd->m_pLoad->m_dTank_TankLength;
	
	sCF[nLR].dPr1  = dLSDWeight;
	sCF[nLR].dCf1  = CalcCentriodLoad_CF(dV, dR);
	sCF[nLR].dPcf1 = sCF[nLR].dPr1 / sCF[nLR].dE1 * sCF[nLR].dCf1;
	sCF[nLR].dMcf1 = sCF[nLR].dPcf1 * sCF[nLR].dHeight;	

	// ��ũ
	if(pCheckData->m_bFloorTank)
	{
		sCF[nLR].dPr2  = pData->m_dTank_WeightTank / 2 / dTankEdit1;
		sCF[nLR].dCf2  = CalcCentriodLoad_CF(dVT, dR);
		sCF[nLR].dPcf2 = sCF[nLR].dPr2 * sCF[nLR].dCf2;
		sCF[nLR].dMcf2 = sCF[nLR].dPcf2 * sCF[nLR].dHeight;	
	}

	// Ʈ���Ϸ�
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
	pLoad->sName = _T("��������");
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
	
	CString szSubTitle = (bLeft==TRUE) ? _T("���� ĵƿ������") : _T("���� ĵƿ������");
	pXLText->AddFormatText(_T("$h$tb10%d.%d %s$n$n"), nIdxTitle, nIdxSubTitle, szSubTitle);
 
	CDomyun *pDom = new CDomyun;
	DrawGuardWall(pDom, pBri, pGW, bLeft );
	long nSizeRow = (long)(pDom->GetExtHeight()/150);

	pXLText->GoPosition(NO_MOVE, COL_POS_3);
	pXLText->InsertDomyun(pDom,m_pStd->m_pView,nSizeRow,CXLFormatText::INSERT_DOM_CURPOS | CXLFormatText::INSERT_DOM_MOVEROW);

	long nIdxBig = 1;
	long nIdxMiddle = 1;

	pXLText->GoPosition(NO_MOVE, 0);
	pXLText->AddFormatText(_T("$n$h$r%d) ���߻���$n"), nIdxBig++);

 	if(pCheckPos->IsLoad(eLSD_LOADCASE_DC1))//��ȣ��,������
		WriteLoadConcrete(pXLText, nIdxMiddle, bLeft);

	if(pCheckPos->IsLoad(eLSD_LOADCASE_DW1))//��������
		WriteLoadPavement(pXLText, nIdxMiddle, bLeft);

	if(pCheckPos->IsLoad(eLSD_LOADCASE_DW1) && pWorkingLoad->GetBodoWeight().m_bExist)//��������
		WriteLoadFootWay(pXLText, nIdxMiddle, bLeft);

	if(pCheckPos->IsLoad(eLSD_LOADCASE_PL) && pWorkingLoad->GetWalkLoad())//��������
		WriteLoadCrowd(pXLText, nIdxMiddle, bLeft);


	if(pCheckPos->IsLoad(eLSD_LOADCASE_DW1))//������, ����
	{
		if(pWorkingLoad->GetUserLoad(BANGUM).m_bExist)
			WriteLoadUpperStructure(pXLText, nIdxMiddle, BANGUM, bLeft);
		if(pWorkingLoad->GetUserLoad(NANGAN).m_bExist)
			WriteLoadUpperStructure(pXLText, nIdxMiddle, NANGAN, bLeft);
	}

	if(pCheckPos->IsLoad(eLSD_LOADCASE_LL1))//��������
		WriteLoadVehicle(pXLText, nIdxMiddle, bLeft);

	if(pCheckPos->IsLoad(eLSD_LOADCASE_CT11))//�浹����
	{
		WriteLoadCollision(pXLText, nIdxMiddle, bLeft);
	}

	if(pCheckPos->IsLoad(eLSD_LOADCASE_WS11))//ǳ����
		WriteLoadWind(pXLText, nIdxMiddle, bLeft);

	if(pCheckPos->IsLoad(eLSD_LOADCASE_CF))//��������
		WriteLoadCentrifugal(pXLText, nIdxMiddle, bLeft);

	//2)��������
	nIdxMiddle = 1;
	pXLText->GoPosition(NO_MOVE, 0);
	pXLText->AddFormatText(_T("$n$h$r%d) ��������$n"), nIdxBig++);
	
	WriteCombineTotal(pXLText, nIdxMiddle, bLeft); //��������

	//3)�ٴ��� �ܸ����
	nIdxMiddle = 1;
	pXLText->GoPosition(NO_MOVE, 0);
	pXLText->AddFormatText(_T("$n$h$r%d) Cantilever �ܸ����$n"), nIdxBig++);
	
	WriteMinThickSlab(pXLText, nIdxMiddle, bLeft);//�ٴ��� �β��� ������ ����

	WriteRebarCheck(pXLText, nIdxMiddle, bLeft);// �ܸ����(ö�ٷ�����)
	WriteRebarUsability(pXLText, nIdxMiddle, bLeft);

	WriteSupportRebar(pXLText, nIdxMiddle, bLeft);// ���ö�� ����
	
	//4)ó������
	pXLText->GoPosition(NO_MOVE, 0);
	pXLText->AddFormatText(_T("$n$h$r%d) Cantilever ó������$n"), nIdxBig++);
	
	WriteDeflectionCheck(pXLText, bLeft);//ó������
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
	HGDOUBLE dCantilH = pBri->m_dTS - pBri->m_dCantilH1[nPos] - pBri->m_dCantilH2[nPos];		// ���� ����
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

	// �������� ���� /

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

	// ��ȣ �ٿ��� ��� modified by jaeho 2004.04.09
	CHgBaseDrawStd baseDraw;

	// 2 ��° ��ȣ�� Ÿ��
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

	// ĵƿ����
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
		pXLText->AddFormatText(_T("$s+%02d$h$m+02(%d) %s (%s) $m+22$i(���� m��)$n")
			, icArea+1, nIdxMiddle++, strLoadName, pBri->GetLoadCaseName(ixLoadType == eSlabDC ? eLSD_LOADCASE_DC1 : eLSD_LOADCASE_DW1));

		pXLText->GoPosition(NO_MOVE, COL_POS_2);
		pXLText->AddTable(icArea+1, 22, 1, 2);

		pXLText->AddFormatText(_T("$h$me1$cNo.$m+02$me9$c��   �� (ü�� x �����߷�)$m+10$me2$c�ۿ�����(N)$m+03$me3$c�ۿ�Ÿ�(mm)$m+04$me3$c���Ʈ(N��mm)$n"));

		for(long ix = 0; ix < icArea; ++ix)
		{
			CTwinVectorAreaShape *pArea = m_vArea[ixLoadType][nLR][ix];
			if(pArea == NULL) continue;

			long icArr = pArea->m_tvArr.GetSize();
			double dL = pArea->GetWidth();
			double dH = pArea->GetHeight();
			double dWeight = 0; //�ۿ�����

			pXLText->AddFormatText(_T("$h$me1$c%d"), ix+1); //No.

			//���
			if(icArr == 3) //�ﰢ��
			{
				pXLText->AddFormatText(_T("$m+02$me9$e %4.f x %3.f x 1000 x %f x 1/2"), dL, dH, m_dUnitWeightSteelConc);
				dWeight = (dL * dH * dB * m_dUnitWeightSteelConc)/2;
			}
			else if(icArr == 4) //�簢��
			{
				pXLText->AddFormatText(_T("$m+02$me9$e %4.f x %3.f x 1000 x %f"), dL, dH, m_dUnitWeightSteelConc);
				dWeight = dL * dH * dB * m_dUnitWeightSteelConc;
			}

			dDistArea = fabs(pArea->m_xyCen.x - dDistCantilever * (bLeft? 1 : -1));

			//�ۿ�����
			pXLText->AddFormatText(_T("$m+10$me2$f[#,##0_ ]$i%s "), CommaC(dWeight));

			//�ۿ�Ÿ�
			pXLText->AddFormatText(_T("$m+03$me3$i$f[#,##0_ ]%s "), CommaC(dDistArea));

			//���Ʈ
			pXLText->AddFormatText(_T("$m+04$me3$i$f[#,##0_ ]%s $n"), CommaC(dWeight * dDistArea));
		}

		double dForceVert = pUnitLoad->m_dForceVert;
		double dMoment = pUnitLoad->GetMoment(FALSE);

		pXLText->AddFormatText(_T("$h"));
		pXLText->AddMergeFormatText(0, 11, _T("$c�� ��"));
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

//	long nHDanType = 0; // ����(����)
	CDPoint xyPave = GetXyHDanByType(bLeft); // x = ������ġ, y = ����ġ (�����곡�ܱ���)

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

	CDPoint xyFootway = GetXyHDanByType(bLeft); // x = ������ġ, y = ����ġ (�����곡�ܱ���)

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

	CDPoint xyFootway = GetXyHDanByType(bLeft); // x = ������ġ, y = ����ġ (�����곡�ܱ���)

	double dLoad = N_CROWD_LOAD * _1000MM;
	double dLength = fabs(xyFootway.x - xyFootway.y);
//	double dLoadPosition = pUnitLoad->m_xyLoadPosition.x;
	double dMoment = pUnitLoad->GetMoment(FALSE);

	pXLText->AddFormatText(_T("$h$m+02(%d) %s (%s)$n"), nIdxMiddle++, pUnitLoad->sName, pBri->GetLoadCaseName(pUnitLoad->GetLoadCaseKind()));
	pXLText->AddFormatText(_T("$h$m+03$cM`(f)$r$c=$r%s kN/m�� x L�� / 2  (���� 5.0 kN/m�� ��������� ����)$n"), CommaC(dLoad));
	pXLText->AddFormatText(_T("$h$m+04$c=$r%.3f x %s x %s�� / 2$m+09$c'=$r%s N.mm$n$n"), N_CROWD_LOAD, CommaC(_1000MM), CommaC(dLength), CommaC(dMoment));
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

	pXLText->AddFormatText(_T("$s+05$h$m+02(%d) %s (%s)$m+23$i[��.��(2015) 3.6.1]$n")
		, nIdxMiddle++, STR_LOAD_VEHICLE, pBri->GetLoadCaseName(pUnitLoad->GetLoadCaseKind()));
	pXLText->AddFormatText(_T("$h$m+03%s ������߰�� (IM)$m+22$i[��.��(2015) 3.7.1]$n"), GetNumberString(nIdxSmall++,NUM_CIRCLE) );

	pXLText->GoPosition(NO_MOVE, COL_POS_3);
	pXLText->AddFormatText(_T("$h"));
	pXLText->AddTable(2, 22, 1, 19);
	pXLText->AddFormatText(_T("$h$me7$c����$m+06$me9$m+08$me4$m+05$me3$cIM$n"));
	pXLText->AddFormatText(_T("$h$me6$md1$c$j$c�ٴ��� ����������ġ�� ������ ��� �ٸ� ����$m+07$me7$c�Ƿ��Ѱ���¸� ������ ��� �Ѱ����$m+06$me5 $m+06$me3$c25%%$n"));
	pXLText->AddFormatText(_T("$h$m+07$me7$c�Ƿ��Ѱ����$m+06$me5 $m+06$me3$c15%%$n"));

	pXLText->GoPosition(NO_MOVE, 0);

	*pXLText += 1;

	long nLR = bLeft == TRUE ? iLEFT : iRIGHT;

	double dEl = (double)VEHICLE_FACTOR_E1;
	double dE2 = (double)VEHICLE_FACTOR_E2;
	double dX = GetLengthSupport(bLeft);
	double dE = dEl * dX + dE2;

	pXLText->AddFormatText(_T("$h$m+03%s ������ ������$m+22$i[��.��(2015) 4.6.2.5]$n"), GetNumberString(nIdxSmall++,NUM_CIRCLE));
	pXLText->AddFormatText(_T("$h$m+04'- �Ϲ�Ȱ���� �ۿ��$n"));
	pXLText->AddFormatText(_T("$h$m+05E$r$c'=$r%.1f x X + %s = %.1f x %.f + %s = %s mm$n"), dEl, CommaC(dE2), dEl, dX, CommaC(dE2), CommaC(dE));

	//��ũƮ���Ϸ� �ۿ��
	if(pCheckData->m_bFloorTrailer)
	{
		pXLText->AddFormatText(_T("$h$m+04'- ��ũƮ���Ϸ����� �ۿ��$n"));
		pXLText->AddFormatText(_T("$h$m+05E$r$c'=$r%.1f x L + %s = %.1f x %s + %s = %s mm$n"), dEl, CommaC(dE2), dEl, CommaC(sVC[nLR].dL3), CommaC(dE2), CommaC(sVC[nLR].dE3));
	}
	*pXLText += 1;

	pXLText->AddFormatText(_T("$h$m+03%s �ڸ��Ʈ$n"), GetNumberString(nIdxSmall++,NUM_CIRCLE));
	pXLText->AddFormatText(_T("$h$m+04'- �Ϲ�Ȱ���� �ۿ��$n"));
	pXLText->AddFormatText(_T("$h$m+05M`(LL)$r$c'=$r(Pr / E) x X x (1 + IM) = (%s / %s x %s) x %s x (1 + %.3f)$n"), CommaC(sVC[nLR].dPr), CommaC(dE), CommaC(_1000MM), CommaC(dX), _dImpactLoad[1]);
	pXLText->AddFormatText(_T("$h$m+06$c'=$r%s N��m$n"), CommaC(sVC[nLR].dMl));

	if(pCheckData->m_bFloorTank)
	{
		HGDOUBLE m_dTankEdit1 = 100;		// ��ũ ��������
		pXLText->AddFormatText(_T("$h$m+04'- ��ũ�˵����� �ۿ��$m+21$i[��ȣ���� P48, ���� ��ݰ�� 15%% ����]$n"));
		pXLText->AddFormatText(_T("$h$m+05Pr$m+02$c'=$rW / 2 / L(��ũ��������) = %s / 2 / %.3f = %s kN$n"), CommaC(sVC[nLR].dTankW), m_dTankEdit1, CommaC(sVC[nLR].dP2));
		pXLText->AddFormatText(_T("$h$m+05M`(LL.TK)$m+02$c'=$rPr x L x (1 + IMTL) = %s x %s x (1 + %.3f)$n"), CommaC(sVC[nLR].dP2), CommaC(sVC[nLR].dL2), _dImpactLoad[2]);
		pXLText->AddFormatText(_T("$h$m+07$c'=$r%s N��mm$n"), CommaC(sVC[nLR].dMtank));
	}
	if(pCheckData->m_bFloorTrailer)
	{
		pXLText->AddFormatText(_T("$h$m+04'- ��ũƮ���Ϸ����� �ۿ��$n"));
		pXLText->AddFormatText(_T("$h$m+05M`(LL.TL)$m+02$c'=$r(Pr / E) x L x (1 + IMTL) = (%s / %s x %s) x %s x (1 + %.3f)$n"), CommaC(sVC[nLR].dTrailerW), CommaC(sVC[nLR].dE3), CommaC(_1000MM), CommaC(sVC[nLR].dL3), _dImpactLoad[2]);
		pXLText->AddFormatText(_T("$h$m+07$c'=$r%s N��mm$n"), CommaC(sVC[nLR].dMTrail));
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
	CGuardWallRC* pGw = pBri->GetGuardWallByHDan(bLeft==TRUE ? 0 : nQtyHDan-1);		// ������ �������� �Ѵ�.
	if( pGw == NULL ) return;


	BOOL bBodo = (pWorking->GetBodoWeight().m_bExist && pWorking->GetBodoWeight().m_dHeight > 0);
	double dPaveT = bBodo ? frM(pWorking->GetBodoWeight().m_dHeight) : pBri->GetThickPave();	// ����β� 
	double dSlabT =	pBri->m_dTS;	// ������ ���� �β�	

	double dWindStrength = GetValueUnitChange(pCheckData->m_dWindStrength, UNIT_CGS_TONF_M2, pData->m_szTonf_M2);	// ǳ���� ����
	double dHeight = pGw->GetHeight();
	double dForceHori = pUnitLoad->m_dForceHori;

	pXLText->AddFormatText(_T("$h$m+02(%d) %s (%s)$m+23$i[��.��(2015) 3.13.2.1]$n")
		, nIdxMiddle++, pUnitLoad->sName, pBri->GetLoadCaseName(pUnitLoad->GetLoadCaseKind()));

	pXLText->AddFormatText(_T("$h$m+03Pw$r$c'=$r%s kN/m��x H = %.3f x %s x %s$n"), CommaC(dWindStrength * _1000MM), dWindStrength, CommaC(_1000MM), CommaC(dHeight, 0, FALSE));
	pXLText->AddFormatText(_T("$h$m+04$c'=$r%s N/m$n"), CommaC(pUnitLoad->m_dForceHori, 0, FALSE));

	pXLText->AddFormatText(_T("$h$m+03Mw$r$c'=$rPw x ( H / 2 + Pt + t/2 ) = %s x ( %s / 2 + %s + %s / 2 )$n"), CommaC(dForceHori, 0, FALSE), CommaC(dHeight, 0, FALSE), CommaC(dPaveT), CommaC(dSlabT));
	pXLText->AddFormatText(_T("$h$m+04$c'=$r%s N��mm$n$n"), CommaC(pUnitLoad->GetMoment(FALSE), 0, FALSE));
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
	CGuardWallRC* pGw = pBri->GetGuardWallByHDan(bLeft==TRUE ? 0 : nQtyHDan-1);		// ������ �������� �Ѵ�.
	if( pGw == NULL ) return;

	double dLendSupport = GetLengthSupport(bLeft);
	BOOL bExistFootway = pWorking->GetBodoWeight().m_bExist;
	double dHeightNangan = pWorking->GetUserLoad(NANGAN).m_bExist ? pWorking->GetUserLoad(NANGAN).m_dHeight : 0;
	double dGuardH	= pGw->GetHeight() + pBri->m_dTS / 2 + dHeightNangan;
// 	double dTerm = m_pStd->GetTermCantilever(pBx, bLeft);
// 	double dSharp = m_pStd->GetSharpCantilever(pBx, bLeft);

	double dTerm = pBri->m_dCantilB[bLeft==TRUE ? 0 : 1];

	long nIdxSmall = 1;

	if(bExistFootway && dLendSupport > 0) // ������ �ְ� ������ �ִ� ���
	{
		pXLText->AddFormatText(_T("$h$m+02 ��꼭 Ÿ�� �߰�$n"));
	}
	else if(!bExistFootway) // ������ ���� ��ȣ�� Ȥ�� �ߺд�
	{
		double dVelocity= pCheckData->m_dCo_V;
		double dHeight = COLLISION_H1 + pBri->GetThickPave() + pBri->m_dTS / 2; // ���� �浹 ����

		double dR = pCheckData->m_dCenter_R[bLeft ? iLEFT : iRIGHT];
		double dH = sCO[nLR].dH;

		pXLText->AddFormatText(_T("$h$m+02(%d) %s (%s)$m+23$i[��.��.��(2008) 2.4.3.6]$n"), nIdxMiddle++, pUnitLoad->sName, pBri->GetLoadCaseName(pUnitLoad->GetLoadCaseKind()));
		pXLText->AddFormatText(_T("$h$m+03%s �����浹��  H�� ���� 1m ���̿� ������ ���� 1m ���� �ۿ��ϴ� ������ �Ѵ�.$n"), GetNumberString(nIdxSmall++,NUM_CIRCLE));
		pXLText->AddFormatText(_T("$h$m+04H = (V / 60)�� x %s + %s = (%s / 60)��x %s + %s = %s N$n"), CommaC(COLLISION_V750), CommaC(COLLISION_V250), CommaC(dVelocity), CommaC(COLLISION_V750), CommaC(COLLISION_V250), CommaC(dH));
		pXLText->AddFormatText(_T("$h$m+04�ۿ� ���� h = %s mm$n"), CommaC(dHeight));
		pXLText->AddFormatText(_T("$h$m+04�ۿ���Ʈ M`(CT1) = H x h = %s x %s = %s N��mm$n$n"), CommaC(dH), CommaC(dHeight), CommaC(dH * dHeight));
		pXLText->AddFormatText(_T("$h$m+03%s ��ȣ�� ��ܿ� Ⱦ�������� 10 kN/m�� ���������� �����Ѵ�.$n"), GetNumberString(nIdxSmall++,NUM_CIRCLE));
		pXLText->AddFormatText(_T("$h$m+04�ۿ� ���� h = %s mm$n"), CommaC(dGuardH));
		pXLText->AddFormatText(_T("$h$m+04�ۿ���Ʈ M`(CT2) = H x h = %s x %s = %s N��mm$n$n"), CommaC(COLLISION_UNIT_H), CommaC(dGuardH), CommaC(COLLISION_UNIT_H * dGuardH));

		if(dR > 0 && dR <= COLLISION_Rstd)
		{
			pXLText->AddFormatText(_T("$h$m+04��������� R = %s m �� %s m �̹Ƿ�, ��� ���� �� �� ū ���� 2�踦 �����Ѵ�.$n"),CommaC(dR), CommaC(COLLISION_Rstd));
		}
		else
		{
			pXLText->AddFormatText(_T("$h$m+04��������� R = %s m > %s m �̹Ƿ�, ��� ���� �� �� ū ���� �����Ѵ�.$n"),CommaC(dR), CommaC(COLLISION_Rstd));
		}

		pXLText->AddFormatText(_T("$h$m+04Mco = %s N��mm$n$n"), CommaC(pUnitLoad->GetMoment(FALSE)));
	}
	else  if(bExistFootway && dLendSupport <= 0) // ������ �ִ� ���
	{
		double dSharp = 200;

		double dH = dGuardH + pWorking->GetUserLoad(NANGAN).m_dHeight;
		double LEN = pBri->GetLengthHDan(bLeft==TRUE ? 0 : nQtyHDan-1);
		double dDist = (dTerm - dSharp/2 - LEN) + (bLeft ? (pGw->m_dW5+pGw->m_dW4) : (pGw->m_dW1+pGw->m_dW2)) + pGw->m_dW3 - sCO[nLR].dLoadPos;

		pXLText->AddFormatText(_T("$h$m+02(%d) %s (%s)$m+23$i[��.��.��(2008) 9.7.3]$n"), nIdxMiddle++, pUnitLoad->sName, pBri->GetLoadCaseName(pUnitLoad->GetLoadCaseKind()));
		pXLText->AddFormatText(_T("$h$m+03%s �����浹�� H�� ������ V�� ��ܺο� ���� ����ο� �ۿ��ϴ� ������ �Ѵ�.$n"), GetNumberString(nIdxSmall++,NUM_CIRCLE));
		pXLText->AddFormatText(_T("$h$m+04���� �浹��$m+04H $r$c'= $r$c$me1%s $r$rN$n"), CommaC(sCO[nLR].dH));
		pXLText->AddFormatText(_T("$h$m+04������$m+04V $r$c'= $r$c$me1%s $r$rN$n"), CommaC(sCO[nLR].dV));
		pXLText->AddFormatText(_T("$h$m+04�ۿ� ���Ʈ Mct = %s x %s + %s x %s = %s N��mm$n"), CommaC(sCO[nLR].dH), CommaC(dH), CommaC(sCO[nLR].dV), CommaC(dDist), CommaC(pUnitLoad->GetMoment(FALSE)));
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

	double dR		= pCheckData->m_dCenter_R[bLeft ? iLEFT : iRIGHT];					// ����ݰ� 
	double dV		= pCheckData->m_dCo_V;						// ����ӵ� : km/hr
	double dV_sec	= pCheckData->m_dCo_V * 1000. / (60.0 * 60.0);						// ����ӵ� : m/sec

	BOOL bTankOrTarailer = (pCheckData->m_bFloorTank || pCheckData->m_bFloorTrailer) ? TRUE : FALSE;

	pXLText->AddFormatText(_T("$h$m+02(%d) %s (%s)$m+23$i[��.��(2015) 3.18]$n"), nIdxMiddle++, pUnitLoad->sName, pBri->GetLoadCaseName(pUnitLoad->GetLoadCaseKind()));
	
	pXLText->AddFormatText(_T("$h$m+03�� ����ӵ�  V = %4.1f km/hr = %4.1f m/s $m+04(�Ϲ�����)$n"), dV, dV_sec);
	if(bTankOrTarailer)
	{
		double dVT		= CENTRIFUGAL_TANK_V;					// ��ũ or Ʈ���Ϸ� ����ӵ�
		double dVT_sec	= CENTRIFUGAL_TANK_V * 1000. / (60.0 * 60.0);					// ��ũ or Ʈ���Ϸ� ����ӵ�

		pXLText->AddFormatText(_T("$h$m+03�� ����ӵ�  V = %4.1f km/hr = %4.1f m/s $m+04(��ũ, ��ũƮ���Ϸ� ����)$n$n"), dVT, dVT_sec);
	}

	pXLText->AddFormatText(_T("$h$m+03- �Ϲ�Ȱ���� �ۿ��$n"));
	pXLText->AddFormatText(_T("$h$m+04C = 4 / 3 x  V�� / (g x R) = 4 / 3 x 33.333�� / (9.81 x %.1f) = %.3f$n"), dR, sCF[nLR].dCf1);
	pXLText->AddFormatText(_T("$h$m+04Pcf = (Pr/E) x C = (%.1f/%.1f) x %.3f = %.3f$n"), sCF[nLR].dPr1, sCF[nLR].dE1, sCF[nLR].dCf1, sCF[nLR].dPcf1);
	pXLText->AddFormatText(_T("$h$m+04Mcf = Pcf x %.1f = %.3f x %.1f = %.3f %s$n"), sCF[nLR].dHeight, sCF[nLR].dPcf1, sCF[nLR].dHeight, sCF[nLR].dMcf1, pData->m_szTonfM);

	if(pCheckData->m_bFloorTank)
	{
		pXLText->AddFormatText(_T("$h$m+03- ��ũ�˵����� �ۿ��$n"));
		pXLText->AddFormatText(_T("$h$m+04C = 4 / 3 x  V�� / (g x R) = 4 / 3 x 11.111�� / (9.81 x %.1f) = %.3f$n"), dR, sCF[nLR].dCf2);
		pXLText->AddFormatText(_T("$h$m+04Pcf = Pr x C = %.1f x %.1f = %.3f N $n"), sCF[nLR].dPr2, sCF[nLR].dCf2, sCF[nLR].dPcf2);
		pXLText->AddFormatText(_T("$h$m+04Mcf = Pcf x %.1f = %.3f x %.1f = %.3f %s$n"), sCF[nLR].dHeight, sCF[nLR].dPcf2, sCF[nLR].dHeight, sCF[nLR].dMcf2, pData->m_szTonfM);
	}

	if (pCheckData->m_bFloorTrailer)
	{
		pXLText->AddFormatText(_T("$h$m+03- ��ũƮ���Ϸ����ۿ��$n"));
		pXLText->AddFormatText(_T("$h$m+04C = 4 / 3 x  V�� / (g x R) = 4 / 3 x 11.111�� / (9.81 x %.1f) = %.3f$n"), dR, sCF[nLR].dCf3);
		pXLText->AddFormatText(_T("$h$m+04Pcf = Pr x C = %.1f x %.1f = %.3f N $n"), sCF[nLR].dPr3, sCF[nLR].dCf3, sCF[nLR].dPcf3);
		pXLText->AddFormatText(_T("$h$m+04Mcf = Pcf x %.1f = %.3f x %.1f = %.3f %s$n"), sCF[nLR].dHeight, sCF[nLR].dPcf3, sCF[nLR].dHeight, sCF[nLR].dMcf3, pData->m_szTonfM);
	}

	pXLText->AddFormatText(_T("$h$m+04 ���������߿��� ���� ū ���� Mcf = %.f N��m�� �����Ѵ�.$n$n"), pUnitLoad->GetMoment(FALSE));

	return TRUE;
}

void CCalcCantilever::WriteCombineTotal( CXLFormatText *pXLText, long &nIdxMiddle, HGBOOL bLeft )
{
	pXLText->OutXL();
	pXLText->GoPosition(NO_MOVE, 0);

	//��Ÿ����ǥ
	pXLText->AddFormatText(_T("$h$s+12"));
	pXLText->GoPosition(NO_MOVE,COL_POS_2);
	pXLText->AddFormatText(_T("$h(%d) ������ǥ$n"), nIdxMiddle++);
	m_pStd->m_pLoad->MakeTableOutValueEta_Lsd(pXLText, ePartUpperSlab, -1, FALSE);

	//���߰��
	pXLText->AddFormatText(_T("$h$s+17"));
	pXLText->GoPosition(NO_MOVE,COL_POS_2);
	pXLText->AddFormatText(_T("$h(%d) ���߰��$n"), nIdxMiddle++);

	////�������� ����(ĵƿ����)
	CLsdManager *pLsdManager = m_pStd->m_pBri->m_plsdMngForCantilever;
	if(pLsdManager)
	{
		long nTypeWrite			= LSD_COMB_DETAIL; //���� ����� �����ų��
		ELSDLoadCaseDir eDir = eLSD_LOADCASE_DIR_PER;
		
		for(long ixComboType=0; ixComboType<eLSD_COMBINATION_FATIGUE; ++ixComboType)
		{
			long nIdxTitle_Trash = 0;
			pLsdManager->WriteTableLsdCombine_Detail(pXLText, eDir, ixComboType, nIdxTitle_Trash, nIdxTitle_Trash, COL_POS_2, _T(""), TRUE, static_cast<long>(eLSD_COMBINE_TOTAL_COUNT), 2);
		}
	}
	*pXLText += 1;

	//��������
	WriteTotalTable(pXLText, nIdxMiddle, bLeft);
	//��������
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

	BOOL bSumLoad = pCheckPos->IsSameLoadCase(); //���� ���� ���̽��� �ִٸ� �հ踦 ���� ��������Ѵ�.

	//Table ����
	long nTotRow = nSizeLoad;
	long nTotCol = bSumLoad ? 20 : 16;

	pXLText->AddFormatText(_T("$h$s+17"));
	pXLText->GoPosition(NO_MOVE,COL_POS_2);
	pXLText->AddFormatText(_T("$h(%d) ��������$n"), nIdxMiddle++);
	
	pXLText->AddTable(nTotRow, nTotCol, 1, 1);

	stCantileverLoadUnit *pLoadNext = NULL;
	BOOL bFirst = TRUE;
	//title
	long nTitleLoadValCol = bSumLoad ? 8 : 4; //���߰� �÷�����
	pXLText->AddFormatText(_T("$h$me8$c%s$m+09$me%d$c%s$m+%02d$me3$c%s$n$h"), hggettext("����"), nTitleLoadValCol-1, hggettext("���߰� (N.mm)"), nTitleLoadValCol, hggettext("���"));

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


		//���� ����Ÿ���� ������� �հ踦 ���ϱ� ����
		if(bSumLoad)
		{
			if((sQes == sQesNext) && (ix < nSizeLoad -1)) //���� ���߰� LoadCase�� ���ٸ� ���� �����Ѵ�. �հ赵 ���س��´�.
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

	//////����
	long nCntCol = 23;
	CString strCombo = _T(""), sMo = _T(""), str = _T("");

	//���ü��� ���� + �� �� ����
	long nSizeCombo = pCheckPos->GetSizeCombo();

	///Print
	pXLText->AddFormatText(_T("$h$s+17"));
	pXLText->GoPosition(NO_MOVE,COL_POS_2);
	pXLText->AddFormatText(_T("$h(%d) �������� $m+23$i���� m�� $n$h"), nIdxMiddle++);

	pXLText->AddTable(nSizeCombo, nCntCol, 1, 1);

	//Title (merge�� 10�� �̻��� ��� �и��ؼ� ���� ���Ѿ���)
	pXLText->AddMergeFormatText(0, 1, _T("$c��Ī"));
	pXLText->AddFormatText(_T("$m+02"));

	pXLText->AddMergeFormatText(0, 17, _T("$c�� �� ��"));
	pXLText->AddFormatText(_T("$m+18"));

	pXLText->AddMergeFormatText(0, 3, _T("$c�հ� (N.mm)"));
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
		str.Format(_T("�� �����Ѱ���´� �������� �߿��� ���� ū Mu = %s N.mm �� �����Ѵ�."), GetStrDouble(pCheckPos->GetMoment(eLSD_COMBINATION_LIMIT), 0, TRUE));
		pXLText->AddFormatText(_T("$h%s $n$h"), str);
	}
	if(pCheckPos->GetSizeCombo(eLSD_COMBINATION_LIMIT) > 0)
	{
		str.Format(_T("�� �شܻ�Ȳ�Ѱ���´� �������� �߿��� ���� ū Mu = %s N.mm �� �����Ѵ�."), GetStrDouble(pCheckPos->GetMoment(eLSD_COMBINATION_EXTREME), 0, TRUE));
		pXLText->AddFormatText(_T("$h%s $n$h"), str);
	}
	if(pCheckPos->GetSizeCombo(eLSD_COMBINATION_SERVICE) > 0)
	{
		str.Format(_T("�� ����Ѱ���´� �������� �߿��� ���� ū Mu = %s N.mm �� �����Ѵ�."), GetStrDouble(pCheckPos->GetMoment(eLSD_COMBINATION_SERVICE), 0, TRUE));
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
	pXLText->AddFormatText(_T("$h(%d) �ٴ��� �β��� ������ ����$n"), nIdxMiddle++);

	pXLText->GoPosition(NO_MOVE,COL_POS_3);
	CString strOK = pCheckPos->IsOkMinThick(FALSE) ? _T("O.K") : _T("N.G");
	str.Format(_T("�� Ȩ �Ǵ� ����������� �β��� �� �ּҵβ��� %.0f mm ���� Ŀ�� �Ѵ�."), pCheckPos->GetThickSlabRequire(FALSE));
	pXLText->AddFormatText(_T("$h%s $m+18�� %s $n$h"), str, strOK);

	strOK = pCheckPos->IsOkMinThick(TRUE) ? _T("O.K") : _T("N.G");
	str.Format(_T("�� ��ũ��Ʈ �ٴ����� %.0fmm ���� �۾Ƽ��� �ȵȴ�."), pCheckPos->GetThickSlabRequire(FALSE));
	pXLText->AddFormatText(_T("$h%s $m+18�� %s $n$n$h"), str, strOK);

	*pXLText += 1;
}

void CCalcCantilever::WriteRebarCheck( CXLFormatText *pXLText, long &nIdxMiddle, HGBOOL bLeft )
{
	CCantileverCheckPosition *pCheckPos = GetCheckPosition(bLeft);

	pXLText->AddFormatText(_T("$h$s+40"));

	pXLText->GoPosition(NO_MOVE,COL_POS_2);
	pXLText->AddFormatText(_T("$h(%d) ö�ٷ� ����$n"), nIdxMiddle++);

	// �ܸ����
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
	pXLText->AddFormatText(_T("$h(%d)��뼺 ����$n"), nIdxMiddle++);

	long nSizeConc = pCheckPos->GetSizeConcData();

	// ��뼺����
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
			pXLText->AddFormatText(_T("$h$m+03$e������ A�� ��տ� ���� �� �տ��� �Ѱ���� �������� �ƴϹǷ� �տ� ���並 �������� �ʽ��ϴ�.$n$h"));
			continue;
		}

		CString sDesign= _T(""), sKind=_T(""), sExtra = _T("");
		double dCrackWitdh = pBri->GetCrackLimitWidth_LSD(ePartUpperSlab, TRUE, sKind, sDesign);
		pXLText->AddFormatText(_T("$h$m+03�� ���� ȯ�濡 ���� ���� ���� ��� : %s %s$n$h"), sDesign, sExtra);
		// 		if(sDesign == _T("A"))	
		// 			pXLText->AddFormatText(_T("$h$r������ A�� ��տ� ���� �� �տ��� �Ѱ���� �������� �ƴϹǷ�$n$h �տ� ���並 �������� �ʽ��ϴ�.$n$h"));

		if(pBri->IsCheckPosEtaUseLoad(ePartUpperSlab) == FALSE)
		{
			pXLText->AddFormatText(_T("$h$m+03������� ���տ��� �Ѱ���� ������ ���� ���������� �̰����Ͽ����Ƿ�$n$h$m+03��뼺 ����� �������� �ʴ´�.$n$n$h"));
			continue;
		}

		if(sDesign != _T("A"))
			pXLText->AddFormatText(_T("$h$m+03$e������ %s �̹Ƿ� �տ��� �Ѱ���´� %s, ǥ�� �Ѱ�տ����� %.1fmm�� ����.$n$n$h"), sDesign,sKind,dCrackWitdh );

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
	double dFy = GetValueUnitChange(pConc->m_Fy, UNIT_CGS_KGF_CM2, _T("N/mm��"));
	double dAsUse = pCheckPos->GetAsUseSupport();
	double dAsReq = pCheckPos->GetAsReqSupport();
	double dCtcUse = pCheckPos->GetCtcUseSupport();
	double dCtcMax = pCheckPos->GetCtcMaxSupport();

	CString strOK1 = pCheckPos->IsOkSupportAs() ? _T("O.K") : _T("N.G");
	CString strOK2 = pCheckPos->IsOkSupportAs() ? _T("��") : _T("��");

	///Print
	pXLText->AddFormatText(_T("$h$s+10"));

	//Title
	pXLText->GoPosition(NO_MOVE,COL_POS_2);
	pXLText->AddFormatText(_T("$h(%d)���ö�� ����$n"), nIdxMiddle++);

	pXLText->GoPosition(NO_MOVE,COL_POS_3);
	CString strTitle = _T("ö�� �ܸ��� ����");
	pXLText->AddFormatText(_T("$h'- %s$n"), strTitle);

	str.Format(_T("As �� %.2f Ag / fy"), SUPPORT_REBAR_AREA_FACTOR);
	pXLText->AddFormatText(_T("$h$r%s$n"), str);

	pXLText->AddFormatText(_T("$h$r���ö�ٷ�$m+03$c=$r$e%.1f mm��$n"), dAsUse);
	pXLText->AddFormatText(_T("$h$r$m+04%c%.0f - CTC %.0f$n"), cHD, pConc->m_TSupportBarDia[nDan], pConc->m_TSupportBarCTC[nDan]);
	pXLText->AddFormatText(_T("$h$r�� As = %.1f mm�� %s %.2f x %.1f x %.1f / %.1f = %.1f mm��  �� %s$n"), dAsUse, strOK2, SUPPORT_REBAR_AREA_FACTOR, pConc->m_Ho, pConc->m_Bo, dFy, dAsReq, strOK1);


	strTitle = _T("���ö�� ���� ����");
	pXLText->AddFormatText(_T("$n$h'- %s$n"), strTitle);
	strOK1 = pCheckPos->IsOkSupportCtc() ? _T("O.K") : _T("N.G");
	strOK2 = pCheckPos->IsOkSupportCtc() ? _T("��") : _T("��");
	str.Format(_T("S �� min(%.0fh, %.1f)"), SUPPORT_REBAR_CTC_FACTOR, SUPPORT_REBAR_CTC_MAX);
	pXLText->AddFormatText(_T("$h$r%s$n"), str);

	pXLText->AddFormatText(_T("$h$r�� S = %.1f mm %s min(%.0f x %.1f, %.1f) = %.1f mm  �� %s$n"), dCtcUse, strOK2, SUPPORT_REBAR_CTC_FACTOR, pConc->m_Ho, SUPPORT_REBAR_CTC_MAX, dCtcMax, strOK1);

	*pXLText += 1;
}

void CCalcCantilever::WriteDeflectionCheck( CXLFormatText *pXLText, HGBOOL bLeft )
{
	CCantileverCheckPosition *pCheckPos = GetCheckPosition(bLeft);

	CString str = _T("");

	BOOL bNeedCheck = pCheckPos->IsNeedDeflectionCheck();
	CString strOK1 = bNeedCheck ? _T("N.G") : _T("O.K");
	CString strOK2 = bNeedCheck ? _T("��") : _T("��");

	double dSpanLen = pCheckPos->GettMinSpanLength();
	double dMinThick = pCheckPos->GettMinDeflection();
	double dSlabThick = pCheckPos->GetThickSlab();
	double dDiv = pCheckPos->GettMinDivide();

	///Print
	pXLText->GoPosition(NO_MOVE,COL_POS_2);
	pXLText->AddFormatText(_T("$h'-ó�� ����� ���� �ʴ� ����� �ں����� �ּ� �β�$n"));

	pXLText->AddFormatText(_T("$h$r t min$m+02$c=$r$eL / %.0f = %.1f / %.0f$m+05$c=$r%.1f mm$n"), dDiv, dSpanLen, dDiv, dMinThick);

	pXLText->AddFormatText(_T("$h$r t min$m+02$c=$r$e%.1f mm %s h = %.1f mm  �� %s$n"), dMinThick, strOK2, dSlabThick, strOK1);

	if(bNeedCheck)
	{
		pXLText->AddFormatText(_T("$h$r�� ������ ó������� �ʿ�.$n"));
	}
	else
	{
		pXLText->AddFormatText(_T("$h$r�� ������ ó������� �ʿ� ����.$n"));
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

	pConc->m_sTitle	= bLeft? _T("���� ĵƿ������") : _T("���� ĵƿ������");
	pConc->m_Bo		= frM(UNIT_METER);
	pConc->m_Ho		= pBri->m_dCantilH1[bLeft? iLEFT : iRIGHT]; // ĵƿ�����β�

	pConc->m_nTypeSection = SECTION_SINGLE;
	pConc->m_dDeflectionCoefficient = 10.0;

	// ���ö�� ����
	for(long ix=0; ix<MAX_REBAR_DAN; ix++)
	{
		CDPoint xyInfo = pBri->GetInfoSupportRebar_UpperSlab(TRUE, 0, ix, 0);
		pConc->m_RbT1C_DIA[ix] = xyInfo.x; // ö���� ����
		if(xyInfo.x>0)
		{
			pConc->m_RbT1C_dc[ix] = pBri->m_dCoverUpper_UpperSlab[ix];  // ö���� �߽ɰŸ� �Ǻ� + 100*�ܼ�(-1)
			pConc->m_RbT1C_B [ix] = pConc->m_Bo/xyInfo.y;  // ö���� ���� (1000/CTC)
		}

		xyInfo = pBri->GetInfoSupportRebar_UpperSlab(FALSE, 0, ix, 0);
		pConc->m_RbC1C_DIA[ix] = xyInfo.x; // ö���� ����
		if(xyInfo.x>0)
		{
			pConc->m_RbC1C_dc[ix] = pBri->m_dCoverLower_UpperSlab[ix];  // ö���� �߽ɰŸ� �Ǻ� + 100*�ܼ�(-1)
			pConc->m_RbC1C_B [ix] = pConc->m_Bo/xyInfo.y;  // ö���� ���� (1000/CTC)
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
