using System;
using System.Collections.Generic;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

using WebUI.Code;

namespace WebUI.Controls
{
    public partial class VideoView : AppUserControl
    {
        protected void Page_Load(object sender, EventArgs e)
        {

        }

        protected override void Render (HtmlTextWriter writer)
		{
        	
			switch (VideoType) {
				case WebStreamType.MJPEG:
					writer.Write("<img src=\"VideoStream.ashx?VInput={0}&OType=MJPEG\" width=\"{1}\" height=\"{2}>", VInput, Width, Height);
					break;
				case WebStreamType.MJPEG_TRANS:
					writer.Write("<img src=\"VideoStream.ashx?VInput={0}&OType=MJPEG_TRANS&Width={1}&Hieght={2}&FPS={3}&Quality{4}\" width=\"{1}\" height=\"{2}\">",
							VInput, Width, Height, FPS, Quality);
					break;
				default:
					writer.Write("<video width=\"{0}\" height=\"{1}\" autoplay>", Width, Height);
    		        writer.Write("<source src=\"VideoStream.ashx?VInput={0}&OType=MKV\" type=\"video/mp4\">", VInput);
            		writer.Write("Your browser does not support the video tag.");
            		writer.Write("</video>");
            		break;
			}
        }

        public int VInput
        {
            get;
            set;
        }

        public int Width
        {
            get;
            set;
        }

        public int Height
        {
            get;
            set;
        }

        public WebStreamType VideoType
        {
        	get;
        	set;
        }

        public int FPS
        {
        	get;
        	set;
        }

        public int Quality
        {
        	get;
        	set;
        }
    }
}
