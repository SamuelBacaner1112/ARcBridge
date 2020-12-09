// OutBMTogong.cpp: implementation of the COutBMTogong class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "arcbridgeoutbm.h"
#include "../../Src/ARcBridgeDBDraw/ARcBridgeDBDraw.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COutBMTogong::COutBMTogong(long nBri)
{
	m_nBri	= nBri;

	// 삽도 출력에 관한 옵션들 /////////////////////////////////////////
	m_dScaleDim_VertSection		= 100;	// 종단면도 스케일
	m_dHeightText_VertSection	= 7;	// 종단면도 텍스트 높이
	////////////////////////////////////////////////////////////////////
}

COutBMTogong::~COutBMTogong()
{
	ClearBM();
}

// 수량을 모두 산출함.
// 산출한뒤 머지도 바로 함.
void COutBMTogong::CalculateBM()
{
	CRcBridgeRebar *pBri	= GetRcBridgeRebar();
	if(!pBri) return;
	
	ClearBM();

	if(pBri->IsRcSlab()) return;

	CRcBridgeRebar *pBriOrg	 = m_pStd->m_pARcBridgeDataStd->GetBridgeRebar(m_nBri);
	m_pStd->m_pARcBridgeDataStd->MakeRcBridgeForBM(pBriOrg, m_pStd->GetBridge(m_nBri));

	// 수량 산출
	MakeBMTyopagi();			// 터파기
	MakeBMWaterOut();			// 물푸기
	MakeBMBohoTyopagi();		// 보호블럭 터파기
	MakeBMBackFill();			// 뒷채움
	MakeBMReFill();			// 되메우기
	MakeBMReFillBoho();		// 보호블럭 되메우기
	MakeBMValuableSoil();		// 유용토
	MakeBMValuableSoilBoho();	// 보호블럭 유용토
	MakeBMDeductSoil();		// 공제토
	MakeBMApsungto();			// 앞성토
	
	// 산출한 수량 ViewBM에 등록
	long nJ = 0; for(nJ = 0; nJ < pBri->GetCountJijum(); nJ++)
	{
		if(pBri->IsFootingJijum(nJ) == FALSE) continue;
		SetViewBM(&m_pArrBMTogongStd[nJ]);
		ApplyOptionBMStd(&m_pArrBMTogongStd[nJ]);
	}
	
	long stt = 0; for(stt = 0; stt < 2; stt++)
	{
		SetViewBM(&m_pArrBMBohoStd[stt]);
		// 등록한뒤 산출한 수량을 ViewBM옵션을 적용 시킨다.
		ApplyOptionBMStd(&m_pArrBMBohoStd[stt]);
	}
}

// 그냥 산출한 수량에 대해서 필요에 따라 이리저리 머지를 해댐.
void COutBMTogong::MergeBMStd()
{
	CRcBridgeRebar	*pBri	= GetRcBridgeRebar();
	if(!pBri) return;

	// 차례대로 머지. /////////////////////
	CHgBaseBMStd baseBM;
	DWORD exceptionFlag	= 0x00000000;
	long stt(0), jijum(0);


	// 정제 되지 않은 수량을 정제함.(산출근거 출력용) - 완전히같은 수량에 대해서만 머지함.
	for(jijum = 0; jijum <= pBri->m_nQtyJigan; jijum++)
	{
		if(pBri->IsFootingJijum(jijum) == FALSE) continue;
		baseBM.MergeBMList(&m_pArrBMTogong[jijum], &m_pArrBMTogongStd[jijum], FALSE, exceptionFlag);
	}
	for(stt = 0; stt < 2; stt++)
	{
		baseBM.MergeBMList(&m_pArrBMBoho[stt], &m_pArrBMBohoStd[stt], FALSE, exceptionFlag);
	}
	
	// 부위별 집계표용
	for(jijum = 0; jijum <= pBri->m_nQtyJigan; jijum++)
	{
		if(pBri->IsFootingJijum(jijum) == FALSE) continue;
		baseBM.MergeBMList(&m_pArrBMTogongForPartSum[jijum], &m_pArrBMTogong[jijum], TRUE, exceptionFlag);
	}
	for(stt = 0; stt < 2; stt++)
	{
		baseBM.MergeBMList(&m_pArrBMBohoForPartSum[stt], &m_pArrBMBoho[stt], TRUE, exceptionFlag);
	}

	// 교량집계표용(토공+보호블럭) - 총계내는 레벨에 따라서 머지함
	for(jijum = 0; jijum <= pBri->m_nQtyJigan; jijum++)
	{
		if(pBri->IsFootingJijum(jijum) == FALSE) continue;
		baseBM.MergeBMList(&m_pArrBMTogongSum, &m_pArrBMTogong[jijum], TRUE, exceptionFlag);
	}

	for(stt = 0; stt < 2; stt++)
	{
		baseBM.MergeBMList(&m_pArrBMBohoSum, &m_pArrBMBoho[stt], TRUE, exceptionFlag);
	}
	
	// 교량집계표 계
	baseBM.MergeBMList(&m_pArrBMSum, &m_pArrBMTogongSum, TRUE, exceptionFlag);
	baseBM.MergeBMList(&m_pArrBMSum, &m_pArrBMBohoSum, TRUE, exceptionFlag);
}

// 수량 출력용 문자열 생성
void COutBMTogong::MakeBMStringStd()
{
	CRcBridgeRebar	*pBri	= GetRcBridgeRebar();
	if(!pBri) return;
	
	CHgBaseBMStd baseBM;
	long stt(0), jijum(0);

	// 정제된 산출근거용 수량
	for(jijum = 0; jijum <= pBri->m_nQtyJigan; jijum++)
	{
		if(pBri->IsFootingJijum(jijum) == FALSE) continue;
		baseBM.MakeBMString(&m_pArrBMStringTogong[jijum], &m_pArrBMTogong[jijum], TRUE);
	}
	for(stt = 0; stt < 2; stt++)
	{
		baseBM.MakeBMString(&m_pArrBMStringBoho[stt], &m_pArrBMBoho[stt], TRUE);
	}

	// 부위별 집계표(토공집계, 보호블럭집계)
	for(jijum = 0; jijum <= pBri->m_nQtyJigan; jijum++)
	{
		if(pBri->IsFootingJijum(jijum) == FALSE) continue;
		baseBM.MakeBMSumString(&m_pArrBMStringTogongSum, &m_pArrBMTogongForPartSum[jijum], TRUE, jijum==0);		// 본체
	}
	baseBM.MakeBMSumString(&m_pArrBMStringTogongSum, &m_pArrBMTogongSum, TRUE, FALSE);		// 본체

	for(stt = 0; stt < 2; stt++)
	{
		baseBM.MakeBMSumString(&m_pArrBMStringBohoSum, &m_pArrBMBohoForPartSum[stt], TRUE, stt==0);		// 보호블럭
	}
	baseBM.MakeBMSumString(&m_pArrBMStringBohoSum, &m_pArrBMBohoSum, TRUE, FALSE);		// 보호블럭

	// 교량집계표용(토공+보호블럭)
	baseBM.MakeBMSumString(&m_pArrBMStringAllSum, &m_pArrBMTogongSum, TRUE, TRUE);		// 본체
	baseBM.MakeBMSumString(&m_pArrBMStringAllSum, &m_pArrBMBohoSum, TRUE, FALSE);		
	baseBM.MakeBMSumString(&m_pArrBMStringAllSum, &m_pArrBMSum, TRUE, FALSE);	// 계
}

// 수량 제거
void COutBMTogong::ClearBM(BOOL bRemainStdBM)
{
	long jijum(0), stt(0);

	if(bRemainStdBM == FALSE)
	{
		// 수집용(정제안된 순수 수량)
		for(jijum = 0; jijum < MAX_JIJUM_COUNT_RC; jijum++)
		{
			AhTPADelete(&m_pArrBMTogongStd[jijum],	(CBMData*)0);			// 토공수량(정제 안된것)
		}
		for(stt = 0; stt < 2; stt++)
		{
			AhTPADelete(&m_pArrBMBohoStd[stt],		(CBMData*)0);			// 보호블럭수량(정제 안된것)
		}
	}

	// 출력용(정제된 수량)
	for(jijum = 0; jijum < MAX_JIJUM_COUNT_RC; jijum++)
	{
		AhTPADelete(&m_pArrBMTogong[jijum],		(CBMData*)0);			// 토공수량(정제 안된것) - 산출근거 출력
		AhTPADelete(&m_pArrBMStringTogong[jijum],	(CBMString*)0);			// 토공수량(정제 안된것) - 산출근거 출력
	}
	
	for(stt = 0; stt < 2; stt++)
	{
		AhTPADelete(&m_pArrBMBoho[stt],			(CBMData*)0);			// 보호블럭수량(정제 안된것) - 산출근거 출력
		AhTPADelete(&m_pArrBMStringBoho[stt],	(CBMString*)0);			// 보호블럭수량(정제 안된것) - 산출근거 출력
	}

	// 부위별집계표에 출력된 내용
	for(jijum = 0; jijum < MAX_JIJUM_COUNT_RC; jijum++)
	{
		AhTPADelete(&m_pArrBMTogongForPartSum[jijum],	(CBMData*)0);
	}

	for(stt = 0; stt < 2; stt++)
	{
		AhTPADelete(&m_pArrBMBohoForPartSum[stt],	(CBMData*)0);;	
	}

	AhTPADelete(&m_pArrBMTogongSum,			(CBMData*)0);				// 토공수량 - 교량별 집계표 출력 
	AhTPADelete(&m_pArrBMStringTogongSum,	(CBMString*)0);
		
	AhTPADelete(&m_pArrBMBohoSum,			(CBMData*)0);				// 보호블럭수량 - 교량별 집계표 출력
	AhTPADelete(&m_pArrBMStringBohoSum,		(CBMString*)0);

	AhTPADelete(&m_pArrBMStringAllSum,		(CBMString*)0);				// 집계(토공수량+보호블럭) - 교량별 집계표 출력 
	AhTPADelete(&m_pArrBMSum,				(CBMData*)0);				// 교량집계(정제 안된것) - 교량별 집계표 출력

	AhTPADelete(&m_pArrBMStringSepBridgeSum,		(CBMSumString*)0);
	AhTPADelete(&m_pArrBMSepBridgeSum,				(CBMData*)0);	
}

// 수량을 ViewBM에 등록함.
void COutBMTogong::SetViewBM(CTypedPtrArray <CObArray, CBMData*> *pArrBM)
{
	if(!pArrBM) return;

	long i = 0; for(i = 0; i < pArrBM->GetSize(); i++)
	{
		m_pStd->m_pViewBMTogong->AddBM(pArrBM->GetAt(i)->m_sBigClass);
	}
}

// 수량을 ViewBM에 적용시켜서 필요 없는거는 제거함.
void COutBMTogong::ApplyViewBM(CTypedPtrArray <CObArray, CBMData*> *pArrBM)
{
	if(!pArrBM) return;

	long i = 0; for(i = 0; i < pArrBM->GetSize(); i++)
	{
		if(!m_pStd->m_pViewBMTogong->IsCheckView(pArrBM->GetAt(i)->m_sBigClass))
		{
			delete pArrBM->GetAt(i);
			pArrBM->RemoveAt(i);
			i--;
		}
	}
}

// 수량을 셀참조식으로 출력할지에 대한 옵션 적용
// 단, 임의수식인 경우 BMData의 옵션이 우선권을 가짐
void COutBMTogong::ApplyRefCellBM(CTypedPtrArray <CObArray, CBMData*> *pArrBM)
{
	if(!pArrBM) return;

	long i = 0; for(i = 0; i < pArrBM->GetSize(); i++)
	{
		CBMData *pBM	= pArrBM->GetAt(i);
		if(pBM->m_sForm == "" || pBM->m_bIsPrintByRefForm)
		{
			pBM->m_bIsPrintByRefCell	= m_pStd->m_bConvertToRef;
			pBM->m_bIsPrintByRefForm	= m_pStd->m_bConvertToForm;
		}
		
		pArrBM->SetAt(i, pBM);
	}
}

// 수량에 옵션을 적용함.
void COutBMTogong::ApplyOptionBMStd(CTypedPtrArray <CObArray, CBMData*> *pArrBM)
{
	ApplyViewBM(pArrBM);
	ApplyRefCellBM(pArrBM);
}


// 현재 작업중인 bri 리턴
CRcBridgeRebar* COutBMTogong::GetRcBridgeRebar(BOOL bOrg)
{
	if(m_nBri < 0 || m_nBri > m_pStd->m_pARcBridgeDataStd->GetBridgeSize()-1) return NULL;
	CRcBridgeRebar *pBri	 = m_pStd->m_pARcBridgeDataStd->GetBridgeRebar(m_nBri);

	return bOrg ? pBri : m_pStd->GetBridge(m_nBri);
}

// 공종의 이름이 옵션에 따라서 바뀔 경우를 대비해서 함수로 처리함.
CString COutBMTogong::GetStringBigClass(long nIdxBigClass)
{
	CRcBridgeRebar	*pBri	= GetRcBridgeRebar();
	if(!pBri) return _T("");

	if(nIdxBigClass == BM_BIG_TUPAGI)					return "터파기";
	if(nIdxBigClass == BM_BIG_TUPAGI_WATER)		return pBri->m_bApplyYongSu? "용수터파기" : "수중터파기";
	if(nIdxBigClass == BM_BIG_BACKFILL)				return "뒷채움";
	if(nIdxBigClass == BM_BIG_REFILL)					return "되메우기";
	if(nIdxBigClass == BM_BIG_VALUABLE_SOIL)		return "유용토";
	if(nIdxBigClass == BM_BIG_DEDUCT_SOIL)		return "공제토";
	if(nIdxBigClass == BM_BIG_APSUNGTO)			return "앞성토";
		
	return _T("");
}

// 공종의 이름이 옵션에 따라서 바뀔 경우를 대비해서 함수로 처리함.
CString COutBMTogong::GetStringMidClass(long nIdxMidClass)
{
	CRcBridgeRebar	*pBri	= GetRcBridgeRebar();
	if(!pBri) return _T("");

	if(nIdxMidClass == BM_MID_TUPAGI_TOSA)		return "토사";
	if(nIdxMidClass == BM_MID_TUPAGI_PUWHAAM)	return "풍화암";
	if(nIdxMidClass == BM_MID_TUPAGI_YUNAM)		return "연암";
	if(nIdxMidClass == BM_MID_TUPAGI_GYUNGAM)	return "경암";


	return _T("");
}

CString COutBMTogong::GetStringSmallClass(long nIdxSmallClass)
{
	CRcBridgeRebar	*pBri	= GetRcBridgeRebar();
	if(!pBri) return _T("");

	if(nIdxSmallClass == BM_SMALL_DEDUCT_SOIL_NOSANG)			return "노상";
	if(nIdxSmallClass == BM_SMALL_DEDUCT_SOIL_NOCHE)			return "노체";

	return _T("");
}

// 총계 내는 레벨 리턴
long COutBMTogong::GetLevelTotal(long nIdxBigClass)
{
	CRcBridgeRebar	*pBri	= GetRcBridgeRebar();
	if(!pBri) return 0;

	if(nIdxBigClass == BM_BIG_TUPAGI)			return 4;
	if(nIdxBigClass == BM_BIG_TUPAGI_WATER)		return 4;
	if(nIdxBigClass == BM_BIG_BACKFILL)			return 0;
	if(nIdxBigClass == BM_BIG_REFILL)			return 0;
	if(nIdxBigClass == BM_BIG_VALUABLE_SOIL)	return 0;
	if(nIdxBigClass == BM_BIG_DEDUCT_SOIL)		return 2;
	if(nIdxBigClass == BM_BIG_APSUNGTO)			return 0;
	
	
	
	return 0;
}

