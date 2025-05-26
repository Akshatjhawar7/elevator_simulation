#include "ControlSystem.h"
#include "Elevator.h"
#include "Floor.h"
#include "Passengers.h"
#include "SafetySystem.h"
#include <iostream>

ControlSystem::ControlSystem(GUI* guiInstance) : gui(guiInstance), currentGlobalTimeStep(0) {
    safetySystem = new SafetySystem(this, gui);
}

ControlSystem::~ControlSystem() {
    for (Elevator* elevator : elevators) {
        delete elevator;
    }
    for (Floor* floor : floors) {
        delete floor;
    }
    delete safetySystem;
}

void ControlSystem::setupSimulation(int totalFloors, int totalElevators, std::list<Passengers*>& allPassengers) {
    for (int i = 1; i <= totalFloors; i++) {
        floors.push_back(new Floor(i, this, gui));
    }

    for (int i = 1; i <= totalElevators; i++) {
        elevators.push_back(new Elevator(i, this, gui));
    }
    passengers = allPassengers;

    gui->logger("Simulation setup complete: " + std::to_string(totalFloors) + " floors, " +
                std::to_string(totalElevators) + " elevators initialized.");
}

Floor* ControlSystem::getFloor(int floorNumber) {
    for (Floor* floor : floors) {
        if (floor->getNumber() == floorNumber)
            return floor;
    }
    return nullptr;
}

Elevator* ControlSystem::getElevator(int elevatorId) {
    for (Elevator* elevator : elevators) {
        if (elevator->getId() == elevatorId)
            return elevator;
    }
    return nullptr;
}

void ControlSystem::handleSafetyEvent(SafetyEvent event, Elevator* elevator) {
    safetySystem->handleSafetyEvent(event, elevator);
}

void ControlSystem::requestElevator(int timeStep, std::list<Passengers*> passenger_requests) {
        for(Passengers* p : passenger_requests){
            gui->logger("ControlSystem: Elevator requested at Floor " + std::to_string(p->getStartFloor()));
            pendingRequests.push_back({p->getStartFloor(), p->getDestinationFloor(), p->getRequestTime(), p});
        }
        updateElevator(timeStep);
        return;
}

const std::list<Elevator*>& ControlSystem::getElevators() const {
    return elevators;
}

void ControlSystem::setGlobalTimeStep(int timeStep) {
    currentGlobalTimeStep = timeStep;
}

int ControlSystem::getGlobalTimeStep() const {
    return currentGlobalTimeStep;
}

Elevator* ControlSystem::getElevatorAtFloor(int floorNumber) {
    for (Elevator* elevator : elevators) {
        if (elevator->getCurrentFloor() == floorNumber) {
            return elevator;
        }
    }
    return nullptr;
}

void ControlSystem::updateElevator(int timeStep) {
    if (pendingRequests.empty()) {
        bool allPassengersDone = true;
        for (Passengers* p : passengers) {
            if (!p->hasReachedDestination()) {
                allPassengersDone = false;
                break;
            }
        }
        if (allPassengersDone) {
            bool allElevatorsDone = true;
            for(Elevator* elevator : elevators){
                if(elevator->getState() != "idle"){
                    allElevatorsDone = false;
                    break;
                }
            }
            if(allElevatorsDone){
                for(Elevator* elevator : elevators){
                    elevator->stopElevatorThread();
                }
                gui->logger("Simulation complete: All passengers have reached their destinations.");
                return;
            }
        }
        return;
    }
    for (Elevator* elevator : elevators) {
        if (elevator->getState() == "idle" && !pendingRequests.empty()) {
            ElevatorRequest request = pendingRequests.front();
            pendingRequests.pop_front();  // ✅ Remove request from queue
            gui->logger("Assigning Elevator " + std::to_string(elevator->getId()) +
                        " to Floor " + std::to_string(request.floorNumber));

            transported(request.passenger);

            elevator->setState("pick_up");
            elevator->setPickupFloor(request.floorNumber);
            elevator->setTargetFloor(request.destFloor);
            elevator->startElevatorThread(timeStep + 1);
            return;
        }
    }
}

SafetySystem* ControlSystem::getSafetySystem(){
    return safetySystem;
}

void ControlSystem::transported(Passengers* passenger){
    for(Passengers* p : passengers){
        if(p->getId() == passenger->getId()){
            p->hasArrived(true);
        }
    }
}

void ControlSystem::addPassenger(Passengers* passenger){
    passengers.push_back(passenger);
}

void ControlSystem::setSafety(){
    safety = true;
}


bool ControlSystem::getSafety(){
    return safety;
}

