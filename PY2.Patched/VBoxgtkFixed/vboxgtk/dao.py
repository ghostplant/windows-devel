#!/usr/bin/python2

import os
import threading
import vboxapi

import iface
import util

class WatcherThread(threading.Thread):
    """Watches a VM session in a separate thread and notifies the
    interface of the vm state changes"""

    def __init__(self, vm):
        threading.Thread.__init__(self)
        self.vm = vm

    def run(self):
        session = get_session(self.vm, False)
        console = session.console
        evsource = console.eventSource
        listener = evsource.createListener()
        evs = [cts.VBoxEventType_OnStateChanged]
        evsource.registerListener(listener, evs, False)
        keep_running = True
        while keep_running:
            try:
                event = evsource.getEvent(listener, 1000)
            except:
                keep_running = False
            else:
                if event is not None:
                    iface.vm_updated(self.vm, True)
        iface.vm_updated(self.vm, True)  # one last update is needed
        return 0


class SysProps():
    """Struct to store the important system parameters"""
    def __init__(self, min_ncpus, max_ncpus, min_ram, max_ram, min_vram,
                 max_vram, max_hdsize, vm_folder, hwvirtex, pae, gpuaccel):
        self.min_ncpus = min_ncpus
        self.max_ncpus = max_ncpus
        self.min_ram = min_ram
        self.max_ram = max_ram
        self.min_vram = min_vram
        self.max_vram = max_vram
        self.max_hdsize = max_hdsize
        self.vm_folder = vm_folder
        self.hwvirtex = hwvirtex
        self.pae = pae
        self.gpuaccel = gpuaccel


# Singleton

def modmethod(func):
    class modproxy(object):
        __getattribute__ = func.func_globals.__getitem__
        __setattr__ = func.func_globals.__setitem__
    self = modproxy()
    def call_with_method(*args,**kwargs):
        return func(self,*args,**kwargs)
    call_with_method.func_name = func.func_name
    return call_with_method

@modmethod
def init(self):
    """Reads the needed info from VirtualBoxManager"""
    self.vbox_mgr = vboxapi.VirtualBoxManager(None, None)
    self.vbox = vbox_mgr.vbox
    
    self.mgr = vbox_mgr.mgr
    self.cts = vbox_mgr.constants
    self.vbox.openMedium("/usr/share/virtualbox/VBoxGuestAdditions.iso", cts.DeviceType_DVD, cts.AccessMode_ReadOnly, False)
    vbsp = vbox.systemProperties
    max_ncpus = min(vbox.host.processorCount, vbsp.maxGuestCPUCount)
    max_ram = min(vbox.host.memorySize, vbsp.maxGuestRAM)
    hwvirtex = vbox.host.getProcessorFeature(cts.ProcessorFeature_HWVirtEx)
    pae = vbox.host.getProcessorFeature(cts.ProcessorFeature_PAE)
    self.sysprops = SysProps(vbsp.minGuestCPUCount, max_ncpus, vbsp.minGuestRAM,
                             max_ram, vbsp.minGuestVRAM, vbsp.maxGuestVRAM,
                             vbsp.infoVDSize, vbsp.defaultMachineFolder,
                             hwvirtex, pae, vbox.host.acceleration3DAvailable)
    self.os_types = sorted(vbox.getGuestOSTypes(), key=lambda x:x.id)
    self.dvd_images = vbox.getDVDImages()
    self.dvd_host_drives = vbox.host.getDVDDrives()
    self.sessions = {}
    self.vms = []
    for vm in vbox.getMachines():
        if not vm.accessible:
            continue
        self.vms.append(vm)
        if vm.state >= cts.MachineState_Running:
            watch_vm(vm)

def exit():
    """Make sure to close all the pending sessions before exiting"""
    for vm in vms:
        close_session(vm)
    return False

def check_progress_status(progress, err_msg):
    if progress.resultCode:
        err_text = (progress.errorInfo.text if progress.errorInfo is not None
                    else _("Unknown error"))
        iface.msg_show(True, err_msg, err_text)
        return 1
    return 0

## Session management

def get_session(vm=None, write=True):
    if vm is None:  # new unlocked session (e.g., for launchVMProcess)
        session = mgr.getSessionObject(vbox)
    elif vm.id in sessions:  # we have one stored session for the vm
        if sessions[vm.id].state == cts.SessionState_Locked:
            session = sessions[vm.id]
        else:  # it was a leftover, delete it and create another session
            close_session(vm)
            session = get_session(vm)
    else:  # there is no session stored for the vm, create a new one
        session = get_session()
        stype = cts.LockType_Write if write else cts.LockType_Shared
        # careful: can throw an exception if not successful
        vm.lockMachine(session, stype)
        # if we get a write session, keep it in self.sessions
        if session.state == cts.SessionState_Locked:
            sessions[vm.id] = session
    return session

