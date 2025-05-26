#ifndef BUILDING_H
#define BUILDING_H
#include <list>

#include "Elevator.h"
#include "Floor.h"

class Building {
private:
    std::list<Elevator> elevators;
    std::list<Floor> floors;
public:
    void setupSimulation(ControlSystem*);
    void startSimulation();
};

#endif
