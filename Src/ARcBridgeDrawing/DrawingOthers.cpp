// DrawingOthers.cpp: implementation of the CDrawingOthers class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../../Src/ARcBridgeData/ARcBridgeData.h"
#include "../ARcBridgeDrawingOption/ARcBridgeDrawingOption.h"
#include "ARcBridgeDrawing.h"
#include "DrawingOthers.h"
#include "../../Src/ARcBridgeDBDraw/ARcBridgeDBDraw.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define MPE_REBAR_SLABPLANE				10		// ������ ��� ������
#define MPE_REBAR_SLABPLANE_TITLE		40		// ������ ��� ������ ����
#define MPE_REBAR_SLABFRONT				70		// ������ ���� ������
#define MPE_REBAR_SLABFRONT_TITLE		100		// ������ ���� ������ ����
#define MPE_REBAR_SLABSIDE				130		// ������ ���� ������
#define MPE_REBAR_SLABSIDE_TITLE		160		// ������ ���� ������ ����
#define MPE_REBAR_SLABDETAIL			190		// ������ ö�ٻ�
#define MPE_REBAR_SLABDETAIL_TITLE		220		// ������ ö�ٻ� ����
#define MPE_REBAR_SLABDETAILA			250		// ������ �� A
#define MPE_REBAR_SLABDETAILA_TITLE		280		// ������ �� A ����
#define MPE_REBAR_SLABTABLEM			310		// ������ ���ǥ
#define MPE_REBAR_SLABTABLEM_TITLE		340		// ������ ���ǥ ����
#define MPE_REBAR_SLABTABLE				370		// ������ ö�� ���ǥ
#define MPE_REBAR_SLABTABLE_TITLE		400		// ������ ö�� ���ǥ ����
#define MPE_REBAR_SLABNOTE				430		// ������ �⺻��Ʈ
#define MPE_REBAR_SLABNOTE_TITLE		460		// ������ �⺻ ��Ʈ ����
#define MPE_REBAR_HOLDDETAIL			480		// ����Ÿ������ ��
#define MPE_REBAR_HOLDDETAIL_TITLE		490		// ����Ÿ������ �� ����
#define MPE_REBAR_SPACERDETAIL			500		// ����Ÿ������ SPACER��
#define MPE_REBAR_SPACERDETAIL_TITLE	510		// ����Ÿ������ SPACER�� ����
#define MPE_REBAR_MONUFRONT				520		// ������ ���� ������
#define MPE_REBAR_MONUFRONT_TITLE		530		// ������ ���� ������ ����
#define MPE_REBAR_MONUPLANE				540		// ������ ��� ������ 
#define MPE_REBAR_MONUPLANE_TITLE		550		// ������ ��� ������ ����
#define MPE_REBAR_MONUDETAIL			560		// ������ ö�ٻ�
#define MPE_REBAR_MONUDETAIL_TITLE		570		// ������ ö�ٻ� ����
#define MPE_REBAR_MONUTABLE				580		// ������ ö�� ���ǥ
#define MPE_REBAR_MONUTABLE_TITLE		590		// ������ ö�� ���ǥ ����
#define MPE_SHOE						600		// ������ħ
#define MPE_SHOE_TABLE					610     // ������ħ ��ǥ ���̺�
#define MPE_SHOE_TABLE_TITLE			620     // ������ħ ��ǥ ���̺� ����
#define MPE_SHOE_LIST					630		// ������ħ List
#define MPE_SHOE_LIST_TITLE				640		// ������ħ List ����
#define MPE_SHOE_BUM					650		// ������ħ ����
#define MPE_SHOE_BUM_TITLE				660		// ������ħ ���� ����





CDrawingOthers::CDrawingOthers()
{
	m_nBridge = -1;
}

CDrawingOthers::~CDrawingOthers()
{

}

long CDrawingOthers::GetPageSu()
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;

	if(pStd->GetCountTotalPage()<=0)
		pStd->MakePageInfo();

	m_nPage	= pStd->GetCountCategoryPage(_T("��Ÿ ����"));

	return m_nPage;
}

CString CDrawingOthers::GetTitle()
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;
	CPageInfo* pInfo = pStd->GetPageInfoOrg(_T("��Ÿ ����"), pStd->m_nCurPage-m_nSttPage);
	if(pInfo==NULL) return _T("");

	CString strTitle = pStd->GetStringTitle(pStd->m_pARcBridgeDataStd->GetBridge(m_nBridge), pInfo->m_strTitle);

	return strTitle;
}

double CDrawingOthers::GetScale()
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;
	CARcBridgeDrawingOptionStd* pOpt = pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pOptData = pOpt->GetDrawingOption(m_nBridge);

	return pOptData->m_dScaleRebarOthers;
}

void CDrawingOthers::SetNumber(long nSubPage)
{
	m_nBridge = GetNumberBridge(nSubPage-m_nSttPage);
}

// ���� �׷��� �Ǵ� RcBridge����
CRcBridgeRebar* CDrawingOthers::GetRcBridgeRebar()
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;
	CARcBridgeDataStd* pDataStd	= pStd->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = NULL;
	
	if(m_nBridge>-1 && pDataStd->GetBridgeSize()>m_nBridge)
		pBri = pDataStd->GetBridgeRebar(m_nBridge);
	
	return pBri;
}

long CDrawingOthers::GetNumberBridge(long nCategoryPageNum)
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;
	CPageInfo* pInfo = pStd->GetPageInfoOrg(_T("��Ÿ ����"), nCategoryPageNum);
	if(pInfo==NULL) return -1;

	return pInfo->m_nBri;
}

void CDrawingOthers::DrawOnePage()
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;
	CARcBridgeDrawingOptionStd* pOpt = pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDataStd* pDataStd = pStd->m_pARcBridgeDataStd;
	CDrawDogak* pDgk = pStd->GetDogak();	
	CDrawPageMng* pMng = pStd->GetPageManage();

	long nPage = pStd->m_nCurPage;
	long nSubPage = nPage;

	SetNumber(pStd->m_nDrawPage);
	double dScale = GetScale();

	CDomyun* pDom = pStd->GetDomyun(nPage);
	pDom->SetScaleDim(dScale);

	CPageInfo* pInfo = pStd->GetPageInfoOrg(_T("��Ÿ ����"), pStd->m_nDrawPage-m_nSttPage);
	if(pInfo==NULL) return;

	nPage = pStd->GetPageNum(pInfo);

	pStd->m_pOptStd->SetEnvType(pDom, HCAD_MIS3);
	pStd->m_pOptStd->SetEnvType(pDom, HCAD_SYMB);

	CDomyun Dom(pDom);
	Dom.SetScaleDim(dScale);

	pMng->SetProgressData(_T("���� ��� ��..."), 0);

	long nSttPage = 0;
	CString strDgkName = pOpt->m_strDgkName;
	CString strHeadNote = pOpt->m_strDomNumHead;
	CString strSmallTitle = GetTitle();
	CString strXRefPath = IsDxfOut() ? pOpt->m_strXRefPath : _T("");

	CRcBridgeApp* pBri = pDataStd->GetBridge(m_nBridge);
	if(pBri == NULL) return;
	pDataStd->m_strTempName = pBri->m_strBridgeName;

	pDgk->Add(&Dom, nPage, dScale, strDgkName, strHeadNote, strSmallTitle, nSttPage, strXRefPath);
	pDgk->AddTitle(&Dom, "", strSmallTitle, _T(""), FALSE);

	pDgk->SetHCADExtendedData(pDataStd->m_strProjectName, pBri->m_strBridgeName, pDataStd->m_strEngName,
							  pDataStd->m_strConstructorName, pDataStd->m_strSpotName, _T(""));

	// ���� ũ�⸦ �������� �Ѱ��༭ �ִ��� �����ϰ� ��ġ �ǵ��� ������ �ش�.
	Dom.SetCalcExtRect();
	CDRect rcDogak	= Dom.GetExtRect();
	if(m_bDrawForMe)
		*pDom << Dom;
	else
		Dom.ClearEtt(TRUE, FALSE);

	// ��Ÿ ���� ���
	long nTypeBridge = pBri->m_nTypeBridge;
	long nCountGuardWall	= 1;
	long nCountMoun			= pBri->m_nMonumenu>0 ? 1 : 0;
	long nCountShoe			= (nTypeBridge == BRIDGE_TYPE_PI || nTypeBridge == BRIDGE_TYPE_SLAB || nTypeBridge == BRIDGE_TYPE_TRAP)? 1 : 0;
	long nCountEHP			= m_arrEhpDxfName.GetSize();
	long nCountHyunta		= pStd->GetCountPageOther(m_nBridge, nCountEHP) - nCountGuardWall - nCountMoun - nCountEHP;
	
	
	long nPageNum = pInfo->m_nPageNumber;
	BOOL bDraw		= FALSE;
	
	// ��ȣ��
	if(nPageNum < nCountGuardWall && !bDraw)
	{
		bDraw	= TRUE;
		AddPageOthers(&Dom, rcDogak);
	}
	else
		nPageNum -= nCountGuardWall;

	// ������ ��ħ
	if(nPageNum < nCountMoun && !bDraw)
	{
		bDraw	= TRUE;
		AddPageMonument(&Dom, rcDogak);
	}
	else
		nPageNum -= nCountMoun;

	//���� ��ħ
	if(nPageNum < nCountShoe  && !bDraw)
	{
		bDraw = TRUE;
		AddPageShoe(&Dom, rcDogak);
	}
	else
		nPageNum -=nCountShoe;


	// ����Ÿ��
	if(nPageNum < nCountHyunta && !bDraw)
	{
		bDraw	= TRUE;
		
		long nIdx	= 0;
		for(long n=0; n<pBri->m_nQtyJigan+1; n++)
		{
			CFootingApp *pFoot = pBri->GetFootingByJijumNumber(n);
			if(pFoot == NULL) continue;
			
			CExPileApp *pPile = &pFoot->m_exFooting.m_Pile;
			if(pPile->m_bIs && pPile->m_nType == PILE_TYPE_HYUNJANG)
			{
				if(nIdx == nPageNum)
				{
					AddPageHyunTa(&Dom, rcDogak, nSubPage, n);
					break;
				}
				else
					nIdx++;
			}
		}
	}
	else
		nPageNum -= nCountHyunta;

	// ehp
	if(nPageNum < nCountEHP && !bDraw)
	{
//		bDraw	= TRUE;
		AddPageEHP(&Dom, rcDogak, nPageNum);
	}
// 	else
// 		nPageNum -= nCountEHP;

	pMng->SetProgressData(_T("��� �Ϸ�  ..."), 100);

	*pDom << Dom;
}

