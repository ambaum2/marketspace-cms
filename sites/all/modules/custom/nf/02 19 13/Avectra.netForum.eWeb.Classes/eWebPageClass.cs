using Avectra.netForum.Common;
using Avectra.netForum.Components.CO;
using Avectra.netForum.Components.EV;
using Avectra.netForum.Controls;
using Avectra.netForum.Data;
using System;
using System.Collections;
using System.Collections.Specialized;
using System.Configuration;
using System.Data;
using System.Data.OleDb;
using System.IO;
using System.Net.Mail;
using System.Web;
using System.Web.UI;
using System.Web.UI.HtmlControls;
using System.Web.UI.WebControls;

namespace Avectra.netForum.eWeb.Classes
{
	public class eWebPageClass : PageEditClass
	{
		private DynamicFacade oDummyDynamicFacade;

		protected Hashtable hashPanes;

		protected Hashtable hashCSSClasses;

		protected bool bDesigner;

		protected int nTableBorder;

		protected string szWebContentKey;

		protected string szWebContentCode;

		protected string szRecordKey;

		protected string sziWebFolder;

		protected string szHttp;

		protected eWebPageClass.LinkCollectionClass oLinks;

		public bool bSiteEnableAtlas;

		public string szWebSiteEntityKey;

		public string szWebSiteKey;

		public string szWebSiteCode;

		public string szSectionKey;

		public string szMetatagKeyWords;

		protected string szWebAlign;

		protected string szWebWidth;

		protected string szWebWidthUnit;

		protected string szWebHeight;

		protected string szWebHeightUnit;

		protected string szWebBgColor;

		public bool bExcludeDefaults;

		public bool bSectionExcludeDefaults;

		protected string szExpandImage;

		protected string szCollapseImage;

		protected string szBodyAttributesSite;

		protected string szBodyAttributesContent;

		protected HtmlGenericControl oBodyTag;

		protected string szSQLVisible;

		protected bool bIgnoreSQLVisible;

		protected string szFromEmail;

		protected string szDenyContentKey;

		protected bool bPopupScriptAdded;

		protected int nPopupIDCounter;

		protected bool bRemoveEmptyFrameworkCells;

		private FacadeClass oLastFormFacadeObject;

		private bool bTitleFlag;

		public eWebPageClass()
		{
			this.oDummyDynamicFacade = new DynamicFacade();
			this.szWebContentKey = "";
			this.szWebContentCode = "";
			this.szRecordKey = "";
			this.sziWebFolder = "";
			this.szHttp = "";
			this.szWebSiteEntityKey = "";
			this.szWebSiteKey = "";
			this.szWebSiteCode = "";
			this.szSectionKey = "";
			this.szMetatagKeyWords = "";
			this.szWebAlign = "left";
			this.szWebWidth = "100%";
			this.szWebWidthUnit = "";
			this.szWebHeight = "";
			this.szWebHeightUnit = "";
			this.szWebBgColor = "";
			this.szExpandImage = "~/images/img_plus.gif";
			this.szCollapseImage = "~/images/img_minus.gif";
			this.szBodyAttributesSite = "";
			this.szBodyAttributesContent = "";
			this.szSQLVisible = "";
			this.szFromEmail = "";
			this.szDenyContentKey = "";
			this.bRemoveEmptyFrameworkCells = true;
			this.EnableViewState = false;
		}

		protected void AddBodyAttributes(string szAttributes)
		{
			if (!UtilityFunctions.EmptyString(szAttributes))
			{
				this.oBodyTag = (HtmlGenericControl)this.Page.FindControl("BodyTag");
				if (this.oBodyTag != null)
				{
					char[] chrArray = new char[1];
					chrArray[0] = ':';
					Array arrays = szAttributes.Split(chrArray);
					for (int i = 0; i < arrays.Length; i++)
					{
						char[] chrArray1 = new char[1];
						chrArray1[0] = '=';
						Array arrays1 = arrays.GetValue(i).ToString().Split(chrArray1);
						if (arrays1.Length == 2)
						{
							string str = arrays1.GetValue(0).ToString();
							string str1 = arrays1.GetValue(1).ToString();
							if (!(str.ToLower() == "emptycells") || !(str1.ToLower() == "yes"))
							{
								if (str.ToLower() != "iwebfolder")
								{
									if (this.oBodyTag.Attributes[str] != null)
									{
										this.oBodyTag.Attributes.Remove(str);
									}
									this.oBodyTag.Attributes.Add(str, str1);
								}
								else
								{
									this.sziWebFolder = str1;
								}
							}
							else
							{
								this.bRemoveEmptyFrameworkCells = false;
							}
						}
					}
				}
			}
		}

		protected void AddControlContent(string szCellID, eWebPageClass.PaneDataClass oPaneData, bool bMultipleContent)
		{
			string str = oPaneData.szControlPath;
			if (str != "")
			{
				HtmlTableCell contentCell = (HtmlTableCell)UtilityFunctions.FindControl(this.Page, szCellID);
				contentCell = this.GetContentCell(contentCell, oPaneData, bMultipleContent);
				if (contentCell != null && oPaneData.bIsVisible)
				{
					if (str.ToLower().IndexOf("listcontrol.ascx") <= 0)
					{
						eWebControlClass _eWebControlClass = (eWebControlClass)base.LoadControl(str);
						_eWebControlClass.ID = oPaneData.szKey.Replace("-", "_");
						_eWebControlClass.szFormKey = oPaneData.szFormKey;
						_eWebControlClass.szDestinationKey = oPaneData.szDestinationKey;
						_eWebControlClass.szData = oPaneData.szContentHtml;
						_eWebControlClass.szFromEmail = this.szFromEmail;
						_eWebControlClass.szAttributes = oPaneData.szContentPath;
						if (oPaneData.szContentPath.ToLower().IndexOf("newwindow") >= 0)
						{
							_eWebControlClass.bDestinationNewWindow = true;
						}
						contentCell.Controls.Add(_eWebControlClass);
						if (oPaneData.nHeight > 0)
						{
							contentCell.Height = string.Concat(oPaneData.nHeight, oPaneData.szHeightUnit);
						}
						if (oPaneData.nWidth > 0)
						{
							contentCell.Width = string.Concat(oPaneData.nWidth, oPaneData.szWidthUnit);
						}
						if (oPaneData.szAlign != "")
						{
							contentCell.Align = oPaneData.szAlign;
						}
						if (oPaneData.szVAlign != "")
						{
							contentCell.VAlign = oPaneData.szVAlign;
						}
						if (oPaneData.szBGColor != "")
						{
							contentCell.BgColor = oPaneData.szBGColor;
						}
						this.AddDesigner(oPaneData, contentCell);
					}
					else
					{
						if (!UtilityFunctions.EmptyString(oPaneData.szFormKey))
						{
							this.InstantiateContentFormFacadeObject(oPaneData.szFormKey, true);
						}
						eWebListControlClass _eWebListControlClass = (eWebListControlClass)base.LoadControl(str);
						_eWebListControlClass.ID = oPaneData.szKey.Replace("-", "_");
						_eWebListControlClass.szDetailKey = oPaneData.szKey;
						_eWebListControlClass.szContentHtml = oPaneData.szContentHtml;
						_eWebListControlClass.szCurrentWebKey = this.szWebSiteKey;
						_eWebListControlClass.szCurrentWebCode = this.szWebSiteCode;
						_eWebListControlClass.szWebKey = this.szWebContentKey;
						_eWebListControlClass.szWebCode = this.szWebContentCode;
						contentCell.Controls.Add(_eWebListControlClass);
						if (oPaneData.nHeight > 0)
						{
							contentCell.Height = string.Concat(oPaneData.nHeight, oPaneData.szHeightUnit);
						}
						if (oPaneData.nWidth > 0)
						{
							contentCell.Width = string.Concat(oPaneData.nWidth, oPaneData.szWidthUnit);
						}
						if (oPaneData.szAlign != "")
						{
							contentCell.Align = oPaneData.szAlign;
						}
						if (oPaneData.szVAlign != "")
						{
							contentCell.VAlign = oPaneData.szVAlign;
						}
						if (oPaneData.szBGColor != "")
						{
							contentCell.BgColor = oPaneData.szBGColor;
						}
						this.AddDesigner(oPaneData, contentCell);
						return;
					}
				}
			}
		}

		protected void AddDesigner(eWebPageClass.PaneDataClass oPaneData, HtmlTableCell oCell)
		{
			Image image;
			if (this.bDesigner && !UtilityFunctions.EmptyString(oPaneData.szKey))
			{
				if (oCell != null)
				{
					oCell.Align = "left";
					oCell.VAlign = "top";
				}
				string str = "";
				if (!oPaneData.bIsDefault || ConfigurationManager.AppSettings["editDefaultDetailFormKey"] == null)
				{
					if (!oPaneData.bIsSectionDefault || ConfigurationManager.AppSettings["editSectionDefaultDetailFormKey"] == null)
					{
						if (ConfigurationManager.AppSettings["editDetailFormKey"] != null)
						{
							str = ConfigurationManager.AppSettings["editDetailFormKey"].ToString();
						}
					}
					else
					{
						str = ConfigurationManager.AppSettings["editSectionDefaultDetailFormKey"].ToString();
					}
				}
				else
				{
					str = ConfigurationManager.AppSettings["editDefaultDetailFormKey"].ToString();
				}
				if (str != "")
				{
					string lower = Config.HttpServerPath.ToLower();
					if (lower.EndsWith("/eweb"))
					{
						lower = string.Concat(lower, "/");
					}
					lower = lower.Replace("/eweb/", "/iweb/");
					if (this.sziWebFolder != "")
					{
						lower = this.sziWebFolder;
						if (!lower.ToLower().StartsWith("http://"))
						{
							lower = string.Concat(this.szHttp, lower);
						}
					}
					image = new Image();
					image.ID = base.NewUniqueID();
					if (!oPaneData.bIsDefault)
					{
						if (!oPaneData.bIsSectionDefault)
						{
							image.ImageUrl = string.Concat(Config.ApplicationPath, "/images/designer/local.gif");
							oCell.Controls.AddAt(0, image);
							image = new Image();
							image.ID = base.NewUniqueID();
							image.ToolTip = "Edit WebPage Content";
							image.ImageUrl = string.Concat(image.ImageUrl, "local_");
						}
						else
						{
							image.ImageUrl = string.Concat(Config.ApplicationPath, "/images/designer/section_default.gif");
							oCell.Controls.AddAt(0, image);
							image = new Image();
							image.ID = base.NewUniqueID();
							image.ToolTip = "Edit Section Default Content";
						}
					}
					else
					{
						image.ImageUrl = string.Concat(Config.ApplicationPath, "/images/designer/site_default.gif");
						oCell.Controls.AddAt(0, image);
						image = new Image();
						image.ID = base.NewUniqueID();
						image.ToolTip = "Edit Site Default Content";
					}
					Image image1 = image;
					image1.ToolTip = string.Concat(image1.ToolTip, ": ", oPaneData.szContentType.ToString());
					image.ImageUrl = string.Concat(Config.ApplicationPath, "/images/designer/");
					string lower1 = oPaneData.szContentType.ToString().ToLower();
					string str1 = lower1;
					if (lower1 != null)
					{
						if (str1 == "html")
						{
							image.ImageUrl = string.Concat(image.ImageUrl, "html.gif");
							goto Label1;
						}
						else if (str1 == "form")
						{
							image.ImageUrl = string.Concat(image.ImageUrl, "form.gif");
							goto Label1;
						}
						else if (str1 == "control (ascx)")
						{
							image.ImageUrl = string.Concat(image.ImageUrl, "ascx.gif");
							goto Label1;
						}
						else if (str1 == "list")
						{
							image.ImageUrl = string.Concat(image.ImageUrl, "list.gif");
							goto Label1;
						}
						else if (str1 == "profile")
						{
							image.ImageUrl = string.Concat(image.ImageUrl, "profile.gif");
							goto Label1;
						}
						else if (str1 == "wizard")
						{
							image.ImageUrl = string.Concat(image.ImageUrl, "wizard.gif");
							goto Label1;
						}
						else if (str1 == "bundlewizard")
						{
							image.ImageUrl = string.Concat(image.ImageUrl, "wizard.gif");
							goto Label1;
						}
						else if (str1 == "iframe")
						{
							image.ImageUrl = string.Concat(image.ImageUrl, "iframe.gif");
							goto Label1;
						}
						else if (str1 == "file")
						{
							image.ImageUrl = string.Concat(image.ImageUrl, "file.gif");
							goto Label1;
						}
						goto Label0;
					}
					else
					{
						goto Label0;
					}
				Label1:
					oCell.BorderColor = "black";
					HtmlTable parent = (HtmlTable)oCell.Parent.Parent;
					parent.Border = 1;
					if (this.sziWebFolder != "")
					{
						string[] strArrays = new string[6];
						strArrays[0] = lower;
						strArrays[1] = "/Forms/DynamicEditModal.aspx?FormKey=";
						strArrays[2] = str;
						strArrays[3] = "&Key=";
						strArrays[4] = oPaneData.szKey;
						strArrays[5] = "&ReloadParent=Yes";
						image.Attributes.Add("onclick", string.Concat("window.open('", Functions.AppendSiteCodeToURL(string.Concat(strArrays)), "','eWebDesignDialog','');"));
					}
					else
					{
						string[] strArrays1 = new string[6];
						strArrays1[0] = lower;
						strArrays1[1] = "/Forms/DynamicEditModal.aspx?FormKey=";
						strArrays1[2] = str;
						strArrays1[3] = "&Key=";
						strArrays1[4] = oPaneData.szKey;
						strArrays1[5] = "&ReloadParent=Yes";
						image.Attributes.Add("onclick", string.Concat("OpenNewWindow('", Functions.AppendSiteCodeToURL(string.Concat(strArrays1)), "')"));
					}
					image.Attributes.Add("onmouseover", "style['cursor'] = 'hand';");
					oCell.Controls.AddAt(1, image);
					if (oPaneData.szContentType.ToLower() == "form" && oPaneData.szFormKey != "")
					{
						image = new Image();
						image.ID = base.NewUniqueID();
						image.ToolTip = "Design Form";
						image.ImageUrl = string.Concat(Config.ApplicationPath, "/images/img_eweb_design.gif");
						if (this.sziWebFolder != "")
						{
							image.Attributes.Add("onclick", string.Concat("window.open('", Functions.AppendSiteCodeToURL(string.Concat(lower, "/forms/md/md_dynamic_formDesign.aspx?FormKey=", oPaneData.szFormKey)), "','eWebDesignDialog','');"));
						}
						else
						{
							image.Attributes.Add("onclick", string.Concat("OpenNewWindow('", Functions.AppendSiteCodeToURL(string.Concat(lower, "/forms/md/md_dynamic_formDesign.aspx?FormKey=", oPaneData.szFormKey)), "')"));
						}
						image.Attributes.Add("onmouseover", "style['cursor'] = 'hand';");
						oCell.Controls.AddAt(2, image);
					}
					if ((oPaneData.szContentType.ToLower() == "wizard" || oPaneData.szContentType.ToLower() == "bundlewizard") && oPaneData.szWizardKey != "")
					{
						image = new Image();
						image.ID = base.NewUniqueID();
						image.ToolTip = "Design Wizard";
						image.ImageUrl = string.Concat(Config.ApplicationPath, "/images/img_wiz_edit.gif");
						if (this.sziWebFolder != "")
						{
							image.Attributes.Add("onclick", string.Concat("window.open('", Functions.AppendSiteCodeToURL(string.Concat(lower, "/forms/DynamicEditModal.aspx?ShowChildForms=Yes&ItemKey=ef6ad6c1-7512-4167-9d9d-b5b994d3c288&LinkKey=66d53ff8-b86a-4ddf-bac1-d27a7675130a&FormKey=ea52fe3a-4b18-479f-b1c1-490ea2ef7cfe&Key=", oPaneData.szWizardKey, "&ReloadParent=Yes")), "','eWebDesignDialog','');"));
						}
						else
						{
							image.Attributes.Add("onclick", string.Concat("OpenNewWindow('", Functions.AppendSiteCodeToURL(string.Concat(lower, "/forms/DynamicEditModal.aspx?Modal=yes&HideUI=yes&ShowChildForms=Yes&ItemKey=ef6ad6c1-7512-4167-9d9d-b5b994d3c288&LinkKey=66d53ff8-b86a-4ddf-bac1-d27a7675130a&FormKey=ea52fe3a-4b18-479f-b1c1-490ea2ef7cfe&Key=", oPaneData.szWizardKey, "&ReloadParent=Yes")), "')"));
						}
						image.Attributes.Add("onmouseover", "style['cursor'] = 'hand';");
						oCell.Controls.AddAt(2, image);
						if (base.szFormKey != null && base.szFormKey != "")
						{
							image = new Image();
							image.ID = base.NewUniqueID();
							image.ToolTip = "Design Form";
							image.ImageUrl = string.Concat(Config.ApplicationPath, "/images/img_eweb_design.gif");
							if (this.sziWebFolder != "")
							{
								image.Attributes.Add("onclick", string.Concat("window.open('", Functions.AppendSiteCodeToURL(string.Concat(lower, "/forms/md/md_dynamic_formDesign.aspx?FormKey=", base.szFormKey)), "','eWebDesignDialog','');"));
							}
							else
							{
								image.Attributes.Add("onclick", string.Concat("OpenNewWindow('", Functions.AppendSiteCodeToURL(string.Concat(lower, "/forms/md/md_dynamic_formDesign.aspx?FormKey=", base.szFormKey)), "')"));
							}
							image.Attributes.Add("onmouseover", "style['cursor'] = 'hand';");
							oCell.Controls.AddAt(3, image);
						}
					}
				}
			}
			return;
		Label0:
			image.ImageUrl = string.Concat(image.ImageUrl, "default.gif");
            //unkown Label1 goto Label1;
		}

		protected void AddFileContent(string szCellID, eWebPageClass.PaneDataClass oPaneData, bool bMultipleContent)
		{
			if (oPaneData.szControlPath != "" && oPaneData.bIsVisible)
			{
				string str = oPaneData.szControlPath.Replace("~", base.Request.PhysicalApplicationPath);
				try
				{
					FileStream fileStream = File.Open(str, FileMode.Open, FileAccess.Read, FileShare.Read);
					StreamReader streamReader = new StreamReader(fileStream);
					oPaneData.szContentHtml = streamReader.ReadToEnd();
					streamReader.Close();
					fileStream.Close();
					this.AddHtmlContent(szCellID, oPaneData, bMultipleContent);
				}
				catch
				{
				}
			}
		}

		protected void AddFormContent(string szCellID, eWebPageClass.PaneDataClass oPaneData, bool bMultipleContent)
		{
			string str = oPaneData.szFormKey;
			if (str != "")
			{
				this.InstantiateContentFormFacadeObject(str, false, true);
				this.HideUI = true;
				if (this.oFacadeObject != null)
				{
					oPaneData.oFacadeObject = this.oFacadeObject;
					this.oLastFormFacadeObject = this.oFacadeObject;
					if (base.bIsFirstTime && this.bHasRelatedObjects && base.szAddEditMode.ToLower().Trim() == "add" && base.Request["action"] != null && base.Request["action"].ToString().ToLower() == "add")
					{
						this.oFacadeObject.GetRelatedObjectValues(this.Session, (PageClass)this.Page);
					}
					this.oFacadeObject.ClearLastValueChangedFlag();
					if (this.Page.Request["ButtonCancel"] == null || this.Page.Request["ButtonCancel"] != "Cancel")
					{
						this.oFacadeObject.SetRequestData(this.Page);
					}
					if (base.bIsFirstTime)
					{
						this.oFacadeObject.SetRequestData(this.Page, true);
					}
					this.oFacadeObject.LoadRelatedData();
					if (base.bIsFirstTime && base.Request["RecNo"] == null)
					{
						UtilityFunctions.SetUrlReferrer(base.Request, this.ViewState);
						DataUtils.SetUrlReferrer(this.oFacadeObject, base.Request);
						if (base.szAddEditMode == "edit")
						{
							this.oFacadeObject.GetData(this.Page);
						}
					}
					base.SetDefaultValues();
				}
				HtmlTableCell contentCell = (HtmlTableCell)UtilityFunctions.FindControl(this.Page, szCellID);
				contentCell = this.GetContentCell(contentCell, oPaneData, bMultipleContent);
				if (contentCell != null && oPaneData.bIsVisible)
				{
					this.BuildFormTable(contentCell);
					HtmlTable htmlTable = (HtmlTable)this.Page.FindControl("DataFormTable");
					PageUtils.BuildEditFormContent((PageClass)this.Page, htmlTable, str, "", false);
					HtmlInputHidden htmlInputHidden = new HtmlInputHidden();
					htmlInputHidden.ID = "MultiChildAttributes";
					HtmlInputHidden htmlInputHidden1 = htmlInputHidden;
					contentCell.Controls.Add(htmlInputHidden1);
					base.BuildChildFormGrids((PageClass)this.Page, htmlTable, "", false, false, base.bEnableAtlas);
					if (oPaneData.nHeight > 0)
					{
						contentCell.Height = string.Concat(oPaneData.nHeight, oPaneData.szHeightUnit);
					}
					if (oPaneData.nWidth > 0)
					{
						contentCell.Width = string.Concat(oPaneData.nWidth, oPaneData.szWidthUnit);
					}
					this.AddDesigner(oPaneData, contentCell);
				}
			}
		}

