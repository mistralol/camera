using System;
using System.Collections.Generic;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

using WebUI.Code;

namespace WebUI.Users
{
    public partial class LiveView : AppPage
    {
        protected void Page_Load(object sender, EventArgs e)
        {

            if (IsPostBack == false)
            {
                int Count = Camera.VideoInputCount();
                for (int i = 0; i < Count; i++)
                {  
                    ddlVideo.Items.Add(new ListItem(string.Format("Video Input {0}", i+1), string.Format("{0}", i)));
                }

                if (ddlVideo.SelectedIndex < 0)
                    ddlVideo.SelectedIndex = 0;
            }

            //VideoInputConfig Cfg;
            //Camera.VideoInputGetConfig(0, Cfg);

            //FIXME:
            //VStream.Width = Cfg.G
//            VStream.Height = Cfg.GetHight();
            VStream.VInput = Convert.ToInt32(ddlVideo.SelectedValue);
            VStream.Width = 640;
            VStream.Height = 400;

        }
    }
}