CString COutBMTogong::GetStringItemClass(long nIdxItemClass)
{
	CRcBridgeRebar	*pBri	= GetRcBridgeRebar();
	if(!pBri) return _T("");
	
	if(nIdxItemClass == BM_ITEM_BACKFILL)						return "뒷채움";
	if(nIdxItemClass == BM_ITEM_BACKFILL_APS)					return "접속슬래브 공제";
	if(nIdxItemClass == BM_ITEM_BACKFILL_WING)					return "날개벽 공제";
	if(nIdxItemClass == BM_ITEM_BACKFILL_BRACKET)				return "브라켓 공제";
	if(nIdxItemClass == BM_ITEM_REFILL_TYPAGI)					return "터파기";
	if(nIdxItemClass == BM_ITEM_REFILL_MAIN)					return "구체공제";
	if(nIdxItemClass == BM_ITEM_REFILL_BACKFILL)				return "뒷채움공제";
	if(nIdxItemClass == BM_ITEM_APSUNGTO_MAINS)					return "본체(시점)";
	if(nIdxItemClass == BM_ITEM_APSUNGTO_MAINE)					return "본체(종점)";
	if(nIdxItemClass == BM_ITEM_APSUNGTO_WINGSL)				return "날개벽(시점좌측)";
	if(nIdxItemClass == BM_ITEM_APSUNGTO_WINGSR)				return "날개벽(시점우측)";
	if(nIdxItemClass == BM_ITEM_APSUNGTO_WINGEL)				return "날개벽(종점좌측)";
	if(nIdxItemClass == BM_ITEM_APSUNGTO_WINGER)				return "날개벽(종점우측)";

	return _T("");
}

// 산출근거중 수식 중간에 들어가야 되는 삽도를 추가시킨다.
void COutBMTogong::SetInsertDomyunStd()
{
	SetInsertDomyunBackFill();
	SetInsertDomyunApsungto();
}

// 미터별로 나눠야 되는 수량에 미터정보를 생성시켜준다.
void COutBMTogong::RemakeByHeightBMList(CTypedPtrArray <CObArray, CBMData*> *pArrBM, double dYBase, double dFirstMeter, double dSecondMeter, long nIdxStt, long nIdxEnd)
{
	if(!pArrBM) return;
	long nSize		= pArrBM->GetSize();
	if(dFirstMeter < 0 && dSecondMeter > 0) return;
	if(dFirstMeter > 0 && dSecondMeter < 0) return;
	if(dSecondMeter == 0) return;

	BOOL bRev		= dFirstMeter < 0 && dSecondMeter < 0;
	long nIdxMeter	= 0;
	double dYCur	= 0;
	double dYOld	= 0;
	if(nIdxStt == -1) nIdxStt	= 0;
	if(nIdxEnd == -1) nIdxEnd	= nSize-1;
	if(nIdxEnd >= nSize) nIdxEnd	= nSize-1;

	long i = 0; for(i = nIdxStt; i <= nIdxEnd; i++)
	{
		CBMData *pBM	= pArrBM->GetAt(i);
		if(!IsPrintMeter(pBM)) continue;

		dYCur		= dYBase;
		dYOld		= dYBase;
		nIdxMeter	= 0;

		while(1)
		{
			dYCur += nIdxMeter==0 ? dFirstMeter : dSecondMeter;

			if((bRev && Compare(dYCur, frM(pBM->m_dV[EndMeter]), "<=")) || (!bRev && Compare(dYCur, frM(pBM->m_dV[EndMeter]), ">=")))
			{
				pBM->m_sMeterClass.Format("%s(%.0f~%.0fm)", pBM->m_sSmallClass, toM(fabs(dYOld-dYBase)), toM(fabs(dYCur-dYBase)));
				pBM->m_nIdxMeterClass	= nIdxMeter;
				break;
			}
			nIdxMeter++;

			dYOld	= dYCur;
		}
	}
}

// 미터정보 출력하는 수량은 자를게 없어도 미터정보를 만들어 줘야 된다.
BOOL COutBMTogong::IsPrintMeter(CBMData *pBM)
{
	if(pBM->m_nIdxBigClass == BM_BIG_TUPAGI) return TRUE;
	if(pBM->m_nIdxBigClass == BM_BIG_TUPAGI_WATER) return TRUE;

	return FALSE;
}

// 미터정보 나눌때 첫번째 자를 높이와 그 다음 자를 높이 가져옴.
// CFootingApp *pFooting : 터파기 최초 굴착깊이 등을 구할때는 필요함
void COutBMTogong::GetInfoMeter(long nIdxBigClass, double &dFirstMeter, double &dSecondMeter, CFootingApp *pFooting)
{
	if(!pFooting) return;

	if(nIdxBigClass == BM_BIG_TUPAGI)
	{
		dFirstMeter		= -pFooting->m_dDepthDiggingFirst;
		dSecondMeter	= -pFooting->m_dDepthDiggingNext;
	}
}

// 토공 산출근거 첫폐이지 삽도
// 터파기 (단면도, 정면도)(각 기초별)
long COutBMTogong::DrawFirstPageTogong(CHgBaseBMStd *pBaseBMStd, long jijum)
{
	long nRow	= 5;
	if(!pBaseBMStd) return nRow;

	CRcBridgeRebar *pBri	= GetRcBridgeRebar();
	if(!pBri) return nRow;

	// 정면도 토공선은 사로 그려야 함.
	// (ARCBRIDGE-3549) BOX교의 터파기는 교량시점의 직방향으로 구한다.
	pBri->GetTvVertSection_Std(0, pBri->IsBoxType() ? FALSE : TRUE, FALSE, FALSE, 2, TRUE);

	CARcBridgeDBStandard DBStd(pBri, m_pStd->m_pARoadOptionStd, m_pStd->m_pARcBridgeDataStd);
	DBStd.m_bApplyDimSettingOfBaseDom	= TRUE;
	CDrawVertSection *pDrawVertSection		= DBStd.m_pDrawVertSection;
	CDrawCrossSection *pDrawCrossSection	= DBStd.m_pDrawCrossSection;
	
	// 실제로 출력되었으면 하는 사이즈
	double dWidthOrg			= 400;	// 도면 가로크기
	BOOL bDrawRubble = pBri->IsBoxType() && pBri->m_dThickCobbStone > 0 && pBri->m_nTypeLower == LOWERSLAB_TYPE_COBBSTONE;

	// 스케일 조정용 도면 ///////
	CDomyun domTmp;
	domTmp.SetCWnd(m_pStd->m_pView);
	pDrawVertSection->DrawPartByJijum(&domTmp, jijum, TRUE);
	if(bDrawRubble)
		pDrawVertSection->DrawExFootingBoxType(&domTmp, TRUE);
	pDrawVertSection->DrawTogongLine(&domTmp, pBri, jijum, TRUE, FALSE, FALSE, FALSE);
	pDrawVertSection->DimTogongLine(&domTmp, pBri, jijum, TRUE, FALSE, FALSE);
	/////////////////////////////

	// 터파기 선 그리기 ///////////////////////////////////////////////////////////
	CFootingApp *pFooting	= pBri->GetFootingByJijumNumber(jijum);
	if(!pFooting)
	{
		pBri->GetTvVertSection_Std(0, TRUE, FALSE, TRUE, 2, TRUE);
		return nRow;
	}

	if(!pFooting->m_bIs) 
	{
		pBri->GetTvVertSection_Std(0, TRUE, FALSE, TRUE, 2, TRUE);
		return nRow;
	}

	CDomyun domVertSection;
	domVertSection.SetCWnd(m_pStd->m_pView);

	// 도면 설정 ////////////////////
	double dScale	= pBaseBMStd->GetScale(&domTmp, dWidthOrg);
	pBaseBMStd->SetDomyunValue(&domVertSection, dWidthOrg, &domTmp);
	///////////////////////////////////
	
	// 토공선 그림
	pDrawVertSection->DrawPartByJijum(&domVertSection, jijum, TRUE);
	if(bDrawRubble)
		pDrawVertSection->DrawExFootingBoxType(&domVertSection, TRUE);
	pDrawVertSection->DrawTogongLine(&domVertSection, pBri, jijum, TRUE, FALSE, FALSE, FALSE);
	pDrawVertSection->DimTogongLine(&domVertSection, pBri, jijum, TRUE, FALSE, FALSE, TRUE);

	//BOX형일때는 단면 전체를 그려줌
	if(pBri->IsBoxType())
	{
		DBStd.m_pDrawVertSection->DrawVertSection(&domVertSection, TRUE);
	}

	// 삽도를 WidthOrg 사이즈로 변경
	domVertSection.RedrawByScale(dScale, TRUE);

	// 엑셀에 삽입
	domVertSection.SetCalcExtRect();
	CDRect rect	= domVertSection.GetExtRect();

	CString sTitle	= _T("");
	if(jijum == 0)
		sTitle	= "시점측 터파기 단면";
	else if(jijum == pBri->GetCountJijum()-1)
		sTitle	= "종점측 터파기 단면";
	else
		sTitle.Format("중간지점%d 터파기 단면", jijum);
	CDoubleArray dArrTmp;
	nRow	= pBaseBMStd->InsertDomyun(nRow, TA_CENTER, &domVertSection, dArrTmp, rect.Width(), rect.Height(), sTitle) + 5;

	// 터파기 선 정면도

	// 기준 도면  ////////////////
	CDomyun domTmp2;
	domTmp2.SetCWnd(m_pStd->m_pView);

	// 토공선 그림
	//pDrawCrossSection->DrawCrossAllAtJijum(&domTmp2, jijum, FALSE, FALSE, FALSE, FALSE);

	// 토공선 그림
	pDrawCrossSection->DrawTogongLine(&domTmp2, jijum, TRUE, FALSE);

	pBri->GetTvVertSection_Std(0, TRUE, FALSE, TRUE, 2, TRUE);
	pDrawCrossSection->DimTogongLine(&domTmp2, jijum, FALSE, TRUE, pBri->m_nDeductNearTupagiBM);
	pBri->GetTvVertSection_Std(0, TRUE, FALSE, FALSE, 2, TRUE);
	// 기준 도면  ////////////////

	CDomyun domFront;
	domFront.SetCWnd(m_pStd->m_pView);

	// 도면 설정 ////////////////////
	dScale	= pBaseBMStd->GetScale(&domTmp2, dWidthOrg);
	pBaseBMStd->SetDomyunValue(&domFront, dWidthOrg, &domTmp2);
	///////////////////////////////////

	// 토공선 그림
	//pDrawCrossSection->DrawCrossAllAtJijum(&domFront, jijum, FALSE, FALSE, FALSE, FALSE);
	// pDrawCrossSection->DrawCrossAllAtJijum를 그리면 기존 셋팅되어 있던 것이 풀려 버리므로
	// 기초와 확대기초만 따로 그리자
	CExFootingApp *pExFooting	= &pFooting->m_exFooting;
	DBStd.m_pDrawFooting->DrawLeftSide(&domFront, pFooting, FALSE, 0, TRUE, TRUE);
	DBStd.m_pDrawExFooting->DrawLeftSide(&domFront, pExFooting, 0);
	
	if(pBri->IsBoxType())
	{
		DBStd.m_pDrawCrossSection->DrawCrossAllAtJijum(&domFront, 0, TRUE, FALSE, TRUE, TRUE, bDrawRubble);
	}

	// 토공선 그림
	pDrawCrossSection->DrawTogongLine(&domFront, jijum, TRUE, FALSE, FALSE, pBri->m_nDeductNearTupagiBM);
	pBri->GetTvVertSection_Std(0, TRUE, FALSE, TRUE, 2, TRUE);
	pDrawCrossSection->DimTogongLine(&domFront, jijum, FALSE, TRUE, pBri->m_nDeductNearTupagiBM);
	pBri->GetTvVertSection_Std(0, TRUE, FALSE, FALSE, 2, TRUE);

	// 삽도를 WidthOrg 사이즈로 변경
	domFront.RedrawByScale(dScale, TRUE);
	
	// 엑셀에 삽입
	domFront.SetCalcExtRect();
	rect	= domFront.GetExtRect();

	sTitle	= _T("");
	if(jijum == 0)
		sTitle	= "시점측 터파기 정면";
	else if(jijum == pBri->GetCountJijum()-1)
		sTitle	= "종점측 터파기 정면";
	else
		sTitle.Format("중간지점%d 터파기 정면", jijum);

	nRow	= pBaseBMStd->InsertDomyun(nRow, TA_CENTER, &domFront, dArrTmp, rect.Width(), rect.Height(), sTitle) + 3;


	pBri->GetTvVertSection_Std(0, TRUE, FALSE, TRUE, 2, TRUE);
	return nRow;
}

// 보호블럭 도면 첫페이지 삽도
void COutBMTogong::DrawFirstPageBoho(CHgBaseBMStd *pBaseBMStd, BOOL bStt)
{
	if(!pBaseBMStd) return;

	CRcBridgeRebar *pBri	= GetRcBridgeRebar();
	if(!pBri) return;
	long nTypeBoho	= pBri->m_nTypeRcBridgeBlock;
	if(nTypeBoho == 0) return;

	CARcBridgeDBStandard DBStd(pBri, m_pStd->m_pARoadOptionStd, m_pStd->m_pARcBridgeDataStd);
	
	// 실제로 출력되었으면 하는 사이즈
	double dWidthOrg			= 600;	// 도면 가로크기
	double dHeightOrg			= 800;	// 도면 가로크기

	// 터파기 선 그리기 ///////////////////////////////////////////////////////////
	long nRow	= 3;
	
	CDomyun Dom;
	Dom.SetCWnd(m_pStd->m_pView);

	// 교대 보호블럭 dxf 삽입.
	CString szPath = AfxGetApp()->GetProfileString("Afx Directory", "Fixed Directory");
	
	// 교대보호블럭이 없음으로 되어 있으면 하천용으로 생각하고 구함.
	CString strDxfFileName		= nTypeBoho == 2  ? "교대보호블럭(육교용).dxf" : "교대보호블럭(하천용).dxf";
	CString strDxfName			= _T("");
	CString sBoho				= _T("");
	
	strDxfName.Format("%s\\dxf\\%s", szPath, strDxfFileName);
	//sBoho.Format("교대보호블럭(%s)", nTypeBoho == 2 ? "육교용" : "하천용");
	
	if(!Dom.DxfIn(strDxfName)) AfxMessageBox("교대보호블럭 가져오기 실패!");

	// 엑셀에 삽입
	Dom.SetCalcExtRect();
	CDoubleArray dArrTmp;
	nRow	= pBaseBMStd->InsertDomyun(nRow, TA_CENTER, &Dom, dArrTmp, dWidthOrg, dHeightOrg, sBoho);
}

