// ARcBridgeOutDesignStd.cpp: implementation of the CARcBridgeOutDesignStd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../../Src/ARcBridgeData/ARcBridgeData.h"
#include "ARcBridgeOutDesign.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CARcBridgeOutDesignStd::CARcBridgeOutDesignStd()
{
	m_pDlg	= new COutDesignDlg;

	m_pOutDesignOption  = new COutDesignOptionSheet("출력 옵션");
	m_pOutDesignOption->m_pStd = this;

	m_pXLOption = new COptionItem;
	m_pStmOption = new COptionItem;
	m_pEtcOption = new COptionItem;

	m_pDlg->m_pStd = this;

	m_pOutBridge = new COutDesignBridge;
	m_pOutBridge->m_pStd = this;

	m_pOutResister = new COutBridgeResister;
	m_pOutResister->m_pStd = this;

	m_nCurBridge = 0;
}

CARcBridgeOutDesignStd::~CARcBridgeOutDesignStd()
{
	delete m_pDlg;
	delete m_pOutDesignOption;
	delete m_pOutBridge;
	delete m_pOutResister;

	delete m_pXLOption;
	delete m_pStmOption;
	delete m_pEtcOption;
}

void CARcBridgeOutDesignStd::SetExcelFormat(CXLControl *pXL, long nRowStt, long nRowEnd, long nCellHeight, long nTextSize, BOOL bVert)
{
	long nColEnd = bVert ? XFL_COLEND : XFL_COLEND + 14;
	if(m_pARcBridgeDataStd->m_bExcelDefaultSetting)
	{
		//long TextSize = 9;
		CString szFont = "굴림체";
		pXL->SetCellWidth(XFL_COLSTT,nColEnd,XFL_COLWIDTH);
		pXL->SetCellHeight(nRowStt,nRowEnd,nCellHeight);
		pXL->SetFonts(nRowStt,XFL_COLSTT,nRowEnd,nColEnd,(short)nTextSize,XFL_SHEET_FONT,1,FALSE);
		pXL->SetVerAlign(nRowStt,XFL_COLSTT,nRowEnd,nColEnd,2); // 2 = CENTER
		pXL->SetHoriAlign(nRowStt,XFL_COLSTT,nRowEnd,nColEnd,TA_LEFT);
	}
	else
	{
		long TextSize  = (long)m_pARcBridgeDataStd->m_dFontSizePrint;
		CString szFont = m_pARcBridgeDataStd->m_szFontPrint;
		pXL->SetCellWidth(XFL_COLSTT,nColEnd,XFL_COLWIDTH);
		pXL->SetCellHeight(nRowStt,nRowEnd,nCellHeight);
		pXL->SetFonts(nRowStt,XFL_COLSTT,nRowEnd,nColEnd,(short)TextSize,szFont,1,FALSE);
		pXL->SetVerAlign(nRowStt,XFL_COLSTT,nRowEnd,nColEnd,2); // 2 = CENTER
		pXL->SetHoriAlign(nRowStt,XFL_COLSTT,nRowEnd,nColEnd,TA_LEFT);
		pXL->SetPrintMargin(m_pARcBridgeDataStd->m_dOffsetLeftPrint,
			                m_pARcBridgeDataStd->m_dOffsetRightPrint,
							m_pARcBridgeDataStd->m_dOffsetTopPrint,
							m_pARcBridgeDataStd->m_dOffsetBottomPrint);
	}
}