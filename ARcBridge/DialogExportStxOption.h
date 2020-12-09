#pragma once


// CDialogExportStxOption 대화 상자입니다.

class CDialogExportStxOption : public CDialog
{
	DECLARE_DYNAMIC(CDialogExportStxOption)

public:
	CDialogExportStxOption(CARcBridgeDataStd *pDataStd, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDialogExportStxOption();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_EXPORT_STX_OPTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
 	void SetDataInit();
 	void SetDataSave();
 	void SetGridData();
 	void SetDataDefault();
	
	void AddOptionItem( CString strName, HGINT32 iDesignType, HGINT32 iRcBridge );
 	void SetButtonImage();
 	HGINT32 GetCountCheck();

private:
	CGridCtrlMng m_Grid;

	CARcBridgeDataStd *m_pDataStd;
	CXTPButton m_bnCheckAll;
	CXTPButton m_bnUncheckAll;

	CButton m_btnArcCorner;
	CButton m_btnBraket;
	CButton m_btnFooting;
	CButton m_btnFootingMid;

	CBitmap m_bmpAbutFooting;
	CBitmap m_bmpAbutBracket;
	CBitmap m_bmpPierCopingFront;
	CBitmap m_bmpPierCopingSide;
	CBitmap m_bmpPierFooingSide;
	CBitmap m_bmpPierFooingFront;

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCheckAll();
	afx_msg void OnBnClickedUncheckAll();
	afx_msg void OnBnClickedExport();
	afx_msg void OnBnClickedBtnArccorner();
	afx_msg void OnBnClickedBtnBracket();
	afx_msg void OnBnClickedBtnFooting();
	afx_msg void OnBnClickedBtnFootingMid();

	afx_msg void OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result);
};
