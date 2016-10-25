#!/usr/bin/python2

import os
import datetime
from gi.repository import Gtk
from gi.repository import Gdk
from gi.repository import GObject

import dao
import paths
import util


MB = 1024 * 1024


def modmethod(func):
    class modproxy(object):
        __getattribute__ = func.func_globals.__getitem__
        __setattr__ = func.func_globals.__setitem__
    self = modproxy()
    def call_with_method(*args,**kwargs):
        return func(self,*args,**kwargs)
    call_with_method.func_name = func.func_name
    return call_with_method

############## Initialization functions ##############

@modmethod
def main(self):
    dao.init()
    self.selected_vm = None
    self.vm_states = {dao.cts.MachineState_Null: _("Null"),
                      dao.cts.MachineState_PoweredOff: _("Powered Off"),
                      dao.cts.MachineState_Saved: _("Saved"),
                      dao.cts.MachineState_Aborted: _("Aborted"),
                      dao.cts.MachineState_Running: _("Running"),
                      dao.cts.MachineState_Paused: _("Paused"),
                      dao.cts.MachineState_Stuck: _("Stuck"),
                      dao.cts.MachineState_Starting: _("Starting"),
                      dao.cts.MachineState_Stopping: _("Stopping"),
                      dao.cts.MachineState_Saving: _("Saving"),
                      dao.cts.MachineState_Restoring: _("Restoring"),
                      dao.cts.MachineState_SettingUp: _("Setting Up") }
    self.builder = Gtk.Builder()
    xml_files = ['xml/vboxgtk-actions.xml',
                 'xml/vboxgtk.xml',
                 'xml/vboxgtk-menutoolbar.xml']
    for f in xml_files:
        builder.add_from_file(os.path.join(paths.data_path, f))
    menubar = self.bgo('menubar')
    toolbar = self.bgo('toolbar')
    box_main = self.bgo('box_main')
    box_main.pack_start(menubar, False, False, 0)
    box_main.pack_start(toolbar, False, False, 0)
    init_signals()
    init_widgets()
    init_wstates()
    init_icons()
    self.must_apply_media = False
    self.must_apply_settings = False
    self.must_apply_devices = False
    GObject.threads_init()
    Gtk.main()

def init_signals():
    dic = {'on_vms_selchanged': on_vms_selchanged,
           'on_vms_rowactivated': on_vms_rowactivated,
           'on_newvm_activate': on_newvm_activate,
           'on_ostype_changed': on_ostype_changed,
           'on_vmname_changed': on_vmname_changed,
           'on_delvm_activate': on_delvm_activate,
           'on_runvm_activate': on_runvm_activate,
           'on_sleepvm_activate': on_sleepvm_activate,
           'on_stopvm_activate': on_stopvm_activate,
           'on_hwvirtex_toggled': on_hwvirtex_toggled,
           'on_ioapic_toggled': on_ioapic_toggled,
           'on_settings_changed': on_settings_changed,
           'on_dvd_changed': on_dvd_changed,
           'on_adddvd_clicked': on_adddvd_clicked,
           'on_media_changed': on_media_changed,
           'on_nicatt_changed': on_nicatt_changed,
           'on_generatemac_clicked': on_generatemac_clicked,
           'on_devices_changed': on_devices_changed,
           'on_sharedname_changed': on_sharedname_changed,
           'on_shared_selchanged': on_shared_selchanged,
           'on_newshared_clicked': on_newshared_clicked,
           'on_rmshared_clicked': on_rmshared_clicked,
           'on_snaps_selchanged': on_snaps_selchanged,
           'on_snapname_edited': on_snapname_edited,
           'on_takesnap_clicked': on_takesnap_clicked,
           'on_delsnap_clicked': on_delsnap_clicked,
           'on_restoresnap_clicked': on_restoresnap_clicked,
           'on_about_activate': on_about_activate,
           'on_main_destroy': on_main_destroy}
    builder.connect_signals(dic)

