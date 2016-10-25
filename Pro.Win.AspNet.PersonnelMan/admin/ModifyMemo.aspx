<%@ Page Language="C#" MasterPageFile="~/admin/AdminMasterPage.master" AutoEventWireup="true" CodeFile="ModifyMemo.aspx.cs" Inherits="admin_ModifyMemo" Title="人事管理系统 - 修改公司简介" %>
<asp:Content ID="Content1" ContentPlaceHolderID="ContentPlaceHolder1" Runat="Server">
<div align="center">
    <span style="font-size: 16pt; color: #00cc00; font-family: 楷体_GB2312">修改公司简介<br />
        <br />
        <asp:TextBox ID="TextBox1" runat="server" Height="407px" TextMode="MultiLine" Width="734px"></asp:TextBox><br />
        <br />
        <asp:Button ID="Button1" runat="server" OnClick="Button1_Click" Text="确认修改" /><br />
    </span></div>
</asp:Content>

