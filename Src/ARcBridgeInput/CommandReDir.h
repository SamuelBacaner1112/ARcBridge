// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

#pragma once


#include "CommandReDir.h"

#define SET_WINDOW_TEXT   1
#define STDIO_WRITE_TEXT  2
#define STDERR_WRITE_TEXT 3

class CCommandResultDlg;
class AFX_EXT_CLASS CCommandReDir : public CRedirectSap
{
public:
	CCommandReDir(CCommandResultDlg *pDlg);
	virtual ~CCommandReDir();
public:
	CCommandResultDlg *m_pDlg;
protected:
	void OnChildWrite(UINT OutputID, HGLPCTSTR lpszOutput);
protected:
	virtual void OnChildStarted(HGLPCTSTR lpszCmdLine);
	virtual void OnChildStdOutWrite(HGLPCTSTR lpszBuffer);
	virtual void OnChildStdErrWrite(HGLPCTSTR lpszBuffer);
	virtual void OnChildTerminate();
};