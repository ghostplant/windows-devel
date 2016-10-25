MyDesktop = new Ext.app.App( {

	init : function() {
		Ext.QuickTips.init();
	},

	getModules : function() {
		var appWindows = [];
		var level = Ext.appStorage.getItem('level');
		for (var i=0; i!=AppWin.GenericWindows.length;i++) {
			var appItem = AppWin.GenericWindows[i];
			if (appItem.visible[level])
				appWindows.push(new appItem.window());
		}
		return appWindows;
	},

	getStartConfig : function() {
		var userName = this.getGatewayUsername();

		var struct = {
			title : userName,
			iconCls : 'member',
			toolItems : [ {
				text : 'Unias.Org',
				iconCls : 'user-kid',
				scope : this,
				listeners : {
					click : function() {
						window.open('http://www.unias.org');
					}
				}
			}, {
				text : 'Contact Us',
				iconCls : 'user-girl',
				scope : this,
				listeners : {
					click : function() {
						window.open('http://www.unias.org/trac/unias/contact');
					}
				}
			}, '-', {
				text : 'Logout',
				iconCls : 'logout',
				scope : this,
				listeners : {
					click : function() {
						MyDesktop.confirmBox("Are you sure to logout?", function(reply) {
							if (reply=="yes") {
								var reply = MyDesktop.gatewayRequestObjectWithGetSync('/account/logout');
								if (reply.status == 'error')
									alert('Logout encountered error from server!');
								MyDesktop.setCookie(null);
								location.reload();
							}
						});
					}
				}
			} ]
		};
		return struct;
	}
});

