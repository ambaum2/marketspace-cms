using Avectra.netForum.Common;
using Avectra.netForum.Components.AC;
using Avectra.netForum.Components.CO;
using Avectra.netForum.Components.OE;
using Avectra.netForum.Data;
using Avectra.netForum.Extension;
using System;
using System.Data;
using System.Data.OleDb;
using System.Reflection;
using System.Web;
using System.Web.SessionState;
using System.Web.UI;
using System.Web.UI.HtmlControls;
using System.Web.UI.WebControls;

namespace Avectra.netForum.eWeb.Classes
{
    public struct Functions
    {
        public static string CurrentUserName
        {
            get
            {
                if (HttpContext.Current == null || HttpContext.Current.Session == null)
                {
                    return "web:Anonymous";
                }
                else
                {
                    if (HttpContext.Current.Session["eWebCurrentUserName"] != null)
                    {
                        return HttpContext.Current.Session["eWebCurrentUserName"].ToString();
                    }
                    else
                    {
                        return "web:Anonymous";
                    }
                }
            }
            set
            {
                if (HttpContext.Current != null && HttpContext.Current.Session != null)
                {
                    HttpContext.Current.Session["eWebCurrentUserName"] = value;
                }
            }
        }

        public static Functions.BrowserType GetBrowserType
        {
            get
            {
                Functions.BrowserType browserType = Functions.BrowserType.IE;
                string lower = HttpContext.Current.Request.Browser.Browser.ToLower();
                if (lower != "netscape")
                {
                    if (HttpContext.Current.Request.Browser.Platform.ToLower().StartsWith("mac"))
                    {
                        browserType = Functions.BrowserType.IEMac;
                    }
                }
                else
                {
                    if (HttpContext.Current.Request.Browser.MajorVersion >= 5)
                    {
                        browserType = Functions.BrowserType.NS6;
                    }
                    else
                    {
                        browserType = Functions.BrowserType.NS4;
                    }
                }
                return browserType;
            }
        }

        public static void AddImageLink(HtmlTableCell oContentCell, string szImage, string szUrl, string szToolTip)
        {
            System.Web.UI.WebControls.Table table = new System.Web.UI.WebControls.Table();
            TableRow tableRow = new TableRow();
            TableCell tableCell = new TableCell();
            HyperLink hyperLink = new HyperLink();
            hyperLink.ImageUrl = szImage;
            hyperLink.NavigateUrl = Functions.AppendSiteCodeToURL(szUrl);
            hyperLink.ToolTip = szToolTip;
            tableCell.Controls.Add(hyperLink);
            tableRow.Cells.Add(tableCell);
            table.Rows.Add(tableRow);
            oContentCell.Controls.Add(table);
        }

        public static void AddImageLink(TableCell oContentCell, string szImage, string szUrl, string szToolTip)
        {
            System.Web.UI.WebControls.Table table = new System.Web.UI.WebControls.Table();
            TableRow tableRow = new TableRow();
            TableCell tableCell = new TableCell();
            HyperLink hyperLink = new HyperLink();
            hyperLink.ImageUrl = szImage;
            hyperLink.NavigateUrl = Functions.AppendSiteCodeToURL(szUrl);
            hyperLink.ToolTip = szToolTip;
            tableCell.Controls.Add(hyperLink);
            tableRow.Cells.Add(tableCell);
            table.Rows.Add(tableRow);
            oContentCell.Controls.Add(table);
        }

        public static void AddImageLink(HtmlTable oImageTable, HtmlTableRow oRow, HtmlTableCell oContentCell, string szImage, string szUrl, string szToolTip)
        {
            HtmlTableCell htmlTableCell = new HtmlTableCell();
            HyperLink hyperLink = new HyperLink();
            hyperLink.ImageUrl = szImage;
            hyperLink.NavigateUrl = Functions.AppendSiteCodeToURL(szUrl);
            hyperLink.ToolTip = szToolTip;
            htmlTableCell.Controls.Add(hyperLink);
            oRow.Cells.Add(htmlTableCell);
            oImageTable.Rows.Add(oRow);
            oContentCell.Controls.Add(oImageTable);
        }

        public static void AddLink(HtmlTableCell oCell, string szUrl, string szText, string szImage, string szToolTip)
        {
            Functions.AddLink(oCell, szUrl, szText, szImage, szToolTip, "_self");
        }

        public static void AddLink(HtmlTableCell oTableCell, string szUrl, string szText, string szImage, string szToolTip, string szTarget)
        {
            HtmlTableCell htmlTableCell;
            HtmlTable htmlTable = new HtmlTable();
            HtmlTableRow htmlTableRow = new HtmlTableRow();
            htmlTable.Controls.Add(htmlTableRow);
            if (szImage != "")
            {
                htmlTableCell = new HtmlTableCell();
                htmlTableRow.Controls.Add(htmlTableCell);
                Image image = new Image();
                image.ImageUrl = szImage;
                image.AlternateText = szToolTip;
                image.ImageAlign = ImageAlign.Middle;
                htmlTableCell.Controls.Add(image);
            }
            htmlTableCell = new HtmlTableCell();
            htmlTableRow.Controls.Add(htmlTableCell);
            HyperLink hyperLink = new HyperLink();
            hyperLink.Text = szText;
            hyperLink.ToolTip = szToolTip;
            hyperLink.NavigateUrl = Functions.AppendSiteCodeToURL(szUrl);
            hyperLink.CssClass = "bodyTXT";
            hyperLink.Target = szTarget;
            htmlTableCell.Controls.Add(hyperLink);
            htmlTableCell.Align = "left";
            oTableCell.Controls.Add(htmlTable);
        }

        public static void AddLink(System.Web.UI.WebControls.Table oTable, TableRow oRow, HtmlTableCell oTableCell, string szUrl, string szText, string szImage, string szToolTip, string szTarget)
        {
            //TableCell tableCell = new TableCell();
            System.Web.UI.WebControls.TableCell tableCell = new System.Web.UI.WebControls.TableCell();
            oTable.Controls.Add(oRow);
            if (szImage != "")
            {
                tableCell = new TableCell();
                oRow.Controls.Add(tableCell);
                Image image = new Image();
                image.ImageUrl = szImage;
                image.AlternateText = szToolTip;
                image.ImageAlign = ImageAlign.Middle;
                tableCell.Controls.Add(image);
            }
            tableCell = new TableCell();
            oRow.Controls.Add(tableCell);
            HyperLink hyperLink = new HyperLink();
            hyperLink.Text = szText;
            hyperLink.ToolTip = szToolTip;
            hyperLink.NavigateUrl = Functions.AppendSiteCodeToURL(szUrl);
            hyperLink.CssClass = "bodyTXT";
            hyperLink.Target = szTarget;
            tableCell.Controls.Add(hyperLink);
            oTableCell.Controls.Add(oTable);
        }

        public static void AddLink(HtmlTable oLinkTable, string szUrl, string szText, string szImage, string szToolTip)
        {
            Functions.AddLink(oLinkTable, szUrl, szText, szImage, szToolTip, "_self");
        }

        public static void AddLink(HtmlTable oLinkTable, string szUrl, string szText, string szImage, string szToolTip, string szTarget)
        {
            HtmlTableCell htmlTableCell;
            HtmlTableRow htmlTableRow = new HtmlTableRow();
            oLinkTable.Controls.Add(htmlTableRow);
            if (szImage != "")
            {
                htmlTableCell = new HtmlTableCell();
                htmlTableRow.Controls.Add(htmlTableCell);
                Image image = new Image();
                image.ImageUrl = szImage;
                image.AlternateText = szToolTip;
                image.ImageAlign = ImageAlign.Middle;
                htmlTableCell.Controls.Add(image);
            }
            htmlTableCell = new HtmlTableCell();
            htmlTableRow.Controls.Add(htmlTableCell);
            HyperLink hyperLink = new HyperLink();
            hyperLink.Text = szText;
            hyperLink.ToolTip = szToolTip;
            hyperLink.NavigateUrl = Functions.AppendSiteCodeToURL(szUrl);
            hyperLink.CssClass = "bodyTXT";
            hyperLink.Target = szTarget;
            htmlTableCell.Controls.Add(hyperLink);
            htmlTableCell.Align = "left";
        }

        public static string AppendSiteCodeToURL(string szURL)
        {
            return Config.eWebAppendSiteCodeToURL(szURL);
        }

