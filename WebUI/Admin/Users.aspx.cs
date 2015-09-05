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
                MembershipUserCollection Users = Membership.GetAllUsers();
                lstusers.DataSource = Users;
                lstusers.DataBind();
            }
        }
    }
}