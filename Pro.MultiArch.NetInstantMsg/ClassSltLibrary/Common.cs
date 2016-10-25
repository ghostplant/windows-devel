/*
 * 创建时间：2012.3.29
 * 修改时间：2012.4.1
 * 负责人：崔嵬
 */

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net;
using System.Net.Sockets;
using System.IO;
using System.Windows;
using System.Windows.Controls;

using System.Threading;

namespace CommonLibrary
{
    public interface MessageTranslatable
    {
        void TranslateMessage(string key, string[] datas);
    }

    public class MsgPackager
    {
        public static string[] Splitter(string str)
        {
            string[] basArr = str.Split(new char[] { '|' });
            string[] realArr = new string[basArr.Length];
            for (int i = 0; i < basArr.Length; i++)
                realArr[i] = Base64Change.Base64ToString(basArr[i]);
            return realArr;
        }

        public static string Packager(string[] str)
        {
            StringBuilder sb = new StringBuilder();
            sb.Append(Base64Change.StringToBase64(str[0]));
            for (int i = 1; i < str.Length; i++)
            {
                sb.Append('|');
                sb.Append(Base64Change.StringToBase64(str[i]));
            }
            sb.Append(';');
            return sb.ToString();
        }
    }

    public class MsgQueue
    {
        public const int plength = 1 << 23;
        public bool interrupt;
        public MessageTranslatable mt;
        public Socket socket;

        private List<byte> rcvbuf;
        private byte[] buffer;

        public MsgQueue(MessageTranslatable mt)
        {
            this.mt = mt;

            rcvbuf = new List<byte>();
            buffer = new byte[plength];
            interrupt = false;
        }

        public void ReceiveListen()
        {
            SocketAsyncEventArgs args = new SocketAsyncEventArgs();
            args.RemoteEndPoint = socket.RemoteEndPoint;
            args.SetBuffer(buffer, 0, plength);
            args.Completed += OnActionReceive;
            args.SetBuffer(0, buffer.Length);
            socket.ReceiveAsync(args);
        }

        private void OnActionReceive(object sender, SocketAsyncEventArgs e)
        {
            for (int i = 0; i < e.BytesTransferred; i++)
            {
                if (e.Buffer[i] == ';')
                {
                    StringBuilder sb = new StringBuilder();
                    foreach (byte b in rcvbuf)
                        sb.Append((char)b);
                    string[] str = MsgPackager.Splitter(sb.ToString()), datas = new string[str.Length - 1];
                    for (int j = 1; j < str.Length; j++)
                        datas[j - 1] = str[j];
                    mt.TranslateMessage(str[0], datas);
                    rcvbuf.Clear();
                }
                else
                    rcvbuf.Add(e.Buffer[i]);
            }
            e.SetBuffer(0, plength);
            socket.ReceiveAsync(e);
        }

        private void OnAfterConncection(object sender, SocketAsyncEventArgs e)
        {
            interrupt = false;
        }

        public bool ConnectAsync()
        {
            socket = new System.Net.Sockets.Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            SocketAsyncEventArgs args = new SocketAsyncEventArgs();
            args.RemoteEndPoint = new DnsEndPoint(Interface.serverIP, Interface.port);
            args.Completed += OnAfterConncection;

            interrupt = true;
            socket.ConnectAsync(args);
            int count = 10;
            while (interrupt)
            {
                Thread.Sleep(500);
                if (--count <= 0)
                    return false;
            }
            new Thread(this.ReceiveListen).Start();
            return true;
        }

        Queue<byte> sndBuffer=new Queue<byte>();
        bool sndFinished = true;

        public void SendAsync(string base64, EventHandler<SocketAsyncEventArgs> eh = null)
        {
            byte[] data = SafeEncoder.Base64ToBytes(base64);
            for (int i = 0; i < data.Length; i++)
                sndBuffer.Enqueue(data[i]);
            if (sndFinished)
            {
                sndFinished = false;
                SocketAsyncEventArgs args = new SocketAsyncEventArgs();
                args.RemoteEndPoint = socket.RemoteEndPoint;
                int getLength = Math.Min(1 << 20, sndBuffer.Count);
                byte[] prepSend = new byte[getLength];
                for (int i = 0; i < getLength; i++)
                    prepSend[i] = sndBuffer.Dequeue();
                args.SetBuffer(prepSend, 0, getLength);
                args.Completed += SendComplete;
                if (eh != null)
                    args.Completed += eh;
                socket.SendAsync(args);
            }
        }

        public void SendComplete(object sender, SocketAsyncEventArgs e)
        {
            if (sndBuffer.Count == 0)
                sndFinished = true;
            else
            {
                int getLength = Math.Min(1 << 20, sndBuffer.Count);
                byte[] prepSend = new byte[getLength];
                for (int i = 0; i < getLength; i++)
                    prepSend[i] = sndBuffer.Dequeue();
                e.SetBuffer(prepSend, 0, getLength);
                e.Completed += SendComplete;
                socket.SendAsync(e);
            }
        }
    }

    public class Base64Change
    {
        public static string StringToBase64(string str)
        {
            try
            {
                return Convert.ToBase64String(Encoding.UTF8.GetBytes(str));
            }
            catch
            {
                return "";
            }
        }

        public static string Base64ToString(string base64)
        {
            try
            {
                byte[] bytes = Convert.FromBase64String(base64);
                return UTF8Encoding.UTF8.GetString(bytes, 0, bytes.Length);
            }
            catch
            {
                return "";
            }
        }
    }

    public class Interface
    {
        public const String serverIP = "202.119.81.70";//"127.0.0.1";
        public const int port = 4502;
    }

    public class SafeEncoder
    {
        public const Char seperator = ';';
        public const int length = MsgQueue.plength;

        public static byte[] Base64ToBytes(String base64)
        {
            return UTF8Encoding.UTF8.GetBytes(base64);
        }

        public static String GetSendUserString(String userid, String password)
        {
            return MsgPackager.Packager(new string[] { "log", userid, password });
        }
    }

    public class ShareFile
    {
        public static String Read(String path)
        {
            FileStream fs = new FileStream(path, FileMode.Open);
            byte[] data = new byte[fs.Length];
            fs.Read(data, 0, data.Length);
            fs.Close();
            return Convert.ToBase64String(data);
        }

        public static void Write(String path, String content)
        {
            byte[] data = Convert.FromBase64String(content);
            FileStream fs = new FileStream(path, FileMode.Create);
            fs.Write(data, 0, data.Length);
            fs.Close();
        }
    }
}
