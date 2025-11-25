/* main.cpp */
#include "NetworkSim.h"

// Thread worker function
void runSim(CellTower* tower) {
    if(tower) tower->calculateCapacity();
}

// Exposed to syscall.s
extern "C" int main() {
    io.activateInput();
    io.outputstring("Starting Cellular Simulator...\n\n");

    try {
        // 1. Setup Core
        CellularCore* core = new CellularCore();

        // 2. Setup Towers
        Tower2G* t2 = new Tower2G(core);
        Tower3G* t3 = new Tower3G(core);
        Tower4G* t4 = new Tower4G(core);
        Tower5G* t5 = new Tower5G(core);

        // 3. Multithreading (Allowed by prompt)
        std::thread th2(runSim, t2);
        std::thread th3(runSim, t3);
        std::thread th4(runSim, t4);
        std::thread th5(runSim, t5);

        // 4. Wait for completion
        th2.join();
        th3.join();
        th4.join();
        th5.join();

        // 5. Final Stats
        io.outputstring("=== Final Results ===\n");
        io.outputstring("Cores Required: ");
        io.outputint(core->getRequiredCores());
        io.terminate();

        // 6. Template Demonstration (Channel Usage)
        // Creating a 5G High Band Channel (1MHz slice)
        Channel<UserDevice> highBandChannel(30, 1000.0); 
        UserDevice u1(101, 10);
        if(highBandChannel.addUser(u1)) {
            io.outputstring("Template Test: User added to channel successfully.\n");
        }

        // Cleanup
        delete t2; delete t3; delete t4; delete t5;
        delete core;

    } catch (const NetworkException& e) {
        io.errorstring("Error: ");
        io.errorstring(e.getMessage());
        io.terminate();
    } catch (...) {
        io.errorstring("Unknown system error.\n");
    }

    return 0;
}