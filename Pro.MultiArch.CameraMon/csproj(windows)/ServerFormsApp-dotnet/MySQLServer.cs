using System;
using System.Collections.Generic;
using System.Text;

using System.Data;
using MySQLDriverCS;

namespace ServerFormsApp
{
    class MySQLServer
    {
        private static MySQLConnection conn = Open();

        private static MySQLConnection Open()
        {
            try
            {
                conn = new MySQLConnection(new MySQLConnectionString("localhost", "test", "root", "root").AsString);
                conn.Open();
                return conn;
            }
            catch (Exception ex)
            {
                System.Windows.Forms.MessageBox.Show("数据库访问出错："+ex.Message);
                return null;
            }
        }

        public static void Insert(string text)
        {
            lock (conn)
            {
                try
                {
                    MySQLCommand commn;
                    commn = new MySQLCommand("create table if not exists monsqldb (text varchar(1000))", conn);
                    commn.ExecuteNonQuery();
                    commn = new MySQLCommand("insert into monsqldb values('" + text + "')", conn);
                    commn.ExecuteNonQuery();
                }
                catch (Exception ex)
                {
                    System.Windows.Forms.MessageBox.Show("数据库访问出错：" + ex.Message);
                }
            }
        }

        public static List<string> Query()
        {
            List<string> ret = new List<string>();
            lock (conn)
            {
                try
                {
                    MySQLCommand commn;
                    commn = new MySQLCommand("create table if not exists monsqldb (text varchar(1000))", conn);
                    commn.ExecuteNonQuery();
                    commn = new MySQLCommand("select text from monsqldb", conn);
                    MySQLDataReader mdr = commn.ExecuteReaderEx();
                    while (mdr.Read())
                        ret.Add(mdr.GetString(0));
                }
                catch (Exception ex)
                {
                    System.Windows.Forms.MessageBox.Show("数据库访问出错：" + ex.Message);
                }
            }
            return ret;
        }
    }
}
