#include "Sensor.h"
#include "ControlSystem.h"
#include "Elevator.h"

Sensor::Sensor(GUI* gui, Elevator* elev, SafetySystem* safetySys) : elevator(elev), safetySystem(safetySys), gui(gui) {}

void Sensor::detectDoorObstacle(int elevatorId) {
    gui->logger("Sensor: Door obstacle detected in Elevator " + std::to_string(elevatorId));
    if (elevator) {
        elevator->notifyControlSystem(SafetyEvent::DOOR_OBSTACLE);
    }
}

void Sensor::detectOverload(int elevatorId) {
    gui->logger("Sensor: Overload detected in Elevator " + std::to_string(elevatorId));
    if (elevator) {
        elevator->notifyControlSystem(SafetyEvent::OVERLOAD);
    }
}

void Sensor::detectFireAlarm() {
    gui->logger("Sensor: Fire alarm triggered.");
    elevator->notifyControlSystem(SafetyEvent::FIRE);
}

void Sensor::detectPowerOutage() {
    gui->logger("Sensor: Power outage detected.");
    elevator->notifyControlSystem(SafetyEvent::POWER_OUTAGE);
}