void CDrawingOthers::AddPageOthers(CDomyun *pDomP, CDRect rcDogak)
{
	CARcBridgeDrawingStd* pStd			= (CARcBridgeDrawingStd*)m_pStd;
	CRcBridgeRebar *pBri				= GetRcBridgeRebar();
	CARcBridgeDrawingOptionStd* pOpt	= pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData	= pOpt->GetDrawingOption(m_nBridge);
	CDrawPageMng* pPageMng				= pStd->GetPageManage();
	COutMovePoint* pMove				= pStd->GetMovePoint(pStd->m_nCurPage);
	CHgBaseDrawStd baseDraw;

	CARcBridgeDBStandard DBStd(pBri, pStd->m_pARoadOptionStd, pStd->m_pARcBridgeDataStd);
	DBStd.SetSelectFckDomText(pOpt->m_nSelectFckDomText);
	
	double  dScale       = pData->m_dScaleRebarOthers;
	double  dScaleDetail = pData->m_dScaleDetailGuardWall;
	
	long	nCountDom	= 8;
	double	dAddPer		= 100 / nCountDom;
	double	dCurPer		= 0;

	CDomyun Dom(pDomP);
	CDomyun Dom2(pDomP);
	Dom.SetScaleDim(dScaleDetail);
	Dom2.SetScaleDim(dScaleDetail);

	// ���鵵 //////////////////////////////////////////////////////////////////////
	CDPoint xyMove(0, 0);
	CDRect rcFront(0, 0, 0, 0);	// ���鵵 ����
	CDRect rcFront2(0, 0, 0, 0);// ����+����
	pPageMng->SetProgressData(_T("��ȣ�� �����..."), (int)(dCurPer+=dAddPer));
	
	long nCnt = 0;
	long nIdx = 0;
	long nTableSu = 0;
	long nHDan = -1;	// �ߺ��Ǵ� ��ȣ���� �ִ� ����
	long nQtyHDan = pBri->GetQtyHDan();
	CGuardWallRC* pGW	= NULL;
	long i=0; for(i=0; i<nQtyHDan; i++)
	{
		if(m_pStd->IsDrawByBlock(MPE_REBAR_SLABFRONT+nIdx))
		{
			pGW = pBri->GetGuardWallByHDan(i);
			if(!pGW) continue;
			BOOL bCheck = FALSE;

			if((pGW->IsTypeHDanRoadGuardFence() || pGW->IsTypeHDanRailDetectGravel()) && !pGW->m_bSoundProof)
			{
				BOOL bDup = pGW->IsTypeHDanRoadGuardFence() ? IsDuplicate(i, nHDan, HDAN_TYPE_ROAD_GUARDFENCE)
															: IsDuplicate(i, nHDan, HDAN_TYPE_RAIL_DETECTGRAVEL);	// �ߺ��Ǵ� ���� �ִ��� �˻�
				if(!bDup || (bDup && i<nHDan))	// �����Ѱ� Ȥ�� �����͸� ���
				{
					DBStd.m_pDrawCrossSection->DrawRebarGuardFence(&Dom, i, TRUE, TRUE, FALSE);
					bCheck = TRUE;
				}
			}
			else if(pGW->IsTypeHDanRoadCentralWallFull() && !pGW->m_bSoundProof)
			{
				BOOL bDup = IsDuplicate(i, nHDan, HDAN_TYPE_ROAD_CENTERGUARD);	// �ߺ��Ǵ� ���� �ִ��� �˻�
				if(!bDup || (bDup && i<nHDan))	// �����Ѱ� Ȥ�� �����͸� ���
				{
					DBStd.m_pDrawCrossSection->DrawRebarCenterGuard(&Dom, i, TRUE, TRUE, FALSE);
					bCheck = TRUE;
				}
			}
			else if(pGW->IsTypeHDanRoadCentralWallHalf() && !pGW->m_bSoundProof)
			{
				BOOL bDup = IsDuplicate(i, nHDan, HDAN_TYPE_ROAD_CENTERGUARDHALF);	// �ߺ��Ǵ� ���� �ִ��� �˻�
				if(!bDup || (bDup && i<nHDan))	// �����Ѱ� Ȥ�� �����͸� ���
				{
					DBStd.m_pDrawCrossSection->DrawRebarCenterGuardHalf(&Dom, i, TRUE, TRUE, FALSE);
					bCheck = TRUE;
				}
			}
			else if(pGW->IsTypeHDanRoadHandRail() && !pGW->m_bSoundProof)
			{
				BOOL bDup = IsDuplicate(i, nHDan, HDAN_TYPE_ROAD_HANDRAIL);	// �ߺ��Ǵ� ���� �ִ��� �˻�
				if(!bDup || (bDup && i<nHDan))	// �����Ѱ� Ȥ�� �����͸� ���
				{
					DBStd.m_pDrawCrossSection->DrawRebarHandRail(&Dom, i, TRUE, TRUE, FALSE);
					bCheck = TRUE;
				}
			}
			else if(pGW->IsTypeHDanRoadSoundProof() || pGW->m_bSoundProof)
			{
				BOOL bDup = IsDuplicate(i, nHDan, HDAN_TYPE_ROAD_SOUNDPROOF);	// �ߺ��Ǵ� ���� �ִ��� �˻�
				if(!bDup || (bDup && i<nHDan))	// �����Ѱ� Ȥ�� �����͸� ���
				{
					DBStd.m_pDrawCrossSection->DrawRebarSoundProof(&Dom, i, TRUE, TRUE, FALSE);
					bCheck = TRUE;
				}
			}
			else if(pGW->IsTypeHDanRoadRetainingWall())
			{
				BOOL bDup = IsDuplicate(i, nHDan, HDAN_TYPE_ROAD_RETAININGWALL);	// �ߺ��Ǵ� ���� �ִ��� �˻�
				if(!bDup || (bDup && i<nHDan))	// �����Ѱ� Ȥ�� �����͸� ���
				{
					DBStd.m_pDrawCrossSection->DrawRebarRetainingWall(&Dom, i, TRUE, TRUE, FALSE);
					bCheck = TRUE;
				}
			}
			else if(pGW->IsTypeHDanRailFootwayBlock())
			{
				BOOL bDup = IsDuplicate(i, nHDan, HDAN_TYPE_RAIL_FOOTWAYBLOCK);	// �ߺ��Ǵ� ���� �ִ��� �˻�
				if(!bDup || (bDup && i<nHDan))	// �����Ѱ� Ȥ�� �����͸� ���
				{
					DBStd.m_pDrawCrossSection->DrawRebarFootwayBlock(&Dom, i, TRUE, TRUE, FALSE);
					DBStd.m_pDrawCrossSection->DrawRebarFootwayBlock(&Dom2, i, TRUE, TRUE, FALSE, TRUE);
					bCheck = TRUE;
				}
			}
			else if(pGW->IsTypeHDanRailGuardFence())
			{
				BOOL bDup = IsDuplicate(i, nHDan, HDAN_TYPE_RAIL_GUARDFENCE);	// �ߺ��Ǵ� ���� �ִ��� �˻�
				if(!bDup || (bDup && i<nHDan))	// �����Ѱ� Ȥ�� �����͸� ���
				{
					DBStd.m_pDrawCrossSection->DrawRebarRailGuardFence(&Dom, i, TRUE, TRUE, FALSE);
					bCheck = TRUE;
				}
			}
			else if(pGW->IsTypeHDanRailHole())
			{
				BOOL bDup = IsDuplicate(i, nHDan, HDAN_TYPE_RAIL_HOLE);	// �ߺ��Ǵ� ���� �ִ��� �˻�
				if(!bDup || (bDup && i<nHDan))	// �����Ѱ� Ȥ�� �����͸� ���
				{
					DBStd.m_pDrawCrossSection->DrawRebarTotalHole(&Dom, i, TRUE, TRUE, FALSE);
					DBStd.m_pDrawCrossSection->DrawRebarTotalHole(&Dom2, i, TRUE, TRUE, FALSE, TRUE);
					bCheck = TRUE;
				}
			}
			else if(pGW->IsTypeHDanRailHoleFootway())
			{
				BOOL bDup = IsDuplicate(i, nHDan, HDAN_TYPE_RAIL_HOLEFOOTWAY);	// �ߺ��Ǵ� ���� �ִ��� �˻�
				if(!bDup || (bDup && i<nHDan))	// �����Ѱ� Ȥ�� �����͸� ���
				{
					DBStd.m_pDrawCrossSection->DrawRebarTotalHole(&Dom, i, TRUE, TRUE, FALSE);
					DBStd.m_pDrawCrossSection->DrawRebarTotalHole(&Dom2, i, TRUE, TRUE, FALSE, TRUE);
					bCheck = TRUE;
				}
			}
			
			if(bCheck==FALSE) continue;
		}
		// �ߺ��Ǵ� ������ ��������� �˾Ƴ���.
		long pGugan[5];
		for(long k=0; k<5; k++)
			pGugan[k] = -1;
		long nType = pGW->m_nTypeGuardWall;
		GetDupGugan(i, pGugan, nType);		
		
		if(m_pStd->IsDrawByBlock(MPE_REBAR_SLABFRONT+nIdx+1))
		{
			if(pGW->IsTypeHDanRailFootwayBlock() || pGW->IsTypeHDanRailTotalHole())
			{
				Dom2.RedrawByScale(dScale/dScaleDetail);
				Dom2.SetCalcExtRect();
				rcFront2 = Dom2.GetExtRect();
				Dom2.Move(-rcFront2.right+rcFront2.Width()*3, -rcFront2.bottom + Dom2.Always(1300));
				Dom2.Move(pMove->GetPoint(MPE_REBAR_SLABFRONT+nIdx));
				Dom2.SetCalcExtRect();
				rcFront2 = Dom2.GetExtRect();	
				pStd->ChangeSeperateLayer(&Dom2);
				pMove->AddMPE(&Dom2, Dom2.GetExtRect(), MPE_REBAR_SLABFRONT+nIdx+1, _T("���鵵"));
				*pDomP << Dom2;
			}
		}
		
		if(m_pStd->IsDrawByBlock(MPE_REBAR_SLABFRONT+nIdx))
		{
			Dom.RedrawByScale(dScale/dScaleDetail);
			Dom.SetCalcExtRect();
			rcFront = Dom.GetExtRect();
			Dom.Move(rcDogak.left - rcFront.left + pDomP->Always(50) + nIdx * pDomP->Always(400), rcDogak.bottom - rcFront.bottom - pDomP->Always(50));
			Dom.Move(pMove->GetPoint(MPE_REBAR_SLABFRONT+nIdx));
			Dom.SetCalcExtRect();
			rcFront = Dom.GetExtRect();	
			pStd->ChangeSeperateLayer(&Dom);
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_SLABFRONT+nIdx, _T("���鵵"));
			*pDomP << Dom;
		}
		
		CString str = pBri->GetGuardWallName(pGW);
		if(m_pStd->IsDrawByBlock(MPE_REBAR_SLABFRONT_TITLE+nIdx))
		{
			CDomyun DomTitle(pDomP);
			pStd->AddTitleOnDom(&DomTitle, m_nBridge, str, xyMove, 0, dScaleDetail);		
			DomTitle.Move(rcFront.CenterPoint().x, rcFront.bottom + DomTitle.Always(15));
			DomTitle.Move(pMove->GetPoint(MPE_REBAR_SLABFRONT_TITLE+nIdx));
			pStd->ChangeSeperateLayer(&DomTitle);
			DomTitle.SetCalcExtRect();
			pMove->AddMPE(&DomTitle, DomTitle.GetExtRect(), MPE_REBAR_SLABFRONT_TITLE+nIdx, _T("���鵵 Ÿ��Ʋ"));
			*pDomP << DomTitle;
		}


		// ��Ÿö�� �и� ����� ��쿡��..
		if(pData->m_bSeparateRebarBMOther)
		{
			CDomyun Dom3(pDomP);
			CHgBaseConcStd baseConc;
			CDRect rcDetailRB(0, 0, 0, 0);
			CDPoint xyBase(0);

			CTypedPtrArray <CObArray, CRebar*> arrRebar;	// ���ǥ�� ö�ٸ���Ʈ
			MakeRebarList(pGW, &arrRebar);

			long nSize = arrRebar.GetSize();
			if(nSize < 0) continue;

			if(pOpt->m_nTypeRebarTable == 0) //�Ϲ�Ÿ��
			{
				pPageMng->SetProgressData(_T("ö�ٻ� �����..."), 20);
				baseConc.ShellSortRebarList(&arrRebar, &CRebar::CompareForList);
				nCnt = DrawRebarDetail(&Dom3, &arrRebar, str, rcFront, nCnt);
				
				Dom3.SetCalcExtRect();
				rcDetailRB	= Dom3.GetExtRect();
				pStd->ChangeSeperateLayer(&Dom3);
				*pDomP << Dom3;

				rcFront = CDPoint(rcFront.CenterPoint().x, Dom3.Always(0));
										  
			}
			else //���ǥ ���� 
			{
				rcFront = CDPoint(rcFront.CenterPoint().x, Dom3.Always(350));
			}
			
			baseConc.SetRebarIndexBySortRebarList(&arrRebar, TRUE);//���̺� ��ȣ ����
			CDRect rcTableRB(0, 0, 0, 0);
			
			// ö�� ���ǥ
			pPageMng->SetProgressData(_T("ö�����ǥ �����..."), (int)(dCurPer+=dAddPer));
			baseConc.ShellSortRebarList(&arrRebar, &CRebar::CompareForTable);
			xyBase = DrawRebarTable(&Dom3, &arrRebar, str, _T(""), rcFront, pGugan, nTableSu++, TRUE, pGW);
		
			Dom3.SetCalcExtRect();
			rcDetailRB	= Dom3.GetExtRect();
			pStd->ChangeSeperateLayer(&Dom3);
			*pDomP << Dom3;

			if(pOpt->m_nTypeRebarTable == 1) //���鰣��ȭŸ��
			{
				pPageMng->SetProgressData("ö�ٻ� �����...", 80);
				baseConc.ShellSortRebarList(&arrRebar, &CRebar::CompareForList);
				baseDraw.DrawRebarListDetailSimple(&Dom3, &arrRebar, xyBase, pOpt->m_dWidthRebarTableRect, pOpt->m_dHeightRebarTableRect);

				Dom3.SetCalcExtRect();
				Dom3.Move(pMove->GetPoint(MPE_REBAR_MONUDETAIL + nTableSu));
				pStd->ChangeSeperateLayer(&Dom3);
				pMove->AddMPE(&Dom3, Dom3.GetExtRect(), MPE_REBAR_MONUDETAIL + nTableSu, hggettext("ö�ٻ� ����"));

				*pDomP << Dom3;
			}

			AhTPADelete(&arrRebar,	(CRebar*)0);

			++nTableSu;
			DrawRebarDetailTable_RawRebar(pDomP, pGW, xyBase, nTableSu);
		}

		nIdx++;
	}
}

