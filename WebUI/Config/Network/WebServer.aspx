<%@ Page Title="" Language="C#" MasterPageFile="~/Site.Master" AutoEventWireup="true" CodeBehind="WebServer.aspx.cs" Inherits="WebUI.Config.Network.WebServer" %>
<asp:Content ID="Content1" ContentPlaceHolderID="head" runat="server">
</asp:Content>
<asp:Content ID="Content2" ContentPlaceHolderID="ContentPlaceHolder1" runat="server">

    <table>
        <tr>
            <td>Web Server Port</td>
            <td><asp:TextBox runat="server" ID="txtPort"></asp:TextBox></td>
            <td>
                <asp:RequiredFieldValidator runat="server" ID="valPort" ControlToValidate="txtPort" ErrorMessage="Port is required"></asp:RequiredFieldValidator>
            </td>
        </tr>
        <tr>
            <td></td>
            <td>
                <asp:Button runat="server" ID="btnSave" Text="Save" onclick="btnSave_Click" />
                <asp:Button runat="server" ID="btnCancel" Text="Cancel" 
                    CausesValidation="false" onclick="btnCancel_Click" />
            </td>
            <td>
                <asp:Label runat="server" ID="lblError"></asp:Label>
            </td>
        </tr>
    </table>

</asp:Content>