		protected void AddHtmlContent(string szCellID, eWebPageClass.PaneDataClass oPaneData, bool bMultipleContent)
		{
			if (oPaneData.szContentHtml != "")
			{
				HtmlTableCell contentCell = (HtmlTableCell)UtilityFunctions.FindControl(this.Page, szCellID);
				contentCell = this.GetContentCell(contentCell, oPaneData, bMultipleContent);
				if (contentCell != null && oPaneData.bIsVisible)
				{
					if (oPaneData.szFormKey == "")
					{
						oPaneData.szContentHtml = DataUtils.ParseValues(this.oDummyDynamicFacade, oPaneData.szContentHtml, false);
					}
					else
					{
						this.InstantiateContentFormFacadeObject(oPaneData.szFormKey, true);
						if (this.oFacadeObject != null)
						{
							oPaneData.oFacadeObject = this.oFacadeObject;
							if (!UtilityFunctions.EmptyString(oPaneData.szCurrentKey))
							{
								this.oFacadeObject.CurrentKey = oPaneData.szCurrentKey;
								this.oFacadeObject.SelectByKey();
							}
							oPaneData.szContentHtml = DataUtils.ParseValues(this.oFacadeObject, oPaneData.szContentHtml, false);
						}
					}
					contentCell.InnerHtml = oPaneData.szContentHtml.Replace("../../eweb", Config.ApplicationPath);
					if (oPaneData.nHeight > 0)
					{
						contentCell.Height = string.Concat(oPaneData.nHeight, oPaneData.szHeightUnit);
					}
					if (oPaneData.nWidth > 0)
					{
						contentCell.Width = string.Concat(oPaneData.nWidth, oPaneData.szWidthUnit);
					}
					if (oPaneData.szAlign != "")
					{
						contentCell.Align = oPaneData.szAlign;
					}
					if (oPaneData.szVAlign != "")
					{
						contentCell.VAlign = oPaneData.szVAlign;
					}
					if (oPaneData.szBGColor != "")
					{
						contentCell.BgColor = oPaneData.szBGColor;
					}
					this.AddDesigner(oPaneData, contentCell);
				}
			}
		}

		protected void AddIFrameContent(string szCellID, eWebPageClass.PaneDataClass oPaneData, bool bMultipleContent)
		{
			if (oPaneData.szContentPath != "" || oPaneData.szControlPath != "")
			{
				string str = oPaneData.szContentPath;
				if (str == "")
				{
					str = oPaneData.szControlPath;
				}
				HtmlTableCell contentCell = (HtmlTableCell)UtilityFunctions.FindControl(this.Page, szCellID);
				contentCell = this.GetContentCell(contentCell, oPaneData, bMultipleContent);
				if (contentCell != null && oPaneData.bIsVisible)
				{
					HtmlGenericControl htmlGenericControl = new HtmlGenericControl("IFrame");
					string str1 = "";
					if (oPaneData.nHeight > 0)
					{
						string str2 = str1;
						string[] strArrays = new string[5];
						strArrays[0] = str2;
						strArrays[1] = "HEIGHT: ";
						strArrays[2] = oPaneData.nHeight.ToString();
						strArrays[3] = oPaneData.szHeightUnit;
						strArrays[4] = "; ";
						str1 = string.Concat(strArrays);
					}
					if (oPaneData.nWidth > 0)
					{
						string str3 = str1;
						string[] strArrays1 = new string[5];
						strArrays1[0] = str3;
						strArrays1[1] = "WIDTH: ";
						strArrays1[2] = oPaneData.nWidth.ToString();
						strArrays1[3] = oPaneData.szWidthUnit;
						strArrays1[4] = "; ";
						str1 = string.Concat(strArrays1);
					}
					if (str1 != "")
					{
						htmlGenericControl.Attributes.Add("style", str1);
					}
					htmlGenericControl.Attributes.Add("src", str);
					if (oPaneData.szContentHtml != "")
					{
						char[] chrArray = new char[1];
						chrArray[0] = ';';
						Array arrays = HtmlUtils.RemoveHtmlTags(oPaneData.szContentHtml).Split(chrArray);
						for (int i = 0; i < arrays.Length; i++)
						{
							string str4 = arrays.GetValue(i).ToString();
							if (str4.IndexOf("=") >= 0)
							{
								char[] chrArray1 = new char[1];
								chrArray1[0] = '=';
								Array arrays1 = str4.Split(chrArray1);
								if (arrays1.Length == 2)
								{
									htmlGenericControl.Attributes.Add(arrays1.GetValue(0).ToString().Trim(), arrays1.GetValue(1).ToString().Trim());
								}
							}
						}
					}
					contentCell.Controls.Add(htmlGenericControl);
					if (oPaneData.nHeight > 0)
					{
						contentCell.Height = string.Concat(oPaneData.nHeight, oPaneData.szHeightUnit);
					}
					if (oPaneData.nWidth > 0)
					{
						contentCell.Width = string.Concat(oPaneData.nWidth, oPaneData.szWidthUnit);
					}
					if (oPaneData.szAlign != "")
					{
						contentCell.Align = oPaneData.szAlign;
					}
					if (oPaneData.szVAlign != "")
					{
						contentCell.VAlign = oPaneData.szVAlign;
					}
					if (oPaneData.szBGColor != "")
					{
						contentCell.BgColor = oPaneData.szBGColor;
					}
					this.AddDesigner(oPaneData, contentCell);
				}
			}
		}

		protected void AddLink(eWebPageClass.LinkClass oLink, HtmlTable oLinkTable)
		{
			HtmlTableRow item;
			bool flag;
			bool flag1 = true;
			eWebPageClass.LinkClass linkClass = oLink;
            //HtmlGenericControl oLinkTable = (HtmlGenericControl)oLinkTable;
			if (oLink.bRequiresLogin)
			{
				flag = true;
			}
			else
			{
				flag = !UtilityFunctions.EmptyString(oLink.szSQLVisible);
			}
			linkClass.bRequiresLogin = flag;
			if (this.Session["CustomerKey"] != null || !oLink.bRequiresLogin)
			{
				if (this.Session["CustomerKey"] == null || !oLink.bDisableUponLogin)
				{
					if (!this.bExcludeDefaults || !oLink.bIsDefault)
					{
						if (!this.DataExists(oLink.szSQLVisible))
						{
							flag1 = false;
						}
					}
					else
					{
						flag1 = false;
					}
				}
				else
				{
					flag1 = false;
				}
			}
			else
			{
				flag1 = false;
			}
            /*
             * if the link is a site link check to make sure 
             * site links are enabled for this page. This is
             * hardcoded in siteLinkOnlyPages array by webkey. Ideally
             * we would do this some other way
             */
            if (flag1 == true && oLink.bIsDefault)
            {
                String thisWebKey = "";
                String thisWebCode = "";
                if (base.Request["WebKey"] != null)
                {
                    thisWebKey = base.Request["WebKey"].ToString();
                }
                if (base.Request["WebCode"] != null)
                {
                    thisWebCode = base.Request["WebCode"].ToString();
                }
                if(Functions.AAOCheckDisplayMyAAOMenuByWebKeyOrCode(thisWebKey, thisWebCode))
                { //if the current page matches one of the 
                    flag1 = true;
                }
                else
                {
                    flag1 = false;
                }
            }
			if (flag1) //render navigationif user logged in should see page and site links are okay on this page
			{
				bool flag2 = false;
				if (oLinkTable != null)
				{
					flag2 = true;
				}
				else
				{
					string linkCellIDCheck = this.GetLinkCellID(oLink.szPosition, oLink.szSection);
                    HtmlTableCell htmlTableCellCheck = (HtmlTableCell)UtilityFunctions.FindControl(this.Page, linkCellIDCheck);
                    
                    if (htmlTableCellCheck != null)
                    {
                        string str = string.Concat(linkCellIDCheck, "Table");//make table id
                        HtmlGenericControl menuUL = (HtmlGenericControl)UtilityFunctions.FindControl(htmlTableCellCheck, str);
                        if (menuUL == null) //the table does not exist yet so make it
                        {
                            menuUL = new HtmlGenericControl("UL");
                            menuUL.ID = str;
                            htmlTableCellCheck.Controls.Add(menuUL);
                        }
                    }
				}
                string linkCellID = this.GetLinkCellID(oLink.szPosition, oLink.szSection);
                HtmlTableCell htmlTableCell = (HtmlTableCell)UtilityFunctions.FindControl(this.Page, linkCellID);
                /*
                 * I removed the code for multi level links - we have no use for it at this point
                * We are handling multiple menu items by placing the overflow into the "More" tab
                * see commit: dfbcad39d069070f160c3f31083985f656bfaf38
                */
                if (htmlTableCell != null)
                {                   
                    string str = string.Concat(linkCellID, "Table");
                    HtmlGenericControl menuUL = (HtmlGenericControl)UtilityFunctions.FindControl(htmlTableCell, str);
                    if (menuUL != null)
                    {
                        string orientation = "Vertical";
                        HyperLink linkHyperLink = this.GetLinkHyperLink(oLink);
                        if (flag2)
                        {
                            linkHyperLink.Text = string.Concat("&nbsp;&nbsp;", linkHyperLink.Text);
                        }
                        else
                        {
                            orientation = this.GetOrientation(oLink.szPosition, oLink.szSection);
                        }
                        //add the menu list item
                        HtmlGenericControl menuItem = new HtmlGenericControl("LI");
                        menuUL.Controls.Add(menuItem);
                        if (oLink.SubMenuLinks.Count <= 0)
                        { //modified SetupLinkHyperLink
                            this.SetupLinkHyperLink(oLink, menuItem, linkHyperLink);
                        }
                    }
                }
			}
		}

		protected void AddLinks()
		{
			for (int i = 0; i < this.oLinks.Count; i++)
			{
				if (this.oLinks[i].szParentKey == "")
				{
					this.AddLink(this.oLinks[i], null);
				}
			}
		}

		protected void AddListContent(string szCellID, eWebPageClass.PaneDataClass oPaneData, bool bMultipleContent)
		{
			if (oPaneData.szContentHtml != "")
			{
				HtmlTableCell contentCell = (HtmlTableCell)UtilityFunctions.FindControl(this.Page, szCellID);
				contentCell = this.GetContentCell(contentCell, oPaneData, bMultipleContent);
				if (contentCell != null && oPaneData.bIsVisible)
				{
					HtmlGenericControl htmlGenericControl = new HtmlGenericControl("div");
					htmlGenericControl.ID = oPaneData.szKey.Replace("-", "_");
					contentCell.Controls.Add(htmlGenericControl);
					string listPagerHtml = "";
					if (oPaneData.szFormKey != "")
					{
						this.InstantiateContentFormFacadeObject(oPaneData.szFormKey, true);
						if (this.oFacadeObject != null)
						{
							if (base.bIsFirstTime)
							{
								this.oFacadeObject.SetRequestData(this, true);
							}
							oPaneData.oFacadeObject = this.oFacadeObject;
							string str = oPaneData.szCurrentKey;
							if (UtilityFunctions.EmptyString(str))
							{
								try
								{
									str = base.Request[this.oFacadeObject.GetKeyField()].ToString();
								}
								catch
								{
								}
							}
							if (!UtilityFunctions.EmptyString(str))
							{
								this.oFacadeObject.CurrentKey = str;
								this.oFacadeObject.SelectByKey();
							}
							if (!this.Page.IsPostBack && this.Page.Request["FromSearchControl"] == null)
							{
								this.oFacadeObject.ListWhere = null;
							}
							string listProperty = this.GetListProperty(ref oPaneData.szContentHtml, "{BeginListSQL}", "{EndListSQL}");
							if (UtilityFunctions.EmptyString(base.Request.QueryString["ListSearchFor"]))
							{
								listProperty = listProperty.Replace("{ListSearchTerm}", "");
								oPaneData.szContentHtml = oPaneData.szContentHtml.Replace("{ListSearchTerm}", "");
							}
							else
							{
								listProperty = listProperty.Replace("{ListSearchTerm}", base.Request.QueryString["ListSearchFor"].Replace("'", "''"));
								oPaneData.szContentHtml = oPaneData.szContentHtml.Replace("{ListSearchTerm}", base.Request.QueryString["ListSearchFor"]);
							}
							listProperty = DataUtils.ParseValues(this.oFacadeObject, listProperty, true);
							string listProperty1 = this.GetListProperty(ref oPaneData.szContentHtml, "{BeginListWhere}", "{EndListWhere}");
							listProperty1 = DataUtils.ParseValues(this.oFacadeObject, listProperty1, true);
							string str1 = this.GetListProperty(ref oPaneData.szContentHtml, "{BeginListOrderBy}", "{EndListOrderBy}");
							string listProperty2 = this.GetListProperty(ref oPaneData.szContentHtml, "{BeginListColumns}", "{EndListColumns}");
							listProperty2 = DataUtils.ParseValues(this.oFacadeObject, listProperty2, false);
							string str2 = this.GetListProperty(ref oPaneData.szContentHtml, "{BeginPagerProperties}", "{EndPagerProperties}");
							bool flag = false;
							int num = 20;
							string str3 = "0";
							if (str2 != "")
							{
								flag = true;
								try
								{
									num = Convert.ToInt32(str2);
								}
								catch
								{
									num = 20;
								}
							}
							if (base.Request["__EVENTTARGET"] != null)
							{
								string str4 = base.Request["__EVENTTARGET"].ToString();
								if (str4.StartsWith(string.Concat("Page_", oPaneData.szKey)))
								{
									str3 = base.Request["__EVENTARGUMENT"].ToString();
								}
							}
							if (listProperty != "")
							{
								this.oFacadeObject.QuerySQL = listProperty;
							}
							if (listProperty2 != "")
							{
								this.oFacadeObject.ListColumns = listProperty2;
							}
							if (listProperty1 != "")
							{
								if (UtilityFunctions.EmptyString(this.oFacadeObject.ListWhere) || base.Request["FromSearchControl"] == null)
								{
									this.oFacadeObject.ListWhere = listProperty1;
								}
								else
								{
									string[] listWhere = new string[5];
									listWhere[0] = "( ";
									listWhere[1] = this.oFacadeObject.ListWhere;
									listWhere[2] = " ) AND ( ";
									listWhere[3] = listProperty1;
									listWhere[4] = " ) ";
									this.oFacadeObject.ListWhere = string.Concat(listWhere);
								}
							}
							if (str1 != "")
							{
								this.oFacadeObject.ListOrderBy = str1;
							}
							string[] listColumns = new string[6];
							listColumns[0] = this.oFacadeObject.ListColumns;
							listColumns[1] = " ";
							listColumns[2] = this.oFacadeObject.ListWhere;
							listColumns[3] = " ";
							listColumns[4] = this.oFacadeObject.ListOrderBy;
							listColumns[5] = " ";
							this.oFacadeObject.ListFrom = QueryUtils.GetAllJoins(string.Concat(listColumns), this.oFacadeObject.GetTableName());
							OleDbConnection connection = DataUtils.GetConnection();
							RecordSet recordSet = new RecordSet();
							if (UtilityFunctions.EmptyString(listProperty2))
							{
								this.oFacadeObject.CurrentDataSet = recordSet.GetDataSet(this.oFacadeObject.QuerySQL, connection);
							}
							else
							{
								this.oFacadeObject.CurrentDataSet = recordSet.GetDataSet(this.oFacadeObject.ListQueryString, connection);
							}
							connection.Close();
							string str5 = "";
							string str6 = "";
							string listDetailHtml = this.GetListDetailHtml(oPaneData.szContentHtml, ref str5, ref str6);
							string str7 = "";
							try
							{
								DataTable item = this.oFacadeObject.CurrentDataSet.Tables[0];
								int num1 = 0;
								int count = item.Rows.Count;
								if (flag)
								{
									if (str3 != "")
									{
										num1 = Convert.ToInt32(str3) * num;
									}
									if (num1 > item.Rows.Count)
									{
										num1 = 0;
										str3 = "0";
									}
									count = num1 + num;
									if (count > item.Rows.Count)
									{
										count = item.Rows.Count;
									}
									listPagerHtml = this.GetListPagerHtml(item, num, str3, string.Concat("Page_", oPaneData.szKey));
									str5 = this.SetupListPager(str5, flag, listPagerHtml);
									listDetailHtml = this.SetupListPager(listDetailHtml, flag, listPagerHtml);
									str6 = this.SetupListPager(str6, flag, listPagerHtml);
								}
								for (int i = num1; i < count; i++)
								{
									str7 = string.Concat(str7, DataUtils.ParseValues(item.Rows[i], listDetailHtml, false));
								}
								str5 = DataUtils.ParseValues(this.oFacadeObject, str5, false);
								str5 = str5.Replace("../../eweb", Config.ApplicationPath);
								listDetailHtml = listDetailHtml.Replace("../../eweb", Config.ApplicationPath);
								str6 = DataUtils.ParseValues(this.oFacadeObject, str6, false);
								str6 = str6.Replace("../../eweb", Config.ApplicationPath);
								htmlGenericControl.InnerHtml = string.Concat(str5, str7, str6);
							}
							catch
							{
								if (Config.LastError == null || Config.LastError.Number == 0)
								{
									htmlGenericControl.InnerHtml = "No results returned.  The query may have timed out.  Please try again.";
								}
								else
								{
									htmlGenericControl.InnerHtml = string.Concat("The following error has occurred:  ", Config.LastError.Message);
								}
							}
						}
					}
					if (listPagerHtml != "")
					{
						LinkButton linkButton = new LinkButton();
						linkButton.Text = "";
						linkButton.ID = string.Concat("Hidden", oPaneData.szKey);
						htmlGenericControl.Controls.Add(linkButton);
					}
					if (oPaneData.nHeight > 0)
					{
						contentCell.Height = string.Concat(oPaneData.nHeight, oPaneData.szHeightUnit);
					}
					if (oPaneData.nWidth > 0)
					{
						contentCell.Width = string.Concat(oPaneData.nWidth, oPaneData.szWidthUnit);
					}
					if (oPaneData.szAlign != "")
					{
						contentCell.Align = oPaneData.szAlign;
					}
					if (oPaneData.szVAlign != "")
					{
						contentCell.VAlign = oPaneData.szVAlign;
					}
					if (oPaneData.szBGColor != "")
					{
						contentCell.BgColor = oPaneData.szBGColor;
					}
					this.AddDesigner(oPaneData, contentCell);
				}
			}
		}

		private void AddSubMenu(eWebPageClass.LinkClass oLink, HtmlTableCell oExpandCell, HtmlTableCell oCell)
		{
			HtmlGenericControl htmlGenericControl = new HtmlGenericControl();
			htmlGenericControl.ID = oLink.szUniqueKey;
			oCell.Controls.Add(htmlGenericControl);
			HtmlImage htmlImage = new HtmlImage();
			HtmlTable htmlTable = new HtmlTable();
			htmlTable.Border = 0;
			htmlTable.CellPadding = 0;
			htmlTable.CellSpacing = 0;
			oExpandCell.Controls.Add(htmlTable);
			oExpandCell.VAlign = "top";
			HtmlTableRow htmlTableRow = new HtmlTableRow();
			htmlTable.Rows.Add(htmlTableRow);
			HtmlTableCell htmlTableCell = new HtmlTableCell();
			htmlTableCell.Height = "15px";
			htmlTableRow.Cells.Add(htmlTableCell);
			htmlTableCell.Controls.Add(htmlImage);
			htmlImage.ID = string.Concat("Img_", htmlGenericControl.ID);
			htmlImage.Alt = "Expand/Collapse Submenu";
			htmlImage.Attributes.Add("onmouseover", "style['cursor'] = 'hand';");
			bool flag = false;
			if (base.Request.Cookies["Exp_Col"] != null && base.Request.Cookies["Exp_Col"].Value.IndexOf(string.Concat("{", htmlGenericControl.ID, "}")) > 0)
			{
				flag = true;
			}
			if (!flag)
			{
				htmlImage.Src = this.szExpandImage.Replace("~", Config.ApplicationPath);
				htmlGenericControl.Attributes.Add("style", "DISPLAY: none;");
			}
			else
			{
				htmlImage.Src = this.szCollapseImage.Replace("~", Config.ApplicationPath);
				htmlGenericControl.Attributes.Add("style", "DISPLAY: inline;");
			}
			string[] d = new string[9];
			d[0] = "expandCollapseDiv('";
			d[1] = htmlGenericControl.ID;
			d[2] = "','";
			d[3] = htmlImage.ID;
			d[4] = "','";
			d[5] = this.szExpandImage.Replace("~", Config.ApplicationPath);
			d[6] = "','";
			d[7] = this.szCollapseImage.Replace("~", Config.ApplicationPath);
			d[8] = "');";
			htmlImage.Attributes.Add("onclick", string.Concat(d));
			HtmlTable htmlTable1 = new HtmlTable();
			htmlGenericControl.Controls.Add(htmlTable1);
			htmlTable1.Border = 0;
			htmlTable1.CellPadding = 0;
			htmlTable1.CellSpacing = 0;
			for (int i = 0; i < oLink.SubMenuLinks.Count; i++)
			{
				this.AddLink(oLink.SubMenuLinks[i], htmlTable1);
			}
		}

		protected void AddSubMenuLinkNS4(eWebPageClass.LinkClass oLink, HtmlGenericControl oLayer)
		{
			bool flag;
			bool flag1 = true;
			eWebPageClass.LinkClass linkClass = oLink;
			if (oLink.bRequiresLogin)
			{
				flag = true;
			}
			else
			{
				flag = !UtilityFunctions.EmptyString(oLink.szSQLVisible);
			}
			linkClass.bRequiresLogin = flag;
			if (this.Session["CustomerKey"] != null || !oLink.bRequiresLogin)
			{
				if (this.Session["CustomerKey"] == null || !oLink.bDisableUponLogin)
				{
					if (!this.bExcludeDefaults || !oLink.bIsDefault)
					{
						if (!this.DataExists(oLink.szSQLVisible))
						{
							flag1 = false;
						}
					}
					else
					{
						flag1 = false;
					}
				}
				else
				{
					flag1 = false;
				}
			}
			else
			{
				flag1 = false;
			}
			if (flag1)
			{
				HyperLink linkHyperLink = this.GetLinkHyperLink(oLink);
				oLayer.Controls.Add(linkHyperLink);
				Literal literal = new Literal();
				literal.Text = "<br>";
				oLayer.Controls.Add(literal);
				if (oLink.SubMenuLinks.Count > 0)
				{
					this.AddSubMenuPopup(oLink, oLayer);
				}
			}
		}

