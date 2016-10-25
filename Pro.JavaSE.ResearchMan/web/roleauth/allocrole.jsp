<%-- 
    Document   : allocauthority
    Created on : 2011-9-8, 21:13:87
    Author     : Ghostplant
--%>

<%@page contentType="text/html" pageEncoding="UTF-8"%>
<%@taglib prefix="s" uri="/struts-tags" %>

<body>
    <s:form action="allocrole">
        <s:if test="firstStep">
            <s:select list="teachers" listKey="id" listValue="name" name="teacherId" value="bean.id" label="选择教师用户"/>
            <s:submit value="下一步" method="next"/>
        </s:if><s:else>
            <s:hidden name="selectedTeacherId"/>
            <s:hidden name="selectedTeacherName"/>
            <s:textfield disabled="true" name="textSelectedTeacherName" label="所选用户的名称"/>
            <s:checkboxlist name="play" list="roles" label="角色类别"/>
            <s:submit value="更新角色" method="update"/>
        </s:else>
    </s:form>
</body>