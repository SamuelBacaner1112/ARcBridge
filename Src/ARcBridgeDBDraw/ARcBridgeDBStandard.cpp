// ARcBridgeDBStandard.cpp: implementation of the CARcBridgeDBStandard class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../../ARcBridge/ARcBridgeDoc.h"
#include "../../Src/ARcBridgeDrawingOption/ARcBridgeDrawingOptionStd.h"
#include "../../Src/ARcBridgeDrawingOption/ARcBridgeDrawingOptionData.h"
#include "ARcBridgeDBDraw.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CARoadOptionStd	*CARcBridgeDBStandard::m_pARoadOptionStd = NULL;
//BOOL CARcBridgeDBStandard::m_bUseHCadBlock = TRUE;
CDomyun	CARcBridgeDBStandard::m_HCadSymbolDom;
CStringArray *CARcBridgeDBStandard::m_pBlockNameArray = NULL;
CStringArray *CARcBridgeDBStandard::m_pBlockUserNameArray = NULL;
CString CARcBridgeDBStandard::m_sMarkForMarkRebar	= _T("");
BOOL CARcBridgeDBStandard::m_bViewVectorInfo	= FALSE;
BOOL CARcBridgeDBStandard::m_bDisplayAngLenStd	= FALSE;	// ��Ÿ� ǥ�� ���� ǥ��

/*
CARcBridgeDBStandard::CARcBridgeDBStandard(CRcBridgeApp *pRcBridge, CARoadOptionStd *pOptionStd, CARcBridgeDataStd *pARcBridgeDataStd)
{
	m_nCurGroupCode	= 1;
	m_pRcBridge = pRcBridge;
	m_pRcBridgeRebar = NULL;
	m_pARoadOptionStd = pOptionStd;
	m_pARcBridgeDataStd = pARcBridgeDataStd;
	m_bApplyDimSettingOfBaseDom	= FALSE;	// ���� �׸��� �Ѱܹ��� ������ ������ �״�� ���� ��ų��?
	SetStd();
}
*/

CARcBridgeDBStandard::CARcBridgeDBStandard(CRcBridgeRebar *pRcBridge, CARoadOptionStd *pOptionStd, CARcBridgeDataStd *pARcBridgeDataStd)
{
	m_nCurGroupCode	= 1;
	m_pRcBridge = dynamic_cast<CRcBridgeApp *>(pRcBridge);
	m_pRcBridgeRebar = pRcBridge;
	m_pARoadOptionStd = pOptionStd;
	m_pARcBridgeDataStd = pARcBridgeDataStd;
	m_bApplyDimSettingOfBaseDom	= FALSE;	// ���� �׸��� �Ѱܹ��� ������ ������ �״�� ���� ��ų��?
	m_pDrawingOptionData	= NULL;
	m_bDrawRebarInfoCycleByHidden	= FALSE;
	SetStd();

	m_bCheckOverLengthRebar	= FALSE;
	m_nSelectFckDomText	= 0;
}

CARcBridgeDBStandard::~CARcBridgeDBStandard()
{
	delete m_pDimParabola;
	delete m_pDrawVertSection;
	delete m_pDrawWingWall;
	delete m_pDrawPlane;
	delete m_pDrawCrossSection;
	delete m_pDrawWall;
	delete m_pDrawFooting;
	delete m_pDrawExFooting;
	delete m_pDrawColumn;
	delete m_pDrawAps;
	delete m_pDrawLine;
}

// UINT nTypeDrawOverLapLine : overlap ���� �׸��� ��� 
// 0 : �׸��� ����. 1 : ������ �׸�,
void CARcBridgeDBStandard::DrawTvArray(CDomyun *pDomP, CTwinVectorArray &tvArr, BOOL bDrawInfo, BOOL bDrawDirInfo, UINT nTypeDrawOverLapLine)
{
	CHgBaseDrawStd baseDraw;
	if(m_pRcBridge)
		baseDraw.m_pLine	= m_pRcBridge->GetLineBase();
	baseDraw.DrawTvArray(pDomP, tvArr, bDrawInfo, bDrawDirInfo, nTypeDrawOverLapLine);
}


// ���� �迭�� ���� �׸���.
void CARcBridgeDBStandard::DrawLineArray(CDomyun *pDomP, CDPointArray &xyArr)
{
	if(xyArr.GetSize() < 2) return;
	if(xyArr.GetSize() % 2 != 0) xyArr.RemoveAt(xyArr.GetSize()-1);

	long i = 0; for(i = 0; i < xyArr.GetSize()-1; i+=2)
		pDomP->LineTo(xyArr.GetAt(i), xyArr.GetAt(i+1));
}

// �׷� �ڵ� �ʱ�ȭ
// void CARcBridgeDBStandard::InitCurGroupCode()
// {
// 	m_nCurGroupCode = 1;
// }

// void CARcBridgeDBStandard::SettingCurGroupCode(BOOL bVal)
// {
// 	m_bSettingGroupCode = bVal;
// }

CObInsert* CARcBridgeDBStandard::InsertHCadBlock(CDomyun *pDomP, const CString &szBlkName, CDPoint xy, double dScale, double dRotDegAng, BOOL bHCADOnly)
{	
	CString szCovBlkName = szBlkName;
	if(m_pBlockNameArray)
	{
		long n=0; for(n=0; n<m_pBlockNameArray->GetSize(); n++)
		{
			if(m_pBlockNameArray->GetAt(n) == szCovBlkName)
			{
				szCovBlkName = m_pBlockUserNameArray->GetAt(n);
				break;
			}
		}
	}
	// ǥ�� ���̾� ����
	m_pARoadOptionStd->SetEnvType(pDomP, HCAD_SYMB);
	m_pARoadOptionStd->SetEnvType(pDomP, HCAD_BARA);
	m_pARoadOptionStd->SetEnvType(pDomP, HCAD_BARX);
	m_pARoadOptionStd->SetEnvType(pDomP, HCAD_BNTL);
	m_pARoadOptionStd->SetEnvType(pDomP, HCAD_BNTT);
	m_pARoadOptionStd->SetEnvType(pDomP, HCAD_BNTB);
	m_pARoadOptionStd->SetEnvType(pDomP, HCAD_MIS1);
	m_pARoadOptionStd->SetEnvType(pDomP, HCAD_MIS2);
	m_pARoadOptionStd->SetEnvType(pDomP, HCAD_TXTG);
	CString szImsiCode = m_pARoadOptionStd->m_szDetLayerCode;
	m_pARoadOptionStd->m_szDetLayerCode = "";
	m_pARoadOptionStd->SetEnvType(pDomP, HCAD_SYMB);
	m_pARoadOptionStd->SetEnvType(pDomP, HCAD_BARA);
	m_pARoadOptionStd->SetEnvType(pDomP, HCAD_BARX);
	m_pARoadOptionStd->SetEnvType(pDomP, HCAD_BNTL);
	m_pARoadOptionStd->SetEnvType(pDomP, HCAD_BNTT);
	m_pARoadOptionStd->SetEnvType(pDomP, HCAD_BNTB);
	m_pARoadOptionStd->SetEnvType(pDomP, HCAD_MIS1);
	m_pARoadOptionStd->SetEnvType(pDomP, HCAD_MIS2);
	m_pARoadOptionStd->SetEnvType(pDomP, HCAD_TXTG);
	m_pARoadOptionStd->m_szDetLayerCode = szImsiCode;

	BOOL bFind = m_HCadSymbolDom.IsExistBlock(szCovBlkName+"-NOATTR");
	if(IsUseHCadBlock() || !bFind || bHCADOnly)
	{
		if(m_HCadSymbolDom.IsExistBlock(szCovBlkName))
		{
			if(!pDomP->IsExistBlock(szCovBlkName))
			{
				CMapStringToPtr* pBlockArray = m_HCadSymbolDom.GetBlockArray();
				CDomyunBase* p;					
				if( pBlockArray->Lookup( szCovBlkName, (void *&)p ) )
				{
					CObInsert* pInsert = new CObInsert, *pInsertFind=NULL;
					CPtrList *pInsertList = m_HCadSymbolDom.GetObjectList(ETT_INSERT);
					POSITION pos = pInsertList->GetHeadPosition();
					while(pos)
					{
						CObInsert* p = (CObInsert*)pInsertList->GetNext(pos);
						if(p->m_sBlockName == szCovBlkName) { pInsertFind = p; break; }
					}				 
					pInsert->SetsLayer(m_pARoadOptionStd->m_szDetLayerCode + ((szCovBlkName == "CSBSEC1") ? "CS-BARX" : "CS-SYMB"));
					pInsert->m_sBlockName = szCovBlkName;
					pInsert->m_vPoint = xy;	
					pInsert->m_vSFactor = CVector(dScale,dScale,dScale);
					pInsert->m_RotationAngle = dRotDegAng;					
					if(pInsertFind)
					{
						AhPtrListCopy(pInsert->m_AttDefArr,pInsertFind->m_AttDefArr,(CObAttdef*)0);														
						pos = pInsert->m_AttDefArr.GetHeadPosition();
						while(pos)
						{
							CObAttdef* p = (CObAttdef*)pInsert->m_AttDefArr.GetNext(pos);						
							p->m_ptPoint = xy + (p->m_ptPoint - pInsertFind->m_vPoint) * dScale;
							p->m_ptPointSecond = xy + (p->m_ptPointSecond - pInsertFind->m_vPoint) * dScale;
							p->m_TextStyle.Height *= dScale;
						}
					}				
					
					CDomyunBase *pBlockDom = new CDomyunBase;					
					*pBlockDom = *p;
					m_pARoadOptionStd->ChangeLayerOnSet(pInsert);
					m_pARoadOptionStd->ChangeDomLastLayerNameForAPier(pBlockDom);					
					pDomP->AddBlockAndInsert(pBlockDom, pInsert);						
					return pInsert;
				}
			}	
			else
			{									
				CObInsert* pInsert = new CObInsert, *pInsertFind=NULL;	
				CPtrList *pInsertList = m_HCadSymbolDom.GetObjectList(ETT_INSERT);
				POSITION pos = pInsertList->GetHeadPosition();
				while(pos)
				{
					CObInsert* p = (CObInsert*)pInsertList->GetNext(pos);
					if(p->m_sBlockName == szCovBlkName) { pInsertFind = p; break; }
				}		
				if(pInsertFind)
				{
					AhPtrListCopy(pInsert->m_AttDefArr,pInsertFind->m_AttDefArr,(CObAttdef*)0);														
					pos = pInsert->m_AttDefArr.GetHeadPosition();
					while(pos)
					{
						CObAttdef* p = (CObAttdef*)pInsert->m_AttDefArr.GetNext(pos);						
						p->m_ptPoint = xy + (p->m_ptPoint - pInsertFind->m_vPoint) * dScale;
						p->m_ptPointSecond = xy + (p->m_ptPointSecond - pInsertFind->m_vPoint) * dScale;
						p->m_TextStyle.Height *= dScale;
					}
				}	
				pInsert->SetsLayer(m_pARoadOptionStd->m_szDetLayerCode + (szCovBlkName == "CSBSEC1" ? "CS-BARX" : "CS-SYMB"));
				pInsert->m_sBlockName = szCovBlkName;
				pInsert->m_vPoint = xy;	
				pInsert->m_vSFactor = CVector(dScale,dScale,dScale);
				m_pARoadOptionStd->ChangeLayerOnSet(pInsert);
				pDomP->Insert(pInsert);				
				return pInsert;
			}
		}
	}
	else
	{
		if(m_HCadSymbolDom.IsExistBlock(szCovBlkName+"-NOATTR"))
		{
			CMapStringToPtr* pBlockArray = m_HCadSymbolDom.GetBlockArray();
			CDomyun Dom(pDomP);
			CDomyunBase* p;					
			if( pBlockArray->Lookup( szCovBlkName+"-NOATTR", (void *&)p ) )
			{				
				m_pARoadOptionStd->ChangeDomLastLayerNameForAPier(p);
				Dom += *p;												
				Dom.RedrawByScale(dScale);
				Dom.Move(xy);
				*pDomP << Dom;				
			}
		}
	}

	return NULL;	
}


BOOL CARcBridgeDBStandard::IsUseHCadBlock()
{
	return CHgBaseDrawStd::IsUseHCadBlock();
	//return m_bUseHCadBlock;
}

void CARcBridgeDBStandard::SetUseHCadBlock(BOOL vUseBlock)
{
	CHgBaseDrawStd::SetUseHCadBlock(vUseBlock);
	//m_bUseHCadBlock = vUseBlock;
}

CDomyun* CARcBridgeDBStandard::GetHCadSymbolDom()
{
	return &m_HCadSymbolDom;
}

// rectCut�� �������� ���ܼ� ǥ�� �����ؼ� tvArr�� �׷��ش�.
void CARcBridgeDBStandard::DrawCuttingTvArray(CDomyun *pDomP, CTwinVectorArray &tvArr, CDRect rectCut, BOOL bDrawCuttingLine)
{
	CRebarPlacing rbPlacing;
	if(m_pRcBridge)
		rbPlacing.m_pLine	= m_pRcBridge->GetLineBase();
	CVectorArray vecArrArea;
	vecArrArea.Add(CVector(rectCut.left, rectCut.top, 0));
	vecArrArea.Add(CVector(rectCut.right, rectCut.top, 0));

	vecArrArea.Add(CVector(rectCut.right, rectCut.top, 0));
	vecArrArea.Add(CVector(rectCut.right, rectCut.bottom, 0));

	vecArrArea.Add(CVector(rectCut.right, rectCut.bottom, 0));
	vecArrArea.Add(CVector(rectCut.left, rectCut.bottom, 0));

	vecArrArea.Add(CVector(rectCut.left, rectCut.bottom, 0));
	vecArrArea.Add(CVector(rectCut.left, rectCut.top, 0));

	CTwinVectorArray tvArrArea;
	tvArrArea.AddFromVectorArray(vecArrArea);

	// Ŀ�ö����� ������ ���̴� ���� üũ��.
	BOOL bLeft = FALSE;
	BOOL bRight = FALSE;
	BOOL bTop = FALSE;
	BOOL bBottom = FALSE;
	
	// ������ �ջ��Ű�� ����ϹǷ�...
	CTwinVectorArray tvArrTmp;
	tvArrTmp = tvArr;
	rbPlacing.GetTvArrayByCrossTvArrayArea(tvArrTmp, tvArrArea, TRUE);
	CDRect rectTvArr = tvArrTmp.GetRect();	// �׸� �׸��� �簢��ǥ

	
	// �˻�
	if(rectCut.left >= rectTvArr.left) bLeft = TRUE;
	if(rectCut.right <= rectTvArr.right) bRight = TRUE;
	if(rectCut.top <= rectTvArr.top) bTop = TRUE;
	if(rectCut.bottom >= rectTvArr.bottom) bBottom = TRUE;


	CDomyun Dom(pDomP);
	DrawTvArray(&Dom, tvArrTmp, IsViewVectorInfo(), IsViewVectorInfo());

	// �߸� �κп� ���ؼ� Ŀ�ö��� ǥ�� �Ѵ�.
	// ����� �� �˻�.
	if(tvArrTmp.GetSize() > 0 && bDrawCuttingLine)
	{
		m_pARoadOptionStd->SetEnvType(&Dom,HCAD_CUTL);

		BOOL bDir = FALSE;
		DWORD dwArea1 = FALSE;
		DWORD dwArea2 = FALSE;
		BOOL bVer = FALSE;
		double dLenExt = Dom.Always(5);
		CDPointArray xyArr1;
		CDPointArray xyArr2;
		tvArrTmp.GetToPointArray(xyArr1);
		tvArrTmp.GetToPointArray(xyArr2);

		long i = 0; for(i = 0; i < 4; i++)
		{
			switch(i)
			{
			case 0: bDir = bLeft; dwArea1 = LEFTDOWN_AREA; dwArea2 = LEFTUP_AREA; bVer = TRUE; break;
			case 1: bDir = bRight; dwArea1 = RIGHTDOWN_AREA; dwArea2 = RIGHTUP_AREA; bVer = TRUE; break;
			case 2: bDir = bTop; dwArea1 = UPLEFT_AREA; dwArea2 = UPRIGHT_AREA; bVer = FALSE; break;
			case 3: bDir = bBottom; dwArea1 = DOWNLEFT_AREA; dwArea2 = DOWNRIGHT_AREA; bVer = FALSE; break;
			}

			if(bDir)
			{
				CDPoint xy1 = rbPlacing.GetXyEdgePoint(xyArr1, dwArea1);
				CDPoint xy2 = rbPlacing.GetXyEdgePoint(xyArr2, dwArea2);
				
				// �� ��ǥ�� ���� ���..
				if(ComparePt(xy1, xy2, "="))
				{
					switch(i)
					{
					case 0: xy1 = CDPoint(rectCut.left, rectCut.bottom); xy2 = CDPoint(rectCut.left, rectCut.top); break;
					case 1: xy1 = CDPoint(rectCut.right, rectCut.bottom); xy2 = CDPoint(rectCut.right, rectCut.top); break;
					case 2: xy1 = CDPoint(rectCut.left, rectCut.top); xy2 = CDPoint(rectCut.right, rectCut.top); break;
					case 3: xy1 = CDPoint(rectCut.left, rectCut.bottom); xy2 = CDPoint(rectCut.right, rectCut.bottom); break;
					}
				}

				CDPoint xyDir = (xy2-xy1).Unit();
				xy1 += xyDir*-dLenExt;
				xy2 += xyDir*dLenExt;

				Dom.CutLightning(xy1.x, xy1.y, xy2.x, xy2.y, bVer, 1);
			}
		}
	}
	
	*pDomP << Dom;
}

// rcCut�� �������� ���ܼ� ǥ�� �����ؼ� tvArr�� �׷��ش�.
// �� �ϳ��� Cutting Line�� ǥ��(������ ��鵵���� ���. ���� ���� �ʿ�)
// void CARcBridgeDBStandard::DrawCuttingTvArraySingle(CDomyun *pDomP, CTwinVectorArray &tvArr, CDRect rcCut, BOOL bDrawInfo)
// {
// 	CRebarPlacing rbPlacing;
// 	if(m_pRcBridge)
// 		rbPlacing.m_pLine	= m_pRcBridge->GetLineBase();
// 	CVectorArray vecArrArea;
// 	vecArrArea.Add(CVector(rcCut.left, rcCut.top, 0));
// 	vecArrArea.Add(CVector(rcCut.right, rcCut.top, 0));
// 
// 	vecArrArea.Add(CVector(rcCut.right, rcCut.top, 0));
// 	vecArrArea.Add(CVector(rcCut.right, rcCut.bottom, 0));
// 
// 	vecArrArea.Add(CVector(rcCut.right, rcCut.bottom, 0));
// 	vecArrArea.Add(CVector(rcCut.left, rcCut.bottom, 0));
// 
// 	vecArrArea.Add(CVector(rcCut.left, rcCut.bottom, 0));
// 	vecArrArea.Add(CVector(rcCut.left, rcCut.top, 0));
// 
// 	// ������ �ջ��Ű�� ����ϹǷ�...
// 	CTwinVectorArray tvArrArea;
// 	CTwinVectorArray tvArrTmp;
// 	tvArrTmp = tvArr;
// 	tvArrArea.AddFromVectorArray(vecArrArea);
// 	rbPlacing.GetTvArrayByCrossTvArrayArea(tvArrTmp, tvArrArea, TRUE);
// 
// 	CDomyun Dom(pDomP);
// 
// 	DrawTvArray(&Dom, tvArrTmp, bDrawInfo);
// 
// 	CDPoint xy1(0, 0), xy2(0, 0), xy3(0, 0), xy4(0, 0), xyResult(0, 0);
// 	CDPointArray xyArr;
// 	CTwinVector tv;
// 	BOOL bCross = FALSE;
// 	double dDist1 = 0, dDist2 = 0;
// 	int nSize = tvArrTmp.GetSize();
// 
// 	// �������� Ȯ���� ã�� ���� ������ 0.001��ŭ ��ҽ�Ŵ
// 	rcCut.top -= 0.001;
// 	rcCut.right -= 0.001;
// 	rcCut.left += 0.001;
// 	rcCut.bottom += 0.001;
// 
// 	int i=0; for(i=0; i<4; i++)	// top, right, left, bottom
// 	{
// 		switch(i)
// 		{
// 		case 0:
// 			xy1 = rcCut.TopLeft();
// 			xy2 = rcCut.TopRight();
// 			break;
// 		case 1:
// 			xy1 = rcCut.TopRight();
// 			xy2 = rcCut.BottomRight();
// 			break;
// 		case 2:
// 			xy1 = rcCut.TopLeft();
// 			xy2 = rcCut.BottomLeft();
// 			break;
// 		case 3:
// 			xy1 = rcCut.BottomLeft();
// 			xy2 = rcCut.BottomRight();
// 			break;
// 		}
// 		for(int j=0; j<nSize; j++)
// 		{
// 			tv = tvArrTmp.GetAt(j);
// 			bCross = GetXyMatchSegAndSeg(xy1, xy2, tv.m_v1, tv.m_v2, xyResult);
// 			if(bCross)
// 			{
// 				dDist1 = ~(xyResult-tv.m_v1);
// 				dDist2 = ~(xyResult-tv.m_v2);
// 				xyResult = dDist1>dDist2 ? tv.m_v2 : tv.m_v1;
// 				xyArr.Add(xyResult);
// 			}
// 		}
// 	}
// 
// 	nSize = xyArr.GetSize();
// 	if(nSize>0)
// 	{
// 		m_pARoadOptionStd->SetEnvType(&Dom,HCAD_CUTL);
// 		Dom.CutLightningAnyDir(xyArr.GetAt(0), xyArr.GetAt(nSize-1), 500);
// 	}
// 
// 	*pDomP << Dom;
// }

void CARcBridgeDBStandard::RotateDomByBridgeSttEndXy(CDomyun *pDomP, BOOL bMoveOrigin)
{
	if(m_pRcBridge)
	{
		CLineInfoApp *pLine = dynamic_cast<CLineInfoApp*>(m_pRcBridge->GetLineBase());
		CDPoint xyStt   = pLine->GetXyLine(m_pRcBridge->m_dStationBridgeStt);
		pDomP->Rotate(xyStt, m_pRcBridge->GetXyDirRotateDomByBridgeSttEndXy());
		if(bMoveOrigin)
			pDomP->Move(-xyStt.x, -xyStt.y);
	}
}
void CARcBridgeDBStandard::SetAttrib(CDomyun *pDom, CObInsert *pIns, const CString& szTag, const CString& szText)
{				
	if(IsUseHCadBlock() || ( pIns && pIns->m_AttDefArr.GetCount()>0))
	{		
		if(!pIns) return;
		POSITION pos = pIns->m_AttDefArr.GetHeadPosition();
		while(pos)
		{
			CObAttdef* p = (CObAttdef*)pIns->m_AttDefArr.GetNext(pos);						
			if(p->m_szTag == szTag) 
			{
				p->m_szText = szText;
				break;
			}
		}
	}
	else
	{				
		pDom->ReplaceString(szTag, szText);
	}
}

