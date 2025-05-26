#include "Building.h"
#include "ControlSystem.h"
#include <iostream>

void Building::setupSimulation(ControlSystem* controlsystem) {
    for (int i = 1; i <= 5; i++) {  // Example: 5 floors
        floors.push_back(Floor(i, controlsystem));  // ✅ Pass ControlSystem*
    }

    elevators.push_back(Elevator(1));
    elevators.push_back(Elevator(2));

    std::cout << "Simulation setup complete: 5 floors, 2 elevators initialized." << std::endl;
}

void Building::startSimulation() {
    std::cout << "Simulation started. Awaiting user input..." << std::endl;
}
