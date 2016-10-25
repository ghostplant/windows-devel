/*
 * 创建时间：2012.3.29
 * 修改时间：2012.4.9
 * 负责人：HJWAJ
 */

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Media;
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
using System.Net.Sockets;
using System.Windows.Navigation;
using System.Diagnostics;
using System.Windows.Resources;
using Microsoft.Win32;

namespace IMWinClient
{

    public partial class MainWindow : Window,MessageTranslatable
    {
        LoginWindow parent;
        public Image imale, ifemale;
        public string male, female;
        public RichTextBox maleRTB, femaleRTB;
        //通过调用SendMessage(key,datas)发送数据
        //分组和好友的map
        #region
        public Dictionary<String, TreeViewItem> getGroupItemById = new Dictionary<string, TreeViewItem>();
        public Dictionary<String, TreeViewItem> getFriendItemById = new Dictionary<string, TreeViewItem>();
        public Dictionary<String, ChatDialog> getChatItemById = new Dictionary<string, ChatDialog>();
        public Dictionary<String, HistoryWindow> getHistoryItemById = new Dictionary<string, HistoryWindow>();
        #endregion
        //离线文件的map
        #region
        public Dictionary<String, Progress> getProgressOffItemById = new Dictionary<string, Progress>();
        public Dictionary<String, int> getProgressCntOffItemById = new Dictionary<string, int>();
        public Dictionary<String, int> getProgressCntOffNumItemById = new Dictionary<string, int>();
        public Dictionary<String, string> getSaveFileUrlOffItemById = new Dictionary<string, string>();
        public Dictionary<String, string> getSaveFileOffItemById = new Dictionary<string, string>();
        public Dictionary<String, string> getProgressDataOffItemById = new Dictionary<string, string>();
        public Dictionary<String, string> getUrlOffItemById = new Dictionary<string, string>();
        public Dictionary<String, string> getToIdOffItemById = new Dictionary<string, string>();
        #endregion
        //在线文件的map
        #region
        public Dictionary<String, Progress> getProgressOnItemById = new Dictionary<string, Progress>();
        public Dictionary<String, int> getProgressCntOnItemById = new Dictionary<string, int>();
        public Dictionary<String, int> getProgressCntOnNumItemById = new Dictionary<string, int>();
        public Dictionary<String, string> getSaveFileUrlOnItemById = new Dictionary<string, string>();
        public Dictionary<String, string> getSaveFileOnItemById = new Dictionary<string, string>();
        public Dictionary<String, string> getProgressDataOnItemById = new Dictionary<string, string>();
        public Dictionary<String, string> getUrlOnItemById = new Dictionary<string, string>();
        public Dictionary<String, string> getToIdOnItemById = new Dictionary<string, string>();
        #endregion
        //个人分组好友的临时存储变量
        #region
        public String remGroupKey = "";
        public String remGroupName = "";
        public String remFriendKey = "";
        public String remFriendName = "";
        public String selfId;
        #endregion

