#pragma once


// CDialogRailLoadAlways 대화 상자입니다.

class CDialogRailLoadAlways : public CUserDialog
{
	DECLARE_DYNAMIC(CDialogRailLoadAlways)

public:
	CDialogRailLoadAlways(CRcBridgeApp *pBri, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDialogRailLoadAlways();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_RAILLOAD_ALWAYS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	
	virtual void OnOK();

public:
	virtual BOOL OnInitDialog();

private:
	void SetGridData();
	void SetDataSave();
	void SetDataInit();
	void SetDataDefault();

	afx_msg void OnButtonDefault();

private:
	CGridCtrlMng m_Grid;
	CRcBridgeApp *m_pBri;
};
