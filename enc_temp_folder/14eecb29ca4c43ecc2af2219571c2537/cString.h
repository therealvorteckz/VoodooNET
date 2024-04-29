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

#pragma once
#ifndef __cString_H__
#define __cString_H__
#include <vector>


class cString
{
public:
	cString();
	cString(const char* szString);
	cString(const cString& pString);
	cString(const int iValue);
	void Init();
	virtual	~cString();

	void	 Assign(const char* szString);
	//void	 Assign(const cString& pString);
	void	 Assign(const int iValue);

	void	 Append(const char* szString);
	void	 Append(const cString& pString);
	void	 Append(const int iValue);

	int		 Compare(const char* szString) const;
	int		 Compare(const cString& pString) const;

	int		 CompareNoCase(const char* szString) const;
	int		 CompareNoCase(const cString& pString) const;


	int		 Find(const char cChar) const;
	int		 Find(const char cChar, int iStart) const;
	int		 Find(const cString& pString) const;
	int		 Find(const cString& pString, int iStart) const;
	int		 Find(const char* szString) const;
	int		 Find(const char* szString, int iStart) const;


	//cString	 Decrypt() const;
	//cString	 Encrypt(const char* szKey) const;

	void	 Empty();
	

	void	 Format(const char* szFormat, ...);

	char* GetBuffer(int iBufLen);

	int		 GetLength() const;
	int		 GetLength();
	/*
	cString	 Mid(int iFirst, int iCount) const;
	cString	 Mid(int iFirst) const;
	*/
	cString	Token(int iNum, const char* szDelim, bool bUseQuotes);
	cString	Token(int iNum, const char* szDelim);
	
	void operator=(const cString& sStr);
	void operator=(const char* szStr);
	char& operator[](int iPos);
	const char& operator[](int iPos) const;
	operator const char* () const;
	operator char* ();

	const char* CStr() const;
	const char* CStr();
	char* Str();
	
	char*		 m_szString;
	char*		 m_szTemp;
	int			 m_iLength;


	moduload	m_cStringModule;

};

typedef struct token_data_s {
	char* szString;
	char* szDelim;
	std::vector<char*>	 vTokens;
	unsigned char		 szMD5[16];
} token_data;

#endif