        public void TranslateMessage(String key, String[] data)
        {
            this.Dispatcher.BeginInvoke((Action)delegate()
            {
                try
                {
                    if ("con".Equals(key))
                    {
                        if (data[0].Equals("Y"))
                        {
                            this.Show();
                            parent.loggedIn = true;
                            parent.Close();
                        }
                        else if (data[0].Equals("D"))
                        {
                            MessageBoxResult mbResult = MessageBox.Show("您开没有对该应用进行授权，是否跳转到主站进行授权操作？", "授权验证请求", MessageBoxButton.OKCancel, MessageBoxImage.Question);
                            if (mbResult == MessageBoxResult.OK)
                            {
                                string ret = string.Format("{0}?client_id={1}&response_type=code&redirect_uri={2}", "http://icpc.njust.edu.cn/OAuth/Authorize", "b681a5074007b0a8c2e8fda84ec7be80017c2f04", "http://202.119.81.70:90/OAuthRedirect.aspx");
                                System.Diagnostics.Process.Start(ret);
                            }
                        }
                        else
                            MessageBox.Show("对不起，用户账号或者密码不正确，登录不成功！");
                        this.Dispatcher.BeginInvoke((Action)delegate()
                        {
                            parent.msg.Content = "";
                        });
                    }
                    //个人信息
                    #region
                    else if (key.Equals("nick"))//昵称
                    {
                        NickName.Text = data[0];
                    }
                    else if (key.Equals("gender"))//性别
                    {
                        try
                        {
                            if (data[0] == "True")
                            {
                                imale = new Image();
                                //Gender = this.maleRTB;
                                System.Windows.Documents.FlowDocument doc = Gender.Document;
                                doc.Blocks.Clear();
                                byte[] image_full_original_data = Convert.FromBase64String(male);
                                BitmapImage bImg = new BitmapImage();
                                bImg.BeginInit();
                                bImg.StreamSource = new MemoryStream(image_full_original_data);
                                bImg.EndInit();
                                imale.Source = bImg;
                                new InlineUIContainer(this.imale, Gender.Selection.End);
                                Gender.DataContext = "♂";
                            }
                            else
                            {
                                ifemale = new Image();
                                //Gender = this.femaleRTB;
                                System.Windows.Documents.FlowDocument doc = Gender.Document;
                                doc.Blocks.Clear();
                                byte[] image_full_original_data = Convert.FromBase64String(female);
                                BitmapImage bImg = new BitmapImage();
                                bImg.BeginInit();
                                bImg.StreamSource = new MemoryStream(image_full_original_data);
                                bImg.EndInit();
                                ifemale.Source = bImg;
                                new InlineUIContainer(this.ifemale, Gender.Selection.End);
                                Gender.DataContext = "♀";
                            }
                        }
                        catch (Exception ee)
                        {
                            MessageBox.Show(ee.ToString());
                        }
                    }
                    else if (key.Equals("error"))//报错
                    {
                        MessageBox.Show(data[0]);
                    }
                    #endregion
                    //分组管理
                    #region
                    else if (key.Equals("crtgroup"))//创建分组
                    {
                        TreeViewItem tvItem = new TreeViewItem();
                        tvItem.Header = data[1];
                        tvItem.DataContext = data[0];
                        tvItem.MouseRightButtonDown += tb_MouseRightButtonDown;
                        tvItem.MouseRightButtonUp += tb_MouseRightButtonUp;
                        friendsList.Items.Add(tvItem);
                        getGroupItemById.Add(data[0], tvItem);
                    }
                    else if (key.Equals("chggroup"))//修改分组名称
                    {
                        getGroupItemById[data[0]].Header = data[1];
                    }
                    else if (key.Equals("delroup"))//删除分组
                    {
                        TreeViewItem tvItem = getGroupItemById[data[0]];
                        friendsList.Items.Remove(tvItem);
                        getGroupItemById.Remove(data[0]);
                    }
                    #endregion
                    //好友管理
                    #region
                    else if (key.Equals("crtfrd"))//添加好友
                    {
                        TreeViewItem friendItem = new TreeViewItem();
                        TreeViewItem grpItem = getGroupItemById[data[1]];
                        String friendInfo = "";
                        friendInfo += data[2];
                        friendInfo += "(";
                        friendInfo += data[0];
                        friendInfo += ")";
                        String gender;
                        if (data[3] == "True")
                        {
                            friendInfo += "  ♂";
                            gender = "  ♂";
                        }
                        else
                        {
                            friendInfo += "  ♀";
                            gender = "  ♀";
                        }
                        friendItem.Header = friendInfo;
                        object obj = new object[] { (object)data[0], (object)data[1], (object)data[2], (object)gender, (object)"  离线", (object)"False" };//ID，组ID，备注，性别，是否在线，是否有窗口
                        friendItem.DataContext = obj;
                        friendItem.MouseRightButtonDown += friend_MouseRightButtonDown;
                        friendItem.MouseRightButtonUp += friend_MouseRightButtonUp;
                        friendItem.MouseDoubleClick += friend_MouseDoubleClick;
                        grpItem.Items.Add(friendItem);
                        getFriendItemById.Add(data[0], friendItem);
                    }
                    else if (key.Equals("chgfrd"))//好友改名
                    {
                        object[] obj = (object[])getFriendItemById[data[0]].DataContext;
                        String[] datas = new String[6];
                        for (int i = 0; i < obj.Length; i++) datas[i] = (String)obj[i];
                        getFriendItemById[data[0]].Header = data[1] + "(" + datas[0] + ")" + datas[3] + datas[4];
                        obj[2] = (object)data[1];
                        getFriendItemById[data[0]].DataContext = (object)obj;
                        if (getChatItemById.ContainsKey(data[0]))
                        {
                            getChatItemById[data[0]].Title = "与 " + data[1] + " 聊天中";
                        }
                    }
                    else if (key.Equals("delfrd"))//删除好友
                    {
                        TreeViewItem tvItem = getFriendItemById[data[0]];
                        object[] obj = (object[])getFriendItemById[data[0]].DataContext;
                        String[] info = new String[2];
                        info[0] = (String)obj[0];
                        info[1] = (String)obj[1];
                        getGroupItemById[info[1]].Items.Remove(tvItem);
                        if (getChatItemById.ContainsKey(data[0]))
                        {
                            MessageBox.Show("由于删除好友，聊天窗口即将关闭！");
                            getChatItemById[data[0]].Close();
                            getChatItemById.Remove(data[0]);
                        }
                        getFriendItemById.Remove(data[0]);
                    }
                    else if (key.Equals("frdstt"))//提示好友是否在线
                    {
                        object[] obj = (object[])getFriendItemById[data[0]].DataContext;
                        if (data[1] == "True") obj[4] = "  在线";
                        else obj[4] = "  离线";
                        getFriendItemById[data[0]].Header = (String)obj[2] + "(" + (String)obj[0] + ")" + (String)obj[3] + (String)obj[4];
                        getFriendItemById[data[0]].DataContext = (object)obj;
                    }
                    else if (key.Equals("cnffrd"))//添加好友确认
                    {
                        String msg = "";
                        msg += "用户 ";
                        msg += data[1];
                        msg += " 请求添加您为好友，是否同意？";
                        MessageBoxResult mbResult = MessageBox.Show(msg, "", MessageBoxButton.OKCancel, MessageBoxImage.Question);
                        if (mbResult == MessageBoxResult.OK)
                        {
                            new AcptCnf(this, data[0], data[2], data[3]).ShowDialog();
                        }
                    }
                    #endregion
                    //接受消息和图片
                    #region
                    else if (key.Equals("recvmsg"))//接受消息
                    {
                        SoundPlayer player = new SoundPlayer((String)AppDomain.CurrentDomain.BaseDirectory + "msg.wav");
                        player.Play();
                        RichTextBox stackrtb = new RichTextBox();
                        stackrtb.SelectAll();
                        stackrtb.Paste();
                        object[] obj;
                        try
                        {
                            obj = (object[])getFriendItemById[data[0]].DataContext;
                        }
                        catch
                        {
                            MessageBox.Show("陌生人 " + data[0] + " 于 " + data[2] + " 发来消息：" + data[1]);
                            return;
                        }
                        if (((String)obj[5]).Equals("False"))
                        {
                            obj[5] = (object)"True";
                            getFriendItemById[(String)obj[0]].DataContext = (object)obj;
                            ChatDialog chatDialog = new ChatDialog((String)obj[2], this, (String)obj[0]);
                            getChatItemById.Add((String)obj[0], chatDialog);
                            chatDialog.Show();
                            chatDialog.Activate();
                        }
                        ChatDialog cDialog = getChatItemById[data[0]];
                        String head = (String)obj[2] + "  " + data[2];
                        RichTextBox rtb = new RichTextBox();
                        RichTextBox rtb2 = cDialog.msgRecRichTextBox;
                        rtb2.SelectAll();
                        rtb2.Copy();
                        cDialog.msgRecRichTextBox.SelectAll();
                        cDialog.msgRecRichTextBox.Paste();
                        rtb.AppendText(head);
                        rtb.SelectAll();
                        rtb.Selection.ApplyPropertyValue(TextElement.ForegroundProperty, Brushes.Blue);
                        rtb.Copy();
                        cDialog.msgRecRichTextBox.Paste();
                        cDialog.msgRecRichTextBox.AppendText(data[1]+"\r\n");
                        stackrtb.SelectAll();
                        stackrtb.Copy();
                    }
                    else if (key.Equals("recvimg"))//接收图片
                    {
                        SoundPlayer player = new SoundPlayer((String)AppDomain.CurrentDomain.BaseDirectory + "msg.wav");
                        player.Play();
                        RichTextBox stackrtb = new RichTextBox();
                        stackrtb.SelectAll();
                        stackrtb.Paste();
                        object[] obj;
                        try
                        {
                            obj = (object[])getFriendItemById[data[0]].DataContext;
                        }
                        catch
                        {
                            MessageBox.Show("陌生人 " + data[0] + " 发来图片，由于您未加其为好友，内容不予显示");
                            return;
                        }
                        if (((String)obj[5]).Equals("False"))
                        {
                            obj[5] = (object)"True";
                            getFriendItemById[(String)obj[0]].DataContext = (object)obj;
                            ChatDialog chatDialog = new ChatDialog((String)obj[2], this, (String)obj[0]);
                            getChatItemById.Add((String)obj[0], chatDialog);
                            chatDialog.Show();
                            chatDialog.Activate();
                        }
                        ChatDialog cDialog = getChatItemById[data[0]];
                        String head = (String)obj[2] + "  " + data[2];
                        RichTextBox rtb = new RichTextBox();
                        RichTextBox rtb2 = cDialog.msgRecRichTextBox;
                        rtb2.SelectAll();
                        rtb2.Copy();
                        cDialog.msgRecRichTextBox.SelectAll();
                        cDialog.msgRecRichTextBox.Paste();
                        rtb.AppendText(head);
                        rtb.SelectAll();
                        rtb.Selection.ApplyPropertyValue(TextElement.ForegroundProperty, Brushes.Blue);
                        rtb.Copy();
                        cDialog.msgRecRichTextBox.Paste();
                        Image img2 = new Image();
                        BitmapImage bImg2 = new BitmapImage();
                        img2.IsEnabled = true;
                        byte[] image_full_original_data = Convert.FromBase64String(data[1]);
                        bImg2.BeginInit();
                        bImg2.StreamSource = new MemoryStream(image_full_original_data);
                        bImg2.EndInit();
                        img2.Source = bImg2;
                        new InlineUIContainer(img2, cDialog.msgRecRichTextBox.Selection.End); //插入图片到选定位置
                        stackrtb.SelectAll();
                        stackrtb.Copy();
                    }
                    else if (key.Equals("imgack"))
                    {
                        getChatItemById[data[0]].SendPicButton.IsEnabled = true;
                    }
                    #endregion
                    //离线接收文件
                    #region
                    else if (key.Equals("cnffsnd"))//确认是否接收
                    {
                        object[] obj;
                        try
                        {
                            obj = (object[])getFriendItemById[data[0]].DataContext;
                        }
                        catch
                        {
                            MessageBox.Show("陌生人 " + data[0] + " 请求向您发送文件，由于您未加其为好友，服务器代为拒绝了此文件。");
                            SendMessage("rjctget", new String[] { data[2], data[0], data[1] });
                            return;
                        }
                        String nick = (String)obj[2];
                        if (MessageBox.Show("好友 " + nick + " 请求向您发送文件 " + data[1] + " ，是否接收？", "消息", MessageBoxButton.YesNo, MessageBoxImage.Question).Equals(MessageBoxResult.Yes))
                        {
                            //接收
                            SaveFileDialog sfd = new SaveFileDialog();
                            sfd.Filter = "All Files (*.*)|*.*";
                            sfd.DefaultExt = "*" + data[1].Substring(data[1].LastIndexOf('.'));
                            if (sfd.ShowDialog() == true)
                            {
                                getSaveFileUrlOffItemById.Add(data[2], sfd.SafeFileName);
                                getSaveFileOffItemById.Add(data[2], "");
                            }
                            SendMessage("accptget", new String[] { data[2], data[1] });
                        }
                        else//拒绝
                        {
                            SendMessage("rjctget", new String[] { data[2], data[0], data[1] });
                        }
                    }
                    else if (key.Equals("rjctdvr"))
                    {
                        object[] obj = (object[])getFriendItemById[data[0]].DataContext;
                        String nick = (String)obj[2];
                        MessageBox.Show("对不起，" + nick + " 拒绝接收您发送的文件 \"" + data[1] + "\"。");
                    }
                    else if (key.Equals("prepget"))//服务器准备接受文件
                    {
                        getToIdOffItemById.Add(data[2], data[0]);
                        getUrlOffItemById.Add(data[2], data[1]);
                        String datas = ShareFile.Read(data[1]);
                        getProgressDataOffItemById.Add(data[2], datas);
                        int parts = datas.Length / 200000;
                        if (datas.Length % 200000 != 0) parts++;
                        getProgressCntOffItemById.Add(data[2], parts);
                        getProgressCntOffNumItemById.Add(data[2], 0);
                        Progress pg = new Progress();
                        getProgressOffItemById.Add(data[2], pg);
                        getProgressOffItemById[data[2]].label.Content = "正在发送 " + data[1];
                        //设置进度条节数parts
                        getProgressOffItemById[data[2]].progressBar.Maximum = parts;
                        pg.Show();
                        if (getProgressCntOffItemById[data[2]] - 1 == getProgressCntOffNumItemById[data[2]])
                            SendMessage("sfileseg", new String[] { data[2], datas.Substring(200000 * getProgressCntOffNumItemById[data[2]]) });
                        else
                            SendMessage("sfileseg", new String[] { data[2], datas.Substring(200000 * getProgressCntOffNumItemById[data[2]], 200000) });
                    }
                    else if (key.Equals("segcnt"))
                    {
                        int cntnum = getnum(data[1]);
                        Progress pg = new Progress();
                        getProgressOffItemById.Add(data[0], pg);
                        getProgressOffItemById[data[0]].label.Content = "正在接收文件" + getSaveFileUrlOffItemById[data[0]];
                        //设置进度条节数cntnum
                        getProgressOffItemById[data[0]].progressBar.Maximum = cntnum;
                        pg.Show();
                    }
                    else if (key.Equals("segsucc"))
                    {
                        getProgressOffItemById[data[0]].progressBar.Value += 1.0;
                        getProgressCntOffNumItemById[data[0]]++;
                        if (getProgressCntOffNumItemById[data[0]] == getProgressCntOffItemById[data[0]])
                        {
                            SendMessage("sfilefnsh", new String[] { data[0], getUrlOffItemById[data[0]], getToIdOffItemById[data[0]] });
                            MessageBox.Show("文件已成功发送至服务器！");
                            getProgressOffItemById[data[0]].Close();
                            getProgressOffItemById.Remove(data[0]);
                            getProgressCntOffNumItemById.Remove(data[0]);
                            getProgressCntOffItemById.Remove(data[0]);
                            getProgressDataOffItemById.Remove(data[0]);
                            getToIdOffItemById.Remove(data[0]);
                            getUrlOffItemById.Remove(data[0]);
                        }
                        else
                        {
                            if (getProgressCntOffItemById[data[0]] - 1 == getProgressCntOffNumItemById[data[0]])
                                SendMessage("sfileseg", new String[] { data[0], getProgressDataOffItemById[data[0]].Substring(200000 * getProgressCntOffNumItemById[data[0]]) });
                            else
                                SendMessage("sfileseg", new String[] { data[0], getProgressDataOffItemById[data[0]].Substring(200000 * getProgressCntOffNumItemById[data[0]], 200000) });
                        }
                    }
                    else if (key.Equals("tfileseg"))
                    {
                        getSaveFileOffItemById[data[0]] += data[1];
                        getProgressOffItemById[data[0]].progressBar.Value += 1.0;
                    }
                    else if (key.Equals("tfilefnsh"))
                    {
                        ShareFile.Write(getSaveFileUrlOffItemById[data[0]], getSaveFileOffItemById[data[0]]);
                        MessageBox.Show("文件 " + data[1] + " 已成功接收！");
                        getProgressOffItemById[data[0]].Close();
                        getProgressOffItemById.Remove(data[0]);
                        getSaveFileUrlOffItemById.Remove(data[0]);
                        getSaveFileOffItemById.Remove(data[0]);
                    }
                    #endregion
                    //在线接收文件
                    #region
                    else if (key.Equals("prepgeton"))
                    {
                        getToIdOnItemById.Add(data[1], data[0]);
                        getUrlOnItemById.Add(data[1], data[2]);
                        Progress pg = new Progress();
                        getProgressOnItemById.Add(data[1], pg);
                        String datas = ShareFile.Read(data[2]);
                        getProgressDataOnItemById.Add(data[1], datas);
                        int parts = datas.Length / 200000;
                        if (datas.Length % 200000 != 0) parts++;
                        getProgressOnItemById[data[1]].label.Content = "正在发送 " + data[2];
                        //设置进度条节数parts
                        getProgressOnItemById[data[1]].progressBar.Maximum = parts;
                        pg.Show();
                        getProgressCntOnItemById.Add(data[1], parts);
                        getProgressCntOnNumItemById.Add(data[1], 0);
                        SendMessage("ssegcnton", new String[] { data[0], data[1], parts.ToString() });
                        if (getProgressCntOnItemById[data[1]] - 1 == getProgressCntOnNumItemById[data[1]])
                            SendMessage("sfilesegon", new String[] { data[1], datas.Substring(200000 * getProgressCntOnNumItemById[data[1]]), data[0] });
                        else
                            SendMessage("sfilesegon", new String[] { data[1], datas.Substring(200000 * getProgressCntOnNumItemById[data[1]], 200000), data[0] });
                    }
                    else if (key.Equals("cnffsndon"))
                    {
                        object[] obj;
                        try
                        {
                            obj = (object[])getFriendItemById[data[0]].DataContext;
                        }
                        catch
                        {
                            MessageBox.Show("陌生人 " + data[0] + " 请求向您发送文件，由于您未加其为好友，服务器代为拒绝了此文件。");
                            SendMessage("rjctgeton", new String[] { data[2], data[0], data[1] });
                            return;
                        }
                        String nick = (String)obj[2];
                        if (MessageBox.Show("好友 " + nick + " 请求向您发送文件 " + data[1] + " ，是否接收？", "消息", MessageBoxButton.YesNo, MessageBoxImage.Question).Equals(MessageBoxResult.Yes))
                        {
                            //接收
                            SaveFileDialog sfd = new SaveFileDialog();
                            sfd.Filter = "All Files (*.*)|*.*";
                            if (sfd.ShowDialog() == true)
                            {
                                getSaveFileUrlOnItemById.Add(data[2], sfd.SafeFileName);
                                getSaveFileOnItemById.Add(data[2], "");
                            }
                            SendMessage("accptgeton", new String[] { data[0], data[2], data[1] });
                        }
                        else//拒绝
                        {
                            SendMessage("rjctgeton", new String[] { data[2], data[0], data[1] });
                        }
                    }
                    else if (key.Equals("rjctdvron"))
                    {
                        object[] obj = (object[])getFriendItemById[data[0]].DataContext;
                        String nick = (String)obj[2];
                        MessageBox.Show("对不起，" + nick + " 拒绝接收您发送的文件 \"" + data[1] + "\"。");
                    }
                    else if (key.Equals("tsegcnton"))
                    {
                        int cntnum = getnum(data[1]);
                        Progress pg = new Progress();
                        getProgressOnItemById.Add(data[0], pg);
                        getProgressOnItemById[data[0]].label.Content = "正在接收文件" + getSaveFileUrlOnItemById[data[0]];
                        //设置进度条节数cntnum
                        getProgressOnItemById[data[0]].progressBar.Maximum = cntnum;
                        pg.Show();
                    }
                    else if (key.Equals("segsuccon"))
                    {
                        getProgressOnItemById[data[0]].progressBar.Value += 1.0;
                        getProgressCntOnNumItemById[data[0]]++;
                        if (getProgressCntOnNumItemById[data[0]] == getProgressCntOnItemById[data[0]])
                        {
                            SendMessage("sfilefnshon", new String[] { data[0], getUrlOnItemById[data[0]], getToIdOnItemById[data[0]] });
                            MessageBox.Show("向好友 " + getToIdOnItemById[data[0]] + " 发送的文件 " + getUrlOnItemById[data[0]] + "已成功发送！");
                            getProgressOnItemById[data[0]].Close();
                            getProgressOnItemById.Remove(data[0]);
                            getProgressCntOnNumItemById.Remove(data[0]);
                            getProgressCntOnItemById.Remove(data[0]);
                            getProgressDataOnItemById.Remove(data[0]);
                            getToIdOnItemById.Remove(data[0]);
                            getUrlOnItemById.Remove(data[0]);
                        }
                        else
                        {
                            if (getProgressCntOnItemById[data[0]] - 1 == getProgressCntOnNumItemById[data[0]])
                                SendMessage("sfilesegon", new String[] { data[0], getProgressDataOnItemById[data[0]].Substring(200000 * getProgressCntOnNumItemById[data[0]]), getToIdOnItemById[data[0]] });
                            else
                                SendMessage("sfilesegon", new String[] { data[0], getProgressDataOnItemById[data[0]].Substring(200000 * getProgressCntOnNumItemById[data[0]], 200000), getToIdOnItemById[data[0]] });
                        }
                    }
                    else if (key.Equals("tfilesegon"))
                    {
                        getSaveFileOnItemById[data[0]] += data[1];
                        getProgressOnItemById[data[0]].progressBar.Value += 1.0;
                    }
                    else if (key.Equals("tfilefnshon"))
                    {
                        ShareFile.Write(getSaveFileUrlOnItemById[data[0]], getSaveFileOnItemById[data[0]]);
                        MessageBox.Show("文件 " + data[1] + " 已成功接收！");
                        getProgressOnItemById[data[0]].Close();
                        getProgressOnItemById.Remove(data[0]);
                        getSaveFileUrlOnItemById.Remove(data[0]);
                        getSaveFileOnItemById.Remove(data[0]);
                    }
                    else if (key.Equals("off"))
                    {
                        MessageBox.Show("当前账号在其他地区登录，您被迫下线！");
                        Environment.Exit(0);
                    }
                    #endregion
                    //历史消息
                    #region
                    else if (key.Equals("history"))
                    {
                        try
                        {
                            HistoryWindow hw;
                            if (this.selfId.Equals(data[0]))
                            {
                                hw = getHistoryItemById[data[1]];
                            }
                            else
                            {
                                hw = getHistoryItemById[data[0]];
                            }
                            RichTextBox stackrtb = new RichTextBox();
                            stackrtb.SelectAll();
                            stackrtb.Paste();

                            RichTextBox rtb = hw.historyRecRichTextBox;
                            rtb.SelectAll();
                            rtb.Copy();
                            hw.historyRecRichTextBox.SelectAll();
                            hw.historyRecRichTextBox.Paste();
                            RichTextBox rtb2 = new RichTextBox();
                            String head = "";
                            if (this.selfId.Equals(data[0])) head += this.NickName.Text;
                            else
                            {
                                object[] obj = (object[])getFriendItemById[data[0]].DataContext;
                                head += (String)obj[2];
                            }
                            head += "  ";
                            head += data[4];
                            rtb2.AppendText(head);
                            rtb2.SelectAll();
                            if (this.selfId.Equals(data[0])) rtb2.Selection.ApplyPropertyValue(TextElement.ForegroundProperty, Brushes.DarkGreen);
                            else rtb2.Selection.ApplyPropertyValue(TextElement.ForegroundProperty, Brushes.Blue);
                            rtb2.Copy();
                            hw.historyRecRichTextBox.Paste();
                            if (data[2].Equals("msg"))//文字记录
                            {
                                hw.historyRecRichTextBox.AppendText(data[3] + "\r\n");
                            }
                            else//图片记录
                            {
                                Image img2 = new Image();
                                BitmapImage bImg2 = new BitmapImage();
                                img2.IsEnabled = true;
                                byte[] image_full_original_data = Convert.FromBase64String(data[3]);
                                bImg2.BeginInit();
                                bImg2.StreamSource = new MemoryStream(image_full_original_data);
                                bImg2.EndInit();
                                img2.Source = bImg2;
                                new InlineUIContainer(img2, hw.historyRecRichTextBox.Selection.End); //插入图片到选定位置
                                //hw.historyRecRichTextBox.AppendText("\r\n");
                            }
                            stackrtb.SelectAll();
                            stackrtb.Copy();
                        }
                        catch (Exception ee)
                        {
                            MessageBox.Show(ee.ToString());
                        }
                    }
                    #endregion
                    else
                        MessageBox.Show(key);
                }
                catch { }
            });
        }