def init_widgets():
    vm_list = bgo('lstore_vms')
    for vm_elem in dao.vms:
        try:
            vmstate = vm_states[vm_elem.state]
        except KeyError:
            vmstate = _("Unknown")
        vm_list.append([vm_elem.name, vmstate])
    os_types = bgo('lstore_ostypes')
    for os_type_elem in dao.os_types:
        os_types.append([os_type_elem.description, os_type_elem.id])

    bgo('ckbutton_hwvirtex').set_sensitive(dao.sysprops.hwvirtex)
    bgo('scale_ncpus').set_range(dao.sysprops.min_ncpus, dao.sysprops.max_ncpus)
    bgo('scale_ram').set_range(dao.sysprops.min_ram, dao.sysprops.max_ram)
    bgo('scale_vram').set_range(dao.sysprops.min_vram, dao.sysprops.max_vram)
    bgo('ckbutton_accel3d').set_sensitive(dao.sysprops.gpuaccel)
    def fill_model(model, list):
        for e in list:
            model.append((e[0], e[1]))
    chipsets = bgo('lstore_chipsets')
    data = (('PIIX3', dao.cts.ChipsetType_PIIX3),
            ('ICH9', dao.cts.ChipsetType_ICH9))
    fill_model(chipsets, data)
    is_separator = lambda model, iter, data: model.get_value(iter, 0) == -1
    bgo('combo_dvd').set_row_separator_func(is_separator, None)
    fill_media_list('dvd')
    nic_atts = bgo('lstore_nicatts')
    data = ((_("Null"), dao.cts.NetworkAttachmentType_Null),
            (_("NAT"), dao.cts.NetworkAttachmentType_NAT),
            (_("Bridged"), dao.cts.NetworkAttachmentType_Bridged),
            (_("Internal network"), dao.cts.NetworkAttachmentType_Internal))
    fill_model(nic_atts, data)
    nic_devs = bgo('lstore_nicdevs')
    data = (('Am79C970A', dao.cts.NetworkAdapterType_Am79C970A),
            ('Am79C973', dao.cts.NetworkAdapterType_Am79C973),
            ('82540EM', dao.cts.NetworkAdapterType_I82540EM),
            ('82543GC', dao.cts.NetworkAdapterType_I82543GC),
            ('82545EM', dao.cts.NetworkAdapterType_I82545EM))
    fill_model(nic_devs, data)
    audiobackends = bgo('lstore_audiobackends')
    data = ((_("Null"), dao.cts.AudioDriverType_Null),
            ('OSS', dao.cts.AudioDriverType_OSS),
            ('ALSA', dao.cts.AudioDriverType_ALSA),
            ('PulseAudio', dao.cts.AudioDriverType_Pulse))
    fill_model(audiobackends, data)
    audiodevs = bgo('lstore_audiodevs')
    data = (('AC97', dao.cts.AudioControllerType_AC97),
            ('SB16', dao.cts.AudioControllerType_SB16),
            ('HDA', dao.cts.AudioControllerType_HDA))
    fill_model(audiodevs, data)
    bgo('notebook_vm').set_sensitive(True)
    bgo('notebook_vm').set_sensitive(False)

@modmethod
def init_wstates(self):
    """Widgets that *ONLY* depend on the VM state, not on other widgets"""
    self.wstates = []
    non_sleep_widgets = ['frame_media', 'buttonbox_shared']
    non_running_widgets = ['action_runvm']
    running_widgets = ['action_sleepvm', 'action_stopvm']
    stopped_widgets = ['action_delvm', 'frame_cpu', 'frame_mobo',
                       'frame_memory', 'frame_graphics', 'frame_network',
                       'frame_audio']

    for w in non_sleep_widgets:
        wstates.append((w, [dao.cts.MachineState_PoweredOff,
                            dao.cts.MachineState_Aborted,
                            dao.cts.MachineState_Running]))
    for w in non_running_widgets:
        wstates.append((w, [dao.cts.MachineState_PoweredOff,
                            dao.cts.MachineState_Saved,
                            dao.cts.MachineState_Aborted]))
    for w in stopped_widgets:
        wstates.append((w, [dao.cts.MachineState_PoweredOff,
                            dao.cts.MachineState_Aborted]))
    for w in running_widgets:
        wstates.append((w, [dao.cts.MachineState_Running]))

