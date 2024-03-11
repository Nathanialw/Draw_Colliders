#include <iostream>
#include "src/App/loop.h"

int main() {
  App::App app;
  App::Init(app);
  App::Run(app);
  App::Close(app);

  return 0;
}