        #region
        private void changePersonalData_Click(object sender, RoutedEventArgs e)
        {
            new ChangeData(this).ShowDialog();
        }

        private void newGroup_KeyUp(object sender, KeyEventArgs e)
        {
            TextBox tb = (TextBox)sender;
            if (e.Key.Equals(Key.Enter))
            {
                if (tb.Text.Length > 0)
                {
                    if (tb.Text.Length <= 20)
                    {
                        SendMessage("addgroup", new String[] { tb.Text, "False" });
                        friendsList.Items.Remove(tb);
                    }
                    else
                    {
                        MessageBox.Show("分组命名过长，请限制在20个字符之内！");
                    }
                }
            }
        }

        private void renameGroup_KeyUp(object sender, KeyEventArgs e)
        {
            String name = ((TextBox)sender).Text;
            if (e.Key.Equals(Key.Enter) && name.Length > 0)
            {
                if (name.Length > 20)
                {
                    MessageBox.Show("分组命名过长，请限制在20个字符之内！");
                }
                else
                {
                    SendMessage("rengroup", new String[] { remGroupKey, name });
                    TreeViewItem tvItem = getGroupItemById[remGroupKey];
                    tvItem.Header = remGroupName;
                }
            }
        }

        private void renameFriend_KeyUp(object sender, KeyEventArgs e)
        {
            String name = ((TextBox)sender).Text;
            if (e.Key.Equals(Key.Enter) && name.Length > 0)
            {
                if (name.Length > 20)
                {
                    MessageBox.Show("好友昵称命名过长，请限制在20个字符之内！");
                }
                else
                {
                    SendMessage("renfrd", new String[] { remFriendKey, name });
                    TreeViewItem tvItem = getFriendItemById[remFriendKey];
                    tvItem.Header = remFriendName;
                }
            }
        }

