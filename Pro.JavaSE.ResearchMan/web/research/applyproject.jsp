<%-- 
    Document   : adduser
    Created on : 2011-9-1, 16:43:16
    Author     : Ghostplant
--%>
<%@page contentType="text/html" pageEncoding="UTF-8"%>
<%@taglib  prefix="s" uri="/struts-tags" %>
<body>
    <s:form action="applyproject" validate="true">
        <table>
            <thead>
                <tr>
                    <th>申请项目</th>
                </tr>
            </thead>
            <tbody>
                <tr>

                    <s:textfield name="name" label="项目或课题名称" />
                    <s:textfield name="number" label="项目或课题编号" />
                    <s:textfield name="source" label="来源" />
                    <s:textfield name="type" label="类别" />
                    <td><s:select list="StartYears" name="startYear" label="项目开始时间"/></td>
                    <td>  <s:select list="EndYears" name="endYear" label="项目结束时间"/></td>
                    <s:iterator value="personMap.keySet()" id="key" status="st">
                    <tr>
                        <td>
                            索引：<s:property value="#st.getIndex()+1"/>
                        </td>
                        <td>
                            姓名：<s:property value="personMap.get(#key)"/>
                        </td>
                        <td>
                            排名： <s:property value="#key"/>
                        </td>
                    </tr>
                </s:iterator>

                </tr>
                <tr>



                    <td>
                        <s:select list="teachers" name="teacher" label="研究人员"/>
                    </td>



                    <td>
                        <s:textfield name="rank" label="排名"/>
                    </td>
                    <td>
                        <s:submit value="添加"  method="addPerson"/>
                    </td>
                </tr>


                <s:select name="status" label="状态" list="{'申请','在研','结题','其它'}"/>
                <s:submit value="提交" method="submit" />


            </tbody>
        </table>
    </s:form>
</body>











