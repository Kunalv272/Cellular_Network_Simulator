/* NetworkSim.cpp */
#include "NetworkSim.h"
std::mutex printMutex;
// Helper for String Conversion
void itoa(int n, char* s) {
    int i = 0, j = 0, sign = n;
    if ((sign = n) < 0) n = -n;
    do {
        s[i++] = n % 10 + '0';
    } while ((n /= 10) > 0);
    if (sign < 0) s[i++] = '-';
    s[i] = '\0';
    for (j = 0; j < i / 2; j++) {
        char temp = s[j];
        s[j] = s[i - 1 - j];
        s[i - 1 - j] = temp;
    }
}

// --- CellularCore (Thread-Safe) ---
CellularCore::CellularCore() : totalMessagesProcessed(0) {}

void CellularCore::processMessages(int count) {
    // Lock the mutex so only one thread can add at a time
    std::lock_guard<std::mutex> lock(coreMutex);
    totalMessagesProcessed += count;
}

int CellularCore::getRequiredCores() {
    // Lock while reading to ensure we don't read while someone else writes
    std::lock_guard<std::mutex> lock(coreMutex);
    if (totalMessagesProcessed == 0) return 1;
    return (totalMessagesProcessed + MESSAGE_LIMIT_PER_CORE - 1) / MESSAGE_LIMIT_PER_CORE;
}

// --- CellTower ---
CellTower::CellTower(double bw, int ant, CellularCore* c) 
    : totalBandwidth(bw), antennas(ant), core(c) {}

void CellTower::printStr(const char* str) {
    print((char*)str); 
}

void CellTower::printInt(int num) {
    char buffer[20];
    itoa(num, buffer);
    print(buffer);
}



