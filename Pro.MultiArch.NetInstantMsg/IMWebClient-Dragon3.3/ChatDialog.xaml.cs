/*
 * 创建时间：2012.3.29
 * 修改时间：2012.4.4
 * 负责人：崔嵬
 */

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using System.ComponentModel;
using System.IO;
using System.Windows.Markup;
using CommonLibrary;
using System.Diagnostics;
using Microsoft.Win32;
using System.Threading;

namespace IMWebClient
{
    public partial class ChatDialog : FloatableWindow
    {
        public String fid;
        public MainPage mp;

        #region 已完成
        public ChatDialog(MainPage mp,String fid)
        {
            this.mp = mp;
            this.fid = fid;
            InitializeComponent();
            mp.getChatDialogByPid[fid] = this;
            
            ParentLayoutRoot = mp.LayoutRoot;
            Width = 500;
            Height = 500;
            ResizeMode = ResizeMode.CanResize;

            string text = mp.getPersonByPid[fid].Text;
            this.Title = "与 " + text.Substring(0, text.IndexOf('(')) + " 聊天中";
        }

        private void FloatableWindow_Closed(object sender, EventArgs e)
        {
            mp.getChatDialogByPid.Remove(fid);
        }

        private void SendButton_Click(object sender, RoutedEventArgs e)
        {
            if (sendMsgTextBox.Text.Length == 0 || sendMsgTextBox.Text.Length > 2048)
            {
                MessageWindow.ShowMessage("对不起，你所发送的信息长度必须在1-2048个字之内!");
                return;
            }

            InsertText(mp.selfName, DateTime.Now.ToString(), sendMsgTextBox.Text,mp.getPersonByPid[fid].Text.Contains("离线"));

            mp.SendMessage("sendmsg", new string[] { fid, sendMsgTextBox.Text });
            sendMsgTextBox.Text = "";
        }


        public void InsertText(string fname, string time, string data, bool remind = false)
        {
            time = time.Replace('-', '/');
            Bold info_run = new Bold();
            Run msg_run = new Run();
            info_run.Inlines.Add(fname + " [" + time + "]");
            msg_run.Text = data;

            Paragraph info_p = new Paragraph();
            info_p.Inlines.Add(info_run);
            msgRecRichTextBox.Blocks.Add(info_p);

            Paragraph msg_p = new Paragraph();
            msg_p.Inlines.Add(msg_run);
            msgRecRichTextBox.Blocks.Add(msg_p);

            if (remind)
            {
                Paragraph remind_p = new Paragraph();
                remind_p.Inlines.Add("(对方不在线，您发送的这条消息将会在对方下次登录的时候接收)");
                msgRecRichTextBox.Blocks.Add(remind_p);
            }

            Paragraph blank_p = new Paragraph();
            blank_p.Inlines.Add("");
            msgRecRichTextBox.Blocks.Add(blank_p);

            byte[] image = new byte[] { 0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a, 0x00, 0x00, 0x00, 0x0d, 0x49, 0x48, 0x44,
                0x52, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x08, 0x02, 0x00, 0x00, 0x00, 0x90, 0x77, 0x53, 0xde, 0x00,
                0x00, 0x00, 0x04, 0x67, 0x41, 0x4d, 0x41, 0x00, 0x00, 0xb1, 0x8f, 0x0b, 0xfc, 0x61, 0x05, 0x00, 0x00, 0x00, 0x0c,
                0x49, 0x44, 0x41, 0x54, 0x18, 0x57, 0x63, 0xf8, 0xff, 0xff, 0x3f, 0x00, 0x05, 0xfe, 0x02, 0xfe, 0xa7, 0x35, 0x81,
                0x84, 0x00, 0x00, 0x00, 0x00, 0x49, 0x45, 0x4e, 0x44, 0xae, 0x42, 0x60, 0x82 };
            Stream stream = new MemoryStream(image);
            BitmapImage bImg = new BitmapImage();
            bImg.SetSource(stream);
            Image img = new Image();
            img.Source = bImg;

            img.Height = img.Width = 1;

            InlineUIContainer container = new InlineUIContainer();
            container.Child = img;

            TextPointer EndofContent = msgRecRichTextBox.ContentEnd.GetNextInsertionPosition(LogicalDirection.Backward);
            msgRecRichTextBox.Selection.Select(EndofContent, EndofContent);
            msgRecRichTextBox.Selection.Insert(container);
        }