// rectCut������ŭ ���η� �߶� ����ǥ�ñ��� ����(��հ����� �׸��� �����Ұ���)
// void CARcBridgeDBStandard::DrawCuttingTvArrayHorRange(CDomyun *pDomP, CTwinVectorArray &tvArr, CDRect rectCut, BOOL bDrawCuttingLine)
// {
// 	CTwinVectorArray tvArrUpper, tvArrLower;	// ��, �Ʒ��� ���� ����
// 	CRebarPlacing rbPlacing;
// 	if(m_pRcBridge)
// 		rbPlacing.m_pLine	= m_pRcBridge->GetLineBase();
// 
// 	CDomyun Dom(pDomP);
// 	if(rbPlacing.CuttingHorRange(tvArr, rectCut, tvArrUpper, tvArrLower, TRUE, 100))
// 	{
// 		DrawTvArray(pDomP, tvArrUpper);
// 		DrawTvArray(pDomP, tvArrLower);
// 
// 		if(bDrawCuttingLine)
// 		{
// 			// ���� ������ �������� ���ö����� �׸���.
// 			CDRect rectUpper = tvArrUpper.GetRect();
// 			CDRect rectLower = tvArrLower.GetRect();
// 			double dLeft = min(rectUpper.left, rectLower.left);
// 			double dRight = max(rectUpper.right, rectLower.right);
// 
// 			m_pARoadOptionStd->SetEnvType(&Dom,HCAD_CUTL);
// 
// 			Dom.CutLightning(dLeft, rectUpper.bottom, dRight, rectUpper.bottom, FALSE, 1);
// 			Dom.CutLightning(dLeft, rectLower.top, dRight, rectLower.top, FALSE, 1);
// 		}
// 	}
// 	else
// 	{
// 		DrawTvArray(pDomP, tvArr);
// 	}
// 
// 	*pDomP << Dom;
// }

// rebar cycle info �׸���.
// BOOL bDecreaseSttRebarInFooting : ���ʿ� �� �ִ� ���ۺ� ö���� �������� ���̵��� �ణ ���°� ���� �ʴ´�.
// CParabola *pPb : �������� ���� �Ǿ� �ִٸ�..
void CARcBridgeDBStandard::DrawRebarInfoCycle(CDomyun *pDomP, CRebarInfoCycle *pRC, long nDrawLeft, CDRect rectLimit, BOOL bDecreaseSttRebarInFooting, CParabola *pPb)
{
	if(!pRC) return;
	if(Compare(pRC->GetLength(), 0.0, "=")) return;
	if(!pRC->m_bExist) return;
	if(m_bDrawRebarInfoCycleByHidden) return;	// ������ �������� �׷��� ��

	CRcBridgeData data;

	// ���� �ϸ��� ��� �������� ���̵��� ���� �ణ �ٿ��� �׷��ش�.
	double dGap	= bDecreaseSttRebarInFooting ? 50 : 0;
	if(pRC->m_nPosStt == REBARINFO_POS_FOOTING_LOWER && !pPb)
	{
		pRC->m_xyStt.y	+= dGap;
		pRC->m_xyMid1.y	+= dGap;
		pRC->m_xyMid2.y	+= dGap;
		pRC->m_xyCen.y	+= dGap;
	}

	CTwinVectorArray tvArr;
	CTwinVector tv;
	CRcBridgeRebar *pBri	= m_pRcBridgeRebar;
	BOOL bSttRebar	= TRUE;

	if(pRC->m_bUseTvArrRebar)
	{
		tvArr	= pRC->m_tvArrRebar;
	}
	else
	{
		// ���� ��������?(ö���� 2���̻��ΰ��..)
		BOOL bApplyRound	= TRUE;

		// ���ʿ��� ������ ö���� ������ ���� ���� ����.
		if(pRC->m_nPosEnd == REBARINFO_POS_FOOTING_UPPER || pRC->m_nPosEnd == REBARINFO_POS_FOOTING_LOWER ||
			pRC->m_nPosEnd == REBARINFO_POS_FOOTING_LEFT || pRC->m_nPosEnd == REBARINFO_POS_FOOTING_RIGHT)
			bApplyRound	= FALSE;

		// ��, ��ν������ ������ü�� ����� ���´� ���� ���� ��.
		if(pBri->IsMainRebarJointUpperSlabAndOutWall(pRC, bSttRebar))
		{
			bApplyRound	= TRUE;
		}

		if(Compare(pRC->m_R_Mid, 0.0, "=")) bApplyRound	= FALSE;

		
		if(pRC->m_nPosStt != pRC->m_nPosEnd)
		{

			if(pPb && pRC->m_nPosStt != REBARINFO_POS_FOOTING_LOWER)
			{
				double dDistStt	= pPb->GetDistByXPos(pRC->m_xyStt.x);
				double dDistEnd	= pPb->GetDistByXPos(pRC->m_xyMid1.x);
				data.AddFromParabola(tvArr, *pPb, dDistStt, dDistEnd);
				tv.m_v1	= pRC->m_xyMid1;
			}
			else
			{
				tv.m_v1	= pRC->m_xyStt;
				tv.m_v2	= pRC->m_xyMid1;
				tv.m_v1.z	= 0;
				tv.m_v2.z	= 0;
				tvArr.Add(tv);
				tv.m_v1	= tv.m_v2;
			}

			
			tv.m_v2	= pRC->m_xyMid2;

			long nLeftCenter	= -1;
			if(bApplyRound)
			{
				// ��� ������ ö�ٸ� ����Ǵµ�
				// �Ϻ� �����꿡 ������ ���°��� ������ ������ü�� ���� ������ü�̱� ������ ��������.
				// ���� ��ü���� �����ؼ� ������ü���� �����ٸ� �Ǵ��� ����� �Ѵ�.
				if(pRC->m_nPosStt == REBARINFO_POS_STTWALL_OUTTER && pRC->m_nPosEnd == REBARINFO_POS_ENDWALL_OUTTER)
				{
					nLeftCenter	= (pRC->m_xyStt.y > pRC->m_xyMid1.y)? -1 : 1;
				}
				else if(pRC->m_nPosStt == REBARINFO_POS_UPPERSLAB_UPPER || pRC->m_nPosEnd == REBARINFO_POS_UPPERSLAB_UPPER)
				{
					nLeftCenter	= 1;
				}
				else
				{
					nLeftCenter	= pRC->m_xyMid2.x > pRC->m_xyMid1.x ? -1 : 1;
				}

				// ������ü�� ����� ��ν����� ö���� ���
				if(pBri->IsMainRebarJointUpperSlabAndOutWall(pRC, bSttRebar))
				{
					nLeftCenter	= bSttRebar ? -1 : 1;
				}

				tv.m_v1.z	= nLeftCenter;
				tv.m_v2.z	= pRC->m_R_Mid;//fabs(pRC->m_xyMid2.x - pRC->m_xyMid1.x);

				if(nLeftCenter == 1)
					tv.m_v2.z = tv.m_v2.z * -1;
			}
			tvArr.Add(tv);
			if(pPb && pRC->m_nPosStt == REBARINFO_POS_FOOTING_LOWER)
			{
				double dDistStt	= pPb->GetDistByXPos(pRC->m_xyMid2.x);
				double dDistEnd	= pPb->GetDistByXPos(pRC->m_xyEnd.x);

				data.AddFromParabola(tvArr, *pPb, dDistStt, dDistEnd);
				tv.m_v1	= pPb->GetXyByDist(dDistStt);
				tv.m_v2	= pPb->GetXyByDist(dDistEnd);
			}
			else
			{
				tv.m_v1	= pRC->m_xyMid2;
				tv.m_v2	= pRC->m_xyEnd;
				tv.m_v1.z	= 0;
				tv.m_v2.z	= 0;
				tvArr.Add(tv);
			}

			// ������ü�� ����� ��ν����� ö���� ���
			if(pBri->IsMainRebarJointUpperSlabAndOutWall(pRC, bSttRebar))
				nLeftCenter	= bSttRebar ? 1 : -1;

			if(pRC->m_bUseMidExt)
			{
				tv.m_v1		= tv.m_v2;
				tv.m_v2		= pRC->m_xyMidExt;

				// ��ݰ��� 0�̸� �׳� �������� �׷��ش�.
				if(pRC->m_R_MidExt != 0)
				{
					tv.m_v1.z	= nLeftCenter;
					tv.m_v2.z	= pRC->m_R_MidExt;
					if(nLeftCenter == 1)
						tv.m_v2.z = tv.m_v2.z * -1;
				}
				tvArr.Add(tv);
			}

			if(pRC->m_bUseEndExt)
			{
				tv.m_v1		= tv.m_v2;
				tv.m_v2		= pRC->m_xyEndExt;
				tv.m_v1.z	= 0;
				tv.m_v2.z	= 0;
				tvArr.Add(tv);
			}
		}
		else
		{
			if(pPb)
			{				
				double dDistStt	= pPb->GetDistByXPos(pRC->m_xyStt.x);
				double dDistEnd	= pPb->GetDistByXPos(pRC->m_xyMid1.x);
				data.AddFromParabola(tvArr, *pPb, dDistStt, dDistEnd);
			}
			else
			{
				tv.m_v1	= pRC->m_xyStt;
				tv.m_v2	= pRC->m_xyMid1;
				tv.m_v1.z	= 0;
				tv.m_v2.z	= 0;
				tvArr.Add(tv);
			}
		}
	}
	
	if(rectLimit != CDRect(0, 0, 0, 0))
	{
		CTwinVectorArray tvArrLimit;
		tvArrLimit.SetFromRect(rectLimit);
		
		// ���� ���ʸ� ����.
		CRebarPlacing rb;
		CVectorArray vecArrArea;
		tvArrLimit.GetToVectorArray(vecArrArea);
		rb.GetTvArrayByCrossVectorArrayArea(tvArr, vecArrArea, TRUE);
	}

	if(nDrawLeft != 0)
	{
		CRebarPlacing rb;
		CDRect rect	= tvArr.GetRect();
		double dStaCuttingOrg	= GetStaCuttingOrg();
		CDPoint xy1	= CDPoint(dStaCuttingOrg, rect.bottom-100),
				xy2	= CDPoint(dStaCuttingOrg, rect.top+100);

		CVectorArray vecArr;
		tvArr.GetToVectorArray(vecArr);
		if(nDrawLeft == -1)
		{
			rb.TrimVectorArrayByLine(vecArr, xy1, xy2, FALSE, TRUE, FALSE);
		}
		else if(nDrawLeft == 1)
		{
			rb.TrimVectorArrayByLine(vecArr, xy1, xy2, TRUE, TRUE, FALSE);
		}
		
		tvArr.SetFromVectorArray(vecArr);
	}

	DrawTvArray(pDomP, tvArr);


	// ���� �ϸ��� ��� �������� ���̵��� ���� �ణ �ٿ��� �׷��ذ� ����
	if(pRC->m_nPosStt == REBARINFO_POS_FOOTING_LOWER && !pPb)
	{
		pRC->m_xyStt.y	-= dGap;
		pRC->m_xyMid1.y	-= dGap;
		pRC->m_xyMid2.y	-= dGap;
		pRC->m_xyCen.y	-= dGap;
	}
}

void CARcBridgeDBStandard::SetStd()
{
	m_pDimParabola	= new CDimParabola;
	m_pDimParabola->m_pStd = this;

	m_pDrawVertSection = new CDrawVertSection;
	m_pDrawVertSection->m_pStd = this;

	m_pDrawWingWall = new CDrawWingWall;
	m_pDrawWingWall->m_pStd = this;

	m_pDrawPlane = new CDrawPlane;
	m_pDrawPlane->m_pStd = this;

	m_pDrawCrossSection = new CDrawCrossSection;
	m_pDrawCrossSection->m_pStd = this;

	m_pDrawWall = new CDrawWall;
	m_pDrawWall->m_pStd = this;

	m_pDrawFooting = new CDrawFooting;
	m_pDrawFooting->m_pStd = this;

	m_pDrawExFooting = new CDrawExFooting;
	m_pDrawExFooting->m_pStd = this;

	m_pDrawColumn = new CDrawColumn;
	m_pDrawColumn->m_pStd = this;

	m_pDrawAps = new CDrawApproachSlab;
	m_pDrawAps->m_pARoadOptionStd = m_pARoadOptionStd;

	m_pDrawLine = new CDrawLine;
	m_pDrawLine->m_pStd = this;
}

// rebarinfocycle array���� ġ�����Կ� ������ �м��ؼ� �������ش�.
// sArrInfo ���� : 
// 0 : �����
// 1 : ö���� ���� ����(�Է¹޴� ö���� ����)
// 2 : �Է¹��� �ʴ� ö���� ���� ����(���峪 ����� ������)
// BOOL bApplyJoint : ������ ����ؼ� ������ �м�����?
// BOOL bApplyEndIs2 : ������ ö���� ������ 2�� ��������?
// BOOL bReverse : ������� ö���� ���� ������ �ݴ�� ã��.. (2�� �״��)
//// #ARCBRIDGE-3716 bReverse ���ڴ� ���� �ٸ� �ɼ����� ������� �ʰ� ����ϴ� ���� ����,
//// GetValueJoint �Լ����� �ش� ���ڷ� ö�ٹ����� �ݴ�� ���ϴ� �������� ����Ϸ��� ������ ������(������ ���������� Ȯ��ġ ����)
void CARcBridgeDBStandard::GetDimInfoAtRebarInfoCycle(CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC, BOOL bHor, CDPointArray &xyArrDim, CStringArray &sArrInfo, BOOL bMerge, BOOL bApplyJoint, BOOL bApplyEndIs2, BOOL bReverse, double dCoverStt, double dCoverEnd, long nVertDir)
{
	if(m_bDrawRebarInfoCycleByHidden)
	{
		xyArrDim.RemoveAll();
		sArrInfo.RemoveAll();
		return;
	}
	if(!bMerge)
	{
		xyArrDim.RemoveAll();
		sArrInfo.RemoveAll();
	}
	

	// ���������� ������� ���� �Լ��̴�. /////////////////////////////////////
	// ���� rc�� ���������� �ִ� �ΰ��� ���ļ� �ϳ��� ���� �ڿ� 
	// ġ������ ������ �����Ѵ�.
	CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> pArrRCTmp;
	
	long i = 0; for(i = 0; i < pArrRC->GetSize(); i++)
	{
		CRebarInfoCycle *pRC	= pArrRC->GetAt(i);
		CRebarInfoCycle *pRCNew	= new CRebarInfoCycle;

		*pRCNew = *pRC;

		if(i > 0 && i < pArrRC->GetSize()-1)
		{
			if(pRC->m_nPosEnd == REBARINFO_POS_UPPERSLAB_SIDE)
			{
				CRebarInfoCycle *pRCNext	= pArrRC->GetAt(i+1);
				pRCNew->m_nPosEnd	= pRCNext->m_nPosEnd;
				if(pRCNext->m_nPosStt == pRCNext->m_nPosEnd)
					pRCNew->m_xyMid1	= pRCNext->m_xyMid1;
				else
					pRCNew->m_xyMid1	= pRCNext->m_xyEnd;
				i++;
			}
		}

		pArrRCTmp.Add(pRCNew);
	}

	// ������ �������� ������ ������ �������� mid���� �������� �� ���
	// ������ ������ mid���� �������� �� ��찡 �ִ�. �̰� ������ mid�� ���� 
	CRcBridgeRebar *pBri	= m_pRcBridgeRebar;
	for(i = 0; i < pArrRCTmp.GetSize(); i++)
	{
		CRebarInfoCycle *pRC	= pArrRCTmp.GetAt(i);
		if(pBri->IsBlockOut(TRUE) && pRC->m_nPosStt == REBARINFO_POS_UPPERSLAB_SIDE && pRC->m_nPosEnd	== REBARINFO_POS_UPPERSLAB_LOWER)
		{
			pRC->m_xyStt	= pRC->m_xyMid1;
		}
		else if(pBri->IsBlockOut(FALSE) && pRC->m_nPosStt == REBARINFO_POS_UPPERSLAB_LOWER && pRC->m_nPosEnd == REBARINFO_POS_UPPERSLAB_SIDE)
		{
			pRC->m_xyEnd	= pRC->m_xyMid2;
		}
	}
	/////////////////////////////////////////////////////////////////////////////

	



	CDPoint xyLast(0, 0);
	CDPoint xyDir			= bHor ? CDPoint(1, 0) : CDPoint(0, 1);
	CDPoint xyDirOrtho		= bHor ? CDPoint(0, 1) : CDPoint(1, 0);
	double dLengthJointStt	= 0;
	double dLengthJointEnd	= 0;
	long nSize				= pArrRCTmp.GetSize();	

	// ������ġ�� ö��������� �Ǵ�.
	if(nSize > 0) 
	{
		CRebarInfoCycle *pRC = pArrRCTmp.GetAt(0);
		CDPoint xyStt(0, 0);
		CDPoint xyEnd(0, 0);
		if(pRC->m_nPosStt != pRC->m_nPosEnd)
		{
			if(!GetXyMatchLineAndLine(pRC->m_xyStt, (pRC->m_xyMid1-pRC->m_xyStt).Unit(), pRC->m_xyEnd, (pRC->m_xyMid2-pRC->m_xyEnd).Unit(), xyLast))
				xyLast	= pRC->m_xyMid1;
			
			xyStt	= pRC->m_xyMid2;
			xyEnd	= pRC->m_xyEnd;

			if(pRC->m_bUseMidExt && pRC->m_bUseEndExt)
			{
				xyEnd	= pRC->m_xyEndExt;
			}
		}
		else
		{
			xyLast	= pRC->m_xyStt;
			xyStt	= pRC->m_xyStt;
			xyEnd	= pRC->m_xyMid1;
		}
	
		xyDir		= (xyEnd-xyStt).Unit();
		xyDirOrtho	= ToDPointFrDegree(xyDir.GetAngleDegree()+90);

		xyArrDim.Add(xyLast);
		sArrInfo.Add("0");
		if(nSize == 1)
		{
			xyArrDim.Add(xyEnd);
			sArrInfo.Add("0");

			// �Ǻ� ���� �߰��Ѵ�.
			// ó���� ���κп� �Ǻ� ���� �߰��Ѵ�. ////////////////
			if(xyArrDim.GetSize() > 1)
			{
				CDPoint xy1	= xyArrDim.GetAt(0);
				CDPoint xy2	= xyArrDim.GetAt(xyArrDim.GetSize()-1);

				xy1 = xy1 + (xy1-xy2).Unit() * dCoverStt;
				xyArrDim.InsertAt(0, xy1);
				sArrInfo.InsertAt(0, "0");

				xy2	= xy2 + (xy2-xy1).Unit() * dCoverEnd;
				xyArrDim.Add(xy2);
				sArrInfo.Add("0");
			}
			////////////////////////////////////////////////////////

			AhTPADelete(&pArrRCTmp, (CRebarInfoCycle*)0);
			return;	// 1���� ���� ��� �̷��Ը� �ϰ� �������Ѵ�. ������ �ڽð� ����.
		}
	}
	
	double dLenJointLast	= 0;
	for(i = 0; i < nSize; i++)
	{
		CRebarInfoCycle *pRC = pArrRCTmp.GetAt(i);
		double dLenJoint	= 0;
		if(i < nSize-1 && m_pRcBridgeRebar)
		{
			dLenJoint	= m_pRcBridgeRebar->GetValueJoint(pRC, pArrRCTmp.GetAt(i+1)->m_Dia, NULL, nVertDir, bReverse);
			if(Compare(pRC->GetLength(), 0.0, "="))
				dLenJoint	= 0;
		}
		

		// ����, �߰�, ���� ���� �Ǿ�� �� ���� ����..
		if(bApplyJoint)
		{
			if(i == 0)
			{
				dLengthJointStt	= 0;
				dLengthJointEnd	= dLenJoint/2;
			}
			else if(i == nSize-1)
			{
				dLengthJointStt	= dLenJointLast/2;
				dLengthJointEnd	= 0;
			}
			else 
			{
				dLengthJointStt	= dLenJointLast/2;
				dLengthJointEnd	= dLenJoint/2;
			}
		}
		dLenJointLast	= dLenJoint;


		// ������ġ�� ����ġ�� �ٸ��� �����(����)�� �ִ�.
		if(pRC->m_nPosStt != pRC->m_nPosEnd)
		{
			CDPoint xyStt	= pRC->m_xyStt;

			if(i == 0)
			{
				if(!GetXyMatchLineAndLine(pRC->m_xyStt, (pRC->m_xyMid1-pRC->m_xyStt).Unit(), pRC->m_xyEnd, (pRC->m_xyMid2-pRC->m_xyEnd).Unit(), xyStt))
					xyStt = pRC->m_xyMid1;
			}
			xyStt			= xyStt + (xyDir*dLengthJointStt);
			CDPoint xyMid1	= pRC->m_xyMid1;
			CDPoint xyMid2	= pRC->m_xyMid2;
			CDPoint xyEnd	= pRC->m_xyEnd;
			if(i == nSize-1)
			{
				if(!GetXyMatchLineAndLine(pRC->m_xyStt, (pRC->m_xyMid1-pRC->m_xyStt).Unit(), pRC->m_xyEnd, (pRC->m_xyMid2-pRC->m_xyEnd).Unit(), xyEnd))
					xyEnd	= pRC->m_xyMid2;
			}
			xyEnd			= xyEnd + (-xyDir*dLengthJointEnd);

			if(!ComparePt(xyLast, xyStt, "="))
			{
				xyArrDim.Add(xyStt);
				sArrInfo.Add("0");
			}	

			if(i == 0)
			{
				xyArrDim.Add(xyMid2);
				sArrInfo.Add("2");

				xyArrDim.Add(xyEnd);
				sArrInfo.Add("1");
			}
			else if(i == nSize-1)
			{
				xyArrDim.Add(xyMid1);
				sArrInfo.Add("1");

				xyArrDim.Add(xyEnd);
				sArrInfo.Add("2");
			}

			xyLast = xyEnd + (xyDir * dLengthJointEnd);
		}
		// ������ ����
		else 
		{
			CDPoint xyStt	= pRC->m_xyStt + (xyDir*dLengthJointStt);
			CDPoint xyMid1	= pRC->m_xyMid1 + (-xyDir*dLengthJointEnd);
			if(!ComparePt(xyLast, xyStt, "="))
			{
				xyArrDim.Add(xyStt);
				sArrInfo.Add("0");
			}

			xyArrDim.Add(xyMid1);
			sArrInfo.Add("1");


			xyLast = xyMid1 + (xyDir * dLengthJointEnd);
		}
	}


	// reverse�̸� 1�� 0�� �ڹٲ�.
	if(bReverse)
	{
		nSize	= sArrInfo.GetSize();
		for(i = 0; i < nSize; i++)
		{
			if(sArrInfo.GetAt(i) == "0") sArrInfo.SetAt(i, "1");
			else if(sArrInfo.GetAt(i) == "1") sArrInfo.SetAt(i, "0");
		}
	}


	// �������� �ִ� "1"�� ������ "2"�� �Ǿ�� �Ѵ�.(������ ���̰�..)
	if(bApplyEndIs2)
	{
		nSize = sArrInfo.GetSize();
		for(i = nSize-1; i >= 0; i--)
		{
			if(sArrInfo.GetAt(i) == "1")
			{
				sArrInfo.SetAt(i, "2");
				break;
			}
		}
	}

	// �Ǻ� ���� �߰��Ѵ�.
	// ó���� ���κп� �Ǻ� ���� �߰��Ѵ�. ////////////////
	if(xyArrDim.GetSize() > 1)
	{
		CDPoint xy1	= xyArrDim.GetAt(0);
		CDPoint xy2	= xyArrDim.GetAt(xyArrDim.GetSize()-1);

		xy1 = xy1 + (xy1-xy2).Unit() * dCoverStt;
		xyArrDim.InsertAt(0, xy1);
		sArrInfo.InsertAt(0, "0");

		xy2	= xy2 + (xy2-xy1).Unit() * dCoverEnd;
		xyArrDim.Add(xy2);
		sArrInfo.Add("0");
	}
	////////////////////////////////////////////////////////

	AhTPADelete(&pArrRCTmp, (CRebarInfoCycle*)0);
}

