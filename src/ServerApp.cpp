/***************************************************************************
 *                                                                         *
 *   Jednoduchý tester end-to-end parametrů sítě                           *
 *                                                                         *
 *   Peter Gazdík, xgazdi03@stud.fit.vutbr.cz                              *
 *                                                                         *
 ***************************************************************************/

#include "ServerApp.h"

#include <iostream>
#include <thread>
#include <csignal>
#include <iostream>

using namespace std;

UDPStream *stream = nullptr;

void sigintHandler(int sig)
{
  cout << "Server is going down ..." << endl;
  stream->closeSocket();
}

ServerApp::ServerApp()
{
  signal(SIGINT, sigintHandler);

  buffer = new char[BUFFSIZE];
}

ServerApp::~ServerApp()
{
  delete[] buffer;

  if (stream) {
    delete stream;
    stream = nullptr;
  }
}

void ServerApp::run(int argc, char *argv[])
{
  args.parse(argc, argv);

  if (args.help()) {
    return;
  }

  stream = acceptor.start(args.port());

  echo();
}

void ServerApp::echo()
{
  int sz_received, sz_sent;

  while(1) {
    // Receive data from client
    sz_received = stream->receive(buffer, BUFFSIZE);

    if (sz_received <= 0) {
      break;
    }

    // Send data back to client
    sz_sent = stream->send(buffer, sz_received);
  }
}
