using System;

namespace WebUI.Code
{
	public class AppPage : System.Web.UI.Page
	{
		public CameraClient Camera {
			get {
				lock (Session) {
					if (Session ["Camera"] == null) {
						CameraClient tmp = new CameraClient ();
						tmp.Connect("unix:/tmp/CameraServer");
						Session ["Camera"] = tmp;
					}
					return (CameraClient) Session["Camera"];
				}
			}
		}
	}
}

