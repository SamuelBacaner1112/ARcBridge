// ARcBridgeMeshCalc.cpp: implementation of the CARcBridgeMeshCalc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ARcBridgeCalculate.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CARcBridgeMeshCalc::CARcBridgeMeshCalc()
{
	m_dArrStation.RemoveAll();
	m_dArrHDanPos.RemoveAll();
	for(long iUD = iUPPER; iUD <= iLOWER; iUD++)
	{
		m_vArrSlab[iUD].RemoveAll();
	}
	m_vArrSlabvPoint_real.RemoveAll();

	m_dArrStation_Archrib.RemoveAll();
	m_dArrHDanPos_Archrib.RemoveAll();
	m_vArrArchRib.RemoveAll();
	long i=0; for(i=0; i<MAX_JIJUM_COUNT_RC; i++)
	{
		m_nIdxJijumStation[i]	 = -1;
		m_nIdxJijumShoePos[0][i] = -1;
		m_nIdxJijumShoePos[1][i] = -1;
		m_vArrWall[i].RemoveAll();
		m_dArrPointJijum[iUPPER][i].RemoveAll();
		m_dArrPointJijum[iLOWER][i].RemoveAll();
		m_dArrPointJijumArch[i].RemoveAll();
		m_dArrPointWallFix[i].RemoveAll();
	}
	m_dArrPointArchRibFix[0].RemoveAll();
	m_dArrPointArchRibFix[1].RemoveAll();
}
 
CARcBridgeMeshCalc::~CARcBridgeMeshCalc()
{
	AhTPADelete(&m_pListMeshSlab[iUPPER],(CMeshFace*)0);
	AhTPADelete(&m_pListMeshSlab[iLOWER],(CMeshFace*)0);
	int i=0; for(i=0; i<MAX_JIJUM_COUNT_RC; i++)
	{
		AhTPADelete(&m_pListMeshWall[i],(CMeshFace*)0);
		AhTPADelete(&m_pListMeshArchRib[i],(CMeshFace*)0);
	}
}

void CARcBridgeMeshCalc::CalculateAll()
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CLineInfo      *pLine = pBri->GetLineBase();

	AhTPADelete(&m_pListMeshSlab[iUPPER],(CMeshFace*)0);
	AhTPADelete(&m_pListMeshSlab[iLOWER],(CMeshFace*)0);
	long i=0; for(i=0; i<MAX_JIJUM_COUNT_RC; i++)
	{
		AhTPADelete(&m_pListMeshWall[i],(CMeshFace*)0);
		AhTPADelete(&m_pListMeshArchRib[i],(CMeshFace*)0);
	}

	m_xyCenter = pLine->GetXyLine(pBri->m_dStationBridgeStt);
	
	if(pBri->IsArchType())
	{
		MakeStationAndHDanPosTypeArch();
	}
	else 
	{
		MakeStationAndHDanPos();
	}
	//////////////////////////////////////////////////////////////////////////
	MakeMeshUpperSlab();
	if(pBri->IsArchType()) 
	{
		MakeMeshArchRib();
	}
	if(pBri->IsBoxType())
	{
		MakeMeshLowerSlab();
	}
	MakeMeshWall();
}


void CARcBridgeMeshCalc::MakeStationAndHDanPos()
{
	m_dArrStation.RemoveAll();
	m_dArrHDanPos.RemoveAll();
	m_dArrHDanType.RemoveAll();
	m_dArrHDanIndex.RemoveAll();

	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	AddStationHDan(FALSE);

	AddHDanInfo(pBri->GetStationBridgeMid());

	if(pBri->IsBoxType() && pBri->IsPileBase(-1))
	{

		CDPoint vAng = CDPoint(0,1);
		CFootingApp *pFooting = pBri->GetFootingByJijumNumber(0);
		CExPileApp *pPile = &pFooting->m_exFooting.m_Pile;
		// 첫번째 말뚝열을 가지고 설정
		double dDistPile = pBri->GetDisSlabRightAct(pBri->GetStationBridgeMid(), vAng);
		double dDistLeft = Round(pBri->GetDisSlabLeftAct(pBri->GetStationBridgeMid(), vAng), 3); 
		CDoubleArray dArrDistPile;
		long nIndexOrg = 0;
		long nCountSide = pBri->GetCountSidePileOrAnchor(pFooting, nIndexOrg, FALSE);
		for(long nPile = 0; nPile < nCountSide; nPile++)
		{
			CPileAttr *pPileAttr = pPile->GetPileAttr(nIndexOrg, nPile);
			if(pPileAttr == NULL) continue;
			dDistPile -= pPileAttr->dist;

			long nSizePos = m_dArrHDanPos.GetSize();
			for(long kk = 0; kk < nSizePos - 1; kk++)
			{
				double dDistHDanRight = m_dArrHDanPos.GetAt(kk + 1);
				if(Compare(dDistPile, dDistLeft, _T(">"), MIN_LEN_SHELL) && Compare(dDistPile, dDistHDanRight, _T("<"), MIN_LEN_SHELL))
				{
					long nType = (long)m_dArrHDanType.GetAt(kk);
					long nNode = (long)m_dArrHDanIndex.GetAt(kk);
					m_dArrHDanPos.InsertAt(kk+1,Round(dDistPile,3));
					m_dArrHDanType.InsertAt(kk+1,nType);
					m_dArrHDanIndex.InsertAt(kk+1,nNode);
					break;
				}
				dDistLeft = dDistHDanRight;
				if(dDistPile <  dDistLeft)
					break;
			}
		}
	}


	// 가각이 있을때 가각에 대한 부분생성
	if(pBri->m_bIsExp)
	{
		AddStationForGagak();
		AddHDanPosForGagak();
	}
	else
	{
		m_nIdxGagakStation[0][0] = 0;
		m_nIdxGagakStation[0][1] = 0;
		m_nIdxGagakStation[1][0] = m_dArrStation.GetSize()-1;
		m_nIdxGagakStation[1][1] = m_dArrStation.GetSize()-1;
	}

	AddShoeAndColPos();
	MakeStationforPF();
}

HGDOUBLE CARcBridgeMeshCalc::AddStationHDan(HGBOOL bArch, HGDOUBLE *pDW)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	//////////////////////////////////////////////////////////////////////////
	// 교량진행방향에 대한 도로중심 스테이션을 저장
	// 지점의 중심 및 벽체두께위치를 저장
	// dShoePos[] = 슬래브교 혹은 파이형 라멘인 경우
	long nJijumConnectRib = pBri->GetJijumConnectRibAndSlab();

	double dStaJijum = 0, dStaUnit = 0, /*dStaNext  = 0,*/ dStaCur = 0, dShoePos[2]={0,0}, dStationAdd = 0, dW =0;
	for(long nSpan=0; nSpan<pBri->m_nQtyJigan; nSpan++)
	{
		CJijum *pJ1 = pBri->GetJijum(nSpan);
		CJijum *pJ2 = pBri->GetJijum(nSpan+1);

		dStaJijum = pBri->GetStationOnJijum(nSpan);
		double dStaNext  = pBri->GetStationOnJijum(nSpan+1);

		if (bArch == FALSE)
		{
			if(pBri->IsRcSlab() || pBri->IsPiRahmen())
			{
				if(nSpan == 0)
				{
					dStaJijum += pJ1->m_dLengthUgan[1];
				}
				if(nSpan == pBri->m_nQtyJigan-1)
				{
					dStaNext -= pJ2->m_dLengthUgan[0];
				}
			}
		}

		dShoePos[0] = pJ1->m_dLengthSlabToShoe[1];
		dShoePos[1] = pJ2->m_dLengthSlabToShoe[0];

		m_nIdxJijumStation[nSpan] = MAX(0, m_dArrStation.GetSize());

		long nQtyStation = 0;
		if (bArch)
		{
			if(nSpan+1 == nJijumConnectRib || nSpan == nJijumConnectRib)
			{
				CWallApp *pWall = pBri->GetInWall(nJijumConnectRib-1);
				dW = pWall->m_dW;

				if(nSpan+1 == nJijumConnectRib) dStaNext -= dW/2;
				else dStaJijum += dW/2;
			}

			if(nSpan == nJijumConnectRib)
			{
				double dStaTemStt = dStaJijum - dW/2;
				nQtyStation = (long)Round((dW/2) / 1000.0,0);
				dStaUnit = (dW/2) / (double)nQtyStation;

				long i=0; for(i=0; i<nQtyStation; i++)
				{
					dStaCur = dStaTemStt + dStaUnit*i;
					if(IsExistValueInArray(m_dArrStation,dStaCur)==FALSE)
						m_dArrStation.Add(dStaCur);
				}
			}
		}
		
		nQtyStation = (long)Round((dStaNext - dStaJijum) / 1000.0,0);

		if (bArch)
		{
			if(nSpan==0 && pBri->m_dWS>0)
			{
				if(IsExistValueInArray(m_dArrStation, dStaJijum - pBri->m_dWS/2)==FALSE)
					m_dArrStation.Add(dStaJijum - pBri->m_dWS/2);
			}
		}
		else
		{
			if(!pBri->IsRcSlab() && nSpan==0 && pBri->m_dWS>0)
			{
				if(IsExistValueInArray(m_dArrStation, dStaJijum - pBri->m_dWS/2)==FALSE)
					m_dArrStation.Add(dStaJijum - pBri->m_dWS/2);
			}
		}

		if(dShoePos[0]>0)
		{
			if(IsExistValueInArray(m_dArrStation, dStaJijum)==FALSE)
				m_dArrStation.Add(dStaJijum);
			m_nIdxJijumShoePos[1][nSpan] = m_dArrStation.GetSize();
		}
		dStaUnit = ((dStaNext-dShoePos[1]) - (dStaJijum+dShoePos[0])) / (double)nQtyStation;
		long i=0; for(i=0; i<nQtyStation; i++)
		{
			dStaCur = dStaJijum + dShoePos[0] + dStaUnit*i;
			if(IsExistValueInArray(m_dArrStation,dStaCur)==FALSE)
				m_dArrStation.Add(dStaCur);

			HGBOOL bAdd = bArch ? (i==nQtyStation-1 && nSpan<pBri->m_nQtyJigan-1 && nSpan+1!=nJijumConnectRib) : (i==nQtyStation-1 && nSpan<pBri->m_nQtyJigan-1);
			if(bAdd)
			{
				long nIdxInWall = nSpan;
				CWallApp *pWall = pBri->GetInWall(nIdxInWall);
				dStationAdd = dStaNext - pWall->m_dW/2;
				if(pWall->m_bIs && IsExistValueInArray(m_dArrStation,dStationAdd)==FALSE)
					m_dArrStation.Add(dStationAdd);
			}

			bAdd = bArch ? (i==0 && nSpan>0 && nSpan!=nJijumConnectRib) : (i==0 && nSpan>0);
			if(bAdd)
			{
				long nIdxInWall = nSpan - 1;
				CWallApp *pWall = pBri->GetInWall(nIdxInWall);
				dStationAdd = dStaCur + pWall->m_dW/2;
				if(pWall->m_bIs && IsExistValueInArray(m_dArrStation,dStationAdd)==FALSE)
					m_dArrStation.Add(dStationAdd);
			}
		}

		if (bArch)
		{
			if(nSpan+1 == nJijumConnectRib)
			{
				double dStaTemStt = dStaNext;
				nQtyStation = (long)Round((dW/2) / 1000.0,0);
				dStaUnit = (dW/2) / (double)nQtyStation;

				long i=0; for(i=0; i<nQtyStation; i++)
				{
					dStaCur = dStaTemStt + dStaUnit*i;
					if(IsExistValueInArray(m_dArrStation,dStaCur)==FALSE)
						m_dArrStation.Add(dStaCur);
				}
			}
		}
		

		if(dShoePos[1]>0)
		{
			m_nIdxJijumShoePos[0][nSpan+1] = m_dArrStation.GetSize();
			if(IsExistValueInArray(m_dArrStation, dStaNext-dShoePos[1])==FALSE)
				m_dArrStation.Add(dStaNext-dShoePos[1]);
		}
		else if(bArch == FALSE && dShoePos[1]==0 && pBri->IsRcSlab())
		{
			m_nIdxJijumShoePos[0][nSpan+1] = m_dArrStation.GetSize();
		}

		// 함형라멘교 말뚝위치
		if(bArch == FALSE && pBri->IsBoxType() && pBri->IsPileBase(-1))
		{
			CFootingApp *pFooting = pBri->GetFootingByJijumNumber(0);

			long nIndexStt = pBri->GetIndexPileOrAnchorSttAtSpan(pFooting, nSpan, FALSE);
			long nIndexEnd = pBri->GetIndexPileOrAnchorEndAtSpan(pFooting, nSpan, FALSE);
			//double dStationPile = 0;
			for(long nPile = nIndexStt; nPile <= nIndexEnd; nPile++)
			{
				double dStationPile = pBri->GetStationPileOrAnchor(pFooting, nPile, FALSE);
				if(IsExistValueInArray(m_dArrStation, dStationPile)==FALSE)
					m_dArrStation.Add(dStationPile);
			}
		}

		if (bArch == FALSE)
			m_dArrStation.RemoveSameValue();

		if(nSpan==pBri->m_nQtyJigan-1)
		{
			if(IsExistValueInArray(m_dArrStation, dStaNext)==FALSE)
				m_dArrStation.Add(dStaNext);
			HGBOOL bAdd = bArch ? (pBri->m_dWE>0) :(!pBri->IsRcSlab() && pBri->m_dWE>0);
			if(bAdd)
			{
				if(IsExistValueInArray(m_dArrStation, dStaNext + pBri->m_dWE/2)==FALSE)
					m_dArrStation.Add(dStaNext + pBri->m_dWE/2);
			}
			m_nIdxJijumStation[pBri->m_nQtyJigan] = m_dArrStation.GetSize() - 1;
		}
	}

	m_dArrStation.Sort();

	if (pDW)
	{
		*pDW = dW;
	}

	return dStaJijum;
}

void CARcBridgeMeshCalc::AddHDanInfo(HGDOUBLE dStation)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	//////////////////////////////////////////////////////////////////////////
	// 도로중심에 대한 횡단위치를 저장.
	// 교량시점을 기준으로
	CDPoint vAng = CDPoint(0,1);
	double dDistLeft = Round(pBri->GetDisSlabLeftAct(dStation, vAng), 3); 
	//double dLengthHDan = 0;
	double dLengthSum  = 0;
	double dLengthUnit = 0;
	double dDivideUnit = 1200;
	//long nTypeHDan = 0;

	m_dArrHDanPos.Add(Round(dDistLeft,3));
	for(long nNode=0; nNode<pBri->GetQtyHDan(); nNode++)
	{
		double dLengthHDan = pBri->GetLengthHDan(nNode);
		long nTypeHDan   = pBri->GetTypeHDan(nNode);
		if(Compare(dLengthHDan,0.00,"=") || nTypeHDan<0) continue;

		if(dLengthHDan>dDivideUnit)
		{
			long nQtyNode = (long)(dLengthHDan/dDivideUnit+1);
			dLengthUnit = dLengthHDan/nQtyNode;
			long i=0; for(i=1; i<=nQtyNode; i++)
			{
				dLengthSum += dLengthUnit;
				m_dArrHDanPos.Add(Round(dDistLeft + dLengthSum,3));
				m_dArrHDanType.Add(nTypeHDan);
				m_dArrHDanIndex.Add(nNode);
			}
		}
		else
		{
			dLengthSum += dLengthHDan;
			m_dArrHDanPos.Add(Round(dDistLeft + dLengthSum,3));
			m_dArrHDanType.Add(nTypeHDan);
			m_dArrHDanIndex.Add(nNode);
		}
	}
}

