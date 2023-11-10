
/*
#include "includes.h"
#include <stdio.h>
#include <winsock.h>
using namespace std;

cConfig::cConfig()
{

}

cConfig::~cConfig()
{
}



void cConfig::Init()
{

	//string key = "shadow";
	string data = "looter.net";

	// g_cVoodoo->m_cXORCipher.XORCipherDecrypt(channel_encrypted, key);

	struct addrinfo* result = NULL;
	struct addrinfo* ptr = NULL;
	struct addrinfo hints;

	struct sockaddr_in* sockaddr_ipv4;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	//int hostname = 
	/*
	int iResult = getaddrinfo(g_cVoodoo->m_cXORCipher.XORCipherDecrypt(data, key).c_str(), "6667", &hints, &result);
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {



		sockaddr_ipv4 = (struct sockaddr_in*)ptr->ai_addr;
#ifdef _DEBUG
		g_cVoodoo->m_cLogging.Print("DBG:IPV4Look", "[Hostname: %s IPv4 address(s): %s", g_cVoodoo->m_cXORCipher.XORCipherDecrypt(data, key).c_str(), inet_ntoa(sockaddr_ipv4->sin_addr));
#endif

	}
	
	
}
*/