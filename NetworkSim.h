/* NetworkSim.h */
#ifndef NETWORK_SIM_H
#define NETWORK_SIM_H

#include "Basicio.h"
#include <mutex>  // <--- FIXED: Needed for std::mutex
#include <thread> // <--- Added for threading support
#include <vector>

// Custom Exception
class SimulationException {
private:
    const char* message;
public:
    SimulationException(const char* msg) : message(msg) {}
    const char* getMessage() const { return message; }
};

// Template Class for a Frequency Channel
template <typename T>
class Channel {
private:
    T* users;
    int capacity;
    int currentCount;
    double frequencyBand;

public:
    Channel(int cap, double band) : capacity(cap), currentCount(0), frequencyBand(band) {
        users = new T[capacity];
    }
    
    ~Channel() {
        delete[] users;
    }

    bool addUser(T user) {
        if (currentCount < capacity) {
            users[currentCount++] = user;
            return true;
        }
        return false;
    }

    int getCount() const { return currentCount; }
    int getCapacity() const { return capacity; }
};

// --- Base Classes ---

class UserDevice {
private:
    int id;
    int messageCount;
public:
    UserDevice() : id(0), messageCount(0) {}
    UserDevice(int uid, int msgs) : id(uid), messageCount(msgs) {}
    int getId() const { return id; }
    int getMessages() const { return messageCount; }
};

class CellularCore {
private:
    long totalMessagesProcessed;
    const long MESSAGE_LIMIT_PER_CORE = 100000;
    std::mutex coreMutex; // <--- The Lock

public:
    CellularCore();
    void processMessages(int count); // Thread-safe adder
    int getRequiredCores();          // Thread-safe getter
};

class CellTower {
protected:
    double totalBandwidth;
    int antennas;
    CellularCore* core;
    
public:
    CellTower(double bandwidth, int numAntennas, CellularCore* c);
    virtual ~CellTower() {}
    
    virtual void calculateCapacity() = 0; 
    virtual void printStats() = 0;
    
    void printInt(int num);
    void printStr(const char* str);
};

// --- Derived Classes ---

class Tower2G : public CellTower {
private:
    const double CHANNEL_WIDTH = 200.0;
    const int USERS_PER_CHANNEL = 16;
    const int MSGS_PER_USER = 20;
public:
    Tower2G(CellularCore* c);
    void calculateCapacity() override;
    void printStats() override;
};

class Tower3G : public CellTower {
private:
    const double CHANNEL_WIDTH = 200.0; 
    const int USERS_PER_CHANNEL = 32;
    const int MSGS_PER_USER = 10;
public:
    Tower3G(CellularCore* c);
    void calculateCapacity() override;
    void printStats() override;
};

class Tower4G : public CellTower {
private:
    const double CHANNEL_WIDTH = 10.0;
    const int USERS_PER_CHANNEL = 30;
    const int MSGS_PER_USER = 10;
public:
    Tower4G(CellularCore* c);
    void calculateCapacity() override;
    void printStats() override;
};

class Tower5G : public CellTower {
private:
    const double HIGH_FREQ_BANDWIDTH = 10000.0;
    const int USERS_PER_MHZ_HIGH = 30;
public:
    Tower5G(CellularCore* c);
    void calculateCapacity() override;
    void printStats() override;
};

#endif