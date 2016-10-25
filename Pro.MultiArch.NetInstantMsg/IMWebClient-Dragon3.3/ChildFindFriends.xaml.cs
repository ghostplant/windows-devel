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
    public partial class ChildFindFriends : ChildWindow
    {
        MainPage mp;

        public ChildFindFriends(MainPage mp)
        {
            this.mp = mp;
            mp.cff = this;
            InitializeComponent();
            foreach (var pair in mp.getGroupByGid)
            {
                TextBlock tb = new TextBlock();
                tb.DataContext = pair.Key;
                tb.Text = ((TextBlock)pair.Value.Header).Text;
                comboBox1.Items.Add(tb);
            }
            comboBox1.SelectedIndex = 0;
        }

        private void OKButton_Click(object sender, RoutedEventArgs e)
        {
            if (OKButton.Content.Equals("查找"))
            {
                if (textBox1.Text.Length == 0)
                    MessageWindow.ShowMessage("请您输入要查找的好友部分描述信息来进行模糊查找！");
                else if (!textBox1.Text.Contains("\'"))
                {
                    listBox1.Items.Clear();
                    string search = textBox1.Text.Replace('%', '!').Replace('*', '%');
                    mp.SendMessage("find", new string[] { search });
                    OKButton.Content = "添加";
                    textBox1.IsEnabled = false;
                }
                else
                    MessageWindow.ShowMessage("请您不要输入含有非法字符的查询信息！");
            }
            else
            {
                if (listBox1.SelectedItem == null)
                    MessageWindow.ShowMessage("请您选中一个要添加的好友项目！");
                else
                {
                    string cut=listBox1.SelectedItem.ToString().Substring(7);
                    string addid = cut.Substring(0, cut.IndexOf('('));
                    mp.SendMessage("addfrd", new string[] { addid, (string)((TextBlock)comboBox1.SelectedItem).DataContext, "\n" });
                    this.Close();
                }
            }
        }

        private void CancelButton_Click(object sender, RoutedEventArgs e)
        {
            this.Close();
        }

        private void ChildWindow_Closed(object sender, EventArgs e)
        {
            mp.cff = null;
        }
    }
}

