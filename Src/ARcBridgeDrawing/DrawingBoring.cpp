// DrawingBoring.cpp: implementation of the CDrawingBoring class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../../Src/ARcBridgeData/ARcBridgeData.h"
#include "../../Src/ARcBridgeCalculate/ARcBridgeCalculate.h"
#include "../../Src/ARcBridgeDBDraw/ARcBridgeDBDraw.h"
#include "../ARcBridgeDrawingOption/ARcBridgeDrawingOption.h"
#include "ARcBridgeDrawing.h"
#include "DrawingBoring.h"

#define MPE_VERSECT			10
#define MPE_FORCE_TABLE1	20
#define MPE_FORCE_TABLE2	30
#define MPE_LEGEND			40
#define MPE_NOTE			50

#define MPE_TITLE_VERSECT		11
#define MPE_TITLE_FORCE			21
#define MPE_TITLE_LEGEND		41
#define MPE_TITLE_NOTE			51
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDrawingBoring::CDrawingBoring()
{
	m_pCalc = NULL;
	m_nBridge = -1;
}

CDrawingBoring::~CDrawingBoring()
{

}

long CDrawingBoring::GetPageSu()
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;

	if(pStd->GetCountTotalPage()<=0)
		pStd->MakePageInfo();

	m_nPage	= pStd->GetCountCategoryPage(_T("지질 주상도"));

	return m_nPage;
}

CString CDrawingBoring::GetTitle()
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;
	CPageInfo* pInfo = pStd->GetPageInfoOrg(_T("지질 주상도"), pStd->m_nCurPage-m_nSttPage);
	if(pInfo==NULL) return _T("");

	CString strTitle = pStd->GetStringTitle(pStd->m_pARcBridgeDataStd->GetBridge(m_nBridge), pInfo->m_strTitle);

	return strTitle;
}

double CDrawingBoring::GetScale()
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;
	CARcBridgeDrawingOptionStd* pOpt = pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pOptData = pOpt->GetDrawingOption(m_nBridge);

	m_Scale	= pOptData->m_dScaleBoring;
	return pOptData->m_dScaleBoring;
}

void CDrawingBoring::SetNumber(long nSubPage)
{
	m_nBridge = GetNumberBridge(nSubPage-m_nSttPage);
}

long CDrawingBoring::GetNumberBridge(long nCategoryPageNum)
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;
	CPageInfo* pInfo = pStd->GetPageInfoOrg(_T("지질 주상도"), nCategoryPageNum);
	if(pInfo==NULL) return -1;

	return pInfo->m_nBri;
}

void CDrawingBoring::DrawOnePage()
{
	TRACE("CDrawingBoring");
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

	CPageInfo* pInfo = pStd->GetPageInfoOrg(_T("지질 주상도"), pStd->m_nDrawPage-m_nSttPage);
	if(pInfo==NULL) return;

	nPage = pStd->GetPageNum(pInfo);

	pStd->m_pOptStd->SetEnvType(pDom, HCAD_MIS3);
	pStd->m_pOptStd->SetEnvType(pDom, HCAD_SYMB);

	CDomyun Dom(pDom);

	pMng->SetProgressData(_T("도각 출력 중..."), 0);

	if(m_bDrawForMe)
	{
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

		*pDom << Dom;
	}

	// 지질 주상도 출력
	AddPageOne(&Dom, nSubPage);

	pMng->SetProgressData(_T("출력 완료  ..."), 100);

	*pDom << Dom;
}

void CDrawingBoring::AddPageOne(CDomyun* pDomP, long nSubPage)
{
	CARcBridgeDrawingStd* pStd			= (CARcBridgeDrawingStd*)m_pStd;
	CRcBridgeRebar *pBri				= GetRcBridgeRebar();
	CARcBridgeDrawingOptionStd* pOpt	= pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData	= pOpt->GetDrawingOption(m_nBridge);
	CDrawPageMng* pPageMng				= pStd->GetPageManage();
	COutMovePoint* pMove				= pStd->GetMovePoint(pStd->m_nCurPage);

	double dScaleBorin = pData->m_dScaleBoring;
	DWORD dOptDraw	= pData->m_dwDrawBoring;
	
	CDRect rcVerSect;
	CDomyun Dom(pDomP);
	Dom.SetScaleDim(GetScale());
	
	if(dOptDraw & BORING_VERSECT)
	{
		if(m_pStd->IsDrawByBlock(MPE_VERSECT))
		{
			pPageMng->SetProgressData(_T("종단면도 출력 중..."), 20);
			rcVerSect = AddVerSect(&Dom);
			
			Dom.SetCalcExtRect();
			Dom.Move((Dom.Always(530)-rcVerSect.Width())/2, (Dom.Always(600)-rcVerSect.Height())/2);
			Dom.Move(pMove->GetPoint(MPE_VERSECT));
			pStd->ChangeSeperateLayer(&Dom);
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_VERSECT, _T("종단면도"));
			Dom.SetCalcExtRect();
			rcVerSect = Dom.GetExtRect();
			*pDomP << Dom;
		}

		if(m_pStd->IsDrawByBlock(MPE_TITLE_VERSECT))
		{
			pStd->AddTitleOnDom(&Dom, m_nBridge, _T("종단면도"), CDPoint(0,0), 0, dScaleBorin);
			Dom.Move(Dom.Always(530)/2, (Dom.Always(600)+rcVerSect.Height())/2.0+GAP_TITLE);
			Dom.Move(pMove->GetPoint(MPE_TITLE_VERSECT));
			pStd->ChangeSeperateLayer(&Dom);
			Dom.SetCalcExtRect();
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_TITLE_VERSECT, _T("종단면도 제목"));
			
			*pDomP << Dom;
		}
	}
	
	if(dOptDraw & BORING_FORCE && (pBri->m_FemNormal.IsOutputLoaded() && pBri->GetFemManage(COMBO_EARTHQUAKE)->IsOutputLoaded()))
	{
		CDRect rcTable1, rcTable2;
		if(m_pStd->IsDrawByBlock(MPE_FORCE_TABLE1))
		{
			pPageMng->SetProgressData(_T("기초검토 테이블 출력 중..."), 40);
			if(pBri->IsBoxType())
			{
				if(pBri->m_pARcBridgeDataStd->IsLsdDesign())
					rcTable1 = AddForceTableDirect_BoxType_Lsd(&Dom);
				else
					rcTable1 = AddForceTableDirect_BoxType_Usd(&Dom);
			}
			else
				rcTable1 = AddForceTableDirect(&Dom);
			Dom.SetCalcExtRect();
			Dom.Move(Dom.Always(530), Dom.Always(450));
			Dom.Move(pMove->GetPoint(MPE_FORCE_TABLE1));
			pStd->ChangeSeperateLayer(&Dom);
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_FORCE_TABLE1, _T("Table1"));
			*pDomP << Dom;
		}

		if(m_pStd->IsDrawByBlock(MPE_FORCE_TABLE2))
		{
			rcTable2 = AddForceTablePile(&Dom);
			Dom.SetCalcExtRect();
			Dom.Move(Dom.Always(530), Dom.Always(440)-rcTable1.Height());
			Dom.Move(pMove->GetPoint(MPE_FORCE_TABLE2));
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_FORCE_TABLE2, _T("Table2"));
			*pDomP << Dom;
		}
	}

	// 주상도 범례
	if(dOptDraw & BORING_LEGEND)
	{
		CDRect rcBum;
		if(m_pStd->IsDrawByBlock(MPE_LEGEND))
		{
			pPageMng->SetProgressData(_T("주상도 범례 출력 중..."), 60);
			rcBum = AddBoringLegend(&Dom);
			Dom.SetCalcExtRect();
			Dom.Move(Dom.Always(530), Dom.Always(250));
			Dom.Move(pMove->GetPoint(MPE_LEGEND));
			pStd->ChangeSeperateLayer(&Dom);
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_LEGEND, _T("범례"));
			*pDomP << Dom;
		}
			
		if(m_pStd->IsDrawByBlock(MPE_TITLE_LEGEND))
		{
			pStd->AddTitleOnDom(&Dom, m_nBridge, _T("범례"), CDPoint(0,0), 0, -1);
			Dom.Move(Dom.Always(530)+rcBum.Width()/2.0, Dom.Always(255)+rcBum.Height());
			Dom.Move(pMove->GetPoint(MPE_TITLE_LEGEND));
			pStd->ChangeSeperateLayer(&Dom);
			Dom.SetCalcExtRect();
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_TITLE_LEGEND, _T("범례 제목"));
			*pDomP << Dom;
		}
	}

	// NOTE
	if(dOptDraw & BORING_NOTE)
	{
		CDRect rcNote;
		if(m_pStd->IsDrawByBlock(MPE_NOTE))
		{
			pPageMng->SetProgressData(_T("NOTE 출력 중..."), 80);
//			rcNote = AddNote(&Dom);
			AddNote(&Dom);
			Dom.SetCalcExtRect();
			Dom.Move(Dom.Always(530), Dom.Always(80));
			Dom.Move(pMove->GetPoint(MPE_NOTE));
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_NOTE, _T("NOTE"));
			
			Dom.SetCalcExtRect();
			rcNote = Dom.GetExtRect();
			*pDomP << Dom;
		}
			
		if(m_pStd->IsDrawByBlock(MPE_TITLE_NOTE))
		{
			pStd->AddTitleOnDom(&Dom, m_nBridge, _T("NOTE"), CDPoint(0,0), 0, -1);
			Dom.SetCalcExtRect();
			Dom.Move(Dom.Always(530)+rcNote.Width()/2.0, Dom.Always(80)+rcNote.Height()+GAP_TITLE);
			Dom.Move(pMove->GetPoint(MPE_TITLE_NOTE));
			pStd->ChangeSeperateLayer(&Dom);
			Dom.SetCalcExtRect();
			pMove->AddMPE(&Dom, Dom.GetExtRect(), MPE_TITLE_NOTE, _T("NOTE"));
			*pDomP << Dom;
		}
	}
}

