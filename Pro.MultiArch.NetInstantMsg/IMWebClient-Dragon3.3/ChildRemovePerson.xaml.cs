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
    public partial class ChildRemovePerson : ChildWindow
    {
        MainPage mp;
        string fid;

        public ChildRemovePerson(MainPage mp, string fid)
        {
            this.mp = mp;
            this.fid = fid;
            InitializeComponent();
        }

        private void OKButton_Click(object sender, RoutedEventArgs e)
        {
            mp.SendMessage("rmfrd", new String[] { fid });
            this.DialogResult = true;
        }

        private void CancelButton_Click(object sender, RoutedEventArgs e)
        {
            this.DialogResult = false;
        }
    }
}

