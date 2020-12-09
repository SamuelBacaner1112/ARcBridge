// ARcBridgeOutBMStd.cpp: implementation of the CARcBridgeOutBMStd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../../Src/ARcBridgeData/ARcBridgeData.h"
#include "ARcBridgeOutBM.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CARcBridgeOutBMStd::CARcBridgeOutBMStd(CARcBridgeDataStd *pARcBridgeDataStd)
{
	m_pARcBridgeDataStd	= pARcBridgeDataStd;
	m_pBaseBM = new CHgBaseBMStd;

	m_pOutBMDialog = new COutBMDialog;
	m_pOutBMDialog->m_pStd = this;
	m_pOutBMDialog->m_pBaseBM = m_pBaseBM;

	m_nTypeBM	= 0;	// 0 : 일반산출기준, 1 : 실적산출기준(공통수량분리). 2 : 실적수량산출기준(공통수량포함)
	// 공종별 수량 산출 옵션(어떤걸 산출할지...)
	m_pViewBM		= new CViewBM;
	m_pViewBMCommon	= new CViewBM;
	m_pViewBMTogong	= new CViewBM;
	m_bCalNormalBM	= TRUE;			// 일반적인 수량 산출
	m_bCalRebarBM	= TRUE;			// 철근집계 수량 산출
	
	// 출력 내용 선택 옵션(산출한 것 중에 출력할 내용만 선택)
	m_pViewBMSheet	= new CViewBM;

	// 수량 산출 옵션(수량 산출이나 출력시 고려해야 될 사항들)
	m_bConvertToForm	= TRUE;		// 엑셀출력시 수식을 변환할지...
	m_bConvertToRef		= FALSE;	// 엑셀출력시 셀 참조형태로 변환할지...
	m_bRefCellBMSum		= FALSE;		// 집계표 간 셀 참조
	m_bDrawElEarth		= TRUE;		// 삽도에 원지반선 표현
	m_nTypeRebarBM_XXX		= 0;		// 철근집계 방법(0:강도별로집계, 1:본체와접속슬래브로나눠서 집계)
	m_bSeparateExcludeRebarMonument	= FALSE;	// 본체철근집계시 교명주 철근수량 제외
	
	// 쓰이지 않는 변수(CRcBridgeData의 멤버변수로 대체됨) - 김도헌20051103 
	m_nTypePost			= 0;		// 동바리 타입(육교용, 일반용)
	m_nTypePostBaseLine	= 0;		// 동바리 기준선(0 : 지반선, 1 : 기초)
	m_dThickPostUpper	= 1200;		// 육교용 동바리 상부 두께
	m_dThickPostSide	= 1200;		// 육교용 동바리 좌우측 두께

	m_pDlgOptionBMSheet	= new CDlgOptionBMSheet("수량산출 옵션");
	m_pDlgOptionBMSheet->m_pStd	= this;
	//////////////////////////////////////////////////////////////////

	// 토공 수량 산출 옵션 ///////////////////////////////////////////
	//////////////////////////////////////////////////////////////////

	// 보호블럭 상수 값 ///////////
	// 보호블럭 기초 치수(육교용)
	m_dBLBaseMaxHeight[0][0]	= 0.4;
	m_dBLBaseMinHeight[0][0]	= 0.234;
	m_dBLBaseMaxWidth[0][0]		= 0.4;
	m_dBLBaseMinWidth[0][0]		= 0.289;

	m_dBLBaseMaxHeight[0][1]	= 0.4;
	m_dBLBaseMinHeight[0][1]	= 0.225;
	m_dBLBaseMaxWidth[0][1]		= 0.4;
	m_dBLBaseMinWidth[0][1]		= 0.303;

	// 보호블럭 기초 치수(하천용)
	m_dBLBaseMaxHeight[1][0]	= 0.4;
	m_dBLBaseMinHeight[1][0]	= 0.234;
	m_dBLBaseMaxWidth[1][0]		= 0.4;
	m_dBLBaseMinWidth[1][0]		= 0.290;

	m_dBLBaseMaxHeight[1][1]	= 0.4;
	m_dBLBaseMinHeight[1][1]	= 0.225;
	m_dBLBaseMaxWidth[1][1]		= 0.4;
	m_dBLBaseMinWidth[1][1]		= 0.303;

	// 콘크리트 종류
	m_nTypeConc[0]				= 0;		// 기초용 : 3종
	m_nTypeConc[1]				= 1;		// 뒷채움용 : 5종

	// 미터당 재료량
	// 보호블럭 기초콘크리트
	m_dMaterialConc[0]			= 0.151;	// 1.5	
	m_dMaterialConc[1]			= 0.152;	// 1.8
	// 보호블럭 기초거푸집
	m_dMaterialForm[0]			= 0.634;	// 1.5
	m_dMaterialForm[1]			= 0.625;	// 1.8

	m_nFlag	= 0;
}

