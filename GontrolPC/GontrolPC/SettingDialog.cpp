// SettingDialog.cpp : implementation file
//

#include "stdafx.h"
#include "GontrolPC.h"
#include "SettingDialog.h"


// CSettingDialog dialog

IMPLEMENT_DYNAMIC(CSettingDialog, CDialog)

CSettingDialog::CSettingDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CSettingDialog::IDD, pParent)
	, m_szLanguage(_T(""))
{
	pLangRes = CLangRes::GetInstance();
	m_fShown = FALSE;
}

CSettingDialog::~CSettingDialog()
{
}

void CSettingDialog::LoadConfig()
{
	if (m_cfmgr->GetKeyValueInt(L"LangID",&m_langid))
	{
		m_langid = 0;
	}
	pLangRes->SetLang(m_langid);
	
	HMODULE d = pLangRes->GetResourceDll();
	::AfxSetResourceHandle(pLangRes->GetResourceDll());
	m_langid = pLangRes->GetLangID();

	CInputFilter::LoadConfig();
}

void CSettingDialog::LoadConfig(IConfigMgr * cfmgr)
{
	CInputFilter::LoadConfig(cfmgr);
}	

void CSettingDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Slider(pDX, IDC_SLIDERMOVESPEED, m_iMoveSpeed);
	DDX_Control(pDX, IDC_COMBOLANG, m_comboLanguage);
	DDX_CBString(pDX, IDC_COMBOLANG, m_szLanguage);
	if (pDX->m_bSaveAndValidate)
	{
		std::wstring szLanguage = m_szLanguage;
		pLangRes->SetLang(szLanguage);
		if (m_langid != pLangRes->GetLangID())
		{
			CString szLangChange;
			CString szLangChangeTitle;
			szLangChangeTitle.LoadString(pLangRes->GetResourceDll(),IDS_LANGCHANGETITLE);
			szLangChange.LoadString(pLangRes->GetResourceDll(),IDS_LANGCHANGE);
			MessageBox(szLangChange,szLangChangeTitle,MB_OK|MB_ICONQUESTION);
			m_langid = pLangRes->GetLangID();
			m_cfmgr->SetKeyValueInt(L"LangID",m_langid);
			
		}
		SaveConfig();
	}
}


BEGIN_MESSAGE_MAP(CSettingDialog, CDialog)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


BOOL CSettingDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	UpdateData(FALSE);
	m_comboLanguage.AddString(_T("English"));
	m_comboLanguage.AddString(_T("中文"));
	m_comboLanguage.SelectString(-1,pLangRes->LangIDToName(m_langid));
	m_fShown = TRUE;
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSettingDialog::OnOK()
{
	UpdateData(TRUE);
	CDialog::OnOK();
}

BOOL CSettingDialog::IsShown()
{
	return m_fShown;
}
void CSettingDialog::OnDestroy()
{
	CDialog::OnDestroy();
	m_fShown = FALSE;

}
