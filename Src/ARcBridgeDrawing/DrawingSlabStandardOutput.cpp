// DrawingSlabStandardOutput.cpp: implementation of the CDrawingSlabStandardOutput class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../../Src/ARcBridgeData/ARcBridgeData.h"
#include "../../Src/ARcBridgeCalculate/ARcBridgeCalculate.h"
#include "../../Src/ARcBridgeDBDraw/ARcBridgeDBDraw.h"
#include "../ARcBridgeDrawingOption/ARcBridgeDrawingOption.h"
#include "ARcBridgeDrawing.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define MPE_RCSTD1_DESIGN_CONDITION			10		// ����1 - ��������
#define MPE_RCSTD1_CROSS					20		// ����1 - Ⱦ�ܸ鵵
#define MPE_RCSTD1_CROSS_TITLE				30		// ����1 - Ⱦ�ܸ鵵 ����
#define MPE_RCSTD1_PLAN						40		// ����1 - ��鵵
#define MPE_RCSTD1_PLAN_TITLE				50		// ����1 - ��鵵 ����
#define MPE_RCSTD1_STANDARD_OUTLINE			60		// ����1 - ǥ�شܸ鵵 �ܰ�
#define MPE_RCSTD1_NOTE						70		// ����1 - ��Ʈ

#define MPE_RCSTD2_STANDARD					90		// ����2 - ǥ�شܸ鵵
#define MPE_RCSTD2_STANDARD_TITLE			100		// ����2 - ǥ�شܸ鵵
#define MPE_RCSTD2_BMD						110		// ����2 - BMD
#define MPE_RCSTD2_BMD_TITLE				120		// ����2 - BMD ����
#define MPE_RCSTD2_SFD						130		// ����2 - SFD
#define MPE_RCSTD2_SFD_TITLE				140		// ����2 - SFD ����
#define MPE_RCSTD2_MAIN_ASSM_MAINTITLE		150		// ����2 - ��ö��������1 ��������
#define MPE_RCSTD2_MAIN_ASSM1				160		// ����2 - ��ö��������1
#define MPE_RCSTD2_MAIN_ASSM1_TITLE			170		// ����2 - ��ö��������1 ����
#define MPE_RCSTD2_MAIN_ASSM2				180		// ����2 - ��ö��������2
#define MPE_RCSTD2_MAIN_ASSM2_TITLE			190		// ����2 - ��ö��������2
#define MPE_RCSTD2_PLAN						200		// ����2 - ��鵵
#define MPE_RCSTD2_PLAN_TITLE				210		// ����2 - ��鵵 ����
#define MPE_RCSTD2_OBTUS_UPPER				220		// ����2 - �а��λ��
#define MPE_RCSTD2_OBTUS_UPPER_TITLE		230		// ����2 - �а��λ�� ����
#define MPE_RCSTD2_OBTUS_LOWER				240		// ����2 - �а����ϸ�
#define MPE_RCSTD2_OBTUS_LOWER_TITLE		250		// ����2 - �а����ϸ� ����
#define MPE_RCSTD2_OBTUS_SECTION			260		// ����2 - �а��δܸ�
#define MPE_RCSTD2_OBTUS_SECTION_TITLE		270		// ����2 - �а��δܸ� ����

#define MPE_RCSTD3_GIRDER_DETAIL			290		// ����3 - �Ŵ���
#define MPE_RCSTD3_GIRDER_DETAIL_TITLE		300		// ����3 - �Ŵ��� ����
#define MPE_RCSTD3_GUARDWALL_DETAIL			310		// ����3 - ��ȣ����
#define MPE_RCSTD3_GUARDWALL_DETAIL_TITLE	320		// ����3 - ��ȣ���� ����
#define MPE_RCSTD3_DETAIL_OUTLINE			330		// ����3 - �󼼵� �ܰ�
#define MPE_RCSTD3_NOTE						340		// ����3 - ��Ʈ
#define MPE_RCSTD3_SPACER					350		// ����3 - ����������
#define MPE_RCSTD3_SPACER_TITLE				360		// ����3 - ���������� ����
#define MPE_RCSTD3_DESIGN_METHOD			370		// ����3 - ������

#define MPE_RCSTD4_REBAR_DETAIL				390		// ����4 - ö�ٻ�
#define MPE_RCSTD4_REBAR_DETAIL_TITLE		400		// ����4 - ö�ٻ� ����
#define MPE_RCSTD4_REBAR_TABLE				410		// ����4 - ö�����ǥ
#define MPE_RCSTD4_REBAR_TABLE_TITLE		420		// ����4 - ö�����ǥ ����



CDrawingSlabStandardOutput::CDrawingSlabStandardOutput()
{

}

CDrawingSlabStandardOutput::~CDrawingSlabStandardOutput()
{

}

long CDrawingSlabStandardOutput::GetPageSu()
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;

	if(pStd->GetCountTotalPage()<=0)
		pStd->MakePageInfo();

	m_nPage	= pStd->GetCountCategoryPage(TITLE_SLAB_STANDARD_OUTPUT);

	return m_nPage;
}

CString CDrawingSlabStandardOutput::GetTitle()
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;
	CPageInfo* pInfo = pStd->GetPageInfoOrg(TITLE_SLAB_STANDARD_OUTPUT, pStd->m_nCurPage-m_nSttPage);
	if(pInfo==NULL) return _T("");

	CString strTitle = pStd->GetStringTitle(pStd->m_pARcBridgeDataStd->GetBridge(m_nBridge), pInfo->m_strTitle);

	return strTitle;
}
double CDrawingSlabStandardOutput::GetScale()
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;
	CARcBridgeDrawingOptionStd* pOpt = pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pOptData = pOpt->GetDrawingOption(m_nBridge);

	CPageInfo* pInfo = pStd->GetPageInfoOrg(TITLE_SLAB_STANDARD_OUTPUT, pStd->m_nDrawPage-m_nSttPage);

	long nPageNum	= pInfo->m_nPageNumber;

	m_Scale	= 100;
	if(nPageNum == 0)
		m_Scale	= pOptData->m_dScaleRcStd1Base;
	else if(nPageNum == 1)
		m_Scale	= pOptData->m_dScaleRcStd2Base;
	else if(nPageNum == 2)
		m_Scale	= pOptData->m_dScaleRcStd3Base;
	else if(nPageNum == 3)
		m_Scale	= 50;
	
	return m_Scale;
}
void CDrawingSlabStandardOutput::SetNumber(long nSubPage)
{
	m_nBridge = GetNumberBridge(nSubPage-m_nSttPage);
}
void CDrawingSlabStandardOutput::DrawOnePage()
{
	TRACE("CDrawingSlabStandardOutput");
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;
	CARcBridgeDrawingOptionStd* pOpt = pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDataStd* pDataStd = pStd->m_pARcBridgeDataStd;
	CDrawDogak* pDgk = pStd->GetDogak();	
	CDrawPageMng* pMng = pStd->GetPageManage();

	long nPage = pStd->m_nCurPage;

	SetNumber(pStd->m_nDrawPage);
	double dScale = GetScale();

	CDomyun* pDom = pStd->GetDomyun(nPage);
	pDom->SetScaleDim(dScale);

	CPageInfo* pInfo = pStd->GetPageInfoOrg(TITLE_SLAB_STANDARD_OUTPUT, pStd->m_nDrawPage-m_nSttPage);
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


	// rc�����걳 ǥ�ص� ���
	long nPageNum	= pInfo->m_nPageNumber;	// �ε���

	if(nPageNum == 0)	// RC�����걳 ǥ�� ���� ���(1)
	{
		AddPageOne1(&Dom, rcDogak);
	}
	else if(nPageNum == 1)	// RC�����걳 ǥ�� ���� ���(2)
	{
		AddPageOne2(&Dom, rcDogak);
	}
	else if(nPageNum == 2)	// RC�����걳 ǥ�� ���� ���(3)
	{
		AddPageOne3(&Dom, rcDogak);
	}
	else if(nPageNum == 3)
	{
		AddPageOne4(&Dom, rcDogak);
	}
	

	pMng->SetProgressData(_T("��� �Ϸ�  ..."), 100);

	*pDom << Dom;
}

// ���� �׷��� �Ǵ� RcBridge����
CRcBridgeRebar* CDrawingSlabStandardOutput::GetRcBridgeRebar()
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

long CDrawingSlabStandardOutput::GetNumberBridge(long nCategoryPageNum)
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;
	CPageInfo* pInfo = pStd->GetPageInfoOrg(TITLE_SLAB_STANDARD_OUTPUT, nCategoryPageNum);
	if(pInfo==NULL) return -1;

	return pInfo->m_nBri;
}


