using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Text;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Web.Security;

namespace WebUI.Code.WebControls
{
    public class BoundYesNo : BoundField
    {
        protected override string FormatDataValue(object dataValue, bool encode)
        {
            try
            {
                bool tmp = (bool)dataValue;

                if (tmp)
                    return "Yes";
                return "No";
            }
            catch (Exception ex)
            {
                return ex.Message;
            }
        }
    }
}
