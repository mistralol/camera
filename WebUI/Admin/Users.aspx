<%@ Page Title="" Language="C#" MasterPageFile="~/Site.Master" AutoEventWireup="true" CodeBehind="Users.aspx.cs" Inherits="WebUI.Admin.Users" %>

<asp:Content ID="Content1" ContentPlaceHolderID="head" runat="server">
</asp:Content>

<asp:Content ID="Content2" ContentPlaceHolderID="ContentPlaceHolder1" runat="server">
    <a href="/Admin/UserAdd.aspx">Add New User</a><br />
    <br />

    <asp:DataGrid runat="server" ID="lstusers" AutoGenerateColumns="false">
        <Columns>
            <asp:BoundColumn DataField="ProviderUserKey" HeaderText="Key"></asp:BoundColumn>
            <asp:BoundColumn DataField="Username" HeaderText="Username"></asp:BoundColumn>
            <asp:BoundColumn DataField="EMail" HeaderText="EMail"></asp:BoundColumn>
            <asp:BoundColumn DataField="IsApproved" HeaderText="Active"></asp:BoundColumn>
            <asp:BoundColumn DataField="IsLockedOut" HeaderText="Locked Out"></asp:BoundColumn>
            <asp:ButtonColumn ButtonType="LinkButton" Text="Edit"></asp:ButtonColumn>
            <asp:ButtonColumn ButtonType="LinkButton" Text="Delete"></asp:ButtonColumn>
        </Columns>
    </asp:DataGrid>

</asp:Content>