void CDrawingSlabStandardOutput::AddPageOne1(CDomyun *pDomP, CDRect rcDogak)
{
	CARcBridgeDrawingStd* pStd			= (CARcBridgeDrawingStd*)m_pStd;
	CRcBridgeRebar *pBri				= GetRcBridgeRebar();
	CARcBridgeDrawingOptionStd* pOpt	= pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData	= pOpt->GetDrawingOption(m_nBridge);
	CDrawPageMng* pPageMng				= pStd->GetPageManage();
	COutMovePoint* pMove				= pStd->GetMovePoint(pStd->m_nCurPage);

	CARcBridgeDBStandard DBStd(pBri, pStd->m_pARoadOptionStd, pStd->m_pARcBridgeDataStd);
	DBStd.SetSelectFckDomText(pOpt->m_nSelectFckDomText);
	DBStd.m_pDrawingOptionData	= pData;
	double dScale	= GetScale();
	DWORD dOptDraw	= pData->m_dwDrawRcStd;

	BOOL bSkew	= pBri->IsSkew();

	CDomyun Dom(pDomP);
	Dom.SetScaleDim(dScale);

	CString str	= _T("");
	CDPoint xyMove(0, 0);
	CDRect rect(0, 0, 0, 0);
	int nCountMPE	= RCSTD1_COUNT;
	int Pos	= 0;
	int nOffsetPos	= 100 / nCountMPE;
	
	// ����1 - �������� //////////////////////////////////////////////////////////////////////
	CDRect rcDesignCondition(0, 0, 0, 0);	// ����1 - ��������
	pPageMng->SetProgressData(_T("�������� �����..."), Pos+=nOffsetPos);
	if(dOptDraw & RCSTD1_DESIGN_CONDITION)
	{
		if(m_pStd->IsDrawByBlock(MPE_RCSTD1_DESIGN_CONDITION))
		{
			rcDesignCondition = DrawRcStd1DesignCondition(&Dom);
			xyMove	= CDPoint(rcDogak.left - rcDesignCondition.left + Dom.Always(50), rcDogak.bottom - rcDesignCondition.bottom - Dom.Always(100));
			Dom.Move(xyMove);
			rcDesignCondition.left	+= xyMove.x; rcDesignCondition.right	+= xyMove.x;
			rcDesignCondition.top		+= xyMove.y; rcDesignCondition.bottom	+= xyMove.y;
			Dom.SetCalcExtRect();
			rect	= Dom.GetExtRect();

			Dom.Move(pMove->GetPoint(MPE_RCSTD1_DESIGN_CONDITION));
			pStd->ChangeSeperateLayer(&Dom);
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_RCSTD1_DESIGN_CONDITION, _T("��������"));
			*pDomP << Dom;
		}

		/*
		if(m_pStd->IsDrawByBlock(MPE_MAINREBAR_ASSM_TITLE+nIdxMainRebarAssmPage))
		{
			pStd->AddTitleOnDom(&Dom, m_nBridge, _T("��������"), CDPoint(0,0), 0, dScale);
			Dom.Move(rect.left+rect.Width()/2.0, rect.bottom+GAP_TITLE);
			Dom.Move(pMove->GetPoint(MPE_RCSTD1_DESIGN_CONDITION));
			pStd->ChangeSeperateLayer(&Dom);
			Dom.SetCalcExtRect();
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_MAINREBAR_ASSM_TITLE+nIdxMainRebarAssmPage, _T("�������� Ÿ��Ʋ"));
			*pDomP << Dom;
		}
		*/
	}
	else
	{
		rcDesignCondition.left	= rcDogak.left + Dom.Always(20);
		rcDesignCondition.bottom	= rcDogak.bottom - Dom.Always(30);
		rcDesignCondition.right	= rcDesignCondition.left;
		rcDesignCondition.top		= rcDesignCondition.bottom;
	}
	////////////////////////////////////////////////////////////////////////////////////


	// ����1 - Ⱦ�ܸ鵵 //////////////////////////////////////////////////////////////////////
	CDRect rcStandard(0, 0, 0, 0);	// ����1 - Ⱦ�ܸ鵵
	pPageMng->SetProgressData(_T("Ⱦ�ܸ鵵 �����..."), Pos+=nOffsetPos);
	if(dOptDraw & RCSTD1_CROSS)
	{
		if(m_pStd->IsDrawByBlock(MPE_RCSTD1_CROSS))
		{
			rcStandard	= DrawRcStd1Cross(&Dom);
			if(bSkew)
				xyMove	= CDPoint(rcDogak.CenterPoint().x - rcStandard.CenterPoint().x + rcDogak.Width()/4, rcDogak.bottom - rcStandard.bottom - Dom.Always(150));
			else
				xyMove	= CDPoint(rcDogak.CenterPoint().x - rcStandard.CenterPoint().x + rcDogak.Width()/4, rcDogak.CenterPoint().y - rcStandard.CenterPoint().y);
			Dom.Move(xyMove);
			rcStandard.left	+= xyMove.x; rcStandard.right	+= xyMove.x;
			rcStandard.top		+= xyMove.y; rcStandard.bottom	+= xyMove.y;
			Dom.SetCalcExtRect();
			rect	= Dom.GetExtRect();

			Dom.Move(pMove->GetPoint(MPE_RCSTD1_CROSS));
			pStd->ChangeSeperateLayer(&Dom);
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_RCSTD1_CROSS, _T("Ⱦ�ܸ鵵"));
			*pDomP << Dom;
		}

		if(m_pStd->IsDrawByBlock(MPE_RCSTD1_CROSS_TITLE))
		{
			pStd->AddTitleOnDom(&Dom, m_nBridge, _T("Ⱦ�ܸ鵵"), CDPoint(0,0), 0, dScale);
			Dom.Move(rect.left+rect.Width()/2.0+Dom.GetTextWidth("Ⱦ�ܸ鵵"), rect.bottom+GAP_TITLE);
			Dom.Move(pMove->GetPoint(MPE_RCSTD1_CROSS_TITLE));
			pStd->ChangeSeperateLayer(&Dom);
			Dom.SetCalcExtRect();
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_RCSTD1_CROSS_TITLE, _T("Ⱦ�ܸ鵵 Ÿ��Ʋ"));
			*pDomP << Dom;
		}
	}
	else
	{
		rcStandard.left		= rcDogak.right - Dom.Always(20);
		rcStandard.bottom	= rcDogak.bottom - Dom.Always(30);
		rcStandard.right	= rcStandard.left;
		rcStandard.top		= rcStandard.bottom;
	}
	////////////////////////////////////////////////////////////////////////////////////


	// ����1 - ��鵵 //////////////////////////////////////////////////////////////////////
	CDRect rcPlan(0, 0, 0, 0);	// ����1 - ��鵵
	pPageMng->SetProgressData(_T("��鵵 �����..."), Pos+=nOffsetPos);
	if(dOptDraw & RCSTD1_PLAN && bSkew)
	{
		if(m_pStd->IsDrawByBlock(MPE_RCSTD1_PLAN))
		{
			rcPlan	= DrawRcStd1Plan(&Dom);
			xyMove	= CDPoint(rcDogak.CenterPoint().x - rcPlan.CenterPoint().x + rcDogak.Width()/4, rcStandard.top - rcPlan.bottom - Dom.Always(30));
			Dom.Move(xyMove);
			rcPlan.left	+= xyMove.x; rcPlan.right	+= xyMove.x;
			rcPlan.top		+= xyMove.y; rcPlan.bottom	+= xyMove.y;
			Dom.SetCalcExtRect();
			rect	= Dom.GetExtRect();

			Dom.Move(pMove->GetPoint(MPE_RCSTD1_PLAN));
			pStd->ChangeSeperateLayer(&Dom);
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_RCSTD1_PLAN, _T("��鵵"));
			*pDomP << Dom;
		}

		if(m_pStd->IsDrawByBlock(MPE_RCSTD1_PLAN_TITLE))
		{
			pStd->AddTitleOnDom(&Dom, m_nBridge, _T("��鵵"), CDPoint(0,0), 0, dScale);
			Dom.Move(rect.left+rect.Width()/2.0+Dom.GetTextWidth("��鵵"), rect.bottom+GAP_TITLE);
			Dom.Move(pMove->GetPoint(MPE_RCSTD1_PLAN_TITLE));
			pStd->ChangeSeperateLayer(&Dom);
			Dom.SetCalcExtRect();
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_RCSTD1_PLAN_TITLE, _T("��鵵 Ÿ��Ʋ"));
			*pDomP << Dom;
		}
	}
	else
	{
		rcPlan.left		= rcDogak.right - Dom.Always(20);
		rcPlan.bottom	= rcDogak.bottom - Dom.Always(30);
		rcPlan.right	= rcPlan.left;
		rcPlan.top		= rcPlan.bottom;
	}
	////////////////////////////////////////////////////////////////////////////////////
	
	// ����1 - ǥ�شܸ鵵 �ܰ� //////////////////////////////////////////////////////////////////////
	CDRect rcStandardOutLine(0, 0, 0, 0);	// ����1 - ǥ�شܸ鵵 �ܰ�
	pPageMng->SetProgressData(_T("ǥ�شܸ鵵 �ܰ� �����..."), Pos+=nOffsetPos);
	if(dOptDraw & RCSTD1_STANDARD_OUTLINE)
	{
		if(m_pStd->IsDrawByBlock(MPE_RCSTD1_STANDARD_OUTLINE))
		{
			rcStandardOutLine	= DrawRcStd1StandardOutLine(&Dom);
			xyMove	= CDPoint(rcDogak.CenterPoint().x - rcStandardOutLine.CenterPoint().x + rcDogak.Width()/4, rcDogak.CenterPoint().y - rcStandardOutLine.CenterPoint().y);
			Dom.Move(xyMove);
			rcStandardOutLine.left	+= xyMove.x; rcStandardOutLine.right	+= xyMove.x;
			rcStandardOutLine.top		+= xyMove.y; rcStandardOutLine.bottom	+= xyMove.y;
			Dom.SetCalcExtRect();
			rect	= Dom.GetExtRect();

			Dom.Move(pMove->GetPoint(MPE_RCSTD1_STANDARD_OUTLINE));
			pStd->ChangeSeperateLayer(&Dom);
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_RCSTD1_STANDARD_OUTLINE, _T("ǥ�شܸ鵵 �ܰ�"));
			*pDomP << Dom;
		}


	}
	else
	{
		rcStandardOutLine.left		= rcDogak.right - Dom.Always(20);
		rcStandardOutLine.bottom	= rcDogak.bottom - Dom.Always(30);
		rcStandardOutLine.right	= rcStandardOutLine.left;
		rcStandardOutLine.top		= rcStandardOutLine.bottom;
	}
	////////////////////////////////////////////////////////////////////////////////////
}

