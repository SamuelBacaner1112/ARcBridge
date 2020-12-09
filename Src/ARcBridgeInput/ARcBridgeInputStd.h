// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// ARcBridgeInputStd.h: interface for the CARcBridgeInputStd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ARCBRIDGEINPUTSTD_H__4B90CE42_0D1B_4D02_9AD9_D8C7E884B778__INCLUDED_)
#define AFX_ARCBRIDGEINPUTSTD_H__4B90CE42_0D1B_4D02_9AD9_D8C7E884B778__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MENU_BASE	 0
#define MENU_GENERAL 1
#define MENU_REBAR	 2
#define MENU_STM 3
#define MENU_COUNT	 4

// 입력 그리드 환경변수
#define GRID_TEXT_BK_COLOR	RGB(253, 250, 255)
#define GRID_TEXT_BK_COLOR_REV	RGB(220, 250, 255)
#define GRID_TEXT_BK_COLOR2	RGB(255, 255, 128)
#define GRID_TEXT_BK_COLOR3	RGB(128, 255, 128)
//#define GRID_ROW_HEIGHT		20

// 입력창 최대,최소 스케일
#define MIN_SCALE		1
#define MAX_SCALE		500
#define GAP_SCALE		20
#define RCBRI_DEFAULT_SCALE		50

#define UK_PAGE_UP		33
#define UK_PAGE_DOWN	34

#include "ARcBridgeInput.h"

typedef struct STRUCT_MOUSE_EVENT
{
	BOOL bViewMouseEvent;
	CDPoint xy;
}structMouseEvent;

class CLineInfoApp;
class CARcBridgeDataStd;
class CARoadOptionStd;
class CDlgAddRateSheet;
class CDialogHDanLength;

class AFX_EXT_CLASS CARcBridgeInputStd  
{
public:
	// 
	UINT GetIDDInputDialogByFlag1(UINT nIDDOld);
	CDialogHDanLength *m_pDlgHDanLength;
	void ShowDlgHDanLength();
	//
	void GridMessage(CGridCtrlMng *pGrid=NULL, CString strMessage=_T(""));
	// 마우스 이벤트용 변수들 /////////////////
	structMouseEvent m_strME;		// 뷰에서 보내는 마우스 이벤트 정보 저장
	///////////////////////////////////////////
	CLineInfoApp* GetLineApp();
	CRcBridgeApp* GetRcBridge();
	CRcBridgeRebar* GetRcBridgeRebar();
	CARcBridgeDataStd *m_pARcBridgeDataStd;
	CARoadOptionStd	*m_pARoadOptionStd;

	CDlgAddRateSheet *m_pDlgAddRateSheet;

	BOOL m_bZoomAllOnInitDialog;	// 처음 뜰때 zoom all을 할지?
	long m_nCurrentBridge;
	BOOL m_bViewAllDir;
	long m_nCurrentSap;
	long m_nLastTab_RebarMain;	// 주철근 입력부에서 마지막으로 선택되어 있는 부위

	CARcBridgeInputStd();
	virtual ~CARcBridgeInputStd();

	void Serialize(CArchive& ar);
	void InsertInputMenu(CXTPMenuBar* pMenuBar);
	void AddDialog(UINT nID, CInputDialogBase* pDlg, long nMenu);
	BOOL IsEnableDialog(UINT nID);
	CInputDialogBase* GetCurrentDialog() { return m_pCurrentDialog; }
	void SetCurrentDialog(CInputDialogBase* pDlg);
	void GoCurrentDlg();
	CInputDialogBase* GetDialogByOrder(BOOL bNext);
	CInputDialogBase* GetDialogByMenuID(UINT nMenuID);
	CInputDialogBase* GetDialogByIDD(UINT nIDD);
	UINT GetSaveMenuIDD() { return m_nSaveMenuID; }
	void SetSaveMenuIDD(UINT nIDD) { m_nSaveMenuID = nIDD; }
	UINT GetMenuID(CInputDialogBase* pDlg);
	UINT GetCurrentMenuID() { return m_pCurrentDialog ? m_pCurrentDialog->GetMenuID() : 0; }
	UINT ShowDialog(CInputDialogBase* pDlg, BOOL bNext);
	BOOL HideDialog(CInputDialogBase* pDlgNext=NULL);
	void MoveNextDialog();
	void MovePrevDialog();
	void SetUsedDialog(CInputDialogBase* pDlg) { pDlg->SetUsedFlag(TRUE); }

