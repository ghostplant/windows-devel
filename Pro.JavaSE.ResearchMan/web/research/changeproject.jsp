<%-- 
    Document   : appacademicactivity
    Created on : 2011-9-13, 18:12:16
    Author     : Ghostplant
--%>

<%@page contentType="text/html" pageEncoding="UTF-8"%>
<%@taglib prefix="s" uri="/struts-tags" %>

<body>
    <s:form action="changeproject" theme="simple" validate="true">
        <table border="1" cellspacing="0" cellpadding="0" align="center" >
            <tr>
                <td style="background-color: rgb(255, 255, 51)"colspan="11" width="773" align="center" valign="top" ><p ><strong>更改项目、课程 </strong></p></td>
            </tr>
            <tr style="background-color:antiquewhite">
                <td  align="center" valign="middle" ><p >序号 </p></td>
                <td  align="center" valign="middle" ><p >项目或课题名称 </p></td>
                <td  align="center" valign="middle" ><p >编号 </p></td>
                <td  align="center" valign="middle" ><p >来源 </p></td>
                <td  align="center" valign="middle" ><p >类别 </p></td>
                <td  align="center" valign="middle" ><p >起始时间(年份) </p></td>
                <td  align="center" valign="middle" ><p >终止时间(年份)</p></td>
                <td  align="center" valign="middle" ><p >本单位所有人员（注明排名） </p></td>
                <td  align="center" valign="middle" ><p >经费（万元） </p></td>
                <td  align="center" valign="middle" ><p >状态 </p></td>
                <td  align="center" valign="middle" ><p >操作 </p></td>
            </tr>
            <tr style="background-color: rgb(153, 255, 255)">
                <td  align="center" valign="middle" ><p ><s:textfield theme="simple" name="id" size="1" /></p></td>
                <td  align="center" valign="middle" ><p ><s:textfield theme="simple" name="name" size="1" /></p></td>
                <td  align="center" valign="middle" ><p ><s:textfield theme="simple" name="number" size="1"/></p></td>
                <td  align="center" valign="middle" ><p ><s:textfield theme="simple" name="source" size="1"/></p></td>
                <td  align="center" valign="middle" ><p ><s:textfield theme="simple" name="type" size="1"/></p></td>
                <td  align="center" valign="middle" ><p ><s:textfield theme="simple" name="startYear" size="1"/></p></td>
                <td  align="center" valign="middle" ><p ><s:textfield theme="simple" name="endYear" size="1"/></p></td>

                <td  align="center" valign="middle" ><p >
                        <s:hidden name="teacherList"/>
                        <s:generator val="teacherList" separator="@" >
                            <s:iterator>
                                <s:property/><br/>
                            </s:iterator>
                        </s:generator>
                        研究人员：<s:select list="teachers" name="itemOfTeacher" label="研究人员（工号）"/>
                        排名：<s:textfield theme="simple" name="rank" size="1" label="排名"/>
                        <s:submit value="列入" theme="simple" method="addItem" />
                        <s:submit value="清空" theme="simple" method="clearItem" />
                    </p>
                </td>

                <td  align="center" valign="middle" ><p ><s:textfield theme="simple" name="cost" size="1"/></p></td>
                <td  align="center" valign="middle" ><p ><s:textfield theme="simple" name="status" size="1"/></p></td>
                <td  align="center" valign="middle" ><p ><s:submit theme="simple" value="更新" method="insert"/></p></td>
            </tr>
        </table>
    </s:form>
</body>