def init_icons():
    Gtk.IconTheme.get_default().append_search_path(paths.icons_path)
    Gtk.Window.set_default_icon_name('vboxgtk')
    bgo('window_main').set_icon_name('vboxgtk')
    bgo('dialog_about').set_logo_icon_name('vboxgtk')

############## VM ##############

def apply_pending_changes():
    if must_apply_settings:
        apply_pending_settings()
    if must_apply_media:
        apply_pending_media()
    if must_apply_devices:
        apply_pending_devices()

def fill_vm_everything():
    fill_vm_settings()
    fill_vm_media()
    fill_vm_devices()
    fill_vm_shared()
    fill_vmsnaps()

@modmethod
def on_vms_selchanged(self, obj):
    apply_pending_changes()
    vm_cursor = obj.get_cursor()[0]
    if vm_cursor is None:
        for w in wstates:
            bgo(w[0]).set_sensitive(False)
        bgo('notebook_vm').set_sensitive(False)
        self.selected_vm = None
        return
    self.selected_vm = int(str(vm_cursor))
    bgo('notebook_vm').set_sensitive(True)
    for w in wstates:
        bgo(w[0]).set_sensitive(dao.vms[selected_vm].state in w[1])
    fill_vm_everything()

def on_vms_rowactivated(treev, path, view_column):
    bgo('action_runvm').activate()

def on_newvm_activate(obj):
    bgo('combo_ostype').set_active(0)
    bgo('entry_vmname').set_text('')
    bgo('scale_hdsize').set_value(8 * 1024)
    bgo('button_addvm').set_sensitive(False)
    dialog = bgo('dialog_newvm')
    finished = False
    while not finished:
        if dialog.run() != 0:
            dialog.hide()
            return
        os_type = comboget('combo_ostype', 1)
        vm_name = bgo('entry_vmname').get_text()
        hd_size = bgo('scale_hdsize').get_value()
        hd_dynamic = bgo('ckbutton_hddynamic').get_active()
        if dao.create_vm(vm_name, os_type, hd_size, hd_dynamic):
            continue
        finished = True
    dialog.hide()
    state = vm_states[dao.vms[-1].state]
    bgo('lstore_vms').append([vm_name, state])

def on_ostype_changed(obj):
    os_idx = obj.get_active()
    os_type = dao.os_types[os_idx]
    #bgo('entry_vmname').set_text(os_type.id)
    bgo('scale_hdsize').set_value(os_type.recommendedHDD / MB)

def on_vmname_changed(obj):
    vm_name = bgo('entry_vmname').get_text()
    bgo('button_addvm').set_sensitive(vm_name)

@modmethod
def on_delvm_activate(self, obj):
    msg1 = _("Are you sure you want to delete the virtual "
             "machine '%s'?") % (dao.vms[selected_vm].name,)
    msg2 = _("The machine settings and its hard disks data will be lost.")
    if msg_confirm(msg1, msg2) == Gtk.ResponseType.CANCEL:
        return
    dao.delete_vm(selected_vm)
    vm_list = bgo('lstore_vms')
    vm_list.remove(vm_list.get_iter(selected_vm))
    self.must_apply_settings = False
    self.must_apply_media = False
    self.must_apply_devices = False
    self.selected_vm = -1
    on_vms_selchanged(bgo('treev_vms'))

def on_runvm_activate(obj):
    apply_pending_changes()
    dao.start_vm(selected_vm)

def on_sleepvm_activate(obj):
    dao.suspend_vm(selected_vm)

def on_stopvm_activate(obj):
    dao.stop_vm(selected_vm)

############## Media ##############