		private void AddSubMenuPopup(eWebPageClass.LinkClass oLink, Control oContainerControl)
		{
			if (!this.bPopupScriptAdded)
			{
				this.bPopupScriptAdded = true;
				HtmlGenericControl htmlGenericControl = new HtmlGenericControl("script");
				htmlGenericControl.Attributes.Add("language", "javascript");
				if (Functions.GetBrowserType != Functions.BrowserType.NS4)
				{
					if (Functions.GetBrowserType != Functions.BrowserType.NS6)
					{
						htmlGenericControl.Attributes.Add("src", string.Concat(Config.ApplicationPath, "/include/ShowMenuIE.js"));
					}
					else
					{
						htmlGenericControl.Attributes.Add("src", string.Concat(Config.ApplicationPath, "/include/ShowMenuNS6.js"));
					}
				}
				else
				{
					htmlGenericControl.Attributes.Add("src", string.Concat(Config.ApplicationPath, "/include/ShowMenuNS4.js"));
				}
				oContainerControl.Controls.Add(htmlGenericControl);
			}
			string str = "left";
			if (oLink.szPosition != "Left")
			{
				if (oLink.szPosition != "Right")
				{
					if (oLink.szSection != "Right")
					{
						if (oLink.szSection != "Left")
						{
							if (oLink.szPosition != "Top")
							{
								if (oLink.szPosition != "Bottom")
								{
									if (oLink.szSection != "Top")
									{
										if (oLink.szSection == "Bottom")
										{
											str = "bottom";
										}
									}
									else
									{
										str = "top";
									}
								}
								else
								{
									str = "bottom";
								}
							}
							else
							{
								str = "top";
							}
						}
						else
						{
							str = "left";
						}
					}
					else
					{
						str = "right";
					}
				}
				else
				{
					str = "right";
				}
			}
			else
			{
				str = "left";
			}
			HtmlTable htmlTable = new HtmlTable();
			htmlTable.Attributes.Add("class", "PopupMenuSubMenuTable");
			htmlTable.Border = 0;
			htmlTable.CellPadding = 0;
			htmlTable.CellSpacing = 0;
			string popupID = this.GetPopupID();
			if (Functions.GetBrowserType != Functions.BrowserType.NS4)
			{
				if (Functions.GetBrowserType != Functions.BrowserType.NS6)
				{
					HtmlGenericControl htmlGenericControl1 = new HtmlGenericControl("div");
					htmlGenericControl1.ID = string.Concat(popupID, "BarS");
					htmlGenericControl1.Attributes.Add("style", "position:absolute;");
					oContainerControl.Controls.Add(htmlGenericControl1);
					HtmlGenericControl htmlGenericControl2 = new HtmlGenericControl("div");
					htmlGenericControl2.ID = string.Concat(popupID, "Bar");
					string[] strArrays = new string[5];
					strArrays[0] = "ShowMenu('";
					strArrays[1] = popupID;
					strArrays[2] = "','show','";
					strArrays[3] = str;
					strArrays[4] = "');";
					htmlGenericControl2.Attributes.Add("onMouseOver", string.Concat(strArrays));
					htmlGenericControl2.Attributes.Add("onMouseOut", string.Concat("ShowMenu('", popupID, "','hide');"));
					oContainerControl.Controls.Add(htmlGenericControl2);
					HyperLink linkHyperLink = this.GetLinkHyperLink(oLink);
					this.SetupLinkHyperLink(oLink, htmlGenericControl2, linkHyperLink);
					HtmlGenericControl htmlGenericControl3 = new HtmlGenericControl("div");
					htmlGenericControl3.ID = popupID;
					htmlGenericControl3.Attributes.Add("style", "position:absolute; visibility:hidden; z-index:65535;");
					string[] strArrays1 = new string[5];
					strArrays1[0] = "ShowMenu('";
					strArrays1[1] = popupID;
					strArrays1[2] = "','show','";
					strArrays1[3] = str;
					strArrays1[4] = "');";
					htmlGenericControl3.Attributes.Add("onMouseOver", string.Concat(strArrays1));
					htmlGenericControl3.Attributes.Add("onMouseOut", string.Concat("ShowMenu('", popupID, "','hide');"));
					oContainerControl.Controls.Add(htmlGenericControl3);
					htmlGenericControl3.Controls.Add(htmlTable);
					for (int i = 0; i < oLink.SubMenuLinks.Count; i++)
					{
						this.AddLink(oLink.SubMenuLinks[i], htmlTable);
						if (oLink.SubMenuLinks[i].szCssClass.IndexOf(";") > 0)
						{
							htmlGenericControl3.Attributes.Remove("class");
							htmlGenericControl3.Attributes.Add("class", oLink.SubMenuLinks[i].szCssClass.Substring(0, oLink.SubMenuLinks[i].szCssClass.IndexOf(";")));
						}
					}
					return;
				}
				else
				{
					HtmlGenericControl htmlGenericControl4 = new HtmlGenericControl("div");
					htmlGenericControl4.ID = string.Concat(popupID, "Bar");
					htmlGenericControl4.Attributes.Add("style", "position:relative;");
					string[] strArrays2 = new string[5];
					strArrays2[0] = "ShowMenu('";
					strArrays2[1] = popupID;
					strArrays2[2] = "','show','";
					strArrays2[3] = str;
					strArrays2[4] = "');";
					htmlGenericControl4.Attributes.Add("onMouseOver", string.Concat(strArrays2));
					htmlGenericControl4.Attributes.Add("onMouseOut", string.Concat("ShowMenu('", popupID, "','hide');"));
					oContainerControl.Controls.Add(htmlGenericControl4);
					HyperLink hyperLink = this.GetLinkHyperLink(oLink);
					this.SetupLinkHyperLink(oLink, htmlGenericControl4, hyperLink);
					HtmlGenericControl htmlGenericControl5 = new HtmlGenericControl("div");
					htmlGenericControl5.ID = popupID;
					htmlGenericControl5.Attributes.Add("style", "position:absolute; visibility:hidden;  z-index:65535;");
					string[] strArrays3 = new string[5];
					strArrays3[0] = "ShowMenu('";
					strArrays3[1] = popupID;
					strArrays3[2] = "','show','";
					strArrays3[3] = str;
					strArrays3[4] = "');";
					htmlGenericControl5.Attributes.Add("onMouseOver", string.Concat(strArrays3));
					htmlGenericControl5.Attributes.Add("onMouseOut", string.Concat("ShowMenu('", popupID, "','hide');"));
					oContainerControl.Controls.Add(htmlGenericControl5);
					htmlGenericControl5.Controls.Add(htmlTable);
					for (int j = 0; j < oLink.SubMenuLinks.Count; j++)
					{
						this.AddLink(oLink.SubMenuLinks[j], htmlTable);
					}
					return;
				}
			}
			else
			{
				HtmlGenericControl htmlGenericControl6 = new HtmlGenericControl("ilayer");
				htmlGenericControl6.ID = string.Concat(popupID, "BarIL");
				oContainerControl.Controls.Add(htmlGenericControl6);
				HtmlGenericControl htmlGenericControl7 = new HtmlGenericControl("layer");
				htmlGenericControl7.ID = string.Concat(popupID, "BarL");
				htmlGenericControl6.Controls.Add(htmlGenericControl7);
				string[] strArrays4 = new string[5];
				strArrays4[0] = "ShowMenu('";
				strArrays4[1] = popupID;
				strArrays4[2] = "','show','";
				strArrays4[3] = str;
				strArrays4[4] = "');";
				htmlGenericControl7.Attributes.Add("onMouseOver", string.Concat(strArrays4));
				htmlGenericControl7.Attributes.Add("onMouseOut", string.Concat("ShowMenu('", popupID, "','hide');"));
				HyperLink linkHyperLink1 = this.GetLinkHyperLink(oLink);
				this.SetupLinkHyperLink(oLink, htmlGenericControl7, linkHyperLink1);
				HtmlGenericControl htmlGenericControl8 = new HtmlGenericControl("layer");
				htmlGenericControl8.ID = popupID;
				htmlGenericControl8.Attributes.Add("visibility", "hidden");
				oContainerControl.Controls.Add(htmlGenericControl8);
				HtmlGenericControl htmlGenericControl9 = new HtmlGenericControl("layer");
				htmlGenericControl9.ID = string.Concat(popupID, "L");
				htmlGenericControl8.Controls.Add(htmlGenericControl9);
				string[] strArrays5 = new string[5];
				strArrays5[0] = "ShowMenu('";
				strArrays5[1] = popupID;
				strArrays5[2] = "','show','";
				strArrays5[3] = str;
				strArrays5[4] = "');";
				htmlGenericControl9.Attributes.Add("onMouseOver", string.Concat(strArrays5));
				htmlGenericControl9.Attributes.Add("onMouseOut", string.Concat("ShowMenu('", popupID, "','hide');"));
				for (int k = 0; k < oLink.SubMenuLinks.Count; k++)
				{
					this.AddSubMenuLinkNS4(oLink.SubMenuLinks[k], htmlGenericControl9);
				}
				return;
			}
		}

		protected void AddWizardContent(string szCellID, eWebPageClass.PaneDataClass oPaneData, bool bMultipleContent)
		{
			this.AddWizardContent(szCellID, oPaneData, bMultipleContent, false);
		}

		protected void AddWizardContent(string szCellID, eWebPageClass.PaneDataClass oPaneData, bool bMultipleContent, bool bBundleWizard)
		{
			WizardClass wizardClass;
			string str = oPaneData.szWizardKey;
			if (str != "")
			{
				HtmlTableCell contentCell = (HtmlTableCell)UtilityFunctions.FindControl(this.Page, szCellID);
				contentCell = this.GetContentCell(contentCell, oPaneData, bMultipleContent);
				if (contentCell != null && oPaneData.bIsVisible)
				{
					if (!bBundleWizard)
					{
						wizardClass = new WizardClass();
					}
					else
					{
						wizardClass = new COEBundleWizardClass_EWEB();
					}
					wizardClass.szAspxFileName = "DynamicPage.aspx";
					wizardClass.szWebKey = this.szWebContentKey;
					if (base.bIsFirstTime)
					{
						if (base.Request.UrlReferrer == null)
						{
							this.Session[string.Concat("WizardUrlReferrer_", str)] = null;
						}
						else
						{
							if (base.Request.Url.ToString().Trim().ToLower().IndexOf(wizardClass.szAspxFileName.ToLower()) < 0)
							{
								this.Session[string.Concat("WizardUrlReferrer_", str)] = base.Request.UrlReferrer.ToString();
							}
						}
					}
					wizardClass.szWizardKey = str;
					wizardClass.oPage = (PageEditClass)this.Page;
					wizardClass.InitializeWizardProperties();
					if (UtilityFunctions.EmptyString(base.szFormKey))
					{
						base.szFormKey = oPaneData.szFormKey;
					}
					this.InstantiateContentFormFacadeObject(base.szFormKey, false, true);
					if (this.oFacadeObject != null)
					{
						oPaneData.oFacadeObject = this.oFacadeObject;
						if (base.bIsFirstTime && base.szAddEditMode == "add" && this.bHasRelatedObjects && base.Request["action"] != null && base.Request["action"].ToLower().Trim() == "add")
						{
							if (base.Request["PFKey"] == null || base.Request["PKey"] == null)
							{
								this.oFacadeObject.GetRelatedObjectValues(this.Session, (PageClass)this.Page);
							}
							else
							{
								FacadeClass facadeClass = base.InstantiateRelatedObject(base.Request["PFKey"].ToString(), base.Request["PKey"].ToString());
								this.oFacadeObject.GetRelatedObjectValues(this.Session, (PageClass)this.Page, facadeClass.ObjectKey);
							}
						}
						this.oFacadeObject.ClearLastValueChangedFlag();
						if (this.Page.Request["ButtonCancel"] == null || this.Page.Request["ButtonCancel"] != "Cancel")
						{
							this.oFacadeObject.SetRequestData(this.Page);
						}
						if (base.bIsFirstTime && base.szAddEditMode == "add")
						{
							this.oFacadeObject.SetRequestData(this.Page, true);
						}
						base.InitLookupValues(this.Page);
						this.oFacadeObject.LoadRelatedData();
						if (base.bIsFirstTime && base.Request["RecNo"] == null)
						{
							UtilityFunctions.SetUrlReferrer(base.Request, this.ViewState);
							DataUtils.SetUrlReferrer(this.oFacadeObject, base.Request);
							if (base.szAddEditMode == "edit")
							{
								this.oFacadeObject.GetData(this.Page);
							}
						}
						base.SetDefaultValues();
					}
					if (this.Page.IsPostBack && Config.LastError != null)
					{
						this.oPostbackError = Config.LastError;
					}
					if (HttpContext.Current.Session[wizardClass.szProgressObjectName] != null)
					{
						wizardClass.oProgressFacadeObject = (FacadeClass)HttpContext.Current.Session[wizardClass.szProgressObjectName];
						wizardClass.szProgressObjectCurrentKey = wizardClass.oProgressFacadeObject.CurrentKey;
					}
					wizardClass.BuildWizardContent(contentCell, false);
					wizardClass.BuildProgressBar("eWebLeftPaneTableCell");
					if (oPaneData.nHeight > 0)
					{
						contentCell.Height = string.Concat(oPaneData.nHeight, oPaneData.szHeightUnit);
					}
					if (oPaneData.nWidth > 0)
					{
						contentCell.Width = string.Concat(oPaneData.nWidth, oPaneData.szWidthUnit);
					}
					this.AddDesigner(oPaneData, contentCell);
					this.Session["oWizard"] = wizardClass;
				}
				this.HideUI = true;
			}
		}

		protected void BuildFormTable(HtmlTableCell oContentCell)
		{
			ValidationSummary validationSummary;
			int num = 3;
			string str = "&nbsp;&nbsp;";
			HtmlTable htmlTable = new HtmlTable();
			htmlTable.CellPadding = 0;
			htmlTable.CellSpacing = 0;
			htmlTable.Border = 0;
			htmlTable.ID = "DataFormTable";
			HtmlTableRow htmlTableRow = new HtmlTableRow();
			htmlTableRow.Attributes.Add("class", "DataFormHeaderTR");
			HtmlTableCell htmlTableCell = new HtmlTableCell();
			htmlTableCell.Attributes.Add("class", "DataFormHeaderTD");
			htmlTableCell.NoWrap = true;
			htmlTableCell.Align = "left";
			htmlTableCell.VAlign = "top";
			htmlTableCell.ID = "LabelDataFormHeaderCell";
			Label label = new Label();
			label.CssClass = "DataFormLabelHeader";
			label.ID = "LabelDataFormHeader";
			htmlTableCell.Controls.Add(label);
			htmlTableRow.Cells.Add(htmlTableCell);
			htmlTableRow = new HtmlTableRow();
			htmlTableCell = new HtmlTableCell();
			htmlTableCell.Attributes.Add("class", "DataFormSubHeaderTD");
			htmlTableCell.ColSpan = num;
			htmlTableCell.ID = "ItemLinksCell";
			htmlTableRow.Cells.Add(htmlTableCell);
			htmlTable.Rows.Add(htmlTableRow);
			htmlTableRow = new HtmlTableRow();
			htmlTableRow.Attributes.Add("class", "DataFormValidationTR");
			htmlTableCell = new HtmlTableCell();
			htmlTableCell.Attributes.Add("class", "DataFormValidationTD");
			htmlTableCell.ColSpan = num;
			if (UtilityFunctions.EmptyString(Config.GetSystemOption("ValidationSummaryPosition")) || Config.GetSystemOption("ValidationSummaryPosition").ToLower().Trim() == "top" || Config.GetSystemOption("ValidationSummaryPosition").ToLower().Trim() == "both")
			{
				validationSummary = new ValidationSummary();
				validationSummary.CssClass = "DataFormValidationSummary";
				validationSummary.ID = "ValidationSummaryTop";
				htmlTableCell.Controls.Add(validationSummary);
			}
			htmlTableRow.Controls.Add(htmlTableCell);
			htmlTable.Controls.Add(htmlTableRow);
			htmlTableRow = new HtmlTableRow();
			htmlTableRow.Attributes.Add("class", "DataFormErrorMessageTR");
			htmlTableCell = new HtmlTableCell();
			htmlTableCell.Attributes.Add("class", "DataFormErrorMessageTD");
			htmlTableCell.ColSpan = num;
			Label label1 = new Label();
			label1.CssClass = "DataformLabelErrorMessage";
			label1.ID = "LabelMessageTop";
			label1.Visible = false;
			htmlTableCell.Controls.Add(label1);
			htmlTableRow.Controls.Add(htmlTableCell);
			htmlTable.Controls.Add(htmlTableRow);
			if (!this.bDesignedForm)
			{
				htmlTableRow = new HtmlTableRow();
				htmlTableRow.ID = "StandardFieldsRow";
				htmlTableCell = new HtmlTableCell();
				htmlTableCell.Attributes.Add("class", "DataFormSubHeaderTD");
				htmlTableCell.ColSpan = num;
				label = new Label();
				label.CssClass = "DataFormLabelSubHeader";
				label.ID = "LabelStandardFields";
				label.Text = "Standard Fields";
				htmlTableCell.Controls.Add(label);
				htmlTableRow.Controls.Add(htmlTableCell);
				htmlTable.Controls.Add(htmlTableRow);
				htmlTableRow = new HtmlTableRow();
				htmlTableRow.ID = "ExtendedFieldsRow";
				htmlTableCell = new HtmlTableCell();
				htmlTableCell.Attributes.Add("class", "DataFormSubHeaderTD");
				htmlTableCell.ColSpan = num;
				label = new Label();
				label.CssClass = "DataFormLabelSubHeader";
				label.ID = "LabelExtendedFields";
				label.Text = "Extended Fields";
				htmlTableCell.Controls.Add(label);
				htmlTableRow.Controls.Add(htmlTableCell);
				htmlTable.Controls.Add(htmlTableRow);
			}
			else
			{
				htmlTableRow = new HtmlTableRow();
				htmlTableRow.ID = "DesignedRowOuter";
				htmlTableCell = new HtmlTableCell();
				htmlTableCell.ID = "DesignedCellOuter";
				htmlTableCell.ColSpan = num;
				HtmlTable htmlTable1 = new HtmlTable();
				htmlTable1.CellPadding = 0;
				htmlTable1.CellSpacing = 0;
				htmlTable1.Border = 0;
				htmlTable1.Attributes.Add("class", "DesignedTABLE");
				htmlTable1.ID = "DesignedTable";
				HtmlTableRow htmlTableRow1 = new HtmlTableRow();
				htmlTableRow1.ID = "DesignedRow";
				htmlTableRow1.Attributes.Add("class", "DesignedRowTR");
				HtmlTableCell htmlTableCell1 = new HtmlTableCell();
				htmlTableCell1.ID = "DesignedCell";
				htmlTableCell1.Attributes.Add("class", "DesignedRowTD");
				htmlTableRow1.Cells.Add(htmlTableCell1);
				htmlTable1.Rows.Add(htmlTableRow1);
				htmlTableCell.Controls.Add(htmlTable1);
				htmlTableRow.Cells.Add(htmlTableCell);
				htmlTable.Rows.Add(htmlTableRow);
			}
			htmlTableRow = new HtmlTableRow();
			htmlTableCell = new HtmlTableCell();
			htmlTableCell.ColSpan = num;
			htmlTableRow.Controls.Add(htmlTableCell);
			htmlTable.Controls.Add(htmlTableRow);
			htmlTableRow = new HtmlTableRow();
			htmlTableRow.Attributes.Add("class", "DataFormValidationTR");
			htmlTableCell = new HtmlTableCell();
			htmlTableCell.Attributes.Add("class", "DataFormValidationTD");
			htmlTableCell.ColSpan = num;
			if (UtilityFunctions.EmptyString(Config.GetSystemOption("ValidationSummaryPosition")) || Config.GetSystemOption("ValidationSummaryPosition").ToLower().Trim() == "bottom" || Config.GetSystemOption("ValidationSummaryPosition").ToLower().Trim() == "both")
			{
				validationSummary = new ValidationSummary();
				validationSummary.CssClass = "DataFormValidationSummary";
				validationSummary.ID = "ValidationSummaryBottom";
				htmlTableCell.Controls.Add(validationSummary);
			}
			htmlTableRow.Controls.Add(htmlTableCell);
			htmlTable.Controls.Add(htmlTableRow);
			htmlTableRow = new HtmlTableRow();
			htmlTableRow.Attributes.Add("class", "DataFormErrorMessageTR");
			htmlTableCell = new HtmlTableCell();
			htmlTableCell.Attributes.Add("class", "DataFormErrorMessageTD");
			htmlTableCell.ColSpan = num;
			label = new Label();
			label.CssClass = "DataformLabelErrorMessage";
			label.ID = "LabelMessage";
			label.Visible = false;
			htmlTableCell.Controls.Add(label);
			htmlTableRow.Controls.Add(htmlTableCell);
			htmlTable.Controls.Add(htmlTableRow);
			htmlTableRow = new HtmlTableRow();
			htmlTableRow.Attributes.Add("class", "DataFormFooterTR");
			htmlTableCell = new HtmlTableCell();
			htmlTableCell.Align = "right";
			htmlTableCell.Attributes.Add("class", "DataFormFooterTD");
			htmlTableCell.ColSpan = num;
			Button button = new Button();
			button.CssClass = "DataFormButton";
			button.ID = "ButtonSave";
			button.Text = "Save";
			button.CausesValidation = true;
			button.AccessKey = "S";
			button.Click += new EventHandler(this.ButtonSave_Click);
			htmlTableCell.Controls.Add(button);
			Literal literal = new Literal();
			literal.Text = str;
			htmlTableCell.Controls.Add(literal);
			button = new Button();
			button.CssClass = "DataFormButton";
			button.ID = "ButtonCancel";
			button.Text = "Cancel";
			button.CausesValidation = false;
			button.AccessKey = "C";
			button.Click += new EventHandler(this.ButtonCancel_Click);
			htmlTableCell.Controls.Add(button);
			htmlTableRow.Controls.Add(htmlTableCell);
			htmlTable.Controls.Add(htmlTableRow);
			oContentCell.Controls.Add(htmlTable);
		}

