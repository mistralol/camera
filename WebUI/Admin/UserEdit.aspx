<%@ Page Title="" Language="C#" MasterPageFile="~/Site.Master" AutoEventWireup="true" CodeBehind="UserEdit.aspx.cs" Inherits="WebUI.Admin.UserEdit" %>

<asp:Content ID="Content1" ContentPlaceHolderID="head" runat="server">
</asp:Content>

<asp:Content ID="Content2" ContentPlaceHolderID="ContentPlaceHolder1" runat="server">

    <asp:Panel runat="server" ID="pnlEdit">
        <table>
            <tr>
                <td></td>
                <td>
                    <asp:Button runat="server" ID="btnSave" Text="Save" CausesValidation="true" 
                        onclick="btnSave_Click" />
                    <asp:Button runat="server" ID="btnCancel" Text="Cancel" 
                        CausesValidation="false" onclick="btnCancel_Click" />
                </td>
            </tr>
        </table>
    </asp:Panel>

    <asp:Panel runat="server" ID="pnlContinue" Visible="false">
        <asp:Button runat="server" ID="btnContinue" Text="Continue" 
            onclick="btnContinue_Click" />
    </asp:Panel>

</asp:Content>

