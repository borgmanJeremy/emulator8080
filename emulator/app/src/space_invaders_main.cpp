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

void drawWindow(int screen_width, int screen_height, Cpu_8080 &cpu,
                Uint32 *pixels);

int main()
{
  std::vector<std::string> paths_to_files = {
    "/home/jeremy/emulator8080/emulator/resource/invaders.h",
    "/home/jeremy/emulator8080/emulator/resource/invaders.g",
    "/home/jeremy/emulator8080/emulator/resource/invaders.f",
    "/home/jeremy/emulator8080/emulator/resource/invaders.e",
  };

  Cpu_8080 cpu;
  uint16_t shift_register_0 = 0;
  uint16_t shift_register_1 = 0;
  uint16_t shift_register_offset = 0;
  configureMemory(cpu, paths_to_files);
  /*
    for (unsigned long i = 0; i < 42433; i++)
    {
      cpu.instruction_set_[cpu.memory_[cpu.reg_.pc]].exp();
    }
  */
  cpu.instruction_set_[cpu.memory_[cpu.reg_.pc]].exp();

  // Replace standard out/in instruction to handle external shift register
  cpu.instruction_set_[0xD3] = (Instruction{
    0xd3, 1, "OUT D8",
    [&cpu, &shift_register_0, &shift_register_1, &shift_register_offset]() {
      uint8_t port_num = cpu.memory_[cpu.reg_.pc + 1];
      cpu.output_port_[port_num] = cpu.reg_.a;
      cpu.reg_.pc += 2;
      cpu.cycle_count_ += 10;
      if (port_num == 2)
      {
        // lower 3 bits used to make offset
        shift_register_offset = cpu.output_port_[port_num] & 0x07;
      }

      else if (port_num == 4)
      {
        shift_register_0 = shift_register_1;
        shift_register_1 = cpu.output_port_[port_num];
      }
    }});

  cpu.instruction_set_[0xDB] = (Instruction{
    0xdb, 1, "IN D8",
    [&cpu, &shift_register_0, &shift_register_1, &shift_register_offset]() {
      uint8_t port_num = cpu.memory_[cpu.reg_.pc + 1];
      cpu.reg_.a = cpu.input_port_[port_num];
      cpu.reg_.pc += 2;
      cpu.cycle_count_ += 10;
      if (port_num == 3)
      {
        uint16_t shift_register_value =
          (shift_register_1 << 8) | shift_register_0;
        cpu.reg_.a =
          ((shift_register_value >> (8 - shift_register_offset)) & 0xFF);
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
  auto next_interrupt = 1;

  RingBuffer<unsigned int> pc_log(250);
  RingBuffer<unsigned int> pc_log_hl(250);

  while (!quit)
  {
    auto now = std::chrono::high_resolution_clock::now();

    while ((std::chrono::high_resolution_clock::now() - now) <
           std::chrono::microseconds(20'000))
    {
    };
    SDL_UpdateTexture(texture, NULL, pixels, screen_width * sizeof(Uint32));

    // TODO: Need a non blocking way to do this, i think using wait instead of
    // poll and then using keyup / keydown to set port state will work better
    // SDL_PollEvent(&event);

    // TODO: Maybe use std::variant for this
    quit = inputDevice.processKeyboardEvent(event);
    // updateCPUPorts(cpu, inputDevice.key_state_);

    // SmartCounter<unsigned long int> interrupt_count(64'000);
    unsigned long int interrupt_count = 0;

    auto cpu_tick_time = std::chrono::nanoseconds(500);

    auto host_cpu_interval = std::chrono::milliseconds(20);
    auto cpu_cycles_per_host_interval = host_cpu_interval / cpu_tick_time;

    auto interrupt_interval = std::chrono::microseconds(8'333);
    auto interrupt_interal_tick = interrupt_interval / cpu_tick_time;

    cpu.cycle_count_ = 0;
    unsigned long old_cpu_count = cpu.cycle_count_;
    while (cpu.cycle_count_ < cpu_cycles_per_host_interval)
    {
      old_cpu_count = cpu.cycle_count_;

      pc_log.writeData(cpu.reg_.pc);
      cpu.instruction_set_[cpu.memory_[cpu.reg_.pc]].exp();

      interrupt_count += (cpu.cycle_count_ - old_cpu_count);

      if (interrupt_count >= interrupt_interal_tick)
      {
        interrupt_count = 0;
        if (cpu.int_enable_ == 1)
        {
          cpu.reg_.pc--;
          cpu.int_enable_ = 0;

          if (next_interrupt == 1)
          {
            cpu.instruction_set_[0xCF].exp();
            next_interrupt = 2;
          }
          else
          {
            cpu.instruction_set_[0xD7].exp();
            next_interrupt = 1;
          }

          drawWindow(screen_width, screen_height, cpu, pixels);
          SDL_RenderClear(renderer);
          SDL_RenderCopy(renderer, texture, NULL, NULL);
          SDL_RenderPresent(renderer);
        }
      }
    }
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

void drawWindow(int screen_width, int screen_height, Cpu_8080 &cpu,
                Uint32 *pixels)
{
  // draw
  for (auto byte_count = 0; byte_count < screen_width * screen_height / 8;
       byte_count++)
  {
    for (unsigned int bit = 0; bit < 8; bit++)
    {
      if (cpu.memory_[0x2000 + 0x400 + byte_count] & (0x01 << bit))
      {
        pixels[byte_count * 8 + bit] = 255 * 255 * 255;
      }

      else
      {
        pixels[byte_count * 8 + bit] = 0;
      }
    }
  }
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