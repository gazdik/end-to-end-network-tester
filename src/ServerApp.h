/***************************************************************************
 *                                                                         *
 *   Jednoduchý tester end-to-end parametrů sítě                           *
 *                                                                         *
 *   Peter Gazdík, xgazdi03@stud.fit.vutbr.cz                              *
 *                                                                         *
 ***************************************************************************/

#ifndef SERVERAPP_H
#define SERVERAPP_H

#include "ServerArgparse.h"
#include "UDPAcceptor.h"

/**
 * @brief The ServerApp class Receive and send packets back to clients.
 */
class ServerApp
{
public:
  ServerApp();
  ~ServerApp();
  void run(int argc, char* argv[]);
private:
  void echo();

  ServerArgparse args;
  UDPAcceptor acceptor;

  char *buffer;
  const int BUFFSIZE = 65527;
};

#endif // SERVERAPP_H
