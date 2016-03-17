#include "hartasciihandle.h"

HartASCIIHandle::HartASCIIHandle()
{

}

void HartASCIIHandle::PackASC423(QByteArray &source, QByteArray &result)
{
    if(source.count() != 4)//不是四个字节
    {
        return;
    }
    QByteArray temparray;
    temparray = source.toUpper();//转换成大写
    char sourceChar[4];
    char resultChar[3];
    for(int i=0;i<4;i++)
    {
        sourceChar[i] = temparray.at(i);
    }

    unsigned char a,b;
    a = sourceChar[0];

    a <<= 2;//左移抛弃最高两位
    b = sourceChar[1];
    b &= 0x30;
    b >>= 4;
    a |= b;//第二个字节抛弃最高两位，剩下的6位中将最高两位补给第一个字节
    resultChar[0] = a;//获得压缩后的第一个字节


    a = sourceChar[1];

    a <<= 4;
    b = sourceChar[2];
    b >>= 2;
    b &= 0x0F;
    a |= b;
    resultChar[1] = a;


    a = sourceChar[2];

    a <<= 6;
    b = sourceChar[3];
    b &= 0x3f;
    a |= b;
    resultChar[2] = a;


    result.resize(0);
    for(int i = 0;i<3;i++)
    {
        result.append(resultChar[i]);
    }
}

void HartASCIIHandle::CompressASCString(QByteArray &data, QByteArray &result)
{
    if(data.count() %4 !=0 || data.count()>32)//hart协议中要压缩的最多32字节
    {
        return;
    }
    result.resize(0);
    const char * dataChar = data.constData();
    for(int i =0;i<data.count();i+=4)
    {
        QByteArray temparray(dataChar+i,4);
        QByteArray tempresult;
        PackASC423(temparray,tempresult);
        result.append(tempresult);
    }
}

void HartASCIIHandle::UnPack324(QByteArray &source, QByteArray &result)
{
    if(source.count() != 3)//需要将3个字节解压成4个字节
    {
        return;
    }
    char sourceChar[3];
    char resultChar[4];
    for(int i=0;i<3;i++)
    {
        sourceChar[i] = source.at(i);
    }
    unsigned char a,b,c;
    a =  sourceChar[0];
    a >>= 2;
    a |= (a&0x20 ? 0 : 0x40);//高位的第三位不为一的话最高两位补上01，避免出现小写字母，补上00的话对应asc码表数字区域
    resultChar[0] = a;

    a = sourceChar[0];
    a &= 0x03;//保留最低两位
    a <<= 4;
    b = sourceChar[1];
    b >>= 4;
    b |= a;
    b |= (b&0x20 ? 0 : 0x40);
    resultChar[1] = b;

    c = sourceChar[1];
    c &= 0xf;
    c <<= 2;
    a = sourceChar[2];
    a >>=6;
    c |= a;
    c |= (c&0x20 ? 0 : 0x40);
    resultChar[2] = c;

    a = sourceChar[2];
    a &= 0x3f;
    a |= (a&0x20 ? 0 : 0x40);
    resultChar[3] = a;
    result.resize(0);
    for(int i=0;i<4;i++)
    {
        result.append(resultChar[i]);
    }

}

void HartASCIIHandle::DecompressASCString(QByteArray &data, QByteArray &result)
{
    if(data.count() %3 !=0)
    {
        return;
    }
    result.resize(0);
    const char * dataChar = data.constData();
    for(int i=0;i<data.count();i+=3)
    {
        QByteArray temparray(dataChar+i,3);
        QByteArray tempresult;
        UnPack324(temparray,tempresult);
        result.append(tempresult);
    }
}
