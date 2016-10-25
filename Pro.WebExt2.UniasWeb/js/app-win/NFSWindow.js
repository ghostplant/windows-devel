NFSManage = Ext.extend(AppWin.AbstractWindow, {
	id : 'nfs-win',
	label : 'NFS Config',
	bigIcon : 'supertux.png',

	prepareItems : function() {
		var storageLocation = MyDesktop.gatewayRequestObjectWithGetSync("/storage/location");
		if (storageLocation.status!='ok') {
			MyDesktop.messageBox('Error: '+storageLocation.reason);
			return null;
		}
		return { html:'<iframe id="frame" name="frame" src="'+storageLocation.location+'" width=100% height=100%/>' }; //window.open
	}
});

AppWin.GenericWindows.push({
	visible: [false, true],
	window: NFSManage
});