// 보호블럭 터파기(시종점)
void COutBMTogong::MakeBMBohoTyopagi()
{
	CRcBridgeRebar *pBri	= GetRcBridgeRebar();
	if(!pBri) return;
	CLineInfo *pLine	= pBri->GetLineBase();
	if(!pLine) return;
	long nTypeBoho	= pBri->m_nTypeRcBridgeBlock;
	if(nTypeBoho == 0) return;

	CRebarPlacing rb;

	// 상수
	const double dTyopagiDepth	= 1000;		// 교대보호블럭 터파기 깊이
	CTypedPtrArray <CObArray, CTwinVectorAreaShape*> pArrAreaConc;

	long stt = 0; for(stt = 0; stt < 2; stt++)
	{
		BOOL bStt		= stt == iSTT;
		long nJ			= bStt ? 0 : pBri->m_nQtyJigan;
		CTypedPtrArray <CObArray, CBMData*> *pArrBM	= &m_pArrBMBohoStd[stt];

		BOOL bLeft	= TRUE;
		CWingWall *pWing	= NULL;
		if(pBri->IsWingWall(bStt, TRUE))
		{
			pWing	= pBri->GetWingWall(bStt, TRUE);
			bLeft	= TRUE;
		}
		else if(pBri->IsWingWall(bStt, FALSE)) 
		{
			pWing	= pBri->GetWingWall(bStt, FALSE);
			bLeft	= FALSE;
		}
		if(!pWing) continue;
	
		double dLenWall			= toM(pBri->GetTvJijum(nJ, 0).GetLength());
		double dLenBohoFooting	= toM(2*ConstPi*GetLengthBohoHor(bStt)*15*2/360) + dLenWall;

		// 보호블럭 기초에 해당하는 footingApp를 생성해서 터파기 라인을 구함 /////////////////////
		// 기초 생성시 기초 하면만 있으면 되므로 기초 타입은 FOOTING_TYPE_BASIC_BOTH로 그냥 한다.
		double dElEarth		= 0;
		pLine->GetElevationErathOnStation(pBri->GetStationOnJijum(nJ, 0), dElEarth);

		CFootingApp *pFooting	= bStt ? &pBri->m_footingStt : &pBri->m_footingEnd;
		CFootingApp footing;
		footing.m_nType		= FOOTING_TYPE_BASIC_BOTH;
		footing.m_dFH		= 400;
		footing.m_dEL		= dElEarth - dTyopagiDepth + footing.m_dFH;
		footing.m_dSta		= pFooting->m_dSta;
		footing.m_dFWR		= 200;
		footing.m_dFWL		= 200;
		footing.m_dMR		= 0;
		footing.m_dML		= 0;
		footing.m_dDistOffsetDigging	= 1000;
		footing.m_xyBottomCenter.x	= footing.m_dSta;
		footing.m_xyBottomCenter.y	= footing.m_dEL - footing.GetHeight();
		footing.m_dEL_DiggingFirst	= dElEarth;
		footing.m_dDiggingSlopeTosa	= pFooting->m_dDiggingSlopeTosa;
		footing.m_dDiggingSlopeGyungam	= pFooting->m_dDiggingSlopeGyungam;
		footing.m_dDiggingSlopePungwhaam	= pFooting->m_dDiggingSlopePungwhaam;
		footing.m_dDiggingSlopeYunam	= pFooting->m_dDiggingSlopeYunam;
		
//		CBoringDataEx *pExBoring	= pBri->GetBoringDataExByStation(footing.m_dSta);

		CBoringDataEx *pExBoring	= pBri->GetBoringDataEx(nJ);
		if(!pExBoring) continue;

		CTwinVectorArray tvArr;
		pBri->GetTvTogongLine(tvArr, &footing, pExBoring, TRUE, FALSE, TRUE, FALSE);

		// 영역별로 나눈다.
		CTwinVectorArray tvArrResult;
		CDoubleArray dArrAngleSu;
		long nIdx	= 0;

		rb.SeperatorTwinVectorArea(tvArr, tvArrResult, dArrAngleSu);
		long j = 0; for(j = 0; j < dArrAngleSu.GetSize(); j++)
		{
			CTwinVectorAreaShape *pArea	= new CTwinVectorAreaShape(j+1);
			for(long k = 0; k < (long)dArrAngleSu.GetAt(j); k++)
				pArea->AddTwinVector(tvArrResult.GetAt(nIdx++));

			pArea->Calculate();
			pArrAreaConc.Add(pArea);
		}
		//////////////////////////////////////////////////////////////////////////////////////////////
		
		// 수량 산출
		long nSizeBM	= pArrBM->GetSize();
		long nSize		= pArrAreaConc.GetSize();
		long i = 0; for(i = 0; i < nSize; i++)
		{
			CTwinVectorAreaShape *pArea	= pArrAreaConc.GetAt(i);

			double dWidthUpper	= toM(pArea->m_dWidthUpper);
			double dWidthLower	= toM(pArea->m_dWidthLower);
			double dHeight		= toM(pArea->m_dHeight);
			
			// 보링 타입에 따른 이름
			long nMidClass	= 0;
			CTwinVector tvLeft	= pArea->GetTvLeft();
				
			if(tvLeft.m_sInfo.Find("토사", 0) != -1) nMidClass = BM_MID_TUPAGI_TOSA;
			else if(tvLeft.m_sInfo.Find("풍화암", 0) != -1) nMidClass = BM_MID_TUPAGI_PUWHAAM;
			else if(tvLeft.m_sInfo.Find("연암", 0) != -1) nMidClass = BM_MID_TUPAGI_YUNAM;
			else if(tvLeft.m_sInfo.Find("경암", 0) != -1) nMidClass = BM_MID_TUPAGI_GYUNGAM;
			else nMidClass	= BM_MID_TUPAGI_TOSA;
			
			CBMData *pBM				= new CBMData;
			pBM->m_nIdxBigClass			= BM_BIG_TUPAGI;		
			pBM->m_sBigClass			= GetStringBigClass(pBM->m_nIdxBigClass);
			pBM->m_nLevelTotal			= GetLevelTotal(pBM->m_nIdxBigClass);
			pBM->m_nIdxMidClass			= nMidClass;			
			pBM->m_sMidClass			= GetStringMidClass(pBM->m_nIdxMidClass);
			pBM->m_nTypeResult			= RESULT_VOLUMN;		
			pBM->m_sUnit				= "㎥";
			pBM->m_dV[SttMeter]			= 0;
			pBM->m_dV[EndMeter]			= 4;
			pBM->m_nShape				= pArea->m_nTypeShape;
			pBM->m_dV[WidthUpper]		= Round(dWidthUpper,3);
			pBM->m_dV[Length]			= Round(dLenBohoFooting,3);
			pBM->m_dV[WidthLower]		= Round(dWidthLower,3);
			pBM->m_dV[Height]			= Round(dHeight,3);
			pBM->CalculateResult();
			pBM->m_sVCN[Result]			= "보호블럭터파기";
			pArrBM->Add(pBM);
		}

		// 수량에 미터정보를 생성해서 넣어준다
		// 보호블럭은 고정이다.(0 ~ 4m)
		RemakeByHeightBMList(pArrBM, 0, 4000, 1000, nSizeBM-1, pArrBM->GetSize()-1);
		AhTPADelete(&pArrAreaConc,	(CTwinVectorAreaShape*)0);
	}

	AhTPADelete(&pArrAreaConc,	(CTwinVectorAreaShape*)0);
}

// 보호블럭 수평 길이
// 성토라인에서 깊이 600만큼 더 들어간 곳 까지의 라인임
double COutBMTogong::GetLengthBohoHor(BOOL bStt)
{
	CRcBridgeRebar *pBri	= GetRcBridgeRebar();
	if(!pBri) return 0;
	
	// 보호블럭은 기본적으로 왼쪽을 기준으로 하지만, 없다면 오른쪽도 함
	// 오른쪽도 없다면 0 리턴
	BOOL bLeft	= pBri->IsWingWall(bStt, TRUE);
	if(!bLeft && !pBri->IsWingWall(bStt, FALSE)) return 0;

	CTwinVectorArray tvArrBoho;
	pBri->GetTvSungtoLine(tvArrBoho, bStt, bLeft, TRUE);

	return tvArrBoho.GetHorLength();
}

// 터파기
void COutBMTogong::MakeBMTyopagi()
{
	CRcBridgeRebar *pBri	= GetRcBridgeRebar();
	if(!pBri) return;

	CLineInfo *pLine	= pBri->GetLineBase();
	if(!pLine) return;
	CTypedPtrArray <CObArray, CTwinVectorAreaShape*> pArrAreaConc[2];

	CRebarPlacing rb;
	CHgBaseBMStd baseBM;

	pBri->GetTvVertSection_Std(0, pBri->IsBoxType() ? FALSE : TRUE, FALSE, FALSE, 2, TRUE);
	
	long jijum = 0; for(jijum = 0; jijum <= pBri->m_nQtyJigan; jijum++)
	{
		CFootingApp *pFooting	= pBri->GetFootingByJijumNumber(jijum);
		if(!pFooting) continue;
		if(!pFooting->m_bIs) continue;
		if(pBri->IsFootingJijum(jijum) == FALSE) continue;

		CTypedPtrArray <CObArray, CBMData*> *pArrBM	= &m_pArrBMTogongStd[jijum];

		CTwinVectorArray tvArrVert, tvArrCross;	// 종단과 횡단면의 토공라인을 모두 구해야 됨.
		pBri->GetTvTogongLine(tvArrVert, jijum, FALSE, TRUE, TRUE, FALSE);
		pBri->GetTvVertSection_Std(0, TRUE, TRUE, FALSE, 2);
		pBri->GetTvTogongLine(tvArrCross, jijum, TRUE, TRUE, TRUE, FALSE, pBri->m_nDeductNearTupagiBM);
		pBri->GetTvVertSection_Std(0, TRUE, FALSE, FALSE, 2, TRUE);

		// 32223
		//구배가 모두 0이 아니라면 False 모두 0이라면 TRUE
		double dYBase		= 0;
		dYBase	= tvArrVert.GetXyTop().y; //이곳으로 옮김

		BOOL bGooBae = FALSE;
		CTwinVectorArray tvArrResultGooBae;

		if(pFooting->m_dDiggingSlopeTosa == 0 && pFooting->m_dDiggingSlopePungwhaam == 0 && 
			pFooting->m_dDiggingSlopeYunam == 0 && pFooting->m_dDiggingSlopeGyungam == 0)
		{
			bGooBae = TRUE;
		}
		//}
		
		// 영역별로 나눈다. ////////////////////////////////////////////////////////////////////////
		for(long vert = 0; vert < 2; vert++)
		{
			CTwinVectorArray tvArrResult;
			CDoubleArray dArrAngleSu;
			CTwinVectorArray tvArr;
			long nIdx	= 0;
			tvArr	= vert==0 ? tvArrVert : tvArrCross;

			//32223 모든 구배가 0이고 수중EL이 존재할 경우에만 사용함
			if(bGooBae && pFooting->m_dEL_DiggingWater > 1)
			{
				rb.SeperatorTwinVectorArea(tvArr, tvArrResult, dArrAngleSu, 0.0, FALSE, FALSE);

				CalcZeroGooBaeExcavation(tvArrResult, tvArrResultGooBae, dYBase, pFooting->m_dEL_DiggingWater ); 
			}
			else
			{
				CTwinVectorArray tvArrTosa, tvArrPung, tvArrYun, tvArrKyung, tvArrEtc;

				for (long nIdx =0; nIdx < tvArr.GetSize(); nIdx++)
				{
					CTwinVector tvTu = tvArr.GetAt(nIdx);
					if(tvTu.m_sInfo.Find(_T("토사")))
						tvArrTosa.Add(tvTu);
					else if(tvTu.m_sInfo.Find(_T("풍화암")))
						tvArrPung.Add(tvTu);
					else if(tvTu.m_sInfo.Find(_T("연암")))
						tvArrYun.Add(tvTu);
					else if(tvTu.m_sInfo.Find(_T("경암")))
						tvArrKyung.Add(tvTu);
					else
						tvArrEtc.Add(tvTu);
				}

				long nCntSize = 0;
				if(tvArrTosa.GetSize() > 0)
					nCntSize++;
				if(tvArrPung.GetSize() > 0)
					nCntSize++;
				if(tvArrYun.GetSize() > 0)
					nCntSize++;
				if(tvArrKyung.GetSize() > 0)
					nCntSize++;
				if(tvArrEtc.GetSize() > 0)
					nCntSize++;

				BOOL bMeanLessSameType = nCntSize > 1? FALSE : TRUE;
				rb.SeperatorTwinVectorArea(tvArr, tvArrResult, dArrAngleSu, 0.0, FALSE, bMeanLessSameType);
			}

			long j = 0; for(j = 0; j < dArrAngleSu.GetSize(); j++)
			{
				CTwinVectorAreaShape *pArea	= new CTwinVectorAreaShape(j+1);
				for(long k = 0; k < (long)dArrAngleSu.GetAt(j); k++)
				{
					if(bGooBae )
						pArea->AddTwinVector(tvArrResultGooBae.GetAt(nIdx++));
					else
						pArea->AddTwinVector(tvArrResult.GetAt(nIdx++));
				}

				pArea->Calculate();
				pArrAreaConc[vert].Add(pArea);
			}
		}
		//////////////////////////////////////////////////////////////////////////////////////////////

		// 미터별로 나눈다 ///////
		//double dYBase		= 0;// 32223
		double dFirstMeter	= 0;
		double dSeconeMeter	= 0;
		GetInfoMeter(BM_BIG_TUPAGI, dFirstMeter, dSeconeMeter, pFooting);
		//dYBase	= tvArrVert.GetXyTop().y; // 32223
		baseBM.RemakeByHeightTvAreaShape(&pArrAreaConc[0], dYBase, dFirstMeter, dSeconeMeter);
		baseBM.RemakeByHeightTvAreaShape(&pArrAreaConc[1], dYBase, dFirstMeter, dSeconeMeter);

		// 수중 터파기 지원시 한번 더 자름
		baseBM.RemakeByHeightTvAreaShape(&pArrAreaConc[0], dYBase, pFooting->m_dEL_DiggingWater - dYBase, -tvArrVert.GetVerLength());
		baseBM.RemakeByHeightTvAreaShape(&pArrAreaConc[1], dYBase, pFooting->m_dEL_DiggingWater - dYBase, -tvArrVert.GetVerLength());
		///////////////////////////
		
		// 종단과 횡단은 어짜피 토공영역의 개수가 같아야 계산이 되므로 혹시라도 다르게 나오면 계산안함.
		if(pArrAreaConc[0].GetSize() != pArrAreaConc[1].GetSize()) continue;

		// 수량 산출
		long nSizeBM	= pArrBM->GetSize();
		long nSize		= pArrAreaConc[0].GetSize();
		long i = 0; for(i = 0; i < nSize; i++)
		{
			CTwinVectorAreaShape *pArea[2]	= {pArrAreaConc[0].GetAt(i), pArrAreaConc[1].GetAt(i)};
			
			double dWidthUpper[2]	= {toM(pArea[0]->m_dWidthUpper), toM(pArea[1]->m_dWidthUpper)};
			double dWidthLower[2]	= {toM(pArea[0]->m_dWidthLower), toM(pArea[1]->m_dWidthLower)};
			double dHeight			= toM(pArea[0]->m_dHeight);
						
			// 보링 타입에 따른 이름
			long nMidClass	= 0;
			CTwinVector tvLeft	= pArea[0]->GetTvLeft();

			BOOL bWaterTyopagi	= Compare(tvLeft.GetXyTop().y, pFooting->m_dEL_DiggingWater, "<=");
			
			if(tvLeft.m_sInfo.Find("토사", 0) != -1) nMidClass = BM_MID_TUPAGI_TOSA;
			else if(tvLeft.m_sInfo.Find("풍화암", 0) != -1) nMidClass = BM_MID_TUPAGI_PUWHAAM;
			else if(tvLeft.m_sInfo.Find("연암", 0) != -1) nMidClass = BM_MID_TUPAGI_YUNAM;
			else if(tvLeft.m_sInfo.Find("경암", 0) != -1) nMidClass = BM_MID_TUPAGI_GYUNGAM;
			else nMidClass	= BM_MID_TUPAGI_TOSA;
		
			
			CBMData *pBM				= new CBMData;
			pBM->m_nIdxBigClass			= bWaterTyopagi ? BM_BIG_TUPAGI_WATER : BM_BIG_TUPAGI;		
			pBM->m_sBigClass			= GetStringBigClass(pBM->m_nIdxBigClass);
			pBM->m_nLevelTotal			= GetLevelTotal(pBM->m_nIdxBigClass);
			pBM->m_nIdxMidClass			= nMidClass;			
			pBM->m_sMidClass			= GetStringMidClass(pBM->m_nIdxMidClass);
			if(bWaterTyopagi)
				pBM->m_sMidClass	= " " + pBM->m_sMidClass + " ";
			pBM->m_nTypeResult			= RESULT_VOLUMN;		
			pBM->m_sUnit				= "㎥";
			pBM->m_dV[SttMeter]			= Round(toM(tvLeft.GetXyTop().y),3);
			pBM->m_dV[EndMeter]			= Round(toM(tvLeft.GetXyBottom().y),3);
			pBM->m_nShape				= SHAPE_TRAPEZOID;
			// 양단면 평균법으로 구함(실제로 맞지는 않음)
			// 정확히 하려면 각뿔공식 이용해야 하나 복잡하고, 설계사에서 실제로 사용하지 않으므로
			// 좀 안 맞더라도 양단면 평균법을 사용함.
			pBM->m_sForm.Format("(((%.3f x %.3f) + (%.3f x %.3f)) / 2) x %.3f", dWidthUpper[0], dWidthUpper[1], dWidthLower[0], dWidthLower[1], dHeight);
			pBM->SetResult((((dWidthUpper[0]*dWidthUpper[1])+(dWidthLower[0]*dWidthLower[1]))/2)*dHeight);
			pBM->m_sVCN[Result]			= "터파기";
			pBM->m_bIsPrintByRefForm	= TRUE;
			pArrBM->Add(pBM);
		}

		// 수량에 미터정보를 생성해서 넣어준다
		RemakeByHeightBMList(pArrBM, dYBase, dFirstMeter, dSeconeMeter, nSizeBM-1, pArrBM->GetSize()-1);

		AhTPADelete(&pArrAreaConc[0],	(CTwinVectorAreaShape*)0);
		AhTPADelete(&pArrAreaConc[1],	(CTwinVectorAreaShape*)0);
	}

	AhTPADelete(&pArrAreaConc[0],	(CTwinVectorAreaShape*)0);
	AhTPADelete(&pArrAreaConc[1],	(CTwinVectorAreaShape*)0);

	pBri->GetTvVertSection_Std(0, TRUE, FALSE, TRUE, 1);
}