        public static void BuildFrameWorkTable(Page oPage, int nTableBorder)
        {
            
            HtmlGenericControl htmlGenericControl = (HtmlGenericControl)oPage.FindControl("BodyTag");
            HtmlGenericControl wrapper = new HtmlGenericControl("DIV");
            wrapper.InnerHtml = Functions.SaforianHeaderMenu("AAOHeader");
            String wrapperText = wrapper.InnerHtml;

            HtmlGenericControl logindiv = new HtmlGenericControl("DIV");
            if (Config.Session["CustomerKey"] != null && !UtilityFunctions.IsWebUserKey(Config.Session["CustomerKey"].ToString()))
            {
                //find start and end of login html area "My AAO"
                int startNav = wrapperText.IndexOf("<nav class=\"navbar pull-right\">");
                int endNav = wrapperText.IndexOf("</nav>", startNav);
                //remove current MyAAO then replace with new MyAAO that has firstname 
                wrapperText = wrapperText.Remove(startNav, (endNav - startNav));
                String loginHtml = Functions.AAOGetLoginHtml(Config.Session["CustomerKey"].ToString());
                wrapperText = wrapperText.Insert(startNav, loginHtml);
                wrapper.InnerHtml = wrapperText.ToString();     
            }
            
            htmlGenericControl.Controls.Add(wrapper);
            wrapper.Controls.Add(logindiv);

            //if this is a my aao page then display bread crums
            if (Functions.AAOIsMyAAOPage(oPage))
            {
                HtmlGenericControl breadcrumb = new HtmlGenericControl("DIV");
                breadcrumb.InnerHtml = Functions.SaforianHeaderMenu("AAOBreadcrumb");
                htmlGenericControl.Controls.Add(breadcrumb);
            }
            if (htmlGenericControl != null)
            {
                bool item = oPage.Request["Modal"] == null;
                HtmlForm htmlForm = new HtmlForm();
                htmlForm.ID = "eWebForm";
                htmlForm.Method = "post";
                //htmlGenericControl.Controls.Add(htmlForm);
                htmlGenericControl.Controls.Add(htmlForm);
                HtmlTable htmlTable = new HtmlTable();
                htmlTable.ID = "PageFrameWorkTable";
                htmlTable.Border = nTableBorder;
                htmlTable.CellPadding = 0;
                htmlTable.CellSpacing = 0;
                if (nTableBorder > 0)
                {
                    htmlTable.BorderColor = "#CCCCCC";
                }
                htmlForm.Controls.Add(htmlTable);
                HtmlTableRow htmlTableRow = new HtmlTableRow();
                htmlTableRow.ID = "eWebFrameWorkTopRow";
                htmlTable.Rows.Add(htmlTableRow);
                HtmlTableCell htmlTableCell = new HtmlTableCell();
                htmlTableCell.ID = "eWebTopPaneTableCell";
                htmlTableCell.ColSpan = 3;
                if (item)
                {
                    htmlTableCell.Width = "100%";
                }
                htmlTableRow.Cells.Add(htmlTableCell);
                HtmlTable htmlTable1 = new HtmlTable();
                htmlTable1.ID = "eWebTopPaneTable";
                htmlTable1.Border = nTableBorder;
                htmlTable1.CellPadding = 0;
                htmlTable1.CellSpacing = 0;
                if (nTableBorder > 0)
                {
                    htmlTable1.BorderColor = "#CCCCCC";
                }
                if (item)
                {
                    htmlTable1.Width = "100%";
                }
                htmlTableCell.Controls.Add(htmlTable1);
                htmlTableRow = new HtmlTableRow();
                htmlTableRow.ID = "eWebTopPaneTableRowTop";
                htmlTable1.Rows.Add(htmlTableRow);
                htmlTableCell = new HtmlTableCell();
                htmlTableCell.ID = "eWebTopPaneTopLinksCell";
                htmlTableCell.VAlign = "top";
                htmlTableCell.ColSpan = 3;
                htmlTableRow.Controls.Add(htmlTableCell);
                htmlTableRow = new HtmlTableRow();
                htmlTableRow.ID = "eWebTopPaneTableRowMiddle";
                htmlTable1.Rows.Add(htmlTableRow);
                htmlTableCell = new HtmlTableCell();
                htmlTableCell.ID = "eWebTopPaneLeftLinksCell";
                htmlTableCell.VAlign = "top";
                htmlTableRow.Controls.Add(htmlTableCell);
                htmlTableCell = new HtmlTableCell();
                htmlTableCell.ID = "TopPane";
                htmlTableRow.Controls.Add(htmlTableCell);
                htmlTableCell = new HtmlTableCell();
                htmlTableCell.ID = "eWebTopPaneRightLinksCell";
                htmlTableCell.VAlign = "top";
                htmlTableRow.Controls.Add(htmlTableCell);
                htmlTableRow = new HtmlTableRow();
                htmlTableRow.ID = "eWebTopPaneTableRowBottom";
                htmlTable1.Rows.Add(htmlTableRow);
                htmlTableCell = new HtmlTableCell();
                htmlTableCell.ID = "eWebTopPaneBottomLinksCell";
                htmlTableCell.VAlign = "top";
                htmlTableCell.ColSpan = 3;
                htmlTableRow.Controls.Add(htmlTableCell);
                HtmlTableRow htmlTableRow1 = new HtmlTableRow();
                htmlTableRow1.ID = "eWebFrameWorkMiddleRow";
                htmlTable.Rows.Add(htmlTableRow1);
                htmlTableCell = new HtmlTableCell();
                htmlTableCell.ID = "eWebLeftPaneTableCell";
                htmlTableCell.VAlign = "top";
                htmlTableCell.Height = "100%";
                htmlTableRow1.Cells.Add(htmlTableCell);
                htmlTable1 = new HtmlTable();
                htmlTable1.ID = "eWebLeftPaneTable";
                htmlTable1.Border = nTableBorder;
                htmlTable1.CellPadding = 0;
                htmlTable1.CellSpacing = 0;
                if (nTableBorder > 0)
                {
                    htmlTable1.BorderColor = "#CCCCCC";
                }
                htmlTableCell.Controls.Add(htmlTable1);
                htmlTableRow = new HtmlTableRow();
                htmlTableRow.ID = "eWebLeftPaneTableRowTop";
                htmlTable1.Rows.Add(htmlTableRow);
                htmlTableCell = new HtmlTableCell();
                htmlTableCell.ID = "eWebLeftPaneTopLinksCell";
                htmlTableCell.VAlign = "top";
                htmlTableCell.ColSpan = 3;
                htmlTableRow.Controls.Add(htmlTableCell);
                htmlTableRow = new HtmlTableRow();
                htmlTableRow.ID = "eWebLeftPaneTableRowMiddle";
                htmlTable1.Rows.Add(htmlTableRow);
                htmlTableCell = new HtmlTableCell();
                htmlTableCell.ID = "eWebLeftPaneLeftLinksCell";
                htmlTableCell.VAlign = "top";
                htmlTableRow.Controls.Add(htmlTableCell);
                htmlTableCell = new HtmlTableCell();
                htmlTableCell.ID = "LeftPane";
                htmlTableRow.Controls.Add(htmlTableCell);
                htmlTableCell = new HtmlTableCell();
                htmlTableCell.ID = "eWebLeftPaneRightLinksCell";
                htmlTableCell.VAlign = "top";
                htmlTableRow.Controls.Add(htmlTableCell);
                htmlTableRow = new HtmlTableRow();
                htmlTableRow.ID = "eWebLeftPaneTableRowBottom";
                htmlTable1.Rows.Add(htmlTableRow);
                htmlTableCell = new HtmlTableCell();
                htmlTableCell.ID = "eWebLeftPaneBottomLinksCell";
                htmlTableCell.VAlign = "top";
                htmlTableCell.ColSpan = 3;
                htmlTableRow.Controls.Add(htmlTableCell);
                htmlTableCell = new HtmlTableCell();
                htmlTableCell.ID = "eWebContentPaneTableCell";
                htmlTableCell.VAlign = "top";
                htmlTableRow1.Cells.Add(htmlTableCell);
                htmlTable1 = new HtmlTable();
                htmlTable1.ID = "eWebContentPaneTable";
                if (item)
                {
                    htmlTable1.Width = "100%";
                }
                htmlTable1.Border = nTableBorder;
                htmlTable1.CellPadding = 0;
                htmlTable1.CellSpacing = 0;
                if (nTableBorder > 0)
                {
                    htmlTable1.BorderColor = "#CCCCCC";
                }
                htmlTableCell.Controls.Add(htmlTable1);
                htmlTableRow = new HtmlTableRow();
                htmlTableRow.ID = "eWebContentPaneTableRowTop";
                htmlTable1.Rows.Add(htmlTableRow);
                htmlTableCell = new HtmlTableCell();
                htmlTableCell.ID = "eWebContentPaneTopLinksCell";
                htmlTableCell.VAlign = "top";
                htmlTableCell.ColSpan = 3;
                htmlTableRow.Controls.Add(htmlTableCell);
                htmlTableRow = new HtmlTableRow();
                htmlTableRow.ID = "eWebContentPaneTableRowMiddle";
                htmlTable1.Rows.Add(htmlTableRow);
                htmlTableCell = new HtmlTableCell();
                htmlTableCell.ID = "eWebContentPaneLeftLinksCell";
                htmlTableCell.VAlign = "top";
                htmlTableRow.Controls.Add(htmlTableCell);
                htmlTableCell = new HtmlTableCell();
                htmlTableCell.VAlign = "top";
                htmlTableCell.ID = "ContentPane";
                htmlTableRow.Controls.Add(htmlTableCell);
                htmlTableCell = new HtmlTableCell();
                htmlTableCell.ID = "eWebContentPaneRightLinksCell";
                htmlTableCell.VAlign = "top";
                htmlTableRow.Controls.Add(htmlTableCell);
                htmlTableRow = new HtmlTableRow();
                htmlTableRow.ID = "eWebContentPaneTableRowBottom";
                htmlTable1.Rows.Add(htmlTableRow);
                htmlTableCell = new HtmlTableCell();
                htmlTableCell.ID = "eWebContentPaneBottomLinksCell";
                htmlTableCell.VAlign = "top";
                htmlTableCell.ColSpan = 3;
                htmlTableRow.Controls.Add(htmlTableCell);
                htmlTableCell = new HtmlTableCell();
                htmlTableCell.ID = "eWebRightPaneTableCell";
                htmlTableCell.VAlign = "top";
                htmlTableRow1.Cells.Add(htmlTableCell);
                htmlTable1 = new HtmlTable();
                htmlTable1.ID = "eWebRightPaneTable";
                htmlTable1.Border = nTableBorder;
                htmlTable1.CellPadding = 0;
                htmlTable1.CellSpacing = 0;
                if (nTableBorder > 0)
                {
                    htmlTable1.BorderColor = "#CCCCCC";
                }
                htmlTableCell.Controls.Add(htmlTable1);
                htmlTableRow = new HtmlTableRow();
                htmlTableRow.ID = "eWebRightPaneTableRowTop";
                htmlTable1.Rows.Add(htmlTableRow);
                htmlTableCell = new HtmlTableCell();
                htmlTableCell.ID = "eWebRightPaneTopLinksCell";
                htmlTableCell.VAlign = "top";
                htmlTableCell.ColSpan = 3;
                htmlTableRow.Controls.Add(htmlTableCell);
                htmlTableRow = new HtmlTableRow();
                htmlTableRow.ID = "eWebRightPaneTableRowMiddle";
                htmlTable1.Rows.Add(htmlTableRow);
                htmlTableCell = new HtmlTableCell();
                htmlTableCell.ID = "eWebRightPaneLeftLinksCell";
                htmlTableCell.VAlign = "top";
                htmlTableRow.Controls.Add(htmlTableCell);
                htmlTableCell = new HtmlTableCell();
                htmlTableCell.ID = "RightPane";
                htmlTableRow.Controls.Add(htmlTableCell);
                htmlTableCell = new HtmlTableCell();
                htmlTableCell.ID = "eWebRightPaneRightLinksCell";
                htmlTableCell.VAlign = "top";
                htmlTableRow.Controls.Add(htmlTableCell);
                htmlTableRow = new HtmlTableRow();
                htmlTableRow.ID = "eWebRightPaneTableRowBottom";
                htmlTable1.Rows.Add(htmlTableRow);
                htmlTableCell = new HtmlTableCell();
                htmlTableCell.ID = "eWebRightPaneBottomLinksCell";
                htmlTableCell.VAlign = "top";
                htmlTableCell.ColSpan = 3;
                htmlTableRow.Controls.Add(htmlTableCell);
                htmlTableRow = new HtmlTableRow();
                htmlTableRow.ID = "eWebFrameWorkBottomRow";
                htmlTable.Rows.Add(htmlTableRow);
                htmlTableCell = new HtmlTableCell();
                htmlTableCell.ID = "eWebBottomPaneTableCell";
                htmlTableCell.ColSpan = 3;
                if (item)
                {
                    htmlTableCell.Width = "100%";
                }
                htmlTableRow.Cells.Add(htmlTableCell);
                htmlTable1 = new HtmlTable();
                htmlTable1.ID = "eWebBottomPaneTable";
                if (item)
                {
                    htmlTable1.Width = "100%";
                }
                htmlTable1.Border = nTableBorder;
                htmlTable1.CellPadding = 0;
                htmlTable1.CellSpacing = 0;
                if (nTableBorder > 0)
                {
                    htmlTable1.BorderColor = "#CCCCCC";
                }
                htmlTableCell.Controls.Add(htmlTable1);
                htmlTableRow = new HtmlTableRow();
                htmlTableRow.ID = "eWebBottomPaneTableRowTop";
                htmlTable1.Rows.Add(htmlTableRow);
                htmlTableCell = new HtmlTableCell();
                htmlTableCell.ID = "eWebBottomPaneTopLinksCell";
                htmlTableCell.VAlign = "top";
                htmlTableCell.ColSpan = 3;
                htmlTableRow.Controls.Add(htmlTableCell);
                htmlTableRow = new HtmlTableRow();
                htmlTableRow.ID = "eWebBottomPaneTableRowMiddle";
                htmlTable1.Rows.Add(htmlTableRow);
                htmlTableCell = new HtmlTableCell();
                htmlTableCell.ID = "eWebBottomPaneLeftLinksCell";
                htmlTableCell.VAlign = "top";
                htmlTableRow.Controls.Add(htmlTableCell);
                htmlTableCell = new HtmlTableCell();
                htmlTableCell.ID = "BottomPane";
                htmlTableRow.Controls.Add(htmlTableCell);
                htmlTableCell = new HtmlTableCell();
                htmlTableCell.ID = "eWebBottomPaneRightLinksCell";
                htmlTableCell.VAlign = "top";
                htmlTableRow.Controls.Add(htmlTableCell);
                htmlTableRow = new HtmlTableRow();
                htmlTableRow.ID = "eWebBottomPaneTableRowBottom";
                htmlTable1.Rows.Add(htmlTableRow);
                htmlTableCell = new HtmlTableCell();
                htmlTableCell.ID = "eWebBottomPaneBottomLinksCell";
                htmlTableCell.VAlign = "top";
                htmlTableCell.ColSpan = 3;
                htmlTableRow.Controls.Add(htmlTableCell);
                HtmlGenericControl footer = new HtmlGenericControl("DIV");
                footer.InnerHtml = Functions.SaforianHeaderMenu("AAOFooter") 
                    + "<div class='available_session_vars'>" + Functions.AAOGetSessionVariables() + "</div>"
                    + "<div class='available_page_vars'>" + Functions.AAOGetPageVariables(oPage) + "</div>";
                htmlGenericControl.Controls.Add(footer);
                }
        }
        /* 
         * get login block of html for logged in 
         * user. cst_key is guid. May want to have a page of html
         * content where login text is stored instead of hardcoding here
         * @TODO probably should do logged in check here as well
         */
        public static string AAOGetLoginHtml(String cst_key)
        {
            String name_str = Functions.AAOGetName(cst_key);
            String loginText = "<nav class=\"navbar pull-right\"><ul class=\"nav logged-in\"><li class=\"myaao\"><a href=\"http://dev.aaomembers.org/user\">My AAO</a></li><li class=\"divider-vertical\"></li><li class=\"user\"> <span class=\"firstname\">" + name_str + "</span></li><li class=\"signout\"><a href=\"http://dev.aaomembers.org/user/logout\">Sign Out</a></li></ul></nav>";
            return loginText;
        }
        /*
         * return first name by cst_key
         * cst_key guid customer id
         * TODO add more variables for fullname lastname only etc
         */
        public static string AAOGetName(String cst_key)
        {
            string[] strArrays = new string[6];
            strArrays[0] = "SELECT ind_first_name,ind_cst_key FROM co_individual ";
            strArrays[3] = DataUtils.NoLock();
            strArrays[4] = " WHERE ind_cst_key = '" + cst_key.ToString() + "'";
            string name = string.Concat(strArrays);
            string name_str = "";
            OleDbConnection connection = DataUtils.GetConnection();
            using (connection)
            {
                OleDbDataReader dataReader = DataUtils.GetDataReader(name, connection);
                using (dataReader)
                {
                    if (dataReader != null && dataReader.Read())
                    {
                        //string[] strArrays1 = new string[6];
                        name_str = dataReader["ind_first_name"].ToString();
                        return name_str;
                    }
                }
            }
            return "Name Not Found";
        }
        public static string AAOGetPageVariables(Page oPage)
        {
            string sessionVars = "<p>All Page Variables</p>";
            HttpRequest url_variables = oPage.Request;
            for(int i=0; i< url_variables.Params.Count;i++) 
            {
                sessionVars += "<p>Key: " + url_variables.Params.Keys[i] + " Value: " + url_variables.Params[i] + "</p>";
            }
            //oPage.Request.QueryString["WebKey"];
            return sessionVars;
        }
        public static string AAOGetSessionVariables()
        {
            string str;
            string sessionVars = "<p>All Session Variables</p>";
            HttpSessionState session = HttpContext.Current.Session;
            for (int i = 0; i < session.Contents.Count; i++)
            {
                sessionVars += "<p>Key: " + session.Keys[i] + " Value: " + session[i] + "</p>";
            }
            try
            {
                str = session["eWebCurrentUserFriendlyName"].ToString();//{CurrentUserFriendlyName}
            }
            catch
            {
                str = Config.CurrentUserName;
            }
            if (UtilityFunctions.EmptyString(str))
            {
                try
                {
                    str = UtilityFunctions.GetUserName(HttpContext.Current.User.Identity.Name);
                }
                catch
                {
                    str = "web:Anonymous";
                }
            }
            if (UtilityFunctions.EmptyString(str))
            {
                str = "web:Anonymous";
            }
            return sessionVars;
        }

