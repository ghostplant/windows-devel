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

public partial class Downloads : System.Web.UI.Page
{
    public const int PageSize = 4; // 分页
    protected void Page_Load(object sender, EventArgs e)
    {
        int page = (Request["page"] == null) ? 1 : Int32.Parse(Request["page"].ToString());
        DownloadList.DataSource = new SqlServer().GetPageView("SELECT [name],[lnkpath] FROM [download]", "download", PageSize, page, prevPage,lbPage, lbTotal, nextPage);
        DownloadList.DataKeyField = "name";
        this.DataBind();
    }
    protected void prevPage_Click(object sender, EventArgs e)
    {
        int Page = Int32.Parse(lbPage.Text) - 1;
        Response.Redirect("Downloads.aspx?page=" + Page.ToString());
    }
    protected void nextPage_Click(object sender, EventArgs e)
    {
        int Page = Int32.Parse(lbPage.Text) + 1;
        Response.Redirect("Downloads.aspx?page=" + Page.ToString());
    }
}
