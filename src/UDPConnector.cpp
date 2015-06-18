/***************************************************************************
 *                                                                         *
 *   Jednoduchý tester end-to-end parametrů sítě                           *
 *                                                                         *
 *   Peter Gazdík, xgazdi03@stud.fit.vutbr.cz                              *
 *                                                                         *
 ***************************************************************************/

#include "UDPConnector.h"

#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdexcept>
#include <cerrno>
#include <netdb.h>

using namespace std;

UDPStream *UDPConnector::connect(int port, const char *server, int timeout)
{
  int sd;
  struct sockaddr_in servaddr;

  memset(&servaddr, 0, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(port);
  resolveHost(server, &servaddr.sin_addr);

  sd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sd < 0) {
    throw runtime_error(strerror(errno));
  }

  return new UDPStream {sd, &servaddr, timeout};
}

void UDPConnector::resolveHost(const char *host, struct in_addr *addr)
{
  struct addrinfo *res;

  int result = getaddrinfo (host, NULL, NULL, &res);
  if (result != 0) {
    throw runtime_error("resolveHost() failed");
  }
  memcpy(addr, &((struct sockaddr_in *) res->ai_addr)->sin_addr, sizeof(struct in_addr));
  freeaddrinfo(res);
}
