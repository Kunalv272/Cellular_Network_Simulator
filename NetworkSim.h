#pragma once

#include "basicIO.h"
#include <mutex>
#include <vector>
#include <thread>

// Custom Exception for the project
class NetworkException {
private:
    const char* msg;
public:
    NetworkException(const char* m) : msg(m) {}
    const char* getMessage() const { return msg; }
};

// 1. Template Class: Channel (Data Abstraction)
template <typename T>
class Channel {
private:
    T* users;
    int capacity;
    int currentCount;
    double frequency; // in kHz or MHz (abstract)
public:
    Channel(int cap, double freq) : capacity(cap), currentCount(0), frequency(freq) {
        users = new T[capacity];
    }
    ~Channel() { delete[] users; }

    bool addUser(T user) {
        if (currentCount < capacity) {
            users[currentCount++] = user;
            return true;
        }
        return false;
    }
    int getCount() const { return currentCount; }
    double getFrequency() const { return frequency; }
};

// User Device Class
class UserDevice {
private:
    int id;
    int messages;
public:
    UserDevice() : id(0), messages(0) {}
    UserDevice(int uid, int msgs) : id(uid), messages(msgs) {}

    int getMessages() const { return messages; }
    int getId() const { return id; }
};

// Cellular Core (The Central Server)
class CellularCore {
private:
    long totalMessages;
    std::mutex coreMutex; // Data Hiding & Thread Safety

    // Per problem statement: overhead is modeled per 100 messages.
    static const long MESSAGES_PER_OVERHEAD_BLOCK = 100;
    // Assume a single core can handle this many "overhead blocks"
    static const long OVERHEAD_BLOCKS_PER_CORE    = 1000; // => 100 * 1000 = 100000 msgs/core

public:
    CellularCore();
    void addTraffic(int msgs);
    int getRequiredCores();
};

// Abstract Base Class (Polymorphism)
class CellTower {
protected:
    double bandwidth;   // in kHz or MHz depending on interpretation
    int antennas;
    CellularCore* core;

    // For "identify the users occupying a specific frequency/channel"
    // we store the users per channel. We only need channel 0 for the assignment.
    std::vector<std::vector<UserDevice>> frequencyUsers;

public:
    CellTower(double bw, int ant, CellularCore* c);
    virtual ~CellTower() {}

    // Pure virtual function
    virtual void calculateCapacity() = 0;

    // Accessor if we ever want to query from outside
    const std::vector<UserDevice>& getUsersInChannel(size_t idx) const;
};

// Derived Classes
class Tower2G : public CellTower {
public:
    Tower2G(CellularCore* c);
    void calculateCapacity() override;
};

class Tower3G : public CellTower {
public:
    Tower3G(CellularCore* c);
    void calculateCapacity() override;
};

class Tower4G : public CellTower {
public:
    Tower4G(CellularCore* c);
    void calculateCapacity() override;
};

class Tower5G : public CellTower {
public:
    Tower5G(CellularCore* c);
    void calculateCapacity() override;
};