        public static bool AAOIsMyAAOPage(Page oPage)
        {
            String[] siteLinkOnlyPages = { "7811c961-946f-4eda-afa5-3696cc8bc403", "CEManager", "b649f8d3-c5ad-4bae-a536-4e51524edfb4", 
                                             "MyInformation", "dee08ecf-1b1b-43ac-bed0-c529ffc1dd77", "MyCommitteeInfo", 
                                             "a51d2747-4eda-4cd7-be73-ead835b8f06e", "MT"};
            String thisWebKeyOrCode = "";
            if (oPage.Request.Params["WebKey"] != null)
            {
                thisWebKeyOrCode = oPage.Request.Params["WebKey"].ToString();
            }
            else if (oPage.Request.Params["WebCode"] != null)
            {
                thisWebKeyOrCode = oPage.Request.Params["WebCode"].ToString();
            }

            if (Array.Exists(siteLinkOnlyPages, str => str.Contains(thisWebKeyOrCode)) && thisWebKeyOrCode != "")
            { //if the current page matches one of the 
                return true;
            }
            else
            {
                return false;
            }
        }
        public static string SaforianHeaderMenu(string position)
        {
            string content_html = Functions.getWebContentDetailByKey(position);
            return content_html;

        }

        /* check if given webcode/key 
         * belongs to a valid section
         * sections comma seperated list of sections
         */
        public static bool AAOCheckDisplayMyAAOMenuByWebKeyOrCode(String WebKey, String WebCode)
        {
            bool runQuery = false;
            string[] strArrays = new string[6];
            strArrays[0] = "SELECT wbc_code FROM md_web_content ";
            strArrays[1] = DataUtils.NoLock();
            strArrays[2] = " LEFT JOIN md_web_section ";
            strArrays[3] = DataUtils.NoLock();
            if(WebKey != "") {
                strArrays[4] = " ON wbs_key = wbc_wbs_key WHERE (wbs_key IN ("
                + "'3bdfcd8a-0932-455c-9946-60f76b0a49d8','0e7700f1-832a-46c2-8229-a2ee561af58f',"
                + "'765b7ccd-89ca-4561-85e3-dc2d64c83068', 'ba5e6610-544e-427e-aa30-f2bf9be950b9')) AND wbc_key = ";
                strArrays[5] = DataUtils.ValuePrep(WebKey, "av_key", true);
                
                runQuery = true;
            }
            if (WebCode != "" && !(WebKey != "")) //the query is finicky we can't have a null webcode and webkey can't exist
            {
               strArrays[4] = " ON wbs_key = wbc_wbs_key WHERE (wbs_key IN ("
                + "'3bdfcd8a-0932-455c-9946-60f76b0a49d8','0e7700f1-832a-46c2-8229-a2ee561af58f',"
                + "'765b7ccd-89ca-4561-85e3-dc2d64c83068', 'ba5e6610-544e-427e-aa30-f2bf9be950b9'))";
               strArrays[5] = " AND wbc_code ='" + WebCode + "'";
               runQuery = true;
            }

            if (runQuery)
            {
                string str = string.Concat(strArrays);
                OleDbConnection connection = DataUtils.GetConnection();
                using (connection)
                {
                    OleDbDataReader dataReader = DataUtils.GetDataReader(str, connection);
                    if (dataReader.HasRows)
                    {
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
            }
            return false;
        }

        /* 
         * returns page detail html
         *  position is the page code of the page html you want
         */ 
        public static string getWebContentDetailByKey(String position)
        {
            string[] strArrays = new string[6];
            strArrays[0] = "SELECT wbd_html,wbd_position,wbd_content_type,wbc_code FROM md_web_content ";
            strArrays[1] = DataUtils.NoLock();
            strArrays[2] = " LEFT JOIN md_web_content_detail ";
            strArrays[3] = DataUtils.NoLock();
            strArrays[4] = " ON wbd_wbc_key = wbc_key WHERE wbc_code = '"
                + position.ToString() + "' AND wbd_position = 'Content'";
            //strArrays[5] = DataUtils.ValuePrep(key, "av_key", true);
            string str = string.Concat(strArrays);
            string str1 = "";
            OleDbConnection connection = DataUtils.GetConnection();
            using (connection)
            {
                OleDbDataReader dataReader = DataUtils.GetDataReader(str, connection);
                using (dataReader)
                {
                    if (dataReader != null && dataReader.Read())
                    {
                        str1 = dataReader["wbd_html"].ToString();
                    }
                }
            }
            return str1;
        }
        private static bool CellIsEmpty(HtmlTableCell oCell)
        {
            bool flag = true;
            if (oCell != null && (oCell.Controls.Count > 0 || oCell.Attributes["class"] != null || oCell.Width != "" || oCell.Height != "" || oCell.BgColor != "" || oCell.Align != ""))
            {
                flag = false;
            }
            return flag;
        }

        public static void CentralizedShoppingCart_Initialize()
        {
            OrderEntryExtension.AfterSaveReInitCOEObject(null);
        }

        public bool CheckCustomerLoginStatus(PageClass oPage, Control oControl, string szCustomerKey, string szLogin = "true")
        {
            bool flag = false;
            bool lower = szLogin.ToLower() == "true";
            if (UtilityFunctions.EmptyString(szCustomerKey) && oPage.oFacadeObject.GetCstKeyXRefField() != null)
            {
                szCustomerKey = oPage.oFacadeObject.GetValue(oPage.oFacadeObject.GetCstKeyXRefField());
            }
            if (Config.Session["CustomerKey"] != null && !UtilityFunctions.IsWebUserKey(Config.Session["CustomerKey"].ToString()))
            {
                flag = true;
            }
            if (!flag && (Config.Session["CustomerKey"] == null || UtilityFunctions.IsWebUserKey(Config.Session["CustomerKey"].ToString())) && (!UtilityFunctions.IsWebUserKey(szCustomerKey) || !UtilityFunctions.EmptyString(szCustomerKey)) && lower)
            {
                flag = Functions.LoginByCustomerKey(szCustomerKey);
            }
            return flag;
        }

        public static void CheckForLoginEntityKey(string szSiteEntityKey, HttpSessionState oSession)
        {
            string value;
            if (!UtilityFunctions.EmptyString(szSiteEntityKey) && oSession["CustomerKey"] != null)
            {
                if (oSession["Individual"] == null)
                {
                    if (oSession["Organization"] == null)
                    {
                        value = Functions.ReadEntityKeyFromCustomerKey(oSession["CustomerKey"].ToString(), oSession);
                    }
                    else
                    {
                        value = ((FacadeClass)oSession["Organization"]).GetValue("org_entity_key");
                    }
                }
                else
                {
                    value = ((FacadeClass)oSession["Individual"]).GetValue("ind_entity_key");
                }
                if (!UtilityFunctions.EmptyString(value) && value.ToLower() != szSiteEntityKey.ToLower())
                {
                    oSession["Individual"] = null;
                    oSession["Organization"] = null;
                    oSession["CustomerName"] = null;
                    oSession["CustomerKey"] = null;
                    oSession["OrgCustomerKey"] = null;
                    oSession["CustomerType"] = null;
                    oSession["CustomerEMail"] = null;
                    oSession["CustomerWebLogin"] = null;
                    oSession["ForcePasswordChange"] = null;
                    oSession["CustomerBillingAddressKey"] = null;
                    oSession["CustomerMailingAddressKey"] = null;
                }
            }
        }

        public static void CheckForWebEntityKey(string szSiteEntityKey, string szWebSiteCode)
        {
            if (!UtilityFunctions.EmptyString(szSiteEntityKey) && (Config.UserEntities == null || UtilityFunctions.EmptyString(Config.UserEntityKey) || Config.UserEntityKey.ToLower() != szSiteEntityKey.ToLower()))
            {
                Config.UserEntityKey = szSiteEntityKey;
                UserEntitiesClass userEntitiesClass = new UserEntitiesClass();
                userEntitiesClass.InitializeUserEntitiesWeb(szSiteEntityKey, szWebSiteCode);
                Config.UserEntities = userEntitiesClass;
                SystemUtils.LoadSystemOptionsForEntity();
            }
        }

        public static string GetContentURL(string szContentKey)
        {
            return Functions.GetContentURL(szContentKey, null);
        }

        public static string GetContentURL(string szContentKey, FacadeClass oFacadeObject)
        {
            string[] strArrays = new string[6];
            strArrays[0] = "SELECT web_code, wbc_page_name,wbc_parameters FROM md_web_content ";
            strArrays[1] = DataUtils.NoLock();
            strArrays[2] = " JOIN md_web ";
            strArrays[3] = DataUtils.NoLock();
            strArrays[4] = " ON web_key=wbc_web_key WHERE wbc_key=";
            strArrays[5] = DataUtils.ValuePrep(szContentKey, "av_key", true);
            string str = string.Concat(strArrays);
            string str1 = "";
            OleDbConnection connection = DataUtils.GetConnection();
            using (connection)
            {
                OleDbDataReader dataReader = DataUtils.GetDataReader(str, connection);
                using (dataReader)
                {
                    if (dataReader != null && dataReader.Read())
                    {
                        string[] strArrays1 = new string[6];
                        strArrays1[0] = "~/";
                        strArrays1[1] = dataReader["wbc_page_name"].ToString();
                        strArrays1[2] = "?Site=";
                        strArrays1[3] = dataReader["web_code"].ToString();
                        strArrays1[4] = "&WebKey=";
                        strArrays1[5] = szContentKey;
                        str1 = string.Concat(strArrays1);
                        if (dataReader["wbc_parameters"].ToString() != "")
                        {
                            if (oFacadeObject != null)
                            {
                                str1 = string.Concat(str1, "&", DataUtils.ParseValues(oFacadeObject, dataReader["wbc_parameters"].ToString(), false));
                            }
                            else
                            {
                                str1 = string.Concat(str1, "&", dataReader["wbc_parameters"].ToString());
                            }
                        }
                    }
                }
            }
            return str1;
        }

        public static string GetEntityKeyFromSiteCode(string szSiteCode)
        {
            string lower = "";
            string[] strArrays = new string[5];
            strArrays[0] = "SELECT web_entity_key FROM md_web ";
            strArrays[1] = DataUtils.NoLock();
            strArrays[2] = " WHERE web_code=";
            strArrays[3] = DataUtils.ValuePrep(szSiteCode, "nvarchar", true);
            strArrays[4] = " /*(NoEntity)*/";
            string str = string.Concat(strArrays);
            OleDbConnection connection = DataUtils.GetConnection();
            using (connection)
            {
                OleDbDataReader dataReader = DataUtils.GetDataReader(str, connection);
                using (dataReader)
                {
                    if (dataReader != null)
                    {
                        if (dataReader.Read())
                        {
                            lower = dataReader["web_entity_key"].ToString().ToLower();
                        }
                        dataReader.Close();
                    }
                }
            }
            return lower;
        }

        public static string GetUserName()
        {
            string str;
            HttpSessionState session = HttpContext.Current.Session;
            try
            {
                str = session["eWebCurrentUserName"].ToString();
            }
            catch
            {
                str = Config.CurrentUserName;
            }
            if (UtilityFunctions.EmptyString(str))
            {
                try
                {
                    str = UtilityFunctions.GetUserName(HttpContext.Current.User.Identity.Name);
                }
                catch
                {
                    str = "web:Anonymous";
                }
            }
            if (UtilityFunctions.EmptyString(str))
            {
                str = "web:Anonymous";
            }
            return str;
        }

        private static void KeepCellProperties(HtmlTableCell oCell, HtmlTableCell oCellTop)
        {
            if (oCellTop.Width != "")
            {
                oCell.Width = oCellTop.Width;
            }
            if (oCellTop.Height != "")
            {
                oCell.Height = oCellTop.Height;
            }
            if (oCellTop.Align != "")
            {
                oCell.Align = oCellTop.Align;
            }
            if (oCellTop.VAlign != "")
            {
                oCell.VAlign = oCellTop.VAlign;
            }
            if (oCellTop.BgColor != "")
            {
                oCell.BgColor = oCellTop.BgColor;
            }
            if (oCellTop.Attributes["class"] != null)
            {
                oCell.Attributes.Remove("class");
                oCell.Attributes.Add("class", oCellTop.Attributes["class"].ToString());
            }
        }

        public static bool Login(string szSQL)
        {
            bool flag;
            bool flag1;
            bool flag2 = false;
            bool flag3 = false;
            HttpContext.Current.Session["CustomerName"] = null;
            HttpContext.Current.Session["CustomerKey"] = null;
            HttpContext.Current.Session["OrgCustomerKey"] = null;
            HttpContext.Current.Session["CustomerType"] = null;
            HttpContext.Current.Session["CustomerEMail"] = null;
            HttpContext.Current.Session["CustomerWebLogin"] = null;
            HttpContext.Current.Session["ForcePasswordChange"] = null;
            HttpContext.Current.Session["CustomerBillingAddressKey"] = null;
            HttpContext.Current.Session["CustomerMailingAddressKey"] = null;
            OleDbConnection connection = DataUtils.GetConnection();
            using (connection)
            {
                OleDbDataReader dataReader = DataUtils.GetDataReader(szSQL, connection);
                using (dataReader)
                {
                    if (dataReader != null && dataReader.Read())
                    {
                        HttpContext.Current.Session["CustomerName"] = dataReader["cst_name_cp"].ToString();
                        HttpContext.Current.Session["CustomerKey"] = dataReader["cst_key"].ToString();
                        if (Config.Session["CentralizedOrderEntry"] != null)
                        {
                            ((Invoice)Config.Session["CentralizedOrderEntry"]).SetValue("inv_cst_key", dataReader["cst_key"].ToString());
                            try
                            {
                                if (Config.MC && !UtilityFunctions.IsGuid(((Invoice)Config.Session["CentralizedOrderEntry"]).GetValue("inv_cur_key")))
                                {
                                    ((Invoice)Config.Session["CentralizedOrderEntry"]).SetValue("inv_cur_key", dataReader["cst_cur_key"].ToString());
                                }
                            }
                            catch
                            {
                            }
                        }
                        HttpContext.Current.Session["OrgCustomerKey"] = dataReader["ixo_org_cst_key"].ToString();
                        HttpContext.Current.Session["CustomerType"] = dataReader["cst_type"].ToString();
                        HttpContext.Current.Session["CustomerBillingAddressKey"] = dataReader["cst_cxa_billing_key"].ToString();
                        HttpContext.Current.Session["CustomerMailingAddressKey"] = dataReader["cst_cxa_key"].ToString();
                        HttpContext.Current.Session["CustomerEMail"] = dataReader["cst_eml_address_dn"].ToString();
                        HttpContext.Current.Session["CustomerWebLogin"] = dataReader["cst_web_login"].ToString();
                        HttpSessionState session = HttpContext.Current.Session;
                        string str = "ForcePasswordChange";
                        if (dataReader["cst_web_force_password_change"].ToString() == "1")
                        {
                            flag = true;
                        }
                        else
                        {
                            flag = false;
                        }
                        session[str] = flag;
                        if (dataReader["cst_web_forgot_password_status"].ToString() == "1")
                        {
                            flag1 = true;
                        }
                        else
                        {
                            flag1 = false;
                        }
                        flag3 = flag1;
                        if (Config.GetSystemOption("useEmailForAuthorization") == null || Config.GetSystemOption("useEmailForAuthorization").ToString().ToLower() == "true")
                        {
                            HttpContext.Current.Session["eWebCurrentUserName"] = string.Concat("web:", HttpContext.Current.Session["CustomerEMail"].ToString().Trim());
                        }
                        else
                        {
                            HttpContext.Current.Session["eWebCurrentUserName"] = string.Concat("web:", HttpContext.Current.Session["CustomerWebLogin"].ToString().Trim());
                        }
                        Functions.CurrentUserName = HttpContext.Current.Session["eWebCurrentUserName"].ToString();
                        HttpContext.Current.Session["UserName"] = HttpContext.Current.Session["eWebCurrentUserName"].ToString().Trim();
                        flag2 = true;
                    }
                    if (dataReader != null)
                    {
                        dataReader.Close();
                    }
                }
                if (flag2)
                {
                    if (HttpContext.Current.Session["CustomerKey"] != null)
                    {
                        FacadeClass facadeClass = DataUtils.InstantiateFacadeObject(HttpContext.Current.Session, "f41b6e06-299b-4022-be6f-0641ba87de59");
                        facadeClass.CurrentKey = HttpContext.Current.Session["CustomerKey"].ToString();
                        facadeClass.ObjectDescription = "Individual";
                        facadeClass.SelectByKey(connection, null);
                        HttpContext.Current.Session["Individual"] = facadeClass;
                    }
                    if (HttpContext.Current.Session["OrgCustomerKey"] != null)
                    {
                        FacadeClass str1 = DataUtils.InstantiateFacadeObject(HttpContext.Current.Session, "9c0100ce-f6c2-4b42-aff2-1c065f3734d9");
                        str1.CurrentKey = HttpContext.Current.Session["OrgCustomerKey"].ToString();
                        str1.ObjectDescription = "Organization";
                        str1.SelectByKey(connection, null);
                        HttpContext.Current.Session["Organization"] = str1;
                    }
                }
                if (flag3 && HttpContext.Current.Session["CustomerKey"] != null)
                {
                    szSQL = string.Concat("update co_customer set cst_web_forgot_password_status=0 where cst_key=", DataUtils.ValuePrep(HttpContext.Current.Session["CustomerKey"].ToString(), "av_key", true));
                    OleDbCommand oleDbCommand = new OleDbCommand(szSQL, connection);
                    oleDbCommand.ExecuteNonQuery();
                }
                if (flag2)
                {
                    Functions.RecalculateShoppingCartPrices(connection, null);
                    Functions.RecalculateCentralizedShoppingCartPrices(connection, null);
                }
                connection.Close();
            }
            if (flag2)
            {
                string systemOption = Config.GetSystemOption("ClientAfterLoginActions");
                if (!UtilityFunctions.EmptyString(systemOption))
                {
                    char[] chrArray = new char[1];
                    chrArray[0] = '|';
                    Array arrays = systemOption.Split(chrArray);
                    string str2 = arrays.GetValue(0).ToString().Trim();
                    string str3 = arrays.GetValue(1).ToString().Trim();
                    string str4 = "ExecAfterLoginJobs";
                    try
                    {
                        str4 = arrays.GetValue(2).ToString().Trim();
                    }
                    catch
                    {
                    }
                    ErrorClass errorClass = new ErrorClass();
                    try
                    {
                        Assembly assembly = Assembly.Load(str2);
                        Type type = assembly.GetType(str3);
                        MethodInfo method = type.GetMethod(str4);
                        object obj2 = Activator.CreateInstance(type);
                        errorClass = (ErrorClass)method.Invoke(obj2, null);
                    }
                    catch (Exception exception1)
                    {
                        Exception exception = exception1;
                        errorClass = new ErrorClass();
                        errorClass.Level = ErrorClass.ErrorLevel.Error;
                        string[] message = new string[18];
                        message[0] = "The '";
                        message[1] = str2;
                        message[2] = "' assembly failed to load.";
                        message[3] = UtilityFunctions.CRLF();
                        message[4] = "Please make sure that ";
                        message[5] = str2;
                        message[6] = ".dll has been deployed to the application '\\bin' directory.";
                        message[7] = UtilityFunctions.CRLF();
                        message[8] = "Also, please check that the 'ClientAfterLoginActions' system option is setup correctly.";
                        message[9] = UtilityFunctions.CRLF();
                        message[10] = "Finally, please make sure the '";
                        message[11] = str4;
                        message[12] = "' method is declared as 'public' in the ";
                        message[13] = str3;
                        message[14] = " class.";
                        message[15] = UtilityFunctions.CRLF();
                        message[16] = "The raw error message is:";
                        message[17] = exception.Message;
                        errorClass.Message = string.Concat(message);
                        errorClass.Number = 10;
                    }
                }
            }
            return flag2;
        }

        public static bool LoginByCustomerID(string szCustomerID)
        {
            string[] strArrays = new string[7];
            strArrays[0] = "SELECT cst_cur_key,cst_key, cst_web_login, cst_eml_address_dn, cst_web_force_password_change, cst_name_cp, cst_type, cst_cxa_key, cst_cxa_billing_key=CASE WHEN cst_cxa_billing_key is null THEN cst_cxa_key ELSE cst_cxa_billing_key END  ,ixo_org_cst_key , cst_web_forgot_password_status  FROM co_customer ";
            strArrays[1] = DataUtils.NoLock();
            strArrays[2] = " LEFT JOIN co_individual_x_organization ";
            strArrays[3] = DataUtils.NoLock();
            strArrays[4] = " ON cst_ixo_key = ixo_key  WHERE cst_id = ";
            strArrays[5] = DataUtils.ValuePrep(szCustomerID, "char", true);
            strArrays[6] = " and cst_delete_flag=0";
            string str = string.Concat(strArrays);
            return Functions.Login(str);
        }

        public static bool LoginByCustomerKey(string szCustomerKey)
        {
            string[] strArrays = new string[7];
            strArrays[0] = "SELECT cst_cur_key,cst_key, cst_web_login, cst_eml_address_dn, cst_web_force_password_change, cst_name_cp, cst_type, cst_cxa_key, cst_cxa_billing_key=CASE WHEN cst_cxa_billing_key is null THEN cst_cxa_key ELSE cst_cxa_billing_key END  ,ixo_org_cst_key , cst_web_forgot_password_status  FROM co_customer ";
            strArrays[1] = DataUtils.NoLock();
            strArrays[2] = " LEFT JOIN co_individual_x_organization ";
            strArrays[3] = DataUtils.NoLock();
            strArrays[4] = " ON cst_ixo_key = ixo_key  WHERE cst_key = ";
            strArrays[5] = DataUtils.ValuePrep(szCustomerKey, "av_key", true);
            strArrays[6] = " and cst_delete_flag=0";
            string str = string.Concat(strArrays);
            return Functions.Login(str);
        }

        public static bool LoginByCustomerWebLogin(string szWebLogin, string szPassword)
        {
            string[] strArrays = new string[6];
            strArrays[0] = "SELECT TOP 1 cst_cur_key,cst_key, cst_web_login, cst_eml_address_dn, cst_web_force_password_change, cst_name_cp, cst_type, cst_cxa_key, cst_cxa_billing_key=CASE WHEN cst_cxa_billing_key is null THEN cst_cxa_key ELSE cst_cxa_billing_key END  ,ixo_org_cst_key , cst_web_forgot_password_status  FROM co_customer ";
            strArrays[1] = DataUtils.NoLock();
            strArrays[2] = " LEFT JOIN co_individual_x_organization ";
            strArrays[3] = DataUtils.NoLock();
            strArrays[4] = " ON cst_ixo_key = ixo_key  WHERE cst_delete_flag=0 and cst_web_login = ";
            strArrays[5] = DataUtils.ValuePrep(szWebLogin, "nvarchar", true);
            string str = string.Concat(strArrays);
            str = string.Concat(str, " AND getdate() between isnull(convert(datetime,cst_web_start_date),convert(datetime,'1/1/1900')) and isnull(convert(datetime,cst_web_end_date),convert(datetime,'12/31/9999')) ");
            str = string.Concat(str, " AND isnull(cst_web_login_disabled_flag,0) = 0 ");
            if (szPassword != null)
            {
                if (Config.hashPassword)
                {
                    str = string.Concat(str, " AND cst_web_password = ", DataUtils.ValuePrep(UtilityFunctions.GetHash(szPassword), "nvarchar", true));
                }
                else
                {
                    str = string.Concat(str, " AND cst_web_password = ", DataUtils.ValuePrep(szPassword, "nvarchar", true));
                }
            }
            return Functions.Login(str);
        }

        public static bool LoginByEMail(string szEMailAddress, string szPassword)
        {
            string[] strArrays = new string[6];
            strArrays[0] = "SELECT TOP 1 cst_cur_key,cst_key, cst_web_login, cst_eml_address_dn, cst_web_force_password_change, cst_name_cp, cst_type, cst_cxa_key, cst_cxa_billing_key=CASE WHEN cst_cxa_billing_key is null THEN cst_cxa_key ELSE cst_cxa_billing_key END  ,ixo_org_cst_key , cst_web_forgot_password_status  FROM co_customer ";
            strArrays[1] = DataUtils.NoLock();
            strArrays[2] = " LEFT JOIN co_individual_x_organization ";
            strArrays[3] = DataUtils.NoLock();
            strArrays[4] = " ON cst_ixo_key = ixo_key  WHERE cst_delete_flag=0 and cst_eml_address_dn = ";
            strArrays[5] = DataUtils.ValuePrep(szEMailAddress, "av_email", true);
            string str = string.Concat(strArrays);
            str = string.Concat(str, " AND getdate() between isnull(convert(datetime,cst_web_start_date),convert(datetime,'1/1/1900')) and isnull(convert(datetime,cst_web_end_date),convert(datetime,'12/31/9999')) ");
            str = string.Concat(str, " AND isnull(cst_web_login_disabled_flag,0) = 0 ");
            if (szPassword != null)
            {
                if (Config.hashPassword)
                {
                    str = string.Concat(str, " AND cst_web_password = ", DataUtils.ValuePrep(UtilityFunctions.GetHash(szPassword), "nvarchar", true));
                }
                else
                {
                    str = string.Concat(str, " AND cst_web_password = ", DataUtils.ValuePrep(szPassword, "nvarchar", true));
                }
            }
            return Functions.Login(str);
        }

        public static bool LoginByToken(string szToken)
        {
            string[] strArrays = new string[9];
            strArrays[0] = "SELECT cst_cur_key,cst_key, cst_web_login, cst_eml_address_dn, cst_web_force_password_change, cst_name_cp, cst_type, cst_cxa_key, cst_cxa_billing_key=CASE WHEN cst_cxa_billing_key is null THEN cst_cxa_key ELSE cst_cxa_billing_key END, ixo_org_cst_key, cst_web_forgot_password_status from ws_security ";
            strArrays[1] = DataUtils.NoLock();
            strArrays[2] = " join co_customer ";
            strArrays[3] = DataUtils.NoLock();
            strArrays[4] = " on cst_key = xws_cst_key LEFT JOIN co_individual_x_organization ";
            strArrays[5] = DataUtils.NoLock();
            strArrays[6] = " ON cst_ixo_key = ixo_key where xws_current_token = ";
            strArrays[7] = DataUtils.ValuePrep(szToken, "nvarchar", true);
            strArrays[8] = " and getdate() <= xws_expiration and cst_delete_flag = 0 ";
            string str = string.Concat(strArrays);
            return Functions.Login(str);
        }

        public static void PaintEmptyLine(HtmlControl oContentCell)
        {
            System.Web.UI.WebControls.Table table = new System.Web.UI.WebControls.Table();
            oContentCell.Controls.Add(table);
            table.BorderWidth = 0;
            table.CellPadding = 0;
            table.CellSpacing = 0;
            table.Attributes.Add("width", "100%");
            TableRow tableRow = new TableRow();
            table.Controls.Add(tableRow);
            TableCell tableCell = new TableCell();
            tableRow.Controls.Add(tableCell);
            tableCell.CssClass = "bodyTXT";
            Label label = new Label();
            tableCell.Controls.Add(label);
            label.Text = "&nbsp;";
        }

        public static void PaintSectionBar(HtmlControl oContentCell, string szText)
        {
            System.Web.UI.WebControls.Table table = new System.Web.UI.WebControls.Table();
            oContentCell.Controls.Add(table);
            table.BorderWidth = 0;
            table.CellPadding = 1;
            table.CellSpacing = 0;
            table.Attributes.Add("width", "100%");
            TableRow tableRow = new TableRow();
            table.Controls.Add(tableRow);
            TableCell tableCell = new TableCell();
            tableRow.Controls.Add(tableCell);
            tableCell.CssClass = "SectionBar";
            Label label = new Label();
            tableCell.Controls.Add(label);
            label.Text = szText;
        }

        public static void PaintTitle(HtmlControl oContentCell, string szTitle, string szImage)
        {
            System.Web.UI.WebControls.Table table = new System.Web.UI.WebControls.Table();
            oContentCell.Controls.Add(table);
            table.BorderWidth = 0;
            table.CellPadding = 3;
            table.CellSpacing = 0;
            table.Attributes.Add("width", "100%");
            TableRow tableRow = new TableRow();
            table.Controls.Add(tableRow);
            TableCell tableCell = new TableCell();
            tableRow.Controls.Add(tableCell);
            tableCell.CssClass = "tabTXT";
            tableCell.VerticalAlign = VerticalAlign.Middle;
            tableCell.HorizontalAlign = HorizontalAlign.Left;
            if (!UtilityFunctions.EmptyString(szImage))
            {
                Image image = new Image();
                tableCell.Controls.Add(image);
                image.ImageUrl = szImage;
                image.BorderWidth = 0;
            }
            Label label = new Label();
            tableCell.Controls.Add(label);
            label.CssClass = "PageTitle";
            label.Text = szTitle;
        }

        public static string ParseTokenInDestinationURL(string szOriginalUrl, string szLoginName)
        {
            string str = szOriginalUrl;
            if (Config.Session != null && Config.Session["CustomerKey"] != null && Config.Session["CustomerKey"].ToString() != "" && szOriginalUrl.ToLower().Contains("{token}"))
            {
                Guid guid = Guid.NewGuid();
                object[] objArray = new object[9];
                objArray[0] = "INSERT INTO ws_security (xws_usr_code, xws_current_token, xws_expiration, xws_expiration_policy, xws_cst_key) values ('";
                objArray[1] = szLoginName.Replace("'", "''");
                objArray[2] = "', '";
                objArray[3] = guid.ToString();
                objArray[4] = "', '";
                DateTime now = DateTime.Now;
                objArray[5] = now.AddMinutes(Convert.ToDouble(Config.Session.Timeout));
                objArray[6] = "', 'Absolute', '";
                objArray[7] = Config.Session["CustomerKey"].ToString();
                objArray[8] = "')";
                string str1 = string.Concat(objArray);
                DataUtils.ExecuteSqlScalar(str1);
                str = string.Concat(szOriginalUrl.Substring(0, szOriginalUrl.ToLower().IndexOf("{token}")), guid.ToString(), szOriginalUrl.Substring(szOriginalUrl.ToLower().IndexOf("{token}") + 7));
            }
            return str;
        }

        public static string ReadEntityKeyFromCustomerKey(string szCustomerKey, HttpSessionState oSession)
        {
            string lower = "";
            string[] strArrays = new string[5];
            strArrays[0] = "SELECT cst_entity_key FROM co_customer ";
            strArrays[1] = DataUtils.NoLock();
            strArrays[2] = " WHERE cst_key=";
            strArrays[3] = DataUtils.ValuePrep(szCustomerKey, "av_key", true);
            strArrays[4] = " /*(NoEntity)*/";
            string str = string.Concat(strArrays);
            OleDbConnection connection = DataUtils.GetConnection();
            using (connection)
            {
                OleDbDataReader dataReader = DataUtils.GetDataReader(str, connection);
                using (dataReader)
                {
                    if (dataReader != null)
                    {
                        if (dataReader.Read())
                        {
                            lower = dataReader["cst_entity_key"].ToString().ToLower();
                            oSession["CustomerEntityKey"] = lower;
                        }
                        dataReader.Close();
                    }
                }
            }
            return lower;
        }

        public static void RecalculateCentralizedShoppingCartPrices()
        {
            OleDbConnection connection = DataUtils.GetConnection();
            Functions.RecalculateCentralizedShoppingCartPrices(connection, null);
            connection.Close();
        }

        public static void RecalculateCentralizedShoppingCartPrices(OleDbConnection oConn, OleDbTransaction oTrans)
        {
            if (Config.Session["CentralizedOrderEntry"] != null)
            {
                OrderEntry item = (OrderEntry)Config.Session["CentralizedOrderEntry"];
                CO_Individual cOIndividual = (CO_Individual)Config.Session["Individual"];
                if (cOIndividual != null && !UtilityFunctions.EmptyString(cOIndividual.CurrentKey))
                {
                    item.SetValue("inv_cst_key", cOIndividual.CurrentKey);
                    item.SetValue("inv_cst_billing_key", cOIndividual.CurrentKey);
                    item.LoadRelatedData(oConn, oTrans);
                    if (Config.GetSystemOptionBoolean("TradeOrganizationFlag") && Config.Session["OrgCustomerKey"] != null && !UtilityFunctions.EmptyString(Config.Session["OrgCustomerKey"].ToString()))
                    {
                        item.SetValue("inv_cst_billing_key", Config.Session["OrgCustomerKey"].ToString());
                        item.LoadRelatedData(oConn, oTrans);
                    }
                }
                FacadeDetails invoiceDetails = item.InvoiceDetails;
                if (!item.bReCalculatePrices)
                {
                    item.bReCalculatePrices = true;
                }
                for (int i = invoiceDetails.Count - 1; i >= 0; i--)
                {
                    if (invoiceDetails.Count - 1 >= i)
                    {
                        InvoiceDetail byIndex = (InvoiceDetail)invoiceDetails.GetByIndex(i);
                        if (UtilityFunctions.ES(byIndex.GetValue("ivd_pak_prd_key")) && !byIndex.SetPriceByProduct(oConn, oTrans))
                        {
                            invoiceDetails.Remove(byIndex);
                        }
                    }
                }
            }
        }

        public static void RecalculateShoppingCartPrices()
        {
            OleDbConnection connection = DataUtils.GetConnection();
            Functions.RecalculateShoppingCartPrices(connection, null);
            connection.Close();
        }

        public static void RecalculateShoppingCartPrices(OleDbConnection oConn, OleDbTransaction oTrans)
        {
            DataSet dataSet;
            if (Config.Session["EcommerceShoppingCart"] != null)
            {
                DynamicFacade item = (DynamicFacade)Config.Session["EcommerceShoppingCart"];
                string[] strArrays = new string[5];
                strArrays[0] = "delete from ec_shopping_cart_items where eit_esc_key in (select esc_key from ec_shopping_cart where esc_cst_key = ";
                strArrays[1] = DataUtils.ValuePrep(Config.Session["CustomerKey"].ToString(), "av_key", true);
                strArrays[2] = " and esc_order_status is null and esc_key != ";
                strArrays[3] = DataUtils.ValuePrep(item.GetValue("esc_key"), "av_key", true);
                strArrays[4] = ")";
                DataUtils.ExecuteSql(string.Concat(strArrays), oConn, oTrans);
                string[] strArrays1 = new string[5];
                strArrays1[0] = "delete from ec_payment where epm_esc_key in (select esc_key from ec_shopping_cart where esc_cst_key = ";
                strArrays1[1] = DataUtils.ValuePrep(Config.Session["CustomerKey"].ToString(), "av_key", true);
                strArrays1[2] = " and esc_order_status is null and esc_key != ";
                strArrays1[3] = DataUtils.ValuePrep(item.GetValue("esc_key"), "av_key", true);
                strArrays1[4] = ")";
                DataUtils.ExecuteSql(string.Concat(strArrays1), oConn, oTrans);
                DataUtils.ExecuteSql(string.Concat("delete from ec_shopping_cart where esc_cst_key = ", DataUtils.ValuePrep(Config.Session["CustomerKey"].ToString(), "av_key", true), " and esc_order_status is null and esc_key != ", DataUtils.ValuePrep(item.GetValue("esc_key"), "av_key", true)), oConn, oTrans);
                CO_Individual cOIndividual = (CO_Individual)Config.Session["Individual"];
                if (cOIndividual != null && !UtilityFunctions.EmptyString(cOIndividual.CurrentKey))
                {
                    item.SetValue("esc_cst_key", cOIndividual.CurrentKey);
                    item.SetValue("esc_cxa_key", cOIndividual.GetValue("cst_cxa_key"));
                    item.SetValue("esc_cxa_billing_key", cOIndividual.GetValue("cst_cxa_billing_key"));
                    item.SetValue("esc_eml_key", cOIndividual.GetValue("cst_eml_key"));
                    item.SetValue("esc_cph_key", cOIndividual.GetValue("cst_cph_key"));
                    item.SetValue("esc_cfx_key", cOIndividual.GetValue("cst_cfx_key"));
                }
                if (UtilityFunctions.EmptyString(item.GetValue("esc_cxa_billing_key")))
                {
                    item.SetValue("esc_cxa_billing_key", item.GetValue("esc_cxa_key"));
                }
                item.Update(oConn, oTrans);
                FacadeDetails facadeDetails = null;
                DataClass dataObject = null;
                string str = "E-Commerce Shopping Cart Items";
                string str1 = "EcommerceShoppingCartItem";
                string str2 = null;
                if (item.oDetailsHash.Contains(str))
                {
                    facadeDetails = (FacadeDetails)item.oDetailsHash[str];
                    dataObject = item.GetDataObject(facadeDetails.szPrefix);
                }
                if (dataObject != null)
                {
                    if (!UtilityFunctions.EmptyString(str2))
                    {
                        dataSet = dataObject.SelectDetailsWithWhereClause(str2, "", oConn, oTrans);
                    }
                    else
                    {
                        string str3 = facadeDetails.szParentForeignKeyField;
                        string currentKey = item.CurrentKey;
                        dataSet = dataObject.SelectByForeignKeyDataSet(str3, currentKey, oConn, oTrans, "", "");
                    }
                    if (dataSet != null)
                    {
                        foreach (DataRow row in dataSet.Tables[0].Rows)
                        {
                            FacadeClass facadeClass = DataUtils.InstantiateFacadeObject(str1);
                            facadeClass.oParentFacadeDetails = facadeDetails;
                            facadeClass.CurrentKey = row[facadeDetails.szKeyField].ToString();
                            facadeClass.SelectByKey(oConn, oTrans);
                            facadeDetails.Add(facadeClass, true);
                        }
                    }
                }
                if (facadeDetails.Count > 0)
                {
                    string str4 = "";
                    foreach (DynamicFacade dynamicFacade in facadeDetails)
                    {
                        if (!UtilityFunctions.EmptyString(str4))
                        {
                            str4 = string.Concat(str4, ", ");
                        }
                        str4 = string.Concat(str4, "'", dynamicFacade.GetValue("prc_prd_key").Replace("'", "''"), "'");
                    }
                    string[] strArrays2 = new string[5];
                    strArrays2[0] = "select vw_pricing_control.pat_prc_key, vw_pricing_control.prc_prd_key from vw_pricing_control join oe_price on vw_pricing_control.pat_prc_key=oe_price.prc_key where vw_pricing_control.cst_key='";
                    strArrays2[1] = cOIndividual.CurrentKey.Replace("'", "''");
                    strArrays2[2] = "' and vw_pricing_control.prc_prd_key in (";
                    strArrays2[3] = str4;
                    strArrays2[4] = ") and oe_price.prc_sell_online=1 order by vw_pricing_control.prc_price";
                    string str5 = string.Concat(strArrays2);
                    if (!UtilityFunctions.EmptyString(str4))
                    {
                        DataSet dataSet1 = DataUtils.GetDataSet(str5, oConn, oTrans);
                        if (dataSet1 != null && dataSet1.Tables[0] != null)
                        {
                            for (int i = facadeDetails.Count - 1; i >= 0; i--)
                            {
                                DynamicFacade byIndex = (DynamicFacade)facadeDetails.GetByIndex(i);
                                bool flag = false;
                                int num = 0;
                                while (num < dataSet1.Tables[0].Rows.Count)
                                {
                                    if (dataSet1.Tables[0].Rows[num]["prc_prd_key"].ToString() != byIndex.GetValue("prc_prd_key"))
                                    {
                                        num++;
                                    }
                                    else
                                    {
                                        flag = true;
                                        if (dataSet1.Tables[0].Rows[num]["pat_prc_key"].ToString() == byIndex.GetValue("prc_key"))
                                        {
                                            break;
                                        }
                                        if (HttpContext.Current.Session["ShoppingCartMessage"] == null)
                                        {
                                            HttpContext.Current.Session["ShoppingCartMessage"] = "Please note your new qualified prices have been updated.";
                                        }
                                        byIndex.SetValue("eit_prc_key", dataSet1.Tables[0].Rows[num]["pat_prc_key"].ToString());
                                        byIndex.LoadRelatedData(oConn, oTrans);
                                        byIndex.SetValue("eit_price", byIndex.GetValue("prc_price"));
                                        byIndex.Update(oConn, oTrans);
                                        break;
                                    }
                                }
                                if (!flag)
                                {
                                    facadeDetails.Remove(byIndex);
                                }
                            }
                        }
                    }
                }
            }
        }

        public void RedirectToUrlSuccess(PageClass oPage, Control oControl)
        {
            string value;
            if (Config.Session["URL_Success"] != null && !UtilityFunctions.EmptyString(Config.Session["URL_Success"].ToString()))
            {
                string str = Config.Session["URL_Success"].ToString();
                if (Config.GetSystemOption("useEmailForAuthorization").ToLower() != "true")
                {
                    value = oPage.oFacadeObject.GetValue("cst_web_login");
                }
                else
                {
                    value = oPage.oFacadeObject.GetValue("cst_eml_address_dn");
                }
                str = Functions.ParseTokenInDestinationURL(str, value);
                PageUtils.Redirect(str);
            }
        }

        private static void RemoveCell(HtmlTableCell oCell)
        {
            if (oCell != null)
            {
                oCell.Style.Add("display", "none");
            }
        }

        private static void RemoveEmptyCells(HtmlTableRow oRow)
        {
            if (oRow != null)
            {
                for (int i = oRow.Cells.Count - 1; i >= 0; i--)
                {
                    if (Functions.CellIsEmpty(oRow.Cells[i]))
                    {
                        Functions.RemoveCell(oRow.Cells[i]);
                    }
                }
            }
        }

        public static void RemoveEmptyFrameworkCells(Page oPage)
        {
            HtmlTableCell item;
            HtmlTable count = (HtmlTable)oPage.FindControl("PageFrameWorkTable");
            if (count != null)
            {
                int num = 0;
                HtmlTableRow htmlTableRow = (HtmlTableRow)count.FindControl("eWebTopPaneTableRowMiddle");
                if (!Functions.RowIsEmpty(htmlTableRow))
                {
                    Functions.RemoveEmptyCells(htmlTableRow);
                    num = htmlTableRow.Cells.Count;
                }
                else
                {
                    Functions.RemoveRow(htmlTableRow);
                    num = 0;
                }
                htmlTableRow = (HtmlTableRow)count.FindControl("eWebTopPaneTableRowTop");
                if (!Functions.RowIsEmpty(htmlTableRow))
                {
                    htmlTableRow.Cells[0].ColSpan = num;
                }
                else
                {
                    Functions.RemoveRow(htmlTableRow);
                }
                htmlTableRow = (HtmlTableRow)count.FindControl("eWebTopPaneTableRowBottom");
                if (!Functions.RowIsEmpty(htmlTableRow))
                {
                    htmlTableRow.Cells[0].ColSpan = num;
                }
                else
                {
                    Functions.RemoveRow(htmlTableRow);
                }
                HtmlTable htmlTable = (HtmlTable)count.FindControl("eWebTopPaneTable");
                if (htmlTable != null)
                {
                    if (htmlTable.Rows.Count != 0)
                    {
                        if (htmlTable.Rows.Count == 1 && htmlTable.Rows[0].Cells.Count == 1)
                        {
                            item = htmlTable.Rows[0].Cells[0];
                            Functions.KeepCellProperties(item, count.Rows[0].Cells[0]);
                            item.ColSpan = 3;
                            count.Rows[0].Cells.RemoveAt(0);
                            count.Rows[0].Cells.Add(item);
                        }
                    }
                    else
                    {
                        ((HtmlTableCell)htmlTable.Parent).Controls.Remove(htmlTable);
                    }
                }
                htmlTableRow = (HtmlTableRow)count.FindControl("eWebBottomPaneTableRowMiddle");
                if (!Functions.RowIsEmpty(htmlTableRow))
                {
                    Functions.RemoveEmptyCells(htmlTableRow);
                    num = htmlTableRow.Cells.Count;
                }
                else
                {
                    Functions.RemoveRow(htmlTableRow);
                    num = 0;
                }
                htmlTableRow = (HtmlTableRow)count.FindControl("eWebBottomPaneTableRowTop");
                if (!Functions.RowIsEmpty(htmlTableRow))
                {
                    htmlTableRow.Cells[0].ColSpan = num;
                }
                else
                {
                    Functions.RemoveRow(htmlTableRow);
                }
                htmlTableRow = (HtmlTableRow)count.FindControl("eWebBottomPaneTableRowBottom");
                if (!Functions.RowIsEmpty(htmlTableRow))
                {
                    htmlTableRow.Cells[0].ColSpan = num;
                }
                else
                {
                    Functions.RemoveRow(htmlTableRow);
                }
                htmlTable = (HtmlTable)count.FindControl("eWebBottomPaneTable");
                if (htmlTable != null)
                {
                    if (htmlTable.Rows.Count != 0)
                    {
                        if (htmlTable.Rows.Count == 1 && htmlTable.Rows[0].Cells.Count == 1)
                        {
                            item = htmlTable.Rows[0].Cells[0];
                            Functions.KeepCellProperties(item, count.Rows[2].Cells[0]);
                            item.ColSpan = 3;
                            count.Rows[2].Cells.RemoveAt(0);
                            count.Rows[2].Cells.Add(item);
                        }
                    }
                    else
                    {
                        ((HtmlTableCell)htmlTable.Parent).Controls.Remove(htmlTable);
                    }
                }
                htmlTableRow = (HtmlTableRow)count.FindControl("eWebLeftPaneTableRowMiddle");
                if (!Functions.RowIsEmpty(htmlTableRow))
                {
                    Functions.RemoveEmptyCells(htmlTableRow);
                    num = htmlTableRow.Cells.Count;
                }
                else
                {
                    Functions.RemoveRow(htmlTableRow);
                    num = 0;
                }
                htmlTableRow = (HtmlTableRow)count.FindControl("eWebLeftPaneTableRowTop");
                if (!Functions.RowIsEmpty(htmlTableRow))
                {
                    htmlTableRow.Cells[0].ColSpan = num;
                }
                else
                {
                    Functions.RemoveRow(htmlTableRow);
                }
                htmlTableRow = (HtmlTableRow)count.FindControl("eWebLeftPaneTableRowBottom");
                if (!Functions.RowIsEmpty(htmlTableRow))
                {
                    htmlTableRow.Cells[0].ColSpan = num;
                }
                else
                {
                    Functions.RemoveRow(htmlTableRow);
                }
                htmlTable = (HtmlTable)count.FindControl("eWebLeftPaneTable");
                if (htmlTable != null)
                {
                    if (htmlTable.Rows.Count != 0)
                    {
                        if (htmlTable.Rows.Count == 1 && htmlTable.Rows[0].Cells.Count == 1)
                        {
                            item = htmlTable.Rows[0].Cells[0];
                            Functions.KeepCellProperties(item, count.Rows[1].Cells[0]);
                            count.Rows[1].Cells.RemoveAt(0);
                            count.Rows[1].Cells.Insert(0, item);
                        }
                    }
                    else
                    {
                        ((HtmlTableCell)htmlTable.Parent).Controls.Remove(htmlTable);
                    }
                }
                htmlTableRow = (HtmlTableRow)count.FindControl("eWebContentPaneTableRowMiddle");
                if (!Functions.RowIsEmpty(htmlTableRow))
                {
                    Functions.RemoveEmptyCells(htmlTableRow);
                    num = htmlTableRow.Cells.Count;
                }
                else
                {
                    Functions.RemoveRow(htmlTableRow);
                    num = 0;
                }
                htmlTableRow = (HtmlTableRow)count.FindControl("eWebContentPaneTableRowTop");
                if (!Functions.RowIsEmpty(htmlTableRow))
                {
                    htmlTableRow.Cells[0].ColSpan = num;
                }
                else
                {
                    Functions.RemoveRow(htmlTableRow);
                }
                htmlTableRow = (HtmlTableRow)count.FindControl("eWebContentPaneTableRowBottom");
                if (!Functions.RowIsEmpty(htmlTableRow))
                {
                    htmlTableRow.Cells[0].ColSpan = num;
                }
                else
                {
                    Functions.RemoveRow(htmlTableRow);
                }
                htmlTable = (HtmlTable)count.FindControl("eWebContentPaneTable");
                if (htmlTable != null)
                {
                    if (htmlTable.Rows.Count != 0)
                    {
                        if (htmlTable.Rows.Count == 1 && htmlTable.Rows[0].Cells.Count == 1)
                        {
                            item = htmlTable.Rows[0].Cells[0];
                            Functions.KeepCellProperties(item, count.Rows[1].Cells[1]);
                            count.Rows[1].Cells.RemoveAt(1);
                            count.Rows[1].Cells.Insert(1, item);
                        }
                    }
                    else
                    {
                        ((HtmlTableCell)htmlTable.Parent).Controls.Remove(htmlTable);
                    }
                }
                htmlTableRow = (HtmlTableRow)count.FindControl("eWebRightPaneTableRowMiddle");
                if (!Functions.RowIsEmpty(htmlTableRow))
                {
                    Functions.RemoveEmptyCells(htmlTableRow);
                    num = htmlTableRow.Cells.Count;
                }
                else
                {
                    Functions.RemoveRow(htmlTableRow);
                    num = 0;
                }
                htmlTableRow = (HtmlTableRow)count.FindControl("eWebRightPaneTableRowTop");
                if (!Functions.RowIsEmpty(htmlTableRow))
                {
                    htmlTableRow.Cells[0].ColSpan = num;
                }
                else
                {
                    Functions.RemoveRow(htmlTableRow);
                }
                htmlTableRow = (HtmlTableRow)count.FindControl("eWebRightPaneTableRowBottom");
                if (!Functions.RowIsEmpty(htmlTableRow))
                {
                    htmlTableRow.Cells[0].ColSpan = num;
                }
                else
                {
                    Functions.RemoveRow(htmlTableRow);
                }
                htmlTable = (HtmlTable)count.FindControl("eWebRightPaneTable");
                if (htmlTable != null)
                {
                    if (htmlTable.Rows.Count != 0)
                    {
                        if (htmlTable.Rows.Count == 1 && htmlTable.Rows[0].Cells.Count == 1)
                        {
                            item = htmlTable.Rows[0].Cells[0];
                            Functions.KeepCellProperties(item, count.Rows[1].Cells[2]);
                            count.Rows[1].Cells.RemoveAt(2);
                            count.Rows[1].Cells.Insert(2, item);
                        }
                    }
                    else
                    {
                        ((HtmlTableCell)htmlTable.Parent).Controls.Remove(htmlTable);
                    }
                }
                Functions.RemoveEmptyCells(count.Rows[1]);
                count.Rows[0].Cells[0].ColSpan = count.Rows[1].Cells.Count;
                count.Rows[2].Cells[0].ColSpan = count.Rows[1].Cells.Count;
                if (Functions.RowIsEmpty(count.Rows[2]))
                {
                    Functions.RemoveRow(count.Rows[2]);
                }
                if (Functions.RowIsEmpty(count.Rows[1]))
                {
                    Functions.RemoveRow(count.Rows[1]);
                }
                if (Functions.RowIsEmpty(count.Rows[0]))
                {
                    Functions.RemoveRow(count.Rows[0]);
                }
            }
        }

        private static void RemoveRow(HtmlTableRow oRow)
        {
            if (oRow != null)
            {
                oRow.Style.Add("display", "none");
            }
        }

        private static bool RowIsEmpty(HtmlTableRow oRow)
        {
            bool flag = true;
            if (oRow != null)
            {
                for (int i = 0; i < oRow.Cells.Count && flag; i++)
                {
                    flag = Functions.CellIsEmpty(oRow.Cells[i]);
                }
            }
            return flag;
        }

        public static void SetLoginTableVisibility(HtmlTable LoginTable, HtmlGenericControl ContentListTableDiv)
        {
            if (HttpContext.Current.Session["CustomerKey"] != null)
            {
                LoginTable.Visible = false;
                ContentListTableDiv.Attributes.Add("style", "DISPLAY:inline;");
                return;
            }
            else
            {
                LoginTable.Visible = true;
                ContentListTableDiv.Attributes.Add("style", "DISPLAY:none;");
                return;
            }
        }

        public enum BrowserType
        {
            IE,
            IEMac,
            NS4,
            NS6
        }
    }
}