<%@ Page Title="" Language="C#" MasterPageFile="~/Site.Master" AutoEventWireup="true" CodeBehind="ChangePassword.aspx.cs" Inherits="WebUI.Users.ChangePassword" %>

<asp:Content ID="Content1" ContentPlaceHolderID="head" runat="server">
</asp:Content>

<asp:Content ID="Content2" ContentPlaceHolderID="ContentPlaceHolder1" runat="server">

    <asp:Panel runat="server" ID="pnlStart">
        <asp:ChangePassword runat="server" ID="chgPassword" OnChangedPassword="chgPassword_ChangedPassword" OnChangePasswordError="chgPassword_ChangePasswordError">
        
        </asp:ChangePassword>
    </asp:Panel>

    <asp:Panel runat="server" ID="pnlComplete" Visible="false">
        Your password has been changed successfully
    </asp:Panel>

    <asp:Panel runat="server" ID="pnlError" Visible="false">
        Failed to change password<br />
        <br />
        <asp:Button runat="server" ID="btnTryAgain" Text="Try Again" OnClick="btnTryAgain_Click" />
    </asp:Panel>

</asp:Content>

