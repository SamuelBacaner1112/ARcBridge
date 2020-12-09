// DrawingCns.cpp: implementation of the CDrawingCns class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../../Src/ARcBridgeData/ARcBridgeData.h"
#include "../../Src/ARcBridgeDBDraw/ARcBridgeDBDraw.h"
#include "../ARcBridgeDrawingOption/ARcBridgeDrawingOption.h"
#include "ARcBridgeDrawing.h"
#include "DrawingCns.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define MPE_REBAR_SLABPLANE				10		// ������ ��� ������
#define MPE_REBAR_SLABPLANE_TITLE		20		// ������ ��� ������ ����
#define MPE_REBAR_SLABFRONT				30		// ������ ���� ������
#define MPE_REBAR_SLABFRONT_TITLE		40		// ������ ���� ������ ����
#define MPE_REBAR_SLABSIDE				50		// ������ ���� ������
#define MPE_REBAR_SLABSIDE_TITLE		60		// ������ ���� ������ ����
#define MPE_REBAR_SLABDETAIL			70		// ������ ö�ٻ�
#define MPE_REBAR_SLABDETAIL_TITLE		80		// ������ ö�ٻ� ����
#define MPE_REBAR_SLABDETAILA			90		// ������ �� A
#define MPE_REBAR_SLABDETAILA_TITLE		100		// ������ �� A ����
#define MPE_REBAR_SLABTABLEM			110		// ������ ���ǥ
#define MPE_REBAR_SLABTABLEM_TITLE		120		// ������ ���ǥ ����
#define MPE_REBAR_SLABTABLE				130		// ������ ö�� ���ǥ
#define MPE_REBAR_SLABTABLE_TITLE		140		// ������ ö�� ���ǥ ����
#define MPE_REBAR_SLABNOTE				150		// ������ �⺻��Ʈ
#define MPE_REBAR_SLABNOTE_TITLE		160		// ������ �⺻ ��Ʈ ����

CDrawingCns::CDrawingCns()
{
	m_nBridge = -1;
}

CDrawingCns::~CDrawingCns()
{

}

long CDrawingCns::GetPageSu()
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;

	if(pStd->GetCountTotalPage()<=0)
		pStd->MakePageInfo();

	m_nPage	= pStd->GetCountCategoryPage(_T("���� ������"));

	return m_nPage;
}

CString CDrawingCns::GetTitle()
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;
	CPageInfo* pInfo = pStd->GetPageInfoOrg(_T("���� ������"), pStd->m_nCurPage-m_nSttPage);
	if(pInfo==NULL) return _T("");

	CString strTitle = pStd->GetStringTitle(pStd->m_pARcBridgeDataStd->GetBridge(m_nBridge), pInfo->m_strTitle);

	return strTitle;
}

double CDrawingCns::GetScale()
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;
	CARcBridgeDrawingOptionStd* pOpt = pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pOptData = pOpt->GetDrawingOption(m_nBridge);

	m_Scale	= pOptData->m_dScaleRebarAps;
	return pOptData->m_dScaleRebarAps;
}

void CDrawingCns::SetNumber(long nSubPage)
{
	m_nBridge = GetNumberBridge(nSubPage-m_nSttPage);
}

long CDrawingCns::GetNumberBridge(long nCategoryPageNum)
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;
	CPageInfo* pInfo = pStd->GetPageInfoOrg(_T("���� ������"), nCategoryPageNum);
	if(pInfo==NULL) return -1;

	return pInfo->m_nBri;
}

