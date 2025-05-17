# Mouse Tracker with Kalman Filter

## Overview
This project implements a Kalman Filter in C++ using OpenCV to track and predict global mouse cursor positions on a Linux system with X11. The Kalman Filter smooths noisy mouse movements, providing accurate predictions of the cursor's position. The application continuously reads the mouse's global coordinates and outputs both the measured and predicted positions to the console.

## Features
- Tracks global mouse cursor position using X11 (`XQueryPointer`).
- Implements a 2D Kalman Filter with a constant velocity model for smooth position estimation.
- Uses OpenCV's `KalmanFilter` class for robust filtering.
- Outputs real-time mouse coordinates and Kalman Filter predictions.

## Prerequisites
- **Operating System**: Linux with X11 (e.g., Ubuntu, Debian).
- **Compiler**: C++11 compatible (e.g., `g++`).
- **Dependencies**:
  - OpenCV 4.x (with `core`, `video`, and `tracking` modules).
  - X11 development libraries (`libx11-dev`).
- **Tools**: CMake (for building the project).

## Installation
1. **Clone the Repository**:
   ```bash
   git clone https://github.com/Mikayel2004/KalmanFilter.git
   cd KalmanFilter
   ```

2. **Install Dependencies**:
   On Ubuntu/Debian, install the required packages:
   ```bash
   sudo apt update
   sudo apt install libopencv-dev libx11-dev cmake g++
   ```

3. **Build the Project**:
   Create a build directory and compile using CMake:
   ```bash
   mkdir build && cd build
   cmake ..
   make
   ```

4. **Run the Application**:
   Execute the compiled binary:
   ```bash
   ./MouseTracker
   ```

## Usage
- Run the program, and it will start tracking the global mouse cursor position.
- The console will display:
  - **Mouse**: The current mouse coordinates (`x`, `y`).
  - **Prediction**: The Kalman Filter's predicted coordinates.
- Press `Ctrl+C` to stop the program.

Example output:
```
Tracking mouse globally using Kalman filter. Press Ctrl+C to stop.
Mouse: (500, 300) | Prediction: (498, 302)
Mouse: (502, 305) | Prediction: (501, 304)
```

## Implementation Details
- **Kalman Filter Model**:
  - **State**: 4D vector `[x, y, vx, vy]` (position and velocity in x and y directions).
  - **Measurement**: 2D vector `[x, y]` (mouse cursor position).
  - **Transition Matrix**: Assumes constant velocity model with time step `dt=1`.
  - **Noise Parameters**:
    - Process noise covariance: `1e-4` (small, assuming smooth motion).
    - Measurement noise covariance: `1e-1` (accounts for mouse jitter).
- **Files**:
  - `kalman_filter.h`: Defines the `MouseKalman` class for Kalman Filter operations.
  - `kalman_filter.cpp`: Implements the Kalman Filter initialization, update, and prediction.
  - `main.cpp`: Handles mouse position retrieval via X11 and integrates with the Kalman Filter.
- **Libraries**:
  - OpenCV for Kalman Filter and matrix operations.
  - X11 for accessing global mouse coordinates.

## Limitations
- Only works on Linux systems with X11 (not compatible with Wayland or other platforms).
- Assumes a linear motion model, which may not handle erratic mouse movements optimally.
- No graphical visualization of the mouse trajectory (console output only).

## Contributing
Contributions are welcome! To contribute:
1. Fork the repository.
2. Create a new branch (`git checkout -b feature-branch`).
3. Make your changes and commit (`git commit -m "Add feature"`).
4. Push to the branch (`git push origin feature-branch`).
5. Open a Pull Request.

Report issues at [https://github.com/Mikayel2004/KalmanFilter/issues](https://github.com/Mikayel2004/KalmanFilter/issues).

## License
This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Acknowledgments
- Built using OpenCV's robust `KalmanFilter` implementation.
- Inspired by real-time tracking applications in computer vision.