void CDrawingOthers::AddPageMonument(CDomyun *pDomP, CDRect rcDogak)
{
	CARcBridgeDrawingStd* pStd			= (CARcBridgeDrawingStd*)m_pStd;
	CRcBridgeRebar *pBri				= GetRcBridgeRebar();
	CARcBridgeDrawingOptionStd* pOpt	= pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData	= pOpt->GetDrawingOption(m_nBridge);
	CDrawPageMng* pPageMng				= pStd->GetPageManage();
	COutMovePoint* pMove				= pStd->GetMovePoint(pStd->m_nCurPage);
	CHgBaseDrawStd baseDraw;

	CARcBridgeDBStandard DBStd(pBri, pStd->m_pARoadOptionStd, pStd->m_pARcBridgeDataStd);
	DBStd.SetSelectFckDomText(pOpt->m_nSelectFckDomText);
	
	double  dScale       = pData->m_dScaleRebarOthers;
	double  dScaleDetail = pData->m_dScaleDetailMonument;

	long	nCountDom	= 8;
	double	dAddPer		= 100 / nCountDom;
	double	dCurPer		= 0;
	
	CDPoint xyMove(0, 0);
	CDRect rcFront(0,0,0,0);
	CDRect rcPlane(0,0,0,0);

	// ���鵵
	CDomyun DomFront(pDomP);
	DomFront.SetScaleDim(dScaleDetail);
	if(m_pStd->IsDrawByBlock(MPE_REBAR_MONUFRONT))
	{
		pPageMng->SetProgressData(_T("���鵵 �����..."), (int)(dCurPer+=dAddPer));

		DBStd.m_pDrawCrossSection->DrawMonumentFront(&DomFront);
		DBStd.m_pDrawCrossSection->DrawRebarMonumentFront(&DomFront);
		DBStd.m_pDrawCrossSection->DimRebarMonumentFront(&DomFront, FALSE);
		DBStd.m_pDrawCrossSection->MarkRebarMonuFront(&DomFront, FALSE);


		DomFront.RedrawByScale(dScale/dScaleDetail);
		DomFront.SetCalcExtRect();
		rcFront = DomFront.GetExtRect();
		DomFront.Move(rcDogak.left - rcFront.left + DomFront.Always(50), rcDogak.bottom - rcFront.bottom - DomFront.Always(100));
		DomFront.Move(pMove->GetPoint(MPE_REBAR_MONUFRONT));
		DomFront.SetCalcExtRect();
		pStd->ChangeSeperateLayer(&DomFront);
		pMove->AddMPE(&DomFront, DomFront.GetExtRect(), MPE_REBAR_MONUFRONT, _T("���鵵"));
		*pDomP << DomFront;
	}

	if(m_pStd->IsDrawByBlock(MPE_REBAR_MONUFRONT_TITLE))
	{
		CDomyun DomTitle(pDomP);
		DomFront.SetCalcExtRect();
		rcFront = DomFront.GetExtRect();
		pStd->AddTitleOnDom(&DomTitle, m_nBridge, _T("���鵵"), CDPoint(0,0), 0, dScaleDetail);
		DomTitle.Move(rcFront.CenterPoint().x, rcFront.bottom+DomTitle.Always(20));
		DomTitle.Move(pMove->GetPoint(MPE_REBAR_MONUFRONT_TITLE));
		pStd->ChangeSeperateLayer(&DomTitle);
		DomTitle.SetCalcExtRect();
		pMove->AddMPE(&DomTitle, DomTitle.GetExtRect(), MPE_REBAR_MONUFRONT_TITLE, _T("���鵵 Ÿ��Ʋ"));
		*pDomP << DomTitle;
	}

	// ��鵵
	CDomyun DomPlane(pDomP);
	if(m_pStd->IsDrawByBlock(MPE_REBAR_MONUPLANE))
	{
		DomPlane.SetScaleDim(dScaleDetail);
		pPageMng->SetProgressData(_T("��鵵 �����..."), (int)(dCurPer+=dAddPer));

		DBStd.m_pDrawCrossSection->DrawMonumentPlane(&DomPlane);
		DBStd.m_pDrawCrossSection->DrawRebarMonumentPlane(&DomPlane);
		DBStd.m_pDrawCrossSection->DimRebarMonumentPlane(&DomPlane, FALSE);
		DBStd.m_pDrawCrossSection->MarkRebarMonuPlane(&DomPlane, FALSE);

		DomPlane.RedrawByScale(dScale/dScaleDetail);
		DomPlane.SetCalcExtRect();
		rcPlane = DomPlane.GetExtRect();
		DomPlane.Move(rcDogak.left - rcPlane.left + pDomP->Always(300), rcDogak.bottom - rcPlane.bottom - pDomP->Always(100));
		DomPlane.Move(pMove->GetPoint(MPE_REBAR_MONUPLANE));
		DomPlane.SetCalcExtRect();
		pStd->ChangeSeperateLayer(&DomPlane);
		pMove->AddMPE(&DomPlane, DomPlane.GetExtRect(), MPE_REBAR_MONUPLANE, _T("��鵵"));
		*pDomP << DomPlane;
	}

	CDomyun DomTitle2(pDomP);
	if(m_pStd->IsDrawByBlock(MPE_REBAR_MONUPLANE_TITLE))
	{
		DomPlane.SetCalcExtRect();
		rcPlane = DomPlane.GetExtRect();
		pStd->AddTitleOnDom(&DomTitle2, m_nBridge, _T("��鵵"), CDPoint(0,0), 0, dScaleDetail);
		DomTitle2.Move(rcPlane.CenterPoint().x, rcPlane.bottom+DomTitle2.Always(20));
		DomTitle2.Move(pMove->GetPoint(MPE_REBAR_MONUPLANE_TITLE));
		pStd->ChangeSeperateLayer(&DomTitle2);
		DomTitle2.SetCalcExtRect();
		pMove->AddMPE(&DomTitle2, DomTitle2.GetExtRect(), MPE_REBAR_MONUPLANE_TITLE, _T("��鵵 Ÿ��Ʋ"));
		*pDomP << DomTitle2;
	}

	// ��Ÿö�� �и������ ��쿡�� ����� �����
	CDomyun Dom2(pDomP);
	if(pData->m_bSeparateRebarBMOther)
	{
		CHgBaseConcStd baseConc;
		CDRect rcDetailRB(0, 0, 0, 0);
		CDPoint xyBase(0);
		
		CTypedPtrArray <CObArray, CRebar*> arrRebar;	// ���ǥ�� ö�ٸ���Ʈ
		MakeRebarList(&arrRebar);
		
		long nSize = arrRebar.GetSize();
		if(nSize > 0)
		{
			if(pOpt->m_nTypeRebarTable == 0) //�⺻Ÿ��
			{
				pPageMng->SetProgressData(_T("ö�ٻ� �����..."), 20);
				baseConc.ShellSortRebarList(&arrRebar, &CRebar::CompareForList);
				DrawRebarDetail(&Dom2, &arrRebar, _T(""), rcFront, 0, FALSE, TRUE);

				Dom2.SetCalcExtRect();
				rcDetailRB	= Dom2.GetExtRect();
				*pDomP << Dom2;
				rcFront = CDPoint(rcFront.left+Dom2.Always(620), rcFront.bottom+Dom2.Always(10));
			}
			else //���ǥ ���� 
			{
				rcFront.left = rcFront.right+Dom2.Always(350);
				rcFront.top = rcFront.top+Dom2.Always(10);
			}

			CDRect rcTableRB(0, 0, 0, 0);
			baseConc.SetRebarIndexBySortRebarList(&arrRebar, TRUE);//���̺� ��ȣ ����

			// ö�� ���ǥ
			pPageMng->SetProgressData(_T("ö�����ǥ �����..."), (int)(dCurPer+=dAddPer));
			baseConc.ShellSortRebarList(&arrRebar, &CRebar::CompareForTable);
			xyBase = DrawRebarTable(&Dom2, &arrRebar, _T(""), _T(""), rcFront, NULL, 0, FALSE, NULL, TRUE);

			Dom2.SetCalcExtRect();
			rcDetailRB	= Dom2.GetExtRect();
			*pDomP << Dom2;

			if(pOpt->m_nTypeRebarTable == 1) //���鰣��ȭŸ��
			{
				pPageMng->SetProgressData("ö�ٻ� �����...", 80);
				baseConc.ShellSortRebarList(&arrRebar, &CRebar::CompareForList);
				baseDraw.DrawRebarListDetailSimple(&Dom2, &arrRebar, xyBase, pOpt->m_dWidthRebarTableRect, pOpt->m_dHeightRebarTableRect);

				Dom2.SetCalcExtRect();
				Dom2.Move(pMove->GetPoint(MPE_REBAR_MONUDETAIL));
				pStd->ChangeSeperateLayer(&Dom2);
				pMove->AddMPE(&Dom2, Dom2.GetExtRect(), MPE_REBAR_MONUDETAIL, hggettext("ö�ٻ� ����"));

				*pDomP << Dom2;
			}

			AhTPADelete(&arrRebar,	(CRebar*)0);
			long nIdxTable(2);
			DrawRebarDetailTable_RawRebar(pDomP, NULL, xyBase, nIdxTable);
		}
	}
}

void CDrawingOthers::AddPageShoe(CDomyun *pDomP, CDRect rcDogak)
{
	CARcBridgeDrawingStd* pStd			= (CARcBridgeDrawingStd*)m_pStd;
	CRcBridgeRebar *pBri				= GetRcBridgeRebar();
	if(!pBri) return;

	double dScale = GetScale();
	CDomyun Dom(pDomP);
	CDrawPageMng* pPageMng				= pStd->GetPageManage();
	COutMovePoint* pMove				= pStd->GetMovePoint(pStd->m_nCurPage);

	CDRect rcShoe, rcCoordTable, rcList, rcBum;

	double dGapDogak = GAP_LEFT + Dom.Always(20);

	//��ġ��
	if(m_pStd->IsDrawByBlock(MPE_SHOE))
	{
		pPageMng->SetProgressData(_T("������ħ��ġ�� �����..."), 60);
		AddShoePosition(&Dom);

		Dom.SetCalcExtRect();
		rcShoe = Dom.GetExtRect();

		Dom.Move(-rcShoe.left, -rcShoe.top);
		Dom.Move((rcDogak.Width() - rcShoe.Width())/2, rcDogak.Height() - rcShoe.Height() - Dom.Always(30));
		Dom.Move(pMove->GetPoint(MPE_SHOE));
		Dom.SetCalcExtRect();
		rcShoe = Dom.GetExtRect();
		pStd->ChangeSeperateLayer(&Dom);
		pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_SHOE, _T("������ħ��ġ��"));
		*pDomP << Dom;
	}

	//������ħ ��ǥ
	if(m_pStd->IsDrawByBlock(MPE_SHOE_TABLE))
	{
		pPageMng->SetProgressData(_T("������ħ ��ǥ �����..."), 60);
		AddTableShoeCoordinate(&Dom);

		Dom.SetCalcExtRect();
		rcCoordTable = Dom.GetExtRect();

		Dom.Move(dGapDogak, rcCoordTable.Height() + dGapDogak);
		Dom.Move(pMove->GetPoint(MPE_SHOE_TABLE));
		Dom.SetCalcExtRect();
		rcCoordTable = Dom.GetExtRect();
		pStd->ChangeSeperateLayer(&Dom);
		pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_SHOE_TABLE, _T("������ħ ��ǥ"));
		*pDomP << Dom;

		pStd->m_pOptStd->SetEnvType(&Dom, HCAD_SMALL_TITLE);	
		CDPoint xyTytle = CDPoint(dGapDogak + rcCoordTable.Width()/2, dGapDogak + rcCoordTable.Height() + GAP_TITLE);
		pStd->AddTitleOnDom(&Dom, m_nBridge, hggettext("������ħ ��ǥ"), xyTytle, -1, dScale);
		Dom.Move(pMove->GetPoint(MPE_SHOE_TABLE_TITLE));
		pStd->ChangeSeperateLayer(&Dom);
		Dom.SetCalcExtRect();
		pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_SHOE_TABLE_TITLE, hggettext("������ħ ��ǥ Ÿ��Ʋ"));
		
		*pDomP << Dom;
	}

	//������ħ ����Ʈ
	if(m_pStd->IsDrawByBlock(MPE_SHOE_LIST))
	{
		pPageMng->SetProgressData(_T("������ħ ����Ʈ �����..."), 60);
		AddTableShoeList(&Dom);
		pStd->ChangeSeperateLayer(&Dom);

		Dom.SetCalcExtRect();
		rcList = Dom.GetExtRect();

		Dom.Move(rcDogak.right - rcList.Width() - dGapDogak, rcList.Height() + dGapDogak);
		Dom.Move(pMove->GetPoint(MPE_SHOE_LIST));
		Dom.SetCalcExtRect();
		rcList = Dom.GetExtRect();
		pStd->ChangeSeperateLayer(&Dom);
		pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_SHOE_LIST, _T("������ħ ����Ʈ"));
		*pDomP << Dom;

		pStd->m_pOptStd->SetEnvType(&Dom, HCAD_SMALL_TITLE);	
		CDPoint xyTytle = CDPoint(rcDogak.right - rcList.Width() / 2 - dGapDogak, rcList.Height() + dGapDogak + GAP_TITLE);
		pStd->AddTitleOnDom(&Dom, m_nBridge, hggettext("������ħ ����Ʈ"), xyTytle, -1, dScale);
		Dom.Move(pMove->GetPoint(MPE_SHOE_LIST_TITLE));
		pStd->ChangeSeperateLayer(&Dom);
		Dom.SetCalcExtRect();
		pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_SHOE_LIST_TITLE, hggettext("������ħ ����Ʈ Ÿ��Ʋ"));
		*pDomP << Dom;
	}

	//����
	if(m_pStd->IsDrawByBlock(MPE_SHOE_BUM))
	{
		pPageMng->SetProgressData(_T("���� �����..."), 60);
		AddTableShoeLegend(&Dom);

		Dom.SetCalcExtRect();
		rcBum = Dom.GetExtRect();

		Dom.Move(rcDogak.right - rcBum.Width() - dGapDogak, Dom.Always(250));
		Dom.Move(pMove->GetPoint(MPE_SHOE_BUM));
		Dom.SetCalcExtRect();
		rcBum = Dom.GetExtRect();
		pStd->ChangeSeperateLayer(&Dom);
		pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_SHOE_BUM, _T("����"));
		*pDomP << Dom;

		pStd->m_pOptStd->SetEnvType(&Dom, HCAD_SMALL_TITLE);	
		CDPoint xyTytle = CDPoint(rcDogak.right - rcBum.Width() / 2 - dGapDogak, Dom.Always(250) + GAP_TITLE);
		pStd->AddTitleOnDom(&Dom, m_nBridge, hggettext("����"), xyTytle, -1, dScale);
		Dom.Move(pMove->GetPoint(MPE_SHOE_BUM_TITLE));
		pStd->ChangeSeperateLayer(&Dom);
		Dom.SetCalcExtRect();
		pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_SHOE_BUM_TITLE, hggettext("���� Ÿ��Ʋ"));
		*pDomP << Dom;
	}

	*pDomP << Dom;
}

