// Termm--Winter 2021

#include <iostream>
#include "scene_lua.hpp"

int main(int argc, char** argv)
{
  std::string filename = "simple.lua";
  if (argc >= 2) {
    filename = argv[1];
  }

  if (!run_lua(filename)) {
    std::cerr << "Could not open " << filename <<
                 ". Try running the executable from inside of" <<
                 " the Assets/ directory" << std::endl;
    return 1;
  }
}
