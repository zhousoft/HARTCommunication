#ifndef HARTASCIIHANDLE_H
#define HARTASCIIHANDLE_H
#include <QByteArray>

class HartASCIIHandle
{
public:
    HartASCIIHandle();
    static void PackASC423(QByteArray &source,QByteArray &result);
    static void CompressASCString(QByteArray &data,QByteArray &result);
    static void UnPack324(QByteArray &source,QByteArray &result);
    static void DecompressASCString(QByteArray &data,QByteArray &result);
};

#endif // HARTASCIIHANDLE_H