        private void addGroup_Click(object sender, RoutedEventArgs e)
        {
            TextBox tb = new TextBox();
            tb.Text = "新的分组";
            tb.SelectAll();
            tb.KeyUp += newGroup_KeyUp;
            friendsList.Items.Add(tb);
        }

        private void renameGroup_Click(object sender, RoutedEventArgs e)
        {
            TextBox tb = new TextBox();
            TreeViewItem tvItem=getGroupItemById[remGroupKey];
            tb.Text = (String)tvItem.Header;
            remGroupName = (String)tvItem.Header;
            tb.KeyUp += renameGroup_KeyUp;
            tvItem.Header = tb;
        }

        private void delGroup_Click(object sender, RoutedEventArgs e)
        {
            if (getGroupItemById.Count == 1)
            {
                MessageBox.Show("对不起，仅剩一个分组时不能删除！");
                return;
            }
            else
            {
                MessageBoxResult mbResult = MessageBox.Show("确定要删除该分组？", "", MessageBoxButton.OKCancel, MessageBoxImage.Question);
                if (mbResult == MessageBoxResult.OK) SendMessage("rmgroup", new String[] { remGroupKey });
            }
        }

        private void addFriend_Click(object sender, RoutedEventArgs e)
        {
            new AddFriend(this).ShowDialog();
        }