void CDrawingCns::DrawOnePage()
{
	TRACE("CDrawingCns");
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;
	CARcBridgeDrawingOptionStd* pOpt = pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDataStd* pDataStd = pStd->m_pARcBridgeDataStd;
	CDrawDogak* pDgk = pStd->GetDogak();	
	CDrawPageMng* pMng = pStd->GetPageManage();

	long nPage = pStd->m_nCurPage;

	SetNumber(pStd->m_nDrawPage);
	CARcBridgeDrawingOptionData* pOptData = pOpt->GetDrawingOption(m_nBridge);
	if(!pOptData) return;

	double dScale = GetScale();

	CDomyun* pDom = pStd->GetDomyun(nPage);
	pDom->SetScaleDim(dScale);

	CPageInfo* pInfo = pStd->GetPageInfoOrg(_T("���� ������"), pStd->m_nDrawPage-m_nSttPage);
	if(pInfo==NULL) return;

	nPage = pStd->GetPageNum(pInfo);

	pStd->m_pOptStd->SetEnvType(pDom, HCAD_MIS3);
	pStd->m_pOptStd->SetEnvType(pDom, HCAD_SYMB);

	CDomyun Dom(pDom);

	pMng->SetProgressData(_T("���� ��� ��..."), 0);

	long nSttPage = 0;
	CString strDgkName = pOpt->m_strDgkName;
	CString strHeadNote = pOpt->m_strDomNumHead;
	CString strSmallTitle = GetTitle();
	CString strXRefPath = IsDxfOut() ? pOpt->m_strXRefPath : _T("");

	CRcBridgeApp* pBri = pDataStd->GetBridge(m_nBridge);
	pStd->m_pARcBridgeDataStd->m_strTempName = pBri->m_strBridgeName;

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

	// ���� ������ ���
	long nPageNum	= pInfo->m_nPageNumber-1;	// �ε���
	BOOL bMerge12	= pOptData->m_nTypeCnsRebar == 0 ? TRUE : FALSE;	// ���潽���� 1,2 ����ǥ�� ����

	BOOL bDraw		= FALSE;

	long nCountCnsSttLeft	= pBri->IsCns(TRUE, TRUE, 0) ? 1 : 0;
	long nCountCnsSttLeft1	= pBri->IsCns(TRUE, TRUE, 1) ? 1 : 0;
	long nCountCnsSttRight	= pBri->IsCns(TRUE, FALSE, 0) ? 1 : 0;
	long nCountCnsSttRight1	= pBri->IsCns(TRUE, FALSE, 1) ? 1 : 0;
	long nCountCnsEndLeft	= pBri->IsCns(FALSE, TRUE, 0) ? 1 : 0;
	long nCountCnsEndLeft1	= pBri->IsCns(FALSE, TRUE, 1) ? 1 : 0;
	long nCountCnsEndRight	= pBri->IsCns(FALSE, FALSE, 0) ? 1 : 0;
	long nCountCnsEndRight1	= pBri->IsCns(FALSE, FALSE, 1) ? 1 : 0;

	if(bMerge12)
	{
		nCountCnsSttLeft1	= 0;
		nCountCnsSttRight1	= 0;
		nCountCnsEndLeft1	= 0;
		nCountCnsEndRight1	= 0;
	}
	
	// ����, ���� ���潽����1
	if(nPageNum < nCountCnsSttLeft && !bDraw)
	{
		AddPageCns(&Dom, TRUE, TRUE, bMerge12 ? -1 : 0, rcDogak);
		bDraw	= TRUE;
	}
	else
		nPageNum -= nCountCnsSttLeft;

	// ����, ���� ���潽����2
	if(nPageNum < nCountCnsSttLeft1 && !bDraw)
	{
		AddPageCns(&Dom, TRUE, TRUE, bMerge12 ? -1 : 1, rcDogak);
		bDraw	= TRUE;
	}
	else
		nPageNum -= nCountCnsSttLeft1;

	// ����, ���� ���潽����1
	if(nPageNum < nCountCnsSttRight && !bDraw)
	{
		AddPageCns(&Dom, TRUE, FALSE, bMerge12 ? -1 : 0, rcDogak);
		bDraw	= TRUE;
	}
	else
		nPageNum -= nCountCnsSttRight;

	// ����, ���� ���潽����2
	if(nPageNum < nCountCnsSttRight1 && !bDraw)
	{
		AddPageCns(&Dom, TRUE, FALSE, bMerge12 ? -1 : 1, rcDogak);
		bDraw	= TRUE;
	}
	else
		nPageNum -= nCountCnsSttRight1;

	// ����, ���� ���潽����1
	if(nPageNum < nCountCnsEndLeft && !bDraw)
	{
		AddPageCns(&Dom, FALSE, TRUE, bMerge12 ? -1 : 0, rcDogak);
		bDraw	= TRUE;
	}
	else
		nPageNum -= nCountCnsEndLeft;

	// ����, ���� ���潽����2
	if(nPageNum < nCountCnsEndLeft1 && !bDraw)
	{
		AddPageCns(&Dom, FALSE, TRUE, bMerge12 ? -1 : 1, rcDogak);
		bDraw	= TRUE;
	}
	else
		nPageNum -= nCountCnsEndLeft1;

	// ����, ���� ���潽����1
	if(nPageNum < nCountCnsEndRight && !bDraw)
	{
		AddPageCns(&Dom, FALSE, FALSE, bMerge12 ? -1 : 0, rcDogak);
		bDraw	= TRUE;
	}
	else
		nPageNum -= nCountCnsEndRight;

	// ����, ���� ���潽����2
	if(nPageNum < nCountCnsEndRight1 && !bDraw)
	{
		AddPageCns(&Dom, FALSE, FALSE, bMerge12 ? -1 : 1, rcDogak);
// 		bDraw	= TRUE;
	}
// 	else
// 		nPageNum -= nCountCnsEndRight1;


	pMng->SetProgressData(_T("��� �Ϸ�  ..."), 100);

	*pDom << Dom;
}

