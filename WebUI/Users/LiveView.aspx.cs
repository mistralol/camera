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
					if (Camera.VideoInputGetEnabled(i) == 1)
						ddlVideo.Items.Add(new ListItem(string.Format("Video Input {0}", i + 1), string.Format("{0}", i)));
				}

				if (ddlVideo.SelectedIndex < 0)
					ddlVideo.SelectedIndex = 0;
			}

			int VInput = Convert.ToInt32(ddlVideo.SelectedValue);
			VideoInputConfig Config = Camera.VideoInputGetConfig(VInput);

			VStream.VInput = VInput;
			VStream.Width = Config.GetWidth();
			VStream.Height = Config.GetHeight();

			switch (Config.GetCodec())
			{
				case "H264":
					VStream.VideoType = WebStreamType.MKV;
					break;
				case "MJPEG":
					VStream.VideoType = WebStreamType.MJPEG;
					break;
				default:
					VStream.VideoType = WebStreamType.MJPEG_TRANS;
					break;
			}
        }
    }
}
