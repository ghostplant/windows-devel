<%@ Page Language="C#" MasterPageFile="~/admin/AdminMasterPage.master" AutoEventWireup="true" CodeFile="Recommend.aspx.cs" Inherits="admin_Recommend" Title="人事管理系统 - 修改人物推荐" %>

<asp:Content ID="Content1" ContentPlaceHolderID="ContentPlaceHolder1" Runat="Server">
<div align="center">
    <span style="font-size: 16pt; color: #00cc00; font-family: 楷体_GB2312">人物推荐<br />
    </span>
        <br />
        <asp:GridView ID="gridList" runat="server" AllowPaging="True" AutoGenerateColumns="False" DataKeyNames="usrid" OnPageIndexChanging="gridList_PageIndexChanging" OnRowDeleting="gridList_RowDeleting">
            <Columns>
                <asp:BoundField DataField="usrid" HeaderText="用户ID" ReadOnly="True" SortExpression="usrid" />
                <asp:CommandField ShowDeleteButton="True" />
            </Columns>
        <AlternatingRowStyle BackColor="#FFE0C0" />
        <SelectedRowStyle BackColor="#C0C0FF" />
        </asp:GridView>
    &nbsp;<br />
    &nbsp;
    <asp:TextBox ID="TextBox1" runat="server"></asp:TextBox>&nbsp;
    <asp:Button ID="Button1" runat="server" OnClick="Button1_Click" Text="添加" /><br />
        <br />
</div>
</asp:Content>


