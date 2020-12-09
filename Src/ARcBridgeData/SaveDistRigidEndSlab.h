// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// SaveDistRigidEndSlab.h: interface for the CSaveDistRigidEndSlab class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SAVEDISTRIGIDENDSLAB_H__268177E2_DE96_4D67_B82B_72D4309DF990__INCLUDED_)
#define AFX_SAVEDISTRIGIDENDSLAB_H__268177E2_DE96_4D67_B82B_72D4309DF990__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSaveDistRigidEndSlab : public CObject  
{
public:
	void SetValue(long nJijum, BOOL bSttPos, BOOL bUpperSlab, double d);
	void GetValue(double &d);
	BOOL IsMatch(long nJijum, BOOL bSttPos, BOOL bUpperSlab);
	// 비교대상 //////
	long m_nJijum;
	BOOL m_bSttPos;
	BOOL m_bUpperSlab;
	/////////////////
	
	// 리턴대상 ////////////
	double m_dRet;
	///////////////////////
	
	CSaveDistRigidEndSlab();
	virtual ~CSaveDistRigidEndSlab();

};

#endif // !defined(AFX_SAVEDISTRIGIDENDSLAB_H__268177E2_DE96_4D67_B82B_72D4309DF990__INCLUDED_)
