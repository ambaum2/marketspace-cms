using System;

namespace Avectra.netForum.eWeb.Classes
{
	public class EmailContentClass
	{
		public string szFromEmail;

		public string szContentHtml;

		public string szContentUrl;

		public EmailContentClass()
		{
			this.szFromEmail = "";
			this.szContentHtml = "";
			this.szContentUrl = "";
		}
	}
}