/**
	@brief
		물푸기 수량 추가
*/
void COutBMTogong::MakeBMWaterOut()
{
	CRcBridgeRebar *pBri	= GetRcBridgeRebar();
	if(!pBri) return;

	long jijum = 0; for(jijum = 0; jijum <= pBri->m_nQtyJigan; jijum++)
	{
		CFootingApp *pFooting	= pBri->GetFootingByJijumNumber(jijum);
		if(!pFooting) continue;
		if(!pFooting->m_bIs) continue;
		if(pBri->IsFootingJijum(jijum) == FALSE) continue;

		CTypedPtrArray <CObArray, CBMData*> *pArrBM	= &m_pArrBMTogongStd[jijum];


		double dWaterTupagiTosa	= 0;
		double dWaterTupaiOfPungwhaAm	= 0;
		double dWaterTupagOfYunAmGyungAm	= 0;

		// 수중 터파기 수량에서 물푸기 수량을 가져옴.
		for(HGINT32 ix = 0; ix < pArrBM->GetSize(); ++ix)
		{
			CBMData *pBM	= pArrBM->GetAt(ix);
			if(pBM->m_nIdxBigClass == BM_BIG_TUPAGI_WATER)
			{
				if(pBM->m_nIdxMidClass == BM_MID_TUPAGI_TOSA)
					dWaterTupagiTosa += pBM->GetResult();
				else if(pBM->m_nIdxMidClass == BM_MID_TUPAGI_PUWHAAM)
					dWaterTupaiOfPungwhaAm += pBM->GetResult();
				else 
					dWaterTupagOfYunAmGyungAm += pBM->GetResult();
			}
		}

		CMakeBMStd makeBMStd;
		makeBMStd.InitBMWaterOut(BM_BIG_WATEROUT);
		makeBMStd.MakeBMWaterOut(pArrBM, dWaterTupagiTosa, dWaterTupaiOfPungwhaAm, dWaterTupagOfYunAmGyungAm, pBri->m_dTimeConcForm);
	}
	
}

// 뒷채움
// 뒷채움 구한뒤
// 날개벽, 접속슬래브, 브라켓 공제해야 됨

// nForUse : 0 : 원래 뒷채움, 1 : 되메우기용 뒷채움 수량, 2 : 노상공제토용 뒷채움 수량, 3 : 노체공제토용 뒷채움 수량
// 1 (되메우기용) : 뒷채움 하부에서 지산선(터파기상면)까지
// 2 (노상공제토용) : 노상 상하부(하부한계는 지반선)
// 3 (노체공제토용) : 노체 상하부(하부한계는 지반선)
// 4 (뒷채움수량산출용) : 원래 뒷채움선에서 포장부분 뺀거.
// 공제토용으로 구할때는 시종점까지의 거리로 구하고, 구조물은 공제 하지 않는다.
// 왜냐하면, 공제토는 도로부에서 구한 토공량에서 구조부할당을 빼는 것이기 때문임.
void COutBMTogong::MakeBMBackFill(long nForUse)
{
	CRcBridgeRebar *pBri	= GetRcBridgeRebar();
	if(!pBri) return;
	pBri->GetTvVertSection_Std(0, FALSE, FALSE, FALSE, 1);
	CRebarPlacing rb;

	long nIdxStt[2] = {0, 0};
	long nIdxEnd[2] = {0, 0};
	CDPoint vAng = pBri->GetAngleJijum(0);
	long stt = 0; 
	for(stt = 0; stt < 2; stt++)
	{
		BOOL bStt	= stt == iSTT;
		if(!pBri->IsOutWall(bStt)) continue;	// 외측벽체가 없으면 뒷채움 없음

		//Box Type일때는 시점에서 모두 계산.
		long nJ	= bStt ? 0 : pBri->m_nQtyJigan;	 
		if(pBri->IsBoxType()) nJ = 0;
		CTypedPtrArray <CObArray, CBMData*> *pArrBM	= &m_pArrBMTogongStd[nJ];

		// 뒷채움 수량 구함
		// 브라켓상하의 직선이 수평선이므로 그냥 사다리꼴로 구하면 됨
		double dX	= bStt ? pBri->GetStationOnJijum(0, 0, TRUE)-pBri->m_dWS : pBri->GetStationOnJijum(pBri->m_nQtyJigan, 0, TRUE)+pBri->m_dWE;
		// 공제토는 시종점부부터 구함
		if(nForUse==2 || nForUse == 3 )
		{
			dX	= bStt ? pBri->GetStationOnJijum(0, 0, TRUE) : pBri->GetStationOnJijum(pBri->m_nQtyJigan, 0, TRUE);
		}

		CTwinVector tvBackFill		= pBri->GetTvBackFill(bStt, nForUse);
		double dHoriBackFill		= tvBackFill.GetHorLength();
		double dDistOffsetBackFill	= fabs(tvBackFill.m_v1.x - dX);
		
		//ARCBRIDGE-3080 가각을 포함하기로 함
		//double dLenBackFill			= pBri->GetWidthSlabAct(bStt ? pBri->m_dStationBridgeStt : pBri->m_dStationBridgeStt + pBri->m_dLengthBridge, vAng, 0, FALSE, FALSE);		
		CTwinVector tvSlab;
		pBri->GetTvWingWallPlane(bStt);
		pBri->m_tvArrPlaneWingWall.GetTvByInfo("슬래브끝단외측", tvSlab);
		double dLenBackFill = tvSlab.GetLength();

		//todo
		CFootingApp* pFooting = NULL;
		//박스교의 경우 시점기초가 종점기초 역활까지 한다.
		if(pBri->m_nTypeBridge != BRIDGE_TYPE_BOX)
			pFooting	= bStt ? &pBri->m_footingStt : &pBri->m_footingEnd;
		else 
			pFooting	= &pBri->m_footingBox;
		double dELBackFill = tvBackFill.GetXyTop().y;



		CBMData *pBM				= new CBMData;
		pBM->m_nIdxBigClass			= BM_BIG_BACKFILL;		
		pBM->m_sBigClass			= GetStringBigClass(pBM->m_nIdxBigClass);
		pBM->m_nIdxMidClass			= -1;
		pBM->m_sMidClass			= _T("");
		if(pBri->IsBoxType())
		{
			pBM->m_nIdxPartClass		= bStt ? 0 : 1;
			pBM->m_sPartClass	    	= bStt ? "시점" : "종점";
		}
		pBM->m_nLevelTotal			= GetLevelTotal(pBM->m_nIdxBigClass);
		pBM->m_nIdxItemClass		= BM_ITEM_BACKFILL;
		pBM->m_sItemClass			= GetStringItemClass(pBM->m_nIdxItemClass);
		pBM->m_nTypeResult			= RESULT_VOLUMN;		
		pBM->m_sUnit				= "㎥";
		pBM->m_nShape				= SHAPE_TRAPEZOID;
		pBM->m_dV[WidthUpper]		= Round(toM(tvBackFill.GetHorLength()+dDistOffsetBackFill),3);
		pBM->m_dV[WidthLower]		= Round(toM(dDistOffsetBackFill),3);
		pBM->m_dV[Height]			= Round(toM(tvBackFill.GetVerLength()),3);
		pBM->m_dV[Length]			= Round(toM(dLenBackFill),3);

		pBM->CalculateResult();
		pArrBM->Add(pBM);

		if(!pBri->IsBoxType())
		{
			nIdxStt[stt]	= pArrBM->GetSize();
			nIdxEnd[stt]	= nIdxStt[stt];
		}
		else
		{
			nIdxStt[0]	= pArrBM->GetSize();
			nIdxEnd[0]	= nIdxStt[0];
		}
		
	}
	
	// 공제토용으로 구할때는 시종점까지의 거리로 구하고, 구조물은 공제 하지 않는다.
	// 왜냐하면, 공제토는 도로부에서 구한 토공량에서 구조부할당을 빼는 것이기 때문임.
	if(nForUse == 2 || nForUse == 3) 
	{
		pBri->GetTvVertSection_Std(0, FALSE, FALSE, TRUE, 1);
		return;
	}


	// 1. 날개벽 공제(COutBM에 있는 날개벽 콘크리트 양 구하는 함수에서 공제용으로 수량을 뽑아와서 수정함)
	// 공제용으로 영역 구한 날개벽의 콘크리트 수량 가져옴
	COutBM *pOutBM	= m_pStd->m_pArrOutBMNormal.GetAt(m_nBri);
	pOutBM->MakeBMConcreteWing(nForUse+1);

	for(stt = 0; stt < 2; stt++)
	{
		BOOL bStt	= stt == iSTT;
		if(pBri->IsBoxType() && !bStt) continue;
		long nJ	= bStt ? 0 : pBri->m_nQtyJigan;
		CTypedPtrArray <CObArray, CBMData*> *pArrBM	= &m_pArrBMTogongStd[nJ];

		// 공제용 수량에 맞게 수정함
		nIdxEnd[stt]	= pArrBM->GetSize()-1;
		
		if(nIdxStt[stt] > -1 && nIdxEnd[stt] > nIdxStt[stt])
		{
			long i = 0; for(i = nIdxStt[stt]; i <= nIdxEnd[stt]; i++)
			{
				CBMData *pBM			= pArrBM->GetAt(i);
				pBM->m_nIdxBigClass		= BM_BIG_BACKFILL;
				pBM->m_sBigClass		= GetStringBigClass(pBM->m_nIdxBigClass);
				pBM->m_nIdxMidClass			= -1;
				pBM->m_sMidClass			= _T("");
				pBM->m_nIdxSmallClass = 0;

				pBM->m_nIdxSmallClass	= -1;
				pBM->m_sSmallClass		= _T("");
				pBM->m_nLevelTotal		= GetLevelTotal(pBM->m_nIdxBigClass);
				pBM->m_nIdxItemClass	= BM_ITEM_BACKFILL_WING;

				pBM->m_sItemClass		= GetStringItemClass(pBM->m_nIdxItemClass);
				pBM->m_bIsUseForDeduct	= !pBM->m_bIsUseForDeduct;
				pBM->CalculateResult();
			}
		}
	}

	// 브라켓과 접속슬래브는 되메우기 용 공제일 때는 공제하지 않는다.
	// 정확하게 구해지면 그때 공제함
	if(1)//nForUse == 0)
	{
		// 2. 브라켓 공제
		for(stt = 0; stt < 2; stt++)
		{
			BOOL bStt	= stt == iSTT;
			long nJ	= bStt ? 0 : pBri->m_nQtyJigan;
			if(pBri->IsBoxType()) nJ = 0;
			CTypedPtrArray <CObArray, CBMData*> *pArrBM	= &m_pArrBMTogongStd[nJ];

			if(pBri->IsOutWall(bStt))
			{
				CTwinVector tvBackFill		= pBri->GetTvBackFill(bStt, nForUse);
				
				CTwinVector tvBracket;
				pBri->GetTvWingWallPlane(bStt, TRUE);
				
				if(pBri->m_tvArrPlaneWingWall.GetTvByInfo("브라켓", tvBracket))
				{
					CTwinVectorArray tvArrBracket;
					pBri->GetTvVertSectionBracket(tvArrBracket, bStt);
					CDRect rect	= tvArrBracket.GetRect();
					rect.top	= tvBackFill.GetXyTop().y;
					rect.bottom	= tvBackFill.GetXyBottom().y;
					rect.left -= 10;
					rect.right += 10;
					
					CTwinVectorArray tvArrRect;
					CTwinVectorArray tvArrResult;
					tvArrRect.SetFromRect(rect);
					rb.SubTractTvArrAndTvArr(tvArrBracket, tvArrRect, tvArrResult);

					if(tvArrResult.GetSize() == 0) continue;

					CTwinVectorAreaShape area;
					area.m_tvArr	= tvArrResult;
					area.Calculate();

					CBMData *pBM	= new CBMData;
					pBM->m_nIdxBigClass			= BM_BIG_BACKFILL;		
					pBM->m_sBigClass			= GetStringBigClass(pBM->m_nIdxBigClass);
					pBM->m_nLevelTotal			= GetLevelTotal(pBM->m_nIdxBigClass);
					pBM->m_nIdxMidClass			= -1;
					pBM->m_sMidClass			= _T("");
					pBM->m_nIdxSmallClass = 1;
					if(pBri->IsBoxType())
					{
						pBM->m_nIdxPartClass		= bStt ? 0 : 1;
						pBM->m_sPartClass	    	= bStt ? "시점" : "종점";
					}
					pBM->m_nIdxItemClass		= BM_ITEM_BACKFILL_BRACKET;
					pBM->m_sItemClass			= GetStringItemClass(pBM->m_nIdxItemClass);
					pBM->m_nTypeResult			= RESULT_VOLUMN;		
					pBM->m_sUnit				= "㎥";
					pBM->m_nShape				= area.m_nTypeShape;
					pBM->m_dV[Width]			= Round(toM(area.m_dWidth),3);
					pBM->m_dV[WidthLower]		= Round(toM(area.m_dWidthLower),3);
					pBM->m_dV[WidthUpper]		= Round(toM(area.m_dWidthUpper),3);
					pBM->m_dV[WidthDiffLeft]	= Round(toM(area.m_dWidthDiffLeft),3);
					pBM->m_dV[WidthDiffRight]	= Round(toM(area.m_dWidthDiffRight),3);
					pBM->m_dV[Height]			= Round(toM(area.m_dHeight),3);
					pBM->m_dV[HeightLeft]		= Round(toM(area.m_dHeightLeft),3);
					pBM->m_dV[HeightRight]		= Round(toM(area.m_dHeightRight),3);
					pBM->m_dV[Length]			= Round(toM(tvBracket.GetLength()),3);
					pBM->m_bIsUseForDeduct		= TRUE;
					pBM->CalculateResult();
					pArrBM->Add(pBM);
				}
			}

			if(!pBri->IsBoxType())
			{
				nIdxStt[stt]	= pArrBM->GetSize();
				nIdxEnd[stt]	= nIdxStt[stt];
			}
			else
			{
				nIdxStt[0]	= pArrBM->GetSize();
				nIdxEnd[0]	= nIdxStt[stt];
			}
		}

		// 3. 접속슬래브 공제
		// 공제용으로 영역 구한 날개벽의 콘크리트 수량 가져옴
		COutBM *pOutBM	= m_pStd->m_pArrOutBMNormal.GetAt(m_nBri);
		//pOutBM->MakeRcBridgeForBM();
		// 위에서 이미 변경한상태이므로 다른상태로 변경을 하기 위해서는
		// 복구를 하고 바꿔야 한다
		pBri->GetTvVertSection_Std(0, FALSE, FALSE, TRUE, 1);
		pBri->GetTvVertSection_Std(0, FALSE, FALSE, FALSE, 1);

		pBri->SyncBridgeAps(0, FALSE);
		CTwinVector tvBackFill[2]	= {pBri->GetTvBackFill(TRUE, nForUse), pBri->GetTvBackFill(FALSE, nForUse)};
		pOutBM->MakeBMConcreteAps(TRUE, tvBackFill[0], tvBackFill[1]);
		for(stt = 0; stt < 2; stt++) // 접속슬래브 콘크리트 구하는 함수는 시종점 모두 구하므로 여기서는 한번만 호출 하면 됨
		{
			BOOL bStt	= stt == iSTT;
			if(pBri->IsBoxType() && !bStt) continue;
			long nJ	= bStt ? 0 : pBri->m_nQtyJigan;
			CTypedPtrArray <CObArray, CBMData*> *pArrBM	= &m_pArrBMTogongStd[nJ];

			// 원래는 날개벽처럼 정확하게 공제토라인으로 잘리는 부분을 검사해야 되지만, 
			// 일단은 그냥 접속슬래브 콘크리트양과 동일하게 구함.(거의 그렇게 뭍힐꺼기 때문임)
			// 1. 날개벽 공제(COutBM에 있는 날개벽 콘크리트 양 구하는 함수에서 공제용으로 수량을 뽑아와서 수정함)
	
			// 공제용 수량에 맞게 수정함
			nIdxEnd[stt]	= pArrBM->GetSize()-1;
			long i = 0; for(i = nIdxStt[stt]; i <= nIdxEnd[stt]; i++)
			{
				CBMData *pBM			= pArrBM->GetAt(i);
				pBM->m_nIdxBigClass		= BM_BIG_BACKFILL;
				pBM->m_sBigClass		= GetStringBigClass(pBM->m_nIdxBigClass);
				pBM->m_nIdxMidClass			= -1;
				pBM->m_sMidClass			= _T("");
				pBM->m_nIdxSmallClass	= -1;
				pBM->m_sSmallClass		= _T("");
				pBM->m_nLevelTotal		= GetLevelTotal(pBM->m_nIdxBigClass);
				pBM->m_nIdxItemClass	= BM_ITEM_BACKFILL_APS;
				pBM->m_sItemClass		= GetStringItemClass(pBM->m_nIdxItemClass);
				pBM->m_bIsUseForDeduct	= !pBM->m_bIsUseForDeduct;
				if(pBM->m_nShape != -1)
					pBM->CalculateResult();
			}
		}
		pBri->GetTvVertSection_Std(0, FALSE, FALSE, TRUE);
	}

	pBri->GetTvVertSection_Std(0, FALSE, FALSE, TRUE, 1);
}

