#ifndef ELEVATORWORKER_H
#define ELEVATORWORKER_H

#include <QObject>

class Elevator;

class ElevatorWorker : public QObject {
    Q_OBJECT
private:
    Elevator* elevator;
public:
    ElevatorWorker(Elevator* e) : elevator(e) {}

public slots:
    void runElevator();
};

#endif // ELEVATORWORKER_H
