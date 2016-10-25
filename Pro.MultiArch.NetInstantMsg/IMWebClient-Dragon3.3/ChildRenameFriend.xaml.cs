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
    public partial class ChildRenameFriend : ChildWindow
    {
        MainPage mp;
        string fid;

        public ChildRenameFriend(MainPage mp, string fid)
        {
            this.mp = mp;
            this.fid = fid;
            InitializeComponent();
            textName.Text = fid;
            string text=mp.getPersonByPid[fid].Text;
            textBox1.Text = text.Substring(0, text.IndexOf('('));
        }

        private void OKButton_Click(object sender, RoutedEventArgs e)
        {
            if (textBox1.Text.Length > 0 && textBox1.Text.Length <= 20 && textBox1.Text.IndexOf('\'') == -1 && textBox1.Text.IndexOf('(') == -1 && textBox1.Text.IndexOf(')') == -1)
            {
                mp.SendMessage("renfrd", new string[] { textName.Text, textBox1.Text });
                this.DialogResult = true;
            }
            else
                MessageWindow.ShowMessage("对不起，好友名称长度范围1-20且不允许含有非法字符！");
        }

        private void CancelButton_Click(object sender, RoutedEventArgs e)
        {
            this.DialogResult = false;
        }
    }
}