// 뒷채움용 삽도 삽입
// 1. 날개벽 공제
void COutBMTogong::SetInsertDomyunBackFill()
{
	// 날개벽과 토공 관련해서는 원래 교량을 사용한다.
	// 평균높이 구할 필요 없고, 날개벽 EL을 정확하게 하기 위해서임.
	CRcBridgeRebar	*pBri	= GetRcBridgeRebar();	
	if(!pBri) return;

	CHgBaseBMStd baseBMStd;
	CHgBaseDrawStd baseDraw;
	CARcBridgeDBStandard DBStd(pBri, m_pStd->m_pARoadOptionStd, m_pStd->m_pARcBridgeDataStd);
	DBStd.m_bApplyDimSettingOfBaseDom	= TRUE;

	double dWidthOrg	= 350;
	COutBM *pOutBM	= m_pStd->m_pArrOutBMNormal.GetAt(m_nBri);
	pOutBM->CalAreaConcWing(pBri, 5);	// 뒷채움 공제용 날개벽 영역 구함.

	BOOL bDrawRubble = pBri->IsBoxType() && pBri->m_dThickCobbStone > 0 && pBri->m_nTypeLower == LOWERSLAB_TYPE_COBBSTONE;
	// 뒷채움에서 날개벽 영역만 그림
	long stt = 0; for(stt = 0; stt < 2; stt++)
	{
		BOOL bStt	= stt == iSTT;
		double dY	= 0;
		long nJ	= bStt ? 0 : pBri->m_nQtyJigan;
		if(!pBri->IsWingWall(bStt, TRUE) && !pBri->IsWingWall(bStt, FALSE)) continue;
		pBri->GetTvVertSection_Std(0, pBri->IsBoxType() ? FALSE : TRUE, FALSE, FALSE, 2);

		// 스케일 조정용 /////////////
		CDomyun domTmp;
		domTmp.SetCWnd(m_pStd->m_pView);
		
		
		double dCutXPos = 0;
		//단면 Box자를 좌표
		if(pBri->IsBoxType())
			dCutXPos = bStt ? pBri->GetStationOnJijum(0) + 3000 : pBri->GetStationOnJijum(pBri->m_nQtyJigan) - 3000;

		// 종단면 그림
	
		// 벽체
		DBStd.m_pDrawVertSection->DrawPartByJijum(&domTmp, nJ, FALSE);
		// 뒷채움선
		DBStd.m_pDrawVertSection->DrawTogongLine(&domTmp, pBri, nJ, FALSE, FALSE, TRUE, FALSE, TRUE, TRUE);
		DBStd.m_pDrawVertSection->DimTogongLine(&domTmp, pBri, nJ, FALSE, TRUE, FALSE, TRUE);


		CDRect rect = domTmp.GetExtRect();
		//박스형일경우 뒷채움선이 시점 종점까지 다 포함되므로 적당히 잘라주자
		if(pBri->IsBoxType())
		{
			if(bStt) rect.right = dCutXPos;
			else	rect.left = dCutXPos;
			domTmp.TrimByRect(rect, FALSE);
		}

		// 스케일 조정용 /////////////
		
		// 콘크리트 산출시 기초 산출전에 기초형태가 날개벽따라 꺽이는거 추가
		CDomyun domWing;
		domWing.SetCWnd(m_pStd->m_pView);

		// 도면 설정 ////////////////////
		double dScale	= baseBMStd.GetScale(&domTmp, dWidthOrg);
		baseBMStd.SetDomyunValue(&domWing, dWidthOrg, &domTmp);
		///////////////////////////////////

		// 뒷채움 선 그림
		CDomyun domBack(&domWing);

		//단면
		if(pBri->IsBoxType())
		{
			DBStd.m_pDrawVertSection->DrawVertSection(&domBack, TRUE);
			if(pBri->IsBoxType() && pBri->m_dThickCobbStone > 0 && pBri->m_nTypeLower == LOWERSLAB_TYPE_COBBSTONE)
				DBStd.m_pDrawVertSection->DrawExFootingBoxType(&domTmp, TRUE);
		}
		// 벽체
		DBStd.m_pDrawVertSection->DrawPartByJijum(&domBack, nJ, FALSE);
		// 뒷채움선
		DBStd.m_pDrawVertSection->DrawTogongLine(&domBack, pBri, nJ, FALSE, FALSE, TRUE, FALSE, TRUE, TRUE);
		DBStd.m_pDrawVertSection->DimTogongLine(&domBack, pBri, nJ, FALSE, TRUE, FALSE, TRUE);

		rect	= domBack.GetExtRect();
		//박스형일경우 뒷채움선이 시점 종점까지 다 포함되므로 적당히 잘라주자
		if(pBri->IsBoxType())
		{
			if(bStt) rect.right = dCutXPos;
			else	rect.left = dCutXPos;;
			domBack.TrimByRect(rect, FALSE);
			domBack.CutLightning(dCutXPos, rect.bottom, dCutXPos, rect.top);
		}
		// 삽도를 WidthOrg 사이즈로 변경
		domBack.RedrawByScale(dScale);
		domBack.SetCalcExtRect();

		rect	= domBack.GetExtRect();
		// 정해진 위치로 옮김
		domBack.Move(CDPoint(-rect.left + dWidthOrg/2, dY));
		domBack.SetCalcExtRect();
		
		domWing << domBack;
		pBri->GetTvVertSection_Std(0, TRUE, TRUE, TRUE, 2);

		dY -= (rect.Height() + 30);
		// 좌우측 날개벽 공제 영역 그림
		for(long left = 0; left < 2; left++)
		{
			BOOL bLeft	= left == iLEFT;
			if(!pBri->IsWingWall(bStt, bLeft)) continue;
			CWingWall *pWing	= pBri->GetWingWall(bStt, bLeft);
			if(!pWing) continue;
			if(pWing->m_pArrAreaConc.GetSize() == 0) continue;
			
			CDomyun dom(&domWing);

			// 날개벽 영역 (뒷채움시 공제되는 영역만)
			int se = bStt ? -1 : 1;
			int nLeft = bLeft ? -se : se;	// 시점일 경우 교량 진행 방향의 좌우와 날개벽의 좌우가 반대임.

			pBri->GetTvVertSection_Std(nLeft, TRUE, TRUE, FALSE, 2);

			// 날개벽
			CTwinVectorArray tvArrGen;
			pBri->GetTvWingWallFront(bStt, bLeft, 1, tvArrGen, FALSE, FALSE, TRUE, TRUE);
			baseDraw.DrawTvArray(&dom, tvArrGen, FALSE, FALSE);

			if(pWing->m_nAttachPos == 0)
				DBStd.m_pDrawVertSection->DrawPartByJijum(&dom, nJ, FALSE);
			///////////////박스형일떄 토공라인 한쪽만----------------------
			DBStd.m_pDrawVertSection->DrawTogongLine(&dom, pBri, nJ, FALSE, FALSE, TRUE, FALSE, FALSE, TRUE);
			baseBMStd.DrawAreaSectionNumber(&dom, &pWing->m_pArrAreaConc, 1, TRUE);
			baseBMStd.DimAreaSection(&dom, &pWing->m_pArrAreaConc, 1, TRUE, CDPoint(0, 0));
			baseBMStd.DimAreaSection(&dom, &pWing->m_pArrAreaConc, bStt ? 2 : 0, TRUE, CDPoint(0, 0), FALSE);
			baseBMStd.DimAreaSection(&dom, &pWing->m_pArrAreaConc, 3, TRUE, CDPoint(0, 0));

			// 영역 1,2에 대해서만 따로 치수 기입(날개벽 L2가 있는 경우에만)
			if(pWing->m_dL2 > 0)
			{

			}
	
			dom.RedrawByScale(dScale);

			dom.SetCalcExtRect();
			CDRect rectWing	= dom.GetExtRect();

			// 정해진 위치로 옮김
			dom.Move(CDPoint(-rectWing.left + (bLeft ? 10 : dWidthOrg+40), dY));
			dom.SetCalcExtRect();

			domWing << dom;
			pBri->GetTvVertSection_Std(nLeft, TRUE, TRUE, TRUE, 2);
		}

		// 뒷채움수량 이전에 삽입
		long nIdx	= bStt ? 0 : pBri->m_nQtyJigan;
		if(pBri->IsBoxType()) nIdx = 0;
		long i = 0; for(i = 0; i < m_pArrBMStringTogong[nIdx].GetSize(); i++)
		{
			CBMString *pBMString			= m_pArrBMStringTogong[nIdx].GetAt(i);

			if(!pBri->IsBoxType())
			{
				if(pBMString->m_sProcessKind1.Find("뒷채움", 0) != -1)
				{
					pBMString->m_Dom			= domWing;
					pBMString->m_bInsertDomyun	= TRUE;
						break;
				}
			}
			//Box형일때는 삽도 두개가 들어감(뒷채움시점앞, 종점앞)
			else
			{
				if(bStt && pBMString->m_sForm == "(시점)")
				{
					pBMString->m_Dom			= domWing;
					pBMString->m_bInsertDomyun	= TRUE;
					break;
				}

				if(!bStt && pBMString->m_sForm == "(종점)")
				{
					pBMString->m_Dom			= domWing;
					pBMString->m_bInsertDomyun	= TRUE;
					break;
				}
			}
		}	
	}

	DBStd.m_bApplyDimSettingOfBaseDom	= TRUE;
}

