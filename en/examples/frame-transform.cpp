#include "ast/FrameTransform.hpp"
#include "ast/TimePoint.hpp"
#include "ast/Vector.hpp"
#include "ast/Matrix.hpp"

#include <iostream>
#include <iomanip>
#include <clocale>

AST_USING_NAMESPACE

int main()
{
    setlocale(LC_ALL, ".UTF-8");

    // ============================================================
    // Pick an epoch and a J2000 state
    //   2024-01-01 00:00:00 UTC, a LEO satellite
    // ============================================================
    TimePoint tp = TimePoint::FromUTC(2024, 1, 1, 0, 0, 0.0);

    Vector3d rJ2000{-4.463444e6,  4.463444e6,  3.644387e6};  // position (m)
    Vector3d vJ2000{-5.668038e3, -5.233606e3,  1.773562e2};  // velocity (m/s)

    std::cout << "===== J2000 state (2024-01-01 00:00:00 UTC) =====" << std::endl;
    std::cout << "  r_J2000 = " << rJ2000.x() << "  " << rJ2000.y() << "  " << rJ2000.z() << std::endl;
    std::cout << "  v_J2000 = " << vJ2000.x() << "  " << vJ2000.y() << "  " << vJ2000.z() << std::endl;

    // ============================================================
    // Shortcut transform: J2000 → ECF (position + velocity)
    // ============================================================
    Vector3d rECF, vECF;
    aJ2000ToECF(tp, rJ2000, vJ2000, rECF, vECF);

    std::cout << "\n===== J2000 → ECF (shortcut, with velocity) =====" << std::endl;
    std::cout << "  r_ECF = " << rECF.x() << "  " << rECF.y() << "  " << rECF.z() << std::endl;
    std::cout << "  v_ECF = " << vECF.x() << "  " << vECF.y() << "  " << vECF.z() << std::endl;

    // ============================================================
    // Step-by-step transform: J2000 → MOD → TOD → GTOD → ECF
    // ============================================================
    Vector3d rMOD, rTOD, rGTOD, rECF2;
    aJ2000ToMOD(tp, rJ2000, rMOD);
    aMODToTOD(tp, rMOD, rTOD);
    aTODToGTOD(tp, rTOD, rGTOD);
    aGTODToECF(tp, rGTOD, rECF2);

    std::cout << "\n===== J2000 → MOD → TOD → GTOD → ECF (step by step) =====" << std::endl;
    std::cout << "  r_MOD  = " << rMOD.x()  << "  " << rMOD.y()  << "  " << rMOD.z()  << std::endl;
    std::cout << "  r_TOD  = " << rTOD.x()  << "  " << rTOD.y()  << "  " << rTOD.z()  << std::endl;
    std::cout << "  r_GTOD = " << rGTOD.x() << "  " << rGTOD.y() << "  " << rGTOD.z() << std::endl;
    std::cout << "  r_ECF  = " << rECF2.x() << "  " << rECF2.y() << "  " << rECF2.z() << std::endl;

    // ============================================================
    // Compare the shortcut with the step-by-step chain
    // ============================================================
    double diff = (rECF2 - rECF).norm();

    std::cout << "\n===== shortcut vs step-by-step =====" << std::endl;
    std::cout << "  position difference = " << std::scientific << diff << " m" << std::endl;

    // ============================================================
    // Get the transform matrix for each step
    // ============================================================
    Matrix3d matJ2000ToMOD, matMODToTOD, matTODToGTOD, matGTODToECF;
    aJ2000ToMODMatrix(tp, matJ2000ToMOD);
    aMODToTODMatrix(tp, matMODToTOD);
    aTODToGTODMatrix(tp, matTODToGTOD);
    aGTODToECFMatrix(tp, matGTODToECF);

    std::cout << "\n===== per-step transform matrices =====" << std::endl;
    std::cout << "  M(J2000→MOD):" << std::endl;
    std::cout << "    [" << matJ2000ToMOD(0,0) << "  " << matJ2000ToMOD(0,1) << "  " << matJ2000ToMOD(0,2) << "]" << std::endl;
    std::cout << "    [" << matJ2000ToMOD(1,0) << "  " << matJ2000ToMOD(1,1) << "  " << matJ2000ToMOD(1,2) << "]" << std::endl;
    std::cout << "    [" << matJ2000ToMOD(2,0) << "  " << matJ2000ToMOD(2,1) << "  " << matJ2000ToMOD(2,2) << "]" << std::endl;

    std::cout << "  M(MOD→TOD):" << std::endl;
    std::cout << "    [" << matMODToTOD(0,0) << "  " << matMODToTOD(0,1) << "  " << matMODToTOD(0,2) << "]" << std::endl;
    std::cout << "    [" << matMODToTOD(1,0) << "  " << matMODToTOD(1,1) << "  " << matMODToTOD(1,2) << "]" << std::endl;
    std::cout << "    [" << matMODToTOD(2,0) << "  " << matMODToTOD(2,1) << "  " << matMODToTOD(2,2) << "]" << std::endl;

    // ============================================================
    // J2000 → ICRF
    // ============================================================
    Vector3d rICRF;
    aJ2000ToICRF(tp, rJ2000, rICRF);

    double icrfJ2000Diff = (rICRF - rJ2000).norm();
    std::cout << "\n===== ICRF ↔ J2000 =====" << std::endl;
    std::cout << "  r_ICRF = " << rICRF.x() << "  " << rICRF.y() << "  " << rICRF.z() << std::endl;
    std::cout << "  |r_ICRF - r_J2000| = " << std::scientific << icrfJ2000Diff << " m" << std::endl;

    // ============================================================
    // ECF → J2000 (inverse transform)
    // ============================================================
    Vector3d rJ2000_back;
    aECFToJ2000(tp, rECF, rJ2000_back);

    double roundTrip = (rJ2000_back - rJ2000).norm();
    std::cout << "\n===== ECF → J2000 (inverse) =====" << std::endl;
    std::cout << "  r_J2000_back = " << rJ2000_back.x() << "  " << rJ2000_back.y() << "  " << rJ2000_back.z() << std::endl;
    std::cout << "  |r_back - r_orig| = " << std::scientific << roundTrip << " m" << std::endl;

    // ============================================================
    // Summary
    // ============================================================
    std::cout << "\n===== transform chain summary =====" << std::endl;
    std::cout << "  ICRF ←→ J2000  (frame bias, |r_ICRF - r_J2000| = " << icrfJ2000Diff << " m)" << std::endl;
    std::cout << "  J2000 → MOD    (precession)" << std::endl;
    std::cout << "  MOD   → TOD    (nutation)" << std::endl;
    std::cout << "  TOD   → GTOD   (Earth rotation)" << std::endl;
    std::cout << "  GTOD  → ECF    (polar motion)" << std::endl;
    std::cout << "  shortcut J2000 → ECF matches the step-by-step chain, position difference = " << diff << " m" << std::endl;
    std::cout << "  round trip J2000 → ECF → J2000 accuracy = " << roundTrip << " m" << std::endl;

    std::cout << "\nexample completed." << std::endl;
    return 0;
}
