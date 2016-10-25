/*
 * 创建时间：2012.3.29
 * 修改时间：2012.4.3
 * 负责人：HJWAJ
 */

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using CommonLibrary;
using System.Net.Sockets;
using System.Windows.Navigation;

namespace IMWinClient
{
    public partial class AddFriend : Window
    {
        MainWindow mw;
        private void confirmButton_Click(object sender, RoutedEventArgs e)
        {
            if (friendID.Text.Length > 0 && myNick.Text.Length > 0)
            {
                if (myNick.Text.Length > 20)
                {
                    MessageBox.Show("好友昵称命名过长，请限制在20个字符之内！");
                }
                else
                {
                    mw.SendMessage("addfrd", new String[] { friendID.Text, mw.remGroupKey, myNick.Text });
                    this.Close();
                }
            }
            else
            {
                MessageBox.Show("ID及昵称不能为空！");
            }
        }

        private void cancelButton_Click(object sender, RoutedEventArgs e)
        {
            this.Close();
        }

        public AddFriend(MainWindow mw)
        {
            InitializeComponent();
            this.mw = mw;
            friendID.Focus();
        }
    }
}
