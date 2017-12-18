#ifdef LINUX
#include <string.h>
#endif
#include <string>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <net/if.h>
#include <linux/sockios.h>


/** @brief 자신의 IP를 가져온다.
	@return 자신의 Ip
	@param 없음.
*/
std::string getMyIp()
{
	int nFamily = AF_INET;
	int sock;
	int nRet;

	size_t nNIC;
	const size_t nMaxNIC = 256;

	struct ifconf ifc;
	struct ifreq ifr[nMaxNIC];

	struct sockaddr_in* pAddr(NULL);

	sock = socket(nFamily, SOCK_STREAM, 0);
	if(sock == -1) return "";

	ifc.ifc_len = sizeof(ifr);
	ifc.ifc_ifcu.ifcu_req = ifr;

	nRet = ioctl(sock, SIOCGIFCONF, &ifc);
	if(nRet == -1) return "";

	close(sock);

	nNIC = ifc.ifc_len / sizeof(struct ifreq);

	for(size_t i = 0; i < nNIC; i++)
	{
		if(nFamily == ifc.ifc_ifcu.ifcu_req[i].ifr_ifru.ifru_addr.sa_family)
		{
			pAddr = (sockaddr_in*)(&ifc.ifc_ifcu.ifcu_req[i].ifr_ifru.ifru_addr);
			if(ntohl(pAddr->sin_addr.s_addr) != INADDR_LOOPBACK)
			{
				char *_ip;
				_ip = (char*)malloc(sizeof(char)*24);
				memset(_ip, 0, sizeof(char)*24);
				sprintf(_ip, "%s", inet_ntoa(((in_addr)(pAddr->sin_addr))));
				std::string strIp(_ip);

				if(_ip) free(_ip);
				return strIp;
			}
		}
	}

	return "";
}

