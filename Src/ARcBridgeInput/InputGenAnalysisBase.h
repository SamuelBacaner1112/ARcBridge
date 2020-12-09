#pragma once


// CInputGenAnalysisBase ��ȭ �����Դϴ�.

class CInputGenAnalysisBase : public CInputDialogBase
{
	DECLARE_DYNAMIC(CInputGenAnalysisBase)

public:
	CInputGenAnalysisBase(UINT nID, CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CInputGenAnalysisBase();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
