<%@ Page Title="" Language="C#" MasterPageFile="~/Site.Master" AutoEventWireup="true" CodeBehind="Users.aspx.cs" Inherits="WebUI.Admin.Users" %>

<asp:Content ID="Content1" ContentPlaceHolderID="head" runat="server">
</asp:Content>

<asp:Content ID="Content2" ContentPlaceHolderID="ContentPlaceHolder1" runat="server">
    <a href="/Admin/UserAdd.aspx">Add New User</a><br />
    <br />

    <asp:DataGrid runat="server" ID="lstusers">
        <Columns>
            <asp:BoundColumn DataField="Username"></asp:BoundColumn>
        </Columns>
    </asp:DataGrid>

</asp:Content>