CDRect CDrawingBoring::AddVerSect(CDomyun* pDomP)
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;
	CARcBridgeDrawingOptionStd* pOpt = pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData = pOpt->GetDrawingOption(m_nBridge);
	
	CARcBridgeDataStd* pDataStd = pStd->m_pARcBridgeDataStd;
	CRcBridgeApp* pBri = pDataStd->GetBridge(m_nBridge);
	CRcBridgeRebar* pBriRebar = pDataStd->GetBridgeRebar(m_nBridge);

	//////////////////////////////////////////////////////////////////////////
	// 종단면도
	//long nOldTypeJongDanRebar	= pBri->m_nTypeJongdanForRebar;
	//pBri->m_nTypeJongdanForRebar	= 2;
///	pBri->SyncBridge(0, TRUE);
	pBri->GetTvVertSection_Std(0, TRUE, FALSE, FALSE, 2);


	CARcBridgeDBStandard DBStd(pBriRebar, pStd->m_pARoadOptionStd, pDataStd);
	DBStd.SetSelectFckDomText(pOpt->m_nSelectFckDomText);
	double dScale = GetScale();
	BOOL bSkew = pBri->IsSkew();
	
	CDomyun Dom(pDomP);
	
	Dom.SetScaleDim(dScale);
	//////////////////////////////////////////////////////////////////////////
	pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONC);
	DBStd.m_pDrawVertSection->DrawVertSectionAll(&Dom, TRUE, TRUE, TRUE, FALSE);
	DBStd.m_pDrawVertSection->DrawVertSectionCenterLine(&Dom, FALSE, TRUE, FALSE);
	
	pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);
	DBStd.m_pDrawVertSection->DimTextVerSectionEL(&Dom);
	DBStd.m_pDrawVertSection->DimVertSectionUpper(&Dom, 1, TRUE, FALSE);

	if(pBri->IsBoxType())
		DBStd.m_pDrawVertSection->DimVertSectionLowerSlab(&Dom, 0, FALSE, TRUE, FALSE);
	else
	{
		long nCntFooting = pBri->GetCountFooting();
		long i=0; for(i=0; i<nCntFooting; i++)
		{
			CFootingApp *pFooting = pBri->GetFooting(i);
			if(pFooting)
			{
				DBStd.m_pDrawExFooting->DimTextFrontEL(&Dom, &pFooting->m_exFooting, FALSE);
			}
		}
	}
	
// 	long nCntBoring = pDataStd->m_pArrBoringEx.GetSize();
// 	long i=0; for(i=0; i<nCntBoring; i++)
// 	{
// 		CBoringDataEx *pBoring = pDataStd->m_pArrBoringEx.GetAt(i);
// 		double dStaBroing = pBoring->m_dStation;
// //		double dStaStt = toM(pBri->m_dStationBridgeStt-5000);
// //		double dStaEnd = toM(pBri->m_dStationBridgeStt+pBri->m_dLengthBridge+5000);
// 		double dStaStt = toM(DBStd.m_pDrawLine->GetStationJongDanStt());
// 		double dStaEnd = toM(DBStd.m_pDrawLine->GetStationJongDanEnd());
// 		if(dStaBroing>dStaStt && dStaBroing<dStaEnd)
// 		{
// 			pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CUTL);
// 			DBStd.ApplyDimSetting(pDomP, &Dom);
// 			DBStd.m_pDrawLine->DrawBroing(&Dom, pBoring, FALSE, FALSE);
// 		}
// 	}
	
	//pBri->m_nTypeJongdanForRebar = nOldTypeJongDanRebar;
	pBri->GetTvVertSection_Std(0, TRUE, FALSE, TRUE, 2);

	double dStaStt = pBri->m_dStationBridgeStt-10000;
	double dStaEnd = pBri->m_dStationBridgeStt+pBri->m_dLengthBridge+10000;
	double dStaMarkG = pBri->m_dStationBridgeStt+pBri->m_dLengthBridge-(pBri->GetLengthJigan(pBri->m_nQtyJigan-1))/2;
	DBStd.m_pDrawVertSection->DrawElEarthLine(&Dom,dStaStt,dStaEnd,TRUE,FALSE, TRUE, dStaMarkG);

	CDRect rc;

	if(bSkew)
	{
		Dom.SetCalcExtRect();
		CDRect rc = Dom.GetExtRect();
		pStd->m_pARoadOptionStd->SetEnvType(&Dom, HCAD_TXTG);
		Dom.SetTextAlignHorz(TA_RIGHT);
		Dom.SetTextHeight(4);
		Dom.TextOut(rc.right-5000, rc.top, "* ( ) 안의 치수는 직거리 치수임");
	}
	//////////////////////////////////////////////////////////////////////////
	Dom.SetCalcExtRect();
	rc = Dom.GetExtRect();
	
	Dom.Move(-rc.left, -rc.top);
	Dom.SetCalcExtRect();
	rc = Dom.GetExtRect();
	*pDomP << Dom;
	
	//pBri->GetTvVertSection_Std(0, TRUE, FALSE, TRUE, 2);
	
	return rc;
}

CRcBridgeRebar* CDrawingBoring::GetRcBridgeRebar()
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;
	CARcBridgeDataStd* pDataStd	= pStd->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = NULL;
	
	if(m_nBridge>-1 && pDataStd->GetBridgeSize()>m_nBridge)
		pBri = pDataStd->GetBridgeRebar(m_nBridge);
	
	return pBri;
}

CDRect CDrawingBoring::AddBoringLegend(CDomyun* pDomP)
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;
	CARcBridgeDrawingOptionStd* pOpt = pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData = pOpt->GetDrawingOption(m_nBridge);
	
	CARcBridgeDataStd* pDataStd = pStd->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = pDataStd->GetBridgeRebar(m_nBridge);
	CARcBridgeDBStandard DBStd(pBri, pStd->m_pARoadOptionStd, pDataStd);
	DBStd.SetSelectFckDomText(pOpt->m_nSelectFckDomText);
	
//	double dScale = 300;
	CDomyun Dom(pDomP);
	
//	Dom.SetScaleDim(dScale);
	
	double dStaStt = DBStd.m_pDrawLine->GetStationJongDanStt();
	double dStaEnd = DBStd.m_pDrawLine->GetStationJongDanEnd();
	DBStd.m_pDrawLine->DrawBoringLegend(&Dom, dStaStt, dStaEnd);
	
//	Dom.RedrawByScale(GetScale()/dScale);
	
	Dom.SetCalcExtRect();
	CDRect rc = Dom.GetExtRect();
	Dom.Move(-rc.left, -rc.top);
	
	*pDomP << Dom;
	
	return rc;
}

