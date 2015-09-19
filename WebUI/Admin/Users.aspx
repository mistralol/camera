<%@ Page Title="" Language="C#" MasterPageFile="~/Site.Master" AutoEventWireup="true" CodeBehind="Users.aspx.cs" Inherits="WebUI.Admin.Users" %>

<asp:Content ID="Content1" ContentPlaceHolderID="head" runat="server">
</asp:Content>

<asp:Content ID="Content2" ContentPlaceHolderID="ContentPlaceHolder1" runat="server">
    <a href="/Admin/UserAdd.aspx">Add New User</a><br />
    <br />

    <asp:Panel runat="server" ID="pnlError">
        <asp:Label runat="server" ID="lblError"></asp:Label>
    </asp:Panel>

    <asp:GridView runat="server" ID="lstUsers" AutoGenerateColumns="false" 
        DataKeyNames="ProviderUserKey" onrowcommand="lstUsers_RowCommand">
        <Columns>
            <asp:BoundField DataField="Username" HeaderText="Username"></asp:BoundField>
            <asp:BoundField DataField="EMail" HeaderText="EMail"></asp:BoundField>
            <custom:BoundYesNo DataField="IsApproved" HeaderText="Active"></custom:BoundYesNo>
            <custom:BoundYesNo DataField="IsLockedOut" HeaderText="Locked Out"></custom:BoundYesNo>
            <asp:ButtonField ButtonType="Link" Text="View" CommandName="DoView"></asp:ButtonField>
            <asp:ButtonField ButtonType="Link" Text="Edit" CommandName="DoEdit"></asp:ButtonField>
            <custom:ButtonFieldConfirm ButtonType="Link" Text="Delete" CommandName="DoDelete"></custom:ButtonFieldConfirm>
        </Columns>
    </asp:GridView>

</asp:Content>
