cppcheck --project=emulator/build/compile_commands.json
python3 /usr/bin/run-clang-tidy -checks='*,-fuchsia*,-clang-diagnostic-unused-lambda-capture,-llvm-include-order' -p emulator/build/