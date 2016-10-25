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
    public partial class ChildAddFriend : ChildWindow
    {
        MainPage mp;
        string gid;

        public ChildAddFriend(MainPage mp,string groupid)
        {
            this.mp = mp;
            gid = groupid;
            InitializeComponent();
        }

        private void OKButton_Click(object sender, RoutedEventArgs e)
        {
            if (textName.Text.Length > 0 && textName.Text.Length <= 20 && textName.Text.IndexOf('\'') == -1 && textName.Text.IndexOf('(') == -1 && textName.Text.IndexOf(')') == -1)
            {
                if (textBox1.Text.Length > 0 && textBox1.Text.Length <= 20 && textBox1.Text.IndexOf('\'') == -1 && textBox1.Text.IndexOf('(') == -1 && textBox1.Text.IndexOf(')') == -1)
                {
                    if (!textName.Text.Equals(mp.selfId))
                    {
                        string memo = textBox1.Text;
                        if (memo.Equals("[对方默认昵称]"))
                            memo = "\n";
                        mp.SendMessage("addfrd", new string[] { textName.Text, gid, memo });
                        this.DialogResult = true;
                    } else
                        MessageWindow.ShowMessage("对不起，不允许自己给自己添加好友！");
                }
                else
                    MessageWindow.ShowMessage("对不起，好友备注长度范围1-20且不允许含有非法字符！");
            }
            else
                MessageWindow.ShowMessage("对不起，好友账号长度范围1-20且不允许含有非法字符！");
        }

        private void CancelButton_Click(object sender, RoutedEventArgs e)
        {
            this.DialogResult = false;
        }

        private void textBox1_MouseEnter(object sender, MouseEventArgs e)
        {
            if (textBox1.Text.Equals("[对方默认昵称]"))
                textBox1.Text = "";
        }

        private void textBox1_MouseLeave(object sender, MouseEventArgs e)
        {
            if (textBox1.Text.Equals(""))
                textBox1.Text = "[对方默认昵称]";
        }
    }
}

