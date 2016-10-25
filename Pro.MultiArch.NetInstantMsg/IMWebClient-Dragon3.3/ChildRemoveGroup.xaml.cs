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
    public partial class ChildRemoveGroup : ChildWindow
    {
        MainPage mp;
        string gid;

        public ChildRemoveGroup(MainPage mp,string gid)
        {
            this.mp = mp;
            this.gid = gid;
            InitializeComponent();
        }

        private void OKButton_Click(object sender, RoutedEventArgs e)
        {
            mp.SendMessage("rmgroup", new String[] { gid });
            this.DialogResult = true;
        }

        private void CancelButton_Click(object sender, RoutedEventArgs e)
        {
            this.DialogResult = false;
        }
    }
}

