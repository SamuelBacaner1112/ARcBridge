#pragma once


// CEarthquakeGradeKDS17Dlg ��ȭ �����Դϴ�.

class CEarthquakeGradeKDS17Dlg : public CUserDialog
{
public:
	CEarthquakeGradeKDS17Dlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CEarthquakeGradeKDS17Dlg();

	int		m_nType;
// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_EARTHQUACKGRADE_KDS17 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
};
