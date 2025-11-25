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
        CellularCore* sharedCore = new CellularCore();
        CellTower* tower2g = new Tower2G(sharedCore);
        CellTower* tower3g = new Tower3G(sharedCore);
        CellTower* tower4g = new Tower4G(sharedCore);
        CellTower* tower5g = new Tower5G(sharedCore);
        std::thread t2(runTowerSimulation, tower2g);
        std::thread t3(runTowerSimulation, tower3g);
        std::thread t4(runTowerSimulation, tower4g);
        std::thread t5(runTowerSimulation, tower5g);
        t2.join();
        t3.join();
        t4.join();
        t5.join();
        print((char*)"\n=== Final Network Core Status ===\n");
        print((char*)"All generations processed.\n");
        print((char*)"Total Cellular Cores Required: ");
        tower2g->printInt(sharedCore->getRequiredCores());
        print((char*)"\n");

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