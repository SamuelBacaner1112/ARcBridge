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

	// �� ��¿� ���� �ɼǵ� /////////////////////////////////////////
	m_dScaleDim_VertSection		= 100;	// ���ܸ鵵 ������
	m_dHeightText_VertSection	= 7;	// ���ܸ鵵 �ؽ�Ʈ ����
	////////////////////////////////////////////////////////////////////
}

COutBM::~COutBM()
{
	ClearBM();

	AhTPADelete(&m_pArrAreaConc,	(CTwinVectorAreaShape*)0);
}

// ������ ��� ������.
// �����ѵ� ������ �ٷ� ��.
void COutBM::CalculateBM(BOOL bNewCreateBM)
{
	ClearBM();
//	m_pStd->CheckCalBM();

	// (ARCBRIDGE-1477) ������ ���õǾ� ���� �ʾƵ� �𵵴� �׸��� ��찡 �ִ�. ���� �����ʹ� ������ְ� return����.
	// ���� ����
	CRcBridgeRebar *pBriOrg	 = m_pStd->m_pARcBridgeDataStd->GetBridgeRebar(m_nBri);
	m_pStd->m_pARcBridgeDataStd->MakeRcBridgeForBM(pBriOrg, m_pStd->GetBridge(m_nBri));

	// ��������� ��� ���ǿ� ���� ������ �ȵ� ���� ����.
	if(m_dwCalBM == 0x00000000)
		return;

	long stt(0), left(0);

	if(m_dwCalBM & NORMAL_BM)
	{
		MakeBMBohoBlock();			// ��ȣ��
		MakeBMConcreteMain();		// ��ũ��Ʈ(��ü, ����)
		MakeBMConcreteGuardWall();	// ��ũ��Ʈ(��ȣ��)
		MakeBMConcreteMonument();	// ��ũ��Ʈ(������)
		MakeBMConcreteWing();		// ��ũ��Ʈ(������)
		MakeBMConcreteAps();		// ��ũ��Ʈ(���ӽ�����)
		MakeBMConcBohoFooting();	// ��ũ��Ʈ(��ȣ�� ����)
		MakeBMConcBohoBackFill();	// ��ũ��Ʈ(��ȣ�� ��ä��)
		MakeBMFormMain();			// ��Ǫ��(��ü, ����)
		MakeBMFormGuardWall();		// ��Ǫ��(��ȣ��)
		MakeBMFormMonument();		// ��Ǫ��(������)
		MakeBMFormWing();			// ��Ǫ��(������)
		MakeBMStyrofoamAps();		// ��Ƽ����(���ӽ�����)
		MakeBMFormAps();			// ��Ǫ��(���ӽ�����)
		MakeBMFormBoho();			// ��Ǫ��(��ȣ��)
		MakeBMDowelBar();			// DowelBar(���ӽ�����)
	}

	if(m_dwCalBM & COMMON_BM)
	{
		MakeBMPostMain();			// ���ٸ�(��ü)
		MakeBMDeckFinisher(); //��ũ�Ǵϼ�
		MakeBMPostWing();			// ���ٸ�(������)
		MakeBMHoriConnectMain();	// ���ٸ����򿬰���(��ü)
		MakeBMHoriConnectWing();	// ���ٸ����򿬰���(������)
	
		MakeBMScaffoldMain();		// ���(��ü)
		MakeBMScaffoldWing();		// ���(������)
		
		MakeBMScaffoldLegMain(); //���ٸ�(��ü)
		MakeBMScaffoldLegWing(); //���ٸ�(������)
		MakeRubbleBM();				//�⼮ ����
	}

	if(m_dwCalBM & NORMAL_BM)
	{
		MakeBMSpacerMain();			// �����̼�(��ü)
		MakeBMSpacerWing();			// �����̼�(������)
		MakeBMSpacerAps();			// �����̼�(���ӽ�����)
		MakeBMSlabCare();			// ��������(��ü����)
		MakeBMConCareMain();		// ��ũ��Ʈ���(��ü)
		MakeBMConCareWing();		// ��ũ��Ʈ���(������)
		MakeBMConCareAps();			// ��ũ��Ʈ���(���ӽ�����)
		MakeBMRebar(0);				// ö�ٰ�������(��ü)
		MakeBMRebar(1);				// ö�ٰ�������(������)
		MakeBMRebar(2);				// ö�ٰ�������(���ӽ�����)
		MakeBMRebar(3);				// ö�ٰ�������(��ȣ��)
		MakeBMRebar(4);				// ö�ٰ�������(������)
		MakeBMSurface();			// �����(��ü����)
		MakeBMAscon();				// �ƽ��� ����(��ü����)
		MakeBMTackCotting();		// �� ����(��ü����)
		MakeBMSurfaceProof();		// ������(��ü����)
		MakeBMAsphaltProof();		// �ƽ���Ʈ���(��ü+������)
		MakeBMPile();				// ���Ҽ���
		MakeBMNamePanel();			// ������
		MakeBMDesPanel();			// ������
		MakeBMTBM();				// TBM��ġ
		MakeBMFlexibleJoint();		// ��������
		MakeBMFlexibleJointCover();	// ��������Ŀ��
		MakeBMNonShrinkageConc();	// ������ ��ũ��Ʈ
		MakeBMCoverPlate();			// cover plate
		MakeBMMonument();			// ������
		MakeBMNotch();				// NOTCH;
	}
	
	if(m_pStd->m_bCalRebarBM)
	{
		MakeBMRebar();
		MakeBMRebarTable();
	}
	for(stt = 0; stt < 2; stt++)
	{
		// ������ ���� ViewBM�� ���
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

		// ����ѵ� ������ ������ ViewBM�ɼ��� ���� ��Ų��.
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

// �׳� ������ ������ ���ؼ� �ʿ信 ���� �̸����� ������ �ش�.
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

	// ���ʴ�� ����. /////////////////////
	CHgBaseBMStd baseBM;
	DWORD exceptionFlag	= 0x00000000;

	for(stt = 0; stt < 2; stt++)
	{
		// ���� ���� ���� ������ ������.(����ٰ� ��¿�) - ���������� ������ ���ؼ��� ������.
		if(stt == 0)
		{
			// ��ü
			baseBM.MergeBMList(&m_pArrBMBridge, &m_pArrBMBridgeStd, FALSE, exceptionFlag);
			if(m_dwCalBM & NORMAL_BM)
			{
				// ��ȣ��
				long i=0; for(i=0; i<nQtyHDan; i++)
				{
					CGuardWallRC* pGW = pBriOrg->GetGuardWallByHDan(i);
					if(!pGW) continue;
					if(pGW->IsTypeHDanRoadTotalGuardWall())				
						baseBM.MergeBMList(&m_pArrBMGuardWall[i], &m_pArrBMGuardWallStd[i], FALSE, exceptionFlag);
				}
				// ������
				baseBM.MergeBMList(&m_pArrBMMonument, &m_pArrBMMonumentStd, FALSE, exceptionFlag);
			}
		}
	}

	//������
	for(stt = 0; stt < 2; stt++)
	{
		for(left = 0; left < 2; left++)
		{
			if(m_pArrBMWingStd[stt][left].GetSize() == 0) continue;
			baseBM.MergeBMList(&m_pArrBMWing[stt][left], &m_pArrBMWingStd[stt][left], FALSE, exceptionFlag);
		}
	}
	//���ӽ�����
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
	//���潽����
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
	//��ȣ��
	for(stt = 0; stt < 2; stt++)
	{
		if(m_dwCalBM & NORMAL_BM)
			baseBM.MergeBMList(&m_pArrBMBoho[stt], &m_pArrBMBohoStd[stt], FALSE, exceptionFlag);		
	}


	// ������ ����ǥ��(��ü����,����������,���ӽ���������,���潽��������) - �Ѱ賻�� ������ ���� ������
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
	//������
	for(stt = 0; stt < 2; stt++)
	{
		for(left = 0; left < 2; left++)
		{
			if(m_pArrBMWing[stt][left].GetSize() == 0) continue;
			baseBM.MergeBMList(&m_pArrBMWingForPartSum[stt][left], &m_pArrBMWing[stt][left], TRUE, exceptionFlag);
		}
	}
	//���ӽ�����
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
	//���潽����
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
	//��ȣ��
	for(stt = 0; stt < 2; stt++)
	{
		if(m_dwCalBM & NORMAL_BM)
			baseBM.MergeBMList(&m_pArrBMBohoForPartSum[stt], &m_pArrBMBoho[stt], TRUE, exceptionFlag);
	}

	// �Ѱ�����ǥ��
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

	//������
	for(stt = 0; stt < 2; stt++)
	{
		for(left = 0; left < 2; left++)
		{
			if(m_pArrBMWing[stt][left].GetSize() == 0) continue;
			baseBM.MergeBMList(&m_pArrBMWingSum, &m_pArrBMWing[stt][left], TRUE, exceptionFlag);
		}
	}
	//���ӽ�����
	for(stt = 0; stt < 2; stt++)
	{
		for(left = 0; left < 2; left++)
		{
			if(m_dwCalBM & NORMAL_BM)	// ���ӽ�����, ���潽����� ������ ���� ���⿡���� ���Ե�
			{
				if(m_pArrBMAps[stt][left].GetSize() == 0) continue;
				baseBM.MergeBMList(&m_pArrBMApsSum, &m_pArrBMAps[stt][left], TRUE, exceptionFlag);
			}
		}
	}
	//���潽����
	for(stt = 0; stt < 2; stt++)
	{
		for(left = 0; left < 2; left++)
		{
			if(m_dwCalBM & NORMAL_BM)	// ���ӽ�����, ���潽����� ������ ���� ���⿡���� ���Ե�
			{
				long i = 0; for(i = 0; i < 2; i++)
				{
					if(m_pArrBMCns[stt][left][i].GetSize() == 0) continue;
					baseBM.MergeBMList(&m_pArrBMCnsSum, &m_pArrBMCns[stt][left][i], TRUE, exceptionFlag);
				}
			}
		}
	}
	//��ȣ��
	for(stt = 0; stt < 2; stt++)
	{
		if(m_dwCalBM & NORMAL_BM)	// ��ȣ���� ������ ���� ���⿡���� ���Ե�
			baseBM.MergeBMList(&m_pArrBMBohoSum, &m_pArrBMBoho[stt], TRUE, exceptionFlag);
	}

	// ���� ����ǥ��
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

	// ���� �Ѱ� ����ǥ���� ������ ��ü�� ��� �׸��� �־�� �ȴ�.
	// �׸� ���������� �ϳ� ���� (�� ������ ��ü�� �����ϰ� ��� 0���� ��)
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


	// ö�������
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

// ���� ��¿� ���ڿ� ����
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

	// ������ ����ٰſ� ����
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
	//������
	for(stt = 0; stt < 2; stt++)
	{
		for(left = 0; left < 2; left++)
		{
			if(m_pArrBMWing[stt][left].GetSize() == 0) continue;
			baseBM.MakeBMString(&m_pArrBMStringWing[stt][left], &m_pArrBMWing[stt][left], TRUE);
		}
	}
	//���ӽ�����
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
	//���潽����
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
	//��ȣ��
	for(stt = 0; stt < 2; stt++)
	{
		if(m_dwCalBM & NORMAL_BM)
			baseBM.MakeBMString(&m_pArrBMStringBoho[stt], &m_pArrBMBoho[stt], TRUE);
	}

	// �� ������ ����ǥ(��ü����, ����������, ���ӽ���������, ���潽��������, ��ȣ������)
	baseBM.MakeBMSumString(&m_pArrBMStringBridgeSum, &m_pArrBMBridgeSum, TRUE, TRUE);	// ��ü
	if(m_dwCalBM & NORMAL_BM)
	{
		long i=0; for(i=0; i<nQtyHDan; i++)
		{
			CGuardWallRC* pGW = pBriOrg->GetGuardWallByHDan(i);
			if(!pGW) continue;
			if(pGW->IsTypeHDanRoadTotalGuardWall())	
				baseBM.MakeBMSumString(&m_pArrBMStringGuardWallSum, &m_pArrBMGuardWallForPartSum[i], TRUE, FALSE);	// ��ȣ��
		}
		if(m_pArrBMMonumentPartSum.GetSize() > 0)
			baseBM.MakeBMSumString(&m_pArrBMStringGuardWallSum, &m_pArrBMMonumentPartSum, TRUE, FALSE);	// ������
	}
	//������
	for(stt = 0; stt < 2; stt++)
	{
		for(left = 0; left < 2; left++)
		{
			if(m_pArrBMWingForPartSum[stt][left].GetSize() == 0) continue;
			baseBM.MakeBMSumString(&m_pArrBMStringWingSum, &m_pArrBMWingForPartSum[stt][left], TRUE, stt==0&&left==0);			// ������
		}
	}
	//���ӽ�����
	for(stt = 0; stt < 2; stt++)
	{
		for(left = 0; left < 2; left++)
		{
			if(m_dwCalBM & NORMAL_BM)
			{
				if(m_pArrBMApsForPartSum[stt][left].GetSize() == 0) continue;
				baseBM.MakeBMSumString(&m_pArrBMStringApsSum, &m_pArrBMApsForPartSum[stt][left], TRUE, stt==0&&left==0);				// ���ӽ�����
			}
		}
	}
	//���潽����
	for(stt = 0; stt < 2; stt++)
	{
		for(left = 0; left < 2; left++)
		{
			if(m_dwCalBM & NORMAL_BM)
			{
				long i = 0; for(i = 0; i < 2; i++)
				{
					if(m_pArrBMCnsForPartSum[stt][left][i].GetSize() == 0) continue;
					baseBM.MakeBMSumString(&m_pArrBMStringCnsSum, &m_pArrBMCnsForPartSum[stt][left][i], TRUE, stt==0&&left==0&&i==0);	// ���潽����
				}
			}
		}
	}
	//��ȣ��
	for(stt = 0; stt < 2; stt++)
	{
		if(m_dwCalBM & NORMAL_BM)
			baseBM.MakeBMSumString(&m_pArrBMStringBohoSum, &m_pArrBMBohoForPartSum[stt], TRUE, stt==0&&left==0);				// ��ȣ��
	}

	// �� ������ ����ǥ�� �� �߰�.
	baseBM.MakeBMSumString(&m_pArrBMStringBridgeSum, &m_pArrBMBridgeSum, TRUE, FALSE);			// ��ü
	if(m_dwCalBM & NORMAL_BM)
		baseBM.MakeBMSumString(&m_pArrBMStringGuardWallSum, &m_pArrBMGuardWallSum, TRUE, FALSE);// ��ȣ��
	baseBM.MakeBMSumString(&m_pArrBMStringWingSum, &m_pArrBMWingSum, TRUE, FALSE);				// ������
	if(m_dwCalBM & NORMAL_BM)
	{
		baseBM.MakeBMSumString(&m_pArrBMStringApsSum, &m_pArrBMApsSum, TRUE, FALSE);			// ���ӽ�����
		baseBM.MakeBMSumString(&m_pArrBMStringCnsSum, &m_pArrBMCnsSum, TRUE, FALSE);			// ���潽����
		baseBM.MakeBMSumString(&m_pArrBMStringBohoSum, &m_pArrBMBohoSum, TRUE, FALSE);			// ��ȣ��
	}

	// ��������ǥ��(��ü+��ȣ��+������+���ӽ�����+���潽����+��ȣ��)
	baseBM.MakeBMSumString(&m_pArrBMStringAllSum, &m_pArrBMSumForTitle, TRUE, TRUE);				// ��ü
	if(m_dwCalBM & NORMAL_BM)
	{
		if(m_pArrBMGuardWallSum.GetCount() > 0)
			baseBM.MakeBMSumString(&m_pArrBMStringAllSum, &m_pArrBMGuardWallSum, TRUE, FALSE);		// ��ȣ��
	}
	if(m_pArrBMWingSum.GetCount() > 0)
		baseBM.MakeBMSumString(&m_pArrBMStringAllSum, &m_pArrBMWingSum, TRUE, FALSE);				// ������
	if(m_dwCalBM & NORMAL_BM)
	{
		if(m_pArrBMApsSum.GetCount() > 0)
			baseBM.MakeBMSumString(&m_pArrBMStringAllSum, &m_pArrBMApsSum, TRUE, FALSE);			// ���ӽ�����
		if(m_pArrBMCnsSum.GetCount() > 0)
			baseBM.MakeBMSumString(&m_pArrBMStringAllSum, &m_pArrBMCnsSum, TRUE, FALSE);			// ���潽����
		if(m_pArrBMBohoSum.GetCount() > 0)
			baseBM.MakeBMSumString(&m_pArrBMStringAllSum, &m_pArrBMBohoSum, TRUE, FALSE);			// ��ȣ��
	}
	baseBM.MakeBMSumString(&m_pArrBMStringAllSum, &m_pArrBMSum, TRUE, FALSE);					// ��

	// ö�������
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

// ���� ����
void COutBM::ClearBM(BOOL bRemainStdBM)
{
	long stt(0), left(0);

	if(bRemainStdBM == FALSE)
	{
		// ������(�����ȵ� ���� ����)
		AhTPADelete(&m_pArrBMBridgeStd,	(CBMData*)0);					// ���뺻ü(���� �ȵȰ�)

		long i=0; 
		for(i=0; i<nMaxQtyHdan; i++)
			AhTPADelete(&m_pArrBMGuardWallStd[i],	(CBMData*)0);		// ��ȣ��(���� �ȵȰ�)

		AhTPADelete(&m_pArrBMMonumentStd,	(CBMData*)0);		//  ������(���� �ȵȰ�)
	
		for(stt = 0; stt < 2; stt++)
		{
			for(left = 0; left < 2; left++)
			{
				AhTPADelete(&m_pArrBMWingStd[stt][left],		(CBMData*)0);		// ���볯����(���� �ȵȰ�)
				AhTPADelete(&m_pArrBMApsStd[stt][left],			(CBMData*)0);		// ���ӽ�����(���� �ȵȰ�)
				long i = 0; for(i = 0; i < 2; i++)
					AhTPADelete(&m_pArrBMCnsStd[stt][left][i],	(CBMData*)0);		// ���潽����(���� �ȵȰ�)
				AhTPADelete(&m_pArrBMBohoStd[stt],				(CBMData*)0);		// ��ȣ��(���� �ȵȰ�)
			}
		}
	}

	// ��¿�(������ ����)
	AhTPADelete(&m_pArrBMBridge, (CBMData*)0);							// ���뺻ü(���� �ȵȰ�) - ����ٰ� ���
	AhTPADelete(&m_pArrBMStringBridge, (CBMSumString*)0);				// ���뺻ü(���� �ȵȰ�) - ����ٰ� ���

	AhTPADelete(&m_pArrBMMonument, (CBMData*)0);						// ������
	AhTPADelete(&m_pArrBMStringMonument, (CBMSumString*)0);				// ������

	long i=0; 
	for(i=0; i<nMaxQtyHdan; i++)
	{
		AhTPADelete(&m_pArrBMGuardWall[i], (CBMData*)0);				// ��ȣ��(���� �ȵȰ�) - ����ٰ� ���
		AhTPADelete(&m_pArrBMStringGuardWall[i], (CBMSumString*)0);		// ��ȣ��(���� �ȵȰ�) - ����ٰ� ���
	}

	for(stt = 0; stt < 2; stt++)
	{
		for(left = 0; left < 2; left++)
		{
			AhTPADelete(&m_pArrBMWing[stt][left],				(CBMData*)0);			// ���볯����(���� �ȵȰ�) - ����ٰ� ���
			AhTPADelete(&m_pArrBMStringWing[stt][left],			(CBMSumString*)0);		// ���볯����(���� �ȵȰ�) - ����ٰ� ���
			AhTPADelete(&m_pArrBMAps[stt][left],				(CBMData*)0);			// ���ӽ�����(���� �ȵȰ�) - ����ٰ� ���
			AhTPADelete(&m_pArrBMStringAps[stt][left],			(CBMSumString*)0);		// ���ӽ�����(���� �ȵȰ�) - ����ٰ� ���
			long i = 0; for(i = 0; i < 2; i++)
			{
				AhTPADelete(&m_pArrBMCns[stt][left][i],			(CBMData*)0);			// ���潽����(���� �ȵȰ�) - ����ٰ� ���
				AhTPADelete(&m_pArrBMStringCns[stt][left][i],	(CBMSumString*)0);		// ���潽����(���� �ȵȰ�) - ����ٰ� ���
			}
		}
		AhTPADelete(&m_pArrBMBoho[stt],							(CBMData*)0);			// ��ȣ��(���� �ȵȰ�) - ����ٰ� ���
		AhTPADelete(&m_pArrBMStringBoho[stt],					(CBMSumString*)0);		// ��ȣ��(���� �ȵȰ�) - ����ٰ� ���
	}

	// ����������ǥ�� ��µ� ����
	AhTPADelete(&m_pArrBMBridgeForPartSum, (CBMData*)0);				// ���뺻ü(���� �Ȱ�) - ����ٰ� ���
	AhTPADelete(&m_pArrBMMonumentPartSum, (CBMData*)0);					// ������
	
	for(i=0; i<nMaxQtyHdan; i++)
		AhTPADelete(&m_pArrBMGuardWallForPartSum[i], (CBMData*)0);		// ��ȣ��(���� �ȵȰ�)
	
	for(stt = 0; stt < 2; stt++)
	{
		for(left = 0; left < 2; left++)
		{
			AhTPADelete(&m_pArrBMWingForPartSum[stt][left],		(CBMData*)0);			// ���볯����(���� �Ȱ�) - ����ٰ� ���
			AhTPADelete(&m_pArrBMApsForPartSum[stt][left],		(CBMData*)0);			// ���ӽ�����(���� �Ȱ�) - ����ٰ� ���
			long i = 0; for(i = 0; i < 2; i++)
			{
				AhTPADelete(&m_pArrBMCnsForPartSum[stt][left][i],	(CBMData*)0);		// ���潽����(���� �Ȱ�) - ����ٰ� ���
			}
		}
		AhTPADelete(&m_pArrBMBohoForPartSum[stt],			(CBMData*)0);				// ��ȣ��(���� �Ȱ�) - ����ٰ� ���
	}

	AhTPADelete(&m_pArrBMBridgeSum,				(CBMData*)0);			// ���뺻ü - ������ ����ǥ ��� 
	AhTPADelete(&m_pArrBMStringBridgeSum,		(CBMSumString*)0);

	AhTPADelete(&m_pArrBMGuardWallSum,			(CBMData*)0);			// ��ȣ�� - ������ ����ǥ ��� 
	AhTPADelete(&m_pArrBMStringGuardWallSum,	(CBMSumString*)0);

	AhTPADelete(&m_pArrBMWingSum,				(CBMData*)0);			// ���볯���� - ������ ����ǥ ���
	AhTPADelete(&m_pArrBMStringWingSum,			(CBMSumString*)0);

	AhTPADelete(&m_pArrBMApsSum,				(CBMData*)0);			// ���ӽ����� - ������ ����ǥ ���
	AhTPADelete(&m_pArrBMStringApsSum,			(CBMSumString*)0);

	AhTPADelete(&m_pArrBMCnsSum,				(CBMData*)0);			// ���潽���� - ������ ����ǥ ���
	AhTPADelete(&m_pArrBMStringCnsSum,			(CBMSumString*)0);

	AhTPADelete(&m_pArrBMBohoSum,				(CBMData*)0);			// ��ȣ�� - ������ ����ǥ ���
	AhTPADelete(&m_pArrBMStringBohoSum,			(CBMSumString*)0);

	AhTPADelete(&m_pArrBMStringAllSum,			(CBMSumString*)0);		// ���뺻ü(��ü+������+���ӽ�����) - ������ ����ǥ ��� 

	AhTPADelete(&m_pArrBMSum,					(CBMData*)0);			// ��������(���� �ȵȰ�) - ������ ����ǥ ���
	AhTPADelete(&m_pArrBMSumForTitle,					(CBMData*)0);			// ��������(���� �ȵȰ�) - ������ ����ǥ ���

	AhTPADelete(&m_pArrBMStringSepBridgeSum,			(CBMSumString*)0);
	AhTPADelete(&m_pArrBMSepBridgeSum,					(CBMData*)0);

	for(long bar = 0; bar < 2; bar++)
	{
		AhTPADelete(&m_pArrRebarBMBridgeStd[bar],	(CRebarBMData*)0);				// ���뺻ü(���� �ȵȰ�)
		
		long i=0; for(i=0; i<nMaxQtyHdan; i++)
			AhTPADelete(&m_pArrRebarBMGuardWallStd[i][bar],	(CRebarBMData*)0);		// ��ȣ��(���� �ȵȰ�)

		for(i=0; i<nMaxQtyHyunta; i++)
			AhTPADelete(&m_pArrRebarBMHyuntaStd[i][bar],	(CRebarBMData*)0);		// ����Ÿ��(���� �ȵȰ�)

		AhTPADelete(&m_pArrRebarBMMonumentStd[bar],	(CRebarBMData*)0);				// ������(���� �ȵȰ�)

		for(stt = 0; stt < 2; stt++)
		{
			for(left = 0; left < 2; left++)
			{
				AhTPADelete(&m_pArrRebarBMWingStd[stt][left][bar],	(CRebarBMData*)0);			// ���볯����(���� �ȵȰ�)
				AhTPADelete(&m_pArrRebarBMApsStd[stt][left][bar],	(CRebarBMData*)0);			// ���ӽ�����(���� �ȵȰ�)
				long i = 0; for(i = 0; i < 2; i++)
					AhTPADelete(&m_pArrRebarBMCnsStd[stt][left][i][bar],	(CRebarBMData*)0);	// ���潽����(���� �ȵȰ�)
			}
			AhTPADelete(&m_pArrRebarBMBohoStd[stt][bar],	(CRebarBMData*)0);					// ��ȣ��(���� �ȵȰ�)
		}

		AhTPADelete(&m_pArrRebarBMBridge[bar],	(CRebarBMData*)0);					// ���뺻ü(���� �Ȱ�)
		
		for(i=0; i<nMaxQtyHdan; i++)
			AhTPADelete(&m_pArrRebarBMGuardWall[i][bar],	(CRebarBMData*)0);		// ��ȣ��(���� �Ȱ�)

		for(i=0; i<nMaxQtyHyunta; i++)
			AhTPADelete(&m_pArrRebarBMHyunta[i][bar],	(CRebarBMData*)0);		// ����Ÿ��

		AhTPADelete(&m_pArrRebarBMMonument[bar],	(CRebarBMData*)0);				// ������(���� �Ȱ�)

		for(stt = 0; stt < 2; stt++)
		{
			for(left = 0; left < 2; left++)
			{
				AhTPADelete(&m_pArrRebarBMWing[stt][left][bar],	(CRebarBMData*)0);			// ���볯����(���� �Ȱ�)
				AhTPADelete(&m_pArrRebarBMAps[stt][left][bar],	(CRebarBMData*)0);			// ���ӽ�����(���� �Ȱ�)
				long i = 0; for(i = 0; i < 2; i++)
					AhTPADelete(&m_pArrRebarBMCns[stt][left][i][bar],	(CRebarBMData*)0);	// ���潽����(���� �Ȱ�)
			}
			AhTPADelete(&m_pArrRebarBMBoho[stt][bar],	(CRebarBMData*)0);		// ��ȣ��(���� �Ȱ�)
		}

		AhTPADelete(&m_pArrRebarBMStringBridge[bar],	(CRebarBMString*)0);			// ���뺻ü
		
		for(i=0; i<nMaxQtyHdan; i++)
			AhTPADelete(&m_pArrRebarBMStringGuardWall[i][bar],	(CRebarBMString*)0);	// ��ȣ��
		
		for(i=0; i<nMaxQtyHyunta; i++)
			AhTPADelete(&m_pArrRebarBMStringHyunta[i][bar],	(CRebarBMData*)0);		// ����Ÿ��

		AhTPADelete(&m_pArrRebarBMStringMonument[bar],	(CRebarBMString*)0);	// ������
		
		for(stt = 0; stt < 2; stt++)
		{
			for(left = 0; left < 2; left++)
			{
				AhTPADelete(&m_pArrRebarBMStringWing[stt][left][bar],	(CRebarBMString*)0);		// ���볯����
				AhTPADelete(&m_pArrRebarBMStringAps[stt][left][bar],	(CRebarBMString*)0);		// ���ӽ�����
				long i = 0; for(i = 0; i < 2; i++)
					AhTPADelete(&m_pArrRebarBMStringCns[stt][left][i][bar],	(CRebarBMString*)0);	// ���潽����
			}
			AhTPADelete(&m_pArrRebarBMStringBoho[stt][bar],	(CRebarBMString*)0);		// ��ȣ��
		}
	}
}

// ������ ViewBM�� �����.
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
			if(bNewCreateBM) //���� �������� ���� ��쿡�� ���� �ɼ� ����Ʈ ���·� �����
			{
				switch(nBigIndex)
				{
				case BM_BIG_DECKFINISH: //���� ����� ���� ��쿡�� �Ⱥ���
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

// ������ ViewBM�� ������Ѽ� �ʿ� ���°Ŵ� ������.
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

// ������ ������������ ��������� ���� �ɼ� ����
// ��, ���Ǽ����� ��� BMData�� �ɼ��� �켱���� ����
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

// ������ �ɼ��� ������.
void COutBM::ApplyOptionBMStd(CTypedPtrArray <CObArray, CBMData*> *pArrBM)
{
	ApplyViewBM(pArrBM);
	ApplyRefCellBM(pArrBM);
}

// ��ũ��Ʈ ����
// BOOL bForDeductOfReFill : �Ǹ޿�� ������ ���ݼ� ������ ��ü���� ������ ���� ����
// �������� ��� ������ ���ѵ� ���ݼ��� �������� ��� �߶󳽴�.
void COutBM::MakeBMConcreteMain(BOOL bForDeductOfReFill)
{
	CRcBridgeRebar *pBri	= GetRcBridgeRebar();
	if(!pBri) return;
	CLineInfo *pLine	= pBri->GetLineBase();
	if(!pLine) return;
	CRcBridgeRebar *pBriOrg	= GetRcBridgeRebar(TRUE);
	if(!pBriOrg) return;

	// ���� ����.
	CalAreaConc(pBri, bForDeductOfReFill);

	// (ARCBRIDGE-16530) ���������� ���� ������ ����, ������, ���ӽ����� ������ �׸���'�ٴ� �� ��'�׸����� ���̴µ� �̸��� �ٲ�� Index�� ���� �ٸ���.
	// �Ѱ� ����� Merge�� ���� �ʰ� �׸� ���� ���ܼ� ��ü �Ѱ� ���̺��� ���������ִ�.
	// BM_SMALL_CONCRETE_FLOOR�� �߰��ؼ� '�ٴ� �� ��'�� ���� ���� �ߴ�.
	long nTypeBM	= m_pStd->m_pARcBridgeDataStd->m_nTypeBMApply;
	HGBOOL bFootingLabeledFloor = (nTypeBM != TYPE_BM_NORMAL && m_pStd->m_pARcBridgeDataStd->m_bSepFootingForSiljuk == FALSE)? TRUE : FALSE;
	long nSize = 0;

	// ����
	// �Ϲ��� : ��鿵���� ���ʳ��̸� ���ϸ� ���� OK
	// slope�� : slope���� ���������� ���̴� ���°� ���ٰ� �����ϰ�, ���� ������ ���ʱ��̸� ���Ѵ�.
	long jijum = 0; 
	for(jijum = 0; jijum <= pBri->m_nQtyJigan; jijum++)
	{
		if(pBri->IsBoxType()) break; // ��ü���� ����

		// ��������� ���ϴ� ��� ��������� �߰��Ѵ�.
		// �� �ڼ��� ������ �� �Լ��� ȣ�� �ϴ� ������ ������ �� ��� ��.
		CTypedPtrArray <CObArray, CBMData*> *pArrBMStd	= 
			bForDeductOfReFill ? &m_pStd->m_pArrOutBMTogong.GetAt(m_nBri)->m_pArrBMTogongStd[jijum] : &m_pArrBMBridgeStd;

		BOOL bInner = (jijum == 0 || jijum == pBri->m_nQtyJigan)? FALSE : TRUE;
		CFootingApp *pFooting	= pBri->GetFootingByJijumNumber(jijum);
		if(pFooting)
		{
			// �Ϲ����϶� ..
			if(pFooting->m_nType == FOOTING_TYPE_BASIC || pFooting->m_nType == FOOTING_TYPE_BASIC_BOTH)
			{                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   
				// �Ϲݼ��� ���Ҷ��� ���� ���� ��� ���ϸ� ������, �Ǹ޿�� �������� ���� ���ݼ��� ���� ���� ���̰� �޶���
				// ������ ���� �߰��� �ִٸ� ���� ���̸� �����ϸ鿡�� ���ݱ����� ������
				// �� ���ʿ� ���ؼ��� ������ ���� �ȿ��� �������������ϴ� ��쿡�� ������ el�� ���� �������� ó����(���� ���� �ʿ�)
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
					pBM->m_sUnit				= "��";
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
					pBM->m_sInfo				= "������ũ��Ʈ";
					pBM->CalculateResult();
					
					pArrBMStd->Add(pBM);
				}
			}
			// ������϶�...
			// ������ : Ȯ���϶��� ����� ���������, �簢�� ������ ���ʸ� ����ó���ϸ� �������. �Ѥ�;
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
					pBM->m_sUnit				= "��";
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
					pBM->m_sInfo				= "������ũ��Ʈ";
					pBM->CalculateResult();
					
					pArrBMStd->Add(pBM);
				}
			}
		}
	}
	
	// ��ü(Ȯ������ �ȵ�)
	// ���� ��ü�� ��Ÿ��� ������ ���Ǿ� �ִ�. ��ũ��Ʈ ���� ����ÿ��� ���Ÿ��� ���̸� ���ؾ� �Ѵ�.
	// ������ ���� ���Ÿ��� ���������.
	CDPoint vAngStt	= pBri->GetAngleJijum(0);
	CRebarPlacing rb;
	double dLenBridge	= pBri->GetWidthSlabAct(pBri->m_dStationBridgeStt, CDPoint(0, 1), 0, TRUE);
	nSize	= pBri->m_pArrAreaConc.GetSize();
	long i = 0; for(i = 0; i < nSize; i++)
	{
		// ���� ����.. �׸��� 
		CTwinVectorAreaShape *pArea	= pBri->m_pArrAreaConc.GetAt(i);

		// ������ sta�� ���� ������ �����ؼ� ��ü ���̸� �ľ��Ѵ�.
		// �� ��ν������� ��� ��鿡�� �� ���·� ���ϴ°� ����������.
		// �׷�������� �����ȵ�.
		CDPointArray xyArr;
		pArea->m_tvArr.GetToPointArray(xyArr);
		double dStaLeft		= rb.GetXyEdgePoint(xyArr, UPLEFT_AREA).x;
		double dStaRight	= rb.GetXyEdgePoint(xyArr, UPRIGHT_AREA).x;
		
		double dStaMid		= (dStaLeft + dStaRight)/2;
		CDPoint xyDir		= pBri->GetAngleByStation(dStaMid);
		
		if(bForDeductOfReFill)
		{
			//ARCBRIDGE-3080 ������ �����ϱ�� ��
			BOOL bStt= TRUE;
			for (long ix=0; ix< pArea->m_tvArr.GetSize(); ix++)
			{
				CTwinVector tv = pArea->m_tvArr.GetAt(ix);
				if(tv.m_sInfo.Find(_T("����")) > -1)
				{
					bStt = FALSE;
					break;
				}
			}

			CTwinVector tvSlab;
			pBri->GetTvWingWallPlane(bStt);
			pBri->m_tvArrPlaneWingWall.GetTvByInfo("�����곡�ܿ���", tvSlab);
			dLenBridge = tvSlab.GetLength();
		}
		else
		{
			dLenBridge			= pBri->GetWidthSlabAct(dStaMid, xyDir, 0, FALSE);
		}		

		long nTypeConcrete = ePartUpperSlab;
		if(pArea->m_sInfo.Find(_T("�Ϻν�����")) != -1 || pArea->m_sInfo.Find(_T("�λ���� ����")) != -1)
			nTypeConcrete = ePartLowerSlab;
		else if(pArea->m_sInfo.Find(_T("��ν�����")) != -1 || pArea->m_sInfo.Find(_T("��ġ")) != -1)
			nTypeConcrete = ePartUpperSlab;
		else
		{
			BOOL bStt = (pArea->m_sInfo.Find(_T("����")) != -1);
			nTypeConcrete = bStt? ePartOutWallStt : ePartOutWallEnd;
		}
		
		// �������̿��� ���� �ؾ� �Ǵ� ��Ȳ ��� /////////////////////////////////////////////
		
		// [1] ������ ������� ������ ����ؾ� ��.
		// �������� : �ܺ�ȭ�� �������� ���������� ���� ��Ȯ�ϰ� ����� �Ǿ�� ��.
		if(pArea->m_sInfo == "���������" || pArea->m_sInfo == "���������")
		{
			BOOL bStt = pArea->m_sInfo == "���������";
			pBri->GetTvWingWallPlane(bStt, TRUE, TRUE);		// ����� ���� ���
			CTwinVector tv = pBri->m_tvArrPlaneWingWall.GetTvByInfo("�����");
			dLenBridge = tv.GetLength();
		}

		// [2] ������� ������ ���� ��ġ���� ���ܿ��� ��ġ���� �� �Ÿ� ���
		double dRealLenBridge = 0;
		CTwinVector tvSlab;
		BOOL bSttWall		= pArea->m_sInfo.Find(_T("����"),0) > -1? TRUE : FALSE;

		pBri->GetTvWingWallPlane(bSttWall);			
		pBri->m_tvArrPlaneWingWall.GetTvByInfo("�����곡�ܿ���", tvSlab);

		dRealLenBridge = tvSlab.GetLength();
		
		if(pArea->m_sInfo.Find("��ü��ġ", 0) != -1 || pArea->m_sInfo.Find("������ġ", 0) != -1)
		{
			dLenBridge = pArea->m_sInfo.Find("������ġ", 0) != -1? dLenBridge : dRealLenBridge;
			double dJijumAng = xyDir.GetAngleRadian();
			
			dLenBridge	-= (pBri->m_dDH[0] + pBri->m_dDH[1])/sin(dJijumAng);
			if(pBri->m_bCantilSlab[0])
				dLenBridge -= pBri->m_dCantilB[0]/sin(dJijumAng);
			if(pBri->m_bCantilSlab[1])
				dLenBridge -= pBri->m_dCantilB[1]/sin(dJijumAng);
		}
		else if(pArea->m_sInfo.Find(_T("��ü"), 0) != -1)
		{
			dLenBridge = dRealLenBridge;
		}
		
		//////////////////////////////////////////////////////////////////////////////////////////////

		// ��������� ���ϴ� ��� ��������� �߰��Ѵ�.
		// �� �ڼ��� ������ �� �Լ��� ȣ�� �ϴ� ������ ������ �� ��� ��.
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

		if(pArea->m_sInfo == _T("��ν�����") || pArea->m_sInfo == _T("�Ϻν�����") || pArea->m_sInfo == _T("�λ���� ����"))
		{
			// ��ν������ �Ϲݼ�����������϶� ��ü�� ������
			// �������� ��������϶��� �ٴ� �� ���� ���� �Ѵ�.
			if(nTypeBM != TYPE_BM_NORMAL)
			{
				// pBM->m_nIdxSmallClass	= BM_SMALL_CONCRETE_FOOTING;
				pBM->m_nIdxSmallClass	= BM_SMALL_CONCRETE_FLOOR;
			}

			// ��ν������ �ɼǿ� ���� �����Ÿ��� ���� ���̸� ����ϱ⵵ �Ѵ�. ////
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
		// ## "���� ����� ���ʸ� �ٴ� �� ���� �и�" �ɼǿ� ���� 
		// (pBM->m_nIdxSmallClass == BM_SMALL_CONCRETE_FOOTING)�� ���ʷ� ������ �Ǿ� 
		// �ٽ� ����.(2010.02.09, ȫ���)
// 		if(nTypeBM != TYPE_BM_NORMAL)
// 		{
// 			if(pArea->m_sInfo == _T("��ν�����") || pArea->m_sInfo == _T("�Ϻν�����") || pArea->m_sInfo == _T("�λ���� ����"))
// 			{
// 				pBM->m_sSmallClass = _T("�ٴ� �� ��");
// 			}
// 		}
		pBM->m_nIdxItemClass		= BM_ITEM_CONCRETE_MAIN;
		pBM->m_sItemClass			= GetStringItemClass(pBM->m_nIdxItemClass);
		pBM->m_nTypeResult			= RESULT_VOLUMN;
		pBM->m_sUnit				= "��";
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

		if(pArea->m_sInfo == "����")	// ��ġ�� ����
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
			strTail.Format(_T("���� ���� �� %.3f"), pBM->m_dV[Length]);
			pBM->m_sNoteTail = strTail;
			pBM->m_dV[Result] = toM(toM(dArea)) * pBM->m_dV[Length];
		}

		pArrBMStd->Add(pBM);
	}

	// �߰����� ��ü
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

			// ������ sta�� ���� ������ �����ؼ� ��ü ���̸� �ľ��Ѵ�.
			// �� ��ν������� ��� ��鿡�� �� ���·� ���ϴ°� ����������.
			// �׷�������� �����ȵ�.
			CDPointArray xyArr;
			pArea->m_tvArr.GetToPointArray(xyArr);
// 			double dStaLeft	= rb.GetXyEdgePoint(xyArr, UPLEFT_AREA).x;
// 			double dStaRight	= rb.GetXyEdgePoint(xyArr, UPRIGHT_AREA).x;

			// ��������� ���ϴ� ��� ��������� �߰��Ѵ�.
			// �� �ڼ��� ������ �� �Լ��� ȣ�� �ϴ� ������ ������ �� ��� ��.
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
			pBM->m_sUnit				= "��";
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

			// ��ս��� ��ռ� ��ŭ �߰� ����
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

// �Ϲ���
// #define EXFOOTING_TYPE_LEAN			0	// �Ϲ� ���� ��ũ��Ʈ(��������)
// #define EXFOOTING_TYPE_MASS			1	// MASS
// #define EXFOOTING_TYPE_PILE			2	// ����
// #define EXFOOTING_TYPE_ALONE_HYUNTAPILE	3	// �������� Ÿ�� ����

// ������౳ : ��������,MASS����,���ұ���,�⼮����,�η¹��� Anchor
// #define BOXFOOTING_TYPE_LEAN			0
// #define BOXFOOTING_TYPE_MASS			1
// #define BOXFOOTING_TYPE_PILE			2
// #define BOXFOOTING_TYPE_STONE		3
// #define BOXFOOTING_TYPE_ANCHOR		4


	// ������ũ��Ʈ(LEAN) �� MASS ��ũ��Ʈ
	// �Ϲ��� : ��鿵���� ���ʳ��̸� ���ϸ� ���� OK
	// slope�� : slope���� ���������� ���̴� ���°� ���ٰ� �����ϰ�, ���� ������ ���ʱ��̸� ���Ѵ�.
	// Box�� : ������ũ��Ʈ�� �ϳ��� �Ǿ��ִ�. Jijum = 0

	long nQtyJijum = pBri->IsBoxType() ? 1 : pBri->GetCountJijum();
	for(jijum = 0; jijum < nQtyJijum; jijum++)
	{
		// ��������� ���ϴ� ��� ��������� �߰��Ѵ�.
		// �� �ڼ��� ������ �� �Լ��� ȣ�� �ϴ� ������ ������ �� ��� ��.
		CTypedPtrArray <CObArray, CBMData*> *pArrBMStd	= 
			bForDeductOfReFill ? &m_pStd->m_pArrOutBMTogong.GetAt(m_nBri)->m_pArrBMTogongStd[jijum] : &m_pArrBMBridgeStd;

		CFootingApp *pFooting	= pBri->GetFootingByJijumNumber(jijum);
		if(pFooting == NULL) continue;
		CExFootingApp *pExFooting	= &pFooting->m_exFooting;

		// ������ũ��Ʈ(LEAN)
		long nTypeExFooting = pBri->IsBoxType() ? pBri->m_nTypeLower : pExFooting->m_nType;
		if(nTypeExFooting != EXFOOTING_TYPE_MASS)
		{
			// �Ϲ����϶� ..
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
						pBM->m_sUnit				= "��";
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
					pBM->m_sUnit				= "��";
					pBM->m_nShape				= SHAPE_RECTANGLE;
					pBM->m_dV[WidthLower]		= Round(toM(dWidth),3);
					pBM->m_dV[Height]			= Round(toM(dHeight),3);
					pBM->m_dV[Length]			= Round(toM(dLength),3);
					pBM->CalculateResult();
					pArrBMStd->Add(pBM);
				}
			}
			// ������϶�...
			// ������ : Ȯ���϶��� ����� ���������, �簢�� ������ ���ʸ� ����ó���ϸ� �������. �Ѥ�;
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
					pBM->m_sUnit				= "��";
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
		// MASS ��ũ��Ʈ
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
				pBM->m_sUnit				= "��";
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

// ��ȣ�� ����
void COutBM::MakeBMConcreteGuardWall()
{
	CRcBridgeRebar *pBri = GetRcBridgeRebar();
	CRcBridgeRebar *pBriOrg = GetRcBridgeRebar(TRUE);
	if(!pBri) return;
	if(!pBriOrg) return;

	CLineInfo *pLine = pBri->GetLineBase();
	if(!pLine) return;

	// ���� ����.
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
				// �����ΰ�� ������ �°� �̾ƾ� ��
				// ��ũ��Ʈ, ��Ż�ΰ�� ��ũ��Ʈ�� �̰�, ���� ��� �𷡷� ����
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
				pBM->m_sUnit				= "��";
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
				pBM->m_sInfo				= "��ȣ����ũ��Ʈ";
				pBM->m_bIsUseForDeduct		= pArea->m_bMinusArea;
				pBM->CalculateResult();
				
				pArrBMStd->Add(pBM);

				nLastNum	= max(nLastNum, pArea->m_nNum);
			}


			// �����ΰ�� ���� ������ �̾ƾ� �ȴ�.
			// �ƽ���, �������� �β��� �Բ� �������� �����ϰ�,
			// ��ũ��Ʈ�� ��� ���� ��ũ��Ʈ�� �����ϰ� ����
			if(pGW->IsTypeHDanRoadFootway())
			{
				CBMData *pBM				= new CBMData;
				pBM->m_nNumbering			= nLastNum+1;
				pBM->m_nLevelTotal			= 2;
				if(pGW->m_nTypePave == 1)	// ��ũ��Ʈ ����
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
					pBM->m_sUnit				= "��";
				}
				else	// ������/�ƽ��� ����(�������α���)
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
					pBM->m_sUnit				= "��";
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
				pBM->m_sInfo				= "��ȣ����ũ��Ʈ";
				pBM->CalculateResult();
				
				pArrBMStd->Add(pBM);		
			}
		}
	}
}

// ���� �۾����� bri ����
// BOOL bRealBridge : ��ճ��̰� �ƴ� ���� ���� ���� ��������
CRcBridgeRebar* COutBM::GetRcBridgeRebar(BOOL bOrg)
{
	if(m_nBri < 0 || m_nBri > m_pStd->m_pARcBridgeDataStd->GetBridgeSize()-1) return NULL;
	CRcBridgeRebar *pBri = m_pStd->m_pARcBridgeDataStd->GetBridgeRebar(m_nBri);

	return bOrg ? pBri : m_pStd->GetBridge(m_nBri);
}

// ���ʸ� �����ؼ� ��� ��ũ��Ʈ ���� ����.
// ���⼭ �ѹ��� ���ؾ� ��ü���� �ε����� ���� �� ����
// ����, ��ü, ��ü ������ ����.
// BOOL bForDeductOfReFill : �Ǹ޿�⿡�� ���ݼ� ������ ��ü �������� ���
// ���� : ��κ� Separater �Լ��� ���� �ڵ����� ���� ������, ��ü�� ���� ���۾����� ������ ������.
// ���� : ������� �ǰ�����̱⵵ �ϰ�, ������ �ڵ����� �������� �۾����� ����
//       �� ��ä����� ��쿡�� ��� ©���� ����ϱ� �����ϹǷ� �ڵ������� �Լ��� �����.
void COutBM::CalAreaConc(CRcBridgeRebar *pBri, BOOL bForDeductOfReFill)
{
	if(!pBri) return;
	CRebarPlacing rb;
	CHgBaseBMStd baseBM;

	CTwinVectorArray tvArrVertSectionOld;
	tvArrVertSectionOld = pBri->m_tvArrVertSection;
	long nTypeJongdan	= pBri->m_nTypeJongdanForRebar;
	BOOL bBoxType = pBri->IsBoxType();
	
	// �Ǹ޿�� �������ϰ�쿡�� �����������·� ���Ѵ�.
	// �������� �̹� level�� ������ ���ڷ� �����Ƿ� 
	// ���⼭ �ٽ� GetTvVertSection_Std�� ȣ���ϸ� ��ǥ�� �̻�����
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

	// ����
	long nJ = 0; for(nJ = 0; nJ <= pBri->m_nQtyJigan; nJ++)
	{
		if(bBoxType) break;
		CFootingApp *pFooting	= pBri->GetFootingByJijumNumber(nJ);
		if(pFooting)
		{
			if(!pFooting->m_bIs) continue;

			// �������δ� 2���� ���� �� ���� �����Ƿ�.. ��ȣ�� ����� ��ȣ�� ������.
			if(nNum < nNumPlane)
				nNum = nNumPlane;	
			else
				nNumPlane = nNum;	
			
			AhTPADelete(&pFooting->m_pArrAreaConc,	(CTwinVectorAreaShape*)0);
			nIdx	= 0;

			// ������ ������ tvAreaShape�� ����
			CTwinVectorArray tvArr;
			tvArr	= pFooting->m_tvArrFront;

			if(bForDeductOfReFill)
			{
				// �Ǹ޿�� �������� ��� ���ı� ����� ���� �߶���� //////////////
				CTwinVectorArray tvArrTypagi;
				pBri->GetTvTogongLine(tvArrTypagi, nJ, FALSE, TRUE, TRUE);
				CTwinVector tvUpper,tvLower,tvLeft,tvRight;
				if(tvArrTypagi.GetTvByInfo("���", tvUpper) && tvArrTypagi.GetTvByInfo("�ϸ�", tvLower))
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

			// ���ʴ� ��鿡 ���ؼ� �����ߵ�.(����� Ư���ϸ� ����� �������� �����ϴ°� ���ϱ� ����)
			// ������� �������� ���ʸ� �������� ���� ����.
			AhTPADelete(&pFooting->m_pArrAreaConcPlane,	(CTwinVectorAreaShape*)0);
			nIdx	= 0;

			// ������ ������ tvAreaShape�� ����
			tvArr	= pFooting->m_tvArrplan;
			CTwinVector tvBack	= tvArr.GetTvByInfo("��������ĸ�");
			CDPoint xyRot		= ToDPointFrDegree(tvBack.GetXyDir().GetAngleDegree() * -1);
			rb.RotateTvArr(tvArr, CDPoint(0, 0), xyRot.GetAngleDegree());

			tvArr.Sort(0.1);
			if(pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP || pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP2)
			{
				//��鿡���� ���ش�
				for (long ix =0; ix < tvArr.GetSize(); ix ++)
				{
					CTwinVector tvCl = tvArr.GetAt(ix);
					if(tvCl.m_sInfo.Find(_T("���")) > -1)
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

	// ��ü
	long nCountInWall	= pBri->GetCountInWall();
	long i = 0; for(i = 0; i < nCountInWall; i++)
	{
		CWallApp *pWall	= pBri->GetInWall(i);
		if(pWall)
		{
			AhTPADelete(&pWall->m_pArrAreaConc,	(CTwinVectorAreaShape*)0);
			nIdx	= 0;

			// ������ ������ tvAreaShape�� ����
			CTwinVectorArray tvArr;
			tvArr	= pWall->m_tvArrFront;

			if(bForDeductOfReFill)
			{
				// �Ǹ޿�� �������� ��� ���ı� ����� ���� �߶���� //////////////
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
				if(tvArrTypagi.GetTvByInfo("���", tvUpper) && tvArrTypagi.GetTvByInfo("�ϸ�", tvLower))
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

	// ��ü
	AhTPADelete(&pBri->m_pArrAreaConc,	(CTwinVectorAreaShape*)0);
	
	if(bForDeductOfReFill)
	{
		//Box���϶��� ��ü �ϳ� ��°�� ���ش�..
		long nEnd = bBoxType ? 1 : 2;

		long stt = 0; for(stt = 0; stt < nEnd; stt++)
		{
			// ������ ������ tvAreaShape�� ����
			CTwinVectorArray tvArr;
			// ��ġ������ ���� ����. ���߿� ���� ������.
			if(pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
			{
				CTwinVector tv;
				long nSize = pBri->m_tvArrVertSection.GetSize();
				long n=0; for(n=0; n<nSize; n++)
				{
					tv = pBri->m_tvArrVertSection.GetAt(n);
					if(tv.m_sInfo != _T(""))	// ����
						tvArr.Add(tv);
				}
			}
			else if(bBoxType) //�ڽ����ϰ�� �ܰ� ���θ� ����..
				pBri->GetTvArrVertSectionOutLine(tvArr);
			else
				tvArr	= pBri->m_tvArrVertSection;

			// �Ǹ޿�� �������� ��� ���ı� ����� ���� �߶���� //////////////
			// ��ü�� �ڸ��Ⱑ �� ��ٷο�. ������ ������ �ι� �߶� ���°� ���ľߵ�
			long nJ = stt ? pBri->m_nQtyJigan : 0; 
			CTwinVectorArray tvArrTypagi;
			pBri->GetTvTogongLine(tvArrTypagi, nJ, FALSE, TRUE, TRUE);
			CTwinVector tvUpper,tvLower,tvLeft,tvRight;
			if(tvArrTypagi.GetTvByInfo("���", tvUpper) && tvArrTypagi.GetTvByInfo("�ϸ�", tvLower))
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

			//## �̽�#0011794
			//	SubTractTvArrAndTvArr���� ��ũ�� ���� ��ǥ�� üũ ����.
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
		// ������ ������ tvAreaShape�� ����
		CTwinVectorArray tvArr;
		tvArr = pBri->m_tvArrVertSection;
		// ��ġ������ ���� ����. ���߿� ���� ������.
		if(pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
		{
			CTwinVector tv;
			tvArr.RemoveAll();
			long nSize = pBri->m_tvArrVertSection.GetSize();
			long n=0; for(n=0; n<nSize; n++)
			{
				tv = pBri->m_tvArrVertSection.GetAt(n);
				if(tv.m_sInfo != _T(""))	// ����
					tvArr.Add(tv);
			}
		}

		nIdx = 0;
		CTwinVector tvLeft, tvRight, tvUpper, tvLower;

		long stt = 0; for(stt = 0; stt < 2; stt++)
		{		
			BOOL bStt = stt == iSTT;
			CString sStt = bStt ? "����" : "����";

			// ������ ��ü �ִٸ� ����������ü�� �����, ��ġ�� ����.
			if(pBri->IsOutWall(bStt))
			{
				// ��ü
				tvLeft	= tvArr.GetTvByInfo(sStt+"��ü������").Sort(FALSE);
				tvRight	= tvArr.GetTvByInfo(sStt+"��ü������").Sort(FALSE);
				tvUpper	= tvArr.GetTvByInfo("�������ϸ�").Sort(TRUE);
				GetXyMatchLineAndLine(tvLeft.m_v1, tvLeft.GetXyDir(), tvUpper.m_v1, tvUpper.GetXyDir(), tvLeft.m_v2);
				tvUpper.m_v1	= tvLeft.m_v2;
				GetXyMatchLineAndLine(tvRight.m_v1, tvRight.GetXyDir(), tvUpper.m_v1, tvUpper.GetXyDir(), tvRight.m_v2);
				tvUpper.m_v2	= tvRight.m_v2;
				tvLower.m_v1	= tvLeft.m_v1;
				tvLower.m_v2	= tvRight.m_v1;
				tvLower.m_sInfo	= "";
				if(bBoxType)
				{
					tvLower	= tvArr.GetTvByInfo("�Ϻν�������").Sort(TRUE);
					GetXyMatchLineAndLine(tvLeft.m_v1, tvLeft.GetXyDir(), tvLower.m_v1, tvLower.GetXyDir(), tvLeft.m_v1);
					tvUpper.m_v1	= tvLeft.m_v2;
					GetXyMatchLineAndLine(tvRight.m_v1, tvRight.GetXyDir(), tvLower.m_v1, tvLower.GetXyDir(), tvRight.m_v1);
					tvUpper.m_v2	= tvRight.m_v2;
					tvLower.m_v1	= tvLeft.m_v1;
					tvLower.m_v2	= tvRight.m_v1;
				}

				CTwinVector tv;
				BOOL bHunch	= tvArr.GetTvByInfo(sStt+"��ü�Ϻ���ġ", tv);
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
							pArea->m_sInfo	= sStt+"��ü�Ϻ���ġ";
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
						pArea->m_sInfo	= sStt+"��ü";
						pBri->m_pArrAreaConc.Add(pArea);
					}
				}

				CTwinVectorAreaShape *pArea	= new CTwinVectorAreaShape(nNum++);
				pArea->AddTwinVector(tvLower);
				pArea->AddTwinVector(tvLeft);
				pArea->AddTwinVector(tvRight);
				pArea->AddTwinVector(tvUpper);
				pArea->Calculate();
				pArea->m_sInfo	= sStt+"��ü";
				pBri->m_pArrAreaConc.Add(pArea);

				// �����
				pArea	= new CTwinVectorAreaShape(nNum++);
				tvLeft	= tvArr.GetTvByInfo(sStt+"���������").Sort(FALSE);
				tvUpper	= tvArr.GetTvByInfo(sStt+"����ϻ��").Sort(TRUE);
				tvLower	= tvArr.GetTvByInfo(sStt+"�������ġ").Sort(TRUE);
				tvUpper.m_v2.x = tvLower.m_v2.x; // �귡Ŷ ���ϸ��� ��ü�� ��ǥ������ �߻��ϴ� ��찡 ����.
				tvRight.m_v1 = bStt ? tvLower.m_v2 : tvLower.m_v1;
				tvRight.m_v2 = bStt ? tvUpper.m_v2 : tvUpper.m_v1;
				tvRight.m_sInfo	= "";
				pArea->AddTwinVector(tvLower);
				pArea->AddTwinVector(tvLeft);
				pArea->AddTwinVector(tvRight);
				pArea->AddTwinVector(tvUpper);
				pArea->Calculate();
				pArea->m_sInfo	= sStt+"�����";
				pBri->m_pArrAreaConc.Add(pArea);

				// ��ġ
				CTwinVector tvHunch	= tvArr.GetTvByInfo(sStt+"��ü��ġ").Sort(TRUE);
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
					pArea->m_sInfo	= sStt+"��ü��ġ";
					pBri->m_pArrAreaConc.Add(pArea);
				}
			}
		}

		// �߰����� ��ġ��
		CString str	= _T("");
		long i = 0; for(i = 0; i < pBri->m_nCountInWallHunch; i++)
		{
			CTwinVectorAreaShape *pArea	= NULL;
			str.Format("%d��°������ġ", i+1);

			CTwinVectorArray tvArrHunch, tvArrResult;
			CDoubleArray dArrAngleSu;
			
			// ���� ��ٸ���
			tvLeft	= tvArr.GetTvByInfo(str+"������").Sort(TRUE);
 			tvRight	= tvArr.GetTvByInfo(str+"������").Sort(TRUE);
			tvUpper.m_v1	= tvLeft.GetXyTop();
			tvUpper.m_v2	= tvRight.GetXyTop();
			tvUpper.m_sInfo	= "";
			tvLower.m_v1	= tvLeft.GetXyBottom();
			tvLower.m_v2	= tvRight.GetXyBottom();
			tvLower.m_sInfo	= "";

			// (ARCBRIDGE-3398) ���ܸ��� nLeft�� 0�� �ƴ϶�� ������ ������ �ʴ´�. ��ġ�� ���°�츦 ��� ����� �Ѵ�. 
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

			// �Ʒ��� ���簢��
			if(tvArr.GetTvByInfo(str+"�Ʒ�������", tvLeft))
			{
				tvLeft.Sort(FALSE);
				tvRight	= tvArr.GetTvByInfo(str+"�Ʒ�������").Sort(FALSE);
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
					pArea->m_sInfo	= str+"�Ϻ�";
					pBri->m_pArrAreaConc.Add(pArea);
				}
			}

			if(bBoxType)
			{
				str.Format("%d��°�����Ϻ���ġ", i+1);
				tvLeft	= tvArr.GetTvByInfo(str+"������").Sort(TRUE);
				tvRight	= tvArr.GetTvByInfo(str+"������").Sort(TRUE);
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

				// ���� ���簢��
				if(tvArr.GetTvByInfo(str+"��������", tvLeft))
				{
					tvLeft.Sort(FALSE);
					tvRight	= tvArr.GetTvByInfo(str+"��������").Sort(FALSE);
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
						pArea->m_sInfo	= str+"���";
						pBri->m_pArrAreaConc.Add(pArea);
					}
				}
			}
		}

		// ��� ������
		CString sSideStt	= pBri->IsOutWall(TRUE) ? "��������ϻ������" : "��������������";
		CString sSideEnd	= pBri->IsOutWall(FALSE) ? "��������ϻ������" : "��������������";
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
		
		str  = _T("��ν�����");
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
			// �Ϻ� ������
			CTwinVector tvStt	= tvArr.GetTvByInfo(_T("�Ϻν������ϸ�(������ü�Ʒ�)")).Sort(FALSE);
			CTwinVector tvEnd	= tvArr.GetTvByInfo(_T("�Ϻν������ϸ�(������ü�Ʒ�)")).Sort(FALSE);
			tvLeft.m_v1 = tvStt.m_v1;
			tvRight.m_v1 = tvEnd.m_v2;
			tvLeft.m_v2 = tvLeft.m_v1 + CDPoint(0, pBri->m_dTS_Lower);
			tvRight.m_v2 = tvRight.m_v1 + CDPoint(0, pBri->m_dTS_Lower);
			tvLeft.m_sInfo = _T("�Ϻν������ϸ�(������ü�Ʒ�)");
			tvRight.m_sInfo = _T("�Ϻν������ϸ�(������ü�Ʒ�)");
			tvUpper.m_v1	= tvLeft.m_v2;
			tvUpper.m_v2	= tvRight.m_v2;
			tvUpper.m_sInfo	= "";
			tvLower.m_v1	= tvLeft.m_v1;
			tvLower.m_v2	= tvRight.m_v1;
			tvLower.m_sInfo	= "";

			str  = _T("�Ϻν�����");
			CTwinVectorAreaShape *pArea	= new CTwinVectorAreaShape(nNum++);
			pArea->AddTwinVector(tvLower);
			pArea->AddTwinVector(tvLeft);
			pArea->AddTwinVector(tvRight);
			pArea->AddTwinVector(tvUpper);
			pArea->Calculate();
			pArea->m_sInfo	= str;
			pBri->m_pArrAreaConc.Add(pArea);

			// �λ��������
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

					str  = _T("�λ���� ����");
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

	// ������ũ��Ʈ(LEAN) �� MASS ��ũ��Ʈ
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

			// ������ ������ tvAreaShape�� ����
			CTwinVectorArray tvArr;
			tvArr	= pExFooting->m_tvArrFront;
			if(bForDeductOfReFill)
			{
				// �Ǹ޿�� �������� ��� ���ı� ����� ���� �߶���� //////////////
				CTwinVectorArray tvArrTypagi;
				pBri->GetTvTogongLine(tvArrTypagi, i, FALSE, TRUE, TRUE);
				CTwinVector tvUpper,tvLower,tvLeft,tvRight;
				if(tvArrTypagi.GetTvByInfo("���", tvUpper) && tvArrTypagi.GetTvByInfo("�ϸ�", tvLower))
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

			// ����
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

			// ������ũ��Ʈ�� ��鿡 ���ؼ� �����ߵ�.(����� Ư���ϸ� ����� �������� �����ϴ°� ���ϱ� ����)
			AhTPADelete(&pExFooting->m_pArrAreaConcPlane, (CTwinVectorAreaShape*)0);
			nIdx		= 0;
			nNumPlane	= nNum - 1;		// �ε��� ���߱� ����...

			// ������ ������ tvAreaShape�� ����
			tvArr	= pExFooting->m_tvArrPlan;
			CDPoint xyRot	= pBri->GetXyDirRotateDomByBridgeSttEndXy().Rotate90();
			rb.RotateTvArr(tvArr, CDPoint(0, 0), xyRot.GetAngleDegree());
			// �ϳ��� �������� �ڸ��� ���� �������� �����.
			for(long k=0; k<tvArr.GetSize(); k++)
			{
				CString str = tvArr[k].m_sInfo;
				if(str.Find(_T("�����������"))>=0)
				{
					CTwinVector tv = tvArr[k];
					CDPoint v = (tv.m_v2 - tv.m_v1).Unit();
					double dAng = v.GetAngleDegree();		// �߶Ծ��� ���� �˾Ƴ���
					rb.RotateTvArr(tvArr, CDPoint(0, 0), -dAng);	// �ݴ�� ���� �������� �����.
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

	// ��ġ���� ��� ���꿡 ���ؼ��� ���ش�.
	if(pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
	{
		CTwinVectorAreaShape *pArea	= new CTwinVectorAreaShape(nNum);
		long nSize = pBri->m_tvArrVertSectionRib[1].GetSize();
		long nNo = nSize/2;
		CTwinVector tvLeft  = pBri->m_tvArrVertSectionRib[0].GetAt(nNo);
		CTwinVector tvRight = pBri->m_tvArrVertSectionRib[1].GetAt(nNo);
		CDRect rt(tvLeft.m_v1, tvRight.m_v2);
		pArea->m_xyCen = rt.CenterPoint();
		pArea->m_sInfo	= "����";
		pBri->m_pArrAreaConc.Add(pArea);

		for(long nStt=0; nStt<2; nStt++)
		{
			// ���� tvArr
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
				// �Ǹ޿�� �������� ��� ���ı� ����� ���� �߶���� //////////////
				CTwinVectorArray tvArrReFill;
				CTwinVectorArray tvArrTypagi;
				nJ = nStt==0 ? 0 : pBri->m_nQtyJigan;
				pBri->GetTvTogongLine(tvArrTypagi, nJ, FALSE, TRUE, TRUE);
				CTwinVector tvUpper,tvLower,tvLeft,tvRight;
				if(tvArrTypagi.GetTvByInfo("���", tvUpper) && tvArrTypagi.GetTvByInfo("�ϸ�", tvLower))
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

	// ��ȣ��
	CRcBridgeRebar *pBriOrg	= GetRcBridgeRebar(TRUE);	// ��ȣ���� ���� �������� �۾���.
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

			// ������ ������ tvAreaShape�� ����
			CTwinVectorArray tvArr, tvArrDeduct;
			pBriOrg->GetTvGuardWall(pGW, tvArr, sta, vAng, TRUE, FALSE, FALSE, TRUE);
			// (ARCBRIDGE-3243) ������ �ִ°��� ���� �и��Ѵ�.
			tvArr.GetTvArrByInfo(_T("Hole"), tvArrDeduct, TRUE, FALSE);
			if(bForDeductOfReFill)
			{
				// �Ǹ޿�� �������� ��� ���ı� ����� ���� �߶���� //////////////
				CTwinVectorArray tvArrTypagi;
				pBri->GetTvTogongLine(tvArrTypagi, i, FALSE, TRUE, TRUE);
				CTwinVector tvUpper,tvLower,tvLeft,tvRight;
				if(tvArrTypagi.GetTvByInfo("���", tvUpper) && tvArrTypagi.GetTvByInfo("�ϸ�", tvLower))
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

// ������ �̸��� �ɼǿ� ���� �ٲ� ��츦 ����ؼ� �Լ��� ó����.
CString COutBM::GetStringBigClass(long nIdxBigClass)
{
	CRcBridgeRebar	*pBri	= GetRcBridgeRebar();
	if(!pBri) return _T("");

	if(nIdxBigClass == BM_BIG_BOHOBLOCK)		return "��ȣ��";
	if(nIdxBigClass == BM_BIG_CONCRETE)			return "��ũ��Ʈ";
	if(nIdxBigClass == BM_BIG_FORM)				return "��Ǫ��";
	if(nIdxBigClass == BM_BIG_POST)				return "���ٸ�";
	if(nIdxBigClass == BM_BIG_DOWELBAR)		return "DOWEL BAR";
	if(nIdxBigClass == BM_BIG_SCAFFOLD)			return "���";
	if(nIdxBigClass == BM_BIG_SPACER)				return "�����̼�";
	if(nIdxBigClass == BM_BIG_SLABCARE)			return "������ ���";
	if(nIdxBigClass == BM_BIG_CONCCARE)			return "��ũ��Ʈ ���";
	if(nIdxBigClass == BM_BIG_ASPHALTPROOF)	return "ASP ���";
	if(nIdxBigClass == BM_BIG_REBAR)				return "ö�ٰ�������";
	if(nIdxBigClass == BM_BIG_SURFACE)			return "�����";
	if(nIdxBigClass == BM_BIG_ASCON)				return pBri->m_nRoadMaterial==0 ? "ASCON ����" : "LMC ����";
	if(nIdxBigClass == BM_BIG_SAND)				return "��";
	if(nIdxBigClass == BM_BIG_PERMEABLE)		return "������";
	if(nIdxBigClass == BM_BIG_TACKCOTTING)		return "�� ����";
	if(nIdxBigClass == BM_BIG_SURFACEPROOF)	return "������";
	if(nIdxBigClass == BM_BIG_ANTIDROPPING)	return "���Ϲ� ����";
	if(nIdxBigClass == BM_BIG_NAMEPANEL)		return "������";
	if(nIdxBigClass == BM_BIG_DESPANEL)			return "������";
	if(nIdxBigClass == BM_BIG_TBM)					return "TBM��ġ";
	if(nIdxBigClass == BM_BIG_STYROFOAM)		return "��Ƽ����";
	if(nIdxBigClass == BM_BIG_FLEXIBLEJOINT)		return "��������";
	if(nIdxBigClass == BM_BIG_FLEXIBLEJOINT_COVER) return "������������";
	if(nIdxBigClass == BM_BIG_SHRINKAGE)		return "������ ��ũ��Ʈ";
	if(nIdxBigClass == BM_BIG_COVERPLATE)		return "COVER PLATE";
	if(nIdxBigClass == BM_BIG_MONUMENT)		return "������";
	if(nIdxBigClass == BM_BIG_NOTCH)				return "NOTCH";
	if(nIdxBigClass == BM_BIG_SCAFFOLD_LEG)	return "���ٸ�"; //todo 31274
	if(nIdxBigClass == BM_BIG_RUBBLEFILL)			return "�⼮ä��"; 
	if(nIdxBigClass == BM_BIG_DECKFINISH)		return "��ũ�Ǵϼ�";

	return _T("");
}

// ������ �̸��� �ɼǿ� ���� �ٲ� ��츦 ����ؼ� �Լ��� ó����.
// BOOL bAps : ���ӽ����� ��ũ��Ʈ ������ ����?
// long nTypeOfConcrete : ��ũ��Ʈ main�϶��� ����(0:������, 1:��ü, 2:������ü, 3:����, 4:��������, 5, ������, 6:���ӽ�����, 7:������)    (0 : ����, 1 : ��ü, 2 : ���ӽ�����, 3 : ������)
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
		
		CString strCGS = _T("kgf/cm��");
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
			sAggregateMain.Format("%.0f-", pGW ? pGW->m_dAggregate : pBri->GetValueAggregate((EPartOfBridge)nTypeOfConcrete)); //�ߺд�, ��ȣ�� , ��ȣ�� ��

		
		sAggregateLean.Format("%.0f-", pBri->GetValueAggregateLean());
		sAggregateMass.Format("%.0f-", pBri->GetValueAggregateMass());
		sAggregateBoho.Format("%.0f-", pBri->GetValueAggregateEtc());
		
		sSlumpMain.Format("-%.0f", pBri->GetValueSlump((EPartOfBridge)nTypeOfConcrete));
		sSlumpLean.Format("-%.0f", pBri->GetValueSlumpLean());
		sSlumpMass.Format("-%.0f", pBri->GetValueSlumpMass());
		sSlumpBoho.Format("-%.0f", pBri->GetValueSlumpEtc()); //���� ���� ��� ���߿� ������ �ٲ�ٸ� (��ȣ��, ��ȣ��, �ߺд�, ������)
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

	if(nIdxMidClass == BM_MID_FORM_3TIMES)		return nTypeBM == TYPE_BM_NORMAL ? "����3ȸ"			: "�Ų��� ����";
	if(nIdxMidClass == BM_MID_FORM_PATTERN)		return "���� ��Ǫ��";
	if(nIdxMidClass == BM_MID_FORM_PATTERN_S)	return "����� Ư¡�ִ� ���� (���̰�Ǫ��)"; 
	if(nIdxMidClass == BM_MID_FORM_GANGJE_S)	return nTypeBM == TYPE_BM_NORMAL ? "�����Ǫ��"			: "����� Ư¡�ִ� ���� (�����Ǫ��)";
	if(nIdxMidClass == BM_MID_FORM_COTTING_S)	return "����� Ư¡�ִ� ���� (���ð�Ǫ��)";
	if(nIdxMidClass == BM_MID_FORM_DECK_S)		return "����� Ư¡�ִ� ���� (DECK PLATE)";
	if(nIdxMidClass == BM_MID_FORM_ONE_S)		return "����� Ư¡�ִ� ���� (1ȸ ���)";
	if(nIdxMidClass == BM_MID_FORM_CIRCLE)		return "������Ǫ��";
	if(nIdxMidClass == BM_MID_FORM_EURO)		return "������";
	if(nIdxMidClass == BM_MID_FORM_4TIMES)		return nTypeBM == TYPE_BM_NORMAL ? "����4ȸ"			: "���븶��";
	if(nIdxMidClass == BM_MID_FORM_6TIMES)		return nTypeBM == TYPE_BM_NORMAL ? "����6ȸ"			: "��ģ����";
	if(nIdxMidClass == BM_MID_SCAFFOLD_MAIN || nIdxMidClass == BM_MID_SCAFFOLD_MAIN_LEG)	return "��ü";
	if(nIdxMidClass == BM_MID_SCAFFOLD_WING || nIdxMidClass == BM_MID_SCAFFOLD_WING_LEG)	return "������"; // 31724
	if(nIdxMidClass == BM_MID_SLABCARE)			return "������ ���";
	if(nIdxMidClass == BM_MID_CONCCARE)			return "��ũ��Ʈ ���";
	if(nIdxMidClass == BM_MID_REBAR)			return "ö�ٰ�������";
	if(nIdxMidClass == BM_MID_TACK_RSC1)		return "";		// (ARCBRIDGE-1570) "RSC-1" ���� ��û
	if(nIdxMidClass == BM_MID_SURFACEPROOF_PERCOLATE)	return pBri->m_nTypeSurfaceProof == 0 ? "ħ����" : "��Ʈ���";
	if(nIdxMidClass == BM_MID_ASPHALTPROOF)		return "ASP ���";
	if(nIdxMidClass == BM_MID_STYROFOAM)		return "��Ƽ����";
	if(nIdxMidClass == BM_MID_FLEXIBLEJOINT)	return "��������";
	if(nIdxMidClass == BM_MID_FLEXIBLEJOINT_COVER)	return "������������";
	if(nIdxMidClass == BM_MID_SHRINKAGE)		return "������ ��ũ��Ʈ";
	//if(nIdxMidClass == BM_MID_RUBBLEFILL)		return "�⼮ä��";

	return _T("");
}

CString COutBM::GetStringSmallClass(long &nIdxSmallClass, BOOL bStt)
{
	CRcBridgeRebar	*pBri = GetRcBridgeRebar();
	if(!pBri) return _T("");
	long nTypeBM	= m_pStd->m_pARcBridgeDataStd->m_nTypeBMApply;
	BOOL bSepFootingForSiljuk	= ((nTypeBM == TYPE_BM_ACTUAL) && (m_pStd->m_pARcBridgeDataStd->m_bSepFootingForSiljuk));

	if(nIdxSmallClass == BM_SMALL_CONCRETE_FOOTING)		return nTypeBM == TYPE_BM_NORMAL ? "����"			: 
																							(bSepFootingForSiljuk) ? "����" : "�ٴ� �� ��";
	if(nIdxSmallClass == BM_SMALL_CONCRETE_MAIN)		return nTypeBM == TYPE_BM_NORMAL ? "��ü"			: "�� �� ���";
	if(nIdxSmallClass == BM_SMALL_CONCRETE_LEAN)		return nTypeBM == TYPE_BM_NORMAL ? "������ũ��Ʈ"	: "�����";
	if(nIdxSmallClass == BM_SMALL_CONCRETE_MASS)		return "MASS ��ũ��Ʈ";
	if(nIdxSmallClass == BM_SMALL_CONCRETE_WALL)		return "�� �� ���";
	if(nIdxSmallClass == BM_SMALL_CONCRETE_WING)	
	{
		if(nTypeBM != TYPE_BM_NORMAL)						// �Ϲ� ���� ���ؿ����� �������� ������ �ϹǷ� BM_SMALL_CONCRETE_WING
			nIdxSmallClass = BM_SMALL_CONCRETE_MAIN;		// �� �ܴ� ���� �ؾ� �ϹǷ� BM_SMALL_CONCRETE_MAIN
		return nTypeBM == TYPE_BM_NORMAL ? "������"	: "�� �� ���";
	}
	if(nIdxSmallClass == BM_SMALL_CONCRETE_APS)			return nTypeBM == TYPE_BM_NORMAL ? "���ӽ�����"		: "�ٴ� �� ��";
	if(nIdxSmallClass == BM_SMALL_CONCRETE_CNS)			return nTypeBM == TYPE_BM_NORMAL ? "���潽����"		: "�ٴ� �� ��";
	if(nIdxSmallClass == BM_SMALL_CONCRETE_BOHO_FOOTING)	return "���ʺ� CON'C";
	if(nIdxSmallClass == BM_SMALL_CONCRETE_BOHO_BACKFILL)	return "��ä�� CON'C";
	if(nIdxSmallClass == BM_SMALL_CONCRETE_FLOOR)		return "�ٴ� �� ��";
	if(nIdxSmallClass == BM_SMALL_FORM_VERTICAL)		return "������";
	if(nIdxSmallClass == BM_SMALL_FORM_SLOPE)			return "����";
	if(nIdxSmallClass == BM_SMALL_FORM_CURVE)			return "���";
	if(nIdxSmallClass == BM_SMALL_FORM_HORI)			return "�����";
	if(nIdxSmallClass == BM_SMALL_POST_STEELPIPE)		return "�������ٸ�";
	if(nIdxSmallClass == BM_SMALL_POST_WOOD)			return "���絿�ٸ�";
	if(nIdxSmallClass == BM_SMALL_POST_BRIDGE_STEEL)	return "���絿�ٸ�(������)";
	if(nIdxSmallClass == BM_SMALL_POST_HORI_CONNECT)	return "���򿬰���";
	if(nIdxSmallClass == BM_SMALL_SCAFFOLD_MAIN)		return GetStringMidClass(BM_MID_SCAFFOLD_MAIN);
	if(nIdxSmallClass == BM_SMALL_SCAFFOLD_WING)		return GetStringMidClass(BM_MID_SCAFFOLD_WING);
	if(nIdxSmallClass == BM_SMALL_SPACER_WALL)			return "��ü��";
	if(nIdxSmallClass == BM_SMALL_SPACER_SLAB)			return "������ �� ���ʿ�";
	if(nIdxSmallClass == BM_SMALL_CONCCARE)				return "��ũ��Ʈ ���";
	if(nIdxSmallClass == BM_SMALL_ASPHALTPROOF)			return _T("");	// (ARCBRIDGE-1570) "����� 1ȸ" ���� ��û
	if(nIdxSmallClass == BM_SMALL_REBAR_SIMPLE || nIdxSmallClass == BM_SMALL_DOWELBAR)			return "����";
	if(nIdxSmallClass == BM_SMALL_REBAR_NORMAL)			return "����";
	if(nIdxSmallClass == BM_SMALL_REBAR_COMPLEXITY1)	return "����";
	if(nIdxSmallClass == BM_SMALL_REBAR_COMPLEXITY2)	return "�ſ캹��";
	if(nIdxSmallClass == BM_SMALL_STYROFOAM)			return "";
	if(nIdxSmallClass == BM_SMALL_FLEXIBLEJOINT)		return bStt ? pBri->m_fxJointStt.m_sName : pBri->m_fxJointEnd.m_sName;
	if(nIdxSmallClass == BM_SMALL_SHRINKAGE)			return bStt ? pBri->m_fxJointStt.m_sName : pBri->m_fxJointEnd.m_sName;
	if(nIdxSmallClass == BM_SMALL_SCAFFOLD_WING_LEG)	return GetStringMidClass(BM_MID_SCAFFOLD_WING_LEG);// 31724
	if(nIdxSmallClass == BM_SMALL_SCAFFOLD_MAIN_LEG)	return GetStringMidClass(BM_MID_SCAFFOLD_MAIN_LEG);
	if(nIdxSmallClass == BM_SMALL_FJ_NONE)				return "����";
	if(nIdxSmallClass == BM_SMALL_FJ_GUARDFENCE)		return "��ȣ��";
	if(nIdxSmallClass == BM_SMALL_FJ_CENTERGUARD)		return "�ߺд�";
	if(nIdxSmallClass == BM_SMALL_FJ_CENTERGUARDHALF)	return "�ߺд�1/2";
	if(nIdxSmallClass == BM_SMALL_FJ_FOOTWAY)			return "����";
	if(nIdxSmallClass == BM_SMALL_FJ_FOOTWAYBLOCK)		return "������ ��輮";
	if(nIdxSmallClass == BM_SMALL_FJ_SOUNDPROOF)		return "������ ����";
	if(nIdxSmallClass == BM_SMALL_FJ_RETAININGWALL)		return "�븷�̺�";
	if(nIdxSmallClass == BM_SMALL_FJ_HANDRAIL)			return "��������";
	if(nIdxSmallClass == BM_SMALL_FJ_DIKE)				return "����ũ";
	//if(nIdxSmallClass == BM_SMALL_RUBBLEFILL)				return "�⼮ä��";

	

	return _T("");
}

// ��ü ��Ǫ�� ����
void COutBM::MakeBMFormMain()
{
	CRcBridgeRebar *pBri = GetRcBridgeRebar();
	if(!pBri) return;

	CRcBridgeRebar *pBriOrg	= GetRcBridgeRebar(TRUE);
	if(!pBriOrg) return;

	// ���� ����.
	CHgBaseBMStd baseBM;
	CalAreaConc(pBri, FALSE);
	
	long	nSize = 0;
	double	dSttMeter = 0;
	double	dEndMeter = 0;

	long	nSizeBM	= m_pArrBMBridgeStd.GetSize();
	double	dFirstMeter	= 7000;
	double	dSeconeMeter = 3000;

	// ����������� ���� ///////////////////////////////////
	const HGBOOL bDeductLeft	= pBri->m_nDeductNearFormBM == 1;
	const HGBOOL bDeductRight	= pBri->m_nDeductNearFormBM == 2;
	/////////////////////////////////////////////////////////

	CRebarPlacing rb;

	// ����
	// �Ϲ��� : ��鿵���� �������� ����.
	// slope�� : ������ �������� ����.
	long nCountFooting	= pBri->GetCountFooting();
	long i = 0; 
	for(i = 0; i < nCountFooting; i++)
	{
		if(pBri->IsBoxType()) break; // ��ü���� ����

		CFootingApp *pFooting	= pBri->GetFooting(i);
		if(pFooting)
		{
			// �Ϲ���
			if(pFooting->m_nType == FOOTING_TYPE_BASIC || pFooting->m_nType == FOOTING_TYPE_BASIC_BOTH)
			{
				double dHeight	= pFooting->GetHeight();
				nSize	= pFooting->m_pArrAreaConcPlane.GetSize();
				long j = 0; for(j = 0; j < nSize; j++)
				{
					CTwinVectorAreaShape *pArea	= pFooting->m_pArrAreaConcPlane.GetAt(j);

					// ������
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
					pBMVert->m_sUnit				= "��";
					pBMVert->m_nShape				= pArea->m_nTypeShape;
					pBMVert->m_dV[WidthLower]		= Round(toM(pArea->m_dWidthLower),3);
					pBMVert->m_dV[WidthUpper]		= Round(toM(pArea->m_dWidthUpper),3);
					pBMVert->m_dV[WidthDiffLeft]	= Round(toM(pArea->m_dWidthDiffLeft),3);
					pBMVert->m_dV[WidthDiffRight]	= Round(toM(pArea->m_dWidthDiffRight),3);
					pBMVert->m_dV[Height]			= Round(toM(pArea->m_dHeight),3);
					pBMVert->m_dV[Length]			= Round(toM(dHeight),3);
					pBMVert->m_dwSide				= SIDE_UPPER | SIDE_LOWER | SIDE_LEFT | SIDE_RIGHT;
					pBMVert->m_sInfo				= "���ʰ�Ǫ��";
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
			// slop�� 
			// �米�̰ų� Ȯ���϶� �뷫 ���.. �Ѥ�;
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

					// ������
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
					pBMVert->m_sUnit				= "��";
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

					pBMVert->m_sInfo				= "���ʰ�Ǫ��";
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

					// ����
					double dLenFooting	= Round(toM(pFooting->GetLength()),3);
					if(!Compare(pBMVert->m_dV[WidthDiffLeft], 0.0, "="))
					{
						CBMData *pBMSlope			= new CBMData(pBMVert);
						pBMSlope->m_nIdxSmallClass	= BM_SMALL_FORM_SLOPE;
						pBMSlope->m_sSmallClass		= GetStringSmallClass(pBMSlope->m_nIdxSmallClass);
						pBMSlope->m_sUnit			= "��";
						pBMSlope->m_dwSide			= 0;
						pBMSlope->m_nTypeResult		= RESULT_AREA;
	
						CTwinVector tvLeft	= pArea->GetTvLeft();
						double dHeight = toM(tvLeft.GetVerLength());
						double dWidth  = toM(tvLeft.GetHorLength());
						if(tvLeft.m_sInfo != "")
						{
							// overlap üũ�ؼ� �������̳� �⺻���̵��� ���� �������� /////////////////
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
										// ���� ���� ������ ��ü �ϸ�(�ﰢ�� ������)�� �������Ѵٰ� ����
										CTwinVector tv	= pAreaShape->GetTvRight();	
										dHeight -= toM(tv.GetVerLength());
										dWidth  -= toM(tv.GetHorLength());
									}
								}
							}
							
							pBMSlope->m_dwSide |= SIDE_LEFT;
							pBMSlope->m_sInfo = "���ʰ�Ǫ��";
							pBMSlope->m_sForm.Format("��((%.3f x %.3f) + (%.3f x %.3f)) x %.3f", dHeight, dHeight, dWidth, dWidth, dLenFooting);
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
						pBMSlope->m_sUnit			= "��";
						pBMSlope->m_dwSide			= 0;
						pBMSlope->m_nTypeResult		= RESULT_AREA;

						CTwinVector tvRight	= pArea->GetTvRight();
						double dHeight = toM(tvRight.GetVerLength());
						double dWidth  = toM(tvRight.GetHorLength());

						if(tvRight.m_sInfo != "")
						{								
							// overlap üũ�ؼ� �������̳� �⺻���̵��� ���� �������� /////////////////
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
										// ���� ���� ������ ��ü �ϸ�(�ﰢ�� ����)�� �������Ѵٰ� ����
										CTwinVector tv	= pAreaShape->GetTvLeft();	
										dHeight -= toM(tv.GetVerLength());
										dWidth  -= toM(tv.GetHorLength());
									}
								}
							}
							
							pBMSlope->m_dwSide |= SIDE_RIGHT;
							pBMSlope->m_sInfo = "���ʰ�Ǫ��";
							pBMSlope->m_sForm.Format("��((%.3f x %.3f) + (%.3f x %.3f)) x %.3f", dHeight, dHeight, dWidth, dWidth, dLenFooting);
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
	// ��ü and ������ü(����� ��� ���α���)
	// ������ü ������, ������ü ������ ��ü ���̿��� ������ �β��� ����ߵȴ�.
	CDPoint xyDirStt		= pBri->GetAngleByStation(pBri->m_dStationBridgeStt);
	CDPoint xyDirEnd		= pBri->GetAngleByStation(pBri->m_dStationBridgeStt+pBri->m_dLengthBridge);
	double dBridgeLength	= pBri->GetWidthSlabAct(pBri->m_dStationBridgeStt, xyDirStt, 0, TRUE);
	double dSttBridgeLength	= pBri->GetWidthSlabAct(pBri->m_dStationBridgeStt, xyDirStt, 0, TRUE);
	double dEndBridgeLength	= pBri->GetWidthSlabAct(pBri->m_dStationBridgeStt+pBri->m_dLengthBridge, xyDirEnd, 0, TRUE);

	// ������ִ� ��� ����������� ��������ġ�� ���⿡�� ����� ��찡 �־ ��ü ���̰� �� �� ��� �� �� �ִ�.
	double dSttBridgeLengthBracket	= dSttBridgeLength;
	double dEndBridgeLengthBracket	= dEndBridgeLength;

	// ������ ���� ��ü���̿� ������ �β� ����� ���� //////////////////////////////////////////////
	CDPoint xyMatch(0, 0);
	pBri->GetTvWingWallPlane(TRUE);
	CTwinVector tvSlab;
	if(pBri->m_tvArrPlaneWingWall.GetTvByInfo("�����곡�ܳ���", tvSlab))
		dSttBridgeLength	= tvSlab.GetLength();
	if(pBri->m_tvArrPlaneWingWall.GetTvByInfo("�����", tvSlab))
		dSttBridgeLengthBracket	= tvSlab.GetLength();
	else
		dSttBridgeLengthBracket	= dSttBridgeLength;

	pBri->GetTvWingWallPlane(FALSE);
	if(pBri->m_tvArrPlaneWingWall.GetTvByInfo("�����곡�ܳ���", tvSlab))
		dEndBridgeLength	= tvSlab.GetLength();
	if(pBri->m_tvArrPlaneWingWall.GetTvByInfo("�����", tvSlab))
		dEndBridgeLengthBracket	= tvSlab.GetLength();
	else
		dEndBridgeLengthBracket	= dEndBridgeLength;
	///////////////////////////////////////////////////////////////////////////////////////////////
	
	double dYBase	= GetElBaseForForm(-1);
	long nOpt = pBri->m_nHeightForSlabBM;


	// part : 0 �� ��ü
	// 1 ���� ������ü(��ս��� ���� ����)
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
		if(part==0)	// slab�� ������
			baseBM.RemakeByHeightTvAreaShape(pArrAreaConc, dYBase, dFirstMeter, dSeconeMeter, TRUE, nOpt);
		else
			baseBM.RemakeByHeightTvAreaShape(pArrAreaConc, dYBase, dFirstMeter, dSeconeMeter);

		nSize	= pArrAreaConc->GetSize();
		long nAdd = nSize - nFirstSize;	// ������� ������ �ε�...������ �ΰ��� �и��� ��� �ϳ��� �� �þ��.
		
		for(i = 0; i < nSize; i++)
		{
			CTwinVectorAreaShape *pArea	= pArrAreaConc->GetAt(i);
			
			// ���� sta������ ��ü/��ü ���� ����(��ü ������(��ν����� �κ��� ���� �ľ��ؼ� ���̸� �����ؾ� ��))
			CDPointArray xyArr;
			pArea->m_tvArr.GetToPointArray(xyArr);
			double dStaLeft		= rb.GetXyEdgePoint(xyArr, UPLEFT_AREA).x;
			double dStaRight	= rb.GetXyEdgePoint(xyArr, UPRIGHT_AREA).x;
			double dStaMid		= (dStaLeft+dStaRight)/2;

			// ���� ��ü�� ��ս� ��ü�� ���������� �� ����.
			BOOL bRoundRectWall	= part > 0 && (pWall->IsRoundRectWall() || pWall->m_bTopIsColumn);

			// ������ �Ÿ�
			// ���������� �������� ���ؼ� ���̰� �����Ǿ�� �ϴ� ��쿡 ���� ����
			// �̹� ������ ���� (dSttBridgeLength ��....)
			CDPoint xyDir	= pBri->GetAngleByStation(dStaMid);
			dBridgeLength	= pBri->GetWidthSlabAct(dStaMid, xyDir, 0, FALSE);

			// �������̿��� ���� �ؾ� �Ǵ� ��Ȳ ��� /////////////////////////////////////////////
			// [1] ������ ������� ������ ����ؾ� ��.
			// �������� : �ܺ�ȭ�� �������� ���������� ���� ��Ȯ�ϰ� ����� �Ǿ�� ��.
			if(pArea->m_sInfo == "���������" || pArea->m_sInfo == "���������")
			{
				BOOL bStt = pArea->m_sInfo == "���������";
				pBri->GetTvWingWallPlane(bStt, TRUE, TRUE);		// ����� ���� ���
				CTwinVector tv = pBri->m_tvArrPlaneWingWall.GetTvByInfo("�����");
				dBridgeLength = tv.GetLength();

				if(bStt)
					dSttBridgeLengthBracket = dBridgeLength;
				else
					dEndBridgeLengthBracket = dBridgeLength;
			}
			// �����Ǵ� Ȯ���϶� ���� ���� ������ ���� �������� �ٲ��ش�.
			if((pBri->IsExtLine() || pBri->IsExp(dStaMid)) && (pArea->m_sInfo == "������ü" || pArea->m_sInfo == "������ü"))
			{
				BOOL bStt = pArea->m_sInfo == "������ü";
				pBri->GetTvWingWallPlane(bStt);		// ����� ���� ���
				CTwinVector tv = pBri->m_tvArrPlaneWingWall.GetTvByInfo("�����곡�ܿ���");
				dBridgeLength = tv.GetLength();
			}

			// [2] ������� ������ ���� ��ġ���� ���ܿ��� ��ġ���� �� �Ÿ� ���
			if(pArea->m_sInfo.Find("��ü��ġ", 0) != -1 || pArea->m_sInfo.Find("������ġ", 0) != -1)
			{
				double dJijumAng = xyDir.GetAngleRadian();

				if(pArea->m_sInfo.Find("������ġ", 0) != -1)
				{
					dBridgeLength -= (pBri->m_dDH[0] + pBri->m_dDH[1])/sin(dJijumAng);
				}
				else
				{
					BOOL bStt = pArea->m_sInfo.Find(_T("����"))>-1? TRUE : FALSE;
					pBri->GetTvWingWallPlane(bStt);		// ����� ���� ���
					CTwinVector tv = pBri->m_tvArrPlaneWingWall.GetTvByInfo("�����곡�ܿ���");
					dBridgeLength = tv.GetLength();
				}		
				
				if(pBri->m_bCantilSlab[0])
					dBridgeLength -= pBri->m_dCantilB[0]/sin(dJijumAng);
				if(pBri->m_bCantilSlab[1])
					dBridgeLength -= pBri->m_dCantilB[1]/sin(dJijumAng);
			}

			// [3] �߰������� ��� ��ġ���̱����� ���ؾ� ��
			if(pWall)
				dBridgeLength	= pWall->m_tvPathPlan.GetLength();

			// [4] ��ġ����� ��ġ���� ���̸� �����
			if(pArea->m_sInfo == "����")
				dBridgeLength	= pBri->GetWidthArchRib(0);
			//////////////////////////////////////////////////////////////////////////////////////////////

			// ��ν������� �ɼǿ� ���� �������̸� ����ϱ⵵ �Ѵ�.//
			if(pArea->m_sInfo == "��ν�����")
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

			// �յ�(������)
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
			baseBMVert.m_sUnit					= "��";
			baseBMVert.m_nShape					= pArea->m_nTypeShape;
			baseBMVert.m_dV[SttMeter]			= Round(toM(dSttMeter),3);
			baseBMVert.m_dV[EndMeter]			= Round(toM(dEndMeter),3);

			if(part==0 && ((i==nSize-nAdd-2 && pBri->m_bIsExp) || i==nSize-nAdd-1) && pBri->m_nHeightForSlabBM > 0)
				baseBMVert.m_bSlab = TRUE;	// �Ѱ��� �������



			CBMData *pBMVert				= new CBMData(&baseBMVert);

			if(bRoundRectWall)	// ��ս� ��ü�� ���� ��ü
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
				pBMVert->m_dV[Length]			= Round(toM(dBridgeLength),3);			// ��� ���⼱ �ǹ̰� ���� ��
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
			pBMVert->m_sInfo = "��ü��Ǫ��";
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
			

			if(pArea->m_sInfo == "����")	// ��ġ�� ����
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
				strTail.Format(_T("���� ���� �� %d��"), nFace);
				pBMVert->m_sNoteTail = strTail;
				pBMVert->m_dV[Result] = toM(toM(dArea)) * nFace;

				CBMData *pBM	= CreateBMStyrotormForSide(pBMVert);
				if (pBM)
				{
					m_pArrBMBridgeStd.Add(pBM);
				}
			}

			// ������ ��� ���� ���̴� �ٽ� ���ϰ��¿������̰� �ٸ� �� �����Ƿ� ���� ����
			BOOL bFindSlab = pArea->m_sInfo.Find(_T("������")) > -1? TRUE : FALSE;
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
					if(tv.m_sInfo.Find("��", 0) != -1) dLenLeft += tv.GetLength();
					if(tv.m_sInfo.Find("��", 0) != -1) dLenRight += tv.GetLength();
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

			// ������ü �������� ������ ��ü�������� ��
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
					pBMVertSide->m_sInfo	= "��ü�����Ǫ��";
					pBMVertSide->CalculateResult();
					m_pArrBMBridgeStd.Add(pBMVertSide);
				}
				else
				{
					delete pBMVertSide;
				}
				
			}

			// ���� �ϸ�� ���
			if(pArea->m_sInfo == "����")
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
				pBMHor->m_sUnit				= "��";
				pBMHor->m_dwSide			= SIDE_UPPER;
				pBMHor->m_sNoteTail			= "��ġ������";
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
				pBMHor2->m_sUnit			= "��";
				pBMHor2->m_dwSide			= SIDE_LOWER;
				pBMHor2->m_sNoteTail		= "��ġ�����ϸ�";
				pBMHor2->CalculateResult();
				m_pArrBMBridgeStd.Add(pBMHor2);
			}

			// ��ü ����
			if(pWall)
			{
				if(Compare(pBMVert->m_dV[ArcLengthR], 0.0, ">") || Compare(pBMVert->m_dV[ArcLengthL], 0.0, ">"))
				{
					CBMData *pBMArc	= new CBMData(&baseBMVert);
					pBMArc->m_nIdxMidClass		= GetFormByPos(FORM_POS_WALL_FRONT);
					pBMArc->m_sMidClass			= GetStringMidClass(pBMArc->m_nIdxMidClass);
					pBMArc->m_nIdxSmallClass	= BM_SMALL_FORM_CURVE;
					pBMArc->m_sSmallClass		= GetStringSmallClass(pBMArc->m_nIdxSmallClass);
					pBMArc->m_sUnit				= "��";
					pBMArc->m_dwSide			= SIDE_LEFT | SIDE_RIGHT;
					pBMArc->m_sInfo				= "��ü�����Ǫ��";
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
					pBMSlope->m_sUnit			= "��";
					pBMSlope->m_dwSide			= 0;
					
					CTwinVector tvLeft	= pArea->GetTvLeft();	// ��ü �ϸ��� ���ʿ� ���ϹǷ� ��Ǫ������ �����Ѵ�.

					if(tvLeft.m_sInfo.Find(_T("��ü�ϸ�"))<0)
					{
						if(tvLeft.m_sInfo.Find(_T("��ü���"))<0)	// ��ü �ϸ��� ������� ���ϹǷ� ��Ǫ������ �����Ѵ�.
						{
							if(!Compare(pBMVert->m_dV[WidthDiffLeft], 0.0, "="))
								pBMSlope->m_dwSide	|= SIDE_LEFT;
						}
					}
					
					CTwinVector tvRight	= pArea->GetTvRight();
					if(tvRight.m_sInfo.Find(_T("��ü�ϸ�"))<0)
					{
						if(tvRight.m_sInfo.Find(_T("��ü���"))<0)
						{
							if(!Compare(pBMVert->m_dV[WidthDiffRight], 0.0, "="))
								pBMSlope->m_dwSide	|= SIDE_RIGHT;
						}
					}
					
					pBMSlope->m_sInfo = "��ü�����Ǫ��";
					pBMSlope->CalculateResult();
					if(pBMSlope->m_sFormForCompare != _T(""))
						m_pArrBMBridgeStd.Add(pBMSlope);
				}
			}

			// ���� ��ü�϶��� ����� ��Ǫ���� ����� ��.
			if(bRoundRectWall)
			{
				CBMData *pBMRound			= new CBMData(&baseBMVert);
				pBMRound->m_nIdxSmallClass	= BM_SMALL_FORM_CURVE;
				pBMRound->m_sSmallClass		= GetStringSmallClass(pBMRound->m_nIdxSmallClass);
				pBMRound->m_dV[Radius]		= Round(toM(pWall->IsRoundRectWall() ? pWall->m_dW/2 : pWall->m_Column.m_dR[iOUTTER][iUPPER]),3);
				pBMRound->m_dwSide			= SIDE_ROUND;
				pBMRound->CalculateResult();
				pBMRound->m_sInfo			= "��ü����ΰ�Ǫ��";
				m_pArrBMBridgeStd.Add(pBMRound);

				// Ư���� ��ս��̶�� ��� ������ŭ �߰�������
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

			// ���� �Ʒ��� ��ü�� �ƴ� �͵鿡 ���ؼ� ����.
			if(pWall) continue;

			// ������ ��ü�� ��ν������ �¿����� ��Ǫ�� ���Ҷ� ������̳� ��ġ�θ� ���� ���ؾ� �ȴ�.
			// �׷��� �¿����� ���ؼ��� �׻� �ٸ� �������� ��ħ�� üũ�Ѵ�.

			// ������౳ �Ϻ���ġ�� ������ ������ �ʴ´�.
			if((pArea->m_nTypeShape == SHAPE_TV_TRIANGLE || pArea->m_nTypeShape == SHAPE_TV_TRAPEZOID) && pArea->m_sInfo.Find("�Ϻ���ġ", 0) != -1)
			{
				continue;
			}

			// ��(���� �Ǵ� ������)
			// m_sInfo�� �־�� �������� ���̴�. ���°�� ������ ������ ��ġ�� �κ��̹Ƿ� ������ ����
			CTwinVector tvLeft	= pArea->GetTvLeft();
			if(tvLeft.m_sInfo != "")
			{
				CBMData *pBMLeft	= new CBMData(&baseBMVert);

				// overlap üũ�ؼ� �������̳� �⺻���̵��� ���� �������� /////////////////
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

				// �������Ȱ� ���ٸ� ���� �ʾƾ� ��
				if(bOverlap)
				{
					for(j = 0; j < tvArrLeft.GetSize(); j++)
					{
						dTotHeight	-= tvArrLeft.GetAt(j).GetVerLength();
						dTotHeightLeft -= tvArrLeft.GetAt(j).GetLength();
					}
				}

				BOOL bSttWall = tvLeft.m_sInfo.Find("����", 0) != -1? TRUE : FALSE;
				if(tvLeft.m_sInfo.Find("��ü", 0) != -1)
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
				
				if(dTotHeight > 0.0000001)	// ��Ȯ�� 0�� �ȳ��´�. //������ ��Ǫ��
				{
					if(!Compare(pBMLeft->m_dV[WidthDiffLeft], 0.0, "="))
						pBMLeft->m_nIdxSmallClass	= BM_SMALL_FORM_SLOPE;
					pBMLeft->m_sSmallClass		= GetStringSmallClass(pBMLeft->m_nIdxSmallClass);
					pBMLeft->m_dwSide			= SIDE_LEFT;

					switch(nTypeLeft)
					{
					case -2: pBMLeft->m_dV[Length]	= Round(toM(dSttBridgeLengthBracket),3); break;
					case -1: pBMLeft->m_dV[Length]	= Round(toM( dSttBridgeLength),3); break;
					default: pBMLeft->m_dV[Length]	= Round(toM(dBridgeLength),3); break; //������ü�� �����鵵 ������ ���� �������
					}
					pBMLeft->CalculateResult();
					pBMLeft->m_sInfo = "��ü��������Ǫ��";
					m_pArrBMBridgeStd.Add(pBMLeft);
				}
				else
					delete pBMLeft;
			}

			// ��(���� �Ǵ� ������)
			// m_sInfo�� �־�� �������� ���̴�. ���°�� ������ ������ ��ġ�� �κ��̹Ƿ� ������ ����
			CTwinVector tvRight	= pArea->GetTvRight();
			if(tvRight.m_sInfo != "")
			{
				CBMData *pBMRight	= new CBMData(&baseBMVert);
				
				// overlap üũ�ؼ� �������̳� �⺻���̵��� ���� �������� /////////////////
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

				// �������Ȱ� ���ٸ� ���� �ʾƾ� ��
				if(bOverlap)
				{
					for(j = 0; j < tvArrRight.GetSize(); j++)
					{
						dTotHeight	-= tvArrRight.GetAt(j).GetVerLength();
						dTotHeightRight -= tvArrRight.GetAt(j).GetLength();
					}
				}

				if(tvRight.m_sInfo.Find("��ü", 0) != -1)
				{
					if(tvRight.m_sInfo.Find("����", 0) != -1)
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
				if(dTotHeight > 0.0000001)	// ��Ȯ�� 0�� �ȳ��´�. ����  ��Ǫ��
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
					pBMRight->m_sInfo = "��ü��������Ǫ��";
					m_pArrBMBridgeStd.Add(pBMRight);
				}
				else
					delete pBMRight;
			}

			// ȸ���� ��ٸ����� ��� �Ʒ��ʵ� ���ؾ� �ȴ�. ///////////////////////// ���� ��Ǫ��, ����� ���� �̰��� ����
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
						if(nTypeBracket == -2 || nTypeBracket == 2) //����� ���� ��Ǫ�� 
						{
							CString sResult = _T("");
							double dHBracket = Round(toM(tv.GetHorLength()), 3);
							double dVBracket = Round(toM(tv.GetVerLength()), 3);
							double dLenBra	   = pBMHunch->m_dV[Length];

							dResult = sqrt((dHBracket*dHBracket) + (dVBracket*dVBracket)) * dLenBra;

							if(dResult > 0)
							{
								sResult.Format(_T("��(%.3f�� x %.3f��) x %.3f"), dHBracket, dVBracket, dLenBra);
								pBMHunch->m_sForm = sResult;
								pBMHunch->m_sFormForCompare = pBMHunch->m_sForm ;
								pBMHunch->SetResult(Round(dResult, 3));
							}							
						}						
						
						if(dResult == 0)
						{
							pBMHunch->CalculateResult();
						}						
					
						pBMHunch->m_sInfo = "��ü�������ġ�ΰ�Ǫ��";
						m_pArrBMBridgeStd.Add(pBMHunch);
					}
				}
			}
			///////////////////////////////////////////////////////////////////////////
		}

	}

	// �����Ǫ��(������ �ϸ�, ��ġ������)
	// �������ϸ��� ��쿡 ���� �ٸ� �� �����Ƿ� ������ �������ʰ� �������� ���� ���ؼ� ���̸� ����.
	// ���� ������ �̸��� �پ� ���� �ʾƼ� �׳� ��ȣ�� ��
	// ���� ������ �̸��� ���� �� �ְ� �Ǹ� �Ʒ� �ּ��� Ǯ��� ��.
	CTwinVector tvLower;
	long nNumSlabLower	= 0;//baseBM.GetIdxAreaByName(&pBri->m_pArrAreaConc, "��ν������ϸ�");
	for(i = 0; i < pBri->m_pArrAreaConc.GetSize(); i++)
	{
		CTwinVectorAreaShape *pArea	= pBri->m_pArrAreaConc.GetAt(i);
		if(pArea->m_sInfo.Find(_T("��ν�����"))>=0)
		{
			nNumSlabLower	= pArea->m_nNum;
			break;
		}
	}

	double	dLenLeft	= 0,
			dLenRight	= 0;
	CTwinVector tv;
	CString str	= _T("");

	// ���ʽ����� �ϸ� �� ����..
	pBri->GetTvVertSection(-1, TRUE);
	long j = 0; for(j = 0; j < pBri->m_nCountInWallHunch+1; j++)
	{
		if(j == 0) str.Format("�������ϸ�");
		else str.Format("%d��°������ġ�����������ϸ�", j);

		if(pBri->m_tvArrVertSection.GetTvByInfo(str, tv))	// ��米 �϶� �������ϸ� ��ġ ���Ͱ� Ʋ��...���� �Ǿ����.
			dLenLeft	+= tv.GetLength();
	}
	// �����ʽ����� �ϸ� �� ����..
	pBri->GetTvVertSection(1, TRUE);
	for(j = 0; j < pBri->m_nCountInWallHunch+1; j++)
	{
		if(j == 0) str.Format("�������ϸ�");
		else str.Format("%d��°������ġ�����������ϸ�", j);

		if(pBri->m_tvArrVertSection.GetTvByInfo(str, tv))	// ��米 �϶� �������ϸ� ��ġ ���Ͱ� Ʋ��...���� �Ǿ����.
			dLenRight	+= tv.GetLength();
	}
	
	// ������� �ǵ��� ����
	pBri->GetTvVertSection(0, TRUE);

	// �����
	// ������ �ϸ�� ��ġ�� �κ� ����
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
	pBMHor->m_sUnit					= "��";
	pBMHor->m_nShape				= SHAPE_TV_TRAPEZOID;
//	pBMHor->m_dV[SttMeter]			= Round(toM(dSttMeter),3);
//	pBMHor->m_dV[EndMeter]			= Round(toM(dEndMeter),3);
	pBMHor->m_dV[WidthLower]		= Round(toM(dLenLeft),3);
	pBMHor->m_dV[WidthUpper]		= Round(toM(dLenRight),3);
	pBMHor->m_dV[WidthDiffLeft]		= Round(toM(0),3);
	pBMHor->m_dV[WidthDiffRight]	= pBMHor->m_dV[WidthLower] - pBMHor->m_dV[WidthUpper];
	pBMHor->m_dV[Height]			= Round(toM(pBri->GetWidthSlabAct(pBri->m_dStationBridgeStt, CDPoint(0, 1), 0)),3);
	pBMHor->m_dV[Length]			= Round(toM(0),3);	// ��� ���⼱ �ǹ̰� ���� ��
	pBMHor->m_dwSide				= SIDE_FRONT;
	pBMHor->CalculateResult();
	pBMHor->m_sInfo					= "��ν������ϸ��Ǫ��";
	m_pArrBMBridgeStd.Add(pBMHor);	

	// �߰����� ��ġ �����(�ʺ� : ���Ÿ�, ���� : ��Ÿ�)
	long nCountInHunch	= pBri->m_nCountInWallHunch;
	for(long hunch	= 0; hunch < nCountInHunch; hunch++)
	{
		if(pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP) break;	// ��米 ����

		long nIdxWall	= 0;
		BOOL bLeft		=TRUE;
		pBri->GetInfoHunch(hunch, nIdxWall, bLeft);
		CString sLeft	= _T("");
		CString sRight	= _T("");
		sLeft.Format("%d��°������ġ", hunch+1); //%d��°������ġ�Ʒ�������
		sRight.Format("%d��°������ġ", hunch+1); //%d��°������ġ�Ʒ������

		long nSize	= pBri->m_pArrAreaConc.GetSize();
		long i = 0; for(i = 0; i < nSize; i++)
			CString st = pBri->m_pArrAreaConc.GetAt(i)->m_sInfo;

		long nIdxArea	= baseBM.GetIdxAreaByName(&pBri->m_pArrAreaConc, sLeft);	// ��ġ �ϸ� �ѹ��� ã�ƾ� ��
		if(nIdxArea<0)
			nIdxArea	= baseBM.GetIdxAreaByName(&pBri->m_pArrAreaConc, sRight);	// ��ġ �ϸ� �ѹ��� ã�ƾ� ��
		long nNumHunch	= nIdxArea > -1 ? pBri->m_pArrAreaConc.GetAt(nIdxArea)->m_nNum : 0;
		double dWidthUpperOfWall	= 0;	// ������ü���(�����ؾ� �� �κ���)
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

	// ������ ���� ��� (�Ϲ��� ���...��ƿ������ ��� �ؾߵ�)
	pBri->GetTvVertSection_Std(0, TRUE, FALSE);
	for(i=0; i<2; i++)	// ��,��
	{
		if(pBri->m_nSlabChemfer[i] == 0) continue;		// ����� ���°� ������

		// ���� ����
		CTwinVector tv;
		i==0 ? pBri->GetTvVertSection(-1, TRUE) : pBri->GetTvVertSection(1, TRUE);
		
		CString str(_T(""));
		double dSlabLength = 0;
		
		if(pBri->m_nTypeBridge == BRIDGE_TYPE_SLAB)
		{
			for(long nJijum = 0; nJijum < pBri->m_nQtyJigan; nJijum++)
			{
				str.Format(_T("��������(%d��°����)"), nJijum+1);
				pBri->m_tvArrVertSection.GetTvByInfo(str, tv);
				dSlabLength += tv.GetLength();
			}
		}
		else
		{
			str = _T("��������(������ü����)");
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
		pBMCurve->m_sUnit				= "��";
		pBMCurve->m_nShape				= SHAPE_TV_RECTANGLE;
		pBMCurve->m_dV[WidthLower]		= Round(toM(dSlabLength),3);
		pBMCurve->m_dV[WidthUpper]		= Round(toM(dSlabLength),3);
		pBMCurve->m_dV[WidthDiffLeft]	= Round(toM(0),3);
		pBMCurve->m_dV[WidthDiffRight]	= pBMCurve->m_dV[WidthLower] - pBMCurve->m_dV[WidthUpper];
		double dL = sqrt(pow(pBri->m_dTCW1[i],2)+pow(pBri->m_dTCH1[i],2));
		pBMCurve->m_dV[Height]			= pBri->m_nSlabChemfer[i] == 1 ? Round(toM(dL),3) : Round(toM(2*ConstPi*pBri->m_dRE1[i]/4),3);
		pBMCurve->m_dV[Length]			= Round(toM(0),3);	// ��� ���⼱ �ǹ̰� ���� ��
		pBMCurve->m_dwSide				= SIDE_FRONT;
		pBMCurve->CalculateResult();
		pBMCurve->m_sInfo				= "��ν����꿬�ܰ�Ǫ��";
		pBMCurve->m_bSlab				= TRUE;
		m_pArrBMBridgeStd.Add(pBMCurve);	

		// ��ν����� �¿��� ��Ǫ���� ������ ���̰� ��� �ȵǾ� �ֱ� ������ ���⼭ ������ش�.
		// ��ν����� ù��° ��Ǫ���� ���� ������ ������.
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
			pBMDeduct->m_sInfo				= "��ü��Ǫ��";
			m_pArrBMBridgeStd.Add(pBMDeduct);
		}
	}
	// ������ ���������� �߰��Ѵ�.
	RemakeByHeightBMList(&m_pArrBMBridgeStd, dFirstMeter, dSeconeMeter, nSizeBM-1, m_pArrBMBridgeStd.GetSize()-1, nOpt);
	pBri->GetTvVertSection_Std(0, TRUE, FALSE, TRUE);
	
	pBri->GetTvVertSection_Std(0, FALSE, FALSE);
	pBri->GetTvVertSection(0, TRUE);

	nSizeBM	= m_pArrBMBridgeStd.GetSize();
		 
	// ������ũ��Ʈ(LEAN) �� MASS ��ũ��Ʈ
	// �Ϲ��� : ��鿵���� �������� ����.
	// slope�� : ������ �������� ����.
	for(i=0; i<nCountFooting; i++)
	{
		CFootingApp *pFooting		= pBri->GetFooting(i);
		if(!pFooting) continue;
		CExFootingApp *pExFooting	= &pFooting->m_exFooting;
	
		// ������ũ��Ʈ
		long nTypeExFooting = pBri->IsBoxType() ? pBri->m_nTypeLower : pExFooting->m_nType;
		if(nTypeExFooting != EXFOOTING_TYPE_MASS)
		{
			// �Ϲ���
			if(pFooting->m_nType == FOOTING_TYPE_BASIC || pFooting->m_nType == FOOTING_TYPE_BASIC_BOTH || pFooting->m_nType == FOOTING_TYPE_BOX)
			{
				if(!pBri->IsBoxType())
				{
					double dHeight = pExFooting->m_dEFHR;
					nSize = pExFooting->m_pArrAreaConcPlane.GetSize();
					long j = 0; for(j = 0; j < nSize; j++)
					{
						CTwinVectorAreaShape *pArea	= pExFooting->m_pArrAreaConcPlane.GetAt(j);

						// ������
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
						pBMVert->m_sUnit				= "��";
						pBMVert->m_nShape				= pArea->m_nTypeShape;
						pBMVert->m_dV[WidthLower]		= Round(toM(pArea->m_dWidthLower),3);
						pBMVert->m_dV[WidthUpper]		= Round(toM(pArea->m_dWidthUpper),3);
						pBMVert->m_dV[WidthDiffLeft]	= Round(toM(pArea->m_dWidthDiffLeft),3);
						pBMVert->m_dV[WidthDiffRight]	= Round(toM(pArea->m_dWidthDiffRight),3);
						pBMVert->m_dV[Height]			= Round(toM(pArea->m_dHeight),3);
						pBMVert->m_dV[Length]			= Round(toM(dHeight),3);
						pBMVert->m_dwSide				= SIDE_UPPER | SIDE_LOWER | SIDE_LEFT | SIDE_RIGHT;
						pBMVert->m_sInfo				= "������ũ��Ʈ��Ǫ��";
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
					//�ո�, ����
					CTwinVectorAreaShape *pArea	= pExFooting->m_pArrAreaConcPlane.GetAt(0);
					if(!pArea) return;
						
					// ������
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
					pBMVert->m_sUnit				= "��";
					pBMVert->m_nShape				= SHAPE_RECTANGLE;
					pBMVert->m_dV[WidthLower]		= Round(toM(dWidth),3);
					pBMVert->m_dV[Height]			= Round(toM(dLength),3);
					pBMVert->m_dV[Length]			= Round(toM(dHeight),3);
					pBMVert->m_dwSide				= SIDE_UPPER | SIDE_LOWER | SIDE_LEFT | SIDE_RIGHT;
					pBMVert->m_sInfo				= "������ũ��Ʈ��Ǫ��";
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
			// slop�� 
			// �米�̰ų� Ȯ���϶� �뷫 ���.. �Ѥ�;
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

					// ������
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
					pBMVert->m_sUnit				= "��";
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

					pBMVert->m_sInfo				= "������ũ��Ʈ��Ǫ��";
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

					// ����
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
						
						pBMSlope->m_sInfo			= "������ũ��Ʈ��Ǫ��";
						pBMSlope->CalculateResult();
						m_pArrBMBridgeStd.Add(pBMSlope);
					}
				}
			}
		}
		// MASS ��ũ��Ʈ
		else if(nTypeExFooting == EXFOOTING_TYPE_MASS)
		{
			double dLenFooting	= pExFooting->m_bFrontDanMass ? pExFooting->GetLength() : pExFooting->GetWidth(TRUE);
			nSize = pExFooting->m_bFrontDanMass ?
				pExFooting->m_pArrAreaConc.GetSize() : pExFooting->m_pArrAreaConcSide.GetSize();
			long j=0; for(j=0; j<nSize; j++)
			{
				CTwinVectorAreaShape *pArea	= pExFooting->m_bFrontDanMass ?
					pExFooting->m_pArrAreaConc.GetAt(j) : pExFooting->m_pArrAreaConcSide.GetAt(j);

				// ������
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
				pBMVert->m_sUnit				= "��";
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
				
				pBMVert->m_sInfo				= "MASS��ũ��Ʈ��Ǫ��";
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

				// ����
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
					
					pBMSlope->m_sInfo			= "������ũ��Ʈ��Ǫ��";
					pBMSlope->CalculateResult();
					m_pArrBMBridgeStd.Add(pBMSlope);
				}
			}
		}
	}

	// ������ ���������� �߰��Ѵ�.
	RemakeByHeightBMList(&m_pArrBMBridgeStd, dFirstMeter, dSeconeMeter, nSizeBM, m_pArrBMBridgeStd.GetSize()-1);

	// ���������� ������ 0�̸� �����Ѵ�
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

// ��ü ����ٰ� ù��° �������� ���� ��..
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

	// ������ ��µǾ����� �ϴ� ������
	double dWidthOrg = 600;	// ���� ����ũ��

	// ���ܸ鵵�� �������� ��������!!! ////////////////////////////////////
	// �װɷ� ���ܸ�� ���õ� ������ �׷�����.
	CDomyun domTmp;
	domTmp.SetCWnd(m_pStd->m_pView);
	domTmp.SetScaleDim(1);
	domTmp.SetDimTxt(1);
	pDrawVertSection->DimVertSectionAll(&domTmp, TRUE);

	// �����ݼ�
	if(m_pStd->m_bDrawElEarth)
		pDrawVertSection->DrawElEarthLine(&domTmp,0,0,FALSE,TRUE);
	/////////////////////////////////////////////////////////////////////////
	
	// ���ܸ鵵  ////////////////////////////////////////////////////////////////////////////
	CDomyun domVertSection;
	domVertSection.SetCWnd(m_pStd->m_pView);

	// ���� ���� ////////////////////
	double dScale	= pBaseBMStd->GetScale(&domTmp, dWidthOrg);
	pBaseBMStd->SetDomyunValue(&domVertSection, dWidthOrg, &domTmp);
	double dScaleDim = toM(pBri->m_dLengthBridge)/35 * 100; //double dScaleDim = domVertSection.GetScaleDim();	// 30M������ �������� 1:100, �ּ� 100;
	domVertSection.SetScaleDim(dScaleDim);
	///////////////////////////////////

	CTwinVectorArray tvArrTmp;
	CalAreaConc(pBri, FALSE);

	// ���ʵ�
	long nCountFooting	= pBri->GetCountFooting();
	long i = 0; for(i = 0; i < nCountFooting; i++)
	{
		CFootingApp *pFooting	= pBri->GetFooting(i);
		if(pFooting)
		{
			pBaseBMStd->DrawAreaSectionNumber(&domVertSection, &pFooting->m_pArrAreaConc, 1);
			pBaseBMStd->DrawAreaSectionNumber(&domVertSection, &pFooting->m_exFooting.m_pArrAreaConc, 1);
			// ���� ǥ�� ����
			pDrawExFooting->DrawFront(&domVertSection, &pBri->m_footingStt.m_exFooting, FALSE, TRUE, &tvArrTmp, TRUE, TRUE, FALSE);
			pDrawExFooting->DrawFront(&domVertSection, &pBri->m_footingEnd.m_exFooting, FALSE, TRUE, &tvArrTmp, TRUE, TRUE, FALSE);
		}
	}
	
	// ��ü
	pBaseBMStd->DrawAreaSectionNumber(&domVertSection, &pBri->m_pArrAreaConc, 1);
	DBStd.DrawTvArray(&domVertSection, pBri->m_tvArrVertSectionRib[0]);	// ��ġ�� ����
	DBStd.DrawTvArray(&domVertSection, pBri->m_tvArrVertSectionRib[1]);
	
	// ��ü��
	long nCountInWall	= pBri->GetCountInWall();
	for(i = 0; i < nCountInWall; i++)
	{
		CWallApp *pWall	= pBri->GetInWall(i);
		if(pWall)
			pBaseBMStd->DrawAreaSectionNumber(&domVertSection, &pWall->m_pArrAreaConc, 1);
		// ���� ǥ�� ����
		pDrawExFooting->DrawFront(&domVertSection, &pWall->m_footing.m_exFooting, FALSE, TRUE, &tvArrTmp, TRUE, TRUE, FALSE);
	}

	// ġ������ 
	pDrawVertSection->DimVertSectionAll(&domVertSection, TRUE);
	DimTextArchRibConcAndForm(&domVertSection);

	// ��ü ���� ������ ǥ��
	DimTextFormOfSlabELCalc(&domVertSection);


	// �����ݼ�
	//if(m_pStd->m_bDrawElEarth)
	//	pDrawVertSection->DrawElEarthLine(&domVertSection,0,0,FALSE,TRUE);

	// ������ ����
	domVertSection.RedrawByScale(dScale, TRUE);
	domVertSection.SetCalcExtRect();
	double dWidth = dWidthOrg;//domVertSection.GetExtRect().Width();
	double dHeight = 350;//domVertSection.GetExtRect().Height();
	CDoubleArray dArrTmp;
	long nRow	= nKindBM==0 ? pBaseBMStd->InsertDomyun(5, TA_CENTER, &domVertSection, dArrTmp, dWidth, dHeight) : 0;

	// ��鵵 �׸���
	CDomyun domTmp2;
	domTmp2.SetScaleDim(1);
	domTmp2.SetDimTxt(1);
	DBStd.m_pDrawPlane->DrawPlaneScaffold(&domTmp2);

	// ������� �и� ����� ��쿡�� �����

	CDomyun domPlane;
	domPlane.SetCWnd(m_pStd->m_pView);
	dScale	= pBaseBMStd->GetScale(&domTmp2, dWidthOrg);
	pBaseBMStd->SetDomyunValue(&domPlane, dWidthOrg, &domTmp2);
	dScaleDim = toM(pBri->m_dLengthBridge)/35*100; //domPlane.GetScaleDim() * 4;	// 30M������ �������� 1:600, �ּ� 600;
	domPlane.SetScaleDim(dScaleDim);

	// ��������и����� �Ϲݼ����� ��쿡�� �����
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

				// ���� ���
				DBStd.m_pDrawPlane->DrawPlaneCenterLineEachSlab(&domPlane);
				DBStd.m_pDrawPlane->DrawPlaneSlabOnlyOutLine(&domPlane);
				
				// ��ü �� ��� ���
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
				
				// ���� ���
				if(pBri->IsBoxType())
				{
					// BOX���� ���ʾ��� Ȯ����ʸ� �����ϱ� ���⼭ Ȯ����ʸ� �׷�����.
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
				
				

				// ������ ���
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

		// �׷ȴٸ� ���ϴ� ũ��� ������ ����
		domPlane.RedrawByScale(dScale, TRUE);
		DBStd.RotateDomByBridgeSttEndXy(&domPlane);
		domPlane.SetCalcExtRect();

		// ������ ����
		//double dRate		= domPlane.GetExtRect().Width() / dWidthOrg;
		double dWidthRate	= dWidthOrg;//domPlane.GetExtRect().Width() / dRate;
		double dHeightRate	= 350;//domPlane.GetExtRect().Height() / dRate;
		CDoubleArray dArrTmp;
		nRow = pBaseBMStd->InsertDomyun(nRow+3, TA_CENTER, &domPlane, dArrTmp, dWidthRate, dHeightRate);
	}
	
	// �����鵵 �׸��� /////////////////////////////////////////////////////////
	// �Ϲݼ����̰ų�, ������������̰ų�, ������� �и����� ��������϶��� �����
	if(m_pStd->m_pARcBridgeDataStd->m_nTypeBMApply == 0 || m_pStd->m_pARcBridgeDataStd->m_nTypeBMApply == 2 ||
		(m_pStd->m_pARcBridgeDataStd->m_nTypeBMApply == 1 && nKindBM == 1))
	{
		CDomyun domPlane2;
		domPlane2.SetCWnd(m_pStd->m_pView);

		// ���� ���� ////////////////////
		pBaseBMStd->SetDomyunValue(&domPlane2, dWidthOrg, &domTmp);
		///////////////////////////////////

		// �����鵵 �׸��� -> ���� ������ ���
		DBStd.m_pDrawPlane->DrawPlaneScaffold(&domPlane2);

		// �׷ȴٸ� ���ϴ� ũ��� ������ ����
		domPlane2.RedrawByScale(dScale, TRUE);
		DBStd.RotateDomByBridgeSttEndXy(&domPlane2);
		domPlane2.SetCalcExtRect();

		// ������ ����
		double dRate	= domPlane2.GetExtRect().Width() / dWidthOrg;
		double dWidthRate	= domPlane2.GetExtRect().Width() / dRate;
		double dHeightRate	= domPlane2.GetExtRect().Height() / dRate;	
		CDoubleArray dArrTmp;
		nRow = pBaseBMStd->InsertDomyun(nRow+5, TA_CENTER, &domPlane2, dArrTmp, dWidthRate, dHeightRate);
	}

	// ������� �����̸� ù�忡 ���ٸ��� ����Ѵ�.
	if(nKindBM != 0)
	{
		// ���ٸ��� ���Ÿ��� �׸�
		//pBri->SyncBridge(0, FALSE, TRUE, FALSE);
		pBri->GetTvVertSection_Std(0, FALSE, FALSE);
		pBri->SyncBridgeAps(0, FALSE);

		// ���� ����
		CDomyun domPost;
		domPost.SetCWnd(m_pStd->m_pView);
		// ���� ���� ////////////////////
		pBaseBMStd->SetDomyunValue(&domPost, dWidthOrg, &domTmp);
		///////////////////////////////////
			

		// ���ܸ�
		pDrawVertSection->DrawVertSectionAll(&domPost, TRUE, TRUE, TRUE, FALSE, TRUE);
		if(m_pStd->m_bDrawElEarth)
			pDrawVertSection->DrawElEarthLine(&domPost,0,0,FALSE,TRUE);

		// ������ ����� ���ٸ�
		CHgBaseBMStd baseBMStd;
		long nNum	= 1;
		long stt = 0; for(stt = 0; stt < 2; stt++)
		{
			BOOL bStt = stt == iSTT ? TRUE : FALSE;
			nNum = GetTvAreaShapePostBracket(&m_pArrAreaConc, bStt, nNum);
			baseBMStd.DrawAreaSectionNumber(&domPost, &m_pArrAreaConc, 1, TRUE, 500, bStt ? 45 : 315);
		}

		// �� ������ ���ٸ�
		for(long jigan = 0; jigan < pBri->m_nQtyJigan; jigan++)
		{
			nNum = GetTvAreaShapePostJigan(&m_pArrAreaConc, jigan, nNum);
			baseBMStd.DrawAreaSectionNumber(&domPost, &m_pArrAreaConc, 1, TRUE, 500, 45);
		}

		// ġ�� ����
		DBStd.m_pDrawVertSection->DimPostForBM(&domPost);

		// �������
		DimTextPostAreaOfArch(&domPost);

		// �𵵸� WidthOrg ������� ����
		domPost.RedrawByScale(dScale);
		domPost.SetCalcExtRect();

		// ������ ����
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

// ��ȣ�� ����ٰ� ù��° �������� ���� ��..
long COutBM::DrawFirstPageGuardWall(CHgBaseBMStd *pBaseBMStd, long nHDan, long nIdx)
{
	if(!pBaseBMStd) return 5;

	CRcBridgeRebar *pBriOrg = GetRcBridgeRebar(TRUE);		// ��ȣ���� ���� �������� �۾���.
	if(!pBriOrg) return 5;

	CARcBridgeDBStandard DBStd(pBriOrg, m_pStd->m_pARoadOptionStd, m_pStd->m_pARcBridgeDataStd);
	DBStd.m_bApplyDimSettingOfBaseDom	= TRUE;
	CDrawCrossSection *pDrawCrossSection	= DBStd.m_pDrawCrossSection;
	CGuardWallRC* pGW = pBriOrg->GetGuardWallByHDan(nHDan);
	if(!pGW) return 5;
	BOOL bLeft = pGW->IsLeftGuardFence();
	// ������ ��µǾ����� �ϴ� ������
	double dWidthOrg = 100;	// ���� ����ũ��

	// ���ܸ鵵�� �������� ��������!!! ////////////////////////////////////
	// �װɷ� ���ܸ�� ���õ� ������ �׷�����.
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
	
	// ���ܸ鵵
	CDomyun domCrossSection;
	domCrossSection.SetCWnd(m_pStd->m_pView);

	// ���� ����
	double dScale	= pBaseBMStd->GetScale(&domTmp, dWidthOrg);
	pBaseBMStd->SetDomyunValue(&domCrossSection, dWidthOrg, &domTmp);
			
	

	// ��ȣ��	
	CalAreaConc(pBriOrg, FALSE);
	pBaseBMStd->DrawAreaSectionNumber(&domCrossSection, &pGW->m_pArrAreaConc, 1);

	// Dim
	
	if(_T("����") == pBriOrg->GetGuardWallName(pGW))
	{
		if(pGW->m_dSlopeUpper > 0)		bLeft = TRUE;
		else							bLeft = FALSE;
	}
	DBStd.m_pDrawCrossSection->DimGuardWall(&domCrossSection, pGW, sta, FALSE, bLeft, !bLeft);
	// ���� ǥ��
	DimTextLengthOfGuardWall(&domCrossSection, pGW, nIdx, nHDan);

	// ������ ����
	domCrossSection.RedrawByScale(dScale, TRUE);
	domCrossSection.SetCalcExtRect();

	double dWidth = dWidthOrg*2;//domCrossSection.GetExtRect().Width();
	double dHeight = dWidth;//domCrossSection.GetExtRect().Height();
	CDoubleArray dArrTmp;
	long nRow = pBaseBMStd->InsertDomyun(5, TA_CENTER, &domCrossSection, dArrTmp, dWidth, dHeight);

	// ��鵵 �׸��� /////////////////////////////////////////////////////////
	/* ��鵵 ��ȣ���׸��� ����� �����Ǹ� Ǯ ����.
	CDomyun domPlane;
	domPlane.SetCWnd(m_pStd->m_pView);
	dWidthOrg = 550;

	// ���� ���� ////////////////////
	pBaseBMStd->SetDomyunValue(&domPlane, dWidthOrg, &domTmp);
	///////////////////////////////////

	// ��鵵 �׸���
	if(pBriOrg->m_nTypeBridge!=BRIDGE_TYPE_SLAB)
	{
		CString str = pBriOrg->GetGuardWallName(pGW);
		str += _T(" ����");
		// ���� ���
		DBStd.m_pDrawPlane->DrawPlaneCenterLineEachSlab(&domPlane);
		DBStd.m_pDrawPlane->DrawPlaneSlabOnlyOutLine(&domPlane, TRUE, str);

		// ������ ���
		pBriOrg->GetTvWingWallPlane(TRUE);
		DBStd.m_pDrawWingWall->DrawGenPlane(&domPlane, TRUE);

		pBriOrg->GetTvWingWallPlane(FALSE);
		DBStd.m_pDrawWingWall->DrawGenPlane(&domPlane, FALSE);
	}

	// �׷ȴٸ� ���ϴ� ũ��� ������ ����
	domPlane.RedrawByScale(dScale, TRUE);
	DBStd.RotateDomByBridgeSttEndXy(&domPlane);
	domPlane.SetCalcExtRect();

	// ������ ����
	double dRate	= domPlane.GetExtRect().Width() / dWidthOrg;
	nRow	= pBaseBMStd->InsertDomyun(nRow+3, TA_CENTER, &domPlane, domPlane.GetExtRect().Width()/dRate, domPlane.GetExtRect().Height()/dRate);
	*/

	DBStd.m_bApplyDimSettingOfBaseDom	= FALSE;


	return nRow;
}

// �Ѱ� ���� ���� ����
long COutBM::GetLevelTotal(long nIdxBigClass)
{
	CRcBridgeRebar	*pBri	= GetRcBridgeRebar();
	if(!pBri) return 0;
	long nTypeBM	= m_pStd->m_pARcBridgeDataStd->m_nTypeBMApply;

	if(nIdxBigClass == BM_BIG_BOHOBLOCK)				return 0;		// ��ȣ��
	if(nIdxBigClass == BM_BIG_CONCRETE)					return nTypeBM == TYPE_BM_NORMAL ? 4 : 1;		// ��ũ��Ʈ
	if(nIdxBigClass == BM_BIG_FORM)						return 4;		// ��Ǫ��
	if(nIdxBigClass == BM_BIG_POST)						return 2;		// ���ٸ�
	if(nIdxBigClass == BM_BIG_DOWELBAR)				return 2;		// DowelBar
	if(nIdxBigClass == BM_BIG_SCAFFOLD)					return 0;		// ���
	if(nIdxBigClass == BM_BIG_SPACER)						return 2;		// �����̼�
	if(nIdxBigClass == BM_BIG_SLABCARE)					return 0;		// ��������
	if(nIdxBigClass == BM_BIG_CONCCARE)					return 0;		// ��ũ��Ʈ���
	if(nIdxBigClass == BM_BIG_ASPHALTPROOF)			return 0;		// �ƽ���Ʈ ���
	if(nIdxBigClass == BM_BIG_REBAR)						return 6;		// ö�ٰ�������
	if(nIdxBigClass == BM_BIG_SURFACE)					return 0;		// �����
	if(nIdxBigClass == BM_BIG_ASCON)						return 0;		// �ƽ��� ����
	if(nIdxBigClass == BM_BIG_SAND)						return 0;		//�� ����
	if(nIdxBigClass == BM_BIG_PERMEABLE)				return 0;		// ������ ����
	if(nIdxBigClass == BM_BIG_TACKCOTTING)				return 0;		// �� ����
	if(nIdxBigClass == BM_BIG_SURFACEPROOF)			return 0;		// ������
	if(nIdxBigClass == BM_BIG_ANTIDROPPING)			return 0;		// ���Ϲ� ����
	if(nIdxBigClass == BM_BIG_NAMEPANEL)				return 0;		// ������
	if(nIdxBigClass == BM_BIG_DESPANEL)					return 0;		// ������
	if(nIdxBigClass == BM_BIG_TBM)							return 0;		// TBM��ġ
	if(nIdxBigClass == BM_BIG_STYROFOAM)				return 2;		// ��Ƽ����
	if(nIdxBigClass == BM_BIG_FLEXIBLEJOINT)				return 2;		// ��������
	if(nIdxBigClass == BM_BIG_FLEXIBLEJOINT_COVER)	return 2;		// ��������
	if(nIdxBigClass == BM_BIG_SHRINKAGE)				return 1;		// ������ ��ũ��Ʈ
	if(nIdxBigClass == BM_BIG_COVERPLATE)				return 0;		// cover plate
	if(nIdxBigClass == BM_BIG_MONUMENT)				return 0;		// ������
	if(nIdxBigClass == BM_BIG_NOTCH)						return 0;		// NOTCH
	if(nIdxBigClass == BM_BIG_SCAFFOLD_LEG)			return 0;		// ���ٸ� 
	if(nIdxBigClass == BM_BIG_DECKFINISH)				return 2;		// ��ũ�Ǵϼ�

	return 0;
}

CString COutBM::GetStringItemClass(long nIdxItemClass)
{
	CRcBridgeRebar	*pBri	= GetRcBridgeRebar();
	if(!pBri) return _T("");
	long nTypeBM	= m_pStd->m_pARcBridgeDataStd->m_nTypeBMApply;

	if(nIdxItemClass == BM_ITEM_BOHOBLOCK_SF)			return "������ ��ü ����";	// ��ȣ��(��������)
	if(nIdxItemClass == BM_ITEM_BOHOBLOCK_SS)			return "������ ��ü ����";	// ��ȣ��(��������)
	if(nIdxItemClass == BM_ITEM_BOHOBLOCK_EF)			return "������ ��ü ����";	// ��ȣ��(��������)
	if(nIdxItemClass == BM_ITEM_BOHOBLOCK_ES)			return "������ ��ü ����";	// ��ȣ��(��������)
	if(nIdxItemClass == BM_ITEM_CONCRETE_FOOTING)		return nTypeBM == TYPE_BM_NORMAL ? "����" : "";	// ��ũ��Ʈ (����)
	if(nIdxItemClass == BM_ITEM_CONCRETE_MAIN)			return nTypeBM == TYPE_BM_NORMAL ? "��ü" : "";	// ��ũ��Ʈ (��ü)
	if(nIdxItemClass == BM_ITEM_CONCRETE_WING_SL)		return nTypeBM == TYPE_BM_NORMAL ? "��������" : "��������";	// ��ũ��Ʈ (��������)
	if(nIdxItemClass == BM_ITEM_CONCRETE_WING_SR)		return nTypeBM == TYPE_BM_NORMAL ? "��������" : "��������";	// ��ũ��Ʈ (��������)
	if(nIdxItemClass == BM_ITEM_CONCRETE_WING_EL)		return nTypeBM == TYPE_BM_NORMAL ? "��������" : "��������";	// ��ũ��Ʈ (��������)
	if(nIdxItemClass == BM_ITEM_CONCRETE_WING_ER)		return nTypeBM == TYPE_BM_NORMAL ? "��������" : "��������";	// ��ũ��Ʈ (��������)
	if(nIdxItemClass == BM_ITEM_CONCRETE_APS)			return "���ӽ�����";
	if(nIdxItemClass == BM_ITEM_FORM_WING_SLF)			return "��������(����)";	// ��Ǫ�� (�������� ����)
	if(nIdxItemClass == BM_ITEM_FORM_WING_SLB)			return "��������(���)";	// ��Ǫ�� (�������� ���)
	if(nIdxItemClass == BM_ITEM_FORM_WING_SRF)			return "��������(����)";	// ��Ǫ�� (�������� ����)
	if(nIdxItemClass == BM_ITEM_FORM_WING_SRB)			return "��������(���)";	// ��Ǫ�� (�������� ���)
	if(nIdxItemClass == BM_ITEM_FORM_WING_ELF)			return "��������(����)";	// ��Ǫ�� (�������� ����)
	if(nIdxItemClass == BM_ITEM_FORM_WING_ELB)			return "��������(���)";	// ��Ǫ�� (�������� ���)
	if(nIdxItemClass == BM_ITEM_FORM_WING_ERF)			return "��������(����)";	// ��Ǫ�� (�������� ����)
	if(nIdxItemClass == BM_ITEM_FORM_WING_ERB)			return "��������(���)";	// ��Ǫ�� (�������� ���)

	return _T("");
}

// ����ٰ��� ���� �߰��� ���� �Ǵ� �𵵸� �߰���Ų��.
void COutBM::SetInsertDomyunStd()
{
	SetInsertDomyunPlaneFooting();
	SetInsertDomyunCross();
	SetInsertDomyunPost();
	SetInsertDomyunScaffoldMain();
}

// ���ٸ� ���� ���ϱ�.
// ����� �ϸ�(������)
// �� ������ �⺻�������� ��� offset�� ������ �¿��� offset�� ������, offset�� �������� �� ������ �����ؾ� ��.
long COutBM::GetTvAreaShapePostBracket(CTypedPtrArray <CObArray, CTwinVectorAreaShape*> *pArrAreaConc, BOOL bStt, long nBaseNum)
{
	AhTPADelete(pArrAreaConc,	(CTwinVectorAreaShape*)0);

	CRcBridgeRebar	*pBri	= GetRcBridgeRebar();
	if(!pBri) return nBaseNum;
	
	long nNum	= nBaseNum;
	CTwinVectorArray tvArrBracket;
	pBri->GetTvPostBracket(tvArrBracket, bStt);
	// ȸ���� ��ٸ����� �����ϹǷ� ���������� ���� �ʰ� �׳� ����
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

	// ���������� ����������, ������ ���ٸ��� ��ġ�� �� ���� ������ ��� �Ϲݿ� ���ٸ��� ������ �����Ѵ�.
	if(pBri->m_nTypePost == 0 && bOK)
	{
		//AfxMessageBox(_T("������ ���ٸ��� ��ġ�� �� ���� �����̹Ƿ�, �Ϲݿ� ���ٸ��� ������ �����մϴ�."));
	}

	// �� 5������ ���� �� �ִ�.
	long i = 0; for(i = 0; i < 5; i++)
	{
		CTwinVectorArray tvArr;
		CString sInfo	= _T("");
		switch(i)
		{
			case 0: 
				tvArr = tvArrUpper;
				sInfo = _T("���");
				break;
			case 1:
				tvArr = tvArrLeft;
				sInfo = _T("����");
				break;
			case 2:
				tvArr = tvArrRight;
				sInfo = _T("����");
				break;
			case 3: 
				tvArr = tvArrMid;
				sInfo = bOK ? _T("") : _T("�⺻");		// bOK == TRUE : �Ϲݿ�
				break;
			case 4:
				tvArr = tvArrArch;
				sInfo = bOK ? _T("��ġR�Ϲ�") : _T("��ġR����");
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

						CString str = _T("��ġ");
						CTwinVectorAreaShape *pArea	= new CTwinVectorAreaShape;
						pArea->m_tvArr  = tvArrMid2;
						pArea->m_nNum	= nNum++;
						pArea->m_sInfo	= str;
						pArea->m_nTypeShape = SHAPE_TV_FAN;
						pArea->m_xyCen = pBri->GetCentroid(vArrRib, nSize);
						pArea->m_dArcAreaL = pArea->m_dArcAreaR = pBri->GetPolygonArea(vArrRib, nSize);
						pArrAreaConc->Add(pArea);
					}
					
					if(nJigan==0)	// ������ ������� �ѹ��� ����.
					{
						CDPointArray vArr;
						long nSize = tvArr.GetSize();
						long n=0; for(n=nSize-1; n>=0; n--)
							vArr.Add(tvArr.GetAt(n).m_v1);

						rb.RemoveSamePoint(vArr);

						CString str = _T("��ġ");
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
					
					// ��ȣ ���� �ڸ� xyCen
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

	// ���ٸ��� ���Ÿ��� �𵵸� �׸��� ���Ÿ� �ʺ� �����
	//pBri->SyncBridge(0, FALSE, TRUE, FALSE);
	pBri->GetTvVertSection_Std(0, FALSE, FALSE);
	pBri->SyncBridgeAps(0, FALSE);
	CDPoint vAng = pBri->GetAngleJijum(0);

	// ��,���� �����
	// ����� �ʺ�� ���Ÿ��� �̰�, ���̴� ��Ÿ��� ����
	long nNum	= 1;
	long stt = 0; for(stt = 0; stt < 2; stt++)
	{
		BOOL bStt = stt == iSTT ? TRUE : FALSE;

		// ����� ���� ���ϱ�
		pBri->GetTvWingWallPlane(bStt);
		CTwinVector tvBracket	= pBri->m_tvArrPlaneWingWall.GetTvByInfo("�����");

		nNum = GetTvAreaShapePostBracket(&m_pArrAreaConc, bStt, nNum);	// ����� ���� ���ϱ�.
		long nSize	= m_pArrAreaConc.GetSize();
		long i = 0; for(i = 0; i < nSize; i++)
		{
			CTwinVectorAreaShape *pArea	= m_pArrAreaConc.GetAt(i);

			// ������
			CBMData *pBM				= new CBMData;
			pBM->m_nNumbering			= pArea->m_nNum;
			pBM->m_nIdxBigClass			= BM_BIG_POST;
			pBM->m_sBigClass			= GetStringBigClass(pBM->m_nIdxBigClass);
			pBM->m_nLevelTotal			= GetLevelTotal(pBM->m_nIdxBigClass); 
			pBM->m_nIdxSmallClass		= BM_SMALL_POST_STEELPIPE;
			pBM->m_sSmallClass			= GetStringSmallClass(pBM->m_nIdxSmallClass);
			pBM->m_nTypeResult			= RESULT_VOLUMN;
			pBM->m_sUnit				= "��";
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
			// �������� ��¾��
			pBM->m_nLevelMidClass		= 2;
			m_pArrBMBridgeStd.Add(pBM);
		}
	}

	// �� ����
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
			if(pBri->m_nTypePost == TYPE_POST_BRIDGE)	// ������ ���ٸ�
			{
				if(pArea->m_sInfo.Find("���") != -1 || pArea->m_sInfo.Find("��ġR����") != -1)
					pBM->m_nIdxSmallClass	= BM_SMALL_POST_WOOD;
				else if(pArea->m_sInfo.Find("����") != -1 || pArea->m_sInfo.Find("����") != -1)
					pBM->m_nIdxSmallClass	= BM_SMALL_POST_STEELPIPE;
				else if(pArea->m_sInfo.Find("�⺻") != -1)
					pBM->m_nIdxSmallClass	= BM_SMALL_POST_BRIDGE_STEEL;
				else
					pBM->m_nIdxSmallClass	= BM_SMALL_POST_STEELPIPE;
			}
			else	// �Ϲ�(����) ���ٸ�
			{
				pBM->m_nIdxSmallClass = BM_SMALL_POST_STEELPIPE;
			}
			pBM->m_sSmallClass = GetStringSmallClass(pBM->m_nIdxSmallClass);
			
			double dLength	= toM(pBri->GetWidthSlabAct(pBri->m_dStationBridgeStt, vAng, 0, FALSE));
			if(pBM->m_nIdxSmallClass	== BM_SMALL_POST_BRIDGE_STEEL)
			{
				pBM->m_nTypeResult			= RESULT_LENGTH;
				pBM->m_sUnit				= "m";
				pBM->m_sItemClass			= "8 x 4.5";//"8 x 4"; 2006����� 4.5�� ����
				pBM->m_sForm.Format("%.3f", dLength);
				pBM->m_bIsPrintByRefForm	= TRUE;
				pBM->SetResult(dLength);
			}
			else
			{
				if(pArea->m_sInfo.Find("��ġR") != -1 || pArea->m_sInfo.Find("��ġ") != -1)
				{
					pBM->m_nTypeResult			= RESULT_VOLUMN;
					pBM->m_sUnit				= "��";
					pBM->m_nShape				= pArea->m_nTypeShape;
					pBM->m_dV[Width]			= Round(toM(pArea->m_dWidth),3);			// ��ä�� ���� ����
					pBM->m_dV[Height]			= Round(toM(toM(pArea->m_dArcAreaL)),3);	// ����
					pBM->m_dV[Length]			= Round(dLength,3);							
					pBM->CalculateResult();
				}
				else
				{
					pBM->m_nTypeResult			= RESULT_VOLUMN;
					pBM->m_sUnit				= "��";
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
			// �������� ��¾��
			pBM->m_nLevelMidClass		= 2;
			m_pArrBMBridgeStd.Add(pBM);
		}
	}
	pBri->GetTvVertSection_Std(0, FALSE, FALSE, TRUE);
}

//��ũ�Ǵϼ� (�ݵ�� ��ü ���ٸ� ���� ���� ����)
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
		pBM->m_sUnit					= "��";
		pBM->m_sForm					= sVal;
		pBM->m_sFormExcel			= pBM->m_sFormForCompare = pBM->m_sForm;
		pBM->SetResult(dResult);

		pBM->m_nLevelMidClass		= 2;
		m_pArrBMBridgeStd.Add(pBM);	
	}	
}

// Ⱦ�� �� �׷� �ֱ�
// MASS ��ũ��Ʈ�̸鼭 ���������϶�
void COutBM::SetInsertDomyunCross()
{
	CRcBridgeRebar	*pBri	= GetRcBridgeRebar();
	if(!pBri) return;
	
	CHgBaseBMStd baseBMStd;
	CARcBridgeDBStandard DBStd(pBri, m_pStd->m_pARoadOptionStd, m_pStd->m_pARcBridgeDataStd);
	DBStd.m_bApplyDimSettingOfBaseDom	= TRUE;
	CDrawCrossSection *pDrawCrossSection	= DBStd.m_pDrawCrossSection;
	double dSta = pBri->GetStationOnJijum(0,0);

	// ������������ ////////
	BOOL bDrawRubble = pBri->IsBoxType() && pBri->m_dThickCobbStone > 0 && pBri->m_nTypeLower == LOWERSLAB_TYPE_COBBSTONE;

	CDomyun domTmp;
	domTmp.SetCWnd(m_pStd->m_pView);
	pDrawCrossSection->DrawCrossSection(&domTmp, dSta, CDPoint(0,1), FALSE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, FALSE, TRUE, 0, -1, TRUE, bDrawRubble);
	
	// ��ũ��Ʈ ����� ���� �������� �������°� ���������� ���̴°� �߰�
	CDomyun domCrossTot;
	domCrossTot.SetCWnd(m_pStd->m_pView);

	double dWidthOrg	= 350;

	// ���� ���� ////////////////////
	double dScale	= baseBMStd.GetScale(&domTmp, dWidthOrg);
	baseBMStd.SetDomyunValue(&domCrossTot, dWidthOrg, &domTmp);

	// Ⱦ�ܸ�
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
			DBStd.m_pDrawCrossSection->DimCrossBottom(&domCross, nJ, FALSE);	// �ϸ� ġ��
			
			// ����
			CString str	= _T("");
			str.Format("����%d ����", nJ+1);
			domCross.SetCalcExtRect();
			CDRect rcPlane	= domCross.GetExtRect();
			domCross.SetTextAlignHorz(TA_CENTER);
			domCross.TextOut(CDPoint(rcPlane.left+rcPlane.Width()/2, rcPlane.bottom+10), str);

			// �𵵸� WidthOrg ������� ����
			domCross.RedrawByScale(dScale);
			
			// �������� �Ʒ������� �̵�
			domCross.SetCalcExtRect();
			rcPlane	= domCross.GetExtRect();
			domCross.Move(CDPoint(rect.left-rcPlane.left , rect.top - 10 - rcPlane.bottom));

			domCrossTot << domCross;
		}
	}

	domCrossTot.SetCalcExtRect();
	
	// MASS ��ũ��Ʈ ������ ����
	long i = 0; for(i = 0; i < m_pArrBMStringBridge.GetSize(); i++)
	{
		CBMString *pBMString	= m_pArrBMStringBridge.GetAt(i);
		if(pBMString->m_sProcessKind1.Find("��ũ��Ʈ", 0) != -1 && pBMString->m_sForm.Find("MASS", 0) != -1)
		{
			pBMString->m_Dom	= domCrossTot;
			pBMString->m_bInsertDomyun	= TRUE;
			break;
		}
	}

	DBStd.m_bApplyDimSettingOfBaseDom	= FALSE;
}

// ���ٸ� �� �׷� �ֱ�
void COutBM::SetInsertDomyunPost()
{
	CRcBridgeRebar	*pBri	= GetRcBridgeRebar();
	if(!pBri) return;

	CHgBaseBMStd baseBMStd;
	CARcBridgeDBStandard DBStd(pBri, m_pStd->m_pARoadOptionStd, m_pStd->m_pARcBridgeDataStd);
	DBStd.m_bApplyDimSettingOfBaseDom	= TRUE;
	CDrawVertSection *pDrawVertSection	= DBStd.m_pDrawVertSection;

	// ���ٸ��� ���Ÿ��� �׸�
	//pBri->SyncBridge(0, FALSE, TRUE, FALSE);
	pBri->GetTvVertSection_Std(0, FALSE, FALSE);
	pBri->SyncBridgeAps(0, FALSE);

	long stt = 0;
	long nNum	= 1;

	// ������������
	CDomyun domTmp;
	domTmp.SetCWnd(m_pStd->m_pView);
	
	pDrawVertSection->DrawVertSectionAll(&domTmp, FALSE, TRUE, TRUE, FALSE, TRUE);
	if(m_pStd->m_bDrawElEarth)
		pDrawVertSection->DrawElEarthLine(&domTmp,0,0,FALSE,TRUE);
	
	// ���� ����
	CDomyun domPost;
	domPost.SetCWnd(m_pStd->m_pView);
	double dWidthOrg = 600;
	double dScale = baseBMStd.GetScale(&domTmp, dWidthOrg);
	baseBMStd.SetDomyunValue(&domPost, dWidthOrg, &domTmp);
	
	// ���ܸ�
	pDrawVertSection->DrawVertSectionAll(&domPost, TRUE, TRUE, TRUE, FALSE, TRUE);	
	if(m_pStd->m_bDrawElEarth)
		pDrawVertSection->DrawElEarthLine(&domPost,0,0,FALSE,TRUE);

	// ������ ����� ���ٸ�
	for(stt = 0; stt < 2; stt++)
	{
		BOOL bStt = stt == iSTT ? TRUE : FALSE;
		nNum = GetTvAreaShapePostBracket(&m_pArrAreaConc, bStt, nNum);
		baseBMStd.DrawAreaSectionNumber(&domPost, &m_pArrAreaConc, 1, TRUE, 500, bStt ? 45 : 315);
	}

	// �� ������ ���ٸ�
	for(long jigan = 0; jigan < pBri->m_nQtyJigan; jigan++)
	{
		nNum = GetTvAreaShapePostJigan(&m_pArrAreaConc, jigan, nNum);
		baseBMStd.DrawAreaSectionNumber(&domPost, &m_pArrAreaConc, 1, TRUE, 500, 45);
	}

	// ġ�� ����
	DBStd.m_pDrawVertSection->DimPostForBM(&domPost);

	// �������
	DimTextPostAreaOfArch(&domPost);

	// �𵵸� WidthOrg ������� ����
	domPost.RedrawByScale(dScale);
	domPost.SetCalcExtRect();
	
	// ���ٸ����� ������ ����
	// ������� �����̸� �̹� ù�忡 ���Ƿ� ��� �߰��� �� �ʿ� ����
	if(m_dwCalBM != COMMON_BM)
	{
		long i = 0; for(i = 0; i < m_pArrBMStringBridge.GetSize(); i++)
		{
			CBMString *pBMString	= m_pArrBMStringBridge.GetAt(i);
			if(pBMString->m_sProcessKind1.Find("���ٸ�", 0) != -1)
			{
				pBMString->m_Dom	= domPost;
				pBMString->m_bInsertDomyun	= TRUE;
				break;
			}
		}
	}

	// ������ ���ٸ� �� ����
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

			// ���� ���� ////////////////////
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

			// �𵵸� WidthOrg ������� ����
			domWing.RedrawByScale(dScale);
			domWing.SetCalcExtRect();
			
			// �������� ���ٸ� ���� ������ ����
			CTypedPtrArray <CObArray, CBMString*> *pArrBMString	= &m_pArrBMStringWing[stt][left];

			// ���ٸ����� ������ ����
			long i = 0; for(i = 0; i < pArrBMString->GetSize(); i++)
			{
				CBMString *pBMString	= pArrBMString->GetAt(i);
				if(pBMString->m_sProcessKind1.Find("���ٸ�", 0) != -1)
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

// ���� ��鵵 �� �׷� �ֱ�
void COutBM::SetInsertDomyunPlaneFooting()
{
	CRcBridgeRebar	*pBri	= GetRcBridgeRebar();
	if(!pBri) return;

	CHgBaseBMStd baseBMStd;
	CARcBridgeDBStandard DBStd(pBri, m_pStd->m_pARoadOptionStd, m_pStd->m_pARcBridgeDataStd);
	DBStd.m_bApplyDimSettingOfBaseDom	= TRUE;

	// ��ũ��Ʈ ����� ���� �������� �������°� ���������� ���̴°� �߰�
	// ������������ ///////////
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

	// ���� ���� ////////////////////
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

			// ����
			CString str	= _T("");
			str.Format("����%d ���� ���", i+1);
			domFootingPlane.SetCalcExtRect();
			CDRect rcPlane	= domFootingPlane.GetExtRect();
			domFootingPlane.SetTextAlignHorz(TA_CENTER);
			domFootingPlane.TextOut(CDPoint(rcPlane.left+rcPlane.Width()/2, rcPlane.bottom+10), str);
			
			// �������� �Ʒ������� �̵�
			domFootingPlane.SetCalcExtRect();
			rcPlane	= domFootingPlane.GetExtRect();
			domFootingPlane.Move(CDPoint(rect.left-rcPlane.left , rect.top - 10 - rcPlane.bottom));

			

			domFootingPlaneTot << domFootingPlane;
		}
	}


	// �𵵸� WidthOrg ������� ����
	domFootingPlaneTot.RedrawByScale(dScale);

	domFootingPlaneTot.SetCalcExtRect();

	// ������ũ��Ʈ��� ������ ù��° ������ �߰�
	BOOL bInputFootingPlane	= FALSE;
	for(i = 0; i < m_pArrBMStringBridge.GetSize(); i++)
	{
		CBMString *pBMString	= m_pArrBMStringBridge.GetAt(i);
		if(pBMString->m_sInfo.Find("������ũ��Ʈ", 0) != -1)
		{
			pBMString->m_Dom = domFootingPlaneTot;
			pBMString->m_bInsertDomyun	= TRUE;
			bInputFootingPlane	= TRUE;
			break;
		}
	}

	// ���ʰ�Ǫ���̶�� ������ ù��° ������ �߰�
	// ��, ��ũ��Ʈ�� ����ϴ� ��쿡�� �߰� ���� �ʴ´�.
	if(!bInputFootingPlane)
	{
		for(i = 0; i < m_pArrBMStringBridge.GetSize(); i++)
		{
			CBMString *pBMString	= m_pArrBMStringBridge.GetAt(i);
			if(pBMString->m_sInfo.Find("���ʰ�Ǫ��", 0) != -1)
			{
				pBMString->m_Dom = domFootingPlaneTot;
				pBMString->m_bInsertDomyun	= TRUE;
				break;
			}
		}
	}

	DBStd.m_bApplyDimSettingOfBaseDom	= FALSE;
}

// ��ũ��Ʈ ����(������)
// long nForUse : 0 : ���� �뵵(��ũ��Ʈ ����)  
//                1 : ��ä��� ������ ���� : ��ä�� ������ ������ �κи� ����
//                2 : �Ǹ޿���� ��ä��� ������ ���� : ���ݼ������� ��ä�� ������ ������ �κи� ����
//                3 (���������) : ��� ���Ϻ�(�Ϻ��Ѱ�� ���ݼ�)
//                4 (��ü�������) : ��ü ���Ϻ�(�Ϻ��Ѱ�� ���ݼ�)
//				  6 : �Ǹ޿���� ��ü ����
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
			CString sleft = (bLeft)? _T("(��)") : _T("(��)");
			// ��������� ���ϴ� ��� ��������� �߰��Ѵ�.
			// �� �ڼ��� ������ �� �Լ��� ȣ�� �ϴ� ������ ������ �� ��� ��.
			CTypedPtrArray <CObArray, CBMData*> *pArrBMStd	= nForUse == 0 ? &m_pArrBMWingStd[stt][left] : &m_pStd->m_pArrOutBMTogong.GetAt(m_nBri)->m_pArrBMTogongStd[nJ];

			long nIdxItemClass	= bStt ? (bLeft ? BM_ITEM_CONCRETE_WING_SL : BM_ITEM_CONCRETE_WING_SR) : 
										 (bLeft ? BM_ITEM_CONCRETE_WING_EL : BM_ITEM_CONCRETE_WING_ER);
			CWingWall *pWing		= pBri->GetWingWall(bStt, bLeft);

			CRebarPlacing rb;
			double dThickWingWall	= pWing->m_nDirHunch == 1 ? pWing->m_dT2 : pWing->m_dT1;
			CBMData *pBMBase		= NULL;	// ��ġ�� ö�� �����Ҷ� �⺻�Ǵ� ������
			nSize	= pWing->m_pArrAreaConc.GetSize();
			for(i = 0; i < nSize; i++)
			{
				// ���� ����.. �׸��� 
				CTwinVectorAreaShape *pArea	= pWing->m_pArrAreaConc.GetAt(i);

				// �ܸ麯ȭ���� ������  �������� ������ �β��� dT1�� �����Ѵ�.
				if(pArea->GetTvLower().m_sInfo.Find("�ܸ麯ȭ��", 0) != -1)
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
					pBM->m_sPartClass	    	= bStt ? "����" : "����";
				}
				pBM->m_nIdxSmallClass		= BM_SMALL_CONCRETE_WING;
				pBM->m_sSmallClass			= GetStringSmallClass(pBM->m_nIdxSmallClass);
				pBM->m_nIdxItemClass		= nIdxItemClass;
				pBM->m_sItemClass			= GetStringItemClass(pBM->m_nIdxItemClass);
				pBM->m_nTypeResult			= RESULT_VOLUMN;
				pBM->m_sUnit				= "��";
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

			// 3. ��ü�� ��ġ�� �κ��� ��ġ�δ� ���������� �ѹ��� �����.
			// �� �ڵ�� ��Ǫ������ ���ͼ� ������ ������
			// ���� ��ũ��Ʈ�� ���ͺ��� �ڸ��� ������, ���� �ڸ��°� �����ϰ� �Ǹ� ��Ǫ���� �ڵ带 �����ؾ���.
			CTwinVectorArray tvArrWing;
			int se = bStt ? -1 : 1;
			int nLeft = bLeft ? -se : se;	// ������ ��� ���� ���� ������ �¿�� �������� �¿찡 �ݴ���.
			pBri->GetTvVertSection(nLeft, TRUE, TRUE);
			pBri->GetTvWingWallFront(bStt, bLeft, 1, tvArrWing, TRUE, FALSE, TRUE, TRUE);
			pBri->GetTvWingWallPlane(bStt);
			CTwinVector tvHunch	= pBri->m_tvArrPlaneWingWall.GetTvByInfo(bLeft ? "����������������ġ" : "����������������ġ");
			CTwinVectorArray tvArrHunch;
			for(long h = 0; h < tvArrWing.GetSize(); h++)
			{
				tv	= tvArrWing.GetAt(h).Sort(FALSE);
				if(tv.m_sInfo == "��ü")
					tvArrHunch.Add(tv);
			}

			// ���� ������
			// ������� ���� �ΰ��� ���� �� ����
			if(pBMBase && pWing->m_dHuH2 > 0 && pWing->m_dHuW2 > 0)
			{
				CBMData *pBMVert	= new CBMData(pBMBase);
				pBMVert->m_dV[Result] = 0;		// ���� ����ִ� ��찡 �־� �ʱ�ȭ ��Ų��.

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
					double dAreaHunch	= Round(bm.GetArea8(WidthLower, WidthUpper, Height, nFRONT),3);	// nFRONTȤ�� nBACK��
					
					pBMVert->m_nNumbering			= 99999;	// 99999�� �ѹ������� ������ �ȵǰ� �����Ҷ��� ���������� ���ߵǴ� ��� �����
					str	= pBMVert->m_sForm;
					pBMVert->m_sForm.Format("(%s) x %.3f", str, dAreaHunch);
					pBMVert->m_sFormExcel = pBMVert->m_sFormForCompare = pBMVert->m_sForm;
					pBMVert->m_nShape	= SHAPE_NONE;
					pBMVert->m_dV[Result] *= Round(dAreaHunch,3);
					pBMVert->m_sNoteHead	= "��ġ��";
					pArrBMStd->Add(pBMVert);
				}
				else 
					delete pBMVert;
			}

			for(i = 0; i < nSize; i++)
			{
				// ���� ����.. �׸��� 
				CTwinVectorAreaShape *pArea	= pWing->m_pArrAreaConc.GetAt(i);

				// �ܸ麯ȭ���� ������  �������� ������ �β��� dT1�� �����Ѵ�.
				if(pArea->GetTvLower().m_sInfo.Find("�ܸ麯ȭ��", 0) != -1)
				{
					dThickWingWall	= pWing->m_dT1;
				}

				CBMData *pBM				= new CBMData(pBMBase);	// ��� ����ҷ��� ����
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
				pBM->m_sUnit				= "��";

				// 1. ������ ���� �ϴ�(���ʿ� ��ġ�� �κ� ��ġ�� ���� ��ũ��Ʈ �߰�)
				if(pArea->m_tvArr.GetTvByInfo("�����������ϴ�", tv))
				{
					CBMData *pBMHunch			= new CBMData(pBM);
					pBMHunch->m_nNumbering		= 99999;	// 99999�� �ѹ������� ������ �ȵǰ� �����Ҷ��� ���������� ���ߵǴ� ��� �����
					pBMHunch->m_sNoteHead		= "��ġ��";
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

				// 2. �ܸ麯ȭ���� �ִ� ��� ���⿡ ���ؼ� T2->T1���� ��ȭ�Ǵ� ��ġ�ο� ���� ��ũ��Ʈ �� �߰�
				if(pArea->GetTvLower().m_sInfo.Find("�ܸ麯ȭ��", 0) != -1)
				{
					CTwinVectorArray tvArrSec;
					pBri->GetTvWingWallSection(bStt, bLeft, tvArrSec);
					CTwinVector tv	= tvArrSec.GetTvByInfo("�������ܸ鳻����ġ");
					if(tv.GetHorLength() > 0 && tv.GetVerLength() > 0)
					{
						CBMData *pBMHunch			= new CBMData(pBM);
						pBMHunch->m_nNumbering		= 99999;	// 99999�� �ѹ������� ������ �ȵǰ� �����Ҷ��� ���������� ���ߵǴ� ��� �����
						pBMHunch->m_sNoteHead		= "��ġ��";
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
				delete pBM;	// ����
			}
		}
	}

	pBri->m_tvArrVertSection	= tvArrVertSectionOld;
	pBri->m_nTypeJongdanForRebar	= nTypeJongdan;
	pBri->m_nTypeJongdanForRebarOld	= nTypeJongdanOld;
}

// ������ ����ٰ� ù ������ �� ����
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
	
	// �������� �������� ��������!!! ////////////////////////////////////
	CDomyun domTmp;
	domTmp.SetCWnd(m_pStd->m_pView);
	domTmp.SetScaleDim(1);
	pDrawWingWall->DrawGenFront(&domTmp, bStt, bLeft);

	// ������ ��µǾ����� �ϴ� ������
	double dWidthOrg = 300;	// ���� ����ũ��
	/////////////////////////////////////////////////////////////////////////

	// ������ �ܸ� �׸���
	CDomyun domWing;
	domWing.SetCWnd(m_pStd->m_pView);

	// ���� ���� ////////////////////
	double dScale	= pBaseBMStd->GetScale(&domTmp, dWidthOrg);
	pBaseBMStd->SetDomyunValue(&domWing, dWidthOrg, &domTmp);
	///////////////////////////////////
	
	// ������ �¿���
	CString sTitle	= _T("");
	sTitle.Format("%s ������(%s)", bStt ? "������" : "������", bLeft ? "��" : "��");

	pBaseBMStd->DrawAreaSectionNumber(&domWing, &pWing->m_pArrAreaConc);
	pBaseBMStd->DimAreaSection(&domWing, &pWing->m_pArrAreaConc, 1, TRUE, CDPoint(0, 0));
	pBaseBMStd->DimAreaSection(&domWing, &pWing->m_pArrAreaConc, bStt ? 2 : 0, TRUE, CDPoint(0, 0));

	// �𵵸� WidthOrg ������� ����
	domWing.RedrawByScale(dScale, TRUE);

	domWing.SetCalcExtRect();
	double dWidth	= 300;
	double dHeight	= 600;
	//long nRow	= pBaseBMStd->InsertDomyun(5, TA_CENTER, &domWing, domWing.GetExtRect().Width(), domWing.GetExtRect().Height(), sTitle);
	CDoubleArray dArrTmp;
	pBaseBMStd->InsertDomyun(5, TA_CENTER, &domWing, dArrTmp, dWidth, dHeight, sTitle);

	/*
	// �����鵵 �׸��� /////////////////////////////////////////////////////////
	// ���ܸ鵵�� �������� ��������!!! ////////////////////////////////////
	// �װɷ� ���ܸ�� ���õ� ������ �׷�����.
	CDomyun domTmp2;
	domTmp2.SetCWnd(m_pStd->m_pView);
	domTmp2.SetScaleDim(1);
	domTmp2.SetDimTxt(1);
	pDrawVertSection->DimVertSectionAll(&domTmp2, TRUE);
	// �����ݼ�
	//if(m_pStd->m_bDrawElEarth)
	//	pDrawVertSection->DrawElEarthLine(&domTmp2, 0,0,FALSE,TRUE);
	////////////////////////////////////////////////

	dWidthOrg		= 500;
	CDomyun domPlane;
	domPlane.SetCWnd(m_pStd->m_pView);

	// ���� ���� ////////////////////
	dScale	= pBaseBMStd->GetScale(&domTmp2, dWidthOrg);
	pBaseBMStd->SetDomyunValue(&domPlane, dWidthOrg, &domTmp2);
	///////////////////////////////////

	// �����鵵 �׸���
	DBStd.m_pDrawPlane->DrawPlaneScaffold(&domPlane);

	// �𵵸� WidthOrg ������� ����
	domPlane.RedrawByScale(dScale, TRUE);
	DBStd.RotateDomByBridgeSttEndXy(&domPlane);
	
	// ������ ����
	domPlane.SetCalcExtRect();
	pBaseBMStd->InsertDomyun(nRow+3, TA_CENTER, &domPlane, domPlane.GetExtRect().Width(), domPlane.GetExtRect().Height());
	*/

	DBStd.m_bApplyDimSettingOfBaseDom	= FALSE;
}


// �ؿ��� ���� ������ �ڸ���.
// ���������� ��Ÿ���� �Ǵ� ������ ���ؼ� �������� ������ �����ؼ� �־��ش�.
// �ڸ��°� ���� ���ϸ鼭 �̹� �� �߶���
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
	sArr.Add("19m �̻�");

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
				if(pBM->m_bSlab && nOpt > 0)		// �Ѱ��� �������� ����ڰ� ������ ��ġ��
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

// �������� ����ϴ� ������ �ڸ��� ��� ���������� ����� ��� �ȴ�.
BOOL COutBM::IsPrintMeter(CBMData *pBM)
{
	if(pBM->m_nIdxBigClass == BM_BIG_FORM) return TRUE;

	return FALSE;
}

// �������� ������ ù��° �ڸ� ���̿� �� ���� �ڸ� ���� ������.
void COutBM::GetInfoMeter(CBMData *pBM, double &dFirstMeter, double &dSecondMeter)
{
	if(pBM->m_nIdxBigClass == BM_BIG_FORM)
	{
		dFirstMeter		= 7;
		dSecondMeter	= 3;
	}
}

// ���ٸ� ���򿬰���
// ���ٸ� �������� �̹� ��ü ���� ������ ��� ����.
void COutBM::MakeBMHoriConnectMain()
{
	// ���ٸ� ���� ������ ��ġ ����
	// ���� 3.5���Ϳ� ��ġ�ϰ�, ���� 2���� ���� ��ġ�Ѵ�.
	double dFirstHeight		= 3500;
	double dSecondHeight	= 2000;

	CTwinVector tvLeft, tvRight;
	CDPoint xyMatchLeft(0, 0), xyMatchRight(0, 0);
	BOOL bFirst	= TRUE;
	double dYBase	= 0;
	double dYCur	= 0;

	CRcBridgeRebar	*pBri	= GetRcBridgeRebar();
	if(!pBri) return;

	// ���򿬰��絵 ���Ÿ���
	pBri->GetTvVertSection_Std(0, FALSE, FALSE);
	
	CDPoint vAng = pBri->GetAngleJijum(0);
	double dWidth	= pBri->GetWidthSlabAct(pBri->m_dStationBridgeStt, vAng, 0, FALSE);

	// ��,���� �����
	CTwinVectorArray tvArr, tvArrCen;
	
	long nNum	= 1;
	long nNumLast = nNum;
	long stt = 0; for(stt = 0; stt < 2; stt++)
	{
		BOOL bStt = stt == iSTT ? TRUE : FALSE;
		pBri->GetTvScaffoldPlane(tvArr, tvArrCen, bStt);	// �ǰ����� ������ ���� ���� ����
		nNum	= GetTvAreaShapePostBracket(&m_pArrAreaConc, bStt, nNum);	// ����� ���� ���ϱ�.
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

				// ������ �¿��� ������ ã��
				GetXyMatchLineAndLine(tvLeft.m_v1, tvLeft.GetXyDir(), CDPoint(0, dYCur), CDPoint(1, 0), xyMatchLeft);
				GetXyMatchLineAndLine(tvRight.m_v1, tvRight.GetXyDir(), CDPoint(0, dYCur), CDPoint(1, 0), xyMatchRight);

				CBMData *pBM				= new CBMData;
				pBM->m_nNumbering			= pArea->m_nNum;
				pBM->m_nIdxBigClass			= BM_BIG_POST;
				pBM->m_sBigClass			= GetStringBigClass(pBM->m_nIdxBigClass);
				pBM->m_nLevelTotal			= GetLevelTotal(pBM->m_nIdxBigClass); 
				pBM->m_nIdxSmallClass		= BM_SMALL_POST_HORI_CONNECT;
				pBM->m_sSmallClass			= GetStringSmallClass(pBM->m_nIdxSmallClass);
				pBM->m_sNoteHead.Format("���򿬰���(h=%.1f�ʰ��� ���� %.1fm���� ��ġ)", toM(dFirstHeight), toM(dSecondHeight));
				pBM->m_nTypeResult			= RESULT_AREA;
				pBM->m_sUnit				= "��";
				pBM->m_nShape				= SHAPE_TV_RECTANGLE;
				pBM->m_dV[WidthLower]		= Round(toM(fabs(xyMatchRight.x-xyMatchLeft.x)),3);
				pBM->m_dV[Length]			= Round(toM(tvArrCen.GetLength()), 3);//pBri->GetWidthSlabAct(pBri->m_dStationBridgeStt, vAng, 0, FALSE)),3);
				pBM->m_dwSide				= SIDE_LOWER;
				pBM->CalculateResult();
				// �������� ��¾��
				pBM->m_nLevelMidClass		= 2;
				dPrevH = dYCur;
				m_pArrBMBridgeStd.Add(pBM);
			}
		}
	}
	nNumLast = nNum;

	// �� ����
	CRebarPlacing rb;
	
	CTwinVectorArray tvArrUpper, tvArrLeft, tvArrRight, tvArrMid, tvArrArch, tvArrMid2;
	// ��ġ�� �ִ��� �����긦 ���� ��� �ϹǷ� ������ ������ ������ �Ѵ�.
	

	for(long jigan = 0; jigan < pBri->m_nQtyJigan; jigan++)
	{
		nNum	= GetTvAreaShapePostJigan(&m_pArrAreaConc, jigan, nNum);
		pBri->GetTvPostJigan(tvArrUpper, tvArrLeft, tvArrRight, tvArrMid, tvArrArch, tvArrMid2, jigan);

		//1. ����� ���ٸ��� �ִٸ� �������� ����
		BOOL bPostWood = FALSE;
		for (long nIdx =0; nIdx < m_pArrAreaConc.GetSize(); nIdx++)
		{
			CTwinVectorAreaShape *pArea	= m_pArrAreaConc.GetAt(nIdx);
			if(pArea == NULL) continue;
			if(pArea->m_tvArr.GetSize() == 0) continue;

			if(pArea->m_sInfo.Find("���") != -1 || pArea->m_sInfo.Find("��ġR����") != -1)
			{
				bPostWood = TRUE;
				break;
			}
		}

		//2.��ν����� Numüũ
		long nNumSlab = -1;
		for (long nIdx =0; nIdx < pBri->m_pArrAreaConc.GetSize(); nIdx++)
		{
			CTwinVectorAreaShape *pAreaConc = 	pBri->m_pArrAreaConc.GetAt(nIdx);
			if(pAreaConc == NULL) continue;
			if(pAreaConc->m_tvArr.GetSize() == 0) continue;

			if(pAreaConc->m_sInfo.Find(_T("��ν�����") )> -1)
			{
				nNumSlab = pAreaConc->m_nNum;
				break;
			}			
		}

		if(bPostWood || nNumSlab == -1) 
			continue;
		if(tvArrMid.GetSize() == 0)
			continue;
		
		//3. �ش� �ְ� EL ���� EL ����
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
			pBM->m_sNoteHead.Format("���򿬰���(h=%.1f�ʰ��� ���� %.1fm���� ��ġ)", toM(dFirstHeight), toM(dSecondHeight));
			pBM->m_nTypeResult			= RESULT_AREA;
			pBM->m_sUnit				= "��";
			pBM->m_nShape				= SHAPE_TV_RECTANGLE;
			pBM->m_dV[WidthLower]		= Round(toM(dLenSlab),3);
			pBM->m_dV[Length]			= Round(toM(dWidth),3);
			pBM->m_dwSide				= SIDE_LOWER;
			pBM->CalculateResult();
			// �������� ��¾��
			pBM->m_nLevelMidClass		= 2;
			m_pArrBMBridgeStd.Add(pBM);

			if(nCount > 40 || dHeight <=0)
				break;
			nCount++;
		}
	}

	//ARCBRIDGE-3141 �߰� ���ٸ� Ÿ���� �������� ��� �� ������ ������ �߰��� �ش�.
	if(pBri->m_nTypePost == 0)
	{
		for(long jigan = 0; jigan < pBri->m_nQtyJigan; jigan++)
		{
			nNum	= GetTvAreaShapePostJigan(&m_pArrAreaConc, jigan, nNumLast);
			pBri->GetTvPostJigan(tvArrUpper, tvArrLeft, tvArrRight, tvArrMid, tvArrArch, tvArrMid2, jigan);

			if(tvArrLeft.GetSize() ==0 && tvArrRight.GetSize() == 0 && tvArrMid.GetSize() == 0)
				continue;

			//�¿� ���� �������� �ִ��� Ȯ��
			BOOL bFindLeftPost = FALSE, bFindRightPost = FALSE;	
			long nNumLeft = -1, nNumRight = -1;
			for (long ix =0; ix < m_pArrAreaConc.GetSize(); ix++)
			{
				CTwinVectorAreaShape *pArea	= m_pArrAreaConc.GetAt(ix);
				if(pArea == NULL) continue;
				if(pArea->m_tvArr.GetSize() == 0) continue;
				if(pArea->m_sInfo.Find("����") > -1 && bFindLeftPost == FALSE)
				{
					bFindLeftPost = TRUE; 
					nNumLeft = pArea->m_nNum;
				}
				else if(pArea->m_sInfo.Find("����") > -1 && bFindRightPost == FALSE)
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
				//��ġ�α��� �����ؼ� �������� �� �ΰ��� �׸� -> ���� ���̺��� ����
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

					// ������ �¿��� ������ ã��
					GetXyMatchLineAndLine(tvLeft.m_v1, tvLeft.GetXyDir(), CDPoint(0, dYCur), CDPoint(1, 0), xyMatchLeft);
					GetXyMatchLineAndLine(tvRight.m_v1, tvRight.GetXyDir(), CDPoint(0, dYCur), CDPoint(1, 0), xyMatchRight);

					CBMData *pBM					= new CBMData;
					pBM->m_nNumbering			= bPost_Left? nNumLeft : nNumRight;
					pBM->m_nIdxBigClass			= BM_BIG_POST;
					pBM->m_sBigClass				= GetStringBigClass(pBM->m_nIdxBigClass);
					pBM->m_nLevelTotal			= GetLevelTotal(pBM->m_nIdxBigClass); 
					pBM->m_nIdxSmallClass		= BM_SMALL_POST_HORI_CONNECT;
					pBM->m_sSmallClass			= GetStringSmallClass(pBM->m_nIdxSmallClass);
					pBM->m_sNoteHead.Format("���򿬰���(h=%.1f�ʰ��� ���� %.1fm���� ��ġ)", toM(dFirstHeight), toM(dSecondHeight));
					pBM->m_nTypeResult			= RESULT_AREA;
					pBM->m_sUnit					= "��";
					pBM->m_nShape				= SHAPE_TV_RECTANGLE;
					pBM->m_dV[WidthLower]		= Round(toM(fabs(xyMatchRight.x-xyMatchLeft.x)),3);
					pBM->m_dV[Length]			= Round(toM(dWidth),3);
					pBM->m_dwSide				= SIDE_LOWER;
					pBM->CalculateResult();
					// �������� ��¾��
					pBM->m_nLevelMidClass		= 2;
					dPrevH = dYCur;
					m_pArrBMBridgeStd.Add(pBM);
				}
			}			
		}		
	}

	pBri->GetTvVertSection_Std(0, FALSE, FALSE, TRUE);
}

// ���� ������ �¿� ������ ������
// ������ ������ü �������� �ִ��� �˾ƾ� ��(��ü ��Ǫ�� ���� ����� ������ �β��κ��� 
// ���� �Ǵ��� �Ǵ��ϱ� ����)
// nTypeLeft, nTypeRight
// -2 : ����������� ����
// -1 : ������
// 0 : �Ϲݴܸ�
// 1 : ������
// 2 : ����������� ����
void COutBM::GetInfoPosAreaConc(CTwinVectorAreaShape *pArea, long &nTypeLeft, long &nTypeRight)
{
	nTypeLeft	= 0;
	nTypeRight	= 0;

	// ���������� Ȯ���ϴ� ����
	const long nCountStt	= 4;
	CString sInfoStt[nCountStt]	= {"��������ϻ������", "�������������", "�����������ġ", "������ü������"};

	// ���������� Ȯ���ϴ� ����
	const long nCountEnd	= 4;
	CString sInfoEnd[nCountEnd]	= {"��������ϻ������", "�������������", "�����������ġ", "������ü������"};

	CTwinVector tv;
	// ���� ������ �ִ��� �˻�
	long i = 0; for(i = 0; i < nCountStt; i++)
	{
		if(pArea->m_tvArr.GetTvByInfo(sInfoStt[i], tv))
		{
			if(sInfoStt[i] == "�������������")
				nTypeLeft	= -2;
			else
				nTypeLeft	= -1;
			break;
		}
	}

	// ���� ������ �ִ��� �˻�
	for(i = 0; i < nCountEnd; i++)
	{
		if(pArea->m_tvArr.GetTvByInfo(sInfoEnd[i], tv))
		{
			if(sInfoEnd[i] == "�������������")
				nTypeRight	= 2;
			else
				nTypeRight	= 1;
			break;
		}
	}
}

// ������ ���� ����
// ���ӽ����� ������ �¿����� ����.
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
				CString sDowel = _T("������");
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

			pRB->m_sDescription = _T("������");
			switch(stt)
			{
				case iSTT : sItemClass = bLeft ? "����(����)" : "����(����)"; break;
				case iEND : sItemClass = bLeft ? "����(����)" : "����(����)"; break;
			}
			CBMData *pBM				= new CBMData;
			pBM->m_nIdxBigClass			= BM_BIG_DOWELBAR;
			pBM->m_sBigClass			= GetStringBigClass(pBM->m_nIdxBigClass);
			pBM->m_nLevelTotal			= GetLevelTotal(pBM->m_nIdxBigClass); 
			pBM->m_nIdxMidClass			= BM_MID_DOWELBAR;
			pBM->m_sMidClass.Format("%s%.0f, L=%.0fmm, CTC=%.0fmm", strBar, pRB->m_dDia, pRB->GetLength(), pAps->m_dCtcDowelBar);
			pBM->m_nIdxSmallClass		= BM_SMALL_DOWELBAR;	// ����
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

// ��ũ��Ʈ(���ӽ�����)
// ����� ������ ������.
// CTwinVector tvBackFillStt, CTwinVector tvBackFillEnd: ��ä�� ���������� ���Ҷ� ��ä�� ����ġ
// ���ӽ����� ���� �ܸ鵵�� ��ä�� ���Ե� �ܸ鵵�� ���� �� ������� �����ش�.
void COutBM::MakeBMConcreteAps(BOOL bForDeductOfBackFill, CTwinVector tvBackFillStt, CTwinVector tvBackFillEnd)
{

	// �������϶��� ���� �������� ���ؾ� ��.
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
				BOOL bCns	= cns != -1;	// ���潽��������?

				// ������ΰ�� ���潽���� ��� ����
				if(bForDeductOfBackFill && bCns) continue;

				// ��������� ���ϴ� ��� ��������� �߰��Ѵ�.
				// �� �ڼ��� ������ �� �Լ��� ȣ�� �ϴ� ������ ������ �� ��� ��.
				CTypedPtrArray <CObArray, CBMData*> *pArrBMStd	= NULL;
				CApsApp *pAps	= NULL;
				if(bCns)
				{
					pAps	= bStt ? &pBri->m_cnsStt[left][cns] : &pBri->m_cnsEnd[left][cns];
					pArrBMStd	= bForDeductOfBackFill ? &m_pStd->m_pArrOutBMTogong.GetAt(m_nBri)->m_pArrBMTogongStd[nJ] : &m_pArrBMCnsStd[stt][left][cns];
					sItemClass.Format("%s ���潽����(%s%d)", bStt ? "����" : "����", bLeft ? "��" : "��", cns+1);
				}
				else
				{
					pAps	= bStt ? &pBri->m_apsStt[left] : &pBri->m_apsEnd[left];
					pArrBMStd	= bForDeductOfBackFill ? &m_pStd->m_pArrOutBMTogong.GetAt(m_nBri)->m_pArrBMTogongStd[nJ] : &m_pArrBMApsStd[stt][left];
					sItemClass.Format("%s ���ӽ�����(%s)", bStt ? "����" : "����", bLeft ? "��" : "��");
				}
				if(!pAps->m_bIs) continue;

				///////////////////// ��ä�� ���������� ���� �ʿ��� /////////////
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
				///////////////////// ��ä�� ���������� ���� �ʿ��� /////////////

				double dH1	= pAps->m_dH1;
				double dH2	= pAps->m_dH2;
				double dL2	= pAps->m_dL2;
				double dL3	= pAps->m_dL3;

				// ��ġ��
				// ���ϸ� ��� ��.
				if(!bCns && pAps->m_dH2 > 0)
				{
					// �������϶��� ��ä�� ���Ե� �ܸ鵵�� ������ �����ش�.
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
					pAps->m_tvArrPlan.GetTvByInfo("���ظ�", tvOrg);
					pAps->m_tvArrPlan.GetTvByInfo("���ӽ���������", tvLeft);
					pAps->m_tvArrPlan.GetTvByInfo("���ӽ��������", tvRight);
					tvUpper	= tvOrg;
					tvLower	= tvOrg;
					Offset(tvUpper.m_v1, tvUpper.m_v2, -(dL2 + dL3));
					Offset(tvLower.m_v1, tvLower.m_v2, -dL3);

					// ��ġ��� ��𸮲� 
					CDPoint xyMatch1(0, 0), xyMatch2(0, 0);
					GetXyMatchLineAndLine(tvUpper.m_v1, tvUpper.GetXyDir(), tvLeft.m_v1, tvLeft.GetXyDir(), xyMatch1);
					GetXyMatchLineAndLine(tvUpper.m_v1, tvUpper.GetXyDir(), tvRight.m_v1, tvRight.GetXyDir(), xyMatch2);
					tvUpper.m_v1	= xyMatch1;
					tvUpper.m_v2	= xyMatch2;

					// ��ġ�ϸ� ��ٸ���
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
					// (ARCBRIDGE-1481) ��ġ�ο��� �ѹ��� �߰�
					pBMHunch->m_nLevelTotal			= 2;
					pBMHunch->m_nNumbering			= 1;
					pBMHunch->m_nIdxBigClass		= BM_BIG_CONCRETE;
					pBMHunch->m_sBigClass			= GetStringBigClass(pBMHunch->m_nIdxBigClass);
					pBMHunch->m_nIdxMidClass		= BM_MID_CONCRETE_MAIN;
					pBMHunch->m_sMidClass			= GetStringMidClass(pBMHunch->m_nIdxMidClass, pAps, bStt? ePartApsStt : ePartApsEnd);
					if(pBri->IsBoxType())
					{
						pBMHunch->m_nIdxPartClass		= bStt ? 0 : 1;
						pBMHunch->m_sPartClass	    	= bStt ? "����" : "����";
					}
					pBMHunch->m_nIdxSmallClass		= (nTypeBM == TYPE_BM_NORMAL)? BM_SMALL_CONCRETE_APS : BM_SMALL_CONCRETE_FLOOR;
					pBMHunch->m_sSmallClass			= GetStringSmallClass(pBMHunch->m_nIdxSmallClass);
					pBMHunch->m_nIdxItemClass		= nIdxItemClass++;
					pBMHunch->m_sItemClass			= sItemClass;
					pBMHunch->m_nTypeResult			= RESULT_VOLUMN;
					pBMHunch->m_sUnit				= "��";
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
				
				// lean = 0 : ������ ��ũ��Ʈ
				// lean = 1 : ������ũ��Ʈ
				for(long lean = 0; lean < 2; lean++)
				{
					BOOL bLean	= lean == 0 ? TRUE : FALSE;

					// ���潽����� ������ũ��Ʈ ����
					if(bLean && bCns) continue;
					// ������ũ��Ʈ�� ��ġ�ΰ� �������� �����ϸ� 
					// ���潽���갡 �ִٸ� ������ũ��Ʈ�� ������ �ƴ϶� �Ϲ� ������ �������� 
					// ������ũ��Ʈ ���� ���ؾ� �ȴ�.
					if(bLean)
					{
						if(pAps->m_dH2 > 0) continue;
						if((bStt && pBri->m_cnsStt[left][0].m_bIs) && (!bStt && pBri->m_cnsEnd[left][0].m_bIs)) bLean = FALSE;
					}

					// ���ӽ����� ��� ���� ���ϱ� .
					GetTvAreaShapeApsPlane(&m_pArrAreaConc, bStt, bLeft, bLean);

					// �������϶��� ��ä�� ���Ե� �ܸ鵵�� ������ �����ش�.
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


					// ��ġ�� ������ ������/������ũ��Ʈ
					long nSize	= m_pArrAreaConc.GetSize();
					long i = 0; for(i = 0; i < nSize; i++)
					{
						CTwinVectorAreaShape *pArea	= m_pArrAreaConc.GetAt(i);

						CBMData *pBM				= new CBMData;
						// (ARCBRIDGE-1481) ��ġ�ο��� �ѹ��� �߰�
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
							pBM->m_sPartClass	    	= bStt ? "����" : "����";
						}
						pBM->m_nIdxSmallClass		= bLean ? (BM_SMALL_CONCRETE_LEAN) : (bCns ? BM_SMALL_CONCRETE_CNS : BM_SMALL_CONCRETE_APS);
						if(bLean == FALSE && nTypeBM != TYPE_BM_NORMAL)
							pBM->m_nIdxSmallClass = BM_SMALL_CONCRETE_FLOOR;

						pBM->m_sSmallClass			= GetStringSmallClass(pBM->m_nIdxSmallClass);
						pBM->m_nIdxItemClass		= nIdxItemClass++;
						pBM->m_sItemClass			= sItemClass;
						pBM->m_nTypeResult			= RESULT_VOLUMN;
						pBM->m_sUnit				= "��";
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

// ���ӽ����� ���⼭ ù������ ��
void COutBM::DrawFirstPageAps(CHgBaseBMStd *pBaseBMStd, BOOL bStt, BOOL bLeft)
{
	if(!pBaseBMStd) return;

	CRcBridgeRebar *pBri	= GetRcBridgeRebar();
	if(!pBri) return;

	CARcBridgeDBStandard DBStd(pBri, m_pStd->m_pARoadOptionStd, m_pStd->m_pARcBridgeDataStd);
	DBStd.m_pDrawAps->m_bApplyDimSettingOfBaseDom	= TRUE;
	CRebarPlacing rb;

	// ������ ��µǾ����� �ϴ� ������
	double dWidthOrg			= 200;	// ���� ����ũ��

	long nRow	= 5;
	long left	= bLeft ? iLEFT : iRIGHT;
	CApsApp *pAps	= bStt ? &pBri->m_apsStt[left] : &pBri->m_apsEnd[left];
	if(!pAps->m_bIs) return;

	// �������� ��������!!! ////////////////////////////////////
	// �װɷ� ���ܸ�� ���õ� ������ �׷�����.
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

	// ���� ���� ////////////////////
	double dScale	= pBaseBMStd->GetScale(&domTmp, dWidthOrg);
	pBaseBMStd->SetDomyunValue(&domPlane, dWidthOrg, &domTmp);
	///////////////////////////////////

	// ������ ���ؿ�
	GetTvAreaShapeApsPlane(&m_pArrAreaConc, bStt, bLeft);

	// ���ؿ� ������ �׸���.
	pBaseBMStd->DrawAreaSectionNumber(&domPlane, &m_pArrAreaConc, 1);

	// ġ������
	pBaseBMStd->DimAreaSection(&domPlane, &m_pArrAreaConc, 2, TRUE);
	pBaseBMStd->DimAreaSection(&domPlane, &m_pArrAreaConc, 3, TRUE);
	
	domPlane.SetCalcExtRect();
	
	// �׷ȴٸ� ���ϴ� ũ��� ������ ����
	domPlane.RedrawByScale(dScale, TRUE);
	
	// ������ ����
	domPlane.SetCalcExtRect();

	// �׸��� ������ ����
	long nTaAlign	= TA_CENTER;

	CString sTitle	= _T("");
	sTitle.Format("%s ���ӽ�����(%s)", bStt ? "������" : "������", bLeft ? "��" : "��");

	CDoubleArray dArrTmp;
	nRow	= pBaseBMStd->InsertDomyun(nRow, nTaAlign, &domPlane, dArrTmp, domPlane.GetExtRect().Width(), domPlane.GetExtRect().Height(), sTitle)+3;

	// ���ӽ����� �ܸ�
	CDomyun domSec;
	domSec.SetCWnd(m_pStd->m_pView);

	// ���� ���� ////////////////////
	dScale	= pBaseBMStd->GetScale(&domTmp, dWidthOrg);
	pBaseBMStd->SetDomyunValue(&domSec, dWidthOrg, &domTmp);
	///////////////////////////////////

	// ������ ���ؿ�
	GetTvAreaShapeApsSection(&m_pArrAreaConc, bStt, bLeft);
	
	// ���ؿ� ������ �׸���.
	pBaseBMStd->DrawAreaSectionNumber(&domSec, &m_pArrAreaConc, 1);

	// ġ������
	pBaseBMStd->DimAreaSection(&domSec, &m_pArrAreaConc, 2, TRUE);
	pBaseBMStd->DimAreaSection(&domSec, &m_pArrAreaConc, 3, TRUE);
	
	// �׷ȴٸ� ���ϴ� ũ��� ������ ����
	domSec.RedrawByScale(dScale, TRUE);
	
	// ������ ����
	domSec.SetCalcExtRect();

	// �׸��� ������ ����
	nTaAlign	= TA_CENTER;
	sTitle	= _T("");


	nRow	= pBaseBMStd->InsertDomyun(nRow, nTaAlign, &domSec, dArrTmp, domSec.GetExtRect().Width(), domSec.GetExtRect().Height(), sTitle)+3;

	DBStd.m_bApplyDimSettingOfBaseDom	= FALSE;
}

// ���ӽ����� ��� ������ ������.
// BOOL bLean : ������ũ��Ʈ ������ ���ϴ���?
// ���� ������ũ��Ʈ�� �β��� ���ӽ����꿡���� Ȯ����̴� 100���� ����
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

	pAps->GetTvPlan(FALSE, FALSE, bLean);	//ARCBRIDGE-2493 ��ġ�� ������ ���ϴ� ������ , pAps->GetTvPlan(FALSE, TRUE, bLean);
	tvArrPlane	= pAps->m_tvArrPlan;
	CDPoint xyDirRot	= ToDPointFrDegree(pBri->GetXyDirRotateDomByBridgeSttEndXy().GetAngleDegree()+pAps->m_xyDirPlane.GetAngleDegree()-90);
	if(!pAps->m_bLeft)
		xyDirRot		= ToDPointFrDegree(xyDirRot.GetAngleDegree()-180);
	rb.RotateTvArr(tvArrPlane, CDPoint(0, 0), xyDirRot.GetAngleDegree());

	pAps->GetTvPlan(FALSE, TRUE);		// ���ӽ������ǿ��� ����� ���� �س��� ����

	rb.RotateTvArr(tvArrPlane, CDPoint(0, 0), xyRot.Rotate90().GetAngleDegree()*-1);
	rb.SeperatorTwinVectorArea(tvArrPlane, tvArrResult, dArrAngleSu, 1);
	
	m_pStd->ConvertTvArrToTvArrAreaShape(tvArrResult, dArrAngleSu, pArrAreaConc, 1, FALSE);

	return 0;
}


// ���ӽ����� �ܸ�
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

	// ��ġ�ΰ� 1������ ��������. ��ġ�ΰ� �ִٸ� 2���� �ǵ��� ������ �ٲ�.
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

// ��� ��ü
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
			pBM->m_sUnit			= "��";
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

// ��� ������
void COutBM::MakeBMScaffoldWing()
{
	CRcBridgeRebar *pBri	= GetRcBridgeRebar();
	if(!pBri) return;

	// ������ �¿��� ������
	CTwinVectorArray tvArrOut, tvArrCenOut, tvArrSideOut;
	CTwinVectorArray tvArrIn, tvArrCenIn ,tvArrSideIn;

	CTwinVectorArray tvArrScaffoldSec;
	CString sNoteTail	= _T("");
	CString sStt		= _T("");
	CString sLeft		= _T("");

	long stt = 0; for(stt = 0; stt < 2; stt++)
	{
		BOOL bStt	= stt == iSTT ? TRUE : FALSE;
		sStt	= bStt ? "������" : "������";

		pBri->GetTvScaffoldSection(tvArrScaffoldSec, bStt);	// ���� ������
		for(long left = 0; left < 2; left++)
		{
			BOOL bLeft	= left == iLEFT ? TRUE : FALSE;
			if(!pBri->IsWingWall(bStt, bLeft)) continue;

			sLeft	= bLeft ? "����" : "����";
			
			CTypedPtrArray <CObArray, CBMData*> *pArrBMStd	= &m_pArrBMWingStd[stt][left];

			pBri->GetTvScaffoldWingPlane(tvArrOut, tvArrIn, tvArrCenOut, tvArrCenIn, tvArrSideOut, tvArrSideIn, bStt, bLeft);
			double dHeight	= fabs(tvArrScaffoldSec.GetRect().Height());

			// ������ ����
			sNoteTail	= sStt + sLeft + "������ ����";
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
				pBM->m_sUnit			= "��";
				pBM->m_nShape			= SHAPE_TV_RECTANGLE;
				pBM->m_dV[WidthLower]	= Round(toM(tvArrCenOut.GetAt(i).GetLength()),3);
				pBM->m_dV[Height]		= Round(toM(dHeight),3);
				pBM->m_dwSide			= SIDE_FRONT;
				pBM->CalculateResult();
				pArrBMStd->Add(pBM);	
			}

			// ������ ����
			sNoteTail	= sStt + sLeft + "������ ����";
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
				pBM->m_sUnit			= "��";
				pBM->m_nShape			= SHAPE_TV_RECTANGLE;
				pBM->m_dV[WidthLower]	= Round(toM(tvArrCenIn.GetAt(i).GetLength()),3);
				pBM->m_dV[Height]		= Round(toM(dHeight),3);
				pBM->m_dwSide			= SIDE_FRONT;
				pBM->CalculateResult();
				pArrBMStd->Add(pBM);	
			}

			//������ ����
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
				pBM->m_sUnit					= "��";
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

// ��� ��ü �� ����
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

	// ������ ������ //////////
	CDomyun domTmp;
	domTmp.SetCWnd(m_pStd->m_pView);
	// ���ܸ� �׸�
	DBStd.m_pDrawVertSection->DrawVertSectionAll(&domTmp);
	DBStd.m_pDrawVertSection->DrawElEarthLine(&domTmp,0,0,FALSE,TRUE);
	// ������ ������ //////////

	// ��� ��ü �ܸ鵵  ////////////////////
	CDomyun domScaffoldSec;
	domScaffoldSec.SetCWnd(m_pStd->m_pView);

	// ġ������
	double dWidthOrg	= 600;
	// ���� ���� ////////////////////
	double dScale	= baseBMStd.GetScale(&domTmp, dWidthOrg);
	baseBMStd.SetDomyunValue(&domScaffoldSec, dWidthOrg, &domTmp);
	///////////////////////////////////

	// ���ܸ� �׸�
	DBStd.m_pDrawVertSection->DrawVertSectionAll(&domScaffoldSec);
	if(m_pStd->m_bDrawElEarth)
		DBStd.m_pDrawVertSection->DrawElEarthLine(&domScaffoldSec,0,0,FALSE,TRUE);

	// ������ ��� �׸�
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

	// ��� ġ��
	DBStd.m_pDrawVertSection->DimScaffoldForBM(&domScaffoldSec);

	// �𵵸� WidthOrg ������� ����
	domScaffoldSec.RedrawByScale(dScale);
	domScaffoldSec.SetCalcExtRect();

	// ������ �ٷ� �տ� �߰�
	long i = 0; for(i = 0; i < m_pArrBMStringBridge.GetSize(); i++)
	{
		CBMString *pBMString	= m_pArrBMStringBridge.GetAt(i);
		if(pBMString->m_sProcessKind1.Find("���", 0) != -1)
		{
			pBMString->m_Dom	= domScaffoldSec;
			pBMString->m_bInsertDomyun	= TRUE;
			break;
		}
	}

	DBStd.m_bApplyDimSettingOfBaseDom	= FALSE;
	pBri->m_tvArrVertSection = tvArrVertSectionOld;
}

// ��ü �����̼� ����
// ������ũ��Ʈ�̿��� ö�� ���� �κ��� ��� �����̼� ������ ���Ե�..
void COutBM::MakeBMSpacerMain()
{
	CRcBridgeRebar *pBri	= GetRcBridgeRebar();
	if(!pBri) return;

	// ��ü�� �����̼��� ��ü ���� ���Կ���...
	// ��, ���������� �׻� ���Ե�.
	BOOL bIncludeSideForm	= TRUE;
	
	// ö���� ���� �κ��� ��Ǫ���� ��� �����Ѵ�.
	// ������ũ��Ʈ�� mass��ũ��Ʈ �׸��� �ɼǿ� ���� ��ü���� ��Ǫ���� ������ 
	// ����,��� -> ��ü��, ���� -> �������
	CTypedPtrArray <CObArray, CBMData*> pArrFormVert;
	CTypedPtrArray <CObArray, CBMData*> pArrFormHor;
	BOOL bSlab	= FALSE;
	long nSizeBM	= m_pArrBMBridgeStd.GetSize();
	long i = 0; for(i = 0; i < nSizeBM; i++)
	{
		CBMData *pBM = m_pArrBMBridgeStd.GetAt(i);
		if(pBM->m_nIdxBigClass == BM_BIG_FORM)
		{
 			if(pBM->m_sInfo.Find("������ũ��Ʈ��Ǫ��") != -1) continue;
			if(pBM->m_sInfo.Find("MASS��ũ��Ʈ��Ǫ��") != -1) continue;
			if(!bIncludeSideForm)
				if(pBM->m_sInfo.Find("��ü�����Ǫ��") != -1) continue;

			if(pBM->m_sSmallClass.Find("����", 0) != -1 || pBM->m_sNoteTail=="��ġ�����ϸ�") bSlab	= TRUE;
			else bSlab	= FALSE;

			CBMData *pForm = new CBMData;
			*pForm = *pBM;
			// ��Ǫ¤ �������� ������ ������ ��ȣ�� ������� ���ĵ��� �ʴ´�.
			// �׷��� ���⼭�� ��Ǫ�� ������ ��� ���� ������ ���ش�.
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
			if (pBM->m_sInfo == "���������")
			{
				CBMData *pForm = new CBMData;
				*pForm = *pBM;
				// ��Ǫ¤ �������� ������ ������ ��ȣ�� ������� ���ĵ��� �ʴ´�.
				// �׷��� ���⼭�� ��Ǫ�� ������ ��� ���� ������ ���ش�.
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
	
	// ����
	CHgBaseBMStd baseBM;
	baseBM.ShellSortBMList(&pArrFormVert, &CBMData::CompareBM);
	baseBM.ShellSortBMList(&pArrFormHor, &CBMData::CompareBM);
	
	// ��ü�� �����̼� ���� ����
	if(pArrFormVert.GetSize() > 0)
	{
		CBMData *pWall				= new CBMData;
		pWall->m_nIdxBigClass		= BM_BIG_SPACER;
		pWall->m_sBigClass			= GetStringBigClass(pWall->m_nIdxBigClass);
		pWall->m_nLevelTotal		= GetLevelTotal(pWall->m_nIdxBigClass);
		pWall->m_nIdxSmallClass		= BM_SMALL_SPACER_WALL;
		pWall->m_sSmallClass		= GetStringSmallClass(pWall->m_nIdxSmallClass);
		pWall->m_nTypeResult		= RESULT_AREA;
		pWall->m_sUnit				= "��";
		pWall->m_sInfo				= "��ü��ü�뽺���̼�";

		// ������ ��Ǫ�� �������� �߰���Ŵ
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
			sCurve = _T(",���");
		if(bIncludeSideForm)
			pWall->m_sForm = (str + "�� ��Ǫ��(������,����" + sCurve + ")����");
		else
			pWall->m_sForm = (str + "�� ��Ǫ��(������,����" + sCurve + ")����(�������� ���� ����)");

		pWall->m_bIsPrintByRefForm	= FALSE;
		pWall->CalculateResult();
		// �������� ��¾��
		pWall->m_nLevelMidClass		= 2;
		m_pArrBMBridgeStd.Add(pWall);
	}

	// ������� �����̼� ���� ���� 
	if(pArrFormHor.GetSize() > 0)
	{
		// ��ν����� �ϸ� ��Ǫ�� + �����ϸ� ����
		// �����
		CBMData *pSlab1				= new CBMData;
		pSlab1->m_nIdxBigClass		= BM_BIG_SPACER;
		pSlab1->m_sBigClass			= GetStringBigClass(pSlab1->m_nIdxBigClass);
		pSlab1->m_nLevelTotal		= GetLevelTotal(pSlab1->m_nIdxBigClass);
		pSlab1->m_nIdxSmallClass	= BM_SMALL_SPACER_SLAB;
		pSlab1->m_sSmallClass		= GetStringSmallClass(pSlab1->m_nIdxSmallClass);
		pSlab1->m_nTypeResult		= RESULT_AREA;
		pSlab1->m_sUnit				= "��";
		pSlab1->m_sInfo				= "��ü������뽺���̼�(��Ǫ�������)";

		// ������ ��Ǫ�� �������� �߰���Ŵ
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
		pSlab1->m_sForm = (str + "�� ��Ǫ��(�����) ����");

		pSlab1->m_bIsPrintByRefForm	= FALSE;
		pSlab1->CalculateResult();
		// �������� ��¾��
		pSlab1->m_nLevelMidClass		= 2;
		m_pArrBMBridgeStd.Add(pSlab1);	
	}

	// �����ϸ����
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
				pSlab2->m_sUnit					= "��";
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
				pSlab2->m_sInfo					= "��ü������뽺���̼�(�����ϸ����)";
				pSlab2->m_sNoteHead				= "�����ϸ� ����";
				pSlab2->CalculateResult();
				// �������� ��¾��
				pSlab2->m_nLevelMidClass		= 2;

				m_pArrBMBridgeStd.Add(pSlab2);
			}
		}
	}

	//�߰� Box������� ��� ���� �������� �����̼� ���� (�Ϻ� ������� ���� ��Ǫ�� ������ ������ �ʱ� ������ ���� ����)
	if(pBri->IsBoxType())
	{
		long nNumLower = -1;
		double dLen(0);
		double dSumPfWidth(0);

		for (long nIdx =0; nIdx < pBri->m_pArrAreaConc.GetSize(); nIdx++)
		{
			CTwinVectorAreaShape *pArea	= pBri->m_pArrAreaConc.GetAt(nIdx);
			if(pArea->m_sInfo.Find(_T("�λ���� ����")) > -1)
			{
				dSumPfWidth += Round(toM(pArea->m_dWidth),3);
			}
			if(pArea->m_sInfo.Find(_T("�Ϻν�����")) > -1)
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
				str += _T("�� ��Ǫ��(�����) ����");				
				
				sFormula.Format("%s %.3f x %.3f", str , dWidth, dLen+dSumPfWidth);

				CBMData *pSlabLower			= new CBMData;
				pSlabLower->m_nNumbering			= -1;
				pSlabLower->m_nIdxBigClass			= BM_BIG_SPACER;
				pSlabLower->m_sBigClass				= GetStringBigClass(pSlabLower->m_nIdxBigClass);
				pSlabLower->m_nLevelTotal			= GetLevelTotal(pSlabLower->m_nIdxBigClass);
				pSlabLower->m_nIdxSmallClass		= BM_SMALL_SPACER_SLAB;
				pSlabLower->m_sSmallClass			= GetStringSmallClass(pSlabLower->m_nIdxSmallClass);
				pSlabLower->m_nTypeResult			= RESULT_AREA;
				pSlabLower->m_sUnit					= "��";
				pSlabLower->m_nShape				= SHAPE_TV_RECTANGLE;
				pSlabLower->m_sInfo					= "��ü������뽺���̼�(�Ϻν������ϸ����)";

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

// ��ũ��Ʈ ��� ��ü
// ���ʸ� ������ ��Ǫ�� ���� + ��μ����
void COutBM::MakeBMConCareMain()
{
	CRcBridgeRebar *pBri	= GetRcBridgeRebar();
	if(!pBri) return;
	if(!pBri->m_bSelectBM_ConcCare_Main) return;

	
	// ��Ǫ���� ��� �����Ѵ�.
	// ������ũ��Ʈ�� mass��ũ��Ʈ, ���ʸ� ������ ��� ��Ǫ���� ����
	CTypedPtrArray <CObArray, CBMData*> pArrForm;
	long nSizeBM	= m_pArrBMBridgeStd.GetSize();
	long i = 0; for(i = 0; i < nSizeBM; i++)
	{
		CBMData *pBM = m_pArrBMBridgeStd.GetAt(i);
		if(pBM->m_nIdxBigClass == BM_BIG_FORM)
		{
 			if(pBM->m_sInfo.Find("������ũ��Ʈ��Ǫ��") != -1) continue;
			if(pBM->m_sInfo.Find("MASS��ũ��Ʈ��Ǫ��") != -1) continue;
			if(pBM->m_sInfo.Find("���ʰ�Ǫ��") != -1) continue;
			
			CBMData *pForm = new CBMData;
			*pForm = *pBM;
			// ��Ǫ¤ �������� ������ ������ ��ȣ�� ������� ���ĵ��� �ʴ´�.
			// �׷��� ���⼭�� ��Ǫ�� ������ ��� ���� ������ ���ش�.
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
	
	// ����
	CHgBaseBMStd baseBM;
	baseBM.ShellSortBMList(&pArrForm, &CBMData::CompareBM);

	// ��Ǫ�� ���� �߰�
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
		pBM->m_sUnit			= "��";
		pBM->m_sInfo			= "��ũ��Ʈ���(��Ǫ��)";

		if(pBri->m_nSelectBM_ConcCare_Cal == 0)
		{
			// ������ ��Ǫ�� �������� �߰���Ŵ
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
			pBM->m_sForm = (str + "�� ��Ǫ��(��������) ������ ����");

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
				sArrPosition.Add(_T("�������ʵޱ�"));
				sArrPosition.Add(_T("�������ʾձ�"));
				sArrPosition.Add(_T("�������ʵޱ�"));
				sArrPosition.Add(_T("�������ʾձ�"));

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
								str.Format(_T("�߰�����%d"),footing);
								pBMHor->m_sNoteTail	= str;
								m_pArrBMBridgeStd.Add(pBMHor);
							}
						}

					}
				}
			}
			else //������౳ type
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

				//��ν����� ���
				CTwinVectorArray tvArrSecCopy;

				tvArrSecCopy.AddFromTvArray(pBri->m_tvArrVertSection);
				for (long nIdx=0; nIdx< tvArrSecCopy.GetSize(); nIdx++)
				{
					CTwinVector tvSec = tvArrSecCopy.GetAt(nIdx);
					if(tvSec.m_sInfo.Find(_T("�����")) > -1 || tvSec.m_sInfo.Find(_T("�λ����")) > -1)
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

// ���ӽ����� ��ũ��Ʈ ���
// ������ũ��Ʈ�� ������ ��Ǫ�� + ���������
void COutBM::MakeBMConCareAps()
{
	CRcBridgeRebar *pBri	= GetRcBridgeRebar();
	if(!pBri) return;	
	if(!pBri->m_bSelectBM_ApsCnsInclude) return;
	if(!pBri->m_bSelectBM_ConcCare_Aps) return;

	// ��� ����
	long stt = 0; for(stt = 0; stt < 2; stt++)
	{
		BOOL bStt	= stt == iSTT ? TRUE : FALSE;

		for(long left = 0; left < 2; left++)
		{
			BOOL bLeft	= left == iLEFT ? TRUE : FALSE;

			CApsApp *pAps	= bStt ? &pBri->m_apsStt[left] : &pBri->m_apsEnd[left];
			if(!pAps->m_bIs) continue;
			CTypedPtrArray <CObArray, CBMData*> *pArrBMStd	= &m_pArrBMApsStd[stt][left];

			// ��Ǫ���� ��� �����Ѵ�. ///////////////////////////////////////////////////////////
			// ������ũ��Ʈ�� ������ ��� ��Ǫ���� ����
			CTypedPtrArray <CObArray, CBMData*> pArrForm;
			long nSizeBM	= m_pArrBMApsStd[stt][left].GetSize();
			long i = 0; for(i = 0; i < nSizeBM; i++)
			{
				CBMData *pBM = m_pArrBMApsStd[stt][left].GetAt(i);
				if(pBM->m_nIdxBigClass == BM_BIG_FORM)
				{
 					if(pBM->m_sInfo.Find("������ũ��Ʈ��Ǫ��") != -1) continue;
					
					CBMData *pForm = new CBMData;
					*pForm = *pBM;
					// ��Ǫ¤ �������� ������ ������ ��ȣ�� ������� ���ĵ��� �ʴ´�.
					// �׷��� ���⼭�� ��Ǫ�� ������ ��� ���� ������ ���ش�.
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
						
			// ����
			CHgBaseBMStd baseBM;
			baseBM.ShellSortBMList(&pArrForm, &CBMData::CompareBM);

			// ��Ǫ�� ���� �߰�
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
				pBM->m_sUnit			= "��";
				pBM->m_sInfo			= "��ũ��Ʈ���(��Ǫ��)";

				// ������ ��Ǫ�� �������� �߰���Ŵ
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
				pBM->m_sForm = (str + "�� ��Ǫ��(������ũ��Ʈ����) ������ ����");
				
				pBM->m_bIsPrintByRefForm	= FALSE;
				pBM->CalculateResult();
				pArrBMStd->Add(pBM);
			}
			AhTPADelete(&pArrForm, (CBMData*)0);
			// ��Ǫ���� ��� �����Ѵ�. ///////////////////////////////////////////////////////////

			// ���ӽ����� ��� ���� ���ϱ� . //////////////////////////////////////////////////////
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
				pBM->m_sUnit				= "��";
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
			// ���ӽ����� ��� ���� ���ϱ� . //////////////////////////////////////////////////////
		}
	}
}

// long nTypeComplex : 0 : ����, 1 : ����, 2 : ����, 3 : �ſ캹��
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

	// ������ ���� �����ϴ� �κ��� ���⼭ ���� (������ ���� ö�� ���踸 ��)
	CString sRebarType[2]		= {"����", "��"};
	CString sDia[2][4]			= {{"D10����", "D13", "D16~25", "D29~35"}, {"H10����", "H13", "H16~25", "H29~35"}};
	double dWeightTotal[2][4]	= {0, 0, 0, 0};	// 0 : ����, 1 : �� , 10����, 13, 16~25, 25~35 
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
		
		if(nTypeBM == TYPE_BM_NORMAL)	// �Ϲݼ����̸� ������ ������� ����
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
	
	// �������� ö�� ����
	// ��������ö���� pRB->m_dLengthTotal�� ����� �� ����
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

				// �������� ����ǥ ��� ��� ����
				pBM->m_nLevelMidClass	= 2;
				pBM->m_nLevelSmallClass	= 6;
				pBM->m_bApplyIncreaseForm	= FALSE;
				pBM->m_sNoteHead	= sNote;
				pArrBM->Add(pBM);
			}
		}
	} 
}

// ��ü ö�� ���� ����
// nType 0 : ��ü, 1 : ������, 2 : ���ӽ�����(��������),  3 : ��ȣ�� 5: ����Ÿ������
void COutBM::MakeBMRebar(long nType)
{
	CRcBridgeRebar *pBri	= GetRcBridgeRebar(TRUE);
	if(!pBri) return;

	// 0 : ����, 1 : ����, 2 : ����, 3 : �ſ캹��
	CTypedPtrArray <CObArray, CRebar*>  pArrRebar[4];

	// �������� ������ ��� ö���� �����;� �� ////////////////////////////////////////////
	// -1 : ��ü
	// 0 : ��ü
	// 1 : �߰���ü
	// 2 : ����
	// 3 : ������
	// 4 : �󼼺�ö��
	// 5 : ���
	// 6 : ������� �ƴ�(�Ŵ�ö��)
	// 7 : �������(�Ŵ�ö��)
	// 8 : ��ȣ��(�Ŵ�ö��)
	if(nType == 0)
	{
		long i = 0; for(i = 0; i < 4; i++)
			AhTPADelete(&pArrRebar[i],(CReBar*)0);

		pBri->MakeRebarList(&pArrRebar[2], 0, TRUE, TRUE, 0);		// ��ü(����)
		for(i = 0; i <= pBri->m_nQtyJigan/*pBri->GetCountInWall()*/; i++)
		{
			pBri->MakeRebarList(&pArrRebar[2], 1, TRUE, TRUE, i);	// �߰�����(����)
			pBri->MakeRebarList(&pArrRebar[2], 5, TRUE, TRUE, i);	// �߰�����(����)
			pBri->MakeRebarList(&pArrRebar[2], 6, TRUE, TRUE, i);	// ������� �ƴ�(�Ŵ�ö��)
			pBri->MakeRebarList(&pArrRebar[2], 7, TRUE, TRUE, i);	// �������(�Ŵ�ö��)
		}
		pBri->MakeRebarList(&pArrRebar[2], 2, TRUE, TRUE, 0);		// ����(����)
		pBri->MakeRebarList(&pArrRebar[2], 10, TRUE, TRUE, i);		//����Ÿ������

		if(!m_pStd->m_bSeparateExcludeRebarMonument)
			pBri->MakeRebarList(&pArrRebar[2], 4, TRUE, TRUE, i);		// �󼼺�(����)
		
		SetBMRebar(&m_pArrBMBridgeStd, &pArrRebar[2], 2, TRUE, "��ν�����,��ü(���),����, �����ֹ�ħ, �������� ö�ټ��� ����");
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
		// �����ٴ� ���� �� ���ٰ� �������� ��.
		// ���ӽ����� ö�� ��� ������
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
					// �����ٿ� �ٸ� ö�� ������.
					CRebar *pRB	= pAps->m_pArrRebar.GetAt(i);
					if(pRB->m_sDescription.Find("������", 0) != -1)
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
		// ���潽���� ö�� ö�� ��� ������
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
						// �����ٿ� �ٸ� ö�� ������.
						CRebar *pRB	= pCns->m_pArrRebar.GetAt(i);
						if(pRB->m_sDescription.Find("������", 0) != -1)
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

				pBri->MakeRebarList(&pArrRebar[2], 8, TRUE, TRUE, i);		// ��ȣ��(����) -> �������� ����(#0014842)
				SetBMRebar(&m_pArrBMGuardWallStd[i], &pArrRebar[2], 1);
			}
		}
	}
	// ��ü���� ������ ö�� ���� �и��� �ߵ�
	// ���� ������ ö�� ������ ��ü�� ���ԵǾ ������ �����δ� ö������ ���� �ʾ�����
	// �ɼ�ó���Ͽ� ������ ������ ��ü���� �и��� �� �ְ� ��.
	else if(nType == 4)
	{
		if(m_pStd->m_bSeparateExcludeRebarMonument)
		{
			long j = 0; for(j = 0; j < 4; j++)
				AhTPADelete(&pArrRebar[j],(CReBar*)0);

			pBri->MakeRebarList(&pArrRebar[2],  4, TRUE, TRUE, 0);		// �󼼺�(����)
			SetBMRebar(&m_pArrBMMonumentStd, &pArrRebar[2], 2);
		}
	}

	//����Ÿ������ ö�� ����
	else if(nType == 5 )
	{
		long nCountFooting	= pBri->GetCountFooting();
		for(long footing = 0; footing < nCountFooting; footing++)
		{
			CFootingApp *pFooting	= pBri->GetFooting(footing);
			if(pFooting == NULL) continue;

			// ����Ÿ������
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

// �����
// ������ ��� ������ ����
void COutBM::MakeBMSurface()
{
	CRcBridgeRebar *pBri	= GetRcBridgeRebar();
	if(!pBri) return;
	
	CTypedPtrArray <CObArray, CBMData*> *pArrBM	= &m_pArrBMBridgeStd;
	
	// ������ ��� ���� ����
	double dTot		= 0;
	long nSizeBM	= pArrBM->GetSize();
	long i = 0; for(i = 0; i < nSizeBM; i++)
	{
		CBMData *pBM = pArrBM->GetAt(i);
		if(pBM->m_nIdxBigClass == BM_BIG_SLABCARE)
			dTot	+= pBM->GetResult();
	}

	// �������� �߰�
	CBMData *pBM = new CBMData;
	pBM->m_nIdxBigClass		= BM_BIG_SURFACE;
	pBM->m_sBigClass		= GetStringBigClass(pBM->m_nIdxBigClass);
	pBM->m_nLevelTotal		= GetLevelTotal(pBM->m_nIdxBigClass);
	pBM->m_nTypeResult		= RESULT_AREA;
	pBM->m_sUnit			= "��";
	pBM->m_sNoteHead		= "���������� ����";
	pBM->SetResult(dTot);
	pBM->m_sVC[Result].Format("#��������:%.3f#", pBM->GetResult());
	pArrBM->Add(pBM);
}

// �ƽ��� ����
// ������ ��� ������ ����
// (�� ������ �޶� �����������ٰ� ������  100)
void COutBM::MakeBMAscon()
{
	CRcBridgeRebar *pBri	= GetRcBridgeRebar();
	if(!pBri) return;
	
	CTypedPtrArray <CObArray, CBMData*> *pArrBM	= &m_pArrBMBridgeStd;
	
	// �������� ���� ����
	double dTot	= 0;
	CTypedPtrArray <CObArray, CBMData*> pArrConcCare;
	long nSizeBM	= pArrBM->GetSize();
	long i = 0; for(i = 0; i < nSizeBM; i++)
	{
		CBMData *pBM = pArrBM->GetAt(i);
		if(pBM->m_nIdxBigClass == BM_BIG_SLABCARE)
			dTot	+= pBM->GetResult();
	}

	// �ƽ��� ����
	CBMData *pBM = new CBMData;
	pBM->m_nIdxBigClass			= BM_BIG_ASCON;
	pBM->m_sBigClass			= GetStringBigClass(pBM->m_nIdxBigClass);
	pBM->m_nLevelTotal			= GetLevelTotal(pBM->m_nIdxBigClass);
	pBM->m_nIdxMidClass			= BM_MID_ASCON;
	pBM->m_sMidClass.Format("T=%.0fmm", pBri->m_dTP);
	pBM->m_nTypeResult			= RESULT_AREA;
	pBM->m_sUnit				= "��";
	pBM->m_sNoteHead			= "�������� - ���� - ����";
	pBM->m_sForm.Format("%.3f", dTot);
	pBM->m_sFormExcel.Format("#��������:%.3f#", dTot);
	pBM->SetResult(dTot);
	pBM->m_bIsPrintByRefForm	= TRUE;

	pArrBM->Add(pBM);
}

// �� ����
// ���� 2ȸ�� ����
// �������� ������ �� �����Ǿ� �ֵ��� �ϱ� ���� ������
// ���� �β��� ���� Ƚ���� �޶� ��  �� ������ ����� ����
// �ɼ��� ��� ó�������� ��� �غ��� �� ������
// �ƽ������� * ȸ��
void COutBM::MakeBMTackCotting()
{
	CRcBridgeRebar *pBri	= GetRcBridgeRebar();
	if(!pBri) return;
	
	CTypedPtrArray <CObArray, CBMData*> *pArrBM	= &m_pArrBMBridgeStd;
	
	// �ƽ��� ���� ���� ����
	double dTot	= 0;
	long nSizeBM	= pArrBM->GetSize();
	long i = 0; for(i = 0; i < nSizeBM; i++)
	{
		CBMData *pBM = pArrBM->GetAt(i);
		if(pBM->m_nIdxBigClass == BM_BIG_ASCON)
			dTot	+= pBM->GetResult();
	}

	// �� ����
	double dTackCotting	= Round(pBri->m_dTackCotting, 1);
	CBMData *pBM = new CBMData;
	pBM->m_nIdxBigClass		= BM_BIG_TACKCOTTING;
	pBM->m_sBigClass		= GetStringBigClass(pBM->m_nIdxBigClass);
	pBM->m_nLevelTotal		= GetLevelTotal(pBM->m_nIdxBigClass);
	pBM->m_nIdxMidClass		= BM_MID_TACK_RSC1;
	pBM->m_sMidClass		= GetStringMidClass(pBM->m_nIdxMidClass);
	pBM->m_nTypeResult		= RESULT_AREA;
	pBM->m_sUnit			= "��";
	pBM->m_sForm.Format("%.3f x %.1fȸ", dTot, dTackCotting);
	pBM->m_sFormExcel.Format("#��������:%.3f# x %.1fȸ", dTot, dTackCotting);
	pBM->SetResult(dTot * dTackCotting);
	pBM->m_bIsPrintByRefForm	= TRUE;
	pArrBM->Add(pBM);
}

// ������ ����
// ������ ����� ����
// ������ ��� ������
void COutBM::MakeBMSurfaceProof()
{
	CRcBridgeRebar *pBri	= GetRcBridgeRebar();
	if(!pBri) return;
		
	CTypedPtrArray <CObArray, CBMData*> *pArrBM	= &m_pArrBMBridgeStd;
	
	// ������ ���
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
	pBM->m_sUnit			= "��";
	pBM->m_sNoteHead		= "���������� ����";
	pBM->SetResult(dTot);
	pBM->m_sVC[Result].Format("#��������:%.3f#", pBM->GetResult());
	pArrBM->Add(pBM);
}

// ������ ���
// ��ν����� �� ������ ������ ����
// ����� �����θ� ������ �ʰ� ��ü �� ������(2005.1.25)
void COutBM::MakeBMSlabCare()
{
	//CRcBridgeRebar *pBri = GetRcBridgeRebar();
	CRcBridgeRebar *pBriOrg = GetRcBridgeRebar(TRUE);
	//if(!pBri) return;
	if(!pBriOrg) return;
	
	// ��������
	// ���������� �¿��� ���� ���
	double dLen[2]	= {0, 0};
	CTwinVector tv;
	CString str	= _T("");
	for(long left = 0; left < 2; left++)
	{
		BOOL bLeft	= left == iLEFT;

		/*
		// pBriOrg->m_bSettingVertSection �� TRUE �̸� m_tvArrVertSection �� RemoveAll�ȴ�.
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
				str.Format("��������(%d��°����)", i+1);
				if(pBriOrg->m_tvArrVertSection.GetTvByInfo(str, tv))
					dLen[left]	+= tv.GetLength(TRUE);
			}
		}
		else
		{
			dLen[left]	= 0;
			if(pBriOrg->m_tvArrVertSection.GetTvByInfo("��������(������ü����)", tv))
				dLen[left]	+= tv.GetLength(TRUE);
			if(pBriOrg->m_tvArrVertSection.GetTvByInfo("��������(������ü��)", tv))
				dLen[left]	+= tv.GetLength(TRUE);
			if(pBriOrg->m_tvArrVertSection.GetTvByInfo("��������(������ü��)", tv))
				dLen[left]	+= tv.GetLength(TRUE);
		}
		pBriOrg->GetTvVertSection_Std(bLeft ? -1 : 1, TRUE, FALSE, TRUE);
	}
	
	pBriOrg->GetTvVertSection(0, TRUE);

	// �����꿡�� �����θ��� ���̴� ���� ������ �������� ����(Ȯ���� ��� ���� ����)
	// Ȯ���� ��쿡�� ������ �ٸ� ������� ���ؾ� ��.
	// �׸��� Ⱦ�� ���� ������� �ʰ� level�Ÿ��� �����
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
	pBM->m_sUnit			= "��";
	pBM->m_nShape			= dLen[iLEFT]==dLen[iRIGHT] ? SHAPE_TV_RECTANGLE : SHAPE_TV_TRAPEZOID;
	pBM->m_dV[WidthLower]	= Round(toM(dLen[iRIGHT]),3);
	pBM->m_sVC[WidthLower]	= "";
	pBM->m_dV[WidthUpper]	= Round(toM(dLen[iLEFT]),3);
	pBM->m_sVC[WidthUpper]	= "";
	pBM->m_dV[Height]		= Round(toM(dWidthSlab),3);
	pBM->m_sVC[Height]		= "";
	pBM->m_dwSide			= SIDE_FRONT;
	pBM->m_sNoteHead		= "��ȣ���� �� �ߺд� �������� ������ ������ ���� ����";
	pBM->m_sInfo			= "��ũ��Ʈ���(��Ǫ��)";
	pBM->CalculateResult();
	pBM->m_sVCN[Result]		= "��������";
	m_pArrBMBridgeStd.Add(pBM);
}

// ���Ϲ� ������
// ���絿�ٸ� �� * (�������� + 1.0M)
// Ȯ���� �ȵǹǷ� �������� ������ ������ ���������� ���ٸ� ���� ������� ����Ѵ�.
// void COutBM::MakeBMAntiDropping()
// {	
// 	CRcBridgeRebar *pBri	= GetRcBridgeRebar();
// 	if(!pBri) return;
// 
// 	// �������� �¿����� ���ٸ� ���� ���Ѵ�.
// }

// ���� ����(���Լ��� ���Ŀ� ������ �Ǹ� �������� �ö󰡾� ������)
void COutBM::MakeBMPile()
{
	CRcBridgeRebar *pBri	= GetRcBridgeRebar();
	if(!pBri) return;

	CHgBaseBMStd baseBM;
	CString strFooting = _T("");

	BOOL bBoxType		= pBri->IsBoxType();
	long nCountJijum	= bBoxType? 1 : pBri->GetCountJijum();
	long nIdxMidClass	= 0;
	
	// nCase = 0 : �����(����)
	// nCase = 1 : õ����
	// nCase = 2 : �ø�Ʈ ��ũ��
	// nCase = 3 : ���� �� �κκ���
	// nCase = 4 : ��������
	// nCase = 5 : �����Ͻ����
	// nCase = 6 : �����Ͻ����
	// nCase = 7 : Ⱦ���Ͻ����
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
						strFooting = _T("����");
					else if(j == nCountJijum-1)
						strFooting = _T("����");
					else
						strFooting.Format("�߰�����%d", j+1);
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

				// ���α��� ������ mid�ε����� �ϳ��� ���������ش�.(�ߺ����� �ʰ� �ϱ� ���ؼ�)
				// �������� ������ �ֱ� ������ ������ �и��ؼ� �����.
				long i = 0; for(i = nCount; i < m_pArrBMBridgeStd.GetSize(); i++)
					m_pArrBMBridgeStd.GetAt(i)->m_nIdxMidClass	= nIdxMidClass;
			}
			else // ����Ÿ������
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

					makeBM.SetIdxMidClass(AHP_MID_STEEL_INNER_DIGGING_BIG, BM_MID_STEEL_INNER_DIGGING_BIG);	// �������α���(�뱸��õ��)
					makeBM.SetIdxMidClass(AHP_MID_STEEL_INNER_DIGGING_RCD, BM_MID_STEEL_INNER_DIGGING_RCD);	// �������α���(RCDõ��)
					makeBM.SetIdxMidClass(AHP_MID_DIGGING_MACHINE_MOVE_DEPER, BM_MID_DIGGING_MACHINE_MOVE_DEPER);	// ������� �̵��� ��ġ
					makeBM.SetIdxMidClass(AHP_MID_PILE_BUILD, BM_MID_PILE_BUILD);			// ���� ������
					makeBM.SetIdxMidClass(AHP_MID_SPACER, BM_MID_SPACER_ALONE_HYUNTAPILE);	// SPACER ���� �� ��ġ
					makeBM.SetIdxMidClass(AHP_MID_REBAR_ANTI_RISE, BM_MID_REBAR_ANTI_RISE);	// ö�� ���� ����
					makeBM.SetIdxMidClass(AHP_MID_COPPING, BM_MID_COPPING);					// �κ� ����
					makeBM.SetIdxMidClass(AHP_MID_REMAIN_SOIL, BM_MID_REMAIN_SOIL);			// ����ó��
					makeBM.SetIdxMidClass(AHP_MID_REBAR_ASSM, BM_MID_REBAR_ASSM_AHP);		// ö�ٰ�������
					makeBM.SetIdxMidClass(AHP_MID_WASTE, BM_MID_WASTE);						// ��⹰ ó��
					makeBM.SetIdxMidClass(AHP_MID_SLIME, BM_MID_SLIME);						// slime ����
					makeBM.SetIdxMidClass(AHP_MID_CONCRETE, BM_MID_CONCRETE_AHP);			// concrete Ÿ��
					makeBM.SetIdxMidClass(AHP_MID_SURFACE, BM_MID_SURFACE_AHP);					// �� ����
					makeBM.SetIdxMidClass(AHP_MID_REBAR_NET, BM_MID_REBAR_NET);				// ö�ٸ� ����

					makeBM.SetPile(&pFooting->m_exFooting.m_Pile);
					makeBM.MakeBMAloneHyuntaPileStd();
				}
			}
		}
		
		nIdxMidClass++;
	}
}

// ������
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

// ������
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

// TBM��ġ
// ���� 1���� �����Ǿ� ����
// ������ �ٲ��ָ� ��
// �����Ǽ����� ����ǹ� ��� P298
// - 100m�̸� ������ 1����, �̻��� 2����
// - ��, �и� ������ ��� �ѵ���� ���� �ǹǷ�, ���༱���� ��ġ
void COutBM::MakeBMTBM()
{
	CRcBridgeRebar	*pBri	= GetRcBridgeRebar();
	if(!pBri) return;
	if(pBri->m_nSeparBri == 2) return;	// ������ ��� ������ ����

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

// ���ӽ����� ��Ǫ��
void COutBM::MakeBMFormAps()
{
	CRcBridgeRebar	*pBri	= GetRcBridgeRebar();
	if(!pBri) return;
	if(!pBri->m_bSelectBM_ApsCnsInclude) return;

	// ���ӽ����� ��Ǫ�� ǥ�� ����
	CBMData bmBase;
	bmBase.m_nIdxBigClass		= BM_BIG_FORM;
	bmBase.m_sBigClass			= GetStringBigClass(bmBase.m_nIdxBigClass);
	bmBase.m_nLevelTotal		= GetLevelTotal(bmBase.m_nIdxBigClass);
	bmBase.m_nIdxMidClass		= GetFormByPos(FORM_POS_APS);
	bmBase.m_sMidClass			= GetStringMidClass(bmBase.m_nIdxMidClass);
	bmBase.m_nIdxSmallClass		= BM_SMALL_FORM_VERTICAL;
	bmBase.m_sSmallClass		= GetStringSmallClass(bmBase.m_nIdxSmallClass);
	bmBase.m_nTypeResult		= RESULT_AREA;
	bmBase.m_sUnit				= "��";
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

				// ���潽���갡 ������ ������ũ��Ʈ�� ������ ������ ������ ����.
				double dExtLean		= 100;
				double dThickLean	= 100;
				if((bStt && pBri->m_cnsStt[left][0].m_bIs) || (!bStt && pBri->m_cnsEnd[left][0].m_bIs))
					dExtLean	= 0;

				// �ϴ� �������̳� �¿����� ���ӽ����꿡 ���� ��Ƽ������ ������� ����

				// 1. ���ӽ����� ����(���� ���̿��ٰ� �β� ���ϸ� ��, ���ų� �� �� ����)
				CTwinVector tvEndPlane	= pAps->m_tvArrPlan.GetTvByInfo("���ӽ����곡��");
				CTwinVector tvEndSide	= pAps->m_tvArrSide.GetTvByInfo("���ӽ����곡������");
				if(tvEndPlane.GetLength() > 0 && tvEndSide.GetLength() > 0)
				{
					CBMData *pBMEnd				= new CBMData(&bmBase);
					pBMEnd->m_nNumbering		= nNum++;
					pBMEnd->m_dV[WidthLower]	= Round(toM(tvEndPlane.GetLength()),3);
					pBMEnd->m_dV[Height]		= Round(toM(tvEndSide.GetLength()),3);
					pBMEnd->m_nShape			= SHAPE_TV_RECTANGLE;
					pBMEnd->CalculateResult();
					pArrBMStd->Add(pBMEnd);

					// ���潽���갡 ���ٸ� ���鿡�� ������ũ��Ʈ ��Ǫ�� ����.
					if(dExtLean > 0 && !bCns && pAps->m_dH2 == 0)
					{
						CBMData *pBMEndLean				= new CBMData(pBMEnd);
						pBMEndLean->m_nIdxMidClass		= GetFormByPos(FORM_POS_LEAN);
						pBMEndLean->m_sMidClass			= GetStringMidClass(pBMEndLean->m_nIdxMidClass);
						pBMEndLean->m_dV[WidthLower]	+= Round(toM(dExtLean*2),3);
						pBMEndLean->m_dV[Height]		= Round(toM(dThickLean),3);
						pBMEndLean->CalculateResult();
						pBMEndLean->m_sInfo				= "������ũ��Ʈ��Ǫ��";
						pArrBMStd->Add(pBMEndLean);
					}
				}
				
				// 2. ���ӽ����� ������(���߿� �������̳� ���� �ִ� ���ӽ������ ��ġ�� �κ� ���� ��)
				// �������� �� 2������ ���� �� ����
				for(long leftSide = 0; leftSide < 2; leftSide++)
				{
					BOOL bLeftSide	= leftSide == iLEFT;
					CTwinVector tvSidePlane	= pAps->m_tvArrPlan.GetTvByInfo(bLeftSide ? "���ӽ���������" : "���ӽ��������");
					
					// ����(�⺻)
					CBMData *pBMLeft			= new CBMData(&bmBase);
					pBMLeft->m_nNumbering		= nNum++;
					pBMLeft->m_dV[WidthLower]	= Round(toM(tvSidePlane.GetLength()),3);
					pBMLeft->m_dV[Height]		= Round(toM(tvEndSide.GetLength()),3);
					pBMLeft->m_nShape			= SHAPE_TV_RECTANGLE;
					pBMLeft->CalculateResult();
					pArrBMStd->Add(pBMLeft);

					// ������ũ��Ʈ(���潽���� ���� ���ο� ���� ������ũ��Ʈ ���̸� ���ؼ� ������)
					// ��ġ�� ������ ������ũ��Ʈ�� ����
					if(!bCns && pAps->m_dH2 == 0)
					{
						CBMData *pBMLeftLean		= new CBMData(pBMLeft);
						pBMLeftLean->m_nIdxMidClass	= GetFormByPos(FORM_POS_LEAN);;
						pBMLeftLean->m_sMidClass	= GetStringMidClass(pBMLeftLean->m_nIdxMidClass);
						pBMLeftLean->m_dV[WidthLower]	+= Round(toM(dExtLean),3);
						pBMLeftLean->m_dV[Height]		= Round(toM(dThickLean),3);
						pBMLeftLean->CalculateResult();
						pBMLeftLean->m_sInfo		= "������ũ��Ʈ��Ǫ��";
						pArrBMStd->Add(pBMLeftLean);
		
						/*
						// ��ġ�� 
						// ��ġ�δ� �������� ������ ���� �簢����� ���̰� �� �� �ִ�.
						if(pAps->m_dH2 != 0)
						{
							// L1�� L1+L2�� ���� ����
							double dL12	= pAps->m_dL1 + pAps->m_dL2;
							double dL1	= pAps->m_dL1;
							CDPoint xyMatch(0, 0);

							CTwinVector tvOrg	= pAps->m_tvArrPlan.GetTvByInfo("���ظ�");
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

					// ���ӽ����� �¿����� �ִ� ��Ƽ���� ������ŭ �����ؾ� ��.
					// ���� ������ ������ �ʰ�, �̹� ������ ������ �̿��ϸ��
					// ����, ���ӽ����꺸�� ��Ƽ���� ������ ���� ���ؾ� ��.
					CString sInfo	= _T("");
					sInfo.Format("��Ǫ�������뽺Ƽ����(%s)", bLeftSide ? "����" : "����");
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
							pBMTmp->m_sUnit				= "��";
							pBMTmp->m_dwSide				= SIDE_FRONT;
							pBMTmp->m_bIsUseForDeduct	= TRUE;
							pArrBMStd->Add(pBMTmp);
						}
					}
				}

				// ������ ���������� �߰��Ѵ�.
				RemakeByHeightBMList(pArrBMStd, dFirstMeter, dSeconeMeter, nSizeBM, pArrBMStd->GetSize()-1);
			}
		}
	}
}

// ���ӽ����� ��Ƽ����
// 1. ���ӽ������ ���� ���Ӻ�
// 2. ���ӽ������ ������ ���Ӻ�
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

	// ���ӽ����� ��Ǫ�� ǥ�� ����
	CBMData bmBase;
	bmBase.m_nIdxBigClass		= BM_BIG_STYROFOAM;
	bmBase.m_sBigClass			= GetStringBigClass(bmBase.m_nIdxBigClass);
	bmBase.m_nLevelTotal		= GetLevelTotal(bmBase.m_nIdxBigClass);

	//{ 28042
	bmBase.m_nIdxMidClass		= GetFormByPos(FORM_POS_APS);
	bmBase.m_sMidClass			= GetStringMidClass(nIdxMidClass);
	//}

	bmBase.m_nTypeResult		= RESULT_AREA;
	bmBase.m_sUnit				= "��";
	bmBase.m_dwSide				= SIDE_FRONT;
	// �������� ��¾��
// 	bmBase.m_nLevelMidClass		= 2; //#28042 ���� �������� ȯ�漳���� ���� ������ �� �� �����Ƿ� �ּ� ó���ϰ� ������ MidŬ���� �������� �����

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

			// 1. ������ ���Ӻ�(��ġ����)
			CTwinVector tvOrgPlane		= pAps->m_tvArrPlan.GetTvByInfo("���ظ�");
			CTwinVector tvHunchLPlane	= pAps->m_tvArrPlan.GetTvByInfo("������������ġ");
			CTwinVector tvHunchRPlane	= pAps->m_tvArrPlan.GetTvByInfo("������������ġ");
			
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
		
			// �β����� smallclass�ε����� ������ ��� �ȴ�. ///////////////////////
			long nIdxSameBM	= GetIdxSameBM(pArrBMStd, pBM1, exceptionFlag);
			if(nIdxSameBM > -1)
				pBM1->m_nIdxSmallClass	= pArrBMStd->GetAt(nIdxSameBM)->m_nIdxSmallClass;
			else
				nIdxSmallClass++;
			/////////////////////////////////////////////////////////////////////////

			CRebarPlacing rb;
			// 2. ����/���� �������� ���Ӻ�
			// ������ �Ÿ��� 100�����̾�� ��
			for(long leftSide = 0; leftSide < 2; leftSide++)
			{
				BOOL bLeftSide	= leftSide == iLEFT;
				double dAD		= bLeftSide	? dAD1 : dAD2;
				CString sLeft	= bLeftSide ? "����" : "����";

				CWingWall *pWing	= pBri->GetWingWall(bStt, bLeftSide);
				if(!pWing) continue;

				if(dAD <= 100 && pBri->IsWingWall(bStt, bLeftSide) && pWing->m_nAttachPos == 0)
				{
					pBri->GetTvWingWallPlane(bStt, bLeftSide);
					CDPoint xyMatch(0, 0);
					CTwinVector tvSidePlaneAps	= pAps->m_tvArrPlan.GetTvByInfo("���ӽ�����"+sLeft);
					CTwinVector tvSidePlaneWing	= pBri->m_tvArrPlaneWingWall.GetTvByInfo(sLeft+"������������ġ��");

					// �������� ���ӽ����� ����� �����ϴ�� ������ ����
					CDPoint vAngSideAps	= tvSidePlaneAps.GetXyDir();
					CDPoint vAngSideWing	= tvSidePlaneWing.GetXyDir();

					// �����϶� �̾ƾ� �ȴٰ� �س����� �����̸� continue��Ű�� �־���
					// �׷��� ������ �ƴ� ��쿡 continue��Ű���� ������(2009.05.19) 
					// ������ ���� ��Ȯ�� ����˻��ϴ°� ������ - ����1���� ��. (2009.09.22)
					if(!rb.IsParallel(vAngSideAps, vAngSideWing, 1)) continue;

					if(!GetXyMatchSegAndLine(tvSidePlaneAps.m_v1, tvSidePlaneAps.m_v2, tvSidePlaneWing.m_v1, tvSidePlaneAps.GetXyDir().Rotate90(), xyMatch))
						xyMatch	= tvSidePlaneAps.m_v2;
					double dLenOverlap	= ~(xyMatch-tvSidePlaneAps.m_v1);
					
					// �Ϲݸ�
					CBMData *pBM2	= new CBMData(&bmBase);
					pBM2->m_nIdxSmallClass	= nIdxSmallClass;
					pBM2->m_sSmallClass.Format("T=%.0fMM", dAD);
					pBM2->m_nShape			= SHAPE_TV_RECTANGLE;
					pBM2->m_dV[WidthLower]	= Round(toM(dLenOverlap),3);
					pBM2->m_dV[Height]		= Round(toM(dH1),3);
					pBM2->CalculateResult();
					pBM2->m_sInfo.Format("��Ǫ�������뽺Ƽ����(%s)", sLeft);
					pArrBMStd->Add(pBM2);

					// �β����� smallclass�ε����� ������ ��� �ȴ�. ///////////////////////
					long nIdxSameBM	= GetIdxSameBM(pArrBMStd, pBM2, exceptionFlag);
					if(nIdxSameBM > -1)
						pBM2->m_nIdxSmallClass	= pArrBMStd->GetAt(nIdxSameBM)->m_nIdxSmallClass;
					else
						nIdxSmallClass++;
					/////////////////////////////////////////////////////////////////////////
					
					// ��ġ��
					if(pAps->m_dL3 + pAps->m_dL2 > 0 && pAps->m_dH2 > 0)
					{
						// L3�κ�(���簢��)
						if(pAps->m_dL3 > 0)
						{
							CBMData *pBM3	= new CBMData(&bmBase);
							pBM3->m_nIdxSmallClass	= nIdxSmallClass;
							pBM3->m_sSmallClass.Format("T=%.0fMM", dAD);
							pBM3->m_nShape			= SHAPE_TV_RECTANGLE;
							pBM3->m_dV[WidthLower]	= Round(toM(min(dLenOverlap, pAps->m_dL3)),3);
							pBM3->m_dV[Height]		= Round(toM(dH2),3);
							pBM3->CalculateResult();
							pBM3->m_sInfo.Format("��Ǫ�������뽺Ƽ����(%s)", sLeft);
							pArrBMStd->Add(pBM3);
						}
						
						// L2�κ�(�ﰢ�� ~ ��ٸ���)
						if(pAps->m_dL2 > 0 && pAps->m_dL3 < dLenOverlap)
						{
							dLenOverlap	= dLenOverlap - pAps->m_dL3;
							if(dLenOverlap > pAps->m_dL2)	// ��� �����ϸ� �ﰢ��
							{
								CBMData *pBM4	= new CBMData(&bmBase);
								pBM4->m_nIdxSmallClass	= nIdxSmallClass;
								pBM4->m_sSmallClass.Format("T=%.0fMM", dAD);
								pBM4->m_nShape			= SHAPE_TV_TRIANGLE;
								pBM4->m_dV[WidthLower]	= Round(toM(min(dLenOverlap, pAps->m_dL2)),3);
								pBM4->m_dV[Height]		= Round(toM(dH2),3);
								pBM4->CalculateResult();
								pBM4->m_sInfo.Format("��Ǫ�������뽺Ƽ����(%s)", sLeft);
								pArrBMStd->Add(pBM4);	
							}
							else	// �ƴϸ� ��ٸ���
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
								pBM4->m_sInfo.Format("��Ǫ�������뽺Ƽ����(%s)", sLeft);
								pArrBMStd->Add(pBM4);	
							}
						}
					}
				}
			}
		}
	}
}

// ���� ������ �ִ��� �˻��ؼ� �ε��� ����
// long nLevel : 0 : big, 1 : mid, 2 : small ���� ��
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

// ���ӽ����� ������
// �������(���ӽ����� ��� ����)
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

			// ���ӽ����� ��� ���� ���ϱ� .
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
				pBM->m_sUnit				= "��";
				pBM->m_nShape				= pArea->m_nTypeShape;
				pBM->m_dV[WidthLower]		= Round(toM(pArea->m_dWidthLower),3);
				pBM->m_dV[WidthUpper]		= Round(toM(pArea->m_dWidthUpper),3);
				pBM->m_dV[WidthDiffLeft]	= Round(toM(pArea->m_dWidthDiffLeft),3);
				pBM->m_dV[WidthDiffRight]	= Round(toM(pArea->m_dWidthDiffRight),3);
				pBM->m_dV[Height]			= Round(toM(pArea->m_dHeight),3);
				pBM->m_dwSide				= SIDE_FRONT;
				pBM->CalculateResult();
				// �������� ��¾��
				pBM->m_nLevelMidClass		= 2;

				pArrBMStd->Add(pBM);
			}
		}
	}
}


// ������ ��Ǫ��
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
			CBMData *pBMBackBaseForHunch	= NULL;	// ��ü�� ���ϴ� �κ��� ��ġ�� ��Ǫ���� ���ϱ� ���� base ����

			baseBM.RemakeByHeightTvAreaShape(&pWing->m_pArrAreaConc, dYBase, dFirstMeter, dSeconeMeter);
			nSize	= pWing->m_pArrAreaConc.GetSize();
			for(i = 0; i < nSize; i++) 
			{
				double dThickWingWall	= pWing->m_nDirHunch == 1 ? pWing->m_dT2 : pWing->m_dT1;

				// ���� ����.. �׸��� 
				CTwinVectorAreaShape *pArea	= pWing->m_pArrAreaConc.GetAt(i);
				
				double dSttMeter	= pArea->GetTvLeft().GetXyBottom().y - dYBase;
				double dEndMeter	= pArea->GetTvLeft().GetXyTop().y - dYBase;

				// �ܸ麯ȭ���� ������  �������� ������ �β��� dT1�� �����Ѵ�.
				BOOL bChangleSection	= pArea->GetTvLower().m_sInfo.Find("�ܸ麯ȭ��", 0) != -1;
				if(bChangleSection)
				{
					dThickWingWall	= pWing->m_dT1;
				}
				
				// ������ ����ó���ΰ�� �β��� �簢�� ��������� �Ѵ�.
				BOOL bRAngEnd	= pWing->m_bRAngEnd;
				CDPoint vAng	= CDPoint(0, 1);
				if(!bRAngEnd)
				{
					// �����ڵ�� �����ǥ ���ϴ� ���� �����ؿ�
					CDPoint A[4];
					pBri->GetXySss(bStt ? BRIDGE_OFF_STT_STT : BRIDGE_OFF_END_END, A);
					// ���� �� ��ǥ
					int nJ = bStt ? 0 : pBri->m_nQtyJigan;
					CDPoint xyOutStt	= bStt ? A[3] : A[0];
					CDPoint xyOutEnd	= pBri->GetXyPlaneWingWallEnd(bStt, bLeft);
					CDPoint vAngJijum	= pBri->GetAngleJijum(nJ);//pBri->GetAngleAzimuthJijum(nJ);				// ������ ���� ����
					
					// ���� ����
					CDPoint	vAng = ToDPointFrDegree(vAngJijum.GetAngleDegree()+pWing->m_vAng.GetAngleDegree() - (pWing->m_nAttachPos == 1 ? 90 : 0));	// ���� ������ �ܺ� ����
					dThickWingWall /= vAng.y;
				}

				// ������ �������¿����� ���� ��Ǫ�� ���� ��ġ
				long nPosFront	= bStt ? (bLeft ? FORM_POS_WINGSL_FRONT : FORM_POS_WINGSR_FRONT) : 
										(bLeft ? FORM_POS_WINGEL_FRONT : FORM_POS_WINGER_FRONT);
				long nPosBack	= bStt ? (bLeft ? FORM_POS_WINGSL_BACK : FORM_POS_WINGSR_BACK) : 
										(bLeft ? FORM_POS_WINGEL_BACK : FORM_POS_WINGER_BACK);

				// ����
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
				pBM->m_sUnit				= "��";
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

				// ���
				CBMData *pBMBack			= new CBMData(pBM);
				pBMBackBaseForHunch			= pBMBack;
				pBMBack->m_nIdxMidClass		= GetFormByPos(nPosBack);
				pBMBack->m_sMidClass		= GetStringMidClass(pBMBack->m_nIdxMidClass);
				pBMBack->m_nIdxItemClass	++;
				pBMBack->m_sItemClass		= GetStringItemClass(pBMBack->m_nIdxItemClass);
				
				// 1. ��� ���Ҷ� �ܸ麯ȭ���� �ִ� ��� �ܸ� ��ȭ�� ��ŭ ������ �ʴ´�
				if(bChangleSection)
				{
					CTwinVectorArray tvArrSec;
					pBri->GetTvWingWallSection(bStt, bLeft, tvArrSec);
					CTwinVector tv	= tvArrSec.GetTvByInfo("�������ܸ鳻����ġ");
					if(tv.GetHorLength() > 0 && tv.GetVerLength() > 0)
					{
						pBMBack->m_dV[Height]	-= Round(toM(tv.GetVerLength()),3);
						pBMBack->m_dV[SttMeter]	+= Round(toM(tv.GetVerLength()),3);
						if(pBMBack->m_dV[Height] < 0) pBMBack->m_dV[Height]	= 0;
					}
				}

				// 2. ��� ���Ҷ� ���ʿ� ��ġ�� �κ��� ��ġ���� ��ŭ�� ����� �Ѵ�.
				// 33072 ����� ���̸� ���Ҷ� ��ġ ���� ��ŭ ����� �ϴ� ���� ������ �Ǳ⶧���� �ּ� ó����
				//���� �ٽ� ������ �Ǿ��� �ÿ��� �ٸ� ������� �Ʒ��� �����ؾ� ��
// 				if(pArea->m_tvArr.GetTvByInfo("�����������ϴ�", tv))
// 				{
// 					pBMBack->m_dV[Height]	-= Round(toM(pWing->m_dHuH1),3);
// 					pBMBack->m_sVC[Height]	= _T("");
// 				}

				// 3. ��� ���Ҷ� ��ü�� ��ġ�� �κ��� ��ġ�ʺ� ��ŭ ����� �Ѵ�.
				if(pArea->m_tvArr.GetTvByInfo("��ü", tv))
				{
					CTwinVector tvH;
					CString sLeft	= bLeft ? "����" : "����";
					pBri->GetTvWingWallPlane(bStt, TRUE);
					if(pBri->m_tvArrPlaneWingWall.GetTvByInfo(sLeft+"������������ġ", tvH))
					{
						pBMBack->m_dV[WidthLower]	-= Round(toM(pWing->m_dHuW2),3);
						pBMBack->m_dV[WidthUpper]	-= Round(toM(pWing->m_dHuW2),3);
					}
				}				

				// ���� ��Ǫ���� ���鸸
				// �ﰢ���̸� �������� ���Ƿ�...���ش�.
				if(pBMBack->m_nShape == SHAPE_TV_TRIANGLE || pBMBack->m_nShape == SHAPE_TV_REVTRIANGLE)
				{
					pBMBack->m_dwSide = SIDE_BACK;
				}
				else
				{
					bStt ? pBMBack->m_dwSide = SIDE_BACK | SIDE_LEFT : pBMBack->m_dwSide = SIDE_BACK | SIDE_RIGHT;
				}

				pBMBack->CalculateResult();
				pBMBack->m_sInfo = "��������Ǫ��(���)";
				pArrBMStd->Add(pBMBack);

				// ��鿡�� ���ﰢ���� ��쿡�� ������ �߰� ��
				if(pBMBack->m_nShape == SHAPE_TV_REVTRIANGLE)
				{
					CBMData *pBMDataSlope	= new CBMData(pBMBack);
					bStt ? pBMDataSlope->m_dwSide = SIDE_LEFT : pBMDataSlope->m_dwSide = SIDE_RIGHT;
					pBMDataSlope->m_nIdxSmallClass		= BM_SMALL_FORM_SLOPE;
					pBMDataSlope->m_sSmallClass			= GetStringSmallClass(pBMDataSlope->m_nIdxSmallClass);
					pBMDataSlope->CalculateResult();

					pArrBMStd->Add(pBMDataSlope);
				}

				// ��� ���ѵ� ������ ���� ���� ���� (��ġ�ε�)
				// 1. ����� ���ϸ� �ܸ� ��ȭ���� �ִ� ��� �ű� ��ġ�ο� ���� ���� ��Ǫ�� ���ؾ� ��
				if(bChangleSection)
				{
					CTwinVectorArray tvArrSec;
					pBri->GetTvWingWallSection(bStt, bLeft, tvArrSec);
					CTwinVector tv	= tvArrSec.GetTvByInfo("�������ܸ鳻����ġ");
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
						pBMHunch->m_sInfo				= "��������Ǫ��(�����ġ)";
						pArrBMStd->Add(pBMHunch);

						//33072 
						//��ġ �ε鿡 ���� ���� ���ߴٸ� ���鿡 �簢����� ��Ǫ�� ������ ���� �־�� �Ѵ�(��ȭ����)
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
						pBMRect->m_sInfo				= _T(""); //�̰��� Info�� ���� ����
						pArrBMStd->Add(pBMRect);
					}
				}

				// 2. ��� ���Ҷ� ���ʿ� ��ġ�� �κ��� ��ġ���� ��ŭ�� ���� ����
				if(pArea->m_tvArr.GetTvByInfo("�����������ϴ�", tv))
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
						pBMHunch->m_sInfo				= "��������Ǫ��(�����ġ)";
						pBMHunch->CalculateResult();
						pArrBMStd->Add(pBMHunch);
					}					
				}
			} // end of for(i = 0; i < nSize; i++)


			// 3. ��ü�� ��ġ�� �κ��� ��ġ�δ� ���������� �ѹ��� �����.
			CTwinVectorArray tvArrWing;
			int se = bStt ? -1 : 1;
			int nLeft = bLeft ? -se : se;	// ������ ��� ���� ���� ������ �¿�� �������� �¿찡 �ݴ���.
			pBri->GetTvVertSection(nLeft, TRUE, TRUE);
			pBri->GetTvWingWallFront(bStt, bLeft, 1, tvArrWing, TRUE, FALSE, TRUE, TRUE);
			pBri->GetTvWingWallPlane(bStt);
			CTwinVector tvHunch	= pBri->m_tvArrPlaneWingWall.GetTvByInfo(bLeft ? "����������������ġ" : "����������������ġ");
			double dLenHunch	= Round(toM(tvHunch.GetLength()),3);
			CTwinVectorArray tvArrHunch;
			for(long h = 0; h < tvArrWing.GetSize(); h++)
			{
				tv	= tvArrWing.GetAt(h).Sort(FALSE);
				if(tv.m_sInfo == "��ü")
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

				// �߶� ����Ҳ�.. -> tvArrTmp
				// �ڸ��� ���°� -> tvArrHunch
				rb.TrimTvArrayByLine(tvArrTmp, tvCut.m_v1, tvCut.m_v2, TRUE, TRUE, FALSE);
				rb.TrimTvArrayByLine(tvArrHunch, tvCut.m_v1, tvCut.m_v2, FALSE, TRUE, FALSE);

				double dSttMeter	= tvArrTmp.GetXyBottom().y - dYBase;
				double dEndMeter	= tvArrTmp.GetXyTop().y - dYBase;

				// ���� ������
				// ������� ���� �ΰ��� ���� �� ����
				CBMData *pBMVert = new CBMData(pBMBackBaseForHunch);
				pBMVert->m_dV[Result] = 0;		// ���� ����ִ� ��찡 �־� �ʱ�ȭ ��Ų��.
				CBMData *pBMSlope = new CBMData(pBMBackBaseForHunch);
				pBMSlope->m_dV[Result] = 0;		// ���� ����ִ� ��찡 �־� �ʱ�ȭ ��Ų��.

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
					pBMVert->m_nNumbering		= 99999;	// 99999�� �ѹ������� ������ �ȵǰ� �����Ҷ��� ���������� ���ߵǴ� ��� �����
					pBMVert->m_dV[SttMeter]		= Round(toM(dSttMeter),3);
					pBMVert->m_sVC[SttMeter]	= _T("");
					pBMVert->m_dV[EndMeter]		= Round(toM(dEndMeter),3);
					pBMVert->m_sVC[EndMeter]	= _T("");
					str	= pBMVert->m_sForm;
					pBMVert->m_sForm.Format("(%s) x %.3f", str, dLenHunch);
					pBMVert->m_dV[Result] *= Round(dLenHunch,3);
					pBMVert->m_sNoteHead	= "��ġ��";
					pArrBMStd->Add(pBMVert);
				}
				else 
					delete pBMVert;

				if(pBMSlope->m_sForm != "")
				{
					pBMSlope->m_nNumbering		= 99999;	// 99999�� �ѹ������� ������ �ȵǰ� �����Ҷ��� ���������� ���ߵǴ� ��� �����
					pBMSlope->m_dV[SttMeter]	= Round(toM(dSttMeter),3);
					pBMSlope->m_sVC[SttMeter]	= _T("");
					pBMSlope->m_dV[EndMeter]	= Round(toM(dEndMeter),3);
					pBMSlope->m_sVC[EndMeter]	= _T("");
					str = pBMSlope->m_sForm;
					pBMSlope->m_sForm.Format("(%s) x %.3f", str, dLenHunch);
					pBMSlope->m_dV[Result] *= Round(dLenHunch,3);
					pBMSlope->m_sNoteHead	= "��ġ��";
					pArrBMStd->Add(pBMSlope);
				}
				else
					delete pBMSlope;

				// ���� �ø�
				dCurMeter += dSeconeMeter;
			}

			// ������ ���������� �߰��Ѵ�.
			RemakeByHeightBMList(pArrBMStd, dFirstMeter, dSeconeMeter, nSizeBM-1, pArrBMStd->GetSize()-1);
		}
	}

	pBri->m_tvArrVertSection	= tvArrVertSection;
	pBri->m_nTypeJongdanForRebar	= nTypeJongdan;
}

// ������ �����̼� ����(��ü��)
void COutBM::MakeBMSpacerWing()
{
	CRcBridgeRebar *pBri	= GetRcBridgeRebar(TRUE);
	if(!pBri) return;


	long stt = 0; for(stt = 0; stt < 2; stt++)
	{
		for(long left = 0; left < 2; left++)
		{
			CTypedPtrArray <CObArray, CBMData*> *pArrBMStd	= &m_pArrBMWingStd[stt][left];

			// ö���� ���� �κ��� ��Ǫ���� ��� �����Ѵ�.
			// ����,���
			CTypedPtrArray <CObArray, CBMData*> pArrFormVert;
			long nSizeBM	= pArrBMStd->GetSize();
			long i = 0; for(i = 0; i < nSizeBM; i++)
			{
				CBMData *pBM = pArrBMStd->GetAt(i);
				if(pBM->m_nIdxBigClass == BM_BIG_FORM)
				{
					if(pBM->m_sSmallClass.Find("����", 0) != -1) continue;

					CBMData *pForm = new CBMData;
					*pForm = *pBM;
					// ��Ǫ¤ �������� ������ ������ ��ȣ�� ������� ���ĵ��� �ʴ´�.
					// �׷��� ���⼭�� ��Ǫ�� ������ ��� ���� ������ ���ش�.
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
			
			// ����
			CHgBaseBMStd baseBM;
			baseBM.ShellSortBMList(&pArrFormVert, &CBMData::CompareBM);

			// ��ü�� �����̼� ���� ����
			if(pArrFormVert.GetSize() > 0)
			{
				CBMData *pWall				= new CBMData;
				pWall->m_nIdxBigClass		= BM_BIG_SPACER;
				pWall->m_sBigClass			= GetStringBigClass(pWall->m_nIdxBigClass);
				pWall->m_nLevelTotal		= GetLevelTotal(pWall->m_nIdxBigClass);
				pWall->m_nIdxSmallClass		= BM_SMALL_SPACER_WALL;
				pWall->m_sSmallClass		= GetStringSmallClass(pWall->m_nIdxSmallClass);
				pWall->m_nTypeResult		= RESULT_AREA;
				pWall->m_sUnit				= "��";
				pWall->m_sInfo				= "��������ü�뽺���̼�";

				// ������ ��Ǫ�� �������� �߰���Ŵ
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
				pWall->m_sForm = (str + "�� ��Ǫ�� ������ ����(�Ų��� ����/���̰�Ǫ��)");
				pWall->CalculateResult();
				// �������� ��¾��
				pWall->m_nLevelMidClass		= 2;				

				pArrBMStd->Add(pWall);
			}
			AhTPADelete(&pArrFormVert,(CBMData*)0);
		}
	}
}

// ��ũ��Ʈ ��� ������
// ������ ��Ǫ�� ���, ��� 
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

			// ��Ǫ���� ��� �����Ѵ�. //////////////////////////////////////////////
			// ������ũ��Ʈ�� mass��ũ��Ʈ, ���ʸ� ������ ��� ��Ǫ���� ����
			CTypedPtrArray <CObArray, CBMData*> pArrForm;
			long nSizeBM	= pArrBMStd->GetSize();
			long i = 0; for(i = 0; i < nSizeBM; i++)
			{
				CBMData *pBM = pArrBMStd->GetAt(i);
				if(pBM->m_nIdxBigClass == BM_BIG_FORM)
				{
					if(pBM->m_sSmallClass.Find("����", 0) != -1) continue;

					CBMData *pForm = new CBMData;
					*pForm = *pBM;
					// ��Ǫ¤ �������� ������ ������ ��ȣ�� ������� ���ĵ��� �ʴ´�.
					// �׷��� ���⼭�� ��Ǫ�� ������ ��� ���� ������ ���ش�.
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
			
			// ����
			CHgBaseBMStd baseBM;
			baseBM.ShellSortBMList(&pArrForm, &CBMData::CompareBM);

			// ��Ǫ�� ���� �߰�
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
				pBM->m_sUnit			= "��";
				pBM->m_sInfo			= "��ũ��Ʈ���(��Ǫ��)";

				// ������ ��Ǫ�� �������� �߰���Ŵ
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
				pBM->m_sForm = (str + "�� ��Ǫ��(����, ���) ������ ����");
				
				pBM->m_bIsPrintByRefForm	= FALSE;
				pBM->CalculateResult();
				pArrBMStd->Add(pBM);
			}
			AhTPADelete(&pArrForm, (CBMData*)0);
			// ��Ǫ���� ��� �����Ѵ�. //////////////////////////////////////////////



			// ������ ��� �߰� /////////////////////////////////////////////////////
			// ������ ����� �׻� ������ �����̴�.
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
			pBM->m_sUnit			= "��";
			pBM->m_nShape			= SHAPE_TV_RECTANGLE;
			CTwinVectorArray tvArrFront;
			CTwinVectorArray tvArrVertSection;
			tvArrVertSection	= pBri->m_tvArrVertSection;
			pBri->GetTvVertSection(bLeft ? 0 : 1, TRUE, TRUE); //31503 
			pBri->GetTvWingWallFront(bStt, bLeft, 1, tvArrFront, TRUE, FALSE, TRUE, TRUE);
			pBri->m_tvArrVertSection	= tvArrVertSection;
			CTwinVector tvUpper;
			tvArrFront.GetTvByInfo("������������", tvUpper);
			pBM->m_dV[WidthLower]	= Round(toM(tvUpper.GetLength()),3);
			pBM->m_dV[Height]		= Round(toM(pWing->m_dT1),3);
			pBM->m_dwSide			= SIDE_FRONT;
			pBM->m_sInfo			= "��ũ��Ʈ���(��Ǫ��)";
			pBM->CalculateResult();
			pArrBMStd->Add(pBM);		
			// ������ ��� �߰� /////////////////////////////////////////////////////
			
		}
	}
}

// ������ ���� ����
// long nForUse : 0 : ���� �뵵(��ũ��Ʈ ����)  
//                1 : ��ä��� ������ ���� : ��ä�� ������ ������ �κи� ����
//                2 : �Ǹ޿���� ��ä��� ������ ���� : ���ݼ������� ��ä�� ������ ������ �κи� ����
//                3 (���������) : ��� ���Ϻ�(�Ϻ��Ѱ�� ���ݼ�)
//                4 (��ü�������) : ��ü ���Ϻ�(�Ϻ��Ѱ�� ���ݼ�)-----5���� ���´�. nForUse+1
//				  6 : �Ǹ޿���� ��ü ����
void COutBM::CalAreaConcWing(CRcBridgeRebar *pBri, long nForUse)
{
	if(!pBri) return;

	CRebarPlacing rb;
	long nNum	= 0;
	long nIdx	= 0;

	// ������
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

			// ������ ������ tvAreaShape�� ����
			CTwinVectorArray tvArr;
			CTwinVectorArray tvArrResult;
			CDoubleArray dArrAngleSu;
			int se = bStt ? -1 : 1;
			int nLeft = bLeft ? -se : se;	// ������ ��� ���� ���� ������ �¿�� �������� �¿찡 �ݴ���.
			pBri->GetTvVertSection_Std(nLeft, TRUE, TRUE, FALSE, 2);
			//pBri->SyncBridge(nLeft, TRUE, TRUE);
			//pBri->SyncBridgeAps(nLeft, FALSE);

			// ��ä��������Ҷ� ������ ������ �ʿ�� �� ���� �ִ�
			// �� ���� ���ؼ� ��ä�� �������� ������ ��ŭ���� ������ ������ �������� ������ ������.
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

			// �������� ���� ���� �ڿ� �ܸ麯ȭ���� ���ؼ� �ѹ� �� �߶��ش�.
			// ����� ���κ�ȭ�� ������
			// ��ȭ�Ǵ¼��� "�ܸ麯ȭ��"�̶�� �����صд�
			if(pWing->m_nDirHunch == 1)	// ���κ�ȭ
			{
				CTwinVectorArray tvArrSec;
				CTwinVector tv;
				pBri->GetTvWingWallSection(bStt, bLeft, tvArrSec);
				if(tvArrSec.GetTvByInfo("�������ܸ鳻����ġ", tv))
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
								long nNum	= 1;	// ���κ�ȭ�� �߸��ں��ʹ� ��ȣ�� �ϳ��� ���� �Ǿ�� �Ѵ�. �׷��� ��ȣ�� �ٽ� �ο�

								tvLower.m_sInfo += "(�ܸ麯ȭ��)";
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
								
								// �߸��� �Ʒ��� ����, ���� ���λ���
								CTwinVector tvLowerTmp, tvLeftTmp, tvRightTmp, tvUpperTmp;

								// �Ʒ�������
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

								// �Ʒ��� �߰�
								CTwinVectorAreaShape *pAreaNew	= new CTwinVectorAreaShape;
								tvLowerTmp.m_sInfo	= "(�ܸ麯ȭ��)";
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
			else if(pWing->m_nDirHunch == 2) // ���κ�ȭ
			{
			}
			pBri->GetTvVertSection_Std(nLeft, TRUE, TRUE, TRUE, 2);
		}
	}
}

// ��ȣ�� ���� ��ũ��Ʈ
// ��ȣ���� ������ ��� ������ ���Ѵ�(ü������ ������ ����)
// ������ũ��Ʈ = ��ȣ�� ���� * ���ʹ� ��ᷮ 
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

		// ��ȣ�� �������
		COutBMTogong *outBMTogong	= m_pStd->m_pArrOutBMTogong.GetAt(m_nBri);
		double dLenHor	= outBMTogong->GetLengthBohoHor(bStt);
		if(dLenHor <= 0) continue;
				
		long nCountWing			= 0;
		if(pBri->IsWingWall(bStt, TRUE)) nCountWing ++;
		if(pBri->IsWingWall(bStt, FALSE)) nCountWing ++;
		double dLenWall			= toM(pBri->GetTvJijum(nJ, 0).GetLength());
		double dLenBohoFooting	= toM(2*ConstPi*dLenHor*dAngleBoho*nCountWing/360) + dLenWall;	// ��ȣ������ �� ����
		double dMaterialConc	= Compare(pWing->m_dSlopeNoriNext, 1.8, "=") ? m_pStd->m_dMaterialConc[1] : m_pStd->m_dMaterialConc[0];	// ���ʹ� ��ũ��Ʈ ��ᷮ

		// ���� ����(���ʺ���ũ��Ʈ ���ʹ� �� * �� ����)
		CBMData *pBM				= new CBMData;
		pBM->m_nIdxBigClass			= BM_BIG_CONCRETE;
		pBM->m_sBigClass			= GetStringBigClass(pBM->m_nIdxBigClass);
		pBM->m_nLevelTotal			= GetLevelTotal(pBM->m_nIdxBigClass);
		pBM->m_nIdxMidClass			= BM_MID_CONCRETE_BOHO;
		pBM->m_sMidClass			= GetStringMidClass(pBM->m_nIdxMidClass);
		pBM->m_nIdxSmallClass		= BM_SMALL_CONCRETE_BOHO_FOOTING;
		pBM->m_sSmallClass			= GetStringSmallClass(pBM->m_nIdxSmallClass);
		pBM->m_nTypeResult			= RESULT_VOLUMN;
		pBM->m_sUnit				= "��";
		pBM->m_sForm.Format("%.3f x %.3f", dLenBohoFooting, dMaterialConc);
		pBM->SetResult(dLenBohoFooting * dMaterialConc);
		pBM->m_bIsPrintByRefForm	= TRUE;
		m_pArrBMBohoStd[stt].Add(pBM);
	}
}

// ��ȣ��
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

		// ��ȣ�� ������ ���Ѵ�..
		CTwinVectorArray tvArrBoho;
		pBri->GetTvSungtoLine(tvArrBoho, bStt, TRUE, TRUE);

		long nSize	= tvArrBoho.GetSize();
		if(nSize == 0) 
		{
			// ������������ ��� �� ���Ҽ��� �ִ�. 
			// �����������ʿ����� �ѹ� ������.
			pBri->GetTvSungtoLine(tvArrBoho, bStt, FALSE, TRUE);
			nSize = tvArrBoho.GetSize();
			if(nSize == 0)
			{
				return;
			}
		}

		// ��ȣ�� �ѱ���..
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

		// ���� ����
		CTwinVector tvJigan	= pBri->GetTvJijum(nJ, 0);
		CBMData *pBM			= new CBMData;
		pBM->m_nIdxBigClass		= BM_BIG_BOHOBLOCK;
		pBM->m_sBigClass		= GetStringBigClass(pBM->m_nIdxBigClass);
		pBM->m_nLevelTotal		= GetLevelTotal(pBM->m_nIdxBigClass);
		pBM->m_nIdxItemClass	= bStt ? BM_ITEM_BOHOBLOCK_SF : BM_ITEM_BOHOBLOCK_EF;
		pBM->m_sItemClass		= GetStringItemClass(pBM->m_nIdxItemClass);
		pBM->m_nTypeResult		= RESULT_AREA;
		pBM->m_sUnit			= "��";
		pBM->m_sNoteHead		= sNote;
		pBM->m_sForm.Format("%.3f x %.3f", toM(dLenBoho), toM(tvJigan.GetLength()));
		pBM->SetResult(toM(dLenBoho) * toM(tvJigan.GetLength()));
		pBM->m_bIsPrintByRefForm	= TRUE;
		pBM->m_sVCN[Result]		= "��ȣ��";
		m_pArrBMBohoStd[stt].Add(pBM);
		
		// ���� ��ȣ�� ����
		// �������� �ִ� ������ŭ ������ ������.
		// ���԰ѳ��� ���� : �缱*�Ʒ�������*��
		if(nSize > 0)
		{
			long nCountWing		= 0;
			if(pBri->IsWingWall(bStt, TRUE)) nCountWing++;
			if(pBri->IsWingWall(bStt, FALSE)) nCountWing++;

			CDPoint xyOrg		= tvArrBoho.GetAt(0).m_v1;	// ������ �߽���
			CDPoint xyDirOrg	= CDPoint(0, -1);
			CDPoint xyMatch		= CDPoint(0, 0);

// 			double dLenSlopeLower	= 0;	// ������ �缱����
// 			double dLenSlopeUpper	= 0;	// ���� ������ �߸������� ��� �ڸ����� ���� ���� ������ �缱����.
// 			double dRLower			= 0;	// ������ ������
// 			double dRUpper			= 0;	// ���� ������ �߸������� ��� �߸����� ���� ���� ������ ������.
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
				pSide->m_sUnit			= "��";
				if(Compare(dRUpper, 0.0, "="))
				{
					pSide->m_sForm.Format("(%.3f x %.3f x ��) x (%.3f x %d / 360)", dLenSlopeLower, dRLower, dAngleBoho, nCountWing);
					pSide->SetResult((dLenSlopeLower*dRLower*ConstPi) * (dAngleBoho*nCountWing/360));
				}
				else
				{
					pSide->m_sForm.Format("(((%.3f + %.3f) x %.3f x ��) - (%.3f x %.3f x ��)) x (%.3f x %d / 360)", dLenSlopeLower, dLenSlopeUpper, dRLower, dLenSlopeUpper, dRUpper, dAngleBoho, nCountWing);
					pSide->SetResult((((dLenSlopeLower+dLenSlopeUpper)*dRLower*ConstPi) - (dLenSlopeUpper*dRUpper*ConstPi)) * (dAngleBoho*nCountWing/360));
				}
				pSide->m_bIsPrintByRefForm	= TRUE;
				pSide->m_sVCN[Result]		= "��ȣ��";
				m_pArrBMBohoStd[stt].Add(pSide);
			}
		}
	}
}

// ��ȣ�� ��ä�� ��ũ��Ʈ
void COutBM::MakeBMConcBohoBackFill()
{
	CRcBridgeRebar *pBri	= GetRcBridgeRebar();
	if(!pBri) return;
	long nTypeBoho	= pBri->m_nTypeRcBridgeBlock;
	if(nTypeBoho == 0) return;
	
	long stt = 0; for(stt = 0; stt < 2; stt++)
	{
		// ���� ����(��ȣ�� ���� * ��ũ��Ʈ �β�)
		// ��ȣ�� ���� ����
		double dBoho	= 0;
		long i = 0; for(i = 0; i < m_pArrBMBohoStd[stt].GetSize(); i++)
		{
			CBMData *pBM	= m_pArrBMBohoStd[stt].GetAt(i);
			if(pBM->m_sBigClass == "��ȣ��")
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
			pBM->m_sUnit				= "��";
			pBM->m_sForm.Format("%.3f x %.3f", dBoho, 0.1);
			pBM->m_sFormExcel.Format("#��ȣ��:%.3f# x 0.1", dBoho);
			pBM->SetResult(dBoho * 0.1);
			pBM->m_bIsPrintByRefForm	= TRUE;
			m_pArrBMBohoStd[stt].Add(pBM);
		}
	}
}

// ��ȣ�� ��Ǫ��
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
		// ��ȣ�� ������ ���Ѵ�..
		CTwinVectorArray tvArrBoho;
		pBri->GetTvSungtoLine(tvArrBoho, bStt, bLeft, TRUE);

		long nSize	= tvArrBoho.GetSize();
		if(nSize == 0) return;

		// �ʿ��� ����
		double dLenBoho	= 0;	// ��ȣ�� �ѱ���
		long i = 0; for(i = 0; i < nSize; i++)
			dLenBoho	+= toM(tvArrBoho.GetAt(i).GetLength());

		// ��ȣ�� ��Ǫ�� (��ģ����) 
		// ��ģ����(������ũ��Ʈ)�� ���ֿ� ���ؼ� ��Ǫ�� ������ �ʴ��� �ǹ��� ����.. ???? 
		CBMData *pBM				= new CBMData;
		pBM->m_nIdxBigClass			= BM_BIG_FORM;
		pBM->m_sBigClass			= GetStringBigClass(pBM->m_nIdxBigClass);
		pBM->m_nLevelTotal			= GetLevelTotal(pBM->m_nIdxBigClass);
		pBM->m_nIdxMidClass			= BM_MID_FORM_6TIMES;
		pBM->m_sMidClass			= GetStringMidClass(pBM->m_nIdxMidClass);
		pBM->m_nIdxSmallClass		= BM_SMALL_FORM_VERTICAL;
		pBM->m_sSmallClass			= GetStringSmallClass(pBM->m_nIdxSmallClass);
		pBM->m_nTypeResult			= RESULT_AREA;
		pBM->m_sUnit				= "��";
		pBM->m_dV[SttMeter]		= 0;
		pBM->m_dV[EndMeter]		= 1;
		pBM->m_sForm.Format("%.3f x %.3f x 2EA", dLenBoho, 0.1);
		pBM->m_bIsPrintByRefForm	= TRUE;
		pBM->SetResult(dLenBoho * 0.1 * 2);
		pArrBMStd->Add(pBM);

		// �ʿ��� ����
		COutBMTogong *pOutBMTogong	= m_pStd->m_pArrOutBMTogong.GetAt(m_nBri);
		long nBLType			= pBri->m_nTypeRcBridgeBlock == 2 ? 0 : 1;
		long nGugeType			= Compare(pWing->m_dSlopeNoriNext, 1.8, "=") ? 1 : 0;
		double dAngleBoho		= pBri->m_dAngleRcBridgeBlock;
		double dMaterialForm	= m_pStd->m_dMaterialForm[nGugeType];	// ���ʹ� ��Ǫ�� ��ᷮ
		double dLenHorBoho		= pOutBMTogong->GetLengthBohoHor(bStt);
		long nCountWing			= 0;
		if(pBri->IsWingWall(bStt, TRUE)) nCountWing++;
		if(pBri->IsWingWall(bStt, FALSE)) nCountWing++;
		double dSideLength		= toM((2*ConstPi*dLenHorBoho)*(dAngleBoho*nCountWing/360));	// ��ȣ��.. ����
		
		// ���� ��Ǫ��(���븶��) ����
		double dMaxH			= m_pStd->m_dBLBaseMaxHeight[nBLType][nGugeType];
		double dMinH			= m_pStd->m_dBLBaseMinHeight[nBLType][nGugeType];
		double dMaxW			= m_pStd->m_dBLBaseMaxWidth[nBLType][nGugeType];
		double dMinW			= m_pStd->m_dBLBaseMinWidth[nBLType][nGugeType];
		dLenBoho				+= dSideLength;

		// ��ᷮ ���� + �糡�� 2��
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
		pBM1->m_sUnit				= "��";
		pBM1->m_sForm.Format("(%.3f x %.3f) + ((%.3f + %.3f) / 2 x %.3f + (%.3f x %.3f)) x 2EA", 
			dLenBoho, dMaterialForm, dMinW, dMaxW, (dMaxH-dMinH), dMaxW, dMinH);
		pBM1->m_bIsPrintByRefForm	= TRUE;
		pBM1->SetResult((dLenBoho * dMaterialForm) + ((dMinW + dMaxW) / 2 * (dMaxH-dMinH) + (dMaxW * dMinH)) * 2);
		pArrBMStd->Add(pBM1);

		RemakeByHeightBMList(pArrBMStd, 7000, 3000, nSizeBM-1, pArrBMStd->GetSize()-1);
	}	
}

// ������ ���ٸ�
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
			nNum	= GetTvAreaShapePostWing(&m_pArrAreaConc, bStt, bLeft, nNum);	// ����� ���� ���ϱ�.

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
				pBM->m_sUnit				= "��";
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

				// �������� ��¾��
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
	int nLeft = bLeft ? -se : se;	// ������ ��� ���� ���� ������ �¿�� �������� �¿찡 �ݴ���.
	pBri->GetTvVertSection_Std(nLeft, TRUE, TRUE, FALSE, 2);
	
	long nNum	= nBaseNum;
	CTwinVectorArray tvArrWing;
	pBri->GetTvPostWing(tvArrWing, bStt, bLeft, pBri->m_nTypePostBaseLine);
	
	// ȸ���� ��ٸ����� �����ϹǷ� ���������� ���� �ʰ� �׳� ����
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
	// ���ٸ� ���� ������ ��ġ ����
	// ���� 3.5���Ϳ� ��ġ�ϰ�, ���� 2���� ���� ��ġ�Ѵ�.
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
			nNum	= GetTvAreaShapePostWing(&m_pArrAreaConc, bStt, bLeft, nNum);	// ����� ���� ���ϱ�.
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

					// ������ �¿��� ������ ã��
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
					pBM->m_sNoteHead.Format("���򿬰���(h=%.1f�ʰ��� ���� %.1fm���� ��ġ)", toM(dFirstHeight), toM(dSecondHeight));
					pBM->m_nTypeResult			= RESULT_AREA;
					pBM->m_sUnit				= "��";
					pBM->m_nShape				= SHAPE_TV_RECTANGLE;
					pBM->m_dV[WidthLower]		= Round(toM(fabs(xyMatchRight.x-xyMatchLeft.x)),3);
					pBM->m_dV[Length]			= Round(toM(pWing->m_dT2),3);
					pBM->m_dwSide				= SIDE_LOWER;
					pBM->CalculateResult();
					// �������� ��¾��
					pBM->m_nLevelMidClass		= 2;
					pArrBMStd->Add(pBM);
				}
			}
		}
	}
}
// �Լ� ���� DataStd�� �̵�
/*
// ������ ���ϱ� ���� ���ο� ���� ����
// BOOL bForTogong : ��� �������� ��� ���� ���Ѵ�.(���ı���� EL�� ������ ��Ʊ� ������)
void COutBM::MakeRcBridgeForBM(BOOL bForTogong)
{
	if(m_nBri < 0 || m_nBri > m_pStd->m_pARcBridgeDataStd->GetBridgeSize()-1) return;
	CRcBridgeRebar *pBri	 = m_pStd->m_pARcBridgeDataStd->GetBridgeRebar(m_nBri);
	CLineInfo *pLineOrg	= pBri->GetLineBase();
	
	// ���� �̶�� ������ ������� �ڸ��� �ʰ� �ϳ��� ����.
	if(pBri->m_bIsExp && pBri->m_nHeightForSlabBM == 0)
		pBri->m_nHeightForSlabBM	= 1;

	*m_pStd->m_pBri	= *pBri;
	m_pStd->m_pBri->m_pARcBridgeDataStd	= pBri->m_pARcBridgeDataStd;

	// �������� �׸�. //////
	// Box���� ���� ������ ��ü���̸� ������� ���� �������� ������ش�.
	m_pStd->m_pBri->m_nTypePlacingMainRebar_UpperSlab	= 0;
	if(m_pStd->m_pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP || m_pStd->m_pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP2 ||
		m_pStd->m_pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
		m_pStd->m_pBri->m_nTypeJongdanForRebar	= 1;
	else
		m_pStd->m_pBri->m_nTypeJongdanForRebar	= 0;

	// Box�����϶��� ��������� 
	if(bForTogong && pBri->IsBoxType() == FALSE)
		m_pStd->m_pBri->m_nTypeJongdanForRebar	= 1;	// ����� ��� ������ ����
	/////////////////////////



	// ��ü ���� ���� �ɼǿ� ���� �򱸹�� ��ȹ�� ���� /////////
	CLineInfo *pLine	= m_pStd->m_pBri->GetLineBase();
	//*pLine	= *pBri->GetLineBase();
	if(m_pStd->m_pBri->m_nTypeHeightWall == 0)	// �����߽� ���� ����
	{
		// �״�� ����ϸ� ��.
	}
	else if(m_pStd->m_pBri->m_nTypeHeightWall == 1 || m_pStd->m_pBri->m_nTypeHeightWall == 2 || m_pStd->m_pBri->m_nTypeHeightWall == 3)	// 1 : ��ճ��� ����(�ִ�+�ּ�)/2, 2 : �ִ� ��ȹ��, 3 : ������ ��� ����
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

		// ��� ���踦 0���� ����
		// ��
		for(i = 0; i < pLine->m_pSlopLeft.GetSize(); i++)
		{
			CSlop *pSlope	= pLine->m_pSlopLeft.GetAt(i);
			pSlope->m_dValuePyungubae	= 0;
		}

		// ��
		for(i = 0; i < pLine->m_pSlopRight.GetSize(); i++)
		{
			CSlop *pSlope	= pLine->m_pSlopRight.GetAt(i);
			pSlope->m_dValuePyungubae	= 0;
		}

		// �� ������ ��� ������ ���
		// ������ ��� ���̿� �� �������� ��� ���̿��� ���̸� �����ϴ� EL�� �����Ѵ�.
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
				
				// ������ level�� �׸��� ��� ���ܰ�翡 ���� ���̵� �� ��� �Ѵ�.
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
		// �ϴܿ��� ���� ���� EL�� ������Ű�� ��ü ��� ���̸� ���������.
		if(m_pStd->m_pBri->m_bSlopeLowerSlab)
		{
			// ������ ��ü ���̴� ����. ������ EL�� ���� ��� �����ϰ��̹Ƿ�
			// ���� �������� ���̸��� ��������.
			// ������ �����̶�� �׳� �ֵ� �ȴ�..
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
			// ���� �ϴ� EL�� �״�� �ΰ� ����� dDiffH/2 ��ŭ �ø��ų� ���� �ֱ� ���ؼ� ������ �̵����� ���ڴ�...
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

	// ��Ǫ�� Ÿ�� ����
	if(nPos == FORM_POS_NORMAL)			nFormType	= pBri->m_nFormType_ConcNormal;					// �Ϲ���ũ��Ʈ
	if(nPos == FORM_POS_FOOTING)		nFormType	= pBri->m_nFormType_Footing;					// ����
	if(nPos == FORM_POS_WALL_FRONT)		nFormType	= pBri->m_nFormType_WallFront;					// ��ü����
	if(nPos == FORM_POS_WALL_BACK)		nFormType	= pBri->m_nFormType_WallBack;					// ��ü���
	if(nPos == FORM_POS_WINGSL_FRONT)	nFormType	= pBri->m_nFormType_WingFront[iSTT][iLEFT];		// �������������� ����
	if(nPos == FORM_POS_WINGSL_BACK)	nFormType	= pBri->m_nFormType_WingBack[iSTT][iLEFT];		// �������������� ���
	if(nPos == FORM_POS_WINGSR_FRONT)	nFormType	= pBri->m_nFormType_WingFront[iSTT][iRIGHT];	// �������������� ����
	if(nPos == FORM_POS_WINGSR_BACK)	nFormType	= pBri->m_nFormType_WingBack[iSTT][iRIGHT];		// �������������� ���
	if(nPos == FORM_POS_WINGEL_FRONT)	nFormType	= pBri->m_nFormType_WingFront[iEND][iLEFT];		// �������������� ����
	if(nPos == FORM_POS_WINGEL_BACK)	nFormType	= pBri->m_nFormType_WingBack[iEND][iLEFT];		// �������������� ���
	if(nPos == FORM_POS_WINGER_FRONT)	nFormType	= pBri->m_nFormType_WingFront[iEND][iRIGHT];	// �������������� ����
	if(nPos == FORM_POS_WINGER_BACK)	nFormType	= pBri->m_nFormType_WingBack[iEND][iRIGHT];		// �������������� ���
	if(nPos == FORM_POS_APS)			nFormType	= pBri->m_nFormType_Aps;						// ���ӽ�����
	if(nPos == FORM_POS_MASS)			nFormType	= pBri->m_nFormType_MassConc;					// mass
	if(nPos == FORM_POS_LEAN)			nFormType	= pBri->m_nFormType_LeanConc;					// lean

	
	// Ÿ�Կ� ���� ���� �������� ����ϴ� nIdxMidClass�� ����
	if(pStd->m_nTypeBMApply == TYPE_BM_NORMAL)
	{
		/*
			CString CARcBridgeDataStd::m_sBM_FormTypeNormal[MAX_BM_FORM_TYPE_NORMAL] = 
			{			
				"����6ȸ",
				"����4ȸ",
				"����3ȸ",
				"���̰�Ǫ��",
				"�����Ǫ��",
				"������Ǫ��",
			};
		*/
		switch(nFormType)
		{
			case 0: nIdxMidClass	= BM_MID_FORM_6TIMES; break;	// ����6ȸ
			case 1: nIdxMidClass	= BM_MID_FORM_4TIMES; break;	// ����4ȸ
			case 2: nIdxMidClass	= BM_MID_FORM_3TIMES; break;	// ����3ȸ
			case 3: nIdxMidClass	= BM_MID_FORM_PATTERN; break;	// ���̰�Ǫ��
			case 4: nIdxMidClass	= BM_MID_FORM_GANGJE; break;	// �����Ǫ��
			case 5: nIdxMidClass	= BM_MID_FORM_CIRCLE; break;	// �����Ǫ��
			case 6: nIdxMidClass	= BM_MID_FORM_EURO; break;		// ������
			default: break;
		}
	}
	else
	{
		/*
		CString CARcBridgeDataStd::m_sBM_FormType[MAX_BM_FORM_TYPE] = 
			{			
				"��ģ����",
				"���븶��",
				"�Ų��Ѹ���",
				"����� Ư¡�ִ� ���� (���̰�Ǫ��)",
				"����� Ư¡�ִ� ���� (�����Ǫ��)",
				"����� Ư¡�ִ� ���� (���ð�Ǫ��)",
				"����� Ư¡�ִ� ���� (DECK PLATE)",
				"����� Ư¡�ִ� ���� (1ȸ ���)"
			};
		*/
		switch(nFormType)
		{
			case 0: nIdxMidClass	= BM_MID_FORM_6TIMES; break;	// ��ģ����
			case 1: nIdxMidClass	= BM_MID_FORM_4TIMES; break;	// ���븶��
			case 2: nIdxMidClass	= BM_MID_FORM_3TIMES; break;	// �Ų��Ѹ���
			case 3: nIdxMidClass	= BM_MID_FORM_PATTERN_S; break;	// ����� Ư¡�ִ� ���� (���̰�Ǫ��)
			case 4: nIdxMidClass	= BM_MID_FORM_GANGJE_S; break;	// ����� Ư¡�ִ� ���� (�����Ǫ��)
			case 5: nIdxMidClass	= BM_MID_FORM_COTTING_S; break;	// ����� Ư¡�ִ� ���� (���ð�Ǫ��)
			case 6: nIdxMidClass	= BM_MID_FORM_DECK_S; break;	// ����� Ư¡�ִ� ���� (DECK PLATE)
			case 7: nIdxMidClass	= BM_MID_FORM_ONE_S; break;		// ����� Ư¡�ִ� ���� (1ȸ ���)
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

	// ������ü ö�� ���� ����ǥ
	m_rebarTableAll.InitRebar();
	m_rebarTableAll.SetBasicConcInfo(pBasic);
	m_rebarTableAll.SetTypeUnit(nTypeUnit);
	m_rebarTableAll.SetTypeRoundRebarLength(pBasic->m_nRoundRebarLength);


	// ��ü
	m_rebarTableBridgeSum.InitRebar();
	m_rebarTableBridgeSum.SetBasicConcInfo(pBasic);
	m_rebarTableBridgeSum.SetTypeUnit(nTypeUnit);
	m_rebarTableBridgeSum.SetTypeRoundRebarLength(pBasic->m_nRoundRebarLength);


	AhTPADelete(&pArrRebar,(CReBar*)0);
	pBri->MakeRebarList(&pArrRebar, 0, TRUE, TRUE, 0);		// ��ü
	long i = 0;
	for(i=0; i<=pBri->m_nQtyJigan; i++)
	{
		pBri->MakeRebarList(&pArrRebar, 1, TRUE, TRUE, i);	// �߰�����(����)
		pBri->MakeRebarList(&pArrRebar, 6, TRUE, TRUE, i);	// ������� �ƴ�(�Ŵ�ö��)		
	}
	pBri->MakeRebarList(&pArrRebar, 2, TRUE, TRUE, i);		// ����

	// ���� ��ü�� ������ ö���� ���ԵǾ� �־����� �ɼ����� ������ �� �ִ�.
	// 2009.03.01
	if(!m_pStd->m_bSeparateExcludeRebarMonument)
		pBri->MakeRebarList(&pArrRebar, 4, TRUE, TRUE, i);		// ������

	// copy�صд�. 
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
	m_rebarTableBridge.AddRebar(&pArrRebar, hggettext("��ü"), _T(""));
	m_rebarTableBridge.FinishAddRebar();

	if(m_pStd->m_bRefCellBMSum)	sSheetName.Format(hggettext("%s ��ü ö�ټ��� ����ǥ"), pBri->m_strBridgeName);
	else											sSheetName = _T("");
	m_rebarTableBridgeSum.AddRebar(&pArrRebar, hggettext("��ü"), sSheetName);
	
	
	//����� �� �Ŵ� ö�� �и� ����
	for (long nJ =0; nJ < pBri->GetCountJijum(); nJ++)
	{
		AhTPADelete(&pArrRebar,(CReBar*)0);
		pBri->MakeRebarList(&pArrRebar, 5, TRUE, TRUE, nJ);		// �߰�����(����)
		pBri->MakeRebarList(&pArrRebarAll, 5, TRUE, TRUE, nJ);	
		pBri->MakeRebarList(&pArrRebar, 7, TRUE, TRUE, nJ);		// �������(�Ŵ�ö��)
		pBri->MakeRebarList(&pArrRebarAll, 7, TRUE, TRUE, nJ);	

		sTitleName.Format(_T("�����%d"), nJ);
		m_rebarTableBridgeSum.AddRebar(&pArrRebar, sTitleName, sSheetName);
	}

	m_rebarTableBridgeSum.FinishAddRebar();
	m_rebarTableAll.AddRebar(&pArrRebarAll, hggettext("��ü"), sSheetName);


	// ��ȣ��
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
			pBri->MakeRebarList(&pArrRebar, 8, TRUE, TRUE, i);		// ��ȣ��
			pBri->MakeRebarList(&pArrRebarAll, 8, TRUE, TRUE, i);		// ��ȣ��

			m_rebarTableGuardWall[i].InitRebar();
			m_rebarTableGuardWall[i].SetBasicConcInfo(pBasic);
			m_rebarTableGuardWall[i].SetTypeRoundRebarLength(pBasic->m_nRoundRebarLength);
			m_rebarTableGuardWall[i].SetTypeUnit(nTypeUnit);
			sTitleName.Format(hggettext("��ȣ��%d"), i + 1);
			m_rebarTableGuardWall[i].AddRebar(&pArrRebar, sTitleName, _T(""));
			m_rebarTableGuardWall[i].FinishAddRebar();

			sSheetName.Format(hggettext("%s ��ȣ��%d ö�ټ��� ����ǥ"), pBri->m_strBridgeName, i + 1);
			m_rebarTableGuardWallSum.AddRebar(&pArrRebar, sTitleName, sSheetName);
		}
	}

	if(m_pStd->m_bSeparateExcludeRebarMonument)
	{
		AhTPADelete(&pArrRebar,(CReBar*)0);
		pBri->MakeRebarList(&pArrRebar, 4, TRUE, TRUE, 0);		// ������
		pBri->MakeRebarList(&pArrRebarAll, 4, TRUE, TRUE, 0);		// ������

		m_rebarTableGuardWall[i].InitRebar();
		m_rebarTableGuardWall[i].SetBasicConcInfo(pBasic);
		m_rebarTableGuardWall[i].SetTypeRoundRebarLength(pBasic->m_nRoundRebarLength);
		m_rebarTableGuardWall[i].SetTypeUnit(nTypeUnit);
		m_rebarTableGuardWall[i].AddRebar(&pArrRebar, hggettext("������"), _T(""));
		m_rebarTableGuardWall[i].FinishAddRebar();

		sSheetName.Format(hggettext("%s ������ ö�ټ��� ����ǥ"), pBri->m_strBridgeName);
		m_rebarTableGuardWallSum.AddRebar(&pArrRebar, hggettext("������"), sSheetName);
	}

	m_rebarTableGuardWallSum.FinishAddRebar();

	if(m_pStd->m_bRefCellBMSum)	sSheetName.Format(hggettext("%s ��ȣ�� ö�ټ��� ����ǥ"), pBri->m_strBridgeName);
	else											sSheetName = _T("");
	m_rebarTableAll.AddRebar(&pArrRebarAll, hggettext("��ȣ��"), sSheetName);

	// ������
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
		sStt	= bStt ? "����" : "����";
		for(left = 0; left < 2; left++)
		{
			bLeft	= left == iLEFT;
			sLeft	= bLeft ? "����" : "����";

			AhTPADelete(&pArrRebar,(CReBar*)0);
			if(!pBri->IsWingWall(bStt, bLeft)) continue;

			pBri->MakeRebarList(&pArrRebar, 3, bStt, bLeft, 0);
			pBri->MakeRebarList(&pArrRebarAll, 3, bStt, bLeft, 0);

			m_rebarTableWing[stt][left].InitRebar();
			m_rebarTableWing[stt][left].SetBasicConcInfo(pBasic);
			m_rebarTableWing[stt][left].SetTypeRoundRebarLength(pBasic->m_nRoundRebarLength);
			m_rebarTableWing[stt][left].SetTypeUnit(nTypeUnit);
			sTitleName.Format(_T("������(%s%s)"), sStt, sLeft);
			m_rebarTableWing[stt][left].AddRebar(&pArrRebar, sTitleName, _T(""));
			m_rebarTableWing[stt][left].FinishAddRebar();

			sSheetName.Format(_T("%s %s ö�ټ��� ����ǥ"), pBri->m_strBridgeName, sTitleName);
			m_rebarTableWingSum.AddRebar(&pArrRebar, sTitleName, sSheetName);
		}
	}

	m_rebarTableWingSum.FinishAddRebar();

	if(m_pStd->m_bRefCellBMSum)	sSheetName.Format(_T("%s ������ ö�ټ��� ����ǥ"), pBri->m_strBridgeName);
	else											sSheetName = _T("");
	m_rebarTableAll.AddRebar(&pArrRebarAll, hggettext("������"), sSheetName);
	
	

	// ���ӽ�����
	AhTPADelete(&pArrRebarAll, (CReBar*)0);
	m_rebarTableApsSum.InitRebar();
	m_rebarTableApsSum.SetBasicConcInfo(pBasic);
	m_rebarTableApsSum.SetTypeRoundRebarLength(pBasic->m_nRoundRebarLength);
	m_rebarTableApsSum.SetTypeUnit(nTypeUnit);

	for(stt = 0; stt < 2; stt++)
	{
		bStt	= stt == iSTT;
		sStt	= bStt ? "����" : "����";
		for(left = 0; left < 2; left++)
		{
			bLeft	= left == iLEFT;
			sLeft	= bLeft ? "����" : "����";

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
			sTitleName.Format(_T("���ӽ�����(%s%s)"), sStt, sLeft);
			m_rebarTableAps[stt][left].AddRebar(&pArrRebar, sTitleName, _T(""));
			m_rebarTableAps[stt][left].FinishAddRebar();

			sSheetName.Format(hggettext("%s %s ö�ټ��� ����ǥ "), pBri->m_strBridgeName, sTitleName);
			m_rebarTableApsSum.AddRebar(&pArrRebar, sTitleName, sSheetName);
		}
	}
	m_rebarTableApsSum.FinishAddRebar();

	if(m_pStd->m_bRefCellBMSum)	sSheetName.Format(hggettext("%s ���ӽ����� ö�ټ��� ����ǥ"), pBri->m_strBridgeName);
	else											sSheetName = _T("");
	m_rebarTableAll.AddRebar(&pArrRebarAll, hggettext("���ӽ�����"), sSheetName);

	// ���潽����
	AhTPADelete(&pArrRebarAll, (CReBar*)0);
	m_rebarTableCnsSum.InitRebar();
	m_rebarTableCnsSum.SetBasicConcInfo(pBasic);
	m_rebarTableCnsSum.SetTypeRoundRebarLength(pBasic->m_nRoundRebarLength);
	m_rebarTableCnsSum.SetTypeUnit(nTypeUnit);
	for(stt = 0; stt < 2; stt++)
	{
		bStt	= stt == iSTT;
		sStt	= bStt ? "����" : "����";
		for(left = 0; left < 2; left++)
		{
			bLeft	= left == iLEFT;
			sLeft	= bLeft ? "����" : "����";

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
				sTitleName.Format(_T("���潽����(%s%s%d)"), sStt, sLeft, cns+1);
				m_rebarTableCns[stt][left][cns].AddRebar(&pArrRebar, sTitleName, _T(""));
				m_rebarTableCns[stt][left][cns].FinishAddRebar();

				sSheetName.Format(hggettext("%s %s ö�ټ��� ����ǥ"), pBri->m_strBridgeName, sTitleName);
				m_rebarTableCnsSum.AddRebar(&pArrRebar, sTitleName, sSheetName);
			}
		}
	}
	m_rebarTableCnsSum.FinishAddRebar();

	if(m_pStd->m_bRefCellBMSum)	sSheetName.Format(hggettext("%s ���潽���� ö�ټ��� ����ǥ"), pBri->m_strBridgeName);
	else											sSheetName = _T("");
	m_rebarTableAll.AddRebar(&pArrRebarAll, hggettext("���潽����"), sSheetName);

	//����Ÿ�� ���� //�߰�
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
			pBri->MakeRebarList(&pArrRebar, 10, TRUE, TRUE, ix); //����Ÿ������
			pBri->MakeRebarList(&pArrRebarAll, 10, TRUE, TRUE, ix); //����Ÿ������

			m_rebarTableHuynta[ix].InitRebar();
			m_rebarTableHuynta[ix].SetBasicConcInfo(pBasic);
			m_rebarTableHuynta[ix].SetTypeRoundRebarLength(pBasic->m_nRoundRebarLength);
			m_rebarTableHuynta[ix].SetTypeUnit(nTypeUnit);
			//
			if(ix ==0 || ix == pBri->GetCountFooting()-1)
			{
				sTitleName = ix == 0 ? _T("����") : _T("����");
			}
			else
			{
				sTitleName.Format(hggettext("�߰�������%d"), ix);
			}
			
			//
			m_rebarTableHuynta[ix].AddRebar(&pArrRebar, sTitleName, _T(""));
			m_rebarTableHuynta[ix].FinishAddRebar();

			sSheetName.Format(hggettext("%s ����Ÿ������%d ö�ټ��� ����ǥ"), pBri->m_strBridgeName, ix + 1);
			m_rebarTableHuyntaSum.AddRebar(&pArrRebar, sTitleName, sSheetName);
		}
	}
	m_rebarTableHuyntaSum.FinishAddRebar();

	if(m_pStd->m_bRefCellBMSum)	sSheetName.Format(hggettext("%s ����Ÿ������ ö�ټ��� ����ǥ"), pBri->m_strBridgeName);
	else											sSheetName = _T("");
	m_rebarTableAll.AddRebar(&pArrRebarAll, hggettext("����Ÿ������"), sSheetName);
	m_rebarTableAll.FinishAddRebar();


	AhTPADelete(&pArrRebar, (CReBar*)0);
	AhTPADelete(&pArrRebarAll, (CReBar*)0);
}

// ö�� ����
void COutBM::MakeBMRebar()
{
	CRcBridgeRebar *pBri	= GetRcBridgeRebar(TRUE);
	if(!pBri) return;

	long stt(0), left(0), i(0), cns(0);
	long nQtyHDan = pBri->GetQtyHDan();
	BOOL bLeft(TRUE), bStt(TRUE);
	CString sLeft(""), sStt(""), str("");

	CTypedPtrArray <CObArray, CRebar*> pArrRebar;

	// ��ü
	AhTPADelete(&pArrRebar,(CReBar*)0);
	pBri->MakeRebarList(&pArrRebar, 0, TRUE, TRUE, 0);		// ��ü
	for(i=0; i<=pBri->m_nQtyJigan; i++)
	{
		pBri->MakeRebarList(&pArrRebar, 1, TRUE, TRUE, i);	// �߰�����(����)
		pBri->MakeRebarList(&pArrRebar, 5, TRUE, TRUE, i);	// �߰�����(����)
		pBri->MakeRebarList(&pArrRebar, 6, TRUE, TRUE, i);	// ������� �ƴ�(�Ŵ�ö��)
		pBri->MakeRebarList(&pArrRebar, 7, TRUE, TRUE, i);	// �������(�Ŵ�ö��)
	}
	pBri->MakeRebarList(&pArrRebar, 2, TRUE, TRUE, i);		// ����

	// ���� ��ü�� ������ ö���� ���ԵǾ� �־����� �ɼ����� ������ �� �ִ�.
	// 2009.03.01
	if(!m_pStd->m_bSeparateExcludeRebarMonument)
		pBri->MakeRebarList(&pArrRebar, 4, TRUE, TRUE, i);		// ������

	SetRebarBMData(&m_pArrRebarBMBridgeStd[0], &m_pArrRebarBMBridgeStd[1], &pArrRebar, "��ü");
	
	// ��ȣ��
	for(i=0; i<nQtyHDan; i++)
	{
		CGuardWallRC* pGW = pBri->GetGuardWallByHDan(i);
		if(!pGW) continue;
		if(pGW->IsTypeHDanRoadTotalGuardWall() && !pGW->IsTypeHDanNonRebarConcrete())	
		{
			AhTPADelete(&pArrRebar,(CReBar*)0);
			pBri->MakeRebarList(&pArrRebar, 8, TRUE, TRUE, i);		// ��ȣ��
			SetRebarBMData(&m_pArrRebarBMGuardWallStd[i][0], &m_pArrRebarBMGuardWallStd[i][1], &pArrRebar, "��ȣ��");
		}
	}

	// ������
	if(m_pStd->m_bSeparateExcludeRebarMonument)
	{
		AhTPADelete(&pArrRebar,(CReBar*)0);
		pBri->MakeRebarList(&pArrRebar, 4, TRUE, TRUE, 0);		// ������
		SetRebarBMData(&m_pArrRebarBMMonumentStd[0], &m_pArrRebarBMMonumentStd[1], &pArrRebar, "������");
	}

	// ������
	for(stt = 0; stt < 2; stt++)
	{
		bStt	= stt == iSTT;
		sStt	= bStt ? "����" : "����";
		for(left = 0; left < 2; left++)
		{
			bLeft	= left == iLEFT;
			sLeft	= bLeft ? "����" : "����";
			str.Format("������(%s%s)", sStt, sLeft);

			AhTPADelete(&pArrRebar,(CReBar*)0);
			if(!pBri->IsWingWall(bStt, bLeft)) continue;

			pBri->MakeRebarList(&pArrRebar, 3, bStt, bLeft, 0);

			SetRebarBMData(&m_pArrRebarBMWingStd[stt][left][0], &m_pArrRebarBMWingStd[stt][left][1], &pArrRebar, str);
		}
	}

	// ���ӽ�����
	for(stt = 0; stt < 2; stt++)
	{
		bStt	= stt == iSTT;
		sStt	= bStt ? "����" : "����";
		for(left = 0; left < 2; left++)
		{
			bLeft	= left == iLEFT;
			sLeft	= bLeft ? "����" : "����";
			str.Format("���ӽ�����(%s%s)", sStt, sLeft);

			CApsApp *pAps	= bStt ? &pBri->m_apsStt[left] : &pBri->m_apsEnd[left];
			if(!pAps->m_bIs) continue;

			SetRebarBMData(&m_pArrRebarBMApsStd[stt][left][0], &m_pArrRebarBMApsStd[stt][left][1], &pAps->m_pArrRebar, str);
		}
	}

	// ���潽����
	for(stt = 0; stt < 2; stt++)
	{
		bStt	= stt == iSTT;
		sStt	= bStt ? "����" : "����";
		for(left = 0; left < 2; left++)
		{
			bLeft	= left == iLEFT;
			sLeft	= bLeft ? "����" : "����";
			
			for(cns = 0; cns < 2; cns++)
			{
				str.Format("���潽����(%s%s%d)", sStt, sLeft, cns+1);
				CApsApp *pCns	= bStt ? &pBri->m_cnsStt[left][cns] : &pBri->m_cnsEnd[left][cns];
				if(!pCns->m_bIs) continue;

				SetRebarBMData(&m_pArrRebarBMCnsStd[stt][left][cns][0], &m_pArrRebarBMCnsStd[stt][left][cns][1], &pCns->m_pArrRebar, str);
			}
		}
	}

	//����Ÿ�� ����
	for (long ix =0; ix < pBri->GetCountFooting(); ix++)
	{
		CFootingApp *pFooting = pBri->GetFooting(ix);
		if(pFooting == NULL) continue;

		if(pFooting->m_exFooting.m_nType == EXFOOTING_TYPE_PILE)
		{
			str.Format("����Ÿ������(%d)", ix+1);
			
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

// �ƽ���Ʈ ��� ����
// �׻� ���� �� : ����� ����, ����, ��ü������
// �ɼ� : ������ ����, ������ ����
void COutBM::MakeBMAsphaltProof()
{
	CRcBridgeRebar *pBri	= GetRcBridgeRebar(FALSE);
	if(!pBri) return;

	// ���߿� �ɼ����� �� ����.
	BOOL bIncludeSlabSide	= TRUE;

	CHgBaseBMStd baseBM;
	CTwinVector tvW, tvH;
	CTwinVectorArray tvArrPlane;

	// �⺻ Ÿ�� ����
	CBMData bmBase;
	bmBase.m_nIdxBigClass		= BM_BIG_ASPHALTPROOF;
	bmBase.m_sBigClass			= GetStringBigClass(bmBase.m_nIdxBigClass);
	bmBase.m_nLevelTotal		= GetLevelTotal(bmBase.m_nIdxBigClass); 
	bmBase.m_nIdxMidClass		= BM_MID_ASPHALTPROOF;
	bmBase.m_sMidClass			= GetStringMidClass(bmBase.m_nIdxMidClass);
	bmBase.m_nIdxSmallClass		= BM_SMALL_ASPHALTPROOF;
	bmBase.m_sSmallClass		= GetStringSmallClass(bmBase.m_nIdxSmallClass);
	bmBase.m_nTypeResult		= RESULT_AREA;
	bmBase.m_sUnit				= "��";
	bmBase.m_nShape				= SHAPE_TV_RECTANGLE;
	bmBase.m_dwSide				= SIDE_FRONT;
	bmBase.m_bIsPrintByRefForm	= TRUE;
	bmBase.m_nLevelMidClass		= 2;

	long stt = 0; for(stt = 0; stt < 2; stt++)
	{
		BOOL bStt		= stt == iSTT;
		CString sStt	= bStt ? "����" : "����";

		pBri->GetTvWingWallPlane(bStt, TRUE, FALSE, TRUE);

		// 1. ��ü ����
		if(pBri->IsOutWall(bStt))
		{
			CString sWall	= _T("");
			sWall.Format("%s��ü%s��", sStt, bStt ? "����" : "����");
			pBri->m_tvArrPlaneWingWall.GetTvByInfo("�����곡�ܳ���", tvW);
			pBri->m_tvArrVertSection.GetTvByInfo(sWall, tvH);

			CBMData *pBM			= new CBMData(&bmBase);
			pBM->m_sNoteTail		= bStt ? _T("���������곡�ܳ���") : _T("���������곡�ܳ���");
			pBM->m_dV[WidthLower]	= Round(toM(~(tvW.m_v2 - tvW.m_v1)),3);
			pBM->m_dV[Height]		= Round(toM(~(tvH.m_v2 - tvH.m_v1)),3);
			pBM->CalculateResult();
			m_pArrBMBridgeStd.Add(pBM);
		}

		// 2. ����� ����� ����
		if(pBri->IsBracket(bStt))
		{
			CString sBraket	= _T("");

			// ����� ����
			sBraket.Format("%s���������", sStt);
			pBri->m_tvArrPlaneWingWall.GetTvByInfo("�����", tvW);
			pBri->m_tvArrVertSection.GetTvByInfo(sBraket, tvH);

			CBMData *pBM			= new CBMData(&bmBase);
			pBM->m_sNoteTail		= bStt ? _T("�������������") : _T("�������������");
			pBM->m_dV[WidthLower]	= Round(toM(~(tvW.m_v2 - tvW.m_v1)),3);
			pBM->m_dV[Height]		= Round(toM(~(tvH.m_v2 - tvH.m_v1)),3);
			pBM->CalculateResult();
			m_pArrBMBridgeStd.Add(pBM);

			// ����� ����
			sBraket.Format("%s�������ġ", sStt);
			pBri->m_tvArrVertSection.GetTvByInfo(sBraket, tvH);

			CBMData *pBM1			= new CBMData(&bmBase);
			pBM1->m_sNoteTail		= bStt ? _T("��������ϰ���") : _T("��������ϰ���");
			pBM1->m_dV[WidthLower]	= Round(toM(~(tvW.m_v2 - tvW.m_v1)),3);
			pBM1->m_dV[Height]		= Round(toM(~(tvH.m_v2 - tvH.m_v1)),3);	// tv.m_dAngle�� ������ ���� ���� GetLength�� �ȵǴ°���??
			pBM1->CalculateResult();
			m_pArrBMBridgeStd.Add(pBM1);
		}

		// 3. ������ ���� 
		if(bIncludeSlabSide)
		{
			CTwinVector tvSlab;
			CString sSlab	= _T("");
			if(pBri->IsOutWall(bStt)) sSlab.Format("%s����ϻ������", sStt);
			else sSlab.Format("%s����������", sStt);
			pBri->m_tvArrVertSection.GetTvByInfo(sSlab, tvH);
			
			pBri->m_tvArrPlaneWingWall.GetTvByInfo("�����곡�ܳ���", tvW);

			CBMData *pBM			= new CBMData(&bmBase);
			pBM->m_sNoteTail		= bStt ? _T("��������������") : _T("��������������");
			pBM->m_dV[WidthLower]	= Round(toM(~(tvW.m_v2 - tvW.m_v1)),3);
			pBM->m_dV[Height]		= Round(toM(~(tvH.m_v2 - tvH.m_v1)),3);
			pBM->CalculateResult();
			m_pArrBMBridgeStd.Add(pBM);
		}
	}

	// ������
	pBri = GetRcBridgeRebar(TRUE);
	for(stt = 0; stt < 2; stt++)
	{
		BOOL bStt = stt == iSTT;
		CString sStt = bStt ? "����" : "����";

		for(long left = 0; left < 2; left++)
		{
			BOOL bLeft = left == iLEFT;
			CString sLeft = bLeft ? "����" : "����";

			if(!pBri->IsWingWall(bStt, bLeft)) continue;
			CTypedPtrArray <CObArray, CBMData*> *pArrBMStd	= &m_pArrBMWingStd[stt][left];

			// ��������Ǫ��(���) ������ ���� /////////////////////
			CTypedPtrArray <CObArray, CBMData*> pArrForm;
			long nSizeBM	= pArrBMStd->GetSize();
			long i = 0; for(i = 0; i < nSizeBM; i++)
			{
				CBMData *pBM = pArrBMStd->GetAt(i);
				if(pBM->m_sInfo == "��������Ǫ��(���)" || pBM->m_sInfo == "��������Ǫ��(�����ġ)")
				{
					CBMData *pForm = new CBMData;
					*pForm = *pBM;
					// ��Ǫ¤ �������� ������ ������ ��ȣ�� ������� ���ĵ��� �ʴ´�.
					// �׷��� ���⼭�� ��Ǫ�� ������ ��� ���� ������ ���ش�.
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

			// ������ ��Ǫ�� �������� �߰���Ŵ //
			CBMData *pBM = new CBMData(&bmBase);
			pBM->m_sNoteTail = bStt ? _T("������������Ǫ��") : _T("������������Ǫ��");
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
			pBM->m_sForm = (str + "�� ��Ǫ��(���) ������ ����");
			
			pBM->m_bIsPrintByRefForm	= FALSE;
			pBM->CalculateResult();
			pArrBMStd->Add(pBM);
			////////////////////////////////////////////

			AhTPADelete(&pArrForm,(CBMData*)0);
		}
	}
}

// ��Ǫ�� ���̺��� �ڸ��� ������ �Ǵ� EL
// ������ ���� ������� ����
// �� : -1�̸� ���� ���� EL�� �������� ��
double COutBM::GetElBaseForForm(long nJ)
{
	CRcBridgeRebar *pBri	= GetRcBridgeRebar(TRUE);
	if(!pBri) return 0;
	CRcBridgeRebar *pBriBM	= GetRcBridgeRebar(FALSE);
	if(!pBri) return 0;

	CDRect rect	= pBri->m_tvArrVertSection.GetRect();
	double dEL	= rect.top - pBri->m_dTS;

	//Box���϶� Footing������ �����Ƿ� ����el���� ������β���ŭ �÷��� ���
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

	// �ش� ������ ���ʰ� ���ٸ� ���� ������ ���
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

/// ��ġ�� ���ٸ��� ��� �Ʒ��ʿ� ���� ǥ��
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

	// �� ����
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

			if(pArea->m_sInfo.Find("��ġR") != -1 || pArea->m_sInfo.Find("��ġ") != -1)
			{
				if(bFirst)
					xyText.x = rect.left + rect.Width()/3;
				else
					xyText.x = rect.left + rect.Width()/3 + rect.Width()/5;
				sText.Format("%s %.3f ��", baseBM.GetCircleText(i+(nNum-nSize)+2), toM(toM(pArea->m_dArcAreaL)));
				Dom.TextOut(xyText, sText);

				if(!bFirst)
					xyText.y -= Dom.GetTextHeight()*1.5;

				bFirst = !bFirst;
			}
		}
	}

	*pDomP << Dom;
}

// ��ġ���� ��ũ��Ʈ / ��Ǫ���� ���� ����,���� ���� ���
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

	
	// ���� ����
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

	// �� ����
	CDPoint xyText	= CDPoint(rect.left + rect.Width()/3, rect.top - Dom.GetTextHeight()*2);
//	CString sText	= _T("");

	xyText.x	= rect.left + rect.Width()/3;

	CString sText	= "��ġ����";
	Dom.TextOut(xyText, sText);

	sText.Format("Area : %.3f ��", dAreaRib);
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

	// ���� ���� ���� ���� ���
	/*
	xyText	= CDPoint(rect.left, rect.bottom + Dom.GetTextHeight()*2);
	BOOL bRail	= pBri->m_pARcBridgeDataStd->IsRailDesign();
	sText.Format("%s %s", baseBM.GetCircleText(nIdx+1), GetStringTypeHDanByNum(pBri->GetTypeHDan(nHDan, FALSE), bRail));
	Dom.TextOut(xyText, sText);
	*/


	// ���� ���� �Ʒ��ʿ� ���
	Dom.SetTextAlignHorz(TA_LEFT);
	CDPoint xyText	= CDPoint(rect.left, rect.top-Dom.GetTextHeight());

	sText.Format("���� ���� = %.3f m", dLenBridge);
	xyText.y -= Dom.GetTextHeight() * 1.5;
	Dom.TextOut(xyText, sText);

	sText.Format("��ȣ�� �߰� ���� = %.3f m", dLenAdd);
	xyText.y -= Dom.GetTextHeight() * 1.5;
	Dom.TextOut(xyText, sText);

	xyText.y -= Dom.GetTextHeight() * 0.25;
	Dom.SetCalcExtRect();
	rect	= Dom.GetExtRect();
	Dom.LineTo(xyText.x, xyText.y, xyText.x + Dom.GetTextWidth(sText), xyText.y);

	sText.Format("�� = %.3f m", dLenTot);
	xyText.y -= Dom.GetTextHeight() * 1.25;
	Dom.TextOut(xyText, sText);
	
	*pDomP << Dom;
}

// �������� ����
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

			//���� �翷���� ������ ����ġ���� 0.07 * 2
			//���� ������ ���ʸ� ������ ����ġ���� 0.07 * 1;
			for(long nHDan = 0; nHDan < pBri->GetQtyHDan(FALSE); nHDan++)
			{
				CGuardWallRC* pGW = pBri->GetGuardWallByHDan(nHDan, FALSE);
				if(!pGW) continue;
				if(pGW->IsTypeHDanOnlyNone()) //�����ΰ��
				{
					//���ʴ� �ִ°��
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
			pBM->m_sNoteHead			= "�������� ����(������ ���� + ����ġ��)";
			pBM->m_sForm				= sSum;
			pBM->SetResult(dResult);
			pBM->m_bIsPrintByRefForm	= TRUE;
			pBM->m_nLevelMidClass		= 2;
			pArrBM->Add(pBM);
		}
	}
}

// ������������ ����
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
			if(pGW->IsTypeHDanOnlyNone()) //����, ���� ����
				continue;
				
			//�������� ������ ���� .
			if(pGW->IsTypeHDanRoadGuardFence()) nCountHDan[HDAN_TYPE_ROAD_GUARDFENCE]++;				//��ȣ��
			else if(pGW->IsTypeHDanRoadFootwayBlock()) nCountHDan[HDAN_TYPE_ROAD_FOOTWAYBLOCK]++;		//���������
			else if(pGW->IsTypeHDanRoadCentralWallFull()) nCountHDan[HDAN_TYPE_ROAD_CENTERGUARD]++;		//�ߺд�
			else if(pGW->IsTypeHDanRoadCentralWallHalf()) nCountHDan[HDAN_TYPE_ROAD_CENTERGUARDHALF]++;	//�ߺд�1/2
			else if(pGW->IsTypeHDanRoadSoundProof()) nCountHDan[HDAN_TYPE_ROAD_SOUNDPROOF]++;			//����������
			else if(pGW->IsTypeHDanRoadRetainingWall()) nCountHDan[HDAN_TYPE_ROAD_RETAININGWALL]++;		//�븷�̺�
			else if(pGW->IsTypeHDanRoadHandRail()) nCountHDan[HDAN_TYPE_ROAD_HANDRAIL]++;				//��������
			else if(pGW->IsTypeHDanRoadDike()) nCountHDan[HDAN_TYPE_ROAD_DIKE]++;						//����ũ
		}

		// 10 : �ִ���������
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

// ���������� ��������ũ��Ʈ
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
			long nQty		= 2; // 2�� �������� �����̶�.
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
			pBM->m_sUnit				= "��";
			pBM->m_sNoteHead			= "������ ������ ��ũ��Ʈ ����";
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
	pBM->m_sUnit					= "��";
	pBM->m_nShape				= SHAPE_TRAPEZOID;
	pBM->m_dV[WidthLower]		= Round(toM(pBri->m_dMonuH2+pBri->m_dMonuH3),3);
	pBM->m_dV[WidthUpper]		= Round(toM(pBri->m_dMonuH2),3);
	pBM->m_dV[WidthDiffLeft]	= 0;
	pBM->m_dV[WidthDiffRight]	= pBM->m_dV[WidthLower] - pBM->m_dV[WidthUpper];
	pBM->m_dV[Height]			= Round(toM(pBri->m_dMonuD),3);
	pBM->m_dV[Length]			= Round(toM(pBri->m_dMonuL),3);
	pBM->m_dwSide				= SIDE_FRONT;
	pBM->m_sInfo				= "������";
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
	pBM->m_sUnit				= "��";
	pBM->m_nShape				= SHAPE_RECTANGLE;
	pBM->m_dV[WidthLower]		= Round(toM(pBri->m_dMonuW-pBri->m_dMonuD),3);
	pBM->m_dV[Height]			= Round(toM(pBri->m_dMonuH1),3);
	pBM->m_dV[Length]			= Round(toM(pBri->m_dMonuL),3);
	pBM->m_dwSide				= SIDE_FRONT;
	pBM->m_sInfo				= "������";
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

	// �յ�(��ٸ���)
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
	pBM->m_sUnit				= "��";
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

	// ��ٸ��� �κ� ���� ����
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
	pBM->m_sUnit				= "��";
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

	// ��ٸ��� �κ� ���� ���
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
	pBM->m_sUnit				= "��";
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
	
	// �յ�(�簢��) / ����
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
	pBM->m_sUnit				= "��";
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

	// ������ ���������� �߰��Ѵ�.
	RemakeByHeightBMList(pArrBMStd, dFirstMeter, dSeconeMeter, nSizeBM, pArrBMStd->GetSize()-1);
}

long COutBM::DrawFirstPageMonument(CHgBaseBMStd *pBaseBMStd)
{
	if(!pBaseBMStd) return 5;

	CRcBridgeRebar *pBriOrg = GetRcBridgeRebar(TRUE);		// ��ȣ���� ���� �������� �۾���.
	if(!pBriOrg) return 5;

	CARcBridgeDBStandard DBStd(pBriOrg, m_pStd->m_pARoadOptionStd, m_pStd->m_pARcBridgeDataStd);
	DBStd.m_bApplyDimSettingOfBaseDom	= TRUE;

	// ������ ��µǾ����� �ϴ� ������
	double dWidthOrg = 200;	// ���� ����ũ��

	// ���ܸ鵵�� �������� ��������!!! ////////////////////////////////////
	// �װɷ� ���ܸ�� ���õ� ������ �׷�����.
	CDomyun domTmp;
	domTmp.SetCWnd(m_pStd->m_pView);
	domTmp.SetScaleDim(1);
	domTmp.SetDimTxt(1);

	CDPoint vAng(0,1);
	DBStd.m_pDrawCrossSection->DrawMonumentFront(&domTmp);
	DBStd.m_pDrawCrossSection->DimMonumentFront(&domTmp);

	/////////////////////////////////////////////////////////////////////////
	
	// ���ܸ鵵
	CDomyun domCrossSection;
	domCrossSection.SetCWnd(m_pStd->m_pView);

	// ���� ����
	double dScale	= pBaseBMStd->GetScale(&domTmp, dWidthOrg);
	pBaseBMStd->SetDomyunValue(&domCrossSection, dWidthOrg, &domTmp);
			
	// ��ȣ��	
	DBStd.m_pDrawCrossSection->DrawMonumentFront(&domCrossSection);
	DBStd.m_pDrawCrossSection->DimMonumentFront(&domCrossSection, FALSE);

	// ������ ����
	domCrossSection.RedrawByScale(dScale, TRUE);
	domCrossSection.SetCalcExtRect();

	double dWidth = dWidthOrg;//domCrossSection.GetExtRect().Width();
	double dHeight = 300;//domCrossSection.GetExtRect().Height();
	CDoubleArray dArrTmp;
	long nRow = pBaseBMStd->InsertDomyun(5, TA_CENTER, &domCrossSection, dArrTmp, dWidth, dHeight);

	// ��鵵 �׸��� /////////////////////////////////////////////////////////
	CDomyun domPlane;
	domPlane.SetCWnd(m_pStd->m_pView);
	dWidthOrg = 200;

	// ���� ���� ////////////////////
	pBaseBMStd->SetDomyunValue(&domPlane, dWidthOrg, &domTmp);
	///////////////////////////////////

	DBStd.m_pDrawCrossSection->DrawMonumentPlane(&domPlane);
	DBStd.m_pDrawCrossSection->DimMonumentPlane(&domPlane, FALSE);


	// �׷ȴٸ� ���ϴ� ũ��� ������ ����
	domPlane.RedrawByScale(dScale, TRUE);
	domPlane.SetCalcExtRect();

	// ������ ����
	//double dRate	= domPlane.GetExtRect().Width() / dWidthOrg;
	//nRow	= pBaseBMStd->InsertDomyun(nRow+3, TA_CENTER, &domPlane, domPlane.GetExtRect().Width()/dRate, domPlane.GetExtRect().Height()/dRate);

	nRow	= pBaseBMStd->InsertDomyun(nRow+3, TA_CENTER, &domPlane, dArrTmp, dWidth, dHeight);

	DBStd.m_bApplyDimSettingOfBaseDom	= FALSE;


	return nRow;
}

// ������
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

// ��ġ
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
				str.Format("��������(%d��°����)", i+1);
				if(pBriOrg->m_tvArrVertSection.GetTvByInfo(str, tv))
					dLen[left]	+= tv.GetLength();
			}
		}
		else
		{
			dLen[left]	= 0;
			if(pBriOrg->m_tvArrVertSection.GetTvByInfo("��������(������ü����)", tv))
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

// ������ ��� El ������ ǥ��
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
		sText1	= "* ���� ����� ��ü ���� ���� : �����߽� ���� ����";
	}
	else if(pBri->m_nTypeHeightWall == 1 || pBri->m_nTypeHeightWall == 3)
	{
		sText1	= "* ���� ����� ��ü ���� ���� : ��� ���� ����((�ִ�+�ּ�)/2)";
	}
	else if(pBri->m_nTypeHeightWall == 2)
	{
		sText1	= "* ���� ����� ��ü ���� ���� : �ִ� ���� ����";
	}
	xyText.x	= rect.left + ((rect.Width() - Dom.GetTextWidth(sText1))*3/5);
	Dom.TextOut(xyText, sText1);

	if(bBoxType)
	{
		CRcBridgeRebar *pBriConvert	= GetRcBridgeRebar(FALSE);
		// ��ȯ��Ų Bridge���� ��ü ���̸� ��������. ����ٰŴ� ���� Bridge����.
		double dStaStt	= pBriConvert->GetStationByTarget(BRIDGE_OFF_STT_SLAB, 0, FALSE);
		double dELTop	= pBriConvert->GetXySlabUpperActByDist(dStaStt, 0, FALSE, FALSE, FALSE, FALSE).y;
		double dELBot	= pBriConvert->m_dELSB;
		dHeightWall = dELTop - dELBot;

		if(pBri->m_bSlopeLowerSlab == FALSE)
		{
			// ��� ��ü ���� ���� �ٰ�
			dStaStt	= pBri->GetStationByTarget(BRIDGE_OFF_STT_SLAB, 0, FALSE);
			dELTop	= pBri->GetXySlabUpperActByDist(dStaStt, 0, FALSE, FALSE, FALSE, FALSE).y;
			dELBot	= pBri->m_dELSB;
			double dStaEnd	= pBri->GetStationByTarget(BRIDGE_OFF_END_SLAB, 0, FALSE);
			double dELEnd	= pBri->GetXySlabUpperActByDist(dStaEnd, 0, FALSE, FALSE, FALSE, FALSE).y;

			double dHStt = dELTop - dELBot;
			double dHEnd = dELEnd - dELBot;
			double dHAvg = (dHStt + dHEnd) / 2.;

			sText2.Format("- ��� ��ü ���� : (%s + %s) / 2 = %s", COMMA(dHStt), COMMA(dHEnd), COMMA(dHAvg));
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
			// ��ȯ�� Bridge���� ���ؿ� ��ü ���� �̱� ������ dELDiff�� �̹� �ݿ����ִ� �׷��� �ٽ� �ѹ� ��������.
			dH += dELDiff;
		}

		if(pBri->m_nTypeHeightWall == 0)
		{
			sText2.Format("- ����%d ��ü ���� : %s", i+1, COMMA(dH));
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
				


				sText2.Format("- ����%d ��ü ���� : ((%s+%s)/2 - %s) - %s - %s = %s", i+1, COMMA(dELMax), COMMA(dELMin), COMMA(dELB), COMMA(dHMinusT), COMMA(dHFooting), COMMA((dELMax+dELMin)/2 - dELB - dHMinusT - dHFooting));
			}
			else
			{
				sText2.Format("- ����%d ��ü ���� : %s - (%s - (%s+%s)/2) = %s", i+1, COMMA(dH), COMMA(dEL), COMMA(dELMax), COMMA(dELMin), COMMA(dH - (dEL - (dELMax+dELMin)/2)));
			}
		}
		else if(pBri->m_nTypeHeightWall == 2)
		{
			sText2.Format("- ����%d ��ü ���� : %s - (%s - %s) = %s", i+1, COMMA(dH), COMMA(dEL), COMMA(dELMax), COMMA(dH - (dEL - dELMax)));
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

		if( sType == _T("����") ||  sType == _T("����") || sType == _T("������ ��輮")) 
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
				
				//{32073 GetVerLengh()�� double���� ���ҿ����� ���� �� ���� ������ �Ʒ��� ������ �����
				double dLenH1 =0, dLenH2 = 0;
				dLenH1 = tv.m_v1.y;
				dLenH2 = tv.m_v2.y;

				if(tv.m_sInfo.Find(_T("Hole")) != -1)
				{
					// ������ �ϸ��ϰ� ��ũ�� ����
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
			pBM->m_sUnit				= "��";
			pBM->m_nShape				= SHAPE_RECTANGLE;
			pBM->m_dV[SttMeter]			= Round(toM(0),3);
			pBM->m_dV[EndMeter]			= Round(toM(7),3);
			pBM->m_dV[WidthLower]		= Round(toM(dLength),3);
			pBM->m_dV[Height]			= Round(toM(dLenOutLine),3);
			pBM->m_dwSide				= SIDE_FRONT;
			pBM->CalculateResult();
			pArrBMStd->Add(pBM);
		}

		// ������ ���������� �߰��Ѵ�.
		RemakeByHeightBMList(pArrBMStd, dFirstMeter, dSeconeMeter, nSizeBM, pArrBMStd->GetSize()-1);
	}
}


//31724 ������ ��� �ٸ� ���� (������ ���ٸ�)
void COutBM::MakeBMScaffoldLegWing()
{
	CRcBridgeRebar *pBri	= GetRcBridgeRebar();
	if(!pBri) return;

	// ������ �¿��� ������
	CTwinVectorArray tvArrOut, tvArrCenOut, tvArrSideOut;
	CTwinVectorArray tvArrIn, tvArrCenIn, tvArrSideIn;

	CTwinVectorArray tvArrScaffoldSec;
	CString sNoteTail	= _T("");
	CString sStt		= _T("");
	CString sLeft		= _T("");

	long stt = 0; for(stt = 0; stt < 2; stt++)
	{
		BOOL bStt	= stt == iSTT ? TRUE : FALSE;
		sStt	= bStt ? "������" : "������";

		pBri->GetTvScaffoldSection(tvArrScaffoldSec, bStt);	// ���� ������
		for(long left = 0; left < 2; left++)
		{
			BOOL bLeft	= left == iLEFT ? TRUE : FALSE;
			if(!pBri->IsWingWall(bStt, bLeft)) continue;

			CTwinVectorArray tvArrWingWall;
			tvArrWingWall.RemoveAll();

			//�ܸ鵵 ������ ������ ���̸� ����
			pBri->GetTvWingWallSection(bStt, bLeft, tvArrWingWall);

			CDRect rectWingWall;
			rectWingWall = tvArrWingWall.GetRect();

			double dHeightWingWall	= fabs(tvArrScaffoldSec.GetRect().Height());
			double dHeightApply	= 2000;	
			double dHeightOffset	= 1500;

			long nDan = 0;	// ��� �ܼ�
			double dCurHeight = dHeightApply;
			while(dCurHeight <= dHeightWingWall)
			{
				nDan++;
				dCurHeight += dHeightOffset;
			}

			sLeft	= bLeft ? "����" : "����";

			CTypedPtrArray <CObArray, CBMData*> *pArrBMStd	= &m_pArrBMWingStd[stt][left];

			pBri->GetTvScaffoldWingPlane(tvArrOut, tvArrIn, tvArrCenOut, tvArrCenIn,  tvArrSideOut, tvArrSideIn, bStt, bLeft);
// 			double dCenLen = 0;
// 			double dCenIn = 0;
// 			double dResultCen= 0;
// 			double dResultIn =0;


			// ������ ����
			sNoteTail	= sStt + sLeft + "������ ����";
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
				pBM->m_sForm.Format(_T("%.3f x %d ��"),dCenLen, nDan ); //Height�� �ܼ� 
				//pBM->m_bIsPrintByRefForm	= TRUE;
				pBM->m_dV[WidthLower]	= dCenLen;
				pBM->m_dV[Height]		= (double) nDan;
				pBM->m_dwSide			= SIDE_FRONT;
				pBM->CalculateResult();
				pBM->SetResult(dResultCen);
				pArrBMStd->Add(pBM);	
			}

			// ������ ���� 
			sNoteTail	= sStt + sLeft + "������ ����";
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
				pBM2->m_sForm.Format(_T("%.3f x %d ��"),dCenIn, nDan ); //Height�� �ܼ� 
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
				pBM->m_sForm.Format(_T("%.3f x %d ��"),dCenSide, nDan ); //Height�� �ܼ� 
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

//31724 ��ü ���ٸ� ����
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

			long nDan = 0;	// ��� �ܼ�
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
			pBM->m_sForm.Format(_T("%.3f x %d ��"),dWidth, nDan ); //Height�� �ܼ� 
			pBM->m_dV[Height]		= dWidth;
			pBM->m_dV[WidthLower]	= Round(toM(dHeight), 3);
 			pBM->m_dwSide			= SIDE_FRONT;
 			pBM->CalculateResult();

			// (ARCBRIGE-1573) ���� Width x �ܼ��ε� ����� height x �ܼ���... �Ŀ� ���缭 ����.
			// dHeight = Round(toM(dHeight), 3);
			// dResult = dHeight * (double) nDan;
			double dResult = dWidth * (double) nDan;
 			pBM->SetResult(dResult);
// 			//}

 			m_pArrBMBridgeStd.Add(pBM);	
		}
	}
}

//�⼮���� (�Ϲ� ����) [�������]
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
		pTvArrFront->GetTvByInfo("Ȯ����ʻ��������", tvL);
		pTvArrFront->GetTvByInfo("Ȯ����ʻ�ܿ�����", tvR);
		dExtLeft		= tvL.GetHorLength(TRUE);
		dExtRight		= tvR.GetHorLength(TRUE);
	}

	tvR = pExFoot->m_tvArrFront.GetTvByInfo(_T("Ȯ����ʿ�����"));
	tvL = pExFoot->m_tvArrFront.GetTvByInfo(_T("Ȯ�����������"));
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
		if(tv.m_sInfo.Find(_T("�ϸ�")) != -1)
		{
			tvArrSec.Add(tv);
		}
	}

	//Plan
	pBri->GetTvRebarOut_Plane_UpperSlabStd(tvArrPlane, -1, TRUE);
	for(long ix = 0; ix < tvArrPlane.GetSize(); ix++)
	{
		tv	= tvArrPlane.GetAt(ix);
		if(tv.m_sInfo.Find("����", 0) != -1) tvArrLeft.Add(tv);
		else if(tv.m_sInfo.Find("����", 0) != -1) tvArrRight.Add(tv);
		else if(tv.m_sInfo.Find("����", 0) != -1) tvArrStt.Add(tv);
		else if(tv.m_sInfo.Find("����", 0) != -1) tvArrEnd.Add(tv);
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
	pBM->m_sUnit				= "��";
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
	// �������� ��¾��
	pBM->m_nLevelMidClass		= 2;
	if (pBM->m_sInfo.Find("������ũ��Ʈ��Ǫ��") < 0 && pBM->m_sInfo.Find("MASS��ũ��Ʈ��Ǫ��") < 0)
	{
		pBM->m_sInfo			= _T("���������");
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
