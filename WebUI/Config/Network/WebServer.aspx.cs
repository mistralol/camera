using System;
using System.Collections.Generic;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

using WebUI.Code;

namespace WebUI.Config.Network
{
    public partial class WebServer : AppPage
    {
        protected void Page_Load(object sender, EventArgs e)
        {
            lblError.Text = "";
            if (IsPostBack == false)
            {
                Bind();
            }
        }

        protected void Bind()
        {
            int port = 0;
            
            int ret = Camera.WebServerGetPort(out port);
            if (ret < 0)
                throw(new CameraClientException(ret));

            txtPort.Text = port.ToString();
        }

        protected void UnBind()
        {
            bool success = true;
            int port = Convert.ToInt16(txtPort.Text);

            if (Camera.WebServerSetPort(port) < 0)
                success = false;

            if (success)
            {
                lblError.Text = "Saved";
            }
            else
            {
                lblError.Text = "Failed";
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