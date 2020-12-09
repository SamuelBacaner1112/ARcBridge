// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// SaveXyMatchLineAndSlabUpper.h: interface for the CSaveXyMatchLineAndSlabUpper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SAVEXYMATCHLINEANDSLABUPPER_H__3F13BBCE_56BD_4ACF_B557_EE8F77384D4B__INCLUDED_)
#define AFX_SAVEXYMATCHLINEANDSLABUPPER_H__3F13BBCE_56BD_4ACF_B557_EE8F77384D4B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSaveXyMatchLineAndSlabUpper : public CObject  
{
public:
	void SetValue(CDPoint xy, CDPoint xyDir, long nLeft, double dOffset, BOOL bVertDir, long nJongDan, CDPoint xyRet);
	void GetValue(CDPoint &xy);
	BOOL IsMatch(CDPoint xy, CDPoint xyDir, long nLeft, double dOffset, BOOL bVertDir, long nJongDan);
	// 비교대상 //////
	CDPoint m_xy;
	CDPoint m_xyDir;
	long m_nLeft;
	double m_dOffset;
	BOOL m_bVertDir;
	long m_nJongDan;
	/////////////////
	
	// 리턴대상 ////////////
	CDPoint m_xyRet;
	///////////////////////
	
	CSaveXyMatchLineAndSlabUpper();
	virtual ~CSaveXyMatchLineAndSlabUpper();

};

#endif // !defined(AFX_SAVEXYMATCHLINEANDSLABUPPER_H__3F13BBCE_56BD_4ACF_B557_EE8F77384D4B__INCLUDED_)