void CDrawingCns::AddPageOne(CDomyun* pDomP, long nSubPage)
{

}

// ���ӽ����� ���� �߰�
// nIdx �� -1�̸� ���潽���� 1, 2�� �Բ� �׸�.
void CDrawingCns::AddPageCns(CDomyun *pDomP, BOOL bStt, BOOL bLeft, long nIdx, CDRect rcDogak)
{
	CARcBridgeDrawingStd* pStd			= (CARcBridgeDrawingStd*)m_pStd;
	CRcBridgeRebar *pBri				= GetRcBridgeRebar();
	CARcBridgeDrawingOptionStd* pOpt	= pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData	= pOpt->GetDrawingOption(m_nBridge);
	CDrawPageMng* pPageMng				= pStd->GetPageManage();
	COutMovePoint* pMove				= pStd->GetMovePoint(pStd->m_nCurPage);

	DWORD dwOptDraw = pData->m_dwDrawRebarCns;

	long nIdxCns	= nIdx == -1 ? 0 : nIdx;
	long nIdxCns1	= nIdx == -1 ? 1 : nIdx;
	long left		= bLeft ? iLEFT : iRIGHT;
	CApsApp *pCns	= bStt ? &pBri->m_cnsStt[left][nIdxCns] : &pBri->m_cnsEnd[left][nIdxCns];
	CApsApp *pCns1	= bStt ? &pBri->m_cnsStt[left][nIdxCns1] : &pBri->m_cnsEnd[left][nIdxCns1];
	if(!pCns->m_bIs) return;
	
	CARcBridgeDBStandard DBStd(pBri, pStd->m_pARoadOptionStd, pStd->m_pARcBridgeDataStd);
	DBStd.SetSelectFckDomText(pOpt->m_nSelectFckDomText);
	double dScale	= GetScale();

	CDomyun Dom(pDomP);
	Dom.SetScaleDim(dScale);
	
	BOOL bDrawCns2	= nIdx == -1 && nIdxCns != nIdxCns1;	// ���潽���긦 2�� �Ѳ����� �׸���?
	if(!pCns1->m_bIs) bDrawCns2	= FALSE;					// ���潽���갡 �ϳ��� ������ ������ �Ѱ��� �׸�.(�翬�� ����)
	CString str		= _T("");
	CString sCns	= bStt ? "������ ���ӽ�����" : "������ ���ӽ�����";
	sCns			+= bLeft ? "(����)" : "(����)";
	CString sNum	= nIdx == -1 ? "1, 2" : "";
	if(nIdx != -1) sNum.Format("%d", nIdx+1);
	sCns			+= sNum;


	long nCountDom	= 3;
	if(nIdx == -1 && nIdxCns != nIdxCns1)
		nCountDom++;
	double dAddPer	= 100 / nCountDom;
	double dCurPer	= 0;

	CDPoint xyMove(0, 0);
	// ��鵵 //////////////////////////////////////////////////////////////////////
	CDRect rcPlane(0, 0, 0, 0);	// ��鵵 ����
	pPageMng->SetProgressData(_T("��鵵 �����..."), (int)(dCurPer+=dAddPer));
	if(dwOptDraw & REBAR_CNS_PLANE)
	{
		if(m_pStd->IsDrawByBlock(MPE_REBAR_SLABPLANE))
		{
			rcPlane	= DBStd.m_pDrawAps->DrawRebar_PlaneStd(&Dom, pCns, FALSE);
			if(bDrawCns2)
			{
				CDomyun domCns1(&Dom);
				CDRect rcPlane1	= DBStd.m_pDrawAps->DrawRebar_PlaneStd(&domCns1, pCns1, FALSE);

				CDPoint xyMove1(0, 0);
				if(bStt)
					xyMove	= CDPoint(rcPlane.left - rcPlane1.right - Dom.Always(100), rcPlane.top - rcPlane1.top);
				else
					xyMove	= CDPoint(rcPlane.right - rcPlane1.left + Dom.Always(100), rcPlane.top - rcPlane1.top);

				domCns1.Move(xyMove);
				rcPlane1.left	+= xyMove.x; rcPlane1.right		+= xyMove.x;
				rcPlane1.top	+= xyMove.y; rcPlane1.bottom	+= xyMove.y;
				rcPlane			= DBStd.MergeRectAndRect(rcPlane, rcPlane1);

				Dom << domCns1;
			}
			xyMove	= CDPoint(rcDogak.left-rcPlane.left+Dom.Always(250), rcDogak.bottom-rcPlane.bottom-Dom.Always(150));
			
			Dom.Move(xyMove);
			rcPlane.left	+= xyMove.x; rcPlane.right	+= xyMove.x; 
			rcPlane.top		+= xyMove.y; rcPlane.bottom	+= xyMove.y;

			Dom.Move(pMove->GetPoint(MPE_REBAR_SLABPLANE));
			pStd->ChangeSeperateLayer(&Dom);
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_SLABPLANE, _T("��鵵"));
			*pDomP << Dom;
		}

		if(m_pStd->IsDrawByBlock(MPE_REBAR_SLABPLANE_TITLE))
		{
			pStd->AddTitleOnDom(&Dom, m_nBridge, _T("��鵵"), CDPoint(0,0), 0, dScale);
			Dom.Move(rcPlane.left+rcPlane.Width()/2.0, rcPlane.bottom+Dom.Always(50));
			Dom.Move(pMove->GetPoint(MPE_REBAR_SLABPLANE_TITLE));
			pStd->ChangeSeperateLayer(&Dom);
			Dom.SetCalcExtRect();
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_SLABPLANE_TITLE, sCns+_T("��鵵 Ÿ��Ʋ"));
			*pDomP << Dom;
		}
	}
	else
	{
		rcPlane.left	= rcDogak.left + Dom.Always(250);
		rcPlane.bottom	= rcDogak.bottom - Dom.Always(150);
		rcPlane.right	= rcPlane.left;
		rcPlane.top		= rcPlane.bottom;
	}
	////////////////////////////////////////////////////////////////////////////////////
	

	// ���鵵(A-A) //////////////////////////////////////////////////////////////////////
	CDRect rcFront(0, 0, 0, 0);	// ���鵵(A-A) ����
	pPageMng->SetProgressData(_T("�ܸ� A-A �����..."), (int)(dCurPer+=dAddPer));
	if(dwOptDraw & REBAR_CNS_FRONT)
	{
		if(m_pStd->IsDrawByBlock(MPE_REBAR_SLABFRONT))
		{
			rcFront	= DBStd.m_pDrawAps->DrawRebar_FrontStd(&Dom, pCns, FALSE);
			xyMove	= CDPoint(rcPlane.left-rcFront.right-Dom.Always(100), rcPlane.bottom-rcFront.bottom);
			Dom.Move(xyMove);
			rcFront.left	+= xyMove.x; rcFront.right	+= xyMove.x;
			rcFront.top		+= xyMove.y; rcFront.bottom	+= xyMove.y;
			
			Dom.Move(pMove->GetPoint(MPE_REBAR_SLABFRONT));
			Dom.SetCalcExtRect();
			pStd->ChangeSeperateLayer(&Dom);
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_SLABFRONT, _T("�ܸ� A-A"));
			*pDomP << Dom;
		}

		if(m_pStd->IsDrawByBlock(MPE_REBAR_SLABFRONT_TITLE))
		{
			pStd->AddTitleOnDom(&Dom, m_nBridge, _T("�ܸ� A-A"), CDPoint(0,0), 0, dScale);
			Dom.Move(rcFront.left+rcFront.Width()/2.0, rcFront.bottom+Dom.Always(50));
			Dom.Move(pMove->GetPoint(MPE_REBAR_SLABFRONT_TITLE));
			pStd->ChangeSeperateLayer(&Dom);
			Dom.SetCalcExtRect();
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_SLABFRONT_TITLE, _T("�ܸ� A-A Ÿ��Ʋ"));
			*pDomP << Dom;
		}
	}
	else
	{
		rcFront.left	= rcPlane.left + Dom.Always(100);
		rcFront.bottom	= rcPlane.bottom;
		rcFront.right	= rcFront.left;
		rcFront.top		= rcFront.bottom;
	}
	////////////////////////////////////////////////////////////////////////////////////

	// ���鵵(B-B) //////////////////////////////////////////////////////////////////////
	CDRect rcSide(0, 0, 0, 0);	// ���鵵(B-B) ����
	if(dwOptDraw & REBAR_CNS_SIDE)
	{
		if(m_pStd->IsDrawByBlock(MPE_REBAR_SLABSIDE))
		{
			pPageMng->SetProgressData(_T("�ܸ� B-B �����..."), (int)(dCurPer+=dAddPer));
			rcSide	= DBStd.m_pDrawAps->DrawRebar_SideStd(&Dom, pCns, FALSE);

			if(bDrawCns2)
			{
				CDomyun domCns1(&Dom);
				CDRect rcSide1	= DBStd.m_pDrawAps->DrawRebar_SideStd(&domCns1, pCns1, FALSE);

				CDPoint xyMove1(0, 0);
				if(bStt)
					xyMove	= CDPoint(rcSide.left - rcSide1.right - Dom.Always(100), rcSide.top - rcSide1.top);
				else
					xyMove	= CDPoint(rcSide.right - rcSide1.left + Dom.Always(100), rcSide.top - rcSide1.top);

				domCns1.Move(xyMove);
				rcSide1.left	+= xyMove.x; rcSide1.right	+= xyMove.x;
				rcSide1.top		+= xyMove.y; rcSide1.bottom	+= xyMove.y;
				rcSide			= DBStd.MergeRectAndRect(rcSide, rcSide1);

				Dom << domCns1;
			}

			xyMove	= CDPoint(rcPlane.left-rcSide.left, rcPlane.top-rcSide.bottom-Dom.Always(100)); 
			Dom.Move(xyMove);
			rcSide.left		+= xyMove.x; rcSide.right	+= xyMove.x;
			rcSide.top		+= xyMove.y; rcSide.bottom	+= xyMove.y;

			Dom.Move(pMove->GetPoint(MPE_REBAR_SLABSIDE));
			Dom.SetCalcExtRect();
			pStd->ChangeSeperateLayer(&Dom);
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_SLABSIDE, _T("�ܸ� B-B "));
			*pDomP << Dom;
		}

		if(m_pStd->IsDrawByBlock(MPE_REBAR_SLABSIDE_TITLE))
		{
			pStd->AddTitleOnDom(&Dom, m_nBridge, _T("�ܸ� B-B "), CDPoint(0,0), 0, dScale);
			Dom.Move(rcSide.left+rcSide.Width()/2.0, rcSide.bottom+Dom.Always(50));
			Dom.Move(pMove->GetPoint(MPE_REBAR_SLABSIDE_TITLE));
			pStd->ChangeSeperateLayer(&Dom);
			Dom.SetCalcExtRect();
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_SLABSIDE_TITLE, _T("�ܸ� B-B Ÿ��Ʋ"));
			*pDomP << Dom;
		}
	}
	else
	{
		rcSide.left		= rcPlane.left;
		rcSide.top		= rcPlane.bottom - Dom.Always(100);
		rcSide.right	= rcSide.left;
		rcSide.bottom	= rcSide.top;
	}
	////////////////////////////////////////////////////////////////////////////////////

	// ö�� �� ///////////////////////////////////////////////////////////////////////
	CHgBaseConcStd baseConc;
	CHgBaseDrawStd baseDraw;
	CDRect rcDetailRB(0, 0, 0, 0);
	pPageMng->SetProgressData(_T("ö�ٻ� �����..."), (int)(dCurPer+=dAddPer));
	CTypedPtrArray <CObArray, CRebar*> arrRebar;	// ���ǥ�� ö�ٸ���Ʈ
	MakeRebarList(pCns, &arrRebar);
	if(bDrawCns2)
		MakeRebarList(pCns1, &arrRebar);

	baseConc.ShellSortRebarList(&arrRebar, &CRebar::CompareForList);

	if(pOpt->m_nTypeRebarTable == 0) //�Ϲ�Ÿ���� ��� ö�� ��
	{
		if(dwOptDraw & REBAR_CNS_DETAIL_RB)
		{
			DrawRebarDetail(&Dom, pCns, &arrRebar, CDPoint(rcPlane.right + Dom.Always(50), rcPlane.bottom));
			Dom.SetCalcExtRect();
			rcDetailRB	= Dom.GetExtRect();
			*pDomP << Dom;
		}
	}

	// �⺻Ÿ�Կ� ö�ٻ󼼰� ��µ��� �ʰų�, ���鰣��ȭŸ���̸� x��ǥ �������� ������
	if((pOpt->m_nTypeRebarTable == 0 &&!(dwOptDraw & REBAR_CNS_DETAIL_RB))
		|| pOpt->m_nTypeRebarTable == 1)
	{
		rcDetailRB.left		= rcPlane.left + Dom.Always(100);
		rcDetailRB.bottom	= rcPlane.bottom;
		rcDetailRB.right	= rcDetailRB.left;
		rcDetailRB.top		= rcDetailRB.bottom;
	}

	////////////////////////////////////////////////////////////////////////////////////


	// ���ǥ   ///////////////////////////////////////////////////////////////////////
	CDRect rcTableM(0, 0, 0, 0);
	pPageMng->SetProgressData(_T("���ǥ �����..."), (int)(dCurPer+=dAddPer));
	if(dwOptDraw & REBAR_CNS_TABLE_M)
	{
		if(m_pStd->IsDrawByBlock(MPE_REBAR_SLABTABLEM))
		{
			DBStd.m_pDrawAps->DrawMaterialTable(&Dom, pCns, TRUE, pBri->m_pARcBridgeDataStd->m_nTypeUnit);
			Dom.SetCalcExtRect();
			rcTableM	= Dom.GetExtRect();
			xyMove	= CDPoint(rcDetailRB.right-rcTableM.left+Dom.Always(50), rcDetailRB.bottom - rcTableM.bottom - Dom.Always(20)); 
			Dom.Move(xyMove);
			rcTableM.left		+= xyMove.x; rcTableM.right	+= xyMove.x;
			rcTableM.top		+= xyMove.y; rcTableM.bottom	+= xyMove.y;

			Dom.Move(pMove->GetPoint(MPE_REBAR_SLABTABLEM));
			pStd->ChangeSeperateLayer(&Dom);
			Dom.SetCalcExtRect();
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_SLABTABLEM, _T("���ǥ"));
			*pDomP << Dom;
		}

		if(m_pStd->IsDrawByBlock(MPE_REBAR_SLABTABLEM_TITLE))
		{
			pStd->m_pARoadOptionStd->SetEnvType(&Dom,HCAD_SUB_TITLE);
			Dom.TextOut(0, 0, "���ǥ");
			Dom.Move(rcTableM.left+rcTableM.Width()/2.0, rcTableM.bottom);
			Dom.Move(pMove->GetPoint(MPE_REBAR_SLABTABLEM_TITLE));
			pStd->ChangeSeperateLayer(&Dom);
			Dom.SetCalcExtRect();
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_SLABTABLEM_TITLE, _T("���ǥ Ÿ��Ʋ"));
			*pDomP << Dom;
		}
	}
	else
	{
		rcTableM.left		= rcDetailRB.right + Dom.Always(50);
		rcTableM.bottom		= rcDetailRB.bottom;
		rcTableM.right		= rcTableM.left;
		rcTableM.top		= rcTableM.bottom;
	}
	////////////////////////////////////////////////////////////////////////////////////

	// ö�� ���ǥ   ///////////////////////////////////////////////////////////////////////
	CDRect rcTableRB(0, 0, 0, 0);
	CDPoint xyBase(0);
	baseConc.SetRebarIndexBySortRebarList(&arrRebar, TRUE);  //����ö�ٻ� �ε������� ���̺� ��ȣ ����

	if(dwOptDraw & REBAR_CNS_TABLE_RB)
	{
		pPageMng->SetProgressData(_T("ö�����ǥ �����..."), (int)(dCurPer+=dAddPer));
		xyBase = DrawRebarTable(&Dom, pCns, &arrRebar, "ö�����ǥ", CDPoint(rcTableM.left, rcTableM.top - Dom.Always(100)));
		Dom.SetCalcExtRect();
		rcDetailRB	= Dom.GetExtRect();

		*pDomP << Dom;
	}
	////////////////////////////////////////////////////////////////////////////////////

	if(dwOptDraw & REBAR_CNS_DETAIL_RB) //���鰣��ȭ�� ö�ٻ�
	{
		if(pOpt->m_nTypeRebarTable == 1) //���鰣��ȭŸ��
		{
			pPageMng->SetProgressData("ö�ٻ� �����...", 80);
			baseConc.ShellSortRebarList(&arrRebar, &CRebar::CompareForList);
			baseDraw.DrawRebarListDetailSimple(&Dom, &arrRebar, xyBase, pOpt->m_dWidthRebarTableRect, pOpt->m_dHeightRebarTableRect);

			Dom.SetCalcExtRect();
			Dom.Move(pMove->GetPoint(MPE_REBAR_SLABDETAIL));
			pStd->ChangeSeperateLayer(&Dom);
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_SLABDETAIL, hggettext("ö�ٻ� ����"));

			*pDomP << Dom;
		}
	}

	AhTPADelete(&arrRebar,	(CRebar*)0);

	if(dwOptDraw & REBAR_CNS_TABLE_RB)
	{
		DrawRebarDetailTable_RawRebar(pDomP, pCns, 0, xyBase);
	}
}

