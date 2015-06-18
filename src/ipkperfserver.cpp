/***************************************************************************
 *                                                                         *
 *   Jednoduchý tester end-to-end parametrů sítě                           *
 *                                                                         *
 *   Peter Gazdík, xgazdi03@stud.fit.vutbr.cz                              *
 *                                                                         *
 ***************************************************************************/

#include <iostream>

#include "ServerApp.h"

using namespace std;

int main(int argc, char* argv[])
{
  ServerApp app;
  try {
    app.run(argc, argv);
  }
  catch (exception &e) {
    cerr << "ERROR: " << e.what() << endl;
  }
}