void CARcBridgeMeshCalc::MakeMeshSlabCoord(HGBOOL bUpper)
{
	const HGINT32 ixSlab = bUpper ? iUPPER : iLOWER;

	CRcBridgeRebar *pBri  = m_pStd->m_pBri;
	CLineInfo      *pLine = pBri->GetLineBase();

	m_vArrSlab[ixSlab].RemoveAll();
	m_vArrSlabvPoint_real.RemoveAll();

	long nQtySta = m_dArrStation.GetSize();
	long nQtyPos = m_dArrHDanPos.GetSize();
	long nJijum = 0;

	double /*dStation=0,*/dStationSc=0,dDist=0;
	CDPoint vAngStt = pBri->GetAngleJijum(0);

	BOOL bLsd = pBri->m_pARcBridgeDataStd->IsLsdDesign();
	BOOL bJongDan = FALSE;
	BOOL bArch = FALSE;
	long nJijumConnectRib = 0;

	double dElBase = 0;
	double dInWallLeft = 0;
	double dInWallRight = 0;

	if (bUpper)
	{
		bJongDan = (pBri->IsArchType() || (pBri->IsBoxType() && pBri->m_bSlopeLowerSlab == FALSE)) ? TRUE : FALSE;
		bArch = pBri->IsArchType();
		nJijumConnectRib = pBri->GetJijumConnectRibAndSlab();

		double dWS = pBri->m_dWS/vAngStt.y;
		dElBase = pLine->GetElevationPlanEl(pBri->m_dStationBridgeStt - dWS/2);
		dInWallLeft = bArch ? pBri->GetWidthArchRib(-1) : 0;
		dInWallRight = bArch ? pBri->GetWidthArchRib(1) : 0;
	}

	const HGDOUBLE dZ = bUpper ? 0 : -(bLsd? pBri->GetHeightModelJijum(nJijum) : toM(pBri->GetHeightModelJijum(nJijum)));
	CVector vVec(0,0,dZ);
	CDPoint xy(0,0);

	long nCountStaAdd = 0;
	// BOX형이고 부상방지 저판이 있다면.
	if(!bUpper && pBri->IsBoxType() && pBri->m_bExistProtectionRiseFooting && pBri->m_dPRF_W > 0)
	{
		nCountStaAdd = m_dArrStation_Pf[iSTT].GetSize() + m_dArrStation_Pf[iEND].GetSize();
	}

	// SLAB 좌표 생성...
	m_dArrStation.Sort();
	CDPoint vAng(0,0);
	CDoubleArray dArr, dArrType, dArrIndex;
	long nSizeUpperJoint = bUpper ? 0 : m_vArrSlab[iUPPER].GetSize();
	long i=0; for(i=0; i<nQtySta+nCountStaAdd; i++)
	{
		double dStation = 0;
		if(i >= nQtySta)
		{
			long idxStaBase = 0;
			if(i - nQtySta < m_dArrStation_Pf[iSTT].GetSize())
			{
				dStation = m_dArrStation_Pf[iSTT].GetAt(i - nQtySta);
				idxStaBase = 0;
			}
			else
			{
				long idx = i - m_dArrStation_Pf[iSTT].GetSize() - nQtySta;
				dStation = m_dArrStation_Pf[iEND].GetAt(idx);
				idxStaBase = nQtySta-1;
			}
			
			double dStationBase = m_dArrStation.GetAt(idxStaBase);
			nJijum = pBri->GetJijumNumberByStation(dStationBase);
			vAng   = pBri->GetAngleByStationForMesh(dStationBase);
			nQtyPos = GetArrayHDanPosByStationIndex(idxStaBase, dArr, dArrType, dArrIndex);
		}
		else
		{
			dStation = m_dArrStation.GetAt(i);
			nJijum = pBri->GetJijumNumberByStation(dStation);
			vAng   = pBri->GetAngleByStationForMesh(dStation);
			nQtyPos = GetArrayHDanPosByStationIndex(i, dArr, dArrType, dArrIndex);
		}
		long j=0; for(j=0; j<nQtyPos; j++)
		{
			dDist = dArr.GetAt(j);
			dStationSc = pLine->GetStationByScInLine(dStation,dDist,vAng);

			//////////////////////////////////////////////////////////////////////////
			// 지점위치에 대한 저장
			BOOL bJijumPoint = FALSE;
			long k=0; for(k=0; k<pBri->GetCountJijum(); k++)
			{
				if(i==m_nIdxJijumStation[k])
				{
					bJijumPoint = TRUE;
					break;
				}
			}
			if(bJijumPoint)
			{
				BOOL bExistWall = TRUE;
				if (bUpper)
				{
					if(nJijum==0) bExistWall = pBri->IsOutsideWall();
					else if(nJijum==pBri->m_nQtyJigan) bExistWall = pBri->IsOutsideWall();
					else if(pBri->GetInWall(nJijum-1)->m_bIs==FALSE) bExistWall = FALSE;
					if(nJijum==nJijumConnectRib) bExistWall = FALSE;
				}
				else
				{
					if((nJijum > 0 && nJijum < pBri->m_nQtyJigan) && pBri->GetInWall(nJijum-1)->m_bIs == FALSE) bExistWall = FALSE;
				}

				if(bExistWall)
				{
					BOOL bColumn = (pBri->GetTypeColumnByJijum(nJijum)!=-1) ? TRUE : FALSE;
					if(bColumn)
					{
						// 기둥위치인 점만 저장...
						CWallApp *pWall = pBri->GetInWall(nJijum-1);
						CDPoint vAngJijum = pBri->GetAngleJijum(nJijum);
						double dColumnDist = pBri->GetDisSlabLeftAct(dStation, vAngJijum)*vAngJijum.y; 
						for(long nCol=0; nCol<pWall->m_nCountColumn; nCol++)
						{
							dColumnDist += pWall->m_dDistColumn[nCol]*vAngJijum.y;
							if(Compare(dColumnDist, dDist,"=", MIN_LEN_SHELL))
							{
								m_dArrPointJijum[ixSlab][nJijum].Add(nSizeUpperJoint + m_vArrSlab[ixSlab].GetSize());
								break;
							}
						}
					}
					else
					{
						// 지점부의 슬래브 포인트를 저장 => 벽체와 연결
						if(bArch && (nJijum!=0 && nJijum!=pBri->m_nQtyJigan))
						{
							if(dDist >= -dInWallLeft && dDist<=dInWallRight)
							{
								m_dArrPointJijum[ixSlab][nJijum].Add(nSizeUpperJoint + m_vArrSlab[ixSlab].GetSize());
							}
						}
						else
						{
							m_dArrPointJijum[ixSlab][nJijum].Add(nSizeUpperJoint + m_vArrSlab[ixSlab].GetSize());
						}
					}
				}
			}

			if (bUpper)
			{
				//////////////////////////////////////////////////////////////////////////
				// 받침위치에 대한 저장
				BOOL bSttShoePoint = FALSE; // 전열의 받침이 있는 경우
				BOOL bEndShoePoint = FALSE; // 후열의 받침이 있는 경우
				for(k=0; k<pBri->GetCountJijum(); k++)
				{
					if(i==m_nIdxJijumShoePos[0][k])
					{
						bSttShoePoint = TRUE;
						break;
					}
				}
				for(k=0; k<pBri->GetCountJijum(); k++)
				{
					if(i==m_nIdxJijumShoePos[1][k])
					{
						bEndShoePoint = TRUE;
						break;
					}
				}
				if(bSttShoePoint && IsShoePos(dDist,nJijum,TRUE))
				{
					m_dArrJointShoeoPos[0][nJijum].Add(m_vArrSlab[ixSlab].GetSize());
				}
				if(bEndShoePoint && IsShoePos(dDist,nJijum,FALSE))
				{
					m_dArrJointShoeoPos[1][nJijum].Add(m_vArrSlab[ixSlab].GetSize());
				}
			}
			
			//////////////////////////////////////////////////////////////////////////
			xy = pLine->GetXyLineDis(dStationSc,dDist);
			m_vArrSlabvPoint_real.Add(xy);

			xy = GetXyRotateVector(m_xyCenter,xy,pBri->GetXyDirRotateDomByBridgeSttEndXy());


			vVec.x = bLsd? (xy.x - m_xyCenter.x) : toM(xy.x - m_xyCenter.x);
			vVec.y = bLsd? (xy.y - m_xyCenter.y) : toM(xy.y - m_xyCenter.y);
			if(bJongDan) 
			{
				vVec.z = bLsd? (pLine->GetElevationPlanEl(dStationSc) - dElBase) : toM (pLine->GetElevationPlanEl(dStationSc) - dElBase);
			}

			m_vArrSlab[ixSlab].Add(vVec);
		}
	}
}

// 시점에서 종점으로....
// 각 지간별로
// 교량좌측 -> 교량우측
void CARcBridgeMeshCalc::MakeMeshUpperSlab()
{
	MakeMeshSlabCoord(TRUE);

	// SLAB MESH 생성...
	// m_pListMeshSlab[iUPPER]에 저장.. 분리형 슬래브인경우에만 1 ~ 사용
	long nIndexFace = 0;
	long nSumJointIdx = 0;
	MakeMeshSlab(nIndexFace, nSumJointIdx, 0, m_vArrSlab[iUPPER], m_pListMeshSlab[iUPPER]);
	
	CRcBridgeRebar *pBri  = m_pStd->m_pBri;

	// Constraint되는 절점의 Index저장
	if(pBri->IsArchType())
	{
		long nQtyPosArchRib = m_dArrHDanPos_Archrib.GetSize();
		double dPosLeft = m_dArrHDanPos_Archrib.GetAt(0);
		double dPosRight = m_dArrHDanPos_Archrib.GetAt(nQtyPosArchRib-1);

		nSumJointIdx = 0;
		long nQtySta = m_dArrStation.GetSize();
		CDoubleArray dArr, dArrType, dArrIndex;
		for(HGINT32 i=0; i<nQtySta; i++)
		{
			HGINT32 nQtyPos = GetArrayHDanPosByStationIndex(i, dArr, dArrType, dArrIndex);
			long j=0; for(j=0; j<nQtyPos; j++)
			{
				double dPosCur = dArr.GetAt(j);
				if(i>=m_nIndexStaConstraintStt && i<=m_nIndexStaConstraintEnd && dPosCur>=dPosLeft && dPosCur<=dPosRight)
				{
					m_dArrConstraintJointIdx_Slab.Add(nSumJointIdx + j);
				}
			}
			nSumJointIdx += nQtyPos;
		}
	}
}

void CARcBridgeMeshCalc::MakeMeshLowerSlab()
{
	MakeMeshSlabCoord(FALSE);

	// SLAB MESH 생성...
	// m_pListMeshSlab[iLOWER]에 저장.. 분리형 슬래브인경우에만 1 ~ 사용
	long nSizeUpperJoint = m_vArrSlab[iUPPER].GetSize();
	long nIndexFace = m_pListMeshSlab[iUPPER].GetSize();
	long nSumJointIdx = nSizeUpperJoint;
	MakeMeshSlab(nIndexFace, nSumJointIdx, nSizeUpperJoint, m_vArrSlab[iLOWER], m_pListMeshSlab[iLOWER]);
}

void CARcBridgeMeshCalc::MakeMeshSlab(long nIndexFace, long nSumJointIdx, long nSizeBeforeJoint, const CVectorArray &vArrSlab, CTypedPtrArray <CObArray, CMeshFace*> &arrListMeshSlab)
{
	CRcBridgeRebar *pBri  = m_pStd->m_pBri;

	CDoubleArray dArr, dArrType, dArrIndex;

	long nQtySta = m_dArrStation.GetSize();
	long nQtyPos = m_dArrHDanPos.GetSize();
	long nQtyPosCur = 0, nQtyPosNex = 0;
	
	double dStation=0;
	double dStation1=0,dStation2=0;

	long nCountStaAdd = 0;
	long nSumJointIndexNotPf(0);
	long nSumJointIdxBase = nSumJointIdx;
	// BOX형이고 부상방지 저판이 있다면.
	if(nIndexFace > 0 && pBri->IsBoxType() && pBri->m_bExistProtectionRiseFooting && pBri->m_dPRF_W > 0)
	{
		nCountStaAdd = m_dArrStation_Pf[iSTT].GetSize() + m_dArrStation_Pf[iEND].GetSize();
	}

	for(HGINT32 i=0; i<nQtySta+nCountStaAdd-1; i++)
	{
		long nPosPf = 0;
		long idxStaBase = i;
		if(i >= nQtySta-1)
		{
			BOOL bStt = (i < nQtySta - 1 + nCountStaAdd/2)? TRUE : FALSE;
			idxStaBase = bStt? 0 : nQtySta-1;
			nPosPf = bStt? -1 : 1;
		}
		if(pBri->m_bIsExp)
		{
			CDoubleArray dArrCur, dArrNext;
			if(i < nQtySta-1)
			{
				nQtyPosCur = GetArrayHDanPosByStationIndex(i, dArrCur, dArrType, dArrIndex);
				nQtyPosNex = GetArrayHDanPosByStationIndex(i+1, dArrNext, dArrType, dArrIndex);

				nQtyPos = MAX(nQtyPosCur, nQtyPosNex);
				if(nQtyPosCur >= nQtyPosNex) nQtyPosCur = GetArrayHDanPosByStationIndex(i, dArr, dArrType, dArrIndex);
			}
			else
			{
				nQtyPos = GetArrayHDanPosByStationIndex(idxStaBase, dArr, dArrType, dArrIndex);
				nQtyPosCur = nQtyPosNex = nQtyPos;
			}
			
			long nIdxStaSttLeft = m_nIdxGagakStation[0][0];
			long nIdxStaSttRight = m_nIdxGagakStation[0][1];
			long nIdxStaEndLeft = m_nIdxGagakStation[1][0];
			long nIdxStaEndRight = m_nIdxGagakStation[1][1];
			if(idxStaBase >= nIdxStaEndLeft && idxStaBase < nIdxStaSttRight)
			{
				dArrCur.Add(-1);
				dArrType.Add(-1);
				dArrIndex.Add(-1);
				nQtyPos++;
			}
			else if(idxStaBase >= nIdxStaEndRight && idxStaBase < nIdxStaSttLeft)
			{
				dArrCur.Add(-1);
				dArrType.Add(-1);
				dArrIndex.Add(-1);
				nQtyPos++;
			}
		}
		else 
		{
			nQtyPosCur = GetArrayHDanPosByStationIndex(idxStaBase, dArr, dArrType, dArrIndex);
			nQtyPos = nQtyPosNex = nQtyPosCur;
		}
		long nIndexPosStd = 0;
		long j=0; for(j=0; j<nQtyPos-1; j++)
		{
			CMeshFace *pFace = new CMeshFace;

			pFace->m_nIndexFace = nIndexFace++;
			pFace->m_nTypeHDan  = (long)dArrType.GetAt(j);
			pFace->m_nIndexHDan = (long)dArrIndex.GetAt(j);
			pFace->m_nIndexPos  = j;
			pFace->m_nIndexPosStd = (pFace->m_nTypeHDan!=-1) ? nIndexPosStd++ : -1;

			// 시종점부상방지 저판은 절점번호가 연결되지 않으니까 좌우를 구할때 절점 Index를 따로 줘야 한다.
			if(nPosPf == 0)
			{
				pFace->m_nIndexJoint[0] = GetNumberJoint(nSumJointIdx, nQtyPosCur, nQtyPos, j, i, TRUE, TRUE);	// 좌측상단
				pFace->m_nIndexJoint[1] = GetNumberJoint(nSumJointIdx, nQtyPosCur, nQtyPos, j, i, FALSE, TRUE);	// 우측상단
				pFace->m_nIndexJoint[2] = GetNumberJoint(nSumJointIdx, nQtyPosCur, nQtyPos, j, i, TRUE, FALSE);	// 좌측하단
				pFace->m_nIndexJoint[3] = GetNumberJoint(nSumJointIdx, nQtyPosCur, nQtyPos, j, i, FALSE, FALSE);	// 우측하단
			}
			else if(nPosPf == -1)
			{
				pFace->m_nIndexJoint[0] = GetNumberJoint(nSumJointIdx, nQtyPosCur, nQtyPos, j, i, TRUE, TRUE);	// 좌측상단
				pFace->m_nIndexJoint[1] = GetNumberJoint((i == nQtySta - 2 + nCountStaAdd/2)? nSumJointIdxBase : nSumJointIdx, nQtyPosCur, nQtyPos, j, i, FALSE, TRUE);	// 우측상단
				pFace->m_nIndexJoint[2] = GetNumberJoint(nSumJointIdx, nQtyPosCur, nQtyPos, j, i, TRUE, FALSE);	// 좌측하단
				pFace->m_nIndexJoint[3] = GetNumberJoint((i == nQtySta - 2 + nCountStaAdd/2)? nSumJointIdxBase : nSumJointIdx, nQtyPosCur, nQtyPos, j, i, FALSE, FALSE);	// 우측하단
			}
			else
			{
				pFace->m_nIndexJoint[0] = GetNumberJoint((i == nQtySta - 1 + nCountStaAdd/2)? nSumJointIndexNotPf : nSumJointIdx, nQtyPosCur, nQtyPos, j, i, TRUE, TRUE);	// 좌측상단
				pFace->m_nIndexJoint[1] = GetNumberJoint(nSumJointIdx, nQtyPosCur, nQtyPos, j, i, FALSE, TRUE);	// 우측상단
				pFace->m_nIndexJoint[2] = GetNumberJoint((i == nQtySta - 1 + nCountStaAdd/2)? nSumJointIndexNotPf : nSumJointIdx, nQtyPosCur, nQtyPos, j, i, TRUE, FALSE);	// 좌측하단
				pFace->m_nIndexJoint[3] = GetNumberJoint(nSumJointIdx, nQtyPosCur, nQtyPos, j, i, FALSE, FALSE);	// 우측하단
			}

			// 절점이 없는 위치를 맨마지막으로 바꿔준다.
			if(pFace->m_nIndexJoint[0]==pFace->m_nIndexJoint[2] || pFace->m_nIndexJoint[1]==pFace->m_nIndexJoint[3])
			{
				if(pFace->m_nIndexJoint[0]==pFace->m_nIndexJoint[2])
				{
					pFace->m_nIndexJoint[0] = pFace->m_nIndexJoint[3];
					pFace->m_nIndexJoint[3] = pFace->m_nIndexJoint[2];
				}
				else
				{
					pFace->m_nIndexJoint[1] = pFace->m_nIndexJoint[2];
					pFace->m_nIndexJoint[2] = pFace->m_nIndexJoint[3];
				}
			}

			if(pFace->m_nIndexJoint[0] > -1) pFace->m_vPoint[0] = vArrSlab.GetAt(pFace->m_nIndexJoint[0] - nSizeBeforeJoint);
			if(pFace->m_nIndexJoint[1] > -1) pFace->m_vPoint[1] = vArrSlab.GetAt(pFace->m_nIndexJoint[1] - nSizeBeforeJoint);
			if(pFace->m_nIndexJoint[2] > -1) pFace->m_vPoint[2] = vArrSlab.GetAt(pFace->m_nIndexJoint[2] - nSizeBeforeJoint);
			if(pFace->m_nIndexJoint[3] > -1) pFace->m_vPoint[3] = vArrSlab.GetAt(pFace->m_nIndexJoint[3] - nSizeBeforeJoint);

			if(nPosPf == 0)
			{
				dStation1 = m_dArrStation.GetAt(i);
				dStation2 = m_dArrStation.GetAt(i+1);
				dStation  = (dStation1+dStation2)/2;
				pFace->m_nSpanSlab = GetNumberSlabSpan(dStation);
				pFace->m_bSpanCheckStt = IsSpanCheckStt(dStation1,dStation2);
				pFace->m_bSpanCheckEnd = IsSpanCheckEnd(dStation1,dStation2);
			}
			else
			{
				pFace->m_nSpanSlab = (nPosPf == -1)? -1 : -2;
				pFace->m_bSpanCheckStt = FALSE;
				pFace->m_bSpanCheckEnd = FALSE;
			}

			arrListMeshSlab.Add(pFace);
		}

		nSumJointIdx += nQtyPosCur;
		if(i == nQtySta-2)
		{
			nSumJointIndexNotPf = nSumJointIdx;
			nSumJointIdx += nQtyPosNex;
		}
		else if(nPosPf == -1 && i == nQtySta - 2 + nCountStaAdd/2)
		{
			nSumJointIdx -= nQtyPosCur;
		}
	}
}


