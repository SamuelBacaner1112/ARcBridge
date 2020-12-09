#pragma once


// COutDesignXLPropPage_Stm 대화 상자입니다.

class COutDesignXLPropPage_Stm : public CPropertyPage
{
	DECLARE_DYNAMIC(COutDesignXLPropPage_Stm)

public:
	COutDesignXLPropPage_Stm();
	virtual ~COutDesignXLPropPage_Stm();

	void SetDataInit();
	void SetGridData();
	BOOL IsEnableItem(long nItem);
	long m_nItemOld;

	void SetDataSave();
	void InitListCtrl();

	CGridCtrlMng m_Grid;
	COutDesignOptionSheet *m_pOpt;

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_PROPPAGE_STM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnAllCheck();
	afx_msg void OnAllUnCheck();
	afx_msg void OnItemchangingListOutlist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchangedListOutlist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickListOutlist(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()
};
