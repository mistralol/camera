using System;
using System.Collections.Generic;
using System.Text;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

namespace WebUI.Code.WebControls
{
    public class ButtonFieldConfirm : ButtonField
    {


        public override void InitializeCell(DataControlFieldCell cell, DataControlCellType cellType, DataControlRowState rowState, int rowIndex)
        {
            switch (cellType)
            {
                case DataControlCellType.DataCell:
                    base.InitializeCell(cell, cellType, rowState, rowIndex);
                    SetAttrRecursive(cell);
                    break;
                default:
                    base.InitializeCell(cell, cellType, rowState, rowIndex);
                    break;
            }
        }

        private void SetAttrRecursive(Control Ctl)
        {
            for (int i = 0; i < Ctl.Controls.Count; i++)
                SetAttrRecursive(Ctl.Controls[i]);

            if (Ctl.GetType().ToString() == "System.Web.UI.WebControls.DataControlLinkButton")
            {
                LinkButton x = (LinkButton)Ctl;
                x.OnClientClick = string.Format("return confirm('{0}');", ConfirmQuestion);
            }
            else if (Ctl.GetType().ToString() == "System.Web.UI.WebControls.DataControlImageButton")
            {
                ImageButton x = (ImageButton)Ctl;
                x.OnClientClick = string.Format("return confirm('{0}');", ConfirmQuestion);
            }
        }

        private string _ConfirmQuestion = "Are You Sure?";
        public string ConfirmQuestion
        {
            get
            {
                return _ConfirmQuestion;
            }
            set
            {
                _ConfirmQuestion = value;
            }
        }
    }
}
