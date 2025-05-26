#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ControlSystem.h"
#include "Elevator.h"
#include "Sensor.h"
#include "Logger.h"
#include <iostream>
#include "Passengers.h"
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), logger(nullptr), gui(new GUI(this)),  controlSystem(new ControlSystem(gui)) {

    ui->setupUi(this);


    connect(ui->init_sim, SIGNAL(released()), this, SLOT(InitializeSimulation()));
    connect(ui->start, SIGNAL(released()), this, SLOT(startSimulation()));
    connect(ui->add_passenger, SIGNAL(released()), this, SLOT(addPassenger()));
    // ✅ Connect Simulation Control Buttons
    connect(ui->Pause, SIGNAL(released()), this, SLOT(onPauseClicked()));
    connect(ui->Resume, SIGNAL(released()), this, SLOT(onContinueClicked()));
    connect(ui->Stop, SIGNAL(released()), this, SLOT(onStopClicked()));

    // ✅ Connect Safety Event Buttons
    connect(ui->Fire, SIGNAL(released()), this, SLOT(onFireAlarmTriggered()));
    connect(ui->Power_out, SIGNAL(released()), this, SLOT(onPowerOutageTriggered()));

    // ✅ Connect Elevator 1 Buttons
    connect(ui->e1_Help, SIGNAL(released()), this, SLOT(onElevator1Help()));
    connect(ui->e1_door_obstacle, SIGNAL(released()), this, SLOT(onElevator1DoorObstacle()));
    connect(ui->e1_overload, SIGNAL(released()), this, SLOT(onElevator1Overload()));
    connect(ui->e1_open, SIGNAL(released()), this, SLOT(onElevator1OpenDoor()));
    connect(ui->e1_close, SIGNAL(released()), this, SLOT(onElevator1CloseDoor()));

    // ✅ Connect Elevator 2 Buttons
    connect(ui->e2_help, SIGNAL(released()), this, SLOT(onElevator2Help()));
    connect(ui->e2_door_obstacle, SIGNAL(released()), this, SLOT(onElevator2DoorObstacle()));
    connect(ui->e2_overload, SIGNAL(released()), this, SLOT(onElevator2Overload()));
    connect(ui->e2_open, SIGNAL(released()), this, SLOT(onElevator2OpenDoor()));
    connect(ui->e2_close, SIGNAL(released()), this, SLOT(onElevator2CloseDoor()));
    connect(ui->e1_safety, SIGNAL(released()), this, SLOT(onElevator1Safety()));
    connect(ui->e2_safety, SIGNAL(released()), this, SLOT(onElevator2Safety()));
}

MainWindow::~MainWindow() {
    delete ui;
    delete controlSystem;
    delete gui;
    delete logger;
}

void MainWindow::InitializeSimulation() {
    totalFloors = ui->num_floors->value();
    totalElevators = ui->num_elevators->value();

    // ✅ Convert QList to std::list
    std::list<Passengers *> stdPassengers(passengers.begin(), passengers.end());

    controlSystem->setupSimulation(totalFloors, totalElevators, stdPassengers);  // ✅ Now it matches std::list

    passengers.clear();  // Reset passenger list
    ui->start->setEnabled(true);  // Enable start after initialization
}

void MainWindow::startSimulation() {
    globalTimeStep = 0;  // ✅ Reset time when simulation starts
    checkPassengerRequests();
}

void MainWindow::addPassenger() {
    int startFloorNum = ui->start_floor->value();
    QString direction = ui->direction->currentText();
    int requestTime = ui->request_time->value();
    int destinationFloorNum = ui->dest_floor->value();

    Floor* startFloor = controlSystem->getFloor(startFloorNum);
    Floor* destinationFloor = controlSystem->getFloor(destinationFloorNum);

    if (!startFloor || !destinationFloor) {
           gui->logger("Error: Invalid floor selection for passenger.");
            return;
        }
    Passengers* newPassenger = new Passengers(passengers.size() + 1, startFloor, destinationFloor, controlSystem, gui, requestTime);
    controlSystem->addPassenger(newPassenger);  // Store by value
    passengers.append(newPassenger);

    std::string passengerEntry = "Floor " + std::to_string(startFloorNum) + " " +
                             " at t=" + std::to_string(requestTime) + " -> Dest: " +
                             std::to_string(destinationFloorNum);

    gui->logger("Passenger added: " + passengerEntry);
}