	CStmDataConfig *GetStmDataConfig(HGINT32 ixModelKey);

	void SetComment(CString str, BOOL bWarring=FALSE);
	CString GetComment();

	BOOL m_bShowComment;
	CInputDialogPane m_paneInputDialog;

	CDocument* m_pDoc;
	CMenu m_menuInput;
	CWordArray m_IDDArray[MENU_COUNT];

	// 각 입력창 스케일 관련 함수
	void SetScaleInputDialog(double dScale);	// 스케일 설정함수
	double GetScaleInputDialog();				// 스케일 리턴 함수(없으면 기본값 리턴)
	double IncreaseScaleInputDialog(BOOL bBig=FALSE);			// 현재 입력다이얼로그의 스케일을 증기시키고 리턴
	double DecreaseScaleInputDialog(BOOL bBig=FALSE);			// 현재 입력다이얼로그의 스케일을 감소시키고 리턴
//	void SetEnableScaleInputDialog();			// 스케일 조정 활성화
//	void SetDisableScaleInputDialog();			// 스케일 조정 비활성화
	BOOL IsScaleInputDialog();
	CString GetSolveName(long nSolveType);
	CString GetManualPath() const;

	CStmUbRenderer *GetUbRenderer();
	void SetUbRenderer(CStmUbRenderer *pUbRenderer);
	CString GetPathDirectory();

public:
	void SettingNeedPlane();
	BOOL CheckStation();
	BOOL m_bPushMoveButton;
	SYSTEMTIME m_Time;
//	void EndSpeedTest();
//	void StartSpeedTest();
	void SetDialogEnable();
	void CheckZoomAllOnInitDialog();
	long m_nIDDLast;
	long GetLeft();
	void DrawMarginPoint(CDomyun *pDomP, double dRatio=10);
	CString GetStringArray(CStringArray &sArr, long nIdx);
	BOOL IsViewVectorInfo();
	BOOL m_bViewVectorInfo;
	long m_nLeft;

	//////////////////////////////////////////////////////////////////////////
	// SAP2000 Run
	CFEMDraw m_FemDraw;	// FEM Draw - 쉰
	CSapResultDlg m_SapDlg;
	CStringArray m_sSapFileNameArray;
	//void OnDrawColorIndex(CDomyunGLView *pView);

	void MakeInputData(CRcBridgeRebar *pBri, long nDesignType, CString szOpenFileName,long nSolveType=SOLVE_SAP_6AND7, BOOL bKorCode=TRUE);
	CString MakeS2kFile(CRcBridgeRebar *pBri, long nDesignType);
	CString Maket3dFile(CRcBridgeRebar *pBri, long nDesignType);	
	CString GetSap200InstallFolder(CWnd *pParent);
	CString GetNeoMaxInstallFolder(CWnd *pParent);
	void GetDirectoryName(CString &strDirName, const CString& strPathName);	
	BOOL CompleteRunOfNeoMax(CRcBridgeRebar *pBri, long nDesignType);
	BOOL CompleteRunOfSap2000(CRcBridgeRebar *pBri, long nDesignType);

	void RunSap2000(CRcBridgeRebar *pBri, long nDesignType, CDialog *pDlg, BOOL bAutoClose=FALSE);
	void RunSap2000All(CDialog *pDlg, long nDesignType, BOOL bAutoClose=FALSE);	
	void RunNeoMax(CRcBridgeRebar *pBri, long nDesignType, CDialog *pDlg, BOOL bAutoClose=FALSE);
	void RunNeoMaxAll(CDialog *pDlg, long nDesignType, BOOL bAutoClose=FALSE);	
	//////////////////////////////////////////////////////////////////////////

