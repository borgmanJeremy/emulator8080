#include <array>
#include <exception>
#include <fstream>
#include <iostream>
#include <vector>
#include "cpu_8080.hpp"

void append_from_file(std::string const &file_name,
                      std::vector<uint8_t> &program_data)
{
  std::ifstream file(file_name, std::ifstream::binary);

  if (!file)
  {
    throw std::runtime_error("file not found");
  }
  constexpr unsigned int buffer_size = 1024;
  std::array<char, buffer_size> buffer;

  while (!file.eof())
  {
    file.read(buffer.data(), buffer_size);

    for (unsigned int i = 0; i < file.gcount(); i++)
    {
      program_data.emplace_back(static_cast<uint8_t>(buffer.at(i)));
    }
  }
  file.close();
}

int main()
{
  std::vector<std::string> paths_to_files = {
    "/home/jeremy/emulator8080/emulator/resource/invaders.h",
    "/home/jeremy/emulator8080/emulator/resource/invaders.g",
    "/home/jeremy/emulator8080/emulator/resource/invaders.f",
    "/home/jeremy/emulator8080/emulator/resource/invaders.e",
  };

  std::vector<uint8_t> program_data;
  for (auto const &path : paths_to_files)
  {
    append_from_file(path, program_data);
  }

  Cpu_8080 cpu;
  cpu.memory_.pushSegment(
    {"rom", 0x2000, smart_memory::MemoryAttribute::READ_ONLY});
  cpu.memory_.pushSegment(
    {"ram", 0x400, smart_memory::MemoryAttribute::READ_WRITE});
  cpu.memory_.pushSegment(
    {"v_ram", 0x1C00, smart_memory::MemoryAttribute::READ_WRITE});
  cpu.memory_.flashReadOnlySegment("rom", program_data);

  while (1)
  {
    std::cout << cpu.instruction_set_[cpu.memory_[cpu.reg_.pc]].instruction
              << std::endl;
    cpu.instruction_set_[cpu.memory_[cpu.reg_.pc]].exp();
  }

  return 0;
}