CDRect CDrawingBoring::AddForceTableDirect_BoxType_Usd(CDomyun* pDomP)
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;
	CARcBridgeDrawingOptionStd* pOpt = pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData = pOpt->GetDrawingOption(m_nBridge);

	CARcBridgeDataStd* pDataStd = pStd->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = pDataStd->GetBridgeRebar(m_nBridge);
	CRcBridgeRebar* pBriRebar = pDataStd->GetBridgeRebar(m_nBridge);
	CARcBridgeDBStandard DBStd(pBri, pStd->m_pARoadOptionStd, pDataStd);
	DBStd.SetSelectFckDomText(pOpt->m_nSelectFckDomText);

	double dScale = 100;
	CDomyun Dom(pDomP);

	CDRect rc(0, 0, 0, 0), rc1(0, 0, 0, 0);

	long nJijum = 0;
	long nCntItem = 1;
	if(pBri->IsPileBase(nJijum)) return rc;

	m_pCalc = new CARcBridgeCalcStd(pBriRebar);
	m_pCalc->m_pARoadOptionStd = pStd->m_pARoadOptionStd;
	m_pCalc->CalculateAll();

	// Title
	long nAlign = GRID_ALIGN_CENTER | GRID_ALIGN_VCENTER; 
	double dX = 0;
	double dY = 0;

	CString str = _T("");
	CGridDomyunEx Dom1(pDomP);

	pStd->m_pARoadOptionStd->SetEnvType(&Dom1,HCAD_TBL1);
	double TitleWidth1[] = {
		Dom1.Always(45),
		Dom1.Always(25),
		Dom1.Always(25),
		Dom1.Always(25) };

	CDPoint xyBase = CDPoint(dX, dY);
	Dom1.SetBasePoint(xyBase);
	Dom1.SetColumnCount(4);
	Dom1.SetRowCount(2+nCntItem);		
	Dom1.SetRowHeightAll(Dom1.Always(6));

	for(long i=0; i<Dom1.GetColumnCount(); i++)
	{
		Dom1.SetColumnWidth(i,TitleWidth1[i]);	
	}

	Dom1.SetMargin(5);
	Dom1.SetMergeCol(0, 1, 3);
	Dom1.SetMergeRow(0, 1, 0);

	long nRow = 0;
	Dom1.SetTextMatrix(nRow, 0, "구분", nAlign);
	Dom1.SetTextMatrix(nRow, 1, "지지력에 대한 안정", nAlign);

	nRow = 1;
	str.Format("Qmax(%s)", pDataStd->m_szTonf_M2);
	Dom1.SetTextMatrix(nRow, 1, str, nAlign);
	str.Format("Qa(%s)", pDataStd->m_szTonf_M2);
	Dom1.SetTextMatrix(nRow, 2, str, nAlign);
	Dom1.SetTextMatrix(nRow, 3, "안전율(%)", nAlign);

	BOOL bOutPutNormal = pBri->m_FemNormal.IsOutputLoaded();

	//////////////////////////////////////////////////////////////////////////
	CString strTitle = _T("");
	double Qm = 0, Qa = 0;
	if(bOutPutNormal)
	{
		m_pCalc->m_pFooting->GetCheckValueSupport(nJijum, &Qa, &Qm, TRUE);
	}

// 	nRow = 1;
	str.Format("Qmax(%s)", pDataStd->m_szTonf);
	Dom1.SetTextMatrix(1, 1, str, nAlign);
	str.Format("Qa(%s)", pDataStd->m_szTonf);
	Dom1.SetTextMatrix(1, 2, str, nAlign);
	Dom1.SetTextMatrix(1, 3, "안전율(%)", nAlign);

	nRow = 2;
	Dom1.SetTextMatrix(nRow, 0, _T("직접기초"), nAlign);

	if(bOutPutNormal) 
		str.Format("%8.3f",Qm);
	else 
		str = _T("-");
	Dom1.SetTextMatrix(nRow, 1, str, nAlign);
	//
	if(bOutPutNormal) 
		str.Format("%8.3f",Qa);
	else 
		str = _T("-");
	Dom1.SetTextMatrix(nRow, 2, str, nAlign);
	//
	if(bOutPutNormal) 
		str.Format("%7.2f",Qm/Qa*100);
	else 
		str = _T("-");
	Dom1.SetTextMatrix(nRow, 3, str, nAlign);


	Dom1.Draw(FALSE);
	pStd->m_pARoadOptionStd->SetEnvType(&Dom1,HCAD_TBL2);
	Dom1.DrawOutLine(); 
	Dom1.DrawOutLineShade();
	pStd->ChangeSeperateLayer(&Dom1);

	Dom << Dom1;
	Dom.RedrawByScale(GetScale()/dScale);

	Dom.SetCalcExtRect();
	rc = Dom.GetExtRect();
	Dom.Move(-rc.left, -rc.top);

	*pDomP << Dom;

	delete m_pCalc;
	return rc;
}

CDRect CDrawingBoring::AddForceTableDirect_BoxType_Lsd( CDomyun* pDomP )
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;
	CARcBridgeDrawingOptionStd* pOpt = pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData = pOpt->GetDrawingOption(m_nBridge);

	CARcBridgeDataStd* pDataStd = pStd->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = pDataStd->GetBridgeRebar(m_nBridge);
	CRcBridgeRebar* pBriRebar = pDataStd->GetBridgeRebar(m_nBridge);
	CARcBridgeDBStandard DBStd(pBri, pStd->m_pARoadOptionStd, pDataStd);
	DBStd.SetSelectFckDomText(pOpt->m_nSelectFckDomText);

	double dScale = 100;
	CDomyun Dom(pDomP);

	CDRect rc(0, 0, 0, 0), rc1(0, 0, 0, 0);

	long nJijum = 0;
	long nCntItem = 3;
	if(pBri->IsPileBase(nJijum)) return rc;

	m_pCalc = new CARcBridgeCalcStd(pBriRebar);
	m_pCalc->m_pARoadOptionStd = pStd->m_pARoadOptionStd;
	m_pCalc->CalculateAll();

	// Title
	long nAlign = GRID_ALIGN_CENTER | GRID_ALIGN_VCENTER; 
	double dX = 0;
	double dY = 0;

	CString str = _T("");
	CGridDomyunEx Dom1(pDomP);

	pStd->m_pARoadOptionStd->SetEnvType(&Dom1,HCAD_TBL1);
	double TitleWidth1[] = {
		Dom1.Always(45),
		Dom1.Always(25),
		Dom1.Always(25),
		Dom1.Always(25) };

	CDPoint xyBase = CDPoint(dX, dY);
	Dom1.SetBasePoint(xyBase);
	Dom1.SetColumnCount(4);
	Dom1.SetRowCount(2+nCntItem);		
	Dom1.SetRowHeightAll(Dom1.Always(6));

	for(long i=0; i<Dom1.GetColumnCount(); i++)
	{
		Dom1.SetColumnWidth(i,TitleWidth1[i]);	
	}

	Dom1.SetMargin(5);
	Dom1.SetMergeCol(0, 1, 3);
	Dom1.SetMergeRow(0, 1, 0);

	long nRow = 0;
	Dom1.SetTextMatrix(nRow, 0, "구분", nAlign);
	Dom1.SetTextMatrix(nRow, 1, "지지력에 대한 안정", nAlign);

	nRow = 1;
	str.Format("Qmax(%s)", pDataStd->m_szTonf_M2);
	Dom1.SetTextMatrix(nRow, 1, str, nAlign);
	str.Format("Qa(%s)", pDataStd->m_szTonf_M2);
	Dom1.SetTextMatrix(nRow, 2, str, nAlign);
	Dom1.SetTextMatrix(nRow, 3, "안전도", nAlign);

	BOOL bOutPutNormal = pBri->m_FemNormal.IsOutputLoaded();

	//////////////////////////////////////////////////////////////////////////
	CString strTitle = _T("");
	for(long nCombo=0; nCombo<3; ++nCombo)
	{
		ELSDLoadGroupKind eKind = static_cast<ELSDLoadGroupKind>(nCombo);
		double Qm = 0, Qa = 0;
		if(bOutPutNormal)
		{
			m_pCalc->m_pFooting->GetCheckValueSupport(nJijum, &Qa, &Qm, TRUE, eKind);
		}

		if( eKind == eLSD_COMBINATION_LIMIT ) strTitle = "극한한계상태";
		else if( eKind == eLSD_COMBINATION_EXTREME ) strTitle = "극단상황한계상태";
		else strTitle = "사용한계상태";

		++nRow;
		Dom1.SetTextMatrix(nRow, 0, strTitle, nAlign);

		if(bOutPutNormal) 
			str.Format("%8.3f",Qm);
		else 
			str = _T("-");
		Dom1.SetTextMatrix(nRow, 1, str, nAlign);
		//
		if(bOutPutNormal) 
			str.Format("%8.3f",Qa);
		else 
			str = _T("-");
		Dom1.SetTextMatrix(nRow, 2, str, nAlign);
		//
		if(bOutPutNormal) 
			str.Format("%7.2f",Qm/Qa*100);
		else 
			str = _T("-");
		Dom1.SetTextMatrix(nRow, 3, str, nAlign);
	}


	Dom1.Draw(FALSE);
	pStd->m_pARoadOptionStd->SetEnvType(&Dom1,HCAD_TBL2);
	Dom1.DrawOutLine(); 
	Dom1.DrawOutLineShade();
	pStd->ChangeSeperateLayer(&Dom1);

	Dom << Dom1;
	Dom.RedrawByScale(GetScale()/dScale);

	Dom.SetCalcExtRect();
	rc = Dom.GetExtRect();
	Dom.Move(-rc.left, -rc.top);

	*pDomP << Dom;

	delete m_pCalc;
	return rc;
}

