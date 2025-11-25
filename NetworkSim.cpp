#include "NetworkSim.h"

// Global Mutex for IO to prevent overlapping text from threads
std::mutex ioMutex;

// --- CellularCore ---
CellularCore::CellularCore() : totalMessages(0) {}

void CellularCore::addTraffic(int msgs) {
    std::lock_guard<std::mutex> lock(coreMutex);
    totalMessages += msgs;
}

int CellularCore::getRequiredCores() {
    std::lock_guard<std::mutex> lock(coreMutex);
    if (totalMessages == 0) return 1;

    // Model "overhead per 100 messages":
    // 1 overhead block = 100 messages
    long overheadBlocks =
        (totalMessages + MESSAGES_PER_OVERHEAD_BLOCK - 1) / MESSAGES_PER_OVERHEAD_BLOCK;

    // Each core can handle OVERHEAD_BLOCKS_PER_CORE blocks
    long cores =
        (overheadBlocks + OVERHEAD_BLOCKS_PER_CORE - 1) / OVERHEAD_BLOCKS_PER_CORE;

    return (int)cores;
}

// --- CellTower Base ---
CellTower::CellTower(double bw, int ant, CellularCore* c)
    : bandwidth(bw), antennas(ant), core(c) {}

const std::vector<UserDevice>& CellTower::getUsersInChannel(size_t idx) const {
    if (idx >= frequencyUsers.size()) {
        throw NetworkException("Channel index out of range");
    }
    return frequencyUsers[idx];
}

// --- 2G Implementation ---
// 1 MHz total; 16 users per 200 kHz; each user: 5 data + 15 voice = 20 messages.

// --- 4G Implementation ---
// 10 kHz sub-channels, 30 users per sub-channel, up to 4 antennas (MIMO).
// Each user still takes 10 messages.
