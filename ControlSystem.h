#ifndef CONTROLSYSTEM_H
#define CONTROLSYSTEM_H

#include <list>
#include <string>
#include "Elevator.h"
#include "Floor.h"
#include "Passengers.h"
#include "SafetySystem.h"
#include "Logger.h"
#include "GUI.h"

struct ElevatorRequest {
    int floorNumber;
    int destFloor;
    int timeStep;
    Passengers* passenger;

    ElevatorRequest(int floor, int destination, int time, Passengers* p)
            : floorNumber(floor), destFloor(destination), timeStep(time), passenger(p) {}
};

class ControlSystem {
private:
    std::list<Elevator*> elevators;
    std::list<Floor*> floors;
    std::list<Passengers*> passengers;
    std::list<ElevatorRequest> pendingRequests;
    SafetySystem* safetySystem;
    GUI* gui;
    int currentGlobalTimeStep;
    bool safety = false;

public:
    ControlSystem(GUI* guiInstance);
    ~ControlSystem();

    void setupSimulation(int totalFloors, int totalElevators, std::list<Passengers*>& allPassengers);
    void requestElevator(int timeStep, std::list<Passengers*>);
    void updateElevator(int);
    void handleSafetyEvent(SafetyEvent event, Elevator* elevator);
    void setGlobalTimeStep(int timeStep);
    int getGlobalTimeStep() const;
    const std::list<Elevator*>& getElevators() const;
    Floor* getFloor(int floorNumber);
    Elevator* getElevator(int elevatorId);
    Elevator* getElevatorAtFloor(int floorNumber);
    SafetySystem* getSafetySystem();
    void transported(Passengers*);
    void addPassenger(Passengers*);
    void setSafety();
    bool getSafety();
};

#endif // CONTROLSYSTEM_H