void CDrawingOthers::AddShoePosition(CDomyun *pDom)
{
	CARcBridgeDrawingStd* pStd			= (CARcBridgeDrawingStd*)m_pStd;
	CRcBridgeRebar *pBri				= GetRcBridgeRebar();
	if(!pBri) return;

	CDomyun Dom(pDom);

	CARcBridgeDBStandard DBStd(pBri, pStd->m_pARoadOptionStd, pStd->m_pARcBridgeDataStd);

	DBStd.m_pDrawPlane->DrawPlaneGen(&Dom);
	DBStd.m_pDrawPlane->DimPlaneJijumAngle(&Dom);
	DBStd.m_pDrawPlane->DimPlaneJijumLength(&Dom,FALSE,FALSE,TRUE);
	DBStd.m_pDrawPlane->MarkPlaneBridgeSttEnd(&Dom);

	CEnvRegist env;		
	BOOL bRotate = env.LoadEnvInt(CEnvRegist::INPUTDOM_ROTATE);
	if(bRotate)
	{
		// ���� ���� �������� ȸ��
		DBStd.RotateDomByBridgeSttEndXy(&Dom);
	}

	*pDom << Dom;

}
void CDrawingOthers::AddTableShoeCoordinate(CDomyun *pDom)
{
	CARcBridgeDrawingStd *pStd = (CARcBridgeDrawingStd*)m_pStd;
	CRcBridgeRebar *pBri = GetRcBridgeRebar();

	CJijum* pJ   = NULL; 
	CDPoint vAng = CDPoint(0,0);
	CGridDomyunEx Dom(pDom);

	long nAlign = GRID_ALIGN_CENTER | GRID_ALIGN_VCENTER;
	long nCountJijum = pBri->GetCountJijum();  //�����Ѱ���
	long nQtyJijum = pBri->GetCountJijumBearing(); //�����ִ� ��������
	long nColSu = 6 * nQtyJijum;
	long nRowSu = pBri->GetMaxShoe();
	long nCountShoe  = 0;
	CString str(_T(""));

	pStd->m_pOptStd->SetEnvType(&Dom, HCAD_TBL1);	
	long nCol = 0;
	double Height = 8.0;
	double Width = 40.0;

	Dom.SetColumnCount(nColSu);
	Dom.SetRowCount(nRowSu + 2);
	Dom.SetMargin(3);
	Dom.SetRowHeightAll(Dom.Always(Height));
	Dom.SetColumnWidthAll(Dom.Always(Width));

	long nJijumStt = 0;
	long nJijumEnd = nCountJijum-1;
	
	for(long nJ=nJijumStt; nJ<=nJijumEnd; nJ++)
	{
		pJ = pBri->GetJijum(nJ);
		for(long nLeft = 0; nLeft < 2; nLeft++) // nLeft ������ ������ħ��...
		{
			if(nJ == nJijumStt && nLeft == 0) continue;
			if(nJ == nJijumEnd && nLeft == 1) break;
			if(pJ->m_strJijumShoe[nLeft] == _T("")) continue;
			if(pJ->m_nQtyShoe[nLeft] <= 0) continue;
				
			Dom.SetColumnWidth(nCol, Dom.Always(13));
			Dom.SetColumnWidth(nCol+1, Dom.Always(35.0));
			Dom.SetTextMatrix(1, nCol, hggettext("����"), nAlign);
			Dom.SetMergeCol(0, nCol, nCol + 5);
			Dom.SetTextMatrix(0, nCol, pJ->m_strJijumShoe[nLeft], nAlign);
			Dom.SetTextMatrix(1, nCol+1, hggettext("������ħ"), nAlign);
			Dom.SetTextMatrix(1, nCol+2, hggettext("X��ǥ"), nAlign);
			Dom.SetTextMatrix(1, nCol+3, hggettext("Y��ǥ"), nAlign);		
			Dom.SetTextMatrix(1, nCol+4, hggettext("Z��ǥ"), nAlign);		
			Dom.SetTextMatrix(1, nCol+5, hggettext("�Ϲݰ�����"), nAlign);	
				
			nCountShoe = pJ->m_nQtyShoe[nLeft];
			for(long nRow = 0; nRow < nCountShoe; nRow++)
			{
				long Dir = pJ->m_nDirShoe[nLeft][nRow];
				CDPoint xyShoe = pBri->GetXyShoe(nJ, nLeft, nRow);			
				vAng = pBri->GetAngleShoe(nJ, nLeft, nRow, Dir, 0, nJijumStt, nJijumEnd);

				str.Format(_T("%d"), nRow+1);
				Dom.SetTextMatrix(nRow+2, nCol+0, str, nAlign); //���� 
				Dom.SetTextMatrixCircle(nRow+2, nCol, str, nAlign); //���� ��
				Dom.SetTextMatrix(nRow+2, nCol+1, pBri->GetStrShoeDirection(Dir), nAlign); //������ħ
				str.Format(_T("%.4f"), xyShoe.x);
				Dom.SetTextMatrix(nRow+2, nCol+2, str, nAlign); //x��ǥ
				str.Format(_T("%.4f"), xyShoe.y);
				Dom.SetTextMatrix(nRow+2, nCol+3, str, nAlign); //y��ǥ
				Dom.SetTextMatrix(nRow+2, nCol+4, _T("0"), nAlign); //z��ǥ
				str.Format(_T("%.4f"), vAng.GetAngleDegree());
				Dom.SetTextMatrix(nRow+2, nCol+5, str, nAlign); //����

			}
			nCol += 6;
		}
	}

	Dom.Draw(FALSE);	
	Dom.DrawOutLine();
	*pDom << Dom;
}

void CDrawingOthers::AddTableShoeList(CDomyun *pDom)
{
	CARcBridgeDrawingStd	   *pStd	 = (CARcBridgeDrawingStd*)m_pStd;
	CARcBridgeDrawingOptionStd *pOpt	 = pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDataStd		   *pDataStd = pStd->m_pARcBridgeDataStd;
	CRcBridgeRebar *pBri = GetRcBridgeRebar();

	CString str = _T("");
	CGridDomyunEx Dom(pDom);
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_TBL1);	
	
	long nAlign = GRID_ALIGN_CENTER | GRID_ALIGN_VCENTER;
	long nCountJijum = pBri->GetCountJijum();  //�����Ѱ���
	long nQtyJijum = pBri->GetCountJijumBearing(); //�����ִ� ��������
	long nJ = 0;

	long nCols = nQtyJijum + 2;
	long nRows = 6;
	double Height = 8.0;
	double Width = 25.0;
	
	Dom.SetColumnCount(nCols);
	Dom.SetRowCount(nRows);
	Dom.SetMargin(3);
	Dom.SetRowHeightAll(Dom.Always(Height));
	Dom.SetColumnWidthAll(Dom.Always(Width));
	
	// ��� title
	Dom.SetTextMatrix(0, 0, hggettext("��  ��"), nAlign);
	Dom.SetTextMatrix(0, nQtyJijum+1, hggettext("��"), nAlign);
	long nTempCol = 0;
	for( nJ = 0; nJ < nCountJijum; nJ++)
	{	
		CJijum	*pJ	= pBri->GetJijum(nJ);
		long nV=0; 
		for(nV = 0; nV < JIJUM_VPOS_MAX; nV++)
		{
			if(pJ->m_nQtyShoe[nV] == 0) continue;

			nTempCol++;
			CString strPierMark = pJ->m_strJijumShoe[nV];
			Dom.SetTextMatrix(0, nTempCol, strPierMark, nAlign);
		}
	}

	long nTotalFix			= 0;
	long nTotalOneWay		= 0;
	long nTotalOneWayVer	= 0;
	long nTotalTwoWay		= 0;

	Dom.SetColumnWidth(0, Dom.Always(35));
	Dom.SetTextMatrix(1, 0, hggettext("������"), nAlign);
	Dom.SetTextMatrix(2, 0, hggettext("�Ϲ���(����)"), nAlign);
	Dom.SetTextMatrix(3, 0, hggettext("�Ϲ���(��������)"), nAlign);
	Dom.SetTextMatrix(4, 0, hggettext("�����"), nAlign);
	Dom.SetTextMatrix(5, 0, hggettext("��"), nAlign);		
	nTempCol = 0;
		
	for( nJ = 0; nJ < nCountJijum; nJ++)
	{
		CJijum *pJ = pBri->GetJijum(nJ);
		for(long nV = 0; nV < JIJUM_VPOS_MAX; nV++)
		{				
			if(pJ->m_nQtyShoe[nV] == 0) continue;

			nTempCol++;
		
			long nFix		= 0;
			long nOneWay	= 0;
			long nOneWayVer	= 0;
			long nTwoWay	= 0;
			long nQtyShoe	= pJ->m_nQtyShoe[nV];  //�������ִ� ��ħ����

			for(long nRow = 0; nRow < nQtyShoe; nRow++)
			{
				//dDir => 0:������, 1:�Ϲ���(����) 2:�Ϲ���(��������) 3:�����
				int dDir = pJ->m_nDirShoe[nV][nRow];
				if(dDir > 3) continue;
					
				switch(dDir)
				{
				case 0:
					nFix++; break;
				case 1:
					nOneWay++; break;
				case 2:
					nOneWayVer++; break;
				case 3:
					nTwoWay++; break;
				}
			}
			str.Format(_T("%d"), nFix);
			Dom.SetTextMatrix(1, nTempCol, str, nAlign);
			str.Format(_T("%d"), nOneWay);
			Dom.SetTextMatrix(2, nTempCol, str, nAlign);
			str.Format(_T("%d"), nOneWayVer);
			Dom.SetTextMatrix(3, nTempCol, str, nAlign);
			str.Format(_T("%d"), nTwoWay);
			Dom.SetTextMatrix(4, nTempCol, str, nAlign);
			str.Format(_T("%d"), nFix+nOneWay+nOneWayVer+nTwoWay);
			Dom.SetTextMatrix(5, nTempCol, str, nAlign);	
				
			nTotalFix		+= nFix;
			nTotalOneWay	+= nOneWay;
			nTotalOneWayVer += nOneWayVer;
			nTotalTwoWay	+= nTwoWay;
		}
	}

	//���� �հ�
	str.Format(_T("%d"), nTotalFix);
	Dom.SetTextMatrix(1, nTempCol+1, str, nAlign);
	str.Format(_T("%d"), nTotalOneWay);
	Dom.SetTextMatrix(2, nTempCol+1, str, nAlign);
	str.Format(_T("%d"), nTotalOneWayVer);
	Dom.SetTextMatrix(3, nTempCol+1, str, nAlign);
	str.Format(_T("%d"), nTotalTwoWay);
	Dom.SetTextMatrix(4, nTempCol+1, str, nAlign);
	str.Format(_T("%d"), nTotalFix+nTotalOneWay+nTotalOneWayVer+nTotalTwoWay);
	Dom.SetTextMatrix(5, nTempCol+1, str, nAlign);
	
	Dom.Draw(FALSE);	
	Dom.DrawOutLine();
	
	*pDom << Dom;
}

void CDrawingOthers::AddTableShoeLegend(CDomyun *pDom)
{
	CARcBridgeDrawingStd	   *pStd	 = (CARcBridgeDrawingStd*)m_pStd;
	CARcBridgeDrawingOptionStd *pOpt	 = pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDataStd		   *pDataStd = pStd->m_pARcBridgeDataStd;

	CGridDomyunEx Dom(pDom);
	pStd->m_pOptStd->SetEnvType(&Dom,HCAD_TBL1);	
	
	Dom.SetColumnCount(2);
	Dom.SetRowCount(4);
	Dom.SetColumnWidth(0, Dom.Always(20));
	Dom.SetColumnWidth(1, Dom.Always(30));
	Dom.SetRowHeight(0,Dom.Always(8));
	long i=0; for(i=1; i<4; i++)
		Dom.SetRowHeight(i, Dom.Always(15));
	long nAlign = GRID_ALIGN_CENTER | GRID_ALIGN_VCENTER;
	Dom.SetTextMatrix(0, 0, hggettext("��ȣ"), nAlign);
	Dom.SetTextMatrix(0, 1, hggettext("����"), nAlign);
	Dom.SetTextMatrix(1, 1, hggettext("������ SHOE")); 
	Dom.SetTextMatrix(3, 1, hggettext("����� SHOE"));
	Dom.SetTextMatrix(2, 1, hggettext("�Ϲ��� SHOE"));
	
	CConcSymbol sym(pStd->m_pOptStd);
	CDPoint vAngShoe(0,0);
	CDPoint xyShoe(Dom.Always(10), -Dom.Always(15));
	sym.DrawShoeSymbolPlan(&Dom, 0 ,xyShoe, vAngShoe, 0.6);
	xyShoe.y	= -Dom.Always(30);
	sym.DrawShoeSymbolPlan(&Dom, 2 ,xyShoe, vAngShoe, 0.6);
	xyShoe.y	= -Dom.Always(45.5);
	sym.DrawShoeSymbolPlan(&Dom, 3 ,xyShoe, vAngShoe, 0.6);
	
	Dom.Draw(FALSE);	
	Dom.DrawOutLine();

	*pDom <<Dom;	
}
void CDrawingOthers::GetDupGugan(long n, long* pGugan, long nType)
{
	CRcBridgeRebar *pBri = GetRcBridgeRebar();

	long nQtyHDan		= pBri->GetQtyHDan();
	CGuardWallRC* pCurrGW = pBri->GetGuardWallByHDan(n);
	double dCurrLen = pBri->GetGuardWallTotalLength(pCurrGW);
	
	long nCnt = 0;
	pGugan[nCnt++] = n;
	long i=0; for(i=0; i<nQtyHDan; i++)
	{
		if(i!=n)
		{
			CGuardWallRC* pGW = pBri->GetGuardWallByHDan(i); 
			double dLen = pBri->GetGuardWallTotalLength(pGW);
			if(!pGW) continue;
			if(pGW->m_nTypeGuardWall == nType)
			{
				if(*pCurrGW == *pGW && dCurrLen == dLen)
					pGugan[nCnt++] = i;
			}
		}
	}
}

