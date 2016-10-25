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

public partial class admin_AdminDownLoad : System.Web.UI.Page
{
    protected void Page_Load(object sender, EventArgs e)
    {
        if (!IsPostBack)
            getData();
    }

    public void getData()
    {
        this.GridView1.DataSource = new SqlServer().GetDataView("SELECT * FROM [download]","download");
        this.DataBind();
    }

    protected void GridView1_PageIndexChanging(object sender, GridViewPageEventArgs e)
    {
        this.GridView1.PageIndex = e.NewPageIndex;
        getData();
    }
    protected void GridView1_RowDeleting(object sender, GridViewDeleteEventArgs e)
    {
        string DLName = this.GridView1.DataKeys[e.RowIndex][0].ToString();
        new SqlServer().Update("DELETE FROM [download] WHERE [name] = '" + DLName + "'");
        getData();
    }

    protected void Button1_Click(object sender, EventArgs e)
    {
        if (TextBox1.Text.Length == 0 || TextBox2.Text.Length == 0)
            Response.Redirect("~/MessagePage.aspx?msg=请将信息填写完整！");
        else
        {
            try
            {
                new SqlServer().Update("INSERT INTO [download] VALUES('" + TextBox1.Text + "','" + TextBox2.Text + "')");

            }
            catch (Exception ex)
            {
                Response.Redirect("~/MessagePage.aspx?msg="+ex.Message.Replace("\n","<br>"));
            }
            getData();
        }
    }
}