		protected void BuildPageContent()
		{
			string str;
			ArrayList item;
			ArrayList arrayLists;
			int num;
			int num1;
			int num2;
			int num3;
			int num4;
			int num5;
			if (this.szWebSiteKey != "")
			{
				if (this.szWebContentKey != "")
				{
					str = string.Concat(" WHERE wbc_key = ", DataUtils.ValuePrep(this.szWebContentKey, "av_key", true), " AND wbd_delete_flag=0");
				}
				else
				{
					if (this.szWebContentCode != "")
					{
						string[] strArrays = new string[5];
						strArrays[0] = " WHERE wbc_web_key=";
						strArrays[1] = DataUtils.ValuePrep(this.szWebSiteKey, "av_key", true);
						strArrays[2] = " AND wbc_code=";
						strArrays[3] = DataUtils.ValuePrep(this.szWebContentCode, "nvarchar", true);
						strArrays[4] = " AND wbd_delete_flag=0";
						str = string.Concat(strArrays);
					}
					else
					{
						string[] strArrays1 = new string[5];
						strArrays1[0] = " WHERE wbc_web_key=";
						strArrays1[1] = DataUtils.ValuePrep(this.szWebSiteKey, "av_key", true);
						strArrays1[2] = " AND wbc_page_name=";
						strArrays1[3] = DataUtils.ValuePrep(base.GetFileName(), "nvarchar", true);
						strArrays1[4] = " AND wbd_delete_flag=0";
						str = string.Concat(strArrays1);
					}
				}
				string[] strArrays2 = new string[11];
				strArrays2[0] = "SELECT *  FROM md_web_content_detail ";
				strArrays2[1] = DataUtils.NoLock();
				strArrays2[2] = " JOIN md_web_content ";
				strArrays2[3] = DataUtils.NoLock();
				strArrays2[4] = " ON wbc_key=wbd_wbc_key LEFT JOIN md_web_section ";
				strArrays2[5] = DataUtils.NoLock();
				strArrays2[6] = " ON wbs_key=wbc_wbs_key JOIN md_web ";
				strArrays2[7] = DataUtils.NoLock();
				strArrays2[8] = " ON web_key=wbc_web_key";
				strArrays2[9] = str;
				strArrays2[10] = " ORDER BY wbd_position,wbd_row,wbd_col";
				string str1 = string.Concat(strArrays2);
				bool flag = false;
				bool flag1 = false;
				bool flag2 = false;
				OleDbConnection connection = DataUtils.GetConnection();
				using (connection)
				{
					OleDbDataReader dataReader = DataUtils.GetDataReader(str1, connection);
					using (dataReader)
					{
						bool flag3 = true;
						if (dataReader != null)
						{
							try
							{
								while (dataReader.Read())
								{
									if (!flag)
									{
										flag = true;
									}
									if (!(dataReader["wbs_ssl_flag"].ToString() == "1") || this.szHttp.StartsWith("https:"))
									{
										if ((dataReader["wbc_requires_login"].ToString() == "1" || dataReader["wbs_requires_login"].ToString() == "1") && this.Session["CustomerKey"] == null)
										{
											flag2 = true;
											break;
										}
										else
										{
											if (this.szWebContentKey == "")
											{
												this.szWebContentKey = dataReader["wbc_key"].ToString();
											}
											if (this.szWebContentCode == "")
											{
												this.szWebContentCode = dataReader["wbc_code"].ToString();
											}
											this.szSectionKey = dataReader["wbc_wbs_key"].ToString();
											if (dataReader["wbc_exclude_defaults"].ToString() == "1")
											{
												this.bExcludeDefaults = true;
											}
											if (dataReader["wbs_exclude_defaults"].ToString() == "1")
											{
												this.bSectionExcludeDefaults = true;
											}
											this.szMetatagKeyWords = dataReader["wbc_metatag_keywords"].ToString();
											this.szBodyAttributesSite = dataReader["web_body_attributes"].ToString();
											this.szBodyAttributesContent = dataReader["wbc_body_attributes"].ToString();
											if (dataReader["wbc_ignore_sql_visible"].ToString() != "1")
											{
												this.szSQLVisible = dataReader["wbc_sql_visible"].ToString();
												if (this.szSQLVisible == "")
												{
													this.szSQLVisible = dataReader["wbs_sql_visible"].ToString();
												}
											}
											else
											{
												this.bIgnoreSQLVisible = true;
												this.szSQLVisible = "";
											}
											this.szDenyContentKey = dataReader["wbc_deny_wbc_key"].ToString();
											if (this.szDenyContentKey == "")
											{
												this.szDenyContentKey = dataReader["wbs_deny_wbc_key"].ToString();
											}
											this.szFromEmail = dataReader["wbs_email_from"].ToString();
											if (this.szFromEmail == "")
											{
												this.szFromEmail = dataReader["web_email_from"].ToString();
											}
											string lower = dataReader["wbd_position"].ToString().ToLower();
											if (dataReader["wbc_page_title"].ToString() == "")
											{
												this.bTitleFlag = false;
											}
											else
											{
												this.bTitleFlag = true;
												this.PageTitle = dataReader["wbc_page_title"].ToString();
											}
											if (flag3)
											{
												flag3 = false;
												if (this.bSectionExcludeDefaults)
												{
													this.hashPanes.Clear();
												}
												this.GetSectionPaneDefaults();
											}
											if (this.hashPanes[lower] != null)
											{
												item = (ArrayList)this.hashPanes[lower];
											}
											else
											{
												item = new ArrayList();
												this.hashPanes[lower] = item;
											}
											eWebPageClass.PaneDataClass paneDataClass = new eWebPageClass.PaneDataClass();
											this.GetPaneData(dataReader, "wbd", paneDataClass);
											paneDataClass.szDestinationKey = dataReader["wbd_dest_wbc_key"].ToString();
											paneDataClass.szContentPath = dataReader["wbd_content_path"].ToString();
											paneDataClass.szPosition = lower;
											eWebPageClass.PaneDataClass paneDataClass1 = paneDataClass;
											if (dataReader["wbd_row"].ToString() == "")
											{
												num = 1;
											}
											else
											{
												num = Convert.ToInt32(dataReader["wbd_row"].ToString());
											}
											paneDataClass1.nRow = num;
											eWebPageClass.PaneDataClass paneDataClass2 = paneDataClass;
											if (dataReader["wbd_col"].ToString() == "")
											{
												num1 = 0;
											}
											else
											{
												num1 = Convert.ToInt32(dataReader["wbd_col"].ToString());
											}
											paneDataClass2.nCol = num1;
											eWebPageClass.PaneDataClass paneDataClass3 = paneDataClass;
											if (dataReader["wbd_rowspan"].ToString() == "")
											{
												num2 = 0;
											}
											else
											{
												num2 = Convert.ToInt32(dataReader["wbd_rowspan"].ToString());
											}
											paneDataClass3.nRowSpan = num2;
											eWebPageClass.PaneDataClass paneDataClass4 = paneDataClass;
											if (dataReader["wbd_colspan"].ToString() == "")
											{
												num3 = 0;
											}
											else
											{
												num3 = Convert.ToInt32(dataReader["wbd_colspan"].ToString());
											}
											paneDataClass4.nColSpan = num3;
											eWebPageClass.PaneDataClass paneDataClass5 = paneDataClass;
											if (dataReader["wbd_content_height"].ToString() == "")
											{
												num4 = 0;
											}
											else
											{
												num4 = Convert.ToInt32(dataReader["wbd_content_height"].ToString());
											}
											paneDataClass5.nHeight = num4;
											paneDataClass.szHeightUnit = dataReader["wbd_content_height_unit"].ToString();
											eWebPageClass.PaneDataClass paneDataClass6 = paneDataClass;
											if (dataReader["wbd_content_width"].ToString() == "")
											{
												num5 = 0;
											}
											else
											{
												num5 = Convert.ToInt32(dataReader["wbd_content_width"].ToString());
											}
											paneDataClass6.nWidth = num5;
											paneDataClass.szWidthUnit = dataReader["wbd_content_width_unit"].ToString();
											if (paneDataClass.szHeightUnit == "")
											{
												paneDataClass.szHeightUnit = "px";
											}
											if (paneDataClass.szWidthUnit == "")
											{
												paneDataClass.szWidthUnit = "px";
											}
											paneDataClass.szVAlign = dataReader["wbd_content_align"].ToString();
											string str2 = dataReader["wbd_sql_visible"].ToString();
											if (str2 != "" && !this.DataExists(str2))
											{
												paneDataClass.bIsVisible = false;
											}
											paneDataClass.szParentDetailKey = dataReader["wbd_wbd_key"].ToString();
											item.Add(paneDataClass);
										}
									}
									else
									{
										flag1 = true;
										break;
									}
								}
							}
							catch (Exception exception1)
							{
								Exception exception = exception1;
								Config.LastError = new ErrorClass();
								Config.LastError.Message = exception.Message;
								if (exception.InnerException != null && exception.InnerException.Message != null)
								{
									ErrorClass lastError = Config.LastError;
									lastError.Message = string.Concat(lastError.Message, "; ", exception.InnerException.Message);
								}
								Config.LastError.Number = -1;
							}
						}
					}
				}
				if (!flag1)
				{
					if (!flag2)
					{
						if (base.Request["PrinterFriendly"] != null)
						{
							this.bExcludeDefaults = true;
						}
						this.AddBodyAttributes(this.szBodyAttributesSite);
						this.AddBodyAttributes(this.szBodyAttributesContent);
						if (flag || !(this.szWebContentCode.ToLower() == "loginrequired"))
						{
							if (flag)
							{
								if (this.szSQLVisible != "" && !this.DataExists(this.szSQLVisible))
								{
									if (this.szDenyContentKey != "")
									{
										string contentURL = Functions.GetContentURL(this.szDenyContentKey);
										if (contentURL != "")
										{
											PageUtils.Redirect(contentURL);
										}
									}
									foreach (object hashPane in this.hashPanes)
									{
										DictionaryEntry dictionaryEntry = (DictionaryEntry)hashPane;
										string str3 = dictionaryEntry.Key.ToString();
										if (str3 != "content")
										{
											continue;
										}
										ArrayList arrayLists1 = new ArrayList();
										eWebPageClass.PaneDataClass paneDataClass7 = new eWebPageClass.PaneDataClass();
										paneDataClass7.szContentType = "html";
										paneDataClass7.szContentHtml = "<HTML><br><br><br><span class=\"DataFormLabel\">Access Denied..</span></HTML>";
										paneDataClass7.szPosition = str3;
										paneDataClass7.nRow = 1;
										paneDataClass7.nCol = 1;
										paneDataClass7.nRowSpan = 1;
										paneDataClass7.nColSpan = 1;
										paneDataClass7.nHeight = 0;
										paneDataClass7.szHeightUnit = "px";
										paneDataClass7.nWidth = 0;
										paneDataClass7.szWidthUnit = "px";
										arrayLists1.Add(paneDataClass7);
										this.hashPanes.Remove(str3);
										this.hashPanes[str3] = arrayLists1;
										break;
									}
								}
							}
							else
							{
								if (base.GetFileName().ToLower() != "startpage.aspx")
								{
									PageUtils.Redirect("~/StartPage.aspx");
									return;
								}
								else
								{
									base.Response.Write("Default Home Page (StartPage.aspx) not found...<br>");
									return;
								}
							}
						}
						else
						{
							this.bExcludeDefaults = false;
							string str4 = "content";
							if (this.hashPanes[str4] != null)
							{
								arrayLists = (ArrayList)this.hashPanes[str4];
							}
							else
							{
								arrayLists = new ArrayList();
								this.hashPanes[str4] = arrayLists;
							}
							eWebPageClass.PaneDataClass paneDataClass8 = new eWebPageClass.PaneDataClass();
							paneDataClass8.szContentType = "control";
							paneDataClass8.szControlPath = "~/controls/login.ascx";
							paneDataClass8.szPosition = str4;
							paneDataClass8.nRow = 1;
							paneDataClass8.nCol = 1;
							paneDataClass8.nRowSpan = 1;
							paneDataClass8.nColSpan = 1;
							paneDataClass8.nHeight = 0;
							paneDataClass8.szHeightUnit = "px";
							paneDataClass8.nWidth = 0;
							paneDataClass8.szWidthUnit = "px";
							arrayLists.Add(paneDataClass8);
						}
						string str5 = null;
						foreach (object obj in this.hashPanes)
						{
							DictionaryEntry dictionaryEntry1 = (DictionaryEntry)obj;
							string paneID = this.GetPaneID(dictionaryEntry1.Key.ToString());
							DictionaryEntry dictionaryEntry2 = (DictionaryEntry)obj;
							ArrayList item1 = (ArrayList)this.hashPanes[dictionaryEntry2.Key];
							bool flag4 = false;
							if (item1.Count > 1)
							{
								flag4 = true;
							}
							for (int i = 0; i < item1.Count; i++)
							{
								eWebPageClass.PaneDataClass item2 = (eWebPageClass.PaneDataClass)item1[i];
								if (!this.bExcludeDefaults || !item2.bIsDefault)
								{
									this.GetPaneDataFromParent(item2);
									if (item2.szContentType != "html")
									{
										if (item2.szContentType != "file")
										{
											if (item2.szContentType != "form")
											{
												if (!item2.szContentType.StartsWith("control"))
												{
													if (item2.szContentType != "iframe")
													{
														if (item2.szContentType != "list")
														{
															if (item2.szContentType != "profile")
															{
																if (item2.szContentType != "wizard")
																{
																	if (item2.szContentType == "bundlewizard")
																	{
																		this.AddWizardContent(paneID, item2, flag4, true);
																	}
																}
																else
																{
																	this.AddWizardContent(paneID, item2, flag4);
																}
															}
															else
															{
																if (base.Request["FormKey"] != null)
																{
																	base.szFormKey = base.Request["Formkey"].ToString();
																}
																else
																{
																	base.szFormKey = item2.szFormKey;
																}
																if (item2.oFacadeObject == null && !UtilityFunctions.EmptyString(item2.szFormKey))
																{
																	this.InstantiateContentFormFacadeObject(item2.szFormKey, false, true);
																	item2.oFacadeObject = this.oFacadeObject;
																	this.oLastFormFacadeObject = item2.oFacadeObject;
																	str5 = item2.szFormKey;
																}
															}
														}
														else
														{
															this.AddListContent(paneID, item2, flag4);
														}
													}
													else
													{
														this.AddIFrameContent(paneID, item2, flag4);
													}
												}
												else
												{
													this.AddControlContent(paneID, item2, flag4);
												}
											}
											else
											{
												this.AddFormContent(paneID, item2, flag4);
											}
										}
										else
										{
											this.AddFileContent(paneID, item2, flag4);
										}
									}
									else
									{
										this.AddHtmlContent(paneID, item2, flag4);
									}
								}
							}
						}
						if (this.oLastFormFacadeObject != null)
						{
							this.oFacadeObject = this.oLastFormFacadeObject;
						}
						if (str5 != null)
						{
							base.szFormKey = str5;
						}
						return;
					}
					else
					{
						this.Session["IntendedURL"] = base.Request.Url.ToString();
						string str6 = "~/DynamicPage.aspx?WebCode=LoginRequired&expires=yes";
						if (this.ModalForm)
						{
							str6 = string.Concat(str6, "&Modal=Yes");
						}
						PageUtils.Redirect(str6, true);
						return;
					}
				}
				else
				{
					string str7 = base.Request.Url.AbsoluteUri.Replace("http:", "https:");
					PageUtils.Redirect(str7, true);
					return;
				}
			}
			else
			{
				base.Response.Write("Unable to load a valid site...");
				if (!UtilityFunctions.EmptyString(this.szWebSiteCode))
				{
					base.Response.Write(string.Concat(" Site: <b>", base.Server.HtmlEncode(this.szWebSiteCode), "</b> Does Not Exist..."));
				}
				base.Response.Write("<br>");
				return;
			}
		}

		private void ButtonCancel_Click(object sender, EventArgs e)
		{
			string uRL = "~/StartPage.aspx";
			if (this.Session["IntendedURL"] == null)
			{
				base.Cancel_ClickBase(this.Page);
			}
			if (!this.ModalForm)
			{
				if (this.Session["IntendedURL"] != null)
				{
					uRL = Functions.AppendSiteCodeToURL(this.Session["IntendedURL"].ToString());
					this.Session["IntendedURL"] = null;
				}
			}
			else
			{
				uRL = "~/CloseWindow.htm?NoReload=Yes";
			}
			PageUtils.Redirect(uRL);
		}

		private void ButtonSave_Click(object sender, EventArgs e)
		{
			Button button = (Button)sender;
			if (base.IsValid && Config.LastError == null)
			{
				base.Save_ClickBase(this.Page, "", true, button.ID);
				if (Config.LastError == null)
				{
					string uRL = "~/StartPage.aspx";
					if (!this.ModalForm)
					{
						if (this.Session["IntendedURL"] != null)
						{
							uRL = Functions.AppendSiteCodeToURL(this.Session["IntendedURL"].ToString());
							this.Session["IntendedURL"] = null;
						}
					}
					else
					{
						uRL = "~/CloseWindow.htm?OpenerSubmit=Yes";
					}
					PageUtils.Redirect(uRL);
				}
			}
		}

		protected void CheckEmailContent()
		{
			if (base.Request["EmailContent"] != null || base.Request["EmailContentAvectraCRM"] != null)
			{
				Control control = this.FindControl("ContentPane");
				if (control != null)
				{
					EmailContentClass emailContentClass = new EmailContentClass();
					emailContentClass.szFromEmail = this.szFromEmail;
					string str = string.Concat(Config.HttpServerPath.Substring(0, Config.HttpServerPath.IndexOf("//")), "//");
					emailContentClass.szContentUrl = this.RemoveEmailContentParameter(string.Concat(str, Config.HttpServerName, base.Request.RawUrl));
					HtmlGenericControl htmlGenericControl = new HtmlGenericControl("body");
					while (control.Controls.Count > 0)
					{
						Control item = control.Controls[0];
						htmlGenericControl.Controls.Add(item);
					}
					HtmlUtils.ReplaceFormBasedControls(htmlGenericControl);
					string controlHtml = HtmlUtils.GetControlHtml(htmlGenericControl);
					string cssClasses = HtmlUtils.GetCssClasses(controlHtml, HttpContext.Current.Server.MapPath(this.Session["StyleSheet"].ToString()));
					if (!UtilityFunctions.EmptyString(cssClasses))
					{
						cssClasses = HtmlUtils.RemoveCssBehaviors(cssClasses);
						HtmlGenericControl htmlGenericControl1 = new HtmlGenericControl("html");
						HtmlGenericControl htmlGenericControl2 = new HtmlGenericControl("style");
						htmlGenericControl2.Attributes.Add("type", "text/css");
						htmlGenericControl2.InnerText = cssClasses;
						htmlGenericControl1.Controls.Add(htmlGenericControl2);
						htmlGenericControl1.Controls.Add(htmlGenericControl);
						controlHtml = HtmlUtils.GetControlHtml(htmlGenericControl1);
					}
					controlHtml = HtmlUtils.RemoveHtmlAttributes(controlHtml, "id");
					controlHtml = HtmlUtils.RemoveHtmlAttributes(controlHtml, "onclick");
					controlHtml = HtmlUtils.RemoveHtmlAttributes(controlHtml, "onmouseover");
					emailContentClass.szContentHtml = controlHtml;
					this.Session["EmailContentObject"] = emailContentClass;
					if (base.Request["EmailContentAvectraCRM"] == null)
					{
						PageUtils.Redirect("~/EmailContent.aspx");
					}
					else
					{
						PageUtils.Redirect("~/EmailContentAvectra.aspx");
						return;
					}
				}
			}
		}

		protected bool DataExists(string szSQL)
		{
			bool flag = true;
			if (!UtilityFunctions.EmptyString(szSQL))
			{
				flag = false;
				if (this.Session["CustomerKey"] != null && szSQL.IndexOf("{CustomerKey}") >= 0)
				{
					szSQL = szSQL.Replace("{CustomerKey}", string.Concat("'", this.Session["CustomerKey"].ToString(), "'"));
				}
				if (this.oFacadeObject == null)
				{
					if (this.Session["Individual"] == null)
					{
						if (this.Session["Organization"] == null)
						{
							szSQL = DataUtils.ParseValues(this.oDummyDynamicFacade, szSQL, true);
						}
						else
						{
							szSQL = DataUtils.ParseValues((FacadeClass)this.Session["Organization"], szSQL, true);
						}
					}
					else
					{
						szSQL = DataUtils.ParseValues((FacadeClass)this.Session["Individual"], szSQL, true);
					}
				}
				else
				{
					szSQL = DataUtils.ParseValues(this.oFacadeObject, szSQL, true);
				}
				if (szSQL.IndexOf("{CustomerKey}") < 0)
				{
					Config.LastError = null;
					OleDbConnection connection = DataUtils.GetConnection();
					using (connection)
					{
						OleDbDataReader dataReader = DataUtils.GetDataReader(szSQL, connection);
						using (dataReader)
						{
							if (Config.LastError == null && dataReader != null && dataReader.Read())
							{
								flag = true;
							}
						}
					}
				}
			}
			return flag;
		}

