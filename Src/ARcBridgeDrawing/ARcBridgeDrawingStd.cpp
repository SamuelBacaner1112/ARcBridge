// ARcBridgeDrawingStd.cpp: implementation of the CARcBridgeDrawingStd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../../Src/ARcBridgeData/ARcBridgeData.h"
#include "../ARcBridgeDrawingOption/ARcBridgeDrawingOption.h"
#include "ARcBridgeDrawing.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CARcBridgeDrawingStd::CARcBridgeDrawingStd() : CDrawStd()
{
	m_pDrawPageMng->AddOnePage(new CDrawingVerticalPlane, _T("����鵵"), this);
	m_pDrawPageMng->AddOnePage(new CDrawingBoring, _T("���� �ֻ�"), this);
	m_pDrawPageMng->AddOnePage(new CDrawingGeneral, _T("�Ϲݵ�"), this);
	m_pDrawPageMng->AddOnePage(new CDrawingRebar, _T("������"), this);
	m_pDrawPageMng->AddOnePage(new CDrawingAps, _T("���� ������"), this);
	m_pDrawPageMng->AddOnePage(new CDrawingCns, _T("���� ������"), this);
	m_pDrawPageMng->AddOnePage(new CDrawingOthers, _T("��Ÿ ����"), this);
	m_pDrawPageMng->AddOnePage(new CDrawingUser, _T("����� ����"), this);
	//m_pDrawPageMng->AddOnePage(new CDrawingShoeArrange, _T("���� ��ħ ��ġ��"), this);
}

CARcBridgeDrawingStd::~CARcBridgeDrawingStd()
{
	m_pDrawPageMng->ResetAll();	

	AhTPADelete(&m_pPageInfo, (CPageInfo*)0);
}

long CARcBridgeDrawingStd::GetCnvPage(long nPage)
{
	return GetOrgPage(nPage-1);
}

long CARcBridgeDrawingStd::GetCnvSubPage(long nPage)
{	
	return GetSubPage(nPage)+1;
}

void CARcBridgeDrawingStd::MakePageInfo()
{
// 	MakePage();

	// ������ ��������� ������ ��� ����
	AhTPADelete(&m_pPageInfo, (CPageInfo*)0);

	// ���� ������ŭ ���� Manage ����
	CString str = _T("");
	int nCountBri = m_pARcBridgeDataStd->GetBridgeSize();
	int nCountCate = GetCountCategory();
	long i=0; for(i=0; i<nCountBri; i++)
	{
		for(int j=0; j<nCountCate; j++)
		{
			str = GetTitleCategory(j);
			if(str==_T("����鵵"))
				MakeCategoryVerticalPlane(i, j);
			else if(str==_T("���� �ֻ�"))
				MakeCategoryBoring(i, j);
			else if(str==_T("�Ϲݵ�"))
				MakeCategoryGeneral(i, j);
			else if(str==_T("������"))
				MakeCategoryRebar(i, j);
			else if(str==_T("���� ������"))
				MakeCategoryAps(i, j);
			else if(str==_T("���� ������"))
				MakeCategoryCns(i, j);
			else if(str==_T("��Ÿ ����"))
				MakeCategoryOthers(i, j);
// 			else if(str==_T("���� ��ħ ��ġ��"))
// 				MakeCategoryShoeArrange(i, j);
			else if(str==TITLE_SLAB_STANDARD_OUTPUT)
				MakeCategorySlabStandardOutput(i, j);
		}
	}
}

CPageInfo* CARcBridgeDrawingStd::AddPageInfo(long nBri, long nCategory, long nCategoryPageNum, CString strTitle)
{
	CPageInfo* pInfo = new CPageInfo;

	pInfo->m_nBri = nBri;
	pInfo->m_nCategory = nCategory;
	pInfo->m_nPageNumber = nCategoryPageNum;
	pInfo->m_strTitle = strTitle;

	m_pPageInfo.Add(pInfo);

	return pInfo;
}

void CARcBridgeDrawingStd::MakeCategoryVerticalPlane(long nBri, long nCategory)
{
	CRcBridgeApp* pBri = m_pARcBridgeDataStd->GetBridge(nBri);
	if(pBri==NULL) return;

	AddPageInfo(nBri, nCategory, 1, _T("����鵵"));
}

void CARcBridgeDrawingStd::MakeCategoryGeneral(long nBri, long nCategory)
{
	CRcBridgeApp* pBri = m_pARcBridgeDataStd->GetBridge(nBri);
	if(pBri==NULL) return;

	AddPageInfo(nBri, nCategory, 1, _T("�Ϲݵ� (1)"));
	int nDom = (int)(RoundUp(pBri->GetCountJijum()/2.0, 0));

	// (ARCBRIDGE-3415) ��� ��ü�� ���� ���鵵�� ������� �ʴ´�.
	if(pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP || pBri->m_nTypeBridge == BRIDGE_TYPE_TRAP2)
		nDom -= 1;

	CString str = _T("");
	int i=0; for(i=0; i<nDom; i++)
	{
		str.Format(_T("�Ϲݵ� (%d)"), i+2);
		AddPageInfo(nBri, nCategory, i+2, str);
	}
	
	BOOL bWingWall = FALSE;
	if(pBri->IsWingWall(TRUE, TRUE)) bWingWall = TRUE;
	if(pBri->IsWingWall(TRUE, FALSE)) bWingWall = TRUE;
	if(pBri->IsWingWall(FALSE, TRUE)) bWingWall = TRUE;
	if(pBri->IsWingWall(FALSE, FALSE)) bWingWall = TRUE;
	if(bWingWall)
	{
		str.Format(_T("�Ϲݵ� (%d)"), nDom+2);
		AddPageInfo(nBri, nCategory, nDom+2, str);
	}
}

