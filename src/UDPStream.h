/***************************************************************************
 *                                                                         *
 *   Jednoduchý tester end-to-end parametrů sítě                           *
 *                                                                         *
 *   Peter Gazdík, xgazdi03@stud.fit.vutbr.cz                              *
 *                                                                         *
 ***************************************************************************/

#ifndef UDPSTREAM_H
#define UDPSTREAM_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <string>

/**
 * @brief The UDPStream class Provide network I/O mechanism.
 */
class UDPStream
{
public:
  friend class UDPAcceptor;
  friend class UDPConnector;
  /**
   * @brief send Send UDP datagram
   * @param buffer Data to send
   * @param len Length of data in bytes
   * @return Number of bytes sent
   */
  ssize_t send(const void *buffer, size_t len);
  /**
   * @brief receive Receive UDP datagram
   * @param buffer
   * @param len Size of buffer
   * @return Number of bytes written
   */
  ssize_t receive(void* buffer, size_t len);
  /**
   * @brief getPeerIP Get peer IP address
   * @return pointer to address or NULL peer doesn't exist.
   */
  std::string getPeerIP();
  /**
   * @brief getPeerPort Get peer port number
   * @return Peer port number or -1 if peer doesn't exist.
   */
  int getPeerPort();
  /**
   * @brief closeSocket Close opened socket
   */
  void closeSocket();
  ~UDPStream();
private:
  /**
   * @brief UDPStream construct object and initialize socket descriptor
   * and peer address
   * @param sd Socket descriptor
   * @param addr Peer IP address
   */
  UDPStream(int sd, const struct sockaddr_in *addr, int timeout = 0);
  /**
   * @brief UDPStream construct object and initialize socket descriptor and
   * also clear m_peerIP
   * @param sd
   */
  UDPStream(int sd, int timeout = 0);

  int m_sd;
  int m_timeout;
  struct sockaddr_in m_peerIP;
};

#endif // UDPSTREAM_H
