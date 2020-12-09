// DialogFce.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "resource.h"
#include "DialogFce.h"
#include "afxdialogex.h"


// CDialogFce 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDialogFce, CDialog)

CDialogFce::CDialogFce(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogFce::IDD, pParent)
{

}

CDialogFce::~CDialogFce()
{
}

void CDialogFce::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialogFce, CDialog)
END_MESSAGE_MAP()


// CDialogFce 메시지 처리기입니다.
