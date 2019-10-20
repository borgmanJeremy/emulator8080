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

  path_to_file = "/home/jeremy/emulator8080/emulator/resource/cpudiag.bin";

  std::ifstream file(path_to_file, std::ifstream::binary);

  if (!file)
  {
    throw std::runtime_error("file not found");
  }
  constexpr unsigned int buffer_size = 1024;
  std::array<char, buffer_size> buffer;
  std::vector<uint8_t> program_data;

  // Diag program expects to start at 0x100 so fill to 0x100 and jump there

  program_data.resize(0x100);
  std::fill(program_data.begin(), program_data.end(), 0);
  program_data[0x00] = 0xC3;
  program_data[0x01] = 0x00;
  program_data[0x02] = 0x01;

  // Move SP init by 0x100
  program_data[368] = 0x7;

  while (!file.eof())
  {
    file.read(buffer.data(), buffer_size);

    for (unsigned int i = 0; i < file.gcount(); i++)
    {
      program_data.emplace_back(static_cast<uint8_t>(buffer.at(i)));
    }
  }
  file.close();
  // Skip DAA test
  program_data[0x59c] = 0xc3;  // JMP
  program_data[0x59d] = 0xc2;
  program_data[0x59e] = 0x05;

  Cpu_8080 cpu;
  cpu.memory_.pushSegment(
    {"rom", 0x2000, smart_memory::MemoryAttribute::READ_WRITE});
  cpu.memory_.pushSegment(
    {"ram", 0x400, smart_memory::MemoryAttribute::READ_WRITE});
  cpu.memory_.pushSegment(
    {"v_ram", 0x1600, smart_memory::MemoryAttribute::READ_WRITE});

  cpu.memory_.flashReadOnlySegment("rom", program_data);
  while (1)
  {
    if (cpu.memory_[cpu.reg_.pc] == 0xCD)
    {
      if (cpu.reg_.c == 9)
      {
        uint16_t offset = (cpu.reg_.d << 8) | (cpu.reg_.e);

        uint16_t start_byte = offset + 3;
        uint16_t count = 0;
        while (static_cast<char>(cpu.memory_[start_byte + count]) != '$')
        {
          count++;
          printf("%c", static_cast<char>(cpu.memory_[start_byte + count]));
        }
        printf("\n");
      }
    }
    std::cout << cpu.instruction_set_[cpu.memory_[cpu.reg_.pc]].instruction
              << std::endl;
    cpu.instruction_set_[cpu.memory_[cpu.reg_.pc]].exp();
  }

  return 0;
}