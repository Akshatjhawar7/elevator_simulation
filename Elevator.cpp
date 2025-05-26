#include "Elevator.h"
#include "ControlSystem.h"
#include "GUI.h"
#include "Sensor.h"
#include <QTimer>

Elevator::Elevator(int elevatorId, ControlSystem* control, GUI* guiInstance)
    : QObject(), controlSystem(control), gui(guiInstance), id(elevatorId), currentFloor(1), state("idle"), direction(""), targetFloor(1), pickupFloor(1), isPaused(false){
    sensor = new Sensor(gui, this, controlSystem->getSafetySystem());
    workerThread = new QThread(this);
    moveToThread(workerThread);

    connect(workerThread, SIGNAL(started()), this, SLOT(runElevator()));
    connect(this, SIGNAL(finished()), workerThread, SLOT(quit()));
}

Elevator::~Elevator() {
    delete sensor;
    if(workerThread->isRunning()) {
        stopElevatorThread();
    }
}

void Elevator::moveToFloor(int currentTimeStep) {
    timeStep = currentTimeStep;
    controlSystem->setGlobalTimeStep(currentTimeStep);

    mutex.lock();
    while (isPaused) {
       gui->logger("Elevator " + std::to_string(id) + " waiting for RESUME.");
       condition.wait(&mutex);
    }
    mutex.unlock();

    if (currentFloor == targetFloor) {
        gui->logger("t=" + std::to_string(currentTimeStep) + ": Elevator " +
                    std::to_string(id) + " arrived at Floor " + std::to_string(currentFloor));
        openDoors(currentTimeStep);
        return;
    }
    currentFloor += (currentFloor < targetFloor) ? 1 : -1;
    gui->logger("t=" + std::to_string(currentTimeStep) + ": Elevator " +
                std::to_string(id) + " now at Floor " + std::to_string(currentFloor));
    QTimer::singleShot(1000, this, [this, currentTimeStep]() {
        moveToFloor(currentTimeStep + 1);
    });
}

void Elevator::stepTowardsTarget(int currentTimeStep) {
    timeStep = currentTimeStep;
    controlSystem->setGlobalTimeStep(currentTimeStep);
    if(currentFloor != pickupFloor){
        currentFloor += (currentFloor < pickupFloor) ? 1 : -1;
        gui->logger("t=" + std::to_string(currentTimeStep) + ": Elevator " + std::to_string(id) + " now at Floor " + std::to_string(currentFloor));
    }
    if (currentFloor == pickupFloor) {
        gui->logger("t=" + std::to_string(currentTimeStep) + ": Elevator " + std::to_string(id) + " arrived at Floor " + std::to_string(currentFloor));
        openDoors(currentTimeStep);
        return;
    }
    controlSystem->setGlobalTimeStep(currentTimeStep+1);
    stepTowardsTarget(currentTimeStep + 1);
}

void Elevator::openDoors(int currentTimeStep) {
    if(state == "overload_obstacle"){
        pauseElevator();
    }
    timeStep = currentTimeStep;
    controlSystem->setGlobalTimeStep(currentTimeStep);
    gui->logger("t=" + std::to_string(currentTimeStep) + ": Elevator " + std::to_string(id) + " doors opening.");
    if(state == "emergency"){
        gui->logger("All passengers off");
        gui->logger("Simulation over");
        stopElevatorThread();
        return;
    }
    if (state == "help") {
        gui->logger("Exit if needed, elevator temporarily suspended");
        QTimer::singleShot(5000, this, [this]() {
            gui->logger("t=" + std::to_string(timeStep + 5) + ": No response heard, calling 911");
            stopElevatorThread();
        });
        return;
    }
    closeDoors(currentTimeStep);
}

void Elevator::closeDoors(int currentTimeStep) {
    timeStep = currentTimeStep;
    controlSystem->setGlobalTimeStep(currentTimeStep);
    if(state == "moving"){
        gui->logger("Passenger dropped");
        gui->logger("Elevator " + std::to_string(id) + " doors closing.");
        setState("idle");
        controlSystem->updateElevator(currentTimeStep+1);
        return;
    }
    else if(state == "pick_up"){
        gui->logger("Passenger picked up");
        gui->logger("Elevator " + std::to_string(id) + " doors closing.");
        setState("moving");
        moveToFloor(currentTimeStep+1);
    }
}


void Elevator::notifyControlSystem(SafetyEvent event) {
    controlSystem->handleSafetyEvent(event, this);
}

int Elevator::getId() const {
    return id;
}

int Elevator::getCurrentFloor() const {
    return currentFloor;
}

std::string Elevator::getState() const {
    return state;
}

void Elevator::setTargetFloor(int floor) {
    targetFloor = floor;
}

int Elevator::getTargetFloor() const {
    return targetFloor;
}

Sensor* Elevator::getSensor(){
    return sensor;
}

void Elevator::setState(std::string current_state){
    state = current_state;
}

void Elevator::setPickupFloor(int floor){
    pickupFloor = floor;
}

void Elevator::startElevatorThread(int time){   
    setTimeStep(time);

        // ✅ Stop thread if it's already running
        if(workerThread->isRunning()) {
            stepTowardsTarget(timeStep);
        }
        workerThread->start();
}

void Elevator::runElevator() {
    stepTowardsTarget(timeStep);
}

void Elevator::stopElevatorThread(){
    workerThread->quit();
}

void Elevator::setTimeStep(int time){
    timeStep = time;
}

int Elevator::getTimeStep(){
    return timeStep;
}

void Elevator::pauseElevator() {
    mutex.lock();
    isPaused = true;
    gui->logger("Elevator " + std::to_string(id) + " is now PAUSED");
    mutex.unlock();
}

void Elevator::resumeElevator() {
    mutex.lock();
    isPaused = false; // 🚀 Reset the paused flag
    gui->logger("Elevator " + std::to_string(id) + " is now RESUMED.");
    condition.wakeAll(); // ✅ Wake up the paused thread
    setState("moving");
    mutex.unlock();
}
