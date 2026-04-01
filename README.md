# Autonomous Sensor & Intercept Simulation

Real-time simulation of a sensor-based perception and intercept system, inspired by radar/LIDAR tracking pipelines used in robotics, aerospace, and defense systems.

---

## Overview

This project implements a full perception-to-intercept pipeline:

Sensor → Point Cloud → Clustering → Tracking → Prediction → Intercept

The system simulates a scanning LIDAR sensor, detects objects in the environment, tracks them over time using state estimation, predicts future motion, and computes intercept solutions in real-time.

---

## Key Features

- Real-time LIDAR-style scanning using raycasts
- Point cloud accumulation and voxel grid filtering
- Spatial clustering for object detection
- Multi-object tracking with data association
- Kalman Filter (alpha-beta) for state estimation
- Motion-aware tracking using predicted positions
- Future trajectory prediction
- Analytical intercept solver
- Debug visualization for all stages

---

## System Pipeline

### 1. Sensor Simulation
- Rotating LIDAR scan using configurable rays and vertical layers
- Generates dynamic point cloud from environment geometry

### 2. Point Cloud Processing
- Accumulation over time
- Voxel grid filtering to reduce noise and density

### 3. Clustering
- Distance-based clustering (with ground filtering)
- Extracts object candidates from point cloud

### 4. Tracking
- Nearest-neighbor data association with motion prediction
- Each tracked object maintains:
    - Position
    - Velocity
    - Bounding box

### 5. State Estimation
- Lightweight Kalman Filter (alpha-beta model)
- Smooths noisy measurements
- Produces stable velocity estimates

### 6. Prediction
- Future trajectory estimation:
  x(t) = x₀ + v * t

### 7. Intercept Solver
- Analytical solution for projectile interception
- Computes intercept point given:
    - Target position
    - Target velocity
    - Projectile speed

---

## Technical Highlights

- Modular C++ architecture (Unreal Engine)
- Separation of algorithms and runtime systems
- Unit-tested core algorithms:
    - Clustering
    - Voxel filtering
    - Intercept solver
    - Kalman filter
- Real-time simulation (frame-by-frame processing)
- Motion-aware data association (reduces ID switching)

---

## Visualization

The system provides real-time debug visualization:

- Point cloud (depth-colored)
- Cluster bounding boxes
- Tracked objects (velocity vectors)
- Predicted trajectories
- Intercept points and paths

---

## Technologies

- C++
- Unreal Engine 5
- Real-time simulation
- Spatial algorithms
- State estimation (Kalman Filter)
- Computational geometry

---

## Applications

This project demonstrates concepts applicable to:

- Autonomous systems (robotics, drones, vehicles)
- Radar and LIDAR perception pipelines
- Target tracking systems
- Guidance and intercept algorithms
- Simulation and sensor fusion research

---

## Future Improvements

- Multi-target assignment (Hungarian algorithm)
- Track lifecycle management (confirmed / lost states)
- Sensor noise modeling
- Extended Kalman Filter (EKF)
- Multi-sensor fusion (radar + LIDAR)

---

## Author

Wanderson Lopes  
Software Engineer (C++ / Unreal Engine / Systems)

---