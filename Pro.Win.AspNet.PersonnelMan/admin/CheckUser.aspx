<%@ Page ValidateRequest="false" Language="C#" MasterPageFile="~/admin/AdminMasterPage.master" AutoEventWireup="true" CodeFile="CheckUser.aspx.cs" Inherits="admin_CheckUser" Title="人事管理系统 - 审核账号" %>
<asp:Content ID="Content1" ContentPlaceHolderID="ContentPlaceHolder1" Runat="Server">
<div align="center">
    <span style="font-size: 16pt; color: #00cc00; font-family: 楷体_GB2312">审核账号<br />
        <br />
    </span>
    <asp:GridView ID="GridView1" runat="server" AllowPaging="True" AllowSorting="True"
        AutoGenerateColumns="False" DataKeyNames="id" DataSourceID="SqlCR" Width="1000px" OnSelectedIndexChanging="GridView1_SelectedIndexChanging">
        <Columns>
            <asp:CommandField ShowDeleteButton="True" ShowEditButton="True" ShowSelectButton="True" />
            <asp:BoundField DataField="id" HeaderText="账号ID" ReadOnly="True" SortExpression="id" />
            <asp:BoundField DataField="name" HeaderText="昵称" SortExpression="name" />
            <asp:BoundField DataField="depid" HeaderText="部门ID" SortExpression="depid" />
            <asp:CheckBoxField DataField="gender" HeaderText="男性" SortExpression="gender" />
            <asp:BoundField DataField="phone" HeaderText="联系电话" SortExpression="phone" />
            <asp:BoundField DataField="birth" HeaderText="出生年份" SortExpression="birth" />
            <asp:BoundField DataField="salary" HeaderText="工资" SortExpression="salary" />
            <asp:BoundField DataField="pswdhash" HeaderText="密码Hash值" SortExpression="pswdhash" />
            <asp:CheckBoxField DataField="checked" HeaderText="通过审核" SortExpression="checked" />
        </Columns>
        <AlternatingRowStyle BackColor="#FFE0C0" />
        <SelectedRowStyle BackColor="#C0C0FF" />
    </asp:GridView>
    <br />
    <strong>简介：</strong>&nbsp;<br />
    <asp:TextBox ID="TextBox1" runat="server" Height="87px" ReadOnly="True" TextMode="MultiLine"
        Width="691px"></asp:TextBox><br />
    <br />
    <asp:SqlDataSource ID="SqlCR" runat="server" ConnectionString="<%$ ConnectionStrings:ConnectionString %>"
        DeleteCommand="DELETE FROM [user] WHERE [id] = @id" InsertCommand="INSERT INTO [user] ([id], [name], [depid], [gender], [phone], [birth], [descr], [salary], [checked], [pswdhash]) VALUES (@id, @name, @depid, @gender, @phone, @birth, @descr, @salary, @checked, @pswdhash)"
        SelectCommand="SELECT * FROM [user]" UpdateCommand="UPDATE [user] SET [name] = @name, [depid] = @depid, [gender] = @gender, [phone] = @phone, [birth] = @birth, [descr] = @descr, [salary] = @salary, [checked] = @checked, [pswdhash] = @pswdhash WHERE [id] = @id">
        <DeleteParameters>
            <asp:Parameter Name="id" Type="String" />
        </DeleteParameters>
        <UpdateParameters>
            <asp:Parameter Name="name" Type="String" />
            <asp:Parameter Name="depid" Type="String" />
            <asp:Parameter Name="gender" Type="Boolean" />
            <asp:Parameter Name="phone" Type="String" />
            <asp:Parameter Name="birth" Type="Int32" />
            <asp:Parameter Name="descr" Type="String" />
            <asp:Parameter Name="salary" Type="Double" />
            <asp:Parameter Name="checked" Type="Boolean" />
            <asp:Parameter Name="pswdhash" Type="Int32" />
            <asp:Parameter Name="id" Type="String" />
        </UpdateParameters>
        <InsertParameters>
            <asp:Parameter Name="id" Type="String" />
            <asp:Parameter Name="name" Type="String" />
            <asp:Parameter Name="depid" Type="String" />
            <asp:Parameter Name="gender" Type="Boolean" />
            <asp:Parameter Name="phone" Type="String" />
            <asp:Parameter Name="birth" Type="Int32" />
            <asp:Parameter Name="descr" Type="String" />
            <asp:Parameter Name="salary" Type="Double" />
            <asp:Parameter Name="checked" Type="Boolean" />
            <asp:Parameter Name="pswdhash" Type="Int32" />
        </InsertParameters>
    </asp:SqlDataSource>
    <br />
    <br /></div>
</asp:Content>