// 슬래브의 지점좌표에 연결해서 생성
void CARcBridgeMeshCalc::MakeMeshWall()
{
	CRcBridgeRebar *pBri  = m_pStd->m_pBri;
	CLineInfo      *pLine = pBri->GetLineBase();

	if(pBri->IsRcSlab()) return;
	BOOL bLsd = pBri->m_pARcBridgeDataStd->IsLsdDesign();
	BOOL bOutWall = pBri->IsOutsideWall();

	BOOL bArch = (pBri->IsArchType())? TRUE : FALSE;
	long nJijumConnectRib = pBri->GetJijumConnectRibAndSlab();
	long nSizeUpperJoint = m_vArrSlab[iUPPER].GetSize();
	CDPoint vAngStt = pBri->GetAngleJijum(0);
//	double dWS = pBri->m_dWS/vAngStt.y;
	long nQtyPos = 0;

	CVector vVec(0,0,0);
	for(long nJijum=0; nJijum<MAX_JIJUM_COUNT_RC; nJijum++)
	{
		m_vArrWall[nJijum].RemoveAll();
		m_dArrPointWallFix[nJijum].RemoveAll();

		if((nJijum==0 || nJijum==pBri->m_nQtyJigan) && !bOutWall) continue;
		if(nJijum > pBri->m_nQtyJigan) continue;
		if(bArch && nJijum == nJijumConnectRib) continue;

		BOOL bColumn = (pBri->GetTypeColumnByJijum(nJijum)!=-1) ? TRUE : FALSE;
		double cosVal =  0;
		if(nJijum>0 && nJijum<pBri->m_nQtyJigan)
		{
			CWallApp *pWall = pBri->GetInWall(nJijum-1);
			double dRadian = ToRadian(pWall->GetAngleWallByHor());
			cosVal = cos(dRadian);
			// cos값은 각도의 +,-와는 상관없이  +값이 나온다.
			if(dRadian < 0) cosVal *= -1;
		}

		// 절점좌표 생성
		CDoubleArray dArr, dArrType, dArrIndex;

		nQtyPos = GetArrayHDanPosByStationIndex(m_nIdxJijumStation[nJijum], dArr, dArrType, dArrIndex);
		if(bArch)
		{
			if(nJijum == 0 || nJijum == pBri->m_nQtyJigan)
			{
				nQtyPos = GetArrayHDanPosByStationIndex(m_nIdxJijumStation[nJijum], dArr, dArrType, dArrIndex);
			}
			else nQtyPos = (bArch)? m_dArrHDanPos_Archrib.GetSize() : m_dArrHDanPos.GetSize();
			CDPoint vAngEnd = pBri->GetAngleJijum(pBri->m_nQtyJigan);
//			double dStaOff = 0;
// 			if(nJijum==0 || nJijum==pBri->m_nQtyJigan)
// 			{
// 				double dStaOff = ((nJijum==0)? -pBri->m_dWS/vAngStt.y : pBri->m_dWE/vAngEnd.y)/2;
// 			}
		}
		double dHeight = bLsd? pBri->GetHeightModelJijum(nJijum) : toM(pBri->GetHeightModelJijum(nJijum));

		if(bColumn) 
		{
			long nQtyColumn = m_dArrPointJijum[iUPPER][nJijum].GetSize();
			long j=0; for(j=0; j<nQtyColumn; j++)
			{
				long nIdxSlab = (long)m_dArrPointJijum[iUPPER][nJijum].GetAt(j);
				vVec = m_vArrSlab[iUPPER].GetAt(nIdxSlab);
				vVec.z -= dHeight;
				vVec.x -= fabs(vVec.z)*cosVal;
				m_vArrWall[nJijum].Add(vVec);
				if(bArch == FALSE && pBri->IsBoxType() == FALSE) 
				{
					m_dArrPointWallFix[nJijum].Add(j); // 벽체하단 고정점 저장
				}
			}
			continue;
		}

		long nQtyDivide = bLsd? (long)Round(dHeight/1000,0) : (long)Round(dHeight,0);
		
		double dUnitHeight = dHeight/(double)nQtyDivide;
		// 횡단 높이차 때문에 동일한 높이로 분할 했을때 문제가 발생한다. dUnitHeight를 각 횡단 위치마다 따로 구해준다.
		std::vector<double> vUnitHeight;
		long nBasePos = nQtyPos / 2;
		long nIdxSlab = (long)m_dArrPointJijum[iUPPER][nJijum].GetAt(nBasePos);
		double dBaseHeight = m_vArrSlab[iUPPER].GetAt(nIdxSlab).z;

		for(long j=0; j<nQtyPos; j++)
		{
			nIdxSlab = (long)m_dArrPointJijum[iUPPER][nJijum].GetAt(j);
			vVec = m_vArrSlab[iUPPER].GetAt(nIdxSlab);
			vUnitHeight.push_back(dUnitHeight + (vVec.z - dBaseHeight)/(double)nQtyDivide);
		}

		long i=0; 
		for(i=0; i<nQtyDivide; i++)
		{
			// 슬래브 접합면 다음부터 생성
			if(i==0)
			{
				for(long j=0; j<nQtyPos; j++)
				{
					nIdxSlab = (long)m_dArrPointJijum[iUPPER][nJijum].GetAt(j);
					vVec = m_vArrSlab[iUPPER].GetAt(nIdxSlab);
					vVec.z -= bLsd? (pBri->m_dTS/2) : toM(pBri->m_dTS/2);
					vVec.x -= fabs(vVec.z)*cosVal;
					m_vArrWall[nJijum].Add(vVec);
				}
			}
			for(long j=0; j < nQtyPos; j++)
			{
				nIdxSlab = (long)m_dArrPointJijum[iUPPER][nJijum].GetAt(j);
				vVec = m_vArrSlab[iUPPER].GetAt(nIdxSlab);
				vVec.z -= (i+1)*vUnitHeight[j];
				if(i == nQtyDivide-1 && pBri->IsBoxType())
				{
					vVec.z += bLsd? (pBri->m_dTS_Lower/2) : toM(pBri->m_dTS_Lower/2);
				}
				vVec.x -= fabs(vVec.z)*cosVal;
				m_vArrWall[nJijum].Add(vVec);
			}
		}
		nQtyDivide++;

		// 벽체면 생성
		long nIndexFace  = m_pListMeshSlab[iUPPER].GetSize() + m_pListMeshSlab[iLOWER].GetSize() + m_pListMeshArchRib[0].GetSize();
		long nIndexSlab	 = nSizeUpperJoint + m_vArrSlab[iLOWER].GetSize();
		long nIndexPrev  = nIndexSlab + m_vArrArchRib.GetSize();
		long nIndexJoint = nSizeUpperJoint + m_vArrSlab[iLOWER].GetSize(); // 현재 위치의 벽체
		for(i=0; i<nJijum; i++)
		{
			nIndexFace += m_pListMeshWall[i].GetSize();
			nIndexPrev += m_vArrWall[i].GetSize();
		}
		for(i=0; i<nQtyDivide; i++)
		{
			// 슬래브 접합면 다음부터 생성
			long j=0; for(j=0; j<nQtyPos-1; j++)
			{
				CMeshFace *pFace = new CMeshFace;

				pFace->m_nIndexFace = nIndexFace++;

				if(i==0)
				{
					long nIdxSlab = (long)m_dArrPointJijum[iUPPER][nJijum].GetAt(j);
					pFace->m_nIndexJoint[0] = nIdxSlab;		// 좌측상단
					pFace->m_nIndexJoint[1] = nIdxSlab+1;	// 우측상단
					pFace->m_vPoint[0] = m_vArrSlab[iUPPER].GetAt(pFace->m_nIndexJoint[0]);
					pFace->m_vPoint[1] = m_vArrSlab[iUPPER].GetAt(pFace->m_nIndexJoint[1]);
				}
				else
				{
					nIndexJoint = (i-1)*nQtyPos + j;
					pFace->m_nIndexJoint[0] = nIndexPrev + nIndexJoint;
					pFace->m_vPoint[0] = m_vArrWall[nJijum].GetAt(nIndexJoint);
					nIndexJoint = (i-1)*nQtyPos + j + 1;
					pFace->m_nIndexJoint[1] = nIndexPrev + nIndexJoint;
					pFace->m_vPoint[1] = m_vArrWall[nJijum].GetAt(nIndexJoint);
				}
				// 아치리브 접합부 일경우...
				if(nJijum > 0 && nJijum < pBri->m_nQtyJigan && bArch && i==nQtyDivide-1)
				{
					long nIdxArchRib = (long)m_dArrPointJijumArch[nJijum].GetAt(j);
					pFace->m_nIndexJoint[3] = nIdxArchRib;		// 좌측하단
					pFace->m_nIndexJoint[2] = nIdxArchRib+1;	// 우측하단
					pFace->m_vPoint[3] = m_vArrArchRib.GetAt(pFace->m_nIndexJoint[3] - nIndexSlab);
					pFace->m_vPoint[2] = m_vArrArchRib.GetAt(pFace->m_nIndexJoint[2] - nIndexSlab);
				}
				else
				{
					nIndexJoint = i*nQtyPos + j;
					pFace->m_nIndexJoint[3] = nIndexPrev + nIndexJoint;
					pFace->m_vPoint[3] = m_vArrWall[nJijum].GetAt(nIndexJoint);
					if(i==nQtyDivide-1 && j==0 && pBri->IsBoxType() == FALSE) 
					{
						m_dArrPointWallFix[nJijum].Add(nIndexJoint); // 벽체하단 고정점 저장
					}
					
					nIndexJoint = i*nQtyPos + j + 1;
					pFace->m_nIndexJoint[2] = nIndexPrev + nIndexJoint;
					pFace->m_vPoint[2] = m_vArrWall[nJijum].GetAt(nIndexJoint);
					if(i==nQtyDivide-1 && pBri->IsBoxType() == FALSE)
					{
						m_dArrPointWallFix[nJijum].Add(nIndexJoint); // 벽체하단 고정점 저장
					}
				}
	
				m_pListMeshWall[nJijum].Add(pFace);

				if(i == nQtyDivide - 1 && pBri->IsBoxType())
				{
					long nIdxSlab = (long)m_dArrPointJijum[iLOWER][nJijum].GetAt(j);
					CMeshFace *pFaceLower = new CMeshFace;

					pFaceLower->m_nIndexFace = nIndexFace++;
					pFaceLower->m_nIndexJoint[0] = pFace->m_nIndexJoint[3];
					pFaceLower->m_nIndexJoint[1] = pFace->m_nIndexJoint[2];
					pFaceLower->m_nIndexJoint[3] = nIdxSlab;
					pFaceLower->m_nIndexJoint[2] = nIdxSlab + 1;
					m_pListMeshWall[nJijum].Add(pFaceLower);
				}
			}
		}
	}
}

// 지점내에서의 nIndexJoint
BOOL CARcBridgeMeshCalc::IsWallFixPoint(long nJijum, long nIndexJoint)
{
	long nSize  = m_dArrPointWallFix[nJijum].GetSize();
	//long nJoint = 0;
	long i=0; for(i=0; i<nSize; i++)
	{
		long nJoint = (long)m_dArrPointWallFix[nJijum].GetAt(i);
		if(nJoint==nIndexJoint)
		{
			return TRUE;
		}
	}

	return FALSE;
}

