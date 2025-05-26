This project is a real-time simulation of an elevator system, built using C++ and Qt. It models the behavior of elevators in a multi-story building, focusing on accurate state transitions, time-based events, and user interactions.

🌟 **Features**
Dynamic elevator movement: Elevators respond to floor requests and move one floor at a time, simulating realistic elevator behavior.

State management: Tracks elevator status (idle, moving, loading/unloading) and current location in real time.

Passenger requests: Handles multiple requests across different floors, ensuring efficient floor service.

GUI Visualization: Provides a user-friendly interface to display elevator locations, directions, and state changes.

Customizable: Easily adjust the number of elevators, floors, and passengers for different simulation scenarios.

Event-Driven Simulation: Uses signals and slots to manage asynchronous events and state transitions.

🛠️ **Tech Stack**
C++ for system logic and simulation.

Qt Framework for graphical interface and event handling.

📚 **Learning Highlights**
Real-time system design: Implemented responsive state transitions and time-based movement logic.

GUI development with Qt: Created an intuitive interface for simulation control and status visualization.

Debugging and testing: Resolved issues in asynchronous behavior, event sequencing, and memory management.

🏃‍♂️ **How to Run**
Prerequisites
Qt (version 5 or 6) installed on your system.

C++ compiler (e.g., g++, clang).

Steps
Clone the repository:

git clone https://github.com/your-username/elevator-system-simulation.git
cd elevator-system-simulation

Open the project in Qt Creator:

Launch Qt Creator.

Open the .pro from the cloned repository.

Let Qt configure the project (you may need to select a kit like Desktop Qt 6.x.x GCC 64-bit).

Build the project:

Click the Build button (🔨) in Qt Creator.

Ensure there are no build errors.

Run the simulation:

Click the Run button (▶️) in Qt Creator.

The GUI will launch, allowing you to simulate elevators, request floors, and watch state changes.

🛠️ Optional: Command-line build (for advanced users)
If you prefer to build from the terminal:

mkdir build && cd build
qmake ../elevator-system-simulation.pro
make
./elevator-system-simulation

📝 Notes
You can adjust the number of elevators, floors, and simulation parameters in the source code for different scenarios.

For any issues, feel free to open an issue or submit a pull request!
