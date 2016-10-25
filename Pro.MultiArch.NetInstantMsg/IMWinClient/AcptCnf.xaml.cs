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

namespace IMWinClient
{
    /// <summary>
    /// AcptCnf.xaml 的交互逻辑
    /// </summary>
    public partial class AcptCnf : Window
    {
        MainWindow mw;
        String data0, data2, data3;

        public AcptCnf(MainWindow mw, String data0, String data2, String data3)
        {
            InitializeComponent();
            this.mw = mw;
            this.data0 = data0;
            this.data2 = data2;
            this.data3 = data3;
            this.nickName.Text = data0;
            foreach (KeyValuePair<string, TreeViewItem> myItem in this.mw.getGroupItemById)
            {
                TextBlock TBItem = new TextBlock();
                TBItem.Text = (String)(myItem.Value.Header);
                //MessageBox.Show(TBItem.Text);
                TBItem.DataContext = myItem.Key;
                GroupListComboBox.Items.Add(TBItem);
            }
            GroupListComboBox.SelectedItem = GroupListComboBox.Items.GetItemAt(0);
        }
        
        private void confirmButton_Click(object sender, RoutedEventArgs e)
        {
            if (this.nickName.Text.Length > 0)
            {
                if (this.nickName.Text.Length > 20)
                {
                    MessageBox.Show("好友昵称命名过长，请限制在20个字符之内！");
                }
                else
                {
                    TextBlock TBItem = (TextBlock)(GroupListComboBox.SelectedItem);
                    String key = (String)(TBItem.DataContext);
                    mw.SendMessage("acptcnf", new String[] { data0, data2, data3, key, this.nickName.Text });
                    this.Close();
                }
            }
            else
            {
                MessageBox.Show("昵称不能为空！");
            }
        }
    }
}