// 해당 절점이 교량받침위치인지를 판단하고 지점번호,받침번호,전.후열을 구한다...
BOOL CARcBridgeMeshCalc::IsShoePoint(long nIndexJoint, long &nJijum, long &nShoe, long &nVerPos)
{
	long nQtyJijum = m_pStd->m_pBri->GetCountJijum();
	long nJ=0; for(nJ=0; nJ<nQtyJijum; nJ++)
	{
		long nQtyPrev = m_dArrJointShoeoPos[0][nJ].GetSize();
		long nQtyNext = m_dArrJointShoeoPos[1][nJ].GetSize();
		long i=0; for(i=0; i<nQtyPrev; i++)
		{
			long nJoint = (long)m_dArrJointShoeoPos[0][nJ].GetAt(i);
			if(nJoint==nIndexJoint)
			{
				nJijum  = nJ;
				nShoe   = i;
				nVerPos = 0;

				return TRUE;
			}
		}
		for(i=0; i<nQtyNext; i++)
		{
			long nJoint = (long)m_dArrJointShoeoPos[1][nJ].GetAt(i);
			if(nJoint==nIndexJoint)
			{
				nJijum  = nJ;
				nShoe   = i;
				nVerPos = 1;

				return TRUE;
			}
		}
	}

	return FALSE;
}


// 재하되는 차선의 구간 개수 (차선수아님, 2차선+중분대+2차선 => 2개)
long CARcBridgeMeshCalc::GetSizeLaneGroup()
{
	CRcBridgeRebar *pBri  = m_pStd->m_pBri;
	long nLaneGroup = 0;

	long nTypeHDan = -1;
	long nTypePrev = -1;
	long nSizeHDanType = m_dArrHDanType.GetSize();
	if(pBri->m_pARcBridgeDataStd->IsRailDesign())
	{
		long i=0; for(i=0; i<nSizeHDanType; i++)
		{
			nTypeHDan = (long)m_dArrHDanType.GetAt(i);
			if(nTypePrev!=HDAN_TYPE_RAIL_DOSANGGRAVEL && nTypeHDan==HDAN_TYPE_RAIL_DOSANGGRAVEL)
				nLaneGroup++;
			else if(nTypePrev!=HDAN_TYPE_RAIL_DOSANGVER && nTypeHDan==HDAN_TYPE_RAIL_DOSANGVER)
				nLaneGroup++;
			nTypePrev = nTypeHDan;
		}
	}
	else
	{
		long i=0; for(i=0; i<nSizeHDanType; i++)
		{
			nTypeHDan = (long)m_dArrHDanType.GetAt(i);
			if(nTypePrev!=HDAN_TYPE_ROAD_NONE && nTypeHDan==HDAN_TYPE_ROAD_NONE)
				nLaneGroup++;
			nTypePrev = nTypeHDan;
		}
	}

	return nLaneGroup;
}

// LANE GROUP의 시작위치점
long CARcBridgeMeshCalc::GetPosLaneGroupStt(long nLaneGroup)
{
	CRcBridgeRebar *pBri  = m_pStd->m_pBri;
	long nCountGroup = 0;

	long nTypeHDan = -1;
	long nTypePrev = -1;
	long nSizeHDanType = m_dArrHDanType.GetSize();
	
	if(pBri->m_pARcBridgeDataStd->IsRailDesign())
	{
		long i=0; for(i=0; i<nSizeHDanType; i++)
		{
			nTypeHDan = (long)m_dArrHDanType.GetAt(i);
			if(nTypePrev!=HDAN_TYPE_RAIL_DOSANGGRAVEL && nTypeHDan==HDAN_TYPE_RAIL_DOSANGGRAVEL)
			{
				if(nLaneGroup==nCountGroup)
					return i;
				else
					nCountGroup++;
			}
			else if(nTypePrev!=HDAN_TYPE_RAIL_DOSANGVER && nTypeHDan==HDAN_TYPE_RAIL_DOSANGVER)
			{
				if(nLaneGroup==nCountGroup)
					return i;
				else
					nCountGroup++;
			}
			nTypePrev = nTypeHDan;
		}
	}
	else
	{
		long i=0; for(i=0; i<nSizeHDanType; i++)
		{
			nTypeHDan = (long)m_dArrHDanType.GetAt(i);
			if(nTypePrev!=HDAN_TYPE_ROAD_NONE && nTypeHDan==HDAN_TYPE_ROAD_NONE)
			{
				if(nLaneGroup==nCountGroup)
					return i;
				else
					nCountGroup++;
			}
			nTypePrev = nTypeHDan;
		}
	}

	return -1;
}

// double CARcBridgeMeshCalc::GetAngleBridgeDirPos(long nIndex)
// {
// 	CRcBridgeRebar *pBri  = m_pStd->m_pBri;
// 
// 	long nJijum = 0;
// 	long nSizeSta = m_dArrStation.GetSize();
// 	long i=0; for(i=0; i<nSizeSta; i++)
// 	{
// 		long j=0; for(j=0; j<MAX_JIJUM_COUNT_RC-1; j++)
// 		{
// 			if(i>=m_nIdxJijumStation[j] && i<m_nIdxJijumStation[j+1])
// 			{
// 				nJijum = j;
// 				break;
// 			}
// 		}
// 	}
// 
// 	double dAngDeg = pBri->GetAngleJijum(nJijum).GetAngleDegree();
// 
// 	return dAngDeg;
// }

long CARcBridgeMeshCalc::GetCountLane(long nGroup)
{
	HGDOUBLE dWidth = GetWidthLane(nGroup);
	
	long nCountLane = m_pStd->GetCountLane(dWidth);

	return nCountLane;
}

long CARcBridgeMeshCalc::GetCountLaneForRailLoad(long nGroup, long &nStt, long&nEnd)
{
	double dDistStt = 0;
	double dDistEnd = 0;
	if (GetDistHDan(nGroup, TRUE, dDistStt, dDistEnd) == FALSE)
		return -1;
	
	CRcBridgeRebar *pBri  = m_pStd->m_pBri;
	BOOL bFirst = TRUE;
	long nCountLane = 0;
		
	for(HGINT32 i=0; i<pBri->GetQtyRailNode(); i++)
	{
		double dDistRail = pBri->GetLengthRailByLine(i);
		if(dDistRail > dDistStt && dDistRail < dDistEnd)
		{
			if(bFirst) nStt = i;
			nEnd = i;
			bFirst = FALSE;
			nCountLane++;
		}
	}

	return nCountLane;
}

HGBOOL CARcBridgeMeshCalc::GetDistHDan(long nGroup, HGBOOL bRail, double &dDistStt, double &dDistEnd)
{
	long nIndexStt = GetPosLaneGroupStt(nGroup);

	if(nIndexStt==-1) return FALSE;

	//long nTypeHDan = -1;
	long nSizeHDanType = m_dArrHDanType.GetSize();
	long i=0; for(i=nIndexStt; i<nSizeHDanType; i++)
	{
		long nTypeHDan = (long)m_dArrHDanType.GetAt(i);
		if (bRail)
		{
			if(nTypeHDan!=HDAN_TYPE_RAIL_DOSANGVER && nTypeHDan!=HDAN_TYPE_RAIL_DOSANGGRAVEL)
			{
				break;
			}
		}
		else
		{
			if(nTypeHDan!=HDAN_TYPE_ROAD_NONE)
			{
				break;
			}
		}
	}

	dDistStt = m_dArrHDanPos.GetAt(nIndexStt);
	dDistEnd = m_dArrHDanPos.GetAt(i);

	return TRUE;
}

HGDOUBLE CARcBridgeMeshCalc::GetWidthLane(long nGroup, HGBOOL bRail)
{
	double dDistStt = 0;
	double dDistEnd = 0;
	if (GetDistHDan(nGroup, bRail, dDistStt, dDistEnd) == FALSE)
		return -1;
	
	CRcBridgeRebar *pBri  = m_pStd->m_pBri;
	double dWidth   = dDistEnd - dDistStt;
	double dWidthSlabEnd = pBri->GetWidthSlabAct(pBri->m_dStationBridgeStt + pBri->m_dLengthBridge, CDPoint(0, 1));
	
	return MIN(dWidth, dWidthSlabEnd);
}

double CARcBridgeMeshCalc::GetWidthLane(long nGroup)
{
	double dWidth = GetWidthLane(nGroup, FALSE);

	return dWidth;
}

double CARcBridgeMeshCalc::GetWidthLaneForRailLoad(long nGroup)
{
	double dWidth = GetWidthLane(nGroup, TRUE);

	return dWidth;
}

void CARcBridgeMeshCalc::GetPointArrayLaneGroupStt(long nGroup, CDoubleArray &dArrPoint)
{
	dArrPoint.RemoveAll();

	long nIdxHDan   = GetPosLaneGroupStt(nGroup);
	long nSizeMesh  = m_pListMeshSlab[iUPPER].GetSize();
	long nSizePoint = 0;

	long i=0; for(i=0; i<nSizeMesh; i++)
	{
		CMeshFace *pFace = m_pListMeshSlab[iUPPER].GetAt(i);
		if(pFace->m_nIndexPosStd == nIdxHDan)
		{
			nSizePoint = dArrPoint.GetSize();
			long j=0; for(j=0; j<nSizePoint; j++)
			{
				long nPoint = (long)dArrPoint.GetAt(j);
				if(nPoint==pFace->m_nIndexJoint[0])
					break;
			}
			if(j==nSizePoint)
				dArrPoint.Add(pFace->m_nIndexJoint[0]);
			//
			nSizePoint = dArrPoint.GetSize();
			for(j=0; j<nSizePoint; j++)
			{
				long nPoint = (long)dArrPoint.GetAt(j);
				if(nPoint==pFace->m_nIndexJoint[1])
					break;
			}
			if(j==nSizePoint)
				dArrPoint.Add(pFace->m_nIndexJoint[1]);
		}
	}
}

long CARcBridgeMeshCalc::GetJointJijumFixStt(long nJijum)
{
	CRcBridgeRebar *pBri  = m_pStd->m_pBri;
	long nJointPrev = m_vArrSlab[iUPPER].GetSize();
	long nJointRet = 0;

	if(pBri->m_nTypeBridge==BRIDGE_TYPE_ARCH)
	{
		if(nJijum==0)
		{
			nJointPrev += m_vArrArchRib.GetSize();
			nJointRet =  nJointPrev + (long)m_dArrPointWallFix[0].GetAt(0) + 1;
		}
		else if(nJijum==3)
		{
			nJointPrev += m_vArrArchRib.GetSize();
			long i=0; for(i=0; i<pBri->m_nQtyJigan; i++)
			{
				nJointPrev += m_vArrWall[i].GetSize();
			}
			nJointRet =  nJointPrev + (long)m_dArrPointWallFix[pBri->m_nQtyJigan].GetAt(0) + 1;
		}
		else
		{
			nJointRet =  nJointPrev + (long)m_dArrPointArchRibFix[nJijum-1].GetAt(0) + 1;
		}
	}
	else
	{
		long i=0; for(i=0; i<nJijum; i++)
		{
			nJointPrev += m_vArrWall[i].GetSize();
		}
		
		nJointRet =  nJointPrev + (long)m_dArrPointWallFix[nJijum].GetAt(0) + 1;
	}
	return nJointRet;
}

long CARcBridgeMeshCalc::GetJointJijumFixEnd(long nJijum)
{
	CRcBridgeRebar *pBri  = m_pStd->m_pBri;
	long nJointPrev = m_vArrSlab[iUPPER].GetSize();
	long nJointRet = 0;

	if(pBri->m_nTypeBridge==BRIDGE_TYPE_ARCH)
	{
		if(nJijum==0)
		{
			nJointPrev += m_vArrArchRib.GetSize();
			nJointRet =  nJointPrev + (long)m_dArrPointWallFix[0].GetAt(m_dArrPointWallFix[0].GetSize()-1) + 1;
		}
		else if(nJijum==3)
		{
			nJointPrev += m_vArrArchRib.GetSize();
			long i=0; for(i=0; i<pBri->m_nQtyJigan; i++)
			{
				nJointPrev += m_vArrWall[i].GetSize();
			}
			nJointRet =  nJointPrev + (long)m_dArrPointWallFix[pBri->m_nQtyJigan].GetAt(m_dArrPointWallFix[pBri->m_nQtyJigan].GetSize()-1) + 1;
		}
		else
		{
			nJointRet =  nJointPrev + (long)m_dArrPointArchRibFix[nJijum-1].GetAt(m_dArrPointArchRibFix[nJijum-1].GetSize()-1) + 1;
		}
	}
	else
	{
		long i=0; for(i=0; i<nJijum; i++)
		{
			nJointPrev += m_vArrWall[i].GetSize();
		}
		
		nJointRet = nJointPrev + (long)m_dArrPointWallFix[nJijum].GetAt(m_dArrPointWallFix[nJijum].GetSize()-1) + 1;
	}
	return nJointRet;
}


long CARcBridgeMeshCalc::GetCountHDanPos()
{
	return m_dArrHDanPos.GetSize();
}

BOOL CARcBridgeMeshCalc::IsShoePos(double dHPos, long nJijum, BOOL bSttShoe)
{
	BOOL bShoePos = FALSE;

	CJijum *pJ = m_pStd->m_pBri->GetJijum(nJijum);

	long nV = bSttShoe ? 0 : 1;
	long nQtyShoe = pJ->m_nQtyShoe[nV];

	long i=0; for(i=0; i<nQtyShoe; i++)
	{
		double dDistShoe = pJ->m_dDistShoe[nV][i];
		if(Compare(dHPos, dDistShoe,"=", MIN_LEN_SHELL)) 
			return TRUE;
	}

	return bShoePos;
}

BOOL CARcBridgeMeshCalc::IsExistValueInArray(CDoubleArray dArr, double dValue)
{
	//double dArrValue = 0;

	long nSizeArr = dArr.GetSize();
	long i=0; for(i=0; i<nSizeArr; i++)
	{
		double dArrValue = dArr.GetAt(i);
		if(Compare(dArrValue,dValue,"=", MIN_LEN_SHELL)) return TRUE;
	}

	return FALSE;
}

long CARcBridgeMeshCalc::GetNumberSlabSpan(double dStation)
{
// 	double dStationStt = 0;
// 	double dStationEnd = 0;

	long i=0; for(i=0; i<m_pStd->m_pBri->m_nQtyJigan; i++)
	{
		double dStationStt = m_pStd->m_pBri->GetStationOnJijum(i);
		double dStationEnd = m_pStd->m_pBri->GetStationOnJijum(i+1);

		if(dStation>dStationStt && dStation<dStationEnd) return i;
		// 시종점은 교량 Station 범위를 넘어갈수도 있다.
		if(i==0 && dStation <= dStationStt)
			return i;
		if(i==m_pStd->m_pBri->m_nQtyJigan-1 && dStation > dStationEnd)
			return i;
	}

	return -1;
}

// dStation1이 지간의 좌측 검토위치인 경우에 TRUE
BOOL CARcBridgeMeshCalc::IsSpanCheckStt(double dStation1, double dStation2)
{
	long nQtyJijum = m_pStd->m_pBri->GetCountJijum();
	double dStaChcek  = 0;

	long nJijum=0; for(nJijum=0; nJijum<nQtyJijum; nJijum++)
	{
		if(nJijum==nQtyJijum-1) continue;

		dStaChcek  = m_pStd->m_pBri->GetStationOnJijum(nJijum);
		if(nJijum==0 && (m_pStd->m_pBri->IsRcSlab() || m_pStd->m_pBri->IsPiRahmen()))
		{
			CJijum *pJ = m_pStd->m_pBri->GetJijum(nJijum);
			dStaChcek += (pJ->m_dLengthUgan[1]+pJ->m_dLengthSlabToShoe[1]);
		}
		if(Compare(dStaChcek,dStation1,"=", MIN_LEN_SHELL)) return TRUE;
		// 시종점은 지점 Station밖에도 부재가 있다.
		if(nJijum == 0 && Compare(dStaChcek,dStation1,">"))
			 return TRUE;
	}

	return FALSE;
}

