<%@ Page Language="C#" MasterPageFile="~/MasterPage.master" AutoEventWireup="true" CodeFile="Downloads.aspx.cs" Inherits="Downloads" Title="人事管理系统 - 下载专区" %>
<asp:Content ID="ContentDownloads" ContentPlaceHolderID="Main_Information" Runat="Server">
    <asp:DataList ID="DownloadList" runat="server">
        <ItemTemplate>
            [资源下载] &nbsp;
            <asp:HyperLink ID="nameLabel" runat="server" Text='<%# Eval("name") %>' NavigateUrl='<%# Eval("lnkpath") %>'>'>ss</asp:HyperLink><br />
            <br />
        </ItemTemplate>
    </asp:DataList><br />
            <asp:LinkButton ID="prevPage" runat="server" OnClick="prevPage_Click">上一页</asp:LinkButton>
             &nbsp;第&nbsp;<asp:Label ID="lbPage" runat="server" Text="Label"></asp:Label>&nbsp;页/&nbsp;共
            <asp:Label ID="lbTotal" runat="server" Text="Label"></asp:Label>
            页&nbsp;<asp:LinkButton ID="nextPage" runat="server" OnClick="nextPage_Click">下一页</asp:LinkButton>
</asp:Content>

