using Avectra.netForum.Common;
using Avectra.netForum.Data;
using System;
using System.Collections.Generic;
using System.Data.OleDb;
using System.IO;

namespace Avectra.netForum.eWeb.Classes.Calendars
{
	public class iCalendar
	{
		public List<EventCalendarInfo> oCalendarEvents;

		public string Method;

		public string EventTitle;

		public iCalendar()
		{
			this.oCalendarEvents = new List<EventCalendarInfo>();
			this.Method = "PUBLISH";
			this.EventTitle = string.Empty;
		}

		private ErrorClass AddEventsByRegistrant(string reg_key)
		{
			ErrorClass errorClass = new ErrorClass();
			if (UtilityFunctions.IsGuid(reg_key))
			{
				string str = string.Concat("SELECT [SUMMARY] = evt_title, [DESCRIPTION] = evt_description, [LOCATION] = loc_name,        [START_DATE_TIME] = convert(nvarchar(10),evt_start_date,101) + ' ' + isnull(evt_start_time,''),        [END_DATE_TIME] = convert(nvarchar(10),evt_end_date,101) + ' ' + isnull(evt_end_time,'') FROM ev_registrant (nolock) JOIN ev_event (nolock) ON evt_key = reg_evt_key LEFT JOIN ev_event_location (nolock) ON evl_evt_key = evt_key LEFT JOIN ev_location (nolock) ON loc_key = evl_loc_key WHERE reg_key = ", DataUtils.ValuePrep(reg_key, "av_key", false));
				try
				{
					OleDbDataReader dataReader = DataUtils.GetDataReader(str, DataUtils.GetConnection());
					if (dataReader.Read())
					{
						EventCalendarInfo eventCalendarInfo = new EventCalendarInfo();
						eventCalendarInfo.szSummary = dataReader["SUMMARY"].ToString();
						eventCalendarInfo.szDescription = dataReader["DESCRIPTION"].ToString();
						eventCalendarInfo.szLocation = dataReader["LOCATION"].ToString();
						if (!UtilityFunctions.ES(dataReader["START_DATE_TIME"].ToString()))
						{
							eventCalendarInfo.oStartDate = Convert.ToDateTime(dataReader["START_DATE_TIME"].ToString());
						}
						if (UtilityFunctions.ES(dataReader["END_DATE_TIME"].ToString()))
						{
                            //eventCalendarInfo.oStartDate; took this out unecessary abaum
                            
							DateTime dateTime = eventCalendarInfo.oStartDate;
							eventCalendarInfo.oEndDate = dateTime.AddHours(1);
						}
						else
						{
							eventCalendarInfo.oEndDate = Convert.ToDateTime(dataReader["END_DATE_TIME"].ToString());
						}
						string[] regKey = new string[5];
						regKey[0] = eventCalendarInfo.szSummary;
						regKey[1] = " - ";
						regKey[2] = eventCalendarInfo.szLocation;
						regKey[3] = " - ";
						regKey[4] = reg_key;
						eventCalendarInfo.szUID = string.Concat(regKey);
						this.EventTitle = eventCalendarInfo.szSummary;
						this.oCalendarEvents.Add(eventCalendarInfo);
					}
				}
				catch (Exception exception1)
				{
					Exception exception = exception1;
					errorClass = ErrorFactory.CreateInstance(exception);
				}
			}
			return errorClass;
		}

		private ErrorClass AddSessionsByRegistrant(string reg_key)
		{
			ErrorClass errorClass = new ErrorClass();
			if (UtilityFunctions.IsGuid(reg_key))
			{
				string str = string.Concat("SELECT [SUMMARY] = ses_title, [DESCRIPTION] = ses_description, [LOCATION] = loc_name,        [START_DATE] = convert(nvarchar(10),ses_start_date,101), [START_TIME] = ses_start_time,        [END_DATE] = convert(nvarchar(10),isnull(ses_end_date,ses_start_date),101), [END_TIME] = isnull(ses_end_time,ses_start_time) FROM ev_registrant_session (nolock) JOIN ev_session (nolock) ON ses_key = rgs_ses_key LEFT JOIN ev_event_location (nolock) ON evl_ses_key = ses_key LEFT JOIN ev_location (nolock) ON loc_key = evl_loc_key WHERE rgs_reg_key = ", DataUtils.ValuePrep(reg_key, "av_key", false), " AND ses_start_date is not null ");
				try
				{
					OleDbDataReader dataReader = DataUtils.GetDataReader(str, DataUtils.GetConnection());
					while (dataReader.Read())
					{
						EventCalendarInfo eventCalendarInfo = new EventCalendarInfo();
						eventCalendarInfo.szSummary = dataReader["SUMMARY"].ToString();
						eventCalendarInfo.szDescription = dataReader["DESCRIPTION"].ToString();
						eventCalendarInfo.szLocation = dataReader["LOCATION"].ToString();
						if (!UtilityFunctions.ES(dataReader["START_DATE"].ToString()))
						{
							eventCalendarInfo.oStartDate = Convert.ToDateTime(dataReader["START_DATE"].ToString());
						}
						if (!UtilityFunctions.ES(dataReader["END_DATE"].ToString()))
						{
							eventCalendarInfo.oEndDate = Convert.ToDateTime(dataReader["END_DATE"].ToString());
						}
						TimeSpan timeSpanFromTimeString = this.GetTimeSpanFromTimeString(dataReader["START_TIME"].ToString());
						TimeSpan timeSpan = this.GetTimeSpanFromTimeString(dataReader["END_TIME"].ToString());
						if (timeSpan <= timeSpanFromTimeString)
						{
							timeSpan = timeSpanFromTimeString.Add(TimeSpan.FromHours(1));
						}
						string[] regKey = new string[5];
						regKey[0] = eventCalendarInfo.szSummary;
						regKey[1] = " - ";
						regKey[2] = eventCalendarInfo.szLocation;
						regKey[3] = " - ";
						regKey[4] = reg_key;
						eventCalendarInfo.szUID = string.Concat(regKey);
						TimeSpan timeSpan1 = eventCalendarInfo.oEndDate - eventCalendarInfo.oStartDate;
						if (timeSpan1.Days > 0)
						{
							eventCalendarInfo.bRecurring = true;
							eventCalendarInfo.iRecurCount = timeSpan1.Days + 1;
							eventCalendarInfo.szRecurInterval = "DAILY";
						}
						DateTime dateTime = eventCalendarInfo.oStartDate;
						eventCalendarInfo.oEndDate = dateTime.Add(timeSpan);
						DateTime dateTime1 = eventCalendarInfo.oStartDate;
						eventCalendarInfo.oStartDate = dateTime1.Add(timeSpanFromTimeString);
						this.oCalendarEvents.Add(eventCalendarInfo);
					}
				}
				catch (Exception exception1)
				{
					Exception exception = exception1;
					errorClass = ErrorFactory.CreateInstance(exception);
				}
			}
			return errorClass;
		}

