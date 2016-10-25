using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Net.Sockets;
using System.Net;

namespace ServerFormsApp
{
    public partial class FormMonitor : Form
    {
        IPAddress address;
        FormServer server;
        NetworkStream ns;
        public byte target;

        public FormMonitor(IPAddress address,FormServer server,bool webcam)
        {
            InitializeComponent();
            this.address = address;
            this.server = server;
            this.target = (byte)(webcam ? 'c' : 'd');

            this.Text = "来自 " + address + " 的远程视频";
            server.getFrame[address] = this;
            ns = server.getStream[address];
            ns.WriteByte(target);
            FormMonitor_Resize(null, null);
        }

        public void setImage(Image image)
        {
            pictureBox.Image = image;
        }

        private void FormMonitor_FormClosed(object sender, FormClosedEventArgs e)
        {
            server.getFrame.Remove(address);
        }

        private void FormMonitor_Resize(object sender, EventArgs e)
        {
            pictureBox.Size = this.ClientSize;
        }
    }
}
