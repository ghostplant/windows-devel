#ifndef DRIVER_H
#define DRIVER_H 

#define DRIVERNAME "CuiWei_USBReadonlyFilter(0911080114)"

typedef struct tagDEVICE_EXTENSION {

	PDEVICE_OBJECT DeviceObject;			// 该扩展所属的设备对象
	PDEVICE_OBJECT LowerDeviceObject;		// 同一堆栈的下一个 LowerDriver
	PDEVICE_OBJECT Pdo;						// PDO 对象
	IO_REMOVE_LOCK RemoveLock;
	} DEVICE_EXTENSION, *PDEVICE_EXTENSION;



///////////////////////////////////////////////////////////////////////////////

// 全局函数

VOID RemoveDevice(IN PDEVICE_OBJECT fdo);

NTSTATUS CompleteRequest(IN PIRP Irp, IN NTSTATUS status, IN ULONG_PTR info);

NTSTATUS DispatchForSCSI(IN PDEVICE_OBJECT fido, IN PIRP Irp);

#endif

