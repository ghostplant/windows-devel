/*
 * 创建时间：2012.3.29
 * 修改时间：2012.4.9
 * 负责人：HJWAJ
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

namespace IMWinClient
{
    /// <summary>
    /// ChatDialog.xaml 的交互逻辑
    /// </summary>
    public partial class ChatDialog : Window
    {
        public String frd;
        public String key;
        public MainWindow mw;
        public ChatDialog(String frd, MainWindow mw, String key)
        {
            InitializeComponent();
            this.Title = "与 " + frd + " 聊天中";
            this.frd = frd;
            this.mw = mw;
            this.key = key;
            this.Closed += new EventHandler(ChatDialog_Closed);
            this.msgRecRichTextBox.TextChanged += msgRecRichTextBox_TextChanged;
            this.sendMsgTextBox.Text = "";
            this.sendMsgTextBox.KeyUp += new KeyEventHandler(sendMsgTextBox_KeyUp);
        }

        void sendMsgTextBox_KeyUp(object sender, KeyEventArgs e)
        {
            if( e.KeyboardDevice.Modifiers == ModifierKeys.Control
             && e.KeyboardDevice.IsKeyDown(Key.LeftCtrl)
             && (e.Key == Key.Enter || e.SystemKey == Key.Enter) )
            {
                SendButton_Click(sender, new RoutedEventArgs());
            }
        }

        void ChatDialog_Closed(object sender, EventArgs e)
        {
            mw.getChatItemById.Remove(this.key);
            object[] obj = (object[])mw.getFriendItemById[this.key].DataContext;
            obj[5] = (object)"False";
            mw.getFriendItemById[this.key].DataContext = (object)obj;
        }

        #region
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
        #endregion

        private void msgRecRichTextBox_TextChanged(object sender, TextChangedEventArgs e)
        {
            this.msgRecRichTextBox.ScrollToEnd();
        }

        private void SendButton_Click(object sender, RoutedEventArgs e)
        {
            String msg;
            sendMsgTextBox.SelectAll();
            msg = sendMsgTextBox.Text;
            if (msg.Length <= 0)
            {
                MessageBox.Show("发送内容不能为空！");
                return;
            }
            if (msg.Length >= 2048)
            {
                MessageBox.Show("消息太长，请分条发送！");
                return;
            }
            String head = mw.NickName.Text + "  " + DateTime.Now;
            RichTextBox rtb = new RichTextBox();
            RichTextBox rtb2 = this.msgRecRichTextBox;
            RichTextBox stackrtb = new RichTextBox();
            stackrtb.SelectAll();
            stackrtb.Paste();
            rtb2.SelectAll();
            rtb2.Copy();
            this.msgRecRichTextBox.SelectAll();
            this.msgRecRichTextBox.Paste();
            rtb.AppendText(head);
            rtb.SelectAll();
            rtb.Selection.ApplyPropertyValue(TextElement.ForegroundProperty, Brushes.DarkGreen);
            rtb.Copy();
            this.msgRecRichTextBox.Paste();
            stackrtb.SelectAll();
            stackrtb.Copy();
            sendMsgTextBox.Text = "";
            msgRecRichTextBox.AppendText(msg+"\r\n");
            mw.SendMessage("sendmsg", new String[] { key, msg });
        }

        private void CloseButton_Click(object sender, RoutedEventArgs e)
        {
            mw.getChatItemById.Remove(this.key);
            object[] obj=(object [])mw.getFriendItemById[this.key].DataContext;
            obj[5] = (object)"False";
            mw.getFriendItemById[this.key].DataContext = (object)obj;
            this.Close();
        }

        private void SendPicButton_Click(object sender, RoutedEventArgs e)
        {
            RichTextBox stackrtb = new RichTextBox();
            stackrtb.SelectAll();
            stackrtb.Paste();
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.Filter = "Picture Files (*.bmp;*.jpg;*.jpeg;*.gif;*.png)|*.bmp;*.jpg;*.jpeg;*.gif;*.png";
            ofd.Multiselect = false;
            if (ofd.ShowDialog() == true)
            {
                String fname1 = "", fname2 = "";
                String url = ofd.FileName;
                String fname=ofd.SafeFileName;
                String localurl = (String)AppDomain.CurrentDomain.BaseDirectory + "Temp\\";
                while(true)
                {
                    fname1=""+DateTime.Now;
                    for (int i = 0; i < fname1.Length; i++)
                    {
                        if (fname1.ElementAt(i).Equals('/') || fname1.ElementAt(i).Equals(' ') || fname1.ElementAt(i).Equals(':'))
                        {
                            fname1 = fname1.Substring(0, i) + "_" + fname1.Substring(i + 1, fname1.Length-i-1);
                        }
                    }
                    Random rd = new Random();
                    int rand = rd.Next();
                    fname2=rand.ToString();
                    if(!File.Exists(localurl+fname1+fname2+fname))break;
                }
                try
                {
                    File.Copy(url, localurl + fname1 + fname2 + fname);
                }
                catch (Exception ee)
                {
                    MessageBox.Show(ee.ToString());
                }
                url = localurl + fname1 + fname2 + fname;
                String tmp = ShareFile.Read(url);
                if (tmp.Length > 1000000)
                {
                    MessageBox.Show("图片太大，请以文件形式发送！");
                    stackrtb.SelectAll();
                    stackrtb.Copy();
                    return;
                }
                mw.SendMessage("sendimg",new string[]{key, tmp});
                SendPicButton.IsEnabled = false;

                Image img = new Image();
                BitmapImage bImg = new BitmapImage();
                img.IsEnabled = true;
                bImg.BeginInit();
                bImg.UriSource = new Uri(url, UriKind.Relative);
                bImg.EndInit();
                img.Source = bImg;

                RichTextBox rtb = msgRecRichTextBox;
                rtb.SelectAll();
                rtb.Copy();
                msgRecRichTextBox.SelectAll();
                msgRecRichTextBox.Paste();
                String head = mw.NickName.Text + "  " + DateTime.Now;
                RichTextBox rtb2 = new RichTextBox();
                rtb2.AppendText(head);
                rtb2.SelectAll();
                rtb2.Selection.ApplyPropertyValue(TextElement.ForegroundProperty, Brushes.DarkGreen);
                rtb2.Copy();
                msgRecRichTextBox.Paste();
                new InlineUIContainer(img, msgRecRichTextBox.Selection.End); //插入图片到选定位置
                msgRecRichTextBox.AppendText("\r\n");
                
            }
            
            stackrtb.SelectAll();
            stackrtb.Copy();
        }

        private void SendFileButton_Click(object sender, RoutedEventArgs e)
        {
            OpenFileDialog ofd = new Microsoft.Win32.OpenFileDialog();
            ofd.Filter = "All Files (*.*)|*.*";
            ofd.Multiselect = false;
            if (ofd.ShowDialog() == true)
            {
                FileInfo file = new FileInfo(ofd.FileName);
                if (file.Length > 2000000)
                {
                    MessageBox.Show("文件太大，无法传输！");
                    return;
                }
                if (file.Length <= 0)
                {
                    MessageBox.Show("请不要传输空文件！");
                    return;
                }
                String url=(String)ofd.FileName;
                object[] obj = (object[])mw.getFriendItemById[this.key].DataContext;
                if (((String)obj[4]).Equals("  在线"))//在线
                {
                    if (MessageBox.Show("是否使用离线传送功能？", "消息", MessageBoxButton.YesNo, MessageBoxImage.Question).Equals(MessageBoxResult.Yes))//离线发送
                    {
                        //离线发送
                        mw.SendMessage("offlnfile",new String[] { this.key, url });
                    }
                    else//在线传送
                    {
                        mw.SendMessage("onlnfile", new String[] { this.key, url });
                    }
                }
                else//离线
                { 
                    //离线传送
                    mw.SendMessage("offlnfile", new String[] { this.key, url });
                }
            }
        }

        private void HistoryButton_Click(object sender, RoutedEventArgs e)
        {
            if (!mw.getHistoryItemById.ContainsKey(this.key))
            {
                HistoryWindow hw;
                try
                {
                    hw = new HistoryWindow(this.mw, this.key);
                    hw.Show();
                }
                catch(Exception ee)
                {
                    MessageBox.Show(ee.ToString());
                }
                
                mw.SendMessage("history", new String[] { this.key });
            }
        }

    }
}
