// OutDesignXLPropPage_Stm.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ARcBridgeOutDesign.h"

// COutDesignXLPropPage_Stm 대화 상자입니다.

IMPLEMENT_DYNAMIC(COutDesignXLPropPage_Stm, CPropertyPage)

	COutDesignXLPropPage_Stm::COutDesignXLPropPage_Stm()
	: CPropertyPage(COutDesignXLPropPage_Stm::IDD)
{
	m_pOpt = NULL;
	m_nItemOld = -1;
}

COutDesignXLPropPage_Stm::~COutDesignXLPropPage_Stm()
{
	m_pOpt = NULL;
	m_nItemOld= -1;
}

void COutDesignXLPropPage_Stm::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_GridControl(pDX, IDC_GRID_SEL_OUTDESIGN, m_Grid);
}


BEGIN_MESSAGE_MAP(COutDesignXLPropPage_Stm, CPropertyPage)
	ON_BN_CLICKED(ID_ALLCHECK, OnAllCheck)
	ON_BN_CLICKED(ID_ALLUNCHECK, OnAllUnCheck)
	ON_NOTIFY(LVN_ITEMCHANGING, IDC_LIST_OUTLIST, OnItemchangingListOutlist)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_OUTLIST, OnItemchangedListOutlist)
	ON_NOTIFY(NM_CLICK, IDC_LIST_OUTLIST, OnClickListOutlist)
END_MESSAGE_MAP()

BOOL COutDesignXLPropPage_Stm::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	SetWindowText(hggettext("기타 옵션"));
	GetDlgItem(ID_ALLCHECK)->SetWindowText(hggettext("전체선택"));
	GetDlgItem(ID_ALLUNCHECK)->SetWindowText(hggettext("전체해제"));

	SetGridData();
	SetDataInit();

	return TRUE;
}
void COutDesignXLPropPage_Stm::InitListCtrl()
{

}

void COutDesignXLPropPage_Stm::OnOK() 
{
	SetDataSave();
	CPropertyPage::OnOK();
}

void COutDesignXLPropPage_Stm::SetDataSave()
{
	m_Grid.UpdateData(TRUE);
}

void COutDesignXLPropPage_Stm::OnAllCheck() 
{
	COptionItem *pOpt	= m_pOpt->m_pStmOption;
	long nSize = pOpt->m_dArrItem.GetSize();
	long i = 0; for(i = 0; i < nSize; i++)
		pOpt->SetCheck(i, pOpt->m_dArrEnable.GetAt(i) == TRUE  ? TRUE : FALSE);

	SetGridData();
	SetDataInit();
}

void COutDesignXLPropPage_Stm::OnAllUnCheck() 
{
	COptionItem *pOpt	= m_pOpt->m_pStmOption;
	long nSize = pOpt->m_dArrItem.GetSize();
	long i = 0; for(i = 0; i < nSize; i++)
		pOpt->SetCheck(i, FALSE);

	SetGridData();
	SetDataInit();
}

void COutDesignXLPropPage_Stm::OnItemchangingListOutlist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	m_nItemOld	= pNMListView->iItem;

	*pResult = 0;
}

void COutDesignXLPropPage_Stm::OnItemchangedListOutlist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	if(!IsEnableItem(pNMListView->iItem))
	{

	}

	*pResult = 0;
}

void COutDesignXLPropPage_Stm::OnClickListOutlist(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	if(!IsEnableItem(pNMListView->iItem))
	{
		InitListCtrl();
	}

	*pResult = 0;
}

BOOL COutDesignXLPropPage_Stm::IsEnableItem(long nItem)
{
	return m_pOpt->m_pStmOption->IsEnable(nItem);
}

void COutDesignXLPropPage_Stm::SetGridData()
{
	m_Grid.SetGridDefault(2, 2, 1, 0);
	m_Grid.SetCellSize(19, -1);
	m_Grid.SetGridLines(GVL_NONE);

	long row = 0;
	long col = 0;
	UINT nFormat = DT_LEFT | DT_VCENTER;
	UINT nLine = CELL_LINE_NO;

	m_Grid.AddCell(row, col, hggettext("출력항목"));
	m_Grid.AddCell(row++, col+1, hggettext("출력항목"));


	COptionItem *pOpt	= m_pOpt->m_pStmOption;
	long i = 0; for(i = 0; i < pOpt->GetOptionSize(); i++)
	{
		CString strNum  = _T("");
		strNum.Format(_T(" %2d. "), i+1);
		CString strItem = strNum + m_pOpt->m_pStmOption->GetItem(i);
		BOOL bIsEnable	= pOpt->IsEnable(i);

		m_Grid.AddCell(row, col, &pOpt->m_dArrItem, i, nFormat, bIsEnable ? nLine : CELL_READONLY, _T(""), CELL_TYPE_CHECK);
		m_Grid.AddCell(row++, col+1, strItem, nFormat, bIsEnable ? nLine : CELL_READONLY);
	}

	m_Grid.GetCellCount(row, col);
	m_Grid.SetRows(row);
	m_Grid.SetCols(col);
	m_Grid.SetColumnWidth(0, 30);
	m_Grid.SetColumnWidth(1, 220);
	m_Grid.ExpandColumnsToFit();
}

void COutDesignXLPropPage_Stm::SetDataInit()
{
	m_Grid.UpdateData(FALSE);

	long nRows	= m_Grid.GetRows();
	long nCols	= m_Grid.GetCols();

	if(nRows > 0 && nCols > 0)
		m_Grid.MergeGrid(0, 0, 0, 1);
}