        public void InsertImage(string fname, string time, byte[] data,bool remind=false)
        {
            time = time.Replace('-', '/');
            Bold info_run = new Bold();
            info_run.Inlines.Add(fname + " [" + time + "]\n");

            Paragraph info_p = new Paragraph();
            info_p.Inlines.Add(info_run);
            msgRecRichTextBox.Blocks.Add(info_p);

            Stream stream = new MemoryStream(data);
            BitmapImage bImg = new BitmapImage();
            bImg.SetSource(stream);
            Image img = new Image();
            img.Source = bImg;

            img.Height = (img.Width = 200) * 1.2;

            InlineUIContainer container = new InlineUIContainer();
            container.Child = img;

            TextPointer EndofContent = msgRecRichTextBox.ContentEnd.GetNextInsertionPosition(LogicalDirection.Backward);
            msgRecRichTextBox.Selection.Select(EndofContent, EndofContent);
            msgRecRichTextBox.Selection.Insert(container);

            if (remind)
            {
                Paragraph remind_p = new Paragraph();
                remind_p.Inlines.Add("(对方不在线，您发送的这条消息将会在对方下次登录的时候接收)");
                msgRecRichTextBox.Blocks.Add(remind_p);
            }

            Paragraph blank_p = new Paragraph();
            blank_p.Inlines.Add("");
            msgRecRichTextBox.Blocks.Add(blank_p);
        }

        private void SendPicButton_Click(object sender, RoutedEventArgs e)
        {
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.Filter = "图像文档 (*.jpg;*.jpeg;*.png)|*.jpg;*.jpeg;*.png";
            ofd.Multiselect = false;
            if (ofd.ShowDialog() == true)
            {
                FileInfo fi = ofd.File;
                FileStream fs = fi.OpenRead();
                if (fs.Length > (1 << 20))
                {
                    MessageWindow.ShowMessage("图片文档过大，直接文件发送只允许1M以内！");
                    return;
                }

                byte[] pic = new byte[fs.Length];
                fs.Read(pic, 0, pic.Length);
                fs.Close();

                string text = mp.getPersonByPid[fid].Text;

                InsertImage(mp.selfName, DateTime.Now.ToString(), pic,mp.getPersonByPid[fid].Text.Contains("离线"));
                mp.SendMessage("sendimg", new String[] { fid, Convert.ToBase64String(pic) });
            }
        }

        private void CloseButton_Click(object sender, RoutedEventArgs e)
        {
            this.Close();
        }
        #endregion

        #region 排除内容
        /*
        private static void LoadFile(string filename, RichTextBox richTextBox)
        {
            if (string.IsNullOrEmpty(filename))
            {
                throw new ArgumentNullException();
            }
            if (!File.Exists(filename))
            {
                throw new FileNotFoundException();
            }
            using (FileStream stream = File.OpenRead(filename))
            {
                TextRange documentTextRange = new TextRange(richTextBox.Document.ContentStart, richTextBox.Document.ContentEnd);
                string dataFormat = DataFormats.Text;
                string ext = System.IO.Path.GetExtension(filename);
                if (String.Compare(ext, ".xaml", true) == 0)
                {
                    dataFormat = DataFormats.Xaml;
                }
                else if (String.Compare(ext, ".rtf", true) == 0)
                {
                    dataFormat = DataFormats.Rtf;
                }
                documentTextRange.Load(stream, dataFormat);
            }
        }

        private static void SaveFile(string filename, RichTextBox richTextBox)
        {
            if (string.IsNullOrEmpty(filename))
            {
                throw new ArgumentNullException();
            }
            using (FileStream stream = File.OpenWrite(filename))
            {
                TextRange documentTextRange = new TextRange(richTextBox.Document.ContentStart, richTextBox.Document.ContentEnd);
                string dataFormat = DataFormats.Text;
                string ext = System.IO.Path.GetExtension(filename);
                if (String.Compare(ext, ".xaml", true) == 0)
                {
                    dataFormat = DataFormats.Xaml;
                }
                else if (String.Compare(ext, ".rtf", true) == 0)
                {
                    dataFormat = DataFormats.Rtf;
                }
                documentTextRange.Save(stream, dataFormat);
            }
        }
        */



        private void HistoryButton_Click(object sender, RoutedEventArgs e)
        {
            new ChildHistoryWin(this).Show();
            mp.SendMessage("history", new String[] { fid });
        }

        #endregion

        private void sendMsgTextBox_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Escape)
                SendButton_Click(sender, null);
        }

        private void SendFileButton_Click(object sender, RoutedEventArgs e)
        {
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.Filter="所有文件(*.*)|*.*||";
            ofd.Multiselect=false;
            if ((bool)ofd.ShowDialog())
            {
                if (mp.getPersonByPid[fid].Text.EndsWith("离线"))
                    MessageWindow.ShowMessage("对不起，对方目前不在线，如需要请使用离线发送方式！");
                else if (ofd.File.Length > (1 << 20))
                    MessageWindow.ShowMessage("对不起，目前浏览器支持的单个文件发送大小在1M以内，请分卷发送！");
                else
                {
                    FileStream fs = ofd.File.OpenRead();
                    byte[] data = new byte[ofd.File.Length];
                    fs.Read(data, 0, data.Length);
                    fs.Close();
                    mp.SendMessage("onlnfile", new string[] { fid, ofd.File.ToString(), Convert.ToBase64String(data) });
                }
            }
        }

    }
}

