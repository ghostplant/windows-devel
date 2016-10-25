<%@ Page Language="C#" MasterPageFile="~/MasterPage.master" AutoEventWireup="true" CodeFile="Register.aspx.cs" Inherits="Register" Title="人事管理系统 - 修改资料" %>
<asp:Content ID="Content1" ContentPlaceHolderID="Main_Information" Runat="Server">
    <br />
    <table border="0" cellspacing="0" style="width: 563px; height: 586px">
        <tr>
            <td style="width: 87px">
    用户ID：</td>
            <td style="width: 102px">
                <asp:TextBox ID="mUserID" runat="server" Width="150px"></asp:TextBox>&nbsp;<asp:Label
                    ID="conUserID" runat="server" ForeColor="Red" Text="只能包含字母、数字，限制1~10字符！" Visible="False"></asp:Label></td>
        </tr>
        <tr>
            <td style="width: 87px">
    昵称：</td>
            <td style="width: 102px">
                <asp:TextBox ID="mNickName" runat="server" Width="150px"></asp:TextBox>
                <asp:Label ID="conNickName" runat="server" ForeColor="Red" Text="限制1~10字符！" Visible="False"></asp:Label></td>
        </tr>
        <tr>
            <td style="width: 87px">
    所在部门：</td>
            <td style="width: 102px">
                <asp:Label ID="mDepart" runat="server" Text="暂未指定"></asp:Label></td>
        </tr>
        <tr>
            <td style="width: 87px">
    用户身份：</td>
            <td style="width: 102px">
                <asp:Label ID="mAuthority" runat="server" Text="普通用户"></asp:Label></td>
        </tr>
        <tr>
            <td style="width: 87px">
    性别：</td>
            <td style="width: 102px">
                <asp:RadioButton ID="mMale" runat="server" Checked="True" GroupName="gender" Text="男" />
                &nbsp;&nbsp;
                <asp:RadioButton ID="mFemale" runat="server" GroupName="gender" Text="女" /></td>
        </tr>
        <tr>
            <td style="width: 87px">
    联系电话：</td>
            <td style="width: 102px">
                <asp:TextBox ID="mPhone" runat="server" Width="150px"></asp:TextBox>
                <asp:Label ID="conPhone" runat="server" ForeColor="Red" Text="只能包含数字，限制5~15个数字！"
                    Visible="False"></asp:Label></td>
        </tr>
        <tr>
            <td style="width: 87px">
    出生年份：</td>
            <td style="width: 102px">
                <asp:TextBox ID="mBirthYear" runat="server" Width="150px"></asp:TextBox>
                <asp:Label ID="conBirth" runat="server" ForeColor="Red" Text="年份范围必须在1900~2010之间！"
                    Visible="False"></asp:Label></td>
        </tr>
        <tr>
            <td style="width: 87px">
    个人描述：</td>
            <td style="width: 102px; text-align: center;">
                <asp:TextBox ID="mDescription" runat="server" Height="155px" TextMode="MultiLine" Width="450px"></asp:TextBox><br />
                <asp:Label ID="conDescr" runat="server" ForeColor="Red" Text="限制字数最多6000字！" Visible="False"></asp:Label></td>
        </tr>
        <tr>
            <td style="width: 87px">
    月薪：</td>
            <td style="width: 102px">
                <asp:Label ID="mSalary" runat="server" Text="暂未指定"></asp:Label></td>
        </tr>
        <tr>
            <td style="width: 87px">
                新密码：</td>
            <td style="width: 102px">
                <asp:TextBox ID="mPassword" runat="server" Width="150px" TextMode="Password"></asp:TextBox>
                <asp:Label ID="conPass" runat="server" ForeColor="Red" Text="只能包含字母、数字，限制1~10字符！"
                    Visible="False"></asp:Label></td>
        </tr>
        <tr>
            <td style="width: 87px;">
    确认密码：</td>
            <td style="width: 102px;">
                <asp:TextBox ID="mRepeat" runat="server" Width="150px" TextMode="Password"></asp:TextBox>
                <asp:Label ID="conRep" runat="server" ForeColor="Red" Text="两次密码输入不匹配！"
                    Visible="False"></asp:Label></td>
        </tr>
    </table>
    <br />
    <br />
    <p align="center"><asp:Button ID="btnModify" runat="server" OnClick="btnModify_Click" /><br /></p>
    <br />
</asp:Content>

