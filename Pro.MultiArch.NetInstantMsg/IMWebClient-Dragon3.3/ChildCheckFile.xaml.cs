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
    public partial class ChildCheckFile : ChildWindow
    {
        MainPage mp;
        string fid, fname, fileInfo;

        public ChildCheckFile(MainPage mp, string []data)
        {
            this.mp = mp;
            fid = data[0];
            fname = data[1];
            fileInfo = data[2];
            InitializeComponent();
            string fnick = mp.getPersonByPid[fid].Text;
            information.Text = "您的好友'" + fnick.Substring(0, fnick.IndexOf('(')) + "'请求向您发送文件'" + fname + "'，是否同意接收？";
        }

        private void OKButton_Click(object sender, RoutedEventArgs e)
        {
            SaveFileDialog sfd = new SaveFileDialog();
            if (fname.Contains('.')) {
                string ext=fname.Substring(fname.LastIndexOf('.')+1);
                sfd.DefaultExt = ext;
                sfd.Filter = ext + " 文件(*." + ext + ")|*." + ext + "||";
            }
            else
                sfd.Filter = "所有文件(*.*)|*.*||";
            if ((bool)sfd.ShowDialog())
            {
                byte[] datas = Convert.FromBase64String(fileInfo);
                Stream stream = sfd.OpenFile();
                stream.Write(datas, 0, datas.Length);
                stream.Close();
                mp.SendMessage("onlnaccpt", new string[] { fid, mp.selfId, fname });
            }
            else
                mp.SendMessage("onlnrejct", new string[] { fid, mp.selfId, fname });
            this.DialogResult = true;
        }

        private void CancelButton_Click(object sender, RoutedEventArgs e)
        {
            mp.SendMessage("onlnrejct", new string[] { fid, mp.selfId, fname });
            this.DialogResult = false;
        }
    }
}

