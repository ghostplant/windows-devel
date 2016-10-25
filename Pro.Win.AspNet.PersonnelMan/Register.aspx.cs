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

public partial class Register : System.Web.UI.Page
{
    protected void Page_Load(object sender, EventArgs e)
    {
        if (this.IsPostBack)
            return;
        if (Session["logged"] == null)
        {
            mUserID.Enabled = true;
            btnModify.Text = "确认注册";
            mUserID.Text = "";
            mNickName.Text = "";
            mDepart.Text = "暂未指定";
            mAuthority.Text = "普通用户";
            mMale.Checked = true;
            mFemale.Checked = false;
            mPhone.Text = "";
            mBirthYear.Text = "";
            mDescription.Text = "";
            mSalary.Text = "暂未指定";
        }
        else
        {
            mUserID.Enabled = false;
            btnModify.Text = "更新资料";
            SqlServer sqlsvr = new SqlServer();
            String query = "SELECT * FROM [user] WHERE checked=1 AND id='" + Session["id"] + "'";
            mUserID.Text = sqlsvr.QueryString(query, "id");
            mNickName.Text = sqlsvr.QueryString(query, "name");
            mDepart.Text = sqlsvr.QueryString("SELECT descr FROM department WHERE name IN (SELECT depid FROM [user] WHERE checked=1 AND id='" + Session["id"] + "')", "descr");
            String authority = sqlsvr.QueryString("SELECT descr FROM department WHERE mgrid='" + Session["id"] + "'", "descr");
            if (authority != null)
                mAuthority.Text = "管理员";
            else
                mAuthority.Text = "普通用户";
            bool male = Boolean.Parse(sqlsvr.QueryString(query, "gender"));
            mMale.Checked = male;
            mFemale.Checked = !male;
            mPhone.Text = sqlsvr.QueryString(query, "phone");
            mBirthYear.Text = sqlsvr.QueryString(query, "birth");
            mDescription.Text = sqlsvr.QueryString(query, "descr").Replace("<br>","\n");
            mSalary.Text = sqlsvr.QueryString(query, "salary");
            if (mSalary.Text.Equals("0"))
                mSalary.Text = "暂未指定";
        }
    }
    protected void btnModify_Click(object sender, EventArgs e)
    {
        int error = 0;
        if (mUserID.Text.Length > 0 && mUserID.Text.Length <= 10 && PublicStatus.validLetterDigit(mUserID.Text))
            conUserID.Visible = false;
        else
        {
            conUserID.Visible = true;
            error++;
        }
        Session["userid"]=mUserID.Text;
        if (mNickName.Text.Length > 0 && mNickName.Text.Length <= 10)
            conNickName.Visible = false;
        else
        {
            conNickName.Visible = true;
            error++;
        }
        Session["nickname"] = mNickName.Text;
        if (mPhone.Text.Length > 5 && mPhone.Text.Length <= 15 && PublicStatus.validDigit(mPhone.Text))
            conPhone.Visible = false;
        else
        {
            conPhone.Visible = true;
            error++;
        }
        Session["phone"] = mPhone.Text;
        if (mBirthYear.Text.Length>0 && PublicStatus.validDigit(mBirthYear.Text) && Int32.Parse(mBirthYear.Text) >= 1900 && Int32.Parse(mBirthYear.Text) <= 2010)
            conBirth.Visible = false;
        else
        {
            conBirth.Visible = true;
            error++;
        }
        Session["birth"] = mBirthYear.Text;
        if (mDescription.Text.Length <= 6000)
            conDescr.Visible = false;
        else
        {
            conDescr.Visible = true;
            error++;
        }
        Session["descr"] = mDescription.Text;
        if (mPassword.Text.Length > 0 && mPassword.Text.Length <= 10 && PublicStatus.validLetterDigit(mPassword.Text))
            conPass.Visible = false;
        else
        {
            conPass.Visible = true;
            error++;
        }
        if (mPassword.Text.Equals(mRepeat.Text))
            conRep.Visible = false;
        else
        {
            conRep.Visible = true;
            error++;
        }
        if (error == 0)
        {
            bool succ = false;
            try
            {
                if (Session["logged"] != null)
                {
                    new SqlServer().Update("UPDATE [user] SET name='" + mNickName.Text + "',gender=" + (mMale.Checked ? 1 : 0) + ",phone='" + mPhone.Text + "',birth=" + mBirthYear.Text + ",descr='" + mDescription.Text.Replace("\n", "<br>") + "',pswdhash=" + mPassword.Text.GetHashCode() + " WHERE checked=1 AND id='" + Session["id"] + "'");
                    succ = true;
                    Response.Redirect("MessagePage.aspx?msg=资料更新成功！");
                }
                else
                {
                    if (new SqlServer().QueryString("SELECT id FROM [user] WHERE checked=1 AND id='" + mUserID.Text + "'", "id") != null)
                    {
                        succ = true;
                        Response.Redirect("MessagePage.aspx?msg=该 ID 已经被使用，请更换 ID ！");
                    }
                    else
                    {
                        new SqlServer().Update("INSERT INTO [user] VALUES('" + mUserID.Text + "','" + mNickName.Text + "'," + (mDepart.Text.Equals("暂未指定") ? "NULL" : ("'" + mDepart.Text + "'")) + "," + (mMale.Checked ? 1 : 0) + ",'" + mPhone.Text + "'," + mBirthYear.Text + ",'" + mDescription.Text.Replace("\n", "<br>") + "',0,0," + mPassword.Text.GetHashCode() + ")");
                        succ = true;
                        Response.Redirect("MessagePage.aspx?msg=注册成功！请等待审核成功……");
                    }
                }
            }
            catch (Exception ex)
            {
                if (!succ)
                    Response.Redirect("MessagePage.aspx?msg=您的操作并未成功执行！" + ex.Message);
            }
        }
    }
}
