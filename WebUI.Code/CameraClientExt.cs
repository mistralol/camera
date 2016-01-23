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
    }
}


