#pragma once


// CDialogSTMFootingMainStartEnd ��ȭ �����Դϴ�.

class CDialogSTMFootingMainStartEnd : public CDialog
{
	DECLARE_DYNAMIC(CDialogSTMFootingMainStartEnd)

public:
	CDialogSTMFootingMainStartEnd(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDialogSTMFootingMainStartEnd();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_STM_FOOTING_MAIN_STARTEND };

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

	HGBOOL ISFootSttType();

	DECLARE_MESSAGE_MAP()

protected:
	CGridCtrlMng m_Grid;
	CFootingApp	m_FootingOri;
};
