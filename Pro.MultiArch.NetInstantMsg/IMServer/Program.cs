/*
 * 创建时间：..
 * 最后修改时间：2012.3.30
 * 负责人：崔嵬
 */

using System;
using System.Collections.Generic;
using System.IO;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Windows.Forms;

using CommonLibrary;
using System.Data.SqlClient;
using System.Data;
using IMServer;

namespace SocketServer
{
    class ClientThread:MessageTranslatable
    {
        public ClientThread(Socket socket)
        {
            this.socket = socket;
            loggedIn = false;
        }

        public bool loggedIn;

        public String id,nick;
        public Socket socket;
        public bool male;
        MsgQueue mq;

        class AsyncThread
        {
            public string dest,key;
            public string[] data;
            public ClientThread parent;

            public AsyncThread(string dest, string key, string[] data, ClientThread parent)
            {
                this.dest = dest;
                this.key = key;
                this.data = data;
                this.parent = parent;
            }

            public void SendAsync()
            {
                parent.SendMessage(dest, key, data);
            }
        }

        public void TranslateMessage(string key,string[] data)
        {
            Console.WriteLine(key);
            if (loggedIn)
            {
                #region 发送文本或图片
                if (("sendmsg".Equals(key) || "sendimg".Equals(key)) && data.Length == 2)
                {
                    string Type = key.Substring(key.Length - 3);
                    if ("img".Equals(Type))
                        SendMessage(id, "imgack", new string[] { data[0] });
                    bool online = false;
                    if (Program.onlineClients.ContainsKey(data[0]))
                    {
                        SendMessage(data[0], "recv" + Type, new string[] { id, data[1], DateTime.Now.ToString() });
                        online = true;
                    }
                    SqlConnection conn = new SqlConnection(SqlserverDB.connString);
                    conn.Open();
                    SqlCommand cmd = new SqlCommand("insert into messages values('" + id + "','" + data[0] + "',@Msg,@Dat," + (online ? "1" : "0") + ",'" + Type + "')", conn);
                    SqlParameter parmeter = new SqlParameter("@Msg", SqlDbType.Image);
                    byte[] b = UTF8Encoding.UTF8.GetBytes(Base64Change.StringToBase64(data[1]));
                    parmeter.Value = b;
                    cmd.Parameters.Add(parmeter);

                    parmeter = new SqlParameter("@Dat", SqlDbType.DateTime);
                    parmeter.Value = DateTime.Now;
                    cmd.Parameters.Add(parmeter);

                    cmd.ExecuteNonQuery();
                    conn.Close();
                }
                #endregion
                #region 分组管理
                else if ("rennick".Equals(key) && data.Length == 1)
                {
                    if (data[0].IndexOf('\'') != -1 || data[0].Length == 0)
                        SendMessage(id, "error", new String[] { "昵称为空或含有非法字符，不能更改！" });
                    else
                    {
                        SqlserverDB.Update("update clients set name='" + data[0] + "' where id='" + id + "'");
                        SendMessage(id, "nick", new String[] { data[0] });
                    }
                }
                else if ("rengender".Equals(key) && data.Length == 1)
                {
                    int value = data[0].Equals("True") ? 1 : 0;
                    SqlserverDB.Update("update clients set male=" + value + " where id='" + id + "'");
                    SendMessage(id, "gender", new String[] { data[0] });
                }
                else if ("addgroup".Equals(key) && data.Length == 2)
                {
                    if (data[0].IndexOf('\'') != -1 || data[0].Length == 0)
                        SendMessage(id, "error", new String[] { "组名称为空或含有非法字符，不能创建！" });
                    else
                    {
                        SqlDataReader sdr = SqlserverDB.Query("select * from groups where crtid='" + id + "' and name='" + data[0] + "'");
                        bool exist = sdr.Read();
                        SqlserverDB.Free(sdr);
                        if (exist)
                            SendMessage(id, "error", new String[] { "该组名称已经存在，不能创建！" });
                        else
                        {
                            SqlserverDB.Update("insert into groups(name,crtid,super) values('" + data[0] + "','" + id + "'," + (data[1].Equals("True") ? 1 : 0) + ")");
                            sdr = SqlserverDB.Query("select * from groups where crtid='" + id + "' and name='" + data[0] + "'");
                            exist = sdr.Read();
                            int gid = sdr.GetInt32(0);
                            SqlserverDB.Free(sdr);
                            if (!exist)
                                SendMessage(id, "error", new String[] { "新建分组未知错误，创建失败！" });
                            else
                            {
                                SendMessage(id, "crtgroup", new String[] { gid.ToString(), data[0], data[1] });
                            }
                        }
                    }
                }
                else if ("rengroup".Equals(key) && data.Length == 2)
                {
                    if (data[1].IndexOf('\'') != -1 || data[1].Length == 0)
                        SendMessage(id, "error", new String[] { "新组名称为空或含有非法字符，不能更改！" });
                    else
                    {
                        SqlDataReader sdr = SqlserverDB.Query("select * from groups where id<>" + data[0] + " and name='" + data[1] + "' and crtid='" + id + "'");
                        bool exist = sdr.Read();
                        SqlserverDB.Free(sdr);
                        if (exist)
                            SendMessage(id, "error", new String[] { "该组名称已经存在，不能更改！" });
                        else
                        {
                            int gid = 0;
                            try
                            {
                                gid = Int32.Parse(data[0]);
                                SqlserverDB.Update("update groups set name='" + data[1] + "' where id=" + gid + " and crtid='" + id + "'");
                                SendMessage(id, "chggroup", new String[] { data[0], data[1] });
                            }
                            catch (Exception ex)
                            {
                                Console.WriteLine("[警告:来自 TranslateMesssage::rengroup] " + ex.Message);
                                SendMessage(id, "error", new String[] { "组ID为非法关键字，不能更改！" });
                            }
                        }
                    }
                }
                else if ("rmgroup".Equals(key) && data.Length == 1)
                {
                    int gid = 0;
                    try
                    {
                        gid = Int32.Parse(data[0]);
                        SqlDataReader sdr = SqlserverDB.Query("select * from relations where groupid=" + gid);
                        bool exist = sdr.Read();
                        SqlserverDB.Free(sdr);
                        if (exist)
                            SendMessage(id, "error", new String[] { "该组含有好友成员，不能删除！（请先删除组内所有好友再删除该组）" });
                        else
                        {
                            SqlserverDB.Update("delete from groups where id=" + gid + " and crtid='" + id + "'");
                            SendMessage(id, "delroup", new String[] { data[0] });
                        }
                    }
                    catch (Exception ex)
                    {
                        Console.WriteLine("[警告:来自 TranslateMesssage::rmgroup] " + ex.Message);
                        SendMessage(id, "error", new String[] { "组ID为非法关键字，不能删除！" });
                    }
                }
                #endregion
                #region 好友管理
                else if (key.Equals("renfrd") && data.Length == 2)
                {
                    if (data[1].Length > 0 && data[1].IndexOf('\'') == -1)
                    {
                        SqlserverDB.Update("update relations set fndnm='" + data[1] + "' where fndid='" + data[0] + "' and groupid in (select id from groups where crtid='" + id + "')");
                        SendMessage(id, "chgfrd", data);
                    }
                    else
                        SendMessage(id, "error", new String[] { "好友备注含有非法关键字，不能更改！" });
                }
                else if (key.Equals("rmfrd") && data.Length == 1)
                {
                    SqlserverDB.Update("delete from relations where fndid='" + data[0] + "' and groupid in (select id from groups where crtid='" + id + "')");
                    SqlserverDB.Update("delete from relations where fndid='" + id + "' and groupid in (select id from groups where crtid='" + data[0] + "')");
                    SendMessage(id, "delfrd", data);
                    SendMessage(data[0], "delfrd", new string[] { id });
                }
                else if (key.Equals("xchgfrd") && data.Length == 3)
                {
                    SqlserverDB.Update("update relations set groupid=" + data[1] + " where fndid='" + data[0] + "' and groupid in (select id from groups where crtid='" + id + "')");
                    SendMessage(id, "delfrd", new string[] { data[0] });
                    SqlDataReader sdr = SqlserverDB.Query("select male from clients where id in (select fndid from relations where groupid in (select id from groups where crtid='" + id + "'))");
                    sdr.Read();
                    SendMessage(id, "crtfrd", new string[] { data[0], data[1], data[2], sdr["male"].ToString() });
                    if (Program.onlineClients.ContainsKey(data[0]))
                        SendMessage(id, "frdstt", new String[] { data[0], "True" });
                    else
                        SendMessage(id, "frdstt", new String[] { data[0], "False" });
                    SqlserverDB.Free(sdr);
                }
                else if (key.Equals("addfrd") && data.Length == 3)
                {
                    if (id.Equals(data[0]))
                    {
                        SendMessage(id, "error", new String[] { "对不起，您自己不是自己的好友！" });
                        return;
                    }
                    SqlDataReader sdr = SqlserverDB.Query("select * from request where srcid='" + id + "' and desid='" + data[0] + "'");
                    bool exist = sdr.Read();
                    SqlserverDB.Free(sdr);
                    if (exist) // 已经记录与数据库
                        SendMessage(id, "error", new String[] { "您已经发送过添加好友请求，请不要着急，谢谢！" });
                    else
                    {
                        sdr = SqlserverDB.Query("select * from relations where fndid='" + data[0] + "' and groupid in (select id from groups where crtid='" + id + "')");
                        exist = sdr.Read();
                        SqlserverDB.Free(sdr);
                        if (exist) // 已经是好友
                            SendMessage(id, "error", new String[] { "对方已经存在于您的好友列表，请核实！" });
                        else
                        {
                            sdr = SqlserverDB.Query("select name from clients where id='" + data[0] + "'");
                            exist = sdr.Read();
                            if (exist) // 对方ID存在
                            {
                                if (data[2].Equals("\n")) // 默认昵称
                                    data[2] = sdr.GetString(0);
                                SqlserverDB.Free(sdr);
                                String myname;
                                sdr = SqlserverDB.Query("select name from clients where id='" + id + "'");
                                sdr.Read();
                                myname = sdr.GetString(0);
                                SqlserverDB.Free(sdr);
                                SendMessage(id, "error", new String[] { "添加好友请求已经发送，请等待对方同意。" });
                                if (Program.onlineClients.ContainsKey(data[0]))
                                { // 直接发送
                                    AsyncThread at = new AsyncThread(data[0], "cnffrd", new String[] { id, myname, data[1], data[2] }, this);
                                    new Thread(new ThreadStart(at.SendAsync)).Start();
                                }
                                else // 离线发送
                                    SqlserverDB.Update("insert into request values('" + id + "','" + data[0] + "'," + data[1] + ",'" + data[2] + "','" + myname + "')");
                            }
                            else
                            {
                                SendMessage(id, "error", new String[] { "不存在ID为'" + data[0] + "'的用户，添加失败！" });
                                SqlserverDB.Free(sdr);
                            }
                        }
                    }
                }
                else if (key.Equals("acptcnf") && data.Length == 5)
                {
                    SqlDataReader sdr = SqlserverDB.Query("select * from groups where id=" + data[1]);
                    bool exist = sdr.Read();
                    SqlserverDB.Free(sdr);
                    if (exist)
                    {
                        sdr = SqlserverDB.Query("select male from clients where id='" + id + "'");
                        sdr.Read();
                        bool male = sdr.GetBoolean(0);
                        SqlserverDB.Free(sdr);
                        SqlserverDB.Update("insert into relations values(" + data[1] + ",'" + id + "','" + data[2] + "')");

                        sdr = SqlserverDB.Query("select * from groups where id=" + data[3]); // d3 for reverse groupid
                        exist = sdr.Read();
                        SqlserverDB.Free(sdr);
                        if (exist)
                        {
                            sdr = SqlserverDB.Query("select male from clients where id='" + id + "'");
                            sdr.Read();
                            bool revmale = sdr.GetBoolean(0);
                            SqlserverDB.Free(sdr);
                            SqlserverDB.Update("insert into relations values(" + data[3] + ",'" + data[0] + "','" + data[4] + "')");

                            SendMessage(id, "crtfrd", new string[] { data[0], data[3], data[4], revmale.ToString() });
                            if (Program.onlineClients.ContainsKey(data[0]))
                                SendMessage(id, "frdstt", new String[] { data[0], "True" });
                            else
                                SendMessage(id, "frdstt", new String[] { data[0], "False" });

                        }

                        if (Program.onlineClients.ContainsKey(data[0]))
                        {
                            SendMessage(data[0], "crtfrd", new string[] { id, data[1], data[2], male.ToString() });
                            if (Program.onlineClients.ContainsKey(data[0]))
                                SendMessage(data[0], "frdstt", new String[] { id, "True" });
                            else
                                SendMessage(data[0], "frdstt", new String[] { id, "False" });

                            SendMessage(data[0], "error", new string[] { data[2] + " 已经同意您的添加好友请求。" });
                        }
                    }
                }
                #endregion
                #region 历史管理
                else if ("history".Equals(key) && data.Length == 1)
                {
                    SqlDataReader sdr = SqlserverDB.Query("select top 30 * from messages where desid='" + id + "' and scrid='" + data[0] + "' or scrid='" + id + "' and desid='" + data[0] + "' order by cdate desc");
                    while (sdr.Read())
                    {
                        string Type = sdr["type"].ToString();
                        SendMessage(id, "history", new string[] { sdr["scrid"].ToString(), sdr["desid"].ToString(), Type, Base64Change.Base64ToString(UTF8Encoding.UTF8.GetString((byte[])sdr["msg"])), sdr["cdate"].ToString() });
                    }
                    SqlserverDB.Free(sdr);
                }
                #endregion
                /*#region 离线文件
                else if ("offlnfile".Equals(key) && data.Length == 2)
                {
                    Random rnd = new Random();
                    int next = rnd.Next();
                    while (Program.fileClip.ContainsKey(next))
                        next = rnd.Next();
                    Program.fileClip[next] = "";
                    SendMessage(id, "prepget", new string[] { data[0], data[1], next.ToString() });
                }
                else if ("sfileseg".Equals(key) && data.Length == 2)
                {
                    Program.fileClip[Int32.Parse(data[0])] += data[1];
                    SendMessage(id, "segsucc", new string[] { data[0] }); // 回应进度
                }
                else if ("sfilefnsh".Equals(key) && data.Length == 3)
                {
                    int clipId = Int32.Parse(data[0]);
                    String fname = data[1].Substring(data[1].LastIndexOf('\\') + 1);

                    if (Program.onlineClients.ContainsKey(data[2]))
                        SendMessage(data[2], "cnffsnd", new string[] { id, fname, data[0] });
                    else
                    {
                        byte[] b = UTF8Encoding.UTF8.GetBytes(Program.fileClip[clipId]);
                        Program.fileClip.Remove(clipId);


                        SqlConnection conn = new SqlConnection(SqlserverDB.connString);
                        conn.Open();
                        SqlCommand cmd = new SqlCommand("insert into msgfile(scrid,desid,msg,fname) values('" + id + "','" + data[2] + "',@Msg,'" + fname + "')", conn);
                        SqlParameter parmeter = new SqlParameter("@Msg", SqlDbType.Image);
                        parmeter.Value = b;
                        cmd.Parameters.Add(parmeter);
                        cmd.ExecuteNonQuery();
                        conn.Close();
                    }
                }
                else if ("rjctget".Equals(key) && data.Length == 3)
                {
                    int clipId = Int32.Parse(data[0]);
                    Program.fileClip.Remove(clipId);
                    if (Program.onlineClients.ContainsKey(data[1]))
                        SendMessage(data[1], "rjctdvr", new string[] { id, data[2] }); // 拒绝
                }
                else if ("accptget".Equals(key) && data.Length == 2)
                {
                    const int buffer = (1 << 18);
                    int clipId = Int32.Parse(data[0]);
                    String base64 = Program.fileClip[clipId];
                    Program.fileClip.Remove(clipId);
                    SendMessage(id, "segcnt", new string[] { data[0], ((base64.Length + buffer - 1) / buffer).ToString() });
                    for (int i = 0; i * buffer < base64.Length; ++i)
                    {
                        int l = (i + 1) * buffer;
                        if (l > base64.Length)
                            l = base64.Length;
                        SendMessage(id, "tfileseg", new string[] { data[0], base64.Substring(i * buffer, l - i * buffer) });
                    }
                    SendMessage(id, "tfilefnsh", new string[] { clipId.ToString(), data[1] });
                }
                #endregion*/
                #region 在线文件
                else if ("onlnfile".Equals(key) && data.Length == 3)
                    SendMessage(data[0], "onrcvfile", new string[] { id, data[1], data[2] });
                else if ("onlnaccpt".Equals(key) && data.Length == 3)
                    SendMessage(data[0], "recvmsg", new string[] { data[1], "[系统消息：" + data[1] + " 同意并接收了您发送的文件 " + data[2] + " 。]", DateTime.Now.ToString() });
                else if ("onlnrejct".Equals(key) && data.Length == 3)
                    SendMessage(data[0], "recvmsg", new string[] { data[1], "[系统消息：" + data[1] + " 拒绝接收您发送的文件 " + data[2] + " 。]", DateTime.Now.ToString() });
                #endregion
                #region 激活与剩余处理
                else if (key.Equals("activate"))
                {
                    if (!Program.onlineClients.ContainsKey(id))
                    {
                        Program.onlineClients.Add(id, socket);
                        SqlDataReader sdr = SqlserverDB.Query("select crtid from groups where id in (select groupid from relations where fndid='" + id + "')");
                        while (sdr.Read())
                        {
                            String oid = sdr.GetString(0);
                            if (Program.onlineClients.ContainsKey(oid))
                                SendMessage(oid, "frdstt", new String[] { id, "True" });
                        }
                        SqlserverDB.Free(sdr);
                    }
                }
                else if (key.Equals("find"))
                {
                    SqlDataReader sdr = SqlserverDB.Query("select id,name from clients where id like '%" + data[0] + "%' or name like '%" + data[0] + "%'");
                    while (sdr.Read())
                        SendMessage(id, "option", new string[] { sdr["id"].ToString(), sdr["name"].ToString() });
                    sdr.Close();
                }
                else if (key.Equals("webdisk"))
                {
                    if (!Directory.Exists(".\\imwebclient\\webdisk"))
                        Directory.CreateDirectory(".\\imwebclient\\webdisk");
                    if (!Directory.Exists(".\\imwebclient\\webdisk\\" + id))
                        Directory.CreateDirectory(".\\imwebclient\\webdisk\\" + id);
                    string[] files = Directory.GetFiles(".\\imwebclient\\webdisk\\" + id);
                    foreach (string file in files)
                        SendMessage(id, "webfile", new string[] { file.Substring(file.LastIndexOf('\\') + 1) });
                }
                else if (key.Equals("rmfile"))
                {
                    File.Delete(".\\imwebclient\\webdisk\\" + id + "\\" + data[0]);
                    SendMessage(id, "refresh", new string[] { });
                }
                else if (key.Equals("upload"))
                {
                    string fname = data[0];
                    if (File.Exists(".\\imwebclient\\webdisk\\" + id + "\\" + fname))
                    {
                        string prefix = fname.Contains(".") ? fname.Substring(0, fname.LastIndexOf('.')) : fname;
                        string ext = fname.Contains(".") ? fname.Substring(fname.LastIndexOf('.')) : "";
                        for (int i = 2; ;i++ )
                            if (!File.Exists(".\\imwebclient\\webdisk\\" + id + "\\" + prefix+" ("+i+")"+ext))
                            {
                                fname = ".\\imwebclient\\webdisk\\" + id + "\\" + prefix + " (" + i + ")" + ext;
                                break;
                            }
                    } else
                        fname = ".\\imwebclient\\webdisk\\" + id + "\\" + fname;
                    FileStream fs = new FileStream(fname, FileMode.Create);
                    byte[] datas = Convert.FromBase64String(data[1]);
                    fs.Write(datas, 0, datas.Length);
                    fs.Close();
                    SendMessage(id, "refresh", new string[] { });

                }
                else
                    SendMessage(id, "error", new String[] { "你发送的'" + key + "'消息格式错了，或者我还没有处理该消息！" });
                #endregion
            }
            else
            {   // 未登录状态
                if ("log".Equals(key))
                {
                    string userid = data[0];
                    int password = data[1].GetHashCode();
                    SqlDataReader sdr = SqlserverDB.Query("select * from clients where id='" + userid + "'");
                    bool exist = sdr.Read();
                    if (!exist)
                    {
                        mq.SendAsync(MsgPackager.Packager(new string[] { "con", "N" }));
                        SqlserverDB.Free(sdr);
                        return;
                    }

                    string hash = sdr["passhash"].ToString();
                    SqlserverDB.Free(sdr);

                    if (!password.ToString().Equals(hash))
                    {
                        mq.SendAsync(MsgPackager.Packager(new string[] { "con", "N" }));
                        return;
                    }
                    loggedIn = true;
                    mq.SendAsync(MsgPackager.Packager(new string[] { "con", "Y" }));
                    id = userid;
                    if (Program.onlineClients.ContainsKey(userid))
                    {
                        SendMessage(userid, "off", new string[] { });
                        Program.onlineClients.Remove(userid);
                    }
                    Program.onlineClients.Add(id, socket);
                    Console.WriteLine(DateTime.Now.ToString() + "：用户'" + id + "'登陆成功(" + socket.RemoteEndPoint.ToString() + ")。");
                    InitSendMessage();
                }
                else if ("reg".Equals(key))
                {
                    SqlDataReader sdr = SqlserverDB.Query("select * from clients where id='" + data[0] + "'");
                    bool exist = sdr.Read();
                    sdr.Close();
                    if (!exist)
                    {
                        SqlserverDB.Update("insert into clients values('" + data[0] + "','" + data[1] + "'," + (data[2].Equals("True") ? 1 : 0) + "," + data[3].GetHashCode() + ",0)");
                        SqlserverDB.Update("insert into groups(name,crtid,super) values('我的好友','" + data[0] + "',0)");
                        mq.SendAsync(MsgPackager.Packager(new string[] { "accept", data[0], data[3] }));
                    }
                    else
                        mq.SendAsync(MsgPackager.Packager(new string[] { "reject" }));
                }
            }
        }

