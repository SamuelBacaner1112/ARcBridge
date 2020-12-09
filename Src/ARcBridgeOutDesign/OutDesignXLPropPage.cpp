// OutDesignXLPropPage.cpp : implementation file
//

#include "stdafx.h"
#include "arcbridgeoutdesign.h"
#include "OutDesignXLPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COutDesignXLPropPage property page

IMPLEMENT_DYNCREATE(COutDesignXLPropPage, CPropertyPage)

COutDesignXLPropPage::COutDesignXLPropPage() : CPropertyPage(COutDesignXLPropPage::IDD)
{
	//{{AFX_DATA_INIT(COutDesignXLPropPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pOpt = NULL;
	m_bLsd = FALSE;
}

COutDesignXLPropPage::~COutDesignXLPropPage()
{
}

void COutDesignXLPropPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COutDesignXLPropPage)
	DDX_Control(pDX, ID_ALLUNCHECK, m_btnCheckNone);
	DDX_Control(pDX, ID_ALLCHECK, m_btnCheckAll);
	DDX_Control(pDX, IDC_LIST_OUTLIST, m_ctrlOutList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COutDesignXLPropPage, CPropertyPage)
	//{{AFX_MSG_MAP(COutDesignXLPropPage)
	ON_BN_CLICKED(ID_ALLCHECK, OnAllcheck)
	ON_BN_CLICKED(ID_ALLUNCHECK, OnAlluncheck)
	//}}AFX_MSG_MAP
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_OUTLIST, &COutDesignXLPropPage::OnItemchangedListOutlist)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_OUTLIST, &COutDesignXLPropPage::OnCustomdrawListOutlist)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COutDesignXLPropPage message handlers

void COutDesignXLPropPage::OnAllcheck() 
{
	// TODO: Add your control notification handler code here
	long nSize = m_ctrlOutList.GetItemCount();
	long i=0; for(i=0; i<nSize; i++)
	{
		m_ctrlOutList.SetCheck(i, TRUE);
	}
	
}

void COutDesignXLPropPage::OnAlluncheck() 
{
	// TODO: Add your control notification handler code here
	long nSize = m_ctrlOutList.GetItemCount();
	long i=0; for(i=0; i<nSize; i++)
	{
		m_ctrlOutList.SetCheck(i, FALSE);
	}
	
}

BOOL COutDesignXLPropPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here

	InitListCtrl();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COutDesignXLPropPage::InitListCtrl()
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
	m_ctrlOutList.SetExtendedStyle(ExtStyle);

	m_ctrlOutList.InsertColumn(0,"출력항목",LVCFMT_LEFT,250);
		
	int i=0; for(i=0;i<m_pOpt->m_pXLOption->GetOptionSize(); i++)
	{	
		CString strNum  = _T("");
		strNum.Format(" %2d. ", i+1);
		CString strItem = strNum + m_pOpt->m_pXLOption->GetItem(i);
		BOOL bIsCheck	= m_pOpt->m_pXLOption->IsCheck(i);
		
		lvitem.mask = LVIF_TEXT;	
		lvitem.iItem = i;
		lvitem.iSubItem = 0;
		lvitem.pszText = strItem.GetBuffer(strItem.GetLength());
		m_ctrlOutList.InsertItem(&lvitem);
		
		m_ctrlOutList.SetCheck(i, bIsCheck);
	}	
}

void COutDesignXLPropPage::SetDataSave()
{
	long nSize = m_ctrlOutList.GetItemCount();
	int i=0; for(i=0; i<nSize; i++)
	{					
		BOOL bIsCheck = m_ctrlOutList.GetCheck(i) ? TRUE : FALSE;
		m_pOpt->m_pXLOption->SetCheck(i, bIsCheck);
	}
	
}

void COutDesignXLPropPage::OnOK() 
{
	// TODO: Add your specialized code here and/or call the base class
	SetDataSave();
	
	CPropertyPage::OnOK();
}


void COutDesignXLPropPage::OnItemchangedListOutlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	
	if (pNMLV->uChanged & LVIF_STATE)
	{
		if (pNMLV->uOldState & INDEXTOSTATEIMAGEMASK(1) && pNMLV->uNewState & INDEXTOSTATEIMAGEMASK(2)) // 체크되었음
		{
			if (IsEnableItem(m_pOpt->m_pXLOption->GetItem(pNMLV->iItem)) == FALSE)
			{
				m_ctrlOutList.SetCheck(pNMLV->iItem, FALSE);
			}
		}
	}

	*pResult = 0;
}


void COutDesignXLPropPage::OnCustomdrawListOutlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	LPNMLVCUSTOMDRAW pNMLVCD = reinterpret_cast<LPNMLVCUSTOMDRAW>(pNMHDR);
	*pResult = CDRF_DODEFAULT;

	switch(pNMCD->dwDrawStage)
	{
	case CDDS_PREPAINT:
		{
			*pResult = CDRF_NOTIFYITEMDRAW;
			break;
		}
	case CDDS_ITEMPREPAINT:
		{
			if (IsEnableItem(m_pOpt->m_pXLOption->GetItem(pNMCD->dwItemSpec)) == FALSE)
			{
				pNMCD->uItemState = CDIS_DISABLED;
				pNMLVCD->clrText = RGBREADONLY_TEXT;
				pNMLVCD->clrTextBk = RGBREADONLY;
			}
			
			break;
		}
	}
}

HGBOOL COutDesignXLPropPage::IsEnableItem(const CString &str) const
{
	if (m_bLsd)
	{
		if (str == "우각부 및 배력철근 검토")
		{
			return FALSE;
		}
	}
	else
	{
		if (str == "노출등급")
		{
			return FALSE;
		}

		if (str == "캔틸레버 검토")
		{
			return FALSE;
		}
	}

	return TRUE;
}
