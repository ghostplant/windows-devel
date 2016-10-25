<%@ Page ValidateRequest="false" Language="C#" MasterPageFile="~/admin/AdminMasterPage.master" AutoEventWireup="true" CodeFile="CheckNews.aspx.cs" Inherits="admin_CheckNews" Title="人事管理系统 - 审核文章"%>
<asp:Content ID="Content1" ContentPlaceHolderID="ContentPlaceHolder1" Runat="Server">
<div align="center">
    <span>
    </span><span style="font-size: 16pt; color: #00cc00; font-family: 楷体_GB2312">审核文章</span><br />
    <br />
    <asp:GridView ID="GridView1" runat="server" AllowPaging="True"
        AutoGenerateColumns="False" DataKeyNames="id" Width="700px" OnPageIndexChanging="GridView1_PageIndexChanging" OnRowDeleting="GridView1_RowDeleting" OnSelectedIndexChanging="GridView1_SelectedIndexChanging">
        <Columns>
            <asp:BoundField DataField="id" HeaderText="文章ID" InsertVisible="False" ReadOnly="True"
                SortExpression="id" />
            <asp:BoundField DataField="title" HeaderText="标题" SortExpression="title" />
            <asp:BoundField DataField="usrid" HeaderText="作者ID" SortExpression="usrid" />
            <asp:BoundField DataField="time" HeaderText="提交时间" SortExpression="time" />
            <asp:CheckBoxField DataField="checked" HeaderText="通过审核" SortExpression="checked" />
            <asp:CommandField ShowDeleteButton="True" ShowSelectButton="True" />
        </Columns>
        <AlternatingRowStyle BackColor="#FFE0C0" />
        <SelectedRowStyle BackColor="#C0C0FF" />
    </asp:GridView>
    &nbsp;&nbsp;<br />
    <strong><em>
    正文区：<asp:Label ID="Label1" runat="server" Text="Label"></asp:Label></em></strong><br />
    <br />
    &nbsp;&nbsp;
    <asp:TextBox ID="TextBox1" runat="server" Width="700px" Height="300px" ReadOnly="True" TextMode="MultiLine"></asp:TextBox><br />
    <br />
    <asp:Button ID="btnCheck" runat="server" Text="通过审核" OnClick="btnCheck_Click" Enabled="False" /><br />
    <br />
    &nbsp;&nbsp;<br />
    &nbsp;<br />
    <br />
    <br /></div>
</asp:Content>

