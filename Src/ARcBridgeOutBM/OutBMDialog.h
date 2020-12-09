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
	CString sName;	// tab(page)이름
	HGLONG32 nLevel;	// 레벨
	HGLONG32 nType;		// -1 : 제목만 있는 것, 0 : 산출근거, 1 : 집계표, 2 : 철근집계
	HGBOOL bSelect;
	HGLONG32 nTypeBM;	// 0 : 일반수량, 1 : 공통수량, 2 : 토공수량, 3 : 철근집계  -> 현재 토공인지 아닌지만 구분해서 사용하고 있음

	CTypedPtrArray <CObArray, CBMData*> *pArrBM;
	CTypedPtrArray <CObArray, CBMString*> *pBMString;
	CTypedPtrArray <CObArray, CBMSumString*> *pBMSumString;
	CBMRebarTable *pBMRebarTable;
	CViewBM *pViewBM;
	COutBMBase *pOutBM;

	// 산출근거나 집계표의 계를 참조하는 상위 집계표
	CTypedPtrArray <CObArray, CBMSumString*> *pBMSumStringParent;
	HGLONG32 nIdxOfParent;
	HGLONG32 nIdxBridge;

}structBMTreeInfo;


// COutBMDialog 대화 상자입니다.

class AFX_EXT_CLASS COutBMDialog : public CXTResizeDialog, public CThreadWork
{
	DECLARE_DYNAMIC(COutBMDialog)

public:
	COutBMDialog(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~COutBMDialog();

// 대화 상자 데이터입니다.
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

	// 수량관련 데이터는 여기다 모으자 /////////////////////////////////
	CTypedPtrArray <CObArray, structBMTreeInfo*> m_arrBMTreeInfo;		// 일반수량
	CTypedPtrArray <CObArray, structBMTreeInfo*> m_arrBMTreeInfoCommon;	// 공통수량
	CTypedPtrArray <CObArray, structBMTreeInfo*> m_arrBMTreeInfoTogong;	// 토공수량
	CTypedPtrArray <CObArray, structBMTreeInfo*> m_arrBMTreeInfoRebar;	// 철근수량

	// 구조물별 집계 ////////////////////////
	// 일반수량
	CTypedPtrArray <CObArray, CBMData*> m_pArrBMAll[2];			// 총괄 집계표(계)
	CTypedPtrArray <CObArray, CBMSumString*> m_pArrBMStringAll[2];

	// 토공수량
	CTypedPtrArray <CObArray, CBMData*> m_pArrBMTogong;			// 토공수량 집계표(계)
	CTypedPtrArray <CObArray, CBMSumString*> m_pArrBMStringTogong;

	std::vector<CBMRebarTable*> m_pArrRebarTableAll;		// 철근 집계표 

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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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