        private void renameFriend_Click(object sender, RoutedEventArgs e)
        {
            TextBox tb = new TextBox();
            TreeViewItem tvItem = getFriendItemById[remFriendKey];
            object[] obj=(object [])tvItem.DataContext;
            String datas=(String)obj[2];
            tb.Text = datas;
            remFriendName = (String)tvItem.Header;
            tb.KeyUp += renameFriend_KeyUp;
            tvItem.Header = tb;
        }

        private void delFriend_Click(object sender, RoutedEventArgs e)
        {
            MessageBoxResult mbResult = MessageBox.Show("确定要删除该好友？", "", MessageBoxButton.OKCancel, MessageBoxImage.Question);
            if (mbResult == MessageBoxResult.OK) SendMessage("rmfrd", new String[] { remFriendKey });
        }

        private void chgFrdGroup_Click(object sender, RoutedEventArgs e)
        {
            MenuItem mi=(MenuItem)sender;
            SendMessage("xchgfrd", new String[] { remFriendKey, (String)mi.DataContext, remFriendName });
        }

        private void tb_MouseRightButtonDown(object sender, MouseButtonEventArgs e)
        {
            e.Handled = true;
        }

        private void tb_MouseRightButtonUp(object sender, MouseButtonEventArgs e)
        {
            TreeViewItem item = (TreeViewItem)sender;
            String info = (String)(item.DataContext);
            remGroupKey=info;

            ContextMenu groupMenu = new ContextMenu();
            groupMenu.StaysOpen = true;

            MenuItem addGroup=new MenuItem();
            addGroup.Header = "新建分组";
            addGroup.Click += addGroup_Click;
            groupMenu.Items.Add(addGroup);

            MenuItem renameGroup = new MenuItem();
            renameGroup.Header = "修改名称";
            renameGroup.Click += renameGroup_Click;
            groupMenu.Items.Add(renameGroup);

            MenuItem delGroup = new MenuItem();
            delGroup.Header = "删除分组";
            delGroup.Click += delGroup_Click;
            groupMenu.Items.Add(delGroup);

            MenuItem addFriend = new MenuItem();
            addFriend.Header = "添加好友";
            addFriend.Click += addFriend_Click;
            groupMenu.Items.Add(addFriend);

            item.ContextMenu = groupMenu;
        }

