# Converting orbit elements

## Overview

Orbital mechanics commonly uses five representations of orbit elements: Cartesian state, classical elements, modified orbital elements, equinoctial elements and modified equinoctial elements. Each has its own use:

| Type | Typical use |
|------|-------------|
| Cartesian state (CartState) | Numerical integration, dynamics propagation |
| Classical elements (OrbElem) | Orbit design, intuitive human understanding |
| Modified orbital elements (ModOrbElem) | Near-circular orbits, analysis near periapsis |
| Equinoctial elements (EquinElem) | Low eccentricity and inclination, avoids singularities |
| Modified equinoctial elements (ModEquinElem) | Perturbation analysis, long-term propagation |

This example shows the complete conversion chain between all five representations, using both the class-reference interface and the C array interface.

## Conversion diagram

```
OrbElem  ←→  CartState  ←→  ModOrbElem
   ↓            ↓               ↓
EquinElem ←→ ModEquinElem  ←→  CartState
```

## Example

@[code](orbit-element-convert.cpp)

## Notes

- All angles are in radians, distances in meters and velocities in meters per second.
- Modified equinoctial elements become singular when the inclination approaches 180 degrees.
- Equinoctial elements work well for low eccentricity and low inclination orbits.
- Zero-eccentricity circular orbits may need special handling in some conversions.
