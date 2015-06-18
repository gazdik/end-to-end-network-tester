/***************************************************************************
 *                                                                         *
 *   Jednoduchý tester end-to-end parametrů sítě                           *
 *                                                                         *
 *   Peter Gazdík, xgazdi03@stud.fit.vutbr.cz                              *
 *                                                                         *
 ***************************************************************************/

#include "ClientApp.h"

#include <cstdlib>
#include <cerrno>
#include <cstring>
#include <future>
#include <thread>
#include <ctime>
#include <iomanip>
#include <unistd.h>
#include <atomic>
#include <csignal>
#include <cstring>

using namespace std;
using namespace std::chrono;

bool syncMeasure = true;     /* Synchronize main thread */

void sigint_handler(int sig)
{
  syncMeasure = false;
}

ClientApp::ClientApp(ClientArgparse &args) : args {args}
{
  // Allockate memory for buffers and set size:

  /// Buffer for outgoing message
  msgSize = args.size();
  if (msgSize < sizeof(Msg)) {
    throw length_error("Size of header in message is bigger than input size");
  }
  msg = reinterpret_cast<Msg *>(malloc(msgSize));
  memset(msg, 0, msgSize);

  /// Buffer for incoming message
  if (sizeof(MsgRcv) > msgSize)
    msgRcv = reinterpret_cast<MsgRcv *>(malloc(sizeof(MsgRcv)));
  else
    msgRcv = reinterpret_cast<MsgRcv *>(malloc(msgSize));

  // Create stream:
  stream = connector.connect(args.port(), args.server().c_str(), 2);

  // Set sending limit:
  sendingTime = microseconds (1000000/ args.rate());

  // Set interval:
  intervalTime = seconds (args.interval());

  // Set measure time:
  measureTime = seconds (args.timeout());

  // Open output file
  string fileName = "ipkperf-" + stream->getPeerIP() + "-" +
      to_string(args.size()) + "-" + to_string(args.rate());
  outfile.open(fileName, std::ios::app);

  // Define SIGINT handler
  signal(SIGINT, sigint_handler);
}

ClientApp::~ClientApp()
{
  free(msg);
  free(msgRcv);
  delete stream;

  outfile.close();
}

void ClientApp::run()
{
  system_clock::time_point startInterval;

  // Start timeout timer
  syncMeasure = true;
  thread (&ClientApp::timerMeasure, this).detach();

  while(syncMeasure) {
    // Save start time of interval:
    startInterval = system_clock::now();

    // Start interval timer:
    thread (&ClientApp::timerInterval, this).detach();

    // Create sender() thread:
    syncSender = true;
    future<int> tSender = async(launch::async, &ClientApp::sender, this);

    // Create receiver() thread:
    future<void> tReceiver = async(launch::async, &ClientApp::receiver, this);

    // Get number of send packets:
    int sentPackets = tSender.get();

    // Wait for receiver
    tReceiver.wait();

    // Analyze data
    analyser(sentPackets, startInterval);
  }
}

int ClientApp::sender()
{
  // Initialize counter
  unsigned int count = 0;

  // Send until sender_sync is true
  while (syncSender && syncMeasure) {
    // Set time of current message
    msg->sendTime = high_resolution_clock::now();

    // Send message
    int szSent = stream->send(msg, msgSize);

    // Test size of sent bytes:
    if (szSent != msgSize) {
      throw runtime_error("Sent message is incomplet");
    }

#ifdef HARDDEBUG
  cout << "send msg " << order << " : " << szSent << " bytes" << endl;
#endif

    // Increment order
    count++;
    this_thread::sleep_for(sendingTime);
  }

  // Return number of sended packets
  return count;
}

void ClientApp::receiver()
{
  // Initialize container for result
  result.clear();

  // Loop receiving packets:
  while (true) {
    // Receive message
    int szReceived = stream->receive(msgRcv, msgSize);

    // Set time when message arrived:
    msgRcv->rcvTime = high_resolution_clock::now();

    // If socket is closed, break:
    if (szReceived < 0) {
      break;
    }

    // Test size of received message:
    if (szReceived != msgSize) {
      throw runtime_error("Received message has wrong size");
    }

    // Save message into vector:
    result.push_back(*msgRcv);

#ifdef HARDDEBUG
  cout << "received msg " << msgRcv->order << " : " << szReceived << " bytes" << endl;
#endif

  }
}

void ClientApp::timerMeasure()
{
  if (args.timeout() == -1) {
    pause();
  }
  else {
    this_thread::sleep_for(measureTime);
  }
  syncMeasure = false;
}

void ClientApp::timerInterval()
{
  this_thread::sleep_for(intervalTime);
  syncSender = false;
}

void ClientApp::analyser(int sentPackets, system_clock::time_point startInterval)
{
  if (result.empty()) {
    throw runtime_error("No answer from server");
  }

  time_t startInt = system_clock::to_time_t(startInterval);
  char oTime[20];

  // Convert time to string format
  strftime(oTime, 20, "%F-%H:%M:%S", localtime(&startInt));

  // Length of interval
  int oIntLen = args.interval();

  // Number of sent packets
  int oPcktSent = sentPackets;

  // Number of received packets
  int oPcktRecv = result.size();

  // Size of sent data in bytes
  int oBytesSent = oPcktSent * args.size();

  // Size of received data in bytes
  int oBytesRecv = oPcktRecv * args.size();

  // Avarage delay in milliseconds
  float oAvgDelay;
  microseconds tmp {0};
  microseconds sum {0};
  microseconds min {duration_cast<microseconds>(system_clock::duration::max())};
  microseconds pvd {0};
  microseconds maxPvd {0};

  /// Calc avarage time, find min and max
  for (MsgRcv x : result) {
    tmp = duration_cast<microseconds>(x.rcvTime - x.sendTime);
    sum += tmp;
    if (tmp < min)
      min = tmp;
  }

  /// Convert to nanoseconds and than to milliseconds
  oAvgDelay = sum.count() / result.size() / 1000.0;

  // Calc Jitter
  float oJitter;
  for (MsgRcv x : result) {
    tmp  = duration_cast<microseconds>(x.rcvTime - x.sendTime);
    pvd = tmp - min;
    if (pvd > maxPvd) {
      maxPvd = pvd;
    }
  }
  oJitter = maxPvd.count() / 1000.0;

  // Calc out-of-order
  float oOutOfOrder;
  int reordered = 0;
  high_resolution_clock::time_point prev = result[0].sendTime;
  for (MsgRcv x : result) {
    if (x.sendTime < prev) {
      reordered++;
    }
    prev = x.sendTime;
  }

  oOutOfOrder = reordered / oPcktRecv * 100;

  outfile << oTime << "," << oIntLen << "," << oPcktSent << "," <<
             oPcktRecv << "," << oBytesSent << "," << oBytesRecv << "," <<
             fixed <<
             setprecision(3) << oAvgDelay << "," << setprecision(3) <<
             oJitter << "," << setprecision(0) << oOutOfOrder << endl;


#ifdef HARDDEBUG
  cout << "Sent packets: " << sentPackets << endl;
  cout << "Received packets: " << result.size() << endl;
#endif
}
