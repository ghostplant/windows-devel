/*
 *  学校：南京理工大学计算机学院
 *  作者：崔嵬
 *  班级：软件工程一班(09065501)
 *  学号：0911080114
 *  课程：网络操作系统
 *  工程名：USB只读过滤驱动程序（WDM）
 *  指导老师：俞研
 */

#include <wdm.h>
#include <stdio.h>
#include <srb.h>
#include <scsi.h>

#define PAGEDCODE code_seg("PAGE")
#define LOCKEDCODE code_seg()
#define INITCODE code_seg("INIT")
#define PAGEDDATA data_seg("PAGEDATA")
#define LOCKEDDATA data_seg()
#define INITDATA data_seg("INITDATA")
#define length(p) (sizeof(p)/sizeof(*(p)))

#include "Driver_CuiWei.h"

NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject,IN PUNICODE_STRING RegistryPath);

///////////////////////////////////////////////////////////////////////////////
#pragma LOCKEDCODE
NTSTATUS StartDeviceCompletionRoutine(PDEVICE_OBJECT fido, PIRP Irp, PDEVICE_EXTENSION pdx)
{	// IO加载设备完成例程
	if (Irp->PendingReturned)
		IoMarkIrpPending(Irp);
	if (pdx->LowerDeviceObject->Characteristics & FILE_REMOVABLE_MEDIA)
		fido->Characteristics |= FILE_REMOVABLE_MEDIA;
	IoReleaseRemoveLock(&pdx->RemoveLock, Irp);
	return STATUS_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////
#pragma LOCKEDCODE
NTSTATUS UsageNotificationCompletionRoutine(PDEVICE_OBJECT fido, PIRP Irp, PDEVICE_EXTENSION pdx)
{	// IO响应完成例程
	if (Irp->PendingReturned)
		IoMarkIrpPending(Irp);
	// If lower driver cleared pageable flag, we must do the same
	if (!(pdx->LowerDeviceObject->Flags & DO_POWER_PAGABLE))
		fido->Flags &= ~DO_POWER_PAGABLE;
	IoReleaseRemoveLock(&pdx->RemoveLock, Irp);
	return STATUS_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////
#pragma PAGEDCODE
VOID DriverUnload(IN PDRIVER_OBJECT DriverObject)
{	// 卸载设备提示消息
	PAGED_CODE();
	KdPrint((DRIVERNAME " - Entering DriverUnload: DriverObject %8.8lX\n", DriverObject));
}

///////////////////////////////////////////////////////////////////////////////
#pragma PAGEDCODE
ULONG GetDeviceTypeToUse(PDEVICE_OBJECT pdo)
{	//获取使用的设备类型
	ULONG devtype;
	PDEVICE_OBJECT ldo = IoGetAttachedDeviceReference(pdo);
	if (!ldo)
		return FILE_DEVICE_UNKNOWN;
	devtype = ldo->DeviceType;
	ObDereferenceObject(ldo);
	return devtype;
}

///////////////////////////////////////////////////////////////////////////////
NTSTATUS AddDevice(IN PDRIVER_OBJECT DriverObject, IN PDEVICE_OBJECT pdo)
{	// 添加设备
	NTSTATUS status;
	PDEVICE_OBJECT fido;
	PDEVICE_EXTENSION pdx;
	PDEVICE_OBJECT fdo;

	PAGED_CODE();

	status = IoCreateDevice(DriverObject, sizeof(DEVICE_EXTENSION), NULL,
		GetDeviceTypeToUse(pdo), 0, FALSE, &fido);
	if (!NT_SUCCESS(status))
	{	// 创建失败
		KdPrint((DRIVERNAME " - IoCreateDevice failed - %X\n", status));
		return status;
	}
	pdx = (PDEVICE_EXTENSION) fido->DeviceExtension;

	do
	{
		IoInitializeRemoveLock(&pdx->RemoveLock, 0, 0, 0);
		pdx->DeviceObject = fido;
		pdx->Pdo = pdo;
		
		//将过滤驱动附加在底层驱动之上
		fdo = IoAttachDeviceToDeviceStack(fido, pdo);
		if (!fdo)
		{	//附加失败
			KdPrint((DRIVERNAME " - IoAttachDeviceToDeviceStack failed\n"));
			status = STATUS_DEVICE_REMOVED;
			break;
		}

		//记录底层驱动
		pdx->LowerDeviceObject = fdo;
		//由于不知道底层驱动是直接IO还是BufferIO，因此将标志都置上
		fido->Flags |= fdo->Flags & (DO_DIRECT_IO | DO_BUFFERED_IO | DO_POWER_PAGABLE);
		//清除初始化标志以便能够获取IRPs
		fido->Flags &= ~DO_DEVICE_INITIALIZING;
	}	while (FALSE);
	// 完成初始化

	if (!NT_SUCCESS(status))
	{	// 释放资源
		if (pdx->LowerDeviceObject)
			IoDetachDevice(pdx->LowerDeviceObject);
		IoDeleteDevice(fido);
	}

	return status;
}

///////////////////////////////////////////////////////////////////////////////
#pragma LOCKEDCODE
NTSTATUS CompleteRequest(IN PIRP Irp, IN NTSTATUS status, IN ULONG_PTR info)
{	// 完成请求
	Irp->IoStatus.Status = status;
	Irp->IoStatus.Information = info;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return status;
}

NTSTATUS USBSCSICompletion( IN PDEVICE_OBJECT DeviceObject,IN PIRP Irp,IN PVOID Context )
{	// 执行USBSCSI的写保护功能
	PIO_STACK_LOCATION irpStack;
	PSCSI_REQUEST_BLOCK CurSrb;
	PCDB cdb;
	UCHAR opCode;
	PMODE_PARAMETER_HEADER modeData;

	PDEVICE_EXTENSION pdx = ( PDEVICE_EXTENSION )
                                   DeviceObject->DeviceExtension;

	IoAcquireRemoveLock(&pdx->RemoveLock,Irp);

    irpStack = IoGetCurrentIrpStackLocation( Irp );

	CurSrb=irpStack->Parameters.Scsi.Srb; 
	cdb = (PCDB)CurSrb->Cdb; 
	opCode=cdb->CDB6GENERIC.OperationCode; 

	if(opCode==SCSIOP_MODE_SENSE  && CurSrb->DataBuffer 
		&& CurSrb->DataTransferLength >= 
		sizeof(MODE_PARAMETER_HEADER))
	{
		KdPrint(("SCSIOP_MODE_SENSE comming!\n"));

		modeData = (PMODE_PARAMETER_HEADER)CurSrb->DataBuffer;

		modeData->DeviceSpecificParameter |= MODE_DSP_WRITE_PROTECT; // 写保护选项
	} 

	if ( Irp->PendingReturned )
	{
		IoMarkIrpPending( Irp );
	} 

	IoReleaseRemoveLock(&pdx->RemoveLock,Irp);

	return Irp->IoStatus.Status ;
} 
 
#pragma LOCKEDCODE
NTSTATUS DispatchForSCSI(IN PDEVICE_OBJECT fido, IN PIRP Irp)
{	// SCSI分发例程
	PDEVICE_EXTENSION pdx = (PDEVICE_EXTENSION) fido->DeviceExtension;
	
	PIO_STACK_LOCATION irpStack = IoGetCurrentIrpStackLocation(Irp); 

	// 无附加进程下允许请求
	NTSTATUS status;
	status = IoAcquireRemoveLock(&pdx->RemoveLock, Irp);
	if (!NT_SUCCESS(status))
		return CompleteRequest(Irp, status, 0);

	IoCopyCurrentIrpStackLocationToNext(Irp);
	
	IoSetCompletionRoutine( Irp,USBSCSICompletion,NULL,TRUE,TRUE,TRUE ); 
	status = IoCallDriver(pdx->LowerDeviceObject, Irp);
	IoReleaseRemoveLock(&pdx->RemoveLock, Irp);
	return status;
}

///////////////////////////////////////////////////////////////////////////////
#pragma LOCKEDCODE
NTSTATUS DispatchAny(IN PDEVICE_OBJECT fido, IN PIRP Irp)
{	// 对普通（任意）的Irp进行处理
	PDEVICE_EXTENSION pdx = (PDEVICE_EXTENSION) fido->DeviceExtension;
	PIO_STACK_LOCATION stack = IoGetCurrentIrpStackLocation(Irp);
#if DBG
	static char* irpname[] = 
	{
		"IRP_MJ_CREATE",
		"IRP_MJ_CREATE_NAMED_PIPE",
		"IRP_MJ_CLOSE",
		"IRP_MJ_READ",
		"IRP_MJ_WRITE",
		"IRP_MJ_QUERY_INFORMATION",
		"IRP_MJ_SET_INFORMATION",
		"IRP_MJ_QUERY_EA",
		"IRP_MJ_SET_EA",
		"IRP_MJ_FLUSH_BUFFERS",
		"IRP_MJ_QUERY_VOLUME_INFORMATION",
		"IRP_MJ_SET_VOLUME_INFORMATION",
		"IRP_MJ_DIRECTORY_CONTROL",
		"IRP_MJ_FILE_SYSTEM_CONTROL",
		"IRP_MJ_DEVICE_CONTROL",
		"IRP_MJ_INTERNAL_DEVICE_CONTROL",
		"IRP_MJ_SHUTDOWN",
		"IRP_MJ_LOCK_CONTROL",
		"IRP_MJ_CLEANUP",
		"IRP_MJ_CREATE_MAILSLOT",
		"IRP_MJ_QUERY_SECURITY",
		"IRP_MJ_SET_SECURITY",
		"IRP_MJ_POWER",
		"IRP_MJ_SYSTEM_CONTROL",
		"IRP_MJ_DEVICE_CHANGE",
		"IRP_MJ_QUERY_QUOTA",
		"IRP_MJ_SET_QUOTA",
		"IRP_MJ_PNP",
	};
	
#endif
	
	// 无附加进程下允许请求
	NTSTATUS status;
	status = IoAcquireRemoveLock(&pdx->RemoveLock, Irp);
	if (!NT_SUCCESS(status))
		return CompleteRequest(Irp, status, 0);
	IoSkipCurrentIrpStackLocation(Irp);
	status = IoCallDriver(pdx->LowerDeviceObject, Irp);
	IoReleaseRemoveLock(&pdx->RemoveLock, Irp);
	return status;
}


///////////////////////////////////////////////////////////////////////////////
NTSTATUS DispatchPower(IN PDEVICE_OBJECT fido, IN PIRP Irp)
{	// 电源分发例程
	PDEVICE_EXTENSION pdx;
	NTSTATUS status;

#if DBG
	PIO_STACK_LOCATION stack = IoGetCurrentIrpStackLocation(Irp);
	ULONG fcn = stack->MinorFunction;
	static char* fcnname[] = 
	{
		"IRP_MN_WAIT_WAKE",
		"IRP_MN_POWER_SEQUENCE",
		"IRP_MN_SET_POWER",
		"IRP_MN_QUERY_POWER",
	};

	if (fcn == IRP_MN_SET_POWER || fcn == IRP_MN_QUERY_POWER)
	{
		static char* sysstate[] = 
		{
			"PowerSystemUnspecified",
			"PowerSystemWorking",
			"PowerSystemSleeping1",
			"PowerSystemSleeping2",
			"PowerSystemSleeping3",
			"PowerSystemHibernate",
			"PowerSystemShutdown",
			"PowerSystemMaximum",
		};

		static char* devstate[] = 
		{
			"PowerDeviceUnspecified",
			"PowerDeviceD0",
			"PowerDeviceD1",
			"PowerDeviceD2",
			"PowerDeviceD3",
			"PowerDeviceMaximum",
		};

		ULONG context = stack->Parameters.Power.SystemContext;
		POWER_STATE_TYPE type = stack->Parameters.Power.Type;
		KdPrint((DRIVERNAME " - IRP_MJ_POWER (%s)", fcnname[fcn]));
		if (type == SystemPowerState)
			KdPrint((", SystemPowerState = %s\n", sysstate[stack->Parameters.Power.State.SystemState]));
		else
			KdPrint((", DevicePowerState = %s\n", devstate[stack->Parameters.Power.State.DeviceState]));
	}
	else if (fcn < length(fcnname))
		KdPrint((DRIVERNAME " - IRP_MJ_POWER (%s)\n", fcnname[fcn]));
	else
		KdPrint((DRIVERNAME " - IRP_MJ_POWER (%2.2X)\n", fcn));
#endif // DBG

	pdx = (PDEVICE_EXTENSION) fido->DeviceExtension;
	PoStartNextPowerIrp(Irp);	// must be done while we own the IRP
	status = IoAcquireRemoveLock(&pdx->RemoveLock, Irp);
	if (!NT_SUCCESS(status))
		return CompleteRequest(Irp, status, 0);
	IoSkipCurrentIrpStackLocation(Irp);
	status = PoCallDriver(pdx->LowerDeviceObject, Irp);
	IoReleaseRemoveLock(&pdx->RemoveLock, Irp);
	return status;
}


///////////////////////////////////////////////////////////////////////////////
NTSTATUS DispatchPnp(IN PDEVICE_OBJECT fido, IN PIRP Irp)
{	// 即插即用分发例程
	PIO_STACK_LOCATION stack = IoGetCurrentIrpStackLocation(Irp);
	ULONG fcn = stack->MinorFunction;
	NTSTATUS status;
	PDEVICE_EXTENSION pdx = (PDEVICE_EXTENSION) fido->DeviceExtension;
#if DBG 
	static char* pnpname[] = 
	{
		"IRP_MN_START_DEVICE",
		"IRP_MN_QUERY_REMOVE_DEVICE",
		"IRP_MN_REMOVE_DEVICE",
		"IRP_MN_CANCEL_REMOVE_DEVICE",
		"IRP_MN_STOP_DEVICE",
		"IRP_MN_QUERY_STOP_DEVICE",
		"IRP_MN_CANCEL_STOP_DEVICE",
		"IRP_MN_QUERY_DEVICE_RELATIONS",
		"IRP_MN_QUERY_INTERFACE",
		"IRP_MN_QUERY_CAPABILITIES",
		"IRP_MN_QUERY_RESOURCES",
		"IRP_MN_QUERY_RESOURCE_REQUIREMENTS",
		"IRP_MN_QUERY_DEVICE_TEXT",
		"IRP_MN_FILTER_RESOURCE_REQUIREMENTS",
		"",
		"IRP_MN_READ_CONFIG",
		"IRP_MN_WRITE_CONFIG",
		"IRP_MN_EJECT",
		"IRP_MN_SET_LOCK",
		"IRP_MN_QUERY_ID",
		"IRP_MN_QUERY_PNP_DEVICE_STATE",
		"IRP_MN_QUERY_BUS_INFORMATION",
		"IRP_MN_DEVICE_USAGE_NOTIFICATION",
		"IRP_MN_SURPRISE_REMOVAL",
		"IRP_MN_QUERY_LEGACY_BUS_INFORMATION",
	};

#endif

	status = IoAcquireRemoveLock(&pdx->RemoveLock, Irp);
	if (!NT_SUCCESS(status))
		return CompleteRequest(Irp, status, 0);
#if DBG 
	if (fcn < length(pnpname))
		KdPrint((DRIVERNAME " - IRP_MJ_PNP (%s)\n", pnpname[fcn]));
	else
		KdPrint((DRIVERNAME " - IRP_MJ_PNP (%2.2X)\n", fcn));
#endif

	// Handle usage notification specially in order to track power pageable
	// flag correctly. We need to avoid allowing a non-pageable handler to be
	// layered on top of a pageable handler.
	if (fcn == IRP_MN_DEVICE_USAGE_NOTIFICATION)
	{	// 响应
		if (!fido->AttachedDevice || (fido->AttachedDevice->Flags & DO_POWER_PAGABLE))
			fido->Flags |= DO_POWER_PAGABLE;
		IoCopyCurrentIrpStackLocationToNext(Irp);
		IoSetCompletionRoutine(Irp, (PIO_COMPLETION_ROUTINE) UsageNotificationCompletionRoutine,
			(PVOID) pdx, TRUE, TRUE, TRUE);
		return IoCallDriver(pdx->LowerDeviceObject, Irp);
	}

	// Handle start device specially in order to correctly inherit
	// FILE_REMOVABLE_MEDIA
	if (fcn == IRP_MN_START_DEVICE)
	{	// 设备启动
		IoCopyCurrentIrpStackLocationToNext(Irp);
		IoSetCompletionRoutine(Irp, (PIO_COMPLETION_ROUTINE) StartDeviceCompletionRoutine,
			(PVOID) pdx, TRUE, TRUE, TRUE);
		return IoCallDriver(pdx->LowerDeviceObject, Irp);
	}

	// Handle remove device specially in order to cleanup device stack
	if (fcn == IRP_MN_REMOVE_DEVICE)
	{
		IoSkipCurrentIrpStackLocation(Irp);
		status = IoCallDriver(pdx->LowerDeviceObject, Irp);
		IoReleaseRemoveLockAndWait(&pdx->RemoveLock, Irp);
		RemoveDevice(fido);	// 移除设备
		return status;
	}

	// 其他类型的即插即用请求
	IoSkipCurrentIrpStackLocation(Irp);
	status = IoCallDriver(pdx->LowerDeviceObject, Irp);
	IoReleaseRemoveLock(&pdx->RemoveLock, Irp);
	return status;
}

///////////////////////////////////////////////////////////////////////////////
#pragma PAGEDCODE
VOID RemoveDevice(IN PDEVICE_OBJECT fido)
{	// 移除设备
	PDEVICE_EXTENSION pdx;
	KdPrint(("Enter RemoveDevice"));
	PAGED_CODE();
	pdx = (PDEVICE_EXTENSION) fido->DeviceExtension;
	if (pdx->LowerDeviceObject)
		IoDetachDevice(pdx->LowerDeviceObject);
	IoDeleteDevice(fido);
}

///////////////////////////////////////////////////////////////////////////////
#pragma INITCODE 
NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject,IN PUNICODE_STRING RegistryPath)
{	// 驱动程序的入口函数
	int i;
	KdPrint((DRIVERNAME " - Entering DriverEntry: DriverObject %8.8lX\n", DriverObject));
	// 初始化函数指针
	DriverObject->DriverUnload = DriverUnload;
	DriverObject->DriverExtension->AddDevice = AddDevice;
	for (i = 0; i < length(DriverObject->MajorFunction); ++i)
		DriverObject->MajorFunction[i] = DispatchAny;
	DriverObject->MajorFunction[IRP_MJ_POWER] = DispatchPower;
	DriverObject->MajorFunction[IRP_MJ_PNP] = DispatchPnp;
	DriverObject->MajorFunction[IRP_MJ_SCSI] = DispatchForSCSI;
	return STATUS_SUCCESS;
}
