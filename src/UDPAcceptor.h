/***************************************************************************
 *                                                                         *
 *   Jednoduchý tester end-to-end parametrů sítě                           *
 *                                                                         *
 *   Peter Gazdík, xgazdi03@stud.fit.vutbr.cz                              *
 *                                                                         *
 ***************************************************************************/

#ifndef UDPACCEPTOR_H
#define UDPACCEPTOR_H

#include "UDPStream.h"


/**
 * @brief The UDPAcceptor class Encapsulate socket machanism to accept
 * connections from a clients.
 */
class UDPAcceptor
{
public:
  /**
   * @brief start Create socket descriptor, bind it and ccreate UDPStream object
   * @return Pointer to UDPStream object
   */
  UDPStream *start(int port);
private:
  int m_lsd;
  int m_port;
};

#endif // UDPACCEPTOR_H