void CDrawingOthers::AddPageHyunTa(CDomyun *pDomP, CDRect rcDogak, int nCategoryPage, int nJijum)
{
	CARcBridgeDrawingStd* pStd			= (CARcBridgeDrawingStd*)m_pStd;
	CRcBridgeRebar *pBri				= GetRcBridgeRebar();
	CARcBridgeDrawingOptionStd* pOpt	= pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData	= pOpt->GetDrawingOption(m_nBridge);
	CDrawPageMng* pPageMng				= pStd->GetPageManage();
	COutMovePoint* pMove				= pStd->GetMovePoint(pStd->m_nCurPage);
	CHgBaseDrawStd baseDraw;

	CARcBridgeDBStandard DBStd(pBri, pStd->m_pARoadOptionStd, pStd->m_pARcBridgeDataStd);
	DBStd.SetSelectFckDomText(pOpt->m_nSelectFckDomText);
	
	double dScaleFront = pData->m_dScaleHyunTaFront;
	double dScaleSection = pData->m_dScaleHyunTaSection;
	DWORD dOptDraw	= pData->m_dwDrawHyunTa;
	
	CDomyun Dom(pDomP);
	Dom.SetScaleDim(GetScale());

	double dSpaceHor = Dom.Always(30);	// ��ü ���� ����
	double dSpaceVer = Dom.Always(60);	// ��ü ���� ����
	
	CDRect rcFront(0, 0, 0, 0);			// ���鵵 ����
	CDRect rcSection[3];				// �ܸ鵵 ����
	double dDomX = 0;
	CString str	= _T(""), strSub = _T("");

	// ���鵵 //////////////////////////////////////////////////////////////////////
	if(dOptDraw & HYUNTA_FRONT)
	{
		if(m_pStd->IsDrawByBlock(MPE_REBAR_SLABFRONT))
		{
			str = _T("���鵵") + strSub;
			pPageMng->SetProgressData(str+_T(" �����..."), 20);
			AddFrontHyunTa(&Dom, nJijum);					// �ι�° ���� ������...(������ȣ �Է�)
			Dom.SetCalcExtRect();
			rcFront	= Dom.GetExtRect();
			
			dDomX = rcDogak.left-rcFront.left+GAP_LEFT*2;
			
			Dom.Move(dDomX, rcDogak.bottom-rcFront.bottom-dSpaceVer*2);
			Dom.SetCalcExtRect();
			rcFront	= Dom.GetExtRect();
			
			Dom.Move(pMove->GetPoint(60));
			pStd->ChangeSeperateLayer(&Dom);
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_SLABFRONT, str);
			*pDomP << Dom;
		}
			
		if(m_pStd->IsDrawByBlock(MPE_REBAR_SLABFRONT_TITLE))
		{
			pStd->AddTitleOnDom(&Dom, m_nBridge, str, CDPoint(0,0), 0, dScaleFront);
			Dom.Move(rcFront.left+rcFront.Width()/2.0, rcFront.bottom+GAP_TITLE);
			Dom.Move(pMove->GetPoint(90));
			pStd->ChangeSeperateLayer(&Dom);
			Dom.SetCalcExtRect();
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_SLABFRONT_TITLE, str+_T(" ����"));
			*pDomP << Dom;
		}
	}

	// �ܸ鵵 //////////////////////////////////////////////////////////////////////////
	CFootingApp *pFoot = pBri->GetFootingByJijumNumber(nJijum);
	CExPileApp *pPile = &pFoot->m_exFooting.m_Pile;
	DWORD dwSect[3] = {HYUNTA_SECT_A, HYUNTA_SECT_B, HYUNTA_SECT_C};
	
	int nCntSection = 0;
	CString strSect[] = {_T("A-A"), _T("B-B"), _T("C-C")};

	for( int i=0; i<pPile->m_pHyunTa->m_nHP_MainRebarGuganSu+1; i++)
	{
		if(dOptDraw & dwSect[i])
		{
			str = _T("�ܸ� ") + strSect[i];
			if(m_pStd->IsDrawByBlock(MPE_REBAR_SLABDETAILA + i))
			{
				pPageMng->SetProgressData(str+_T(" �����..."), 30 + i*10);
				
				AddSectionHyunTa(&Dom, nJijum, i);
				Dom.SetCalcExtRect();
				rcSection[i]	= Dom.GetExtRect();
				
				dDomX = rcDogak.left-rcSection[i].left+GAP_LEFT*6 + dSpaceHor*6*(nCntSection/2);
				
				Dom.Move(dDomX, rcDogak.bottom-rcSection[i].bottom-dSpaceVer*2 - dSpaceVer*3*(nCntSection%2));
				Dom.SetCalcExtRect();
				rcSection[i]	= Dom.GetExtRect();
				
				Dom.Move(pMove->GetPoint(60));
				pStd->ChangeSeperateLayer(&Dom);
				pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_SLABDETAILA + i, str);
				*pDomP << Dom;
			}
				
			if(m_pStd->IsDrawByBlock(MPE_REBAR_SLABDETAILA_TITLE + i))
			{
				pStd->AddTitleOnDom(&Dom, m_nBridge, str, CDPoint(0,0), 0, dScaleSection);
				Dom.Move(rcSection[i].left+rcSection[i].Width()/2.0, rcSection[i].bottom+GAP_TITLE);
				Dom.Move(pMove->GetPoint(90));
				pStd->ChangeSeperateLayer(&Dom);
				Dom.SetCalcExtRect();
				pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_SLABDETAILA_TITLE + i, str+_T(" ����"));
				*pDomP << Dom;
			}
			++nCntSection;
		}
	}

	// ö�ٿ������� ö�� ��
	if(dOptDraw & HYUNTA_DETAIL_HOLD)
	{
		CDRect rcHold;

		str = _T("ö�ٿ������� ö�� ��");
		if(m_pStd->IsDrawByBlock(MPE_REBAR_HOLDDETAIL))
		{
			pPageMng->SetProgressData(str+_T(" �����..."), 70);

			AddHyunTaRebarHoldDetail(&Dom, nJijum);
			Dom.SetCalcExtRect();
			rcHold	= Dom.GetExtRect();
			
			dDomX = rcDogak.left-rcHold.left+GAP_LEFT*6;

			Dom.Move(dDomX, -rcHold.top + Dom.Always(60));
			Dom.SetCalcExtRect();
			rcHold	= Dom.GetExtRect();
			
			Dom.Move(pMove->GetPoint(60));
			pStd->ChangeSeperateLayer(&Dom);
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_HOLDDETAIL, str);
			*pDomP << Dom;
		}
			
		if(m_pStd->IsDrawByBlock(MPE_REBAR_HOLDDETAIL_TITLE))
		{
			pStd->AddTitleOnDom(&Dom, m_nBridge, str, CDPoint(0,0), 0, dScaleSection);
			Dom.Move(rcHold.left+rcHold.Width()/2.0, rcHold.bottom+GAP_TITLE);
			Dom.Move(pMove->GetPoint(90));
			pStd->ChangeSeperateLayer(&Dom);
			Dom.SetCalcExtRect();
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_HOLDDETAIL_TITLE, str+_T(" ����"));
			*pDomP << Dom;
		}
	}

	// SPACER��
	if(dOptDraw & HYUNTA_DETAIL_SPAC)
	{
		CDRect rcSpacer;
		
		str = _T("SPACER ��");
		if(m_pStd->IsDrawByBlock(MPE_REBAR_SPACERDETAIL))
		{
			pPageMng->SetProgressData(str+_T(" �����..."), 70);
			
			AddHyunTaSpacerDetail(&Dom, nJijum);
			Dom.SetCalcExtRect();
			rcSpacer	= Dom.GetExtRect();
			
			dDomX = rcDogak.left-rcSpacer.left+GAP_LEFT*10;
			
			Dom.Move(dDomX, -rcSpacer.top + Dom.Always(60));
			Dom.SetCalcExtRect();
			rcSpacer	= Dom.GetExtRect();
			
			Dom.Move(pMove->GetPoint(60));
			pStd->ChangeSeperateLayer(&Dom);
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_SPACERDETAIL, str);
			*pDomP << Dom;
		}
			
		if(m_pStd->IsDrawByBlock(MPE_REBAR_SPACERDETAIL_TITLE))
		{
			pStd->AddTitleOnDom(&Dom, m_nBridge, str, CDPoint(0,0), 0, dScaleSection);
			Dom.Move(rcSpacer.left+rcSpacer.Width()/2.0, rcSpacer.bottom+GAP_TITLE);
			Dom.Move(pMove->GetPoint(90));
			pStd->ChangeSeperateLayer(&Dom);
			Dom.SetCalcExtRect();
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_SPACERDETAIL_TITLE, str+_T(" ����"));
			*pDomP << Dom;
		}
	}

	// NOTE
	if(dOptDraw & HYUNTA_NOTE)
	{
		CDRect rcNote;
		if(m_pStd->IsDrawByBlock(MPE_REBAR_SLABNOTE))
		{
			pPageMng->SetProgressData(_T("NOTE ��� ��..."), 80);
			AddHyunTaNote(&Dom);
			Dom.SetCalcExtRect();
			Dom.Move(Dom.Always(100), Dom.Always(80));
			Dom.Move(pMove->GetPoint(MPE_REBAR_SLABNOTE));
			pStd->ChangeSeperateLayer(&Dom);
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_SLABNOTE, _T("NOTE"));
			
			Dom.SetCalcExtRect();
			rcNote = Dom.GetExtRect();
			*pDomP << Dom;
		}
			
		if(m_pStd->IsDrawByBlock(MPE_REBAR_SLABNOTE_TITLE))
		{
			pStd->AddTitleOnDom(&Dom, m_nBridge, _T("NOTE"), CDPoint(0,0), 0, -1);
			Dom.SetCalcExtRect();
			Dom.Move(Dom.Always(100)+rcNote.Width()/2.0, Dom.Always(80)+rcNote.Height()+GAP_TITLE);
			Dom.Move(pMove->GetPoint(MPE_REBAR_SLABNOTE_TITLE));
			pStd->ChangeSeperateLayer(&Dom);
			Dom.SetCalcExtRect();
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_SLABNOTE_TITLE, _T("NOTE"));
			*pDomP << Dom;
		}
	}

	// ö�� ���ǥ
	// ��Ÿö�� �и� ����� ��쿡��..
	if(pData->m_bSeparateRebarBMOther)
	{
// 		 nCnt = 0;
		CDomyun Dom3(pDomP);
		CHgBaseConcStd baseConc;
		CDRect rcDetailRB(0, 0, 0, 0);
		CDPoint xyBase(0);

		CTypedPtrArray <CObArray, CRebar*> arrRebar;	// ���ǥ�� ö�ٸ���Ʈ
		

		MakeRebarListHyunTa(&arrRebar, nJijum);
		long nSize = arrRebar.GetSize();
		if(nSize>0)
		{
			if(pOpt->m_nTypeRebarTable == 0) //�Ϲ�Ÿ��
			{
				int nCnt(0);
				pPageMng->SetProgressData(_T("ö�ٻ� �����..."), 60);
				baseConc.ShellSortRebarList(&arrRebar, &CRebar::CompareForList);
				nCnt = DrawRebarDetail(&Dom3, &arrRebar, _T(""), CDRect(Dom.Always(640), Dom.Always(480), Dom.Always(640), Dom.Always(480)), nCnt);
				Dom3.SetCalcExtRect();
				rcDetailRB	= Dom3.GetExtRect();
				*pDomP << Dom3;
			}

			// ö�� ���ǥ
			baseConc.SetRebarIndexBySortRebarList(&arrRebar, TRUE);//���̺� ��ȣ ����
			baseConc.ShellSortRebarList(&arrRebar, &CRebar::CompareForTable);
			CDRect rcTableRB(0, 0, 0, 0);
			pPageMng->SetProgressData(_T("ö�����ǥ �����..."), 80);

			xyBase = DrawRebarTableHyunTa(&Dom3, &arrRebar, _T(""), CDPoint(Dom.Always(640), Dom.Always(500)));
			Dom3.SetCalcExtRect();
			rcDetailRB	= Dom3.GetExtRect();
			*pDomP << Dom3;

			if(pOpt->m_nTypeRebarTable == 1) //���鰣��ȭŸ��
			{
				pPageMng->SetProgressData("ö�ٻ� �����...", 80);
				baseConc.ShellSortRebarList(&arrRebar, &CRebar::CompareForList);
				baseDraw.DrawRebarListDetailSimple(&Dom3, &arrRebar, xyBase, pOpt->m_dWidthRebarTableRect, pOpt->m_dHeightRebarTableRect);

				Dom3.SetCalcExtRect();
				Dom3.Move(pMove->GetPoint(MPE_REBAR_MONUDETAIL));
				pStd->ChangeSeperateLayer(&Dom3);
				pMove->AddMPE(&Dom3, Dom3.GetExtRect(), MPE_REBAR_MONUDETAIL, hggettext("ö�ٻ� ����"));

				*pDomP << Dom3;
			}
			AhTPADelete(&arrRebar,	(CRebar*)0);

			long nIdxTable(1);
			DrawRebarDetailTable_RawRebar(pDomP, NULL, xyBase, nIdxTable, TRUE, nJijum);
		}
	}
}

