var uriPrefix='';

Ext.splitter = '<ext-splitter/>';
Ext.appStorage = sessionStorage;

Ext.app.App = function(cfg) {
	Ext.apply(this, cfg);
	this.addEvents({
		'ready' : true,
		'beforeunload' : true
	});

	Ext.onReady(this.initApp, this);
};

Ext.extend(Ext.app.App, Ext.util.Observable, {
	isReady : false,
	startMenu : null,
	modules : null,

	getStartConfig : function() {
	},
	
	getCookie : function() {
		return Ext.appStorage.getItem('cookie');
	},
	
	setCookie : function(value) {
		Ext.appStorage.setItem('cookie', value);
	},
	
	messageBox : function(msg, reply) {
		Ext.MessageBox.alert("Message", msg, reply);
		Ext.MessageBox.getDialog().getEl().setStyle('z-index','80000');
		if (this.desktop != null)
			this.desktop.layout();
	},
	
	confirmBox : function(msg, reply) {
		Ext.MessageBox.confirm("Confirm", msg, reply);
		Ext.MessageBox.getDialog().getEl().setStyle('z-index','80000');
		if (this.desktop != null)
			this.desktop.layout();
	},
	
	promptBox : function(msg, reply) {
		Ext.MessageBox.prompt("Prompt", msg, reply);
		Ext.MessageBox.getDialog().getEl().setStyle('z-index','80000');
		if (this.desktop != null)
			this.desktop.layout();
	},

	requestObjectWithGetSync : function(uri, args) {
		var conn = Ext.lib.Ajax.getConnectionObject().conn;
		var requestTarget = uri;
		if (args != null && args != '')
			requestTarget += '?' + args;
		conn.open("GET", uriPrefix+requestTarget, false);
		try { conn.send(null); } catch (e) {}
		if (conn.responseText[0]=='{')
			return Ext.decode(conn.responseText);
		alert(conn.responseText);
		return Ext.decode('{"status":"error","reason":"Critical Error from Server that received message is not JSON!"}');
	},

	gatewayRequestObjectWithGetSync : function(uri, extraArgs) {
		var token = this.getCookie();
		if (token == '' || token == null) {
			location.reload();
			return null;
		}
		var args = token;
		if (extraArgs != null && extraArgs != '')
			args += '&' + extraArgs;
		return this.requestObjectWithGetSync(uri, args);
	},

	/*gatewayRequestObjectWithGetAsync : function(uri, extraArgs, onReceived) {
		var token = this.getCookie();
		if (token == '' || token == null) {
			location.reload();
			return null;
		}
		var args = token;
		if (extraArgs != null && extraArgs != '')
			args += '&' + extraArgs;
		
		var response = function(conn, request) {
			if (conn.responseText[0]!='{') {
				alert(conn.responseText);
				conn.responseText='{"status":"error","reason":"Critical Error from Server that received message is not JSON!"}';
			}
			onReceived(Ext.decode(conn.responseText));
		};
		
		Ext.Ajax.request({
			url: uri + '?' + args,
			method: 'GET',
			//params:
			async :  false,
			success: response,
			failure: response
		});

		conn.open("GET", uriPrefix+requestTarget, false);
		try { conn.send(null); } catch (e) {}

		return this.requestObjectWithGetSync(uri, args);
	},*/

	getGatewayUsername : function() {
		var cookie = this.getCookie();
		if (cookie == '')
			return null;
		return cookie.split('&')[0].split('=')[1];
	},

	initApp : function() {
		var testCookie = this.requestObjectWithGetSync('/test_cookie', this.getCookie());
		if (testCookie.status == 'ok') {
			document.getElementById('shortcutList').style.display = 'block';
			this.startConfig = this.startConfig || this.getStartConfig();
			this.desktop = new Ext.Desktop(this);
			this.launcher = this.desktop.taskbar.startMenu;
			this.modules = this.getModules();
			if (this.modules)
				this.initModules(this.modules);
			this.init();
			Ext.EventManager.on(window, 'beforeunload', this.onUnload, this);
			this.fireEvent('ready', this);
			this.isReady = true;

			NotifyWindow.show();
			NotifyWindow.reloadNotification();
			var notifyTimerEvent = function() {
				var obj = document.getElementById('notify_local_time');
				if (obj != null)
					obj.innerHTML='TimeLocal: '+new Date();
				setTimeout(function(){notifyTimerEvent();}, 2000);
			};
			notifyTimerEvent();
			this.desktop.layout();
			
			var nextSubscribe = function(conn, request) {
				var request = true;

				if (conn!=null) {
					if (conn.responseText != null) {
						var reply = Ext.decode(conn.responseText);
						if (reply.status!='ok') {
							// alert("Critical Error from Server: " + reply.reason);
							request = false;
						} else {
							var encodes = Ext.appStorage.getItem('notify');
							Ext.appStorage.setItem('notify', encodes + Ext.encode(reply) + Ext.splitter);
							NotifyWindow.reloadNotification();
						}
					}
				}

				if (request) {
					Ext.Ajax.request({
						url: '/job/subscribe' + '?' + MyDesktop.getCookie(),
						method: 'GET',
						//params:
						async :  false,
						success: nextSubscribe,
						failure: nextSubscribe
					});
				}
			};
			
			nextSubscribe(null);
		} else {
			this.setCookie(null);
			LoginWindow.show();
			Ext.getCmp('j_userid').focus(false, 100);
		}
	},

	getModules : Ext.emptyFn,
	init : Ext.emptyFn,

	initModules : function(ms) {
		for (var i = 0, len = ms.length; i < len; i++) {
			var m = ms[i];
			this.launcher.add(m.launcher);
			m.app = this;
		}
	},

	getModule : function(name) {
		var ms = this.modules;
		for (var i = 0, len = ms.length; i < len; i++)
			if (ms[i].id == name || ms[i].appType == name)
				return ms[i];
		return '';
	},

	onReady : function(fn, scope) {
		if (!this.isReady)
			this.on('ready', fn, scope);
		else
			fn.call(scope, this);
	},

	getDesktop : function() {
		return this.desktop;
	},

	onUnload : function(e) {
		if (this.fireEvent('beforeunload', this) === false)
			e.stopEvent();
	}
});

