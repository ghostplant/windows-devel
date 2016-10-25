AdminAccountManage = Ext.extend(AccountManage, {
	id : 'admin-account-win',
	
	accountLogURI : "/admin/account/get_log",
	accountInfoURI : "/admin/account/list_accounts",
	accountClusterURI : "/admin/account/get_cluster",
	authenticated : true,

	getAccounts : function(accountInfo) {
		return accountInfo.accounts;
	}
});

AppWin.GenericWindows.push({
	visible: [false, true],
	window: AdminAccountManage
});

