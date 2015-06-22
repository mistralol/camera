using System;
using System.Collections;
using System.ComponentModel;
using System.Web;
using System.Web.SessionState;

namespace WebUI
{
	public class Global : System.Web.HttpApplication
	{
		
		protected void Application_Start (Object sender, EventArgs e)
		{
		}

		protected void Session_Start (Object sender, EventArgs e)
		{
		}

		protected void Application_BeginRequest (Object sender, EventArgs e)
		{
		}

		protected void Application_EndRequest (Object sender, EventArgs e)
		{
		}

		protected void Application_AuthenticateRequest (Object sender, EventArgs e)
		{
		}

		protected void Application_Error (Object sender, EventArgs e)
		{
            Exception ex = Server.GetLastError();
            if (ex == null)
                Response.Redirect("/ErrorUnknown.aspx");

            string txt = "";
            while (ex != null)
            {
                txt += "\n\n";
                txt += ex.Message + "\n";
                txt += ex.StackTrace + "\n";
                ex = ex.InnerException;
            }
            Session["ErrorText"] = txt;
            Response.Redirect("/ErrorException.aspx");
		}

		protected void Session_End (Object sender, EventArgs e)
		{
		}

		protected void Application_End (Object sender, EventArgs e)
		{
		}
	}
}

