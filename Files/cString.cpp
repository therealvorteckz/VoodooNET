/*
 *  VoodooNET - a modular multi-threaded program for Windows x86/x64 bit
 *	Coded by VoRTeCKZ - @therealvorteckz
 *  VoRTeCKZ Studios
 *
 *	Copyright (C) 2023 - All Rights Reserved.
 *
 * Permission to ("use, copy, and/or modify") this software for
 * any purpose  with or without fee is hereby granted, provided that the above copyright
 * notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE AND NONINFRINGEMENT OF THIRD PARTY RIGHTS. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN	
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#include "includes.h"
#include "global.h"
#include "md5.h"

// Token cache
token_data	g_tData[128];
bool		g_bInited = false;
mutex		g_mMutex;
void cString::Init()
{
#ifdef _DEBUG
	g_cVoodoo->m_cLogging.Print("cString:Init", "Initializing cString...");
#endif
	ADDMODULE(m_cStringModule, "cString", "0.1", "String Module");



}


cString::cString() {
	if (!g_bInited) {
		for (int i = 0; i < 128; i++) {
			token_data* pData = &g_tData[i]; pData->szDelim = NULL; pData->szString = NULL;
		}
		g_bInited = true;
	}

	m_szString = NULL;
	m_iLength = 0;
	m_szTemp = NULL;
}

cString::cString(const char* szString) {
	if (!g_bInited) {
		for (int i = 0; i < 128; i++) {
			token_data* pData = &g_tData[i]; pData->szDelim = NULL; pData->szString = NULL;
		}
		g_bInited = true;
	}

	m_szString = NULL; m_iLength = 0;
	Assign(szString);
	m_szTemp = NULL;
}

cString::cString(const cString& pString) {
	if (!g_bInited) {
		for (int i = 0; i < 128; i++) {
			token_data* pData = &g_tData[i]; pData->szDelim = NULL; pData->szString = NULL;
		}
		g_bInited = true;
	}

	m_szString = NULL; m_iLength = 0;
	Assign(pString); m_szTemp = NULL;
}

cString::cString(const int iValue) {
	if (!g_bInited) {
		for (int i = 0; i < 128; i++) {
			token_data* pData = &g_tData[i]; pData->szDelim = NULL; pData->szString = NULL;
		}
		g_bInited = true;
	}

	char szTemp[16];
	sprintf_s(szTemp, "%d", iValue);
	m_szString = NULL; m_iLength = 0;
	Assign(szTemp);
	m_szTemp = NULL;
}

cString::~cString() {
	if (m_szString != NULL) { delete[] m_szString; m_szString = NULL; }
	if (m_szTemp != NULL) { delete[] m_szTemp; m_szTemp = NULL; }
	m_iLength = 0;
}

void cString::Assign(const char* szString)
{
	if (!szString) return;

	int iLength = (int)strlen(szString);
	if (m_szString) { delete[] m_szString; m_szString = NULL; }

	m_szString = new char[iLength + 2]; if (!m_szString) return; m_iLength = iLength;
	memset(m_szString, 0, m_iLength + 2);
	strncpy_s(m_szString, iLength + 2, szString, m_iLength);
}

void cString::Assign(const int iValue) { char szTemp[16]; sprintf_s(szTemp, "%d", iValue); Assign(szTemp); }

void cString::Append(const char* szString)
{
	if (!szString || !m_szString) return;
	int iLength = m_iLength + (int)strlen(szString); char* szTemp = new char[iLength + 20];
	if (!szTemp) return;
	m_iLength = iLength;
	strncpy_s(szTemp, sizeof(szTemp), m_szString, iLength + 1);
	if (m_szString) delete[] m_szString;
	m_szString = szTemp;
}

void cString::Append(const cString& pString) { Append(pString.CStr()); }
void cString::Append(const int iValue) { char szTemp[16]; sprintf_s(szTemp, "%d", iValue); Append(szTemp); }

int cString::Compare(const char* szString) const
{
	if (!szString || !m_szString) return 0; if (!szString) return 1;
	else return ::strcmp(m_szString, szString);
}
int cString::Compare(const cString& pString) const { return Compare(pString.CStr()); }
int cString::CompareNoCase(const char* szString) const
{
	if (!szString || !m_szString) return 0; if (!szString) return 1;
	else return _stricmp(m_szString, szString);
}
int cString::CompareNoCase(const cString& pString) const { return Compare(pString.CStr()); }
void cString::Empty() { if (m_szString) delete[] m_szString; m_szString = NULL; }

void cString::Format(const char* szFormat, ...)
{
	if (!szFormat) return;
	va_list va_alist; char formatbuf[1024]; va_start(va_alist, szFormat);
	vsnprintf(formatbuf, sizeof(formatbuf), szFormat, va_alist); va_end(va_alist);
	Assign(formatbuf);
}

int cString::Find(const char cChar) const { return Find(cChar, 0); }
int cString::Find(const char cChar, int iStart) const
{
	//f (m_bIsCryptStr) return Decrypt().Find(cChar, iStart);
	//if (::strchr(m_szString, cChar) == NULL) return 0;
	return (int)((char*)::strchr(m_szString, cChar) - (char*)m_szString) + 1;
	
}
int cString::Find(const cString& pString) const { return Find(pString, 0); }
int cString::Find(const cString& pString, int iStart) const { return Find(pString.CStr(), iStart); }
int cString::Find(const char* szString) const { return Find(szString, 0); }
int cString::Find(const char* szString, int iStart) const
{
	if (!szString) return 0;
	//if (m_bIsCryptStr) return Decrypt().Find(szString, iStart);
	//if (::strstr(m_szString, szString) == NULL) return 0;
	return (int)((char*)::strstr(m_szString, szString) - (char*)m_szString) + 1;
}

char* cString::GetBuffer(int iBufLen)
{
	if (!iBufLen) return NULL;
	m_iLength = iBufLen;
	char* szNewString = new char[iBufLen + 1]; if (!szNewString) return NULL;
	if (m_szString) delete[] m_szString; m_szString = szNewString;
	memset(m_szString, 0, iBufLen + 1); return m_szString;
}

int cString::GetLength() const
{	
	if (!m_szString) return 0;
	return (int)strlen(m_szString);
}

int cString::GetLength()
{
	if (!m_szString) return 0;
	m_iLength = (int)strlen(m_szString);
	return m_iLength;
}
cString cString::Token(int iNum, const char* szDelim, bool bUseQuotes)
{
	static int iCacheIndex = 0; int i; char* szStrCopy;

	if (!szDelim) return cString("");

	try {
		//		if (m_bIsCryptStr) return Decrypt().Token(iNum, szDelim, bUseQuotes);

		szStrCopy = new char[strlen(m_szString) + 2];
		memset(szStrCopy, 0, strlen(m_szString) + 2);
		strncpy_s(szStrCopy, m_iLength + 2, m_szString, strlen(m_szString) + 1);
		//strncpy_s(szStrCopy, m_szString, strlen(m_szString) + 1);
	}
	catch (...) {
		return cString("");
	}

	//! Compute MD5 of input token
	md5::MD5_CTX md5; md5::MD5Init(&md5); unsigned char szMD5[16];
	md5::MD5Update(&md5, (unsigned char*)szStrCopy, (int)strlen(szStrCopy));
	md5::MD5Final(szMD5, &md5);

	g_mMutex.lock();
	for (i = 0; i < 128; i++) {
		token_data* pData = &g_tData[i]; if (!pData->szDelim) continue;

		if (!memcmp(pData->szMD5, szMD5, sizeof(pData->szMD5)) &&
			!memcmp(pData->szDelim, szDelim, (int)strlen(szDelim))) {
			g_mMutex.unlock();

			try {
				if (iNum > pData->vTokens.size()) return cString("");
				return pData->vTokens.at(iNum);
#ifdef _WIN32
			}
			catch (out_of_range) {
#else
			}
			catch (...) {
#endif // _WIN32
				return cString("");
			};

		}
	}

	iCacheIndex++; if (iCacheIndex >= 128) iCacheIndex = 0;
	memcpy(g_tData[iCacheIndex].szMD5, szMD5, sizeof(szMD5));
	g_tData[iCacheIndex].vTokens.clear();

	//! Copy the string

	if (g_tData[iCacheIndex].szString) delete[] g_tData[iCacheIndex].szString;
	if (g_tData[iCacheIndex].szDelim) delete[] g_tData[iCacheIndex].szDelim;
	g_tData[iCacheIndex].szString = szStrCopy;
	g_tData[iCacheIndex].szDelim = new char[strlen(szDelim) + 2];
	memset(g_tData[iCacheIndex].szDelim, 0, strlen(szDelim) + 2);
	strcpy_s(g_tData[iCacheIndex].szDelim, sizeof(g_tData[iCacheIndex].szDelim), szDelim);

	char* szText = g_tData[iCacheIndex].szString;
	bool bInQuotes = false; while (*szText)
	{
		while (*szText == szDelim[0]) szText++;			// skip leading whitespace
		bInQuotes = (*szText == '\"');						// see if this token is quoted
		if (bInQuotes) szText++;							// skip leading quote
		g_tData[iCacheIndex].vTokens.push_back(szText);	// store position of current token
		if (bInQuotes)
		{	// find next quote followed by a space or terminator
			while (*szText && *szText != '\"') szText++;
			if (*szText)
			{
				*szText = '\0'; if (szText[1]) szText += 2;
			}
		}
		else // !bInQuotes
		{	// skip to next non-whitespace/delimiter character
			while (*szText && *szText != szDelim[0]) szText++;
			if (*szText && *szText == szDelim[0])
			{
				*szText = '\0'; szText++;
			}
		}
	} // while(*szText)

	if (iNum >= g_tData[iCacheIndex].vTokens.size()) {
		g_mMutex.unlock(); return cString("");
	}

	g_mMutex.unlock(); return cString(g_tData[iCacheIndex].vTokens.at(iNum));
}

cString cString::Token(int iNum, const char* szDelim) { return Token(iNum, szDelim, false); }


cString::operator const char* () const {
	return CStr();
}
cString::operator char* () {
	return Str();
}


const char* cString::CStr() const
{
	return (const char*)m_szString;
}
const char* cString::CStr()
{
	return (const char*)m_szString;
}
char* cString::Str()
{

	return m_szString;
}