CDRect CDrawingBoring::AddForceTableDirect(CDomyun* pDomP)
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;
	CARcBridgeDataStd* pDataStd = pStd->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = pDataStd->GetBridgeRebar(m_nBridge);
	CRcBridgeRebar* pBriRebar = pDataStd->GetBridgeRebar(m_nBridge);

	CDRect rc(0, 0, 0, 0);

	long nCntItem = 0;
	BOOL bIsDirecBase = FALSE;
	for(long ix=0; ix<pBri->GetCountJijum(); ++ix)
	{
		if(pBri->GetFootingByJijumNumber(ix) == NULL) continue;
		else if(pBri->IsPileBase(ix) != TRUE)
		{
			bIsDirecBase = TRUE;
			nCntItem++;
		}
	}

	if(bIsDirecBase == FALSE)
	{
		return rc;
	}

	m_pCalc = new CARcBridgeCalcStd(pBriRebar);
	m_pCalc->m_pARoadOptionStd = pStd->m_pARoadOptionStd;
	m_pCalc->CalculateAll();

	if(pDataStd->IsLsdDesign())
	{
		rc = AddForceTableDirect_Lsd(pDomP, nCntItem);
	}
	else
	{
		rc = AddForceTableDirect_Usd(pDomP, nCntItem);
	}

	
	delete m_pCalc;
	return rc;
}

CDRect CDrawingBoring::AddForceTableDirect_Usd(CDomyun* pDomP, long nCntItem)
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;
	CARcBridgeDrawingOptionStd* pOpt = pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData = pOpt->GetDrawingOption(m_nBridge);

	CARcBridgeDataStd* pDataStd = pStd->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = pDataStd->GetBridgeRebar(m_nBridge);
	CRcBridgeRebar* pBriRebar = pDataStd->GetBridgeRebar(m_nBridge);
	CARcBridgeDBStandard DBStd(pBri, pStd->m_pARoadOptionStd, pDataStd);
	DBStd.SetSelectFckDomText(pOpt->m_nSelectFckDomText);

	CDRect rc(0, 0, 0, 0), rc1(0, 0, 0, 0);

	// Title
	long nAlign = GRID_ALIGN_CENTER | GRID_ALIGN_VCENTER; 
	double dX = 0;
	double dY = 0;

	double dScale = 100;
	CDomyun Dom(pDomP);

	CString str = _T("");
	CGridDomyunEx Dom1(pDomP);

	pStd->m_pARoadOptionStd->SetEnvType(&Dom1,HCAD_TBL1);
	double TitleWidth1[] = {Dom1.Always(20),
		Dom1.Always(20),
		Dom1.Always(25),
		Dom1.Always(25),
		Dom1.Always(25),
		Dom1.Always(25),
		Dom1.Always(25),
		Dom1.Always(25),
		Dom1.Always(25),
		Dom1.Always(25),
		Dom1.Always(25)};
	CDPoint xyBase = CDPoint(dX, dY);
	Dom1.SetBasePoint(xyBase);
	Dom1.SetColumnCount(11);
	Dom1.SetRowCount(2+nCntItem*2);		
	Dom1.SetRowHeightAll(Dom1.Always(6));

	for(long ix=0;ix<Dom1.GetColumnCount(); ++ix)
	{
		Dom1.SetColumnWidth(ix,TitleWidth1[ix]);	
	}

	Dom1.SetMargin(5);
	Dom1.SetMergeCol(0, 0, 1);
	Dom1.SetMergeCol(1, 0, 1);
	Dom1.SetMergeCol(0, 2, 4);
	Dom1.SetMergeCol(0, 5, 7);
	Dom1.SetMergeCol(0, 8, 10);
	Dom1.SetMergeRow(0, 1, 0);
	Dom1.SetMergeRow(0, 1, 1);
	for(long ix=0; ix<nCntItem; ++ix)
	{
		Dom1.SetMergeRow(2+ix*2, 2+ix*2+1, 0);
	}

	Dom1.SetTextMatrix(0, 0, "구분", nAlign);

	Dom1.SetTextMatrix(0, 2, "전도에 대한 안정", nAlign);
	str.Format("Mr(%s)", pDataStd->m_szTonfM);
	Dom1.SetTextMatrix(1, 2, str, nAlign);
	str.Format("Mo(%s)", pDataStd->m_szTonfM);
	Dom1.SetTextMatrix(1, 3, str, nAlign);
	Dom1.SetTextMatrix(1, 4, "안전도", nAlign);

	Dom1.SetTextMatrix(0, 5, "활동에 대한 안정", nAlign);
	str.Format("Hu(%s)", pDataStd->m_szTonf);
	Dom1.SetTextMatrix(1, 5, str, nAlign);
	str.Format("H(%s)", pDataStd->m_szTonf);
	Dom1.SetTextMatrix(1, 6, str, nAlign);
	Dom1.SetTextMatrix(1, 7, "안전도", nAlign);

	Dom1.SetTextMatrix(0, 8, "지지력에 대한 안정", nAlign);
	str.Format("Qmax(%s)", pDataStd->m_szTonf_M2);
	Dom1.SetTextMatrix(1, 8, str, nAlign);
	str.Format("Qa(%s)", pDataStd->m_szTonf_M2);
	Dom1.SetTextMatrix(1, 9, str, nAlign);
	Dom1.SetTextMatrix(1, 10, "안전율(%)", nAlign);

	//////////////////////////////////////////////////////////////////////////
	BOOL bOutPutNormal	= (pBri->m_FemNormal.IsOutputLoaded() == TRUE) ? TRUE : FALSE;
	BOOL bOutPutEQ		= (pBri->GetFemManage(COMBO_EARTHQUAKE)->IsOutputLoaded() == TRUE) ? TRUE : FALSE;
	//////////////////////////////////////////////////////////////////////////
	long nRow = 0;
	long nCntJijum = pBri->GetCountJijum();
	for(long nJiumIndex=0; nJiumIndex<nCntJijum; nJiumIndex++)
	{
		CFootingApp *pFooting = pBri->GetFootingByJijumNumber(nJiumIndex);
		if(pFooting == NULL) continue;

		if(pFooting->m_exFooting.m_nType == EXFOOTING_TYPE_PILE)
			continue;
		//////////////////////////////////////////////////////////////////////////
		CString strTitle = _T("");
		double Mr[2] = {0,0}; // 전도에 대한 안정 , 평상시,지진시
		double Mo[2] = {0,0}; // 
		double Hu[2] = {0,0}; // 활동에 대한 안정
		double  H[2] = {0,0}; // 
		double Qm[2] = {0,0}; // 지지력에 대한 안정
		double Qa[2] = {0,0}; // 

		if(nJiumIndex == 0) strTitle = _T("시점측");
		else if(nJiumIndex == pBri->GetCountJijum()-1) strTitle = _T("종점측");
		else strTitle.Format("지점 %d", nJiumIndex+1);

		// 평상시
		m_pCalc->m_pFooting->GetCheckValueOverturning(nJiumIndex, &Mr[0], &Mo[0], TRUE);
		m_pCalc->m_pFooting->GetCheckValueSliding(nJiumIndex, &Hu[0], &H[0], TRUE);
		m_pCalc->m_pFooting->GetCheckValueSupport(nJiumIndex, &Qa[0], &Qm[0], TRUE);

		// 지진시
		m_pCalc->m_pFooting->GetCheckValueOverturning(nJiumIndex, &Mr[1], &Mo[1], FALSE);
		m_pCalc->m_pFooting->GetCheckValueSliding(nJiumIndex, &Hu[1], &H[1], FALSE);
		m_pCalc->m_pFooting->GetCheckValueSupport(nJiumIndex, &Qa[1], &Qm[1], FALSE);
		//////////////////////////////////////////////////////////////////////////
		nRow += 2;
		Dom1.SetTextMatrix(nRow, 0, strTitle, nAlign);
		Dom1.SetTextMatrix(nRow, 1, "상  시", nAlign);
		if(bOutPutNormal) str.Format("%8.3f",Mr[0]);
		else str = _T("-");
		Dom1.SetTextMatrix(nRow, 2, str, nAlign);
		if(bOutPutNormal) str.Format("%8.3f",Mo[0]);
		else str = _T("-");
		Dom1.SetTextMatrix(nRow, 3, str, nAlign);
		if(bOutPutNormal) str.Format("%7.2f",Mr[0]/Mo[0]);
		else str = _T("-");
		Dom1.SetTextMatrix(nRow, 4, str, nAlign);

		Dom1.SetTextMatrix(nRow+1, 1, "지진시", nAlign);
		if(bOutPutEQ) str.Format("%8.3f",Mr[1]);
		else str = _T("-");
		Dom1.SetTextMatrix(nRow+1, 2, str, nAlign);
		if(bOutPutEQ) str.Format("%8.3f",Mo[1]);
		else str = _T("-");
		Dom1.SetTextMatrix(nRow+1, 3, str, nAlign);
		if(bOutPutEQ) str.Format("%7.2f",Mr[1]/Mo[1]);
		else str = _T("-");
		Dom1.SetTextMatrix(nRow+1, 4, str, nAlign);

		if(bOutPutNormal) str.Format("%8.3f",Hu[0]);
		else str = _T("-");
		Dom1.SetTextMatrix(nRow, 5, str, nAlign);
		if(bOutPutNormal) str.Format("%8.3f",H[0]);
		else str = _T("-");
		Dom1.SetTextMatrix(nRow, 6, str, nAlign);
		if(bOutPutNormal) str.Format("%7.2f",Hu[0]/H[0]);
		else str = _T("-");
		Dom1.SetTextMatrix(nRow, 7, str, nAlign);

		if(bOutPutEQ) str.Format("%8.3f",Hu[1]);
		else str = _T("-");
		Dom1.SetTextMatrix(nRow+1, 5, str, nAlign);
		if(bOutPutEQ) str.Format("%8.3f",H[1]);
		else str = _T("-");
		Dom1.SetTextMatrix(nRow+1, 6, str, nAlign);
		if(bOutPutEQ) str.Format("%7.2f",Hu[1]/H[1]);
		else str = _T("-");
		Dom1.SetTextMatrix(nRow+1, 7, str, nAlign);

		if(bOutPutNormal) str.Format("%8.3f",Qm[0]);
		else str = _T("-");
		Dom1.SetTextMatrix(nRow, 8, str, nAlign);
		if(bOutPutNormal) str.Format("%8.3f",Qa[0]);
		else str = _T("-");
		Dom1.SetTextMatrix(nRow, 9, str, nAlign);
		if(bOutPutNormal) str.Format("%7.2f",Qm[0]/Qa[0]*100);
		else str = _T("-");
		Dom1.SetTextMatrix(nRow, 10, str, nAlign);

		if(bOutPutEQ) str.Format("%8.3f",Qm[1]);
		else str = _T("-");
		Dom1.SetTextMatrix(nRow+1, 8, str, nAlign);
		if(bOutPutEQ) str.Format("%8.3f",Qa[1]);
		else str = _T("-");
		Dom1.SetTextMatrix(nRow+1, 9, str, nAlign);
		if(bOutPutEQ) str.Format("%7.2f",Qm[1]/Qa[1]*100);
		else str = _T("-");
		Dom1.SetTextMatrix(nRow+1, 10, str, nAlign);
		//////////////////////////////////////////////////////////////////////////
	}
	Dom1.Draw(FALSE);
	pStd->m_pARoadOptionStd->SetEnvType(&Dom1,HCAD_TBL2);
	Dom1.DrawOutLine(); 
	Dom1.DrawOutLineShade();
	pStd->ChangeSeperateLayer(&Dom1);

	Dom << Dom1;
	Dom.RedrawByScale(GetScale()/dScale);

	Dom.SetCalcExtRect();
	rc = Dom.GetExtRect();
	Dom.Move(-rc.left, -rc.top);

	*pDomP << Dom;

	return rc;
}

