// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// SaveOrgInWall.h: interface for the CSaveOrgInWall class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SAVEORGINWALL_H__2449CFB5_9FC0_4C45_B7E9_AF8B34A8C360__INCLUDED_)
#define AFX_SAVEORGINWALL_H__2449CFB5_9FC0_4C45_B7E9_AF8B34A8C360__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSaveOrgInWall : public CObject
{
public:
	void GetValue(CDPoint &xyOrg);
	void SetValue(long nIdxWall, long nLeft, BOOL bVertDir, CDPoint xyOrg);
	BOOL IsMatch(long nIdxWall, long nLeft, BOOL bVertDir);
	CSaveOrgInWall();
	virtual ~CSaveOrgInWall();

	// 비교 대상 ////
	long m_nIdxWall;
	long m_nLeft;
	BOOL m_bVertDir;
	/////////////////

	// 리턴 대상 //
	CDPoint m_xyOrg;
	///////////////

};

#endif // !defined(AFX_SAVEORGINWALL_H__2449CFB5_9FC0_4C45_B7E9_AF8B34A8C360__INCLUDED_)
