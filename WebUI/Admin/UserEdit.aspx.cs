using System;
using System.Collections.Generic;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

using WebUI.Code;

namespace WebUI.Admin
{
    public partial class UserEdit : AppPage
    {
        protected void Page_Load(object sender, EventArgs e)
        {

        }

        protected void btnSave_Click(object sender, EventArgs e)
        {
            pnlEdit.Visible = false;
            pnlContinue.Visible = true;
        }

        protected void btnCancel_Click(object sender, EventArgs e)
        {
            Response.Redirect("/Admin/Users.aspx");
        }

        protected void btnContinue_Click(object sender, EventArgs e)
        {
            Response.Redirect("/Admin/Users.aspx");
        }
    }
}