using System;
using System.Data;
using System.Configuration;
using System.Web;
using System.Web.Security;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.Web.UI.WebControls.WebParts;
using System.Web.UI.HtmlControls;

/// <summary>
/// PublicStatus 的摘要说明
/// </summary>
public class PublicStatus
{
    public const String validChar = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    public static bool validLetter(String str)
    {
        for (int i = 0; i < str.Length; ++i)
            if (!(str[i] >= 'a' && str[i] <= 'z' || str[i] >= 'A' && str[i] <= 'Z'))
                return false;
        return true;
    }
    public static bool validDigit(String str)
    {
        for (int i = 0; i < str.Length; ++i)
            if (!(str[i] >= '0' && str[i] <= '9'))
                return false;
        return true;
    }
    public static bool validLetterDigit(String str)
    {
        for (int i = 0; i < str.Length; ++i)
            if (!(str[i] >= 'a' && str[i] <= 'z' || str[i] >= 'A' && str[i] <= 'Z' || str[i]>='0' && str[i]<='9'))
                return false;
        return true;
    }
}
