<%@ Page Title="" Language="C#" MasterPageFile="~/Site.Master" AutoEventWireup="true" CodeBehind="Performance.aspx.cs" Inherits="WebUI.Config.Sys.Performance" %>

<asp:Content ID="Content1" ContentPlaceHolderID="head" runat="server">
</asp:Content>

<asp:Content ID="Content2" ContentPlaceHolderID="ContentPlaceHolder1" runat="server">

    <asp:GridView runat="server" ID="gvPerf" AutoGenerateColumns="false">
        <Columns>
            <asp:BoundField DataField="Name" HeaderText="Counter Name" HtmlEncode="true" />
            <asp:BoundField DataField="Count" HeaderText="Calls" />
            <asp:BoundField DataField="BestTime" HeaderText="Best (MicroSeconds)" />
            <asp:BoundField DataField="WorstTime" HeaderText="Worst (MicroSeconds)" />
            <asp:BoundField DataField="AverageTime" HeaderText="Average (MicroSeconds)" />
        </Columns>
    </asp:GridView>

    <asp:Button runat="server" ID="btnRefresh" Text="Refresh" 
        onclick="btnRefresh_Click" />
    <asp:Button runat="server" ID="btnReset" Text="Reset Stats" onclick="btnReset_Click" />
    <asp:Button runat="server" ID="btnDump" Text="Dump Stats" onclick="btnDump_Click" />
</asp:Content>

