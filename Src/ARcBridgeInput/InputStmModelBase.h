#pragma once

// CInputStmModelBase 대화 상자입니다.

class CInputStmModelBase : public CInputDialogBase
{
	DECLARE_DYNAMIC(CInputStmModelBase)

public:
	CInputStmModelBase(UINT nID, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CInputStmModelBase();

	void SetUnlock();
	virtual void RecalculateStmLoadDistribution();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	virtual void OnPreInitDialog();

	void SetDataSave();
	void SetDataInit();

	void InitComboCtrl();
	void SetLoadCaseCombo();
	void InitComboCtrlTools();

	void ClearHistoryAndDeletedObject();
	void RunStad2D();

	virtual afx_msg void OnSelchangeComboBridgeNo();
	virtual afx_msg void OnCbnSelchangeComboStmcombo();
	virtual afx_msg void OnBnClickedBtnStmsetting();
	virtual afx_msg void OnBnClickedBtnDefaultmodel();
	virtual afx_msg void OnBnClickedBtnRenum();
	virtual afx_msg void OnCbnSelchangeComboLoadcase();
	virtual afx_msg void OnBnClickedBtnViewoption();
	virtual afx_msg void OnBnClickedButtonStressFlow();
	virtual afx_msg void OnBnClickedButtonSolver();
	virtual afx_msg void OnBnClickedButtonInput();
	virtual afx_msg void OnBnClickedButtonOutput();
	virtual afx_msg void OnBnClickedBtnCheckdesign();
	virtual afx_msg void OnBnClickedBtnCheckmain();
	virtual afx_msg void OnBnClickedBtnCheckshear();
	virtual afx_msg void OnCbnSelchangeComboDesignTool();
	virtual afx_msg void OnBnClickedCheckLoadtype();
	virtual afx_msg void OnBnClickedCheckStrutTie();
	virtual afx_msg void OnBnClickedCheckNodalZone();
	virtual afx_msg void OnBnClickedCheckStressFlow();

	virtual afx_msg LRESULT OnCalcOk(WPARAM, LPARAM);
	virtual afx_msg LRESULT OnCalcOkStressFlow(WPARAM, LPARAM);
	virtual afx_msg LRESULT OnCalcOkStad2D(WPARAM, LPARAM);

	afx_msg void OnBnClickedCheckAllcombo();
	afx_msg void OnBnClickedCheckAllbridge();
	afx_msg void OnBnClickedCheckAllModel();

	DECLARE_MESSAGE_MAP()

	virtual void DrawInputDomyunView(BOOL bZoomAll=TRUE);

protected:
	CXTPComboBox	m_cbBridgeNo;
	CXTPComboBox	m_ComboLoadCase;
	CXTPComboBox	m_ComboModel;
	CXTPComboBox	m_ComboCase;
	HGINT32 m_nCurModel;
	HGINT32 m_nCurCase;
	CComboBox m_cbDesignTool;
	HGBOOL m_bCheckAllBridge;
	HGBOOL m_bCheckAllModel;
	HGBOOL m_bCheckAllCombo;
	CButton m_btRunSolver;
	CButton m_btInputSap;
	CButton m_btOunputSap;
	CButton m_btRunStressFlow;
	HGBOOL m_bCheckShowLoad;
	HGBOOL m_bCheckShowStrutTie;
	HGBOOL m_bCheckShowNodalZone;
	HGBOOL m_bCheckShowStressFlow;

	CInputCheckGrid *m_pDlgCheck;

	CStmUbRenderer *m_pUbRenderer;
	CStmModel *m_pModel;
	CStmModel *m_pStress;

	HGBOOL m_bEnableStmControl;
	HGBOOL m_bRunSapGeneralModel;		// Frame 해석을 실행하는지(STM이 아닌).
	HGBOOL m_bEndCalcOK;
	HGBOOL m_bRunStad2DForSap2000;

	std::vector<HGINT32> m_vKeyModel;
	std::vector<HGINT32> m_vKeyCase;

	HGINT32 m_ixStmKey;

	void SetSelectModel();
	virtual void InitModelCombo(HGINT32 iDepth=0) = 0;
	virtual BOOL InitStmModel(CStmModel *pModel);
	virtual void MakeStmStressFlowData(CStmModel *pStm, HGINT32 nBri, HGINT32 ixKey) = 0;
	virtual void MakeStmTrussData(HGINT32 nBri, HGINT32 ixKey, BOOL bSetLoadOnly=FALSE) = 0;

	BOOL SetCurrentStmModel();
	void SetCurModelKey();
	void SetSelected2DFrameTool(long nSolverTool);
	long GetSelected2DFrameTool();

private:
	CCommandResultDlg m_CommandDlg;

	void MakeStmInputData(HGINT32 ixBridge, HGINT32 ixDesignType, BOOL bRenumberModel, BOOL bRemakeModel);
	void MakeStressFlowInputData(CString szPath);
	void MakeInputStad2D();
	void ReadOututStad2D();
	void DeleteAndCopyOutput(CString szExt);
	void SetStmAreaByInput(CStmModel *pStm);

	void ClearFile(CString szFileName);

	void GetModelKeysForAnalysis(HGINT32 nBri, std::vector<HGINT32> &vKeys);
	void GetModelKeysForAnalysis_Lsd(HGINT32 nBri, std::vector<HGINT32> &vKeys);
};


