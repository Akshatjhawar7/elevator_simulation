#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include "ControlSystem.h"
#include "GUI.h"
#include "Logger.h"
#include "Passengers.h"


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void updateSimulationStep();
    Logger* getLogger();
    void logToConsole(const QString& message);

private slots:
    void InitializeSimulation();
    void startSimulation();
    void addPassenger();
    void onPauseClicked();
    void onContinueClicked();
    void onStopClicked();

    void onFireAlarmTriggered();
    void onPowerOutageTriggered();

    void onElevator1Help();
    void onElevator1DoorObstacle();
    void onElevator1Overload();
    void onElevator1OpenDoor();
    void onElevator1CloseDoor();

    void onElevator2Help();
    void onElevator2DoorObstacle();
    void onElevator2Overload();
    void onElevator2OpenDoor();
    void onElevator2CloseDoor();
    void checkPassengerRequests();
//    void logMessage(const QString&);
    void onElevator1Safety();
    void onElevator2Safety();


private:
    Ui::MainWindow *ui;
    Logger* logger;
    GUI* gui;
    ControlSystem* controlSystem;
    int globalTimeStep = 0;
    int totalFloors;
    int totalElevators;
    int totalPassengers;

    struct Passenger {
       int startFloor;
       QString direction;
       int requestTime;
       int destinationFloor;
    };

    QList<Passengers*> passengers;
    std::list<Passengers*> requests;
};

#endif
