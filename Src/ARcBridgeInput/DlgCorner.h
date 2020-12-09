#pragma once


// CDlgCorner 대화 상자입니다.

class AFX_EXT_CLASS CDlgCorner : public CDialog
{
	DECLARE_DYNAMIC(CDlgCorner)

public:
	CDlgCorner(CARcBridgeInputStd* pIn, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgCorner();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_DANCORNER };
	double	m_dUpSlab_1Dan;
	double	m_dUpSlab_2Dan;
	double	m_dUpSlab_3Dan;
	double	m_dLoSlab_1Dan;
	double	m_dLoSlab_2Dan;
	double	m_dLoSlab_3Dan;	

	// (ARCBRIDGE-3636)
	double m_dUpSlab_CornerVert;
	double m_dLoSlab_CornerVert;
	BOOL	  m_bUpSlab_CornerCurve;
	BOOL	  m_bLoSlab_CornerCurve;

private:
	CARcBridgeInputStd *m_pIn;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonDefault();
	virtual void OnCancel();
	virtual void OnOK();

	afx_msg void OnClickOption();

	DECLARE_MESSAGE_MAP()

public:
	void SetInitData();
};
