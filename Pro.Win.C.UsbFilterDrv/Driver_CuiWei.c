/*
 *  ѧУ���Ͼ�����ѧ�����ѧԺ
 *  ���ߣ�����
 *  �༶���������һ��(09065501)
 *  ѧ�ţ�0911080114
 *  �γ̣��������ϵͳ
 *  ��������USBֻ��������������WDM��
 *  ָ����ʦ������
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
{	// IO�����豸�������
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
{	// IO��Ӧ�������
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
{	// ж���豸��ʾ��Ϣ
	PAGED_CODE();
	KdPrint((DRIVERNAME " - Entering DriverUnload: DriverObject %8.8lX\n", DriverObject));
}

///////////////////////////////////////////////////////////////////////////////
#pragma PAGEDCODE
ULONG GetDeviceTypeToUse(PDEVICE_OBJECT pdo)
{	//��ȡʹ�õ��豸����
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
{	// ����豸
	NTSTATUS status;
	PDEVICE_OBJECT fido;
	PDEVICE_EXTENSION pdx;
	PDEVICE_OBJECT fdo;

	PAGED_CODE();

	status = IoCreateDevice(DriverObject, sizeof(DEVICE_EXTENSION), NULL,
		GetDeviceTypeToUse(pdo), 0, FALSE, &fido);
	if (!NT_SUCCESS(status))
	{	// ����ʧ��
		KdPrint((DRIVERNAME " - IoCreateDevice failed - %X\n", status));
		return status;
	}
	pdx = (PDEVICE_EXTENSION) fido->DeviceExtension;

	do
	{
		IoInitializeRemoveLock(&pdx->RemoveLock, 0, 0, 0);
		pdx->DeviceObject = fido;
		pdx->Pdo = pdo;
		
		//���������������ڵײ�����֮��
		fdo = IoAttachDeviceToDeviceStack(fido, pdo);
		if (!fdo)
		{	//����ʧ��
			KdPrint((DRIVERNAME " - IoAttachDeviceToDeviceStack failed\n"));
			status = STATUS_DEVICE_REMOVED;
			break;
		}

		//��¼�ײ�����
		pdx->LowerDeviceObject = fdo;
		//���ڲ�֪���ײ�������ֱ��IO����BufferIO����˽���־������
		fido->Flags |= fdo->Flags & (DO_DIRECT_IO | DO_BUFFERED_IO | DO_POWER_PAGABLE);
		//�����ʼ����־�Ա��ܹ���ȡIRPs
		fido->Flags &= ~DO_DEVICE_INITIALIZING;
	}	while (FALSE);
	// ��ɳ�ʼ��

	if (!NT_SUCCESS(status))
	{	// �ͷ���Դ
		if (pdx->LowerDeviceObject)
			IoDetachDevice(pdx->LowerDeviceObject);
		IoDeleteDevice(fido);
	}

	return status;
}

///////////////////////////////////////////////////////////////////////////////
#pragma LOCKEDCODE
NTSTATUS CompleteRequest(IN PIRP Irp, IN NTSTATUS status, IN ULONG_PTR info)
{	// �������
	Irp->IoStatus.Status = status;
	Irp->IoStatus.Information = info;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return status;
}

NTSTATUS USBSCSICompletion( IN PDEVICE_OBJECT DeviceObject,IN PIRP Irp,IN PVOID Context )
{	// ִ��USBSCSI��д��������
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

		modeData->DeviceSpecificParameter |= MODE_DSP_WRITE_PROTECT; // д����ѡ��
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
{	// SCSI�ַ�����
	PDEVICE_EXTENSION pdx = (PDEVICE_EXTENSION) fido->DeviceExtension;
	
	PIO_STACK_LOCATION irpStack = IoGetCurrentIrpStackLocation(Irp); 

	// �޸��ӽ�������������
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
{	// ����ͨ�����⣩��Irp���д���
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
	
	// �޸��ӽ�������������
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
{	// ��Դ�ַ�����
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
{	// ���弴�÷ַ�����
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
	{	// ��Ӧ
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
	{	// �豸����
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
		RemoveDevice(fido);	// �Ƴ��豸
		return status;
	}

	// �������͵ļ��弴������
	IoSkipCurrentIrpStackLocation(Irp);
	status = IoCallDriver(pdx->LowerDeviceObject, Irp);
	IoReleaseRemoveLock(&pdx->RemoveLock, Irp);
	return status;
}

///////////////////////////////////////////////////////////////////////////////
#pragma PAGEDCODE
VOID RemoveDevice(IN PDEVICE_OBJECT fido)
{	// �Ƴ��豸
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
{	// �����������ں���
	int i;
	KdPrint((DRIVERNAME " - Entering DriverEntry: DriverObject %8.8lX\n", DriverObject));
	// ��ʼ������ָ��
	DriverObject->DriverUnload = DriverUnload;
	DriverObject->DriverExtension->AddDevice = AddDevice;
	for (i = 0; i < length(DriverObject->MajorFunction); ++i)
		DriverObject->MajorFunction[i] = DispatchAny;
	DriverObject->MajorFunction[IRP_MJ_POWER] = DispatchPower;
	DriverObject->MajorFunction[IRP_MJ_PNP] = DispatchPnp;
	DriverObject->MajorFunction[IRP_MJ_SCSI] = DispatchForSCSI;
	return STATUS_SUCCESS;
}