// 되메우기 = 터파기 - (지반선이하의 구체 + 지반선이하의 뒷채움)
void COutBMTogong::MakeBMReFill()
{
	CRcBridgeRebar	*pBri	= GetRcBridgeRebar();
	if(!pBri) return;
	pBri->GetTvVertSection_Std(0, FALSE, FALSE, FALSE, 1);

	long nIdxStt[MAX_JIJUM_COUNT_RC];
	long nIdxEnd[MAX_JIJUM_COUNT_RC];

	long jijum = 0; for(jijum = 0; jijum <= pBri->m_nQtyJigan; jijum++)
	{
		if(pBri->IsFootingJijum(jijum) == FALSE) continue;
		CTypedPtrArray <CObArray, CBMData*> *pArrBM	= &m_pArrBMTogongStd[jijum];

		// 터파기 가져옴 ////////////
		double dTypagi = 0;
		long nSize	= pArrBM->GetSize();
		long i = 0; for(i = 0; i < nSize; i++)
		{
			CBMData *pBM = pArrBM->GetAt(i);
			if(pBM->m_sBigClass == "터파기" || pBM->m_sBigClass == "수중터파기" || pBM->m_sBigClass == "용수터파기")
				dTypagi	+= pBM->GetResult();
		}

		if(dTypagi > 0)
		{
			CBMData *pBM			= new CBMData;
			pBM->m_nIdxBigClass		= BM_BIG_REFILL;
			pBM->m_sBigClass		= GetStringBigClass(pBM->m_nIdxBigClass);
			pBM->m_nLevelTotal		= GetLevelTotal(pBM->m_nIdxBigClass);
			pBM->m_nIdxItemClass	= BM_ITEM_REFILL_TYPAGI;
			pBM->m_sItemClass		= GetStringItemClass(pBM->m_nIdxItemClass);
			pBM->m_nTypeResult		= RESULT_VOLUMN;		
			pBM->m_sUnit			= "㎥";
			pBM->m_sNoteHead		= "되메우기 = 터파기 - (지반선 이하의 구체 및 뒷채움)";
			pBM->m_sForm.Format("%.3f", dTypagi);
			pBM->m_sFormExcel.Format("#터파기:%.3f#", dTypagi);
			pBM->SetResult(dTypagi);
			pBM->m_sVCN[Result]		= "되메우기";
			pBM->m_bIsPrintByRefForm	= TRUE;
			pArrBM->Add(pBM);
		}

		nIdxStt[jijum]	= pArrBM->GetSize();
		nIdxEnd[jijum]	= nIdxStt[jijum];
	}
	
	// 1. 지반선(터파기 상면 수평선)이하의 구체 공제(각 지점별로 다 있다)
	// 공제용으로 영역 구한 구체의 콘크리트 수량 가져옴
	COutBM *pOutBM	= m_pStd->m_pArrOutBMNormal.GetAt(m_nBri);
	pOutBM->MakeBMConcreteMain(TRUE);
	
	for(jijum = 0; jijum <= pBri->m_nQtyJigan; jijum++)
	{
		if(pBri->IsFootingJijum(jijum) == FALSE) continue;
		CTypedPtrArray <CObArray, CBMData*> *pArrBM	= &m_pArrBMTogongStd[jijum];
		
		// 되메우기 없으면 공제도 없음
		BOOL bReFill	= FALSE;
		long j = 0; for(j = 0; j < pArrBM->GetSize(); j++)
		{
			CBMData *pBM	= pArrBM->GetAt(j);
			if(pBM->m_sBigClass.Find("되메우기", 0) != -1) 
			{
				bReFill	= TRUE;
				break;
			}
		}
		if(!bReFill) continue;

		// 공제용 수량에 맞게 수정함
		nIdxEnd[jijum] = pArrBM->GetSize();
		long i = 0; for(i = nIdxStt[jijum]; i < nIdxEnd[jijum]; i++)
		{
			CBMData *pBM			= pArrBM->GetAt(i);
			pBM->m_nIdxBigClass		= BM_BIG_REFILL;
			pBM->m_sBigClass		= GetStringBigClass(pBM->m_nIdxBigClass);
			pBM->m_nLevelTotal		= GetLevelTotal(pBM->m_nIdxBigClass);
			pBM->m_nIdxMidClass		= -1;
			pBM->m_sMidClass		= _T("");
			pBM->m_nIdxSmallClass	= -1;
			pBM->m_sSmallClass		= _T("");
			pBM->m_nIdxItemClass	= BM_ITEM_REFILL_MAIN;
			pBM->m_sItemClass		= GetStringItemClass(pBM->m_nIdxItemClass);
			pBM->m_bIsUseForDeduct	= !pBM->m_bIsUseForDeduct;
			pBM->CalculateResult();
			pBM->m_sVCN[Result]		= "되메우기";
			pBM->m_bIsPrintByRefForm	= TRUE;
		}	

		nIdxStt[jijum]	= pArrBM->GetSize();
		nIdxEnd[jijum]	= nIdxStt[jijum];
	}
	
	pOutBM->MakeBMConcreteWing(6);	// 터파기 내의 날개벽 벽체 공제
	
	for(jijum = 0; jijum <= pBri->m_nQtyJigan; jijum++)
	{
		if(pBri->IsFootingJijum(jijum) == FALSE) continue;
		CTypedPtrArray <CObArray, CBMData*> *pArrBM	= &m_pArrBMTogongStd[jijum];
		
		// 되메우기 없으면 공제도 없음
		BOOL bReFill	= FALSE;
		long j = 0; for(j = 0; j < pArrBM->GetSize(); j++)
		{
			CBMData *pBM	= pArrBM->GetAt(j);
			if(pBM->m_sBigClass.Find("되메우기", 0) != -1) 
			{
				bReFill	= TRUE;
				break;
			}
		}
		if(!bReFill) continue;

		// 공제용 수량에 맞게 수정함
		nIdxEnd[jijum] = pArrBM->GetSize();
		if(!bReFill) 
		{
			long j = 0; for(j = nIdxStt[jijum]; j < nIdxEnd[jijum]; j++)
			{
				delete pArrBM->GetAt(nIdxStt[jijum]);
				pArrBM->RemoveAt(nIdxStt[jijum]);
			}
			continue;
		}
		long i = 0; for(i = nIdxStt[jijum]; i < nIdxEnd[jijum]; i++)
		{
			CBMData *pBM			= pArrBM->GetAt(i);
			pBM->m_nIdxBigClass		= BM_BIG_REFILL;
			pBM->m_sBigClass		= GetStringBigClass(pBM->m_nIdxBigClass);
			pBM->m_nLevelTotal		= GetLevelTotal(pBM->m_nIdxBigClass);
			pBM->m_nIdxMidClass		= -1;
			pBM->m_sMidClass		= _T("");
			pBM->m_nIdxSmallClass	= -1;
			pBM->m_sSmallClass		= _T("");
			pBM->m_nIdxItemClass	= BM_ITEM_REFILL_MAIN;
			pBM->m_sItemClass		= GetStringItemClass(pBM->m_nIdxItemClass);
			pBM->m_bIsUseForDeduct	= !pBM->m_bIsUseForDeduct;
			pBM->CalculateResult();
			pBM->m_sVCN[Result]		= "되메우기";
			pBM->m_bIsPrintByRefForm	= TRUE;
		}	

		nIdxStt[jijum]	= pArrBM->GetSize();
		nIdxEnd[jijum]	= nIdxStt[jijum];
	}

	// 2. 지반선(터파기 상면 수평선)이하의 뒷채움 공제
	// 공제용으로 영역 구한 뒷채움 수량 가져옴

	// MakeBMBackFill(1); 에서도 GetTvVertSection_Std를 호출하므로 보관용 데이타가 바뀌어 버린다.
	// 원래대로 복구시켰다가 다시 GetTvVertSection_Std를 호출하자.
	pBri->GetTvVertSection_Std(0, FALSE, FALSE, TRUE, 1);
	MakeBMBackFill(1);
	pBri->GetTvVertSection_Std(0, FALSE, FALSE, FALSE, 1);
	
	for(jijum = 0; jijum <= pBri->m_nQtyJigan; jijum++)
	{
		if(pBri->IsFootingJijum(jijum) == FALSE) continue;
		CTypedPtrArray <CObArray, CBMData*> *pArrBM	= &m_pArrBMTogongStd[jijum];

		// 되메우기 없으면 공제도 없음
		BOOL bReFill	= FALSE;
		long j = 0; for(j = 0; j < pArrBM->GetSize(); j++)
		{
			CBMData *pBM	= pArrBM->GetAt(j);
			if(pBM->m_sBigClass.Find("되메우기", 0) != -1) 
			{
				bReFill	= TRUE;
				break;
			}
		}

		// 공제용 수량에 맞게 수정함
		nIdxEnd[jijum]	= pArrBM->GetSize();
		if(!bReFill) 
		{
			long j = 0; for(j = nIdxStt[jijum]; j < nIdxEnd[jijum]; j++)
			{
				delete pArrBM->GetAt(nIdxStt[jijum]);
				pArrBM->RemoveAt(nIdxStt[jijum]);
			}
			continue;
		}
		long i = 0; for(i = nIdxStt[jijum]; i < nIdxEnd[jijum]; i++)
		{
			CBMData *pBM			= pArrBM->GetAt(i);
			pBM->m_nIdxBigClass		= BM_BIG_REFILL;
			pBM->m_sBigClass		= GetStringBigClass(pBM->m_nIdxBigClass);
			pBM->m_nLevelTotal		= GetLevelTotal(pBM->m_nIdxBigClass);
			pBM->m_nIdxMidClass		= -1;
			pBM->m_sMidClass		= _T("");
			pBM->m_nIdxSmallClass	= -1;
			pBM->m_sSmallClass		= _T("");
			pBM->m_nIdxItemClass	= BM_ITEM_REFILL_BACKFILL;
			pBM->m_sItemClass		= GetStringItemClass(pBM->m_nIdxItemClass);
			pBM->m_bIsUseForDeduct	= !pBM->m_bIsUseForDeduct;
			pBM->m_sVCN[Result]		= "되메우기";
			if(pBM->m_nShape != -1)
			{
				pBM->CalculateResult();
				pBM->m_bIsPrintByRefForm	= TRUE;
			}
			else
				pBM->m_bIsPrintByRefForm	= FALSE;
		}	
	}

	pBri->GetTvVertSection_Std(0, FALSE, FALSE, TRUE, 1);
}

// 유용토
// 터파기 - 되메우기
void COutBMTogong::MakeBMValuableSoil()
{
	CRcBridgeRebar	*pBri	= GetRcBridgeRebar();
	if(!pBri) return;
	
	long jijum = 0; for(jijum = 0; jijum <= pBri->m_nQtyJigan; jijum++)
	{
		if(pBri->IsFootingJijum(jijum) == FALSE) continue;
		CTypedPtrArray <CObArray, CBMData*> *pArrBM	= &m_pArrBMTogongStd[jijum];

		// 터파기와 되메우기 가져옴
		double dTypagi	= 0;
		double dReFill	= 0;
		long nSize	= pArrBM->GetSize();
		long i = 0; for(i = 0; i < nSize; i++)
		{
			CBMData *pBM	= pArrBM->GetAt(i);
			if(pBM->m_sBigClass == "터파기" || pBM->m_sBigClass == "수중터파기" || pBM->m_sBigClass == "용수터파기")
				dTypagi	+= pBM->GetResult();
			else if(pBM->m_sBigClass == "되메우기")
				dReFill	+= pBM->GetResult();
		}

		CBMData *pBM			= new CBMData;
		pBM->m_nIdxBigClass		= BM_BIG_VALUABLE_SOIL;
		pBM->m_sBigClass		= GetStringBigClass(pBM->m_nIdxBigClass);
		pBM->m_nLevelTotal		= GetLevelTotal(pBM->m_nIdxBigClass);
		pBM->m_nTypeResult		= RESULT_VOLUMN;		
		pBM->m_sUnit			= "㎥";
		pBM->m_sNoteHead		= "유용토 = 터파기 - 되메우기";
		pBM->m_sForm.Format("%.3f - %.3f", dTypagi, dReFill);
		pBM->m_sFormExcel.Format("#터파기:%.3f# - #되메우기:%.3f#", dTypagi, dReFill);
		pBM->SetResult(dTypagi - dReFill);
		pBM->m_bIsPrintByRefForm	= TRUE;
		pArrBM->Add(pBM);
	}
}

// 공제토
// 노상, 노체
// 노상과 노체에 해당하는 부분의 뒷채움 수량을 구하면 된다.
// 노상이나 노체가 지반보다 아래쪽에 있다면 노상이나 노체는 없음
void COutBMTogong::MakeBMDeductSoil()
{
	CRcBridgeRebar	*pBri	= GetRcBridgeRebar();
	if(!pBri) return;

	long nIdxStt[2] = {0, 0};
	long nIdxEnd[2] = {0, 0};

	for(long bm = 0; bm < 2; bm++)
	{
		long stt = 0; for(stt =0; stt < 2; stt++)
		{
			BOOL bStt		= stt == iSTT;
			long nJ			= bStt ? 0 : pBri->m_nQtyJigan;
		
			if(pBri->IsBoxType()) 
				if(!bStt) continue;
			CTypedPtrArray <CObArray, CBMData*> *pArrBM	= &m_pArrBMTogongStd[nJ];

			if(!pBri->IsBoxType())
			{
				nIdxStt[stt]	= pArrBM->GetSize();
				nIdxEnd[stt]	= nIdxStt[stt];
			}
			else
			{
				nIdxStt[0]	= pArrBM->GetSize();
				nIdxEnd[0]	= nIdxStt[0];
			}
		}

		// 공제용으로 영역 구한 뒷채움 수량 가져옴
		MakeBMBackFill(bm==0 ? 2 : 3);

		// 가져온 수량 공제수량으로 변경
		for(stt = 0; stt < 2; stt++)
		{
			BOOL bStt		= stt == iSTT;
			if(pBri->IsBoxType() && !bStt) continue;

			long nJ	= bStt ? 0 : pBri->m_nQtyJigan;
			CTypedPtrArray <CObArray, CBMData*> *pArrBM	= &m_pArrBMTogongStd[nJ];
			// 노상 / 노체
			// 노상/노체부에 해당하는 뒷채움 수량 가져와서 공제로토 수정
			
			// 공제용 수량에 맞게 수정함
			nIdxEnd[stt]	= pArrBM->GetSize();
			long i = 0; for(i = nIdxStt[stt]; i < nIdxEnd[stt]; i++)
			{
				CBMData *pBM			= pArrBM->GetAt(i);
				pBM->m_nIdxBigClass		= BM_BIG_DEDUCT_SOIL;
				pBM->m_sBigClass		= GetStringBigClass(pBM->m_nIdxBigClass);
				pBM->m_nLevelTotal		= GetLevelTotal(pBM->m_nIdxBigClass);
 				pBM->m_nIdxMidClass		= -1;
 				pBM->m_sMidClass		= _T("");
				pBM->m_nIdxSmallClass	= bm == 0 ? BM_SMALL_DEDUCT_SOIL_NOSANG : BM_SMALL_DEDUCT_SOIL_NOCHE;
				pBM->m_sSmallClass		= GetStringSmallClass(pBM->m_nIdxSmallClass);
				pBM->m_nIdxItemClass	= -1;
				pBM->m_sItemClass		= _T("");
				if(pBri->IsBoxType())
				{
					pBM->m_nIdxItemClass	= -1;
					pBM->m_sItemClass		= _T("");
				}
				pBM->m_sVCN[Result]		= "공제토";
			}	
		}
	}
}

