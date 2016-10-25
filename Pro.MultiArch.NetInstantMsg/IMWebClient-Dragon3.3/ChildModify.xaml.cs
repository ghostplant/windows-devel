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
    public partial class ChildModify : ChildWindow
    {
        MainPage mp;

        public ChildModify(MainPage mp)
        {
            this.mp = mp;
            InitializeComponent();
            textBox1.Text = mp.selfName;
            if (mp.NickName.Content.ToString().EndsWith("♀"))
                radioButton2.IsChecked = true;
            else
                radioButton1.IsChecked = true;
        }

        private void OKButton_Click(object sender, RoutedEventArgs e)
        {
            this.DialogResult = true;
        }

        private void CancelButton_Click(object sender, RoutedEventArgs e)
        {
            this.DialogResult = false;
        }

        private void button1_Click(object sender, RoutedEventArgs e)
        {
            if (textBox1.Text.Length > 0 && textBox1.Text.Length <= 20 && textBox1.Text.IndexOf('\'') == -1 && textBox1.Text.IndexOf('(') == -1 && textBox1.Text.IndexOf(')') == -1)
            {
                mp.SendMessage("rennick", new string[] { textBox1.Text });
                mp.SendMessage("rengender", new string[] { radioButton1.IsChecked.ToString() });
                this.DialogResult = true;
            }
            else
                MessageWindow.ShowMessage("对不起，账号名称长度范围1-20且不允许含有非法字符！");
        }
    }
}

