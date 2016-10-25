/*
 * 创建时间：2012.3.29
 * 修改时间：2012.4.3
 * 负责人：崔嵬
 */
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net;
using System.Windows;
using System.Windows.Browser;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Media.Animation;
using System.Windows.Shapes;
using System.ComponentModel;
using System.Windows.Navigation;
using System.Net.Sockets;
using System.IO;
using System.Windows.Markup;
using CommonLibrary;
using System.Diagnostics;
using Microsoft.Win32;

namespace IMWebClient
{
    public partial class MainPage : Page, MessageTranslatable
    {
        LoginPage parent;

        public String selfId;
        public String selfName;

        #region

        public int status;
        public MsgQueue mq;

        private void SendFinish(object sender, SocketAsyncEventArgs args)
        {
            status = 20;
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

        public void ActivateState()
        {
            while (true)
            {
                System.Threading.Thread.Sleep(2000);
                SendMessage("activate", new string[] { });
                this.Dispatcher.BeginInvoke((Action)delegate()
                {
                    progressBar1.Value = status * 25;
                });
                status = status - 1;
                if (status==0)
                    break;
            }
            this.Dispatcher.BeginInvoke((Action)delegate()
            {
                this.Content = new LoginPage();
                MessageWindow.ShowMessage("网络断线，请检查网络环境状态！");
            });
        }

        public MainPage(LoginPage pg)
        {
            parent = pg;
            InitializeComponent();
        }

        // 当用户导航到此页面时执行。
        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
        }
        #endregion
        
        //个人分组好友的临时存储变量

        public Dictionary<string, TreeViewItem> getGroupByGid = new Dictionary<string, TreeViewItem>();
        public Dictionary<string, TextBlock> getPersonByPid = new Dictionary<string, TextBlock>();
        public Dictionary<string, TreeViewItem> getGroupByPid = new Dictionary<string, TreeViewItem>();
        public Dictionary<string, ChatDialog> getChatDialogByPid = new Dictionary<string, ChatDialog>();


        public ChildHistoryWin historyWin;
        public ChildWebDisk webDisk;

