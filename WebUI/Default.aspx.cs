
using System;
using System.Web;
using System.Web.UI;
using System.Web.Security;

using WebUI.Code;

namespace WebUI
{
	public partial class Default : AppPage
	{
		protected override void OnLoad (EventArgs e)
		{
            if (User.Identity.IsAuthenticated)
            {
                Response.Redirect("/Users/Default.aspx");
            }
            else
            {
                Response.Redirect("/Login.aspx");
            }
		}
	}
}
