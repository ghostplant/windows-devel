<%-- 
    Document   : libraryorder
    Created on : 2011-9-15, 11:22:09
    Author     : Ghostplant
--%>

<%@page contentType="text/html" pageEncoding="UTF-8"%>
<%@taglib prefix="s" uri="/struts-tags"%>

<body>
    <s:form action="libraryorder" theme="simple">
        <table style="font-size: small" align="center" border="1" cellspacing="0" cellpadding="0">
            <tr>
                <td style="background-color: rgb(255, 255, 51)" width="692" colspan="10"><p align="center"><strong>本一级学科点图书资料情况 </strong></p></td>
            </tr>
            <tr>
                <td style="background-color:paleturquoise"><p align="center">馆藏总量（万册） </p></td>
                <td style="background-color:lavender"><p align="center"><s:property value="totalAmount"/></p></td>
                <td style="background-color:paleturquoise"><p align="center">中文藏书量（万册） </p></td>
                <td style="background-color:lavender"><p align="center"><s:textfield theme="simple" name="chsStoreAmout" size="1"/></p></td>
                <td style="background-color:paleturquoise"><p align="center">外文藏书量（万册） </p></td>
                <td style="background-color:lavender"><p align="center"><s:textfield theme="simple" name="fgnStoreAmount" size="1"/></p></td>
                <td style="background-color:paleturquoise"><p align="center">中文期刊（种） </p></td>
                <td style="background-color:lavender"><p align="center"><s:textfield theme="simple" name="chsPeriodicalAmount" size="1"/></p></td>
                <td style="background-color:paleturquoise"><p align="center">外文期刊（种） </p></td>
                <td style="background-color:lavender"><p align="center"><s:textfield theme="simple" name="fgnPeriodicalAmount" size="1"/></p></td>
            </tr>
            <tr>
                <td style="background-color:paleturquoise"><p align="center">数据库（种） </p></td>
                <td style="background-color:lavender"><p align="center"><s:textfield theme="simple" name="databaseTypeAmount" size="1"/></p></td>
                <td style="background-color:paleturquoise"><p align="center">中文电子图书（万册） </p></td>
                <td style="background-color:lavender"><p align="center"><s:textfield theme="simple" name="chsEBookAmount" size="1"/></p></td>
                <td style="background-color:paleturquoise"><p align="center">外文电子图书（万册） </p></td>
                <td style="background-color:lavender"><p align="center"><s:textfield theme="simple" name="fgnEBookAmount" size="1"/></p></td>
                <td style="background-color:paleturquoise"><p align="center">中文电子期刊（种） </p></td>
                <td style="background-color:lavender"><p align="center"><s:textfield theme="simple" name="chsEPeriodicalAmount" size="1"/></p></td>
                <td style="background-color:paleturquoise"><p align="center">外文电子期刊（种） </p></td>
                <td style="background-color:lavender"><p align="center"><s:textfield theme="simple" name="fgnEPeriodicalAmount" size="1"/></p></td>
            </tr>
            <tr style="background-color:thistle">
                <td colspan="10" align="right" valign="top"><s:submit theme="simple" value="更新统计量" method="update"/> <s:submit theme="simple" value="全部导出" method="export"/></td>
            </tr>
            <tr style="background-color:lavender">
                <td colspan="10" valign="top"><p><strong>订购主要专业期刊、重要图书的名称、册数、时间 </strong><br /><br />
                        类型：<s:select theme="simple" list="{'外文期刊','中文期刊','数字资源'}" name="type"/>；
                        资源名称：<s:textfield theme="simple" name="bookName" size="6"/>；
                        起订年份：<s:textfield theme="simple" name="orderYear" size="4"/>。
                        <s:submit theme="simple" value="添加" method="insert"/> <br/><br/>
                    <p>1. 主要外文期刊： <br />
                        <s:iterator value="%{foreignList}" id="id">
                            &nbsp;&nbsp;<s:property value="#id.name"/>（<s:property value="#id.subscribeYear"/>起） <s:url id="url" method="delete">
                                <s:param name="selectedId" value="#id.id"/>
                            </s:url>
                            <s:a href="%{url}">删除</s:a>；
                        </s:iterator>
                    </p>
                    <p>2. 主要中文期刊： <br />
                        <s:iterator value="%{nativeList}" id="id">
                            &nbsp;&nbsp;<s:property value="#id.name"/>（<s:property value="#id.subscribeYear"/>起） <s:url id="url" method="delete">
                                <s:param name="selectedId" value="#id.id"/>
                            </s:url>
                            <s:a href="%{url}">删除</s:a>；
                        </s:iterator>
                    </p>
                    <p>3. 订购主要数字资源的时间和名称（含电子图书、期刊、全文数据库、文摘索引数据库等）： <br />
                        <s:iterator value="%{electronicList}" id="id">
                            &nbsp;&nbsp;<s:property value="#id.name"/>（<s:property value="#id.subscribeYear"/>起） <s:url id="url" method="delete">
                                <s:param name="selectedId" value="#id.id"/>
                            </s:url>
                            <s:a href="%{url}">删除</s:a>；
                        </s:iterator>
                    </p>
                </td>
            </tr>
        </table>

    </s:form>
</body>