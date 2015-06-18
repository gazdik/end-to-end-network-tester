/***************************************************************************
 *                                                                         *
 *   Jednoduchý tester end-to-end parametrů sítě                           *
 *                                                                         *
 *   Peter Gazdík, xgazdi03@stud.fit.vutbr.cz                              *
 *                                                                         *
 ***************************************************************************/

#ifndef CLIENT_ARGPARSE_H
#define CLIENT_ARGPARSE_H

#include <boost/program_options.hpp>
#include <string>

namespace po = boost::program_options;

/**
 * @brief The ClientArgparse class Parse client arguments.
 */
class ClientArgparse
{
public:
  ClientArgparse();
  void parse(int argc, char *argv[]);
  int port();
  int size();
  int rate();
  int timeout();
  int interval();
  const std::string &server();
  bool help();
  po::options_description desc;
private:
  po::variables_map vm;
  po::positional_options_description pos;
};

#endif // CLIENT_ARGPARSE_H
