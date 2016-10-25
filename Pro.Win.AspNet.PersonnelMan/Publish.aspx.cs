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

public partial class Publish : System.Web.UI.Page
{
    protected void Page_Load(object sender, EventArgs e)
    {
        if (Session["logged"] == null)
        {
            lbMsg.Visible = true;
            pMain.Visible = false;
        }
        else
        {
            lbMsg.Visible = false;
            pMain.Visible = true;
        }
    }
    protected void btnSubmit_Click(object sender, EventArgs e)
    {
        if (txtTitle.Text.Length == 0)
            Response.Redirect("MessagePage.aspx?msg=您必须输出发表文章的标题！");
        else if (txtMain.Text.Length == 0)
            Response.Redirect("MessagePage.aspx?msg=请在正文区填写文章内容！");
        else
        {
            SqlServer db=new SqlServer();
            String title = txtTitle.Text, main = txtMain.Text.Replace("\n", "<br>").Replace(" ", "&nbsp;");
            DateTime current=DateTime.Now;
            db.Update("INSERT INTO passage(title,main,usrid,time,checked) VALUES('" + title + "','" + main + "','" + Session["id"] + "','" + current + "',0)");

            if (myfile.PostedFile != null)
            {
                if (myfile.PostedFile.ContentLength > 4096*1024)
                {
                    Response.Redirect("MessagePage.aspx?msg=上传文件大小不能超过4M！");
                    return;
                }
                int id = Int32.Parse(db.QueryString("SELECT id FROM passage WHERE title='" + title + "' AND time='" + current + "'", "id"));
                string fileName = myfile.PostedFile.FileName;
                int i = fileName.LastIndexOf(".");
                string extension = fileName.Substring(i);
                string nFileName = current.DayOfYear.ToString() + myfile.PostedFile.ContentLength.ToString();
                string path = "~\\upload\\" + nFileName + extension;
                myfile.PostedFile.SaveAs(Server.MapPath(path));
                db.Update("INSERT INTO accessory VALUES(" + id + ",'" + path + "','" + fileName + "')");
                Response.Redirect("MessagePage.aspx?msg=文章、附件成功提交，等待审核中。成功审核后文章即刊登入新闻列表！");
            }
            else
                Response.Redirect("MessagePage.aspx?msg=文章成功提交，等待审核中。成功审核后文章即刊登入新闻列表！");
        }
    }
}
