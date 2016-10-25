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

public partial class admin_Recommend : System.Web.UI.Page
{
    protected void Page_Load(object sender, EventArgs e)
    {
        if (!IsPostBack)
            getData();
    }

    public void getData()
    {
        this.gridList.DataSource = new SqlServer().GetDataView("SELECT * FROM [recommend]", "recommend");
        this.DataBind();

    }

    protected void gridList_PageIndexChanging(object sender, GridViewPageEventArgs e)
    {
        this.gridList.PageIndex = e.NewPageIndex;
        getData();
    }
    protected void gridList_RowDeleting(object sender, GridViewDeleteEventArgs e)
    {
        string UserID = this.gridList.DataKeys[e.RowIndex][0].ToString();
        new SqlServer().Update("DELETE FROM [recommend] WHERE usrid='" + UserID+"'");
        getData();
    }
    protected void Button1_Click(object sender, EventArgs e)
    {
        String UserID = TextBox1.Text;
        if (UserID.Length <= 0 || UserID.Length > 10)
            Response.Redirect("~/MessagePage.aspx?msg=用户ID长度不在允许范围！");
        else if (!PublicStatus.validLetterDigit(UserID))
            Response.Redirect("~/MessagePage.aspx?msg=用户ID只能包含数字、字母！");
        else
        {
            String valid=new SqlServer().QueryString("SELECT usrid FROM recommend WHERE usrid='" + UserID + "'", "usrid");
            if (valid == null)
            {
                valid = new SqlServer().QueryString("SELECT name FROM [user] WHERE id='" + UserID + "' AND checked=1", "name");
                if (valid == null)
                {
                    Response.Redirect("~/MessagePage.aspx?msg=该用户ID不存在！");
                    return;
                }
                new SqlServer().Update("INSERT INTO recommend VALUES('"+UserID+"')");
                TextBox1.Text = "";
                getData();
            } else
                Response.Redirect("~/MessagePage.aspx?msg=用户ID已经存在与推荐列表中，更新取消！");
        }
    }
}
