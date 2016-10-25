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

public partial class admin_CheckNews : System.Web.UI.Page
{
    protected void Page_Load(object sender, EventArgs e)
    {
        if (!IsPostBack)
            getData();
    }
    public void getData()
    {
        this.GridView1.DataSource = new SqlServer().GetDataView("SELECT * FROM [passage]", "passage");
        this.DataBind();
    }

    protected void GridView1_PageIndexChanging(object sender, GridViewPageEventArgs e)
    {
        this.GridView1.PageIndex = e.NewPageIndex;
        getData();
    }
    protected void GridView1_RowDeleting(object sender, GridViewDeleteEventArgs e)
    {
        string psgID = this.GridView1.DataKeys[e.RowIndex][0].ToString();
        new SqlServer().Update("DELETE FROM [passage] WHERE [id] = '" + psgID + "'");
        String rev = new SqlServer().QueryString("SELECT [linkpath] FROM [accessory] WHERE psgid='" + psgID + "'", "linkpath");
        if (rev != null)
        {
            System.IO.File.Delete(Server.MapPath(rev));
            new SqlServer().Update("DELETE FROM [accessory] WHERE [psgid]='" + psgID + "'");
        }
        getData();
    }
    protected void GridView1_SelectedIndexChanging(object sender, GridViewSelectEventArgs e)
    {
        String PsgID=this.GridView1.DataKeys[e.NewSelectedIndex][0].ToString();
        String main = new SqlServer().QueryString("SELECT main FROM [passage] WHERE [id]='" + PsgID + "'", "main");
        TextBox1.Text = main.Replace("<br>", "\n").Replace("&nbsp;", " ");
        bool check = Boolean.Parse(new SqlServer().QueryString("SELECT checked FROM [passage] WHERE [id]='" + PsgID + "'", "checked"));
        btnCheck.Enabled = !check;
        Label1.Text = PsgID;
    }
    protected void btnCheck_Click(object sender, EventArgs e)
    {
        btnCheck.Enabled = false;
        new SqlServer().Update("UPDATE [passage] SET checked=1 WHERE id='"+Label1.Text+"'");
        getData();
    }
}
