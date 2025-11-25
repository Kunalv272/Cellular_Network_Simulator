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
Tower2G::Tower2G(CellularCore* c) : CellTower(1000.0, 1, c) {} // 1000 kHz = 1 MHz

void Tower2G::calculateCapacity() {
    int channelWidthKHz = 200;
    int channels = (int)(bandwidth / channelWidthKHz); // 1000 / 200 = 5
    int usersPerChannel = 16;
    int totalUsers = channels * usersPerChannel;

    int messagesPerUser = 5 + 15; // data + voice
    core->addTraffic(totalUsers * messagesPerUser);

    // Build list of users occupying the *first* 200 kHz channel
    frequencyUsers.clear();
    frequencyUsers.resize(1);
    for (int uid = 1; uid <= usersPerChannel; ++uid) {
        frequencyUsers[0].emplace_back(uid, messagesPerUser);
    }

    {
        std::lock_guard<std::mutex> lock(ioMutex);
        io.outputstring("--- 2G Analysis ---\n");
        io.outputstring("Users per 200kHz: ");
        io.outputint(usersPerChannel);
        io.terminate();

        io.outputstring("Total Users (1 MHz): ");
        io.outputint(totalUsers);
        io.terminate();

        io.outputstring("Users in first 200kHz channel: ");
        for (const auto& u : frequencyUsers[0]) {
            io.outputint(u.getId());
            io.outputstring(" ");
        }
        io.terminate();
        io.terminate();
    }
}

// --- 3G Implementation ---
// 32 users per 200 kHz; 10 messages per user, 1 MHz total.
Tower3G::Tower3G(CellularCore* c) : CellTower(1000.0, 1, c) {}

void Tower3G::calculateCapacity() {
    int channelWidthKHz = 200;
    int channels = (int)(bandwidth / channelWidthKHz); // 5 channels
    int usersPerChannel = 32;
    int totalUsers = channels * usersPerChannel;

    int messagesPerUser = 10;
    core->addTraffic(totalUsers * messagesPerUser);

    // Users in the first 200 kHz channel
    frequencyUsers.clear();
    frequencyUsers.resize(1);
    for (int uid = 1; uid <= usersPerChannel; ++uid) {
        frequencyUsers[0].emplace_back(uid, messagesPerUser);
    }

    {
        std::lock_guard<std::mutex> lock(ioMutex);
        io.outputstring("--- 3G Analysis ---\n");
        io.outputstring("Users per 200kHz: ");
        io.outputint(usersPerChannel);
        io.terminate();

        io.outputstring("Total Users (1 MHz): ");
        io.outputint(totalUsers);
        io.terminate();

        io.outputstring("Users in first 200kHz channel: ");
        for (const auto& u : frequencyUsers[0]) {
            io.outputint(u.getId());
            io.outputstring(" ");
        }
        io.terminate();
        io.terminate();
    }
}

// --- 4G Implementation ---
// 10 kHz sub-channels, 30 users per sub-channel, up to 4 antennas (MIMO).
// Each user still takes 10 messages.
Tower4G::Tower4G(CellularCore* c) : CellTower(1000.0, 4, c) {} // 1000 kHz, 4 antennas

void Tower4G::calculateCapacity() {
    int subChannelWidthKHz = 10;
    int subChannels = (int)(bandwidth / subChannelWidthKHz); // 100 sub-channels

    int usersPerSubChannel = 30;
    int usersPerAntenna = subChannels * usersPerSubChannel;
    int totalUsers = usersPerAntenna * antennas; // reuse channels via antennas

    int messagesPerUser = 10;
    core->addTraffic(totalUsers * messagesPerUser);

    // Users in the first 10 kHz sub-channel (independent of antenna index)
    frequencyUsers.clear();
    frequencyUsers.resize(1);
    for (int uid = 1; uid <= usersPerSubChannel; ++uid) {
        frequencyUsers[0].emplace_back(uid, messagesPerUser);
    }

    {
        std::lock_guard<std::mutex> lock(ioMutex);
        io.outputstring("--- 4G Analysis ---\n");
        io.outputstring("Sub-channels (10kHz): ");
        io.outputint(subChannels);
        io.terminate();

        io.outputstring("Antennas: ");
        io.outputint(antennas);
        io.terminate();

        io.outputstring("Total Users (all antennas): ");
        io.outputint(totalUsers);
        io.terminate();

        io.outputstring("Users in first 10kHz sub-channel: ");
        for (const auto& u : frequencyUsers[0]) {
            io.outputint(u.getId());
            io.outputstring(" ");
        }
        io.terminate();
        io.terminate();
    }
}

// --- 5G Implementation ---
// Base band: similar to 4G but with 16 antennas.
// High-frequency band: extra 10 MHz at 1800 MHz; 30 users per 1 MHz,
// with massive MIMO (16 antennas). Each user: 10 messages.
Tower5G::Tower5G(CellularCore* c) : CellTower(1000.0, 16, c) {} // base band 1 MHz, 16 antennas

void Tower5G::calculateCapacity() {
    // 1. Base band (10 kHz sub-channels, like 4G but more antennas)
    int subChannelWidthKHz = 10;
    int subChannels = (int)(bandwidth / subChannelWidthKHz); // 100 sub-channels
    int usersPerSubChannel = 30;
    int baseUsers = (subChannels * usersPerSubChannel) * antennas;

    // 2. High-frequency band: 10 MHz at 1800 MHz,
    // 30 users per 1 MHz * 16 antennas (massive MIMO)
    int highBandMHz = 10;
    int usersPerMHz = 30 * antennas;
    int highBandUsers = highBandMHz * usersPerMHz;

    int totalUsers = baseUsers + highBandUsers;

    int messagesPerUser = 10;
    core->addTraffic(totalUsers * messagesPerUser);

    // Users in the first 1 MHz high-frequency channel at 1800 MHz
    frequencyUsers.clear();
    frequencyUsers.resize(1);
    for (int uid = 1; uid <= usersPerMHz; ++uid) {
        frequencyUsers[0].emplace_back(uid, messagesPerUser);
    }

    {
        std::lock_guard<std::mutex> lock(ioMutex);
        io.outputstring("--- 5G Analysis ---\n");
        io.outputstring("Total Users (base + high band): ");
        io.outputint(totalUsers);
        io.terminate();

        io.outputstring("Users in first 1MHz high-frequency channel: ");
        for (const auto& u : frequencyUsers[0]) {
            io.outputint(u.getId());
            io.outputstring(" ");
        }
        io.terminate();
        io.terminate();
    }
}