def close_session(vm):
    if not vm.id in sessions:
        return
    if sessions[vm.id].state == cts.SessionState_Locked:
        sessions[vm.id].unlockMachine()
    del sessions[vm.id]

## Media

def refresh_media_states():
    for dvd in dvd_images:
        dvd.refreshState()
    for dvd in dvd_host_drives:
        dvd.refreshState()

def register_dvd(path):
    devtype = cts.DeviceType_DVD
    mode = cts.AccessMode_ReadOnly
    try:
        dvd = vbox.openMedium(path, devtype, mode, False)
    except Exception as e:
        iface.msg_show(True, _("Could not register DVD"), str(e))
        return 1
    if not dvd in dvd_images:
        dvd_images.append(dvd)
    return 0

def remove_dvd(dvd_id):
    dvd = util.find(dvd_images, 'id', dvd_id)
    assert dvd is not None
    try:
        dvd.close()
    except Exception as e:
        iface.msg_show(True, _("Could not unregister DVD"), str(e))
        return 1
    dvd_images.remove(dvd)
    return 0

## VM

def watch_vm(vm):
    thread = WatcherThread(vm);
    thread.daemon = True
    thread.start()

def create_vm(vm_name, os_type, hd_size, hd_dynamic):
    try:
        ot = vbox.getGuestOSType(os_type)
        
        if os.system('VBoxManage createvm --name "%s" --register' % vm_name) != 0:
            iface.msg_show(True, _('Cannot create Virtual Machine'), _("Virtual machine named '%s' already exists.") % vm_name)
            return 1
        os.system('VBoxManage modifyvm "%s" --memory %d --vram %d --acpi on --ioapic on --boot1 dvd' % (vm_name, ot.recommendedRAM, ot.recommendedVRAM))
        os.system('VBoxManage modifyvm "%s" --nic1 nat' % vm_name)
        os.system('VBoxManage modifyvm "%s" --ostype "%s"' % (vm_name, os_type))
        os.system('VBoxManage modifyvm "%s" --audiocontroller ac97 --audio pulse --mouse usbtablet' % vm_name)
        
        vm = None
        for vmit in vbox.getMachines():
            if not vmit.accessible:
                continue
            if vmit.name == vm_name:
                vm = vmit
        if vm == None:
            return 0

        """print (cts.HWVirtExPropertyType_Enabled, sysprops.hwvirtex)
        vm = vbox.createMachine('', vm_name, os_type, None, False)
        print 2
        vm.addStorageController('IDE Controller', cts.StorageBus_IDE)
        print 3
        vm.memorySize = ot.recommendedRAM
        vm.VRAMSize = ot.recommendedVRAM
        vm.setHWVirtExProperty(cts.HWVirtExPropertyType_Enabled,
                               sysprops.hwvirtex)
        vbox.registerMachine(vm)"""
        
    except Exception as e:
        iface.msg_show(True, _("Could not create the VM"), str(e))
        return 1
    vms.append(vm)
    vm_name_full = vbox.composeMachineFilename(vm_name, None, None, None)
    vm_dir = os.path.dirname(vm_name_full)
    hd_path = os.path.join(vm_dir, '%s.vdi' % vm_name)
    os.system('VBoxManage createhd --filename "%s" --size %d' % (hd_path, hd_size))
    os.system('VBoxManage storagectl "%s" --name "IDE Controller" --add ide' % vm_name)
    os.system('VBoxManage storageattach "%s" --storagectl "IDE Controller" --port 0 --device 0 --type hdd --medium "%s"' % (vm_name, hd_path))
    os.system('VBoxManage storageattach "%s" --storagectl "IDE Controller" --port 1 --device 0 --type dvddrive --medium emptydrive' % vm_name)
    
    """err_msg = _("Could not create the VM hard disk")
    try:
        #hd = vbox.createHardDisk('', hd_path)
        #mtype = (cts.MediumVariant_Standard if hd_dynamic
        #         else cts.MediumVariant_Fixed)
        print "=====================", 'VBoxManage createhd --filename "%s" --size %d' % (hd_path, hd_size)
        
        #progress = hd.createBaseStorage(hd_size * 1024 * 1024, mtype)
        #print "---------------------", progress
        #progress.waitForCompletion(-1)
    except Exception as e:
        delete_vm(-1, False)
        iface.msg_show(True, err_msg, str(e))
        return 1
    if (check_progress_status(progress, err_msg) or
        attach_device(-1, 'hd', hd.id) or
        attach_device(-1, 'dvd', None)):
        delete_vm(-1, False)
        return 1"""
    return 0