        void InitSendMessage()
        {
            Console.WriteLine(id+" 数据初始化。");
            #region 成功测试
            SqlDataReader sdr;
            //通知上线提醒
            sdr = SqlserverDB.Query("select crtid from groups where id in (select groupid from relations where fndid='" + id + "')");
            while (sdr.Read())
            {
                String oid = sdr.GetString(0);
                if (Program.onlineClients.ContainsKey(oid))
                    SendMessage(oid, "frdstt", new String[] { id, "True" });
            }
            SqlserverDB.Free(sdr);

            // 获取基本信息
            sdr = SqlserverDB.Query("select * from clients where id='" + id + "'");
            sdr.Read();
            nick = sdr.GetString(1);
            male = sdr.GetBoolean(2);
            SqlserverDB.Free(sdr);
            SendMessage(id, "nick", new String[] { nick });
            SendMessage(id, "gender", new String[] { male.ToString() });

            // 获取分组
            sdr = SqlserverDB.Query("select * from groups where crtid='" + id + "'");
            while (sdr.Read())
            {
                String gid, gname, super;
                gid = sdr.GetInt32(0).ToString();
                gname = sdr.GetString(1);
                super = sdr.GetBoolean(3).ToString();
                SendMessage(id, "crtgroup", new String[] { gid, gname, super });
            }
            SqlserverDB.Free(sdr);

            // 获取好友列表
            Dictionary<String, bool> dict = new Dictionary<string, bool>();
            sdr = SqlserverDB.Query("select id,male from clients where id in (select fndid from relations where groupid in(select id from groups where crtid='" + id + "'))");
            while (sdr.Read())
            {
                String fid = sdr.GetString(0);
                bool fmale = sdr.GetBoolean(1);
                dict[fid] = fmale;
            }
            SqlserverDB.Free(sdr);

            // 获取添加好友请求
            sdr = SqlserverDB.Query("select * from request where desid='" + id + "'");
            while (sdr.Read())
                SendMessage(id, "cnffrd", new String[] { sdr.GetString(0), sdr.GetString(4), sdr.GetInt32(2).ToString(), sdr.GetString(3) });
            SqlserverDB.Free(sdr);

            // 清理离线好友请求
            SqlserverDB.Update("delete from request where desid='" + id + "'");

            // 获取好友信息
            sdr = SqlserverDB.Query("select * from relations where groupid in(select id from groups where crtid='" + id + "')");
            while (sdr.Read())
            {
                String gid,fid, fname;
                gid = sdr.GetInt32(0).ToString();
                fid = sdr.GetString(1);
                fname = sdr.GetString(2);

                SendMessage(id, "crtfrd", new String[] { fid, gid, fname, dict[fid].ToString() });
                SendMessage(id, "frdstt", new String[] { fid, Program.onlineClients.ContainsKey(fid).ToString() });
            }
            SqlserverDB.Free(sdr);

            // 获取离线消息与图片
            sdr = SqlserverDB.Query("select * from messages where readed=0 and desid='" + id + "'");
            while (sdr.Read())
            {
                byte[] b = (byte[])sdr["msg"];
                string Type = (string)sdr["type"];
                SendMessage(id, "recv" + Type, new string[] { sdr["scrid"].ToString(), Base64Change.Base64ToString(UTF8Encoding.UTF8.GetString(b)), sdr["cdate"].ToString() });
            }
            SqlserverDB.Free(sdr);
            SqlserverDB.Update("update messages set readed=1 where desid='" + id + "'");
            #endregion
            /*#region 离线文件
            sdr = SqlserverDB.Query("select scrid,msg,fname from msgfile where desid='" + id + "'");
            while (sdr.Read())
            {
                Random rnd = new Random();
                int next = rnd.Next();
                while (Program.fileClip.ContainsKey(next))
                    next = rnd.Next();
                Program.fileClip[next] = UTF8Encoding.UTF8.GetString((byte[])sdr["msg"]);
                SendMessage(id, "cnffsnd", new string[] { sdr["scrid"].ToString(), sdr["fname"].ToString(), next.ToString() });
            }
            SqlserverDB.Free(sdr);

            SqlserverDB.Update("delete from msgfile where desid='" + id + "'");
            #endregion*/
        }

