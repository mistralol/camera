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

        protected override void Render(HtmlTextWriter writer)
        {
            writer.Write("<video width=\"{0}\" height=\"{1}\" autoplay>", Width, Height);
            writer.Write("<source src=\"VideoStream.ashx?VInput={0}&OType=MKV\" type=\"video/mp4\">", VInput);
            writer.Write("Your browser does not support the video tag.");
            writer.Write("</video>");
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
    }
}