		protected HtmlTableCell GetContentCell(HtmlTableCell oCell, eWebPageClass.PaneDataClass oPaneData, bool bMultipleContent)
		{
			HtmlTable htmlTable;
			HtmlTableRow item;
			if (oCell != null && bMultipleContent)
			{
				if (oCell.Controls.Count <= 0 || oCell.Controls[0].ToString().IndexOf(".HtmlTable") < 0)
				{
					htmlTable = new HtmlTable();
					htmlTable.Border = 0;
					htmlTable.CellPadding = 0;
					htmlTable.CellSpacing = 0;
					htmlTable.Width = "100%";
					oCell.Controls.Add(htmlTable);
				}
				else
				{
					htmlTable = (HtmlTable)oCell.Controls[0];
				}
				if (!oPaneData.bIsDefault)
				{
					if (htmlTable.Rows.Count >= oPaneData.nRow)
					{
						item = htmlTable.Rows[oPaneData.nRow - 1];
					}
					else
					{
						item = new HtmlTableRow();
						htmlTable.Rows.Add(item);
					}
				}
				else
				{
					string orientation = this.GetOrientation(oPaneData.szPosition, "");
					if (orientation != "Vertical")
					{
						if (htmlTable.Rows.Count != 0)
						{
							item = htmlTable.Rows[0];
						}
						else
						{
							item = new HtmlTableRow();
							htmlTable.Rows.Add(item);
						}
					}
					else
					{
						item = new HtmlTableRow();
						htmlTable.Rows.Add(item);
					}
				}
				oCell = new HtmlTableCell();
				if (oPaneData.nColSpan > 0)
				{
					oCell.ColSpan = oPaneData.nColSpan;
				}
				if (oPaneData.nRowSpan > 0)
				{
					oCell.RowSpan = oPaneData.nRowSpan;
				}
				if (oPaneData.nHeight > 0)
				{
					oCell.Height = string.Concat(oPaneData.nHeight, oPaneData.szHeightUnit);
				}
				if (oPaneData.nWidth > 0)
				{
					oCell.Width = string.Concat(oPaneData.nWidth, oPaneData.szWidthUnit);
				}
				if (oPaneData.szAlign != "")
				{
					oCell.Align = oPaneData.szAlign;
				}
				if (oPaneData.szVAlign != "")
				{
					oCell.VAlign = oPaneData.szVAlign;
				}
				if (oPaneData.szBGColor != "")
				{
					oCell.BgColor = oPaneData.szBGColor;
				}
				item.Cells.Add(oCell);
			}
			return oCell;
		}

		protected void GetContentLinks()
		{
			bool flag;
			bool flag1;
			bool flag2;
			bool flag3;
			bool flag4;
			if (this.szWebContentKey != "")
			{
				string[] strArrays = new string[11];
				strArrays[0] = "SELECT wbx_key,wbl_key,wbx_position,wbx_section,wbl_wbc_key,  wbl_wbl_key,wbc_page_name,wbc_parameters,wbl_link_text,wbl_description,wbl_image_file, wbl_image_file_hover,wbl_image_file_active,wbl_submenu_type,wbl_requires_login, wbs_requires_login,wbl_disable_upon_login,wbl_external_url,wbl_target_new, wbl_cssclass,wbc_sql_visible,wbs_sql_visible,wbs_ssl_flag,wbl_ssl_flag  FROM md_web_content_x_link ";
				strArrays[1] = DataUtils.NoLock();
				strArrays[2] = " JOIN md_web_link ";
				strArrays[3] = DataUtils.NoLock();
				strArrays[4] = " ON wbl_key=wbx_wbl_key LEFT JOIN md_web_content ";
				strArrays[5] = DataUtils.NoLock();
				strArrays[6] = " ON wbc_key=wbl_wbc_key LEFT JOIN md_web_section ";
				strArrays[7] = DataUtils.NoLock();
				strArrays[8] = " ON wbs_key=wbc_wbs_key WHERE wbx_wbc_key=";
				strArrays[9] = DataUtils.ValuePrep(this.szWebContentKey, "av_key", true);
				strArrays[10] = " AND wbx_delete_flag=0 AND wbl_wbl_key IS NULL AND (wbl_wbc_key IS NULL OR (wbc_publish_date IS NULL OR wbc_publish_date <= GetDate()) AND (wbc_expire_date IS NULL OR wbc_expire_date > GetDate()))  ORDER BY wbx_order";
				string str = string.Concat(strArrays);
				OleDbConnection connection = DataUtils.GetConnection();
				using (connection)
				{
					OleDbDataReader dataReader = DataUtils.GetDataReader(str, connection);
					using (dataReader)
					{
						if (dataReader != null)
						{
							try
							{
								while (dataReader.Read())
								{
									eWebPageClass.LinkClass linkClass = new eWebPageClass.LinkClass();
									linkClass.szUniqueKey = dataReader["wbx_key"].ToString();
									linkClass.szLinkKey = dataReader["wbl_key"].ToString();
									linkClass.szPosition = dataReader["wbx_position"].ToString();
									linkClass.szSection = dataReader["wbx_section"].ToString();
									linkClass.szLinkContentKey = dataReader["wbl_wbc_key"].ToString();
									linkClass.szParentKey = dataReader["wbl_wbl_key"].ToString();
									linkClass.szLinkPageName = dataReader["wbc_page_name"].ToString();
									linkClass.szLinkParameters = dataReader["wbc_parameters"].ToString();
									linkClass.szLinkText = dataReader["wbl_link_text"].ToString();
									linkClass.szDescription = dataReader["wbl_description"].ToString();
									linkClass.szImageFile = dataReader["wbl_image_file"].ToString();
									linkClass.szImageFileHover = dataReader["wbl_image_file_hover"].ToString();
									linkClass.szImageFileActive = dataReader["wbl_image_file_active"].ToString();
									linkClass.szSubMenuType = dataReader["wbl_submenu_type"].ToString();
									eWebPageClass.LinkClass linkClass1 = linkClass;
									if (dataReader["wbl_requires_login"].ToString() == "1")
									{
										flag = true;
									}
									else
									{
										flag = false;
									}
									linkClass1.bRequiresLogin = flag;
									if (!linkClass.bRequiresLogin && dataReader["wbs_requires_login"].ToString() == "1")
									{
										linkClass.bRequiresLogin = true;
									}
									eWebPageClass.LinkClass linkClass2 = linkClass;
									if (dataReader["wbl_disable_upon_login"].ToString() == "1")
									{
										flag1 = true;
									}
									else
									{
										flag1 = false;
									}
									linkClass2.bDisableUponLogin = flag1;
									linkClass.szExternalURL = dataReader["wbl_external_url"].ToString();
									eWebPageClass.LinkClass linkClass3 = linkClass;
									if (dataReader["wbl_target_new"].ToString() == "1")
									{
										flag2 = true;
									}
									else
									{
										flag2 = false;
									}
									linkClass3.bTargetNew = flag2;
									linkClass.szCssClass = dataReader["wbl_cssclass"].ToString();
									linkClass.bIsDefault = false;
									if (linkClass.szLinkContentKey == this.szWebContentKey && linkClass.szExternalURL.IndexOf("EmailContent=") < 0)
									{
										linkClass.bActive = true;
									}
									linkClass.szSQLVisible = dataReader["wbc_sql_visible"].ToString();
									if (linkClass.szSQLVisible == "")
									{
										linkClass.szSQLVisible = dataReader["wbs_sql_visible"].ToString();
									}
									eWebPageClass.LinkClass linkClass4 = linkClass;
									if (dataReader["wbs_ssl_flag"].ToString() == "1")
									{
										flag3 = true;
									}
									else
									{
										flag3 = false;
									}
									linkClass4.bSectionSSL = flag3;
									eWebPageClass.LinkClass linkClass5 = linkClass;
									if (dataReader["wbl_ssl_flag"].ToString() == "1")
									{
										flag4 = true;
									}
									else
									{
										flag4 = false;
									}
									linkClass5.bLinkSSL = flag4;
									this.oLinks.Add(linkClass);
								}
							}
							catch (Exception exception1)
							{
								Exception exception = exception1;
								Config.LastError = new ErrorClass();
								Config.LastError.Message = exception.Message;
								if (exception.InnerException != null && exception.InnerException.Message != null)
								{
									ErrorClass lastError = Config.LastError;
									lastError.Message = string.Concat(lastError.Message, "; ", exception.InnerException.Message);
								}
								Config.LastError.Number = -1;
							}
						}
					}
				}
			}
		}

		private string GetKey(string szUrl, string szKey)
		{
			string str = "";
			if (szUrl.ToLower().Trim().IndexOf(string.Concat(szKey.ToLower().Trim(), "=&")) <= 0)
			{
				int num = szUrl.ToLower().IndexOf(string.Concat("?", szKey.ToLower(), "="));
				if (num < 0)
				{
					num = szUrl.ToLower().IndexOf(string.Concat("&", szKey.ToLower(), "="));
					if (num >= 0)
					{
						str = szUrl.Substring(num + szKey.Length + 2, 36).Replace("'", "''");
					}
				}
				else
				{
					try
					{
						str = szUrl.Substring(num + szKey.Length + 2, 36).Replace("'", "''");
					}
					catch
					{
						str = "";
					}
				}
				return str;
			}
			else
			{
				return "";
			}
		}

		protected string GetLinkCellID(string szPosition, string szSection)
		{
			if (szPosition == "")
			{
				szPosition = "Top";
			}
			if (szSection == "")
			{
				szPosition = "Bottom";
			}
			string[] strArrays = new string[5];
			strArrays[0] = "eWeb";
			strArrays[1] = szPosition;
			strArrays[2] = "Pane";
			strArrays[3] = szSection;
			strArrays[4] = "LinksCell";
			return string.Concat(strArrays);
		}

		private HyperLink GetLinkHyperLink(eWebPageClass.LinkClass oLink)
		{
			string str;
			HyperLink hyperLink = new HyperLink();
			if (oLink.szLinkText != "")
			{
				hyperLink.Text = oLink.szLinkText;
			}
			if (oLink.szDescription != "")
			{
				hyperLink.ToolTip = oLink.szDescription;
			}
			if (oLink.szImageFile != "" && oLink.szLinkText == "")
			{
				if (!oLink.bActive || !(oLink.szImageFileActive != ""))
				{
					hyperLink.ImageUrl = oLink.szImageFile;
				}
				else
				{
					hyperLink.ImageUrl = oLink.szImageFileActive;
				}
			}
			if (oLink.szExternalURL.ToLower().IndexOf("emailcontent") < 0)
			{
				if (oLink.szLinkPageName != "")
				{
					string[] strArrays = new string[7];
					strArrays[0] = "~/";
					strArrays[1] = oLink.szLinkPageName;
					string[] strArrays1 = strArrays;
					int num = 2;
					if (oLink.szLinkPageName.IndexOf("?") == -1)
					{
						str = "?";
					}
					else
					{
						str = "&";
					}
					strArrays1[num] = str;
					strArrays[3] = "Site=";
					strArrays[4] = this.szWebSiteCode;
					strArrays[5] = "&WebKey=";
					strArrays[6] = oLink.szLinkContentKey;
					string str1 = string.Concat(strArrays);
					if (oLink.szLinkParameters != "")
					{
						str1 = string.Concat(str1, "&", DataUtils.ParseValues(this.oFacadeObject, oLink.szLinkParameters, false));
					}
					if (oLink.szExternalURL != "")
					{
						str1 = string.Concat(str1, "&", DataUtils.ParseValues(this.oFacadeObject, oLink.szExternalURL, false));
					}
					hyperLink.NavigateUrl = str1;
				}
				else
				{
					hyperLink.NavigateUrl = oLink.szExternalURL;
				}
			}
			else
			{
				hyperLink.NavigateUrl = base.Request.RawUrl;
				if (hyperLink.NavigateUrl.IndexOf("?") < 0)
				{
					HyperLink hyperLink1 = hyperLink;
					hyperLink1.NavigateUrl = string.Concat(hyperLink1.NavigateUrl, "?", DataUtils.ParseValues(this.oFacadeObject, oLink.szExternalURL, false));
				}
				else
				{
					HyperLink hyperLink2 = hyperLink;
					hyperLink2.NavigateUrl = string.Concat(hyperLink2.NavigateUrl, "&", DataUtils.ParseValues(this.oFacadeObject, oLink.szExternalURL, false));
				}
			}
			if (UtilityFunctions.EmptyString(oLink.szExternalURL) && hyperLink.NavigateUrl != "" && hyperLink.NavigateUrl.ToLower().IndexOf("javascript") < 0)
			{
				hyperLink.NavigateUrl = Functions.AppendSiteCodeToURL(hyperLink.NavigateUrl);
			}
			if (oLink.bTargetNew)
			{
				hyperLink.Target = "_new";
			}
			if (oLink.szCssClass != "")
			{
				if (oLink.szCssClass.IndexOf(";") >= 0)
				{
					hyperLink.CssClass = oLink.szCssClass.Substring(oLink.szCssClass.IndexOf(";") + 1);
				}
				else
				{
					hyperLink.CssClass = oLink.szCssClass;
				}
			}
			else
			{
				hyperLink.CssClass = "TopUIHyperLink";
			}
			if ((oLink.bSectionSSL || oLink.bLinkSSL) && !this.szHttp.ToLower().StartsWith("https:") && hyperLink.NavigateUrl.StartsWith("~"))
			{
				string str2 = Config.HttpServerPath.Replace("http:", "https:");
				hyperLink.NavigateUrl = hyperLink.NavigateUrl.Replace("~", str2);
			}
			return hyperLink;
		}

		private string GetListDetailHtml(string szHtml, ref string szTopHtml, ref string szBottomHtml)
		{
			string str = "";
			string str1 = "{BeginListDetail}";
			string str2 = "{EndListDetail}";
			int num = szHtml.IndexOf(str1);
			int num1 = szHtml.IndexOf(str2);
			if (num >= 0 && num1 >= 0)
			{
				szTopHtml = szHtml.Substring(0, num);
				szBottomHtml = szHtml.Substring(num1 + str2.Length);
				str = szHtml.Substring(num + str1.Length, num1 - num - str1.Length);
			}
			return str;
		}

		private string GetListPagerHtml(DataTable oDSTable, int nMaxPerPage, string szCurrentPageNumber, string szLinkID)
		{
			string str = "";
			if (oDSTable != null && oDSTable.Rows.Count > nMaxPerPage)
			{
				int count = oDSTable.Rows.Count / nMaxPerPage;
				if (oDSTable.Rows.Count % nMaxPerPage != 0)
				{
					count++;
				}
				for (int i = 0; i < count; i++)
				{
					int num = i + 1;
					if (i.ToString() != szCurrentPageNumber)
					{
						string str1 = str;
						string[] strArrays = new string[8];
						strArrays[0] = str1;
						strArrays[1] = "<a href=\"#\" class=\"DataFormChildDataGridPagerLink\" onclick=\"__doPostBack('";
						strArrays[2] = szLinkID;
						strArrays[3] = "','";
						strArrays[4] = i.ToString();
						strArrays[5] = "');\">";
						strArrays[6] = num.ToString();
						strArrays[7] = "</a>&nbsp;";
						str = string.Concat(strArrays);
					}
					else
					{
						str = string.Concat(str, "<span class=\"DataFormChildDataGridPagerLink\"><b>", num.ToString(), "</b></span>&nbsp;");
					}
				}
			}
			return str;
		}

		private string GetListProperty(ref string szHtml, string szBeginTag, string szEndTag)
		{
			string str = "";
			int num = szHtml.IndexOf(szBeginTag);
			int num1 = szHtml.IndexOf(szEndTag);
			if (num >= 0 && num1 >= 0)
			{
				str = szHtml.Substring(num + szBeginTag.Length, num1 - num - szBeginTag.Length);
				szHtml = szHtml.Remove(num, num1 + szEndTag.Length - num);
			}
			return HttpUtility.HtmlDecode(str);
		}

		private string GetOrientation(string szPosition, string szSection)
		{
			szPosition = szPosition.ToLower();
			szSection = szSection.ToLower();
			string str = "Horizontal";
			if (szPosition == "left" || szPosition == "right" || (szPosition == "top" || szPosition == "bottom" || szPosition == "content") && (szSection == "left" || szSection == "right"))
			{
				str = "Vertical";
			}
			return str;
		}

		protected string GetPaneCellID(string szPosition, string szSection)
		{
			string str = "";
			if (szPosition != "")
			{
				if (szSection != "")
				{
					if (szSection != "Content")
					{
						if (szSection != "")
						{
							string[] strArrays = new string[5];
							strArrays[0] = "eWeb";
							strArrays[1] = szPosition;
							strArrays[2] = "Pane";
							strArrays[3] = szSection;
							strArrays[4] = "LinksCell";
							str = string.Concat(strArrays);
						}
					}
					else
					{
						str = string.Concat(szPosition, "Pane");
					}
				}
				else
				{
					str = string.Concat("eWeb", szPosition, "PaneTableCell");
				}
			}
			return str;
		}

		protected void GetPaneCSSClasses()
		{
			string str;
			if (this.szWebContentKey != "")
			{
				string[] strArrays = new string[7];
				strArrays[0] = "SELECT wss_position,wss_section,wss_cssclass,wss_height,wss_height_unit, wss_width,wss_width_unit,wss_align,wss_valign,wss_bg_color  FROM md_web_cssclass ";
				strArrays[1] = DataUtils.NoLock();
				strArrays[2] = " WHERE wss_web_key=";
				strArrays[3] = DataUtils.ValuePrep(this.szWebSiteKey, "av_key", true);
				strArrays[4] = " AND (wss_wbc_key IS NULL OR wss_wbc_key=";
				strArrays[5] = DataUtils.ValuePrep(this.szWebContentKey, "av_key", true);
				strArrays[6] = ") AND wss_delete_flag=0 ORDER BY wss_wbc_key,wss_section";
				str = string.Concat(strArrays);
			}
			else
			{
				string[] strArrays1 = new string[5];
				strArrays1[0] = "SELECT wss_position,wss_section,wss_cssclass,wss_height,wss_height_unit, wss_width,wss_width_unit,wss_align,wss_valign,wss_bg_color  FROM md_web_cssclass ";
				strArrays1[1] = DataUtils.NoLock();
				strArrays1[2] = " WHERE wss_web_key=";
				strArrays1[3] = DataUtils.ValuePrep(this.szWebSiteKey, "av_key", true);
				strArrays1[4] = " AND wss_wbc_key IS NULL AND wss_delete_flag=0 ORDER BY wss_section";
				str = string.Concat(strArrays1);
			}
			OleDbConnection connection = DataUtils.GetConnection();
			using (connection)
			{
				OleDbDataReader dataReader = DataUtils.GetDataReader(str, connection);
				using (dataReader)
				{
					if (dataReader != null)
					{
						while (dataReader.Read())
						{
							string paneCellID = this.GetPaneCellID(dataReader["wss_position"].ToString(), dataReader["wss_section"].ToString());
							if (paneCellID == "")
							{
								paneCellID = "FrameWorkTable";
							}
							eWebPageClass.PaneCssClass paneCssClass = new eWebPageClass.PaneCssClass();
							paneCssClass.szPaneID = paneCellID;
							paneCssClass.szCssClass = dataReader["wss_cssclass"].ToString();
							if (dataReader["wss_height"].ToString() != "")
							{
								paneCssClass.nHeight = Convert.ToInt32(dataReader["wss_height"].ToString());
							}
							paneCssClass.szHeightUnit = dataReader["wss_height_unit"].ToString();
							if (dataReader["wss_width"].ToString() != "")
							{
								paneCssClass.nWidth = Convert.ToInt32(dataReader["wss_width"].ToString());
							}
							paneCssClass.szWidthUnit = dataReader["wss_width_unit"].ToString();
							paneCssClass.szAlign = dataReader["wss_align"].ToString();
							paneCssClass.szVAlign = dataReader["wss_valign"].ToString();
							paneCssClass.szBGColor = dataReader["wss_bg_color"].ToString();
							if (this.hashCSSClasses[paneCellID] != null)
							{
								this.hashCSSClasses[paneCellID] = paneCssClass;
							}
							else
							{
								this.hashCSSClasses.Add(paneCellID, paneCssClass);
							}
						}
					}
				}
			}
		}

		protected void GetPaneData(OleDbDataReader oDR, string szPrefix, eWebPageClass.PaneDataClass oPaneData)
		{
			oPaneData.szKey = oDR[string.Concat(szPrefix, "_key")].ToString();
			oPaneData.szContentType = oDR[string.Concat(szPrefix, "_content_type")].ToString().ToLower();
			oPaneData.szFormKey = oDR[string.Concat(szPrefix, "_dyn_key")].ToString();
			oPaneData.szWizardKey = oDR[string.Concat(szPrefix, "_wiz_key")].ToString();
			oPaneData.szCurrentKey = oDR[string.Concat(szPrefix, "_control_file")].ToString();
			oPaneData.szControlPath = oDR[string.Concat(szPrefix, "_control_file")].ToString();
			oPaneData.szContentHtml = oDR[string.Concat(szPrefix, "_html")].ToString();
			oPaneData.szContentHtml = oPaneData.szContentHtml.Replace("~", Config.ApplicationPath);
			oPaneData.szContentHtml = oPaneData.szContentHtml.Replace("{SiteCode}", this.szWebSiteCode);
			oPaneData.szContentHtml = oPaneData.szContentHtml.Replace("[|SiteCode|]", this.szWebSiteCode);
			if (szPrefix == "wbp" || szPrefix == "wsd")
			{
				if (oDR[string.Concat(szPrefix, "_height")].ToString() != "")
				{
					oPaneData.nHeight = Convert.ToInt32(oDR[string.Concat(szPrefix, "_height")].ToString());
				}
				oPaneData.szHeightUnit = oDR[string.Concat(szPrefix, "_height_unit")].ToString();
				if (oDR[string.Concat(szPrefix, "_width")].ToString() != "")
				{
					oPaneData.nWidth = Convert.ToInt32(oDR[string.Concat(szPrefix, "_width")].ToString());
				}
				oPaneData.szWidthUnit = oDR[string.Concat(szPrefix, "_width_unit")].ToString();
				oPaneData.szAlign = oDR[string.Concat(szPrefix, "_align")].ToString();
				oPaneData.szVAlign = oDR[string.Concat(szPrefix, "_valign")].ToString();
				oPaneData.szBGColor = oDR[string.Concat(szPrefix, "_bg_color")].ToString();
				oPaneData.szParentDetailKey = oDR[string.Concat(szPrefix, "_wbd_key")].ToString();
			}
		}

