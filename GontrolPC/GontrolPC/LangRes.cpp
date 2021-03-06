#include "stdafx.h"
#include "LangRes.h"


const TCHAR * CLangRes::m_szDllDir = _T("res\\");
const TCHAR * CLangRes::m_szDllPrefix = _T("lang-");


CLangRes * CLangRes::GetInstance()
{
	static CLangRes inst;
	return &inst;
}

CLangRes::CLangRes()
{
	m_langid = DEFAULT_LANGID;
	m_hResourceDll = NULL;

	m_mapLangNameToID.insert(std::make_pair<std::wstring,LANGID>(L"English",LANGID_EN));
	m_mapLangNameToID.insert(std::make_pair<std::wstring,LANGID>(L"中文",LANGID_ZH));
	m_mapLangNameToID.insert(std::make_pair<std::wstring,LANGID>(L"Español",LANGID_ES));
	m_mapLangNameToID.insert(std::make_pair<std::wstring,LANGID>(L"日本語",LANGID_JA));
	m_mapLangNameToID.insert(std::make_pair<std::wstring,LANGID>(L"Française",LANGID_FR));
	m_mapLangNameToID.insert(std::make_pair<std::wstring,LANGID>(L"Deutsch",LANGID_DE));
	m_mapLangNameToID.insert(std::make_pair<std::wstring,LANGID>(L"Россию",LANGID_RU));
	m_mapLangNameToID.insert(std::make_pair<std::wstring,LANGID>(L"Português",LANGID_PT));

	LangSymbol symbol;
	symbol.langid = LANGID_EN;
	symbol.tszSymbol[0] = _T("en");
	symbol.tszSymbol[1] = NULL;
	symbol.tszSymbol[2] = NULL;
	symbol.tszSymbol[3] = NULL;
	wcscpy_s(symbol.wszLangName,_countof(symbol.wszLangName),L"English");
	symbol.wszLangName[_countof(symbol.wszLangName)-1] = NULL;
	s_vSymbols.push_back(symbol);

	symbol.langid = LANGID_ZH;
	symbol.tszSymbol[0] = _T("zh");
	symbol.tszSymbol[1] = _T("sc");
	symbol.tszSymbol[2] = _T("tc");
	symbol.tszSymbol[3] = NULL;
	wcscpy_s(symbol.wszLangName,_countof(symbol.wszLangName),L"中文");
	symbol.wszLangName[_countof(symbol.wszLangName)-1] = NULL;
	s_vSymbols.push_back(symbol);

	symbol.langid = LANGID_ES;
	symbol.tszSymbol[0] = _T("es");
	symbol.tszSymbol[1] = NULL;
	symbol.tszSymbol[2] = NULL;
	symbol.tszSymbol[3] = NULL;
	wcscpy_s(symbol.wszLangName,_countof(symbol.wszLangName),L"Español");
	symbol.wszLangName[_countof(symbol.wszLangName)-1] = NULL;
	s_vSymbols.push_back(symbol);

	symbol.langid = LANGID_JA;
	symbol.tszSymbol[0] = _T("ja");
	symbol.tszSymbol[1] = NULL;
	symbol.tszSymbol[2] = NULL;
	symbol.tszSymbol[3] = NULL;
	wcscpy_s(symbol.wszLangName,_countof(symbol.wszLangName),L"日本語");
	symbol.wszLangName[_countof(symbol.wszLangName)-1] = NULL;
	s_vSymbols.push_back(symbol);

	symbol.langid = LANGID_FR;
	symbol.tszSymbol[0] = _T("fr");
	symbol.tszSymbol[1] = NULL;
	symbol.tszSymbol[2] = NULL;
	symbol.tszSymbol[3] = NULL;
	wcscpy_s(symbol.wszLangName,_countof(symbol.wszLangName),L"Française");
	symbol.wszLangName[_countof(symbol.wszLangName)-1] = NULL;
	s_vSymbols.push_back(symbol);

	symbol.langid = LANGID_DE;
	symbol.tszSymbol[0] = _T("de");
	symbol.tszSymbol[1] = NULL;
	symbol.tszSymbol[2] = NULL;
	symbol.tszSymbol[3] = NULL;
	wcscpy_s(symbol.wszLangName,_countof(symbol.wszLangName),L"Deutsch");
	symbol.wszLangName[_countof(symbol.wszLangName)-1] = NULL;
	s_vSymbols.push_back(symbol);

	symbol.langid = LANGID_RU;
	symbol.tszSymbol[0] = _T("ru");
	symbol.tszSymbol[1] = NULL;
	symbol.tszSymbol[2] = NULL;
	symbol.tszSymbol[3] = NULL;
	wcscpy_s(symbol.wszLangName,_countof(symbol.wszLangName),L"Россию");
	symbol.wszLangName[_countof(symbol.wszLangName)-1] = NULL;
	s_vSymbols.push_back(symbol);

	symbol.langid = LANGID_PT;
	symbol.tszSymbol[0] = _T("pt");
	symbol.tszSymbol[1] = NULL;
	symbol.tszSymbol[2] = NULL;
	symbol.tszSymbol[3] = NULL;
	wcscpy_s(symbol.wszLangName,_countof(symbol.wszLangName),L"Português");
	symbol.wszLangName[_countof(symbol.wszLangName)-1] = NULL;
	s_vSymbols.push_back(symbol);
}