void CDrawingSlabStandardOutput::AddPageOne2(CDomyun *pDomP, CDRect rcDogak)
{
	CARcBridgeDrawingStd* pStd			= (CARcBridgeDrawingStd*)m_pStd;
	CRcBridgeRebar *pBri				= GetRcBridgeRebar();
	CARcBridgeDrawingOptionStd* pOpt	= pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData	= pOpt->GetDrawingOption(m_nBridge);
	CDrawPageMng* pPageMng				= pStd->GetPageManage();
	COutMovePoint* pMove				= pStd->GetMovePoint(pStd->m_nCurPage);

	CARcBridgeDBStandard DBStd(pBri, pStd->m_pARoadOptionStd, pStd->m_pARcBridgeDataStd);
	DBStd.SetSelectFckDomText(pOpt->m_nSelectFckDomText);
	DBStd.m_pDrawingOptionData	= pData;
	double dScale	= GetScale();
	DWORD dOptDraw	= pData->m_dwDrawRcStd;

	BOOL bSkew	= pBri->IsSkew();

	CDomyun Dom(pDomP);
	Dom.SetScaleDim(dScale);

	CString str	= _T("");
	CDPoint xyMove(0, 0);
	CDRect rect(0, 0, 0, 0);
	int nCountMPE	= RCSTD2_COUNT;
	int Pos	= 0;
	int nOffsetPos	= 100 / nCountMPE;


	// ����2 - ǥ�شܸ鵵 //////////////////////////////////////////////////////////////////////
	CDRect rcStandard(0, 0, 0, 0);	// ����2 - ǥ�شܸ鵵
	pPageMng->SetProgressData(_T("ǥ�شܸ鵵 �����..."), Pos+=nOffsetPos);
	if(dOptDraw & RCSTD2_STANDARD)
	{
		if(m_pStd->IsDrawByBlock(MPE_RCSTD2_STANDARD))
		{
			rcStandard	= DrawRcStd2Standard(&Dom);
			if(bSkew)
				xyMove	= CDPoint(rcDogak.CenterPoint().x - rcStandard.CenterPoint().x + rcDogak.Width()/4, rcDogak.top - rcStandard.top + Dom.Always(120));
			else
				xyMove	= CDPoint(rcDogak.CenterPoint().x - rcStandard.CenterPoint().x - rcDogak.Width()/4, rcDogak.bottom - rcStandard.bottom - Dom.Always(120));
			Dom.Move(xyMove);
			rcStandard.left	+= xyMove.x; rcStandard.right	+= xyMove.x;
			rcStandard.top		+= xyMove.y; rcStandard.bottom	+= xyMove.y;
			Dom.SetCalcExtRect();
			rect	= Dom.GetExtRect();

			Dom.Move(pMove->GetPoint(MPE_RCSTD2_STANDARD));
			pStd->ChangeSeperateLayer(&Dom);
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_RCSTD2_STANDARD, _T("ǥ�شܸ鵵"));
			*pDomP << Dom;
		}

		if(m_pStd->IsDrawByBlock(MPE_RCSTD2_STANDARD_TITLE))
		{
			pStd->AddTitleOnDom(&Dom, m_nBridge, _T("ǥ�شܸ鵵"), CDPoint(0,0), 0, dScale);
			Dom.Move(rect.left+rect.Width()/2.0, rect.bottom+GAP_TITLE);
			Dom.Move(pMove->GetPoint(MPE_RCSTD2_STANDARD_TITLE));
			pStd->ChangeSeperateLayer(&Dom);
			Dom.SetCalcExtRect();
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_RCSTD2_STANDARD_TITLE, _T("ǥ�شܸ鵵 Ÿ��Ʋ"));
			*pDomP << Dom;
		}
	}
	else
	{
		rcStandard.left		= rcDogak.right - Dom.Always(20);
		rcStandard.bottom	= rcDogak.bottom - Dom.Always(30);
		rcStandard.right	= rcStandard.left;
		rcStandard.top		= rcStandard.bottom;
	}
	////////////////////////////////////////////////////////////////////////////////////


	// ����2 - BMD //////////////////////////////////////////////////////////////////////
	CDRect rcBMD(0, 0, 0, 0);	// ����2 - BMD
	pPageMng->SetProgressData(_T("BMD �����..."), Pos+=nOffsetPos);
	if(dOptDraw & RCSTD2_BMD)
	{
		if(m_pStd->IsDrawByBlock(MPE_RCSTD2_BMD))
		{
			rcBMD	= DrawDiagramBMD_SFD(&Dom, TRUE, FALSE, FALSE, FALSE, FALSE);
			if(bSkew)
				xyMove	= CDPoint(rcDogak.CenterPoint().x - rcBMD.CenterPoint().x - rcDogak.Width()/4 - rcBMD.Width()/2, rcDogak.top - rcBMD.top + Dom.Always(70));
			else
				xyMove	= CDPoint(rcDogak.CenterPoint().x - rcBMD.CenterPoint().x - rcDogak.Width()/4, rcStandard.top - rcBMD.bottom - Dom.Always(50));
			Dom.Move(xyMove);
			rcBMD.left	+= xyMove.x; rcBMD.right	+= xyMove.x;
			rcBMD.top		+= xyMove.y; rcBMD.bottom	+= xyMove.y;
			Dom.SetCalcExtRect();
			rect	= Dom.GetExtRect();

			Dom.Move(pMove->GetPoint(MPE_RCSTD2_BMD));
			pStd->ChangeSeperateLayer(&Dom);
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_RCSTD2_BMD, _T("BMD"));
			*pDomP << Dom;
		}

		if(m_pStd->IsDrawByBlock(MPE_RCSTD2_BMD_TITLE))
		{
			pStd->AddTitleOnDom(&Dom, m_nBridge, _T("BMD"), CDPoint(0,0), -1, 0);
			Dom.Move(rect.left+rect.Width()/2.0, rect.bottom+GAP_TITLE);
			Dom.Move(pMove->GetPoint(MPE_RCSTD2_BMD_TITLE));
			pStd->ChangeSeperateLayer(&Dom);
			Dom.SetCalcExtRect();
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_RCSTD2_BMD_TITLE, _T("BMD"));
			*pDomP << Dom;
		}
	}
	else
	{
		rcBMD.left		= rcStandard.right - Dom.Always(20);
		rcBMD.bottom	= rcStandard.bottom - Dom.Always(30);
		rcBMD.right	= rcBMD.left;
		rcBMD.top		= rcBMD.bottom;
	}
	////////////////////////////////////////////////////////////////////////////////////

	// ����2 - SFD //////////////////////////////////////////////////////////////////////
	CDRect rcSFD(0, 0, 0, 0);	// ����2 - SFD
	pPageMng->SetProgressData(_T("SFD �����..."), Pos+=nOffsetPos);
	if(dOptDraw & RCSTD2_SFD)
	{
		if(m_pStd->IsDrawByBlock(MPE_RCSTD2_SFD))
		{
			rcSFD	= DrawDiagramBMD_SFD(&Dom, FALSE, TRUE, FALSE, TRUE, FALSE);
			if(bSkew)
				xyMove	= CDPoint(rcDogak.CenterPoint().x - rcSFD.CenterPoint().x - rcDogak.Width()/4 + rcSFD.Width()/2, rcDogak.top - rcSFD.top + Dom.Always(70));
			else
				xyMove	= CDPoint(rcDogak.CenterPoint().x - rcSFD.CenterPoint().x - rcDogak.Width()/4, rcBMD.top - rcSFD.bottom - Dom.Always(50));
			Dom.Move(xyMove);
			rcSFD.left	+= xyMove.x; rcSFD.right	+= xyMove.x;
			rcSFD.top		+= xyMove.y; rcSFD.bottom	+= xyMove.y;
			Dom.SetCalcExtRect();
			rect	= Dom.GetExtRect();

			Dom.Move(pMove->GetPoint(MPE_RCSTD2_SFD));
			pStd->ChangeSeperateLayer(&Dom);
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_RCSTD2_SFD, _T("SFD"));
			*pDomP << Dom;
		}

		if(m_pStd->IsDrawByBlock(MPE_RCSTD2_SFD_TITLE))
		{
			pStd->AddTitleOnDom(&Dom, m_nBridge, _T("SFD"), CDPoint(0,0), -1, 0);
			Dom.Move(rect.left+rect.Width()/2.0, rect.bottom+GAP_TITLE);
			Dom.Move(pMove->GetPoint(MPE_RCSTD2_SFD_TITLE));
			pStd->ChangeSeperateLayer(&Dom);
			Dom.SetCalcExtRect();
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_RCSTD2_SFD_TITLE, _T("SFD"));
			*pDomP << Dom;
		}
	}
	else
	{
		rcSFD.left		= rcBMD.right - Dom.Always(20);
		rcSFD.bottom	= rcBMD.bottom - Dom.Always(30);
		rcSFD.right	= rcSFD.left;
		rcSFD.top		= rcSFD.bottom;
	}
	////////////////////////////////////////////////////////////////////////////////////

	// ����2 - ��ö��������1 //////////////////////////////////////////////////////////////////////
	CDRect rcMainAssm1(0, 0, 0, 0);	// ����2 - ��ö��������1
	CString sTitle	= _T("");
	sTitle.Format("�� C.T.C %.0f", pBri->GetCtcRebarMain(TRUE));
	pPageMng->SetProgressData(_T("��ö��������1 �����..."), Pos+=nOffsetPos);
	if(dOptDraw & RCSTD2_MAINASSM_1)
	{
		if(m_pStd->IsDrawByBlock(MPE_RCSTD2_MAIN_ASSM1))
		{
			rcMainAssm1	= DrawRcStd2MainAssm(&Dom, 0);
			xyMove	= CDPoint(rcDogak.CenterPoint().x - rcMainAssm1.CenterPoint().x + rcDogak.Width()/4, rcDogak.bottom - rcMainAssm1.bottom - Dom.Always(120));
			Dom.Move(xyMove);
			rcMainAssm1.left	+= xyMove.x; rcMainAssm1.right	+= xyMove.x;
			rcMainAssm1.top		+= xyMove.y; rcMainAssm1.bottom	+= xyMove.y;
			Dom.SetCalcExtRect();
			rect	= Dom.GetExtRect();

			Dom.Move(pMove->GetPoint(MPE_RCSTD2_MAIN_ASSM1));
			pStd->ChangeSeperateLayer(&Dom);
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_RCSTD2_MAIN_ASSM1, _T("��ö��������1"));
			*pDomP << Dom;
		}

		if(m_pStd->IsDrawByBlock(MPE_RCSTD2_MAIN_ASSM1_TITLE))
		{
			pStd->AddTitleOnDom(&Dom, m_nBridge, sTitle, CDPoint(0,0), -1, dScale, TRUE);
			Dom.Move(rect.left+rect.Width()/2.0, rect.bottom+GAP_TITLE);
			Dom.Move(pMove->GetPoint(MPE_RCSTD2_MAIN_ASSM1_TITLE));
			pStd->ChangeSeperateLayer(&Dom);
			Dom.SetCalcExtRect();
			rect	= Dom.GetExtRect();
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_RCSTD2_MAIN_ASSM1_TITLE, _T("��ö��������1 Ÿ��Ʋ"));
			*pDomP << Dom;
		}
	}
	else
	{
		rcMainAssm1.left		= rcDogak.right - Dom.Always(20);
		rcMainAssm1.bottom	= rcDogak.bottom - Dom.Always(30);
		rcMainAssm1.right	= rcMainAssm1.left;
		rcMainAssm1.top		= rcMainAssm1.bottom;
	}
	////////////////////////////////////////////////////////////////////////////////////

	// ��ö�������� ���� Ÿ��Ʋ
	if(dOptDraw & RCSTD2_MAINASSM_1 || dOptDraw & RCSTD2_MAINASSM_2)
	{
		if(m_pStd->IsDrawByBlock(MPE_RCSTD2_MAIN_ASSM_MAINTITLE))
		{
			pStd->AddTitleOnDom(&Dom, m_nBridge, "��ö�� ������", CDPoint(0,0), 0, dScale);
			Dom.Move(rect.left+rect.Width()/2.0, rect.bottom+GAP_TITLE);
			Dom.Move(pMove->GetPoint(MPE_RCSTD2_MAIN_ASSM_MAINTITLE));
			pStd->ChangeSeperateLayer(&Dom);
			Dom.SetCalcExtRect();
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_RCSTD2_MAIN_ASSM_MAINTITLE, _T("��ö�� ������ Ÿ��Ʋ"));
			*pDomP << Dom;
		}
	}

	// ����2 - ��ö��������2 //////////////////////////////////////////////////////////////////////
	CDRect rcMainAssm2(0, 0, 0, 0);	// ����2 - ��ö��������2
	sTitle.Format("�� C.T.C %.0f", pBri->GetCtcRebarMain(TRUE));
	pPageMng->SetProgressData(_T("��ö��������2 �����..."), Pos+=nOffsetPos);
	if(dOptDraw & RCSTD2_MAINASSM_2)
	{
		if(m_pStd->IsDrawByBlock(MPE_RCSTD2_MAIN_ASSM2))
		{
			rcMainAssm2	= DrawRcStd2MainAssm(&Dom, 1);
			if(bSkew)
				xyMove	= CDPoint(rcDogak.CenterPoint().x - rcMainAssm2.CenterPoint().x + rcDogak.Width()/4, rcDogak.CenterPoint().y - rcMainAssm2.CenterPoint().y);
			else
				xyMove	= CDPoint(rcDogak.CenterPoint().x - rcMainAssm2.CenterPoint().x + rcDogak.Width()/4, rcDogak.top - rcMainAssm2.top + Dom.Always(120));
			Dom.Move(xyMove);
			rcMainAssm2.left	+= xyMove.x; rcMainAssm2.right	+= xyMove.x;
			rcMainAssm2.top		+= xyMove.y; rcMainAssm2.bottom	+= xyMove.y;
			Dom.SetCalcExtRect();
			rect	= Dom.GetExtRect();

			Dom.Move(pMove->GetPoint(MPE_RCSTD2_MAIN_ASSM2));
			pStd->ChangeSeperateLayer(&Dom);
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_RCSTD2_MAIN_ASSM2, _T("��ö��������2"));
			*pDomP << Dom;
		}

		if(m_pStd->IsDrawByBlock(MPE_RCSTD2_MAIN_ASSM2_TITLE))
		{
			pStd->AddTitleOnDom(&Dom, m_nBridge, sTitle, CDPoint(0,0), -1, dScale, TRUE);
			Dom.Move(rect.left+rect.Width()/2.0, rect.bottom+GAP_TITLE);
			Dom.Move(pMove->GetPoint(MPE_RCSTD2_MAIN_ASSM2_TITLE));
			pStd->ChangeSeperateLayer(&Dom);
			Dom.SetCalcExtRect();
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_RCSTD2_MAIN_ASSM2_TITLE, _T("��ö��������2 Ÿ��Ʋ"));
			*pDomP << Dom;
		}
	}
	else
	{
		rcMainAssm2.left		= rcDogak.right - Dom.Always(20);
		rcMainAssm2.bottom	= rcDogak.bottom - Dom.Always(30);
		rcMainAssm2.right	= rcMainAssm2.left;
		rcMainAssm2.top		= rcMainAssm2.bottom;
	}

	////////////////////////////////////////////////////////////////////////////////////

	// ����2 - ��鵵 //////////////////////////////////////////////////////////////////////
	CDRect rcPlan(0, 0, 0, 0);	// ����2 - ��鵵
	pPageMng->SetProgressData(_T("��鵵 �����..."), Pos+=nOffsetPos);
	if(bSkew && dOptDraw & RCSTD2_STANDARD)
	{
		if(m_pStd->IsDrawByBlock(MPE_RCSTD2_PLAN))
		{
			
			rcPlan	= DrawRcStd2Plan(&Dom);
			xyMove	= CDPoint(rcDogak.CenterPoint().x - rcPlan.CenterPoint().x - rcDogak.Width()/4, rcDogak.bottom - rcPlan.bottom - Dom.Always(100));
			Dom.Move(xyMove);
			rcPlan.left	+= xyMove.x; rcPlan.right	+= xyMove.x;
			rcPlan.top		+= xyMove.y; rcPlan.bottom	+= xyMove.y;
			Dom.SetCalcExtRect();
			rect	= Dom.GetExtRect();

			Dom.Move(pMove->GetPoint(MPE_RCSTD2_PLAN));
			pStd->ChangeSeperateLayer(&Dom);
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_RCSTD2_PLAN, _T("��鵵"));
			*pDomP << Dom;
		}

		if(m_pStd->IsDrawByBlock(MPE_RCSTD2_PLAN_TITLE))
		{
			pStd->AddTitleOnDom(&Dom, m_nBridge, _T("��鵵"), CDPoint(0,0), 0, dScale);
			Dom.Move(rect.left+rect.Width()/2.0, rect.bottom+GAP_TITLE);
			Dom.Move(pMove->GetPoint(MPE_RCSTD2_PLAN_TITLE));
			pStd->ChangeSeperateLayer(&Dom);
			Dom.SetCalcExtRect();
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_RCSTD2_PLAN_TITLE, _T("��鵵 Ÿ��Ʋ"));
			*pDomP << Dom;
		}
	}
	else
	{
		rcPlan.left		= rcDogak.right - Dom.Always(20);
		rcPlan.bottom	= rcDogak.bottom - Dom.Always(30);
		rcPlan.right	= rcPlan.left;
		rcPlan.top		= rcPlan.bottom;
	}
	////////////////////////////////////////////////////////////////////////////////////

	// ����2 - �а��� ���(����) //////////////////////////////////////////////////////////////////////
	pPageMng->SetProgressData(_T("�а���(���) �����..."), Pos+=nOffsetPos);
	long nIdx	= 0;
	for(long stt = 0; stt < 2; stt++)
	{
		BOOL bStt	= stt == iSTT;
		for(long left = 0; left < 2; left++)
		{
			BOOL bLeft	= left == iLEFT;

			if(!pBri->m_bIsObtuseAngle_UpperSlab[stt][left]) continue;

			nIdx++;

			for(long upper = 0; upper < 2; upper++)
			{
				BOOL bUpper	= upper == iUPPER;

				CDRect rcObtusPlan(0, 0, 0, 0);	// ����2 - �а���(���)
				long nMPE	= bUpper ? MPE_RCSTD2_OBTUS_UPPER+nIdx : MPE_RCSTD2_OBTUS_LOWER+nIdx;
				long nMPETitle	= bUpper ? MPE_RCSTD2_OBTUS_UPPER_TITLE+nIdx : MPE_RCSTD2_OBTUS_LOWER_TITLE+nIdx;
				if(dOptDraw & RCSTD2_OBTUS_DETAIL)
				{
					if(m_pStd->IsDrawByBlock(nMPE))
					{
						rcObtusPlan	= DrawRcStd2ObtusPlan(&Dom, bStt, bLeft, bUpper);
						if(bUpper)
							xyMove	= CDPoint(rcDogak.right - rcObtusPlan.left + (Dom.Always(50) + rcObtusPlan.Width())*nIdx, rcDogak.bottom - rcObtusPlan.bottom - Dom.Always(50));
						else
							xyMove	= CDPoint(rcDogak.right - rcObtusPlan.left + (Dom.Always(50) + rcObtusPlan.Width())*nIdx, rcDogak.CenterPoint().y - rcObtusPlan.CenterPoint().y);
						Dom.Move(xyMove);
						rcObtusPlan.left	+= xyMove.x; rcObtusPlan.right	+= xyMove.x;
						rcObtusPlan.top		+= xyMove.y; rcObtusPlan.bottom	+= xyMove.y;
						Dom.SetCalcExtRect();
						rect	= Dom.GetExtRect();

						Dom.Move(pMove->GetPoint(nMPE));
						pStd->ChangeSeperateLayer(&Dom);
						pMove->AddMPE(&Dom, Dom.GetExtRect(), nMPE, _T("�а���(���)"));
						*pDomP << Dom;
					}

					if(m_pStd->IsDrawByBlock(nMPETitle))
					{
						CString sUpper	= _T("");
						CString sLower	= _T("");
						sUpper.Format("�а���(%s%s)", bStt ? "����" : "����", bLeft ? "����" : "����");
						sLower.Format("���� ��(%s)", bUpper ? "���" : "�ϸ�");
						pStd->AddTitleOnDom(&Dom, m_nBridge, sUpper, CDPoint(0,Dom.GetTextHeight()*3), 0, -1);
						pStd->AddTitleOnDom(&Dom, m_nBridge, sLower, CDPoint(0, -Dom.GetTextHeight()), 0, dScale);
						Dom.Move(rect.left+rect.Width()/2.0, rect.bottom+GAP_TITLE);
						Dom.Move(pMove->GetPoint(nMPETitle));
						pStd->ChangeSeperateLayer(&Dom);
						Dom.SetCalcExtRect();
						pMove->AddMPE(&Dom, Dom.GetExtRect(), nMPETitle, _T("�а���(���) Ÿ��Ʋ"));
						*pDomP << Dom;
					}
				}
				else
				{
					rcObtusPlan.left		= rcDogak.right - Dom.Always(20);
					rcObtusPlan.bottom	= rcDogak.bottom - Dom.Always(30);
					rcObtusPlan.right	= rcObtusPlan.left;
					rcObtusPlan.top		= rcObtusPlan.bottom;
				}
			}

			CDRect rcObtusSection(0, 0, 0, 0);	// ����2 - �а��� ���ܸ�
			if(dOptDraw & RCSTD2_OBTUS_DETAIL)
			{
				if(m_pStd->IsDrawByBlock(MPE_RCSTD2_OBTUS_SECTION+nIdx))
				{
					rcObtusSection	= DrawRcStd2ObtusSection(&Dom, bStt, bLeft);
					xyMove	= CDPoint(rcDogak.right - rcObtusSection.left + (Dom.Always(50) + rcObtusSection.Width())*nIdx, rcDogak.top - rcObtusSection.top + Dom.Always(70));
					Dom.Move(xyMove);
					rcObtusSection.left	+= xyMove.x; rcObtusSection.right	+= xyMove.x;
					rcObtusSection.top		+= xyMove.y; rcObtusSection.bottom	+= xyMove.y;
					Dom.SetCalcExtRect();
					rect	= Dom.GetExtRect();

					Dom.Move(pMove->GetPoint(MPE_RCSTD2_OBTUS_SECTION+nIdx));
					pStd->ChangeSeperateLayer(&Dom);
					pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_RCSTD2_OBTUS_SECTION+nIdx, _T("�а��� ���ܸ�"));
					*pDomP << Dom;
				}

				if(m_pStd->IsDrawByBlock(MPE_RCSTD2_OBTUS_SECTION_TITLE+nIdx))
				{
					CString sUpper	= _T("");
					CString sLower	= _T("");
					sUpper.Format("�а���(%s%s)", bStt ? "����" : "����", bLeft ? "����" : "����");
					sLower.Format("���� ��(���ܸ�)");
					pStd->AddTitleOnDom(&Dom, m_nBridge, sUpper, CDPoint(0,Dom.GetTextHeight()*3), 0, -1);
					pStd->AddTitleOnDom(&Dom, m_nBridge, sLower, CDPoint(0, -Dom.GetTextHeight()), 0, dScale);
					Dom.Move(rect.left+rect.Width()/2.0, rect.bottom+GAP_TITLE);
					Dom.Move(pMove->GetPoint(MPE_RCSTD2_OBTUS_SECTION_TITLE+nIdx));
					pStd->ChangeSeperateLayer(&Dom);
					Dom.SetCalcExtRect();
					pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_RCSTD2_OBTUS_SECTION_TITLE+nIdx, _T("�а��� ���ܸ� Ÿ��Ʋ"));
					*pDomP << Dom;
				}
			}
			else
			{
				rcObtusSection.left		= rcDogak.right - Dom.Always(20);
				rcObtusSection.bottom	= rcDogak.bottom - Dom.Always(30);
				rcObtusSection.right	= rcObtusSection.left;
				rcObtusSection.top		= rcObtusSection.bottom;
			}
		}
	}
	////////////////////////////////////////////////////////////////////////////////////
}

