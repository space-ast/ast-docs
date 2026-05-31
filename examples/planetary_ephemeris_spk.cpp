#include "ast/SPKParser.hpp"
#include "ast/Vector.hpp"
#include "ast/JulianDate.hpp"
#include <iostream>
#include <iomanip>
#include <clocale>

AST_USING_NAMESPACE

int main()
{
    // 设置UTF-8编码，确保中文正确显示
    setlocale(LC_ALL, ".UTF-8");

    // 创建SPK解析器对象
    SPKParser spkParser;

    // 打开SPK星历文件（请替换为实际文件路径）
    std::string spkFilePath = "./data/Test/kernels/spk/de430.bsp";

    errc_t err = spkParser.parse(spkFilePath);
    if (err != eNoError) {
        std::cerr << "无法解析SPK星历文件: " << spkFilePath << std::endl;
        std::cerr << "错误码: " << err << std::endl;
        return -1;
    }

    std::cout << "成功打开SPK星历文件" << std::endl;

    // 打印文件注释信息
    std::cout << "\n=== SPK文件注释 ===" << std::endl;
    spkParser.printComment();

    // 获取SPK段描述符信息
    const auto& descriptors = spkParser.getDescriptors();
    std::cout << "\n=== SPK段信息 ===" << std::endl;
    std::cout << "段数量: " << descriptors.size() << std::endl;

    for (size_t i = 0; i < descriptors.size(); ++i) {
        const auto& desc = descriptors[i];
        std::cout << "\n段 " << (i + 1) << ":" << std::endl;
        std::cout << "  目标体ID: " << desc.target << std::endl;
        std::cout << "  中心体ID: " << desc.center << std::endl;
        std::cout << "  参考系ID: " << desc.frame << std::endl;
        std::cout << "  数据类型: " << desc.type << std::endl;
        std::cout << "  时间范围: " << desc.start_time << " 至 " << desc.end_time << " (ET秒)" << std::endl;
    }

    // 使用J2000.0 TDB时刻（ET=0）作为示例时间
    double et = 0.0;  // J2000.0 TDB时刻，即2000年1月1日12:00:00 TDB

    // 定义一些常用的NAIF天体ID
    const int SUN_ID = 10;
    const int EARTH_ID = 399;
    const int MOON_ID = 301;
    const int MARS_BARYCENTER_ID = 4; // 火星系质心ID

    // 获取地球的位置和速度（相对于地月系质心）
    Vector3d pos, vel;
    err = spkParser.getPosVelNative(et, EARTH_ID, pos, vel);

    if (err == eNoError) {
        std::cout << "\n=== J2000.0时刻地球状态(相对于地月系质心) ===" << std::endl;
        std::cout << "位置 (m):" << std::endl;
        std::cout << "  X: " << std::setprecision(8) << pos.x() << std::endl;
        std::cout << "  Y: " << std::setprecision(8) << pos.y() << std::endl;
        std::cout << "  Z: " << std::setprecision(8) << pos.z() << std::endl;
        std::cout << "速度 (m/s):" << std::endl;
        std::cout << "  Vx: " << std::setprecision(6) << vel.x() << std::endl;
        std::cout << "  Vy: " << std::setprecision(6) << vel.y() << std::endl;
        std::cout << "  Vz: " << std::setprecision(6) << vel.z() << std::endl;

        // 计算距离和速度大小
        double distance = pos.norm() / 1000.0;  // 转换为km
        double speed = vel.norm();              // m/s
        std::cout << "距地月系质心距离: " << std::setprecision(6) << distance << " km" << std::endl;
        std::cout << "速度大小: " << std::setprecision(4) << speed << " m/s" << std::endl;
    } else {
        std::cerr << "获取地球状态失败，错误码: " << err << std::endl;
    }

    // 获取火星系质心的位置（相对于太阳系质心）
    err = spkParser.getPosNative(et, MARS_BARYCENTER_ID, pos);
    if (err == eNoError) {
        std::cout << "\n=== J2000.0 TDB时刻火星系质心位置(相对于太阳系质心) ===" << std::endl;
        std::cout << "位置 (m):" << std::endl;
        std::cout << "  X: " << std::setprecision(8) << pos.x() << std::endl;
        std::cout << "  Y: " << std::setprecision(8) << pos.y() << std::endl;
        std::cout << "  Z: " << std::setprecision(8) << pos.z() << std::endl;
    } else {
        std::cerr << "获取火星系质心位置失败，错误码: " << err << std::endl;
    }

    // 查找特定目标和时间的SPK段描述符
    const SPK_Descriptor* desc = spkParser.findSpkDescriptor(EARTH_ID, et);
    if (desc != nullptr) {
        std::cout << "\n=== 地球在ET=" << et << "的SPK段信息 ===" << std::endl;
        std::cout << "目标体ID: " << desc->target << std::endl;
        std::cout << "中心体ID: " << desc->center << std::endl;
        std::cout << "参考系ID: " << desc->frame << std::endl;
        std::cout << "数据类型: " << desc->type << std::endl;
    } else {
        std::cout << "\n未找到地球在ET=" << et << "的SPK段" << std::endl;
    }

    std::cout << "\n示例程序执行完毕" << std::endl;

    return 0;
}