// dStation2가 지간의 우측 검토위치인 경우에 TRUE
BOOL CARcBridgeMeshCalc::IsSpanCheckEnd(double dStation1, double dStation2)
{
	long nQtyJijum = m_pStd->m_pBri->GetCountJijum();
	double dStaChcek  = 0;

	long nJijum=0; for(nJijum=0; nJijum<nQtyJijum; nJijum++)
	{
		if(nJijum==0) continue;

		dStaChcek = m_pStd->m_pBri->GetStationOnJijum(nJijum);
		if(nJijum==nQtyJijum-1 && (m_pStd->m_pBri->IsRcSlab() || m_pStd->m_pBri->IsPiRahmen()))
		{
			CJijum *pJ = m_pStd->m_pBri->GetJijum(nJijum);
			dStaChcek -= (pJ->m_dLengthUgan[0]+pJ->m_dLengthSlabToShoe[0]);
		}
		if(Compare(dStaChcek,dStation2,"=", MIN_LEN_SHELL)) return TRUE;

		// 시종점은 지점 Station밖에도 부재가 있다.
		if(nJijum == nQtyJijum-1 && Compare(dStaChcek,dStation2,"<"))
			return TRUE;
	}

	return FALSE;
}


void CARcBridgeMeshCalc::MakeStationAndHDanPosTypeArch()
{
	m_dArrStation.RemoveAll();
	m_dArrHDanPos.RemoveAll();
	m_dArrStation_Archrib.RemoveAll();
	m_dArrHDanPos_Archrib.RemoveAll();
	m_dArrHDanType.RemoveAll();
	m_dArrHDanIndex.RemoveAll();
	m_dArrHDanType_Archrib.RemoveAll();
	m_dArrHDanIndex_Archrib.RemoveAll();
	m_dArrThick_Archrib.RemoveAll();

	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	HGDOUBLE dW = 0;
	HGDOUBLE dStaJijum = AddStationHDan(TRUE, &dW);

	BOOL bLsd = pBri->m_pARcBridgeDataStd->IsLsdDesign();
	AddHDanInfo(dStaJijum);

	// 가각이 있을때 가각에 대한 부분생성
	if(pBri->m_bIsExp)
	{
		AddStationForGagak();
		AddHDanPosForGagak();
	}
	else
	{
		m_nIdxGagakStation[0][0] = 0;
		m_nIdxGagakStation[0][1] = 0;
		m_nIdxGagakStation[1][0] = m_dArrStation.GetSize()-1;
		m_nIdxGagakStation[1][1] = m_dArrStation.GetSize()-1;
	}
	
	AddShoeAndColPos();
	// ArchRib에대한 위치....
	double dStaRibStt = pBri->GetStationRibBySpan(0, CHECK_POS_LEFT);
	double dStaRibEnd = pBri->GetStationRibBySpan(pBri->m_nQtyJigan-1, CHECK_POS_RIGHT);
	double dWidthRibLeft = pBri->GetWidthArchRib(-1);
	double dWidthRibRight = pBri->GetWidthArchRib(1);
	
	long nSizePos = m_dArrHDanPos.GetSize();
				
	long n=0; for(n=0; n<2; n++)
	{
		double dDistAdd = (n==0)? -Round(dWidthRibLeft,3) : Round(dWidthRibRight,3);
		for(long nH=0; nH<nSizePos-1; nH++)
		{
			double HPosCur  = m_dArrHDanPos.GetAt(nH);
			double HPosNext = m_dArrHDanPos.GetAt(nH+1);
			
			if(dDistAdd>HPosCur && dDistAdd<HPosNext)
			{
				long nType = (long)m_dArrHDanType.GetAt(nH);
				long nNode = (long)m_dArrHDanIndex.GetAt(nH);
				m_dArrHDanPos.InsertAt(nH+1,Round(dDistAdd,3));
				m_dArrHDanType.InsertAt(nH+1,nType);
				m_dArrHDanIndex.InsertAt(nH+1,nNode);
				break;
			}
		}
	}
	
	// 종방향
	double dSta = 0;
	dSta = dStaRibStt;
	m_dArrStation_Archrib.Add(dSta);
	long nTemp = m_dArrStation.GetSize();
	
	long i=0; for(i=0; i<nTemp; i++)
	{
		dSta = m_dArrStation.GetAt(i);
		if(dSta < dStaRibStt) continue;
		else if(dSta > dStaRibEnd) break;

		m_dArrStation_Archrib.Add(dSta);
	}
	dSta = dStaRibEnd;
	m_dArrStation_Archrib.Add(dSta);

	// 횡방향
//	double dPos = 0;
	nTemp = m_dArrHDanPos.GetSize();
	for(i=0; i<nTemp; i++)
	{
		double dPos = m_dArrHDanPos.GetAt(i);
		if(dPos < -dWidthRibLeft) continue;
		else if(dPos > dWidthRibRight) break;
		
		m_dArrHDanPos_Archrib.Add(dPos);
		m_dArrHDanType_Archrib.Add(m_dArrHDanType.GetAt(i));
		m_dArrHDanIndex_Archrib.Add(m_dArrHDanIndex.GetAt(i));
	}

	// 아치리브 두께
	nTemp = m_dArrStation_Archrib.GetSize();
	for(i=1; i<nTemp-1; i++)
	{
		dSta = m_dArrStation_Archrib.GetAt(i);
		double dThickRib = bLsd? m_pStd->GetThickArchRib(dSta) : toM(m_pStd->GetThickArchRib(dSta));
		
		if(dThickRib>0)
		{
			long nSizeThick = m_dArrThick_Archrib.GetSize();
			long i=0; for(i=0; i<nSizeThick; i++)
			{
				double dThcick1 = m_dArrThick_Archrib.GetAt(i);
				if(Compare(dThcick1, dThickRib, "=")) break;
			}
			if(i==nSizeThick)
				m_dArrThick_Archrib.Add(dThickRib);
		}
	}
	m_dArrThick_Archrib.Sort();

	// Constraint 구간
	long nJijumConnectRib = pBri->GetJijumConnectRibAndSlab();
	dStaJijum = pBri->GetStationOnJijum(nJijumConnectRib);
	nTemp = m_dArrStation.GetSize();

	for(i=0; i<nTemp; i++)
	{
		double dStaCur = m_dArrStation.GetAt(i);

		if((dStaJijum - dW/2) == dStaCur) m_nIndexStaConstraintStt = i;
		else if((dStaJijum + dW/2) == dStaCur) m_nIndexStaConstraintEnd = i;
	}
}

void CARcBridgeMeshCalc::MakeMeshArchRib()
{
	CRcBridgeRebar *pBri  = m_pStd->m_pBri;
	CLineInfo      *pLine = pBri->GetLineBase();

	m_vArrArchRib.RemoveAll();
	m_dArrPointArchRibFix[0].RemoveAll();
	m_dArrPointArchRibFix[1].RemoveAll();
	m_dArrConstraintJointIdx_Archrib.RemoveAll();

	BOOL bLsd = pBri->m_pARcBridgeDataStd->IsLsdDesign();
	long nCntJointSlab = m_vArrSlab[iUPPER].GetSize();
	long nQtySta = m_dArrStation_Archrib.GetSize();
	long nQtyPos = m_dArrHDanPos_Archrib.GetSize();
//	long nJijum = 0;

	double dStation=0,dStationSc=0,dDist=0;
//	CVector vVec(0,0,pLine->GetElevationPlanEl(pBri->m_dStationBridgeStt));
	CVector vVec(0,0,0);
	CDPoint xy(0,0);

	long nJijumConnect = pBri->GetJijumConnectRibAndSlab();
	CDPoint vAngStt = pBri->GetAngleJijum(0);
	double dWS = pBri->m_dWS/vAngStt.y;
	double dElBase = pLine->GetElevationPlanEl(pBri->m_dStationBridgeStt - dWS/2) - (pBri->GetThickPave()+pBri->m_dTS/2);
	double dStaCrown = pBri->m_pbRibCen.m_xyCrown.x;
	
	// ARCHRIB 좌표 생성...
	CDPoint vAng(0,0);
	long i=0; for(i=0; i<nQtySta; i++)
	{
		dStation = m_dArrStation_Archrib.GetAt(i);

		long nJijum = pBri->GetJijumNumberByStation(dStation);
		vAng = CDPoint(0, 1);

		long j=0; for(j=0; j<nQtyPos; j++)
		{
			dDist = m_dArrHDanPos_Archrib.GetAt(j);
			dStationSc = pLine->GetStationByScInLine(dStation,dDist,vAng);

			//////////////////////////////////////////////////////////////////////////
			// 지점위치에 대한 저장
			BOOL bJijumPoint = FALSE;
			long k=0; for(k=0; k<pBri->GetCountJijum(); k++)
			{
				// 이부분 .. m_nIdxJijumStation[k]은 상부슬래브기준이므로 아치리브에서는 같지않다.
				long nIdxConvArchRib = GetConvertMatchIndexSlabToArchRib(m_nIdxJijumStation[k]);

				if(i==nIdxConvArchRib && i!=0)
				{
					bJijumPoint = TRUE;
					break;
				}
			}
			if(bJijumPoint)
			{
				BOOL bExistWall = TRUE;
				if(nJijum==0) bExistWall = FALSE;
				else if(nJijum==pBri->m_nQtyJigan) bExistWall = FALSE;
				else if(pBri->GetInWall(nJijum-1)->m_bIs==FALSE) bExistWall = FALSE;
				if(nJijum == nJijumConnect) bExistWall = FALSE;

				if(bExistWall)
				{
					BOOL bColumn = (pBri->GetTypeColumnByJijum(nJijum)!=-1) ? TRUE : FALSE;
					if(bColumn)
					{
						// 기둥위치인 점만 저장...
						CWallApp *pWall = pBri->GetInWall(nJijum-1);

//						double dColumnDist = pBri->GetWidthSlabFrLineLeftOrRight(dStation, CDPoint(0,1), TRUE);
						CDPoint vAngJijum = pBri->GetAngleJijum(nJijum);
						double dColumnDist = pBri->GetDisSlabLeftAct(dStation, vAngJijum)*vAngJijum.y; 
						for(long nCol=0; nCol<pWall->m_nCountColumn; nCol++)
						{
							dColumnDist += pWall->m_dDistColumn[nCol]*vAngJijum.y;
							if(Compare(Round(dColumnDist, 0), Round(dDist, 0),"="))
							{
								m_dArrPointJijumArch[nJijum].Add(m_vArrArchRib.GetSize());
								break;
							}
						}
					}
					else
					{
						// 지점부의 슬래브 포인트를 저장 => 벽체와 연결
						m_dArrPointJijumArch[nJijum].Add(m_vArrArchRib.GetSize() + nCntJointSlab);
					}
				}
			}
			
			//////////////////////////////////////////////////////////////////////////
			xy = pLine->GetXyLineDis(dStationSc,dDist);
			xy = GetXyRotateVector(m_xyCenter,xy,pBri->GetXyDirRotateDomByBridgeSttEndXy());
			if(bLsd)
			{
				vVec.x = xy.x - m_xyCenter.x;
				vVec.y = xy.y - m_xyCenter.y;
				vVec.z = pBri->m_pbRibCen.GetXyByDist(fabs(dStationSc-dStaCrown)).y - dElBase;
			}
			else
			{
				vVec.x = toM(xy.x - m_xyCenter.x);
				vVec.y = toM(xy.y - m_xyCenter.y);
				vVec.z = toM(pBri->m_pbRibCen.GetXyByDist(fabs(dStationSc-dStaCrown)).y - dElBase);
			}
			
			m_vArrArchRib.Add(vVec);
		}
	}

	// ARCHRIB MESH 생성...
	// m_pListMeshArchRib[0]에 저장.. 분리형 슬래브인경우에만 1 ~ 사용
	long nIndexFace  = m_pListMeshSlab[iUPPER].GetSize();
	double dStation1=0,dStation2=0;
	for(i=0; i<nQtySta-1; i++)
	{
		long j=0; for(j=0; j<nQtyPos-1; j++)
		{
			CMeshFace *pFace = new CMeshFace;

			pFace->m_nIndexFace = nIndexFace++;
			pFace->m_nTypeHDan  = (long)m_dArrHDanType_Archrib.GetAt(j);
			// 아치리브에서는 pFace->m_nIndexHDan이 필요없기에 단면두께인덱스를 저장.
			//pFace->m_nIndexHDan = (long)m_dArrHDanIndex_Archrib.GetAt(j);
			pFace->m_nIndexPos  = j;
			pFace->m_nIndexPosStd  = j;

			pFace->m_nIndexJoint[0] = nCntJointSlab + i*nQtyPos + j;		// 좌측상단
			pFace->m_nIndexJoint[1] = nCntJointSlab + (i+1)*nQtyPos + j;	// 우측상단
			pFace->m_nIndexJoint[2] = nCntJointSlab + i*nQtyPos + j + 1;	// 좌측상단
			pFace->m_nIndexJoint[3] = nCntJointSlab + (i+1)*nQtyPos + j + 1;// 우측상단

			pFace->m_vPoint[0] = m_vArrArchRib.GetAt(pFace->m_nIndexJoint[0] - nCntJointSlab);
			pFace->m_vPoint[1] = m_vArrArchRib.GetAt(pFace->m_nIndexJoint[1] - nCntJointSlab);
			pFace->m_vPoint[2] = m_vArrArchRib.GetAt(pFace->m_nIndexJoint[2] - nCntJointSlab);
			pFace->m_vPoint[3] = m_vArrArchRib.GetAt(pFace->m_nIndexJoint[3] - nCntJointSlab);

			dStation1 = m_dArrStation_Archrib.GetAt(i);
			dStation2 = m_dArrStation_Archrib.GetAt(i+1);
			dStation  = (dStation1+dStation2)/2;
			pFace->m_nSpanSlab = GetNumberArchRibSpan(dStation);
			pFace->m_bSpanCheckStt = IsArchRibSpanCheckStt(dStation1,dStation2);
			pFace->m_bSpanCheckEnd = IsArchRibSpanCheckEnd(dStation1,dStation2);

			// 아치리브 단면두께 인덱스 임시 저장.
			pFace->m_nIndexHDan = GetIdxArchRibThick(dStation1, dStation2);

			if(i == 0)
			{
				if(j==0) m_dArrPointArchRibFix[0].Add(pFace->m_nIndexJoint[0] - nCntJointSlab);
				m_dArrPointArchRibFix[0].Add(pFace->m_nIndexJoint[2] - nCntJointSlab); // 아치리브 스프링깅 고정점
			}
			else if(i == nQtySta-2)
			{
				if(j==0) m_dArrPointArchRibFix[1].Add(pFace->m_nIndexJoint[1] - nCntJointSlab);
				m_dArrPointArchRibFix[1].Add(pFace->m_nIndexJoint[3] - nCntJointSlab);
			}
			
			m_pListMeshArchRib[0].Add(pFace);


		}
	}

	// Constraint되는 절점의 Index저장
	long nIndexConstStt = GetConvertMatchIndexSlabToArchRib(m_nIndexStaConstraintStt);
	long nIndexConstEnd = GetConvertMatchIndexSlabToArchRib(m_nIndexStaConstraintEnd);

	for(i=0; i<nQtySta; i++)
	{
		long j=0; for(j=0; j<nQtyPos; j++)
		{
			if(i>=nIndexConstStt && i<=nIndexConstEnd)
			{
				m_dArrConstraintJointIdx_Archrib.Add(nCntJointSlab + i*nQtyPos + j);
			}
		}
	}
}

BOOL CARcBridgeMeshCalc::IsArchRibFixPoint(long nIndexJoint)
{
	long nSize  = m_dArrPointArchRibFix[0].GetSize();
	long nJoint = 0;
	long i=0; for(i=0; i<nSize; i++)
	{
		nJoint = (long)m_dArrPointArchRibFix[0].GetAt(i);
		if(nJoint==nIndexJoint)
		{
			return TRUE;
		}
	}

	nSize  = m_dArrPointArchRibFix[1].GetSize();
	for(i=0; i<nSize; i++)
	{
		nJoint = (long)m_dArrPointArchRibFix[1].GetAt(i);
		if(nJoint==nIndexJoint)
		{
			return TRUE;
		}
	}
	
	return FALSE;
}