@modmethod
def fill_media_list(self, media_type):
    dao.refresh_media_states()
    self.filling_media = True
    model = bgo('lstore_%s' % (media_type,))
    model.clear()
    if media_type == 'dvd':
        model.append([0, '', _("(none)")])
        images = dao.dvd_images
        hostdrives = dao.dvd_host_drives
    if images:
        model.append([-1, '', ""])
        for image in images:
            if image.state == dao.cts.MediumState_Inaccessible:
                name = '(!) %s' % (image.name,)
            else:
                name = image.name
            model.append([1, image.id, name])
    if hostdrives:
        model.append([-1, '', ''])
        for hostdrive in hostdrives:
            model.append([2, hostdrive.id, hostdrive.location])
    self.filling_media = False
    fill_vm_media()  # we must reselect the value in the combobox(es)

@modmethod
def fill_vm_media(self):
    if selected_vm is None:
        return
    self.filling_media = True
    info = dao.get_medium_info(selected_vm, 'dvd')
    if info is None:
        return
    dvd_id = info[0]
    pt = info[1]
    comboset('combo_dvd', 1, dvd_id)
    update_dvdpt_state()
    bgo('ckbutton_dvdpt').set_active(pt)
    self.filling_media = False

@modmethod
def apply_pending_media(self):
    is_hostdvd = (comboget('combo_dvd', 0) == 2)
    dvd_id = comboget('combo_dvd', 1)
    if is_hostdvd:
        dvdpt = bgo('ckbutton_dvdpt').get_active()
        dao.attach_device(selected_vm, 'dvd', dvd_id, dvdpt)
    else:
        dao.mount_dvd(selected_vm, dvd_id)
    self.must_apply_media = False

@modmethod
def on_media_changed(self, obj):
    if filling_media:
        return
    if dao.vms[selected_vm].state == dao.cts.MachineState_Running:
        apply_pending_media()
    else:
        self.must_apply_media = True

def update_dvdpt_state():
    combo = bgo('combo_dvd')
    sens = (combo.get_state() != Gtk.StateType.INSENSITIVE and
            comboget('combo_dvd', 0) == 2)
    bgo('ckbutton_dvdpt').set_sensitive(sens)

def on_dvd_changed(obj):
    update_dvdpt_state()

def on_adddvd_clicked(obj):
    buttons = (Gtk.STOCK_CANCEL, Gtk.ResponseType.CANCEL,
               Gtk.STOCK_OPEN, Gtk.ResponseType.OK)
    dialog = Gtk.FileChooserDialog(_("Open..."), None,
                                   Gtk.FileChooserAction.OPEN, buttons)
    dialog.set_default_response(Gtk.ResponseType.OK)
    if obj is bgo('button_adddvd'):
        media_type = 'dvd'
    if dialog.run() != Gtk.ResponseType.OK:
        dialog.destroy()
        return
    filename = dialog.get_filename()
    dialog.destroy()
    if dao.register_dvd(filename):
        return
    fill_media_list(media_type)

############## VM settings ##############

@modmethod
def fill_vm_settings(self):
    if selected_vm is None:
        return
    vm = dao.vms[selected_vm]
    self.filling_settings = True
    hwvirtex = vm.getHWVirtExProperty(dao.cts.HWVirtExPropertyType_Enabled)
    ncpus = vm.CPUCount
    cpucap = vm.CPUExecutionCap
    ioapic = vm.BIOSSettings.IOAPICEnabled
    chipset = vm.chipsetType
    accel2d = vm.accelerate2DVideoEnabled
    accel3d = vm.accelerate3DEnabled
    ram = vm.memorySize
    vram = vm.VRAMSize
    bgo('ckbutton_hwvirtex').set_active(hwvirtex and dao.sysprops.hwvirtex)
    bgo('scale_ncpus').set_value(ncpus)
    bgo('scale_cpucap').set_value(cpucap)
    bgo('ckbutton_ioapic').set_active(ioapic)
    comboset('combo_chipset', 1, chipset)
    bgo('scale_ram').set_value(ram)
    bgo('scale_vram').set_value(vram)
    bgo('ckbutton_accel2d').set_active(accel2d)
    bgo('ckbutton_accel3d').set_active(accel3d and dao.sysprops.gpuaccel)
    update_ncpus_state()
    update_chipset_state()
    self.filling_settings = False

