// DialogFce.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "resource.h"
#include "DialogFce.h"
#include "afxdialogex.h"


// CDialogFce ��ȭ �����Դϴ�.

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


// CDialogFce �޽��� ó�����Դϴ�.
