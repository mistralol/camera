using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Text;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

namespace WebUI.Code.WebControls
{
    [ToolboxData("<{0}:NumerBox runat=server></{0}:NumerBox>")]
    public class NumberBox : WebControl
    {
        private TextBox txtNumber = new TextBox();

        protected override void OnInit(EventArgs e)
        {
            base.OnInit(e);

            Controls.Add(txtNumber);
        }

        protected override void OnLoad(EventArgs e)
        {
            base.OnLoad(e);

            Page.ClientScript.RegisterClientScriptBlock(typeof(string), "NumberBox.js", JScript.NumberBox, true);
            txtNumber.Attributes["onkeydown"] = "return NumberBoxCheckValue(event);";
        }

        protected override void RenderContents(HtmlTextWriter output)
        {
            txtNumber.RenderControl(output);
        }

        [Browsable(true)]
        public string Text
        {
            get
            {
                return txtNumber.Text;
            }
            set
            {
                txtNumber.Text = value;
            }
        }

        [Browsable(true)]
        public int Value
        {
            get
            {
                return int.Parse(txtNumber.Text);
            }
            set
            {
                txtNumber.Text = value.ToString();
            }
        }

        [Browsable(true)]
        public override Unit Width
        {
            get
            {
                return base.Width;
            }
            set
            {
                base.Width = value;
                txtNumber.Width = value;
            }
        }

        [Browsable(true)]
        public override Unit Height
        {
            get
            {
                return base.Height;
            }
            set
            {
                base.Height = value;
                txtNumber.Height = value;
            }
        }
    }
}
