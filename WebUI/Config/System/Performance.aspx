<%@ Page Title="" Language="C#" MasterPageFile="~/Site.Master" AutoEventWireup="true" CodeBehind="Performance.aspx.cs" Inherits="WebUI.Config.System.Performance" %>

<asp:Content ID="Content1" ContentPlaceHolderID="head" runat="server">
</asp:Content>

<asp:Content ID="Content2" ContentPlaceHolderID="ContentPlaceHolder1" runat="server">


    <asp:Button runat="server" ID="btnReset" Text="Reset Stats" onclick="btnReset_Click" />
    <asp:Button runat="server" ID="btnDump" Text="Dump Stats" onclick="btnDump_Click" />
</asp:Content>

