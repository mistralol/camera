<%@ Page Language="C#" Inherits="WebUI.Default" %>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html>
<head runat="server">
	<title>Default</title>
</head>
<body>
	<form id="form1" runat="server">
		<asp:Label runat="server" id="lblInfo"></asp:Label>
		<br/>
		<br/>
		<asp:Button id="button1" runat="server" Text="Click me!" OnClick="button1Clicked" />
		<br/>
		<asp:Label runat="server" id="lblPing"></asp:Label>
	</form>
</body>
</html>