        #region
        void SendMessage(string destId, string key, string[] data)
        {
            Socket socket = GetUserSocket(destId);
            if (socket == null)
                return;
            string[] vect = new string[data.Length + 1];
            vect[0] = key;
            for (int i = 0; i < data.Length; i++)
                vect[i + 1] = data[i];
            string str = MsgPackager.Packager(vect);
            socket.Send(SafeEncoder.Base64ToBytes(str));
        }

        Socket GetUserSocket(String usrid)
        {
            if (Program.onlineClients.ContainsKey(usrid))
                return Program.onlineClients[usrid];
            else
                return null;
        }

        public void Execute()
        {
            try
            {
                mq = new MsgQueue(this);
                mq.socket = socket;
                mq.ServerListen();
            }
            catch (Exception ex)
            {
                Console.WriteLine("[警告:来自 Program.Excute] " + ex.Message);
                if (ex.Message.Contains("time"))
                {
                    Console.WriteLine("[警告:服务器用户并发数已经达到上限，资源不足，请重新启动服务器端程序，或者设置更大的并发性数上限（同时对硬件也要有更高的要求）] ");
                    Environment.Exit(0);
                }
            }
            try
            {
                socket.Close();
            }
            catch (Exception ex)
            {
                Console.WriteLine("[警告:来自 Program.socket.Close()] " + ex.Message);
            }
            ////////////////////////////////////////////////////////////////////////////////////////////////////////
            if (id != null && Program.onlineClients.ContainsKey(id))
            {
                Program.onlineClients.Remove(id);
                SqlDataReader sdr = SqlserverDB.Query("select crtid from groups where id in (select groupid from relations where fndid='" + id + "')");
                while (sdr.Read())
                {
                    String oid=sdr.GetString(0);
                    if (Program.onlineClients.ContainsKey(oid))
                        SendMessage(oid, "frdstt", new String[] { id, "False" });
                }
                SqlserverDB.Free(sdr);
                Console.WriteLine(DateTime.Now.ToString() + "：断开与用户'" + id + "'连接！");
            }
            else
                Console.WriteLine("断开一个无效连接！");
        }
        #endregion
    }

