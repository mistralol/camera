<%@ Page Title="" Language="C#" MasterPageFile="~/Site.Master" AutoEventWireup="true" CodeBehind="RTSP.aspx.cs" Inherits="WebUI.Config.Network.RTSP" %>

<asp:Content ID="Content1" ContentPlaceHolderID="head" runat="server">
</asp:Content>

<asp:Content ID="Content2" ContentPlaceHolderID="ContentPlaceHolder1" runat="server">

    <table>
        <tr>
            <td>Port</td>
            <td><asp:TextBox runat="server" ID="txtPort"></asp:TextBox></td>
            <td>
                <asp:RequiredFieldValidator runat="server" ID="valPort" ControlToValidate="txtPort" ErrorMessage="* Required"></asp:RequiredFieldValidator>
            </td>
        </tr>
        <tr>
            <td>Max Clients</td>
            <td><asp:TextBox runat="server" ID="txtMaxClient"></asp:TextBox></td>
            <td>
                <asp:RequiredFieldValidator runat="server" ID="valMaxClient" ControlToValidate="txtMaxClient" ErrorMessage="* Required"></asp:RequiredFieldValidator>
            </td>
        </tr>
        <tr>
            <td>Max Backlog</td>
            <td><asp:TextBox runat="server" ID="txtMaxBackLog"></asp:TextBox></td>
            <td>
                <asp:RequiredFieldValidator runat="server" ID="valMaxBackLog" ControlToValidate="txtMaxBackLog" ErrorMessage="* Required"></asp:RequiredFieldValidator>
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
