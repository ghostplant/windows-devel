AccountManage = Ext.extend(AppWin.AbstractWindow, {
	id : 'account-win',
	label : 'User Account',
	bigIcon : 'wine-winecfg.png',
	
	accountLogURI : "/account/get_log",
	accountInfoURI : "/account/get_info",
	accountClusterURI : "/account/get_cluster",
	authenticated : false,

	getAccounts : function(accountInfo) {
		return [{
			user: MyDesktop.getGatewayUsername(),
			role: accountInfo.role,
			quota: accountInfo.quota
		}];
	},
	
	prepareItems : function() {
		var thisWindow = this, accoutInfoArray = [];

		var accountInfo = MyDesktop.gatewayRequestObjectWithGetSync(thisWindow.accountInfoURI);
		if (accountInfo.status!='ok') {
			MyDesktop.messageBox('Error: '+accountInfo.reason);
			return null;
		}

		var accounts = thisWindow.getAccounts(accountInfo);
		for (var i=0;i!=accounts.length;i++)
			accoutInfoArray[i] = [ accounts[i].user, accounts[i].role, accounts[i].quota ];
		
		var displayLog = function() {
			var record = Ext.getCmp('account-grid').getSelectionModel().getSelected();
			if (record == null)
				return MyDesktop.messageBox("You are supposed to select a user!");
			var userName = record.get("user");
			var accountLog = MyDesktop.gatewayRequestObjectWithGetSync(thisWindow.accountLogURI, "owner="+userName);
			if (accountLog.status!='ok')
				return MyDesktop.messageBox("Error: "+accountLog.reason);
			var textArea = document.getElementById('user_log_area');
			textArea.value = 'User: '+userName+';  TimeStamp: '+accountLog.timeStamp+'\n\n'+accountLog.accountLog;
			textArea.scrollTop=textArea.scrollHeight;
		};
		
		var getCluster = function() {
			var record = Ext.getCmp('account-grid').getSelectionModel().getSelected();
			if (record == null)
				return MyDesktop.messageBox("You are supposed to select a user!");
			var userName = record.get("user");
						
			var getCluster = MyDesktop.gatewayRequestObjectWithGetSync(thisWindow.accountClusterURI, "owner="+userName);
			if (getCluster.status!='ok')
				return MyDesktop.messageBox("Error: "+getCluster.reason);
			var hosts = getCluster.hosts, nodes = getCluster.nodes, store = Ext.getCmp('hostip-grid').store;
			store.removeAll();
			for (var i=0;i<hosts.length;i++)
				store.add(new Ext.data.Record({
					'hostIp': hosts[i].ip
				}));

			store = Ext.getCmp('cluster-grid').store;
			store.removeAll();
			for (var i=0;i<nodes.length;i++)
				store.add(new Ext.data.Record({
					'nname': nodes[i].name,
					'nvnode': nodes[i].nvnode,
					'vnodes': Ext.encode(nodes[i].vnodes)
				}));
			// alert(Ext.encode(getCluster));
			// {"nhost":1,"hosts":[{"ip":"192.168.4.218"}],"nnode":1,"nodes":[{"name":"Virtual Cluster 1","nvnode":1,"vnodes":[{"vnode":"unias_node_0@192.168.4.218"}]}]}
		};

		return new Ext.Panel({
			layout: 'border',
			items: [{
				region: 'center',
				items: new Ext.form.TextArea({
					id: 'user_log_area',
					style: 'height:100%;width:100%;font-size:20px;color:white;background:black',
					readOnly: true
				})
			},{
				region: 'west',
				split:true,
				width: 310,
				items: new Ext.grid.GridPanel({
					id : 'account-grid',
					border : false,
					ds : new Ext.data.Store({
						id : 'accountInfo',
						reader : new Ext.data.ArrayReader({}, [{ name : 'user' },{ name : 'role' },{ name : 'quota' } ]),
						data : accoutInfoArray
					}),
					cm : new Ext.grid.ColumnModel([
						new Ext.grid.RowNumberer(),
						{ header : "UserName", width : 40, sortable : true, dataIndex : 'user' },
						{ header : "Role", width : 60, sortable : true, dataIndex : 'role' },
						{ header : "Quota", width : 40, sortable : true, dataIndex : 'quota' }
					]),
					viewConfig : { forceFit : true },
					listeners : {
						'rowdblclick' : function() {
							displayLog();
							getCluster();
						}
					}
				})
			},{
				region: 'south',
				split:true,
				height: 240,
				layout: 'border',
				items: [ new Ext.grid.GridPanel({
					region : "west",
					id : 'hostip-grid',
					ds : new Ext.data.Store({
						reader : new Ext.data.ArrayReader({}, [{ name : 'hostIp' } ]),
						data : []
					}),
					cm : new Ext.grid.ColumnModel([
						new Ext.grid.RowNumberer(),
						{ header : "Host IP", sortable : true, dataIndex : 'hostIp' }
					]),
					width: 200,
					split:true,
					viewConfig : { forceFit : true }
				}), new Ext.grid.GridPanel({
					region : "center",
					id : 'cluster-grid',
					ds : new Ext.data.Store({
						reader : new Ext.data.ArrayReader({}, [{ name : 'nname' },{ name : 'nvnode' },{ name : 'vnodes' } ]),
						data : []
					}),
					cm : new Ext.grid.ColumnModel([
						new Ext.grid.RowNumberer(),
						{ header : "VCluster Name", sortable : true, dataIndex : 'nname' },
						{ header : "VNodes Number", sortable : true, dataIndex : 'nvnode' },
						{ header : "VNodes", width:400 , sortable : true, dataIndex : 'vnodes' }
					]),
					viewConfig : { forceFit : true }
				})],
				buttons: [
					{text:'Update Log', handler: displayLog },
					{text:'Change Role', disabled: !thisWindow.authenticated, handler: function() {
						var record = Ext.getCmp('account-grid').getSelectionModel().getSelected();
						if (record == null)
							return MyDesktop.messageBox("You are supposed to select a user!");
						var userName = record.get("user"), role = record.get("role");
						var newRole = (role=='ADMINISTRATOR')?'AUTHENTICATED':'ADMINISTRATOR';
						MyDesktop.confirmBox("Are you sure to do the following operation?<br/><br/>Change "+userName+"'s role from "+role+" to "+newRole+".", function(reply){
							if (reply=='yes') {
								var changeRole = MyDesktop.gatewayRequestObjectWithGetSync("/admin/account/change_role", "owner="+userName+"&newRole="+newRole);
								if (changeRole.status=='ok') {
									record.set("role", newRole);
									MyDesktop.messageBox("Successfully changing the role!", function() {
										if (MyDesktop.getGatewayUsername()==userName) {
											Ext.appStorage.setItem('level', 0);
											location.reload();
										}
									});
								} else
									MyDesktop.messageBox("Error: "+changeRole.reason);
							}
						});
					}},
					{text:'Change Quota', disabled: !thisWindow.authenticated, handler: function() {
						var record = Ext.getCmp('account-grid').getSelectionModel().getSelected();
						if (record == null)
							return MyDesktop.messageBox("You are supposed to select a user!");
						var userName = record.get("user");
						MyDesktop.promptBox("Please input a positive number for the new quota of "+userName+":", function(reply, text) {
							if (reply=='ok') {
								var newQuota=parseInt(text);
								if (isNaN(newQuota) || newQuota<=0)
									return MyDesktop.messageBox("Error: Quota input '"+text+"' is not valid!");
								var changeQuota = MyDesktop.gatewayRequestObjectWithGetSync("/admin/account/quota", "owner="+userName+"&quota="+newQuota);
								if (changeQuota.status=='ok') {
									record.set("quota", newQuota);
									MyDesktop.messageBox("Quota changed successfully!");
								} else
									MyDesktop.messageBox("Error: "+changeQuota.reason);
							}
						});
					}}
				]
			}]
		});
	}
});

AppWin.GenericWindows.push({
	visible: [true, false],
	window: AccountManage
});