        public void TranslateMessage(String key, String[] data)
        {
            this.Dispatcher.BeginInvoke((Action)delegate()
            {
                try
                {
                    #region 个人信息处理
                    if ("con".Equals(key))
                    {

                        if (data[0].Equals("Y"))
                        {
                            parent.Content = this;

                            status = 20;
                            new System.Threading.Thread(new System.Threading.ThreadStart(ActivateState)).Start();
                        }
                        else
                            MessageWindow.ShowMessage("对不起，用户账号或者密码不正确，登录不成功！");
                        this.Dispatcher.BeginInvoke((Action)delegate()
                        {
                            parent.msg.Content = "";
                        });
                    }
                    else if ("off".Equals(key))
                        HtmlPage.Window.Navigate(new Uri("http://202.119.81.70:90/OKickedOut.aspx"));
                    else if (key.Equals("nick"))
                    {
                        selfName = data[0];
                        NickName.Content = selfName;
                    }
                    else if (key.Equals("gender"))
                    {
                        string gender = data[0].Equals("True") ? "♂" : "♀";
                        NickName.Content += gender;
                    }
                    #endregion

                    #region 分组处理

                    else if (key.Equals("crtgroup"))
                    {
                        TreeViewItem tvi = new TreeViewItem();
                        TextBlock tb = new TextBlock();
                        tb.DataContext = data[0]; // TreeViewItem::TextBlock 记录：(string)分组ID
                        tb.Text = data[1];
                        tvi.Header = tb;
                        getGroupByGid[data[0]] = tvi;
                        friendsList.Items.Add(tvi);

                        ContextMenu cm = new ContextMenu();
                        MenuItem addGroup = new MenuItem();
                        addGroup.DataContext = data[0]; // MenuItem 记录：(string)分组ID
                        addGroup.Header = "创建新分组";
                        addGroup.Click += AddGroup_Click;
                        cm.Items.Add(addGroup);
                        MenuItem renameGroup = new MenuItem();
                        renameGroup.Header = "重命名分组";
                        renameGroup.Click += RenameGroup_Click;
                        renameGroup.DataContext = data[0]; // MenuItem 记录：(string)分组ID
                        cm.Items.Add(renameGroup);
                        MenuItem removeGroup = new MenuItem();
                        removeGroup.Header = "删除该分组";
                        removeGroup.Click += RemoveGroup_Click;
                        removeGroup.DataContext = data[0]; // MenuItem 记录：(string)分组ID
                        cm.Items.Add(removeGroup);
                        MenuItem addFriend = new MenuItem();
                        addFriend.Header = "添加新好友";
                        addFriend.Click += AddFriend_Click;
                        removeGroup.DataContext = data[0]; // MenuItem 记录：(string)分组ID
                        cm.Items.Add(addFriend);
                        ContextMenuService.SetContextMenu(tb, cm);

                    }
                    else if (key.Equals("chggroup"))
                    {
                        ((TextBlock)getGroupByGid[data[0]].Header).Text = data[1];
                    }
                    else if (key.Equals("delroup"))
                    {
                        TreeViewItem tvi = getGroupByGid[data[0]];
                        friendsList.Items.Remove(tvi);
                        getGroupByGid.Remove(data[0]);
                    }
                    #endregion

                    #region 好友处理
                    else if (key.Equals("cnffrd"))
                    {
                        new ChildAddConfirm(this, data).Show();
                    }
                    else if (key.Equals("crtfrd"))
                    {
                        TreeViewItem groupItem = getGroupByGid[data[1]];
                        TextBlock tb = new TextBlock();
                        tb.Text = data[2] + "(" + data[0] + ")" + (data[3].Equals("True") ? "♂" : "♀");
                        tb.DataContext = data[0];
                        groupItem.Items.Add(tb);
                        getPersonByPid[data[0]] = tb;
                        getGroupByPid[data[0]] = groupItem;
                        tb.MouseLeftButtonUp += OpenChatDialog_Click;

                        ContextMenu cm = new ContextMenu();
                        MenuItem renameFriend = new MenuItem();
                        renameFriend.Header = "修改好友备注";
                        renameFriend.Click += RenameFriend_Click;
                        renameFriend.DataContext = data[0]; // MenuItem 记录：(string)好友ID
                        cm.Items.Add(renameFriend);
                        MenuItem deleteFriend = new MenuItem();
                        deleteFriend.Header = "将该好友删除";
                        deleteFriend.Click += DeleteFriend_Click; // MenuItem 记录：(string)好友ID
                        renameFriend.DataContext = data[0];
                        cm.Items.Add(deleteFriend);
                        MenuItem chgGroup = new MenuItem();
                        chgGroup.Header = "将好友移动至";
                        chgGroup.Click += ChangeGroup_Click;
                        cm.Items.Add(chgGroup);
                        ContextMenuService.SetContextMenu(tb, cm);
                    }
                    else if (key.Equals("delfrd"))
                    {
                        getGroupByPid[data[0]].Items.Remove(getPersonByPid[data[0]]);
                        getGroupByPid.Remove(data[0]);
                        getPersonByPid.Remove(data[0]);
                        if (getChatDialogByPid.ContainsKey(data[0]))
                        {
                            getChatDialogByPid[data[0]].Close();
                            MessageWindow.ShowMessage("好友 " + data[0] + " 从您的好友列表中移除，故双方会话结束。");
                        }
                    }
                    else if (key.Equals("chgfrd"))
                    {
                        string text = getPersonByPid[data[0]].Text;
                        getPersonByPid[data[0]].Text = data[1];
                        if (text.IndexOf('(') != -1)
                            getPersonByPid[data[0]].Text += text.Substring(text.IndexOf('('));
                        if (getChatDialogByPid.ContainsKey(data[0]))
                            getChatDialogByPid[data[0]].Title = "与 " + data[1] + " 聊天中";
                    }
                    else if (key.Equals("frdstt"))//提示好友是否在线
                    {
                        bool onln = data[1].Equals("True");
                        if (onln)
                        {
                            sound_log.Play();
                            sound_log.Position = new TimeSpan(0, 0, 0);
                        }
                        string online = onln ? "在线" : "离线";
                        string text = getPersonByPid[data[0]].Text;
                        string sign = text.Substring(text.IndexOf('('));
                        int pos = text.IndexOf('(') + Math.Max(sign.IndexOf("♂"), sign.IndexOf("♀"));
                        getPersonByPid[data[0]].Text = text.Substring(0, pos + 1) + online;
                    }
                    #endregion

                    #region 接收消息
                    else if (key.Equals("recvmsg"))
                    {
                        sound_msg.Play();
                        sound_msg.Position = new TimeSpan(0, 0, 0);
                        if (!getPersonByPid.ContainsKey(data[0]))
                            return;
                        if (!getChatDialogByPid.ContainsKey(data[0]))
                            new ChatDialog(this, data[0]).Show();
                        string text = getPersonByPid[data[0]].Text;
                        getChatDialogByPid[data[0]].InsertText(text.Substring(0, text.IndexOf('(')), data[2], data[1]);
                    }
                    #endregion

                    #region 接收图片
                    else if (key.Equals("recvimg"))
                    {
                        sound_msg.Play();
                        sound_msg.Position = new TimeSpan(0, 0, 0);
                        if (!getPersonByPid.ContainsKey(data[0]))
                            return;
                        if (!getChatDialogByPid.ContainsKey(data[0]))
                            new ChatDialog(this, data[0]).Show();
                        string text = getPersonByPid[data[0]].Text;
                        getChatDialogByPid[data[0]].InsertImage(text.Substring(0, text.IndexOf('(')), data[2], Convert.FromBase64String(data[1]));
                    }
                    #endregion

                    #region 历史记录
                    else if (key.Equals("history"))
                    {
                        if (historyWin != null)
                        {
                            try
                            {
                                string text;
                                if (data[0].Equals(selfId))
                                    text = selfName;
                                else
                                {
                                    text = getPersonByPid[data[0]].Text;
                                    text = text.Substring(0, text.IndexOf('('));
                                }
                                if (!data[2].Equals("img"))
                                    historyWin.InsertText(text, data[4], data[3]);
                                else
                                    historyWin.InsertImage(text, data[4], Convert.FromBase64String(data[3]));
                            }
                            catch { }
                        }
                    }
                    #endregion
                    else if (key.Equals("option") && cff != null)
                        cff.listBox1.Items.Add("账号(昵称)：" + data[0] + "(" + data[1] + ")");
                    //其他
                    else if (key.Equals("error"))
                        MessageWindow.ShowMessage(data[0]);
                    else if (key.Equals("accept"))
                    {
                        parent.userid.Text = data[0];
                        parent.password.Password = data[1];
                        parent.login_Click(this, null);
                        MessageWindow.ShowMessage("恭喜您！新账号注册成功。");
                    }
                    else if (key.Equals("reject"))
                        MessageWindow.ShowMessage("对不起，该账号名已被其他用户使用，请更换账号名称。");
                    else if (key.Equals("onrcvfile"))
                        new ChildCheckFile(this, data).Show();
                    else if (key.Equals("webfile"))
                    {
                        if (webDisk != null)
                        {
                            TextBlock tb = new TextBlock();
                            tb.Text = data[0];
                            webDisk.lbFiles.Items.Insert(0, tb);
                            ContextMenu cm = new ContextMenu();
                            MenuItem download = new MenuItem();
                            download.DataContext = tb.Text;
                            download.Header = "下载该文档";
                            download.Click += Download_Click;
                            cm.Items.Add(download);
                            MenuItem deletion = new MenuItem();
                            deletion.DataContext = tb.Text;
                            deletion.Header = "删除该文档";
                            deletion.Click += Delete_Click;
                            cm.Items.Add(deletion);
                            ContextMenuService.SetContextMenu(tb, cm);
                        }
                    }
                    else if (key.Equals("refresh"))
                    {
                        if (webDisk != null)
                        {
                            webDisk.lbFiles.Items.Clear();
                            webDisk.fileUpload.IsEnabled = true;
                            SendMessage("webdisk", new string[] { });
                        }
                    }
                }
                catch (Exception ex)
                {
                    MessageBox.Show("来自MainPage::TranslateMessage"+ex.Message);
                }
            });
        }

