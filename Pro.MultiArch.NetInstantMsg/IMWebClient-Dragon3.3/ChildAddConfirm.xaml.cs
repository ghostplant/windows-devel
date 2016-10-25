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
    public partial class ChildAddConfirm : ChildWindow
    {
        MainPage mp;
        string[] datas;

        public ChildAddConfirm(MainPage mp, string[] datas)
        {
            this.mp = mp;
            this.datas = datas;
            InitializeComponent();
            textName.Text = datas[0];
            textBox1.Text = datas[1];

            foreach (var gid in mp.getGroupByGid)
            {
                TextBlock tb=new TextBlock();
                tb.DataContext=gid.Key;
                tb.Text = ((TextBlock)gid.Value.Header).Text;
                comboBox1.Items.Add(tb);
            }
            comboBox1.SelectedIndex = 0;
        }

        private void OKButton_Click(object sender, RoutedEventArgs e)
        {
            if (textBox1.Text.Length > 0 && textBox1.Text.Length <= 20 && textBox1.Text.IndexOf('\'') == -1 && textBox1.Text.IndexOf('(') == -1 && textBox1.Text.IndexOf(')') == -1)
            {
                mp.SendMessage("acptcnf", new string[] { textName.Text, datas[2], datas[3], (string)((TextBlock)comboBox1.SelectedItem).DataContext, textBox1.Text });
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

