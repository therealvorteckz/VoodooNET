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
#ifndef __INCLUDES_H__
#define __INCLUDES_H__
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#define MAX_THREADS 250
#define VERSION 01


#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <process.h>
#include <thread>
#include <ctime>
#include <list>
#include <string>
#include <random>

#include <sstream>
#include <iomanip>
#include <wininet.h>
#include "cLogging.h"
#include "cXORCipher.h"
#include "cSystem.h"
#include "cSocket.h"
#include "cString.h"
#include "cThread.h"
#include "cServer.h"
#include "cServerControl.h"

#include "cTestThread.h"
#include "cMagic.h"
#define IRCLINE 200
#define MAX_NICKLEN 16
#define MAX_DATA_SIZE 1024
#define FLOOD_DELAY 10
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

//#define NO_ASTRO
#ifndef NO_ASTRO
#include "cAstro.h"
#endif

#include "externs.h"

//#include "threadstrings.h"

#endif
