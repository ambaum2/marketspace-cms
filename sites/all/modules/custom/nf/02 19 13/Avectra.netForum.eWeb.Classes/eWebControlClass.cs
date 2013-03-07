using Avectra.netForum.Data;
using System;
using System.Web.UI;

namespace Avectra.netForum.eWeb.Classes
{
	public class eWebControlClass : UserControl
	{
		public string szFormKey;

		public string szObjectName;

		public string szDestinationKey;

		public string szData;

		public string szAttributes;

		public string szFromEmail;

		public bool bDestinationNewWindow;

		public FacadeClass oFacadeObject;

		public eWebPageClass oeWebPage;

		public eWebPageClass.PaneDataClass oPaneData;

		public eWebControlClass()
		{
			this.szFormKey = "";
			this.szObjectName = "";
			this.szDestinationKey = "";
			this.szData = "";
			this.szAttributes = "";
			this.szFromEmail = "";
		}
	}
}