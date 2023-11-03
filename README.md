# Kozmoworks

![Figure_1](https://github.com/arda-guler/Kozmoworks/assets/80536083/a66d6e98-f1c1-423f-bd40-941b56f31283)
*Trajectory lines for two satellites maneuvering around Earth - scales in meters*

**Kozmoworks** is an astrodynamics simulator, to-be successor of [OrbitSim3D](https://github.com/arda-guler/orbitSim3D), with similar features but faster computations. 
This version features an **8th order symplectic orbit propagator** and currently supports **n-body problems** with:
- impulsive orbital maneuvers
- finite burn orbital maneuvers of constant acceleration type
- atmospheric drag effects
- radiation pressure

Mission scenarios are defined in .json files - see scenarios/test.json for an example.

This is still a Work-In-Progress (TM) - some features are missing but intended for implementation:

- spherical harmonics gravity
- ...
