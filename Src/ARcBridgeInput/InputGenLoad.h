#pragma once


// CInputGenLoad 대화 상자입니다.

class CInputGenLoad : public CInputDialogBase
{
	DECLARE_DYNAMIC(CInputGenLoad)

public:
	CInputGenLoad(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CInputGenLoad();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_GEN_LOAD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	afx_msg void OnSelchangeComboBridgeNo();
	afx_msg void OnButtonAddCombine();
	afx_msg void OnButtonRemoveCombine();
	afx_msg void OnButtonRemoveUser();
	afx_msg void OnButtonAddUser();
	afx_msg void OnButtonApplySameAll();
	afx_msg void OnButtonApplyMirror();
	afx_msg void OnButtonLsdComboReset();
	afx_msg void OnCheckShowUseLoad();
	afx_msg void OnCheckAutoCombinInit();
	afx_msg void OnCheckEtaCombinEach();
	afx_msg void OnTabSelchange(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTabSelchanging(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTabSelchangeLsdEta(NMHDR *pNMHDR, LRESULT *pResult);

	void OnCellChangedData(NMHDR* pNotifyStruct, LRESULT* result);
	virtual afx_msg void OnCellChanged(NMHDR* pNotifyStruct, LRESULT* result);
	afx_msg void OnCellButtonClick(NMHDR* pNotifyStruct, LRESULT* result);
	afx_msg void OnCellChangedRow(NMHDR* pNotifyStruct, LRESULT* result);

private:
	void DrawInputDomyunView(BOOL bZoomAll=FALSE);
	void DrawModel(CDomyun* pDomP, BOOL bEarthQuake);

	void OnPreInitDialog();
	void InitComboCtrl();
	void InitTabCtrl();
	void SetDataInit();
	void SetDataDefault();
	void SetDataSave();

	void SetGridData();
	void SetGridDataCombineNormal(long nTypeCombine);
	void SetGridDataCombineEQLoad();
	void SetGridDataSelectLoad();
	void SetGridDataSelectLoadUsd();
	void SetGridDataSelectLoadLsd();

	void SetGridData_LsdLoadFactor();				//하중조합선택
	void SetGridData_LsdCombo(long nTypeLsdCombo);	//극한한계상태, 극단상황한계상태,사용한계상태
	void SetGridData_LsdModifiedFactor();			//하중수정계수
	void SetGridData_LsdLoad();						//하중
	void SetGridData_LsdUserLoad();					//사용자하중
	void SetGridData_User(long nLoadType=0);
	void SetGridTitle_UserLoad(long nLoadType=0);

	void MakeSameCombineUltLoad(CRcBridgeApp* pBri1, CRcBridgeApp* pBri2);
	void MakeSameCombineUseLoad(CRcBridgeApp* pBri1, CRcBridgeApp* pBri2);
	void MakeSameCombineEccLoad(CRcBridgeApp* pBri1, CRcBridgeApp* pBri2);
	void MakeSameCombineEQLoad(CRcBridgeApp* pBri1, CRcBridgeApp* pBri2);
	void MakeSameSelectLoad(CRcBridgeApp* pBri1, CRcBridgeApp* pBri2);
	void MakeSameLsdLoad(CRcBridgeApp* pBri1, CRcBridgeApp* pBri2);
	void MakeStringUserLoadLsd();

private:
	CLsdGridCtrl m_Grid;
	CXTPTabCtrl	m_Tab;
	CXTPComboBox	m_cbBridgeNo;

	CButton	m_btnRemoveCombine;
	CButton	m_btnAddCombine;
	CButton	m_btnMirror;
	CButton	m_btnAddUser;
	CButton	m_btnRemoveUser;

	CButton	m_btnLsdComboReset;
	CTabCtrl m_ctlTabLsdEta;//한계상태 에타 컨트롤

	BOOL	m_bAutoDefaultCombine;
	HGBOOL m_bShowUseLoad;
	HGBOOL m_bAutoCombinInit;
	HGBOOL m_bEtaCombinEach;
	
	CDPoint m_xyButtonRailLoad; //철도상재하중 위치
	
	CStringArray m_sArrTabName;

	long m_nIdxUserSel;
	CStringArray m_szArrUserLoad;
	CStringArray m_szArrTypeUserLoad;
	
	long m_nTabSelectLoad;
	long m_nTabCombineUseLoad;
	long m_nTabCombineUltLoad;
	long m_nTabCombineEccLoad;
	long m_nTabCombineEQLoad;

	long m_nTabLsdLoadFactor;		//하중조합선택
	long m_nTabLsdComboLimit;		//극한한계상태
	long m_nTabLsdComboExtreme;		//극단상황한계상태
	long m_nTabLsdComboService;		//사용한계상태
	long m_nTabLsdComboFatigue;		// 피로한계상태
	long m_nTabLsdModifiedFactor;	//하중수정계수
	long m_nTabLsdLoad;				//하중
	long m_nTabLsdUserLoadNormal;			//사용자하중
	long m_nTabLsdUserLoadEQ;				//사용자하중
	long m_nTabLsdUserLoadEQ2;
	long m_nTabLsdUserLoadEQ3;
	long m_nTabLsdUserLoadEQ4;
};
