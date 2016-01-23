using System;
using System.Collections.Generic;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Web.Security;

using WebUI.Code;

namespace WebUI.Admin
{
    public partial class Users : AppPage
    {
        protected void Page_Load(object sender, EventArgs e)
        {
            if (IsPostBack == false)
            {
                Bind();
            }
            pnlError.Visible = false;
        }

        void Bind()
        {
            MembershipUserCollection Users = Membership.GetAllUsers();
            lstUsers.DataSource = Users;
            lstUsers.DataBind();
        }

        protected void lstUsers_RowCommand(object sender, GridViewCommandEventArgs e)
        {
            try
            {
                int RowID = int.Parse(e.CommandArgument.ToString());
                string UserID = lstUsers.DataKeys[RowID].Value.ToString();
                switch (e.CommandName)
                {
                    case "DoView":
                        Response.Redirect(string.Format("UserView.aspx?UserID={0}", UserID));
                        break;
                    case "DoEdit":
                        Response.Redirect(string.Format("UserEdit.aspx?UserID={0}", UserID));
                        break;
                    case "DoDelete":
                        MembershipUser mu = Membership.GetUser((object)UserID);
                        Camera.LogInfo(string.Format("Requested to delete user '{0}' by user '{1}'", mu.UserName, User.Identity.Name));
                        Membership.DeleteUser(mu.UserName);
                        Bind();
                        break;
                    default:
                        pnlError.Visible = true;
                        lblError.Text = "Command not supported";
                        break;
                }
            }
            catch (Exception ex)
            {
                pnlError.Visible = true;
                lblError.Text = string.Format("Unexpected Error: {0}", HttpUtility.HtmlEncode(ex.Message));
                Camera.LogException("Unknown Error", ex);
            }
        }
    }
}
