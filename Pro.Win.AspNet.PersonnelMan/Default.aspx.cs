using System;
using System.Data;
using System.Configuration;
using System.Collections;
using System.Web;
using System.Web.Security;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Web.UI.WebControls.WebParts;
using System.Web.UI.HtmlControls;
using System.Data.SqlClient;

public partial class _Default : System.Web.UI.Page
{
    public const int PageSize = 4; // 分页
    protected void Page_Load(object sender, EventArgs e)
    {
        if (Request["log"]!=null)
        {
            Session["admin"] = null;
            Session["logged"] = null;
        }
        SqlServer sqlsvr=new SqlServer();
        int Page = (Request["page"] == null)?1:Int32.Parse(Request["page"].ToString());
        String key = (Request["key"] == null) ? "" : Request["key"].ToString();
        repPsgTitle.DataSource = sqlsvr.GetPageView("SELECT title,id FROM passage WHERE checked=1 AND main LIKE '%" + key + "%' ORDER BY time DESC", "passge", PageSize, Page, prevPage, lbPage, lbTotal, nextPage);
        this.DataBind();
    }
    protected void nextPage_Click(object sender, EventArgs e)
    {
        int Page = Int32.Parse(lbPage.Text) + 1;
        if (Request["key"]==null)
            Response.Redirect("Default.aspx?page=" + Page.ToString());
        else
            Response.Redirect("Default.aspx?page=" + Page.ToString()+"&key="+Request["key"].ToString());
    }
    protected void prevPage_Click(object sender, EventArgs e)
    {
        int Page = Int32.Parse(lbPage.Text) - 1;
        if (Request["key"] == null)
            Response.Redirect("Default.aspx?page=" + Page.ToString());
        else
            Response.Redirect("Default.aspx?page=" + Page.ToString() + "&key=" + Request["key"].ToString());
    }
}
