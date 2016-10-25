<%-- 
    Document   : download
    Created on : 2011-9-19, 16:30:55
    Author     : Administrator
--%>

<%@page contentType="text/html" pageEncoding="UTF-8"%>
<%@taglib prefix="s" uri="/struts-tags" %>


<body>
    <s:form action="download" theme="simple">
        <table align="center" border="1" cellspacing="0" cellpadding="0">
            
  <tr >
    <td width="624" valign="center" colspan="8" align="center" style="background-color: rgb(255, 255, 51)"><p >本一级学科点研究生招生与授予学位人数 </p></td>
  </tr>
  <tr style="background-color:antiquewhite">
    <td width="157" valign="center" colspan="2" ><p > </p></td>
    <td width="77" valign="center" align="center"><p ><s:property value="year-4"/> </p></td>
    <td width="77" valign="center" align="center"><p ><s:property value="year-3"/> </p></td>
    <td width="77" valign="center" align="center"><p ><s:property value="year-2"/></p></td>
    <td width="77" valign="center" align="center"><p ><s:property value="year-1"/></p></td>
    <td width="78" valign="center" align="center"><p ><s:property value="year"/></p></td>
    <td width="88" valign="center" align="center"><p >近五年人数合计</p></td>
  </tr>
  <tr >
    <td width="57" valign="center" rowspan="2" align="center" style="background-color: rgb(153, 255, 255)"><p >硕士 </p></td>
    <td width="100" valign="center" align="center" style="background-color: rgb(153, 255, 255)"><p >招生人数 </p></td>
    <td width="77" valign="center" align="center"><p ><s:property value="getAmount1(year-4)"/></p></td>
    <td width="77" valign="center" align="center"><p ><s:property value="getAmount1(year-3)"/> </p></td>
    <td width="77" valign="center" align="center"><p ><s:property value="getAmount1(year-2)"/></p></td>
    <td width="77" valign="center" align="center"><p ><s:property value="getAmount1(year-1)"/></p></td>
    <td width="78" valign="center" align="center"><p ><s:property value="getAmount1(year)"/></p></td>
    <td width="78" valign="center" align="center"><p ><s:property value="totalenrollmaster"/></p></td>
  </tr>
  <tr >
    <td width="100" valign="center" align="center" style="background-color: rgb(153, 255, 255)"><p >授予学位人数 </p></td>
    <td width="77" valign="center" align="center"><p ><s:property value="getAmount2(year-4)"/></p></td>
    <td width="77" valign="center" align="center"><p ><s:property value="getAmount2(year-3)"/></p></td>
    <td width="77" valign="center" align="center"><p ><s:property value="getAmount2(year-2)"/></p></td>
    <td width="77" valign="center" align="center"><p ><s:property value="getAmount2(year-1)"/></p></td>
    <td width="78" valign="center" align="center"><p ><s:property value="getAmount2(year)"/></p></td>
    <td width="78" valign="center" align="center"><p ><s:property value="totalgraduatemaster"/></p></td>
  </tr>
  <tr >
    <td width="57" valign="center" rowspan="2" align="center" style="background-color: rgb(153, 255, 255)"><p >博士 </p></td>
    <td width="100" valign="center" align="center" style="background-color: rgb(153, 255, 255)"><p >招生人数 </p></td>
    <td width="77" valign="center" align="center"><p ><s:property value="getAmount3(year-4)"/></p></td>
    <td width="77" valign="center" align="center"><p ><s:property value="getAmount3(year-3)"/></p></td>
    <td width="77" valign="center" align="center"><p ><s:property value="getAmount3(year-2)"/></p></td>
    <td width="77" valign="center" align="center"><p ><s:property value="getAmount3(year-1)"/></p></td>
    <td width="78" valign="center" align="center"><p ><s:property value="getAmount3(year)"/></p></td>
    <td width="78" valign="center" align="center"><p ><s:property value="totalenrolldoctor"/></p></td>
  </tr>
  <tr >
    <td width="100" valign="center" align="center" style="background-color: rgb(153, 255, 255)"><p >授予学位人数 </p></td>
    <td width="77" valign="center" align="center"><p ><s:property value="getAmount4(year-4)"/></p></td>
    <td width="77" valign="center" align="center"><p ><s:property value="getAmount4(year-3)"/></p></td>
    <td width="77" valign="center" align="center"><p ><s:property value="getAmount4(year-2)"/></p></td>
    <td width="77" valign="center" align="center"><p ><s:property value="getAmount4(year-1)"/></p></td>
    <td width="78" valign="center" align="center"><p ><s:property value="getAmount4(year)"/></p></td>
    <td width="78" valign="center" align="center"><p ><s:property value="totalgraduatedoctor"/></p></td>
  </tr>
</table>
        
        
        
        
        
        <p align="center"><s:submit theme="simple" value="下载" method="export"/></p>
        
    </s:form>
</body>
