/*
 * 创建时间：..
 * 最后修改时间：2012.3.30
 * 负责人：崔嵬
 */

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Data.SqlClient;
using System.IO;

namespace IMServer
{
    class SqlserverDB
    {
        public static void Init()
        {
        }

        public static string LoadConnString()
        {
            return "Data Source=202.119.81.70;Initial Catalog=imsqldb;Persist Security Info=True;User ID=root;Password=root";
        }

        public static string connString = LoadConnString();

        public static SqlDataReader Query(String sql)
        {
            SqlConnection conn = new SqlConnection(connString);
            conn.Open();
            SqlCommand cmd = new SqlCommand(sql, conn);
            SqlDataReader sdr =  cmd.ExecuteReader();
            relate[sdr]=conn;
            return sdr;
        }

        public static int Update(String sql)
        {
            SqlConnection conn = new SqlConnection(connString);
            conn.Open();
            SqlCommand cmd = new SqlCommand(sql, conn);
            int ret=cmd.ExecuteNonQuery();
            conn.Close();
            return ret;
        }

        public static Dictionary<SqlDataReader,SqlConnection> relate=new Dictionary<SqlDataReader,SqlConnection>();

        public static void Free(SqlDataReader sdr)
        {
            sdr.Close();
            relate[sdr].Close();
        }

    }
}