CARcBridgeOutBMStd::~CARcBridgeOutBMStd()
{
	AhTPADelete(&m_pArrOutBMTogong, (COutBM*)0);
	AhTPADelete(&m_pArrOutBMNormal, (COutBM*)0);
	AhTPADelete(&m_pArrOutBMCommon, (COutBM*)0);
	AhTPADelete(&m_pArrBri, (CRcBridgeRebar*)0);

	delete m_pBaseBM;
	delete m_pOutBMDialog;
	delete m_pViewBM;
	delete m_pViewBMCommon;
	delete m_pViewBMTogong;
	delete m_pViewBMSheet;

	delete m_pDlgOptionBMSheet;
}

// 수량 산출시 필요한 만큼의 배열을 만든다.(교량별로 하나씩 가짐)
void CARcBridgeOutBMStd::MakeBMRcBridgeArray()
{
	AhTPADelete(&m_pArrOutBMTogong,	(COutBMTogong*)0);
	AhTPADelete(&m_pArrOutBMNormal, (COutBM*)0);
	AhTPADelete(&m_pArrOutBMCommon, (COutBM*)0);
	AhTPADelete(&m_pArrBri, (CRcBridgeRebar*)0);

	long i = 0; for(i = 0; i < m_pARcBridgeDataStd->GetBridgeSize(); i++)
	{
		// 일반수량
		COutBM *pOutBM	= new COutBM(i);
		pOutBM->m_pStd = this;
		m_pArrOutBMNormal.Add(pOutBM);

		// 공통수량
		pOutBM = new COutBM(i);
		pOutBM->m_pStd = this;
		m_pArrOutBMCommon.Add(pOutBM);

		// 토공수량
		COutBMTogong *pOutBMTogong	= new COutBMTogong(i);
		pOutBMTogong->m_pStd	= this;
		m_pArrOutBMTogong.Add(pOutBMTogong);

		// 수량산출용 교량
		CRcBridgeRebar *pBri = new CRcBridgeRebar;
		pBri->m_bBridgeForBM = TRUE;
		m_pArrBri.Add(pBri);
	}
}

