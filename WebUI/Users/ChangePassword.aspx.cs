using System;
using System.Collections.Generic;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

using WebUI.Code;

namespace WebUI.Users
{
    public partial class ChangePassword : AppPage
    {
        protected void Page_Load(object sender, EventArgs e)
        {

        }

        protected void chgPassword_ChangedPassword(object sender, EventArgs e)
        {
            pnlStart.Visible = false;
            pnlComplete.Visible = true;
        }

        protected void chgPassword_ChangePasswordError(object sender, EventArgs e)
        {
            pnlStart.Visible = false;
            pnlError.Visible = true;
        }

        protected void btnTryAgain_Click(object sender, EventArgs e)
        {
            Response.Redirect("/Users/ChangePassword.aspx");
        }
    }
}