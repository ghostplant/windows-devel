<%-- 
    Document   : appacademicactivity
    Created on : 2011-9-13, 18:12:16
    Author     : Ghostplant
--%>

<%@page contentType="text/html" pageEncoding="UTF-8"%>
<%@taglib prefix="s" uri="/struts-tags" %>

<body>
    <s:form action="workingcondition" theme="simple">
        <table align="center" border="1" cellspacing="0" cellpadding="0">
            <tr>
                <td style="background-color: rgb(255, 255, 51)" width="690" align="center" colspan="6"><p align="center"><strong>可用于本一级学科点研究生培养的教学或专业实验室情况（仪器设备限填20台/套） </strong></p></td>
            </tr>
            <tr style="background-color:powderblue">
                <td><p align="center">实验室名称 </p></td>
                <td><p align="center">仪器设备名称、型号、规格 </p></td>
                <td><p align="center">实验室人员配备（人）</p></td>
                <td><p align="center">仪器设备（台或套）</p></td>
                <td><p align="center">仪器设备值（万元）</p></td>
                <td><p align="center">项目操作</p></td>
            </tr>
            <tr style="background-color:antiquewhite">
                <td><p align="center"><strong>总  计</strong></p></td>
                <td><p align="center">----</p></td>
                <td><p align="center"><s:property value="sumPersonnel"/></p></td>
                <td><p align="center"><s:property value="sumDevice"/></p></td>
                <td><p align="center"><s:property value="sumCost"/></p></td>
                <td><p align="center">----</p></td>
            </tr>

            <s:iterator value="%{recordList}" id="id">
                <tr>
                    <td valign="top"><p align="center"><s:property value="#id.name"/></p></td>
                    <td valign="top"><p align="center">
                            <s:iterator value="getDeviceSet(#id.id)" id="dev">
                                <s:property value="#dev.name"/><br/>
                            </s:iterator>
                        </p></td>
                    <td valign="top"><p align="center"><s:property value="#id.personnelCount"/></p></td>
                    <td valign="top"><p align="center"><s:property value="#id.deviceCount"/></p></td>
                    <td valign="top"><p align="center"><s:property value="#id.deviceCost"/></p></td>
                    <td valign="top"><p align="center">
                            <s:url id="url" method="delete">
                                <s:param name="selectedId" value="#id.id"/>
                            </s:url>
                            <s:a href="%{url}">删除</s:a>
                            </p></td>
                    </tr>
            </s:iterator>

            <tr>
                <td valign="top"><p align="center"><s:textfield theme="simple" name="labName" size="12"/></p></td>
                <td valign="top"><p align="center">
                        <s:hidden name="deviceList"/>
                        <s:generator val="deviceList" separator="~">
                            <s:iterator>
                                <s:property/><br/>
                            </s:iterator>
                        </s:generator>
                        <s:textfield theme="simple" name="item" size="6"/><br/>
                        <s:submit value="列入" theme="simple" method="addItem"/>
                        <s:submit value="清空" theme="simple" method="clearItem"/>
                    </p></td>
                <td valign="top"><p align="center"><s:textfield theme="simple" name="totPersonnel" size="4"/></p></td>
                <td valign="top"><p align="center"><s:textfield theme="simple" name="totDevice" size="4"/></p></td>
                <td valign="top"><p align="center"><s:textfield theme="simple" name="totCost" size="4"/></p></td>
                <td valign="top"><p align="center"><s:submit theme="simple" value="添加" method="insert"/></p></td>
            </tr>
        </table>
        <br/>
        <p align="center"><s:submit theme="simple" value="导出" method="export"/></p>
    </s:form>
</body>