long CARcBridgeMeshCalc::GetConvertMatchIndexSlabToArchRib(long nIdxSlab)
{
	long nIdxArchRib = 0;

	double dStaAtIdx = m_dArrStation.GetAt(nIdxSlab);
	double dStaRibStt = m_dArrStation_Archrib.GetAt(0);
	double dStaRibEnd = m_dArrStation_Archrib.GetAt(m_dArrStation_Archrib.GetSize()-1);

	if(dStaAtIdx < dStaRibStt) nIdxArchRib = 0;
	else if(dStaAtIdx > dStaRibEnd) nIdxArchRib = m_dArrStation_Archrib.GetSize()-1;
	else
	{
		long nSize = m_dArrStation_Archrib.GetSize();
		long n=0; for(n=0; n<nSize; n++)
		{
			double dStaRibIdx = m_dArrStation_Archrib.GetAt(n);
			if(dStaRibIdx == dStaAtIdx)
			{
				nIdxArchRib = n;
				break;
			}
		}
	}

	return nIdxArchRib;
}

long CARcBridgeMeshCalc::GetIdxArchRibThick(double dXLeft, double dXRight)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	BOOL bLsd = pBri->m_pARcBridgeDataStd->IsLsdDesign();

	double dDist = 0;
	if(pBri->m_pbRibCen.m_xyCrown.x < dXLeft) dDist = MIN(pBri->m_pbRibCen.GetDistByXPos(dXLeft), pBri->m_pbRibCen.GetDistByXPos(dXRight));
	else dDist = MAX(pBri->m_pbRibCen.GetDistByXPos(dXLeft), pBri->m_pbRibCen.GetDistByXPos(dXRight));
	double dThick = bLsd? m_pStd->GetThickArchRib(pBri->m_pbRibCen.m_xyCrown.x + dDist) : toM(m_pStd->GetThickArchRib(pBri->m_pbRibCen.m_xyCrown.x + dDist));
	
	long nSize = m_pStd->m_pMeshCalc->m_dArrThick_Archrib.GetSize();
	long n=0; for(n=0; n<nSize; n++)
	{
		double dThickTemp = m_pStd->m_pMeshCalc->m_dArrThick_Archrib.GetAt(n);
		if(dThick == dThickTemp)
		{
			break;
		}
	}
	
	return n;
}

long CARcBridgeMeshCalc::GetNumberArchRibSpan(double dStation)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;

// 	double dStationStt = 0;
// 	double dStationEnd = 0;
	
	long nJijumConnect = pBri->GetJijumConnectRibAndSlab();
	CWallApp *pWall = pBri->GetInWall(nJijumConnect-1);
	double dW = pWall->m_dW; 

	long i=0; for(i=0; i<m_pStd->m_pBri->m_nQtyJigan; i++)
	{
		long nSpan = i;
		double dStationStt = m_pStd->m_pBri->GetStationOnJijum(nSpan);
		double dStationEnd = m_pStd->m_pBri->GetStationOnJijum(nSpan+1);

		if(nSpan==nJijumConnect)
		{
			double dStaTemp = 0;
			dStationStt -= dW/2;
			dStaTemp = dStationStt + dW;
			if(dStation>dStationStt && dStation<dStaTemp) return nSpan;

			dStationStt = dStaTemp;
			if(dStation>dStationStt && dStation<dStationEnd) return nSpan+1;
		}
		else if(i > nJijumConnect)
		{
			if(dStation>dStationStt && dStation<dStationEnd) return nSpan+1;
		}
		else
		{
			if(i+1 == nJijumConnect) dStationEnd -= dW/2;
			if(dStation>dStationStt && dStation<dStationEnd) return nSpan;
		}
	}
	
	return -1;
}

BOOL CARcBridgeMeshCalc::IsArchRibSpanCheckStt(double dStation1, double dStation2)
{
	long nQtyJijum = m_pStd->m_pBri->GetCountJijum();
	
	double dStaCheck  = 0;
	long nJijumConnect = m_pStd->m_pBri->GetJijumConnectRibAndSlab();
	double dTW = m_pStd->m_pBri->GetInWall(nJijumConnect-1)->m_dW;
	
	long nJijum=0; for(nJijum=0; nJijum<nQtyJijum; nJijum++)
	{
		if(nJijum==nQtyJijum-1) continue;
		
		dStaCheck  = m_pStd->m_pBri->GetStationOnJijum(nJijum);
		
		if(nJijum == nJijumConnect)
		{
			if(Compare(dStaCheck - dTW/2,dStation1,"=")) return TRUE;
			else if(Compare(dStaCheck + dTW/2,dStation1,"=")) return TRUE;
		}
		else
		{
			if(nJijum == 0)
			{
				double dStaRibStt = m_pStd->m_pBri->GetStationRibBySpan(0, CHECK_POS_LEFT);
				dStaCheck = dStaRibStt;
			}
			if(Compare(dStaCheck,dStation1,"=")) return TRUE;
		}
	}
	
	return FALSE;
}

BOOL CARcBridgeMeshCalc::IsArchRibSpanCheckEnd(double dStation1, double dStation2)
{
	long nQtyJijum = m_pStd->m_pBri->GetCountJijum();
	
	double dStaCheck  = 0;
	long nJijumConnect = m_pStd->m_pBri->GetJijumConnectRibAndSlab();
	double dTW = m_pStd->m_pBri->GetInWall(nJijumConnect-1)->m_dW;
	
	long nJijum=0; for(nJijum=0; nJijum<nQtyJijum; nJijum++)
	{
		if(nJijum==0) continue;

		dStaCheck = m_pStd->m_pBri->GetStationOnJijum(nJijum);

		if(nJijum == nJijumConnect)
		{
			if(Compare(dStaCheck - dTW/2,dStation2,"=")) return TRUE;
			else if(Compare(dStaCheck + dTW/2,dStation2,"=")) return TRUE;
		}
		else
		{
			if(nJijum == nQtyJijum-1)
			{
				double dStaRibEnd = m_pStd->m_pBri->GetStationRibBySpan(m_pStd->m_pBri->m_nQtyJigan-1, CHECK_POS_RIGHT);
				dStaCheck = dStaRibEnd;
			}
			if(Compare(dStaCheck,dStation2,"=")) return TRUE;
		}
	}
	
	return FALSE;
}

void CARcBridgeMeshCalc::AddStationForGagak()
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	double dStaAdd[4] = { -1, -1, -1, -1 };
	// 시점 좌측
	if(pBri->IsGagakForLine(TRUE, TRUE))
		dStaAdd[0] = GetStationGagakStt(TRUE, TRUE);
	// 시점 우측
	if(pBri->IsGagakForLine(TRUE, FALSE))
		dStaAdd[1] = GetStationGagakStt(TRUE, FALSE);
	// 종정 좌측
	if(pBri->IsGagakForLine(FALSE, TRUE))
		dStaAdd[2] = GetStationGagakStt(FALSE, TRUE);
	// 종점 우측
	if(pBri->IsGagakForLine(FALSE, FALSE))
		dStaAdd[3] = GetStationGagakStt(FALSE, FALSE);

	// 지점에 대한 Index에서 Station을 미리 기억해둔다.
	double dStaBuff[3][MAX_JIJUM_COUNT_RC];
	long nJ=0; for(nJ=0; nJ<=pBri->m_nQtyJigan; nJ++)
	{
		dStaBuff[0][nJ] = (m_nIdxJijumShoePos[0][nJ]>-1)? m_dArrStation.GetAt(m_nIdxJijumShoePos[0][nJ]) : -1;
		dStaBuff[1][nJ] = (m_nIdxJijumShoePos[1][nJ]>-1)? m_dArrStation.GetAt(m_nIdxJijumShoePos[1][nJ]) : -1;
		dStaBuff[2][nJ] = (m_nIdxJijumStation[nJ]>-1)? m_dArrStation.GetAt(m_nIdxJijumStation[nJ]) : -1;
	}

	long i=0; for(i=0; i<4; i++)
	{
		BOOL bAdd = TRUE;
		long nSize = m_dArrStation.GetSize();

		if(dStaAdd[i] > 0)
		{
			double dR = (i<2)? pBri->m_dExpSttR[(i%2==0)? 0:1] : dR = pBri->m_dExpEndR[(i%2==0)? 0:1];
			
			long j=0; for(j=0; j<nSize; j++)
			{
				if(dR > 0)
				{
					bAdd = FALSE;
					
					if(dStaAdd[i] > m_dArrStation.GetAt(j)) continue;
					else
					{
						dStaAdd[i] = m_dArrStation.GetAt((i<2)? j-1 : j);
						break;
					}	
				}
				else
				{
					if(Compare(dStaAdd[i], m_dArrStation.GetAt(j), "=", 100) && dStaAdd[i]>0)
					{
						dStaAdd[i] = m_dArrStation.GetAt(j);
						bAdd = FALSE;
						break;
					}
				}
			}
		}
		else bAdd = FALSE;

		if(bAdd && (IsExistValueInArray(m_dArrStation, dStaAdd[i])==FALSE)) 
			m_dArrStation.Add(dStaAdd[i]);
	}
	m_dArrStation.Sort();

	// 기억해놓은 Station으로 Index를 다시 생성
//	double dStaTemp = 0;
	long nSize = m_dArrStation.GetSize();
	for(i=0; i<nSize; i++)
	{
		double dStaTemp = m_dArrStation.GetAt(i);

		long nJ=0; for(nJ=0; nJ<=pBri->m_nQtyJigan; nJ++)
		{
			if(dStaTemp == dStaBuff[0][nJ]) m_nIdxJijumShoePos[0][nJ] = i;
			if(dStaTemp == dStaBuff[1][nJ]) m_nIdxJijumShoePos[1][nJ] = i;
			if(dStaTemp == dStaBuff[2][nJ]) m_nIdxJijumStation[nJ] = i;
		}

		if(dStaTemp == dStaAdd[0]) m_nIdxGagakStation[0][0] = i;
		if(dStaTemp == dStaAdd[1]) m_nIdxGagakStation[0][1] = i;
		if(dStaTemp == dStaAdd[2]) m_nIdxGagakStation[1][0] = i;
		if(dStaTemp == dStaAdd[3]) m_nIdxGagakStation[1][1] = i;
	}

	if(dStaAdd[0] < 0) m_nIdxGagakStation[0][0] = 0;
	if(dStaAdd[1] < 0) m_nIdxGagakStation[0][1] = 0;
	if(dStaAdd[2] < 0) m_nIdxGagakStation[1][0] = m_dArrStation.GetSize()-1;
	if(dStaAdd[3] < 0) m_nIdxGagakStation[1][1] = m_dArrStation.GetSize()-1;
}

double CARcBridgeMeshCalc::GetStationGagakStt(BOOL bStt, BOOL bLeft)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CLineInfo *pLineBase = pBri->GetLineBase();
	CLineInfo *pLineLR = pBri->GetLineLeftRight(bLeft);

//	double staGagakEnd = 0;
	double dStation = 0;

	CTwinVectorArray tvArrPlane;
	CTwinVector tv;
	pBri->GetTvPlaneOfExp(tvArrPlane, bStt, bLeft, -1);
	if(tvArrPlane.GetSize() > 0)
	{
		CDPoint xy1 = pLineBase->GetXyLine(pBri->m_dStationBridgeStt);
		CDPoint xy2 = pLineBase->GetXyLine(pBri->m_dStationBridgeStt + pBri->m_dLengthBridge);
		CDPoint vAng1 = pBri->GetAngleAzimuthJijum(0, TRUE);
		CDPoint vAng2 = pBri->GetAngleAzimuthJijum(pBri->m_nQtyJigan, TRUE);
		CDPoint xyGagakEnd(0, 0);

		if(pBri->IsExpRound(bStt, bLeft))
		{
			double dStaMax = bStt? 0 : pBri->m_dStationBridgeStt+pBri->m_dLengthBridge;
			long i=0; for(i=0; i<tvArrPlane.GetSize(); i++)
			{
				double dStaTemp = 0;
				tv = tvArrPlane.GetAt(i);
				pLineLR->GetStationMatchByXyAndAngle(tv.m_v1, vAng1, dStaTemp);
				if(bStt && dStaTemp > dStaMax)
				{
					dStaMax = dStaTemp;
					xyGagakEnd = tv.m_v1;
				}
				else if(!bStt && dStaTemp < dStaMax)
				{
					dStaMax = dStaTemp;
					xyGagakEnd = tv.m_v1;
				}
				pLineLR->GetStationMatchByXyAndAngle(tv.m_v2, vAng1, dStaTemp);
				if(bStt && dStaTemp > dStaMax)
				{
					dStaMax = dStaTemp;
					xyGagakEnd = tv.m_v2;
				}
				else if(!bStt && dStaTemp < dStaMax)
				{
					dStaMax = dStaTemp;
					xyGagakEnd = tv.m_v2;
				}
			}
		}
		else
		{
			CString str = _T("");
			str.Format("%s(%s%s가각부)", bLeft? "좌측":"우측", bStt? "시점":"종점", bLeft? "좌측":"우측");
			tv = tvArrPlane.GetTvByInfo(str);

			xyGagakEnd = (bStt==bLeft)? tv.m_v2 : tv.m_v1;
		}
		double staGagakEnd = 0;
		pLineLR->GetStationMatchByXyAndAngle(xyGagakEnd, vAng1, staGagakEnd);

		// 각도를 굳이 알필요가 없다... 바깥쪽 선형에서의 스테이션비와 중심선형에서의 스테이션비만 같으면 된다.
		// 바깥쪽 선형에서의 Sta.
		double dSta1, dSta2;
		pLineLR->GetStationMatchByXyAndAngle(xy1, vAng1, dSta1);
		pLineLR->GetStationMatchByXyAndAngle(xy2, vAng2, dSta2);
		double dStaRatio = (staGagakEnd-dSta1)/(dSta2-dSta1);

		// 중심선형에서의 Sta.를 구한다.
		dStation = pBri->m_dStationBridgeStt + (pBri->m_dLengthBridge * dStaRatio);
	}
	else dStation = -1;

	return dStation;
}

void CARcBridgeMeshCalc::AddHDanPosForGagak()
{
	m_dArrHDanPosGagak[0][0].RemoveAll();
	m_dArrHDanPosGagak[0][1].RemoveAll();
	m_dArrHDanPosGagak[1][0].RemoveAll();
	m_dArrHDanPosGagak[1][1].RemoveAll();

	long nSizeSta = m_dArrStation.GetSize();
	long i=0; for(i=0; i<nSizeSta; i++)
	{
		InsertHDanGagakByStation(i);
	}

	m_dArrHDanPosGagak[0][0].Sort();
	m_dArrHDanPosGagak[0][1].Sort();
	m_dArrHDanPosGagak[1][0].Sort();
	m_dArrHDanPosGagak[1][1].Sort();
}

