using System;
using System.Collections.Generic;

using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

namespace WebUI
{
    public partial class ErrorException : System.Web.UI.Page
    {
        protected void Page_Load(object sender, EventArgs e)
        {
            try
            {
                if (Session["ErrorText"] == null)
                    Response.Redirect("/ErrorUnknown.aspx");

                string txt = HttpUtility.HtmlEncode((string)Session["ErrorText"]);
                txt = txt.Replace("\n", "<br/>");
                lblError.Text = txt;
            }
            catch (Exception )
            {
                //Can't deal with this
                Response.Redirect("/ErrorUnknown.aspx");
            }
        }
    }
}
