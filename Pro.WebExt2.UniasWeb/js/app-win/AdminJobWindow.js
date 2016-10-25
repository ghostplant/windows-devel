AdminJobManage = Ext.extend(JobManage, {
	id : 'admin-job-win',
	
	jobListURI : "/admin/job/list_jobs",
	jobLogURI : "/admin/job/get_log",
	jobUsageURI : "/admin/job/get_usage",
	jobInfoURI : "/admin/job/get_info"
});

AppWin.GenericWindows.push({
	visible: [false, true],
	window: AdminJobManage
});

