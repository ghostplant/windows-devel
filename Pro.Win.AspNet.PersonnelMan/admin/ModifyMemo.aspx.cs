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

public partial class admin_ModifyMemo : System.Web.UI.Page
{
    protected void Page_Load(object sender, EventArgs e)
    {
        if (IsPostBack)
            return;
        TextBox1.Text = new SqlServer().QueryString("SELECT information FROM company", "information").Replace("<br>", "\n").Replace("&nbsp;"," ");
    }
    protected void Button1_Click(object sender, EventArgs e)
    {
        if (TextBox1.Text.Length > 6000)
            Response.Redirect("~/MessagePage.aspx?msg=字数不能超过6000字！");
        else
        {
            new SqlServer().Update("UPDATE company SET information='" + TextBox1.Text.Replace("\n", "<br>").Replace(" ", "&nbsp;") + "'");
            Response.Redirect("~/MessagePage.aspx?msg=公司简介修改成功！");
        }
    }
}
