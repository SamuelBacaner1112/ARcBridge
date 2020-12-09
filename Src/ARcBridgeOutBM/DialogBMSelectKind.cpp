// DialogBMSelectKind.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "resource.h"
#include "arcbridgeoutbm.h"


// CDialogBMSelectKind 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDialogBMSelectKind, CDialog)

CDialogBMSelectKind::CDialogBMSelectKind(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogBMSelectKind::IDD, pParent)
{
	m_pViewBM	= NULL;
}

CDialogBMSelectKind::~CDialogBMSelectKind()
{
}

void CDialogBMSelectKind::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_BUTTON_REV_SELECT, m_btnRevSelect);
	DDX_Control(pDX, IDC_BUTTON_NON_SELECT, m_btnNonSelect);
	DDX_Control(pDX, IDC_BUTTON_ALL_SELECT, m_btnAllSelect);
	DDX_Control(pDX, IDC_LIST_VIEW, m_listView);
}


BEGIN_MESSAGE_MAP(CDialogBMSelectKind, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_ALL_SELECT, OnButtonAllSelect)
	ON_BN_CLICKED(IDC_BUTTON_NON_SELECT, OnButtonNonSelect)
	ON_BN_CLICKED(IDC_BUTTON_REV_SELECT, OnButtonRevSelect)
END_MESSAGE_MAP()


BOOL CDialogBMSelectKind::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	SetWindowText(hggettext("수량산출 공종 선택"));
	GetDlgItem(IDOK)->SetWindowText(hggettext("확인"));
	GetDlgItem(IDCANCEL)->SetWindowText(hggettext("취소"));
	GetDlgItem(IDC_BUTTON_ALL_SELECT)->SetWindowText(hggettext("전체선택"));
	GetDlgItem(IDC_BUTTON_NON_SELECT)->SetWindowText(hggettext("전체해제"));
	GetDlgItem(IDC_BUTTON_REV_SELECT)->SetWindowText(hggettext("반전선택"));
	InitListCtrl();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogBMSelectKind::InitListCtrl()
{
	LV_ITEM lvitem;

	long ExtStyle = 		
		LVS_EX_FULLROWSELECT | 
		LVS_EX_INFOTIP |
		LVS_EX_FLATSB  |
		LVS_EX_CHECKBOXES;
	m_listView.SetExtendedStyle(ExtStyle);

	m_listView.DeleteAllItems();
	m_listView.DeleteColumn(0);

	m_listView.InsertColumn(0,hggettext("출력항목"),LVCFMT_LEFT,250);

	for(int i = 0; i < m_pViewBM->GetBMSize(); i++)
	{
		CString sBigClass	= _T("");
		BOOL bIsView		= FALSE;
		BOOL bIs			= TRUE;
		m_pViewBM->GetBMInfo(i, bIsView, sBigClass, bIs);

		if(1)
		{
			lvitem.mask		= LVIF_TEXT;	
			lvitem.iItem	= i;
			lvitem.iSubItem = 0;
			lvitem.pszText	= sBigClass.GetBuffer(sBigClass.GetLength());
			m_listView.InsertItem(&lvitem);		

			m_listView.SetCheck(i, bIsView);
		}
	}
}

void CDialogBMSelectKind::SetViewBM(CViewBM *pViewBM)
{
	m_pViewBM	= pViewBM;
}

void CDialogBMSelectKind::SetDataSave()
{
	UpdateData(TRUE);

	for(int i = 0; i < m_listView.GetItemCount(); i++)
	{		
		BOOL bIsView = m_listView.GetCheck(i);
		m_pViewBM->SetBMState(i, bIsView);
	}
}

void CDialogBMSelectKind::OnCancel() 
{
	// TODO: Add extra cleanup here

	CDialog::OnCancel();
}

void CDialogBMSelectKind::OnOK() 
{
	// TODO: Add extra validation here
	SetDataSave();
	CDialog::OnOK();
}

void CDialogBMSelectKind::OnButtonAllSelect() 
{
	// TODO: Add your control notification handler code here
	m_pViewBM->SetAllView(TRUE);	
	InitListCtrl();
}

void CDialogBMSelectKind::OnButtonNonSelect() 
{
	// TODO: Add your control notification handler code here
	m_pViewBM->SetAllView(FALSE);
	InitListCtrl();
}

void CDialogBMSelectKind::OnButtonRevSelect() 
{
	// TODO: Add your control notification handler code here
	SetDataSave();
	m_pViewBM->SetReverseView();	
	InitListCtrl();
}
