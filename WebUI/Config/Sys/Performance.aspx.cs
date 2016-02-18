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
            if (IsPostBack == false)
            {
                Bind();
            }
        }

        protected void btnRefresh_Click(object sender, EventArgs e)
        {
            Bind();
        }

        protected void btnReset_Click(object sender, EventArgs e)
        {
            Camera.StatsReset();
            Bind();
        }

        protected void btnDump_Click(object sender, EventArgs e)
        {
            Camera.StatsDump();
            Bind();
        }

        private void Bind()
        {
            StringVector list = Camera.StatsList();

            List<StatsDetails> lst = new List<StatsDetails>();
            for (int i = 0; i < list.Count; i++)
            {
                StatsDetails info = new StatsDetails();
                Camera.StatsInfo(list[i], info);
                lst.Add(info);
            }
            gvPerf.DataSource = lst;
            gvPerf.DataBind();
        }
    }
}