void CARcBridgeOutBMStd::Serialize(CArchive &ar)
{
	// 1 : 공종옵션이 FALSE로 저장되서 옵션이 안나오는 현상 고침.일단 1이전데이타는 TRUE로 읽어들임.
	// 2 : m_bRefCellBMSum;		// 집계표 간 셀 참조
	// 3 : m_bSeparateExcludeRebarMonument;	// 본체철근집계시 교명주 철근수량 제외
	// 4 : m_pViewBMCommon 공통수량옵션 추가.
	m_nFlag	= 4;

	if(ar.IsStoring())
	{
		ar << m_nFlag;
		// 수량 옵션 //////////////////////////////////////////////////////
		ar << m_nTypeBM;		// 0 : 일반산출기준, 1 : 실적산출기준.

		// 공종별 수량 산출 옵션(어떤걸 산출할지...)
		m_pViewBM->Serialize(ar);				// 일반수량옵션(공종별 산출 옵션)
		m_pViewBMCommon->Serialize(ar);			// 공통수량옵션(공종별 산출 옵션)
		m_pViewBMTogong->Serialize(ar);			// 토공수량옵션(공종별 산출 옵션)
		ar << m_bCalNormalBM;			// 일반적인 수량 산출
		ar << m_bCalRebarBM;			// 철근집계 수량 산출
		
		// 출력 내용 선택 옵션(산출한 것 중에 출력할 내용만 선택)
		m_pViewBMSheet->Serialize(ar);	// 수량옵션(시트별 출력 옵션)

		// 수량 산출 옵션(수량 산출이나 출력시 고려해야 될 사항들)
		ar << m_bConvertToForm;	// 엑셀출력시 수식을 변환할지...
		ar << m_bConvertToRef;		// 엑셀출력시 셀 참조형태로 변환할지...
		ar << m_bRefCellBMSum;		// 집계표 간 셀 참조
		ar << m_bDrawElEarth;		// 삽도에 원지반선 표현
		ar << m_nTypeRebarBM_XXX;		// 철근집계 방법(0:강도별로집계, 1:본체와접속슬래브로나눠서 집계)
		ar << m_bSeparateExcludeRebarMonument;	// 본체철근집계시 교명주 철근수량 제외
		
		// 쓰이지 않는 변수(CRcBridgeData의 멤버변수로 대체됨) - 김도헌20051103 
		ar << m_nTypePost;			// 동바리 타입(육교용, 일반용)
		ar << m_nTypePostBaseLine;	// 동바리 기준선(0 : 자동, 1 : 지반선, 2 : 기초)
		ar << m_dThickPostUpper;	// 육교용 동바리 상부 두께
		ar << m_dThickPostSide;		// 육교용 동바리 좌우측 두께
		//////////////////////////////////////////////////////////////////

		// 토공 수량 산출 옵션 ///////////////////////////////////////////
		//////////////////////////////////////////////////////////////////
	}
	else
	{
		ar >> m_nFlag;
		// 수량 옵션 //////////////////////////////////////////////////////
		ar >> m_nTypeBM;		// 0 : 일반산출기준, 1 : 실적산출기준.

		// 공종별 수량 산출 옵션(어떤걸 산출할지...)
		m_pViewBM->Serialize(ar);			// 일반수량옵션(공종별 산출 옵션)
		if(m_nFlag > 3)
			m_pViewBMCommon->Serialize(ar);	// 공통수량옵션(공종별 산출 옵션)
		m_pViewBMTogong->Serialize(ar);		// 토공수량옵션(공종별 산출 옵션)
		if(m_nFlag < 1)
		{
			long i = 0; for(i = 0; i < m_pViewBM->m_pArrViewBM.GetSize(); i++)
				m_pViewBM->SetBMState(i, TRUE);
			for(i = 0; i < m_pViewBMTogong->m_pArrViewBM.GetSize(); i++)
				m_pViewBMTogong->SetBMState(i, TRUE);
		}
		ar >> m_bCalNormalBM;			// 일반적인 수량 산출
		ar >> m_bCalRebarBM;			// 철근집계 수량 산출
		
		// 출력 내용 선택 옵션(산출한 것 중에 출력할 내용만 선택)
		m_pViewBMSheet->Serialize(ar);	// 수량옵션(시트별 출력 옵션)

		// 일반 수량 산출 옵션(수량 산출이나 출력시 고려해야 될 사항들)
		ar >> m_bConvertToForm;		// 엑셀출력시 수식을 변환할지...
		ar >> m_bConvertToRef;		// 엑셀출력시 셀 참조형태로 변환할지...
		if(m_nFlag > 1)
			ar >> m_bRefCellBMSum;		// 집계표 간 셀 참조
		ar >> m_bDrawElEarth;		// 삽도에 원지반선 표현
		ar >> m_nTypeRebarBM_XXX;		// 철근집계 방법(0:강도별로집계, 1:본체와접속슬래브로나눠서 집계)
		if(m_nFlag > 2)
			ar >> m_bSeparateExcludeRebarMonument;	// 본체철근집계시 교명주 철근수량 제외
		
		// 쓰이지 않는 변수(CRcBridgeData의 멤버변수로 대체됨) - 김도헌20051103 
		ar >> m_nTypePost;			// 동바리 타입(육교용, 일반용)
		ar >> m_nTypePostBaseLine;	// 동바리 기준선(0 : 자동, 1 : 지반선, 2 : 기초)
		ar >> m_dThickPostUpper;	// 육교용 동바리 상부 두께
		ar >> m_dThickPostSide;		// 육교용 동바리 좌우측 두께
		//////////////////////////////////////////////////////////////////

		// 토공 수량 산출 옵션 ///////////////////////////////////////////
		//////////////////////////////////////////////////////////////////
	}
}