		private TimeSpan GetTimeSpanFromTimeString(string szTime)
		{
			TimeSpan timeSpan = TimeSpan.FromHours(0);
			bool flag = false;
			if (!UtilityFunctions.ES(szTime))
			{
				int num = szTime.ToUpper().IndexOf("AM");
				if (num > -1)
				{
					szTime = szTime.Substring(0, num);
				}
				int num1 = szTime.ToUpper().IndexOf("PM");
				if (num1 > -1)
				{
					szTime = szTime.Substring(0, num1);
					flag = true;
				}
				try
				{
					timeSpan = TimeSpan.Parse(szTime);
				}
				catch
				{
				}
				if (!flag || timeSpan.Hours >= 12)
				{
					if (!flag && timeSpan.Hours == 12)
					{
						timeSpan = timeSpan.Subtract(TimeSpan.FromHours(12));
					}
				}
				else
				{
					timeSpan = timeSpan.Add(TimeSpan.FromHours(12));
				}
			}
			return timeSpan;
		}

		public ErrorClass PopulateEventsByRegistrant(string reg_key, bool bIncludeSessions)
		{
			ErrorClass errorClass = this.AddEventsByRegistrant(reg_key);
			if (bIncludeSessions && !UtilityFunctions.ER(errorClass))
			{
				errorClass = this.AddSessionsByRegistrant(reg_key);
			}
			return errorClass;
		}

		public void WriteToStream(Stream oStream)
		{
			DateTime now = DateTime.Now;
			DateTime universalTime = now.ToUniversalTime();
			string str = "yyyyMMdd'T'HHmmss'Z'";
			if (oStream != null && oStream.CanWrite)
			{
				StreamWriter streamWriter = new StreamWriter(oStream);
				streamWriter.WriteLine("BEGIN:VCALENDAR");
				streamWriter.WriteLine("VERSION:2.0");
				streamWriter.WriteLine("PRODID:netFORUM Enterprise");
				if (!UtilityFunctions.ES(this.Method))
				{
					streamWriter.WriteLine(string.Concat("METHOD:", this.Method));
				}
				foreach (EventCalendarInfo oCalendarEvent in this.oCalendarEvents)
				{
					streamWriter.WriteLine("BEGIN:VEVENT");
					streamWriter.WriteLine("CLASS:PUBLIC");
					streamWriter.WriteLine(string.Concat("UID:", oCalendarEvent.szUID));
					streamWriter.WriteLine(string.Concat("CREATED:", universalTime.ToString(str)));
					streamWriter.WriteLine(string.Concat("DTSTAMP:", universalTime.ToString(str)));
					DateTime dateTime = oCalendarEvent.oStartDate;
					DateTime universalTime1 = dateTime.ToUniversalTime();
					streamWriter.WriteLine(string.Concat("DTSTART:", universalTime1.ToString(str)));
					DateTime dateTime1 = oCalendarEvent.oEndDate;
					DateTime universalTime2 = dateTime1.ToUniversalTime();
					streamWriter.WriteLine(string.Concat("DTEND:", universalTime2.ToString(str)));
					streamWriter.WriteLine(string.Concat("LAST-MODIFIED:", universalTime.ToString(str)));
					streamWriter.WriteLine(string.Concat("SUMMARY:", oCalendarEvent.szSummary));
					streamWriter.WriteLine(string.Concat("DESCRIPTION:", oCalendarEvent.szDescription));
					streamWriter.WriteLine(string.Concat("LOCATION:", oCalendarEvent.szLocation));
					streamWriter.WriteLine("PRIORITY:5");
					streamWriter.WriteLine("SEQUENCE:0");
					streamWriter.WriteLine("TRANSP:OPAQUE");
					if (oCalendarEvent.bRecurring)
					{
						streamWriter.WriteLine(string.Format("RRULE:FREQ={0};COUNT={1}", oCalendarEvent.szRecurInterval, oCalendarEvent.iRecurCount));
					}
					if (oCalendarEvent.bIncludeAlarm)
					{
						streamWriter.WriteLine("BEGIN:VALARM");
						streamWriter.WriteLine("TRIGGER:-P15M");
						streamWriter.WriteLine("ACTION:DISPLAY");
						streamWriter.WriteLine("END:VALARM");
					}
					streamWriter.WriteLine("END:VEVENT");
				}
				streamWriter.WriteLine("END:VCALENDAR");
				streamWriter.Flush();
			}
		}
	}
}