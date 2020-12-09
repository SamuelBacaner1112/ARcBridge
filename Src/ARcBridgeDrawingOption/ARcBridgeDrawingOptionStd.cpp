// ARcBridgeDrawingOptionStd.cpp: implementation of the CARcBridgeDrawingOptionStd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ARcBridgeDrawingOption.h"
#include "../../ARcBridge/ARcBridgeDoc.h"
#include "../../Src/ARcBridgeData/ARcBridgeDataStd.h"
#include "../../Src/ARcBridgeDBDraw/ARcBridgeDBDraw.h"
#include "../ARcBridgeDrawing/ARcBridgeDrawingStd.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CARcBridgeDrawingOptionStd::CARcBridgeDrawingOptionStd()
{
	m_nCurPropPage = 0;

	DefaultDrawingOptionCommon();
}

CARcBridgeDrawingOptionStd::~CARcBridgeDrawingOptionStd()
{
	AhTPADelete(&m_pArrDrawingOption, (CARcBridgeDrawingOptionData*)0);
}

void CARcBridgeDrawingOptionStd::Serialize(CArchive& ar)
{
	CARcBridgeDoc* pDoc = dynamic_cast<CARcBridgeDoc*>(m_pDoc);
	CARcBridgeDataStd* pStd = pDoc->m_pARcBridgeDataStd;
	// 2 : m_nSelectFckDomText
	// 3 : m_nTypeRebarTable, , , 도면간소화 타입 추가
	// 4 : m_bAddRawRebarTable
	long nFlag = 4;
	long nCountOpt = m_pArrDrawingOption.GetSize();
	if(ar.IsStoring())
	{
		ar << nFlag;

		// 도면 일반
		ar << m_dwDrawCommon;
		ar << m_strDgkName;
		ar << m_strXRefPath;
		ar << m_strDomNumHead;
		m_strDgkAttrPrompt.Serialize(ar);
		m_strDgkAttrText.Serialize(ar);
		ar << m_nTypeEL;
		ar << m_nSelectFckDomText;
		ar << m_nTypeTitleSeparatorBridge;	// 분리형 교량 대제목 타입(0:도면명, 1:교량명+도면명, 2:교량명+선형/반대방향명+도면명)
		ar << m_nTypeTitleBridge;			// 일체형 교량 대제목 타입(0:도면명, 1:교량명+도면명)
		ar << m_nTypeRebarTable; // 도면간소화타입
		ar << m_dWidthRebarTableRect; //상세박스 가로
		ar << m_dHeightRebarTableRect; //상세박스 세로
		ar << m_bAddRawRebarTable;
		// 교량별 도면 옵션
		ar << nCountOpt;
		long i=0; for(i=0; i<nCountOpt; i++)
			m_pArrDrawingOption[i]->Serialize(ar);
	}
	else
	{
		ar >> nFlag;
		
		// 도면 일반
		ar >> m_dwDrawCommon;
		ar >> m_strDgkName;
		ar >> m_strXRefPath;
		ar >> m_strDomNumHead;
		m_strDgkAttrPrompt.Serialize(ar);
		m_strDgkAttrText.Serialize(ar);
		ar >> m_nTypeEL;
		if(nFlag > 1)
			ar >> m_nSelectFckDomText;

		if(nFlag > 0)
		{
			ar >> m_nTypeTitleSeparatorBridge;	// 분리형 교량 대제목 타입(0:도면명, 1:교량명+도면명, 2:교량명+선형/반대방향명+도면명)
			ar >> m_nTypeTitleBridge;			// 일체형 교량 대제목 타입(0:도면명, 1:교량명+도면명)
		}
		if(nFlag > 2)
		{
			ar >> m_nTypeRebarTable; // 도면간소화타입
			ar >> m_dWidthRebarTableRect; //상세박스 가로
			ar >> m_dHeightRebarTableRect; //상세박스 세로
		}
		if(nFlag > 3)
			ar >> m_bAddRawRebarTable;
		// 교량별 도면 옵션
		ar >> nCountOpt;
		// 파일 오픈시는 무조건 옵션을 생성해야 된다. //////
		pStd->m_bMakeDrawingOptionUsed	= FALSE;
		MakeDrawingOption(-1);	// 교량면 도면옵션 개수는 저장된 개수이고 실제로 필요한 것은 교량 개수만큼이므로 -1
		////////////////////////////////////////////////////
		long i=0; for(i=0; i<nCountOpt; i++)
		{
			if(m_pArrDrawingOption.GetSize() > i)
			{
				m_pArrDrawingOption[i]->Serialize(ar);
			}
			else
			{
				m_pArrDrawingOption[0]->Serialize(ar);
			}
		}


		CHgBaseDrawStd::SetUseHCadBlock(!(m_dwDrawCommon & COMMON_NOHCAD));
		CARcBridgeDBStandard::SetDisplayAngLenStd(m_dwDrawCommon & COMMON_DISPLAY_ANG_LEN_BASE);
	}
}