        private void friend_MouseRightButtonDown(object sender, MouseButtonEventArgs e)
        {
            e.Handled = true;
        }

        private void friend_MouseRightButtonUp(object sender, MouseButtonEventArgs e)
        {
            TreeViewItem item = (TreeViewItem)sender;
            object[] obj = (object [])(item.DataContext);
            remFriendKey = (String)obj[0];
            remFriendName = (String)obj[2];

            ContextMenu friendMenu = new ContextMenu();
            friendMenu.StaysOpen = true;

            MenuItem renameFriend = new MenuItem();
            renameFriend.Header = "修改备注";
            renameFriend.Click += renameFriend_Click;
            friendMenu.Items.Add(renameFriend);

            MenuItem delFriend = new MenuItem();
            delFriend.Header = "删除好友";
            delFriend.Click += delFriend_Click;
            friendMenu.Items.Add(delFriend);

            MenuItem chgGroup = new MenuItem();
            chgGroup.Header = "修改组别";
               
            foreach (KeyValuePair<string, TreeViewItem> myItem in getGroupItemById)
            {
                MenuItem grpMenuItem = new MenuItem();
                grpMenuItem.Header = myItem.Value.Header;
                grpMenuItem.DataContext = myItem.Key;
                grpMenuItem.Click += chgFrdGroup_Click;
                chgGroup.Items.Add(grpMenuItem);
            }
            friendMenu.Items.Add(chgGroup);
            item.ContextMenu = friendMenu;
        }

