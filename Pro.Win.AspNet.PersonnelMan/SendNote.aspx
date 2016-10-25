<%@ Page Language="C#" MasterPageFile="~/MasterPage.master" AutoEventWireup="true" CodeFile="SendNote.aspx.cs" Inherits="SendNote" Title="人事管理系统 - 发送消息" %>
<asp:Content ID="Content1" ContentPlaceHolderID="Main_Information" Runat="Server">
    发送方ID：<asp:TextBox ID="txtDestID" runat="server" Width="188px"></asp:TextBox><br />
    <br />
    消息内容：<br />
    <asp:TextBox ID="txtNote" runat="server" Height="552px" TextMode="MultiLine" Width="538px"></asp:TextBox><br />
    <br />
    <asp:Button ID="btnSend" runat="server" OnClick="btnSend_Click" Text="发送" /><br />
</asp:Content>

