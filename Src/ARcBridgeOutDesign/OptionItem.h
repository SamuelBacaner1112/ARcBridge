// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// OptionItem.h: interface for the COptionItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OPTIONITEM_H__CCD46EC9_DEDA_4FC9_AD84_CFFF8631E3B9__INCLUDED_)
#define AFX_OPTIONITEM_H__CCD46EC9_DEDA_4FC9_AD84_CFFF8631E3B9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class COptionItem  
{
public:
	BOOL IsEnable(long nIndex);
//	long GetCheckedOptionSize();
	void RemoveAllData();
	long GetOptionSize();
	void SetCheck(long nIndex, BOOL bCheck);
	BOOL IsCheck(CString strItem);
	BOOL IsCheck(long nIndex);
	CString GetItem(long nIndex);
	void AddItem(CString strItem, BOOL bInitCheck = TRUE, BOOL bEnable = TRUE);
	
	COptionItem();
	virtual ~COptionItem();

public:
	CStringArray m_strArrItem;
	CDoubleArray m_dArrItem;
	CDoubleArray m_dArrEnable;

};

#endif // !defined(AFX_OPTIONITEM_H__CCD46EC9_DEDA_4FC9_AD84_CFFF8631E3B9__INCLUDED_)
