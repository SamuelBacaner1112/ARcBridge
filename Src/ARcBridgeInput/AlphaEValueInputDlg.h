#pragma once


// CAlphaEValueInputDlg ��ȭ �����Դϴ�.

class CAlphaEValueInputDlg : public CUserDialog
{
	DECLARE_DYNAMIC(CAlphaEValueInputDlg)

public:
	CAlphaEValueInputDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CAlphaEValueInputDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_ALPHAEVALUE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
