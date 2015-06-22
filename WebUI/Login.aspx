<%@ Page Language="C#" AutoEventWireup="true" CodeBehind="Login.aspx.cs" Inherits="WebUI.Login" %>

<!DOCTYPE html>

<html xmlns="http://www.w3.org/1999/xhtml">
<head runat="server">
    <title>Login</title>
</head>
<body>
    <style type="text/css">
        * {
            margin: 0;
            padding: 0;
        }

        #header {
            background-color: #808080;
            width: 100%;
            height: 50px;
            text-align: center;
            vertical-align: central;
        }

        #spacer {
            width: 100%;
            height: 50px;
        }

        #middle {
            width: 400px;
            margin-left: auto;
            margin-right: auto;
        }

    </style>

    <form id="form1" runat="server" autocomplete="off">
    <div id="header">
        Header
    </div>

    <div id="spacer">

    </div>

    <div id="middle">
        <asp:Login runat="server" ID="login1" DisplayRememberMe="false" DestinationPageUrl="/Users/Default.aspx">

        </asp:Login>
    </div>

    </form>
</body>
</html>
