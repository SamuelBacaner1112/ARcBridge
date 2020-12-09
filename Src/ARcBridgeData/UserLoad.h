// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#pragma once

#define USER_LOAD_JOINT_V		0
#define USER_LOAD_JOINT_M		1
#define USER_LOAD_ELEMENT_V		2
#define USER_LOAD_ELEMENT_WX	3
#define USER_LOAD_ELEMENT_WY	4
#define USER_LOAD_ELEMENT_WZ	5



class AFX_EXT_CLASS CBridgeUserLoad : public CObject
{
public:
	CBridgeUserLoad();
	virtual ~CBridgeUserLoad();

	CBridgeUserLoad& operator=(const CBridgeUserLoad& Obj);

	long m_nFlag;
	void Serialize (CArchive &ar);

	long	m_nType;
	long	m_nIdxNo;		// "0" base
	double	m_Ux;
	double	m_Uy;
	double	m_Uz;
	double	m_Mx;
	double	m_My;
	double	m_Mz;

	double  m_dDist[2];	// 하중 시작위치,끝위치
	double	m_dLoadW2[2];	// 하중크기, 시작,끝
	double	m_dLoadW3[2];	// 하중크기, 시작,끝
};

class AFX_EXT_CLASS CBridgeUserLoadSet : public CObject
{
public:
	CBridgeUserLoadSet();
	virtual ~CBridgeUserLoadSet();

	CBridgeUserLoadSet& operator=(const CBridgeUserLoadSet& Obj);

	long m_nFlag;
	void Serialize (CArchive &ar);

	BOOL m_bSelect;
	CString m_szTitle;
	CString m_szComent;
	long m_nTypeLoadCase; //하중구분
	CString m_sTypeLoadCase;

	CTypedPtrArray <CObArray, CBridgeUserLoad*> m_pArrUserLoad;	 // 부재하중
};