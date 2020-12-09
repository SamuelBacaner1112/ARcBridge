#pragma once


// CDialogSetExpJointSpace 대화 상자입니다.

class CDialogSetExpJointSpace : public CDialog
{
	DECLARE_DYNAMIC(CDialogSetExpJointSpace)

public:
	CDialogSetExpJointSpace(CRcBridgeApp* pBri, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDialogSetExpJointSpace();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_SET_EXPANSTION_JOIN_SPACE };

	CGridCtrlMng	m_Grid;
	CRcBridgeApp*	m_pBri;

private:
	int m_nSelRadio;

	void SetDataInit();
	void SetGridData();
	void SetDataSave();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	afx_msg void OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result);
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonInit();
	afx_msg void OnBnClickedOk();
	afx_msg void OnRadioSelect(WPARAM nID);
};
