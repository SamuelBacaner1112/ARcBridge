// AlphaEValueInputDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ARcBridgeInput.h"
#include "AlphaEValueInputDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CAlphaEValueInputDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CAlphaEValueInputDlg, CUserDialog)

CAlphaEValueInputDlg::CAlphaEValueInputDlg(CWnd* pParent /*=NULL*/)
	: CUserDialog(CAlphaEValueInputDlg::IDD, pParent)
{

}

CAlphaEValueInputDlg::~CAlphaEValueInputDlg()
{
}

void CAlphaEValueInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CUserDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAlphaEValueInputDlg, CUserDialog)
END_MESSAGE_MAP()


// CAlphaEValueInputDlg 메시지 처리기입니다.
