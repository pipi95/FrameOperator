#ifndef EQUIPFRAME_H
#define EQUIPFRAME_H

#include "frameoperator_global.h"
#include <QObject>
#include <iterator>
#include <vector>

namespace FrameOperatorSpace {

Q_NAMESPACE
enum FrameStructureIDEnum {
    // 命名规则说明：
    // ID_x_y_z， 其中x代表外部设备名称，y代表该设备的输入（I）或输出（O）或用于数据交换等其他用途的
    // 数据帧（Ex）对于任管机内部的数据帧用（S）表示，z代表帧的帧的更详细描述信息（如飞控的1~6，A~I帧）。
    ID_Fr_Invalid = 0, // 非法数据帧，该帧不存在
    // 飞控相关的帧
    ID_FCC_O_Fr1 = 1, // 飞控遥测1~4,A~O帧
    ID_FCC_O_Fr2 = 2,
    ID_FCC_O_Fr3 = 3,
    ID_FCC_O_Fr4 = 4,
    ID_FCC_O_FrA = 5,
    ID_FCC_O_FrB = 6,
    ID_FCC_O_FrC = 7,
    ID_FCC_O_FrD = 8,
    ID_FCC_O_FrE = 9,
    ID_FCC_O_FrF = 10,
    ID_FCC_O_FrG = 11,
    ID_FCC_O_FrH = 12,
    ID_FCC_O_FrI = 13,
    ID_FCC_O_FrJ = 14,
    ID_FCC_O_FrO = 15,
    ID_FCC_I_Remote_Control = 16, //飞控遥控帧
    ID_FCC_I_MFCC = 17,
    ID_FCC_O_Dynamic = 18,
    // (预留18~30)
    // 光电相关的帧
    ID_EO_O_Telemetry = 31, //光电载荷遥测帧
    ID_EO_I_Remote_Control = 32, //光电载荷遥控帧
    ID_EO_I_Flight_MFCC_State = 33, //飞控I帧数据加离轨状态信息经任管发给光电
    ID_EO_O_MFCC = 34, //光电到任管的原始数据，经处理发送到火控机
    ID_EO_O_Dynamic = 35, //光电载荷动态读取帧长
    // (预留35~40)
    // 火控相关的帧
    ID_MFCC_O_FrC1 = 41, // 火控遥测帧C1
    ID_MFCC_O_FrB0 = 42, //火控遥测帧B0
    ID_MFCC_O_FrB1 = 43, //火控遥测帧B1
    ID_MFCC_O_FrB5 = 44, //火控遥测帧B5
    ID_MFCC_O_FrBB = 45, //火控遥测帧BB
    ID_MFCC_O_FrA1 = 46, //火控遥测帧A1
    ID_MFCC_O_FrA3 = 47, //火控遥测帧A3
    ID_MFCC_O_FrAF = 48, //火控全系统状态反馈，经任管发给飞控，和光电，暨AF帧
    ID_MFCC_O_Fr24 = 49, //火控遥测帧24，改由任管直接发送
    ID_MFCC_O_Fr51 = 50, //火控遥测帧51，改由任管直接发送
    ID_MFCC_I_EO = 51, //光电到任管到火控机，已处理帧头
    ID_MFCC_I_Flight_State = 52, //飞控机到任管的I帧，改变帧头后发到火控机，B0
    ID_MFCC_I_Remote_Control = 53, //火控遥控帧
    // (预留54~70)
    // 应答机相关的帧
    ID_ATC_O_Telemetry = 71, //应答机遥测数据帧
    ID_ATC_I_Remote_Control_PressAndAlt = 72, //应答机遥控数据帧加上气压高度后发给应答机
    ID_ATC_Ex_Remote_Control = 72, //应答机遥控帧
    // (预留73~80)
    // 链路相关的帧
    ID_Link_I_Telemetry = 81,
    ID_Link_O_Remote_Control = 82, // GDC上传遥控帧，包括所有遥控指令
    ID_Link_I_Syn422 = 83, // 同步422数据给链路
    // (预留84~90)
    // GPS、惯导相关
    ID_DGPS_I_DCode = 91, //processed DGPS data
    ID_INU_O_Info = 92, // 惯导数据
    ID_GPS_O_Info = 93, // 侧向GPS数据
    ID_DGPS_I_PositionA = 94, // DGPS位置数据，在GDC中首次使用
    ID_DGPS_I_PositionB = 95, // DGPS位置数据，在GDC中首次使用
    // (预留96~100)
    // 雷达相关的帧
    ID_Radar_I_Remote_Control = 101, // 雷达遥控帧
    ID_Radar_O_Telemetry = 102, // 雷达遥测
    ID_AIS_O_Telemetry = 103, // AIS
    // (预留103~150)
    // 任管机相关的帧
    ID_MMC_S_Telemetry = 151, // 任管机自身的遥测数据帧
    ID_MMC_S_Remote_Control = 152, //MMC remote control frame
    ID_MMC_O_Telemetry = 153, // 任管机拼接后的载荷遥测帧
    ID_MMC_S_Merged_PLTelemetry = 154,
    ID_MMC_S_Modifyed_FCC_FrJ = 155,