// tvArr를 tvArrAreaShape로 대치
long CARcBridgeOutBMStd::ConvertTvArrToTvArrAreaShape(CTwinVectorArray &tvArr, CDoubleArray &dArrAngleSu, CTypedPtrArray <CObArray, CTwinVectorAreaShape*> *pArrAreaConc, long nBaseNum, BOOL bMerge)
{
	if(!pArrAreaConc) return nBaseNum;
	if(!bMerge)
		AhTPADelete(pArrAreaConc,	(CTwinVectorAreaShape*)0);

	long nSize	= dArrAngleSu.GetSize();
	long nIdx	= 0;
	for(long i = 0; i < nSize; i++)
	{
		long nCountAngle = (long)dArrAngleSu.GetAt(i);

		CTwinVectorAreaShape *pArea	= new CTwinVectorAreaShape;
		pArea->m_nNum	= nBaseNum++;
		for(long j = 0; j < nCountAngle; j++)
		{
			pArea->AddTwinVector(tvArr.GetAt(nIdx++));
		}
		pArea->Calculate();
		pArrAreaConc->Add(pArea);
	}

	return nBaseNum;
}

// 부위별 거푸집 타입 설정해줌
// void CARcBridgeOutBMStd::SetFormByPos(CBMData *pBM, long nPos)
// {
// }

void CARcBridgeOutBMStd::CheckCalBM()
{
	if(!m_pARcBridgeDataStd) return;

	long nTypeBM	= m_pARcBridgeDataStd->m_nTypeBMApply;

	for(HGINT32 ix = 0; ix < m_pArrOutBMNormal.GetSize(); ix++)
	{
		// 일반수량
		COutBM *pOutBMNor = m_pArrOutBMNormal.GetAt(ix);
		if(pOutBMNor == NULL)
			continue;

		pOutBMNor->m_dwCalBM = NORMAL_BM;

		// 공통수량을 분리하지 않은 경우에는 공통수량도 산출하도록 추가한다. 
		if(m_pARcBridgeDataStd->m_nTypeBMApply != 1)
			pOutBMNor->m_dwCalBM |= COMMON_BM;


		///////////////////////////////////////////////////////////////////////////////////////////
		// 공통수량
		COutBM *pOutBMCom = m_pArrOutBMCommon.GetAt(ix);
		if(pOutBMCom == NULL)
			continue;

		// 공통수량 분리 옵션일 경우에만 산출되도록 한다. 
		pOutBMCom->m_dwCalBM = (nTypeBM == 1) ? COMMON_BM : 0x00000000;
	}
}

/**
	@brief
		tvArrStructure의 tv중에서 연결된 선끼리 분류, 각각을 tvArrArrReturn에 저장.\n
		CHgRcStructureGenStd에서 복사.
*/
void CARcBridgeOutBMStd::GroupConnectedTvArr(CTwinVectorArrayArray &tvArrArrReturn, CTwinVectorArray &tvArrStructure, BOOL bCheckMeanless)
{
	AhTPADelete(&tvArrArrReturn, (CTwinVectorArray*)0);
	if(tvArrStructure.GetSize() == 0)	return;
	
	CTwinVectorArray tvArrBase;
	tvArrBase	= tvArrStructure;
	
	CRebarPlacing rb;
	rb.RemoveMinLengthLine(tvArrBase, 0);
	tvArrBase.Sort();
	if(bCheckMeanless)
		rb.RemoveMeanlessTwinVector(tvArrBase);
	tvArrBase.Sort();
	
	CTwinVectorArray *pTvArrTemp	=  NULL;
	long i = 0;
	for(i = 0; i < tvArrBase.GetSize(); i++)
	{
		CTwinVector *pTv	= tvArrBase.GetPAt(i);
		if(!pTvArrTemp)
		{
			pTvArrTemp	=  new CTwinVectorArray;
			pTvArrTemp->Add(*pTv);
			tvArrBase.RemoveAt(i);
		}
		else
		{
			CTwinVector tvLast;
			pTvArrTemp->GetLast(tvLast);
			if(ComparePt(CDPoint(tvLast.m_v2), CDPoint(pTv->m_v1), "="))
			{
				pTvArrTemp->Add(*pTv);
				tvArrBase.RemoveAt(i);
			}
			else
			{
				tvArrArrReturn.Add(pTvArrTemp);
				pTvArrTemp	= NULL;
				tvArrBase.Sort();
				//i++;
			}
		}
		i--;
	}
	if(pTvArrTemp)
		tvArrArrReturn.Add(pTvArrTemp);
}

CRcBridgeRebar* CARcBridgeOutBMStd::GetBridge( long nBri )
{
	CRcBridgeRebar *pBri = NULL;
	long nSize = m_pArrBri.GetSize();
	if(nBri > -1  && nBri < nSize)
		pBri = m_pArrBri.GetAt(nBri);

	return pBri;
}
