#ifndef GLOBALVALUE_H
#define GLOBALVALUE_H

#include <QString>

class GlobalValue
{
public:
    GlobalValue();
    enum ConnectStatus
    {
        LOCAL,
        OFFLINE
    };
    enum DeviceTyPe
    {
        PULSERADAR,
        TOWLINESOUND
    };
    enum ProtocolType
    {
        NORMALHART,
        GLPHART
    };
    enum HostType
    {
        FIRTST_HOST,
        SECOND_HOST
    };

    static ConnectStatus connectStatus;//连接状态
    static DeviceTyPe deviceType;//设备类型
    static QString portName;//串口号
    static HostType hostType;
    static ProtocolType protocolType;//协议类型
    static int address;//地址号
};

#endif // GLOBALVALUE_H