void CDrawingSlabStandardOutput::AddPageOne3(CDomyun *pDomP, CDRect rcDogak)
{
	CARcBridgeDrawingStd* pStd			= (CARcBridgeDrawingStd*)m_pStd;
	CRcBridgeRebar *pBri				= GetRcBridgeRebar();
	CARcBridgeDrawingOptionStd* pOpt	= pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData	= pOpt->GetDrawingOption(m_nBridge);
	CDrawPageMng* pPageMng				= pStd->GetPageManage();
	COutMovePoint* pMove				= pStd->GetMovePoint(pStd->m_nCurPage);

	CARcBridgeDBStandard DBStd(pBri, pStd->m_pARoadOptionStd, pStd->m_pARcBridgeDataStd);
	DBStd.SetSelectFckDomText(pOpt->m_nSelectFckDomText);
	DBStd.m_pDrawingOptionData	= pData;
	double dScale	= GetScale();
	DWORD dOptDraw	= pData->m_dwDrawRcStd;

	CDomyun Dom(pDomP);
	Dom.SetScaleDim(dScale);

	CString str	= _T("");
	CDPoint xyMove(0, 0);
	CDRect rect(0, 0, 0, 0);
	int nCountMPE	= RCSTD3_COUNT;
	int Pos	= 0;
	int nOffsetPos	= 100 / nCountMPE;


	// ����3 - �Ŵ��� //////////////////////////////////////////////////////////////////////
	CDRect rcGirderDetail(0, 0, 0, 0);	// ����3 - �Ŵ���
	pPageMng->SetProgressData(_T("�Ŵ��� �����..."), Pos+=nOffsetPos);
	if(dOptDraw & RCSTD3_GIRDER_DETAIL)
	{
		if(m_pStd->IsDrawByBlock(MPE_RCSTD3_GIRDER_DETAIL))
		{
			rcGirderDetail	= DrawRcStd3GirderDetail(&Dom, 0);
			xyMove	= CDPoint(rcDogak.CenterPoint().x - rcGirderDetail.CenterPoint().x - rcDogak.Width()/4 - rcDogak.Width()/10, rcDogak.bottom - rcGirderDetail.bottom - Dom.Always(120));
			Dom.Move(xyMove);
			rcGirderDetail.left	+= xyMove.x; rcGirderDetail.right	+= xyMove.x;
			rcGirderDetail.top		+= xyMove.y; rcGirderDetail.bottom	+= xyMove.y;
			Dom.SetCalcExtRect();
			rect	= Dom.GetExtRect();

			Dom.Move(pMove->GetPoint(MPE_RCSTD3_GIRDER_DETAIL));
			pStd->ChangeSeperateLayer(&Dom);
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_RCSTD3_GIRDER_DETAIL, _T("�Ŵ���"));
			*pDomP << Dom;
		}

		if(m_pStd->IsDrawByBlock(MPE_RCSTD3_GIRDER_DETAIL_TITLE))
		{
			pStd->AddTitleOnDom(&Dom, m_nBridge, _T("�Ŵ���"), CDPoint(0,0), 0, dScale);
			Dom.Move(rect.left+rect.Width()/2.0, rect.bottom+GAP_TITLE);
			Dom.Move(pMove->GetPoint(MPE_RCSTD3_GIRDER_DETAIL_TITLE));
			pStd->ChangeSeperateLayer(&Dom);
			Dom.SetCalcExtRect();
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_RCSTD3_GIRDER_DETAIL_TITLE, _T("�Ŵ��� Ÿ��Ʋ"));
			*pDomP << Dom;
		}
	}
	else
	{
		rcGirderDetail.left		= rcDogak.right - Dom.Always(20);
		rcGirderDetail.bottom	= rcDogak.bottom - Dom.Always(30);
		rcGirderDetail.right	= rcGirderDetail.left;
		rcGirderDetail.top		= rcGirderDetail.bottom;
	}
	////////////////////////////////////////////////////////////////////////////////////

	// ����3 - ��ȣ�� �� //////////////////////////////////////////////////////////////////////
	CDRect rcGuardWallDetail(0, 0, 0, 0);	// ����3 - ��ȣ�� ��
	pPageMng->SetProgressData(_T("��ȣ���� �����..."), Pos+=nOffsetPos);
	if(dOptDraw & RCSTD3_GUARDWALL_DETAIL)
	{
		if(m_pStd->IsDrawByBlock(MPE_RCSTD3_GUARDWALL_DETAIL))
		{
			rcGuardWallDetail	= DrawRcStd3GuardWallDetail(&Dom, 0);
			xyMove	= CDPoint(rcDogak.CenterPoint().x - rcGuardWallDetail.CenterPoint().x - rcDogak.Width()/4 + rcDogak.Width()/8, rcDogak.bottom - rcGuardWallDetail.bottom - Dom.Always(120));
			Dom.Move(xyMove);
			rcGuardWallDetail.left		+= xyMove.x; rcGuardWallDetail.right	+= xyMove.x;
			rcGuardWallDetail.top		+= xyMove.y; rcGuardWallDetail.bottom	+= xyMove.y;
			Dom.SetCalcExtRect();
			rect	= Dom.GetExtRect();

			Dom.Move(pMove->GetPoint(MPE_RCSTD3_GUARDWALL_DETAIL));
			pStd->ChangeSeperateLayer(&Dom);
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_RCSTD3_GUARDWALL_DETAIL, _T("��ȣ����"));
			*pDomP << Dom;
		}

		if(m_pStd->IsDrawByBlock(MPE_RCSTD3_GUARDWALL_DETAIL_TITLE))
		{
			pStd->AddTitleOnDom(&Dom, m_nBridge, _T("��ȣ����"), CDPoint(0,0), 0, dScale);
			Dom.Move(rect.left+rect.Width()/2.0, rect.bottom+GAP_TITLE);
			Dom.Move(pMove->GetPoint(MPE_RCSTD3_GUARDWALL_DETAIL_TITLE));
			pStd->ChangeSeperateLayer(&Dom);
			Dom.SetCalcExtRect();
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_RCSTD3_GUARDWALL_DETAIL_TITLE, _T("��ȣ���� Ÿ��Ʋ"));
			*pDomP << Dom;
		}
	}
	else
	{
		rcGuardWallDetail.left		= rcDogak.right - Dom.Always(20);
		rcGuardWallDetail.bottom	= rcDogak.bottom - Dom.Always(30);
		rcGuardWallDetail.right	= rcGuardWallDetail.left;
		rcGuardWallDetail.top		= rcGuardWallDetail.bottom;
	}
	////////////////////////////////////////////////////////////////////////////////////

	
	// ����3 - �󼼵� �ܰ� //////////////////////////////////////////////////////////////////////
	CDRect rcDetailOutLine(0, 0, 0, 0);	// ����3 - �󼼵� �ܰ�
	pPageMng->SetProgressData(_T("�󼼵� �ܰ� �����..."), Pos+=nOffsetPos);
	if(dOptDraw & RCSTD3_NOTE)
	{
		if(m_pStd->IsDrawByBlock(MPE_RCSTD3_DETAIL_OUTLINE))
		{
			rcDetailOutLine	= DrawRcStd3DetailOutLine(&Dom);
			xyMove	= CDPoint(rcDogak.CenterPoint().x - rcDetailOutLine.CenterPoint().x - rcDogak.Width()/4 + Dom.Always(20), rcDogak.bottom - rcDetailOutLine.bottom - Dom.Always(70));
			Dom.Move(xyMove);
			rcDetailOutLine.left		+= xyMove.x; rcDetailOutLine.right	+= xyMove.x;
			rcDetailOutLine.top		+= xyMove.y; rcDetailOutLine.bottom	+= xyMove.y;
			Dom.SetCalcExtRect();
			rect	= Dom.GetExtRect();

			Dom.Move(pMove->GetPoint(MPE_RCSTD3_DETAIL_OUTLINE));
			pStd->ChangeSeperateLayer(&Dom);
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_RCSTD3_DETAIL_OUTLINE, _T("�󼼵� �ܰ�"));
			*pDomP << Dom;
		}
	}
	else
	{
		rcDetailOutLine.left		= rcDogak.right - Dom.Always(20);
		rcDetailOutLine.bottom	= rcDogak.bottom - Dom.Always(30);
		rcDetailOutLine.right	= rcDetailOutLine.left;
		rcDetailOutLine.top		= rcDetailOutLine.bottom;
	}
	////////////////////////////////////////////////////////////////////////////////////

	// ����3 - ������ //////////////////////////////////////////////////////////////////////
	CDRect rc3Design(0, 0, 0, 0);	// ����3 - ������
	pPageMng->SetProgressData(_T("������ �����..."), Pos+=nOffsetPos);
	if(dOptDraw & RCSTD3_DESIGN_METHOD)
	{
		if(m_pStd->IsDrawByBlock(MPE_RCSTD3_DESIGN_METHOD))
		{
			rc3Design	= DrawRcStd3DesignMethod(&Dom);
			xyMove	= CDPoint(rcDogak.CenterPoint().x - rc3Design.CenterPoint().x + rcDogak.Width()/4, rcDogak.top - rc3Design.top + Dom.Always(70));
			Dom.Move(xyMove);
			rc3Design.left		+= xyMove.x; rc3Design.right	+= xyMove.x;
			rc3Design.top		+= xyMove.y; rc3Design.bottom	+= xyMove.y;
			Dom.SetCalcExtRect();
			rect	= Dom.GetExtRect();

			Dom.Move(pMove->GetPoint(MPE_RCSTD3_DESIGN_METHOD));
			pStd->ChangeSeperateLayer(&Dom);
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_RCSTD3_DESIGN_METHOD, _T("������"));
			*pDomP << Dom;
		}
	}
	else
	{
		rc3Design.left		= rcDogak.right - Dom.Always(20);
		rc3Design.bottom	= rcDogak.bottom - Dom.Always(30);
		rc3Design.right		= rc3Design.left;
		rc3Design.top		= rc3Design.bottom;
	}
	////////////////////////////////////////////////////////////////////////////////////

	// ����3 - ���������� //////////////////////////////////////////////////////////////////////
	CDRect rcSpacer(0, 0, 0, 0);	// ����3 - ����������
	pPageMng->SetProgressData(_T("���������� �����..."), Pos+=nOffsetPos);
	if(dOptDraw & RCSTD3_SPACER)
	{
		if(m_pStd->IsDrawByBlock(MPE_RCSTD3_SPACER))
		{
			rcSpacer	= DrawRcStd3Spacer(&Dom);
			xyMove	= CDPoint(rcDogak.CenterPoint().x - rcSpacer.CenterPoint().x - rcDogak.Width()/4 + Dom.Always(20), rcDogak.top - rcSpacer.top + Dom.Always(70));
			Dom.Move(xyMove);
			rcSpacer.left		+= xyMove.x; rcSpacer.right	+= xyMove.x;
			rcSpacer.top		+= xyMove.y; rcSpacer.bottom	+= xyMove.y;
			Dom.SetCalcExtRect();
			rect	= Dom.GetExtRect();

			Dom.Move(pMove->GetPoint(MPE_RCSTD3_SPACER));
			pStd->ChangeSeperateLayer(&Dom);
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_RCSTD3_SPACER, _T("����������"));
			*pDomP << Dom;
		}

		if(m_pStd->IsDrawByBlock(MPE_RCSTD3_SPACER_TITLE))
		{
			pStd->AddTitleOnDom(&Dom, m_nBridge, _T("����������"), CDPoint(0,0), 0, -1);
			Dom.Move(rect.left+rect.Width()/2.0, rect.bottom+GAP_TITLE);
			Dom.Move(pMove->GetPoint(MPE_RCSTD3_SPACER_TITLE));
			pStd->ChangeSeperateLayer(&Dom);
			Dom.SetCalcExtRect();
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_RCSTD3_SPACER_TITLE, _T("���������� Ÿ��Ʋ"));
			*pDomP << Dom;
		}
	}
	else
	{
		rcSpacer.left		= rcDogak.right - Dom.Always(20);
		rcSpacer.bottom	= rcDogak.bottom - Dom.Always(30);
		rcSpacer.right	= rcSpacer.left;
		rcSpacer.top		= rcSpacer.bottom;
	}
	////////////////////////////////////////////////////////////////////////////////////
}

