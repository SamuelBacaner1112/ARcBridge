#pragma once

class CDlgProgress;
class CHgBaseBMStd;
class CBMString;
class CBMSumString;
class CRebarBMString;

class CViewBM;
class COutBMBase;
class CBMRebarTable;

typedef struct BMTREE_INFO : public CObject
{
	CString sName;	// tab(page)�̸�
	HGLONG32 nLevel;	// ����
	HGLONG32 nType;		// -1 : ���� �ִ� ��, 0 : ����ٰ�, 1 : ����ǥ, 2 : ö������
	HGBOOL bSelect;
	HGLONG32 nTypeBM;	// 0 : �Ϲݼ���, 1 : �������, 2 : �������, 3 : ö������  -> ���� ������� �ƴ����� �����ؼ� ����ϰ� ����

	CTypedPtrArray <CObArray, CBMData*> *pArrBM;
	CTypedPtrArray <CObArray, CBMString*> *pBMString;
	CTypedPtrArray <CObArray, CBMSumString*> *pBMSumString;
	CBMRebarTable *pBMRebarTable;
	CViewBM *pViewBM;
	COutBMBase *pOutBM;

	// ����ٰų� ����ǥ�� �踦 �����ϴ� ���� ����ǥ
	CTypedPtrArray <CObArray, CBMSumString*> *pBMSumStringParent;
	HGLONG32 nIdxOfParent;
	HGLONG32 nIdxBridge;

}structBMTreeInfo;


// COutBMDialog ��ȭ �����Դϴ�.

class AFX_EXT_CLASS COutBMDialog : public CXTResizeDialog, public CThreadWork
{
	DECLARE_DYNAMIC(COutBMDialog)

public:
	COutBMDialog(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~COutBMDialog();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_OUTBM };

	CStatic	m_staticPrintBM;
	CXTPComboBox	m_comboPreview;
	CProgressCtrl	m_ctlProgressPrintBM;
	CButton	m_btnWmf;
	CButton	m_btnUserBM;
	CButton	m_btnPrintSelect;
	CButton	m_btnOption;
	CButton	m_btnKindSelect;
	CButton	m_btnCancel;
	int		m_nRadioViewVert;
	CGridCtrlMng	m_Grid;
	CGridCtrlMng	m_GridOther;


	CHgBaseBMStd *m_pBaseBM;
	CARcBridgeOutBMStd *m_pStd;
	CTreeCtrl *m_pTreeCtrl;

	long m_nKindBM;
	BOOL m_bPrintingExcel;
	BOOL m_bCancelPrint;
	BOOL m_bEnableOpen;
	BOOL m_bFocusDlg;
	BOOL m_bFocusOther;
	CDlgProgress* m_pDlgProgress;
	structBMTreeInfo* m_pBMTreeInfoOther;
	structBMTreeInfo* m_pBMTreeInfoCur;
	long m_nCountGrid;
	BOOL m_bReadyOutBMDlg;

	// �������� �����ʹ� ����� ������ /////////////////////////////////
	CTypedPtrArray <CObArray, structBMTreeInfo*> m_arrBMTreeInfo;		// �Ϲݼ���
	CTypedPtrArray <CObArray, structBMTreeInfo*> m_arrBMTreeInfoCommon;	// �������
	CTypedPtrArray <CObArray, structBMTreeInfo*> m_arrBMTreeInfoTogong;	// �������
	CTypedPtrArray <CObArray, structBMTreeInfo*> m_arrBMTreeInfoRebar;	// ö�ټ���

	// �������� ���� ////////////////////////
	// �Ϲݼ���
	CTypedPtrArray <CObArray, CBMData*> m_pArrBMAll[2];			// �Ѱ� ����ǥ(��)
	CTypedPtrArray <CObArray, CBMSumString*> m_pArrBMStringAll[2];

	// �������
	CTypedPtrArray <CObArray, CBMData*> m_pArrBMTogong;			// ������� ����ǥ(��)
	CTypedPtrArray <CObArray, CBMSumString*> m_pArrBMStringTogong;

	std::vector<CBMRebarTable*> m_pArrRebarTableAll;		// ö�� ����ǥ 

	HTREEITEM AddItem(CTreeCtrl *pTreeCtrl, structBMTreeInfo *pBMTreeInfo, BOOL bOnlyInit=FALSE);
	void InitTree(CTreeCtrl *pTreeCtrl, long nKindBM);
	void InitBMTreeInfo(BOOL bErase=FALSE);
	void InitBMTreeInfoByKind(long nKindBM);
	void InitBMTreeInfoByKindTogong();
	void EraseBMTreeInfoNonData(CTypedPtrArray <CObArray, structBMTreeInfo*> *pArrBMTreeInfo);
	void InsertUserBmDataTreeinfo(long nType);
	void PrintToExcel(CTypedPtrArray <CObArray, structBMTreeInfo*> *pArrBMTreeInfo);
	void PrintSumTitleToExcel(CXLControl *pXL, structBMTreeInfo *pBMTreeInfo);
	void MakeExcelCoverPage(CString sSheetName, CRcBridgeRebar *pBri);
	void SetProgress(BOOL bInit, long nCountTotal=0);
	void SetCheck(long nType, long nKindBM);
	void SetCheck(CTreeCtrl *pTreeCtrl, HTREEITEM hItem, BOOL bCheck);

	void SetDataInit(void *p, BOOL bOtherGrid);
	void ClearBM();
	BOOL CreateBM(BOOL bOnlyReplaceString=FALSE, BOOL bNewCreateBM = TRUE);
	void MakeBMRebarTable();
	void MakeBMStringStd();
	void MergeBMStd();
	UINT DoWork();
	void GetStringSumTitle(CStringArray &sArrTitle, structBMTreeInfo *pBMTreeInfo);
	void GetTreeInfoArray(std::vector<structPageIndexInfo> &vArrTreeInfo, long nType);
	void InitControl();
	void SetControl(BOOL bStartPrint);
	void SetControl();
	void SettingGrid();
	void SetTreeCtrl(CTreeCtrl *pTreeCtrl);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboPreview();
	afx_msg void OnRadioViewHor();
	afx_msg void OnRadioViewVert();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnButtonOption();
	afx_msg void OnButtonWmf();
	afx_msg void OnButtonPrintCancel();
	afx_msg void OnButtonPrintSelect();
	afx_msg void OnButtonKindSelect();
	afx_msg void OnButtonUserBmInput();
	afx_msg void OnButtonUserBmApply();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);

	DECLARE_MESSAGE_MAP()
};