void CDrawingOthers::AddFrontHyunTa(CDomyun* pDomP, int nJijum)
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;
	CARcBridgeDrawingOptionStd* pOpt = pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData = pOpt->GetDrawingOption(m_nBridge);
	CRcBridgeRebar *pBri = GetRcBridgeRebar();
	
	CARcBridgeDBStandard DBStd(pBri, pStd->m_pARoadOptionStd, pStd->m_pARcBridgeDataStd);
	DBStd.SetSelectFckDomText(pOpt->m_nSelectFckDomText);
	
	double dScaleFront = pData->m_dScaleHyunTaFront;
	CDPoint vAng = pBri->GetAngleJijum(nJijum);

	CFootingApp *pFoot = pBri->GetFootingByJijumNumber(nJijum);
	CExPileApp *pPile = &pFoot->m_exFooting.m_Pile;
	if(!pPile->m_bIs || pPile->m_nType != PILE_TYPE_HYUNJANG) return;

	CDrawExPile drawExPile(pPile, pStd->m_pARoadOptionStd);
	
	CDomyun Dom(pDomP);
	Dom.SetScaleDim(dScaleFront);
	
	pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);

	drawExPile.DrawFrontDetail_D(&Dom, pData->m_nTypeHyunTaCut);
	drawExPile.DrawFrontRebarMainDetail_D(&Dom, pData->m_nTypeHyunTaCut);
	drawExPile.DrawFrontRebarRoundDetail_D(&Dom, 0, pData->m_nTypeHyunTaCut);
	drawExPile.DimFrontMainRebar_D(&Dom, pData->m_nTypeHyunTaCut, FALSE, TRUE, FALSE);
	drawExPile.DimFrontRoundRebar_D(&Dom, 0, pData->m_nTypeHyunTaCut, FALSE, FALSE, FALSE);
	drawExPile.DimFrontBottom_D(&Dom);
	drawExPile.MarkFrontSectionPos_D(&Dom);
	
	Dom.RedrawByScale(GetScale()/dScaleFront);
	
	*pDomP << Dom;
}

void CDrawingOthers::AddSectionHyunTa(CDomyun* pDomP, int nJijum, long nGugan)
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;
	CARcBridgeDrawingOptionStd* pOpt = pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData = pOpt->GetDrawingOption(m_nBridge);
	CRcBridgeRebar *pBri = GetRcBridgeRebar();
	
	CARcBridgeDBStandard DBStd(pBri, pStd->m_pARoadOptionStd, pStd->m_pARcBridgeDataStd);
	DBStd.SetSelectFckDomText(pOpt->m_nSelectFckDomText);
	
	double dScaleSection = pData->m_dScaleHyunTaSection;
	CDPoint vAng = pBri->GetAngleJijum(nJijum);
	
	CFootingApp *pFoot = pBri->GetFootingByJijumNumber(nJijum);
	CExPileApp *pPile = &pFoot->m_exFooting.m_Pile;
	if(!pPile->m_bIs || pPile->m_nType != PILE_TYPE_HYUNJANG) return;

	CDrawExPile drawExPile(pPile, pStd->m_pARoadOptionStd);
	
	CDomyun Dom(pDomP);
	Dom.SetScaleDim(dScaleSection);
	long nDansu = (nGugan > 0)? 1:pPile->m_pHyunTa->m_nHP_MainRebarDanSu;

	double dDepth = 0;
	for(int j=0; j<=nGugan; j++)
	{
		if( j == pPile->m_pHyunTa->m_nHP_MainRebarGuganSu) dDepth = (pPile->m_dLength-dDepth)/2;
		else if( j == nGugan-1) dDepth += pPile->m_pHyunTa->m_dHP_MainRebarLen[j]/2;
		else dDepth += pPile->m_pHyunTa->m_dHP_MainRebarLen[j];
	}
	
	pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
	
	drawExPile.DrawPlanDetail(&Dom);
	drawExPile.DrawPlanRebarCrossTieDetail(&Dom, GetScale());
	drawExPile.DrawSectionPlanRebarMain_D(&Dom, dDepth, GetScale());		// ����° ������ dDepth ���.. 
	
	int i=0; for(i=0; i<nDansu; i++)
	{
		drawExPile.MarkRebarMainPlanDetail_D(&Dom, i, nGugan, FALSE);
		drawExPile.MarkRebarRoundPlanDetail_D(&Dom, i, FALSE, GetScale(), nGugan);
	}
	drawExPile.MarkRebarCrossTiePlanDetail_D(&Dom, 0,FALSE);
	drawExPile.DimRebarCrossTiePlan_D(&Dom, FALSE);
	
	Dom.RedrawByScale(GetScale()/dScaleSection);
	
	*pDomP << Dom;
}

void CDrawingOthers::MakeRebarList(CGuardWallRC *pGW, CTypedPtrArray <CObArray, CRebar*> *pArrRebar)
{
	AhTPADelete(pArrRebar,	(CRebar*)0);

	// ��ü�� ���� ǥ���Ҷ��� ��ȣ������ ���� ö�� ���踦 ���� ���� modified by jaeho 2005.08.22

	long nSize	= pGW->m_pArrRebar.GetSize();

	long i=0; for(i=0; i<nSize; i++)
	{
		CRebar *pRB	= pGW->m_pArrRebar.GetAt(i);
		if(pRB->IsTrue())
		{
			CRebar *pRBTmp = new CRebar;
			*pRBTmp	= *pRB;
			pArrRebar->Add(pRBTmp);
		}
	}
}

void CDrawingOthers::MakeRebarList(CTypedPtrArray <CObArray, CRebar*> *pArrRebar)
{
	CRcBridgeRebar *pBri = GetRcBridgeRebar();

	AhTPADelete(pArrRebar,	(CRebar*)0);
	
	long nSize	= pBri->m_pArrRebarMonument.GetSize();

	for(long j=0; j<nSize; j++)
	{
		CRebar *pRB	= pBri->m_pArrRebarMonument.GetAt(j);
		if(pRB->IsTrue())
		{
			CRebar *pRBTmp = new CRebar;
			*pRBTmp	= *pRB;
			pArrRebar->Add(pRBTmp);
		}
	}	
}

long CDrawingOthers::DrawRebarDetail(CDomyun *pDomP, CTypedPtrArray <CObArray, CRebar*> *pArrRebar, CString str, CDRect rt, long nCnt, BOOL bOthers, BOOL bMonu)
{
	CARcBridgeDrawingStd* pStd			= (CARcBridgeDrawingStd*)m_pStd;
	CARcBridgeDrawingOptionStd* pOpt	= pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData	= pOpt->GetDrawingOption(m_nBridge);
	CDrawPageMng* pPageMng				= pStd->GetPageManage();
	COutMovePoint* pMove				= pStd->GetMovePoint(pStd->m_nCurPage);
	CHgBaseDrawStd HGDraw;
	CConcSymbol symDB(pStd->m_pARoadOptionStd);
	CDomyun Dom(pDomP);
	
	double dSpace = pData->m_dSpaceBetweenRebarDetail;
	double dMaxY  = bOthers ? Dom.Always(320) : Dom.Always(480);
	double dMinY  = Dom.Always(60);
	double dYPlus = 0;
	CDPoint xy = bOthers ? CDPoint(rt.CenterPoint().x,dMaxY) : CDPoint(rt.right+Dom.Always(300),rt.bottom+Dom.Always(10));
	CRebar *pRB;

	if(m_pStd->IsDrawByBlock(bMonu ? MPE_REBAR_MONUDETAIL_TITLE : MPE_REBAR_SLABDETAIL_TITLE+nCnt))
	{
		pStd->m_pARoadOptionStd->SetEnvType(&Dom,HCAD_SUB_TITLE);
		Dom.TextOut(xy.x,xy.y+Dom.Always(10), str + _T(" ö�ٻ�"));
		Dom.Move(pMove->GetPoint(bMonu ? MPE_REBAR_MONUDETAIL_TITLE : MPE_REBAR_SLABDETAIL_TITLE+nCnt));
		pStd->ChangeSeperateLayer(&Dom);
		Dom.SetCalcExtRect();
		pMove->AddMPE(&Dom, Dom.GetExtRect(), bMonu ? MPE_REBAR_MONUDETAIL_TITLE : MPE_REBAR_SLABDETAIL_TITLE+nCnt, _T("ö�ٻ� ����"));
		*pDomP << Dom;
	}

	pStd->m_pARoadOptionStd->SetEnvType(&Dom,HCAD_DIML);

	long nSize  = pArrRebar->GetSize();
	long nRow   = 0;
	long nType  = 0;
	long nDetailCol = bMonu ? 0 : nCnt;
	BOOL bJoint = FALSE;
	xy.x -= Dom.Always(50);
	long i=0; for(i=0; i<nSize; i++)
	{
		pRB		= pArrRebar->GetAt(i);
		if(pRB->m_nTypeRebar!=nType || bJoint!=(pRB->m_nJoint>0))
		{
			nRow += 2;
			nType	= pRB->m_nTypeRebar;
			bJoint	= (pRB->m_nJoint>0) ? TRUE : FALSE;
			if((dMaxY-dSpace*nRow) < dMinY)
			{
				if(m_pStd->IsDrawByBlock(bMonu ? MPE_REBAR_MONUDETAIL+nDetailCol : MPE_REBAR_SLABDETAIL+nDetailCol*1))
				{
					Dom.Move(pMove->GetPoint(bMonu ? MPE_REBAR_MONUDETAIL+nDetailCol : MPE_REBAR_SLABDETAIL+nDetailCol*1));
					pStd->ChangeSeperateLayer(&Dom);
					Dom.SetCalcExtRect();
					pMove->AddMPE(&Dom, Dom.GetExtRect(), bMonu ? MPE_REBAR_MONUDETAIL+nDetailCol : MPE_REBAR_SLABDETAIL+nDetailCol*1, "ö�ٻ� "+COMMA(1+nDetailCol));
					*pDomP << Dom;
				}
				nDetailCol++;
				xy.x += Dom.Always(120);
				xy.y  = dMaxY;
				nRow  = 3;
			}

			CDPoint xyDrawPoint = CDPoint(xy.x+Dom.Always(50),xy.y+Dom.Always(10)+dYPlus);
			nRow = HGDraw.GetPlusCntRowDrawSymbolHeight(&Dom, pStd->m_pARoadOptionStd, nRow, nType, dSpace,  i, bJoint, xyDrawPoint);

			xy.y   = dMaxY - dSpace*nRow;
			dYPlus = (nType==1) ? Dom.Always(8) : 0;

			symDB.RebarDetailBasic(&Dom,nType,CDPoint(xy.x+Dom.Always(50),xy.y+Dom.Always(10)+dYPlus),bJoint);
		}
		else
			xy.y = dMaxY - dSpace*nRow;
		if(xy.y < dMinY)
		{
			if(m_pStd->IsDrawByBlock(bMonu ? MPE_REBAR_MONUDETAIL+nDetailCol : MPE_REBAR_SLABDETAIL+nDetailCol*1))
			{
				Dom.Move(pMove->GetPoint(bMonu ? MPE_REBAR_MONUDETAIL+nDetailCol : MPE_REBAR_SLABDETAIL+nDetailCol*1));
				pStd->ChangeSeperateLayer(&Dom);
				Dom.SetCalcExtRect();
				pMove->AddMPE(&Dom, Dom.GetExtRect(), bMonu ? MPE_REBAR_MONUDETAIL+nDetailCol : MPE_REBAR_SLABDETAIL+nDetailCol*1, "ö�ٻ� "+COMMA(1+nDetailCol));
				*pDomP << Dom;
			}
			nDetailCol++;
			xy.x += Dom.Always(120);
			xy.y  = dMaxY;
			nRow  = 0;
		}
		symDB.DrawJewonBar(&Dom,pRB,xy,pStd->m_pARcBridgeDataStd->m_pBasicConcInfo->m_nRoundRebarLength);
		nRow++;
	}

	if(m_pStd->IsDrawByBlock(bMonu ? MPE_REBAR_MONUDETAIL+nDetailCol : MPE_REBAR_SLABDETAIL+nDetailCol*1))
	{
		Dom.Move(pMove->GetPoint(bMonu ? MPE_REBAR_MONUDETAIL+nDetailCol : MPE_REBAR_SLABDETAIL+nDetailCol*1));
		pStd->ChangeSeperateLayer(&Dom);
		Dom.SetCalcExtRect();
		pMove->AddMPE(&Dom, Dom.GetExtRect(), bMonu ? MPE_REBAR_MONUDETAIL+nDetailCol : MPE_REBAR_SLABDETAIL+nDetailCol*1, "ö�ٻ� "+COMMA(1+nDetailCol));
		*pDomP << Dom;
	}

	return ++nDetailCol;
}

