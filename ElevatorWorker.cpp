#include "ElevatorWorker.h"
#include "Elevator.h"

void ElevatorWorker::runElevator() {
    elevator->stepTowardsTarget(elevator->getCurrentFloor());
}
