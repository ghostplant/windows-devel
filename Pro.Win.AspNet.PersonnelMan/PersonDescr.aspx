<%@ Page Language="C#" MasterPageFile="~/MasterPage.master" AutoEventWireup="true" CodeFile="PersonDescr.aspx.cs" Inherits="PersonDescr" Title="职员简介" %>
<asp:Content ID="Content1" ContentPlaceHolderID="Main_Information" Runat="Server">
    <h3><p align="center">人物简介：<asp:Label ID="lbPerson" runat="server" Text="Label"></asp:Label>
        </p></h3>
        <br />
    <asp:Label ID="lbDesc" runat="server" Text="Label"></asp:Label>
</asp:Content>