	CInputBaseCrossConstruct	m_dlgCrossConstruct;

private:
	int m_nFlag;
	BOOL m_bFlagScaleInputDialog;				// 스케일 조정 활성/비활성 flag
	long GetIdxInputDialog();				// 현재 입력다이얼로그의 스케일이 저장 되어 있는 index 리턴
	CDPointArray m_xyArrScaleInputDialog;	// 각 입력창의 스케일을 보관한다. x : 입력장 menuID, y : 스케일(없는 경우 기본값 리턴)
	CString m_strHelp;
	UINT m_nSaveMenuID;
	CTypedPtrArray<CObArray, CInputDialogBase*> m_DialogList;
	CInputDialogBase* m_pCurrentDialog;
	
	CInputDialogBase* FindValidDlg(CInputDialogBase* pDlg, BOOL bNext);

	CStmUbRenderer *m_pUbRenderer;
	
public:
//	void MessageGagakLength(CGridCtrlMng *pGrid, long nRow, long nCol);
	void MessageExpAndGagak(BOOL bGrid=FALSE, CGridCtrlMng *pGrid=NULL, long nRow=0, long nCol=0);
	CInputBaseProjectInfo		m_dlgProjectInfo;
	CInputBaseDesignBasicData	m_dlgDesignBasicdata;
	CInputBaseLineInfo			m_dlgLineInfo;
	CInputBaseBridgeFormFactor	m_dlgBridgeFormFactor;
	CInputBaseGroundInfo		m_dlgGroundInfo;
	CInputBaseCrossPosition		m_dlgCrossPosition;
	CInputBaseSpanConstruct		m_dlgSpanConstruct;
	CInputBaseShoePosition		m_dlgShoePosition;
	CInputGenDesignVariable		m_dlgDesignVariable;
	CInputGenJongSection		m_dlgJongSection;
	CInputGenColumn				m_dlgColumn;
	CInputGenWingWall			m_dlgWingWall;
	CInputGenSodan				m_dlgSodan;
	CInputGenTogong				m_dlgTogong;
	CInputGenPlane				m_dlgPlane;
	CInputGenFooting			m_dlgFooting;
// 	CInputGenHyunTa				m_dlgHyunTa;
	CInputGenLoad				m_dlgLoad;
	CInputGenFrameAnalysis		m_dlgFrameAnalysis;
	CInputGenShellAnalysis		m_dlgShellAnalysis;
	CInputGenWingShellAnalysis	m_dlgWingShellAnalysis;
	CInputGenAps				m_dlgAps;
	CInputGenMonument			m_dlgMonument;
	CInputRebarMain				m_dlgRebarMain;
	CInputRebarMain2			m_dlgRebarMain2;
	CInputRebarMain3			m_dlgRebarMain3;
	CInputRebarSection			m_dlgRebarSection;
	CInputRebarGagak			m_dlgRebarGagak;
	CInputRebarColumn			m_dlgRebarColumn;
	CInputRebarWingWall			m_dlgRebarWingWall;
	CInputRebarAps				m_dlgRebarAps;
	CInputRebarOthers			m_dlgRebarOthers;
	CInputRebarJewon			m_dlgRebarJewon;
	CInputStmCondition			m_dlgStmCondition;
	CInputStmArcCorner			m_dlgStmArcCorner;
	CInputStmBracket			m_dlgStmBracket;
	CInputStmFootingStartEnd	m_dlgStmFootingStartEnd;
	CInputStmFootingMid		m_dlgStmFooringMid;
};

#endif // !defined(AFX_ARCBRIDGEINPUTSTD_H__4B90CE42_0D1B_4D02_9AD9_D8C7E884B778__INCLUDED_)
