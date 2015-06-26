using System;
using System.Collections.Generic;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

using WebUI.Code;

namespace WebUI
{
    public partial class Site1 : AppMasterPage
    {
        protected void Page_Load(object sender, EventArgs e)
        {
            lblUsername.Text = HttpUtility.HtmlEncode(Page.User.Identity.Name);
            lblVersion.Text = HttpUtility.HtmlEncode(Camera.Version());
        }
    }
}