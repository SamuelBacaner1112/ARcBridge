// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// SaveXySlabUpperByLenFrSttStation.h: interface for the CSaveXySlabUpperByLenFrSttStation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SAVEXYSLABUPPERBYLENFRSTTSTATION_H__F2AD70B1_5AA1_47EF_AD51_930F46E3110D__INCLUDED_)
#define AFX_SAVEXYSLABUPPERBYLENFRSTTSTATION_H__F2AD70B1_5AA1_47EF_AD51_930F46E3110D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSaveXySlabUpperByLenFrSttStation : public CObject  
{
public:
	void SetValue(double dDistFromStt, long nLeft, BOOL bVertDir, long nJongDan, CDPoint xy);
	void GetValue(CDPoint &xy);
	BOOL IsMatch(double dDistFromStt, long nLeft, BOOL bVertDir, long nJongDan);
	// 비교대상 //////
	double m_dDistFromStt;
	long m_nLeft;
	BOOL m_bVertDir;
	long m_nJongDan;
	/////////////////
	
	// 리턴대상 ////////////
	CDPoint m_xy;
	///////////////////////

	CSaveXySlabUpperByLenFrSttStation();
	virtual ~CSaveXySlabUpperByLenFrSttStation();

};

#endif // !defined(AFX_SAVEXYSLABUPPERBYLENFRSTTSTATION_H__F2AD70B1_5AA1_47EF_AD51_930F46E3110D__INCLUDED_)
