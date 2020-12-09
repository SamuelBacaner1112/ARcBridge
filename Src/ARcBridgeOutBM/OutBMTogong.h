// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// OutBMTogong.h: interface for the COutBMTogong class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OUTBMTOGONG_H__A72186AE_0EA6_4DDE_9C71_CC26D0B37311__INCLUDED_)
#define AFX_OUTBMTOGONG_H__A72186AE_0EA6_4DDE_9C71_CC26D0B37311__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define BM_BIG_TUPAGI			0	// 터파기
#define BM_BIG_TUPAGI_WATER		1	// 수중 터파기
#define BM_BIG_WATEROUT			2	// 물푸기
#define BM_BIG_BACKFILL			3	// 뒷채움
#define BM_BIG_REFILL			4	// 퇴메우기
#define BM_BIG_VALUABLE_SOIL	5	// 유용토
#define BM_BIG_DEDUCT_SOIL		6	// 공제토
#define BM_BIG_APSUNGTO			7	// 앞성토

#define BM_MID_TUPAGI_TOSA		0	// 터파기(토사)
#define BM_MID_TUPAGI_PUWHAAM	1	// 터파기(풍화암)
#define BM_MID_TUPAGI_YUNAM		2	// 터파기(연암)
#define BM_MID_TUPAGI_GYUNGAM	3	// 터파기(경암)

#define BM_SMALL_DEDUCT_SOIL_NOSANG	0	// 공제토(노상)
#define BM_SMALL_DEDUCT_SOIL_NOCHE	1	// 공제토(노체)

#define BM_ITEM_BACKFILL			0	// 뒷채움
#define BM_ITEM_BACKFILL_APS		1	// 뒷채움 접속슬래브 공제
#define BM_ITEM_BACKFILL_WING		2	// 뒷채움 날개벽 공제
#define BM_ITEM_BACKFILL_BRACKET	3	// 뒷채움 브라켓 공제
#define BM_ITEM_REFILL_TYPAGI		4	// 되메우기 터파기
#define BM_ITEM_REFILL_MAIN			5	// 되메우기 지반선 이하의 구체 공제
#define BM_ITEM_REFILL_BACKFILL		6	// 되메우기 지반선 이하의 뒷채움 공제
#define BM_ITEM_APSUNGTO_MAINS		7	// 앞성토 본체(시점)
#define BM_ITEM_APSUNGTO_WINGSL		8	// 앞성토 날개벽(시점좌측)
#define BM_ITEM_APSUNGTO_WINGSR		9	// 앞성토 날개벽(시점우측)
#define BM_ITEM_APSUNGTO_MAINE		10	// 앞성토 본체(종점)
#define BM_ITEM_APSUNGTO_WINGEL		11	// 앞성토 날개벽(종점좌측)
#define BM_ITEM_APSUNGTO_WINGER		12	// 앞성토 날개벽(종점우측)


class CBMData;
class CBMString;
class CBMSumString;
class CRcBridgeRebar;
class CFootingApp;
#define MAX_JIJUM_COUNT_RC 21
class AFX_EXT_CLASS COutBMTogong : public COutBMBase  
{
public:
	void CalConcAreaSungto(BOOL bStt);	// 성토 영역 구하기
	// 삽도 출력에 관한 옵션들 /////////////////////////////////////////
	double m_dScaleDim_VertSection;	// 종단면도 스케일
	double m_dHeightText_VertSection;	// 종단면도 텍스트 높이
	////////////////////////////////////////////////////////////////////

	void DrawFirstPageBoho(CHgBaseBMStd *pBaseBMStd, BOOL bStt);
	long DrawFirstPageTogong(CHgBaseBMStd *pBaseBMStd, long jijum);
	void CalculateBM();
	void ClearBM(BOOL bRemainStdBM=FALSE);
	void MakeBMStringStd();
	void MergeBMStd();
	void SetInsertDomyunStd();
	COutBMTogong(long nBri);
	virtual ~COutBMTogong();

	// 수량
	// 수집용(정제안된 순수 수량)
	CTypedPtrArray <CObArray, CBMData*> m_pArrBMTogongStd[MAX_JIJUM_COUNT_RC];			// 토공수량(정제 안된것)
	CTypedPtrArray <CObArray, CBMData*> m_pArrBMBohoStd[2];				// 보호블럭수량(정제 안된것)