long CARcBridgeMeshCalc::GetNumberJoint(long nCntSumPrev, long nCntCur, long nCntNext, long nPos, long nIdxSta, BOOL bLeft, BOOL bUpper)
{
	CRcBridgeRebar *pBri  = m_pStd->m_pBri;
	long nIdxJoint = 0;
	long nSum = 0;
	long nNode = 0;

	long nCountSta = m_dArrStation.GetSize();
	if(nIdxSta > nCountSta-2)
	{
		// 부상방지저판 추가절점
		BOOL bStt = (nIdxSta == nCountSta-1)? TRUE : FALSE;
		if(bStt)
		{
			nSum = (bLeft)? nCntSumPrev : nCntSumPrev + nCntCur;
		}
		else
		{
			nSum = (bLeft)? nCntSumPrev : nCntSumPrev + nCntCur;
		}
		nNode = (bUpper)? nPos : nPos+1;
		nIdxJoint = nSum + nNode;
	}
	else if(!pBri->m_bIsExp)
	{
		nSum = (bLeft)? nCntSumPrev : nCntSumPrev + nCntCur;
		nNode = (bUpper)? nPos : nPos+1;
		nIdxJoint = nSum + nNode;
	}
	else if(pBri->m_bIsExp)
	{
		long nIdxStaSttLeft = m_nIdxGagakStation[0][0];
		long nIdxStaSttRight = m_nIdxGagakStation[0][1];
		long nIdxStaEndLeft = m_nIdxGagakStation[1][0];
		long nIdxStaEndRight = m_nIdxGagakStation[1][1];

		// 시종점 좌우 가각이 교차될때 ㅂ재를 정확히 못만드는 문제가 있어 수정.
		if(nIdxSta >= MAX(nIdxStaSttLeft, nIdxStaSttRight) && nIdxSta < MIN(nIdxStaEndLeft, nIdxStaEndRight))
		{
			nSum = (bLeft)? nCntSumPrev : nCntSumPrev + nCntCur;
			nNode = (bUpper)? nPos : nPos+1;
			nIdxJoint = nSum + nNode;
		}
		else
		{
			nSum = (bLeft)? nCntSumPrev : nCntSumPrev + nCntCur;
			nNode = (bUpper)? nPos : nPos+1;

			

			if(nIdxSta > (pBri->m_bExpCutStt[0]? 0 : -1) && nIdxSta < nIdxStaSttLeft && !bLeft && !(nPos == 0 && bUpper)) nNode -= 1;
			if(nIdxSta < (pBri->m_bExpCutEnd[0]? m_dArrStation.GetSize()-2 : m_dArrStation.GetSize()-1) && nIdxSta >= nIdxStaEndLeft && bLeft && !(nPos == 0 && bUpper)) nNode -= 1;
			if(nIdxSta > (pBri->m_bExpCutStt[1]? 0 : -1) && nIdxSta < nIdxStaSttRight && !bLeft && (nPos==MAX(nCntCur, nCntNext)-2 && !bUpper))  nNode -= 1;
			if(nIdxSta < (pBri->m_bExpCutEnd[1]? m_dArrStation.GetSize()-2 : m_dArrStation.GetSize()-1) && nIdxSta >= nIdxStaEndRight && bLeft && (nPos==MAX(nCntCur, nCntNext)-2 && !bUpper)) nNode -= 1;

			nIdxJoint = nSum + nNode;
		}
	}

	return nIdxJoint;
}

void CARcBridgeMeshCalc::InsertHDanGagakByStation(long nIdxSta)
{
	CRcBridgeRebar *pBri  = m_pStd->m_pBri;
	double dDistPos = 0;

	if(m_nIdxGagakStation[0][0] > nIdxSta)
	{
		if(!pBri->m_bExpCutStt[0] || nIdxSta>0)
		{
			dDistPos = GetDistHDanByStationForGagak(TRUE, TRUE, nIdxSta);
			long nSize = m_dArrHDanPosGagak[0][0].GetSize();
			long n=0; for(n=0; n<nSize; n++)
			{
				if(Round(dDistPos, 0) == Round(m_dArrHDanPosGagak[0][0].GetAt(n), 0)) break;
			}
			if(n==nSize)
			{
				m_dArrHDanPosGagak[0][0].Add(dDistPos);
				m_dArrHDanPosGagak[0][0].Sort();
			}
		}
	}
	if(m_nIdxGagakStation[0][1] > nIdxSta)
	{
		if(!pBri->m_bExpCutStt[1] || nIdxSta>0)
		{
			dDistPos = GetDistHDanByStationForGagak(TRUE, FALSE, nIdxSta);
			long nSize = m_dArrHDanPosGagak[0][1].GetSize();
			long n=0; for(n=0; n<nSize; n++)
			{
				if(Round(dDistPos, 0) == Round(m_dArrHDanPosGagak[0][1].GetAt(n), 0)) break;
			}
			if(n==nSize)
			{
				m_dArrHDanPosGagak[0][1].Add(dDistPos);
				m_dArrHDanPosGagak[0][1].Sort();
			}
		}
	}
	if(m_nIdxGagakStation[1][0] < nIdxSta)
	{
		if(!pBri->m_bExpCutEnd[0] || nIdxSta<m_dArrStation.GetSize()-1)
		{
			dDistPos = GetDistHDanByStationForGagak(FALSE, TRUE, nIdxSta);
			long nSize = m_dArrHDanPosGagak[1][0].GetSize();
			long n=0; for(n=0; n<nSize; n++)
			{
				if(Round(dDistPos, 0) == Round(m_dArrHDanPosGagak[1][0].GetAt(n), 0)) break;
			}
			if(n==nSize)
			{
				m_dArrHDanPosGagak[1][0].Add(dDistPos);
				m_dArrHDanPosGagak[1][0].Sort();
			}
		}
	}
	if(m_nIdxGagakStation[1][1] < nIdxSta)
	{
		if(!pBri->m_bExpCutEnd[1] || nIdxSta<m_dArrStation.GetSize()-1)
		{
			dDistPos = GetDistHDanByStationForGagak(FALSE, FALSE, nIdxSta);
			long nSize = m_dArrHDanPosGagak[1][1].GetSize();
			long n=0; for(n=0; n<nSize; n++)
			{
				if(Round(dDistPos, 0) == Round(m_dArrHDanPosGagak[1][1].GetAt(n), 0)) break;
			}
			if(n==nSize)
			{
				m_dArrHDanPosGagak[1][1].Add(dDistPos);
				m_dArrHDanPosGagak[1][1].Sort();
			}
		}
	}
}

long CARcBridgeMeshCalc::InsertStation(double dSta, BOOL bResetIndex)
{
	CRcBridgeRebar *pBri  = m_pStd->m_pBri;

	double dStaBuff[3][MAX_JIJUM_COUNT_RC];
	double dStaGagakTemp[2][2];
	if(bResetIndex)
	{
		// 지점에 대한 Index에서 Station을 미리 기억해둔다.
		long nJ=0; for(nJ=0; nJ<=pBri->m_nQtyJigan; nJ++)
		{
			dStaBuff[0][nJ] = (m_nIdxJijumShoePos[0][nJ]>-1)? m_dArrStation.GetAt(m_nIdxJijumShoePos[0][nJ]) : -1;
			dStaBuff[1][nJ] = (m_nIdxJijumShoePos[1][nJ]>-1)? m_dArrStation.GetAt(m_nIdxJijumShoePos[1][nJ]) : -1;
			dStaBuff[2][nJ] = (m_nIdxJijumStation[nJ]>-1)? m_dArrStation.GetAt(m_nIdxJijumStation[nJ]) : -1;
		}
		dStaGagakTemp[0][0] = m_dArrStation.GetAt(m_nIdxGagakStation[0][0]);
		dStaGagakTemp[0][1] = m_dArrStation.GetAt(m_nIdxGagakStation[0][1]);
		dStaGagakTemp[1][0] = m_dArrStation.GetAt(m_nIdxGagakStation[1][0]);
		dStaGagakTemp[1][1] = m_dArrStation.GetAt(m_nIdxGagakStation[1][1]);
	}

	// Station Insert
	long nIdxAddSta = 0;
	long nSize = m_dArrStation.GetSize();
	long n=0; for(n=0; n<nSize-1; n++)
	{
		double dSta1 = m_dArrStation.GetAt(n);
		double dSta2 = m_dArrStation.GetAt(n+1);
		if(Round(dSta, 0)>Round(dSta1, 0) && Round(dSta2, 0)>Round(dSta, 0))
		{
			nIdxAddSta = n+1;
			m_dArrStation.InsertAt(n+1, dSta);
			break;
		}
		else if(Round(dSta, 0)==Round(dSta1, 0) || Round(dSta2, 0)==Round(dSta, 0))
		{
			nIdxAddSta = (dSta==dSta1)? n : n+1;
			return nIdxAddSta;
		}
	}
	
	if(bResetIndex)
	{
		// 기억해놓은 Station으로 Index를 다시 생성
// 		double dStaTemp = 0;
		long nSize = m_dArrStation.GetSize();
		long i=0; for(i=0; i<nSize; i++)
		{
			double dStaTemp = m_dArrStation.GetAt(i);
			
			long nJ=0; for(nJ=0; nJ<=pBri->m_nQtyJigan; nJ++)
			{
				if(dStaTemp == dStaBuff[0][nJ]) m_nIdxJijumShoePos[0][nJ] = i;
				if(dStaTemp == dStaBuff[1][nJ]) m_nIdxJijumShoePos[1][nJ] = i;
				if(dStaTemp == dStaBuff[2][nJ]) m_nIdxJijumStation[nJ] = i;
			}
			
			if(dStaTemp == dStaGagakTemp[0][0]) m_nIdxGagakStation[0][0] = i;
			if(dStaTemp == dStaGagakTemp[0][1]) m_nIdxGagakStation[0][1] = i;
			if(dStaTemp == dStaGagakTemp[1][0]) m_nIdxGagakStation[1][0] = i;
			if(dStaTemp == dStaGagakTemp[1][1]) m_nIdxGagakStation[1][1] = i;
		}
	}

	return nIdxAddSta;
}

double CARcBridgeMeshCalc::GetDistHDanByStationForGagak(BOOL bStt, BOOL bLeft, long nIdxSta)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CLineInfo *pLineBase = pBri->GetLineBase();
	
	CTwinVectorArray tvArrPlane;
	pBri->GetTvPlaneOfExp(tvArrPlane, bStt, bLeft, -1);
	CRebarPlacing rb;

	double dStation = m_dArrStation.GetAt(nIdxSta);
	
	CDPoint vAng = pBri->GetAngleByStationForMesh(dStation, 0, FALSE);
	CDPoint vAng2 = pBri->GetAngleByStationForMesh(dStation, 0, TRUE);
	CDPoint xyBase = pLineBase->GetXyLineDis(dStation, 0);
	CDPoint xyMatch(0, 0);
	CDPointArray xyArrMatch;
	
	// vAng은 선형 기준의 상대 각도, vAng2는 실제각도 이므로 평면 회전 옵션에 따라서 매치시킨다..XXXX
	// vAng2은 선형 기준의 상대 각도, vAng는 실제각도 이므로 평면 회전 옵션에 따라서 매치시킨다..
	rb.GetXyMatchTvArrAndLine(tvArrPlane, xyBase, vAng, xyArrMatch);
	if(xyArrMatch.GetSize() > 0)
		xyMatch	= xyArrMatch.GetAt(0);
	else
		xyMatch	= bStt? tvArrPlane.GetXyLeft() : tvArrPlane.GetXyRight();
	
	long n=0; for(n=0; n<tvArrPlane.GetSize(); n++)
	{
		CTwinVector tv = tvArrPlane.GetAt(n);
	}
	//double dDist = pLineBase->GetDisMatchVerticalByXy(xyMatch);
	// 선형이 가각부 위에 있을수 있으므로 Dist가 단순한 거리가 아니라  +,- 방향이 있어야 한다.
	BOOL bRightOfLine = IsDirectionRotateRight(xyBase, pLineBase->GetAngleAzimuth(dStation), xyMatch);
	double dDist = ~(xyMatch - pLineBase->GetXyLineDis(dStation, 0))*fabs(vAng2.y);
	return (bLeft && !bRightOfLine)? -dDist : dDist;
}

