/*
 * 创建时间：2012.3.29
 * 修改时间：2012.4.9
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
    /// ChangeData.xaml 的交互逻辑
    /// </summary>
    public partial class ChangeData : Window
    {
        MainWindow mw;
        public ChangeData(MainWindow mw)
        {
            InitializeComponent();
            this.mw = mw;
            if (((String)(mw.Gender.DataContext)).Equals("♂"))
            {
                this.Male.IsChecked = true;
            }
            else
            {
                this.Female.IsChecked = true;
            }
            this.nick.Text = mw.NickName.Text;
            this.nick.SelectAll();
        }

        private void confirmButton_Click(object sender, RoutedEventArgs e)
        {
            if (this.nick.Text.Length > 0)
            {
                if (this.nick.Text.Length < 20)
                {
                    mw.SendMessage("rennick", new String[] { nick.Text });
                    if (Male.IsChecked == true)
                    {
                        mw.SendMessage("rengender", new String[] { "True" });
                    }
                    else
                    {
                        mw.SendMessage("rengender", new String[] { "False" });
                    }
                    this.Close();
                }
                else
                {
                    MessageBox.Show("昵称设置过长，请限制在20个字符之内！");
                }
            }
            else
            {
                MessageBox.Show("昵称不能为空！");
            }
        }

        private void cancelButton_Click(object sender, RoutedEventArgs e)
        {
            this.Close();
        }
    }
}
