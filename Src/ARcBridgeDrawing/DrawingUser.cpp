// DrawingUser.cpp: implementation of the CDrawingUser class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../../Src/ARcBridgeData/ARcBridgeData.h"
#include "../../Src/ARcBridgeDBDraw/ARcBridgeDBDraw.h"
#include "../ARcBridgeDrawingOption/ARcBridgeDrawingOption.h"
#include "ARcBridgeDrawing.h"
#include "DrawingUser.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDrawingUser::CDrawingUser()
{

}

CDrawingUser::~CDrawingUser()
{

}

long CDrawingUser::GetPageSu()
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;

	if(pStd->GetCountTotalPage()<=0)
		pStd->MakePageInfo();

	m_nPage	= pStd->GetCountCategoryPage(_T("슬래브 표준도"));

	return m_nPage;
}

CString CDrawingUser::GetTitle()
{
	return "사용자 정의";
}
double CDrawingUser::GetScale()
{
	return 50;
}
void CDrawingUser::SetNumber(long nSubPage)
{

}
void CDrawingUser::DrawOnePage()
{
	CARcBridgeDrawingStd* pStd = (CARcBridgeDrawingStd*)m_pStd;
	CARcBridgeDrawingOptionStd* pOpt = pStd->m_pARcBridgeDrawingOptionStd;
	CARcBridgeDataStd* pDataStd = pStd->m_pARcBridgeDataStd;
	CDrawDogak* pDgk = pStd->GetDogak();	
	CDrawPageMng* pMng = pStd->GetPageManage();
	double dScale = GetScale();

	long nPage	= pStd->m_nCurPage;
	CDomyun* pDom = pStd->GetDomyun(nPage);
	pDom->SetScaleDim(dScale);

	pStd->m_pOptStd->SetEnvType(pDom, HCAD_MIS3);
	pStd->m_pOptStd->SetEnvType(pDom, HCAD_SYMB);

	CDomyun Dom(pDom);

	pMng->SetProgressData(_T("도각 출력 중..."), 0);

	long nSttPage = 0;
	CString strDgkName = pOpt->m_strDgkName;
	CString strHeadNote = pOpt->m_strDomNumHead;
	CString strSmallTitle = GetTitle();
	CString strXRefPath = IsDxfOut() ? pOpt->m_strXRefPath : _T("");

	pDgk->Add(&Dom, nPage, dScale, strDgkName, strHeadNote, strSmallTitle, nSttPage, strXRefPath);
	pDgk->AddTitle(&Dom, "", strSmallTitle, _T(""), FALSE);

	pDataStd->m_strTempName = "사용자교량";
	pDgk->SetHCADExtendedData(pDataStd->m_strProjectName, "사용자교량", pDataStd->m_strEngName,
							  pDataStd->m_strConstructorName, pDataStd->m_strSpotName, _T(""));

	// 도각 크기를 페이지에 넘겨줘서 최대한 적절하게 배치 되도록 도움을 준다.
	if(m_bDrawForMe)
		*pDom << Dom;
	else
		Dom.ClearEtt(TRUE, FALSE);
}