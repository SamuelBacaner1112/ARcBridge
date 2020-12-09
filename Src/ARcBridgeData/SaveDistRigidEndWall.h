// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// SaveDistRigidEndWall.h: interface for the CSaveDistRigidEndWall class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SAVEDISTRIGIDENDWALL_H__086C6947_806D_445F_82FD_35EFAE7E3A22__INCLUDED_)
#define AFX_SAVEDISTRIGIDENDWALL_H__086C6947_806D_445F_82FD_35EFAE7E3A22__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSaveDistRigidEndWall : public CObject  
{
public:
	void SetValue(long nJijum, double d, BOOL bUpper);
	void GetValue(double &d);
	BOOL IsMatch(long nJijum, BOOL bUpper);
	// 비교대상 //////
	long m_nJijum;
	BOOL m_bUpper;
	/////////////////
	
	// 리턴대상 ////////////
	double m_dRet;
	///////////////////////
	
	CSaveDistRigidEndWall();
	virtual ~CSaveDistRigidEndWall();

};

#endif // !defined(AFX_SAVEDISTRIGIDENDWALL_H__086C6947_806D_445F_82FD_35EFAE7E3A22__INCLUDED_)