        #region 开发人员自测完成
        private void LayoutRoot_MouseRightButtonDown(object sender, MouseButtonEventArgs e)
        {
            e.Handled = true;   //屏蔽默认的右键菜单
        }

        private void Download_Click(object sender, RoutedEventArgs e)
        {
            string text = ((MenuItem)sender).DataContext.ToString();
            HtmlPage.Window.Navigate(new Uri("http://202.119.81.70:91/webdisk/" + selfId + "/" + text));
        }

        private void Delete_Click(object sender, RoutedEventArgs e)
        {
            string text = ((MenuItem)sender).DataContext.ToString();
            SendMessage("rmfile", new string[] { text });
        }

        private void hyperlink_Click(object sender, RoutedEventArgs e)
        {
            MessageWindow.ShowMessage("对不起，桌面端聊天工具尚不完善，部分功能受限。");
            HtmlPage.Window.Navigate(new Uri("http://202.119.81.70:91/IMWinClient.zip"));
        }

        private void javalink_Click(object sender, RoutedEventArgs e)
        {
            HtmlPage.Window.Navigate(new Uri("http://202.119.81.70:91/MobileIMsg.jar"));
        }

        private void AddGroup_Click(object sender, RoutedEventArgs e)
        {
            new ChildAddGroup(this).Show();
        }

