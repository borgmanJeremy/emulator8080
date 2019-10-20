#include <array>
#include <cxxopts.hpp>
#include <exception>
#include <fstream>
#include <iostream>
#include <vector>
#include "cpu_8080.hpp"

int main(int argc, char** argv)
{
  cxxopts::Options options("8080 Disassembler", "Disassembles 8080 byte code.");
  options.add_options()("f,file", "File name", cxxopts::value<std::string>());

  auto arg_results = options.parse(argc, argv);
  auto path_to_file = arg_results["file"].as<std::string>();

  path_to_file = "/home/jeremy/emulator_8080/emulator/resource/invaders.h";

  std::ifstream file(path_to_file, std::ifstream::binary);

  if (!file)
  {  // exception}
  }

  /*  file.seekg(0, file.end);
    unsigned int length = file.tellg();
    file.seekg(0, file.beg);
  */
  constexpr unsigned int buffer_size = 1024;
  std::array<char, buffer_size> buffer;
  std::vector<uint8_t> program_data;
  while (!file.eof())
  {
    file.read(buffer.data(), buffer_size);

    for (unsigned int i = 0; i < file.gcount(); i++)
    {
      program_data.emplace_back(static_cast<uint8_t>(buffer.at(i)));
    }
  }
  file.close();

  Cpu_8080 cpu;
  cpu.memory_.pushSegment(
    {"rom", 0x2000, smart_memory::MemoryAttribute::READ_WRITE});
  cpu.memory_.pushSegment(
    {"ram", 0x400, smart_memory::MemoryAttribute::READ_WRITE});
  cpu.memory_.pushSegment(
    {"v_ram", 0x1600, smart_memory::MemoryAttribute::READ_WRITE});

  // cpu.instruction_set[0].exp({0});
  auto it = program_data.begin();
  while (it != program_data.end())
  {
    auto inst = cpu.instruction_set_[(*it)];
    std::cout << it - program_data.begin() << " ";
    std::cout << inst.instruction << " ";
    it++;

    if (inst.num_args == 1)
    {
      std::cout << std::hex << static_cast<int>(*it) << " ";
      it++;
    }
    else if (inst.num_args == 2)
    {
      std::cout << std::hex << static_cast<int>(*(it + 1)) << " "
                << static_cast<int>(*it) << " ";
      it += 2;
    }
    std::cout << std::endl;
  }
}