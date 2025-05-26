#ifndef FLOOR_H
#define FLOOR_H

class ControlSystem;
#include "GUI.h"

class Floor {
private:
    int number;
    ControlSystem* controlSystem;
    GUI* gui;
    bool upButton;
    bool downButton;

public:
    Floor(int floorNumber, ControlSystem* control, GUI* guiInstance);
    void pressButton(bool goingUp);
    int getNumber() const;
    void resetButton(bool);
};

#endif // FLOOR_H
