// MsValueInputDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "ARcBridgeInput.h"
#include "MsValueInputDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CMsValueInputDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CMsValueInputDlg, CUserDialog)

CMsValueInputDlg::CMsValueInputDlg(CWnd* pParent /*=NULL*/)
	: CUserDialog(CMsValueInputDlg::IDD, pParent)
{

}

CMsValueInputDlg::~CMsValueInputDlg()
{
}

void CMsValueInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CUserDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMsValueInputDlg, CUserDialog)
END_MESSAGE_MAP()


// CMsValueInputDlg �޽��� ó�����Դϴ�.