// 앞성토 수량
void COutBMTogong::MakeBMApsungto()
{
	CRcBridgeRebar	*pBri	= GetRcBridgeRebar();
	if(!pBri) return;

	long nIdxItemClass	= 0;
	// 본체 
	// 좌우측 두 쪽을 입력 받고 있으나, 본체는 좌측을 기준으로 앞성토를산출 하고
	// 좌측이 없으면 우측을 기준으로 산출한다.
	long stt = 0; for(stt = 0; stt < 2; stt++)
	{
		//BOX형일때는 시점에서 모두 넣어준다.
		BOOL bStt	= stt == iSTT;
		long nJ	= bStt ? 0 : pBri->m_nQtyJigan;
		if(!pBri->IsOutWall(bStt)) continue;
		if(pBri->IsBoxType()) nJ = 0;
		CTypedPtrArray <CObArray, CBMData*> *pArrBM	= &m_pArrBMTogongStd[nJ];

		double dWidth		= pBri->GetWidthSlabAct(pBri->GetStationByTarget(bStt ? BRIDGE_OFF_STT_SLAB : BRIDGE_OFF_END_SLAB, 0, TRUE), CDPoint(0, 1), 0);

		// 성토 영역 구함
		CalConcAreaSungto(bStt);
		long nSize	= pBri->m_pArrAreaConc.GetSize();

		// 수량 산출함(본체)
		long i = 0; for(i = 0; i < nSize; i++)
		{
			CTwinVectorAreaShape *pArea	= pBri->m_pArrAreaConc.GetAt(i);
			
			nIdxItemClass	= bStt ? BM_ITEM_APSUNGTO_MAINS : BM_ITEM_APSUNGTO_MAINE;
			CBMData *pBM			= new CBMData;
			pBM->m_nNumbering		= pArea->m_nNum;
			pBM->m_nIdxBigClass		= BM_BIG_APSUNGTO;
			pBM->m_sBigClass		= GetStringBigClass(pBM->m_nIdxBigClass);
			pBM->m_nLevelTotal		= GetLevelTotal(pBM->m_nIdxBigClass);
			pBM->m_nIdxMidClass		= -1;
			pBM->m_sMidClass		= _T("");
			pBM->m_nIdxSmallClass	= -1;
			pBM->m_sSmallClass		= _T("");
			pBM->m_nIdxItemClass	= nIdxItemClass;
			pBM->m_sItemClass		= GetStringItemClass(pBM->m_nIdxItemClass);
			pBM->m_nTypeResult		= RESULT_VOLUMN;
			pBM->m_sUnit			= "㎥";
			pBM->m_nShape			= pArea->m_nTypeShape;
			pBM->m_dV[WidthUpper]	= Round(toM(pArea->m_dWidthUpper),3);
			pBM->m_dV[WidthLower]	= Round(toM(pArea->m_dWidthLower),3);
			pBM->m_dV[Height]		= Round(toM(pArea->m_dHeight),3);
			pBM->m_dV[Length]		= Round(toM(dWidth),3);
			pBM->CalculateResult();
			pBM->m_sVCN[Result]		= "앞성토";			
			pArrBM->Add(pBM);
	
			// 본체 측면
			for(long left = 0; left < 2; left++)
			{
				BOOL bLeft	= left == iLEFT;
				nIdxItemClass	= bStt ? (bLeft ? BM_ITEM_APSUNGTO_WINGSL : BM_ITEM_APSUNGTO_WINGSR) : (bLeft ? BM_ITEM_APSUNGTO_WINGEL : BM_ITEM_APSUNGTO_WINGER);
				double dAng		= pBri->GetAngleJijum(bStt ? 0 : pBri->m_nQtyJigan).GetAngleDegree();
				dAng			= bLeft ? 180 - dAng : dAng;

				CBMData *pBMSide			= new CBMData(pBM);
				pBMSide->m_nIdxItemClass	= nIdxItemClass;
				pBMSide->m_sItemClass		= GetStringItemClass(pBMSide->m_nIdxItemClass);
				pBMSide->m_dV[Angle]		= dAng;
				pBMSide->m_dwSide			= SIDE_ROTATE;
				pBMSide->CalculateResult();
				pBMSide->m_sVCN[Result]			= "앞성토";
				if(!Compare(pBMSide->m_dV[Result], 0.0, _T(">")))
				{
					delete pBMSide;
					pBMSide	= NULL;
					continue;
				}
				pArrBM->Add(pBMSide);
			}
		}
	}
}

// 콘크리트 영역 구하기
void COutBMTogong::CalConcAreaSungto(BOOL bStt)
{
	CRcBridgeRebar	*pBri	= GetRcBridgeRebar();
	if(!pBri) return;

	CRebarPlacing rb;
	CTwinVectorArray tvArr;
	CTwinVector tv;
	
	// 앞성토
	AhTPADelete(&pBri->m_pArrAreaConc, (CTwinVectorAreaShape*)0);

	pBri->GetTvSungtoLine(tvArr, bStt, TRUE, FALSE);
	if(tvArr.GetSize() == 0)
	{
		pBri->GetTvSungtoLine(tvArr, bStt, FALSE, FALSE);
		if(tvArr.GetSize() == 0) return;
	}

	// 구한 성토라인을 폐다각형으로 만듬
	long nSize	= tvArr.GetSize();
	tv.m_v1	= tvArr.GetAt(nSize-1).m_v2;
	tv.m_v2	= CVector(tvArr.GetAt(0).m_v1.x, tvArr.GetAt(nSize-1).m_v2.y, 0);
	tvArr.Add(tv);

	tv.m_v1	= tv.m_v2;
	tv.m_v2	= tvArr.GetAt(0).m_v1;
	tvArr.Add(tv);

	// 폐 다각형을 영역을 나눔
	long nNum	= 1;
	long nIdx	= 0;
	CTwinVectorArray tvArrResult;
	CDoubleArray dArrAngleSu;
	rb.SeperatorTwinVectorArea(tvArr, tvArrResult, dArrAngleSu);
	long j = 0; for(j = 0; j < dArrAngleSu.GetSize(); j++)
	{
		CTwinVectorAreaShape *pArea	= new CTwinVectorAreaShape(nNum++);
		for(long k = 0; k < (long)dArrAngleSu.GetAt(j); k++)
			pArea->AddTwinVector(tvArrResult.GetAt(nIdx++));

		pArea->Calculate();
		pBri->m_pArrAreaConc.Add(pArea);
	}
}

// 앞성토 삽도
void COutBMTogong::SetInsertDomyunApsungto()
{
	CRcBridgeRebar	*pBri	= GetRcBridgeRebar();
	if(!pBri) return;

	CHgBaseBMStd baseBMStd;
	double dWidthOrg	= 350;

	// 뒷채움에서 날개벽 영역만 그림
	long stt = 0; for(stt = 0; stt < 2; stt++)
	{
		BOOL bStt	= stt == iSTT;
		if(!pBri->IsOutWall(bStt)) continue;

		// 앞성토 영역 구함
		CalConcAreaSungto(bStt);

		// 스케일 조정용 //////////////
		CDomyun domTmp;
		domTmp.SetCWnd(m_pStd->m_pView);
		// 날개벽 영역 (뒷채움시 공제되는 영역만)
		baseBMStd.DrawAreaSectionNumber(&domTmp, &pBri->m_pArrAreaConc);
		baseBMStd.DimAreaSection(&domTmp, &pBri->m_pArrAreaConc, 0, TRUE, CDPoint(0, 0), FALSE);
		baseBMStd.DimAreaSection(&domTmp, &pBri->m_pArrAreaConc, 1, TRUE, CDPoint(0, 0), FALSE);
		// 스케일 조정용 //////////////

		// 콘크리트 산출시 기초 산출전에 기초형태가 날개벽따라 꺽이는거 추가
		CDomyun domTot;
		domTot.SetCWnd(m_pStd->m_pView);

		// 도면 설정 ////////////////////
		double dScale	= baseBMStd.GetScale(&domTmp, dWidthOrg);
		baseBMStd.SetDomyunValue(&domTot, dWidthOrg, &domTmp);
		///////////////////////////////////

		CDomyun dom(&domTot);

		// 날개벽 영역 (뒷채움시 공제되는 영역만)
		baseBMStd.DrawAreaSectionNumber(&dom, &pBri->m_pArrAreaConc);
		baseBMStd.DimAreaSection(&dom, &pBri->m_pArrAreaConc, 0, TRUE, CDPoint(0, 0), FALSE);
		baseBMStd.DimAreaSection(&dom, &pBri->m_pArrAreaConc, 1, TRUE, CDPoint(0, 0), FALSE);

		// 삽도를 250, 400 사이즈로 변경
		dom.RedrawByScale(dScale);
		dom.SetCalcExtRect();
		domTot << dom;

		// 앞성토 이전에 삽입
		long nIdx	= bStt ? 0 : pBri->m_nQtyJigan;
		if(pBri->IsBoxType()) nIdx = 0;
		long i = 0; for(i = 0; i < m_pArrBMStringTogong[nIdx].GetSize(); i++)
		{
			CBMString *pBMString	= m_pArrBMStringTogong[nIdx].GetAt(i);

			if(!pBri->IsBoxType())
			{
				if(pBMString->m_sProcessKind1.Find("앞성토", 0) != -1)
				{
					pBMString->m_Dom	= domTot;
					pBMString->m_bInsertDomyun	= TRUE;
					break;
				}
			}
			//Box형일때는 삽도 두개가 들어감(앞성토 앞, 종점 앞)
			else
			{
				if(bStt && pBMString->m_sForm == "(본체(시점))")
				{
					pBMString->m_Dom	= domTot;
					pBMString->m_bInsertDomyun	= TRUE;
					break;
				}
				if(!bStt && pBMString->m_sForm == "(본체(종점))")
				{
					pBMString->m_Dom	= domTot;
					pBMString->m_bInsertDomyun	= TRUE;
					break;
				}
			}
		}	
	}
}

// 보호블럭 되메우기
// 터파기 - 구조물수량(보호블럭 + 보호블럭기초)
void COutBMTogong::MakeBMReFillBoho()
{
	CRcBridgeRebar	*pBri	= GetRcBridgeRebar();
	if(!pBri) return;
	long stt = 0; for(stt = 0; stt < 2; stt++)
	{
		BOOL bStt	= stt == iSTT;
		long nJ			= bStt ? 0 : pBri->m_nQtyJigan;
		CTypedPtrArray <CObArray, CBMData*> *pArrBM	= &m_pArrBMBohoStd[stt];

		BOOL bLeft	= TRUE;
		CWingWall *pWing	= NULL;
		if(pBri->IsWingWall(bStt, TRUE))
		{
			pWing	= pBri->GetWingWall(bStt, TRUE);
			bLeft	= TRUE;
		}
		else if(pBri->IsWingWall(bStt, FALSE)) 
		{
			pWing	= pBri->GetWingWall(bStt, FALSE);
			bLeft	= FALSE;
		}
		if(!pWing) continue;
		if(pBri->m_nTypeRcBridgeBlock == 0) continue;

		// 육교용 : 0인지 하천용 : 1인지..
		long nBlockType		= pBri->m_nTypeRcBridgeBlock == 2 ? 0 : 1;
		// 소단 구배는 1.8일땐 1.8용으로 쓰고 나머지는 1.5용으로 사용
		long nGubeType		= Compare(pWing->m_dSlopeNoriNext, 1.8, "=") ? 1 : 0;
		
		CTwinVectorArray tvArrBoho;
		pBri->GetTvSungtoLine(tvArrBoho, bStt, bLeft, TRUE);
		
		// 필요한 변수
		long nCountWing	= 0;
		if(pBri->IsWingWall(bStt, TRUE)) nCountWing++;
		if(pBri->IsWingWall(bStt, FALSE)) nCountWing++;

		CTwinVector tvJigan	= pBri->GetTvJijum(nJ, 0);
		double dAngle		= pBri->m_dAngleRcBridgeBlock;
		double dAngleBoho	= 33.6901;	// 보호블럭이 기초에 놓여지는 각..(이거 따로 계산해야됨..)
		double dAngleBohoRad	= dAngleBoho*ConstPi/180;
		double dLenBoho		= toM(tvJigan.GetLength());
		double dMaxH		= m_pStd->m_dBLBaseMaxHeight[nBlockType][nGubeType];
		double dMinH		= m_pStd->m_dBLBaseMinHeight[nBlockType][nGubeType];
		double dMaxW		= m_pStd->m_dBLBaseMaxWidth[nBlockType][nGubeType];
		double dMinW		= m_pStd->m_dBLBaseMinWidth[nBlockType][nGubeType];
		double dTopagi		= 0;
		double dTotalLength	= 0;	// 보호블럭 총길이
		double dBohoWidth	= 0;	// 보호블럭 너비
		double dOutLength	= 0;	// 보호블럭 외곽 길이
		double dT			= 0;	// 보호블럭	두께(뒷채움 콘크리트 포함)
		double dSungtoLength	= 0;	// 성토라인 길이(지반에 파붙히지 않은 보호블럭 길이)
		double dLastGube	= pWing->m_dSlopeNoriNext;	// 보호블럭 마지막 구배
		double dBohoLengthInEl	= 0;	// 땅에 파뭍힌 보호블럭 길이
		double dBohoH		= 0;		// 기초와 직각인 보호블럭에서 남는 높이..
		double dLenBohoHor	= GetLengthBohoHor(bStt);
		double dLenBohoFooting	= toM(2*ConstPi*dLenBohoHor*dAngle*nCountWing/360) + dLenBoho;
		
		long i = 0; for(i = 0; i < pArrBM->GetSize(); i++)
		{
			CBMData *pBM	= pArrBM->GetAt(i);
			if(pBM->m_sBigClass == "터파기")
				dTopagi += pBM->GetResult();
		}

		for(i = 0; i < tvArrBoho.GetSize(); i++)
		{
			dTotalLength += toM(tvArrBoho.GetAt(i).GetLength());
			dBohoWidth += toM(tvArrBoho.GetAt(i).GetHorLength());
		}

		dOutLength		= dLenBoho + 2 * ConstPi * dBohoWidth * (dAngle * nCountWing / 360);
		dT				= sqrt(pow((dMaxW-dMinW),2) + pow((dMaxH-dMinH),2));
		dBohoLengthInEl	= sqrt(pow(0.6,2)+pow((0.6*dLastGube),2));
		dSungtoLength	= dTotalLength - dBohoLengthInEl;
		dBohoH			= dT/tan(dAngleBohoRad);

		if(dTopagi > 0)
		{
			CBMData *pBM			= new CBMData;
			pBM->m_nIdxBigClass		= BM_BIG_REFILL;
			pBM->m_sBigClass		= GetStringBigClass(pBM->m_nIdxBigClass);
			pBM->m_nLevelTotal		= GetLevelTotal(pBM->m_nIdxBigClass);
			pBM->m_nIdxItemClass	= BM_ITEM_REFILL_TYPAGI;
			pBM->m_sItemClass		= GetStringItemClass(pBM->m_nIdxItemClass);
			pBM->m_nTypeResult		= RESULT_VOLUMN;		
			pBM->m_sUnit			= "㎥";
			pBM->m_sNoteHead		= "되메우기 = 터파기 - 구조물공제";
			pBM->m_sForm.Format("%.3f", dTopagi);
			pBM->m_sFormExcel.Format("#보호블럭터파기:%.3f#", dTopagi);
			pBM->SetResult(dTopagi);
			pBM->m_sVCN[Result]			= "보호블럭되메우기";
			pBM->m_bIsPrintByRefForm	= TRUE;
			pArrBM->Add(pBM);

			// 기초수량 공제
			double dH = fabs(m_pStd->m_dBLBaseMaxHeight[nBlockType][nGubeType]-m_pStd->m_dBLBaseMinHeight[nBlockType][nGubeType]);
			CBMData *pBM1 = new CBMData;
			pBM1->m_nIdxBigClass		= BM_BIG_REFILL;
			pBM1->m_sBigClass			= GetStringBigClass(pBM1->m_nIdxBigClass);
			pBM1->m_nLevelTotal			= GetLevelTotal(pBM1->m_nIdxBigClass);
			pBM1->m_nIdxItemClass		= BM_ITEM_REFILL_MAIN;
			pBM1->m_sItemClass			= GetStringItemClass(pBM1->m_nIdxItemClass);
			pBM1->m_sUnit				= "㎥";
			pBM1->m_nTypeResult			= RESULT_VOLUMN;		
			pBM1->m_sForm.Format("(((%.3f + %.3f) / 2 x %.3f) + (%.3f x %.3f)) x %.3f", 
				dMaxW, dMinW, dH, dMaxW, dMinH, dLenBohoFooting);
			pBM1->SetResult((((dMaxW + dMinW) / 2 * dH) + (dMaxW * dMinH)) * dLenBohoFooting);
			pBM1->m_bIsPrintByRefForm	= TRUE;
			pBM1->m_bIsUseForDeduct		= TRUE;
			pBM1->m_sNoteHead			= "보호블럭 기초공제";
			pBM1->m_sVCN[Result]		= "보호블럭되메우기";
			pArrBM->Add(pBM1);

			// 보호블럭 수량 공제
			CBMData *pBM2 = new CBMData;
			pBM2->m_nIdxBigClass		= BM_BIG_REFILL;
			pBM2->m_sBigClass			= GetStringBigClass(pBM2->m_nIdxBigClass);
			pBM2->m_nLevelTotal			= GetLevelTotal(pBM2->m_nIdxBigClass);
			pBM2->m_nIdxItemClass		= BM_ITEM_REFILL_MAIN;
			pBM2->m_sItemClass			= GetStringItemClass(pBM2->m_nIdxItemClass);
			pBM2->m_sUnit				= "㎥";
			pBM2->m_nTypeResult			= RESULT_VOLUMN;
			pBM2->m_sForm.Format("(%.3f x %.3f x %.3f) + (((π x %.3f² x %.3f / 360) - (π x %.3f² x %.3f / 360)) x %.3f) + (%.3f x %.3f / 2 x %.3f)",
				dT, dBohoLengthInEl, dLenBoho, dTotalLength, dAngle*2, dSungtoLength, dAngle*2, dT, dT, dBohoH, dOutLength);
			pBM2->SetResult((dT * dBohoLengthInEl * dLenBoho) + (((ConstPi * pow(dTotalLength,2) * dAngle*2 / 360) - (ConstPi * pow(dSungtoLength,2) * dAngle*2 / 360)) * dT) + (dT * dBohoH / 2 * dOutLength));
			pBM2->m_bIsUseForDeduct		= TRUE;
			pBM2->m_bIsPrintByRefForm	= TRUE;
			pBM2->m_sNoteHead			= "보호블럭 공제";
			pBM2->m_sVCN[Result]		= "보호블럭되메우기";
			pArrBM->Add(pBM2);		
		}
	}
}