// ���� �׸���.
// long nDrawLeft : -1 �������� ������ �׸�, 0 ������ϰ� �� �׸�, 1 �������� ������ �׸�
// double dStaCuttingOrg : ���� sta
void CARcBridgeDBStandard::DrawJoint(CDomyun *pDomP, CRebarJoint &joint, long nDrawLeft, CDRect rectLimit)
{
	CDPoint xyStt	= joint.m_xyPos + (-joint.m_xyDir * joint.m_dLength/2);
	CDPoint xyEnd	= xyStt + (joint.m_xyDir * joint.m_dLength);

	if(joint.m_bDrawUpper)
		Offset(xyStt, xyEnd, GAP_JOINT);
	else
		Offset(xyStt, xyEnd, -GAP_JOINT);

	CTwinVectorArray tvArr;
	CTwinVector tv;

	if(nDrawLeft == 0)
	{
		tv.m_v1	= xyStt;
		tv.m_v2	= xyEnd;
		tvArr.Add(tv);
	}
	else
	{
		double dStaCuttingOrg	= GetStaCuttingOrg();	// ������ ã��.

		CRebarPlacing rb;
		CDPoint xyDir	= (xyEnd-xyStt).Unit();
		// ���ʸ� �׸��� ���
		if(nDrawLeft == -1)
		{
			if(rb.IsParallel(xyDir, CDPoint(0, 1)))
			{
				if(xyStt.x > dStaCuttingOrg)
				{
					xyStt.x = dStaCuttingOrg;
					xyEnd	= xyStt;
				}
			}
			else
			{
				if(xyStt.x > dStaCuttingOrg)
					GetXyMatchLineAndLine(xyStt, xyDir, CDPoint(dStaCuttingOrg, 0), CDPoint(0, 1), xyStt);
				if(xyEnd.x > dStaCuttingOrg)
					GetXyMatchLineAndLine(xyEnd, xyDir, CDPoint(dStaCuttingOrg, 0), CDPoint(0, 1), xyEnd);
			}
		}
		// �����ʸ� �׸��� ���.
		else if(nDrawLeft == 1)
		{
			if(rb.IsParallel(xyDir, CDPoint(0, 1)))
			{
				if(xyStt.x < dStaCuttingOrg)
				{
					xyStt.x = dStaCuttingOrg;
					xyEnd	= xyStt;
				}
			}
			else
			{
				if(xyStt.x < dStaCuttingOrg)
					GetXyMatchLineAndLine(xyStt, xyDir, CDPoint(dStaCuttingOrg, 0), CDPoint(0, 1), xyStt);
				if(xyEnd.x < dStaCuttingOrg)
					GetXyMatchLineAndLine(xyEnd, xyDir, CDPoint(dStaCuttingOrg, 0), CDPoint(0, 1), xyEnd);
			}
		}

		if(!ComparePt(xyStt, xyEnd, "="))
		{
			tv.m_v1	= xyStt;
			tv.m_v2	= xyEnd;
			tvArr.Add(tv);
		}
	}

	if(rectLimit != CDRect(0, 0, 0, 0))
	{
		CTwinVectorArray tvArrLimit;
		tvArrLimit.SetFromRect(rectLimit);
		
		// ���� ���ʸ� ����.
		CRebarPlacing rb;
		CVectorArray vecArrArea;
		tvArrLimit.GetToVectorArray(vecArrArea);
		rb.GetTvArrayByCrossVectorArrayArea(tvArr, vecArrArea, TRUE);
	}

	CDomyun Dom;
	m_pARoadOptionStd->SetEnvType(&Dom, m_bDrawRebarInfoCycleByHidden ? HCAD_BARR : HCAD_BARC);
	DrawTvArray(&Dom, tvArr);
	*pDomP << Dom;
}

// ���� �׸���
// void CARcBridgeDBStandard::DrawJointByRebarInfoCycleArray(CDomyun *pDomP, CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC)
// {
// 	if(!pArrRC) return;
// 	CJointArray arrJoint;
// 
// 	// rebarinfocycle���� ���� ���� �м�
// 	m_pRcBridgeRebar->m_Util.GetJointAtRebarInfoCycleArray(arrJoint, pArrRC);
// 
// 	// ���� �׸���
// 	DrawJointArray(pDomP, arrJoint);
// }

// rebarinfocycle �迭 �׷����..
void CARcBridgeDBStandard::DrawRebarInfoCycleArray(CDomyun *pDomP, CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC, long nDrawLeft, CDRect rectLimit, BOOL bDecreaseSttRebarInFooting, CParabola *pPb)
{
	if(!pArrRC) return;
	long nSize = pArrRC->GetSize();	
	long i = 0; for(i = 0; i < nSize; i++)
	{
		CRebarInfoCycle *pRC = pArrRC->GetAt(i);
		if(pRC)
		{
			DrawRebarInfoCycle(pDomP, pRC, nDrawLeft, rectLimit, bDecreaseSttRebarInFooting, pPb);
		}
	}
}

void CARcBridgeDBStandard::DrawJointArray(CDomyun *pDomP, CJointArray &arrJoint, long nDrawLeft, CDRect rectLimit)
{
	// ã���� ���� ������ �ϳ��� �׸�.
	long nSize	= arrJoint.GetSize();
	long i = 0; for(i = 0; i < nSize; i++)
	{
		DrawJoint(pDomP, arrJoint.GetAt(i), nDrawLeft, rectLimit);
	}
}

