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
#include <ws2tcpip.h>


class cSocket 
{
public:
	cSocket();
	virtual ~cSocket();
	// Functions
	bool	Connect(const char* szHost, int iPort);
	void	Init();
	bool	Connect(const unsigned long lHost, int iPort);
	void	Disconnect();
	void    ClearSocket();
	bool	Bind(int iPort);
	bool	Accept(cSocket& sSocket, sockaddr_in* cssin);
	bool	Recv(char* szBuffer, int iBufSize, int* pBytesRead = NULL);
	bool	RecvTO(char* szBuffer, int iBufSize, unsigned long lTimeOut, int* pBytesRead);
	bool	Write(const char* szBuffer, int iBufSize, int* pBytesWritten);
	//IRC p2p
	bool RecvLine(char* szBuffer, int iBufSize);
	bool RecvLineIRC(char* szBuffer, int iBufSize);
	bool	IsConnected();
	bool	GotError();
	int		GetSocket() { return m_cSocket; }
	unsigned short	 m_iPort;
	unsigned long		m_lLastRecv;
	char			 m_szHost[2048];

	void			 operator=(const cSocket& sSock) {
		m_cSocket = sSock.m_cSocket; m_bConnected = sSock.m_bConnected;

	}

	moduload m_sSockModule;
protected:
	int				m_cSocket;
	bool				m_bRunning;
	bool			 m_bPerm;
	bool			 m_bConnected;

};
