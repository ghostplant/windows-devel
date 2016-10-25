using System;
using System.Data;
using System.Data.SqlClient;
using System.Configuration;
using System.Web;
using System.Web.Security;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Web.UI.WebControls.WebParts;
using System.Web.UI.HtmlControls;

/// <summary>
/// SqlServer 的摘要说明
/// </summary>
public class SqlServer
{
    public String QueryString(String sql, String col)
    {
        String ret;
        SqlConnection conn = new SqlConnection(ConfigurationManager.ConnectionStrings["database"].ConnectionString);
        SqlCommand cmd = new SqlCommand();
        cmd.Connection = conn;
        cmd.CommandText = sql;
        conn.Open();
        SqlDataReader dr = cmd.ExecuteReader();
        if (dr.Read())
            ret = dr[col].ToString();
        else
            ret = null;
        dr.Close();
        conn.Close();
        return ret;
    }
    public int Update(String sql)
    {
        SqlConnection conn = new SqlConnection(ConfigurationManager.ConnectionStrings["database"].ConnectionString);
        SqlCommand cmd = new SqlCommand();
        cmd.Connection = conn;
        cmd.CommandText = sql;
        conn.Open();
        int ln = cmd.ExecuteNonQuery();
        conn.Close();
        return ln;
    }
    public DataView GetDataView(String sql, String table)
    {
        SqlConnection conn = new SqlConnection(ConfigurationManager.ConnectionStrings["database"].ConnectionString);
        SqlCommand cmd = new SqlCommand(sql, conn);
        SqlDataAdapter dap = new SqlDataAdapter(cmd);
        DataSet ds = new DataSet();
        dap.Fill(ds, table);
        conn.Close();
        return ds.Tables[0].DefaultView;
    }
    public PagedDataSource PagedDataSource(DataView origin, int PageSize)
    {
        PagedDataSource pds = new PagedDataSource();
        pds.DataSource = origin;
        pds.AllowPaging = true;
        pds.PageSize = PageSize;
        return pds;
    }
    public PagedDataSource GetPageView(String sql, String table, int PageSize, int curPage, LinkButton prev, Label lbCurPage, Label lbTotal, LinkButton next) // ADO.NET
    {
        PagedDataSource pds = PagedDataSource(GetDataView(sql,table), PageSize);
        prev.Enabled = (curPage > 1);
        next.Enabled = (curPage < pds.PageCount);
        lbTotal.Text = pds.PageCount.ToString();
        if (curPage < 1)
            curPage = 1;
        lbCurPage.Text = curPage.ToString();
        pds.CurrentPageIndex = curPage - 1; // 页码 0..PageCount-1
        return pds;
    }
}
