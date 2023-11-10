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
#include "cLogging.h"
#define MAX_LOG 1024
using namespace std;
cLogging::cLogging()
{
#ifdef _DEBUG
	Print("cLog", "[Initializing]");
	//printf("%s", )
#endif
}

cLogging::~cLogging()
{
#ifdef _DEBUG
	Print("cLog", "[Destructor!]");
#endif
}
void cLogging::Init()
{

	ADDMODULE(m_cLogModule, "cLogging", "1.0", "Logging Module");
}

int cLogging::PrintLine(const char* recv, const char* format, ...)
{
	char LogBuf[512];

	va_list va_alist;
	va_start(va_alist, format);
	vsprintf_s(LogBuf, format, va_alist);
	va_end(va_alist);

	Print(recv, LogBuf);
	return 0;
}
int cLogging::Print(const char* Function, const char* strBuf, ...) //change to string
{

	time_t now = time(0);
	struct tm timeinfo;
	time(&now);
	localtime_s(&timeinfo, &now);
	printf("[%.2d:%.2d:%.2d] [Voodoo:Debug] [cLog:%s] %s\r\n", timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec, Function, strBuf);


	return 0;
}