		private void GetPaneDataFromParent(eWebPageClass.PaneDataClass oPaneData)
		{
			string str = oPaneData.szParentDetailKey;
			if (!UtilityFunctions.EmptyString(str))
			{
				string str1 = string.Concat("SELECT *  FROM md_web_content_detail ", DataUtils.NoLock(), " WHERE wbd_key=", DataUtils.ValuePrep(str, "av_key", true));
				OleDbConnection connection = DataUtils.GetConnection();
				using (connection)
				{
					OleDbDataReader dataReader = DataUtils.GetDataReader(str1, connection);
					using (dataReader)
					{
						if (dataReader != null && dataReader.Read())
						{
							this.GetPaneData(dataReader, "wbd", oPaneData);
						}
					}
				}
			}
		}

		protected string GetPaneID(string szPosition)
		{
			string str = "";
			if (szPosition != "left")
			{
				if (szPosition != "top")
				{
					if (szPosition != "right")
					{
						if (szPosition != "content")
						{
							if (szPosition == "bottom")
							{
								str = "BottomPane";
							}
						}
						else
						{
							str = "ContentPane";
						}
					}
					else
					{
						str = "RightPane";
					}
				}
				else
				{
					str = "TopPane";
				}
			}
			else
			{
				str = "LeftPane";
			}
			return str;
		}

		private string GetPopupID()
		{
			eWebPageClass _eWebPageClass = this;
			_eWebPageClass.nPopupIDCounter = _eWebPageClass.nPopupIDCounter + 1;
			return string.Concat("Pop", this.nPopupIDCounter.ToString());
		}

		protected void GetSectionDefaultLinks()
		{
			bool flag;
			bool flag1;
			bool flag2;
			bool flag3;
			bool flag4;
			if (this.szSectionKey != "")
			{
				string[] strArrays = new string[11];
				strArrays[0] = "SELECT wsl_key,wbl_key,wsl_position,wsl_section,wbl_wbc_key,  wbl_wbl_key,wbc_page_name,wbc_parameters,wbl_link_text,wbl_description,wbl_image_file,  wbl_image_file_hover,wbl_image_file_active,wbl_submenu_type,wbl_requires_login, wbs_requires_login,wbl_disable_upon_login,wbl_external_url,wbl_target_new, wbl_cssclass,wbc_sql_visible,wbs_sql_visible,wbs_ssl_flag,wbl_ssl_flag  FROM md_web_section_x_link ";
				strArrays[1] = DataUtils.NoLock();
				strArrays[2] = " JOIN md_web_link ";
				strArrays[3] = DataUtils.NoLock();
				strArrays[4] = " ON wbl_key=wsl_wbl_key LEFT JOIN md_web_content ";
				strArrays[5] = DataUtils.NoLock();
				strArrays[6] = " ON wbc_key=wbl_wbc_key LEFT JOIN md_web_section ";
				strArrays[7] = DataUtils.NoLock();
				strArrays[8] = " ON wbs_key=wbc_wbs_key WHERE wsl_wbs_key=";
				strArrays[9] = DataUtils.ValuePrep(this.szSectionKey, "av_key", true);
				strArrays[10] = " AND wsl_delete_flag=0 AND wbl_wbl_key IS NULL AND (wbl_wbc_key IS NULL OR (wbc_publish_date IS NULL OR wbc_publish_date <= GetDate()) AND (wbc_expire_date IS NULL OR wbc_expire_date > GetDate()))  ORDER BY wsl_order";
				string str = string.Concat(strArrays);
				OleDbConnection connection = DataUtils.GetConnection();
				using (connection)
				{
					OleDbDataReader dataReader = DataUtils.GetDataReader(str, connection);
					using (dataReader)
					{
						if (dataReader != null)
						{
							try
							{
								while (dataReader.Read())
								{
									eWebPageClass.LinkClass linkClass = new eWebPageClass.LinkClass();
									linkClass.szUniqueKey = dataReader["wsl_key"].ToString();
									linkClass.szLinkKey = dataReader["wbl_key"].ToString();
									linkClass.szPosition = dataReader["wsl_position"].ToString();
									linkClass.szSection = dataReader["wsl_section"].ToString();
									linkClass.szLinkContentKey = dataReader["wbl_wbc_key"].ToString();
									linkClass.szParentKey = dataReader["wbl_wbl_key"].ToString();
									linkClass.szLinkPageName = dataReader["wbc_page_name"].ToString();
									linkClass.szLinkParameters = dataReader["wbc_parameters"].ToString();
									linkClass.szLinkText = dataReader["wbl_link_text"].ToString();
									linkClass.szDescription = dataReader["wbl_description"].ToString();
									linkClass.szImageFile = dataReader["wbl_image_file"].ToString();
									linkClass.szImageFileHover = dataReader["wbl_image_file_hover"].ToString();
									linkClass.szImageFileActive = dataReader["wbl_image_file_active"].ToString();
									linkClass.szSubMenuType = dataReader["wbl_submenu_type"].ToString();
									eWebPageClass.LinkClass linkClass1 = linkClass;
									if (dataReader["wbl_requires_login"].ToString() == "1")
									{
										flag = true;
									}
									else
									{
										flag = false;
									}
									linkClass1.bRequiresLogin = flag;
									if (!linkClass.bRequiresLogin && dataReader["wbs_requires_login"].ToString() == "1")
									{
										linkClass.bRequiresLogin = true;
									}
									eWebPageClass.LinkClass linkClass2 = linkClass;
									if (dataReader["wbl_disable_upon_login"].ToString() == "1")
									{
										flag1 = true;
									}
									else
									{
										flag1 = false;
									}
									linkClass2.bDisableUponLogin = flag1;
									linkClass.szExternalURL = dataReader["wbl_external_url"].ToString();
									eWebPageClass.LinkClass linkClass3 = linkClass;
									if (dataReader["wbl_target_new"].ToString() == "1")
									{
										flag2 = true;
									}
									else
									{
										flag2 = false;
									}
									linkClass3.bTargetNew = flag2;
									linkClass.szCssClass = dataReader["wbl_cssclass"].ToString();
									linkClass.bIsSectionDefault = true;
									if (linkClass.szLinkContentKey == this.szWebContentKey && linkClass.szExternalURL.IndexOf("EmailContent=") < 0)
									{
										linkClass.bActive = true;
									}
									linkClass.szSQLVisible = dataReader["wbc_sql_visible"].ToString();
									if (linkClass.szSQLVisible == "")
									{
										linkClass.szSQLVisible = dataReader["wbs_sql_visible"].ToString();
									}
									eWebPageClass.LinkClass linkClass4 = linkClass;
									if (dataReader["wbs_ssl_flag"].ToString() == "1")
									{
										flag3 = true;
									}
									else
									{
										flag3 = false;
									}
									linkClass4.bSectionSSL = flag3;
									eWebPageClass.LinkClass linkClass5 = linkClass;
									if (dataReader["wbl_ssl_flag"].ToString() == "1")
									{
										flag4 = true;
									}
									else
									{
										flag4 = false;
									}
									linkClass5.bLinkSSL = flag4;
									this.oLinks.Add(linkClass);
								}
							}
							catch (Exception exception1)
							{
								Exception exception = exception1;
								Config.LastError = new ErrorClass();
								Config.LastError.Message = exception.Message;
								if (exception.InnerException != null && exception.InnerException.Message != null)
								{
									ErrorClass lastError = Config.LastError;
									lastError.Message = string.Concat(lastError.Message, "; ", exception.InnerException.Message);
								}
								Config.LastError.Number = -1;
							}
						}
					}
				}
			}
		}

		protected void GetSectionPaneDefaults()
		{
			ArrayList item;
			if (this.szSectionKey != "")
			{
				string[] strArrays = new string[5];
				strArrays[0] = "SELECT *  FROM md_web_section_default ";
				strArrays[1] = DataUtils.NoLock();
				strArrays[2] = " WHERE wsd_wbs_key=";
				strArrays[3] = DataUtils.ValuePrep(this.szSectionKey, "av_key", true);
				strArrays[4] = " ORDER BY wsd_position,wsd_order";
				string str = string.Concat(strArrays);
				OleDbConnection connection = DataUtils.GetConnection();
				using (connection)
				{
					OleDbDataReader dataReader = DataUtils.GetDataReader(str, connection);
					using (dataReader)
					{
						if (dataReader != null)
						{
							while (dataReader.Read())
							{
								if (this.ModalForm || !(dataReader["wsd_content_type"].ToString() != ""))
								{
									continue;
								}
								eWebPageClass.PaneDataClass paneDataClass = new eWebPageClass.PaneDataClass();
								paneDataClass.bIsDefault = true;
								this.GetPaneData(dataReader, "wsd", paneDataClass);
								string lower = dataReader["wsd_position"].ToString().ToLower();
								paneDataClass.szPosition = lower;
								if (this.hashPanes[lower] != null)
								{
									item = (ArrayList)this.hashPanes[lower];
								}
								else
								{
									item = new ArrayList();
									this.hashPanes.Add(lower, item);
								}
								item.Add(paneDataClass);
							}
						}
					}
				}
			}
		}

		protected void GetSiteDefaultLinks()
		{
			bool flag;
			bool flag1;
			bool flag2;
			bool flag3;
			bool flag4;
			string[] strArrays = new string[11];
			strArrays[0] = "SELECT wbk_key,wbl_key,wbk_position,wbk_section,wbl_wbc_key, wbl_wbl_key,wbc_page_name,wbc_parameters,wbl_link_text,wbl_description, wbl_image_file,wbl_image_file_hover,wbl_image_file_active,wbl_submenu_type, wbl_requires_login,wbs_requires_login,wbl_disable_upon_login,wbl_external_url, wbl_target_new,wbl_cssclass,wbc_sql_visible,wbs_sql_visible,wbs_ssl_flag,wbl_ssl_flag  FROM md_web_x_link ";
			strArrays[1] = DataUtils.NoLock();
			strArrays[2] = " JOIN md_web_link ";
			strArrays[3] = DataUtils.NoLock();
			strArrays[4] = " ON wbl_key=wbk_wbl_key LEFT JOIN md_web_content ";
			strArrays[5] = DataUtils.NoLock();
			strArrays[6] = " ON wbc_key=wbl_wbc_key LEFT JOIN md_web_section ";
			strArrays[7] = DataUtils.NoLock();
			strArrays[8] = " ON wbs_key=wbc_wbs_key WHERE wbk_web_key=";
			strArrays[9] = DataUtils.ValuePrep(this.szWebSiteKey, "av_key", true);
			strArrays[10] = " AND wbk_delete_flag=0 AND  wbl_wbl_key IS NULL AND (wbl_wbc_key IS NULL OR (wbc_publish_date IS NULL OR wbc_publish_date <= GetDate()) AND (wbc_expire_date IS NULL OR wbc_expire_date > GetDate()))  ORDER BY wbk_order";
			string str = string.Concat(strArrays);
			OleDbConnection connection = DataUtils.GetConnection();
			using (connection)
			{
				OleDbDataReader dataReader = DataUtils.GetDataReader(str, connection);
				using (dataReader)
				{
					if (dataReader != null)
					{
						try
						{
							while (dataReader.Read())
							{
								eWebPageClass.LinkClass linkClass = new eWebPageClass.LinkClass();
								linkClass.szUniqueKey = dataReader["wbk_key"].ToString();
								linkClass.szLinkKey = dataReader["wbl_key"].ToString();
								linkClass.szPosition = dataReader["wbk_position"].ToString();
								linkClass.szSection = dataReader["wbk_section"].ToString();
								linkClass.szLinkContentKey = dataReader["wbl_wbc_key"].ToString();
								linkClass.szParentKey = dataReader["wbl_wbl_key"].ToString();
								linkClass.szLinkPageName = dataReader["wbc_page_name"].ToString();
								linkClass.szLinkParameters = dataReader["wbc_parameters"].ToString();
								linkClass.szLinkText = dataReader["wbl_link_text"].ToString();
								linkClass.szDescription = dataReader["wbl_description"].ToString();
								linkClass.szImageFile = dataReader["wbl_image_file"].ToString();
								linkClass.szImageFileHover = dataReader["wbl_image_file_hover"].ToString();
								linkClass.szImageFileActive = dataReader["wbl_image_file_active"].ToString();
								linkClass.szSubMenuType = dataReader["wbl_submenu_type"].ToString();
								eWebPageClass.LinkClass linkClass1 = linkClass;
								if (dataReader["wbl_requires_login"].ToString() == "1")
								{
									flag = true;
								}
								else
								{
									flag = false;
								}
								linkClass1.bRequiresLogin = flag;
								if (!linkClass.bRequiresLogin && dataReader["wbs_requires_login"].ToString() == "1")
								{
									linkClass.bRequiresLogin = true;
								}
								eWebPageClass.LinkClass linkClass2 = linkClass;
								if (dataReader["wbl_disable_upon_login"].ToString() == "1")
								{
									flag1 = true;
								}
								else
								{
									flag1 = false;
								}
								linkClass2.bDisableUponLogin = flag1;
								linkClass.szExternalURL = dataReader["wbl_external_url"].ToString();
								eWebPageClass.LinkClass linkClass3 = linkClass;
								if (dataReader["wbl_target_new"].ToString() == "1")
								{
									flag2 = true;
								}
								else
								{
									flag2 = false;
								}
								linkClass3.bTargetNew = flag2;
								linkClass.szCssClass = dataReader["wbl_cssclass"].ToString();
								linkClass.bIsDefault = true;
								if (linkClass.szLinkContentKey == this.szWebContentKey && linkClass.szExternalURL.IndexOf("EmailContent=") < 0)
								{
									linkClass.bActive = true;
								}
								linkClass.szSQLVisible = dataReader["wbc_sql_visible"].ToString();
								if (linkClass.szSQLVisible == "")
								{
									linkClass.szSQLVisible = dataReader["wbs_sql_visible"].ToString();
								}
								eWebPageClass.LinkClass linkClass4 = linkClass;
								if (dataReader["wbs_ssl_flag"].ToString() == "1")
								{
									flag3 = true;
								}
								else
								{
									flag3 = false;
								}
								linkClass4.bSectionSSL = flag3;
								eWebPageClass.LinkClass linkClass5 = linkClass;
								if (dataReader["wbl_ssl_flag"].ToString() == "1")
								{
									flag4 = true;
								}
								else
								{
									flag4 = false;
								}
								linkClass5.bLinkSSL = flag4;
								this.oLinks.Add(linkClass);
							}
						}
						catch (Exception exception1)
						{
							Exception exception = exception1;
							Config.LastError = new ErrorClass();
							Config.LastError.Message = exception.Message;
							if (exception.InnerException != null && exception.InnerException.Message != null)
							{
								ErrorClass lastError = Config.LastError;
								lastError.Message = string.Concat(lastError.Message, "; ", exception.InnerException.Message);
							}
							Config.LastError.Number = -1;
						}
					}
				}
			}
		}

		protected void GetSitePaneDefaults()
		{
			ArrayList item;
			string[] strArrays = new string[7];
			strArrays[0] = "SELECT *  FROM md_web ";
			strArrays[1] = DataUtils.NoLock();
			strArrays[2] = " LEFT JOIN md_web_default_pane ";
			strArrays[3] = DataUtils.NoLock();
			strArrays[4] = " ON wbp_web_key=web_key AND wbp_delete_flag=0 WHERE web_code=";
			strArrays[5] = DataUtils.ValuePrep(this.szWebSiteCode, "nvarchar", true);
			strArrays[6] = " ORDER BY wbp_position,wbp_order";
			string str = string.Concat(strArrays);
			OleDbConnection connection = DataUtils.GetConnection();
			using (connection)
			{
				OleDbDataReader dataReader = DataUtils.GetDataReader(str, connection);
				using (dataReader)
				{
					if (dataReader != null)
					{
						bool flag = true;
						while (dataReader.Read())
						{
							if (flag)
							{
								flag = false;
								if (this.szExpandImage != dataReader["web_exp_image"].ToString())
								{
									this.szExpandImage = dataReader["web_exp_image"].ToString();
								}
								if (this.szCollapseImage != dataReader["web_col_image"].ToString())
								{
									this.szCollapseImage = dataReader["web_col_image"].ToString();
								}
								if (this.szWebSiteKey == "")
								{
									this.szWebSiteKey = dataReader["web_key"].ToString();
								}
								if (dataReader["web_cssfile"].ToString() != "" && this.Session["StyleSheet"].ToString() != dataReader["web_cssfile"].ToString())
								{
									this.Session["StyleSheet"] = dataReader["web_cssfile"].ToString().Replace("~", Config.ApplicationPath);
								}
								if (dataReader["web_width"].ToString() != "")
								{
									this.szWebWidth = dataReader["web_width"].ToString();
								}
								this.szWebWidthUnit = dataReader["web_width_unit"].ToString();
								if (dataReader["web_height"].ToString() != "")
								{
									this.szWebHeight = dataReader["web_height"].ToString();
								}
								this.szWebHeightUnit = dataReader["web_height_unit"].ToString();
								if (dataReader["web_align"].ToString() != "")
								{
									this.szWebAlign = dataReader["web_align"].ToString();
								}
								if (dataReader["web_bg_color"].ToString() != "")
								{
									this.szWebBgColor = dataReader["web_bg_color"].ToString();
								}
								if (dataReader["web_enable_atlas_flag"].ToString() == "1" && Config.GetSystemOptionBoolean("EnableAtlas"))
								{
									this.bSiteEnableAtlas = true;
								}
								this.szBodyAttributesSite = dataReader["web_body_attributes"].ToString();
							}
							if (this.ModalForm || !(dataReader["wbp_content_type"].ToString() != ""))
							{
								continue;
							}
							eWebPageClass.PaneDataClass paneDataClass = new eWebPageClass.PaneDataClass();
							paneDataClass.bIsDefault = true;
							this.GetPaneData(dataReader, "wbp", paneDataClass);
							string lower = dataReader["wbp_position"].ToString().ToLower();
							paneDataClass.szPosition = lower;
							if (this.hashPanes[lower] != null)
							{
								item = (ArrayList)this.hashPanes[lower];
							}
							else
							{
								item = new ArrayList();
								this.hashPanes.Add(lower, item);
							}
							item.Add(paneDataClass);
						}
					}
				}
			}
		}

		protected string GetWebContentKeyByCode()
		{
			string str = "";
			if (this.szWebSiteKey != "" && this.szWebContentCode != "")
			{
				string[] strArrays = new string[7];
				strArrays[0] = "SELECT wbc_key FROM md_web_content ";
				strArrays[1] = DataUtils.NoLock();
				strArrays[2] = " WHERE wbc_web_key='";
				strArrays[3] = this.szWebSiteKey.Replace("'", "''");
				strArrays[4] = "' AND wbc_code='";
				strArrays[5] = this.szWebContentCode.Replace("'", "''");
				strArrays[6] = "'";
				string str1 = string.Concat(strArrays);
				OleDbConnection connection = DataUtils.GetConnection();
				RecordSet recordSet = new RecordSet();
				OleDbDataReader dataReader = recordSet.GetDataReader(str1, connection);
				if (dataReader != null)
				{
					if (dataReader.Read())
					{
						str = dataReader["wbc_key"].ToString();
					}
					dataReader.Close();
				}
				connection.Close();
			}
			return str;
		}