CDPoint CDrawingOthers::DrawRebarTable(CDomyun *pDomP, CTypedPtrArray <CObArray, CRebar*> *pArrRebar, CString str, CString sSubTitle, CDRect rt, long* pGugan, long nTableSu, BOOL bOthers, CGuardWallRC *pGW, BOOL bMonu)
{
	CARcBridgeDrawingStd* pStd			= (CARcBridgeDrawingStd*)m_pStd;
	CRcBridgeRebar *pBri				= GetRcBridgeRebar();
	CARcBridgeDrawingOptionStd* pOpt	= pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData	= pOpt->GetDrawingOption(m_nBridge);
	CDrawPageMng* pPageMng				= pStd->GetPageManage();
	COutMovePoint* pMove				= pStd->GetMovePoint(pStd->m_nCurPage);
	
	CGridDomyunEx Dom(pDomP);
	
	long nDefTableTitle	= bMonu ? MPE_REBAR_MONUTABLE_TITLE : MPE_REBAR_SLABTABLE_TITLE;
	
	CString sTitle = bOthers ? str + " ö�����ǥ" : "ö �� �� �� ǥ";
	if(sSubTitle != _T("")) sTitle += _T("(") + sSubTitle + _T(")");
	//double dMaxY  = Dom.Always(0);
	//CDPoint xyBase = bOthers ? CDPoint(rt.CenterPoint().x, dMaxY) :
	//						   CDPoint(rt.left+Dom.Always(620), rt.bottom+Dom.Always(10));
	CDPoint xyBase;
	xyBase.x = rt.left;
	xyBase.y = rt.top;

	long nTableType = pOpt->m_nTypeRebarTable ;
	if(nTableType == 0) //�⺻Ÿ���϶��� ���� ���
	{
		if(m_pStd->IsDrawByBlock(nDefTableTitle+nTableSu))
		{
			pStd->m_pARoadOptionStd->SetEnvType(&Dom,HCAD_SUB_TITLE);
			bOthers ? Dom.TextOut(xyBase.x+Dom.Always(15), xyBase.y+Dom.Always(10), sTitle)
				: Dom.TextOut(xyBase.x+Dom.Always(27), xyBase.y+Dom.Always(10), sTitle);
			Dom.Move(pMove->GetPoint(nDefTableTitle+nTableSu));
			pStd->ChangeSeperateLayer(&Dom);
			Dom.SetCalcExtRect();
			pMove->AddMPE(&Dom, Dom.GetExtRect(), nDefTableTitle+nTableSu, "ö�����ǥ ����");
			*pDomP << Dom;
		}
	}

	xyBase.x -= Dom.Always(50);
	double	dLengthSum		= 0;
	double	dLengthSumAdd	= 0;
	double	dLengthTotal	= 0;
	long	nMaxRow			= pData->m_nCountRowEachTable;
	long	nDefTable		= bMonu ? MPE_REBAR_MONUTABLE : MPE_REBAR_SLABTABLE;
	double	dWeightTotal	= 0;
	double	dWeightTotalAdd	= 0;

	CBasicConcInfo *pBCI = pStd->m_pARcBridgeDataStd->m_pBasicConcInfo;
	CHgBaseDrawStd baseDraw;
	double Fy = bOthers ? pGW->m_dFy : pBri->m_dMonuR_Fy;
	
	// ö�����ǥ �׸��� ������ ����
	if(pBri->m_pARcBridgeDataStd->m_nTypeUnit == 1)
		Fy = Fy * 10;

	BOOL bShadeOutLone = TRUE;//(pOpt->m_dDrawGen & GEBDRAW_GEN_TABLESHADE) ? TRUE : FALSE;
	
	// �ߺ�����
	CString szNoteBottom(_T(""));
	if(bOthers)
	{
		long   nQtyHDan = pBri->GetQtyHDan();
		double dGuardWallLength = 0;
		double dAddLength = 0;
		// �ߺ����� ������ ����
		if(0)//pGugan && pGugan[1]>-1)
		{
			szNoteBottom = _T("* ");
			long i=0; for(i=0; i<5; i++)
			{
				if(pGugan[i]>-1)
				{
					if(pGugan[i] == 0)
					{
						CGuardWallRC* pGW2 = pBri->GetGuardWallByHDan(pGugan[i]); 
						dGuardWallLength += toM(pBri->GetGuardWallTotalLength(pGW2));
						dAddLength += pBri->m_dAddLenghForGuardWall[0][1] + pBri->m_dAddLenghForGuardWall[1][0];
					}
					else if(pGugan[i] == nQtyHDan-1)
					{
						CGuardWallRC* pGW2 = pBri->GetGuardWallByHDan(pGugan[i]);
						dGuardWallLength += toM(pBri->GetGuardWallTotalLength(pGW2));
						dAddLength += pBri->m_dAddLenghForGuardWall[0][0] + pBri->m_dAddLenghForGuardWall[1][1];	
					}
				
					CString str(_T(""));
					str.Format(_T("%d,"), pGugan[i]+1);
					szNoteBottom += str;
				}
			}
			szNoteBottom.Delete(szNoteBottom.GetLength()-1,1);	// ������ ',' ����
			szNoteBottom += _T("������ ���� �հ�");
		}
		
		// ��ȣ�� ��������� �߰��Ÿ� ����.
		if(dGuardWallLength==0)
			dGuardWallLength = toM(pBri->GetGuardWallTotalLength(pGW));
		if(dAddLength==0)
		{
			if(pGW->m_nNumberHDan == 0)
				dAddLength = pBri->m_dAddLenghForGuardWall[0][1] + pBri->m_dAddLenghForGuardWall[1][0];
			else if(pGW->m_nNumberHDan == nQtyHDan-1)
				dAddLength = pBri->m_dAddLenghForGuardWall[0][0] + pBri->m_dAddLenghForGuardWall[1][1];	
		}
		
		CString szAddLength(_T(""));
		CString szComma = _T("* ");
		if(szNoteBottom!=_T(""))
			szComma = _T(" , ");
		if(dAddLength > 0)
			szAddLength.Format(_T("��ü����(L) = %.3fm(�߰��Ÿ�%.3fm����)"), dGuardWallLength, dAddLength);
		else
			szAddLength.Format(_T("��ü����(L) = %.3fm"), dGuardWallLength);
		szNoteBottom += szComma + szAddLength;
	}
	else		// ������ �϶�
		szNoteBottom.Format(_T("* %d���� ��ü��"), pBri->m_nMonumenu);
	
	long nIndex = 0;
	CDRect rtTable(0);
	while(1)
	{
		if(nIndex >= pArrRebar->GetSize()) break;
		if(m_pStd->IsDrawByBlock(nDefTable+nTableSu))
		{
			if(nTableType == 0) //�⺻Ÿ��
				nIndex = baseDraw.DrawRebarTable(&Dom, nIndex, nMaxRow, xyBase, dLengthSum, dLengthSumAdd, dLengthTotal, dWeightTotal, dWeightTotalAdd, *pArrRebar, Fy, bShadeOutLone, szNoteBottom, pBCI, pBCI->m_nRoundRebarLength);
			else //����ȭŸ��
				nIndex = baseDraw.DrawRebarTableSimple(&Dom, nIndex, nMaxRow, xyBase, dLengthSum, dLengthSumAdd, dLengthTotal, dWeightTotal, dWeightTotalAdd, *pArrRebar, Fy, bShadeOutLone, szNoteBottom, pBCI, pBCI->m_nRoundRebarLength);

			Dom.SetCalcExtRect();
			rtTable = Dom.GetExtRect();

			Dom.Move(pMove->GetPoint(nDefTable+nTableSu));
			pStd->ChangeSeperateLayer(&Dom);
			Dom.SetCalcExtRect();
			pMove->AddMPE(&Dom, Dom.GetExtRect(), nDefTable+nTableSu, "ö�����ǥ");
			*pDomP << Dom;
		}
		//xyBase.x += rtTable.Width() + Dom.Always(10);
		xyBase.y -= rtTable.Height() + Dom.Always(10);
	}	

	xyBase.x += Dom.Always(50);

	return xyBase;
}

BOOL CDrawingOthers::IsDuplicate(long n, long& nHDan, long nType)
{
	// ������ ö�������� ������ ���� �ʾƼ�
	// ������ �ߺ� ���� �ʵ��� ��.
	// 2007.08.07 .
	return FALSE;

	CRcBridgeRebar *pBri = GetRcBridgeRebar();

	long nQtyHDan		= pBri->GetQtyHDan();
	CGuardWallRC* pCurrGW = pBri->GetGuardWallByHDan(n);
	double dCurrLen = pBri->GetGuardWallTotalLength(pCurrGW);
	
	long i=0; for(i=0; i<nQtyHDan; i++)
	{
		if(i!=n)
		{
			CGuardWallRC* pGW = pBri->GetGuardWallByHDan(i); 
			double dLen = pBri->GetGuardWallTotalLength(pGW);
			if(!pGW) continue;
			if((pGW->m_nTypeGuardWall==nType) || (pCurrGW->m_bSoundProof && pGW->m_bSoundProof))
			{
				if(*pCurrGW == *pGW && dCurrLen == dLen)
				{
					nHDan = i;
					return TRUE;
				}
			}
		}
	}
	return FALSE;	// �ߺ��Ȱ��� ���ٸ�
}

void CDrawingOthers::MakeRebarListHyunTa(CTypedPtrArray <CObArray, CRebar*> *pArrRebar, int nJijum)
{
	AhTPADelete(pArrRebar,	(CRebar*)0);
	
	CRcBridgeRebar *pBri = GetRcBridgeRebar();
		CARcBridgeDrawingStd* pStd			= (CARcBridgeDrawingStd*)m_pStd;
	CARcBridgeDrawingOptionStd* pOpt	= pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData	= pOpt->GetDrawingOption(m_nBridge);

	CFootingApp *pFoot = pBri->GetFootingByJijumNumber(nJijum);
	CExPileApp *pPile = &pFoot->m_exFooting.m_Pile;
	if(!pPile->m_bIs || pPile->m_nType != PILE_TYPE_HYUNJANG) return;
	
	
	long nTotPile = pData->m_nTypeHyunTaVol == 0? pPile->m_pArrPile.GetSize() : 1;

	long nSize	= pPile->m_pHyunTa->m_pArrRebar.GetSize();
	
	long i=0; for(i=0; i<nSize; i++)
	{
		CRebar *pRB	= pPile->m_pHyunTa->m_pArrRebar.GetAt(i);
		if(pRB->IsTrue())
		{
			CRebar *pRBTmp = new CRebar;
			*pRBTmp	= *pRB;
			pRBTmp->m_dEa /= (double)nTotPile;
			pRBTmp->m_nEa /= nTotPile;
			pArrRebar->Add(pRBTmp);
		}
	}
}

CDPoint CDrawingOthers::DrawRebarTableHyunTa(CDomyun *pDomP, CTypedPtrArray <CObArray, CRebar*> *pArrRebar, CString sSubTitle, CDPoint xyBase)
{
	CARcBridgeDrawingStd* pStd			= (CARcBridgeDrawingStd*)m_pStd;
	CRcBridgeRebar *pBri				= GetRcBridgeRebar();
	CARcBridgeDrawingOptionStd* pOpt	= pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData	= pOpt->GetDrawingOption(m_nBridge);
	CDrawPageMng* pPageMng				= pStd->GetPageManage();
	COutMovePoint* pMove				= pStd->GetMovePoint(pStd->m_nCurPage);
	
	CGridDomyunEx Dom(pDomP);
	
	long nTableType = pOpt->m_nTypeRebarTable ;
	if(nTableType == 0) //�⺻Ÿ���϶��� ���� ���
	{
		long nDefTableTitle	= MPE_REBAR_SLABTABLE_TITLE;

		if(m_pStd->IsDrawByBlock(nDefTableTitle))
		{
			pStd->m_pARoadOptionStd->SetEnvType(&Dom,HCAD_SUB_TITLE);
			CString sTitle = "ö �� �� �� ǥ";
			if(sSubTitle != "") sTitle += "(" + sSubTitle + ")";
			Dom.TextOut(xyBase.x+Dom.Always(155/2),xyBase.y+Dom.Always(10), sTitle);
			Dom.Move(pMove->GetPoint(nDefTableTitle));
			pStd->ChangeSeperateLayer(&Dom);
			Dom.SetCalcExtRect();
			pMove->AddMPE(&Dom, Dom.GetExtRect(), nDefTableTitle, "ö�����ǥ ����");
			*pDomP << Dom;
		}
	}
	
	
	double dLengthSum		= 0;
	double dLengthSumAdd	= 0;
	double dLengthTotal		= 0;
	long nMaxRow			= pData->m_nCountRowEachTable;
	long nDefTable			= MPE_REBAR_SLABTABLE;
	double dWeightTotal		= 0;
	double dWeightTotalAdd	= 0;

	CBasicConcInfo *pBCI = pStd->m_pARcBridgeDataStd->m_pBasicConcInfo;
	CHgBaseDrawStd baseDraw;
	double Fy = pArrRebar->GetSize() > 0 ? pArrRebar->GetAt(0)->m_Fy : pBri->m_pARcBridgeDataStd->m_Fy;
	
	// ö�����ǥ �׸��� ������ ����
	if(pBri->m_pARcBridgeDataStd->m_nTypeUnit == 1)
		Fy = Fy * 10;
	
	BOOL bShadeOutLone = TRUE;//(pOpt->m_dDrawGen & GEBDRAW_GEN_TABLESHADE) ? TRUE : FALSE;
	
	CDRect rtTable(0);
	long nIndex = 0;
	long nTableSu = 0;
	while(1)
	{
		if(nIndex >= pArrRebar->GetSize()) break;
		if(m_pStd->IsDrawByBlock(nDefTable+nTableSu))
		{
			CString sNoteHyunTa = pData->m_nTypeHyunTaVol == 0? _T("���� 1���� ����") : _T("���� ��ü����");
			if(nTableType == 0) //�⺻Ÿ��
				nIndex = baseDraw.DrawRebarTable(&Dom, nIndex, nMaxRow, xyBase, dLengthSum, dLengthSumAdd, dLengthTotal, dWeightTotal, dWeightTotalAdd, *pArrRebar, Fy, bShadeOutLone, sNoteHyunTa, pBCI, pBCI->m_nRoundRebarLength);
			else //����ȭŸ��
				nIndex = baseDraw.DrawRebarTableSimple(&Dom, nIndex, nMaxRow, xyBase, dLengthSum, dLengthSumAdd, dLengthTotal, dWeightTotal, dWeightTotalAdd, *pArrRebar, Fy, bShadeOutLone, sNoteHyunTa, pBCI, pBCI->m_nRoundRebarLength);

			Dom.SetCalcExtRect();
			rtTable = Dom.GetExtRect();

			Dom.Move(pMove->GetPoint(nDefTable+nTableSu));
			pStd->ChangeSeperateLayer(&Dom);
			Dom.SetCalcExtRect();
			pMove->AddMPE(&Dom, Dom.GetExtRect(), nDefTable+nTableSu, "ö�����ǥ");
		}
		*pDomP << Dom;
		xyBase.x += rtTable.Width() + Dom.Always(10);
		nTableSu++;
	}		
	
	return xyBase;
}

