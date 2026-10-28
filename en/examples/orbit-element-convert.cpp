#include "ast/OrbitElement.hpp"
#include "ast/Vector.hpp"
#include <iostream>
#include <iomanip>
#include <clocale>
#include <cmath>

AST_USING_NAMESPACE

int main()
{
    setlocale(LC_ALL, ".UTF-8");

    const double gm = 3.986004418e14;   // Earth gravitational parameter (m^3/s^2)

    // ============================================================
    // Step 1: start from a set of classical orbital elements
    //   LEO orbit: a=8000km, e=0.1, i=30deg, raan=45deg, argper=60deg, trueA=30deg
    // ============================================================
    OrbElem coe;
    coe.a()      = 8000000.0;           // semi-major axis (m)
    coe.e()      = 0.1;                 // eccentricity
    coe.i()      = kDegToRad * 30.0;    // inclination (rad)
    coe.raan()   = kDegToRad * 45.0;    // right ascension of the ascending node (rad)
    coe.argper() = kDegToRad * 60.0;    // argument of periapsis (rad)
    coe.trueA()  = kDegToRad * 30.0;    // true anomaly (rad)

    std::cout << "===== initial classical elements (OrbElem) =====" << std::endl;
    std::cout << "  semi-major axis a = " << coe.a()      << " m" << std::endl;
    std::cout << "  eccentricity e    = " << coe.e()      << std::endl;
    std::cout << "  inclination i     = " << coe.i()      << " rad (" << coe.i()      * kRadToDeg << " deg)" << std::endl;
    std::cout << "  RAAN              = " << coe.raan()   << " rad (" << coe.raan()   * kRadToDeg << " deg)" << std::endl;
    std::cout << "  argument of periapsis = " << coe.argper() << " rad (" << coe.argper() * kRadToDeg << " deg)" << std::endl;
    std::cout << "  true anomaly      = " << coe.trueA()  << " rad (" << coe.trueA()  * kRadToDeg << " deg)" << std::endl;

    // ============================================================
    // Step 2: OrbElem -> CartState
    // ============================================================
    Vector3d pos, vel;
    errc_t err = aOrbElemToCart(coe, gm, pos, vel);
    if (err != eNoError) {
        std::cerr << "OrbElem -> CartState conversion failed, err=" << err << std::endl;
        return -1;
    }

    std::cout << "\n===== OrbElem -> CartState =====" << std::endl;
    std::cout << "  pos (m):  " << pos.x() << "  " << pos.y() << "  " << pos.z() << std::endl;
    std::cout << "  vel (m/s): " << vel.x() << "  " << vel.y() << "  " << vel.z() << std::endl;

    // ============================================================
    // Step 3: CartState -> ModOrbElem (modified orbital elements)
    // ============================================================
    ModOrbElem moe;
    err = aCartToModOrbElem(pos, vel, gm, moe);
    if (err != eNoError) {
        std::cerr << "CartState -> ModOrbElem conversion failed, err=" << err << std::endl;
        return -1;
    }

    std::cout << "\n===== CartState -> ModOrbElem =====" << std::endl;
    std::cout << "  periapsis radius rp = " << moe.rp()      << " m" << std::endl;
    std::cout << "  eccentricity e      = " << moe.e()       << std::endl;
    std::cout << "  inclination i       = " << moe.i()       << " rad (" << moe.i()       * kRadToDeg << " deg)" << std::endl;
    std::cout << "  RAAN                = " << moe.raan()    << " rad (" << moe.raan()    * kRadToDeg << " deg)" << std::endl;
    std::cout << "  argument of periapsis = " << moe.argper()  << " rad (" << moe.argper()  * kRadToDeg << " deg)" << std::endl;
    std::cout << "  true anomaly        = " << moe.trueA()   << " rad (" << moe.trueA()   * kRadToDeg << " deg)" << std::endl;

    // ============================================================
    // Step 4: ModOrbElem -> EquinElem (equinoctial elements)
    // ============================================================
    EquinElem ee;
    err = aModOrbToEquinElem(moe, ee);
    if (err != eNoError) {
        std::cerr << "ModOrbElem -> EquinElem conversion failed, err=" << err << std::endl;
        return -1;
    }

    std::cout << "\n===== ModOrbElem -> EquinElem =====" << std::endl;
    std::cout << "  semi-major axis a = " << ee.a()      << " m" << std::endl;
    std::cout << "  h        = " << ee.h()      << std::endl;
    std::cout << "  k        = " << ee.k()      << std::endl;
    std::cout << "  p        = " << ee.p()      << std::endl;
    std::cout << "  q        = " << ee.q()      << std::endl;
    std::cout << "  lambda   = " << ee.lambda() << " rad" << std::endl;

    // ============================================================
    // Step 5: EquinElem -> ModEquinElem (modified equinoctial elements)
    // ============================================================
    ModEquinElem mee;
    ee2mee(ee.data(), mee.data());

    std::cout << "\n===== EquinElem -> ModEquinElem =====" << std::endl;
    std::cout << "  semi-latus rectum p = " << mee.p() << " m" << std::endl;
    std::cout << "  f       = " << mee.f() << std::endl;
    std::cout << "  g       = " << mee.g() << std::endl;
    std::cout << "  h       = " << mee.h() << std::endl;
    std::cout << "  k       = " << mee.k() << std::endl;
    std::cout << "  L       = " << mee.L() << " rad" << std::endl;

    // ============================================================
    // Step 6: ModEquinElem -> CartState (back to Cartesian coordinates)
    // ============================================================
    Vector3d pos2, vel2;
    aModEquinElemToCart(mee, gm, pos2, vel2);

    std::cout << "\n===== ModEquinElem -> CartState =====" << std::endl;
    std::cout << "  pos (m):  " << pos2.x() << "  " << pos2.y() << "  " << pos2.z() << std::endl;
    std::cout << "  vel (m/s): " << vel2.x() << "  " << vel2.y() << "  " << vel2.z() << std::endl;

    // ============================================================
    // Step 7: verify round-trip accuracy
    // ============================================================
    double posErr = (pos2 - pos).norm();
    double velErr = (vel2 - vel).norm();

    std::cout << "\n===== round-trip accuracy =====" << std::endl;
    std::cout << "  path: OrbElem -> Cart -> ModOrb -> Equin -> ModEquin -> Cart" << std::endl;
    std::cout << "  position error = " << std::scientific << posErr << " m" << std::endl;
    std::cout << "  velocity error = " << std::scientific << velErr << " m/s" << std::endl;

    // ============================================================
    // Step 8: equivalent conversions with the C array interface
    // ============================================================
    double coeArr[6] = {coe.a(), coe.e(), coe.i(), coe.raan(), coe.argper(), coe.trueA()};
    double posArr[3], velArr[3];
    err = coe2rv(coeArr, gm, posArr, velArr);
    if (err != eNoError) {
        std::cerr << "coe2rv conversion failed, err=" << err << std::endl;
        return -1;
    }

    std::cout << "\n===== C array interface: coe2rv =====" << std::endl;
    std::cout << "  pos (m):  " << posArr[0] << "  " << posArr[1] << "  " << posArr[2] << std::endl;
    std::cout << "  vel (m/s): " << velArr[0] << "  " << velArr[1] << "  " << velArr[2] << std::endl;

    // ----- rv -> moe -> ee -> mee -> coe (reverse chain) -----
    double moeArr[6], eeArr[6], meeArr[6], coeArr2[6];
    rv2moe(posArr, velArr, gm, moeArr);
    moe2ee(moeArr, eeArr);
    ee2mee(eeArr, meeArr);
    mee2coe(meeArr, coeArr2);

    std::cout << "\n===== C array interface: rv -> moe -> ee -> mee -> coe =====" << std::endl;
    std::cout << "  a   = " << coeArr2[0] << " m    (original " << coeArr[0] << ")" << std::endl;
    std::cout << "  e   = " << coeArr2[1] << "      (original " << coeArr[1] << ")" << std::endl;
    std::cout << "  i   = " << coeArr2[2] << " rad  (original " << coeArr[2] << ")" << std::endl;
    std::cout << "  raan  = " << coeArr2[3] << " rad  (original " << coeArr[3] << ")" << std::endl;
    std::cout << "  argper  = " << coeArr2[4] << " rad  (original " << coeArr[4] << ")" << std::endl;
    std::cout << "  trueA  = " << coeArr2[5] << " rad  (original " << coeArr[5] << ")" << std::endl;

    // ============================================================
    // Summary: comparison of the five representations
    // ============================================================
    std::cout << "\n===== comparison of the five element sets =====" << std::endl;
    std::cout << "  [1] CartState:" << std::endl;
    std::cout << "      r = (" << pos.x() << ", " << pos.y() << ", " << pos.z() << ")" << std::endl;
    std::cout << "      v = (" << vel.x() << ", " << vel.y() << ", " << vel.z() << ")" << std::endl;
    std::cout << "  [2] OrbElem (classical):" << std::endl;
    std::cout << "      a=" << coe.a() << "  e=" << coe.e() << "  i=" << coe.i() << std::endl;
    std::cout << "      raan=" << coe.raan() << "  argper=" << coe.argper() << "  trueA=" << coe.trueA() << std::endl;
    std::cout << "  [3] ModOrbElem (modified):" << std::endl;
    std::cout << "      rp=" << moe.rp() << "  e=" << moe.e() << "  i=" << moe.i() << std::endl;
    std::cout << "      raan=" << moe.raan() << "  argper=" << moe.argper() << "  trueA=" << moe.trueA() << std::endl;
    std::cout << "  [4] EquinElem (equinoctial):" << std::endl;
    std::cout << "      a=" << ee.a() << "  h=" << ee.h() << "  k=" << ee.k() << std::endl;
    std::cout << "      p=" << ee.p() << "  q=" << ee.q() << "  lambda=" << ee.lambda() << std::endl;
    std::cout << "  [5] ModEquinElem (modified equinoctial):" << std::endl;
    std::cout << "      p=" << mee.p() << "  f=" << mee.f() << "  g=" << mee.g() << std::endl;
    std::cout << "      h=" << mee.h() << "  k=" << mee.k() << "  L=" << mee.L() << std::endl;

    std::cout << "\nexample completed." << std::endl;
    return 0;
}
