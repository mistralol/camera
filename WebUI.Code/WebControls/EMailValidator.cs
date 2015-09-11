using System;
using System.Collections.Generic;
using System.Text;
using System.Web.UI.WebControls;

namespace WebUI.Code.WebControls
{
    public class EMailValidator : RegularExpressionValidator
    {
        public EMailValidator()
        {
            ValidationExpression = @"(?:[a-z0-9!#$%&'*+/=?^_`{|}~-]+(?:\.[a-z0-9!#$%&'*+/=?^_`{|}~-]+)*|""(?:[\x01-\x08\x0b\x0c\x0e-\x1f\x21\x23-\x5b\x5d-\x7f]|\\[\x01-\x09\x0b\x0c\x0e-\x7f])*"")@(?:(?:[a-z0-9](?:[a-z0-9-]*[a-z0-9])?\.)+[a-z0-9](?:[a-z0-9-]*[a-z0-9])?|\[(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?|[a-z0-9-]*[a-z0-9]:(?:[\x01-\x08\x0b\x0c\x0e-\x1f\x21-\x5a\x53-\x7f]|\\[\x01-\x09\x0b\x0c\x0e-\x7f])+)\])";
        }

        protected override bool EvaluateIsValid()
        {
            TextBox txt = (TextBox)FindControl(ControlToValidate);
            if (txt == null)
                throw (new Exception(string.Format("Unable to Find Control '{0}'", ControlToValidate)));
            if (txt.Text == "")
                return false;
            return base.EvaluateIsValid();
        }
    }
}