// 보호블럭 유용토
void COutBMTogong::MakeBMValuableSoilBoho()
{
	CRcBridgeRebar	*pBri	= GetRcBridgeRebar();
	if(!pBri) return;
	
	long stt = 0; for(stt = 0; stt < 2; stt++)
	{
		BOOL bStt = stt == iSTT;
		BOOL bLeft	= TRUE;
		CWingWall *pWing	= NULL;
		if(pBri->IsWingWall(bStt, TRUE))
		{
			pWing	= pBri->GetWingWall(bStt, TRUE);
			bLeft	= TRUE;
		}
		else if(pBri->IsWingWall(bStt, FALSE)) 
		{
			pWing	= pBri->GetWingWall(bStt, FALSE);
			bLeft	= FALSE;
		}
		if(!pWing) continue;
		if(pBri->m_nTypeRcBridgeBlock == 0) continue;

		CTypedPtrArray <CObArray, CBMData*> *pArrBM	= &m_pArrBMBohoStd[stt];
		
		// 터파기와 되메우기 가져옴
		double dTypagi	= 0;
		double dReFill	= 0;
		long nSize	= pArrBM->GetSize();
		long i = 0; for(i = 0; i < nSize; i++)
		{
			CBMData *pBM	= pArrBM->GetAt(i);
			if(pBM->m_sBigClass == "터파기")
				dTypagi	+= pBM->GetResult();
			else if(pBM->m_sBigClass == "되메우기")
				dReFill	+= pBM->GetResult();
		}

		CBMData *pBM			= new CBMData;
		pBM->m_nIdxBigClass		= BM_BIG_VALUABLE_SOIL;
		pBM->m_sBigClass		= GetStringBigClass(pBM->m_nIdxBigClass);
		pBM->m_nLevelTotal		= GetLevelTotal(pBM->m_nIdxBigClass);
		pBM->m_nTypeResult		= RESULT_VOLUMN;		
		pBM->m_sUnit			= "㎥";
		pBM->m_sNoteHead		= "유용토 = 터파기 - 되메우기";
		pBM->m_sForm.Format("%.3f - %.3f", dTypagi, dReFill);
		pBM->m_sFormExcel.Format("#보호블럭터파기:%.3f# - #보호블럭되메우기:%.3f#", dTypagi, dReFill);
		pBM->SetResult(dTypagi - dReFill);
		pBM->m_bIsPrintByRefForm	= TRUE;
		pArrBM->Add(pBM);
	}
}


// void COutBMTogong::MakeBMWaterTupagi(CTypedPtrArray <CObArray, CBMData*> *pArr, BOOL bWaterTupagi, double dELWaterTupagi, long nIdxStt, long nIdxEnd)
// {
// 	if(!bWaterTupagi) return;
// 
// 	long i = 0; for(i = nIdxStt; i <= nIdxEnd; i++)
// 	{
// 		CBMData *pBM	= pArr->GetAt(i);
// 		if(pBM->m_nIdxBigClass != BM_BIG_TUPAGI) continue;
// 
// 	
// 		// 수량이 수중터파기에 완전히 포함되는 경우
// 		if(pBM->m_dV[SttMeter] <= dELWaterTupagi)
// 		{
// 			pBM->m_nIdxBigClass		= BM_BIG_TUPAGI_WATER;
// 			pBM->m_sBigClass		= GetStringBigClass(pBM->m_nIdxBigClass);
// 			pBM->m_sMidClass		= " " + pBM->m_sMidClass + " ";
// 		}
// 		// 수량이 수중터파기에 걸리는 경우. 잘라서 아래쪽을 수중터파기 수량으로 만든다.
// 		else if(pBM->m_dV[SttMeter] > dELWaterTupagi && pBM->m_dV[EndMeter] < dELWaterTupagi)
// 		{
// 			double dHOld	= pBM->m_dV[SttMeter] - pBM->m_dV[EndMeter];
// 			double dHNew	= pBM->m_dV[SttMeter] - dELWaterTupagi;
// 			double dRate	= dHNew / dHOld;
// 			double dDiffNew	= pBM->m_dV[WidthDiffLeft] * dRate;
// 			double dWLNew	= pBM->m_dV[WidthLower] * dRate;
// 			
// 			CBMData *pNew	= new CBMData(pBM);
// 
// 			// 원래꺼 (위) 수정
// 			pBM->m_dV[WidthLower]		= dWLNew;
// 			pBM->m_dV[WidthDiffLeft]	= dDiffNew;
// 			pBM->m_dV[EndMeter]			= dELWaterTupagi;
// 			pBM->m_dV[Height]			= pBM->m_dV[SttMeter] - pBM->m_dV[EndMeter];
// 			pBM->CalculateResult();
// 			pBM->m_dV[Result]			= pBM->GetResult();
// 
// 			// 새로운거(아래) 추가
// 			pNew->m_nIdxBigClass		= BM_BIG_TUPAGI_WATER;
// 			pNew->m_sBigClass			= GetStringBigClass(pNew->m_nIdxBigClass);
// 			pNew->m_sMidClass			= " " + pNew->m_sMidClass + " ";
// 			pNew->m_dV[WidthUpper]		= pBM->m_dV[WidthLower];
// 			pNew->m_dV[WidthDiffLeft]	= pNew->m_dV[WidthDiffLeft] - dDiffNew;
// 			pNew->m_dV[WidthDiffRight]	= pNew->m_dV[WidthLower] - pNew->m_dV[WidthUpper];
// 			pNew->m_dV[Height]			= pNew->m_dV[SttMeter] - pNew->m_dV[EndMeter];
// 			pNew->CalculateResult();
// 			pNew->m_dV[Result]			= pNew->GetResult();
// 			pArr->Add(pNew);
// 		}
// 	}
// }

//32223 터파기 에서 구배가 0일 경우에 연속되는 같은 토양으로 터파기를 구분해주기 위한 함수
//tvArr = tvArrVert가 필요 tvArrResultGooBae: 리턴 되는 보정된 수중 터파기 dYBase: 육상 터파기 EL  dWaterEL: 수중 터파기 EL
// 수중 터파기가 존재하고 다른 토양의 터파기 이며, 구배가 모두 0일 경우에는 아래의 식을 사용함
// 리턴되는 tvArrResultGooBae에는 SeperatorTwinVectorArea에서 계산된 tvArr을 수중터파기 EL을 기준으로 섹션을 다시 보정함
void COutBMTogong::CalcZeroGooBaeExcavation(CTwinVectorArray &tvArr, CTwinVectorArray &tvArrResultGooBae, double dYBase, double dWaterEL)
{
	tvArrResultGooBae.RemoveAll();

	CTwinVector tvBottom, tvLeft, tvRight, tvTop, tvTemp;
	CTwinVector tvFixBottom, tvFixLeft, tvFixRight, tvFixTop;
	CTwinVectorArray tvArrTemp;
	CTwinVectorArray tvArrPolyGon , tvArrPolyGonPrev;

	CRebarPlacing rb;

	long nIdx4 = 0;
	BOOL bFixHeight = FALSE;

	tvArrResultGooBae.AddFromTvArray(tvArr);

	for (long ix =tvArr.GetSize()-1; ix >= 1; ix--)
	{
		tvTemp = tvArr.GetAt(ix);

		if(nIdx4 < 4)
		{
			tvArrPolyGon.Add(tvTemp);
			
			if(nIdx4 == 3)
			{
				long nIdxBottom = 0, nIdxPrev =0, nIdxNext = 0, nIdxTop = 0;
				nIdx4 = 0;

				nIdxBottom	= rb.GetIdxLeftBottomOfTvArr(tvArrPolyGon);

				nIdxPrev	= rb.GetIdxSideOfTvArr(tvArrPolyGon, nIdxBottom, TRUE);
				nIdxNext	= rb.GetIdxSideOfTvArr(tvArrPolyGon, nIdxBottom, FALSE);

				for(long nx=0; nx < 4 ; nx++)
				{
					if(nx == nIdxBottom || nx == nIdxNext || nx == nIdxPrev)
						continue;

					nIdxTop = nx;
				}

				//하단의 수평면을 구한다
				tvBottom = tvArrPolyGon.GetAt(nIdxBottom);
				tvLeft = tvArrPolyGon.GetAt(nIdxNext);
				tvRight = tvArrPolyGon.GetAt(nIdxPrev);
				tvTop = tvArrPolyGon.GetAt(nIdxTop);

				//상하면의 높이가 수중터가 가운데 끼어 있을때 실행
				if(Compare(tvBottom.m_v1.y, dWaterEL , _T("<"),0.001) && Compare(tvTop.m_v1.y, dWaterEL , _T(">"),0.001))
				{
					tvFixBottom.m_v1.x = tvBottom.m_v1.x;
					tvFixBottom.m_v1.y = dWaterEL;
					tvFixBottom.m_v2.x  = tvBottom.m_v2.x;
					tvFixBottom.m_v2.y  = dWaterEL;
					tvFixBottom.m_sInfo = tvBottom.m_sInfo;

					tvFixLeft.m_v1 = tvLeft.m_v1;
					tvFixLeft.m_v2.x = tvLeft.m_v2.x;
					tvFixLeft.m_v2.y = dWaterEL;
					tvFixLeft.m_sInfo = tvLeft.m_sInfo;

					tvFixRight.m_v2 = tvRight.m_v2;
					tvFixRight.m_v1.y = dWaterEL;
					tvFixRight.m_v1.x = tvRight.m_v1.x;
					tvFixRight.m_sInfo = tvRight.m_sInfo;

					tvFixTop = tvTop;
					tvFixTop.m_sInfo = tvTop.m_sInfo;

					bFixHeight = TRUE;

					tvArrPolyGonPrev.Add(tvFixLeft);
					tvArrPolyGonPrev.Add(tvFixTop);
					tvArrPolyGonPrev.Add(tvFixRight);
					tvArrPolyGonPrev.Add(tvBottom);

					//고친것을 집어 넣어주어야 함 결과값에
					CTwinVectorArray tvArrPush;

					tvArrPush.Add(tvFixBottom);
					tvArrPush.Add(tvFixRight);
					tvArrPush.Add(tvFixTop);
					tvArrPush.Add(tvFixLeft);

					long nIdxPush = 0;
					for (long jx = ix; jx < ix+4; jx++)
					{
						CTwinVector tvPush = tvArrPush.GetAt(nIdxPush);
						tvArrResultGooBae.SetAt(jx, tvPush);

						nIdxPush++;
					}
					tvArrPush.RemoveAll();
				}
				//상위의 식을 고쳤을 경우에 바로 다음에 오는 경우에는 상단의 높이가 달라지기 때문에 수정이 필요함
				else if(bFixHeight && tvArrPolyGonPrev.GetSize() > 0)
				{
					tvFixBottom = tvFixBottom;
					tvFixBottom.m_sInfo = tvBottom.m_sInfo;

					tvFixLeft.m_v2 = tvLeft.m_v2;
					tvFixLeft.m_v1.x = tvLeft.m_v1.x;
					tvFixLeft.m_v1.y = tvArrPolyGonPrev.GetAt(2).m_v1.y;
					tvFixLeft.m_sInfo = tvLeft.m_sInfo;
					
					tvFixRight.m_v1 = tvRight.m_v1;
					tvFixRight.m_v2.y =  tvArrPolyGonPrev.GetAt(2).m_v1.y;
					tvFixRight.m_v2.x = tvRight.m_v2.x;
					tvFixRight.m_sInfo = tvRight.m_sInfo;

					tvFixTop.m_v1.y = tvArrPolyGonPrev.GetAt(2).m_v1.y;
					tvFixTop.m_v2.y = tvArrPolyGonPrev.GetAt(2).m_v1.y;
					tvFixTop.m_sInfo = tvTop.m_sInfo;

					bFixHeight = FALSE;
					tvArrPolyGonPrev.RemoveAll();

					//여기도 집어 넣어야 하는데 어떻게 집어 넣지...
					CTwinVectorArray tvArrPush;

					tvArrPush.Add(tvFixBottom);
					tvArrPush.Add(tvFixRight);
					tvArrPush.Add(tvFixTop);
					tvArrPush.Add(tvFixLeft);

					long nIdxPush = 0;
					for (long jx = ix; jx < ix+4; jx++)
					{
						CTwinVector tvPush = tvArrPush.GetAt(nIdxPush);
						tvArrResultGooBae.SetAt(jx, tvPush);

						nIdxPush++;
					}
					tvArrPush.RemoveAll();
				}

				tvArrPolyGon.RemoveAll();
			} // nIdx > 3
			else
				nIdx4++;
		}

	}
}