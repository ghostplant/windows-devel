<%-- 
    Document   : appacademicactivity
    Created on : 2011-9-13, 18:12:16
    Author     : Ghostplant
--%>

<%@page contentType="text/html" pageEncoding="UTF-8"%>
<%@taglib prefix="s" uri="/struts-tags" %>

<body>
    <s:form action="queryproject" theme="simple">
        <table border="1" cellspacing="0" cellpadding="0" align="center" >
            <tr>
                <td colspan="12" width="773" align="center" valign="top" style="background-color: rgb(255, 255, 51)"><p ><strong>更改项目、课程 </strong></p></td>
            </tr>
            <tr style="background-color:antiquewhite">
                <td width="35" align="center" valign="middle" ><p >序号 </p></td>
                <td width="95" align="center" valign="middle" ><p >项目或课题名称 </p></td>
                <td width="65" align="center" valign="middle" ><p >编号 </p></td>
                <td width="62" align="center" valign="middle" ><p >来源 </p></td>
                <td width="68" align="center" valign="middle" ><p >类别 </p></td>
                <td width="62" align="center" valign="middle" ><p >起始时间(年份) </p></td>
                <td width="62" align="center" valign="middle" ><p >终止时间(年份)</p></td>
                <td width="124" align="center" valign="middle" ><p >本单位所有人员（注明排名） </p></td>
                <td width="49" align="center" valign="middle" ><p >经费（万元） </p></td>
                <td width="57" align="center" valign="middle" ><p >状态 </p></td>
                <td width="46" align="center" valign="middle" ><p >操作 1</p></td>
                <td width="46" align="center" valign="middle" ><p >操作 2</p></td>
            </tr>

            <tr style="background-color: rgb(153, 255, 255)">
                <td width="35" align="center" valign="middle" ><p >-- </p></td>
                <td width="95" align="center" valign="middle" ><p >--</p></td>
                <td width="65" align="center" valign="middle" ><p >-- </p></td>
                <td width="62" align="center" valign="middle" ><p >-- </p></td>
                <td width="68" align="center" valign="middle" ><p >--</p></td>
                <td width="62" align="center" valign="middle" ><p >-- </p></td>
                <td width="62" align="center" valign="middle" ><p >--</p></td>
                <td width="124" align="center" valign="middle" ><p >-- </p></td>
                <td width="49" align="center" valign="middle" ><p >-- </p></td>
                <td width="57" align="center" valign="middle" ><p >-- </p></td>
                <td width="46" align="center" valign="middle" ><p ><s:select list="projects" name="projectName" label="按项目名称查询"/><s:submit value="按项目名称查询" method="queryByProject"/></p></td>
                <td width="46" align="center" valign="middle" ><p ><s:select list="teachers"  name="teacherName" label="按老师查询"/><s:submit value="按老师查询" method="queryByTeacher" /></p></td>
            </tr>
            <s:if test="project!=null">
                <tr style="background-color: rgb(153, 255, 255)">
                    <td width="35" align="center" valign="middle" ><p ><s:property value="project.id"/></p></td>
                    <td width="95" align="center" valign="middle" ><p ><s:property value="project.name"/></p></td>
                    <td width="65" align="center" valign="middle" ><p ><s:property value="project.number"/></p></td>
                    <td width="62" align="center" valign="middle" ><p ><s:property value="project.source"/></p></td>
                    <td width="68" align="center" valign="middle" ><p ><s:property value="project.type"/></p></td>
                    <td width="62" align="center" valign="middle" ><p ><s:property value="project.startYear"/></p></td>
                    <td width="62" align="center" valign="middle" ><p ><s:property value="project.endYear"/></p></td>

                    <td width="124" align="center" valign="middle" ><p >
                            <s:iterator value="getTeacherOfProjectSet(project.id)" id="tea">
                                <s:property value="#tea.techerId"/><s:property value="getTeacherName(#tea.teacherId)"/><s:property value="#tea.rank"/> <br/>
                            </s:iterator>
                        </p>
                    </td>

                    <td width="49" align="center" valign="middle" ><p ><s:property value="project.cost"/></p></td>
                    <td width="57" align="center" valign="middle" ><p ><s:property value="project.status"/></p></td>
                    <td width="46" align="center" valign="middle" ><p >--</p></td>
                    <td width="46" align="center" valign="middle" ><p >--</p></td>
                </tr>

            </s:if>





            <s:if test="teacher!=null">
                <s:iterator value="getProjectOfTeacherSet(teacher.id)" id="protea">
                    <tr style="background-color: rgb(153, 255, 255)">
                        <td width="100" valign="top"><p align="center"><s:property value="#protea.projectId"/></p></td>
                        <td width="100" valign="top"><p align="center"><s:property value="getPro(#protea.projectId).name"/></p></td>
                        <td width="100" valign="top"><p align="center"><s:property value="getPro(#protea.projectId).number"/></p></td>
                        <td width="100" valign="top"><p align="center"><s:property value="getPro(#protea.projectId).source"/></p></td>
                        <td width="100" valign="top"><p align="center"><s:property value="getPro(#protea.projectId).type"/></p></td>
                        <td width="100" valign="top"><p align="center"><s:property value="getPro(#protea.projectId).startYear"/></p></td>
                        <td width="100" valign="top"><p align="center"><s:property value="getPro(#protea.projectId).endYear"/></p></td>
                        <td width="100" valign="top"><p align="center"><s:property value="teacher.getName()"/></p></td>
                        <td width="100" valign="top"><p align="center"><s:property value="getPro(#protea.projectId).cost"/></p></td>
                        <td width="100" valign="top"><p align="center"><s:property value="getPro(#protea.projectId).status"/></p></td>
                        <td width="100" valign="top"><p align="center">--</p></td>
                        <td width="100" valign="top"><p align="center">--</p></td>
                    </tr>
                </s:iterator>
            </s:if>
        </table>
    </s:form>
</body>
