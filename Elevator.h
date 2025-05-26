#ifndef ELEVATOR_H
#define ELEVATOR_H

#include <string>
#include "Passengers.h"
class ControlSystem; // Forward declaration
class Sensor;
class GUI;
#include "SafetyEvent.h"
#include <QThread>
#include <QObject>
#include <QMutex>
#include <QWaitCondition>

class Elevator: public QObject{
    Q_OBJECT
private:
    ControlSystem* controlSystem;
    GUI* gui;
    int id;
    int currentFloor;
    std::string state; // "idle", "moving"
    std::string direction; // "up" or "down"
    bool openButton;
    bool closeButton;
    int targetFloor; // Store the destination floor
    int pickupFloor;
    Sensor* sensor;
    int timeStep;
    QThread* workerThread;
    QMutex mutex;
    QWaitCondition condition;
    bool isPaused = false;

public:
    Elevator(int elevatorId, ControlSystem* control, GUI* guiInstance);
    ~Elevator();
    void moveToFloor(int currentTimeStep);
    void stepTowardsTarget(int currentTimeStep);
    void openDoors(int);
    void closeDoors(int);
    void notifyControlSystem(SafetyEvent event);
    int getId() const;
    int getCurrentFloor() const;
    std::string getState() const;
    void setTargetFloor(int floor);
    int getTargetFloor() const;
    Sensor* getSensor();
    void setState(std::string);
    void setPickupFloor(int);
    void startElevatorThread(int);
    void stopElevatorThread();
    void setTimeStep(int);
    int getTimeStep();
    void pauseElevator();
    void resumeElevator();

public slots:
    void runElevator();

signals:
    void finished();
};

#endif // ELEVATOR_H
