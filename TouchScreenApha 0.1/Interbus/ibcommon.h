#ifndef IBCOMMON_H
#define IBCOMMON_H

#include <QMetaType>

enum RegGetStatusTypes
{
    GetSuccess,
    GetBusy,
    GetNacked,
    GetCRCErr,
    GetTimeout,
    GetComError,
    GetPortClosed,
    GetPortNotFound
};
Q_DECLARE_METATYPE ( RegGetStatusTypes )

enum RegSetStatusTypes
{
    SetSuccess,
    SetBusy,
    SetNacked,
    SetCRCErr,
    SetTimeout,
    SetComError,
    SetPortClosed,
    SetPortNotFound
};
Q_DECLARE_METATYPE ( RegSetStatusTypes )

enum RegWriteTypes
{
    regWrite,
    regWriteSET,
    regWriteCLR,
    regWriteTGL
};
Q_DECLARE_METATYPE ( RegWriteTypes )

enum PriorityTypes
{
    Priority_Low,
    Priority_High
};
Q_DECLARE_METATYPE ( PriorityTypes )

enum RegDataTypes
{
    DataType_Unknown,
    DataType_Array,
    DataType_U8,
    DataType_S8,
    DataType_U16,
    DataType_S16,
    DataType_U32,
    DataType_S32,
    DataType_F32,
    DataType_Ascii,
    DataType_Paramset
};
Q_DECLARE_METATYPE ( RegDataTypes )

enum RxStates
{
    RxStStopped,
    RxStHunting_SOT,
    RxStHunting_EOT,
    RxStMessageReady,
    RxStTimeout_Error,
    RxStCRC_Error,
    RxStGarbage_Error,
    RxStOverrun_Error,
    RxStSync_Error
};
Q_DECLARE_METATYPE ( RxStates )


#endif // IBCOMMON_H
