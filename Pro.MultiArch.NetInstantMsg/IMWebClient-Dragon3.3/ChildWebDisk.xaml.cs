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
using System.IO;

namespace IMWebClient
{
    public partial class ChildWebDisk : ChildWindow
    {
        MainPage mp;

        public ChildWebDisk(MainPage mp)
        {
            this.mp = mp;
            InitializeComponent();
            mp.webDisk = this;
        }

        private void ChildWindow_Closed(object sender, EventArgs e)
        {
            mp.webDisk = null;
        }

        private void fileUpload_Click(object sender, RoutedEventArgs e)
        {
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.Filter = "所有文件 (*.*) |*.*||";
            if (ofd.ShowDialog().Value)
            {
                FileStream fstream = ofd.File.OpenRead();
                byte[] data = new byte[fstream.Length];
                fstream.Read(data, 0, (int)fstream.Length);
                fstream.Close();
                string text = Convert.ToBase64String(data);
                mp.SendMessage("upload", new string[] { ofd.File.Name, text });
                fileUpload.IsEnabled = false;
            }
        }
    }
}