void MainWindow::onPauseClicked() {
    gui->logger("Simulation paused.");
    for (Elevator* elevator : controlSystem->getElevators()) {
            elevator->pauseElevator();
    }
}

void MainWindow::onContinueClicked() {
    gui->logger("Simulation paused.");
    for (Elevator* elevator : controlSystem->getElevators()) {
            elevator->resumeElevator();
    }
}

void MainWindow::onStopClicked() {
    gui->logger("Simulation stopped.");
    for (Elevator* elevator : controlSystem->getElevators()) {
            elevator->stopElevatorThread();
            exit(0);
    }
}

// ✅ Safety Event Slots
void MainWindow::onFireAlarmTriggered() {
    controlSystem->handleSafetyEvent(SafetyEvent::FIRE, nullptr);
}

void MainWindow::onPowerOutageTriggered() {
    controlSystem->handleSafetyEvent(SafetyEvent::POWER_OUTAGE, nullptr);
}

// ✅ Elevator 1 Slots
void MainWindow::onElevator1Help() {
    controlSystem->handleSafetyEvent(SafetyEvent::HELP, controlSystem->getElevator(1));
}

void MainWindow::onElevator1DoorObstacle() {
    controlSystem->getElevator(1)->getSensor()->detectDoorObstacle(1);
}

void MainWindow::onElevator1Overload() {
    controlSystem->handleSafetyEvent(SafetyEvent::OVERLOAD, controlSystem->getElevator(1));
}

void MainWindow::onElevator1OpenDoor() {
    controlSystem->getElevator(1)->openDoors(globalTimeStep);
}

void MainWindow::onElevator1CloseDoor() {
    controlSystem->getElevator(1)->closeDoors(globalTimeStep);
}

// ✅ Elevator 2 Slots
void MainWindow::onElevator2Help() {
    controlSystem->handleSafetyEvent(SafetyEvent::HELP, controlSystem->getElevator(2));
}

void MainWindow::onElevator2DoorObstacle() {
    controlSystem->getElevator(2)->getSensor()->detectDoorObstacle(2);
}

void MainWindow::onElevator2Overload() {
    controlSystem->handleSafetyEvent(SafetyEvent::OVERLOAD, controlSystem->getElevator(2));
}

void MainWindow::onElevator2OpenDoor() {
    controlSystem->getElevator(2)->openDoors(globalTimeStep);
}

void MainWindow::onElevator2CloseDoor() {
    controlSystem->getElevator(2)->closeDoors(globalTimeStep);
}

void MainWindow::checkPassengerRequests() {
    for (Passengers* p : passengers) {
        if (p->getRequestTime() == globalTimeStep && !p->getAdded()) {
            p->setAdded();
            requests.push_back(p);
        }
    }
    controlSystem->requestElevator(globalTimeStep, requests);  // ✅ Pass global time step
    requests.clear();
    globalTimeStep = controlSystem->getGlobalTimeStep() - 1;
    globalTimeStep++;

    QTimer::singleShot(1000, this, SLOT(checkPassengerRequests()));
}

void MainWindow::onElevator1Safety(){
    gui->logger("Simulation continued.");

    // ✅ Resume all paused elevators
    controlSystem->getElevator(1)->resumeElevator();
}


void MainWindow::onElevator2Safety(){
    gui->logger("Simulation continued.");
    controlSystem->getElevator(2)->resumeElevator();
}
