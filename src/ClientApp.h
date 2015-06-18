/***************************************************************************
 *                                                                         *
 *   Jednoduchý tester end-to-end parametrů sítě                           *
 *                                                                         *
 *   Peter Gazdík, xgazdi03@stud.fit.vutbr.cz                              *
 *                                                                         *
 ***************************************************************************/

#ifndef CLIENTAPP_H
#define CLIENTAPP_H

#include "ClientArgparse.h"
#include "UDPConnector.h"

#include <fstream>
#include <iostream>
#include <chrono>
#include <vector>
#include <mutex>

typedef struct msg {
  std::chrono::high_resolution_clock::time_point sendTime;
} Msg;

typedef struct msgrcv {
  std::chrono::high_resolution_clock::time_point sendTime;
  std::chrono::high_resolution_clock::time_point rcvTime;
} MsgRcv;

/**
 * @brief The ClientApp class Client Application, which sends/receives packets
 * to/from client and processes them
 */
class ClientApp
{
public:
  ClientApp(ClientArgparse &args);
  ~ClientApp();
  void run();

private:
  /**
   * @brief sender Send packets to server.
   * @return Number of sended packets.
   */
  int sender();
  /**
   * @brief receiver Receive packets from server.
   */
  void receiver();

  // Timers
  void timerMeasure();
  void timerInterval();

  /**
   * @brief analyser Analyse received packets from server.
   * @param sentPackets Number of sended packets.
   * @param startInterval Time when measure starts.
   */
  void analyser(int sentPackets, std::chrono::system_clock::time_point startInterval);

  // Shared variables for synchronization
  bool syncSender;      /* Synchronize sender thread */

  MsgRcv *msgRcv;     /* Buffer for incoming message */
  Msg* msg;           /* Buffer for outgoing message */
  int msgSize;        /* Size of message */

  std::chrono::microseconds sendingTime; /* Sending limit */
  std::chrono::seconds intervalTime;    /* Interval between measurements */
  std::chrono::seconds measureTime;     /* Time of measure */
  std::vector<MsgRcv> result;  /* Container for incoming messages */

  UDPConnector connector;
  UDPStream* stream;
  ClientArgparse args;

  std::ofstream outfile;     /* Output file */
};


#endif // CLIENTAPP_H
