using System;
using System.Collections.Generic;
using System.Text;

namespace WebUI.Code
{
    public static class Util
    {
        public static void Log(CameraClient Camera, string Prefix, Exception ex)
        {
            Exception e = ex;

            Camera.LogError(Prefix);
            while (e != null)
            {
                Camera.LogError(string.Format("{0}", Prefix, e.Message));
                Camera.LogError(string.Format("{0}", Prefix, e.StackTrace));
                e = e.InnerException;
            }
        }
    }
}
