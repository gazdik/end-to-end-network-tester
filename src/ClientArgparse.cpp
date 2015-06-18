/***************************************************************************
 *                                                                         *
 *   Jednoduchý tester end-to-end parametrů sítě                           *
 *                                                                         *
 *   Peter Gazdík, xgazdi03@stud.fit.vutbr.cz                              *
 *                                                                         *
 ***************************************************************************/

#include "ClientArgparse.h"

#include <exception>
#include <iostream>

using namespace po;
using namespace std;

ClientArgparse::ClientArgparse()
{
  desc.add_options()
      ("help,h", "produce  help message")
      ("port,p", value<unsigned int>()->default_value(5656), "UDP port of server")
      ("size,s", value<unsigned int>()->default_value(100), "size of payload packet")
      ("rate,r", value<unsigned int>()->default_value(10), "rate in packets per second")
      ("timeout,t", value<int>()->default_value(-1), "timeout")
      ("interval,i", value<int>(), "interval of one measure in seconds")
      ("server", value<string>(), "address of server");

  pos.add("server", 1);
}

void ClientArgparse::parse(int argc, char* argv[])
{
  store(command_line_parser(argc, argv).options(desc).positional(pos).run(), vm);
  notify(vm);
}

int ClientArgparse::port()
{
  return vm["port"].as<unsigned int>();
}

int ClientArgparse::size()
{
  if (vm["size"].as<unsigned int>() < 10) {
    throw runtime_error("Size of payload packet is too small. Minimum is 10 bytes.");
  }
  return vm["size"].as<unsigned int>();
}

int ClientArgparse::rate()
{
  return vm["rate"].as<unsigned int>();
}

int ClientArgparse::timeout()
{
  return vm["timeout"].as<int>();
}

int ClientArgparse::interval()
{
  if (vm.count("interval"))
    return vm["interval"].as<int>();
  else
    return 100 / vm["rate"].as<unsigned int>();
}

const std::string& ClientArgparse::server()
{
  if (vm.count("server"))
    return vm["server"].as<string>();
  else throw runtime_error("The option SERVER is required but missing");
}

bool ClientArgparse::help()
{
  if (vm.count("help")) {
    cout << desc << endl;
    return true;
  }
  return false;
}
