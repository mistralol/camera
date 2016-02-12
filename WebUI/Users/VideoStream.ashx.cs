using System;
using System.Collections.Generic;
using System.Web;
using System.Net;
using System.Net.Sockets;

using WebUI.Code;

namespace WebUI.Users
{
    /// <summary>
    /// Summary description for VideoStream
    /// </summary>
    public class VideoStream : IHttpHandler
    {
        public void ProcessRequest(HttpContext context)
        {

            using (CameraClient Camera = new CameraClient())
            {
                Int64 TotalSize = 0;

                Camera.Connect("unix:/tmp/CameraServer");
                try
                {
                    //FIXME: Verify incoming data?
                    string VInput = context.Request.QueryString["VInput"];
                    string OType = context.Request.QueryString["OType"];

                    Camera.LogDebug(string.Format("New WebVideoStream Request @ {0}", context.Request.Url.ToString()));

                    WebStreamOptions Opts = new WebStreamOptions();
                    try
                    {
                        Opts.vinput = Convert.ToInt32(VInput);
                        Opts.type = WebStreamType.Unknown;
                        Opts.localonly = true;
                        Opts.type = (WebStreamType)Enum.Parse(typeof(WebStreamType), OType);
                    }
                    catch (Exception ex)
                    {
                        Camera.LogError("VideoStream Failed to parse paramaters");
                        throw (ex);
                    }

                    int Port = Camera.WebStreamStart(Opts);

                    if (Port < 0)
                        throw (new Exception(string.Format("Unable to start stream error: {0}", Port)));

                    string Content = "";
                    switch (Opts.type)
                    {
                        case WebStreamType.FLV:
                            Content = "video/x-flv";
                            break;
                        case WebStreamType.MKV:
                            Content = "video/mkv";
                            break;
                        case WebStreamType.MP4:
                            Content = "video/mp4";
                            break;
                        case WebStreamType.WEBM:
                            Content = "video/webm";
                            break;
                        case WebStreamType.Unknown:
                            throw (new NotImplementedException());
                        default:
                            throw (new NotImplementedException());
                    }

                    context.Response.Clear();
                    context.Response.ContentType = Content;

                    context.Server.ScriptTimeout = 1000000;

                    try
                    {
                        using (TcpClient tcp = new TcpClient("127.0.0.1", Port))
                        {
                            NetworkStream stream = tcp.GetStream();

                            byte[] buffer = new byte[65535];
                            int size = stream.Read(buffer, 0, buffer.Length);
                            while (size > 0)
                            {
                                TotalSize += size;
                                context.Response.OutputStream.Write(buffer, 0, size);
                                context.Response.Flush();

                                size = stream.Read(buffer, 0, buffer.Length);
                            }
                        }
                    }
                    catch (Exception ex)
                    {
                        Camera.LogError(string.Format("VideoStream Error Port {0} Error: {1}", Port, ex.Message));
                        Camera.LogInfo(string.Format("VideoStream Closed Total Bytes Written {0}", TotalSize));
                    }
                    Camera.LogDebug(string.Format("Ending VideoStream on port {0}", Port));
                    context.Response.End();

                }
                catch (System.Threading.ThreadAbortException ex)
                {
                    Camera.LogDebug(string.Format("VideoStream: {0}", ex.Message)); //Expected when client disconnects
                }
                catch (Exception ex)
                {
                    Camera.LogError(string.Format("VideoStream Failed: {0}", ex.Message));
                    throw (ex);
                }
 
            }
        }

        public bool IsReusable
        {
            get
            {
                return false;
            }
        }
    }
}