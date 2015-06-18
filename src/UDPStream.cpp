/***************************************************************************
 *                                                                         *
 *   Jednoduchý tester end-to-end parametrů sítě                           *
 *                                                                         *
 *   Peter Gazdík, xgazdi03@stud.fit.vutbr.cz                              *
 *                                                                         *
 ***************************************************************************/

#include "UDPStream.h"

#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <stdexcept>
#include <cerrno>
// DEBUG
#include <iostream>

using namespace std;

ssize_t UDPStream::send(const void *buffer, size_t len)
{
  // Send data
  int n = sendto(m_sd, buffer, len, 0,
                 reinterpret_cast<sockaddr *>(&m_peerIP), sizeof(m_peerIP));

    // If error occurred, throw exception
    if (n < 0) {
      if (errno = EFAULT) {
        throw runtime_error("Message is too large for one UDP packet.");
      } else {
        throw runtime_error(strerror(errno));
      }
    }

  // Returun number of bytes send
  return n;
}

ssize_t UDPStream::receive(void *buffer, size_t len)
{
  // Length of IP address struct
  socklen_t ip_len = sizeof(m_peerIP);

  // Time value for timeout
  struct timeval tv;
  tv.tv_sec = m_timeout;
  tv.tv_usec = 0;

  // Set socket timeout
  setsockopt(m_sd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

  // Receive data
  int n = recvfrom(m_sd, buffer, len, 0,
                   reinterpret_cast<sockaddr *>(&m_peerIP), &ip_len);

  // If error occurred, throw exception
  if (n < 0) {
    if (errno == EWOULDBLOCK) {
      /* Socket timeout expire, do nothing */
    } else {
      throw runtime_error(strerror(errno));
    }
  }

  // Return number of bytes read
  return n;
}

string UDPStream::getPeerIP()
{
  return inet_ntoa(m_peerIP.sin_addr);
}

int UDPStream::getPeerPort()
{
  return ntohs(m_peerIP.sin_port);
}

void UDPStream::closeSocket()
{
  if (m_sd > 0) {
    shutdown(m_sd, SHUT_RDWR);
  }
}

UDPStream::~UDPStream()
{
  if (m_sd > 0) {
    close(m_sd);
  }
}

UDPStream::UDPStream(int sd, const struct sockaddr_in *addr, int timeout)
  : m_sd {sd}, m_timeout {timeout}
{
  m_peerIP = *addr;
}

UDPStream::UDPStream(int sd, int timeout) : m_sd {sd}, m_timeout {timeout}
{
  memset(&m_peerIP, 0, sizeof(m_peerIP));
}
