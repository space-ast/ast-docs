#include "ast/TimePoint.hpp"
#include "ast/TimeSystem.hpp"
#include "ast/JulianDate.hpp"
#include "ast/DateTime.hpp"

#include "ast/Duration.hpp"
#include <iostream>
#include <iomanip>
#include <clocale>

AST_USING_NAMESPACE

int main()
{
    setlocale(LC_ALL, ".UTF-8");

    // ============================================================
    // Step 1: create a TimePoint from a UTC calendar date
    //   2024-01-01 00:00:00 UTC (accumulated leap seconds ΔAT = 37 s)
    // ============================================================
    TimePoint tp = TimePoint::FromUTC(2024, 1, 1, 0, 0, 0.0);

    std::cout << "===== TimePoint basics =====" << std::endl;
    std::cout << "  TimePoint string: " << tp.toString() << std::endl;
    std::cout << "  integerPart:  " << tp.integerPart() << std::endl;
    std::cout << "  fractionalPart: " << tp.fractionalPart() << std::endl;

    // ============================================================
    // Step 2: TimePoint → JulianDate (five time systems)
    // ============================================================
    JulianDate jdUTC, jdTAI, jdTT, jdTDB, jdUT1;
    aTimePointToUTC(tp, jdUTC);
    aTimePointToTAI(tp, jdTAI);
    aTimePointToTT(tp, jdTT);
    aTimePointToTDB(tp, jdTDB);
    aTimePointToUT1(tp, jdUT1);

    std::cout << "\n===== TimePoint → JulianDate (five time systems) =====" << std::endl;
    std::cout << "  UTC: day=" << jdUTC.day() << "  sec=" << jdUTC.second() << std::endl;
    std::cout << "  TAI: day=" << jdTAI.day() << "  sec=" << jdTAI.second() << std::endl;
    std::cout << "  TT:  day=" << jdTT.day()  << "  sec=" << jdTT.second()  << std::endl;
    std::cout << "  TDB: day=" << jdTDB.day() << "  sec=" << jdTDB.second() << std::endl;
    std::cout << "  UT1: day=" << jdUT1.day() << "  sec=" << jdUT1.second() << std::endl;

    // ============================================================
    // Step 3: offsets between time systems
    // ============================================================
    std::cout << "\n===== offsets relative to UTC =====" << std::endl;
    double taiOff  = (jdTAI - jdUTC).totalSecond();
    double ttOff   = (jdTT  - jdUTC).totalSecond();
    double tdbOff  = (jdTDB - jdUTC).totalSecond();
    double ut1Off  = (jdUT1 - jdUTC).totalSecond();
    std::cout << "  TAI - UTC  = " << taiOff  << " s" << std::endl;
    std::cout << "  TT  - UTC  = " << ttOff   << " s  (= TAI-UTC + 32.184 s)" << std::endl;
    std::cout << "  TDB - UTC  = " << tdbOff  << " s" << std::endl;
    std::cout << "  UT1 - UTC  = " << ut1Off  << " s  (Earth rotation irregularity)" << std::endl;

    // ============================================================
    // Step 4: direct JulianDate conversions (using the TimeSystem functions)
    // ============================================================
    std::cout << "\n===== direct JulianDate conversions =====" << std::endl;

    // UTC → TAI → TT → TDB
    JulianDate jdTAI2, jdTT2, jdTDB2;
    aUTCToTAI(jdUTC, jdTAI2);
    aTAIToTT(jdTAI2, jdTT2);
    aTTToTDB(jdTT2, jdTDB2);
    std::cout << "  UTC → TAI → TT → TDB:" << std::endl;
    std::cout << "    TAI: day=" << jdTAI2.day() << " sec=" << jdTAI2.second() << std::endl;
    std::cout << "    TT:  day=" << jdTT2.day()  << " sec=" << jdTT2.second()  << std::endl;
    std::cout << "    TDB: day=" << jdTDB2.day() << " sec=" << jdTDB2.second() << std::endl;

    // TDB - TT difference (periodic term, ~1.6 ms)
    double tdbMinusTT = aTDBMinusTT(jdTT);
    std::cout << "  TDB - TT periodic term = " << std::scientific << tdbMinusTT << " s  (" << tdbMinusTT * 1e3 << " ms)" << std::endl;

    // ============================================================
    // Step 5: JulianDate → DateTime (calendar representation)
    // ============================================================
    std::cout << "\n===== JulianDate → DateTime (calendar representation) =====" << std::endl;
    DateTime dttmUTC, dttmTAI, dttmTT;
    aJDToDateTime(jdUTC, dttmUTC);
    aJDToDateTime(jdTAI, dttmTAI);
    aJDToDateTime(jdTT,  dttmTT);
    std::cout << "  UTC: " << dttmUTC.toString() << std::endl;
    std::cout << "  TAI: " << dttmTAI.toString() << std::endl;
    std::cout << "  TT:  " << dttmTT.toString() << std::endl;

    // ============================================================
    // Step 6: create TimePoints from DateTimes in different time systems
    // ============================================================
    std::cout << "\n===== DateTime → TimePoint round trip =====" << std::endl;
    TimePoint tpFromUTC = TimePoint::FromUTC(dttmUTC);
    TimePoint tpFromTAI = TimePoint::FromTAI(dttmTAI);
    TimePoint tpFromTT  = TimePoint::FromTT(dttmTT);
    std::cout << "  FromUTC: " << tpFromUTC.toString() << std::endl;
    std::cout << "  FromTAI: " << tpFromTAI.toString() << std::endl;
    std::cout << "  FromTT:  " << tpFromTT.toString() << std::endl;

    // ============================================================
    // Step 7: Julian centuries (common in celestial mechanics)
    // ============================================================
    std::cout << "\n===== Julian centuries (relative to J2000.0 TT) =====" << std::endl;
    double jcTT  = jdTT.julianCenturyFromJ2000();
    double jcTDB = jdTDB.julianCenturyFromJ2000();
    double daysTT = jdTT.daysFromJ2000();
    std::cout << "  days from J2000.0 (TT):  " << daysTT << " days" << std::endl;
    std::cout << "  T_TT  = " << std::fixed << std::setprecision(8) << jcTT  << " centuries" << std::endl;
    std::cout << "  T_TDB = " << std::fixed << std::setprecision(8) << jcTDB << " centuries" << std::endl;

    // ============================================================
    // Step 8: TimePoint arithmetic
    // ============================================================
    std::cout << "\n===== TimePoint arithmetic =====" << std::endl;
    TimePoint tpNext = tp + 86400.0;  // one day later
    std::cout << "  original: " << tp.toString() << std::endl;
    std::cout << "  +1 day:   " << tpNext.toString() << std::endl;

    JulianDate jdTT2_val;
    aTimePointToTT(tpNext, jdTT2_val);
    double diffDays = jdTT2_val.daysFromJ2000() - jdTT.daysFromJ2000();
    std::cout << "  actual TT day difference: " << diffDays << " days" << std::endl;

    // ============================================================
    // Step 9: the J2000.0 epoch (a common reference time)
    // ============================================================
    std::cout << "\n===== J2000.0 epoch =====" << std::endl;
    TimePoint j2000 = TimePoint::J2000TT();
    JulianDate j2000TT, j2000TAI, j2000UTC;
    aTimePointToTT(j2000, j2000TT);
    aTimePointToTAI(j2000, j2000TAI);
    aTimePointToUTC(j2000, j2000UTC);
    std::cout << "  J2000 TT:  day=" << j2000TT.day()  << " sec=" << j2000TT.second() << std::endl;
    std::cout << "  J2000 TAI: day=" << j2000TAI.day() << " sec=" << j2000TAI.second() << std::endl;
    std::cout << "  J2000 UTC: day=" << j2000UTC.day() << " sec=" << j2000UTC.second() << std::endl;

    // ============================================================
    // Summary
    // ============================================================
    std::cout << "\n===== time system comparison (2024-01-01 00:00:00 UTC) =====" << std::endl;
    std::cout << "  [UTC] Coordinated Universal Time: " << dttmUTC.toString() << std::endl;
    std::cout << "  [TAI] International Atomic Time:  " << dttmTAI.toString() << "  (UTC + " << taiOff << " s)" << std::endl;
    std::cout << "  [TT]  Terrestrial Time:           " << dttmTT.toString() << "  (TAI + 32.184 s)" << std::endl;
    std::cout << "  [TDB] Barycentric Dynamical Time: TT + " << std::scientific << tdbMinusTT << " s (periodic term)" << std::endl;
    std::cout << "  [UT1] Universal Time:             UTC + " << ut1Off << " s (Earth rotation correction)" << std::endl;
    std::cout << "  [GPS] GPS Time:                   TAI - 19 s = UTC + " << (taiOff - 19.0) << " s" << std::endl;

    std::cout << "\n  time system relations:" << std::endl;
    std::cout << "  TAI = UTC + ΔAT (accumulated leap seconds, currently " << taiOff << " s)" << std::endl;
    std::cout << "  TT  = TAI + 32.184 s" << std::endl;
    std::cout << "  TDB = TT  + periodic term (amplitude ~1.6 ms)" << std::endl;
    std::cout << "  GPS = TAI - 19 s" << std::endl;
    std::cout << "  UT1 = UTC + DUT1 (Earth rotation, |DUT1| < 0.9 s)" << std::endl;

    std::cout << "\nexample completed." << std::endl;
    return 0;
}
