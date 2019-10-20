#include <cxxopts.hpp>
#include <fstream>

int main(int argc, char** argv)
{
  cxxopts::Options options("8080 Disassembler", "Disassembles 8080 byte code.");
  options.add_options()("f,file", "File name", cxxopts::value<std::string>());

  auto path_to_file = options.parse(argc, argv);

  return 0;
}