        private void RenameGroup_Click(object sender, RoutedEventArgs e)
        {
            new ChildRenameGroup(this, (string)((MenuItem)sender).DataContext).Show();
        }

        private void RemoveGroup_Click(object sender, RoutedEventArgs e)
        {
            if (getGroupByGid.Count == 1)
                MessageWindow.ShowMessage("对不起，您不能将所有分组删除！");
            else
                new ChildRemoveGroup(this,(string)((MenuItem)sender).DataContext).Show();
        }

        private void AddFriend_Click(object sender, RoutedEventArgs e)
        {
            new ChildAddFriend(this, (string)((MenuItem)sender).DataContext).Show();
        }

        private void Modify_Click(object sender, RoutedEventArgs e)
        {
            new ChildModify(this).Show();
        }

        private void Logoff_Click(object sender, RoutedEventArgs e)
        {
            HtmlPage.Window.Navigate(new Uri("http://202.119.81.70:91/"));
        }

        private void RenameFriend_Click(object sender, RoutedEventArgs e)
        {
            new ChildRenameFriend(this, (string)((MenuItem)sender).DataContext).Show();
        }

        private void DeleteFriend_Click(object sender, RoutedEventArgs e)
        {
            new ChildRemovePerson(this, (string)((MenuItem)sender).DataContext).Show();
        }

        private void ChangeGroup_Click(object sender, RoutedEventArgs e)
        {
            string fid=(string)((MenuItem)sender).DataContext;
            if (!getChatDialogByPid.ContainsKey(fid))
                new ChildChangeGroup(this, fid).Show();
            else
                MessageWindow.ShowMessage("请您先关闭与该好友的聊天会话区在进行移动好友。");
        }

        private void OpenChatDialog_Click(object sender, RoutedEventArgs e)
        {
            string fid = (string)((TextBlock)sender).DataContext;
            if (getChatDialogByPid.ContainsKey(fid))
                getChatDialogByPid[fid].Focus();
            else
                new ChatDialog(this, fid).Show();
        }
        #endregion

        public ChildFindFriends cff=null;

        private void hyperlinkButton1_Click(object sender, RoutedEventArgs e)
        {
            new ChildFindFriends(this).Show();
        }

        private void WebDisk_Click(object sender, RoutedEventArgs e)
        {
            new ChildWebDisk(this).Show();
            SendMessage("webdisk", new string[] { });
        }
    }
}
