Ext.getExtType = function(fileName) {
	var at = fileName.lastIndexOf('.') + 1;
	if (at > 0) {
		var extName = '|'+fileName.substring(at);
		if (Ext.getExtType.textFileExt.indexOf(extName)>=0)
			return 'editable';
		else if (Ext.getExtType.execFileExt.indexOf(extName)>=0)
			return 'executable';
		else if (Ext.getExtType.packFileExt.indexOf(extName)>=0)
			return 'package';
	}
	return 'unknown';
};

Ext.getExtType.textFileExt = "|txt|in|out|ini|sh|c|cpp|erl|java|log|xcf";
Ext.getExtType.execFileExt = "|xar";
Ext.getExtType.packFileExt = "|zip|rar|7z|gz|lzma|xz|cab|iso|jar|war";


Ext.getFilePathAndOwner = function(node) {
	var pathName = node.text;
	while (node.parentNode != null) {
		node = node.parentNode;
		if (node.getDepth()<=1)
			break;
		pathName = node.text + '/' + pathName;
	}
	return {
		filePathName: pathName,
		fileOwner: node.text
	};
};

FileManage = Ext.extend(AppWin.AbstractWindow, {
	id : 'file-win',
	label : 'File Browser',
	bigIcon : 'unetbootin.png',
	
	downloadFileURI: "/file/download",
	deleteFileURI: "/file/delete",
	listFileURI: "/file/list_files",
	
	fileTreeList: [],
	
	getUserFiles : function(listFiles) {
		return [{
			owner: MyDesktop.getGatewayUsername(),
			status: listFiles.status,
			files: listFiles.files
		}];
	},

	refresh : function(window) {
		var listFiles = MyDesktop.gatewayRequestObjectWithGetSync(window.listFileURI, "filePath=/");
		if (listFiles.status != 'ok')
			return MyDesktop.messageBox("Error: "+listFiles.reason);

		var userFiles = window.getUserFiles(listFiles);
		window.fileTreeList.splice(0, window.fileTreeList.length);
		for (var i=0;i!=userFiles.length;i++) {
			var one = userFiles[i], ownerFileList=[];
			if (one.status=='ok') {
				for (var j=0;j!=one.files.length;j++) {
					var iconType = 'folder', notFolder = (one.files[j].fileType != 'folder');
					if (notFolder)
						iconType = Ext.getExtType(one.files[j].fileName);
					ownerFileList.push({
						text: one.files[j].fileName,
						iconCls: iconType,
						leaf: notFolder,
						fileSize: one.files[j].fileSize,
						loaded: false,
						children: []
					});
				}
			}
			window.fileTreeList.push({
				text: one.owner,
				expanded: true,
				iconCls: 'user-kid',
				children: ownerFileList
			});
		}

		var tree = Ext.getCmp('im-tree');
		tree.body.mask('Loading', 'x-mask-loading');
		tree.root.reload();
		tree.root.collapse(true, false);
		setTimeout(function(){
			tree.body.unmask();
			tree.root.expand(true, true);
		}, 500);
	},

	prepareItems : function() {
		var thisWindow = this;
		var userName = MyDesktop.getGatewayUsername();
		
		var downloadFile = function(isTextFile) {
			var tree = Ext.getCmp('im-tree');
			var selectedNode = tree.getSelectionModel().getSelectedNode();
			if (selectedNode==null || selectedNode.getDepth()<=1 )
				return MyDesktop.messageBox("You are supposed to select a file!");
			
			var fileDatas = Ext.getFilePathAndOwner(selectedNode);
			var result = MyDesktop.gatewayRequestObjectWithGetSync(thisWindow.downloadFileURI, "fileName="+fileDatas.filePathName+"&owner="+fileDatas.fileOwner);
			if (result.status!='ok')
				MyDesktop.messageBox("Download operation encountered error!<br/>Reason:"+result.reason);
			else if (isTextFile!=true)
				window.open(result.serverpath);
			else {
				var conn = Ext.lib.Ajax.getConnectionObject().conn;
				conn.open("GET", result.serverpath, false);
				try { conn.send(null); } catch (e) {};
				return conn.responseText;
			}
			return '';
		};

		return new Ext.Panel({
			layout: 'border',
			items: [ {
				region: 'east',
				split:true,
				collapsible:true,
				width: '45%',
				items: [{
					title: 'File Size',
					collapsible:true,
					html: '<p id="file-win-filesize-field"></p>'
				}, {
					title: 'File Content',
					collapsible:true,
					items: new Ext.form.TextArea({
						id: 'file-win-textbox',
						height: '70%',
						width: '100%',
						readOnly: true,
					})
				}]
			}, new Ext.tree.TreePanel({
						id:'im-tree',
						region:'center',
						title: 'File List',
						loader: new Ext.tree.TreeLoader(),
						rootVisible:false,
						lines:false,
						autoScroll:true,
						tools:[{
							id:'refresh',
							on:{
								click: function() {
									thisWindow.refresh(thisWindow);
								}
							}
						}],
						root: new Ext.tree.AsyncTreeNode({
							children: thisWindow.fileTreeList
						}),
						listeners: {
							'dblclick': function(node,e) {
								if (node.getDepth()>1) {
									if (node.attributes.iconCls == 'folder') {
										if (node.attributes.loaded)
											return;
										var param = Ext.getFilePathAndOwner(node);
										var one = MyDesktop.gatewayRequestObjectWithGetSync(thisWindow.listFileURI, "filePath="+param.filePathName+"&owner="+param.fileOwner);
										if (one.status != 'ok')
											return MyDesktop.messageBox("Error: "+one.reason);

										for (var j=0;j!=one.files.length;j++) {
											var iconType = 'folder', notFolder = (one.files[j].fileType != 'folder');
											if (notFolder)
												iconType = Ext.getExtType(one.files[j].fileName);
											node.appendChild(new Ext.tree.TreeNode({
												text: one.files[j].fileName,
												iconCls: iconType,
												leaf: notFolder,
												fileSize: one.files[j].fileSize,
												loaded: false,
												children: []
											}));
										}
										node.expand(true, true);
										node.attributes.loaded=true;
										return;
									}
									var fileType = Ext.getExtType(node.text), fileSizeField = document.getElementById('file-win-filesize-field');
									if (fileSizeField!=null)
										fileSizeField.innerHTML = node.attributes.fileSize + ' Byte';
									var content = '<< ... binary-file ... >>';
									if (fileType=='editable') {
										if (node.attributes.fileSize > 1024000)
											content = '( File too large, please download instead ... )';
										else
											content = downloadFile(true);
									}
									
									var obj = document.getElementById('file-win-textbox');
									if (obj != null)
										obj.value = content;
								}
							}
						},
						buttons : [{
							text : 'Run',
							handler : function() {
								var tree = Ext.getCmp('im-tree');
								var selectedNode = tree.getSelectionModel().getSelectedNode();
								if (selectedNode==null || selectedNode.getDepth()<=1)
									return MyDesktop.messageBox("You are supposed to select a file!");
								var fileDatas = Ext.getFilePathAndOwner(selectedNode);
								if (fileDatas.fileOwner != userName)
									return MyDesktop.messageBox("You cannot run a program that is not yours!");
								
								var result = MyDesktop.gatewayRequestObjectWithGetSync("/file/run", "fileName="+fileDatas.filePathName+"&owner="+fileDatas.fileOwner);
								if (result.status!='ok')
									MyDesktop.messageBox("Running operation encountered error!<br/>Reason: "+result.reason);
								else
									MyDesktop.messageBox("Successfully started running!<br/>Job ID: "+result.jobID);
							}
						},{
							text : 'Unzip',
							handler : function() {
								var tree = Ext.getCmp('im-tree');
								var selectedNode = tree.getSelectionModel().getSelectedNode();
								if (selectedNode==null || selectedNode.getDepth()<=1)
									return MyDesktop.messageBox("You are supposed to select a file!");
								var fileDatas = Ext.getFilePathAndOwner(selectedNode);
								if (fileDatas.fileOwner != userName)
									return MyDesktop.messageBox("You cannot unzip a file that is not yours!");

								MyDesktop.confirmBox("Note: It may take a while to finish this operation. Are you sure to do it anyway?", function (reply) {
									if (reply == 'yes') {
										var result = MyDesktop.gatewayRequestObjectWithGetSync("/file/unzip", "fileName="+fileDatas.filePathName+"&owner="+fileDatas.fileOwner);
										if (result.status!='ok')
											MyDesktop.messageBox("Unzip operation encountered error!<br/>Reason:"+result.reason);
										thisWindow.refresh(thisWindow);
									}
								},this);
							}
						},{
							text : 'Upload',
							handler : function() {
								var targetUrl='/file/upload' + ('?'+MyDesktop.getCookie() );
								//alert(Ext.encode(Ext.decode('{'+MyDesktop.getCookie().replace(/&/g,'",').replace(/=/g,':"')+'"}')));
								var dialog = new Ext.ux.UploadDialog.Dialog({
									title : 'Upload Files',
									url : targetUrl,
									//post_var_name : 'uploadFiles',
									width : 450,
									height : 300,
									minWidth : 450,
									minHeight : 300,
									draggable : true,
									resizable : true,
									constraintoviewport : true,
									//permitted_extensions : ['jpg','docx'],
									modal : true,
									reset_on_hide : false,
									allow_close_on_upload : false,
									//autoCreate: true,
									//base_params : Ext.decode('{'+MyDesktop.getCookie().replace(/&/g,'",').replace(/=/g,':"')+'"}'),
									upload_autostart : false
								});
								
								dialog.errorList=[];
								dialog.on('uploaderror', onUploadError);
								dialog.on('uploadcomplete', onUploadComplete);
								/*dialog.on('uploadsuccess', onUploadSuccess);
								dialog.on('uploadfailed', onUploadFailed);*/
								dialog.show();
								MyDesktop.desktop.layout();
								
								function onUploadComplete(dialog) {
									thisWindow.refresh(thisWindow);
									var messages="Finish all files uploaded.";
									if (dialog.errorList.length>0)
										messages+='<br/><br/>The following files were not uploaded correctly:<br/>'+dialog.errorList;
									dialog.errorList=[];
									MyDesktop.messageBox(messages, function() {
										dialog.hide();
									});
								};
								
								function onUploadError(dialog, filename, responseJson, record) {
									var index = filename.lastIndexOf('\\')+1;
									dialog.errorList.push(filename.substring(index));
								};
							}
						},{
							text : 'Download',
							handler : downloadFile
						},{
							text : 'Delete',
							handler : function() {
								var tree = Ext.getCmp('im-tree');
								var selectedNode = tree.getSelectionModel().getSelectedNode();
								if (selectedNode==null || selectedNode.getDepth()<=1 )
									return MyDesktop.messageBox("You are supposed to select a file!");

								var fileDatas = Ext.getFilePathAndOwner(selectedNode);
								MyDesktop.confirmBox("Are you sure to delete the file '/"+fileDatas.filePathName+"' owned by '"+fileDatas.fileOwner+"' ?", function (reply) {
									if (reply=='yes') {
										var result = MyDesktop.gatewayRequestObjectWithGetSync(thisWindow.deleteFileURI, "fileName="+fileDatas.filePathName+"&owner="+fileDatas.fileOwner);
										if (result.status=='error')
											MyDesktop.messageBox('Deleting operation failed.<br/>Reason: '+result.reason);
										else
											thisWindow.refresh(thisWindow);
											//selectedNode.parentNode.removeChild(selectedNode);
									}
								},this);
							}
						}]
					})]
		});
	},
	
	afterPrepare : function() {
		this.refresh(this);
	}
});

AppWin.GenericWindows.push({
	visible: [true, false],
	window: FileManage
});

