#ifndef SAFETYSYSTEM_H
#define SAFETYSYSTEM_H

#include "SafetyEvent.h"
#include "Elevator.h"
#include "GUI.h"

class ControlSystem; // Forward declaration

class SafetySystem {
private:
    ControlSystem* controlSystem;
    GUI* gui;

public:
    SafetySystem(ControlSystem* control, GUI* guiInstance);
    void handleSafetyEvent(SafetyEvent event, Elevator* elevator);
};

#endif // SAFETYSYSTEM_H
