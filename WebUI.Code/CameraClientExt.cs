using System;
using System.Collections.Generic;
using System.Text;

namespace WebUI.Code
{
    public static class CameraClientExt
    {
        public static void LogDebug(this CameraClient Camera, string fmt, params object[] args)
        {
            Camera.LogDebug(string.Format(fmt, args));
        }

        public static void LogInfo(this CameraClient Camera, string fmt, params object[] args)
        {
            Camera.LogInfo(string.Format(fmt, args));
        }

        public static void LogNotice(this CameraClient Camera, string fmt, params object[] args)
        {
            Camera.LogNotice(string.Format(fmt, args));
        }

        public static void LogWarning(this CameraClient Camera, string fmt, params object[] args)
        {
            Camera.LogWarning(string.Format(fmt, args));
        }

        public static void LogError(this CameraClient Camera, string fmt, params object[] args)
        {
            Camera.LogError(string.Format(fmt, args));
        }

        public static void LogCrtical(this CameraClient Camera, string fmt, params object[] args)
        {
            Camera.LogCrtical(string.Format(fmt, args));
        }

        public static void LogAlert(this CameraClient Camera, string fmt, params object[] args)
        {
            Camera.LogAlert(string.Format(fmt, args));
        }

        public static void LogEmergancy(this CameraClient Camera, string fmt, params object[] args)
        {
            Camera.LogEmergancy(string.Format(fmt, args));
        }

        public static void LogException(this CameraClient Camera, string Message, Exception ex)
        {
            Camera.LogError("LogException {0}", Message);
            Camera.LogError("LogException: {0}", ex.Message);
            Exception e = ex;
            while(e != null)
            {
                char[] newline = {'\n'};
                string[] stack = e.StackTrace.Split(newline);
                foreach(string x in stack)
                {
                    Camera.LogError("Stack: {0}", x);
                }

                e = e.InnerException;
                if (e != null)
                    Camera.LogError("Starting Next Stack");
            }

            Camera.LogError("End OF LogException");
        }

    }
}


