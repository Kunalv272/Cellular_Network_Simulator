/* main.cpp */
#include "NetworkSim.h"
#include <thread>

// Thread worker function
void runTowerSimulation(CellTower* tower) {
    if (tower) {
        tower->calculateCapacity();
    }
}

int main() {
    print((char*)"Starting Multithreaded Cellular Network Simulator...\n");

    try {
        // 1. Create one Shared Core for the entire network
        CellularCore* sharedCore = new CellularCore();
        
        // 2. Create Towers
        CellTower* tower2g = new Tower2G(sharedCore);
        CellTower* tower3g = new Tower3G(sharedCore);
        CellTower* tower4g = new Tower4G(sharedCore);
        CellTower* tower5g = new Tower5G(sharedCore);

        // 3. Launch Threads (Simulate all running at the same time)
        // Pass the helper function and the specific tower pointer
        std::thread t2(runTowerSimulation, tower2g);
        std::thread t3(runTowerSimulation, tower3g);
        std::thread t4(runTowerSimulation, tower4g);
        std::thread t5(runTowerSimulation, tower5g);

        // 4. Wait for all threads to finish
        t2.join();
        t3.join();
        t4.join();
        t5.join();

        // 5. Final Report
        print((char*)"\n=== Final Network Core Status ===\n");
        print((char*)"All generations processed.\n");
        print((char*)"Total Cellular Cores Required: ");
        
        // We can use the helper inside any tower to print the int, 
        // or add a helper in main. Reusing tower2g's helper for convenience:
        tower2g->printInt(sharedCore->getRequiredCores());
        print((char*)"\n");

        // 6. Cleanup
        delete tower2g;
        delete tower3g;
        delete tower4g;
        delete tower5g;
        delete sharedCore;

    } catch (const SimulationException& e) {
        print((char*)"Error: ");
        print((char*)e.getMessage());
        print((char*)"\n");
    } catch (...) {
        print((char*)"Unknown Error\n");
    }

    print((char*)"Simulation Complete.\n");
    return 0;
}