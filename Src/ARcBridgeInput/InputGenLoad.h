#pragma once


// CInputGenLoad ��ȭ �����Դϴ�.

class CInputGenLoad : public CInputDialogBase
{
	DECLARE_DYNAMIC(CInputGenLoad)

public:
	CInputGenLoad(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CInputGenLoad();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_GEN_LOAD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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

	void SetGridData_LsdLoadFactor();				//�������ռ���
	void SetGridData_LsdCombo(long nTypeLsdCombo);	//�����Ѱ����, �شܻ�Ȳ�Ѱ����,����Ѱ����
	void SetGridData_LsdModifiedFactor();			//���߼������
	void SetGridData_LsdLoad();						//����
	void SetGridData_LsdUserLoad();					//���������
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
	CTabCtrl m_ctlTabLsdEta;//�Ѱ���� ��Ÿ ��Ʈ��

	BOOL	m_bAutoDefaultCombine;
	HGBOOL m_bShowUseLoad;
	HGBOOL m_bAutoCombinInit;
	HGBOOL m_bEtaCombinEach;
	
	CDPoint m_xyButtonRailLoad; //ö���������� ��ġ
	
	CStringArray m_sArrTabName;

	long m_nIdxUserSel;
	CStringArray m_szArrUserLoad;
	CStringArray m_szArrTypeUserLoad;
	
	long m_nTabSelectLoad;
	long m_nTabCombineUseLoad;
	long m_nTabCombineUltLoad;
	long m_nTabCombineEccLoad;
	long m_nTabCombineEQLoad;

	long m_nTabLsdLoadFactor;		//�������ռ���
	long m_nTabLsdComboLimit;		//�����Ѱ����
	long m_nTabLsdComboExtreme;		//�شܻ�Ȳ�Ѱ����
	long m_nTabLsdComboService;		//����Ѱ����
	long m_nTabLsdComboFatigue;		// �Ƿ��Ѱ����
	long m_nTabLsdModifiedFactor;	//���߼������
	long m_nTabLsdLoad;				//����
	long m_nTabLsdUserLoadNormal;			//���������
	long m_nTabLsdUserLoadEQ;				//���������
	long m_nTabLsdUserLoadEQ2;
	long m_nTabLsdUserLoadEQ3;
	long m_nTabLsdUserLoadEQ4;
};
