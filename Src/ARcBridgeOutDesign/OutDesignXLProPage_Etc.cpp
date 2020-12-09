// OutDesignXLProPage_Etc.cpp : implementation file
//

#include "stdafx.h"
#include "arcbridgeoutdesign.h"
#include "OutDesignXLProPage_Etc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COutDesignXLProPage_Etc property page

IMPLEMENT_DYNCREATE(COutDesignXLProPage_Etc, CPropertyPage)

COutDesignXLProPage_Etc::COutDesignXLProPage_Etc() : CPropertyPage(COutDesignXLProPage_Etc::IDD)
{
	//{{AFX_DATA_INIT(COutDesignXLProPage_Etc)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pOpt = NULL;
}

COutDesignXLProPage_Etc::~COutDesignXLProPage_Etc()
{
}

void COutDesignXLProPage_Etc::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COutDesignXLProPage_Etc)
	DDX_Control(pDX, ID_ALLUNCHECK, m_btnCheckNone);
	DDX_Control(pDX, ID_ALLCHECK, m_btnCheckAll);
	DDX_Control(pDX, IDC_LIST_OUTLIST_ETC, m_ctrlEtcList_Etc);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COutDesignXLProPage_Etc, CPropertyPage)
	//{{AFX_MSG_MAP(COutDesignXLProPage_Etc)
	ON_BN_CLICKED(ID_ALLCHECK, OnAllcheck)
	ON_BN_CLICKED(ID_ALLUNCHECK, OnAlluncheck)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COutDesignXLProPage_Etc message handlers

void COutDesignXLProPage_Etc::OnAllcheck() 
{
	// TODO: Add your control notification handler code here
	long nSize = m_ctrlEtcList_Etc.GetItemCount();
	long i=0; for(i=0; i<nSize; i++)
	{
		m_ctrlEtcList_Etc.SetCheck(i, TRUE);
	}
	
}

void COutDesignXLProPage_Etc::OnAlluncheck() 
{
	// TODO: Add your control notification handler code here
	long nSize = m_ctrlEtcList_Etc.GetItemCount();
	long i=0; for(i=0; i<nSize; i++)
	{
		m_ctrlEtcList_Etc.SetCheck(i, FALSE);
	}
		
}

BOOL COutDesignXLProPage_Etc::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here

	InitListCtrl();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COutDesignXLProPage_Etc::InitListCtrl()
{
	LV_ITEM lvitem;

	long ExtStyle = 		
			LVS_EX_FULLROWSELECT | 
	//		LVS_EX_UNDERLINEHOT | 
			LVS_EX_INFOTIP |
			LVS_EX_FLATSB  |
	//		LVS_EX_GRIDLINES |
			LVS_EX_CHECKBOXES;
	//		LVS_EX_SUBITEMIMAGES;	
	m_ctrlEtcList_Etc.SetExtendedStyle(ExtStyle);

	m_ctrlEtcList_Etc.InsertColumn(0,"출력항목",LVCFMT_LEFT,250);
		
	int i=0; for(i=0;i<m_pOpt->m_pEtcOption->GetOptionSize(); i++)
	{
		CString strItem = m_pOpt->m_pEtcOption->GetItem(i);
		BOOL bIsCheck	= m_pOpt->m_pEtcOption->IsCheck(i);		

		lvitem.mask = LVIF_TEXT;	
		lvitem.iItem = i;
		lvitem.iSubItem = 0;
		lvitem.pszText = strItem.GetBuffer(strItem.GetLength());
		m_ctrlEtcList_Etc.InsertItem(&lvitem);		

		m_ctrlEtcList_Etc.SetCheck(i, bIsCheck);
	}	
}

void COutDesignXLProPage_Etc::SetDataSave()
{
	int i=0; for(i=0;i<m_ctrlEtcList_Etc.GetItemCount();i++)
	{					
		BOOL bIsCheck = m_ctrlEtcList_Etc.GetCheck(i) ? TRUE : FALSE;
		m_pOpt->m_pEtcOption->SetCheck(i, bIsCheck);
	}
	
}

void COutDesignXLProPage_Etc::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	SetDataSave();
	
	CPropertyPage::OnOK();
}
