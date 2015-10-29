using System;
using System.Collections.Generic;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

using WebUI.Code;

namespace WebUI.Config.VideoInput
{
    public partial class Default : AppPage
    {
        protected void Page_Load(object sender, EventArgs e)
        {
            if (VideoCount <= 0)
            {
                Response.Redirect("NoInput.aspx");
                return;
            }

            if (IsPostBack == false)
            {
                for (int i = 0; i < VideoCount; i++)
                {
                    ListItem tmp = new ListItem(string.Format("Vidoe Input {0}", i + 1), i.ToString());
                    ddlInput.Items.Add(tmp);
                }

                Bind();
            }
        }

        protected void ddlInput_SelectedIndexChanged(object sender, EventArgs e)
        {
            Bind();
        }

        protected void btnEnable_Click(object sender, EventArgs e)
        {
            Camera.VideoInputSetEnabled(SelectedInput, true);
            pnlEnable.Visible = false;
            pnlConfig.Visible = true;
        }

        protected void btnDisable_Click(object sender, EventArgs e)
        {
            Camera.VideoInputSetEnabled(SelectedInput, false);
            pnlEnable.Visible = true;
            pnlConfig.Visible = false;
        }

        protected void btnSave_Click(object sender, EventArgs e)
        {

        }

        protected void btnCancel_Click(object sender, EventArgs e)
        {

        }

        private void Bind()
        {
            int enabled = Camera.VideoInputGetEnabled(SelectedInput);
            if (enabled == 1)
            {
                pnlEnable.Visible = false;
                pnlConfig.Visible = true;
            }
            else
            {
                pnlEnable.Visible = true;
                pnlConfig.Visible = false;
            }
        }

        private int SelectedInput
        {
            get
            {
                int sel = ddlInput.SelectedIndex;
                if (sel < 0)
                {
                    ddlInput.SelectedIndex = 0;
                    return SelectedInput;
                }
                int input = Convert.ToInt32(ddlInput.Items[sel].Value);
                return input;
            }
        }

        private int VideoCount
        {
            get
            {
                int value = Camera.VideoInputCount();
                return value;
            }
        }
    }
}
