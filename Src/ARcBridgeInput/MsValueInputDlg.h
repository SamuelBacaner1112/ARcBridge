#pragma once


// CMsValueInputDlg ��ȭ �����Դϴ�.

class CMsValueInputDlg : public CUserDialog
{
	DECLARE_DYNAMIC(CMsValueInputDlg)

public:
	CMsValueInputDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CMsValueInputDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_MSVALUE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
