using System;
using System.Collections.Generic;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

using WebUI.Code;

namespace WebUI.Config.VideoInput
{
    public partial class Default : AppPage
    {
        protected void Page_Load(object sender, EventArgs e)
        {
            if (VideoCount <= 0)
            {
                Response.Redirect("NoInput.aspx");
                return;
            }

            if (IsPostBack == false)
            {
                for (int i = 0; i < VideoCount; i++)
                {
                    ListItem tmp = new ListItem(string.Format("Vidoe Input {0}", i + 1), i.ToString());
                    ddlInput.Items.Add(tmp);
                }

                Bind();
            }
        }

        protected void ddlInput_SelectedIndexChanged(object sender, EventArgs e)
        {
            Bind();
        }

        protected void btnEnable_Click(object sender, EventArgs e)
        {
            Camera.VideoInputSetEnabled(SelectedInput, true);
            pnlEnable.Visible = false;
            pnlConfig.Visible = true;
			Bind();
        }

        protected void btnDisable_Click(object sender, EventArgs e)
        {
            Camera.VideoInputSetEnabled(SelectedInput, false);
            pnlEnable.Visible = true;
            pnlConfig.Visible = false;
			Bind();
        }

        protected void btnSave_Click(object sender, EventArgs e)
        {
            VideoInputConfig Config = Camera.VideoInputGetConfig(SelectedInput);

            Config.SetCodec(ddlCodec.SelectedValue);
            Config.SetResolution(ddlResolution.SelectedValue);
            Config.SetFrameRate(Convert.ToInt32(ddlFrameRate.SelectedValue));

            Camera.VideoInputSetConfig(SelectedInput, Config);
        }

        protected void btnCancel_Click(object sender, EventArgs e)
        {
            Response.Redirect(Request.Url.ToString());
        }

        private void Bind()
        {
            try
            {
                int enabled = Camera.VideoInputGetEnabled(SelectedInput);
                ddlCodec.Items.Clear();
                ddlFrameRate.Items.Clear();
                ddlResolution.Items.Clear();
                if (enabled == 1)
                {
                    pnlEnable.Visible = false;
                    pnlConfig.Visible = true;

                    VideoInputConfig Config = Camera.VideoInputGetConfig(SelectedInput);
                    VideoInputSupported Supported = Camera.VideoInputGetSupported(SelectedInput);
                    StringVector Codecs = Supported.GetCodecs();
                    for (int i = 0; i < Codecs.Count; i++)
                        ddlCodec.Items.Add(new ListItem(Codecs[i], Codecs[i]));
                    ddlCodec.SelectedValue = Config.GetCodec();
                    Codecs.Dispose();

                    StringVector Res = Supported.GetCodecResolutions(ddlCodec.SelectedValue);
                    for (int i = 0; i < Res.Count; i++)
                        ddlResolution.Items.Add(new ListItem(Res[i], Res[i]));
                    ddlResolution.SelectedValue = Config.GetResolution();
                    Res.Dispose();

                    StringInt Rates = Supported.GetCodecFrameRates(ddlCodec.SelectedValue, ddlResolution.SelectedValue);
                    for (int i = 0; i < Rates.Count; i++)
                        ddlFrameRate.Items.Add(new ListItem(Rates[i].ToString(), Rates[i].ToString()));
                    ddlFrameRate.SelectedValue = Config.GetFrameRate().ToString();
                    Rates.Dispose();

                }
                else
                {
                    pnlEnable.Visible = true;
                    pnlConfig.Visible = false;
                }
            }
            catch (Exception ex)
            {
                Camera.LogError(string.Format("VideoInput Config Page Exception: {0}", ex.Message));
                throw (ex);
            }
        }

        private int SelectedInput
        {
            get
            {
                int sel = ddlInput.SelectedIndex;
                if (sel < 0)
                {
                    ddlInput.SelectedIndex = 0;
                    return SelectedInput;
                }
                int input = Convert.ToInt32(ddlInput.Items[sel].Value);
                return input;
            }
        }

        private int VideoCount
        {
            get
            {
                int value = Camera.VideoInputCount();
                return value;
            }
        }
    }
}