	// 출력용(정제된 수량)
	CTypedPtrArray <CObArray, CBMData*> m_pArrBMTogong[MAX_JIJUM_COUNT_RC];				// 토공수량(정제 안된것) - 산출근거 출력
	CTypedPtrArray <CObArray, CBMString*> m_pArrBMStringTogong[MAX_JIJUM_COUNT_RC];		// 토공수량(정제 안된것) - 산출근거 출력
	CTypedPtrArray <CObArray, CBMData*> m_pArrBMBoho[2];				// 보호블럭수량(정제 안된것) - 산출근거 출력
	CTypedPtrArray <CObArray, CBMString*> m_pArrBMStringBoho[2];		// 보호블럭수량(정제 안된것) - 산출근거 출력
	

	// 부위별집계표에 출력된 내용
	CTypedPtrArray <CObArray, CBMData*> m_pArrBMTogongForPartSum[MAX_JIJUM_COUNT_RC];
	CTypedPtrArray <CObArray, CBMData*> m_pArrBMBohoForPartSum[2];	


	CTypedPtrArray <CObArray, CBMData*> m_pArrBMTogongSum;					// 토공수량 - 교량별 집계표 출력 
	CTypedPtrArray <CObArray, CBMSumString*> m_pArrBMStringTogongSum;
		
	CTypedPtrArray <CObArray, CBMData*> m_pArrBMBohoSum;					// 보호블럭수량 - 교량별 집계표 출력
	CTypedPtrArray <CObArray, CBMSumString*> m_pArrBMStringBohoSum;

	CTypedPtrArray <CObArray, CBMSumString*> m_pArrBMStringAllSum;			// 집계(토공수량+보호블럭) - 교량별 집계표 출력 
	CTypedPtrArray <CObArray, CBMData*> m_pArrBMSum;						// 교량집계(정제 안된것) - 교량별 집계표 출력

	CTypedPtrArray <CObArray, CBMSumString*> m_pArrBMStringSepBridgeSum;	// 상하행 분리 교량 집계표 출력
	CTypedPtrArray <CObArray, CBMData*> m_pArrBMSepBridgeSum;

	CARcBridgeOutBMStd* m_pStd;

	double GetLengthBohoHor(BOOL bStt);
private:
	void MakeBMValuableSoilBoho();
	void MakeBMReFillBoho();
	void SetInsertDomyunApsungto();
	void MakeBMApsungto();
	void MakeBMDeductSoil();
	void MakeBMValuableSoil();
	void MakeBMReFill();
	void SetInsertDomyunBackFill();
	void MakeBMBackFill(long nForUse=4);
	void MakeBMTyopagi();
	void MakeBMWaterOut();
	void MakeBMBohoTyopagi();
	void GetInfoMeter(long nIdxBigClass, double &dFirstMeter, double &dSecondMeter, CFootingApp *pFooting);
	BOOL IsPrintMeter(CBMData *pBM);
	void RemakeByHeightBMList(CTypedPtrArray <CObArray, CBMData*> *pArrBM, double dYBase, double dFirstMeter, double dSecondMeter, long nIdxStt=-1, long nIdxEnd=-1);
//	void MakeBMWaterTupagi(CTypedPtrArray <CObArray, CBMData*> *pArr, BOOL bWaterTupagi, double dELWaterTupagi, long nIdxStt, long nIdxEnd);

	CString GetStringBigClass(long nIdxBigClass);
	CString GetStringMidClass(long nIdxMidClass);
	CString GetStringSmallClass(long nIdxSmallClass);
	CString GetStringItemClass(long nIdxItemClass);
	long GetLevelTotal(long nIdxBigClass);
	
	CRcBridgeRebar* GetRcBridgeRebar(BOOL bOrg = TRUE);
	
	void SetViewBM(CTypedPtrArray <CObArray, CBMData*> *pArrBM);
	void ApplyViewBM(CTypedPtrArray <CObArray, CBMData*> *pArrBM);
	void ApplyRefCellBM(CTypedPtrArray <CObArray, CBMData*> *pArrBM);
	void ApplyOptionBMStd(CTypedPtrArray <CObArray, CBMData*> *pArrBM);

	long m_nBri;

	//32223 터파기 에서 구배가 0일 경우에 연속되는 같은 토양으로 터파기를 구분해주기 위한 함수
	void CalcZeroGooBaeExcavation(CTwinVectorArray &tvArr, CTwinVectorArray &tvArrResultGooBae, double dYBase, double dWaterEL);
};

#endif // !defined(AFX_OUTBMTOGONG_H__A72186AE_0EA6_4DDE_9C71_CC26D0B37311__INCLUDED_)
