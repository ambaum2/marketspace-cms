using System;

namespace Avectra.netForum.eWeb.Classes.Calendars
{
	public class EventCalendarInfo
	{
		public bool bIncludeAlarm;

		public bool bRecurring;

		public string szRecurInterval;

		public int iRecurCount;

		public DateTime oEndDate
		{
			get;
			set;
		}

		public DateTime oStartDate
		{
			get;
			set;
		}

		public string szDescription
		{
			get;
			set;
		}

		public string szLocation
		{
			get;
			set;
		}

		public string szSummary
		{
			get;
			set;
		}

		public string szUID
		{
			get;
			set;
		}

		public EventCalendarInfo()
		{
			this.szUID = string.Empty;
			this.szSummary = string.Empty;
			this.szDescription = string.Empty;
			this.szLocation = string.Empty;
			this.oStartDate = DateTime.Now;
			this.oEndDate = DateTime.Now;
			this.bIncludeAlarm = true;
			this.bRecurring = false;
		}
	}
}