// ����1 - ��������
CDRect CDrawingSlabStandardOutput::DrawRcStd1DesignCondition(CDomyun *pDom)
{
	if(!pDom) return CDRect(0, 0, 0, 0);
	
	CRcBridgeRebar *pBri = GetRcBridgeRebar();

	CDomyun Dom(pDom);
	
	CString sBlkName	= _T("RCSTD1_DESIGN");
	DxfIn(&Dom, sBlkName, 2, 2);
	

// 	CString sBefore	= _T("");
	CString sAfter	= _T("");

	// ���� ����
	// #��ü��ũ��Ʈ����#
	CString sBefore	= "#��ü��ũ��Ʈ����#";
	CString szKgf_cm2_CGS   = "kgf/cm��";
	CString szKgf_cm2_Apply = pBri->m_pARcBridgeDataStd->GetStringUnitType(szKgf_cm2_CGS);
	sAfter.Format(" = %.0f%s", GetValueUnitChange(pBri->GetValueFck(ePartUpperSlab), szKgf_cm2_CGS, szKgf_cm2_Apply), szKgf_cm2_Apply);
	Dom.ReplaceString(sBefore, sAfter);
	
	// #��üö�ٰ���#
	sBefore	= "#��üö�ٰ���#";
	sAfter.Format(" = %.0f%s", GetValueUnitChange(pBri->m_pARcBridgeDataStd->m_Fy, szKgf_cm2_CGS, szKgf_cm2_Apply), szKgf_cm2_Apply);
	Dom.ReplaceString(sBefore, sAfter);

	// #��üö�ٰ������#
	sBefore	= "#��üö�ٰ������#";
	sAfter.Format("SD %.0f", (pBri->m_pARcBridgeDataStd->m_nTypeUnit == 0 ? pBri->m_pARcBridgeDataStd->m_Fy : pBri->m_pARcBridgeDataStd->m_Fy*10)/100);
	Dom.ReplaceString(sBefore, sAfter);
	
	// #�氣����#
	sBefore	= "#�氣����#";
	sAfter.Format("L = %.1fm", toM(pBri->GetLengthBridge(FALSE)));
	Dom.ReplaceString(sBefore, sAfter);

	// #�簢#
	sBefore	= "#�簢#";
	double dAng	= pBri->GetAngleJijum(0).GetAngleDegree();
	if(dAng != 90)
		dAng	= 90 - dAng;
	sAfter.Format("%.0f%%%%d", dAng);
	Dom.ReplaceString(sBefore, sAfter);


	Dom.SetCalcExtRect();
	CDRect rect	= Dom.GetExtRect();
	*pDom << Dom;

	return rect;
}

// ����1 - Ⱦ�ܸ鵵
CDRect CDrawingSlabStandardOutput::DrawRcStd1Cross(CDomyun *pDom)
{
	if(!pDom) return CDRect(0, 0, 0, 0);
	
	CARcBridgeDrawingStd* pStd			= (CARcBridgeDrawingStd*)m_pStd;
	CARcBridgeDrawingOptionStd* pOpt	= pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData	= pOpt->GetDrawingOption(m_nBridge);
	CRcBridgeRebar *pBri = GetRcBridgeRebar();

	CARcBridgeDBStandard DBStd(pBri, pStd->m_pARoadOptionStd, pStd->m_pARcBridgeDataStd);
	DBStd.SetSelectFckDomText(pOpt->m_nSelectFckDomText);
	DBStd.m_pDrawingOptionData	= pData;

	CDomyun Dom(pDom);

	double dSta	= pBri->GetStationBridgeStt(FALSE);
	CDPoint vAng	= CDPoint(0, 1);

	pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
	DBStd.m_pDrawCrossSection->DrawCrossSection(&Dom, dSta, vAng, FALSE, TRUE, TRUE, TRUE, TRUE, FALSE);

	pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	DBStd.m_pDrawCrossSection->DimCrossSection(&Dom, dSta, vAng, FALSE);

	DBStd.m_pDrawCrossSection->DimCrossSide(&Dom, dSta, vAng, TRUE, FALSE);
	DBStd.m_pDrawCrossSection->DimThickCrossSection(&Dom, dSta, vAng, FALSE);
	DBStd.m_pDrawCrossSection->MarkGenCrossSlope(&Dom, dSta, vAng);
		

	

	Dom.SetCalcExtRect();
	CDRect rect	= Dom.GetExtRect();


	*pDom << Dom;

	return rect;
}

// ����2 - ǥ�شܸ鵵
CDRect CDrawingSlabStandardOutput::DrawRcStd2Standard(CDomyun *pDom)
{
	if(!pDom) return CDRect(0, 0, 0, 0);
	
	CARcBridgeDrawingStd* pStd			= (CARcBridgeDrawingStd*)m_pStd;
	CARcBridgeDrawingOptionStd* pOpt	= pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData	= pOpt->GetDrawingOption(m_nBridge);
	CRcBridgeRebar *pBri = GetRcBridgeRebar();

	CARcBridgeDBStandard DBStd(pBri, pStd->m_pARoadOptionStd, pStd->m_pARcBridgeDataStd);
	DBStd.SetSelectFckDomText(pOpt->m_nSelectFckDomText);
	DBStd.m_pDrawingOptionData	= pData;

	CDomyun Dom(pDom);

	DBStd.m_pDrawVertSection->DrawVertSection(&Dom, FALSE);
	for(long upper = 0; upper < 2; upper++)
	{
		BOOL bUpper	= upper == iUPPER;
		for(long dan = 0; dan < MAX_REBAR_DAN; dan++)
		{
			if(bUpper)
			{
				DBStd.m_pDrawVertSection->DrawRebarMain_VertSection_UpperSlab(&Dom, 0, dan, TRUE, FALSE, FALSE, 0, CDRect(0, 0, 0, 0), FALSE, 0);
				DBStd.m_pDrawVertSection->DrawRebarMain_VertSection_UpperSlabHunch(&Dom, 0, FALSE, 0, CDRect(0, 0, 0, 0), FALSE);
			}
			DBStd.m_pDrawVertSection->DrawRebarSupport_VertSection_UpperSlab(&Dom, bUpper, dan, TRUE, dan==0, 0, FALSE);
		}
	}
	DBStd.m_pDrawVertSection->DrawRebarShear_VertSection_UpperSlab(&Dom, TRUE, FALSE, FALSE);
	DBStd.m_pDrawVertSection->DrawRebarAll_SlabGirder(&Dom, 0, FALSE, FALSE, 0, FALSE, TRUE);


	Dom.SetCalcExtRect();
	CDRect rect	= Dom.GetExtRect();
	*pDom << Dom;

	return rect;
}

// ����2 - ��ö�� ������ 1, 2
CDRect CDrawingSlabStandardOutput::DrawRcStd2MainAssm(CDomyun *pDom, long nCycle)
{
	if(!pDom) return CDRect(0, 0, 0, 0);
	
	CARcBridgeDrawingStd* pStd			= (CARcBridgeDrawingStd*)m_pStd;
	CARcBridgeDrawingOptionStd* pOpt	= pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData	= pOpt->GetDrawingOption(m_nBridge);
	CRcBridgeRebar *pBri = GetRcBridgeRebar();

	CARcBridgeDBStandard DBStd(pBri, pStd->m_pARoadOptionStd, pStd->m_pARcBridgeDataStd);
	DBStd.SetSelectFckDomText(pOpt->m_nSelectFckDomText);
	DBStd.m_pDrawingOptionData	= pData;

	CDomyun Dom(pDom);

	DBStd.m_pDrawVertSection->DrawVertSection(&Dom, FALSE);
	for(long upper = 0; upper < 2; upper++)
	{
		BOOL bUpper	= upper == iUPPER;
		for(long dan = 0; dan < MAX_REBAR_DAN; dan++)
		{
			if(bUpper)
			{
				DBStd.m_pDrawVertSection->DrawRebarMain_VertSection_UpperSlab(&Dom, nCycle, dan, TRUE, TRUE, TRUE, 0, CDRect(0, 0, 0, 0), FALSE, 0);
				DBStd.m_pDrawVertSection->DrawRebarMain_VertSection_UpperSlabHunch(&Dom, nCycle, TRUE, 0, CDRect(0, 0, 0, 0), FALSE);
			}
		}
	}
	DBStd.m_pDrawVertSection->DimRebarMainAssm_VertSection_UpperSlabSide(&Dom, 0, nCycle, 0);


	Dom.SetCalcExtRect();
	CDRect rect	= Dom.GetExtRect();
	*pDom << Dom;

	return rect;
}

// ����3 - �Ŵ���
CDRect CDrawingSlabStandardOutput::DrawRcStd3GirderDetail(CDomyun *pDom, long nJ)
{
	if(!pDom) return CDRect(0, 0, 0, 0);
	
	CARcBridgeDrawingStd* pStd			= (CARcBridgeDrawingStd*)m_pStd;
	CARcBridgeDrawingOptionStd* pOpt	= pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData	= pOpt->GetDrawingOption(m_nBridge);
	CRcBridgeRebar *pBri = GetRcBridgeRebar();

	CARcBridgeDBStandard DBStd(pBri, pStd->m_pARoadOptionStd, pStd->m_pARcBridgeDataStd);
	DBStd.SetSelectFckDomText(pOpt->m_nSelectFckDomText);
	DBStd.m_pDrawingOptionData	= pData;

	CDomyun Dom(pDom);

	DBStd.m_pDrawVertSection->DrawDetail_Girder(&Dom, nJ);
	DBStd.m_pDrawVertSection->DrawRebarAll_SlabGirder(&Dom, nJ, TRUE, TRUE, 0, FALSE, FALSE);
	
	Dom.SetCalcExtRect();
	CDRect rect	= Dom.GetExtRect();
	*pDom << Dom;

	return rect;
}

CDRect CDrawingSlabStandardOutput::DrawRcStd3GuardWallDetail(CDomyun *pDom, long nHDan)
{
	if(!pDom) return CDRect(0, 0, 0, 0);
	
	CARcBridgeDrawingStd* pStd			= (CARcBridgeDrawingStd*)m_pStd;
	CARcBridgeDrawingOptionStd* pOpt	= pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData	= pOpt->GetDrawingOption(m_nBridge);
	CRcBridgeRebar *pBri = GetRcBridgeRebar();

	CARcBridgeDBStandard DBStd(pBri, pStd->m_pARoadOptionStd, pStd->m_pARcBridgeDataStd);
	DBStd.SetSelectFckDomText(pOpt->m_nSelectFckDomText);
	DBStd.m_pDrawingOptionData	= pData;

	CDomyun Dom(pDom);

	DBStd.m_pDrawCrossSection->DrawRebarGuardFence(&Dom, nHDan, FALSE, FALSE, FALSE);
	
	Dom.SetCalcExtRect();
	CDRect rect	= Dom.GetExtRect();
	*pDom << Dom;

	return rect;
}