def delete_vm(vm_number, delete_media=True):
    vm = vms[vm_number]
    close_session(vm)
    del vms[vm_number]
    os.system('VBoxManage unregistervm "%s" --delete' % vm.name)

    """err_msg = _("Could not delete the VM")
    try:
        media = vm.unregister(cts.CleanupMode_DetachAllReturnHardDisksOnly)
        mediadel = media if delete_media else []
        progress = vm.delete(mediadel)
        progress.waitForCompletion(-1)
    except Exception as e:
        iface.msg_show(True, err_msg, str(e))
        return 1
    if check_progress_status(progress, err_msg):
        return 1"""
    return 0

def start_vm(vm_number):
    vm = vms[vm_number]
    close_session(vm)
    err_msg = _("Could not start the VM")
    try:
        session = get_session()
        progress = vm.launchVMProcess(session, 'sdl', None)
        progress.waitForCompletion(-1)
    except Exception as e:
        iface.msg_show(True, err_msg, str(e))
        return 1
    if session.state == cts.SessionState_Locked:
        session.unlockMachine()
    else:
        err_text = _("This usually means that the vboxdrv module is not "
                     "loaded. Try again after loading the module.")
        iface.msg_show(True, err_msg, err_text)
        return 1
    if check_progress_status(progress, err_msg):
        return 1
    iface.vm_updated(vm, False)
    watch_vm(vm)
    return 0

def stop_vm(vm_number):
    vm = vms[vm_number]
    try:
        session = get_session(vm, False)
        console = session.console
        console.powerDown()
    except Exception as e:
        iface.msg_show(True, _("Could not stop the VM"), str(e))
        return 1
    return 0

def suspend_vm(vm_number):
    vm = vms[vm_number]
    try:
        session = get_session(vm, False)
        console = session.console
        console.saveState()
    except Exception as e:
        iface.msg_show(True, _("Could not suspend the VM"), str(e))
        return 1
    return 0

def update_settings(vm_number, hwvirtex, ncpus, cpucap, ioapic, chipset,
                    mem, vmem, accel2d, accel3d):
    vm = vms[vm_number]
    try:
        session = get_session(vm)
        svm = session.machine
        svm.setHWVirtExProperty(cts.HWVirtExPropertyType_Enabled, hwvirtex)
        svm.CPUCount = ncpus
        svm.CPUExecutionCap = cpucap
        svm.BIOSSettings.IOAPICEnabled = ioapic
        svm.chipsetType = chipset
        svm.memorySize = mem
        svm.VRAMSize = vmem
        svm.accelerate2DVideoEnabled = accel2d
        svm.accelerate3DEnabled = accel3d
        svm.saveSettings()
    except Exception as e:
        iface.msg_show(True, _("Could not update the VM settings"), str(e))
        return 1
    return 0

def get_medium_info(vm_number, mtype):
    vm = vms[vm_number]
    port = 0 if mtype == 'hd' else 1
    try:
        sc = vm.getStorageControllers()[0].name
        medatt = vm.getMediumAttachment(sc, port, 0)
    except Exception as e:
        iface.msg_show(True, _("Could not get medium attachment info"), str(e))
        return None
    medium = medatt.medium
    passthrough = medatt.passthrough
    medium_id = str(medium.id) if medium else None
    return (medium_id, passthrough)

def attach_device(vm_number, devtype, dev_id, passthrough=False):
    if devtype == 'hd':
        port = 0
        dev_type = cts.DeviceType_HardDisk
        try:
            device = vbox.findMedium(dev_id, dev_type)
        except Exception as e:
            iface.msg_show(True, _("Could not find hd"), str(e))
            return 1
    else:
        port = 1
        dev_type = cts.DeviceType_DVD
        device = util.find(dvd_host_drives, 'id', dev_id)
    vm = vms[vm_number]
    try:
        session = get_session(vm)
        svm = session.machine
        sc = svm.getStorageControllers()[0].name
    except Exception as e:
        iface.msg_show(True, _("Could not get the VM storage controller"),
                       str(e))
        return 1
    try:
        svm.detachDevice(sc, port, 0)
    except:  # no device was connected
        pass
    try:
        svm.attachDevice(sc, port, 0, dev_type, device)
        if devtype == 'dvd':
            svm.passthroughDevice(sc, port, 0, passthrough)
        svm.saveSettings()
    except Exception as e:
        iface.msg_show(True, _("Could not attach device"), str(e))
        return 1
    return 0

