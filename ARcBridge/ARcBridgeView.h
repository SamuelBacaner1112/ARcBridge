// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// ARcBridgeView.h : interface of the CARcBridgeView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ARCBRIDGEVIEW_H__F7A5B5A8_11EA_44B9_8F45_9D929053281C__INCLUDED_)
#define AFX_ARCBRIDGEVIEW_H__F7A5B5A8_11EA_44B9_8F45_9D929053281C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WaitForSubProgram.h"
//#include "UbiARcBridgeData.h"
//class CDlgOcc3D;
class CARcBridgeView : public CDomyunGLView
{
protected: // create from serialization only
	CARcBridgeView();
	DECLARE_DYNCREATE(CARcBridgeView)

// Attributes
public:
	CARcBridgeDoc* GetDocument();
	
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CARcBridgeView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrintPreview(CDC* pDC, CPrintInfo* pInfo, POINT point, CPreviewView* pView);
	virtual void CalcWindowRect(LPRECT lpClientRect, UINT nAdjustType = adjustBorder);
	//}}AFX_VIRTUAL

// Implementation
public:
	void SettingViewByShortcutBar();
	BOOL IsView3D();
	BOOL IsViewOutBM();
	LRESULT ApplyDom(WPARAM wParam, LPARAM lParam);
	void InputCheatKey();
	void SetUseToolTip(BOOL bValue) { m_bUseToolTip = bValue; }
	BOOL IsHVMode() { return m_bOnlyVHDraw; }
	void SetHVMode(BOOL bMode) { m_bOnlyVHDraw = bMode; }
	void SetAutoSave(BOOL bStart);

	long m_nLeft;
	CEdit *m_pEdit;
	BOOL m_bViewVectorInfo;
	CWaitForSubProgram m_WaitForALineDlg;
	//CUbiARcBridgeData m_ubARcBridgeData;
	//CDlgOccView *m_pDlgOccView;
	//CDlgOcc3D	*m_pDlgOccViewRendering;

	virtual ~CARcBridgeView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	virtual void OnPostDrawList();

protected:
	CCalcStationDlg m_dlgCalcStation;
	CCalcLineInfoDlg m_dlgCalcLineInfo;
	

// Generated message map functions
protected:	
	CToolTipCtrl m_wndToolTip;
	BOOL m_bTransZoomMove;
	BOOL m_bModeCursor;
	BOOL m_bMoveCursor;
	BOOL m_bEnableCrossCursor;
	BOOL m_bUseToolTip;	
	BOOL m_bOldOwnerMouse;
	HCURSOR m_ArrCur;
	HCURSOR m_ActiveSizeCurUD;
	HCURSOR m_ActiveSizeCurLR;
	HCURSOR m_ActiveSizeCurUDLR;
	BOOL m_bPainted;
	BOOL m_bPush;
	//{{AFX_MSG(CARcBridgeView)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnZoomAll();
	afx_msg void OnZoomDynamic();
	afx_msg void OnZoomIn();
	afx_msg void OnZoomOut();
	afx_msg void OnZoomPrev();
	afx_msg void OnZoomWindow();
	afx_msg void OnRealtimeZoom();
	afx_msg void OnRealtimeMove();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDrawingOption();
	afx_msg void OnDrawingExport();
	afx_msg void OnStructOut();
	afx_msg void OnGenOut();
	afx_msg void OnCommonOut();
	afx_msg void OnEarthOut();
	afx_msg void OnDrawCurrentPage();
	afx_msg void OnDrawPrevPage();
	afx_msg void OnDrawNextPage();
	afx_msg void OnDrawPrevCategory();
	afx_msg void OnDrawNextCategory();
	afx_msg void OnDrawFirstCategory();
	afx_msg void OnDrawLastCategory();
	afx_msg void OnDrawPageInit();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSelectObject();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnMeasureDist();
	afx_msg void OnObjectInfo();
	afx_msg void OnSetLayer();
	afx_msg void OnSetLine();
	afx_msg void OnSetFont();
	afx_msg void OnSetDim();
	afx_msg void OnCalcStation();
	afx_msg void OnCalcLineInfo();
	afx_msg void OnSnapEnd();
	afx_msg void OnUpdateSnapEnd(CCmdUI* pCmdUI);
	afx_msg void OnSnapMiddle();
	afx_msg void OnUpdateSnapMiddle(CCmdUI* pCmdUI);
	afx_msg void OnSnapIntersection();
	afx_msg void OnUpdateSnapIntersection(CCmdUI* pCmdUI);
	afx_msg void OnSnapCenter();
	afx_msg void OnUpdateSnapCenter(CCmdUI* pCmdUI);
	afx_msg void OnSnapPerpen();
	afx_msg void OnUpdateSnapPerpen(CCmdUI* pCmdUI);
	afx_msg void OnSnapNode();
	afx_msg void OnUpdateSnapNode(CCmdUI* pCmdUI);
	afx_msg void OnSnapNear();
	afx_msg void OnUpdateSnapNear(CCmdUI* pCmdUI);
	afx_msg void OnAlignLeft();
	afx_msg void OnAlignCenterHor();
	afx_msg void OnAlignRight();
	afx_msg void OnAlignTop();
	afx_msg void OnAlignCenterVer();
	afx_msg void OnAlignBottom();
	afx_msg void OnAlignSpaceAcross();
	afx_msg void OnAlignSpaceDown();
	afx_msg void OnAlignAttachHor();
	afx_msg void OnAlignAttachVer();
	afx_msg void OnObjectAlign();
	afx_msg void OnAddNote();
	afx_msg void OnUpdateAddNote(CCmdUI* pCmdUI);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnUpdateCalcStation(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCalcLineInfo(CCmdUI* pCmdUI);
	afx_msg void OnExportLine();
	afx_msg void OnCategoryOrder();
	afx_msg void OnAlignUndo();
	afx_msg void OnUpdateAlignUndo(CCmdUI* pCmdUI);
	afx_msg void OnAlignRedo();
	afx_msg void OnUpdateAlignRedo(CCmdUI* pCmdUI);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnBlockCut();
	afx_msg void OnBlockPaste();
	afx_msg void OnBlockCancel();
	afx_msg void OnUpdateBlockCancel(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBlockPaste(CCmdUI* pCmdUI);
	afx_msg void OnBlockCopy();
	afx_msg void OnBlockInitCur();
	afx_msg void OnBlockInitAll();
	afx_msg void OnExportBridgeinfo();
	afx_msg void OnToolHgexport();
	afx_msg void OnUpdateToolHgexport(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg BOOL OnToolbarControl(UINT nID, NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDrawChangePage();
	afx_msg void OnUpdateAll(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAlign(CCmdUI* pCmdUI);
	afx_msg void OnAlignUndoDummy(WPARAM nID);
	afx_msg void OnAlignRedoDummy(WPARAM nID);
	afx_msg void OnApplyDomyun();
	// Stm
	afx_msg void OnExportAStrutTie();
	afx_msg void OnUpdateExportAStrutTie(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()

public:
	static UINT	m_nIDEventLock;
};

#ifndef _DEBUG  // debug version in ARcBridgeView.cpp
inline CARcBridgeDoc* CARcBridgeView::GetDocument()
   { return (CARcBridgeDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ARCBRIDGEVIEW_H__F7A5B5A8_11EA_44B9_8F45_9D929053281C__INCLUDED_)