        private void friend_MouseDoubleClick(object sender, MouseButtonEventArgs e)
        {
            TreeViewItem item = (TreeViewItem)sender;
            object[] obj = (object[])(item.DataContext);
            String state = (String)obj[5];
            if (obj[5].Equals("False"))
            {
                obj[5] = (object)"True";
                getFriendItemById[(String)obj[0]].DataContext = (object)obj;
                ChatDialog chatDialog = new ChatDialog((String)obj[2], this, (String)obj[0]);
                getChatItemById.Add((String)obj[0], chatDialog);
                chatDialog.Show();
                chatDialog.Activate();
            }
            else
            {
                getChatItemById[(String)obj[0]].Activate();
            }
        }
        #endregion

        //MsgQueue
        #region
        public MsgQueue mq;
        int status = 5;

        private void SendFinish(object sender, SocketAsyncEventArgs args)
        {
            status = 5;
        }
        
        public bool SendMessage(String key, String[] data)
        {
            string[] arr = new string[data.Length + 1];
            arr[0] = key;
            for (int i = 0; i < data.Length; i++)
                arr[i + 1] = data[i];
            mq.SendAsync(MsgPackager.Packager(arr), SendFinish);
            return true;
        }

        public MainWindow(String mid,LoginWindow lw)
        {
            InitializeComponent();
            parent = lw;
            this.selfId = mid;
            this.Closing += dialogCloseEvent;
            String localurl = (String)AppDomain.CurrentDomain.BaseDirectory + "Images\\i_male.gif";
            male = ShareFile.Read(localurl);
            localurl = (String)AppDomain.CurrentDomain.BaseDirectory + "Images\\i_female.gif";
            female = ShareFile.Read(localurl);
        }
        
        #endregion

        public void ActivateState()
        {
            while (true)
            {
                status = status - 1;
                SendMessage("activate", new string[] { });
                System.Threading.Thread.Sleep(2000);
                if (status <= 0)
                {
                    MessageBox.Show("请检查网络状态，与服务器的连接中断。");
                    Environment.Exit(0);
                }
            }
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            new System.Threading.Thread(new System.Threading.ThreadStart(ActivateState)).Start();
        }

        private void dialogCloseEvent(object sender, CancelEventArgs e)
        {
            Environment.Exit(0);
        }

        private void SaveFile(string filename, RichTextBox richTextBox)
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

        private int getnum(String s)
        {
            return Int32.Parse(s);
        }
    }
}
