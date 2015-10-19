using System;
using System.Collections.Generic;
using System.Web;

namespace WebUI.Code
{
    public abstract class AppHandler : IHttpHandler
    {
        public CameraClient Camera
        {
            get
            {
                lock (HttpContext.Current.Session)
                {
                    if (HttpContext.Current.Session["Camera"] == null)
                    {
                        CameraClient tmp = new CameraClient();
                        tmp.Connect("unix:/tmp/CameraServer");
                        HttpContext.Current.Session["Camera"] = tmp;
                    }
                    return (CameraClient)HttpContext.Current.Session["Camera"];
                }
            }
        }

        public virtual void ProcessRequest(HttpContext context)
        {
            throw (new NotImplementedException());
        }

        public virtual bool IsReusable
        {
            get
            {
                return false;
            }
        }
    }
}