CDRect CDrawingBoring::AddForceTableDirect_Lsd(CDomyun* pDomP, long nCntItem)
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;
	CARcBridgeDrawingOptionStd* pOpt = pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData = pOpt->GetDrawingOption(m_nBridge);

	CARcBridgeDataStd* pDataStd = pStd->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = pDataStd->GetBridgeRebar(m_nBridge);
	CRcBridgeRebar* pBriRebar = pDataStd->GetBridgeRebar(m_nBridge);
	CARcBridgeDBStandard DBStd(pBri, pStd->m_pARoadOptionStd, pDataStd);
	DBStd.SetSelectFckDomText(pOpt->m_nSelectFckDomText);

	double dScale = 100;
	CDomyun Dom(pDomP);
	CDRect rc(0, 0, 0, 0), rc1(0, 0, 0, 0);

	// Title
	long nAlign = GRID_ALIGN_CENTER | GRID_ALIGN_VCENTER; 
	double dX = 0;
	double dY = 0;

	CString str = _T("");
	CGridDomyunEx Dom1(pDomP);

	pStd->m_pARoadOptionStd->SetEnvType(&Dom1,HCAD_TBL1);
	double TitleWidth1[] = {Dom1.Always(20),
		Dom1.Always(20),
		Dom1.Always(25),
		Dom1.Always(25),
		Dom1.Always(25),
		Dom1.Always(25),
		Dom1.Always(25),
		Dom1.Always(25),
		Dom1.Always(25),
		Dom1.Always(25),
		Dom1.Always(25),
		Dom1.Always(25),
		Dom1.Always(25),
		Dom1.Always(25)};
	CDPoint xyBase = CDPoint(dX, dY);
	Dom1.SetBasePoint(xyBase);
	Dom1.SetColumnCount(14);
	Dom1.SetRowCount(2+nCntItem*3);		
	Dom1.SetRowHeightAll(Dom1.Always(6));

	for(long ix=0;ix<Dom1.GetColumnCount(); ++ix)
	{
		Dom1.SetColumnWidth(ix,TitleWidth1[ix]);	
	}

	Dom1.SetMargin(5);
	Dom1.SetMergeCol(0, 0, 1);
	Dom1.SetMergeCol(1, 0, 1);
	Dom1.SetMergeCol(0, 2, 4);
	Dom1.SetMergeCol(0, 5, 7);
	Dom1.SetMergeCol(0, 8, 10);
	Dom1.SetMergeCol(0, 11, 13);
	Dom1.SetMergeRow(0, 1, 0);
	Dom1.SetMergeRow(0, 1, 1);
	for(long ix=0; ix<nCntItem; ++ix)
	{
		Dom1.SetMergeRow(2+ix*3, 2+ix*3+2, 0);
	}

	Dom1.SetTextMatrix(0, 0, "구분", nAlign);

	Dom1.SetTextMatrix(0, 2, "전도에 대한 안정", nAlign);
	str.Format("e(%s)", pDataStd->m_szM);
	Dom1.SetTextMatrix(1, 2, str, nAlign);
	str.Format("허용폭(%s)", pDataStd->m_szM);
	Dom1.SetTextMatrix(1, 3, str, nAlign);
	Dom1.SetTextMatrix(1, 4, "안전도", nAlign);

	Dom1.SetTextMatrix(0, 5, "활동에 대한 안정", nAlign);
	str.Format("QR(%s)", pDataStd->m_szTonf);
	Dom1.SetTextMatrix(1, 5, str, nAlign);
	str.Format("H(%s)", pDataStd->m_szTonf);
	Dom1.SetTextMatrix(1, 6, str, nAlign);
	Dom1.SetTextMatrix(1, 7, "안전도", nAlign);

	Dom1.SetTextMatrix(0, 8, "지지력에 대한 안정", nAlign);
	str.Format("Qmax(%s)", pDataStd->m_szTonf_M2);
	Dom1.SetTextMatrix(1, 8, str, nAlign);
	str.Format("Qa(%s)", pDataStd->m_szTonf_M2);
	Dom1.SetTextMatrix(1, 9, str, nAlign);
	Dom1.SetTextMatrix(1, 10, "안전도", nAlign);

	Dom1.SetTextMatrix(0, 11, "침하 대한 안정", nAlign);
	str.Format("Se(%s)", pDataStd->m_szM);
	Dom1.SetTextMatrix(1, 11, str, nAlign);
	str.Format("Sa(%s)", pDataStd->m_szM);
	Dom1.SetTextMatrix(1, 12, str, nAlign);
	Dom1.SetTextMatrix(1, 13, "안전도", nAlign);

	//////////////////////////////////////////////////////////////////////////
	BOOL bOutPut	= (pBri->m_FemNormal.IsOutputLoaded() == TRUE) ? TRUE : FALSE;
	//////////////////////////////////////////////////////////////////////////
	long nRow = 2;
	long nCntJijum = pBri->GetCountJijum();
	for(long nJiumIndex=0; nJiumIndex<nCntJijum; nJiumIndex++)
	{
		CFootingApp *pFooting = pBri->GetFootingByJijumNumber(nJiumIndex);
		if(pFooting == NULL) continue;

		if(pFooting->m_exFooting.m_nType == EXFOOTING_TYPE_PILE)
			continue;

		CLsdFootingDesign* pLsdFooting = m_pCalc->m_pFooting->GetLsdFootingDesign(nJiumIndex);
		if(pLsdFooting == NULL) continue;


		//////////////////////////////////////////////////////////////////////////
		CString strTitle = _T("");
		double dE[3] = {0,0,0};	 // 전도에 대한 안정 , 극한, 극단, 사용
		double dEa[3] = {0,0,0}; // 
		double Hu[3] = {0,0,0}; // 활동에 대한 안정
		double  H[3] = {0,0,0}; // 
		double Qm[3] = {0,0,0}; // 지지력에 대한 안정
		double Qa[3] = {0,0,0}; // 
		double Se[3] = {0,0,0};	// 침하에 대한 안정
		double Sa[3] = {0,0,0};

		if(nJiumIndex == 0) strTitle = _T("시점측");
		else if(nJiumIndex == pBri->GetCountJijum()-1) strTitle = _T("종점측");
		else strTitle.Format("지점 %d", nJiumIndex+1);

		// 극한한계
		Qm[0] = pLsdFooting->GetQmax(eLSD_COMBINATION_LIMIT);
		Qm[0] = pLsdFooting->GetQmax(eLSD_COMBINATION_LIMIT);
		Qm[0] = pLsdFooting->GetQmax(eLSD_COMBINATION_LIMIT);
// 		m_pCalc->m_pFooting->GetCheckValueOverturning(nJiumIndex, &Mr[0], &Mo[0], TRUE);
// 		m_pCalc->m_pFooting->GetCheckValueSliding(nJiumIndex, &Hu[0], &H[0], TRUE);
// 		m_pCalc->m_pFooting->GetCheckValueSupport(nJiumIndex, &Qa[0], &Qm[0], TRUE);

		// 극단상황 한계
// 		m_pCalc->m_pFooting->GetCheckValueOverturning(nJiumIndex, &Mr[1], &Mo[1], FALSE);
// 		m_pCalc->m_pFooting->GetCheckValueSliding(nJiumIndex, &Hu[1], &H[1], FALSE);
// 		m_pCalc->m_pFooting->GetCheckValueSupport(nJiumIndex, &Qa[1], &Qm[1], FALSE);

		// 사용한계
		//////////////////////////////////////////////////////////////////////////
		CString strTitleEnv[3] = { "극한한계", "극단상황", "사용한계" };
		ELSDLoadGroupKind eLoadGroup[3] = { eLSD_COMBINATION_LIMIT, eLSD_COMBINATION_EXTREME, eLSD_COMBINATION_SERVICE };

		for(long nEnv=0; nEnv<3; ++nEnv)
		{
			HGBOOL bCheck = pLsdFooting->GetOverTurningCheckByGroupKind(eLoadGroup[nEnv], dE[nEnv], dEa[nEnv]);

			Dom1.SetTextMatrix(nRow, 0, strTitle, nAlign);
			Dom1.SetTextMatrix(nRow, 1, strTitleEnv[nEnv], nAlign);

			if(bCheck && bOutPut) str = GetStrDouble(dE[nEnv], 0, TRUE);
			else str = _T("-");
			Dom1.SetTextMatrix(nRow, 2, str, nAlign);
			if(bCheck && bOutPut) str = GetStrDouble(dEa[nEnv], 0, TRUE);
			else str = _T("-");
			Dom1.SetTextMatrix(nRow, 3, str, nAlign);
			if(bCheck && bOutPut) str.Format("%7.2f",dEa[nEnv]/dE[nEnv]);
			else str = _T("-");
			Dom1.SetTextMatrix(nRow, 4, str, nAlign);

			bCheck = pLsdFooting->GetOverSlidingByGroupKind(eLoadGroup[nEnv], H[nEnv], Hu[nEnv]);

			if(bCheck && bOutPut) str = GetStrDouble(Hu[nEnv], 0, TRUE);
			else str = _T("-");
			Dom1.SetTextMatrix(nRow, 5, str, nAlign);
			if(bCheck && bOutPut) str = GetStrDouble(H[nEnv], 0, TRUE);
			else str = _T("-");
			Dom1.SetTextMatrix(nRow, 6, str, nAlign);
			if(bCheck && bOutPut) str.Format("%7.2f",Hu[nEnv]/H[nEnv]);
			else str = _T("-");
			Dom1.SetTextMatrix(nRow, 7, str, nAlign);

			bCheck = pLsdFooting->GetSupportCheckByGroupKind(eLoadGroup[nEnv], Qm[nEnv], Qa[nEnv]);

			if(bCheck && bOutPut) str = GetStrDouble(Qm[nEnv], 3, TRUE);
			else str = _T("-");
			Dom1.SetTextMatrix(nRow, 8, str, nAlign);
			if(bCheck && bOutPut) str = GetStrDouble(Qa[nEnv], 3, TRUE);
			else str = _T("-");
			Dom1.SetTextMatrix(nRow, 9, str, nAlign);
			if(bCheck && bOutPut) str.Format("%7.2f",Qa[nEnv]/Qm[nEnv]);
			else str = _T("-");
			Dom1.SetTextMatrix(nRow, 10, str, nAlign);

			bCheck = pLsdFooting->GetSubsideCheckByGroupKind(eLoadGroup[nEnv], Se[nEnv], Sa[nEnv]);

			if(bCheck && bOutPut) str.Format("%8.3f",Se[nEnv]);
			else str = _T("-");
			Dom1.SetTextMatrix(nRow, 11, str, nAlign);
			if(bCheck && bOutPut) str.Format("%8.3f",Sa[nEnv]);
			else str = _T("-");
			Dom1.SetTextMatrix(nRow, 12, str, nAlign);
			if(bCheck && bOutPut) str.Format("%7.2f",Sa[nEnv]/Se[nEnv]);
			else str = _T("-");
			Dom1.SetTextMatrix(nRow, 13, str, nAlign);

			nRow++;
		}
		//////////////////////////////////////////////////////////////////////////
	}
	Dom1.Draw(FALSE);
	pStd->m_pARoadOptionStd->SetEnvType(&Dom1,HCAD_TBL2);
	Dom1.DrawOutLine(); 
	Dom1.DrawOutLineShade();
	pStd->ChangeSeperateLayer(&Dom1);

	Dom << Dom1;
	Dom.RedrawByScale(GetScale()/dScale);

	Dom.SetCalcExtRect();
	rc = Dom.GetExtRect();
	Dom.Move(-rc.left, -rc.top);

	*pDomP << Dom;
	return rc;
	// CARcBridgeCalcSummary::MakeTableCheckFootAndPile_Lsd() 를 참조해서 새로 찍어야 한다.
}

