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

namespace 语法分析器
{
    public partial class MessagePage : ChildWindow
    {
        public MessagePage(string information)
        {
            InitializeComponent();
            lbMessage.Text = information;
        }

        private void OKButton_Click(object sender, RoutedEventArgs e)
        {
            this.DialogResult = true;
        }

        public static void Show(string information)
        {
            new MessagePage(information).Show();
        }
    }
}

