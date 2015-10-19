<%@ Page Title="" Language="C#" MasterPageFile="~/Site.Master" AutoEventWireup="true" CodeBehind="LiveView.aspx.cs" Inherits="WebUI.Users.LiveView" %>
<%@ Register Src="~/Controls/VideoView.ascx" TagName="VideoView" TagPrefix="WebUI" %>

<asp:Content ID="Content1" ContentPlaceHolderID="head" runat="server">
</asp:Content>

<asp:Content ID="Content2" ContentPlaceHolderID="ContentPlaceHolder1" runat="server">

<asp:DropDownList runat="server" ID="ddlVideo" AutoPostBack="true">

</asp:DropDownList><br />

<WebUI:VideoView runat="server" ID="VStream" />

</asp:Content>