CDRect CDrawingBoring::AddForceTablePile(CDomyun* pDomP)
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;
	CARcBridgeDrawingOptionStd* pOpt = pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pData = pOpt->GetDrawingOption(m_nBridge);
	
	CARcBridgeDataStd* pDataStd = pStd->m_pARcBridgeDataStd;
	CRcBridgeRebar* pBri = pDataStd->GetBridgeRebar(m_nBridge);
	CRcBridgeRebar* pBriRebar = pDataStd->GetBridgeRebar(m_nBridge);
	CARcBridgeDBStandard DBStd(pBri, pStd->m_pARoadOptionStd, pDataStd);

	CDRect rc(0, 0, 0, 0), rc1(0, 0, 0, 0);

	long nCntJijum = pBri->IsBoxType() ? 1 : pBri->GetCountJijum();

	BOOL bLsd = pDataStd->IsLsdDesign();
	BOOL bPileASD = FALSE;
	long nCountCaseAll(0);
	for(long nJiumIndex=0; nJiumIndex<nCntJijum; nJiumIndex++)
	{
		CFootingApp *pFooting = pBri->GetFootingByJijumNumber(nJiumIndex);
		if(pFooting == NULL) continue;

		if(pBri->IsPileBase(nJiumIndex) == FALSE) continue;

		CExPileApp *pPile = &pFooting->m_exFooting.m_Pile;

		BOOL bCurPileASD = FALSE;
		if(pDataStd->IsApplyPhcPileASD(pPile) || pDataStd->IsApplyBoredPileASD(pPile))
		{
			bPileASD = TRUE;
			bCurPileASD = TRUE;
		}

		nCountCaseAll += (bLsd && !bCurPileASD) ? 3 : 2;
	}
	BOOL bTypeLsd = bLsd && !bPileASD;
	long nCntItem = 0;
	// long nCntCase = bTypeLsd ? 3 : 2;			//bLsd? pBri->GetCountApplyEnvType(eEnvService_Crack) : 2;
	BOOL bIsPileBase = FALSE;
	long i=0; for(i=0; i<nCntJijum; i++)
	{
		if(pBri->GetFootingByJijumNumber(i) == NULL) continue;
		else if(pBri->IsPileBase(i) == TRUE)
		{
			bIsPileBase = TRUE;
			nCntItem++;
		}
	}

	if(bIsPileBase == FALSE)
	{
		return rc;
	}

	DBStd.SetSelectFckDomText(pOpt->m_nSelectFckDomText);
	
	m_pCalc = new CARcBridgeCalcStd(pBriRebar);
	m_pCalc->m_pARoadOptionStd = pStd->m_pARoadOptionStd;
	m_pCalc->CalculateAll();
	
	double dScale = 100;
	CDomyun Dom(pDomP);

		// Title
	long nAlign = GRID_ALIGN_CENTER | GRID_ALIGN_VCENTER; 
	double dX = 0;
	double dY = 0;
		
	CString str = _T("");
	CGridDomyunEx Dom1(pDomP);
		
	pStd->m_pARoadOptionStd->SetEnvType(&Dom1,HCAD_TBL1);
	double TitleWidth1[] = {
		Dom1.Always(20),
		Dom1.Always(20),
		Dom1.Always(20),
		Dom1.Always(20),
		Dom1.Always(17),
		Dom1.Always(20),
		Dom1.Always(20),
		Dom1.Always(17),
		Dom1.Always(20),
		Dom1.Always(20),
		Dom1.Always(17),
		Dom1.Always(20),
		Dom1.Always(20),
		Dom1.Always(17)
	};
	CDPoint xyBase = CDPoint(dX, dY);
	Dom1.SetBasePoint(xyBase);
	Dom1.SetColumnCount(bTypeLsd ? 8 : 14);
	Dom1.SetRowCount(2 + nCountCaseAll);	
	Dom1.SetRowHeightAll(Dom1.Always(6));

	for(i=0;i<Dom1.GetColumnCount();i++)
	{
		Dom1.SetColumnWidth(i,TitleWidth1[i]);	
	}
		
	Dom1.SetMargin(5);
	Dom1.SetMergeCol(0, 0, 1);
	Dom1.SetMergeCol(1, 0, 1);
	Dom1.SetMergeCol(0, 2, 4);
	Dom1.SetMergeCol(0, 5, 7);
	if(!bTypeLsd)
	{
		Dom1.SetMergeCol(0, 8, 10);
		Dom1.SetMergeCol(0, 11, 13);
	}
	Dom1.SetMergeRow(0, 1, 0);
	Dom1.SetMergeRow(0, 1, 1);
