# Frame transforms

## Overview

Coordinate transforms between Earth reference frames are the basis of orbit computation. The chain from an inertial frame (J2000/ICRF) to an Earth-fixed frame (ECF) has four stages:

```
J2000 → MOD       precession
MOD   → TOD       nutation
TOD   → GTOD      Earth rotation (GAST)
GTOD  → ECF       polar motion
```

This example walks through the full transform chain, the shortcut functions, how to obtain the matrices, and round-trip accuracy.

## Example

@[code](frame-transform.cpp)

## Notes

- ECI is the generic name for Earth-centered inertial frames; depending on configuration it may mean J2000 or ICRF.
- Velocity-aware transforms account for the motion of the rotating frame.
- Polar motion correction requires EOP data.
- Switch between the IAU1980 and JPL DE nutation models with `aNutationMethodSet`.
