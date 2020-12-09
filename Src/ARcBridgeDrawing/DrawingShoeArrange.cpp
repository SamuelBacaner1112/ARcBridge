// DrawingShoeArrange.cpp: implementation of the CDrawingShoeArrange class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../../Src/ARcBridgeData/ARcBridgeData.h"
#include "../ARcBridgeDrawingOption/ARcBridgeDrawingOption.h"
#include "ARcBridgeDrawing.h"
#include "DrawingShoeArrange.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDrawingShoeArrange::CDrawingShoeArrange()
{
	m_nBridge = -1;
}

CDrawingShoeArrange::~CDrawingShoeArrange()
{

}

long CDrawingShoeArrange::GetPageSu()
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;

	if(pStd->GetCountTotalPage()<=0)
		pStd->MakePageInfo();

	m_nPage	= pStd->GetCountCategoryPage(_T("교량 받침 배치도"));

	return m_nPage;
}

CString CDrawingShoeArrange::GetTitle()
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;
	CPageInfo* pInfo = pStd->GetPageInfoOrg(_T("교량 받침 배치도"), pStd->m_nCurPage-m_nSttPage);
	if(pInfo==NULL) return _T("");

	CString strTitle = pInfo->m_strTitle;

	return strTitle;
}

double CDrawingShoeArrange::GetScale()
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;
	CARcBridgeDrawingOptionStd* pOpt = pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDrawingOptionData* pOptData = pOpt->GetDrawingOption(m_nBridge);

	m_Scale	= pOptData->m_dScaleVPBase;
	return pOptData->m_dScaleVPBase;
}

void CDrawingShoeArrange::SetNumber(long nSubPage)
{
	m_nBridge = GetNumberBridge(nSubPage-m_nSttPage);
}

long CDrawingShoeArrange::GetNumberBridge(long nCategoryPageNum)
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;
	CPageInfo* pInfo = pStd->GetPageInfoOrg(_T("교량 받침 배치도"), nCategoryPageNum);
	if(pInfo==NULL) return -1;

	return pInfo->m_nBri;
}

void CDrawingShoeArrange::DrawOnePage()
{
	TRACE("CDrawingShoeArrange");
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;
	CARcBridgeDrawingOptionStd* pOpt = pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDataStd* pDataStd = pStd->m_pARcBridgeDataStd;
	CDrawDogak* pDgk = pStd->GetDogak();
	CDrawPageMng* pMng = pStd->GetPageManage();

	long nPage = pStd->m_nCurPage;
	long nSubPage = nPage;

	SetNumber(nSubPage);
	double dScale = GetScale();

	CDomyun* pDom = pStd->GetDomyun(nPage);
	pDom->SetScaleDim(dScale);

	CPageInfo* pInfo = pStd->GetPageInfoOrg(_T("교량 받침 배치도"), pStd->m_nCurPage-m_nSttPage);
	if(pInfo==NULL) return;

	nPage = pStd->GetPageNum(pInfo);

	pStd->m_pOptStd->SetEnvType(pDom, HCAD_MIS3);
	pStd->m_pOptStd->SetEnvType(pDom, HCAD_SYMB);

	CDomyun Dom(pDom);

	pMng->SetProgressData(_T("도각 출력 중..."), 0);

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

	// 교량 받침 배치도 출력
//	long nPageNum = pInfo->m_nPageNumber;

	AddPageOne(&Dom, nSubPage);
/*
	if(nPageNum==OUT_GEN1)	// 첫번째 일반도 페이지 라면...
		AddPageOne(&Dom, nSubPage);
	else if( nPageNum == OUT_GEN2 )
		AddPageTwo(&Dom, nSubPage);	// 두번째 페이지( 보통 날개벽이 있을경우 추가되는 페이지)
*/
	pMng->SetProgressData(_T("출력 완료  ..."), 100);

	*pDom << Dom;
}

void CDrawingShoeArrange::AddPageOne(CDomyun* pDomP, long nSubPage)
{
}