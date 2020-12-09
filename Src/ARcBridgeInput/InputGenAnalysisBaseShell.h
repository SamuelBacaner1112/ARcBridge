#pragma once


// CInputGenAnalysisBaseShell 대화 상자입니다.

class CInputGenAnalysisBaseShell : public CInputGenAnalysisBase
{
	DECLARE_DYNAMIC(CInputGenAnalysisBaseShell)

public:
	CInputGenAnalysisBaseShell(UINT nID, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CInputGenAnalysisBaseShell();
	

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

	afx_msg void OnRadioFxx();
	afx_msg void OnRadioFyy();
	afx_msg void OnRadioFxy();
	afx_msg void OnRadioFmax();
	afx_msg void OnRadioFmin(); 
	afx_msg void OnRadioMxx(); 
	afx_msg void OnRadioMyy(); 
	afx_msg void OnRadioMxy(); 
	afx_msg void OnRadioMmax(); 
	afx_msg void OnRadioMmin(); 
	afx_msg void OnRadioVxx(); 
	afx_msg void OnRadioVyy(); 
	afx_msg void OnRadioView3d();
	afx_msg void OnRadioViewXy();
	afx_msg void OnRadioViewXz();
	afx_msg void OnRadioViewYz();
	afx_msg void OnRadioV22();
	afx_msg void OnRadioV33();
	afx_msg void OnRadioM22();
	afx_msg void OnRadioM33();
	afx_msg void OnRadioAxial();
	afx_msg void OnRadioTortion();

protected:
	long	m_nForceType;
	long	m_nForceTypeFrame;
};
