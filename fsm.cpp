#include <iostream>
#include "fsm.hpp"
#include <string>
#include <cstdint>
#include <chrono>
#include <vector>
#include <thread>

using namespace std;
// Fungsi global untuk waktu saat ini (dalam milidetik)
uint32_t millis() {
    static auto start = chrono::steady_clock::now();
    auto now = chrono::steady_clock::now();
    return chrono::duration_cast<chrono::milliseconds>(now - start).count();
}

// Konstruktor default
FSM::FSM() : currentState(SystemState::INIT), lastHeartbeat(0), delay(0), errorCount(0), moveCount(0) {
    stateHistory.clear();
}

// Konstruktor dengan delay
FSM::FSM(uint32_t delay) : currentState(SystemState::INIT), lastHeartbeat(0), delay(delay), errorCount(0), moveCount(0) {
    stateHistory.clear();
}
// Destruktor
FSM::~FSM() {
    stateHistory.clear();
}

// Getter dan Setter
SystemState FSM::getCurrentState() const {
    return currentState;
}

void FSM::transitionToState(SystemState newState) {
    currentState = newState;
    lastHeartbeat = millis();
    addStateToHistory(currentState, lastHeartbeat);
}

void FSM::setDelay(uint32_t d) {
    delay = d;
}

void FSM::getDelay(uint32_t &d) const {
    d = delay;
}

void FSM::setErrorCount(int count) {
    errorCount = count;
}

int FSM::getErrorCount() const {
    return errorCount;
}

void FSM::setMoveCount(int count) {
    moveCount = count;
}

int FSM::getMoveCount() const {
    return moveCount;
}

void FSM::addStateToHistory(SystemState state, uint32_t time) {
    stateHistory.emplace_back(state, time);
}

vector<pair<SystemState, uint32_t>> FSM::getStateHistory() const {
    return stateHistory;
}

uint32_t FSM::getLastHeartbeat() const {
    return lastHeartbeat;
}

void FSM::setLastHeartbeat(uint32_t heartbeat) {
    lastHeartbeat = heartbeat;
}

// Fungsi utama untuk menjalankan FSM
void FSM::start() {
    while (currentState != SystemState::STOPPED) {
        update();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

// Update state sesuai currentState
void FSM::update() {
    switch (currentState) {
        case SystemState::INIT:
            performInit();
            break;
        case SystemState::IDLE:
            performProcess();
            break;
        case SystemState::MOVEMENT:
            performMovement();
            break;
        case SystemState::SHOOTING:
            performShooting();
            break;
        case SystemState::CALCULATION:
            performCalculation();
            break;
        case SystemState::ERROR:
            performErrorHandling();
            break;
        case SystemState::STOPPED:
            shutdown();
            break;
    }
    lastHeartbeat = millis();
    addStateToHistory(currentState, lastHeartbeat);
}

// Cetak status FSM saat ini
void FSM::printStatus() {
    cout << "Current State: ";
    switch (currentState) {
        case SystemState::INIT: cout << "INIT"; break;
        case SystemState::IDLE: cout << "IDLE"; break;
        case SystemState::MOVEMENT: cout << "MOVEMENT"; break;
        case SystemState::SHOOTING: cout << "SHOOTING"; break;
        case SystemState::CALCULATION: cout << "CALCULATION"; break;
        case SystemState::ERROR: cout << "ERROR"; break;
        case SystemState::STOPPED: cout << "STOPPED"; break;
    }
    cout << "\nLast Heartbeat: " << lastHeartbeat
         << "\nDelay: " << delay
         << "\nError Count: " << errorCount
         << "\nMove Count: " << moveCount << endl;
}

//Print state history
void FSM::printStateHistory() {
    cout << "State History:\n";
    for (const auto& entry : stateHistory) {
        cout << "State: ";
        switch (entry.first) {
            case SystemState::INIT: cout << "INIT"; break;
            case SystemState::IDLE: cout << "IDLE"; break;
            case SystemState::MOVEMENT: cout << "MOVEMENT"; break;
            case SystemState::SHOOTING: cout << "SHOOTING"; break;
            case SystemState::CALCULATION: cout << "CALCULATION"; break;
            case SystemState::ERROR: cout << "ERROR"; break;
            case SystemState::STOPPED: cout << "STOPPED"; break;
        }
        cout << " at " << entry.second << " ms" << endl;
    }
}
// INIT → IDLE
void FSM::performInit() {
    cout << "Initializing system..." << endl;
    delay = 1000;
    transitionToState(SystemState::IDLE);
    printStatus();
}

// Pilih proses dari user input
void FSM::performProcess() {
    cout << "\nChoose an action:\n"
         << "1. Show status and history (IDLE)\n"
         << "2. Move (MOVEMENT)\n"
         << "3. Shoot (SHOOTING)\n"
         << "4. Calculate (CALCULATION)\n"
         << "Choice: ";
    
    int choice;
    cin >> choice;

    switch (choice) {
        case 1:
            printStatus();
            printStateHistory();
            break;
        case 2:
            transitionToState(SystemState::MOVEMENT);
            break;
        case 3:
            transitionToState(SystemState::SHOOTING);
            break;
        case 4:
            transitionToState(SystemState::CALCULATION);
            break;
        default:
            cout << "Invalid input. Try again.\n";
            break;
    }
}

// MOVEMENT
void FSM::performMovement() {
    cout << "Moving..." << endl;
    moveCount++;
    if (moveCount >= 3) {
        transitionToState(SystemState::SHOOTING);
    } else {
        transitionToState(SystemState::IDLE);
    }
    lastHeartbeat = millis();
}

// SHOOTING
void FSM::performShooting() {
    cout << "Shooting..." << endl;
    moveCount = 0;
    transitionToState(SystemState::IDLE);
    lastHeartbeat = millis();
}

// CALCULATION
void FSM::performCalculation() {
    cout << "Performing calculation..." << endl;
    if (moveCount == 0) {
        transitionToState(SystemState::ERROR);
    } else {
        transitionToState(SystemState::IDLE);
    }
}

// ERROR
void FSM::performErrorHandling() {
    cout << "Error occurred, performing error handling..." << endl;
    errorCount++;
    if (errorCount > 3) {
        transitionToState(SystemState::STOPPED);
    } else {
        transitionToState(SystemState::IDLE);
    }
}

// STOPPED
void FSM::shutdown() {
    cout << "System stopped, shutting down..." << endl;
    stateHistory.clear();
}