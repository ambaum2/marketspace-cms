using Avectra.netForum.Common;
using Avectra.netForum.Components.OE;
using Avectra.netForum.Data;
using Avectra.netForum.Monitors;
using System;
using System.Collections;
using System.Data.OleDb;
using System.Web;
using System.Web.UI.WebControls;

public class COEBundleWizardClass_EWEB : WizardClass
{
	public COEBundleWizardClass_EWEB()
	{
	}

	public override void HandleButtonClick(int nButtonIndex, string szID)
	{
		MethodMonitor methodMonitor = new MethodMonitor("HandleButtonClick");
		string str = "";
		WizardButtonClass item = this.oWizardButtons[nButtonIndex];
		if (Config.LastError == null || Config.LastError.Number == 0)
		{
			try
			{
				try
				{
					if (!(item.szAction == "Validate") && !(item.szAction == "Save") || this.oPage.IsValid)
					{
						Config.LastError = null;
						if (item.szDestinationKey == "")
						{
							str = item.szDestinationURL;
						}
						else
						{
							string[] strArrays = new string[5];
							strArrays[0] = this.szAspxFileName;
							strArrays[1] = "?WizardKey=";
							strArrays[2] = base.szWizardKey;
							strArrays[3] = "&WizardStep=";
							strArrays[4] = item.szDestinationKey;
							str = string.Concat(strArrays);
							if (this.szWebKey != "")
							{
								str = string.Concat(str, "&WebKey=", this.szWebKey);
							}
							str = UtilityFunctions.AppendQueryStringParameters(str, item.szDestinationFormParameters);
							str = UtilityFunctions.AppendQueryStringParameters(str, item.szDestinationURL);
						}
						if (!item.bExitWizard)
						{
							str = base.CheckForModal(str);
						}
						if (item.szAction != "Validate")
						{
							if (item.szAction != "Save")
							{
								if (item.bExitWizard && this.oPage != null)
								{
									this.oPage.ExecuteExtensionBeforeCancel();
								}
							}
							else
							{
								if (this.oPage.oFacadeObject != null && Config.LastError == null)
								{
									this.oPage.Save_ClickBase(this.oPage, "", true, szID);
								}
								if (Config.LastError == null && this.bShowProgress && !UtilityFunctions.EmptyString(this.szProgressObjectCurrentKey) && !UtilityFunctions.EmptyString(base.szWizardKey) && !UtilityFunctions.EmptyString(base.szWizardStep))
								{
									base.CompleteStep();
								}
							}
						}
						else
						{
							if (this.oPage.oFacadeObject != null)
							{
								this.oPage.oFacadeObject.SetData(this.oPage);
							}
						}
						if (Config.LastError == null)
						{
							if (item.bExitWizard)
							{
								if (HttpContext.Current.Request["Modal"] != "Yes")
								{
									if (str == "")
									{
										if (HttpContext.Current.Session["IntendedURL"] == null)
										{
											if (HttpContext.Current.Session[string.Concat("WizardUrlReferrer_", base.szWizardKey)] != null)
											{
												str = HttpContext.Current.Session[string.Concat("WizardUrlReferrer_", base.szWizardKey)].ToString();
											}
										}
										else
										{
											str = HttpContext.Current.Session["IntendedURL"].ToString();
										}
									}
								}
								else
								{
									if (str == "" || item.szDestinationKey != "")
									{
										str = string.Concat(Config.ApplicationPath, "/CloseWindow.htm?OpenerSubmit=yes");
									}
									else
									{
										str = string.Concat(Config.ApplicationPath, "/CloseWindow.htm?Redirect=", str);
									}
								}
							}
							OrderEntryBundle orderEntryBundle = (OrderEntryBundle)this.oProgressFacadeObject;
							ArrayList arrayLists = new ArrayList();
							foreach (FacadeClass oAssociatedObject in orderEntryBundle.oAssociatedObjects)
							{
								arrayLists.Add(oAssociatedObject);
							}
							foreach (FacadeClass invoiceDetail in orderEntryBundle.oInvoice.InvoiceDetails)
							{
								if (invoiceDetail.GetValue("ivd_prc_prd_ptp_key").ToLower() != Config.GetSystemOption("DefaultProductTypeForSubscription"))
								{
									continue;
								}
								arrayLists.Add(invoiceDetail);
							}
							int num = -1;
							if (Config.Context.Request["AssociatedFacadeObjectIndex"] != null)
							{
								num = UtilityFunctions.ConvertToInt(Config.Context.Request["AssociatedFacadeObjectIndex"].ToString());
							}
							if (item.szAction == "Save")
							{
								int num1 = num + 1;
								if (num1 >= arrayLists.Count)
								{
									if (UtilityFunctions.EmptyString(str))
									{
										str = "DynamicPage.aspx?webcode=COE";
									}
								}
								else
								{
									str = string.Concat("DynamicPage.aspx?WebCode=", HttpContext.Current.Request["webcode"].ToString(), "&WizardKey=", base.szWizardKey);
								}
								str = base.CheckForModal(str);
								if (num1 >= arrayLists.Count)
								{
									if (num1 != arrayLists.Count)
									{
										if (Config.LastError == null)
										{
											if (HttpContext.Current.Request["Modal"] != "Yes")
											{
												if (str == "")
												{
													if (HttpContext.Current.Session["IntendedURL"] == null)
													{
														if (HttpContext.Current.Session[string.Concat("WizardUrlReferrer_", base.szWizardKey)] != null)
														{
															str = HttpContext.Current.Session[string.Concat("WizardUrlReferrer_", base.szWizardKey)].ToString();
														}
													}
													else
													{
														str = HttpContext.Current.Session["IntendedURL"].ToString();
													}
												}
											}
											else
											{
												str = string.Concat(Config.ApplicationPath, "/CloseWindow.htm?Redirect=DynamicPage.aspx?webcode=COE");
											}
										}
									}
									else
									{
										str = string.Concat(str, "&DoNotSave=Yes");
										str = string.Concat(str, "&AssociatedFacadeObjectIndex=999");
										string str1 = string.Concat("select top 1 wzf_key  from md_wizard_form (nolock)  join md_dynamic_form (nolock) on dyn_key = wzf_dyn_key  join md_object (nolock) on obj_key=dyn_obj_key  where wzf_wiz_key=", DataUtils.ValuePrep(base.szWizardKey, "av_key", true), " and obj_name='OrderEntryBundle' order by wzf_order desc");
										OleDbConnection connection = DataUtils.GetConnection();
										using (connection)
										{
											OleDbDataReader dataReader = DataUtils.GetDataReader(str1, connection);
											using (dataReader)
											{
												if (dataReader != null && dataReader.Read())
												{
													str = string.Concat(str, "&WizardStep=", dataReader["wzf_key"].ToString(), "&ParentObject=CentralizedOrderEntry&ParentDataObject=Order Entry Bundle");
												}
											}
										}
									}
								}
								else
								{
									str = string.Concat(str, "&AssociatedFacadeObjectIndex=", num1.ToString());
									str = string.Concat(str, "&DoNotSave=Yes");
									str = string.Concat(str, "&NoSelect=Yes");
									FacadeClass facadeClass = (FacadeClass)arrayLists[num1];
									Config.Context.Session[facadeClass.ObjectName] = facadeClass;
									string str2 = string.Concat("select wzf_key  from md_wizard_form (nolock)  join md_dynamic_form (nolock) on dyn_key = wzf_dyn_key  join md_object (nolock) on obj_key=dyn_obj_key  where wzf_wiz_key=", DataUtils.ValuePrep(base.szWizardKey, "av_key", true), " and obj_name=", DataUtils.ValuePrep(facadeClass.ObjectName, "nvarchar", true));
									OleDbConnection oleDbConnection = DataUtils.GetConnection();
									using (oleDbConnection)
									{
										OleDbDataReader oleDbDataReader = DataUtils.GetDataReader(str2, oleDbConnection);
										using (oleDbDataReader)
										{
											if (oleDbDataReader != null && oleDbDataReader.Read())
											{
												str = string.Concat(str, "&WizardStep=", oleDbDataReader["wzf_key"].ToString());
											}
										}
									}
								}
							}
						}
					}
				}
				catch (Exception exception1)
				{
					Exception exception = exception1;
					UtilityFunctions.SetLastError(exception);
					if (this.oPage.FindControl("LabelMessage") != null)
					{
						UtilityFunctions.ConfigureLabel((Label)this.oPage.FindControl("LabelMessage"), true, Config.LastError.Message);
					}
					if (this.oPage.FindControl("LabelMessageTop") != null)
					{
						UtilityFunctions.ConfigureLabel((Label)this.oPage.FindControl("LabelMessageTop"), true, Config.LastError.Message);
					}
					methodMonitor.Publish("Error", exception, false);
				}
			}
			finally
			{
				methodMonitor.End(this);
			}
			bool flag = true;
			if (flag && UtilityFunctions.EmptyString(str))
			{
				flag = false;
			}
			if (flag && Config.LastError != null)
			{
				if (Config.LastError.Number == 0)
				{
					if (Config.LastError.Level == ErrorClass.ErrorLevel.AddressVerificationFailed || Config.LastError.Level == ErrorClass.ErrorLevel.AlertDialog || Config.LastError.Level == ErrorClass.ErrorLevel.ConfirmDialog || Config.LastError.Level == ErrorClass.ErrorLevel.Duplicate || Config.LastError.Level == ErrorClass.ErrorLevel.Warning)
					{
						flag = false;
					}
				}
				else
				{
					flag = false;
				}
			}
			if (flag)
			{
				HttpContext.Current.Session[string.Concat("WizardUrlReferrer_", base.szWizardKey)] = null;
				PageUtils.Redirect(DataUtils.ParseValues(this.oPage.oFacadeObject, str, false));
			}
		}
		else
		{
			UtilityFunctions.ConfigureLabel((Label)this.oPage.FindControl("LabelMessage"), true, DataUtils.GetUserMessage(this.oProgressFacadeObject));
			if (this.oPage.FindControl("LabelMessageTop") != null)
			{
				UtilityFunctions.ConfigureLabel((Label)this.oPage.FindControl("LabelMessageTop"), true, DataUtils.GetUserMessage(this.oProgressFacadeObject));
				return;
			}
		}
	}

