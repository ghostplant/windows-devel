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

public partial class admin_CheckUser : System.Web.UI.Page
{
    protected void Page_Load(object sender, EventArgs e)
    {

    }
    protected void GridView1_SelectedIndexChanging(object sender, GridViewSelectEventArgs e)
    {
        String ans=new SqlServer().QueryString("SELECT descr FROM [user] WHERE checked=1 AND id='" + GridView1.DataKeys[e.NewSelectedIndex][0].ToString() + "'", "descr");
        TextBox1.Text = ans.Replace("<br>","\n");
    }
}
