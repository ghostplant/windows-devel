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

public partial class SendNote : System.Web.UI.Page
{
    protected void Page_Load(object sender, EventArgs e)
    {
        if (Session["logged"] == null)
            Response.Redirect("MessagePage.aspx?msg=您还没有登录！");
    }
    protected void btnSend_Click(object sender, EventArgs e)
    {
        if (Session["logged"] == null)
        {
            Response.Redirect("MessagePage.aspx?msg=您还没有登录！");
            return;
        }
        if (txtDestID.Text.Length == 0 || txtNote.Text.Length == 0)
            Response.Redirect("MessagePage.aspx?msg=请您填写完整 ID 和内容以后再发送！");
        else if (txtNote.Text.Length > 1500)
            Response.Redirect("MessagePage.aspx?msg=发送消息字数限制1500字以内！");
        else if (!PublicStatus.validLetterDigit(txtDestID.Text))
            Response.Redirect("MessagePage.aspx?msg=您填写的对方的ID非法，请检查！");
        else
        {
            SqlServer sqlsvr=new SqlServer();
            if (sqlsvr.QueryString("SELECT id FROM [user] WHERE id='" + txtDestID.Text + "'", "id") == null)
                Response.Redirect("MessagePage.aspx?msg=ID为“" + txtDestID.Text + "”的用户不存在！");
            else
            {
                sqlsvr.Update("INSERT INTO clarification VALUES('" + Session["id"] + "','" + DateTime.Now + "','" + txtNote.Text.Replace("\n", "<br>") + "','" + txtDestID.Text + "',0)");
                Response.Redirect("MessagePage.aspx?msg=您的消息发送成功！");
            }
        }
    }
}
