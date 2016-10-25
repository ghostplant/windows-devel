<%@ Page Language="C#" MasterPageFile="~/MasterPage.master" AutoEventWireup="true" CodeFile="Default.aspx.cs" Inherits="_Default" Title="人事管理系统 - 首页" %>
<asp:Content ID="Main" ContentPlaceHolderID="Main_Information" Runat="Server">
    <strong><span style="color: #cc0099">[最新资讯]</span></strong><br />
    <br />
    <div class="MAIN">
        <asp:Repeater ID="repPsgTitle" runat="server">
            <HeaderTemplate>
            </HeaderTemplate>
            <ItemTemplate>
            最新文章：<a href='<%# "Passage.aspx?title="+Eval("id") %>'><%# Eval("title") %></a>
            <br /><br />
            </ItemTemplate>
            <FooterTemplate>
            </FooterTemplate>
        </asp:Repeater>
        <br />
            <asp:LinkButton ID="prevPage" runat="server" OnClick="prevPage_Click">上一页</asp:LinkButton>
             &nbsp;第&nbsp;<asp:Label ID="lbPage" runat="server" Text="Label"></asp:Label>&nbsp;页/&nbsp;共
            <asp:Label ID="lbTotal" runat="server" Text="Label"></asp:Label>
            页&nbsp;<asp:LinkButton ID="nextPage" runat="server" OnClick="nextPage_Click">下一页</asp:LinkButton>
        <br />
    </div>
</asp:Content>

