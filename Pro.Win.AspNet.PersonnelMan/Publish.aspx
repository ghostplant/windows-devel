<%@ Page Language="C#" MasterPageFile="~/MasterPage.master" AutoEventWireup="true" CodeFile="Publish.aspx.cs" Inherits="Publish" Title="人事管理系统 - 发表文章" %>
<asp:Content ID="Content1" ContentPlaceHolderID="Main_Information" Runat="Server">
    <asp:Label ID="lbMsg" runat="server" Text="对不起，您尚未登录!"></asp:Label><br />
    <asp:Panel ID="pMain" runat="server" Height="148px" Width="567px">
        标题：<asp:TextBox ID="txtTitle" runat="server" Width="343px"></asp:TextBox><br />
        <br />
        内容：<br />
        <asp:TextBox ID="txtMain" runat="server" Height="388px" TextMode="MultiLine" Width="539px"></asp:TextBox><br />
        <br />
        附件：<input type="file" runat="server" id="myfile">&nbsp;<br />
        <br />
        <asp:Button ID="btnSubmit" runat="server" Text="提交" OnClick="btnSubmit_Click" /><br />
        <p align="center">
            <br />
            &nbsp;</p>
    </asp:Panel>
</asp:Content>

