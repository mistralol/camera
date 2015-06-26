<%@ Page Title="" Language="C#" MasterPageFile="~/Site.Master" AutoEventWireup="true" CodeBehind="UserAdd.aspx.cs" Inherits="WebUI.Admin.UserAdd" %>

<asp:Content ID="Content1" ContentPlaceHolderID="head" runat="server">

</asp:Content>

<asp:Content ID="Content2" ContentPlaceHolderID="ContentPlaceHolder1" runat="server">

    <asp:Panel runat="server" ID="pnlCreate">
        <table>
            <tr>
                <td>Username</td>
                <td><asp:TextBox runat="server" ID="txtUsername"></asp:TextBox></td>
                <td>
                    <asp:RequiredFieldValidator runat="server" ID="valUsername1" ValidationGroup="Create" ControlToValidate="txtUsername" ErrorMessage="* Required"></asp:RequiredFieldValidator>
                    <asp:RegularExpressionValidator runat="server" ID="valUsername2" ValidationGroup="Create" ControlToValidate="txtUsername" ValidationExpression="([a-z]|[A-Z]|[0-9])+" ErrorMessage="* Invalid charaters present"></asp:RegularExpressionValidator>
                </td>
            </tr>
            <tr>
                <td>Password</td>
                <td><asp:TextBox runat="server" ID="txtPassword" TextMode="Password"></asp:TextBox></td>
                <td>
                    <asp:RequiredFieldValidator runat="server" ID="valPassword" ValidationGroup="Create" ControlToValidate="txtPassword" ErrorMessage="* Required"></asp:RequiredFieldValidator>
                </td>
            </tr>
            <tr>
                <td>Confirm Password</td>
                <td><asp:TextBox runat="server" ID="txtPassword2" TextMode="Password"></asp:TextBox></td>
                <td>
                    <asp:RequiredFieldValidator runat="server" ID="txtPassword21" ValidationGroup="Create" ControlToValidate="txtPassword2" ErrorMessage="* Required"></asp:RequiredFieldValidator>
                    <asp:CompareValidator runat="server" ID="txtPassword22" ValidationGroup="Create" ControlToValidate="txtPassword2" ControlToCompare="txtPassword" ErrorMessage="* Password do not match"></asp:CompareValidator>
                </td>
            </tr>

            <tr>
                <td></td>
                <td><asp:Button runat="server" ID="btnCreate" Text="Create User" ValidationGroup="Create" OnClick="btnCreate_Click" /></td>
                <td></td>
            </tr>
        </table>
        <asp:Label runat="server" ID="lblError"></asp:Label>
    </asp:Panel>

    <asp:Panel runat="server" ID="pnlDone" Visible="false">
        Successfully created user <asp:Label runat="server" ID="lblCreatedUser"></asp:Label><br />
        <br />
        <asp:Button runat="server" ID="btnContinue" Text="Continue" OnClick="btnContinue_Click" />
    </asp:Panel>

</asp:Content>

