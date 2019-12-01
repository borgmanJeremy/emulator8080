#include <SDL.h>
#include <array>
#include <chrono>
#include <exception>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include "cpu_8080.hpp"
#include "space_invaders.hpp"

int main()
{
  std::vector<std::string> paths_to_files = {
    "/home/jeremy/emulator8080/emulator/resource/invaders.h",
    "/home/jeremy/emulator8080/emulator/resource/invaders.g",
    "/home/jeremy/emulator8080/emulator/resource/invaders.f",
    "/home/jeremy/emulator8080/emulator/resource/invaders.e",
  };

  Cpu_8080 cpu;
  uint16_t shift_register = 0;
  uint16_t shift_register_offset = 0;
  configureMemory(cpu, paths_to_files);
  /*
    for (unsigned long i = 0; i < 9228; i++)
    {
      cpu.instruction_set_[cpu.memory_[cpu.reg_.pc]].exp();
    }

    cpu.instruction_set_[cpu.memory_[cpu.reg_.pc]].exp();

  */
  // Replace standard out/in instruction to handle external shift register
  cpu.instruction_set_[0xD3] = (Instruction{
    0xd3, 1, "OUT D8", [&cpu, &shift_register, &shift_register_offset]() {
      uint8_t port_num = cpu.memory_[cpu.reg_.pc + 1];
      cpu.port_[port_num - 1] = cpu.reg_.a;
      cpu.reg_.pc += 2;
      if (port_num == 2)
      {
        // lower 3 bits used to make offset
        shift_register_offset = cpu.port_[port_num - 1] & 0x07;
      }

      else if (port_num == 4)
      {
        shift_register <<= 8;
        shift_register &= cpu.port_[port_num - 1];
      }
    }});

  cpu.instruction_set_[0xDB] = (Instruction{
    0xdb, 1, "IN D8", [&cpu, &shift_register, &shift_register_offset]() {
      uint8_t port_num = cpu.memory_[cpu.reg_.pc + 1];
      cpu.reg_.a = cpu.port_[port_num - 1];
      cpu.reg_.pc += 2;
      if (port_num == 3)
      {
        cpu.reg_.a = static_cast<uint8_t>(
          (shift_register >> shift_register_offset) & 0xFF);
      }
    }});

  PlayerInput inputDevice;

  // Create Screen
  SDL_Event event;
  SDL_Init(SDL_INIT_VIDEO);
  constexpr unsigned int screen_width = 256;
  constexpr unsigned int screen_height = 224;
  SDL_Window *window =
    SDL_CreateWindow("SDL2 Keyboard/Mouse events", SDL_WINDOWPOS_UNDEFINED,
                     SDL_WINDOWPOS_UNDEFINED, screen_width, screen_height, 0);

  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
  SDL_Texture *texture =
    SDL_CreateTexture(renderer, SDL_PIXELFORMAT_UNKNOWN,
                      SDL_TEXTUREACCESS_STATIC, screen_width, screen_height);
  Uint32 *pixels = new Uint32[screen_width * screen_height];

  memset(pixels, 62, screen_width * screen_height * sizeof(Uint32));
  bool quit = false;

  // Emulator kernel: process IO, display, compute instructions, etc...
  auto last_interrupt = std::chrono::high_resolution_clock::now();
  auto time_now = std::chrono::high_resolution_clock::now();
  auto last_instruction = std::chrono::high_resolution_clock::now();
  auto next_interrupt = 1;

  while (!quit)
  {
    SDL_UpdateTexture(texture, NULL, pixels, screen_width * sizeof(Uint32));

    // TODO: Need a non blocking way to do this, i think using wait instead of
    // poll and then using keyup / keydown to set port state will work better
    // SDL_PollEvent(&event);

    // TODO: Maybe use std::variant for this
    quit = inputDevice.processKeyboardEvent(event);
    updateCPUPorts(cpu, inputDevice.key_state_);

    time_now = std::chrono::high_resolution_clock::now();
    if (std::chrono::duration_cast<std::chrono::milliseconds>(time_now -
                                                              last_interrupt)
          .count() > 5000)
    {
      cpu.int_enable_ = 1;
      std::cout << "enabled interrupts\n";
    }

    if (std::chrono::duration_cast<std::chrono::milliseconds>(time_now -
                                                              last_interrupt)
          .count() > 16)
    {
      if (cpu.int_enable_ == 1)
      {
        cpu.reg_.pc--;
        last_interrupt = std::chrono::high_resolution_clock::now();
        if (next_interrupt == 1)
        {
          cpu.instruction_set_[0xCF].exp();
          next_interrupt = 2;
        }
        else
        {
          cpu.instruction_set_[0xD7].exp();
          next_interrupt = 1;

          // draw
          for (unsigned int byte_count = 0;
               byte_count < screen_width * screen_height / 8; byte_count++)
          {
            for (unsigned int bit = 0; bit < 8; bit++)
            {
              if (cpu.memory_[0x2000 + 0x400 + byte_count] & (0x01 << bit))
              {
                pixels[byte_count * 8 + bit] = 255;
              }

              else
              {
                pixels[byte_count * 8 + bit] = 0;
              }
            }
          }
          SDL_RenderClear(renderer);
          SDL_RenderCopy(renderer, texture, NULL, NULL);
          SDL_RenderPresent(renderer);
        }
      }
      time_now = std::chrono::high_resolution_clock::now();
      auto real_ticks = std::chrono::duration_cast<std::chrono::nanoseconds>(
                          time_now - last_instruction)
                          .count();

      auto num_instructions = real_ticks / 500;

      for (auto i = 0; i < num_instructions; i++)
      {
        cpu.instruction_set_[cpu.memory_[cpu.reg_.pc]].exp();
      }
      last_instruction = std::chrono::high_resolution_clock::now();
    }

    // std::cout << cpu.instruction_set_[cpu.memory_[cpu.reg_.pc]].instruction
    //          << std::endl;
  }

  // cleanup SDL
  // TODO: RAII method for this?
  delete[] pixels;
  SDL_DestroyTexture(texture);
  SDL_DestroyRenderer(renderer);

  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}

// Not ready for rendering code yet
/*

            for (unsigned int byte_count = 0;
                 byte_count < screen_width * screen_height / 8; byte_count++)
            {
              if ((cpu.memory_[0x2000 + 0x400 + byte_count]) != 0)
              {
                std::cout << "something\n";
              }

              for (unsigned int bit = 0; bit < 8; bit++)
              {
                if (cpu.memory_[0x2000 + 0x400 + byte_count] & (0x01 << bit))
                {
                  pixels[byte_count * 8 + bit] = 255;
                }

                else
                {
                  pixels[byte_count * 8 + bit] = 0;
                }
              }
            }

*/