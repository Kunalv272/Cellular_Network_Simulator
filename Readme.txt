
Network Simulator Project
-------------------------

Included Files:
1. NetworkSim.h   - Header file defining classes, templates, and hierarchy.
2. NetworkSim.cpp - Implementation of 2G, 3G, 4G, 5G logic.
3. main.cpp       - Main execution loop and exception handling.
4. Makefile       - Build script.
5. Basicio.cpp/.h - (Required) Provided I/O library.
6. syscall.s      - (Required) Provided assembly system calls.

Compilation:
To build the project, open a terminal in the directory and run:

    make all

This will generate two executables:
1. sim_debug (includes debugging symbols)
2. sim_opt   (optimized for performance)

Execution:
Run the optimized binary:
    ./sim_opt

Logic Overview:
- 2G: 1MHz total / 200kHz channels = 5 channels. 16 users/channel.
- 3G: 1MHz total / 200kHz channels = 5 channels. 32 users/channel (CDMA).
- 4G: 1MHz total / 10kHz sub-channels. 30 users/sub-channel. 4x MIMO antennas.
- 5G: 4G Base + 10MHz High Freq Band (at 30 users/MHz). 16x Massive MIMO.

Note:
This program strictly uses the provided Basicio library for output, avoiding
standard C++ iostreams to meet system constraints. Integer-to-string conversion
is handled manually within NetworkSim.cpp.