@modmethod
def apply_pending_settings(self):
    hwvirtex = bgo('ckbutton_hwvirtex').get_active()
    ncpus = bgo('scale_ncpus').get_value()
    cpucap = bgo('scale_cpucap').get_value()
    chipset = comboget('combo_chipset', 1)
    ioapic = bgo('ckbutton_ioapic').get_active()
    ram = bgo('scale_ram').get_value()
    vram = bgo('scale_vram').get_value()
    accel2d = bgo('ckbutton_accel2d').get_active()
    accel3d = bgo('ckbutton_accel3d').get_active()
    if not dao.update_settings(selected_vm, hwvirtex, ncpus, cpucap, ioapic,
                               chipset, ram, vram, accel2d, accel3d):
        self.must_apply_settings = False

@modmethod
def on_settings_changed(self, obj):
    if filling_settings:
        return
    if dao.vms[selected_vm].state == dao.cts.MachineState_Running:
        apply_pending_settings()
    else:
        self.must_apply_settings = True

def on_ioapic_toggled(obj):
    update_chipset_state()

def on_hwvirtex_toggled(obj):
    update_ncpus_state()

def update_ncpus_state():
    toggled = bgo('ckbutton_hwvirtex').get_active()
    if not toggled:
        bgo('scale_ncpus').set_value(1)
    bgo('box_ncpus').set_sensitive(toggled)

def update_chipset_state():
    toggled = bgo('ckbutton_ioapic').get_active()
    if not toggled:
        comboset('combo_chipset', 1, dao.cts.ChipsetType_PIIX3)
    bgo('box_chipset').set_sensitive(toggled)

############## VM Devices ##############

@modmethod
def fill_vm_devices(self):
    if selected_vm is None:
        return
    vm = dao.vms[selected_vm]
    self.filling_devices = True
    net = vm.getNetworkAdapter(0)
    comboset('combo_nicatt', 1, net.attachmentType)
    comboset('combo_nicdev', 1, net.adapterType)
    if net.bridgedInterface is not None:
        bgo('entry_hostif').set_text(net.bridgedInterface)
    if net.internalNetwork is not None:
        bgo('entry_intnet').set_text(net.internalNetwork)
    bgo('entry_mac').set_text(net.MACAddress)
    bgo('ckbutton_cable').set_active(net.cableConnected)
    update_nic_config_state()
    audiobackend = vm.audioAdapter.audioDriver
    audiodev = vm.audioAdapter.audioController
    comboset('combo_audiobackend', 1, audiobackend)
    comboset('combo_audiodev', 1, audiodev)
    self.filling_devices = False

@modmethod
def apply_pending_devices(self):
    net_att = comboget('combo_nicatt', 1)
    net_dev = comboget('combo_nicdev', 1)
    hostif = bgo('entry_hostif').get_text()
    intnet = bgo('entry_intnet').get_text()
    if not intnet:
        intnet = 'intnet'
        bgo('entry_intnet').set_text(intnet)
    mac = bgo('entry_mac').get_text()
    cable = bgo('ckbutton_cable').get_active()
    audiobackend = comboget('combo_audiobackend', 1)
    audiodev = comboget('combo_audiodev', 1)
    if not dao.update_devices(selected_vm, net_att, net_dev, intnet,
                              hostif, mac, cable, audiobackend, audiodev):
        self.must_apply_devices = False

@modmethod
def on_devices_changed(self, obj):
    if filling_devices:
        return
    if dao.vms[selected_vm].state == dao.cts.MachineState_Running:
        apply_pending_devices()
    else:
        self.must_apply_devices = True

def on_generatemac_clicked(obj):
    bgo('entry_mac').set_text(util.random_mac())

def on_nicatt_changed(obj):
    update_nic_config_state()

