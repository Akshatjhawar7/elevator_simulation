#include "SafetySystem.h"
#include "ControlSystem.h"
#include "GUI.h"

SafetySystem::SafetySystem(ControlSystem* control, GUI* guiInstance)
    : controlSystem(control), gui(guiInstance) {}

void SafetySystem::handleSafetyEvent(SafetyEvent event, Elevator* elevator) {
    std::string eventMessage;

    switch (event) {
        case SafetyEvent::FIRE:
            gui->logger("Fire alarm triggered. Moving all elevators to Floor 1.");

            // Stop ongoing movements & trigger emergency mode
            for (Elevator* elevator : controlSystem->getElevators()) {
                elevator->setState("emergency"); // Mark as emergency
                elevator->setTargetFloor(1);     // Force move to Floor 1
                elevator->moveToFloor(elevator->getTimeStep() + 1);
            }
            break;
        case SafetyEvent::POWER_OUTAGE:
            gui->logger("Power outage triggered. Moving all elevators to Floor 1.");

            // Stop ongoing movements & trigger emergency mode
            for (Elevator* elevator : controlSystem->getElevators()) {
                elevator->setState("emergency"); // Mark as emergency
                elevator->setTargetFloor(1);     // Force move to Floor 1
                elevator->moveToFloor(elevator->getTimeStep() + 1);
            }
            break;
        case SafetyEvent::HELP:

            if (elevator) {
                gui->logger("Help requested in Elevator " + std::to_string(elevator->getId()) + ", contacting emergency services.");
                elevator->setState("help");
                elevator->setTargetFloor(elevator->getCurrentFloor()+1);
            }
            break;
        case SafetyEvent::OVERLOAD:
            if (elevator) {
                gui->logger("Overload detected in Elevator " + std::to_string(elevator->getId()) + ", kindly reduce weight");
                elevator->setState("overload_obstacle");
                elevator->openDoors(elevator->getTimeStep()+1); /// Open doors to allow passengers to adjust weight
            }
            break;
        case SafetyEvent::DOOR_OBSTACLE:
            if (elevator) {
                gui->logger("Door obstacle detected in Elevator " + std::to_string(elevator->getId()) + ", kindly remove obstacle");
                elevator->setState("overload_obstacle");
                elevator->openDoors(elevator->getTimeStep()+1);
            }
            break;
    }
}