		protected void InitializePage()
		{
			ev_event item;
			DynamicFacade str;
			base.CheckNewSession();
			if (base.Request["shoppingcartredirect"] != null)
			{
				this.Session["shoppingcartredirect"] = base.Request["shoppingcartredirect"].ToString();
			}
			this.InitializeSite();
			if (base.Request["logoff"] == null)
			{
				if (this.Session["CustomerKey"] == null && ConfigurationManager.AppSettings["CustomLoginMethod"] != null)
				{
					string str1 = ConfigurationManager.AppSettings["CustomLoginMethod"].ToString();
					if (!UtilityFunctions.EmptyString(str1))
					{
						this.LoginCustom(str1);
					}
				}
				if (this.Session["CustomerKey"] != null && (base.Request.QueryString[string.Concat(this.Session["eWebSiteID"].ToString(), "login")] != null || base.Request.QueryString[string.Concat(this.Session["eWebSiteID"].ToString(), "cstid")] != null || base.Request.Form[string.Concat(this.Session["eWebSiteID"].ToString(), "login")] != null || base.Request.Form[string.Concat(this.Session["eWebSiteID"].ToString(), "cstid")] != null))
				{
					this.Session.Abandon();
					string rawUrl = base.Request.RawUrl;
					for (int i = 0; i < base.Request.Form.Count; i++)
					{
						rawUrl = UtilityFunctions.AppendQueryStringParameters(rawUrl, string.Concat(base.Request.Form.GetKey(i), "=", base.Server.UrlEncode(base.Server.UrlDecode(base.Request.Form.Get(i)))));
					}
					base.Response.Redirect(rawUrl);
				}
				bool flag = false;
				if (!flag && this.Session["CustomerKey"] == null)
				{
					flag = this.LoginByRequest("POST");
				}
				if (!flag && this.Session["CustomerKey"] == null)
				{
					flag = this.LoginByRequest("GET");
				}
				if (!flag)
				{
					if (this.Session["CustomerKey"] == null)
					{
						this.LoginByCookie();
					}
					if (this.Session["CustomerKey"] != null && (this.Session["ForcePasswordChange"] != null && (bool)this.Session["ForcePasswordChange"] || base.Request.QueryString["WebCode"] != "ChangePassword" && this.Session["Individual"] != null && ((FacadeClass)this.Session["Individual"]).GetValue("cst_web_force_password_change") == "1"))
					{
						string str2 = "~/DynamicPage.aspx?WebCode=ChangePassword";
						if (this.Session["ForcePasswordChange"] == null || !(bool)this.Session["ForcePasswordChange"])
						{
							str2 = string.Concat(str2, "&ChangeFirst=1");
						}
						this.Session["ForcePasswordChange"] = false;
						this.Session["IntendedURL"] = base.Request.Url.ToString();
						PageUtils.Redirect(str2, true);
						return;
					}
				}
			}
			if (Config.GetSystemOption("KeepSessionAliveUrl") != null && Config.GetSystemOption("KeepSessionAliveUrl").ToString() != "")
			{
				this.KeepAlive(Config.GetSystemOption("KeepSessionAliveUrl").ToString());
			}
			if (base.bIsFirstTime)
			{
				if (this.Session["CustomerKey"] == null && base.Request["cst_key"] != null && this.Session["Individual"] == null)
				{
					CO_Individual cOIndividual = new CO_Individual("Individual", this.Session);
					cOIndividual.ObjectDescription = "Individual";
					cOIndividual.CurrentKey = base.Request["cst_key"].ToString();
					cOIndividual.SelectByKey();
					this.Session["Individual"] = cOIndividual;
				}
				if (base.Request["evt_key"] != null)
				{
					if (this.Session["Events"] != null)
					{
						item = (ev_event)this.Session["Events"];
					}
					else
					{
						item = (ev_event)DataUtils.InstantiateFacadeObject(this.Session, "Events", "Events");
					}
					item.CurrentKey = base.Request["evt_key"].ToString();
					item.SelectByKey();
					this.Session["Events"] = item;
				}
				if (base.Request["jbb_key"] != null)
				{
					if (this.Session["JobBank"] != null)
					{
						str = (DynamicFacade)this.Session["JobBank"];
					}
					else
					{
						str = (DynamicFacade)DataUtils.InstantiateFacadeObject(this.Session, "JobBank", "JobBank");
					}
					str.CurrentKey = base.Request["jbb_key"].ToString();
					str.SelectByKey();
					this.Session["JobBank"] = str;
				}
				if (base.Request["chp_cst_key"] != null)
				{
					CO_Chapter cOChapter = new CO_Chapter("Chapter", this.Session);
					cOChapter.ObjectDescription = "Chapter";
					cOChapter.CurrentKey = base.Request["chp_cst_key"].ToString();
					cOChapter.SelectByKey();
					this.Session["Chapter"] = cOChapter;
				}
				if (base.Request["reg_key"] != null)
				{
					Registration registration = new Registration("EventsRegistrant", this.Session);
					registration.ObjectDescription = "EventsRegistrant";
					registration.CurrentKey = base.Request["reg_key"].ToString();
					registration.SelectByKey();
					this.Session["EventsRegistrant"] = registration;
				}
				if (base.Request["org_cst_key"] != null)
				{
					CO_Organization cOOrganization = new CO_Organization("Organization", this.Session);
					cOOrganization.ObjectDescription = "Organization";
					cOOrganization.CurrentKey = base.Request["org_cst_key"].ToString();
					cOOrganization.SelectByKey();
					this.Session["Organization"] = cOOrganization;
				}
			}
			this.GetSitePaneDefaults();
			Functions.BuildFrameWorkTable(this.Page, this.nTableBorder);
			if (!this.ModalForm)
			{
				HtmlTable htmlTable = (HtmlTable)this.Page.FindControl("PageFrameWorkTable");
				if (htmlTable != null)
				{
					htmlTable.Width = string.Concat(this.szWebWidth, this.szWebWidthUnit);
					htmlTable.Align = this.szWebAlign;
					if (!UtilityFunctions.EmptyString(this.szWebHeight))
					{
						htmlTable.Height = string.Concat(this.szWebHeight, this.szWebHeightUnit);
					}
					if (!UtilityFunctions.EmptyString(this.szWebBgColor))
					{
						htmlTable.BgColor = this.szWebBgColor;
					}
				}
			}
			if (!this.ModalForm || !(base.GetFileName().ToLower() == "profilepage.aspx") || base.Request["FormKey"] == null)
			{
				this.BuildPageContent();
				if (this.szWebSiteKey != "" && this.szWebContentKey != "")
				{
					this.oLinks = new eWebPageClass.LinkCollectionClass();
					if (!this.ModalForm)
					{
						if (!this.bSectionExcludeDefaults)
						{
							this.GetSiteDefaultLinks();
						}
						this.GetSectionDefaultLinks();
					}
					this.GetContentLinks();
					for (int j = 0; j < this.oLinks.Count; j++)
					{
						if (this.oLinks[j].szSubMenuType != "")
						{
							this.oLinks[j].GetSubMenuLinks(this.szWebContentKey);
						}
					}
					this.AddLinks();
					this.GetPaneCSSClasses();
					this.SetPaneCSSClasses();


      
				}
			}
			else
			{
				base.szFormKey = base.Request["FormKey"].ToString();
			}
			if (!this.Page.IsPostBack && Config.PageAccessLogging.ToUpper() == "YES")
			{
				this.LogPageAccess();
			}
			if (this.bRemoveEmptyFrameworkCells)
			{
				base.PreRender += new EventHandler(this.Page_PreRender);
			}
            /*HtmlForm nfContent = (HtmlForm)this.Page.FindControl("eWebForm");
            if (nfContent != null)
            {
                
                //var sb = new StringBuilder();
                
                //string nfHtml = nfContent.InnerHtml.ToString();

                //nfHtml.Replace("<tbody", "<div");//.Replace("<table", "<div").Replace("</table>", "</div>")
                    //.Replace("</tbody>", "</div>").Replace("<tr", "<ul").Replace("</tr>", "</ul>")
                    //.Replace("<td", "<li").Replace("</td>", "</li>");
                //nfContent.InnerHtml = "7";
            }*/
		}

		protected void InitializeSite()
		{
			string str;
			if (!base.IsPostBack)
			{
				UtilityFunctions.CheckGC();
			}
			Config.EntityCheckOff();
			this.Session["LoginName"] = "";
			if (base.Request["Modal"] != null)
			{
				this.ModalForm = true;
			}
			if (base.Request["Design"] == null)
			{
				if (this.Session["ewebDesigner"] != null)
				{
					this.bDesigner = (bool)this.Session["ewebDesigner"];
				}
			}
			else
			{
				if (base.Request["Design"].ToString().ToLower() != "yes")
				{
					this.bDesigner = false;
				}
				else
				{
					this.bDesigner = true;
				}
				this.Session["ewebDesigner"] = this.bDesigner;
			}
			if (this.bDesigner)
			{
				this.nTableBorder = 1;
			}
			if (Config.SystemOptions == null || Config.SystemOptions != null && Config.SystemOptions.Count == 0)
			{
				try
				{
					if (Config.SystemOptions != null)
					{
						str = "eweb - System Option count is 0";
					}
					else
					{
						str = "eweb - System Options is null";
					}
					SMTP sMTP = new SMTP();
					sMTP.SendMail(Config.EmailNotifyFrom, "hstechl@avectra.com", "", "", str, "", MailPriority.High, false);
				}
				catch
				{
				}
			}
			if (Config.SystemOptions == null || Config.SystemOptions.Count == 0)
			{
				SystemFunctions.LoadSystemOptions();
			}
			CacheUtils.CheckCacheSetup();
			this.hashPanes = new Hashtable();
			this.hashCSSClasses = new Hashtable();
			if (base.Request.QueryString["Site"] != null)
			{
				this.szWebSiteCode = base.Request.QueryString["Site"].ToString();
			}
			else
			{
				if (this.Session["eWebSiteID"] != null)
				{
					this.szWebSiteCode = this.Session["eWebSiteID"].ToString();
				}
				else
				{
					if (ConfigurationManager.AppSettings["defaultWebSiteCode"] != null)
					{
						this.szWebSiteCode = ConfigurationManager.AppSettings["defaultWebSiteCode"].ToString();
					}
					else
					{
						this.szWebSiteCode = "eWeb";
					}
				}
			}
			this.Session["eWebSiteID"] = this.szWebSiteCode;
			if (Config.GetSystemOptionBoolean("MultiEntitySystem"))
			{
				this.szWebSiteEntityKey = Functions.GetEntityKeyFromSiteCode(this.szWebSiteCode);
				if (!UtilityFunctions.EmptyString(this.szWebSiteEntityKey))
				{
					Functions.CheckForWebEntityKey(this.szWebSiteEntityKey, this.szWebSiteCode);
					Functions.CheckForLoginEntityKey(this.szWebSiteEntityKey, this.Session);
				}
			}
			if (base.Request["WebKey"] != null)
			{
				this.szWebContentKey = base.Request["WebKey"].ToString();
			}
			if (base.Request["WebCode"] != null)
			{
				this.szWebContentCode = base.Request["WebCode"].ToString();
			}
			if (base.Request["Key"] != null)
			{
				this.szRecordKey = base.Request["Key"].ToString();
			}
			if (this.szWebContentKey == "" && this.szWebContentCode != "")
			{
				this.szWebContentKey = this.GetWebContentKeyByCode();
			}
			Config.ApplicationPath = base.Request.ApplicationPath;
			Config.HttpServerName = base.Request.ServerVariables["HTTP_HOST"].ToString();
			if (base.Request.ServerVariables["SERVER_PORT_SECURE"] != "0")
			{
				this.szHttp = "https://";
			}
			else
			{
				this.szHttp = "http://";
			}
			Config.HttpServerPath = string.Concat(this.szHttp, HttpContext.Current.Request.ServerVariables["HTTP_HOST"].ToString(), HttpContext.Current.Request.ApplicationPath);
			this.LoadUserProfile();
			this.Session["StartPage"] = string.Concat(base.Request.ApplicationPath, "/StartPage.aspx");
			if (this.Session["eWebCurrentUserName"] == null || UtilityFunctions.EmptyString(this.Session["eWebCurrentUserName"].ToString()))
			{
				this.Session["UserName"] = "WebUser";
			}
			else
			{
				this.Session["UserName"] = this.Session["eWebCurrentUserName"];
			}
			if (UtilityFunctions.EmptyString(Config.GetUserOption("VisualTheme")))
			{
				if (this.Session["StyleSheet"] == null)
				{
					if (Config.StyleSheet == "" || Config.StyleSheet == null)
					{
						this.Session["StyleSheet"] = string.Concat(base.Request.ApplicationPath, "/style/empty.css");
						return;
					}
					else
					{
						this.Session["StyleSheet"] = string.Concat(base.Request.ApplicationPath, Config.StyleSheet);
					}
				}
				return;
			}
			else
			{
				this.Session["StyleSheet"] = string.Concat(base.Request.ApplicationPath, "/style/", Config.GetUserOption("VisualTheme"));
				return;
			}
		}

		protected void InstantiateContentFormFacadeObject(string szContentFormKey)
		{
			this.InstantiateContentFormFacadeObject(szContentFormKey, false);
		}

		protected void InstantiateContentFormFacadeObject(string szContentFormKey, bool bHtmlContent)
		{
			this.InstantiateContentFormFacadeObject(szContentFormKey, bHtmlContent, false);
		}

		protected void InstantiateContentFormFacadeObject(string szContentFormKey, bool bHtmlContent, bool bAddScriptManger)
		{
			bool flag;
			bool lower;
			base.szFormKey = szContentFormKey;
			eWebPageClass _eWebPageClass = this;
			if (!this.bSiteEnableAtlas)
			{
				flag = false;
			}
			else
			{
				flag = bAddScriptManger;
			}
			_eWebPageClass.SetPropertyValues(flag);
			if (this.bSiteEnableAtlas && bAddScriptManger)
			{
				PageUtils.AddScriptManager(base.Form, this);
			}
			if (this.Session["LinkAgent"] != null)
			{
				if (this.Session[this.szObjectName] == null)
				{
					this.Session[this.szObjectName] = DataUtils.InstantiateFacadeObject(this.szObjectName, base.szFormKey, this.Session);
				}
				base.CheckLinkAgent((FacadeClass)this.Session[this.szObjectName], this.Session, this.szPrefix);
			}
			if (this.Session[this.szObjectName] == null || base.Request["NoSelect"] == null || !(base.Request["NoSelect"].ToString().ToLower() == "yes"))
			{
				if (!bHtmlContent || this.Session[this.szObjectName] == null)
				{
					this.Session[this.szObjectName] = DataUtils.GetFacadeObject(this.Page, this.szObjectName, base.szFormKey);
				}
				this.oFacadeObject = (FacadeClass)this.Session[this.szObjectName];
				if (base.bIsFirstTime && this.oFacadeObject != null && this.szRecordKey != "")
				{
					this.oFacadeObject.CurrentKey = this.szRecordKey;
					this.oFacadeObject.SelectByKey();
				}
			}
			else
			{
				this.oFacadeObject = (FacadeClass)this.Session[this.szObjectName];
			}
			this.szFormTitle = DataUtils.ParseValues(this.oFacadeObject, this.szFormTitle, false);
			this.szFormTitle = UtilityFunctions.RemoveMissingFields(this.szFormTitle);
			if (!this.bTitleFlag)
			{
				if (!this.szFormTitle.StartsWith("@"))
				{
					if (!DataUtils.EmptyKey(this.oFacadeObject))
					{
						this.PageTitle = string.Concat("Edit - ", this.szFormTitle);
					}
					else
					{
						this.PageTitle = string.Concat("Add - ", this.szFormTitle);
					}
				}
				else
				{
					this.PageTitle = this.szFormTitle.Substring(1);
				}
			}
			if (!bHtmlContent)
			{
				if (this.oFacadeObject != null)
				{
					FacadeClass facadeClass = this.oFacadeObject;
					if (base.Request["DoNotSave"] == null)
					{
						lower = false;
					}
					else
					{
						lower = base.Request["DoNotSave"].ToString().ToLower() == "yes";
					}
					facadeClass.DoNotSave = lower;
				}
				if (base.bIsFirstTime && (base.Request["Action"] != null || base.Request["Key"] != null) && base.Request["ParentObject"] != null)
				{
					if (UtilityFunctions.EmptyString(this.oFacadeObject.GetValue(this.oFacadeObject.GetDataObject(0).KeyField)))
					{
						Guid guid = Guid.NewGuid();
						this.oFacadeObject.SetValue(this.oFacadeObject.GetDataObject(0).KeyField, guid.ToString());
					}
					FacadeClass item = (FacadeClass)this.Session[base.Request["ParentObject"].ToString()];
					if (item != null && item.oDetailsHash != null && base.Request["ParentDataObject"] != null)
					{
						this.oFacadeObject.oParentFacadeDetails = (FacadeDetails)item.oDetailsHash[base.Request["ParentDataObject"].ToString()];
					}
				}
			}
		}

		public void KeepAlive(string szUrl)
		{
			if (!UtilityFunctions.EmptyString(szUrl))
			{
				char[] chrArray = new char[1];
				chrArray[0] = ';';
				Array arrays = szUrl.Split(chrArray);
				string str = "";
				str = string.Concat(str, "<script language=\"javascript\">", UtilityFunctions.CRLF());
				str = string.Concat(str, "<!--", UtilityFunctions.CRLF());
				for (int i = 0; i < arrays.Length; i++)
				{
					string str1 = str;
					string[] strArrays = new string[5];
					strArrays[0] = str1;
					strArrays[1] = "blackHole";
					strArrays[2] = i.ToString();
					strArrays[3] = " = new Image();";
					strArrays[4] = UtilityFunctions.CRLF();
					str = string.Concat(strArrays);
					string str2 = str;
					string[] strArrays1 = new string[7];
					strArrays1[0] = str2;
					strArrays1[1] = "blackHole";
					strArrays1[2] = i.ToString();
					strArrays1[3] = ".src = '";
					strArrays1[4] = arrays.GetValue(i).ToString();
					strArrays1[5] = "';";
					strArrays1[6] = UtilityFunctions.CRLF();
					str = string.Concat(strArrays1);
				}
				str = string.Concat(str, "//-->", UtilityFunctions.CRLF());
				str = string.Concat(str, "</SCRIPT>", UtilityFunctions.CRLF());
				this.Page.ClientScript.RegisterClientScriptBlock(this.Page.GetType(), "KeepAlive", str);
			}
		}

		protected void LoadUserProfile()
		{
		}

		private bool LoginByCookie()
		{
			bool flag;
			bool flag1;
			string value;
			bool flag2 = false;
			if (this.Session["CustomerKey"] == null)
			{
				if (base.Request.Cookies[string.Concat(this.Session["eWebSiteID"].ToString(), "login")] == null || !(base.Request.Cookies[string.Concat(this.Session["eWebSiteID"].ToString(), "login")].Value != "") || base.Request.Cookies[string.Concat(this.Session["eWebSiteID"].ToString(), "pw")] == null)
				{
					if (base.Request.Cookies[string.Concat(this.Session["eWebSiteID"].ToString(), "cstid")] != null && base.Request.Cookies[string.Concat(this.Session["eWebSiteID"].ToString(), "cstid")].Value != "")
					{
						string str = base.Request.Cookies[string.Concat(this.Session["eWebSiteID"].ToString(), "cstid")].Value;
						if (Functions.LoginByCustomerID(str))
						{
							flag2 = true;
							this.Session["ForcePasswordChange"] = false;
						}
					}
					if (!UtilityFunctions.EmptyString(Config.GetSystemOption("eWebLoginByCustomerIDCookieName")) && base.Request.Cookies[Config.GetSystemOption("eWebLoginByCustomerIDCookieName")] != null && base.Request.Cookies[Config.GetSystemOption("eWebLoginByCustomerIDCookieName")].Value != "")
					{
						string value1 = base.Request.Cookies[Config.GetSystemOption("eWebLoginByCustomerIDCookieName")].Value;
						if (Functions.LoginByCustomerID(value1))
						{
							flag2 = true;
							this.Session["ForcePasswordChange"] = false;
						}
					}
				}
				else
				{
					if (Config.GetSystemOption("useEmailForAuthorization") == null || Config.GetSystemOption("useEmailForAuthorization").ToString().ToLower() == "true")
					{
						flag = true;
					}
					else
					{
						flag = false;
					}
					if (ConfigurationManager.AppSettings["verifyPassword"] == null || ConfigurationManager.AppSettings["verifyPassword"].ToString().ToLower() == "false")
					{
						flag1 = false;
					}
					else
					{
						flag1 = true;
					}
					string str1 = base.Request.Cookies[string.Concat(this.Session["eWebSiteID"].ToString(), "login")].Value;
					if (!flag1)
					{
						value = null;
					}
					else
					{
						value = base.Request.Cookies[string.Concat(this.Session["eWebSiteID"].ToString(), "pw")].Value;
						try
						{
							if (!UtilityFunctions.EmptyString(value))
							{
								string lower = "C3BC4902-A043-4A61-B51D-603DABA62575";
								if (ConfigurationManager.AppSettings["eWebUserPasswordCookieEncryptionKey"] != null && ConfigurationManager.AppSettings["eWebUserPasswordCookieEncryptionKey"].Trim() != "")
								{
									lower = ConfigurationManager.AppSettings["eWebUserPasswordCookieEncryptionKey"].Trim().ToLower();
								}
								value = CryptoUtils.Decrypt(value, lower);
							}
						}
						catch
						{
						}
					}
					if (!flag)
					{
						if (Functions.LoginByCustomerWebLogin(str1, value))
						{
							flag2 = true;
						}
					}
					else
					{
						if (Functions.LoginByEMail(str1, value))
						{
							flag2 = true;
						}
					}
				}
			}
			else
			{
				flag2 = true;
			}
			return flag2;
		}

		private bool LoginByRequest(string szType)
		{
			NameValueCollection form;
			bool flag;
			bool flag1;
			string str;
			if (szType != "GET")
			{
				form = base.Request.Form;
			}
			else
			{
				form = base.Request.QueryString;
			}
			bool flag2 = false;
			bool flag3 = false;
			if (form[string.Concat(this.Session["eWebSiteID"].ToString(), "login")] == null || !(form[string.Concat(this.Session["eWebSiteID"].ToString(), "login")].ToString() != "") || form[string.Concat(this.Session["eWebSiteID"].ToString(), "pw")] == null)
			{
				if (form[string.Concat(this.Session["eWebSiteID"].ToString(), "cstid")] == null || !(form[string.Concat(this.Session["eWebSiteID"].ToString(), "cstid")].ToString() != ""))
				{
					if (form[string.Concat(this.Session["eWebSiteID"].ToString(), "token")] != null && form[string.Concat(this.Session["eWebSiteID"].ToString(), "token")].ToString() != "")
					{
						flag3 = true;
						string str1 = form[string.Concat(this.Session["eWebSiteID"].ToString(), "token")].ToString();
						if (Functions.LoginByToken(str1))
						{
							flag2 = true;
							this.Session["ForcePasswordChange"] = false;
						}
					}
				}
				else
				{
					flag3 = true;
					string str2 = form[string.Concat(this.Session["eWebSiteID"].ToString(), "cstid")].ToString();
					if (Functions.LoginByCustomerID(str2))
					{
						flag2 = true;
						this.Session["ForcePasswordChange"] = false;
					}
				}
			}
			else
			{
				flag3 = true;
				if (Config.GetSystemOption("useEmailForAuthorization") == null || Config.GetSystemOption("useEmailForAuthorization").ToString().ToLower() == "true")
				{
					flag = true;
				}
				else
				{
					flag = false;
				}
				if (ConfigurationManager.AppSettings["verifyPassword"] == null || ConfigurationManager.AppSettings["verifyPassword"].ToString().ToLower() == "false")
				{
					flag1 = false;
				}
				else
				{
					flag1 = true;
				}
				string str3 = form[string.Concat(this.Session["eWebSiteID"].ToString(), "login")].ToString();
				if (!flag1)
				{
					str = null;
				}
				else
				{
					str = form[string.Concat(this.Session["eWebSiteID"].ToString(), "pw")].ToString();
				}
				if (!flag)
				{
					if (Functions.LoginByCustomerWebLogin(str3, str))
					{
						flag2 = true;
						this.Session["ForcePasswordChange"] = false;
					}
				}
				else
				{
					if (Functions.LoginByEMail(str3, str))
					{
						flag2 = true;
						this.Session["ForcePasswordChange"] = false;
					}
				}
			}
			if (flag3)
			{
				if (!flag2)
				{
					if (base.Request["URL_failed"] != null && base.Request["URL_failed"].ToString() != "")
					{
						PageUtils.Redirect(base.Request["URL_failed"].ToString(), true);
					}
				}
				else
				{
					if (base.Request["URL_success"] != null && base.Request["URL_success"].ToString() != "")
					{
						PageUtils.Redirect(DataUtils.ParseValues((CO_Individual)Config.Session["Individual"], base.Server.UrlDecode(base.Request["URL_success"].ToString()), false), true);
					}
				}
			}
			return flag2;
		}

