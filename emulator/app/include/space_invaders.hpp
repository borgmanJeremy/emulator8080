#include <SDL.h>
#include <array>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "cpu_8080.hpp"

#ifndef SPACE_INVADERS_HPP_
#define SPACE_INVADERS_HPP_

enum class KeyState
{
  PRESSED,
  NOT_PRESSED
};

struct PlayerInput
{
  PlayerInput();
  bool processKeyboardEvent(SDL_Event &event);
  void updateKeyState();

  std::map<std::string, KeyState> key_state_;
};

void configureMemory(Cpu_8080 &cpu, std::vector<std::string> pathsToFiles);

void appendFromFile(std::string const &file_name,
                    std::vector<uint8_t> &program_data);

void setBit(uint8_t &byte, uint8_t bit_pos);
void clearBit(uint8_t &byte, uint8_t bit_pos);

void updateCPUPorts(Cpu_8080 &cpu,
                    std::map<std::string, KeyState> const &state);
#endif  // SPACE_INVADERS_HPP_