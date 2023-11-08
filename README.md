# Kozmoworks

![Figure_1](https://github.com/arda-guler/Kozmoworks/assets/80536083/a66d6e98-f1c1-423f-bd40-941b56f31283)
*Trajectory lines for two satellites maneuvering around Earth - scales in meters*

**Kozmoworks** is an astrodynamics and space mission simulator. This version features an **8th order symplectic orbit propagator** and currently supports **n-body problems** with:
- impulsive orbital maneuvers
- finite burn orbital maneuvers of constant acceleration type
- atmospheric drag effects
- radiation pressure
- spherical harmonics gravity field models

Some lower priority features are yet to be implemented:

- Relativistic effects
- Complex propulsion and attitude simulations (incl. gravity gradient torque and the like)
- Spacecraft power and systems management
- Additional physics integrator/propagator options
- Built-in 3D visualizations
- Graphical scenario editor

Mission scenarios are defined in .json files - see scenarios/test.json for an example.

## License(s)
Kozmoworks is licensed under [MIT License](https://github.com/arda-guler/Kozmoworks/blob/master/LICENSE).

Kozmoworks uses [nlohmann/json](https://github.com/nlohmann/json), licensed under [MIT License](https://github.com/nlohmann/json/blob/develop/LICENSE.MIT).

## OS3D
Kozmoworks is also the successor to [OrbitSim3D](https://github.com/arda-guler/orbitSim3D), a space mission simulator written entirely in Python, using pyOpenGL - check it out if you like!
