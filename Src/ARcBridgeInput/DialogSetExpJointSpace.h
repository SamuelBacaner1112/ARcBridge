#pragma once


// CDialogSetExpJointSpace ��ȭ �����Դϴ�.

class CDialogSetExpJointSpace : public CDialog
{
	DECLARE_DYNAMIC(CDialogSetExpJointSpace)

public:
	CDialogSetExpJointSpace(CRcBridgeApp* pBri, CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDialogSetExpJointSpace();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_SET_EXPANSTION_JOIN_SPACE };

	CGridCtrlMng	m_Grid;
	CRcBridgeApp*	m_pBri;

private:
	int m_nSelRadio;

	void SetDataInit();
	void SetGridData();
	void SetDataSave();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	afx_msg void OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result);
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonInit();
	afx_msg void OnBnClickedOk();
	afx_msg void OnRadioSelect(WPARAM nID);
};
