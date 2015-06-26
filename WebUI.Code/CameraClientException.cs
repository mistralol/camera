using System;
using System.Collections.Generic;
using System.Text;

namespace WebUI.Code
{
    public class CameraClientException : Exception
    {
        int ErrCode = 0;

        public CameraClientException(int err)
        {
            ErrCode = err;
        }

        public override string Message
        {
            get
            {
                return string.Format("CameraClientException: {0}", ErrCode);
            }
        }

        public override string ToString()
        {
            return Message;
        }
    }
}
