
using WebUI.Code;

namespace WebUI
{
	using System;
	using System.Web;
	using System.Web.UI;

	public partial class Default : AppPage
	{
		protected override void OnLoad (EventArgs e)
		{
			lblInfo.Text = Camera.Version();
		}
		
		public void button1Clicked (object sender, EventArgs args)
		{
			lblPing.Text = Camera.Ping().ToString();
		}
	}
}

