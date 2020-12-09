#pragma once


// CDialogSTMArcCornerShear 대화 상자입니다.

class CDialogSTMArcCornerShear : public CDialog
{
	DECLARE_DYNAMIC(CDialogSTMArcCornerShear)

public:
	CDialogSTMArcCornerShear(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDialogSTMArcCornerShear();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_STM_ARCCORNER_SHEAR };

public:
	CARcBridgeInputStd* m_pIn;
	HGINT32 m_ixStmKey;
	BOOL m_bUpper;
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

	void SetDanRebarLocation();
	DECLARE_MESSAGE_MAP()

protected:
	CGridCtrlMng m_Grid;
	HGINT32 m_nUpper;
	HGINT32 m_nStt;
	HGDOUBLE m_dDiaCycle1Ori[3];
	HGDOUBLE m_dDiaCycle2Ori[3];
	HGINT32 m_nCountStirrupAtDanBuRebarOri;
	HGDOUBLE m_dDiaStirrupAtDanBuRebarOri;
};
