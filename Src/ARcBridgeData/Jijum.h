// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

// Jijum.h: interface for the CJijum class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_JIJUM_H__BA9977FD_981D_48E4_B5D3_EAACE519B634__INCLUDED_)
#define AFX_JIJUM_H__BA9977FD_981D_48E4_B5D3_EAACE519B634__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// 2007.02.27 �� �ִ� ������ 30�� ���� �Է� �ް� �� ���� ���⼱ 20�� ������ �Ǿ� �־
// �ִ� ���� 30���� ������. modified by jaeho
// �ø��� üũ �� �� ��� ��.
#define MAX_SHOE_COUNT_RC_OLD 20
#define MAX_SHOE_COUNT_RC_20090716 30
#define MAX_SHOE_COUNT_RC 40

class CShoePos;

class AFX_EXT_CLASS CJijum : public CObject  
{
public:
	void SerializeNew(CArchive &ar);
	CJijum();
	virtual ~CJijum();

	void SetAngJijum(CDPoint vAng);
	BOOL IsFixJijum();

	void Serialize(CArchive& ar);
	CJijum& operator=(const CJijum& Obj);

	long m_nFlag;						// added by jaeho 2007.02.27
	double m_dLengthUgan[2];			// ���� �Ÿ�
	double m_dLengthSlabToShoe[2];		// ������ ���� ��ħ �Ÿ�
	CDPoint m_vAngle;					// ���� ����
	double m_dStaJijum;					// ���� �����̼�

	// ���� ��ħ
	int m_nQtyShoe[2];						// ���� ��ħ ����
	int m_nDirShoe[2][MAX_SHOE_COUNT_RC];		// ���� ��ħ ����
	CString m_strJijumShoe[2];				// ���� ��ħ ���� �̸�
	double m_dDistShoe[2][MAX_SHOE_COUNT_RC];	// �߽� �������� ������ ���� ��ħ ���� �Ÿ�
	double m_dKh[2][MAX_SHOE_COUNT_RC];		// ź����ħ�� ����� ���� ���������
	double m_dKv[2][MAX_SHOE_COUNT_RC];		// ź����ħ�� ����� ���� ���������

	// ���ƿ�
	double m_dWidBlockOut[2];	// nV=0 ����, nV=1 �Ŀ�
	double m_dHeightBlockOut[2];	// nV=0 ����, nV=1 �Ŀ�
};

#endif // !defined(AFX_JIJUM_H__BA9977FD_981D_48E4_B5D3_EAACE519B634__INCLUDED_)