// rebarinfocycle ö�� ��ŷ�ϱ�.
// ��ŷ ������ ã�Ƽ� dan�� ���� ��ŷ�Ѵ�.
// 1�� : ����������� ����������
// 2�� : ������������ ��������
// 3�� : �������������� ����������
// bUpperAndLeft : ���γ� �������� ��ŷ����?
// bSide : ���� ��ŷ����?
// long nTypeStt : ������ ö���� ���۰� ���� �ٸ� Ÿ���϶��� ��ŷŸ��
// long nTypeEnd : ������ ö���� ���۰� ���� �ٸ� Ÿ���϶��� ��ŷŸ��
// ��ŷŸ�� 0 - ù��° ��, 1 - ��� ��, 2 - ������ ��
void CARcBridgeDBStandard::MarkRebarInfoCycle(CDomyun *pDomP, CRebarInfoCycle *pRC, BOOL bUpperAndLeft, BOOL bSide, double dHeightMark, long nType, long nDrawLeft, CDRect rectLimit, double dScalePos, BOOL bInput, void *pArrRB, CParabola *pPb, CRebarInfoCycle *pRCForMark)
{
	if(!pRC) return;
	if(Compare(pRC->GetLength(), 0.0, "=")) return;
	if(!pRC->m_bExist) return;
	CRcBridgeRebar *pBri	= m_pRcBridgeRebar;
	if(!pBri) return;
	if(m_bDrawRebarInfoCycleByHidden) return;

	// �ִ���� ������ ǥ���ϴ� ����
	CRebar *pRB = NULL;
	double dMaxLength	= pBri->m_pARcBridgeDataStd->m_dLengthRebarMax;

	pRCForMark	= pRCForMark ? pRCForMark : pRC;
	CString sMark	= pRCForMark->m_szMark;
	long nTypeRebar = -1;
	
		if(pArrRB)
		{
			CSafetyTypedPtrArray <CObArray, CRebar*> *pArrRBTmp	= (CSafetyTypedPtrArray <CObArray, CRebar*>*)pArrRB;
			long i = 0; for(i = 0; i < pArrRBTmp->GetSize(); i++)
			{
				pRB	= pArrRBTmp->GetAt(i);
				if(pRB->m_sDescription == pRCForMark->m_sDescription)
				{
					if(!bInput)
					{
						sMark	= pRB->m_sMark;
					}
					nTypeRebar = pRB->m_nTypeRebar;
					break;
				}
			}
		}
	
	if(m_sMarkForMarkRebar != "")
	{
		if(m_sMarkForMarkRebar != sMark) return;
	}

	if(!Compare(dHeightMark, 0.0, "=")) dHeightMark += pDomP->Always(MARK_BASE_HEIGHT);
	
	CDomyun Dom(pDomP);
	m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

	EPartOfBridge ePart = ePartUpperSlab;
	if(pRC->m_nPosEnd == REBARINFO_POS_STTWALL_OUTTER || (pRC->m_nPosStt == REBARINFO_POS_STTWALL_OUTTER && pRC->m_nPosEnd == REBARINFO_POS_FOOTING_LOWER) || pRC->m_nPosStt == REBARINFO_POS_STTWALL_INNER)
		ePart = ePartOutWallStt;
	else if(pRC->m_nPosEnd == REBARINFO_POS_ENDWALL_INNER || ((pRC->m_nPosStt == REBARINFO_POS_ENDWALL_OUTTER || pRC->m_nPosStt == REBARINFO_POS_FOOTING_LOWER) && pRC->m_nPosEnd == REBARINFO_POS_ENDWALL_OUTTER))
		ePart = ePartOutWallEnd;
	else if(pRC->m_nPosStt == REBARINFO_POS_INWALL_LEFT || pRC->m_nPosStt == REBARINFO_POS_INWALL_RIGHT)
		ePart = ePartInWall;
	else if(pRC->m_nPosStt == REBARINFO_POS_LOWERSLAB_LOWER || pRC->m_nPosEnd == REBARINFO_POS_LOWERSLAB_LOWER || pRC->m_nPosStt == REBARINFO_POS_LOWERSLAB_UPPER || pRC->m_nPosStt ==REBARINFO_POS_LOWERSLAB_SIDE)
		ePart = ePartLowerSlab;

	CConcSymbol sym(m_pARoadOptionStd);
	CString sDia	= m_pARcBridgeDataStd->GetStringDia(pRC->m_Dia, pBri->GetValueFy(ePart));
	long nDan		= pRC->m_nRebarDan;
	CVector vecStt	= CVector(0, 0, 0);
	CVector vecEnd	= CVector(0, 0, 0);
	CDPoint xyMark	= CDPoint(0, 0);
	BOOL bLeft		= TRUE;
	double dStaCuttingOrg	= GetStaCuttingOrg();

	// tvarr�� ����� ��� ��ŷ�� ��ġ�� vecStt�� vecEnd�� �־��ش�.
	if(pRC->m_bUseTvArrRebar)
	{
		long nSize	= pRC->m_tvArrRebar.GetSize();
		if(nSize > 0)
		{
			CTwinVector tv	= pRC->m_tvArrRebar.GetAt((nSize-1)/2);
			vecStt			= tv.m_v1;
			vecEnd			= tv.m_v2;
		}
	}
	else
	{
		// ��ŷ�� �� ã��
		if(pRC->m_nPosStt == pRC->m_nPosEnd)
		{
			vecStt	= pRC->m_xyStt;
			vecEnd	= pRC->m_xyMid1;
		}
		else
		{
			if(nType == 0)
			{
				vecStt	= pRC->m_xyStt;
				vecEnd	= pRC->m_xyMid1;
			}
			else if(nType == 1)
			{
				if(pRC->m_bUseEndExt)
				{
					vecStt	= pRC->m_xyMid2;
					vecEnd	= pRC->m_xyEnd;
				}
				else
				{
					vecStt	= pRC->m_xyMid1;
					vecEnd	= pRC->m_xyMid2;
				}
			}
			else if(nType == 2)
			{
				if(pRC->m_bUseEndExt)
				{
					vecStt	= pRC->m_xyMidExt;
					vecEnd	= pRC->m_xyEndExt;
				}
				else
				{
					vecStt	= pRC->m_xyMid2;
					vecEnd	= pRC->m_xyEnd;
				}
			}
		}
	}

	// ��ŷ�� ���� ã��
	CDPoint xyStt	= vecStt;
	CDPoint xyEnd	= vecEnd;
	double xDisStt	= 0;
// 	double xDisEnd	= 0;
	double dLenPb	= 0;
	if(pPb)
	{
		xDisStt	= pPb->GetDistByXPos(xyStt.x);
		double xDisEnd	= pPb->GetDistByXPos(xyEnd.x);
		dLenPb	= pPb->GetLenByDistToDist(xDisStt, xDisEnd);
	}

	long nDivide	= 5;
	if(nDan == 0)
	{
		if(pPb)
		{
			xyMark	= pPb->GetXyByDist(pPb->GetDistByDistToLen(xDisStt, dLenPb*dScalePos));
		}
		else
		{
			double dDist	= ~(xyStt-xyEnd);
			CDPoint xyDir	= (xyEnd-xyStt).Unit();

			xyMark	= xyStt+xyDir*dDist*dScalePos;
		}
		bLeft	= FALSE;
	}
	else if(nDan == 1)
	{
		if(pPb)
		{
			double dScaleStt	= 1-((1-dScalePos)*2);
			double dLenMove		= (dLenPb*dScaleStt)+(dLenPb*dScalePos/nDivide*nDan+1);

			xyMark	= pPb->GetXyByDist(pPb->GetDistByDistToLen(xDisStt, dLenMove));
		}
		else
		{
			double dDist	= ~(xyStt-xyEnd);
			CDPoint xyDir	= (xyEnd-xyStt).Unit();

			double dScaleStt	= 1-((1-dScalePos)*2);
			xyMark			= (xyStt+xyDir*dDist*dScaleStt) + (xyDir * dDist*dScalePos/nDivide * nDan+1);
		}
		bLeft			= TRUE;
	}
	else if(nDan == 2)
	{
		if(pPb)
		{
			double dScaleStt	= 1-((1-dScalePos)*2);
			double dLenMove		= (dLenPb*dScaleStt) + (dLenPb*dScalePos/nDivide * (nDivide-nDan+1));

			xyMark	= pPb->GetXyByDist(pPb->GetDistByDistToLen(xDisStt, dLenMove));
		}
		else
		{
			double dDist	= ~(xyStt-xyEnd);
			CDPoint xyDir	= (xyEnd-xyStt).Unit();

			double dScaleStt	= 1-((1-dScalePos)*2);
			xyMark			= (xyStt+xyDir*dDist*dScaleStt) + (xyDir * dDist*dScalePos/nDivide * (nDivide-nDan+1));
		}
		bLeft			= FALSE;
	}

	if(rectLimit != CDRect(0, 0, 0, 0))
	{
		CTwinVectorArray tvArrLimit;
		tvArrLimit.SetFromRect(rectLimit);

		CVectorArray vecArrLimit;
		tvArrLimit.GetToVectorArray(vecArrLimit);

		CRebarPlacing rb;
		if(!rb.CheckXyByVectorArrayArea(xyMark, vecArrLimit, TRUE)) return;
	}

	
	if(nDrawLeft != 0)
	{
		// ���ʸ� �׸�
		if(nDrawLeft == -1)
		{
			if(xyMark.x > dStaCuttingOrg) return;
		}
		// �����ʸ� �׸�
		else if(nDrawLeft == 1)
		{
			if(xyMark.x < dStaCuttingOrg) return;
		}
	}

	// ���������� ���ؾ� �Ǵ� ��ġ�� ���ؼ� ���⼭ ������ �� �� �ִ�. ////////////////////////
	// 1. ������ü ����ġ���� ��ġ�� ����ϸ� ��������.
	// ��, ��ν����� ö�ٸ� ö�ٸ� �����ϵ� ��ġ�� ö���� �������
	if(pRC->m_nPosStt == REBARINFO_POS_UPPERSLAB_LOWER || pRC->m_nPosEnd == REBARINFO_POS_UPPERSLAB_LOWER || 
		pRC->m_nPosStt == REBARINFO_POS_UPPERSLAB_UPPER || pRC->m_nPosEnd == REBARINFO_POS_UPPERSLAB_UPPER)
	{
		// tv�� �̿��Ѱ��� ��ġ�� ö���̴�. �������� ����
		if(!pRC->m_bUseTvArrRebar)
		{
			double dRange	= Dom.GetTextWidth("000000");
			double dOffset	= Dom.Always(Dom.GetDimLevelDis()*2 + Dom.GetDimVLen() + Dom.GetDimExo());
			long stt = 0; for(stt = 0; stt < 2; stt++)
			{
				BOOL bStt	= stt == iSTT;
				if(!pBri->IsOutWall(bStt)) continue;

				CTwinVector tvWall	= pBri->m_tvArrVertSection.GetTvByInfo(bStt ? "������ü������" : "������ü������");
				tvWall.m_v1.x	+= bStt ? dOffset : -dOffset;
				if(tvWall.m_v1.x + dRange > xyMark.x && tvWall.m_v1.x - dRange < xyMark.x)
				{
					// ������ �ɸ��� �������� �ɸ��Ÿ� ���������� �ƴϸ� �ݴ�� �̵�
					double dDist		= bStt ? fabs(xyMark.x - tvWall.m_v1.x) : fabs(xyMark.x - tvWall.m_v1.x) + dRange;
					CDPoint xyDirMove	= bStt ? (xyEnd-xyStt).Unit() : (xyStt-xyEnd).Unit();
					xyMark	= xyMark + xyDirMove * (dRange/2 + dDist);
					if(bStt && xyMark.x > xyEnd.x) xyMark	= xyEnd;
					if(!bStt && xyMark.x < xyStt.x) xyMark	= xyStt;
				}
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////

	// ��ũ�� ��� ��ũ�� �°� ������ ã�� ��� ��. /////////////
	if(vecStt.z != 0)
	{
		CRebarPlacing rb;
		CDPointArray xyArrMatch;
		rb.GetXyMatchLineAndArc2(xyMark, CDPoint(0, 1), vecStt, vecEnd, vecEnd.z, vecStt.z==-1, TRUE, xyArrMatch, TRUE);
		if(xyArrMatch.GetSize() > 0)
			xyMark	= xyArrMatch.GetAt(0);
	}
	/////////////////////////////////////////////////////////////

	// ���� ����
	double dLen	= pPb ? dLenPb : pRC->GetLength();

	BOOL bDrawLengthInfo	= IsDrawingLengthInfo();
	SRebarMarkInputData mark_input_data;
	SRebarMarkviewData mark_view_data;

	if(bSide)
	{
		mark_view_data.mark_pos = eMarkSide;
		mark_input_data.left_dir = bUpperAndLeft;
// 		if(bUpperAndLeft)
// 		{
// 			if(bDrawLengthInfo)
// 				sym.RebarMarkLineSideEx(&Dom, xyMark, sMark, sDia, dLen, 0, TRUE, dHeightMark, TRUE, "", m_bCheckOverLengthRebar ? dMaxLength < dLen : FALSE);
// 			else
// 				sym.RebarMarkLineSide(&Dom, xyMark, sMark, sDia, TRUE, dHeightMark);
// 		}
// 		else
// 		{
// 			if(bDrawLengthInfo)
// 				sym.RebarMarkLineSideEx(&Dom, xyMark, sMark, sDia, dLen, 0, FALSE, dHeightMark, TRUE, "", m_bCheckOverLengthRebar ? dMaxLength < dLen : FALSE);
// 			else
// 				sym.RebarMarkLineSide(&Dom, xyMark, sMark, sDia, FALSE, dHeightMark);
// 		}
	}
	else
	{
		mark_view_data.mark_pos = bUpperAndLeft? eMarkUpper : eMarkLower;
		mark_input_data.left_dir = bLeft;

// 		if(bUpperAndLeft)
// 		{
// 			if(bDrawLengthInfo)
// 				sym.RebarMarkLineUpperEx(&Dom, xyMark, sMark, sDia, dLen, 0, bLeft, dHeightMark, TRUE, FALSE, "", m_bCheckOverLengthRebar ? dMaxLength < dLen : FALSE);
// 			else
// 				sym.RebarMarkLineUpper(&Dom, xyMark, sMark, sDia, bLeft, dHeightMark);
// 		}
// 		else
// 		{
// 			dHeightMark += Dom.Always(MARK_BASE_RADIUS*1.5);
// 			if(bDrawLengthInfo)
// 				sym.RebarMarkLineLowerEx(&Dom, xyMark, sMark, sDia, dLen, 0, bLeft, dHeightMark, TRUE, FALSE, "", m_bCheckOverLengthRebar ? dMaxLength < dLen : FALSE);
// 			else
// 				sym.RebarMarkLineLower(&Dom, xyMark, sMark, sDia, bLeft, dHeightMark);
// 		}
	}

	mark_input_data.mark = sMark;
	mark_input_data.dia = sDia;
	mark_input_data.position = xyMark;
	mark_input_data.ctc = pBri->m_dCTC_MainRebar;
	mark_input_data.type_rebar = nTypeRebar;
	mark_input_data.rebar_length = bDrawLengthInfo? dLen : 0;
	mark_input_data.height	= dHeightMark;

	mark_view_data.apply_basic_height = TRUE;
	mark_view_data.count_mark = TRUE;
	mark_view_data.line_rebar = TRUE;
	mark_view_data.check = (bDrawLengthInfo && m_bCheckOverLengthRebar) ? dMaxLength < dLen : FALSE;

	sym.RebarMarkST(&Dom, mark_input_data, mark_view_data);

	if(pPb)
	{
		CDPoint vAngTan	= pPb->GetXyDirTanByDist(pPb->GetDistByXPos(xyMark.x));
		Dom.Rotate(xyMark, vAngTan);
	}
	*pDomP << Dom;
}

// rebarinfocycle ö�� ��ŷ�ϱ�.
// ��ŷ ������ ã�Ƽ� dan�� ���� ��ŷ�Ѵ�.
// 1�� : ����������� ����������
// 2�� : ������������ ��������
// 3�� : �������������� ����������
// bUpperAndLeft : ���γ� �������� ��ŷ����?
// bSide : ���� ��ŷ����?
// long nTypeStt : ������ ö���� ���۰� ���� �ٸ� Ÿ���϶��� ��ŷŸ��
// long nTypeEnd : ������ ö���� ���۰� ���� �ٸ� Ÿ���϶��� ��ŷŸ��
// ��ŷŸ�� 0 - ù��° ��, 1 - ��� ��, 2 - ������ ��
void CARcBridgeDBStandard::MarkRebarInfoCycleArray(CDomyun *pDomP, CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC, BOOL bUpperAndLeft, BOOL bSide, double dHeightMark, long nTypeStt, long nTypeEnd, long nDrawLeft, CDRect rectLimit, double dScalePos, BOOL bInput, void *pArrRB, CParabola *pPb, BOOL bFindOwner)
{
	if(!pArrRC) return;
	CRcBridgeRebar *pBri	= m_pRcBridgeRebar;
	if(!pBri) return;

	long nSize		= pArrRC->GetSize();
	long nType		= 2;
	
	long i = 0; for(i = 0; i < nSize; i++)
	{
		CRebarInfoCycle *pRC	= pArrRC->GetAt(i);
		CRebarInfoCycle *pRCForMark	= bFindOwner ? pBri->GetOwnerRebarInfoCycle(pArrRC, i) : pRC;

		if(i == 0) nType = nTypeStt;
		else if(i == nSize-1) nType = nTypeEnd;
		else nType = 2;

		// ������ ���鿡�� ������� ��뼱���� ö�ٸ�ũ �ؾ� ��
		if(pRC->m_nPosStt == REBARINFO_POS_UPPERSLAB_SIDE)
			nType = 2;
		else if(pRC->m_nPosEnd == REBARINFO_POS_UPPERSLAB_SIDE)
			nType = 0;

		// ��ν����� ��ü���϶� �������� ��ŷ�Ѵ�.
		if(pBri->IsMainRebarJointUpperSlabAndUpperSlab(pRC))
		{
			nType	= 1;
		}

		MarkRebarInfoCycle(pDomP, pRC, bUpperAndLeft, bSide, dHeightMark, nType, nDrawLeft, rectLimit, dScalePos, bInput, pArrRB, pPb, pRCForMark);
	}
}

// ���� ���� �۷��� �Ǵ� ������...
BOOL CARcBridgeDBStandard::IsDrawByLineInfo(long nPosInRebarInfoCycle)
{
	BOOL bLine	= FALSE;
	if(nPosInRebarInfoCycle == REBARINFO_POS_UPPERSLAB_UPPER || 
		nPosInRebarInfoCycle == REBARINFO_POS_UPPERSLAB_LOWER || 
		nPosInRebarInfoCycle == REBARINFO_POS_LOWERSLAB_UPPER || 
		nPosInRebarInfoCycle == REBARINFO_POS_LOWERSLAB_LOWER)
	{
		bLine	= TRUE;
	}

	return bLine;
}

// ��ö�� �׸���(���̾� ������ ���� �� ��� ��)
void CARcBridgeDBStandard::DrawPointRebar(CDomyun *pDomP, CDPointArray &xyArr)
{
	long nSize	= xyArr.GetSize();
	long i = 0; for(i = 0; i < nSize; i++)
		CHgBaseDrawStd::InsertHCadBlock(pDomP, HCAD_E05, xyArr.GetAt(i), pDomP->GetScaleDim());
}

// PlaceRebarByBlock���� ġ�����Կ� ������ ���´�. ����
void CARcBridgeDBStandard::GetDimInfoAtPlaceRebarByBlock(CDoubleArray &dArrDim, CStringArray &sArrText, CStringArray &sArrUnderText, CPlaceRebarByBlock *pPlace, CString sMark, BOOL bInput, BOOL bMerge, double dCurAngle, double dApplyAngle)
{
	if(!bMerge)
	{
		dArrDim.RemoveAll();
		sArrText.RemoveAll();
		sArrUnderText.RemoveAll();
	}

	if(!pPlace) return;

	CRebarPlacing rb;
	CDPointArray xyArrOffsetInfo;
	xyArrOffsetInfo	= pPlace->m_xyArr;
	if(dCurAngle != dApplyAngle)
	{
		rb.ApplyAngleToOffsetInfo(xyArrOffsetInfo, dCurAngle, dApplyAngle);
	}

	long nSize	= xyArrOffsetInfo.GetSize();
	CDPoint xyR(0, 0);
	CString sNum	= _T("");
	for(long block = 0; block < pPlace->m_nCountBlock; block++)
	{
		if(nSize-1 < block) break;

		xyR	= xyArrOffsetInfo.GetAt(block);
		
		dArrDim.Add(xyR.x*xyR.y);
		if(bInput)
		{
			sNum.Format("%s%d", sMark, block+1);
			sArrText.Add(sNum);
			if(xyR.x > 1)
				sNum.Format("%d@%s=%s", (long)xyR.x, COMMA(xyR.y), COMMA(xyR.x*xyR.y));
			else
				sNum.Format("%s", COMMA(xyR.y));
			sArrUnderText.Add(sNum);
		}
		else
		{
			if(xyR.x > 1)
				sNum.Format("%d@%s=%s", (long)xyR.x, COMMA(xyR.y), COMMA(xyR.x*xyR.y));
			else
				sNum.Format("%s", COMMA(xyR.y));
			sArrText.Add(sNum);
			sArrUnderText.Add("0");
		}
	}
}

// PlaceRebarByBlock���� ġ�����Կ� ������ ���´�. ����
void CARcBridgeDBStandard::GetDimInfoAtPlaceRebarByBlock(CDPointArray &xyArr, CDPointArray &xyArrDim, CStringArray &sArrText, CStringArray &sArrUnderText, CPlaceRebarByBlock *pPlace, CString sMark, CDPoint xyStt, CDPoint xyDir, BOOL bInput, BOOL bMerge, double dCurAngle, double dApplyAngle)
{
	if(!bMerge)
	{
		xyArrDim.RemoveAll();
		sArrText.RemoveAll();
		sArrUnderText.RemoveAll();
	}

	if(!pPlace) return;

	CRebarPlacing rb;
	CDPointArray xyArrOffsetInfo;
	xyArrOffsetInfo	= pPlace->m_xyArr;
	if(dCurAngle != dApplyAngle)
	{
		rb.ApplyAngleToOffsetInfo(xyArrOffsetInfo, dCurAngle, dApplyAngle);
	}

	long nSize	= xyArrOffsetInfo.GetSize();
	if(nSize < 1) return;

	xyArrDim.Add(xyStt);
	sArrText.Add("0");
	sArrUnderText.Add("0");

	CDPoint xyR(0, 0);
	CString sNum	= _T("");
	for(long block = 0; block < pPlace->m_nCountBlock; block++)
	{
		if(nSize-1 < block) break;
		xyR	= xyArrOffsetInfo.GetAt(block);

		xyStt	= xyStt + (xyDir * (xyR.x*xyR.y));
		xyArrDim.Add(xyStt);

		if(bInput)
		{
			sNum.Format("%s%d", sMark, block+1);
			sArrText.Add(sNum);
			if(xyR.x > 1)
				sNum.Format("%d@%s=%s", (long)xyR.x, COMMA(xyR.y), COMMA(xyR.x*xyR.y));
			else
				sNum.Format("%s", COMMA(xyR.y));
			sArrUnderText.Add(sNum);
		}
		else
		{
			if(xyR.x > 1)
				sNum.Format("%d@%s=%s", (long)xyR.x, COMMA(xyR.y), COMMA(xyR.x*xyR.y));
			else
				sNum.Format("%s", COMMA(xyR.y));
			sArrText.Add(sNum);
			sArrUnderText.Add("0");
		}
	}
}


// �ɼǿ� ���� EL���� �������ش�.
CString CARcBridgeDBStandard::GetStringEL(double dEL)
{
	CARcBridgeDoc* pDoc = dynamic_cast<CARcBridgeDoc*>(m_pARcBridgeDataStd->m_pDoc);
	CARcBridgeDrawingStd* pStd = pDoc->m_pARcBridgeDrawingStd;
	CARcBridgeDrawingOptionStd* pOpt = pDoc->m_pARcBridgeDrawingOptionStd;

	// nTypeEL -> 0 : EL. 15.000, 1 : EL.:15.000, 2 : EL.=15.000
	long nTypeEL	= pOpt->m_nTypeEL;	
	CString sEL	= _T("");
	switch(nTypeEL)
	{
		case 0: sEL.Format("EL. %.3f", dEL); break;
		case 1: sEL.Format("EL.:%.3f", dEL); break;
		case 2: sEL.Format("EL.=%.3f", dEL); break;
		default: sEL.Format("EL. %.3f", dEL); break;
	}

	return sEL;
}

// RebarInfoCycle �迭�� ġ�� �ֱ�(��ö�� ������ ��)
// bLeft : ��������� ������ ġ�� ������ ������ ġ�� ������... 
// long nDrawLeft : -1 ������ �������� ġ�� ����, 0 ������ ��� ����, 1 : ������ �������� ġ�� ����.
// �������δ� �Ʒ����� �����ص� ������, �������δ� �ݵ�� �¿��� ��θ� �����ؾ� ����� ġ���� ���Եȴ�.
// BOOL bAliDim : ��米���� ������ü ġ�����Կ� bAli�ʹ� ���еǾ�� ��.
// BOOL bModifyStt : ������ġ�� ��������?(��纮ü�� �����ϸ� �ȵ�, ��Ȯ�ϰ԰���ؼ� ������ġ�� ���� �ֱ� ����)
// BOOL bCopplingSttEnd : ��ü�� ����Ǿ��ִ� ����. CDPoint xyStt, CDPoint xyEnd�� �����ϰ� �ǳ�ö�ٰ� ó��ö���� �����Ѵ�.
void CARcBridgeDBStandard::DimRebarInfoCycleArray(CDomyun *pDomP, CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC, CDPoint xyStt, CDPoint xyEnd, BOOL bLeft, long nDimDan, long nDrawLeft, BOOL bAliDim, BOOL bModifyStt, BOOL bCopplingSttEnd, BOOL bArcReverse)
{
	CRcBridgeRebar *pBri	= m_pRcBridgeRebar;
	if(!pArrRC) return;

	// ���� ġ�� �������� bExist�� FALSE �ΰ��� ������ ���Ÿ� ���ش�.
	pBri->m_Util.RemoveNotExistRebarInfoCycle(pArrRC);

	if(pArrRC->GetSize() == 0) return;

	CJointArray arrJoint;
	if(bCopplingSttEnd)
	{
		// ������ ���鶧�� �Ǿ�ö���� ���� �ϳ� �� �־��ٰ� ������.
		CRebarInfoCycle *pRc = pArrRC->GetAt(0);
		pArrRC->Add(pRc);
	}
	// rebarinfocycle���� ���� ���� �м�
	m_pRcBridgeRebar->m_Util.GetJointAtRebarInfoCycleArray(arrJoint, pArrRC);
	if(bCopplingSttEnd)
	{
		pArrRC->RemoveAt(pArrRC->GetSize()-1);
	}

	CDomyun Dom(pDomP);
	
	// �������� ������ �޾Ƽ� ġ���� ���ԵǴ� ����� ���� �� �մ�
	// -1 : �������� ����, 0 : ���� ����, 1 : �߷��� ���Ե�.(arc������ �߷��� ���ԵǴ°� ��� �ȵ�)
	long nTypeDim			= 0;	
	double dStaCuttingOrg	= GetStaCuttingOrg();
	BOOL bHor				= FALSE;
	double dLen				= 0;	// ali���϶� �� ��� �׷���� �Ǳ� ������ ����
	double dLenTxt			= 0;	// ġ�����Խ� ��µ� ġ������
	CDPoint xyLast			= CDPoint(0, 0);
	CRebarInfoCycle *pRC	= NULL;

	long nSize				= pArrRC->GetSize();
	CDRect rect				= pBri->m_tvArrVertSection.GetRect();
	double dHDiff			= pBri->GetXySectionSttEnd(TRUE).y - pBri->GetXySectionSttEnd(FALSE).y;
	CDPoint vAng			= (pBri->GetXySectionSttEnd(FALSE) - pBri->GetXySectionSttEnd(TRUE)).Unit();
	BOOL bUpperSlabLower	= FALSE;
	double dYOrg			= 0;
	BOOL bAli				= FALSE;


	CDPoint vAngRot	= CDPoint(1, 0);
	if(nSize > 0)
	{
		CRebarInfoCycle *pRC	= pArrRC->GetAt(0);
		vAngRot	= ToDPointFrDegree((pRC->m_xyMid1-pRC->m_xyStt).GetAngleDegree()-90);
	}


	for(long rc = 0; rc < nSize; rc++)
	{
		pRC	= pArrRC->GetAt(rc);

		// �ǰŸ� ġ�� ������ �ƴѰ��..
		// ��ν����� ġ�� ���Խô� ġ������ Y��ǥ�� ���� ���� ���� �������� ���� ��Ų��.
		if(pRC->m_nPosStt == REBARINFO_POS_UPPERSLAB_UPPER || pRC->m_nPosEnd == REBARINFO_POS_UPPERSLAB_UPPER)
		{
			bUpperSlabLower	= FALSE;
			bAli	= TRUE;
		}
		else if(pRC->m_nPosStt == REBARINFO_POS_UPPERSLAB_LOWER || pRC->m_nPosEnd == REBARINFO_POS_UPPERSLAB_LOWER)
		{
			bUpperSlabLower	= TRUE;
			dYOrg	= rect.top - pBri->m_dTS + pBri->m_dCoverLower_UpperSlab[0] + dHDiff;
			bAli	= FALSE;
		}
		else if(pRC->m_nPosStt == REBARINFO_POS_LOWERSLAB_UPPER || pRC->m_nPosEnd == REBARINFO_POS_LOWERSLAB_UPPER)
		{
			bUpperSlabLower	= TRUE;
			if(pBri->m_bSlopeLowerSlab == FALSE) dHDiff = 0;
			dYOrg	= rect.bottom + pBri->m_dTS_Lower - pBri->m_dCoverUpper_LowerSlab[0] + fabs(dHDiff);
			bAli	= FALSE;
		}
		else if(pRC->m_nPosStt == REBARINFO_POS_LOWERSLAB_LOWER || pRC->m_nPosEnd == REBARINFO_POS_LOWERSLAB_LOWER)
		{
			bUpperSlabLower	= FALSE;
//			vAng = (pBri->m_bSlopeLowerSlab)? vAng*-1 : CDPoint(-1, 0);
			bAli	= TRUE;
		}
		else
		{
			bUpperSlabLower	= FALSE;
			bAli	= FALSE;
		}



		// ���� ��������?(ö���� 2���̻��ΰ��..) /////////////////
		BOOL bApplyRound	= !pBri->m_bBendingDanBuRebar;
		
		// ���ʿ��� ������ ö���� ������ ���� ���� ����.
		if(pRC->m_nPosEnd == REBARINFO_POS_FOOTING_UPPER || pRC->m_nPosEnd == REBARINFO_POS_FOOTING_LOWER ||
			pRC->m_nPosEnd == REBARINFO_POS_FOOTING_LEFT || pRC->m_nPosEnd == REBARINFO_POS_FOOTING_RIGHT)
			bApplyRound	= FALSE;
		/////////////////////////////////////////////////////////////

		bApplyRound	= TRUE;
		nTypeDim	= 0;

		if(rc == 0 && bCopplingSttEnd == FALSE)
		{
			bHor		= IsDrawByLineInfo(pRC->m_nPosStt) ? TRUE : FALSE;
			nTypeDim	= GetTypeDimRebarInfoCycle(bHor, xyStt, pRC->m_xyStt, nDrawLeft, dStaCuttingOrg);
			GetDimInfoByTypeDim(xyStt, pRC->m_xyStt, bHor, nTypeDim, nDrawLeft, dStaCuttingOrg);
			if(bAliDim)
			{
				dLenTxt	= ~(pRC->m_xyStt - xyStt);
				dLen	= dLenTxt;
			}
			else
			{
				dLenTxt	= (bHor ? pRC->m_xyStt.x - xyStt.x : pRC->m_xyStt.y - xyStt.y);
				dLen	= bAli ? ~(pRC->m_xyStt - xyStt) : dLenTxt;
			}
			

			// ������ü�� ���� ��� ������ ġ���� �����Ҷ��� �Ǻ� ġ���� ���Ƿ� �־��ش�.(���ܻ���) ////
			if(pRC->m_nPosStt == REBARINFO_POS_STTWALL_OUTTER && !pBri->IsOutWall(TRUE))
			{
				Dom.SetDirection("TOP");
				Dom.SetDimObq(bUpperSlabLower ? 0 : vAng.GetAngleDegree());
				Dom.DimMoveTo(xyStt.x, bUpperSlabLower ? dYOrg : xyStt.y, nDimDan, COMMA(pRC->m_xyStt.x-xyStt.x));
				Dom.DimLineTo(bUpperSlabLower ? pRC->m_xyStt.x-xyStt.x : ~(pRC->m_xyStt-xyStt), nDimDan, "");
				if(!bUpperSlabLower)
				{
					Dom.Rotate(xyStt, vAng);
				}
				*pDomP << Dom;

				bHor	= TRUE;
			}
			/////////////////////////////////////////////////////////////////////////////////////

			if(bModifyStt)
			{
				if(bHor)
					xyStt.y	= bUpperSlabLower ? dYOrg : pRC->m_xyStt.y;
				else
					xyStt.x	= pRC->m_xyStt.x;
			}

			Dom.SetDirection(bHor ? (bLeft ? "TOP" : "BOTTOM") : (bLeft ? "LEFT" : "RIGHT"));
			if(dLen > Dom.GetTextWidth(COMMA(fabs(dLenTxt))))
			{
				if(!Compare(dLen, 0.0, "="))
				{
					Dom.SetDimObq(bAli && bHor ? vAng.GetAngleDegree() : 0);
					Dom.DimMoveTo(xyStt.x, xyStt.y, nDimDan);
					Dom.DimLineTo(dLen, nDimDan, COMMA(fabs(dLenTxt)));
					if(bAli && bHor)
						Dom.Rotate(xyStt, vAng);

					if(bAliDim)
						Dom.Rotate(xyStt, vAngRot);
					*pDomP << Dom;

				}
			}
			else
			{
				if(!Compare(dLen, 0.0, "="))
				{
					Dom.SetDimObq(bAli && bHor ? vAng.GetAngleDegree() : 0);
					Dom.DimMoveTo(xyStt.x, xyStt.y, nDimDan, COMMA(fabs(dLenTxt)));
					Dom.DimLineTo(dLen, nDimDan, "");
					if(bAli && bHor)
						Dom.Rotate(xyStt, vAng);

					if(bAliDim)
						Dom.Rotate(xyStt, vAngRot);
					*pDomP << Dom;
				}
			}
			xyLast		= pRC->m_xyStt;
		}	// end of if(rc == 0)
		else if(rc == 0 && bCopplingSttEnd)
		{
			// �� ������ ö�� ��ġ�� �־��ش�.
			CRebarInfoCycle *pRCLast	= pArrRC->GetAt(nSize-1);
			xyLast		= pRCLast->m_xyEnd;
			// �̺κ� �ʿ� ����.
			if(pRC->m_nPosStt != pRC->m_nPosEnd && pRC->m_xyStt.y > xyLast.y)
			{
				// ��ü ���� ������ �ƴ϶�� ���� �κ�(STT ~ MID)�� �־���� �Ѵ�.
				bHor	= IsDrawByLineInfo(pRC->m_nPosStt) ? TRUE : FALSE;
				Dom.SetDirection(bHor ? (bLeft ? "TOP" : "BOTTOM") : (bLeft ? "LEFT" : "RIGHT"));

				if(bHor)
				{
					vAng = (pRC->m_xyStt - xyLast).Unit();
					if(vAng.x < 0) vAng *= -1;
				}

				if(bAliDim)
				{
					dLenTxt	= ~(pRC->m_xyStt-xyLast);
					dLen	= dLenTxt;
				}
				else
				{
					dLenTxt	= bHor ? (pRC->m_xyStt.x-xyLast.x) : (pRC->m_xyStt.y-xyLast.y);
					dLen	= bAli ? ~(pRC->m_xyStt-xyLast) * (dLenTxt < 0 ? -1 : 1) : dLenTxt;
				}
				Dom.SetDirection(bHor ? (bLeft ? "TOP" : "BOTTOM") : (bLeft ? "LEFT" : "RIGHT"));
				if(!Compare(dLen, 0.0, "="))
				{
					Dom.SetDimObq(bAli && bHor ? vAng.GetAngleDegree() * (dLen < 0 ? -1 : 1) : 0);
					if(!bHor && dLen < 0)
					{
						Dom.SetDirection(bLeft ? "RIGHT" : "LEFT");
					}
					if(bHor && dLen < 0)
					{
						Dom.SetDirection(bLeft ? "BOTTOM" : "TOP");
					}

					if(bHor && bUpperSlabLower)
						xyLast.y	= dYOrg;

					Dom.DimMoveTo(xyLast.x, xyLast.y, nDimDan);
					Dom.DimLineTo(dLen, nDimDan, COMMA(fabs(dLenTxt)));
					if(bAli && bHor)
						Dom.Rotate(xyLast, vAng);

					if(bAliDim)
						Dom.Rotate(xyLast, vAngRot);
					*pDomP << Dom;

					// (ARCBRIDGE-3300) ġ���� ������ġ ���� �־����� ������ xyLas�� ���� ��ġ�� �Ǿ���Ѵ�.
					xyLast = pRC->m_xyStt;
				}
			}
		}
		
		if(Compare(pRC->GetLength(), 0.0, "=")) continue;

		if(pRC->m_nPosStt == pRC->m_nPosEnd)
		{
 			// �������� ������ ��ġ�� ������ ���� ��ġ���� �ľ��ؾ� �ȴ�. ///////////
			BOOL bJoint		= FALSE;
			CRebarJoint joint;
			CDPoint xySttJoint	= CDPoint(0, 0);
			CDPoint xyEndJoint	= CDPoint(0, 0);
			long i = 0; for(i = 0; i < arrJoint.GetSize(); i++)
			{
				joint	= arrJoint.GetAt(i);
				xySttJoint	= joint.m_xyPos + (-joint.m_xyDir * joint.m_dLength/2);
				xyEndJoint	= xySttJoint + (joint.m_xyDir * joint.m_dLength);
				
				if(ComparePt(xyEndJoint, pRC->m_xyMid1, "=") || ComparePt(xySttJoint, pRC->m_xyMid1, "="))
				{
					bJoint	= TRUE;
					if(ComparePt(xySttJoint, pRC->m_xyMid1, "=")) GetSwap(xySttJoint, xyEndJoint);
					break;
				}
			}
			////////////////////////////////////////////////////////////////////////////////
			bHor	= IsDrawByLineInfo(pRC->m_nPosStt) ? TRUE : FALSE;
			Dom.SetDirection(bHor ? (bLeft ? "TOP" : "BOTTOM") : (bLeft ? "LEFT" : "RIGHT"));
			if(bJoint)
			{
				// dStaCuttingOrg ����ؼ� ��ǥ ������.
				nTypeDim	= GetTypeDimRebarInfoCycle(bHor, xyLast, xySttJoint, nDrawLeft, dStaCuttingOrg);
				GetDimInfoByTypeDim(xyLast, xySttJoint, bHor, nTypeDim, nDrawLeft, dStaCuttingOrg);

				if(bHor)
				{
					vAng = (xySttJoint - xyLast).Unit();
					if(vAng.x < 0) vAng *= -1;
				}

				if(bAliDim)
				{
					dLenTxt	= ~(xySttJoint - xyLast);
					dLen	= dLenTxt;
				}
				else
				{
					dLenTxt	= bHor ? (xySttJoint.x-xyLast.x) : (xySttJoint.y-xyLast.y);
					dLen	= bAli ? ~(xySttJoint - xyLast) * (dLenTxt < 0 ? -1 : 1) : dLenTxt;
				}
				if(!Compare(dLen, 0.0, "="))
				{
					Dom.SetDimObq(bAli && bHor ? vAng.GetAngleDegree() * (dLen < 0 ? -1 : 1) : 0);
					if(!bHor && dLen < 0)
					{
						Dom.SetDirection(bLeft ? "RIGHT" : "LEFT");
					}
					if(bHor && dLen < 0)
					{
						Dom.SetDirection(bLeft ? "BOTTOM" : "TOP");
					}

					if(bHor && bUpperSlabLower)
						xyLast.y	= dYOrg;
					Dom.DimMoveTo(xyLast.x, xyLast.y, nDimDan);
					Dom.DimLineTo(dLen, nDimDan, COMMA(fabs(dLenTxt)));
					if(bAli && bHor)
						Dom.Rotate(xyLast, vAng);
				
					if(bAliDim)
						Dom.Rotate(xyLast, vAngRot);
					*pDomP << Dom;
				}

				// dStaCuttingOrg ����ؼ� ��ǥ ������.
				nTypeDim	= GetTypeDimRebarInfoCycle(bHor, xySttJoint, xyEndJoint, nDrawLeft, dStaCuttingOrg);
				GetDimInfoByTypeDim(xySttJoint, xyEndJoint, bHor, nTypeDim, nDrawLeft, dStaCuttingOrg);
				
				if(bAliDim)
				{
					dLenTxt	= ~(xyEndJoint-xySttJoint);
					dLen	= dLenTxt;
				}
				else
				{
					dLenTxt	= bHor ? (xyEndJoint.x-xySttJoint.x) : (xyEndJoint.y-xySttJoint.y);
					dLen	= bAli ? ~(xyEndJoint-xySttJoint) * (dLenTxt < 0 ? -1 : 1) : dLenTxt;
				}
				if(!Compare(dLen, 0.0, "="))
				{
					if(bHor)
					{
						vAng = (xyEndJoint - xySttJoint).Unit();
						if(vAng.x < 0) vAng *= -1;
					}

					Dom.SetDimObq(bAli && bHor ? vAng.GetAngleDegree() * (dLen < 0 ? -1 : 1) : 0);
					if(!bHor && dLen < 0)
					{
						Dom.SetDirection(bLeft ? "RIGHT" : "LEFT");
					}
					if(bHor && dLen < 0)
					{
						Dom.SetDirection(bLeft ? "BOTTOM" : "TOP");
					}
				
					if(bHor && bUpperSlabLower)
						xySttJoint.y	= dYOrg;
					Dom.DimMoveTo(xySttJoint.x, xySttJoint.y, nDimDan);
					Dom.DimLineTo(dLen, nDimDan, COMMA(fabs(dLenTxt)));
					if(bAli && bHor)
						Dom.Rotate(xySttJoint, vAng);

					if(bAliDim)
						Dom.Rotate(xySttJoint, vAngRot);
					*pDomP << Dom;
				}

				xyLast	= xyEndJoint;
			}
			else
			{
				// dStaCuttingOrg ����ؼ� ��ǥ ������.
				nTypeDim	= GetTypeDimRebarInfoCycle(bHor, xyLast, pRC->m_xyMid1, nDrawLeft, dStaCuttingOrg);
				GetDimInfoByTypeDim(xyLast, pRC->m_xyMid1, bHor, nTypeDim, nDrawLeft, dStaCuttingOrg);

				if(bHor)
				{
					vAng = (pRC->m_xyMid1 - xyLast).Unit();
					if(vAng.x < 0) vAng *= -1;
				}

				if(bAliDim)
				{
					dLenTxt	= ~(pRC->m_xyMid1-xyLast);
					dLen	= dLenTxt;
				}
				else
				{
					dLenTxt	= bHor ? (pRC->m_xyMid1.x-xyLast.x) : (pRC->m_xyMid1.y-xyLast.y);
					dLen	= bAli ? ~(pRC->m_xyMid1-xyLast) * (dLenTxt < 0 ? -1 : 1) : dLenTxt;
				}
				if(!Compare(dLen, 0.0, "="))
				{
					Dom.SetDimObq(bAli && bHor ? vAng.GetAngleDegree() * (dLen < 0 ? -1 : 1) : 0);
					if(!bHor && dLen < 0)
					{
						Dom.SetDirection(bLeft ? "RIGHT" : "LEFT");
					}
					if(bHor && dLen < 0)
					{
						Dom.SetDirection(bLeft ? "BOTTOM" : "TOP");
					}

					if(bHor && bUpperSlabLower)
						xyLast.y	= dYOrg;
					Dom.DimMoveTo(xyLast.x, xyLast.y, nDimDan);
					Dom.DimLineTo(dLen, nDimDan, COMMA(fabs(dLenTxt)));
					if(bAli && bHor)
						Dom.Rotate(xyLast, vAng);
					
					if(bAliDim)
						Dom.Rotate(xyLast, vAngRot);
					*pDomP << Dom;
				}

				// ���� ö�� ���۱��� ġ�� ����
				if(rc < nSize-1)
				{
					CRebarInfoCycle *pRCNext	= pArrRC->GetAt(rc+1);

					// dStaCuttingOrg ����ؼ� ��ǥ ������.
					nTypeDim	= GetTypeDimRebarInfoCycle(bHor, pRC->m_xyMid1, pRCNext->m_xyStt, nDrawLeft, dStaCuttingOrg);
					GetDimInfoByTypeDim(pRC->m_xyMid1, pRCNext->m_xyStt, bHor, nTypeDim, nDrawLeft, dStaCuttingOrg);

					if(bHor)
					{
						vAng = (pRCNext->m_xyStt - pRC->m_xyMid1).Unit();
						if(vAng.x < 0) vAng *= -1;
					}

					if(bAliDim)
					{
						dLenTxt	= ~(pRCNext->m_xyStt-pRC->m_xyMid1);
						dLen	= dLenTxt;
					}
					else
					{
						dLenTxt	= bHor ? (pRCNext->m_xyStt.x-pRC->m_xyMid1.x) : (pRCNext->m_xyStt.y-pRC->m_xyMid1.y);
						dLen	= bAli ? ~(pRCNext->m_xyStt-pRC->m_xyMid1) * (dLenTxt < 0 ? -1 : 1) : dLenTxt;
					}
					if(!Compare(dLen, 0.0, "="))
					{
						Dom.SetDimObq(bAli && bHor ? vAng.GetAngleDegree() * (dLen < 0 ? -1 : 1) : 0);
						if(!bHor && dLen < 0)
						{
							Dom.SetDirection(bLeft ? "RIGHT" : "LEFT");
						}
						if(bHor && dLen < 0)
						{
							Dom.SetDirection(bLeft ? "BOTTOM" : "TOP");
						}
					
						CDPoint xyMid1	= pRC->m_xyMid1;
						if(bHor && bUpperSlabLower)
							xyMid1.y	= dYOrg;
						Dom.DimMoveTo(xyMid1.x, xyMid1.y, nDimDan);
						Dom.DimLineTo(dLen, nDimDan, COMMA(fabs(dLenTxt)));
						if(bAli && bHor)
							Dom.Rotate(xyMid1, vAng);

						if(bAliDim)
							Dom.Rotate(xyMid1, vAngRot);

						*pDomP << Dom;
					}

					xyLast	= pRCNext->m_xyStt;
				}
				else
					xyLast	= pRC->m_xyMid1;
			}
		}
		else
		{
			bHor	= IsDrawByLineInfo(pRC->m_nPosStt) ? TRUE : FALSE;

			// dStaCuttingOrg ����ؼ� ��ǥ ������.
			nTypeDim	= GetTypeDimRebarInfoCycle(bHor, xyLast, pRC->m_xyMid1, nDrawLeft, dStaCuttingOrg);
			GetDimInfoByTypeDim(xyLast, pRC->m_xyMid1, bHor, nTypeDim, nDrawLeft, dStaCuttingOrg);

			if(bHor)
			{
				vAng = (pRC->m_xyMid1 - xyLast).Unit();
				if(vAng.x < 0) vAng *= -1;
			}

			if(bAliDim)
			{
				dLenTxt	= ~(pRC->m_xyMid1-xyLast);
				dLen	= dLenTxt;
			}
			else
			{
				dLenTxt	= bHor ? (pRC->m_xyMid1.x-xyLast.x) : (pRC->m_xyMid1.y-xyLast.y);
				dLen	= bAli ? ~(pRC->m_xyMid1-xyLast) * (dLenTxt < 0 ? -1 : 1) : dLenTxt;
			}
			Dom.SetDirection(bHor ? (bLeft ? "TOP" : "BOTTOM") : (bLeft ? "LEFT" : "RIGHT"));
			if(!Compare(dLen, 0.0, "="))
			{
				Dom.SetDimObq(bAli && bHor ? vAng.GetAngleDegree() * (dLen < 0 ? -1 : 1) : 0);
				if(!bHor && dLen < 0)
				{
					Dom.SetDirection(bLeft ? "RIGHT" : "LEFT");
				}
				if(bHor && dLen < 0)
				{
					Dom.SetDirection(bLeft ? "BOTTOM" : "TOP");
				}

				if(bHor && bUpperSlabLower)
					xyLast.y	= dYOrg;

				Dom.DimMoveTo(xyLast.x, xyLast.y, nDimDan);
				Dom.DimLineTo(dLen, nDimDan, COMMA(fabs(dLenTxt)));
				if(bAli && bHor)
					Dom.Rotate(xyLast, vAng);

				if(bAliDim)
					Dom.Rotate(xyLast, vAngRot);
				*pDomP << Dom;
			}

			// arc
			if(bApplyRound)
			{
				// dStaCuttingOrg ����ؼ� ��ǥ ������.
				nTypeDim	= GetTypeDimRebarInfoCycle(bHor, pRC->m_xyMid1, pRC->m_xyMid2, nDrawLeft, dStaCuttingOrg);
				GetDimInfoByTypeDim(pRC->m_xyMid1, pRC->m_xyMid2, bHor, nTypeDim, nDrawLeft, dStaCuttingOrg);

				CRebarPlacing rb;
				double dR	= pRC->m_R_Mid;//fabs(pRC->m_xyMid2.x - pRC->m_xyMid1.x);

				dLenTxt		= rb.GetArcLength(pRC->m_xyCen, pRC->m_xyMid2, pRC->m_xyMid1);
				dLen		= dLenTxt;

				// arc ġ�� �������� �ݵ�� TOP���� �����ؾ� ���ϴ� ��� ���´�.
				if(nTypeDim == 0)
				{
					Dom.SetDirection("TOP");
					if(!Compare(dLen, 0.0, "="))
					{
						double dDist	= Dom.Always(Dom.GetDimLevelDis()) * nDimDan;
						CDPoint xyMid2	= bArcReverse? pRC->m_xyMid1 : pRC->m_xyMid2;
						CDPoint xyMid1	= bArcReverse? pRC->m_xyMid2 : pRC->m_xyMid1;
						CDPoint xyCen	= pRC->m_xyCen;
						if(pRC->m_R_Mid > 0)
						{
							Dom.DimArcMoveTo(xyCen.x, xyCen.y, dR+dDist, xyMid2.x, xyMid2.y, 0);
							Dom.DimArcLineToExtend(xyMid1.x, xyMid1.y, 0, COMMA(pRC->GetLengthMid()), "R="+COMMA(fabs(dR)));
						}
						else
						{
							double dDimExo	= Dom.GetDimExo();
							double dDimVLen	= Dom.GetDimVLen();
							dDist	+= Dom.Always(dDimExo+dDimVLen);
							Dom.SetDimExo(0);
							Dom.SetDimVLen(0);
							
							xyMid1	+= (xyMid1-xyCen).Unit() * (dDist);
							xyMid2	+= (xyMid2-xyCen).Unit() * (dDist);
							Dom.DimMoveTo(xyMid1.x, xyMid1.y, 0, "", FALSE);
							Dom.DimLineTo(~(xyMid1-xyMid2), 0, COMMA(pRC->GetLengthMid()), FALSE);
							Dom.Rotate(xyMid1, (xyMid2-xyMid1).Unit());

							Dom.SetDimExo(dDimExo);
							Dom.SetDimVLen(dDimVLen);
						}
					}
					*pDomP << Dom;
				}
			}

			// �������� ������ ��ġ�� ������ ���� ��ġ���� �ľ��ؾ� �ȴ�. ///////////
			BOOL bJoint		= FALSE;
			CRebarJoint joint;
			CDPoint xySttJoint	= CDPoint(0, 0);
			CDPoint xyEndJoint	= CDPoint(0, 0);
			long i = 0; for(i = 0; i < arrJoint.GetSize(); i++)
			{
				joint	= arrJoint.GetAt(i);
				xySttJoint	= joint.m_xyPos + (-joint.m_xyDir * joint.m_dLength/2);
				xyEndJoint	= xySttJoint + (joint.m_xyDir * joint.m_dLength);
				
				if(ComparePt(xyEndJoint, pRC->m_xyEnd, "=") || ComparePt(xySttJoint, pRC->m_xyEnd, "="))
				{
					bJoint	= TRUE;
					if(ComparePt(xySttJoint, pRC->m_xyEnd, "=")) GetSwap(xySttJoint, xyEndJoint);
					break;
				}
			}
			////////////////////////////////////////////////////////////////////////////////

			bHor	= IsDrawByLineInfo(pRC->m_nPosEnd) ? TRUE : FALSE;
			Dom.SetDirection(bHor ? (bLeft ? "TOP" : "BOTTOM") : (bLeft ? "LEFT" : "RIGHT"));

			if(pRC->m_nPosStt == REBARINFO_POS_UPPERSLAB_UPPER && pRC->m_nPosEnd == REBARINFO_POS_ENDWALL_OUTTER)
			{
				bAli = FALSE;
			}

			if(bJoint)
			{
				// dStaCuttingOrg ����ؼ� ��ǥ ������.
				nTypeDim	= GetTypeDimRebarInfoCycle(bHor, pRC->m_xyMid2, xySttJoint, nDrawLeft, dStaCuttingOrg);
				GetDimInfoByTypeDim(pRC->m_xyMid2, xySttJoint, bHor, nTypeDim, nDrawLeft, dStaCuttingOrg);

				if(bHor)
				{
					vAng = (xySttJoint - pRC->m_xyMid2).Unit();
					if(vAng.x < 0) vAng *= -1;
				}

				if(bAliDim)
				{
					dLenTxt	= ~(xySttJoint-pRC->m_xyMid2);
					dLen	= dLenTxt;
				}
				else
				{
					dLenTxt	= bHor ? (xySttJoint.x-pRC->m_xyMid2.x) : (xySttJoint.y-pRC->m_xyMid2.y);
					dLen	= bAli ? ~(xySttJoint-pRC->m_xyMid2) * (dLenTxt < 0 ? -1 : 1) : dLenTxt;
				}

				if(!Compare(dLen, 0.0, "="))
				{
					Dom.SetDimObq(bAli && bHor ? vAng.GetAngleDegree() * (dLen < 0 ? -1 : 1) : 0);
					if(!bHor && dLen < 0)
					{
						Dom.SetDirection(bLeft ? "RIGHT" : "LEFT");
					}
					if(bHor && dLen < 0)
					{
						Dom.SetDirection(bLeft ? "BOTTOM" : "TOP");
					}

					CDPoint xyMid2	= pRC->m_xyMid2;
					if(bHor && bUpperSlabLower)
						xyMid2.y	= dYOrg;
					
					Dom.DimMoveTo(xyMid2.x, xyMid2.y, nDimDan);
					Dom.DimLineTo(dLen, nDimDan, COMMA(fabs(dLenTxt)));
					if(bAli && bHor)
						Dom.Rotate(xyMid2, vAng);

					if(bAliDim)
						Dom.Rotate(xyMid2, vAngRot);
					*pDomP << Dom;
				}

				// dStaCuttingOrg ����ؼ� ��ǥ ������.
				nTypeDim	= GetTypeDimRebarInfoCycle(bHor, xySttJoint, xyEndJoint, nDrawLeft, dStaCuttingOrg);
				GetDimInfoByTypeDim(xySttJoint, xyEndJoint, bHor, nTypeDim, nDrawLeft, dStaCuttingOrg);

				if(bHor)
				{
					vAng = (xyEndJoint - xySttJoint).Unit();
					if(vAng.x < 0) vAng *= -1;
				}

				if(bAliDim)
				{
					dLenTxt	= ~(xyEndJoint-xySttJoint);
					dLen	= dLenTxt;
				}
				else
				{
					dLenTxt	= bHor ? (xyEndJoint.x-xySttJoint.x) : (xyEndJoint.y-xySttJoint.y);
					dLen	= bAli ? ~(xyEndJoint-xySttJoint) * (dLenTxt < 0 ? -1 : 1) : dLenTxt;
				}
				if(!Compare(dLen, 0.0, "="))
				{
					Dom.SetDimObq(bAli && bHor ? vAng.GetAngleDegree() * (dLen < 0 ? -1 : 1) : 0);
					if(!bHor && dLen < 0)
					{
						Dom.SetDirection(bLeft ? "RIGHT" : "LEFT");
					}
					if(bHor && dLen < 0)
					{
						Dom.SetDirection(bLeft ? "BOTTOM" : "TOP");
					}

					if(bHor && bUpperSlabLower)
						xySttJoint.y	= dYOrg;


					Dom.DimMoveTo(xySttJoint.x, xySttJoint.y, nDimDan);
					Dom.DimLineTo(dLen, nDimDan, COMMA(fabs(dLenTxt)));
					if(bAli && bHor)
						Dom.Rotate(xySttJoint, vAng);

					if(bAliDim)
						Dom.Rotate(xySttJoint, vAngRot);
					*pDomP << Dom;
				}

				xyLast	= xyEndJoint;
			}
			else
			{
				// dStaCuttingOrg ����ؼ� ��ǥ ������.
				nTypeDim	= GetTypeDimRebarInfoCycle(bHor, pRC->m_xyMid2, pRC->m_xyEnd, nDrawLeft, dStaCuttingOrg);
				GetDimInfoByTypeDim(pRC->m_xyMid2, pRC->m_xyEnd, bHor, nTypeDim, nDrawLeft, dStaCuttingOrg);

				if(bHor)
				{
					vAng = (pRC->m_xyEnd - pRC->m_xyMid2).Unit();
					if(vAng.x < 0) vAng *= -1;
				}

				if(bAliDim)
				{
					dLenTxt	= ~(pRC->m_xyEnd-pRC->m_xyMid2);
					dLen	= dLenTxt;
				}
				else
				{
					dLenTxt	= bHor ? (pRC->m_xyEnd.x-pRC->m_xyMid2.x) : (pRC->m_xyEnd.y-pRC->m_xyMid2.y);
					dLen	= bAli ? ~(pRC->m_xyEnd-pRC->m_xyMid2) * (dLenTxt < 0 ? -1 : 1) : dLenTxt;
				}
				if(!Compare(dLen, 0.0, "="))
				{
					Dom.SetDimObq(bAli && bHor ? vAng.GetAngleDegree() * (dLen < 0 ? -1 : 1) : 0);
					if(!bHor && dLen < 0)
					{
						Dom.SetDirection(bLeft ? "RIGHT" : "LEFT");
					}
					if(bHor && dLen < 0)
					{
						Dom.SetDirection(bLeft ? "BOTTOM" : "TOP");
					}

					CDPoint xyMid2	= pRC->m_xyMid2;
					if(bHor && bUpperSlabLower)
						xyMid2.y	= dYOrg;

					Dom.DimMoveTo(xyMid2.x, xyMid2.y, nDimDan);
					Dom.DimLineTo(dLen, nDimDan, COMMA(fabs(dLenTxt)));
					if(bAli && bHor)
						Dom.Rotate(pRC->m_xyMid2, vAng);

					if(bAliDim)
						Dom.Rotate(pRC->m_xyMid2, vAngRot);
					*pDomP << Dom;
				}

				// ���� ö�� ���۱��� ġ�� ����
				if(rc < nSize-1)
				{
					CRebarInfoCycle *pRCNext	= pArrRC->GetAt(rc+1);

					// dStaCuttingOrg ����ؼ� ��ǥ ������.
					nTypeDim	= GetTypeDimRebarInfoCycle(bHor, pRC->m_xyEnd, pRCNext->m_xyStt, nDrawLeft, dStaCuttingOrg);
					GetDimInfoByTypeDim(pRC->m_xyEnd, pRCNext->m_xyStt, bHor, nTypeDim, nDrawLeft, dStaCuttingOrg);

					if(bHor)
					{
						vAng = (pRCNext->m_xyStt - pRC->m_xyEnd).Unit();
						if(vAng.x < 0) vAng *= -1;
					}

					if(bAliDim)
					{
						dLenTxt	= ~(pRCNext->m_xyStt-pRC->m_xyEnd);
						dLen	= dLenTxt;
					}
					else
					{
						dLenTxt	= bHor ? (pRCNext->m_xyStt.x-pRC->m_xyEnd.x) : (pRCNext->m_xyStt.y-pRC->m_xyEnd.y);
						dLen	= bAli ? ~(pRCNext->m_xyStt-pRC->m_xyEnd) * (dLenTxt < 0 ? -1 : 1) : dLenTxt;
					}
					if(!Compare(dLen, 0.0, "="))
					{
						Dom.SetDimObq(bAli && bHor ? vAng.GetAngleDegree() * (dLen < 0 ? -1 : 1) : 0);
						if(!bHor && dLen < 0)
						{
							Dom.SetDirection(bLeft ? "RIGHT" : "LEFT");
						}
						if(bHor && dLen < 0)
						{
							Dom.SetDirection(bLeft ? "BOTTOM" : "TOP");
						}

						CDPoint xyEnd	= pRC->m_xyEnd;
						if(bHor && bUpperSlabLower)
							xyEnd.y	= dYOrg;

						Dom.DimMoveTo(xyEnd.x, xyEnd.y, nDimDan);
						Dom.DimLineTo(dLen, nDimDan, COMMA(fabs(dLenTxt)));
						if(bAli && bHor)
							Dom.Rotate(xyEnd, vAng);

						if(bAliDim)
							Dom.Rotate(xyEnd, vAngRot);
						*pDomP << Dom;
					}

					xyLast	= pRCNext->m_xyStt;
				}
				else
				{
					xyLast	= pRC->m_xyEnd;
					bHor	= IsDrawByLineInfo(pRC->m_nPosEnd) ? TRUE : FALSE;
					// ������ü�� ���� ��� ������ ġ���� �����Ҷ��� �Ǻ� ġ���� ���Ƿ� �־��ش�.(���ܻ���) ////
					if(pRC->m_nPosEnd == REBARINFO_POS_ENDWALL_OUTTER && !pBri->IsOutWall(FALSE))
					{
						bHor	= TRUE;
						bAli	= TRUE;
					}
				}
			}
		}
	}

	// ��� ġ�� ������ ������ ġ�� ����
	if(nSize > 0 && bCopplingSttEnd==FALSE)
	{
		// dStaCuttingOrg ����ؼ� ��ǥ ������.
		nTypeDim	= GetTypeDimRebarInfoCycle(bHor, xyLast, xyEnd, nDrawLeft, dStaCuttingOrg);
		GetDimInfoByTypeDim(xyLast, xyEnd, bHor, nTypeDim, nDrawLeft, dStaCuttingOrg);

		if(bHor)
		{
			vAng = (xyEnd - xyLast).Unit();
			if(vAng.x < 0) vAng *= -1;
		}

		if(bAliDim)
		{
			dLenTxt	= ~(xyEnd-xyLast);
			dLen	= dLenTxt;
		}
		else
		{
			dLenTxt	= bHor ? (xyEnd.x-xyLast.x) : (xyEnd.y-xyLast.y);
			dLen	= bAli ? ~(xyEnd-xyLast) * (dLenTxt < 0 ? -1 : 1) : dLenTxt;
		}
		if(!Compare(dLen, 0.0, "="))
		{
			Dom.SetDimObq(bAli && bHor ? vAng.GetAngleDegree() * (dLen < 0 ? -1 : 1) : 0);
			Dom.SetDirection(bHor ? (bLeft ? "TOP" : "BOTTOM") : (bLeft ? "LEFT" : "RIGHT"));
			if(!bHor && dLen < 0)
			{
				Dom.SetDirection(bLeft ? "RIGHT" : "LEFT");
				Dom.DimMoveTo(xyEnd.x, xyEnd.y, nDimDan, COMMA(fabs(dLenTxt)));
				Dom.DimLineTo(fabs(dLen), nDimDan, "");
				if(bAli && bHor)
					Dom.Rotate(xyEnd, vAng);

				if(bAliDim)
					Dom.Rotate(xyEnd, vAngRot);
				*pDomP << Dom;
			}
			else
			{
				
				// ��� ������ �ϸ��� ���� ���� ��� ��
				CRebarInfoCycle *pRC	= pArrRC->GetAt(nSize-1);
				if(!bAli)
				{
					if(pRC->m_nPosStt == REBARINFO_POS_UPPERSLAB_LOWER || pRC->m_nPosEnd == REBARINFO_POS_UPPERSLAB_LOWER)
					{
						xyLast.y	= rect.top - pBri->m_dTS + pBri->m_dCoverLower_UpperSlab[0] + dHDiff;
					}
				}


				Dom.DimMoveTo(xyLast.x, xyLast.y, nDimDan);
				if(Dom.GetTextWidth(COMMA(fabs(dLenTxt))) > fabs(dLen))
					Dom.DimLineToOver(dLen, nDimDan, COMMA(fabs(dLenTxt)));
				else
					Dom.DimLineTo(dLen, nDimDan, COMMA(fabs(dLenTxt)));
				if(bAli && bHor)
					Dom.Rotate(xyLast, vAng);

				if(bAliDim)
					Dom.Rotate(xyLast, vAngRot);
				*pDomP << Dom;
			}
		}
	}
	else if(bCopplingSttEnd==FALSE)
	{
		// dStaCuttingOrg ����ؼ� ��ǥ ������.
		nTypeDim	= GetTypeDimRebarInfoCycle(bHor, xyStt, xyEnd, nDrawLeft, dStaCuttingOrg);
		GetDimInfoByTypeDim(xyStt, xyEnd, bHor, nTypeDim, nDrawLeft, dStaCuttingOrg);

		if(bAliDim)
		{
			dLenTxt	= ~(xyEnd-xyStt);
			dLen	= dLenTxt;
		}
		else
		{
			dLenTxt	= bHor ? (xyEnd.x-xyStt.x) : (xyEnd.y-xyStt.y);
			dLen	= bAli ? ~(xyEnd-xyStt) : dLenTxt;
		}
		if(!Compare(dLen, 0.0, "="))
		{
			Dom.SetDimObq(bAli && bHor ? vAng.GetAngleDegree() : 0);
			Dom.SetDirection(bHor ? (bLeft ? "TOP" : "BOTTOM") : (bLeft ? "LEFT" : "RIGHT"));

			Dom.DimMoveTo(xyStt.x, xyEnd.y, nDimDan);
			Dom.DimLineTo(dLen, nDimDan, COMMA(fabs(dLenTxt)));
		
			if(bAliDim)
				Dom.Rotate(xyStt, (xyEnd-xyStt).Unit());
			*pDomP << Dom;
		}
	}

	*pDomP << Dom;
}

// �¿��� �߶� ǥ���� ��� ������ �Ǵ� sta ����
double CARcBridgeDBStandard::GetStaCuttingOrg()
{
	if(!m_pRcBridgeRebar) return 0;

	double dSta	= 0;
	BOOL bVertDir	= m_pRcBridgeRebar->IsVertDir();
	// �������� Ȧ���̸� �߰� ������ ������ ���� ��ġ,
	// �������� ¦���̸� �߰� ���� ��ġ.
	long nQtyJigan	= m_pRcBridgeRebar->m_nQtyJigan;
	if(nQtyJigan%2 == 0)
	{
		long nQtyJijum	= nQtyJigan+1;
		long nMidJijum	= nQtyJijum/2;

		dSta	= m_pRcBridgeRebar->GetStationOnJijum(nMidJijum, 0, bVertDir);	
	}
	else
	{
		long nJijumPrev	= nQtyJigan/2;
		long nJijumNext	= nJijumPrev+1;

		double dStaJijumPrev	= m_pRcBridgeRebar->GetStationOnJijum(nJijumPrev, 0, bVertDir);
		double dStaJijumNext	= m_pRcBridgeRebar->GetStationOnJijum(nJijumNext, 0, bVertDir);

		dSta	= (dStaJijumPrev+dStaJijumNext)/2;
	}

	return dSta;
}

// rebarinfocycle ġ�� ���Խ� ������ ����ؼ� ġ�� ���� Ÿ���� �����Ѵ�.
long CARcBridgeDBStandard::GetTypeDimRebarInfoCycle(BOOL bHor, CDPoint xyStt, CDPoint xyEnd, long nDrawLeft, double dStaCuttingOrg)
{
	long nTypeDim	= 0;

	// dStaCuttingOrg ��� /////////////////////////////////////////////////
	if(nDrawLeft != 0)
	{
		if(!bHor)
		{
			if(nDrawLeft == -1)
			{
				if(xyStt.x > dStaCuttingOrg) nTypeDim	= -1;
			}
			else if(nDrawLeft == 1)
			{
				if(xyStt.x < dStaCuttingOrg) nTypeDim	= -1;
			}
		}
		else
		{
			if(nDrawLeft == -1)
			{
				if(xyStt.x >= dStaCuttingOrg) nTypeDim	= -1;
				else if(xyEnd.x > dStaCuttingOrg) nTypeDim	= 1;
			}
			else if(nDrawLeft == 1)
			{
				if(xyEnd.x <= dStaCuttingOrg) nTypeDim	= -1;
				else if(xyStt.x < dStaCuttingOrg) nTypeDim	= 1;
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////

	return nTypeDim;
}

// typeDim�� ���� ������ ���� �Ǵ� ġ���� ��ġ�� �����Ѵ�.
void CARcBridgeDBStandard::GetDimInfoByTypeDim(CDPoint &xyStt, CDPoint &xyEnd, BOOL bHor, long nTypeDim, long nDrawLeft, double dStaCuttingOrg)
{
	// dStaCuttingOrg ��� �����϶��� ����
	if(nTypeDim == 0) return;
	if(nDrawLeft == 0) return;

	// �ƿ� ġ�� ���� ���� �������� ���� ��ġ ����ؼ� �������� �����ؼ� ����
	if(nTypeDim == -1)
	{
		xyStt	= xyEnd;
		return;
	}


	// �߷��� ġ�� ���� �ɶ�.
	if(nTypeDim	== 1)
	{
		if(bHor)
		{
			// ���ʸ� �����ϴµ� �߷��� �Ǹ� ������ ġ���� ����
			if(nDrawLeft == -1)
			{
				xyEnd.x	= dStaCuttingOrg;
			}
			else if(nDrawLeft == 1)
			{
				xyStt.x	= dStaCuttingOrg;
			}
		}
		else
		{
			// �����϶��� �߷��� ���� �Ǵ� ���� �����Ƿ� �׳� ����.
			return;
		}
	}
}

// ���ݹ迭�� �ݺ�ġ�� ���� ������ ��ȯ
// CDomyun *pDomP : ġ�����Խ� ���ڳʺ� �ľ��ϱ� ����.
// BOOL bApplySttDistRep, BOOL bApplyEndDistRep : ó���� ������ ���ݿ� �ݺ�ġ�� ��������?
// CDoubleArray &dArrDistText : ���� �Ÿ��� ��µǴ� ���ڰ� �ٸ� �� ����(� ������ ���..)
void CARcBridgeDBStandard::GetDimInfoAtDistArray(CDoubleArray &dArrDim, CStringArray &sArrText, CStringArray &sArrUnderText, CDoubleArray &dArrDist, CString sMark, BOOL bInput, BOOL bMerge, BOOL bHor, CDomyun *pDomP, double dDimLimit, BOOL bApplySttDistRep, BOOL bApplyEndDistRep, CDoubleArray &dArrDistText)
{
	if(!bMerge)
	{
		dArrDim.RemoveAll();
		sArrText.RemoveAll();
		sArrUnderText.RemoveAll();
	}

	if(dArrDistText.GetSize() < dArrDist.GetSize())
	{
		long i = 0; for(i = dArrDistText.GetSize(); i < dArrDist.GetSize(); i++)
			dArrDistText.Add(dArrDist.GetAt(i));
	}

	// ������ offset ������ ��ȯ
	CRebarPlacing rb;
	CDPointArray xyArr;
	rb.ConvertOffsetDoubleArrayToOffsetInfo(dArrDistText, xyArr, dDimLimit, bApplySttDistRep, bApplyEndDistRep);

	
	long nSize	= xyArr.GetSize();
	CDPoint xyR(0, 0);
	CString sNum	= _T("");
	CString sNumUnder	= _T("");
	long nIdxCur	= 0;
// 	double dTot	= 0;
	long i = 0; for(i = 0; i < nSize; i++)
	{
		if(nSize-1 < i) break;

		xyR	= xyArr.GetAt(i);

		// ConvertOffsetDoubleArrayToOffsetInfo�� dDimLimit�� ���ؼ� ���е��� �ݿ��� ��� ////////
		// ���� �Ÿ��� ������ ���� �����Ƿ�, ���� �Ÿ��� dArrDist�� �����ؼ� �ٽ� ����Ѵ�.
		double dTot	= 0;
		for(long j = 0; j < xyR.x; j++)
		{
			dTot += dArrDist.GetAt(nIdxCur++);
		}
		//////////////////////////////////////////////////////////////////////////////////////////
		
		dArrDim.Add(dTot);
		if(bInput)
		{
			sNum.Format("%s%d", sMark, i+1);
			sArrText.Add(sNum);
			if(xyR.x > 1)
				sNum.Format("%d@%s=%s", (long)xyR.x, COMMA(xyR.y), COMMA(xyR.x*xyR.y));
			else
				sNum.Format("%s", COMMA(xyR.y));
			sArrUnderText.Add(sNum);
		}
		else
		{
			if(xyR.x > 1)
			{
				if(pDomP)
				{
					sNum.Format("%d@%s", (long)xyR.x, COMMA(xyR.y));
					sNumUnder.Format("=%s", COMMA(xyR.x*xyR.y));

					if(pDomP->GetTextWidth(sNum+sNumUnder) < xyR.x*xyR.y)
					{
						sNum += sNumUnder;
						sNumUnder	= _T("");
					}

					sArrText.Add(sNum);
					sArrUnderText.Add(sNumUnder);
				}
				else
				{
					sNum.Format("%d@%s", (long)xyR.x, COMMA(xyR.y));
					sArrText.Add(sNum);

					sNum.Format("=%s", COMMA(xyR.x*xyR.y));
					sArrUnderText.Add(sNum);
				}
			}
			else
			{
				sNum.Format("%s", COMMA(xyR.y));
				sArrText.Add(sNum);
				sArrUnderText.Add("");
			}
		}
	}
}

// rect �ΰ��� �ϳ��� rect�� ����
CDRect CARcBridgeDBStandard::MergeRectAndRect(CDRect rect1, CDRect rect2)
{
	CDRect rect;
	rect.left	= rect1.left < rect2.left ? rect1.left : rect2.left;
	rect.bottom	= rect1.bottom > rect2.bottom ? rect1.bottom : rect2.bottom;
	rect.top	= rect1.top < rect2.top ? rect1.top : rect2.top;
	rect.right	= rect1.right > rect2.right ? rect1.right : rect2.right;

	return rect;
}

// �米���� ���� ġ�� ������ �ʿ�����?
// ��Ģ : ��� ������ �米�� ��찡 �ϳ��� ������ ������ �米���� ���� ġ�� ���� �ϰ�
//        ��� ������ ���� ������ �米������� ���� ġ�� ������ �Ѵ�.
BOOL CARcBridgeDBStandard::IsDimByAngleLen()
{
	CRcBridgeApp *pBri	= m_pRcBridge;

	BOOL bDimByAngleLen	= FALSE;
	long i = 0; for(i = 0; i < pBri->m_nQtyJigan+1; i++)
	{
		if(!ComparePt(pBri->GetAngleJijum(i), CDPoint(0, 1), "="))
		{
			bDimByAngleLen	= TRUE;
			break;
		}
	}

	return bDimByAngleLen;
}

// �������ϰ�� ��ȣ�� ġ���� ���־� ��.
void CARcBridgeDBStandard::ModifyInfoAutoDimConc(CStringArray &sArrText, CStringArray &sArrUnderText, BOOL bInput)
{
	if(IsDimByAngleLen()) return;

	if(!bInput)
	{
		sArrUnderText.RemoveAll();
		return;
	}

	long nSize1 = sArrText.GetSize();
	long nSize2 = sArrUnderText.GetSize();

	CString str1	= _T("");
	CString str2	= _T("");
	long i = 0; for(i = 0; i < nSize2; i++)
	{
		str1	= i >= nSize1 ? _T("") : sArrText.GetAt(i);
		str2	= sArrUnderText.GetAt(i);

		if(str2.GetLength() > 2)
		{
			str2.Delete(0);
			str2.Delete(str2.GetLength()-1);
			if(str1 == str2)
			{
				str2	= _T("0");
				sArrUnderText.SetAt(i, str2);
			}
		}
	}
}

// tvArr�� ö�� R ������ ������ �׸�.
// bRoundAtStt : ���� ���ۺο� ���尡 �ִٴ� ��.
// dLenRound : ����� ǥ���� ����
void CARcBridgeDBStandard::DrawTvArrWidthRound(CDomyun *pDomP, CTwinVectorArray &tvArr, double dLenRound, BOOL bRoundAtStt)
{
	CDomyun Dom(pDomP);
	long lineType	= m_pARoadOptionStd->GetEnvType(&Dom);

	CTwinVector tv, tvSolid, tvHidden;
	long nSize	= tvArr.GetSize();

	long i = 0; for(i = 0; i < nSize; i++)
	{
		tv	= tvArr.GetAt(i);
		if(bRoundAtStt)
		{
			tvHidden.m_v1	= tv.m_v1;
			tvHidden.m_v2	= tv.m_v1 + (tv.GetXyDir()*dLenRound);
			tvSolid.m_v1	= tvHidden.m_v2;
			tvSolid.m_v2	= tv.m_v2;
		}
		else
		{
			tvHidden.m_v1	= tv.m_v2 + (tv.GetXyDir(TRUE) * dLenRound);
			tvHidden.m_v2	= tv.m_v2;
			tvSolid.m_v1	= tv.m_v1;
			tvSolid.m_v2	= tvHidden.m_v1;
		}
		
		m_pARoadOptionStd->SetEnvLType(&Dom, LT_SOLID);
		Dom.LineTo(tvSolid.m_v1, tvSolid.m_v2);

		m_pARoadOptionStd->SetEnvLType(&Dom, LT_HIDDEN_LINE);
		Dom.LineTo(tvHidden.m_v1, tvHidden.m_v2);
	}

	m_pARoadOptionStd->SetEnvType(&Dom, lineType);

	*pDomP << Dom;
}

// ���� ö��ó�� �����ư��鼭 �������� �׷��� �ϴ� ���..
// bHiddenAtStt�� true�̸� �����Ҷ����� ����..
void CARcBridgeDBStandard::DrawTvArrayWidthDashDot(CDomyun *pDomP, CTwinVectorArray &tvArr, BOOL bHiddenAtStt)
{
	CDomyun Dom(pDomP);

	CTwinVector tv;
	long nSize	= tvArr.GetSize();

	long nLineTypeFir	= bHiddenAtStt ? LT_DASHDOT_LINE : LT_SOLID;
	long nLineTypeSec	= bHiddenAtStt ? LT_SOLID : LT_DASHDOT_LINE;
	BOOL bFirst			= TRUE;

	long i = 0; for(i = 0; i < nSize; i++)
	{
		tv	= tvArr.GetAt(i);

		m_pARoadOptionStd->SetEnvLType(&Dom, bFirst ? nLineTypeFir : nLineTypeSec);
		bFirst	= !bFirst;
		Dom.LineTo(tv.m_v1, tv.m_v2);
	}

	*pDomP << Dom;
}

// CRebar *pRB : ��ȣ�� �ʻ��..
// pRB �� NULL�� ��� �Ϲ������� �׳� �׸���.
// �׷���!!! pRB�� �����ϴ� ���, �̹� ���谡 �� ��Ȳ�̶� �Ǵ��ϸ�..
// pRB�� RebarInfoCycle�� �̹� ��ȣ��  match�Ǿ� �ִ�...
// �׷��Ƿ� pRB�� RebarInfoCycle �� ��ȣ�� ���ؼ� �����Ÿ� �׸���.
// �̰Ŵ� ö�� ���� â���� �ش� ö�ٸ� �������ؼ� �׷��ش�.
void CARcBridgeDBStandard::SetMarkForMarkRebar(CString sMark)
{
	CARcBridgeDBStandard::m_sMarkForMarkRebar	= sMark;
}

// ��ö���� m_sMarkForMarkRebar�� ���� ��ŷ�� �ؾ� �Ǵ� ��찡 �ְ�, �� �ϴ� ��찡�ִ�.
// ö�� ����� ö�� �������� �����ֱ�����.
BOOL CARcBridgeDBStandard::IsMarkRebar(CString sMark)
{
	if(CARcBridgeDBStandard::m_sMarkForMarkRebar == "") return TRUE;
	if(CARcBridgeDBStandard::m_sMarkForMarkRebar == sMark) return TRUE;

	return FALSE;
}

void CARcBridgeDBStandard::MarkCenterOfRoad(CDomyun *pDomP, CDPoint xyText, double dLength, CString strSec, BOOL bCenterOfRoad)
{
	CDomyun Dom(pDomP);

	m_pARoadOptionStd->SetEnvType(&Dom,HCAD_TXTG);

	if(dLength==0)
		xyText.y += Dom.Always(3);

	Dom.SetTextAlignHorz(TA_LEFT);
	double dTextHgt  = Dom.GetTextHeight();
	Dom.TextOut(xyText.x-dTextHgt*0.5, xyText.y, _T("C"));
	Dom.TextOut(xyText.x-Dom.Always(0.4), xyText.y-dTextHgt*0.5, bCenterOfRoad ? _T("L OF ROAD") : _T("L OF STR."));
	if(strSec!=_T(""))
		Dom.TextOut(xyText.x, xyText.y-dTextHgt*2.0, strSec);

	if(dLength>0)
	{
		m_pARoadOptionStd->SetEnvType(&Dom,HCAD_CENT);
		double dTextFactor = strSec==_T("") ? 1.0 : 2.5;
		Dom.LineTo(CDPoint(xyText.x, xyText.y-dTextHgt*dTextFactor), CDPoint(xyText.x, xyText.y-dLength));
	}

	*pDomP << Dom;
}

// ������ ���� �Ǵ� �ܺθ� Ʈ��.
// �������� �ݽð�������� �׷��� �־�� ��.
// bOutside:�ܺθ� Ʈ��(���� �ܺ� Ʈ���� ����).
void CARcBridgeDBStandard::GetTvArrTrimByPolygon(CTwinVectorArray &tvArrSource, CTwinVectorArray &tvArrPolygon, BOOL bOutside, CTwinVectorArray &tvArrResult)
{
	CTwinVectorArray tvArr;
	tvArr = tvArrSource;

	CTwinVector tv, tvP, tvIn, tvOut;
	CDPoint xyMatch(0, 0);

	BOOL bLeft1 = FALSE, bLeft2 = FALSE;
	long nSize = tvArr.GetSize();
	long nSizeP = tvArrPolygon.GetSize();

	long i=0; for(i=0; i<nSizeP; i++)
	{
		tvP = tvArrPolygon.GetAt(i);
		if(tvP.GetLength()==0) continue;
		for(long j=0; j<nSize; j++)
		{
			tv = tvArr.GetAt(j);
			if(tv.GetLength()==0)
			{
				tvArr.RemoveAt(j);
				nSize = tvArr.GetSize();
				j--;
				continue;
			}
			bLeft1 = GetDirPointByLine(tv.m_v1, tvP.m_v1, tvP.m_v2);
			bLeft2 = GetDirPointByLine(tv.m_v2, tvP.m_v1, tvP.m_v2);
			if(!bLeft1 && !bLeft2)
			{
				tvArr.RemoveAt(j);
				nSize = tvArr.GetSize();
				j--;
				continue;
			}
			tvIn = tv;
			if(GetXyMatchSegAndSeg(tvP.m_v1, tvP.m_v2, tv.m_v1, tv.m_v2, xyMatch))
			{
				if(bLeft1 && !bLeft2)
				{
					tvIn.m_v1 = tv.m_v1;
					tvIn.m_v2 = xyMatch;
				}
				else if(!bLeft1 && bLeft2)
				{
					tvIn.m_v1 = xyMatch;
					tvIn.m_v2 = tv.m_v2;
				}
			}
			if(bOutside)
				tvArr.SetAt(j, tvIn);
		}
	}

	tvArrResult = tvArr;
}

// ���� ġ�����Կ� �ʿ��� ������ �����Ѵ�.
void CARcBridgeDBStandard::ApplyDimSetting(CDomyun *pDomPSource, CDomyun *pDomPDest)
{
	if(m_bApplyDimSettingOfBaseDom)
	{
		pDomPDest->SetScaleDim(pDomPSource->GetScaleDim());
		pDomPDest->SetDimStdTxtHeight(pDomPSource->GetDimStdTxtHeight());
		pDomPDest->SetDimTxt(pDomPSource->GetDimTxt());
		pDomPDest->SetTextHeight(pDomPSource->GetTextHeight(FALSE));
		pDomPDest->SetDimExe(pDomPSource->GetDimExe());
		pDomPDest->SetDimExo(pDomPSource->GetDimExo());
		pDomPDest->SetDimLevelDis(pDomPSource->GetDimLevelDis());
		pDomPDest->SetDimVLen(pDomPSource->GetDimVLen());
	}
}

// ���� �ð� ������ �׸���(xy:���� ����, dWidth:��)
void CARcBridgeDBStandard::DrawConstJointHor(CDomyun* pDomP, CDPoint xy, double dWidth, double dAng)
{
	CDomyun Dom(pDomP);
	m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONH);

	double dJointW = Dom.Always(1);
	double dJointH = Dom.Always(0.5);

	Dom.MoveTo(xy);
	Dom.LineToEx((dWidth-dJointW)/2.0, 0);
	Dom.LineToEx(0, -dJointH);
	Dom.LineToEx(dJointW, 0);
	Dom.LineToEx(0, dJointH);
	Dom.LineToEx((dWidth-dJointW)/2.0, 0);

	Dom.Rotate(xy, ToDPointFrDegree(dAng));

	*pDomP << Dom;
}

// ���� �ð� ������ �׸���(xy:��� ����, dHeight:����)
// double dAng : 0���� �Ʒ��� ���� ���� ����
void CARcBridgeDBStandard::DrawConstJointVer(CDomyun* pDomP, CDPoint xy, double dHeight, BOOL bLeft, double dAng)
{
	CDomyun Dom(pDomP);
	m_pARoadOptionStd->SetEnvType(&Dom, HCAD_CONH);

	double dJointW = Dom.Always(2);
	double lr = bLeft ? -1 : 1;

	Dom.MoveTo(xy);
	Dom.LineToEx(0, -dHeight/2.0);
	Dom.LineToEx(dJointW*lr, 0);
	Dom.LineToEx(0, -dHeight/2.0);

	Dom.Rotate(xy, ToDPointFrDegree(dAng));

	*pDomP << Dom;
}


// ��ö���� R ��ũ ����
// ����� ���ʺο� ���� R ǥ�ø� ���ֹǷ� �¿�ǥ�ø� �����Ѵ�.
// �ٸ������� �ʿ��ϴٸ� �׶� �����ϵ��� ��. 2005.2.4
void CARcBridgeDBStandard::MarkRadiusRebarInfoCycle(CDomyun *pDomP, CRebarInfoCycle *pRC, CDRect rectLimit)
{
	if(!pRC) return;

	CDPoint xyStt	= pRC->m_xyStt;
	CDPoint xyMid1	= pRC->m_xyMid1;
	CDPoint xyMid2	= pRC->m_xyMid2;
	CDPoint xyEnd	= pRC->m_xyEnd;
	CDPoint xyCen	= pRC->m_xyCen;
	double dRMid	= pRC->m_R_Mid;

	if(Compare(dRMid, 0.0, "<=")) return;
	if(!pRC->m_bExist) return;

	double dGap	= 50;
	xyStt.y		+= dGap;
	xyMid1.y	+= dGap;
	xyMid2.y	+= dGap;
	xyCen.y		+= dGap;


	if(dRMid > 0 && pRC->GetLengthMid() > 0)
	{
		BOOL bLeftDir	= xyMid2.x > xyMid1.x;
		CDPoint xyMark	= xyCen;
		CDPoint xyDir	= bLeftDir ? ToDPointFrDegree(-45) : ToDPointFrDegree(225);
		xyMark			= xyMark + xyDir * dRMid;

		// �ش� ������ �ؾ� �� ���...
		if(rectLimit.Width() != 0 && rectLimit.Height() != 0)
		{
			BOOL bOK	= FALSE;
			if(xyMark.x >= rectLimit.left && xyMark.x <= rectLimit.right &&
				xyMark.y >= rectLimit.bottom && xyMark.y <= rectLimit.top)
				bOK	= TRUE;

			if(!bOK) return;
		}

		CDomyun Dom(pDomP);
		m_pARoadOptionStd->SetEnvType(&Dom, HCAD_DIML);

		CString sR		= _T("");
		CString sLen	= _T("");
		sR.Format("R=%.0f", dRMid);
		sLen.Format("L=%.0f", pRC->GetLengthMid(TRUE));
		double dWidthText 	= max(Dom.GetTextWidth(sLen), Dom.GetTextWidth(sR));
		Dom.GiSiArrowExtend(xyMark.x, xyMark.y, Dom.GetTextHeight()*3, bLeftDir ? -dWidthText : dWidthText, bLeftDir ? 3 : 1, sR, sLen);

		*pDomP << Dom;
	}
}

BOOL CARcBridgeDBStandard::IsViewVectorInfo()
{
	BOOL bView	= FALSE;
#ifdef _DEBUG
	bView = m_bViewVectorInfo;
// #else
// 	bView = FALSE;
#endif

	return bView;
}


// HIDDEN ������ �մ� ��� �������� �׸��� �Լ�
// SOLID ������ �Ǽ�
void CARcBridgeDBStandard::DrawTvArrWidthHidden(CDomyun *pDomP, CTwinVectorArray &tvArr, CLineInfo *pLine)
{
	CDomyun Dom(pDomP);
	long lineType	= m_pARoadOptionStd->GetEnvType(&Dom);

	CHgBaseDrawStd baseDraw;
	baseDraw.m_pLine	= pLine;
	CTwinVector tv;
	long nSize	= tvArr.GetSize();

	long i = 0; for(i = 0; i < nSize; i++)
	{
		tv	= tvArr.GetAt(i);

		if(tv.m_sInfo == "HIDDEN")
			m_pARoadOptionStd->SetEnvLType(&Dom, LT_HIDDEN_LINE);
		else
			m_pARoadOptionStd->SetEnvLType(&Dom, LT_SOLID);
		baseDraw.DrawTv(&Dom, tv);
	}

	m_pARoadOptionStd->SetEnvType(&Dom, lineType);

	*pDomP << Dom;
}

void CARcBridgeDBStandard::MarkStationJijum(CDomyun *pDomP, long nJijum, CDPoint xyMark)
{
	CDomyun Dom(pDomP);

	double staJijum = m_pRcBridge->GetStationOnJijum(nJijum);

	CString csText = _T("");
	if(nJijum==0) 
		csText = _T("���� ����");
	else if(nJijum==m_pRcBridge->m_nQtyJigan) 
		csText = _T("���� ����");
	else
		csText.Format("P%d",nJijum);

	CString csSta = _T("  ") + GetStationForStringOut(staJijum, 3, TRUE, FALSE);

	double dLengthLine = Dom.GetTextWidth(csSta,0);
	CDPoint vX((nJijum==0)?-1:1,0),vY(0,1);
	CDPoint xy1(xyMark);
	CDPoint xy2 = xyMark + Dom.Always(2)*vY;
	CDPoint xy3 = xy2 + dLengthLine*vX;

	Dom.LineTo(xy1,xy2);
	*pDomP << Dom;

	double dAngRad = CDPoint(1.5,1).Unit().GetAngleRadian();

	Dom.LineTo(xy2,xy3);
	Dom.TextOut((xy2+xy3)/2+Dom.Always(1)*vY,csText);
	Dom.TextOut((xy2+xy3)/2-Dom.GetTextHeight()-Dom.Always(1)*vY,csSta);
	if(nJijum==0)
		Dom.Rotate(-dAngRad,xy2.x,xy2.y);
	else
		Dom.Rotate(dAngRad,xy2.x,xy2.y);
	*pDomP << Dom;
}

// ��ö�� ������ �׸��� ö�� ��ȣ�� ���� ���� ǥ������?
BOOL CARcBridgeDBStandard::IsDrawingLengthInfo()
{
	return m_pDrawingOptionData ? m_pDrawingOptionData->m_bLengthInfoInMainRebarNumbering : TRUE;
}

void CARcBridgeDBStandard::DivideTvArrWithRound(CTwinVectorArrayArray *pTvArr, CTwinVectorArrayArray *pTvArrHidden, double dLenRoundStt, double dLenRoundEnd)
{
	if(pTvArrHidden)
		AhTPADelete(pTvArrHidden, (CTwinVectorArray*)0);

	if(dLenRoundEnd <= 0 && dLenRoundStt <= 0) return;

	CTwinVectorArray tvArrTot, tvArrTotHidden;


	CRebarPlacing rb;
	CTwinVector tv, tvOld;
	
// 	CTwinVectorArray *pTvArrLine	= NULL;
	CTwinVectorArray tvArrSolid, tvArrHiddenStt, tvArrHiddenEnd;
	
	long nSize	= pTvArr->GetSize();
	long i = 0; for(i = 0; i < nSize; i++)
	{
		CTwinVectorArray *pTvArrLine	= pTvArr->GetAt(i);
		tvArrSolid	= *pTvArrLine;
		tvArrHiddenStt	= *pTvArrLine;
		tvArrHiddenEnd	= *pTvArrLine;

		rb.StretchTvArr(tvArrSolid, -dLenRoundStt, -dLenRoundEnd);
		*pTvArrLine	= tvArrSolid;

		if(dLenRoundStt > 0)
		{
			rb.StretchTvArr(tvArrHiddenStt, 0, -(tvArrHiddenStt.GetLength()-dLenRoundStt));
			CTwinVectorArray *pTvArrHiddenTmp	= new CTwinVectorArray;
			*pTvArrHiddenTmp	= tvArrHiddenStt;
			pTvArrHidden->Add(pTvArrHiddenTmp);
		}

		if(dLenRoundEnd > 0)
		{
			rb.StretchTvArr(tvArrHiddenEnd, -(tvArrHiddenEnd.GetLength()-dLenRoundEnd), 0);
			CTwinVectorArray *pTvArrHiddenTmp	= new CTwinVectorArray;
			*pTvArrHiddenTmp	= tvArrHiddenEnd;
			pTvArrHidden->Add(pTvArrHiddenTmp);
		}
	}
}

// ������ ������.
void CARcBridgeDBStandard::DivideTvArrWithRound(CTwinVectorArray &tvArr, CTwinVectorArray &tvArrHidden, double dLenRoundStt, double dLenRoundEnd)
{
	tvArrHidden.RemoveAll();
	if(dLenRoundEnd <= 0 && dLenRoundStt <= 0) return;

	CTwinVectorArray tvArrTot, tvArrTotHidden;


	CRebarPlacing rb;
	CTwinVector tv, tvOld;
	CTwinVectorArray tvArrSolid;
	CTwinVectorArray tvArrLine;

	
	long nSize	= tvArr.GetSize();
	long i = 0; for(i = 0; i < nSize; i++)
	{
		tvArrLine.RemoveAll();
		BOOL bFirst	= TRUE;
		for(long j = i; j < nSize; j++)
		{
			tv	= tvArr.GetAt(j);
			i	= j;
			if(bFirst)
			{
				tvArrLine.Add(tv);
				bFirst	= FALSE;
			}
			else
			{
				if(tv.m_sInfo == tvOld.m_sInfo)
					tvArrLine.Add(tv);
				else
				{
					i--;
					break;
				}
			}
			tvOld	= tv;
		}

		tvArrSolid	= tvArrLine;
		rb.StretchTvArr(tvArrSolid, -dLenRoundStt, -dLenRoundEnd);
		tvArrTot.AddFromTvArray(tvArrSolid);


		if(dLenRoundStt > 0)
		{
			tvArrHidden	= tvArrLine;
			rb.StretchTvArr(tvArrHidden, 0, -(tvArrHidden.GetLength()-dLenRoundStt));
			tvArrTotHidden.AddFromTvArray(tvArrHidden);
		}

		if(dLenRoundEnd > 0)
		{
			tvArrHidden	= tvArrLine;
			rb.StretchTvArr(tvArrHidden, -(tvArrHidden.GetLength()-dLenRoundEnd), 0);
			tvArrTotHidden.AddFromTvArray(tvArrHidden);		
		}
	}

	tvArr	= tvArrTot;
	tvArrHidden	= tvArrTotHidden;
}

void CARcBridgeDBStandard::DrawParabola(CDomyun *pDomP, CParabola *pPb)
{
	CHgBaseDrawStd baseDraw;
	baseDraw.DrawLineInArchByCatenary(pDomP, pPb->m_dDistStt, pPb->m_dDistEnd, pPb->m_dLenArchLine, pPb->m_dDistTotal, pPb->m_dHeightTotal, pPb->m_dFactorM, pPb->m_xyCrown);
}

void CARcBridgeDBStandard::DrawParabola(CDomyun *pDomP, CParabola *pPb, double dDistStt, double dDistEnd, double dOffset)
{
	CHgBaseDrawStd baseDraw;
	if(dOffset != 0)
	{
		CParabola pb;
		pPb->GetOffsetParabola(pb, dOffset);
		dDistStt	= pPb->GetXyPerByDistAndOffset(dDistStt, dOffset).x - pPb->m_xyCrown.x;
		dDistEnd	= pPb->GetXyPerByDistAndOffset(dDistEnd, dOffset).x - pPb->m_xyCrown.x;
		baseDraw.DrawLineInArchByCatenary(pDomP, dDistStt, dDistEnd, pb.m_dLenArchLine, pb.m_dDistTotal, pb.m_dHeightTotal, pb.m_dFactorM, pb.m_xyCrown);
	}
	else
	{
		baseDraw.DrawLineInArchByCatenary(pDomP, dDistStt, dDistEnd, pPb->m_dLenArchLine, pPb->m_dDistTotal, pPb->m_dHeightTotal, pPb->m_dFactorM, pPb->m_xyCrown);
	}
}


// �ڵ����� ġ���� �������ִ� �Լ� (cont ���)
// arrText�� ���� ġ�� ��� �� ���ڸ� ������ �� �� �ִ�.
// arrText�� ���ڴ� ġ���� �ε����� ���ؼ� ����, ġ�� ������ ġ���� ������� ���ڵ� �������.
// bLeftMargin : ġ���� �ϳ��϶� ������ ��� ���������� ġ���� �������� ���� ���������� ����..
// BOOL bInput : �Է¿� ġ�� ����(�Ʒ��ٿ� ������ ���� ġ���� �߰����ش�)
// double dApplyAngle : 90�� �ƴϰ� �Ϻ� �ؽ�Ʈ�� ������ �ڵ����� ��Ÿ� �����ؼ� �־� �ش�.
// BOOL bOrgIsVert : ���� ���� ���Ÿ�������, ��Ÿ� ������
// CString sMsgStt : ġ�� ���ۿ� ���� �޽���.
// pPb�϶� arr�� ���� �Ÿ���.
double CARcBridgeDBStandard::AutoDimCont(CDomyun *pDomP, CDPoint xyStt, CDoubleArray &arr, CStringArray &arrText, CStringArray &arrUnderText, long nDimDan, CString sDirection, double dMinMargin, BOOL bIsTotalDim, BOOL bRepetition, BOOL bEraseMinus, BOOL bLeftMargin, BOOL bInput, BOOL bIncludeSttInRepetition, BOOL bIncludeEndInRepetition, double dApplyAngle, BOOL bOrgIsVert, CString sMsgStt, CParabola *pPb, double dAccuracy)
{
	CDimParabola Dom(pDomP, pPb);

	Dom.SetDirection(sDirection);

	if(arr.GetSize() == 0) return 0;

// 	double dLen			= 0;
	CString sText		= _T("");
	CString sUnderText	= _T("");
	CString str			= _T("");
	BOOL bAssert		= FALSE;
	CDPoint xy			= xyStt;
	double dTotalLen	= 0;
	long nCount			= 1;
	long nSize			= 0;
	CString sEmptyText	= "0";
	BOOL bIsUnder		= FALSE;	// ���� ġ�����Խ� ġ���� �Ʒ��� ���ڸ� �����ߴ°�?


	CDoubleArray dArrUnderDim;
	BOOL bApplyAngle	= FALSE; 
	CDPoint vAng	= ToDPointFrDegree(dApplyAngle);
	if(arrUnderText.GetSize() == 0 && !Compare(dApplyAngle, 90.0, "="))
	{
		bApplyAngle	= TRUE;
		long nCount	= arr.GetSize();
		CString str	= _T("");
		long i = 0; for(i = 0; i < nCount; i++)
		{
			dArrUnderDim.Add(bOrgIsVert ? arr[i]/vAng.y : arr[i]*vAng.y);
		}
	}

	// ġ�����ڰ� ġ������ ������ ������ 0���� ä���ش�.
	// ��¥�� ġ�����ڸ� �̾ƿö� 0�̸� ���°����� �ν��Ѵ�.
	if(arrText.GetSize() < arr.GetSize())
	{
		long nCount = arr.GetSize() - arrText.GetSize();
		long i = 0; for(i = 0; i < nCount; i++)
			arrText.Add(sEmptyText);
	}

	// �Ʒ�ġ�����ڰ� ġ������ ������ ������ 0���� ä���ش�.
	// ��¥�� ġ�����ڸ� �̾ƿö� 0�̸� ���°����� �ν��Ѵ�.
	if(arrUnderText.GetSize() < arr.GetSize())
	{
		long nCount = arr.GetSize() - arrUnderText.GetSize();
		long i = 0; for(i = 0; i < nCount; i++)
			arrUnderText.Add(sEmptyText);
	}


	// ġ���� 0�ΰ��� ��� �����Ѵ�. (���� �ʿ� ����)
	long i = 0; for(i = 0; i < arr.GetSize(); i++)
	{
		if(Compare(arr.GetAt(i), 0.0, "=")) 
		{
			arr.RemoveAt(i, 1);
			arrText.RemoveAt(i, 1);
			arrUnderText.RemoveAt(i, 1);
			if(bApplyAngle)
				dArrUnderDim.RemoveAt(i, 1);
			i--;
		}
	}

	
	// ���̳ʽ��ΰ��� �μ��� ���� �����Ѵ�.
	// ������ ���̳ʽ��ΰ͸� ������ ������ �ٲٱ� ������ �ʿ��Ѱ��� �ƴ��� �Ǵ��ϱ� ���� 
	// ���̳ʽ��ΰ��� �����ϰ� ������ ���� ġ������ -��ŭ ����� �ȴ�.(�׷��� ��ġ���� ������ �Ȼ���.)
	if(bEraseMinus && arr.GetSize() > 0)
	{
		// ������ +�̸� -�ΰ��� �����ϰ� 
		// ������ -�̸� +�ΰ��� �����Ѵ�.
		BOOL bDeletePlus = arr.GetAt(0) < 0 ? TRUE : FALSE;
		long i = 0; for(i = 0; i < arr.GetSize(); i++)
		{
			if((bDeletePlus && Compare(arr.GetAt(i), 0.0, ">")) || (!bDeletePlus && Compare(arr.GetAt(i), 0.0, "<"))) 
			{
				double dDiff	= arr.GetAt(i);
				arr.RemoveAt(i, 1);
				arrText.RemoveAt(i, 1);
				arrUnderText.RemoveAt(i, 1);
				if(bApplyAngle)
					dArrUnderDim.RemoveAt(i, 1);
				i--;

				// -�� ������ ��� ���� ġ���� ������ �縸ŭ ���ų� ������� �ȴ�.
				if(i < arr.GetSize()-1)
				{
					double dLen	= arr.GetAt(i+1);
					if(bDeletePlus)
						dLen -= dDiff;
					else 
						dLen += dDiff;
					arr.SetAt(i+1, dLen);
					if(bApplyAngle)
						dArrUnderDim.SetAt(i+1, dLen/vAng.y);
				}
			}
		}
	}

	nSize = arr.GetSize();
	if(nSize == 0) return 0;

	// �ѱ��̸� �̸� ���� ���´�.
	for(i = 0; i < nSize; i++)
		dTotalLen += arr.GetAt(i);

	// ���� ġ���߿� ������ -�����̸�, DimLineToOver��뿡 ���Ǹ� ���Ѵ�.
	if(Compare(arr.GetAt(0), 0.0, "<"))
		bAssert = TRUE;

	// ���Ǹ� ���ϴ� ����̸� ġ���� �ݴ�� �����ϱ� ���� ���� ��ġ�� �ű��.
	// ���� �������� ��� �����´�.
	if(bAssert) 
	{
		if(sDirection == "TOP" || sDirection == "BOTTOM") xy.x += dTotalLen;
		else xy.y += dTotalLen;

		double *tmp = new double[nSize];
		CString *tmpText	= new CString[nSize];
		CString *tmpUnderText	= new CString[nSize];
		double *tmpUnder	= new double[nSize];

		long i = 0; for(i = 0; i < nSize; i++)
		{
			tmp[i] = fabs(arr.GetAt(nSize-1-i));
			tmpText[i] = arrText.GetAt(nSize-1-i);
			tmpUnderText[i] = arrUnderText.GetAt(nSize-1-i);
		}
		if(bApplyAngle)
		{
			long i = 0; for(i = 0; i < nSize; i++)
				tmpUnder[i]	= fabs(dArrUnderDim.GetAt(nSize-1-i));
		}

		arr.RemoveAll();
		arrText.RemoveAll();
		arrUnderText.RemoveAll();
		dArrUnderDim.RemoveAll();
		for(i = 0; i < nSize; i++)
		{
			arr.Add(tmp[i]);
			arrText.Add(tmpText[i]);
			arrUnderText.Add(tmpUnderText[i]);
		}
		if(bApplyAngle)
		{
			for(i = 0; i < nSize; i++)
				dArrUnderDim.Add(tmpUnder[i]);
		}

		delete [] tmp;
		delete [] tmpText;
		delete [] tmpUnderText;
		delete [] tmpUnder;
	}

	// �Է¿��̸� �Ʒ��ٿ� ��� ���� ġ���� ������ �ش�.
	if(bInput)
	{
		for(i = 0; i < nSize; i++)
		{
			arrUnderText.SetAt(i, COMMA(arr.GetAt(i)));
		}
	}


	// ������ ġ�� ���� �ϴ� �κ� ******************************************************/
	BOOL bInitMoveTo = FALSE;
//	double dLenUnder	= 0;
	CString strUnder	= _T("");
	BOOL bSttDim		= TRUE;
	double dLenAccu		= 0;
	for(i = 0; i < nSize; i++)
	{
		double dLen		= arr.GetAt(i);
		sText		= arrText.GetAt(i);
		sUnderText	= arrUnderText.GetAt(i);
		double dLenUnder	= bApplyAngle ? dArrUnderDim.GetAt(i) : 0;

		if(dLen == 0) continue;
		dLenAccu += dLen;

		// ġ�� ������ ���� �ؽ�Ʈ ������ /////////////////////////////
		// �ݺ��Ǵ� ġ���� �ִ����˻��Ѵ�.
		if((i < nSize-1) && bRepetition) 
		{
			if(!((i == 0 && !bIncludeSttInRepetition) || ((i == nSize-2) && !bIncludeEndInRepetition)))
			{
				if((COMMA(dLen) == COMMA(arr.GetAt(i+1))) || (fabs(dLen-arr.GetAt(i+1)) <= dAccuracy))
				//if(EQUAL(dLen, arr.GetAt(i+1), 0.001))
				{
					// �ݺ�ġ���� sArrText�� �ִٸ� ���ƾ߸� ������.
					if(sText == arrText.GetAt(i+1))
					{
						nCount++;
						continue;
					}
				}
			}
		}
		
		if(sText == sEmptyText)
		{
			if(nCount > 1)
			{
				if(dAccuracy != 0)
					str.Format("%d@%.2f=%s", nCount, dLenAccu/nCount, COMMA(dLenAccu));
				else
					str.Format("%d@%s=%s", nCount, COMMA(dLenAccu/nCount/*dLen*/), COMMA(dLenAccu));
				strUnder.Format("(%d@%s=%s)", nCount, COMMA(dLenUnder), COMMA(dLenUnder*nCount));
				if(!bApplyAngle)
				{
					if(sUnderText == sEmptyText && Dom.GetTextWidth(str) > dLenAccu)
					{
						if(dAccuracy != 0)
							str.Format("%d@%.2f", nCount, dLenAccu/nCount);
						else
							str.Format("%d@%s", nCount, COMMA(dLenAccu/nCount/*dLen*/));
						sUnderText.Format("=%s", COMMA(dLenAccu));

						if(!bInitMoveTo) 
						{
							str += sUnderText;
							sUnderText = "0";
						}
					}
				}
			}
			else
			{
				str = COMMA(dLen);
				strUnder.Format("(%s)", COMMA(dLenUnder));
			}
		}
		else
		{
			str = sText;
		}

		// ġ�� ���ڰ� "0"�̸� ���� �ʿ䰡 ����.
		// 0�� .������ �̷�� ���ٸ� 0�ΰ����� �ν�
		CString strTmp	= str;
		strTmp.Replace("0", "");
		strTmp.Replace(".", "");
		if(strTmp == "") continue;

		// ġ�� ���� ///////////////////////////////////////////////////
		// ġ���� �ּ� �������� ������ �Ʒ��� ���� ����.
		// �ּҸ����� -1�̸� �ڵ����� �Ǵ��Ѵ�.
		double dMinWidth = dMinMargin == -1 ? Dom.GetTextWidth(str) : dMinMargin;
		if(bApplyAngle)
			sUnderText	= strUnder;

		if(bSttDim && sMsgStt != "")
		{
			if(pPb)
			{
				Dom.DimMoveToParabola(pPb->GetDistByXPos(xy.x), nDimDan);
				Dom.DimLineToParabola(0, nDimDan, "");
			}
			else
			{
				Dom.DimMoveTo(xy.x, xy.y, nDimDan, sMsgStt);
				Dom.DimLineTo(0, nDimDan, "");
			}
			bSttDim	= FALSE;
		}

		if(fabs(dLen*nCount) <= dMinWidth && dMinMargin != 0)
		{
			if(i == 0)
			{
				if(!bInitMoveTo)
				{
					// ġ���� �ϳ��ۿ� ������ ���������� �������̶��..
					if(nSize-1 == i && !bLeftMargin)
					{
						if(pPb)
						{
							Dom.DimMoveToParabola(pPb->GetDistByXPos(xy.x), nDimDan);
							bInitMoveTo = TRUE;

							if(sUnderText == sEmptyText) sUnderText = "";
							Dom.DimLineToParabola(dLenAccu, nDimDan, str, sUnderText);
						}
						else
						{
							Dom.DimMoveTo(xy.x, xy.y, nDimDan);
							bInitMoveTo = TRUE;

							if(sUnderText == sEmptyText) sUnderText = "";
							Dom.DimLineToOverExtend(dLenAccu, nDimDan, str, sUnderText);
						}
					}
					else
					{
						if(pPb)
						{
							//Dom.DimMoveTo(xy.x, xy.y, nDimDan, str);
							if(sUnderText == sEmptyText) sUnderText = "";
							Dom.DimMoveToParabola(pPb->GetDistByXPos(xy.x), nDimDan, str, sUnderText);
							bInitMoveTo = TRUE;

							Dom.DimLineToParabola(dLenAccu, nDimDan, "");
						}
						else
						{
							//Dom.DimMoveTo(xy.x, xy.y, nDimDan, str);
							if(sUnderText == sEmptyText) sUnderText = "";
							Dom.DimMoveToExt(xy.x, xy.y, nDimDan, str, sUnderText);
							bInitMoveTo = TRUE;
						
							Dom.DimLineTo(dLenAccu, nDimDan, "");
						}
					}
				}
				else
				{
					if(pPb)
					{
						if(sUnderText == sEmptyText)
							Dom.DimLineToParabola(dLenAccu, nDimDan, "");
						else
							Dom.DimLineToParabola(dLenAccu, nDimDan, "", sUnderText);
					}
					else
					{
						if(sUnderText == sEmptyText)
							Dom.DimLineTo(dLenAccu, nDimDan, "");
						else
							Dom.DimLineToExtend(dLenAccu, nDimDan, "", sUnderText);
					}

					bIsUnder	= FALSE;
				}
			}
			else if(i == arr.GetSize()-1)
			{
				if(!bInitMoveTo) 
				{
					if(dLenAccu>Dom.GetTextWidth(str,2))
					{
						if(pPb)
						{
							Dom.DimMoveToParabola(pPb->GetDistByXPos(xy.x), nDimDan);
							Dom.DimLineToParabola(dLenAccu, nDimDan, str);
						}
						else
						{
							Dom.DimMoveTo(xy.x, xy.y, nDimDan);
							Dom.DimLineTo(dLenAccu, nDimDan, str);
						}
					}
					else
					{
						if(pPb)
						{
							Dom.DimMoveToParabola(pPb->GetDistByXPos(xy.x), nDimDan, str);
							if(sUnderText == sEmptyText) sUnderText = "";
							Dom.DimLineToParabola(dLenAccu, nDimDan, sUnderText);
						}
						else
						{
							Dom.DimMoveTo(xy.x, xy.y, nDimDan, str);
							if(sUnderText == sEmptyText) sUnderText = "";
							Dom.DimLineTo(dLenAccu, nDimDan, sUnderText);
						}
					}
					bInitMoveTo = TRUE;
				}
				else
				{
					if(pPb)
					{
						if(sUnderText == sEmptyText)
							Dom.DimLineToParabola(dLenAccu, nDimDan, str, "", TRUE);
						else
							Dom.DimLineToParabola(dLenAccu, nDimDan, str, sUnderText, TRUE);
					}
					else
					{
						if(sUnderText == sEmptyText)
							Dom.DimLineToOver(dLenAccu, nDimDan, str);
						else
							Dom.DimLineToOverExtend(dLenAccu, nDimDan, str, sUnderText);
					}
					bIsUnder	= FALSE;
				}
			}
			else
			{
				if(!bInitMoveTo) 
				{
					if(dLenAccu>Dom.GetTextWidth(str,2))
					{
						if(pPb)
						{
							Dom.DimMoveToParabola(pPb->GetDistByXPos(xy.x), nDimDan);
							Dom.DimLineToParabola(dLenAccu, nDimDan, str);
						}
						else
						{
							Dom.DimMoveTo(xy.x, xy.y, nDimDan);
							Dom.DimLineTo(dLenAccu, nDimDan, str);
						}
					}
					else
					{
						if(pPb)
						{
							Dom.DimMoveToParabola(pPb->GetDistByXPos(xy.x), nDimDan, str);
							if(sUnderText == sEmptyText) sUnderText = "";
							Dom.DimLineToParabola(dLenAccu, nDimDan, sUnderText);
						}
						else
						{
							Dom.DimMoveTo(xy.x, xy.y, nDimDan, str);
							if(sUnderText == sEmptyText) sUnderText = "";
							Dom.DimLineTo(dLenAccu, nDimDan, sUnderText);
						}
					}
					bInitMoveTo = TRUE;
				}
				else
				{
					if(sUnderText == sEmptyText)
					{
						if(pPb)
						{
							if(!bIsUnder)
								Dom.DimLineToParabola(dLenAccu, nDimDan, "", str);
							else
								Dom.DimLineToParabola(dLenAccu, nDimDan, str);
						}
						else
						{
							if(!bIsUnder)
								Dom.DimLineToExtend(dLenAccu, nDimDan, "", str);
							else
								Dom.DimLineTo(dLenAccu, nDimDan, str);
						}
					}
					else
					{
						if(pPb)
						{
							Dom.DimLineToParabola(dLenAccu, nDimDan, str, sUnderText);
						}
						else
						{
							Dom.DimLineToExtend(dLenAccu, nDimDan, str, sUnderText);
						}
					}
				}

				bIsUnder	= TRUE;
			}
		}
		else
		{
			if(!bInitMoveTo)
			{
				if(pPb)
				{
					Dom.DimMoveToParabola(pPb->GetDistByXPos(xy.x), nDimDan);
					bInitMoveTo = TRUE;

					if(sUnderText == sEmptyText)
						Dom.DimLineToParabola(dLenAccu, nDimDan, str);
					else
						Dom.DimLineToParabola(dLenAccu, nDimDan, str, sUnderText);
				}
				else
				{
					Dom.DimMoveTo(xy.x, xy.y, nDimDan);
					bInitMoveTo = TRUE;

					if(sUnderText == sEmptyText)
						Dom.DimLineTo(dLenAccu, nDimDan, str);
					else
						Dom.DimLineToExtend(dLenAccu, nDimDan, str, sUnderText);
				}
			}
			else
			{
				if(pPb)
				{
					if(sUnderText == sEmptyText)
						Dom.DimLineToParabola(dLenAccu, nDimDan, str);
					else
						Dom.DimLineToParabola(dLenAccu, nDimDan, str, sUnderText);
				}
				else
				{
					if(sUnderText == sEmptyText)
						Dom.DimLineTo(dLenAccu, nDimDan, str);
					else
						Dom.DimLineToExtend(dLenAccu, nDimDan, str, sUnderText);
				}

				bIsUnder	= FALSE;
			}
		}

		nCount		= 1;
		dLenAccu	= 0;
	}


	// ��ü ġ�� �����ϴ� ����̸�..
	if(bIsTotalDim)
	{
		str = COMMA(fabs(dTotalLen));
		strUnder.Format("(%s)", COMMA(fabs(dTotalLen/vAng.y)));
			
		if(str != "")
		{
			if(pPb)
			{
				Dom.DimMoveToParabola(pPb->GetDistByXPos(xyStt.x), nDimDan+1);
				if(bApplyAngle)
					Dom.DimLineToParabola(dTotalLen, nDimDan+1, str, strUnder);
				else
					Dom.DimLineToParabola(dTotalLen, nDimDan+1, str);
			}
			else
			{
				Dom.DimMoveTo(xyStt.x, xyStt.y, nDimDan+1);
				if(bApplyAngle)
					Dom.DimLineToExtend(dTotalLen, nDimDan+1, str, strUnder);
				else
					Dom.DimLineTo(dTotalLen, nDimDan+1, str);
			}
		}

	}

	// ****************************************************** ������ ġ�� ���� �ϴ� �κ� /

	*pDomP << Dom;

	return dTotalLen;
}

// ��ǥ���� �޾ƿͼ�.. �ڵ����� cont ġ���� �����Ѵ�.
// arrText�� ���� ġ�� ��� �� ���ڸ� ������ �� �� �ִ�.
// arrText�� ���ڴ� ġ���� �ε����� ���ؼ� ����, ġ�� ������ ġ���� ������� ���ڵ� �������.
// bAli : �밢�� ���� ��������? (�⺻�� ��������� ��)
// bInput : �Է�â ������ ġ������?(�̶��� sArrText���� ��ȣ�� ������.. sArrUnderText���� �ڵ����� ���̰��� ����.)
// double dApplyAngle : 90�� �ƴϰ� �Ϻ� �ؽ�Ʈ�� ������ �ڵ����� ��Ÿ� �����ؼ� �־� �ش�.
// BOOL bOrgIsVert : ���� ���� ���Ÿ�������, ��Ÿ� ������
// CString sMsgStt : ġ�� ���ۿ� ���� �޽���.
double CARcBridgeDBStandard::AutoDimCont(CDomyun *pDomP, CDPoint xyStt, CDPointArray &arr, CStringArray &arrText, CStringArray &arrUnderText, long nDimDan, CString sDirection, double dMinMargin, BOOL bIsTotalDim, BOOL bRepetition, BOOL bEraseMinus, BOOL bLeftMargin, BOOL bAli, BOOL bInput, BOOL bIncludeSttInRepetition, BOOL bIncludeEndInRepetition, double dApplyAngle, BOOL bOrgIsVert, CString sMsgStt, CParabola *pPb, double dAccuracy)
{
	// 
	if(arr.GetSize() < 2) return 0;
	double dLen = 0;
	CDoubleArray arrLen;
	BOOL bHor = sDirection == "TOP" || sDirection == "BOTTOM" ? TRUE : FALSE;
	if(pPb)
	{
		bHor	= TRUE;
	}
	if(bInput)
	{
		arrUnderText.RemoveAll();
		arrUnderText.Add("0");
	}

	// ���������� ����.
	long i = 0; for(i = 0; i < arr.GetSize()-1; i++)
	{
		if(pPb)
		{
			double dDistStt	= pPb->GetDistByXPos(arr.GetAt(i).x);
			double dDistEnd	= pPb->GetDistByXPos(arr.GetAt(i+1).x);
			dLen	= pPb->GetLenByDistToDist(dDistStt, dDistEnd);
		}
		else
		{
			if(bAli)
			{
				dLen = ~(arr.GetAt(i+1) - arr.GetAt(i));
			}
			else
			{
				if(bHor)
					dLen = arr.GetAt(i+1).x - arr.GetAt(i).x;
				else
					dLen = arr.GetAt(i+1).y - arr.GetAt(i).y;
			}
		}

		arrLen.Add(dLen);
		if(bInput) arrUnderText.Add(COMMA(dLen));
	}

	// ���� ������ ġ�� ���� 
	if(arrText.GetSize() > 0) arrText.RemoveAt(0);
	if(arrUnderText.GetSize() > 0) arrUnderText.RemoveAt(0);
	
	double dTot	= AutoDimCont(pDomP, xyStt, arrLen, arrText, arrUnderText, nDimDan, sDirection, dMinMargin, bIsTotalDim, bRepetition, bEraseMinus, bLeftMargin, bInput, bIncludeSttInRepetition, bIncludeEndInRepetition, dApplyAngle, bOrgIsVert, sMsgStt, pPb, dAccuracy);

	// ���� ���̷� ������ ��� ���⿡ ���� ȸ������ ��� �ȴ�.
	if(bAli && arr.GetSize() > 1)
	{
		double dAng = (arr.GetAt(arr.GetSize()-1)-arr.GetAt(0)).GetAngleDegree();
		dAng = bHor ? dAng : dAng - 90;
		CDPoint xyDir = ToDPointFrRadian(ToRadian(dAng));
		pDomP->Rotate(xyStt, xyDir);
	}

	return dTot;
}

// ��ü ���鵵(������) ǥ���� ����� ����� ���� �޶� �и��ؼ� �׷��� �Ǵ� ��츦 �Ǵ�.
// �׸��� ��ġ���� ��ü�� ��� �и��ؼ� �׸���.
BOOL CARcBridgeDBStandard::IsDivideFrontAndBackWall(long nJ)
{
	CRcBridgeApp *pBri	= NULL;
	if(m_pRcBridge)
		pBri	= m_pRcBridge;
	else if(m_pRcBridgeRebar)
		pBri	= m_pRcBridgeRebar;

	if(!pBri) return FALSE;
	if(m_pDrawingOptionData)
	{
		if(m_pDrawingOptionData->m_bSepFrontAndBackWallRebar) return TRUE;
	}

	double dStaLeft	= 0;
	double dStaRight	= 0;
	double dWidthLeft	= 0;
	double dWidthRight	= 0;

	CLineInfo *pLine	= pBri->GetLineBase();
	CDPoint vAng	= pBri->GetAngleJijum(nJ);
	double dSta	= pBri->GetStationOnJijum(nJ, 0);
	CDPoint vAngL	= vAng;
	CDPoint vAngR	= vAng;

	BOOL bSame	= TRUE;
	BOOL bVertWall	= TRUE;
	if(nJ == 0 || nJ == pBri->m_nQtyJigan)
	{
		BOOL bStt	= nJ == 0;
		dStaLeft	= pBri->GetStationByTarget(bStt ? BRIDGE_OFF_STT_SLAB : BRIDGE_OFF_END_END, 0, TRUE);
		dStaRight	= pBri->GetStationByTarget(bStt ? BRIDGE_OFF_STT_STT : BRIDGE_OFF_END_SLAB, 0, TRUE);
		vAngL		= pBri->GetAngleByTarget(bStt ? BRIDGE_OFF_STT_SLAB : BRIDGE_OFF_END_END, 0);
		vAngR		= pBri->GetAngleByTarget(bStt ? BRIDGE_OFF_STT_STT : BRIDGE_OFF_END_SLAB, 0);
	}
	else
	{
		CWallApp *pWall	= pBri->GetInWall(nJ - 1);
		if(!pWall) return FALSE;
		double dW	= pWall->m_dW;
		
		dStaLeft	= pLine->GetStationByOffsetWidth(dSta, -dW/2, vAng);
		dStaRight	= pLine->GetStationByOffsetWidth(dSta, dW/2, vAng);
		vAngL		= pLine->GetAngleByOffsetWidth(dSta, -dW/2, vAng);
		vAngR		= pLine->GetAngleByOffsetWidth(dSta, dW/2, vAng);

		bVertWall	= pWall->IsVertWall();
	}

	dWidthLeft	= pBri->GetWidthSlabActByGagak(dStaLeft, vAngL, 0);
	dWidthRight	= pBri->GetWidthSlabActByGagak(dStaRight, vAngR, 0);

	// (ARCBRIDGE-1180) 1mm ������ �����ɷ�... (��������� ��û)
	bSame	= Compare(dWidthLeft, dWidthRight, "=", 1.0);

	if(nJ != 0 && nJ != pBri->m_nQtyJigan && pBri->m_nTypeBridge == BRIDGE_TYPE_ARCH)
		bSame	= FALSE;

	return !bSame || !bVertWall;
}

void CARcBridgeDBStandard::SetDisplayAngLenStd(BOOL bDis)
{
	m_bDisplayAngLenStd	= bDis;
}


CString CARcBridgeDBStandard::GetStringTextDimFck(double dFck, double dAggregate, double dSlump)
{
	CString szRet = _T("");
	CARcBridgeDataStd *pStd	= m_pARcBridgeDataStd;

	dFck = GetValueUnitChange(dFck,UNIT_CGS_KGF_CM2,m_pARcBridgeDataStd->m_szKgf_CM2);

	if(m_nSelectFckDomText == 0)
	{
		if(pStd->IsSI())
			szRet.Format("\\A1;fck = %.0f MPa ", dFck); 
		else
			szRet.Format("\\A1;fck = %.0f kgf/cm{\\H0.6x;\\S2^ ;}", dFck); 
	}
	else
	{
		szRet.Format("\\A1;%.0f-%.0f-%.0f ", dAggregate, dFck, dSlump); 
	}

	return szRet;
}

void CARcBridgeDBStandard::SetSelectFckDomText(long nSelectFckDomText)
{
	m_nSelectFckDomText	= nSelectFckDomText;
}

BOOL CARcBridgeDBStandard::IsDrawUpperSlabRebar2DanAll()
{
	return (m_pDrawingOptionData)? m_pDrawingOptionData->m_bSepUpperSlab2DanAll : FALSE;
}
