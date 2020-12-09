#pragma once


// CDialogSTMFootingMainMid ��ȭ �����Դϴ�.

class CDialogSTMFootingMainMid : public CDialog
{
	DECLARE_DYNAMIC(CDialogSTMFootingMainMid)

public:
	CDialogSTMFootingMainMid(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDialogSTMFootingMainMid();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_STM_FOOTING_MAIN_MID };

public:
	CARcBridgeInputStd* m_pIn;
	HGINT32 m_nCurIdx;
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
	CFootingApp	m_FootingOri;
};
