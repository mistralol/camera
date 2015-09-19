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

            if (IsPostBack == false)
            {
                string[] lst = Roles.GetAllRoles();
                chklstRoles.Items.Clear();
                for(int i =0;i<lst.Length;i++)
                {
                    chklstRoles.Items.Add(new ListItem(lst[i], lst[i]));
                }
            }
        }

        protected void btnCreate_Click(object sender, EventArgs e)
        {
            if (IsValid == false)
                return;

            try
            {
                Membership.CreateUser(txtUsername.Text, txtPassword.Text, txtEMail.Text);

                for (int i = 0; i < chklstRoles.Items.Count;i++ )
                {
                    Roles.AddUserToRole(txtUsername.Text, chklstRoles.Items[i].Value);
                }

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
