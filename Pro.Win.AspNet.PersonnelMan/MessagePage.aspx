<%@ Page validateRequest=false Language="C#" AutoEventWireup="true" CodeFile="MessagePage.aspx.cs" Inherits="MessagePage" %>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" >
<head runat="server">
    <title>消息提示</title>
</head>
<body>
    <form id="form1" runat="server">
    <div style="text-align: center">
        <br />
        <asp:Label ID="msg" runat="server" Text="您的操作非法！"></asp:Label><br />
        <br /><a href="javascript:window.history.go(-1)">返回</a></div>
    </form>
</body>
</html>