LANGID CLangRes::LangNameToID(const std::wstring & szLangName)
{
	LANGID id = DEFAULT_LANGID;
	std::map<std::wstring,LANGID>::iterator iter;
	if ((iter = m_mapLangNameToID.find(szLangName)) != m_mapLangNameToID.end())
	{
		id = iter->second;
	}
	return id;
}

void CLangRes::SetLang(const std::wstring & szLangName)
{
	LANGID id = LangNameToID(szLangName);
	SetLang(id);
}

int CLangRes::LangIDToIndex(LANGID id)
{
	if (!id)
	{
		id = GetSystemDefaultLangID();
	}
	unsigned int i;
	for (i = 0; i < s_vSymbols.size(); i++)
	{
		if (s_vSymbols[i].langid == id)
		{
			break;
		}
	}
	if (i == s_vSymbols.size())	//not found, search primary lang symbol
	{
		for (i = 0; i < s_vSymbols.size(); i++)
		{
			if ((s_vSymbols[i].langid & 0x3FF) == (id & 0x3FF))
			{
				break;
			}
		}
	}
	if (i == s_vSymbols.size())
	{
		i = 0;
	}
	return i;
}

const wchar_t * CLangRes::LangIDToName(LANGID id)
{
	int i = LangIDToIndex(id);
	return s_vSymbols[i].wszLangName;
}

void CLangRes::SetLang(LANGID id)
{
	TCHAR szPath[MAX_PATH];
	int i;
	if (m_hResourceDll)
	{
	//	::FreeLibrary(m_hResourceDll);
		m_hResourceDll = NULL;
	}
	i = LangIDToIndex(id);
	m_langid = s_vSymbols[i].langid;
	int j = 0;
	while (!m_hResourceDll && j < 4)
	{
		_tcscpy_s(szPath,MAX_PATH,m_szDllDir);
		_tcscat_s(szPath,MAX_PATH,m_szDllPrefix);
		_tcscat_s(szPath,MAX_PATH,s_vSymbols[i].tszSymbol[j]);
		_tcscat_s(szPath,MAX_PATH,_T(".dll"));
		m_hResourceDll = ::LoadLibrary(szPath);
		j++;
	}
	if (!m_hResourceDll && id != DEFAULT_LANGID)
	{
		SetLang(DEFAULT_LANGID);
	}
}

HMODULE CLangRes::GetResourceDll()
{
	return m_hResourceDll;
}

LANGID CLangRes::GetLangID()
{
	return m_langid;
}