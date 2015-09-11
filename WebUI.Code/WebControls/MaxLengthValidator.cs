using System;
using System.Configuration;
using System.Web;
using System.Web.Security;
using System.Web.UI;
using System.Web.UI.HtmlControls;
using System.Web.UI.WebControls;
using System.Web.UI.WebControls.WebParts;

namespace WebUI.Code.WebControls
{
    [ToolboxData("<{0}:MaxLengthValidator runat=server></{0}:MaxLengthValidator>")]
    public class MaxLengthValidator : BaseValidator
    {
        private TextBox txtBox;

        protected override bool ControlPropertiesValid()
        {
            Control ctrl = FindControl(ControlToValidate);

            if (ctrl != null)
            {
                txtBox = (TextBox)ctrl;
                return (txtBox != null);
            }
            else
            {
                return false;
            }
        }
        protected override bool EvaluateIsValid()
        {
            if (txtBox.Text.Length > txtBox.MaxLength)
                return false;
            return true;
        }
    }
}
