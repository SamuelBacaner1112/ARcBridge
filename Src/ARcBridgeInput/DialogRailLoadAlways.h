#pragma once


// CDialogRailLoadAlways ��ȭ �����Դϴ�.

class CDialogRailLoadAlways : public CUserDialog
{
	DECLARE_DYNAMIC(CDialogRailLoadAlways)

public:
	CDialogRailLoadAlways(CRcBridgeApp *pBri, CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDialogRailLoadAlways();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_RAILLOAD_ALWAYS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