def update_nic_config_state():
    combo_atts = bgo('combo_nicatt')
    nic_att_sensitive = (combo_atts.get_state() != Gtk.StateType.INSENSITIVE)
    nic_att = comboget('combo_nicatt', 1)
    net_widg = ('label_nicdev', 'combo_nicdev', 'label_mac', 'box_mac',
                'ckbutton_cable')
    sens = (nic_att != dao.cts.NetworkAttachmentType_Null and
            nic_att_sensitive)
    for w in net_widg:
        bgo(w).set_sensitive(sens)
    sens = (nic_att == dao.cts.NetworkAttachmentType_Bridged and
            nic_att_sensitive)
    bgo('label_hostif').set_sensitive(sens)
    bgo('entry_hostif').set_sensitive(sens)
    sens = (nic_att == dao.cts.NetworkAttachmentType_Internal and
            nic_att_sensitive)
    bgo('label_intnet').set_sensitive(sens)
    bgo('entry_intnet').set_sensitive(sens)

############## VM Shared folders ##############

def fill_vm_shared():
    if selected_vm is None:
        return
    model_shared = bgo('lstore_shared')
    model_shared.clear()
    shared_list = dao.vms[selected_vm].getSharedFolders()
    for shared in shared_list:
        model_shared.append([shared.name, util.shortname(shared.hostPath),
                             shared.writable])
    update_shared_remove_state()

def on_newshared_clicked(obj):
    dialog = bgo('dialog_addshared')
    finished = False
    while not finished:
        if dialog.run() != 0:
            dialog.hide()
            return
        name = bgo('entry_sharedname').get_text();
        path = bgo('filechooserbutton_sharedpath').get_filename();
        writable = bgo('ckbutton_sharedwritable').get_active();
        if dao.create_shared(selected_vm, name, path, writable, True):
            continue
        finished = True
    model = bgo('treev_shared').get_model()
    model.append([name, util.shortname(path), writable])
    dialog.hide()

def on_sharedname_changed(obj):
    shared_name = obj.get_text()
    button_add = bgo('button_addshared')
    button_add.set_sensitive(shared_name)

def on_rmshared_clicked(obj):
    treev = bgo('treev_shared')
    model = treev.get_model()
    selected_shared = int(str(treev.get_cursor()[0]))
    shared_name = model[selected_shared][0]
    if dao.remove_shared(selected_vm, shared_name):
        return
    model.remove(model.get_iter(selected_shared))
    update_shared_remove_state()

def on_shared_selchanged(obj):
    update_shared_remove_state()

def update_shared_remove_state():
    treev = bgo('treev_shared')
    model = treev.get_model()
    selected_shared_cursor = treev.get_cursor()[0]
    sensitive = (selected_shared_cursor is not None and
                 bgo('buttonbox_shared').get_sensitive())
    bgo('button_rmshared').set_sensitive(sensitive)

############## VM Snapshots ##############

def fill_vmsnaps():
    if selected_vm is None:
        return
    treev_snaps = bgo('treev_snaps')
    model_snaps = treev_snaps.get_model()
    model_snaps.clear()
    snapshot_list = dao.get_snapshot_tree(selected_vm)
    def fill_snapshot_list(node, pos):
        snapshot = snapshot_list[pos][0]
        iter = model_snaps.append(node, [snapshot.name, str(snapshot.id)])
        pos = pos + 1
        while (pos != len(snapshot_list) and
               snapshot_list[pos][1] == snapshot.id):
            fill_snapshot_list(iter, pos)
            pos = pos + 1
    if snapshot_list != []:
        fill_snapshot_list(None, 0)
    treev_snaps.expand_all()
    update_snap_buttons_state()

def on_snapname_edited(cell, path, new_name):
    if new_name:
        return
    model = bgo('tstore_snaps')
    treeiter = model.get_iter(path)
    snapshot_id = model.get_value(treeiter, 1)
    if dao.edit_snapshot(selected_vm, snapshot_id, new_name):
        return
    model[path][0] = new_name

