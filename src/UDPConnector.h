/***************************************************************************
 *                                                                         *
 *   Jednoduchý tester end-to-end parametrů sítě                           *
 *                                                                         *
 *   Peter Gazdík, xgazdi03@stud.fit.vutbr.cz                              *
 *                                                                         *
 ***************************************************************************/

#ifndef UDPCONNECTOR_H
#define UDPCONNECTOR_H

#include "UDPStream.h"
#include <sys/socket.h>
#include <netinet/in.h>

/**
 * @brief The UDPConnector class Encapsulate socket mechanism to
 * connect to a server.
 */
class UDPConnector
{
public:
  UDPStream *connect(int port, const char* server, int timeout = 0);

private:
  void resolveHost(const char* host, struct in_addr *addr);
};

#endif // UDPCONNECTOR_H
