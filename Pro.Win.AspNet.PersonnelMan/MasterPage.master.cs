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

public partial class MasterPage : System.Web.UI.MasterPage
{
    protected void BuildComfirm()
    {
        comfBuild.Text="";
        Random r = new Random();
        for (int i = 0; i < 4; i++)
        {
            int j = r.Next() % PublicStatus.validChar.Length;
            comfBuild.Text += PublicStatus.validChar[j];
        }
        Session["preconf"]=Session["conf"];
        Session["conf"]=comfBuild.Text;
    }
    protected void showLogin()
    {
        usrName.Text = Session["name"].ToString();
        msgCount.Text = new SqlServer().QueryString("SELECT count(*) AS total FROM clarification WHERE hasRead=0 AND destid='" + Session["id"] + "'", "total");
        Panel1.Visible = false;
        Panel2.Visible = true;
        Panel3.Visible = true;
        Panel4.Visible = false;
    }
    protected void showLogout()
    {
        msgCount.Text = "0";
        Session["logged"] = null;
        Panel1.Visible = true;
        Panel2.Visible = false;
        Panel3.Visible = false;
        Panel4.Visible = true;
    }
    protected void Page_Load(object sender, EventArgs e)
    {
        PersonList.DataSource = new SqlServer().GetDataView("SELECT TOP 7 name,id FROM [user] WHERE checked=1 AND id IN (SELECT usrid FROM recommend)", "user");
        PersonList.DataKeyField = "name";
        this.DataBind();
        if (Session["logged"] != null && Session["admin"] == null)
            showLogin();
        else
        {
            BuildComfirm();
            showLogout();
        }
    }
    protected void btnLogin_Click(object sender, EventArgs e)
    {
        if (Session["preconf"] != null && Session["preconf"].Equals(logComf.Text))
        {
            String usr = logUser.Text;
            int hash = logPswd.Text.GetHashCode();

            String name,id;
            if (rbUser.Checked)
            {
                name = new SqlServer().QueryString("SELECT name FROM [user] WHERE id='" + usr + "' AND pswdhash=" + hash + " AND checked=1", "name");
                id = new SqlServer().QueryString("SELECT id FROM [user] WHERE id='" + usr + "' AND pswdhash=" + hash + " AND checked=1", "id");
            }
            else
            {
                name = new SqlServer().QueryString("SELECT name FROM [user] WHERE id='" + usr + "' AND pswdhash=" + hash + " AND checked=1 AND id IN (SELECT mgrid FROM department)", "name");
                id = new SqlServer().QueryString("SELECT id FROM [user] WHERE id='" + usr + "' AND pswdhash=" + hash + " AND checked=1 AND id IN (SELECT mgrid FROM department)", "id");
            }
            if (name != null) // 存在用户
            {
                Session["logged"] = true;
                Session["id"] = id;
                Session["name"] = name;
                txtTag.Text = "";
                showLogin();
                if (rbAdmin.Checked)
                {
                    Session["admin"] = true;
                    Response.Redirect("admin/admin.aspx");
                }
                else
                    Response.Redirect("Default.aspx");
            }
            else
                txtTag.Text = "当前用户不存在或者密码不正确！";
        }
        else
            txtTag.Text = "验证码不正确！";
    }
    protected void btnLogout_Click(object sender, EventArgs e)
    {
        Session["logged"]=null;
        BuildComfirm();
        showLogout();
    }
    protected void comfBuild_Click(object sender, EventArgs e)
    {
        BuildComfirm();
    }
    protected void btnRegister_Click(object sender, EventArgs e)
    {
        Response.Redirect("Register.aspx");
    }
    protected void HyperLink3_Click(object sender, EventArgs e)
    {
        Response.Redirect("Default.aspx");
    }
    protected void btnSearch_Click(object sender, EventArgs e)
    {
        Response.Redirect("Default.aspx?key=" + TextSearch.Text);
    }
}