    //GDC组帧软件帧定义
    ID_FCC_I_TC = 201, //飞控遥控帧
    ID_EO_I_TC = 202, //光电遥控帧
    ID_MFCU_I_TC = 203, //火控遥控帧
    ID_SAR_I_TC = 204, //SAR遥控帧
    ID_RADAR_I_TC = 205, //RADAR遥控帧
    ID_COMINT_I_TC = 206, //COMINT遥控帧
    ID_ELINT_I_TC = 207, //ELINT遥控帧
    ID_SIGINT_I_TC = 208, //SIGINT遥控帧
    ID_DGPS1_I_TC = 209, //GPS差分码
    ID_DGPS2_I_TC = 210, //GPS差分码
    ID_BEIDOU_I_TC = 211,
    ID_GLONASS_I_TC = 212,
    ID_GALILEO_I_TC = 213,
    ID_STATION_I_TC = 214,
    ID_UPLINK_O_TC = 215, //上行遥控数据帧
    ID_FCC_O_TC = 216,
    ID_EO_O_TC = 217,
    ID_MFCU_O_TC = 218,
    ID_SAR_O_TC = 219,
    ID_DGPS1_O_TC = 220,
    ID_DGPS2_O_TC = 221,
    //遥测数据帧协议
    ID_FCC_I_TM = 231,
    ID_EO_I_TM = 232,
    ID_MFCU_I_TM = 233,
    ID_SAR_I_TM = 234,
    ID_RADAR_I_TM = 235,
    ID_COMINT_I_TM = 236,
    ID_ELINT_I_TM = 237,
    ID_SIGINT_I_TM = 238,
    ID_SEAT_ASSIGN = 239, //席位分配帧
    ID_UNITY_I_TC = 240
};
Q_ENUM_NS(FrameStructureIDEnum)

class FRAMEOPERATORSHARED_EXPORT EquipFrame {
public:
    EquipFrame();
    EquipFrame(const EquipFrame& fr);
    EquipFrame(const QByteArray& content, FrameStructureIDEnum id);
    EquipFrame& operator=(const EquipFrame& fr);
    void Print() const;
    QString ToString() const;
    QByteArray GetContent() const;
    FrameStructureIDEnum GetID() const;
    void SetContent(const QByteArray& content);
    void SetID(FrameStructureIDEnum id);
    void SetFrameID(FrameStructureIDEnum id) { SetID(id); }
    void SetFrameContent(const std::vector<unsigned char>& content)
    {
        using namespace std;
        QByteArray newContent;
        for (vector<unsigned char>::const_iterator i = content.begin();
             i != content.end(); ++i) {
            newContent.append(static_cast<char>(*i));
        }
        SetContent(newContent);
    }

private:
    QByteArray content;
    FrameStructureIDEnum id;
};
}
#endif // EQUIPFRAME_H
