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

public partial class admin_AdminMasterPage : System.Web.UI.MasterPage
{
    protected void Page_Load(object sender, EventArgs e)
    {
        if (Session["logged"]==null && Session["admin"] == null)
        {
            Response.Redirect("../MessagePage.aspx?msg=您没有权限查看该页面！");
        }
        lbAdmin.Text = Session["id"].ToString();
    }
}
