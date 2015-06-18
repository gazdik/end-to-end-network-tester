/***************************************************************************
 *                                                                         *
 *   Jednoduchý tester end-to-end parametrů sítě                           *
 *                                                                         *
 *   Peter Gazdík, xgazdi03@stud.fit.vutbr.cz                              *
 *                                                                         *
 ***************************************************************************/

#ifndef SERVER_ARGPARSE_H
#define SERVER_ARGPARSE_H

#include <boost/program_options.hpp>
#include <string>

namespace po = boost::program_options;

/**
 * @brief The ServerArgparse class Parse server arguments.
 */
class ServerArgparse
{
public:
  ServerArgparse();
  void parse(int argc, char *argv[]);
  int port();
  bool help();
  po::options_description desc;
private:
  po::variables_map vm;
};

#endif // SERVER_ARGPARSE_H
