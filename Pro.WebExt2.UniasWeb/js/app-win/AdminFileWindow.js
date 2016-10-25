AdminFileManage = Ext.extend(FileManage, {
	id : 'admin-file-win',

	downloadFileURI: "/admin/file/download",
	deleteFileURI: "/admin/file/delete",
	listFileURI: "/admin/file/list_files",
	
	getUserFiles : function(listFiles) {
		return listFiles.result;
	}
});

AppWin.GenericWindows.push({
	visible: [false, true],
	window: AdminFileManage
});

