#include "Passengers.h"
#include "ControlSystem.h"
#include "GUI.h"

Passengers::Passengers(int passengerId, Floor* start, Floor* destination, ControlSystem* control, GUI* guiInstance, int requestTime)
    : id(passengerId), startFloor(start), destinationFloor(destination), controlSystem(control), gui(guiInstance), requestTime(requestTime), arrived(false), added(false) {}

int Passengers::getRequestTime() const {
    return requestTime;
}

int Passengers::getId() const {
    return id;
}

int Passengers::getDestinationFloor() const {
    return destinationFloor->getNumber();
}

int Passengers::getStartFloor(){

    return startFloor->getNumber();
}
void Passengers::exitElevator() {
    gui->logger("Passenger " + std::to_string(id) + " has reached their destination.");
}

void Passengers::hasArrived(bool flag){
    arrived = flag;
}


bool Passengers::hasReachedDestination(){
    return arrived;
}

void Passengers::setAdded(){
    added = true;
}

bool Passengers::getAdded(){
    return added;
}
