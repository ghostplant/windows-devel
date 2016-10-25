<%@ Page Language="C#" MasterPageFile="~/MasterPage.master" AutoEventWireup="true" CodeFile="Passage.aspx.cs" Inherits="Passage" Title="人事管理系统 - 新闻资讯" %>
<asp:Content ID="Content1" ContentPlaceHolderID="Main_Information" Runat="Server">
<p align="center">
    &nbsp;</p>
    <h3><p align="center">
        <asp:Label ID="lbTitle" runat="server" Text="Label"></asp:Label>&nbsp;</p></h3>
    <p align="right">
        作者：<asp:Label ID="lbAuthor" runat="server" Text="Label"></asp:Label>&nbsp;
        &nbsp;
        发表时间：<asp:Label ID="lbDate" runat="server" Text="Label"></asp:Label>&nbsp;</p>
<p>
    <asp:Label ID="lbMain" runat="server" Text="Label"></asp:Label>&nbsp;</p>
    <p>
        附件：
        <asp:HyperLink ID="HyperLink1" runat="server">无</asp:HyperLink></p>
</asp:Content>

