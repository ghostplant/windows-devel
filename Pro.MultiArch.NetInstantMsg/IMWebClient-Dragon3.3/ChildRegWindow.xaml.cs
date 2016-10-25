using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Shapes;

namespace IMWebClient
{
    public partial class ChildRegWindow : ChildWindow
    {
        MainPage mp;

        public ChildRegWindow(MainPage mp)
        {
            InitializeComponent();
            this.mp = mp;
        }

        private void OKButton_Click(object sender, RoutedEventArgs e)
        {
            if (tbIdentity.Text.Length > 0 && tbIdentity.Text.Length < 21 && !tbIdentity.Text.Contains('\'') && !tbIdentity.Text.Contains('(') && !tbIdentity.Text.Contains(')'))
            {
                if (tbNickname.Text.Length > 0 && tbNickname.Text.Length < 21 && !tbNickname.Text.Contains('\'') && !tbNickname.Text.Contains('(') && !tbNickname.Text.Contains(')'))
                {
                    if (tbPassword.Password.Length > 0 && tbPassword.Password.Length < 21 && tbPassword.Password.Equals(tbConfirm.Password))
                    {
                        mp.SendMessage("reg", new string[] { tbIdentity.Text, tbNickname.Text, rbMale.IsChecked.ToString(), tbPassword.Password });
                        this.DialogResult = true;
                    }
                    else
                        MessageWindow.ShowMessage("请您确保密码不为空、并且两次密码输入一致。");
                }
                else
                    MessageWindow.ShowMessage("请您填写合法的信息，信息不能为空、且不能含单引号、括号等字符。");
            }
            else
                MessageWindow.ShowMessage("请您填写合法的信息，信息不能为空、且不能含单引号、括号等字符。");
        }

        private void CancelButton_Click(object sender, RoutedEventArgs e)
        {
            this.DialogResult = false;
        }
    }
}

