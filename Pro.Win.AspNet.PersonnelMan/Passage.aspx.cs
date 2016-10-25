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

public partial class Passage : System.Web.UI.Page
{
    protected void Page_Load(object sender, EventArgs e)
    {
        if (Request["title"] == null)
        {
            lbMain.Text = "";
            lbTitle.Text = "对不起，您没有选择要打开的文章！";
        }
        else
        {
            SqlServer sqlsvr=new SqlServer();
            int id = Int32.Parse(Request["title"].ToString());
            String query = "SELECT title,time,main,usrid,id FROM passage WHERE checked=1 AND id=" + id;
            if (sqlsvr.QueryString(query, "title") == null)
            {
                lbMain.Text = "";
                lbTitle.Text = "对不起，没有找到相应文章！";
                return;
            }
            lbTitle.Text = sqlsvr.QueryString(query, "title");
            lbDate.Text = sqlsvr.QueryString(query, "time");
            lbMain.Text = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;" + sqlsvr.QueryString(query, "main").Replace("<br>", "<br><br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;");
            String usrid = sqlsvr.QueryString(query, "usrid");
            String pid = sqlsvr.QueryString(query, "id");
            query = "SELECT [name] FROM [user] WHERE checked =1 AND id='" + usrid +"'";
            lbAuthor.Text = sqlsvr.QueryString(query, "name");
            query = "SELECT name,linkpath FROM accessory WHERE psgid=" + pid;
            String name = sqlsvr.QueryString(query, "name");
            if (name != null)
            {
                HyperLink1.Text=name;
                HyperLink1.NavigateUrl = sqlsvr.QueryString(query,"linkpath");
            }
            else
            {
                HyperLink1.NavigateUrl = null;
                HyperLink1.Text = "无";
            }
        }
    }
}
