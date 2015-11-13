using System;
using System.Collections.Generic;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

using WebUI.Code;

namespace WebUI.Config.System
{
    public partial class Service : AppPage
    {
        protected void Page_Load(object sender, EventArgs e)
        {
            pnlError.Visible = false;
        }

        protected void btnStopServer_Click(object sender, EventArgs e)
        {
            try
            {
                Camera.Quit();
                pnlDone.Visible = true;
            }
            catch (Exception ex)
            {
                pnlStart.Visible = false;
                pnlError.Visible = true;
                lblError.Text = "Error: " + ex.Message;
            }

        }

        protected void btnShutdown_Click(object sender, EventArgs e)
        {
            try
            {
                Camera.SystemShutdown();
                pnlDone.Visible = true;
            }
            catch (Exception ex)
            {
                pnlStart.Visible = false;
                pnlError.Visible = true;
                lblError.Text = "Error: " + ex.Message;
            }
        }

        protected void btnReboot_Click(object sender, EventArgs e)
        {
            try
            {
                Camera.SystemReboot();
                pnlDone.Visible = true;
            }
            catch (Exception ex)
            {
                pnlStart.Visible = false;
                pnlError.Visible = true;
                lblError.Text = "Error: " + ex.Message;
            }
        }
    }
}
