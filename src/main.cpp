//#include <iostream>
#include "App/loop.h"

int main() {
//    std::cout << "Hello, World!" << std::endl;
//    return 0;

  App::App app;
  App::Init(app);
  App::Run(app);
  App::Close(app);

  return 0;
}
