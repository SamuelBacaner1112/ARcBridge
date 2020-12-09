// OutDesignXLProPage_Exel.cpp : implementation file
//

#include "stdafx.h"
#include "arcbridgeoutdesign.h"
#include "../../Src/ARcBridgeData/ARcBridgeData.h"
#include "OutDesignXLProPage_Exel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COutDesignXLProPage_Exel property page

IMPLEMENT_DYNCREATE(COutDesignXLProPage_Exel, CPropertyPage)

COutDesignXLProPage_Exel::COutDesignXLProPage_Exel() : CPropertyPage(COutDesignXLProPage_Exel::IDD)
{
	//{{AFX_DATA_INIT(COutDesignXLProPage_Exel)
	m_pOpt = NULL;
	m_bUseDefault = FALSE;
	m_szFont = _T("");
	m_szLeft = _T("");
	m_szBottom = _T("");
	m_szRight = _T("");
	m_szTop = _T("");
	m_nSizeFont = -1;
	m_szPageRow = _T("");
	//}}AFX_DATA_INIT
}

COutDesignXLProPage_Exel::~COutDesignXLProPage_Exel()
{
}

void COutDesignXLProPage_Exel::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COutDesignXLProPage_Exel)
	DDX_Check(pDX, IDC_CHECK_DEFAULT_EXCEL, m_bUseDefault);
	DDX_CBString(pDX, IDC_COMBO_FONT, m_szFont);
	DDX_Text(pDX, IDC_EDIT_LEFT, m_szLeft);
	DDX_Text(pDX, IDC_EDIT_BOTTOM, m_szBottom);
	DDX_Text(pDX, IDC_EDIT_RIGHT, m_szRight);
	DDX_Text(pDX, IDC_EDIT_TOP, m_szTop);
	DDX_Text(pDX, IDC_EDIT_PAGEROW, m_szPageRow);
	DDX_CBIndex(pDX, IDC_COMBO_FONTSIZE, m_nSizeFont);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COutDesignXLProPage_Exel, CPropertyPage)
	//{{AFX_MSG_MAP(COutDesignXLProPage_Exel)
	ON_BN_CLICKED(IDC_CHECK_DEFAULT_EXCEL, OnCheckDefaultExcel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COutDesignXLProPage_Exel message handlers

void COutDesignXLProPage_Exel::OnCheckDefaultExcel() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	GetDlgItem(IDC_COMBO_FONT)->EnableWindow(!m_bUseDefault);
	GetDlgItem(IDC_COMBO_FONTSIZE)->EnableWindow(!m_bUseDefault);
	GetDlgItem(IDC_EDIT_BOTTOM)->EnableWindow(!m_bUseDefault);
	GetDlgItem(IDC_EDIT_LEFT)->EnableWindow(!m_bUseDefault);
	GetDlgItem(IDC_EDIT_RIGHT)->EnableWindow(!m_bUseDefault);
	GetDlgItem(IDC_EDIT_TOP)->EnableWindow(!m_bUseDefault);
	GetDlgItem(IDC_EDIT_PAGEROW)->EnableWindow(!m_bUseDefault);
	
	if(m_bUseDefault)
	{
		m_szFont	= "±¼¸²Ã¼";
		m_nSizeFont	= 9;
		m_szBottom	= "2.5";
		m_szLeft	= "1.9";
		m_szRight	= "1.9";
		m_szTop		= "2.5";
		m_szPageRow = "46";
	}

	UpdateData(FALSE);
}

BOOL COutDesignXLProPage_Exel::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	m_bUseDefault = m_pOpt->m_pStd->m_pARcBridgeDataStd->m_bExcelDefaultSetting;
	m_szFont	  = m_pOpt->m_pStd->m_pARcBridgeDataStd->m_szFontPrint;
	m_nSizeFont	  = (long)m_pOpt->m_pStd->m_pARcBridgeDataStd->m_dFontSizePrint;
	m_szLeft.Format("%.1f",m_pOpt->m_pStd->m_pARcBridgeDataStd->m_dOffsetLeftPrint);
	m_szRight.Format("%.1f",m_pOpt->m_pStd->m_pARcBridgeDataStd->m_dOffsetRightPrint);
	m_szBottom.Format("%.1f",m_pOpt->m_pStd->m_pARcBridgeDataStd->m_dOffsetBottomPrint);
	m_szTop.Format("%.1f",m_pOpt->m_pStd->m_pARcBridgeDataStd->m_dOffsetTopPrint);
	m_szPageRow.Format("%.0f",(double)m_pOpt->m_pStd->m_pARcBridgeDataStd->m_nPageRow);
	
	// TODO: Add extra initialization here
	GetDlgItem(IDC_COMBO_FONT)->EnableWindow(!m_bUseDefault);
	GetDlgItem(IDC_COMBO_FONTSIZE)->EnableWindow(!m_bUseDefault);
	GetDlgItem(IDC_EDIT_BOTTOM)->EnableWindow(!m_bUseDefault);
	GetDlgItem(IDC_EDIT_LEFT)->EnableWindow(!m_bUseDefault);
	GetDlgItem(IDC_EDIT_RIGHT)->EnableWindow(!m_bUseDefault);
	GetDlgItem(IDC_EDIT_TOP)->EnableWindow(!m_bUseDefault);
	GetDlgItem(IDC_EDIT_PAGEROW)->EnableWindow(!m_bUseDefault);

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COutDesignXLProPage_Exel::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData(TRUE);
	m_pOpt->m_pStd->m_pARcBridgeDataStd->m_bExcelDefaultSetting = m_bUseDefault;
	m_pOpt->m_pStd->m_pARcBridgeDataStd->m_szFontPrint			= m_szFont;
	m_pOpt->m_pStd->m_pARcBridgeDataStd->m_dFontSizePrint		= (double)m_nSizeFont;
	m_pOpt->m_pStd->m_pARcBridgeDataStd->m_dOffsetLeftPrint		= atof(m_szLeft);
	m_pOpt->m_pStd->m_pARcBridgeDataStd->m_dOffsetRightPrint	= atof(m_szRight);
	m_pOpt->m_pStd->m_pARcBridgeDataStd->m_dOffsetTopPrint		= atof(m_szTop);
	m_pOpt->m_pStd->m_pARcBridgeDataStd->m_dOffsetBottomPrint	= atof(m_szBottom);
	m_pOpt->m_pStd->m_pARcBridgeDataStd->m_nPageRow	= (long)atof(m_szPageRow);
	
	CPropertyPage::OnOK();
}
