---
home: true
icon: house
title: Project overview
footer: false
heroText: SpaceAST
tagline: Modern C++ algorithms for aerospace simulation
heroImage: /hero.svg
heroAlt: SpaceAST orbit and satellite illustration
actions:
  - text: Get started
    link: /en/guide/quickstart.html
    type: primary
  - text: Module reference
    link: /en/reference/
  - text: API reference
    link: https://space-ast.github.io/ast/api/
  - text: Download
    link: https://gitcode.com/space-ast/ast/releases/
features:
  - title: High-precision orbit propagation
    details: Two-body, J2/J4 analytical and HPOP numerical propagation with configurable force models.
    icon: satellite
    link: /en/reference/core/propagator/
  - title: Complete frames and time system
    details: J2000/ECEF transformation chains, Julian date and UTC/TAI/TT conversions, JPL DE and SPICE SPK ephemerides.
    icon: clock
    link: /en/reference/core/time.html
  - title: Attitude and math foundations
    details: Quaternions, Euler angles, DCMs, linear algebra, ODE integrators and interpolation.
    icon: compass
    link: /en/reference/math/attitude.html
  - title: Modular shared libraries
    details: core, math and util can be linked independently with clear dependencies.
    icon: cubes
    link: /en/reference/
  - title: Cross-platform builds
    details: Windows and Linux, C++11, with xmake or CMake integration.
    icon: laptop-code
    link: /en/guide/build-from-source.html
  - title: Python bindings
    details: SWIG-generated bindings for scripting, validation and rapid prototyping.
    icon: code
    link: https://github.com/space-ast/ast
---

## Getting started

Get a release and build your first program from the [quick start](guide/quickstart.html), or browse the [module reference](reference/) and [examples](examples/)

> SpaceAST (/æst/) provides reusable algorithm components for aerospace mission analysis
