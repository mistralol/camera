<%@ Page Language="C#" AutoEventWireup="true" CodeBehind="Login.aspx.cs" Inherits="WebUI.Login" %>

<!DOCTYPE html>

<html xmlns="http://www.w3.org/1999/xhtml">
<head runat="server">
    <title></title>
</head>
<body>
    <form id="form1" runat="server" autocomplete="off">
    <div>
        <asp:Login runat="server" ID="login1" DisplayRememberMe="false" DestinationPageUrl="/Users/Default.aspx">

        </asp:Login>
    </div>
    </form>
</body>
</html>