def on_takesnap_clicked(obj):
    model = bgo('tstore_snaps')
    new_name = 'Snapshot %s' % (datetime.datetime.now().ctime(),)
    if dao.take_snapshot(selected_vm, new_name):
        return
    fill_vmsnaps()
    update_snap_buttons_state()

def on_delsnap_clicked(obj):
    tree = bgo('treev_snaps')
    model = tree.get_model()
    treeiter = model.get_iter(tree.get_cursor()[0])
    snapshot_id = model.get_value(treeiter, 1)
    if dao.delete_snapshot(selected_vm, snapshot_id):
        return
    fill_vmsnaps()
    update_snap_buttons_state()

def on_restoresnap_clicked(obj):
    tree = bgo('treev_snaps')
    model = tree.get_model()
    treeiter = model.get_iter(tree.get_cursor()[0])
    snapshot_id = model.get_value(treeiter, 1)
    if dao.restore_snapshot(selected_vm, snapshot_id):
        return
    update_snap_buttons_state()
    fill_vm_everything()

def on_snaps_selchanged(obj):
    update_snap_buttons_state()

def update_snap_buttons_state():
    cur = bgo('treev_snaps').get_cursor()[0]
    s = cur is not None
    bgo('button_delsnap').set_sensitive(s)
    s = s and dao.vms[selected_vm].state != dao.cts.MachineState_Running
    bgo('button_restoresnap').set_sensitive(s)

############## Auxiliary functions ##############

def bgo(name):
    return builder.get_object(name)

def comboget(combo_name, column):
    combo = bgo(combo_name)
    model = combo.get_model()
    row = combo.get_active()
    if row == -1:
        return None
    return model[row][column]

def comboset(combo_name, column, value):
    combo = bgo(combo_name)
    model = combo.get_model()
    for i, row in enumerate(model):
        if (value is None and not row[column]) or row[column] == value:
            combo.set_active(i)
            break

def vm_updated(vm, from_thread=False):
    if from_thread:
        Gdk.threads_enter()
    model_vm_list = bgo('treev_vms').get_model()
    try:
        status = vm_states[vm.state];
    except KeyError:
        status = _("Unknown")
    model_vm_list[dao.vms.index(vm)][1] = status
    if dao.vms[selected_vm] == vm:
        for w in wstates:
            bgo(w[0]).set_sensitive(vm.state in w[1])
        update_dvdpt_state()
        update_nic_config_state()
        update_shared_remove_state()
        update_snap_buttons_state()
    if from_thread:
        Gdk.threads_leave()

def msg_show(is_error, msg1, msg2='', from_thread=False):
    if from_thread:
        Gdk.threads_enter()
    if is_error:
        msg_type = Gtk.MessageType.ERROR
    else:
        msg_type = Gtk.MessageType.INFO
    dialog = Gtk.MessageDialog(None, Gtk.DialogFlags.MODAL, msg_type,
                               Gtk.ButtonsType.CLOSE, msg1)
    dialog.format_secondary_text(msg2)
    dialog.run()
    dialog.destroy()
    if from_thread:
        Gdk.threads_leave()

def msg_confirm(msg1, msg2):
    dialog = Gtk.MessageDialog(None, Gtk.DialogFlags.MODAL,
                                     Gtk.MessageType.WARNING,
                                     Gtk.ButtonsType.NONE, msg1)
    dialog.add_buttons(Gtk.STOCK_CANCEL, Gtk.ResponseType.CANCEL,
                       Gtk.STOCK_DELETE, Gtk.ResponseType.OK)
    dialog.format_secondary_text(msg2)
    decision = dialog.run()
    dialog.destroy()
    return decision

############## About ##############

def on_about_activate(obj):
    about = bgo('dialog_about')
    about.run()
    about.hide()

############## Exit ##############

def on_main_destroy(obj):
    apply_pending_changes()
    needs_stopping = dao.exit()
    if needs_stopping:
        for i, vm in enumerate(dao.vms):
            if vm.state == dao.cts.MachineState_Running:
                dao.stop_vm(i)
    Gtk.main_quit()
