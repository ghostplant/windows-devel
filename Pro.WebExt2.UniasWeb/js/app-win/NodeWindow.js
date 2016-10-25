NodeManage = Ext.extend(AppWin.AbstractWindow, {
	id : 'node-win',
	label : 'Cluster Info',
	bigIcon : 'gnome-glines.png',
	
	prepareItems : function() {
		var node = MyDesktop.gatewayRequestObjectWithGetSync("/admin/node/host/list_hosts");
		if (node.status!='ok')
			return MyDesktop.messageBox('Error: '+node.reason);

		var hostInfoArray = new Array();
		node.clusterPortal = node.clusterPortal;

		MyDesktop.hostUsageUpdate = function(hostIp, recordIndex) {
			var hostUsage=MyDesktop.gatewayRequestObjectWithGetSync("/admin/node/host/get_usage", 'hostIP='+hostIp);
			if (hostUsage.status!='ok')
				MyDesktop.messageBox('Error: '+hostUsage.reason); // clearTimeout
			else {
				var cmp=Ext.getCmp('host-grid');
				if (cmp == null)
					return;
				var record = cmp.store.getAt(recordIndex);
				record.set('ts', hostUsage.timeStamp);
				record.set('cr', hostUsage.cpuRate);
				record.set('ir', hostUsage.ioRate);
				record.set('mr', hostUsage.memRate);
				record.set('mt', hostUsage.memTotal);
				record.set('mf', hostUsage.memFree);
				record.set('nc', hostUsage.netCost);
				
				setTimeout("MyDesktop.hostUsageUpdate('"+hostIp+"', "+recordIndex+")", 2000);
			}
		};

		for (var i = 0; i != node.hosts.length; i++) {
			hostInfoArray[i] = [ node.hosts[i].host, '0000-00-00 00:00:00' ];
			setTimeout("MyDesktop.hostUsageUpdate('"+node.hosts[i].host+"', "+i+")", 2000);
		}
		var getHostInfo = function(record) {
			if (record==null)
				MyDesktop.messageBox( "You are supposed to select a host item.");
			else {
				var hostinfo=MyDesktop.gatewayRequestObjectWithGetSync("/admin/node/host/get_info", 'hostIP='+record.get('host'));
				if (hostinfo.status=='ok') {
					var store = Ext.getCmp('host-detail-grid').store;
					store.removeAll();
					store.add(new Ext.data.Record({
						'host': record.get('host'),
						'ts': hostinfo.timeStamp,
						'processLimit': hostinfo.hostInfo.processLimit,
						'osType': hostinfo.hostInfo.osType,
						'osVersion': hostinfo.hostInfo.osVersion,
						'cpuThreads': hostinfo.hostInfo.cpuThreads,
						'systemVersion': hostinfo.hostInfo.systemVersion
					}));
				} else
					MyDesktop.messageBox('Error: '+hostinfo.reason);
			}
		};

		var getHostUsage = function(record) {
			if (record==null)
				MyDesktop.messageBox( "You are supposed to select a host item.");
			else {
				/*{
					text : "Host Usage",
					handler : function() {
						getHostUsage(Ext.getCmp('host-grid').getSelectionModel().getSelected());
					}
				},*/
				
				/*var hostUsage=MyDesktop.gatewayRequestObjectWithGetSync("/admin/node/host/get_usage", 'hostIP='+record.get('host'));
				if (hostUsage.status=='ok')
					setTimeout("NodeManage.hostUsageUpdate('"+record.get('host')+"')",0); //{"status":"ok","timeStamp":"{{2014,4,13},{16,32,9}}","cpuRate":0.02359046945034206,"ioRate":"n/a","memRate":0.013371042404438377,"memTotal":101578088448,"memFree":100219883520,"netCost":"n/a"}
				else
					MyDesktop.messageBox('Error: '+hostUsage.reason);*/
			}
		};

		var win=this.window, items = new Ext.Panel({
			items: [ {
				title : "Cluster Portal",
				collapsible:true,
				html : '<p>' + node.clusterPortal + '</p>'
			}, new Ext.grid.GridPanel({
				id : 'host-grid',
				title : "Host Info",
				border : false,
				collapsible:true,
				autoHeight:true,
				ds : new Ext.data.Store({
					id : 'hostInfo',
					reader : new Ext.data.ArrayReader({}, [{ name : 'host' },/*{ name : 'load' },*/{ name : 'ts' },{ name : 'cr' },{ name : 'ir' },{ name : 'mr' },{ name : 'mt' },{ name : 'mf' },{ name : 'nc' } ]),
					data : hostInfoArray
				}),
				cm : new Ext.grid.ColumnModel([
					new Ext.grid.RowNumberer(),
					{ header : "Host", sortable : true, dataIndex : 'host' },
					/*{ header : "Load", sortable : true, dataIndex : 'load' },*/
					{ header : "TimeStamp", width:140 , sortable : true, dataIndex : 'ts' },
					{ header : "CpuRate", sortable : true, dataIndex : 'cr' },
					{ header : "IoRate", sortable : true, dataIndex : 'ir' },
					{ header : "MemRate", sortable : true, dataIndex : 'mr' },
					{ header : "MemTotal", sortable : true, dataIndex : 'mt' },
					{ header : "MemFree", sortable : true, dataIndex : 'mf' },
					{ header : "NetCost", sortable : true, dataIndex : 'nc'}
				]),
				//viewConfig : { forceFit : true },
				listeners : {
					'rowdblclick' : function(grid, rowIndex, e) {
						var model=grid.getSelectionModel();
						model.clearSelections();
						model.selectRow(rowIndex);
						getHostInfo(model.getSelected());
					}
				}
			}), new Ext.grid.GridPanel({
				id : 'host-detail-grid',
				title : "Host Detail",
				border : false,
				collapsible:true,
				autoHeight:true,
				ds : new Ext.data.Store({
					reader : new Ext.data.ArrayReader({}, [{ name : 'host' }, { name : 'ts' },{ name : 'processLimit' },{ name : 'osType' },{ name : 'osVersion' },{ name : 'cpuThreads' },{ name : 'systemVersion' } ]),
					data : []
				}),
				cm : new Ext.grid.ColumnModel([
					new Ext.grid.RowNumberer(),
					{ header : "Host", sortable : true, dataIndex : 'host' },
					/*{ header : "Load", sortable : true, dataIndex : 'load' },*/
					{ header : "TimeStamp", width:140 , sortable : true, dataIndex : 'ts' },
					{ header : "ProcessLimit", sortable : true, dataIndex : 'processLimit' },
					{ header : "OsType", sortable : true, dataIndex : 'osType' },
					{ header : "OsVersion", sortable : true, dataIndex : 'osVersion' },
					{ header : "CpuThreads", sortable : true, dataIndex : 'cpuThreads' },
					{ header : "SystemVersion", width:270 , sortable : true, dataIndex : 'systemVersion' }
				]),
				//viewConfig : { forceFit : true },
				listeners : {
					'rowdblclick' : function(grid, rowIndex, e) {
						var model=grid.getSelectionModel();
						model.clearSelections();
						model.selectRow(rowIndex);
						getHostInfo(model.getSelected());
					}
				}
			}) ],
			buttons : [{
				text:'Show Detail',
				handler: function() {
					getHostInfo(Ext.getCmp('host-grid').getSelectionModel().getSelected());
				}
			}/*,{
				text : "Close",
				handler : function() {
					win.handle.close();
				}
			}*/]
		});
		return items;
	}
});

AppWin.GenericWindows.push({
	visible: [false, true],
	window: NodeManage
});

