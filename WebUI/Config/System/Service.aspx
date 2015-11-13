<%@ Page Title="" Language="C#" MasterPageFile="~/Site.Master" AutoEventWireup="true" CodeBehind="Service.aspx.cs" Inherits="WebUI.Config.System.Service" %>
<asp:Content ID="Content1" ContentPlaceHolderID="head" runat="server">
</asp:Content>
<asp:Content ID="Content2" ContentPlaceHolderID="ContentPlaceHolder1" runat="server">

    <asp:Panel runat="server" ID="pnlStart">
        <asp:Button runat="server" ID="btnStopServer" Text="Stop Server" 
            onclick="btnStopServer_Click" /><br />
        <br />
        <asp:Button runat="server" ID="btnShutdown" Text="Shutdown" onclick="btnShutdown_Click" /><br />
        <br />
        <asp:Button runat="server" ID="btnReboot" Text="Reboot" onclick="btnReboot_Click" /><br />
        <br />
    </asp:Panel>

    <asp:Panel runat="server" ID="pnlDone" Visible="false">
        Please Wait
    </asp:Panel>

    <asp:Panel runat="server" ID="pnlError" Visible="false">
        Error: Unable to complete operation
        <asp:Label runat="server" ID="lblError"></asp:Label>
    </asp:Panel>

</asp:Content>