void CARcBridgeDrawingStd::MakeCategoryRebar(long nBri, long nCategory)
{
	CRcBridgeRebar *pBri = m_pARcBridgeDataStd->GetBridgeRebar(nBri);
	if(pBri==NULL) return;
	CARcBridgeDrawingOptionStd* pOpt = m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData = pOpt->GetDrawingOption(nBri);
	if(!pData) return;

	long nCountMainRebarAssm	= GetCountPageMainRebarAssm(nBri);
	CString str	= _T("");
	long nPageRebar	= 0;

	// ��ö�� ������.
	long i = 0; for(i = 0; i < nCountMainRebarAssm; i++)
	{
		str.Format("��ö�� ������ (%d)", i+1);
		AddPageInfo(nBri, nCategory, nPageRebar++, str);
	}

	// ǥ�شܸ鵵
	AddPageInfo(nBri, nCategory, nPageRebar++, _T("ǥ�� �ܸ鵵"));

	if(pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
	{
		// ǥ�شܸ鵵
		AddPageInfo(nBri, nCategory, nPageRebar++, _T("��ġ���� ������"));
	}

	// ��� ���� ������
	AddPageInfo(nBri, nCategory, nPageRebar++, "��� ������ ������");
	if(pBri->IsBoxType())
	{
		AddPageInfo(nBri, nCategory, nPageRebar++, "�Ϻ� ������ ������");
	}

	// ������ü ������
	if(pBri->IsOutWall(TRUE))
	{
		AddPageInfo(nBri, nCategory, nPageRebar++, "������ ��ü ������");
	}
	if(pBri->IsOutWall(FALSE))
	{
		AddPageInfo(nBri, nCategory, nPageRebar++, "������ ��ü ������");
	}

	// ������ü ������
	BOOL bSeparateBMColumn	= pData->m_bSeparateRebarBMColumn;
	long nCountInWallRebar	= pBri->GetCountInWall();
	for(i = 0; i < nCountInWallRebar; i++)
	{
		CWallApp *pWall	= pBri->GetInWall(i);
		if(!pWall) continue;
		if(!pWall->m_bIs) continue;
		if(pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
		{
			if(i+1 == pBri->GetJijumConnectRibAndSlab())
				continue;
		}
		if(!bSeparateBMColumn || !pWall->m_bTopIsColumn)
		{
			CString sWall	= pWall->m_bTopIsColumn ? "�����" : "�߰���ü";
			
			str.Format("%s ������(%d)", sWall, i+1);
			AddPageInfo(nBri, nCategory, nPageRebar++, str);

			if(bSeparateBMColumn && pWall->m_bTopIsColumn)
			{
				str.Format("%s ö�� ���ǥ(%d)", sWall, i+1);
				AddPageInfo(nBri, nCategory, nPageRebar++, str);
			}
		}
	}

	CWingWall* pWingBL = pBri->GetWingWall(TRUE, TRUE);
	CWingWall* pWingER = pBri->GetWingWall(FALSE, FALSE);
	CWingWall* pWingBR = pBri->GetWingWall(TRUE, FALSE);
	CWingWall* pWingEL = pBri->GetWingWall(FALSE, TRUE);

	int nCount = 0;
	BOOL bSeparateBMWing	= pData->m_bSeparateRebarBMWing;
	if(!bSeparateBMWing)
	{
		if(pWingBL->m_bExist)
		{
			nCount++;
			str.Format(_T("������ ������ (%d)"), nCount);
			AddPageInfo(nBri, nCategory, nPageRebar++, str);

//			if(bSeparateBMWing)
//			{
//				str.Format("������ ö�� ���ǥ (%d)", nCount);
//				AddPageInfo(nBri, nCategory, nPageRebar++, str);
//			}
		}
		if(pWingER->m_bExist)
		{
			nCount++;
			str.Format(_T("������ ������ (%d)"), nCount);
			AddPageInfo(nBri, nCategory, nPageRebar++, str);

//			if(bSeparateBMWing)
//			{
//				str.Format("������ ö�� ���ǥ (%d)", nCount);
//				AddPageInfo(nBri, nCategory, nPageRebar++, str);
//			}
		}
		if(pWingBR->m_bExist)
		{
			nCount++;
			str.Format(_T("������ ������ (%d)"), nCount);
			AddPageInfo(nBri, nCategory, nPageRebar++, str);

//			if(bSeparateBMWing)
//			{
//				str.Format("������ ö�� ���ǥ (%d)", nCount);
//				AddPageInfo(nBri, nCategory, nPageRebar++, str);
//			}
		}
		if(pWingEL->m_bExist)
		{
			nCount++;
			str.Format(_T("������ ������ (%d)"), nCount);
			AddPageInfo(nBri, nCategory, nPageRebar++, str);

//			if(bSeparateBMWing)
//			{
//				str.Format("������ ö�� ���ǥ (%d)", nCount);
//				AddPageInfo(nBri, nCategory, nPageRebar++, str);
//			}
		}
	}

	// ö�����ǥ(��ü,������) - �׻� �������� �߰��ؾ� ��. �ٸ� ���� �߰��� ���� ö�����ǥ ���� �ϱ⸦ �ٶ�!!!
	BOOL bSeparateOutputRebarBMMain	= pData->m_bSeparateOutputRebarBMMain;
	if(bSeparateOutputRebarBMMain)
	{
		AddPageInfo(nBri, nCategory, nPageRebar++, _T("ö�� ���ǥ (1)"));
		AddPageInfo(nBri, nCategory, nPageRebar++, _T("ö�� ���ǥ (2)"));
	}
	else
	{
		AddPageInfo(nBri, nCategory, nPageRebar++, _T("ö�� ���ǥ"));
	}

	// �����,�߰���ü
	for(i = 0; i < nCountInWallRebar; i++)
	{
		CWallApp *pWall	= pBri->GetInWall(i);
		if(!pWall) continue;
		if(!pWall->m_bIs) continue;
		if(bSeparateBMColumn && pWall->m_bTopIsColumn)
		{
			CString sWall	= pWall->m_bTopIsColumn ? "�����" : "�߰���ü";
			
			str.Format("%s ������(%d)", sWall, i+1);
			AddPageInfo(nBri, nCategory, nPageRebar++, str);

			if(bSeparateBMColumn && pWall->m_bTopIsColumn)
			{
				str.Format("%s ö�� ���ǥ(%d)", sWall, i+1);
				AddPageInfo(nBri, nCategory, nPageRebar++, str);
			}
		}
	}

	if(bSeparateBMWing)
	{
		if(pWingBL->m_bExist)
		{
			nCount++;
			str.Format(_T("������ ������ (%d)"), nCount);
			AddPageInfo(nBri, nCategory, nPageRebar++, str);

//			if(bSeparateBMWing)
//			{
//				str.Format("������ ö�� ���ǥ (%d)", nCount);
//				AddPageInfo(nBri, nCategory, nPageRebar++, str);
//			}
		}
		if(pWingER->m_bExist)
		{
			nCount++;
			str.Format(_T("������ ������ (%d)"), nCount);
			AddPageInfo(nBri, nCategory, nPageRebar++, str);

//			if(bSeparateBMWing)
//			{
//				str.Format("������ ö�� ���ǥ (%d)", nCount);
//				AddPageInfo(nBri, nCategory, nPageRebar++, str);
//			}
		}
		if(pWingBR->m_bExist)
		{
			nCount++;
			str.Format(_T("������ ������ (%d)"), nCount);
			AddPageInfo(nBri, nCategory, nPageRebar++, str);

//			if(bSeparateBMWing)
//			{
//				str.Format("������ ö�� ���ǥ (%d)", nCount);
//				AddPageInfo(nBri, nCategory, nPageRebar++, str);
//			}
		}
		if(pWingEL->m_bExist)
		{
			nCount++;
			str.Format(_T("������ ������ (%d)"), nCount);
			AddPageInfo(nBri, nCategory, nPageRebar++, str);

//			if(bSeparateBMWing)
//			{
//				str.Format("������ ö�� ���ǥ (%d)", nCount);
//				AddPageInfo(nBri, nCategory, nPageRebar++, str);
//			}
		}
	}
}

void CARcBridgeDrawingStd::MakeCategoryAps(long nBri, long nCategory)
{
	CRcBridgeApp* pBri = m_pARcBridgeDataStd->GetBridge(nBri);
	if(pBri==NULL) return;

	long nIdx	= 0;

	// ���ӽ�����
	if(pBri->IsAps(TRUE, TRUE))
	{
		AddPageInfo(nBri, nCategory, PAGE_APS_1+nIdx, _T("���� ������ (")+COMMA(nIdx+1)+")");
		nIdx++;
	}
	if(pBri->IsAps(TRUE, FALSE))
	{
		AddPageInfo(nBri, nCategory, PAGE_APS_1+nIdx, _T("���� ������ (")+COMMA(nIdx+1)+")");
		nIdx++;
	}
	if(pBri->IsAps(FALSE, TRUE))
	{
		AddPageInfo(nBri, nCategory, PAGE_APS_1+nIdx, _T("���� ������ (")+COMMA(nIdx+1)+")");
		nIdx++;
	}
	if(pBri->IsAps(FALSE, FALSE))
	{
		AddPageInfo(nBri, nCategory, PAGE_APS_1+nIdx, _T("���� ������ (")+COMMA(nIdx+1)+")");
		nIdx++;
	}
}

void CARcBridgeDrawingStd::MakeCategoryCns(long nBri, long nCategory)
{
	CRcBridgeApp* pBri = m_pARcBridgeDataStd->GetBridge(nBri);
	if(pBri==NULL) return;
	CARcBridgeDrawingOptionStd* pOpt = m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pOptData = pOpt->GetDrawingOption(nBri);
	if(!pOptData) return;

	BOOL bDrawMerge12	= pOptData->m_nTypeCnsRebar==0 ? TRUE : FALSE;
	long nIdx	= 0;

	// ���潽����
	if(pBri->IsCns(TRUE, TRUE, 0))
	{
		AddPageInfo(nBri, nCategory, PAGE_CNS_1+nIdx, _T("���� ������ (")+COMMA(nIdx+1)+")");
		nIdx++;
	}
	if(!bDrawMerge12 && pBri->IsCns(TRUE, TRUE, 1))
	{
		AddPageInfo(nBri, nCategory, PAGE_CNS_1+nIdx, _T("���� ������ (")+COMMA(nIdx+1)+")");
		nIdx++;
	}

	if(pBri->IsCns(TRUE, FALSE, 0))
	{
		AddPageInfo(nBri, nCategory, PAGE_CNS_1+nIdx, _T("���� ������ (")+COMMA(nIdx+1)+")");
		nIdx++;
	}
	if(!bDrawMerge12 && pBri->IsCns(TRUE, FALSE, 1))
	{
		AddPageInfo(nBri, nCategory, PAGE_CNS_1+nIdx, _T("���� ������ (")+COMMA(nIdx+1)+")");
		nIdx++;
	}

	if(pBri->IsCns(FALSE, TRUE, 0))
	{
		AddPageInfo(nBri, nCategory, PAGE_CNS_1+nIdx, _T("���� ������ (")+COMMA(nIdx+1)+")");
		nIdx++;
	}
	if(!bDrawMerge12 && pBri->IsCns(FALSE, TRUE, 1))
	{
		AddPageInfo(nBri, nCategory, PAGE_CNS_1+nIdx, _T("���� ������ (")+COMMA(nIdx+1)+")");
		nIdx++;
	}

	if(pBri->IsCns(FALSE, FALSE, 0))
	{
		AddPageInfo(nBri, nCategory, PAGE_CNS_1+nIdx, _T("���� ������ (")+COMMA(nIdx+1)+")");
		nIdx++;
	}
	if(!bDrawMerge12 && pBri->IsCns(FALSE, FALSE, 1))
	{
		AddPageInfo(nBri, nCategory, PAGE_CNS_1+nIdx, _T("���� ������ (")+COMMA(nIdx+1)+")");
		nIdx++;
	}
}

void CARcBridgeDrawingStd::MakeCategoryOthers(long nBri, long nCategory)
{
	CRcBridgeApp* pBri = m_pARcBridgeDataStd->GetBridge(nBri);
	if(pBri==NULL) return;

	CDrawingOthers *pOther = (CDrawingOthers*)m_pDrawPageMng->GetPage(nCategory);
	if(!pOther)	return;
	
	CString strTitle = _T("");
	CExFootingApp *pExFooting;

	long nPageCount = 0;

	// ��ȣ��(������ 1��)
	AddPageInfo(nBri, nCategory, nPageCount++, "��ȣ�� �󼼵�");

	// ������(�ɼǿ� ���� 1�� �Ǵ� 0��)
	if(pBri->m_nMonumenu>0)
	{
		AddPageInfo(nBri, nCategory, nPageCount++, "������ ��ħ �󼼵�");
	}

	long nTypeBridge = pBri->m_nTypeBridge;
	if(nTypeBridge == BRIDGE_TYPE_PI || nTypeBridge == BRIDGE_TYPE_SLAB 
		|| nTypeBridge == BRIDGE_TYPE_TRAP)
	{
		AddPageInfo(nBri, nCategory, nPageCount++, "���� ��ħ ��ġ��");
	}

	// ���� Ÿ�� (�������)
	if(pBri->IsExsitHyunTa())
	{
		BOOL bBoxType = pBri->IsBoxType();

		int nCntJijum = pBri->GetCountJijum();
		if(!pBri->IsFooting(FALSE)) --nCntJijum;

		for(long nJijum=0; nJijum<nCntJijum; nJijum++)
		{
			if(!bBoxType && !pBri->IsFooting(TRUE) && nJijum==0) continue;
			if(bBoxType && nJijum > 0) break;

			CFootingApp *pFoot = pBri->GetFootingByJijumNumber(nJijum);
			if(pFoot == NULL) continue;

			pExFooting = &pFoot->m_exFooting;

			CExPileApp *pPile(NULL);
			if(pExFooting->m_bIs) pPile = &pExFooting->m_Pile;

			if(pPile != NULL && pPile->m_nType == PILE_TYPE_HYUNJANG)
			{
				if(nJijum == 0) strTitle = bBoxType ? "" : "(������)";
				else if(nJijum == pBri->GetCountJijum()-1) strTitle = "(������)";
				else strTitle.Format("(�߰� ������ %d)", nJijum);
				AddPageInfo(nBri, nCategory, nPageCount++, "����Ÿ�� ����"+strTitle+" �󼼵�");
			}
		}
	}

	pOther->SetArrEhpDXFName(nBri);
	if(pBri->IsIncludeEHP())
	{
		long i = 0; for(i = 0; i < pOther->m_arrEhpDxfName.GetSize(); i++)
			AddPageInfo(nBri, nCategory, nPageCount++, pOther->m_arrEhpDxfName.GetAt(i));

		//AddPageInfo(nBri, nCategory, nPageCount++, "E.H.P ��������(1)");
		//AddPageInfo(nBri, nCategory, nPageCount++, "E.H.P ��������(2)");
	}

//	AddPageInfo(nBri, nCategory, PAGE_OTHERS_1, _T("��Ÿ ���� (1)"));
//	AddPageInfo(nBri, nCategory, PAGE_OTHERS_2, _T("��Ÿ ���� (2)"));
}

void CARcBridgeDrawingStd::MakeCategoryShoeArrange(long nBri, long nCategory)
{
	CRcBridgeApp* pBri = m_pARcBridgeDataStd->GetBridge(nBri);
	if(pBri==NULL) return;

//	AddPageInfo(nBri, nCategory, PAGE_SHOE_ARRANGE_1, _T("���� ��ħ ��ġ��"));
//	AddPageInfo(nBri, nCategory, PAGE_SHOE_ARRANGE_2, _T("Elevation ���� �ٰŵ�"));
}

void CARcBridgeDrawingStd::MakeCategorySlabStandardOutput(long nBri, long nCategory)
{
	AddPageInfo(nBri, nCategory, 0, "RC�����걳 ǥ�� ���� ���(1)");
	AddPageInfo(nBri, nCategory, 1, "RC�����걳 ǥ�� ���� ���(2)");
	AddPageInfo(nBri, nCategory, 2, "RC�����걳 ǥ�� ���� ���(3)");
	AddPageInfo(nBri, nCategory, 3, "RC�����걳 ǥ�� ���� ���(4)");
}

void CARcBridgeDrawingStd::MakeCategoryBoring(long nBri, long nCategory)
{
	CRcBridgeApp* pBri = m_pARcBridgeDataStd->GetBridge(nBri);
	if(pBri==NULL) return;

	if(pBri->m_nTypeBridge != BRIDGE_TYPE_SLAB)
		AddPageInfo(nBri, nCategory, PAGE_BORING_1, _T("���� �ֻ�"));

}

// ���� Ÿ��Ʋ ����
// BOOL bSmallTitle : ���� �������� ����
void CARcBridgeDrawingStd::AddTitleOnDom(CDomyun *pDom, long nBri, CString strTitle, CDPoint xy, long nScaleDir, double dScale, BOOL bSmallTitle)
{
	CARcBridgeDrawingOptionStd* pStd = m_pARcBridgeDrawingOptionStd;

	CDomyun	Dom(pDom);

	m_pARoadOptionStd->SetEnvType(&Dom, bSmallTitle ? HCAD_SMALL_TITLE : HCAD_SUB_TITLE);
	if(pStd->m_dwDrawCommon & COMMON_TITLEBOX)
	{
		double dTextW = Dom.GetTextWidth(strTitle);
		double dTextH = Dom.GetTextHeight();
		double dT = Dom.Always(1);

		m_pARoadOptionStd->SetEnvType(&Dom, HCAD_TBL2);

		CDRect rect;
		rect = CDRect(xy.x - dTextW/2 - Dom.Always(8), xy.y - Dom.Always(3), xy.x + dTextW/2 + Dom.Always(8), xy.y + dTextH + Dom.Always(3));				
		Dom.Rectangle(rect);
		Dom.Solid(rect.right, rect.bottom-dT, rect.right+dT, rect.bottom-dT, rect.right+dT, rect.top, rect.right, rect.top);
		Dom.Solid(rect.left+dT, rect.top, rect.right+dT, rect.top, rect.right+dT, rect.top-dT, rect.left+dT, rect.top-dT);
	}
	
	m_pARoadOptionStd->SetEnvType(&Dom, bSmallTitle ? HCAD_SMALL_TITLE : HCAD_SUB_TITLE);
	double dLength = Dom.GetTextWidth(strTitle);
	Dom.TextOut(xy, strTitle);

	if(nScaleDir>=0)
	{
		m_pARoadOptionStd->SetEnvType(&Dom, HCAD_SMALL_TITLE);
		double term = pStd->m_dwDrawCommon & COMMON_TITLEBOX ? Dom.GetTextHeight()*2.5 : Dom.GetTextHeight()*2;
		if (nScaleDir==0)
			Dom.SetTextAlignHorz(TA_LEFT);
		else
			Dom.SetTextAlignHorz(TA_RIGHT);

		xy.x += dLength/2 - Dom.GetTextWidth(_T(" "));
		xy.y -= term;

		CString	str;
		if(dScale>0)
			str.Format(_T("S=1:%.f"), dScale);
		else if(dScale == 0 )
			str	= _T("S=NONE");

		Dom.TextOut(xy, str);
	}

	*pDom << Dom;
}

void CARcBridgeDrawingStd::ChangeSeperateLayer(CDomyun *pDom)
{		
	POSITION pos = pDom->m_DrawObjMap.GetStartPosition(),p2;
	try
	{
		while(pos)
		{	
			p2 = pos;					
			CDrawObject *Key, *Value;
			pDom->m_DrawObjMap.GetNextAssoc(pos,(void *&)Key,(void *&)Value);				
			if( Value->GetEttKind() == ETT_ARC )
			{
				CString szLayer		= Value->GetsLayer(),
						szCadLayer	= m_pOptStd->m_LayerList[HCAD_DIML]->m_szCadLayername;

				if( szLayer.Right(szCadLayer.GetLength()) == szCadLayer && CHgBaseDrawStd::IsUseHCadBlock())
				{
					CObArc *pArc = (CObArc *)Value;					
					if(!pArc->m_bCircle || !pArc->m_bFillstyle) continue;
					CDPoint ptCen = pArc->m_SttPoint;
					double dAng = pArc->m_dRotate;
					pDom->RemoveObject(pArc);					
					CDomyun Dom(pDom);
					CHgBaseDrawStd::InsertHCadBlock(&Dom, "CGDIMDT2", ptCen.x, ptCen.y, pDom->GetScaleDim() * m_pOptStd->m_DimSet.ArrowSize);																				
					if(dAng!=0) Dom.Rotate(dAng, ptCen.x, ptCen.y);
					*pDom << Dom;
					continue;
				}
			}

			if( Value->GetEttKind() == ETT_TEXT )
			{
				CObText *pText = (CObText *)Value;
				CString szLayer		= Value->GetsLayer(),
						szCadLayer	= m_pOptStd->m_LayerList[HCAD_DIML]->m_szCadLayername,
						szCadLayer2 = m_pOptStd->m_LayerList[HCAD_TBL1]->m_szCadLayername;

				if( szLayer.Right(szCadLayer.GetLength()) == szCadLayer )
				{
					szLayer.Replace(m_pOptStd->m_LayerList[HCAD_DIML]->m_szCadLayername, m_pOptStd->m_LayerList[HCAD_DIMT]->m_szCadLayername);					Value->SetsLayer(szLayer);
					Value->m_nColor = -1;
				}
				else if( szLayer.Right(szCadLayer2.GetLength()) == szCadLayer2 )
				{
					szLayer.Replace(m_pOptStd->m_LayerList[HCAD_TBL1]->m_szCadLayername, m_pOptStd->m_LayerList[HCAD_TBTC]->m_szCadLayername);
					Value->SetsLayer(szLayer);
					Value->m_nColor = -1;
				}
			}
			else if( Value->GetEttKind() == ETT_MTEXT )
			{
				CObMText *pText = (CObMText *)Value;
				CString szLayer		= Value->GetsLayer(),
						szCadLayer	= m_pOptStd->m_LayerList[HCAD_DIML]->m_szCadLayername,
						szCadLayer2 = m_pOptStd->m_LayerList[HCAD_TBL1]->m_szCadLayername;

				if( szLayer.Right(szCadLayer.GetLength()) == szCadLayer )
				{
					szLayer.Replace(m_pOptStd->m_LayerList[HCAD_DIML]->m_szCadLayername, m_pOptStd->m_LayerList[HCAD_DIMT]->m_szCadLayername);
					Value->SetsLayer(szLayer);
					Value->m_nColor = -1;
				}
				else if( szLayer.Right(szCadLayer2.GetLength()) == szCadLayer2 )
				{
					szLayer.Replace(m_pOptStd->m_LayerList[HCAD_TBL1]->m_szCadLayername, m_pOptStd->m_LayerList[HCAD_TBTC]->m_szCadLayername);
					Value->SetsLayer(szLayer);
					Value->m_nColor = -1;
				}
			}			
		}

		// ���� ����
		CString sKey;
		CDomyunBase* pBaseDom(NULL);
		CMapStringToPtr* pBlockMap = pDom->GetBlockArray();
		pos = pBlockMap->GetStartPosition();
		while(pos)
		{
			pBlockMap->GetNextAssoc(pos,sKey,(void *&)pBaseDom);
			if( NULL != pBaseDom )
			{
				CString szBlockName = pBaseDom->GetBlockName();
				if(szBlockName.Left(2)=="CG")
				{
					pBaseDom->SetBlockName("CX"+szBlockName.Mid(2));
				}
			}
		}
		// Insert ��ü�� ����
		CPtrList *pList = pDom->GetObjectList(ETT_INSERT);
		pos = pList->GetHeadPosition();
		while(pos)
		{
			CObInsert *pIns = (CObInsert *)pList->GetNext(pos);			
			CString szBlockName = pIns->m_sBlockName;
			if(szBlockName.Left(2)=="CG")
			{
				pIns->m_sBlockName = "CX"+szBlockName.Mid(2);
			}			
		}

	}
	catch(...) {}
}

CPageInfo* CARcBridgeDrawingStd::GetPageInfo(long nBri, long nSubPage)
{
	CRcBridgeApp* pBri = m_pARcBridgeDataStd->GetBridge(nBri);

	if(pBri==NULL)
		return NULL;

	long nPage = 0;
	long i=0; for(i=0; i<GetCountTotalPage(); i++)
	{
		if(m_pPageInfo[i]->m_nBri==nBri)
		{
			if(nPage==nSubPage-1)
				return m_pPageInfo[i];

			nPage++;
		}
	}

	return NULL;
}

CPageInfo* CARcBridgeDrawingStd::GetPageInfo(HTREEITEM hItem)
{
	if(hItem==NULL)
		return NULL;

	long i=0; for(i=0; i<GetCountTotalPage(); i++)
	{
		if(m_pPageInfo[i]->m_hItem==hItem)
			return m_pPageInfo[i];
	}

	return NULL;
}

CPageInfo* CARcBridgeDrawingStd::GetPageInfo(long nSubPage)
{
	if(nSubPage<1 || nSubPage>GetCountTotalPage())
		return NULL;

	return m_pPageInfo.GetAt(nSubPage-1);
}

// �Ϲݵ�, ������, ���� ������ Ŭ�������� ���� �������� �´� CPageInfo �����͸� ���Ѵ�.
// m_pStd->m_nCurPage �� ���� Ŭ���������� �ٲ�� ������ ���ؾ� ��.
CPageInfo* CARcBridgeDrawingStd::GetPageInfoOrg(CString strCategory, long nCategoryPageNum)
{
	CString str = _T("");
	long nCPN = 0;
	long i=0; for(i=0; i<GetCountTotalPage(); i++)
	{
		str = GetTitleCategory(m_pPageInfo[i]->m_nCategory);
		if(str!=strCategory)
			continue;

		nCPN++;
		if(nCPN==nCategoryPageNum)
			return m_pPageInfo[i];
	}

	return NULL;
}

// �Է��� �����Ϳ� ���� �� ���� ��¼��� ���Ѵ�.
long CARcBridgeDrawingStd::GetCountTotalPage()
{
	return m_pPageInfo.GetSize();
}

// ���� ���� Ÿ�Կ��� ������ ��ȣ�� ã�´�.(�������ΰ��� �ƴ�)
long CARcBridgeDrawingStd::GetPageNum(CString strCategory, HTREEITEM hItem)
{
	if(hItem==NULL) return 0;

	CString str = _T("");
	long nPage = 0;
	long i=0; for(i=0; i<GetCountTotalPage(); i++)
	{
		if(m_pPageInfo[i]->m_hItem==hItem)
		{
			nPage++;
			return nPage;
		}

		str = GetTitleCategory(m_pPageInfo[i]->m_nCategory);
		if(str==strCategory)
			nPage++;
	}

	return nPage;
}

long CARcBridgeDrawingStd::GetPageNum(CPageInfo* pInfo)
{
	long nPageNum = 0;
	long i=0; for(i=0; i<GetCountTotalPage(); i++)
	{
		nPageNum++;
		if(m_pPageInfo[i]==pInfo)
			break;
	}

	return nPageNum;
}

long CARcBridgeDrawingStd::GetCountCategoryPage(CString strCategory)
{
	CString str = _T("");
	long nCountCategoryPage	= 0;
	long i=0; for(i=0; i<GetCountTotalPage(); i++)
	{
		str = GetTitleCategory(m_pPageInfo[i]->m_nCategory);
		if(str==strCategory)
			nCountCategoryPage++;
	}

	return nCountCategoryPage;
}

long CARcBridgeDrawingStd::GetCountCategory()
{
	return m_pDrawPageMng->GetPagePartSize();
}

CString CARcBridgeDrawingStd::GetTitleCategory(int nCategory)
{
	return m_pDrawPageMng->GetPage(nCategory)->GetPartTitle();
}

// �� ������ ī�װ��� ���� ��
long CARcBridgeDrawingStd::GetCountBriCategoryPage(long nBri, CString strCategory)
{
	CString str = _T("");
	long nCountPage = 0;
	long i=0; for(i=0; i<GetCountTotalPage(); i++)
	{
		str = GetTitleCategory(m_pPageInfo[i]->m_nCategory);
		if(str==strCategory && m_pPageInfo[i]->m_nBri==nBri)
			nCountPage++;
	}

	return nCountPage;
}

// ī�װ����� �ڽ��� �������� ���° ������ ���� �˾Ƴ���.
long CARcBridgeDrawingStd::GetPageNumCategory(CPageInfo* pInfo)
{
	if(pInfo==NULL)
		return -1;

	long nPageNum = 0;
	long i=0; for(i=0; i<GetCountTotalPage(); i++)
	{
		if(m_pPageInfo[i]->m_nCategory==pInfo->m_nCategory)
		{
			nPageNum++;
			if(m_pPageInfo[i]==pInfo)
				break;
		}
	}

	return nPageNum;
}

// ������ ��ȣ�� ���� ��ȣ�� ����
long CARcBridgeDrawingStd::GetBridgeNumber(long nSubPage)
{
	long nBri = m_pPageInfo[nSubPage-1]->m_nBri;

	return nBri;
}

// ������ ��ȣ�� ī�װ� ��ȣ�� ����
long CARcBridgeDrawingStd::GetCategoryNumber(long nSubPage)
{
	long nCategory = m_pPageInfo[nSubPage-1]->m_nCategory;

	return nCategory;
}

long CARcBridgeDrawingStd::GetCategorySttPage(long nCategory)
{
	long nSttPage = 0;
	long i=0; for(i=0; i<GetCountTotalPage(); i++)
	{
		if(m_pPageInfo[i]->m_nCategory<nCategory)
			nSttPage++;
	}

	return nSttPage;
}

// ���� ������ ���ϱ�
// nSubPage : m_pPageInfo�� �迭 ��ȣ�� ����.
long CARcBridgeDrawingStd::GetOrgPage(long nSubPage)
{
	if(nSubPage<0 || nSubPage>=GetCountTotalPage())
		return 0;

	CString str = _T("");//, strCategory = _T("");
	long nOrgPage = 0;
	CPageInfo* pInfo = m_pPageInfo[nSubPage];
	CString strCategory = GetTitleCategory(pInfo->m_nCategory);

	int nCountCate = GetCountCategory();
	int i=0; for(i=0; i<nCountCate; i++)
	{
		str = GetTitleCategory(i);
		if(str==strCategory)	// ���� ī�װ����
		{
			nOrgPage += GetPageNumCategory(pInfo);
			break;
		}

		nOrgPage += GetCountCategoryPage(str);	// �� Ÿ�Ժ� ��ü �������� ���Ѵ�.
	}

	return nOrgPage;
}

// ���� ������ ���ϱ�
// nOrgPage	: ��ü �������� ��������( ���� Ÿ�� ������� ���� ��������ȣ )
long CARcBridgeDrawingStd::GetSubPage(long nOrgPage)
{
	long nSubPage = 0;
	long nPage = 0;
	long nCountCategoryPage = 0;
	CString str = _T(""), strCategory = _T("");

	// ���� Ÿ���� nOrgPage�� ���° �����ȣ ���� ���Ѵ�.
	int nCountCate = GetCountCategory();
	int i=0; for(i=0; i<nCountCate; i++)
	{
		str = GetTitleCategory(i);
		nPage += GetCountCategoryPage(str);
		if(nOrgPage<=nPage)
		{
			nOrgPage -= nCountCategoryPage;
			strCategory = str;
			break;
		}

		nCountCategoryPage = nPage;
	}

	nPage = 0;
	// ���� Ÿ���� ������ ���� nOrgPage ��°�� ���� ��ȣ�� ���Ѵ�.
	for(i=0; i<GetCountTotalPage(); i++)
	{
		str = GetTitleCategory(m_pPageInfo[i]->m_nCategory);
		if(str==strCategory)
		{
			nPage++;
			if(nPage==nOrgPage)
			{
				nSubPage = i;
				break;
			}
		}
	}

	return nSubPage;
}

long CARcBridgeDrawingStd::GetSubPageSttCategory(long nBri, CString strCategory)
{
	CString str =_T("");
	long nSubPage = 0;
	long i=0; for(i=0; i<GetCountTotalPage(); i++)
	{
		str = GetTitleCategory(m_pPageInfo[i]->m_nCategory);
		if(m_pPageInfo[i]->m_nBri==nBri && str==strCategory)
		{
			nSubPage = i;
			break;
		}
	}

	return nSubPage;
}

// long CARcBridgeDrawingStd::GetCountBridgePage(long nBri)
// {
// 	CRcBridgeApp* pBri = m_pARcBridgeDataStd->GetBridge(nBri);
// 	if(pBri==NULL)
// 		return 0;
// 
// 	long nPage = 0;
// 	long i=0; for(i=0; i<GetCountTotalPage(); i++)
// 	{
// 		if(m_pPageInfo[i]->m_nBri==nBri)
// 			nPage++;
// 	}
// 
// 	return nPage;
// }

// nSubPage�� �����ִ� ī�װ��� ù��° ������ ���ؿ���
// ���� ������ ��ȣ�� return
long CARcBridgeDrawingStd::GetPageFirstCategory(long nSubPage)
{
	long nCategory = m_pPageInfo[nSubPage-1]->m_nCategory;
	CString str = GetTitleCategory(nCategory);
	long nBri = GetBridgeNumber(nSubPage);
	long nPage = GetSubPageSttCategory(nBri, str);

	return GetOrgPage(nPage);
}

// nSubPage�� �����ִ� ī�װ��� ������ ������ ���ؿ���
// ���� ������ ��ȣ�� return
long CARcBridgeDrawingStd::GetPageLastCategory(long nSubPage)
{
	long nCategory = m_pPageInfo[nSubPage-1]->m_nCategory;
	CString str = GetTitleCategory(nCategory);
	long nBri = GetBridgeNumber(nSubPage);
	long nPage = GetSubPageSttCategory(nBri, str) + 1;
	long nCountCategoryPage = GetCountBriCategoryPage(nBri, str);

	return GetOrgPage(nPage+nCountCategoryPage-2);
}

// ��ö�� ������ ������ ���� ǥ����Ŀ� ���� �ٲ� �� �ִ�.
long CARcBridgeDrawingStd::GetCountPageMainRebarAssm(long nBri)
{
	CRcBridgeApp* pBri = m_pARcBridgeDataStd->GetBridge(nBri);
	if(!pBri) return 0;
	
	CARcBridgeDrawingOptionStd* pOpt = m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData = pOpt->GetDrawingOption(nBri);
	if(!pData) return 0;

	long nCount	= 0;
	long nType	= pData->m_nTypeMainRebarAssm;

	// 2Cycle�϶�
	if(pBri->GetCountCycleMainRebar() == 2)
	{
		switch(nType)
		{
		case 0: nCount	= 2; break;
		case 1: nCount	= 1; break;
		default: nCount	= 0; break;
		}
	}
	// 4Cycle�϶�.
	else
	{
		switch(nType)
		{
		case 0: nCount	= 3; break;
		case 1: nCount	= 2; break;
		default: nCount	= 0; break;
		}
	}

	return nCount;
}

// ������ü �������� �� ������..
// ���� ��ü�� ������, ��ս��ΰŵ� �ִ�. �� ������ �� �ٸ��� ��µ�(�Ŵ� �󼼵���..)
// BOOL bFrontOfRebarBMMain : ��üö�����ǥ ������ ������ ��ü����? ���Ŀ� ������ ��ü����?(�ɼǿ� ���� �ٸ�)
long CARcBridgeDrawingStd::GetCountPageInWallRebar(long nBri, BOOL bFrontOfRebarBMMain)
{
	CRcBridgeApp* pBri = m_pARcBridgeDataStd->GetBridge(nBri);
	if(!pBri) return 0;
	CARcBridgeDrawingOptionStd* pOpt = m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData = pOpt->GetDrawingOption(nBri);
	if(!pData) return 0;


	long nResultOfFront	= 0;
	long nResultOfBack	= 0;
	long nCount			= pBri->GetCountInWall();

	long i = 0; for(i = 0; i < nCount; i++)
	{
		CWallApp *pWall	= pBri->GetInWall(i);
		if(pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
		{
			if(i+1 == pBri->GetJijumConnectRibAndSlab()) continue;
		}

		if(pWall)
		{
			if(pWall->m_bIs)
			{
				if(pData->m_bSeparateRebarBMColumn && pWall->m_bTopIsColumn)
					nResultOfBack+=2;
				else
					nResultOfFront++;
			}
		}
	}

	return bFrontOfRebarBMMain ? nResultOfFront : nResultOfBack;
}

// ���ӽ����� ������ ������ �� ����
long CARcBridgeDrawingStd::GetCountPageApsRebar(long nBri)
{
	CRcBridgeApp* pBri = m_pARcBridgeDataStd->GetBridge(nBri);
	if(!pBri) return 0;

	long nCount	= 0;
	if(pBri->IsOutWall(TRUE))
	{
		if(pBri->m_apsStt[iLEFT].m_bIs) nCount++;
		if(pBri->m_apsStt[iRIGHT].m_bIs) nCount++;
	}

	if(pBri->IsOutWall(FALSE))
	{
		if(pBri->m_apsEnd[iLEFT].m_bIs) nCount++;
		if(pBri->m_apsEnd[iRIGHT].m_bIs) nCount++;
	}

	return nCount;
}

// ������ ������ ������ ��
long CARcBridgeDrawingStd::GetCountPageWingWallRebar(long nBri)
{
	CRcBridgeRebar *pBri = m_pARcBridgeDataStd->GetBridgeRebar(nBri);
	if(!pBri) return 0;
	CARcBridgeDrawingOptionStd* pOpt = m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData = pOpt->GetDrawingOption(nBri);
	if(!pData) return 0;

	CWingWall* pWingBL = pBri->GetWingWall(TRUE, TRUE);
	CWingWall* pWingER = pBri->GetWingWall(FALSE, FALSE);
	CWingWall* pWingBR = pBri->GetWingWall(TRUE, FALSE);
	CWingWall* pWingEL = pBri->GetWingWall(FALSE, TRUE);

	long nCount = 0;
	if(pWingBL->m_bExist)
		nCount++;
	if(pWingER->m_bExist)
		nCount++;
	if(pWingBR->m_bExist)
		nCount++;
	if(pWingEL->m_bExist)
		nCount++;

	// ���ǥ�� ���� ����ؾ� �ǹǷ� ���徿 ����Ѵ�.
	// ���忡 ���� ����ϹǷ� �ּ� ó��
//	if(pData->m_bSeparateRebarBMWing)
//		nCount *= 2;

	return nCount;
}

// ��Ÿ ���� ������ ��
// 1. ��ȣ��(������ 1��)
// 2. ������(������ ��� 1��)
// 3. ����Ÿ������(���Ҵ��)
long CARcBridgeDrawingStd::GetCountPageOther(long nBri, long nCountEHP)
{
	CRcBridgeRebar *pBri = m_pARcBridgeDataStd->GetBridgeRebar(nBri);
	if(!pBri) return 0;

	
	long nCountGuardWall	= 1;
	long nCountMoun			= pBri->m_nMonumenu>0 ? 1 : 0;
	long nCountHyunTa		= 0;

	long nCountFooting	= pBri->GetCountFooting();
	long i = 0; for(i = 0; i < nCountFooting; i++)
	{
		CFootingApp *pFooting	= pBri->GetFooting(i);
		if(!pFooting) continue;
		if(!pFooting->m_bIs) continue;
		if(pFooting->m_exFooting.m_nType != EXFOOTING_TYPE_PILE) continue;
		if(pFooting->m_exFooting.m_Pile.m_nType == PILE_TYPE_HYUNJANG) 
			nCountHyunTa++;
	}


	return nCountGuardWall + nCountMoun + nCountHyunTa + nCountEHP;
}

// ���� ���� �ϴܿ� ��Ʈ ����
void CARcBridgeDrawingStd::DrawNoteBottomOfDomyun(CDomyun *pDomP, CString sNote)
{
	CDomyun Dom(pDomP);

	pDomP->SetCalcExtRect();
	CDRect rc = pDomP->GetExtRect();

	m_pARoadOptionStd->SetEnvType(&Dom, HCAD_TXTG);
	Dom.SetTextAlignHorz(TA_RIGHT);
	Dom.TextOut(rc.right, rc.top-Dom.GetTextHeight(), sNote);

	*pDomP << Dom;
}

// sTitle�� �ɼǿ� ���� ������ �������� ����
CString CARcBridgeDrawingStd::GetStringTitle(CRcBridgeApp *pBri, CString sTitle)
{
	if(!pBri) return sTitle;

	if(pBri->m_nSeparBri == 0)	// ��ü
	{
		switch(m_pARcBridgeDrawingOptionStd->m_nTypeTitleBridge)
		{
		case 0: sTitle	= sTitle; break;
		case 1: sTitle	= pBri->m_strBridgeName + " " + sTitle;
		}
	}
	else if(pBri->m_nSeparBri == 1)	// ���༱(�����ݴ����)
	{
		switch(m_pARcBridgeDrawingOptionStd->m_nTypeTitleSeparatorBridge)
		{
		case 0: sTitle	= sTitle; break;
		case 1: sTitle	= pBri->m_strBridgeName + " " + sTitle; break;
		case 2: sTitle	= pBri->m_strBridgeName + " " + m_pARcBridgeDataStd->m_strLineRevDirName + " " + sTitle; break;
		}
	}
	else if(pBri->m_nSeparBri == 2)	// ���༱(��������)
	{
		switch(m_pARcBridgeDrawingOptionStd->m_nTypeTitleSeparatorBridge)
		{
		case 0: sTitle	= sTitle; break;
		case 1: sTitle	= pBri->m_strBridgeName + " " + sTitle; break;
		case 2: sTitle	= pBri->m_strBridgeName + " " + m_pARcBridgeDataStd->m_strLineDirName + " " + sTitle; break;
		}
	}

	
	return sTitle;
}

// Ŀ�÷� �󼼵� ���̺� �׸���
void CARcBridgeDrawingStd::DrawTableCouplerDetail(CDomyun *pDom, CTypedPtrArray <CObArray, CRebar*> *pArrRebar)
{
	CARcBridgeDrawingOptionStd *pOpt	= m_pARcBridgeDrawingOptionStd;
	if(!pOpt) return;
	CRcBridgeRebar bri;

	
	CGridDomyunEx Dom(pDom);
	m_pARoadOptionStd->SetEnvType(&Dom,HCAD_TBL1);
	Dom.SetScaleDim(pDom->GetScaleDim());

//	BOOL bCouplerMain = FALSE;
// 	BOOL bCouplerRound = FALSE;
	CDPointArray xyArrRoundRebar, xyArrMainRebar;
	bri.CalcCouplerRebar(xyArrRoundRebar, xyArrMainRebar, pArrRebar);

	BOOL bCouplerMain	= xyArrMainRebar.GetSize() > 0;
	BOOL bCouplerRound	= xyArrRoundRebar.GetSize() > 0;

 	long nRowSu = 4;
	long nColSu = 8;
	if(bCouplerMain) nRowSu ++;
	if(bCouplerRound) nRowSu ++;
	/////////////////////////////////////////////
	//���̺� ȯ�� ����
	Dom.SetBasePoint(CDPoint(0,0));
	Dom.SetColumnCount(nColSu);
	Dom.SetRowCount(nRowSu);
	Dom.SetRowHeightAll(Dom.Always(8));
	Dom.SetColumnWidthAll(Dom.Always(30));

	long nRow = 0;
	long nCol = 0;
	double dColumnWidth = 0;
	Dom.SetColumnWidth(nCol++, Dom.Always(40));
	dColumnWidth += 40;
	long i = 0; for(i = 1; i < nColSu; i++)
	{
		Dom.SetColumnWidth(nCol++, Dom.Always(10));
		dColumnWidth += 10;
	}

	Dom.SetBasePoint(CDPoint(-Dom.Always(dColumnWidth/2), 0));

	double dDia[7] = {13, 16, 19, 22, 25, 29, 32};

	nCol = 0;
	Dom.SetTextMatrix(nRow, nCol++, "�� ��");
	Dom.SetTextMatrix(nRow, nCol++, "D13");
	Dom.SetTextMatrix(nRow, nCol++, "D16");
	Dom.SetTextMatrix(nRow, nCol++, "D19");
	Dom.SetTextMatrix(nRow, nCol++, "D22");
	Dom.SetTextMatrix(nRow, nCol++, "D25");
	Dom.SetTextMatrix(nRow, nCol++, "D29");
	Dom.SetTextMatrix(nRow, nCol, "D32");

	nCol = 0;
	nRow++;
	Dom.SetTextMatrix(nRow++, nCol, "Ŀ�÷� �ܰ�(D)");
	Dom.SetTextMatrix(nRow++, nCol, "ü���� ����(L)");
	Dom.SetTextMatrix(nRow++, nCol, "��       ��(T)");
	if(bCouplerMain) Dom.SetTextMatrix(nRow++, nCol, "��ö�� Ŀ�÷� ����");
	if(bCouplerRound) Dom.SetTextMatrix(nRow++, nCol, "��ö�� Ŀ�÷� ����");

	nCol = 1;
	nRow = 1;
	Dom.SetTextMatrix(nRow, nCol++, "26");
	Dom.SetTextMatrix(nRow, nCol++, "34");
	Dom.SetTextMatrix(nRow, nCol++, "41");
	Dom.SetTextMatrix(nRow, nCol++, "46");
	Dom.SetTextMatrix(nRow, nCol++, "51");
	Dom.SetTextMatrix(nRow, nCol++, "55");
	Dom.SetTextMatrix(nRow, nCol, "60");
	
	nCol = 1;
	nRow++;
	Dom.SetTextMatrix(nRow, nCol++, "45");
	Dom.SetTextMatrix(nRow, nCol++, "55");
	Dom.SetTextMatrix(nRow, nCol++, "65");
	Dom.SetTextMatrix(nRow, nCol++, "70");
	Dom.SetTextMatrix(nRow, nCol++, "80");
	Dom.SetTextMatrix(nRow, nCol++, "85");
	Dom.SetTextMatrix(nRow, nCol, "90");

	nCol = 1;
	nRow++;
	Dom.SetTextMatrix(nRow, nCol++, "6.5");
	Dom.SetTextMatrix(nRow, nCol++, "9");
	Dom.SetTextMatrix(nRow, nCol++, "11");
	Dom.SetTextMatrix(nRow, nCol++, "12");
	Dom.SetTextMatrix(nRow, nCol++, "13");
	Dom.SetTextMatrix(nRow, nCol++, "13");
	Dom.SetTextMatrix(nRow, nCol, "14");

	// ��ö�� Ŀ�÷� ����
	
	CString str = _T("");
	
	if(bCouplerMain)
	{
		nRow++;
		CDPoint xy(0, 0);
		long i = 0; for(i = 0; i < 7; i++)
		{
			for(long j = 0; j < xyArrMainRebar.GetSize(); j++)
			{
				xy = xyArrMainRebar.GetAt(j);
				if(Compare(dDia[i], xy.y, "="))
				{
					str.Format("%d", (long)xy.x);
					Dom.SetTextMatrix(nRow, i+1, str);
				}
			}
		}
	}
	

	// ��ö�� Ŀ�÷� ����
	if(bCouplerRound) 
	{
		nRow++;
		CDPoint xy(0, 0);
		
		long i = 0; for(i = 0; i < 7; i++)
		{
			for(long j = 0; j < xyArrRoundRebar.GetSize(); j++)
			{
				xy = xyArrRoundRebar.GetAt(j);
				if(Compare(dDia[i], xy.y, "="))
				{
					str.Format("%d", (long)xy.x);
					Dom.SetTextMatrix(nRow, i+1, str);
				}
			}
		}
	}


	Dom.Draw();
	m_pARoadOptionStd->SetEnvType(&Dom, HCAD_TBL2);
	Dom.DrawOutLine();
	if(pOpt->m_dwDrawCommon & COMMON_TABLESHADE) Dom.DrawOutLineShade();


	// note
	Dom.SetCalcExtRect();
	CDRect rect = Dom.GetExtRect();
	CStringArray sArrNote;
	sArrNote.Add("* NOTE : COUPLER�� �����ϱ� ���� ���� SLIP�� ����");
	sArrNote.Add("        �������θ� ������ �� ����� ����������");
	sArrNote.Add("        �����Ͽ� ���� �� ����ؾ� �Ѵ�");
	*pDom << Dom;

	
	m_pARoadOptionStd->SetEnvType(&Dom,HCAD_TBL1);
	Dom.SetTextAlignHorz(TA_LEFT);
	Dom.SetTextHeight(3);
	for(i = 0; i < sArrNote.GetSize(); i++)
	{
		Dom.TextOut(rect.left, rect.top-Dom.Always(7*i+15), sArrNote.GetAt(i));
	}

	*pDom << Dom;
}

CDRect CARcBridgeDrawingStd::DrawCouplerDetail(CDomyun *pDom, CTypedPtrArray <CObArray, CRebar*> *pArrRebar)
{
	CRcBridgeRebar bri;
	CDPointArray xyArrMain, xyArrRound;
	bri.CalcCouplerRebar(xyArrRound, xyArrMain, pArrRebar);
	if(xyArrRound.GetSize() == 0 && xyArrMain.GetSize() == 0) return CDRect(0, 0, 0, 0);

	CDomyun Dom(pDom);
	
	CString strDxfName;
	CString strDxfFileName;
	CString szPath = AfxGetApp()->GetProfileString("Afx Directory", "Fixed Directory");
	strDxfFileName	   = "Ŀ�÷��󼼵�.dxf";
	strDxfName.Format("%s\\dxf\\%s",szPath,strDxfFileName);
	
	if(!Dom.DxfIn(strDxfName)) AfxMessageBox("�󼼵� �������� ����!");

	*pDom << Dom;

	Dom.SetCalcExtRect();

	return Dom.GetExtRect();
}


double CARcBridgeDrawingStd::GetRebarListByFy(CTypedPtrArray <CObArray, CRebar*> *pArrRebarOrg, CTypedPtrArray <CObArray, CRebar*> *pArrRebarGet, double dHigherThanFy)
{
	if(!pArrRebarOrg) return dHigherThanFy;
	if(!pArrRebarGet) return dHigherThanFy;

	pArrRebarGet->RemoveAll();

	// dHigherThanFy���� ū �� �߿� ���� ���� fy�� �����
	double dFy	= 0;
	BOOL bFirst	= TRUE;
	long i = 0; for(i = 0; i < pArrRebarOrg->GetSize(); i++)
	{
		CRebar *pRB	= pArrRebarOrg->GetAt(i);

		if(pRB->m_Fy > dHigherThanFy)
		{
			if(bFirst || pRB->m_Fy < dFy)
			{
				dFy	= pRB->m_Fy;
				bFirst	= FALSE;
			}
		}
	}

	// dFY�ΰ� �����
	for(i = 0; i < pArrRebarOrg->GetSize(); i++)
	{
		CRebar *pRB	= pArrRebarOrg->GetAt(i);

		if(Compare(pRB->m_Fy, dFy, "="))
		{
			pArrRebarGet->Add(pRB);
		}
	}

	return dFy;

}

void CARcBridgeDrawingStd::MakePage()
{
	// ABD-1522
	// ������� ����� ������� ����..
	// �ʿ�� �ٸ� ������� ���� : �����ڷ� - ������ ǥ�ص� �۾�
	return;

	m_pDrawPageMng->ResetAll();
	if(m_pARcBridgeDataStd->m_nTypeProcess == TYPE_PROCESS_NORMAL)
	{
		m_pDrawPageMng->AddOnePage(new CDrawingVerticalPlane, _T("����鵵"), this);
		m_pDrawPageMng->AddOnePage(new CDrawingBoring, _T("���� �ֻ�"), this);
		m_pDrawPageMng->AddOnePage(new CDrawingGeneral, _T("�Ϲݵ�"), this);
		m_pDrawPageMng->AddOnePage(new CDrawingRebar, _T("������"), this);
		m_pDrawPageMng->AddOnePage(new CDrawingAps, _T("���� ������"), this);
		m_pDrawPageMng->AddOnePage(new CDrawingCns, _T("���� ������"), this);
		m_pDrawPageMng->AddOnePage(new CDrawingOthers, _T("��Ÿ ����"), this);
		m_pDrawPageMng->AddOnePage(new CDrawingUser, _T("����� ����"), this);
	}
	else
	{
		m_pDrawPageMng->AddOnePage(new CDrawingSlabStandardOutput, TITLE_SLAB_STANDARD_OUTPUT, this);
		m_pDrawPageMng->AddOnePage(new CDrawingUser, _T("����� ����"), this);
	}
}
