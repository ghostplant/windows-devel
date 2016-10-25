var userSuffix = '@sei.pku.edu.cn';

LoginWindow = new Ext.Window({
	el : 'login-window',
	layout : 'fit',
	title : 'Unias Login - Welcome to Unias',
	width : 350,
	height : 205,
	closable : false,
	resizable : false,
	closeAction : 'hide',
	items : new Ext.FormPanel( {
		id : 'login-form',
		//renderTo : Ext.getBody(),
		labelWidth : 55,
		frame : false,
		bodyStyle : 'background-color: #DFE8F6; padding-top: 25px; padding-left: 20px; border: 0px solid;',
		defaults : {
			width : 230,
			anchor : '88%',
			allowBlank : false,
			selectOnFocus : true,
			msgTarget : 'side'
		},
		defaultType : 'textfield',
		method : 'POST',
		bodyBorder : false,
		border : false,
		items : [ {
			xtype : 'combo',
			store : new Ext.data.Store(),
			id : 'j_userid',
			name : 'j_userid',
			fieldLabel : 'User',
			displayField : 'userid',
			valueField : 'userid',
			typeAhead : true,
			mode : 'local',
			triggerAction : 'all',
			selectOnFocus : true,
			allowBlank : false,
			value : userSuffix,
			//submit : function() {},
			listeners: {
				specialkey: function(field,e) {
					if (e.getKey()==Ext.EventObject.ENTER)
						Ext.getCmp('submit_login').handler();
				}
			}
		}, {
			id : 'j_password',
			name : 'j_password',
			fieldLabel : 'Password',
			inputType : 'password',
			listeners: {
				specialkey: function(field,e) {
					if (e.getKey()==Ext.EventObject.ENTER)
						Ext.getCmp('submit_login').handler();
				}
			}
		} ],
		buttons : [ {
					id : 'submit_login',
					text : 'Login',
					inputType : 'submit',
					handler : function() {
						var pass = document.getElementById('j_password').value;
						var userid = document.getElementById('j_userid').value.replace(userSuffix, '');
						var messages = null;
						if (pass.length == 0 || userid.length == 0)
							messages = "Please fill in the blocked!";
						else {
							var reply = MyDesktop.requestObjectWithGetSync('/account/login','user='+ userid+ '&passwd='+ pass);
							if (reply.status == 'ok') {
								var cookie = 'user='+ userid+ '&token='+ reply.token;
								MyDesktop.setCookie(cookie);
								Ext.appStorage.setItem('level', parseInt(reply.level));
								Ext.appStorage.setItem('notify', '');
								location.reload();
							} else
								messages = "Sorry, authentication failed, please try again!";
								//messages = "Authentication failed!<br/>Reason: "+ reply.reason;
						}
						if (messages != null)
							MyDesktop.messageBox(messages, function() {
								Ext.getCmp('j_userid').focus(false, 100);
							});
					}
				}, {
					text : 'Reset',
					handler : function() {
						//this.form.reset();
						document.getElementById('j_password').value='';
						document.getElementById('j_userid').value='';
						Ext.getCmp('j_userid').focus(false, 100);
					}
				}, {
					text : 'Contact Us!',
					handler : function() {
						window.open('http://www.unias.org/trac/unias/contact');
					}
				} ]
	}),
	iconCls : 'bogus',
	bbar : [ {
		iconCls : 'user-kid',
		text : 'Guest to view Unias.Org',
		handler : function() {
			window.open('http://www.unias.org');
		}
	} ]
});
