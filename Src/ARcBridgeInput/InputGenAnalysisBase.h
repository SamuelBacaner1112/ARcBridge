#pragma once


// CInputGenAnalysisBase 대화 상자입니다.

class CInputGenAnalysisBase : public CInputDialogBase
{
	DECLARE_DYNAMIC(CInputGenAnalysisBase)

public:
	CInputGenAnalysisBase(UINT nID, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CInputGenAnalysisBase();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	afx_msg virtual void OnSelchangeComboBridgeNo();
	afx_msg virtual void OnSelchangeComboLoadcase();
	afx_msg virtual void OnSelchangeComboLoadtype();
	afx_msg void OnSelEndOkBackClr();

	afx_msg void OnKillfocusEditJointnumSize();
	
	afx_msg void OnCheckLoadtype();
	afx_msg void OnCheckElement(); 
	afx_msg void OnCheckJoint();
	afx_msg void OnCheckOutValue();
	afx_msg void OnCheckRestraint();
	
	afx_msg void OnRadioView3d();
	afx_msg void OnRadioViewXy();
	afx_msg void OnRadioViewXz();
	afx_msg void OnRadioViewYz();

	afx_msg void OnButtonViewSetting();
	afx_msg void OnButtonSaveImgage();

	afx_msg void OnDestroy();

	void InitComboBridge();

protected:
	CXTPComboBox	m_ComboLoadCase;
	CXTPComboBox	m_ComboLoadCombo;
	CXTPComboBox	m_cbBridgeNo;
	CXTColorPicker	m_btnBkChange;
	CButton	m_btnLoadOutput;
	CButton	m_btnMakeData;
	CButton	m_btnSaveImage;
	CButton	m_btnRunTool;
	CButton	m_btnViewCalc;
	BOOL	m_bCheckViewJoint;
	BOOL	m_bChceckViewElement;
	BOOL	m_bCheckLoadType;
	BOOL	m_bCheckViewOutValue;
	BOOL	m_bCheckViewRestraint;	
	int		m_nRadioView;	
	double	m_dSizeDataValue;
	double	m_dSizeElement;
	double	m_dSizeJoint;

	char    m_cAxis;
};