// CObInsert* CDrawingSlabStandardOutput::InsertRcStdBlock(CDomyun *pDomP, const CString &szBlkName, CDPoint xy, double dScaleHor, double dScaleVer, double dRotDegAng, BOOL bHCADOnly)
// {
// 	CString szCovBlkName = szBlkName;
// 	CARcBridgeDrawingStd* pStd			= (CARcBridgeDrawingStd*)m_pStd;
// 	CARoadOptionStd *pARoadOptionStd	= pStd->m_pARoadOptionStd;
// 
// 	// ǥ�� ���̾� ����
// 	pARoadOptionStd->SetEnvType(pDomP, HCAD_SYMB);
// 	pARoadOptionStd->SetEnvType(pDomP, HCAD_BARA);
// 	pARoadOptionStd->SetEnvType(pDomP, HCAD_BARX);
// 	pARoadOptionStd->SetEnvType(pDomP, HCAD_BARC);
// 	pARoadOptionStd->SetEnvType(pDomP, HCAD_BNTL);
// 	pARoadOptionStd->SetEnvType(pDomP, HCAD_BNTT);
// 	pARoadOptionStd->SetEnvType(pDomP, HCAD_BNTB);
// 	pARoadOptionStd->SetEnvType(pDomP, HCAD_MIS1);
// 	pARoadOptionStd->SetEnvType(pDomP, HCAD_MIS2);
// 	pARoadOptionStd->SetEnvType(pDomP, HCAD_TXTG);
// 	CString szImsiCode = pARoadOptionStd->m_szDetLayerCode;
// 	pARoadOptionStd->m_szDetLayerCode = "";
// 	pARoadOptionStd->SetEnvType(pDomP, HCAD_SYMB);
// 	pARoadOptionStd->SetEnvType(pDomP, HCAD_BARA);
// 	pARoadOptionStd->SetEnvType(pDomP, HCAD_BARX);
// 	pARoadOptionStd->SetEnvType(pDomP, HCAD_BARC);
// 	pARoadOptionStd->SetEnvType(pDomP, HCAD_BNTL);
// 	pARoadOptionStd->SetEnvType(pDomP, HCAD_BNTT);
// 	pARoadOptionStd->SetEnvType(pDomP, HCAD_BNTB);
// 	pARoadOptionStd->SetEnvType(pDomP, HCAD_MIS1);
// 	pARoadOptionStd->SetEnvType(pDomP, HCAD_MIS2);
// 	pARoadOptionStd->SetEnvType(pDomP, HCAD_TXTG);
// 	pARoadOptionStd->m_szDetLayerCode = szImsiCode;
// 
// 	// ġ�� ũ�� ���� ==========================================================
// 	double dScaleFactor = 1.0;//pARoadOptionStd->m_LayerList[HCAD_DIMT]->m_TextHeight / 2.5;
// 	//pARoadOptionStd->SetEnvType(pDomP,HCAD_DIMT);
// 	//long nDimIdx = pARoadOptionStd->GetEnvType(pDomP);
// 	//if(nDimIdx!=-1) dScaleFactor = pARoadOptionStd->m_LayerList[HCAD_DIMT]->m_TextHeight / 2.5;	
// 	// =========================================================================
// 
// 	BOOL bFind = m_domRcStdStandard.IsExistBlock(szCovBlkName+"-NOATTR");
// 	if(!bFind || bHCADOnly)
// 	{
// 		if(m_domRcStdStandard.IsExistBlock(szCovBlkName))
// 		{
// 			if(!pDomP->IsExistBlock(szCovBlkName))
// 			{
// 				CMapStringToPtr* pBlockArray = m_domRcStdStandard.GetBlockArray();
// 				CDomyunBase* p;					
// 				if( pBlockArray->Lookup( szCovBlkName, (void *&)p ) )
// 				{
// 					CObInsert* pInsert = new CObInsert, *pInsertFind=NULL;
// 					CPtrList *pInsertList = m_domRcStdStandard.GetObjectList(ETT_INSERT);
// 					POSITION pos = pInsertList->GetHeadPosition();
// 					while(pos)
// 					{
// 						CObInsert* p = (CObInsert*)pInsertList->GetNext(pos);
// 						if(p->m_sBlockName == szCovBlkName) { pInsertFind = p; break; }
// 					}				 
// 					CLayerList* pLayer1 = pARoadOptionStd->m_LayerList[HCAD_BARX];
// 					CLayerList* pLayer2 = szCovBlkName.Left(6)=="CSBNUM" ? pARoadOptionStd->m_LayerList[HCAD_BARC] : szCovBlkName.Left(7)=="CSBSHPE" ? pARoadOptionStd->m_LayerList[HCAD_BARA] : pARoadOptionStd->m_LayerList[HCAD_SYMB];
// 					pInsert->SetsLayer(pARoadOptionStd->m_szDetLayerCode + ((szCovBlkName == "CSBSEC1") ? pLayer1->m_szCadLayername : pLayer2->m_szCadLayername));
// 					pInsert->m_sBlockName = szCovBlkName;
// 					pInsert->m_vPoint = xy;	
// 					pInsert->m_vSFactor = CVector(dScaleHor*dScaleFactor,dScaleVer*dScaleFactor,dScaleHor*dScaleFactor);
// 					pInsert->m_RotationAngle = dRotDegAng;					
// 					if(pInsertFind)
// 					{
// 						AhPtrListCopy(pInsert->m_AttDefArr,pInsertFind->m_AttDefArr,(CObAttdef*)0);														
// 						pos = pInsert->m_AttDefArr.GetHeadPosition();
// 						while(pos)
// 						{
// 							CObAttdef* p = (CObAttdef*)pInsert->m_AttDefArr.GetNext(pos);						
// 							p->m_ptPoint = xy + (p->m_ptPoint - pInsertFind->m_vPoint) * dScaleHor*dScaleFactor;
// 							p->m_ptPointSecond = xy + (p->m_ptPointSecond - pInsertFind->m_vPoint) * dScaleHor*dScaleFactor;
// 							p->m_TextStyle.Height *= dScaleHor*dScaleFactor;
// 
// 							// ũ�� ������ ���Ѱ�
// 							if(	szCovBlkName == HCAD_E00 || 
// 								szCovBlkName == HCAD_E01 || 
// 								szCovBlkName == HCAD_E00_KCFIT ||
// 								szCovBlkName == HCAD_E08 || // added by jaeho 2007.12.06
// 								szCovBlkName == HCAD_E09 )	// added by jaeho 2007.12.06
// 							{							
// 								double dNewSize = pARoadOptionStd->m_DimSet.RebarTextHeight * dScaleHor*dScaleFactor;
// 								p->m_TextStyle.Height = dNewSize;
// 
// 								if(p->m_TextStyle.Horizontal == DOM_TEXT_JUST_MIDDLE)
// 								{
// 									p->m_ptPoint.y += (pARoadOptionStd->m_DimSet.RebarCircleSize - 4)* dScaleHor*dScaleFactor;
// 									p->m_ptPointSecond.y += (pARoadOptionStd->m_DimSet.RebarCircleSize - 4)* dScaleHor*dScaleFactor;
// 								}
// 								else if(p->m_TextStyle.Horizontal == DOM_TEXT_JUST_FIT)
// 								{
// 									p->m_ptPoint.y += (pARoadOptionStd->m_DimSet.RebarCircleSize - 4)* dScaleHor*dScaleFactor/2;
// 									p->m_ptPointSecond.y += (pARoadOptionStd->m_DimSet.RebarCircleSize - 4)* dScaleHor*dScaleFactor/2;
// 								}
// 							}
// 						}
// 					}				
// 					
// 					CDomyunBase *pBlockDom = new CDomyunBase;					
// 					*pBlockDom = *p;
// 					pARoadOptionStd->ChangeLayerOnSet(pInsert);
// 					pARoadOptionStd->ChangeDomLastLayerNameForAPier(pBlockDom);					
// 					if(	szCovBlkName == HCAD_E00 || 
// 						szCovBlkName == HCAD_E01 || 
// 						szCovBlkName == HCAD_E00_KCFIT || 
// 						szCovBlkName == HCAD_E08 || // added by jaeho 2007.12.06
// 						szCovBlkName == HCAD_E09 )	// added by jaeho 2007.12.06
// 						pARoadOptionStd->ChangeRebarMark(pBlockDom, TRUE);
// 					pDomP->AddBlockAndInsert(pBlockDom, pInsert);						
// 					return pInsert;
// 				}
// 			}	
// 			else
// 			{									
// 				CObInsert* pInsert = new CObInsert, *pInsertFind=NULL;	
// 				CPtrList *pInsertList = m_domRcStdStandard.GetObjectList(ETT_INSERT);
// 				POSITION pos = pInsertList->GetHeadPosition();
// 				while(pos)
// 				{
// 					CObInsert* p = (CObInsert*)pInsertList->GetNext(pos);
// 					if(p->m_sBlockName == szCovBlkName) { pInsertFind = p; break; }
// 				}		
// 				if(pInsertFind)
// 				{
// 					AhPtrListCopy(pInsert->m_AttDefArr,pInsertFind->m_AttDefArr,(CObAttdef*)0);														
// 					pos = pInsert->m_AttDefArr.GetHeadPosition();
// 					while(pos)
// 					{
// 						CObAttdef* p = (CObAttdef*)pInsert->m_AttDefArr.GetNext(pos);						
// 						p->m_ptPoint = xy + (p->m_ptPoint - pInsertFind->m_vPoint) * dScaleHor*dScaleFactor;
// 						p->m_ptPointSecond = xy + (p->m_ptPointSecond - pInsertFind->m_vPoint) * dScaleHor*dScaleFactor;
// 						p->m_TextStyle.Height *= dScaleHor*dScaleFactor;
// 					}
// 				}	
// 				CLayerList* pLayer1 = pARoadOptionStd->m_LayerList[HCAD_BARX];
// 				CLayerList* pLayer2 = szCovBlkName.Left(6)=="CSBNUM" ? pARoadOptionStd->m_LayerList[HCAD_BARC] : szCovBlkName.Left(7)=="CSBSHPE" ? pARoadOptionStd->m_LayerList[HCAD_BARA] : pARoadOptionStd->m_LayerList[HCAD_SYMB];
// 				pInsert->SetsLayer(pARoadOptionStd->m_szDetLayerCode + (szCovBlkName == "CSBSEC1" ? pLayer1->m_szCadLayername : pLayer2->m_szCadLayername));
// 				pInsert->m_sBlockName = szCovBlkName;
// 				pInsert->m_vPoint = xy;	
// 				pInsert->m_vSFactor = CVector(dScaleHor*dScaleFactor,dScaleHor*dScaleFactor,dScaleHor*dScaleFactor);
// 				pARoadOptionStd->ChangeLayerOnSet(pInsert);
// 				pDomP->Insert(pInsert);				
// 				return pInsert;
// 			}
// 		}
// 	}
// 	else
// 	{
// 		if(m_domRcStdStandard.IsExistBlock(szCovBlkName+"-NOATTR"))
// 		{
// 			CMapStringToPtr* pBlockArray = m_domRcStdStandard.GetBlockArray();
// 			CDomyun Dom(pDomP);
// 			CDomyunBase* p;					
// 			if( pBlockArray->Lookup( szCovBlkName+"-NOATTR", (void *&)p ) )
// 			{				
// 				pARoadOptionStd->ChangeDomLastLayerNameForAPier(p);
// 				if(	szCovBlkName == HCAD_E00 || 
// 						szCovBlkName == HCAD_E01 || 
// 						szCovBlkName == HCAD_E00_KCFIT ||
// 						szCovBlkName == HCAD_E08 || // added by jaeho 2007.12.06
// 						szCovBlkName == HCAD_E09 )	// added by jaeho 2007.12.06
// 					pARoadOptionStd->ChangeRebarMark(p, TRUE);
// 				Dom += *p;												
// 				Dom.RedrawByScale(dScaleHor*dScaleFactor);
// 				Dom.Move(xy);
// 				if(dRotDegAng!=0)
// 					Dom.Rotate(xy,ToDPointFrDegree(dRotDegAng));
// 				*pDomP << Dom;				
// 			}
// 		}
// 	}
// 
// 	return NULL;
// }

// ����3 - ������
CDRect CDrawingSlabStandardOutput::DrawRcStd3DesignMethod(CDomyun *pDom)
{
	if(!pDom) return CDRect(0, 0, 0, 0);
	
	CDomyun Dom(pDom);
	
	CString sBlkName	= _T("RCSTD3_DESIGN");
	CDPoint xy	= CDPoint(0, 0);
	DxfIn(&Dom, sBlkName, 1, 1);
	


	Dom.SetCalcExtRect();
	CDRect rect	= Dom.GetExtRect();
	*pDom << Dom;

	return rect;
}