// ���� �׷��� �Ǵ� RcBridge����
CRcBridgeRebar* CDrawingCns::GetRcBridgeRebar()
{
	CARcBridgeDrawingStd* pStd	= (CARcBridgeDrawingStd*)m_pStd;
	CARcBridgeDataStd* pDataStd	= pStd->m_pARcBridgeDataStd;

	CRcBridgeRebar *pBri		= NULL;

	if(m_nBridge > -1 && pDataStd->GetBridgeSize() > m_nBridge)
	{
		pBri	= pDataStd->GetBridgeRebar(m_nBridge);
	}

	return pBri;
}

void CDrawingCns::DrawRebarDetail(CDomyun *pDomP, CApsApp *pAps, CTypedPtrArray <CObArray, CRebar*> *pArrRebar, CDPoint xyBase)
{
	CARcBridgeDrawingStd* pStd			= (CARcBridgeDrawingStd*)m_pStd;
	CARcBridgeDrawingOptionStd* pOpt	= pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData	= pOpt->GetDrawingOption(m_nBridge);
	CDrawPageMng* pPageMng				= pStd->GetPageManage();
	COutMovePoint* pMove				= pStd->GetMovePoint(pStd->m_nCurPage);
	CHgBaseDrawStd HGDraw;
	CConcSymbol symDB(pStd->m_pARoadOptionStd);
	CDomyun Dom(pDomP);

	double dSpace = pData->m_dSpaceBetweenRebarDetailAps;
	double dMaxY  = Dom.Always(525);
	double dMinY  = Dom.Always(60);
	double dYPlus = 0;
	CDPoint xy	= CDPoint(xyBase.x, dMaxY);
	CRebar *pRB;

	if(m_pStd->IsDrawByBlock(MPE_REBAR_SLABDETAIL_TITLE))
	{
		pStd->m_pARoadOptionStd->SetEnvType(&Dom,HCAD_SUB_TITLE);
		Dom.TextOut(xy.x+Dom.Always(50),xy.y+Dom.GetTextHeight()/2,"ö �� �� ��");
		Dom.Move(pMove->GetPoint(MPE_REBAR_SLABDETAIL_TITLE));
		pStd->ChangeSeperateLayer(&Dom);
		Dom.SetCalcExtRect();
		pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_SLABDETAIL_TITLE, "ö�ٻ� ����");
		*pDomP << Dom;
	}

	pStd->m_pARoadOptionStd->SetEnvType(&Dom,HCAD_DIML);

	long nSize  = pArrRebar->GetSize();
	long nRow   = 0;
	long nType  = 0;
	long nDetailCol = 0;
	BOOL bJoint = FALSE;
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
				if(m_pStd->IsDrawByBlock(MPE_REBAR_SLABDETAIL+nDetailCol))
				{
					Dom.Move(pMove->GetPoint(MPE_REBAR_SLABDETAIL+nDetailCol));
					pStd->ChangeSeperateLayer(&Dom);
					Dom.SetCalcExtRect();
					pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_SLABDETAIL+nDetailCol, "ö�ٻ� "+COMMA(1+nDetailCol));
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
			if(m_pStd->IsDrawByBlock(MPE_REBAR_SLABDETAIL+nDetailCol*10))
			{
				Dom.Move(pMove->GetPoint(MPE_REBAR_SLABDETAIL+nDetailCol*10));
				pStd->ChangeSeperateLayer(&Dom);
				Dom.SetCalcExtRect();
				pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_SLABDETAIL+nDetailCol, "ö�ٻ� "+COMMA(1+nDetailCol));
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

	if(m_pStd->IsDrawByBlock(MPE_REBAR_SLABDETAIL+nDetailCol))
	{
		Dom.Move(pMove->GetPoint(MPE_REBAR_SLABDETAIL+nDetailCol));
		pStd->ChangeSeperateLayer(&Dom);
		Dom.SetCalcExtRect();
		pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_REBAR_SLABDETAIL+nDetailCol, "ö�ٻ� "+COMMA(1+nDetailCol));
		*pDomP << Dom;
	}
}

