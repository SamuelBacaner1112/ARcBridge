#pragma once

class CARcBridgeView;

class CARcBridgeStmView :
	public CStmSpecialView
{
	DECLARE_DYNCREATE(CARcBridgeStmView);

public:
	CARcBridgeStmView(void);
	~CARcBridgeStmView(void);

protected:
	virtual CStmUbManager *GetUbManager();
	virtual void SetCustomInitialUpdate();
	virtual void SetCustomStatusBarText(const CString &strText);
	virtual void SetCustomSize();
	virtual void SetCustomLButtonDown();
	virtual void SetCustomUndo(CHgModelEntityObj *pEnt);
	virtual void SetCustomRedo(CHgModelEntityObj *pEnt);
	virtual void SetCustomLock();

public:
	void SetDomyunView(CARcBridgeView *pView);
	void SettingViewByShortcutBar();

private:
	CARcBridgeView *m_pDomyunView;

protected:
	afx_msg void OnDrawingOption();
	afx_msg void OnDrawingExport();
	afx_msg void OnStructOut();
	afx_msg void OnGenOut();
	afx_msg void OnCommonOut();
	afx_msg void OnEarthOut();
	DECLARE_MESSAGE_MAP()

	void OnDrawViewCurrentPage();
};

