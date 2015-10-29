using System;
using System.Collections.Generic;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

using WebUI.Code;

namespace WebUI.Config.Network
{
    public partial class RTSP : AppPage
    {
        protected void Page_Load(object sender, EventArgs e)
        {
            if (IsPostBack == false)
            {
                Bind();
            }
        }

        protected void Bind()
        {
            txtPort.Text = Camera.RTSPGetPort().ToString();
            txtMaxClient.Text = Camera.RTSPGetMaxClients().ToString();
            txtMaxBackLog.Text = Camera.RTSPGetMaxBacklog().ToString();
        }

        protected void UnBind()
        {
            try
            {
                int port = Convert.ToInt32(txtPort.Text);
                int maxbacklog = Convert.ToInt32(txtMaxBackLog.Text);
                int maxclients = Convert.ToInt32(txtMaxClient.Text);

                Camera.RTSPSetPort(port);
                Camera.RTSPSetMaxClients(maxclients);
                Camera.RTSPSetMaxBacklog(maxbacklog);
                lblError.Text = "Saved";
            }
            catch (Exception ex)
            {
                Camera.LogError(ex.Message);
                lblError.Text = "Error";
            }
        }

        protected void btnSave_Click(object sender, EventArgs e)
        {
            if (IsValid == false)
            {
                lblError.Text = "* Invalid values";
                return;
            }

            UnBind();
            Bind();
        }

        protected void btnCancel_Click(object sender, EventArgs e)
        {
            Bind();
        }
    }
}