// ����3 - ����������
CDRect CDrawingSlabStandardOutput::DrawRcStd3Spacer(CDomyun *pDom)
{
	if(!pDom) return CDRect(0, 0, 0, 0);
	
	CDomyun Dom(pDom);
	
	CString sBlkName	= _T("RCSTD3_SPACER");
	CDPoint xy	= CDPoint(0, 0);
	DxfIn(&Dom, sBlkName, 1, 1);
	


	Dom.SetCalcExtRect();
	CDRect rect	= Dom.GetExtRect();
	*pDom << Dom;

	return rect;
}

// ����3 - �󼼵� �ܰ�
CDRect CDrawingSlabStandardOutput::DrawRcStd3DetailOutLine(CDomyun *pDom)
{
	if(!pDom) return CDRect(0, 0, 0, 0);
	
	CDomyun Dom(pDom);
	
	CString sBlkName	= _T("RCSTD3_DETAIL_OUTLINE");
	CDPoint xy	= CDPoint(0, 0);
	DxfIn(&Dom, sBlkName, 1, 1);
	


	Dom.SetCalcExtRect();
	CDRect rect	= Dom.GetExtRect();
	*pDom << Dom;

	return rect;
}

CDRect CDrawingSlabStandardOutput::DrawDiagramBMD_SFD(CDomyun *pDomP, BOOL bBMD, BOOL bSFD, BOOL bAXIAL, BOOL bWhenDiagramIsNoneDrawRect, BOOL bInput)
{
	CARcBridgeDrawingStd* pStd			= (CARcBridgeDrawingStd*)m_pStd;
	CRcBridgeRebar *pBri				= GetRcBridgeRebar();
	CARcBridgeDrawingOptionStd* pOpt	= pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData	= pOpt->GetDrawingOption(m_nBridge);

	CARcBridgeDBStandard DBStd(pBri, pStd->m_pARoadOptionStd, pStd->m_pARcBridgeDataStd);
	DBStd.SetSelectFckDomText(pOpt->m_nSelectFckDomText);
	DBStd.m_pDrawingOptionData	= pData;
	
	CDomyun Dom(pDomP);

	DBStd.m_pDrawVertSection->DrawDiagramBMD_SFD(&Dom, bBMD, bSFD, bAXIAL, bWhenDiagramIsNoneDrawRect, bInput);

	Dom.SetCalcExtRect();
	CDRect rect	= Dom.GetExtRect();

	*pDomP << Dom;

	CString sBlkName	= _T("RCSTD2_BMDSFD_OUTLINE");
	DxfIn(&Dom, sBlkName, rect.Width(), rect.Height());

	Dom.SetCalcExtRect();
	CDRect rcDxf	= Dom.GetExtRect();

	Dom.Move(rect.left - rcDxf.left, rect.top - rcDxf.top);

	*pDomP << Dom;

	return rect;
}

// �����걳 ǥ�� ������(4)
void CDrawingSlabStandardOutput::AddPageOne4(CDomyun *pDomP, CDRect rcDogak)
{
	CARcBridgeDrawingStd* pStd			= (CARcBridgeDrawingStd*)m_pStd;
	CRcBridgeRebar *pBri				= GetRcBridgeRebar();
	CARcBridgeDrawingOptionStd* pOpt	= pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData	= pOpt->GetDrawingOption(m_nBridge);
	CDrawPageMng* pPageMng				= pStd->GetPageManage();
	COutMovePoint* pMove				= pStd->GetMovePoint(pStd->m_nCurPage);

	CARcBridgeDBStandard DBStd(pBri, pStd->m_pARoadOptionStd, pStd->m_pARcBridgeDataStd);
	DBStd.SetSelectFckDomText(pOpt->m_nSelectFckDomText);
	DBStd.m_pDrawingOptionData	= pData;
	double dScale	= GetScale();

	CDomyun Dom(pDomP);
	Dom.SetScaleDim(dScale);

	// ����4 - ö�����ǥ //////////////////////////////////////////////////////////////////////
	DrawRebarBM(&Dom, rcDogak);
	////////////////////////////////////////////////////////////////////////////////////

	*pDomP << Dom;
}


void CDrawingSlabStandardOutput::DrawRebarDetail(CDomyun *pDomP, CTypedPtrArray <CObArray, CRebar*> *pArrRebar, double dMaxYIn, double dMinYIn)
{
	CARcBridgeDrawingStd* pStd			= (CARcBridgeDrawingStd*)m_pStd;
	CARcBridgeDrawingOptionStd* pOpt	= pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData	= pOpt->GetDrawingOption(m_nBridge);
	CDrawPageMng* pPageMng				= pStd->GetPageManage();
	COutMovePoint* pMove				= pStd->GetMovePoint(pStd->m_nCurPage);

	CConcSymbol symDB(pStd->m_pARoadOptionStd);
	CDomyun Dom(pDomP);

	double dSpace = pData->m_dSpaceBetweenRebarDetail;
	double dMaxY  = Dom.Always(dMaxYIn);
	double dMinY  = Dom.Always(dMinYIn);
	double dYPlus = 0;
	CDPoint xy(Dom.Always(70),dMaxY);
	CRebar *pRB;

	if(m_pStd->IsDrawByBlock(MPE_RCSTD4_REBAR_DETAIL_TITLE))
	{
		pStd->m_pARoadOptionStd->SetEnvType(&Dom,HCAD_SUB_TITLE);
		Dom.TextOut(xy.x+Dom.Always(50),xy.y+Dom.GetTextHeight()/2,"ö �� �� ��");
		Dom.Move(pMove->GetPoint(MPE_RCSTD4_REBAR_DETAIL_TITLE));
		pStd->ChangeSeperateLayer(&Dom);
		Dom.SetCalcExtRect();
		pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_RCSTD4_REBAR_DETAIL_TITLE, "ö�ٻ� ����");
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
				if(m_pStd->IsDrawByBlock(MPE_RCSTD4_REBAR_DETAIL+nDetailCol*1))
				{
					Dom.Move(pMove->GetPoint(MPE_RCSTD4_REBAR_DETAIL+nDetailCol*1));
					pStd->ChangeSeperateLayer(&Dom);
					Dom.SetCalcExtRect();
					pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_RCSTD4_REBAR_DETAIL+nDetailCol*1, "ö�ٻ� "+COMMA(1+nDetailCol));
					*pDomP << Dom;
				}
				else
				{
					Dom.ClearEtt();
				}
				nDetailCol++;
				xy.x += Dom.Always(120);
				xy.y  = dMaxY;
				nRow  = 3;
			}
			xy.y   = dMaxY - dSpace*nRow;
			dYPlus = (nType==1) ? Dom.Always(8) : 0;
			symDB.RebarDetailBasic(&Dom,nType,CDPoint(xy.x+Dom.Always(50),xy.y+Dom.Always(10)+dYPlus),bJoint);
		}
		else
			xy.y = dMaxY - dSpace*nRow;
		if(xy.y < dMinY)
		{
			if(m_pStd->IsDrawByBlock(MPE_RCSTD4_REBAR_DETAIL+nDetailCol*1))
			{
				Dom.Move(pMove->GetPoint(MPE_RCSTD4_REBAR_DETAIL+nDetailCol*1));
				pStd->ChangeSeperateLayer(&Dom);
				Dom.SetCalcExtRect();
				pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_RCSTD4_REBAR_DETAIL+nDetailCol*1, "ö�ٻ� "+COMMA(1+nDetailCol));
				*pDomP << Dom;
			}
			else
			{
				Dom.ClearEtt(TRUE, FALSE);
			}
			nDetailCol++;
			xy.x += Dom.Always(120);
			xy.y  = dMaxY;
			nRow  = 0;
		}
		symDB.DrawJewonBar(&Dom,pRB,xy,pStd->m_pARcBridgeDataStd->m_pBasicConcInfo->m_nRoundRebarLength);
		nRow++;
	}
	if(m_pStd->IsDrawByBlock(MPE_RCSTD4_REBAR_DETAIL+nDetailCol*1))
	{
		Dom.Move(pMove->GetPoint(MPE_RCSTD4_REBAR_DETAIL+nDetailCol*1));
		pStd->ChangeSeperateLayer(&Dom);
		Dom.SetCalcExtRect();
		pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_RCSTD4_REBAR_DETAIL+nDetailCol*1, "ö�ٻ� "+COMMA(1+nDetailCol));
		*pDomP << Dom;
	}
}


CDPoint CDrawingSlabStandardOutput::DrawRebarTable(CDomyun *pDomP, CTypedPtrArray <CObArray, CRebar*> *pArrRebar, CString sSubTitle, long nIdxTable, CDPoint xyBase, CDRect rcDogak)
{
	CARcBridgeDrawingStd* pStd			= (CARcBridgeDrawingStd*)m_pStd;
	CRcBridgeRebar *pBri				= GetRcBridgeRebar();
	CARcBridgeDrawingOptionStd* pOpt	= pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData	= pOpt->GetDrawingOption(m_nBridge);
	CDrawPageMng* pPageMng				= pStd->GetPageManage();
	COutMovePoint* pMove				= pStd->GetMovePoint(pStd->m_nCurPage);

	CBasicConcInfo *pBCI = pStd->m_pARcBridgeDataStd->m_pBasicConcInfo;
	
	CGridDomyunEx Dom(pDomP);
	
	long nDefTableTitle	= MPE_RCSTD4_REBAR_TABLE_TITLE;
	
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

	double dLengthSum		= 0;
	double dLengthSumAdd	= 0;
	double dLengthTotal		= 0;
	long nMaxRow			= pData->m_nCountRowEachTable;
	long nDefTable			= MPE_RCSTD4_REBAR_TABLE;
	double dWeightTotal		= 0;
	double dWeightTotalAdd	= 0;


	CHgBaseDrawStd baseDraw;
	double Fy = pData->m_bRebarTableSepByFy && pArrRebar->GetSize() > 0 ? pArrRebar->GetAt(0)->m_Fy : pBri->m_pARcBridgeDataStd->m_Fy;

	// ö�����ǥ �׸��� ������ ����
	if(pBri->m_pARcBridgeDataStd->m_nTypeUnit == 1)
		Fy = Fy * 10;

	BOOL bShadeOutLone = TRUE;//(pOpt->m_dDrawGen & GEBDRAW_GEN_TABLESHADE) ? TRUE : FALSE;

	long nIndex = 0;
	long nTableSu = 0;
	while(1)
	{
		if(nMaxRow <= 0) break;
		if(nIndex >= pArrRebar->GetSize()) break;
		nIndex = baseDraw.DrawRebarTable(&Dom, nIndex, nMaxRow, xyBase, dLengthSum, dLengthSumAdd, dLengthTotal, dWeightTotal, dWeightTotalAdd, *pArrRebar, Fy, bShadeOutLone, "", pBCI, pBCI->m_nRoundRebarLength);
		if(m_pStd->IsDrawByBlock(nDefTable+nTableSu))
		{
			Dom.Move(pMove->GetPoint(nDefTable+nTableSu));
			pStd->ChangeSeperateLayer(&Dom);
			Dom.SetCalcExtRect();
			pMove->AddMPE(&Dom, Dom.GetExtRect(), nDefTable+nTableSu, "ö�����ǥ");
			*pDomP << Dom;
		}
		else
		{ 
			Dom.ClearEtt(TRUE, FALSE);
			*pDomP << Dom;
		}
		xyBase.x += Dom.Always(155 + 10);
		nTableSu++;
	}		

	*pDomP << Dom;

	return xyBase;
}

