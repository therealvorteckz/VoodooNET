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

#include "cXORCipher.h"

cXORCipher::cXORCipher()
{
#ifdef _DEBUG
	g_cVoodoo->m_cLogging.Print("TestCIPHER:DBG", "Tesing Constructor");
#endif
}

cXORCipher::~cXORCipher()
{
#ifdef _DEBUG
	g_cVoodoo->m_cLogging.Print("TestCIPHER:DBG", "Testing Destructor");
#endif

}

void cXORCipher::Init()
{
#ifdef _DEBUG
	g_cVoodoo->m_cLogging.Print("XORCipher", "Initializing");
#endif
    ADDMODULE(m_cXORCipher, "cXORCipher", "2.0", "XOR Encryption/Decryption Module");
}

string cXORCipher::XORCipherEncrypt(string msg, string key) {

    string encryptHexa = "";
    int keyItr = 0;
    stringstream res;
    for (int i = 0; i < msg.length(); i++) {
        int temp = msg[i] ^ key[keyItr];

        res << hex << setfill('0') << std::setw(2) << (int)temp;
        keyItr++;
        if (keyItr >= key.length()) {
            keyItr = 0;
        }
    }

    res >> encryptHexa;

    return encryptHexa;
}

string cXORCipher::XORCipherDecrypt(string msg, string key) {
    string hexToUni = "";
    for (int i = 0; i < msg.length() - 1; i += 2) {
        // splitting hex into a pair of two
        string output = msg.substr(i, 2);
        // converting hex into unicode
        int decimal = strtol(output.c_str(), NULL, 16);
        hexToUni += (char)decimal;
    }

    string decrypText = "";
    int keyItr = 0;
    for (int i = 0; i < hexToUni.length(); i++) {
        int temp = hexToUni[i] ^ key[keyItr];

        decrypText += (char)temp;
        keyItr++;
        if (keyItr >= key.length()) {
            // once all of key's letters are used, repeat the key
            keyItr = 0;
        }
    }
    return decrypText;
}

