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
    // 第一步: 从 UTC 日历日期创建 TimePoint
    //   2024-01-01 00:00:00 UTC (闰秒累计 ΔAT = 37s)
    // ============================================================
    TimePoint tp = TimePoint::FromUTC(2024, 1, 1, 0, 0, 0.0);

    std::cout << "===== TimePoint 基本信息 =====" << std::endl;
    std::cout << "  TimePoint 字符串: " << tp.toString() << std::endl;
    std::cout << "  integerPart:  " << tp.integerPart() << std::endl;
    std::cout << "  fractionalPart: " << tp.fractionalPart() << std::endl;

    // ============================================================
    // 第二步: TimePoint → JulianDate (五种时间系统)
    // ============================================================
    JulianDate jdUTC, jdTAI, jdTT, jdTDB, jdUT1;
    aTimePointToUTC(tp, jdUTC);
    aTimePointToTAI(tp, jdTAI);
    aTimePointToTT(tp, jdTT);
    aTimePointToTDB(tp, jdTDB);
    aTimePointToUT1(tp, jdUT1);

    std::cout << "\n===== TimePoint → JulianDate (五种时间系统) =====" << std::endl;
    std::cout << "  UTC: day=" << jdUTC.day() << "  sec=" << jdUTC.second() << std::endl;
    std::cout << "  TAI: day=" << jdTAI.day() << "  sec=" << jdTAI.second() << std::endl;
    std::cout << "  TT:  day=" << jdTT.day()  << "  sec=" << jdTT.second()  << std::endl;
    std::cout << "  TDB: day=" << jdTDB.day() << "  sec=" << jdTDB.second() << std::endl;
    std::cout << "  UT1: day=" << jdUT1.day() << "  sec=" << jdUT1.second() << std::endl;

    // ============================================================
    // 第三步: 各时间系统之间的偏移量
    // ============================================================
    std::cout << "\n===== UTC 为基准的各系统偏移 =====" << std::endl;
    double taiOff  = (jdTAI - jdUTC).totalSecond();
    double ttOff   = (jdTT  - jdUTC).totalSecond();
    double tdbOff  = (jdTDB - jdUTC).totalSecond();
    double ut1Off  = (jdUT1 - jdUTC).totalSecond();
    std::cout << "  TAI - UTC  = " << taiOff  << " s" << std::endl;
    std::cout << "  TT  - UTC  = " << ttOff   << " s  (= TAI-UTC + 32.184s)" << std::endl;
    std::cout << "  TDB - UTC  = " << tdbOff  << " s" << std::endl;
    std::cout << "  UT1 - UTC  = " << ut1Off  << " s  (地球自转不均匀)" << std::endl;

    // ============================================================
    // 第四步: JulianDate 之间的直接转换 (使用 TimeSystem 函数)
    // ============================================================
    std::cout << "\n===== JulianDate 间直接转换 =====" << std::endl;

    // UTC → TAI → TT → TDB
    JulianDate jdTAI2, jdTT2, jdTDB2;
    aUTCToTAI(jdUTC, jdTAI2);
    aTAIToTT(jdTAI2, jdTT2);
    aTTToTDB(jdTT2, jdTDB2);
    std::cout << "  UTC → TAI → TT → TDB:" << std::endl;
    std::cout << "    TAI: day=" << jdTAI2.day() << " sec=" << jdTAI2.second() << std::endl;
    std::cout << "    TT:  day=" << jdTT2.day()  << " sec=" << jdTT2.second()  << std::endl;
    std::cout << "    TDB: day=" << jdTDB2.day() << " sec=" << jdTDB2.second() << std::endl;

    // TT → TDB 差值 (周期项, ~1.6ms)
    double tdbMinusTT = aTDBMinusTT(jdTT);
    std::cout << "  TDB - TT 周期项 = " << std::scientific << tdbMinusTT << " s  (" << tdbMinusTT * 1e3 << " ms)" << std::endl;

    // ============================================================
    // 第五步: JulianDate → DateTime (日历表示)
    // ============================================================
    std::cout << "\n===== JulianDate → DateTime (日历表示) =====" << std::endl;
    DateTime dttmUTC, dttmTAI, dttmTT;
    aJDToDateTime(jdUTC, dttmUTC);
    aJDToDateTime(jdTAI, dttmTAI);
    aJDToDateTime(jdTT,  dttmTT);
    std::cout << "  UTC: " << dttmUTC.toString() << std::endl;
    std::cout << "  TAI: " << dttmTAI.toString() << std::endl;
    std::cout << "  TT:  " << dttmTT.toString() << std::endl;

    // ============================================================
    // 第六步: 不同时间系统的 DateTime 创建 TimePoint
    // ============================================================
    std::cout << "\n===== DateTime → TimePoint 往返 =====" << std::endl;
    TimePoint tpFromUTC = TimePoint::FromUTC(dttmUTC);
    TimePoint tpFromTAI = TimePoint::FromTAI(dttmTAI);
    TimePoint tpFromTT  = TimePoint::FromTT(dttmTT);
    std::cout << "  FromUTC: " << tpFromUTC.toString() << std::endl;
    std::cout << "  FromTAI: " << tpFromTAI.toString() << std::endl;
    std::cout << "  FromTT:  " << tpFromTT.toString() << std::endl;

    // ============================================================
    // 第七步: 儒略世纪数 (天体力学常用)
    // ============================================================
    std::cout << "\n===== 儒略世纪数 (以 J2000.0 TT 为基准) =====" << std::endl;
    double jcTT  = jdTT.julianCenturyFromJ2000();
    double jcTDB = jdTDB.julianCenturyFromJ2000();
    double daysTT = jdTT.daysFromJ2000();
    std::cout << "  距 J2000.0 (TT):  " << daysTT << " days" << std::endl;
    std::cout << "  T_TT  = " << std::fixed << std::setprecision(8) << jcTT  << " centuries" << std::endl;
    std::cout << "  T_TDB = " << std::fixed << std::setprecision(8) << jcTDB << " centuries" << std::endl;

    // ============================================================
    // 第八步: TimePoint 算术运算
    // ============================================================
    std::cout << "\n===== TimePoint 算术运算 =====" << std::endl;
    TimePoint tpNext = tp + 86400.0;  // 一天后
    std::cout << "  原始: " << tp.toString() << std::endl;
    std::cout << "  +1天: " << tpNext.toString() << std::endl;

    JulianDate jdTT2_val;
    aTimePointToTT(tpNext, jdTT2_val);
    double diffDays = jdTT2_val.daysFromJ2000() - jdTT.daysFromJ2000();
    std::cout << "  实际 TT 天数差: " << diffDays << " days" << std::endl;

    // ============================================================
    // 第九步: J2000.0 历元 (常用参考时刻)
    // ============================================================
    std::cout << "\n===== J2000.0 历元 =====" << std::endl;
    TimePoint j2000 = TimePoint::J2000TT();
    JulianDate j2000TT, j2000TAI, j2000UTC;
    aTimePointToTT(j2000, j2000TT);
    aTimePointToTAI(j2000, j2000TAI);
    aTimePointToUTC(j2000, j2000UTC);
    std::cout << "  J2000 TT:  day=" << j2000TT.day()  << " sec=" << j2000TT.second() << std::endl;
    std::cout << "  J2000 TAI: day=" << j2000TAI.day() << " sec=" << j2000TAI.second() << std::endl;
    std::cout << "  J2000 UTC: day=" << j2000UTC.day() << " sec=" << j2000UTC.second() << std::endl;

    // ============================================================
    // 总结
    // ============================================================
    std::cout << "\n===== 时间系统对照表 (2024-01-01 00:00:00 UTC) =====" << std::endl;
    std::cout << "  [UTC] 协调世界时: " << dttmUTC.toString() << std::endl;
    std::cout << "  [TAI] 国际原子时: " << dttmTAI.toString() << "  (UTC + " << taiOff << "s)" << std::endl;
    std::cout << "  [TT]  地球时:     " << dttmTT.toString() << "  (TAI + 32.184s)" << std::endl;
    std::cout << "  [TDB] 质心动力学时: TT + " << std::scientific << tdbMinusTT << "s (周期项)" << std::endl;
    std::cout << "  [UT1] 世界时:     UTC + " << ut1Off << "s (地球自转修正)" << std::endl;
    std::cout << "  [GPS] GPS时:      TAI - 19s = UTC + " << (taiOff - 19.0) << "s" << std::endl;

    std::cout << "\n  时间系统关系:" << std::endl;
    std::cout << "  TAI = UTC + ΔAT (闰秒累计, 当前 " << taiOff << "s)" << std::endl;
    std::cout << "  TT  = TAI + 32.184s" << std::endl;
    std::cout << "  TDB = TT  + 周期项 (幅度 ~1.6ms)" << std::endl;
    std::cout << "  GPS = TAI - 19s" << std::endl;
    std::cout << "  UT1 = UTC + DUT1 (地球自转, |DUT1| < 0.9s)" << std::endl;

    std::cout << "\nexample completed." << std::endl;
    return 0;
}
