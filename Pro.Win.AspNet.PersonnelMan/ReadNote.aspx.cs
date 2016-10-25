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

public partial class ReadNote : System.Web.UI.Page
{
    protected void Page_Load(object sender, EventArgs e)
    {
        if (Session["logged"] == null)
            Response.Redirect("MessagePage.aspx?msg=您还没有登录！");
        else
        {
            repNote.DataSource=new SqlServer().GetDataView("SELECT * FROM clarification WHERE destid='" + Session["id"] + "' ORDER BY hasRead", "clarification");
            this.DataBind();
        }
    }

    protected void lbRead_Click(object sender, EventArgs e)
    {
        if (Session["logged"] == null)
            Response.Redirect("MessagePage.aspx?msg=您还没有登录！");
        else
        {
            String msg = ((LinkButton)sender).ToolTip;
            int pos = msg.IndexOf(", ");
            String time = msg.Substring(0, pos);
            pos += 2;
            String fromid = msg.Substring(pos, msg.Length - pos);
            new SqlServer().Update("UPDATE [clarification] SET hasRead=1 WHERE time='" + time + "' AND destid='" + Session["id"] + "' AND orgid='" + fromid + "'");
            ((LinkButton)sender).Enabled = false;
        }
    }
}
