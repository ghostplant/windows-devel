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
    public partial class ChildRenameGroup : ChildWindow
    {
        MainPage mp;
        string gid;

        public ChildRenameGroup(MainPage mp,string gid)
        {
            this.mp = mp;
            this.gid = gid;
            InitializeComponent();
            textName.Text = ((TextBlock)mp.getGroupByGid[gid].Header).Text;
        }

        private void OKButton_Click(object sender, RoutedEventArgs e)
        {
            if (textName.Text.Length > 0 && textName.Text.Length <= 20 && textName.Text.IndexOf('\'') == -1)
            {
                mp.SendMessage("rengroup", new string[] { gid, textName.Text });
                this.DialogResult = true;
            }
            else
                MessageWindow.ShowMessage("对不起，分组名称长度范围1-20且不允许含有非法字符！");
        }

        private void CancelButton_Click(object sender, RoutedEventArgs e)
        {
            this.DialogResult = false;
        }
    }
}

