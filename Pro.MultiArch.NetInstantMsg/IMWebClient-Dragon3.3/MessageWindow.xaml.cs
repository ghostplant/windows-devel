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
    public partial class MessageWindow : ChildWindow
    {

        public MessageWindow()
        {
            InitializeComponent();
        }

        public static void ShowMessage(String msg)
        {
            MessageWindow cw = new MessageWindow();
            cw.txtMessage.Text = msg;
            cw.Show();
        }

        private void OKButton_Click(object sender, RoutedEventArgs e)
        {
            this.DialogResult = true;
        }

    }
}

