using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Net;
using System.Net.Sockets;
using System.Diagnostics;

namespace WindowsFormsApp
{
    static class Program
    {
        /// <summary>
        /// 应用程序的主入口点。
        /// </summary>
        [STAThread]
        static void Main()
        {
            TcpListener server = new TcpListener(IPAddress.Any, 8001);
            try
            {
                server.Start();
                while (true)
                {
                    TcpClient client = server.AcceptTcpClient();
                    Process.Start("ClientFormsApp.exe",((IPEndPoint)client.Client.RemoteEndPoint).Address.ToString());
                    client.Close();
                }
            }
            catch (Exception ex)
            {
                server.Stop();
                MessageBox.Show("后台控制已经停止，原因：" + ex.Message);
            }
        }
    }
}
