JobManage = Ext.extend(AppWin.AbstractWindow, {
	id : 'job-win',
	label : 'Task Manager',
	bigIcon : 'utilities-system-monitor.png',
	
	jobListURI : "/job/list_jobs",
	jobLogURI : "/job/get_log",
	jobUsageURI : "/job/get_usage",
	jobInfoURI : "/job/get_info",
	
	prepareItems : function() {
		var thisWindow = this, listJobs = MyDesktop.gatewayRequestObjectWithGetSync(thisWindow.jobListURI);
		if (listJobs.status!='ok') {
			MyDesktop.messageBox('Error: '+listJobs.reason);
			return null;
		}
		var runningGridDatas = [], jobs = listJobs.jobs, terminatedGridDatas=[];
		for (var i=0;i!=jobs.length;i++)
			if (jobs[i].jobStatus=='RUNNING')
				runningGridDatas.push([jobs[i].jobID, jobs[i].jobName, jobs[i].jobOwner]);
			else {
				var query = MyDesktop.gatewayRequestObjectWithGetSync(thisWindow.jobInfoURI, "jobID="+jobs[i].jobID+"&owner="+jobs[i].jobOwner);
				terminatedGridDatas.push([jobs[i].jobID, jobs[i].jobName, jobs[i].jobOwner, jobs[i].jobStatus, query.startTime, query.endTime]);
			}
		
		var getItemSelectedParam = function() {
			var record = Ext.getCmp('running-job-grid').getSelectionModel().getSelected();
			if (record==null) {
				MyDesktop.messageBox('You are supposed to select an item.');
				return null;
			}
			return 'jobID='+record.get('jid')+'&owner='+record.get('owner');
		};

		var getJobDetail = function() {
			var param = getItemSelectedParam();
			if (param==null)
				return;
			var jobInfo = MyDesktop.gatewayRequestObjectWithGetSync(thisWindow.jobInfoURI, param);
			if (jobInfo.status!='ok')
				MyDesktop.messageBox('Error: '+jobInfo.reason);
			else {
				document.getElementById('running_job_id').innerHTML=jobInfo.jobID;
				document.getElementById('running_job_name').innerHTML=jobInfo.jobName;
				document.getElementById('running_start_time').innerHTML=jobInfo.startTime;
				document.getElementById('running_job_container').innerHTML=jobInfo.jobContainer;
			}
		};
		
		var getJobLog = function() {
			var param = getItemSelectedParam();
			if (param==null)
				return;
			var jobLog = MyDesktop.gatewayRequestObjectWithGetSync(thisWindow.jobLogURI, param);
			if (jobLog.status!='ok')
				MyDesktop.messageBox('Error: '+jobLog.reason);
			else {
				var textArea = document.getElementById('job_log_area');
				textArea.value='[ THIS-LOG-TIME-STAMP ] '+jobLog.timeStamp+'\n\n'+jobLog.jobLog;
				textArea.scrollTop=textArea.scrollHeight;
			}
		};
		
		var doScale = function() {
			var param = getItemSelectedParam();
			if (param==null)
				return;
			MyDesktop.confirmBox("Are you sure to scale this job?", function(reply) {
				if (reply!='yes')
					return;
				var jobScale = MyDesktop.gatewayRequestObjectWithGetSync("/job/scale", param);
				if (jobScale.status!='ok')
					MyDesktop.messageBox('Error: '+jobScale.reason);
				else
					MyDesktop.messageBox('Job scale succeed!');
			});
			
		};
		
		var doStop = function() {
			var param = getItemSelectedParam();
			if (param==null)
				return;
			MyDesktop.confirmBox("Are you sure to stop this job?", function(reply) {
				if (reply!='yes')
					return;
				var jobScale = MyDesktop.gatewayRequestObjectWithGetSync("/job/stop", param);
				if (jobScale.status!='ok')
					MyDesktop.messageBox('Error: '+jobScale.reason);
				else {
					var runGrid = Ext.getCmp('running-job-grid'), stopGrid = Ext.getCmp('terminated-job-grid');
					var record = runGrid.getSelectionModel().getSelected();
					var query = MyDesktop.gatewayRequestObjectWithGetSync(thisWindow.jobInfoURI, "jobID="+record.get('jid')+"&owner="+record.get('owner'));
					record.set('status', 'STOPPING');
					record.set('startTime', query.startTime);
					record.set('endTime', query.endTime);
					stopGrid.store.add(record);
					runGrid.store.remove(record);
					MyDesktop.messageBox('Stop request is sent successfully!');
				}
			});
		};

		var width = 1500, height = 700;
		var getJobMonitor = function() {
			var param = getItemSelectedParam();
			if (param == null)
				return;
			var jobID = param.split('&')[0].split('=')[1], tabID = 'job_tab_panel_j'+jobID, canvasID = 'job_monitor_canvas_j'+jobID;
			var tab = Ext.getCmp('job_tab_panel').getItem(tabID);
			if (tab != null)
				return tab.show();

			var closed = false;
			Ext.getCmp('job_tab_panel').add({
				id: tabID,
				title: 'Process Monitor (JID:'+jobID+')',
				tbar: [{
					text: 'Job Info',
					iconCls: 'settings',
					handler: function() {
						MyDesktop.messageBox("This job ID is "+jobID);
					}
				}, {
					text: 'Close This Monitor',
					iconCls: 'settings',
					handler: function() {
						var tabPanel = Ext.getCmp('job_tab_panel');
						closed = true;
						tabPanel.remove(tabPanel.getItem(tabID));
					}
				}],
				html: '<canvas id="'+canvasID+'" style="height:100%;width:100%" height='+height+' width='+width+'>Your browser is too old to support Web Draw.</canvas>'
			}).show();
			var context = document.getElementById(canvasID).getContext('2d');
			context.fillStyle='black';
			context.fillRect(0,0, width, height);
	
			var prevProcCpuCost = [], prevPos = [], ratio = 18;
							
			var drawCanvas = function(ctx, nodes) {
				var len = nodes.length;
				ctx.font="28px sans-serif";
				ctx.lineWidth = 3;
				var clientWidth = width-20, clientHeight = height-40;
				var top = 10, bottom, left=10, right=clientWidth+10;
				for (var i=0; i!=len; i++, top=bottom+10) {
					bottom = (i+1)*clientHeight/len;
					ctx.fillStyle='#4F4F4F';
					ctx.fillRect(left,top,right-left,bottom-top);

					var memLow = bottom-top-50, memHeight = memLow*parseInt(nodes[i].sysMemRate)/100;
					ctx.fillStyle='#DCDCDC';
					ctx.fillRect(left+10,top+10,100,bottom-top-50); // MemBox
					ctx.fillStyle='#4169E1';
					ctx.fillRect(left+10,top+10+memLow-memHeight,100,memHeight); // MemUse
					ctx.fillStyle='#ADFF2F';
					ctx.fillText("SysMem",left+10,bottom-10); // MemText

					var cpuLow = bottom-top-50, cpuHeight = cpuLow*parseInt(nodes[i].sysCpuRate)/100;
					ctx.fillStyle='#DCDCDC';
					ctx.fillRect(left+150,top+10,100,bottom-top-50); // CpuBox
					ctx.fillStyle='#FFFF00';
					ctx.fillRect(left+150,top+10+cpuLow-cpuHeight,100,cpuHeight); // CpuUse
					ctx.fillStyle='#ADFF2F';
					ctx.fillText("SysCpu",left+150,bottom-10); // CpuText

					var ioLow = bottom-top-50, ioHeight = ioLow*parseInt(nodes[i].sysIoRate)/100;
					ctx.fillStyle='#DCDCDC';
					ctx.fillRect(left+290,top+10,100,bottom-top-50); // IoBox
					ctx.fillStyle='#FFFF00';
					ctx.fillRect(left+290,top+10+ioLow-ioHeight,100,ioHeight); // IoUse
					ctx.fillStyle='#ADFF2F';
					ctx.fillText("SysIO",left+290,bottom-10); // IoText

					var totalTextHeight = bottom-top-20;
					var realProcCpuCost = Math.floor(parseInt(nodes[i].procCpuCost)*1e-2/ratio);
					ctx.fillStyle='#ADFF2F';
					ctx.fillText("SMT: ",left+430, top+5+0*totalTextHeight/8+30);
					ctx.fillText("SMF: ",left+430, top+5+1*totalTextHeight/8+30);
					ctx.fillText("SNC: ",left+430, top+5+2*totalTextHeight/8+30);
					ctx.fillText("PCC: ",left+430, top+5+3*totalTextHeight/8+30);
					ctx.fillText("PMC: ",left+430, top+5+4*totalTextHeight/8+30);
					ctx.fillText("PMP: ",left+430, top+5+5*totalTextHeight/8+30);
					ctx.fillText("PIC: ",left+430, top+5+6*totalTextHeight/8+30);
					ctx.fillText("PTU: ",left+430, top+5+7*totalTextHeight/8+30);
					ctx.fillStyle='#FFFFFF';
					ctx.fillText(nodes[i].sysMemTotal+" KB",	left+500, top+5+0*totalTextHeight/8+30);
					ctx.fillText(nodes[i].sysMemFree+" KB",		left+500, top+5+1*totalTextHeight/8+30);
					ctx.fillText(nodes[i].sysNetCost+" KB",		left+500, top+5+2*totalTextHeight/8+30);
					ctx.fillText(realProcCpuCost+" s",			left+500, top+5+3*totalTextHeight/8+30);
					ctx.fillText(nodes[i].procMemCurrent+" KB",	left+500, top+5+4*totalTextHeight/8+30);
					ctx.fillText(nodes[i].procMemPeak+" KB",	left+500, top+5+5*totalTextHeight/8+30);
					ctx.fillText(nodes[i].procIoCost+" KB",		left+500, top+5+6*totalTextHeight/8+30);
					ctx.fillText(nodes[i].procTimeUse+" s",	left+500, top+5+7*totalTextHeight/8+30);

					var cpuHistoryLeft = left+750, cpuHistoryTop = top+10, cpuHistoryWeight = right-10-cpuHistoryLeft, cpuHistoryHeight = bottom-10-cpuHistoryTop;
					ctx.strokeStyle='#FFFFFF';
					ctx.strokeRect(cpuHistoryLeft, cpuHistoryTop, cpuHistoryWeight, cpuHistoryHeight);
					
					var diff=10, recordCount = cpuHistoryWeight/diff;
					if (prevProcCpuCost[i] != null) {
						//var cpuRate = parseInt(nodes[i].sysCpuRate);
						var cpuRate = (parseInt(nodes[i].procCpuCost) - parseInt(prevProcCpuCost[i]))/(ratio+ratio);
						var procHeight = Math.floor(cpuHistoryHeight*cpuRate/100);
						var pos = cpuHistoryTop+cpuHistoryHeight-procHeight;
						if (pos < cpuHistoryTop)
							pos = cpuHistoryTop;
						if (prevPos[i] == null) {
							var make=(cpuHistoryTop+cpuHistoryHeight);
							prevPos[i] = ""+make;
							for (var j=0;j<recordCount;j++)
								prevPos[i]+=","+make;
						}
						prevPos[i]=prevPos[i].substring(prevPos[i].indexOf(',')+1)+","+pos;

						ctx.beginPath();
						ctx.strokeStyle='#FF0000';
						var datas = prevPos[i].split(",");
						ctx.moveTo(cpuHistoryLeft+cpuHistoryWeight/datas.length, datas[0]);
						for (var j=1;j<datas.length;j++)
							ctx.lineTo(cpuHistoryLeft+(j+1)*cpuHistoryWeight/datas.length, datas[j]);
						ctx.stroke();
					}
					prevProcCpuCost[i] = nodes[i].procCpuCost;
				}
				return true;
			};
			
			var timerEvent = function() {
				var monitor = document.getElementById(canvasID);
				if (!closed && monitor) {
					var jobUsage = MyDesktop.gatewayRequestObjectWithGetSync(thisWindow.jobUsageURI, param);
					if (jobUsage.status!='ok' || jobUsage.nodes.length==0)
						return MyDesktop.messageBox("Monitor Error: "+jobUsage.reason);
					var context=monitor.getContext('2d');
					context.fillStyle='black';
					context.fillRect(0,0, width, height);
					if (drawCanvas(context, jobUsage.nodes))
						setTimeout(timerEvent, 5000);
				}
			};
			timerEvent();
		};
		
		return new Ext.TabPanel({
			id: 'job_tab_panel',
			activeTab : 0,
			items: [{
				title : 'Runing Jobs',
				layout:"border",
				items: [
					{region:"south",title:"Job Operation",split:true,collapsible:true,buttons:[/*{
						text:'Show Detail',
						handler: getJobDetail
					},*/{
						text:'Update Log',
						handler: getJobLog
					},{
						text:'Scale',
						handler: doScale
					},{
						text:'Stop',
						handler: doStop
					},{
						text:'Job Monitor',
						handler: getJobMonitor
					}]},
					{region:"west",title:"Job List, on: "+ listJobs.timeStamp,split:true,items: new Ext.grid.GridPanel({
						id : 'running-job-grid',
						border : false,
						ds : new Ext.data.Store({
							autoLoad:true,
							reader : new Ext.data.ArrayReader({}, [{ name : 'jid' }, { name : 'jobname' }, { name : 'owner' }]),
							data : runningGridDatas
						}),
						cm : new Ext.grid.ColumnModel([
							new Ext.grid.RowNumberer(),
							{ header : "JID", width:40, sortable : true, dataIndex : 'jid' },
							{ header : "Job Name", width:100, sortable : true, dataIndex : 'jobname' },
							{ header : "Owner", width:80, sortable : true, dataIndex : 'owner' }
						]),
						autoHeight: true,
						monitorResize: true,
						listeners : {
							'rowdblclick' : function() {
								getJobDetail();
								getJobLog();
								//getJobMonitor();
							}
						}
					}), collapsible:true, width:270},
					{region:"east", width:200, split:true, collapsible:true, items:[{
						title:'Job ID',
						collapsible:true,
						html:'<p id="running_job_id"></p>'
					},{
						title:'Job Name',
						collapsible:true,
						html:'<p id="running_job_name"></p>'
					},{
						title:'Start Time',
						collapsible:true,
						html:'<p id="running_start_time"></p>'
					},{
						title:'Job Container',
						collapsible:true,
						html:'<p id="running_job_container"></p>'
					}/*,{
						title:'Work Nodes',
						collapsible:true,
						html:'<p id="running_work_nodes"></p>'
					}*/]},
					{region:"center", title:'Latest Logs', items: new Ext.form.TextArea({
						id: 'job_log_area',
						style:'height:100%;width:100%;font-size:16px;color:white;background:black',
						readOnly: true
					})}
				]
			}/*,{
				title : 'Process Monitor',
				html: '<canvas id="job_monitor_canvas" style="height:100%;width:100%" height='+height+' width='+width+'>Your browser is too old to support Web Draw.</canvas>'
			}*/,{
				title : 'Terminated Jobs',
				layout:"border",
				items: [
					new Ext.grid.GridPanel({
						id : 'terminated-job-grid',
						region:"center",
						border : false,
						title:"Job List",
						split:true,
						ds : new Ext.data.Store({
							reader : new Ext.data.ArrayReader({}, [{ name : 'jid' }, { name : 'jobname' }, { name : 'owner' }, { name : 'status' }, { name : 'startTime' }, { name : 'endTime' }]),
							data : terminatedGridDatas
						}),
						cm : new Ext.grid.ColumnModel([
							new Ext.grid.RowNumberer(),
							{ header : "JID", sortable : true, dataIndex : 'jid' },
							{ header : "Job Name", sortable : true, dataIndex : 'jobname' },
							{ header : "Owner", sortable : true, dataIndex : 'owner' },
							{ header : "Status", sortable : true, dataIndex : 'status' },
							{ header : "Start Time", sortable : true, dataIndex : 'startTime' },
							{ header : "End Time", sortable : true, dataIndex : 'endTime' }
						]),
						viewConfig : { forceFit : true },
						listeners : {
							'rowdblclick' : function(grid, rowIndex, e) {
								var record = grid.getSelectionModel().getSelected();
								MyDesktop.messageBox('This job (ID='+record.get('jid')+') is finally '+record.get('status'));
							}
						}
					})/*, {
						region:"south",
						title:"Job Operation",
						split:true,
						collapsible:true,
						buttons:[{
							text:'Show Info',
							disabled : true,
							handler: function() {
								MyDesktop.messageBox('// To do: ...');
							}
						}, {
							text:'Show Log',
							handler:getTerminatedJobLog
						}]
					}*/
				]
			}]
		});
	}
});

AppWin.GenericWindows.push({
	visible: [true, false],
	window: JobManage
});

