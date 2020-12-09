// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// ARcBridgeDoc.h : interface of the CARcBridgeDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ARCBRIDGEDOC_H__A301EBD1_71CC_40AC_8DF5_407121B9CE07__INCLUDED_)
#define AFX_ARCBRIDGEDOC_H__A301EBD1_71CC_40AC_8DF5_407121B9CE07__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SaveCompleteDlg.h"
#include "../Src/ARcBridgeData/ARcBridgeData.h"
class CARoadOptionStd;
class CARcBridgeDataStd;
class CARcBridgeInputStd;
class CARcBridgeDrawingStd;
class CARcBridgeDrawingOptionStd;
class CARcBridgeOutBMStd;
class CARcBridgeOutDesignStd;
class CShareWidthADManagerStd;
class CShareBridgeInfoStd;
class CStmUbManager;

class CARcBridgeDoc : public CDocument
{
protected: // create from serialization only
	CARcBridgeDoc();
	DECLARE_DYNCREATE(CARcBridgeDoc)

// Attributes
public:
	static BOOL	m_bFirstDocument;

// Operations
public:
	BOOL m_bFileOpen;	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CARcBridgeDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual void DeleteContents();	
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	void ShowDlgHDanLength();
	CString m_strCurFileOpenPathName;
	BOOL MakeBackup(CString sPathName, BOOL bOpen);
	void Make3DTvArr(CTwinVectorArray &tvArrPlan, CTwinVectorArray &tvArrFront, CTwinVectorArray &tvArr3D);
	void MakeCWTvArr(CTwinVectorArray &tvArr);
	void SerializeStructInfoForATemp(CArchive &ar, CRcBridgeApp *pBri, long jijum);
	void SerializeATemp(CArchive &ar);
	void ExportBridgeInfo();
	BOOL CheckLockSupportBridgeType();		

	CStmUbManager *GetUbManager();

	UINT m_nActiveIDD;
	long m_nCurBriNoOfSettingDomyun;	// 도면환경설정에서 현재 작업중이던 교량 인덱스 보관
	CString m_sFileSaveInfo;
	virtual ~CARcBridgeDoc();

	CARoadOptionStd* m_pARoadOptionStd;
	CARcBridgeDataStd* m_pARcBridgeDataStd;
	CARcBridgeInputStd* m_pARcBridgeInputStd;
	CARcBridgeDrawingStd* m_pARcBridgeDrawingStd;
	CARcBridgeDrawingOptionStd* m_pARcBridgeDrawingOptionStd;
	CARcBridgeOutBMStd* m_pARcBridgeOutBMStd;
	CARcBridgeOutDesignStd* m_pARcBridgeOutDesignStd;

	CShareWidthADManagerStd* m_pShareWithADManagerStd;
	CShareBridgeInfoStd* m_pShareBridgeInfoStd;

public:

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual BOOL DoSave(LPCTSTR pszPathName, BOOL bReplace = TRUE);

// Generated message map functions
protected:
	//{{AFX_MSG(CARcBridgeDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void CreateMember();
	void ClearMember();

	CString GetStringFileSaveInfo();

	CStmUbManager *m_pUbManager;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ARCBRIDGEDOC_H__A301EBD1_71CC_40AC_8DF5_407121B9CE07__INCLUDED_)