def mount_dvd(vm_number, dvd_id):
    vm = vms[vm_number]
    try:
        os.system('VBoxManage modifyvm "%s" --dvd "%s"' % (vm.name, dvd_id))
        # dvd = vbox.findMedium(dvd_id, cts.DeviceType_DVD) if dvd_id else None
    except Exception as e:
        iface.msg_show(True, _("Could not find dvd"), str(e))
        return 1
    """try:
        session = get_session(vm, False)
        svm = session.machine
        sc = svm.getStorageControllers()[0].name
        svm.mountMedium(sc, 1, 0, dvd, False)
        svm.saveSettings()
    except Exception as e:
        iface.msg_show(True, _("Could not mount DVD"), str(e))
        return 1"""
    return 0

def update_devices(vm_number, attachment, device, intnet, hostif,
                   mac, cable, audiodev, audiocont):
    vm = vms[vm_number]
    try:
        session = get_session(vm)
        svm = session.machine
        net = svm.getNetworkAdapter(0)
        net.adapterType = device
        net.internalNetwork = intnet
        net.bridgedInterface = hostif
        net.MACAddress = mac
        net.cableConnected = cable
        net.attachmentType = attachment
        svm.audioAdapter.audioDriver = audiodev
        svm.audioAdapter.audioController = audiocont
        svm.saveSettings()
    except Exception as e:
        iface.msg_show(True, _("Could not update the VM devices"), str(e))
        return 1
    return 0

def create_shared(vm_number, name, folder, writable, automount):
    vm = vms[vm_number]
    try:
        session = get_session(vm, False)
        svm = session.machine
        svm.createSharedFolder(name, folder, writable, automount)
        svm.saveSettings()
    except Exception as e:
        iface.msg_show(True, _("Could not create shared folder"), str(e))
        return 1
    return 0

def remove_shared(vm_number, shared_name):
    vm = vms[vm_number]
    try:
        session = get_session(vm, False)
        svm = session.machine
        svm.removeSharedFolder(shared_name)
        svm.saveSettings()
    except Exception as e:
        iface.msg_show(True, _("Could not remove shared folder"), str(e))
        return 1
    return 0

def get_snapshot_tree(vm_number):
    vm = vms[vm_number]
    try:
        root = vm.findSnapshot(None)
    except Exception as e:
        return []
    tree = [(root, None)]
    def traverse_tree(root):
        for child in root.getChildren():
            tree.append((child, root.id))
            traverse_tree(child)
    traverse_tree(root)
    return tree

def edit_snapshot(vm_number, uuid, name):
    vm = vms[vm_number]
    try:
        snapshot = vm.findSnapshot(uuid)
        snapshot.name = name
    except Exception as e:
        iface.msg_show(True, _("Could not edit the snapshot name"), str(e))
        return 1
    return 0

def take_snapshot(vm_number, name, description = ''):
    vm = vms[vm_number]
    err_msg = _("Could not take snapshot")
    try:
        session = get_session(vm, False)
        console = session.console
        progress = console.takeSnapshot(name, description)
        progress.waitForCompletion(-1)
    except Exception as e:
        iface.msg_show(True, err_msg, str(e))
        return 1
    if check_progress_status(progress, err_msg):
        return 1
    return 0

def delete_snapshot(vm_number, uuid):
    vm = vms[vm_number]
    err_msg = _("Could not delete snapshot")
    try:
        session = get_session(vm, False)
        console = session.console
        progress = console.deleteSnapshot(uuid)
        progress.waitForCompletion(-1)
    except Exception as e:
        iface.msg_show(True, err_msg, str(e))
        return 1
    if check_progress_status(progress, err_msg):
        return 1
    return 0

def restore_snapshot(vm_number, uuid):
    vm = vms[vm_number]
    err_msg = _("Could not restore snapshot")
    try:
        session = get_session(vm, False)
        console = session.console
        progress = console.restoreSnapshot(vm.findSnapshot(uuid))
        progress.waitForCompletion(-1)
    except Exception as e:
        iface.msg_show(True, err_msg, str(e))
        return 1
    if check_progress_status(progress, err_msg):
        return 1
    return 0

