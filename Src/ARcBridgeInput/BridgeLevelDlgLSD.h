#pragma once


// CBridgeLevelDlgLSD ��ȭ �����Դϴ�.

class CBridgeLevelDlgLSD : public CUserDialog
{
	DECLARE_DYNAMIC(CBridgeLevelDlgLSD)

public:
	CBridgeLevelDlgLSD(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CBridgeLevelDlgLSD();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_BRIDGELEVEL_LSD };
	int		m_nType;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

protected:
	virtual void OnOK();
	virtual BOOL OnInitDialog();
};
