using System;
using System.Collections.Generic;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Web.Security;

using WebUI.Code;

namespace WebUI.Admin
{
    public partial class UserAdd : AppPage
    {
        protected void Page_Load(object sender, EventArgs e)
        {
            lblError.Text = "";
        }

        protected void btnCreate_Click(object sender, EventArgs e)
        {
            if (IsValid == false)
                return;

            try
            {
                Membership.CreateUser(txtUsername.Text, txtPassword.Text);

                pnlCreate.Visible = false;
                pnlDone.Visible = true;
            }
            catch (Exception ex)
            {
                lblError.Text = HttpUtility.HtmlEncode(string.Format("Error creating user: {0}", ex.Message));
                Util.Log(Camera, "WebUI.Admin.UserAdd.btnCreate_Click", ex);
            }
        }

        protected void btnContinue_Click(object sender, EventArgs e)
        {
            Response.Redirect("/Admin/Users.aspx");
        }

    }
}
