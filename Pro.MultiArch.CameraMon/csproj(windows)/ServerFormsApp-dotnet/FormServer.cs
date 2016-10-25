using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Net;
using System.Net.Sockets;
using System.Threading;
using System.IO;

namespace ServerFormsApp
{
    public partial class FormServer : Form
    {
        public Dictionary<IPAddress, NetworkStream> getStream = new Dictionary<IPAddress, NetworkStream>();
        public Dictionary<IPAddress, FormMonitor> getFrame = new Dictionary<IPAddress, FormMonitor>();
        TcpListener server;

        MenuItem mnuOper, mnuOperExit, menuCamera, menuDesktop, menuEliminate;

        public FormServer()
        {
            InitializeComponent();
            this.FormBorderStyle = FormBorderStyle.FixedSingle;
            new Thread(Listening).Start();

            Menu = new MainMenu();
            mnuOper = new MenuItem("操作(&O)", new EventHandler(popupMenuAction));
            mnuOperExit = new MenuItem("退出(&X)", new EventHandler(popupMenuAction));
            menuCamera = new MenuItem("监控摄像头(&C)", new EventHandler(popupMenuAction));
            menuDesktop = new MenuItem("监控桌面(&D)", new EventHandler(popupMenuAction));
            menuEliminate = new MenuItem("逐出列表(&E)", new EventHandler(popupMenuAction));

            mnuOper.MenuItems.Add(mnuOperExit);
            Menu.MenuItems.Add(mnuOper);
        }

        void Listening()
        {
            server = new TcpListener(IPAddress.Any, 8000);
            server.Start();
            while (true)
            {
                ClientThread ct = new ClientThread(this,server.AcceptTcpClient());
                new Thread(ct.Run).Start();
            }
        }

        public ListBox getListBox()
        {
            return lbClient;
        }

        public void triggerRecover(IPAddress address)
        {
            if (address.Equals(IPAddress.Parse(tbClientIP.Text)))
            {
                tbClientIP.Enabled = true;
                btStart.Enabled = true;
            }
        }

        private void FormServer_FormClosed(object sender, FormClosedEventArgs e)
        {
            Environment.Exit(0);
        }

        private void showClientVideo(string webcam)
        {
            IPAddress address = (IPAddress)lbClient.SelectedItem;
            if (address == null)
                return;
            if (!getFrame.ContainsKey(address))
                new FormMonitor(address, this, webcam.Equals("cam")).Show();
            else
                getFrame[address].Focus();
        }

        private void eliminateClient()
        {
            IPAddress address=(IPAddress)lbClient.SelectedItem;
            if (getFrame.ContainsKey(address)) {
                getFrame[address].Close();
                getFrame.Remove(address);
            }
            if (getStream.ContainsKey(address))
            {
                getStream[address].Close();
                getStream.Remove(address);
            }
        }

        private void popupMenuAction(object sender, EventArgs e)
        {
            if (menuCamera == sender)
                showClientVideo("cam");
            else if (menuDesktop == sender)
                showClientVideo("des");
            else if (menuEliminate == sender)
                eliminateClient();
        }

        private void btStart_Click(object sender, EventArgs e)
        {
            IPAddress address = IPAddress.Parse(tbClientIP.Text);
            if (lbClient.Items.Contains(address))
            {
                MessageBox.Show("该客户端已经处于启动状态！");
                return;
            }
            tbClientIP.Enabled = false;
            btStart.Enabled = false;
            try
            {
                TcpClient client = new TcpClient(address.ToString(), 8001);
                NetworkStream ns = client.GetStream();
                ns.Close();
                client.Close();
            }
            catch
            {
                MessageBox.Show("不能正确连接到客户端 '" + tbClientIP.Text + "'！");
                tbClientIP.Enabled = true;
                btStart.Enabled = true;
            }
        }

        private void btRefresh_Click(object sender, EventArgs e)
        {
            List<string> history=MySQLServer.Query();
            lbHistory.Items.Clear();
            foreach (string item in history)
                lbHistory.Items.Add(item);
        }

        private void lbClient_MouseUp(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Right && lbClient.SelectedItem != null)
            {
                ContextMenu cm = new ContextMenu();
                cm.MenuItems.Add(menuCamera);
                cm.MenuItems.Add(menuDesktop);
                cm.MenuItems.Add(menuEliminate);
                cm.Show(this, new Point(e.X + 30, e.Y + 60));
            }
        }
    }

    class ClientThread
    {
        public NetworkStream ns;
        TcpClient client;
        FormServer server;

        public ClientThread(FormServer server,TcpClient client)
        {
            this.server = server;
            this.client = client;
        }

        public void Run()
        {
            IPAddress address=((IPEndPoint)client.Client.RemoteEndPoint).Address;

            if (server.getListBox().Items.Contains(address))
            {
                client.GetStream().WriteByte((byte)'x');
                client.GetStream().Close();
                client.Close();
                return;
            }
            server.getListBox().Items.Add(address);
            server.triggerRecover(address);

            MySQLServer.Insert("Windows: client from [ip:" + address + "] booted at [" + DateTime.Now + "]."); // 原子函数

            try
            {
                ns=client.GetStream();
                server.getStream[address] = ns;
                const int MAX_LEN = 1 << 20;
                byte[] jpg = new byte[MAX_LEN], head = new byte[4];
                while (true)
                {
                    int c = ns.Read(head, 0, 4);
                    if (c == 0)
                    {
                        server.getFrame.ContainsKey(address);
                        FormMonitor fm = server.getFrame[address];
                        server.getFrame.Remove(address);
                        fm.Close();
                        break;
                    }
                    else
                    {
                        int offset, rcv = -1;
                        for (offset = c; rcv != 0 && offset != 4; offset += rcv)
                            rcv = ns.Read(head, offset, 4 - offset);
                        int length = (head[3] << 24) | (head[2] << 16) | (head[1] << 8) | head[0];

                        for (offset = 0; rcv!=0 && offset != length; offset += rcv)
                            rcv = ns.Read(jpg, offset, length - offset);
                        if (server.getFrame.ContainsKey(address))
                        {
                            FormMonitor frame = server.getFrame[address];
                            MemoryStream ms = new MemoryStream();
                            ms.Write(jpg, 0, length);
                            Image image = Image.FromStream(ms);
                            frame.setImage(image);
                            Thread.Sleep(40);
                            ns.WriteByte(frame.target);
                        }
                    }
                }
            }
            catch
            {
            }
            MySQLServer.Insert("Windows: client from [ip:" + address + "] exited at [" + DateTime.Now + "]."); // 原子函数
            server.getStream.Remove(address);
            server.getListBox().Items.Remove(address);
        }
    }

}