void CARcBridgeDrawingOptionStd::DefaultDrawingOptionCommon()
{
	m_dwDrawCommon = 0x000000;
	CString strPath = AfxGetApp()->GetProfileString("Afx Directory", "Fixed Directory");
	m_strDgkName = strPath + "\\Sample Files\\KHCBLKA1-A.dgk";
	m_strDgkAttrPrompt.RemoveAll();
	m_strDgkAttrText.RemoveAll();
	m_nTypeEL	= 0;
	m_nSelectFckDomText	= 0;
	m_nTypeTitleSeparatorBridge	= 0;	// 분리형 교량 대제목 타입(0:도면명, 1:교량명+도면명, 2:교량명+선형/반대방향명+도면명)
	m_nTypeTitleBridge			= 0;			// 일체형 교량 대제목 타입(0:도면명, 1:교량명+도면명)
	m_nTypeRebarTable = 0; //도면간소화타입
	m_bAddRawRebarTable = FALSE;
	m_dWidthRebarTableRect = 4000.0;
	m_dHeightRebarTableRect = 4000.0;

	m_strXRefPath = _T("");
	m_strDomNumHead = _T("");
}

// long nCountOpt : 원래 교량 개수만큼만 만들면 되지만, 이상하게 교량 개수가 저장 되지 않고 nCountOpt가 저장 되는 경우가 발생함
// 따라서 -1이면 교량 개수만큼 만들고 그렇지 않을 경우 nCountOpt만큼 옵션을 생성한다. 2005.06.17 - 재호
void CARcBridgeDrawingOptionStd::MakeDrawingOption(long nCountOpt)
{
	CARcBridgeDoc* pDoc = dynamic_cast<CARcBridgeDoc*>(m_pDoc);
	CARcBridgeDataStd* pStd = pDoc->m_pARcBridgeDataStd;

	if(!pStd->m_bMakeDrawingOptionUsed)
	{
		int nSize = pStd->GetBridgeSize();
		if(nSize==0)
			nSize = 1;
		if(nCountOpt > 0)
			nSize	= nCountOpt;
		AhTPAMake(nSize, &m_pArrDrawingOption, (CARcBridgeDrawingOptionData*)0);
	}
	pStd->m_bMakeDrawingOptionUsed = TRUE;
}

CARcBridgeDrawingOptionData* CARcBridgeDrawingOptionStd::GetDrawingOption(long nBri)
{
	if(nBri<0 || nBri>=m_pArrDrawingOption.GetSize()) 
		return NULL;

	return m_pArrDrawingOption[nBri];
}

void CARcBridgeDrawingOptionStd::SyncApsDowelRebarSeperateRebarOption(long nBri)
{
	CARcBridgeDoc* pDoc						= dynamic_cast<CARcBridgeDoc*>(m_pDoc);
	if(pDoc == NULL) return;

	CARcBridgeDataStd *pStd					= pDoc->m_pARcBridgeDataStd;
	if(pStd == NULL) return;
	
	CARcBridgeDrawingOptionData* pData = NULL;

	if(nBri == -1)
	{
		long nCntBir = pStd->GetBridgeSize();
		for (long nBir =0; nBri < nCntBir; nBri++)
		{
			pData	= GetDrawingOption(nBri);
			if(pData == NULL) continue;

			CRcBridgeApp *pBri	= pStd->GetBridge(nBri);
			if(pBri == NULL) continue;

			pData->m_bRebarTableSepByFyAps = pBri->m_bApsDowelSeparateRebar;
		}
	}
	else
	{
		pData	= GetDrawingOption(nBri);
		if(pData == NULL) return;

		CRcBridgeApp *pBri	= pStd->GetBridge(0);
		if(pBri == NULL) return;

		pData->m_bRebarTableSepByFyAps = pBri->m_bApsDowelSeparateRebar;
	}

	
}
