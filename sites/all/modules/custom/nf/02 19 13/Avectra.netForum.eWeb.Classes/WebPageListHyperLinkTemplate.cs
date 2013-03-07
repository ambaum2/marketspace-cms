using Avectra.netForum.Common;
using Avectra.netForum.Data;
using System;
using System.Data;
using System.Web.UI;
using System.Web.UI.WebControls;

namespace Avectra.netForum.eWeb.Classes
{
	public class WebPageListHyperLinkTemplate : ITemplate
	{
		private string _szToolTip;

		private string _szURL;

		private string _szKeyColumnName;

		private string _szColumnName;

		private bool _bSameWindow;

		private string _szText;

		private bool _bDynamicText;

		private string _szMask;

		private string _szClass;

		private string _szRequest;

		private string _szFieldName;

		public WebPageListHyperLinkTemplate(string szToolTip, string szURL, string szColumnName, string szText, bool bSameWindow, bool bDynamicText, string szKeyColumnName, string szMask)
		{
			this._szToolTip = "";
			this._szURL = "";
			this._szKeyColumnName = "";
			this._szColumnName = "";
			this._szText = "";
			this._szMask = "";
			this._szClass = "";
			this._szRequest = "";
			this._szFieldName = "";
			this._szToolTip = szToolTip;
			this._szURL = szURL;
			this._szColumnName = szColumnName;
			this._szKeyColumnName = szKeyColumnName;
			this._szText = szText;
			this._bSameWindow = bSameWindow;
			this._bDynamicText = bDynamicText;
			this._szMask = szMask;
		}

		public WebPageListHyperLinkTemplate(string szToolTip, string szURL, string szColumnName, string szText, bool bSameWindow, bool bDynamicText)
		{
			this._szToolTip = "";
			this._szURL = "";
			this._szKeyColumnName = "";
			this._szColumnName = "";
			this._szText = "";
			this._szMask = "";
			this._szClass = "";
			this._szRequest = "";
			this._szFieldName = "";
			this._szToolTip = szToolTip;
			this._szURL = szURL;
			this._szColumnName = szColumnName;
			this._szText = szText;
			this._bSameWindow = bSameWindow;
			this._bDynamicText = bDynamicText;
		}

		public WebPageListHyperLinkTemplate(string szToolTip, string szURL, string szColumnName, string szText, bool bSameWindow)
		{
			this._szToolTip = "";
			this._szURL = "";
			this._szKeyColumnName = "";
			this._szColumnName = "";
			this._szText = "";
			this._szMask = "";
			this._szClass = "";
			this._szRequest = "";
			this._szFieldName = "";
			this._szToolTip = szToolTip;
			this._szURL = szURL;
			this._szColumnName = szColumnName;
			this._szText = szText;
			this._bSameWindow = bSameWindow;
		}

		public WebPageListHyperLinkTemplate(string szToolTip, string szURL, string szColumnName, string szText, string szFieldName, string szRequestParaName, bool bSameWindow)
		{
			this._szToolTip = "";
			this._szURL = "";
			this._szKeyColumnName = "";
			this._szColumnName = "";
			this._szText = "";
			this._szMask = "";
			this._szClass = "";
			this._szRequest = "";
			this._szFieldName = "";
			this._szToolTip = szToolTip;
			this._szURL = szURL;
			this._szColumnName = szColumnName;
			this._szText = szText;
			this._bSameWindow = bSameWindow;
			this._szRequest = szRequestParaName;
			this._szFieldName = szFieldName;
		}

		public WebPageListHyperLinkTemplate(string szToolTip, string szURL, string szColumnName, string szText, bool bSameWindow, string szClass)
		{
			this._szToolTip = "";
			this._szURL = "";
			this._szKeyColumnName = "";
			this._szColumnName = "";
			this._szText = "";
			this._szMask = "";
			this._szClass = "";
			this._szRequest = "";
			this._szFieldName = "";
			this._szToolTip = szToolTip;
			this._szURL = szURL;
			this._szColumnName = szColumnName;
			this._szText = szText;
			this._bSameWindow = bSameWindow;
			this._szClass = szClass;
		}

		public void InstantiateIn(Control oControl)
		{
			HyperLink hyperLink = new HyperLink();
			hyperLink.ToolTip = this._szToolTip;
			hyperLink.Text = this._szText;
			hyperLink.DataBinding += new EventHandler(this.SetupOnClick);
			oControl.Controls.Add(hyperLink);
		}

		private void SetupOnClick(object sender, EventArgs e)
		{
			string str;
			string str1;
			string str2;
			HyperLink uRL = (HyperLink)sender;
			DataGridItem namingContainer = (DataGridItem)uRL.NamingContainer;
			DataRowView dataItem = (DataRowView)namingContainer.DataItem;
			if (this._szURL != "" && this._szURL.Length >= 10)
			{
				if (this._szURL.Substring(0, 11).ToLower() == "javascript:")
				{
					uRL.NavigateUrl = Functions.AppendSiteCodeToURL(this._szURL);
				}
				else
				{
					WebPageListHyperLinkTemplate webPageListHyperLinkTemplate = this;
					string str3 = webPageListHyperLinkTemplate._szURL;
					if (this._szURL.IndexOf("?") < 0)
					{
						str1 = "?";
					}
					else
					{
						str1 = "";
					}
					webPageListHyperLinkTemplate._szURL = string.Concat(str3, str1);
					WebPageListHyperLinkTemplate webPageListHyperLinkTemplate1 = this;
					string str4 = webPageListHyperLinkTemplate1._szURL;
					if (!(this._szURL.Substring((this._szURL.Length - 1)).ToLower() != "&") || !(this._szURL.Substring((this._szURL.Length - 1)).ToLower() != "?"))
					{
						str2 = "";
					}
					else
					{
						str2 = "&";
					}
					webPageListHyperLinkTemplate1._szURL = string.Concat(str4, str2);
					if (!UtilityFunctions.EmptyString(this._szKeyColumnName))
					{
						string uRL1 = Functions.AppendSiteCodeToURL(string.Concat(this._szURL, this._szRequest, "=", ((DataRowView)namingContainer.DataItem)[this._szKeyColumnName].ToString()));
						if (!this._bSameWindow)
						{
							str = string.Concat("OpenNewWindow('", uRL1, "')");
						}
						else
						{
							str = string.Concat("window.document.URL='", uRL1, "'");
						}
					}
					else
					{
						string uRL2 = Functions.AppendSiteCodeToURL(string.Concat(this._szURL, this._szRequest, "=", ((DataRowView)namingContainer.DataItem)[this._szColumnName].ToString()));
						if (!this._bSameWindow)
						{
							str = string.Concat("OpenNewWindow('", uRL2, "')");
						}
						else
						{
							str = string.Concat("window.document.URL='", uRL2, "'");
						}
					}
					if (this._bDynamicText)
					{
						string str5 = dataItem[this._szColumnName].ToString();
						if (!UtilityFunctions.EmptyString(this._szMask))
						{
							str5 = DataUtils.ApplyMask(str5, this._szMask);
						}
						uRL.Text = str5;
					}
					if (UtilityFunctions.EmptyString(this._szText))
					{
						uRL.Text = ((DataRowView)namingContainer.DataItem)[this._szFieldName.ToString().ToLower()].ToString();
					}
					uRL.NavigateUrl = string.Concat("javascript:", str);
				}
				uRL.CssClass = this._szClass;
				int itemIndex = namingContainer.ItemIndex;
				uRL.ID = string.Concat(itemIndex.ToString(), "_", this._szColumnName);
			}
		}
	}
}