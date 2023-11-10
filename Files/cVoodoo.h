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

#ifndef __CVOODOO_H__
#define __CVOODOO_H__
#include "includes.h"

#define NO_IRC
//#define NO_ASTRO
//#define NO_P2P // Don't do this, pz warned


char* GetPCInfo();

class cVoodoo : public cSocket
{
public:
	cVoodoo();
	virtual ~cVoodoo();
	
//	void Init();
	bool m_bRunning;
	int isRunning() { return m_bRunning; }

	cLogging	m_cLogging;
#ifndef NO_IRC
	cIRC		m_cIRC;
#endif
	cCommands	m_cCommands;
	cThread		m_cThread;
	cSystem		m_cSystem;

	cString		m_cString;
	
	cSocket		m_cSocket;

	cMagic		m_cMagic;
#ifndef NO_ASTRO
	cAstro		m_cAstro;
#endif
#ifndef NO_P2P
	cServer			m_cServer;
	cServerControl	m_cServerControl;
#endif
	cXORCipher		m_cXORCipher;
///	cConfig     m_cConfig;
};
#endif
