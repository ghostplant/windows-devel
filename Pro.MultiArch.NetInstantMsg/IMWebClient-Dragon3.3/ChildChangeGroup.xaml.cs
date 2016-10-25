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
    public partial class ChildChangeGroup : ChildWindow
    {
        MainPage mp;
        string fid;

        public ChildChangeGroup(MainPage mp, string fid)
        {
            this.mp = mp;
            this.fid = fid;
            InitializeComponent();
            textName.Text = fid;
            textBox1.Text = mp.getPersonByPid[fid].Text;

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
            string memo = textBox1.Text.Substring(0,textBox1.Text.IndexOf('('));
            mp.SendMessage("xchgfrd", new string[] { fid, (string)((TextBlock)comboBox1.SelectedItem).DataContext, memo });
            this.DialogResult = true;
        }

        private void CancelButton_Click(object sender, RoutedEventArgs e)
        {
            this.DialogResult = false;
        }
    }
}

