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
	void MakeExcelResister(CXLFormatText *pXLText, long nBri);				//���� ����(���谳��)
	void MakeExcelResister2(CXLFormatText *pXLText, long nBri);				//���� ����(���谳��)
	void MakeExcelBridge(CXLFormatText *pXLText, long nBri);				//���� ����(����)
	CString GetStringJigan(long nBri);										//��������
	CString GetStringBridgeType(CRcBridgeRebar *pBri);						//��������
	CString GetStringFootName(CRcBridgeRebar *pBri, CFootingApp *pFoot);	//���� Title
	CString GetStringFootTypeAll(CRcBridgeRebar *pBri);
	
	CString GetStringMaxMomentSlabCenter(CRcBridgeRebar *pBri);				//�������߾Ӻο����� ���Ʈ ����
	CString GetStringMaxMomentWall(CRcBridgeRebar *pBri, BOOL bUpper);		//��ü ���Ʈ ���� bUpper = TRUE: ��ܺ�, FALSE: �߾Ӻ�
	CString GetStringMaxMomentFoot(CRcBridgeRebar *pBri);					//���� ���Ʈ ����

	CString GetStringMaxAsSlabCenter(CRcBridgeRebar *pBri);				//�������߾Ӻο����� ���Ʈ ����
	CString GetStringMaxAsWall(CRcBridgeRebar *pBri, BOOL bUpper);		//��ü ���Ʈ ���� bUpper = TRUE: ��ܺ�, FALSE: �߾Ӻ�
	CString GetStringMaxAsFoot(CRcBridgeRebar *pBri);					//���� ���Ʈ ����

	CString GetStringMaxVuSlabCenter(CRcBridgeRebar *pBri);				//�������߾Ӻο����� ���Ʈ ����
	CString GetStringMaxVuWall(CRcBridgeRebar *pBri, BOOL bUpper);		//��ü ���Ʈ ���� bUpper = TRUE: ��ܺ�, FALSE: �߾Ӻ�
	CString GetStringMaxVuFoot(CRcBridgeRebar *pBri);					//���� ���Ʈ ����
};

#endif // !defined(AFX_OUTBRIDGERESISTER_H__8F7F0413_EC2E_4A52_83CE_BC8FD897FAEF__INCLUDED_)
