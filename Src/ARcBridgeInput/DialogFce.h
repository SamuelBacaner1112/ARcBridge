#pragma once


// CDialogFce ��ȭ �����Դϴ�.

class AFX_EXT_CLASS CDialogFce : public CDialog
{
	DECLARE_DYNAMIC(CDialogFce)

public:
	CDialogFce(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDialogFce();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_FCE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