	public override bool InitializeWizardProperties()
	{
		string str;
		bool flag;
		bool flag1;
		bool flag2;
		bool flag3;
		bool flag4;
		bool flag5;
		bool flag6;
		bool flag7;
		MethodMonitor methodMonitor = new MethodMonitor("InitializeWizardProperties");
		bool flag8 = false;
		try
		{
			try
			{
				if (HttpContext.Current.Request["WizardKey"] != null)
				{
					base.szWizardKey = HttpContext.Current.Request["WizardKey"].ToString();
				}
				if (base.szWizardKey != "")
				{
					if (HttpContext.Current.Request["WizardStep"] != null)
					{
						base.szWizardStep = HttpContext.Current.Request["WizardStep"].ToString();
					}
					if (base.szWizardStep != "")
					{
						string[] strArrays = new string[6];
						strArrays[0] = "SELECT *  FROM md_wizard_form ";
						strArrays[1] = DataUtils.NoLock();
						strArrays[2] = " JOIN md_wizard ";
						strArrays[3] = DataUtils.NoLock();
						strArrays[4] = " ON wiz_key=wzf_wiz_key WHERE wzf_key=";
						strArrays[5] = DataUtils.ValuePrep(base.szWizardStep, "av_key", true);
						str = string.Concat(strArrays);
					}
					else
					{
						string[] strArrays1 = new string[7];
						strArrays1[0] = "SELECT TOP 1 *  FROM md_wizard_form ";
						strArrays1[1] = DataUtils.NoLock();
						strArrays1[2] = " JOIN md_wizard ";
						strArrays1[3] = DataUtils.NoLock();
						strArrays1[4] = " ON wiz_key=wzf_wiz_key WHERE wzf_wiz_key=";
						strArrays1[5] = DataUtils.ValuePrep(base.szWizardKey, "av_key", true);
						strArrays1[6] = " ORDER BY wzf_order";
						str = string.Concat(strArrays1);
					}
					OleDbConnection connection = DataUtils.GetConnection();
					using (connection)
					{
						OleDbDataReader dataReader = DataUtils.GetDataReader(str, connection);
						using (dataReader)
						{
							if (dataReader != null && dataReader.Read())
							{
								flag8 = true;
								base.szWizardStep = dataReader["wzf_key"].ToString();
								string str1 = UtilityFunctions.RemoveMissingFields(dataReader["wiz_title"].ToString());
								if (str1.StartsWith("@"))
								{
									str1 = str1.Substring(1);
								}
								this.szWizardTitle = str1;
								this.szWizardIconFile = dataReader["wiz_icon_file"].ToString();
								COEBundleWizardClass_EWEB cOEBundleWizardClassEWEB = this;
								if (dataReader["wiz_show_progress"].ToString() == "1")
								{
									flag = true;
								}
								else
								{
									flag = false;
								}
								cOEBundleWizardClassEWEB.bWizardShowProgress = flag;
								this.szProgressObjectKey = dataReader["wiz_obj_key"].ToString();
								COEBundleWizardClass_EWEB cOEBundleWizardClassEWEB1 = this;
								if (dataReader["wiz_top_buttons"].ToString() == "1")
								{
									flag1 = true;
								}
								else
								{
									flag1 = false;
								}
								cOEBundleWizardClassEWEB1.bTopButtons = flag1;
								this.szTopCSSClass = dataReader["wiz_top_cssclass"].ToString();
								COEBundleWizardClass_EWEB cOEBundleWizardClassEWEB2 = this;
								if (dataReader["wiz_bottom_buttons"].ToString() == "1")
								{
									flag2 = true;
								}
								else
								{
									flag2 = false;
								}
								cOEBundleWizardClassEWEB2.bBottomButtons = flag2;
								this.szBottomCSSClass = dataReader["wiz_bottom_cssclass"].ToString();
								COEBundleWizardClass_EWEB cOEBundleWizardClassEWEB3 = this;
								if (dataReader["wiz_left_buttons"].ToString() == "1")
								{
									flag3 = true;
								}
								else
								{
									flag3 = false;
								}
								cOEBundleWizardClassEWEB3.bLeftButtons = flag3;
								this.szLeftCSSClass = dataReader["wiz_left_cssclass"].ToString();
								COEBundleWizardClass_EWEB cOEBundleWizardClassEWEB4 = this;
								if (dataReader["wiz_right_buttons"].ToString() == "1")
								{
									flag4 = true;
								}
								else
								{
									flag4 = false;
								}
								cOEBundleWizardClassEWEB4.bRightButtons = flag4;
								this.szRightCSSClass = dataReader["wiz_right_cssclass"].ToString();
								str1 = UtilityFunctions.RemoveMissingFields(dataReader["wzf_form_title"].ToString());
								if (str1.StartsWith("@"))
								{
									str1 = str1.Substring(1);
								}
								this.szWizardFormTitle = str1;
								str1 = UtilityFunctions.RemoveMissingFields(dataReader["wzf_form_header"].ToString());
								if (str1.StartsWith("@"))
								{
									str1 = str1.Substring(1);
								}
								this.szWizardFormHeader = str1;
								this.szWizardContentType = dataReader["wzf_content_type"].ToString();
								this.oPage.szFormKey = dataReader["wzf_dyn_key"].ToString();
								this.oPage.szFormKey = PageUtils.GetFormKey(this.oPage.szFormKey);
								this.szWizardStepChildFormKey = dataReader["wzf_dyc_key"].ToString();
								COEBundleWizardClass_EWEB cOEBundleWizardClassEWEB5 = this;
								if (dataReader["wzf_show_all_child"].ToString() == "1")
								{
									flag5 = true;
								}
								else
								{
									flag5 = false;
								}
								cOEBundleWizardClassEWEB5.bShowAllChildForms = flag5;
								this.szWizardControlFile = dataReader["wzf_control_file"].ToString();
								this.szWizardHTML = dataReader["wzf_html"].ToString();
								COEBundleWizardClass_EWEB cOEBundleWizardClassEWEB6 = this;
								if (dataReader["wiz_show_progress"].ToString() == "1")
								{
									flag6 = true;
								}
								else
								{
									flag6 = false;
								}
								cOEBundleWizardClassEWEB6.bShowProgress = flag6;
								COEBundleWizardClass_EWEB cOEBundleWizardClassEWEB7 = this;
								if (dataReader["wiz_show_progress_top"].ToString() == "1")
								{
									flag7 = true;
								}
								else
								{
									flag7 = false;
								}
								cOEBundleWizardClassEWEB7.bShowProgressTop = flag7;
							}
						}
						if (this.szProgressObjectKey != "")
						{
							str = string.Concat("SELECT obj_name  FROM md_object ", DataUtils.NoLock(), " WHERE obj_key=", DataUtils.ValuePrep(this.szProgressObjectKey, "av_key", true));
							OleDbDataReader oleDbDataReader = DataUtils.GetDataReader(str, connection);
							using (oleDbDataReader)
							{
								if (oleDbDataReader != null)
								{
									if (oleDbDataReader.Read())
									{
										this.szProgressObjectName = oleDbDataReader["obj_name"].ToString();
									}
									if (HttpContext.Current.Session[this.szProgressObjectName] == null)
									{
										this.oProgressFacadeObject = DataUtils.InstantiateFacadeObject(Config.Session, this.szProgressObjectName, null);
									}
									else
									{
										this.oProgressFacadeObject = (FacadeClass)HttpContext.Current.Session[this.szProgressObjectName];
										this.szProgressObjectCurrentKey = this.oProgressFacadeObject.CurrentKey;
									}
								}
							}
						}
						if ((this.bShowProgress || this.bShowProgressTop) && this.szProgressObjectKey != "" && !UtilityFunctions.EmptyString(this.szProgressObjectCurrentKey))
						{
							string[] strArrays2 = new string[6];
							strArrays2[0] = "SELECT wzp_step_key  FROM md_wizard_progress ";
							strArrays2[1] = DataUtils.NoLock();
							strArrays2[2] = " WHERE wzp_wiz_key = ";
							strArrays2[3] = DataUtils.ValuePrep(base.szWizardKey, "av_key", true);
							strArrays2[4] = " AND wzp_object_currentkey=";
							strArrays2[5] = DataUtils.ValuePrep(this.szProgressObjectCurrentKey, "av_key", true);
							str = string.Concat(strArrays2);
							OleDbDataReader dataReader1 = DataUtils.GetDataReader(str, connection);
							using (dataReader1)
							{
								if (dataReader1 != null)
								{
									while (dataReader1.Read())
									{
										if (this.hCompletedSteps == null)
										{
											this.hCompletedSteps = new Hashtable();
										}
										this.hCompletedSteps.Add(dataReader1["wzp_step_key"].ToString(), true);
									}
								}
							}
						}
						this.oWizardButtons = new WizardButtonCollectionClass();
						if (!UtilityFunctions.EmptyString(base.szWizardStep))
						{
							WizardButtonClass wizardButtonClass = new WizardButtonClass();
							Guid guid = Guid.NewGuid();
							wizardButtonClass.szKey = guid.ToString();
							wizardButtonClass.szAction = "None";
							wizardButtonClass.szDestinationURL = Config.eWebAppendSiteCodeToURL("DynamicPage.aspx?WebKey=7186e0fb-9857-45bb-945e-b68caca8799e");
							wizardButtonClass.szDestinationFormParameters = "";
							wizardButtonClass.szCaption = "Cancel";
							wizardButtonClass.szImageURL = "";
							wizardButtonClass.szToolTip = "Cancel...";
							wizardButtonClass.szCSSClass = "wizardbutton";
							wizardButtonClass.szVisibleControl = "";
							wizardButtonClass.szVisibleOperator = "";
							wizardButtonClass.szVisibleValue = "";
							wizardButtonClass.bExitWizard = false;
							this.oWizardButtons.Add(wizardButtonClass);
							wizardButtonClass = new WizardButtonClass();
							Guid guid1 = Guid.NewGuid();
							wizardButtonClass.szKey = guid1.ToString();
							wizardButtonClass.szAction = "Save";
							wizardButtonClass.szDestinationURL = "";
							wizardButtonClass.szDestinationFormParameters = "";
							wizardButtonClass.szCaption = "Next";
							wizardButtonClass.szImageURL = "";
							wizardButtonClass.szToolTip = "Proceed to next step...";
							wizardButtonClass.szCSSClass = "wizardbutton";
							wizardButtonClass.szVisibleControl = "";
							wizardButtonClass.szVisibleOperator = "";
							wizardButtonClass.szVisibleValue = "";
							wizardButtonClass.bExitWizard = false;
							this.oWizardButtons.Add(wizardButtonClass);
						}
					}
				}
			}
			catch (Exception exception1)
			{
				Exception exception = exception1;
				methodMonitor.Publish("Error", exception, false);
			}
		}
		finally
		{
			methodMonitor.End(this);
		}
		return flag8;
	}
}