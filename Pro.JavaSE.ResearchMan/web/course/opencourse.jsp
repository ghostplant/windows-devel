<%-- 
    Document   : adduser
    Created on : 2011-9-1, 16:43:16
    Author     : Ghostplant
--%>
<%@page contentType="text/html" pageEncoding="UTF-8"%>
<%@taglib  prefix="s" uri="/struts-tags" %>
<body>
    <s:form action="opencourse" validate="true">
        <table>
            <thead>
                <tr>
                    <th>开设课程</th>
                </tr>
            </thead>
            <tbody>
                <tr>

                    <s:textfield name="id" label="课程编号" />
                    <s:textfield name="name" label="课程名称" />
                    <s:textfield name="creditHour" label="学时" />
                    <s:textfield name="creditValue" label="学分" />
                    <s:textfield name="target" label="授课对象" />
                    <s:textfield name="type" label="类型" />
                    <s:select list="teachers" name="teacher"  label="教师"/>
                    <s:radio name="state" value="'已开设'" list="{'已开设','待开放'}"/>
                    <s:submit value="提交" method="submit" />
                    
                    
                </tr>


            </tbody>
        </table>
    </s:form>
</body>
