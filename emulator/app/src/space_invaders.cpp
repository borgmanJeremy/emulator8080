#include "space_invaders.hpp"

void appendFromFile(std::string const &file_name,
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

void configureMemory(Cpu_8080 &cpu, std::vector<std::string> paths_to_files)
{
  std::vector<uint8_t> program_data;
  for (auto const &path : paths_to_files)
  {
    appendFromFile(path, program_data);
  }

  cpu.memory_.pushSegment(
    {"rom", 0x2000, smart_memory::MemoryAttribute::READ_ONLY});
  cpu.memory_.pushSegment(
    {"ram", 0x400, smart_memory::MemoryAttribute::READ_WRITE});
  cpu.memory_.pushSegment(
    {"v_ram", 0x1C00, smart_memory::MemoryAttribute::READ_WRITE});
  cpu.memory_.flashReadOnlySegment("rom", program_data);
}

void setBit(uint8_t &byte, uint8_t bit_pos) { byte |= (0x01 << bit_pos); }

void clearBit(uint8_t &byte, uint8_t bit_pos) { byte &= ~(0x01 << bit_pos); }

void updateCPUPorts(Cpu_8080 &cpu, std::map<std::string, KeyState> const &state)
{
  (state.at("player_1_shoot") == KeyState::PRESSED)
    ? setBit(cpu.input_port_[0], 4)
    : clearBit(cpu.input_port_[0], 4);

  (state.at("player_1_left") == KeyState::PRESSED)
    ? setBit(cpu.input_port_[0], 5)
    : clearBit(cpu.input_port_[0], 5);

  (state.at("player_1_right") == KeyState::PRESSED)
    ? setBit(cpu.input_port_[0], 6)
    : clearBit(cpu.input_port_[0], 6);
}

PlayerInput::PlayerInput()
{
  key_state_["player_1_start"] = KeyState::NOT_PRESSED;
  key_state_["player_1_shoot"] = KeyState::NOT_PRESSED;
  key_state_["player_1_left"] = KeyState::NOT_PRESSED;
  key_state_["player_1_right"] = KeyState::NOT_PRESSED;

  key_state_["player_2_start"] = KeyState::NOT_PRESSED;
  key_state_["player_2_shoot"] = KeyState::NOT_PRESSED;
  key_state_["player_2_left"] = KeyState::NOT_PRESSED;
  key_state_["player_2_right"] = KeyState::NOT_PRESSED;
}

bool PlayerInput::processKeyboardEvent(SDL_Event &event)
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
          key_state_["player_1_left"] = KeyState::PRESSED;
          std::cout << "left down\n";
          break;
        }
        case SDLK_RIGHT:
        {
          key_state_["player_1_right"] = KeyState::PRESSED;
          std::cout << "rght down\n";
          break;
        }
        case SDLK_SPACE:
        {
          key_state_["player_1_space"] = KeyState::PRESSED;
          std::cout << "space down\n";
          break;
        }
        case SDLK_RETURN:
        {
          key_state_["player_1_start"] = KeyState::PRESSED;
          std::cout << "Enter Down\n";
          break;
        }
      }
      break;

    case SDL_KEYUP:
      switch (event.key.keysym.sym)
      {
        case SDLK_LEFT:
        {
          key_state_["player_1_left"] = KeyState::NOT_PRESSED;
          std::cout << "left up\n";
          break;
        }
        case SDLK_RIGHT:
        {
          key_state_["player_1_right"] = KeyState::NOT_PRESSED;
          std::cout << "rght up\n";
          break;
        }
        case SDLK_SPACE:
        {
          key_state_["player_1_space"] = KeyState::NOT_PRESSED;
          std::cout << "space up\n";
          break;
        }
        case SDLK_RETURN:
        {
          key_state_["player_1_start"] = KeyState::NOT_PRESSED;
          std::cout << "Enter up\n";
          break;
        }
      }
      break;
  }
  return quit;
}