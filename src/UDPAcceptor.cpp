/***************************************************************************
 *                                                                         *
 *   Jednoduchý tester end-to-end parametrů sítě                           *
 *                                                                         *
 *   Peter Gazdík, xgazdi03@stud.fit.vutbr.cz                              *
 *                                                                         *
 ***************************************************************************/

#include "UDPAcceptor.h"

#include <netinet/in.h>
#include <string.h>
#include <stdexcept>
#include <cerrno>

using namespace std;

UDPStream *UDPAcceptor::start(int port)
{
  m_port = port;
  struct sockaddr_in servaddr;

  m_lsd = socket(AF_INET, SOCK_DGRAM, 0);
  if(m_lsd < 0) {
    throw runtime_error(strerror(errno));
  }

  memset(&servaddr, 0, sizeof(servaddr));

  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(m_port);

  if (bind(m_lsd, reinterpret_cast<sockaddr *>(&servaddr), sizeof(servaddr))) {
    throw runtime_error(strerror(errno));
  }

  return new UDPStream(m_lsd);
}