CDPoint CDrawingCns::DrawRebarTable(CDomyun *pDomP, CApsApp *pAps, CTypedPtrArray <CObArray, CRebar*> *pArrRebar, CString sSubTitle, CDPoint xyBase)
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
	double Fy = pBri->m_pARcBridgeDataStd->m_Fy;
	
	// ö�����ǥ �׸��� ������ ����
	if(pBri->m_pARcBridgeDataStd->m_nTypeUnit == 1)
		Fy = Fy * 10;

	BOOL bShadeOutLone = TRUE;//(pOpt->m_dDrawGen & GEBDRAW_GEN_TABLESHADE) ? TRUE : FALSE;

	CDRect rtTable;
	long nIndex = 0;
	long nTableSu = 0;
	while(1)
	{
		if(nIndex >= pArrRebar->GetSize()) break;
		if(m_pStd->IsDrawByBlock(nDefTable+nTableSu))
		{
			if(nTableType == 0) //�⺻Ÿ��
				nIndex = baseDraw.DrawRebarTable(&Dom, nIndex, nMaxRow, xyBase, dLengthSum, dLengthSumAdd, dLengthTotal, dWeightTotal, dWeightTotalAdd, *pArrRebar, Fy, bShadeOutLone, "", pBCI, pBCI->m_nRoundRebarLength);
			else //����ȭŸ��
				nIndex = baseDraw.DrawRebarTableSimple(&Dom, nIndex, nMaxRow, xyBase, dLengthSum, dLengthSumAdd, dLengthTotal, dWeightTotal, dWeightTotalAdd, *pArrRebar, Fy, bShadeOutLone, "", pBCI, pBCI->m_nRoundRebarLength);

			Dom.SetCalcExtRect();
			rtTable = Dom.GetExtRect();

			Dom.Move(pMove->GetPoint(nDefTable+nTableSu));
			pStd->ChangeSeperateLayer(&Dom);
			Dom.SetCalcExtRect();
			pMove->AddMPE(&Dom, Dom.GetExtRect(), nDefTable+nTableSu, "ö�����ǥ");
			*pDomP << Dom;
		}
		xyBase.x += rtTable.Width() + Dom.Always(10);
		nTableSu++;
	}		

	return xyBase;}

