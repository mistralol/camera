using System;
using System.Collections.Generic;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

using WebUI.Code;

namespace WebUI.Config.Sys
{
    public partial class Debug : AppPage
    {
        protected void Page_Load(object sender, EventArgs e)
        {
            if (IsPostBack == false)
            {
                if (Camera.DebugGetEnabled())
                {
                    ddlDebugEnabled.SelectedIndex = 0;
                }
                else
                {
                    ddlDebugEnabled.SelectedIndex = 1;
                }
            }
        }

        protected void btnSave_Click(object sender, EventArgs e)
        {
            if (ddlDebugEnabled.SelectedValue == "1")
                Camera.DebugSetEnabled(true);
            else
                Camera.DebugSetEnabled(false);
        }

        protected void btnCancel_Click(object sender, EventArgs e)
        {
            Response.Redirect("Debug.aspx");
        }
    }
}