		public void LoginCustom(string szCustomMethod)
		{
			char[] chrArray = new char[1];
			chrArray[0] = '|';
			Array arrays = szCustomMethod.Split(chrArray);
			string str = arrays.GetValue(0).ToString().Trim();
			string str1 = arrays.GetValue(1).ToString().Trim();
			string str2 = arrays.GetValue(2).ToString().Trim();
			string str3 = arrays.GetValue(3).ToString().Trim();
			PageUtils.ExecuteObjectMethod(str, str1, str2, str3, this, null, null);
		}

		protected void LogPageAccess()
		{
			if (Config.PageAccessLogging.ToUpper() == "YES")
			{
				string str = base.Request.Url.ToString().Trim();
				string str1 = str;
				if (str.Length > 400)
				{
					str = str.Substring(0, 400);
				}
				string str2 = base.Request.QueryString.ToString();
				if (str2.Length > 400)
				{
					str2 = str2.Substring(0, 400);
				}
				string key = this.GetKey(str1, "Key");
				string currentUserName = Functions.CurrentUserName;
				string str3 = "INSERT INTO md_page_access(";
				str3 = string.Concat(str3, "mpa_url,mpa_app_path,mpa_host_name,mpa_host_address,mpa_local_address,mpa_user_agent,mpa_query_string,mpa_user_name,mpa_cgi_key,mpa_cgl_key,mpa_dyn_key,mpa_rpt_key,mpa_web_key,mpa_wbc_key,mpa_acd_key,mpa_current_key");
				str3 = string.Concat(str3, ") VALUES (");
				str3 = string.Concat(str3, "N'", str.Replace("'", "''"), "',");
				str3 = string.Concat(str3, "N'", base.Request.ApplicationPath.Replace("'", "''"), "',");
				str3 = string.Concat(str3, "N'", base.Request.UserHostName.Replace("'", "''"), "',");
				str3 = string.Concat(str3, "N'", base.Request.UserHostAddress.Replace("'", "''"), "',");
				str3 = string.Concat(str3, "N'", base.Request.ServerVariables["LOCAL_ADDR"].ToString().Replace("'", "''"), "',");
				str3 = string.Concat(str3, "N'", base.Request.UserAgent.Replace("'", "''"), "',");
				str3 = string.Concat(str3, "N'", str2.Replace("'", "''"), "',");
				str3 = string.Concat(str3, "N'", currentUserName.Replace("'", "''"), "',");
				if (!UtilityFunctions.EmptyString(this.GetKey(str1, "ItemKey")))
				{
					str3 = string.Concat(str3, "N'", this.GetKey(str1, "ItemKey").Replace("'", "''"), "',");
				}
				else
				{
					str3 = string.Concat(str3, "null,");
				}
				if (!UtilityFunctions.EmptyString(this.GetKey(str1, "LinkKey")))
				{
					str3 = string.Concat(str3, "N'", this.GetKey(str1, "LinkKey").Replace("'", "''"), "',");
				}
				else
				{
					str3 = string.Concat(str3, "null,");
				}
				if (!UtilityFunctions.EmptyString(this.GetKey(str1, "FormKey")))
				{
					str3 = string.Concat(str3, "N'", this.GetKey(str1, "FormKey").Replace("'", "''"), "',");
				}
				else
				{
					str3 = string.Concat(str3, "null,");
				}
				if (!UtilityFunctions.EmptyString(this.GetKey(str1, "ReportKey")))
				{
					str3 = string.Concat(str3, "N'", this.GetKey(str1, "ReportKey").Replace("'", "''"), "',");
				}
				else
				{
					str3 = string.Concat(str3, "null,");
				}
				if (!UtilityFunctions.EmptyString(this.szWebSiteKey))
				{
					str3 = string.Concat(str3, "N'", this.szWebSiteKey.Replace("'", "''"), "',");
				}
				else
				{
					str3 = string.Concat(str3, "null,");
				}
				if (!UtilityFunctions.EmptyString(this.szWebContentKey))
				{
					str3 = string.Concat(str3, "N'", this.szWebContentKey.Replace("'", "''"), "',");
				}
				else
				{
					str3 = string.Concat(str3, "null,");
				}
				if (base.Request["acd_key"] != null)
				{
					if (UtilityFunctions.EmptyString(base.Request["acd_key"]) || !UtilityFunctions.IsGuid(base.Request["acd_key"].ToString()))
					{
						str3 = string.Concat(str3, "null,");
					}
					else
					{
						str3 = string.Concat(str3, "N'", base.Request["acd_key"].ToString().Replace("'", "''"), "',");
					}
					this.Session["acd_key"] = base.Request["acd_key"].ToString();
				}
				else
				{
					if (this.Session["acd_key"] != null)
					{
						if (!UtilityFunctions.IsGuid(this.Session["acd_key"].ToString()))
						{
							str3 = string.Concat(str3, "null,");
						}
						else
						{
							str3 = string.Concat(str3, "N'", this.Session["acd_key"].ToString().Replace("'", "''"), "',");
						}
					}
					else
					{
						str3 = string.Concat(str3, "null,");
					}
				}
				if (UtilityFunctions.EmptyString(key))
				{
					str3 = string.Concat(str3, "null");
				}
				else
				{
					str3 = string.Concat(str3, "N'", key.Replace("'", "''"), "'");
				}
				str3 = string.Concat(str3, ")");
				DataUtils.ExecuteSql(str3);
			}
		}

		protected void Page_PreRender(object sender, EventArgs e)
		{
			if (this.bRemoveEmptyFrameworkCells)
			{
				Functions.RemoveEmptyFrameworkCells(this.Page);
			}
		}

		private string RemoveEmailContentParameter(string szUrl)
		{
			string str = "&emailcontent=yes";
			int num = szUrl.ToLower().IndexOf(str);
			if (num >= 0)
			{
				szUrl = szUrl.Remove(num, str.Length);
			}
			return szUrl;
		}

		protected void SetOnLoadAttributes(HtmlGenericControl BodyTag)
		{
			if (BodyTag != null && base.bIsFirstTime)
			{
				BodyTag.Attributes.Remove("onload");
				BodyTag.Attributes.Add("onload", "");
				if (this.ModalForm)
				{
					string item = BodyTag.Attributes["onload"];
					item = string.Concat(item, "DialogBlockParent();");
					item = string.Concat(item, "SetWindowSize();");
					BodyTag.Attributes.Add("onload", item);
				}
			}
			if (this.Session["oWizard"] != null)
			{
				WizardClass wizardClass = (WizardClass)this.Session["oWizard"];
				this.Session["oWizard"] = null;
				if (Config.Context.Request["__EVENTTARGET"] != null)
				{
					string str = Config.Context.Request["__EVENTTARGET"].ToString();
					if (str.IndexOf("BypassDupCheck") == 0)
					{
						this.Page.Validate();
						wizardClass.HandleButtonClick(Convert.ToInt32(str.Substring(str.LastIndexOf("_") + 1)), str);
					}
				}
			}
		}

		protected void SetPaneCSSClasses()
		{
			foreach (DictionaryEntry hashCSSClass in this.hashCSSClasses)
			{
				eWebPageClass.PaneCssClass value = (eWebPageClass.PaneCssClass)hashCSSClass.Value;
				if (value.szPaneID != "FrameWorkTable")
				{
					HtmlTableCell htmlTableCell = (HtmlTableCell)this.Page.FindControl(value.szPaneID);
					if (htmlTableCell == null)
					{
						continue;
					}
					if (value.szCssClass != "")
					{
						htmlTableCell.Attributes.Add("class", value.szCssClass);
					}
					if (value.nWidth > 0)
					{
						htmlTableCell.Width = string.Concat(value.nWidth.ToString(), value.szWidthUnit);
					}
					if (value.nHeight > 0)
					{
						htmlTableCell.Height = string.Concat(value.nHeight.ToString(), value.szHeightUnit);
					}
					htmlTableCell.Align = value.szAlign;
					htmlTableCell.VAlign = value.szVAlign;
					htmlTableCell.BgColor = value.szBGColor;
				}
				else
				{
					HtmlTable htmlTable = (HtmlTable)this.Page.FindControl("PageFrameWorkTable");
					if (htmlTable == null)
					{
						continue;
					}
					if (value.szCssClass != "")
					{
						htmlTable.Attributes.Add("class", value.szCssClass);
					}
					if (value.nWidth > 0)
					{
						htmlTable.Width = string.Concat(value.nWidth.ToString(), value.szWidthUnit);
					}
					if (value.nHeight > 0)
					{
						htmlTable.Height = string.Concat(value.nHeight.ToString(), value.szHeightUnit);
					}
					htmlTable.Align = value.szAlign;
					htmlTable.BgColor = value.szBGColor;
				}
			}
		}

		private void SetupLinkHyperLink(eWebPageClass.LinkClass oLink, Control oContainer, HyperLink oHyperLink)
		{
			if (oContainer is HtmlTableCell)
			{
				((HtmlTableCell)oContainer).NoWrap = true;
			}
			Image image = null;
			if (oLink.szImageFile == "")
			{
				Literal literal = new Literal();
				literal.Text = "&nbsp;";
				oContainer.Controls.Add(literal);
			}
			if (oLink.szImageFile != "" && oLink.szLinkText != "")
			{
				image = new Image();
				if (!oLink.bActive || !(oLink.szImageFileActive != ""))
				{
					image.ImageUrl = oLink.szImageFile;
				}
				else
				{
					image.ImageUrl = oLink.szImageFileActive;
				}
				image.AlternateText = oLink.szDescription;
				image.ImageAlign = ImageAlign.Middle;
				oContainer.Controls.Add(image);
			}
			if (!oLink.bActive)
			{
				if (oLink.szImageFile != "" && oLink.szImageFileHover != "")
				{
					oHyperLink.Attributes.Add("onmouseover", string.Concat("ImageSrcChange('", oLink.szImageFileHover.Replace("~", Config.HttpServerPath), "');"));
					oHyperLink.Attributes.Add("onmouseout", string.Concat("ImageSrcChange('", oLink.szImageFile.Replace("~", Config.HttpServerPath), "');"));
					if (image != null)
					{
						image.Attributes.Add("onmouseover", string.Concat("ImageSrcChange('", oLink.szImageFileHover.Replace("~", Config.HttpServerPath), "');"));
						image.Attributes.Add("onmouseout", string.Concat("ImageSrcChange('", oLink.szImageFile.Replace("~", Config.HttpServerPath), "');"));
					}
				}
                oHyperLink.Text = oLink.szLinkText;
				oContainer.Controls.Add(oHyperLink);
				return;
			}
			else
			{
				if (oHyperLink.ImageUrl != "")
				{
					image = new Image();
					image.ImageUrl = oHyperLink.ImageUrl;
					image.ImageAlign = ImageAlign.Middle;
					oContainer.Controls.Add(image);
				}
				Label label = new Label();
				label.Text = oHyperLink.Text;
				label.CssClass = oHyperLink.CssClass;
				oContainer.Controls.Add(label);
				return;
			}
		}

		private string SetupListPager(string szHtml, bool bPager, string szPagerHtml)
		{
			string str = "{BeginPager}";
			string str1 = "{EndPager}";
			if (!bPager || !(szPagerHtml != ""))
			{
				szHtml = szHtml.Replace(str, "").Replace(str1, "");
			}
			else
			{
				int num = 0;
				while (szHtml.IndexOf(str) >= 0)
				{
					if (num < 10)
					{
						num++;
						int num1 = szHtml.IndexOf(str);
						int num2 = szHtml.IndexOf(str1);
						if (num1 < 0 || num2 < 0)
						{
							continue;
						}
						szHtml = szHtml.Remove(num1, num2 + str1.Length - num1);
						szHtml = szHtml.Insert(num1, szPagerHtml);
					}
					else
					{
						break;
					}
				}
			}
			return szHtml;
		}

		protected class LinkClass
		{
			public string szUniqueKey;

			public string szLinkKey;

			public bool bActive;

			public string szPosition;

			public string szSection;

			public string szLinkContentKey;

			public string szParentKey;

			public string szLinkPageName;

			public string szLinkParentKey;

			public string szLinkParameters;

			public string szLinkText;

			public string szDescription;

			public string szImageFile;

			public string szImageFileHover;

			public string szImageFileActive;

			public string szSubMenuType;

			public bool bRequiresLogin;

			public bool bDisableUponLogin;

			public string szExternalURL;

			public bool bTargetNew;

			public string szCssClass;

			public bool bIsDefault;

			public bool bIsSectionDefault;

			public string szSQLVisible;

			public bool bSectionSSL;

			public bool bLinkSSL;

			public eWebPageClass.LinkCollectionClass SubMenuLinks;

			public bool bSSL
			{
				get
				{
					if (this.bSectionSSL)
					{
						return true;
					}
					else
					{
						return this.bLinkSSL;
					}
				}
			}

			public LinkClass()
			{
				this.szUniqueKey = "";
				this.szLinkKey = "";
				this.szPosition = "";
				this.szSection = "";
				this.szLinkContentKey = "";
				this.szParentKey = "";
				this.szLinkPageName = "";
				this.szLinkParentKey = "";
				this.szLinkParameters = "";
				this.szLinkText = "";
				this.szDescription = "";
				this.szImageFile = "";
				this.szImageFileHover = "";
				this.szImageFileActive = "";
				this.szSubMenuType = "";
				this.szExternalURL = "";
				this.szCssClass = "";
				this.szSQLVisible = "";
				this.SubMenuLinks = new eWebPageClass.LinkCollectionClass();
			}

			public void GetSubMenuLinks(string szCurrentContentKey)
			{
				bool flag;
				bool flag1;
				bool flag2;
				bool flag3;
				bool flag4;
				if (this.szSubMenuType != "")
				{
					string[] strArrays = new string[9];
					strArrays[0] = "SELECT wbl_key,wbl_wbc_key,wbl_wbl_key,wbc_page_name,wbc_parameters,wbl_link_text,wbl_description,wbl_image_file,wbl_image_file_hover,wbl_image_file_active,wbl_submenu_type,wbl_requires_login,wbs_requires_login,wbl_disable_upon_login,wbl_external_url,wbl_target_new,wbl_cssclass,wbc_sql_visible,wbs_sql_visible,wbs_ssl_flag,wbl_ssl_flag  FROM md_web_link ";
					strArrays[1] = DataUtils.NoLock();
					strArrays[2] = " LEFT JOIN md_web_content ";
					strArrays[3] = DataUtils.NoLock();
					strArrays[4] = " ON wbc_key=wbl_wbc_key LEFT JOIN md_web_section ";
					strArrays[5] = DataUtils.NoLock();
					strArrays[6] = " ON wbs_key=wbc_wbs_key WHERE wbl_wbl_key=";
					strArrays[7] = DataUtils.ValuePrep(this.szLinkKey, "av_key", true);
					strArrays[8] = " AND wbl_delete_flag=0 ORDER BY wbl_order";
					string str = string.Concat(strArrays);
					OleDbConnection connection = DataUtils.GetConnection();
					using (connection)
					{
						OleDbDataReader dataReader = DataUtils.GetDataReader(str, connection);
						using (dataReader)
						{
							if (dataReader != null)
							{
								int num = 0;
								try
								{
									while (dataReader.Read())
									{
										num++;
										eWebPageClass.LinkClass linkClass = new eWebPageClass.LinkClass();
										linkClass.szUniqueKey = string.Concat(this.szUniqueKey, num.ToString());
										linkClass.szLinkKey = dataReader["wbl_key"].ToString();
										linkClass.szLinkContentKey = dataReader["wbl_wbc_key"].ToString();
										linkClass.szParentKey = dataReader["wbl_wbl_key"].ToString();
										linkClass.szLinkPageName = dataReader["wbc_page_name"].ToString();
										linkClass.szLinkParameters = dataReader["wbc_parameters"].ToString();
										linkClass.szLinkText = dataReader["wbl_link_text"].ToString();
										linkClass.szDescription = dataReader["wbl_description"].ToString();
										linkClass.szImageFile = dataReader["wbl_image_file"].ToString();
										linkClass.szImageFileHover = dataReader["wbl_image_file_hover"].ToString();
										linkClass.szImageFileActive = dataReader["wbl_image_file_active"].ToString();
										linkClass.szSubMenuType = dataReader["wbl_submenu_type"].ToString();
										eWebPageClass.LinkClass linkClass1 = linkClass;
										if (dataReader["wbl_requires_login"].ToString() == "1")
										{
											flag = true;
										}
										else
										{
											flag = false;
										}
										linkClass1.bRequiresLogin = flag;
										if (!linkClass.bRequiresLogin && dataReader["wbs_requires_login"].ToString() == "1")
										{
											linkClass.bRequiresLogin = true;
										}
										eWebPageClass.LinkClass linkClass2 = linkClass;
										if (dataReader["wbl_disable_upon_login"].ToString() == "1")
										{
											flag1 = true;
										}
										else
										{
											flag1 = false;
										}
										linkClass2.bDisableUponLogin = flag1;
										linkClass.szExternalURL = dataReader["wbl_external_url"].ToString();
										eWebPageClass.LinkClass linkClass3 = linkClass;
										if (dataReader["wbl_target_new"].ToString() == "1")
										{
											flag2 = true;
										}
										else
										{
											flag2 = false;
										}
										linkClass3.bTargetNew = flag2;
										linkClass.szCssClass = dataReader["wbl_cssclass"].ToString();
										if (linkClass.szSubMenuType != "")
										{
											linkClass.GetSubMenuLinks(szCurrentContentKey);
										}
										if (linkClass.szLinkContentKey == szCurrentContentKey && linkClass.szExternalURL.IndexOf("EmailContent=") < 0)
										{
											linkClass.bActive = true;
										}
										linkClass.szSQLVisible = dataReader["wbc_sql_visible"].ToString();
										if (linkClass.szSQLVisible == "")
										{
											linkClass.szSQLVisible = dataReader["wbs_sql_visible"].ToString();
										}
										eWebPageClass.LinkClass linkClass4 = linkClass;
										if (dataReader["wbs_ssl_flag"].ToString() == "1")
										{
											flag3 = true;
										}
										else
										{
											flag3 = false;
										}
										linkClass4.bSectionSSL = flag3;
										eWebPageClass.LinkClass linkClass5 = linkClass;
										if (dataReader["wbl_ssl_flag"].ToString() == "1")
										{
											flag4 = true;
										}
										else
										{
											flag4 = false;
										}
										linkClass5.bLinkSSL = flag4;
										this.SubMenuLinks.Add(linkClass);
									}
								}
								catch (Exception exception1)
								{
									Exception exception = exception1;
									Config.LastError = new ErrorClass();
									Config.LastError.Message = exception.Message;
									if (exception.InnerException != null && exception.InnerException.Message != null)
									{
										ErrorClass lastError = Config.LastError;
										lastError.Message = string.Concat(lastError.Message, "; ", exception.InnerException.Message);
									}
									Config.LastError.Number = -1;
								}
							}
						}
					}
				}
			}
		}

		protected class LinkCollectionClass : CollectionBase
		{
			public eWebPageClass.LinkClass this[int nIndex]
			{
				get
				{
					return (eWebPageClass.LinkClass)base.InnerList[nIndex];
				}
			}

			public eWebPageClass.LinkClass this[string szLinkKey]
			{
				get
				{
					eWebPageClass.LinkClass item = null;
					int num = 0;
					while (num < base.Count)
					{
						if (this[num].szLinkKey != szLinkKey)
						{
							num++;
						}
						else
						{
							item = this[num];
							break;
						}
					}
					return item;
				}
			}

			public LinkCollectionClass()
			{
			}

			public bool Add(eWebPageClass.LinkClass oLink)
			{
				base.List.Add(oLink);
				return true;
			}
		}

		protected class PaneCssClass
		{
			public string szPaneID;

			public string szCssClass;

			public int nHeight;

			public string szHeightUnit;

			public int nWidth;

			public string szWidthUnit;

			public string szAlign;

			public string szVAlign;

			public string szBGColor;

			public PaneCssClass()
			{
				this.szPaneID = "";
				this.szCssClass = "";
				this.szHeightUnit = "";
				this.szWidthUnit = "";
				this.szAlign = "";
				this.szVAlign = "";
				this.szBGColor = "";
			}
		}

		public class PaneDataClass
		{
			public string szKey;

			public string szContentType;

			public string szDestinationKey;

			public string szFormKey;

			public string szWizardKey;

			public string szCurrentKey;

			public string szContentPath;

			public string szContentHtml;

			public string szControlPath;

			public bool bIsDefault;

			public bool bIsSectionDefault;

			public string szPosition;

			public int nRow;

			public int nCol;

			public int nRowSpan;

			public int nColSpan;

			public int nHeight;

			public string szHeightUnit;

			public int nWidth;

			public string szWidthUnit;

			public string szAlign;

			public string szVAlign;

			public string szBGColor;

			public string szParentDetailKey;

			public bool bIsVisible;

			public FacadeClass oFacadeObject;

			public PaneDataClass()
			{
				this.szKey = "";
				this.szContentType = "";
				this.szDestinationKey = "";
				this.szFormKey = "";
				this.szWizardKey = "";
				this.szCurrentKey = "";
				this.szContentPath = "";
				this.szContentHtml = "";
				this.szControlPath = "";
				this.szPosition = "";
				this.szHeightUnit = "";
				this.szWidthUnit = "";
				this.szAlign = "";
				this.szVAlign = "";
				this.szBGColor = "";
				this.szParentDetailKey = "";
				this.bIsVisible = true;
			}
		}
	}
}