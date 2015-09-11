using System;
using System.Collections.Generic;
using System.Text;
using System.Web;
using System.Web.UI.WebControls;

namespace WebUI.Code.WebControls
{
    public class SaneValidator : RegularExpressionValidator
    {
        public SaneValidator()
        {
            ValidationExpression = "^[^<>&\"';]*$";
        }
    }
}
