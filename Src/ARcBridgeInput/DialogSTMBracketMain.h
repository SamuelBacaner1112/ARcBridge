#pragma once


// CDialogSTMBracketMain ��ȭ �����Դϴ�.

class CDialogSTMBracketMain : public CDialog
{
	DECLARE_DYNAMIC(CDialogSTMBracketMain)

public:
	CDialogSTMBracketMain(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDialogSTMBracketMain();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_STM_BRACKET_MAIN };

public:
	CARcBridgeInputStd* m_pIn;
	HGINT32 m_ixStmKey;
	BOOL m_bChange;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	virtual BOOL OnInitDialog();
	afx_msg void OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

	void SetDataInit();
	void SetDataSave();
	void SetGridData();

	DECLARE_MESSAGE_MAP()

protected:
	CGridCtrlMng m_Grid;
	double m_dDiaRebarCycleOri1;
	double m_dDiaRebarCycleOri2;
};
