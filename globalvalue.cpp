#include "globalvalue.h"

GlobalValue::GlobalValue()
{

}
//类静态变量初始化
GlobalValue::ConnectStatus GlobalValue::connectStatus = GlobalValue::OFFLINE;
GlobalValue::DeviceTyPe GlobalValue::deviceType = GlobalValue::PULSERADAR;
GlobalValue::ProtocolType GlobalValue::protocolType = GlobalValue::NORMALHART;
GlobalValue::HostType GlobalValue::hostType = GlobalValue::FIRTST_HOST;
QString GlobalValue::portName = QString("COM1");
int GlobalValue::address = 0;
