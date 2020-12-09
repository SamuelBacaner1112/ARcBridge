#pragma once


// CDialogSTMFootingMainStartEnd 대화 상자입니다.

class CDialogSTMFootingMainStartEnd : public CDialog
{
	DECLARE_DYNAMIC(CDialogSTMFootingMainStartEnd)

public:
	CDialogSTMFootingMainStartEnd(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDialogSTMFootingMainStartEnd();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_STM_FOOTING_MAIN_STARTEND };

public:
	CARcBridgeInputStd* m_pIn;
	HGINT32 m_ixStmKey;
	BOOL m_bChange;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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
