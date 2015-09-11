using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Text;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

namespace WebUI.Code.WebControls
{
    [ToolboxData("<{0}:NumberBoxValidator runat=server></{0}:NumberBoxValidator>")]
    public class NumberBoxValidator : BaseValidator
    {
        protected override bool ControlPropertiesValid()
        {
            NumberBox ctrl = (NumberBox)FindControl(ControlToValidate);
            return (ctrl != null);
        }

        protected override bool EvaluateIsValid()
        {
            NumberBox ctrl = (NumberBox)this.FindControl(ControlToValidate);
            int tmp;
            return int.TryParse(ctrl.Text, out tmp);
        }
    }
}
