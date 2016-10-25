/*
 * 创建时间：2012.3.29
 * 修改时间：2012.4.1
 * 负责人：崔嵬
 */

using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Shapes;
using System.Windows.Navigation;
using System.Net;
using System.Net.Sockets;
using CommonLibrary;
using System.Windows.Browser;

namespace IMWebClient
{
    public partial class LoginPage : Page
    {
        MainPage mp;
        #region

        public LoginPage()
        {
            InitializeComponent();
            mp = new MainPage(this);
            mp.mq = new MsgQueue(mp);
            if (!mp.mq.ConnectAsync())
                MessageWindow.ShowMessage("对不起，暂时无法连接服务器！");
        }

        // 当用户导航到此页面时执行。
        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
        }
        
        private void register_Click(object sender, RoutedEventArgs e)
        {
            new ChildRegWindow(mp).Show();
        }

        public void login_Click(object sender, RoutedEventArgs e)
        {
            if (userid.Text.Length == 0 || password.Password.Length == 0)
            {
                MessageWindow.ShowMessage("请您填写完整用户ID和密码！");
                return;
            }
            mp.selfId = userid.Text;
            msg.Content = "正在登录……";
            mp.mq.SendAsync(MsgPackager.Packager(new string[] { "log", userid.Text, password.Password }));
        }
        #endregion

        private void password_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Enter)
                login_Click(sender, null);
        }
    }
}
