/***************************************************************************
 *                                                                         *
 *   Jednoduchý tester end-to-end parametrů sítě                           *
 *                                                                         *
 *   Peter Gazdík, xgazdi03@stud.fit.vutbr.cz                              *
 *                                                                         *
 ***************************************************************************/

#include <iostream>

#include "ClientApp.h"
#include <exception>

using namespace std;

int main(int argc, char* argv[])
{
  ClientArgparse *args;
  ClientApp *app;

  try {
    args = new ClientArgparse;
    args->parse(argc, argv);
  }
  catch (exception &e) {
    cerr << "Argument ERROR: " << e.what() << endl;
    return 1;
  }

  if (args->help()) {
    return 0;
  }

  try {
    app = new ClientApp {*args};
    app->run();
  }
  catch (exception &e) {
    cerr << "ERROR: " << e.what() << endl;
    return 1;
  }

  delete args;
  delete app;
}
