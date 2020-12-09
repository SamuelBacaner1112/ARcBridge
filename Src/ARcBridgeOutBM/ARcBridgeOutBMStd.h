// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// ARcBridgeOutBMStd.h: interface for the CARcBridgeOutBMStd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ARCBRIDGEOUTBMSTD_H__C93881E6_DD29_4C5F_857C_A067C54DF930__INCLUDED_)
#define AFX_ARCBRIDGEOUTBMSTD_H__C93881E6_DD29_4C5F_857C_A067C54DF930__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// 수량 산출 종류

#define COMMON_BM	0x00001		// 공통수량
#define NORMAL_BM	0x00002		// 일반수량
#define REBAR_BM	0x00004		// 철근수량

#define TYPE_BM_NORMAL	0		// 일반수량 산출 기준
#define TYPE_BM_ACTUAL	1		// 실적수량 산출 기준

#define TYPE_POST_BRIDGE	0	// 동바리 타입 (육교용)
#define TYPE_POST_NORMAL	1	// 동바리 타입 (일반용)

// 거푸집 타입 설정
#define FORM_POS_NORMAL			0	// 일반콘크리트
#define FORM_POS_FOOTING		1	// 기초
#define FORM_POS_WALL_FRONT		2	// 벽체전면
#define FORM_POS_WALL_BACK		3	// 벽체배면
#define FORM_POS_WINGSL_FRONT	4	// 시점좌측날개벽 전면
#define FORM_POS_WINGSL_BACK	5	// 시점좌측날개벽 배면
#define FORM_POS_WINGSR_FRONT	6	// 시점우측날개벽 전면
#define FORM_POS_WINGSR_BACK	7	// 시점우측날개벽 배면
#define FORM_POS_WINGEL_FRONT	8	// 종점좌측날개벽 전면
#define FORM_POS_WINGEL_BACK	9	// 종점좌측날개벽 배면
#define FORM_POS_WINGER_FRONT	10	// 종점우측날개벽 전면
#define FORM_POS_WINGER_BACK	11	// 종점우측날개벽 배면
#define FORM_POS_APS			12	// 접속슬래브
#define FORM_POS_MASS			13	// mass
#define FORM_POS_LEAN			14	// lean


class CARcBridgeDataStd;
class CARoadOptionStd;
class COutBMDialog;
class COutBM;
class COutBMTogong;
class CDlgOptionBMSheet;
class CBMData;
class CTwinVectorAreaShape;
class CRcBridgeRebar;
class CViewBM;

class AFX_EXT_CLASS CARcBridgeOutBMStd  
{
public:
	CTypedPtrArray <CObArray, CRcBridgeRebar*> m_pArrBri;
	void CheckCalBM();
//	void SetFormByPos(CBMData *pBM, long nPos);
	void MakeBMRcBridgeArray();
	CRcBridgeRebar* GetBridge(long nBri);

	CDocument* m_pDoc;
	CView* m_pView;
	CARoadOptionStd* m_pARoadOptionStd;
	CARcBridgeDataStd* m_pARcBridgeDataStd;

	// 수량 산출서 dlg
	COutBMDialog *m_pOutBMDialog;
	CHgBaseBMStd *m_pBaseBM;

	// 일반수량
	CTypedPtrArray <CObArray, COutBM*> m_pArrOutBMNormal;	// 일반수량
	CTypedPtrArray <CObArray, COutBM*> m_pArrOutBMCommon;	// 공통수량
	CTypedPtrArray <CObArray, COutBMTogong*> m_pArrOutBMTogong;

	// 수량 옵션 //////////////////////////////////////////////////////
	// 수량산출의 전체적인 옵션에 관련된 변수
	long m_nTypeBM;	// 삭제예정
	
	// 공종별 수량 산출 옵션(어떤걸 산출할지...)
	CViewBM* m_pViewBM;				// 일반수량옵션(공종별 산출 옵션)
	CViewBM* m_pViewBMCommon;		// 공통수량옵션(공종별 산출 옵션)
	CViewBM* m_pViewBMTogong;		// 토공수량옵션(공종별 산출 옵션)

	BOOL m_bCalNormalBM;		// 일반적인 수량 산출을 할 것인지?
	BOOL m_bCalRebarBM;			// 철근집계 수량 산출을 할 것인지?
	
	// 출력 내용 선택 옵션(산출한 것 중에 출력할 내용만 선택)
	CViewBM* m_pViewBMSheet;	// 수량옵션(시트별 출력 옵션)

	// 수량 산출 옵션(수량 산출이나 출력시 고려해야 될 사항들)
	BOOL m_bConvertToForm;		// 엑셀출력시 수식을 변환할지...
	BOOL m_bConvertToRef;		// 엑셀출력시 셀 참조형태로 변환할지...
	BOOL m_bRefCellBMSum;		// 집계표 간 셀 참조
	BOOL m_bDrawElEarth;		// 삽도에 원지반선 표현
	long m_nTypeRebarBM_XXX;		// 철근집계 방법(0:강도별로집계, 1:본체와접속슬래브로나눠서 집계)
	BOOL m_bSeparateExcludeRebarMonument;	// 본체철근집계시 교명주 철근수량 제외

	// 쓰이지 않는 변수(CRcBridgeData의 멤버변수로 대체됨) - 김도헌20051103 
	long m_nTypePost;			// 동바리 타입(육교용, 일반용)	(삭제예정)
	long m_nTypePostBaseLine;	// 동바리 기준선(0 : 지반선, 2 : 기초) (삭제예정)
	double m_dThickPostUpper;	// 육교용 동바리 상부 두께 (삭제예정)
	double m_dThickPostSide;	// 육교용 동바리 좌우측 두께 (삭제예정)

	CDlgOptionBMSheet* m_pDlgOptionBMSheet;
	//////////////////////////////////////////////////////////////////

	// 보호블럭 상수값 /////////////////////////////////////
	// 보호블럭 기초 치수(육교용) 
	double m_dBLBaseMaxHeight[2][2];
	double m_dBLBaseMinHeight[2][2];
	double m_dBLBaseMaxWidth[2][2];
	double m_dBLBaseMinWidth[2][2];

	// 콘크리트 종류
	long m_nTypeConc[2];
	
	// 미터당 재료량
	// 보호블럭 기초콘크리트
	double m_dMaterialConc[2];
	
	// 보호블럭 기초거푸집
	double m_dMaterialForm[2];
	////////////////////////////////////////////////////////

	void GroupConnectedTvArr(CTwinVectorArrayArray &tvArrArrReturn, CTwinVectorArray &tvArrStructure, BOOL bCheckMeanless=TRUE);
	long ConvertTvArrToTvArrAreaShape(CTwinVectorArray &tvArr, CDoubleArray &dArrAngleSu, CTypedPtrArray <CObArray, CTwinVectorAreaShape*> *pArrAreaConc, long nBaseNum, BOOL bMerge);
	CARcBridgeOutBMStd(CARcBridgeDataStd *pARcBridgeDataStd=NULL);
	virtual ~CARcBridgeOutBMStd();
	long m_nFlag;
	void Serialize(CArchive &ar);

private:
	
};

#endif // !defined(AFX_ARCBRIDGEOUTBMSTD_H__C93881E6_DD29_4C5F_857C_A067C54DF930__INCLUDED_)
