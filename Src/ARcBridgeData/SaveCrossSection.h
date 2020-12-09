// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// SaveCrossSection.h: interface for the CSaveCrossSection class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SAVECROSSSECTION_H__A2EA307B_4FBB_4C67_81D4_6650BE065A6F__INCLUDED_)
#define AFX_SAVECROSSSECTION_H__A2EA307B_4FBB_4C67_81D4_6650BE065A6F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// 횡단좌표 보관용
class CSaveCrossSection : public CObject
{
public:
	void SetValue(double dSta, CDPoint vAng, BOOL bRoadNotSlab, BOOL bExp, CDPoint *A);
	void GetValue(CDPoint *A);
	BOOL IsMatch(double dSta, CDPoint vAng, BOOL bRoadNotSlab, BOOL bExp);
	// 비교대상 //////
	double m_dSta;
	CDPoint m_vAng;
	BOOL m_bRoadNotSlab;
	BOOL m_bExp;
	/////////////////
	
	// 리턴대상 ////////////
	CDPoint m_A[30];
	///////////////////////

	CSaveCrossSection();
	virtual ~CSaveCrossSection();

};

#endif // !defined(AFX_SAVECROSSSECTION_H__A2EA307B_4FBB_4C67_81D4_6650BE065A6F__INCLUDED_)
