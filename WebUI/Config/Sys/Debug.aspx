<%@ Page Title="" Language="C#" MasterPageFile="~/Site.Master" AutoEventWireup="true" CodeBehind="Debug.aspx.cs" Inherits="WebUI.Config.Sys.Debug" %>
<asp:Content ID="Content1" ContentPlaceHolderID="head" runat="server">
</asp:Content>
<asp:Content ID="Content2" ContentPlaceHolderID="ContentPlaceHolder1" runat="server">


    Debug Enabled
    <asp:DropDownList runat="server" ID="ddlDebugEnabled">
        <asp:ListItem Text="Yes" Value="1"></asp:ListItem>
        <asp:ListItem Text="No" Value="0"></asp:ListItem>
    </asp:DropDownList>
    <br />
    <br />

    <asp:Button runat="server" ID="btnSave" Text="Save" onclick="btnSave_Click" />
    <asp:Button runat="server" ID="btnCancel" Text="Cancel" 
        onclick="btnCancel_Click" />

</asp:Content>
