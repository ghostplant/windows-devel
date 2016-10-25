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

public partial class PersonDescr : System.Web.UI.Page
{
    protected void Page_Load(object sender, EventArgs e)
    {
        if (Request["id"] != null)
        {
            SqlServer sqlsvr = new SqlServer();
            String query = "SELECT name,descr FROM [user] WHERE checked=1 AND id='" + Request["id"] + "'";
            lbPerson.Text=sqlsvr.QueryString(query,"name");
            lbDesc.Text = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;" + sqlsvr.QueryString(query, "descr").Replace("<br>", "<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;");
        }
        else
            Response.Redirect("WorkerList.aspx");
    }
}