void CARcBridgeMeshCalc::AddShoeAndColPos()
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	
	double dStaJijum = 0;
	//////////////////////////////////////////////////////////////////////////
	// 기둥위치.. 추가.. (도로중심에대한 직선거리 기준)
	// 각 지점의 기둥위치를 횡단구성에 추가 -> 
	// 1) 기존 구성점과 일치하는 경우는 추가하지 않음
	// 2) 기존 횡단구성 사이에 있는 경우에 위치를 추가하고 횡단을 분할함..
	long nJijum=0; for(nJijum=1; nJijum<pBri->m_nQtyJigan; nJijum++)
	{
		BOOL bColumn = (pBri->GetTypeColumnByJijum(nJijum)!=-1) ? TRUE : FALSE;
		if(!bColumn) continue;
		CWallApp *pWall = pBri->GetInWall(nJijum-1);
		
		dStaJijum = pBri->GetStationOnJijum(nJijum);
		CDPoint vAngJijum  = pBri->GetAngleJijum(nJijum);
//		double dColumnDist = pBri->GetWidthSlabFrLineLeftOrRight(dStaJijum, CDPoint(0,1), TRUE);
		double dColumnDist = pBri->GetDisSlabLeftAct(dStaJijum, vAngJijum)*vAngJijum.y; 
		for(long nCol=0; nCol<pWall->m_nCountColumn; nCol++)
		{
			dColumnDist += pWall->m_dDistColumn[nCol]*vAngJijum.y; // m_dDistColumn[nCol] : 기둥간 실제거리
			long nSizePos = 0;
			if(pBri->IsExtLine())
			{
				CDoubleArray dArr;
				nSizePos = GetArrayHDanPosForExtLine(m_nIdxJijumStation[nJijum], dArr);
				
				for(long nH=0; nH<nSizePos-1; nH++)
				{
					double HPosCur  = dArr.GetAt(nH);
					double HPosNext = dArr.GetAt(nH+1);

					// 같은 위치가 있다면 continue;
					if(Compare(dColumnDist, HPosCur, "=", MIN_LEN_SHELL))
						break;

					if(Round(dColumnDist, 0)>Round(HPosCur, 0) && Round(dColumnDist, 0)<Round(HPosNext, 0))
					{
						long nType = (long)m_dArrHDanType.GetAt(nH);
						long nNode = (long)m_dArrHDanIndex.GetAt(nH);
						InsertHDanPosForExtLine(m_nIdxJijumStation[nJijum], dColumnDist, nH+1);
						m_dArrHDanType.InsertAt(nH+1,nType);
						m_dArrHDanIndex.InsertAt(nH+1,nNode);
						break;
					}
				}
			}
			else
			{
				nSizePos = m_dArrHDanPos.GetSize();
				for(long nH=0; nH<nSizePos-1; nH++)
				{
					double HPosCur  = m_dArrHDanPos.GetAt(nH);
					double HPosNext = m_dArrHDanPos.GetAt(nH+1);

					// 같은 위치가 있다면 continue;
					if(Compare(dColumnDist, HPosCur, "=", MIN_LEN_SHELL))
						break;

					if(Round(dColumnDist, 0)>Round(HPosCur, 0) && Round(dColumnDist, 0)<Round(HPosNext, 0))
					{
						long nType = (long)m_dArrHDanType.GetAt(nH);
						long nNode = (long)m_dArrHDanIndex.GetAt(nH);
						m_dArrHDanPos.InsertAt(nH+1,Round(dColumnDist,3));
						m_dArrHDanType.InsertAt(nH+1,nType);
						m_dArrHDanIndex.InsertAt(nH+1,nNode);
						break;
					}
				}
			}
			// 가각이 있을때 가각부분도 체크해서 추가해준다.
			if(pBri->m_bIsExp)
			{
				for(long nStt=0; nStt<2; nStt++)
				{
					for(long nLeft=0; nLeft<2; nLeft++)
					{
						if((m_nIdxJijumStation[nJijum] < m_nIdxGagakStation[nStt][nLeft] && nStt==0) || (m_nIdxJijumStation[nJijum] > m_nIdxGagakStation[nStt][nLeft] && nStt==1))
						{
							nSizePos = m_dArrHDanPosGagak[nStt][nLeft].GetSize();
							for(long nH=0; nH<nSizePos-1; nH++)
							{
								double HPosCur  = m_dArrHDanPosGagak[nStt][nLeft].GetAt(nH);
								double HPosNext = m_dArrHDanPosGagak[nStt][nLeft].GetAt(nH+1);

								// 같은 위치가 있다면 continue;
								if(Compare(dColumnDist, HPosCur, "=", MIN_LEN_SHELL))
									break;

								if(Round(dColumnDist, 0)>Round(HPosCur, 0) && Round(dColumnDist, 0)<Round(HPosNext, 0))
								{
									double dStaTemp = GetStationGagakPos(nStt==0, nLeft==0, dColumnDist);
									long nIdxInsert = InsertStation(dStaTemp, TRUE);
									InsertHDanGagakByStation(nIdxInsert);
									break;
								}
							}
						}
					}
				}
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	// 교량받침 위치에 대한 저장 : 기둥위치 추가와 동일한 방식...
	long nQtyJijum = (pBri->m_nTypeBridge == BRIDGE_TYPE_SLAB || pBri->m_nTypeBridge == BRIDGE_TYPE_PI || pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP)? pBri->GetCountJijum() : 0;
	for(nJijum=0; nJijum<nQtyJijum; nJijum++)
	{
		CJijum *pJ = pBri->GetJijum(nJijum);
		for(long nV=0; nV<2; nV++)
		{
			long nQtyShoe = pJ->m_nQtyShoe[nV];
			if(nQtyShoe==0) continue;
			for(long nShoe=0; nShoe<nQtyShoe; nShoe++)
			{
				double dDistShoe = pJ->m_dDistShoe[nV][nShoe];
				long nSizePos = 0;
				if(pBri->IsExtLine())
				{
					CDoubleArray dArr;
					long nPosStt = (m_nIdxJijumShoePos[0][nJijum] > -1)? 0 : 1;
					nSizePos = GetArrayHDanPosForExtLine(m_nIdxJijumShoePos[nPosStt][nJijum], dArr);

					for(long nH=0; nH<nSizePos-1; nH++)
					{
						double HPosCur  = dArr.GetAt(nH);
						double HPosNext = dArr.GetAt(nH+1);
						// 같은 위치가 있다면 continue;
						if(Compare(dDistShoe, HPosCur, "=", MIN_LEN_SHELL))
							break;

						if(Round(dDistShoe, 0)>Round(HPosCur, 0) && Round(dDistShoe, 0)<Round(HPosNext, 0))
						{
							long nType = (long)m_dArrHDanType.GetAt(nH);
							long nNode = (long)m_dArrHDanIndex.GetAt(nH);
							InsertHDanPosForExtLine(m_nIdxJijumShoePos[nPosStt][nJijum], dDistShoe, nH+1);
							m_dArrHDanType.InsertAt(nH+1,nType);
							m_dArrHDanIndex.InsertAt(nH+1,nNode);
							break;
						}
					}
				}
				else
				{
					nSizePos = m_dArrHDanPos.GetSize();
					for(long nH=0; nH<nSizePos-1; nH++)
					{
						double HPosCur  = m_dArrHDanPos.GetAt(nH);
						double HPosNext = m_dArrHDanPos.GetAt(nH+1);
						// 같은 위치가 있다면 continue;
						if(Compare(dDistShoe, HPosCur, "=", MIN_LEN_SHELL) || Compare(dDistShoe, HPosNext, "=", MIN_LEN_SHELL))
							break;

						if(Round(dDistShoe, 0)>Round(HPosCur, 0) && Round(dDistShoe, 0)<Round(HPosNext, 0))
						{
							long nType = (long)m_dArrHDanType.GetAt(nH);
							long nNode = (long)m_dArrHDanIndex.GetAt(nH);
							m_dArrHDanPos.InsertAt(nH+1,Round(dDistShoe,3));
							m_dArrHDanType.InsertAt(nH+1,nType);
							m_dArrHDanIndex.InsertAt(nH+1,nNode);
							break;
						}
					}
				}

				// 가각이 있을때 가각부분도 체크해서 추가해준다.
				if(pBri->m_bIsExp)
				{
					for(long nStt=0; nStt<2; nStt++)
					{
						for(long nLeft=0; nLeft<2; nLeft++)
						{
							if((m_nIdxJijumStation[nJijum] < m_nIdxGagakStation[nStt][nLeft] && nStt==0) || (m_nIdxJijumStation[nJijum] > m_nIdxGagakStation[nStt][nLeft] && nStt==1))
							{
								nSizePos = m_dArrHDanPosGagak[nStt][nLeft].GetSize();
								for(long nH=0; nH<nSizePos-1; nH++)
								{
									double HPosCur  = m_dArrHDanPosGagak[nStt][nLeft].GetAt(nH);
									double HPosNext = m_dArrHDanPosGagak[nStt][nLeft].GetAt(nH+1);
									if(Round(dDistShoe, 0)>Round(HPosCur, 0) && Round(dDistShoe, 0)<Round(HPosNext, 0))
									{
										double dStaTemp = GetStationGagakPos(nStt==0, nLeft==0, dDistShoe);
										long nIdxInsert = InsertStation(dStaTemp, TRUE);
										InsertHDanGagakByStation(nIdxInsert);
										break;
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

double CARcBridgeMeshCalc::GetStationGagakPos(BOOL bStt, BOOL bLeft, double dDist)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	CLineInfo *pLineBase = pBri->GetLineBase();
	CLineInfo *pLine = pBri->GetLineLeftRight(bLeft);
	
	double dStation = 0;
	double dStaSlab = pBri->GetStationByTarget(bStt? BRIDGE_OFF_STT_SLAB:BRIDGE_OFF_END_SLAB, bLeft? -1:1, FALSE);	

	CTwinVectorArray tvArrPlane;
	pBri->GetTvPlaneOfExp(tvArrPlane, bStt, bLeft, -1);
	CRebarPlacing rb;

	CDPoint vAng = pLine->GetAngleAzimuth(dStaSlab);
	CDPoint xyBase = pLineBase->GetXyLineDis(dStaSlab, dDist);
	CDPoint xyMatch(0, 0);
	CDPointArray xyArrMatch;
	
	rb.GetXyMatchTvArrAndLine(tvArrPlane, xyBase, vAng, xyArrMatch);
	if(xyArrMatch.GetSize() > 0)
		xyMatch	= xyArrMatch.GetAt(0);
	else
		xyMatch	= bStt? tvArrPlane.GetXyLeft() : tvArrPlane.GetXyRight();

	pLine->GetStationMatchVerticalByXy(xyMatch, dStation);

//  이건 뭐지??? 평면상의 x값을 그냥 Station으로 사용하다니... ㅡㅡ
//	vAng   = pBri->GetAngleByStationForMesh(xyMatch.x, dDist);
//	dStation = pLine->GetStationByScInLine(xyMatch.x, -dDist, vAng);
	
	return dStation;
}

long CARcBridgeMeshCalc::GetArrayHDanPosByStationIndex(long nIdxSta, CDoubleArray &dArr, CDoubleArray &dArrType, CDoubleArray &dArrIndex)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;

	dArr.RemoveAll();
	dArrType.RemoveAll();
	dArrIndex.RemoveAll();
	long nSize = 0;
	nSize = m_dArrHDanType.GetSize();
	long n=0; for(n=0; n<nSize; n++)
	{
		dArrType.Add(m_dArrHDanType.GetAt(n));
		dArrIndex.Add(m_dArrHDanIndex.GetAt(n));
	}

	if(pBri->IsExtLine())
	{
		nSize = GetArrayHDanPosForExtLine(nIdxSta, dArr);
	}
	else
	{
		nSize = m_dArrHDanPos.GetSize();
		for(n=0; n<nSize; n++)
		{
			dArr.Add(m_dArrHDanPos.GetAt(n));
		}
	}

	if(!pBri->m_bIsExp) return nSize;

	long nIdxStt = 0;
	long nIdxEnd = 0;
	long nQtySta = m_dArrStation.GetSize();
	for(long nStt=0; nStt<2; nStt++)
	{
		for(long nLeft=0; nLeft<2; nLeft++)
		{
			if(nIdxSta < m_nIdxGagakStation[nStt][nLeft] && nStt==0)
			{
				long nSizeSub = m_dArrHDanPosGagak[nStt][nLeft].GetSize();

				if(pBri->IsCutEndOfExp(TRUE, nLeft==0))
				{
					nIdxStt = (nLeft==0)? MAX(0, nIdxSta-1) : 0;
					nIdxEnd = (nLeft==0)? nSizeSub : (nSizeSub - MAX(0, nIdxSta-1));
				}
				else
				{
					nIdxStt = (nLeft==0)? nIdxSta : 0;
					nIdxEnd = (nLeft==0)? nSizeSub : (nSizeSub - nIdxSta);
				}
			
				long n=0; for(n=nIdxStt; n<nIdxEnd; n++)
				{
					dArr.Add(m_dArrHDanPosGagak[nStt][nLeft].GetAt(n));
					// HDanType와 HDanIndex에는 그냥 0을 채워 넣는다.
					if(nLeft==0)
					{
						dArrType.InsertAt(0,-1);
						dArrIndex.InsertAt(0,-1);
					}
					else
					{
						dArrType.Add(-1);
						dArrIndex.Add(-1);
					}
				}
			}
			else if(nIdxSta > m_nIdxGagakStation[nStt][nLeft] && nStt==1)
			{
				long nSizeSub = m_dArrHDanPosGagak[nStt][nLeft].GetSize();

				if(pBri->IsCutEndOfExp(FALSE, nLeft==0))
				{
					nIdxStt = (nLeft==0)? MAX(0, nQtySta - (nIdxSta+2)): 0;
					nIdxEnd = (nLeft==0)? nSizeSub: MIN(nSizeSub, nIdxSta-(m_nIdxGagakStation[nStt][nLeft]));
				}
				else
				{
					nIdxStt = (nLeft==0)? nQtySta - (nIdxSta+1): 0;
					nIdxEnd = (nLeft==0)? nSizeSub: nIdxSta-(m_nIdxGagakStation[nStt][nLeft]);
				}
								
				long n=0; for(n=nIdxStt; n<nIdxEnd; n++)
				{
					dArr.Add(m_dArrHDanPosGagak[nStt][nLeft].GetAt(n));
					// HDanType와 HDanIndex에는 그냥 0을 채워 넣는다.
					if(nLeft==0)
					{
						dArrType.InsertAt(0,-1);
						dArrIndex.InsertAt(0,-1);
					}
					else
					{
						dArrType.Add(-1);
						dArrIndex.Add(-1);
					}
				}
			}
		}
	}

	dArr.Sort();
	return dArr.GetSize();
}

// 현재 sta의 횡단구성으로 변환하여 구성된 횡단구성을 리턴
long CARcBridgeMeshCalc::GetArrayHDanPosForExtLine(long nIdxSta, CDoubleArray &dArr)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;

	CLineInfo *pLineBase = pBri->GetLineBase();
	CLineInfo *pLineL = pBri->GetLineLeftRight(TRUE);
	CLineInfo *pLineR = pBri->GetLineLeftRight(FALSE);

	double dStation = m_dArrStation.GetAt(nIdxSta);
	CDPoint vAng = pBri->GetAngleByStationForMesh(dStation);
	CDPoint xyBase = pLineBase->GetXyLine(dStation);
	CDPoint xyLineL = pLineBase->GetXyMatchLineDis(dStation, vAng, pLineL, pBri->m_dWidthSlabLeft);
	CDPoint xyLineR = pLineBase->GetXyMatchLineDis(dStation, vAng, pLineR, pBri->m_dWidthSlabRight);

	long nSize = m_dArrHDanPos.GetSize();
// 	double dDistPos = 0, dDistTemp = 0;
	double dWidthCenter  = pBri->GetWidthSlabAct(pBri->GetStationBridgeMid(), CDPoint(0, 1),0);
	double dWidthStation = ~(xyLineL-xyLineR)*vAng.y;
	double dRaitoWidth   = dWidthStation/dWidthCenter;
	double dDistLeftCen  = m_dArrHDanPos.GetAt(0);
	double dDistLeftSta  = pBri->GetDisSlabLeftAct(dStation, vAng)*vAng.y; 

	dArr.RemoveAll();
	long n=0; for(n=0; n<nSize; n++)
	{
		double dDistPos  = m_dArrHDanPos.GetAt(n);
		double dDistTemp = dDistLeftSta + (dDistPos-dDistLeftCen)*dRaitoWidth;
		dArr.Add(dDistTemp);
	}

	return nSize;
}

void CARcBridgeMeshCalc::InsertHDanPosForExtLine(long nIdxSta, double dDistPos, long nIdxInsertPos)
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;
	
	CLineInfo *pLineBase = pBri->GetLineBase();
	CLineInfo *pLineL = pBri->GetLineLeftRight(TRUE);
	CLineInfo *pLineR = pBri->GetLineLeftRight(FALSE);
	
	double dStation = m_dArrStation.GetAt(nIdxSta);
	CDPoint vAng = pBri->GetAngleByStationForMesh(dStation);
	CDPoint xyBase = pLineBase->GetXyLine(dStation);
	CDPoint xyLineL = pLineBase->GetXyMatchLineDis(dStation, vAng, pLineL, pBri->m_dWidthSlabLeft);
	CDPoint xyLineR = pLineBase->GetXyMatchLineDis(dStation, vAng, pLineR, pBri->m_dWidthSlabRight);
	
	double dWidthCenter  = pBri->GetWidthSlabAct(pBri->GetStationBridgeMid(), CDPoint(0, 1),0);
	double dWidthStation = ~(xyLineL-xyLineR)*vAng.y;
	double dRaitoWidth   = dWidthStation/dWidthCenter;
	double dDistLeftCen  = m_dArrHDanPos.GetAt(0);
	double dDistLeftSta  = pBri->GetDisSlabLeftAct(dStation, vAng)*vAng.y; 
	
	double dDistConv = dDistLeftCen + (dDistPos-dDistLeftSta)/dRaitoWidth;
	
	m_dArrHDanPos.InsertAt(nIdxInsertPos, Round(dDistConv, 3));
}

void CARcBridgeMeshCalc::MakeStationforPF()
{
	CRcBridgeRebar *pBri = m_pStd->m_pBri;

	m_dArrStation_Pf[iSTT].RemoveAll();
	m_dArrStation_Pf[iEND].RemoveAll();

	if(pBri->IsBoxType() && pBri->m_bExistProtectionRiseFooting && pBri->m_dPRF_W > 0)
	{
		double dLen = pBri->m_dPRF_W;
		long nSizeSta = m_dArrStation.GetSize();
		long nCount = (long)(MAX(1, Round(dLen / frM(UNIT_METER), 0)));
		double dSpace = dLen / nCount;

		for(long nSe=0; nSe<2; ++nSe)
		{
			BOOL bStt = (nSe == 0)? TRUE : FALSE;
			double dStaBase = m_dArrStation.GetAt(bStt ? 0 : nSizeSta-1) + (bStt ? -dLen : dSpace);

			for(long ix=0; ix<nCount; ++ix)
			{
				m_dArrStation_Pf[nSe].Add(dStaBase + dSpace * ix);
			}
		}
	}
}

HGDOUBLE CARcBridgeMeshCalc::GetHeightWall( long nJijum )
{
	HGDOUBLE dTop(0), dBot(0);
	long nSizePoint = m_dArrPointJijum[iUPPER][nJijum].GetSize();
	for(long ix=0; ix<nSizePoint; ++ix)
	{
		long idx = (long)(m_dArrPointJijum[iUPPER][nJijum].GetAt(ix));
		HGDOUBLE dPos = m_vArrSlab[iUPPER].GetAt(idx).z;

		if(ix == 0 || dPos > dTop)
			dTop = dPos;
	}

	if(m_pStd->m_pBri->IsBoxType())
	{
		long idxStt = m_vArrSlab[iUPPER].GetSize();
		nSizePoint = m_dArrPointJijum[iLOWER][nJijum].GetSize();

		for(long ix=0; ix<nSizePoint; ++ix)
		{
			long idx = (long)(m_dArrPointJijum[iLOWER][nJijum].GetAt(ix)) - idxStt;
			HGDOUBLE dPos = m_vArrSlab[iLOWER].GetAt(idx).z;

			if(ix == 0 || dPos < dBot)
				dBot = dPos;
		}
	}
	else
	{
		nSizePoint = m_vArrWall[nJijum].GetSize();
		for(long ix=0; ix<nSizePoint; ++ix)
		{
			HGDOUBLE dPos = m_vArrWall[nJijum].GetAt(ix).z;
			if(ix == 0 || dPos < dBot)
				dBot = dPos;
		}
	}

	return dTop - dBot;
}
