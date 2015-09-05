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
            int port = 0;
            int maxbacklog = 0;
            int maxclients = 0;

            int ret = Camera.RTSPGetPort(out port);
            if (ret < 0)
                throw(new CameraClientException(ret));

            ret = Camera.RTSPGetMaxBacklog(out maxbacklog);
            if (ret < 0)
                throw(new CameraClientException(ret));

            ret = Camera.RTSPGetMaxClients(out maxclients);
            if (ret < 0)
                throw(new CameraClientException(ret));

            txtPort.Text = port.ToString();
            txtMaxClient.Text = maxclients.ToString();
            txtMaxBackLog.Text = maxbacklog.ToString();
        }

        protected void UnBind()
        {
            bool success = true;

            int port = Convert.ToInt32(txtPort.Text);
            int maxbacklog = Convert.ToInt32(txtMaxBackLog.Text);
            int maxclients = Convert.ToInt32(txtMaxClient.Text);

            if (Camera.RTSPSetPort(port) < 0)
                success = false;

            if (Camera.RTSPSetMaxClients(maxclients) < 0)
                success = false;

            if (Camera.RTSPSetMaxBacklog(maxbacklog) < 0)
                success = false;

            if (success)
            {
                lblError.Text = "Saved";
            }
            else
            {
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