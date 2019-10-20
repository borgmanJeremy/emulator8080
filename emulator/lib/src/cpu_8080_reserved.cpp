#include "cpu_8080.hpp"

void Cpu_8080::addReservedOperations()
{
  instruction_set_.emplace_back(Instruction{
    8, 0, "-",
    [this]() { throw std::runtime_error("Illegal Instruction! "); }});

  instruction_set_.emplace_back(Instruction{
    0x10, 0, "-",
    [this]() { throw std::runtime_error("Illegal Instruction! "); }});

  instruction_set_.emplace_back(Instruction{
    0x18, 0, "-",
    [this]() { throw std::runtime_error("Illegal Instruction! "); }});

  instruction_set_.emplace_back(Instruction{
    0x20, 0, "-",
    [this]() { throw std::runtime_error("Illegal Instruction! "); }});

  instruction_set_.emplace_back(Instruction{
    0x28, 0, "-",
    [this]() { throw std::runtime_error("Illegal Instruction! "); }});

  instruction_set_.emplace_back(Instruction{
    0x30, 0, "-",
    [this]() { throw std::runtime_error("Illegal Instruction! "); }});

  instruction_set_.emplace_back(Instruction{
    0x38, 0, "-",
    [this]() { throw std::runtime_error("Illegal Instruction! "); }});

  instruction_set_.emplace_back(Instruction{
    0xCB, 0, "-",
    [this]() { throw std::runtime_error("Illegal Instruction! "); }});

  instruction_set_.emplace_back(Instruction{
    0xD9, 0, "-",
    [this]() { throw std::runtime_error("Illegal Instruction! "); }});

  instruction_set_.emplace_back(Instruction{
    0xdd, 0, "-",
    [this]() { throw std::runtime_error("Illegal Instruction! "); }});

  instruction_set_.emplace_back(Instruction{
    0xed, 0, "-",
    [this]() { throw std::runtime_error("Illegal Instruction! "); }});

  instruction_set_.emplace_back(Instruction{
    0xfd, 0, "-",
    [this]() { throw std::runtime_error("Illegal Instruction! "); }});
}