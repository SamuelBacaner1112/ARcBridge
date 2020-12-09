// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#if !defined(AFX_EARTHQUAKEDLG_H__4059B612_E3E1_4C95_A5BE_4291593ABF75__INCLUDED_)
#define AFX_EARTHQUAKEDLG_H__4059B612_E3E1_4C95_A5BE_4291593ABF75__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EarthquakeDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEarthquakeDlg dialog

class CEarthquakeDlg : public CUserDialog
{
public:
	CEarthquakeDlg(CWnd* pParent = NULL);   // standard constructor

	enum { IDD = IDD_DIALOG_EARTHQUACK };
	int		m_nType;
	
	void SetKDS(BOOL bKDS);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	void SetEQText();

	DECLARE_MESSAGE_MAP()

private:
	BOOL m_bKSD;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EARTHQUAKEDLG_H__4059B612_E3E1_4C95_A5BE_4291593ABF75__INCLUDED_)
