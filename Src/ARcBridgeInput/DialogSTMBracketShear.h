#pragma once


// CDialogSTMBracketShear ��ȭ �����Դϴ�.

class CDialogSTMBracketShear : public CDialog
{
	DECLARE_DYNAMIC(CDialogSTMBracketShear)

public:
	CDialogSTMBracketShear(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDialogSTMBracketShear();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_STM_BRACKET_SHEAR };

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

	void SetBracketLoation();

	DECLARE_MESSAGE_MAP()

protected:
	CGridCtrlMng m_Grid;
	HGINT32 m_nStt;

	HGDOUBLE m_dDiaRebarOri;
	HGINT32 m_nTypeOri;
	HGINT32 m_nCountWrapOri;
};
