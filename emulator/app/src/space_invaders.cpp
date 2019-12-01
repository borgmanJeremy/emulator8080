#include <SDL.h>
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

bool processKeyboard(SDL_Event &event)
{
  bool quit = false;
  switch (event.type)
  {
    case SDL_QUIT:
    {
      quit = true;
      break;
    }
    case SDL_KEYDOWN:
      switch (event.key.keysym.sym)
      {
        case SDLK_LEFT:
        {
          std::cout << "left\n";

          break;
        }
        case SDLK_RIGHT:
        {
          std::cout << "right\n";
          break;
        }
        case SDLK_UP:
        {
          std::cout << "up\n";
          break;
        }
        case SDLK_DOWN:
        {
          std::cout << "down\n";
          break;
        }
      }
      break;
  }
  return quit;
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
  while (!quit)
  {
    SDL_UpdateTexture(texture, NULL, pixels, screen_width * sizeof(Uint32));

    // TODO: Need a non blocking way to do this, i think using wait instead of
    // poll and then using keyup / keydown to set port state will work better
    SDL_WaitEvent(&event);

    // TODO: Maybe use std::variant for this
    quit = processKeyboard(event);

    cpu.instruction_set_[cpu.memory_[cpu.reg_.pc]].exp();
    // std::cout << cpu.instruction_set_[cpu.memory_[cpu.reg_.pc]].instruction
    //          << std::endl;

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
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