// OutBM.cpp: implementation of the COutBM class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "arcbridgeoutbm.h"
#include "../../Src/ARcBridgeDBDraw/ARcBridgeDBDraw.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COutBM::COutBM(long nBri)
{
	m_nBri	= nBri;
	m_dwCalBM = 0x00000000;

	// 삽도 출력에 관한 옵션들 /////////////////////////////////////////
	m_dScaleDim_VertSection		= 100;	// 종단면도 스케일
	m_dHeightText_VertSection	= 7;	// 종단면도 텍스트 높이
	////////////////////////////////////////////////////////////////////
}

COutBM::~COutBM()
{
	ClearBM();

	AhTPADelete(&m_pArrAreaConc,	(CTwinVectorAreaShape*)0);
}

// 수량을 모두 산출함.
// 산출한뒤 머지도 바로 함.
void COutBM::CalculateBM(BOOL bNewCreateBM)
{
	ClearBM();
//	m_pStd->CheckCalBM();

	// (ARCBRIDGE-1477) 수량이 선택되어 있지 않아도 삽도는 그리는 경우가 있다. 교량 데이터는 만들어주고 return하자.
	// 수량 산출
	CRcBridgeRebar *pBriOrg	 = m_pStd->m_pARcBridgeDataStd->GetBridgeRebar(m_nBri);
	m_pStd->m_pARcBridgeDataStd->MakeRcBridgeForBM(pBriOrg, m_pStd->GetBridge(m_nBri));

	// 공통수량의 경우 조건에 따라 산출이 안될 수도 있음.
	if(m_dwCalBM == 0x00000000)
		return;

	long stt(0), left(0);

	if(m_dwCalBM & NORMAL_BM)
	{
		MakeBMBohoBlock();			// 보호블럭
		MakeBMConcreteMain();		// 콘크리트(본체, 기초)
		MakeBMConcreteGuardWall();	// 콘크리트(방호벽)
		MakeBMConcreteMonument();	// 콘크리트(교명주)
		MakeBMConcreteWing();		// 콘크리트(날개벽)
		MakeBMConcreteAps();		// 콘크리트(접속슬래브)
		MakeBMConcBohoFooting();	// 콘크리트(보호블럭 기초)
		MakeBMConcBohoBackFill();	// 콘크리트(보호블럭 뒷채움)
		MakeBMFormMain();			// 거푸집(본체, 기초)
		MakeBMFormGuardWall();		// 거푸집(방호벽)
		MakeBMFormMonument();		// 거푸집(교명주)
		MakeBMFormWing();			// 거푸집(날개벽)
		MakeBMStyrofoamAps();		// 스티로폼(접속슬래브)
		MakeBMFormAps();			// 거푸집(접속슬래브)
		MakeBMFormBoho();			// 거푸집(보호블럭)
		MakeBMDowelBar();			// DowelBar(접속슬래브)
	}

	if(m_dwCalBM & COMMON_BM)
	{
		MakeBMPostMain();			// 동바리(본체)
		MakeBMDeckFinisher(); //데크피니셔
		MakeBMPostWing();			// 동바리(날개벽)
		MakeBMHoriConnectMain();	// 동바리수평연결재(본체)
		MakeBMHoriConnectWing();	// 동바리수평연결재(날개벽)
	
		MakeBMScaffoldMain();		// 비계(본체)
		MakeBMScaffoldWing();		// 비계(날개벽)
		
		MakeBMScaffoldLegMain(); //비계다리(본체)
		MakeBMScaffoldLegWing(); //비계다리(날개벽)
		MakeRubbleBM();				//잡석 기초
	}

	if(m_dwCalBM & NORMAL_BM)
	{
		MakeBMSpacerMain();			// 스페이셔(본체)
		MakeBMSpacerWing();			// 스페이셔(날개벽)
		MakeBMSpacerAps();			// 스페이셔(접속슬래브)
		MakeBMSlabCare();			// 슬래브양생(본체전용)
		MakeBMConCareMain();		// 콘크리트양생(본체)
		MakeBMConCareWing();		// 콘크리트양생(날개벽)
		MakeBMConCareAps();			// 콘크리트양생(접속슬래브)
		MakeBMRebar(0);				// 철근가공조립(본체)
		MakeBMRebar(1);				// 철근가공조립(날개벽)
		MakeBMRebar(2);				// 철근가공조립(접속슬래브)
		MakeBMRebar(3);				// 철근가공조립(방호벽)
		MakeBMRebar(4);				// 철근가공조립(교명주)
		MakeBMSurface();			// 면고르기(본체전용)
		MakeBMAscon();				// 아스콘 포장(본체전용)
		MakeBMTackCotting();		// 택 코팅(본체전용)
		MakeBMSurfaceProof();		// 교면방수(본체전용)
		MakeBMAsphaltProof();		// 아스팔트방수(본체+날개벽)
		MakeBMPile();				// 말뚝수량
		MakeBMNamePanel();			// 교명판
		MakeBMDesPanel();			// 설명판
		MakeBMTBM();				// TBM설치
		MakeBMFlexibleJoint();		// 신축이음
		MakeBMFlexibleJointCover();	// 신축이음커버
		MakeBMNonShrinkageConc();	// 무수축 콘크리트
		MakeBMCoverPlate();			// cover plate
		MakeBMMonument();			// 교명주
		MakeBMNotch();				// NOTCH;
	}
	
	if(m_pStd->m_bCalRebarBM)
	{
		MakeBMRebar();
		MakeBMRebarTable();
	}
	for(stt = 0; stt < 2; stt++)
	{
		// 산출한 수량 ViewBM에 등록
		if(stt == 0)
		{
			SetViewBM(&m_pArrBMBridgeStd, bNewCreateBM);
			SetViewBM(&m_pArrBMMonumentStd);

 			long nQtyHDan = pBriOrg->GetQtyHDan();
 			long i=0; for(i=0; i<nQtyHDan; i++)
 			{
 				CGuardWallRC* pGW = pBriOrg->GetGuardWallByHDan(i);
 				if(!pGW) continue;
 				if(pGW->IsTypeHDanRoadTotalGuardWall())		
 					SetViewBM(&m_pArrBMGuardWallStd[i]);
 			}
		}
		for(left = 0; left < 2; left++)
		{
			SetViewBM(&m_pArrBMWingStd[stt][left]);
			SetViewBM(&m_pArrBMApsStd[stt][left]);
			long i = 0; for(i = 0; i < 2; i++)
				SetViewBM(&m_pArrBMCnsStd[stt][left][i]);
		}
		
		SetViewBM(&m_pArrBMBohoStd[stt]);

		// 등록한뒤 산출한 수량을 ViewBM옵션을 적용 시킨다.
		if(stt == 0)
		{
 			ApplyOptionBMStd(&m_pArrBMBridgeStd);
  			ApplyOptionBMStd(&m_pArrBMMonumentStd);
  
  			long nQtyHDan = pBriOrg->GetQtyHDan();
  			long i=0; for(i=0; i<nQtyHDan; i++)
  			{
  				CGuardWallRC* pGW = pBriOrg->GetGuardWallByHDan(i);
  				if(!pGW) continue;
 				if(pGW->IsTypeHDanRoadTotalGuardWall())		
  					ApplyOptionBMStd(&m_pArrBMGuardWallStd[i]);
  			}
		}
		for(left = 0; left < 2; left++)
		{
			ApplyOptionBMStd(&m_pArrBMWingStd[stt][left]);
			ApplyOptionBMStd(&m_pArrBMApsStd[stt][left]);
			long i = 0; for(i = 0; i < 2; i++)
				ApplyOptionBMStd(&m_pArrBMCnsStd[stt][left][i]);
		}
		ApplyOptionBMStd(&m_pArrBMBohoStd[stt]);
	}
}

// 그냥 산출한 수량에 대해서 필요에 따라 이리저리 머지를 해댐.
void COutBM::MergeBMStd()
{
	CRcBridgeRebar *pBri = GetRcBridgeRebar();
	CRcBridgeRebar *pBriOrg = GetRcBridgeRebar(TRUE);
	if(!pBri) return;
	if(!pBriOrg) return;

	if(m_dwCalBM == 0x00000000)
		return;

	long stt(0), left(0);//, cns(0);//, bar(0);
	long nQtyHDan = pBriOrg->GetQtyHDan();

	// 차례대로 머지. /////////////////////
	CHgBaseBMStd baseBM;
	DWORD exceptionFlag	= 0x00000000;

	for(stt = 0; stt < 2; stt++)
	{
		// 정제 되지 않은 수량을 정제함.(산출근거 출력용) - 완전히같은 수량에 대해서만 머지함.
		if(stt == 0)
		{
			// 본체
			baseBM.MergeBMList(&m_pArrBMBridge, &m_pArrBMBridgeStd, FALSE, exceptionFlag);
			if(m_dwCalBM & NORMAL_BM)
			{
				// 방호벽
				long i=0; for(i=0; i<nQtyHDan; i++)
				{
					CGuardWallRC* pGW = pBriOrg->GetGuardWallByHDan(i);
					if(!pGW) continue;
					if(pGW->IsTypeHDanRoadTotalGuardWall())				
						baseBM.MergeBMList(&m_pArrBMGuardWall[i], &m_pArrBMGuardWallStd[i], FALSE, exceptionFlag);
				}
				// 교명주
				baseBM.MergeBMList(&m_pArrBMMonument, &m_pArrBMMonumentStd, FALSE, exceptionFlag);
			}
		}
	}

	//날개벽
	for(stt = 0; stt < 2; stt++)
	{
		for(left = 0; left < 2; left++)
		{
			if(m_pArrBMWingStd[stt][left].GetSize() == 0) continue;
			baseBM.MergeBMList(&m_pArrBMWing[stt][left], &m_pArrBMWingStd[stt][left], FALSE, exceptionFlag);
		}
	}
	//접속슬래브
	for(stt = 0; stt < 2; stt++)
	{
		for(left = 0; left < 2; left++)
		{
			if(m_dwCalBM & NORMAL_BM)
			{
				if(m_pArrBMApsStd[stt][left].GetSize() == 0) continue;
				baseBM.MergeBMList(&m_pArrBMAps[stt][left], &m_pArrBMApsStd[stt][left], FALSE, exceptionFlag);
			}
		}
	}
	//완충슬래브
	for(stt = 0; stt < 2; stt++)
	{
		for(left = 0; left < 2; left++)
		{
			if(m_dwCalBM & NORMAL_BM)
			{
				long i = 0; for(i = 0; i < 2; i++)
				{
					if(m_pArrBMCnsStd[stt][left][i].GetSize() == 0) continue;
					baseBM.MergeBMList(&m_pArrBMCns[stt][left][i], &m_pArrBMCnsStd[stt][left][i], FALSE, exceptionFlag);
				}
			}
		}
	}
	//보호블럭
	for(stt = 0; stt < 2; stt++)
	{
		if(m_dwCalBM & NORMAL_BM)
			baseBM.MergeBMList(&m_pArrBMBoho[stt], &m_pArrBMBohoStd[stt], FALSE, exceptionFlag);		
	}


	// 부위별 집계표용(본체집계,날개벽집계,접속슬래브집계,완충슬래브집계) - 총계내는 레벨에 따라서 머지함
	for(stt = 0; stt < 2; stt++)
	{
		if(stt == 0)
		{
			baseBM.MergeBMList(&m_pArrBMBridgeForPartSum, &m_pArrBMBridge, TRUE, exceptionFlag);
			if(m_dwCalBM & NORMAL_BM)
			{
				long i=0; for(i=0; i<nQtyHDan; i++)
				{
					CGuardWallRC* pGW = pBriOrg->GetGuardWallByHDan(i);
					if(!pGW) continue;
					if(pGW->IsTypeHDanRoadTotalGuardWall())	
						baseBM.MergeBMList(&m_pArrBMGuardWallForPartSum[i], &m_pArrBMGuardWall[i], TRUE, exceptionFlag);
				}
				if(m_pArrBMMonument.GetSize() > 0)
					baseBM.MergeBMList(&m_pArrBMMonumentPartSum, &m_pArrBMMonument, TRUE, exceptionFlag);
			}
		}
	}
	//날개벽
	for(stt = 0; stt < 2; stt++)
	{
		for(left = 0; left < 2; left++)
		{
			if(m_pArrBMWing[stt][left].GetSize() == 0) continue;
			baseBM.MergeBMList(&m_pArrBMWingForPartSum[stt][left], &m_pArrBMWing[stt][left], TRUE, exceptionFlag);
		}
	}
	//접속슬래브
	for(stt = 0; stt < 2; stt++)
	{
		for(left = 0; left < 2; left++)
		{
			if(m_dwCalBM & NORMAL_BM)
			{
				if(m_pArrBMAps[stt][left].GetSize() == 0) continue;
				baseBM.MergeBMList(&m_pArrBMApsForPartSum[stt][left], &m_pArrBMAps[stt][left], TRUE, exceptionFlag);
			}
		}
	}
	//완충슬래브
	for(stt = 0; stt < 2; stt++)
	{
		for(left = 0; left < 2; left++)
		{
			if(m_dwCalBM & NORMAL_BM)
			{
				long i = 0; for(i = 0; i < 2; i++)
				{
					if(m_pArrBMCns[stt][left][i].GetSize() == 0) continue;
					baseBM.MergeBMList(&m_pArrBMCnsForPartSum[stt][left][i], &m_pArrBMCns[stt][left][i], TRUE, exceptionFlag);
				}
			}
		}
	}
	//보호벽
	for(stt = 0; stt < 2; stt++)
	{
		if(m_dwCalBM & NORMAL_BM)
			baseBM.MergeBMList(&m_pArrBMBohoForPartSum[stt], &m_pArrBMBoho[stt], TRUE, exceptionFlag);
	}

	// 총괄집계표용
	for(stt = 0; stt < 2; stt++)
	{
		if(stt == 0)
		{
			baseBM.MergeBMList(&m_pArrBMBridgeSum, &m_pArrBMBridge, TRUE, exceptionFlag);
			if(m_dwCalBM & NORMAL_BM)
			{
				long i=0; for(i=0; i<nQtyHDan; i++)
				{
					CGuardWallRC* pGW = pBriOrg->GetGuardWallByHDan(i);
					if(!pGW) continue;
					if(pGW->IsTypeHDanRoadTotalGuardWall())	
						baseBM.MergeBMList(&m_pArrBMGuardWallSum, &m_pArrBMGuardWall[i], TRUE, exceptionFlag);
				}
				if(m_pArrBMMonument.GetSize() > 0)
					baseBM.MergeBMList(&m_pArrBMGuardWallSum, &m_pArrBMMonument, TRUE, exceptionFlag);
			}
		}
	}

	//날개벽
	for(stt = 0; stt < 2; stt++)
	{
		for(left = 0; left < 2; left++)
		{
			if(m_pArrBMWing[stt][left].GetSize() == 0) continue;
			baseBM.MergeBMList(&m_pArrBMWingSum, &m_pArrBMWing[stt][left], TRUE, exceptionFlag);
		}
	}
	//접속슬래브
	for(stt = 0; stt < 2; stt++)
	{
		for(left = 0; left < 2; left++)
		{
			if(m_dwCalBM & NORMAL_BM)	// 접속슬래브, 완충슬래브는 구조물 수량 산출에서만 포함됨
			{
				if(m_pArrBMAps[stt][left].GetSize() == 0) continue;
				baseBM.MergeBMList(&m_pArrBMApsSum, &m_pArrBMAps[stt][left], TRUE, exceptionFlag);
			}
		}
	}
	//완충슬래브
	for(stt = 0; stt < 2; stt++)
	{
		for(left = 0; left < 2; left++)
		{
			if(m_dwCalBM & NORMAL_BM)	// 접속슬래브, 완충슬래브는 구조물 수량 산출에서만 포함됨
			{
				long i = 0; for(i = 0; i < 2; i++)
				{
					if(m_pArrBMCns[stt][left][i].GetSize() == 0) continue;
					baseBM.MergeBMList(&m_pArrBMCnsSum, &m_pArrBMCns[stt][left][i], TRUE, exceptionFlag);
				}
			}
		}
	}
	//보호벽
	for(stt = 0; stt < 2; stt++)
	{
		if(m_dwCalBM & NORMAL_BM)	// 보호블럭는 구조물 수량 산출에서만 포함됨
			baseBM.MergeBMList(&m_pArrBMBohoSum, &m_pArrBMBoho[stt], TRUE, exceptionFlag);
	}

	// 교량 집계표용
	baseBM.MergeBMList(&m_pArrBMSum, &m_pArrBMBridgeSum, TRUE, exceptionFlag);
	if(m_dwCalBM & NORMAL_BM)
		baseBM.MergeBMList(&m_pArrBMSum, &m_pArrBMGuardWallSum, TRUE, exceptionFlag);
	baseBM.MergeBMList(&m_pArrBMSum, &m_pArrBMWingSum, TRUE, exceptionFlag);
	if(m_dwCalBM & NORMAL_BM)
	{
		baseBM.MergeBMList(&m_pArrBMSum, &m_pArrBMApsSum, TRUE, exceptionFlag);
		baseBM.MergeBMList(&m_pArrBMSum, &m_pArrBMCnsSum, TRUE, exceptionFlag);
		baseBM.MergeBMList(&m_pArrBMSum, &m_pArrBMBohoSum, TRUE, exceptionFlag);
	}

	// 교량 총괄 집계표에선 무조건 본체에 모든 항목이 있어야 된다.
	// 항목 생성용으로 하나 만듬 (단 값들은 본체를 제외하곤 모두 0으로 들어감)
	baseBM.MergeBMList(&m_pArrBMSumForTitle, &m_pArrBMBridgeSum, TRUE, exceptionFlag, FALSE);
	if(m_dwCalBM & NORMAL_BM)
	{
		if(m_pArrBMGuardWallSum.GetCount() > 0)
			baseBM.MergeBMList(&m_pArrBMSumForTitle, &m_pArrBMGuardWallSum, TRUE, exceptionFlag, TRUE);
	}
	if(m_pArrBMWingSum.GetCount() > 0)
		baseBM.MergeBMList(&m_pArrBMSumForTitle, &m_pArrBMWingSum, TRUE, exceptionFlag, TRUE);
	if(m_dwCalBM & NORMAL_BM)
	{
		if(m_pArrBMApsSum.GetCount() > 0)
			baseBM.MergeBMList(&m_pArrBMSumForTitle, &m_pArrBMApsSum, TRUE, exceptionFlag, TRUE);
		if(m_pArrBMCnsSum.GetCount() > 0)
			baseBM.MergeBMList(&m_pArrBMSumForTitle, &m_pArrBMCnsSum, TRUE, exceptionFlag, TRUE);
		if(m_pArrBMBohoSum.GetCount() > 0)
			baseBM.MergeBMList(&m_pArrBMSumForTitle, &m_pArrBMBohoSum, TRUE, exceptionFlag, TRUE);
	}


	// 철근집계용
	if(m_dwCalBM & NORMAL_BM)
	{
		for(long bar = 0; bar < 2; bar++)
		{		
			baseBM.MergeRebarBMList(&m_pArrRebarBMBridge[bar], &m_pArrRebarBMBridgeStd[bar]);

			long i=0; for(i=0; i<nQtyHDan; i++)
			{
				CGuardWallRC* pGW = pBriOrg->GetGuardWallByHDan(i);
				if(!pGW) continue;
				if(pGW->IsTypeHDanRoadTotalGuardWall())	
					baseBM.MergeRebarBMList(&m_pArrRebarBMGuardWall[i][bar], &m_pArrRebarBMGuardWallStd[i][bar]);
			}
			baseBM.MergeRebarBMList(&m_pArrRebarBMMonument[bar], &m_pArrRebarBMMonumentStd[bar]);

			
			for(stt = 0; stt < 2; stt++)
			{
				BOOL bStt = stt == iSTT;

				for(left = 0; left < 2; left++)
				{
					BOOL bLeft = left == iLEFT;

					baseBM.MergeRebarBMList(&m_pArrRebarBMWing[stt][left][bar], &m_pArrRebarBMWingStd[stt][left][bar]);
					baseBM.MergeRebarBMList(&m_pArrRebarBMAps[stt][left][bar], &m_pArrRebarBMApsStd[stt][left][bar]);
					for(long cns = 0; cns < 2; cns++)
					{
						CApsApp *pCns = bStt? &pBri->m_cnsStt[left][cns] : &pBri->m_cnsEnd[left][cns];
						if(!pCns->m_bIs) continue;

						baseBM.MergeRebarBMList(&m_pArrRebarBMCns[stt][left][cns][bar], &m_pArrRebarBMCnsStd[stt][left][cns][bar]);
					}
				}
			}

			long nSize = pBri->GetCountFooting();
			for (long ix =0; ix < nSize; ix++)
			{
				CFootingData *pFooting	= pBri->GetFooting(ix);
				if(!pFooting) continue;

				CExPileApp *pPile = &pFooting->m_exFooting.m_Pile;
				if(pPile == NULL) continue;

				if(pPile->m_nType != PILE_TYPE_HYUNJANG)
					continue;

				baseBM.MergeRebarBMList(&m_pArrRebarBMHyunta[ix][bar], &m_pArrRebarBMHyuntaStd[ix][bar]);
			}
		}
	}
}

// 수량 출력용 문자열 생성
void COutBM::MakeBMStringStd()
{
	CRcBridgeRebar *pBri = GetRcBridgeRebar();
	CRcBridgeRebar *pBriOrg = GetRcBridgeRebar(TRUE);
	if(!pBri) return;
	if(!pBriOrg) return;

	if(m_dwCalBM == 0x00000000)
		return;

	CHgBaseBMStd baseBM;
	long stt(0), left(0);//, bar(0);//, cns(0);
	long nQtyHDan = pBriOrg->GetQtyHDan();

	// 정제된 산출근거용 수량
	baseBM.MakeBMString(&m_pArrBMStringBridge, &m_pArrBMBridge, TRUE);
	if(m_dwCalBM & NORMAL_BM)
	{
		long i=0; for(i=0; i<nQtyHDan; i++)
		{
			CGuardWallRC* pGW = pBriOrg->GetGuardWallByHDan(i);
			if(!pGW) continue;
			if(pGW->IsTypeHDanRoadTotalGuardWall())	
				baseBM.MakeBMString(&m_pArrBMStringGuardWall[i], &m_pArrBMGuardWall[i], TRUE);
		}
		baseBM.MakeBMString(&m_pArrBMStringMonument, &m_pArrBMMonument, TRUE);
	}
	//날개벽
	for(stt = 0; stt < 2; stt++)
	{
		for(left = 0; left < 2; left++)
		{
			if(m_pArrBMWing[stt][left].GetSize() == 0) continue;
			baseBM.MakeBMString(&m_pArrBMStringWing[stt][left], &m_pArrBMWing[stt][left], TRUE);
		}
	}
	//접속슬래브
	for(stt = 0; stt < 2; stt++)
	{
		for(left = 0; left < 2; left++)
		{
			if(m_dwCalBM & NORMAL_BM)
			{
				if(m_pArrBMAps[stt][left].GetSize() == 0) continue;
				baseBM.MakeBMString(&m_pArrBMStringAps[stt][left], &m_pArrBMAps[stt][left], TRUE);
			}
		}
	}
	//완충슬래브
	for(stt = 0; stt < 2; stt++)
	{
		for(left = 0; left < 2; left++)
		{
			if(m_dwCalBM & NORMAL_BM)
			{
				long i = 0; for(i = 0; i < 2; i++)
				{
					if(m_pArrBMCns[stt][left][i].GetSize() == 0) continue;
					baseBM.MakeBMString(&m_pArrBMStringCns[stt][left][i], &m_pArrBMCns[stt][left][i], TRUE);
				}
			}
		}

		if(m_dwCalBM & NORMAL_BM)
			baseBM.MakeBMString(&m_pArrBMStringBoho[stt], &m_pArrBMBoho[stt], TRUE);
	}
	//보호블럭
	for(stt = 0; stt < 2; stt++)
	{
		if(m_dwCalBM & NORMAL_BM)
			baseBM.MakeBMString(&m_pArrBMStringBoho[stt], &m_pArrBMBoho[stt], TRUE);
	}

	// 각 부위별 집계표(본체집계, 날개벽집계, 접속슬래브집계, 완충슬래브집계, 보호블럭집계)
	baseBM.MakeBMSumString(&m_pArrBMStringBridgeSum, &m_pArrBMBridgeSum, TRUE, TRUE);	// 본체
	if(m_dwCalBM & NORMAL_BM)
	{
		long i=0; for(i=0; i<nQtyHDan; i++)
		{
			CGuardWallRC* pGW = pBriOrg->GetGuardWallByHDan(i);
			if(!pGW) continue;
			if(pGW->IsTypeHDanRoadTotalGuardWall())	
				baseBM.MakeBMSumString(&m_pArrBMStringGuardWallSum, &m_pArrBMGuardWallForPartSum[i], TRUE, FALSE);	// 방호벽
		}
		if(m_pArrBMMonumentPartSum.GetSize() > 0)
			baseBM.MakeBMSumString(&m_pArrBMStringGuardWallSum, &m_pArrBMMonumentPartSum, TRUE, FALSE);	// 교명주
	}
	//날개벽
	for(stt = 0; stt < 2; stt++)
	{
		for(left = 0; left < 2; left++)
		{
			if(m_pArrBMWingForPartSum[stt][left].GetSize() == 0) continue;
			baseBM.MakeBMSumString(&m_pArrBMStringWingSum, &m_pArrBMWingForPartSum[stt][left], TRUE, stt==0&&left==0);			// 날개벽
		}
	}
	//접속슬래브
	for(stt = 0; stt < 2; stt++)
	{
		for(left = 0; left < 2; left++)
		{
			if(m_dwCalBM & NORMAL_BM)
			{
				if(m_pArrBMApsForPartSum[stt][left].GetSize() == 0) continue;
				baseBM.MakeBMSumString(&m_pArrBMStringApsSum, &m_pArrBMApsForPartSum[stt][left], TRUE, stt==0&&left==0);				// 접속슬래브
			}
		}
	}
	//완충슬래브
	for(stt = 0; stt < 2; stt++)
	{
		for(left = 0; left < 2; left++)
		{
			if(m_dwCalBM & NORMAL_BM)
			{
				long i = 0; for(i = 0; i < 2; i++)
				{
					if(m_pArrBMCnsForPartSum[stt][left][i].GetSize() == 0) continue;
					baseBM.MakeBMSumString(&m_pArrBMStringCnsSum, &m_pArrBMCnsForPartSum[stt][left][i], TRUE, stt==0&&left==0&&i==0);	// 완충슬래브
				}
			}
		}
	}
	//보호블럭
	for(stt = 0; stt < 2; stt++)
	{
		if(m_dwCalBM & NORMAL_BM)
			baseBM.MakeBMSumString(&m_pArrBMStringBohoSum, &m_pArrBMBohoForPartSum[stt], TRUE, stt==0&&left==0);				// 보호블럭
	}

	// 각 부위별 집계표의 계 추가.
	baseBM.MakeBMSumString(&m_pArrBMStringBridgeSum, &m_pArrBMBridgeSum, TRUE, FALSE);			// 본체
	if(m_dwCalBM & NORMAL_BM)
		baseBM.MakeBMSumString(&m_pArrBMStringGuardWallSum, &m_pArrBMGuardWallSum, TRUE, FALSE);// 방호벽
	baseBM.MakeBMSumString(&m_pArrBMStringWingSum, &m_pArrBMWingSum, TRUE, FALSE);				// 날개벽
	if(m_dwCalBM & NORMAL_BM)
	{
		baseBM.MakeBMSumString(&m_pArrBMStringApsSum, &m_pArrBMApsSum, TRUE, FALSE);			// 접속슬래브
		baseBM.MakeBMSumString(&m_pArrBMStringCnsSum, &m_pArrBMCnsSum, TRUE, FALSE);			// 완충슬래브
		baseBM.MakeBMSumString(&m_pArrBMStringBohoSum, &m_pArrBMBohoSum, TRUE, FALSE);			// 보호블럭
	}

	// 교량집계표용(본체+방호벽+날개벽+접속슬래브+완충슬래브+보호블럭)
	baseBM.MakeBMSumString(&m_pArrBMStringAllSum, &m_pArrBMSumForTitle, TRUE, TRUE);				// 본체
	if(m_dwCalBM & NORMAL_BM)
	{
		if(m_pArrBMGuardWallSum.GetCount() > 0)
			baseBM.MakeBMSumString(&m_pArrBMStringAllSum, &m_pArrBMGuardWallSum, TRUE, FALSE);		// 방호벽
	}
	if(m_pArrBMWingSum.GetCount() > 0)
		baseBM.MakeBMSumString(&m_pArrBMStringAllSum, &m_pArrBMWingSum, TRUE, FALSE);				// 날개벽
	if(m_dwCalBM & NORMAL_BM)
	{
		if(m_pArrBMApsSum.GetCount() > 0)
			baseBM.MakeBMSumString(&m_pArrBMStringAllSum, &m_pArrBMApsSum, TRUE, FALSE);			// 접속슬래브
		if(m_pArrBMCnsSum.GetCount() > 0)
			baseBM.MakeBMSumString(&m_pArrBMStringAllSum, &m_pArrBMCnsSum, TRUE, FALSE);			// 완충슬래브
		if(m_pArrBMBohoSum.GetCount() > 0)
			baseBM.MakeBMSumString(&m_pArrBMStringAllSum, &m_pArrBMBohoSum, TRUE, FALSE);			// 보호블럭
	}
	baseBM.MakeBMSumString(&m_pArrBMStringAllSum, &m_pArrBMSum, TRUE, FALSE);					// 계

	// 철근집계용
	if(m_dwCalBM & NORMAL_BM)
	{
		for(long bar = 0; bar < 2; bar++)
		{
			baseBM.MakeRebarBMString(&m_pArrRebarBMStringBridge[bar], &m_pArrRebarBMBridge[bar], TRUE);
			
			long i=0; for(i=0; i<nQtyHDan; i++)
			{
				CGuardWallRC* pGW = pBriOrg->GetGuardWallByHDan(i);
				if(!pGW) continue;
				if(pGW->IsTypeHDanRoadTotalGuardWall())	
					baseBM.MakeRebarBMString(&m_pArrRebarBMStringGuardWall[i][bar], &m_pArrRebarBMGuardWall[i][bar], TRUE);
			}
			baseBM.MakeRebarBMString(&m_pArrRebarBMStringMonument[bar], &m_pArrRebarBMMonument[bar], TRUE);

			for(stt = 0; stt < 2; stt++)
			{
				for(left = 0; left < 2; left++)
				{
					baseBM.MakeRebarBMString(&m_pArrRebarBMStringWing[stt][left][bar], &m_pArrRebarBMWing[stt][left][bar], TRUE);
					baseBM.MakeRebarBMString(&m_pArrRebarBMStringAps[stt][left][bar], &m_pArrRebarBMAps[stt][left][bar], TRUE);
					for(long cns = 0; cns < 2; cns++)
						baseBM.MakeRebarBMString(&m_pArrRebarBMStringCns[stt][left][cns][bar], &m_pArrRebarBMCns[stt][left][cns][bar], TRUE);
				}
			}

			
			for(long ix=0; ix<pBri->GetCountFooting(); ix++)
			{
				CFootingApp* pFooting = pBri->GetFooting(ix);
				if(!pFooting) continue;

				if(pFooting->m_exFooting.m_nType == EXFOOTING_TYPE_PILE)
				{
					baseBM.MakeRebarBMString(&m_pArrRebarBMStringHyunta[ix][bar], &m_pArrRebarBMHyunta[ix][bar], TRUE);
				}
			}
		}
	}
}

// 수량 제거
void COutBM::ClearBM(BOOL bRemainStdBM)
{
	long stt(0), left(0);

	if(bRemainStdBM == FALSE)
	{
		// 수집용(정제안된 순수 수량)
		AhTPADelete(&m_pArrBMBridgeStd,	(CBMData*)0);					// 교대본체(정제 안된것)

		long i=0; 
		for(i=0; i<nMaxQtyHdan; i++)
			AhTPADelete(&m_pArrBMGuardWallStd[i],	(CBMData*)0);		// 방호벽(정제 안된것)

		AhTPADelete(&m_pArrBMMonumentStd,	(CBMData*)0);		//  교명주(정제 안된것)
	
		for(stt = 0; stt < 2; stt++)
		{
			for(left = 0; left < 2; left++)
			{
				AhTPADelete(&m_pArrBMWingStd[stt][left],		(CBMData*)0);		// 교대날개벽(정제 안된것)
				AhTPADelete(&m_pArrBMApsStd[stt][left],			(CBMData*)0);		// 접속슬래브(정제 안된것)
				long i = 0; for(i = 0; i < 2; i++)
					AhTPADelete(&m_pArrBMCnsStd[stt][left][i],	(CBMData*)0);		// 완충슬래브(정제 안된것)
				AhTPADelete(&m_pArrBMBohoStd[stt],				(CBMData*)0);		// 보호블럭(정제 안된것)
			}
		}
	}

	// 출력용(정제된 수량)
	AhTPADelete(&m_pArrBMBridge, (CBMData*)0);							// 교대본체(정제 안된것) - 산출근거 출력
	AhTPADelete(&m_pArrBMStringBridge, (CBMSumString*)0);				// 교대본체(정제 안된것) - 산출근거 출력

	AhTPADelete(&m_pArrBMMonument, (CBMData*)0);						// 교명주
	AhTPADelete(&m_pArrBMStringMonument, (CBMSumString*)0);				// 교명주

	long i=0; 
	for(i=0; i<nMaxQtyHdan; i++)
	{
		AhTPADelete(&m_pArrBMGuardWall[i], (CBMData*)0);				// 방호벽(정제 안된것) - 산출근거 출력
		AhTPADelete(&m_pArrBMStringGuardWall[i], (CBMSumString*)0);		// 방호벽(정제 안된것) - 산출근거 출력
	}

	for(stt = 0; stt < 2; stt++)
	{
		for(left = 0; left < 2; left++)
		{
			AhTPADelete(&m_pArrBMWing[stt][left],				(CBMData*)0);			// 교대날개벽(정제 안된것) - 산출근거 출력
			AhTPADelete(&m_pArrBMStringWing[stt][left],			(CBMSumString*)0);		// 교대날개벽(정제 안된것) - 산출근거 출력
			AhTPADelete(&m_pArrBMAps[stt][left],				(CBMData*)0);			// 접속슬래브(정제 안된것) - 산출근거 출력
			AhTPADelete(&m_pArrBMStringAps[stt][left],			(CBMSumString*)0);		// 접속슬래브(정제 안된것) - 산출근거 출력
			long i = 0; for(i = 0; i < 2; i++)
			{
				AhTPADelete(&m_pArrBMCns[stt][left][i],			(CBMData*)0);			// 완충슬래브(정제 안된것) - 산출근거 출력
				AhTPADelete(&m_pArrBMStringCns[stt][left][i],	(CBMSumString*)0);		// 완충슬래브(정제 안된것) - 산출근거 출력
			}
		}
		AhTPADelete(&m_pArrBMBoho[stt],							(CBMData*)0);			// 보호블럭(정제 안된것) - 산출근거 출력
		AhTPADelete(&m_pArrBMStringBoho[stt],					(CBMSumString*)0);		// 보호블럭(정제 안된것) - 산출근거 출력
	}

	// 부위별집계표에 출력된 내용
	AhTPADelete(&m_pArrBMBridgeForPartSum, (CBMData*)0);				// 교대본체(정제 된것) - 산출근거 출력
	AhTPADelete(&m_pArrBMMonumentPartSum, (CBMData*)0);					// 교명주
	
	for(i=0; i<nMaxQtyHdan; i++)
		AhTPADelete(&m_pArrBMGuardWallForPartSum[i], (CBMData*)0);		// 방호벽(정제 안된것)
	
	for(stt = 0; stt < 2; stt++)
	{
		for(left = 0; left < 2; left++)
		{
			AhTPADelete(&m_pArrBMWingForPartSum[stt][left],		(CBMData*)0);			// 교대날개벽(정제 된것) - 산출근거 출력
			AhTPADelete(&m_pArrBMApsForPartSum[stt][left],		(CBMData*)0);			// 접속슬래브(정제 된것) - 산출근거 출력
			long i = 0; for(i = 0; i < 2; i++)
			{
				AhTPADelete(&m_pArrBMCnsForPartSum[stt][left][i],	(CBMData*)0);		// 완충슬래브(정제 된것) - 산출근거 출력
			}
		}
		AhTPADelete(&m_pArrBMBohoForPartSum[stt],			(CBMData*)0);				// 보호블럭(정제 된것) - 산출근거 출력
	}

	AhTPADelete(&m_pArrBMBridgeSum,				(CBMData*)0);			// 교대본체 - 교량별 집계표 출력 
	AhTPADelete(&m_pArrBMStringBridgeSum,		(CBMSumString*)0);

	AhTPADelete(&m_pArrBMGuardWallSum,			(CBMData*)0);			// 방호벽 - 교량별 집계표 출력 
	AhTPADelete(&m_pArrBMStringGuardWallSum,	(CBMSumString*)0);

	AhTPADelete(&m_pArrBMWingSum,				(CBMData*)0);			// 교대날개벽 - 교량별 집계표 출력
	AhTPADelete(&m_pArrBMStringWingSum,			(CBMSumString*)0);

	AhTPADelete(&m_pArrBMApsSum,				(CBMData*)0);			// 접속슬래브 - 교량별 집계표 출력
	AhTPADelete(&m_pArrBMStringApsSum,			(CBMSumString*)0);

	AhTPADelete(&m_pArrBMCnsSum,				(CBMData*)0);			// 완충슬래브 - 교량별 집계표 출력
	AhTPADelete(&m_pArrBMStringCnsSum,			(CBMSumString*)0);

	AhTPADelete(&m_pArrBMBohoSum,				(CBMData*)0);			// 보호블럭 - 교량별 집계표 출력
	AhTPADelete(&m_pArrBMStringBohoSum,			(CBMSumString*)0);

	AhTPADelete(&m_pArrBMStringAllSum,			(CBMSumString*)0);		// 교대본체(본체+날개벽+접속슬래브) - 교량별 집계표 출력 

	AhTPADelete(&m_pArrBMSum,					(CBMData*)0);			// 교량집계(정제 안된것) - 교량별 집계표 출력
	AhTPADelete(&m_pArrBMSumForTitle,					(CBMData*)0);			// 교량집계(정제 안된것) - 교량별 집계표 출력

	AhTPADelete(&m_pArrBMStringSepBridgeSum,			(CBMSumString*)0);
	AhTPADelete(&m_pArrBMSepBridgeSum,					(CBMData*)0);

	for(long bar = 0; bar < 2; bar++)
	{
		AhTPADelete(&m_pArrRebarBMBridgeStd[bar],	(CRebarBMData*)0);				// 교대본체(정제 안된것)
		
		long i=0; for(i=0; i<nMaxQtyHdan; i++)
			AhTPADelete(&m_pArrRebarBMGuardWallStd[i][bar],	(CRebarBMData*)0);		// 방호벽(정제 안된것)

		for(i=0; i<nMaxQtyHyunta; i++)
			AhTPADelete(&m_pArrRebarBMHyuntaStd[i][bar],	(CRebarBMData*)0);		// 현장타설(정제 안된것)

		AhTPADelete(&m_pArrRebarBMMonumentStd[bar],	(CRebarBMData*)0);				// 교명주(정제 안된것)

		for(stt = 0; stt < 2; stt++)
		{
			for(left = 0; left < 2; left++)
			{
				AhTPADelete(&m_pArrRebarBMWingStd[stt][left][bar],	(CRebarBMData*)0);			// 교대날개벽(정제 안된것)
				AhTPADelete(&m_pArrRebarBMApsStd[stt][left][bar],	(CRebarBMData*)0);			// 접속슬래브(정제 안된것)
				long i = 0; for(i = 0; i < 2; i++)
					AhTPADelete(&m_pArrRebarBMCnsStd[stt][left][i][bar],	(CRebarBMData*)0);	// 완충슬래브(정제 안된것)
			}
			AhTPADelete(&m_pArrRebarBMBohoStd[stt][bar],	(CRebarBMData*)0);					// 보호블럭(정제 안된것)
		}

		AhTPADelete(&m_pArrRebarBMBridge[bar],	(CRebarBMData*)0);					// 교대본체(정제 된것)
		
		for(i=0; i<nMaxQtyHdan; i++)
			AhTPADelete(&m_pArrRebarBMGuardWall[i][bar],	(CRebarBMData*)0);		// 방호벽(정제 된것)

		for(i=0; i<nMaxQtyHyunta; i++)
			AhTPADelete(&m_pArrRebarBMHyunta[i][bar],	(CRebarBMData*)0);		// 현장타설

		AhTPADelete(&m_pArrRebarBMMonument[bar],	(CRebarBMData*)0);				// 교명주(정제 된것)

		for(stt = 0; stt < 2; stt++)
		{
			for(left = 0; left < 2; left++)
			{
				AhTPADelete(&m_pArrRebarBMWing[stt][left][bar],	(CRebarBMData*)0);			// 교대날개벽(정제 된것)
				AhTPADelete(&m_pArrRebarBMAps[stt][left][bar],	(CRebarBMData*)0);			// 접속슬래브(정제 된것)
				long i = 0; for(i = 0; i < 2; i++)
					AhTPADelete(&m_pArrRebarBMCns[stt][left][i][bar],	(CRebarBMData*)0);	// 완충슬래브(정제 된것)
			}
			AhTPADelete(&m_pArrRebarBMBoho[stt][bar],	(CRebarBMData*)0);		// 보호블럭(정제 된것)
		}

		AhTPADelete(&m_pArrRebarBMStringBridge[bar],	(CRebarBMString*)0);			// 교대본체
		
		for(i=0; i<nMaxQtyHdan; i++)
			AhTPADelete(&m_pArrRebarBMStringGuardWall[i][bar],	(CRebarBMString*)0);	// 방호벽
		
		for(i=0; i<nMaxQtyHyunta; i++)
			AhTPADelete(&m_pArrRebarBMStringHyunta[i][bar],	(CRebarBMData*)0);		// 현장타설

		AhTPADelete(&m_pArrRebarBMStringMonument[bar],	(CRebarBMString*)0);	// 교명주
		
		for(stt = 0; stt < 2; stt++)
		{
			for(left = 0; left < 2; left++)
			{
				AhTPADelete(&m_pArrRebarBMStringWing[stt][left][bar],	(CRebarBMString*)0);		// 교대날개벽
				AhTPADelete(&m_pArrRebarBMStringAps[stt][left][bar],	(CRebarBMString*)0);		// 접속슬래브
				long i = 0; for(i = 0; i < 2; i++)
					AhTPADelete(&m_pArrRebarBMStringCns[stt][left][i][bar],	(CRebarBMString*)0);	// 완충슬래브
			}
			AhTPADelete(&m_pArrRebarBMStringBoho[stt][bar],	(CRebarBMString*)0);		// 보호블럭
		}
	}
}

// 수량을 ViewBM에 등록함.
void COutBM::SetViewBM(CTypedPtrArray <CObArray, CBMData*> *pArrBM, BOOL bNewCreateBM)
{
	if(!pArrBM) return;

	long i = 0; for(i = 0; i < pArrBM->GetSize(); i++)
	{
		CString sBigClass	= pArrBM->GetAt(i)->m_sBigClass;
		long nBigIndex		=  pArrBM->GetAt(i)->m_nIdxBigClass;
		
		if(m_dwCalBM & NORMAL_BM)
		{
			BOOL bIsView = TRUE;
			if(bNewCreateBM) //새로 수량산출 했을 경우에만 공종 옵션 디폴트 상태로 만들기
			{
				switch(nBigIndex)
				{
				case BM_BIG_DECKFINISH: //수량 산출로 했을 경우에는 안보여
					bIsView = FALSE;
					break;
				default:
					bIsView = TRUE;
					break;
				}
			}
			
			m_pStd->m_pViewBM->AddBM(sBigClass, -1, bIsView);
		}
		else
		{
			m_pStd->m_pViewBMCommon->AddBM(sBigClass);
		}
	}
}

// 수량을 ViewBM에 적용시켜서 필요 없는거는 제거함.
void COutBM::ApplyViewBM(CTypedPtrArray <CObArray, CBMData*> *pArrBM)
{
	if(!pArrBM) return;

	long i = 0; for(i = 0; i < pArrBM->GetSize(); i++)
	{
		if(m_dwCalBM & NORMAL_BM)
		{
			if(!m_pStd->m_pViewBM->IsCheckView(pArrBM->GetAt(i)->m_sBigClass))
			{
				delete pArrBM->GetAt(i);
				pArrBM->RemoveAt(i);
				i--;
			}
		}
		else
		{
			if(!m_pStd->m_pViewBMCommon->IsCheckView(pArrBM->GetAt(i)->m_sBigClass))
			{
				delete pArrBM->GetAt(i);
				pArrBM->RemoveAt(i);
				i--;
			}
		}
	}
}

// 수량을 셀참조식으로 출력할지에 대한 옵션 적용
// 단, 임의수식인 경우 BMData의 옵션이 우선권을 가짐
void COutBM::ApplyRefCellBM(CTypedPtrArray <CObArray, CBMData*> *pArrBM)
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
void COutBM::ApplyOptionBMStd(CTypedPtrArray <CObArray, CBMData*> *pArrBM)
{
	ApplyViewBM(pArrBM);
	ApplyRefCellBM(pArrBM);
}

// 콘크리트 수량
// BOOL bForDeductOfReFill : 되메우기 수량의 지반선 이하의 구체공제 수량을 위한 것임
// 공제용인 경우 영역을 구한뒤 지반선을 기준으로 모두 잘라낸다.
void COutBM::MakeBMConcreteMain(BOOL bForDeductOfReFill)
{
	CRcBridgeRebar *pBri	= GetRcBridgeRebar();
	if(!pBri) return;
	CLineInfo *pLine	= pBri->GetLineBase();
	if(!pLine) return;
	CRcBridgeRebar *pBriOrg	= GetRcBridgeRebar(TRUE);
	if(!pBriOrg) return;

	// 영역 구분.
	CalAreaConc(pBri, bForDeductOfReFill);

	// (ARCBRIDGE-16530) 실적수량을 선택 했을때 기초, 슬래브, 접속슬래브 세개의 항목이'바닥 및 보'항목으로 묶이는데 이름만 바뀌고 Index는 각자 다르다.
	// 총계 집계시 Merge가 되지 않고 항목도 따로 생겨서 전체 총계 테이블이 망가지고있다.
	// BM_SMALL_CONCRETE_FLOOR를 추가해서 '바닥 및 보'를 따로 집계 했다.
	long nTypeBM	= m_pStd->m_pARcBridgeDataStd->m_nTypeBMApply;
	HGBOOL bFootingLabeledFloor = (nTypeBM != TYPE_BM_NORMAL && m_pStd->m_pARcBridgeDataStd->m_bSepFootingForSiljuk == FALSE)? TRUE : FALSE;
	long nSize = 0;

	// 기초
	// 일반형 : 평면영역에 기초높이를 곱하면 만사 OK
	// slope형 : slope형은 날개벽따라 꺽이는 형태가 없다고 간주하고, 정면 영역에 기초길이를 곱한다.
	long jijum = 0; 
	for(jijum = 0; jijum <= pBri->m_nQtyJigan; jijum++)
	{
		if(pBri->IsBoxType()) break; // 본체에서 구함

		// 토공용으로 구하는 경우 토공수량에 추가한다.
		// 단 자세한 내용은 이 함수를 호출 하는 곳에서 수정을 해 줘야 됨.
		CTypedPtrArray <CObArray, CBMData*> *pArrBMStd	= 
			bForDeductOfReFill ? &m_pStd->m_pArrOutBMTogong.GetAt(m_nBri)->m_pArrBMTogongStd[jijum] : &m_pArrBMBridgeStd;

		BOOL bInner = (jijum == 0 || jijum == pBri->m_nQtyJigan)? FALSE : TRUE;
		CFootingApp *pFooting	= pBri->GetFootingByJijumNumber(jijum);
		if(pFooting)
		{
			// 일반형일때 ..
			if(pFooting->m_nType == FOOTING_TYPE_BASIC || pFooting->m_nType == FOOTING_TYPE_BASIC_BOTH)
			{                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   
				// 일반수량 구할때는 기초 높이 모두 구하면 되지만, 되메우기 공제용인 경우는 지반선에 따라 적용 높이가 달라짐
				// 지반이 기초 중간에 있다면 기초 높이를 기초하면에서 지반까지로 제한함
				// 단 기초에 대해서는 지반이 기초 안에서 오르락내리릭하는 경우에도 지점의 el에 따라 수평으로 처리됨(차후 수정 필요)
				double dHeight = pFooting->GetHeight();
				if(bForDeductOfReFill)
				{
					dHeight	= pFooting->m_pArrAreaConc.GetSize() > 0 ? pFooting->m_pArrAreaConc.GetAt(0)->m_dHeight : 0;
				}

				if(Compare(dHeight, 0.0, "<=")) continue;

				nSize	= pFooting->m_pArrAreaConcPlane.GetSize();
				long j = 0; for(j = 0; j < nSize; j++)
				{
					CTwinVectorAreaShape *pArea	= pFooting->m_pArrAreaConcPlane.GetAt(j);

					CBMData *pBM				= new CBMData;
					pBM->m_nNumbering			= pArea->m_nNum;
					pBM->m_nLevelTotal			= 2;
					pBM->m_nIdxBigClass			= BM_BIG_CONCRETE;
					pBM->m_sBigClass			= GetStringBigClass(pBM->m_nIdxBigClass);
					pBM->m_nIdxMidClass			= BM_MID_CONCRETE_MAIN;
					pBM->m_sMidClass			= GetStringMidClass(pBM->m_nIdxMidClass, NULL, bInner? ePartInFoot : (jijum == 0? ePartFootStt : ePartFootEnd));
					pBM->m_nIdxSmallClass		= bFootingLabeledFloor? BM_SMALL_CONCRETE_FLOOR : BM_SMALL_CONCRETE_FOOTING;
					pBM->m_sSmallClass			= GetStringSmallClass(pBM->m_nIdxSmallClass);
					pBM->m_nIdxItemClass		= BM_ITEM_CONCRETE_FOOTING;
					pBM->m_sItemClass			= GetStringItemClass(pBM->m_nIdxItemClass);
					pBM->m_nTypeResult			= RESULT_VOLUMN;
					pBM->m_sUnit				= "㎥";
					pBM->m_nShape				= pArea->m_nTypeShape;
					pBM->m_dV[WidthLower]		= Round(toM(pArea->m_dWidthLower),3);
					pBM->m_dV[WidthUpper]		= Round(toM(pArea->m_dWidthUpper),3);
					pBM->m_dV[WidthDiffLeft]	= Round(toM(pArea->m_dWidthDiffLeft),3);
					pBM->m_dV[WidthDiffRight]	= Round(toM(pArea->m_dWidthDiffRight),3);
					pBM->m_dV[Width]			= Round(toM(pArea->m_dWidth),3);
					pBM->m_dV[HeightLeft]		= Round(toM(pArea->m_dHeightLeft),3);
					pBM->m_dV[HeightRight]		= Round(toM(pArea->m_dHeightRight),3);
					pBM->m_dV[Height]			= Round(toM(pArea->m_dHeight),3);
					pBM->m_dV[Length]			= Round(toM(dHeight),3);
					pBM->m_sInfo				= "기초콘크리트";
					pBM->CalculateResult();
					
					pArrBMStd->Add(pBM);
				}
			}
			// 경사형일때...
			// 문제점 : 확폭일때는 계산이 곤란해지고, 사각이 있을때 한쪽만 직각처리하면 곤란해짐. ㅡㅡ;
			else if(   pFooting->m_nType == FOOTING_TYPE_LEFTSLOPE
					|| pFooting->m_nType == FOOTING_TYPE_RIGHTSLOPE
					|| pFooting->m_nType == FOOTING_TYPE_BOTHSLOPE
					|| pFooting->m_nType == FOOTING_TYPE_BASIC_LEFTSLOPE
					|| pFooting->m_nType == FOOTING_TYPE_BASIC_RIGHTSLOPE)
			{
				double dLenFooting	= pFooting->GetLength();

				nSize	= pFooting->m_pArrAreaConc.GetSize();
				long j = 0; for(j = 0; j < nSize; j++)
				{
					CTwinVectorAreaShape *pArea	= pFooting->m_pArrAreaConc.GetAt(j);

					CBMData *pBM				= new CBMData;
					pBM->m_nNumbering			= pArea->m_nNum;
					pBM->m_nLevelTotal			= 2;
					pBM->m_nIdxBigClass			= BM_BIG_CONCRETE;
					pBM->m_sBigClass			= GetStringBigClass(pBM->m_nIdxBigClass);
					pBM->m_nIdxMidClass			= BM_MID_CONCRETE_MAIN;
					pBM->m_sMidClass			= GetStringMidClass(pBM->m_nIdxMidClass, NULL, bInner? ePartInFoot : (jijum == 0? ePartFootStt : ePartFootEnd));
					pBM->m_nIdxSmallClass		= bFootingLabeledFloor? BM_SMALL_CONCRETE_FLOOR : BM_SMALL_CONCRETE_FOOTING;
					pBM->m_sSmallClass			= GetStringSmallClass(pBM->m_nIdxSmallClass);
					pBM->m_nIdxItemClass		= BM_ITEM_CONCRETE_FOOTING;
					pBM->m_sItemClass			= GetStringItemClass(pBM->m_nIdxItemClass);
					pBM->m_nTypeResult			= RESULT_VOLUMN;
					pBM->m_sUnit				= "㎥";
					pBM->m_nShape				= pArea->m_nTypeShape;
					pBM->m_dV[WidthLower]		= Round(toM(pArea->m_dWidthLower),3);
					pBM->m_dV[WidthUpper]		= Round(toM(pArea->m_dWidthUpper),3);
					pBM->m_dV[WidthDiffLeft]	= Round(toM(pArea->m_dWidthDiffLeft),3);
					pBM->m_dV[WidthDiffRight]	= Round(toM(pArea->m_dWidthDiffRight),3);
					pBM->m_dV[Width]			= Round(toM(pArea->m_dWidth),3);
					pBM->m_dV[HeightLeft]		= Round(toM(pArea->m_dHeightLeft),3);
					pBM->m_dV[HeightRight]		= Round(toM(pArea->m_dHeightRight),3);
					pBM->m_dV[Height]			= Round(toM(pArea->m_dHeight),3);
					pBM->m_dV[Length]			= Round(toM(dLenFooting),3);
					pBM->m_sInfo				= "기초콘크리트";
					pBM->CalculateResult();
					
					pArrBMStd->Add(pBM);
				}
			}
		}
	}
	
	// 본체(확폭지원 안됨)
	// 현재 본체는 사거리로 영역이 계산되어 있다. 콘크리트 수량 산출시에는 직거리에 길이를 곱해야 한다.
	// 영역의 폭을 직거리로 계산해주자.
	CDPoint vAngStt	= pBri->GetAngleJijum(0);
	CRebarPlacing rb;
	double dLenBridge	= pBri->GetWidthSlabAct(pBri->m_dStationBridgeStt, CDPoint(0, 1), 0, TRUE);
	nSize	= pBri->m_pArrAreaConc.GetSize();
	long i = 0; for(i = 0; i < nSize; i++)
	{
		// 시점 종점.. 그리고 
		CTwinVectorAreaShape *pArea	= pBri->m_pArrAreaConc.GetAt(i);

		// 영역의 sta별 지점 각도를 적용해서 본체 길이를 파악한다.
		// 단 상부슬래브의 경우 평면에서 본 형태로 구하는게 좋을꺼같다.
		// 그러나현재는 지원안됨.
		CDPointArray xyArr;
		pArea->m_tvArr.GetToPointArray(xyArr);
		double dStaLeft		= rb.GetXyEdgePoint(xyArr, UPLEFT_AREA).x;
		double dStaRight	= rb.GetXyEdgePoint(xyArr, UPRIGHT_AREA).x;
		
		double dStaMid		= (dStaLeft + dStaRight)/2;
		CDPoint xyDir		= pBri->GetAngleByStation(dStaMid);
		
		if(bForDeductOfReFill)
		{
			//ARCBRIDGE-3080 가각을 포함하기로 함
			BOOL bStt= TRUE;
			for (long ix=0; ix< pArea->m_tvArr.GetSize(); ix++)
			{
				CTwinVector tv = pArea->m_tvArr.GetAt(ix);
				if(tv.m_sInfo.Find(_T("종점")) > -1)
				{
					bStt = FALSE;
					break;
				}
			}

			CTwinVector tvSlab;
			pBri->GetTvWingWallPlane(bStt);
			pBri->m_tvArrPlaneWingWall.GetTvByInfo("슬래브끝단외측", tvSlab);
			dLenBridge = tvSlab.GetLength();
		}
		else
		{
			dLenBridge			= pBri->GetWidthSlabAct(dStaMid, xyDir, 0, FALSE);
		}		

		long nTypeConcrete = ePartUpperSlab;
		if(pArea->m_sInfo.Find(_T("하부슬래브")) != -1 || pArea->m_sInfo.Find(_T("부상방지 저판")) != -1)
			nTypeConcrete = ePartLowerSlab;
		else if(pArea->m_sInfo.Find(_T("상부슬래브")) != -1 || pArea->m_sInfo.Find(_T("헌치")) != -1)
			nTypeConcrete = ePartUpperSlab;
		else
		{
			BOOL bStt = (pArea->m_sInfo.Find(_T("시점")) != -1);
			nTypeConcrete = bStt? ePartOutWallStt : ePartOutWallEnd;
		}
		
		// 교량길이에서 공제 해야 되는 상황 고려 /////////////////////////////////////////////
		
		// [1] 시종점 브라켓은 날개벽 고려해야 됨.
		// 수정예정 : 단변화가 세로인지 가로인지에 따라서 정확하게 계산이 되어야 함.
		if(pArea->m_sInfo == "시점브라켓" || pArea->m_sInfo == "종점브라켓")
		{
			BOOL bStt = pArea->m_sInfo == "시점브라켓";
			pBri->GetTvWingWallPlane(bStt, TRUE, TRUE);		// 브라켓 길이 계산
			CTwinVector tv = pBri->m_tvArrPlaneWingWall.GetTvByInfo("브라켓");
			dLenBridge = tv.GetLength();
		}

		// [2] 브라켓을 제외한 곳의 헌치들은 끝단에서 헌치까지 들어간 거리 고려
		double dRealLenBridge = 0;
		CTwinVector tvSlab;
		BOOL bSttWall		= pArea->m_sInfo.Find(_T("시점"),0) > -1? TRUE : FALSE;

		pBri->GetTvWingWallPlane(bSttWall);			
		pBri->m_tvArrPlaneWingWall.GetTvByInfo("슬래브끝단외측", tvSlab);

		dRealLenBridge = tvSlab.GetLength();
		
		if(pArea->m_sInfo.Find("벽체헌치", 0) != -1 || pArea->m_sInfo.Find("내측헌치", 0) != -1)
		{
			dLenBridge = pArea->m_sInfo.Find("내측헌치", 0) != -1? dLenBridge : dRealLenBridge;
			double dJijumAng = xyDir.GetAngleRadian();
			
			dLenBridge	-= (pBri->m_dDH[0] + pBri->m_dDH[1])/sin(dJijumAng);
			if(pBri->m_bCantilSlab[0])
				dLenBridge -= pBri->m_dCantilB[0]/sin(dJijumAng);
			if(pBri->m_bCantilSlab[1])
				dLenBridge -= pBri->m_dCantilB[1]/sin(dJijumAng);
		}
		else if(pArea->m_sInfo.Find(_T("벽체"), 0) != -1)
		{
			dLenBridge = dRealLenBridge;
		}
		
		//////////////////////////////////////////////////////////////////////////////////////////////

		// 토공용으로 구하는 경우 토공수량에 추가한다.
		// 단 자세한 내용은 이 함수를 호출 하는 곳에서 수정을 해 줘야 됨.
		long nJ	= pBri->GetJijumWallByStation(dStaMid);
		if(pBri->IsBoxType()) nJ = 0;
		CTypedPtrArray <CObArray, CBMData*> *pArrBMStd	= 
			bForDeductOfReFill ? &m_pStd->m_pArrOutBMTogong.GetAt(m_nBri)->m_pArrBMTogongStd[nJ] : &m_pArrBMBridgeStd;

		CBMData *pBM				= new CBMData;
		pBM->m_nNumbering			= pArea->m_nNum;
		pBM->m_nLevelTotal			= 2;
		pBM->m_nIdxBigClass			= BM_BIG_CONCRETE;
		pBM->m_sBigClass			= GetStringBigClass(pBM->m_nIdxBigClass);
		pBM->m_nIdxMidClass			= BM_MID_CONCRETE_MAIN;
		pBM->m_sMidClass			= GetStringMidClass(pBM->m_nIdxMidClass, NULL, nTypeConcrete);
		pBM->m_nIdxSmallClass		= BM_SMALL_CONCRETE_MAIN;

		if(pArea->m_sInfo == _T("상부슬래브") || pArea->m_sInfo == _T("하부슬래브") || pArea->m_sInfo == _T("부상방지 저판"))
		{
			// 상부슬래브는 일반수량산출기준일땐 본체로 들어가지만
			// 실적수량 산출기준일때는 바닥 및 보로 들어가야 한다.
			if(nTypeBM != TYPE_BM_NORMAL)
			{
				// pBM->m_nIdxSmallClass	= BM_SMALL_CONCRETE_FOOTING;
				pBM->m_nIdxSmallClass	= BM_SMALL_CONCRETE_FLOOR;
			}

			// 상부슬래브는 옵션에 따라서 선혀거리로 계산된 길이를 사용하기도 한다. ////
			if(pBri->m_nTypeSlabLength == 1)
			{
				pArea->m_dWidth	= pBriOrg->GetLengthSlabReal(0);
				pArea->m_dWidthLower	= pArea->m_dWidth;
				pArea->m_dWidthUpper	= pArea->m_dWidth;
				dLenBridge	= pBriOrg->GetWidthSlabAct(pBriOrg->GetStationBridgeStt(FALSE), CDPoint(0, 1));
				xyDir = CDPoint(0, 1);
			}
			///////////////////////////////////////////////////////////////////////
		}
		pBM->m_sSmallClass = GetStringSmallClass(pBM->m_nIdxSmallClass);
		// ## "수량 산출시 기초를 바닥 및 보와 분리" 옵션에 따라 
		// (pBM->m_nIdxSmallClass == BM_SMALL_CONCRETE_FOOTING)면 기초로 설정이 되어 
		// 다시 설정.(2010.02.09, 홍명기)
// 		if(nTypeBM != TYPE_BM_NORMAL)
// 		{
// 			if(pArea->m_sInfo == _T("상부슬래브") || pArea->m_sInfo == _T("하부슬래브") || pArea->m_sInfo == _T("부상방지 저판"))
// 			{
// 				pBM->m_sSmallClass = _T("바닥 및 보");
// 			}
// 		}
		pBM->m_nIdxItemClass		= BM_ITEM_CONCRETE_MAIN;
		pBM->m_sItemClass			= GetStringItemClass(pBM->m_nIdxItemClass);
		pBM->m_nTypeResult			= RESULT_VOLUMN;
		pBM->m_sUnit				= "㎥";
		pBM->m_nShape				= pArea->m_nTypeShape;
		pBM->m_dV[Width]			= Round(toM(pArea->m_dWidth * xyDir.y),3);
		pBM->m_dV[WidthLower]		= Round(toM(pArea->m_dWidthLower * xyDir.y),3);
		pBM->m_dV[WidthUpper]		= Round(toM(pArea->m_dWidthUpper * xyDir.y),3);
		pBM->m_dV[WidthDiffLeft]	= Round(toM(pArea->m_dWidthDiffLeft * xyDir.y),3);
		pBM->m_dV[WidthDiffRight]	= Round(toM(pArea->m_dWidthDiffRight * xyDir.y),3);
		pBM->m_dV[Height]			= Round(toM(pArea->m_dHeight),3);
		pBM->m_dV[HeightLeft]		= Round(toM(pArea->m_dHeightLeft),3);
		pBM->m_dV[HeightRight]		= Round(toM(pArea->m_dHeightRight),3);
		pBM->m_dV[Length]			= Round(toM(dLenBridge),3);
		pBM->m_dV[ArcAreaR]			= Round(toM(toM(pArea->m_dArcAreaR)),3);
		pBM->m_dV[ArcAreaL]			= Round(toM(toM(pArea->m_dArcAreaL)),3);
		pBM->m_dV[ArcLengthR]		= Round(toM(pArea->m_dArcLengthR),3);
		pBM->m_dV[ArcLengthL]		= Round(toM(pArea->m_dArcLengthL),3);
		
		pBM->CalculateResult();

		if(pArea->m_sInfo == "리브")	// 아치교 리브
		{
			CTwinVector tv;
			CDPointArray vArr;
			long nSize = pBri->m_tvArrVertSectionRib[0].GetSize();
			long n=0; for(n=0; n<nSize; n++)
			{
				tv = pBri->m_tvArrVertSectionRib[0].GetAt(n);
				vArr.Add(tv.m_v1);
				if(n==nSize-1)
					vArr.Add(tv.m_v2);
			}
			nSize = pBri->m_tvArrVertSectionRib[1].GetSize();
			for(n=nSize-1; n>=0; n--)
			{
				tv = pBri->m_tvArrVertSectionRib[1].GetAt(n);
				vArr.Add(tv.m_v2);
				if(n==0)
					vArr.Add(tv.m_v1);
			}
			double dArea = pBri->GetPolygonArea(vArr, vArr.GetSize());
			CString strTail;
			strTail.Format(_T("리브 면적 × %.3f"), pBM->m_dV[Length]);
			pBM->m_sNoteTail = strTail;
			pBM->m_dV[Result] = toM(toM(dArea)) * pBM->m_dV[Length];
		}

		pArrBMStd->Add(pBM);
	}

	// 중간지점 벽체
	long nCountInWall	= pBri->GetCountInWall();
	for(long wall = 0; wall < nCountInWall; wall++)
	{
		CWallApp *pWall	= pBri->GetInWall(wall);
		if(!pWall) continue;
		if(!pWall->m_bIs) continue;
		long nJ	= wall + 1;
		CDPoint xyDir	= pBri->GetAngleJijum(nJ);

		double dLenWall	= pWall->m_tvPathPlan.GetLength();
		nSize	= pWall->m_pArrAreaConc.GetSize();
		for(i = 0; i < nSize; i++)
		{
			CTwinVectorAreaShape *pArea	= pWall->m_pArrAreaConc.GetAt(i);

			// 영역의 sta별 지점 각도를 적용해서 본체 길이를 파악한다.
			// 단 상부슬래브의 경우 평면에서 본 형태로 구하는게 좋을꺼같다.
			// 그러나현재는 지원안됨.
			CDPointArray xyArr;
			pArea->m_tvArr.GetToPointArray(xyArr);
// 			double dStaLeft	= rb.GetXyEdgePoint(xyArr, UPLEFT_AREA).x;
// 			double dStaRight	= rb.GetXyEdgePoint(xyArr, UPRIGHT_AREA).x;

			// 토공용으로 구하는 경우 토공수량에 추가한다.
			// 단 자세한 내용은 이 함수를 호출 하는 곳에서 수정을 해 줘야 됨.
			CTypedPtrArray <CObArray, CBMData*> *pArrBMStd	= 
				bForDeductOfReFill ? &m_pStd->m_pArrOutBMTogong.GetAt(m_nBri)->m_pArrBMTogongStd[nJ] : &m_pArrBMBridgeStd;

			CBMData *pBM				= new CBMData;
			pBM->m_nNumbering			= pArea->m_nNum;
			pBM->m_nLevelTotal			= 2;
			pBM->m_nIdxBigClass			= BM_BIG_CONCRETE;
			pBM->m_sBigClass			= GetStringBigClass(pBM->m_nIdxBigClass);
			pBM->m_nIdxMidClass			= BM_MID_CONCRETE_MAIN;
			pBM->m_sMidClass			= GetStringMidClass(pBM->m_nIdxMidClass, NULL, ePartInWall);
			pBM->m_nIdxSmallClass		= BM_SMALL_CONCRETE_MAIN;
			pBM->m_sSmallClass			= GetStringSmallClass(pBM->m_nIdxSmallClass);
			pBM->m_nIdxItemClass		= BM_ITEM_CONCRETE_MAIN;
			pBM->m_sItemClass			= GetStringItemClass(pBM->m_nIdxItemClass);
			pBM->m_nTypeResult			= RESULT_VOLUMN;
			pBM->m_sUnit				= "㎥";
			if(pWall->IsRoundRectWall())
			{
				pBM->m_nShape				= SHAPE_TV_ROUNDRECTANGLE_PLANE;
				pBM->m_dV[WidthLower]		= Round(toM(pWall->m_dW),3);
				pBM->m_dV[Length]			= Round(toM(dLenWall),3);
				pBM->m_dV[Radius]			= Round(toM(pWall->m_dW/2),3);
				pBM->m_dV[Height]			= Round(toM(pArea->m_dHeight),3);
				pBM->CalculateResult();
			}
			else if(pWall->m_bTopIsColumn)
			{
				CColumnApp *pColumn			= &pWall->m_Column;
				pBM->m_nShape				= SHAPE_TV_ROUNDRECTANGLE_PLANE;
				pBM->m_dV[WidthLower]		= Round(toM(pColumn->m_dB[iOUTTER][iUPPER]),3);
				pBM->m_dV[Length]			= Round(toM(pColumn->m_dH[iOUTTER][iUPPER]),3);
				pBM->m_dV[Radius]			= Round(toM(pColumn->m_dR[iOUTTER][iUPPER]),3);
				pBM->m_dV[Height]			= Round(toM(pArea->m_dHeight),3);
				pBM->CalculateResult();
			}
			else
			{
				pBM->m_nShape				= pArea->m_nTypeShape;
				pBM->m_dV[Width]			= Round(toM(pArea->m_dWidth * xyDir.y),3);
				pBM->m_dV[WidthLower]		= Round(toM(pArea->m_dWidthLower * xyDir.y),3);
				pBM->m_dV[WidthUpper]		= Round(toM(pArea->m_dWidthUpper * xyDir.y),3);
				pBM->m_dV[WidthDiffLeft]	= Round(toM(pArea->m_dWidthDiffLeft * xyDir.y),3);
				pBM->m_dV[WidthDiffRight]	= Round(toM(pArea->m_dWidthDiffRight * xyDir.y),3);
				pBM->m_dV[Height]			= Round(toM(pArea->m_dHeight),3);
				pBM->m_dV[HeightLeft]		= Round(toM(pArea->m_dHeightLeft),3);
				pBM->m_dV[HeightRight]		= Round(toM(pArea->m_dHeightRight),3);
				pBM->m_dV[Length]			= Round(toM(dLenWall),3);
				pBM->m_dV[ArcAreaL]			= Round(toM(toM(pArea->m_dArcAreaL)), 3);
				pBM->m_dV[ArcAreaR]			= Round(toM(toM(pArea->m_dArcAreaR)), 3);
				pBM->CalculateResult();
			}
			
			
			pArrBMStd->Add(pBM);

			// 기둥식은 기둥수 만큼 추가 해줌
			if(pWall->m_bTopIsColumn)
			{
				for(long col = 1; col < pWall->m_nCountColumn; col++)
				{
					CBMData *pBMTmp	= new CBMData(pBM);
					pArrBMStd->Add(pBMTmp);
				}
			}
		}
	}

// 일반형
// #define EXFOOTING_TYPE_LEAN			0	// 일반 버림 콘크리트(직접기초)
// #define EXFOOTING_TYPE_MASS			1	// MASS
// #define EXFOOTING_TYPE_PILE			2	// 파일
// #define EXFOOTING_TYPE_ALONE_HYUNTAPILE	3	// 단일현장 타설 말뚝

// 함형라멘교 : 직접기초,MASS기초,말뚝기초,잡석기초,부력방지 Anchor
// #define BOXFOOTING_TYPE_LEAN			0
// #define BOXFOOTING_TYPE_MASS			1
// #define BOXFOOTING_TYPE_PILE			2
// #define BOXFOOTING_TYPE_STONE		3
// #define BOXFOOTING_TYPE_ANCHOR		4


	// 버림콘크리트(LEAN) 및 MASS 콘크리트
	// 일반형 : 평면영역에 기초높이를 곱하면 만사 OK
	// slope형 : slope형은 날개벽따라 꺽이는 형태가 없다고 간주하고, 정면 영역에 기초길이를 곱한다.
	// Box형 : 버림콘크리트가 하나로 되어있다. Jijum = 0

	long nQtyJijum = pBri->IsBoxType() ? 1 : pBri->GetCountJijum();
	for(jijum = 0; jijum < nQtyJijum; jijum++)
	{
		// 토공용으로 구하는 경우 토공수량에 추가한다.
		// 단 자세한 내용은 이 함수를 호출 하는 곳에서 수정을 해 줘야 됨.
		CTypedPtrArray <CObArray, CBMData*> *pArrBMStd	= 
			bForDeductOfReFill ? &m_pStd->m_pArrOutBMTogong.GetAt(m_nBri)->m_pArrBMTogongStd[jijum] : &m_pArrBMBridgeStd;

		CFootingApp *pFooting	= pBri->GetFootingByJijumNumber(jijum);
		if(pFooting == NULL) continue;
		CExFootingApp *pExFooting	= &pFooting->m_exFooting;

		// 버림콘크리트(LEAN)
		long nTypeExFooting = pBri->IsBoxType() ? pBri->m_nTypeLower : pExFooting->m_nType;
		if(nTypeExFooting != EXFOOTING_TYPE_MASS)
		{
			// 일반형일때 ..
			if(pFooting->m_nType == FOOTING_TYPE_BASIC || pFooting->m_nType == FOOTING_TYPE_BASIC_BOTH || pFooting->m_nType == FOOTING_TYPE_BOX)
			{
				double dHeight = pExFooting->m_dEFHR;

				if(pBri->IsBoxType() == FALSE)
				{
					nSize = pExFooting->m_pArrAreaConcPlane.GetSize();
					long j=0; for(j=0; j<nSize; j++)
					{
						CTwinVectorAreaShape *pArea	= pExFooting->m_pArrAreaConcPlane.GetAt(j);
						CBMData *pBM				= new CBMData;
						pBM->m_nNumbering			= pArea->m_nNum;
						pBM->m_nLevelTotal			= 2;
						pBM->m_nIdxBigClass			= BM_BIG_CONCRETE;
						pBM->m_sBigClass			= GetStringBigClass(pBM->m_nIdxBigClass);
						pBM->m_nIdxMidClass			= BM_MID_CONCRETE_LEAN;
						pBM->m_sMidClass			= GetStringMidClass(pBM->m_nIdxMidClass);
						pBM->m_nIdxSmallClass		= BM_SMALL_CONCRETE_LEAN;
						pBM->m_sSmallClass			= GetStringSmallClass(pBM->m_nIdxSmallClass);
						pBM->m_nTypeResult			= RESULT_VOLUMN;
						pBM->m_sUnit				= "㎥";
						pBM->m_nShape				= pArea->m_nTypeShape;
						pBM->m_dV[WidthLower]		= Round(toM(pArea->m_dWidthLower),3);
						pBM->m_dV[WidthUpper]		= Round(toM(pArea->m_dWidthUpper),3);
						pBM->m_dV[WidthDiffLeft]	= Round(toM(pArea->m_dWidthDiffLeft),3);
						pBM->m_dV[WidthDiffRight]	= Round(toM(pArea->m_dWidthDiffRight),3);
						pBM->m_dV[Width]			= Round(toM(pArea->m_dWidth),3);
						pBM->m_dV[HeightLeft]		= Round(toM(pArea->m_dHeightLeft),3);
						pBM->m_dV[HeightRight]		= Round(toM(pArea->m_dHeightRight),3);
						pBM->m_dV[Height]			= Round(toM(pArea->m_dHeight),3);
						pBM->m_dV[Length]			= Round(toM(dHeight),3);
						pBM->CalculateResult();
						pArrBMStd->Add(pBM);
					}
				}
				else
				{
					double dPRF_W = pBri->m_bExistProtectionRiseFooting ? pBri->m_dPRF_W : 0 ;
					double dLength = bForDeductOfReFill? dLenBridge : fabs(pBri->m_dWidthSlabLeft) + fabs(pBri->m_dWidthSlabRight);
					dLength += pFooting->m_exFooting.m_dEFWB + pFooting->m_exFooting.m_dEFWF;

					double dHeight = pExFooting->m_dEFHR;
					double dSttLength = pBri->m_dWS + dPRF_W + pExFooting->m_dEFWL;
					double dEndLength = pBri->m_dWE + dPRF_W + pExFooting->m_dEFWR;
					double dWidth = pBri->GetLengthBridge(FALSE) + (dSttLength / pBri->GetAngleJijum(0).y) + (dEndLength / pBri->GetAngleJijum(pBri->m_nQtyJigan).y);
					CTwinVectorAreaShape *pArea	= pExFooting->m_pArrAreaConcPlane.GetAt(0);
					if(!pArea) return;					

					CBMData *pBM				= new CBMData;
					pBM->m_nNumbering			= pArea->m_nNum;
					pBM->m_nLevelTotal			= 2;
					pBM->m_nIdxBigClass			= BM_BIG_CONCRETE;
					pBM->m_sBigClass			= GetStringBigClass(pBM->m_nIdxBigClass);
					pBM->m_nIdxMidClass			= BM_MID_CONCRETE_LEAN;
					pBM->m_sMidClass			= GetStringMidClass(pBM->m_nIdxMidClass);
					pBM->m_nIdxSmallClass		= BM_SMALL_CONCRETE_LEAN;
					pBM->m_sSmallClass			= GetStringSmallClass(pBM->m_nIdxSmallClass);
					pBM->m_nTypeResult			= RESULT_VOLUMN;
					pBM->m_sUnit				= "㎥";
					pBM->m_nShape				= SHAPE_RECTANGLE;
					pBM->m_dV[WidthLower]		= Round(toM(dWidth),3);
					pBM->m_dV[Height]			= Round(toM(dHeight),3);
					pBM->m_dV[Length]			= Round(toM(dLength),3);
					pBM->CalculateResult();
					pArrBMStd->Add(pBM);
				}
			}
			// 경사형일때...
			// 문제점 : 확폭일때는 계산이 곤란해지고, 사각이 있을때 한쪽만 직각처리하면 곤란해짐. ㅡㅡ;
			else if(   pFooting->m_nType == FOOTING_TYPE_LEFTSLOPE
					|| pFooting->m_nType == FOOTING_TYPE_RIGHTSLOPE
					|| pFooting->m_nType == FOOTING_TYPE_BOTHSLOPE
					|| pFooting->m_nType == FOOTING_TYPE_BASIC_LEFTSLOPE
					|| pFooting->m_nType == FOOTING_TYPE_BASIC_RIGHTSLOPE)
			{
				double dLenFooting = pExFooting->GetLength();

				nSize = pExFooting->m_pArrAreaConc.GetSize();
				long j=0; for(j=0; j<nSize; j++)
				{
					CTwinVectorAreaShape *pArea	= pExFooting->m_pArrAreaConc.GetAt(j);

					CBMData *pBM				= new CBMData;
					pBM->m_nNumbering			= pArea->m_nNum;
					pBM->m_nLevelTotal			= 2;
					pBM->m_nIdxBigClass			= BM_BIG_CONCRETE;
					pBM->m_sBigClass			= GetStringBigClass(pBM->m_nIdxBigClass);
					pBM->m_nIdxMidClass			= BM_MID_CONCRETE_LEAN;
					pBM->m_sMidClass			= GetStringMidClass(pBM->m_nIdxMidClass);
					pBM->m_nIdxSmallClass		= BM_SMALL_CONCRETE_LEAN;
					pBM->m_sSmallClass			= GetStringSmallClass(pBM->m_nIdxSmallClass);
					pBM->m_nTypeResult			= RESULT_VOLUMN;
					pBM->m_sUnit				= "㎥";
					pBM->m_nShape				= pArea->m_nTypeShape;
					pBM->m_dV[WidthLower]		= Round(toM(pArea->m_dWidthLower),3);
					pBM->m_dV[WidthUpper]		= Round(toM(pArea->m_dWidthUpper),3);
					pBM->m_dV[WidthDiffLeft]	= Round(toM(pArea->m_dWidthDiffLeft),3);
					pBM->m_dV[WidthDiffRight]	= Round(toM(pArea->m_dWidthDiffRight),3);
					pBM->m_dV[Height]			= Round(toM(pArea->m_dHeight),3);
					pBM->m_dV[Length]			= Round(toM(dLenFooting),3);
					pBM->CalculateResult();
					
					pArrBMStd->Add(pBM);
				}
			}
		}
		// MASS 콘크리트
		else if(nTypeExFooting == EXFOOTING_TYPE_MASS)
		{
			double dWidthFooting	= pExFooting->GetWidth();
			double dLenFooting = pExFooting->GetLength();

			nSize = pExFooting->m_bFrontDanMass ?
				pExFooting->m_pArrAreaConc.GetSize() : pExFooting->m_pArrAreaConcSide.GetSize();

			long j=0; for(j=0; j<nSize; j++)
			{
				CTwinVectorAreaShape *pArea	= pExFooting->m_bFrontDanMass ?
					pExFooting->m_pArrAreaConc.GetAt(j) : pExFooting->m_pArrAreaConcSide.GetAt(j);

				CBMData *pBM				= new CBMData;
				pBM->m_nNumbering			= pArea->m_nNum;
				pBM->m_nLevelTotal			= 2;
				pBM->m_nIdxBigClass			= BM_BIG_CONCRETE;
				pBM->m_sBigClass			= GetStringBigClass(pBM->m_nIdxBigClass);
				pBM->m_nIdxMidClass			= BM_MID_CONCRETE_MASS;
				pBM->m_sMidClass			= GetStringMidClass(pBM->m_nIdxMidClass);
				pBM->m_nIdxSmallClass		= BM_SMALL_CONCRETE_MASS;
				pBM->m_sSmallClass			= GetStringSmallClass(pBM->m_nIdxSmallClass);
				pBM->m_nTypeResult			= RESULT_VOLUMN;
				pBM->m_sUnit				= "㎥";
				pBM->m_nShape				= pArea->m_nTypeShape;
				pBM->m_dV[WidthLower]		= Round(toM(pArea->m_dWidthLower),3);
				pBM->m_dV[WidthUpper]		= Round(toM(pArea->m_dWidthUpper),3);
				pBM->m_dV[WidthDiffLeft]	= Round(toM(pArea->m_dWidthDiffLeft),3);
				pBM->m_dV[WidthDiffRight]	= Round(toM(pArea->m_dWidthDiffRight),3);
				pBM->m_dV[Width]			= Round(toM(pArea->m_dWidth),3);
				pBM->m_dV[HeightLeft]		= Round(toM(pArea->m_dHeightLeft),3);
				pBM->m_dV[HeightRight]		= Round(toM(pArea->m_dHeightRight),3);
				pBM->m_dV[Height]			= Round(toM(pArea->m_dHeight),3);
				pBM->m_dV[Length]			= pExFooting->m_bFrontDanMass ?	Round(toM(dLenFooting),3) : Round(toM(dWidthFooting),3);
				pBM->CalculateResult();
				
				pArrBMStd->Add(pBM);
			}
		}
	}
}

// 방호벽 수량
void COutBM::MakeBMConcreteGuardWall()
{
	CRcBridgeRebar *pBri = GetRcBridgeRebar();
	CRcBridgeRebar *pBriOrg = GetRcBridgeRebar(TRUE);
	if(!pBri) return;
	if(!pBriOrg) return;

	CLineInfo *pLine = pBri->GetLineBase();
	if(!pLine) return;

	// 영역 구분.
	CalAreaConc(pBri, FALSE);

	CGuardWallRC* pGW = NULL;
	long nQtyHDan = pBri->GetQtyHDan();
	long i=0; for(i=0; i<nQtyHDan; i++)
	{
		CTypedPtrArray <CObArray, CBMData*> *pArrBMStd	= &m_pArrBMGuardWallStd[i];
		
		pGW = pBriOrg->GetGuardWallByHDan(i);
		if(!pGW) continue;
		if(pGW->IsTypeHDanRoadTotalGuardWall()) //if(pGW->IsTypeHDanNonRebarConcrete())
		{
			double dLength = pBriOrg->GetGuardWallTotalLength(pGW);
			if(Compare(dLength, 0.0, "<=")) continue;

			long nLastNum	= 0;
			long nSize = pGW->m_pArrAreaConc.GetSize();
			long j=0; for(j=0; j<nSize; j++)
			{
				CTwinVectorAreaShape *pArea	= pGW->m_pArrAreaConc.GetAt(j);

				CBMData *pBM				= new CBMData;
				pBM->m_nNumbering			= pArea->m_nNum;
				pBM->m_nLevelTotal			= 2;
				pBM->m_nIdxBigClass			= BM_BIG_CONCRETE;
				pBM->m_sBigClass				= GetStringBigClass(pBM->m_nIdxBigClass);
				pBM->m_nIdxMidClass			= BM_MID_CONCRETE_GUIDEWALL; 
				pBM->m_sMidClass				= GetStringMidClass(pBM->m_nIdxMidClass, NULL, ePartUpperSlab, pGW);
				pBM->m_nIdxSmallClass		= BM_SMALL_FJ_GUARDFENCE;
				pBM->m_sSmallClass			= GetStringSmallClass(pBM->m_nIdxSmallClass);
				pBM->m_nIdxItemClass		= BM_ITEM_CONCRETE_GUARDWALL;
				pBM->m_sItemClass			= GetStringItemClass(pBM->m_nIdxItemClass);
				// 보도인경우 종류에 맞게 뽑아야 함
				// 콘크리트, 몰탈인경우 콘크리트로 뽑고, 모래인 경우 모래로 뽑자
				if(pGW->IsTypeHDanRoadFootway() && pGW->m_nTypeInnerFill == 2)
				{
					pBM->m_nIdxBigClass	= BM_BIG_SAND;
					pBM->m_sBigClass	= GetStringBigClass(pBM->m_nIdxBigClass);
					pBM->m_nIdxMidClass			= -1;
					pBM->m_sMidClass			= "";
					pBM->m_nIdxSmallClass		= -1;
					pBM->m_sSmallClass			= "";
					pBM->m_nIdxItemClass		= -1;
					pBM->m_sItemClass			= "";
				}

				pBM->m_nTypeResult			= RESULT_VOLUMN;
				pBM->m_sUnit				= "㎥";
				pBM->m_nShape				= pArea->m_nTypeShape;
				pBM->m_dV[WidthLower]		= Round(toM(pArea->m_dWidthLower),3);
				pBM->m_dV[WidthUpper]		= Round(toM(pArea->m_dWidthUpper),3);
				pBM->m_dV[WidthDiffLeft]	= Round(toM(pArea->m_dWidthDiffLeft),3);
				pBM->m_dV[WidthDiffRight]	= Round(toM(pArea->m_dWidthDiffRight),3);
				pBM->m_dV[Width]			= Round(toM(pArea->m_dWidth),3);
				pBM->m_dV[HeightLeft]		= Round(toM(pArea->m_dHeightLeft),3);
				pBM->m_dV[HeightRight]		= Round(toM(pArea->m_dHeightRight),3);
				pBM->m_dV[Height]			= Round(toM(pArea->m_dHeight),3);
				pBM->m_dV[Length]			= Round(toM(dLength),3);
				pBM->m_sInfo				= "방호벽콘크리트";
				pBM->m_bIsUseForDeduct		= pArea->m_bMinusArea;
				pBM->CalculateResult();
				
				pArrBMStd->Add(pBM);

				nLastNum	= max(nLastNum, pArea->m_nNum);
			}


			// 보도인경우 포장 수량도 뽑아야 된다.
			// 아스콘, 투수콘은 두께와 함께 면적으로 산출하고,
			// 콘크리트인 경우 기존 콘크리트와 동일하게 산출
			if(pGW->IsTypeHDanRoadFootway())
			{
				CBMData *pBM				= new CBMData;
				pBM->m_nNumbering			= nLastNum+1;
				pBM->m_nLevelTotal			= 2;
				if(pGW->m_nTypePave == 1)	// 콘크리트 포장
				{
					pBM->m_nIdxBigClass			= BM_BIG_CONCRETE;
					pBM->m_sBigClass			= GetStringBigClass(pBM->m_nIdxBigClass);
					pBM->m_nIdxMidClass			= BM_MID_CONCRETE_GUIDEWALL;
					pBM->m_sMidClass			= GetStringMidClass(pBM->m_nIdxMidClass, NULL, ePartUpperSlab, pGW);
					pBM->m_nIdxSmallClass		= BM_SMALL_FJ_GUARDFENCE;
					pBM->m_sSmallClass			= GetStringSmallClass(pBM->m_nIdxSmallClass);
					pBM->m_nIdxItemClass		= BM_ITEM_CONCRETE_GUARDWALL;
					pBM->m_sItemClass			= GetStringItemClass(pBM->m_nIdxItemClass);
					pBM->m_nTypeResult			= RESULT_VOLUMN;
					pBM->m_sUnit				= "㎥";
				}
				else	// 투수콘/아스콘 포장(면적으로구함)
				{
					pBM->m_nIdxBigClass	= pGW->m_nTypePave == 0 ? BM_BIG_PERMEABLE : BM_BIG_ASCON;
					pBM->m_sBigClass	= GetStringBigClass(pBM->m_nIdxBigClass);
					pBM->m_nIdxMidClass			= BM_MID_ASCON;
					pBM->m_sMidClass.Format("T=%.0fmm", pGW->m_dThickPaveFootway);
					pBM->m_nIdxSmallClass		= -1;
					pBM->m_sSmallClass			= "";
					pBM->m_nIdxItemClass		= -1;
					pBM->m_sItemClass			= "";
					pBM->m_nTypeResult			= RESULT_AREA;
					pBM->m_sUnit				= "㎡";
					pBM->m_dwSide				= SIDE_UPPER;
				}


				CTwinVectorArray tvArrPave;
				pGW->GetTvFootwayBase(tvArrPave);
				double dW	= pGW->m_dWidhtHDan;
				double dH	= pGW->m_dThickPaveFootway;
				if(tvArrPave.GetSize() > 7)
					dW	= tvArrPave.GetAt(7).GetLength();			


				pBM->m_nShape				= SHAPE_RECTANGLE;
				pBM->m_dV[WidthLower]		= Round(toM(dW),3);
				pBM->m_dV[WidthUpper]		= Round(toM(dW),3);
				pBM->m_dV[WidthDiffLeft]	= Round(toM(dW),3);
				pBM->m_dV[WidthDiffRight]	= Round(toM(dW),3);
				pBM->m_dV[Width]			= Round(toM(dW),3);
				pBM->m_dV[HeightLeft]		= Round(toM(dH),3);
				pBM->m_dV[HeightRight]		= Round(toM(dH),3);
				pBM->m_dV[Height]			= Round(toM(dH),3);
				pBM->m_dV[Length]			= Round(toM(dLength),3);
				pBM->m_sInfo				= "방호벽콘크리트";
				pBM->CalculateResult();
				
				pArrBMStd->Add(pBM);		
			}
		}
	}
}

// 현재 작업중인 bri 리턴
// BOOL bRealBridge : 평균높이가 아닌 가공 되지 않은 교량리턴
CRcBridgeRebar* COutBM::GetRcBridgeRebar(BOOL bOrg)
{
	if(m_nBri < 0 || m_nBri > m_pStd->m_pARcBridgeDataStd->GetBridgeSize()-1) return NULL;
	CRcBridgeRebar *pBri = m_pStd->m_pARcBridgeDataStd->GetBridgeRebar(m_nBri);

	return bOrg ? pBri : m_pStd->GetBridge(m_nBri);
}

// 기초를 포함해서 모든 콘크리트 영역 구함.
// 여기서 한번에 구해야 구체간의 인덱스를 맞출 수 있음
// 기초, 벽체, 본체 순서로 구함.
// BOOL bForDeductOfReFill : 되메우기에서 지반선 이하의 구체 공제용인 경우
// 참고 : 대부분 Separater 함수에 의해 자동으로 나눠 지지만, 본체는 완전 수작업으로 영역을 나눈다.
// 이유 : 정차잠님 권고사항이기도 하고, 실제로 자동으로 나누듯이 작업하지 않음
//       단 뒷채움용일 경우에는 어디서 짤릴지 계산하기 복잡하므로 자동나누기 함수를 사용함.
void COutBM::CalAreaConc(CRcBridgeRebar *pBri, BOOL bForDeductOfReFill)
{
	if(!pBri) return;
	CRebarPlacing rb;
	CHgBaseBMStd baseBM;

	CTwinVectorArray tvArrVertSectionOld;
	tvArrVertSectionOld = pBri->m_tvArrVertSection;
	long nTypeJongdan	= pBri->m_nTypeJongdanForRebar;
	BOOL bBoxType = pBri->IsBoxType();
	
	// 되메우기 공제용일경우에만 실제교량형태로 구한다.
	// 나머지는 이미 level인 교량이 인자로 들어오므로 
	// 여기서 다시 GetTvVertSection_Std를 호출하면 좌표가 이상해짐
	if(bForDeductOfReFill)
	{
		pBri->m_nTypeJongdanForRebar	= 2;
		pBri->GetTvVertSection_Std(0, FALSE, FALSE);
	}

	CTwinVectorArray tvArrEL;
	CTwinVectorArray tvArrResult;
	CDoubleArray dArrAngleSu;
	long nIdx	= 0;
	long nNum	= 1;
	long nNumPlane	= 1;

	CDPoint vAng(0,1);
	double sta = pBri->GetStationOnJijum(0,0);
	if(pBri->m_nTypeBridge!=BRIDGE_TYPE_PORTAL)
		sta += pBri->GetJijum(0)->m_dLengthUgan[1];
	if(pBri->m_bIsExp)
	{
		sta = pBri->GetStationBridgeMid();
	}

	// 기초
	long nJ = 0; for(nJ = 0; nJ <= pBri->m_nQtyJigan; nJ++)
	{
		if(bBoxType) break;
		CFootingApp *pFooting	= pBri->GetFootingByJijumNumber(nJ);
		if(pFooting)
		{
			if(!pFooting->m_bIs) continue;

			// 평면상으로는 2개씩 나눠 질 때도 있으므로.. 번호는 평면의 번호를 따른다.
			if(nNum < nNumPlane)
				nNum = nNumPlane;	
			else
				nNumPlane = nNum;	
			
			AhTPADelete(&pFooting->m_pArrAreaConc,	(CTwinVectorAreaShape*)0);
			nIdx	= 0;

			// 영역을 나눠서 tvAreaShape에 보관
			CTwinVectorArray tvArr;
			tvArr	= pFooting->m_tvArrFront;

			if(bForDeductOfReFill)
			{
				// 되메우기 공제용인 경우 터파기 상면을 따라서 잘라버림 //////////////
				CTwinVectorArray tvArrTypagi;
				pBri->GetTvTogongLine(tvArrTypagi, nJ, FALSE, TRUE, TRUE);
				CTwinVector tvUpper,tvLower,tvLeft,tvRight;
				if(tvArrTypagi.GetTvByInfo("상면", tvUpper) && tvArrTypagi.GetTvByInfo("하면", tvLower))
				{				
					tvLeft.m_v1 = tvLower.GetXyLeft();
					tvLeft.m_v2 = tvUpper.GetXyLeft();
					tvRight.m_v1 = tvUpper.GetXyRight();
					tvRight.m_v2 = tvLower.GetXyRight();
						
					CTwinVectorArray tvArrCutBox;
					tvArrCutBox.Add(tvUpper);
					tvArrCutBox.Add(tvLower);
					tvArrCutBox.Add(tvLeft);
					tvArrCutBox.Add(tvRight);

					CTwinVectorArray tvArrResult;
					rb.SubTractTvArrAndTvArr(tvArr, tvArrCutBox, tvArrResult);
					tvArr	= tvArrResult;
				}
				//////////////////////////////////////////////////////////////////////
			}

			rb.SeperatorTwinVectorArea(tvArr, tvArrResult, dArrAngleSu, 1);
			long j = 0; for(j = 0; j < dArrAngleSu.GetSize(); j++)
			{
				CTwinVectorAreaShape *pArea	= new CTwinVectorAreaShape(nNum++);
				for(long k = 0; k < (long)dArrAngleSu.GetAt(j); k++)
					pArea->AddTwinVector(tvArrResult.GetAt(nIdx++));

				pArea->Calculate();
				pFooting->m_pArrAreaConc.Add(pArea);
			}

			// 기초는 평면에 대해서 나눠야됨.(모양이 특이하면 평면을 기준으로 산출하는게 편하기 때문)
			// 평면으로 나눌때는 기초를 수평으로 놓고 나눔.
			AhTPADelete(&pFooting->m_pArrAreaConcPlane,	(CTwinVectorAreaShape*)0);
			nIdx	= 0;

			// 영역을 나눠서 tvAreaShape에 보관
			tvArr	= pFooting->m_tvArrplan;
			CTwinVector tvBack	= tvArr.GetTvByInfo("기초평면후면");
			CDPoint xyRot		= ToDPointFrDegree(tvBack.GetXyDir().GetAngleDegree() * -1);
			rb.RotateTvArr(tvArr, CDPoint(0, 0), xyRot.GetAngleDegree());

			tvArr.Sort(0.1);
			if(pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP || pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP2)
			{
				//평면에서는 빼준다
				for (long ix =0; ix < tvArr.GetSize(); ix ++)
				{
					CTwinVector tvCl = tvArr.GetAt(ix);
					if(tvCl.m_sInfo.Find(_T("경사")) > -1)
					{
						tvArr.RemoveAt(ix);
						ix--;
					}
				}						
			}
			
			rb.SeperatorTwinVectorArea(tvArr, tvArrResult, dArrAngleSu, 1);
			for(j = 0; j < dArrAngleSu.GetSize(); j++)
			{
				CTwinVectorAreaShape *pArea	= new CTwinVectorAreaShape(nNumPlane++);
				for(long k = 0; k < (long)dArrAngleSu.GetAt(j); k++)
					pArea->AddTwinVector(tvArrResult.GetAt(nIdx++));

				pArea->Calculate();
				pFooting->m_pArrAreaConcPlane.Add(pArea);
			}
		}
	}

	// 벽체
	long nCountInWall	= pBri->GetCountInWall();
	long i = 0; for(i = 0; i < nCountInWall; i++)
	{
		CWallApp *pWall	= pBri->GetInWall(i);
		if(pWall)
		{
			AhTPADelete(&pWall->m_pArrAreaConc,	(CTwinVectorAreaShape*)0);
			nIdx	= 0;

			// 영역을 나눠서 tvAreaShape에 보관
			CTwinVectorArray tvArr;
			tvArr	= pWall->m_tvArrFront;

			if(bForDeductOfReFill)
			{
				// 되메우기 공제용인 경우 터파기 상면을 따라서 잘라버림 //////////////
				CTwinVectorArray tvArrTypagi;
				long nJ = i+1;
				if(pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP2)
				{
					if(i==0)
						nJ = i;
					else
						nJ = pBri->m_nQtyJigan;
				}
				pBri->GetTvTogongLine(tvArrTypagi, nJ, FALSE, TRUE, TRUE);
				CTwinVector tvUpper,tvLower,tvLeft,tvRight;
				if(tvArrTypagi.GetTvByInfo("상면", tvUpper) && tvArrTypagi.GetTvByInfo("하면", tvLower))
				{				
					tvLeft.m_v1 = tvLower.GetXyLeft();
					tvLeft.m_v2 = tvUpper.GetXyLeft();
					tvRight.m_v1 = tvUpper.GetXyRight();
					tvRight.m_v2 = tvLower.GetXyRight();
						
					CTwinVectorArray tvArrCutBox;
					tvArrCutBox.Add(tvUpper);
					tvArrCutBox.Add(tvLower);
					tvArrCutBox.Add(tvLeft);
					tvArrCutBox.Add(tvRight);

					CTwinVectorArray tvArrResult;
					rb.SubTractTvArrAndTvArr(tvArr, tvArrCutBox, tvArrResult);
					tvArr	= tvArrResult;
				}
				//////////////////////////////////////////////////////////////////////
			}

			rb.SeperatorTwinVectorArea(tvArr, tvArrResult, dArrAngleSu, 1);
			long j = 0; for(j = 0; j < dArrAngleSu.GetSize(); j++)
			{
				CTwinVectorAreaShape *pArea	= new CTwinVectorAreaShape(nNum++);
				for(long k = 0; k < (long)dArrAngleSu.GetAt(j); k++)
					pArea->AddTwinVector(tvArrResult.GetAt(nIdx++));

				pArea->Calculate();
				pWall->m_pArrAreaConc.Add(pArea);
			}
		}
	}

	// 본체
	AhTPADelete(&pBri->m_pArrAreaConc,	(CTwinVectorAreaShape*)0);
	
	if(bForDeductOfReFill)
	{
		//Box형일때는 본체 하나 통째로 빼준다..
		long nEnd = bBoxType ? 1 : 2;

		long stt = 0; for(stt = 0; stt < nEnd; stt++)
		{
			// 영역을 나눠서 tvAreaShape에 보관
			CTwinVectorArray tvArr;
			// 아치교에서 리브 빼자. 나중에 리브 나누자.
			if(pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
			{
				CTwinVector tv;
				long nSize = pBri->m_tvArrVertSection.GetSize();
				long n=0; for(n=0; n<nSize; n++)
				{
					tv = pBri->m_tvArrVertSection.GetAt(n);
					if(tv.m_sInfo != _T(""))	// 리브
						tvArr.Add(tv);
				}
			}
			else if(bBoxType) //박스교일경우 외곽 라인만 보자..
				pBri->GetTvArrVertSectionOutLine(tvArr);
			else
				tvArr	= pBri->m_tvArrVertSection;

			// 되메우기 공제용인 경우 터파기 상면을 따라서 잘라버림 //////////////
			// 본체는 자르기가 좀 까다로움. 시점과 종점을 두번 잘라서 남는거 합쳐야됨
			long nJ = stt ? pBri->m_nQtyJigan : 0; 
			CTwinVectorArray tvArrTypagi;
			pBri->GetTvTogongLine(tvArrTypagi, nJ, FALSE, TRUE, TRUE);
			CTwinVector tvUpper,tvLower,tvLeft,tvRight;
			if(tvArrTypagi.GetTvByInfo("상면", tvUpper) && tvArrTypagi.GetTvByInfo("하면", tvLower))
			{				
				tvLeft.m_v1 = tvLower.GetXyLeft();
				tvLeft.m_v2 = tvUpper.GetXyLeft();
				tvRight.m_v1 = tvUpper.GetXyRight();
				tvRight.m_v2 = tvLower.GetXyRight();
					
				CTwinVectorArray tvArrCutBox;
				tvArrCutBox.Add(tvUpper);
				tvArrCutBox.Add(tvLower);
				tvArrCutBox.Add(tvLeft);
				tvArrCutBox.Add(tvRight);
				tvArrCutBox.Sort();

				CTwinVectorArray tvArrResult;
				rb.SubTractTvArrAndTvArr(tvArr, tvArrCutBox, tvArrResult);
				tvArr	= tvArrResult;
			}
			//////////////////////////////////////////////////////////////////////

			//## 이슈#0011794
			//	SubTractTvArrAndTvArr에서 아크에 대한 좌표를 체크 못함.
			CTwinVectorArrayArray tvArrArrConnect;
			m_pStd->GroupConnectedTvArr(tvArrArrConnect, tvArr);
			long i = 0;
			for(i = 0; i< tvArrArrConnect.GetSize(); i++)
			{
				CTwinVectorArray *pTvArr	= tvArrArrConnect.GetAt(i);
				if(pTvArr->GetSize() <= 0)
					continue;

				nIdx  = 0;
				tvArrResult.RemoveAll();
				rb.SeperatorTwinVectorArea(*pTvArr, tvArrResult, dArrAngleSu, 1);
				long j = 0; for(j = 0; j < dArrAngleSu.GetSize(); j++)
				{
					CTwinVectorAreaShape *pArea	= new CTwinVectorAreaShape(nNum++);
					for(long k = 0; k < (long)dArrAngleSu.GetAt(j); k++)
						pArea->AddTwinVector(tvArrResult.GetAt(nIdx++));
					
					pArea->Calculate();
					pBri->m_pArrAreaConc.Add(pArea);
				}
			}
			AhTPADelete(&tvArrArrConnect, (CTwinVectorArray*)0);
		}
	}
	else
	{
		// 영역을 나눠서 tvAreaShape에 보관
		CTwinVectorArray tvArr;
		tvArr = pBri->m_tvArrVertSection;
		// 아치교에서 리브 빼자. 나중에 리브 나누자.
		if(pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
		{
			CTwinVector tv;
			tvArr.RemoveAll();
			long nSize = pBri->m_tvArrVertSection.GetSize();
			long n=0; for(n=0; n<nSize; n++)
			{
				tv = pBri->m_tvArrVertSection.GetAt(n);
				if(tv.m_sInfo != _T(""))	// 리브
					tvArr.Add(tv);
			}
		}

		nIdx = 0;
		CTwinVector tvLeft, tvRight, tvUpper, tvLower;

		long stt = 0; for(stt = 0; stt < 2; stt++)
		{		
			BOOL bStt = stt == iSTT;
			CString sStt = bStt ? "시점" : "종점";

			// 시종점 벽체 있다면 시종점측벽체와 브라켓, 헌치를 구함.
			if(pBri->IsOutWall(bStt))
			{
				// 벽체
				tvLeft	= tvArr.GetTvByInfo(sStt+"벽체좌측면").Sort(FALSE);
				tvRight	= tvArr.GetTvByInfo(sStt+"벽체우측면").Sort(FALSE);
				tvUpper	= tvArr.GetTvByInfo("슬래브하면").Sort(TRUE);
				GetXyMatchLineAndLine(tvLeft.m_v1, tvLeft.GetXyDir(), tvUpper.m_v1, tvUpper.GetXyDir(), tvLeft.m_v2);
				tvUpper.m_v1	= tvLeft.m_v2;
				GetXyMatchLineAndLine(tvRight.m_v1, tvRight.GetXyDir(), tvUpper.m_v1, tvUpper.GetXyDir(), tvRight.m_v2);
				tvUpper.m_v2	= tvRight.m_v2;
				tvLower.m_v1	= tvLeft.m_v1;
				tvLower.m_v2	= tvRight.m_v1;
				tvLower.m_sInfo	= "";
				if(bBoxType)
				{
					tvLower	= tvArr.GetTvByInfo("하부슬래브상면").Sort(TRUE);
					GetXyMatchLineAndLine(tvLeft.m_v1, tvLeft.GetXyDir(), tvLower.m_v1, tvLower.GetXyDir(), tvLeft.m_v1);
					tvUpper.m_v1	= tvLeft.m_v2;
					GetXyMatchLineAndLine(tvRight.m_v1, tvRight.GetXyDir(), tvLower.m_v1, tvLower.GetXyDir(), tvRight.m_v1);
					tvUpper.m_v2	= tvRight.m_v2;
					tvLower.m_v1	= tvLeft.m_v1;
					tvLower.m_v2	= tvRight.m_v1;
				}

				CTwinVector tv;
				BOOL bHunch	= tvArr.GetTvByInfo(sStt+"벽체하부헌치", tv);
				if(bHunch)
				{
					if(bBoxType)
					{
						double dHunchWidth = bStt? pBri->m_dWSHW : pBri->m_dWEHW;
						if(dHunchWidth > 0)
						{
							CTwinVector tvHLower, tvHLeft;
							CTwinVectorAreaShape *pArea			= new CTwinVectorAreaShape(nNum++);
							tvHLeft.m_v1		= bStt ? tv.m_v1 : tv.m_v2;
							tvHLeft.m_v2.x	= bStt ? tv.m_v1.x : tv.m_v2.x;
							tvHLeft.m_v2.y	= bStt ? tv.m_v2.y : tv.m_v1.y;
							tvHLeft.m_sInfo	= "";
							tvHLower.m_v1	= tvHLeft.GetXyBottom();
							tvHLower.m_v2	= tv.m_v2;
							tvHLower.m_sInfo	= "";
							pArea->AddTwinVector(tvHLeft);
							pArea->AddTwinVector(tv);
							pArea->AddTwinVector(tvHLower);
							pArea->Calculate();
							pArea->m_sInfo	= sStt+"벽체하부헌치";
							pBri->m_pArrAreaConc.Add(pArea);
						}						
					}
					else
					{
						CTwinVector tvL, tvU, tvB;
						tvB.m_v1 = bStt ? tvLeft.GetXyBottom() : tvRight.GetXyBottom();
						tvB.m_v2 = tv.GetXyBottom();
						tvL.m_v1 = tvB.m_v1;
						tvL.m_v2 = CDPoint(tvL.m_v1.x, tv.GetXyTop().y);
						tvU.m_v1 = tvL.m_v2;
						tvU.m_v2 = tv.GetXyTop();
						//
						if(bStt)
						{
							tvLeft.m_v1  = tvL.m_v2;
							tvLower.m_v1 = tvLeft.m_v1;
						}
						else
						{
							tvRight.m_v1 = tvL.m_v2;
							tvLower.m_v2 = tvRight.m_v1;
						}

						CTwinVectorAreaShape *pArea	= new CTwinVectorAreaShape(nNum++);
						pArea->AddTwinVector(tvB);
						pArea->AddTwinVector(tvL);
						pArea->AddTwinVector(tv);
						pArea->AddTwinVector(tvU);
						pArea->Calculate();
						pArea->m_sInfo	= sStt+"벽체";
						pBri->m_pArrAreaConc.Add(pArea);
					}
				}

				CTwinVectorAreaShape *pArea	= new CTwinVectorAreaShape(nNum++);
				pArea->AddTwinVector(tvLower);
				pArea->AddTwinVector(tvLeft);
				pArea->AddTwinVector(tvRight);
				pArea->AddTwinVector(tvUpper);
				pArea->Calculate();
				pArea->m_sInfo	= sStt+"벽체";
				pBri->m_pArrAreaConc.Add(pArea);

				// 브라켓
				pArea	= new CTwinVectorAreaShape(nNum++);
				tvLeft	= tvArr.GetTvByInfo(sStt+"브라켓측면").Sort(FALSE);
				tvUpper	= tvArr.GetTvByInfo(sStt+"브라켓상면").Sort(TRUE);
				tvLower	= tvArr.GetTvByInfo(sStt+"브라켓헌치").Sort(TRUE);
				tvUpper.m_v2.x = tvLower.m_v2.x; // 브래킷 상하면의 벽체측 좌표오차가 발생하는 경우가 있음.
				tvRight.m_v1 = bStt ? tvLower.m_v2 : tvLower.m_v1;
				tvRight.m_v2 = bStt ? tvUpper.m_v2 : tvUpper.m_v1;
				tvRight.m_sInfo	= "";
				pArea->AddTwinVector(tvLower);
				pArea->AddTwinVector(tvLeft);
				pArea->AddTwinVector(tvRight);
				pArea->AddTwinVector(tvUpper);
				pArea->Calculate();
				pArea->m_sInfo	= sStt+"브라켓";
				pBri->m_pArrAreaConc.Add(pArea);

				// 헌치
				CTwinVector tvHunch	= tvArr.GetTvByInfo(sStt+"벽체헌치").Sort(TRUE);
				if(tvHunch.GetHorLength() > 0 && tvHunch.GetVerLength() > 0)
				{
					pArea			= new CTwinVectorAreaShape(nNum++);
					tvLeft.m_v1		= bStt ? tvHunch.m_v1 : tvHunch.m_v2;
					tvLeft.m_v2.x	= bStt ? tvHunch.m_v1.x : tvHunch.m_v2.x;
					tvLeft.m_v2.y	= bStt ? tvHunch.m_v2.y : tvHunch.m_v1.y;
					tvLeft.m_sInfo	= "";
					tvUpper.m_v1	= tvLeft.GetXyTop();
					tvUpper.m_v2	= tvHunch.m_v2;
					tvUpper.m_sInfo	= "";
					pArea->AddTwinVector(tvLeft);
					pArea->AddTwinVector(tvHunch);
					pArea->AddTwinVector(tvUpper);
					pArea->Calculate();
					pArea->m_sInfo	= sStt+"벽체헌치";
					pBri->m_pArrAreaConc.Add(pArea);
				}
			}
		}

		// 중간지점 헌치들
		CString str	= _T("");
		long i = 0; for(i = 0; i < pBri->m_nCountInWallHunch; i++)
		{
			CTwinVectorAreaShape *pArea	= NULL;
			str.Format("%d번째내측헌치", i+1);

			CTwinVectorArray tvArrHunch, tvArrResult;
			CDoubleArray dArrAngleSu;
			
			// 위쪽 사다리꼴
			tvLeft	= tvArr.GetTvByInfo(str+"좌측면").Sort(TRUE);
 			tvRight	= tvArr.GetTvByInfo(str+"우측면").Sort(TRUE);
			tvUpper.m_v1	= tvLeft.GetXyTop();
			tvUpper.m_v2	= tvRight.GetXyTop();
			tvUpper.m_sInfo	= "";
			tvLower.m_v1	= tvLeft.GetXyBottom();
			tvLower.m_v2	= tvRight.GetXyBottom();
			tvLower.m_sInfo	= "";

			// (ARCBRIDGE-3398) 종단면의 nLeft가 0이 아니라면 헌지를 구하지 않는다. 헌치가 없는경우를 고려 해줘야 한다. 
			if(tvLower.GetLength() > 0)
				tvArrHunch.Add(tvLower);
			if(tvLeft.GetLength() > 0)
				tvArrHunch.Add(tvLeft);
			if(tvRight.GetLength() > 0)
				tvArrHunch.Add(tvRight);
			if(tvUpper.GetLength() > 0)
				tvArrHunch.Add(tvUpper);

			rb.SeperatorTwinVectorArea(tvArrHunch, tvArrResult, dArrAngleSu);
			long nIdx = 0;
			long j=0; for(j= 0; j<dArrAngleSu.GetSize(); j++)
			{
				pArea = new CTwinVectorAreaShape(nNum++); 
				for(long k= 0; k<(long)dArrAngleSu.GetAt(j); k++)
					pArea->AddTwinVector(tvArrResult.GetAt(nIdx++));
				
				pArea->Calculate();
				pArea->m_sInfo	= str;
				pBri->m_pArrAreaConc.Add(pArea);
			}

			// 아래쪽 직사각형
			if(tvArr.GetTvByInfo(str+"아래좌측면", tvLeft))
			{
				tvLeft.Sort(FALSE);
				tvRight	= tvArr.GetTvByInfo(str+"아래우측면").Sort(FALSE);
				tvUpper.m_v1	= tvLeft.m_v2;
				tvUpper.m_v2	= tvRight.m_v2;
				tvUpper.m_sInfo	= "";
				tvLower.m_v1	= tvLeft.m_v1;
				tvLower.m_v2	= tvRight.m_v1;
				tvLower.m_sInfo	= "";
				if(tvRight.GetVerLength() > 0)
				{
					pArea	= new CTwinVectorAreaShape(nNum++);
					pArea->AddTwinVector(tvLower);
					pArea->AddTwinVector(tvLeft);
					pArea->AddTwinVector(tvRight);
					pArea->AddTwinVector(tvUpper);
					pArea->Calculate();
					pArea->m_sInfo	= str+"하부";
					pBri->m_pArrAreaConc.Add(pArea);
				}
			}

			if(bBoxType)
			{
				str.Format("%d번째내측하부헌치", i+1);
				tvLeft	= tvArr.GetTvByInfo(str+"좌측면").Sort(TRUE);
				tvRight	= tvArr.GetTvByInfo(str+"우측면").Sort(TRUE);
				tvUpper.m_v1	= tvLeft.GetXyTop();
				tvUpper.m_v2	= tvRight.GetXyTop();
				tvUpper.m_sInfo	= "";
				tvLower.m_v1	= tvLeft.GetXyBottom();
				tvLower.m_v2	= tvRight.GetXyBottom();
				tvLower.m_sInfo	= "";
				tvArrHunch.RemoveAll();
				tvArrHunch.Add(tvLower);
				tvArrHunch.Add(tvLeft);
				tvArrHunch.Add(tvRight);
				tvArrHunch.Add(tvUpper);

				rb.SeperatorTwinVectorArea(tvArrHunch, tvArrResult, dArrAngleSu);
				long nIdx = 0;
				long j=0; for(j= 0; j<dArrAngleSu.GetSize(); j++)
				{
					pArea = new CTwinVectorAreaShape(nNum++); 
					for(long k= 0; k<(long)dArrAngleSu.GetAt(j); k++)
						pArea->AddTwinVector(tvArrResult.GetAt(nIdx++));

					pArea->Calculate();
					pArea->m_sInfo	= str;
					pBri->m_pArrAreaConc.Add(pArea);
				}

				// 위쪽 직사각형
				if(tvArr.GetTvByInfo(str+"위좌측면", tvLeft))
				{
					tvLeft.Sort(FALSE);
					tvRight	= tvArr.GetTvByInfo(str+"위우측면").Sort(FALSE);
					tvUpper.m_v1	= tvLeft.m_v2;
					tvUpper.m_v2	= tvRight.m_v2;
					tvUpper.m_sInfo	= "";
					tvLower.m_v1	= tvLeft.m_v1;
					tvLower.m_v2	= tvRight.m_v1;
					tvLower.m_sInfo	= "";
					if(tvRight.GetVerLength() > 0)
					{
						pArea	= new CTwinVectorAreaShape(nNum++);
						pArea->AddTwinVector(tvLower);
						pArea->AddTwinVector(tvLeft);
						pArea->AddTwinVector(tvRight);
						pArea->AddTwinVector(tvUpper);
						pArea->Calculate();
						pArea->m_sInfo	= str+"상부";
						pBri->m_pArrAreaConc.Add(pArea);
					}
				}
			}
		}

		// 상부 슬래브
		CString sSideStt	= pBri->IsOutWall(TRUE) ? "시점브라켓상부측면" : "시점슬래브측면";
		CString sSideEnd	= pBri->IsOutWall(FALSE) ? "종점브라켓상부측면" : "종점슬래브측면";
		CTwinVector tvStt	= tvArr.GetTvByInfo(sSideStt).Sort(FALSE);
		CTwinVector tvEnd	= tvArr.GetTvByInfo(sSideEnd).Sort(FALSE);
		tvStt.m_v1.y = tvStt.m_v2.y - pBri->m_dTS;
		tvEnd.m_v1.y = tvEnd.m_v2.y - pBri->m_dTS;

		tvUpper.m_v1	= tvStt.m_v2;
		tvUpper.m_v2	= tvEnd.m_v2;
		tvUpper.m_sInfo	= "";
		tvLower.m_v1	= tvStt.m_v1;
		tvLower.m_v2	= tvEnd.m_v1;
		tvLower.m_sInfo	= "";
		
		str  = _T("상부슬래브");
		if(pBri->m_nHeightForSlabBM==0)
		{
//			CTwinVectorAreaShape *pArea	= NULL;
			CTwinVectorArray tvArrSlab, tvArrResult;
			CDoubleArray dArrAngleSu;

			tvArrSlab.Add(tvLower);
			tvArrSlab.Add(tvStt);
			tvArrSlab.Add(tvEnd);
			tvArrSlab.Add(tvUpper);

			rb.SeperatorTwinVectorArea(tvArrSlab, tvArrResult, dArrAngleSu);
			long nIdx = 0;
			long j=0; for(j=0; j<dArrAngleSu.GetSize(); j++)
			{
				CTwinVectorAreaShape *pArea = new CTwinVectorAreaShape(nNum++); 
				for(long k=0; k<(long)dArrAngleSu.GetAt(j); k++)
					pArea->AddTwinVector(tvArrResult.GetAt(nIdx++));
				
				pArea->Calculate();
				pArea->m_sInfo	= str;
				pBri->m_pArrAreaConc.Add(pArea);
			}
		}
		else
		{
			CTwinVectorAreaShape *pArea	= new CTwinVectorAreaShape(nNum++);
			pArea->AddTwinVector(tvLower);
			pArea->AddTwinVector(tvStt);
			pArea->AddTwinVector(tvEnd);
			pArea->AddTwinVector(tvUpper);
			pArea->Calculate();
			pArea->m_sInfo	= str;
			pBri->m_pArrAreaConc.Add(pArea);
		}

		if(bBoxType)
		{
			// 하부 슬래브
			CTwinVector tvStt	= tvArr.GetTvByInfo(_T("하부슬래브하면(시점벽체아래)")).Sort(FALSE);
			CTwinVector tvEnd	= tvArr.GetTvByInfo(_T("하부슬래브하면(종점벽체아래)")).Sort(FALSE);
			tvLeft.m_v1 = tvStt.m_v1;
			tvRight.m_v1 = tvEnd.m_v2;
			tvLeft.m_v2 = tvLeft.m_v1 + CDPoint(0, pBri->m_dTS_Lower);
			tvRight.m_v2 = tvRight.m_v1 + CDPoint(0, pBri->m_dTS_Lower);
			tvLeft.m_sInfo = _T("하부슬래브하면(시점벽체아래)");
			tvRight.m_sInfo = _T("하부슬래브하면(종점벽체아래)");
			tvUpper.m_v1	= tvLeft.m_v2;
			tvUpper.m_v2	= tvRight.m_v2;
			tvUpper.m_sInfo	= "";
			tvLower.m_v1	= tvLeft.m_v1;
			tvLower.m_v2	= tvRight.m_v1;
			tvLower.m_sInfo	= "";

			str  = _T("하부슬래브");
			CTwinVectorAreaShape *pArea	= new CTwinVectorAreaShape(nNum++);
			pArea->AddTwinVector(tvLower);
			pArea->AddTwinVector(tvLeft);
			pArea->AddTwinVector(tvRight);
			pArea->AddTwinVector(tvUpper);
			pArea->Calculate();
			pArea->m_sInfo	= str;
			pBri->m_pArrAreaConc.Add(pArea);

			// 부상방지저판
			if(pBri->m_bExistProtectionRiseFooting)
			{
				for(long nstt=0; nstt<2; nstt++)
				{
					if(nstt == 0)
					{
						tvLeft.m_v1 = tvStt.m_v1 + CDPoint(-pBri->m_dPRF_W, 0);
						tvRight.m_v1 = tvStt.m_v1;
						tvLeft.m_v2 = tvLeft.m_v1 + CDPoint(0, pBri->m_dPRF_H1);
						tvRight.m_v2 = tvRight.m_v1 + CDPoint(0, pBri->m_dPRF_H1 + pBri->m_dPRF_H2);
					}
					else
					{
						tvLeft.m_v1 = tvEnd.m_v2;
						tvRight.m_v1 = tvEnd.m_v2 + CDPoint(pBri->m_dPRF_W, 0);
						tvLeft.m_v2 = tvLeft.m_v1 + CDPoint(0, pBri->m_dPRF_H1 + pBri->m_dPRF_H2);
						tvRight.m_v2 = tvRight.m_v1 + CDPoint(0, pBri->m_dPRF_H1);
					}

					tvUpper.m_v1	= tvLeft.m_v2;
					tvUpper.m_v2	= tvRight.m_v2;
					tvUpper.m_sInfo	= "";
					tvLower.m_v1	= tvLeft.m_v1;
					tvLower.m_v2	= tvRight.m_v1;
					tvLower.m_sInfo	= "";

					str  = _T("부상방지 저판");
					CTwinVectorAreaShape *pArea	= new CTwinVectorAreaShape(nNum++);
					pArea->AddTwinVector(tvLower);
					pArea->AddTwinVector(tvLeft);
					pArea->AddTwinVector(tvRight);
					pArea->AddTwinVector(tvUpper);
					pArea->Calculate();
					pArea->m_sInfo	= str;
					pBri->m_pArrAreaConc.Add(pArea);
				}
			}
		}
	}

	// 버림콘크리트(LEAN) 및 MASS 콘크리트
	long nNumSide	= nNum;
	long nCountFooting	= pBri->GetCountFooting();
	for(i=0; i<nCountFooting; i++)
	{
		CFootingApp *pFooting	= pBri->GetFooting(i);
		if(pFooting)
		{
			CExFootingApp *pExFooting	= &pFooting->m_exFooting;
			AhTPADelete(&pExFooting->m_pArrAreaConc, (CTwinVectorAreaShape*)0);
			nIdx	= 0;

			// 영역을 나눠서 tvAreaShape에 보관
			CTwinVectorArray tvArr;
			tvArr	= pExFooting->m_tvArrFront;
			if(bForDeductOfReFill)
			{
				// 되메우기 공제용인 경우 터파기 상면을 따라서 잘라버림 //////////////
				CTwinVectorArray tvArrTypagi;
				pBri->GetTvTogongLine(tvArrTypagi, i, FALSE, TRUE, TRUE);
				CTwinVector tvUpper,tvLower,tvLeft,tvRight;
				if(tvArrTypagi.GetTvByInfo("상면", tvUpper) && tvArrTypagi.GetTvByInfo("하면", tvLower))
				{				
					tvLeft.m_v1 = tvLower.GetXyLeft();
					tvLeft.m_v2 = tvUpper.GetXyLeft();
					tvRight.m_v1 = tvUpper.GetXyRight();
					tvRight.m_v2 = tvLower.GetXyRight();
						
					CTwinVectorArray tvArrCutBox;
					tvArrCutBox.Add(tvUpper);
					tvArrCutBox.Add(tvLower);
					tvArrCutBox.Add(tvLeft);
					tvArrCutBox.Add(tvRight);

					CTwinVectorArray tvArrResult;
					rb.SubTractTvArrAndTvArr(tvArr, tvArrCutBox, tvArrResult);
					tvArr	= tvArrResult;
				}
				//////////////////////////////////////////////////////////////////////
			}
			rb.SeperatorTwinVectorArea(tvArr, tvArrResult, dArrAngleSu, 1);
			long j=0; for(j=0; j<dArrAngleSu.GetSize(); j++)
			{
				CTwinVectorAreaShape *pArea	= new CTwinVectorAreaShape(nNum++);
				for(long k=0; k<(long)dArrAngleSu.GetAt(j); k++)
					pArea->AddTwinVector(tvArrResult.GetAt(nIdx++));

				pArea->Calculate();
				pExFooting->m_pArrAreaConc.Add(pArea);
			}

			// 측면
			AhTPADelete(&pExFooting->m_pArrAreaConcSide, (CTwinVectorAreaShape*)0);
			nIdx	= 0;
			tvArr	= pExFooting->m_tvArrSide;
			rb.SeperatorTwinVectorArea(tvArr, tvArrResult, dArrAngleSu, 1);
			for(j=0; j<dArrAngleSu.GetSize(); j++)
			{
				CTwinVectorAreaShape *pArea	= new CTwinVectorAreaShape(nNumSide++);
				for(long k=0; k<(long)dArrAngleSu.GetAt(j); k++)
					pArea->AddTwinVector(tvArrResult.GetAt(nIdx++));

				pArea->Calculate();
				pExFooting->m_pArrAreaConcSide.Add(pArea);
			}

			nNum		= max(nNum, nNumSide);
			nNumSide	= nNum;

			// 버림콘크리트는 평면에 대해서 나눠야됨.(모양이 특이하면 평면을 기준으로 산출하는게 편하기 때문)
			AhTPADelete(&pExFooting->m_pArrAreaConcPlane, (CTwinVectorAreaShape*)0);
			nIdx		= 0;
			nNumPlane	= nNum - 1;		// 인덱스 맞추기 위해...

			// 영역을 나눠서 tvAreaShape에 보관
			tvArr	= pExFooting->m_tvArrPlan;
			CDPoint xyRot	= pBri->GetXyDirRotateDomByBridgeSttEndXy().Rotate90();
			rb.RotateTvArr(tvArr, CDPoint(0, 0), xyRot.GetAngleDegree());
			// 하나의 영역으로 자르기 위해 수평으로 만든다.
			for(long k=0; k<tvArr.GetSize(); k++)
			{
				CString str = tvArr[k].m_sInfo;
				if(str.Find(_T("기초평면전면"))>=0)
				{
					CTwinVector tv = tvArr[k];
					CDPoint v = (tv.m_v2 - tv.m_v1).Unit();
					double dAng = v.GetAngleDegree();		// 삐뚤어진 각도 알아내서
					rb.RotateTvArr(tvArr, CDPoint(0, 0), -dAng);	// 반대로 돌려 수평으로 만든다.
					break;
				}
			}

			rb.SeperatorTwinVectorArea(tvArr, tvArrResult, dArrAngleSu, 1);
			for(j = 0; j < dArrAngleSu.GetSize(); j++)
			{
				CTwinVectorAreaShape *pArea	= new CTwinVectorAreaShape(nNumPlane++);
				for(long k = 0; k<(long)dArrAngleSu.GetAt(j); k++)
					pArea->AddTwinVector(tvArrResult.GetAt(nIdx++));

				pArea->Calculate();
				pExFooting->m_pArrAreaConcPlane.Add(pArea);
			}
		}
	}

	// 아치교의 경우 리브에 대해서도 해준다.
	if(pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
	{
		CTwinVectorAreaShape *pArea	= new CTwinVectorAreaShape(nNum);
		long nSize = pBri->m_tvArrVertSectionRib[1].GetSize();
		long nNo = nSize/2;
		CTwinVector tvLeft  = pBri->m_tvArrVertSectionRib[0].GetAt(nNo);
		CTwinVector tvRight = pBri->m_tvArrVertSectionRib[1].GetAt(nNo);
		CDRect rt(tvLeft.m_v1, tvRight.m_v2);
		pArea->m_xyCen = rt.CenterPoint();
		pArea->m_sInfo	= "리브";
		pBri->m_pArrAreaConc.Add(pArea);

		for(long nStt=0; nStt<2; nStt++)
		{
			// 리브 tvArr
			nIdx = 0;
			CTwinVector tv;
			CTwinVectorArray tvArr;
			long nSize0 = pBri->m_tvArrVertSectionRib[0].GetSize();
			long n=0; for(n=0; n<nSize0; n++)
				tvArr.Add(pBri->m_tvArrVertSectionRib[0].GetAt(n));
			long nSize1 = pBri->m_tvArrVertSectionRib[1].GetSize();
			for(n=nSize1-1; n>=0; n--)
			{
				if(n==nSize1-1)
				{
					tv.m_v1 = pBri->m_tvArrVertSectionRib[0].GetAt(nSize0-1).m_v2;
					tv.m_v2 = pBri->m_tvArrVertSectionRib[1].GetAt(n).m_v2;
					tvArr.Add(tv);
					tv.m_v1 = pBri->m_tvArrVertSectionRib[1].GetAt(n).m_v2;
					tv.m_v2 = pBri->m_tvArrVertSectionRib[1].GetAt(n).m_v1;
					tvArr.Add(tv);
				}
				else if(n==0)
				{
					tv.m_v1 = pBri->m_tvArrVertSectionRib[1].GetAt(n).m_v2;
					tv.m_v2 = pBri->m_tvArrVertSectionRib[1].GetAt(n).m_v1;
					tvArr.Add(tv);
					tv.m_v1 = pBri->m_tvArrVertSectionRib[1].GetAt(n).m_v1;
					tv.m_v2 = pBri->m_tvArrVertSectionRib[0].GetAt(0).m_v1;
					tvArr.Add(tv);
				}
				else
				{
					tv.m_v1 = pBri->m_tvArrVertSectionRib[1].GetAt(n).m_v2;
					tv.m_v2 = pBri->m_tvArrVertSectionRib[1].GetAt(n).m_v1;
					tvArr.Add(tv);
				}
			}

			if(bForDeductOfReFill)
			{
				// 되메우기 공제용인 경우 터파기 상면을 따라서 잘라버림 //////////////
				CTwinVectorArray tvArrReFill;
				CTwinVectorArray tvArrTypagi;
				nJ = nStt==0 ? 0 : pBri->m_nQtyJigan;
				pBri->GetTvTogongLine(tvArrTypagi, nJ, FALSE, TRUE, TRUE);
				CTwinVector tvUpper,tvLower,tvLeft,tvRight;
				if(tvArrTypagi.GetTvByInfo("상면", tvUpper) && tvArrTypagi.GetTvByInfo("하면", tvLower))
				{				
					tvLeft.m_v1 = tvLower.GetXyLeft();
					tvLeft.m_v2 = tvUpper.GetXyLeft();
					tvRight.m_v1 = tvUpper.GetXyRight();
					tvRight.m_v2 = tvLower.GetXyRight();
						
					CTwinVectorArray tvArrCutBox;
					tvArrCutBox.Add(tvUpper);
					tvArrCutBox.Add(tvLower);
					tvArrCutBox.Add(tvLeft);
					tvArrCutBox.Add(tvRight);

					for(n=0; n<tvArr.GetSize(); n++)
					{
						tv = tvArr.GetAt(n);
						tv.m_v1.z = 0;
						tv.m_v2.z = 0;
						tvArrReFill.Add(tv);
					}

					CTwinVectorArray tvArrResult;
					rb.SubTractTvArrAndTvArr(tvArrReFill, tvArrCutBox, tvArrResult);
					tvArrReFill = tvArrResult;
				}

				rb.SeperatorTwinVectorArea(tvArrReFill, tvArrResult, dArrAngleSu, 1);
				long j=0; for(j=0; j<dArrAngleSu.GetSize(); j++)
				{
					CTwinVectorAreaShape *pArea	= new CTwinVectorAreaShape(nNum++);
					for(long k=0; k<(long)dArrAngleSu.GetAt(j); k++)
						pArea->AddTwinVector(tvArrResult.GetAt(nIdx++));

					pArea->Calculate();
					pBri->m_pArrAreaConc.Add(pArea);
				}
			}
		}
	}

	// 방호벽
	CRcBridgeRebar *pBriOrg	= GetRcBridgeRebar(TRUE);	// 방호벽은 원래 교량으로 작업함.
	long nQtyHDan = pBriOrg->GetQtyHDan();
	for(i=0; i<nQtyHDan; i++)
	{
		CGuardWallRC* pGW = pBriOrg->GetGuardWallByHDan(i);
		if(!pGW) continue;
		if(pGW->IsTypeHDanRoadTotalGuardWall())
		{
			AhTPADelete(&pGW->m_pArrAreaConc,	(CTwinVectorAreaShape*)0);
			nNum = 1;
			nIdx = 0;

			// 영역을 나눠서 tvAreaShape에 보관
			CTwinVectorArray tvArr, tvArrDeduct;
			pBriOrg->GetTvGuardWall(pGW, tvArr, sta, vAng, TRUE, FALSE, FALSE, TRUE);
			// (ARCBRIDGE-3243) 내공이 있는것은 따로 분리한다.
			tvArr.GetTvArrByInfo(_T("Hole"), tvArrDeduct, TRUE, FALSE);
			if(bForDeductOfReFill)
			{
				// 되메우기 공제용인 경우 터파기 상면을 따라서 잘라버림 //////////////
				CTwinVectorArray tvArrTypagi;
				pBri->GetTvTogongLine(tvArrTypagi, i, FALSE, TRUE, TRUE);
				CTwinVector tvUpper,tvLower,tvLeft,tvRight;
				if(tvArrTypagi.GetTvByInfo("상면", tvUpper) && tvArrTypagi.GetTvByInfo("하면", tvLower))
				{				
					tvLeft.m_v1 = tvLower.GetXyLeft();
					tvLeft.m_v2 = tvUpper.GetXyLeft();
					tvRight.m_v1 = tvUpper.GetXyRight();
					tvRight.m_v2 = tvLower.GetXyRight();
						
					CTwinVectorArray tvArrCutBox;
					tvArrCutBox.Add(tvUpper);
					tvArrCutBox.Add(tvLower);
					tvArrCutBox.Add(tvLeft);
					tvArrCutBox.Add(tvRight);

					CTwinVectorArray tvArrResult;
					rb.SubTractTvArrAndTvArr(tvArr, tvArrCutBox, tvArrResult);
					tvArr	= tvArrResult;
				}
				//////////////////////////////////////////////////////////////////////
			}			
			rb.SeperatorTwinVectorArea(tvArr, tvArrResult, dArrAngleSu, 1);
			long j=0; for(j=0; j<dArrAngleSu.GetSize(); j++)
			{
				CTwinVectorAreaShape *pArea	= new CTwinVectorAreaShape(nNum++);
				for(long k=0; k<(long)dArrAngleSu.GetAt(j); k++)
					pArea->AddTwinVector(tvArrResult.GetAt(nIdx++));

				pArea->Calculate();
				pGW->m_pArrAreaConc.Add(pArea);
			}

			if(tvArrDeduct.GetSize() > 2 && !bForDeductOfReFill)
			{
				CTwinVectorAreaShape *pArea	= new CTwinVectorAreaShape(nNum++);
				for(long k=0; k<tvArrDeduct.GetSize(); ++k)
				{
					CTwinVector tv = tvArrDeduct.GetAt(k);
					if(tv.m_sInfo.Find(_T("HoleLine")) != -1)
						pArea->AddTwinVector(tvArrDeduct.GetAt(k));
				}
				pArea->Calculate();
				pGW->m_pArrAreaConc.Add(pArea);
				pArea->m_bMinusArea = TRUE;
			}
		}
	}

	if(bForDeductOfReFill)
	{
		pBri->m_tvArrVertSection = tvArrVertSectionOld;
		pBri->m_nTypeJongdanForRebar	= nTypeJongdan;
	}
}

// 공종의 이름이 옵션에 따라서 바뀔 경우를 대비해서 함수로 처리함.
CString COutBM::GetStringBigClass(long nIdxBigClass)
{
	CRcBridgeRebar	*pBri	= GetRcBridgeRebar();
	if(!pBri) return _T("");

	if(nIdxBigClass == BM_BIG_BOHOBLOCK)		return "보호블럭";
	if(nIdxBigClass == BM_BIG_CONCRETE)			return "콘크리트";
	if(nIdxBigClass == BM_BIG_FORM)				return "거푸집";
	if(nIdxBigClass == BM_BIG_POST)				return "동바리";
	if(nIdxBigClass == BM_BIG_DOWELBAR)		return "DOWEL BAR";
	if(nIdxBigClass == BM_BIG_SCAFFOLD)			return "비계";
	if(nIdxBigClass == BM_BIG_SPACER)				return "스페이셔";
	if(nIdxBigClass == BM_BIG_SLABCARE)			return "슬래브 양생";
	if(nIdxBigClass == BM_BIG_CONCCARE)			return "콘크리트 양생";
	if(nIdxBigClass == BM_BIG_ASPHALTPROOF)	return "ASP 방수";
	if(nIdxBigClass == BM_BIG_REBAR)				return "철근가공조립";
	if(nIdxBigClass == BM_BIG_SURFACE)			return "면고르기";
	if(nIdxBigClass == BM_BIG_ASCON)				return pBri->m_nRoadMaterial==0 ? "ASCON 포장" : "LMC 포장";
	if(nIdxBigClass == BM_BIG_SAND)				return "모래";
	if(nIdxBigClass == BM_BIG_PERMEABLE)		return "투스콘";
	if(nIdxBigClass == BM_BIG_TACKCOTTING)		return "택 코팅";
	if(nIdxBigClass == BM_BIG_SURFACEPROOF)	return "교면방수";
	if(nIdxBigClass == BM_BIG_ANTIDROPPING)	return "낙하물 방지";
	if(nIdxBigClass == BM_BIG_NAMEPANEL)		return "교명판";
	if(nIdxBigClass == BM_BIG_DESPANEL)			return "설명판";
	if(nIdxBigClass == BM_BIG_TBM)					return "TBM설치";
	if(nIdxBigClass == BM_BIG_STYROFOAM)		return "스티로폼";
	if(nIdxBigClass == BM_BIG_FLEXIBLEJOINT)		return "신축이음";
	if(nIdxBigClass == BM_BIG_FLEXIBLEJOINT_COVER) return "신축이음덮개";
	if(nIdxBigClass == BM_BIG_SHRINKAGE)		return "무수축 콘크리트";
	if(nIdxBigClass == BM_BIG_COVERPLATE)		return "COVER PLATE";
	if(nIdxBigClass == BM_BIG_MONUMENT)		return "교명주";
	if(nIdxBigClass == BM_BIG_NOTCH)				return "NOTCH";
	if(nIdxBigClass == BM_BIG_SCAFFOLD_LEG)	return "비계다리"; //todo 31274
	if(nIdxBigClass == BM_BIG_RUBBLEFILL)			return "잡석채움"; 
	if(nIdxBigClass == BM_BIG_DECKFINISH)		return "데크피니셔";

	return _T("");
}

// 공종의 이름이 옵션에 따라서 바뀔 경우를 대비해서 함수로 처리함.
// BOOL bAps : 접속슬래브 콘크리트 강도를 쓸지?
// long nTypeOfConcrete : 콘크리트 main일때의 종류(0:슬래브, 1:벽체, 2:내측벽체, 3:기초, 4:내측기초, 5, 날개벽, 6:접속슬래브, 7:교명주)    (0 : 기초, 1 : 본체, 2 : 접속슬래브, 3 : 교명주)
CString COutBM::GetStringMidClass(long nIdxMidClass, CApsApp *pAps, long nTypeOfConcrete, CGuardWallRC *pGW, double dUserFck)
{
	CRcBridgeRebar	*pBri	= GetRcBridgeRebar();
	CARcBridgeDataStd *pStd = m_pStd->m_pARcBridgeDataStd;

	if(!pBri) return _T("");
	long nTypeBM	= m_pStd->m_pARcBridgeDataStd->m_nTypeBMApply;

	if(nIdxMidClass == BM_MID_CONCRETE_MAIN || nIdxMidClass == BM_MID_CONCRETE_LEAN || 
		nIdxMidClass == BM_MID_CONCRETE_MASS || nIdxMidClass == BM_MID_CONCRETE_BOHO || 
		nIdxMidClass == BM_MID_CONCRETE_MONU || nIdxMidClass == BM_MID_CONCRETE_GUIDEWALL || nIdxMidClass == BM_MID_CONCRETE_GWHALF) 
	{
		CString str			= _T("");
		CString sFck		= _T(""),
				sFckLean	= _T(""),
				sFckMass	= _T(""),
				sFckBoho	= _T("");
		
		CString strCGS = _T("kgf/cm²");
		CString strSI  = pStd->GetStringUnitType(strCGS);
		BOOL bSI = pStd->m_nTypeUnit==UNIT_TYPE_SI;

		double  dFck		= pBri->GetValueFck((EPartOfBridge)nTypeOfConcrete);
		double	dFckLean	= pBri->m_dFckLeanConc;
		double	dFckMass	= pBri->m_dFckMassConc;
		double	dFckBoho	= m_pStd->m_nTypeConc[1] == 0 ? 150.0 : 210.0;
		if(pAps)
			dFck	= pAps->m_dFck;
		if(pGW)
			dFck	= pGW->m_dFck;
		if(nIdxMidClass == BM_MID_CONCRETE_MONU)
			dFck = pBri->m_dMonuFck;
		if(dUserFck > 0)
			dFck = dUserFck;
	
		
		dFck	 = bSI ? pStd->GetValueUnitType(dFck, strCGS, strSI)	 : dFck;
		dFckLean = bSI ? pStd->GetValueUnitType(dFckLean, strCGS, strSI) : dFckLean;
		dFckMass = bSI ? pStd->GetValueUnitType(dFckMass, strCGS, strSI) : dFckMass;
		dFckBoho = bSI ? pStd->GetValueUnitType(dFckBoho, strCGS, strSI) : dFckBoho;

		sFck.Format("%.0f", dFck);
		sFckLean.Format("%.0f", dFckLean);
		sFckMass.Format("%.0f", dFckMass);
		sFckBoho.Format("%.0f", dFckBoho);

		CString sAggregateMain	= _T(""),
				sAggregateLean	= _T(""),
				sAggregateMass	= _T(""),
				sAggregateBoho	= _T(""),
				sSlumpMain		= _T(""),
				sSlumpLean		= _T(""),
				sSlumpMass		= _T(""),
				sSlumpEtc	= _T(""),
				sSlumpBoho		= _T("");
			
		if(nTypeOfConcrete == ePartFootStt || nTypeOfConcrete == ePartFootEnd || nTypeOfConcrete == ePartInFoot)
			sAggregateMain.Format("%.0f-", pBri->GetValueAggregate((EPartOfBridge)nTypeOfConcrete));
		else if(nTypeOfConcrete == ePartApsStt || nTypeOfConcrete == ePartApsEnd)
			sAggregateMain.Format("%.0f-", pBri->GetValueAggregate((EPartOfBridge)nTypeOfConcrete));
		else if(nIdxMidClass == BM_MID_CONCRETE_MONU)
			sAggregateMain.Format("%.0f-", pBri->m_dMonuAggregate);			
		else
			sAggregateMain.Format("%.0f-", pGW ? pGW->m_dAggregate : pBri->GetValueAggregate((EPartOfBridge)nTypeOfConcrete)); //중분대, 방호벽 , 보호블럭 등

		
		sAggregateLean.Format("%.0f-", pBri->GetValueAggregateLean());
		sAggregateMass.Format("%.0f-", pBri->GetValueAggregateMass());
		sAggregateBoho.Format("%.0f-", pBri->GetValueAggregateEtc());
		
		sSlumpMain.Format("-%.0f", pBri->GetValueSlump((EPartOfBridge)nTypeOfConcrete));
		sSlumpLean.Format("-%.0f", pBri->GetValueSlumpLean());
		sSlumpMass.Format("-%.0f", pBri->GetValueSlumpMass());
		sSlumpBoho.Format("-%.0f", pBri->GetValueSlumpEtc()); //같은 것을 사용 나중에 기준이 바뀐다면 (보호블럭, 방호벽, 중분대, 교명주)
		sSlumpEtc.Format("-%.0f", pBri->GetValueSlumpEtc());

		CString strUnit = bSI ? strSI : strCGS;
		if(nIdxMidClass == BM_MID_CONCRETE_MAIN)
			return nTypeBM == TYPE_BM_NORMAL ? sAggregateMain+sFck+sSlumpMain		: "fck="+sFck+strUnit;
		if(nIdxMidClass == BM_MID_CONCRETE_LEAN)
			return nTypeBM == TYPE_BM_NORMAL ? sAggregateLean+sFckLean+sSlumpLean	: "fck="+sFckLean+strUnit;
		if(nIdxMidClass == BM_MID_CONCRETE_MASS)
			return nTypeBM == TYPE_BM_NORMAL ? sAggregateMass+sFckMass+sSlumpMass	: "fck="+sFckMass+strUnit;
		if(nIdxMidClass == BM_MID_CONCRETE_BOHO)
			return nTypeBM == TYPE_BM_NORMAL ? sAggregateBoho+sFckBoho+sSlumpBoho	: "fck="+sFckBoho+strUnit;
		if(nIdxMidClass == BM_MID_CONCRETE_MONU || nIdxMidClass == BM_MID_CONCRETE_GUIDEWALL || nIdxMidClass == BM_MID_CONCRETE_GWHALF)
			return nTypeBM == TYPE_BM_NORMAL ? sAggregateMain+sFck+sSlumpEtc	: "fck="+sFck+strUnit;
	}

	if(nIdxMidClass == BM_MID_FORM_3TIMES)		return nTypeBM == TYPE_BM_NORMAL ? "합판3회"			: "매끈한 마감";
	if(nIdxMidClass == BM_MID_FORM_PATTERN)		return "무늬 거푸집";
	if(nIdxMidClass == BM_MID_FORM_PATTERN_S)	return "설명된 특징있는 마감 (무늬거푸집)"; 
	if(nIdxMidClass == BM_MID_FORM_GANGJE_S)	return nTypeBM == TYPE_BM_NORMAL ? "강재거푸집"			: "설명된 특징있는 마감 (강재거푸집)";
	if(nIdxMidClass == BM_MID_FORM_COTTING_S)	return "설명된 특징있는 마감 (코팅거푸집)";
	if(nIdxMidClass == BM_MID_FORM_DECK_S)		return "설명된 특징있는 마감 (DECK PLATE)";
	if(nIdxMidClass == BM_MID_FORM_ONE_S)		return "설명된 특징있는 마감 (1회 사용)";
	if(nIdxMidClass == BM_MID_FORM_CIRCLE)		return "원형거푸집";
	if(nIdxMidClass == BM_MID_FORM_EURO)		return "유로폼";
	if(nIdxMidClass == BM_MID_FORM_4TIMES)		return nTypeBM == TYPE_BM_NORMAL ? "합판4회"			: "보통마감";
	if(nIdxMidClass == BM_MID_FORM_6TIMES)		return nTypeBM == TYPE_BM_NORMAL ? "합판6회"			: "거친마감";
	if(nIdxMidClass == BM_MID_SCAFFOLD_MAIN || nIdxMidClass == BM_MID_SCAFFOLD_MAIN_LEG)	return "본체";
	if(nIdxMidClass == BM_MID_SCAFFOLD_WING || nIdxMidClass == BM_MID_SCAFFOLD_WING_LEG)	return "날개벽"; // 31724
	if(nIdxMidClass == BM_MID_SLABCARE)			return "슬래브 양생";
	if(nIdxMidClass == BM_MID_CONCCARE)			return "콘크리트 양생";
	if(nIdxMidClass == BM_MID_REBAR)			return "철근가공조립";
	if(nIdxMidClass == BM_MID_TACK_RSC1)		return "";		// (ARCBRIDGE-1570) "RSC-1" 삭제 요청
	if(nIdxMidClass == BM_MID_SURFACEPROOF_PERCOLATE)	return pBri->m_nTypeSurfaceProof == 0 ? "침투식" : "쉬트방수";
	if(nIdxMidClass == BM_MID_ASPHALTPROOF)		return "ASP 방수";
	if(nIdxMidClass == BM_MID_STYROFOAM)		return "스티로폼";
	if(nIdxMidClass == BM_MID_FLEXIBLEJOINT)	return "신축이음";
	if(nIdxMidClass == BM_MID_FLEXIBLEJOINT_COVER)	return "신축이음덮개";
	if(nIdxMidClass == BM_MID_SHRINKAGE)		return "무수축 콘크리트";
	//if(nIdxMidClass == BM_MID_RUBBLEFILL)		return "잡석채움";

	return _T("");
}

CString COutBM::GetStringSmallClass(long &nIdxSmallClass, BOOL bStt)
{
	CRcBridgeRebar	*pBri = GetRcBridgeRebar();
	if(!pBri) return _T("");
	long nTypeBM	= m_pStd->m_pARcBridgeDataStd->m_nTypeBMApply;
	BOOL bSepFootingForSiljuk	= ((nTypeBM == TYPE_BM_ACTUAL) && (m_pStd->m_pARcBridgeDataStd->m_bSepFootingForSiljuk));

	if(nIdxSmallClass == BM_SMALL_CONCRETE_FOOTING)		return nTypeBM == TYPE_BM_NORMAL ? "기초"			: 
																							(bSepFootingForSiljuk) ? "기초" : "바닥 및 보";
	if(nIdxSmallClass == BM_SMALL_CONCRETE_MAIN)		return nTypeBM == TYPE_BM_NORMAL ? "본체"			: "벽 및 기둥";
	if(nIdxSmallClass == BM_SMALL_CONCRETE_LEAN)		return nTypeBM == TYPE_BM_NORMAL ? "버림콘크리트"	: "빈배합";
	if(nIdxSmallClass == BM_SMALL_CONCRETE_MASS)		return "MASS 콘크리트";
	if(nIdxSmallClass == BM_SMALL_CONCRETE_WALL)		return "벽 및 기둥";
	if(nIdxSmallClass == BM_SMALL_CONCRETE_WING)	
	{
		if(nTypeBM != TYPE_BM_NORMAL)						// 일반 산출 기준에서는 날개벽이 빠져야 하므로 BM_SMALL_CONCRETE_WING
			nIdxSmallClass = BM_SMALL_CONCRETE_MAIN;		// 그 외는 포함 해야 하므로 BM_SMALL_CONCRETE_MAIN
		return nTypeBM == TYPE_BM_NORMAL ? "날개벽"	: "벽 및 기둥";
	}
	if(nIdxSmallClass == BM_SMALL_CONCRETE_APS)			return nTypeBM == TYPE_BM_NORMAL ? "접속슬래브"		: "바닥 및 보";
	if(nIdxSmallClass == BM_SMALL_CONCRETE_CNS)			return nTypeBM == TYPE_BM_NORMAL ? "완충슬래브"		: "바닥 및 보";
	if(nIdxSmallClass == BM_SMALL_CONCRETE_BOHO_FOOTING)	return "기초블럭 CON'C";
	if(nIdxSmallClass == BM_SMALL_CONCRETE_BOHO_BACKFILL)	return "뒷채움 CON'C";
	if(nIdxSmallClass == BM_SMALL_CONCRETE_FLOOR)		return "바닥 및 보";
	if(nIdxSmallClass == BM_SMALL_FORM_VERTICAL)		return "수직면";
	if(nIdxSmallClass == BM_SMALL_FORM_SLOPE)			return "경사면";
	if(nIdxSmallClass == BM_SMALL_FORM_CURVE)			return "곡면";
	if(nIdxSmallClass == BM_SMALL_FORM_HORI)			return "수평면";
	if(nIdxSmallClass == BM_SMALL_POST_STEELPIPE)		return "강관동바리";
	if(nIdxSmallClass == BM_SMALL_POST_WOOD)			return "목재동바리";
	if(nIdxSmallClass == BM_SMALL_POST_BRIDGE_STEEL)	return "강재동바리(육교용)";
	if(nIdxSmallClass == BM_SMALL_POST_HORI_CONNECT)	return "수평연결재";
	if(nIdxSmallClass == BM_SMALL_SCAFFOLD_MAIN)		return GetStringMidClass(BM_MID_SCAFFOLD_MAIN);
	if(nIdxSmallClass == BM_SMALL_SCAFFOLD_WING)		return GetStringMidClass(BM_MID_SCAFFOLD_WING);
	if(nIdxSmallClass == BM_SMALL_SPACER_WALL)			return "벽체용";
	if(nIdxSmallClass == BM_SMALL_SPACER_SLAB)			return "슬래브 및 기초용";
	if(nIdxSmallClass == BM_SMALL_CONCCARE)				return "콘크리트 양생";
	if(nIdxSmallClass == BM_SMALL_ASPHALTPROOF)			return _T("");	// (ARCBRIDGE-1570) "방수공 1회" 삭제 요청
	if(nIdxSmallClass == BM_SMALL_REBAR_SIMPLE || nIdxSmallClass == BM_SMALL_DOWELBAR)			return "간단";
	if(nIdxSmallClass == BM_SMALL_REBAR_NORMAL)			return "보통";
	if(nIdxSmallClass == BM_SMALL_REBAR_COMPLEXITY1)	return "복잡";
	if(nIdxSmallClass == BM_SMALL_REBAR_COMPLEXITY2)	return "매우복잡";
	if(nIdxSmallClass == BM_SMALL_STYROFOAM)			return "";
	if(nIdxSmallClass == BM_SMALL_FLEXIBLEJOINT)		return bStt ? pBri->m_fxJointStt.m_sName : pBri->m_fxJointEnd.m_sName;
	if(nIdxSmallClass == BM_SMALL_SHRINKAGE)			return bStt ? pBri->m_fxJointStt.m_sName : pBri->m_fxJointEnd.m_sName;
	if(nIdxSmallClass == BM_SMALL_SCAFFOLD_WING_LEG)	return GetStringMidClass(BM_MID_SCAFFOLD_WING_LEG);// 31724
	if(nIdxSmallClass == BM_SMALL_SCAFFOLD_MAIN_LEG)	return GetStringMidClass(BM_MID_SCAFFOLD_MAIN_LEG);
	if(nIdxSmallClass == BM_SMALL_FJ_NONE)				return "차도";
	if(nIdxSmallClass == BM_SMALL_FJ_GUARDFENCE)		return "방호벽";
	if(nIdxSmallClass == BM_SMALL_FJ_CENTERGUARD)		return "중분대";
	if(nIdxSmallClass == BM_SMALL_FJ_CENTERGUARDHALF)	return "중분대1/2";
	if(nIdxSmallClass == BM_SMALL_FJ_FOOTWAY)			return "보도";
	if(nIdxSmallClass == BM_SMALL_FJ_FOOTWAYBLOCK)		return "보차도 경계석";
	if(nIdxSmallClass == BM_SMALL_FJ_SOUNDPROOF)		return "방음벽 기초";
	if(nIdxSmallClass == BM_SMALL_FJ_RETAININGWALL)		return "흙막이벽";
	if(nIdxSmallClass == BM_SMALL_FJ_HANDRAIL)			return "난간기초";
	if(nIdxSmallClass == BM_SMALL_FJ_DIKE)				return "다이크";
	//if(nIdxSmallClass == BM_SMALL_RUBBLEFILL)				return "잡석채움";

	

	return _T("");
}

// 본체 거푸집 산출
void COutBM::MakeBMFormMain()
{
	CRcBridgeRebar *pBri = GetRcBridgeRebar();
	if(!pBri) return;

	CRcBridgeRebar *pBriOrg	= GetRcBridgeRebar(TRUE);
	if(!pBriOrg) return;

	// 영역 구분.
	CHgBaseBMStd baseBM;
	CalAreaConc(pBri, FALSE);
	
	long	nSize = 0;
	double	dSttMeter = 0;
	double	dEndMeter = 0;

	long	nSizeBM	= m_pArrBMBridgeStd.GetSize();
	double	dFirstMeter	= 7000;
	double	dSeconeMeter = 3000;

	// 인접교대수량 공제 ///////////////////////////////////
	const HGBOOL bDeductLeft	= pBri->m_nDeductNearFormBM == 1;
	const HGBOOL bDeductRight	= pBri->m_nDeductNearFormBM == 2;
	/////////////////////////////////////////////////////////

	CRebarPlacing rb;

	// 기초
	// 일반형 : 평면영역을 기준으로 구함.
	// slope형 : 정면을 기준으로 구함.
	long nCountFooting	= pBri->GetCountFooting();
	long i = 0; 
	for(i = 0; i < nCountFooting; i++)
	{
		if(pBri->IsBoxType()) break; // 본체에서 구함

		CFootingApp *pFooting	= pBri->GetFooting(i);
		if(pFooting)
		{
			// 일반형
			if(pFooting->m_nType == FOOTING_TYPE_BASIC || pFooting->m_nType == FOOTING_TYPE_BASIC_BOTH)
			{
				double dHeight	= pFooting->GetHeight();
				nSize	= pFooting->m_pArrAreaConcPlane.GetSize();
				long j = 0; for(j = 0; j < nSize; j++)
				{
					CTwinVectorAreaShape *pArea	= pFooting->m_pArrAreaConcPlane.GetAt(j);

					// 수직면
					CBMData *pBMVert				= new CBMData;
					pBMVert->m_nNumbering			= pArea->m_nNum;
					pBMVert->m_nIdxBigClass			= BM_BIG_FORM;
					pBMVert->m_sBigClass			= GetStringBigClass(pBMVert->m_nIdxBigClass);
					pBMVert->m_nLevelTotal			= GetLevelTotal(pBMVert->m_nIdxBigClass);
					pBMVert->m_nIdxMidClass			= GetFormByPos(FORM_POS_FOOTING);
					pBMVert->m_sMidClass			= GetStringMidClass(pBMVert->m_nIdxMidClass);
					pBMVert->m_nIdxSmallClass		= BM_SMALL_FORM_VERTICAL;
					pBMVert->m_sSmallClass			= GetStringSmallClass(pBMVert->m_nIdxSmallClass);
					pBMVert->m_nTypeResult			= RESULT_AREA;
					pBMVert->m_sUnit				= "㎡";
					pBMVert->m_nShape				= pArea->m_nTypeShape;
					pBMVert->m_dV[WidthLower]		= Round(toM(pArea->m_dWidthLower),3);
					pBMVert->m_dV[WidthUpper]		= Round(toM(pArea->m_dWidthUpper),3);
					pBMVert->m_dV[WidthDiffLeft]	= Round(toM(pArea->m_dWidthDiffLeft),3);
					pBMVert->m_dV[WidthDiffRight]	= Round(toM(pArea->m_dWidthDiffRight),3);
					pBMVert->m_dV[Height]			= Round(toM(pArea->m_dHeight),3);
					pBMVert->m_dV[Length]			= Round(toM(dHeight),3);
					pBMVert->m_dwSide				= SIDE_UPPER | SIDE_LOWER | SIDE_LEFT | SIDE_RIGHT;
					pBMVert->m_sInfo				= "기초거푸집";
					if(bDeductLeft)
					{
						pBMVert->m_dwSide &= ~SIDE_LEFT;
						AddBMStyroformBySide(pBMVert, SIDE_LEFT);
					}
					else if(bDeductRight)
					{
						pBMVert->m_dwSide &= ~SIDE_RIGHT;
						AddBMStyroformBySide(pBMVert, SIDE_RIGHT);
					}
										
					pBMVert->CalculateResult();
					m_pArrBMBridgeStd.Add(pBMVert);
				}			
			}
			// slop형 
			// 사교이거나 확폭일땐 대략 곤란.. ㅡㅡ;
			else if(   pFooting->m_nType == FOOTING_TYPE_LEFTSLOPE
					|| pFooting->m_nType == FOOTING_TYPE_RIGHTSLOPE
					|| pFooting->m_nType == FOOTING_TYPE_BOTHSLOPE
					|| pFooting->m_nType == FOOTING_TYPE_BASIC_LEFTSLOPE
					|| pFooting->m_nType == FOOTING_TYPE_BASIC_RIGHTSLOPE)
			{
				nSize	= pFooting->m_pArrAreaConc.GetSize();
				long j = 0; for(j = 0; j < nSize; j++)
				{
					CTwinVectorAreaShape *pArea	= pFooting->m_pArrAreaConc.GetAt(j);

					// 수직면
					CBMData *pBMVert				= new CBMData;
					pBMVert->m_nNumbering			= pArea->m_nNum;
					pBMVert->m_nIdxBigClass			= BM_BIG_FORM;
					pBMVert->m_sBigClass			= GetStringBigClass(pBMVert->m_nIdxBigClass);
					pBMVert->m_nLevelTotal			= GetLevelTotal(pBMVert->m_nIdxBigClass);
					pBMVert->m_nIdxMidClass			= GetFormByPos(FORM_POS_FOOTING);
					pBMVert->m_sMidClass			= GetStringMidClass(pBMVert->m_nIdxMidClass);
					pBMVert->m_nIdxSmallClass		= BM_SMALL_FORM_VERTICAL;
					pBMVert->m_sSmallClass			= GetStringSmallClass(pBMVert->m_nIdxSmallClass);
					pBMVert->m_nTypeResult			= RESULT_AREA;
					pBMVert->m_sUnit				= "㎡";
					pBMVert->m_nShape				= pArea->m_nTypeShape;
					pBMVert->m_dV[WidthLower]		= Round(toM(pArea->m_dWidthLower),3);
					pBMVert->m_dV[WidthUpper]		= Round(toM(pArea->m_dWidthUpper),3);
					pBMVert->m_dV[WidthDiffLeft]	= Round(toM(pArea->m_dWidthDiffLeft),3);
					pBMVert->m_dV[WidthDiffRight]	= Round(toM(pArea->m_dWidthDiffRight),3);
					pBMVert->m_dV[Height]			= Round(toM(pArea->m_dHeight),3);
					pBMVert->m_dV[Length]			= Round(toM(pFooting->GetLength()),3);
					if(pArea->m_nTypeShape == SHAPE_TV_RECTANGLE)
						pBMVert->m_dwSide	= SIDE_FRONT | SIDE_BACK | SIDE_LEFT | SIDE_RIGHT;
					else if(pArea->m_nTypeShape == SHAPE_TV_TRIANGLE || pArea->m_nTypeShape == SHAPE_TV_REVTRIANGLE || pArea->m_nTypeShape == SHAPE_TV_TRAPEZOID)
					{
						pBMVert->m_dwSide	= SIDE_FRONT | SIDE_BACK;
						if(Compare(pBMVert->m_dV[WidthDiffLeft], 0.0, "="))
							pBMVert->m_dwSide	|= SIDE_LEFT;
						if(Compare(pBMVert->m_dV[WidthDiffRight], 0.0, "="))
							pBMVert->m_dwSide	|= SIDE_RIGHT;
					}

					pBMVert->m_sInfo				= "기초거푸집";
					if(bDeductLeft)
					{
						pBMVert->m_dwSide &= ~SIDE_FRONT;
						AddBMStyroformBySide(pBMVert, SIDE_FRONT);
					}
					else if(bDeductRight)
					{
						pBMVert->m_dwSide &= ~SIDE_BACK;
						AddBMStyroformBySide(pBMVert, SIDE_BACK);
					}

					pBMVert->CalculateResult();
					m_pArrBMBridgeStd.Add(pBMVert);

					// 경사면
					double dLenFooting	= Round(toM(pFooting->GetLength()),3);
					if(!Compare(pBMVert->m_dV[WidthDiffLeft], 0.0, "="))
					{
						CBMData *pBMSlope			= new CBMData(pBMVert);
						pBMSlope->m_nIdxSmallClass	= BM_SMALL_FORM_SLOPE;
						pBMSlope->m_sSmallClass		= GetStringSmallClass(pBMSlope->m_nIdxSmallClass);
						pBMSlope->m_sUnit			= "㎡";
						pBMSlope->m_dwSide			= 0;
						pBMSlope->m_nTypeResult		= RESULT_AREA;
	
						CTwinVector tvLeft	= pArea->GetTvLeft();
						double dHeight = toM(tvLeft.GetVerLength());
						double dWidth  = toM(tvLeft.GetHorLength());
						if(tvLeft.m_sInfo != "")
						{
							// overlap 체크해서 좌측높이나 기본높이등의 값을 변경해줌 /////////////////
							CTwinVectorArray tvArrLeft;
							tvArrLeft.Add(tvLeft);

							for(long wall=0; wall<pBri->GetCountInWall(); wall++)
							{
								CWallApp *pWall	= pBri->GetInWall(wall);
								if(!pWall) continue;
								if(!pWall->m_bIs) continue;

								long j = 0; for(j = 0; j < pWall->m_pArrAreaConc.GetSize(); j++)
								{
									CTwinVectorAreaShape *pAreaShape	= pWall->m_pArrAreaConc.GetAt(j);
									CTwinVectorArray tvArrShape;
									tvArrShape	= pAreaShape->m_tvArr;
									if(rb.CheckOverlapLineByTvArr(tvArrLeft, tvArrShape, TRUE, TRUE))
									{
										// 기초 경사면 좌측과 벽체 하면(삼각형 오른쪽)이 오버렙한다고 가정
										CTwinVector tv	= pAreaShape->GetTvRight();	
										dHeight -= toM(tv.GetVerLength());
										dWidth  -= toM(tv.GetHorLength());
									}
								}
							}
							
							pBMSlope->m_dwSide |= SIDE_LEFT;
							pBMSlope->m_sInfo = "기초거푸집";
							pBMSlope->m_sForm.Format("√((%.3f x %.3f) + (%.3f x %.3f)) x %.3f", dHeight, dHeight, dWidth, dWidth, dLenFooting);
							pBMSlope->m_sFormExcel	= pBMSlope->m_sForm;
							pBMSlope->m_bIsPrintByRefForm	= TRUE;
							pBMSlope->SetResult(sqrt(dHeight * dHeight + dWidth * dWidth) * dLenFooting);
							m_pArrBMBridgeStd.Add(pBMSlope);
						}
					}
					if(!Compare(pBMVert->m_dV[WidthDiffRight], 0.0, "="))
					{
						CBMData *pBMSlope			= new CBMData(pBMVert);
						pBMSlope->m_nIdxSmallClass	= BM_SMALL_FORM_SLOPE;
						pBMSlope->m_sSmallClass		= GetStringSmallClass(pBMSlope->m_nIdxSmallClass);
						pBMSlope->m_sUnit			= "㎡";
						pBMSlope->m_dwSide			= 0;
						pBMSlope->m_nTypeResult		= RESULT_AREA;

						CTwinVector tvRight	= pArea->GetTvRight();
						double dHeight = toM(tvRight.GetVerLength());
						double dWidth  = toM(tvRight.GetHorLength());

						if(tvRight.m_sInfo != "")
						{								
							// overlap 체크해서 좌측높이나 기본높이등의 값을 변경해줌 /////////////////
							CTwinVectorArray tvArrRight;
							tvArrRight.Add(tvRight);

							for(long wall=0; wall<pBri->GetCountInWall(); wall++)
							{
								CWallApp *pWall	= pBri->GetInWall(wall);
								if(!pWall) continue;
								if(!pWall->m_bIs) continue;

								long j=0; for(j=0; j<pWall->m_pArrAreaConc.GetSize(); j++)
								{
									CTwinVectorAreaShape *pAreaShape	= pWall->m_pArrAreaConc.GetAt(j);
									CTwinVectorArray tvArrShape;
									tvArrShape	= pAreaShape->m_tvArr;
									if(rb.CheckOverlapLineByTvArr(tvArrRight, tvArrShape, TRUE, TRUE))
									{
										// 기초 경사면 우측과 벽체 하면(삼각형 왼쪽)이 오버렙한다고 가정
										CTwinVector tv	= pAreaShape->GetTvLeft();	
										dHeight -= toM(tv.GetVerLength());
										dWidth  -= toM(tv.GetHorLength());
									}
								}
							}
							
							pBMSlope->m_dwSide |= SIDE_RIGHT;
							pBMSlope->m_sInfo = "기초거푸집";
							pBMSlope->m_sForm.Format("√((%.3f x %.3f) + (%.3f x %.3f)) x %.3f", dHeight, dHeight, dWidth, dWidth, dLenFooting);
							pBMSlope->m_sFormExcel	= pBMSlope->m_sForm;
							pBMSlope->m_bIsPrintByRefForm	= TRUE;
							pBMSlope->SetResult(sqrt(dHeight * dHeight + dWidth * dWidth) * dLenFooting);
							m_pArrBMBridgeStd.Add(pBMSlope);
						}
					}
				}
			}
		}
	}
	
	//////////////////////////////////////////////////////////////////////////
	// 본체 and 내측벽체(기둥인 경우 따로구함)
	// 시점벽체 좌측과, 종점벽체 우측은 본체 길이에서 날개벽 두께를 빼줘야된다.
	CDPoint xyDirStt		= pBri->GetAngleByStation(pBri->m_dStationBridgeStt);
	CDPoint xyDirEnd		= pBri->GetAngleByStation(pBri->m_dStationBridgeStt+pBri->m_dLengthBridge);
	double dBridgeLength	= pBri->GetWidthSlabAct(pBri->m_dStationBridgeStt, xyDirStt, 0, TRUE);
	double dSttBridgeLength	= pBri->GetWidthSlabAct(pBri->m_dStationBridgeStt, xyDirStt, 0, TRUE);
	double dEndBridgeLength	= pBri->GetWidthSlabAct(pBri->m_dStationBridgeStt+pBri->m_dLengthBridge, xyDirEnd, 0, TRUE);

	// 브라켓있는 경우 브라켓측면은 날개벽헌치의 영향에서 벗어나는 경우가 있어서 본체 길이가 좀 더 길어 질 수 있다.
	double dSttBridgeLengthBracket	= dSttBridgeLength;
	double dEndBridgeLengthBracket	= dEndBridgeLength;

	// 시종점 외측 벽체길이에 날개벽 두께 적용된 길이 //////////////////////////////////////////////
	CDPoint xyMatch(0, 0);
	pBri->GetTvWingWallPlane(TRUE);
	CTwinVector tvSlab;
	if(pBri->m_tvArrPlaneWingWall.GetTvByInfo("슬래브끝단내측", tvSlab))
		dSttBridgeLength	= tvSlab.GetLength();
	if(pBri->m_tvArrPlaneWingWall.GetTvByInfo("브라켓", tvSlab))
		dSttBridgeLengthBracket	= tvSlab.GetLength();
	else
		dSttBridgeLengthBracket	= dSttBridgeLength;

	pBri->GetTvWingWallPlane(FALSE);
	if(pBri->m_tvArrPlaneWingWall.GetTvByInfo("슬래브끝단내측", tvSlab))
		dEndBridgeLength	= tvSlab.GetLength();
	if(pBri->m_tvArrPlaneWingWall.GetTvByInfo("브라켓", tvSlab))
		dEndBridgeLengthBracket	= tvSlab.GetLength();
	else
		dEndBridgeLengthBracket	= dEndBridgeLength;
	///////////////////////////////////////////////////////////////////////////////////////////////
	
	double dYBase	= GetElBaseForForm(-1);
	long nOpt = pBri->m_nHeightForSlabBM;


	// part : 0 은 본체
	// 1 부터 내측벽체(기둥식은 따로 구함)
	long nCountInWall	= pBri->GetCountInWall();
	for(long part = 0; part < nCountInWall+1; part++)
	{
		dYBase	= GetElBaseForForm(-1);
		CWallApp *pWall	= NULL;
		CTypedPtrArray <CObArray, CTwinVectorAreaShape*> *pArrAreaConc;
		if(part == 0)
			pArrAreaConc	= &pBri->m_pArrAreaConc;
		else
		{
			pWall	= pBri->GetInWall(part-1);
			if(!pWall) continue;
			if(!pWall->m_bIs) continue;
			pArrAreaConc	= &pWall->m_pArrAreaConc;
		}
	
		long nFirstSize = pArrAreaConc->GetSize();
		if(part==0)	// slab를 나눈다
			baseBM.RemakeByHeightTvAreaShape(pArrAreaConc, dYBase, dFirstMeter, dSeconeMeter, TRUE, nOpt);
		else
			baseBM.RemakeByHeightTvAreaShape(pArrAreaConc, dYBase, dFirstMeter, dSeconeMeter);

		nSize	= pArrAreaConc->GetSize();
		long nAdd = nSize - nFirstSize;	// 슬래브는 마지막 인데...도형이 두개로 분리된 경우 하나가 더 늘어난다.
		
		for(i = 0; i < nSize; i++)
		{
			CTwinVectorAreaShape *pArea	= pArrAreaConc->GetAt(i);
			
			// 현재 sta에서의 벽체/본체 길이 조사(벽체 기준임(상부슬래브 부분은 따로 파악해서 길이를 조정해야 됨))
			CDPointArray xyArr;
			pArea->m_tvArr.GetToPointArray(xyArr);
			double dStaLeft		= rb.GetXyEdgePoint(xyArr, UPLEFT_AREA).x;
			double dStaRight	= rb.GetXyEdgePoint(xyArr, UPRIGHT_AREA).x;
			double dStaMid		= (dStaLeft+dStaRight)/2;

			// 라운드 벽체와 기둥식 벽체는 수정사항이 좀 있음.
			BOOL bRoundRectWall	= part > 0 && (pWall->IsRoundRectWall() || pWall->m_bTopIsColumn);

			// 지점별 거리
			// 날개벽등의 구조물에 의해서 길이가 수정되어야 하는 경우에 대한 값은
			// 이미 구해져 있음 (dSttBridgeLength 등....)
			CDPoint xyDir	= pBri->GetAngleByStation(dStaMid);
			dBridgeLength	= pBri->GetWidthSlabAct(dStaMid, xyDir, 0, FALSE);

			// 교량길이에서 공제 해야 되는 상황 고려 /////////////////////////////////////////////
			// [1] 시종점 브라켓은 날개벽 고려해야 됨.
			// 수정예정 : 단변화가 세로인지 가로인지에 따라서 정확하게 계산이 되어야 함.
			if(pArea->m_sInfo == "시점브라켓" || pArea->m_sInfo == "종점브라켓")
			{
				BOOL bStt = pArea->m_sInfo == "시점브라켓";
				pBri->GetTvWingWallPlane(bStt, TRUE, TRUE);		// 브라켓 길이 계산
				CTwinVector tv = pBri->m_tvArrPlaneWingWall.GetTvByInfo("브라켓");
				dBridgeLength = tv.GetLength();

				if(bStt)
					dSttBridgeLengthBracket = dBridgeLength;
				else
					dEndBridgeLengthBracket = dBridgeLength;
			}
			// 가각또는 확폭일때 교량 폭을 슬래브 끝단 내측으로 바꿔준다.
			if((pBri->IsExtLine() || pBri->IsExp(dStaMid)) && (pArea->m_sInfo == "시점벽체" || pArea->m_sInfo == "종점벽체"))
			{
				BOOL bStt = pArea->m_sInfo == "시점벽체";
				pBri->GetTvWingWallPlane(bStt);		// 브라켓 길이 계산
				CTwinVector tv = pBri->m_tvArrPlaneWingWall.GetTvByInfo("슬래브끝단외측");
				dBridgeLength = tv.GetLength();
			}

			// [2] 브라켓을 제외한 곳의 헌치들은 끝단에서 헌치까지 들어간 거리 고려
			if(pArea->m_sInfo.Find("벽체헌치", 0) != -1 || pArea->m_sInfo.Find("내측헌치", 0) != -1)
			{
				double dJijumAng = xyDir.GetAngleRadian();

				if(pArea->m_sInfo.Find("내측헌치", 0) != -1)
				{
					dBridgeLength -= (pBri->m_dDH[0] + pBri->m_dDH[1])/sin(dJijumAng);
				}
				else
				{
					BOOL bStt = pArea->m_sInfo.Find(_T("시점"))>-1? TRUE : FALSE;
					pBri->GetTvWingWallPlane(bStt);		// 브라켓 길이 계산
					CTwinVector tv = pBri->m_tvArrPlaneWingWall.GetTvByInfo("슬래브끝단외측");
					dBridgeLength = tv.GetLength();
				}		
				
				if(pBri->m_bCantilSlab[0])
					dBridgeLength -= pBri->m_dCantilB[0]/sin(dJijumAng);
				if(pBri->m_bCantilSlab[1])
					dBridgeLength -= pBri->m_dCantilB[1]/sin(dJijumAng);
			}

			// [3] 중간지점의 경우 헌치길이까지만 구해야 됨
			if(pWall)
				dBridgeLength	= pWall->m_tvPathPlan.GetLength();

			// [4] 아치리브는 아치리브 넓이를 사용함
			if(pArea->m_sInfo == "리브")
				dBridgeLength	= pBri->GetWidthArchRib(0);
			//////////////////////////////////////////////////////////////////////////////////////////////

			// 상부슬래브라면 옵션에 따라 선형길이를 사용하기도 한다.//
			if(pArea->m_sInfo == "상부슬래브")
			{
				if(pBri->m_nTypeSlabLength == 1)
				{
					pArea->m_dWidth	= pBriOrg->GetLengthSlabReal(0);
					pArea->m_dWidthLower	= pArea->m_dWidth;
					pArea->m_dWidthUpper	= pArea->m_dWidth;
				}
			}		
			///////////////////////////////////////////////////////

			long nTypeLeft	= 0;
			long nTypeRight	= 0;
			GetInfoPosAreaConc(pArea, nTypeLeft, nTypeRight);	

			dSttMeter	= pArea->GetTvLeft().GetXyBottom().y-dYBase;
			dEndMeter	= pArea->GetTvLeft().GetXyTop().y-dYBase;

			// 앞뒤(수직면)
			CBMData baseBMVert;
			baseBMVert.m_nNumbering				= pArea->m_nNum;
			baseBMVert.m_nIdxBigClass			= BM_BIG_FORM;
			baseBMVert.m_sBigClass				= GetStringBigClass(baseBMVert.m_nIdxBigClass);
			baseBMVert.m_nLevelTotal			= GetLevelTotal(baseBMVert.m_nIdxBigClass); 
			baseBMVert.m_nIdxMidClass			= GetFormByPos(FORM_POS_NORMAL);
			baseBMVert.m_sMidClass				= GetStringMidClass(baseBMVert.m_nIdxMidClass);
			baseBMVert.m_nIdxSmallClass			= BM_SMALL_FORM_VERTICAL;
			baseBMVert.m_sSmallClass			= GetStringSmallClass(baseBMVert.m_nIdxSmallClass);
			baseBMVert.m_nTypeResult			= RESULT_AREA;
			baseBMVert.m_sUnit					= "㎡";
			baseBMVert.m_nShape					= pArea->m_nTypeShape;
			baseBMVert.m_dV[SttMeter]			= Round(toM(dSttMeter),3);
			baseBMVert.m_dV[EndMeter]			= Round(toM(dEndMeter),3);

			if(part==0 && ((i==nSize-nAdd-2 && pBri->m_bIsExp) || i==nSize-nAdd-1) && pBri->m_nHeightForSlabBM > 0)
				baseBMVert.m_bSlab = TRUE;	// 한개의 슬래브로



			CBMData *pBMVert				= new CBMData(&baseBMVert);

			if(bRoundRectWall)	// 기둥식 벽체나 라운드 벽체
			{
				pBMVert->m_nShape			= SHAPE_TV_ROUNDRECTANGLE_PLANE;
				pBMVert->m_dV[WidthLower]	= Round(toM(pWall->IsRoundRectWall() ? pWall->m_dW : pWall->m_Column.m_dB[iOUTTER][iUPPER]),3);
				pBMVert->m_dV[WidthUpper]	= pBMVert->m_dV[WidthLower];
				pBMVert->m_dV[Width]	= pBMVert->m_dV[WidthLower];
				pBMVert->m_dV[Length]		= Round(toM(pWall->IsRoundRectWall() ? dBridgeLength : pWall->m_Column.m_dH[iOUTTER][iUPPER]),3);
				pBMVert->m_dV[Radius]		= Round(toM(pWall->IsRoundRectWall() ? pWall->m_dW/2 : pWall->m_Column.m_dR[iOUTTER][iUPPER]),3);
				pBMVert->m_dV[Height]		= Round(toM(pArea->m_dHeight),3);
			}
			else
			{
				pBMVert->m_dV[Width]			= Round(toM(pArea->m_dWidth),3);
				pBMVert->m_dV[WidthLower]		= Round(toM(pArea->m_dWidthLower),3);
				pBMVert->m_dV[WidthUpper]		= Round(toM(pArea->m_dWidthUpper),3);
				pBMVert->m_dV[WidthDiffLeft]	= Round(toM(pArea->m_dWidthDiffLeft),3);
				pBMVert->m_dV[WidthDiffRight]	= Round(toM(pArea->m_dWidthDiffRight),3);
				pBMVert->m_dV[Height]			= Round(toM(pArea->m_dHeight),3);
				pBMVert->m_dV[HeightLeft]		= Round(toM(pArea->m_dHeightLeft),3);
				pBMVert->m_dV[HeightRight]		= Round(toM(pArea->m_dHeightRight),3);
				pBMVert->m_dV[Length]			= Round(toM(dBridgeLength),3);			// 사실 여기선 의미가 없는 값
				pBMVert->m_dV[ArcAreaR]			= Round(toM(toM(pArea->m_dArcAreaR)),3);
				pBMVert->m_dV[ArcAreaL]			= Round(toM(toM(pArea->m_dArcAreaL)),3);
				pBMVert->m_dV[ArcLengthR]		= Round(toM(pArea->m_dArcLengthR),3);
				pBMVert->m_dV[ArcLengthL]		= Round(toM(pArea->m_dArcLengthL),3);
			}

			baseBMVert.m_nShape				= pBMVert->m_nShape;
			baseBMVert.m_dV[Width]			= pBMVert->m_dV[Width];
			baseBMVert.m_dV[WidthLower]		= pBMVert->m_dV[WidthLower];
			baseBMVert.m_dV[WidthUpper]		= pBMVert->m_dV[WidthUpper];
			baseBMVert.m_dV[WidthDiffLeft]	= pBMVert->m_dV[WidthDiffLeft];
			baseBMVert.m_dV[WidthDiffRight]	= pBMVert->m_dV[WidthDiffRight];
			baseBMVert.m_dV[Height]			= pBMVert->m_dV[Height];
			baseBMVert.m_dV[HeightLeft]		= pBMVert->m_dV[HeightLeft];
			baseBMVert.m_dV[HeightRight]	= pBMVert->m_dV[HeightRight];
			baseBMVert.m_dV[Radius]			= pBMVert->m_dV[Radius];
			baseBMVert.m_dV[Length]			= pBMVert->m_dV[Length];

			pBMVert->m_dwSide	= SIDE_FRONT | SIDE_BACK;
			pBMVert->m_sInfo = "본체거푸집";
			if(bDeductLeft)
			{
				pBMVert->m_dwSide &= ~SIDE_FRONT;
				AddBMStyroformBySide(pBMVert, SIDE_FRONT);
			}
			else if(bDeductRight)
			{
				pBMVert->m_dwSide &= ~SIDE_BACK;
				AddBMStyroformBySide(pBMVert, SIDE_BACK);
			}

			pBMVert->CalculateResult();
			

			if(pArea->m_sInfo == "리브")	// 아치교 리브
			{
				CTwinVector tv;
				CDPointArray vArr;
				long nSize = pBri->m_tvArrVertSectionRib[0].GetSize();
				long n=0; for(n=0; n<nSize; n++)
				{
					tv = pBri->m_tvArrVertSectionRib[0].GetAt(n);
					vArr.Add(tv.m_v1);
					if(n==nSize-1)
						vArr.Add(tv.m_v2);
				}
				nSize = pBri->m_tvArrVertSectionRib[1].GetSize();
				for(n=nSize-1; n>=0; n--)
				{
					tv = pBri->m_tvArrVertSectionRib[1].GetAt(n);
					vArr.Add(tv.m_v2);
					if(n==0)
						vArr.Add(tv.m_v1);
				}
				double dArea = pBri->GetPolygonArea(vArr, vArr.GetSize());
				HGINT32 nFace = bDeductLeft || bDeductRight ? 1 : 2;
				CString strTail;
				strTail.Format(_T("리브 면적 × %d면"), nFace);
				pBMVert->m_sNoteTail = strTail;
				pBMVert->m_dV[Result] = toM(toM(dArea)) * nFace;

				CBMData *pBM	= CreateBMStyrotormForSide(pBMVert);
				if (pBM)
				{
					m_pArrBMBridgeStd.Add(pBM);
				}
			}

			// 가각인 경우 교량 길이는 다시 구하고좌우측길이가 다를 수 있으므로 따로 구함
			BOOL bFindSlab = pArea->m_sInfo.Find(_T("슬래브")) > -1? TRUE : FALSE;
			if(pBri->m_bIsExp && pBMVert->m_bSlab  && bFindSlab)
			{
				CTwinVectorArray tvArrPlane;
				pBri->GetTvRebarOut_Plane_UpperSlabStd(tvArrPlane, -1, TRUE, 2, TRUE, TRUE);
				double dLenLeft		= 0;
				double dLenRight	= 0;
				CTwinVector tv;
				long i = 0; for(i = 0; i < tvArrPlane.GetSize(); i++)
				{
					tv	= tvArrPlane.GetAt(i);
					if(tv.m_sInfo.Find("좌", 0) != -1) dLenLeft += tv.GetLength();
					if(tv.m_sInfo.Find("우", 0) != -1) dLenRight += tv.GetLength();
				}

				pBMVert->m_dwSide				= SIDE_BACK;
				pBMVert->m_dV[Width]			= Round(toM(dLenLeft),3);
				pBMVert->m_dV[WidthLower]		= Round(toM(dLenLeft),3);
				pBMVert->m_dV[WidthUpper]		= Round(toM(dLenLeft),3);
				pBMVert->CalculateResult();
				m_pArrBMBridgeStd.Add(pBMVert);

				CBMData *pBMVert2	= new CBMData(&baseBMVert);
				pBMVert2->m_dwSide				= SIDE_BACK;
				pBMVert2->m_dV[Width]			= Round(toM(dLenRight),3);
				pBMVert2->m_dV[WidthLower]		= Round(toM(dLenRight),3);
				pBMVert2->m_dV[WidthUpper]		= Round(toM(dLenRight),3);
				pBMVert2->CalculateResult();
				m_pArrBMBridgeStd.Add(pBMVert2);
			}
			else
			{
				m_pArrBMBridgeStd.Add(pBMVert);
			}

			// 내측벽체 전면배면은 무조건 벽체전면으로 함
			if(pWall && Compare(pBMVert->m_dV[ArcLengthR], 0.0, "=") && Compare(pBMVert->m_dV[ArcLengthL], 0.0, "="))
			{
				CBMData *pBMVertSide	= new CBMData(&baseBMVert);
				pBMVertSide->m_nIdxMidClass			= GetFormByPos(FORM_POS_WALL_FRONT);
				pBMVertSide->m_sMidClass			= GetStringMidClass(pBMVertSide->m_nIdxMidClass);
				pBMVertSide->m_dwSide	= 0;
				if(Compare(pBMVert->m_dV[WidthDiffLeft], 0.0, "="))
					pBMVertSide->m_dwSide	|= SIDE_LEFT;
				if(Compare(pBMVert->m_dV[WidthDiffRight], 0.0, "="))
					pBMVertSide->m_dwSide	|= SIDE_RIGHT;

				if(pBMVertSide->m_dwSide != 0)
				{
					pBMVertSide->m_sInfo	= "본체측면거푸집";
					pBMVertSide->CalculateResult();
					m_pArrBMBridgeStd.Add(pBMVertSide);
				}
				else
				{
					delete pBMVertSide;
				}
				
			}

			// 리브 하면과 상면
			if(pArea->m_sInfo == "리브")
			{
				double dLenUpper	= pBri->m_tvArrVertSectionRib[iUPPER].GetLength();
				double dLenLower	= pBri->m_tvArrVertSectionRib[iLOWER].GetLength();
				CBMData *pBMHor	= new CBMData(&baseBMVert);
				pBMHor->m_nIdxMidClass		= GetFormByPos(FORM_POS_WALL_BACK);
				pBMHor->m_sMidClass			= GetStringMidClass(pBMHor->m_nIdxMidClass);
				pBMHor->m_nShape			= SHAPE_TV_RECTANGLE;
				pBMHor->m_dV[WidthUpper]	= Round(toM(dLenUpper), 3);
				pBMHor->m_dV[WidthLower]	= Round(toM(dLenUpper), 3);
				pBMHor->m_dV[Width]			= Round(toM(dLenUpper), 3);
				pBMHor->m_dV[Length]		= Round(toM(dBridgeLength),3);
				pBMHor->m_sInfo				= "";
				pBMHor->m_sUnit				= "㎡";
				pBMHor->m_dwSide			= SIDE_UPPER;
				pBMHor->m_sNoteTail			= "아치리브상면";
				pBMHor->CalculateResult();
				m_pArrBMBridgeStd.Add(pBMHor);

				CBMData *pBMHor2	= new CBMData(&baseBMVert);
				pBMHor2->m_nIdxMidClass		= GetFormByPos(FORM_POS_WALL_FRONT);
				pBMHor2->m_sMidClass		= GetStringMidClass(pBMHor2->m_nIdxMidClass);
				pBMHor2->m_nShape			= SHAPE_TV_RECTANGLE;
				pBMHor2->m_dV[WidthUpper]	= Round(toM(dLenLower), 3);
				pBMHor2->m_dV[WidthLower]	= Round(toM(dLenLower), 3);
				pBMHor2->m_dV[Width]		= Round(toM(dLenLower), 3);
				pBMHor2->m_dV[Length]		= Round(toM(dBridgeLength),3);
				pBMHor2->m_sInfo			= "";
				pBMHor2->m_sUnit			= "㎡";
				pBMHor2->m_dwSide			= SIDE_LOWER;
				pBMHor2->m_sNoteTail		= "아치리브하면";
				pBMHor2->CalculateResult();
				m_pArrBMBridgeStd.Add(pBMHor2);
			}

			// 본체 경사면
			if(pWall)
			{
				if(Compare(pBMVert->m_dV[ArcLengthR], 0.0, ">") || Compare(pBMVert->m_dV[ArcLengthL], 0.0, ">"))
				{
					CBMData *pBMArc	= new CBMData(&baseBMVert);
					pBMArc->m_nIdxMidClass		= GetFormByPos(FORM_POS_WALL_FRONT);
					pBMArc->m_sMidClass			= GetStringMidClass(pBMArc->m_nIdxMidClass);
					pBMArc->m_nIdxSmallClass	= BM_SMALL_FORM_CURVE;
					pBMArc->m_sSmallClass		= GetStringSmallClass(pBMArc->m_nIdxSmallClass);
					pBMArc->m_sUnit				= "㎡";
					pBMArc->m_dwSide			= SIDE_LEFT | SIDE_RIGHT;
					pBMArc->m_sInfo				= "본체측면거푸집";
					pBMArc->CalculateResult();
					if(pBMArc->m_sFormForCompare != _T(""))
						m_pArrBMBridgeStd.Add(pBMArc);
				}
				else if(!Compare(pBMVert->m_dV[WidthDiffLeft], 0.0, "=") || !Compare(pBMVert->m_dV[WidthDiffRight], 0.0, "="))
				{
					CBMData *pBMSlope			= new CBMData(&baseBMVert);
					pBMSlope->m_nIdxMidClass	= GetFormByPos(FORM_POS_WALL_FRONT);
					pBMSlope->m_sMidClass		= GetStringMidClass(pBMSlope->m_nIdxMidClass);
					pBMSlope->m_nIdxSmallClass	= BM_SMALL_FORM_SLOPE;
					pBMSlope->m_sSmallClass		= GetStringSmallClass(pBMSlope->m_nIdxSmallClass);
					pBMSlope->m_sUnit			= "㎡";
					pBMSlope->m_dwSide			= 0;
					
					CTwinVector tvLeft	= pArea->GetTvLeft();	// 벽체 하면은 기초와 접하므로 거푸집에서 제외한다.

					if(tvLeft.m_sInfo.Find(_T("벽체하면"))<0)
					{
						if(tvLeft.m_sInfo.Find(_T("벽체상면"))<0)	// 벽체 하면은 슬래브와 접하므로 거푸집에서 제외한다.
						{
							if(!Compare(pBMVert->m_dV[WidthDiffLeft], 0.0, "="))
								pBMSlope->m_dwSide	|= SIDE_LEFT;
						}
					}
					
					CTwinVector tvRight	= pArea->GetTvRight();
					if(tvRight.m_sInfo.Find(_T("벽체하면"))<0)
					{
						if(tvRight.m_sInfo.Find(_T("벽체상면"))<0)
						{
							if(!Compare(pBMVert->m_dV[WidthDiffRight], 0.0, "="))
								pBMSlope->m_dwSide	|= SIDE_RIGHT;
						}
					}
					
					pBMSlope->m_sInfo = "본체측면거푸집";
					pBMSlope->CalculateResult();
					if(pBMSlope->m_sFormForCompare != _T(""))
						m_pArrBMBridgeStd.Add(pBMSlope);
				}
			}

			// 라운드 벽체일때는 라운드부 거푸집도 만들어 줌.
			if(bRoundRectWall)
			{
				CBMData *pBMRound			= new CBMData(&baseBMVert);
				pBMRound->m_nIdxSmallClass	= BM_SMALL_FORM_CURVE;
				pBMRound->m_sSmallClass		= GetStringSmallClass(pBMRound->m_nIdxSmallClass);
				pBMRound->m_dV[Radius]		= Round(toM(pWall->IsRoundRectWall() ? pWall->m_dW/2 : pWall->m_Column.m_dR[iOUTTER][iUPPER]),3);
				pBMRound->m_dwSide			= SIDE_ROUND;
				pBMRound->CalculateResult();
				pBMRound->m_sInfo			= "벽체라운드부거푸집";
				m_pArrBMBridgeStd.Add(pBMRound);

				// 특히나 기둥식이라면 기둥 개수만큼 추가해주자
				if(pWall->m_bTopIsColumn)
				{
					for(long col = 1; col < pWall->m_nCountColumn; col++)
					{
						CBMData *pBMTmp1	= new CBMData(&baseBMVert);
						m_pArrBMBridgeStd.Add(pBMTmp1);

						CBMData *pBMTmp2	= new CBMData(pBMRound);
						m_pArrBMBridgeStd.Add(pBMTmp2);
					}
				}
			}

			// 여기 아래는 벽체가 아닌 것들에 대해서 구함.
			if(pWall) continue;

			// 시종점 벽체와 상부슬래브는 좌우측면 거푸집 구할때 브라켓이나 헌치부를 빼고 구해야 된다.
			// 그래서 좌우측에 대해서는 항상 다른 도형과의 겹침을 체크한다.

			// 함형라멘교 하부헌치는 경사면을 구하지 않는다.
			if((pArea->m_nTypeShape == SHAPE_TV_TRIANGLE || pArea->m_nTypeShape == SHAPE_TV_TRAPEZOID) && pArea->m_sInfo.Find("하부헌치", 0) != -1)
			{
				continue;
			}

			// 좌(경사면 또는 수직면)
			// m_sInfo가 있어야 정상적인 선이다. 없는경우 측면의 영역과 겹치는 부분이므로 구하지 않음
			CTwinVector tvLeft	= pArea->GetTvLeft();
			if(tvLeft.m_sInfo != "")
			{
				CBMData *pBMLeft	= new CBMData(&baseBMVert);

				// overlap 체크해서 좌측높이나 기본높이등의 값을 변경해줌 /////////////////
				CTwinVectorArray tvArrLeft;
				tvArrLeft.Add(tvLeft);

				BOOL bOverlap	= FALSE;
				long j = 0; for(j = 0; j < pBri->m_pArrAreaConc.GetSize(); j++)
				{
					if(j == i) continue;
					CTwinVectorAreaShape *pAreaShape	= pBri->m_pArrAreaConc.GetAt(j);
					CTwinVectorArray tvArrShape;
					tvArrShape	= pAreaShape->m_tvArr;
					if(rb.CheckOverlapLineByTvArr(tvArrLeft, tvArrShape, TRUE, TRUE))
						bOverlap	= TRUE;
				}

				double dTotHeight		= frM(pBMLeft->m_dV[Height]);
				double dTotHeightLeft	= frM(pBMLeft->m_dV[HeightLeft]);

				// 오버랩된게 없다면 빼지 않아야 됨
				if(bOverlap)
				{
					for(j = 0; j < tvArrLeft.GetSize(); j++)
					{
						dTotHeight	-= tvArrLeft.GetAt(j).GetVerLength();
						dTotHeightLeft -= tvArrLeft.GetAt(j).GetLength();
					}
				}

				BOOL bSttWall = tvLeft.m_sInfo.Find("시점", 0) != -1? TRUE : FALSE;
				if(tvLeft.m_sInfo.Find("벽체", 0) != -1)
				{
					if(bSttWall)
					{
						pBMLeft->m_nIdxMidClass	= GetFormByPos(FORM_POS_WALL_BACK);
						pBMLeft->m_sMidClass	= GetStringMidClass(pBMLeft->m_nIdxMidClass);
					}
					else
					{
						pBMLeft->m_nIdxMidClass	= GetFormByPos(FORM_POS_WALL_FRONT);
						pBMLeft->m_sMidClass	= GetStringMidClass(pBMLeft->m_nIdxMidClass);
					}
				}

				pBMLeft->m_dV[Height]		= fabs(Round(toM(dTotHeight),3));
				pBMLeft->m_sVC[Height]		= _T("");
				pBMLeft->m_dV[HeightLeft]	= fabs(Round(toM(dTotHeightLeft),3));
				pBMLeft->m_sVC[HeightLeft]	= _T("");
				//////////////////////////////////////////////////////////////////////////////
				
				if(dTotHeight > 0.0000001)	// 정확히 0이 안나온다. //좌측벽 거푸집
				{
					if(!Compare(pBMLeft->m_dV[WidthDiffLeft], 0.0, "="))
						pBMLeft->m_nIdxSmallClass	= BM_SMALL_FORM_SLOPE;
					pBMLeft->m_sSmallClass		= GetStringSmallClass(pBMLeft->m_nIdxSmallClass);
					pBMLeft->m_dwSide			= SIDE_LEFT;

					switch(nTypeLeft)
					{
					case -2: pBMLeft->m_dV[Length]	= Round(toM(dSttBridgeLengthBracket),3); break;
					case -1: pBMLeft->m_dV[Length]	= Round(toM( dSttBridgeLength),3); break;
					default: pBMLeft->m_dV[Length]	= Round(toM(dBridgeLength),3); break; //종점벽체의 좌측면도 외측과 같이 고려해줌
					}
					pBMLeft->CalculateResult();
					pBMLeft->m_sInfo = "본체시점측거푸집";
					m_pArrBMBridgeStd.Add(pBMLeft);
				}
				else
					delete pBMLeft;
			}

			// 우(경사면 또는 수직면)
			// m_sInfo가 있어야 정상적인 선이다. 없는경우 측면의 영역과 겹치는 부분이므로 구하지 않음
			CTwinVector tvRight	= pArea->GetTvRight();
			if(tvRight.m_sInfo != "")
			{
				CBMData *pBMRight	= new CBMData(&baseBMVert);
				
				// overlap 체크해서 좌측높이나 기본높이등의 값을 변경해줌 /////////////////
				CTwinVectorArray tvArrRight;
				tvArrRight.Add(tvRight);

				BOOL bOverlap	= FALSE;
				long j = 0; for(j = 0; j < pBri->m_pArrAreaConc.GetSize(); j++)
				{
					if(j == i) continue;
					CTwinVectorAreaShape *pAreaShape	= pBri->m_pArrAreaConc.GetAt(j);
					CTwinVectorArray tvArrShape;
					tvArrShape	= pAreaShape->m_tvArr;
					if(rb.CheckOverlapLineByTvArr(tvArrRight, tvArrShape, TRUE, TRUE))
						bOverlap	= TRUE;
				}

				double dTotHeight		= frM(pBMRight->m_dV[Height]);
				double dTotHeightRight	= frM(pBMRight->m_dV[HeightRight]);

				// 오버랩된게 없다면 빼지 않아야 됨
				if(bOverlap)
				{
					for(j = 0; j < tvArrRight.GetSize(); j++)
					{
						dTotHeight	-= tvArrRight.GetAt(j).GetVerLength();
						dTotHeightRight -= tvArrRight.GetAt(j).GetLength();
					}
				}

				if(tvRight.m_sInfo.Find("벽체", 0) != -1)
				{
					if(tvRight.m_sInfo.Find("시점", 0) != -1)
					{
						pBMRight->m_nIdxMidClass	= GetFormByPos(FORM_POS_WALL_FRONT);
						pBMRight->m_sMidClass		= GetStringMidClass(pBMRight->m_nIdxMidClass);
					}
					else
					{
						pBMRight->m_nIdxMidClass	= GetFormByPos(FORM_POS_WALL_BACK);
						pBMRight->m_sMidClass		= GetStringMidClass(pBMRight->m_nIdxMidClass);
					}
				}

				pBMRight->m_dV[Height]		= fabs(Round(toM(dTotHeight),3));
				pBMRight->m_sVC[Height]		= _T("");
				pBMRight->m_dV[HeightRight]	= fabs(Round(toM(dTotHeightRight),3));
				pBMRight->m_sVC[HeightRight]	= _T("");
				//////////////////////////////////////////////////////////////////////////////
				if(dTotHeight > 0.0000001)	// 정확히 0이 안나온다. 우측  거푸집
				{
					if(tvRight.GetHorLength() > 0)
					{
						pBMRight->m_nIdxSmallClass	= BM_SMALL_FORM_SLOPE;
					}
					pBMRight->m_sSmallClass		= GetStringSmallClass(pBMRight->m_nIdxSmallClass);
					pBMRight->m_dwSide			= SIDE_RIGHT;
					switch(nTypeRight)
					{
					case 2: pBMRight->m_dV[Length]	= Round(toM(dEndBridgeLengthBracket),3); break;
					case 1: pBMRight->m_dV[Length]	= Round(toM(dEndBridgeLength),3); break;
					default: pBMRight->m_dV[Length]	= Round(toM(dBridgeLength),3); break;
					}

					pBMRight->CalculateResult();
					pBMRight->m_sInfo = "본체종점측거푸집";
					m_pArrBMBridgeStd.Add(pBMRight);
				}
				else
					delete pBMRight;
			}

			// 회전된 사다리꼴의 경우 아래쪽도 구해야 된다. ///////////////////////// 경사면 거푸집, 브라켓 경사면 이곳에 있음
			if(pArea->m_nTypeShape == SHAPE_TV_TRAPEZOIDROT)
			{
				if(pArea->GetTvLower().m_sInfo != "")
				{
					long nTypeBracket = 0;
					CTwinVector tv	= pArea->GetTvLower();
					if(!Compare(tv.m_v1.y, tv.m_v2.y, "="))
					{
						CBMData *pBMHunch			= new CBMData(&baseBMVert);
						pBMHunch->m_nIdxSmallClass	= BM_SMALL_FORM_SLOPE;
						pBMHunch->m_sSmallClass		= GetStringSmallClass(pBMHunch->m_nIdxSmallClass);
						pBMHunch->m_dwSide			= SIDE_LOWER;

						if(nTypeLeft != 0 || nTypeRight != 0)
						{
							long nType	= nTypeLeft != 0 ? nTypeLeft : nTypeRight;
							nTypeBracket = nType;
							switch(nType)
							{
								case -2: pBMHunch->m_dV[Length]	= Round(toM(dSttBridgeLengthBracket),3); break;
								case -1: pBMHunch->m_dV[Length]	= Round(toM(dSttBridgeLength),3); break;
								case 2: pBMHunch->m_dV[Length]	= Round(toM(dEndBridgeLengthBracket),3); break;
								case 1: pBMHunch->m_dV[Length]	= Round(toM(dEndBridgeLength),3); break;
								default: pBMHunch->m_dV[Length]	= Round(toM(dBridgeLength),3); break;
							}
						}

						double dResult = 0;
						if(nTypeBracket == -2 || nTypeBracket == 2) //브라켓 경사면 거푸집 
						{
							CString sResult = _T("");
							double dHBracket = Round(toM(tv.GetHorLength()), 3);
							double dVBracket = Round(toM(tv.GetVerLength()), 3);
							double dLenBra	   = pBMHunch->m_dV[Length];

							dResult = sqrt((dHBracket*dHBracket) + (dVBracket*dVBracket)) * dLenBra;

							if(dResult > 0)
							{
								sResult.Format(_T("√(%.3f² x %.3f²) x %.3f"), dHBracket, dVBracket, dLenBra);
								pBMHunch->m_sForm = sResult;
								pBMHunch->m_sFormForCompare = pBMHunch->m_sForm ;
								pBMHunch->SetResult(Round(dResult, 3));
							}							
						}						
						
						if(dResult == 0)
						{
							pBMHunch->CalculateResult();
						}						
					
						pBMHunch->m_sInfo = "본체브라켓헌치부거푸집";
						m_pArrBMBridgeStd.Add(pBMHunch);
					}
				}
			}
			///////////////////////////////////////////////////////////////////////////
		}

	}

	// 수평거푸집(슬래브 하면, 헌치부제외)
	// 슬래브하면이 경우에 따라서 다를 수 있으므로 종단을 선형왼쪽과 오른쪽을 각각 구해서 길이를 구함.
	// 현재 영역에 이름이 붙어 있지 않아서 그냥 번호를 씀
	// 만약 영역에 이름을 붙일 수 있게 되면 아래 주석을 풀어야 함.
	CTwinVector tvLower;
	long nNumSlabLower	= 0;//baseBM.GetIdxAreaByName(&pBri->m_pArrAreaConc, "상부슬래브하면");
	for(i = 0; i < pBri->m_pArrAreaConc.GetSize(); i++)
	{
		CTwinVectorAreaShape *pArea	= pBri->m_pArrAreaConc.GetAt(i);
		if(pArea->m_sInfo.Find(_T("상부슬래브"))>=0)
		{
			nNumSlabLower	= pArea->m_nNum;
			break;
		}
	}

	double	dLenLeft	= 0,
			dLenRight	= 0;
	CTwinVector tv;
	CString str	= _T("");

	// 왼쪽슬래브 하면 총 길이..
	pBri->GetTvVertSection(-1, TRUE);
	long j = 0; for(j = 0; j < pBri->m_nCountInWallHunch+1; j++)
	{
		if(j == 0) str.Format("슬래브하면");
		else str.Format("%d번째내측헌치우측슬래브하면", j);

		if(pBri->m_tvArrVertSection.GetTvByInfo(str, tv))	// 경사교 일때 슬래브하면 위치 벡터가 틀림...수정 되어야함.
			dLenLeft	+= tv.GetLength();
	}
	// 오른쪽슬래브 하면 총 길이..
	pBri->GetTvVertSection(1, TRUE);
	for(j = 0; j < pBri->m_nCountInWallHunch+1; j++)
	{
		if(j == 0) str.Format("슬래브하면");
		else str.Format("%d번째내측헌치우측슬래브하면", j);

		if(pBri->m_tvArrVertSection.GetTvByInfo(str, tv))	// 경사교 일때 슬래브하면 위치 벡터가 틀림...수정 되어야함.
			dLenRight	+= tv.GetLength();
	}
	
	// 원래대로 되돌려 놓음
	pBri->GetTvVertSection(0, TRUE);

	// 수평면
	// 슬래브 하면과 겹치는 부분 공제
	CBMData *pBMHor					= new CBMData;
	pBMHor->m_nNumbering			= nNumSlabLower;
	pBMHor->m_nIdxBigClass			= BM_BIG_FORM;
	pBMHor->m_sBigClass				= GetStringBigClass(pBMHor->m_nIdxBigClass);
	pBMHor->m_nLevelTotal			= GetLevelTotal(pBMHor->m_nIdxBigClass); 
	pBMHor->m_nIdxMidClass			= GetFormByPos(FORM_POS_NORMAL);
	pBMHor->m_sMidClass				= GetStringMidClass(pBMHor->m_nIdxMidClass);
	pBMHor->m_nIdxSmallClass		= BM_SMALL_FORM_HORI;
	pBMHor->m_sSmallClass			= GetStringSmallClass(pBMHor->m_nIdxSmallClass);
	pBMHor->m_nTypeResult			= RESULT_AREA;
	pBMHor->m_sUnit					= "㎡";
	pBMHor->m_nShape				= SHAPE_TV_TRAPEZOID;
//	pBMHor->m_dV[SttMeter]			= Round(toM(dSttMeter),3);
//	pBMHor->m_dV[EndMeter]			= Round(toM(dEndMeter),3);
	pBMHor->m_dV[WidthLower]		= Round(toM(dLenLeft),3);
	pBMHor->m_dV[WidthUpper]		= Round(toM(dLenRight),3);
	pBMHor->m_dV[WidthDiffLeft]		= Round(toM(0),3);
	pBMHor->m_dV[WidthDiffRight]	= pBMHor->m_dV[WidthLower] - pBMHor->m_dV[WidthUpper];
	pBMHor->m_dV[Height]			= Round(toM(pBri->GetWidthSlabAct(pBri->m_dStationBridgeStt, CDPoint(0, 1), 0)),3);
	pBMHor->m_dV[Length]			= Round(toM(0),3);	// 사실 여기선 의미가 없는 값
	pBMHor->m_dwSide				= SIDE_FRONT;
	pBMHor->CalculateResult();
	pBMHor->m_sInfo					= "상부슬래브하면거푸집";
	m_pArrBMBridgeStd.Add(pBMHor);	

	// 중간지점 헌치 수평면(너비 : 직거리, 길이 : 사거리)
	long nCountInHunch	= pBri->m_nCountInWallHunch;
	for(long hunch	= 0; hunch < nCountInHunch; hunch++)
	{
		if(pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP) break;	// 경사교 제외

		long nIdxWall	= 0;
		BOOL bLeft		=TRUE;
		pBri->GetInfoHunch(hunch, nIdxWall, bLeft);
		CString sLeft	= _T("");
		CString sRight	= _T("");
		sLeft.Format("%d번째내측헌치", hunch+1); //%d번째내측헌치아래면좌측
		sRight.Format("%d번째내측헌치", hunch+1); //%d번째내측헌치아래면우측

		long nSize	= pBri->m_pArrAreaConc.GetSize();
		long i = 0; for(i = 0; i < nSize; i++)
			CString st = pBri->m_pArrAreaConc.GetAt(i)->m_sInfo;

		long nIdxArea	= baseBM.GetIdxAreaByName(&pBri->m_pArrAreaConc, sLeft);	// 헌치 하면 넘버링 찾아야 됨
		if(nIdxArea<0)
			nIdxArea	= baseBM.GetIdxAreaByName(&pBri->m_pArrAreaConc, sRight);	// 헌치 하면 넘버링 찾아야 됨
		long nNumHunch	= nIdxArea > -1 ? pBri->m_pArrAreaConc.GetAt(nIdxArea)->m_nNum : 0;
		double dWidthUpperOfWall	= 0;	// 내측벽체상면(공제해야 될 부분임)
		CWallApp *pWall	= pBri->GetInWall(nIdxWall);
		if(pWall)
		{
			if(pWall->m_bIs)
			{
				if(pWall->m_nType == WALL_TYPE_V)
					dWidthUpperOfWall	= bLeft ? pWall->m_dWL : pWall->m_dWR;
				else
					dWidthUpperOfWall	= pWall->m_dW;
			}
		}

		if(toM(pBri->m_dWL[hunch][iUPPER] + pBri->m_dWR[hunch][iUPPER] - dWidthUpperOfWall) > 0)
		{
			CBMData *pBMHunch				= new CBMData(pBMHor);
			pBMHunch->m_nNumbering			= nNumHunch;
			pBMHunch->m_nShape				= SHAPE_TV_RECTANGLE;
			pBMHunch->m_dV[WidthLower]		= Round(toM(pBri->m_dWL[hunch][iUPPER] + pBri->m_dWR[hunch][iUPPER] - dWidthUpperOfWall),3);
			pBMHunch->m_dV[Height]			= Round(toM(dBridgeLength),3);//toM(pBri->GetWidthSlabAct(dSta, pBri->GetAngleByStation(dSta), 0, FALSE));
			pBMHunch->CalculateResult();
			m_pArrBMBridgeStd.Add(pBMHunch);
		}
	}

	// 슬래브 연단 곡면 (일반일 경우...켄틸레버일 경우 해야됨)
	pBri->GetTvVertSection_Std(0, TRUE, FALSE);
	for(i=0; i<2; i++)	// 좌,우
	{
		if(pBri->m_nSlabChemfer[i] == 0) continue;		// 모따기 형태가 없을때

		// 연단 길이
		CTwinVector tv;
		i==0 ? pBri->GetTvVertSection(-1, TRUE) : pBri->GetTvVertSection(1, TRUE);
		
		CString str(_T(""));
		double dSlabLength = 0;
		
		if(pBri->m_nTypeBridge == BRIDGE_TYPE_SLAB)
		{
			for(long nJijum = 0; nJijum < pBri->m_nQtyJigan; nJijum++)
			{
				str.Format(_T("슬래브상면(%d번째지간)"), nJijum+1);
				pBri->m_tvArrVertSection.GetTvByInfo(str, tv);
				dSlabLength += tv.GetLength();
			}
		}
		else
		{
			str = _T("슬래브상면(외측벽체안쪽)");
			pBri->m_tvArrVertSection.GetTvByInfo(str, tv);
			dSlabLength = tv.GetLength();
		}

		CBMData *pBMCurve				= new CBMData;
		pBMCurve->m_nNumbering			= nNumSlabLower;
		pBMCurve->m_nIdxBigClass		= BM_BIG_FORM;
		pBMCurve->m_sBigClass			= GetStringBigClass(pBMCurve->m_nIdxBigClass);
		pBMCurve->m_nLevelTotal			= GetLevelTotal(pBMCurve->m_nIdxBigClass); 
		pBMCurve->m_nIdxMidClass		= GetFormByPos(FORM_POS_NORMAL);
		pBMCurve->m_sMidClass			= GetStringMidClass(pBMCurve->m_nIdxMidClass);
		pBMCurve->m_nIdxSmallClass		= pBri->m_nSlabChemfer[i] == 1 ? BM_SMALL_FORM_SLOPE : BM_SMALL_FORM_CURVE;
		pBMCurve->m_sSmallClass			= GetStringSmallClass(pBMCurve->m_nIdxSmallClass);
		pBMCurve->m_nTypeResult			= RESULT_AREA;
		pBMCurve->m_sUnit				= "㎡";
		pBMCurve->m_nShape				= SHAPE_TV_RECTANGLE;
		pBMCurve->m_dV[WidthLower]		= Round(toM(dSlabLength),3);
		pBMCurve->m_dV[WidthUpper]		= Round(toM(dSlabLength),3);
		pBMCurve->m_dV[WidthDiffLeft]	= Round(toM(0),3);
		pBMCurve->m_dV[WidthDiffRight]	= pBMCurve->m_dV[WidthLower] - pBMCurve->m_dV[WidthUpper];
		double dL = sqrt(pow(pBri->m_dTCW1[i],2)+pow(pBri->m_dTCH1[i],2));
		pBMCurve->m_dV[Height]			= pBri->m_nSlabChemfer[i] == 1 ? Round(toM(dL),3) : Round(toM(2*ConstPi*pBri->m_dRE1[i]/4),3);
		pBMCurve->m_dV[Length]			= Round(toM(0),3);	// 사실 여기선 의미가 없는 값
		pBMCurve->m_dwSide				= SIDE_FRONT;
		pBMCurve->CalculateResult();
		pBMCurve->m_sInfo				= "상부슬래브연단거푸집";
		pBMCurve->m_bSlab				= TRUE;
		m_pArrBMBridgeStd.Add(pBMCurve);	

		// 상부슬래브 좌우측 거푸집은 연단의 높이가 고려 안되어 있기 때문에 여기서 고려해준다.
		// 상부슬래브 첫번째 거푸집과 같은 제원을 가진다.
		if(pBMCurve->m_dV[Height] > 0)
		{
			CBMData *pBMSlab	= NULL;
			for(j = 0; j < m_pArrBMBridgeStd.GetSize(); j++)
			{
				CBMData *pBM	= m_pArrBMBridgeStd.GetAt(j);

				if(pBM->m_nIdxBigClass == BM_BIG_FORM && pBM->m_bSlab && nNumSlabLower == pBM->m_nNumbering)
				{
					pBMSlab	= pBM;
					break;
				}
			}
			CBMData *pBMDeduct	= new CBMData(pBMSlab);
			pBMDeduct->m_nShape	= SHAPE_TV_RECTANGLE;
			pBMDeduct->m_nIdxSmallClass		= BM_SMALL_FORM_VERTICAL;
			pBMDeduct->m_sSmallClass		= GetStringSmallClass(pBMDeduct->m_nIdxSmallClass);
			pBMDeduct->m_dV[Width]			= 0;
			pBMDeduct->m_dV[WidthLower]		= pBMCurve->m_dV[WidthLower];
			pBMDeduct->m_dV[WidthUpper]		= 0;
			pBMDeduct->m_dV[WidthDiffLeft]	= 0;
			pBMDeduct->m_dV[WidthDiffRight]	= pBMDeduct->m_dV[WidthLower] - pBMDeduct->m_dV[WidthUpper];
			pBMDeduct->m_dV[Height]			= toM(Round(pBri->m_dRE1[i], 3));
			pBMDeduct->m_dV[HeightLeft]		= 0;
			pBMDeduct->m_dV[HeightRight]	= 0;
			pBMDeduct->m_dV[Length]			= 0;
			pBMDeduct->m_dV[ArcAreaR]		= 0;
			pBMDeduct->m_dV[ArcAreaL]		= 0;
			pBMDeduct->m_dV[ArcLengthR]		= 0;
			pBMDeduct->m_dV[ArcLengthL]		= 0;
			pBMDeduct->m_bIsUseForDeduct	= TRUE;
			pBMDeduct->m_dwSide				= SIDE_FRONT;
			pBMDeduct->CalculateResult();
			pBMDeduct->m_sInfo				= "본체거푸집";
			m_pArrBMBridgeStd.Add(pBMDeduct);
		}
	}
	// 수량에 미터정보를 추가한다.
	RemakeByHeightBMList(&m_pArrBMBridgeStd, dFirstMeter, dSeconeMeter, nSizeBM-1, m_pArrBMBridgeStd.GetSize()-1, nOpt);
	pBri->GetTvVertSection_Std(0, TRUE, FALSE, TRUE);
	
	pBri->GetTvVertSection_Std(0, FALSE, FALSE);
	pBri->GetTvVertSection(0, TRUE);

	nSizeBM	= m_pArrBMBridgeStd.GetSize();
		 
	// 버림콘크리트(LEAN) 및 MASS 콘크리트
	// 일반형 : 평면영역을 기준으로 구함.
	// slope형 : 정면을 기준으로 구함.
	for(i=0; i<nCountFooting; i++)
	{
		CFootingApp *pFooting		= pBri->GetFooting(i);
		if(!pFooting) continue;
		CExFootingApp *pExFooting	= &pFooting->m_exFooting;
	
		// 버림콘크리트
		long nTypeExFooting = pBri->IsBoxType() ? pBri->m_nTypeLower : pExFooting->m_nType;
		if(nTypeExFooting != EXFOOTING_TYPE_MASS)
		{
			// 일반형
			if(pFooting->m_nType == FOOTING_TYPE_BASIC || pFooting->m_nType == FOOTING_TYPE_BASIC_BOTH || pFooting->m_nType == FOOTING_TYPE_BOX)
			{
				if(!pBri->IsBoxType())
				{
					double dHeight = pExFooting->m_dEFHR;
					nSize = pExFooting->m_pArrAreaConcPlane.GetSize();
					long j = 0; for(j = 0; j < nSize; j++)
					{
						CTwinVectorAreaShape *pArea	= pExFooting->m_pArrAreaConcPlane.GetAt(j);

						// 수직면
						CBMData *pBMVert				= new CBMData;
						pBMVert->m_nNumbering			= pArea->m_nNum;
						pBMVert->m_nIdxBigClass			= BM_BIG_FORM;
						pBMVert->m_sBigClass			= GetStringBigClass(pBMVert->m_nIdxBigClass);
						pBMVert->m_nLevelTotal			= GetLevelTotal(pBMVert->m_nIdxBigClass);
						pBMVert->m_nIdxMidClass			= GetFormByPos(FORM_POS_LEAN);
						pBMVert->m_sMidClass			= GetStringMidClass(pBMVert->m_nIdxMidClass);
						pBMVert->m_nIdxSmallClass		= BM_SMALL_FORM_VERTICAL;
						pBMVert->m_sSmallClass			= GetStringSmallClass(pBMVert->m_nIdxSmallClass);
						pBMVert->m_nTypeResult			= RESULT_AREA;
						pBMVert->m_sUnit				= "㎡";
						pBMVert->m_nShape				= pArea->m_nTypeShape;
						pBMVert->m_dV[WidthLower]		= Round(toM(pArea->m_dWidthLower),3);
						pBMVert->m_dV[WidthUpper]		= Round(toM(pArea->m_dWidthUpper),3);
						pBMVert->m_dV[WidthDiffLeft]	= Round(toM(pArea->m_dWidthDiffLeft),3);
						pBMVert->m_dV[WidthDiffRight]	= Round(toM(pArea->m_dWidthDiffRight),3);
						pBMVert->m_dV[Height]			= Round(toM(pArea->m_dHeight),3);
						pBMVert->m_dV[Length]			= Round(toM(dHeight),3);
						pBMVert->m_dwSide				= SIDE_UPPER | SIDE_LOWER | SIDE_LEFT | SIDE_RIGHT;
						pBMVert->m_sInfo				= "버림콘크리트거푸집";
						if(bDeductLeft)
						{
							pBMVert->m_dwSide &= ~SIDE_LEFT;
							AddBMStyroformBySide(pBMVert, SIDE_LEFT);
						}
						else if(bDeductRight)
						{
							pBMVert->m_dwSide &= ~SIDE_RIGHT;
							AddBMStyroformBySide(pBMVert, SIDE_RIGHT);
						}

						pBMVert->CalculateResult();
						m_pArrBMBridgeStd.Add(pBMVert);
					}			
				}
				else
				{
					double dLength = fabs(pBri->m_dWidthSlabLeft) + fabs(pBri->m_dWidthSlabRight) + pFooting->m_exFooting.m_dEFWB + pFooting->m_exFooting.m_dEFWF;;
					double dHeight = pExFooting->m_dEFHR;
					double dPRF_W = pBri->m_bExistProtectionRiseFooting ? pBri->m_dPRF_W : 0;
					double dSttLength = pBri->m_dWS + dPRF_W + pExFooting->m_dEFWL;
					double dEndLength = pBri->m_dWE + dPRF_W + pExFooting->m_dEFWR;
					double dWidth = pBri->GetLengthBridge(FALSE) + (dSttLength / pBri->GetAngleJijum(0).y) + (dEndLength / pBri->GetAngleJijum(pBri->m_nQtyJigan).y);
					//앞면, 옆면
					CTwinVectorAreaShape *pArea	= pExFooting->m_pArrAreaConcPlane.GetAt(0);
					if(!pArea) return;
						
					// 수직면
					CBMData *pBMVert				= new CBMData;
					pBMVert->m_nNumbering			= pArea->m_nNum;
					pBMVert->m_nIdxBigClass			= BM_BIG_FORM;
					pBMVert->m_sBigClass			= GetStringBigClass(pBMVert->m_nIdxBigClass);
					pBMVert->m_nLevelTotal			= GetLevelTotal(pBMVert->m_nIdxBigClass);
					pBMVert->m_nIdxMidClass			= GetFormByPos(FORM_POS_LEAN);
					pBMVert->m_sMidClass			= GetStringMidClass(pBMVert->m_nIdxMidClass);
					pBMVert->m_nIdxSmallClass		= BM_SMALL_FORM_VERTICAL;
					pBMVert->m_sSmallClass			= GetStringSmallClass(pBMVert->m_nIdxSmallClass);
					pBMVert->m_nTypeResult			= RESULT_AREA;
					pBMVert->m_sUnit				= "㎡";
					pBMVert->m_nShape				= SHAPE_RECTANGLE;
					pBMVert->m_dV[WidthLower]		= Round(toM(dWidth),3);
					pBMVert->m_dV[Height]			= Round(toM(dLength),3);
					pBMVert->m_dV[Length]			= Round(toM(dHeight),3);
					pBMVert->m_dwSide				= SIDE_UPPER | SIDE_LOWER | SIDE_LEFT | SIDE_RIGHT;
					pBMVert->m_sInfo				= "버림콘크리트거푸집";
					if(bDeductLeft)
					{
						pBMVert->m_dwSide &= ~SIDE_UPPER;
						AddBMStyroformBySide(pBMVert, SIDE_UPPER);
					}
					else if(bDeductRight)
					{
						pBMVert->m_dwSide &= ~SIDE_LOWER;
						AddBMStyroformBySide(pBMVert, SIDE_LOWER);
					}
					
					pBMVert->CalculateResult();
					m_pArrBMBridgeStd.Add(pBMVert);
					
				}
				
			}
			// slop형 
			// 사교이거나 확폭일땐 대략 곤란.. ㅡㅡ;
			else if(   pFooting->m_nType == FOOTING_TYPE_LEFTSLOPE
					|| pFooting->m_nType == FOOTING_TYPE_RIGHTSLOPE
					|| pFooting->m_nType == FOOTING_TYPE_BOTHSLOPE
					|| pFooting->m_nType == FOOTING_TYPE_BASIC_LEFTSLOPE
					|| pFooting->m_nType == FOOTING_TYPE_BASIC_RIGHTSLOPE)
			{
				nSize = pExFooting->m_pArrAreaConc.GetSize();
				long j=0; for(j=0; j<nSize; j++)
				{
					CTwinVectorAreaShape *pArea	= pExFooting->m_pArrAreaConc.GetAt(j);

					// 수직면
					CBMData *pBMVert				= new CBMData;
					pBMVert->m_nNumbering			= pArea->m_nNum;
					pBMVert->m_nIdxBigClass			= BM_BIG_FORM;
					pBMVert->m_sBigClass			= GetStringBigClass(pBMVert->m_nIdxBigClass);
					pBMVert->m_nLevelTotal			= GetLevelTotal(pBMVert->m_nIdxBigClass);
					pBMVert->m_nIdxMidClass			= GetFormByPos(FORM_POS_LEAN);
					pBMVert->m_sMidClass			= GetStringMidClass(pBMVert->m_nIdxMidClass);
					pBMVert->m_nIdxSmallClass		= BM_SMALL_FORM_VERTICAL;
					pBMVert->m_sSmallClass			= GetStringSmallClass(pBMVert->m_nIdxSmallClass);
					pBMVert->m_nTypeResult			= RESULT_AREA;
					pBMVert->m_sUnit				= "㎡";
					pBMVert->m_nShape				= pArea->m_nTypeShape;
					pBMVert->m_dV[WidthLower]		= Round(toM(pArea->m_dWidthLower),3);
					pBMVert->m_dV[WidthUpper]		= Round(toM(pArea->m_dWidthUpper),3);
					pBMVert->m_dV[WidthDiffLeft]	= Round(toM(pArea->m_dWidthDiffLeft),3);
					pBMVert->m_dV[WidthDiffRight]	= Round(toM(pArea->m_dWidthDiffRight),3);
					pBMVert->m_dV[Height]			= Round(toM(pArea->m_dHeight),3);
					pBMVert->m_dV[Length]			= Round(toM(pExFooting->GetLength()),3);

					if(pArea->m_nTypeShape == SHAPE_TV_RECTANGLE)
						pBMVert->m_dwSide	= SIDE_FRONT | SIDE_BACK | SIDE_LEFT | SIDE_RIGHT;
					else if(pArea->m_nTypeShape == SHAPE_TV_TRIANGLE || pArea->m_nTypeShape == SHAPE_TV_REVTRIANGLE || pArea->m_nTypeShape == SHAPE_TV_TRAPEZOID)
					{
						pBMVert->m_dwSide	= SIDE_FRONT | SIDE_BACK;
						if(Compare(pBMVert->m_dV[WidthDiffLeft], 0.0, "="))
							pBMVert->m_dwSide	|= SIDE_LEFT;
						if(Compare(pBMVert->m_dV[WidthDiffRight], 0.0, "="))
							pBMVert->m_dwSide	|= SIDE_RIGHT;
					}

					pBMVert->m_sInfo				= "버림콘크리트거푸집";
					if(bDeductLeft)
					{
						pBMVert->m_dwSide &= ~SIDE_FRONT;
						AddBMStyroformBySide(pBMVert, SIDE_FRONT);
					}
					else if(bDeductRight)
					{
						pBMVert->m_dwSide &= ~SIDE_BACK;
						AddBMStyroformBySide(pBMVert, SIDE_BACK);
					}

					pBMVert->CalculateResult();
					m_pArrBMBridgeStd.Add(pBMVert);

					// 경사면
					if(!Compare(pBMVert->m_dV[WidthDiffLeft], 0.0, "=") || !Compare(pBMVert->m_dV[WidthDiffRight], 0.0, "="))
					{
						CBMData *pBMSlope			= new CBMData(pBMVert);
						pBMSlope->m_nIdxSmallClass	= BM_SMALL_FORM_SLOPE;
						pBMSlope->m_sSmallClass		= GetStringSmallClass(pBMSlope->m_nIdxSmallClass);
						pBMSlope->m_dwSide			= 0;

						if(!Compare(pBMVert->m_dV[WidthDiffLeft], 0.0, "="))
							pBMSlope->m_dwSide	|= SIDE_LEFT;
						if(!Compare(pBMVert->m_dV[WidthDiffRight], 0.0, "="))
							pBMSlope->m_dwSide	|= SIDE_RIGHT;
						
						pBMSlope->m_sInfo			= "버림콘크리트거푸집";
						pBMSlope->CalculateResult();
						m_pArrBMBridgeStd.Add(pBMSlope);
					}
				}
			}
		}
		// MASS 콘크리트
		else if(nTypeExFooting == EXFOOTING_TYPE_MASS)
		{
			double dLenFooting	= pExFooting->m_bFrontDanMass ? pExFooting->GetLength() : pExFooting->GetWidth(TRUE);
			nSize = pExFooting->m_bFrontDanMass ?
				pExFooting->m_pArrAreaConc.GetSize() : pExFooting->m_pArrAreaConcSide.GetSize();
			long j=0; for(j=0; j<nSize; j++)
			{
				CTwinVectorAreaShape *pArea	= pExFooting->m_bFrontDanMass ?
					pExFooting->m_pArrAreaConc.GetAt(j) : pExFooting->m_pArrAreaConcSide.GetAt(j);

				// 수직면
				CBMData *pBMVert				= new CBMData;
				pBMVert->m_nNumbering			= pArea->m_nNum;
				pBMVert->m_nIdxBigClass			= BM_BIG_FORM;
				pBMVert->m_sBigClass			= GetStringBigClass(pBMVert->m_nIdxBigClass);
				pBMVert->m_nLevelTotal			= GetLevelTotal(pBMVert->m_nIdxBigClass);
				pBMVert->m_nIdxMidClass			= GetFormByPos(FORM_POS_MASS);
				pBMVert->m_sMidClass			= GetStringMidClass(pBMVert->m_nIdxMidClass);
				pBMVert->m_nIdxSmallClass		= BM_SMALL_FORM_VERTICAL;
				pBMVert->m_sSmallClass			= GetStringSmallClass(pBMVert->m_nIdxSmallClass);
				pBMVert->m_nTypeResult			= RESULT_AREA;
				pBMVert->m_sUnit				= "㎡";
				pBMVert->m_nShape				= pArea->m_nTypeShape;
				pBMVert->m_dV[WidthLower]		= Round(toM(pArea->m_dWidthLower),3);
				pBMVert->m_dV[WidthUpper]		= Round(toM(pArea->m_dWidthUpper),3);
				pBMVert->m_dV[WidthDiffLeft]	= Round(toM(pArea->m_dWidthDiffLeft),3);
				pBMVert->m_dV[WidthDiffRight]	= Round(toM(pArea->m_dWidthDiffRight),3);
				pBMVert->m_dV[Height]			= Round(toM(pArea->m_dHeight),3);
				pBMVert->m_dV[Length]			= Round(toM(dLenFooting),3);

				if(pArea->m_nTypeShape == SHAPE_TV_RECTANGLE)
					pBMVert->m_dwSide	= SIDE_FRONT | SIDE_BACK | SIDE_LEFT | SIDE_RIGHT;
				else if(pArea->m_nTypeShape == SHAPE_TV_TRIANGLE || pArea->m_nTypeShape == SHAPE_TV_REVTRIANGLE || pArea->m_nTypeShape == SHAPE_TV_TRAPEZOID)
				{
					pBMVert->m_dwSide	= SIDE_FRONT | SIDE_BACK;
					if(Compare(pBMVert->m_dV[WidthDiffLeft], 0.0, "="))
						pBMVert->m_dwSide	|= SIDE_LEFT;
					if(Compare(pBMVert->m_dV[WidthDiffRight], 0.0, "="))
						pBMVert->m_dwSide	|= SIDE_RIGHT;
				}
				
				pBMVert->m_sInfo				= "MASS콘크리트거푸집";
				if(bDeductLeft)
				{
					pBMVert->m_dwSide &= ~SIDE_FRONT;
					AddBMStyroformBySide(pBMVert, SIDE_FRONT);
				}
				else if(bDeductRight)
				{
					pBMVert->m_dwSide &= ~SIDE_BACK;
					AddBMStyroformBySide(pBMVert, SIDE_BACK);
				}
				
				pBMVert->CalculateResult();
				m_pArrBMBridgeStd.Add(pBMVert);

				// 경사면
				if(!Compare(pBMVert->m_dV[WidthDiffLeft], 0.0, "=") || !Compare(pBMVert->m_dV[WidthDiffRight], 0.0, "="))
				{
					CBMData *pBMSlope			= new CBMData(pBMVert);
					pBMSlope->m_nIdxSmallClass	= BM_SMALL_FORM_SLOPE;
					pBMSlope->m_sSmallClass		= GetStringSmallClass(pBMSlope->m_nIdxSmallClass);
					pBMSlope->m_dwSide			= 0;

					if(!Compare(pBMVert->m_dV[WidthDiffLeft], 0.0, "="))
						pBMSlope->m_dwSide	|= SIDE_LEFT;
					if(!Compare(pBMVert->m_dV[WidthDiffRight], 0.0, "="))
						pBMSlope->m_dwSide	|= SIDE_RIGHT;
					
					pBMSlope->m_sInfo			= "버림콘크리트거푸집";
					pBMSlope->CalculateResult();
					m_pArrBMBridgeStd.Add(pBMSlope);
				}
			}
		}
	}

	// 수량에 미터정보를 추가한다.
	RemakeByHeightBMList(&m_pArrBMBridgeStd, dFirstMeter, dSeconeMeter, nSizeBM, m_pArrBMBridgeStd.GetSize()-1);

	// 마지막으로 수량이 0이면 제거한다
	for(i = 0; i < m_pArrBMBridgeStd.GetSize(); i++)
	{
		CBMData *pBM	= m_pArrBMBridgeStd.GetAt(i);

		if(pBM->GetResult() == 0)
		{
			delete pBM;
			m_pArrBMBridgeStd.RemoveAt(i);
			i--;
		}
	}
	pBri->GetTvVertSection_Std(0, FALSE, FALSE, TRUE);
}

// 본체 산출근거 첫번째 페이지에 들어가는 삽도..
void COutBM::DrawFirstPageMain(CHgBaseBMStd *pBaseBMStd, long nKindBM)
{
	if(!pBaseBMStd) return;

	CRcBridgeRebar *pBri	= GetRcBridgeRebar();
	if(!pBri) return;

	CTwinVectorArray tvArrVertSectionOld;
	tvArrVertSectionOld = pBri->m_tvArrVertSection;
	pBri->GetTvVertSection_Std(0, FALSE, FALSE);

	CARcBridgeDBStandard DBStd(pBri, m_pStd->m_pARoadOptionStd, m_pStd->m_pARcBridgeDataStd);
	DBStd.m_bApplyDimSettingOfBaseDom	= TRUE;
	CDrawVertSection *pDrawVertSection	= DBStd.m_pDrawVertSection;
	CDrawExFooting *pDrawExFooting		= DBStd.m_pDrawExFooting;

	// 실제로 출력되었으면 하는 사이즈
	double dWidthOrg = 600;	// 도면 가로크기

	// 종단면도의 스케일을 결정하자!!! ////////////////////////////////////
	// 그걸로 종단면과 관련된 도면을 그려내자.
	CDomyun domTmp;
	domTmp.SetCWnd(m_pStd->m_pView);
	domTmp.SetScaleDim(1);
	domTmp.SetDimTxt(1);
	pDrawVertSection->DimVertSectionAll(&domTmp, TRUE);

	// 원지반선
	if(m_pStd->m_bDrawElEarth)
		pDrawVertSection->DrawElEarthLine(&domTmp,0,0,FALSE,TRUE);
	/////////////////////////////////////////////////////////////////////////
	
	// 종단면도  ////////////////////////////////////////////////////////////////////////////
	CDomyun domVertSection;
	domVertSection.SetCWnd(m_pStd->m_pView);

	// 도면 설정 ////////////////////
	double dScale	= pBaseBMStd->GetScale(&domTmp, dWidthOrg);
	pBaseBMStd->SetDomyunValue(&domVertSection, dWidthOrg, &domTmp);
	double dScaleDim = toM(pBri->m_dLengthBridge)/35 * 100; //double dScaleDim = domVertSection.GetScaleDim();	// 30M교량을 기준으로 1:100, 최소 100;
	domVertSection.SetScaleDim(dScaleDim);
	///////////////////////////////////

	CTwinVectorArray tvArrTmp;
	CalAreaConc(pBri, FALSE);

	// 기초들
	long nCountFooting	= pBri->GetCountFooting();
	long i = 0; for(i = 0; i < nCountFooting; i++)
	{
		CFootingApp *pFooting	= pBri->GetFooting(i);
		if(pFooting)
		{
			pBaseBMStd->DrawAreaSectionNumber(&domVertSection, &pFooting->m_pArrAreaConc, 1);
			pBaseBMStd->DrawAreaSectionNumber(&domVertSection, &pFooting->m_exFooting.m_pArrAreaConc, 1);
			// 말뚝 표현 위해
			pDrawExFooting->DrawFront(&domVertSection, &pBri->m_footingStt.m_exFooting, FALSE, TRUE, &tvArrTmp, TRUE, TRUE, FALSE);
			pDrawExFooting->DrawFront(&domVertSection, &pBri->m_footingEnd.m_exFooting, FALSE, TRUE, &tvArrTmp, TRUE, TRUE, FALSE);
		}
	}
	
	// 본체
	pBaseBMStd->DrawAreaSectionNumber(&domVertSection, &pBri->m_pArrAreaConc, 1);
	DBStd.DrawTvArray(&domVertSection, pBri->m_tvArrVertSectionRib[0]);	// 아치교 리브
	DBStd.DrawTvArray(&domVertSection, pBri->m_tvArrVertSectionRib[1]);
	
	// 벽체들
	long nCountInWall	= pBri->GetCountInWall();
	for(i = 0; i < nCountInWall; i++)
	{
		CWallApp *pWall	= pBri->GetInWall(i);
		if(pWall)
			pBaseBMStd->DrawAreaSectionNumber(&domVertSection, &pWall->m_pArrAreaConc, 1);
		// 말뚝 표현 위해
		pDrawExFooting->DrawFront(&domVertSection, &pWall->m_footing.m_exFooting, FALSE, TRUE, &tvArrTmp, TRUE, TRUE, FALSE);
	}

	// 치수기입 
	pDrawVertSection->DimVertSectionAll(&domVertSection, TRUE);
	DimTextArchRibConcAndForm(&domVertSection);

	// 벽체 높이 산정식 표기
	DimTextFormOfSlabELCalc(&domVertSection);


	// 원지반선
	//if(m_pStd->m_bDrawElEarth)
	//	pDrawVertSection->DrawElEarthLine(&domVertSection,0,0,FALSE,TRUE);

	// 엑셀에 삽입
	domVertSection.RedrawByScale(dScale, TRUE);
	domVertSection.SetCalcExtRect();
	double dWidth = dWidthOrg;//domVertSection.GetExtRect().Width();
	double dHeight = 350;//domVertSection.GetExtRect().Height();
	CDoubleArray dArrTmp;
	long nRow	= nKindBM==0 ? pBaseBMStd->InsertDomyun(5, TA_CENTER, &domVertSection, dArrTmp, dWidth, dHeight) : 0;

	// 평면도 그리기
	CDomyun domTmp2;
	domTmp2.SetScaleDim(1);
	domTmp2.SetDimTxt(1);
	DBStd.m_pDrawPlane->DrawPlaneScaffold(&domTmp2);

	// 공통수량 분리 출력인 경우에만 출력함

	CDomyun domPlane;
	domPlane.SetCWnd(m_pStd->m_pView);
	dScale	= pBaseBMStd->GetScale(&domTmp2, dWidthOrg);
	pBaseBMStd->SetDomyunValue(&domPlane, dWidthOrg, &domTmp2);
	dScaleDim = toM(pBri->m_dLengthBridge)/35*100; //domPlane.GetScaleDim() * 4;	// 30M교량을 기준으로 1:600, 최소 600;
	domPlane.SetScaleDim(dScaleDim);

	// 공통수량분리에서 일반수량일 경우에만 출력함
	if(m_pStd->m_pARcBridgeDataStd->m_nTypeBMApply == 1 && nKindBM == 0)
	{
		int nSeparBri = pBri->m_nSeparBri;
		for(int nSepar=-1; nSepar<2; nSepar++)
		{
			if(nSeparBri==0 && nSepar!=0) continue;
			if(nSeparBri==1 && nSepar==-1) continue;
			if(nSeparBri==2 && nSepar==1) continue;

			if(pBri->m_nTypeBridge!=BRIDGE_TYPE_SLAB)
			{
				//pBri->SyncBridge();

				// 교량 평면
				DBStd.m_pDrawPlane->DrawPlaneCenterLineEachSlab(&domPlane);
				DBStd.m_pDrawPlane->DrawPlaneSlabOnlyOutLine(&domPlane);
				
				// 벽체 및 기둥 평면
				int nCountInWall = pBri->GetCountInWall();
				int  i = 0;
				for(i=0; i<nCountInWall; i++)
				{
					CWallApp *pWall = pBri->GetInWall(i);
					if(pWall!=NULL)
					{
						DBStd.m_pDrawWall->DrawPlan(&domPlane, pWall, FALSE);
						if(pWall->m_bTopIsColumn)
							DBStd.m_pDrawColumn->DrawPlan(&domPlane, &pWall->m_Column);
					}
				}
				
				// 기초 평면
				if(pBri->IsBoxType())
				{
					// BOX형은 기초없이 확대기초만 있으니깐 여기서 확대기초만 그려주자.
					DBStd.m_pDrawExFooting->DrawPlan(&domPlane, &pBri->m_footingBox.m_exFooting);
				}
				else
				{
					for(i=0; i<pBri->GetCountJijum(); i++)
					{
						CFootingApp* pFooting = pBri->GetFootingByJijumNumber(i);
						if(pFooting==NULL)
							continue;

						pFooting->GetTvPlan();
						DBStd.m_pDrawFooting->DrawPlan(&domPlane, pFooting);
						pFooting->m_exFooting.GetTvPlan();
						DBStd.m_pDrawExFooting->DrawPlan(&domPlane, &pFooting->m_exFooting);

						if(nSepar==0)
						{
							DBStd.m_pDrawFooting->DimPlaneInput(&domPlane, i, FALSE);
							//DBStd.m_pDrawFooting->DimPlaneAngle(&domPlane, i);
						}
					}
				}
				
				

				// 날개벽 평면
				pBri->GetTvWingWallPlane(TRUE);
				DBStd.m_pDrawWingWall->DrawGenPlane(&domPlane, TRUE);
				if(nSepar==0)
				{
					//DBStd.m_pDrawWingWall->DimGenAngle(&domPlane, TRUE);
				}

				pBri->GetTvWingWallPlane(FALSE);
				DBStd.m_pDrawWingWall->DrawGenPlane(&domPlane, FALSE);
				if(nSepar==0)
				{
					//DBStd.m_pDrawWingWall->DimGenAngle(&domPlane, FALSE);
				}
			}
		}

		// 그렸다면 원하는 크기로 사이즈 조정
		domPlane.RedrawByScale(dScale, TRUE);
		DBStd.RotateDomByBridgeSttEndXy(&domPlane);
		domPlane.SetCalcExtRect();

		// 엑셀에 삽입
		//double dRate		= domPlane.GetExtRect().Width() / dWidthOrg;
		double dWidthRate	= dWidthOrg;//domPlane.GetExtRect().Width() / dRate;
		double dHeightRate	= 350;//domPlane.GetExtRect().Height() / dRate;
		CDoubleArray dArrTmp;
		nRow = pBaseBMStd->InsertDomyun(nRow+3, TA_CENTER, &domPlane, dArrTmp, dWidthRate, dHeightRate);
	}
	
	// 비계평면도 그리기 /////////////////////////////////////////////////////////
	// 일반수량이거나, 공통수량포함이거나, 공통수량 분리에서 공통수량일때만 출력함
	if(m_pStd->m_pARcBridgeDataStd->m_nTypeBMApply == 0 || m_pStd->m_pARcBridgeDataStd->m_nTypeBMApply == 2 ||
		(m_pStd->m_pARcBridgeDataStd->m_nTypeBMApply == 1 && nKindBM == 1))
	{
		CDomyun domPlane2;
		domPlane2.SetCWnd(m_pStd->m_pView);

		// 도면 설정 ////////////////////
		pBaseBMStd->SetDomyunValue(&domPlane2, dWidthOrg, &domTmp);
		///////////////////////////////////

		// 비계평면도 그리기 -> 공통 수량일 경우
		DBStd.m_pDrawPlane->DrawPlaneScaffold(&domPlane2);

		// 그렸다면 원하는 크기로 사이즈 조정
		domPlane2.RedrawByScale(dScale, TRUE);
		DBStd.RotateDomByBridgeSttEndXy(&domPlane2);
		domPlane2.SetCalcExtRect();

		// 엑셀에 삽입
		double dRate	= domPlane2.GetExtRect().Width() / dWidthOrg;
		double dWidthRate	= domPlane2.GetExtRect().Width() / dRate;
		double dHeightRate	= domPlane2.GetExtRect().Height() / dRate;	
		CDoubleArray dArrTmp;
		nRow = pBaseBMStd->InsertDomyun(nRow+5, TA_CENTER, &domPlane2, dArrTmp, dWidthRate, dHeightRate);
	}

	// 공통수량 전용이면 첫장에 동바리도 출력한다.
	if(nKindBM != 0)
	{
		// 동바리는 직거리로 그림
		//pBri->SyncBridge(0, FALSE, TRUE, FALSE);
		pBri->GetTvVertSection_Std(0, FALSE, FALSE);
		pBri->SyncBridgeAps(0, FALSE);

		// 도면 설정
		CDomyun domPost;
		domPost.SetCWnd(m_pStd->m_pView);
		// 도면 설정 ////////////////////
		pBaseBMStd->SetDomyunValue(&domPost, dWidthOrg, &domTmp);
		///////////////////////////////////
			

		// 종단면
		pDrawVertSection->DrawVertSectionAll(&domPost, TRUE, TRUE, TRUE, FALSE, TRUE);
		if(m_pStd->m_bDrawElEarth)
			pDrawVertSection->DrawElEarthLine(&domPost,0,0,FALSE,TRUE);

		// 시종점 브라켓 동바리
		CHgBaseBMStd baseBMStd;
		long nNum	= 1;
		long stt = 0; for(stt = 0; stt < 2; stt++)
		{
			BOOL bStt = stt == iSTT ? TRUE : FALSE;
			nNum = GetTvAreaShapePostBracket(&m_pArrAreaConc, bStt, nNum);
			baseBMStd.DrawAreaSectionNumber(&domPost, &m_pArrAreaConc, 1, TRUE, 500, bStt ? 45 : 315);
		}

		// 각 지간별 동바리
		for(long jigan = 0; jigan < pBri->m_nQtyJigan; jigan++)
		{
			nNum = GetTvAreaShapePostJigan(&m_pArrAreaConc, jigan, nNum);
			baseBMStd.DrawAreaSectionNumber(&domPost, &m_pArrAreaConc, 1, TRUE, 500, 45);
		}

		// 치수 기입
		DBStd.m_pDrawVertSection->DimPostForBM(&domPost);

		// 면적출력
		DimTextPostAreaOfArch(&domPost);

		// 삽도를 WidthOrg 사이즈로 변경
		domPost.RedrawByScale(dScale);
		domPost.SetCalcExtRect();

		// 엑셀에 삽입
		double dRate	= domPost.GetExtRect().Width() / dWidthOrg;
		double dWidthRate	= domPost.GetExtRect().Width() / dRate;
		double dHeightRate	= domPost.GetExtRect().Height() / dRate;	
		CDoubleArray dArrTmp;
		nRow = nKindBM==0 ? 0 : pBaseBMStd->InsertDomyun(nRow+5, TA_CENTER, &domPost, dArrTmp, dWidthRate, dHeightRate);

		pBri->GetTvVertSection_Std(0, FALSE, FALSE, TRUE);
	}



	DBStd.m_bApplyDimSettingOfBaseDom = FALSE;

	pBri->m_tvArrVertSection = tvArrVertSectionOld;
}

// 방호벽 산출근거 첫번째 페이지에 들어가는 삽도..
long COutBM::DrawFirstPageGuardWall(CHgBaseBMStd *pBaseBMStd, long nHDan, long nIdx)
{
	if(!pBaseBMStd) return 5;

	CRcBridgeRebar *pBriOrg = GetRcBridgeRebar(TRUE);		// 방호벽은 원래 교량으로 작업함.
	if(!pBriOrg) return 5;

	CARcBridgeDBStandard DBStd(pBriOrg, m_pStd->m_pARoadOptionStd, m_pStd->m_pARcBridgeDataStd);
	DBStd.m_bApplyDimSettingOfBaseDom	= TRUE;
	CDrawCrossSection *pDrawCrossSection	= DBStd.m_pDrawCrossSection;
	CGuardWallRC* pGW = pBriOrg->GetGuardWallByHDan(nHDan);
	if(!pGW) return 5;
	BOOL bLeft = pGW->IsLeftGuardFence();
	// 실제로 출력되었으면 하는 사이즈
	double dWidthOrg = 100;	// 도면 가로크기

	// 종단면도의 스케일을 결정하자!!! ////////////////////////////////////
	// 그걸로 종단면과 관련된 도면을 그려내자.
	CDomyun domTmp;
	domTmp.SetCWnd(m_pStd->m_pView);
	domTmp.SetScaleDim(1);
	domTmp.SetDimTxt(1);

	CDPoint vAng(0,1);
	double sta = pBriOrg->GetStationOnJijum(0,0);
	if(pBriOrg->m_nTypeBridge!=BRIDGE_TYPE_PORTAL)
		sta += pBriOrg->GetJijum(0)->m_dLengthUgan[1];
	if(pBriOrg->m_bIsExp)
	{
		sta = pBriOrg->GetStationBridgeMid();
	}
	pDrawCrossSection->DimGuardWall(&domTmp, pGW, sta, FALSE, bLeft, !bLeft);
	DimTextLengthOfGuardWall(&domTmp, pGW, nIdx, nHDan);
	/////////////////////////////////////////////////////////////////////////
	
	// 종단면도
	CDomyun domCrossSection;
	domCrossSection.SetCWnd(m_pStd->m_pView);

	// 도면 설정
	double dScale	= pBaseBMStd->GetScale(&domTmp, dWidthOrg);
	pBaseBMStd->SetDomyunValue(&domCrossSection, dWidthOrg, &domTmp);
			
	

	// 방호벽	
	CalAreaConc(pBriOrg, FALSE);
	pBaseBMStd->DrawAreaSectionNumber(&domCrossSection, &pGW->m_pArrAreaConc, 1);

	// Dim
	
	if(_T("보도") == pBriOrg->GetGuardWallName(pGW))
	{
		if(pGW->m_dSlopeUpper > 0)		bLeft = TRUE;
		else							bLeft = FALSE;
	}
	DBStd.m_pDrawCrossSection->DimGuardWall(&domCrossSection, pGW, sta, FALSE, bLeft, !bLeft);
	// 길이 표시
	DimTextLengthOfGuardWall(&domCrossSection, pGW, nIdx, nHDan);

	// 엑셀에 삽입
	domCrossSection.RedrawByScale(dScale, TRUE);
	domCrossSection.SetCalcExtRect();

	double dWidth = dWidthOrg*2;//domCrossSection.GetExtRect().Width();
	double dHeight = dWidth;//domCrossSection.GetExtRect().Height();
	CDoubleArray dArrTmp;
	long nRow = pBaseBMStd->InsertDomyun(5, TA_CENTER, &domCrossSection, dArrTmp, dWidth, dHeight);

	// 평면도 그리기 /////////////////////////////////////////////////////////
	/* 평면도 방호벽그리기 제대로 지원되면 풀 예정.
	CDomyun domPlane;
	domPlane.SetCWnd(m_pStd->m_pView);
	dWidthOrg = 550;

	// 도면 설정 ////////////////////
	pBaseBMStd->SetDomyunValue(&domPlane, dWidthOrg, &domTmp);
	///////////////////////////////////

	// 평면도 그리기
	if(pBriOrg->m_nTypeBridge!=BRIDGE_TYPE_SLAB)
	{
		CString str = pBriOrg->GetGuardWallName(pGW);
		str += _T(" 길이");
		// 교량 평면
		DBStd.m_pDrawPlane->DrawPlaneCenterLineEachSlab(&domPlane);
		DBStd.m_pDrawPlane->DrawPlaneSlabOnlyOutLine(&domPlane, TRUE, str);

		// 날개벽 평면
		pBriOrg->GetTvWingWallPlane(TRUE);
		DBStd.m_pDrawWingWall->DrawGenPlane(&domPlane, TRUE);

		pBriOrg->GetTvWingWallPlane(FALSE);
		DBStd.m_pDrawWingWall->DrawGenPlane(&domPlane, FALSE);
	}

	// 그렸다면 원하는 크기로 사이즈 조정
	domPlane.RedrawByScale(dScale, TRUE);
	DBStd.RotateDomByBridgeSttEndXy(&domPlane);
	domPlane.SetCalcExtRect();

	// 엑셀에 삽입
	double dRate	= domPlane.GetExtRect().Width() / dWidthOrg;
	nRow	= pBaseBMStd->InsertDomyun(nRow+3, TA_CENTER, &domPlane, domPlane.GetExtRect().Width()/dRate, domPlane.GetExtRect().Height()/dRate);
	*/

	DBStd.m_bApplyDimSettingOfBaseDom	= FALSE;


	return nRow;
}

// 총계 내는 레벨 리턴
long COutBM::GetLevelTotal(long nIdxBigClass)
{
	CRcBridgeRebar	*pBri	= GetRcBridgeRebar();
	if(!pBri) return 0;
	long nTypeBM	= m_pStd->m_pARcBridgeDataStd->m_nTypeBMApply;

	if(nIdxBigClass == BM_BIG_BOHOBLOCK)				return 0;		// 보호블럭
	if(nIdxBigClass == BM_BIG_CONCRETE)					return nTypeBM == TYPE_BM_NORMAL ? 4 : 1;		// 콘크리트
	if(nIdxBigClass == BM_BIG_FORM)						return 4;		// 거푸집
	if(nIdxBigClass == BM_BIG_POST)						return 2;		// 동바리
	if(nIdxBigClass == BM_BIG_DOWELBAR)				return 2;		// DowelBar
	if(nIdxBigClass == BM_BIG_SCAFFOLD)					return 0;		// 비계
	if(nIdxBigClass == BM_BIG_SPACER)						return 2;		// 스페이셔
	if(nIdxBigClass == BM_BIG_SLABCARE)					return 0;		// 슬래브양생
	if(nIdxBigClass == BM_BIG_CONCCARE)					return 0;		// 콘크리트양생
	if(nIdxBigClass == BM_BIG_ASPHALTPROOF)			return 0;		// 아스팔트 방수
	if(nIdxBigClass == BM_BIG_REBAR)						return 6;		// 철근가공조립
	if(nIdxBigClass == BM_BIG_SURFACE)					return 0;		// 면고르기
	if(nIdxBigClass == BM_BIG_ASCON)						return 0;		// 아스콘 포장
	if(nIdxBigClass == BM_BIG_SAND)						return 0;		//모래 포장
	if(nIdxBigClass == BM_BIG_PERMEABLE)				return 0;		// 투스콘 포장
	if(nIdxBigClass == BM_BIG_TACKCOTTING)				return 0;		// 택 코팅
	if(nIdxBigClass == BM_BIG_SURFACEPROOF)			return 0;		// 교면방수
	if(nIdxBigClass == BM_BIG_ANTIDROPPING)			return 0;		// 낙하물 방지
	if(nIdxBigClass == BM_BIG_NAMEPANEL)				return 0;		// 교명판
	if(nIdxBigClass == BM_BIG_DESPANEL)					return 0;		// 설명판
	if(nIdxBigClass == BM_BIG_TBM)							return 0;		// TBM설치
	if(nIdxBigClass == BM_BIG_STYROFOAM)				return 2;		// 스티로폼
	if(nIdxBigClass == BM_BIG_FLEXIBLEJOINT)				return 2;		// 신축이음
	if(nIdxBigClass == BM_BIG_FLEXIBLEJOINT_COVER)	return 2;		// 신축이음
	if(nIdxBigClass == BM_BIG_SHRINKAGE)				return 1;		// 무수축 콘크리트
	if(nIdxBigClass == BM_BIG_COVERPLATE)				return 0;		// cover plate
	if(nIdxBigClass == BM_BIG_MONUMENT)				return 0;		// 교명주
	if(nIdxBigClass == BM_BIG_NOTCH)						return 0;		// NOTCH
	if(nIdxBigClass == BM_BIG_SCAFFOLD_LEG)			return 0;		// 비계다리 
	if(nIdxBigClass == BM_BIG_DECKFINISH)				return 2;		// 데크피니셔

	return 0;
}

CString COutBM::GetStringItemClass(long nIdxItemClass)
{
	CRcBridgeRebar	*pBri	= GetRcBridgeRebar();
	if(!pBri) return _T("");
	long nTypeBM	= m_pStd->m_pARcBridgeDataStd->m_nTypeBMApply;

	if(nIdxItemClass == BM_ITEM_BOHOBLOCK_SF)			return "시점측 벽체 전면";	// 보호블럭(시점전면)
	if(nIdxItemClass == BM_ITEM_BOHOBLOCK_SS)			return "시점측 벽체 측면";	// 보호블럭(시점측면)
	if(nIdxItemClass == BM_ITEM_BOHOBLOCK_EF)			return "종점측 벽체 전면";	// 보호블럭(종점전면)
	if(nIdxItemClass == BM_ITEM_BOHOBLOCK_ES)			return "종점측 벽체 측면";	// 보호블럭(종점측면)
	if(nIdxItemClass == BM_ITEM_CONCRETE_FOOTING)		return nTypeBM == TYPE_BM_NORMAL ? "기초" : "";	// 콘크리트 (기초)
	if(nIdxItemClass == BM_ITEM_CONCRETE_MAIN)			return nTypeBM == TYPE_BM_NORMAL ? "본체" : "";	// 콘크리트 (본체)
	if(nIdxItemClass == BM_ITEM_CONCRETE_WING_SL)		return nTypeBM == TYPE_BM_NORMAL ? "시점좌측" : "시점좌측";	// 콘크리트 (시점좌측)
	if(nIdxItemClass == BM_ITEM_CONCRETE_WING_SR)		return nTypeBM == TYPE_BM_NORMAL ? "시점우측" : "시점우측";	// 콘크리트 (시점우측)
	if(nIdxItemClass == BM_ITEM_CONCRETE_WING_EL)		return nTypeBM == TYPE_BM_NORMAL ? "종점좌측" : "종점좌측";	// 콘크리트 (종점좌측)
	if(nIdxItemClass == BM_ITEM_CONCRETE_WING_ER)		return nTypeBM == TYPE_BM_NORMAL ? "종점우측" : "종점우측";	// 콘크리트 (종점우측)
	if(nIdxItemClass == BM_ITEM_CONCRETE_APS)			return "접속슬래브";
	if(nIdxItemClass == BM_ITEM_FORM_WING_SLF)			return "시점좌측(전면)";	// 거푸집 (시점좌측 전면)
	if(nIdxItemClass == BM_ITEM_FORM_WING_SLB)			return "시점좌측(배면)";	// 거푸집 (시점좌측 배면)
	if(nIdxItemClass == BM_ITEM_FORM_WING_SRF)			return "시점우측(전면)";	// 거푸집 (시점우측 전면)
	if(nIdxItemClass == BM_ITEM_FORM_WING_SRB)			return "시점우측(배면)";	// 거푸집 (시점우측 배면)
	if(nIdxItemClass == BM_ITEM_FORM_WING_ELF)			return "종점좌측(전면)";	// 거푸집 (종점좌측 전면)
	if(nIdxItemClass == BM_ITEM_FORM_WING_ELB)			return "종점좌측(배면)";	// 거푸집 (종점좌측 배면)
	if(nIdxItemClass == BM_ITEM_FORM_WING_ERF)			return "종점우측(전면)";	// 거푸집 (종점우측 전면)
	if(nIdxItemClass == BM_ITEM_FORM_WING_ERB)			return "종점우측(배면)";	// 거푸집 (종점우측 배면)

	return _T("");
}

// 산출근거중 수식 중간에 들어가야 되는 삽도를 추가시킨다.
void COutBM::SetInsertDomyunStd()
{
	SetInsertDomyunPlaneFooting();
	SetInsertDomyunCross();
	SetInsertDomyunPost();
	SetInsertDomyunScaffoldMain();
}

// 동바리 영역 구하기.
// 브라켓 하면(시종점)
// 각 지간은 기본영역에서 상면 offset한 영역과 좌우측 offset한 영역과, offset한 영역들을 뺀 영역을 구분해야 됨.
long COutBM::GetTvAreaShapePostBracket(CTypedPtrArray <CObArray, CTwinVectorAreaShape*> *pArrAreaConc, BOOL bStt, long nBaseNum)
{
	AhTPADelete(pArrAreaConc,	(CTwinVectorAreaShape*)0);

	CRcBridgeRebar	*pBri	= GetRcBridgeRebar();
	if(!pBri) return nBaseNum;
	
	long nNum	= nBaseNum;
	CTwinVectorArray tvArrBracket;
	pBri->GetTvPostBracket(tvArrBracket, bStt);
	// 회전된 사다리꼴을 지원하므로 영역나누기 하지 않고 그냥 넣음
	if(tvArrBracket.GetSize() > 2)
	{
		CTwinVectorAreaShape *pArea	= new CTwinVectorAreaShape; 
		pArea->m_tvArr	= tvArrBracket;
		pArea->m_nNum	= nNum++;
		pArea->Calculate();
		pArrAreaConc->Add(pArea);
	}

	return nNum;
}

long COutBM::GetTvAreaShapePostJigan(CTypedPtrArray <CObArray, CTwinVectorAreaShape*> *pArrAreaConc, long nJigan, long nBaseNum)
{
	AhTPADelete(pArrAreaConc, (CTwinVectorAreaShape*)0);

	CRcBridgeRebar	*pBri = GetRcBridgeRebar();
	if(!pBri) return nBaseNum;

	long nNum = nBaseNum;
	CTwinVectorArray tvArrUpper, tvArrLeft, tvArrRight, tvArrMid, tvArrArch, tvArrMid2;
	BOOL bOK = pBri->GetTvPostJigan(tvArrUpper, tvArrLeft, tvArrRight, tvArrMid, tvArrArch, tvArrMid2, nJigan);

	// 육교용으로 설정했지만, 육교용 동바리를 설치할 수 없는 교량인 경우 일반용 동바리로 수량을 산출한다.
	if(pBri->m_nTypePost == 0 && bOK)
	{
		//AfxMessageBox(_T("육교용 동바리를 설치할 수 없는 교량이므로, 일반용 동바리로 수량을 산출합니다."));
	}

	// 총 5영역이 있을 수 있다.
	long i = 0; for(i = 0; i < 5; i++)
	{
		CTwinVectorArray tvArr;
		CString sInfo	= _T("");
		switch(i)
		{
			case 0: 
				tvArr = tvArrUpper;
				sInfo = _T("상부");
				break;
			case 1:
				tvArr = tvArrLeft;
				sInfo = _T("좌측");
				break;
			case 2:
				tvArr = tvArrRight;
				sInfo = _T("우측");
				break;
			case 3: 
				tvArr = tvArrMid;
				sInfo = bOK ? _T("") : _T("기본");		// bOK == TRUE : 일반용
				break;
			case 4:
				tvArr = tvArrArch;
				sInfo = bOK ? _T("헌치R일반") : _T("헌치R육교");
				break;
			default:
				break;
		}

		if(tvArr.GetSize()>0)
		{
			CRebarPlacing rb;
			CTwinVectorArray tvArrResult;
			CDoubleArray dArrAngleSu;
			
			if(i<4)
			{
				if(pBri->m_nTypeBridge != BRIDGE_TYPE_ARCH)
				{
					long nIdx	= 0;
					rb.SeperatorTwinVectorArea(tvArr, tvArrResult, dArrAngleSu, 0.0);
					long i = 0; for(i = 0; i < dArrAngleSu.GetSize(); i++)
					{
						CTwinVectorAreaShape *pArea	= new CTwinVectorAreaShape; 
						long nSu = (long)dArrAngleSu.GetAt(i);
						long j=0; for(j=0; j<nSu; j++)
						{
							CTwinVector tv = tvArrResult.GetAt(nIdx++);
							pArea->AddTwinVector(tv);
						}
						pArea->m_nNum	= nNum++;
						pArea->Calculate();
						pArea->m_sInfo	= sInfo;
						pArrAreaConc->Add(pArea);
					}
				}
				else
				{
					if(i==3)
					{
						CDPointArray vArrRib;
						long nSize = tvArrMid2.GetSize();
						long n=0; for(n=nSize-1; n>=0; n--)
							vArrRib.Add(tvArrMid2.GetAt(n).m_v1);

						rb.RemoveSamePoint(vArrRib);

						nSize = vArrRib.GetSize();

						CString str = _T("아치");
						CTwinVectorAreaShape *pArea	= new CTwinVectorAreaShape;
						pArea->m_tvArr  = tvArrMid2;
						pArea->m_nNum	= nNum++;
						pArea->m_sInfo	= str;
						pArea->m_nTypeShape = SHAPE_TV_FAN;
						pArea->m_xyCen = pBri->GetCentroid(vArrRib, nSize);
						pArea->m_dArcAreaL = pArea->m_dArcAreaR = pBri->GetPolygonArea(vArrRib, nSize);
						pArrAreaConc->Add(pArea);
					}
					
					if(nJigan==0)	// 지간에 상관없이 한번만 들어간다.
					{
						CDPointArray vArr;
						long nSize = tvArr.GetSize();
						long n=0; for(n=nSize-1; n>=0; n--)
							vArr.Add(tvArr.GetAt(n).m_v1);

						rb.RemoveSamePoint(vArr);

						CString str = _T("아치");
						CTwinVectorAreaShape *pArea	= new CTwinVectorAreaShape;
						pArea->m_tvArr  = tvArr;
						pArea->m_nNum	= nNum++;
						pArea->m_sInfo	= str + sInfo;
						pArea->m_nTypeShape = SHAPE_TV_FAN;
						pArea->m_xyCen = pBri->GetCentroid(vArr, nSize);
						pArea->m_dArcAreaL = pArea->m_dArcAreaR = pBri->GetPolygonArea(vArr, nSize);
						pArrAreaConc->Add(pArea);
					}
				}
			}
			else
			{
				long nSu = tvArr.GetSize();
				long j=0; for(j=0; j<nSu; j++)
				{
					CTwinVectorAreaShape *pArea	= new CTwinVectorAreaShape; 
					CTwinVector tv = tvArr.GetAt(j);
					tv.m_sInfo = sInfo;
					pArea->AddTwinVector(tv);
					pArea->m_nNum = nNum++;
					pArea->Calculate();
					pArea->m_sInfo	= sInfo;
					
					// 번호 찍을 자리 xyCen
					CDPoint vUnit = (tv.GetXyMid() - tv.GetXyCircleCenter()).Unit();
					double dLen = GetArcBulge(tv.m_v1, tv.m_v2, tv.m_v2.z);
					CDPoint vMove = CDPoint(vUnit.x*dLen/2, vUnit.y*dLen/2);
					pArea->m_xyCen = tv.GetXyMid() + vMove;
					pArrAreaConc->Add(pArea);
				}
			}
		}
	}

	return nNum;
}

void COutBM::MakeBMPostMain()
{
	CRcBridgeRebar	*pBri	= GetRcBridgeRebar();
	if(!pBri) return;

	// 동바리는 직거리로 삽도를 그리고 직거리 너비를 사용함
	//pBri->SyncBridge(0, FALSE, TRUE, FALSE);
	pBri->GetTvVertSection_Std(0, FALSE, FALSE);
	pBri->SyncBridgeAps(0, FALSE);
	CDPoint vAng = pBri->GetAngleJijum(0);

	// 시,종점 브라켓
	// 브라켓 너비는 직거리로 뽑고, 길이는 사거리로 뽑음
	long nNum	= 1;
	long stt = 0; for(stt = 0; stt < 2; stt++)
	{
		BOOL bStt = stt == iSTT ? TRUE : FALSE;

		// 브라켓 길이 구하기
		pBri->GetTvWingWallPlane(bStt);
		CTwinVector tvBracket	= pBri->m_tvArrPlaneWingWall.GetTvByInfo("브라켓");

		nNum = GetTvAreaShapePostBracket(&m_pArrAreaConc, bStt, nNum);	// 브라켓 영역 구하기.
		long nSize	= m_pArrAreaConc.GetSize();
		long i = 0; for(i = 0; i < nSize; i++)
		{
			CTwinVectorAreaShape *pArea	= m_pArrAreaConc.GetAt(i);

			// 수직면
			CBMData *pBM				= new CBMData;
			pBM->m_nNumbering			= pArea->m_nNum;
			pBM->m_nIdxBigClass			= BM_BIG_POST;
			pBM->m_sBigClass			= GetStringBigClass(pBM->m_nIdxBigClass);
			pBM->m_nLevelTotal			= GetLevelTotal(pBM->m_nIdxBigClass); 
			pBM->m_nIdxSmallClass		= BM_SMALL_POST_STEELPIPE;
			pBM->m_sSmallClass			= GetStringSmallClass(pBM->m_nIdxSmallClass);
			pBM->m_nTypeResult			= RESULT_VOLUMN;
			pBM->m_sUnit				= "㎥";
			pBM->m_nShape				= pArea->m_nTypeShape;
			pBM->m_dV[Width]			= Round(toM(pArea->m_dWidth),3);
			pBM->m_dV[WidthLower]		= Round(toM(pArea->m_dWidthLower),3);
			pBM->m_dV[WidthUpper]		= Round(toM(pArea->m_dWidthUpper),3);
			pBM->m_dV[WidthDiffLeft]	= Round(toM(pArea->m_dWidthDiffLeft),3);
			pBM->m_dV[WidthDiffRight]	= Round(toM(pArea->m_dWidthDiffRight),3);
			pBM->m_dV[Height]			= Round(toM(pArea->m_dHeight),3);
			pBM->m_dV[HeightLeft]		= Round(toM(pArea->m_dHeightLeft),3);
			pBM->m_dV[HeightRight]		= Round(toM(pArea->m_dHeightRight),3);
			pBM->m_dV[Length]			= Round(toM(tvBracket.GetLength()),3);
			pBM->CalculateResult();
			// 예외적인 출력양식
			pBM->m_nLevelMidClass		= 2;
			m_pArrBMBridgeStd.Add(pBM);
		}
	}

	// 각 지간
	for(long jigan = 0; jigan < pBri->m_nQtyJigan; jigan++)
	{
		long nIdxJijumPrev	= jigan;
		long nIdxJijumNext	= jigan+1;
		CDPoint vAngPrev	= pBri->GetAngleJijum(nIdxJijumPrev);
		CDPoint vAngNext	= pBri->GetAngleJijum(nIdxJijumNext);
		vAng	= ToDPointFrDegree((vAngPrev.GetAngleDegree() + vAngNext.GetAngleDegree())/2);
		nNum = GetTvAreaShapePostJigan(&m_pArrAreaConc, jigan, nNum);

		long nSize	= m_pArrAreaConc.GetSize();
		long i = 0; for(i = 0; i < nSize; i++)
		{
			CTwinVectorAreaShape *pArea	= m_pArrAreaConc.GetAt(i);

			CBMData *pBM			= new CBMData;
			pBM->m_nNumbering		= pArea->m_nNum;
			pBM->m_nIdxBigClass		= BM_BIG_POST;
			pBM->m_sBigClass		= GetStringBigClass(pBM->m_nIdxBigClass);
			pBM->m_nLevelTotal		= GetLevelTotal(pBM->m_nIdxBigClass); 
			if(pBri->m_nTypePost == TYPE_POST_BRIDGE)	// 육교용 동바리
			{
				if(pArea->m_sInfo.Find("상부") != -1 || pArea->m_sInfo.Find("헌치R육교") != -1)
					pBM->m_nIdxSmallClass	= BM_SMALL_POST_WOOD;
				else if(pArea->m_sInfo.Find("좌측") != -1 || pArea->m_sInfo.Find("우측") != -1)
					pBM->m_nIdxSmallClass	= BM_SMALL_POST_STEELPIPE;
				else if(pArea->m_sInfo.Find("기본") != -1)
					pBM->m_nIdxSmallClass	= BM_SMALL_POST_BRIDGE_STEEL;
				else
					pBM->m_nIdxSmallClass	= BM_SMALL_POST_STEELPIPE;
			}
			else	// 일반(강관) 동바리
			{
				pBM->m_nIdxSmallClass = BM_SMALL_POST_STEELPIPE;
			}
			pBM->m_sSmallClass = GetStringSmallClass(pBM->m_nIdxSmallClass);
			
			double dLength	= toM(pBri->GetWidthSlabAct(pBri->m_dStationBridgeStt, vAng, 0, FALSE));
			if(pBM->m_nIdxSmallClass	== BM_SMALL_POST_BRIDGE_STEEL)
			{
				pBM->m_nTypeResult			= RESULT_LENGTH;
				pBM->m_sUnit				= "m";
				pBM->m_sItemClass			= "8 x 4.5";//"8 x 4"; 2006년부터 4.5로 변경
				pBM->m_sForm.Format("%.3f", dLength);
				pBM->m_bIsPrintByRefForm	= TRUE;
				pBM->SetResult(dLength);
			}
			else
			{
				if(pArea->m_sInfo.Find("헌치R") != -1 || pArea->m_sInfo.Find("아치") != -1)
				{
					pBM->m_nTypeResult			= RESULT_VOLUMN;
					pBM->m_sUnit				= "㎥";
					pBM->m_nShape				= pArea->m_nTypeShape;
					pBM->m_dV[Width]			= Round(toM(pArea->m_dWidth),3);			// 부채꼴 직선 길이
					pBM->m_dV[Height]			= Round(toM(toM(pArea->m_dArcAreaL)),3);	// 면적
					pBM->m_dV[Length]			= Round(dLength,3);							
					pBM->CalculateResult();
				}
				else
				{
					pBM->m_nTypeResult			= RESULT_VOLUMN;
					pBM->m_sUnit				= "㎥";
					pBM->m_nShape				= pArea->m_nTypeShape;
					pBM->m_dV[Width]			= Round(toM(pArea->m_dWidth), 3);
					pBM->m_dV[WidthLower]		= Round(toM(pArea->m_dWidthLower),3);
					pBM->m_dV[WidthUpper]		= Round(toM(pArea->m_dWidthUpper),3);
					pBM->m_dV[WidthDiffLeft]	= Round(toM(pArea->m_dWidthDiffLeft),3);
					pBM->m_dV[WidthDiffRight]	= Round(toM(pArea->m_dWidthDiffRight),3);
					pBM->m_dV[Height]			= Round(toM(pArea->m_dHeight),3);
					pBM->m_dV[HeightLeft]		= Round(toM(pArea->m_dHeightLeft), 3);
					pBM->m_dV[HeightRight]		= Round(toM(pArea->m_dHeightRight), 3);
					pBM->m_dV[Length]			= Round(dLength,3);
					pBM->CalculateResult();
				}
			}
			// 예외적인 출력양식
			pBM->m_nLevelMidClass		= 2;
			m_pArrBMBridgeStd.Add(pBM);
		}
	}
	pBri->GetTvVertSection_Std(0, FALSE, FALSE, TRUE);
}

//데크피니셔 (반드시 본체 동바리 수량 이후 진행)
void COutBM::MakeBMDeckFinisher()
{
	CRcBridgeRebar	*pBri	= GetRcBridgeRebar();
	if(!pBri) return;
	
	CTypedPtrArray <CObArray, CBMData*> pArrBMSteelPost;
	for (long nIdx =0; nIdx < m_pArrBMBridgeStd.GetSize(); nIdx++)
	{
		CBMData *pBMFind = m_pArrBMBridgeStd.GetAt(nIdx);
		if(pBMFind == NULL) continue;
		if(pBMFind->m_sBigClass == GetStringBigClass(pBMFind->m_nIdxBigClass) && pBMFind->m_nIdxSmallClass == BM_SMALL_POST_STEELPIPE)
		{
			pArrBMSteelPost.Add(pBMFind);
		}
	}

	if(pArrBMSteelPost.GetSize() == 0)
		return;

	for (long nIdx =0; nIdx < pArrBMSteelPost.GetSize(); nIdx++)
	{
		CBMData *pBMPost = pArrBMSteelPost.GetAt(nIdx);
		if(pBMPost == NULL) continue;
		if(pBMPost->m_sBigClass != BM_BIG_POST && pBMPost->m_nIdxSmallClass != BM_SMALL_POST_STEELPIPE)
			continue;

		CString sVal			= _T("");
		double dLength		= pBMPost->m_dV[Length];	
		double dWidth		= pBMPost->m_dV[Height];
		double dResult		= CON_DECKFINISHER * dLength * dWidth;
		sVal.Format(_T("%.3f x %.3f x %.3f"), CON_DECKFINISHER, dWidth, dLength);

		CBMData *pBM					= new CBMData(pBMPost);
		for (long ix =0; ix < ValueSu; ix++)
			pBM->m_dV[Result]				= 0;
		
		pBM->m_nIdxBigClass			= BM_BIG_DECKFINISH;
		pBM->m_sBigClass				= GetStringBigClass(pBM->m_nIdxBigClass);
		pBM->m_nLevelTotal			= GetLevelTotal(pBM->m_nIdxBigClass); 
		pBM->m_nIdxSmallClass		= BM_SMALL_POST_STEELPIPE;
		pBM->m_sSmallClass			= GetStringSmallClass(pBM->m_nIdxSmallClass);
		pBM->m_nTypeResult			= RESULT_AREA;
		pBM->m_sUnit					= "㎡";
		pBM->m_sForm					= sVal;
		pBM->m_sFormExcel			= pBM->m_sFormForCompare = pBM->m_sForm;
		pBM->SetResult(dResult);

		pBM->m_nLevelMidClass		= 2;
		m_pArrBMBridgeStd.Add(pBM);	
	}	
}

// 횡단 삽도 그려 넣기
// MASS 콘크리트이면서 교축직각일때
void COutBM::SetInsertDomyunCross()
{
	CRcBridgeRebar	*pBri	= GetRcBridgeRebar();
	if(!pBri) return;
	
	CHgBaseBMStd baseBMStd;
	CARcBridgeDBStandard DBStd(pBri, m_pStd->m_pARoadOptionStd, m_pStd->m_pARcBridgeDataStd);
	DBStd.m_bApplyDimSettingOfBaseDom	= TRUE;
	CDrawCrossSection *pDrawCrossSection	= DBStd.m_pDrawCrossSection;
	double dSta = pBri->GetStationOnJijum(0,0);

	// 스케일조정용 ////////
	BOOL bDrawRubble = pBri->IsBoxType() && pBri->m_dThickCobbStone > 0 && pBri->m_nTypeLower == LOWERSLAB_TYPE_COBBSTONE;

	CDomyun domTmp;
	domTmp.SetCWnd(m_pStd->m_pView);
	pDrawCrossSection->DrawCrossSection(&domTmp, dSta, CDPoint(0,1), FALSE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, FALSE, TRUE, 0, -1, TRUE, bDrawRubble);
	
	// 콘크리트 산출시 기초 산출전에 기초형태가 날개벽따라 꺽이는거 추가
	CDomyun domCrossTot;
	domCrossTot.SetCWnd(m_pStd->m_pView);

	double dWidthOrg	= 350;

	// 도면 설정 ////////////////////
	double dScale	= baseBMStd.GetScale(&domTmp, dWidthOrg);
	baseBMStd.SetDomyunValue(&domCrossTot, dWidthOrg, &domTmp);

	// 횡단면
	for(long nJ=0; nJ<pBri->GetCountJijum(); nJ++)
	{
		CFootingApp *pFooting	= pBri->GetFootingByJijumNumber(nJ);
		CExFootingApp *pExFooting	= &pFooting->m_exFooting;
		if(		pFooting 
			&&	pFooting->m_bIs
			&&	pExFooting->m_nType==EXFOOTING_TYPE_MASS
			&&	!pFooting->m_exFooting.m_bFrontDanMass	)
		{
			domCrossTot.SetCalcExtRect();
			CDRect rect = domCrossTot.GetExtRect();

			CDomyun domCross(&domCrossTot);

			DBStd.m_pDrawFooting->DrawLeftSide(&domCross, pFooting, FALSE);
			DBStd.m_pDrawExFooting->DrawLeftSide(&domCross, &pFooting->m_exFooting);
			baseBMStd.DrawAreaSectionNumber(&domCross, &pExFooting->m_pArrAreaConcSide, 1);
			DBStd.m_pDrawCrossSection->DimCrossBottom(&domCross, nJ, FALSE);	// 하면 치수
			
			// 제목
			CString str	= _T("");
			str.Format("지점%d 기초", nJ+1);
			domCross.SetCalcExtRect();
			CDRect rcPlane	= domCross.GetExtRect();
			domCross.SetTextAlignHorz(TA_CENTER);
			domCross.TextOut(CDPoint(rcPlane.left+rcPlane.Width()/2, rcPlane.bottom+10), str);

			// 삽도를 WidthOrg 사이즈로 변경
			domCross.RedrawByScale(dScale);
			
			// 이전기초 아래쪽으로 이동
			domCross.SetCalcExtRect();
			rcPlane	= domCross.GetExtRect();
			domCross.Move(CDPoint(rect.left-rcPlane.left , rect.top - 10 - rcPlane.bottom));

			domCrossTot << domCross;
		}
	}

	domCrossTot.SetCalcExtRect();
	
	// MASS 콘크리트 이전에 삽입
	long i = 0; for(i = 0; i < m_pArrBMStringBridge.GetSize(); i++)
	{
		CBMString *pBMString	= m_pArrBMStringBridge.GetAt(i);
		if(pBMString->m_sProcessKind1.Find("콘크리트", 0) != -1 && pBMString->m_sForm.Find("MASS", 0) != -1)
		{
			pBMString->m_Dom	= domCrossTot;
			pBMString->m_bInsertDomyun	= TRUE;
			break;
		}
	}

	DBStd.m_bApplyDimSettingOfBaseDom	= FALSE;
}

// 동바리 삽도 그려 넣기
void COutBM::SetInsertDomyunPost()
{
	CRcBridgeRebar	*pBri	= GetRcBridgeRebar();
	if(!pBri) return;

	CHgBaseBMStd baseBMStd;
	CARcBridgeDBStandard DBStd(pBri, m_pStd->m_pARoadOptionStd, m_pStd->m_pARcBridgeDataStd);
	DBStd.m_bApplyDimSettingOfBaseDom	= TRUE;
	CDrawVertSection *pDrawVertSection	= DBStd.m_pDrawVertSection;

	// 동바리는 직거리로 그림
	//pBri->SyncBridge(0, FALSE, TRUE, FALSE);
	pBri->GetTvVertSection_Std(0, FALSE, FALSE);
	pBri->SyncBridgeAps(0, FALSE);

	long stt = 0;
	long nNum	= 1;

	// 스케일조정용
	CDomyun domTmp;
	domTmp.SetCWnd(m_pStd->m_pView);
	
	pDrawVertSection->DrawVertSectionAll(&domTmp, FALSE, TRUE, TRUE, FALSE, TRUE);
	if(m_pStd->m_bDrawElEarth)
		pDrawVertSection->DrawElEarthLine(&domTmp,0,0,FALSE,TRUE);
	
	// 도면 설정
	CDomyun domPost;
	domPost.SetCWnd(m_pStd->m_pView);
	double dWidthOrg = 600;
	double dScale = baseBMStd.GetScale(&domTmp, dWidthOrg);
	baseBMStd.SetDomyunValue(&domPost, dWidthOrg, &domTmp);
	
	// 종단면
	pDrawVertSection->DrawVertSectionAll(&domPost, TRUE, TRUE, TRUE, FALSE, TRUE);	
	if(m_pStd->m_bDrawElEarth)
		pDrawVertSection->DrawElEarthLine(&domPost,0,0,FALSE,TRUE);

	// 시종점 브라켓 동바리
	for(stt = 0; stt < 2; stt++)
	{
		BOOL bStt = stt == iSTT ? TRUE : FALSE;
		nNum = GetTvAreaShapePostBracket(&m_pArrAreaConc, bStt, nNum);
		baseBMStd.DrawAreaSectionNumber(&domPost, &m_pArrAreaConc, 1, TRUE, 500, bStt ? 45 : 315);
	}

	// 각 지간별 동바리
	for(long jigan = 0; jigan < pBri->m_nQtyJigan; jigan++)
	{
		nNum = GetTvAreaShapePostJigan(&m_pArrAreaConc, jigan, nNum);
		baseBMStd.DrawAreaSectionNumber(&domPost, &m_pArrAreaConc, 1, TRUE, 500, 45);
	}

	// 치수 기입
	DBStd.m_pDrawVertSection->DimPostForBM(&domPost);

	// 면적출력
	DimTextPostAreaOfArch(&domPost);

	// 삽도를 WidthOrg 사이즈로 변경
	domPost.RedrawByScale(dScale);
	domPost.SetCalcExtRect();
	
	// 동바리수량 이전에 삽입
	// 공통수량 전용이면 이미 첫장에 들어가므로 산근 중간에 들어갈 필요 없음
	if(m_dwCalBM != COMMON_BM)
	{
		long i = 0; for(i = 0; i < m_pArrBMStringBridge.GetSize(); i++)
		{
			CBMString *pBMString	= m_pArrBMStringBridge.GetAt(i);
			if(pBMString->m_sProcessKind1.Find("동바리", 0) != -1)
			{
				pBMString->m_Dom	= domPost;
				pBMString->m_bInsertDomyun	= TRUE;
				break;
			}
		}
	}

	// 날개벽 동바리 삽도 삽입
	dWidthOrg = 400;

	for(stt = 0; stt < 2; stt++)
	{
		BOOL bStt	= stt == iSTT ? TRUE : FALSE;
		for(long left = 0; left < 2; left++)
		{
			BOOL bLeft	= left == iLEFT ? TRUE : FALSE;
			if(!pBri->IsWingWall(bStt, bLeft)) continue;

			CDomyun domWing;
			domWing.SetCWnd(m_pStd->m_pView);

			// 도면 설정 ////////////////////
			dScale	= baseBMStd.GetScale(&domTmp, dWidthOrg);
			baseBMStd.SetDomyunValue(&domWing, dWidthOrg, &domTmp);
			///////////////////////////////////
			
			nNum	= 1;
			nNum	= GetTvAreaShapePostWing(&m_pArrAreaConc, bStt, bLeft, nNum);
			baseBMStd.DrawAreaSectionNumber(&domWing, &m_pArrAreaConc, 1, TRUE, 500, bStt ? 45 : 315);
			DBStd.m_pDrawWingWall->DrawGenFront(&domWing, bStt, bLeft, 1, FALSE, FALSE, FALSE);
			DBStd.m_pDrawWingWall->DrawGenCutBriSect(&domWing, bStt, bLeft, 1);
			domWing.SetCalcExtRect();
			CDRect rect	= domWing.GetExtRect();
			baseBMStd.DimAreaSection(&domWing, &m_pArrAreaConc, 0, FALSE, CDPoint(rect.right, 0));
			baseBMStd.DimAreaSection(&domWing, &m_pArrAreaConc, 2, FALSE, CDPoint(rect.left, 0));
			baseBMStd.DimAreaSection(&domWing, &m_pArrAreaConc, 3, FALSE, CDPoint(0, rect.top));

			// 삽도를 WidthOrg 사이즈로 변경
			domWing.RedrawByScale(dScale);
			domWing.SetCalcExtRect();
			
			// 날개벽의 동바리 수량 이전에 삽입
			CTypedPtrArray <CObArray, CBMString*> *pArrBMString	= &m_pArrBMStringWing[stt][left];

			// 동바리수량 이전에 삽입
			long i = 0; for(i = 0; i < pArrBMString->GetSize(); i++)
			{
				CBMString *pBMString	= pArrBMString->GetAt(i);
				if(pBMString->m_sProcessKind1.Find("동바리", 0) != -1)
				{
					pBMString->m_Dom	= domWing;
					pBMString->m_bInsertDomyun	= TRUE;
					break;
				}
			}
		}
	}

	pBri->GetTvVertSection_Std(0, FALSE, FALSE, TRUE);
	DBStd.m_bApplyDimSettingOfBaseDom	= FALSE;
}

// 기초 평면도 삽도 그려 넣기
void COutBM::SetInsertDomyunPlaneFooting()
{
	CRcBridgeRebar	*pBri	= GetRcBridgeRebar();
	if(!pBri) return;

	CHgBaseBMStd baseBMStd;
	CARcBridgeDBStandard DBStd(pBri, m_pStd->m_pARoadOptionStd, m_pStd->m_pARcBridgeDataStd);
	DBStd.m_bApplyDimSettingOfBaseDom	= TRUE;

	// 콘크리트 산출시 기초 산출전에 기초형태가 날개벽따라 꺽이는거 추가
	// 스케일조정용 ///////////
	CDomyun domTmp;
	domTmp.SetCWnd(m_pStd->m_pView);
	long nCountFooting	= pBri->GetCountFooting();	
	long i = 0; for(i = 0; i < nCountFooting; i++)
	{
		CFootingApp *pFooting	= pBri->GetFooting(0);
		if(pFooting)
		{
			CDomyun domTmpTmp(&domTmp);

			domTmp.SetCalcExtRect();
			CDRect rect = domTmp.GetExtRect();


			m_pStd->m_pARoadOptionStd->SetEnvType(&domTmpTmp, HCAD_CONC);
			baseBMStd.DrawAreaSectionNumber(&domTmpTmp, &pFooting->m_pArrAreaConcPlane, 1);

			m_pStd->m_pARoadOptionStd->SetEnvType(&domTmpTmp, HCAD_DIML);
			baseBMStd.DimAreaSection(&domTmpTmp, &pFooting->m_pArrAreaConcPlane, 0, TRUE);
			baseBMStd.DimAreaSection(&domTmpTmp, &pFooting->m_pArrAreaConcPlane, 3, TRUE);
			baseBMStd.DimAreaSection(&domTmpTmp, &pFooting->m_pArrAreaConcPlane, 1, TRUE);

			domTmpTmp.SetCalcExtRect();
			CDRect rcPlane	= domTmpTmp.GetExtRect();
			domTmpTmp.Move(CDPoint(rect.left-rcPlane.left , rect.top - 10 - rcPlane.bottom));

			domTmp << domTmpTmp;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	
	CDomyun domFootingPlaneTot;
	domFootingPlaneTot.SetCWnd(m_pStd->m_pView);
	((CDomyunView*)m_pStd->m_pView)->InitDrawBasePoint();
	
	double dWidthOrg	= 350;

	// 도면 설정 ////////////////////
	double dScale	= baseBMStd.GetScale(&domTmp, dWidthOrg);
	baseBMStd.SetDomyunValue(&domFootingPlaneTot, dWidthOrg, &domTmp);
	///////////////////////////////////

	for(i = 0; i < pBri->GetCountJijum(); i++)
	{
		CFootingApp *pFooting	= pBri->GetFootingByJijumNumber(i);
		if(pFooting)
		{
			if(!pFooting->m_bIs) continue;
			domFootingPlaneTot.SetCalcExtRect();
			CDRect rect = domFootingPlaneTot.GetExtRect();

			CDomyun domFootingPlane(&domFootingPlaneTot);
			domFootingPlane.ClearEtt(TRUE, TRUE);
			baseBMStd.DrawAreaSectionNumber(&domFootingPlane, &pFooting->m_pArrAreaConcPlane, 1);
			baseBMStd.DimAreaSection(&domFootingPlane, &pFooting->m_pArrAreaConcPlane, 0, TRUE);
			baseBMStd.DimAreaSection(&domFootingPlane, &pFooting->m_pArrAreaConcPlane, 3, TRUE);
			baseBMStd.DimAreaSection(&domFootingPlane, &pFooting->m_pArrAreaConcPlane, 1, TRUE);

			// 제목
			CString str	= _T("");
			str.Format("지점%d 기초 평면", i+1);
			domFootingPlane.SetCalcExtRect();
			CDRect rcPlane	= domFootingPlane.GetExtRect();
			domFootingPlane.SetTextAlignHorz(TA_CENTER);
			domFootingPlane.TextOut(CDPoint(rcPlane.left+rcPlane.Width()/2, rcPlane.bottom+10), str);
			
			// 이전기초 아래쪽으로 이동
			domFootingPlane.SetCalcExtRect();
			rcPlane	= domFootingPlane.GetExtRect();
			domFootingPlane.Move(CDPoint(rect.left-rcPlane.left , rect.top - 10 - rcPlane.bottom));

			

			domFootingPlaneTot << domFootingPlane;
		}
	}


	// 삽도를 WidthOrg 사이즈로 변경
	domFootingPlaneTot.RedrawByScale(dScale);

	domFootingPlaneTot.SetCalcExtRect();

	// 기초콘크리트라고 나오는 첫번째 수량에 추가
	BOOL bInputFootingPlane	= FALSE;
	for(i = 0; i < m_pArrBMStringBridge.GetSize(); i++)
	{
		CBMString *pBMString	= m_pArrBMStringBridge.GetAt(i);
		if(pBMString->m_sInfo.Find("기초콘크리트", 0) != -1)
		{
			pBMString->m_Dom = domFootingPlaneTot;
			pBMString->m_bInsertDomyun	= TRUE;
			bInputFootingPlane	= TRUE;
			break;
		}
	}

	// 기초거푸집이라고 나오는 첫번째 수량에 추가
	// 단, 콘크리트를 출력하는 경우에는 추가 하지 않는다.
	if(!bInputFootingPlane)
	{
		for(i = 0; i < m_pArrBMStringBridge.GetSize(); i++)
		{
			CBMString *pBMString	= m_pArrBMStringBridge.GetAt(i);
			if(pBMString->m_sInfo.Find("기초거푸집", 0) != -1)
			{
				pBMString->m_Dom = domFootingPlaneTot;
				pBMString->m_bInsertDomyun	= TRUE;
				break;
			}
		}
	}

	DBStd.m_bApplyDimSettingOfBaseDom	= FALSE;
}

// 콘크리트 수량(날개벽)
// long nForUse : 0 : 원래 용도(콘크리트 수량)  
//                1 : 뒷채움용 날개벽 공제 : 뒷채움 영역과 교집합 부분만 구함
//                2 : 되메우기의 뒷채움용 날개벽 공제 : 지반선까지의 뒷채움 영역과 교집합 부분만 구함
//                3 (노상공제토용) : 노상 상하부(하부한계는 지반선)
//                4 (노체공제토용) : 노체 상하부(하부한계는 지반선)
//				  6 : 되메우기의 구체 공제
void COutBM::MakeBMConcreteWing(long nForUse)
{
	CRcBridgeRebar *pBri	= GetRcBridgeRebar(TRUE);
	if(!pBri) return;
	
	CTwinVectorArray tvArrVertSectionOld;
	tvArrVertSectionOld	= pBri->m_tvArrVertSection;
	long nTypeJongdan	= pBri->m_nTypeJongdanForRebar;
	long nTypeJongdanOld	= pBri->m_nTypeJongdanForRebarOld;
	pBri->m_nTypeJongdanForRebar	= 2;

	CalAreaConcWing(pBri, nForUse);

	long stt(0), left(0);
	long nSize(0), i(0);
	CTwinVector tv;

	for(stt = 0; stt < 2; stt++)
	{
		BOOL bStt	= stt == iSTT;
		long nJ		= bStt ? 0 : pBri->m_nQtyJigan;
		if(pBri->IsBoxType()) nJ = 0;

		for(left = 0; left < 2; left++)
		{
			BOOL bLeft	= left == iLEFT;
			if(!pBri->IsWingWall(bStt, bLeft)) continue;
			CString sleft = (bLeft)? _T("(좌)") : _T("(우)");
			// 토공용으로 구하는 경우 토공수량에 추가한다.
			// 단 자세한 내용은 이 함수를 호출 하는 곳에서 수정을 해 줘야 됨.
			CTypedPtrArray <CObArray, CBMData*> *pArrBMStd	= nForUse == 0 ? &m_pArrBMWingStd[stt][left] : &m_pStd->m_pArrOutBMTogong.GetAt(m_nBri)->m_pArrBMTogongStd[nJ];

			long nIdxItemClass	= bStt ? (bLeft ? BM_ITEM_CONCRETE_WING_SL : BM_ITEM_CONCRETE_WING_SR) : 
										 (bLeft ? BM_ITEM_CONCRETE_WING_EL : BM_ITEM_CONCRETE_WING_ER);
			CWingWall *pWing		= pBri->GetWingWall(bStt, bLeft);

			CRebarPlacing rb;
			double dThickWingWall	= pWing->m_nDirHunch == 1 ? pWing->m_dT2 : pWing->m_dT1;
			CBMData *pBMBase		= NULL;	// 헌치부 철근 생성할때 기본되는 수량용
			nSize	= pWing->m_pArrAreaConc.GetSize();
			for(i = 0; i < nSize; i++)
			{
				// 시점 종점.. 그리고 
				CTwinVectorAreaShape *pArea	= pWing->m_pArrAreaConc.GetAt(i);

				// 단면변화선이 나오는  순간부터 날개벽 두께는 dT1을 적용한다.
				if(pArea->GetTvLower().m_sInfo.Find("단면변화선", 0) != -1)
				{
					dThickWingWall	= pWing->m_dT1;
				}

				CBMData *pBM				= new CBMData;
				pBMBase						= pBM;
				pBM->m_nNumbering			= pArea->m_nNum;
				pBM->m_nLevelTotal			= 2;
				pBM->m_nIdxBigClass			= BM_BIG_CONCRETE;
				pBM->m_sBigClass			= GetStringBigClass(pBM->m_nIdxBigClass);
				pBM->m_nIdxMidClass			= BM_MID_CONCRETE_MAIN;
				pBM->m_sMidClass			= GetStringMidClass(pBM->m_nIdxMidClass, NULL, bStt? ePartWingWallStt : ePartWingWallEnd);
				if(pBri->IsBoxType())
				{
					pBM->m_nIdxPartClass		= bStt ? 0 : 1;
					pBM->m_sPartClass	    	= bStt ? "시점" : "종점";
				}
				pBM->m_nIdxSmallClass		= BM_SMALL_CONCRETE_WING;
				pBM->m_sSmallClass			= GetStringSmallClass(pBM->m_nIdxSmallClass);
				pBM->m_nIdxItemClass		= nIdxItemClass;
				pBM->m_sItemClass			= GetStringItemClass(pBM->m_nIdxItemClass);
				pBM->m_nTypeResult			= RESULT_VOLUMN;
				pBM->m_sUnit				= "㎥";
				pBM->m_nShape				= pArea->m_nTypeShape;
				pBM->m_dV[WidthLower]		= Round(toM(pArea->m_dWidthLower),3);
				pBM->m_dV[WidthUpper]		= Round(toM(pArea->m_dWidthUpper),3);
				pBM->m_dV[WidthDiffLeft]	= Round(toM(pArea->m_dWidthDiffLeft),3);
				pBM->m_dV[WidthDiffRight]	= Round(toM(pArea->m_dWidthDiffRight),3);
				pBM->m_dV[Width]			= Round(toM(pArea->m_dWidth),3);
				pBM->m_dV[HeightLeft]		= Round(toM(pArea->m_dHeightLeft),3);
				pBM->m_dV[HeightRight]		= Round(toM(pArea->m_dHeightRight),3);
				pBM->m_dV[Height]			= Round(toM(pArea->m_dHeight),3);
				pBM->m_dV[Length]			= Round(toM(dThickWingWall),3);
				pBM->CalculateResult();
				pArrBMStd->Add(pBM);
			}

			// 3. 벽체와 겹치는 부분의 헌치부는 날개벽별로 한번에 계산함.
			// 이 코드는 거푸집에서 따와서 수정한 내용임
			// 현재 콘크리트는 미터별로 자르지 않으나, 차후 자르는걸 지원하게 되면 거푸집의 코드를 참고해야함.
			CTwinVectorArray tvArrWing;
			int se = bStt ? -1 : 1;
			int nLeft = bLeft ? -se : se;	// 시점일 경우 교량 진행 방향의 좌우와 날개벽의 좌우가 반대임.
			pBri->GetTvVertSection(nLeft, TRUE, TRUE);
			pBri->GetTvWingWallFront(bStt, bLeft, 1, tvArrWing, TRUE, FALSE, TRUE, TRUE);
			pBri->GetTvWingWallPlane(bStt);
			CTwinVector tvHunch	= pBri->m_tvArrPlaneWingWall.GetTvByInfo(bLeft ? "좌측날개벽시점헌치" : "우측날개벽시점헌치");
			CTwinVectorArray tvArrHunch;
			for(long h = 0; h < tvArrWing.GetSize(); h++)
			{
				tv	= tvArrWing.GetAt(h).Sort(FALSE);
				if(tv.m_sInfo == "벽체")
					tvArrHunch.Add(tv);
			}

			// 수량 산출함
			// 수직면과 경사면 두가지 있을 수 있음
			if(pBMBase && pWing->m_dHuH2 > 0 && pWing->m_dHuW2 > 0)
			{
				CBMData *pBMVert	= new CBMData(pBMBase);
				pBMVert->m_dV[Result] = 0;		// 값이 들어있는 경우가 있어 초기화 시킨다.

				CString str			= _T("");
				for(long v = 0; v < tvArrHunch.GetSize(); v++)
				{
					tv	= tvArrHunch.GetAt(v);
					double dtvLength = Round(toM(tv.GetLength()),3);
					str.Format("%.3f", dtvLength);

					pBMVert->m_sForm		+= (v==0 ? str : " + " + str);
					pBMVert->m_dV[Result]	+= Round(dtvLength,3);
				}

				if(pBMVert->m_sForm != "")
				{
					CBMData bm;
					bm.m_dV[WidthLower]	= Round(toM(pWing->m_dHuH2),3);
					bm.m_dV[WidthUpper]	= Round(toM(pWing->m_dHuW2),3);
					bm.m_dV[Height]		= Round(toM(tvHunch.GetLength()),3);
					
					const long nFRONT =	2;
					double dAreaHunch	= Round(bm.GetArea8(WidthLower, WidthUpper, Height, nFRONT),3);	// nFRONT혹은 nBACK임
					
					pBMVert->m_nNumbering			= 99999;	// 99999는 넘버링으로 찍히면 안되고 정렬할때는 마지막으로 가야되는 경우 사용함
					str	= pBMVert->m_sForm;
					pBMVert->m_sForm.Format("(%s) x %.3f", str, dAreaHunch);
					pBMVert->m_sFormExcel = pBMVert->m_sFormForCompare = pBMVert->m_sForm;
					pBMVert->m_nShape	= SHAPE_NONE;
					pBMVert->m_dV[Result] *= Round(dAreaHunch,3);
					pBMVert->m_sNoteHead	= "헌치부";
					pArrBMStd->Add(pBMVert);
				}
				else 
					delete pBMVert;
			}

			for(i = 0; i < nSize; i++)
			{
				// 시점 종점.. 그리고 
				CTwinVectorAreaShape *pArea	= pWing->m_pArrAreaConc.GetAt(i);

				// 단면변화선이 나오는  순간부터 날개벽 두께는 dT1을 적용한다.
				if(pArea->GetTvLower().m_sInfo.Find("단면변화선", 0) != -1)
				{
					dThickWingWall	= pWing->m_dT1;
				}

				CBMData *pBM				= new CBMData(pBMBase);	// 잠깐 사용할려고 만듦
				pBM->m_nLevelTotal			= 2;
				pBM->m_nIdxBigClass			= BM_BIG_CONCRETE;
				pBM->m_sBigClass			= GetStringBigClass(pBM->m_nIdxBigClass);
				pBM->m_nIdxMidClass			= BM_MID_CONCRETE_MAIN;
				pBM->m_sMidClass			= GetStringMidClass(pBM->m_nIdxMidClass, NULL, bStt? ePartWingWallStt : ePartWingWallEnd);
				pBM->m_nIdxSmallClass		= BM_SMALL_CONCRETE_WING;
				pBM->m_sSmallClass			= GetStringSmallClass(pBM->m_nIdxSmallClass);
				pBM->m_nIdxItemClass		= nIdxItemClass;
				pBM->m_sItemClass			= GetStringItemClass(pBM->m_nIdxItemClass);
				pBM->m_nTypeResult			= RESULT_VOLUMN;
				pBM->m_sUnit				= "㎥";

				// 1. 날개벽 정면 하단(기초와 겹치는 부분 헌치에 대한 콘크리트 추가)
				if(pArea->m_tvArr.GetTvByInfo("날개벽정면하단", tv))
				{
					CBMData *pBMHunch			= new CBMData(pBM);
					pBMHunch->m_nNumbering		= 99999;	// 99999는 넘버링으로 찍히면 안되고 정렬할때는 마지막으로 가야되는 경우 사용함
					pBMHunch->m_sNoteHead		= "헌치부";
					pBMHunch->m_nShape			= SHAPE_TV_TRIANGLE;
					pBMHunch->m_dV[WidthLower]	= Round(toM(pWing->m_dHuW1),3);
					pBMHunch->m_sVC[WidthLower]	= _T("");
					pBMHunch->m_dV[Height]		= Round(toM(pWing->m_dHuH1),3);
					pBMHunch->m_sVC[Height]		= _T("");
					pBMHunch->m_dV[Length]		= Round(toM(tv.GetLength()),3);
					pBMHunch->m_sVC[Length]		= _T("");
					pBMHunch->CalculateResult();
					pArrBMStd->Add(pBMHunch);
				}

				// 2. 단면변화선이 있는 경우 여기에 대해서 T2->T1으로 변화되는 헌치부에 대한 콘크리트 양 추가
				if(pArea->GetTvLower().m_sInfo.Find("단면변화선", 0) != -1)
				{
					CTwinVectorArray tvArrSec;
					pBri->GetTvWingWallSection(bStt, bLeft, tvArrSec);
					CTwinVector tv	= tvArrSec.GetTvByInfo("날개벽단면내측헌치");
					if(tv.GetHorLength() > 0 && tv.GetVerLength() > 0)
					{
						CBMData *pBMHunch			= new CBMData(pBM);
						pBMHunch->m_nNumbering		= 99999;	// 99999는 넘버링으로 찍히면 안되고 정렬할때는 마지막으로 가야되는 경우 사용함
						pBMHunch->m_sNoteHead		= "헌치부";
						pBMHunch->m_nShape			= SHAPE_TV_TRIANGLE;
						pBMHunch->m_dV[WidthLower]	= Round(toM(tv.GetHorLength()),3);
						pBMHunch->m_sVC[WidthLower]	= _T("");
						pBMHunch->m_dV[Height]		= Round(toM(tv.GetVerLength()),3);
						pBMHunch->m_sVC[Height]		= _T("");
						pBMHunch->m_dV[Length]		= Round(toM(pArea->m_dWidthLower),3);
						pBMHunch->m_sVC[Length]		= _T("");
						pBMHunch->CalculateResult();
						pArrBMStd->Add(pBMHunch);
					}
				}
				delete pBM;	// 지움
			}
		}
	}

	pBri->m_tvArrVertSection	= tvArrVertSectionOld;
	pBri->m_nTypeJongdanForRebar	= nTypeJongdan;
	pBri->m_nTypeJongdanForRebarOld	= nTypeJongdanOld;
}

// 날개벽 산출근거 첫 페이지 삽도 설정
void COutBM::DrawFirstPageWing(CHgBaseBMStd *pBaseBMStd, BOOL bStt, BOOL bLeft)
{
	if(!pBaseBMStd) return;

	CRcBridgeRebar *pBri	= GetRcBridgeRebar(TRUE);
	if(!pBri) return;

	CalAreaConcWing(pBri);

	CARcBridgeDBStandard DBStd(pBri, m_pStd->m_pARoadOptionStd, m_pStd->m_pARcBridgeDataStd);
	DBStd.m_bApplyDimSettingOfBaseDom	= TRUE;
	CDrawWingWall *pDrawWingWall	= DBStd.m_pDrawWingWall;

	if(!pBri->IsWingWall(bStt, bLeft)) return;
	CWingWall *pWing	= pBri->GetWingWall(bStt, bLeft);
	if(!pWing) return;
	
	// 날개벽의 스케일을 결정하자!!! ////////////////////////////////////
	CDomyun domTmp;
	domTmp.SetCWnd(m_pStd->m_pView);
	domTmp.SetScaleDim(1);
	pDrawWingWall->DrawGenFront(&domTmp, bStt, bLeft);

	// 실제로 출력되었으면 하는 사이즈
	double dWidthOrg = 300;	// 도면 가로크기
	/////////////////////////////////////////////////////////////////////////

	// 날개벽 단면 그리기
	CDomyun domWing;
	domWing.SetCWnd(m_pStd->m_pView);

	// 도면 설정 ////////////////////
	double dScale	= pBaseBMStd->GetScale(&domTmp, dWidthOrg);
	pBaseBMStd->SetDomyunValue(&domWing, dWidthOrg, &domTmp);
	///////////////////////////////////
	
	// 시종점 좌우측
	CString sTitle	= _T("");
	sTitle.Format("%s 날개벽(%s)", bStt ? "시점측" : "종점측", bLeft ? "좌" : "우");

	pBaseBMStd->DrawAreaSectionNumber(&domWing, &pWing->m_pArrAreaConc);
	pBaseBMStd->DimAreaSection(&domWing, &pWing->m_pArrAreaConc, 1, TRUE, CDPoint(0, 0));
	pBaseBMStd->DimAreaSection(&domWing, &pWing->m_pArrAreaConc, bStt ? 2 : 0, TRUE, CDPoint(0, 0));

	// 삽도를 WidthOrg 사이즈로 변경
	domWing.RedrawByScale(dScale, TRUE);

	domWing.SetCalcExtRect();
	double dWidth	= 300;
	double dHeight	= 600;
	//long nRow	= pBaseBMStd->InsertDomyun(5, TA_CENTER, &domWing, domWing.GetExtRect().Width(), domWing.GetExtRect().Height(), sTitle);
	CDoubleArray dArrTmp;
	pBaseBMStd->InsertDomyun(5, TA_CENTER, &domWing, dArrTmp, dWidth, dHeight, sTitle);

	/*
	// 비계평면도 그리기 /////////////////////////////////////////////////////////
	// 종단면도의 스케일을 결정하자!!! ////////////////////////////////////
	// 그걸로 종단면과 관련된 도면을 그려내자.
	CDomyun domTmp2;
	domTmp2.SetCWnd(m_pStd->m_pView);
	domTmp2.SetScaleDim(1);
	domTmp2.SetDimTxt(1);
	pDrawVertSection->DimVertSectionAll(&domTmp2, TRUE);
	// 원지반선
	//if(m_pStd->m_bDrawElEarth)
	//	pDrawVertSection->DrawElEarthLine(&domTmp2, 0,0,FALSE,TRUE);
	////////////////////////////////////////////////

	dWidthOrg		= 500;
	CDomyun domPlane;
	domPlane.SetCWnd(m_pStd->m_pView);

	// 도면 설정 ////////////////////
	dScale	= pBaseBMStd->GetScale(&domTmp2, dWidthOrg);
	pBaseBMStd->SetDomyunValue(&domPlane, dWidthOrg, &domTmp2);
	///////////////////////////////////

	// 비계평면도 그리기
	DBStd.m_pDrawPlane->DrawPlaneScaffold(&domPlane);

	// 삽도를 WidthOrg 사이즈로 변경
	domPlane.RedrawByScale(dScale, TRUE);
	DBStd.RotateDomByBridgeSttEndXy(&domPlane);
	
	// 엑셀에 삽입
	domPlane.SetCalcExtRect();
	pBaseBMStd->InsertDomyun(nRow+3, TA_CENTER, &domPlane, domPlane.GetExtRect().Width(), domPlane.GetExtRect().Height());
	*/

	DBStd.m_bApplyDimSettingOfBaseDom	= FALSE;
}


// 밑에서 위로 수량을 자른다.
// 미터정보를 나타내야 되는 수량에 대해서 수량관련 정보를 생성해서 넣어준다.
// 자르는건 수량 구하면서 이미 다 잘랐음
void COutBM::RemakeByHeightBMList(CTypedPtrArray <CObArray, CBMData*> *pArrBM, double dFirstMeter, double dSecondMeter, long nIdxStt, long nIdxEnd, long nOpt)
{
	if(!pArrBM) return;
	long nSize		= pArrBM->GetSize();

	long nIdxMeter	= 0;
	double dYCur	= 0;
	double dYOld	= 0;
	if(nIdxStt == -1) nIdxStt	= 0;
	if(nIdxEnd == -1) nIdxEnd	= nSize-1;
	if(nIdxEnd >= nSize) nIdxEnd	= nSize-1;

	CStringArray sArr;
	sArr.Add("0~7m");
	sArr.Add("7~10m");
	sArr.Add("10~13m");
	sArr.Add("13~16m");
	sArr.Add("16~19m");
	sArr.Add("19m 이상");

	long i = 0; for(i = nIdxStt; i <= nIdxEnd; i++)
	{
		CBMData *pBM	= pArrBM->GetAt(i);
		if(!IsPrintMeter(pBM)) continue;

		dYCur		= 0;
		dYOld		= 0;
		nIdxMeter	= 0;

		while(1)
		{
			dYCur += nIdxMeter==0 ? dFirstMeter : dSecondMeter;
			if(Compare(dYCur, frM(pBM->m_dV[EndMeter]), ">="))
			{
				if(pBM->m_bSlab && nOpt > 0)		// 한개의 슬래브라면 사용자가 지정한 위치로
				{
					CString str = sArr[nOpt-1];
					pBM->m_sMeterClass.Format("%s(%s)", pBM->m_sSmallClass, str);
					pBM->m_nIdxMeterClass	= nOpt-1;
					break;
				}
				pBM->m_sMeterClass.Format("%s(%.0f~%.0fm)", pBM->m_sSmallClass, toM(dYOld), toM(dYCur));
				pBM->m_nIdxMeterClass	= nIdxMeter;
				break;
			}
			nIdxMeter++;

			if(!pBM->m_bSlab)
				dYOld = dYCur;
		}
	}
}

// 미터정보 출력하는 수량은 자를게 없어도 미터정보를 만들어 줘야 된다.
BOOL COutBM::IsPrintMeter(CBMData *pBM)
{
	if(pBM->m_nIdxBigClass == BM_BIG_FORM) return TRUE;

	return FALSE;
}

// 미터정보 나눌때 첫번째 자를 높이와 그 다음 자를 높이 가져옴.
void COutBM::GetInfoMeter(CBMData *pBM, double &dFirstMeter, double &dSecondMeter)
{
	if(pBM->m_nIdxBigClass == BM_BIG_FORM)
	{
		dFirstMeter		= 7;
		dSecondMeter	= 3;
	}
}

// 동바리 수평연결재
// 동바리 수량에는 이미 본체 길이 정보가 들어 있음.
void COutBM::MakeBMHoriConnectMain()
{
	// 동바리 수평 연결재 설치 높이
	// 최초 3.5미터에 설치하고, 이후 2미터 마다 설치한다.
	double dFirstHeight		= 3500;
	double dSecondHeight	= 2000;

	CTwinVector tvLeft, tvRight;
	CDPoint xyMatchLeft(0, 0), xyMatchRight(0, 0);
	BOOL bFirst	= TRUE;
	double dYBase	= 0;
	double dYCur	= 0;

	CRcBridgeRebar	*pBri	= GetRcBridgeRebar();
	if(!pBri) return;

	// 수평연결재도 직거리로
	pBri->GetTvVertSection_Std(0, FALSE, FALSE);
	
	CDPoint vAng = pBri->GetAngleJijum(0);
	double dWidth	= pBri->GetWidthSlabAct(pBri->m_dStationBridgeStt, vAng, 0, FALSE);

	// 시,종점 브라켓
	CTwinVectorArray tvArr, tvArrCen;
	
	long nNum	= 1;
	long nNumLast = nNum;
	long stt = 0; for(stt = 0; stt < 2; stt++)
	{
		BOOL bStt = stt == iSTT ? TRUE : FALSE;
		pBri->GetTvScaffoldPlane(tvArr, tvArrCen, bStt);	// 피계폭과 연결재 들어가는 폭이 같음
		nNum	= GetTvAreaShapePostBracket(&m_pArrAreaConc, bStt, nNum);	// 브라켓 영역 구하기.
		dYBase	= 0;
		dYCur	= 0;

		long nSize	= m_pArrAreaConc.GetSize();
		long i = 0; for(i = 0; i < nSize; i++)
		{
			CTwinVectorAreaShape *pArea	= m_pArrAreaConc.GetAt(i);
			tvLeft	= pArea->GetTvLeft();
			tvRight	= pArea->GetTvRight();

			dYBase	= i == 0 ? tvLeft.GetXyBottom().y : dYBase;
			dYCur	= i == 0 ? dYBase : dYCur;
			bFirst	= TRUE;
			double dPrevH = 0;
			while(1)
			{
				dYCur += bFirst ? dFirstHeight : dSecondHeight;
				
				bFirst	= FALSE;
				double dMaxEL = max(tvLeft.GetXyTop().y, tvRight.GetXyTop().y);
				//if(max(tvLeft.GetXyTop().y, tvRight.GetXyTop().y) <= dYCur) break;
				if(tvLeft.GetXyBottom().y >= dYCur) continue;
				if(dMaxEL <= dPrevH && dMaxEL <= dYCur) break;

				// 도형의 좌우측 교차점 찾기
				GetXyMatchLineAndLine(tvLeft.m_v1, tvLeft.GetXyDir(), CDPoint(0, dYCur), CDPoint(1, 0), xyMatchLeft);
				GetXyMatchLineAndLine(tvRight.m_v1, tvRight.GetXyDir(), CDPoint(0, dYCur), CDPoint(1, 0), xyMatchRight);

				CBMData *pBM				= new CBMData;
				pBM->m_nNumbering			= pArea->m_nNum;
				pBM->m_nIdxBigClass			= BM_BIG_POST;
				pBM->m_sBigClass			= GetStringBigClass(pBM->m_nIdxBigClass);
				pBM->m_nLevelTotal			= GetLevelTotal(pBM->m_nIdxBigClass); 
				pBM->m_nIdxSmallClass		= BM_SMALL_POST_HORI_CONNECT;
				pBM->m_sSmallClass			= GetStringSmallClass(pBM->m_nIdxSmallClass);
				pBM->m_sNoteHead.Format("수평연결재(h=%.1f초과시 높이 %.1fm마다 설치)", toM(dFirstHeight), toM(dSecondHeight));
				pBM->m_nTypeResult			= RESULT_AREA;
				pBM->m_sUnit				= "㎡";
				pBM->m_nShape				= SHAPE_TV_RECTANGLE;
				pBM->m_dV[WidthLower]		= Round(toM(fabs(xyMatchRight.x-xyMatchLeft.x)),3);
				pBM->m_dV[Length]			= Round(toM(tvArrCen.GetLength()), 3);//pBri->GetWidthSlabAct(pBri->m_dStationBridgeStt, vAng, 0, FALSE)),3);
				pBM->m_dwSide				= SIDE_LOWER;
				pBM->CalculateResult();
				// 예외적인 출력양식
				pBM->m_nLevelMidClass		= 2;
				dPrevH = dYCur;
				m_pArrBMBridgeStd.Add(pBM);
			}
		}
	}
	nNumLast = nNum;

	// 각 지간
	CRebarPlacing rb;
	
	CTwinVectorArray tvArrUpper, tvArrLeft, tvArrRight, tvArrMid, tvArrArch, tvArrMid2;
	// 헌치가 있더라도 슬래브를 받쳐 줘야 하므로 무조건 슬래브 폭으로 한다.
	

	for(long jigan = 0; jigan < pBri->m_nQtyJigan; jigan++)
	{
		nNum	= GetTvAreaShapePostJigan(&m_pArrAreaConc, jigan, nNum);
		pBri->GetTvPostJigan(tvArrUpper, tvArrLeft, tvArrRight, tvArrMid, tvArrArch, tvArrMid2, jigan);

		//1. 목재용 동바리가 있다면 진행하지 않음
		BOOL bPostWood = FALSE;
		for (long nIdx =0; nIdx < m_pArrAreaConc.GetSize(); nIdx++)
		{
			CTwinVectorAreaShape *pArea	= m_pArrAreaConc.GetAt(nIdx);
			if(pArea == NULL) continue;
			if(pArea->m_tvArr.GetSize() == 0) continue;

			if(pArea->m_sInfo.Find("상부") != -1 || pArea->m_sInfo.Find("헌치R육교") != -1)
			{
				bPostWood = TRUE;
				break;
			}
		}

		//2.상부슬래브 Num체크
		long nNumSlab = -1;
		for (long nIdx =0; nIdx < pBri->m_pArrAreaConc.GetSize(); nIdx++)
		{
			CTwinVectorAreaShape *pAreaConc = 	pBri->m_pArrAreaConc.GetAt(nIdx);
			if(pAreaConc == NULL) continue;
			if(pAreaConc->m_tvArr.GetSize() == 0) continue;

			if(pAreaConc->m_sInfo.Find(_T("상부슬래브") )> -1)
			{
				nNumSlab = pAreaConc->m_nNum;
				break;
			}			
		}

		if(bPostWood || nNumSlab == -1) 
			continue;
		if(tvArrMid.GetSize() == 0)
			continue;
		
		//3. 해당 최고 EL 최저 EL 구함
		double dMaxEL = tvArrMid.GetXyTop().y;
		double dMinEL = tvArrMid.GetXyBottom().y;
		double dHeight = fabs(dMaxEL - dMinEL);
		double dLenSlab = fabs(tvArrMid.GetXyLeft().x - tvArrMid.GetXyRight().x);

		if(dMaxEL == dMinEL || dHeight <=0 || dLenSlab <= 0)
			continue;
		
		long nCount = 0;
		while(1)
		{
			if(nCount == 0)
				dHeight -= dFirstHeight;
			else 
				dHeight -= dSecondHeight;

			CBMData *pBM					= new CBMData;
			pBM->m_nNumbering			= nNumSlab;
			pBM->m_nIdxBigClass			= BM_BIG_POST;
			pBM->m_sBigClass				= GetStringBigClass(pBM->m_nIdxBigClass);
			pBM->m_nLevelTotal			= GetLevelTotal(pBM->m_nIdxBigClass); 
			pBM->m_nIdxSmallClass		= BM_SMALL_POST_HORI_CONNECT;
			pBM->m_sSmallClass			= GetStringSmallClass(pBM->m_nIdxSmallClass);
			pBM->m_sNoteHead.Format("수평연결재(h=%.1f초과시 높이 %.1fm마다 설치)", toM(dFirstHeight), toM(dSecondHeight));
			pBM->m_nTypeResult			= RESULT_AREA;
			pBM->m_sUnit				= "㎡";
			pBM->m_nShape				= SHAPE_TV_RECTANGLE;
			pBM->m_dV[WidthLower]		= Round(toM(dLenSlab),3);
			pBM->m_dV[Length]			= Round(toM(dWidth),3);
			pBM->m_dwSide				= SIDE_LOWER;
			pBM->CalculateResult();
			// 예외적인 출력양식
			pBM->m_nLevelMidClass		= 2;
			m_pArrBMBridgeStd.Add(pBM);

			if(nCount > 40 || dHeight <=0)
				break;
			nCount++;
		}
	}

	//ARCBRIDGE-3141 추가 동바리 타입이 육교용일 경우 각 지간의 수량을 추가해 준다.
	if(pBri->m_nTypePost == 0)
	{
		for(long jigan = 0; jigan < pBri->m_nQtyJigan; jigan++)
		{
			nNum	= GetTvAreaShapePostJigan(&m_pArrAreaConc, jigan, nNumLast);
			pBri->GetTvPostJigan(tvArrUpper, tvArrLeft, tvArrRight, tvArrMid, tvArrArch, tvArrMid2, jigan);

			if(tvArrLeft.GetSize() ==0 && tvArrRight.GetSize() == 0 && tvArrMid.GetSize() == 0)
				continue;

			//좌우 측에 육교용이 있는지 확인
			BOOL bFindLeftPost = FALSE, bFindRightPost = FALSE;	
			long nNumLeft = -1, nNumRight = -1;
			for (long ix =0; ix < m_pArrAreaConc.GetSize(); ix++)
			{
				CTwinVectorAreaShape *pArea	= m_pArrAreaConc.GetAt(ix);
				if(pArea == NULL) continue;
				if(pArea->m_tvArr.GetSize() == 0) continue;
				if(pArea->m_sInfo.Find("좌측") > -1 && bFindLeftPost == FALSE)
				{
					bFindLeftPost = TRUE; 
					nNumLeft = pArea->m_nNum;
				}
				else if(pArea->m_sInfo.Find("우측") > -1 && bFindRightPost == FALSE)
				{
					bFindRightPost = TRUE;
					nNumRight = pArea->m_nNum;
				}				
			}

			for (long nLR =0; nLR < 2; nLR++)
			{
				BOOL bPost_Left = nLR == iLEFT;
				if(bFindLeftPost == FALSE && bPost_Left) 
					continue;
				else if(bFindRightPost ==FALSE && bPost_Left == FALSE)
					continue;

				CDPoint xyL_Bot, xyL_Top, xyR_Bot, xyR_Top;
				//헌치부까지 포함해서 수직으로 선 두개를 그림 -> 이후 높이별로 만듦
				CTwinVectorArray tvArrPost;
				tvArrPost.AddFromTvArray(bPost_Left? tvArrLeft : tvArrRight);

				xyL_Bot.x = tvArrPost.GetXyLeft().x;
				xyL_Bot.y = tvArrPost.GetXyBottom().y;
				tvLeft.m_v1 = xyL_Bot;

				xyL_Top.x = xyL_Bot.x;
				xyL_Top.y = tvArrPost.GetXyTop().y;				
				tvLeft.m_v2 = xyL_Top;

				xyR_Bot.x = tvArrPost.GetXyRight().x;
				xyR_Bot.y = tvArrPost.GetXyBottom().y;
				tvRight.m_v1 = xyR_Bot;

				xyR_Top.x = xyR_Bot.x;
				xyR_Top.y = tvArrPost.GetXyTop().y;				
				tvRight.m_v2 = xyR_Top;
				

				double dYBase	= tvLeft.GetXyBottom().y;
				double dYCur	= dYBase;
				double dPrevH	= 0;

				bFirst		= TRUE;				
				while(1)
				{
					dYCur += bFirst ? dFirstHeight : dSecondHeight;

					bFirst	= FALSE;
					double dMaxEL = max(tvLeft.GetXyTop().y, tvRight.GetXyTop().y);
					if(tvLeft.GetXyBottom().y >= dYCur) continue;
					if(dMaxEL <= dPrevH && dMaxEL <= dYCur) break;

					// 도형의 좌우측 교차점 찾기
					GetXyMatchLineAndLine(tvLeft.m_v1, tvLeft.GetXyDir(), CDPoint(0, dYCur), CDPoint(1, 0), xyMatchLeft);
					GetXyMatchLineAndLine(tvRight.m_v1, tvRight.GetXyDir(), CDPoint(0, dYCur), CDPoint(1, 0), xyMatchRight);

					CBMData *pBM					= new CBMData;
					pBM->m_nNumbering			= bPost_Left? nNumLeft : nNumRight;
					pBM->m_nIdxBigClass			= BM_BIG_POST;
					pBM->m_sBigClass				= GetStringBigClass(pBM->m_nIdxBigClass);
					pBM->m_nLevelTotal			= GetLevelTotal(pBM->m_nIdxBigClass); 
					pBM->m_nIdxSmallClass		= BM_SMALL_POST_HORI_CONNECT;
					pBM->m_sSmallClass			= GetStringSmallClass(pBM->m_nIdxSmallClass);
					pBM->m_sNoteHead.Format("수평연결재(h=%.1f초과시 높이 %.1fm마다 설치)", toM(dFirstHeight), toM(dSecondHeight));
					pBM->m_nTypeResult			= RESULT_AREA;
					pBM->m_sUnit					= "㎡";
					pBM->m_nShape				= SHAPE_TV_RECTANGLE;
					pBM->m_dV[WidthLower]		= Round(toM(fabs(xyMatchRight.x-xyMatchLeft.x)),3);
					pBM->m_dV[Length]			= Round(toM(dWidth),3);
					pBM->m_dwSide				= SIDE_LOWER;
					pBM->CalculateResult();
					// 예외적인 출력양식
					pBM->m_nLevelMidClass		= 2;
					dPrevH = dYCur;
					m_pArrBMBridgeStd.Add(pBM);
				}
			}			
		}		
	}

	pBri->GetTvVertSection_Std(0, FALSE, FALSE, TRUE);
}

// 현재 영역의 좌우 정보를 가져옴
// 시종점 외측벽체 외측면이 있는지 알아야 됨(벽체 거푸집 수량 산출시 날개벽 두께부분을 
// 빼야 되는지 판단하기 위함)
// nTypeLeft, nTypeRight
// -2 : 시점측브라켓 측면
// -1 : 시점측
// 0 : 일반단면
// 1 : 종점측
// 2 : 종점측브라켓 측면
void COutBM::GetInfoPosAreaConc(CTwinVectorAreaShape *pArea, long &nTypeLeft, long &nTypeRight)
{
	nTypeLeft	= 0;
	nTypeRight	= 0;

	// 시점측임을 확인하는 정보
	const long nCountStt	= 4;
	CString sInfoStt[nCountStt]	= {"시점브라켓상부측면", "시점브라켓측면", "시점브라켓헌치", "시점벽체좌측면"};

	// 종점측임을 확인하는 정보
	const long nCountEnd	= 4;
	CString sInfoEnd[nCountEnd]	= {"종점브라켓상부측면", "종점브라켓측면", "종점브라켓헌치", "종점벽체우측면"};

	CTwinVector tv;
	// 시점 정보가 있는지 검사
	long i = 0; for(i = 0; i < nCountStt; i++)
	{
		if(pArea->m_tvArr.GetTvByInfo(sInfoStt[i], tv))
		{
			if(sInfoStt[i] == "시점브라켓측면")
				nTypeLeft	= -2;
			else
				nTypeLeft	= -1;
			break;
		}
	}

	// 종점 정보가 있는지 검사
	for(i = 0; i < nCountEnd; i++)
	{
		if(pArea->m_tvArr.GetTvByInfo(sInfoEnd[i], tv))
		{
			if(sInfoEnd[i] == "종점브라켓측면")
				nTypeRight	= 2;
			else
				nTypeRight	= 1;
			break;
		}
	}
}

// 다웰바 수량 산출
// 접속슬래브 시종점 좌우측에 있음.
void COutBM::MakeBMDowelBar()
{
	CRcBridgeRebar	*pBri	= GetRcBridgeRebar(TRUE);
	if(!pBri) return;
	
	CApsApp *pAps	= NULL;
	long nIdxItemClass	= 0;
	CString strBar = pBri->GetDBar_ForDowelBar() ? _T("D") : _T("H");
	long stt = 0; for(stt = 0; stt < 2; stt++)
	{
		BOOL bStt	= stt == iSTT ? TRUE : FALSE;

		for(long left = 0; left < 2; left++)
		{
			BOOL bLeft	= left == iLEFT ? TRUE : FALSE;
			pAps	= bStt ? &pBri->m_apsStt[left] : &pBri->m_apsEnd[left];
			if(!pAps->m_bIs) continue;

			long nIdxDowel = -1;
			if(pAps->m_pArrRebar.GetSize() <= RB_APS_DW3)
			{
				CString sDowel = _T("다웰바");
				for (long ix =0; ix< pAps->m_pArrRebar.GetSize(); ix++)
				{
					CRebar *pRBFind = pAps->m_pArrRebar.GetAt(ix);
					if(pRBFind->m_sDescription.Find(sDowel) > -1)
					{
						nIdxDowel = ix;
						break;
					}
				}
			}
			else
			{
				nIdxDowel = RB_APS_DW3;
			}

			if(nIdxDowel == -1)
				continue;

			CString sItemClass	= _T("");
			CRebar *pRB	= pAps->m_pArrRebar[nIdxDowel];
			if(pRB == NULL) continue;

			pRB->m_sDescription = _T("다웰바");
			switch(stt)
			{
				case iSTT : sItemClass = bLeft ? "시점(좌측)" : "시점(우측)"; break;
				case iEND : sItemClass = bLeft ? "종점(좌측)" : "종점(우측)"; break;
			}
			CBMData *pBM				= new CBMData;
			pBM->m_nIdxBigClass			= BM_BIG_DOWELBAR;
			pBM->m_sBigClass			= GetStringBigClass(pBM->m_nIdxBigClass);
			pBM->m_nLevelTotal			= GetLevelTotal(pBM->m_nIdxBigClass); 
			pBM->m_nIdxMidClass			= BM_MID_DOWELBAR;
			pBM->m_sMidClass.Format("%s%.0f, L=%.0fmm, CTC=%.0fmm", strBar, pRB->m_dDia, pRB->GetLength(), pAps->m_dCtcDowelBar);
			pBM->m_nIdxSmallClass		= BM_SMALL_DOWELBAR;	// 간단
			pBM->m_sSmallClass			= GetStringSmallClass(pBM->m_nIdxSmallClass);
			pBM->m_nIdxItemClass		= nIdxItemClass++;
			pBM->m_sItemClass			= sItemClass;
			pBM->m_nTypeResult			= RESULT_EA;
			pBM->m_sForm.Format("%d NR", pRB->m_nEa);
			pBM->m_dV[Result]			= pRB->m_nEa;
			pBM->m_sUnit				= "NR";
			
			m_pArrBMApsStd[stt][left].Add(pBM);
		}
	}
}

// 콘크리트(접속슬래브)
// 평면을 나눠서 구하자.
// CTwinVector tvBackFillStt, CTwinVector tvBackFillEnd: 뒷채움 공제용으로 구할때 뒷채움 선위치
// 접속슬래브 원래 단면도와 뒷채움에 포함된 단면도의 양의 비를 결과값에 곱해준다.
void COutBM::MakeBMConcreteAps(BOOL bForDeductOfBackFill, CTwinVector tvBackFillStt, CTwinVector tvBackFillEnd)
{

	// 공제용일때는 원래 교량으로 구해야 됨.
	CRcBridgeRebar	*pBri	= GetRcBridgeRebar(bForDeductOfBackFill);
	if(!pBri) return;
	if(!pBri->m_bSelectBM_ApsCnsInclude) return;


	long nTypeBM = m_pStd->m_pARcBridgeDataStd->m_nTypeBMApply;

	CRebarPlacing rb;
	long nIdxItemClass	= 0;
	CString sItemClass	= _T("");
	long stt = 0; for(stt = 0; stt < 2; stt++)
	{
		BOOL bStt	= stt == iSTT ? TRUE : FALSE;
		long nJ		= bStt ? 0 : pBri->m_nQtyJigan;
		if(pBri->IsBoxType()) nJ = 0;
		for(long left = 0; left < 2; left++)
		{
			BOOL bLeft	= left == iLEFT ? TRUE : FALSE;

			for(long cns = -1; cns < 2; cns++)
			{
				BOOL bCns	= cns != -1;	// 완충슬래브인지?

				// 토공용인경우 완충슬래브 고려 안함
				if(bForDeductOfBackFill && bCns) continue;

				// 토공용으로 구하는 경우 토공수량에 추가한다.
				// 단 자세한 내용은 이 함수를 호출 하는 곳에서 수정을 해 줘야 됨.
				CTypedPtrArray <CObArray, CBMData*> *pArrBMStd	= NULL;
				CApsApp *pAps	= NULL;
				if(bCns)
				{
					pAps	= bStt ? &pBri->m_cnsStt[left][cns] : &pBri->m_cnsEnd[left][cns];
					pArrBMStd	= bForDeductOfBackFill ? &m_pStd->m_pArrOutBMTogong.GetAt(m_nBri)->m_pArrBMTogongStd[nJ] : &m_pArrBMCnsStd[stt][left][cns];
					sItemClass.Format("%s 완충슬래브(%s%d)", bStt ? "시점" : "종점", bLeft ? "좌" : "우", cns+1);
				}
				else
				{
					pAps	= bStt ? &pBri->m_apsStt[left] : &pBri->m_apsEnd[left];
					pArrBMStd	= bForDeductOfBackFill ? &m_pStd->m_pArrOutBMTogong.GetAt(m_nBri)->m_pArrBMTogongStd[nJ] : &m_pArrBMApsStd[stt][left];
					sItemClass.Format("%s 접속슬래브(%s)", bStt ? "시점" : "종점", bLeft ? "좌" : "우");
				}
				if(!pAps->m_bIs) continue;

				///////////////////// 뒷채움 공제용으로 쓸때 필요함 /////////////
				CTwinVector tvBack	= bStt ? tvBackFillStt : tvBackFillEnd;
				CTwinVectorArray tvArrBackFill;
				CDRect rectAps	= pAps->m_tvArrSide.GetRect();
				if(bStt)
				{
					CTwinVector tv;
					tvArrBackFill.Add(tvBack);

					tv.m_v1	= tvBack.m_v2;
					tv.m_v2	= CDPoint(rectAps.right + 10, tv.m_v1.y);
					tvArrBackFill.Add(tv);

					tv.m_v1	= tv.m_v2;
					tv.m_v2.y	= tvBack.m_v1.y;
					tvArrBackFill.Add(tv);

					tv.m_v1	= tv.m_v2;
					tv.m_v2	= tvBack.m_v1;
					tvArrBackFill.Add(tv);
				}
				else
				{
					CTwinVector tv;
					tvArrBackFill.Add(tvBack);
					tv.m_v1	= tvBack.GetXyBottom();
					tv.m_v2	= CDPoint(rectAps.left - 10, tv.m_v1.y);
					tvArrBackFill.Add(tv);

					tv.m_v1	= tv.m_v2;
					tv.m_v2.y	= tvBack.GetXyTop().y;
					tvArrBackFill.Add(tv);

					tv.m_v1	= tv.m_v2;
					tv.m_v2	= tvBack.GetXyTop();
					tvArrBackFill.Add(tv);
				}
				///////////////////// 뒷채움 공제용으로 쓸때 필요함 /////////////

				double dH1	= pAps->m_dH1;
				double dH2	= pAps->m_dH2;
				double dL2	= pAps->m_dL2;
				double dL3	= pAps->m_dL3;

				// 헌치부
				// 상하면 평균 냄.
				if(!bCns && pAps->m_dH2 > 0)
				{
					// 공제용일때는 뒷채움에 포함된 단면도의 비율을 곱해준다.
					double dRate	= 1;
					if(bForDeductOfBackFill && !bCns)
					{
						CTwinVectorArray tvArrSec;
						CTwinVectorArray tvArrSecIncludeBackFill;
						double dL1	= pAps->m_dL1;
						double dH1	= pAps->m_dH1;
						pAps->m_dL1	= 0;
						pAps->m_dH1	= 0;

						pAps->GetTvSide();

						pAps->m_dL1	= dL1;
						pAps->m_dH1	= dH1;

						tvArrSec	= pAps->m_tvArrSide;
						tvArrSecIncludeBackFill	= pAps->m_tvArrSide;

						pAps->GetTvSide();

						rb.SubTractTvArrAndTvArr(tvArrSec, tvArrBackFill, tvArrSecIncludeBackFill);
						rb.RemoveMinLengthLine(tvArrSec, 0);
						rb.RemoveMinLengthLine(tvArrSecIncludeBackFill, 0);

						tvArrSec.Sort();
						tvArrSecIncludeBackFill.Sort();

						if(tvArrSecIncludeBackFill.GetSize() == 0)
							dRate	= 0;
						else
						{
							double dArea	= rb.GetAreaTvArr(tvArrSec);
							double dAreaIncludeBackFill	= rb.GetAreaTvArr(tvArrSecIncludeBackFill);
							dRate	= dAreaIncludeBackFill / dArea;
						}
					}

					CTwinVector tvLeft, tvRight;
					CTwinVector tvUpper, tvLower, tvOrg;
					pAps->m_tvArrPlan.GetTvByInfo("기준면", tvOrg);
					pAps->m_tvArrPlan.GetTvByInfo("접속슬래브좌측", tvLeft);
					pAps->m_tvArrPlan.GetTvByInfo("접속슬래브우측", tvRight);
					tvUpper	= tvOrg;
					tvLower	= tvOrg;
					Offset(tvUpper.m_v1, tvUpper.m_v2, -(dL2 + dL3));
					Offset(tvLower.m_v1, tvLower.m_v2, -dL3);

					// 헌치상면 사디리꼴 
					CDPoint xyMatch1(0, 0), xyMatch2(0, 0);
					GetXyMatchLineAndLine(tvUpper.m_v1, tvUpper.GetXyDir(), tvLeft.m_v1, tvLeft.GetXyDir(), xyMatch1);
					GetXyMatchLineAndLine(tvUpper.m_v1, tvUpper.GetXyDir(), tvRight.m_v1, tvRight.GetXyDir(), xyMatch2);
					tvUpper.m_v1	= xyMatch1;
					tvUpper.m_v2	= xyMatch2;

					// 헌치하면 사다리꼴
					GetXyMatchLineAndLine(tvLower.m_v1, tvLower.GetXyDir(), tvLeft.m_v1, tvLeft.GetXyDir(), xyMatch1);
					GetXyMatchLineAndLine(tvLower.m_v1, tvLower.GetXyDir(), tvRight.m_v1, tvRight.GetXyDir(), xyMatch2);
					tvLower.m_v1	= xyMatch1;
					tvLower.m_v2	= xyMatch2;

					double dWidthUpper	= toM(tvUpper.GetLength());
					double dWidthLower	= toM(tvLower.GetLength());
					double dWidthOrg	= toM(tvOrg.GetLength());
					double dHeightUpper	= toM(dL2+dL3);
					double dHeightLower	= toM(dL3);
					double dHeight		= toM(dH2);

					CBMData *pBMHunch				= new CBMData;
					// (ARCBRIDGE-1481) 헌치부에도 넘버링 추가
					pBMHunch->m_nLevelTotal			= 2;
					pBMHunch->m_nNumbering			= 1;
					pBMHunch->m_nIdxBigClass		= BM_BIG_CONCRETE;
					pBMHunch->m_sBigClass			= GetStringBigClass(pBMHunch->m_nIdxBigClass);
					pBMHunch->m_nIdxMidClass		= BM_MID_CONCRETE_MAIN;
					pBMHunch->m_sMidClass			= GetStringMidClass(pBMHunch->m_nIdxMidClass, pAps, bStt? ePartApsStt : ePartApsEnd);
					if(pBri->IsBoxType())
					{
						pBMHunch->m_nIdxPartClass		= bStt ? 0 : 1;
						pBMHunch->m_sPartClass	    	= bStt ? "시점" : "종점";
					}
					pBMHunch->m_nIdxSmallClass		= (nTypeBM == TYPE_BM_NORMAL)? BM_SMALL_CONCRETE_APS : BM_SMALL_CONCRETE_FLOOR;
					pBMHunch->m_sSmallClass			= GetStringSmallClass(pBMHunch->m_nIdxSmallClass);
					pBMHunch->m_nIdxItemClass		= nIdxItemClass++;
					pBMHunch->m_sItemClass			= sItemClass;
					pBMHunch->m_nTypeResult			= RESULT_VOLUMN;
					pBMHunch->m_sUnit				= "㎥";
					pBMHunch->m_sForm.Format("(((%.3f + %.3f) / 2 x %.3f) + ((%.3f + %.3f) / 2 x %.3f)) x %.3f / 2", 
						dWidthUpper, dWidthOrg, dHeightUpper, 
						dWidthLower, dWidthOrg, dHeightLower, dHeight);
					pBMHunch->m_dV[Result]	= Round( (((dWidthUpper+dWidthOrg)/2*dHeightUpper) + ((dWidthLower+dWidthOrg)/2*dHeightLower)) * dHeight / 2 , 3);

					if(!Compare(dRate, 1.0, "="))
					{
						CString sForm	= _T("");
						sForm.Format("%s x %.3f%%", pBMHunch->GetStringForm(TRUE), dRate*100);
						pBMHunch->m_nShape	= -1;
						pBMHunch->m_sForm	= sForm;
						pBMHunch->m_sFormForCompare	= sForm;
						pBMHunch->SetResult(pBMHunch->m_dV[Result] * dRate);
					}

					pArrBMStd->Add(pBMHunch);

					if(pBMHunch->GetResult() == 0)
					{
						delete pBMHunch;
						pArrBMStd->RemoveAt(pArrBMStd->GetSize()-1);
					}
				}
				
				// lean = 0 : 슬래브 콘크리트
				// lean = 1 : 버림콘크리트
				for(long lean = 0; lean < 2; lean++)
				{
					BOOL bLean	= lean == 0 ? TRUE : FALSE;

					// 완충슬래브는 버림콘크리트 없음
					if(bLean && bCns) continue;
					// 버림콘크리트는 헌치부가 없을때만 가능하며 
					// 완충슬래브가 있다면 버림콘크리트의 영역이 아니라 일반 슬래브 영역으로 
					// 버림콘크리트 양을 구해야 된다.
					if(bLean)
					{
						if(pAps->m_dH2 > 0) continue;
						if((bStt && pBri->m_cnsStt[left][0].m_bIs) && (!bStt && pBri->m_cnsEnd[left][0].m_bIs)) bLean = FALSE;
					}

					// 접속슬래브 평면 영역 구하기 .
					GetTvAreaShapeApsPlane(&m_pArrAreaConc, bStt, bLeft, bLean);

					// 공제용일때는 뒷채움에 포함된 단면도의 비율을 곱해준다.
					double dRate	= 1;
					if(bForDeductOfBackFill && !bCns)
					{
						CTwinVectorArray tvArrSec;
						CTwinVectorArray tvArrSecIncludeBackFill;
						if(!bLean)
						{
							pAps->m_dL2	= 0;
							pAps->m_dL3	= 0;
							pAps->m_dH2	= 0;
						}

						pAps->GetTvSide();

						pAps->m_dL2	= dL2;
						pAps->m_dL3	= dL3;
						pAps->m_dH2	= dH2;

						tvArrSec	= bLean ? pAps->m_tvArrSideLean : pAps->m_tvArrSide;
						tvArrSecIncludeBackFill	= bLean ? pAps->m_tvArrSideLean : pAps->m_tvArrSide;

						pAps->GetTvSide();

						rb.SubTractTvArrAndTvArr(tvArrSec, tvArrBackFill, tvArrSecIncludeBackFill);
						rb.RemoveMinLengthLine(tvArrSec, 0);
						rb.RemoveMinLengthLine(tvArrSecIncludeBackFill, 0);

						tvArrSec.Sort();
						tvArrSecIncludeBackFill.Sort();

						if(tvArrSecIncludeBackFill.GetSize() == 0)
							dRate	= 0;
						else
						{
							double dArea	= rb.GetAreaTvArr(tvArrSec);
							double dAreaIncludeBackFill	= rb.GetAreaTvArr(tvArrSecIncludeBackFill);
							dRate	= dAreaIncludeBackFill / dArea;
						}
					}


					// 헌치부 제외한 슬래브/버림콘크리트
					long nSize	= m_pArrAreaConc.GetSize();
					long i = 0; for(i = 0; i < nSize; i++)
					{
						CTwinVectorAreaShape *pArea	= m_pArrAreaConc.GetAt(i);

						CBMData *pBM				= new CBMData;
						// (ARCBRIDGE-1481) 헌치부에도 넘버링 추가
						pBM->m_nNumbering			= bLean ? 0 : pArea->m_nNum;
						if(!bLean && !bCns && pAps->m_dH2 > 0)
						{
							pBM->m_nNumbering += 1;
						}
						pBM->m_nLevelTotal			= 2;
						pBM->m_nIdxBigClass			= BM_BIG_CONCRETE;
						pBM->m_sBigClass			= GetStringBigClass(pBM->m_nIdxBigClass);
						pBM->m_nIdxMidClass			= bLean ? BM_MID_CONCRETE_LEAN : BM_MID_CONCRETE_MAIN;
						pBM->m_sMidClass			= GetStringMidClass(pBM->m_nIdxMidClass, pAps, bStt? ePartApsStt : ePartApsEnd);
						if(pBri->IsBoxType())
						{
							pBM->m_nIdxPartClass		= bStt ? 0 : 1;
							pBM->m_sPartClass	    	= bStt ? "시점" : "종점";
						}
						pBM->m_nIdxSmallClass		= bLean ? (BM_SMALL_CONCRETE_LEAN) : (bCns ? BM_SMALL_CONCRETE_CNS : BM_SMALL_CONCRETE_APS);
						if(bLean == FALSE && nTypeBM != TYPE_BM_NORMAL)
							pBM->m_nIdxSmallClass = BM_SMALL_CONCRETE_FLOOR;

						pBM->m_sSmallClass			= GetStringSmallClass(pBM->m_nIdxSmallClass);
						pBM->m_nIdxItemClass		= nIdxItemClass++;
						pBM->m_sItemClass			= sItemClass;
						pBM->m_nTypeResult			= RESULT_VOLUMN;
						pBM->m_sUnit				= "㎥";
						pBM->m_nShape				= pArea->m_nTypeShape;
						pBM->m_dV[WidthLower]		= Round(toM(pArea->m_dWidthLower),3);
						pBM->m_dV[HeightLeft]		= Round(pBM->m_dV[WidthLower],3);
						pBM->m_dV[WidthUpper]		= Round(toM(pArea->m_dWidthUpper),3);
						pBM->m_dV[HeightRight]		= Round(pBM->m_dV[WidthUpper],3);
						pBM->m_dV[WidthDiffLeft]	= Round(toM(pArea->m_dWidthDiffLeft),3);
						pBM->m_dV[WidthDiffRight]	= Round(toM(pArea->m_dWidthDiffRight),3);
						pBM->m_dV[Height]			= Round(toM(pArea->m_dHeight),3);
						pBM->m_dV[Width]			= Round(pBM->m_dV[Height],3);
						pBM->m_dV[Length]			= bLean ? Round(toM(100),3) : Round(toM(dH1),3);
						pBM->CalculateResult();

						if(!Compare(dRate, 1.0, "="))
						{
							CString sForm	= _T("");
							sForm.Format("%s x %.3f%%", pBM->GetStringForm(TRUE), dRate*100);
							pBM->m_nShape	= -1;
							pBM->m_sForm	= sForm;
							pBM->m_sFormForCompare	= sForm;
							pBM->SetResult(pBM->GetResult() * dRate);
						}
						
						pArrBMStd->Add(pBM);
						if(pBM->GetResult() == 0)
						{
							delete pBM;
							pArrBMStd->RemoveAt(pArrBMStd->GetSize()-1);
						}
					}
				}
			}
		}
	}
}

// 접속슬래브 산출서 첫페이지 삽도
void COutBM::DrawFirstPageAps(CHgBaseBMStd *pBaseBMStd, BOOL bStt, BOOL bLeft)
{
	if(!pBaseBMStd) return;

	CRcBridgeRebar *pBri	= GetRcBridgeRebar();
	if(!pBri) return;

	CARcBridgeDBStandard DBStd(pBri, m_pStd->m_pARoadOptionStd, m_pStd->m_pARcBridgeDataStd);
	DBStd.m_pDrawAps->m_bApplyDimSettingOfBaseDom	= TRUE;
	CRebarPlacing rb;

	// 실제로 출력되었으면 하는 사이즈
	double dWidthOrg			= 200;	// 도면 가로크기

	long nRow	= 5;
	long left	= bLeft ? iLEFT : iRIGHT;
	CApsApp *pAps	= bStt ? &pBri->m_apsStt[left] : &pBri->m_apsEnd[left];
	if(!pAps->m_bIs) return;

	// 스케일을 결정하자!!! ////////////////////////////////////
	// 그걸로 종단면과 관련된 도면을 그려내자.
	CDomyun domTmp;
	domTmp.SetCWnd(m_pStd->m_pView);
	domTmp.SetScaleDim(1);
	domTmp.SetDimTxt(1);

	DBStd.m_pDrawAps->DrawPlane(&domTmp, pAps);
	DBStd.m_pDrawAps->DimPlaneSide(&domTmp, pAps, 0, FALSE);
	DBStd.m_pDrawAps->DimPlaneTop(&domTmp, pAps, 0, FALSE);
	/////////////////////////////////////////////////////////////////////////

	
	CDomyun domPlane;
	domPlane.SetCWnd(m_pStd->m_pView);

	// 도면 설정 ////////////////////
	double dScale	= pBaseBMStd->GetScale(&domTmp, dWidthOrg);
	pBaseBMStd->SetDomyunValue(&domPlane, dWidthOrg, &domTmp);
	///////////////////////////////////

	// 영역을 구해옴
	GetTvAreaShapeApsPlane(&m_pArrAreaConc, bStt, bLeft);

	// 구해온 영역을 그린다.
	pBaseBMStd->DrawAreaSectionNumber(&domPlane, &m_pArrAreaConc, 1);

	// 치수기입
	pBaseBMStd->DimAreaSection(&domPlane, &m_pArrAreaConc, 2, TRUE);
	pBaseBMStd->DimAreaSection(&domPlane, &m_pArrAreaConc, 3, TRUE);
	
	domPlane.SetCalcExtRect();
	
	// 그렸다면 원하는 크기로 사이즈 조정
	domPlane.RedrawByScale(dScale, TRUE);
	
	// 엑셀에 삽입
	domPlane.SetCalcExtRect();

	// 그린걸 엑셀에 삽입
	long nTaAlign	= TA_CENTER;

	CString sTitle	= _T("");
	sTitle.Format("%s 접속슬래브(%s)", bStt ? "시점측" : "종점측", bLeft ? "좌" : "우");

	CDoubleArray dArrTmp;
	nRow	= pBaseBMStd->InsertDomyun(nRow, nTaAlign, &domPlane, dArrTmp, domPlane.GetExtRect().Width(), domPlane.GetExtRect().Height(), sTitle)+3;

	// 접속슬래브 단면
	CDomyun domSec;
	domSec.SetCWnd(m_pStd->m_pView);

	// 도면 설정 ////////////////////
	dScale	= pBaseBMStd->GetScale(&domTmp, dWidthOrg);
	pBaseBMStd->SetDomyunValue(&domSec, dWidthOrg, &domTmp);
	///////////////////////////////////

	// 영역을 구해옴
	GetTvAreaShapeApsSection(&m_pArrAreaConc, bStt, bLeft);
	
	// 구해온 영역을 그린다.
	pBaseBMStd->DrawAreaSectionNumber(&domSec, &m_pArrAreaConc, 1);

	// 치수기입
	pBaseBMStd->DimAreaSection(&domSec, &m_pArrAreaConc, 2, TRUE);
	pBaseBMStd->DimAreaSection(&domSec, &m_pArrAreaConc, 3, TRUE);
	
	// 그렸다면 원하는 크기로 사이즈 조정
	domSec.RedrawByScale(dScale, TRUE);
	
	// 엑셀에 삽입
	domSec.SetCalcExtRect();

	// 그린걸 엑셀에 삽입
	nTaAlign	= TA_CENTER;
	sTitle	= _T("");


	nRow	= pBaseBMStd->InsertDomyun(nRow, nTaAlign, &domSec, dArrTmp, domSec.GetExtRect().Width(), domSec.GetExtRect().Height(), sTitle)+3;

	DBStd.m_bApplyDimSettingOfBaseDom	= FALSE;
}

// 접속슬래브 평면 영역을 나눈다.
// BOOL bLean : 버림콘크리트 영역을 구하는지?
// 현재 버림콘크리트의 두께나 접속슬래브에서의 확장길이는 100으로 고정
long COutBM::GetTvAreaShapeApsPlane(CTypedPtrArray <CObArray, CTwinVectorAreaShape*> *pArrAreaConc, BOOL bStt, BOOL bLeft, BOOL bLean)
{
	AhTPADelete(pArrAreaConc,	(CTwinVectorAreaShape*)0);

	CRcBridgeRebar *pBri	= GetRcBridgeRebar();
	if(!pBri) return 0;
	CRebarPlacing rb;

	CTwinVectorArray tvArrPlane, tvArrResult;
	CDoubleArray dArrAngleSu;

	CDPoint xyRot	= pBri->GetXyDirRotateDomByBridgeSttEndXy();
	long left	= bLeft ? iLEFT : iRIGHT;
	CApsApp *pAps	= bStt ? &pBri->m_apsStt[left] : &pBri->m_apsEnd[left];
	if(!pAps->m_bIs) return 0;

	pAps->GetTvPlan(FALSE, FALSE, bLean);	//ARCBRIDGE-2493 헌치부 수량을 구하는 것으로 , pAps->GetTvPlan(FALSE, TRUE, bLean);
	tvArrPlane	= pAps->m_tvArrPlan;
	CDPoint xyDirRot	= ToDPointFrDegree(pBri->GetXyDirRotateDomByBridgeSttEndXy().GetAngleDegree()+pAps->m_xyDirPlane.GetAngleDegree()-90);
	if(!pAps->m_bLeft)
		xyDirRot		= ToDPointFrDegree(xyDirRot.GetAngleDegree()-180);
	rb.RotateTvArr(tvArrPlane, CDPoint(0, 0), xyDirRot.GetAngleDegree());

	pAps->GetTvPlan(FALSE, TRUE);		// 접속슬래브의원래 모양을 복구 해놔야 좋음

	rb.RotateTvArr(tvArrPlane, CDPoint(0, 0), xyRot.Rotate90().GetAngleDegree()*-1);
	rb.SeperatorTwinVectorArea(tvArrPlane, tvArrResult, dArrAngleSu, 1);
	
	m_pStd->ConvertTvArrToTvArrAreaShape(tvArrResult, dArrAngleSu, pArrAreaConc, 1, FALSE);

	return 0;
}


// 접속슬래브 단면
long COutBM::GetTvAreaShapeApsSection(CTypedPtrArray <CObArray, CTwinVectorAreaShape*> *pArrAreaConc, BOOL bStt, BOOL bLeft)
{
	AhTPADelete(pArrAreaConc,	(CTwinVectorAreaShape*)0);

	CRcBridgeRebar *pBri	= GetRcBridgeRebar();
	if(!pBri) return 0;
	CRebarPlacing rb;

	CTwinVectorArray tvArrSection, tvArrResult;
	CDoubleArray dArrAngleSu;

	long left	= bLeft ? iLEFT : iRIGHT;
	CApsApp *pAps	= bStt ? &pBri->m_apsStt[left] : &pBri->m_apsEnd[left];
	if(!pAps->m_bIs) return 0;

	pAps->GetTvSide(FALSE);
	tvArrSection	= pAps->m_tvArrSide;
	rb.SeperatorTwinVectorArea(tvArrSection, tvArrResult, dArrAngleSu, 1);
	
	m_pStd->ConvertTvArrToTvArrAreaShape(tvArrResult, dArrAngleSu, pArrAreaConc, 1, FALSE);

	// 헌치부가 1번으로 구해진다. 헌치부가 있다면 2번이 되도록 순서를 바꿈.
	long i = 0; for(i = 0; i < pArrAreaConc->GetSize()-1; i++)
	{
		CTwinVectorAreaShape *pArea1	= pArrAreaConc->GetAt(i);
		long j = 0; for(j = i+1; j < pArrAreaConc->GetSize(); j++)
		{
			CTwinVectorAreaShape *pArea2	= pArrAreaConc->GetAt(j);

			CTwinVectorAreaShape tmp;
			tmp	= *pArea1;
			*pArea1	= *pArea2;
			*pArea2	= tmp;
		}
	}

	return 0;
}

// 비계 본체
void COutBM::MakeBMScaffoldMain()
{
	CRcBridgeRebar *pBri	= GetRcBridgeRebar();
	if(!pBri) return;
	
	CDPoint vAng = pBri->GetAngleJijum(0);

	CTwinVectorArray tvArrScaffold, tvArrCenter;
	CTwinVectorArray tvArrScaffoldSec;

	long stt = 0; for(stt = 0; stt < 2; stt++)
	{
		BOOL bStt	= stt == iSTT ? TRUE : FALSE;
		pBri->GetTvScaffoldPlane(tvArrScaffold, tvArrCenter, bStt);
		pBri->GetTvScaffoldSection(tvArrScaffoldSec, bStt);

		CDRect rt = tvArrScaffoldSec.GetRect();
		double dHeight	= fabs(tvArrScaffoldSec.GetRect().Height());

		long i = 0; for(i = 0; i < tvArrCenter.GetSize(); i++)
		{
			CBMData *pBM			= new CBMData;
			pBM->m_nLevelTotal		= 2;
			pBM->m_nIdxBigClass		= BM_BIG_SCAFFOLD;
			pBM->m_sBigClass		= GetStringBigClass(pBM->m_nIdxBigClass);
			pBM->m_nIdxMidClass		= BM_MID_SCAFFOLD_MAIN;
			pBM->m_sMidClass		= GetStringMidClass(pBM->m_nIdxMidClass);
			pBM->m_nIdxSmallClass	= BM_SMALL_SCAFFOLD_MAIN;
			pBM->m_sSmallClass		= GetStringSmallClass(pBM->m_nIdxSmallClass);
			pBM->m_nTypeResult		= RESULT_AREA;
			pBM->m_sUnit			= "㎡";
			pBM->m_nShape			= SHAPE_TV_RECTANGLE;
			//pBM->m_dV[WidthLower]	= Round(toM(tvArrCenter.GetAt(i).GetLength()),3);
			pBM->m_dV[Height]		= Round(toM(tvArrCenter.GetAt(i).GetLength()), 3);//Round(toM(pBri->GetWidthSlabAct(pBri->m_dStationBridgeStt, vAng, 0, FALSE)),3);
			pBM->m_dV[WidthLower]	= Round(toM(dHeight),3);
			pBM->m_dwSide			= SIDE_FRONT;
			pBM->CalculateResult();
			
			m_pArrBMBridgeStd.Add(pBM);	
		}
	}
}

// 비계 날개벽
void COutBM::MakeBMScaffoldWing()
{
	CRcBridgeRebar *pBri	= GetRcBridgeRebar();
	if(!pBri) return;

	// 시종점 좌우측 날개벽
	CTwinVectorArray tvArrOut, tvArrCenOut, tvArrSideOut;
	CTwinVectorArray tvArrIn, tvArrCenIn ,tvArrSideIn;

	CTwinVectorArray tvArrScaffoldSec;
	CString sNoteTail	= _T("");
	CString sStt		= _T("");
	CString sLeft		= _T("");

	long stt = 0; for(stt = 0; stt < 2; stt++)
	{
		BOOL bStt	= stt == iSTT ? TRUE : FALSE;
		sStt	= bStt ? "시점측" : "종점측";

		pBri->GetTvScaffoldSection(tvArrScaffoldSec, bStt);	// 높이 산정용
		for(long left = 0; left < 2; left++)
		{
			BOOL bLeft	= left == iLEFT ? TRUE : FALSE;
			if(!pBri->IsWingWall(bStt, bLeft)) continue;

			sLeft	= bLeft ? "좌측" : "우측";
			
			CTypedPtrArray <CObArray, CBMData*> *pArrBMStd	= &m_pArrBMWingStd[stt][left];

			pBri->GetTvScaffoldWingPlane(tvArrOut, tvArrIn, tvArrCenOut, tvArrCenIn, tvArrSideOut, tvArrSideIn, bStt, bLeft);
			double dHeight	= fabs(tvArrScaffoldSec.GetRect().Height());

			// 날개벽 외측
			sNoteTail	= sStt + sLeft + "날개벽 외측";
			long i = 0; for(i = 0; i < tvArrCenOut.GetSize(); i++)
			{
				CBMData *pBM			= new CBMData;
				pBM->m_nLevelTotal		= 2;
				pBM->m_nIdxBigClass		= BM_BIG_SCAFFOLD;
				pBM->m_sBigClass		= GetStringBigClass(pBM->m_nIdxBigClass);
				pBM->m_nIdxMidClass		= BM_MID_SCAFFOLD_WING;
				pBM->m_sMidClass		= GetStringMidClass(pBM->m_nIdxMidClass);
				pBM->m_nIdxSmallClass	= BM_SMALL_SCAFFOLD_WING;
				pBM->m_sSmallClass		= GetStringSmallClass(pBM->m_nIdxSmallClass);
				pBM->m_nTypeResult		= RESULT_AREA;
				pBM->m_sUnit			= "㎡";
				pBM->m_nShape			= SHAPE_TV_RECTANGLE;
				pBM->m_dV[WidthLower]	= Round(toM(tvArrCenOut.GetAt(i).GetLength()),3);
				pBM->m_dV[Height]		= Round(toM(dHeight),3);
				pBM->m_dwSide			= SIDE_FRONT;
				pBM->CalculateResult();
				pArrBMStd->Add(pBM);	
			}

			// 날개벽 내측
			sNoteTail	= sStt + sLeft + "날개벽 내측";
			for(i = 0; i < tvArrCenIn.GetSize(); i++)
			{
				CBMData *pBM			= new CBMData;
				pBM->m_nLevelTotal		= 2;
				pBM->m_nIdxBigClass		= BM_BIG_SCAFFOLD;
				pBM->m_sBigClass		= GetStringBigClass(pBM->m_nIdxBigClass);
				pBM->m_nIdxMidClass		= BM_MID_SCAFFOLD_WING;
				pBM->m_sMidClass		= GetStringMidClass(pBM->m_nIdxMidClass);
				pBM->m_nIdxSmallClass	= BM_SMALL_SCAFFOLD_WING;
				pBM->m_sSmallClass		= GetStringSmallClass(pBM->m_nIdxSmallClass);
				pBM->m_nTypeResult		= RESULT_AREA;
				pBM->m_sUnit			= "㎡";
				pBM->m_nShape			= SHAPE_TV_RECTANGLE;
				pBM->m_dV[WidthLower]	= Round(toM(tvArrCenIn.GetAt(i).GetLength()),3);
				pBM->m_dV[Height]		= Round(toM(dHeight),3);
				pBM->m_dwSide			= SIDE_FRONT;
				pBM->CalculateResult();
				pArrBMStd->Add(pBM);	
			}

			//날개벽 측면
			if(pBri->m_nTypeScaffoldSideLocation == SCOFFOLD_WING_CON && tvArrSideIn.GetSize() > 0)
			{
				CBMData *pBM					= new CBMData;
				pBM->m_nLevelTotal			= 2;
				pBM->m_nIdxBigClass		= BM_BIG_SCAFFOLD;
				pBM->m_sBigClass				= GetStringBigClass(pBM->m_nIdxBigClass);
				pBM->m_nIdxMidClass		= BM_MID_SCAFFOLD_WING;
				pBM->m_sMidClass			= GetStringMidClass(pBM->m_nIdxMidClass);
				pBM->m_nIdxSmallClass		= BM_SMALL_SCAFFOLD_WING;
				pBM->m_sSmallClass			= GetStringSmallClass(pBM->m_nIdxSmallClass);
				pBM->m_nTypeResult			= RESULT_AREA;
				pBM->m_sUnit					= "㎡";
				pBM->m_nShape				= SHAPE_TV_RECTANGLE;
				pBM->m_dV[WidthLower]	= Round(toM(tvArrSideIn.GetAt(0).GetLength()),3);
				pBM->m_dV[Height]			= Round(toM(dHeight),3);
				pBM->m_dwSide				= SIDE_FRONT;
				pBM->CalculateResult();
				pArrBMStd->Add(pBM);	
			}
		}
	}
}

// 비계 본체 삽도 설정
void COutBM::SetInsertDomyunScaffoldMain()
{
	CRcBridgeRebar	*pBri	= GetRcBridgeRebar();
	if(!pBri) return;

	CTwinVectorArray tvArrVertSectionOld;
	tvArrVertSectionOld = pBri->m_tvArrVertSection;
	pBri->GetTvVertSection_Std(0, FALSE, FALSE);

	CARcBridgeDBStandard DBStd(pBri, m_pStd->m_pARoadOptionStd, m_pStd->m_pARcBridgeDataStd);
	DBStd.m_bApplyDimSettingOfBaseDom	= TRUE;
	CHgBaseBMStd baseBMStd;

	// 스케일 조정용 //////////
	CDomyun domTmp;
	domTmp.SetCWnd(m_pStd->m_pView);
	// 종단면 그림
	DBStd.m_pDrawVertSection->DrawVertSectionAll(&domTmp);
	DBStd.m_pDrawVertSection->DrawElEarthLine(&domTmp,0,0,FALSE,TRUE);
	// 스케일 조정용 //////////

	// 비계 본체 단면도  ////////////////////
	CDomyun domScaffoldSec;
	domScaffoldSec.SetCWnd(m_pStd->m_pView);

	// 치수설정
	double dWidthOrg	= 600;
	// 도면 설정 ////////////////////
	double dScale	= baseBMStd.GetScale(&domTmp, dWidthOrg);
	baseBMStd.SetDomyunValue(&domScaffoldSec, dWidthOrg, &domTmp);
	///////////////////////////////////

	// 종단면 그림
	DBStd.m_pDrawVertSection->DrawVertSectionAll(&domScaffoldSec);
	if(m_pStd->m_bDrawElEarth)
		DBStd.m_pDrawVertSection->DrawElEarthLine(&domScaffoldSec,0,0,FALSE,TRUE);

	// 시종점 비계 그림
	CRebarPlacing rb;
	long stt = 0; for(stt = 0; stt < 2; stt++)
	{
		BOOL bStt	= stt == iSTT ? TRUE : FALSE;

		CTwinVectorArray tvArr;
		CDPointArray xyArr;
		pBri->GetTvScaffoldSection(tvArr, bStt);
		DBStd.DrawTvArray(&domScaffoldSec, tvArr);
		CTwinVectorArray tvArrHatch;
		rb.GetTvArrayHatch(tvArr, tvArrHatch, 500, 45);
		DBStd.DrawTvArray(&domScaffoldSec, tvArrHatch);
	}

	// 비계 치수
	DBStd.m_pDrawVertSection->DimScaffoldForBM(&domScaffoldSec);

	// 삽도를 WidthOrg 사이즈로 변경
	domScaffoldSec.RedrawByScale(dScale);
	domScaffoldSec.SetCalcExtRect();

	// 비계수량 바로 앞에 추가
	long i = 0; for(i = 0; i < m_pArrBMStringBridge.GetSize(); i++)
	{
		CBMString *pBMString	= m_pArrBMStringBridge.GetAt(i);
		if(pBMString->m_sProcessKind1.Find("비계", 0) != -1)
		{
			pBMString->m_Dom	= domScaffoldSec;
			pBMString->m_bInsertDomyun	= TRUE;
			break;
		}
	}

	DBStd.m_bApplyDimSettingOfBaseDom	= FALSE;
	pBri->m_tvArrVertSection = tvArrVertSectionOld;
}

// 본체 스페이셔 수량
// 버림콘크리트이외의 철근 들어가는 부분은 모두 스페이셔 수량에 포함됨..
void COutBM::MakeBMSpacerMain()
{
	CRcBridgeRebar *pBri	= GetRcBridgeRebar();
	if(!pBri) return;

	// 벽체용 스페이서에 본체 측면 포함여부...
	// 단, 기초측면은 항상 포함됨.
	BOOL bIncludeSideForm	= TRUE;
	
	// 철근이 들어가는 부분의 거푸집을 모두 수집한다.
	// 버림콘크리트및 mass콘크리트 그리고 옵션에 따른 본체측면 거푸집을 제외한 
	// 수직,경사 -> 벽체용, 수평 -> 슬래브용
	CTypedPtrArray <CObArray, CBMData*> pArrFormVert;
	CTypedPtrArray <CObArray, CBMData*> pArrFormHor;
	BOOL bSlab	= FALSE;
	long nSizeBM	= m_pArrBMBridgeStd.GetSize();
	long i = 0; for(i = 0; i < nSizeBM; i++)
	{
		CBMData *pBM = m_pArrBMBridgeStd.GetAt(i);
		if(pBM->m_nIdxBigClass == BM_BIG_FORM)
		{
 			if(pBM->m_sInfo.Find("버림콘크리트거푸집") != -1) continue;
			if(pBM->m_sInfo.Find("MASS콘크리트거푸집") != -1) continue;
			if(!bIncludeSideForm)
				if(pBM->m_sInfo.Find("본체측면거푸집") != -1) continue;

			if(pBM->m_sSmallClass.Find("수평", 0) != -1 || pBM->m_sNoteTail=="아치리브하면") bSlab	= TRUE;
			else bSlab	= FALSE;

			CBMData *pForm = new CBMData;
			*pForm = *pBM;
			// 거푸짚 종류별로 묶어져 있으면 번호가 순서대로 정렬되지 않는다.
			// 그래서 여기서는 거푸집 종류를 모두 같은 것으로 해준다.
			pForm->m_sMidClass		= _T("");
			pForm->m_nIdxMidClass	= -1;
			pForm->m_sSmallClass	= _T("");
			pForm->m_nIdxSmallClass	= -1;
			pForm->m_sItemClass		= _T("");
			pForm->m_nIdxItemClass	= -1;
			pForm->m_sPartClass		= _T("");
			pForm->m_nIdxPartClass	= -1;
			pForm->m_sMeterClass	= _T("");
			pForm->m_nIdxMeterClass	= -1;

			if(bSlab) pArrFormHor.Add(pForm);
			else pArrFormVert.Add(pForm);
		}
		else if (pBM->m_nIdxBigClass == BM_BIG_STYROFOAM)
		{
			if (pBM->m_sInfo == "상하행공제")
			{
				CBMData *pForm = new CBMData;
				*pForm = *pBM;
				// 거푸짚 종류별로 묶어져 있으면 번호가 순서대로 정렬되지 않는다.
				// 그래서 여기서는 거푸집 종류를 모두 같은 것으로 해준다.
				pForm->m_sMidClass		= _T("");
				pForm->m_nIdxMidClass	= -1;
				pForm->m_sSmallClass	= _T("");
				pForm->m_nIdxSmallClass	= -1;
				pForm->m_sItemClass		= _T("");
				pForm->m_nIdxItemClass	= -1;
				pForm->m_sPartClass		= _T("");
				pForm->m_nIdxPartClass	= -1;
				pForm->m_sMeterClass	= _T("");
				pForm->m_nIdxMeterClass	= -1;

				pArrFormVert.Add(pForm);
			}
		}
	}
	
	// 정렬
	CHgBaseBMStd baseBM;
	baseBM.ShellSortBMList(&pArrFormVert, &CBMData::CompareBM);
	baseBM.ShellSortBMList(&pArrFormHor, &CBMData::CompareBM);
	
	// 벽체용 스페이셔 수량 생성
	if(pArrFormVert.GetSize() > 0)
	{
		CBMData *pWall				= new CBMData;
		pWall->m_nIdxBigClass		= BM_BIG_SPACER;
		pWall->m_sBigClass			= GetStringBigClass(pWall->m_nIdxBigClass);
		pWall->m_nLevelTotal		= GetLevelTotal(pWall->m_nIdxBigClass);
		pWall->m_nIdxSmallClass		= BM_SMALL_SPACER_WALL;
		pWall->m_sSmallClass		= GetStringSmallClass(pWall->m_nIdxSmallClass);
		pWall->m_nTypeResult		= RESULT_AREA;
		pWall->m_sUnit				= "㎡";
		pWall->m_sInfo				= "본체벽체용스페이셔";

		// 수집한 거푸집 수량들을 추가시킴
		CString str = _T("");
		nSizeBM	= pArrFormVert.GetSize();
		for(i = 0; i < nSizeBM; i++)
		{
			CBMData *pBM	= pArrFormVert.GetAt(i);
			pWall->m_dArrAddValue.Add(pBM->GetResult());
			
			str	= baseBM.GetCircleText(pBM->m_nNumbering);
			if(pWall->m_sForm.Find(str, 0) == -1)
			{
				CString strTmp = _T("");
				strTmp.Format(",%s", str);
				pWall->m_sForm += strTmp;
			}
		}
		
		str = pWall->m_sForm.Right(pWall->m_sForm.GetLength() - 1);
		CString sCurve(_T(""));
		if(pBri->m_nSlabChemfer[0] == 2 || pBri->m_nSlabChemfer[1] == 2)
			sCurve = _T(",곡면");
		if(bIncludeSideForm)
			pWall->m_sForm = (str + "의 거푸집(수직면,경사면" + sCurve + ")수량");
		else
			pWall->m_sForm = (str + "의 거푸집(수직면,경사면" + sCurve + ")수량(교대측면 수량 제외)");

		pWall->m_bIsPrintByRefForm	= FALSE;
		pWall->CalculateResult();
		// 예외적인 출력양식
		pWall->m_nLevelMidClass		= 2;
		m_pArrBMBridgeStd.Add(pWall);
	}

	// 슬래브용 스페이셔 수량 생성 
	if(pArrFormHor.GetSize() > 0)
	{
		// 상부슬래브 하면 거푸집 + 기초하면 면적
		// 수평면
		CBMData *pSlab1				= new CBMData;
		pSlab1->m_nIdxBigClass		= BM_BIG_SPACER;
		pSlab1->m_sBigClass			= GetStringBigClass(pSlab1->m_nIdxBigClass);
		pSlab1->m_nLevelTotal		= GetLevelTotal(pSlab1->m_nIdxBigClass);
		pSlab1->m_nIdxSmallClass	= BM_SMALL_SPACER_SLAB;
		pSlab1->m_sSmallClass		= GetStringSmallClass(pSlab1->m_nIdxSmallClass);
		pSlab1->m_nTypeResult		= RESULT_AREA;
		pSlab1->m_sUnit				= "㎡";
		pSlab1->m_sInfo				= "본체슬래브용스페이셔(거푸집수평면)";

		// 수집한 거푸집 수량들을 추가시킴
		nSizeBM	= pArrFormHor.GetSize();
		for(i = 0; i < nSizeBM; i++)
		{
			CBMData *pBM	= pArrFormHor.GetAt(i);
			pSlab1->m_dArrAddValue.Add(pBM->GetResult());
			
			CString str	= baseBM.GetCircleText(pBM->m_nNumbering);
			if(pSlab1->m_sForm.Find(str, 0) == -1)
			{
				CString strTmp = _T("");
				strTmp.Format(",%s", str);
				pSlab1->m_sForm += strTmp;
			}
		}
		
		CString str = pSlab1->m_sForm.Right(pSlab1->m_sForm.GetLength() - 1);
		pSlab1->m_sForm = (str + "의 거푸집(수평면) 수량");

		pSlab1->m_bIsPrintByRefForm	= FALSE;
		pSlab1->CalculateResult();
		// 예외적인 출력양식
		pSlab1->m_nLevelMidClass		= 2;
		m_pArrBMBridgeStd.Add(pSlab1);	
	}

	// 기초하면면적
	CalAreaConc(pBri, FALSE);
	long nCountFooting	= pBri->GetCountFooting();
	for(long footing = 0; footing < nCountFooting; footing++)
	{
		CFootingApp *pFooting	= pBri->GetFooting(footing);
		if(pFooting)
		{
			long nSize	= pFooting->m_pArrAreaConcPlane.GetSize();
			for(i = 0; i < nSize; i++)
			{
				CTwinVectorAreaShape *pArea	= pFooting->m_pArrAreaConcPlane.GetAt(i);

				CBMData *pSlab2					= new CBMData;
				pSlab2->m_nNumbering			= pArea->m_nNum;
				pSlab2->m_nIdxBigClass			= BM_BIG_SPACER;
				pSlab2->m_sBigClass				= GetStringBigClass(pSlab2->m_nIdxBigClass);
				pSlab2->m_nLevelTotal			= GetLevelTotal(pSlab2->m_nIdxBigClass);
				pSlab2->m_nIdxSmallClass		= BM_SMALL_SPACER_SLAB;
				pSlab2->m_sSmallClass			= GetStringSmallClass(pSlab2->m_nIdxSmallClass);
				pSlab2->m_nTypeResult			= RESULT_AREA;
				pSlab2->m_sUnit					= "㎡";
				pSlab2->m_nShape				= pArea->m_nTypeShape;
				pSlab2->m_dV[WidthLower]		= Round(toM(pArea->m_dWidthLower),3);
				pSlab2->m_sVC[WidthLower]		= "";
				pSlab2->m_dV[HeightLeft]		= Round(pSlab2->m_dV[WidthLower],3);
				pSlab2->m_sVC[HeightLeft]		= "";

				pSlab2->m_dV[WidthUpper]		= Round(toM(pArea->m_dWidthUpper),3);
				pSlab2->m_sVC[WidthUpper]		= "";
				pSlab2->m_dV[HeightRight]		= Round(pSlab2->m_dV[WidthUpper],3);
				pSlab2->m_sVC[HeightRight]		= "";

				pSlab2->m_dV[Height]			= Round(toM(pArea->m_dHeight),3);
				pSlab2->m_sVC[Height]			= "";
				pSlab2->m_dV[Width]				= Round(pSlab2->m_dV[Height],3);
				pSlab2->m_sVC[Width]			= "";

				pSlab2->m_dV[WidthDiffLeft]		= Round(toM(pArea->m_dWidthDiffLeft),3);
				pSlab2->m_sVC[WidthDiffLeft]	= "";
				pSlab2->m_dV[WidthDiffRight]		= Round(toM(pArea->m_dWidthDiffRight),3);
				pSlab2->m_sVC[WidthDiffRight]	= "";
				pSlab2->m_dwSide				= SIDE_FRONT;
				pSlab2->m_sInfo					= "본체슬래브용스페이셔(기초하면면적)";
				pSlab2->m_sNoteHead				= "기초하면 면적";
				pSlab2->CalculateResult();
				// 예외적인 출력양식
				pSlab2->m_nLevelMidClass		= 2;

				m_pArrBMBridgeStd.Add(pSlab2);
			}
		}
	}

	//추가 Box형라멘일 경우 기초 슬래브의 스페이서 면적 (하부 슬래브는 따로 거푸집 수량을 구하지 않기 때문에 따로 구함)
	if(pBri->IsBoxType())
	{
		long nNumLower = -1;
		double dLen(0);
		double dSumPfWidth(0);

		for (long nIdx =0; nIdx < pBri->m_pArrAreaConc.GetSize(); nIdx++)
		{
			CTwinVectorAreaShape *pArea	= pBri->m_pArrAreaConc.GetAt(nIdx);
			if(pArea->m_sInfo.Find(_T("부상방지 저판")) > -1)
			{
				dSumPfWidth += Round(toM(pArea->m_dWidth),3);
			}
			if(pArea->m_sInfo.Find(_T("하부슬래브")) > -1)
			{
				dLen = Round(toM(pArea->m_dWidth),3);
				nNumLower  = pArea->m_nNum;
			}
		}
		
		if(dLen > 0 && nNumLower > -1)
		{
			double dWidth = Round(toM(pBri->GetWidthSlabAct(pBri->m_dStationBridgeStt, CDPoint(0, 1), 0)),3);
			double dResult	= dWidth * (dLen + dSumPfWidth);

			if(dResult > 0)
			{
				CString sFormula = _T(""), str = _T("");

				str = baseBM.GetCircleText(nNumLower);
				str += _T("의 거푸집(수평면) 수량");				
				
				sFormula.Format("%s %.3f x %.3f", str , dWidth, dLen+dSumPfWidth);

				CBMData *pSlabLower			= new CBMData;
				pSlabLower->m_nNumbering			= -1;
				pSlabLower->m_nIdxBigClass			= BM_BIG_SPACER;
				pSlabLower->m_sBigClass				= GetStringBigClass(pSlabLower->m_nIdxBigClass);
				pSlabLower->m_nLevelTotal			= GetLevelTotal(pSlabLower->m_nIdxBigClass);
				pSlabLower->m_nIdxSmallClass		= BM_SMALL_SPACER_SLAB;
				pSlabLower->m_sSmallClass			= GetStringSmallClass(pSlabLower->m_nIdxSmallClass);
				pSlabLower->m_nTypeResult			= RESULT_AREA;
				pSlabLower->m_sUnit					= "㎡";
				pSlabLower->m_nShape				= SHAPE_TV_RECTANGLE;
				pSlabLower->m_sInfo					= "본체슬래브용스페이셔(하부슬래브하면면적)";

				pSlabLower->m_sForm.Format("%s", sFormula);
				pSlabLower->m_sFormForCompare	= pSlabLower->m_sForm;
				pSlabLower->SetResult(Round(dResult, 3));
				pSlabLower->m_sEtc				= _T("");
				pSlabLower->m_bIsPrintByRefForm	= FALSE;
				pSlabLower->m_nLevelMidClass		= 2;

				m_pArrBMBridgeStd.Add(pSlabLower);
			}			
		}		
	}


	AhTPADelete(&pArrFormHor,(CBMData*)0);
	AhTPADelete(&pArrFormVert,(CBMData*)0);
}

// 콘크리트 양생 본체
// 기초를 제외한 거푸집 수량 + 상부수평면
void COutBM::MakeBMConCareMain()
{
	CRcBridgeRebar *pBri	= GetRcBridgeRebar();
	if(!pBri) return;
	if(!pBri->m_bSelectBM_ConcCare_Main) return;

	
	// 거푸집을 모두 수집한다.
	// 버림콘크리트및 mass콘크리트, 기초를 제외한 모든 거푸집을 구함
	CTypedPtrArray <CObArray, CBMData*> pArrForm;
	long nSizeBM	= m_pArrBMBridgeStd.GetSize();
	long i = 0; for(i = 0; i < nSizeBM; i++)
	{
		CBMData *pBM = m_pArrBMBridgeStd.GetAt(i);
		if(pBM->m_nIdxBigClass == BM_BIG_FORM)
		{
 			if(pBM->m_sInfo.Find("버림콘크리트거푸집") != -1) continue;
			if(pBM->m_sInfo.Find("MASS콘크리트거푸집") != -1) continue;
			if(pBM->m_sInfo.Find("기초거푸집") != -1) continue;
			
			CBMData *pForm = new CBMData;
			*pForm = *pBM;
			// 거푸짚 종류별로 묶어져 있으면 번호가 순서대로 정렬되지 않는다.
			// 그래서 여기서는 거푸집 종류를 모두 같은 것으로 해준다.
			pForm->m_sMidClass		= _T("");
			pForm->m_nIdxMidClass	= -1;
			pForm->m_sSmallClass	= _T("");
			pForm->m_nIdxSmallClass	= -1;
			pForm->m_sItemClass		= _T("");
			pForm->m_nIdxItemClass	= -1;
			pForm->m_sPartClass		= _T("");
			pForm->m_nIdxPartClass	= -1;
			pForm->m_sMeterClass	= _T("");
			pForm->m_nIdxMeterClass	= -1;

			pArrForm.Add(pForm);
		}
	}
	
	// 정렬
	CHgBaseBMStd baseBM;
	baseBM.ShellSortBMList(&pArrForm, &CBMData::CompareBM);

	// 거푸집 수량 추가
	if(pArrForm.GetSize() > 0)
	{
		CBMData *pBM			= new CBMData;
		pBM->m_nIdxBigClass		= BM_BIG_CONCCARE;
		pBM->m_sBigClass		= GetStringBigClass(pBM->m_nIdxBigClass);
		pBM->m_nLevelTotal		= GetLevelTotal(pBM->m_nIdxBigClass);
		pBM->m_nIdxMidClass		= BM_MID_CONCCARE;
		pBM->m_sMidClass		= GetStringMidClass(pBM->m_nIdxMidClass);
		pBM->m_nIdxSmallClass	= BM_SMALL_CONCCARE;
		pBM->m_sSmallClass		= GetStringSmallClass(pBM->m_nIdxSmallClass);
		pBM->m_nTypeResult		= RESULT_AREA;
		pBM->m_sUnit			= "㎡";
		pBM->m_sInfo			= "콘크리트양생(거푸집)";

		if(pBri->m_nSelectBM_ConcCare_Cal == 0)
		{
			// 수집한 거푸집 수량들을 추가시킴
			CString str = _T("");
			nSizeBM	= pArrForm.GetSize();
			for(i = 0; i < nSizeBM; i++)
			{
				CBMData *pBMForm	= pArrForm.GetAt(i);
				pBM->m_dArrAddValue.Add(pBMForm->GetResult());

				str	= baseBM.GetCircleText(pBMForm->m_nNumbering);
				if(pBM->m_sForm.Find(str, 0) == -1)
				{
					CString strTmp = _T("");
					strTmp.Format(",%s", str);
					pBM->m_sForm += strTmp;
				}
			}

			str = pBM->m_sForm.Right(pBM->m_sForm.GetLength() - 1);
			pBM->m_sForm = (str + "의 거푸집(기초제외) 수량과 동일");

			pBM->m_bIsPrintByRefForm	= FALSE;
			pBM->CalculateResult();
			m_pArrBMBridgeStd.Add(pBM);
		}
		else
		{
			double dStaL(0),dStaR(0),dStaMid(0), dWidth(0);
			CDPoint vAngStt(0,0), vAngEnd(0,0);
			vAngStt = pBri->GetAngleJijum(0);
			dWidth = pBri->GetWidthSlabAct(pBri->m_dStationBridgeStt, vAngStt, 0, FALSE);
			
			if(pBri->m_nTypeBridge != BRIDGE_TYPE_BOX)
			{
				const long nCount = 4;

				double dLength;
				CString str;
				
				CStringArray sArrPosition;
				sArrPosition.Add(_T("시점기초뒷굽"));
				sArrPosition.Add(_T("시점기초앞굽"));
				sArrPosition.Add(_T("종점기초뒷굽"));
				sArrPosition.Add(_T("종점기초앞굽"));

				long nCountFooting	= pBri->GetCountFooting();
				for(long footing = 0; footing < nCountFooting; footing++)
				{
					CFootingApp *pFooting	= pBri->GetFooting(footing);

					if(pFooting)
					{
						for (i = 0; i < nCount/2; i++)
						{
							if(footing == 0 || footing == nCountFooting -1)
							{
								if(footing == 0)
									dLength = (i == 0 ? pFooting->m_dFWL : pFooting->m_dFWR);
								else
									dLength = (i == 0 ? pFooting->m_dFWR : pFooting->m_dFWL);

								CBMData *pBMHor				= new CBMData(pBM);
								CTwinVectorAreaShape *pArea	= pFooting->m_pArrAreaConcPlane.GetAt(0);
								pBMHor->m_dArrAddValue.RemoveAll();
								pBMHor->m_dArrDeductValue.RemoveAll();
								pBMHor->m_sForm				= _T("");
								pBMHor->m_sFormForCompare	= _T("");
								pBMHor->m_nShape			= SHAPE_TV_RECTANGLE;
								pBMHor->m_dV[WidthLower]	= Round(toM(dLength), 3);
								pBMHor->m_dV[Height]		= Round(toM(dWidth), 3);
								pBMHor->m_dwSide			= SIDE_FRONT;
								pBMHor->CalculateResult();
								pBMHor->m_sNoteTail	= sArrPosition.GetAt(footing == 0 ? i : i+2);
								m_pArrBMBridgeStd.Add(pBMHor);
							}
						}
						if(footing != 0 && footing != nCountFooting -1)
						{
							for(long ix = 0; ix < 2; ix++)
							{
								CBMData *pBMHor				= new CBMData(pBM);
								CWallApp *pWall = pBri->GetInWall(footing -1);
								CTwinVectorAreaShape *pArea	= pFooting->m_pArrAreaConcPlane.GetAt(0);
								pBMHor->m_dArrAddValue.RemoveAll();
								pBMHor->m_dArrDeductValue.RemoveAll();
								pBMHor->m_sForm				= _T("");
								pBMHor->m_sFormForCompare	= _T("");
								pBMHor->m_nShape			= SHAPE_TV_RECTANGLE;
								pBMHor->m_dV[WidthLower]	= Round(toM(pWall->m_footing.m_dFWL), 3);
								pBMHor->m_dV[Height]		= Round(toM(dWidth), 3);
								pBMHor->m_dwSide			= SIDE_FRONT;
								pBMHor->CalculateResult();
								str.Format(_T("중간기초%d"),footing);
								pBMHor->m_sNoteTail	= str;
								m_pArrBMBridgeStd.Add(pBMHor);
							}
						}

					}
				}
			}
			else //함형라멘교 type
			{
				pBri->GetTvVertSection_Std(0, FALSE, FALSE);
				CTwinVectorArray tvArrUpper, tvArrLeft, tvArrRight, tvArrMid, tvArrArch, tvArrMid2;

				for(long jigan = 0; jigan < pBri->m_nQtyJigan; jigan++)
				{
					pBri->GetTvPostJigan(tvArrUpper, tvArrLeft, tvArrRight, tvArrMid, tvArrArch, tvArrMid2, jigan);

					double dLenSlab = fabs(tvArrMid.GetXyLeft().x - tvArrMid.GetXyRight().x);

					CBMData *pBMWall = new CBMData(pBM);
					pBMWall->m_dArrAddValue.RemoveAll();
					pBMWall->m_dArrDeductValue.RemoveAll();
					pBMWall->m_sForm			= _T("");
					pBMWall->m_sFormForCompare	= _T("");
					pBMWall->m_nShape			= SHAPE_TV_RECTANGLE;
					pBMWall->m_dV[WidthLower]	= Round(toM(dLenSlab), 3);
					pBMWall->m_dV[Height]		= Round(toM(dWidth), 3);
					pBMWall->m_dwSide			= SIDE_FRONT;
					pBMWall->CalculateResult();
					m_pArrBMBridgeStd.Add(pBMWall);
				}

				//상부슬래브 상면
				CTwinVectorArray tvArrSecCopy;

				tvArrSecCopy.AddFromTvArray(pBri->m_tvArrVertSection);
				for (long nIdx=0; nIdx< tvArrSecCopy.GetSize(); nIdx++)
				{
					CTwinVector tvSec = tvArrSecCopy.GetAt(nIdx);
					if(tvSec.m_sInfo.Find(_T("브라켓")) > -1 || tvSec.m_sInfo.Find(_T("부상방지")) > -1)
					{
						tvArrSecCopy.RemoveAt(nIdx);
						nIdx--;
					}
				}
				double dTotSlabLen = tvArrSecCopy.GetXyRight().x - tvArrSecCopy.GetXyLeft().x;
				double dLengthTot = dTotSlabLen;

				CBMData *pBMHor				= new CBMData(pBM);
				pBMHor->m_dArrAddValue.RemoveAll();
				pBMHor->m_dArrDeductValue.RemoveAll();
				pBMHor->m_sForm				= _T("");
				pBMHor->m_sFormForCompare	= _T("");
				pBMHor->m_nShape			= SHAPE_TV_RECTANGLE;
				pBMHor->m_dV[WidthLower]	= Round(toM(dLengthTot), 3);
				pBMHor->m_dV[Height]		= Round(toM(dWidth), 3);
				pBMHor->m_dwSide			= SIDE_FRONT;
				pBMHor->CalculateResult();
				m_pArrBMBridgeStd.Add(pBMHor);
			}
		}
	}
	AhTPADelete(&pArrForm,(CBMData*)0);
}

// 접속슬래브 콘크리트 양생
// 버림콘크리트를 제외한 거푸집 + 상부평면면적
void COutBM::MakeBMConCareAps()
{
	CRcBridgeRebar *pBri	= GetRcBridgeRebar();
	if(!pBri) return;	
	if(!pBri->m_bSelectBM_ApsCnsInclude) return;
	if(!pBri->m_bSelectBM_ConcCare_Aps) return;

	// 평면 면적
	long stt = 0; for(stt = 0; stt < 2; stt++)
	{
		BOOL bStt	= stt == iSTT ? TRUE : FALSE;

		for(long left = 0; left < 2; left++)
		{
			BOOL bLeft	= left == iLEFT ? TRUE : FALSE;

			CApsApp *pAps	= bStt ? &pBri->m_apsStt[left] : &pBri->m_apsEnd[left];
			if(!pAps->m_bIs) continue;
			CTypedPtrArray <CObArray, CBMData*> *pArrBMStd	= &m_pArrBMApsStd[stt][left];

			// 거푸집을 모두 수집한다. ///////////////////////////////////////////////////////////
			// 버림콘크리트를 제외한 모든 거푸집을 구함
			CTypedPtrArray <CObArray, CBMData*> pArrForm;
			long nSizeBM	= m_pArrBMApsStd[stt][left].GetSize();
			long i = 0; for(i = 0; i < nSizeBM; i++)
			{
				CBMData *pBM = m_pArrBMApsStd[stt][left].GetAt(i);
				if(pBM->m_nIdxBigClass == BM_BIG_FORM)
				{
 					if(pBM->m_sInfo.Find("버림콘크리트거푸집") != -1) continue;
					
					CBMData *pForm = new CBMData;
					*pForm = *pBM;
					// 거푸짚 종류별로 묶어져 있으면 번호가 순서대로 정렬되지 않는다.
					// 그래서 여기서는 거푸집 종류를 모두 같은 것으로 해준다.
					pForm->m_sMidClass		= _T("");
					pForm->m_nIdxMidClass	= -1;
					pForm->m_sSmallClass	= _T("");
					pForm->m_nIdxSmallClass	= -1;
					pForm->m_sItemClass		= _T("");
					pForm->m_nIdxItemClass	= -1;
					pForm->m_sPartClass		= _T("");
					pForm->m_nIdxPartClass	= -1;
					pForm->m_sMeterClass	= _T("");
					pForm->m_nIdxMeterClass	= -1;

					pArrForm.Add(pForm);
				}
			}
						
			// 정렬
			CHgBaseBMStd baseBM;
			baseBM.ShellSortBMList(&pArrForm, &CBMData::CompareBM);

			// 거푸집 수량 추가
			if(pArrForm.GetSize() > 0)
			{
				CBMData *pBM			= new CBMData;
				pBM->m_nIdxBigClass		= BM_BIG_CONCCARE;
				pBM->m_sBigClass		= GetStringBigClass(pBM->m_nIdxBigClass);
				pBM->m_nLevelTotal		= GetLevelTotal(pBM->m_nIdxBigClass);
				pBM->m_nIdxMidClass		= BM_MID_CONCCARE;
				pBM->m_sMidClass		= GetStringMidClass(pBM->m_nIdxMidClass);
				pBM->m_nIdxSmallClass	= BM_SMALL_CONCCARE;
				pBM->m_sSmallClass		= GetStringSmallClass(pBM->m_nIdxSmallClass);
				pBM->m_nTypeResult		= RESULT_AREA;
				pBM->m_sUnit			= "㎡";
				pBM->m_sInfo			= "콘크리트양생(거푸집)";

				// 수집한 거푸집 수량들을 추가시킴
				CString str = _T("");
				nSizeBM	= pArrForm.GetSize();
				for(i = 0; i < nSizeBM; i++)
				{
					CBMData *pBMForm	= pArrForm.GetAt(i);
					pBM->m_dArrAddValue.Add(pBMForm->GetResult());
					
					str	= baseBM.GetCircleText(pBMForm->m_nNumbering);
					if(pBM->m_sForm.Find(str, 0) == -1)
					{
						CString strTmp = _T("");
						strTmp.Format(",%s", str);
						pBM->m_sForm += strTmp;
					}
				}
				
				str = pBM->m_sForm.Right(pBM->m_sForm.GetLength() - 1);
				pBM->m_sForm = (str + "의 거푸집(버림콘크리트제외) 수량과 동일");
				
				pBM->m_bIsPrintByRefForm	= FALSE;
				pBM->CalculateResult();
				pArrBMStd->Add(pBM);
			}
			AhTPADelete(&pArrForm, (CBMData*)0);
			// 거푸집을 모두 수집한다. ///////////////////////////////////////////////////////////

			// 접속슬래브 평면 영역 구하기 . //////////////////////////////////////////////////////
			GetTvAreaShapeApsPlane(&m_pArrAreaConc, bStt, bLeft);

			long nSize	= m_pArrAreaConc.GetSize();
			for(i = 0; i < nSize; i++)
			{
				CTwinVectorAreaShape *pArea	= m_pArrAreaConc.GetAt(i);

				CBMData *pBM		    	= new CBMData;
				pBM->m_nNumbering			= pArea->m_nNum;
				pBM->m_nIdxBigClass			= BM_BIG_CONCCARE;
				pBM->m_sBigClass			= GetStringBigClass(pBM->m_nIdxBigClass);
				pBM->m_nLevelTotal			= GetLevelTotal(pBM->m_nIdxBigClass);
				pBM->m_nIdxMidClass			= BM_MID_CONCCARE;
				pBM->m_sMidClass			= GetStringMidClass(pBM->m_nIdxMidClass);
				pBM->m_nIdxSmallClass		= BM_SMALL_CONCCARE;
				pBM->m_sSmallClass			= GetStringSmallClass(pBM->m_nIdxSmallClass);
				pBM->m_nTypeResult			= RESULT_AREA;
				pBM->m_sUnit				= "㎡";
				pBM->m_nShape				= pArea->m_nTypeShape;
				pBM->m_dV[WidthLower]		= Round(toM(pArea->m_dWidthLower),3);
				pBM->m_dV[WidthUpper]		= Round(toM(pArea->m_dWidthUpper),3);
				pBM->m_dV[WidthDiffLeft]	= Round(toM(pArea->m_dWidthDiffLeft),3);
				pBM->m_dV[WidthDiffRight]	= Round(toM(pArea->m_dWidthDiffRight),3);
				pBM->m_dV[Height]			= Round(toM(pArea->m_dHeight),3);
				pBM->m_dwSide				= SIDE_FRONT;
				pBM->CalculateResult();
				pArrBMStd->Add(pBM);
			}
			// 접속슬래브 평면 영역 구하기 . //////////////////////////////////////////////////////
		}
	}
}

// long nTypeComplex : 0 : 간단, 1 : 보통, 2 : 복잡, 3 : 매우복잡
void COutBM::SetBMRebar(CTypedPtrArray <CObArray, CBMData*> *pArrBM, CTypedPtrArray <CObArray, CRebar*> *pArrRebar, long nTypeComplex, long bIncludeFlexibleJointRebar, CString sNote)
{
	if(pArrRebar->GetSize() == 0) return;

	CBasicConcInfo concInfo;
	CHgBaseConcStd HGConc;

	switch(nTypeComplex)
	{
		case 0: nTypeComplex	= BM_SMALL_REBAR_SIMPLE; break;
		case 1: nTypeComplex	= BM_SMALL_REBAR_NORMAL; break;
		case 2: nTypeComplex	= BM_SMALL_REBAR_COMPLEXITY1; break;
		case 3: nTypeComplex	= BM_SMALL_REBAR_COMPLEXITY2; break;
		default: nTypeComplex	= BM_SMALL_REBAR_COMPLEXITY1; break;
	}

	//HGConc.ShellSortRebarList(pArrRebar, CRebar::CompareForTable);

	// 실제로 수량 생성하는 부분은 여기서 부터 (위에는 전부 철근 집계만 함)
	CString sRebarType[2]		= {"연강", "고강"};
	CString sDia[2][4]			= {{"D10이하", "D13", "D16~25", "D29~35"}, {"H10이하", "H13", "H16~25", "H29~35"}};
	double dWeightTotal[2][4]	= {0, 0, 0, 0};	// 0 : 연강, 1 : 고강 , 10이하, 13, 16~25, 25~35 
	double dUWeight		= 0;
	double dLength		= 0; 
	double dWeight		= 0;
	CRebar *pRB			= NULL;
	long nTypeBM		= m_pStd->m_pARcBridgeDataStd->m_nTypeBMApply;

	long nSize	= pArrRebar->GetSize();
	
	long i = 0; for(i = 0; i < nSize; i++)
	{
		pRB = (CRebar*)pArrRebar->GetAt(i);
		if(!pRB->IsTrue()) continue;

		dUWeight = concInfo.GetRebarInfo(pRB->m_dDia, BASIC_WEIGHT_UNIT);
		dLength  = Round(toM(pRB->GetLength(TRUE, TRUE)*pRB->m_nEa), 3); 
		dWeight  = toTon(dLength  * dUWeight);
		
		if(nTypeBM == TYPE_BM_NORMAL)	// 일반수량이면 종류에 관계없이 구함
		{
			dWeightTotal[0][0] += dWeight;
		}
		else
		{
			long nType	= pRB->m_Fy > 3000 ? 1 : 0;

			if(pRB->m_dDia <= 10)
				dWeightTotal[nType][0] += dWeight;
			else if(pRB->m_dDia == 13)
				dWeightTotal[nType][1]	+= dWeight;
			else if(pRB->m_dDia >= 16 && pRB->m_dDia <= 25)
				dWeightTotal[nType][2]	+= dWeight;
			else if(pRB->m_dDia >= 29 && pRB->m_dDia <= 35)
				dWeightTotal[nType][3]	+= dWeight;
		}
	}
	
	// 신축이음 철근 수량
	// 신축이음철근은 pRB->m_dLengthTotal에 톤수가 들어가 있음
	if(bIncludeFlexibleJointRebar)
	{
		CRcBridgeRebar *pBri	= GetRcBridgeRebar(TRUE);
		long stt = 0; for(stt = 0; stt < 2; stt++)
		{
			BOOL bStt	= stt == iSTT;
			CFlexibleJoint *pFx	= bStt ? &pBri->m_fxJointStt : &pBri->m_fxJointEnd;
			for(long r = 0; r < 2; r++)
			{
				CRebar *pRB	= &pFx->m_rb[r];
				if(pRB->m_dLengthTotal > 0 && pRB->m_dDia > 0)
				{
					if(nTypeBM == TYPE_BM_NORMAL)
					{
						dWeightTotal[0][0] += pRB->m_dLengthTotal;
					}
					else
					{
						long nType	= pBri->m_pARcBridgeDataStd->m_Fy > 3000 ? 1 : 0;

						if(pRB->m_dDia <= 10)
							dWeightTotal[nType][0] += pRB->m_dLengthTotal;
						else if(pRB->m_dDia == 13)
							dWeightTotal[nType][1]	+= pRB->m_dLengthTotal;
						else if(pRB->m_dDia >= 16 && pRB->m_dDia <= 25)
							dWeightTotal[nType][2]	+= pRB->m_dLengthTotal;
						else if(pRB->m_dDia >= 29 && pRB->m_dDia <= 35)
							dWeightTotal[nType][3]	+= pRB->m_dLengthTotal;
					}
				}
			}
		}
	}


	if(nTypeBM == TYPE_BM_NORMAL)
	{
		CBMData *pBM			= new CBMData;
		pBM->m_nIdxBigClass		= BM_BIG_REBAR;
		pBM->m_sBigClass		= GetStringBigClass(pBM->m_nIdxBigClass);
		pBM->m_nLevelTotal		= GetLevelTotal(pBM->m_nIdxBigClass);
		pBM->m_nIdxMidClass		= BM_MID_REBAR;
		pBM->m_sMidClass		= GetStringMidClass(pBM->m_nIdxMidClass);
		pBM->m_nIdxSmallClass	= nTypeComplex;
		pBM->m_sSmallClass		= GetStringSmallClass(pBM->m_nIdxSmallClass);
		pBM->m_nTypeResult		= RESULT_WEIGHT;
		pBM->m_sUnit			= "TON";
		pBM->m_sForm.Format("%.3f", dWeightTotal[0][0]);
		pBM->m_dV[Result]		= dWeightTotal[0][0];
		pBM->m_bApplyIncreaseForm	= FALSE;
		pBM->m_sNoteHead	= sNote;
		pArrBM->Add(pBM);
	}
	else
	{
		long nIdx	= 0;
		for(long type = 0; type < 2; type++)
		{
			for(i = 0; i < 4; i++)
			{
				CBMData *pBM			= new CBMData;
				pBM->m_nIdxBigClass		= BM_BIG_REBAR;
				pBM->m_nNumbering		= i+1;
				pBM->m_sBigClass		= GetStringBigClass(pBM->m_nIdxBigClass);
				pBM->m_nLevelTotal		= GetLevelTotal(pBM->m_nIdxBigClass);
				pBM->m_nIdxMidClass		= BM_MID_REBAR;
				pBM->m_sMidClass		= GetStringMidClass(pBM->m_nIdxMidClass);
				pBM->m_nIdxSmallClass	= nTypeComplex;
				pBM->m_sSmallClass		= GetStringSmallClass(pBM->m_nIdxSmallClass);
				pBM->m_nIdxItemClass	= nIdx++;
				pBM->m_sItemClass		= _T("");
				pBM->m_sForm.Format("%s(%s)", sDia[type][i], sRebarType[type]); 
				pBM->m_sFormForCompare	= pBM->m_sForm;
				pBM->m_nTypeResult		= RESULT_WEIGHT;
				pBM->m_sUnit			= "TON";
				pBM->m_dV[Result]		= dWeightTotal[type][i];

				// 예외적인 집계표 출력 양식 설정
				pBM->m_nLevelMidClass	= 2;
				pBM->m_nLevelSmallClass	= 6;
				pBM->m_bApplyIncreaseForm	= FALSE;
				pBM->m_sNoteHead	= sNote;
				pArrBM->Add(pBM);
			}
		}
	} 
}

// 본체 철근 가공 조립
// nType 0 : 본체, 1 : 날개벽, 2 : 접속슬래브(완충포함),  3 : 방호벽 5: 현장타설말뚝
void COutBM::MakeBMRebar(long nType)
{
	CRcBridgeRebar *pBri	= GetRcBridgeRebar(TRUE);
	if(!pBri) return;

	// 0 : 간단, 1 : 보통, 2 : 복잡, 3 : 매우복잡
	CTypedPtrArray <CObArray, CRebar*>  pArrRebar[4];

	// 날개벽을 제외한 모든 철근을 가져와야 됨 ////////////////////////////////////////////
	// -1 : 전체
	// 0 : 본체
	// 1 : 중간벽체
	// 2 : 기초
	// 3 : 날개벽
	// 4 : 상세부철근
	// 5 : 기둥
	// 6 : 보기둥이 아닌(거더철근)
	// 7 : 보기둥인(거더철근)
	// 8 : 방호벽(거더철근)
	if(nType == 0)
	{
		long i = 0; for(i = 0; i < 4; i++)
			AhTPADelete(&pArrRebar[i],(CReBar*)0);

		pBri->MakeRebarList(&pArrRebar[2], 0, TRUE, TRUE, 0);		// 본체(복잡)
		for(i = 0; i <= pBri->m_nQtyJigan/*pBri->GetCountInWall()*/; i++)
		{
			pBri->MakeRebarList(&pArrRebar[2], 1, TRUE, TRUE, i);	// 중간지점(복잡)
			pBri->MakeRebarList(&pArrRebar[2], 5, TRUE, TRUE, i);	// 중간지점(복잡)
			pBri->MakeRebarList(&pArrRebar[2], 6, TRUE, TRUE, i);	// 보기둥이 아닌(거더철근)
			pBri->MakeRebarList(&pArrRebar[2], 7, TRUE, TRUE, i);	// 보기둥인(거더철근)
		}
		pBri->MakeRebarList(&pArrRebar[2], 2, TRUE, TRUE, 0);		// 기초(복잡)
		pBri->MakeRebarList(&pArrRebar[2], 10, TRUE, TRUE, i);		//현장타설말뚝

		if(!m_pStd->m_bSeparateExcludeRebarMonument)
			pBri->MakeRebarList(&pArrRebar[2], 4, TRUE, TRUE, i);		// 상세부(복잡)
		
		SetBMRebar(&m_pArrBMBridgeStd, &pArrRebar[2], 2, TRUE, "상부슬래브,벽체(기둥),기초, 교명주받침, 신축이음 철근수량 포함");
	}
	else if(nType == 1)
	{
		long stt = 0; for(stt = 0; stt < 2; stt++)
		{
			BOOL bStt	= stt == iSTT;
			for(long left = 0; left < 2; left++)
			{
				BOOL bLeft	= left == iLEFT;

				long i = 0; for(i = 0; i < 4; i++)
					AhTPADelete(&pArrRebar[i],(CReBar*)0);

				if(!pBri->IsWingWall(bStt, bLeft)) continue;

				pBri->MakeRebarList(&pArrRebar[2], 3, bStt, bLeft, 0);
				SetBMRebar(&m_pArrBMWingStd[stt][left], &pArrRebar[2], 2);
			}
		}
	}
	else if(nType == 2)
	{
		// 다웰바는 따로 빼 놨다가 간단으로 함.
		// 접속슬래브 철근 모두 가져옴
		long stt = 0; for(stt = 0; stt < 2; stt++)
		{
			BOOL bStt	= stt == iSTT;
			for(long left = 0; left < 2; left++)
			{
				CApsApp *pAps	= bStt ? &pBri->m_apsStt[left] : &pBri->m_apsEnd[left];
				if(!pAps->m_bIs) continue;

				long i = 0; for(i = 0; i < 4; i++)
					AhTPADelete(&pArrRebar[i],(CReBar*)0);

				for(i = 0; i < pAps->m_pArrRebar.GetSize(); i++)
				{
					// 다웰바와 다른 철근 구분함.
					CRebar *pRB	= pAps->m_pArrRebar.GetAt(i);
					if(pRB->m_sDescription.Find("다웰바", 0) != -1)
					{
						if(pBri->m_bExcludeApsDowelRebar)
							continue;

						pArrRebar[0].Add(new CRebar(pRB));
					}
					else
					{
						pArrRebar[1].Add(new CRebar(pRB));
					}
				}

				for(long comp = 0; comp < 2; comp++)
					SetBMRebar(&m_pArrBMApsStd[stt][left], &pArrRebar[comp], comp);
			}
		}
		// 완충슬래브 철근 철근 모두 가져옴
		for(stt = 0; stt < 2; stt++)
		{
			BOOL bStt	= stt == iSTT;
			for(long left = 0; left < 2; left++)
			{
				for(long n = 0; n < 2; n++)
				{
					CApsApp *pCns	= bStt ? &pBri->m_cnsStt[left][n] : &pBri->m_cnsEnd[left][n];
					if(!pCns->m_bIs) continue;
					
					long i = 0; for(i = 0; i < 4; i++)
						AhTPADelete(&pArrRebar[i],(CReBar*)0);

					for(i = 0; i < pCns->m_pArrRebar.GetSize(); i++)
					{
						// 다웰바와 다른 철근 구분함.
						CRebar *pRB	= pCns->m_pArrRebar.GetAt(i);
						if(pRB->m_sDescription.Find("다웰바", 0) != -1)
						{
							if(pBri->m_bExcludeApsDowelRebar)
								continue;

							pArrRebar[0].Add(new CRebar(pRB));
						}
						else
						{
							pArrRebar[1].Add(new CRebar(pRB));
						}
					}

					for(long comp = 0; comp < 2; comp++)
						SetBMRebar(&m_pArrBMCnsStd[stt][left][n], &pArrRebar[comp], comp);
				}
			}
		}
	}
	else if(nType == 3)
	{
		long nQtyHDan = pBri->GetQtyHDan();
		long i=0; for(i=0; i<nQtyHDan; i++)
		{
			CGuardWallRC* pGW = pBri->GetGuardWallByHDan(i);
			if(!pGW) continue;
			if(pGW->IsTypeHDanRoadTotalGuardWall() && !pGW->IsTypeHDanNonRebarConcrete())				
			{
				long j = 0; for(j = 0; j < 4; j++)
					AhTPADelete(&pArrRebar[j],(CReBar*)0);

				pBri->MakeRebarList(&pArrRebar[2], 8, TRUE, TRUE, i);		// 방호벽(복잡) -> 보통으로 변경(#0014842)
				SetBMRebar(&m_pArrBMGuardWallStd[i], &pArrRebar[2], 1);
			}
		}
	}
	// 본체에서 교명주 철근 수량 분리시 발동
	// 원래 교명주 철근 수량은 본체에 포함되어서 교명주 별도로는 철근집계 하지 않았으나
	// 옵션처리하여 교명주 수량에 본체에서 분리할 수 있게 함.
	else if(nType == 4)
	{
		if(m_pStd->m_bSeparateExcludeRebarMonument)
		{
			long j = 0; for(j = 0; j < 4; j++)
				AhTPADelete(&pArrRebar[j],(CReBar*)0);

			pBri->MakeRebarList(&pArrRebar[2],  4, TRUE, TRUE, 0);		// 상세부(복잡)
			SetBMRebar(&m_pArrBMMonumentStd, &pArrRebar[2], 2);
		}
	}

	//현장타설말뚝 철근 수량
	else if(nType == 5 )
	{
		long nCountFooting	= pBri->GetCountFooting();
		for(long footing = 0; footing < nCountFooting; footing++)
		{
			CFootingApp *pFooting	= pBri->GetFooting(footing);
			if(pFooting == NULL) continue;

			// 현장타설말뚝
			if(pFooting->m_exFooting.m_nType == EXFOOTING_TYPE_PILE)
			{
				CExPileApp *pPile	= &pFooting->m_exFooting.m_Pile;
				if(pPile->m_nType == PILE_TYPE_HYUNJANG)
				{
					pBri->MakeRebarList(&pArrRebar[2], 10, TRUE, TRUE, 0);		
					
					SetBMRebar(&m_pArrBMHyuntaStd[footing], &pArrRebar[2], 1);
				}
			}
		}	
	}


	long i = 0; for(i = 0; i < 4; i++)
		AhTPADelete(&pArrRebar[i],(CReBar*)0);
}

// 면고르기
// 슬래브 양생 수량과 동일
void COutBM::MakeBMSurface()
{
	CRcBridgeRebar *pBri	= GetRcBridgeRebar();
	if(!pBri) return;
	
	CTypedPtrArray <CObArray, CBMData*> *pArrBM	= &m_pArrBMBridgeStd;
	
	// 슬래브 양생 수량 수집
	double dTot		= 0;
	long nSizeBM	= pArrBM->GetSize();
	long i = 0; for(i = 0; i < nSizeBM; i++)
	{
		CBMData *pBM = pArrBM->GetAt(i);
		if(pBM->m_nIdxBigClass == BM_BIG_SLABCARE)
			dTot	+= pBM->GetResult();
	}

	// 면고르기수량 추가
	CBMData *pBM = new CBMData;
	pBM->m_nIdxBigClass		= BM_BIG_SURFACE;
	pBM->m_sBigClass		= GetStringBigClass(pBM->m_nIdxBigClass);
	pBM->m_nLevelTotal		= GetLevelTotal(pBM->m_nIdxBigClass);
	pBM->m_nTypeResult		= RESULT_AREA;
	pBM->m_sUnit			= "㎡";
	pBM->m_sNoteHead		= "슬래브양생과 동일";
	pBM->SetResult(dTot);
	pBM->m_sVC[Result].Format("#슬래브양생:%.3f#", pBM->GetResult());
	pArrBM->Add(pBM);
}

// 아스콘 포장
// 슬래브 양생 수량과 동일
// (단 단위가 달라서 슬래브양생에다가 나누기  100)
void COutBM::MakeBMAscon()
{
	CRcBridgeRebar *pBri	= GetRcBridgeRebar();
	if(!pBri) return;
	
	CTypedPtrArray <CObArray, CBMData*> *pArrBM	= &m_pArrBMBridgeStd;
	
	// 슬래브양생 수량 수집
	double dTot	= 0;
	CTypedPtrArray <CObArray, CBMData*> pArrConcCare;
	long nSizeBM	= pArrBM->GetSize();
	long i = 0; for(i = 0; i < nSizeBM; i++)
	{
		CBMData *pBM = pArrBM->GetAt(i);
		if(pBM->m_nIdxBigClass == BM_BIG_SLABCARE)
			dTot	+= pBM->GetResult();
	}

	// 아스콘 포장
	CBMData *pBM = new CBMData;
	pBM->m_nIdxBigClass			= BM_BIG_ASCON;
	pBM->m_sBigClass			= GetStringBigClass(pBM->m_nIdxBigClass);
	pBM->m_nLevelTotal			= GetLevelTotal(pBM->m_nIdxBigClass);
	pBM->m_nIdxMidClass			= BM_MID_ASCON;
	pBM->m_sMidClass.Format("T=%.0fmm", pBri->m_dTP);
	pBM->m_nTypeResult			= RESULT_AREA;
	pBM->m_sUnit				= "㎡";
	pBM->m_sNoteHead			= "슬래브양생 - 보도 - 연석";
	pBM->m_sForm.Format("%.3f", dTot);
	pBM->m_sFormExcel.Format("#슬래브양생:%.3f#", dTot);
	pBM->SetResult(dTot);
	pBM->m_bIsPrintByRefForm	= TRUE;

	pArrBM->Add(pBM);
}

// 택 코팅
// 현재 2회로 고정
// 포장위에 포장이 잘 접착되어 있도록 하기 위한 것으로
// 포장 두께에 따라 횟수가 달라 질  수 있으나 현재는 고정
// 옵션을 어떻게 처리할지는 고민 해봐야 할 문제임
// 아스콘포장 * 회수
void COutBM::MakeBMTackCotting()
{
	CRcBridgeRebar *pBri	= GetRcBridgeRebar();
	if(!pBri) return;
	
	CTypedPtrArray <CObArray, CBMData*> *pArrBM	= &m_pArrBMBridgeStd;
	
	// 아스콘 포장 수량 수집
	double dTot	= 0;
	long nSizeBM	= pArrBM->GetSize();
	long i = 0; for(i = 0; i < nSizeBM; i++)
	{
		CBMData *pBM = pArrBM->GetAt(i);
		if(pBM->m_nIdxBigClass == BM_BIG_ASCON)
			dTot	+= pBM->GetResult();
	}

	// 택 코팅
	double dTackCotting	= Round(pBri->m_dTackCotting, 1);
	CBMData *pBM = new CBMData;
	pBM->m_nIdxBigClass		= BM_BIG_TACKCOTTING;
	pBM->m_sBigClass		= GetStringBigClass(pBM->m_nIdxBigClass);
	pBM->m_nLevelTotal		= GetLevelTotal(pBM->m_nIdxBigClass);
	pBM->m_nIdxMidClass		= BM_MID_TACK_RSC1;
	pBM->m_sMidClass		= GetStringMidClass(pBM->m_nIdxMidClass);
	pBM->m_nTypeResult		= RESULT_AREA;
	pBM->m_sUnit			= "㎡";
	pBM->m_sForm.Format("%.3f x %.1f회", dTot, dTackCotting);
	pBM->m_sFormExcel.Format("#슬래브양생:%.3f# x %.1f회", dTot, dTackCotting);
	pBM->SetResult(dTot * dTackCotting);
	pBM->m_bIsPrintByRefForm	= TRUE;
	pArrBM->Add(pBM);
}

// 교면방수 수량
// 슬래브 양생과 동일
// 슬래브 상면 면적임
void COutBM::MakeBMSurfaceProof()
{
	CRcBridgeRebar *pBri	= GetRcBridgeRebar();
	if(!pBri) return;
		
	CTypedPtrArray <CObArray, CBMData*> *pArrBM	= &m_pArrBMBridgeStd;
	
	// 슬래브 양생
	double dTot	= 0;
	long nSizeBM	= pArrBM->GetSize();
	long i = 0; for(i = 0; i < nSizeBM; i++)
	{
		CBMData *pBM = pArrBM->GetAt(i);
		if(pBM->m_nIdxBigClass == BM_BIG_SLABCARE)
			dTot	+= pBM->GetResult();
	}

	CBMData *pBM = new CBMData;
	pBM->m_nIdxBigClass		= BM_BIG_SURFACEPROOF;
	pBM->m_sBigClass		= GetStringBigClass(pBM->m_nIdxBigClass);
	pBM->m_nLevelTotal		= GetLevelTotal(pBM->m_nIdxBigClass);
	pBM->m_nIdxMidClass		= BM_MID_SURFACEPROOF_PERCOLATE;
	pBM->m_sMidClass		= GetStringMidClass(pBM->m_nIdxMidClass);
	pBM->m_nTypeResult		= RESULT_AREA;
	pBM->m_sUnit			= "㎡";
	pBM->m_sNoteHead		= "슬래브양생과 동일";
	pBM->SetResult(dTot);
	pBM->m_sVC[Result].Format("#슬래브양생:%.3f#", pBM->GetResult());
	pArrBM->Add(pBM);
}

// 슬래브 양생
// 상부슬래브 중 차도부 면적만 구함
// 현재는 차도부만 구하지 않고 전체 다 구해짐(2005.1.25)
void COutBM::MakeBMSlabCare()
{
	//CRcBridgeRebar *pBri = GetRcBridgeRebar();
	CRcBridgeRebar *pBriOrg = GetRcBridgeRebar(TRUE);
	//if(!pBri) return;
	if(!pBriOrg) return;
	
	// 슬래브상면
	// 슬래브상면의 좌우측 길이 계산
	double dLen[2]	= {0, 0};
	CTwinVector tv;
	CString str	= _T("");
	for(long left = 0; left < 2; left++)
	{
		BOOL bLeft	= left == iLEFT;

		/*
		// pBriOrg->m_bSettingVertSection 이 TRUE 이면 m_tvArrVertSection 는 RemoveAll된다.
		if(pBriOrg->m_bSettingVertSection)
		{
			pBriOrg->m_bSettingVertSection = FALSE;
			pBriOrg->GetTvVertSection(bLeft ? -1 : 1);
			pBriOrg->m_bSettingVertSection = TRUE;
		}
		else
		{
			pBriOrg->GetTvVertSection(bLeft ? -1 : 1);
		}
		*/
		CTwinVectorArray tvArrVertSection;
		pBriOrg->GetTvVertSection_Std(bLeft ? -1 : 1, TRUE, FALSE, FALSE, 2);

		if(pBriOrg->m_nTypeBridge == BRIDGE_TYPE_SLAB)
		{
			long i = 0; for(i = 0; i < pBriOrg->m_nQtyJigan; i++)
			{
				str.Format("슬래브상면(%d번째지간)", i+1);
				if(pBriOrg->m_tvArrVertSection.GetTvByInfo(str, tv))
					dLen[left]	+= tv.GetLength(TRUE);
			}
		}
		else
		{
			dLen[left]	= 0;
			if(pBriOrg->m_tvArrVertSection.GetTvByInfo("슬래브상면(외측벽체안쪽)", tv))
				dLen[left]	+= tv.GetLength(TRUE);
			if(pBriOrg->m_tvArrVertSection.GetTvByInfo("슬래브상면(시점벽체위)", tv))
				dLen[left]	+= tv.GetLength(TRUE);
			if(pBriOrg->m_tvArrVertSection.GetTvByInfo("슬래브상면(종점벽체위)", tv))
				dLen[left]	+= tv.GetLength(TRUE);
		}
		pBriOrg->GetTvVertSection_Std(bLeft ? -1 : 1, TRUE, FALSE, TRUE);
	}
	
	pBriOrg->GetTvVertSection(0, TRUE);

	// 슬래브에서 차도부만의 넓이는 현재 시점을 기준으로 구함(확폭인 경우 맞지 않음)
	// 확폭인 경우에는 완전히 다른 방법으로 구해야 됨.
	// 그리고 횡단 경사는 고려하지 않고 level거리만 고려함
	double dWidthSlab	= 0;
	long i = 0; for(i = 0; i < pBriOrg->GetQtyHDan(); i++)
	{
		CGuardWallRC *pGW	= pBriOrg->GetGuardWallByHDan(i);
		if(!pGW) continue;
		if(pGW->IsTypeHDanOnlyNone())
			dWidthSlab += pBriOrg->GetLengthHDan(i);
	}

	CBMData *pBM			= new CBMData;
	pBM->m_nIdxBigClass		= BM_BIG_SLABCARE;
	pBM->m_sBigClass		= GetStringBigClass(pBM->m_nIdxBigClass);
	pBM->m_nLevelTotal		= GetLevelTotal(pBM->m_nIdxBigClass);
	pBM->m_nIdxMidClass		= BM_MID_SLABCARE;
	pBM->m_sMidClass		= GetStringMidClass(pBM->m_nIdxMidClass);
	pBM->m_nTypeResult		= RESULT_AREA;
	pBM->m_sUnit			= "㎡";
	pBM->m_nShape			= dLen[iLEFT]==dLen[iRIGHT] ? SHAPE_TV_RECTANGLE : SHAPE_TV_TRAPEZOID;
	pBM->m_dV[WidthLower]	= Round(toM(dLen[iRIGHT]),3);
	pBM->m_sVC[WidthLower]	= "";
	pBM->m_dV[WidthUpper]	= Round(toM(dLen[iLEFT]),3);
	pBM->m_sVC[WidthUpper]	= "";
	pBM->m_dV[Height]		= Round(toM(dWidthSlab),3);
	pBM->m_sVC[Height]		= "";
	pBM->m_dwSide			= SIDE_FRONT;
	pBM->m_sNoteHead		= "방호난간 및 중분대 접지면을 제외한 슬래브 상판 면적";
	pBM->m_sInfo			= "콘크리트양생(거푸집)";
	pBM->CalculateResult();
	pBM->m_sVCN[Result]		= "슬래브양생";
	m_pArrBMBridgeStd.Add(pBM);
}

// 낙하물 방지공
// 강재동바리 폭 * (슬래브폭 + 1.0M)
// 확폭이 안되므로 선형기준 좌측과 우측을 기준으로한 동바리 폭의 평균으로 사용한다.
// void COutBM::MakeBMAntiDropping()
// {	
// 	CRcBridgeRebar *pBri	= GetRcBridgeRebar();
// 	if(!pBri) return;
// 
// 	// 선형기준 좌우측의 동바리 폭을 구한다.
// }

// 말뚝 수량(이함수는 차후에 검증이 되면 엔진으로 올라가야 마땅함)
void COutBM::MakeBMPile()
{
	CRcBridgeRebar *pBri	= GetRcBridgeRebar();
	if(!pBri) return;

	CHgBaseBMStd baseBM;
	CString strFooting = _T("");

	BOOL bBoxType		= pBri->IsBoxType();
	long nCountJijum	= bBoxType? 1 : pBri->GetCountJijum();
	long nIdxMidClass	= 0;
	
	// nCase = 0 : 자재비(직항)
	// nCase = 1 : 천공비
	// nCase = 2 : 시멘트 밀크량
	// nCase = 3 : 선단 및 두부보강
	// nCase = 4 : 말뚝이음
	// nCase = 5 : 정재하시험비
	// nCase = 6 : 동재하시험비
	// nCase = 7 : 횡재하시험비
	for(long nCase=0; nCase<7; nCase++)
	{
		long j = 0; for(j = 0; j < nCountJijum; j++)
		{
			CFootingApp *pFooting	= pBri->GetFootingByJijumNumber(j);
			if(!pFooting) continue;
			if(!pFooting->m_bIs) continue;

			long nTypeExFooting = bBoxType? pBri->m_nTypeLower : pFooting->m_exFooting.m_nType;
			if(nTypeExFooting != EXFOOTING_TYPE_PILE) continue;

			if(pFooting->m_exFooting.m_Pile.m_nType != 2)
			{
				if(bBoxType)
				{
					strFooting = _T("");
				}
				else
				{
					if(j == 0)
						strFooting = _T("시점");
					else if(j == nCountJijum-1)
						strFooting = _T("종점");
					else
						strFooting.Format("중간지점%d", j+1);
				}

				CMakeBMExPile makeBMExPile(&pFooting->m_exFooting.m_Pile, BM_BIG_PILE);
				makeBMExPile.SetFooting(pFooting);
				makeBMExPile.SetTypeMilkBM(pBri->m_nTypePileMilkBM);
				makeBMExPile.SetBoring(pBri->GetBoringDataExByJijumNumber(j), toM(pFooting->m_dEL - pFooting->GetHeight()), pFooting->m_exFooting.m_dEFHR);
				makeBMExPile.SetTypeBMApply(m_pStd->m_pARcBridgeDataStd->m_nTypeBMApply);
				makeBMExPile.SetTypeUnit(m_pStd->m_pARcBridgeDataStd->m_nTypeUnit);
				makeBMExPile.SetWeightUnitLean(m_pStd->m_pARcBridgeDataStd->m_pBasicConcInfo->m_UWeightConcrete);
					

				long nCount	= m_pArrBMBridgeStd.GetSize();
				makeBMExPile.MakeBMPileStd(&m_pArrBMBridgeStd, nCase, strFooting);

				// 새로구한 말뚝의 mid인덱스를 하나씩 증가시켜준다.(중복되지 않게 하기 위해서)
				// 보링정보 문제도 있기 때문에 무조건 분리해서 출력함.
				long i = 0; for(i = nCount; i < m_pArrBMBridgeStd.GetSize(); i++)
					m_pArrBMBridgeStd.GetAt(i)->m_nIdxMidClass	= nIdxMidClass;
			}
			else // 현장타설말뚝
			{
				if(nCase == 0)
				{
					CMakeBMAloneHyuntaPile makeBM;
					makeBM.SetBM(&m_pArrBMBridgeStd);
					makeBM.SetColumnExPlus(NULL);
					makeBM.SetBoringEx(pFooting->m_pExBoring);
					makeBM.SetAggregate(pBri->GetValueAggregateHyunta());
					makeBM.SetSlump(pBri->GetValueSlumpHyunta());
					makeBM.SetTypeBMApply(pBri->m_pARcBridgeDataStd->m_nTypeBMApply);
					makeBM.SetTypeUnit(pBri->m_pARcBridgeDataStd->m_nTypeUnit);
					makeBM.SetTypeRoundRebarLength(pBri->m_pARcBridgeDataStd->m_pBasicConcInfo->m_nRoundRebarLength);
					makeBM.SetRemainSoil(pBri->m_dSurplusSoil_HyunTaPile);
					makeBM.SetAddRateBig(pBri->m_dConcreteSoil_HyunTaPile);		
					makeBM.SetAddRateRcd(pBri->m_dConcreteRock_HyunTaPile);		

					makeBM.SetIdxBigClass(BM_BIG_HYUNTA);

					makeBM.SetIdxMidClass(AHP_MID_STEEL_INNER_DIGGING_BIG, BM_MID_STEEL_INNER_DIGGING_BIG);	// 강관내부굴착(대구경천공)
					makeBM.SetIdxMidClass(AHP_MID_STEEL_INNER_DIGGING_RCD, BM_MID_STEEL_INNER_DIGGING_RCD);	// 강관내부굴착(RCD천공)
					makeBM.SetIdxMidClass(AHP_MID_DIGGING_MACHINE_MOVE_DEPER, BM_MID_DIGGING_MACHINE_MOVE_DEPER);	// 굴착장비 이동및 거치
					makeBM.SetIdxMidClass(AHP_MID_PILE_BUILD, BM_MID_PILE_BUILD);			// 말뚝 조성공
					makeBM.SetIdxMidClass(AHP_MID_SPACER, BM_MID_SPACER_ALONE_HYUNTAPILE);	// SPACER 제작 및 설치
					makeBM.SetIdxMidClass(AHP_MID_REBAR_ANTI_RISE, BM_MID_REBAR_ANTI_RISE);	// 철근 오름 방지
					makeBM.SetIdxMidClass(AHP_MID_COPPING, BM_MID_COPPING);					// 두부 정리
					makeBM.SetIdxMidClass(AHP_MID_REMAIN_SOIL, BM_MID_REMAIN_SOIL);			// 잔토처리
					makeBM.SetIdxMidClass(AHP_MID_REBAR_ASSM, BM_MID_REBAR_ASSM_AHP);		// 철근가공조립
					makeBM.SetIdxMidClass(AHP_MID_WASTE, BM_MID_WASTE);						// 폐기물 처리
					makeBM.SetIdxMidClass(AHP_MID_SLIME, BM_MID_SLIME);						// slime 제거
					makeBM.SetIdxMidClass(AHP_MID_CONCRETE, BM_MID_CONCRETE_AHP);			// concrete 타설
					makeBM.SetIdxMidClass(AHP_MID_SURFACE, BM_MID_SURFACE_AHP);					// 면 정리
					makeBM.SetIdxMidClass(AHP_MID_REBAR_NET, BM_MID_REBAR_NET);				// 철근망 근입

					makeBM.SetPile(&pFooting->m_exFooting.m_Pile);
					makeBM.MakeBMAloneHyuntaPileStd();
				}
			}
		}
		
		nIdxMidClass++;
	}
}

// 교명판
void COutBM::MakeBMNamePanel()
{
	CRcBridgeRebar	*pBri	= GetRcBridgeRebar();
	if(!pBri) return;

	CBMData *pBM			= new CBMData;
	pBM->m_nIdxBigClass		= BM_BIG_NAMEPANEL;
	pBM->m_sBigClass		= GetStringBigClass(pBM->m_nIdxBigClass);
	pBM->m_nLevelBigClass	= GetLevelTotal(pBM->m_nIdxBigClass);
	pBM->m_nTypeResult		= RESULT_EA;
	pBM->m_sUnit			= "EA";
	pBM->m_dV[Ea]			= pBri->m_nNamePanel;
	pBM->CalculateResult();
	m_pArrBMBridgeStd.Add(pBM);
}

// 설명판
void COutBM::MakeBMDesPanel()
{
	CRcBridgeRebar	*pBri	= GetRcBridgeRebar();
	if(!pBri) return;
	
	CBMData *pBM			= new CBMData;
	pBM->m_nIdxBigClass		= BM_BIG_DESPANEL;
	pBM->m_sBigClass		= GetStringBigClass(pBM->m_nIdxBigClass);
	pBM->m_nLevelBigClass	= GetLevelTotal(pBM->m_nIdxBigClass);
	pBM->m_nTypeResult		= RESULT_EA;
	pBM->m_sUnit			= "EA";
	pBM->m_dV[Ea]			= pBri->m_nDesPanel;
	pBM->CalculateResult();
	m_pArrBMBridgeStd.Add(pBM);
}

// TBM설치
// 개수 1개로 고정되어 있음
// 변수만 바꿔주면 됨
// 국도건설공사 설계실무 요령 P298
// - 100m미만 교량은 1개소, 이상은 2개소
// - 단, 분리 교량일 경우 한덩어리로 보면 되므로, 상행선에만 설치
void COutBM::MakeBMTBM()
{
	CRcBridgeRebar	*pBri	= GetRcBridgeRebar();
	if(!pBri) return;
	if(pBri->m_nSeparBri == 2) return;	// 하행인 경우 구하지 않음

	long nCountTBM	= 1;
	if(pBri->m_dLengthBridge <= 100000)
		nCountTBM	= 1;
	else
		nCountTBM	= 2;


	CBMData *pBM			= new CBMData;
	pBM->m_nIdxBigClass		= BM_BIG_TBM;
	pBM->m_sBigClass		= GetStringBigClass(pBM->m_nIdxBigClass);
	pBM->m_nLevelBigClass	= GetLevelTotal(pBM->m_nIdxBigClass);
	pBM->m_nTypeResult		= RESULT_EA;
	pBM->m_sUnit			= "EA";
	pBM->m_dV[Ea]			= nCountTBM;
	pBM->CalculateResult();
	m_pArrBMBridgeStd.Add(pBM);
}

// 접속슬래브 거푸집
void COutBM::MakeBMFormAps()
{
	CRcBridgeRebar	*pBri	= GetRcBridgeRebar();
	if(!pBri) return;
	if(!pBri->m_bSelectBM_ApsCnsInclude) return;

	// 접속슬래브 거푸집 표준 제원
	CBMData bmBase;
	bmBase.m_nIdxBigClass		= BM_BIG_FORM;
	bmBase.m_sBigClass			= GetStringBigClass(bmBase.m_nIdxBigClass);
	bmBase.m_nLevelTotal		= GetLevelTotal(bmBase.m_nIdxBigClass);
	bmBase.m_nIdxMidClass		= GetFormByPos(FORM_POS_APS);
	bmBase.m_sMidClass			= GetStringMidClass(bmBase.m_nIdxMidClass);
	bmBase.m_nIdxSmallClass		= BM_SMALL_FORM_VERTICAL;
	bmBase.m_sSmallClass		= GetStringSmallClass(bmBase.m_nIdxSmallClass);
	bmBase.m_nTypeResult		= RESULT_AREA;
	bmBase.m_sUnit				= "㎡";
	bmBase.m_dwSide				= SIDE_FRONT;

	long stt = 0; for(stt = 0; stt < 2; stt++)
	{
		BOOL bStt	= stt == iSTT ? TRUE : FALSE;

		for(long left = 0; left < 2; left++)
		{	
			for(long cns = -1; cns < 2; cns++)
			{
				long nNum	= 1;
				BOOL bCns	= cns != -1 ? TRUE : FALSE;
				CTypedPtrArray <CObArray, CBMData*> *pArrBMStd	= NULL;
				CApsApp *pAps	= NULL;
				if(bCns)
				{
					pAps	= bStt ? &pBri->m_cnsStt[left][cns] : &pBri->m_cnsEnd[left][cns];
					pArrBMStd	= &m_pArrBMCnsStd[stt][left][cns];
				}
				else
				{
					pAps	= bStt ? &pBri->m_apsStt[left] : &pBri->m_apsEnd[left];
					pArrBMStd	= &m_pArrBMApsStd[stt][left];
				}
				
				if(!pAps->m_bIs) continue;

				long	nSizeBM	= pArrBMStd->GetSize();
				double	dFirstMeter	= 7000;
				double	dSeconeMeter = 3000;

				// 완충슬래브가 있으면 버림콘크리트는 슬래브 면적과 동일해 진다.
				double dExtLean		= 100;
				double dThickLean	= 100;
				if((bStt && pBri->m_cnsStt[left][0].m_bIs) || (!bStt && pBri->m_cnsEnd[left][0].m_bIs))
					dExtLean	= 0;

				// 일단 날개벽이나 좌우측에 접속슬래브에 의한 스티로폴은 고려하지 않음

				// 1. 접속슬래브 끝면(끝면 길이에다가 두께 곱하면 됨, 빼거나 할 거 없음)
				CTwinVector tvEndPlane	= pAps->m_tvArrPlan.GetTvByInfo("접속슬래브끝면");
				CTwinVector tvEndSide	= pAps->m_tvArrSide.GetTvByInfo("접속슬래브끝단측면");
				if(tvEndPlane.GetLength() > 0 && tvEndSide.GetLength() > 0)
				{
					CBMData *pBMEnd				= new CBMData(&bmBase);
					pBMEnd->m_nNumbering		= nNum++;
					pBMEnd->m_dV[WidthLower]	= Round(toM(tvEndPlane.GetLength()),3);
					pBMEnd->m_dV[Height]		= Round(toM(tvEndSide.GetLength()),3);
					pBMEnd->m_nShape			= SHAPE_TV_RECTANGLE;
					pBMEnd->CalculateResult();
					pArrBMStd->Add(pBMEnd);

					// 완충슬래브가 없다면 끝면에도 버림콘크리트 거푸집 들어간다.
					if(dExtLean > 0 && !bCns && pAps->m_dH2 == 0)
					{
						CBMData *pBMEndLean				= new CBMData(pBMEnd);
						pBMEndLean->m_nIdxMidClass		= GetFormByPos(FORM_POS_LEAN);
						pBMEndLean->m_sMidClass			= GetStringMidClass(pBMEndLean->m_nIdxMidClass);
						pBMEndLean->m_dV[WidthLower]	+= Round(toM(dExtLean*2),3);
						pBMEndLean->m_dV[Height]		= Round(toM(dThickLean),3);
						pBMEndLean->CalculateResult();
						pBMEndLean->m_sInfo				= "버림콘크리트거푸집";
						pArrBMStd->Add(pBMEndLean);
					}
				}
				
				// 2. 접속슬래브 좌측면(나중에 날개벽이나 옆에 있는 접속슬래브와 겹치는 부분 빼야 됨)
				// 좌측면은 총 2개면이 나올 수 있음
				for(long leftSide = 0; leftSide < 2; leftSide++)
				{
					BOOL bLeftSide	= leftSide == iLEFT;
					CTwinVector tvSidePlane	= pAps->m_tvArrPlan.GetTvByInfo(bLeftSide ? "접속슬래브좌측" : "접속슬래브우측");
					
					// 윗면(기본)
					CBMData *pBMLeft			= new CBMData(&bmBase);
					pBMLeft->m_nNumbering		= nNum++;
					pBMLeft->m_dV[WidthLower]	= Round(toM(tvSidePlane.GetLength()),3);
					pBMLeft->m_dV[Height]		= Round(toM(tvEndSide.GetLength()),3);
					pBMLeft->m_nShape			= SHAPE_TV_RECTANGLE;
					pBMLeft->CalculateResult();
					pArrBMStd->Add(pBMLeft);

					// 버럼콘크리트(완충슬래브 존재 여부에 따라서 버림콘크리트 길이만 더해서 적용함)
					// 헌치가 있으면 버림콘크리트도 없음
					if(!bCns && pAps->m_dH2 == 0)
					{
						CBMData *pBMLeftLean		= new CBMData(pBMLeft);
						pBMLeftLean->m_nIdxMidClass	= GetFormByPos(FORM_POS_LEAN);;
						pBMLeftLean->m_sMidClass	= GetStringMidClass(pBMLeftLean->m_nIdxMidClass);
						pBMLeftLean->m_dV[WidthLower]	+= Round(toM(dExtLean),3);
						pBMLeftLean->m_dV[Height]		= Round(toM(dThickLean),3);
						pBMLeftLean->CalculateResult();
						pBMLeftLean->m_sInfo		= "버림콘크리트거푸집";
						pArrBMStd->Add(pBMLeftLean);
		
						/*
						// 헌치부 
						// 헌치부는 날개벽의 각도에 따라서 사각적용된 길이가 될 수 있다.
						if(pAps->m_dH2 != 0)
						{
							// L1과 L1+L2의 길이 구함
							double dL12	= pAps->m_dL1 + pAps->m_dL2;
							double dL1	= pAps->m_dL1;
							CDPoint xyMatch(0, 0);

							CTwinVector tvOrg	= pAps->m_tvArrPlan.GetTvByInfo("기준면");
							CTwinVector tvTmp	= tvOrg;
							Offset(tvTmp.m_v1, tvTmp.m_v2, dL1);
							GetXyMatchLineAndLine(tvTmp.m_v1, tvTmp.GetXyDir(), tvSidePlane.m_v1, tvSidePlane.GetXyDir(), xyMatch);
							dL1	= ~(xyMatch-tvSidePlane.m_v1);
							tvTmp	= tvOrg;
							Offset(tvTmp.m_v1, tvTmp.m_v2, dL12);
							GetXyMatchLineAndLine(tvTmp.m_v1, tvTmp.GetXyDir(), tvSidePlane.m_v1, tvSidePlane.GetXyDir(), xyMatch);
							dL12	= ~(xyMatch-tvSidePlane.m_v1);

							CBMData *pBMHunch			= new CBMData(&bmBase);
							pBMHunch->m_nNumbering		= nNum++;
							pBMHunch->m_dV[WidthLower]	= Round(toM(dL1),3);
							pBMHunch->m_dV[WidthUpper]	= Round(toM(dL12),3);
							pBMHunch->m_dV[Height]		= Round(toM(pAps->m_dH2),3);
							pBMHunch->m_nShape			= SHAPE_TV_TRAPEZOID;
							pBMHunch->CalculateResult();
							pArrBMStd->Add(pBMHunch);
						}
						*/
					}

					// 접속슬래브 좌우측에 있는 스티로폼 수량만큼 공제해야 됨.
					// 따로 수식을 구하지 않고, 이미 구해진 수량을 이용하면됨
					// 따라서, 접속슬래브보다 스티로폼 수량을 먼저 구해야 됨.
					CString sInfo	= _T("");
					sInfo.Format("거푸집공제용스티로폼(%s)", bLeftSide ? "좌측" : "우측");
					long nSize	= pArrBMStd->GetSize();
					for(long k = 0; k < nSize; k++)
					{
						CBMData *pBM	= pArrBMStd->GetAt(k);
						if(pBM->m_sInfo == sInfo)
						{
							CBMData *pBMTmp	= new CBMData(pBM);
							pBMTmp->m_nIdxBigClass		= BM_BIG_FORM;
							pBMTmp->m_sBigClass			= GetStringBigClass(pBMTmp->m_nIdxBigClass);
							pBMTmp->m_nLevelTotal		= GetLevelTotal(pBMTmp->m_nIdxBigClass);
							pBMTmp->m_nIdxMidClass		= GetFormByPos(FORM_POS_APS);
							pBMTmp->m_sMidClass			= GetStringMidClass(pBMTmp->m_nIdxMidClass);
							pBMTmp->m_nIdxSmallClass	= BM_SMALL_FORM_VERTICAL;
							pBMTmp->m_sSmallClass		= GetStringSmallClass(pBMTmp->m_nIdxSmallClass);
							pBMTmp->m_nTypeResult		= RESULT_AREA;
							pBMTmp->m_sUnit				= "㎡";
							pBMTmp->m_dwSide				= SIDE_FRONT;
							pBMTmp->m_bIsUseForDeduct	= TRUE;
							pArrBMStd->Add(pBMTmp);
						}
					}
				}

				// 수량에 미터정보를 추가한다.
				RemakeByHeightBMList(pArrBMStd, dFirstMeter, dSeconeMeter, nSizeBM, pArrBMStd->GetSize()-1);
			}
		}
	}
}

// 접속슬래브 스티로폼
// 1. 접속슬래브와 교량 접속부
// 2. 접속슬래브와 날개벽 접속부
void COutBM::MakeBMStyrofoamAps()
{
	CRcBridgeRebar	*pBri	= GetRcBridgeRebar();
	if(!pBri) return;
	if(!pBri->m_bSelectBM_ApsCnsInclude) return;

	//{28042
	long nIdxMidClass = 0;
	
	if (pBri->m_nFormType_Aps == 0) 
		nIdxMidClass = BM_MID_FORM_6TIMES;
	else if(pBri->m_nFormType_Aps == 1)
		nIdxMidClass = BM_MID_FORM_4TIMES;
	else if(pBri->m_nFormType_Aps == 2)
		nIdxMidClass = BM_MID_FORM_3TIMES;
	else if(pBri->m_nFormType_Aps == 3)
		nIdxMidClass = BM_MID_FORM_PATTERN;
	else if(pBri->m_nFormType_Aps == 4)
		nIdxMidClass = BM_MID_FORM_GANGJE_S;
	else if(pBri->m_nFormType_Aps == 5)
		nIdxMidClass = BM_MID_FORM_CIRCLE;
	else 
		nIdxMidClass = BM_MID_FORM_EURO;
	//}

	// 접속슬래브 거푸집 표준 제원
	CBMData bmBase;
	bmBase.m_nIdxBigClass		= BM_BIG_STYROFOAM;
	bmBase.m_sBigClass			= GetStringBigClass(bmBase.m_nIdxBigClass);
	bmBase.m_nLevelTotal		= GetLevelTotal(bmBase.m_nIdxBigClass);

	//{ 28042
	bmBase.m_nIdxMidClass		= GetFormByPos(FORM_POS_APS);
	bmBase.m_sMidClass			= GetStringMidClass(nIdxMidClass);
	//}

	bmBase.m_nTypeResult		= RESULT_AREA;
	bmBase.m_sUnit				= "㎡";
	bmBase.m_dwSide				= SIDE_FRONT;
	// 예외적인 출력양식
// 	bmBase.m_nLevelMidClass		= 2; //#28042 접속 슬래브의 환경설정에 따른 설정을 할 수 없으므로 주석 처리하고 상위에 Mid클래스 설정으로 대신함

	CString str	= _T("");
	long nIdxSmallClass	= BM_SMALL_STYROFOAM;
	DWORD exceptionFlag	= (0xFFFFFFFF & ~(	0x00000001|0x00000040|0x00000002|0x00000080|0x00000100));

	long stt = 0; for(stt = 0; stt < 2; stt++)
	{
		BOOL bStt	= stt == iSTT ? TRUE : FALSE;

		for(long left = 0; left < 2; left++)
		{	
			CApsApp *pAps	= bStt ? &pBri->m_apsStt[left] : &pBri->m_apsEnd[left];
			if(!pAps->m_bIs) continue;
			pBri->SyncBridgeAps(left, TRUE);
			
			CTypedPtrArray <CObArray, CBMData*> *pArrBMStd	= &m_pArrBMApsStd[stt][left];

			double dH1	= pAps->m_dH1;
			double dH2	= pAps->m_dH2;
			double dAD1	= pAps->m_dAD1;
			double dAD2	= pAps->m_dAD2;

			// 1. 교량과 접속부(헌치포함)
			CTwinVector tvOrgPlane		= pAps->m_tvArrPlan.GetTvByInfo("기준면");
			CTwinVector tvHunchLPlane	= pAps->m_tvArrPlan.GetTvByInfo("좌측날개벽헌치");
			CTwinVector tvHunchRPlane	= pAps->m_tvArrPlan.GetTvByInfo("우측날개벽헌치");
			
			CBMData *pBM1	= new CBMData(&bmBase);
			pBM1->m_nIdxSmallClass	= nIdxSmallClass;
			pBM1->m_sSmallClass.Format("T=%.0fMM", pAps->m_dST);
			pBM1->m_nShape			= SHAPE_TV_RECTANGLE;
			pBM1->m_sForm.Format("(%.3f", toM(tvOrgPlane.GetLength()));
			if(tvHunchLPlane.GetLength() > 0)
			{
				str.Format(" + %.3f", toM(tvHunchLPlane.GetLength()));
				pBM1->m_sForm	+= str;
			}
			if(tvHunchRPlane.GetLength() > 0)
			{
				str.Format(" + %.3f", toM(tvHunchRPlane.GetLength()));
				pBM1->m_sForm	+= str;
			}
			str.Format(" x %.3f", toM(dH1 + dH2));
			pBM1->m_sForm += ")" + str;
			pBM1->SetResult(toM((tvOrgPlane.GetLength() + tvHunchLPlane.GetLength() + tvHunchRPlane.GetLength()) * toM(dH1 + dH2)));
			pArrBMStd->Add(pBM1);
		
			// 두께별로 smallclass인덱스를 설정해 줘야 된다. ///////////////////////
			long nIdxSameBM	= GetIdxSameBM(pArrBMStd, pBM1, exceptionFlag);
			if(nIdxSameBM > -1)
				pBM1->m_nIdxSmallClass	= pArrBMStd->GetAt(nIdxSameBM)->m_nIdxSmallClass;
			else
				nIdxSmallClass++;
			/////////////////////////////////////////////////////////////////////////

			CRebarPlacing rb;
			// 2. 좌측/우측 날개벽과 접속부
			// 떨어진 거리가 100이하이어야 들어감
			for(long leftSide = 0; leftSide < 2; leftSide++)
			{
				BOOL bLeftSide	= leftSide == iLEFT;
				double dAD		= bLeftSide	? dAD1 : dAD2;
				CString sLeft	= bLeftSide ? "좌측" : "우측";

				CWingWall *pWing	= pBri->GetWingWall(bStt, bLeftSide);
				if(!pWing) continue;

				if(dAD <= 100 && pBri->IsWingWall(bStt, bLeftSide) && pWing->m_nAttachPos == 0)
				{
					pBri->GetTvWingWallPlane(bStt, bLeftSide);
					CDPoint xyMatch(0, 0);
					CTwinVector tvSidePlaneAps	= pAps->m_tvArrPlan.GetTvByInfo("접속슬래브"+sLeft);
					CTwinVector tvSidePlaneWing	= pBri->m_tvArrPlaneWingWall.GetTvByInfo(sLeft+"날개벽내부헌치후");

					// 날개벽이 접속슬래브 측면과 수직일대는 구하지 않음
					CDPoint vAngSideAps	= tvSidePlaneAps.GetXyDir();
					CDPoint vAngSideWing	= tvSidePlaneWing.GetXyDir();

					// 수평일때 뽑아야 된다고 해놓고서는 수평이면 continue시키고 있었음
					// 그래서 수평이 아닐 경우에 continue시키도록 수정함(2009.05.19) 
					// 오차로 인해 정확히 수평검사하는건 무리임 - 오차1도로 함. (2009.09.22)
					if(!rb.IsParallel(vAngSideAps, vAngSideWing, 1)) continue;

					if(!GetXyMatchSegAndLine(tvSidePlaneAps.m_v1, tvSidePlaneAps.m_v2, tvSidePlaneWing.m_v1, tvSidePlaneAps.GetXyDir().Rotate90(), xyMatch))
						xyMatch	= tvSidePlaneAps.m_v2;
					double dLenOverlap	= ~(xyMatch-tvSidePlaneAps.m_v1);
					
					// 일반면
					CBMData *pBM2	= new CBMData(&bmBase);
					pBM2->m_nIdxSmallClass	= nIdxSmallClass;
					pBM2->m_sSmallClass.Format("T=%.0fMM", dAD);
					pBM2->m_nShape			= SHAPE_TV_RECTANGLE;
					pBM2->m_dV[WidthLower]	= Round(toM(dLenOverlap),3);
					pBM2->m_dV[Height]		= Round(toM(dH1),3);
					pBM2->CalculateResult();
					pBM2->m_sInfo.Format("거푸집공제용스티로폼(%s)", sLeft);
					pArrBMStd->Add(pBM2);

					// 두께별로 smallclass인덱스를 설정해 줘야 된다. ///////////////////////
					long nIdxSameBM	= GetIdxSameBM(pArrBMStd, pBM2, exceptionFlag);
					if(nIdxSameBM > -1)
						pBM2->m_nIdxSmallClass	= pArrBMStd->GetAt(nIdxSameBM)->m_nIdxSmallClass;
					else
						nIdxSmallClass++;
					/////////////////////////////////////////////////////////////////////////
					
					// 헌치부
					if(pAps->m_dL3 + pAps->m_dL2 > 0 && pAps->m_dH2 > 0)
					{
						// L3부분(직사각형)
						if(pAps->m_dL3 > 0)
						{
							CBMData *pBM3	= new CBMData(&bmBase);
							pBM3->m_nIdxSmallClass	= nIdxSmallClass;
							pBM3->m_sSmallClass.Format("T=%.0fMM", dAD);
							pBM3->m_nShape			= SHAPE_TV_RECTANGLE;
							pBM3->m_dV[WidthLower]	= Round(toM(min(dLenOverlap, pAps->m_dL3)),3);
							pBM3->m_dV[Height]		= Round(toM(dH2),3);
							pBM3->CalculateResult();
							pBM3->m_sInfo.Format("거푸집공제용스티로폼(%s)", sLeft);
							pArrBMStd->Add(pBM3);
						}
						
						// L2부분(삼각형 ~ 사다리꼴)
						if(pAps->m_dL2 > 0 && pAps->m_dL3 < dLenOverlap)
						{
							dLenOverlap	= dLenOverlap - pAps->m_dL3;
							if(dLenOverlap > pAps->m_dL2)	// 모두 포함하면 삼각형
							{
								CBMData *pBM4	= new CBMData(&bmBase);
								pBM4->m_nIdxSmallClass	= nIdxSmallClass;
								pBM4->m_sSmallClass.Format("T=%.0fMM", dAD);
								pBM4->m_nShape			= SHAPE_TV_TRIANGLE;
								pBM4->m_dV[WidthLower]	= Round(toM(min(dLenOverlap, pAps->m_dL2)),3);
								pBM4->m_dV[Height]		= Round(toM(dH2),3);
								pBM4->CalculateResult();
								pBM4->m_sInfo.Format("거푸집공제용스티로폼(%s)", sLeft);
								pArrBMStd->Add(pBM4);	
							}
							else	// 아니면 사다리꼴
							{
								CBMData *pBM4	= new CBMData(&bmBase);
								pBM4->m_nIdxSmallClass	= nIdxSmallClass;
								pBM4->m_sSmallClass.Format("T=%.0fMM", dAD);
								pBM4->m_nShape			= SHAPE_TRAPEZOID;
								pBM4->m_dV[WidthLower]	= Round(toM(dH2),3);
								pBM4->m_dV[Height]		= Round(toM(dLenOverlap),3);
								pBM4->m_dV[WidthUpper]	= (dH2 * (pAps->m_dL2 - dLenOverlap)) / pAps->m_dL2;
								pBM4->m_dV[WidthUpper]	= Round(toM(pBM4->m_dV[WidthUpper]), 3);
								pBM4->CalculateResult();
								pBM4->m_sInfo.Format("거푸집공제용스티로폼(%s)", sLeft);
								pArrBMStd->Add(pBM4);	
							}
						}
					}
				}
			}
		}
	}
}

// 같은 수량이 있는지 검사해서 인덱스 리턴
// long nLevel : 0 : big, 1 : mid, 2 : small 까지 비교
long COutBM::GetIdxSameBM(CTypedPtrArray <CObArray, CBMData*> *pArrBM, CBMData *pBM, DWORD exceptionFlag)
{
	long nIdx	= -1;
	long nSize	= pArrBM->GetSize();

	long i = 0; for(i = 0; i < nSize; i++)
	{
		CBMData *pBMTmp	= pArrBM->GetAt(i);
				
		if(pBM->IsSameBM(pBMTmp, exceptionFlag))
		{
			nIdx	= i;
			break;
		}
	}

	return nIdx;
}

// 접속슬래브 간격재
// 슬래브용(접속슬래브 평면 면적)
void COutBM::MakeBMSpacerAps()
{
	CRcBridgeRebar	*pBri	= GetRcBridgeRebar();
	if(!pBri) return;
	if(!pBri->m_bSelectBM_ApsCnsInclude) return;

	CString sItemClass	= _T("");
	long stt = 0; for(stt = 0; stt < 2; stt++)
	{
		BOOL bStt	= stt == iSTT ? TRUE : FALSE;

		for(long left = 0; left < 2; left++)
		{
			BOOL bLeft	= left == iLEFT ? TRUE : FALSE;

			CApsApp *pAps	= bStt ? &pBri->m_apsStt[left] : &pBri->m_apsEnd[left];
			if(!pAps->m_bIs) continue;

			CTypedPtrArray <CObArray, CBMData*> *pArrBMStd	= &m_pArrBMApsStd[stt][left];

			// 접속슬래브 평면 영역 구하기 .
			GetTvAreaShapeApsPlane(&m_pArrAreaConc, bStt, bLeft);

			long nSize	= m_pArrAreaConc.GetSize();
			long i = 0; for(i = 0; i < nSize; i++)
			{
				CTwinVectorAreaShape *pArea	= m_pArrAreaConc.GetAt(i);

				CBMData *pBM	= new CBMData;
				pBM->m_nNumbering			= pArea->m_nNum;
				pBM->m_nIdxBigClass			= BM_BIG_SPACER;
				pBM->m_sBigClass			= GetStringBigClass(pBM->m_nIdxBigClass);
				pBM->m_nLevelTotal			= GetLevelTotal(pBM->m_nIdxBigClass);
				pBM->m_nIdxSmallClass		= BM_SMALL_SPACER_SLAB;
				pBM->m_sSmallClass			= GetStringSmallClass(pBM->m_nIdxSmallClass);
				pBM->m_nTypeResult			= RESULT_AREA;
				pBM->m_sUnit				= "㎡";
				pBM->m_nShape				= pArea->m_nTypeShape;
				pBM->m_dV[WidthLower]		= Round(toM(pArea->m_dWidthLower),3);
				pBM->m_dV[WidthUpper]		= Round(toM(pArea->m_dWidthUpper),3);
				pBM->m_dV[WidthDiffLeft]	= Round(toM(pArea->m_dWidthDiffLeft),3);
				pBM->m_dV[WidthDiffRight]	= Round(toM(pArea->m_dWidthDiffRight),3);
				pBM->m_dV[Height]			= Round(toM(pArea->m_dHeight),3);
				pBM->m_dwSide				= SIDE_FRONT;
				pBM->CalculateResult();
				// 예외적인 출력양식
				pBM->m_nLevelMidClass		= 2;

				pArrBMStd->Add(pBM);
			}
		}
	}
}


// 날개벽 거푸집
void COutBM::MakeBMFormWing()
{
	CRcBridgeRebar *pBri	= GetRcBridgeRebar(TRUE);
	if(!pBri) return;
	
	CTwinVectorArray tvArrVertSection;
	tvArrVertSection	= pBri->m_tvArrVertSection;
	
	long nTypeJongdan	= pBri->m_nTypeJongdanForRebar;
	pBri->m_nTypeJongdanForRebar	= 2;

	long stt(0), left(0);
	long nSize(0), i(0);
	CTwinVector tv;
	CRebarPlacing rb;
	CHgBaseBMStd baseBM;

	for(stt = 0; stt < 2; stt++)
	{
		BOOL bStt	= stt == iSTT;

		for(left = 0; left < 2; left++)
		{
			BOOL bLeft	= left == iLEFT;
			if(!pBri->IsWingWall(bStt, bLeft)) continue;
			CTypedPtrArray <CObArray, CBMData*> *pArrBMStd	= &m_pArrBMWingStd[stt][left];

			long nIdxItemClass	= bStt ? (bLeft ? BM_ITEM_FORM_WING_SLF : BM_ITEM_FORM_WING_SRF) : 
										 (bLeft ? BM_ITEM_FORM_WING_ELF : BM_ITEM_FORM_WING_ERF);
			long nSizeBM		= pArrBMStd->GetSize();
			double dFirstMeter	= 7000;
			double dSeconeMeter	= 3000;

			CWingWall *pWing	= pBri->GetWingWall(bStt, bLeft);
						
			nSize	= pWing->m_pArrAreaConc.GetSize();
			if(nSize == 0) continue;

			double dYBase		= pWing->m_pArrAreaConc.GetAt(0)->m_tvArr.GetXyBottom().y;
			CBMData *pBMBackBaseForHunch	= NULL;	// 벽체와 접하는 부분의 헌치부 거푸집을 구하기 위한 base 수량

			baseBM.RemakeByHeightTvAreaShape(&pWing->m_pArrAreaConc, dYBase, dFirstMeter, dSeconeMeter);
			nSize	= pWing->m_pArrAreaConc.GetSize();
			for(i = 0; i < nSize; i++) 
			{
				double dThickWingWall	= pWing->m_nDirHunch == 1 ? pWing->m_dT2 : pWing->m_dT1;

				// 시점 종점.. 그리고 
				CTwinVectorAreaShape *pArea	= pWing->m_pArrAreaConc.GetAt(i);
				
				double dSttMeter	= pArea->GetTvLeft().GetXyBottom().y - dYBase;
				double dEndMeter	= pArea->GetTvLeft().GetXyTop().y - dYBase;

				// 단면변화선이 나오는  순간부터 날개벽 두께는 dT1을 적용한다.
				BOOL bChangleSection	= pArea->GetTvLower().m_sInfo.Find("단면변화선", 0) != -1;
				if(bChangleSection)
				{
					dThickWingWall	= pWing->m_dT1;
				}
				
				// 끝단이 직각처리인경우 두께에 사각을 적용해줘야 한다.
				BOOL bRAngEnd	= pWing->m_bRAngEnd;
				CDPoint vAng	= CDPoint(0, 1);
				if(!bRAngEnd)
				{
					// 여기코드는 평면좌표 구하는 데서 발췌해옴
					CDPoint A[4];
					pBri->GetXySss(bStt ? BRIDGE_OFF_STT_STT : BRIDGE_OFF_END_END, A);
					// 외측 점 좌표
					int nJ = bStt ? 0 : pBri->m_nQtyJigan;
					CDPoint xyOutStt	= bStt ? A[3] : A[0];
					CDPoint xyOutEnd	= pBri->GetXyPlaneWingWallEnd(bStt, bLeft);
					CDPoint vAngJijum	= pBri->GetAngleJijum(nJ);//pBri->GetAngleAzimuthJijum(nJ);				// 슬래브 끝단 각도
					
					// 각도 설정
					CDPoint	vAng = ToDPointFrDegree(vAngJijum.GetAngleDegree()+pWing->m_vAng.GetAngleDegree() - (pWing->m_nAttachPos == 1 ? 90 : 0));	// 좌측 날개벽 단부 각도
					dThickWingWall /= vAng.y;
				}

				// 날개벽 시종점좌우측에 따른 거푸집 설정 위치
				long nPosFront	= bStt ? (bLeft ? FORM_POS_WINGSL_FRONT : FORM_POS_WINGSR_FRONT) : 
										(bLeft ? FORM_POS_WINGEL_FRONT : FORM_POS_WINGER_FRONT);
				long nPosBack	= bStt ? (bLeft ? FORM_POS_WINGSL_BACK : FORM_POS_WINGSR_BACK) : 
										(bLeft ? FORM_POS_WINGEL_BACK : FORM_POS_WINGER_BACK);

				// 전면
				CBMData *pBM				= new CBMData;
				pBM->m_nNumbering			= pArea->m_nNum;
				pBM->m_nIdxBigClass			= BM_BIG_FORM;
				pBM->m_nLevelTotal			= GetLevelTotal(pBM->m_nIdxBigClass);
				pBM->m_sBigClass			= GetStringBigClass(pBM->m_nIdxBigClass);
				pBM->m_nIdxMidClass			= GetFormByPos(nPosFront);
				pBM->m_sMidClass			= GetStringMidClass(pBM->m_nIdxMidClass);
				pBM->m_nIdxSmallClass		= BM_SMALL_FORM_VERTICAL;
				pBM->m_sSmallClass			= GetStringSmallClass(pBM->m_nIdxSmallClass);
				pBM->m_nIdxItemClass		= nIdxItemClass;
				pBM->m_sItemClass			= GetStringItemClass(pBM->m_nIdxItemClass);
				pBM->m_nTypeResult			= RESULT_AREA;
				pBM->m_sUnit				= "㎡";
				pBM->m_nShape				= pArea->m_nTypeShape;
				pBM->m_dV[SttMeter]			= Round(toM(dSttMeter),3);
				pBM->m_sVC[SttMeter]		= _T("");
				pBM->m_dV[EndMeter]			= Round(toM(dEndMeter),3);
				pBM->m_sVC[EndMeter]		= _T("");
				pBM->m_dV[WidthLower]		= Round(toM(pArea->m_dWidthLower),3);
				pBM->m_sVC[WidthLower]		= _T("");
				pBM->m_dV[WidthUpper]		= Round(toM(pArea->m_dWidthUpper),3);
				pBM->m_sVC[WidthUpper]		= _T("");
				pBM->m_dV[WidthDiffLeft]	= Round(toM(pArea->m_dWidthDiffLeft),3);
				pBM->m_sVC[WidthDiffLeft]	= _T("");
				pBM->m_dV[WidthDiffRight]	= Round(toM(pArea->m_dWidthDiffRight),3);
				pBM->m_sVC[WidthDiffRight]	= _T("");
				pBM->m_dV[Height]			= Round(toM(pArea->m_dHeight),3);
				pBM->m_sVC[Height]			= _T("");
				pBM->m_dV[Length]			= Round(toM(dThickWingWall),3);
				pBM->m_sVC[Length]			= _T("");
				pBM->m_dwSide				= SIDE_FRONT;
				
				pBM->CalculateResult();
				pBM->m_dV[Result] = Round(pBM->m_dV[Result],3);
				pArrBMStd->Add(pBM);

				// 배면
				CBMData *pBMBack			= new CBMData(pBM);
				pBMBackBaseForHunch			= pBMBack;
				pBMBack->m_nIdxMidClass		= GetFormByPos(nPosBack);
				pBMBack->m_sMidClass		= GetStringMidClass(pBMBack->m_nIdxMidClass);
				pBMBack->m_nIdxItemClass	++;
				pBMBack->m_sItemClass		= GetStringItemClass(pBMBack->m_nIdxItemClass);
				
				// 1. 배면 구할때 단면변화선이 있는 경우 단면 변화선 만큼 구하지 않는다
				if(bChangleSection)
				{
					CTwinVectorArray tvArrSec;
					pBri->GetTvWingWallSection(bStt, bLeft, tvArrSec);
					CTwinVector tv	= tvArrSec.GetTvByInfo("날개벽단면내측헌치");
					if(tv.GetHorLength() > 0 && tv.GetVerLength() > 0)
					{
						pBMBack->m_dV[Height]	-= Round(toM(tv.GetVerLength()),3);
						pBMBack->m_dV[SttMeter]	+= Round(toM(tv.GetVerLength()),3);
						if(pBMBack->m_dV[Height] < 0) pBMBack->m_dV[Height]	= 0;
					}
				}

				// 2. 배면 구할때 기초와 겹치는 부분의 헌치높이 만큼은 빼줘야 한다.
				// 33072 배면의 높이를 구할때 헌치 높이 만큼 빼줘야 하는 것이 문제가 되기때문에 주석 처리함
				//향후 다시 문제가 되었을 시에는 다른 방법으로 아래를 수정해야 함
// 				if(pArea->m_tvArr.GetTvByInfo("날개벽정면하단", tv))
// 				{
// 					pBMBack->m_dV[Height]	-= Round(toM(pWing->m_dHuH1),3);
// 					pBMBack->m_sVC[Height]	= _T("");
// 				}

				// 3. 배면 구할때 벽체와 겹치는 부분의 헌치너비 만큼 빼줘야 한다.
				if(pArea->m_tvArr.GetTvByInfo("벽체", tv))
				{
					CTwinVector tvH;
					CString sLeft	= bLeft ? "좌측" : "우측";
					pBri->GetTvWingWallPlane(bStt, TRUE);
					if(pBri->m_tvArrPlaneWingWall.GetTvByInfo(sLeft+"날개벽시점헌치", tvH))
					{
						pBMBack->m_dV[WidthLower]	-= Round(toM(pWing->m_dHuW2),3);
						pBMBack->m_dV[WidthUpper]	-= Round(toM(pWing->m_dHuW2),3);
					}
				}				

				// 무늬 거푸집은 전면만
				// 삼각형이면 윗변까지 들어가므로...빼준다.
				if(pBMBack->m_nShape == SHAPE_TV_TRIANGLE || pBMBack->m_nShape == SHAPE_TV_REVTRIANGLE)
				{
					pBMBack->m_dwSide = SIDE_BACK;
				}
				else
				{
					bStt ? pBMBack->m_dwSide = SIDE_BACK | SIDE_LEFT : pBMBack->m_dwSide = SIDE_BACK | SIDE_RIGHT;
				}

				pBMBack->CalculateResult();
				pBMBack->m_sInfo = "날개벽거푸집(배면)";
				pArrBMStd->Add(pBMBack);

				// 배면에서 역삼각형인 경우에는 경사면이 추가 됨
				if(pBMBack->m_nShape == SHAPE_TV_REVTRIANGLE)
				{
					CBMData *pBMDataSlope	= new CBMData(pBMBack);
					bStt ? pBMDataSlope->m_dwSide = SIDE_LEFT : pBMDataSlope->m_dwSide = SIDE_RIGHT;
					pBMDataSlope->m_nIdxSmallClass		= BM_SMALL_FORM_SLOPE;
					pBMDataSlope->m_sSmallClass			= GetStringSmallClass(pBMDataSlope->m_nIdxSmallClass);
					pBMDataSlope->CalculateResult();

					pArrBMStd->Add(pBMDataSlope);
				}

				// 배면 구한뒤 공제한 곳에 대한 보충 (헌치부들)
				// 1. 배면을 구하면 단면 변화선이 있는 경우 거기 헌치부에 대한 경사면 거푸집 구해야 됨
				if(bChangleSection)
				{
					CTwinVectorArray tvArrSec;
					pBri->GetTvWingWallSection(bStt, bLeft, tvArrSec);
					CTwinVector tv	= tvArrSec.GetTvByInfo("날개벽단면내측헌치");
					if(tv.GetHorLength() > 0 && tv.GetVerLength() > 0)
					{
						CBMData *pBMHunch				= new CBMData(pBMBack);
						pBMHunch->m_nIdxSmallClass		= BM_SMALL_FORM_SLOPE;
						pBMHunch->m_sSmallClass			= GetStringSmallClass(pBMHunch->m_nIdxSmallClass);
						pBMHunch->m_nShape				= SHAPE_TV_TRIANGLE;
						pBMHunch->m_dV[WidthLower]		= Round(toM(tv.GetHorLength()),3);
						pBMHunch->m_dV[WidthDiffLeft]	= Round(toM(tv.GetHorLength()),3);
						pBMHunch->m_dV[WidthDiffRight]	= pBMHunch->m_dV[WidthLower] - pBMHunch->m_dV[WidthUpper];
						pBMHunch->m_dV[Height]			= Round(toM(tv.GetVerLength()),3);
						pBMHunch->m_dV[Length]			= Round(toM(pArea->m_dWidthLower),3);
						pBMHunch->m_dwSide				= SIDE_FRONT | SIDE_LEFT;
						pBMHunch->CalculateResult();	
						pBMHunch->m_sInfo				= "날개벽거푸집(배면헌치)";
						pArrBMStd->Add(pBMHunch);

						//33072 
						//헌치 부들에 대한 것을 구했다면 측면에 사각모양의 거푸집 수량을 더해 주어야 한다(변화구간)
						double dHeightRect = 0, dWidthRect = 0;
						CTwinVector tvLower, tvHeightRect;

						tvLower = pArea->GetTvLower();
						tvHeightRect = tv;

						dHeightRect = Round(toM(fabs(tv.GetXyBottom().y- tvHeightRect.GetXyTop().y)) , 3);
						dWidthRect = pWing->m_nDirHunch == 1 ? pWing->m_dT1 : pWing->m_dT2;
						dWidthRect = Round(toM(dWidthRect),3 );

						CBMData *pBMRect				= new CBMData(pBMBack);
						pBMRect->m_nIdxSmallClass		= BM_SMALL_FORM_VERTICAL;
						pBMRect->m_sSmallClass			= GetStringSmallClass(pBMRect->m_nIdxSmallClass);
						pBMRect->m_nShape				= RESULT_AREA;
						pBMRect->m_dV[WidthLower]		= dWidthRect;
						pBMRect->m_dV[Height]			= dHeightRect;
						pBMRect->m_dV[Length]			= dHeightRect;
						pBMRect->m_dwSide				= SIDE_FRONT ;
						pBMRect->CalculateResult();	
						pBMRect->m_sInfo				= _T(""); //이곳의 Info는 변경 가능
						pArrBMStd->Add(pBMRect);
					}
				}

				// 2. 배면 구할때 기초와 겹치는 부분의 헌치높이 만큼은 뺀거 복구
				if(pArea->m_tvArr.GetTvByInfo("날개벽정면하단", tv))
				{
					// 33072 
					if(dSttMeter == 0 && dEndMeter ==7000)
					{
						CBMData *pBMHunch				= new CBMData(pBMBack);
						pBMHunch->m_nIdxSmallClass		= BM_SMALL_FORM_SLOPE;
						pBMHunch->m_sSmallClass			= GetStringSmallClass(pBMHunch->m_nIdxSmallClass);
						pBMHunch->m_nShape				= SHAPE_TV_TRIANGLE;
						pBMHunch->m_dV[WidthLower]		= Round(toM(pWing->m_dHuW1),3);
						pBMHunch->m_dV[WidthDiffLeft]	= Round(toM(pWing->m_dHuW1),3);
						pBMHunch->m_dV[WidthDiffRight]	= pBMHunch->m_dV[WidthLower] - pBMHunch->m_dV[WidthUpper];
						pBMHunch->m_dV[Height]			= Round(toM(pWing->m_dHuH1),3);
						pBMHunch->m_dV[Length]			= Round(toM(pArea->m_dWidthLower),3);
						pBMHunch->m_dwSide				= SIDE_LEFT;
						pBMHunch->m_sInfo				= "날개벽거푸집(배면헌치)";
						pBMHunch->CalculateResult();
						pArrBMStd->Add(pBMHunch);
					}					
				}
			} // end of for(i = 0; i < nSize; i++)


			// 3. 벽체와 겹치는 부분의 헌치부는 날개벽별로 한번에 계산함.
			CTwinVectorArray tvArrWing;
			int se = bStt ? -1 : 1;
			int nLeft = bLeft ? -se : se;	// 시점일 경우 교량 진행 방향의 좌우와 날개벽의 좌우가 반대임.
			pBri->GetTvVertSection(nLeft, TRUE, TRUE);
			pBri->GetTvWingWallFront(bStt, bLeft, 1, tvArrWing, TRUE, FALSE, TRUE, TRUE);
			pBri->GetTvWingWallPlane(bStt);
			CTwinVector tvHunch	= pBri->m_tvArrPlaneWingWall.GetTvByInfo(bLeft ? "좌측날개벽시점헌치" : "우측날개벽시점헌치");
			double dLenHunch	= Round(toM(tvHunch.GetLength()),3);
			CTwinVectorArray tvArrHunch;
			for(long h = 0; h < tvArrWing.GetSize(); h++)
			{
				tv	= tvArrWing.GetAt(h).Sort(FALSE);
				if(tv.m_sInfo == "벽체")
					tvArrHunch.Add(tv);
			}

			CTwinVector tvCut;
			CDRect rect	= tvArrHunch.GetRect();
			rect.left -= 100;
			rect.right += 100;
			tvCut.m_v1.x = rect.left;
			tvCut.m_v2.x = rect.right;
			
			CTwinVectorArray tvArrTmp;
			double dCurMeter	= dYBase + dFirstMeter;
			while(tvArrHunch.GetSize() > 0)
			{
				tvCut.m_v1.y = dCurMeter;
				tvCut.m_v2.y = dCurMeter;

				tvArrTmp.RemoveAll();
				tvArrTmp.AddFromTvArray(tvArrHunch);

				// 잘라서 계산할꺼.. -> tvArrTmp
				// 자르고 남는거 -> tvArrHunch
				rb.TrimTvArrayByLine(tvArrTmp, tvCut.m_v1, tvCut.m_v2, TRUE, TRUE, FALSE);
				rb.TrimTvArrayByLine(tvArrHunch, tvCut.m_v1, tvCut.m_v2, FALSE, TRUE, FALSE);

				double dSttMeter	= tvArrTmp.GetXyBottom().y - dYBase;
				double dEndMeter	= tvArrTmp.GetXyTop().y - dYBase;

				// 수량 산출함
				// 수직면과 경사면 두가지 있을 수 있음
				CBMData *pBMVert = new CBMData(pBMBackBaseForHunch);
				pBMVert->m_dV[Result] = 0;		// 값이 들어있는 경우가 있어 초기화 시킨다.
				CBMData *pBMSlope = new CBMData(pBMBackBaseForHunch);
				pBMSlope->m_dV[Result] = 0;		// 값이 들어있는 경우가 있어 초기화 시킨다.

				pBMVert->m_nIdxSmallClass		= BM_SMALL_FORM_VERTICAL;
				pBMSlope->m_nIdxSmallClass		= BM_SMALL_FORM_SLOPE;
				pBMVert->m_sSmallClass			= GetStringSmallClass(pBMVert->m_nIdxSmallClass);
				pBMSlope->m_sSmallClass			= GetStringSmallClass(pBMSlope->m_nIdxSmallClass);

				CString str			= _T("");
				BOOL bFirstVert		= TRUE;
				BOOL bFirstSlope	= TRUE;
				for(long v = 0; v < tvArrTmp.GetSize(); v++)
				{
					tv	= tvArrTmp.GetAt(v);
					double dtvLength = Round(toM(tv.GetLength()),3);
					BOOL bVert	= Compare(tv.m_v1.x, tv.m_v2.x, "=");
					str.Format("%.3f", dtvLength);

					if(bVert)
					{
						pBMVert->m_sForm		+= (bFirstVert ? str : " + " + str);
						pBMVert->m_dV[Result]	+= Round(dtvLength,3);
						bFirstVert	= FALSE;
					}
					else
					{
						pBMSlope->m_sForm		+= (bFirstSlope ? str : " + " + str);
						pBMSlope->m_dV[Result]	+= Round(dtvLength,3);
						bFirstSlope	= FALSE;
					}
				}

				if(pBMVert->m_sForm != "")
				{
					pBMVert->m_nNumbering		= 99999;	// 99999는 넘버링으로 찍히면 안되고 정렬할때는 마지막으로 가야되는 경우 사용함
					pBMVert->m_dV[SttMeter]		= Round(toM(dSttMeter),3);
					pBMVert->m_sVC[SttMeter]	= _T("");
					pBMVert->m_dV[EndMeter]		= Round(toM(dEndMeter),3);
					pBMVert->m_sVC[EndMeter]	= _T("");
					str	= pBMVert->m_sForm;
					pBMVert->m_sForm.Format("(%s) x %.3f", str, dLenHunch);
					pBMVert->m_dV[Result] *= Round(dLenHunch,3);
					pBMVert->m_sNoteHead	= "헌치부";
					pArrBMStd->Add(pBMVert);
				}
				else 
					delete pBMVert;

				if(pBMSlope->m_sForm != "")
				{
					pBMSlope->m_nNumbering		= 99999;	// 99999는 넘버링으로 찍히면 안되고 정렬할때는 마지막으로 가야되는 경우 사용함
					pBMSlope->m_dV[SttMeter]	= Round(toM(dSttMeter),3);
					pBMSlope->m_sVC[SttMeter]	= _T("");
					pBMSlope->m_dV[EndMeter]	= Round(toM(dEndMeter),3);
					pBMSlope->m_sVC[EndMeter]	= _T("");
					str = pBMSlope->m_sForm;
					pBMSlope->m_sForm.Format("(%s) x %.3f", str, dLenHunch);
					pBMSlope->m_dV[Result] *= Round(dLenHunch,3);
					pBMSlope->m_sNoteHead	= "헌치부";
					pArrBMStd->Add(pBMSlope);
				}
				else
					delete pBMSlope;

				// 미터 올림
				dCurMeter += dSeconeMeter;
			}

			// 수량에 미터정보를 추가한다.
			RemakeByHeightBMList(pArrBMStd, dFirstMeter, dSeconeMeter, nSizeBM-1, pArrBMStd->GetSize()-1);
		}
	}

	pBri->m_tvArrVertSection	= tvArrVertSection;
	pBri->m_nTypeJongdanForRebar	= nTypeJongdan;
}

// 날개벽 스페이셔 수량(벽체용)
void COutBM::MakeBMSpacerWing()
{
	CRcBridgeRebar *pBri	= GetRcBridgeRebar(TRUE);
	if(!pBri) return;


	long stt = 0; for(stt = 0; stt < 2; stt++)
	{
		for(long left = 0; left < 2; left++)
		{
			CTypedPtrArray <CObArray, CBMData*> *pArrBMStd	= &m_pArrBMWingStd[stt][left];

			// 철근이 들어가는 부분의 거푸집을 모두 수집한다.
			// 수직,경사
			CTypedPtrArray <CObArray, CBMData*> pArrFormVert;
			long nSizeBM	= pArrBMStd->GetSize();
			long i = 0; for(i = 0; i < nSizeBM; i++)
			{
				CBMData *pBM = pArrBMStd->GetAt(i);
				if(pBM->m_nIdxBigClass == BM_BIG_FORM)
				{
					if(pBM->m_sSmallClass.Find("수평", 0) != -1) continue;

					CBMData *pForm = new CBMData;
					*pForm = *pBM;
					// 거푸짚 종류별로 묶어져 있으면 번호가 순서대로 정렬되지 않는다.
					// 그래서 여기서는 거푸집 종류를 모두 같은 것으로 해준다.
					pForm->m_sMidClass		= _T("");
					pForm->m_nIdxMidClass	= -1;
					pForm->m_sSmallClass	= _T("");
					pForm->m_nIdxSmallClass	= -1;
					pForm->m_sItemClass		= _T("");
					pForm->m_nIdxItemClass	= -1;
					pForm->m_sPartClass		= _T("");
					pForm->m_nIdxPartClass	= -1;
					pForm->m_sMeterClass	= _T("");
					pForm->m_nIdxMeterClass	= -1;

					pArrFormVert.Add(pForm);
				}
			}
			
			// 정렬
			CHgBaseBMStd baseBM;
			baseBM.ShellSortBMList(&pArrFormVert, &CBMData::CompareBM);

			// 벽체용 스페이셔 수량 생성
			if(pArrFormVert.GetSize() > 0)
			{
				CBMData *pWall				= new CBMData;
				pWall->m_nIdxBigClass		= BM_BIG_SPACER;
				pWall->m_sBigClass			= GetStringBigClass(pWall->m_nIdxBigClass);
				pWall->m_nLevelTotal		= GetLevelTotal(pWall->m_nIdxBigClass);
				pWall->m_nIdxSmallClass		= BM_SMALL_SPACER_WALL;
				pWall->m_sSmallClass		= GetStringSmallClass(pWall->m_nIdxSmallClass);
				pWall->m_nTypeResult		= RESULT_AREA;
				pWall->m_sUnit				= "㎡";
				pWall->m_sInfo				= "날개벽벽체용스페이셔";

				// 수집한 거푸집 수량들을 추가시킴
				CString str = _T("");
				nSizeBM	= pArrFormVert.GetSize();
				for(i = 0; i < nSizeBM; i++)
				{
					CBMData *pBM	= pArrFormVert.GetAt(i);
					pWall->m_dArrAddValue.Add(pBM->GetResult());
					
					str	= baseBM.GetCircleText(pBM->m_nNumbering);
					if(pWall->m_sForm.Find(str, 0) == -1)
					{
						CString strTmp = _T("");
						strTmp.Format(",%s", str);
						pWall->m_sForm += strTmp;
					}
				}
				
				str = pWall->m_sForm.Right(pWall->m_sForm.GetLength() - 1);
				pWall->m_sForm = (str + "의 거푸집 수량과 동일(매끈한 마감/무늬거푸집)");
				pWall->CalculateResult();
				// 예외적인 출력양식
				pWall->m_nLevelMidClass		= 2;				

				pArrBMStd->Add(pWall);
			}
			AhTPADelete(&pArrFormVert,(CBMData*)0);
		}
	}
}

// 콘크리트 양생 날개벽
// 날개벽 거푸집 배면, 상면 
void COutBM::MakeBMConCareWing()
{
	CRcBridgeRebar *pBri	= GetRcBridgeRebar(TRUE);
	if(!pBri) return;
	if(!pBri->m_bSelectBM_ConcCare_Main) return;

	long stt = 0; for(stt = 0; stt < 2; stt++)
	{
		BOOL bStt	= stt == iSTT;
		for(long left = 0; left < 2; left++)
		{
			BOOL bLeft	= left == iLEFT;
			if(!pBri->IsWingWall(bStt, bLeft)) continue;

			CWingWall *pWing	= pBri->GetWingWall(bStt, bLeft);
			if(!pWing) continue;

			CTypedPtrArray <CObArray, CBMData*> *pArrBMStd	= &m_pArrBMWingStd[stt][left];

			// 거푸집을 모두 수집한다. //////////////////////////////////////////////
			// 버림콘크리트및 mass콘크리트, 기초를 제외한 모든 거푸집을 구함
			CTypedPtrArray <CObArray, CBMData*> pArrForm;
			long nSizeBM	= pArrBMStd->GetSize();
			long i = 0; for(i = 0; i < nSizeBM; i++)
			{
				CBMData *pBM = pArrBMStd->GetAt(i);
				if(pBM->m_nIdxBigClass == BM_BIG_FORM)
				{
					if(pBM->m_sSmallClass.Find("수평", 0) != -1) continue;

					CBMData *pForm = new CBMData;
					*pForm = *pBM;
					// 거푸짚 종류별로 묶어져 있으면 번호가 순서대로 정렬되지 않는다.
					// 그래서 여기서는 거푸집 종류를 모두 같은 것으로 해준다.
					pForm->m_sMidClass		= _T("");
					pForm->m_nIdxMidClass	= -1;
					pForm->m_sSmallClass	= _T("");
					pForm->m_nIdxSmallClass	= -1;
					pForm->m_sItemClass		= _T("");
					pForm->m_nIdxItemClass	= -1;
					pForm->m_sPartClass		= _T("");
					pForm->m_nIdxPartClass	= -1;
					pForm->m_sMeterClass	= _T("");
					pForm->m_nIdxMeterClass	= -1;

					pArrForm.Add(pForm);
				}
			}
			
			// 정렬
			CHgBaseBMStd baseBM;
			baseBM.ShellSortBMList(&pArrForm, &CBMData::CompareBM);

			// 거푸집 수량 추가
			if(pArrForm.GetSize() > 0)
			{
				CBMData *pBM = new CBMData;
				pBM->m_nIdxBigClass		= BM_BIG_CONCCARE;
				pBM->m_sBigClass		= GetStringBigClass(pBM->m_nIdxBigClass);
				pBM->m_nLevelTotal		= GetLevelTotal(pBM->m_nIdxBigClass);
				pBM->m_nIdxMidClass		= BM_MID_CONCCARE;
				pBM->m_sMidClass		= GetStringMidClass(pBM->m_nIdxMidClass);
				pBM->m_nIdxSmallClass	= BM_SMALL_CONCCARE;
				pBM->m_sSmallClass		= GetStringSmallClass(pBM->m_nIdxSmallClass);
				pBM->m_nTypeResult		= RESULT_AREA;
				pBM->m_sUnit			= "㎡";
				pBM->m_sInfo			= "콘크리트양생(거푸집)";

				// 수집한 거푸집 수량들을 추가시킴
				CString str = _T("");
				nSizeBM	= pArrForm.GetSize();
				for(i = 0; i < nSizeBM; i++)
				{
					CBMData *pBMForm	= pArrForm.GetAt(i);
					pBM->m_dArrAddValue.Add(pBMForm->GetResult());
					
					str	= baseBM.GetCircleText(pBMForm->m_nNumbering);
					if(pBM->m_sForm.Find(str, 0) == -1)
					{
						CString strTmp = _T("");
						strTmp.Format(",%s", str);
						pBM->m_sForm += strTmp;
					}
				}
				
				str = pBM->m_sForm.Right(pBM->m_sForm.GetLength() - 1);
				pBM->m_sForm = (str + "의 거푸집(전면, 배면) 수량과 동일");
				
				pBM->m_bIsPrintByRefForm	= FALSE;
				pBM->CalculateResult();
				pArrBMStd->Add(pBM);
			}
			AhTPADelete(&pArrForm, (CBMData*)0);
			// 거푸집을 모두 수집한다. //////////////////////////////////////////////



			// 날개벽 상면 추가 /////////////////////////////////////////////////////
			// 날개벽 상면은 항상 마지막 영역이다.
			long nSize	= pWing->m_pArrAreaConc.GetSize();
			if(nSize == 0) continue;

			CTwinVectorAreaShape *pArea	= pWing->m_pArrAreaConc.GetAt(nSize-1);

			CBMData *pBM = new CBMData;
			pBM->m_nIdxBigClass		= BM_BIG_CONCCARE;
			pBM->m_nNumbering		= pArea->m_nNum;
			pBM->m_sBigClass		= GetStringBigClass(pBM->m_nIdxBigClass);
			pBM->m_nLevelTotal		= GetLevelTotal(pBM->m_nIdxBigClass);
			pBM->m_nIdxMidClass		= BM_MID_CONCCARE;
			pBM->m_sMidClass		= GetStringMidClass(pBM->m_nIdxMidClass);
			pBM->m_nIdxSmallClass	= BM_SMALL_CONCCARE;
			pBM->m_sSmallClass		= GetStringSmallClass(pBM->m_nIdxSmallClass);
			pBM->m_nTypeResult		= RESULT_AREA;
			pBM->m_sUnit			= "㎡";
			pBM->m_nShape			= SHAPE_TV_RECTANGLE;
			CTwinVectorArray tvArrFront;
			CTwinVectorArray tvArrVertSection;
			tvArrVertSection	= pBri->m_tvArrVertSection;
			pBri->GetTvVertSection(bLeft ? 0 : 1, TRUE, TRUE); //31503 
			pBri->GetTvWingWallFront(bStt, bLeft, 1, tvArrFront, TRUE, FALSE, TRUE, TRUE);
			pBri->m_tvArrVertSection	= tvArrVertSection;
			CTwinVector tvUpper;
			tvArrFront.GetTvByInfo("날개벽정면상단", tvUpper);
			pBM->m_dV[WidthLower]	= Round(toM(tvUpper.GetLength()),3);
			pBM->m_dV[Height]		= Round(toM(pWing->m_dT1),3);
			pBM->m_dwSide			= SIDE_FRONT;
			pBM->m_sInfo			= "콘크리트양생(거푸집)";
			pBM->CalculateResult();
			pArrBMStd->Add(pBM);		
			// 날개벽 상면 추가 /////////////////////////////////////////////////////
			
		}
	}
}

// 날개벽 영역 구분
// long nForUse : 0 : 원래 용도(콘크리트 수량)  
//                1 : 뒷채움용 날개벽 공제 : 뒷채움 영역과 교집합 부분만 구함
//                2 : 되메우기의 뒷채움용 날개벽 공제 : 지반선까지의 뒷채움 영역과 교집합 부분만 구함
//                3 (노상공제토용) : 노상 상하부(하부한계는 지반선)
//                4 (노체공제토용) : 노체 상하부(하부한계는 지반선)-----5까지 들어온다. nForUse+1
//				  6 : 되메우기의 구체 공제
void COutBM::CalAreaConcWing(CRcBridgeRebar *pBri, long nForUse)
{
	if(!pBri) return;

	CRebarPlacing rb;
	long nNum	= 0;
	long nIdx	= 0;

	// 날개벽
	long stt = 0; for(stt = 0; stt < 2; stt++)
	{
		BOOL bStt	= stt == iSTT;

		for(long left = 0; left < 2; left++)
		{
			BOOL bLeft	= left == iLEFT;

			CWingWall *pWing	= pBri->GetWingWall(bStt, bLeft);
			if(!pWing) continue;
			AhTPADelete(&pWing->m_pArrAreaConc,	(CTwinVectorAreaShape*)0);
			if(!pBri->IsWingWall(bStt, bLeft)) continue;

			nIdx	= 0;
			nNum	= 1;

			// 영역을 나눠서 tvAreaShape에 보관
			CTwinVectorArray tvArr;
			CTwinVectorArray tvArrResult;
			CDoubleArray dArrAngleSu;
			int se = bStt ? -1 : 1;
			int nLeft = bLeft ? -se : se;	// 시점일 경우 교량 진행 방향의 좌우와 날개벽의 좌우가 반대임.
			pBri->GetTvVertSection_Std(nLeft, TRUE, TRUE, FALSE, 2);
			//pBri->SyncBridge(nLeft, TRUE, TRUE);
			//pBri->SyncBridgeAps(nLeft, FALSE);

			// 뒷채움수량구할때 날개벽 수량을 필요로 할 때가 있다
			// 이 때를 위해서 뒷채움 수량에서 공재할 만큼만의 날개벽 수량을 영역으로 구분해 버린다.
			if(nForUse == 0)
				pBri->GetTvWingWallFront(bStt, bLeft, 1, tvArr, TRUE, FALSE, TRUE, TRUE);
			else if(nForUse == 6)
				pBri->GetTvReFillDeductOfWing(tvArr, bStt, bLeft);
			else
				pBri->GetTvBackFillDeductOfWing(tvArr, bStt, bLeft, nForUse-1);
				
			CTwinVector tvTemp;
			long n=0; for(n=0; n<tvArr.GetSize(); n++)
				tvTemp = tvArr.GetAt(n);

			rb.SeperatorTwinVectorArea(tvArr, tvArrResult, dArrAngleSu, 1);
			long j = 0; for(j = 0; j < dArrAngleSu.GetSize(); j++)
			{
				CTwinVectorAreaShape *pArea	= new CTwinVectorAreaShape(nNum++);
				for(long k = 0; k < (long)dArrAngleSu.GetAt(j); k++)
				{
					CTwinVector tv = tvArrResult.GetAt(nIdx++);
					pArea->AddTwinVector(tv);
				}

				pArea->Calculate();
				pWing->m_pArrAreaConc.Add(pArea);
			}

			// 날개벽은 영역 나눈 뒤에 단면변화선에 대해서 한번 더 잘라준다.
			// 현재는 세로변화만 지원함
			// 변화되는선은 "단면변화선"이라고 지정해둔다
			if(pWing->m_nDirHunch == 1)	// 세로변화
			{
				CTwinVectorArray tvArrSec;
				CTwinVector tv;
				pBri->GetTvWingWallSection(bStt, bLeft, tvArrSec);
				if(tvArrSec.GetTvByInfo("날개벽단면내측헌치", tv))
				{
					double dYCut	= tv.GetXyBottom().y;
					long i = 0; for(i = 0; i < pWing->m_pArrAreaConc.GetSize(); i++)
					{
						CTwinVectorAreaShape *pArea	= pWing->m_pArrAreaConc.GetAt(i);
						if(pArea->m_tvArr.GetXyBottom().y <= dYCut && pArea->m_tvArr.GetXyTop().y > dYCut)
						{
							CTwinVector tvLower	= pArea->GetTvLower();
							CTwinVector tvLeft	= pArea->GetTvLeft();
							CTwinVector tvRight	= pArea->GetTvRight();
							CTwinVector tvUpper	= pArea->GetTvUpper();
							pArea->m_tvArr.RemoveAll();

							if(Compare(tvLeft.GetXyBottom().y, dYCut, "="))
							{
								long nNum	= 1;	// 세로변화로 잘린뒤부터는 번호가 하나씩 증가 되어야 한다. 그래서 번호를 다시 부여

								tvLower.m_sInfo += "(단면변화선)";
								if(tvLower.GetLength() > 0) pArea->AddTwinVector(tvLower);
								if(tvLeft.GetLength() > 0) pArea->AddTwinVector(tvLeft);
								if(tvRight.GetLength() > 0) pArea->AddTwinVector(tvRight);
								if(tvUpper.GetLength() > 0) pArea->AddTwinVector(tvUpper);
								pArea->Calculate();
								pArea->m_nNum	= nNum++;
							}
							else
							{
								CDPoint xyMatchL(0, 0), xyMatchR(0, 0);
								GetXyMatchLineAndLine(tvLeft.m_v1, tvLeft.GetXyDir(), CDPoint(0, dYCut), CDPoint(-1, 0), xyMatchL);
								GetXyMatchLineAndLine(tvRight.m_v1, tvRight.GetXyDir(), CDPoint(0, dYCut), CDPoint(1, 0), xyMatchR);
								
								// 잘린거 아래는 기존, 위는 새로생성
								CTwinVector tvLowerTmp, tvLeftTmp, tvRightTmp, tvUpperTmp;

								// 아래꺼변경
								tvLowerTmp			= tvLower;
								tvUpperTmp.m_sInfo	= tvUpper.m_sInfo;
								tvUpperTmp.m_v1		= xyMatchL;
								tvUpperTmp.m_v2		= xyMatchR;
								tvLeftTmp.m_sInfo	= tvLeft.m_sInfo;
								tvLeftTmp.m_v1		= tvLeft.GetXyBottom();
								tvLeftTmp.m_v2		= xyMatchL;
								tvRightTmp.m_sInfo	= tvRight.m_sInfo;
								tvRightTmp.m_v1		= tvRight.GetXyBottom();
								tvRightTmp.m_v2		= xyMatchR;
								if(tvLowerTmp.GetLength() > 0) pArea->AddTwinVector(tvLowerTmp);
								if(tvRightTmp.GetLength() > 0) pArea->AddTwinVector(tvLeftTmp);
								if(tvRightTmp.GetLength() > 0) pArea->AddTwinVector(tvRightTmp);
								if(tvUpperTmp.GetLength() > 0) pArea->AddTwinVector(tvUpperTmp);
								pArea->Calculate();
								pArea->m_nNum		= nNum++;

								// 아래꺼 추가
								CTwinVectorAreaShape *pAreaNew	= new CTwinVectorAreaShape;
								tvLowerTmp.m_sInfo	= "(단면변화선)";
								tvLowerTmp.m_v1		= xyMatchL;
								tvLowerTmp.m_v2		= xyMatchR;
								tvLeftTmp.m_sInfo	= tvLeft.m_sInfo;
								tvLeftTmp.m_v1		= xyMatchL;
								tvLeftTmp.m_v2		= tvLeft.GetXyTop();
								tvRightTmp.m_sInfo	= tvRight.m_sInfo;
								tvRightTmp.m_v1		= xyMatchR;
								tvRightTmp.m_v2		= tvRight.GetXyTop();
								tvUpperTmp			= tvUpper;
								if(tvLowerTmp.GetLength() > 0) pAreaNew->AddTwinVector(tvLowerTmp);
								if(tvRightTmp.GetLength() > 0) pAreaNew->AddTwinVector(tvLeftTmp);
								if(tvRightTmp.GetLength() > 0) pAreaNew->AddTwinVector(tvRightTmp);
								if(tvUpperTmp.GetLength() > 0) pAreaNew->AddTwinVector(tvUpperTmp);
								pAreaNew->Calculate();
								pAreaNew->m_nNum	= nNum++;
								pWing->m_pArrAreaConc.InsertAt(i+1, pAreaNew);
								i++;
							}
						}
						else
							pArea->m_nNum	= nNum++;
					}
				}
				
			}
			else if(pWing->m_nDirHunch == 2) // 가로변화
			{
			}
			pBri->GetTvVertSection_Std(nLeft, TRUE, TRUE, TRUE, 2);
		}
	}
}

// 보호블럭 기초 콘크리트
// 보호블럭은 정해진 재료 양으로 구한다(체적으로 구하지 않음)
// 기초콘크리트 = 보호블럭 길이 * 미터당 재료량 
void COutBM::MakeBMConcBohoFooting()
{
	CRcBridgeRebar *pBri	= GetRcBridgeRebar();
	if(!pBri) return;
	long nTypeBoho	= pBri->m_nTypeRcBridgeBlock;
	if(nTypeBoho == 0) return;
	
	double dAngleBoho	= pBri->m_dAngleRcBridgeBlock;
	
	long stt = 0; for(stt = 0; stt < 2; stt++)
	{
		BOOL bStt	= stt == iSTT;
		long nJ			= bStt ? 0 : pBri->m_nQtyJigan;

		CWingWall *pWing	= NULL;
		if(pBri->IsWingWall(bStt, TRUE))
		{
			pWing	= pBri->GetWingWall(bStt, TRUE);
		}
		else if(pBri->IsWingWall(bStt, FALSE)) 
		{
			pWing	= pBri->GetWingWall(bStt, FALSE);
		}
		if(!pWing) continue;

		// 보호블럭 수평길이
		COutBMTogong *outBMTogong	= m_pStd->m_pArrOutBMTogong.GetAt(m_nBri);
		double dLenHor	= outBMTogong->GetLengthBohoHor(bStt);
		if(dLenHor <= 0) continue;
				
		long nCountWing			= 0;
		if(pBri->IsWingWall(bStt, TRUE)) nCountWing ++;
		if(pBri->IsWingWall(bStt, FALSE)) nCountWing ++;
		double dLenWall			= toM(pBri->GetTvJijum(nJ, 0).GetLength());
		double dLenBohoFooting	= toM(2*ConstPi*dLenHor*dAngleBoho*nCountWing/360) + dLenWall;	// 보호블럭기초 총 길이
		double dMaterialConc	= Compare(pWing->m_dSlopeNoriNext, 1.8, "=") ? m_pStd->m_dMaterialConc[1] : m_pStd->m_dMaterialConc[0];	// 미터당 콘크리트 재료량

		// 수량 구함(기초블럭콘크리트 미터당 량 * 총 길이)
		CBMData *pBM				= new CBMData;
		pBM->m_nIdxBigClass			= BM_BIG_CONCRETE;
		pBM->m_sBigClass			= GetStringBigClass(pBM->m_nIdxBigClass);
		pBM->m_nLevelTotal			= GetLevelTotal(pBM->m_nIdxBigClass);
		pBM->m_nIdxMidClass			= BM_MID_CONCRETE_BOHO;
		pBM->m_sMidClass			= GetStringMidClass(pBM->m_nIdxMidClass);
		pBM->m_nIdxSmallClass		= BM_SMALL_CONCRETE_BOHO_FOOTING;
		pBM->m_sSmallClass			= GetStringSmallClass(pBM->m_nIdxSmallClass);
		pBM->m_nTypeResult			= RESULT_VOLUMN;
		pBM->m_sUnit				= "㎥";
		pBM->m_sForm.Format("%.3f x %.3f", dLenBohoFooting, dMaterialConc);
		pBM->SetResult(dLenBohoFooting * dMaterialConc);
		pBM->m_bIsPrintByRefForm	= TRUE;
		m_pArrBMBohoStd[stt].Add(pBM);
	}
}

// 보호블럭
void COutBM::MakeBMBohoBlock()
{
	CRcBridgeRebar *pBri	= GetRcBridgeRebar();
	if(!pBri) return;
	double dAngleBoho		= pBri->m_dAngleRcBridgeBlock;
	
	long stt = 0; for(stt = 0; stt < 2; stt++)
	{
		BOOL bStt	= stt == iSTT;
		long nJ			= bStt ? 0 : pBri->m_nQtyJigan;
		if(pBri->m_nTypeRcBridgeBlock == 0) continue;

		// 보호블럭 라인을 구한다..
		CTwinVectorArray tvArrBoho;
		pBri->GetTvSungtoLine(tvArrBoho, bStt, TRUE, TRUE);

		long nSize	= tvArrBoho.GetSize();
		if(nSize == 0) 
		{
			// 좌측날개벽이 없어서 못 구할수도 있다. 
			// 우측날개벽쪽에서도 한번 구하자.
			pBri->GetTvSungtoLine(tvArrBoho, bStt, FALSE, TRUE);
			nSize = tvArrBoho.GetSize();
			if(nSize == 0)
			{
				return;
			}
		}

		// 보호블럭 총길이..
		CString sNote	= _T("");
		CString str		= _T("");
		double dLenBoho	= 0;
//		double dLen		= 0;
		long i = 0; for(i = 0; i < nSize; i++)
		{
			double dLen	= tvArrBoho.GetAt(i).GetLength();
			dLenBoho += dLen;

			if(i == 0)
			{
				sNote.Format("L = %.3f", toM(dLen));
			}
			else
			{
				str.Format(" + %.3f", toM(dLen));
				sNote += str;
			}
		}
		str.Format(" = %.3f", toM(dLenBoho));
		sNote += str;

		// 교대 전면
		CTwinVector tvJigan	= pBri->GetTvJijum(nJ, 0);
		CBMData *pBM			= new CBMData;
		pBM->m_nIdxBigClass		= BM_BIG_BOHOBLOCK;
		pBM->m_sBigClass		= GetStringBigClass(pBM->m_nIdxBigClass);
		pBM->m_nLevelTotal		= GetLevelTotal(pBM->m_nIdxBigClass);
		pBM->m_nIdxItemClass	= bStt ? BM_ITEM_BOHOBLOCK_SF : BM_ITEM_BOHOBLOCK_EF;
		pBM->m_sItemClass		= GetStringItemClass(pBM->m_nIdxItemClass);
		pBM->m_nTypeResult		= RESULT_AREA;
		pBM->m_sUnit			= "㎡";
		pBM->m_sNoteHead		= sNote;
		pBM->m_sForm.Format("%.3f x %.3f", toM(dLenBoho), toM(tvJigan.GetLength()));
		pBM->SetResult(toM(dLenBoho) * toM(tvJigan.GetLength()));
		pBM->m_bIsPrintByRefForm	= TRUE;
		pBM->m_sVCN[Result]		= "보호블럭";
		m_pArrBMBohoStd[stt].Add(pBM);
		
		// 측면 보호블럭 면적
		// 날개벽이 있는 개수만큼 각도가 증가됨.
		// 원뿔겉넓이 공식 : 사선*아래반지름*π
		if(nSize > 0)
		{
			long nCountWing		= 0;
			if(pBri->IsWingWall(bStt, TRUE)) nCountWing++;
			if(pBri->IsWingWall(bStt, FALSE)) nCountWing++;

			CDPoint xyOrg		= tvArrBoho.GetAt(0).m_v1;	// 원뿔의 중심축
			CDPoint xyDirOrg	= CDPoint(0, -1);
			CDPoint xyMatch		= CDPoint(0, 0);

// 			double dLenSlopeLower	= 0;	// 원뿔의 사선길이
// 			double dLenSlopeUpper	= 0;	// 원뿔 위쪽이 잘린형태일 경우 자린형태 위의 가상 원뿔의 사선길이.
// 			double dRLower			= 0;	// 원뿔의 반지름
// 			double dRUpper			= 0;	// 원뿔 위쪽이 잘린형태일 경우 잘린형태 위의 가상 원뿔의 반지름.
// 
			CTwinVector tv;
			long i = 0; for(i = 0; i < nSize; i++)
			{
				tv	= tvArrBoho.GetAt(i);

				GetXyMatchLineAndLine(xyOrg, xyDirOrg, tv.m_v2, tv.GetXyDir(TRUE), xyMatch);
				double dLenSlopeLower	= toM(tv.GetHorLength());
				double dRLower			= toM(fabs(tv.m_v2.x-xyOrg.x));
				double dLenSlopeUpper	= toM(~(xyMatch-tv.m_v1));
				double dRUpper			= toM(fabs(tv.m_v1.x-xyMatch.x));
				
				CBMData *pSide	= new CBMData(pBM);
				pSide->m_nIdxItemClass	= bStt ? BM_ITEM_BOHOBLOCK_SS : BM_ITEM_BOHOBLOCK_ES;
				pSide->m_sItemClass		= GetStringItemClass(pBM->m_nIdxItemClass);
				pSide->m_nTypeResult	= RESULT_AREA;
				pSide->m_sUnit			= "㎡";
				if(Compare(dRUpper, 0.0, "="))
				{
					pSide->m_sForm.Format("(%.3f x %.3f x π) x (%.3f x %d / 360)", dLenSlopeLower, dRLower, dAngleBoho, nCountWing);
					pSide->SetResult((dLenSlopeLower*dRLower*ConstPi) * (dAngleBoho*nCountWing/360));
				}
				else
				{
					pSide->m_sForm.Format("(((%.3f + %.3f) x %.3f x π) - (%.3f x %.3f x π)) x (%.3f x %d / 360)", dLenSlopeLower, dLenSlopeUpper, dRLower, dLenSlopeUpper, dRUpper, dAngleBoho, nCountWing);
					pSide->SetResult((((dLenSlopeLower+dLenSlopeUpper)*dRLower*ConstPi) - (dLenSlopeUpper*dRUpper*ConstPi)) * (dAngleBoho*nCountWing/360));
				}
				pSide->m_bIsPrintByRefForm	= TRUE;
				pSide->m_sVCN[Result]		= "보호블럭";
				m_pArrBMBohoStd[stt].Add(pSide);
			}
		}
	}
}

// 보호블럭 뒷채움 콘크리트
void COutBM::MakeBMConcBohoBackFill()
{
	CRcBridgeRebar *pBri	= GetRcBridgeRebar();
	if(!pBri) return;
	long nTypeBoho	= pBri->m_nTypeRcBridgeBlock;
	if(nTypeBoho == 0) return;
	
	long stt = 0; for(stt = 0; stt < 2; stt++)
	{
		// 수량 구함(보호블럭 넓이 * 콘크리트 두께)
		// 보호블럭 수량 수집
		double dBoho	= 0;
		long i = 0; for(i = 0; i < m_pArrBMBohoStd[stt].GetSize(); i++)
		{
			CBMData *pBM	= m_pArrBMBohoStd[stt].GetAt(i);
			if(pBM->m_sBigClass == "보호블럭")
				dBoho += pBM->GetResult();
		}

		if(dBoho>0)
		{
			CBMData *pBM				= new CBMData;
			pBM->m_nIdxBigClass			= BM_BIG_CONCRETE;
			pBM->m_sBigClass			= GetStringBigClass(pBM->m_nIdxBigClass);
			pBM->m_nLevelTotal			= GetLevelTotal(pBM->m_nIdxBigClass);
			pBM->m_nIdxMidClass			= BM_MID_CONCRETE_BOHO;
			pBM->m_sMidClass			= GetStringMidClass(pBM->m_nIdxMidClass);
			pBM->m_nIdxSmallClass		= BM_SMALL_CONCRETE_BOHO_BACKFILL;
			pBM->m_sSmallClass			= GetStringSmallClass(pBM->m_nIdxSmallClass);
			pBM->m_nTypeResult			= RESULT_VOLUMN;
			pBM->m_sUnit				= "㎥";
			pBM->m_sForm.Format("%.3f x %.3f", dBoho, 0.1);
			pBM->m_sFormExcel.Format("#보호블럭:%.3f# x 0.1", dBoho);
			pBM->SetResult(dBoho * 0.1);
			pBM->m_bIsPrintByRefForm	= TRUE;
			m_pArrBMBohoStd[stt].Add(pBM);
		}
	}
}

// 보호블럭 거푸집
void COutBM::MakeBMFormBoho()
{
	CRcBridgeRebar *pBri	= GetRcBridgeRebar();
	if(!pBri) return;
	
	long stt = 0; for(stt = 0; stt < 2; stt++)
	{
		BOOL bStt	= stt == iSTT;
		if(pBri->m_nTypeRcBridgeBlock == 0) continue;

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
		
		CTypedPtrArray <CObArray, CBMData*> *pArrBMStd	= &m_pArrBMBohoStd[stt];

		long nSizeBM	= pArrBMStd->GetSize();
		// 보호블럭 라인을 구한다..
		CTwinVectorArray tvArrBoho;
		pBri->GetTvSungtoLine(tvArrBoho, bStt, bLeft, TRUE);

		long nSize	= tvArrBoho.GetSize();
		if(nSize == 0) return;

		// 필요한 변수
		double dLenBoho	= 0;	// 보호블럭 총길이
		long i = 0; for(i = 0; i < nSize; i++)
			dLenBoho	+= toM(tvArrBoho.GetAt(i).GetLength());

		// 보호블럭 거푸집 (거친마감) 
		// 거친마감(버림콘크리트)은 원주에 대해서 거푸집 구하지 않는지 의문이 생김.. ???? 
		CBMData *pBM				= new CBMData;
		pBM->m_nIdxBigClass			= BM_BIG_FORM;
		pBM->m_sBigClass			= GetStringBigClass(pBM->m_nIdxBigClass);
		pBM->m_nLevelTotal			= GetLevelTotal(pBM->m_nIdxBigClass);
		pBM->m_nIdxMidClass			= BM_MID_FORM_6TIMES;
		pBM->m_sMidClass			= GetStringMidClass(pBM->m_nIdxMidClass);
		pBM->m_nIdxSmallClass		= BM_SMALL_FORM_VERTICAL;
		pBM->m_sSmallClass			= GetStringSmallClass(pBM->m_nIdxSmallClass);
		pBM->m_nTypeResult			= RESULT_AREA;
		pBM->m_sUnit				= "㎡";
		pBM->m_dV[SttMeter]		= 0;
		pBM->m_dV[EndMeter]		= 1;
		pBM->m_sForm.Format("%.3f x %.3f x 2EA", dLenBoho, 0.1);
		pBM->m_bIsPrintByRefForm	= TRUE;
		pBM->SetResult(dLenBoho * 0.1 * 2);
		pArrBMStd->Add(pBM);

		// 필요한 변수
		COutBMTogong *pOutBMTogong	= m_pStd->m_pArrOutBMTogong.GetAt(m_nBri);
		long nBLType			= pBri->m_nTypeRcBridgeBlock == 2 ? 0 : 1;
		long nGugeType			= Compare(pWing->m_dSlopeNoriNext, 1.8, "=") ? 1 : 0;
		double dAngleBoho		= pBri->m_dAngleRcBridgeBlock;
		double dMaterialForm	= m_pStd->m_dMaterialForm[nGugeType];	// 미터당 거푸집 재료량
		double dLenHorBoho		= pOutBMTogong->GetLengthBohoHor(bStt);
		long nCountWing			= 0;
		if(pBri->IsWingWall(bStt, TRUE)) nCountWing++;
		if(pBri->IsWingWall(bStt, FALSE)) nCountWing++;
		double dSideLength		= toM((2*ConstPi*dLenHorBoho)*(dAngleBoho*nCountWing/360));	// 보호블럭.. 원주
		
		// 기초 거푸집(보통마감) 변수
		double dMaxH			= m_pStd->m_dBLBaseMaxHeight[nBLType][nGugeType];
		double dMinH			= m_pStd->m_dBLBaseMinHeight[nBLType][nGugeType];
		double dMaxW			= m_pStd->m_dBLBaseMaxWidth[nBLType][nGugeType];
		double dMinW			= m_pStd->m_dBLBaseMinWidth[nBLType][nGugeType];
		dLenBoho				+= dSideLength;

		// 재료량 수량 + 양끝면 2개
		CBMData *pBM1 = new CBMData;
		pBM1->m_nIdxBigClass		= BM_BIG_FORM;
		pBM1->m_sBigClass			= GetStringBigClass(pBM1->m_nIdxBigClass);
		pBM1->m_nLevelTotal			= GetLevelTotal(pBM1->m_nIdxBigClass);
		pBM1->m_nIdxMidClass		= BM_MID_FORM_4TIMES;
		pBM1->m_sMidClass			= GetStringMidClass(pBM1->m_nIdxMidClass);
		pBM1->m_nIdxSmallClass		= BM_SMALL_FORM_VERTICAL;
		pBM1->m_sSmallClass			= GetStringSmallClass(pBM1->m_nIdxSmallClass);
		pBM1->m_nTypeResult			= RESULT_AREA;
		pBM1->m_dV[SttMeter]		= 0;
		pBM1->m_dV[EndMeter]		= 1;
		pBM1->m_sUnit				= "㎡";
		pBM1->m_sForm.Format("(%.3f x %.3f) + ((%.3f + %.3f) / 2 x %.3f + (%.3f x %.3f)) x 2EA", 
			dLenBoho, dMaterialForm, dMinW, dMaxW, (dMaxH-dMinH), dMaxW, dMinH);
		pBM1->m_bIsPrintByRefForm	= TRUE;
		pBM1->SetResult((dLenBoho * dMaterialForm) + ((dMinW + dMaxW) / 2 * (dMaxH-dMinH) + (dMaxW * dMinH)) * 2);
		pArrBMStd->Add(pBM1);

		RemakeByHeightBMList(pArrBMStd, 7000, 3000, nSizeBM-1, pArrBMStd->GetSize()-1);
	}	
}

// 날개벽 동바리
void COutBM::MakeBMPostWing()
{
	CRcBridgeRebar	*pBri	= GetRcBridgeRebar();
	if(!pBri) return;

	long nNum	= 1;
	long stt = 0; for(stt = 0; stt < 2; stt++)
	{
		BOOL bStt = stt == iSTT ? TRUE : FALSE;
		for(long left = 0; left < 2; left++)
		{
			BOOL bLeft = left == iLEFT ? TRUE : FALSE;
			if(!pBri->IsWingWall(bStt, bLeft)) continue;
			CWingWall *pWing	= pBri->GetWingWall(bStt, bLeft);
			if(!pWing) continue;

			CTypedPtrArray <CObArray, CBMData*> *pArrBMStd	= &m_pArrBMWingStd[stt][left];
			nNum	= GetTvAreaShapePostWing(&m_pArrAreaConc, bStt, bLeft, nNum);	// 브라켓 영역 구하기.

			long nSize	= m_pArrAreaConc.GetSize();
			long i = 0; for(i = 0; i < nSize; i++)
			{
				CTwinVectorAreaShape *pArea	= m_pArrAreaConc.GetAt(i);

				CBMData *pBM	= new CBMData;
				pBM->m_nNumbering			= pArea->m_nNum;
				pBM->m_nIdxBigClass			= BM_BIG_POST;
				pBM->m_sBigClass			= GetStringBigClass(pBM->m_nIdxBigClass);
				pBM->m_nLevelTotal			= GetLevelTotal(pBM->m_nIdxBigClass); 
				pBM->m_nIdxSmallClass		= BM_SMALL_POST_STEELPIPE;
				pBM->m_sSmallClass			= GetStringSmallClass(pBM->m_nIdxSmallClass);
				pBM->m_nTypeResult			= RESULT_VOLUMN;
				pBM->m_sUnit				= "㎥";
				pBM->m_nShape				= pArea->m_nTypeShape;
				pBM->m_dV[Width]			= Round(toM(pArea->m_dWidth),3);
				pBM->m_dV[WidthLower]		= Round(toM(pArea->m_dWidthLower),3);
				pBM->m_dV[WidthUpper]		= Round(toM(pArea->m_dWidthUpper),3);
				pBM->m_dV[WidthDiffLeft]	= Round(toM(pArea->m_dWidthDiffLeft),3);
				pBM->m_dV[WidthDiffRight]	= Round(toM(pArea->m_dWidthDiffRight),3);
				pBM->m_dV[Height]			= Round(toM(pArea->m_dHeight),3);
				pBM->m_dV[HeightLeft]		= Round(toM(pArea->m_dHeightLeft),3);
				pBM->m_dV[HeightRight]		= Round(toM(pArea->m_dHeightRight),3);
				pBM->m_dV[Length]			= Round(toM(pWing->m_dT2),3);
				pBM->CalculateResult();

				// 예외적인 출력양식
				pBM->m_nLevelMidClass		= 2;
				pArrBMStd->Add(pBM);
			}
		}
	}
}

long COutBM::GetTvAreaShapePostWing(CTypedPtrArray <CObArray, CTwinVectorAreaShape*> *pArrAreaConc, BOOL bStt, BOOL bLeft, long nBaseNum)
{
	AhTPADelete(pArrAreaConc,	(CTwinVectorAreaShape*)0);

	CRcBridgeRebar	*pBri	= GetRcBridgeRebar();
	if(!pBri) return nBaseNum;

	int se = bStt ? -1 : 1;
	int nLeft = bLeft ? -se : se;	// 시점일 경우 교량 진행 방향의 좌우와 날개벽의 좌우가 반대임.
	pBri->GetTvVertSection_Std(nLeft, TRUE, TRUE, FALSE, 2);
	
	long nNum	= nBaseNum;
	CTwinVectorArray tvArrWing;
	pBri->GetTvPostWing(tvArrWing, bStt, bLeft, pBri->m_nTypePostBaseLine);
	
	// 회전된 사다리꼴을 지원하므로 영역나누기 하지 않고 그냥 넣음
	if(tvArrWing.GetSize() > 2)
	{
		CTwinVectorAreaShape *pArea	= new CTwinVectorAreaShape; 
		pArea->m_tvArr	= tvArrWing;
		pArea->m_nNum	= nNum++;
		pArea->Calculate();
		pArrAreaConc->Add(pArea);
	}

	pBri->GetTvVertSection_Std(0, FALSE, FALSE, TRUE);

	return nNum;
}

void COutBM::MakeBMHoriConnectWing()
{
	// 동바리 수평 연결재 설치 높이
	// 최초 3.5미터에 설치하고, 이후 2미터 마다 설치한다.
	double dFirstHeight		= 3500;
	double dSecondHeight	= 2000;

	CTwinVector tvLeft, tvRight;
	CDPoint xyMatchLeft(0, 0), xyMatchRight(0, 0);
	BOOL bFirst	= TRUE;
	double dYBase	= 0;
	double dYCur	= 0;

	CRebarPlacing rb;
	CRcBridgeRebar	*pBri	= GetRcBridgeRebar(TRUE);
	if(!pBri) return;
	
	long nNum	= 1;
	long stt = 0; for(stt = 0; stt < 2; stt++)
	{
		BOOL bStt = stt == iSTT ? TRUE : FALSE;
		for(long left = 0; left < 2; left++)
		{
			BOOL bLeft	= left == iLEFT ? TRUE : FALSE;
			if(!pBri->IsWingWall(bStt, bLeft)) continue;
			CWingWall *pWing	= pBri->GetWingWall(bStt, bLeft);
			if(!pWing) continue;

			CTypedPtrArray <CObArray, CBMData*> *pArrBMStd	= &m_pArrBMWingStd[stt][left];
			nNum	= GetTvAreaShapePostWing(&m_pArrAreaConc, bStt, bLeft, nNum);	// 브라켓 영역 구하기.
			dYBase	= 0;
			dYCur	= 0;

			long nSize	= m_pArrAreaConc.GetSize();
			long i = 0; for(i = 0; i < nSize; i++)
			{
				CTwinVectorAreaShape *pArea	= m_pArrAreaConc.GetAt(i);
				tvLeft	= pArea->GetTvLeft();
				tvRight	= pArea->GetTvRight();

				dYBase	= i == 0 ? tvLeft.GetXyBottom().y : dYBase;
				dYCur	= i == 0 ? dYBase : dYCur;
				bFirst	= TRUE;

				while(1)
				{
					dYCur += bFirst ? dFirstHeight : dSecondHeight;
					bFirst	= FALSE;
					if(max(tvLeft.GetXyTop().y, tvRight.GetXyTop().y) <= dYCur) break;
					if(tvLeft.GetXyBottom().y >= dYCur) continue;

					// 도형의 좌우측 교차점 찾기
					CDPointArray xyArrMatch;
					rb.GetXyMatchTvArrAndLine(pArea->m_tvArr, CDPoint(0, dYCur), CDPoint(1, 0), xyArrMatch);
					if(xyArrMatch.GetSize() > 1)
					{
						xyArrMatch.Sort(TRUE);
						xyMatchLeft		= xyArrMatch.GetAt(0);
						xyMatchRight	= xyArrMatch.GetAt(1);
					}
					else
						continue;
					/*
					GetXyMatchLineAndLine(tvLeft.m_v1, tvLeft.GetXyDir(), CDPoint(0, dYCur), CDPoint(1, 0), xyMatchLeft);
					GetXyMatchLineAndLine(tvRight.m_v1, tvRight.GetXyDir(), CDPoint(0, dYCur), CDPoint(1, 0), xyMatchRight);
					*/

								
					CBMData *pBM	= new CBMData;
					pBM->m_nNumbering			= pArea->m_nNum;
					pBM->m_nIdxBigClass			= BM_BIG_POST;
					pBM->m_sBigClass			= GetStringBigClass(pBM->m_nIdxBigClass);
					pBM->m_nLevelTotal			= GetLevelTotal(pBM->m_nIdxBigClass); 
					pBM->m_nIdxSmallClass		= BM_SMALL_POST_HORI_CONNECT;
					pBM->m_sSmallClass			= GetStringSmallClass(pBM->m_nIdxSmallClass);
					pBM->m_sNoteHead.Format("수평연결재(h=%.1f초과시 높이 %.1fm마다 설치)", toM(dFirstHeight), toM(dSecondHeight));
					pBM->m_nTypeResult			= RESULT_AREA;
					pBM->m_sUnit				= "㎡";
					pBM->m_nShape				= SHAPE_TV_RECTANGLE;
					pBM->m_dV[WidthLower]		= Round(toM(fabs(xyMatchRight.x-xyMatchLeft.x)),3);
					pBM->m_dV[Length]			= Round(toM(pWing->m_dT2),3);
					pBM->m_dwSide				= SIDE_LOWER;
					pBM->CalculateResult();
					// 예외적인 출력양식
					pBM->m_nLevelMidClass		= 2;
					pArrBMStd->Add(pBM);
				}
			}
		}
	}
}
// 함수 삭제 DataStd로 이동
/*
// 수량을 구하기 위한 새로운 교량 생성
// BOOL bForTogong : 토공 수량용인 경우 경사로 구한다.(터파기등의 EL이 조정이 어렵기 때문임)
void COutBM::MakeRcBridgeForBM(BOOL bForTogong)
{
	if(m_nBri < 0 || m_nBri > m_pStd->m_pARcBridgeDataStd->GetBridgeSize()-1) return;
	CRcBridgeRebar *pBri	 = m_pStd->m_pARcBridgeDataStd->GetBridgeRebar(m_nBri);
	CLineInfo *pLineOrg	= pBri->GetLineBase();
	
	// 가각 이라면 무조건 슬래브는 자르지 않고 하나로 구함.
	if(pBri->m_bIsExp && pBri->m_nHeightForSlabBM == 0)
		pBri->m_nHeightForSlabBM	= 1;

	*m_pStd->m_pBri	= *pBri;
	m_pStd->m_pBri->m_pARcBridgeDataStd	= pBri->m_pARcBridgeDataStd;

	// 수평으로 그림. //////
	// Box형인 경우는 시종점 벽체길이를 평균으로 놓고 수평으로 만들어준다.
	m_pStd->m_pBri->m_nTypePlacingMainRebar_UpperSlab	= 0;
	if(m_pStd->m_pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP || m_pStd->m_pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP2 ||
		m_pStd->m_pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
		m_pStd->m_pBri->m_nTypeJongdanForRebar	= 1;
	else
		m_pStd->m_pBri->m_nTypeJongdanForRebar	= 0;

	// Box탕입일때는 토공수량도 
	if(bForTogong && pBri->IsBoxType() == FALSE)
		m_pStd->m_pBri->m_nTypeJongdanForRebar	= 1;	// 토공인 경우 무조건 경사로
	/////////////////////////



	// 벽체 기준 높이 옵션에 따라 평구배와 계획고 변경 /////////
	CLineInfo *pLine	= m_pStd->m_pBri->GetLineBase();
	//*pLine	= *pBri->GetLineBase();
	if(m_pStd->m_pBri->m_nTypeHeightWall == 0)	// 도로중심 기준 높이
	{
		// 그대로 사용하면 됨.
	}
	else if(m_pStd->m_pBri->m_nTypeHeightWall == 1 || m_pStd->m_pBri->m_nTypeHeightWall == 2 || m_pStd->m_pBri->m_nTypeHeightWall == 3)	// 1 : 평균높이 기준(최대+최소)/2, 2 : 최대 계획고, 3 : 지점별 평균 높이
	{
		double dSta		= pBri->GetStationByTarget(BRIDGE_OFF_STT_SLAB, 0, TRUE);
		CDPoint vAng	= pBri->GetAngleByTarget(BRIDGE_OFF_STT_SLAB, 0);
		double dELMax	= pBri->GetElMaxSlabUpper(dSta, vAng);
		double dELMin	= pBri->GetElMinSlabUpper(dSta, vAng);
		double dEL		= pLine->GetElevationPlanEl(dSta) - pBri->m_dTP;
		double dELUse	= m_pStd->m_pBri->m_nTypeHeightWall == 1 || m_pStd->m_pBri->m_nTypeHeightWall == 3 ? (dELMax+dELMin)/2 : dELMax;
		double dELDiff	= dEL - dELUse;

		long i = 0; for(i = 0; i < pLine->m_VIPsu; i++)
		{
			double dEL		= pLine->m_VIPData[i][1];
			pLine->m_VIPData[i][1]	= dEL - dELDiff;
		}

		// 모든 편구배를 0으로 맞춤
		// 좌
		for(i = 0; i < pLine->m_pSlopLeft.GetSize(); i++)
		{
			CSlop *pSlope	= pLine->m_pSlopLeft.GetAt(i);
			pSlope->m_dValuePyungubae	= 0;
		}

		// 우
		for(i = 0; i < pLine->m_pSlopRight.GetSize(); i++)
		{
			CSlop *pSlope	= pLine->m_pSlopRight.GetAt(i);
			pSlope->m_dValuePyungubae	= 0;
		}

		// 각 지점별 평균 높이인 경우
		// 시점의 평균 높이와 각 지점간의 평균 높이와의 차이를 기초하단 EL에 적용한다.
		if(m_pStd->m_pBri->m_nTypeHeightWall == 3)
		{
			long nCountJijum	= pBri->GetCountJijum();
			for(i = 1; i < nCountJijum; i++)
			{
				CFootingApp *pFooting	= pBri->GetFootingByJijumNumber(i);
				if(!pFooting) continue;
				if(pFooting->IsVirtualFooting()) continue;
				double dStaJijum	= i == nCountJijum-1 ? pBri->GetStationByTarget(BRIDGE_OFF_END_SLAB, 0, TRUE) : pBri->GetStationOnJijum(i, 0);
				CDPoint vAngJIjum	= i == nCountJijum-1 ? pBri->GetAngleByTarget(BRIDGE_OFF_END_SLAB,0) : pBri->GetAngleJijum(i);

				double dELMaxJijum	= pBri->GetElMaxSlabUpper(dStaJijum, vAngJIjum);
				double dELMinJijum	= pBri->GetElMinSlabUpper(dStaJijum, vAngJIjum);
				double dELJijum		= pLine->GetElevationPlanEl(dStaJijum) - pBri->m_dTP;
				double dELUseJijum	= (dELMaxJijum+dELMinJijum)/2;

				double dELDiff	= dELUseJijum - dELUse;
				
				// 종단을 level로 그리는 경우 종단경사에 의한 차이도 빼 줘야 한다.
				if(m_pStd->m_pBri->m_nTypeJongdanForRebar == 0)
				{
					double dELDiffByJong	= m_pStd->m_pBri->GetDiffELSttAndJijumForVertSection(i, FALSE);
					dELDiff -= dELDiffByJong;
				}
				////

				if(i == nCountJijum-1)
				{
					m_pStd->m_pBri->m_dELEB -= dELDiff;
				}
				else
				{
					CWallApp *pWall	= m_pStd->m_pBri->GetInWall(i-1);
					if(!pWall) continue;
					pWall->m_dELB -= dELDiff;
				}
				
			}
		}
	}

	if(m_pStd->m_pBri->IsBoxType())
	{
		double dStaStt		= pBri->GetStationByTarget(BRIDGE_OFF_STT_SLAB, 0, TRUE);
		double dStaEnd		= pBri->GetStationByTarget(BRIDGE_OFF_END_SLAB, 0, TRUE);
		double dElStt = pLine->GetElevationPlanEl(dStaStt);
		double dElEnd = pLine->GetElevationPlanEl(dStaEnd);

		double dDiffH = dElEnd - dElStt;
		// 하단에서 가장 낮은 EL을 고정시키고 벽체 평균 높이를 만들어주자.
		if(m_pStd->m_pBri->m_bSlopeLowerSlab)
		{
			// 시종점 벽체 높이는 같다. 하지만 EL이 시점 상단 기준일것이므로
			// 저점 기준으로 차이만금 내려주자.
			// 시점이 저점이라면 그냥 둬도 된다..
			if(dDiffH < 0)
			{
				long ix = 0; for(ix = 0; ix < pLine->m_VIPsu; ix++)
				{
					double dEL		= pLine->m_VIPData[ix][1];
					pLine->m_VIPData[ix][1]	= dEL + dDiffH;
				}
				m_pStd->m_pBri->m_dELSB += dDiffH;
			}
		}
		else
		{
			// 시점 하단 EL은 그대로 두고 상단을 dDiffH/2 만큼 올리거나 내려 주기 위해서 선형을 이동시켜 주자는...
			long ix = 0; for(ix = 0; ix < pLine->m_VIPsu; ix++)
			{
				double dEL		= pLine->m_VIPData[ix][1];
				pLine->m_VIPData[ix][1]	= dEL + dDiffH/2.;
			}

			m_pStd->m_pBri->m_bSlopeLowerSlab	= TRUE;
		}

		m_pStd->m_pBri->SyncBridge();
	}

	m_pStd->m_pBri->m_bSettingVertSection	= FALSE;
	m_pStd->m_pBri->InitSaveCrossSection();
	m_pStd->m_pBri->InitSaveDistRigidEndSlab();
	m_pStd->m_pBri->InitSaveDistRigidEndWall();
	m_pStd->m_pBri->InitSaveOrgInWall();
	m_pStd->m_pBri->InitSaveXyMatchLineAndSlabUpper();
	m_pStd->m_pBri->InitSaveXySlabUpperByLenFrSttStation();
	m_pStd->m_pBri->SyncBridge(0, TRUE);
	m_pStd->m_pBri->SettingVertSection();
}*/

long COutBM::GetFormByPos(long nPos)
{
	CRcBridgeRebar *pBri	= GetRcBridgeRebar();
	if(!pBri) return -1;
	CARcBridgeDataStd *pStd	= pBri->m_pARcBridgeDataStd;
	if(!pStd) return -1;

	long nFormType		= -1;
	long nIdxMidClass	= -1;

	// 거푸집 타입 설정
	if(nPos == FORM_POS_NORMAL)			nFormType	= pBri->m_nFormType_ConcNormal;					// 일반콘크리트
	if(nPos == FORM_POS_FOOTING)		nFormType	= pBri->m_nFormType_Footing;					// 기초
	if(nPos == FORM_POS_WALL_FRONT)		nFormType	= pBri->m_nFormType_WallFront;					// 벽체전면
	if(nPos == FORM_POS_WALL_BACK)		nFormType	= pBri->m_nFormType_WallBack;					// 벽체배면
	if(nPos == FORM_POS_WINGSL_FRONT)	nFormType	= pBri->m_nFormType_WingFront[iSTT][iLEFT];		// 시점좌측날개벽 전면
	if(nPos == FORM_POS_WINGSL_BACK)	nFormType	= pBri->m_nFormType_WingBack[iSTT][iLEFT];		// 시점좌측날개벽 배면
	if(nPos == FORM_POS_WINGSR_FRONT)	nFormType	= pBri->m_nFormType_WingFront[iSTT][iRIGHT];	// 시점우측날개벽 전면
	if(nPos == FORM_POS_WINGSR_BACK)	nFormType	= pBri->m_nFormType_WingBack[iSTT][iRIGHT];		// 시점우측날개벽 배면
	if(nPos == FORM_POS_WINGEL_FRONT)	nFormType	= pBri->m_nFormType_WingFront[iEND][iLEFT];		// 종점좌측날개벽 전면
	if(nPos == FORM_POS_WINGEL_BACK)	nFormType	= pBri->m_nFormType_WingBack[iEND][iLEFT];		// 종점좌측날개벽 배면
	if(nPos == FORM_POS_WINGER_FRONT)	nFormType	= pBri->m_nFormType_WingFront[iEND][iRIGHT];	// 종점우측날개벽 전면
	if(nPos == FORM_POS_WINGER_BACK)	nFormType	= pBri->m_nFormType_WingBack[iEND][iRIGHT];		// 종점우측날개벽 배면
	if(nPos == FORM_POS_APS)			nFormType	= pBri->m_nFormType_Aps;						// 접속슬래브
	if(nPos == FORM_POS_MASS)			nFormType	= pBri->m_nFormType_MassConc;					// mass
	if(nPos == FORM_POS_LEAN)			nFormType	= pBri->m_nFormType_LeanConc;					// lean

	
	// 타입에 따라서 실제 수량에서 사용하는 nIdxMidClass를 설정
	if(pStd->m_nTypeBMApply == TYPE_BM_NORMAL)
	{
		/*
			CString CARcBridgeDataStd::m_sBM_FormTypeNormal[MAX_BM_FORM_TYPE_NORMAL] = 
			{			
				"합판6회",
				"합판4회",
				"합판3회",
				"무늬거푸집",
				"강재거푸집",
				"원형거푸집",
			};
		*/
		switch(nFormType)
		{
			case 0: nIdxMidClass	= BM_MID_FORM_6TIMES; break;	// 합판6회
			case 1: nIdxMidClass	= BM_MID_FORM_4TIMES; break;	// 합판4회
			case 2: nIdxMidClass	= BM_MID_FORM_3TIMES; break;	// 합판3회
			case 3: nIdxMidClass	= BM_MID_FORM_PATTERN; break;	// 무늬거푸집
			case 4: nIdxMidClass	= BM_MID_FORM_GANGJE; break;	// 강재거푸집
			case 5: nIdxMidClass	= BM_MID_FORM_CIRCLE; break;	// 원평거푸집
			case 6: nIdxMidClass	= BM_MID_FORM_EURO; break;		// 유로폼
			default: break;
		}
	}
	else
	{
		/*
		CString CARcBridgeDataStd::m_sBM_FormType[MAX_BM_FORM_TYPE] = 
			{			
				"거친마감",
				"보통마감",
				"매끈한마감",
				"설명된 특징있는 마감 (무늬거푸집)",
				"설명된 특징있는 마감 (강재거푸집)",
				"설명된 특징있는 마감 (코팅거푸집)",
				"설명된 특징있는 마감 (DECK PLATE)",
				"설명된 특징있는 마감 (1회 사용)"
			};
		*/
		switch(nFormType)
		{
			case 0: nIdxMidClass	= BM_MID_FORM_6TIMES; break;	// 거친마감
			case 1: nIdxMidClass	= BM_MID_FORM_4TIMES; break;	// 보통마감
			case 2: nIdxMidClass	= BM_MID_FORM_3TIMES; break;	// 매끈한마감
			case 3: nIdxMidClass	= BM_MID_FORM_PATTERN_S; break;	// 설명된 특징있는 마감 (무늬거푸집)
			case 4: nIdxMidClass	= BM_MID_FORM_GANGJE_S; break;	// 설명된 특징있는 마감 (강재거푸집)
			case 5: nIdxMidClass	= BM_MID_FORM_COTTING_S; break;	// 설명된 특징있는 마감 (코팅거푸집)
			case 6: nIdxMidClass	= BM_MID_FORM_DECK_S; break;	// 설명된 특징있는 마감 (DECK PLATE)
			case 7: nIdxMidClass	= BM_MID_FORM_ONE_S; break;		// 설명된 특징있는 마감 (1회 사용)
			default: break;
		}
	}

	return nIdxMidClass;
}

void COutBM::MakeBMRebarTable()
{
	CRcBridgeRebar *pBri	= GetRcBridgeRebar(TRUE);
	if(!pBri) return;


	CTypedPtrArray <CObArray, CRebar*> pArrRebar;
	CTypedPtrArray <CObArray, CRebar*> pArrRebarAll;

	CBasicConcInfo *pBasic = m_pStd->m_pARcBridgeDataStd->m_pBasicConcInfo;
	long nTypeUnit = m_pStd->m_pARcBridgeDataStd->m_nTypeUnit;
	CString sSheetName = _T(""), sTitleName = _T("");

	// 교량전체 철근 수량 집계표
	m_rebarTableAll.InitRebar();
	m_rebarTableAll.SetBasicConcInfo(pBasic);
	m_rebarTableAll.SetTypeUnit(nTypeUnit);
	m_rebarTableAll.SetTypeRoundRebarLength(pBasic->m_nRoundRebarLength);


	// 본체
	m_rebarTableBridgeSum.InitRebar();
	m_rebarTableBridgeSum.SetBasicConcInfo(pBasic);
	m_rebarTableBridgeSum.SetTypeUnit(nTypeUnit);
	m_rebarTableBridgeSum.SetTypeRoundRebarLength(pBasic->m_nRoundRebarLength);


	AhTPADelete(&pArrRebar,(CReBar*)0);
	pBri->MakeRebarList(&pArrRebar, 0, TRUE, TRUE, 0);		// 본체
	long i = 0;
	for(i=0; i<=pBri->m_nQtyJigan; i++)
	{
		pBri->MakeRebarList(&pArrRebar, 1, TRUE, TRUE, i);	// 중간지점(복잡)
		pBri->MakeRebarList(&pArrRebar, 6, TRUE, TRUE, i);	// 보기둥이 아닌(거더철근)		
	}
	pBri->MakeRebarList(&pArrRebar, 2, TRUE, TRUE, i);		// 기초

	// 원래 본체에 교명주 철근이 포함되어 있었으나 옵션으로 제거할 수 있다.
	// 2009.03.01
	if(!m_pStd->m_bSeparateExcludeRebarMonument)
		pBri->MakeRebarList(&pArrRebar, 4, TRUE, TRUE, i);		// 교명주

	// copy해둔다. 
	for(HGINT32 rx = 0; rx < pArrRebar.GetSize(); rx++)
	{
		CRebar *pRB = pArrRebar.GetAt(rx);
		if(pRB == NULL)
			continue;

		CRebar *pRBNew = new CRebar(pRB);
		pArrRebarAll.Add(pRBNew);
	}

	m_rebarTableBridge.InitRebar();
	m_rebarTableBridge.SetBasicConcInfo(pBasic);
	m_rebarTableBridge.SetTypeUnit(nTypeUnit);
	m_rebarTableBridge.SetTypeRoundRebarLength(pBasic->m_nRoundRebarLength);
	m_rebarTableBridge.AddRebar(&pArrRebar, hggettext("본체"), _T(""));
	m_rebarTableBridge.FinishAddRebar();

	if(m_pStd->m_bRefCellBMSum)	sSheetName.Format(hggettext("%s 본체 철근수량 집계표"), pBri->m_strBridgeName);
	else											sSheetName = _T("");
	m_rebarTableBridgeSum.AddRebar(&pArrRebar, hggettext("본체"), sSheetName);
	
	
	//보기둥 및 거더 철근 분리 집계
	for (long nJ =0; nJ < pBri->GetCountJijum(); nJ++)
	{
		AhTPADelete(&pArrRebar,(CReBar*)0);
		pBri->MakeRebarList(&pArrRebar, 5, TRUE, TRUE, nJ);		// 중간지점(복잡)
		pBri->MakeRebarList(&pArrRebarAll, 5, TRUE, TRUE, nJ);	
		pBri->MakeRebarList(&pArrRebar, 7, TRUE, TRUE, nJ);		// 보기둥인(거더철근)
		pBri->MakeRebarList(&pArrRebarAll, 7, TRUE, TRUE, nJ);	

		sTitleName.Format(_T("보기둥%d"), nJ);
		m_rebarTableBridgeSum.AddRebar(&pArrRebar, sTitleName, sSheetName);
	}

	m_rebarTableBridgeSum.FinishAddRebar();
	m_rebarTableAll.AddRebar(&pArrRebarAll, hggettext("본체"), sSheetName);


	// 방호벽
	AhTPADelete(&pArrRebarAll, (CReBar*)0);
	m_rebarTableGuardWallSum.InitRebar();
	m_rebarTableGuardWallSum.SetBasicConcInfo(pBasic);
	m_rebarTableGuardWallSum.SetTypeUnit(nTypeUnit);
	m_rebarTableGuardWallSum.SetTypeRoundRebarLength(pBasic->m_nRoundRebarLength);

	for(i=0; i<pBri->GetQtyHDan(); i++)
	{
		CGuardWallRC* pGW = pBri->GetGuardWallByHDan(i);
		if(!pGW) continue;

		BOOL bRoadGuard = pGW->IsTypeHDanRoadTotalGuardWall();
		BOOL bNonRebar = pGW->IsTypeHDanNonRebarConcrete();

		if(pGW->IsTypeHDanRoadTotalGuardWall() && !pGW->IsTypeHDanNonRebarConcrete())	
		{
			AhTPADelete(&pArrRebar,(CReBar*)0);
			pBri->MakeRebarList(&pArrRebar, 8, TRUE, TRUE, i);		// 방호벽
			pBri->MakeRebarList(&pArrRebarAll, 8, TRUE, TRUE, i);		// 방호벽

			m_rebarTableGuardWall[i].InitRebar();
			m_rebarTableGuardWall[i].SetBasicConcInfo(pBasic);
			m_rebarTableGuardWall[i].SetTypeRoundRebarLength(pBasic->m_nRoundRebarLength);
			m_rebarTableGuardWall[i].SetTypeUnit(nTypeUnit);
			sTitleName.Format(hggettext("방호벽%d"), i + 1);
			m_rebarTableGuardWall[i].AddRebar(&pArrRebar, sTitleName, _T(""));
			m_rebarTableGuardWall[i].FinishAddRebar();

			sSheetName.Format(hggettext("%s 방호벽%d 철근수량 집계표"), pBri->m_strBridgeName, i + 1);
			m_rebarTableGuardWallSum.AddRebar(&pArrRebar, sTitleName, sSheetName);
		}
	}

	if(m_pStd->m_bSeparateExcludeRebarMonument)
	{
		AhTPADelete(&pArrRebar,(CReBar*)0);
		pBri->MakeRebarList(&pArrRebar, 4, TRUE, TRUE, 0);		// 교명주
		pBri->MakeRebarList(&pArrRebarAll, 4, TRUE, TRUE, 0);		// 교명주

		m_rebarTableGuardWall[i].InitRebar();
		m_rebarTableGuardWall[i].SetBasicConcInfo(pBasic);
		m_rebarTableGuardWall[i].SetTypeRoundRebarLength(pBasic->m_nRoundRebarLength);
		m_rebarTableGuardWall[i].SetTypeUnit(nTypeUnit);
		m_rebarTableGuardWall[i].AddRebar(&pArrRebar, hggettext("교명주"), _T(""));
		m_rebarTableGuardWall[i].FinishAddRebar();

		sSheetName.Format(hggettext("%s 교명주 철근수량 집계표"), pBri->m_strBridgeName);
		m_rebarTableGuardWallSum.AddRebar(&pArrRebar, hggettext("교명주"), sSheetName);
	}

	m_rebarTableGuardWallSum.FinishAddRebar();

	if(m_pStd->m_bRefCellBMSum)	sSheetName.Format(hggettext("%s 방호벽 철근수량 집계표"), pBri->m_strBridgeName);
	else											sSheetName = _T("");
	m_rebarTableAll.AddRebar(&pArrRebarAll, hggettext("방호벽"), sSheetName);

	// 날개벽
	long stt = 0, left = 0, cns = 0;
	HGBOOL bStt = TRUE, bLeft = TRUE;
	CString str = _T(""), sLeft = _T(""), sStt = _T("");
	
	AhTPADelete(&pArrRebarAll, (CReBar*)0);

	m_rebarTableWingSum.InitRebar();
	m_rebarTableWingSum.SetBasicConcInfo(pBasic);
	m_rebarTableWingSum.SetTypeRoundRebarLength(pBasic->m_nRoundRebarLength);
	m_rebarTableWingSum.SetTypeUnit(nTypeUnit);
	for(stt = 0; stt < 2; stt++)
	{
		bStt	= stt == iSTT;
		sStt	= bStt ? "시점" : "종점";
		for(left = 0; left < 2; left++)
		{
			bLeft	= left == iLEFT;
			sLeft	= bLeft ? "좌측" : "우측";

			AhTPADelete(&pArrRebar,(CReBar*)0);
			if(!pBri->IsWingWall(bStt, bLeft)) continue;

			pBri->MakeRebarList(&pArrRebar, 3, bStt, bLeft, 0);
			pBri->MakeRebarList(&pArrRebarAll, 3, bStt, bLeft, 0);

			m_rebarTableWing[stt][left].InitRebar();
			m_rebarTableWing[stt][left].SetBasicConcInfo(pBasic);
			m_rebarTableWing[stt][left].SetTypeRoundRebarLength(pBasic->m_nRoundRebarLength);
			m_rebarTableWing[stt][left].SetTypeUnit(nTypeUnit);
			sTitleName.Format(_T("날개벽(%s%s)"), sStt, sLeft);
			m_rebarTableWing[stt][left].AddRebar(&pArrRebar, sTitleName, _T(""));
			m_rebarTableWing[stt][left].FinishAddRebar();

			sSheetName.Format(_T("%s %s 철근수량 집계표"), pBri->m_strBridgeName, sTitleName);
			m_rebarTableWingSum.AddRebar(&pArrRebar, sTitleName, sSheetName);
		}
	}

	m_rebarTableWingSum.FinishAddRebar();

	if(m_pStd->m_bRefCellBMSum)	sSheetName.Format(_T("%s 날개벽 철근수량 집계표"), pBri->m_strBridgeName);
	else											sSheetName = _T("");
	m_rebarTableAll.AddRebar(&pArrRebarAll, hggettext("날개벽"), sSheetName);
	
	

	// 접속슬래브
	AhTPADelete(&pArrRebarAll, (CReBar*)0);
	m_rebarTableApsSum.InitRebar();
	m_rebarTableApsSum.SetBasicConcInfo(pBasic);
	m_rebarTableApsSum.SetTypeRoundRebarLength(pBasic->m_nRoundRebarLength);
	m_rebarTableApsSum.SetTypeUnit(nTypeUnit);

	for(stt = 0; stt < 2; stt++)
	{
		bStt	= stt == iSTT;
		sStt	= bStt ? "시점" : "종점";
		for(left = 0; left < 2; left++)
		{
			bLeft	= left == iLEFT;
			sLeft	= bLeft ? "좌측" : "우측";

			CApsApp *pAps	= bStt ? &pBri->m_apsStt[left] : &pBri->m_apsEnd[left];
			if(!pAps->m_bIs) continue;

			AhTPADelete(&pArrRebar, (CReBar*)0);
			for(HGINT32 rx = 0; rx < pAps->m_pArrRebar.GetSize(); rx++)
			{
				CRebar *pRB = pAps->m_pArrRebar.GetAt(rx);
				if(pRB == NULL || pRB->IsTrue() == FALSE)
					continue;

				CRebar *pRBNew = new CRebar(pRB);
				pArrRebar.Add(pRBNew);

				pRBNew = new CRebar(pRB);
				pArrRebarAll.Add(pRBNew);
			}

			m_rebarTableAps[stt][left].InitRebar();
			m_rebarTableAps[stt][left].SetBasicConcInfo(pBasic);
			m_rebarTableAps[stt][left].SetTypeRoundRebarLength(pBasic->m_nRoundRebarLength);
			m_rebarTableAps[stt][left].SetTypeUnit(nTypeUnit);
			sTitleName.Format(_T("접속슬래브(%s%s)"), sStt, sLeft);
			m_rebarTableAps[stt][left].AddRebar(&pArrRebar, sTitleName, _T(""));
			m_rebarTableAps[stt][left].FinishAddRebar();

			sSheetName.Format(hggettext("%s %s 철근수량 집계표 "), pBri->m_strBridgeName, sTitleName);
			m_rebarTableApsSum.AddRebar(&pArrRebar, sTitleName, sSheetName);
		}
	}
	m_rebarTableApsSum.FinishAddRebar();

	if(m_pStd->m_bRefCellBMSum)	sSheetName.Format(hggettext("%s 접속슬래브 철근수량 집계표"), pBri->m_strBridgeName);
	else											sSheetName = _T("");
	m_rebarTableAll.AddRebar(&pArrRebarAll, hggettext("접속슬래브"), sSheetName);

	// 완충슬래브
	AhTPADelete(&pArrRebarAll, (CReBar*)0);
	m_rebarTableCnsSum.InitRebar();
	m_rebarTableCnsSum.SetBasicConcInfo(pBasic);
	m_rebarTableCnsSum.SetTypeRoundRebarLength(pBasic->m_nRoundRebarLength);
	m_rebarTableCnsSum.SetTypeUnit(nTypeUnit);
	for(stt = 0; stt < 2; stt++)
	{
		bStt	= stt == iSTT;
		sStt	= bStt ? "시점" : "종점";
		for(left = 0; left < 2; left++)
		{
			bLeft	= left == iLEFT;
			sLeft	= bLeft ? "좌측" : "우측";

			for(cns = 0; cns < 2; cns++)
			{
				CApsApp *pCns	= bStt ? &pBri->m_cnsStt[left][cns] : &pBri->m_cnsEnd[left][cns];
				if(!pCns->m_bIs) continue;

				AhTPADelete(&pArrRebar, (CReBar*)0);
				for(HGINT32 rx = 0; rx < pCns->m_pArrRebar.GetSize(); rx++)
				{
					CRebar *pRB = pCns->m_pArrRebar.GetAt(rx);
					if(pRB == NULL || pRB->IsTrue() == FALSE)
						continue;

					CRebar *pRBNew = new CRebar(pRB);
					pArrRebar.Add(pRBNew);

					pRBNew = new CRebar(pRB);
					pArrRebarAll.Add(pRBNew);
				}

				m_rebarTableCns[stt][left][cns].InitRebar();
				m_rebarTableCns[stt][left][cns].SetBasicConcInfo(pBasic);
				m_rebarTableCns[stt][left][cns].SetTypeRoundRebarLength(pBasic->m_nRoundRebarLength);
				m_rebarTableCns[stt][left][cns].SetTypeUnit(nTypeUnit);
				sTitleName.Format(_T("완충슬래브(%s%s%d)"), sStt, sLeft, cns+1);
				m_rebarTableCns[stt][left][cns].AddRebar(&pArrRebar, sTitleName, _T(""));
				m_rebarTableCns[stt][left][cns].FinishAddRebar();

				sSheetName.Format(hggettext("%s %s 철근수량 집계표"), pBri->m_strBridgeName, sTitleName);
				m_rebarTableCnsSum.AddRebar(&pArrRebar, sTitleName, sSheetName);
			}
		}
	}
	m_rebarTableCnsSum.FinishAddRebar();

	if(m_pStd->m_bRefCellBMSum)	sSheetName.Format(hggettext("%s 완충슬래브 철근수량 집계표"), pBri->m_strBridgeName);
	else											sSheetName = _T("");
	m_rebarTableAll.AddRebar(&pArrRebarAll, hggettext("완충슬래브"), sSheetName);

	//현장타설 말뚝 //추가
	AhTPADelete(&pArrRebarAll, (CReBar*)0);
	m_rebarTableHuyntaSum.InitRebar();
	m_rebarTableHuyntaSum.SetBasicConcInfo(pBasic);
	m_rebarTableHuyntaSum.SetTypeUnit(nTypeUnit);

	for (long ix =0; ix < pBri->GetCountFooting(); ix++)
	{
		CFootingData *pFooting	= pBri->GetFooting(ix);
		if(pFooting == NULL) continue;

		if(pFooting->m_exFooting.m_nType == EXFOOTING_TYPE_PILE)
		{
			AhTPADelete(&pArrRebar,(CReBar*)0);
			pBri->MakeRebarList(&pArrRebar, 10, TRUE, TRUE, ix); //현장타설말뚝
			pBri->MakeRebarList(&pArrRebarAll, 10, TRUE, TRUE, ix); //현장타설말뚝

			m_rebarTableHuynta[ix].InitRebar();
			m_rebarTableHuynta[ix].SetBasicConcInfo(pBasic);
			m_rebarTableHuynta[ix].SetTypeRoundRebarLength(pBasic->m_nRoundRebarLength);
			m_rebarTableHuynta[ix].SetTypeUnit(nTypeUnit);
			//
			if(ix ==0 || ix == pBri->GetCountFooting()-1)
			{
				sTitleName = ix == 0 ? _T("시점") : _T("종점");
			}
			else
			{
				sTitleName.Format(hggettext("중간지점부%d"), ix);
			}
			
			//
			m_rebarTableHuynta[ix].AddRebar(&pArrRebar, sTitleName, _T(""));
			m_rebarTableHuynta[ix].FinishAddRebar();

			sSheetName.Format(hggettext("%s 현장타설말뚝%d 철근수량 집계표"), pBri->m_strBridgeName, ix + 1);
			m_rebarTableHuyntaSum.AddRebar(&pArrRebar, sTitleName, sSheetName);
		}
	}
	m_rebarTableHuyntaSum.FinishAddRebar();

	if(m_pStd->m_bRefCellBMSum)	sSheetName.Format(hggettext("%s 현장타설말뚝 철근수량 집계표"), pBri->m_strBridgeName);
	else											sSheetName = _T("");
	m_rebarTableAll.AddRebar(&pArrRebarAll, hggettext("현장타설말뚝"), sSheetName);
	m_rebarTableAll.FinishAddRebar();


	AhTPADelete(&pArrRebar, (CReBar*)0);
	AhTPADelete(&pArrRebarAll, (CReBar*)0);
}

// 철근 집계
void COutBM::MakeBMRebar()
{
	CRcBridgeRebar *pBri	= GetRcBridgeRebar(TRUE);
	if(!pBri) return;

	long stt(0), left(0), i(0), cns(0);
	long nQtyHDan = pBri->GetQtyHDan();
	BOOL bLeft(TRUE), bStt(TRUE);
	CString sLeft(""), sStt(""), str("");

	CTypedPtrArray <CObArray, CRebar*> pArrRebar;

	// 본체
	AhTPADelete(&pArrRebar,(CReBar*)0);
	pBri->MakeRebarList(&pArrRebar, 0, TRUE, TRUE, 0);		// 본체
	for(i=0; i<=pBri->m_nQtyJigan; i++)
	{
		pBri->MakeRebarList(&pArrRebar, 1, TRUE, TRUE, i);	// 중간지점(복잡)
		pBri->MakeRebarList(&pArrRebar, 5, TRUE, TRUE, i);	// 중간지점(복잡)
		pBri->MakeRebarList(&pArrRebar, 6, TRUE, TRUE, i);	// 보기둥이 아닌(거더철근)
		pBri->MakeRebarList(&pArrRebar, 7, TRUE, TRUE, i);	// 보기둥인(거더철근)
	}
	pBri->MakeRebarList(&pArrRebar, 2, TRUE, TRUE, i);		// 기초

	// 원래 본체에 교명주 철근이 포함되어 있었으나 옵션으로 제거할 수 있다.
	// 2009.03.01
	if(!m_pStd->m_bSeparateExcludeRebarMonument)
		pBri->MakeRebarList(&pArrRebar, 4, TRUE, TRUE, i);		// 교명주

	SetRebarBMData(&m_pArrRebarBMBridgeStd[0], &m_pArrRebarBMBridgeStd[1], &pArrRebar, "본체");
	
	// 방호벽
	for(i=0; i<nQtyHDan; i++)
	{
		CGuardWallRC* pGW = pBri->GetGuardWallByHDan(i);
		if(!pGW) continue;
		if(pGW->IsTypeHDanRoadTotalGuardWall() && !pGW->IsTypeHDanNonRebarConcrete())	
		{
			AhTPADelete(&pArrRebar,(CReBar*)0);
			pBri->MakeRebarList(&pArrRebar, 8, TRUE, TRUE, i);		// 방호벽
			SetRebarBMData(&m_pArrRebarBMGuardWallStd[i][0], &m_pArrRebarBMGuardWallStd[i][1], &pArrRebar, "방호벽");
		}
	}

	// 교명주
	if(m_pStd->m_bSeparateExcludeRebarMonument)
	{
		AhTPADelete(&pArrRebar,(CReBar*)0);
		pBri->MakeRebarList(&pArrRebar, 4, TRUE, TRUE, 0);		// 교명주
		SetRebarBMData(&m_pArrRebarBMMonumentStd[0], &m_pArrRebarBMMonumentStd[1], &pArrRebar, "교명주");
	}

	// 날개벽
	for(stt = 0; stt < 2; stt++)
	{
		bStt	= stt == iSTT;
		sStt	= bStt ? "시점" : "종점";
		for(left = 0; left < 2; left++)
		{
			bLeft	= left == iLEFT;
			sLeft	= bLeft ? "좌측" : "우측";
			str.Format("날개벽(%s%s)", sStt, sLeft);

			AhTPADelete(&pArrRebar,(CReBar*)0);
			if(!pBri->IsWingWall(bStt, bLeft)) continue;

			pBri->MakeRebarList(&pArrRebar, 3, bStt, bLeft, 0);

			SetRebarBMData(&m_pArrRebarBMWingStd[stt][left][0], &m_pArrRebarBMWingStd[stt][left][1], &pArrRebar, str);
		}
	}

	// 접속슬래브
	for(stt = 0; stt < 2; stt++)
	{
		bStt	= stt == iSTT;
		sStt	= bStt ? "시점" : "종점";
		for(left = 0; left < 2; left++)
		{
			bLeft	= left == iLEFT;
			sLeft	= bLeft ? "좌측" : "우측";
			str.Format("접속슬래브(%s%s)", sStt, sLeft);

			CApsApp *pAps	= bStt ? &pBri->m_apsStt[left] : &pBri->m_apsEnd[left];
			if(!pAps->m_bIs) continue;

			SetRebarBMData(&m_pArrRebarBMApsStd[stt][left][0], &m_pArrRebarBMApsStd[stt][left][1], &pAps->m_pArrRebar, str);
		}
	}

	// 완충슬래브
	for(stt = 0; stt < 2; stt++)
	{
		bStt	= stt == iSTT;
		sStt	= bStt ? "시점" : "종점";
		for(left = 0; left < 2; left++)
		{
			bLeft	= left == iLEFT;
			sLeft	= bLeft ? "좌측" : "우측";
			
			for(cns = 0; cns < 2; cns++)
			{
				str.Format("완충슬래브(%s%s%d)", sStt, sLeft, cns+1);
				CApsApp *pCns	= bStt ? &pBri->m_cnsStt[left][cns] : &pBri->m_cnsEnd[left][cns];
				if(!pCns->m_bIs) continue;

				SetRebarBMData(&m_pArrRebarBMCnsStd[stt][left][cns][0], &m_pArrRebarBMCnsStd[stt][left][cns][1], &pCns->m_pArrRebar, str);
			}
		}
	}

	//현장타설 말뚝
	for (long ix =0; ix < pBri->GetCountFooting(); ix++)
	{
		CFootingApp *pFooting = pBri->GetFooting(ix);
		if(pFooting == NULL) continue;

		if(pFooting->m_exFooting.m_nType == EXFOOTING_TYPE_PILE)
		{
			str.Format("현장타설말뚝(%d)", ix+1);
			
			AhTPADelete(&pArrRebar,(CReBar*)0);
			if(!pBri->IsWingWall(bStt, bLeft)) continue;

			pBri->MakeRebarList(&pArrRebar, 10, TRUE, TRUE, ix);

			SetRebarBMData(&m_pArrRebarBMHyuntaStd[ix][0], &m_pArrRebarBMHyuntaStd[ix][1], &pArrRebar, str);
		}
	}

	AhTPADelete(&pArrRebar, (CReBar*)0);
}

void COutBM::SetRebarBMData(CTypedPtrArray <CObArray, CRebarBMData*> *pArrRebarBMDataD, CTypedPtrArray <CObArray, CRebarBMData*> *pArrRebarBMDataH, CTypedPtrArray <CObArray, CRebar*> *pArrRebar, CString sNameBigClass)
{
	if(!pArrRebar || !pArrRebarBMDataD || !pArrRebarBMDataH) return;
	CRcBridgeRebar *pBri	= GetRcBridgeRebar(TRUE);
	if(!pBri) return;
	CARcBridgeDataStd *pStd	= pBri->m_pARcBridgeDataStd;
	if(!pStd) return;

	CRebar *pRB	= NULL;
	long nSize	= pArrRebar->GetSize();
	
	long i=0; for(i=0; i< nSize; i++)
	{
		pRB = (CRebar*)pArrRebar->GetAt(i);
		if(!pRB->IsTrue()) continue;

		double dUWeight = pStd->GetValueBasic((long)pRB->m_dDia, BASIC_WEIGHT_UNIT);

		CRebarBMData *pRebarBM		= new CRebarBMData;
		pRebarBM->m_dDia			= pRB->m_dDia;
		pRebarBM->m_dTotalLength	= Round(toM(pRB->GetLength(TRUE, TRUE) * pRB->m_nEa), 3);
		pRebarBM->m_dUWeight		= dUWeight;
		pRebarBM->m_Fy				= pRB->m_Fy;
		pRebarBM->m_dRateAdd		= pRB->m_dRateAdd;
		pRebarBM->m_sNameBigClass	= sNameBigClass;
		pRebarBM->m_sNameUnit		= "TON";

		if(pRB->m_Fy > 3000)
			pArrRebarBMDataH->Add(pRebarBM);
		else
			pArrRebarBMDataD->Add(pRebarBM);
	}
}

// 아스팔트 방수 수량
// 항상 들어가는 곳 : 브라켓 측면, 경사면, 벽체외측면
// 옵션 : 슬래브 측면, 날개벽 내측
void COutBM::MakeBMAsphaltProof()
{
	CRcBridgeRebar *pBri	= GetRcBridgeRebar(FALSE);
	if(!pBri) return;

	// 나중에 옵션으로 뺄 것임.
	BOOL bIncludeSlabSide	= TRUE;

	CHgBaseBMStd baseBM;
	CTwinVector tvW, tvH;
	CTwinVectorArray tvArrPlane;

	// 기본 타입 설정
	CBMData bmBase;
	bmBase.m_nIdxBigClass		= BM_BIG_ASPHALTPROOF;
	bmBase.m_sBigClass			= GetStringBigClass(bmBase.m_nIdxBigClass);
	bmBase.m_nLevelTotal		= GetLevelTotal(bmBase.m_nIdxBigClass); 
	bmBase.m_nIdxMidClass		= BM_MID_ASPHALTPROOF;
	bmBase.m_sMidClass			= GetStringMidClass(bmBase.m_nIdxMidClass);
	bmBase.m_nIdxSmallClass		= BM_SMALL_ASPHALTPROOF;
	bmBase.m_sSmallClass		= GetStringSmallClass(bmBase.m_nIdxSmallClass);
	bmBase.m_nTypeResult		= RESULT_AREA;
	bmBase.m_sUnit				= "㎡";
	bmBase.m_nShape				= SHAPE_TV_RECTANGLE;
	bmBase.m_dwSide				= SIDE_FRONT;
	bmBase.m_bIsPrintByRefForm	= TRUE;
	bmBase.m_nLevelMidClass		= 2;

	long stt = 0; for(stt = 0; stt < 2; stt++)
	{
		BOOL bStt		= stt == iSTT;
		CString sStt	= bStt ? "시점" : "종점";

		pBri->GetTvWingWallPlane(bStt, TRUE, FALSE, TRUE);

		// 1. 벽체 외측
		if(pBri->IsOutWall(bStt))
		{
			CString sWall	= _T("");
			sWall.Format("%s벽체%s면", sStt, bStt ? "좌측" : "우측");
			pBri->m_tvArrPlaneWingWall.GetTvByInfo("슬래브끝단내측", tvW);
			pBri->m_tvArrVertSection.GetTvByInfo(sWall, tvH);

			CBMData *pBM			= new CBMData(&bmBase);
			pBM->m_sNoteTail		= bStt ? _T("시점슬래브끝단내측") : _T("종점슬래브끝단내측");
			pBM->m_dV[WidthLower]	= Round(toM(~(tvW.m_v2 - tvW.m_v1)),3);
			pBM->m_dV[Height]		= Round(toM(~(tvH.m_v2 - tvH.m_v1)),3);
			pBM->CalculateResult();
			m_pArrBMBridgeStd.Add(pBM);
		}

		// 2. 브라켓 측면과 경사면
		if(pBri->IsBracket(bStt))
		{
			CString sBraket	= _T("");

			// 브라켓 측면
			sBraket.Format("%s브라켓측면", sStt);
			pBri->m_tvArrPlaneWingWall.GetTvByInfo("브라켓", tvW);
			pBri->m_tvArrVertSection.GetTvByInfo(sBraket, tvH);

			CBMData *pBM			= new CBMData(&bmBase);
			pBM->m_sNoteTail		= bStt ? _T("시점브라켓측면") : _T("종점브라켓측면");
			pBM->m_dV[WidthLower]	= Round(toM(~(tvW.m_v2 - tvW.m_v1)),3);
			pBM->m_dV[Height]		= Round(toM(~(tvH.m_v2 - tvH.m_v1)),3);
			pBM->CalculateResult();
			m_pArrBMBridgeStd.Add(pBM);

			// 브라켓 경사면
			sBraket.Format("%s브라켓헌치", sStt);
			pBri->m_tvArrVertSection.GetTvByInfo(sBraket, tvH);

			CBMData *pBM1			= new CBMData(&bmBase);
			pBM1->m_sNoteTail		= bStt ? _T("시점브라켓경사면") : _T("종점브라켓경사면");
			pBM1->m_dV[WidthLower]	= Round(toM(~(tvW.m_v2 - tvW.m_v1)),3);
			pBM1->m_dV[Height]		= Round(toM(~(tvH.m_v2 - tvH.m_v1)),3);	// tv.m_dAngle에 쓰레기 값이 들어가면 GetLength가 안되는건지??
			pBM1->CalculateResult();
			m_pArrBMBridgeStd.Add(pBM1);
		}

		// 3. 슬래브 측면 
		if(bIncludeSlabSide)
		{
			CTwinVector tvSlab;
			CString sSlab	= _T("");
			if(pBri->IsOutWall(bStt)) sSlab.Format("%s브라켓상부측면", sStt);
			else sSlab.Format("%s슬래브측면", sStt);
			pBri->m_tvArrVertSection.GetTvByInfo(sSlab, tvH);
			
			pBri->m_tvArrPlaneWingWall.GetTvByInfo("슬래브끝단내측", tvW);

			CBMData *pBM			= new CBMData(&bmBase);
			pBM->m_sNoteTail		= bStt ? _T("시점슬래브측면") : _T("종점슬래브측면");
			pBM->m_dV[WidthLower]	= Round(toM(~(tvW.m_v2 - tvW.m_v1)),3);
			pBM->m_dV[Height]		= Round(toM(~(tvH.m_v2 - tvH.m_v1)),3);
			pBM->CalculateResult();
			m_pArrBMBridgeStd.Add(pBM);
		}
	}

	// 날개벽
	pBri = GetRcBridgeRebar(TRUE);
	for(stt = 0; stt < 2; stt++)
	{
		BOOL bStt = stt == iSTT;
		CString sStt = bStt ? "시점" : "종점";

		for(long left = 0; left < 2; left++)
		{
			BOOL bLeft = left == iLEFT;
			CString sLeft = bLeft ? "좌측" : "우측";

			if(!pBri->IsWingWall(bStt, bLeft)) continue;
			CTypedPtrArray <CObArray, CBMData*> *pArrBMStd	= &m_pArrBMWingStd[stt][left];

			// 날개벽거푸집(배면) 수량과 같음 /////////////////////
			CTypedPtrArray <CObArray, CBMData*> pArrForm;
			long nSizeBM	= pArrBMStd->GetSize();
			long i = 0; for(i = 0; i < nSizeBM; i++)
			{
				CBMData *pBM = pArrBMStd->GetAt(i);
				if(pBM->m_sInfo == "날개벽거푸집(배면)" || pBM->m_sInfo == "날개벽거푸집(배면헌치)")
				{
					CBMData *pForm = new CBMData;
					*pForm = *pBM;
					// 거푸짚 종류별로 묶어져 있으면 번호가 순서대로 정렬되지 않는다.
					// 그래서 여기서는 거푸집 종류를 모두 같은 것으로 해준다.
					pForm->m_sMidClass		= _T("");
					pForm->m_nIdxMidClass	= -1;
					pForm->m_sSmallClass	= _T("");
					pForm->m_nIdxSmallClass	= -1;
					pForm->m_sItemClass		= _T("");
					pForm->m_nIdxItemClass	= -1;
					pForm->m_sPartClass		= _T("");
					pForm->m_nIdxPartClass	= -1;
					pForm->m_sMeterClass	= _T("");
					pForm->m_nIdxMeterClass	= -1;

					pArrForm.Add(pForm);
				}
			}			
			/////////////////////////////////////////////////////////

			// 수집한 거푸집 수량들을 추가시킴 //
			CBMData *pBM = new CBMData(&bmBase);
			pBM->m_sNoteTail = bStt ? _T("시점날개벽거푸집") : _T("종점날개벽거푸집");
			CString str = _T("");
			nSizeBM	= pArrForm.GetSize();
			for(i = 0; i < nSizeBM; i++)
			{
				CBMData *pBMForm	= pArrForm.GetAt(i);
				pBM->m_dArrAddValue.Add(pBMForm->GetResult());
				
				str	= baseBM.GetCircleText(pBMForm->m_nNumbering);
				if(pBM->m_sForm.Find(str, 0) == -1)
				{
					CString strTmp = _T("");
					strTmp.Format(",%s", str);
					pBM->m_sForm += strTmp;
				}
			}
			
			str = pBM->m_sForm.Right(pBM->m_sForm.GetLength() - 1);
			pBM->m_sForm = (str + "의 거푸집(배면) 수량과 동일");
			
			pBM->m_bIsPrintByRefForm	= FALSE;
			pBM->CalculateResult();
			pArrBMStd->Add(pBM);
			////////////////////////////////////////////

			AhTPADelete(&pArrForm,(CBMData*)0);
		}
	}
}

// 거푸집 높이별로 자를때 기준이 되는 EL
// 지점별 기초 상단으로 하자
// 단 : -1이면 가장 낮은 EL을 기준으로 함
double COutBM::GetElBaseForForm(long nJ)
{
	CRcBridgeRebar *pBri	= GetRcBridgeRebar(TRUE);
	if(!pBri) return 0;
	CRcBridgeRebar *pBriBM	= GetRcBridgeRebar(FALSE);
	if(!pBri) return 0;

	CDRect rect	= pBri->m_tvArrVertSection.GetRect();
	double dEL	= rect.top - pBri->m_dTS;

	//Box형일때 Footing정보가 없으므로 시점el에서 슬래브두께만큼 올려서 계산
	if(pBri->IsBoxType())
	{
		return pBri->m_dELSB + pBri->m_dTS_Lower;
	}
	BOOL bFind	= FALSE;
	long nCountJijum	= pBri->GetCountJijum();
	long j = 0; for(j = 0; j < nCountJijum; j++)
	{
		if(nJ != -1 && nJ != j) continue;

		CFootingApp *pFooting	= pBri->GetFootingByJijumNumber(j);
		if(!pFooting) continue;

		double dELDiffByJong	= pBriBM->GetDiffELSttAndJijumForVertSection(j, FALSE);
		dEL	= min(pFooting->m_dEL - dELDiffByJong, dEL);
		bFind	= TRUE;
	}

	// 해당 지점에 기초가 없다면 가장 낮은걸 사용
	if(!bFind)
	{
		long nCountFooting	= pBri->GetCountFooting();
		long i = 0; for(i = 0; i < nCountFooting; i++)
		{
			CFootingData *pFooting	= pBri->GetFooting(i);
			if(pFooting)
			{
				double dELDiffByJong	= pBriBM->GetDiffELSttAndJijumForVertSection(i, FALSE);
				dEL	= min(pFooting->m_dEL - dELDiffByJong, dEL);
			}
		}
	}

	return dEL;
}

/// 아치교 동바리의 삽오 아래쪽에 면적 표기
void COutBM::DimTextPostAreaOfArch(CDomyun *pDomP)
{
	CRcBridgeRebar *pBri	= GetRcBridgeRebar(FALSE);
	if(!pBri) return;
	if(pBri->m_nTypeBridge != BRIDGE_TYPE_ARCH) return;
	
	pDomP->SetCalcExtRect();
	CDRect rect	= pDomP->GetExtRect();

	CDomyun Dom(pDomP);
	Dom.SetTextHeight(Dom.GetTextHeight(FALSE)*1.5);
	CHgBaseBMStd baseBM;

	// 각 지간
	CDPoint xyText	= CDPoint(rect.left + rect.Width()/3, rect.top - Dom.GetTextHeight()*2);
	CString sText	= _T("");
	BOOL bFirst	= TRUE;
	long nNum	= 1;

	xyText.x	= rect.left + rect.Width()/5;
	Dom.TextOut(xyText, "Area = ");
	for(long jigan = 0; jigan < pBri->m_nQtyJigan; jigan++)
	{
		nNum	= GetTvAreaShapePostJigan(&m_pArrAreaConc, jigan, nNum);

		long nSize	= m_pArrAreaConc.GetSize();
		long i = 0; for(i = 0; i < nSize; i++)
		{
			CTwinVectorAreaShape *pArea	= m_pArrAreaConc.GetAt(i);

			if(pArea->m_sInfo.Find("헌치R") != -1 || pArea->m_sInfo.Find("아치") != -1)
			{
				if(bFirst)
					xyText.x = rect.left + rect.Width()/3;
				else
					xyText.x = rect.left + rect.Width()/3 + rect.Width()/5;
				sText.Format("%s %.3f ㎡", baseBM.GetCircleText(i+(nNum-nSize)+2), toM(toM(pArea->m_dArcAreaL)));
				Dom.TextOut(xyText, sText);

				if(!bFirst)
					xyText.y -= Dom.GetTextHeight()*1.5;

				bFirst = !bFirst;
			}
		}
	}

	*pDomP << Dom;
}

// 아치리브 콘크리트 / 거푸집을 위한 면적,길이 정보 출력
void COutBM::DimTextArchRibConcAndForm(CDomyun *pDomP)
{
	CRcBridgeRebar *pBri	= GetRcBridgeRebar(FALSE);
	if(!pBri) return;
	if(pBri->m_nTypeBridge != BRIDGE_TYPE_ARCH) return;
	
	pDomP->SetCalcExtRect();
	CDRect rect	= pDomP->GetExtRect();

	CDomyun Dom(pDomP);
	Dom.SetTextHeight(Dom.GetTextHeight(FALSE));
	CHgBaseBMStd baseBM;

	double dAreaRib		= 0;
	double dLenUpper	= Round(toM(pBri->m_tvArrVertSectionRib[iUPPER].GetLength()), 3);
	double dLenLower	= Round(toM(pBri->m_tvArrVertSectionRib[iLOWER].GetLength()), 3);

	
	// 리브 면적
	CTwinVector tv;
	CDPointArray vArr;
	long nSize = pBri->m_tvArrVertSectionRib[0].GetSize();
	long n=0; for(n=0; n<nSize; n++)
	{
		tv = pBri->m_tvArrVertSectionRib[0].GetAt(n);
		vArr.Add(tv.m_v1);
		if(n==nSize-1)
			vArr.Add(tv.m_v2);
	}
	nSize = pBri->m_tvArrVertSectionRib[1].GetSize();
	for(n=nSize-1; n>=0; n--)
	{
		tv = pBri->m_tvArrVertSectionRib[1].GetAt(n);
		vArr.Add(tv.m_v2);
		if(n==0)
			vArr.Add(tv.m_v1);
	}
	dAreaRib	= Round(toM(toM(pBri->GetPolygonArea(vArr, vArr.GetSize()))), 3);

	// 각 지간
	CDPoint xyText	= CDPoint(rect.left + rect.Width()/3, rect.top - Dom.GetTextHeight()*2);
//	CString sText	= _T("");

	xyText.x	= rect.left + rect.Width()/3;

	CString sText	= "아치리브";
	Dom.TextOut(xyText, sText);

	sText.Format("Area : %.3f ㎡", dAreaRib);
	xyText.y -= Dom.GetTextHeight() * 1.5;
	Dom.TextOut(xyText, sText);

	sText.Format("Upper Surface : %.3f m", dLenUpper);
	xyText.y -= Dom.GetTextHeight() * 1.5;
	Dom.TextOut(xyText, sText);

	sText.Format("Lower Surface : %.3f m", dLenLower);
	xyText.y -= Dom.GetTextHeight() * 1.5;
	Dom.TextOut(xyText, sText);
	
	*pDomP << Dom;
}

void COutBM::DimTextLengthOfGuardWall(CDomyun *pDomP, CGuardWallRC *pGW, long nIdx, long nHDan)
{
	CRcBridgeRebar *pBri	= GetRcBridgeRebar(FALSE);
	if(!pBri) return;
	if(!pGW) return;
	
	pDomP->SetCalcExtRect();
	CDRect rect	= pDomP->GetExtRect();

	CDomyun Dom(pDomP);
	CARcBridgeDBStandard DBStd(pBri, m_pStd->m_pARoadOptionStd, m_pStd->m_pARcBridgeDataStd);
	DBStd.ApplyDimSetting(pDomP, &Dom);
	//Dom.SetTextHeight(Dom.GetTextHeight(FALSE));
	CHgBaseBMStd baseBM;

	double dLenBridge	= toM(pBri->GetGuardWallTotalLength(pGW, 0));
	double dLenAdd		= toM(pBri->GetGuardWallTotalLength(pGW, 1));
	double dLenTot		= dLenBridge + dLenAdd;

//	CDPoint xyText	= CDPoint(rect.left + rect.Width()/3, rect.top - Dom.GetTextHeight()*2);
	CString sText	= _T("");

	// 구간 정보 왼쪽 위에 출력
	/*
	xyText	= CDPoint(rect.left, rect.bottom + Dom.GetTextHeight()*2);
	BOOL bRail	= pBri->m_pARcBridgeDataStd->IsRailDesign();
	sText.Format("%s %s", baseBM.GetCircleText(nIdx+1), GetStringTypeHDanByNum(pBri->GetTypeHDan(nHDan, FALSE), bRail));
	Dom.TextOut(xyText, sText);
	*/


	// 길이 정보 아래쪽에 출력
	Dom.SetTextAlignHorz(TA_LEFT);
	CDPoint xyText	= CDPoint(rect.left, rect.top-Dom.GetTextHeight());

	sText.Format("교량 길이 = %.3f m", dLenBridge);
	xyText.y -= Dom.GetTextHeight() * 1.5;
	Dom.TextOut(xyText, sText);

	sText.Format("방호벽 추가 길이 = %.3f m", dLenAdd);
	xyText.y -= Dom.GetTextHeight() * 1.5;
	Dom.TextOut(xyText, sText);

	xyText.y -= Dom.GetTextHeight() * 0.25;
	Dom.SetCalcExtRect();
	rect	= Dom.GetExtRect();
	Dom.LineTo(xyText.x, xyText.y, xyText.x + Dom.GetTextWidth(sText), xyText.y);

	sText.Format("계 = %.3f m", dLenTot);
	xyText.y -= Dom.GetTextHeight() * 1.25;
	Dom.TextOut(xyText, sText);
	
	*pDomP << Dom;
}

// 신축이음 수량
void COutBM::MakeBMFlexibleJoint()
{
	CRcBridgeRebar *pBri	= GetRcBridgeRebar(FALSE);
	if(!pBri) return;
	CTypedPtrArray <CObArray, CBMData*> *pArrBM	= &m_pArrBMBridgeStd;

	long stt = 0; for(stt = 0; stt < 2; stt++)
	{
		BOOL bStt	= stt == iSTT;

		if(pBri->m_bExpJoint[bStt ? 0 : pBri->m_nQtyJigan])
		{

			BOOL bFirst = TRUE;
			double dWidthNone = 0;
			double dResult = 0;
			CString sSum = _T("");
			long dPrevNextCnt = 0;

			//차도 양옆으로 있으면 여유치수는 0.07 * 2
			//차도 양쪽중 한쪽만 있으면 여유치수는 0.07 * 1;
			for(long nHDan = 0; nHDan < pBri->GetQtyHDan(FALSE); nHDan++)
			{
				CGuardWallRC* pGW = pBri->GetGuardWallByHDan(nHDan, FALSE);
				if(!pGW) continue;
				if(pGW->IsTypeHDanOnlyNone()) //도로인경우
				{
					//양쪽다 있는경우
					if(pBri->GetGuardWallByHDan(nHDan-1, FALSE) && pBri->GetGuardWallByHDan(nHDan+1, FALSE))
						dPrevNextCnt = 2;
					else
						dPrevNextCnt = 1;

					dWidthNone = toM(pBri->GetLengthHDan(nHDan, FALSE));
					
					if(bFirst)
					{
						sSum.Format("%.3f + 0.07 x %d", dWidthNone, dPrevNextCnt);	
						bFirst = FALSE;
					}
					else
					{
						CString strAdd = _T("");
						strAdd.Format("+ %.3f + 0.07 x %d", dWidthNone, dPrevNextCnt);
						sSum += strAdd;						
					}

					dResult += dWidthNone + 0.07 * dPrevNextCnt;
				}
			}

			CBMData *pBM = new CBMData;
			pBM->m_nIdxBigClass			= BM_BIG_FLEXIBLEJOINT;
			pBM->m_sBigClass			= GetStringBigClass(pBM->m_nIdxBigClass);
			pBM->m_nIdxMidClass			= BM_MID_FLEXIBLEJOINT;
			pBM->m_sMidClass			= GetStringMidClass(pBM->m_nIdxMidClass);
			pBM->m_nIdxSmallClass		= BM_SMALL_FLEXIBLEJOINT;
			pBM->m_sSmallClass			= GetStringSmallClass(pBM->m_nIdxSmallClass, bStt);
			pBM->m_nLevelTotal			= GetLevelTotal(pBM->m_nIdxBigClass);
			pBM->m_nTypeResult			= RESULT_LENGTH;
			pBM->m_sUnit				= "m";
			pBM->m_sNoteHead			= "신축이음 길이(차도부 길이 + 여유치수)";
			pBM->m_sForm				= sSum;
			pBM->SetResult(dResult);
			pBM->m_bIsPrintByRefForm	= TRUE;
			pBM->m_nLevelMidClass		= 2;
			pArrBM->Add(pBM);
		}
	}
}

// 신축이음덮개 수량
void COutBM::MakeBMFlexibleJointCover()
{
	CRcBridgeRebar *pBri	= GetRcBridgeRebar(FALSE);
	if(!pBri) return;
	CTypedPtrArray <CObArray, CBMData*> *pArrBM	= &m_pArrBMBridgeStd;

	
	if(pBri->m_bExpJoint[0] || pBri->m_bExpJoint[pBri->m_nQtyJigan])
	{
		long nCountHDan[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};	
		for(long nHDan = 0; nHDan < pBri->GetQtyHDan(FALSE); nHDan++)
		{
			CGuardWallRC* pGW = pBri->GetGuardWallByHDan(nHDan, FALSE);
			if(!pGW) continue;
			if(pGW->IsTypeHDanOnlyNone()) //도로, 차도 제외
				continue;
				
			//종류별로 개수를 세자 .
			if(pGW->IsTypeHDanRoadGuardFence()) nCountHDan[HDAN_TYPE_ROAD_GUARDFENCE]++;				//방호벽
			else if(pGW->IsTypeHDanRoadFootwayBlock()) nCountHDan[HDAN_TYPE_ROAD_FOOTWAYBLOCK]++;		//보차도경계
			else if(pGW->IsTypeHDanRoadCentralWallFull()) nCountHDan[HDAN_TYPE_ROAD_CENTERGUARD]++;		//중분대
			else if(pGW->IsTypeHDanRoadCentralWallHalf()) nCountHDan[HDAN_TYPE_ROAD_CENTERGUARDHALF]++;	//중분대1/2
			else if(pGW->IsTypeHDanRoadSoundProof()) nCountHDan[HDAN_TYPE_ROAD_SOUNDPROOF]++;			//방음벽기초
			else if(pGW->IsTypeHDanRoadRetainingWall()) nCountHDan[HDAN_TYPE_ROAD_RETAININGWALL]++;		//흙막이벽
			else if(pGW->IsTypeHDanRoadHandRail()) nCountHDan[HDAN_TYPE_ROAD_HANDRAIL]++;				//난간기초
			else if(pGW->IsTypeHDanRoadDike()) nCountHDan[HDAN_TYPE_ROAD_DIKE]++;						//다이크
		}

		// 10 : 최대종류개수
		for(long nCnt = 0; nCnt < 10; nCnt++)
		{
			if(nCountHDan[nCnt] <= 0) continue;
	
			CBMData *pBM = new CBMData;
			pBM->m_nIdxBigClass		= BM_BIG_FLEXIBLEJOINT_COVER;
			pBM->m_sBigClass		= GetStringBigClass(pBM->m_nIdxBigClass);
			pBM->m_nIdxMidClass		= BM_MID_FLEXIBLEJOINT_COVER;
			pBM->m_sMidClass		= GetStringMidClass(pBM->m_nIdxMidClass);
			pBM->m_nIdxSmallClass	= BM_SMALL_FJ_NONE + nCnt;
			pBM->m_sSmallClass		= GetStringSmallClass(pBM->m_nIdxSmallClass);
			pBM->m_nLevelTotal		= GetLevelTotal(pBM->m_nIdxBigClass);
			pBM->m_nLevelMidClass	= 2;
			pBM->m_nTypeResult		= RESULT_EA;
			pBM->m_sUnit			= "EA";
			pBM->m_dV[Ea]			= nCountHDan[nCnt] * 2;
			pBM->CalculateResult();
			pArrBM->Add(pBM);
		}

	}
	
}

// 신축이음의 무수축콘크리트
void COutBM::MakeBMNonShrinkageConc()
{
	CRcBridgeRebar *pBri	= GetRcBridgeRebar(FALSE);
	if(!pBri) return;
	CTypedPtrArray <CObArray, CBMData*> *pArrBM	= &m_pArrBMBridgeStd;

	long stt = 0; for(stt = 0; stt < 2; stt++)
	{
		BOOL bStt	= stt == iSTT;

		if(pBri->IsBlockOut(bStt))
		{
			CFlexibleJoint *pFx	= bStt ? &pBri->m_fxJointStt : &pBri->m_fxJointEnd;
			double dSta		= pBri->GetStationByTarget(bStt ? BRIDGE_OFF_STT_SLAB : BRIDGE_OFF_END_SLAB, 0, TRUE);
			double dWidthSlab	= toM(pBri->GetWidthSlabActByGagak(dSta, pBri->GetAngleJijum(0, FALSE), 0, FALSE));
			double dW		= toM(pFx->m_dWidth);
			double dH		= toM(pFx->m_dHeight);
			long nQty		= 2; // 2는 교대측도 포함이라서.
			double dResult	= (dW * dH) * dWidthSlab * nQty;

			CBMData *pBM = new CBMData;
			pBM->m_nIdxBigClass			= BM_BIG_SHRINKAGE;
			pBM->m_sBigClass			= GetStringBigClass(pBM->m_nIdxBigClass);
			pBM->m_nIdxMidClass			= BM_MID_SHRINKAGE;
			pBM->m_sMidClass			= GetStringMidClass(pBM->m_nIdxMidClass);
			pBM->m_nIdxSmallClass		= BM_SMALL_SHRINKAGE;
			pBM->m_sSmallClass			= GetStringSmallClass(pBM->m_nIdxSmallClass, bStt);
			pBM->m_nLevelTotal			= GetLevelTotal(pBM->m_nIdxBigClass);
			pBM->m_nTypeResult			= RESULT_VOLUMN;
			pBM->m_sUnit				= "㎥";
			pBM->m_sNoteHead			= "교대측 무수축 콘크리트 포함";
			pBM->m_sForm.Format("(%.3f x %.3f) x %.3f x %d", dW, dH, dWidthSlab, nQty);
			pBM->SetResult(dResult);
			pBM->m_bIsPrintByRefForm	= TRUE;
			pBM->m_nLevelSmallClass		= 1;

			pArrBM->Add(pBM);
		}
	}
}

// cover plate
void COutBM::MakeBMCoverPlate()
{
	CRcBridgeRebar *pBri	= GetRcBridgeRebar(FALSE);
	if(!pBri) return;
	if(!pBri->IsBlockOut(TRUE) && !pBri->IsBlockOut(FALSE)) return;

	CTypedPtrArray <CObArray, CBMData*> *pArrBM	= &m_pArrBMBridgeStd;

	long nIdx	= 0;
	for(long nHDan = 0; nHDan < pBri->GetQtyHDan(FALSE); nHDan++)
	{
		CGuardWallRC* pGW = pBri->GetGuardWallByHDan(nHDan, FALSE);
		if(!pGW) continue;
		if((pGW->IsTypeHDanRoadTotalGuardFence() && !pGW->IsTypeHDanRoadFootwayBlock()) ||
			(pGW->IsTypeHDanRailGuardFence() && !pGW->IsTypeHDanRailFootwayBlock()))
		{
			CTwinVectorArray tvArrSec;
			pBri->GetTvGuardWall(pGW, tvArrSec, pBri->m_dStationBridgeStt, CDPoint(0, 1), TRUE, FALSE, TRUE, FALSE);
			double dResult	= toM(tvArrSec.GetLength());

			CBMData *pBM = new CBMData;
			pBM->m_nIdxBigClass			= BM_BIG_COVERPLATE;
			pBM->m_sBigClass			= GetStringBigClass(pBM->m_nIdxBigClass);
			pBM->m_nLevelTotal			= GetLevelTotal(pBM->m_nIdxBigClass);
			pBM->m_nIdxSmallClass		= nIdx++;
			pBM->m_sSmallClass			= "";
			pBM->m_nTypeResult			= RESULT_LENGTH;
			pBM->m_sUnit				= "m";
			pBM->m_sForm.Format("%s - %.3f m", pBri->GetStringTypeHDan(nHDan, FALSE, FALSE), dResult);
			pBM->SetResult(dResult);
			pBM->m_sFormForCompare		= pBM->m_sForm;

			pArrBM->Add(pBM);
		}
	}
}

void COutBM::MakeBMConcreteMonument()
{
	CRcBridgeRebar *pBri = GetRcBridgeRebar();
	CRcBridgeRebar *pBriOrg = GetRcBridgeRebar(TRUE);
	if(!pBri) return;
	if(!pBriOrg) return;
	if(pBri->m_nMonumenu == 0) return;

	double dFck = pBri->m_dMonuFck ;
	double dFy  = pBri->m_dMonuR_Fy ;
	double dAggregate = pBri->m_dMonuAggregate;


	CTypedPtrArray <CObArray, CBMData*> *pArrBMStd	= &m_pArrBMMonumentStd;
	

	CBMData *pBM				= new CBMData;
	pBM->m_nNumbering			= 1;
	pBM->m_nLevelTotal			= 2;
	pBM->m_nIdxBigClass			= BM_BIG_CONCRETE;
	pBM->m_sBigClass				= GetStringBigClass(pBM->m_nIdxBigClass);
	pBM->m_nIdxMidClass			= BM_MID_CONCRETE_GUIDEWALL; 
	pBM->m_sMidClass				= GetStringMidClass(BM_MID_CONCRETE_MONU, NULL, ePartUpperSlab, NULL, dFck);
	pBM->m_nIdxSmallClass		= BM_SMALL_FJ_GUARDFENCE;
	pBM->m_sSmallClass			= GetStringSmallClass(pBM->m_nIdxSmallClass);
	pBM->m_nIdxItemClass		= BM_ITEM_CONCRETE_GUARDWALL;
	pBM->m_sItemClass			= GetStringItemClass(pBM->m_nIdxItemClass);
	pBM->m_nTypeResult			= RESULT_VOLUMN;
	pBM->m_sUnit					= "㎥";
	pBM->m_nShape				= SHAPE_TRAPEZOID;
	pBM->m_dV[WidthLower]		= Round(toM(pBri->m_dMonuH2+pBri->m_dMonuH3),3);
	pBM->m_dV[WidthUpper]		= Round(toM(pBri->m_dMonuH2),3);
	pBM->m_dV[WidthDiffLeft]	= 0;
	pBM->m_dV[WidthDiffRight]	= pBM->m_dV[WidthLower] - pBM->m_dV[WidthUpper];
	pBM->m_dV[Height]			= Round(toM(pBri->m_dMonuD),3);
	pBM->m_dV[Length]			= Round(toM(pBri->m_dMonuL),3);
	pBM->m_dwSide				= SIDE_FRONT;
	pBM->m_sInfo				= "교명주";
	pBM->CalculateResult();
	long i = 0; for(i = 0; i < pBri->m_nMonumenu; i++)
	{
		CBMData *pBMTmp	= new CBMData(pBM);
		pArrBMStd->Add(pBMTmp);
	}
	delete pBM;


	pBM							= new CBMData;
	pBM->m_nNumbering			= 2;
	pBM->m_nLevelTotal			= 2;
	pBM->m_nIdxBigClass			= BM_BIG_CONCRETE;
	pBM->m_sBigClass				= GetStringBigClass(pBM->m_nIdxBigClass);
	pBM->m_nIdxMidClass			= BM_MID_CONCRETE_GUIDEWALL; 
	pBM->m_sMidClass				= GetStringMidClass(BM_MID_CONCRETE_MONU, NULL, ePartUpperSlab, NULL,dFck);
	pBM->m_nIdxSmallClass		= BM_SMALL_FJ_GUARDFENCE;
	pBM->m_sSmallClass			= GetStringSmallClass(pBM->m_nIdxSmallClass);
	pBM->m_nIdxItemClass		= BM_ITEM_CONCRETE_GUARDWALL;
	pBM->m_sItemClass			= GetStringItemClass(pBM->m_nIdxItemClass);
	pBM->m_nTypeResult			= RESULT_VOLUMN;
	pBM->m_sUnit				= "㎥";
	pBM->m_nShape				= SHAPE_RECTANGLE;
	pBM->m_dV[WidthLower]		= Round(toM(pBri->m_dMonuW-pBri->m_dMonuD),3);
	pBM->m_dV[Height]			= Round(toM(pBri->m_dMonuH1),3);
	pBM->m_dV[Length]			= Round(toM(pBri->m_dMonuL),3);
	pBM->m_dwSide				= SIDE_FRONT;
	pBM->m_sInfo				= "교명주";
	pBM->CalculateResult();
	for(i = 0; i < pBri->m_nMonumenu; i++)
	{
		CBMData *pBMTmp	= new CBMData(pBM);
		pArrBMStd->Add(pBMTmp);
	}
	delete pBM;
}

void COutBM::MakeBMFormMonument()
{
	CRcBridgeRebar *pBri = GetRcBridgeRebar();
	CRcBridgeRebar *pBriOrg = GetRcBridgeRebar(TRUE);
	if(!pBri) return;
	if(!pBriOrg) return;

	
	if(pBri->m_nMonumenu == 0) return;

	CTypedPtrArray <CObArray, CBMData*> *pArrBMStd	= &m_pArrBMMonumentStd;
	long	nSizeBM	= pArrBMStd->GetSize();
	double	dFirstMeter	= 7000;
	double	dSeconeMeter = 3000;

	// 앞뒤(사다리꼴)
	CBMData *pBM				= new CBMData;
	pBM->m_nNumbering			= 1;
	pBM->m_nIdxBigClass			= BM_BIG_FORM;
	pBM->m_nLevelTotal			= GetLevelTotal(pBM->m_nIdxBigClass);
	pBM->m_sBigClass			= GetStringBigClass(pBM->m_nIdxBigClass);
	pBM->m_nIdxMidClass			= GetFormByPos(FORM_POS_WALL_FRONT);
	pBM->m_sMidClass			= GetStringMidClass(pBM->m_nIdxMidClass);
	pBM->m_nIdxSmallClass		= BM_SMALL_FORM_VERTICAL;
	pBM->m_sSmallClass			= GetStringSmallClass(pBM->m_nIdxSmallClass);
	pBM->m_nIdxItemClass		= BM_ITEM_CONCRETE_MAIN;
	pBM->m_sItemClass			= GetStringItemClass(pBM->m_nIdxItemClass);
	pBM->m_nTypeResult			= RESULT_AREA;
	pBM->m_sUnit				= "㎡";
	pBM->m_nShape				= SHAPE_TRAPEZOID;
	pBM->m_dV[SttMeter]			= Round(toM(0),3);
	pBM->m_dV[EndMeter]			= Round(toM(7),3);
	pBM->m_dV[WidthLower]		= Round(toM(pBri->m_dMonuH2+pBri->m_dMonuH3),3);
	pBM->m_dV[WidthUpper]		= Round(toM(pBri->m_dMonuH2),3);
	pBM->m_dV[WidthDiffLeft]	= 0;
	pBM->m_dV[WidthDiffRight]	= pBM->m_dV[WidthLower] - pBM->m_dV[WidthUpper];
	pBM->m_dV[Height]			= Round(toM(pBri->m_dMonuD),3);
	pBM->m_dV[Length]			= Round(toM(pBri->m_dMonuL),3);
	pBM->m_dwSide				= SIDE_FRONT | SIDE_BACK;
	pBM->CalculateResult();
	pBM->m_dV[Result] = Round(pBM->m_dV[Result],3);
	long i = 0; for(i = 0; i < pBri->m_nMonumenu; i++)
	{
		CBMData *pBMTmp	= new CBMData(pBM);
		pArrBMStd->Add(pBMTmp);
	}
	delete pBM;

	// 사다리꼴 부분 측면 수직
	pBM							= new CBMData;
	pBM->m_nNumbering			= 1;
	pBM->m_nIdxBigClass			= BM_BIG_FORM;
	pBM->m_nLevelTotal			= GetLevelTotal(pBM->m_nIdxBigClass);
	pBM->m_sBigClass			= GetStringBigClass(pBM->m_nIdxBigClass);
	pBM->m_nIdxMidClass			= GetFormByPos(FORM_POS_WALL_FRONT);
	pBM->m_sMidClass			= GetStringMidClass(pBM->m_nIdxMidClass);
	pBM->m_nIdxSmallClass		= BM_SMALL_FORM_VERTICAL;
	pBM->m_sSmallClass			= GetStringSmallClass(pBM->m_nIdxSmallClass);
	pBM->m_nIdxItemClass		= BM_ITEM_CONCRETE_MAIN;
	pBM->m_sItemClass			= GetStringItemClass(pBM->m_nIdxItemClass);
	pBM->m_nTypeResult			= RESULT_AREA;
	pBM->m_sUnit				= "㎡";
	pBM->m_nShape				= SHAPE_RECTANGLE;
	pBM->m_dV[WidthLower]		= Round(toM(pBri->m_dMonuD),3);
	pBM->m_dV[Height]			= Round(toM(pBri->m_dMonuH2),3);
	pBM->m_dV[Length]			= Round(toM(pBri->m_dMonuL),3);
	pBM->m_dwSide				= SIDE_LEFT;
	pBM->CalculateResult();
	pBM->m_dV[Result] = Round(pBM->m_dV[Result],3);
	for(i = 0; i < pBri->m_nMonumenu; i++)
	{
		CBMData *pBMTmp	= new CBMData(pBM);
		pArrBMStd->Add(pBMTmp);
	}
	delete pBM;

	// 사다리꼴 부분 측면 경사
	pBM							= new CBMData;
	pBM->m_nNumbering			= 1;
	pBM->m_nIdxBigClass			= BM_BIG_FORM;
	pBM->m_nLevelTotal			= GetLevelTotal(pBM->m_nIdxBigClass);
	pBM->m_sBigClass			= GetStringBigClass(pBM->m_nIdxBigClass);
	pBM->m_nIdxMidClass			= GetFormByPos(FORM_POS_WALL_FRONT);
	pBM->m_sMidClass			= GetStringMidClass(pBM->m_nIdxMidClass);
	pBM->m_nIdxSmallClass		= BM_SMALL_FORM_SLOPE;
	pBM->m_sSmallClass			= GetStringSmallClass(pBM->m_nIdxSmallClass);
	pBM->m_nIdxItemClass		= BM_ITEM_CONCRETE_MAIN;
	pBM->m_sItemClass			= GetStringItemClass(pBM->m_nIdxItemClass);
	pBM->m_nTypeResult			= RESULT_AREA;
	pBM->m_sUnit				= "㎡";
	pBM->m_nShape				= SHAPE_TRIANGLE;
	pBM->m_dV[WidthLower]		= Round(toM(pBri->m_dMonuD),3);
	pBM->m_dV[WidthDiffLeft]	= Round(toM(pBri->m_dMonuD),3);
	pBM->m_dV[WidthDiffRight]	= pBM->m_dV[WidthLower] - pBM->m_dV[WidthUpper];
	pBM->m_dV[Height]			= Round(toM(pBri->m_dMonuH3),3);
	pBM->m_dV[Length]			= Round(toM(pBri->m_dMonuL),3);
	pBM->m_dwSide				= SIDE_LEFT;
	pBM->CalculateResult();
	pBM->m_dV[Result] = Round(pBM->m_dV[Result],3);
	for(i = 0; i < pBri->m_nMonumenu; i++)
	{
		CBMData *pBMTmp	= new CBMData(pBM);
		pArrBMStd->Add(pBMTmp);
	}
	delete pBM;
	
	// 앞뒤(사각형) / 측면
	pBM							= new CBMData;
	pBM->m_nNumbering			= 2;
	pBM->m_nIdxBigClass			= BM_BIG_FORM;
	pBM->m_nLevelTotal			= GetLevelTotal(pBM->m_nIdxBigClass);
	pBM->m_sBigClass			= GetStringBigClass(pBM->m_nIdxBigClass);
	pBM->m_nIdxMidClass			= GetFormByPos(FORM_POS_WALL_FRONT);
	pBM->m_sMidClass			= GetStringMidClass(pBM->m_nIdxMidClass);
	pBM->m_nIdxSmallClass		= BM_SMALL_FORM_VERTICAL;
	pBM->m_sSmallClass			= GetStringSmallClass(pBM->m_nIdxSmallClass);
	pBM->m_nIdxItemClass		= BM_ITEM_CONCRETE_MAIN;
	pBM->m_sItemClass			= GetStringItemClass(pBM->m_nIdxItemClass);
	pBM->m_nTypeResult			= RESULT_AREA;
	pBM->m_sUnit				= "㎡";
	pBM->m_nShape				= SHAPE_RECTANGLE;
	pBM->m_dV[WidthLower]		= Round(toM(pBri->m_dMonuW-pBri->m_dMonuD),3);
	pBM->m_dV[Height]			= Round(toM(pBri->m_dMonuH1),3);
	pBM->m_dV[Length]			= Round(toM(pBri->m_dMonuL),3);
	pBM->m_dwSide				= SIDE_FRONT | SIDE_BACK | SIDE_RIGHT;
	pBM->CalculateResult();
	pBM->m_dV[Result] = Round(pBM->m_dV[Result],3);
	for(i = 0; i < pBri->m_nMonumenu; i++)
	{
		CBMData *pBMTmp	= new CBMData(pBM);
		pArrBMStd->Add(pBMTmp);
	}
	delete pBM;

	// 수량에 미터정보를 추가한다.
	RemakeByHeightBMList(pArrBMStd, dFirstMeter, dSeconeMeter, nSizeBM, pArrBMStd->GetSize()-1);
}

long COutBM::DrawFirstPageMonument(CHgBaseBMStd *pBaseBMStd)
{
	if(!pBaseBMStd) return 5;

	CRcBridgeRebar *pBriOrg = GetRcBridgeRebar(TRUE);		// 방호벽은 원래 교량으로 작업함.
	if(!pBriOrg) return 5;

	CARcBridgeDBStandard DBStd(pBriOrg, m_pStd->m_pARoadOptionStd, m_pStd->m_pARcBridgeDataStd);
	DBStd.m_bApplyDimSettingOfBaseDom	= TRUE;

	// 실제로 출력되었으면 하는 사이즈
	double dWidthOrg = 200;	// 도면 가로크기

	// 종단면도의 스케일을 결정하자!!! ////////////////////////////////////
	// 그걸로 종단면과 관련된 도면을 그려내자.
	CDomyun domTmp;
	domTmp.SetCWnd(m_pStd->m_pView);
	domTmp.SetScaleDim(1);
	domTmp.SetDimTxt(1);

	CDPoint vAng(0,1);
	DBStd.m_pDrawCrossSection->DrawMonumentFront(&domTmp);
	DBStd.m_pDrawCrossSection->DimMonumentFront(&domTmp);

	/////////////////////////////////////////////////////////////////////////
	
	// 종단면도
	CDomyun domCrossSection;
	domCrossSection.SetCWnd(m_pStd->m_pView);

	// 도면 설정
	double dScale	= pBaseBMStd->GetScale(&domTmp, dWidthOrg);
	pBaseBMStd->SetDomyunValue(&domCrossSection, dWidthOrg, &domTmp);
			
	// 방호벽	
	DBStd.m_pDrawCrossSection->DrawMonumentFront(&domCrossSection);
	DBStd.m_pDrawCrossSection->DimMonumentFront(&domCrossSection, FALSE);

	// 엑셀에 삽입
	domCrossSection.RedrawByScale(dScale, TRUE);
	domCrossSection.SetCalcExtRect();

	double dWidth = dWidthOrg;//domCrossSection.GetExtRect().Width();
	double dHeight = 300;//domCrossSection.GetExtRect().Height();
	CDoubleArray dArrTmp;
	long nRow = pBaseBMStd->InsertDomyun(5, TA_CENTER, &domCrossSection, dArrTmp, dWidth, dHeight);

	// 평면도 그리기 /////////////////////////////////////////////////////////
	CDomyun domPlane;
	domPlane.SetCWnd(m_pStd->m_pView);
	dWidthOrg = 200;

	// 도면 설정 ////////////////////
	pBaseBMStd->SetDomyunValue(&domPlane, dWidthOrg, &domTmp);
	///////////////////////////////////

	DBStd.m_pDrawCrossSection->DrawMonumentPlane(&domPlane);
	DBStd.m_pDrawCrossSection->DimMonumentPlane(&domPlane, FALSE);


	// 그렸다면 원하는 크기로 사이즈 조정
	domPlane.RedrawByScale(dScale, TRUE);
	domPlane.SetCalcExtRect();

	// 엑셀에 삽입
	//double dRate	= domPlane.GetExtRect().Width() / dWidthOrg;
	//nRow	= pBaseBMStd->InsertDomyun(nRow+3, TA_CENTER, &domPlane, domPlane.GetExtRect().Width()/dRate, domPlane.GetExtRect().Height()/dRate);

	nRow	= pBaseBMStd->InsertDomyun(nRow+3, TA_CENTER, &domPlane, dArrTmp, dWidth, dHeight);

	DBStd.m_bApplyDimSettingOfBaseDom	= FALSE;


	return nRow;
}

// 교명주
void COutBM::MakeBMMonument()
{
	CRcBridgeRebar	*pBri	= GetRcBridgeRebar();
	if(!pBri) return;
	
	const long nCountMonument	= pBri->m_nMonumenu;

	CBMData *pBM			= new CBMData;
	pBM->m_nIdxBigClass		= BM_BIG_MONUMENT;
	pBM->m_sBigClass		= GetStringBigClass(pBM->m_nIdxBigClass);
	pBM->m_nLevelBigClass	= GetLevelTotal(pBM->m_nIdxBigClass);
	pBM->m_nTypeResult		= RESULT_EA;
	pBM->m_sUnit			= "EA";
	pBM->m_dV[Ea]			= nCountMonument;
	pBM->CalculateResult();
	m_pArrBMBridgeStd.Add(pBM);
}

// 노치
void COutBM::MakeBMNotch()
{
	CRcBridgeRebar	*pBri	= GetRcBridgeRebar();
	CRcBridgeRebar *pBriOrg	= GetRcBridgeRebar(TRUE);
	if(!pBri) return;

	CString str	= _T("");
	double dLen[2]	= {0, 0};
	CTwinVector tv;
	for(long left = 0; left < 2; left++)
	{
		BOOL bLeft	= left == iLEFT;

		CTwinVectorArray tvArrVertSection;
		pBriOrg->GetTvVertSection_Std(bLeft ? -1 : 1, TRUE, FALSE, FALSE, -1);

		if(pBriOrg->m_nTypeBridge == BRIDGE_TYPE_SLAB)
		{
			long i = 0; for(i = 0; i < pBriOrg->m_nQtyJigan; i++)
			{
				str.Format("슬래브상면(%d번째지간)", i+1);
				if(pBriOrg->m_tvArrVertSection.GetTvByInfo(str, tv))
					dLen[left]	+= tv.GetLength();
			}
		}
		else
		{
			dLen[left]	= 0;
			if(pBriOrg->m_tvArrVertSection.GetTvByInfo("슬래브상면(외측벽체안쪽)", tv))
				dLen[left]	+= tv.GetLength();
		}
		pBriOrg->GetTvVertSection_Std(bLeft ? -1 : 1, TRUE, FALSE, TRUE);
	}

	dLen[iLEFT]		= toM(Round(dLen[iLEFT], 3));
	dLen[iRIGHT]	= toM(Round(dLen[iRIGHT], 3));

	double dNotchH	= pBri->m_dNotchHeight;
	double dNotchW1	= pBri->m_dNotchWidth1;
	double dNotchW2 = pBri->m_dNotchWidth2;
	double dNotchW	= (dNotchW1+dNotchW2)/2;

	CBMData *pBM			= new CBMData;
	pBM->m_nIdxBigClass		= BM_BIG_NOTCH;
	pBM->m_sBigClass		= GetStringBigClass(pBM->m_nIdxBigClass);
	pBM->m_nLevelBigClass	= GetLevelTotal(pBM->m_nIdxBigClass);
	pBM->m_nTypeResult		= RESULT_LENGTH;
	pBM->m_sUnit			= "m";
	pBM->m_sForm.Format("%.3f + %.3f", dLen[iLEFT], dLen[iRIGHT]);
	pBM->m_sFormExcel.Format("%.3f + %.3f", dLen[iLEFT], dLen[iRIGHT]);
	pBM->m_sNoteHead.Format("(%.0f x %.0f)", dNotchH, dNotchW);
	pBM->SetResult(dLen[iLEFT] + dLen[iRIGHT]);
	pBM->m_bIsPrintByRefForm	= TRUE;
	m_pArrBMBridgeStd.Add(pBM);
}

// 슬래브 상단 El 산정식 표기
void COutBM::DimTextFormOfSlabELCalc(CDomyun *pDom)
{
	CRcBridgeRebar *pBri	= GetRcBridgeRebar(TRUE);
	if(!pBri) return;
	
	pDom->SetCalcExtRect();
	CDRect rect	= pDom->GetExtRect();

	CDomyun Dom(pDom);
	Dom.SetTextHeight(Dom.GetTextHeight(FALSE));
	CHgBaseBMStd baseBM;

	BOOL bBoxType = pBri->IsBoxType();
	double dHeightWall = 0;
	
	CDPoint xyText	= CDPoint(rect.left + rect.Width()/3, rect.top - Dom.GetTextHeight()*2);
	CString sText1	= _T("");
	CString sText2	= _T("");

	if(pBri->m_nTypeHeightWall == 0)
	{
		sText1	= "* 수량 산출시 벽체 기준 높이 : 도로중심 높이 기준";
	}
	else if(pBri->m_nTypeHeightWall == 1 || pBri->m_nTypeHeightWall == 3)
	{
		sText1	= "* 수량 산출시 벽체 기준 높이 : 평균 높이 기준((최대+최소)/2)";
	}
	else if(pBri->m_nTypeHeightWall == 2)
	{
		sText1	= "* 수량 산출시 벽체 기준 높이 : 최대 높이 기준";
	}
	xyText.x	= rect.left + ((rect.Width() - Dom.GetTextWidth(sText1))*3/5);
	Dom.TextOut(xyText, sText1);

	if(bBoxType)
	{
		CRcBridgeRebar *pBriConvert	= GetRcBridgeRebar(FALSE);
		// 변환시킨 Bridge에서 벽체 높이를 가져오자. 산출근거는 원래 Bridge에서.
		double dStaStt	= pBriConvert->GetStationByTarget(BRIDGE_OFF_STT_SLAB, 0, FALSE);
		double dELTop	= pBriConvert->GetXySlabUpperActByDist(dStaStt, 0, FALSE, FALSE, FALSE, FALSE).y;
		double dELBot	= pBriConvert->m_dELSB;
		dHeightWall = dELTop - dELBot;

		if(pBri->m_bSlopeLowerSlab == FALSE)
		{
			// 평균 벽체 높이 산출 근거
			dStaStt	= pBri->GetStationByTarget(BRIDGE_OFF_STT_SLAB, 0, FALSE);
			dELTop	= pBri->GetXySlabUpperActByDist(dStaStt, 0, FALSE, FALSE, FALSE, FALSE).y;
			dELBot	= pBri->m_dELSB;
			double dStaEnd	= pBri->GetStationByTarget(BRIDGE_OFF_END_SLAB, 0, FALSE);
			double dELEnd	= pBri->GetXySlabUpperActByDist(dStaEnd, 0, FALSE, FALSE, FALSE, FALSE).y;

			double dHStt = dELTop - dELBot;
			double dHEnd = dELEnd - dELBot;
			double dHAvg = (dHStt + dHEnd) / 2.;

			sText2.Format("- 평균 벽체 높이 : (%s + %s) / 2 = %s", COMMA(dHStt), COMMA(dHEnd), COMMA(dHAvg));
			xyText.y -= Dom.GetTextHeight() * 1.5;
			Dom.TextOut(xyText, sText2);
		}
	}

	long nCountJijum	= pBri->GetCountJijum();
	long i = 0; for(i = 0; i < nCountJijum; i++)
	{
		xyText.y -= Dom.GetTextHeight() * 1.5;
		if(i == 0 || i == nCountJijum-1)
		{
			if(!pBri->IsOutWall(i==0)) continue;
		}

		CFootingApp *pFooting	= pBri->GetFootingByJijumNumber(i);
		if(!pFooting) continue;

		double dSta	= pBri->GetStationOnJijum(i);
		if(i == 0 || i == nCountJijum-1)
		{
			dSta	= pBri->GetStationByTarget(i == 0 ? BRIDGE_OFF_STT_SLAB : BRIDGE_OFF_END_SLAB, 0, FALSE);
		}
		double dEL	= pBri->GetXySlabUpperActByDist(dSta, 0, FALSE, FALSE, FALSE, FALSE).y;
		double dH	= (bBoxType)? dHeightWall : dEL - pFooting->m_dEL;
		if(i == 0 || i == nCountJijum-1)
		{
			dH -= i==0 ? (pBri->m_dBTHS + pBri->m_dBHS) : (pBri->m_dBTHE + pBri->m_dBHE);
			if(bBoxType && pBri->m_bExistProtectionRiseFooting)
			{
				dH -= (pBri->m_dPRF_H1 + pBri->m_dPRF_H2);
			}
		}
		else
		{
			dH -= pBri->m_dTS;
			dH -= pBri->m_dHHL[i-1][iUPPER];

			if(bBoxType)
			{
				dH -= pBri->m_dTS_Lower;
				dH -= pBri->m_dHHL[i-1][iLOWER];
			}
		}
		
		CLineInfo *pLine	= pBri->GetLineBase();
		if(i == 0)
			dSta		= pBri->GetStationByTarget(BRIDGE_OFF_STT_SLAB, 0, TRUE);
		else if(i == nCountJijum-1)
			dSta		= pBri->GetStationByTarget(BRIDGE_OFF_END_SLAB, 0, TRUE);

		CDPoint vAng	= pBri->GetAngleByTarget(BRIDGE_OFF_STT_SLAB, 0);
		double dELMax	= pBri->GetElMaxSlabUpper(dSta, vAng);
		double dELMin	= pBri->GetElMinSlabUpper(dSta, vAng);
		dEL		= pLine->GetElevationPlanEl(dSta) - pBri->m_dTP;
		double dELUse	= pBri->m_nTypeHeightWall == 1 ? (dELMax+dELMin)/2 : dELMax;
		double dELDiff	= dEL - dELUse;

		if(bBoxType)
		{
			// 변환된 Bridge에서 구해온 벽체 길이 이기 때문에 dELDiff가 이미 반영되있다 그래서 다시 한번 더해주자.
			dH += dELDiff;
		}

		if(pBri->m_nTypeHeightWall == 0)
		{
			sText2.Format("- 지점%d 벽체 높이 : %s", i+1, COMMA(dH));
		}
		else if(pBri->m_nTypeHeightWall == 1 || pBri->m_nTypeHeightWall == 3)
		{
			if(pBri->m_nTypeHeightWall == 3)
			{
				double dHMinusT	= 0;
				if(i == 0)
				{
					dHMinusT += (pBri->m_dBTHS + pBri->m_dBHS);
				}
				else if(i == nCountJijum-1)
				{
					dHMinusT += (pBri->m_dBTHE + pBri->m_dBHE);
				}
				else
				{
					dHMinusT = pBri->m_dTS;
					dHMinusT += pBri->m_dHHL[i-1][iUPPER];
				}

				double dHFooting	= 0;
				double dELB	= 0;
				CFootingApp *pFooting	= pBri->GetFootingByJijumNumber(i);
				if(pFooting)
				{
					dHFooting	= pFooting->GetHeight(TRUE);
					dELB	= pFooting->m_dEL - dHFooting;
				}

				if(i == 0)
				{
					dSta	= pBri->GetStationByTarget(BRIDGE_OFF_STT_SLAB, 0, TRUE);
					vAng	= pBri->GetAngleByTarget(BRIDGE_OFF_STT_SLAB, 0);
				}
				else if(i == nCountJijum-1)
				{
					dSta	= pBri->GetStationByTarget(BRIDGE_OFF_END_SLAB, 0, TRUE);
					vAng	= pBri->GetAngleByTarget(BRIDGE_OFF_END_SLAB, 0);
				}
				else
				{
					dSta	= pBri->GetStationOnJijum(i, 0);
					vAng	= pBri->GetAngleJijum(i);
				}

				dELMax	= pBri->GetElMaxSlabUpper(dSta, vAng);
				dELMin	= pBri->GetElMinSlabUpper(dSta, vAng);
				


				sText2.Format("- 지점%d 벽체 높이 : ((%s+%s)/2 - %s) - %s - %s = %s", i+1, COMMA(dELMax), COMMA(dELMin), COMMA(dELB), COMMA(dHMinusT), COMMA(dHFooting), COMMA((dELMax+dELMin)/2 - dELB - dHMinusT - dHFooting));
			}
			else
			{
				sText2.Format("- 지점%d 벽체 높이 : %s - (%s - (%s+%s)/2) = %s", i+1, COMMA(dH), COMMA(dEL), COMMA(dELMax), COMMA(dELMin), COMMA(dH - (dEL - (dELMax+dELMin)/2)));
			}
		}
		else if(pBri->m_nTypeHeightWall == 2)
		{
			sText2.Format("- 지점%d 벽체 높이 : %s - (%s - %s) = %s", i+1, COMMA(dH), COMMA(dEL), COMMA(dELMax), COMMA(dH - (dEL - dELMax)));
		}

		Dom.TextOut(xyText, sText2);
	}
	
	*pDom << Dom;
}

void COutBM::MakeBMFormGuardWall()
{
	CRcBridgeRebar *pBri = GetRcBridgeRebar();
	CRcBridgeRebar *pBriOrg = GetRcBridgeRebar(TRUE);
	if(!pBri) return;
	if(!pBriOrg) return;

	CLineInfo *pLine = pBri->GetLineBase();
	if(!pLine) return;

// 	CGuardWallRC* pGW = NULL;
	long nQtyHDan = pBri->GetQtyHDan();
	long i=0; for(i=0; i<nQtyHDan; i++)
	{
		CTypedPtrArray <CObArray, CBMData*> *pArrBMStd	= &m_pArrBMGuardWallStd[i];
		long	nSizeBM	= pArrBMStd->GetSize();
		double	dFirstMeter	= 7000;
		double	dSeconeMeter = 3000;
		
		CGuardWallRC* pGW = pBriOrg->GetGuardWallByHDan(i);
		if(!pGW) continue;

//		CString sType = _T("");
		CString sType = pBriOrg->GetGuardWallName(pGW);

		if( sType == _T("차도") ||  sType == _T("보도") || sType == _T("보차도 경계석")) 
			continue;

		if(pGW->IsForm())
		{
			double dLength = pBriOrg->GetGuardWallTotalLength(pGW);
			if(Compare(dLength, 0.0, "<=")) continue;

			CTwinVectorArray tvArr;
			CTwinVector tv;
			pGW->GetTvGuardWallBase(tvArr, TRUE, FALSE);
			double dLenOutLine	= 0;
			long j = 0; for(j = 0; j < tvArr.GetSize(); j++)
			{
				tv	= tvArr.GetAt(j);
				//if(tv.GetVerLength() == 0) continue;
				
				//{32073 GetVerLengh()는 double형의 분할오류를 잡을 수 없기 때문에 아래의 식으로 대신함
				double dLenH1 =0, dLenH2 = 0;
				dLenH1 = tv.m_v1.y;
				dLenH2 = tv.m_v2.y;

				if(tv.m_sInfo.Find(_T("Hole")) != -1)
				{
					// 내공은 하면하고 마크선 빼고
					if(tv.m_sInfo.Find(_T("Mark")) != -1)
						continue;
				}
				else
				{
					if(Compare(dLenH1, dLenH2, _T("="), 0.01))
						continue;
				}
				//}

				dLenOutLine += tv.GetLength();
			}

			if(dLenOutLine == 0) continue;

			CBMData *pBM				= new CBMData;
			pBM->m_nNumbering			= 1;
			pBM->m_nIdxBigClass			= BM_BIG_FORM;
			pBM->m_nLevelTotal			= GetLevelTotal(pBM->m_nIdxBigClass);
			pBM->m_sBigClass			= GetStringBigClass(pBM->m_nIdxBigClass);
			pBM->m_nIdxMidClass			= GetFormByPos(FORM_POS_WALL_FRONT);
			pBM->m_sMidClass			= GetStringMidClass(pBM->m_nIdxMidClass);
			pBM->m_nIdxSmallClass		= BM_SMALL_FORM_VERTICAL;
			pBM->m_sSmallClass			= GetStringSmallClass(pBM->m_nIdxSmallClass);
			pBM->m_nIdxItemClass		= BM_ITEM_CONCRETE_MAIN;
			pBM->m_sItemClass			= GetStringItemClass(pBM->m_nIdxItemClass);
			pBM->m_nTypeResult			= RESULT_AREA;
			pBM->m_sUnit				= "㎡";
			pBM->m_nShape				= SHAPE_RECTANGLE;
			pBM->m_dV[SttMeter]			= Round(toM(0),3);
			pBM->m_dV[EndMeter]			= Round(toM(7),3);
			pBM->m_dV[WidthLower]		= Round(toM(dLength),3);
			pBM->m_dV[Height]			= Round(toM(dLenOutLine),3);
			pBM->m_dwSide				= SIDE_FRONT;
			pBM->CalculateResult();
			pArrBMStd->Add(pBM);
		}

		// 수량에 미터정보를 추가한다.
		RemakeByHeightBMList(pArrBMStd, dFirstMeter, dSeconeMeter, nSizeBM, pArrBMStd->GetSize()-1);
	}
}


//31724 날개벽 비계 다리 수량 (날개벽 비계다리)
void COutBM::MakeBMScaffoldLegWing()
{
	CRcBridgeRebar *pBri	= GetRcBridgeRebar();
	if(!pBri) return;

	// 시종점 좌우측 날개벽
	CTwinVectorArray tvArrOut, tvArrCenOut, tvArrSideOut;
	CTwinVectorArray tvArrIn, tvArrCenIn, tvArrSideIn;

	CTwinVectorArray tvArrScaffoldSec;
	CString sNoteTail	= _T("");
	CString sStt		= _T("");
	CString sLeft		= _T("");

	long stt = 0; for(stt = 0; stt < 2; stt++)
	{
		BOOL bStt	= stt == iSTT ? TRUE : FALSE;
		sStt	= bStt ? "시점측" : "종점측";

		pBri->GetTvScaffoldSection(tvArrScaffoldSec, bStt);	// 높이 산정용
		for(long left = 0; left < 2; left++)
		{
			BOOL bLeft	= left == iLEFT ? TRUE : FALSE;
			if(!pBri->IsWingWall(bStt, bLeft)) continue;

			CTwinVectorArray tvArrWingWall;
			tvArrWingWall.RemoveAll();

			//단면도 제원을 가져와 높이를 구함
			pBri->GetTvWingWallSection(bStt, bLeft, tvArrWingWall);

			CDRect rectWingWall;
			rectWingWall = tvArrWingWall.GetRect();

			double dHeightWingWall	= fabs(tvArrScaffoldSec.GetRect().Height());
			double dHeightApply	= 2000;	
			double dHeightOffset	= 1500;

			long nDan = 0;	// 비계 단수
			double dCurHeight = dHeightApply;
			while(dCurHeight <= dHeightWingWall)
			{
				nDan++;
				dCurHeight += dHeightOffset;
			}

			sLeft	= bLeft ? "좌측" : "우측";

			CTypedPtrArray <CObArray, CBMData*> *pArrBMStd	= &m_pArrBMWingStd[stt][left];

			pBri->GetTvScaffoldWingPlane(tvArrOut, tvArrIn, tvArrCenOut, tvArrCenIn,  tvArrSideOut, tvArrSideIn, bStt, bLeft);
// 			double dCenLen = 0;
// 			double dCenIn = 0;
// 			double dResultCen= 0;
// 			double dResultIn =0;


			// 날개벽 외측
			sNoteTail	= sStt + sLeft + "날개벽 외측";
			for(long ix = 0; ix < tvArrCenOut.GetSize(); ix++)
			{	
				double dCenLen = Round(toM(tvArrCenOut.GetAt(ix).GetLength()),3);
				double dResultCen = (double) nDan * dCenLen;

				CBMData *pBM			= new CBMData;
				pBM->m_nLevelTotal		= 2;
				pBM->m_nIdxBigClass		= BM_BIG_SCAFFOLD_LEG;
				pBM->m_sBigClass		= GetStringBigClass(pBM->m_nIdxBigClass);
				pBM->m_nIdxMidClass		= BM_MID_SCAFFOLD_WING_LEG;
				pBM->m_sMidClass		= GetStringMidClass(pBM->m_nIdxMidClass);
				pBM->m_nIdxSmallClass	= BM_SMALL_SCAFFOLD_WING_LEG;
				pBM->m_sSmallClass		= GetStringSmallClass(pBM->m_nIdxSmallClass);
				pBM->m_nTypeResult		= RESULT_LENGTH;
				pBM->m_sUnit			= "m";
				pBM->m_sForm.Format(_T("%.3f x %d 단"),dCenLen, nDan ); //Height의 단수 
				//pBM->m_bIsPrintByRefForm	= TRUE;
				pBM->m_dV[WidthLower]	= dCenLen;
				pBM->m_dV[Height]		= (double) nDan;
				pBM->m_dwSide			= SIDE_FRONT;
				pBM->CalculateResult();
				pBM->SetResult(dResultCen);
				pArrBMStd->Add(pBM);	
			}

			// 날개벽 내측 
			sNoteTail	= sStt + sLeft + "날개벽 내측";
			for(long jx = 0; jx < tvArrCenIn.GetSize(); jx++)
			{
				double dCenIn = Round(toM(tvArrCenIn.GetAt(jx).GetLength()),3);
				double dResultIn = (double) nDan * dCenIn;

				CBMData *pBM2			= new CBMData;
				pBM2->m_nLevelTotal		= 2;
				pBM2->m_nIdxBigClass		= BM_BIG_SCAFFOLD_LEG;
				pBM2->m_sBigClass		= GetStringBigClass(pBM2->m_nIdxBigClass);
				pBM2->m_nIdxMidClass		= BM_MID_SCAFFOLD_WING_LEG;
				pBM2->m_sMidClass		= GetStringMidClass(pBM2->m_nIdxMidClass);
				pBM2->m_nIdxSmallClass	= BM_SMALL_SCAFFOLD_WING_LEG;
				pBM2->m_sSmallClass		= GetStringSmallClass(pBM2->m_nIdxSmallClass);
				pBM2->m_nTypeResult		= RESULT_LENGTH;
				pBM2->m_sUnit			= "m";
				pBM2->m_sForm.Format(_T("%.3f x %d 단"),dCenIn, nDan ); //Height의 단수 
				pBM2->m_dV[WidthLower]	= dCenIn;
				pBM2->m_dV[Height]		= (double) nDan;
				pBM2->m_dwSide			= SIDE_FRONT;
				pBM2->CalculateResult();
				pBM2->SetResult(dResultIn);
				pArrBMStd->Add(pBM2);	
			}
			
			if(pBri->m_nTypeScaffoldSideLocation == SCOFFOLD_WING_CON && tvArrSideIn.GetSize() > 0)
			{
				double dCenSide	= Round(toM(tvArrSideIn.GetAt(0).GetLength()),3);
				double dResultSide = (double) nDan * dCenSide;

				CBMData *pBM				= new CBMData;
				pBM->m_nLevelTotal		= 2;
				pBM->m_nIdxBigClass	= BM_BIG_SCAFFOLD_LEG;
				pBM->m_sBigClass			= GetStringBigClass(pBM->m_nIdxBigClass);
				pBM->m_nIdxMidClass	= BM_MID_SCAFFOLD_WING_LEG;
				pBM->m_sMidClass		= GetStringMidClass(pBM->m_nIdxMidClass);
				pBM->m_nIdxSmallClass	= BM_SMALL_SCAFFOLD_WING_LEG;
				pBM->m_sSmallClass		= GetStringSmallClass(pBM->m_nIdxSmallClass);
				pBM->m_nTypeResult		= RESULT_LENGTH;
				pBM->m_sUnit				= "m";
				pBM->m_sForm.Format(_T("%.3f x %d 단"),dCenSide, nDan ); //Height의 단수 
				pBM->m_dV[WidthLower]	= dCenSide;
				pBM->m_dV[Height]		= (double) nDan;
				pBM->m_dwSide			= SIDE_FRONT;
				pBM->CalculateResult();
				pBM->SetResult(dResultSide);
				pArrBMStd->Add(pBM);	
			}
		}
	}
}

//31724 본체 비계다리 수량
void COutBM::MakeBMScaffoldLegMain()
{
	CRcBridgeRebar *pBri	= GetRcBridgeRebar();
	if(!pBri) return;

	CDPoint vAng = pBri->GetAngleJijum(0);

	CTwinVectorArray tvArrScaffold, tvArrCenter;
	CTwinVectorArray tvArrScaffoldSec;


	long stt = 0; for(stt = 0; stt < 2; stt++)
	{
// 		double dWidth = 0;
// 		double dResult = 0;

		BOOL bStt	= stt == iSTT ? TRUE : FALSE;
		pBri->GetTvScaffoldPlane(tvArrScaffold, tvArrCenter, bStt);
		pBri->GetTvScaffoldSection(tvArrScaffoldSec, bStt);

		CDRect rt = tvArrScaffoldSec.GetRect();

		double dHeight	= fabs(tvArrScaffoldSec.GetRect().Height());		

		long i = 0; for(i = 0; i < tvArrCenter.GetSize(); i++)
		{
			double dWidth =  Round(toM(tvArrCenter.GetAt(i).GetLength()), 3);

			double dHeightApply	= 2000;	
			double dHeightOffset	= 1500;

			long nDan = 0;	// 비계 단수
			double dCurHeight = dHeightApply;
			while(dCurHeight <= dHeight)
			{
				nDan++;
				dCurHeight += dHeightOffset;
			}

			CBMData *pBM			= new CBMData;
			pBM->m_nLevelTotal		= 2;
			pBM->m_nIdxBigClass		= BM_BIG_SCAFFOLD_LEG;
			pBM->m_sBigClass		= GetStringBigClass(pBM->m_nIdxBigClass);
			pBM->m_nIdxMidClass		= BM_MID_SCAFFOLD_MAIN_LEG;
			pBM->m_sMidClass		= GetStringMidClass(pBM->m_nIdxMidClass);
			pBM->m_nIdxSmallClass	= BM_SMALL_SCAFFOLD_MAIN_LEG;
			pBM->m_sSmallClass		= GetStringSmallClass(pBM->m_nIdxSmallClass);
			pBM->m_nTypeResult		= RESULT_LENGTH;
			pBM->m_sUnit			= "m";

			//{
			pBM->m_sForm.Format(_T("%.3f x %d 단"),dWidth, nDan ); //Height의 단수 
			pBM->m_dV[Height]		= dWidth;
			pBM->m_dV[WidthLower]	= Round(toM(dHeight), 3);
 			pBM->m_dwSide			= SIDE_FRONT;
 			pBM->CalculateResult();

			// (ARCBRIGE-1573) 식은 Width x 단수인데 결과는 height x 단수네... 식에 맞춰서 수정.
			// dHeight = Round(toM(dHeight), 3);
			// dResult = dHeight * (double) nDan;
			double dResult = dWidth * (double) nDan;
 			pBM->SetResult(dResult);
// 			//}

 			m_pArrBMBridgeStd.Add(pBM);	
		}
	}
}

//잡석기초 (일반 수량) [함형라멘]
void COutBM::MakeRubbleBM()
{
	CRcBridgeRebar *pBri	= GetRcBridgeRebar();
	if(!pBri) return;

	CExFootingApp *pExFoot = &pBri->m_footingBox.m_exFooting;
	if(pExFoot == NULL) return;
	
	if(pBri->IsBoxType() == FALSE)									return;
	if(pBri->m_dThickCobbStone == 0)								return;
	if(pBri->m_nTypeLower != LOWERSLAB_TYPE_COBBSTONE)	return;

	CTwinVectorArray tvArrPlane, tvArrSec;
	CTwinVectorArray tvArrLeft, tvArrRight, tvArrStt, tvArrEnd;
	CTwinVector tv, tvL, tvR;
	CDPoint xy1, xy2;
	double dEleBottom = 0, dExtLeft =0, dExtRight = 0;

	//Section
	CTwinVectorArray *pTvArrFront = &pExFoot->m_tvArrFront;
	if(pTvArrFront && pTvArrFront->GetSize() > 0)
	{
		pTvArrFront->GetTvByInfo("확대기초상단좌측면", tvL);
		pTvArrFront->GetTvByInfo("확대기초상단우측면", tvR);
		dExtLeft		= tvL.GetHorLength(TRUE);
		dExtRight		= tvR.GetHorLength(TRUE);
	}

	tvR = pExFoot->m_tvArrFront.GetTvByInfo(_T("확대기초우측면"));
	tvL = pExFoot->m_tvArrFront.GetTvByInfo(_T("확대기초좌측면"));
	dEleBottom = min(tvR.m_v2.y, tvL.m_v1.y) - pBri->m_dThickCobbStone;
	xy1 = CDPoint(tvL.m_v1.x, dEleBottom);
	xy2 = CDPoint(tvR.m_v1.x, dEleBottom);

	tv.m_v1 = xy1;
	tv.m_v2 = xy2;
	tvArrSec.Add(tv);

	tv.m_v1 = xy1;
	tv.m_v2 = tvL.m_v1;
	tvArrSec.Add(tv);

	tv.m_v1 = tvR.m_v2;
	tv.m_v2 = xy2;
	tvArrSec.Add(tv);

	for(long ix = 0; ix < pExFoot->m_tvArrFront.GetSize(); ix++)
	{
		tv = pExFoot->m_tvArrFront.GetAt(ix);
		if(tv.m_sInfo.Find(_T("하면")) != -1)
		{
			tvArrSec.Add(tv);
		}
	}

	//Plan
	pBri->GetTvRebarOut_Plane_UpperSlabStd(tvArrPlane, -1, TRUE);
	for(long ix = 0; ix < tvArrPlane.GetSize(); ix++)
	{
		tv	= tvArrPlane.GetAt(ix);
		if(tv.m_sInfo.Find("좌측", 0) != -1) tvArrLeft.Add(tv);
		else if(tv.m_sInfo.Find("우측", 0) != -1) tvArrRight.Add(tv);
		else if(tv.m_sInfo.Find("시점", 0) != -1) tvArrStt.Add(tv);
		else if(tv.m_sInfo.Find("종점", 0) != -1) tvArrEnd.Add(tv);
	}

	//Value
	double dLength_L  = tvArrLeft.GetLength() + dExtLeft + dExtRight;
	double dLenght_R  = tvArrRight.GetLength() + dExtLeft + dExtRight;
	double dWidth		= tvArrStt.GetLength() + dExtLeft + dExtRight;
	double dHeight		= pBri->m_dThickCobbStone;
	double dResult		= (toM(dLength_L) + toM(dLenght_R))/2 * toM(dWidth) * toM(dHeight);
	double dLength		= (dLength_L + dLenght_R)/2;

	if(dLength_L ==0 || dLenght_R ==0 || dWidth == 0 || dHeight == 0)
		return;

	CString sForm = _T("");
	if(Compare(dLength_L, dLenght_R, _T("="), 0.1))
		sForm.Format(_T("%.3f x %.3f  x %.3f"), toM(dLength_L), toM(dHeight), toM(dWidth));
	else
		sForm.Format(_T("(%.3f + %.3f) / 2 x %.3f  x %.3f"), toM(dLength_L), toM(dLenght_R), toM(dHeight), toM(dWidth));


	//
	CBMData *pBM				= new CBMData;
	pBM->m_nLevelTotal		= 2;
	pBM->m_nIdxBigClass		= BM_BIG_RUBBLEFILL;
	pBM->m_sBigClass			= GetStringBigClass(pBM->m_nIdxBigClass);
	pBM->m_nIdxMidClass		= BM_MID_RUBBLEFILL;
	pBM->m_sMidClass			= GetStringMidClass(pBM->m_nIdxMidClass);
	pBM->m_nIdxSmallClass	= BM_SMALL_RUBBLEFILL;
	pBM->m_sSmallClass		= GetStringSmallClass(pBM->m_nIdxSmallClass);
	pBM->m_nTypeResult		= RESULT_VOLUMN;
	pBM->m_sUnit				= "㎥";
 	pBM->m_sForm				= sForm;
	pBM->m_dV[Width]				= Round(toM(dWidth),3);
	pBM->m_dV[Height]			= Round(toM(dHeight),3);
	pBM->m_dV[Length]			= Round(toM(dLength), 3);
	pBM->CalculateResult();
	pBM->SetResult(dResult);
	
	m_pArrBMBridgeStd.Add(pBM);	
}

CBMData *COutBM::CreateBMStyrotormForSide(CBMData *pBMForm)
{
	if(!pBMForm) 
		return NULL;

	CBMData *pBM	= new CBMData;
	*pBM	= *pBMForm;


	pBM->m_nIdxBigClass		= BM_BIG_STYROFOAM;
	pBM->m_sBigClass		= GetStringBigClass(pBM->m_nIdxBigClass);
	pBM->m_nLevelTotal		= GetLevelTotal(pBM->m_nIdxBigClass);
	pBM->m_nIdxMidClass		= -1;
	pBM->m_sMidClass		= _T("");
	pBM->m_nIdxSmallClass	= BM_SMALL_STYROFOAM;
	pBM->m_sSmallClass.Format(_T("T=%.0fMM"), 10.0);
	// 예외적인 출력양식
	pBM->m_nLevelMidClass		= 2;
	if (pBM->m_sInfo.Find("버림콘크리트거푸집") < 0 && pBM->m_sInfo.Find("MASS콘크리트거푸집") < 0)
	{
		pBM->m_sInfo			= _T("상하행공제");
	}
	else
	{
		pBM->m_sInfo = _T("");
	}

	return pBM;
}

void COutBM::AddBMStyroformBySide(CBMData *pBMForm, DWORD nSide)
{
	CBMData *pBM = CreateBMStyrotormForSide(pBMForm);
	if(!pBM)
		return;

	pBM->m_dwSide			= nSide;	
	pBM->CalculateResult();

	m_pArrBMBridgeStd.Add(pBM);
}
