<%@ Page Language="C#" MasterPageFile="~/admin/AdminMasterPage.master" AutoEventWireup="true" CodeFile="AdminDownLoad.aspx.cs" Inherits="admin_AdminDownLoad" Title="人事管理系统 - 下载管理" %>
<asp:Content ID="Content1" ContentPlaceHolderID="ContentPlaceHolder1" Runat="Server">
<div align="center">
    <span style="font-size: 16pt; color: #00cc00; font-family: 楷体_GB2312">
    下载管理</span><br />
    <br />
    <asp:GridView ID="GridView1" runat="server" AllowPaging="True"
        AutoGenerateColumns="False" DataKeyNames="name" OnPageIndexChanging="GridView1_PageIndexChanging" OnRowDeleting="GridView1_RowDeleting">
        <Columns>
            <asp:BoundField DataField="name" HeaderText="下载名称" ReadOnly="True" SortExpression="name" />
            <asp:BoundField DataField="lnkpath" HeaderText="链接地址" SortExpression="lnkpath" />
            <asp:CommandField ShowDeleteButton="True" />
        </Columns>
        <AlternatingRowStyle BackColor="#FFE0C0" />
        <SelectedRowStyle BackColor="#C0C0FF" />
    </asp:GridView>
    &nbsp;&nbsp;<br />
    下载名称：<asp:TextBox ID="TextBox1" runat="server"></asp:TextBox><br />
    链接地址：<asp:TextBox ID="TextBox2" runat="server"></asp:TextBox><br />
    <br />
    <asp:Button ID="Button1" runat="server" OnClick="Button1_Click" Text="添加" /><br />
    &nbsp;<br />
    <br /></div>
</asp:Content>