// 	for(i=0; i<nCntItem; i++)
// 	{
// 		Dom1.SetMergeRow(2+i*nCntCase, 2+i*nCntCase+(nCntCase-1), 0);
// 	}
		
	long nCol = 0;
	Dom1.SetTextMatrix(0, nCol++, "구분", nAlign);
	++nCol;
		
	str.Format("말뚝의 지지력(%s/본)", pDataStd->m_szTonf);
	Dom1.SetTextMatrix(0, nCol, str, nAlign);
	Dom1.SetTextMatrix(1, nCol++, "말뚝반력", nAlign);
	Dom1.SetTextMatrix(1, nCol++, "허용지지력", nAlign);
	Dom1.SetTextMatrix(1, nCol++, "적용률(%)", nAlign);
		
	if(!bTypeLsd)
	{
		str.Format("말뚝의 휨응력(%s)", pDataStd->m_szKgf_CM2);
		Dom1.SetTextMatrix(0, nCol, str, nAlign);
		Dom1.SetTextMatrix(1, nCol++, "작용응력", nAlign);
		Dom1.SetTextMatrix(1, nCol++, "허용응력", nAlign);
		Dom1.SetTextMatrix(1, nCol++, "적용률(%)", nAlign);

		str.Format("말뚝의 전단응력(%s)", pDataStd->m_szKgf_CM2);
		Dom1.SetTextMatrix(0, nCol, str, nAlign);
		Dom1.SetTextMatrix(1, nCol++, "작용응력", nAlign);
		Dom1.SetTextMatrix(1, nCol++, "허용응력", nAlign);
		Dom1.SetTextMatrix(1, nCol++, "적용률(%)", nAlign);
	}

	Dom1.SetTextMatrix(0, nCol, "말뚝의 수평변위(mm)", nAlign);
	Dom1.SetTextMatrix(1, nCol++, "발생변위", nAlign);
	Dom1.SetTextMatrix(1, nCol++, "허용변위", nAlign);
	Dom1.SetTextMatrix(1, nCol++, "적용률(%)", nAlign);

	//////////////////////////////////////////////////////////////////////////
	BOOL bOutPutNormal	= (pBri->m_FemNormal.IsOutputLoaded() == TRUE) ? TRUE : FALSE;
	BOOL bOutPutEQ		= (pBri->GetFemManage(COMBO_EARTHQUAKE)->IsOutputLoaded() == TRUE) ? TRUE : FALSE;
	//////////////////////////////////////////////////////////////////////////
	long nRow = 2;
	for(long nJiumIndex=0; nJiumIndex<nCntJijum; nJiumIndex++)
	{
		CFootingApp *pFooting = pBri->GetFootingByJijumNumber(nJiumIndex);
		if(pFooting == NULL) continue;
			
		if(pBri->IsPileBase(nJiumIndex) == FALSE) continue;

		BOOL bPileASDCur = bPileASD;
		if(bPileASD)
		{
			CExPileApp *pPile = &pFooting->m_exFooting.m_Pile;
			// 지점마다 말뚝 종류가 다르게 설정될수 있다.
			if(pDataStd->IsApplyPhcPileASD(pPile) == FALSE && pDataStd->IsApplyBoredPileASD(pPile) == FALSE)
			{
				bPileASDCur = FALSE;
			}
		}

		BOOL bTypeLsdCur = (bLsd && !bPileASDCur) ? TRUE : FALSE;
		long nCntCase = (bLsd && !bPileASDCur) ? 3 : 2;			//bLsd? pBri->GetCountApplyEnvType(eEnvService_Crack) : 2;

		Dom1.SetMergeRow(nRow, nRow+(nCntCase-1), 0);
		
		pFooting->m_exFooting.m_Pile.IsPHC();
		pFooting->m_exFooting.m_Pile.IsBoredPile();

		//////////////////////////////////////////////////////////////////////////
		CString strTitle = _T("");
		double Ra[eEnvService_Crack] = { 0, 0, 0, 0 };
		double Pmax[eEnvService_Crack] = { 0, 0, 0, 0 };
		double fsa[eEnvService_Crack] = { 0, 0, 0, 0 };
		double fmax[eEnvService_Crack] = { 0, 0, 0, 0 };
		double Tsa[eEnvService_Crack] = { 0, 0, 0, 0 };
		double Tmax[eEnvService_Crack] = { 0, 0, 0, 0 };
		double Dispa[eEnvService_Crack] = { 0, 0, 0, 0 };
		double Dispmax[eEnvService_Crack] = { 0, 0, 0, 0 };

		if(nJiumIndex == 0) 
			strTitle = pBri->IsBoxType() ? _T("말뚝기초") : _T("시점측");
		else 
			if(nJiumIndex == pBri->GetCountJijum()-1) strTitle = _T("종점측");
		else 
			strTitle.Format("지점 %d", nJiumIndex+1);
		//////////////////////////////////////////////////////////////////////////
		for(long ix=0; ix<nCntCase; ++ix)
		{
			CString szCase(_T(""));
			BOOL bOutputCase(TRUE);
			BOOL bNormal = (ix == 0)? TRUE:FALSE;

			long nCasePile = ix;
			if(bLsd && !bTypeLsdCur)
				nCasePile = bNormal ? PILE_CASE_USE : PILE_CASE_EXTREME;

			if(bLsd)
			{
				// szCase = pBri->GetStringComboEnvType_Lsd(eEnvType, FALSE);
				CString strTitleEnv[3] = { "사용한계", "극한한계", "극단상황" };
				szCase = strTitleEnv[nCasePile];
			}
			else
			{
				szCase = bNormal? _T("상  시") : _T("지진시");
				bOutputCase = bNormal? bOutPutNormal : bOutPutEQ;
			}

			if(!bPileASDCur)
			{
				long nCaseNor(0);
				CDesignPile *pCalcPile = m_pCalc->GetDesignPile(nJiumIndex);

				Pmax[ix] = GetValueUnitChange(pCalcPile->GetMaxReaction(!bNormal, nCaseNor, nCasePile), UNIT_CGS_TONF, pDataStd->m_szTonf);
				Ra[ix]	= bLsd ? pCalcPile->m_Ra[nCasePile] : GetValueUnitChange(pCalcPile->m_Ra[nCasePile], UNIT_CGS_TONF, pDataStd->m_szTonf);

				fmax[ix] = pCalcPile->GetMaxStress(!bNormal, nCaseNor, nCasePile);	
				if(pFooting->m_exFooting.m_Pile.IsPHC())
				{
					fsa[ix] = GetValueUnitChange(pCalcPile->m_PHC_Fca[nCasePile], UNIT_CGS_KGF_CM2, pDataStd->m_szKgf_CM2);
				}
				else if(pCalcPile->m_dArrFsa[nCasePile].GetSize() > nCaseNor)
				{
					fsa[ix] = GetValueUnitChange(pCalcPile->m_dArrFsa[nCasePile][nCaseNor], UNIT_CGS_KGF_CM2, pDataStd->m_szKgf_CM2);
				}

				if(pFooting->m_exFooting.m_Pile.IsPHC())
				{
					Tsa[ix] = GetValueUnitChange(pCalcPile->m_PHC_Ta[nCasePile], UNIT_CGS_KGF_CM2, pDataStd->m_szKgf_CM2);
				}
				else if(pCalcPile->m_dArrTsa[nCasePile].GetSize() > 0)
				{
					Tsa[ix] = GetValueUnitChange(pCalcPile->m_dArrTsa[nCasePile][nCaseNor], UNIT_CGS_KGF_CM2, pDataStd->m_szKgf_CM2);
				}

				nCaseNor = -1;
				Tmax[ix] = GetValueUnitChange(pCalcPile->GetMaxShearForce(!bNormal, nCaseNor, nCasePile), UNIT_CGS_KGF_CM2, pDataStd->m_szKgf_CM2);

				Dispa[ix] = frM(pCalcPile->m_Disa[nCasePile]);
				Dispmax[ix] = frM(pCalcPile->GetMaxDisplacement(!bNormal, nCaseNor, nCasePile));
			}
			else
			{
				m_pCalc->m_pFooting->GetCheckValuePileForce(nJiumIndex, &Ra[ix], &Pmax[ix], bNormal);
				m_pCalc->m_pFooting->GetCheckValuePileBending(nJiumIndex, &fsa[ix], &fmax[ix], bNormal);
				m_pCalc->m_pFooting->GetCheckValuePileShear(nJiumIndex, &Tsa[ix], &Tmax[ix], bNormal);
				m_pCalc->m_pFooting->GetCheckValuePileDisplacement(nJiumIndex, &Dispa[ix], &Dispmax[ix], bNormal);
			}

			nCol = 0;

			if(ix == 0)
			{
				Dom1.SetTextMatrix(nRow, nCol++, strTitle, nAlign);
			}
			else
			{
				nCol++;
			}
			Dom1.SetTextMatrix(nRow, nCol++, szCase, nAlign);
			if(bOutputCase)
			{
				if(bLsd)
					str = pDataStd->GetStrDouble(Pmax[ix], UNIT_CGS_TONF);
				else
					str.Format("%8.3f", Pmax[ix]);
			}
			else str = _T("-");
			Dom1.SetTextMatrix(nRow, nCol++, str, nAlign);
			if(bOutputCase)
			{
				if(bLsd)
					str = pDataStd->GetStrDouble(Ra[ix], UNIT_CGS_TONF);
				else
					str.Format("%8.3f",Ra[ix]);
			}
			else str = _T("-");
			Dom1.SetTextMatrix(nRow, nCol++, str, nAlign);
			if(bOutputCase) str.Format("%7.2f", Pmax[ix]/Ra[ix]*100);
			else str = _T("-");
			Dom1.SetTextMatrix(nRow, nCol++, str, nAlign);

			if(!bTypeLsd)
			{
				// 말뚝의 휨응력
				if(bOutputCase && (bLsd==FALSE || bPileASDCur))
				{
					if(bLsd)
						str = GetStrDouble(fmax[ix], 3, bLsd);	// pDataStd->GetStrDouble(fmax[ix], UNIT_CGS_KGF_CM2, FALSE, 3);
					else
						str.Format("%8.3f", fmax[ix]);
				}
				else str = _T("-");
				Dom1.SetTextMatrix(nRow, nCol++, str, nAlign);
				if(bOutputCase && (bLsd==FALSE || bPileASDCur))
				{
					if(bLsd)
						str = GetStrDouble(fsa[ix], 3, bLsd);	// pDataStd->GetStrDouble(fsa[ix], UNIT_CGS_KGF_CM2, FALSE, 3);
					else
						str.Format("%8.3f", fsa[ix]);
				}
				else str = _T("-");
				Dom1.SetTextMatrix(nRow, nCol++, str, nAlign);
				if(bOutputCase && (bLsd==FALSE || bPileASDCur)) str.Format("%7.2f", fmax[ix]/fsa[ix]*100);
				else str = _T("-");
				Dom1.SetTextMatrix(nRow, nCol++, str, nAlign);

				// 말뚝의 전단응력
				if(bOutputCase && (bLsd==FALSE || bPileASDCur))
				{
					if(bLsd)
						str = GetStrDouble(Tmax[ix], 3, bLsd);	// pDataStd->GetStrDouble(Tmax[ix], UNIT_CGS_KGF_CM2, FALSE, 3);
					else
						str.Format("%8.3f", Tmax[ix]);
				}
				else str = _T("-");
				Dom1.SetTextMatrix(nRow, nCol++, str, nAlign);
				
				if(bOutputCase && (bLsd==FALSE || bPileASDCur))
				{
					if(bLsd)
						str = GetStrDouble(Tsa[ix], 3, bLsd);		// pDataStd->GetStrDouble(Tsa[ix], UNIT_CGS_KGF_CM2, FALSE, 3);
					else
						str.Format("%8.3f", Tsa[ix]);
				}
				else str = _T("-");
				
				Dom1.SetTextMatrix(nRow, nCol++, str, nAlign);
				if(bOutputCase && (bLsd==FALSE || bPileASDCur)) str.Format("%7.2f", Tmax[ix]/Tsa[ix]*100);
				else str = _T("-");
				Dom1.SetTextMatrix(nRow, nCol++, str, nAlign);
			}

			// 말뚝의 수평변위
			if(bOutputCase && !(bTypeLsd && nCasePile != PILE_CASE_USE)) str.Format("%8.3f", Dispmax[ix]);
			else str = _T("-");
			Dom1.SetTextMatrix(nRow, nCol++, str, nAlign);
			if(bOutputCase && !(bTypeLsd && nCasePile != PILE_CASE_USE)) str.Format("%8.3f", Dispa[ix]);
			else str = _T("-");
			Dom1.SetTextMatrix(nRow, nCol++, str, nAlign);
			if(bOutputCase && !(bTypeLsd && nCasePile != PILE_CASE_USE)) str.Format("%7.2f", Dispmax[ix]/Dispa[ix]*100);
			else str = _T("-");
			Dom1.SetTextMatrix(nRow, nCol++, str, nAlign);

			++nRow;
		}
	}

	Dom1.Draw(FALSE);
	pStd->m_pARoadOptionStd->SetEnvType(&Dom1,HCAD_TBL2);
	Dom1.DrawOutLine(); 
	Dom1.DrawOutLineShade();
	pStd->ChangeSeperateLayer(&Dom1);
		
	Dom << Dom1;
	Dom.RedrawByScale(GetScale()/dScale);
		
	Dom.SetCalcExtRect();
	rc = Dom.GetExtRect();
	Dom.Move(-rc.left, -rc.top);
	
	*pDomP << Dom;
	
	delete m_pCalc;
	return rc;
}

