#ifndef PASSENGERS_H
#define PASSENGERS_H

#include <string>
#include "Floor.h"
class ControlSystem;
#include "GUI.h"

class Passengers {
private:
    int id;
    Floor* startFloor;
    Floor* destinationFloor;
    ControlSystem* controlSystem;
    GUI* gui;
    int requestTime;
    bool arrived;
    bool added;

public:
    Passengers(int passengerId, Floor* start, Floor* destination, ControlSystem* control, GUI* guiInstance, int requestTime);
    void requestElevator(int timeStep);
    void enterElevator();
    void exitElevator();
    int getId() const;
    int getRequestTime() const;
    int getDestinationFloor() const;
    bool hasReachedDestination();
    void hasArrived(bool);
    int getStartFloor();  
    void setAdded();
    bool getAdded();
};

#endif // PASSENGERS_H
