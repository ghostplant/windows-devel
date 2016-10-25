using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Net.Sockets;
using System.IO;
using System.Threading;
using System.Drawing.Imaging;
using System.Runtime.InteropServices;
using WindowsFormsApp.CamVideo;

namespace WindowsFormsApp
{
    public partial class FormClient : Form
    {
        TcpClient client;
        WebCamera camera;
        Stream ns;
        Thread listen;

        protected delegate void UpdateDelegate();
        byte[] head=new byte[4];

        public void UpdateCam()
        {
            try
            {
                MemoryStream ms = camera.GrabImage();
                byte[] datas = ms.GetBuffer();
                int length = datas.Length;
                for (int i = 0; i < 4; i++, length >>= 8)
                    head[i] = (byte)(length & 255);
                ns.Write(head, 0, head.Length);
                ns.Write(datas, 0, datas.Length);
                ms.Close();
            }
            catch { }
        }

        public void UpdateDesk()
        {
            try
            {
                Image image = new Bitmap(Screen.PrimaryScreen.Bounds.Size.Width, Screen.PrimaryScreen.Bounds.Size.Height);
                Graphics g = Graphics.FromImage(image);
                g.CopyFromScreen(0, 0, 0, 0, Screen.PrimaryScreen.Bounds.Size);
                g.Save();
                MemoryStream ms = new MemoryStream();
                image.Save(ms, ImageFormat.Jpeg);
                byte[] datas = ms.GetBuffer();
                int length = datas.Length;
                for (int i = 0; i < 4; i++, length >>= 8)
                    head[i] = (byte)(length & 255);
                ns.Write(head, 0, head.Length);
                ns.Write(datas, 0, datas.Length);
                ms.Close();
            }
            catch { }
        }

        void Run()
        {
            try
            {
                UpdateDelegate updateCam = new UpdateDelegate(UpdateCam);
                UpdateDelegate updateDesk = new UpdateDelegate(UpdateDesk);
                ns = client.GetStream();
                while (true)
                {
                    int cmd = ns.ReadByte();
                    if (cmd == -1)
                        throw new Exception();
                    else if (cmd == 'x')
                    {
                        MessageBox.Show("本地已经打开了一个视频监控客户端！");
                        break;
                    }
                    else if (cmd == 'c')
                        BeginInvoke(updateCam);
                    else if (cmd == 'd')
                        BeginInvoke(updateDesk);
                }
            }
            catch
            {
                try
                {
                    FormClient_FormClosed(this, null);
                }
                catch { }
            }
        }

        public FormClient(string init_ip)
        {
            try
            {
                InitializeComponent();
                tbAddress.Text = init_ip;
                this.FormBorderStyle = FormBorderStyle.FixedSingle;
                camera = new WebCamera(imageBox.Handle, 0, 0, imageBox.ClientSize.Width, imageBox.ClientSize.Height);
                camera.Start();
                btnConnect_Click(this, null);
            }
            catch { }
        }

        private void btnConnect_Click(object sender, EventArgs e)
        {
            try
            {
                client = new TcpClient(tbAddress.Text, 8000);
                listen = new Thread(Run);
                listen.Start();
                btnConnect.Enabled = false;
                tbAddress.Enabled = false;
            }
            catch
            {
                if (e!=null)
                    MessageBox.Show("连接服务器失败！");
            }
        }

        private void FormClient_FormClosed(object sender, FormClosedEventArgs e)
        {
            try
            {
                if (camera != null)
                    camera.Stop();
                if (client != null)
                    client.Close();
                if (listen != null)
                    listen.Interrupt();
            }
            catch { }
            finally
            {
                Environment.Exit(0);
            }
        }
    }

    #region WINAPI
    namespace CamVideo
    {
        public class WebCamera
        {
            private const int WS_CHILD = 0x40000000;
            private const int WS_VISIBLE = 0x10000000;
            private const int WM_CAP_START = 0x400;
            private const int WM_CAP_SET_CALLBACK_VIDEOSTREAM = WM_CAP_START + 6;
            private const int WM_CAP_SET_CALLBACK_ERROR = WM_CAP_START + 2;
            private const int WM_CAP_SET_CALLBACK_STATUSA = WM_CAP_START + 3;
            private const int WM_CAP_DRIVER_CONNECT = WM_CAP_START + 10;
            private const int WM_CAP_DRIVER_DISCONNECT = WM_CAP_START + 11;
            private const int WM_CAP_SET_SCALE = WM_CAP_START + 53;
            private const int WM_CAP_SET_PREVIEWRATE = WM_CAP_START + 52;
            private const int WM_CAP_SET_OVERLAY = WM_CAP_START + 51;
            private const int WM_CAP_SET_PREVIEW = WM_CAP_START + 50;
            private const int WM_CAP_EDIT_COPY = WM_CAP_START + 30;

            private IntPtr hWebCam, hParent;
            private int mWidth, mHeight, mLeft, mTop;

            public WebCamera(IntPtr handle, int left, int top, int width, int height)
            {
                hParent = handle;
                mWidth = width;
                mHeight = height;
                mLeft = left;
                mTop = top;
            }

            [DllImport("avicap32.dll")]
            private static extern IntPtr capCreateCaptureWindowA(byte[] lpszWindowName, int dwStyle, int x, int y, int nWidth, int nHeight, IntPtr hWndParent, int nID);
            [DllImport("user32.dll")]
            private static extern bool SendMessage(IntPtr hWnd, int wMsg, int wParam, long lParam);

            public void Start()
            {
                try
                {
                    hWebCam = capCreateCaptureWindowA(new byte[1], WS_CHILD | WS_VISIBLE, mLeft, mTop, mWidth, mHeight, hParent, 0);
                    SendMessage(hWebCam, WM_CAP_DRIVER_CONNECT, 0, 0);
                    SendMessage(hWebCam, WM_CAP_SET_PREVIEWRATE, 20, 0);
                    SendMessage(hWebCam, WM_CAP_SET_PREVIEW, 1, 0);
                    SendMessage(hWebCam, WM_CAP_SET_SCALE, 1, 0);
                }
                catch { }
            }

            public void Stop()
            {
                SendMessage(hWebCam, WM_CAP_DRIVER_DISCONNECT, 0, 0);
            }

            public MemoryStream GrabImage()
            {
                try
                {
                    SendMessage(hWebCam, WM_CAP_EDIT_COPY, 0, 0);
                    Image image = Clipboard.GetImage();
                    while (image == null)
                    {
                        Thread.Sleep(50);
                        image = Clipboard.GetImage();
                    }
                    MemoryStream ms = new MemoryStream();
                    image.Save(ms, ImageFormat.Jpeg);
                    return ms;
                }
                catch { return new MemoryStream(); }
            }
        }
    }
    #endregion
}
