// UbiARcBridgeData.cpp: implementation of the CUbiARcBridgeData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "arcbridge.h"
#include "../Src/ARcBridgeData/ARcBridgeData.h"
#include "UbiARcBridgeData.h"
#include <BRepOffsetAPI_MakePipeShell.hxx>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUbiARcBridgeData::CUbiARcBridgeData()
{
	m_nDrawKind		= -1;		// 0:전체교량, 1:선택교량.
	m_nDrawRebar	= -1;		// 0:배근생성안함, 1:배근생성.
	m_nDrawBridge	= -1;		// 선택교량일 경우의 교량 번호.

	m_pARcBridgeDataStd = NULL;
	m_sItemPathNameHeader = _T("");

	m_nIdxGroupRebar	= 0;
	m_nIdxRebarForAddEnableRebar	= 0;
	m_bBundleRebar	= TRUE;	// 묶음 철근으로 생성할지?
	m_dRateCreateRebar	= 1;	// 철근 생성 비율 (모두 하면 오래걸림)

	m_pMRD	= new CMakeRebarDetail;

	m_arrRebarBundle.RemoveAll();
	m_arrSolid.RemoveAll();
	m_arrRebar.RemoveAll();
}

CUbiARcBridgeData::~CUbiARcBridgeData()
{
	if(m_pMRD)
		delete m_pMRD;
	m_pMRD	= NULL;

	m_arrRebarBundle.RemoveAll();
	m_arrSolid.RemoveAll();
	m_arrRebar.RemoveAll();
}

UINT CUbiARcBridgeData::DoWork()
{
	if(m_pARcBridgeDataStd==NULL)	return 0;
	InitIdx(SOLID_UNKNOW);
	InitIdxRebar(REBAR_UNKNOW);
	if(m_nDrawKind==-1 || m_nDrawRebar==-1)	return 0;

	long nCountBri = m_pARcBridgeDataStd->GetBridgeSize();
	long bri = 0;
	// 프로그래스바 설정
	CString msg	= _T("");
	double dCountPos	= 0;
	double dOffsetPos	= 0;
	double dCurPos		= 0;
	for(bri = 0; bri < nCountBri; bri++)
	{
		if(m_nDrawKind==1 && m_nDrawBridge!=bri)	continue;
		dCountPos++;
	}
	if(m_nDrawRebar==1)		dCountPos	*= 5;
	else					dCountPos	*= 3;
	dOffsetPos	= 100.0 / dCountPos;

	msg = _T("객체생성");
	if(m_pDlgProgress)	m_pDlgProgress->SetProgressPos(msg, 1);

	// 구조물 level, 경사 옵션 적용 할지?
	BOOL bApplyLevelSlopeOption	= TRUE;

	BOOL bDrawFootingFront = FALSE;	
	for(bri = 0; bri < nCountBri; bri++)
	{
		if(m_nDrawKind==1 && m_nDrawBridge!=bri)	continue;

		CRcBridgeApp *pBri	= m_pARcBridgeDataStd->GetBridge(bri);
		if(pBri==NULL || pBri->m_nSeparBri!=0) continue;

		// 종단 실제 형태로 교량셋팅
		if(!bApplyLevelSlopeOption)
			pBri->GetTvVertSection_Std(0, TRUE, FALSE, FALSE, 2, FALSE);
		else
			pBri->SyncBridge(0, TRUE, TRUE, TRUE, FALSE);
		CRcBridgeRebar *pBriRebar = (CRcBridgeRebar *)pBri;


		m_sItemPathNameHeader = pBri->m_strBridgeName;
		CString sItemPathNameHeader	= m_sItemPathNameHeader;
		
		if(pBri->m_nTypeBridge==BRIDGE_TYPE_TRAP ||pBri->m_nTypeBridge==BRIDGE_TYPE_TRAP2 || pBri->m_nTypeBridge==BRIDGE_TYPE_ARCH)
			bDrawFootingFront = TRUE;
		long nCountJijum	= pBri->GetCountJijum();
		long nJijum = 0;
		for(nJijum = 0; nJijum < nCountJijum; nJijum++)
		{
			CFootingApp *pFooting	= pBri->GetFootingByJijumNumber(nJijum);
			if(!pFooting) continue;
			if(!pFooting->m_bIs) continue;
			if(pFooting->m_bVirtualFooting) continue;

			CString sCol	= _T("");
			CString sFooting	= _T("");
			CString sExFooting	= _T("");
			CString sPile	= _T("");
			sCol.Format(_T("/기둥%d"), nJijum+1);
			sFooting.Format(_T("/기초%d"), nJijum+1);
			sExFooting.Format(_T("/확대기초%d"), nJijum+1);
			sPile.Format(_T("/말뚝%d"), nJijum+1);

			m_sItemPathNameHeader	= sItemPathNameHeader + _T("/구조물") + sFooting;
			MakeRcFooting(pFooting, bDrawFootingFront);
			
			CExFootingApp *pExFoot = &pFooting->m_exFooting;
			m_sItemPathNameHeader = sItemPathNameHeader + _T("/구조물/확장기초");
			MakeRcExFooting(pExFoot, bDrawFootingFront);
			if(pExFoot->m_nType == EXFOOTING_TYPE_PILE)
			{
				m_sItemPathNameHeader = sItemPathNameHeader + _T("/구조물/말뚝");
				MakeRcPile(&pExFoot->m_Pile);
			}
			if(m_nDrawRebar==1)
			{
				pBriRebar->SyncBridgeRebar(TRUE, -1);
				m_sItemPathNameHeader	= sItemPathNameHeader + _T("/철근") + sFooting;
				MakeRcFootingRebar(pFooting, bDrawFootingFront);
			}
		}
		dCurPos += dOffsetPos;
		msg.Format(_T("%s 생성중.....\r\n"), pBri->m_strBridgeName);
		if(m_pDlgProgress)
			m_pDlgProgress->SetProgressPos(msg, dCurPos);

		m_sItemPathNameHeader = sItemPathNameHeader + _T("/구조물/기둥");
		MakeRcColumn(pBri);
		if(pBri->m_nTypeBridge!=BRIDGE_TYPE_TRAP && pBri->m_nTypeBridge!=BRIDGE_TYPE_PI)
		{
			m_sItemPathNameHeader = sItemPathNameHeader + _T("/구조물/벽체");
			MakeRcWall(pBri);
			
			m_sItemPathNameHeader = sItemPathNameHeader + _T("/구조물/브래킷");
			MakeRcBracket(pBri);
			m_sItemPathNameHeader = sItemPathNameHeader + _T("/구조물/벽체헌치");
			MakeRcHunch(pBri);
		}
		m_sItemPathNameHeader = sItemPathNameHeader + _T("/구조물/내측벽체헌치");
		MakeRcInnerHunch(pBri);

		CString str	= _T("");
		long nStt=0;
		for(nStt=0; nStt<2; nStt++)
		{
			for(int nLeft=0; nLeft<2; nLeft++)
			{
				BOOL bStt	= (nStt==iSTT);
				CApsApp* pAps = nStt==0 ? &pBri->m_apsStt[nLeft] : &pBri->m_apsEnd[nLeft];
				if(!pAps->m_bIs) continue;
				
				str.Format(_T("/구조물/%s접속슬래브"), bStt ? _T("시점") : _T("종점"));
				m_sItemPathNameHeader	= sItemPathNameHeader + str;
				MakeRcAps(pAps);
				
				for(int nCns=0; nCns<MAX_CNS; nCns++)
				{
					CApsApp* pCns = nStt==0 ? &pBri->m_cnsStt[nLeft][nCns] : &pBri->m_cnsEnd[nLeft][nCns];
					if(!pCns->m_bIs) continue;
					str.Format(_T("/구조물/%s완충슬래브%d"), bStt ? _T("시점") : _T("종점"), nCns+1);
					m_sItemPathNameHeader	= sItemPathNameHeader + str;
					MakeRcAps(pCns);
				}
			}
		}

		dCurPos += dOffsetPos;
		msg.Format(_T("%s 생성중.....\r\n"), pBri->m_strBridgeName);
		if(m_pDlgProgress)
			m_pDlgProgress->SetProgressPos(msg, dCurPos);

		m_sItemPathNameHeader = sItemPathNameHeader + _T("/구조물/슬래브");
		MakeRcSlab(pBri);
		m_sItemPathNameHeader = sItemPathNameHeader + _T("/구조물/슬래브 구조물");
		MakeRcSlabConstruct(pBri);

		if(m_nDrawRebar==1)
		{
			m_sItemPathNameHeader = sItemPathNameHeader + _T("/철근/기둥");
			MakeRcColumnRebar(pBri);
			m_sItemPathNameHeader = sItemPathNameHeader + _T("/철근/벽체헌치");
			MakeRcHunchRebar(pBri);
			m_sItemPathNameHeader = sItemPathNameHeader + _T("/철근/브래킷");
			MakeRcBracketRebar(pBri);
			m_sItemPathNameHeader = sItemPathNameHeader + _T("/철근/단부 주철근");
			MakeRcDanbuRebar_MainRebar(pBriRebar);
			if(pBri->m_nTypeBridge!=BRIDGE_TYPE_TRAP && pBri->m_nTypeBridge!=BRIDGE_TYPE_PI)
			{
				m_sItemPathNameHeader = sItemPathNameHeader + _T("/철근/벽체");
				MakeRcWallRebar(pBri);
			}
			for(nStt=0; nStt<2; nStt++)
			{
				for(int nLeft=0; nLeft<2; nLeft++)
				{
					BOOL bStt	= (nStt==iSTT);
					CApsApp* pAps = nStt==0 ? &pBri->m_apsStt[nLeft] : &pBri->m_apsEnd[nLeft];
					if(!pAps->m_bIs) continue;
					
					str.Format(_T("/철근/%s접속슬래브"), bStt ? _T("시점") : _T("종점"));
					m_sItemPathNameHeader	= sItemPathNameHeader + str;
					MakeRcApsRebar(pAps);
					
					for(int nCns=0; nCns<MAX_CNS; nCns++)
					{
						CApsApp* pCns = nStt==0 ? &pBri->m_cnsStt[nLeft][nCns] : &pBri->m_cnsEnd[nLeft][nCns];
						if(!pCns->m_bIs) continue;
						str.Format(_T("/철근/%s완충슬래브%d"), bStt ? _T("시점") : _T("종점"), nCns+1);
						m_sItemPathNameHeader	= sItemPathNameHeader + str;
						MakeRcApsRebar(pCns);
					}
				}
			}

			dCurPos += dOffsetPos;
			m_sItemPathNameHeader = sItemPathNameHeader + _T("/철근/슬래브");
			MakeRcSlabRebar(pBri, dCurPos);
/*			if(pBriRebar!=NULL)
			{
				CString sItemPathName	= m_sItemPathNameHeader;
				m_sItemPathNameHeader	= sItemPathName + _T("/슬래브 상면 주철근");
				MakeRcSlabRebar_MainRebar(pBriRebar);

				dCurPos += dOffsetPos;
				msg.Format(_T("%s 생성중.....\r\n"), pBri->m_strBridgeName);
				if(m_pDlgProgress)
					m_pDlgProgress->SetProgressPos(msg, dCurPos);

				m_sItemPathNameHeader	= sItemPathName + _T("/슬래브 상면 전단철근");
				MakeRcSlabRebar_ShearRebar(pBriRebar);

				m_sItemPathNameHeader	= sItemPathName + _T("/슬래브 상면 배력철근");
				MakeRcSlabRebar_SupportRebar(pBriRebar);
			}
*/
			dCurPos += dOffsetPos;
			msg.Format(_T("%s 생성중.....\r\n"), pBri->m_strBridgeName);
			if(m_pDlgProgress)
				m_pDlgProgress->SetProgressPos(msg, dCurPos);

			m_sItemPathNameHeader = sItemPathNameHeader + _T("/철근/슬래브 구조물");
			MakeRcSlabConstructRebar(pBri);
		}

		CreateAllSolid();

		dCurPos += dOffsetPos;
		msg.Format(_T("%s 생성중.....\r\n"), pBri->m_strBridgeName);
		if(m_pDlgProgress)
			m_pDlgProgress->SetProgressPos(msg, dCurPos);

		// 종단 실제 형태로 교량셋팅한거 복구
		if(!bApplyLevelSlopeOption)
			pBri->GetTvVertSection_Std(0, TRUE, FALSE, TRUE, 2, FALSE);

	}

	m_nDrawKind		= -1;
	m_nDrawRebar	= -1;
	m_nDrawBridge	= -1;
	msg = _T("");
	if(m_pDlgProgress)
		m_pDlgProgress->SetProgressPos(msg, 100);

	return 1;
}

// 생성된 데이터로  3D Solid Object 만들기.
// 최종적으로 호출되는 함수.
void CUbiARcBridgeData::CreateAllSolid()
{
	CMakeRebarJewon MRJ;
	
	long n=0;
	for(n = 0; n < m_arrRebarBundle.GetSize(); n++)
	{
//		m_pManage->AddRebarBundle(-1, m_arrRebarBundle[n]);
		m_pManage->AddEtt(m_arrRebarBundle[n]);

		//MRJ.CalcRebarJewon(m_arrRebarBundle[n], &pDrawingStd->m_arrRebar, "A", 1, pDrawingStd->m_arrRebar.GetSize());

		for(long i = 0; i < m_arrRebarBundle[n]->m_arrRebar.GetSize(); i++)
		{
			if(m_arrRebarBundle[n]->m_arrRebar.GetAt(i)->m_sMark == _T(""))
			{
				m_arrRebarBundle[n]->m_arrRebar.GetAt(i)->SetTrans(0.1);
				m_arrRebarBundle[n]->SetTrans(0.1);
			}
		}
	}
	
	for(n = 0; n < m_arrRebar.GetSize(); n++)
	{
//		m_pManage->AddRebar(-1, m_arrRebar[n]);
		m_arrRebar[n]->SetDrawColor( CVector(0.20, 0.20, 0.20) );
		m_pManage->AddEtt(m_arrRebar[n]);
		//m_arrRebar[n]->GetRange(m_pManage->m_cbMaxMin);
	}
}

void CUbiARcBridgeData::TrsVectorArray(CUbGeoCurveArray &edgeArr, const CTwinVectorArray &tvArr, TRS_PLANE trPln, double dExtraValue)
{
	GetVectorArrayFrTvArray(edgeArr, tvArr);
	TrsPlane(edgeArr, trPln, dExtraValue);
}

void CUbiARcBridgeData::TrsPlane(CUbGeoCurveArray &edgeArr, TRS_PLANE trPln, double dExtraValue)
{
	for(int n = 0; n < edgeArr.GetSize(); n++ )
	{
		CUbGeoCurve *pEdge	= &edgeArr.GetAt(n);

		TrsPlaneOne(pEdge->m_v1, trPln, dExtraValue);
		TrsPlaneOne(pEdge->m_v2, trPln, dExtraValue);
		TrsPlaneOne(pEdge->m_vCen, trPln, dExtraValue);
	}
}

void CUbiARcBridgeData::TrsPlaneOne(CVector &v1, TRS_PLANE trPln, double dExtraValue)
{
	CVector v2 = v1;
	
	switch(trPln)
	{
	case PLN_XOY :	break;
	case PLN_YOX :
		v1.y = v2.x;
		v1.x = v2.y;
		v1.z = dExtraValue;
		break;
	case PLN_XOZ : 
		v1.z = v2.y;
		v1.y = dExtraValue;
		break;
	case PLN_ZOX :
		v1.z = v2.x;
		v1.x = v2.y;
		v1.y = dExtraValue;
		break;
	case PLN_YOZ :
		v1.y = v2.x;
		v1.z = v2.y;
		v1.x = dExtraValue;
		break;
	case PLN_ZOY :
		v1.z = v2.x;
		v1.x = dExtraValue;
		break;
	}
}

// 변환한 벡터를 통해 페이스 얻기.
TopoDS_Face CUbiARcBridgeData::GetTopoDS_Face(CUbGeoCurveArray &edgeArr)
{
	TopoDS_Face face;
	try
	{
		if(edgeArr.GetSize() == 0) return face;
		face	= MakeFaceByEdgeArr(edgeArr);
	}
	catch(...)
	{
	}
	return face;
}

// edgeArr로 돌출solid객체를 만듬
// 기존 GetTopoDS_Shape와 다른 점은 아크를 포함한 것을 union이나 sub등으로 처리해줌.
TopoDS_Shape CUbiARcBridgeData::GetTopoDS_Shape(CUbGeoCurveArray &edgeArr, CVector vDir)
{
	TopoDS_Shape shp;
	try
	{
		if(edgeArr.GetSize() == 0) return shp;
		if(vDir == CVector(0, 0, 0)) return shp;

		TopoDS_Face face	= MakeFaceByEdgeArr(edgeArr);
		shp	= CUbEttSolid::GetTopoDS_Shape_Extrude(face, vDir);
			

	}
	catch(...)
	{
	}
	return shp;
}

// 좌표 변환
void CUbiARcBridgeData::GetVectorArrayFrTvArray(CUbGeoCurveArray &edgeArr, const CTwinVectorArray &tvArr)
{
	CTwinVectorArray tvArray;
	tvArray = tvArr;
	CRebarPlacing rb;
	rb.RemoveMinLengthLine(tvArray, 0);
	
	tvArray.Sort();

	rb.RemoveMeanlessTwinVector(tvArray);
	
	ConvertTwinVectorArrayToUbEdgeArray(tvArray, edgeArr);
}

// Solid 추가하기.
void CUbiARcBridgeData::AddSolid(CUbEttSolid *pSolid, long nType)
{
	if(!pSolid) return;

	// 색상 정의 
	CVector vRGB	= CVector(255, 255, 0);

	switch(nType)
	{
	case SOLID_UNKNOW : 
		vRGB	= CVector(255, 255, 0);		//기준위치 표식
		break;
	case SOLID_RCWALL : 
		vRGB	= CVector(246, 246, 246);	// 벽체
		break;
	case SOLID_SLAB : 
		vRGB	= CVector(246, 246, 246);	// 슬래브
		break;
	case SOLID_GUARDWALL : 
		vRGB	= CVector(94, 94, 94);
		break;
	}

	vRGB.x	= vRGB.x / 255.;
	vRGB.y	= vRGB.y / 255.;
	vRGB.z	= vRGB.z / 255.;
	////////////

	switch(nType)
	{
	case SOLID_UNKNOW : 
		pSolid->SetItemPathName(m_sItemPathNameHeader.IsEmpty() ? _T("") : m_sItemPathNameHeader);
		break;
	case SOLID_RCWALL : 
		pSolid->SetItemPathName(m_sItemPathNameHeader.IsEmpty() ? _T("벽체") : m_sItemPathNameHeader);
		break;
	case SOLID_SLAB :
		pSolid->SetItemPathName(m_sItemPathNameHeader.IsEmpty() ? _T("슬래브") : m_sItemPathNameHeader);
		break;
	case SOLID_GUARDWALL :
		pSolid->SetItemPathName(m_sItemPathNameHeader.IsEmpty() ? _T("슬래브 상부 구조물") : m_sItemPathNameHeader);
		break;
	}
	pSolid->m_nIndex = GetIdx(nType);
	pSolid->SetDrawColor(vRGB);

	m_pManage->AddEtt(pSolid);

	// ZoomAll() 처리를 위해.
	//pSolid->GetRange(m_pManage->m_cbMaxMin);
}

TopoDS_Face CUbiARcBridgeData::MakeFaceByEdgeArr(CUbGeoCurveArray &edgeArr)
{
	TopoDS_Face face;

	TopoDS_Wire wire;
	if(MakeWire(edgeArr, wire)==FALSE)	return face;
	face = BRepBuilderAPI_MakeFace(wire);

	return face;
}
// nType == SOLID_UNKNOW : 기준위치 표식
void CUbiARcBridgeData::InitIdx(long nType)
{
	m_nIdx	= 0;
	m_arrSolid.RemoveAll();
}

void CUbiARcBridgeData::InitIdxRebar(long nType)
{
	m_nIdxRebar	= 0;
	m_nIdxRebarForAddEnableRebar	= 0;
	m_nIdxGroupRebar				= 0;
	m_arrRebarBundle.RemoveAll();
	m_arrRebar.RemoveAll();
}

long CUbiARcBridgeData::GetIdx(long nType)
{
	long nIdx	= m_nIdx;
	m_nIdx++;
	return nIdx;
}

long CUbiARcBridgeData::GetIdxRebar(long nType)
{
	long nIdx	= m_nIdxRebar;
	m_nIdxRebar++;
	return nIdx;
}

long CUbiARcBridgeData::GetIdxGroupRebar()
{
	return m_nIdxGroupRebar;
}

void CUbiARcBridgeData::IncreaseIdxGroupRebar()
{
	m_nIdxGroupRebar++;
}

// edgeArr로 wire 만들기
BOOL CUbiARcBridgeData::MakeWire(CUbGeoCurveArray &edgeArr, TopoDS_Wire &wire)
{
	BRepBuilderAPI_MakeWire makeWire;
//	TopoDS_Wire wire;

	if(edgeArr.GetSize() == 0) return FALSE;

	CUbGeoCurve *pEdge;

	gp_Pnt aPnt1, aPnt2, aPntMid;
	gp_Pnt aPntLast;
	BOOL bFirst	= TRUE;
	for(long i = 0; i < edgeArr.GetSize(); i++)
	{
		pEdge	= &edgeArr.GetAt(i);
		if(pEdge->m_v1 == pEdge->m_v2) continue;
		
		aPnt1.SetX(pEdge->m_v1.x);
		aPnt1.SetY(pEdge->m_v1.y);
		aPnt1.SetZ(pEdge->m_v1.z);

		aPnt2.SetX(pEdge->m_v2.x);
		aPnt2.SetY(pEdge->m_v2.y);
		aPnt2.SetZ(pEdge->m_v2.z);

		if(pEdge->m_nType == CUbGeoCurve::LINE)
		{
			Handle(Geom_TrimmedCurve) aSeg	= GC_MakeSegment(aPnt1, aPnt2);
			TopoDS_Edge aEdge	= BRepBuilderAPI_MakeEdge(aSeg);
			if(bFirst)
				makeWire	= BRepBuilderAPI_MakeWire(aEdge);
			else
				makeWire.Add(aEdge);
		}
		else
		{
			CVector vMid	= pEdge->GetVecMid();
			aPntMid.SetX(vMid.x);
			aPntMid.SetY(vMid.y);
			aPntMid.SetZ(vMid.z);

			Handle(Geom_TrimmedCurve) aArcOfCircle	= GC_MakeArcOfCircle(aPnt1, aPntMid, aPnt2);
			TopoDS_Edge aEdge	= BRepBuilderAPI_MakeEdge(aArcOfCircle);

			if(bFirst)
				makeWire	= BRepBuilderAPI_MakeWire(aEdge);
			else
				makeWire.Add(aEdge);
		}

		
		bFirst	= FALSE;
		aPntLast	= aPnt2;
	}

	try
	{
		wire = makeWire.Wire();
	}
	catch(...)
	{
		return FALSE;
	}

	return TRUE;
}

void CUbiARcBridgeData::ConvertTwinVectorArrayToUbEdgeArray(CTwinVectorArray &tvArr, CUbGeoCurveArray &edgeArr)
{
	edgeArr.RemoveAll();
	CUbGeoCurve edge;
	for(long i = 0; i < tvArr.GetSize(); i++)
	{
		ConvertTwinVectorToUbEdge(&tvArr.GetAt(i), &edge);
		edgeArr.Add(edge);
	}

}

void CUbiARcBridgeData::ConvertTwinVectorToUbEdge(CTwinVector *pTv, CUbGeoCurve *pEdge)
{
	if(!pTv || !pEdge) return;


	if(pTv->m_v1.z == -1 || pTv->m_v1.z == 1)
	{
		pEdge->SetEdge(CDPoint(pTv->m_v1), CDPoint(pTv->m_v2), CDPoint(pTv->GetXyCircleCenter()));		
	}
	else
	{
		pEdge->SetEdge(CDPoint(pTv->m_v1), CDPoint(pTv->m_v2));
	}

	pEdge->m_sInfo	= pTv->m_sInfo;

}

void CUbiARcBridgeData::MakeRcFooting(CFootingApp *pFooting, BOOL bDrawFootingFront)
{
	if(!pFooting) return;
	if(!pFooting->m_bIs) return;
	if(pFooting->m_bVirtualFooting) return;

	pFooting->GetTvPlan();
	CTwinVector tvPathPlan	= pFooting->m_tvPathPlan;

	TopoDS_Shape shp;
	// 정면도가 경사진 경우
	if(pFooting->IsSlopeFront(TRUE) || pFooting->IsSlopeFront(FALSE) || bDrawFootingFront)
	{
		CTwinVectorArray tvArrFootingFront;
		//pFooting->GetTvFront(FALSE, 0, FALSE);
		tvArrFootingFront	= pFooting->m_tvArrFront;

		// 구해진 정면도 좌표를 벡터로 변환
		CUbGeoCurveArray edgeArr;
		TrsVectorArray(edgeArr, tvArrFootingFront, PLN_XOY, 0);
		
		// 실제 위치로 이동
		ModifyFrontToPlanFooting(pFooting, edgeArr);
		
		// 변환한 벡터를 돌출
		CVector vDir	= tvPathPlan.GetXyDir();
		CTwinVectorArray tvArrPlan;
		pFooting->GetTvPlanByRectAngle(tvArrPlan);
		vDir = vDir * tvArrPlan.GetTvByInfo(_T("좌")).GetLength();	// 가장 길게 만들어서 잘라 내야 됨.
		shp	= GetTopoDS_Shape(edgeArr, vDir);	

		// 솔리드 객체로 등록
//		AddSolid(&shp, SOLID_FOOTING, m_sItemPathNameHeader);
	}
	else
	{	
		CTwinVectorArray tvArrFootingSide, tvArrHidden;
		pFooting->GetTvLeftSide(tvArrFootingSide, tvArrHidden, TRUE);

		// 구해진 정면도 좌표를 벡터로 변환
		CUbGeoCurveArray edgeArr;
		TrsVectorArray(edgeArr, tvArrFootingSide, PLN_XOY, 0);
		
		// 실제 위치로 이동
		ModifySideToPlanFooting(pFooting, edgeArr);
		
		// 변환한 벡터를 돌출
		CVector vDir	= tvPathPlan.GetXyDir().Rotate90();
		vDir = vDir * pFooting->GetWidth();
		shp	= GetTopoDS_Shape(edgeArr, vDir);	
	}

	// 기초 평면에 스큐적용을 위해서 Common  //////////////////////////////////////////////////////////////////////////
	CTwinVectorArray tvArrPlan;
	CTwinVectorArray tvArrPlanRect;
	tvArrPlan	= pFooting->m_tvArrplan;
	pFooting->GetTvPlanByRectAngle(tvArrPlanRect);
	CTwinVector tvLeft, tvRight, tvLeftRect, tvRightRect;
	tvLeft	= tvArrPlan.GetTvByInfo(_T("기초평면좌측1"));
	tvRight	= tvArrPlan.GetTvByInfo(_T("기초평면우측1"));
	tvLeftRect	= tvArrPlanRect.GetTvByInfo(_T("상"));
	tvRightRect	= tvArrPlanRect.GetTvByInfo(_T("하"));
	if(!Compare(tvLeft.GetLength(), tvLeftRect.GetLength(), _T("=")) ||
		!Compare(tvRight.GetLength(), tvRightRect.GetLength(), _T("=")))
	{
		for(long i = 0; i < tvArrPlan.GetSize(); i++)
		{
			if(tvArrPlan.GetAt(i).m_sInfo.Find(_T("기초평면경사"), 0) != -1)
			{
				tvArrPlan.RemoveAt(i);
				i--;
			}
		}

		// 구해진 정면도 좌표를 벡터로 변환
		CUbGeoCurveArray edgeArr;
		tvArrPlan.Sort();
		TrsVectorArray(edgeArr, tvArrPlan, PLN_XOY, 0);

		// 실제 위치로 이동
		edgeArr.Move(CVector(0, 0, pFooting->m_dEL));

		// 변환한 벡터를 돌출
		CVector vDir	= CVector(0, 0, -1);
		vDir	= vDir * pFooting->GetHeight(TRUE);
		TopoDS_Shape shpCommon	= GetTopoDS_Shape(edgeArr, vDir);
		
		shp	= BRepAlgoAPI_Common(shp, shpCommon);
	}
	//////////////////////////////////////////////////////////////////////////
	

	// 솔리드 객체로 등록
	AddSolid(&shp, SOLID_FOOTING, m_sItemPathNameHeader);
}

void CUbiARcBridgeData::AddSolid(TopoDS_Shape *pShp, long nType, CString sItemPathName)
{
	if(!pShp) return;

	// 색상 정의 
	CVector vRGB	= CVector(255, 255, 0);
	switch(nType)
	{
	case SOLID_UNKNOW : 
		vRGB	= CVector(255, 255, 0);		//기준위치 표식
		break;
	case SOLID_COPPING :
		vRGB	= CVector(162, 212, 65);	//두부보
		break;
	case SOLID_COLUMN : 
		vRGB	= CVector(246, 246, 246);//CVector(132, 189, 22);	//기둥
		break;
	case SOLID_FOOTING : 
		vRGB	= CVector(170, 157, 247);//m_bMakingAbut ? CVector(170, 157, 247) : CVector(159, 209, 60);	//기초
		break;
/*	case SOLID_BEARING :
		vRGB	= CVector(0, 255, 0);	// 베어링
		break;
*/	case SOLID_PILE : 
		vRGB	= CVector(125, 0, 0);	//말뚝
		break;
	case SOLID_EXFOOTING : 
		vRGB	= CVector(103, 86, 202);//m_bMakingAbut ? CVector(103, 86, 202) : CVector(88, 130, 5);	//확대기초
		break;
	case SOLID_RCWALL : 
		vRGB	= CVector(246, 246, 246);	// 벽체
		break;
/*	case SOLID_SLAB : 
		vRGB	= CVector(246, 246, 246);	// 슬래브
		break;
	case SOLID_GUARDWALL : 
		vRGB	= CVector(94, 94, 94);
		break;
*/	case SOLID_WINGWALL : 
		vRGB	= CVector(170, 157, 247);	// 교대 날개벽
		break;
	case SOLID_APS : 
		vRGB	= CVector(246, 246, 246);	//(93, 187, 241);	//접속슬래브(완충슬래브)
		break;
	}

	vRGB.x	= vRGB.x / 255.;
	vRGB.y	= vRGB.y / 255.;
	vRGB.z	= vRGB.z / 255.;
	////////////

	CUbEttSolid* pSolid = NULL;
	if(nType == SOLID_UNKNOW)
	{
		pSolid = new CUbEttSolid(*pShp, UNIT_MM);
		pSolid->SetItemPathName(sItemPathName.IsEmpty() ? _T("제목없음") : sItemPathName);
		pSolid->m_nIndex = GetIdx(nType);
		pSolid->SetDrawColor(vRGB);
	}
	else if(nType == SOLID_COPPING)
	{
		pSolid	= new CUbEttSolid(*pShp, UNIT_MM);
		pSolid->SetItemPathName(sItemPathName.IsEmpty() ? _T("두부보") : sItemPathName);
		pSolid->m_nIndex = GetIdx(nType);
		pSolid->SetDrawColor(vRGB);
	}
	else if(nType == SOLID_COLUMN)
	{
		pSolid = new CUbEttSolid(*pShp, UNIT_MM);
		pSolid->SetItemPathName(sItemPathName.IsEmpty() ? _T("기둥") : sItemPathName);
		pSolid->m_nIndex = GetIdx(nType);
		pSolid->SetDrawColor(vRGB);
	}
	else if(nType == SOLID_FOOTING)
	{
		pSolid	= new CUbEttSolid(*pShp, UNIT_MM);
		pSolid->SetItemPathName(sItemPathName.IsEmpty() ? _T("기초") : sItemPathName);
		pSolid->m_nIndex = GetIdx(nType);
		pSolid->SetDrawColor(vRGB);
	}
/*	else if(nType == SOLID_BEARING)
	{
		pSolid	= new CUbEttSolid(*pShp, UNIT_MM);
		pSolid->SetItemPathName(sItemPathName.IsEmpty() ? _T("베어링") : sItemPathName);
		pSolid->m_nIndex = GetIdx(nType);
		pSolid->SetDrawColor(vRGB);
	}
*/	else if(nType == SOLID_PILE)
	{
		pSolid	= new CUbEttSolid(*pShp, UNIT_MM);
		pSolid->SetItemPathName(sItemPathName.IsEmpty() ? _T("말뚝") : sItemPathName);
		pSolid->m_nIndex = GetIdx(nType);
		pSolid->SetDrawColor(vRGB);
	}
	else if(nType == SOLID_EXFOOTING)
	{
		pSolid	= new CUbEttSolid(*pShp, UNIT_MM);
		pSolid->SetItemPathName(sItemPathName.IsEmpty() ? _T("확대기초") : sItemPathName);
		pSolid->m_nIndex = GetIdx(nType);
		pSolid->SetDrawColor(vRGB);
	}
	else if(nType == SOLID_RCWALL)
	{
		pSolid	= new CUbEttSolid(*pShp, UNIT_MM);
		pSolid->SetItemPathName(sItemPathName.IsEmpty() ? _T("벽체") : sItemPathName);
		pSolid->m_nIndex = GetIdx(nType);
		pSolid->SetDrawColor(vRGB);
	}
/*	else if(nType == SOLID_SLAB)
	{
		pSolid	= new CUbEttSolid(*pShp, UNIT_MM);
		pSolid->SetItemPathName(sItemPathName.IsEmpty() ? _T("슬래브") : sItemPathName);
		pSolid->m_nIndex = GetIdx(nType);
		pSolid->SetDrawColor(vRGB);
	}
	else if(nType == SOLID_GUARDWALL)
	{
		pSolid	= new CUbEttSolid(*pShp, UNIT_MM);
		pSolid->SetItemPathName(sItemPathName.IsEmpty() ? _T("슬래브 상부 구조물") : sItemPathName);
		pSolid->m_nIndex = GetIdx(nType);
		pSolid->SetDrawColor(vRGB);
	}
*/	else if(nType == SOLID_WINGWALL)
	{
		pSolid	= new CUbEttSolid(*pShp, UNIT_MM);
		pSolid->SetItemPathName(sItemPathName.IsEmpty() ? _T("날개벽") : sItemPathName);
		pSolid->m_nIndex = GetIdx(nType);
		pSolid->SetDrawColor(vRGB);
	}
	else if(nType == SOLID_APS)
	{
		pSolid	= new CUbEttSolid(*pShp, UNIT_MM);
		pSolid->SetItemPathName(sItemPathName.IsEmpty() ? _T("접속슬래브") : sItemPathName);
		pSolid->m_nIndex = GetIdx(nType);
		pSolid->SetDrawColor(vRGB);
	}
	else
		return;


	m_pManage->AddEtt(pSolid);

	// ZoomAll() 처리를 위해.
	//pSolid->GetRange(m_pManage->m_cbMaxMin);
}

// 기초 정면을 평면으로 옮김
// 기초각도가 90보다 작으면 기초상면 좌측을 기준으로 옮기고, 반대라면 기초상면 우측을 기준으로 옮긴다.
void CUbiARcBridgeData::ModifyFrontToPlanFooting(CFootingApp *pFooting, CUbGeoCurveArray &edgeArr)
{
	TrsPlane(edgeArr, PLN_XOZ, 0);
	BOOL bOrgLeft	= pFooting->m_dAngle <= 90 ? TRUE : FALSE;

	// 기초 상면 좌측을 기준으로 함
	//pFooting->GetTvFront(FALSE, 0, FALSE);
	CDRect rect	= pFooting->m_tvArrFront.GetRect();
	CVector vOrg	= CVector(bOrgLeft ? rect.left : rect.right, rect.top, 0);
	TrsPlaneOne(vOrg, PLN_XOZ, 0);

	// vOrg에서 실제 기초 평면 평면 우측1 좌측까지 거리
	CTwinVectorArray tvArrPlan;
	pFooting->GetTvPlanByRectAngle(tvArrPlan);
//	tvArrPlan = pFooting->m_tvArrplan;
	CDPoint xyOrg	= vOrg;
	CDPoint xyPlan	= bOrgLeft ? tvArrPlan.GetTvByInfo(_T("하")).m_v1 : tvArrPlan.GetTvByInfo(_T("하")).m_v2;
//	CDPoint xyDir = bOrgLeft ? -tvArrPlan.GetTvByInfo(_T("하")).GetXyDir() : tvArrPlan.GetTvByInfo(_T("하")).GetXyDir();
//	CVector vMove	= xyPlan - xyOrg + xyDir*200;
	CVector vMove	= xyPlan - xyOrg;
	edgeArr.Move(vMove);

	// 회전
	CDPoint vAngPlan	= tvArrPlan.GetTvByInfo(_T("좌")).GetXyDir(TRUE);
	CDPoint vAngOrg		= CDPoint(0, 1);
	CDPoint vAngRot		= ToDPointFrDegree(vAngPlan.GetAngleDegree() - vAngOrg.GetAngleDegree());
	edgeArr.Rotate(xyPlan, vAngRot);
}

// 기초 측면을 평면으로 옮김
void CUbiARcBridgeData::ModifySideToPlanFooting(CFootingApp *pFooting, CUbGeoCurveArray &edgeArr)
{
	TrsPlane(edgeArr, PLN_YOZ, 0);
	TrsPlane(edgeArr, PLN_XOY, 0);

	// 기초 상면 좌측을 기준으로 함
	CTwinVectorArray tvArrSide, tvArrHidden;
	pFooting->GetTvLeftSide(tvArrSide, tvArrHidden, TRUE);
	CDRect rect	= tvArrSide.GetRect();
	CVector vOrg	= CVector(rect.left, rect.top, rect.top);
	TrsPlaneOne(vOrg, PLN_YOZ, 0);
	TrsPlaneOne(vOrg, PLN_XOY, 0);

	// vOrg에서 실제 평면 두부보 평면 하면 좌측까지 거리
	CTwinVectorArray tvArrPlan;
	pFooting->GetTvPlanByRectAngle(tvArrPlan);

	CDPoint xyOrg	= vOrg;
	CDPoint xyPlan	= tvArrPlan.GetTvByInfo(_T("상")).m_v1;
	CVector vMove	= xyPlan - xyOrg;
	edgeArr.Move(vMove);

	// 회전
	CDPoint vAngPlan	= tvArrPlan.GetTvByInfo(_T("우")).GetXyDir(TRUE);
	CDPoint vAngOrg		= CDPoint(0, 1);
	CDPoint vAngRot		= ToDPointFrDegree(vAngPlan.GetAngleDegree() - vAngOrg.GetAngleDegree());
	edgeArr.Rotate(xyPlan, vAngRot);
}

// 확대기초
void CUbiARcBridgeData::MakeRcExFooting(CExFootingApp *pExFooting, BOOL bDrawFootingFront)
{
	if(!pExFooting) return;
	if(!pExFooting->m_bIs) return;

	pExFooting->GetTvPlan();
	CTwinVector tvPathPlan	= pExFooting->m_tvPathPlan;
	
	TopoDS_Shape shp;

	CTwinVectorArray tvArrPlanRect;
	pExFooting->GetTvPlanByRectAngle(tvArrPlanRect);
	CTwinVector tvLeftRect	= tvArrPlanRect.GetTvByInfo(_T("상"));
	CTwinVector tvRightRect	= tvArrPlanRect.GetTvByInfo(_T("하"));

	// 정면도가 경사진 경우
	if(pExFooting->m_nType == EXFOOTING_TYPE_MASS && pExFooting->m_bFrontDanMass || bDrawFootingFront)
	{
		//pExFooting->GetTvFront(FALSE, FALSE);
		CTwinVectorArray tvArrFront;
		tvArrFront	= pExFooting->m_tvArrFront;

		// 구해진 정면도 좌표를 벡터로 변환
		CUbGeoCurveArray edgeArr;
		TrsVectorArray(edgeArr, tvArrFront, PLN_XOY, 0);
		
		// 실제 위치로 이동
		ModifyFrontToPlanExFooting(pExFooting, edgeArr);
		
		// 변환한 벡터를 돌출
		CVector vDir	= tvPathPlan.GetXyDir();
		CTwinVectorArray tvArrPlan;
		pExFooting->GetTvPlanByRectAngle(tvArrPlan);
		vDir = vDir * tvArrPlan.GetTvByInfo(_T("좌")).GetLength();	// 가장 길게 만들어서 잘라 내야 됨.
		
		shp	= GetTopoDS_Shape(edgeArr, vDir);	
	}
	else
	{	
		//pExFooting->GetTvFront(FALSE, TRUE, TRUE, FALSE, TRUE, TRUE);
		CTwinVectorArray tvArrSide;
		tvArrSide	= pExFooting->m_tvArrSide;
		
		// 구해진 정면도 좌표를 벡터로 변환
		CUbGeoCurveArray edgeArr;
		TrsVectorArray(edgeArr, tvArrSide, PLN_XOY, 0);
		
		// 실제 위치로 이동
		ModifySideToPlanExFooting(pExFooting, edgeArr);
		
		// 변환한 벡터를 돌출
		CVector vDir	= tvPathPlan.GetXyDir().Rotate90();
		vDir = vDir * pExFooting->GetWidth();

		shp	= GetTopoDS_Shape(edgeArr, vDir);	
	}

	// 기초 평면에 스큐적용을 위해서 Common  //////////////////////////////////////////////////////////////////////////
	CTwinVectorArray tvArrPlan;
	
	tvArrPlan	= pExFooting->m_tvArrPlan;
	pExFooting->GetTvPlanByRectAngle(tvArrPlanRect);

	CTwinVector tvLeft, tvRight;
	tvLeft	= tvArrPlan.GetTvByInfo(_T("확대기초평면좌측1"));
	tvRight	= tvArrPlan.GetTvByInfo(_T("확대기초평면우측1"));
	
	if(!Compare(tvLeft.GetLength(), tvLeftRect.GetLength(), _T("=")) ||
		!Compare(tvRight.GetLength(), tvRightRect.GetLength(), _T("=")))
	{
		// 구해진 정면도 좌표를 벡터로 변환
		CUbGeoCurveArray edgeArr;
		TrsVectorArray(edgeArr, tvArrPlan, PLN_XOY, 0);

		// 실제 위치로 이동
		edgeArr.Move(CVector(0, 0, pExFooting->m_dEL));

		// 변환한 벡터를 돌출
		CVector vDir	= CVector(0, 0, -1);
		vDir	= vDir * pExFooting->m_dEFHR;
		TopoDS_Shape shpCommon	= GetTopoDS_Shape(edgeArr, vDir);
		
		shp	= shpCommon;//BRepAlgoAPI_Common(shp, shpCommon);
	}
	//////////////////////////////////////////////////////////////////////////

	// 솔리드 객체로 등록
	AddSolid(&shp, SOLID_EXFOOTING, m_sItemPathNameHeader);
}

void CUbiARcBridgeData::ModifyFrontToPlanExFooting(CExFootingApp *pExFooting, CUbGeoCurveArray &edgeArr)
{
	TrsPlane(edgeArr, PLN_XOZ, 0);

	// 기초 상면 좌측을 기준으로 함
	CDRect rect	= pExFooting->m_tvArrFront.GetRect();
	CVector vOrg	= CVector(rect.left, rect.top, 0);
	TrsPlaneOne(vOrg, PLN_XOZ, 0);

	// vOrg에서 실제 기초 평면 평면 우측1 좌측까지 거리
	CTwinVectorArray tvArrPlan;
	pExFooting->GetTvPlanByRectAngle(tvArrPlan);

	CDPoint xyOrg	= vOrg;
	CDPoint xyPlan	= tvArrPlan.GetTvByInfo(_T("하")).m_v1;
	CVector vMove	= xyPlan - xyOrg;
	edgeArr.Move(vMove);

	// 회전
	CDPoint vAngPlan	= tvArrPlan.GetTvByInfo(_T("좌")).GetXyDir(TRUE);
	CDPoint vAngOrg		= CDPoint(0, 1);
	CDPoint vAngRot		= ToDPointFrDegree(vAngPlan.GetAngleDegree() - vAngOrg.GetAngleDegree());
	edgeArr.Rotate(xyPlan, vAngRot);
}


void CUbiARcBridgeData::ModifySideToPlanExFooting(CExFootingApp *pExFooting, CUbGeoCurveArray &edgeArr)
{
	TrsPlane(edgeArr, PLN_YOZ, 0);
	TrsPlane(edgeArr, PLN_XOY, 0);

	// 기초 상면 좌측을 기준으로 함
	CTwinVectorArray tvArrSide;
	tvArrSide	= pExFooting->m_tvArrSide;
	CDRect rect	= tvArrSide.GetRect();
	CVector vOrg	= CVector(rect.left, rect.top, rect.top);
	TrsPlaneOne(vOrg, PLN_YOZ, 0);
	TrsPlaneOne(vOrg, PLN_XOY, 0);

	// vOrg에서 실제 평면 두부보 평면 하면 좌측까지 거리
	CTwinVectorArray tvArrPlan;
	pExFooting->GetTvPlanByRectAngle(tvArrPlan);

	CDPoint xyOrg	= vOrg;
	CDPoint xyPlan	= tvArrPlan.GetTvByInfo(_T("상")).m_v1;
	CVector vMove	= xyPlan - xyOrg;
	edgeArr.Move(vMove);

	// 회전
	CDPoint vAngPlan	= tvArrPlan.GetTvByInfo(_T("우")).GetXyDir(TRUE);
	CDPoint vAngOrg		= CDPoint(0, 1);
	CDPoint vAngRot		= ToDPointFrDegree(vAngPlan.GetAngleDegree() - vAngOrg.GetAngleDegree());
	edgeArr.Rotate(xyPlan, vAngRot);
}

void CUbiARcBridgeData::MakeRcColumn(CRcBridgeApp *pBri)
{
	if(pBri==NULL)	return;
	long nCountWall = pBri->GetCountInWall();
	long i = 0;
	for(i = 0; i < nCountWall; i++)
	{
		CWallApp *pWall = pBri->GetInWall(i);
		if(pWall==NULL)	continue;
		
		if(pWall->m_bTopIsColumn || pWall->m_bRoundRect)		// 벽체가 기둥식 or 라운드식.
		{
			MakeRcWallTopIsColumn(pBri, i);
		}
		else
		{
			TopoDS_Shape shp;
			CUbGeoCurveArray edgeArr;
/*			if(pBri->m_nTypeBridge!=BRIDGE_TYPE_TRAP && pBri->m_nTypeBridge!=BRIDGE_TYPE_TRAP2 && pBri->m_nTypeBridge!=BRIDGE_TYPE_ARCH)
			{
				TrsVectorArray(edgeArr, pWall->m_tvArrPlan, PLN_XOY, 0);
				edgeArr.Move(CVector(0, 0, pWall->m_dELB+pWall->m_footing.m_dFH));
				CVector vDir	= CVector(0, 0, pWall->m_tvArrFront.GetVerLength());
				shp	= GetTopoDS_Shape(edgeArr, vDir);
				// 솔리드 객체로 등록
				AddSolid(&shp, SOLID_COLUMN, m_sItemPathNameHeader);
			}
*/
			if(pBri->m_nTypeBridge==BRIDGE_TYPE_SLAB)
			{
				MakeRcWallHunch(pBri, i);
			}
			else
			{
				CTwinVectorArray tvArrColumnFront;
				tvArrColumnFront.RemoveAll();
				tvArrColumnFront.AddFromTvArray(pWall->m_tvArrFront);
				//if(pBri->m_nTypeBridge==BRIDGE_TYPE_TRAP || pBri->m_nTypeBridge==BRIDGE_TYPE_TRAP2)
				//	CalcVertSectionArchHunchHidden(pBri, i, tvArrColumnFront);
				//else if(pBri->m_nTypeBridge!=BRIDGE_TYPE_ARCH)
				CalcVertSectionHunchHidden(pBri, i, tvArrColumnFront);
				
				//TrsVectorArray(edgeArr, tvArrColumnFront, PLN_XOZ, 0);
				long j=0;
				for(j=0;j<tvArrColumnFront.GetSize();j++)
				{
					CTwinVectorArray tvArrTemp;
					CUbGeoCurveArray edgeArrTemp;
					tvArrTemp.Add(tvArrColumnFront.GetAt(j));
					TrsVectorArray(edgeArrTemp, tvArrTemp, PLN_XOZ , 0);
					if(edgeArrTemp.GetSize()!=0)
						edgeArr.Add(edgeArrTemp.GetAt(0));
				} 
				ModifyFrontToPlanColumn(pWall, edgeArr, tvArrColumnFront, pBri->m_nTypeBridge);
				
				CDPoint xyDir = (CDPoint)pWall->m_tvPathPlanLower.m_v2 - (CDPoint)pWall->m_tvPathPlanLower.m_v1;
				CVector vDir	= CVector(xyDir.x, xyDir.y, 0);
				shp	= GetTopoDS_Shape(edgeArr, vDir);
				AddSolid(&shp, SOLID_COLUMN, m_sItemPathNameHeader);
			}

			if(pBri->m_nTypeBridge==BRIDGE_TYPE_ARCH && i==nCountWall/2)
				MakeRcArch(pBri, i);
		}
	}
}

void CUbiARcBridgeData::MakeRcWallTopIsColumn(CRcBridgeApp *pBri, long nWallNum)
{
	if(pBri==NULL || nWallNum<0)		return;
	CWallApp *pWall = pBri->GetInWall(nWallNum);
	if(pWall==NULL)								return;

	if(pWall->m_bTopIsColumn || pWall->m_bRoundRect)
	{
		long nCountColumn = pWall->m_nCountColumn;
		if(pWall->m_bTopIsColumn==FALSE && nCountColumn>0)		nCountColumn=1;
		double dDistColumn = 0.0;
		for(long nC=0; nC<nCountColumn; nC++)
		{
			TopoDS_Shape shp[2];
			if(nC!=0)
				dDistColumn += pWall->m_dDistColumn[nC];
			for(long inner = 0; inner < 2; inner++)
			{
				BOOL bInner	= (inner==iINNER);
				CColumnApp *pColumn	= pWall->GetColumn(nC);
				if(!pColumn) continue;
				
				pColumn->GetTvPlan();
				pColumn->GetTvFront(FALSE, FALSE);
				
				CUbGeoCurveArray edgeArr;
				TrsVectorArray(edgeArr, pColumn->m_tvArrPlan[iOUTTER][iUPPER], PLN_XOY, 0);
				CDPoint xyDir = pWall->m_tvPathPlanLower.GetXyDir();
				edgeArr.Move(CVector(xyDir.x*dDistColumn, xyDir.y*dDistColumn, pWall->m_tvArrFront.GetXyBottom().y));//pColumn->m_dELB + pColumn->m_footing.GetHeight()));
				
				CVector vDir	= CVector(0, 0, 1);
				CDRect rect	= pColumn->m_tvArrFront.GetRect();
				vDir *= (rect.top - rect.bottom);
				shp[inner]	= GetTopoDS_Shape(edgeArr, vDir);
			}
			
			if(0)//pColumnPlus->m_bIsInner)
			{
				shp[iOUTTER]	= BRepAlgoAPI_Cut(shp[iOUTTER], shp[iINNER]);
			}
			// 솔리드 객체로 등록
			AddSolid(&shp[iOUTTER], SOLID_COLUMN, m_sItemPathNameHeader);
		}
		//
		MakeRcWallHunch(pBri, nWallNum);
	}
	
}

void CUbiARcBridgeData::MakeRcWallHunch(CRcBridgeApp *pBri, long nNumWall)
{
	if(pBri==NULL)		return;
	CWallApp *pWall = pBri->GetInWall(nNumWall);
	if(pWall==NULL)		return;
	CJijum *pJ	= pBri->GetJijum(nNumWall+1);
	if(pJ==NULL)		return;

	// ### 단면좌표 구하기.
	CRebarPlacing rb;
	CTwinVectorArray tvArrHunchSect;
	CString strHunch=_T("");
	strHunch.Format(_T("%d번째내측헌치"),nNumWall+1);
	pBri->m_tvArrVertSection.GetTvArrByInfo(strHunch,tvArrHunchSect, FALSE, FALSE);
	long nSize = tvArrHunchSect.GetSize();
	CDPoint xyLeft, xyRight;
	xyLeft = tvArrHunchSect.GetXyLeft();
	xyRight = tvArrHunchSect.GetXyRight();
	CTwinVector tv;
	tv.m_v1 = xyLeft;
	tv.m_v2 = xyRight;
	tvArrHunchSect.Add(tv);
	rb.RemoveMinLengthLine(tvArrHunchSect,0);
	tvArrHunchSect.Sort();
	rb.RemoveMeanlessTwinVector(tvArrHunchSect);
	CUbGeoCurveArray edgeArrSection;
	TrsVectorArray(edgeArrSection, tvArrHunchSect, PLN_XOZ, 0);
	
	long nJ	= nNumWall + 1;
	double dSta = pBri->GetStationOnJijum(nJ);
	CDPoint vAng	= pBri->GetAngleJijum(nJ);
	// ### 벽체 가이드선 구해서 이동, 회전 시키기.
	CTwinVectorArray tvArrCrossSection;
	if(!GetTvArrHunchGuideLine(tvArrCrossSection, pBri, dSta, vAng))			return;
	CUbGeoCurveArray edgeArrLine;
	TrsVectorArray(edgeArrLine, tvArrCrossSection, PLN_XOZ , 0);
	CDRect rcUpperSlab	= tvArrCrossSection.GetRect();
	CDPoint xyOrg	= CVector(rcUpperSlab.right, 0);
	CDPoint xyPlan	= pWall->m_tvPathPlanLower.GetXyBottom();
	// 가운데 벽체가 기둥식일 경우 헌치부 거리를 따로 계산해야 한다.(CRcBridgeApp::GetDistCrossSectionAndInWall)
	if(pWall->m_bTopIsColumn)
	{
		double dLenL	= 0;
		double dLenR	= 0;
		if(nJ > 0 && nJ <pBri->m_nQtyJigan)
		{
			dLenL	= (pBri->m_dDH[0] + pBri->m_dCantilB[0])/vAng.y;
			dLenR	= (pBri->m_dDH[1] + pBri->m_dCantilB[1])/vAng.y;
		}
		xyPlan += pWall->m_tvPathPlanLower.GetXyDir()*dLenR;
	}
	
	CVector vMove	= xyPlan - xyOrg;
	edgeArrLine.Move(vMove);		
	CDPoint xyDir = pWall->m_tvPathPlanLower.GetXyBottom() - pWall->m_tvPathPlanLower.GetXyTop();
	CDPoint vAngPlan	= (xyDir).Unit();
	CDPoint vAngOrg		= CDPoint(1, 0);
	CVector vAngRot		= ToDPointFrDegree(vAngPlan.GetAngleDegree() - vAngOrg.GetAngleDegree());
	edgeArrLine.Rotate(xyPlan, vAngRot);
	
	// ### 구한 단면도를 변환된 가이드선에 맞게 이동 시키기.
	long nEdgeSize = edgeArrLine.GetSize();
	CUbGeoCurve edge(edgeArrLine.GetAt(nEdgeSize-1));
	CVector vPlan	= edge.m_v2;
	strHunch.Format(_T("%d번째내측헌치아래면좌측"),nNumWall+1);
	tv = pBri->m_tvArrVertSection.GetTvByInfo(strHunch);
	CVector vOrg	= CVector(tv.m_v1.x, 0, tvArrHunchSect.GetXyTop().y);
	vMove	= vPlan - vOrg;
	edgeArrSection.Move(vMove);
	
	// ### 솔리드 생성.
	CUbEttSolid *pSolid = NULL;
	pSolid	= new CUbEttSolid();
	pSolid->m_nUnit = UNIT_MM;
	
	TopoDS_Wire aSpine;
	BOOL bMakeWire = MakeWire(edgeArrLine, aSpine);
	if(bMakeWire == FALSE)	return;
	BRepOffsetAPI_MakePipeShell aSweep(aSpine);
	aSweep.SetMode(Standard_False);
	
	TopoDS_Wire aProfile ;
	bMakeWire = MakeWire(edgeArrSection, aProfile );
	if(bMakeWire == FALSE)	return;
	aSweep.Add(aProfile);
	
	BRepBuilderAPI_TransitionMode aTransition = BRepBuilderAPI_RightCorner;
	aSweep.SetTransitionMode(aTransition);
	
	aSweep.Build();
	TopoDS_Shape aResult;
	if(aSweep.IsDone())
		aResult = aSweep.Shape();
	pSolid->SetTopoDS_Shape(aResult);

	// ### 단면 솔리드 생성하기.
	// 시점.
	TopoDS_Face occFace;
	occFace = GetTopoDS_Face(edgeArrSection);
	CUbEttFace *pFace = new CUbEttFace(occFace,UNIT_MM);
	pSolid->AddFace(pFace);
	// 종점.
	edge = edgeArrLine.GetAt(0);
	vPlan	= edge.m_v1;
	strHunch.Format(_T("%d번째내측헌치아래면좌측"),nJ);
	tv = pBri->m_tvArrVertSection.GetTvByInfo(strHunch);
	vOrg	= CVector(tv.m_v1.x, 0, tvArrHunchSect.GetXyTop().y);
	vMove	= vPlan - vOrg;
	TrsVectorArray(edgeArrSection, tvArrHunchSect, PLN_XOZ, 0);
	edgeArrSection.Move(vMove);

	occFace = GetTopoDS_Face(edgeArrSection);
	pFace = new CUbEttFace(occFace,UNIT_MM);
	pSolid->AddFace(pFace);

	AddSolid(pSolid, SOLID_RCWALL);
}

void CUbiARcBridgeData::CalcVertSectionHunchHidden(CRcBridgeApp *pBri, long nNumWall, CTwinVectorArray &tvArrColumnFront)
{
	if(pBri==NULL)	return;
	CWallApp *pWall = pBri->GetInWall(nNumWall);
	if(pWall==NULL)	return;

	long i=0;
	for(i=0;i<tvArrColumnFront.GetSize();i++)
	{
		CTwinVector *pTv = &tvArrColumnFront.GetAt(i);
		if(pTv->m_sInfo.Find(_T("벽체상면"),0)!=-1)
		{
			tvArrColumnFront.RemoveAt(i);
			i--;
		}
	}
	CString sHunch[2];
	sHunch[iLEFT].Format(_T("%d번째내측헌치좌측면"), nNumWall+1);
	sHunch[iRIGHT].Format(_T("%d번째내측헌치우측면"), nNumWall+1);
	CTwinVector tvHunch[2];
	if(!pBri->m_tvArrVertSection.GetTvByInfo(sHunch[iLEFT], tvHunch[iLEFT]))		return;
	if(!pBri->m_tvArrVertSection.GetTvByInfo(sHunch[iRIGHT], tvHunch[iRIGHT]))		return;

	CTwinVector tvWall[2];
	tvWall[iLEFT]	= pWall->m_tvArrFront.GetTvByInfo(_T("벽체좌측면"));
	tvWall[iRIGHT]	= pWall->m_tvArrFront.GetTvByInfo(_T("벽체우측면"));
	CDPoint xyMatch[2] = {CDPoint(0, 0), CDPoint(0,0)};
	CTwinVector tvTemp;
	for(long left = 0; left < 2; left++)
	{
		BOOL bLeft	= left == iLEFT;
		if(GetXyMatchSegAndLine(tvHunch[iLEFT].GetXyTop(), tvHunch[iRIGHT].GetXyTop(), tvWall[left].m_v1, tvWall[left].GetXyDir(), xyMatch[left]))
		{
			tvTemp.m_v1	= tvWall[left].GetXyTop();
			tvTemp.m_v2	= xyMatch[left];
			tvArrColumnFront.Add(tvTemp);
		}
	}

	CTwinVector tvTop;
	tvTop.m_v1	= xyMatch[0];
	tvTop.m_v2	= xyMatch[1];
	tvTop.m_sInfo	= _T("벽체상면");

	tvArrColumnFront.Add(tvTop);
}

void CUbiARcBridgeData::CalcVertSectionArchHunchHidden(CRcBridgeApp *pBri, long nNumWall, CTwinVectorArray &tvArrColumnFront)
{
	if(pBri==NULL)	return;
	CWallApp *pWall = pBri->GetInWall(nNumWall);
	if(pWall==NULL)	return;

	long i=0;
	for(i=0;i<tvArrColumnFront.GetSize();i++)
	{
		CTwinVector *pTv = &tvArrColumnFront.GetAt(i);
		if(pTv->m_sInfo.Find(_T("벽체상면"),0)!=-1)
		{
			tvArrColumnFront.RemoveAt(i);
			i--;
		}
	}

	CDPoint xyMatch[2];
	if(!CalcXyArrMatchArchHidden(pBri, nNumWall, xyMatch))	return;

	CTwinVector tvWall[2];
	tvWall[iLEFT]	= pWall->m_tvArrFront.GetTvByInfo(_T("벽체좌측면"));
	tvWall[iRIGHT]	= pWall->m_tvArrFront.GetTvByInfo(_T("벽체우측면"));
	CTwinVector tvTemp;
	for(long left = 0; left < 2; left++)
	{
		tvTemp.m_v1 = xyMatch[left];
		tvTemp.m_v2 = tvWall[left].GetXyTop();
		tvArrColumnFront.Add(tvTemp);
	}

	tvTemp.m_v1 = xyMatch[0];
	tvTemp.m_v2 = xyMatch[1];
	tvTemp.m_sInfo = _T("벽체상면");
	tvArrColumnFront.Add(tvTemp);

}

BOOL CUbiARcBridgeData::CalcXyArrMatchArchHidden(CRcBridgeApp *pBri, long nNumWall, CDPoint xyMatch[])
{
	if(pBri==NULL)	return FALSE;
	CWallApp *pWall = pBri->GetInWall(nNumWall);
	if(pWall==NULL)	return FALSE;

	CString sHunch[2];
	sHunch[iLEFT].Format(_T("%d번째내측헌치좌측면"), nNumWall+1);
	sHunch[iRIGHT].Format(_T("%d번째내측헌치우측면"), nNumWall+1);
	CTwinVector tvHunch[2];
	tvHunch[iLEFT]	= pBri->m_tvArrVertSection.GetTvByInfo(sHunch[iLEFT]);
	tvHunch[iRIGHT]	= pBri->m_tvArrVertSection.GetTvByInfo(sHunch[iRIGHT]);
	
	CTwinVector tvWall[2];
	tvWall[iLEFT]	= pWall->m_tvArrFront.GetTvByInfo(_T("벽체좌측면"));
	tvWall[iRIGHT]	= pWall->m_tvArrFront.GetTvByInfo(_T("벽체우측면"));
	CTwinVector tvTemp;
	for(long left = 0; left < 2; left++)
	{
		if(tvHunch[left].m_v1.z == -1 || tvHunch[left].m_v1.z == 1)
		{
			if(!GetXyMatchSegAndLine(tvHunch[iLEFT].GetXyTop(), tvHunch[iRIGHT].GetXyTop(), tvWall[left].m_v1, tvWall[left].GetXyDir(), xyMatch[left]))
				return FALSE;
		}
		else 
			return FALSE;
	}
	return TRUE;
}

void CUbiARcBridgeData::MakeRcArch(CRcBridgeApp *pBri, long nNumWall)
{
	if(pBri==NULL)	return;
	CWallApp *pWall = pBri->GetInWall(nNumWall);
	if(pWall==NULL)	return;

	TopoDS_Shape shp[2];
	CTwinVectorArray tvArrRib[2], tvArr;
	pWall->m_tvArrFront.GetTvArrByInfo(_T("벽체하면"), tvArr, FALSE, FALSE);

	for(long i=0;i<2;i++)
	{
		tvArrRib[i].RemoveAll();
		tvArrRib[i] = pBri->m_tvArrVertSectionRib[i];
		if(i==0)
			tvArrRib[i].AddFromTvArray(tvArr);
		
		CTwinVector tv;
		tv.m_v1 = tvArrRib[i].GetXyLeft();
		tv.m_v2 = tvArrRib[i].GetXyRight();
		tvArrRib[i].Add(tv);
		tvArrRib[i].Sort();
		
		CUbGeoCurveArray edgeArr;
		TrsVectorArray(edgeArr, tvArrRib[i], PLN_XOZ, 0);
		
		CTwinVector tvWallLower = pWall->m_tvArrPlan.GetTvByInfo(_T("벽체평면하면"));
		CDPoint xyTop = tvWallLower.GetXyMid();
		edgeArr.Move(CVector(0, xyTop.y, 0));
		CVector vDir	= CVector(0, pWall->m_tvArrPlan.GetVerLength(), 0);
		shp[i]	= GetTopoDS_Shape(edgeArr, vDir);
//		AddSolid(&shp[i], SOLID_COLUMN, m_sItemPathNameHeader);
	}
	// 솔리드 객체로 등록
	shp[iOUTTER]	= BRepAlgoAPI_Cut(shp[iOUTTER], shp[iINNER]);
	AddSolid(&shp[iOUTTER], SOLID_COLUMN, m_sItemPathNameHeader);
}

// 말뚝
void CUbiARcBridgeData::MakeRcPile(CExPileApp *pPile)
{
	if(pPile==NULL)		return;

	CDPoint vAngOrg	= pPile->m_tvArrPlanFooting.GetTvByInfo(_T("기초평면전면")).GetXyDir();
	CDPoint vAngSide = pPile->m_tvArrPlanFooting.GetTvByInfo(_T("좌측기준")).GetXyDir();

	double dR	= pPile->m_dDia/2;
	for(long pile = 0; pile < pPile->m_pArrPile.GetSize(); pile++)
	{
		CPileAttr *pAttr	= pPile->m_pArrPile.GetAt(pile);
		CDPoint xy			= pAttr->xy;

		CVector vTop		= CVector(xy.x, xy.y, pPile->m_tvFrontFooting.m_v1.y + 100);
		CVector vBottom		= vTop + CVector(0, 0, -pAttr->len);
		if(pAttr->ang != 0)
		{
			double dDiff	= pAttr->len / tan(ToRadian(90-pAttr->ang));
			vBottom += vAngOrg * dDiff;
		}
		if(pAttr->angS != 0)
		{
			double dDiff	= pAttr->len / tan(ToRadian(90-pAttr->angS));
			vBottom += vAngSide * dDiff;
		}
				
		CVector vDir		= (vBottom - vTop).Unit();
		TopoDS_Shape shp	= CUbEttSolid::GetTopoDS_Shape_Cylinder(vTop, vDir, dR, pAttr->len);

		// 솔리드 객체로 등록
		AddSolid(&shp, SOLID_PILE, m_sItemPathNameHeader);
	}
}

void CUbiARcBridgeData::MakeRcWall(CRcBridgeApp *pBri)
{
	if(pBri==NULL)	return;

	long i=0;
	for(i=0; i<2; i++)		//	0: 시점, 1 : 종점
	{
		TARGET_IN_ONE_SLAB target;
		if(pBri->m_nTypeWallForRebarGen == 0)
			target = (i==iSTT)?BRIDGE_OFF_STT_SLAB:BRIDGE_OFF_END_SLAB;
		else if(pBri->m_nTypeWallForRebarGen == 1)
			target = (i==iSTT)?BRIDGE_OFF_STT_STT:BRIDGE_OFF_END_END;


		double dSta = pBri->GetStationByTarget(target, 0, TRUE);
		CDPoint vAng = pBri->GetAngleByTarget(target, 0);
		CTwinVectorArray tvArrCrossSection, tvArrLeft, tvArrRight;
		pBri->GetTvCrossSection(tvArrCrossSection, dSta, vAng, FALSE, FALSE, FALSE, 0, FALSE, FALSE);
		if(tvArrCrossSection.GetSize()==0)	continue;

		BOOL bInnerAtLeft	= pBri->m_nTypeWallForRebarGen==0 ? FALSE : TRUE;
		BOOL bInnerAtRight	= bInnerAtLeft;

		pBri->GetTvCrossSection_OutWall_Exp(tvArrLeft, (i==iSTT), TRUE, bInnerAtLeft, TRUE);
		pBri->GetTvCrossSection_OutWall_Exp(tvArrRight, (i==iSTT), FALSE, bInnerAtRight, TRUE);
		CRebarPlacing rb;
		rb.RemoveMinLengthLine(tvArrLeft, 0);
		rb.RemoveMinLengthLine(tvArrRight,0);
		if(tvArrLeft.GetSize()==0 || tvArrRight.GetSize()==0)	continue;
		
		CTwinVector tvBottom, tvLeftWall, tvRightWall;

		tvLeftWall	= tvArrLeft.GetTvByInfo(_T("벽체좌측"));
		tvRightWall	= tvArrRight.GetTvByInfo(_T("벽체우측"));
		
		tvBottom.m_v1 = tvLeftWall.GetXyBottom();
		tvBottom.m_v2 = tvRightWall.GetXyBottom();
		
		tvArrCrossSection.Add(tvLeftWall);
		tvArrCrossSection.Add(tvRightWall);
		tvArrCrossSection.Add(tvBottom);
		tvArrCrossSection.Sort();

		double dDiffEl = pBri->GetDiffELSttAndJijumForVertSectionBySta(dSta);
		rb.MoveRebar(tvArrCrossSection, CDPoint(0,-dDiffEl));
		
		TopoDS_Shape shp;
		CUbGeoCurveArray edgeArr;
		TrsVectorArray(edgeArr, tvArrCrossSection, PLN_XOZ, 0);

		// 실제 위치로 이동 - 벽체 하면 우측을 기준으로 함
		CTwinVectorArray tvArrSide;
		if(!GetTvArrSlabPlanSide(tvArrSide, pBri, (i==iSTT)))			continue;
		CWallApp wall;
		((CRcBridgeRebar*)pBri)->MakeWallByOutWall(wall, (i==iSTT), TRUE);
		ModifySideToPlanWall(&wall,edgeArr, tvArrSide.GetAt(0)); 

		double dBriW = 0.0;
		if(i==0)			dBriW = pBri->m_dWS;
		else				dBriW = pBri->m_dWE;
		
		CTwinVectorArray tvArrSttEnd;
		pBri->GetTvPlaneSttEndLine(tvArrSttEnd, (i==0)?TRUE:FALSE, FALSE);
		
		CVector vDir	= CVector(tvArrSttEnd.GetXyBottom() - tvArrSide.GetXyBottom());
		shp	= GetTopoDS_Shape(edgeArr, vDir);
		// 솔리드 객체로 등록
		AddSolid(&shp, SOLID_RCWALL, m_sItemPathNameHeader);
	}
}
/*
{
	if(pBri==NULL)	return;

	CTwinVectorArray tvArrStt, tvArrEnd;
	CTwinVectorArray tvArrPlane;
	pBri->GetTvRebarOut_Plane_UpperSlabStd(tvArrPlane, -1, TRUE);
	CTwinVector tv;
	long i=0;
	for(i = 0; i < tvArrPlane.GetSize(); i++)
	{
		tv	= tvArrPlane.GetAt(i);
		if(tv.m_sInfo == _T("시점")) tvArrStt.Add(tv);
		else if(tv.m_sInfo == _T("종점")) tvArrEnd.Add(tv);
	}

	if(tvArrStt.GetSize()==0 || tvArrEnd.GetSize()==0)		return;

	for(i=0; i<2; i++)		//	0 : 종점, 1: 시점
	{
		CTwinVectorArray tvArrSttEnd, *pTvArrPlane;
		if(i==iSTT)					pTvArrPlane = &tvArrEnd;
		else if(i==iEND)			pTvArrPlane = &tvArrStt;
		pBri->GetTvPlaneSttEndLine(tvArrSttEnd, i, FALSE);
		
		CTwinVector tvTop, tvBottom;
		tvTop.m_v1 = pTvArrPlane->GetXyTop();
		tvTop.m_v2 = tvArrSttEnd.GetXyTop();
		tvBottom.m_v1 = pTvArrPlane->GetXyBottom();
		tvBottom.m_v2 = tvArrSttEnd.GetXyBottom();
		
		pTvArrPlane->AddFromTvArray(tvArrSttEnd);
		pTvArrPlane->Add(tvTop);
		pTvArrPlane->Add(tvBottom);
		
		TopoDS_Shape shp;
		CUbGeoCurveArray edgeArr;
		TrsVectorArray(edgeArr, *pTvArrPlane, PLN_XOY, 0);
		double dFH = 0.0;
		CFootingApp *pFooting = NULL;
		if(pBri->m_nTypeBridge != BRIDGE_TYPE_BOX)
		{
			if(i==0)
			{
				pFooting = &pBri->m_footingEnd;
				dFH = pFooting->m_dFH + pFooting->m_dFSLH;
			}
			else if(i==1)
			{
				pFooting = &pBri->m_footingStt;
				dFH = pFooting->m_dFH + pFooting->m_dFSRH;
			}
		}
		else
			dFH = pBri->m_dFHS;
		
		double dEL = 0.0;
		if(i==0)
		{
			long bBox = pBri->m_nTypeBridge == BRIDGE_TYPE_BOX;	// 박스교인 경우 값이 시점측을 따라가는 경우가 좀 있다.
			dEL = (bBox) ? pBri->m_dELSB : pBri->m_dELEB;
		}
		else
			dEL = pBri->m_dELSB;
		edgeArr.Move(CVector(0,0,dEL+dFH));
		
		CTwinVectorArray *pTvArr = &pBri->m_tvArrVertSection;
		if(pTvArr!=NULL)
		{
			double dSttWallH = 0.0;
			if(i==0)
			{
				pTvArr->GetTvByInfo(_T("종점벽체우측면"), tv);
				dSttWallH = tv.GetVerLength()+pBri->m_dBHE+pBri->m_dBTHE-200;
			}
			else if(i==1)
			{
				pTvArr->GetTvByInfo(_T("시점벽체좌측면"), tv);
				dSttWallH = tv.GetVerLength()+pBri->m_dBHS+pBri->m_dBTHS-200;
			}
			//dSttWallH = tv.GetVerLength()+pBri->m_dBHS;m_dBHWE;
			CVector vDir	= CVector(0, 0, dSttWallH);
			shp	= GetTopoDS_Shape(edgeArr, vDir);
			// 솔리드 객체로 등록
			AddSolid(&shp, SOLID_RCWALL, m_sItemPathNameHeader);
		}
	}
}
*/

void CUbiARcBridgeData::MakeRcSlab(CRcBridgeApp *pBri)
{
	if(pBri==NULL)	return;

	CUbEttSolid *pSolid = NULL;
	pSolid	= new CUbEttSolid();
	pSolid->m_nUnit = UNIT_MM;
	MakeRcSlabPlan(pSolid, pBri);

	if(pBri->m_nTypeBridge==BRIDGE_TYPE_TRAP || pBri->m_nTypeBridge==BRIDGE_TYPE_PI || pBri->m_nTypeBridge==BRIDGE_TYPE_SLAB)
		MakeRcSlabSection(pSolid, pBri);

	AddSolid(pSolid, SOLID_SLAB);
}

void CUbiARcBridgeData::MakeRcSlabPlan(CUbEttSolid *pSolid, CRcBridgeApp *pBri)
{
	if(pSolid==NULL)	return;
	if(pBri==NULL)		return;
	CLineInfo *pLineCR = pBri->GetLineBase();
	if(pLineCR==NULL)	return;
	
	double dStaStt	= pBri->GetStationByTarget(BRIDGE_OFF_STT_SLAB, 0, TRUE);
	double dStaEnd	= pBri->GetStationByTarget(BRIDGE_OFF_END_SLAB, 0, TRUE);
	CDPoint vAngStt	= pBri->GetAngleByTarget(BRIDGE_OFF_STT_SLAB, 0);
	CDPoint vAngEnd	= pBri->GetAngleByTarget(BRIDGE_OFF_END_SLAB, 0);
	
	TopoDS_Wire wireShape[55];			
	CUbGeoCurveArray edgeArr[55];			
	// 슬래브 연단 계산
	CDPointArray xyArrDistStt, xyArrDistEnd;
	if(GetXySlabAct(pBri, dStaStt, vAngStt, xyArrDistStt)==FALSE)	return;
	if(GetXySlabAct(pBri, dStaEnd, vAngEnd, xyArrDistEnd)==FALSE)	return;
	if(xyArrDistStt.GetSize()==0 || xyArrDistEnd.GetSize()==0)		return;
	// 일단..return;
//	if(xyArrDistStt.GetSize()!=xyArrDistEnd.GetSize())				return;

	CDoubleArray dArrStaStt, dArrStaEnd, dArrDistStt, dArrDistEnd;
	if(GetStationAndDistMatchVertical(pBri, TRUE, xyArrDistStt, dArrStaStt, dArrDistStt)==FALSE)	return;
	if(GetStationAndDistMatchVertical(pBri, FALSE, xyArrDistEnd, dArrStaEnd, dArrDistEnd)==FALSE)	return;
	if(dArrStaStt.GetSize()==0 || dArrStaEnd.GetSize()==0)											return;
	if(dArrDistStt.GetSize()==0 || dArrDistEnd.GetSize()==0)										return;
	//

	long nDan = 0;
	long i=0, j=0;
	long nQtyHDan = xyArrDistStt.GetSize();
	for(nDan=0; nDan<nQtyHDan; nDan++)
	{
		CTwinVectorArray tvArrLine;
		tvArrLine.AddFromLineInfo((CLineInfoApp*)pLineCR, dArrStaStt.GetAt(nDan), dArrStaEnd.GetAt(nDan), dArrDistStt[nDan]);
		tvArrLine.Sort();
		// Station 계산.
		CDoubleArray dStaIP;
		dStaIP.RemoveAll();
		long nIPsu = pLineCR->m_VIPsu;
		dStaIP.Add(dArrStaStt.GetAt(nDan));
		for(i=0;i<nIPsu;i++)
		{
			if(pLineCR->m_VIPData[i][0]<=dArrStaStt.GetAt(nDan))	continue;
			if(pLineCR->m_VIPData[i][0]>=dArrStaEnd.GetAt(nDan))	break;
			dStaIP.Add(pLineCR->m_VIPData[i][0]);
		}
		dStaIP.Add(dArrStaEnd.GetAt(nDan));
		//
		// Trim
		CRebarPlacing rb;
		CTwinVectorArray tvArrTrimLine;
		for(i=0;i<dStaIP.GetSize();i++)
		{
			tvArrTrimLine = tvArrLine;
			CDPoint xy1(0,0),xy2(0,0),xyPos(0,0);
			if(i==0)
			{
				xyPos = pLineCR->GetXyLineDis(dStaIP[i+1],dArrDistStt[nDan]);
				xy1.x = xyPos.x;
				xy1.y = xyPos.y - 100000;
				xy2.x = xyPos.x;
				xy2.y = xyPos.y + 100000;
				rb.TrimTvArrayByLine(tvArrTrimLine,xy1,xy2,FALSE,TRUE,FALSE);
			}
			else if(i==dStaIP.GetSize()-1)
			{
				xyPos = pLineCR->GetXyLineDis(dStaIP[i],dArrDistStt[nDan]);
				xy1.x = xyPos.x;
				xy1.y = xyPos.y - 100000;
				xy2.x = xyPos.x;
				xy2.y = xyPos.y + 100000;
				rb.TrimTvArrayByLine(tvArrTrimLine,xy1,xy2,TRUE,TRUE,FALSE);
			}
			else
			{
				xyPos = pLineCR->GetXyLineDis(dStaIP[i],dArrDistStt[nDan]);
				xy1.x = xyPos.x;
				xy1.y = xyPos.y - 100000;
				xy2.x = xyPos.x;
				xy2.y = xyPos.y + 100000;
				rb.TrimTvArrayByLine(tvArrTrimLine,xy1,xy2,TRUE,TRUE,FALSE);
				xyPos = pLineCR->GetXyLineDis(dStaIP[i+1],dArrDistStt[nDan]);
				xy1.x = xyPos.x;
				xy1.y = xyPos.y - 100000;
				xy2.x = xyPos.x;
				xy2.y = xyPos.y + 100000;
				rb.TrimTvArrayByLine(tvArrTrimLine,xy1,xy2,FALSE,TRUE,FALSE);
			}

			if(tvArrTrimLine.GetSize()>0)
			{
				if(i==dStaIP.GetSize()-1 && dStaIP.GetSize()==2)	continue;
				CUbGeoCurveArray edgeArrTemp;
				TrsVectorArray(edgeArrTemp, tvArrTrimLine, PLN_XOY, 0);
				//
				if(edgeArrTemp.GetSize()<tvArrTrimLine.GetSize())
				{
					edgeArrTemp.RemoveAll();
					CTwinVectorArray tvArrTemp;
					CUbGeoCurveArray edgeArr;
					for(i=0;i<tvArrTrimLine.GetSize();i++)
					{
						tvArrTemp.RemoveAll();
						tvArrTemp.Add(tvArrTrimLine.GetAt(i));
						TrsVectorArray(edgeArr, tvArrTemp, PLN_XOY , 0);
						edgeArrTemp.Add(edgeArr.GetAt(0));
					}
				}
				//
				long nSize = edgeArrTemp.GetSize();
				for(j=0;j<nSize; j++)
				{
					double dSta = 0.0;
					CDPoint vAng(0,1);
					CDPointArray xyArrDist;
					pLineCR->GetStationMatchVerticalByXy(tvArrTrimLine.GetAt(j).m_v1, dSta);
					vAng = (j==0) ? vAngStt : CDPoint(0,1);
					if(GetXySlabAct(pBri, dSta, vAng, xyArrDist)==FALSE)	return;
					edgeArrTemp.GetAt(j).m_v1.z = xyArrDist[nDan].y;

					pLineCR->GetStationMatchVerticalByXy(tvArrTrimLine.GetAt(j).m_v2, dSta);
					vAng = (j==nSize-1) ? vAngEnd :CDPoint(0,1);
					if(GetXySlabAct(pBri, dSta, vAng, xyArrDist)==FALSE)	return;
					edgeArrTemp.GetAt(j).m_v2.z = xyArrDist[nDan].y;

//					if(edgeArrTemp.GetAt(i).m_v1.z == 0)	return;
					edgeArrTemp.GetAt(j).m_vCen.z	= (edgeArrTemp.GetAt(j).m_v1.z + edgeArrTemp.GetAt(j).m_v2.z)/2.0;
					edgeArr[nDan].Add(edgeArrTemp.GetAt(j));
				}
			}
		}
	}
	
	//	nQtyHDan = edgeArr.GetLength();
	// 와이어 생성.
	for(i=0, j=0;i<nQtyHDan;i++,j++)
	{

		BOOL bMakeWire = MakeWire(edgeArr[i], wireShape[j]);
		if(bMakeWire == FALSE)			j--;
	}
	nQtyHDan = j;
	// lofting surface 생성.
	BRepOffsetAPI_ThruSections aTSec[55];
	for(i=0;i<nQtyHDan;i++)
	{
		for(j=0; j<2;j++)
		{
			long nIdx = ((i+j) > nQtyHDan-1) ? 0 : (i+j);
			aTSec[i].AddWire(wireShape[nIdx]);
		}
		
		aTSec[i].Init();
		aTSec[i].Build();
		
		// lofting surface에서 shape 추출.
		TopoDS_Shape aShape = aTSec[i].Shape();
		
//		AddSolid(&aShape, SOLID_SLAB, m_sItemPathNameHeader);		
		pSolid->SetTopoDS_Shape(aShape);
	}
}

// 슬래브 연단 계산(왼쪽상면에서 시계방향)
BOOL CUbiARcBridgeData::GetXySlabAct(CRcBridgeApp *pBri, double sta, CDPoint vAng, CDPointArray &xyArrDist)
{
	if(pBri==NULL)	return FALSE;
	xyArrDist.RemoveAll();
	
	CTwinVectorArray tvArrSlab;
	pBri->GetTvCrossSection(tvArrSlab, sta, vAng, FALSE, TRUE, FALSE, 0, FALSE, FALSE);
	CRebarPlacing rb;
	rb.RemoveMinLengthLine(tvArrSlab, 0);
	tvArrSlab.Sort();

	double dDiffEl = pBri->GetDiffELSttAndJijumForVertSectionBySta(sta);
	rb.MoveRebar(tvArrSlab, CDPoint(0, -dDiffEl));
	
	CalcXyArrAct(tvArrSlab, xyArrDist);

	long nQtyHDan = xyArrDist.GetSize();
	if(nQtyHDan!=0)			return TRUE;
	else					return FALSE;
}

void CUbiARcBridgeData::CalcXyArrAct(CTwinVectorArray &tvArr, CDPointArray &xyArrDist)
{
	xyArrDist.RemoveAll();
	// 왼쪽상면에서 시작
	TrimArcTvArr(tvArr,7);

	long nSlab=0;
	for(nSlab=0; nSlab<tvArr.GetSize(); nSlab++)
	{
		CTwinVector *pTv = &tvArr.GetAt(nSlab);
		xyArrDist.Add(pTv->m_v1);
	}
}

BOOL CUbiARcBridgeData::GetStationAndDistMatchVertical(CRcBridgeApp *pBri, BOOL bStationStt, 
													   CDPointArray &xyArrDist, CDoubleArray &dArrStation, CDoubleArray &dArrDist)
{
	if(pBri==NULL)	return FALSE;
	if(xyArrDist.GetSize()==0)	return FALSE;
	CLineInfo *pLineCR = pBri->GetLineBase();
	if(pLineCR==NULL)	return FALSE;
	dArrStation.RemoveAll();
	dArrDist.RemoveAll();

	CTwinVectorArray tvArrPlane;
	CTwinVectorArray tvArrPlaneLine;
	pBri->GetTvRebarOut_Plane_UpperSlabStd(tvArrPlane, -1, TRUE);
	CTwinVector tv;
	CString strLine(_T(""));

	if(bStationStt)		strLine = _T("시점");
	else				strLine = _T("종점");

	long i=0;
	for(i = 0; i < tvArrPlane.GetSize(); i++)
	{
		tv	= tvArrPlane.GetAt(i);
		if(tv.m_sInfo == strLine)
		{
			if(tv.GetLength()!=0)
				tvArrPlaneLine.Add(tv);
		}
	}
	if(tvArrPlaneLine.GetSize()==0)		return FALSE;

	CDPoint xyLineDir = tvArrPlaneLine.GetAt(0).GetXyDir();
	CDPoint xyPlan = tvArrPlaneLine.GetAt(0).m_v1;
	if(bStationStt)
	{
		xyLineDir *= -1;
//		CDPoint xy = (tvArrPlaneLine.GetAt(1).m_v1 - tvArrPlaneLine.GetAt(1).m_v2).Unit();
		xyPlan = tvArrPlaneLine.GetAt(0).m_v2;
	}
//
	TARGET_IN_ONE_SLAB target = bStationStt ? BRIDGE_OFF_STT_SLAB : BRIDGE_OFF_END_SLAB;
	double dSta = pBri->GetStationByTarget(target, 0, TRUE);
	CDPoint vAng = pBri->GetAngleByTarget(target, 0);
	CTwinVectorArray tvArrSlab;
	pBri->GetTvCrossSection(tvArrSlab, dSta, vAng, FALSE, TRUE, FALSE, 0, FALSE, FALSE);
	CDPoint xyOrg = tvArrSlab.GetXyLeft();

	CRebarPlacing rb;
	double dDiffEl = pBri->GetDiffELSttAndJijumForVertSectionBySta(dSta);
	rb.MoveRebar(tvArrSlab, CDPoint(0, -dDiffEl));
//
	for(i=0;i<xyArrDist.GetSize();i++)
	{
		double dLength = fabs(xyOrg.x-xyArrDist.GetAt(i).x);
		double dStation = 0.0;
		CDPoint xy	= xyPlan + xyLineDir*dLength;
		pLineCR->GetStationMatchVerticalByXy(xy, dStation);
		dArrStation.Add(dStation);
		dArrDist.Add(pLineCR->GetDisMatchVerticalByXy(xy));
	}
	return TRUE;
}


void CUbiARcBridgeData::TrimArcTvArr(CTwinVectorArray &tvArrArc, long nTrimNum)
{
	if(tvArrArc.GetSize()==0)		return;

	tvArrArc.Sort();
	long i = 0;
	for(i=0; i<tvArrArc.GetSize(); i++)
	{
		CTwinVector *pTv = &tvArrArc.GetAt(i);
		if(pTv->m_v2.z!=0)
		{
			CTwinVector tvTemp = tvArrArc.GetAt(i);
			
			CDPoint xyCenter = pTv->GetXyCircleCenter();
			double dR1 = ~(CDPoint(pTv->m_v1)-xyCenter);
			double dR2 = ~(CDPoint(pTv->m_v2)-xyCenter);
			double dAng1 = CDPoint(CDPoint(pTv->m_v1)-xyCenter).GetAngleDegree();
			double dAng2 = CDPoint(CDPoint(pTv->m_v2)-xyCenter).GetAngleDegree();

			for(long j=0;j<nTrimNum;j++)
			{
				if(dAng1<90)	dAng1+=360;
				double dOffsetAng = dAng1 + ((dAng2-dAng1)/nTrimNum*(j+1));
				CDPoint xyLine = ToDPointFrDegree(dOffsetAng).Unit()*((dR1+dR2)/2) + xyCenter;
				if(j==0)
				{
					tvTemp.m_v2.x = xyLine.x;
					tvTemp.m_v2.y = xyLine.y;
				}
				else
				{
					tvTemp.m_v1.x = tvTemp.m_v2.x;
					tvTemp.m_v1.y = tvTemp.m_v2.y;
					tvTemp.m_v2.x = xyLine.x;
					tvTemp.m_v2.y = xyLine.y;
				}
				tvArrArc.InsertAt(i+j,tvTemp);
			}
			
			tvArrArc.RemoveAt(i+nTrimNum);
			i+=(nTrimNum-1);
		}
	}
}

void CUbiARcBridgeData::MakeRcSlabSection(CUbEttSolid *pSolid, CRcBridgeApp *pBri)
{
	if(pSolid==NULL)	return;
	if(pBri==NULL)		return;

	long nSlab=0;
	for(nSlab=0; nSlab<2; nSlab++)		//	0 : 시점, 1: 종점
	{
		TARGET_IN_ONE_SLAB target = (nSlab==0) ? BRIDGE_OFF_STT_SLAB : BRIDGE_OFF_END_SLAB;	
		double dSta	= pBri->GetStationByTarget(target, 0, TRUE);
		
		CTwinVectorArray tvArrPlane, tvArrPlaneSide;
		if(!GetTvArrSlabPlanSide(tvArrPlaneSide, pBri, (nSlab==iSTT)))		continue;
		CTwinVectorArray tvArrSlabSection;
		CUbGeoCurveArray edgeArrSlab;
		CDPoint vAng = pBri->GetAngleByTarget((nSlab==iSTT) ? BRIDGE_OFF_STT_SLAB : BRIDGE_OFF_END_SLAB, 0);
		pBri->GetTvCrossSection(tvArrSlabSection, dSta, vAng, FALSE, TRUE, FALSE, 0, FALSE, FALSE);
		CRebarPlacing rb;
		double dDiffEl = pBri->GetDiffELSttAndJijumForVertSectionBySta(dSta);
		rb.MoveRebar(tvArrSlabSection, CDPoint(0, -dDiffEl));
		TrsVectorArray(edgeArrSlab, tvArrSlabSection, PLN_XOZ , 0);
		
		// 실제 위치로 이동
		long nDir = -1;
		CDPoint uXy(0,0);
		uXy = tvArrPlaneSide.GetXyBottom();
//		if(nSlab==1)			nDir = 1;

		CDRect rect	= tvArrSlabSection.GetRect();
		CVector vSection(rect.right, 0, 0);
		CVector vPlane(CVector(uXy.x, uXy.y,0));
		CVector vMove = vPlane - vSection;
		edgeArrSlab.Move(vMove);
		
		CDPoint vAngPlan	= tvArrPlaneSide.GetXyTop() - tvArrPlaneSide.GetXyBottom();//tvArrPlaneSide.GetXyTop() - tvArrPlaneSide.GetXyBottom();//tvArrPlaneSide.GetAt(1).GetXyDir();
		CDPoint vAngOrg		= CDPoint(nDir,0);
		CDPoint vAngRot		= ToDPointFrDegree(vAngPlan.GetAngleDegree() - vAngOrg.GetAngleDegree());
		edgeArrSlab.Rotate(uXy, vAngRot);
		//	
		
	if(edgeArrSlab.GetSize() > 2)
		{
			TopoDS_Face occFace;
			occFace = GetTopoDS_Face(edgeArrSlab);
			CUbEttFace *pFace = new CUbEttFace(occFace,UNIT_MM);
			pSolid->AddFace(pFace);
		}
	}
}

void CUbiARcBridgeData::MakeRcHunch(CRcBridgeApp *pBri)
{
	if(pBri==NULL)	return;

	long nCountJijum	= pBri->GetCountJijum();
	long nHunch = 0;
	for(nHunch=0; nHunch<nCountJijum; nHunch++)
	{
		if(nHunch!=0 && nHunch!=nCountJijum-1)		continue;

		// 가이드선 구해 평면위치로 이동.
/*		TARGET_IN_ONE_SLAB target = (nHunch==iSTT)?BRIDGE_OFF_STT_STT:BRIDGE_OFF_END_END;
		double dSta	= pBri->GetStationByTarget(target, 0, TRUE);
		CDPoint vAng	= pBri->GetAngleByTarget(target, 0);
*/
		double  dSta = pBri->GetStationOnJijum(nHunch);
		CDPoint  vAng	= pBri->GetAngleJijum(nHunch);
		//
		CLineInfo *pLineCR = pBri->GetLineBase();
		CDPoint xyPos = pLineCR->GetXyLineDis(dSta, 0);
		//
		CTwinVectorArray tvArrCrossSection, tvArrVertSection;
		if(!GetTvArrHunchGuideLine(tvArrCrossSection, pBri, dSta, vAng))		continue;
		CUbGeoCurveArray edgeArrLine;
		TrsVectorArray(edgeArrLine, tvArrCrossSection, PLN_XOZ , 0);

		CDRect rcUpperSlab	= tvArrCrossSection.GetRect();
		ModifyFrontToPlanHunch(pBri, edgeArrLine, nHunch, rcUpperSlab); 
		
		// 헌치단면 구하기.
		CString strHunch = _T("시점벽체헌치");
		CTwinVector tvHunch;
		if(nHunch==0)
		{
			tvHunch = pBri->m_tvArrVertSection.GetTvByInfo(strHunch);
		}
		else
		{
			strHunch = _T("종점벽체헌치");
			tvHunch = pBri->m_tvArrVertSection.GetTvByInfo(strHunch);
			CDPoint xyTemp = tvHunch.m_v1;
			tvHunch.m_v1 = (CDPoint)tvHunch.m_v2;
			tvHunch.m_v1.z *= -1;
			tvHunch.m_v2 = xyTemp;
			tvHunch.m_v2.z *= -1;
		}
		if(tvHunch.m_sInfo!=strHunch)	continue;
		
		tvArrVertSection.Add(tvHunch);
		CTwinVector tvTemp = tvHunch;
		tvTemp.m_v2.y = tvHunch.m_v1.y;
		tvTemp.m_v1.z = 0;
		tvTemp.m_v2.z = 0;
		tvArrVertSection.Add(tvTemp);
		tvTemp = tvHunch;
		tvTemp.m_v1.x = tvHunch.m_v2.x;
		tvTemp.m_v1.z = 0;
		tvTemp.m_v2.z = 0;
		tvArrVertSection.Add(tvTemp);
		tvArrVertSection.Sort();
		
		CUbGeoCurveArray edgeArrSection;
		TrsVectorArray(edgeArrSection, tvArrVertSection, PLN_XOZ, 0);

		// 가이드선 따라 Solid 형성하기.
		CUbEttSolid *pSolid = NULL;
		pSolid	= new CUbEttSolid();
		pSolid->m_nUnit = UNIT_MM;

		CTwinVectorArray tvArrPlanSide;
		if(!GetTvArrSlabPlanSide(tvArrPlanSide, pBri, (nHunch==iSTT)))		continue;
		CTwinVectorArray tvArrSttEnd;
		pBri->GetTvPlaneSttEndLine(tvArrSttEnd, (nHunch==0)?TRUE:FALSE, FALSE);

		CUbGeoCurve edge;
		for(long nEdge=0;nEdge<edgeArrLine.GetSize();nEdge++)
		{
			edge	= edgeArrLine.GetAt(nEdge);
			CUbGeoCurveArray edgeArrTemp;
			edgeArrTemp.Add(edge);

			TopoDS_Wire aSpine;
			BOOL bMakeWire = MakeWire(edgeArrTemp, aSpine);
			if(bMakeWire == FALSE)	return;
			BRepOffsetAPI_MakePipeShell aSweep(aSpine);
			aSweep.SetMode(Standard_False);
			
			if(nEdge==0)
			{
				CDRect rect	= tvArrVertSection.GetRect();
				CDPoint xyOrg(0,0), xyPlan(0,0);
				CVector vMove(0,0,0), vAngPlan(0,0,0), vAngOrg(0,0,0), vAngRot(0,0,0);

				long nDir = 0;
				if(nHunch==0)
				{
					xyOrg	= CVector(rect.left, 0, 0);
					nDir = 1;
				}
				else //if(nHunch==1)
				{
					xyOrg	= CVector(rect.right, 0, 0);
					nDir = -1;
				}
				xyPlan	= (CDPoint)edge.m_v2;
				vMove	= xyPlan - xyOrg;
				edgeArrSection.Move(vMove);
				vAngPlan	= (tvArrSttEnd.GetXyBottom() - tvArrPlanSide.GetXyBottom()).Unit();
				vAngOrg	= CDPoint(nDir, 0);
				vAngRot	= ToDPointFrDegree(vAngPlan.GetAngleDegree() - vAngOrg.GetAngleDegree());
				edgeArrSection.Rotate(xyPlan, vAngRot);
			}
			//
			
			TopoDS_Wire aProfile ;
			bMakeWire = MakeWire(edgeArrSection, aProfile );
			if(bMakeWire == FALSE)	return;
			aSweep.Add(aProfile);
			
			BRepBuilderAPI_TransitionMode aTransition = BRepBuilderAPI_RightCorner;
			aSweep.SetTransitionMode(aTransition);
			
			aSweep.Build();
			TopoDS_Shape aResult;
			if(aSweep.IsDone())
				aResult = aSweep.Shape();
			
			pSolid->SetTopoDS_Shape(aResult);
		}

		BOOL bStt = (nHunch==iSTT);
		MakeRcHunchSection(pSolid, pBri, tvArrVertSection, edgeArrLine, bStt);

		AddSolid(pSolid, SOLID_RCWALL);
	}
}

void CUbiARcBridgeData::MakeRcInnerHunch(CRcBridgeApp *pBri)
{
	if(pBri==NULL)	return;

	long nCountHunch	= (pBri->GetCountInWall())*2;
	long nHunch = 0;
	for(nHunch=0; nHunch<nCountHunch; nHunch++)
	{
		long nNumWall	= nHunch/2;
		CWallApp *pWall = pBri->GetInWall(nNumWall);
		if(pWall==NULL)	continue;

		// 헌치 단면 구하기.
		CTwinVector tvHunch;
		CString sHunch;
		if(nHunch%2==iLEFT)
		{
			sHunch.Format(_T("%d번째내측헌치좌측면"), nNumWall+1);
			tvHunch	= pBri->m_tvArrVertSection.GetTvByInfo(sHunch);
		}
		else if(nHunch%2==iRIGHT)
		{
			sHunch.Format(_T("%d번째내측헌치우측면"), nNumWall+1);
			tvHunch	= pBri->m_tvArrVertSection.GetTvByInfo(sHunch);
		}
		CTwinVectorArray tvArrVertSection;
		tvArrVertSection.Add(tvHunch);

		CString sWall;
		if(nHunch%2==0)			sWall	= _T("슬래브하면");
		else if(nHunch%2==1)	sWall.Format(_T("%d번째내측헌치우측슬래브하면"), nNumWall+1);
		CTwinVector tvSlabLower;
		if(pBri->m_tvArrVertSection.GetTvByInfo(sWall, tvSlabLower)==FALSE)		continue;
		CDPoint vAngLine;
		if(nHunch%2==0)
		{
			vAngLine	= tvSlabLower.GetXyDir(TRUE);
			sWall	= _T("벽체좌측면");
		}
		else if(nHunch%2==1)	
		{
			vAngLine	= tvSlabLower.GetXyDir();
			sWall	= _T("벽체우측면");
		}

		CTwinVector tvWall;
		if(pWall->m_tvArrFront.GetTvByInfo(sWall, tvWall)==FALSE)		continue;

		CDPoint xyMatch(0, 0);
		if(GetXyMatchLineAndLine(tvHunch.GetXyTop(), vAngLine, tvHunch.GetXyBottom(), tvWall.GetXyDir(), xyMatch))
		{
			CTwinVector tvTemp;
			tvTemp.m_v1	= tvHunch.GetXyTop();
			tvTemp.m_v2	= xyMatch;
			tvArrVertSection.Add(tvTemp);
			tvTemp.m_v1	= tvHunch.GetXyBottom();
			tvTemp.m_v2	= xyMatch;
			tvArrVertSection.Add(tvTemp);
		}
		pWall->GetTvPlan();
		CTwinVector tvWallLeft, tvWallRight, tvWallLeftRight;
		if(!pWall->m_tvArrPlan.GetTvByInfo(_T("벽체평면좌측"), tvWallLeft))	continue;
		if(!pWall->m_tvArrPlan.GetTvByInfo(_T("벽체평면우측"), tvWallRight))	continue;
		if(nHunch%2==iLEFT)					tvWallLeftRight	= tvWallLeft;
		else if(nHunch%2==iRIGHT)			tvWallLeftRight	= tvWallRight;


		// 헌치부 가이드 라인 구해서 평면위치로 이동.
		CJijum *pJ	= pBri->GetJijum(nNumWall+1);
		if(pJ==NULL)	continue;
		double dSta	= tvWallLeftRight.GetXyMid().x;
		dSta = pBri->GetStationOnJijum(nNumWall+1);
		CDPoint vAng	= pJ->m_vAngle;
		
		CTwinVectorArray tvArrCrossSection, tvArrCrossHunch;
		if(!GetTvArrHunchGuideLine(tvArrCrossSection, pBri, dSta, vAng))			continue;
		CUbGeoCurveArray edgeArrLine;
		TrsVectorArray(edgeArrLine, tvArrCrossSection, PLN_XOZ , 0);

		CDPoint xyPlan	= tvWallLeftRight.GetXyBottom();
		// 경사교인 경우 벽체상부평면도가 단면도의 상부슬래브 가운데에 투영된 좌표이므로 변경이 필요.
		if(pBri->m_nTypeBridge==BRIDGE_TYPE_TRAP || pBri->m_nTypeBridge==BRIDGE_TYPE_TRAP2)
		{
			CTwinVector tvSectSlabCenLine;
			tvSectSlabCenLine.m_v1		= pBri->GetXySectionSttEnd(TRUE, 0);
			tvSectSlabCenLine.m_v2		= pBri->GetXySectionSttEnd(FALSE, 0);
			tvSectSlabCenLine.m_v1.y	-= pBri->m_dTS/2;
			tvSectSlabCenLine.m_v2.y	-= pBri->m_dTS/2;
			CDPoint xyResult;
			GetXyMatchSegAndLine(tvSectSlabCenLine.m_v1, tvSectSlabCenLine.m_v2, tvWall.GetXyTop(), tvWall.GetXyDir(), xyResult);
			double dOffset = xyMatch.x - xyResult.x;
			CTwinVector tvWallLower;
			if(!pWall->m_tvArrPlan.GetTvByInfo(_T("벽체평면하면"), tvWallLower))		continue;
			xyPlan += tvWallLower.GetXyDir()*dOffset; 
		}

		CDRect rcUpperSlab	= tvArrCrossSection.GetRect();
		CDPoint xyOrg	= CDPoint(rcUpperSlab.right, 0);
		CVector vMove	= xyPlan - xyOrg;
		edgeArrLine.Move(vMove);		
		
		CDPoint vAngPlan	= (tvWallLeftRight.GetXyBottom() - tvWallLeftRight.GetXyTop()).Unit();
		CDPoint vAngOrg		= CDPoint(1, 0);
		CVector vAngRot		= ToDPointFrDegree(vAngPlan.GetAngleDegree() - vAngOrg.GetAngleDegree());
		edgeArrLine.Rotate(xyPlan, vAngRot);

		CDPoint xyFront		= (tvWall.GetXyBottom() - tvWall.GetXyTop()).Unit();
		xyOrg				= CDPoint(0,-1);
		CDPoint xyRot		= ToDPointFrDegree(xyFront.GetAngleDegree()	- xyOrg.GetAngleDegree());
		long i=0;
		for(i=0; i<2; i++)
		{
			CUbGeoCurve *pEdge	= &edgeArrLine.GetAt(i);
			CVector vOrigin = CVector(edgeArrLine.GetAt(0).m_v1);
			vOrigin.z	= edgeArrLine.GetAt(0).m_v2.z;
			CVector vAxis = (tvWallLeftRight.GetXyBottom() - tvWallLeftRight.GetXyTop()).Unit();//tvTemp.GetXyDir();
			if(i==iSTT)
			{
				CVector vOrigin = CVector(pEdge->m_v1);
				vOrigin.z	= pEdge->m_v2.z;
				pEdge->m_v1	= GetRotatePos(pEdge->m_v1, vOrigin, vAxis, xyRot);
				vOrigin = CVector(pEdge->m_vCen);
				vOrigin.z	= pEdge->m_v2.z;
				pEdge->m_vCen	= GetRotatePos(pEdge->m_vCen, vOrigin, vAxis, xyRot);
			}
			else if(i==iEND)
			{
				CVector vOrigin = CVector(pEdge->m_v2);
				vOrigin.z	= pEdge->m_v1.z;
				pEdge->m_v2	= GetRotatePos(pEdge->m_v2, vOrigin, vAxis, xyRot);
				vOrigin = CVector(pEdge->m_vCen);
				vOrigin.z	= pEdge->m_v1.z;
				pEdge->m_vCen	= GetRotatePos(pEdge->m_vCen, vOrigin, vAxis, xyRot);
			}
		}
		

		CUbEttSolid *pSolid = NULL;
		pSolid	= new CUbEttSolid();
		pSolid->m_nUnit = UNIT_MM;
		//

		CUbGeoCurve edge;
		for(long nEdge=0;nEdge<edgeArrLine.GetSize();nEdge++)
		{
			edge	= edgeArrLine.GetAt(nEdge);
			CUbGeoCurveArray edgeArrTemp;
			edgeArrTemp.Add(edge);

			TopoDS_Wire aSpine;
			BOOL bMakeWire = MakeWire(edgeArrTemp, aSpine);
			if(bMakeWire == FALSE)	return;
			BRepOffsetAPI_MakePipeShell aSweep(aSpine);
			aSweep.SetMode(Standard_False);

			CUbGeoCurveArray edgeArrSection;
//			if(nEdge==iSTT)
			{
				TrsVectorArray(edgeArrSection, tvArrVertSection, PLN_XOZ , 0);
				CDPoint xyOrg(0,0), xyPlan(0,0);
				CVector vMove(0,0,0), vAngPlan(0,0,0), vAngOrg(0,0,0), vAngRot(0,0,0);

				CVector vPlan;
				if(nEdge==iSTT)					vPlan	= edge.m_v1;
				else								vPlan	= edge.m_v2;
				CVector vOrg = CVector(xyMatch.x, 0, xyMatch.y);
				vMove	= vPlan - vOrg;
				edgeArrSection.Move(vMove);
				vAngPlan	= (tvWallLeft.GetXyBottom() - tvWallRight.GetXyBottom()).Unit();
				vAngOrg	=CDPoint(-1, 0);
				vAngRot	= ToDPointFrDegree(vAngPlan.GetAngleDegree() - vAngOrg.GetAngleDegree());
				edgeArrSection.Rotate(vPlan, vAngRot);

				TopoDS_Face occFace;
				occFace = GetTopoDS_Face(edgeArrSection);
				CUbEttFace *pFace = new CUbEttFace(occFace,UNIT_MM);
				pSolid->AddFace(pFace);
			}
			//
			
			TopoDS_Wire aProfile ;
			bMakeWire = MakeWire(edgeArrSection, aProfile );
			if(bMakeWire == FALSE)	return;
			aSweep.Add(aProfile);
			
			BRepBuilderAPI_TransitionMode aTransition = BRepBuilderAPI_RightCorner;
			aSweep.SetTransitionMode(aTransition);
			
			aSweep.Build();
			TopoDS_Shape aResult;
			if(aSweep.IsDone())
				aResult = aSweep.Shape();
			
			pSolid->SetTopoDS_Shape(aResult);

		}
		AddSolid(pSolid, SOLID_RCWALL);
	}

}

BOOL CUbiARcBridgeData::GetTvArrHunchGuideLine(CTwinVectorArray &tvArrReturn, CRcBridgeApp *pBri, double dSta, CDPoint vAng)
{
	if(pBri==NULL)		return FALSE;
	tvArrReturn.RemoveAll();

	pBri->GetTvCrossSection(tvArrReturn, dSta, vAng, FALSE, FALSE, FALSE, 0, FALSE, FALSE);
	CRebarPlacing rb;
	rb.RemoveMinLengthLine(tvArrReturn, 0);
	tvArrReturn.Sort();
	rb.RemoveMeanlessTwinVector(tvArrReturn);
	TrimByHunchLine(pBri, tvArrReturn, dSta, vAng);
	if(tvArrReturn.GetSize()==0)		return FALSE;
	double dBTH = pBri->m_dTS;
	long i=0;
	for(i=0;i<tvArrReturn.GetSize();i++)
	{
		CTwinVector *pTv = &tvArrReturn.GetAt(i);
		pTv->m_v1.y -= dBTH;
		pTv->m_v2.y -= dBTH;
	}
	if(tvArrReturn.GetSize()<1)		return FALSE;

	double dDiffEl = pBri->GetDiffELSttAndJijumForVertSectionBySta(dSta);
	rb.MoveRebar(tvArrReturn, CDPoint(0,-dDiffEl));
	return TRUE;
}

BOOL CUbiARcBridgeData::TrimByHunchLine(CRcBridgeApp *pBri, CTwinVectorArray &tvArrSlabCrossSection, double dSta, 
										CDPoint vAng, CDPoint vAngWall)
{
	if(pBri==NULL)	return FALSE;
	if(tvArrSlabCrossSection.GetSize()==0)	return FALSE;
	CTwinVectorArray tvArr, tvArrCrossHunch;
	pBri->GetTvCrossHunch(tvArrCrossHunch, dSta, vAng, FALSE, 0, FALSE, FALSE, vAngWall);
	if(tvArrCrossHunch.GetSize()==0)	
	{
		pBri->GetTvCrossSection(tvArr, dSta, vAng, FALSE, TRUE, FALSE, 0, FALSE, FALSE, FALSE, FALSE);
		CRebarPlacing rb;
		rb.RemoveMinLengthLine(tvArr, 0);
		tvArr.Sort();
		rb.RemoveMeanlessTwinVector(tvArr);
		tvArr.GetTvArrByInfo(_T("슬래브하부"), tvArrCrossHunch, FALSE, FALSE);
		if(tvArrCrossHunch.GetSize()==0)		return FALSE;
/*		//CDrawCrossSection::DrawCrossSection(~~~)
		int nJ = pBri->GetJijumNumberByStation(dSta);
		long nCountJijum	= pBri->GetCountJijum();
		CWallApp *pWall = NULL;
		if(nJ == 0 || nJ == pBri->GetCountJijum()-1)
		{
			if(pBri->IsOutWall(nJ==0))
			{
				CWallApp wall;
				BOOL bVert	= Compare(vAng.GetAngleDegree(), 90.0, "=");
				((CRcBridgeRebar*)pBri)->MakeWallByOutWall(wall, nJ==0, !bVert);
				// 가각부 처리
				if(TRUE)//bExp)
				{
					double dWidthLeftExp	= pBri->GetWidthDiffSlabActOrgAndGagak(nJ == 0, TRUE, FALSE);
					double dWidthRightExp	= pBri->GetWidthDiffSlabActOrgAndGagak(nJ == 0, FALSE, FALSE);
					wall.m_dWidthExp[iLEFT]		= dWidthLeftExp;
					wall.m_dWidthExp[iRIGHT]	= dWidthRightExp;
				}
				pWall = &wall;
			}
		}
		else
		{
			pWall	= pBri->GetInWall(nJ-1);
		}

		if(pWall)
		{
			if(!GetTvArrWallLeftSide(pWall, tvArrCrossHunch))		return FALSE;
		}
		else return FALSE;
*/
	}


	CDPoint xyLeft	= tvArrCrossHunch.GetXyLeft();
	CDPoint xyRight	= tvArrCrossHunch.GetXyRight();
	tvArrSlabCrossSection.Sort();
	CRebarPlacing rb;
	rb.TrimTvArrayByLine(tvArrSlabCrossSection, xyLeft, xyLeft+CDPoint(0,10000), TRUE, TRUE,FALSE);
	rb.TrimTvArrayByLine(tvArrSlabCrossSection, xyRight, xyRight+CDPoint(0,10000), FALSE, TRUE, FALSE);
	
	if(tvArrSlabCrossSection.GetSize()==0)	return FALSE;
	else return TRUE;
}


void CUbiARcBridgeData::ModifyFrontToPlanHunch(CRcBridgeApp *pBri, CUbGeoCurveArray &edgeArr, long nHunch, CDRect rcUpperSlab)
{
	if(pBri==NULL || edgeArr.GetSize()==0)		return;
	CDPoint  vAng	= pBri->GetAngleJijum(nHunch);

	// 벽체 하면 우측을 기준으로 함
	CTwinVector tvJijum = pBri->GetTvJijum(nHunch, 0);
	CDPoint xyPlan	= CDPoint(tvJijum.GetXyBottom());	
	//tvJijum.GetXyBottom()값을 켄틸레버의 폭길이와 연단길이를 계산해서 변경.
	CDPoint xyDir = (tvJijum.GetXyTop() - tvJijum.GetXyBottom()).Unit();
	double dLenR	= (pBri->m_dDH[1] + pBri->m_dCantilB[1])/vAng.y;
	xyPlan	+= xyDir * dLenR;
	CDPoint xyOrg	= CDPoint(rcUpperSlab.left, 0);

	CVector vMove	= xyPlan - xyOrg;
	edgeArr.Move(vMove);		
	
	CDPoint vAngPlan	= xyDir;
	CDPoint vAngOrg		= CDPoint(1, 0);
	CDPoint vAngRot		= ToDPointFrDegree(vAngPlan.GetAngleDegree() - vAngOrg.GetAngleDegree());
	edgeArr.Rotate(xyPlan, vAngRot);
}

void CUbiARcBridgeData::MakeRcHunchSection(CUbEttSolid *pSolid, CRcBridgeApp *pBri, CTwinVectorArray &tvArrVertSection, CUbGeoCurveArray &edgeArrLine, BOOL bStt)
{
	if(pBri==NULL)	return;
	if(tvArrVertSection.GetSize()==0)	return;

	long nDir = (bStt==TRUE) ? 1: -1;

	CTwinVectorArray tvArrPlanSide;
	if(!GetTvArrSlabPlanSide(tvArrPlanSide,pBri, (bStt==TRUE)))		return;
	CTwinVectorArray tvArrSttEnd;
	pBri->GetTvPlaneSttEndLine(tvArrSttEnd, (bStt==TRUE)?TRUE:FALSE, FALSE);
	
	CVector vPlan(0,0,0), vOrg(0,0,0);

	CUbGeoCurve edge;
	long nEdge=0;
	for(nEdge=0;nEdge<edgeArrLine.GetSize();nEdge++)
	{
		CUbGeoCurveArray edgeArrSection;
		TrsVectorArray(edgeArrSection, tvArrVertSection, PLN_XOZ, 0);
		CDRect rect	= tvArrVertSection.GetRect();
		vOrg = (bStt==TRUE) ? CVector(rect.left, 0, rect.top) : CVector(rect.right, 0, rect.top);

		edge	= edgeArrLine.GetAt(nEdge);
		if(nEdge==edgeArrLine.GetSize()-1)
			vPlan	= edge.m_v2;
		else
			vPlan	= edge.m_v1;
		
		CVector vMove	= vPlan - vOrg;
		edgeArrSection.Move(vMove);
		CDPoint vAngPlan	= (tvArrSttEnd.GetXyBottom() - tvArrPlanSide.GetXyBottom()).Unit();
		CDPoint vAngOrg	= CDPoint(nDir, 0);
		CDPoint vAngRot	= ToDPointFrDegree(vAngPlan.GetAngleDegree() - vAngOrg.GetAngleDegree());
		edgeArrSection.Rotate(vPlan, vAngRot);
		
		if(edgeArrSection.GetSize() > 2)
		{
			TopoDS_Face occFace;
			occFace = GetTopoDS_Face(edgeArrSection);
			CUbEttFace *pFace = new CUbEttFace(occFace,UNIT_MM);
			pSolid->AddFace(pFace);
		}
	}
}

void CUbiARcBridgeData::AddRebar(CUbEttRebar *pRebar, long nType, CString sItemPathName, CVector vColor)
{
	if(!pRebar) return;
	if(!IsAddEnableRebar()) return;

	CUbEttRebar *pR = new CUbEttRebar(pRebar);

	// 여기들어온 것들은 모두 MM단위다 M단위로 바꾸자. ////
	for(long i = 0; i < pR->GetPathArray().GetSize(); i++)
	{
		CUbGeoCurve *p	= pR->GetPathArray().GetPAt(i);
		p->m_v1 /= 1000;
		p->m_v2 /= 1000;
		p->m_vCen /= 1000;
	}
	pR->m_dDia /= 1000;
	for(i = 0; i < pR->m_arrUbEdge2.GetSize(); i++)
	{
		CUbGeoCurve *p	= pR->m_arrUbEdge2.GetPAt(i);
		p->m_v1 /= 1000;
		p->m_v2 /= 1000;
		p->m_vCen /= 1000;
	}
	//////////////////////////////////////////////////

	long nGroup	= GetIdxGroupRebar();

	pR->m_hEttUniq = 0;
	pR->GetPathArray().Sort();
	pR->m_nGroup	= nGroup;
	pR->m_nUnit	= UNIT_M;

//	pRebar->m_dDia	= pRebar->m_dDia <= 0 ? 19 : pRebar->m_dDia;

/*	CUbEttRebarBundle *pRebarBundle	= NULL;
	if(m_bBundleRebar)
	{
		pRebarBundle	= GetRebarBundle(nGroup);
		pRebarBundle->SetItemPathName(sItemPathName.IsEmpty() ? "제목없음" : sItemPathName);
		pRebarBundle->SetDrawColor(vColor);
	}
*/	
	
	if(nType == REBAR_UNKNOW)
	{
//		CUbEttRebar *p	= new CUbEttRebar(pRebar, UNIT_MM);
		pR->SetItemPathName(sItemPathName.IsEmpty() ? _T("제목없음") : sItemPathName);
		pR->m_nIndex	= GetIdxRebar(nType);
		pR->m_pManage = m_pManage;
		pR->SetDrawColor(vColor);
//		if(m_bBundleRebar)
//			pRebarBundle->m_arrRebar.Add(p);
//		else
//			m_arrRebar.Add(p);
	}
	else if(nType == REBAR_COPPING)
	{
//		CUbEttRebar *p	= new CUbEttRebar(pRebar, UNIT_MM);
		pR->SetItemPathName(sItemPathName.IsEmpty() ? _T("제목없음") : sItemPathName);
		pR->m_nIndex	= GetIdxRebar(nType);
		pR->m_pManage = m_pManage;
		pR->SetDrawColor(vColor);
//		if(m_bBundleRebar)
//			pRebarBundle->m_arrRebar.Add(p);
//		else
//			m_arrRebar.Add(p);
	}
	else if(nType == REBAR_COLUMN)
	{
//		CUbEttRebar *p	= new CUbEttRebar(pRebar, UNIT_MM);
		pR->SetItemPathName(sItemPathName.IsEmpty() ? _T("제목없음") : sItemPathName);
		pR->m_nIndex	= GetIdxRebar(nType);
		pR->m_pManage = m_pManage;
		pR->SetDrawColor(vColor);
//		if(m_bBundleRebar)
//			pRebarBundle->m_arrRebar.Add(p);
//		else
//			m_arrRebar.Add(p);
	}
	else if(nType == REBAR_FOOTING)
	{
//		CUbEttRebar *p	= new CUbEttRebar(pRebar, UNIT_MM);
		pR->SetItemPathName(sItemPathName.IsEmpty() ? _T("제목없음") : sItemPathName);
		pR->m_nIndex	= GetIdxRebar(nType);
		pR->m_pManage = m_pManage;
		pR->SetDrawColor(vColor);
//		if(m_bBundleRebar)
//			pRebarBundle->m_arrRebar.Add(p);
//		else
//			m_arrRebar.Add(p);
	}
	else if(nType == REBAR_EXFOOTING)
	{
//		CUbEttRebar *p	= new CUbEttRebar(pRebar, UNIT_MM);
		pR->SetItemPathName(sItemPathName.IsEmpty() ? _T("제목없음") : sItemPathName);
		pR->m_nIndex	= GetIdxRebar(nType);
		pR->m_pManage = m_pManage;
		pR->SetDrawColor(vColor);
//		if(m_bBundleRebar)
//			pRebarBundle->m_arrRebar.Add(p);
//		else
//			m_arrRebar.Add(p);
	}
	else if(nType == REBAR_PILE)
	{
//		CUbEttRebar *p	= new CUbEttRebar(pRebar, UNIT_MM);
		pR->SetItemPathName(sItemPathName.IsEmpty() ? _T("제목없음") : sItemPathName);
		pR->m_nIndex	= GetIdxRebar(nType);
		pR->m_pManage = m_pManage;
		pR->SetDrawColor(vColor);
//		if(m_bBundleRebar)
//			pRebarBundle->m_arrRebar.Add(p);
//		else
//			m_arrRebar.Add(p);
	}
	else if(nType == REBAR_SLAB)
	{
//		CUbEttRebar *p	= new CUbEttRebar(pRebar, UNIT_MM);
		pR->SetItemPathName(sItemPathName.IsEmpty() ? _T("제목없음") : sItemPathName);
		pR->m_nIndex	= GetIdxRebar(nType);
		pR->m_pManage = m_pManage;
		pR->SetDrawColor(vColor);
//		if(m_bBundleRebar)
//			pRebarBundle->m_arrRebar.Add(p);
//		else
//			m_arrRebar.Add(p);
	}
	else if(nType == REBAR_GUARDWALL)
	{
//		CUbEttRebar *p	= new CUbEttRebar(pRebar, UNIT_MM);
		pR->SetItemPathName(sItemPathName.IsEmpty() ? _T("제목없음") : sItemPathName);
		pR->m_nIndex	= GetIdxRebar(nType);
		pR->m_pManage = m_pManage;
		pR->SetDrawColor(vColor);
//		if(m_bBundleRebar)
//			pRebarBundle->m_arrRebar.Add(p);
//		else
//			m_arrRebar.Add(p);
	}
	else if(nType == REBAR_RCWALL)
	{
//		CUbEttRebar *p	= new CUbEttRebar(pRebar, UNIT_MM);
		pR->SetItemPathName(sItemPathName.IsEmpty() ? _T("제목없음") : sItemPathName);
		pR->m_nIndex	= GetIdxRebar(nType);
		pR->m_pManage = m_pManage;
		pR->SetDrawColor(vColor);
//		if(m_bBundleRebar)
//			pRebarBundle->m_arrRebar.Add(p);
//		else
//			m_arrRebar.Add(p);
	}
	else if(nType == REBAR_WINGWALL)
	{
//		CUbEttRebar *p	= new CUbEttRebar(pRebar, UNIT_MM);
		pR->SetItemPathName(sItemPathName.IsEmpty() ? _T("제목없음") : sItemPathName);
		pR->m_nIndex	= GetIdxRebar(nType);
		pR->m_pManage = m_pManage;
		pR->SetDrawColor(vColor);
//		if(m_bBundleRebar)
//			pRebarBundle->m_arrRebar.Add(p);
//		else
//			m_arrRebar.Add(p);
	}
	else if(nType == REBAR_APS)
	{
//		CUbEttRebar *p	= new CUbEttRebar(pRebar, UNIT_MM);
		pR->SetItemPathName(sItemPathName.IsEmpty() ? _T("제목없음") : sItemPathName);
		pR->m_nIndex	= GetIdxRebar(nType);
		pR->m_pManage = m_pManage;
		pR->SetDrawColor(vColor);
//		if(m_bBundleRebar)
//			pRebarBundle->m_arrRebar.Add(p);
//		else
//			m_arrRebar.Add(p);
	}
	m_pManage->AddEtt(pR);


	// ZoomAll() 처리를 위해.
//	pRebar->GetRange(m_pManage->m_cbMaxMin);
}

void CUbiARcBridgeData::MakeRcSlabConstruct(CRcBridgeApp *pBri)
{
	if(pBri==NULL)	return;
	double dStaStt	= pBri->GetStationByTarget(BRIDGE_OFF_STT_SLAB,0, TRUE);
	CDPoint vAng	= pBri->GetAngleByTarget(BRIDGE_OFF_STT_SLAB, 0);

	long nHDan = 0;
	long nQtyHDan = pBri->GetQtyHDan();
	for(nHDan=0; nHDan<nQtyHDan; nHDan++)
	{
//		CTwinVectorArray	tvArrSlabConstruct;
//		if(!GetTvArrSlabConstruct(tvArrSlabConstruct, pBri, dStaStt, vAng, nHDan))		continue;

//		long nDan = (nHDan>4 && nQtyHDan>8) ? nHDan+1: nHDan;

		CUbEttSolid *pSolid = NULL;
		pSolid	= new CUbEttSolid();
		pSolid->m_nUnit = UNIT_MM;

		MakeRcSlabConstructPlan(pSolid, pBri, nHDan);
		MakeRcSlabConstructSection(pSolid, pBri, nHDan);

		AddSolid(pSolid, SOLID_GUARDWALL);
	}
}

BOOL CUbiARcBridgeData::GetTvArrSlabConstruct(CTwinVectorArray &tvArrSlabConstruct, CRcBridgeApp *pBri, 
																			double dStaStt, CDPoint vAng, long nHDan)
{
	if(pBri==NULL)	return FALSE;
	tvArrSlabConstruct.RemoveAll();

	CGuardWallRC* pGW = NULL;
	pGW = pBri->GetGuardWallByHDan(nHDan);
	if(pGW == NULL)		return FALSE;
	if(pGW->IsTypeHDanRailTotalDosang())							return FALSE;				//직결식도상, 자갈도상 이면
	if(pGW->IsTypeHDanRailFootwayBlock())							return FALSE;				//연석+보도
	CString str = pBri->GetStringTypeHDan(nHDan, FALSE);
	if(str == _T("없음"))															return FALSE;
	BOOL bInclPave = (str == _T("차도")) ? TRUE : FALSE;


	// 구조물 TvArr구하기.
	if(bInclPave==FALSE)
	{
		//pGW->GetTvGuardWallBase(tvArrSlabConstruct, TRUE, FALSE);
		pBri->GetTvGuardWall(pGW, tvArrSlabConstruct, dStaStt, vAng, TRUE, FALSE, FALSE, TRUE);
		if(tvArrSlabConstruct.GetSize()==0)		return FALSE;
	}
	else
	{
		
		BOOL bInclFootway = TRUE;//pGW->IsTypeHDanRoadFootway();
		pBri->GetTvCrossSection(tvArrSlabConstruct, dStaStt, vAng, bInclFootway, FALSE, TRUE, 0, FALSE, bInclPave);
		
		long nHDanTemp = pGW->m_nNumberHDan;
		long nHDanAct = 0;
		double dLenHDan = 0;
		nHDanAct = pBri->GetNumSlabNodeAct(dStaStt, nHDan);
		dLenHDan = pBri->GetLengthHDanAct(dStaStt, vAng, nHDan);
		CDPoint A[30];
		pBri->GetXySlabUpperAct(dStaStt, vAng, A);
		
		CDRect rect = tvArrSlabConstruct.GetRect();
		rect.left = A[nHDanAct].x;
		if(bInclFootway==FALSE)
			rect.left -= 0.00001;
		else
			rect.left += 0.0001;
		
		rect.top += 100.0;
		if(nHDan==pBri->GetQtyHDan()-1)
			rect.right = A[nHDanAct].x + dLenHDan - 30;
		else
		{
			if(bInclFootway==FALSE)
				rect.right = A[nHDanAct].x + dLenHDan + 0.00001;
			else
				rect.right = A[nHDanAct].x + dLenHDan - 0.0001;
		}
		rect.bottom -= 100.0;
		
		CRebarPlacing rb;
		rb.TrimTvArrayByLine(tvArrSlabConstruct, CDPoint(rect.left,rect.bottom), CDPoint(rect.left, rect.top), TRUE, TRUE, FALSE);
		rb.TrimTvArrayByLine(tvArrSlabConstruct, CDPoint(rect.right,rect.bottom), CDPoint(rect.right, rect.top), FALSE, TRUE, FALSE);
		tvArrSlabConstruct.Sort();
		
/*		if(nHDan==pBri->GetQtyHDan()-1)
		{
			tvArrSlabConstruct.RemoveAll();
			pGW->GetTvGuardWallBase(tvArrSlabConstruct, TRUE, FALSE);
			rb.RemoveMinLengthLine(tvArrSlabConstruct,0);
			tvArrSlabConstruct.Sort();
			CTwinVector tvTemp;
			tvTemp.m_v1	= tvArrSlabConstruct.GetAt(0).m_v1;
			tvTemp.m_v2	= tvArrSlabConstruct.GetAt(tvArrSlabConstruct.GetSize()-1).m_v2;
			tvArrSlabConstruct.Add(tvTemp);
			tvArrSlabConstruct.Move(A[nHDanAct]);
		}
*/		//
		if(bInclPave)
		{
/*			CTwinVector tvTemp = tvArrSlabConstruct.GetAt(0);
			CDPoint xyV1Max(tvTemp.m_v1), xyV1Min(tvTemp.m_v1), xyV2Max(tvTemp.m_v2), xyV2Min(tvTemp.m_v2);
			
			long nTv = 1;
			for(nTv=1; nTv<tvArrSlabConstruct.GetSize(); nTv++)
			{
				tvTemp = tvArrSlabConstruct.GetAt(nTv);
				if(Compare(xyV1Max.y, tvTemp.m_v1.y, _T("<")))
				{
					xyV1Max.x = tvTemp.m_v1.x;			xyV1Max.y = tvTemp.m_v1.y;
				}
				if(Compare(xyV1Min.y, tvTemp.m_v1.y, _T(">")))
				{
					xyV1Min.x = tvTemp.m_v1.x;			xyV1Min.y = tvTemp.m_v1.y;
				}
				if(Compare(xyV2Max.y, tvTemp.m_v2.y, _T("<")))
				{
					xyV2Max.x = tvTemp.m_v2.x;			xyV2Max.y = tvTemp.m_v2.y;
				}
				if(Compare(xyV2Min.y, tvTemp.m_v2.y, _T(">")))
				{
					xyV2Min.x = tvTemp.m_v2.x;			xyV2Min.y = tvTemp.m_v2.y;
				}
			}
			CDPointArray xyArrRect;
			xyArrRect.Add(xyV1Max);
			xyArrRect.Add(xyV2Max);
			xyArrRect.Add(xyV2Min);
			xyArrRect.Add(xyV1Min);
			
			tvArrSlabConstruct.RemoveAll();
			for(long h=0; h<4; h++)
			{
				long nNext = (h==3) ? 0 : h+1;
				CTwinVector tv;
				tv.m_v1 = xyArrRect.GetAt(h);
				tv.m_v2 = xyArrRect.GetAt(nNext);
				tvArrSlabConstruct.Add(tv);
			}
			if(tvArrSlabConstruct.GetSize()!=4)		return FALSE;
*/
			long nTvNum = tvArrSlabConstruct.GetSize()/6;
			CTwinVectorArray tvArrUpper, tvArrLower;
			long i=0,j=0;
			for(j=0;j<nTvNum;j++)
				tvArrUpper.Add(tvArrSlabConstruct.GetAt(j));
			for(i=tvArrSlabConstruct.GetSize()-1,j=0; j<nTvNum; i--,j++)
				tvArrLower.Add(tvArrSlabConstruct.GetAt(i));

			tvArrSlabConstruct.RemoveAll();
			tvArrSlabConstruct.AddFromTvArray(tvArrUpper);
			tvArrSlabConstruct.AddFromTvArray(tvArrLower);
			CTwinVector tvTemp;
			tvTemp.m_v1 = tvArrUpper.GetXyLeft();
			tvTemp.m_v2 = tvArrLower.GetXyLeft();
			tvArrSlabConstruct.Add(tvTemp);
			tvTemp.m_v1 = tvArrUpper.GetXyRight();
			tvTemp.m_v2 = tvArrLower.GetXyRight();
			tvArrSlabConstruct.Add(tvTemp);
			if(tvArrSlabConstruct.GetSize()<4)		return FALSE;
		}
	}

	CRebarPlacing rb;
	rb.RemoveMinLengthLine(tvArrSlabConstruct,0);
	tvArrSlabConstruct.Sort();
	rb.RemoveMeanlessTwinVector(tvArrSlabConstruct);
	double dDiffEl = pBri->GetDiffELSttAndJijumForVertSectionBySta(dStaStt);
	rb.MoveRebar(tvArrSlabConstruct, CDPoint(0, -dDiffEl));
	return TRUE;
}

void CUbiARcBridgeData::MakeRcSlabConstructPlan(CUbEttSolid *pSolid, CRcBridgeApp *pBri, long nHDan)
{
	if(pSolid==NULL)	return;
	if(pBri==NULL)		return;
	CLineInfo *pLineCR = pBri->GetLineBase();
	if(pLineCR==NULL)	return;
	CTwinVectorArray tvArrSlabConstruct;
	
	double dStaStt	= pBri->GetStationByTarget(BRIDGE_OFF_STT_SLAB, 0, TRUE);
	double dStaEnd	= pBri->GetStationByTarget(BRIDGE_OFF_END_SLAB, 0, TRUE);
	CDPoint vAngStt	= pBri->GetAngleByTarget(BRIDGE_OFF_STT_SLAB, 0);
	CDPoint vAngEnd	= pBri->GetAngleByTarget(BRIDGE_OFF_END_SLAB, 0);
	
	TopoDS_Wire wireShape[55];			
	CUbGeoCurveArray edgeArr[55];			
	// 슬래브 연단 계산
	CDPointArray xyArrDistStt, xyArrDistEnd;
	tvArrSlabConstruct.RemoveAll();
	if(!GetTvArrSlabConstruct(tvArrSlabConstruct, pBri, dStaStt, vAngStt, nHDan))		return;
	if(GetXySlabConstructAct(tvArrSlabConstruct, xyArrDistStt)==FALSE)	return;

	tvArrSlabConstruct.RemoveAll();
	if(!GetTvArrSlabConstruct(tvArrSlabConstruct, pBri, dStaEnd, vAngEnd, nHDan))		return;
	if(GetXySlabConstructAct(tvArrSlabConstruct, xyArrDistEnd)==FALSE)	return;
	if(xyArrDistStt.GetSize()==0 || xyArrDistEnd.GetSize()==0)		return;
	// 일단..return;
//	if(xyArrDistStt.GetSize()!=xyArrDistEnd.GetSize())				return;

	CDoubleArray dArrStaStt, dArrStaEnd, dArrDistStt, dArrDistEnd;
	if(GetStationAndDistMatchVertical(pBri, TRUE, xyArrDistStt, dArrStaStt, dArrDistStt)==FALSE)	return;
	if(GetStationAndDistMatchVertical(pBri, FALSE, xyArrDistEnd, dArrStaEnd, dArrDistEnd)==FALSE)	return;
	if(dArrStaStt.GetSize()==0 || dArrStaEnd.GetSize()==0)											return;
	if(dArrDistStt.GetSize()==0 || dArrDistEnd.GetSize()==0)										return;
	//

	long nDan = 0;
	long i=0, j=0;
	long nQtyHDan = xyArrDistStt.GetSize();
	for(nDan=0; nDan<nQtyHDan; nDan++)
	{
		CTwinVectorArray tvArrLine;
		tvArrLine.AddFromLineInfo((CLineInfoApp*)pLineCR, dArrStaStt.GetAt(nDan), dArrStaEnd.GetAt(nDan), dArrDistStt[nDan]);
		tvArrLine.Sort();
		// Station 계산.
		CDoubleArray dStaIP;
		dStaIP.RemoveAll();
		long nIPsu = pLineCR->m_VIPsu;
		dStaIP.Add(dArrStaStt.GetAt(nDan));
		for(i=0;i<nIPsu;i++)
		{
			if(pLineCR->m_VIPData[i][0]<=dArrStaStt.GetAt(nDan))	continue;
			if(pLineCR->m_VIPData[i][0]>=dArrStaEnd.GetAt(nDan))	break;
			dStaIP.Add(pLineCR->m_VIPData[i][0]);
		}
		dStaIP.Add(dArrStaEnd.GetAt(nDan));
		//
		// Trim
		CRebarPlacing rb;
		CTwinVectorArray tvArrTrimLine;
		for(i=0;i<dStaIP.GetSize();i++)
		{
			tvArrTrimLine = tvArrLine;
			CDPoint xy1(0,0),xy2(0,0),xyPos(0,0);
			if(i==0)
			{
				xyPos = pLineCR->GetXyLineDis(dStaIP[i+1],dArrDistStt[nDan]);
				xy1.x = xyPos.x;
				xy1.y = xyPos.y - 100000;
				xy2.x = xyPos.x;
				xy2.y = xyPos.y + 100000;
				rb.TrimTvArrayByLine(tvArrTrimLine,xy1,xy2,FALSE,TRUE,FALSE);
			}
			else if(i==dStaIP.GetSize()-1)
			{
				xyPos = pLineCR->GetXyLineDis(dStaIP[i],dArrDistStt[nDan]);
				xy1.x = xyPos.x;
				xy1.y = xyPos.y - 100000;
				xy2.x = xyPos.x;
				xy2.y = xyPos.y + 100000;
				rb.TrimTvArrayByLine(tvArrTrimLine,xy1,xy2,TRUE,TRUE,FALSE);
			}
			else
			{
				xyPos = pLineCR->GetXyLineDis(dStaIP[i],dArrDistStt[nDan]);
				xy1.x = xyPos.x;
				xy1.y = xyPos.y - 100000;
				xy2.x = xyPos.x;
				xy2.y = xyPos.y + 100000;
				rb.TrimTvArrayByLine(tvArrTrimLine,xy1,xy2,TRUE,TRUE,FALSE);
				xyPos = pLineCR->GetXyLineDis(dStaIP[i+1],dArrDistStt[nDan]);
				xy1.x = xyPos.x;
				xy1.y = xyPos.y - 100000;
				xy2.x = xyPos.x;
				xy2.y = xyPos.y + 100000;
				rb.TrimTvArrayByLine(tvArrTrimLine,xy1,xy2,FALSE,TRUE,FALSE);
			}

			if(tvArrTrimLine.GetSize()>0)
			{
				if(i==dStaIP.GetSize()-1 && dStaIP.GetSize()==2)	continue;
				CDPointArray xyArrDist;
				CUbGeoCurveArray edgeArrTemp;
				TrsVectorArray(edgeArrTemp, tvArrTrimLine, PLN_XOY, 0);
				//
				if(edgeArrTemp.GetSize()<tvArrTrimLine.GetSize())
				{
					edgeArrTemp.RemoveAll();
					CTwinVectorArray tvArrTemp;
					CUbGeoCurveArray edgeArr;
					for(i=0;i<tvArrTrimLine.GetSize();i++)
					{
						tvArrTemp.RemoveAll();
						tvArrTemp.Add(tvArrTrimLine.GetAt(i));
						TrsVectorArray(edgeArr, tvArrTemp, PLN_XOY , 0);
						edgeArrTemp.Add(edgeArr.GetAt(0));
					}
				}
				//
				long nSize = edgeArrTemp.GetSize();
				for(j=0;j<nSize; j++)
				{
					double dSta = 0.0;
					CDPoint vAng(0,1);
					CDPointArray xyArrDist;
					pLineCR->GetStationMatchVerticalByXy(tvArrTrimLine.GetAt(j).m_v1, dSta);
					tvArrSlabConstruct.RemoveAll();
					vAng = (j==0) ? vAngStt : CDPoint(0,1);
					if(!GetTvArrSlabConstruct(tvArrSlabConstruct, pBri, dSta, vAngStt, nHDan))		continue;
					if(!GetXySlabConstructAct(tvArrSlabConstruct, xyArrDist))				return;
					edgeArrTemp.GetAt(j).m_v1.z = xyArrDist[nDan].y;

					pLineCR->GetStationMatchVerticalByXy(tvArrTrimLine.GetAt(j).m_v2, dSta);
					tvArrSlabConstruct.RemoveAll();
					vAng = (j==0) ? vAngEnd : CDPoint(0,1);
					if(!GetTvArrSlabConstruct(tvArrSlabConstruct, pBri, dSta, vAngEnd, nHDan))		continue;
					if(!GetXySlabConstructAct(tvArrSlabConstruct, xyArrDist))				return;
					edgeArrTemp.GetAt(j).m_v2.z = xyArrDist[nDan].y;

					edgeArrTemp.GetAt(j).m_vCen.z	= (edgeArrTemp.GetAt(j).m_v1.z + edgeArrTemp.GetAt(j).m_v2.z)/2.0;
					edgeArr[nDan].Add(edgeArrTemp.GetAt(j));
				}
			}
		}
	}
	
	//	nQtyHDan = edgeArr.GetLength();
	// 와이어 생성.
	for(i=0, j=0;i<nQtyHDan;i++,j++)
	{

		BOOL bMakeWire = MakeWire(edgeArr[i], wireShape[j]);
		if(bMakeWire == FALSE)			
			return;
	}
	
	nQtyHDan = j;
	// lofting surface 생성.
	BRepOffsetAPI_ThruSections aTSec[55];
	for(i=0;i<nQtyHDan;i++)
	{
		for(j=0; j<2;j++)
		{

			long nIdx = ((i+j) > nQtyHDan-1) ? 0 : (i+j);
			aTSec[i].AddWire(wireShape[nIdx]);
		}
		
		aTSec[i].Init();
		aTSec[i].Build();
		
		// lofting surface에서 shape 추출.
		TopoDS_Shape aShape = aTSec[i].Shape();
		
//		AddSolid(&aShape, SOLID_SLAB, m_sItemPathNameHeader);		
		pSolid->SetTopoDS_Shape(aShape);
	}
}

BOOL CUbiARcBridgeData::GetXySlabConstructAct(CTwinVectorArray &tvArrSlabConstruct, CDPointArray &xyArrDist)
{
	xyArrDist.RemoveAll();
	if(tvArrSlabConstruct.GetSize()==0)		return FALSE;
	CalcXyArrAct(tvArrSlabConstruct, xyArrDist);

	long nQtyHDan = xyArrDist.GetSize();
	if(nQtyHDan!=0)			return TRUE;
	else							return FALSE;
}

void CUbiARcBridgeData::MakeRcSlabConstructSection(CUbEttSolid *pSolid, CRcBridgeApp *pBri, long nHDan)
{
	if(pSolid==NULL)	return;
	if(pBri==NULL)		return;
	CRebarPlacing rb;

	CLineInfo *pLineCR = pBri->GetLineBase();
	if(pLineCR==NULL)	return;
	// 슬래브 연단 계산
	double dStaStt	= pBri->GetStationByTarget(BRIDGE_OFF_STT_SLAB, 0, TRUE);
	double dStaEnd	= pBri->GetStationByTarget(BRIDGE_OFF_END_SLAB, 0, TRUE);
	CDPoint vAngStt	= pBri->GetAngleByTarget(BRIDGE_OFF_STT_SLAB, 0);
	CDPoint vAngEnd	= pBri->GetAngleByTarget(BRIDGE_OFF_END_SLAB, 0);

	CDPointArray xyArrDistStt, xyArrDistEnd;
	if(GetXySlabAct(pBri, dStaStt, vAngStt, xyArrDistStt)==FALSE)	return;
	if(GetXySlabAct(pBri, dStaEnd, vAngEnd, xyArrDistEnd)==FALSE)	return;
	if(xyArrDistStt.GetSize()==0 || xyArrDistEnd.GetSize()==0)		return;

	CDoubleArray dArrStaStt, dArrStaEnd, dArrDistStt, dArrDistEnd;
	if(GetStationAndDistMatchVertical(pBri, TRUE, xyArrDistStt, dArrStaStt, dArrDistStt)==FALSE)	return;
	if(GetStationAndDistMatchVertical(pBri, FALSE, xyArrDistEnd, dArrStaEnd, dArrDistEnd)==FALSE)	return;
	if(dArrStaStt.GetSize()==0 || dArrStaEnd.GetSize()==0)											return;
	if(dArrDistStt.GetSize()==0 || dArrDistEnd.GetSize()==0)										return;
	
	CTwinVectorArray tvArrOrgLine;
	long nDan=0, nQtyHDan = pBri->GetQtyHDan();
	if(nHDan==0)	nDan = nQtyHDan-1;
	tvArrOrgLine.AddFromLineInfo((CLineInfoApp*)pLineCR, dArrStaStt.GetAt(nDan), dArrStaEnd.GetAt(nDan), dArrDistStt[nDan]);
	tvArrOrgLine.Sort();
	if(tvArrOrgLine.GetAt(0).GetXyDir().x < 0)
		rb.ReverseRebar(tvArrOrgLine);
	
	CTwinVectorArray tvArrSlab;
	pBri->GetTvCrossSection(tvArrSlab, dStaStt, vAngStt, FALSE, FALSE, FALSE, 0, FALSE, FALSE);
	nDan = (tvArrSlab.GetSize()!=nQtyHDan && nHDan>nQtyHDan/2) ? nHDan+1: nHDan;
	CTwinVectorArray	tvArrLine;
	tvArrLine.AddFromLineInfo((CLineInfoApp*)pLineCR, dArrStaStt.GetAt(nDan), dArrStaEnd.GetAt(nDan), dArrDistStt[nDan]);
	tvArrLine.Sort();
	if(tvArrLine.GetAt(0).GetXyDir().x < 0)
		rb.ReverseRebar(tvArrLine);

	long nSect	= 0;
	for(nSect=0; nSect<2; nSect++)			// 0:시점, 1:종점
	{
		double dSta = (nSect==iSTT) ? dStaStt : dStaEnd;
		CDPoint vAng = (nSect==iSTT) ? vAngStt : vAngEnd;
		CDPointArray xyArrDist;
		CTwinVectorArray tvArrSlabConstruct;
		if(!GetTvArrSlabConstruct(tvArrSlabConstruct, pBri, dSta, vAng, nHDan))		continue;
//		if(!GetXySlabConstructAct(tvArrSlabConstruct, xyArrDist))	continue;
		CUbGeoCurveArray	edgeArrSlab;
		TrsVectorArray(edgeArrSlab, tvArrSlabConstruct, PLN_XOZ , 0);

		// 실제 위치로 이동
		long nDir = -1;
		CDPoint uXy(0,0);
		if(nSect==iSTT)					uXy = tvArrLine.GetAt(0).m_v1;
		else if(nSect==iEND)			uXy = tvArrLine.GetAt(tvArrLine.GetSize()-1).m_v2;
		
		CDRect rect	= tvArrSlabConstruct.GetRect();
		double dRectLeft = rect.left;
		if(nHDan==0)
		{
			CTwinVectorArray tvArrSlab;
			pBri->GetTvCrossSection(tvArrSlab, dSta, vAng, FALSE, TRUE, FALSE, 0, FALSE, FALSE);
			double dDiffEl = pBri->GetDiffELSttAndJijumForVertSectionBySta(dSta);
			rb.MoveRebar(tvArrSlab, CDPoint(0, -dDiffEl));
			dRectLeft = tvArrSlab.GetXyLeft().x;
		}
		CVector vSection(dRectLeft, 0, 0);
		CVector vPlane(CVector(uXy.x, uXy.y,0));
		CVector vMove = vPlane - vSection;
		edgeArrSlab.Move(vMove);
		
		CDPoint vAngPlan(0,0);
		if(nHDan==0)
		{
			if(nSect == 0)
				vAngPlan	= tvArrLine.GetAt(nSect).m_v1 - tvArrOrgLine.GetAt(nSect).m_v1;
			else if(nSect == 1)
				vAngPlan	= tvArrLine.GetAt(tvArrLine.GetSize()-1).m_v2 - tvArrOrgLine.GetAt(tvArrOrgLine.GetSize()-1).m_v2;
		}
		else
		{
			if(nSect == 0)
				vAngPlan	= tvArrOrgLine.GetAt(nSect).m_v1 - tvArrLine.GetAt(nSect).m_v1;
			else if(nSect == 1)
				vAngPlan	= tvArrOrgLine.GetAt(tvArrOrgLine.GetSize()-1).m_v2 - tvArrLine.GetAt(tvArrOrgLine.GetSize()-1).m_v2;
		}
		CDPoint vAngOrg		= CDPoint(nDir,0);
		CDPoint vAngRot		= ToDPointFrDegree(vAngPlan.GetAngleDegree() - vAngOrg.GetAngleDegree());
		edgeArrSlab.Rotate(uXy, vAngRot);
		//
		
/*		CUbEttSolid* pSolid = NULL;
		pSolid	= new CUbEttSolid();
		pSolid->m_nUnit = UNIT_MM;
		CVector vRGB	= CVector(94,94,94);
		vRGB.x	= vRGB.x / 255.;
		vRGB.y	= vRGB.y / 255.;
		vRGB.z	= vRGB.z / 255.;
*/
		if(edgeArrSlab.GetSize() > 2)
		{
			TopoDS_Face occFace;
			occFace = GetTopoDS_Face(edgeArrSlab);
			CUbEttFace *pFace = new CUbEttFace(occFace,UNIT_MM);
//			pSolid->SetItemPathName(m_sItemPathNameHeader.IsEmpty() ? _T("슬래브 구조물") : m_sItemPathNameHeader);
			pSolid->AddFace(pFace);
/*			pSolid->m_nIndex = GetIdx(SOLID_GUARDWALL);
			pSolid->SetDrawColor(vRGB);
			m_pManage->AddEtt(pSolid);
*/		}
	}
}

void CUbiARcBridgeData::MakeRcBracket(CRcBridgeApp *pBri)
{
	if(pBri==NULL)		return;

	CTwinVectorArray tvArrBracketSect;
	CString strBracket=_T("");

	long nBracket=0;
	for(nBracket=0; nBracket<2; nBracket++)
	{
		if(nBracket==0)
			strBracket	=	_T("시점브라켓");
		else if(nBracket==1)
			strBracket	=	_T("종점브라켓");
		pBri->m_tvArrVertSection.GetTvArrByInfo(strBracket,tvArrBracketSect, FALSE, FALSE);

		CTwinVector tv;
		tv.m_v1	= tvArrBracketSect.GetXyTop();
		tv.m_v2	= tvArrBracketSect.GetXyBottom();
		tvArrBracketSect.Add(tv);

		CTwinVectorArray tvArrPlane, tvArrPlanSide;
		if(!GetTvArrSlabPlanSide(tvArrPlanSide, pBri, (nBracket==iSTT)))		continue;
		CUbGeoCurveArray edgeArrSection;
		TrsVectorArray(edgeArrSection, tvArrBracketSect, PLN_XOZ, 0);

		CDPoint xyPlan	= tvArrPlanSide.GetXyBottom();
		CDPoint xyOrg	= (nBracket==0) ? CDPoint(tvArrBracketSect.GetXyRight().x,0) : CDPoint(tvArrBracketSect.GetXyLeft().x, 0);
		CVector	vMove	= xyPlan - xyOrg;
		edgeArrSection.Move(vMove);

		CDPoint xyDir = tvArrPlanSide.GetXyTop() - tvArrPlanSide.GetXyBottom();
		CVector vDir = CVector(xyDir.x, xyDir.y, 0);
		TopoDS_Shape shp = GetTopoDS_Shape(edgeArrSection, vDir);
		AddSolid(&shp, SOLID_RCWALL, m_sItemPathNameHeader);
	}
}

void CUbiARcBridgeData::MakeRcFootingRebar(CFootingApp *pFooting, BOOL bDrawFootingFront)
{
	if(m_pManage->m_nRebaCreateKind==CUbEttRebar::DRAW_NONE) return;
	CString sItemPathNameHeader	= m_sItemPathNameHeader;

	m_sItemPathNameHeader	= sItemPathNameHeader;
	MakeRcFootingRebar_MainRebar(pFooting);

	m_sItemPathNameHeader	= sItemPathNameHeader;
	if(bDrawFootingFront==FALSE)
		MakeRcFootingRebar_MainRebarSide(pFooting);
	else
		MakeRcFootingRebar_MainRebarSideSlope(pFooting);

	m_sItemPathNameHeader	= sItemPathNameHeader;
	MakeRcFootingRebar_SideRebar(pFooting);

	m_sItemPathNameHeader	= sItemPathNameHeader;
	MakeRcFootingRebar_SupportInnerFront(pFooting);

	m_sItemPathNameHeader	= sItemPathNameHeader;
	MakeRcFootingRebar_ShearRebar(pFooting);
}

void CUbiARcBridgeData::MakeRcFootingRebar_MainRebarSide(CFootingApp *pFooting)
{
	if(!pFooting) return;

	CUbEttRebar rebar;
	CUbGeoCurve edge;
	CTwinVectorArray tvArrRebar;
	CTwinVectorArray tvArrPlan;
	tvArrPlan	= pFooting->m_tvArrplan;
	CTwinVector tvPlanFront	= tvArrPlan.GetTvByInfo("기초평면전면");
	CTwinVector tvPlanRight	= tvArrPlan.GetTvByInfo("기초평면우측1");
	CDPoint vAngPlanFront	= tvPlanFront.GetXyDir();
	CDPoint vAngPlanRight	= tvPlanRight.GetXyDir();
	CDPoint xyMatch(0, 0);
	CString str	= _T("");
	CString sItemPathNameHeader	= m_sItemPathNameHeader;
	long upper=0;
	for(upper=0; upper<2; upper++)
	{
		BOOL bUpper	= (upper==iUPPER);
		
		for(long dan = 0; dan < MAX_REBAR_DAN; dan++)
		{
			
			for(long cycle = 0; cycle < MAX_REBAR_CYCLE; cycle++)
			{
//				long nCountDan	= pFooting->GetDanMainRebar(cycle, bUpper);
				if(pFooting->m_pArrRCMainSide[cycle][upper][dan].GetSize()<=0)
				{
					if(!pFooting->IsExistRebarMainBlockSide(cycle, 0, bUpper)) break;

					CTwinVector tvSup	= pFooting->GetTvRebarSupport_Side(bUpper, dan, TRUE);
					if(Compare(tvSup.GetLength(), 0.0, "=")) break;
					
					CRebarInfoCycle RC;
					RC.m_xyMid1	= tvSup.m_v1;
					RC.m_xyMid2	= tvSup.m_v2;
					
					double dCoverTot	= pFooting->m_dCoverLower[dan] + pFooting->m_dCoverUpper[dan];
					if(bUpper)
					{
						RC.m_xyStt	= tvSup.m_v1 - CDPoint(0, pFooting->GetHeight()/2-dCoverTot);
						RC.m_xyEnd	= tvSup.m_v2 - CDPoint(0, pFooting->GetHeight()/2-dCoverTot);
					}
					else
					{
						RC.m_xyStt	= tvSup.m_v1 + CDPoint(0, pFooting->GetHeight()-dCoverTot);
						RC.m_xyEnd	= tvSup.m_v2 + CDPoint(0, pFooting->GetHeight()-dCoverTot);
					}
					pFooting->m_pArrRCMainSide[cycle][upper][dan].Add(&RC);
				}

				for(long rc = 0; rc < pFooting->m_pArrRCMainSide[cycle][upper][dan].GetSize(); rc++)
				{
					CRebarInfoCycle *pRC	= pFooting->m_pArrRCMainSide[cycle][upper][dan].GetAt(rc);
					if(!pRC->m_bExist)	continue;

					// 철근 만들기
//					if(!MakeRebar(rebar, pRC)) continue;

					IncreaseIdxGroupRebar();
					str.Format(_T("/%d사이클 %d단 %s면 직각방향 주철근%d"), cycle+1, dan+1, bUpper ? _T("상") : _T("하"), rc+1);
					m_sItemPathNameHeader	= sItemPathNameHeader + str;
					
					// 실제 위치로 이동
					ModifySideToPlanFooting(pFooting, rebar.m_arrUbEdge);
					
					// offset하면서 복사
					CTwinVectorArray tvArr, tvArrWall, tvArrAngleFront, tvArrAngleBack;
					pFooting->GetTvRebarSupport_Plane(tvArr, tvArrWall, bUpper, dan, -1, FALSE, FALSE, FALSE);
					long nRebarSize = tvArr.GetSize();
					if(nRebarSize==0)	continue;
					
//					MakeMainRebarBytvArrPlan(tvArr, pRC, REBAR_FOOTING, GetColorRebar(REBARCOLOR_MAIN));
		
					if(nRebarSize > 0)
					{
						CVectorArray vArr;
						GetXyMatchLineAndLine(tvArr.GetAt(0).m_v1, vAngPlanRight, tvPlanFront.m_v1, vAngPlanFront, xyMatch);
						vArr.Add(xyMatch);
						for(long i = 0; i < nRebarSize; i++)
							vArr.Add(tvArr.GetAt(i).m_v1);
						
						OffsetRebar(rebar, vArr, REBAR_FOOTING, GetColorRebar(REBARCOLOR_MAIN));
						
						// 상단 경사가 있는 기초인 경우 주철근 길이가 틀려짐
					}
				}

				if(pFooting->m_pArrRCMainSide[cycle][upper][dan].GetSize()>0)
					pFooting->m_pArrRCMainSide[cycle][upper][dan].RemoveAll();
//					AhTPADelete(&(pFooting->m_pArrRCMainSide[cycle][upper][dan]), (CRebarInfoCycle*)0);
			}
		}
	}
}

void CUbiARcBridgeData::MakeRcFootingRebar_MainRebarSideSlope(CFootingApp *pFooting)
{
	if(!pFooting) return;

	CUbEttRebar rebar;
	CUbGeoCurve edge;
	CTwinVectorArray tvArrRebar;
	CTwinVectorArray tvArrPlan;
	tvArrPlan	= pFooting->m_tvArrplan;
	CTwinVector tvPlanFront	= tvArrPlan.GetTvByInfo(_T("기초평면전면"));
	CTwinVector tvPlanRight	= tvArrPlan.GetTvByInfo(_T("기초평면우측1"));
	CDPoint vAngPlanFront	= tvPlanFront.GetXyDir();
	CDPoint vAngPlanRight	= tvPlanRight.GetXyDir();
	CDPoint xyMatch(0, 0);
	CString str	= _T("");
	CString sItemPathNameHeader	= m_sItemPathNameHeader;
	long upper=0;
	for(upper=0; upper<2; upper++)
	{
		BOOL bUpper	= (upper==iUPPER);
		
		for(long dan = 0; dan < MAX_REBAR_DAN; dan++)
		{
			
			for(long cycle = 0; cycle < MAX_REBAR_CYCLE; cycle++)
			{
				if(!pFooting->IsExistRebarMainBlockSide(cycle, 0, bUpper)) break;
				
				long rc = 0;
				CTwinVectorArray tvArr, tvArrAngleFront, tvArrAngleBack, tvArrWall;
				pFooting->GetTvRebarSupport_Plane(tvArr, tvArrWall, bUpper, dan, -1, FALSE, FALSE, FALSE);

				CTwinVector tvSup;
				tvSup	= pFooting->GetTvRebarSupport_Side(bUpper, dan, TRUE);
				if(Compare(tvSup.GetLength(), 0.0, _T("="))) break;
				
				CRebarInfoCycle RC;
				RC.m_xyMid1	= tvSup.m_v1;
				RC.m_xyMid2	= tvSup.m_v2;

				double dCoverTot	= pFooting->m_dCoverLower[dan] + pFooting->m_dCoverUpper[dan];
				if(bUpper)
				{
					RC.m_xyStt	= tvSup.m_v1 - CDPoint(0, pFooting->GetHeight()/2-dCoverTot);
					RC.m_xyEnd	= tvSup.m_v2 - CDPoint(0, pFooting->GetHeight()/2-dCoverTot);
				}
				else
				{
					RC.m_xyStt	= tvSup.m_v1 + CDPoint(0, pFooting->GetHeight()-dCoverTot);
					RC.m_xyEnd	= tvSup.m_v2 + CDPoint(0, pFooting->GetHeight()-dCoverTot);
				}
//				pFooting->m_pArrRCMainSide[cycle][upper][dan].Add(&RC);
				
				
//				CRebarInfoCycle *pRC	= pFooting->m_pArrRCMainSide[cycle][upper][dan].GetAt(rc);
//				if(!pRC->m_bExist)	continue;
				
				// 철근 만들기
				if(!MakeRebar(rebar, &RC)) continue;
				
				// offset하면서 복사
				// MakeTvArrByRC(CTwinVectorArray &tvArrReturn, CRebarInfoCycle *pRC)
				CTwinVectorArray tvArrMainRC;
				CRebarInfoCycle *pMainRC	= pFooting->GetRebarInfoCycle(cycle, TRUE, dan, FALSE);
				if(!pMainRC || !pMainRC->m_bExist) continue;
				MakeTvArrByRC(tvArrMainRC, pMainRC);
				pMainRC	= pFooting->GetRebarInfoCycle(cycle, FALSE, dan, FALSE);
				if(!pMainRC || !pMainRC->m_bExist) continue;
				MakeTvArrByRC(tvArrMainRC, pMainRC);
				if(tvArrMainRC.GetSize()==0)		continue;
				CDRect rcMain	= tvArrMainRC.GetRect();

/*				double dOffset = rcMain.left - rcSupport.left;
				for(i=0;i<tvArrMainRC.GetSize();i++)
				{
					CTwinVector *pTv = &tvArrMainRC.GetAt(i);
					pTv->m_v1.x	-= dOffset;
					pTv->m_v2.x	-= dOffset;
				}
*/				//
				
				IncreaseIdxGroupRebar();
				str.Format(_T("/%d사이클 %d단 %s면 직각방향 주철근%d"), cycle+1, dan+1, bUpper ? _T("상") : _T("하"), rc+1);
				m_sItemPathNameHeader	= sItemPathNameHeader + str;
				
				// 실제 위치로 이동
				ModifySideToPlanFooting(pFooting, rebar.m_arrUbEdge);
				
				if(tvArr.GetSize() > 0)
				{
					CVectorArray vArr;
					CDPointArray xyArr;
					GetXyMatchLineAndLine(tvArr.GetAt(0).m_v1, vAngPlanRight, tvPlanFront.m_v1, vAngPlanFront, xyMatch);
					vArr.Add(xyMatch);
					long i = 0; 
					for(i = 0; i < tvArr.GetSize(); i++)
						vArr.Add(tvArr.GetAt(i).m_v1);

					tvArrMainRC.Move(CDPoint(vArr.GetAt(1).x,0) - CDPoint(rcMain.left,0));
//					OffsetRebar(rebar, vArr, REBAR_FOOTING, GetColorRebar(REBARCOLOR_MAIN));
					CUbEttRebar rebarTmp(&rebar);
					CVector vStt	= vArr.GetAt(0);
					CVector *pV	= NULL;
					CVector vMove(0, 0, 0);
					double dHeight = 0.0;
					for(i = 1; i < vArr.GetSize(); i++)
					{
						pV	= &vArr.GetAt(i);
						CDPointArray xyArrMatch;
						CRebarPlacing rb;
						rb.GetXyMatchTvArrAndLine(tvArrMainRC, (CDPoint)*pV, CDPoint(0,1), xyArrMatch);

/*						while(xyArrMatch.GetSize()==1 && i<vArr.GetSize()-2)
						{
							pV	= &vArr.GetAt(i+1);
							rb.GetXyMatchTvArrAndLine(tvArrMainRC, (CDPoint)*pV, CDPoint(0,1), xyArrMatch);
						}
*/						
						vMove	= (*pV - vStt);
						rebarTmp	= rebar;
						rebarTmp.Move(vMove);

						if(xyArrMatch.GetSize()==2)
						{
							dHeight = fabs(~(xyArrMatch[0] - xyArrMatch[1]));
							if(bUpper)
							{
								rebarTmp.Move(CVector(0,0,xyArrMatch[0].y-tvSup.m_v1.y));
								long j=0;
								for(j=0;j<rebarTmp.GetPathArray().GetSize();j++)
								{
									CUbGeoCurve *pEdge = &rebarTmp.GetPathArray().GetAt(j);
									if(!Compare(pEdge->m_v1.z,xyArrMatch[0].y, _T("=")))
										pEdge->m_v1.z = xyArrMatch[0].y - dHeight/2;
									if(!Compare(pEdge->m_v2.z,xyArrMatch[0].y, _T("=")))
										pEdge->m_v2.z = xyArrMatch[0].y - dHeight/2;
								}
							}
							else
							{
								rebarTmp.Move(CVector(0,0,xyArrMatch[1].y-tvSup.m_v1.y));
								long j=0;
								for(j=0;j<rebarTmp.GetPathArray().GetSize();j++)
								{
									CUbGeoCurve *pEdge = &rebarTmp.GetPathArray().GetAt(j);
									if(!Compare(pEdge->m_v1.z, xyArrMatch[1].y, _T("=")))
										pEdge->m_v1.z = xyArrMatch[0].y;
									if(!Compare(pEdge->m_v2.z, xyArrMatch[1].y, _T("=")))
										pEdge->m_v2.z = xyArrMatch[0].y;
								}
							}
						}
							// 철근 등록
							AddRebar(&rebarTmp, REBAR_FOOTING, m_sItemPathNameHeader, GetColorRebar(REBARCOLOR_MAIN));
					}
					
					
					// 상단 경사가 있는 기초인 경우 주철근 길이가 틀려짐
/*					if(bUpper)
					{
						CDPoint xyOrg	= vArr.GetAt(0);
						double dHeight	= pFooting->GetHeight();
						long nIdx	= GetCurRebarBundle()->m_arrRebar.GetSize()-1;
						for(i = vArr.GetSize()-1; i > 0; i--)
						{
							double dDist	= ~(xyOrg - vArr.GetAt(i));
							double dHCur	= GetHeightFootingByDist(pFooting, FALSE, dDist);
							if(dHeight != dHCur)
							{
								CUbEttRebar *pRebar	= GetCurRebarBundle()->m_arrRebar.GetAt(nIdx);
								pRebar->Move(CVector(0, 0, dHCur - dHeight));
							}
							nIdx--;
						}
					}
*/				}
//				if(pFooting->m_pArrRCMainSide[cycle][upper][dan].GetSize()>0)
//					AhTPADelete(&pFooting->m_pArrRCMainSide[cycle][upper][dan], (CRebarInfoCycle*)0);
			}
		}
	}
}

// CRebarInfoCycle로 rebar 만들기
BOOL CUbiARcBridgeData::MakeRebar(CUbEttRebar &rebar, CRebarInfoCycle *pRC)
{
	rebar.Init();
	if(!pRC || !pRC->m_bExist) return FALSE;

	if(pRC->m_bUseTvArrRebar)
	{
		MakeRebar(rebar, pRC->m_tvArrRebar);
		return TRUE;
	}

	CUbGeoCurve edge;


	if(!ComparePt(pRC->m_xyStt, pRC->m_xyMid1, _T("=")))
	{
		edge.SetEdge(pRC->m_xyStt, pRC->m_xyMid1);
		rebar.AddEdge(edge, UNIT_M);
	}

	if(!ComparePt(pRC->m_xyMid1, pRC->m_xyMid2, _T("=")))
	{
		if(pRC->m_R_Mid > 0)
		{
			edge.SetEdge(pRC->m_xyMid1, pRC->m_xyMid2, pRC->m_xyCen);
			rebar.AddEdge(edge, UNIT_M);
		}
		else
		{
			edge.SetEdge(pRC->m_xyMid1, pRC->m_xyMid2);
			rebar.AddEdge(edge, UNIT_M);
		}
	}

	if(!ComparePt(pRC->m_xyMid2, pRC->m_xyEnd, _T("=")))
	{
		edge.SetEdge(pRC->m_xyMid2, pRC->m_xyEnd);
		rebar.AddEdge(edge, UNIT_M);
	}

	if(pRC->m_bUseMidExt && !ComparePt(pRC->m_xyEnd, pRC->m_xyMidExt, _T("=")))
	{
		if(pRC->m_R_MidExt > 0)
		{
			edge.SetEdge(pRC->m_xyEnd, pRC->m_xyMidExt, pRC->m_xyCenExt);
			rebar.AddEdge(edge, UNIT_M);
		}
		else
		{
			edge.SetEdge(pRC->m_xyEnd, pRC->m_xyMidExt);
			rebar.AddEdge(edge, UNIT_M);
		}
	}

	return TRUE;
}

// rebar를 vecArrPos 좌표들간의 거리만큼 이동
// 첫번째 점은 무조건 기준점(일반도)이 된다. 따라서 철근이 추가 되지 않음
void CUbiARcBridgeData::OffsetRebar(CUbEttRebar &rebar, CVectorArray &vecArrPos, long nType, CVector vColor)
{
	if(vecArrPos.GetSize() == 0) return;

	CUbEttRebar rebarTmp(&rebar);
	CUbEttRebar rebarTmpAll;

	CVector vStt	= vecArrPos.GetAt(0);
	CVector *pV	= NULL;
	CVector vMove(0, 0, 0);
	for(long i = 1; i < vecArrPos.GetSize(); i++)
	{
		pV	= &vecArrPos.GetAt(i);
		vMove	= (*pV - vStt);
		rebarTmp	= rebar;
		rebarTmp.Move(vMove);

		// 철근 등록
		AddRebar(&rebarTmp, nType, m_sItemPathNameHeader, vColor);

	}
}

CUbEttRebarBundle* CUbiARcBridgeData::GetCurRebarBundle()
{
	return GetRebarBundle(GetIdxGroupRebar());
}

CUbEttRebarBundle* CUbiARcBridgeData::GetRebarBundle(long nGroup)
{
	for(long i = 0; i < m_arrRebarBundle.GetSize(); i++)
	{
		if(m_arrRebarBundle.GetAt(i)->m_nGroup == nGroup)
			return m_arrRebarBundle.GetAt(i);
	}

	CUbEttRebarBundle *pRebarBundle	= new CUbEttRebarBundle;
	pRebarBundle->m_nGroup	= nGroup;
	m_arrRebarBundle.Add(pRebarBundle);

	return pRebarBundle;
}

// 철근 종류별 색상
CVector CUbiARcBridgeData::GetColorRebar(long nType)
{
	CVector vColor	= CVector(0.2, 0.2, 0.2);

	switch(nType)
	{
	case REBARCOLOR_MAIN: vColor		= CVector(1, 0, 0); break;
	case REBARCOLOR_SUPPORT: vColor		= CVector(0, 0, 0); break;
	case REBARCOLOR_SHEAR: vColor		= CVector(0, 1, 0); break;
	case REBARCOLOR_DANCHA: vColor		= CVector(0.5, 0.5, 0.5); break;
	case REBARCOLOR_ROUND: vColor		= CVector(0, 1, 1); break;
	case REBARCOLOR_CROSSTIE: vColor	= CVector(0.5, 0.5, 0.25); break;
	}

	return vColor;
}

// 기초의 높이 구하기
double CUbiARcBridgeData::GetHeightFootingByDist(CFootingApp *pFooting, BOOL bSide, double dDistFromLeft)
{
	CTwinVectorArray tvArrFooting;
	if(bSide)
	{
		if(!pFooting->IsSlopeSide(TRUE) && !pFooting->IsSlopeSide(FALSE)) return pFooting->GetHeight();

		CTwinVectorArray tvArrHidden;
		pFooting->GetTvLeftSide(tvArrFooting, tvArrHidden, FALSE, 0, TRUE, FALSE, FALSE, FALSE);
		
	}
	else
	{
		if(!pFooting->IsSlopeFront(TRUE) && !pFooting->IsSlopeFront(FALSE)) return pFooting->GetHeight();

		tvArrFooting	= pFooting->m_tvArrFront;
	}

	CDPointArray xyArrMatch;
	CRebarPlacing rb;
	CDRect rect	= tvArrFooting.GetRect();
	rb.GetXyMatchTvArrAndLine(tvArrFooting, CDPoint(rect.left + dDistFromLeft, 0), CDPoint(0, 1), xyArrMatch);
	if(xyArrMatch.GetSize() > 1)
	{
		return rb.GetXyEdgePoint(xyArrMatch, UPRIGHT_AREA).y - rb.GetXyEdgePoint(xyArrMatch, DOWNRIGHT_AREA).y;
	}

	return pFooting->GetHeight();
}

BOOL CUbiARcBridgeData::MakeRebar(CUbEttRebar &rebar, CTwinVectorArray &tvArr)
{
	if(tvArr.GetSize() == 0) return FALSE;

	TrsVectorArray(rebar.m_arrUbEdge, tvArr, PLN_XOY, 0);

	return TRUE;
}

// 기초 주철근
void CUbiARcBridgeData::MakeRcFootingRebar_MainRebar(CFootingApp *pFooting)
{
	if(!pFooting) return;

	CUbEttRebar rebar;
	CUbGeoCurve edge;
	CTwinVectorArray tvArrRebar;
	CTwinVectorArray tvArrPlan;
	tvArrPlan	= pFooting->m_tvArrplan;
	CTwinVector tvPlanBack	= tvArrPlan.GetTvByInfo(_T("기초평면후면"));
	CTwinVector tvPlanRight	= tvArrPlan.GetTvByInfo(_T("기초평면우측1"));
	CDPoint vAngPlanBack	= tvPlanBack.GetXyDir();
	CDPoint vAngPlanRight	= tvPlanRight.GetXyDir();
	CDPoint xyMatch(0, 0);
	CString str	= _T("");
	CString sItemPathNameHeader	= m_sItemPathNameHeader;

	for(long upper = 0; upper < 2; upper++)
	{
		BOOL bUpper	= upper == iUPPER;

		for(long dan = 0; dan < MAX_REBAR_DAN; dan++)
		{
			for(long cycle = 0; cycle < MAX_REBAR_CYCLE; cycle++)
			{
				CRebarInfoCycle *pRC	= pFooting->GetRebarInfoCycle(cycle, bUpper, dan, FALSE);
				if(!pRC || !pRC->m_bExist) continue;
				IncreaseIdxGroupRebar();
				str.Format(_T("/%d사이클 %d단 %s면 주철근"), cycle+1, dan+1, bUpper ? _T("상") : _T("하"));
				m_sItemPathNameHeader	= sItemPathNameHeader + str;

				// 철근 만들기
				if(!MakeRebar(rebar, pRC)) continue;
				//TrsPlane(rebar.m_arrUbEdge, PLN_XOZ, 0);
				// 실제 위치로 이동
				//ModifyFrontToPlanFooting(pFooting, rebar.m_arrUbEdge);

				// offset하면서 복사
				CTwinVectorArray tvArr, tvArrAngleFront, tvArrAngleBack;
				pFooting->GetTvRebarMain_Plane(tvArr, tvArrAngleFront, tvArrAngleBack, bUpper, cycle, dan, 0, FALSE, FALSE, FALSE, FALSE);


				for(long ang = 0; ang < 3; ang++)
				{
					CTwinVectorArray *pTvArr	= NULL;
					switch(ang)
					{
					case 0: pTvArr	= &tvArr; break;
					case 1: pTvArr	= &tvArrAngleFront; break;
					case 2: pTvArr	= &tvArrAngleBack; break;
					default: break;
					}

					if(!pTvArr) continue;

					if(pTvArr->GetSize() > 0)
					{
						CTwinVector *pTv	= NULL;
						for(long i = 0; i < pTvArr->GetSize(); i++)
						{
							pTv	= &pTvArr->GetAt(i);

							CUbEttRebar rebarCur;
							rebarCur	= rebar;

							ModifyFrontToPlan(pTv, &rebarCur);
							
							AddRebar(&rebarCur, REBAR_FOOTING, m_sItemPathNameHeader, GetColorRebar(REBARCOLOR_MAIN));
						}
						/*
						CVectorArray vArr;
						GetXyMatchLineAndLine(tvArr.GetAt(0).m_v1, vAngPlanBack, tvPlanRight.m_v1, vAngPlanRight, xyMatch);
						vArr.Add(xyMatch);
						for(long i = 0; i < tvArr.GetSize(); i++)
							vArr.Add(tvArr.GetAt(i).m_v1);

						OffsetRebar(rebar, vArr, REBAR_FOOTING);

						// 상단 경사가 있는 기초인 경우 주철근 길이가 틀려짐
						if(bUpper)
						{
							CDPoint xyOrg	= vArr.GetAt(0);
							double dHeight	= pFooting->GetHeight();
							long nIdx	= GetCurRebarBundle()->m_arrRebar.GetSize()-1;
							for(i = vArr.GetSize()-1; i > 0; i--)
							{
								double dDist	= ~(xyOrg - vArr.GetAt(i));
								double dHCur	= GetHeightFootingByDist(pFooting, TRUE, dDist);
								if(dHeight != dHCur)
								{
									CUbEttRebar *pRebar	= GetCurRebarBundle()->m_arrRebar.GetAt(nIdx--);
									pRebar->Move(CVector(0, 0, dHCur - dHeight));
								}
								else
								{
									nIdx--;
								}
							}
						}
						*/
					}
				}
			}
		}
	}
}

// pRebar : 정면도 철근
// pTvPlan : 평면 기준선
// CTwinVector *pTvRef, CUbEttRebar *pRebarRef ; 회전이나 이동의 기준이 되는 철근과 좌표 (없을 수도 있음)
void CUbiARcBridgeData::ModifyFrontToPlan(CTwinVector *pTvPlan, CUbEttRebar *pRebar, CTwinVector *pTvRef, CUbEttRebar *pRebarRef)
{
	CTwinVector *pTvBase	= pTvRef ? pTvRef : pTvPlan;
	CUbEttRebar *pRebarBase	= pRebarRef ? pRebarRef : pRebar;

	if(!pTvBase || !pRebarBase) return;
	if(pRebarBase->GetPathArray().GetSize() == 0) return;

	

	TrsPlane(pRebar->m_arrUbEdge, PLN_XOZ, 0);
	if(pRebarRef)
		TrsPlane(pRebarRef->m_arrUbEdge, PLN_XOZ, 0);


	CDPoint xyOrg	= pTvBase->m_v1;
	CDPoint vAng	= pTvBase->GetXyDir();

	double dX	= min(pRebarBase->GetPathArray().GetAt(0).m_v1.x, pRebarBase->GetPathArray().GetAt(0).m_v2.x);
	for(long i = 1; i < pRebarBase->GetPathArray().GetSize(); i++)
	{
		dX	= min(min(pRebarBase->GetPathArray().GetAt(i).m_v1.x, pRebarBase->GetPathArray().GetAt(i).m_v2.x), dX);
	}

	// 이동
	CVector vMove	= CDPoint(xyOrg.x, xyOrg.y) - CDPoint(dX, 0);
	pRebar->Move(vMove);

	// 회전
	CDPoint vRot	= vAng;
	pRebar->Rotate(xyOrg, vRot);
}

//전단철근
void CUbiARcBridgeData::MakeRcFootingRebar_ShearRebar(CFootingApp *pFooting)
{
	if(!pFooting) return;

	IncreaseIdxGroupRebar();
	m_sItemPathNameHeader += _T("/전단철근");

	CUbGeoCurve edge;
	CUbEttRebar rebar;

	CTwinVectorArray tvArr;
	pFooting->GetTvRebarShear_Plane(tvArr);

	// 
	double dB	= pFooting->GetHeight(TRUE) - pFooting->m_dCoverUpper[0] - pFooting->m_dCoverLower[0];
	double dC	= dB;
	switch(pFooting->m_rebarShearDetail.m_nType)
	{
	case TYPE_SHEAR_CAP : dC	= 100; break;
	case TYPE_SHEAR_DOUPLE : dC	= dB; break;
	case TYPE_SHEAR_DIGUT : dB	= 100; break;
	case TYPE_SHEAR_DIGUT_BEND : dC	= 100; break;
	case TYPE_SHEAR_SPACER : dC	= 100; break;
	case TYPE_SHEAR_CAP2 : dC	= 100; break;
	}

	// MakeTvArrByRC
	CTwinVectorArray tvArrMainRC;
	CRebarInfoCycle *pMainRC	= pFooting->GetRebarInfoCycle(0, TRUE, 0, FALSE);
	if(!pMainRC || !pMainRC->m_bExist) return;
	MakeTvArrByRC(tvArrMainRC, pMainRC);
	pMainRC	= pFooting->GetRebarInfoCycle(0, FALSE, 0, FALSE);		//	pMainRC	= &pFooting->m_rebarInfoCycle_Cycle1_Ext[0]; 
	if(!pMainRC || !pMainRC->m_bExist) return;
	MakeTvArrByRC(tvArrMainRC, pMainRC);
	if(tvArrMainRC.GetSize()==0)	return;

	long i=0;
	CTwinVector *pTv	= NULL;
	for(i = 0; i < tvArr.GetSize(); i++)
	{
		pTv	= &tvArr.GetAt(i);


		double dHeight = pFooting->m_dEL - pFooting->m_dCoverUpper[0];
		CDPointArray xyArrMatch;
		CRebarPlacing rb;
//		rb.GetXyMatchTvArrAndLine(tvArrMainRC, (CDPoint)pTv->m_v1, (CDPoint)pTv->m_v2, xyArrMatch);
		rb.GetXyMatchTvArrAndLine(tvArrMainRC, (CDPoint)pTv->m_v1, CDPoint(0,1), xyArrMatch);
//		BOOL bOverLineIsNotMatch = FALSE;
//		rb.GetXyMatchTvArrAndTv(tvArrMainRC, *pTv, xyArrMatch, bOverLineIsNotMatch);
		
		if(xyArrMatch.GetSize()!=0)
		{
			dHeight = xyArrMatch[0].y;
			if(xyArrMatch.GetSize()==2)
			{
				dB = fabs(~(xyArrMatch[0] - xyArrMatch[1]));
				if(pFooting->m_rebarShearDetail.m_nType == TYPE_SHEAR_DOUPLE)	dC	= dB;
			}
		}
		//

		edge.m_v1	= pTv->m_v1;
		edge.m_v2	= pTv->m_v2;
		edge.m_v1.z = edge.m_v2.z = dHeight;
		
		rebar.Init();
		m_pMRD->SetPlan(edge);
		m_pMRD->SetRebarAttr(pFooting->m_rebarShearDetail.GetRebarType(pFooting->m_rebarShearDetail.m_nType), pTv->GetLength(), dB, dC, 0, 0, 0, CVector(0, 0, -1));
		m_pMRD->GetRebarDetail(rebar.m_arrUbEdge, rebar.m_arrUbEdge2, m_pMRD->m_nTypeRebar == 3 ? 1 : 0);
		rebar.m_bIsUbEdge2	= rebar.m_arrUbEdge2.GetSize() > 0;

		rebar.m_dDia	= pFooting->m_rebarShearDetail.m_dDia;

		// 철근등록
		AddRebar(&rebar, REBAR_FOOTING, m_sItemPathNameHeader, GetColorRebar(REBARCOLOR_SHEAR));

	}
}

// 측면 철근
void CUbiARcBridgeData::MakeRcFootingRebar_SideRebar(CFootingApp *pFooting)
{
	if(!pFooting) return;

	CUbEttRebar rebar;

	CTwinVectorArray tvArrRebarPlan;
	pFooting->GetTvOffset_Plane(tvArrRebarPlan);

	// 철근 생성
	MakeRebar(rebar, tvArrRebarPlan);

	// 실제 위치로이동
	rebar.Move(CVector(0, 0, pFooting->m_dEL - pFooting->GetHeight()));

	CDPointArray xyArr1, xyArr2;
	pFooting->GetXyRebarSideOut_Front(xyArr1, xyArr2, TRUE, TRUE, FALSE);
	if(xyArr1.GetSize() == 0) return;

	IncreaseIdxGroupRebar();
	m_sItemPathNameHeader += _T("/측면철근");

	// offset하면서 철근 생성
	CVectorArray vArr;
	vArr.Add(CVector(0, 0, pFooting->m_dEL - pFooting->GetHeight()));

	for(long i = 0; i < xyArr1.GetSize(); i++)
		vArr.Add(CVector(0, 0, xyArr1.GetAt(i).y));

	OffsetRebar(rebar, vArr, REBAR_FOOTING, GetColorRebar(REBARCOLOR_SUPPORT));
}

// 정면도에서 점으로 보이는 내측보강철근
// 그리기 살짝 까다로움
void CUbiARcBridgeData::MakeRcFootingRebar_SupportInnerFront(CFootingApp *pFooting)
{
	if(!pFooting) return;

	CUbEttRebar rebar;
	CUbGeoCurve edge;


	// 정면도 내측보강철근 ///////////////////////////////////////////////////
	IncreaseIdxGroupRebar();
	m_sItemPathNameHeader += _T("/교축방향 보강철근");


	CDPointArray xyArr;
	pFooting->GetXyRebarSupportInnerFront(xyArr);

	CTwinVector tvPlanBack	= pFooting->m_tvArrplan.GetTvByInfo(_T("기초평면후면"));	// 철근 진행 방향
	CDPoint vAngPlanBack	= tvPlanBack.GetXyDir();
	CDRect rect	= pFooting->m_tvArrFront.GetRect();
	double dCoverF	= pFooting->GetCoverSide(FALSE, 0, TRUE);
	double dCoverB	= pFooting->GetCoverSide(TRUE, 0, TRUE);
	
	CDPoint xy(0, 0);
	CDoubleArray dArrLen;	// 철근들의 길이도 같이 보관
	for(long i = 0; i < xyArr.GetSize(); i++)
	{
		xy	= xyArr.GetAt(i);
		edge.SetEdge(xy, xy);
		rebar.AddEdge(edge, UNIT_M);
		dArrLen.Add(pFooting->GetLengthByDist(xy.x - rect.left));
	}

	// 실제 위치로 이동
	ModifyFrontToPlanFooting(pFooting, rebar.m_arrUbEdge);

	// 철근 생성
	CUbGeoCurve *pEdge	= NULL;
	CUbEttRebar rebarAdd;
	for(i = 0; i < rebar.GetPathArray().GetSize(); i++)
	{
		pEdge	= &rebar.GetPathArray().GetAt(i);

		rebarAdd.Init();
		pEdge->m_v1	= pEdge->m_v1 + vAngPlanBack * dCoverF;
		pEdge->m_v2	= pEdge->m_v1 + vAngPlanBack * (dArrLen.GetAt(i) - dCoverF - dCoverB);
		edge.SetEdge(pEdge->m_v1+CVector(0, 0, -100), pEdge->m_v1);
		rebarAdd.AddEdge(edge, UNIT_M);

		edge.SetEdge(pEdge->m_v1, pEdge->m_v2);
		rebarAdd.AddEdge(edge, UNIT_M);

		edge.SetEdge(pEdge->m_v2, pEdge->m_v2+CVector(0, 0, -100));
		rebarAdd.AddEdge(edge, UNIT_M);

		rebar.m_dDia	= pFooting->GetDiaRebarSupportInner_Front();

		// 철근 등록
		AddRebar(&rebarAdd, REBAR_FOOTING, m_sItemPathNameHeader, GetColorRebar(REBARCOLOR_SUPPORT));
	}

	// 측면도 보강철근 ///////////////////////////////////////////////////////
	IncreaseIdxGroupRebar();
	m_sItemPathNameHeader += _T("/직각방향 보강철근");


	CDPoint vAngPlanRebar	= vAngPlanBack.Rotate90();
	double dCover	= pFooting->GetCoverSide(TRUE, 0, FALSE);
	double dLenRebar	= pFooting->GetWidth() - dCover * 2;
	pFooting->GetXyRebarSupportInnerSide(xyArr);
	rebar.Init();
	for(i = 0; i < xyArr.GetSize(); i++)
	{
		xy	= xyArr.GetAt(i);
		edge.SetEdge(xy, xy);
		rebar.AddEdge(edge, UNIT_M);
	}

	// 실제 위치로 이동
	ModifySideToPlanFooting(pFooting, rebar.m_arrUbEdge);	

	for(i = 0; i < xyArr.GetSize(); i++)
	{
		pEdge	= &rebar.GetPathArray().GetAt(i);

		rebarAdd.Init();
		pEdge->m_v1	= pEdge->m_v1 + vAngPlanRebar * dCover;
		pEdge->m_v2	= pEdge->m_v1 + vAngPlanRebar * dLenRebar;

		edge.SetEdge(pEdge->m_v1+CVector(0, 0, -100), pEdge->m_v1);
		rebarAdd.AddEdge(edge, UNIT_M);

		edge.SetEdge(pEdge->m_v1, pEdge->m_v2);
		rebarAdd.AddEdge(edge, UNIT_M);

		edge.SetEdge(pEdge->m_v2, pEdge->m_v2+CVector(0, 0, -100));
		rebarAdd.AddEdge(edge, UNIT_M);

		rebar.m_dDia	= pFooting->GetDiaRebarSupportInner_Side();

		// 철근 등록
		AddRebar(&rebarAdd, REBAR_FOOTING, m_sItemPathNameHeader, GetColorRebar(REBARCOLOR_SUPPORT));
	}
}


void CUbiARcBridgeData::MakeRcColumnRebar(CRcBridgeApp *pBri)
{
	if(pBri==NULL)	return;
	CString sItemPathName	= m_sItemPathNameHeader;

	long nCountWall = pBri->GetCountInWall();
	for(long i = 0; i < nCountWall; i++)
	{
		CWallApp *pWall = pBri->GetInWall(i);
		if(pWall==NULL)	continue;
		
		if(pWall->m_bTopIsColumn || pWall->m_bRoundRect)		
		{
			MakeRcTopIsColumnRebar(pWall);
			if(pWall->m_bTopIsColumn)
			{
				m_sItemPathNameHeader	= sItemPathName + _T("/상부거더");
				MakeRcUpperSlabGriderRebar((CRcBridgeRebar*)pBri, i+1);
			}
		}
		else
		{
			m_sItemPathNameHeader	= sItemPathName + _T("/내측벽체 주철근");
			MakeRcColumnRebar_MainRebar(pBri, i);//pWall, tvArrColumnFront, pBri->m_nTypeBridge);

			m_sItemPathNameHeader	= sItemPathName + _T("/내측벽체 배력철근");
			MakeRcColumnRebar_SupportRebar(pWall, pBri->m_dCoverWallMid[0], pBri->m_nTypeBridge);

			m_sItemPathNameHeader	= sItemPathName + _T("/내측벽체 전단철근");
			MakeRcColumnRebar_ShearRebar(pBri, i);//pBri->m_dCoverWallMid[0], i, pBri->m_nTypeBridge);

			if(pBri->m_nTypeBridge==BRIDGE_TYPE_ARCH && i==nCountWall/2)
				MakeRcArchRebar(pBri, i);

		}
	}
}

void CUbiARcBridgeData::MakeRcArchRebar(CRcBridgeApp *pBri, long nNumWall)
{
	CString sItemPathName	= m_sItemPathNameHeader;
	m_sItemPathNameHeader	= sItemPathName + _T("/아크리브 주철근");
	MakeRcArchRebar_MainRebar(pBri);

	m_sItemPathNameHeader	= sItemPathName + _T("/아크리브 배력철근");
	MakeRcArchRebar_SupportRebar(pBri);

	m_sItemPathNameHeader	= sItemPathName + _T("/아크리브 전단철근");
	MakeRcArchRebar_ShearRebar(pBri, nNumWall);
}

void CUbiARcBridgeData::MakeRcArchRebar_MainRebar(CRcBridgeApp *pBri)
{
	if(pBri==NULL)	return;

	IncreaseIdxGroupRebar();
	long nDan = 0;
	for(nDan = 0; nDan < MAX_REBAR_DAN; nDan++)
	{
		long nCycle = 0;
		for(nCycle = 0; nCycle < 1; nCycle++)
		{
			long nUpper = 0;
			for(nUpper = 0; nUpper < 2; nUpper++)
			{
				BOOL bUpper	= (nUpper==iUPPER);
				CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC = &((CRcBridgeRebar*)pBri)->m_pArrRCMainRib[nCycle][nUpper][nDan];
				if(pArrRC==NULL)	continue;

				long nSize = pArrRC->GetSize();	
				for(long rc = 0; rc < nSize; rc++)
				{
					CRebarInfoCycle *pRC = pArrRC->GetAt(rc);
					if(pRC==NULL)		continue;
					if(!pRC->m_bExist)	continue;

					//철근 만들기 - 리브의 철근 tvArr를 따로 구해야 함.
					CParabola pb;
					double dCover	= bUpper ? -pBri->m_dCoverRib[nUpper][nDan] : pBri->m_dCoverRib[nUpper][nDan];
					pBri->m_pbRib[nUpper].GetOffsetParabola(pb, dCover);

					CTwinVectorArray tvArrArchRib;
					GetArchRibRebarTvArrByInfoCycle((CRcBridgeRebar*)pBri, pRC, TRUE, &pb, tvArrArchRib);
					if(tvArrArchRib.GetSize()==0)	continue;

					CUbEttRebar rebar;
					MakeRebar(rebar, tvArrArchRib);

					// offset하면서 복사
					CTwinVectorArray tvArr, tvArrJoint;
					((CRcBridgeRebar*)pBri)->GetTvRebarMain_Plane_ArchRib(tvArr, tvArrJoint, nCycle, nUpper, nDan, rc, 0);
					if(tvArr.GetSize() == 0) continue;
					
//					ModifyFrontToPlan(pTv, &rebarCur);
					CTwinVectorArray *pTvArr	= &tvArr;
					CTwinVector *pTv	= NULL;
					long j = 0;
					for(j = 0; j < pTvArr->GetSize(); j++)
					{
						pTv	= &pTvArr->GetAt(j);
						
						CUbEttRebar rebarCur;
						rebarCur	= rebar;

						TrsPlane(rebarCur.m_arrUbEdge, PLN_XOZ, 0);
						
						// 이동
						CVector vMove	= CDPoint(0, pTv->m_v1.y);
						rebarCur. Move(vMove);
						AddRebar(&rebarCur, REBAR_COLUMN, m_sItemPathNameHeader, GetColorRebar(REBARCOLOR_MAIN));
					}
				}
			}
		}
	}
}

// CARcBridgeDBStandard::DrawRebarInfoCycle함수 참고.
void CUbiARcBridgeData::GetArchRibRebarTvArrByInfoCycle(CRcBridgeRebar *pBri, CRebarInfoCycle *pRC, BOOL bDecreaseSttRebarInFooting, 
																CParabola *pPb, CTwinVectorArray &tvArrArchRib)
{
	if(pRC==NULL)		return;
	tvArrArchRib.RemoveAll();

	long nDrawLeft	=  FALSE;
	CDRect rectLimit		= CDRect(0, 0, 0, 0);

	CRcBridgeData data;

	// 기초 하면일 경우 정착장이 보이도록 위로 약간 뛰워서 그려준다.
	double dGap	= bDecreaseSttRebarInFooting ? 50 : 0;
	if(pRC->m_nPosStt == REBARINFO_POS_FOOTING_LOWER && !pPb)
	{
		pRC->m_xyStt.y	+= dGap;
		pRC->m_xyMid1.y	+= dGap;
		pRC->m_xyMid2.y	+= dGap;
		pRC->m_xyCen.y	+= dGap;
	}

	CTwinVector tv;
	BOOL bSttRebar	= TRUE;

	if(pRC->m_bUseTvArrRebar)
	{
		tvArrArchRib	= pRC->m_tvArrRebar;
	}
	else
	{
		// 라운드 적용할지?(철근이 2개이상인경우..)
		BOOL bApplyRound	= TRUE;

		// 기초에서 끝나는 철근은 무조건 라운드 적용 안함.
		if(pRC->m_nPosEnd == REBARINFO_POS_FOOTING_UPPER || pRC->m_nPosEnd == REBARINFO_POS_FOOTING_LOWER ||
			pRC->m_nPosEnd == REBARINFO_POS_FOOTING_LEFT || pRC->m_nPosEnd == REBARINFO_POS_FOOTING_RIGHT)
			bApplyRound	= FALSE;

		// 단, 상부슬래브와 외측벽체가 연결된 형태는 라운드 적용 함.
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
				data.AddFromParabola(tvArrArchRib, *pPb, dDistStt, dDistEnd);
				tv.m_v1	= pRC->m_xyMid1;
			}
			else
			{
				tv.m_v1	= pRC->m_xyStt;
				tv.m_v2	= pRC->m_xyMid1;
				tv.m_v1.z	= 0;
				tv.m_v2.z	= 0;
				tvArrArchRib.Add(tv);
				tv.m_v1	= tv.m_v2;
			}

			
			tv.m_v2	= pRC->m_xyMid2;

			long nLeftCenter	= -1;
			if(bApplyRound)
			{
				if(pRC->m_nPosStt == REBARINFO_POS_UPPERSLAB_UPPER || pRC->m_nPosEnd == REBARINFO_POS_UPPERSLAB_UPPER ||
				(pRC->m_nPosStt == REBARINFO_POS_STTWALL_OUTTER && pRC->m_nPosEnd == REBARINFO_POS_ENDWALL_OUTTER))
				{
					nLeftCenter	= 1;
				}
				else
				{
					nLeftCenter	= pRC->m_xyMid2.x > pRC->m_xyMid1.x ? -1 : 1;
				}

				// 외측벽체와 연결된 상부슬래브 철근인 경우
				if(pBri->IsMainRebarJointUpperSlabAndOutWall(pRC, bSttRebar))
				{
					nLeftCenter	= bSttRebar ? -1 : 1;
				}

				tv.m_v1.z	= nLeftCenter;
				tv.m_v2.z	= pRC->m_R_Mid;//fabs(pRC->m_xyMid2.x - pRC->m_xyMid1.x);

				if(nLeftCenter == 1)
					tv.m_v2.z = tv.m_v2.z * -1;
			}
			tvArrArchRib.Add(tv);
			if(pPb && pRC->m_nPosStt == REBARINFO_POS_FOOTING_LOWER)
			{
				double dDistStt	= pPb->GetDistByXPos(pRC->m_xyMid2.x);
				double dDistEnd	= pPb->GetDistByXPos(pRC->m_xyEnd.x);

				data.AddFromParabola(tvArrArchRib, *pPb, dDistStt, dDistEnd);
				tv.m_v1	= pPb->GetXyByDist(dDistStt);
				tv.m_v2	= pPb->GetXyByDist(dDistEnd);
			}
			else
			{
				tv.m_v1	= pRC->m_xyMid2;
				tv.m_v2	= pRC->m_xyEnd;
				tv.m_v1.z	= 0;
				tv.m_v2.z	= 0;
				tvArrArchRib.Add(tv);
			}

			// 외측벽체와 연결된 상부슬래브 철근인 경우
			if(pBri->IsMainRebarJointUpperSlabAndOutWall(pRC, bSttRebar))
				nLeftCenter	= bSttRebar ? 1 : -1;

			if(pRC->m_bUseMidExt)
			{
				tv.m_v1		= tv.m_v2;
				tv.m_v2		= pRC->m_xyMidExt;
				tv.m_v1.z	= nLeftCenter;
				tv.m_v2.z	= pRC->m_R_MidExt;
				if(nLeftCenter == 1)
					tv.m_v2.z = tv.m_v2.z * -1;
				tvArrArchRib.Add(tv);
			}

			if(pRC->m_bUseEndExt)
			{
				tv.m_v1		= tv.m_v2;
				tv.m_v2		= pRC->m_xyEndExt;
				tv.m_v1.z	= 0;
				tv.m_v2.z	= 0;
				tvArrArchRib.Add(tv);
			}
		}
		else
		{
			if(pPb)
			{				
				double dDistStt	= pPb->GetDistByXPos(pRC->m_xyStt.x);
				double dDistEnd	= pPb->GetDistByXPos(pRC->m_xyMid1.x);
				data.AddFromParabola(tvArrArchRib, *pPb, dDistStt, dDistEnd);
			}
			else
			{
				tv.m_v1	= pRC->m_xyStt;
				tv.m_v2	= pRC->m_xyMid1;
				tv.m_v1.z	= 0;
				tv.m_v2.z	= 0;
				tvArrArchRib.Add(tv);
			}
		}
	}
	
	if(rectLimit != CDRect(0, 0, 0, 0))
	{
		CTwinVectorArray tvArrLimit;
		tvArrLimit.SetFromRect(rectLimit);
		
		// 영역 안쪽만 남김.
		CRebarPlacing rb;
		CVectorArray vecArrArea;
		tvArrLimit.GetToVectorArray(vecArrArea);
		rb.GetTvArrayByCrossVectorArrayArea(tvArrArchRib, vecArrArea, TRUE);
	}

/*	if(nDrawLeft != 0)
	{
		CRebarPlacing rb;
		CDRect rect	= tvArrArchRib.GetRect();
		double dStaCuttingOrg	= GetStaCuttingOrg();
		CDPoint xy1	= CDPoint(dStaCuttingOrg, rect.bottom-100),
				xy2	= CDPoint(dStaCuttingOrg, rect.top+100);

		CVectorArray vecArr;
		tvArrArchRib.GetToVectorArray(vecArr);
		if(nDrawLeft == -1)
		{
			rb.TrimVectorArrayByLine(vecArr, xy1, xy2, FALSE, TRUE, FALSE);
		}
		else if(nDrawLeft == 1)
		{
			rb.TrimVectorArrayByLine(vecArr, xy1, xy2, TRUE, TRUE, FALSE);
		}
		
		tvArrArchRib.SetFromVectorArray(vecArr);
	}

//	DrawTvArray(pDomP, tvArrArchRib);


	// 기초 하면일 경우 정착장이 보이도록 위로 약간 뛰워서 그려준거 복구
	if(pRC->m_nPosStt == REBARINFO_POS_FOOTING_LOWER && !pPb)
	{
		pRC->m_xyStt.y	-= dGap;
		pRC->m_xyMid1.y	-= dGap;
		pRC->m_xyMid2.y	-= dGap;
		pRC->m_xyCen.y	-= dGap;
	}
*/
}

void CUbiARcBridgeData::MakeRcArchRebar_SupportRebar(CRcBridgeApp *pBri)
{
	if(pBri==NULL)	return;
	CRcBridgeRebar *pBriRebar = (CRcBridgeRebar *)pBri;

	IncreaseIdxGroupRebar();

	long nDan = 0;
	for(nDan = 0; nDan < MAX_REBAR_DAN; nDan++)
	{
		long nUpper = 0;
		for(nUpper = 0; nUpper < 2; nUpper++)
		{
			BOOL bUpper	= (nUpper==iUPPER);
			CTwinVectorArray tvArr;
			pBriRebar->GetTvRebarSupport_Plane_ArchRib(tvArr, bUpper, nDan, 0);

			BOOL bVertDir	= pBriRebar->IsVertDir();	//pFooting && pBri->m_nTypeBridge != BRIDGE_TYPE_ARCH ? TRUE : pBri->IsVertDir();
			long nVertDir	= bVertDir ? -1 : 1;
			CDPointArray xyArr[2];
			CDPointArray xyArrDir[2];
			pBriRebar->GetXyRebarSupport_VertSection_ArchRib(xyArr[iLEFT], xyArrDir[iLEFT], xyArr[iRIGHT], xyArrDir[iRIGHT], bUpper, nDan, nVertDir, TRUE);
			long i	= 0;
			for(i=0;i<tvArr.GetSize();i++)
			{

				long nIndex	= (i<xyArr[iLEFT].GetSize()) ? xyArr[iLEFT].GetSize() - (i+1) : i-xyArr[iLEFT].GetSize();
				CDPointArray *pXyArr	= (i<xyArr[iLEFT].GetSize()) ? &xyArr[iLEFT] : &xyArr[iRIGHT];
				CDPointArray *pXyArrDir	= (i<xyArrDir[iLEFT].GetSize()) ? &xyArrDir[iLEFT] : &xyArrDir[iRIGHT];

				CParabola pbRebar;
				pBriRebar->GetRebarRib(pbRebar, !bUpper, 0);
				CTwinVectorArray tvArrLower;
				pBriRebar->AddFromParabola(tvArrLower, pbRebar);

				CDPoint xyBefore	= pXyArr->GetAt(nIndex);
				CDPoint xyAfter		= pbRebar.GetXyMatchXyAndAngle(pXyArr->GetAt(nIndex), pXyArrDir->GetAt(nIndex));

				CTwinVectorArray tvArrTemp;
				tvArrTemp.Add(tvArr.GetAt(i));
				CUbEttRebar rebar;
				MakeRebar(rebar, tvArrTemp);

				rebar.Move(CVector(pXyArr->GetAt(nIndex).x - tvArr.GetAt(i).m_v1.x,0,pXyArr->GetAt(nIndex).y));
				AddRebar(&rebar, REBAR_COLUMN, m_sItemPathNameHeader, GetColorRebar(REBARCOLOR_SUPPORT));


				tvArrTemp.RemoveAll();
				CTwinVector tv;
				tv.m_v1	= xyBefore;
				tv.m_v2	= xyAfter;
				tvArrTemp.Add(tv);
				rebar.GetPathArray().RemoveAll();
				MakeRebar(rebar, tvArrTemp);
				TrsPlane(rebar.m_arrUbEdge, PLN_XOZ, 0);

				CUbEttRebar rebarTemp;
				rebarTemp	= rebar;
				rebarTemp.Move(CVector(0,tvArr.GetAt(i).m_v1.y,0));
				AddRebar(&rebarTemp, REBAR_COLUMN, m_sItemPathNameHeader, GetColorRebar(REBARCOLOR_SUPPORT));
				rebarTemp	= rebar;
				rebarTemp.Move(CVector(0,tvArr.GetAt(i).m_v2.y,0));
				AddRebar(&rebarTemp, REBAR_COLUMN, m_sItemPathNameHeader, GetColorRebar(REBARCOLOR_SUPPORT));
			}
		}
	}
}

void CUbiARcBridgeData::MakeRcArchRebar_ShearRebar(CRcBridgeApp *pBri, long nNumWall)
{
	if(pBri==NULL)	return;
	CWallApp *pWall = pBri->GetInWall(nNumWall);
	if(pWall==NULL)	return;

	IncreaseIdxGroupRebar();

	CRcBridgeRebar *pBriRebar = (CRcBridgeRebar *)pBri;

	// 정면도 상의 전단철근 좌표 구하기.
	BOOL bVertDir	= pBriRebar->IsVertDir();	//pFooting && pBri->m_nTypeBridge != BRIDGE_TYPE_ARCH ? TRUE : pBri->IsVertDir();
	long nVertDir	= bVertDir ? -1 : 1;
	CTwinVectorArray tvArrVert[2];
	pBriRebar->GetTvRebarShear_VertSection_ArchRib(tvArrVert[iLEFT], tvArrVert[iRIGHT], nVertDir);
	
	// 평면도 전단철근 - 다리 구하는데 사용.
	CTwinVectorArray tvArrPlan;
	pBriRebar->GetTvRebarShear_Plane_ArchRib(tvArrPlan, 0);
	
	CRebarShearDetail *pRebarShearDetail	= &pWall->m_rebarShearDetail;
	if(pRebarShearDetail==NULL)		return;
	long nShearIdx	= 0;
	for(long left = 0; left < 2; left++)
	{
		BOOL bLeft	= left == iLEFT;		
		long i=0;
		for(i=0; i<tvArrVert[left].GetSize(); i++)
		{
			long nIdx	= (left==0) ? tvArrVert[left].GetSize()-1-i : i;
			CTwinVector *pTv	= &tvArrVert[left].GetAt(nIdx);
			if(pTv==NULL)	continue;
			CUbGeoCurve edge, edgeTemp;
			edge.m_v1	= pTv->m_v1;
			edge.m_v2	= pTv->m_v2;
			TrsPlaneOne(edge.m_v1, PLN_XOZ, 0);
			TrsPlaneOne(edge.m_v2, PLN_XOZ, 0);
			TrsPlaneOne(edge.m_vCen, PLN_XOZ, 0);
			// 전단철근의 Normal 방향 //
			CVector vDir	= pWall->m_tvPathPlan.GetXyDir();

			CTwinVector *pTvPlan1	= NULL,	*pTvPlan2	= NULL;
			double dB	= 0.0, dC	= 0.0;
			
			do	{
				edgeTemp = edge;
				pTvPlan1	= &tvArrPlan.GetAt(nShearIdx);
				if(pTvPlan1	== NULL)	continue;
				edgeTemp.Move(CVector(0, pTvPlan1->m_v1.y,0));
				dB	= pTvPlan1->GetLength();
				dC	= dB;
				switch(pRebarShearDetail->m_nType)
				{
				case TYPE_SHEAR_CAP : dC	= 100; break;
				case TYPE_SHEAR_DOUPLE : dC	= dB; break;
				case TYPE_SHEAR_DIGUT : dB	= 100; break;
				case TYPE_SHEAR_DIGUT_BEND : dC	= 100; break;
				case TYPE_SHEAR_SPACER : dC	= 100; break;
				case TYPE_SHEAR_CAP2 : dC	= 100; break;
				}
				CUbEttRebar rebar;
				rebar.Init();
				m_pMRD->SetPlan(edgeTemp);
				m_pMRD->SetRebarAttr(pRebarShearDetail->GetRebarType(pRebarShearDetail->m_nType), edge.GetLength(), dB, dC, 0, 0, 0, vDir);
				m_pMRD->GetRebarDetail(rebar.m_arrUbEdge, rebar.m_arrUbEdge2, m_pMRD->m_nTypeRebar == 3 ? 1 : 0);
				rebar.m_bIsUbEdge2	= rebar.m_arrUbEdge2.GetSize() > 0;
				rebar.m_dDia	= pRebarShearDetail->m_dDia;
				
				// 철근등록
				AddRebar(&rebar, REBAR_FOOTING, m_sItemPathNameHeader, GetColorRebar(REBARCOLOR_SHEAR));
				
				nShearIdx++;
				if(nShearIdx>=tvArrPlan.GetSize())		break;
				
				pTvPlan2	= &tvArrPlan.GetAt(nShearIdx);
				if(pTvPlan2	== NULL)	break;
			}while(pTvPlan1->m_v1.x==pTvPlan2->m_v1.x);
		}
	}
}

void CUbiARcBridgeData::MakeRcTopIsColumnRebar(CWallApp *pWall)
{
	if(m_pManage->m_nRebaCreateKind==CUbEttRebar::DRAW_NONE) return;
	CString sItemPathName	= m_sItemPathNameHeader;

	m_sItemPathNameHeader	= sItemPathName + _T("/주철근");
	MakeRcTopIsColumnRebar_MainRebar(pWall);

	m_sItemPathNameHeader	= sItemPathName + _T("/띠철근");
	MakeRcTopIsColumnRebar_RoundRebar(pWall);

	m_sItemPathNameHeader	= sItemPathName + _T("/크로스타이철근");
	MakeRcTopIsColumnRebar_CrossTieRebar(pWall);
}

void CUbiARcBridgeData::MakeRcTopIsColumnRebar_MainRebar(CWallApp *pWall)
{
	if(pWall==NULL)		return;

	CRebarPlacing rb;
	CUbEttRebar rebar;
	CUbGeoCurve edge;
	CString str	= _T("");
	CString sItemPathNameHeader	= m_sItemPathNameHeader;

	long nCountColumn = pWall->m_nCountColumn;
	if(pWall->m_bTopIsColumn==FALSE && nCountColumn>0)		nCountColumn=1;
	double dDistColumn = 0.0;
	for(long nC=0; nC<nCountColumn; nC++)
	{
		if(nC!=0)
			dDistColumn += pWall->m_dDistColumn[nC];
		CColumnApp *pColumn	= pWall->GetColumn(nC);
		if(!pColumn) continue;

		for(long inner = 0; inner < 2; inner++)
		{
			BOOL bInner	= inner == iINNER;
			for(long dan = 0; dan < MAX_REBAR_DAN; dan++)
			{
				IncreaseIdxGroupRebar();
				str.Format(_T("/%s %d단"), bInner ? _T("내측") : _T("외측"), dan+1);
				
				m_sItemPathNameHeader	= sItemPathNameHeader;
				m_sItemPathNameHeader += str;
				
				for(long bundle = 0; bundle < 4; bundle++)
				{
					CDPointArray xyArrRebarPlan, xyArrDir;
					pColumn->GetXyRebarMain_Plan(xyArrRebarPlan, xyArrDir, dan);//, TRUE, bundle, 0, FALSE, 0);
					if(xyArrRebarPlan.GetSize() == 0) continue;
					if(xyArrDir.GetSize()==0)
					{
						for(long s = 0; s < xyArrRebarPlan.GetSize(); s++)
						{
							xyArrDir.Add((xyArrRebarPlan.GetAt(s)-pColumn->m_xyCen).Unit());
						}
					}
					if(xyArrRebarPlan.GetSize() != xyArrDir.GetSize()) continue;
	
					
					// 실제 위치로 이동
					CDPoint vAngRot	= ToDPointFrDegree(pColumn->m_dAngPlan);
					rb.RotateRebar(xyArrRebarPlan, pColumn->m_xyCen, vAngRot.GetAngleDegree());
					
					// 철근 길이를 정확하게 하기 위해 정면도 철근좌표도 가져옴.
					CTwinVectorArray tvArrRebarFront;
					pColumn->GetTvRebarMain_Front(tvArrRebarFront, FALSE, 0, -1, FALSE);
					
					double dR	= pColumn->GetHeightMainRebar(TRUE, pColumn->GetDiaRebarMain(dan, 0, TRUE));
					double LLL	= 500;
					
					long nIdx	= 0;
					for(long i = 0; i < xyArrRebarPlan.GetSize(); i++)
					{
						if(nIdx >= tvArrRebarFront.GetSize()) nIdx	= 0;
						rebar.Init();
						double dTop	= tvArrRebarFront.GetAt(nIdx).m_v2.y;
						double dBottom	= pColumn->m_dELB + pColumn->m_footing.m_dCoverLower[0];
						nIdx++;
						
						CDPoint xy	= xyArrRebarPlan.GetAt(i);
						CDPoint vAng	= xyArrDir.GetAt(i).Rotate(vAngRot);
						CVector v1	= xy + vAng * (dR + LLL);
						CVector v2	= xy + vAng * (dR);
						v1.z	= dBottom;
						v2.z	= dBottom;
						edge.SetEdge(v1, v2);
						rebar.AddEdge(edge, UNIT_M);
						
						v1	= v2;
						v2	= CVector(xy.x, xy.y, dBottom + dR);
						CVector vC	= CVector(v1.x, v1.y, dBottom + dR);
						edge.SetEdge(v1, v2, vC);
						rebar.AddEdge(edge, UNIT_M);
						
						v1	= v2;
						v2.z	= dTop;
						edge.SetEdge(v1, v2);
						rebar.AddEdge(edge, UNIT_M);
						
						rebar.m_dDia	= pColumn->GetDiaRebarMain(dan, 0, TRUE);
						
						CDPoint xyDir = pWall->m_tvPathPlanLower.GetXyDir();
						rebar.Move(CVector(xyDir.x*dDistColumn, xyDir.y*dDistColumn, 0));//pColumn->m_dELB + pColumn->m_footing.GetHeight()));					
						// 철근 등록
						AddRebar(&rebar, REBAR_COLUMN, m_sItemPathNameHeader, GetColorRebar(REBARCOLOR_MAIN));
					}
				}
			}
		}
	}
}

void CUbiARcBridgeData::MakeRcTopIsColumnRebar_RoundRebar(CWallApp *pWall)
{
	if(pWall==NULL)		return;

	CRebarPlacing rb;
	CUbEttRebar rebar;
	CUbGeoCurve edge;
	CString str	= _T("");
	CString sItemPathNameHeader	= m_sItemPathNameHeader;

	long nCountColumn = pWall->m_nCountColumn;
	if(pWall->m_bTopIsColumn==FALSE && nCountColumn>0)		nCountColumn=1;
	double dDistColumn = 0.0;
	for(long nC=0; nC<nCountColumn; nC++)
	{
		if(nC!=0)
			dDistColumn += pWall->m_dDistColumn[nC];
		CColumnApp *pColumn	= pWall->GetColumn(nC);
		if(!pColumn) continue;
		
		for(long inner = 0; inner < 2; inner++)
		{
			BOOL bInner	= inner == iINNER;
			
			for(long dan = 0; dan < MAX_REBAR_DAN; dan++)
			{
				CTwinVectorArray tvArr;
				pColumn->GetTvRebarRound_Front(tvArr, TRUE, dan);
				if(tvArr.GetSize() == 0) continue;
				
				IncreaseIdxGroupRebar();
				str.Format(_T("/%s %d단"), bInner ? _T("내측") : _T("외측"), dan+1);
				m_sItemPathNameHeader	= sItemPathNameHeader;
				m_sItemPathNameHeader += str;
				
				
				CTwinVector *pTv	= NULL;
				CTwinVectorArray tvArrRebarPlan;
				
				for(long i = 0; i < tvArr.GetSize(); i++)
				{
					pTv	= &tvArr.GetAt(i);
					
					// 상하면 다른 단면이거나 i == 0 일때만 구해줌
					if(i == 0 || !pColumn->m_bIsSameUpperAndLower)
					{
						pColumn->GetTvRebarRound_Plan(tvArrRebarPlan, dan);//, TRUE, pTv->m_v1.y - pColumn->m_dELB);
						
						// 회전
						rb.RotateTvArr(tvArrRebarPlan, pColumn->m_xyCen, pColumn->m_dAngPlan);
					}
					
					// 철근으로 생성
					MakeRebar(rebar, tvArrRebarPlan);
					
					// 실제위치로 이동
					rebar.Move(CVector(0, 0, pTv->m_v1.y));
					
					BOOL bSimbu	= FALSE;
					rebar.m_dDia	= pColumn->GetDiaRebarRound(bInner,dan, bSimbu);
					
					CDPoint xyDir = pWall->m_tvPathPlanLower.GetXyDir();
					rebar.Move(CVector(xyDir.x*dDistColumn, xyDir.y*dDistColumn, 0));//pColumn->m_dELB + pColumn->m_footing.GetHeight()));					
					// 철근 등록
					AddRebar(&rebar, REBAR_COLUMN, m_sItemPathNameHeader, GetColorRebar(REBARCOLOR_ROUND));
				}
			}
		}
	}
}

void CUbiARcBridgeData::MakeRcTopIsColumnRebar_CrossTieRebar(CWallApp *pWall)
{
	if(pWall==NULL)		return;

	long nCountColumn = pWall->m_nCountColumn;
	if(pWall->m_bTopIsColumn==FALSE && nCountColumn>0)		nCountColumn=1;
	double dDistColumn = 0.0;
	for(long nC=0; nC<nCountColumn; nC++)
	{
		if(nC!=0)
			dDistColumn += pWall->m_dDistColumn[nC];
		CColumnApp *pColumn	= pWall->GetColumn(nC);
		if(!pColumn) continue;
		
		IncreaseIdxGroupRebar();
		
		CUbEttRebar rebar;
		CRebarPlacing rb;
		CTwinVectorArray tvArr;
		pColumn->GetTvRebarCrossTie_Front(tvArr, FALSE);
//		pColumn->GetTvRebarRound_Front(tvArr, FALSE, 0);

		CTwinVector *pTv	= NULL;
		for(long i = 0; i < tvArr.GetSize(); i++)
		{
			pTv	= &tvArr.GetAt(i);
			for(long hor = 0; hor < 2; hor++)
			{
				BOOL bHor	= hor == 0;
				
				long nIdx	= atol(LPCSTR(pTv->m_sInfo));
				CTwinVectorArray tvArrRebarPlan;
				//pColumnPlus->GetTvRebarCrossTie_Plan(tvArrRebarPlan, bHor, FALSE, nIdx, pTv->m_v1.y - pColumnPlus->m_dELB);
				pColumn->GetTvRebarCrossTie_Plan(tvArrRebarPlan, bHor);
				
				if(tvArrRebarPlan.GetSize() == 0) continue;
				
				// 원래 각도로 회전
				rb.RotateTvArr(tvArrRebarPlan, pColumn->m_xyCen, pColumn->m_dAngPlan);
				
				// 철근 생성
				MakeRebar(rebar, tvArrRebarPlan);
				
				// 원래 위치로 이동
				rebar.Move(CVector(0, 0, pTv->m_v1.y));
				
				rebar.m_dDia	= pColumn->GetDiaRebarCrossTie(nIdx);
				
				CDPoint xyDir = pWall->m_tvPathPlanLower.GetXyDir();
				rebar.Move(CVector(xyDir.x*dDistColumn, xyDir.y*dDistColumn, 0));// pColumn->m_dELB + pColumn->m_footing.GetHeight()));					
				// 철근 등록
				AddRebar(&rebar, REBAR_COLUMN, m_sItemPathNameHeader, GetColorRebar(REBARCOLOR_CROSSTIE));
			}
		}
	}

}

void CUbiARcBridgeData::MakeRcColumnRebar_MainRebar(CRcBridgeApp *pBri, long nInWallNum)
{
	if(pBri==NULL)		return;
	CWallApp *pWall = pBri->GetInWall(nInWallNum);
	if(pWall==NULL)		return;

	CTwinVectorArray tvArrColumnFront;
	tvArrColumnFront.AddFromTvArray(pWall->m_tvArrFront);
	if(pBri->m_nTypeBridge==BRIDGE_TYPE_TRAP || pBri->m_nTypeBridge==BRIDGE_TYPE_TRAP2)
		CalcVertSectionArchHunchHidden(pBri, nInWallNum, tvArrColumnFront);
	if(tvArrColumnFront.GetSize()==0)		return;

	IncreaseIdxGroupRebar();

	CUbEttRebar rebar, rebarTmp;
	for(long nDan = 0; nDan < MAX_REBAR_DAN; nDan++)
	{
		for(long nCycle = 0; nCycle < MAX_REBAR_CYCLE; nCycle++)
		{
			for(long left = 0; left < 2; left++)
			{
				BOOL bLeftRebar	= (left==0) ? TRUE : FALSE;
				CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC = &pWall->m_pArrRCMain[nCycle][left][nDan];
				if(!pArrRC) return;

				long nSize = pArrRC->GetSize();	
				for(long rc = 0; rc < nSize; rc++)
				{
					CRebarInfoCycle *pRC = pArrRC->GetAt(rc);
					if(pRC)
					{
						if(!pRC->m_bExist)	continue;

						// 철근 만들기
						if(!MakeRebar(rebar, pRC)) continue;
						TrsPlane(rebar.m_arrUbEdge, PLN_XOZ, 0);
						ModifyFrontToPlanColumn(pWall, rebar.m_arrUbEdge, tvArrColumnFront, pBri->m_nTypeBridge);

						// 실제위치로 이동하며 복사.
						CTwinVectorArray tvArr, tvArrJoint, tvArrSide;
						pWall->GetTvRebarMain_Side(tvArr, bLeftRebar, nCycle, nDan, rc, tvArrJoint);
						if(rc == 0)
							pWall->m_tvArrRebarMainSideForDim.AddFromTvArray(tvArr);

						pWall->GetTvLeftSide(tvArrSide, FALSE, TRUE, TRUE, FALSE, 0, TRUE, FALSE);
						CTwinVector tvRight	= tvArrSide.GetTvByInfo(_T("벽체우측"));

						CVector vMove(0, 0, 0);
						long nSide	= 0;
						for(nSide=0; nSide<tvArr.GetSize(); nSide++)
						{
							double dOffset	= tvRight.m_v1.x - tvArr.GetAt(nSide).m_v1.x;
							CDPoint xyDir	= pWall->m_tvPathPlan.GetXyDir() * dOffset;
							rebarTmp	= rebar;
							if(rc == nSize-1)
							{
								CUbGeoCurve *pEdge	= &rebarTmp.GetPathArray().GetAt(rebarTmp.GetPathArray().GetSize()-1);
								if(pEdge->m_v2.z > tvArr.GetAt(nSide).m_v2.y)
									pEdge->m_v2	+= (pEdge->m_v1 - pEdge->m_v2).Unit()
													* (pEdge->m_v2.z-tvArr.GetAt(nSide).m_v2.y);
							}
							rebarTmp.Move(CVector(xyDir.x, xyDir.y, 0));
							
							AddRebar(&rebarTmp, REBAR_COLUMN, m_sItemPathNameHeader, GetColorRebar(REBARCOLOR_MAIN));
						}
					}
				}
			}
		}
	}
}

void CUbiARcBridgeData::ModifyFrontToPlanColumn(CWallApp *pWall, CUbGeoCurveArray &edgeArr, CTwinVectorArray &tvArrColumnFront, long nTypeBridge)
{
	if(pWall==NULL)		return;
	if(edgeArr.GetSize()==0)	return;
	if(tvArrColumnFront.GetSize()==0)		return;
	
	CTwinVector tvWallFrontUpper, tvWallFrontLowerL, tvWallFrontLowerR;
	if(!tvArrColumnFront.GetTvByInfo(_T("벽체상면"), tvWallFrontUpper))				return;
	if(!tvArrColumnFront.GetTvByInfo(_T("벽체하면좌측"), tvWallFrontLowerL))		return;
	if(!tvArrColumnFront.GetTvByInfo(_T("벽체하면우측"), tvWallFrontLowerR))		return;
	CTwinVector tvWallFrontLower;
	tvWallFrontLower.m_v1 = tvWallFrontLowerL.GetXyLeft();
	tvWallFrontLower.m_v2 = tvWallFrontLowerR.GetXyRight();
	CString strInfo(_T("벽체평면하면"));
	if(nTypeBridge==BRIDGE_TYPE_TRAP || nTypeBridge==BRIDGE_TYPE_TRAP2)		strInfo = _T("벽체하면평면하면");
	//평면하면의 가운데를 기준으로 이동.
	CTwinVector tvWallLower = pWall->m_tvArrPlan.GetTvByInfo(strInfo);
	CDPoint xyPlan = tvWallLower.GetXyMid();
	CDPoint xyOrg = CDPoint(tvWallFrontLower.GetXyMid().x, 0);
	CVector vMove = xyPlan - xyOrg;
	edgeArr.Move(vMove);
	
	CDPoint vAngPlan = tvWallLower.GetXyDir();
	CDPoint vAngOrg = CDPoint(1,0);//CDPoint(tvWallFrontLower.GetXyDir().x,0);
	CDPoint vAngRot(ToDPointFrDegree(vAngPlan.GetAngleDegree() - vAngOrg.GetAngleDegree()));
	edgeArr.Rotate(tvWallLower.GetXyMid(), vAngRot);
}

void CUbiARcBridgeData::MakeRcColumnRebar_SupportRebar(CWallApp *pWall, double dOffset, long nTypeBridge)
{
	if(pWall==NULL)		return;

	IncreaseIdxGroupRebar();

	long nDan=0;
	for(nDan = 0; nDan < MAX_REBAR_DAN; nDan++)
	{
		long left=0;
		for(left=0; left<2; left++)
		{
			// 평면도에서 배력철근 좌표 구하기.
			CTwinVectorArray tvArrOffsetPlan;
			if(nTypeBridge==BRIDGE_TYPE_TRAP || nTypeBridge==BRIDGE_TYPE_TRAP2)
			{
				tvArrOffsetPlan.RemoveAll();
				pWall->m_tvArrPlan.GetTvArrByInfo(_T("벽체하면"), tvArrOffsetPlan, FALSE, FALSE);
			}
			else
				tvArrOffsetPlan	= pWall->m_tvArrPlan;

			tvArrOffsetPlan.Sort();
			CRebarPlacing rb;
			rb.GetOffsetTvArray(tvArrOffsetPlan, -dOffset, FALSE);

			CString strInfo	= _T("벽체");
			if(nTypeBridge==BRIDGE_TYPE_TRAP || nTypeBridge==BRIDGE_TYPE_TRAP2)			strInfo += _T("하면");
			CString strSide = (left==iLEFT) ? strInfo+_T("평면좌측") : strInfo+_T("평면우측");
			CTwinVector tvSide	= tvArrOffsetPlan.GetTvByInfo(strSide);

			// 배력철근 만들기.
			CTwinVectorArray tvArr;
			tvArr.Add(tvSide);
			CUbEttRebar rebar;
			MakeRebar(rebar, tvArr);
			

			CDPointArray xyArr;
			pWall->GetXyRebarSupport_Front(xyArr, (left==iLEFT), nDan, FALSE, FALSE);
			if(xyArr.GetSize()==0)		continue;

			rebar.Move(CVector(xyArr.GetAt(0).x - tvSide.GetXyBottom().x, 0, xyArr.GetAt(0).y));
//			rebar.Move(CVector(xyArr.GetAt(0).x - tvSide.GetXyBottom().x, 0, xyArr.GetAt(0).y));
//			rebar.Move(CVector(tvFrontSide.GetXyBottom().x, 0, tvFrontSide.GetXyBottom().y));
			CVectorArray vArr;
			vArr.Add(CVector(xyArr.GetAt(0).x, 0, xyArr.GetAt(0).y));
			long i	= 0;
			for(i=0; i<xyArr.GetSize(); i++)
				vArr.Add(CVector(xyArr.GetAt(i).x, 0, xyArr.GetAt(i).y));

			OffsetRebar(rebar, vArr, REBAR_COLUMN, GetColorRebar(REBARCOLOR_SUPPORT));

//			AddRebar(&rebar, REBAR_COLUMN, m_sItemPathNameHeader,GetColorRebar());
		}
	}
}

void CUbiARcBridgeData::MakeRcColumnRebar_ShearRebar(CRcBridgeApp *pBri, long nInWallNum)
{
	if(pBri==NULL)		return;
	CWallApp *pWall = pBri->GetInWall(nInWallNum);
	if(pWall==NULL)		return;
	double dOffset = pBri->m_dCoverWallMid[0];

	IncreaseIdxGroupRebar();
//	m_sItemPathNameHeader += "/전단철근";
	
	CUbEttRebar rebar;

	BOOL bLeftWall = (nInWallNum==0);
	// 측면 전단철근 
	CTwinVectorArray tvArr;
	pWall->GetTvRebarShear_Side(tvArr, bLeftWall);
	CUbGeoCurveArray edgeArr;
	TrsVectorArray(edgeArr, tvArr, PLN_XOZ, 0);

	// 실제 위치로 이동
	CRebarPlacing rb;
	CTwinVectorArray tvArrPlan;
	tvArrPlan	= pWall->m_tvArrPlan;
	rb.GetOffsetTvArray(tvArrPlan, -dOffset, FALSE);
	CString strInfo =  bLeftWall ? _T("벽체평면좌측") : _T("벽체평면우측");
	CTwinVector tvLeft;
	if(!tvArrPlan.GetTvByInfo(strInfo, tvLeft))		return;

	ModifySideToPlanWall(pWall, edgeArr, tvLeft);
	///////////////////////
	
	// 단면도 전단철근 - 전단철근 길이 구하는데 사용
	CTwinVectorArray tvArrFront;
	pWall->GetTvRebarShear_Front(tvArrFront);
	// 평면도 좌표가 없으므로 단면도를 평면위치로 이동.
	CUbGeoCurveArray edgeArrFront;
	TrsVectorArray(edgeArrFront, tvArrFront, PLN_XOZ, 0);
	long nEdgeFrontSize = edgeArrFront.GetSize();
	if(nEdgeFrontSize==0 || nEdgeFrontSize!= tvArrFront.GetSize())		return;
	ModifyFrontToPlanColumn(pWall, edgeArrFront, tvArrFront, pBri->m_nTypeBridge);
	//////////////////////////////////
	
	CTwinVector *pTv		= NULL;
	CTwinVector *pTvFront	= NULL;
	
	long i = 0;
	long nShearIdx	= 0;
	CUbGeoCurve edge, edgeFront;
	long nSize = edgeArr.GetSize();
	for(i = 0; i < nSize; i++)
	{
		edge = edgeArr.GetAt(i);
		pTv	= &tvArr.GetAt(i);
		
		// Side의 좌표를 평면으로 Modify된 단면좌표로 이동.
		if(nShearIdx >= nEdgeFrontSize) 			continue;
		edgeFront = edgeArrFront.GetAt(nShearIdx);
		pTvFront	= &tvArrFront.GetAt(nShearIdx);
		CVector vPlan = bLeftWall ? edgeFront.m_v1 : edgeFront.m_v2;
		CVector vOrg = edge.m_v1;
		CVector vMove = vPlan - vOrg;
		vMove.y = 0 ;
		edge.m_v1 += vMove;
		edge.m_v2 += vMove;
		edge.m_vCen += vMove;
		
		
		CRebarShearDetail *pRebarShearDetail	= &pWall->m_rebarShearDetail;
		
		double dB	= pTvFront->GetLength();
		double dC	= dB;
		switch(pRebarShearDetail->m_nType)
		{
		case TYPE_SHEAR_CAP : dC	= 100; break;
		case TYPE_SHEAR_DOUPLE : dC	= dB; break;
		case TYPE_SHEAR_DIGUT : dB	= 100; break;
		case TYPE_SHEAR_DIGUT_BEND : dC	= 100; break;
		case TYPE_SHEAR_SPACER : dC	= 100; break;
		case TYPE_SHEAR_CAP2 : dC	= 100; break;
		}
		
		rebar.Init();
		m_pMRD->SetPlan(edge);
		// 전단철근의 Normal 방향 //
		CDPoint xyDir = pTvFront->GetXyDir(!bLeftWall);
		CVector vDir = CVector(xyDir.x, 0, xyDir.y);
		m_pMRD->SetRebarAttr(pRebarShearDetail->GetRebarType(pRebarShearDetail->m_nType), 
										edge.GetLength(), dB, dC, 0, 0, 0, vDir);
		m_pMRD->GetRebarDetail(rebar.m_arrUbEdge, rebar.m_arrUbEdge2, m_pMRD->m_nTypeRebar == 3 ? 1 : 0);
		rebar.m_bIsUbEdge2	= rebar.m_arrUbEdge2.GetSize() > 0;
		
		rebar.m_dDia	= pRebarShearDetail->m_dDia;

		// 철근등록
		AddRebar(&rebar, REBAR_FOOTING, m_sItemPathNameHeader, GetColorRebar(REBARCOLOR_SHEAR));

		long nNext = i+1;
		if(nNext==nSize)	break;
		CTwinVector	*pTvNext	= &tvArr.GetAt(nNext);
		if(pTv->m_v1.y != pTvNext->m_v1.y)
			nShearIdx++;
	}
}

void CUbiARcBridgeData::MakeRcWallRebar(CRcBridgeApp *pBri)
{
	if(pBri==NULL)		return;
	CString sItemPathName	= m_sItemPathNameHeader;
	m_sItemPathNameHeader	= sItemPathName + _T("/외측벽체 주철근");
	MakeRcWallRebar_MainRebar(pBri);

	m_sItemPathNameHeader	= sItemPathName + _T("/외측벽체 배력철근");
	MakeRcWallRebar_SupportRebar(pBri);

	m_sItemPathNameHeader	= sItemPathName + _T("/외측벽체 전단철근");
	MakeRcWallRebar_ShearRebar(pBri);
}

void CUbiARcBridgeData::MakeRcWallRebar_MainRebar(CRcBridgeApp *pBri)
{
	if(pBri==NULL)		return;

	IncreaseIdxGroupRebar();
	CUbEttRebar rebar, rebarTmp;
	for(long nDan = 0; nDan < MAX_REBAR_DAN; nDan++)
	{
		for(long nCycle = 0; nCycle < MAX_REBAR_CYCLE; nCycle++)
		{
			BOOL	bStt	= TRUE,
					bInner	= TRUE;
			for(long stt = 0; stt < 2; stt++)
			{
				bStt	= (stt == iSTT);
				for(long inner = 0; inner < 2; inner++)
				{
					bInner	= (inner == iINNER);
					CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC = &((CRcBridgeRebar*)pBri)->m_pArrRCMainOutWall[nCycle][stt][inner][nDan];
					if(!pArrRC) return;
					
					long nSize = pArrRC->GetSize();	
					for(long rc = 0; rc < nSize; rc++)
					{
						CRebarInfoCycle *pRC = pArrRC->GetAt(rc);
						if(pRC==NULL)	continue;
						if(!pRC->m_bExist)	continue;
						
						// 철근 만들기
						if(!MakeRebar(rebar, pRC)) continue;
						TrsPlane(rebar.m_arrUbEdge, PLN_XOZ, 0);
						
						//ModifyFrontToPlanColumn(pWall, rebar.m_arrUbEdge, tvArrColumnFront, nTypeBridge);
						CTwinVectorArray tvArrSttEnd;
						pBri->GetTvPlaneSttEndLine(tvArrSttEnd, bStt, FALSE);
						// 시점 또는 종점 벽면의 평면도 구하기.
						CTwinVectorArray tvArrWallPlan;
						GetOutWallPlan(pBri, tvArrWallPlan, bStt);

						//외측벽체 정면도 구해서 정면 위치로 이동.
						CTwinVectorArray *pTvArr = &pBri->m_tvArrVertSection;
						CTwinVector tvVerLeft, tvVerRight;
						CTwinVectorArray tvArrTemp;
						if(pTvArr!=NULL)
						{
							if(bStt==FALSE)
							{
								pTvArr->GetTvByInfo(_T("종점벽체좌측면"), tvVerLeft);
								pTvArr->GetTvByInfo(_T("종점벽체우측면"), tvVerRight);
							}
							else if(bStt==TRUE)
							{
								pTvArr->GetTvByInfo(_T("시점벽체좌측면"), tvVerLeft);
								pTvArr->GetTvByInfo(_T("시점벽체우측면"), tvVerRight);
							}
							tvArrTemp.Add(tvVerLeft);
							tvArrTemp.Add(tvVerRight);
							CDRect rc	= tvArrTemp.GetRect();
							
							CTwinVector tvBottom	= tvArrWallPlan.GetTvByInfo(_T("하면"));
							CVector vPlan	= CVector(tvBottom.GetXyLeft().x, tvBottom.GetXyLeft().y,0);
							CVector vOrg	= CVector(tvVerLeft.m_v1.x, 0, 0);
							CVector vMove	= vPlan - vOrg;
							rebar.Move(vMove);
							
							CDPoint xyPlan	= (tvBottom.GetXyRight() - tvBottom.GetXyLeft()).Unit();//tvBottom.GetXyDir();
							CDPoint xyOrg	= (rc.BottomRight() - rc.BottomLeft()).Unit();
							rebar.Rotate(tvBottom.GetXyLeft(), ToDPointFrDegree(xyPlan.GetAngleDegree()-xyOrg.GetAngleDegree()));
						}								
//
//						AddRebar(&rebar, REBAR_RCWALL, m_sItemPathNameHeader, GetColorRebar(REBARCOLOR_MAIN));
						
						// 각각의 실제위치로 이동하며 복사.
//						((CRcBridgeRebar*)pBri)->GetTvRebarMain_Side_OutWallHunch(tvArr, nCycle, bStt);
						CWallApp wall;
						((CRcBridgeRebar*)pBri)->MakeWallByOutWall(wall, bStt, TRUE);
						BOOL bLeftRebar	= FALSE;
						if(bStt==TRUE && bInner==FALSE)		bLeftRebar	= TRUE;
						if(bStt==FALSE && bInner==TRUE)		bLeftRebar	= TRUE;
						CTwinVectorArray tvArr, tvArrJoint;
						wall.GetTvRebarMain_Side(tvArr, bLeftRebar, nCycle, nDan, rc, tvArrJoint);
						if(rc == 0)
							wall.m_tvArrRebarMainSideForDim.AddFromTvArray(tvArr);
						
						
						CTwinVectorArray tvArrRight;
						BOOL bInnerAtLeft	= pBri->m_nTypeWallForRebarGen==0 ? FALSE : TRUE;
						BOOL bInnerAtRight	= bInnerAtLeft;
						
						pBri->GetTvCrossSection_OutWall_Exp(tvArrRight, bStt, FALSE, bInnerAtRight, TRUE);
						TARGET_IN_ONE_SLAB target;
						if(pBri->m_nTypeWallForRebarGen == 0)
							target = (bStt)?BRIDGE_OFF_STT_SLAB:BRIDGE_OFF_END_SLAB;
						else if(pBri->m_nTypeWallForRebarGen == 1)
							target = (bStt)?BRIDGE_OFF_STT_STT:BRIDGE_OFF_END_END;
						double dSta = pBri->GetStationByTarget(target, 0, TRUE);
						double dDiffEl = pBri->GetDiffELSttAndJijumForVertSectionBySta(dSta);
						CRebarPlacing rb;
						rb.MoveRebar(tvArrRight, CDPoint(0, -dDiffEl));
						CTwinVector tvRight	= tvArrRight.GetTvByInfo(_T("벽체우측"));
						
						CVector vMove(0, 0, 0);
						long nSide	= 0;
						for(nSide=0; nSide<tvArr.GetSize(); nSide++)
						{
							double dOffset	= tvRight.GetXyBottom().x - tvArr.GetAt(nSide).m_v1.x;
							CDPoint xyDir	= (tvArrSttEnd.GetXyTop()-tvArrSttEnd.GetXyBottom()).Unit() * dOffset;
							rebarTmp	= rebar;
							if(rc == nSize-1)
							{
								CUbGeoCurve *pEdge	= &rebarTmp.GetPathArray().GetAt(rebarTmp.GetPathArray().GetSize()-1);
								if(pEdge->m_v2.z > tvArr.GetAt(nSide).m_v2.y)
									pEdge->m_v2	+= (pEdge->m_v1 - pEdge->m_v2).Unit()
									* (pEdge->m_v2.z-tvArr.GetAt(nSide).m_v2.y);
							}
							rebarTmp.Move(CVector(xyDir.x, xyDir.y, 0));
							
							AddRebar(&rebarTmp, REBAR_COLUMN, m_sItemPathNameHeader, GetColorRebar(REBARCOLOR_MAIN));
						}
					}
				}
			}
		}
	}
}

void CUbiARcBridgeData::MakeRcWallRebar_SupportRebar(CRcBridgeApp *pBri)
{
	if(pBri==NULL)		return;

	IncreaseIdxGroupRebar();

	BOOL	bStt	= TRUE;
	for(long stt = 0; stt < 2; stt++)
	{
		bStt	= (stt == iSTT);
		long nDan=0;
		for(nDan = 0; nDan < MAX_REBAR_DAN; nDan++)
		{
			long inner=0;
			for(inner=0; inner<2; inner++)
			{
				BOOL bInner = (inner==iINNER);
				CDPointArray xyArr;
				((CRcBridgeRebar*) pBri)->GetXyRebarSupport_VertSection_OutWall(xyArr, bStt, bInner, nDan, FALSE);
				if(xyArr.GetSize()==0)		continue;
				
				// 철근 좌표 구하기.
				CRebarPlacing rb;
				CTwinVectorArray tvArrPlan;
				GetOutWallPlan(pBri, tvArrPlan, bStt);
				if(tvArrPlan.GetSize()==0)		continue;

				double dOffset	= (bInner)?pBri->m_dCoverWallIn[0]:pBri->m_dCoverWallOut[0];
				rb.GetOffsetTvArray(tvArrPlan, dOffset, FALSE);

				// 철근 만들기
				CString strSide	= _T("좌측");
				if(bStt==TRUE && bInner==TRUE)		strSide	= _T("우측");
				if(bStt==FALSE && bInner==FALSE)	strSide	= _T("우측");
				CTwinVector tvSide	= tvArrPlan.GetTvByInfo(strSide);
				CTwinVectorArray tvArr;
				tvArr.Add(tvSide);
				CUbEttRebar rebar;
				MakeRebar(rebar, tvArr);

				// 이동하면서 복사 배치하기
				rebar.Move(CVector(0,0,xyArr.GetAt(0).y));
				CVectorArray vArr;
				vArr.Add(CVector(xyArr.GetAt(0).x, 0, xyArr.GetAt(0).y));
				long i	= 0;
				for(i=0; i<xyArr.GetSize(); i++)
					vArr.Add(CVector(xyArr.GetAt(i).x, 0, xyArr.GetAt(i).y));
				
				OffsetRebar(rebar, vArr, REBAR_COLUMN, GetColorRebar(REBARCOLOR_SUPPORT));
				
//				AddRebar(&rebar, REBAR_COLUMN, m_sItemPathNameHeader,GetColorRebar(REBARCOLOR_SUPPORT));
			}
		}
	}
}

void CUbiARcBridgeData::ModifySideToPlanWall(CWallApp *pWall, CUbGeoCurveArray &edgeArr, CTwinVector &tvPlan)
{
	if(pWall==NULL)				return;
	if(edgeArr.GetSize()==0)	return;

	CTwinVectorArray tvArrLeft;
	if(!GetTvArrWallLeftSide(pWall, tvArrLeft))		return;
	
	CDPoint xyPlan	= tvPlan.GetXyBottom();
	CDRect rect	= tvArrLeft.GetRect();
	CDPoint xyOrg	= CDPoint(rect.right, 0);		// 벽체 하면 우측을 기준으로 함
	CVector vMove	= xyPlan - xyOrg;
	edgeArr.Move(vMove);
	
	CDPoint vAngPlan	= (tvPlan.GetXyTop() - tvPlan.GetXyBottom()).Unit();
	CDPoint vAngOrg		= CDPoint(-1, 0);
	CDPoint vAngRot		= ToDPointFrDegree(vAngPlan.GetAngleDegree() - vAngOrg.GetAngleDegree());
	edgeArr.Rotate(xyPlan, vAngRot);
}

//CDrawWall::DrawLeftSide(~~~)
BOOL CUbiARcBridgeData::GetTvArrWallLeftSide(CWallApp *pWall, CTwinVectorArray &tvArrLeft)
{
	if(pWall==NULL)				return FALSE;
	tvArrLeft.RemoveAll();
	pWall->GetTvLeftSide(tvArrLeft, FALSE, TRUE, TRUE, TRUE, FALSE, FALSE, TRUE, TRUE, TRUE);
	
	long nIdx	= tvArrLeft.GetIdxByInfo(_T("벽체좌측"));
	if(nIdx > -1 && pWall->m_dWidthExp[iLEFT] > 0)
	{
		tvArrLeft.RemoveAt(nIdx);
	}
	nIdx	= tvArrLeft.GetIdxByInfo(_T("벽체우측"));
	if(nIdx > -1 && pWall->m_dWidthExp[iRIGHT] > 0)
	{
		tvArrLeft.RemoveAt(nIdx);
	}
	
	long i = 0;
	for(i = 0; i < tvArrLeft.GetSize(); i++)		// 가각부 적용을 하면 상면도 그릴 필요가 없다.
	{
		if(tvArrLeft.GetAt(i).m_sInfo == _T("벽체상면") || tvArrLeft.GetAt(i).m_sInfo == _T("벽체상면끝"))
		{
			tvArrLeft.RemoveAt(i);
			i--;
		}
	}
	if(tvArrLeft.GetSize()==0)		return FALSE;

	return TRUE;
}

void CUbiARcBridgeData::MakeRcWallRebar_ShearRebar(CRcBridgeApp *pBri)
{
	if(pBri==NULL) return;
	
	IncreaseIdxGroupRebar();
//	m_sItemPathNameHeader += "/전단철근";
	
	BOOL	bStt	= TRUE;
	long stt = 0;
	for(stt = 0; stt < 2; stt++)
	{
		bStt	= (stt == iSTT);
		CWallApp wall;
		((CRcBridgeRebar*)pBri)->MakeWallByOutWall(wall, bStt, TRUE);

		// 측면 전단철근 
		CUbGeoCurveArray edgeArr;
		CTwinVectorArray tvArr;
		wall.GetTvRebarShear_Side(tvArr, TRUE);
		TrsVectorArray(edgeArr, tvArr, PLN_XOZ, 0);
		
		// 실제 위치로 이동
		CRebarPlacing rb;
		CTwinVectorArray tvArrPlan;
		GetOutWallPlan(pBri, tvArrPlan, bStt);
		BOOL dOffset	= (bStt) ? pBri->m_dCoverWallOut[0] : pBri->m_dCoverWallIn[0];
		rb.GetOffsetTvArray(tvArrPlan, dOffset, TRUE);
		CTwinVector tvLeft;
		tvLeft	= tvArrPlan.GetTvByInfo(_T("좌측"));
		
		ModifySideToPlanWall(&wall, edgeArr, tvLeft);
		///////////////////////
		
		// 단면도 전단철근 - 전단철근 길이 구하는데 사용
		CTwinVectorArray tvArrFront;
		wall.GetTvRebarShear_Front(tvArrFront);
		//////////////////////////////////
		
		CTwinVector *pTvFront	= NULL;
		CTwinVector *pTv		= NULL;
		CUbGeoCurve edge;
		
		// 전단철근의 Normal 방향 //
		CVector vDir	= tvArrPlan.GetTvByInfo(_T("하면")).GetXyDir(TRUE);
		/////////////////////////////
		
		// 단면도 전단철근과 교좌면 좌표를 이용해서 실제 배치 위치 찾기
		/*	CTwinVectorArray tvArrGenSec;
		pAbut->GetTvArrGenSection(tvArrGenSec, pAbut->m_dDistCenterFrLeft, FALSE);
		CTwinVector tvSec	= tvArrGenSec.GetTvByInfo("교좌면");
		double dXOrg	= tvSec.m_v2.x;
		*/	
		long i = 0;
		for(i = 0; i < edgeArr.GetSize(); i++)
		{
			edge = edgeArr.GetAt(i);
			pTv	= &tvArr.GetAt(i);
			
			
			long nIdx	= atol((char*)(LPCSTR)pTv->m_sInfo);
			if(nIdx >= tvArrFront.GetSize()) continue;
			
			pTvFront	= &tvArrFront.GetAt(nIdx);
			
	//		edge.Move(vDir * fabs(pTvSec->m_v1.x - dXOrg));
	//		edge.Move(vDir * pAbut->m_dSLTot * -1);
			
			CRebarShearDetail *pRebarShearDetail	= &wall.m_rebarShearDetail;
			
			double dB	= pTvFront->GetLength();
			double dC	= dB;
			switch(pRebarShearDetail->m_nType)
			{
			case TYPE_SHEAR_CAP : dC	= 100; break;
			case TYPE_SHEAR_DOUPLE : dC	= dB; break;
			case TYPE_SHEAR_DIGUT : dB	= 100; break;
			case TYPE_SHEAR_DIGUT_BEND : dC	= 100; break;
			case TYPE_SHEAR_SPACER : dC	= 100; break;
			case TYPE_SHEAR_CAP2 : dC	= 100; break;
			}
			
			CUbEttRebar rebar;
			rebar.Init();
			m_pMRD->SetPlan(edge);
			m_pMRD->SetRebarAttr(pRebarShearDetail->GetRebarType(pRebarShearDetail->m_nType), edge.GetLength(), dB, dC, 0, 0, 0, vDir);
			m_pMRD->GetRebarDetail(rebar.m_arrUbEdge, rebar.m_arrUbEdge2, m_pMRD->m_nTypeRebar == 3 ? 1 : 0);
			rebar.m_bIsUbEdge2	= rebar.m_arrUbEdge2.GetSize() > 0;
			
			rebar.m_dDia	= pRebarShearDetail->m_dDia;
			
			// 철근등록
			AddRebar(&rebar, REBAR_FOOTING, m_sItemPathNameHeader, GetColorRebar(REBARCOLOR_SHEAR));
		}
	}
}

void CUbiARcBridgeData::GetOutWallPlan(CRcBridgeApp *pBri, CTwinVectorArray &tvArrOutWallPlan, BOOL bStt)
{
	if(pBri==NULL)		return;
	tvArrOutWallPlan.RemoveAll();

	CTwinVectorArray tvArrPlan;
	pBri->GetTvRebarOut_Plane_UpperSlabStd(tvArrPlan, -1, TRUE);
	CString strPlan = (bStt)? _T("시점") : _T("종점");
	long i=0;
	for(i = 0; i < tvArrPlan.GetSize(); i++)
	{
		CTwinVector *pTv	= &tvArrPlan.GetAt(i);
		if(pTv->m_sInfo == strPlan)
		{
			if(pTv->GetLength()!=0)
			{
				pTv->m_sInfo	= (bStt) ? _T("좌측") : _T("우측");
				tvArrOutWallPlan.Add(pTv);
			}
		}
	}
	if(tvArrOutWallPlan.GetSize()<=0)		return;
	
	CTwinVectorArray tvArrSttEnd, *pTvArrPlan;
	pTvArrPlan	= &tvArrOutWallPlan;
	pBri->GetTvPlaneSttEndLine(tvArrSttEnd, bStt, FALSE);
	for(i=0;i<tvArrSttEnd.GetSize();i++)
	{
		CTwinVector *pTv	= &tvArrSttEnd.GetAt(i);
		if(pTv->GetLength()==0)	
		{
			tvArrSttEnd.RemoveAt(i);		i--;
		}
		else
			pTv->m_sInfo	= (bStt) ? _T("우측") : _T("좌측");
	}
	if(tvArrSttEnd.GetSize()<=0)		return;
	
	CTwinVector tvTop, tvBottom;
	tvTop.m_v1	= pTvArrPlan->GetXyTop();
	tvTop.m_v2	= tvArrSttEnd.GetXyTop();
	tvTop.m_sInfo	= _T("상면");
	tvBottom.m_v1	= pTvArrPlan->GetXyBottom();
	tvBottom.m_v2	= tvArrSttEnd.GetXyBottom();
	tvBottom.m_sInfo	= _T("하면");
	
	pTvArrPlan->AddFromTvArray(tvArrSttEnd);
	pTvArrPlan->Add(tvTop);
	pTvArrPlan->Add(tvBottom);
	pTvArrPlan->Sort();
}

void CUbiARcBridgeData::MakeRcSlabRebar(CRcBridgeApp *pBri, double dCurPos)
{
	if(pBri==NULL)		return;
	CRcBridgeRebar *pBriRebar = (CRcBridgeRebar *)pBri;

	CString sItemPathName	= m_sItemPathNameHeader;
	m_sItemPathNameHeader	= sItemPathName + _T("/슬래브 상면 주철근");
	MakeRcSlabRebar_MainRebar(pBriRebar);

	CString msg(_T(""));
	msg.Format(_T("%s 생성중.....\r\n"), pBri->m_strBridgeName);
	if(m_pDlgProgress)
		m_pDlgProgress->SetProgressPos(msg, dCurPos);

	m_sItemPathNameHeader	= sItemPathName + _T("/슬래브 상면 배력철근");
	MakeRcSlabRebar_SupportRebar(pBriRebar);

	m_sItemPathNameHeader	= sItemPathName + _T("/슬래브 상면 전단철근");
	MakeRcSlabRebar_ShearRebar(pBriRebar);
}

void CUbiARcBridgeData::MakeRcSlabRebar_MainRebar(CRcBridgeRebar *pBri)
{
	if(pBri==NULL)		return;
	
	IncreaseIdxGroupRebar();
	long nDan = 0;
	for(nDan = 0; nDan < MAX_REBAR_DAN; nDan++)
	{
		long nCycle = 0;
		for(nCycle = 0; nCycle < MAX_REBAR_CYCLE; nCycle++)
		{
			long nUpper = 0;
			for(nUpper = 0; nUpper < 2; nUpper++)	//iUPPER : 0, iLOWER : 1
			{
				BOOL bUpper	= (nUpper==iUPPER);
				
				// 측면 상의 상부 슬래브 주철근 위치 구하기.
				double dStaLeft = pBri->GetStationOnJijum(0);
				double dStaRight = (pBri->GetStationOnJijum(0, 0)+pBri->GetStationOnJijum(1, 0))/2;
				pBri->InitValueRebarForCrossSection();
				pBri->SettingValueRebarForCrossSection(dStaLeft, dStaRight);
				
				long leftSta	= Compare(pBri->m_dStaLeftCross, dStaLeft, _T("=")) ? iLEFT : iRIGHT;
				CVectorArray xyArr, xyArrHunch;
				BOOL bUseMatchSta	= FALSE;
				if(bUseMatchSta)
				{
					pBri->GetXyRebarMain_CrossSection_UpperSlab(xyArr, xyArrHunch, dStaLeft, bUpper, nDan, nCycle, TRUE, bUseMatchSta);
					CRebarPlacing rb;
					double dDiffEl = pBri->GetDiffELSttAndJijumForVertSectionBySta(dStaLeft);
					rb.MoveRebar(xyArr, CDPoint(0, -dDiffEl));
					rb.MoveRebar(xyArrHunch, CDPoint(0, -dDiffEl));
				}
				else
				{
					xyArr	= pBri->m_xyArrMainRebarCross[nCycle][nUpper][nDan][leftSta];
					//xyArrHunch	= pBri->m_xyArrMainRebarCrossHunch[nCycle][leftSta];
				}
				xyArr.Sort();					//xyArrHunch.Sort();
				if(xyArr.GetSize()==0)		continue;

				// 평면상의 상부 슬래브 주철근의 위치 구하기.
				pBri->SetDataDefaultDistLeftDirPlacing();			// 사보강 배치를 대비해서 사방향 철근이 들어가는 구간의 거리를 미리 설정함.
				
				// 평면상의 상부슬래브 상면 주철근이 좀 이상한 듯해서 하부 주철근 위치로만 판단.
				CTwinVectorArray tvArr, tvArrAngLeft, tvArrAngRight;
				CTwinVectorArray tvArrJoint, tvArrHidden;
				pBri->GetTvRebarMain_Plane_UpperSlab_TypeNormal_Std(tvArr, tvArrAngLeft,tvArrAngRight, FALSE, 
					nCycle, nDan, 0, 0, FALSE, FALSE, FALSE, tvArrJoint, tvArrHidden, tvArrHidden, tvArrHidden, FALSE);
				if(tvArr.GetSize()==0)		continue;
				
				
				// 평면도상의 각 실제위치로 이동하며 복사.
				long nCnt	= (xyArr.GetSize()<tvArr.GetSize()) ? xyArr.GetSize() : tvArr.GetSize();
				long i = 0;
				double dEdgeArrTop=0.0, dEdgeArrLeft=0.0;
				for(i=0;i<nCnt;i++)
				{
					CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC = 
																						&pBri->m_pArrRCMainSlabUpper[nCycle][nUpper][nDan];
					if(pArrRC==NULL)	return;
					
					long rc = 0;
					long nSize = pArrRC->GetSize();	
					CTwinVectorArray tvArrMainRC;
					for(rc = 0; rc < nSize; rc++)
					{
						CRebarInfoCycle *pRC = pArrRC->GetAt(rc);
						if(pRC==NULL)	continue;
						if(!pRC->m_bExist)	continue;
						
						MakeTvArrByRC(tvArrMainRC, pRC);
					}
					CRebarPlacing rb;
					rb.RemoveMinLengthLine(tvArrMainRC, 0);
					tvArrMainRC.Sort();
					rb.RemoveMeanlessTwinVector(tvArrMainRC);
					if(tvArrMainRC.GetSize()==0)		continue;

					double dMoveHeight=0.0, dMoveLeft=0.0;
					
					CUbEttRebar rebar, rebarTmp;
					// 철근 만들기.
					if(!MakeRebar(rebar, tvArrMainRC))			continue;
					TrsPlane(rebar.m_arrUbEdge, PLN_XOZ, 0);
					
					dEdgeArrLeft = tvArrMainRC.GetXyLeft().x;
					dEdgeArrTop = bUpper ? tvArrMainRC.GetXyTop().y : tvArrMainRC.GetXyBottom().y;
					dMoveLeft	= tvArr.GetXyLeft().x - dEdgeArrLeft;
					dMoveHeight = xyArr.GetAt(i).y - dEdgeArrTop;
					rebarTmp = rebar;
					rebarTmp.Move(CVector(dMoveLeft, xyArr.GetAt(i).x, dMoveHeight));
					
					AddRebar(&rebarTmp, REBAR_SLAB, m_sItemPathNameHeader, GetColorRebar(REBARCOLOR_MAIN));
				}
			}
		}
	}
}

/*
{
	if(pBri==NULL)		return;
	
	IncreaseIdxGroupRebar();
	long nDan = 0;
	for(nDan = 0; nDan < MAX_REBAR_DAN; nDan++)
	{
		long nCycle = 0;
		for(nCycle = 0; nCycle < MAX_REBAR_CYCLE; nCycle++)
		{
			long nUpper = 0;
			for(nUpper = 0; nUpper < 2; nUpper++)	//iUPPER : 0, iLOWER : 1
			{
				BOOL bUpper	= (nUpper==iUPPER);
				
				// 측면 상의 상부 슬래브 주철근 위치 구하기.
				double dStaLeft = pBri->GetStationOnJijum(0);
				double dStaRight = (pBri->GetStationOnJijum(0, 0)+pBri->GetStationOnJijum(1, 0))/2;
				pBri->InitValueRebarForCrossSection();
				pBri->SettingValueRebarForCrossSection(dStaLeft, dStaRight);
				
				long leftSta	= Compare(pBri->m_dStaLeftCross, dStaLeft, _T("=")) ? iLEFT : iRIGHT;
				CVectorArray xyArr, xyArrHunch;
				BOOL bUseMatchSta	= FALSE;
				if(bUseMatchSta)
				{
					pBri->GetXyRebarMain_CrossSection_UpperSlab(xyArr, xyArrHunch, dStaLeft, bUpper, nDan, nCycle, TRUE, bUseMatchSta);
					CRebarPlacing rb;
					double dDiffEl = pBri->GetDiffELSttAndJijumForVertSectionBySta(dStaLeft);
					rb.MoveRebar(xyArr, CDPoint(0, -dDiffEl));
					rb.MoveRebar(xyArrHunch, CDPoint(0, -dDiffEl));
				}
				else
				{
					xyArr	= pBri->m_xyArrMainRebarCross[nCycle][nUpper][nDan][leftSta];
					//xyArrHunch	= pBri->m_xyArrMainRebarCrossHunch[nCycle][leftSta];
				}
				xyArr.Sort();					//xyArrHunch.Sort();
				if(xyArr.GetSize()==0)		continue;

				// 평면상의 상부 슬래브 주철근의 위치 구하기.
				pBri->SetDataDefaultDistLeftDirPlacing();			// 사보강 배치를 대비해서 사방향 철근이 들어가는 구간의 거리를 미리 설정함.
				
				CTwinVectorArray tvArr, tvArrAngLeft, tvArrAngRight;
				CTwinVectorArray tvArrJoint, tvArrHidden;
				pBri->GetTvRebarMain_Plane_UpperSlab_TypeNormal_Std(tvArr, tvArrAngLeft,tvArrAngRight, bUpper, 
					nCycle, nDan, 0, 0, FALSE, FALSE, FALSE, tvArrJoint, tvArrHidden, tvArrHidden, tvArrHidden, FALSE);
				if(tvArr.GetSize()==0)		continue;
				
				
				// 평면도상의 각 실제위치로 이동하며 복사.
				long nCnt	= (xyArr.GetSize()<tvArr.GetSize()) ? xyArr.GetSize() : tvArr.GetSize();
				long i = 0;
				double dEdgeArrTop=0.0, dEdgeArrLeft=0.0;
				for(i=0;i<nCnt;i++)
				{
					CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC = 
																							&pBri->m_pArrRCMainSlabUpper[nCycle][nUpper][nDan];
					if(pArrRC==NULL)	return;
					
					long rc = 0;
					long nSize = pArrRC->GetSize();	
					double dMoveHeight=0.0, dMoveLeft=0.0;
					for(rc = 0; rc < nSize; rc++)
					{
						CRebarInfoCycle *pRC = pArrRC->GetAt(rc);
						if(pRC==NULL)	continue;
						if(!pRC->m_bExist)	continue;
						
						CUbEttRebar rebar, rebarTmp;
						// 철근 만들기.
						if(!MakeRebar(rebar, pRC)) continue;
						TrsPlane(rebar.m_arrUbEdge, PLN_XOZ, 0);
						
						long j	= 0;
						long nSizeRebar = rebar.GetPathArray().GetSize();
						for(j=0;j<nSizeRebar;j++)
						{
							CUbGeoCurve *pEdge	= &rebar.GetPathArray().GetAt(j);
							if(dEdgeArrTop < pEdge->m_v1.z)	dEdgeArrTop = pEdge->m_v1.z;
							if(dEdgeArrTop < pEdge->m_v2.z)	dEdgeArrTop = pEdge->m_v2.z;
							if(rc==0)
							{
								if(j==0)	dEdgeArrLeft	= pEdge->m_v1.x;
								else
								{
									dEdgeArrLeft	= (dEdgeArrLeft<pEdge->m_v1.x) ? pEdge->m_v1.x : dEdgeArrLeft;
								}
							}
						}
						
						dMoveHeight = xyArr.GetAt(i).y - dEdgeArrTop;
						if(rc==0)
							dMoveLeft	= tvArr.GetXyLeft().x - dEdgeArrLeft;//tvArr.GetAt(nCnt-1-i).m_v1.x - dEdgeArrLeft;
						rebarTmp = rebar;
						rebarTmp.Move(CVector(dMoveLeft, xyArr.GetAt(i).x, dMoveHeight));
						
						AddRebar(&rebarTmp, REBAR_SLAB, m_sItemPathNameHeader, GetColorRebar(REBARCOLOR_MAIN));
					}
				}
			}
		}
	}
}
*/

void CUbiARcBridgeData::MakeRcSlabRebar_SupportRebar(CRcBridgeRebar *pBri)
{
	if(pBri==NULL)		return;

	// 일단 횡단면도 가각적용 무조건 안함 ///////////
	BOOL bExpLeft	= FALSE;
	BOOL bExpRight	= FALSE;
	//////////////////////////////////////////////////
	CLineInfoApp *pLine = pBri->m_pLineApp;
	if(pLine==NULL)		return;

	IncreaseIdxGroupRebar();
	long nDan = 0;
	for(nDan = 0; nDan < MAX_REBAR_DAN; nDan++)
	{
		long nUpper = 0;
		for(nUpper = 0; nUpper < 2; nUpper++)	//iUPPER : 0, iLOWER : 1
		{
			BOOL bUpper	= (nUpper==iUPPER);
			//평면도 상의  배력철근 좌표 구함.
			CTwinVectorArray  tvArr, tvArrAngStt, tvArrAngEnd, tvArrRAngStt, tvArrRAngEnd;
			pBri->GetTvRebarSupport_Plane_UpperSlab_Std(tvArr, tvArrAngStt, tvArrAngEnd, tvArrRAngStt, tvArrRAngEnd, bUpper, nDan, 0);

			long nTvArrNum = 0;
			for(nTvArrNum = 0; nTvArrNum < 5; nTvArrNum++)
			{
				CTwinVectorArray *pTvArr	= NULL;
				switch(nTvArrNum)
				{
				case 0: pTvArr	= &tvArr; break;
				case 1: pTvArr	= &tvArrAngStt; break;
				case 2: pTvArr	= &tvArrAngEnd; break;
				case 3: pTvArr	= &tvArrRAngStt; break;
				case 4: pTvArr	= &tvArrRAngEnd; break;
				default: break;
				}
				
				long i	= 0;
				double dSta	= 0.0;
				for(i=0;i<pTvArr->GetSize();i++)
				{
					CTwinVector *pTv = &pTvArr->GetAt(i);
					if(pTv==NULL)	return;
					
					if(nTvArrNum==1)
						dSta = pBri->GetStationByTarget(BRIDGE_OFF_STT_SLAB, 0, TRUE);
					else if(nTvArrNum==2)
						dSta = pBri->GetStationByTarget(BRIDGE_OFF_END_SLAB, 0, TRUE);
					else
						pLine->GetStationMatchByXyAndAngle(pTv->GetXyBottom(), pTv->GetXyDir(), dSta);
					// 횡단면 상의 배력철근 좌표 구함.
					CTwinVectorArray tvArrUpper, tvArrSupLeft, tvArrSupRight;
					pBri->GetTvRebarSupport_CrossSection_UpperSlab(tvArrUpper, tvArrSupLeft, tvArrSupRight, dSta, bUpper, nDan, 0, FALSE, TRUE, bExpLeft, 1, TRUE);
					CRebarPlacing rb;
					double dDiffEl = pBri->GetDiffELSttAndJijumForVertSectionBySta(dSta);
					rb.MoveRebar(tvArrUpper, CDPoint(0, -dDiffEl));
					if(tvArrUpper.GetSize()==0)		continue;
					//슬래브 배력철근 상부는 다리 추가.
					if(bUpper==TRUE)
					{
						CTwinVectorArray tvArrLower, tvArrSupLeftLow, tvArrSupRightLow;
						pBri->GetTvRebarSupport_CrossSection_UpperSlab(tvArrLower, tvArrSupLeftLow, tvArrSupRightLow, dSta, !bUpper, nDan, 0, FALSE, TRUE, bExpLeft, 1, TRUE);
						CRebarPlacing rb;
						double dDiffEl = pBri->GetDiffELSttAndJijumForVertSectionBySta(dSta);
						rb.MoveRebar(tvArrLower, CDPoint(0, -dDiffEl));
						if(tvArrLower.GetSize()==0)	continue;
						CTwinVectorArray tvArrLeft, tvArrRight;
						tvArrLower.GetTvArrByInfo(_T("슬래브좌측면"),tvArrLeft, FALSE,FALSE);
						tvArrLower.GetTvArrByInfo(_T("슬래브우측면"),tvArrRight, FALSE,FALSE);
						tvArrUpper.AddFromTvArray(tvArrLeft);
						tvArrUpper.AddFromTvArray(tvArrRight);
					}
					
					CUbEttRebar rebar;
					// 철근 만들기.
					if(!MakeRebar(rebar,tvArrUpper))	continue;
					TrsPlane(rebar.m_arrUbEdge, PLN_YOZ, 0);
					CDPoint xyPlan	= pTv->GetXyBottom();
					rebar.Move(CVector(xyPlan.x, xyPlan.y - tvArrUpper.GetXyLeft().x, 0));
					
					CDPoint vAngPlan	= (pTv->GetXyTop() - pTv->GetXyBottom()).Unit();
					CDPoint vAngOrg		= CDPoint(0,1);
					CDPoint vAngRot		= ToDPointFrDegree(vAngPlan.GetAngleDegree() - vAngOrg.GetAngleDegree());
					rebar.Rotate(xyPlan, vAngRot);
					
					AddRebar(&rebar, REBAR_SLAB, m_sItemPathNameHeader, GetColorRebar(REBARCOLOR_SUPPORT));
				}
			}
		}
	}
}

void CUbiARcBridgeData::MakeRcSlabRebar_ShearRebar(CRcBridgeRebar *pBri)
{
	if(pBri==NULL)	return;
	CLineInfoApp *pLine = pBri->m_pLineApp;
	if(pLine==NULL)	return;

	IncreaseIdxGroupRebar();
	//평면도 상의 철근 좌표
	CTwinVectorArray tvArr;
	//pBri->GetTvRebarShear_Plane_UpperSlab_Real(tvArr);
	pBri->GetTvRebarShear_Plane_UpperSlab(tvArr, TRUE);
	if(tvArr.GetSize()==0)			return;
	CUbGeoCurveArray edgeArr;
	TrsVectorArray(edgeArr, tvArr, PLN_XOY, 0);

	// 전단철근의 Normal 방향
	CVector vDir(0,0,-1);

	// 단면도 철근 가져와서 그리기 - 다리 길이
	BOOL bVertDir	= pBri->IsVertDir();
	CTwinVectorArray tvArrVert, tvArrHunch, tvArrRib;
	pBri->GetTvRebarShear_VertSection_UpperSlab(tvArrVert, tvArrHunch, tvArrRib, bVertDir?-1:1);

	CTwinVector *pTv		= NULL;
	CUbGeoCurve edge;

	double dBasic = tvArr.GetAt(0).m_v1.y;
	CTwinVector *pTvVert	= NULL;
	CRebarShearDetail *pRebarShearDetail	= &pBri->m_RebarShearDetail_UpperSlab;
	CTwinVectorArray tvArrUpper;
	long i=0 , j=-1;
	for(i = 0; i < edgeArr.GetSize(); i++)
	{
		edge = edgeArr.GetAt(i);
		pTv	= &tvArr.GetAt(i);

		CRebarPlacing rb;
		if(dBasic==pTv->m_v1.y)
		{
			pTvVert	= &tvArrVert.GetAt(++j);
			double dSta	= 0.0;

			pLine->GetStationMatchByXyAndAngle(pTv->GetXyBottom(), pTv->GetXyDir(), dSta);
			// 횡단면 상의 배력철근 좌표 구함.
			CTwinVectorArray tvArrSupLeft, tvArrSupRight;
			pBri->GetTvRebarSupport_CrossSection_UpperSlab(tvArrUpper, tvArrSupLeft, tvArrSupRight, dSta, TRUE, 0, 0, FALSE, TRUE, FALSE, 0, TRUE);
			double dDiffEl = pBri->GetDiffELSttAndJijumForVertSectionBySta(dSta);
			rb.MoveRebar(tvArrUpper, CDPoint(0, -dDiffEl));
		}
		CDPointArray xyArrMatch1, xyArrMatch2;
		rb.GetXyMatchTvArrAndLine(tvArrUpper, CDPoint(pTv->m_v1.y,0), CDPoint(0,1), xyArrMatch1);
		rb.GetXyMatchTvArrAndLine(tvArrUpper, CDPoint(pTv->m_v2.y,0), CDPoint(0,1), xyArrMatch2);
		if(xyArrMatch1.GetSize()==0 || xyArrMatch2.GetSize()==0)	continue;
		edge.m_v1.z	= xyArrMatch1.GetAt(0).y;
		edge.m_v2.z	= xyArrMatch2.GetAt(0).y;

		double dB	= pTvVert->GetLength();
		double dC	= dB;
		switch(pRebarShearDetail->m_nType)
		{
		case TYPE_SHEAR_CAP : dC	= 100; break;
		case TYPE_SHEAR_DOUPLE : dC	= dB; break;
		case TYPE_SHEAR_DIGUT : dB	= 100; break;
		case TYPE_SHEAR_DIGUT_BEND : dC	= 100; break;
		case TYPE_SHEAR_SPACER : dC	= 100; break;
		case TYPE_SHEAR_CAP2 : dC	= 100; break;
		}
		
		CUbEttRebar rebar;
		rebar.Init();
		m_pMRD->SetPlan(edge);
		m_pMRD->SetRebarAttr(pRebarShearDetail->GetRebarType(pRebarShearDetail->m_nType), edge.GetLength(), dB, dC, 0, 0, 0, vDir);
		m_pMRD->GetRebarDetail(rebar.m_arrUbEdge, rebar.m_arrUbEdge2, m_pMRD->m_nTypeRebar == 3 ? 1 : 0);
		rebar.m_bIsUbEdge2	= rebar.m_arrUbEdge2.GetSize() > 0;
		
		rebar.m_dDia	= pRebarShearDetail->m_dDia;
//		rebar.Move(CVector(0,0,pTvVert->m_v1.y));
		
		// 철근등록
		AddRebar(&rebar, REBAR_SLAB, m_sItemPathNameHeader, GetColorRebar(REBARCOLOR_SHEAR));
	}
}

void CUbiARcBridgeData::MakeRcAps(CApsApp *pAps)
{
	TopoDS_Shape shpNormal, shpHunch;

	CTwinVectorArray tvArrPlan;
	pAps->GetTvPlan(TRUE);
	tvArrPlan	= pAps->m_tvArrPlan;

	// 구해진 정면도 좌표를 벡터로 변환
	CUbGeoCurveArray edgeArr;
	TrsVectorArray(edgeArr, tvArrPlan, PLN_XOY, 0);

	// 실제 위치로 이동
	pAps->GetTvSide(FALSE);
	CDRect rectSection	= pAps->m_tvArrSide.GetRect();
	edgeArr.Move(CVector(0, 0, rectSection.bottom+pAps->m_dH2));

	// 변환한 벡터를 돌출
	CVector vDir	= CVector(0, 0, 1);
	vDir	= vDir * (pAps->m_dH1);
	
	shpNormal	= GetTopoDS_Shape(edgeArr, vDir);

	// 헌치단면
	if(pAps->m_dH2 > 0)
	{
		CTwinVectorArray tvArrPlanRect;
		pAps->GetTvPlanRectAngle(tvArrPlanRect);
//		TrsPlanForAbut(tvArrPlanRect);
		CTwinVector tvOrg	= tvArrPlanRect.GetTvByInfo(_T("우"));
		CTwinVector tvLower	= tvArrPlanRect.GetTvByInfo(_T("하"));

		CTwinVectorArray tvArrSec;
		double dL1Old	= pAps->m_dL1;
		pAps->m_dL1	= tvLower.GetLength() - pAps->m_dL2 - pAps->m_dL3;
		pAps->GetTvSide(FALSE);
		tvArrSec	= pAps->m_tvArrSide;
		pAps->m_dL1	= dL1Old;
		pAps->GetTvSide(FALSE);

		// 구해진 단면도 좌표를 벡터로 변환
		edgeArr.RemoveAll();
		TrsVectorArray(edgeArr, tvArrSec, PLN_XOY, 0);

		// 실제 위치로 이동
		ModifySectionToPlanAps(pAps, edgeArr);

		// 변환한 벡터를 돌출
		vDir	= tvOrg.GetXyDir(TRUE);
		vDir.z	= 0;
		vDir	= vDir * tvOrg.GetLength();
		shpHunch	= GetTopoDS_Shape(edgeArr, vDir);

		// common
		shpNormal	= BRepAlgoAPI_Common(shpNormal, shpHunch);
	}

	// 솔리드 객체로 등록
	AddSolid(&shpNormal, SOLID_APS, m_sItemPathNameHeader);
}

void CUbiARcBridgeData::ModifySectionToPlanAps(CApsApp *pAps, CUbGeoCurveArray &edgeArr)
{
	TrsPlane(edgeArr, PLN_XOZ, 0);

	// 접속슬래브 단면 기준상단을 기준으로 함
	CDRect rect	= pAps->m_tvArrSide.GetRect();
	CVector vOrg	= CVector(!pAps->m_bLeft ? rect.left : rect.right, rect.top, 0);
	TrsPlaneOne(vOrg, PLN_XOZ, 0);

	// vOrg에서 실제 직사각접속슬래브 하면 m_v1가지의 거리
	CTwinVectorArray tvArrPlanRect;
	pAps->GetTvPlanRectAngle(tvArrPlanRect);
//	TrsPlanForAbut(tvArrPlanRect);
	CDPoint xyOrg	= vOrg;
	CDPoint xyPlan	= tvArrPlanRect.GetTvByInfo(_T("하")).m_v1;
	CVector vMove	= xyPlan - xyOrg;
	edgeArr.Move(vMove);

	// 회전
	CDPoint vAngPlan	= tvArrPlanRect.GetTvByInfo(_T("하")).GetXyDir();
	CDPoint vAngOrg		= !pAps->m_bLeft ? CDPoint(1, 0) : CDPoint(-1, 0);
	CDPoint vAngRot		= ToDPointFrDegree(vAngPlan.GetAngleDegree() - vAngOrg.GetAngleDegree());
	edgeArr.Rotate(xyPlan, vAngRot);
}

void CUbiARcBridgeData::MakeRcApsRebar(CApsApp *pAps)
{
	if(m_pManage->m_nRebaCreateKind==CUbEttRebar::DRAW_NONE) return;
	MakeRcApsRebar_MainRebar(pAps);
	MakeRcApsRebar_SupportRebar(pAps);
	MakeRcApsRebar_OutRebar(pAps);
	MakeRcApsRebar_MainRebarHunch(pAps);
	MakeRcApsRebar_SupportRebarHunch(pAps);
	MakeRcApsRebar_ShearRebarHunch(pAps);
}

// 접속슬래브 주철근
void CUbiARcBridgeData::MakeRcApsRebar_MainRebar(CApsApp *pAps)
{
	CUbEttRebar rebar;
	CUbGeoCurve edge;
	CString str	= _T("");
	CString sItemPathNameHeader	= m_sItemPathNameHeader;
	for(long upper = 0; upper < 2; upper++)
	{
		BOOL bUpper	= upper == iUPPER;
		if(bUpper) continue;

		CTwinVectorArray tvArrRebar;
		pAps->GetTvRebarPlanMain(tvArrRebar, AREA_APS_ALL, bUpper);
		if(tvArrRebar.GetSize() == 0) continue;

		
		IncreaseIdxGroupRebar();
		str.Format(_T("/%s면 주철근"), bUpper ? _T("상") : _T("하"));
		m_sItemPathNameHeader	= sItemPathNameHeader;
		m_sItemPathNameHeader += str;

//		TrsPlanForAbut(tvArrRebar);

		// 상면주철근은 'ㅡ'자, 하면주철근은 'ㄷ'자
		double dY	= bUpper ? pAps->m_xyOrg.y + (pAps->m_dH1 + pAps->m_dH2) - pAps->m_dCoverUpper : pAps->m_xyOrg.y + pAps->m_dH2 + pAps->m_dCoverLower;
		double dLeg	= bUpper ? 0 : pAps->m_dH1 - pAps->m_dCoverUpper - pAps->m_dCoverLower;
		CTwinVector tv;
		for(long i = 0; i < tvArrRebar.GetSize(); i++)
		{
			tv	= tvArrRebar.GetAt(i);

			rebar.Init();

			if(bUpper)
			{
				edge.SetEdge(tv.m_v1, tv.m_v2);
				rebar.AddEdge(edge, UNIT_M);
			}
			else
			{
				edge.SetEdge(tv.m_v1 + CVector(0, 0, dLeg), tv.m_v1);
				rebar.AddEdge(edge, UNIT_M);

				edge.SetEdge(tv.m_v1, tv.m_v2);
				rebar.AddEdge(edge, UNIT_M);

				edge.SetEdge(tv.m_v2, tv.m_v2 + CVector(0, 0, dLeg));
				rebar.AddEdge(edge, UNIT_M);
			}
			

			// 실제 위치로 이동
			rebar.Move(CVector(0, 0, dY));

			rebar.m_dDia	= pAps->GetDia(bUpper ? RB_APS_U_M1 : RB_APS_L_M1);

			// 철근등록
			AddRebar(&rebar, REBAR_APS, m_sItemPathNameHeader, GetColorRebar(REBARCOLOR_MAIN));
		}
	}
}

void CUbiARcBridgeData::MakeRcApsRebar_SupportRebar(CApsApp *pAps)
{
	CUbEttRebar rebar;
	CUbGeoCurve edge;
	CString str	= _T("");
	CString sItemPathNameHeader	= m_sItemPathNameHeader;
	for(long upper = 0; upper < 2; upper++)
	{
		BOOL bUpper	= upper == iUPPER;

		CTwinVectorArray tvArrRebar;
		pAps->GetTvRebarPlanSup(tvArrRebar, AREA_APS_ALL, bUpper);
		if(tvArrRebar.GetSize() == 0) continue;

		IncreaseIdxGroupRebar();
		str.Format(_T("/%s면 배력철근"), bUpper ? _T("상") : _T("하"));
		m_sItemPathNameHeader	= sItemPathNameHeader;
		m_sItemPathNameHeader += str;

//		TrsPlanForAbut(tvArrRebar);

		// 상면주철근은 'ㅡ'자, 하면주철근은 'ㄷ'자
		double dY	= bUpper ? pAps->m_xyOrg.y + (pAps->m_dH1 + pAps->m_dH2) - pAps->m_dCoverUpper : pAps->m_xyOrg.y + pAps->m_dH2 + pAps->m_dCoverLower;
		double dLeg	= bUpper ? 0 : pAps->m_dH1 - pAps->m_dCoverUpper - pAps->m_dCoverLower;
		CTwinVector tv;
		for(long i = 0; i < tvArrRebar.GetSize(); i++)
		{
			tv	= tvArrRebar.GetAt(i);

			rebar.Init();

			if(bUpper)
			{
				edge.SetEdge(tv.m_v1, tv.m_v2);
				rebar.AddEdge(edge, UNIT_M);
			}
			else
			{
				edge.SetEdge(tv.m_v1 + CVector(0, 0, dLeg), tv.m_v1);
				rebar.AddEdge(edge, UNIT_M);

				edge.SetEdge(tv.m_v1, tv.m_v2);
				rebar.AddEdge(edge, UNIT_M);

				edge.SetEdge(tv.m_v2, tv.m_v2 + CVector(0, 0, dLeg));
				rebar.AddEdge(edge, UNIT_M);
			}
			

			// 실제 위치로 이동
			rebar.Move(CVector(0, 0, dY));

			rebar.m_dDia	= pAps->GetDia(bUpper ? RB_APS_U_B1 : RB_APS_L_B1);

			// 철근등록
			AddRebar(&rebar, REBAR_APS, m_sItemPathNameHeader, GetColorRebar(REBARCOLOR_SUPPORT));
		}
	}
}

// 접속슬래브 외곽철근
void CUbiARcBridgeData::MakeRcApsRebar_OutRebar(CApsApp *pAps)
{
	CString sItemPathNameHeader	= m_sItemPathNameHeader;
	CUbEttRebar rebar;
	CTwinVectorArray tvArrRebar;
	pAps->GetTvRebarPlanOut(tvArrRebar);
	if(tvArrRebar.GetSize() == 0) return;

	// 
//	TrsPlanForAbut(tvArrRebar);

	ConvertTwinVectorArrayToUbEdgeArray(tvArrRebar, rebar.m_arrUbEdge);

	rebar.m_dDia	= pAps->GetDia(RB_APS_SIDE1);

	// 철근등록
	m_sItemPathNameHeader	= sItemPathNameHeader + _T("/상면 외곽철근");
	rebar.Move(CVector(0, 0, pAps->m_xyOrg.y + pAps->m_dH1 + pAps->m_dH2 - pAps->m_dCoverUpper));
	AddRebar(&rebar, REBAR_APS, m_sItemPathNameHeader, GetColorRebar(REBARCOLOR_MAIN));

	m_sItemPathNameHeader	= sItemPathNameHeader + _T("/하면 외곽철근");
	rebar.Move(CVector(0, 0, -(pAps->m_dH1 - pAps->m_dCoverUpper)+5));
	AddRebar(&rebar, REBAR_APS, m_sItemPathNameHeader, GetColorRebar(REBARCOLOR_MAIN));
}

// 헌치 주철근
void CUbiARcBridgeData::MakeRcApsRebar_MainRebarHunch(CApsApp *pAps)
{
	IncreaseIdxGroupRebar();
	m_sItemPathNameHeader += _T("/헌치부 주철근");

	if(!pAps->IsRebarHunch()) return;

	CUbGeoCurveArray edgeArr;
	CUbEttRebar rebar;

	CTwinVectorArray tvArrRebarSide;
	pAps->GetTvRebarSideHunch(tvArrRebarSide);
	if(tvArrRebarSide.GetSize() == 0) return;

	ConvertTwinVectorArrayToUbEdgeArray(tvArrRebarSide, edgeArr);
	TrsPlane(edgeArr, PLN_XOZ);	
	CDPoint xyOrg	= edgeArr.GetAt(0).m_v1;


	CTwinVectorArray tvArrRebarPlan, tvArrTmp;
	pAps->GetTvRebarPlanMainHunch(tvArrRebarPlan, tvArrTmp, TRUE);
//	TrsPlanForAbut(tvArrRebarPlan);
	

	CTwinVector *pTv	= NULL;
	for(long i = 0; i < tvArrRebarPlan.GetSize(); i++)
	{
		pTv	= &tvArrRebarPlan.GetAt(i);
		
		CVector vMove	= (pTv->m_v1 - xyOrg);
		CDPoint vAngRot	= CDPoint(1, 0);
		if(pAps->m_bOrgLeft)
		{
			vAngRot	= pTv->GetXyDir();
		}
		else
		{
			vAngRot	= ToDPointFrDegree(pTv->GetXyDir().GetAngleDegree() - 180);
		}
		//vMove.z -= pAps->m_dH2;

		rebar.Init();

		rebar.m_arrUbEdge	= edgeArr;
		rebar.GetPathArray().Rotate(xyOrg, vAngRot);
		rebar.GetPathArray().Move(vMove);
		
		rebar.m_dDia	= pAps->GetDia(RB_APS_H_M);

		// 철근등록
		AddRebar(&rebar, REBAR_APS, m_sItemPathNameHeader, GetColorRebar(REBARCOLOR_MAIN));
	}
}

void CUbiARcBridgeData::MakeRcApsRebar_SupportRebarHunch(CApsApp *pAps)
{
	if(!pAps->IsRebarHunch()) return;
	IncreaseIdxGroupRebar();
	m_sItemPathNameHeader += _T("/헌치부 배력철근");

	CDPointArray xyArrRebarHunchSide;
	pAps->GetXyRebarSideHunchSup(xyArrRebarHunchSide, TRUE, TRUE);

	CTwinVectorArray tvArrRebarHunchPlan;
	pAps->GetTvRebarPlanSupHunch(tvArrRebarHunchPlan);
//	TrsPlanForAbut(tvArrRebarHunchPlan);

	CUbEttRebar rebar;
	CUbGeoCurve edge;
	CTwinVector *pTv	= NULL;
	CDPoint xy(0, 0);
	for(long i = 0; i < tvArrRebarHunchPlan.GetSize(); i++)
	{
		if(xyArrRebarHunchSide.GetSize() <= i) continue;

		pTv	= &tvArrRebarHunchPlan.GetAt(i);

		rebar.Init();

		edge.SetEdge(pTv->m_v1, pTv->m_v2);
		
		// 실제 위치로 이동
		xy	= xyArrRebarHunchSide.GetAt(i);
		edge.m_v1.z	= xy.y;
		edge.m_v2.z	= xy.y;

		rebar.AddEdge(edge, UNIT_M);

		rebar.m_dDia	= pAps->GetDia(RB_APS_H_B);

		// 철근등록
		AddRebar(&rebar, REBAR_APS, m_sItemPathNameHeader, GetColorRebar(REBARCOLOR_SUPPORT));
	}
}

// 접속슬래브 전단철근
void CUbiARcBridgeData::MakeRcApsRebar_ShearRebarHunch(CApsApp *pAps)
{
	IncreaseIdxGroupRebar();
	m_sItemPathNameHeader += _T("/헌치부 전단철근");

	CUbEttRebar rebar;
	CUbGeoCurve edge;
	
	CTwinVectorArray tvArrRebarShearPlan;
	pAps->GetTvRebarPlanShear(tvArrRebarShearPlan);
//	TrsPlanForAbut(tvArrRebarShearPlan);
	double dB	= pAps->m_dH1 - pAps->m_dCoverUpper - pAps->m_dCoverLower;
	double dC	= dB;
	switch(pAps->m_rebarShearDetail.m_nType)
	{
	case TYPE_SHEAR_CAP : dC	= 100; break;
	case TYPE_SHEAR_DOUPLE : dC	= dB; break;
	case TYPE_SHEAR_DIGUT : dB	= 100; break;
	case TYPE_SHEAR_DIGUT_BEND : dC	= 100; break;
	case TYPE_SHEAR_SPACER : dC	= 100; break;
	case TYPE_SHEAR_CAP2 : dC	= 100; break;
	}

	CTwinVector *pTv	= NULL;
	for(long i = 0; i < tvArrRebarShearPlan.GetSize(); i++)
	{
		pTv	= &tvArrRebarShearPlan.GetAt(i);
		edge.m_v1	= pTv->m_v1;
		edge.m_v2	= pTv->m_v2;
		edge.m_v1.z = edge.m_v2.z = pAps->m_xyOrg.y + (pAps->m_dH1 + pAps->m_dH2) - pAps->m_dCoverUpper;

		rebar.Init();
		m_pMRD->SetPlan(edge);
		m_pMRD->SetRebarAttr(pAps->m_rebarShearDetail.GetRebarType(pAps->m_rebarShearDetail.m_nType), pTv->GetLength(), dB, dC, 0, 0, 0, CVector(0, 0, -1));
		m_pMRD->GetRebarDetail(rebar.m_arrUbEdge, rebar.m_arrUbEdge2, m_pMRD->m_nTypeRebar == 3 ? 1 : 0);
		rebar.m_bIsUbEdge2	= rebar.m_arrUbEdge2.GetSize() > 0;

		rebar.m_dDia	= pAps->GetDia(RB_APS_SHEAR);

		// 철근등록
		AddRebar(&rebar, REBAR_APS, m_sItemPathNameHeader, GetColorRebar(REBARCOLOR_SHEAR));
	}
}

// 현재 철근을 추가해도 되는지 체크(비율에 따라서 체크)
BOOL CUbiARcBridgeData::IsAddEnableRebar()
{
	long nCountOffset	= 1 / m_dRateCreateRebar;
	BOOL bAdd	= FALSE;
	if(m_nIdxRebarForAddEnableRebar % nCountOffset == 0)
		bAdd	= TRUE;
	m_nIdxRebarForAddEnableRebar++;

	return bAdd;
}

void CUbiARcBridgeData::MakeRcSlabConstructRebar(CRcBridgeApp *pBri)
{
	if(pBri==NULL)	return;

	CString sItemPathName	= m_sItemPathNameHeader;
/*	m_sItemPathNameHeader	= sItemPathName + _T("/ 주철근");
	// 방호벽 시점
	long nHDan=0;
	MakeRcGaurdFenceRebar_MainRebar(pBri, nHDan);

	// 증분대
	nHDan=pBri->GetQtyHDan()/2;
	MakeRcGaurdFenceRebar_MainRebar(pBri, nHDan);

	// 방호벽 종점
	nHDan=pBri->GetQtyHDan()-1;
	MakeRcGaurdFenceRebar_MainRebar(pBri, nHDan);

	m_sItemPathNameHeader	= sItemPathName + _T("/ 배력철근");
	// 방호벽 시점
	nHDan=0;
	MakeRcGaurdFenceRebar_SupportRebar(pBri, nHDan);

	// 증분대
	nHDan=pBri->GetQtyHDan()/2;
	MakeRcGaurdFenceRebar_SupportRebar(pBri, nHDan);

	// 방호벽 종점
	nHDan=pBri->GetQtyHDan()-1;
	MakeRcGaurdFenceRebar_SupportRebar(pBri, nHDan);
*/
	long nHDan = 0;
	long nQtyHDan = pBri->GetQtyHDan();
	m_sItemPathNameHeader	= sItemPathName + _T("/ 주철근");
	for(nHDan=0; nHDan<nQtyHDan; nHDan++)
		MakeRcGaurdFenceRebar_MainRebar(pBri, nHDan);
	m_sItemPathNameHeader	= sItemPathName + _T("/ 배력철근");
	for(nHDan=0; nHDan<nQtyHDan; nHDan++)
		MakeRcGaurdFenceRebar_SupportRebar(pBri, nHDan);
}

void CUbiARcBridgeData::MakeRcGaurdFenceRebar_MainRebar(CRcBridgeApp *pBri, long nHDan)
{
	if(pBri==NULL)	return;

	IncreaseIdxGroupRebar();
	double sta = pBri->GetStationBridgeMid();
	CDPoint vAng(0,1);
	CGuardWallRC* pGW = pBri->GetGuardWallByHDan(nHDan);
	if(pGW==NULL)	return;
	CTwinVectorArray tvArrSlabConstruct;
	if(!GetTvArrSlabConstruct(tvArrSlabConstruct, pBri, sta, vAng,nHDan))		return;

//	nHDan = (nHDan>4 && pBri->GetQtyHDan()>8) ? nHDan+1: nHDan;
	long  nQtyHDan = pBri->GetQtyHDan();
	CTwinVectorArray tvArrSlab;
	pBri->GetTvCrossSection(tvArrSlab, sta, vAng, FALSE, FALSE, FALSE, 0, FALSE, FALSE);
	nHDan = (tvArrSlab.GetSize()!=nQtyHDan && nHDan>nQtyHDan/2) ? nHDan+1: nHDan;

	CTwinVectorArray tvArr, tvArrRebar;
	CDPointArray xyPointRebar;
	
	pBri->GetTvGuardWall(pGW, tvArr, sta, vAng);
	if(tvArr.GetSize()==0)		return;
	((CRcBridgeRebar*)pBri)->m_Util.GetTvRebarGuardWall(pGW, tvArr, tvArrRebar, xyPointRebar);	
	if(tvArrRebar.GetSize()==0)	return;
	tvArrRebar.Sort();
	CUbEttRebar rebar;
//	if(!MakeRebar(rebar, tvArrRebar))	return;
	ConvertTwinVectorArrayToUbEdgeArray(tvArrRebar, rebar.m_arrUbEdge);
	TrsPlane(rebar.m_arrUbEdge, PLN_XOZ, 0);

	// 실제 위치로 이동.
	CLineInfo *pLineCR = pBri->GetLineBase();
	if(pLineCR==NULL)	return;
	double dStaStt	= pBri->GetStationByTarget(BRIDGE_OFF_STT_SLAB, 0, TRUE);
	double dStaEnd	= pBri->GetStationByTarget(BRIDGE_OFF_END_SLAB, 0, TRUE);
	CDPoint vAngStt	= pBri->GetAngleByTarget(BRIDGE_OFF_STT_SLAB, 0);
	CDPoint vAngEnd	= pBri->GetAngleByTarget(BRIDGE_OFF_END_SLAB, 0);

	CDPointArray xyArrDistStt, xyArrDistEnd;
	if(GetXySlabAct(pBri, dStaStt, vAngStt, xyArrDistStt)==FALSE)	return;
	if(GetXySlabAct(pBri, dStaEnd, vAngEnd, xyArrDistEnd)==FALSE)	return;
	if(xyArrDistStt.GetSize()==0 || xyArrDistEnd.GetSize()==0)		return;
	CDoubleArray dArrStaStt, dArrStaEnd, dArrDistStt, dArrDistEnd;
	if(GetStationAndDistMatchVertical(pBri, TRUE, xyArrDistStt, dArrStaStt, dArrDistStt)==FALSE)	return;
	if(GetStationAndDistMatchVertical(pBri, FALSE, xyArrDistEnd, dArrStaEnd, dArrDistEnd)==FALSE)	return;
	if(dArrStaStt.GetSize()==0 || dArrStaEnd.GetSize()==0)											return;
	if(dArrDistStt.GetSize()==0 || dArrDistEnd.GetSize()==0)										return;

	CTwinVectorArray tvArrOrgLine, tvArrLine;

	long nDan = 0;
	if(nHDan==0)	nDan = nQtyHDan-1;
	tvArrOrgLine.AddFromLineInfo((CLineInfoApp*)pLineCR, dArrStaStt.GetAt(nDan), dArrStaEnd.GetAt(nDan), dArrDistStt[nDan]);
	tvArrOrgLine.Sort();
	
	tvArrLine.AddFromLineInfo((CLineInfoApp*)pLineCR, dArrStaStt.GetAt(nHDan), dArrStaEnd.GetAt(nHDan), dArrDistStt[nHDan]);
	tvArrLine.Sort();

	// 실제 위치로 이동
	long nDir = -1;
	CDPoint uXy(0,0);
	
	uXy = tvArrLine.GetAt(0).m_v1;
	CDRect rect	= tvArr.GetRect();
	CVector vSection(rect.left, 0, 0);
	CVector vPlane(CVector(uXy.x, uXy.y,0));
	CVector vMove = vPlane - vSection;
	rebar.Move(vMove);

	CDPoint vAngPlan(0,0);
	long nEdge=0;
	if(nHDan==0)
		vAngPlan	= tvArrLine.GetAt(nEdge).m_v1 - tvArrOrgLine.GetAt(nEdge).m_v1;
	else
		vAngPlan	= tvArrOrgLine.GetAt(nEdge).m_v1 - tvArrLine.GetAt(nEdge).m_v1;
	CDPoint vAngOrg		= CDPoint(nDir,0);
	CDPoint vAngRot		= ToDPointFrDegree(vAngPlan.GetAngleDegree() - vAngOrg.GetAngleDegree());
	rebar.Rotate(uXy, vAngRot);
	//

	double dPathLength	= tvArrLine.GetLength();
	long nNumCTC	= (dPathLength/pGW->m_dCTC);
	double dSpace	= (dPathLength - (pGW->m_dCTC*nNumCTC))/2;

	CVectorArray vArr;
	CDPoint xyBase	= tvArrLine.GetXyLeft();
	vArr.Add(CVector(0,0,0));
	CDPoint xyDir	= tvArrLine.GetAt(0).GetXyDir();
	CDPoint xyNext	= xyDir*dSpace;
//	vArr.Add(CVector(xyNext.x, xyNext.y,0));

	long nMainDim = 0;
	for(nMainDim=1;nMainDim<nNumCTC;nMainDim++)
		vArr.Add((CVector)xyDir * (nMainDim*pGW->m_dCTC + dSpace));

	OffsetRebar(rebar, vArr, REBAR_GUARDWALL, GetColorRebar(REBARCOLOR_MAIN));
}

void CUbiARcBridgeData::MakeRcGaurdFenceRebar_SupportRebar(CRcBridgeApp *pBri, long nHDan)
{
	if(pBri==NULL)	return;
	IncreaseIdxGroupRebar();

	// 배력철근 좌표 구하기.
	CDPoint vAng(0,1);
	double sta = pBri->GetStationBridgeMid();
	CGuardWallRC* pGW = pBri->GetGuardWallByHDan(nHDan);
	if(pGW==NULL)	return;
	long nDan=0, nQtyHDan = pBri->GetQtyHDan();
	CTwinVectorArray tvArrSlab;
	pBri->GetTvCrossSection(tvArrSlab, sta, vAng, FALSE, FALSE, FALSE, 0, FALSE, FALSE);
	nHDan = (tvArrSlab.GetSize()!=nQtyHDan && nHDan>nQtyHDan/2) ? nHDan+1: nHDan;

	CTwinVectorArray tvArr, tvArrRebar;
	CDPointArray xyPointRebar;
	
	pBri->GetTvGuardWall(pGW, tvArr, sta, vAng);
	if(tvArr.GetSize()==0)		return;
	((CRcBridgeRebar*)pBri)->m_Util.GetTvRebarGuardWall(pGW, tvArr, tvArrRebar, xyPointRebar);	
	if(xyPointRebar.GetSize()==0)	return;

	// 배력철근 만들기.
	CLineInfo *pLineCR = pBri->GetLineBase();
	if(pLineCR==NULL)	return;
	double dStaStt	= pBri->GetStationByTarget(BRIDGE_OFF_STT_SLAB, 0, TRUE);
	double dStaEnd	= pBri->GetStationByTarget(BRIDGE_OFF_END_SLAB, 0, TRUE);
	CDPoint vAngStt	= pBri->GetAngleByTarget(BRIDGE_OFF_STT_SLAB, 0);
	CDPoint vAngEnd	= pBri->GetAngleByTarget(BRIDGE_OFF_END_SLAB, 0);

	CDPointArray xyArrDistStt, xyArrDistEnd;
	if(GetXySlabAct(pBri, dStaStt, vAngStt, xyArrDistStt)==FALSE)	return;
	if(GetXySlabAct(pBri, dStaEnd, vAngEnd, xyArrDistEnd)==FALSE)	return;
	if(xyArrDistStt.GetSize()==0 || xyArrDistEnd.GetSize()==0)		return;

	CDoubleArray dArrStaStt, dArrStaEnd, dArrDistStt, dArrDistEnd;
	if(GetStationAndDistMatchVertical(pBri, TRUE, xyArrDistStt, dArrStaStt, dArrDistStt)==FALSE)	return;
	if(GetStationAndDistMatchVertical(pBri, FALSE, xyArrDistEnd, dArrStaEnd, dArrDistEnd)==FALSE)	return;
	if(dArrStaStt.GetSize()==0 || dArrStaEnd.GetSize()==0)											return;
	if(dArrDistStt.GetSize()==0 || dArrDistEnd.GetSize()==0)										return;

	CTwinVectorArray tvArrLine;
	tvArrLine.AddFromLineInfo((CLineInfoApp*)pLineCR, dArrStaStt.GetAt(nHDan), dArrStaEnd.GetAt(nHDan), dArrDistStt[nHDan]);
	tvArrLine.Sort();

	CUbEttRebar rebar;
	if(!MakeRebar(rebar, tvArrLine))	return;

	// 상부슬래브 시점 방향 구하기.
	CTwinVectorArray tvArrSide;
	GetTvArrSlabPlanSide(tvArrSide, pBri, TRUE);
	CDPoint xyLineMid = (tvArrSide.GetXyTop()+tvArrSide.GetXyBottom())/2;
	CDPoint xyDir(0,-1);
	if(tvArrSide.GetSize()>0)
		xyDir	= (tvArrSide.GetAt(0).GetXyBottom() - tvArrSide.GetAt(0).GetXyTop()).Unit();

	// Offset하면서 생성.
	CVector		 vPos, vStt;
	CVectorArray vArrPos;
	vStt = CVector(xyLineMid);
	vPos = CVector(tvArrLine.GetXyLeft());
	vArrPos.Add(vPos);
	xyPointRebar.Sort();
	long nMovePnt	= 0;
	for(nMovePnt=0; nMovePnt<xyPointRebar.GetSize(); nMovePnt++)
	{
		CDPoint xyTemp	= xyPointRebar.GetAt(nMovePnt);
		vPos	= CVector(xyDir * xyTemp.x/vAngStt.y);
		vArrPos.Add(vStt + vPos + CVector(0, 0, xyTemp.y));
	}
	OffsetRebar(rebar, vArrPos, REBAR_GUARDWALL, GetColorRebar(REBARCOLOR_SUPPORT));
}

void CUbiARcBridgeData::MakeRcHunchRebar(CRcBridgeApp *pBri)
{
	if(pBri==NULL)	return;

	CString sItemPathName	= m_sItemPathNameHeader;
	m_sItemPathNameHeader	= sItemPathName + _T("/ 주철근");
	MakeRcHunchRebar_MainRebar((CRcBridgeRebar*)pBri);
	m_sItemPathNameHeader	= sItemPathName + _T("/ 배력철근");
	MakeRcHunchRebar_SupportRebar((CRcBridgeRebar*)pBri);
}

void CUbiARcBridgeData::MakeRcHunchRebar_MainRebar(CRcBridgeRebar *pBri)
{
	if(pBri==NULL)	return;
	IncreaseIdxGroupRebar();

	long nCycle = 0;
	for(nCycle = 0; nCycle < MAX_REBAR_CYCLE; nCycle++)
	{
		CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC = 
																						&pBri->m_pArrRCMainSlabUpperHunch[nCycle];
		if(!pArrRC) return;

		long nSize = pArrRC->GetSize();	
		for(long rc = 0; rc < nSize; rc++)
		{
			CRebarInfoCycle *pRC = pArrRC->GetAt(rc);
			if(!pRC || !pRC->m_bExist) continue;

			// 철근 만들기
			MakeOffsetRebar(pBri, pRC, rc);
		}
	}
}

void CUbiARcBridgeData::MakeRcHunchRebar_SupportRebar(CRcBridgeRebar *pBri)
{
	if(pBri==NULL)	return;
	IncreaseIdxGroupRebar();
	
	// 배력철근 구하기.
	long nCountHunchRebar	= pBri->m_pArrRCMainSlabUpperHunch[0].GetSize();
	long nHunch	= 0;
	for(nHunch=0; nHunch<nCountHunchRebar; nHunch++)
	{
		double  dSta = pBri->GetStationOnJijum(nHunch);
		CDPoint  vAng	= pBri->GetAngleJijum(nHunch);
		CTwinVectorArray tvArrUpperSlab;
		if(!GetTvArrHunchGuideLine(tvArrUpperSlab, pBri, dSta, vAng))	return;
		CDRect rcUpperSlab	= tvArrUpperSlab.GetRect();

		CUbEttRebar rebar;
		if(!MakeRebar(rebar, tvArrUpperSlab))	continue;
		TrsPlane(rebar.m_arrUbEdge, PLN_XOZ, 0);

		if(rebar.GetPathArray().GetSize()<2)		continue;
		ModifyFrontToPlanHunch((CRcBridgeApp*)pBri, rebar.m_arrUbEdge, nHunch,rcUpperSlab);

		rebar.Move(CVector(0, 0, -pBri->m_dTS));
		
		// Offset하면서 복사 이동하기.
		CDPointArray xyArrLeft, xyArrRight;
		pBri->GetXyRebarSupport_VertSection_UpperSlabHunch(xyArrLeft, xyArrRight, nHunch);
		
		CVectorArray vArr;
		vArr.Add(CVector(rebar.GetPathArray().GetAt(1).m_v1.x,0,rebar.GetPathArray().GetAt(0).m_v2.z));
		// 단면상의 높이는 rebar의 절곡부를 기준으로 표시된다.(rebar.GetPathArray().GetAt(0).m_v1.z이 아니다.)
		long i	= 0;
		for(i=0; i<xyArrLeft.GetSize(); i++)
			vArr.Add(CVector(xyArrLeft.GetAt(i).x, 0, xyArrLeft.GetAt(i).y));
		for(i=0; i<xyArrRight.GetSize(); i++)
			vArr.Add(CVector(xyArrRight.GetAt(i).x, 0, xyArrRight.GetAt(i).y));
		
		OffsetRebar(rebar, vArr, REBAR_RCWALL, GetColorRebar(REBARCOLOR_SUPPORT));
	}
}


/*

		CTwinVectorArray tvArrPlane;
		pBri->GetTvRebarOut_Plane_UpperSlabStd(tvArrPlane, -1, TRUE);
		CTwinVectorArray tvArrSide;
		CString strPlane = _T("시점");
		long j=0;
		for(j = 0; j < tvArrPlane.GetSize(); j++)
		{
			CTwinVector *pTv	= &tvArrPlane.GetAt(j);
			if(pTv->m_sInfo == strPlane)
			{
				if(pTv->GetLength()!=0)		tvArrSide.Add(pTv);
			}
		}
		if(tvArrSide.GetSize()==0)	continue;
		CDPoint xyPlan	= tvArrSide.GetAt(0).GetXyMid();

		double dSta;
		CLineInfoApp *pLine	= pBri->m_pLineApp;
		pLine->GetStationMatchVerticalByXy(xyPlan, dSta);
			int nCountInWall = pBri->GetCountInWall();
			for(int i=0; i<nCountInWall; i++)
			{
				CWallApp *pWall = pBri->GetInWall(i);
				if(pWall==NULL)	continue;
				pWall->m_tvArrPlan;

			}
			CTwinVectorArray tvArrLine;
			pBri->GetTvCrossSection(tvArrLine, dSta, vAng, FALSE, FALSE, FALSE, 0, FALSE, FALSE);
			tvArrLine.Sort();
	}
}
*/

BOOL CUbiARcBridgeData::CheckDist(CDoubleArray dArrDist)
{
	long nCheckNum = 0;
	long i=0;
	for(i=0;i<dArrDist.GetSize();i++)
	{
		double dTemp = dArrDist.GetAt(i);
		if(Compare(dTemp, 0.0,_T("=")))
			nCheckNum++;
	}

	if(nCheckNum>=2)		return TRUE;

	return FALSE;	
}


void CUbiARcBridgeData::MakeMainRebarBytvArrPlan(CTwinVectorArray &tvArrPlan, CRebarInfoCycle *pRC, long nType, CVector vColor)
{
	if(!pRC->m_bExist)	return;
	long nRebarSize = tvArrPlan.GetSize();
	if(nRebarSize==0)	return;
	
	long nR = 0;
	for(nR=0; nR<nRebarSize; nR++)
	{
		CTwinVector *pTv = &tvArrPlan.GetAt(nR);
		if(pTv==NULL)		continue;
		
		CUbEttRebar rebar;
		
		CUbGeoCurve edge, edgeTemp;
		ConvertTwinVectorToUbEdge(pTv, &edge);
		edgeTemp.m_v1 = edge.m_v1;
		edgeTemp.m_v2 = edge.m_v1;
		edgeTemp.m_v1.z = pRC->m_xyStt.y;
		edgeTemp.m_v2.z = pRC->m_xyMid1.y;
		rebar.AddEdge(edgeTemp, UNIT_M);

		edgeTemp.m_v1 = edge.m_v1;
		edgeTemp.m_v2 = edge.m_v2;
		edgeTemp.m_v1.z = pRC->m_xyMid1.y;
		edgeTemp.m_v2.z = pRC->m_xyMid2.y;
		rebar.AddEdge(edgeTemp, UNIT_M);

		edgeTemp.m_v1 = edge.m_v2;
		edgeTemp.m_v2 = edge.m_v2;
		edgeTemp.m_v1.z = pRC->m_xyMid2.y;
		edgeTemp.m_v2.z = pRC->m_xyEnd.y;
		rebar.AddEdge(edgeTemp, UNIT_M);

		AddRebar(&rebar, nType, m_sItemPathNameHeader, vColor);
	}
	
}

void CUbiARcBridgeData::MakeTvArrByRC(CTwinVectorArray &tvArrReturn, CRebarInfoCycle *pRC)
{
	if(!pRC || !pRC->m_bExist)		return;
	CTwinVector tvTemp;
	tvTemp.m_v1	= pRC->m_xyStt;
	tvTemp.m_v2 = pRC->m_xyMid1;
	tvArrReturn.Add(tvTemp);
	tvTemp.m_v1	= pRC->m_xyMid1;
	tvTemp.m_v2	= pRC->m_xyMid2;
	tvArrReturn.Add(tvTemp);
	tvTemp.m_v1	= pRC->m_xyMid2;
	tvTemp.m_v2	= pRC->m_xyEnd;
	tvArrReturn.Add(tvTemp);
}

void CUbiARcBridgeData::MakeRcBracketRebar(CRcBridgeApp *pBri)
{
	if(pBri==NULL)	return;

	CString sItemPathName	= m_sItemPathNameHeader;
	m_sItemPathNameHeader	= sItemPathName + _T("/ 주철근");
	MakeRcBracketRebar_MainRebar((CRcBridgeRebar*)pBri);
	m_sItemPathNameHeader	= sItemPathName + _T("/ 배력철근");
	MakeRcBracketRebar_SupportRebar((CRcBridgeRebar*)pBri);
}

void CUbiARcBridgeData::MakeRcBracketRebar_MainRebar(CRcBridgeRebar *pBri)
{
	if(pBri==NULL)	return;
	IncreaseIdxGroupRebar();

	long nCountJijum	= pBri->GetCountJijum();

	long nStt = 0;
	for(nStt = 0; nStt < 2; nStt++)
	{
		BOOL bStt	= (nStt == iSTT);
		long nJijum  = bStt ? 0 : nCountJijum-1;

		long nCycle = 0;
		for(nCycle = 0; nCycle < MAX_REBAR_CYCLE; nCycle++)
		{
			CSafetyTypedPtrArray <CObArray, CRebarInfoCycle*> *pArrRC = (nCycle == 0) ? 
				&pBri->m_pArrRCMainOutWallBracket_Cycle1[nStt] : &pBri->m_pArrRCMainOutWallBracket_Cycle2[nStt];
			if(!pArrRC)								return;
			if(pArrRC->GetSize() == 0)		return;
			
			CRebarInfoCycle *pRC = pArrRC->GetAt(0);
			if(!pRC || !pRC->m_bExist)		return;
			
			// 철근 만들기
			if(!pRC->m_bUseTvArrRebar || pRC->m_tvArrRebar.GetSize()==0)		return;
			CUbEttRebar rebar;
			if(!MakeRebar(rebar,  pRC->m_tvArrRebar))	return;
			TrsPlane(rebar.m_arrUbEdge, PLN_XOZ, 0);
			
			// 진행 가이드선 구하기.
			double  dSta = pBri->GetStationOnJijum(nJijum);
			CDPoint  vAng	= pBri->GetAngleJijum(nJijum);
			CTwinVector tvJijum = pBri->GetTvJijum(nJijum, 0);
			
			long nTvNum	= 0;
			CTwinVector	*pTv	= &tvJijum;
			CUbEttRebar rebarTemp;
			rebarTemp	= rebar; 
			CDPoint xyPlan	= pTv->GetXyTop();
			xyPlan += (bStt) ? CDPoint(-pBri->m_dCoverWallIn[0],0) : CDPoint(pBri->m_dCoverWallIn[0],0);
			CDPoint xyOrg	= (bStt) ? CDPoint(pRC->m_tvArrRebar.GetXyRight().x,0) : CDPoint(pRC->m_tvArrRebar.GetXyLeft().x, 0);
			CVector	vMove	= xyPlan - xyOrg;
			rebarTemp.Move(vMove);
			
			double dPathLength	= pTv->GetLength();
			long nNumCtc	= (dPathLength/pBri->m_dCTC_MainRebar);
			double dSpace	= (dPathLength - (pBri->m_dCTC_MainRebar*nNumCtc))/2;
			
			CVectorArray vArr;
			vArr.Add(CVector(0,0,0));
			CDPoint xyAngDir = (pTv->GetXyBottom() - pTv->GetXyTop()).Unit();
			CDPoint xyNext, xyAngNext;
			long nMainDim = 0;
			for(nMainDim=1;nMainDim<nNumCtc;nMainDim++)
			{
				xyAngNext	= xyAngDir * (nMainDim*pBri->m_dCTC_MainRebar);
				vArr.Add(CVector(xyAngNext.x, xyAngNext.y, 0));
			}
			
			OffsetRebar(rebarTemp, vArr, REBAR_RCWALL, GetColorRebar(REBARCOLOR_MAIN));
		}
	}

}

void CUbiARcBridgeData::MakeRcBracketRebar_SupportRebar(CRcBridgeRebar *pBri)
{
	if(pBri==NULL)	return;
	IncreaseIdxGroupRebar();
	
	long nCountJijum	= pBri->GetCountJijum();
	// 배력철근 구하기.
	long nStt = 0;
	for(nStt = 0; nStt < 2; nStt++)
	{
		BOOL bStt	= (nStt == iSTT);
		long nJijum  = bStt ? 0 : nCountJijum-1;

		double  dSta = pBri->GetStationOnJijum(nJijum);
		CDPoint  vAng	= pBri->GetAngleJijum(nJijum);
		CTwinVector tvJijum = pBri->GetTvJijum(nJijum, 0);
		CTwinVectorArray tvArrRebar;
		tvArrRebar.Add(tvJijum);
		CDRect rcUpperSlab	= tvArrRebar.GetRect();

		CUbEttRebar rebar;
		if(!MakeRebar(rebar, tvArrRebar))	continue;
		if(rebar.GetPathArray().GetSize()==0)		continue;

		// Offset하면서 복사 이동하기.
		CDPointArray xyArr;
		pBri->GetXyRebarSupport_VertSection_Bracket(xyArr, bStt);
		
		CVectorArray vArr;
		vArr.Add(CVector(tvArrRebar.GetXyLeft().x,0,0));
		long i	= 0;
		for(i=0; i<xyArr.GetSize(); i++)
			vArr.Add(CVector(xyArr.GetAt(i).x, 0, xyArr.GetAt(i).y));
		
		OffsetRebar(rebar, vArr, REBAR_RCWALL, GetColorRebar(REBARCOLOR_SUPPORT));
	}
}

void CUbiARcBridgeData::MakeRcDanbuRebar_MainRebar(CRcBridgeRebar *pBri)
{
	if(pBri==NULL)	return;
	IncreaseIdxGroupRebar();

	long nCountJijum	= pBri->GetCountJijum();

	long nStt = 0;
	for(nStt = 0; nStt < 2; nStt++)
	{
		BOOL bStt	= (nStt == iSTT);
		long nJijum  = bStt ? 0 : nCountJijum-1;

		long nCycle = 0, nDan=0;
		for(nCycle = 0; nCycle < MAX_REBAR_CYCLE; nCycle++)
		{
			for(nDan = 0; nDan < MAX_REBAR_DAN; nDan++)
			{
				if(pBri->IsDanbuRebar(bStt, nDan, nCycle))
				{
					CRebarInfoCycle *pRC	= (nCycle == 0) ? 
						&pBri->m_rebarInfoCycleDanbu_Cycle1[nStt][nDan] : &pBri->m_rebarInfoCycleDanbu_Cycle2[nStt][nDan];
					if(!pRC || !pRC->m_bExist)		continue;
					
					// 철근 만들기
					MakeOffsetRebar(pBri, pRC, nJijum);
/*					CUbEttRebar rebar;
					if(!MakeRebar(rebar,  pRC))		continue;
					if(rebar.GetPathArray().GetSize()==0)		continue;
					TrsPlane(rebar.m_arrUbEdge, PLN_XOZ, 0);
					
					// 진행 가이드선 구하기.
					double  dSta = pBri->GetStationOnJijum(nJijum);
					CDPoint  vAng	= pBri->GetAngleJijum(nJijum);
					CTwinVector tvJijum = pBri->GetTvJijum(nJijum, 0);
					CTwinVectorArray tvArrLine;
					if(!GetTvArrHunchGuideLine(tvArrLine, pBri, dSta, vAng))	continue;
					
					long nTvNum	= 0;
					for(nTvNum=0; nTvNum<tvArrLine.GetSize(); nTvNum++)
					{
						CTwinVector	*pTv	= &tvArrLine.GetAt(nTvNum);
						CUbEttRebar rebarTemp;
						rebarTemp	= rebar; 
						CDPoint xyBase	= pTv->GetXyTop();
						CVector vMove(0, xyBase.x, 0);
						rebarTemp.Move(vMove);
						
						double dPathLength	= pTv->GetLength();
						long nNumCtc	= (dPathLength/pBri->m_dCTC_MainRebar);
						double dSpace	= (dPathLength - (pBri->m_dCTC_MainRebar*nNumCtc))/2;
						
						CVectorArray vArr;
						vArr.Add(CVector(0,0,0));
						CDPoint xyDir	= (pTv->GetXyRight()-pTv->GetXyLeft()).Unit();
						CDPoint xyAngDir = (tvJijum.GetXyTop() - tvJijum.GetXyBottom()).Unit();
						CDPoint xyNext, xyAngNext;
						long nMainDim = 0;
						for(nMainDim=1;nMainDim<nNumCtc;nMainDim++)
						{
							if(nTvNum==0)
							{
								xyNext	= -xyDir * (nMainDim*pBri->m_dCTC_MainRebar);
								xyAngNext	= -xyAngDir * (nMainDim*pBri->m_dCTC_MainRebar);
							}
							else if(nTvNum==tvArrLine.GetSize()-1)
							{
								xyNext	= xyDir * (nMainDim*pBri->m_dCTC_MainRebar);
								xyAngNext	= xyAngDir * (nMainDim*pBri->m_dCTC_MainRebar);
							}
							vArr.Add(CVector(xyAngNext.x, xyAngNext.y, xyNext.y));
						}
						
						OffsetRebar(rebarTemp, vArr, REBAR_RCWALL, GetColorRebar(REBARCOLOR_MAIN));
					}
*/				}
			}
		}
	}

}

BOOL CUbiARcBridgeData::GetTvArrSlabPlanSide(CTwinVectorArray &tvArrReturn, CRcBridgeApp *pBri, BOOL bStt)
{
	if(pBri==NULL)			return FALSE;
	tvArrReturn.RemoveAll();

	CTwinVectorArray tvArrPlane;
	pBri->GetTvRebarOut_Plane_UpperSlabStd(tvArrPlane, -1, TRUE);
	CRebarPlacing rb;
	rb.RemoveMinLengthLine(tvArrPlane, 0);
	long j=0;
	CString strPlane = (bStt)? _T("시점") : _T("종점");
	for(j = 0; j < tvArrPlane.GetSize(); j++)
	{
		CTwinVector *pTv	= &tvArrPlane.GetAt(j);
		if(pTv==NULL)		continue;
		if(pTv->m_sInfo == strPlane)
		{
			if(pTv->GetLength()!=0)		tvArrReturn.Add(*pTv);
		}
	}
	if(tvArrReturn.GetSize()<=0)		return FALSE;
	return TRUE;
}

BOOL CUbiARcBridgeData::MakeOffsetRebar(CRcBridgeRebar *pBri, CRebarInfoCycle *pRC, long nJijum)
{
	if(!pBri)													return FALSE;
	if(!pRC || !pRC->m_bExist)						return FALSE;
	if(nJijum<0 || nJijum>=pBri->GetCountJijum())	return FALSE;
	
	// 철근 만들기
	CUbEttRebar rebar;
	if(!MakeRebar(rebar,  pRC))						return FALSE;
	if(rebar.GetPathArray().GetSize()==0)		return FALSE;
	TrsPlane(rebar.m_arrUbEdge, PLN_XOZ, 0);
	
	// 진행 가이드선 구하기.
	double  dSta = pBri->GetStationOnJijum(nJijum);
	CDPoint  vAng	= pBri->GetAngleJijum(nJijum);
	CTwinVector tvJijum = pBri->GetTvJijum(nJijum, 0);
	CTwinVectorArray tvArrLine;
	if(!GetTvArrHunchGuideLine(tvArrLine, pBri, dSta, vAng))	return FALSE;
	
	long nTvNum	= 0;
	for(nTvNum=0; nTvNum<tvArrLine.GetSize(); nTvNum++)
	{
		CTwinVector	*pTv	= &tvArrLine.GetAt(nTvNum);
		CUbEttRebar rebarTemp;
		rebarTemp	= rebar; 
		CDPoint xyBase	= pTv->GetXyTop();
		CVector vMove(0, xyBase.x, 0);
		rebarTemp.Move(vMove);
		
		double dPathLength	= pTv->GetLength();
		long nNumCtc	= (dPathLength/pBri->m_dCTC_MainRebar);
		double dSpace	= (dPathLength - (pBri->m_dCTC_MainRebar*nNumCtc))/2;
		
		CVectorArray vArr;
		vArr.Add(CVector(0,0,0));
		CDPoint xyDir	= (pTv->GetXyRight()-pTv->GetXyLeft()).Unit();
		CDPoint xyAngDir = (tvJijum.GetXyTop() - tvJijum.GetXyBottom()).Unit();
		CDPoint xyNext, xyAngNext;
		long nMainDim = 0;
		for(nMainDim=0;nMainDim<=nNumCtc;nMainDim++)
		{
			if(nTvNum==0)
			{
				xyNext	= -xyDir * (nMainDim*pBri->m_dCTC_MainRebar);
				xyAngNext	= -xyAngDir * (nMainDim*pBri->m_dCTC_MainRebar);
			}
			else if(nTvNum==tvArrLine.GetSize()-1)
			{
				xyNext	= xyDir * (nMainDim*pBri->m_dCTC_MainRebar);
				xyAngNext	= xyAngDir * (nMainDim*pBri->m_dCTC_MainRebar);
			}
			vArr.Add(CVector(xyAngNext.x, xyAngNext.y, xyNext.y));
		}
		
		OffsetRebar(rebarTemp, vArr, REBAR_RCWALL, GetColorRebar(REBARCOLOR_MAIN));
	}
	return TRUE;
}

void CUbiARcBridgeData::MakeRcUpperSlabGriderRebar(CRcBridgeRebar *pBri, long nJijum)
{
	if(!pBri)		return;
	if(nJijum<0 || nJijum>=pBri->GetCountJijum())	return;

	CString sItemPathName	= m_sItemPathNameHeader;
	m_sItemPathNameHeader	= sItemPathName + _T("/ 주철근");
	MakeRcUpperSlabGriderRebar_MainRebar((CRcBridgeRebar*)pBri, nJijum);
	m_sItemPathNameHeader	= sItemPathName + _T("/ 배력철근");
	MakeRcUpperSlabGriderRebar_SupportRebar((CRcBridgeRebar*)pBri, nJijum);
}

void CUbiARcBridgeData::MakeRcUpperSlabGriderRebar_MainRebar(CRcBridgeRebar *pBri, long nJijum)
{
	if(!pBri)		return;
	if(nJijum<0 || nJijum>=pBri->GetCountJijum())	return;
	
	BOOL bStt=TRUE, bLeft=TRUE, bGagak=TRUE, bJongGirder=FALSE;
	CGirderSectionRebar *pGirderRB		= bJongGirder ? 
													pBri->GetGirderRebarJong(bStt, bLeft, bGagak) : &pBri->m_girderRebar[nJijum];
	if(pGirderRB==NULL)		return;
	CTwinVectorArray tvArrStrup;
	pBri->GetTvRebarStirrup_UpperSlabGrider(tvArrStrup, nJijum, bStt, bLeft, bGagak, bJongGirder, TRUE);
	if(tvArrStrup.GetSize() == 0) return;

	CUbEttRebar rebar;
	if(!MakeRebar(rebar,  tvArrStrup))			return;
	if(rebar.GetPathArray().GetSize()==0)		return;
	TrsPlane(rebar.m_arrUbEdge, PLN_XOZ, 0);

	CTwinVectorArray tvArr;
	pBri->GetTvRebarMain_Plane_UpperSlabGirder(tvArr, nJijum, -1);
	long nRebarSize = tvArr.GetSize();
	// 진행 가이드선 구하기.
	double  dSta = pBri->GetStationOnJijum(nJijum);
	CDPoint  vAng	= pBri->GetAngleJijum(nJijum);
	CTwinVectorArray tvArrLine;
	if(!GetTvArrHunchGuideLine(tvArrLine, pBri, dSta, vAng))	return;

	CDPoint xyPlan = tvArr.GetAt(0).GetXyLeft();
	CDPoint xyOrg = CDPoint(tvArrStrup.GetXyLeft().x, 0);
	CVector vMove = xyPlan - xyOrg;
	vMove -= CVector(0,0,pGirderRB->m_dCoverUpper[0]);
	rebar.Move(vMove);

	CDPoint vAngPlan = (tvArr.GetAt(0).GetXyRight() - tvArr.GetAt(0).GetXyLeft()).Unit();
	CDPoint vAngOrg(1,0);
	CDPoint vAngRot =  ToDPointFrDegree(vAngPlan.GetAngleDegree() - vAngOrg.GetAngleDegree());
	rebar.Rotate(xyPlan, vAngRot);

	double dPathLength	= tvArrLine.GetLength();
	double dCtcMainRebar	= (dPathLength/nRebarSize);
	double dSpace	= (dPathLength - (dCtcMainRebar*nRebarSize))/2;

	CVectorArray vArr;
	long nTvNum	= 0, nRebar=0;
	for(nTvNum=0; nTvNum<tvArrLine.GetSize(); nTvNum++)
	{
		CTwinVector	*pTv	= &tvArrLine.GetAt(nTvNum);
		vArr.RemoveAll();
		double dHeight = (nTvNum==0) ? pTv->GetXyLeft().y : pTv->GetXyRight().y;
		vArr.Add(CVector(tvArr.GetAt(0).GetXyLeft().x, tvArr.GetAt(0).GetXyLeft().y, dHeight));
		long i = 0;
		CDPoint xyDir	= (pTv->GetXyRight()-pTv->GetXyLeft()).Unit();
		CDPoint xyNext(0,0);
		for(i = 0; i < nRebarSize/2; i++)
		{
			if(nRebar>=nRebarSize)		break;
			xyNext = pTv->GetXyLeft() + xyDir * (i*dCtcMainRebar) + dSpace;
			vArr.Add(CVector(tvArr.GetAt(nRebar).GetXyLeft().x, tvArr.GetAt(nRebar).GetXyLeft().y, xyNext.y));
			nRebar++;
		}
		
		OffsetRebar(rebar,vArr, REBAR_COLUMN, GetColorRebar(REBARCOLOR_MAIN));
	}
}

void CUbiARcBridgeData::MakeRcUpperSlabGriderRebar_SupportRebar(CRcBridgeRebar *pBri, long nJijum)
{
	if(!pBri)		return;
	if(nJijum<0 || nJijum>=pBri->GetCountJijum())	return;
	
	BOOL bStt=TRUE, bLeft=TRUE, bGagak=TRUE, bJongGirder=FALSE;
	CGirderSectionRebar *pGirderRB		= bJongGirder ? 
													pBri->GetGirderRebarJong(bStt, bLeft, bGagak) : &pBri->m_girderRebar[nJijum];
	if(pGirderRB==NULL)		return;
	// 배력철근 구하기.
	double  dSta = pBri->GetStationOnJijum(nJijum);
	CDPoint  vAng	= pBri->GetAngleJijum(nJijum);
	CTwinVector tvJijum = pBri->GetTvJijum(nJijum, 0);
	CTwinVectorArray tvArrLine;
	if(!GetTvArrHunchGuideLine(tvArrLine, pBri, dSta, vAng))	return;

	CUbEttRebar rebar;
	if(!MakeRebar(rebar,  tvArrLine))			return;
	if(rebar.GetPathArray().GetSize()==0)		return;
	TrsPlane(rebar.m_arrUbEdge, PLN_XOZ, 0);
	
	CDPoint xyPlan	= tvJijum.GetXyBottom();
	//tvJijum.GetXyBottom()값을 켄틸레버의 폭길이와 연단길이를 계산해서 변경.
	CDPoint xyDir = (tvJijum.GetXyTop() - tvJijum.GetXyBottom()).Unit();
	double dLenR	= (pBri->m_dDH[1] + pBri->m_dCantilB[1])/vAng.y;
	xyPlan	+= xyDir * dLenR;
	CDRect rcJijumLine	= tvArrLine.GetRect();
	CDPoint xyOrg	= CDPoint(rcJijumLine.right, 0);
	CVector vMove	= xyPlan - xyOrg;
	vMove += CVector(0,0,pBri->m_dCoverLower_UpperSlab[0]);					// 슬래브 하면 피복값 	
	rebar.Move(vMove);		
	CDPoint vAngPlan	= (tvJijum.GetXyBottom() - tvJijum.GetXyTop()).Unit();
	CDPoint vAngOrg		= CDPoint(1, 0);
	CVector vAngRot		= ToDPointFrDegree(vAngPlan.GetAngleDegree() - vAngOrg.GetAngleDegree());
	rebar.Rotate(xyPlan, vAngRot);

	CDPointArray xyArr, xyArrDirApplyScale;
	double dHeightTop = tvArrLine.GetXyTop().y;
	// 스트럽 주철근(내외부) ///////////////////////////////////
	// 상하.
	long upper=0, dan=0;
	for(upper = 0; upper < 2; upper++)
	{
		BOOL bUpper		= (upper == iUPPER);
		for(dan = 0; dan < MAX_REBAR_DAN; dan++)
		{
			// 스트럽  주철근
			if(dan == 0)
				pBri->GetxyRebarSupport_UpperSlabGirder(xyArr, xyArrDirApplyScale, upper, nJijum, bStt, bLeft, bGagak, bJongGirder);
			else
				pBri->GetXyRebarSupport_UpperSlabGirderInner(xyArr, nJijum, upper, dan, bStt, bLeft, bGagak, bJongGirder);

			if(xyArr.GetSize()==0)		continue;
			CVectorArray vArr;
			vArr.Add(CVector(tvJijum.GetXyMid().x,0,dHeightTop));
			long i	= 0;
			for(i=0; i<xyArr.GetSize(); i++)
				vArr.Add(CVector(xyArr.GetAt(i).x, 0, xyArr.GetAt(i).y));
			
			OffsetRebar(rebar, vArr, REBAR_RCWALL, GetColorRebar(REBARCOLOR_SUPPORT));
		}
	}
	// 좌우(수평전단철근임)
	pBri->GetxyRebarSupport_UpperSlabGirder(xyArr, xyArrDirApplyScale, 2, nJijum, bStt, bLeft, bGagak, bJongGirder);
	if(xyArr.GetSize()==0)		return;
	CVectorArray vArr;
	vArr.Add(CVector(tvJijum.GetXyMid().x,0,dHeightTop));
	long i	= 0;
	for(i=0; i<xyArr.GetSize(); i++)
		vArr.Add(CVector(xyArr.GetAt(i).x, 0, xyArr.GetAt(i).y));
	
	OffsetRebar(rebar, vArr, REBAR_RCWALL, GetColorRebar(REBARCOLOR_SUPPORT));
}
