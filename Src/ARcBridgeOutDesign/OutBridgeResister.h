// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// OutBridgeResister.h: interface for the COutBridgeResister class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OUTBRIDGERESISTER_H__8F7F0413_EC2E_4A52_83CE_BC8FD897FAEF__INCLUDED_)
#define AFX_OUTBRIDGERESISTER_H__8F7F0413_EC2E_4A52_83CE_BC8FD897FAEF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CRcBridgeRebar;
class CARcBridgeCalcStd;
class CFootingApp;
class COutBridgeResister  
{
public:
	double GetCalBM(long nBri, long nKind);
	void MakeExcel(CXLControl *pXL, long nBri);
	
	COutBridgeResister();
	virtual ~COutBridgeResister();

	long m_nRowSheet;
	long m_nIndexTitle;
	
	CARcBridgeOutDesignStd *m_pStd;
	CARcBridgeCalcStd	*m_pCalc;
public:
	void MakeExcelResister(CXLFormatText *pXLText, long nBri);				//설계 대장(설계개요)
	void MakeExcelResister2(CXLFormatText *pXLText, long nBri);				//설계 대장(설계개요)
	void MakeExcelBridge(CXLFormatText *pXLText, long nBri);				//설계 대장(교량)
	CString GetStringJigan(long nBri);										//지간구성
	CString GetStringBridgeType(CRcBridgeRebar *pBri);						//교량형식
	CString GetStringFootName(CRcBridgeRebar *pBri, CFootingApp *pFoot);	//기초 Title
	CString GetStringFootTypeAll(CRcBridgeRebar *pBri);
	
	CString GetStringMaxMomentSlabCenter(CRcBridgeRebar *pBri);				//슬래브중앙부에서의 모멘트 검토
	CString GetStringMaxMomentWall(CRcBridgeRebar *pBri, BOOL bUpper);		//벽체 모멘트 검토 bUpper = TRUE: 상단부, FALSE: 중앙부
	CString GetStringMaxMomentFoot(CRcBridgeRebar *pBri);					//기초 모멘트 검토

	CString GetStringMaxAsSlabCenter(CRcBridgeRebar *pBri);				//슬래브중앙부에서의 모멘트 검토
	CString GetStringMaxAsWall(CRcBridgeRebar *pBri, BOOL bUpper);		//벽체 모멘트 검토 bUpper = TRUE: 상단부, FALSE: 중앙부
	CString GetStringMaxAsFoot(CRcBridgeRebar *pBri);					//기초 모멘트 검토

	CString GetStringMaxVuSlabCenter(CRcBridgeRebar *pBri);				//슬래브중앙부에서의 모멘트 검토
	CString GetStringMaxVuWall(CRcBridgeRebar *pBri, BOOL bUpper);		//벽체 모멘트 검토 bUpper = TRUE: 상단부, FALSE: 중앙부
	CString GetStringMaxVuFoot(CRcBridgeRebar *pBri);					//기초 모멘트 검토
};

#endif // !defined(AFX_OUTBRIDGERESISTER_H__8F7F0413_EC2E_4A52_83CE_BC8FD897FAEF__INCLUDED_)
