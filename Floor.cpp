#include "Floor.h"
#include "ControlSystem.h"
#include "GUI.h"

Floor::Floor(int floorNumber, ControlSystem* control, GUI* guiInstance)
    : number(floorNumber), controlSystem(control), gui(guiInstance), upButton(false), downButton(false) {}

int Floor::getNumber() const {
    return number;
}
void Floor::resetButton(bool goingUp) {
    if (goingUp) {
        upButton = false;
    } else {
        downButton = false;
    }
}
