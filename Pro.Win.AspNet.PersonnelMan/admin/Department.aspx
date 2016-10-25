<%@ Page Language="C#" MasterPageFile="~/admin/AdminMasterPage.master" AutoEventWireup="true" CodeFile="Department.aspx.cs" Inherits="admin_Department" Title="人事管理系统 - 部门管理" %>
<asp:Content ID="Content1" ContentPlaceHolderID="ContentPlaceHolder1" Runat="Server">
    <div align="center">
    <span>
    </span><span></span><span style="font-size: 16pt; color: #00cc00; font-family: 楷体_GB2312">
        部门管理</span><br />
        <br />
    <asp:GridView ID="GridView1" runat="server" AllowPaging="True" AllowSorting="True"
        DataSourceID="SqlDataSource1" AutoGenerateColumns="False" DataKeyNames="name" OnSelectedIndexChanged="GridView1_SelectedIndexChanged">
        <Columns>
            <asp:BoundField DataField="name" HeaderText="部门ID" ReadOnly="True" SortExpression="name" />
            <asp:BoundField DataField="mgrid" HeaderText="管理员ID" SortExpression="mgrid" />
            <asp:BoundField DataField="descr" HeaderText="部门名称" SortExpression="descr" />
            <asp:CommandField ShowDeleteButton="True" ShowEditButton="True" ShowSelectButton="True" />
        </Columns>
        <AlternatingRowStyle BackColor="#FFE0C0" />
        <SelectedRowStyle BackColor="#C0C0FF" />
    </asp:GridView>
        <br />
    <br />
    <asp:DetailsView ID="DetailsView1" runat="server" AutoGenerateRows="False" DataKeyNames="name"
        DataSourceID="SqlDataSource1" Height="50px" Width="125px">
        <Fields>
            <asp:BoundField DataField="name" HeaderText="name" ReadOnly="True" SortExpression="name" />
            <asp:BoundField DataField="mgrid" HeaderText="mgrid" SortExpression="mgrid" />
            <asp:BoundField DataField="descr" HeaderText="descr" SortExpression="descr" />
            <asp:CommandField ShowInsertButton="True" />
        </Fields>
    </asp:DetailsView>
    <br />
    <br />
    <asp:SqlDataSource ID="SqlDataSource1" runat="server" ConnectionString="<%$ ConnectionStrings:ConnectionString %>"
        DeleteCommand="DELETE FROM [department] WHERE [name] = @name" InsertCommand="INSERT INTO [department] ([name], [mgrid], [descr]) VALUES (@name, @mgrid, @descr)"
        SelectCommand="SELECT * FROM [department]" UpdateCommand="UPDATE [department] SET [mgrid] = @mgrid, [descr] = @descr WHERE [name] = @name">
        <DeleteParameters>
            <asp:Parameter Name="name" Type="String" />
        </DeleteParameters>
        <UpdateParameters>
            <asp:Parameter Name="mgrid" Type="String" />
            <asp:Parameter Name="descr" Type="String" />
            <asp:Parameter Name="name" Type="String" />
        </UpdateParameters>
        <InsertParameters>
            <asp:Parameter Name="name" Type="String" />
            <asp:Parameter Name="mgrid" Type="String" />
            <asp:Parameter Name="descr" Type="String" />
        </InsertParameters>
    </asp:SqlDataSource>
    </div>
</asp:Content>

