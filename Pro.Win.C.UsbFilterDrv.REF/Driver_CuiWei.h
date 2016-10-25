#ifndef DRIVER_H
#define DRIVER_H 

#define DRIVERNAME "CuiWei_USBReadonlyFilter(0911080114)"

typedef struct tagDEVICE_EXTENSION {

	PDEVICE_OBJECT DeviceObject;			// ����չ�������豸����
	PDEVICE_OBJECT LowerDeviceObject;		// ͬһ��ջ����һ�� LowerDriver
	PDEVICE_OBJECT Pdo;						// PDO ����
	IO_REMOVE_LOCK RemoveLock;
	} DEVICE_EXTENSION, *PDEVICE_EXTENSION;



///////////////////////////////////////////////////////////////////////////////

// ȫ�ֺ���

VOID RemoveDevice(IN PDEVICE_OBJECT fdo);

NTSTATUS CompleteRequest(IN PIRP Irp, IN NTSTATUS status, IN ULONG_PTR info);

NTSTATUS DispatchForSCSI(IN PDEVICE_OBJECT fido, IN PIRP Irp);

#endif