void CDrawingOthers::AddHyunTaRebarHoldDetail(CDomyun* pDomP, int nJijum)
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;
	CARcBridgeDrawingOptionStd* pOpt = pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData = pOpt->GetDrawingOption(m_nBridge);
	CRcBridgeRebar *pBri = GetRcBridgeRebar();
	
	CARcBridgeDBStandard DBStd(pBri, pStd->m_pARoadOptionStd, pStd->m_pARcBridgeDataStd);
	DBStd.SetSelectFckDomText(pOpt->m_nSelectFckDomText);
	
	double dScaleSection = pData->m_dScaleHyunTaSection;
	CDPoint vAng = pBri->GetAngleJijum(nJijum);
	
	CFootingApp *pFoot = pBri->GetFootingByJijumNumber(nJijum);
	CExPileApp *pPile = &pFoot->m_exFooting.m_Pile;
	if(!pPile->m_bIs || pPile->m_nType != PILE_TYPE_HYUNJANG) return;
	
	CDrawExPile drawExPile(pPile, pStd->m_pARoadOptionStd);
	
	CDomyun Dom(pDomP);
	Dom.SetScaleDim(dScaleSection);
	
	drawExPile.DrawRebarholdDetail(&Dom, GetScale());
	
	Dom.RedrawByScale(GetScale()/dScaleSection);

	*pDomP << Dom;
}

void CDrawingOthers::AddHyunTaSpacerDetail(CDomyun* pDomP, int nJijum)
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;
	CARcBridgeDrawingOptionStd* pOpt = pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData = pOpt->GetDrawingOption(m_nBridge);
	CRcBridgeRebar *pBri = GetRcBridgeRebar();
	
	CARcBridgeDBStandard DBStd(pBri, pStd->m_pARoadOptionStd, pStd->m_pARcBridgeDataStd);
	DBStd.SetSelectFckDomText(pOpt->m_nSelectFckDomText);
	
	double dScaleSection = pData->m_dScaleHyunTaSection;
	CDPoint vAng = pBri->GetAngleJijum(nJijum);
	
	CFootingApp *pFoot = pBri->GetFootingByJijumNumber(nJijum);
	CExPileApp *pPile = &pFoot->m_exFooting.m_Pile;
	if(!pPile->m_bIs || pPile->m_nType != PILE_TYPE_HYUNJANG) return;
	
	CDrawExPile drawExPile(pPile, pStd->m_pARoadOptionStd);
	
	CDomyun Dom(pDomP);
	Dom.SetScaleDim(dScaleSection);
	
	drawExPile.DrawSpacerDetail(&Dom);
	
	Dom.RedrawByScale(GetScale()/dScaleSection);
	
	*pDomP << Dom;
}

void CDrawingOthers::AddHyunTaNote(CDomyun* pDomP)
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;
	
	CStringArray szArrNot;
	
	szArrNot.RemoveAll();
	
	szArrNot.Add("PILE�� ���̴� �ð��� ������ ���ǿ� ����");
	szArrNot.Add("���̰� ���� �� �����Ƿ� ���, �Ǵ� �Ϲ�");
	szArrNot.Add("�м��� ���Ͽ� ���̸� �����Ͽ��� �Ѵ�.");
	
	long nSize = szArrNot.GetSize();
	CGridDomyunEx Dom(pDomP);
	double dWidthTxt = Dom.GetTextWidth(szArrNot.GetAt(0), 0);
	pStd->m_pARoadOptionStd->SetEnvType(&Dom,HCAD_NOTE);
	
	Dom.SetBasePoint(CDPoint(0,0));
	Dom.SetGridLines(GVL_NONE);
	Dom.SetColumnCount(2);
	Dom.SetRowCount(nSize+2);
	Dom.SetColumnWidth(0, Dom.Always(10));
	Dom.SetColumnWidth(1, dWidthTxt + Dom.Always(10));
	Dom.SetRowHeightAll(Dom.GetTextHeight()*2.5);
	Dom.SetRowHeight(0,Dom.Always(5));
	Dom.SetRowHeight(nSize+1,Dom.Always(5));

	CString szNo = _T("");
	CString str = _T("");
	long i=0; for(i=1; i<nSize+1; i++)
	{
		str = szArrNot.GetAt(i-1);
		Dom.SetTextMatrix(i, 1, str, GRID_ALIGN_LEFT);
	}
	
	Dom.Draw(FALSE);
	Dom.DrawOutLine();
	Dom.DrawOutLineShade();
	pStd->ChangeSeperateLayer(&Dom);
	
	Dom.SetCalcExtRect();
	CDRect rc = Dom.GetExtRect();
	Dom.Move(-rc.left, -rc.top);
	
	*pDomP << Dom;
}


void CDrawingOthers::AddPageEHP(CDomyun *pDom, CDRect rcDogak, long nPageNum)
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;
	CDrawPageMng* pMng = pStd->GetPageManage();
	long nPage		= nPageNum;

	pMng->SetProgressData("EHP�������� �󼼵� �����..",	50);	

	CString strDxfName;
	CString szPath	= AfxGetApp()->GetProfileString("Afx Directory", "Fixed Directory");
	CString strFileName = m_arrEhpDxfName.GetAt(nPage);
	//CString strFileName	= nPageNum == 0 ? "EHP(1).dxf" : "EHP(2).dxf";
	strDxfName.Format("%s\\dxf\\%s",szPath,strFileName);

	CDomyun Dom(pDom);
	Dom.DxfIn(strDxfName);

	
	Dom.ZoomAll();	


	Dom.SetCalcExtRect();
	CDRect DetailRect	= Dom.GetExtRect();

	// ������ �°� ������ ����
	double dDetailScale = GetDetailScale(rcDogak, DetailRect);
	Dom.RedrawByScale(dDetailScale);
	Dom.SetCalcExtRect();
	DetailRect	= Dom.GetExtRect();

	// ������ �󼼵��� ��ġ ���
	CDPoint dpDogakOrg;
	CDPoint dpDetailOrg;
	CDPoint dpMove;
	
	double	dLeft	= 30 * GetScale(),
			dRight	= 30 * GetScale(),
			dTop	= 100 * GetScale(),
			dBottom	= 30 * GetScale();

	dpDogakOrg.x	= (rcDogak.left + dLeft) + ((rcDogak.Width() - (dLeft + dRight)) / 2);
	dpDogakOrg.y	= (rcDogak.top + dTop) + ((rcDogak.Height() - (dTop + dBottom)) / 2);
	dpDetailOrg.x	= DetailRect.left + (DetailRect.Width() / 2);
	dpDetailOrg.y	= DetailRect.top + (DetailRect.Height() / 2);

	dpMove.x		= dpDogakOrg.x - dpDetailOrg.x;
	dpMove.y		= dpDogakOrg.y - dpDetailOrg.y;
	dpMove.y -= Dom.Always(30);

	Dom.Move(dpMove.x, dpMove.y);


	*pDom << Dom;
}

void CDrawingOthers::SetArrEhpDXFName(long nBri)
{
	m_arrEhpDxfName.RemoveAll();

	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;
	CRcBridgeApp* pBri = pStd->m_pARcBridgeDataStd->GetBridge(nBri);
	if(!pBri)	return;

	CString sDxfName = _T("");
	// ����
	if(pBri->IsOutWall(TRUE) && pBri->m_footingStt.m_exFooting.m_nType == EXFOOTING_TYPE_PILE)
	{
		if(pBri->m_footingStt.m_exFooting.m_Pile.m_nType == PILE_TYPE_EHP)
		{
			sDxfName = pBri->GetEhpDxfNameByDia(pBri->m_footingStt.m_exFooting.m_Pile.m_nKind_EHP);
			if(sDxfName != "")
				m_arrEhpDxfName.Add(sDxfName);
		}
	}
	
	// ����
	long i = 0; for(i = 0; i < pBri->GetCountInWall(); i++)
	{
		CWallApp *pWall = pBri->GetInWall(i);
		if(!pWall)	continue;

		if(pWall->m_bIs && pWall->m_footing.m_exFooting.m_nType == EXFOOTING_TYPE_PILE)
		{
			if(pWall->m_footing.m_exFooting.m_Pile.m_nType == PILE_TYPE_EHP)
			{
				sDxfName = pBri->GetEhpDxfNameByDia(pWall->m_footing.m_exFooting.m_Pile.m_nKind_EHP);
				if(sDxfName != "")
					m_arrEhpDxfName.Add(sDxfName);
			}
		}
	}

	// ����
	if(pBri->IsOutWall(TRUE) && pBri->m_footingEnd.m_exFooting.m_nType == EXFOOTING_TYPE_PILE)
	{
		if(pBri->m_footingEnd.m_exFooting.m_Pile.m_nType == PILE_TYPE_EHP)
		{
			sDxfName = pBri->GetEhpDxfNameByDia(pBri->m_footingEnd.m_exFooting.m_Pile.m_nKind_EHP);
			if(sDxfName != "")
				m_arrEhpDxfName.Add(sDxfName);
		}
	}

	// �ߺ� DXF �ɷ�����
	CString sCurDxf, sDxf;
	for(i = 0; i < m_arrEhpDxfName.GetSize() - 1; i++)
	{
		sCurDxf	= m_arrEhpDxfName.GetAt(i);
		for(long j = i + 1; j < m_arrEhpDxfName.GetSize(); j++)
		{
			sDxf	= m_arrEhpDxfName.GetAt(j);
			if(sCurDxf == sDxf) 
			{
				m_arrEhpDxfName.RemoveAt(j);
				j--;
			}
		}
	}
}

double CDrawingOthers::GetDetailScale(CDRect DogakRect, CDRect DetailRect)
{
	// ������ħ �󼼵��� dxf�� �������� ���ϱⰡ ����ϴ�.
	// ������ ���� ������ ����ؼ� �������� ���ؾ� �ڴ�.
	double	dLeft	= 30 * GetScale(),
			dRight	= 30 * GetScale(),
			dTop	= 100 * GetScale(),
			dBottom	= 30 * GetScale();

	double dHorScale = (DogakRect.Width() - (dLeft + dRight)) / DetailRect.Width();
	double dVerScale = (DogakRect.Height() - (dTop + dBottom)) / DetailRect.Height();

	double dScale;
	if(dHorScale < dVerScale)	dScale	= dHorScale;
	else						dScale	= dVerScale;

	return dScale;
}

void CDrawingOthers::DrawRebarDetailTable_RawRebar( CDomyun *pDom, CGuardWallRC *pGW, CDPoint &xyBase, long &nIdxTable, BOOL bHyunTa, long nJijum )
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;
	CRcBridgeRebar *pBri				= GetRcBridgeRebar();
	CARcBridgeDrawingOptionStd* pOpt = pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pOptData = pOpt->GetDrawingOption(m_nBridge);

	if(pBri->m_pARcBridgeDataStd->m_pBasicConcInfo->IsApplyExtraChargeAtRebarJoint() == FALSE && pOpt->m_bAddRawRebarTable)
	{
		xyBase.y -= pDom->Always(10);
		CTypedPtrArray <CObArray, CRebar*> arrRebar;
		CTypedPtrArray <CObArray, CRebar*> arrRawRebar;

		CHgBaseConcStd baseConc;

		CString str(_T(""));
		if(bHyunTa)
		{
			MakeRebarListHyunTa(&arrRebar, nJijum);
		}
		else if(pGW == NULL)
		{
			MakeRebarList(&arrRebar);
		}
		else
		{
			MakeRebarList(pGW, &arrRebar);
			str = pBri->GetGuardWallName(pGW);
		}

		long nCountRaw = baseConc.GetRawRebarList(&arrRebar, &arrRawRebar, pBri->m_pARcBridgeDataStd->m_pBasicConcInfo->GetLenghtRebarMax());
		if(nCountRaw > 0)
		{
			if(pOptData->m_bRebarTableSepByFy)
			{
				double dFy	= 1;
				while(dFy != 0)
				{
					CTypedPtrArray <CObArray, CRebar*> arrRebarGet;
					dFy	= pStd->GetRebarListByFy(&arrRawRebar, &arrRebarGet, dFy);
					if(dFy == 0) break;
					if(arrRebarGet.GetSize() == 0) break;

					baseConc.ShellSortRebarList(&arrRebarGet, &CRebar::CompareForTable);

					if(bHyunTa)
						xyBase = DrawRebarTableHyunTa(pDom, &arrRebarGet, _T("�̰��� ö��"), xyBase);
					else
						xyBase = DrawRebarTable(pDom, &arrRebarGet, str, _T("�̰��� ö��"), CDRect(xyBase.x, xyBase.y, 0, 0), NULL, nIdxTable++, pGW!=NULL, pGW, pGW==NULL);
				}
			}
			else
			{
				baseConc.ShellSortRebarList(&arrRawRebar, &CRebar::CompareForTable);
				if(bHyunTa)
					xyBase = DrawRebarTableHyunTa(pDom, &arrRebar, _T("�̰��� ö��"), xyBase);
				else
					xyBase = DrawRebarTable(pDom, &arrRebar, str, _T("�̰��� ö��"), CDRect(xyBase.x, xyBase.y, 0, 0), NULL, nIdxTable++, pGW!=NULL, pGW, pGW==NULL);
			}
		}

		AhTPADelete(&arrRawRebar, (CRebar*)0);
		AhTPADelete(&arrRebar,	(CRebar*)0);
	}
}
