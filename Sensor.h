#ifndef SENSOR_H
#define SENSOR_H

#include "SafetyEvent.h"
#include "GUI.h"
class Elevator;
class SafetySystem;

class Sensor {
private:
    Elevator* elevator;
    SafetySystem* safetySystem;
    GUI* gui;

public:
    Sensor(GUI*, Elevator* elev, SafetySystem* safetySys);
    void detectOverload(int);
    void detectDoorObstacle(int elevatorId);
    void detectFireAlarm();
    void detectPowerOutage();
};

#endif // SENSOR_H
