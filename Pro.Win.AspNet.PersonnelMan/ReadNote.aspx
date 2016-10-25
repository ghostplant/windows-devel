<%@ Page Language="C#" MasterPageFile="~/MasterPage.master" AutoEventWireup="true" CodeFile="ReadNote.aspx.cs" Inherits="ReadNote" Title="人事管理系统 - 查看消息" %>
<asp:Content ID="Content1" ContentPlaceHolderID="Main_Information" Runat="Server">
    <asp:Repeater ID="repNote" runat="server">
    <ItemTemplate>
    <strong>发件人ID：</strong><asp:Label ID="lbFrom" runat="server" Text='<%# Eval("orgid") %>'></asp:Label>&nbsp; <strong>发送消息时间：</strong><asp:Label
        ID="lbTime" runat="server" Text='<%# Eval("time") %>'></asp:Label>
    &nbsp; &nbsp;<strong><asp:LinkButton ID='lbRead' ToolTip='<%# Eval("time")+", "+ Eval("orgid") %>' runat="server" Enabled='<%# Eval("hasRead").Equals(false)?true:false %>' OnClick="lbRead_Click">已阅读</asp:LinkButton></strong><br />
    <br />
    <strong>消息内容：</strong><br /><br />
    <asp:Label ID="txtTemplate" runat="server" Text='<%# Eval("main") %>'>
    </asp:Label>
    <hr />
    <br />
    <br />
    </ItemTemplate>
    </asp:Repeater>
</asp:Content>

