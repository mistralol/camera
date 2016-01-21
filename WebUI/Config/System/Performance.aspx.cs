using System;
using System.Collections.Generic;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

using WebUI.Code;

namespace WebUI.Config.System
{
    public partial class Performance : AppPage
    {
        protected void Page_Load(object sender, EventArgs e)
        {
  
        }

        protected void btnReset_Click(object sender, EventArgs e)
        {
            Camera.StatsReset();
        }

        protected void btnDump_Click(object sender, EventArgs e)
        {
            Camera.StatsDump();
        }
    }
}