void CDrawingSlabStandardOutput::DrawRebarBM(CDomyun *pDomP, CDRect rcDogak)
{
	CARcBridgeDrawingStd* pStd			= (CARcBridgeDrawingStd*)m_pStd;
	//CRcBridgeRebar *pBri				= GetRcBridgeRebar();
	CARcBridgeDrawingOptionStd* pOpt	= pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData	= pOpt->GetDrawingOption(m_nBridge);
	CDrawPageMng* pPageMng				= pStd->GetPageManage();
	COutMovePoint* pMove				= pStd->GetMovePoint(pStd->m_nCurPage);

	CHgBaseConcStd baseConc;
	CTypedPtrArray <CObArray, CRebar*> arrRebar;	// ���ǥ�� ö�ٸ���Ʈ
	CDomyun Dom(pDomP);

	MakeRebarList(&arrRebar);
	//pBri->MakeRebarList(&arrRebar, -1, TRUE, TRUE, -1, FALSE);
	baseConc.ShellSortRebarList(&arrRebar, &CRebar::CompareForList);

	// ö�ٻ� //////////////////////////////////////////////////////////////////////
	BOOL bRebarBMDetail	= TRUE;
	BOOL bRebarBMTable	= TRUE;
	if(bRebarBMDetail)
	{
		pPageMng->SetProgressData("ö�ٻ� �����...",30);
		DrawRebarDetail(&Dom, &arrRebar);
		Dom.SetCalcExtRect();
	}

	CDRect rect	= Dom.GetExtRect();
	////////////////////////////////////////////////////////////////////////////////////

	// ���ǥ�� �ɼǿ� ���� 1~3������ ���еǾ� ��µ� �� �ִ�.
	CDPoint xyBase = CDPoint(Dom.Always(650),Dom.Always(525));
	
	// �󼼰� �ʹ� ���� ��� ���ǥ�� �и� �� �ִ�. 
	if(rect.right + 1000 > xyBase.x)
		xyBase.x = rect.right + 1000;

	/*
	// ���� �󼼴� ������� �ʰ� ���ǥ�� ����Ѵٸ� ���ǥ�� ������ ����.
	if(!bRebarBMDetail && bRebarBMTable)
	{
		xyBase.x = Dom.Always(70);
	}
	*/

	if(bRebarBMTable)
	{
		// 1. ��ü ���
		if(pData->m_nTypeOutRebarTable == 0)
		{
			pPageMng->SetProgressData("ö�����ǥ �����...",80);
			if(pData->m_bRebarTableSepByFy)
			{
				double dFy	= 1;
				while(dFy != 0)
				{
					CTypedPtrArray<CObArray, CRebar*> arrRebarGet;
					dFy	= pStd->GetRebarListByFy(&arrRebar, &arrRebarGet, dFy);
					if(dFy == 0) break;
					if(arrRebarGet.GetSize() == 0) break;

						baseConc.ShellSortRebarList(&arrRebarGet, &CRebar::CompareForTable);
						if(arrRebar.GetSize() > 0)
							xyBase	= DrawRebarTable(&Dom, &arrRebarGet, "", 0, xyBase, rcDogak);
				}
			}
			else
			{
				baseConc.ShellSortRebarList(&arrRebar, &CRebar::CompareForTable);
				if(arrRebar.GetSize() > 0)
					xyBase	= DrawRebarTable(&Dom, &arrRebar, "", 0, xyBase, rcDogak);
			}
		}
		// 2. ��ö�ٰ� ��Ÿö�� ����
		else if(pData->m_nTypeOutRebarTable == 1)
		{
			const long nCountTable		= 2;
			long nIdxTable				= 0;
			CString sTitle[nCountTable]	= {"��ö��", "��Ÿö��"};

			for(long table = 0; table < nCountTable; table++)
			{
				MakeRebarList(&arrRebar);
				//pBri->MakeRebarList(&arrRebar, -1, TRUE, TRUE, 0, FALSE);
				if(arrRebar.GetSize() == 0) continue;

				long i = 0; for(i = 0; i < arrRebar.GetSize(); i++)
				{
					if(table == 0)
					{
						if(!arrRebar.GetAt(i)->m_bMainRebar)
						{
							delete arrRebar.GetAt(i);
							arrRebar.RemoveAt(i--);
						}
					}
					else
					{
						if(arrRebar.GetAt(i)->m_bMainRebar)
						{
							delete arrRebar.GetAt(i);
							arrRebar.RemoveAt(i--);
						}
					}
				}

				pPageMng->SetProgressData("ö�����ǥ �����...",80);

				if(pData->m_bRebarTableSepByFy)
				{
					double dFy	= 1;
					while(dFy != 0)
					{
						CTypedPtrArray<CObArray, CRebar*> arrRebarGet;
						dFy	= pStd->GetRebarListByFy(&arrRebar, &arrRebarGet, dFy);
						if(dFy == 0) break;
						if(arrRebarGet.GetSize() == 0) break;

						baseConc.ShellSortRebarList(&arrRebarGet, &CRebar::CompareForTable);
						xyBase	= DrawRebarTable(&Dom, &arrRebarGet, sTitle[table], nIdxTable++, xyBase, rcDogak);
					}
				}
				else
				{
					baseConc.ShellSortRebarList(&arrRebar, &CRebar::CompareForTable);
					xyBase	= DrawRebarTable(&Dom, &arrRebar, sTitle[table], nIdxTable++, xyBase, rcDogak);
				}
			}
		}

		*pDomP << Dom;
	}

	*pDomP << Dom;

	AhTPADelete(&arrRebar,	(CRebar*)0);
}

// ����1 - ǥ�شܸ鵵
CDRect CDrawingSlabStandardOutput::DrawRcStd1StandardOutLine(CDomyun *pDom)
{
	if(!pDom) return CDRect(0, 0, 0, 0);
	
	CARcBridgeDrawingStd* pStd			= (CARcBridgeDrawingStd*)m_pStd;
	CARcBridgeDrawingOptionStd* pOpt	= pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData	= pOpt->GetDrawingOption(m_nBridge);
	CRcBridgeRebar *pBri = GetRcBridgeRebar();

	CARcBridgeDBStandard DBStd(pBri, pStd->m_pARoadOptionStd, pStd->m_pARcBridgeDataStd);
	DBStd.SetSelectFckDomText(pOpt->m_nSelectFckDomText);
	DBStd.m_pDrawingOptionData	= pData;

	CDomyun Dom(pDom);

	CString sBlkName	= _T("RCSTD1_STANDARD_OUTLINE");
	DxfIn(&Dom, sBlkName, 2, 2);
	
	Dom.SetCalcExtRect();
	CDRect rect	= Dom.GetExtRect();
	
	*pDom << Dom;

	return rect;
}

void CDrawingSlabStandardOutput::DxfIn(CDomyun *pDom, CString sDxfName, double dScaleHor, double dScaleVer)
{
	CString szPath = AfxGetApp()->GetProfileString("Afx Directory", "Fixed Directory");
	CString strDxfName = _T("");

	strDxfName.Format("%s\\DXF\\%s.dxf", szPath, sDxfName);
	pDom->DxfIn(strDxfName);
	pDom->RedrawByHVScale(dScaleHor, TRUE);
	pDom->RedrawByHVScale(dScaleVer, FALSE);

	// text ������ ����
	CPtrList *pTextArr	= pDom->GetObjectList(ETT_TEXT);
	if(pTextArr) 
	{
		POSITION pos	= pTextArr->GetHeadPosition();
		while(pos)
		{
			CObText *pObText	= (CObText*)pTextArr->GetNext(pos);
			if(!pObText) continue;

			pObText->m_TextStyle.Height *= dScaleVer;
		}
	}

}

// void CDrawingSlabStandardOutput::ChangeTextAtDom(CDomyun *pDom, CString sBefore, CString sAfter)
// {
// 	pDom->ReplaceString(sBefore, sAfter);
// }

// ����1 - ��鵵
CDRect CDrawingSlabStandardOutput::DrawRcStd1Plan(CDomyun *pDom)
{
	if(!pDom) return CDRect(0, 0, 0, 0);
	
	CARcBridgeDrawingStd* pStd			= (CARcBridgeDrawingStd*)m_pStd;
	CARcBridgeDrawingOptionStd* pOpt	= pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData	= pOpt->GetDrawingOption(m_nBridge);
	CRcBridgeRebar *pBri = GetRcBridgeRebar();

	CARcBridgeDBStandard DBStd(pBri, pStd->m_pARoadOptionStd, pStd->m_pARcBridgeDataStd);
	DBStd.SetSelectFckDomText(pOpt->m_nSelectFckDomText);
	DBStd.m_pDrawingOptionData	= pData;

	CDomyun Dom(pDom);


	pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
	DBStd.m_pDrawPlane->DrawPlaneGen(&Dom, 5000, TRUE, TRUE, FALSE, FALSE, FALSE);
	
	CDimDomyun DomDim(pDom, pBri->m_pLineApp);
	pStd->m_pARoadOptionStd->SetEnvType(&DomDim, HCAD_DIML);
	pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

	// ��������
	DBStd.m_pDrawPlane->DimPlaneJijumAngle(&Dom, FALSE);

	// �������� /////////
	CDoubleArray dArr;
	pBri->GetLenJijumForDim(dArr);
	double dStaStt	= pBri->GetStationBridgeStt(FALSE);
	double dDist	= pBri->m_dWidthSlabRight;
	dDist += DomDim.Always(DomDim.GetDimLevelDis() + DomDim.GetDimExo());
	CString sUpper	= _T("");
	CString sLower	= _T("");
	DomDim.SetDirection("BOTTOM");
	DomDim.DimBaseCurvePoint(dStaStt, dDist, 0);
	DomDim.DimCurveMoveTo(dStaStt, dDist, 0, FALSE, sUpper, sLower);

	long i = 0; for(i = 0; i < dArr.GetSize(); i++)
	{
		sUpper.Format("%.3f", dArr.GetAt(i));
		sLower	= _T("");
		DomDim.DimCurveLineTo(dArr.GetAt(i), dDist, 0, sUpper, sLower);
	}

	Dom << DomDim;
	DBStd.RotateDomByBridgeSttEndXy(&Dom);
	
	Dom.SetCalcExtRect();
	CDRect rect	= Dom.GetExtRect();


	*pDom << Dom;

	

	return rect;
}

// ����2 - ��鵵
CDRect CDrawingSlabStandardOutput::DrawRcStd2Plan(CDomyun *pDom)
{
	if(!pDom) return CDRect(0, 0, 0, 0);
	
	CARcBridgeDrawingStd* pStd			= (CARcBridgeDrawingStd*)m_pStd;
	CARcBridgeDrawingOptionStd* pOpt	= pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData	= pOpt->GetDrawingOption(m_nBridge);
	CRcBridgeRebar *pBri = GetRcBridgeRebar();

	CARcBridgeDBStandard DBStd(pBri, pStd->m_pARoadOptionStd, pStd->m_pARcBridgeDataStd);
	DBStd.SetSelectFckDomText(pOpt->m_nSelectFckDomText);
	DBStd.m_pDrawingOptionData	= pData;

	CDomyun Dom(pDom);
	
	DBStd.m_pDrawPlane->DrawRebar_Plane_UpperSlabStd(&Dom, -1, TRUE, FALSE, TRUE, TRUE, TRUE, 2, TRUE);


	Dom.SetCalcExtRect();
	CDRect rect	= Dom.GetExtRect();
	*pDom << Dom;

	return rect;
}

// ����2 - �а���(��,�ϸ�)
CDRect CDrawingSlabStandardOutput::DrawRcStd2ObtusPlan(CDomyun *pDom, BOOL bStt, BOOL bLeft, BOOL bUpper)
{
	if(!pDom) return CDRect(0, 0, 0, 0);
	
	CARcBridgeDrawingStd* pStd			= (CARcBridgeDrawingStd*)m_pStd;
	CARcBridgeDrawingOptionStd* pOpt	= pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData	= pOpt->GetDrawingOption(m_nBridge);
	CRcBridgeRebar *pBri = GetRcBridgeRebar();

	CARcBridgeDBStandard DBStd(pBri, pStd->m_pARoadOptionStd, pStd->m_pARcBridgeDataStd);
	DBStd.SetSelectFckDomText(pOpt->m_nSelectFckDomText);
	DBStd.m_pDrawingOptionData	= pData;

	CDomyun Dom(pDom);

	DBStd.m_pDrawPlane->DrawPlaneGenObtuse(&Dom, bStt, bLeft, bUpper);
	DBStd.m_pDrawPlane->DrawRebarPlane_Slab_Obtuse(&Dom,TRUE,  bStt, bLeft, bUpper, TRUE, TRUE, FALSE);



	Dom.SetCalcExtRect();
	CDRect rect	= Dom.GetExtRect();
	*pDom << Dom;

	return rect;
}

// ����2 - �а���(���ܸ�)
CDRect CDrawingSlabStandardOutput::DrawRcStd2ObtusSection(CDomyun *pDom, BOOL bStt, BOOL bLeft)
{
	if(!pDom) return CDRect(0, 0, 0, 0);
	
	CARcBridgeDrawingStd* pStd			= (CARcBridgeDrawingStd*)m_pStd;
	CARcBridgeDrawingOptionStd* pOpt	= pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData	= pOpt->GetDrawingOption(m_nBridge);
	CRcBridgeRebar *pBri = GetRcBridgeRebar();

	CARcBridgeDBStandard DBStd(pBri, pStd->m_pARoadOptionStd, pStd->m_pARcBridgeDataStd);
	DBStd.SetSelectFckDomText(pOpt->m_nSelectFckDomText);
	DBStd.m_pDrawingOptionData	= pData;

	CDomyun Dom(pDom);

	DBStd.m_pDrawVertSection->DrawRebarVertSectionObtuseStd(&Dom, bStt, bLeft, TRUE, TRUE);


	Dom.SetCalcExtRect();
	CDRect rect	= Dom.GetExtRect();
	*pDom << Dom;

	return rect;
}


void CDrawingSlabStandardOutput::MakeRebarList(CTypedPtrArray <CObArray, CRebar*> *pArrRebar)
{
	AhTPADelete(pArrRebar,	(CRebar*)0);

	CRcBridgeRebar *pBri = GetRcBridgeRebar();

	// ��ü
	pBri->MakeRebarList(pArrRebar, 0, TRUE, TRUE, 0);		// ��ü
	long i=0; for(i=0; i<=pBri->m_nQtyJigan; i++)
	{
		pBri->MakeRebarList(pArrRebar, 1, TRUE, TRUE, i);	// �߰�����(����)
		pBri->MakeRebarList(pArrRebar, 5, TRUE, TRUE, i);	// �߰�����(����)
		pBri->MakeRebarList(pArrRebar, 6, TRUE, TRUE, i);	// ������� �ƴ�(�Ŵ�ö��)
		pBri->MakeRebarList(pArrRebar, 7, TRUE, TRUE, i);	// �������(�Ŵ�ö��)
	}
	pBri->MakeRebarList(pArrRebar, 2, TRUE, TRUE, i);		// ����
	pBri->MakeRebarList(pArrRebar, 4, TRUE, TRUE, i);		// �󼼺�(����)

}