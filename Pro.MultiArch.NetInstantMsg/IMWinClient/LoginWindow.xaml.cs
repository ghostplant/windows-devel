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
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Net;
using System.Net.Sockets;
using System.ComponentModel;
using System.Media;
using System.Text;

using CommonLibrary;

namespace IMWinClient
{
    public partial class LoginWindow : Window
    {
        public bool loggedIn = false;

        #region
        MainWindow mw;

        public LoginWindow()
        {            
            InitializeComponent();
            userid.Focus();
        }

        private void login_Click(object sender, RoutedEventArgs e)
        {
            if (userid.Text.Length == 0)
            {
                MessageBox.Show("请您填写完整用户ID和密码！");
                return;
            }
            if (mw == null)
                mw = new MainWindow(userid.Text, this);
            if (mw.mq == null)
            {
                mw.mq = new MsgQueue(mw);
                msg.Content = "连接中……";
                if (!mw.mq.ConnectAsync())
                    MessageBox.Show("对不起，暂时无法连接服务器！");
            }
            msg.Content = "正在登录……";
            mw.mq.SendAsync(MsgPackager.Packager(new string[] { "log", userid.Text, password.Password }));
        }
        #endregion

        private void Window_Closed(object sender, EventArgs e)
        {
            if (!loggedIn)
                Environment.Exit(0);
        }

    }
}