void CDrawingCns::MakeRebarList(CApsApp *pAps, CTypedPtrArray <CObArray, CRebar*> *pArrRebar)
{
	AhTPADelete(pArrRebar,	(CRebar*)0);
	long nSize	= pAps->m_pArrRebar.GetSize();

	long i = 0; for(i = 0; i < nSize; i++)
	{
		CRebar *pRB	= pAps->m_pArrRebar.GetAt(i);
		if(pRB->IsTrue())
		{
			CRebar *pRBTmp = new CRebar;
			*pRBTmp	= *pRB;
			pArrRebar->Add(pRBTmp);
		}
	}
}

void CDrawingCns::DrawRebarDetailTable_RawRebar( CDomyun *pDom, CApsApp *pCns, long nIdxTable, CDPoint &xyBase )
{
	CARcBridgeDrawingStd* pStd			= (CARcBridgeDrawingStd*)m_pStd;
	CRcBridgeRebar *pBri				= GetRcBridgeRebar();
	CARcBridgeDrawingOptionStd* pOpt	= pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData	= pOpt->GetDrawingOption(m_nBridge);

	if(pBri->m_pARcBridgeDataStd->m_pBasicConcInfo->IsApplyExtraChargeAtRebarJoint() == FALSE && pOpt->m_bAddRawRebarTable)
	{
		CTypedPtrArray <CObArray, CRebar*> arrRebar;
		CTypedPtrArray <CObArray, CRebar*> arrRawRebar;

		CHgBaseConcStd baseConc;
		long nIdxTable = 3;

		MakeRebarList(pCns, &arrRebar);
		
		long nCountRaw = baseConc.GetRawRebarList(&arrRebar, &arrRawRebar, pBri->m_pARcBridgeDataStd->m_pBasicConcInfo->GetLenghtRebarMax());
		if(nCountRaw > 0)
		{
			if(FALSE)
			{
				double dFy	= 1;
				while(dFy != 0)
				{
					CTypedPtrArray <CObArray, CRebar*> arrRebarGet;
					dFy	= pStd->GetRebarListByFy(&arrRawRebar, &arrRebarGet, dFy);
					if(dFy == 0) break;
					if(arrRebarGet.GetSize() == 0) break;

					baseConc.ShellSortRebarList(&arrRebarGet, &CRebar::CompareForTable);
					xyBase = DrawRebarTable(pDom, pCns, &arrRebar, _T("�̰��� ö��"), xyBase);
				}
			}
			else
			{
				baseConc.ShellSortRebarList(&arrRawRebar, &CRebar::CompareForTable);
				xyBase = DrawRebarTable(pDom, pCns, &arrRebar, _T("�̰��� ö��"), xyBase);
			}
		}

		AhTPADelete(&arrRawRebar, (CRebar*)0);
	}
}
