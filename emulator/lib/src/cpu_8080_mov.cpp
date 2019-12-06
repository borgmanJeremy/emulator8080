#include "cpu_8080.hpp"

void Cpu_8080::pushToStack(uint8_t byte)
{
  memory_[reg_.sp - 1u] = byte;
  reg_.sp--;
}
uint8_t Cpu_8080::popFromStack()
{
  uint8_t value = memory_[reg_.sp];
  reg_.sp++;
  return value;
}

void Cpu_8080::addMoveOperations()
{
  instruction_set_.emplace_back(Instruction{0xC1, 0, "POP B", [this]() {
                                              reg_.c = popFromStack();
                                              reg_.b = popFromStack();
                                              reg_.pc++;
                                              cycle_count_+=10;
                                            }});

  instruction_set_.emplace_back(Instruction{0xC5, 0, "PUSH B", [this]() {
                                              pushToStack(reg_.b);
                                              pushToStack(reg_.c);
                                              reg_.pc++;
                                              cycle_count_+=11;
                                            }});

  instruction_set_.emplace_back(Instruction{0xD1, 0, "POP D", [this]() {
                                              reg_.e = popFromStack();
                                              reg_.d = popFromStack();
                                              reg_.pc++;
                                              cycle_count_+=10;
                                            }});

  instruction_set_.emplace_back(Instruction{0xd5, 0, "PUSH D", [this]() {
                                              pushToStack(reg_.d);
                                              pushToStack(reg_.e);
                                              reg_.pc++;
                                              cycle_count_+=11;
                                            }});

  instruction_set_.emplace_back(Instruction{0xe1, 0, "POP H", [this]() {
                                              reg_.l = popFromStack();
                                              reg_.h = popFromStack();
                                              reg_.pc++;
                                              cycle_count_+=10;
                                            }});

  instruction_set_.emplace_back(Instruction{0xe5, 0, "PUSH H", [this]() {
                                              pushToStack(reg_.h);
                                              pushToStack(reg_.l);
                                              reg_.pc++;
                                              cycle_count_+=11;
                                            }});

  instruction_set_.emplace_back(
    Instruction{0xf1, 0, "POP PSW", [this]() {
                  auto flag_byte = popFromStack();
                  flags_.z = (flag_byte & 0x01) ? 1 : 0;
                  flags_.s = (flag_byte & 0x02) ? 1 : 0;
                  flags_.p = (flag_byte & 0x04) ? 1 : 0;
                  flags_.cy = (flag_byte & 0x08) ? 1 : 0;
                  flags_.ac = (flag_byte & 0x10) ? 1 : 0;

                  reg_.a = popFromStack();
                  reg_.pc++;

                                              cycle_count_+=10;
                }});

  instruction_set_.emplace_back(
    Instruction{0xf5, 0, "PUSH PSW", [this]() {
                  pushToStack(reg_.a);
                  uint8_t flag_byte = 0;
                  flag_byte |= (flags_.z) ? 0x01 : 0;
                  flag_byte |= (flags_.s) ? 0x02 : 0;
                  flag_byte |= (flags_.p) ? 0x04 : 0;
                  flag_byte |= (flags_.cy) ? 0x08 : 0;
                  flag_byte |= (flags_.ac) ? 0x10 : 0;
                  pushToStack(flag_byte);
                  reg_.pc++;
                                              cycle_count_+=11;
                }});

  instruction_set_.emplace_back(Instruction{
    0x32, 2, "STA adr", [this]() {
      auto addr = (static_cast<unsigned int>(memory_[reg_.pc + 2]) << 8) +
                  memory_[reg_.pc + 1];
      memory_[addr] = reg_.a;

      reg_.pc += 3;
                                              cycle_count_+=13;
    }});

  instruction_set_.emplace_back(Instruction{
    0x3A, 2, "LDA adr", [this]() {
      auto addr = (static_cast<unsigned int>(memory_[reg_.pc + 2]) << 8) +
                  memory_[reg_.pc + 1];
      reg_.a = memory_[addr];
      reg_.pc += 3;

                                              cycle_count_+=13;
    }});

  //
  instruction_set_.emplace_back(Instruction{
    0x22, 3, "SHLD adr", [this]() {
      auto addr = (static_cast<unsigned int>(memory_[reg_.pc + 2]) << 8) +
                  memory_[reg_.pc + 1];
      memory_[addr] = reg_.l;
      memory_[addr + 1] = reg_.h;
      reg_.pc += 3;

                                              cycle_count_+=16;
    }});

  instruction_set_.emplace_back(Instruction{
    0x2A, 2, "LHLD adr", [this]() {
      auto addr = (static_cast<unsigned int>(memory_[reg_.pc + 2]) << 8) +
                  memory_[reg_.pc + 1];
      reg_.l = memory_[addr];
      reg_.h = memory_[addr + 1];
      reg_.pc += 3;
                                              cycle_count_+=16;
    }});

  // MVI
  instruction_set_.emplace_back(Instruction{6, 1, "MVI B, D8", [this]() {
                                              reg_.b = memory_[reg_.pc + 1];
                                              reg_.pc += 2;
                                              cycle_count_+=7;
                                            }});

  instruction_set_.emplace_back(Instruction{0x0E, 1, "MVI C,D8", [this]() {
                                              reg_.c = memory_[reg_.pc + 1];
                                              reg_.pc += 2;
                                              cycle_count_+=7;
                                            }});

  instruction_set_.emplace_back(Instruction{0x16, 1, "MVI D, D8", [this]() {
                                              reg_.d = memory_[reg_.pc + 1];
                                              reg_.pc += 2;
                                              cycle_count_+=7;
                                            }});

  instruction_set_.emplace_back(Instruction{0x1E, 1, "MVI E,D8", [this]() {
                                              reg_.e = memory_[reg_.pc + 1];
                                              reg_.pc += 2;
                                              cycle_count_+=7;
                                            }});

  instruction_set_.emplace_back(Instruction{0x26, 1, "MVI H, D8", [this]() {
                                              reg_.h = memory_[reg_.pc + 1];
                                              reg_.pc += 2;
                                              cycle_count_+=7;
                                            }});

  instruction_set_.emplace_back(Instruction{0x2E, 1, "MVI L,D8", [this]() {
                                              reg_.l = memory_[reg_.pc + 1];
                                              reg_.pc += 2;
                                              cycle_count_+=7;
                                            }});

  instruction_set_.emplace_back(Instruction{0x36, 1, "MVI M, D8", [this]() {
                                              auto addr = getAddressFromHL();
                                              uint8_t val =
                                                memory_[reg_.pc + 1];
                                              memory_[addr] = val;
                                              reg_.pc += 2;
                                              cycle_count_+=10;
                                            }});

  instruction_set_.emplace_back(Instruction{0x3E, 1, "MVI A,D8", [this]() {
                                              reg_.a = memory_[reg_.pc + 1];
                                              reg_.pc += 2;
                                              cycle_count_+=7;
                                            }});

  // LXI
  instruction_set_.emplace_back(Instruction{1, 2, "LXI B,D16", [this]() {
                                              reg_.b = memory_[reg_.pc + 2];
                                              reg_.c = memory_[reg_.pc + 1];
                                              reg_.pc += 3;
                                              cycle_count_+=10;
                                            }});
  instruction_set_.emplace_back(Instruction{0x11, 2, "LXI D,D16", [this]() {
                                              reg_.d = memory_[reg_.pc + 2];
                                              reg_.e = memory_[reg_.pc + 1];
                                              reg_.pc += 3;
                                              cycle_count_+=10;
                                            }});

  instruction_set_.emplace_back(Instruction{0x21, 2, "LXI H,D16", [this]() {
                                              reg_.h = memory_[reg_.pc + 2];
                                              reg_.l = memory_[reg_.pc + 1];
                                              reg_.pc += 3;
                                              cycle_count_+=10;
                                            }});

  instruction_set_.emplace_back(
    Instruction{0x31, 2, "LXI SP,D16", [this]() {
                  reg_.sp = static_cast<uint16_t>(
                    static_cast<uint16_t>(memory_[reg_.pc + 2u] << 8u) +
                    static_cast<uint16_t>(memory_[reg_.pc + 1u]));
                  reg_.pc += 3;
                                              cycle_count_+=10;
                }});

  // LDAX
  instruction_set_.emplace_back(Instruction{0x0A, 0, "LDAX B", [this]() {
                                              auto addr = getAddressFromBC();
                                              reg_.a = memory_[addr];
                                              reg_.pc++;
                                              cycle_count_+=7;
                                            }});

  instruction_set_.emplace_back(Instruction{0x1A, 0, "LDAX D", [this]() {
                                              auto addr = getAddressFromDE();
                                              reg_.a = memory_[addr];
                                              reg_.pc++;
                                              cycle_count_+=7;
                                            }});

  // STAX
  instruction_set_.emplace_back(Instruction{2, 0, "STAX B", [this]() {
                                              auto addr = getAddressFromBC();
                                              memory_[addr] = reg_.a;

                                              reg_.pc++;
                                              cycle_count_+=7;
                                            }});

  instruction_set_.emplace_back(Instruction{0x12, 0, "STAX D", [this]() {
                                              auto addr = getAddressFromDE();
                                              memory_[addr] = reg_.a;

                                              reg_.pc++;
                                              cycle_count_+=7;
                                            }});

  ///////////////
  instruction_set_.emplace_back(Instruction{0x40, 0, "MOV B,B", [this]() {
                                              reg_.b = reg_.b;
                                              reg_.pc++;
                                              cycle_count_+=5;
                                            }});

  instruction_set_.emplace_back(Instruction{0x41, 0, "MOV B,C", [this]() {
                                              reg_.b = reg_.c;
                                              reg_.pc++;
                                              cycle_count_+=5;
                                            }});

  instruction_set_.emplace_back(Instruction{0x42, 0, "MOV B,D", [this]() {
                                              reg_.b = reg_.d;
                                              reg_.pc++;
                                              cycle_count_+=5;
                                            }});

  instruction_set_.emplace_back(Instruction{0x43, 0, "MOV B,E", [this]() {
                                              reg_.b = reg_.e;
                                              reg_.pc++;
                                              cycle_count_+=5;
                                            }});

  instruction_set_.emplace_back(Instruction{0x44, 0, "MOV B,H", [this]() {
                                              reg_.b = reg_.h;
                                              reg_.pc++;
                                              cycle_count_+=5;
                                            }});

  instruction_set_.emplace_back(Instruction{0x45, 0, "MOV B,L", [this]() {
                                              reg_.b = reg_.l;
                                              reg_.pc++;
                                              cycle_count_+=5;
                                            }});

  instruction_set_.emplace_back(Instruction{0x46, 0, "MOV B,M", [this]() {
                                              auto addr = getAddressFromHL();
                                              reg_.b = memory_[addr];
                                              reg_.pc++;
                                              cycle_count_+=7;
                                            }});

  instruction_set_.emplace_back(Instruction{0x47, 0, "MOV B,A", [this]() {
                                              reg_.b = reg_.a;
                                              reg_.pc++;
                                              cycle_count_+=5;
                                            }});

  ///////////////
  instruction_set_.emplace_back(Instruction{0x48, 0, "MOV C,B", [this]() {
                                              reg_.c = reg_.b;
                                              reg_.pc++;
                                              cycle_count_+=5;
                                            }});

  instruction_set_.emplace_back(Instruction{0x49, 0, "MOV C,C", [this]() {
                                              reg_.c = reg_.c;
                                              reg_.pc++;
                                              cycle_count_+=5;
                                            }});

  instruction_set_.emplace_back(Instruction{0x4A, 0, "MOV C,D", [this]() {
                                              reg_.c = reg_.d;
                                              reg_.pc++;
                                              cycle_count_+=5;
                                            }});

  instruction_set_.emplace_back(Instruction{0x4B, 0, "MOV C,E", [this]() {
                                              reg_.c = reg_.e;
                                              reg_.pc++;
                                              cycle_count_+=5;
                                            }});

  instruction_set_.emplace_back(Instruction{0x4C, 0, "MOV C,H", [this]() {
                                              reg_.c = reg_.h;
                                              reg_.pc++;
                                              cycle_count_+=5;
                                            }});

  instruction_set_.emplace_back(Instruction{0x4D, 0, "MOV C,L", [this]() {
                                              reg_.c = reg_.l;
                                              reg_.pc++;
                                              cycle_count_+=5;
                                            }});

  instruction_set_.emplace_back(Instruction{0x4E, 0, "MOV C,M", [this]() {
                                              auto addr = getAddressFromHL();
                                              reg_.c = memory_[addr];
                                              reg_.pc++;
                                              cycle_count_+=7;
                                            }});

  instruction_set_.emplace_back(Instruction{0x4F, 0, "MOV C,A", [this]() {
                                              reg_.c = reg_.a;
                                              reg_.pc++;
                                              cycle_count_+=5;
                                            }});

  ///////////////
  instruction_set_.emplace_back(Instruction{0x50, 0, "MOV D,B", [this]() {
                                              reg_.d = reg_.b;
                                              reg_.pc++;
                                              cycle_count_+=5;
                                            }});

  instruction_set_.emplace_back(Instruction{0x51, 0, "MOV D,C", [this]() {
                                              reg_.d = reg_.c;
                                              reg_.pc++;
                                              cycle_count_+=5;
                                            }});

  instruction_set_.emplace_back(Instruction{0x52, 0, "MOV D,D", [this]() {
                                              reg_.d = reg_.d;
                                              reg_.pc++;
                                              cycle_count_+=5;
                                            }});

  instruction_set_.emplace_back(Instruction{0x53, 0, "MOV D,E", [this]() {
                                              reg_.d = reg_.e;
                                              reg_.pc++;
                                              cycle_count_+=5;
                                            }});

  instruction_set_.emplace_back(Instruction{0x54, 0, "MOV D,H", [this]() {
                                              reg_.d = reg_.h;
                                              reg_.pc++;
                                              cycle_count_+=5;
                                            }});

  instruction_set_.emplace_back(Instruction{0x55, 0, "MOV D,L", [this]() {
                                              reg_.d = reg_.l;
                                              reg_.pc++;
                                              cycle_count_+=5;
                                            }});

  instruction_set_.emplace_back(Instruction{0x56, 0, "MOV D,M", [this]() {
                                              auto addr = getAddressFromHL();
                                              reg_.d = memory_[addr];
                                              reg_.pc++;
                                              cycle_count_+=7;
                                            }});

  instruction_set_.emplace_back(Instruction{0x57, 0, "MOV D,A", [this]() {
                                              reg_.d = reg_.a;
                                              reg_.pc++;
                                              cycle_count_+=5;
                                            }});

  ///////////////
  instruction_set_.emplace_back(Instruction{0x58, 0, "MOV E,B", [this]() {
                                              reg_.e = reg_.b;
                                              reg_.pc++;
                                              cycle_count_+=5;
                                            }});

  instruction_set_.emplace_back(Instruction{0x59, 0, "MOV E,C", [this]() {
                                              reg_.e = reg_.c;
                                              reg_.pc++;
                                              cycle_count_+=5;
                                            }});

  instruction_set_.emplace_back(Instruction{0x5A, 0, "MOV E,D", [this]() {
                                              reg_.e = reg_.d;
                                              reg_.pc++;
                                              cycle_count_+=5;
                                            }});

  instruction_set_.emplace_back(Instruction{0x5B, 0, "MOV E,E", [this]() {
                                              reg_.e = reg_.e;
                                              reg_.pc++;
                                              cycle_count_+=5;
                                            }});

  instruction_set_.emplace_back(Instruction{0x5C, 0, "MOV E,H", [this]() {
                                              reg_.e = reg_.h;
                                              reg_.pc++;
                                              cycle_count_+=5;
                                            }});

  instruction_set_.emplace_back(Instruction{0x5D, 0, "MOV E,L", [this]() {
                                              reg_.e = reg_.l;
                                              reg_.pc++;
                                              cycle_count_+=5;
                                            }});

  instruction_set_.emplace_back(Instruction{0x5E, 0, "MOV E,M", [this]() {
                                              auto addr = getAddressFromHL();
                                              reg_.e = memory_[addr];
                                              reg_.pc++;
                                              cycle_count_+=7;
                                            }});

  instruction_set_.emplace_back(Instruction{0x5F, 0, "MOV E,A", [this]() {
                                              reg_.e = reg_.a;
                                              reg_.pc++;
                                              cycle_count_+=5;
                                            }});

  ///////////////
  instruction_set_.emplace_back(Instruction{0x60, 0, "MOV H,B", [this]() {
                                              reg_.h = reg_.b;
                                              reg_.pc++;
                                              cycle_count_+=5;
                                            }});

  instruction_set_.emplace_back(Instruction{0x61, 0, "MOV H,C", [this]() {
                                              reg_.h = reg_.c;
                                              reg_.pc++;
                                              cycle_count_+=5;
                                            }});

  instruction_set_.emplace_back(Instruction{0x62, 0, "MOV H,D", [this]() {
                                              reg_.h = reg_.d;
                                              reg_.pc++;
                                              cycle_count_+=5;
                                            }});

  instruction_set_.emplace_back(Instruction{0x63, 0, "MOV H,E", [this]() {
                                              reg_.h = reg_.e;
                                              reg_.pc++;
                                              cycle_count_+=5;
                                            }});

  instruction_set_.emplace_back(Instruction{0x64, 0, "MOV H,H", [this]() {
                                              reg_.h = reg_.h;
                                              reg_.pc++;
                                              cycle_count_+=5;
                                            }});

  instruction_set_.emplace_back(Instruction{0x65, 0, "MOV H,L", [this]() {
                                              reg_.h = reg_.l;
                                              reg_.pc++;
                                              cycle_count_+=5;
                                            }});

  instruction_set_.emplace_back(Instruction{0x66, 0, "MOV H,M", [this]() {
                                              auto addr = getAddressFromHL();
                                              reg_.h = memory_[addr];

                                              reg_.pc++;
                                              cycle_count_+=7;
                                            }});

  instruction_set_.emplace_back(Instruction{0x67, 0, "MOV H,A", [this]() {
                                              reg_.h = reg_.a;
                                              reg_.pc++;
                                              cycle_count_+=5;
                                            }});
  ///////////////
  instruction_set_.emplace_back(Instruction{0x68, 0, "MOV L,B", [this]() {
                                              reg_.l = reg_.b;
                                              reg_.pc++;
                                              cycle_count_+=5;
                                            }});

  instruction_set_.emplace_back(Instruction{0x69, 0, "MOV L,C", [this]() {
                                              reg_.l = reg_.c;
                                              reg_.pc++;
                                              cycle_count_+=5;
                                            }});

  instruction_set_.emplace_back(Instruction{0x6A, 0, "MOV L,D", [this]() {
                                              reg_.l = reg_.d;
                                              reg_.pc++;
                                              cycle_count_+=5;
                                            }});

  instruction_set_.emplace_back(Instruction{0x6B, 0, "MOV L,E", [this]() {
                                              reg_.l = reg_.e;
                                              reg_.pc++;
                                              cycle_count_+=5;
                                            }});

  instruction_set_.emplace_back(Instruction{0x6C, 0, "MOV L,H", [this]() {
                                              reg_.l = reg_.h;
                                              reg_.pc++;
                                              cycle_count_+=5;
                                            }});

  instruction_set_.emplace_back(Instruction{0x6D, 0, "MOV L,L", [this]() {
                                              reg_.l = reg_.l;
                                              reg_.pc++;
                                              cycle_count_+=5;
                                            }});

  instruction_set_.emplace_back(Instruction{0x6E, 0, "MOV L,M", [this]() {
                                              auto addr = getAddressFromHL();
                                              reg_.l = memory_[addr];
                                              reg_.pc++;
                                              cycle_count_+=7;
                                            }});

  instruction_set_.emplace_back(Instruction{0x6F, 0, "MOV L,A", [this]() {
                                              reg_.l = reg_.a;
                                              reg_.pc++;
                                              cycle_count_+=5;
                                            }});

  ///////////////
  instruction_set_.emplace_back(Instruction{0x78, 0, "MOV A,B", [this]() {
                                              reg_.a = reg_.b;
                                              reg_.pc++;
                                              cycle_count_+=5;
                                            }});

  instruction_set_.emplace_back(Instruction{0x79, 0, "MOV A,C", [this]() {
                                              reg_.a = reg_.c;
                                              reg_.pc++;
                                              cycle_count_+=5;
                                            }});

  instruction_set_.emplace_back(Instruction{0x7A, 0, "MOV A,D", [this]() {
                                              reg_.a = reg_.d;
                                              reg_.pc++;
                                              cycle_count_+=5;
                                            }});

  instruction_set_.emplace_back(Instruction{0x7B, 0, "MOV A,E", [this]() {
                                              reg_.a = reg_.e;
                                              reg_.pc++;
                                              cycle_count_+=5;
                                            }});

  instruction_set_.emplace_back(Instruction{0x7C, 0, "MOV A,H", [this]() {
                                              reg_.a = reg_.h;
                                              reg_.pc++;
                                              cycle_count_+=5;
                                            }});

  instruction_set_.emplace_back(Instruction{0x7D, 0, "MOV A,L", [this]() {
                                              reg_.a = reg_.l;
                                              reg_.pc++;
                                              cycle_count_+=5;
                                            }});

  instruction_set_.emplace_back(Instruction{0x7E, 0, "MOV A,M", [this]() {
                                              auto addr = getAddressFromHL();
                                              reg_.a = memory_[addr];
                                              reg_.pc++;
                                              cycle_count_+=7;
                                            }});

  instruction_set_.emplace_back(Instruction{0x7F, 0, "MOV A,A", [this]() {
                                              reg_.a = reg_.a;
                                              reg_.pc++;
                                              cycle_count_+=5;
                                            }});

  ///////////////
  instruction_set_.emplace_back(Instruction{0x70, 0, "MOV M,B", [this]() {
                                              auto addr = getAddressFromHL();
                                              memory_[addr] = reg_.b;
                                              reg_.pc++;
                                              cycle_count_+=7;
                                            }});

  instruction_set_.emplace_back(Instruction{0x71, 0, "MOV M,C", [this]() {
                                              auto addr = getAddressFromHL();
                                              memory_[addr] = reg_.c;
                                              reg_.pc++;
                                              cycle_count_+=7;
                                            }});

  instruction_set_.emplace_back(Instruction{0x72, 0, "MOV M,D", [this]() {
                                              auto addr = getAddressFromHL();
                                              memory_[addr] = reg_.d;
                                              reg_.pc++;
                                              cycle_count_+=7;
                                            }});

  instruction_set_.emplace_back(Instruction{0x73, 0, "MOV M,E", [this]() {
                                              auto addr = getAddressFromHL();
                                              memory_[addr] = reg_.e;
                                              reg_.pc++;
                                              cycle_count_+=7;
                                            }});

  instruction_set_.emplace_back(Instruction{0x74, 0, "MOV M,H", [this]() {
                                              auto addr = getAddressFromHL();
                                              memory_[addr] = reg_.h;
                                              reg_.pc++;
                                              cycle_count_+=7;
                                            }});

  instruction_set_.emplace_back(Instruction{0x75, 0, "MOV M,L", [this]() {
                                              auto addr = getAddressFromHL();
                                              memory_[addr] = reg_.l;
                                              reg_.pc++;
                                              cycle_count_+=7;
                                            }});

  instruction_set_.emplace_back(Instruction{0x77, 0, "MOV M,A", [this]() {
                                              auto addr = getAddressFromHL();
                                              memory_[addr] = reg_.a;
                                              reg_.pc++;
                                              cycle_count_+=7;
                                            }});

  // Todo: Replace with swap template function. Proxy class isnt working with it
  // right now

  instruction_set_.emplace_back(
    Instruction{0xe3, 0, "XTHL", [this]() {
                  adl_swap(reg_.l, memory_[reg_.sp]);
                  adl_swap(reg_.h, memory_[reg_.sp + 1]);
                  reg_.pc++;

                                              cycle_count_+=18;
                }});

  instruction_set_.emplace_back(Instruction{0xeb, 0, "XCHG", [this]() {
                                              std::swap(reg_.d, reg_.h);
                                              std::swap(reg_.e, reg_.l);
                                              reg_.pc++;
                                              cycle_count_+=5;
                                            }});

  instruction_set_.emplace_back(Instruction{0xf9, 0, "SPHL", [this]() {
                                              reg_.sp = getAddressFromHL();
                                              reg_.pc++;
                                              cycle_count_+=5;
                                            }});

  instruction_set_.emplace_back(Instruction{0xd3, 1, "OUT D8", [this]() {
                                              uint8_t port_num = memory_[reg_.pc + 1];
                                              output_port_[port_num] = reg_.a;
                                              reg_.pc += 2;
                                              cycle_count_+=10;
                                            }});

  instruction_set_.emplace_back(Instruction{0xdb, 1, "IN D8", [this]() {
                                              uint8_t port_num = memory_[reg_.pc + 1];
                                              reg_.a = input_port_[port_num ];
                                              reg_.pc += 2;
                                              cycle_count_+=10;
                                            }});
}