CDRect CDrawingBoring::AddNote(CDomyun* pDomP)
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;

	CStringArray szArrNo;
	CStringArray szArrNot;

	szArrNo.RemoveAll();
	szArrNot.RemoveAll();
	
	szArrNo.Add("1.");
	szArrNot.Add("본 교량의 기초의 심도는 토질조사 결과에 따른 암추정선을 기준으로 하여 작성된것으로");
	szArrNo.Add("");
	szArrNot.Add("기초의 시공계획 수립시 지지층의 상태 및 심도를 조사한 후 설계도와 상이할 경우");
	szArrNo.Add("");
	szArrNot.Add("감독관(감리원)의 승인을 거쳐 변경하여야 한다.");
	szArrNo.Add("2.");
	szArrNot.Add("기초콘크리트 타설시에는 자하수의 영향이 없는 건조한 상태로 타설하여야 하며");
	szArrNo.Add("");
	szArrNot.Add("지하매설물에 손상을 주지 않도록 충분한 사전조사를 시행하여야 한다.");
	szArrNo.Add("3.");
	szArrNot.Add("기초 굴착후 굴착지반에 대한 Face Mapping을 시행토록 하고, 기초계획 수립시");
	szArrNo.Add("");
	szArrNot.Add("기초 지지층의 상태를 사전에 면밀히 조사하여 기초를 확실한 지지층에");
	szArrNo.Add("");
	szArrNot.Add("시공하여야 한다.");
	szArrNo.Add("4.");
	szArrNot.Add("공사전에 도면과 숙지사항을 충분히 이해하고 관련 시방서를 숙지하여 사전공사");
	szArrNo.Add("");
	szArrNot.Add("준비등에 만전을 기하여 공사중 오류 발생이 없도록 하여야 한다.");
	
	
	long nSize = szArrNot.GetSize();
	CGridDomyunEx Dom(pDomP);

	pStd->m_pARoadOptionStd->SetEnvType(&Dom,HCAD_NOTE);
	double dWidthTxt = Dom.GetTextWidth(szArrNot.GetAt(0), 0);
	double dWidthNo = Dom.GetTextWidth(szArrNo.GetAt(0),0);

	double dSpaceWidFirst = Dom.Always(20);
	double dSpaceWidLast = dWidthTxt + dWidthNo + Dom.Always(20);

	Dom.SetBasePoint(CDPoint(0,0));
	Dom.SetGridLines(GVL_NONE);
	Dom.SetColumnCount(2);
	Dom.SetRowCount(nSize+2);
	Dom.SetColumnWidth(0,dSpaceWidFirst);
	Dom.SetColumnWidth(1, dSpaceWidLast);
	Dom.SetRowHeightAll(Dom.GetTextHeight()*2.5);
	Dom.SetRowHeight(0,Dom.Always(5));
	Dom.SetRowHeight(nSize+1,Dom.Always(5));
	
	CString szNo = _T("");
	CString str = _T("");
	long i=0; for(i=1; i<nSize+1; i++)
	{
		szNo = szArrNo.GetAt(i-1);
		str = szArrNot.GetAt(i-1);

		Dom.SetTextMatrix(i,0,szNo,GRID_ALIGN_RIGHT);
		Dom.SetTextMatrix(i,1,str,GRID_ALIGN_LEFT);
	}
	
	Dom.Draw(FALSE);
	Dom.DrawOutLine();
	Dom.DrawOutLineShade();
	pStd->ChangeSeperateLayer(&Dom);
	
	Dom.SetCalcExtRect();
	CDRect rc = Dom.GetExtRect();
	Dom.Move(-rc.left, -rc.top);
	*pDomP << Dom;
	
	return rc;
}
