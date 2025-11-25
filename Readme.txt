Cellular Network Simulator
--------------------------

How to Build:
1. Ensure `nasm` (assembler) and `g++` are installed.
2. Run `make` to build the optimized binary `sim_opt`.
3. Run `./sim_opt` to execute.

Architecture:
- The project uses a custom `_start` entry point defined in `syscall.s`.
- `basicIO` handles all I/O via direct system calls (bypassing <iostream>).
- `NetworkSim` defines the OOP hierarchy for 2G, 3G, 4G, and 5G towers.
- `std::thread` and `std::mutex` are used to simulate concurrent network generation activity.

Files:
- syscall.s: Custom assembly entry point and system wrappers.
- basicIO.cpp/h: Provided IO class implementation.
- NetworkSim.cpp/h: Simulation logic, OOP classes, and Templates.
- main.cpp: Thread orchestration.
- BlockStandardIO.h: Empty dummy file to satisfy dependencies.

Logic:
- 2G: 16 users/200kHz.
- 3G: 32 users/200kHz (CDMA).
- 4G: 30 users/10kHz * 4 Antennas.
- 5G: Base 4G capacity (with 16 antennas) + High Band capacity (30 users/MHz * 16 antennas).