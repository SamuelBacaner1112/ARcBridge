// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__04CD3B28_E28C_4CB4_8FD7_90CF7B16AE7E__INCLUDED_)
#define AFX_MAINFRM_H__04CD3B28_E28C_4CB4_8FD7_90CF7B16AE7E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WM_CREATE_INPUT	WM_USER+2
#define WM_UPDATE_TREE	WM_USER+8
#define WM_UPDATE_INPUT_TITLE	WM_USER+10
#define WM_UPDATE_DRAWING_TITLE	WM_USER+11
#define WM_UPDATE_STATUSBAR		WM_USER+12
#define WM_UPDATE_TOOLBAR_DRAW	WM_USER+13
#define WM_UPDATE_UNITCOMBO		WM_USER+14
#define WM_PAGEINFO					WM_USER+33

#define WM_USER_APPLYDOMYUN		WM_USER+116
#define WM_USER_EDIT_CANCEL		WM_USER+117	// 작업 취소 되었음(ADomManager를 숨겨라)

#define WM_SWITCH_VIEW			WM_USER + 118		// 서로 다른 뷰 전환

#define VIEWID_DEFAULT	AFX_IDW_PANE_FIRST			// 서로 다른 뷰 전환
#define VIEWID_UBICON	AFX_IDW_PANE_FIRST + 100	// 서로 다른 뷰 전환

#define PRJ_TEXT	1
#define ENG_TEXT	2
#define UNIT_TEXT	3
#define DIR_TEXT	4
#define VERSION_TEXT	5

#include "SaveCompleteDlg.h"
#include "ShortcutPaneStart.h"
#include "ShortcutPaneInput.h"
#include "ShortcutPaneDrawing.h"
#include "ShortcutPaneBM.h"
#include "../Src/ARcBridgeInput/ARcBridgeInput.h"
//#include "../../../Engine/FrameEng/Src/HgOccView/SSplitter.h"
//#include "../../Hangil_Frame/src/HgOccView/ShortcutPaneModel.h"

class CARcBridgeView;
class CARcBridgeStmView;
class CHgExportMain;

class CMainFrame : public CXTPFrameWnd, CXTPOffice2007FrameHook
{
/*
	class CSplitterWndEx : public CXTSplitterWnd
	{
	public:
		CSplitterWndEx()
		{
			m_cySplitterGap = 2;
		}

	};
*/
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:
	void CreateShortcutBar();
	void CreateSaveCompleteDlg();
	void InitBMTree();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
public:
	CXTPShortcutBar m_wndShortcutBar;
	CXTPCommandBars* m_pCommandBars;
	CXTPDockingPaneManager m_paneManager;
	CXTPStatusBar m_wndStatusBar;
	CShortcutPaneStart m_paneShortcutStart;
	CShortcutPaneInput m_paneShortcutInput;
	CShortcutPaneDrawing m_paneShortcutDrawing;
	CShortcutPaneBM	m_paneShortcutBM;
	//CShortcutPaneModel m_paneShortcutModel;
	CXTPComboBox m_ComboUnit;
	CFont m_combofont;
	//CFrameWnd *m_pADManage;	
	CARcBridgeView* m_pView;
	CARcBridgeStmView* m_pViewStm;

	BOOL m_bPrintPreview;
	long m_nCurShortcutID;

	BOOL m_bMessageDraw;	// 그리기 관련 메세지가 들어왔을 때 두번 그리는것을 방지

	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CXTCaption m_wndCaption;
	CToolBar m_wndToolBar;
	CSaveCompleteDlg m_dlgSaveComplete;

// Generated message map functions
public:
	CHgExportMain* m_pHgExportMain;
	afx_msg void OnUnitComboChange();
	
private:
	CPageInfoDlg m_PageInfoDlg;

	HGBOOL CreateStatusBar();
	void SetThemeDll();
	void SetCommandBarsTheme(HMODULE hModule=NULL, LPCTSTR lpszINI=NULL);

protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnViewControlpane();
	afx_msg void OnUpdateViewControlpane(CCmdUI* pCmdUI);
	afx_msg void OnViewInputpane();
	afx_msg void OnUpdateViewInputpane(CCmdUI* pCmdUI);
	afx_msg void OnSetCreation();
	afx_msg void OnFileNew();
	afx_msg void OnViewComment();
	afx_msg void OnUpdateViewComment(CCmdUI* pCmdUI);
	afx_msg void OnAppHome();
	afx_msg void OnAppQna();
	afx_msg void OnAppUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnAppHelpPro();
	afx_msg void OnAppHelpPro1();
	afx_msg void OnAppHelpPro2();
	afx_msg void OnAppHelpArcbridge1();
	afx_msg void OnAppHelpArcbridge();
	afx_msg void OnToolHgexport();
	afx_msg void OnUpdateToolHgexport(CCmdUI* pCmdUI);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	afx_msg LRESULT OnShowUpdateDlg(WPARAM wp,LPARAM lp);
	afx_msg void OnCustomize();
	afx_msg LRESULT OnDockingPaneNotify(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnShortcutBarNotify(WPARAM wParam, LPARAM lParam);
	afx_msg void OnInput(UINT nID);
	afx_msg LRESULT OnUpdateInputTitle(WPARAM nMenuID, LPARAM);
	afx_msg LRESULT OnUpdateDrawingTitle(WPARAM nPage,LPARAM);
	afx_msg void OnUpdateInput(CCmdUI* pCmdUI);
	afx_msg LRESULT OnCreateInputPane(UINT nID, LPARAM);
	afx_msg LRESULT OnUpdateTree(WPARAM wParam, LPARAM);
	afx_msg LRESULT OnUpdateStatusBar(WPARAM wParam, LPARAM);
	afx_msg int OnCreateControl(LPCREATECONTROLSTRUCT lpCreateControl);
	afx_msg LRESULT OnUpdateToolbarDraw(WPARAM, LPARAM);	
	afx_msg LRESULT OnUnitComboChangeOnDialog(WPARAM, LPARAM);
	afx_msg LRESULT OnApplyDomyun(WPARAM, LPARAM);
	afx_msg LRESULT OnCheckLine(WPARAM wParam, LPARAM lParam);
	afx_msg void OnCreateKey();
	afx_msg LRESULT OnSwitchView(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPageInfo(WPARAM, LPARAM);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	// INSERT_ATESTCODE
	LRESULT OnAttMainFrm(WPARAM wp, LPARAM lp);
	DECLARE_MESSAGE_MAP()
public:
	UINT		m_nLockOnlyCheck;
	void ResizePane(UINT nID, CSize size, BOOL bHor);
	void SwitchView(HGINT32 idView, HGINT32 idDlg);

	void UpdateLockInfo();
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);	
	virtual BOOL DestroyWindow();

	// INSERT_ATESTCODE
	HGLONG32 attGetCountStru();
	void attSetCheckAllBridge(BOOL bCheck);
	void attSetCheckAllMode(BOOL bCheck);
	void attSetCheckAllCombination(BOOL bCheck);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__04CD3B28_E28C_4CB4_8FD7_90CF7B16AE7E__INCLUDED_)
