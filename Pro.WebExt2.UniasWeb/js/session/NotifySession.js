NotifyWindow = new Ext.Window({
	id : 'notify-win',
	el : 'notify-window',
	title : 'Notification Center',
	width : 350,
	height : 450,
	closable : false,
	draggable : false,
	resizable : false,
	items: [{
		html: '<p id="notify_local_time"></p>'
	}, new Ext.grid.GridPanel({
		id : 'nofity-grid',
		border : false,
		autoHeight: true,
		ds : new Ext.data.Store({
			reader : new Ext.data.ArrayReader({}, [{ name : 'time' }, { name : 'nofity' }]),
			data : []
		}),
		cm : new Ext.grid.ColumnModel([
			{ header : "Time", width : 120, sortable : true, dataIndex : 'time' },
			{ header : "Got Messages", width : '90%', sortable : true, dataIndex : 'nofity' }
		]),
		listeners : {
			'rowdblclick' : function(grid, rowIndex, e) {
				var model=grid.getSelectionModel();
				model.clearSelections();
				model.selectRow(rowIndex);
				var record = model.getSelected();
				var datas = record.get('datas');
				MyDesktop.messageBox("[From Notification Center]<br/>"
					+"<br/>Job ID: "+datas.jobID
					+"<br/>Job Name: "+datas.jobName
					+"<br/>Job Status: "+datas.jobStatus
					+"<br/>Status Message: "+datas.statusMessage
					+"<br/>Start Time: "+datas.startTime
					+"<br/>End Time: "+datas.endTime);
				//grid.store.remove(record);
			}
		}
	})],
	buttons: [{
		text: 'Clear Messages',
		handler: function() {
			MyDesktop.confirmBox('Are you sure to clear all current notified messages?', function(reply) {
				if (reply=='yes') {
					if (Ext.appStorage.getItem('notify')=='')
						MyDesktop.messageBox('There is no message at all.');
					else {
						Ext.appStorage.setItem('notify', '');
						NotifyWindow.reloadNotification();
					}
				}
			});
		}
	}]
});

NotifyWindow.reloadNotification = function() {
	var msgs = Ext.appStorage.getItem('notify'), replies = msgs.split(Ext.splitter), store = Ext.getCmp('nofity-grid').store;
	store.removeAll();
	for (var i=0;i!=replies.length;i++) {
		if (replies[i].length==0)
			continue;
		var reply = Ext.decode(replies[i]);
		store.add(new Ext.data.Record({
			'time': reply.timeStamp,
			'nofity': 'Job ID ' + reply.jobID + ' was ' + reply.jobStatus+'.',
			'datas': reply
		}));
	}
};