    #region
    class Program
    {
        public static Dictionary<String, Socket> onlineClients = new Dictionary<string, Socket>();

        static void Main(string[] args)
        {
            try
            {
                SqlserverDB.Init();
                Console.WriteLine("\t\t============ 开始监听 ============");

                Thread tPolicy = new Thread(PolicyListen); //943策略监听线程
                tPolicy.Start();

                Thread tMsg = new Thread(MessageListen);
                tMsg.Start();
            }
            catch (Exception ex)
            {
                Console.WriteLine("[警告:来自 Program.Main] " + ex.Message);
            }
        }


        //监听信息请求和发送信息方法
        static void MessageListen()
        {
            try
            {
                Socket socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
                socket.Bind(new IPEndPoint(IPAddress.Any, Interface.port));
                socket.Listen(200);

                while (true)
                {
                    ClientThread ct = new ClientThread(socket.Accept());
                    new Thread(ct.Execute).Start();
                    System.Threading.Thread.Sleep(100);
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine("[警告:来自 Program.MessageListen] " + ex.Message);
            }
        }

        //监听策略请求和发送策略请求方法
        static void PolicyListen()
        {
            try
            {
                Socket policy = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
                policy.Bind(new IPEndPoint(IPAddress.Any, 943));
                policy.Listen(10);

                while (true)
                {
                    if (policy.Blocking)
                    {
                        Socket _policy = policy.Accept();

                        string policyRequestString = "<policy-file-request/>";
                        byte[] b = new byte[policyRequestString.Length];
                        _policy.Receive(b);

                        if (System.Text.Encoding.UTF8.GetString(b, 0, b.Length) == policyRequestString)
                        {
                            string _policyFilePath = Application.StartupPath + "\\PolicyFile.xml";
                            FileStream fs = new FileStream(_policyFilePath, FileMode.Open);
                            int length = (int)fs.Length;
                            byte[] bytes = new byte[length];
                            fs.Read(bytes, 0, length);
                            fs.Close();
                            _policy.Send(bytes, length, SocketFlags.None);
                            Console.WriteLine("协议通过，通信就绪。");
                            _policy.Close();
                        }
                        else
                            Console.WriteLine("非法访问策略端口！");
                    }
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine("[警告:来自 Program.PolicyListen] " + ex.Message);
                Environment.Exit(0);
            }
        }
    }
    #endregion
}
