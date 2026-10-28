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

    const double gm = 3.986004418e14;   // 地球引力参数 (m^3/s^2)

    // ============================================================
    // 第一步: 构造一组经典轨道根数作为起点
    //   LEO 轨道: a=8000km, e=0.1, i=30deg, raan=45deg, argper=60deg, trueA=30deg
    // ============================================================
    OrbElem coe;
    coe.a()      = 8000000.0;           // 长半轴 (m)
    coe.e()      = 0.1;                 // 偏心率
    coe.i()      = kDegToRad * 30.0;    // 轨道倾角 (rad)
    coe.raan()   = kDegToRad * 45.0;    // 升交点赤经 (rad)
    coe.argper() = kDegToRad * 60.0;    // 近拱点角 (rad)
    coe.trueA()  = kDegToRad * 30.0;    // 真近点角 (rad)

    std::cout << "===== 初始经典轨道根数 (OrbElem) =====" << std::endl;
    std::cout << "  长半轴 a      = " << coe.a()      << " m" << std::endl;
    std::cout << "  偏心率 e      = " << coe.e()      << std::endl;
    std::cout << "  轨道倾角 i    = " << coe.i()      << " rad (" << coe.i()      * kRadToDeg << " deg)" << std::endl;
    std::cout << "  升交点赤经 raan  = " << coe.raan()   << " rad (" << coe.raan()   * kRadToDeg << " deg)" << std::endl;
    std::cout << "  近拱点角 argper    = " << coe.argper() << " rad (" << coe.argper() * kRadToDeg << " deg)" << std::endl;
    std::cout << "  真近点角 trueA  = " << coe.trueA()  << " rad (" << coe.trueA()  * kRadToDeg << " deg)" << std::endl;

    // ============================================================
    // 第二步: OrbElem -> CartState
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
    // 第三步: CartState -> ModOrbElem (修正轨道根数)
    // ============================================================
    ModOrbElem moe;
    err = aCartToModOrbElem(pos, vel, gm, moe);
    if (err != eNoError) {
        std::cerr << "CartState -> ModOrbElem conversion failed, err=" << err << std::endl;
        return -1;
    }

    std::cout << "\n===== CartState -> ModOrbElem =====" << std::endl;
    std::cout << "  近拱点半径 rp = " << moe.rp()      << " m" << std::endl;
    std::cout << "  偏心率 e       = " << moe.e()       << std::endl;
    std::cout << "  轨道倾角 i     = " << moe.i()       << " rad (" << moe.i()       * kRadToDeg << " deg)" << std::endl;
    std::cout << "  升交点赤经 raan   = " << moe.raan()    << " rad (" << moe.raan()    * kRadToDeg << " deg)" << std::endl;
    std::cout << "  近拱点角 argper     = " << moe.argper()  << " rad (" << moe.argper()  * kRadToDeg << " deg)" << std::endl;
    std::cout << "  真近点角 trueA   = " << moe.trueA()   << " rad (" << moe.trueA()   * kRadToDeg << " deg)" << std::endl;

    // ============================================================
    // 第四步: ModOrbElem -> EquinElem (春分点根数)
    // ============================================================
    EquinElem ee;
    err = aModOrbToEquinElem(moe, ee);
    if (err != eNoError) {
        std::cerr << "ModOrbElem -> EquinElem conversion failed, err=" << err << std::endl;
        return -1;
    }

    std::cout << "\n===== ModOrbElem -> EquinElem =====" << std::endl;
    std::cout << "  长半轴 a = " << ee.a()      << " m" << std::endl;
    std::cout << "  h        = " << ee.h()      << std::endl;
    std::cout << "  k        = " << ee.k()      << std::endl;
    std::cout << "  p        = " << ee.p()      << std::endl;
    std::cout << "  q        = " << ee.q()      << std::endl;
    std::cout << "  lambda   = " << ee.lambda() << " rad" << std::endl;

    // ============================================================
    // 第五步: EquinElem -> ModEquinElem (改进春分点根数)
    // ============================================================
    ModEquinElem mee;
    ee2mee(ee.data(), mee.data());

    std::cout << "\n===== EquinElem -> ModEquinElem =====" << std::endl;
    std::cout << "  半通径 p = " << mee.p() << " m" << std::endl;
    std::cout << "  f       = " << mee.f() << std::endl;
    std::cout << "  g       = " << mee.g() << std::endl;
    std::cout << "  h       = " << mee.h() << std::endl;
    std::cout << "  k       = " << mee.k() << std::endl;
    std::cout << "  L       = " << mee.L() << " rad" << std::endl;

    // ============================================================
    // 第六步: ModEquinElem -> CartState (回到直角坐标)
    // ============================================================
    Vector3d pos2, vel2;
    aModEquinElemToCart(mee, gm, pos2, vel2);

    std::cout << "\n===== ModEquinElem -> CartState =====" << std::endl;
    std::cout << "  pos (m):  " << pos2.x() << "  " << pos2.y() << "  " << pos2.z() << std::endl;
    std::cout << "  vel (m/s): " << vel2.x() << "  " << vel2.y() << "  " << vel2.z() << std::endl;

    // ============================================================
    // 第七步: 往返精度验证
    // ============================================================
    double posErr = (pos2 - pos).norm();
    double velErr = (vel2 - vel).norm();

    std::cout << "\n===== 往返转换精度 =====" << std::endl;
    std::cout << "  转换路径: OrbElem -> Cart -> ModOrb -> Equin -> ModEquin -> Cart" << std::endl;
    std::cout << "  位置误差 = " << std::scientific << posErr << " m" << std::endl;
    std::cout << "  速度误差 = " << std::scientific << velErr << " m/s" << std::endl;

    // ============================================================
    // 第八步: 使用 C 数组接口的等价转换
    // ============================================================
    double coeArr[6] = {coe.a(), coe.e(), coe.i(), coe.raan(), coe.argper(), coe.trueA()};
    double posArr[3], velArr[3];
    err = coe2rv(coeArr, gm, posArr, velArr);
    if (err != eNoError) {
        std::cerr << "coe2rv conversion failed, err=" << err << std::endl;
        return -1;
    }

    std::cout << "\n===== C 数组接口: coe2rv =====" << std::endl;
    std::cout << "  pos (m):  " << posArr[0] << "  " << posArr[1] << "  " << posArr[2] << std::endl;
    std::cout << "  vel (m/s): " << velArr[0] << "  " << velArr[1] << "  " << velArr[2] << std::endl;

    // ----- rv -> moe -> ee -> mee -> coe (反向链) -----
    double moeArr[6], eeArr[6], meeArr[6], coeArr2[6];
    rv2moe(posArr, velArr, gm, moeArr);
    moe2ee(moeArr, eeArr);
    ee2mee(eeArr, meeArr);
    mee2coe(meeArr, coeArr2);

    std::cout << "\n===== C 数组接口: rv -> moe -> ee -> mee -> coe =====" << std::endl;
    std::cout << "  a   = " << coeArr2[0] << " m    (original " << coeArr[0] << ")" << std::endl;
    std::cout << "  e   = " << coeArr2[1] << "      (original " << coeArr[1] << ")" << std::endl;
    std::cout << "  i   = " << coeArr2[2] << " rad  (original " << coeArr[2] << ")" << std::endl;
    std::cout << "  raan  = " << coeArr2[3] << " rad  (original " << coeArr[3] << ")" << std::endl;
    std::cout << "  argper  = " << coeArr2[4] << " rad  (original " << coeArr[4] << ")" << std::endl;
    std::cout << "  trueA  = " << coeArr2[5] << " rad  (original " << coeArr[5] << ")" << std::endl;

    // ============================================================
    // 总结: 五种根数的对比
    // ============================================================
    std::cout << "\n===== 五种轨道根数对照表 =====" << std::endl;
    std::cout << "  [1] CartState:" << std::endl;
    std::cout << "      r = (" << pos.x() << ", " << pos.y() << ", " << pos.z() << ")" << std::endl;
    std::cout << "      v = (" << vel.x() << ", " << vel.y() << ", " << vel.z() << ")" << std::endl;
    std::cout << "  [2] OrbElem (经典):" << std::endl;
    std::cout << "      a=" << coe.a() << "  e=" << coe.e() << "  i=" << coe.i() << std::endl;
    std::cout << "      raan=" << coe.raan() << "  argper=" << coe.argper() << "  trueA=" << coe.trueA() << std::endl;
    std::cout << "  [3] ModOrbElem (修正):" << std::endl;
    std::cout << "      rp=" << moe.rp() << "  e=" << moe.e() << "  i=" << moe.i() << std::endl;
    std::cout << "      raan=" << moe.raan() << "  argper=" << moe.argper() << "  trueA=" << moe.trueA() << std::endl;
    std::cout << "  [4] EquinElem (春分点):" << std::endl;
    std::cout << "      a=" << ee.a() << "  h=" << ee.h() << "  k=" << ee.k() << std::endl;
    std::cout << "      p=" << ee.p() << "  q=" << ee.q() << "  lambda=" << ee.lambda() << std::endl;
    std::cout << "  [5] ModEquinElem (改进春分点):" << std::endl;
    std::cout << "      p=" << mee.p() << "  f=" << mee.f() << "  g=" << mee.g() << std::endl;
    std::cout << "      h=" << mee.h() << "  k=" << mee.k() << "  L=" << mee.L() << std::endl;

    std::cout << "\nexample completed." << std::endl;
    return 0;
}
