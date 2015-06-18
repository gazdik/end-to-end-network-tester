/***************************************************************************
 *                                                                         *
 *   Jednoduchý tester end-to-end parametrů sítě                           *
 *                                                                         *
 *   Peter Gazdík, xgazdi03@stud.fit.vutbr.cz                              *
 *                                                                         *
 ***************************************************************************/

#include "ServerArgparse.h"

#include <exception>
#include <iostream>

using namespace po;
using namespace std;

ServerArgparse::ServerArgparse()
{
  desc.add_options()
      ("help,h", "produce  help message")
      ("port,p", value<unsigned int>()->default_value(5656), "UDP port of server");
}

void ServerArgparse::parse(int argc, char *argv[])
{
  store(parse_command_line(argc, argv, desc), vm);
  notify(vm);
}

int ServerArgparse::port()
{
  return vm["port"].as<unsigned int>();
}

bool ServerArgparse::help()
{
  if (vm.count("help")) {
    cout << desc << endl;
    return true;
  }
  return false;
}
