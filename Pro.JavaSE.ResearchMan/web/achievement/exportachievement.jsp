<%--
    Document   : exportachievement
    Created on : 2011-9-20, 21:28:45
    Author     : zyz
--%>

<%@page contentType="text/html" pageEncoding="UTF-8"%>
<%@taglib prefix="s" uri="/struts-tags" %>
<%@taglib prefix="jscalendar" uri="/jscalendar" %>
<!DOCTYPE html>
<html>

    <script>
        function ChangeTable(){
            var traget1=document.getElementById("table1");
            var traget2=document.getElementById("table2");
            var traget3=document.getElementById("table3");
            var traget4=document.getElementById("table4");
            var o=document.getElementById("exportachievement_type");
            if(o.value==""){
                traget1.style.display="none";
                traget2.style.display="none";
                traget3.style.display="none";
                traget4.style.display="none";
            }else if (o.value=="研究生成果"){
                traget1.style.display="";
                traget2.style.display="none";
                traget3.style.display="none";
                traget4.style.display="none";
            }else if (o.value=="教学成果"){
                traget1.style.display="none";
                traget2.style.display="";
                traget3.style.display="none";
                traget4.style.display="none";
            }else if (o.value=="专著发明"){
                traget1.style.display="none";
                traget2.style.display="none";
                traget3.style.display="";
                traget4.style.display="none";
            }else if (o.value=="论文"){
                traget1.style.display="none";
                traget2.style.display="none";
                traget3.style.display="none";
                traget4.style.display="";
            }
        }
    </script>
    <body onload="ChangeTable()">
        <s:form action="exportachievement" theme="simple" align="center">
            <s:div align="center">
                <h1>成果导出</h1>
            </s:div>
            时间：从<jscalendar:head/>
            <jscalendar:jscalendar name="startDate" format="%Y-%m-%d" showstime="true" size="6"/>
            到<jscalendar:head/>
            <jscalendar:jscalendar name="endDate" format="%Y-%m-%d" showstime="true" size="6"/><br/>
            <s:select list="{'','研究生成果','教学成果','专著发明','论文'}" name="type"/>
            <s:submit value="提交" method="search" onclick="ChangeTable()"/>
            <table id="table1">
                <tr>
                    <td>
                        <s:iterator value="selectList" id="tuple">
                            <table>
                                <tr>
                                    <td width="50" align="center">
                                        <s:property value="presonName"/>
                                    </td>
                                    <td width="300" align="center">
                                        <s:property value="achievementName"/>
                                    </td>
                                    <td width="300" align="center">
                                        <s:property value="detail"/>
                                    </td>
                                    <td width="50" align="center">
                                        <s:property value="date"/>
                                    </td>
                                    <td width="50" align="center">
                                        <s:url id="url" method="add">
                                            <s:param name="key" value="#tuple.key"/>
                                        </s:url>
                                        <s:a href="%{url}">选取</s:a>
                                        </td>
                                    </tr>
                                </table>
                        </s:iterator>
                        <s:iterator value="exportList" id="tuple">
                            <table>
                                <tr>
                                    <td width="50" align="center">
                                        <s:property value="presonName"/>
                                    </td>
                                    <td width="300" align="center">
                                        <s:property value="achievementName"/>
                                    </td>
                                    <td width="300" align="center">
                                        <s:property value="detail"/>
                                    </td>
                                    <td width="50" align="center">
                                        <s:property value="date"/>
                                    </td>
                                    <td width="20" align="center">
                                        <s:url id="url" method="remove">
                                            <s:param name="key" value="#tuple.key"/>
                                        </s:url>
                                        <s:a href="%{url}">移除</s:a>
                                        </td>
                                        <td width="15" align="center">
                                        <s:url id="url" method="moveUp">
                                            <s:param name="key" value="#tuple.key"/>
                                        </s:url>
                                        <s:a href="%{url}">上移</s:a>
                                        </td>
                                        <td width="15" align="center">
                                        <s:url id="url" method="moveDown">
                                            <s:param name="key" value="#tuple.key"/>
                                        </s:url>
                                        <s:a href="%{url}">下移</s:a>
                                        </td>
                                    </tr>
                                </table>
                        </s:iterator>
                    </td>
                </tr>
            </table>
            <table id="table2">
                <tr>
                    <td>
                        <s:iterator value="selectList" id="tuple">
                            <table>
                                <tr>
                                    <td width="300" align="center">
                                        <s:property value="achievementName"/>
                                    </td>
                                    <td width="50" align="center">
                                        <s:property value="presonName"/>
                                    </td>
                                    <td width="300" align="center">
                                        <s:property value="detail"/>
                                    </td>
                                    <td width="50" align="center">
                                        <s:url id="url" method="add">
                                            <s:param name="key" value="#tuple.key"/>
                                        </s:url>
                                        <s:a href="%{url}">选取</s:a>
                                        </td>
                                    </tr>
                                </table>
                        </s:iterator>
                        <s:iterator value="exportList" id="tuple">
                            <table>
                                <tr>
                                    <td width="300" align="center">
                                        <s:property value="achievementName"/>
                                    </td>
                                    <td width="50" align="center">
                                        <s:property value="presonName"/>
                                    </td>
                                    <td width="300" align="center">
                                        <s:property value="detail"/>
                                    </td>
                                    <td width="20" align="center">
                                        <s:url id="url" method="remove">
                                            <s:param name="key" value="#tuple.key"/>
                                        </s:url>
                                        <s:a href="%{url}">移除</s:a>
                                        </td>
                                        <td width="15" align="center">
                                        <s:url id="url" method="moveUp">
                                            <s:param name="key" value="#tuple.key"/>
                                        </s:url>
                                        <s:a href="%{url}">上移</s:a>
                                        </td>
                                        <td width="15" align="center">
                                        <s:url id="url" method="moveDown">
                                            <s:param name="key" value="#tuple.key"/>
                                        </s:url>
                                        <s:a href="%{url}">下移</s:a>
                                        </td>
                                    </tr>
                                </table>
                        </s:iterator>
                    </td>
                </tr>
            </table>
            <table id="table3">
                <tr>
                    <td>
                        <s:iterator value="selectList" id="tuple">
                            <table>
                                <tr>
                                    <td width="300" align="center">
                                        <s:property value="achievementName"/>
                                    </td>
                                    <td width="75" align="center">
                                        <s:property value="presonName"/>
                                    </td>
                                    <td width="75" align="center">
                                        <s:property value="date"/>
                                    </td>
                                    <td width="300" align="center">
                                        <s:property value="detail"/>
                                    </td>
                                    <td width="50" align="center">
                                        <s:url id="url" method="add">
                                            <s:param name="key" value="#tuple.key"/>
                                        </s:url>
                                        <s:a href="%{url}">选取</s:a>
                                        </td>
                                    </tr>
                                </table>
                        </s:iterator>
                        <s:iterator value="exportList" id="tuple">
                            <table>
                                <tr>
                                    <td width="300" align="center">
                                        <s:property value="achievementName"/>
                                    </td>
                                    <td width="75" align="center">
                                        <s:property value="presonName"/>
                                    </td>
                                    <td width="75" align="center">
                                        <s:property value="date"/>
                                    </td>
                                    <td width="300" align="center">
                                        <s:property value="detail"/>
                                    </td>
                                    <td width="20" align="center">
                                        <s:url id="url" method="remove">
                                            <s:param name="key" value="#tuple.key"/>
                                        </s:url>
                                        <s:a href="%{url}">移除</s:a>
                                        </td>
                                        <td width="15" align="center">
                                        <s:url id="url" method="moveUp">
                                            <s:param name="key" value="#tuple.key"/>
                                        </s:url>
                                        <s:a href="%{url}">上移</s:a>
                                        </td>
                                        <td width="15" align="center">
                                        <s:url id="url" method="moveDown">
                                            <s:param name="key" value="#tuple.key"/>
                                        </s:url>
                                        <s:a href="%{url}">下移</s:a>
                                        </td>
                                    </tr>
                                </table>
                        </s:iterator>
                    </td>
                </tr>
            </table>
            <table id="table4">
                <tr>
                    <td>
                        <s:iterator value="selectList" id="tuple">
                            <table>
                                <tr>
                                    <td width="300" align="center">
                                        <s:property value="achievementName"/>
                                    </td>
                                    <td width="75" align="center">
                                        <s:property value="presonName"/>
                                    </td>
                                    <td width="75" align="center">
                                        <s:property value="date"/>
                                    </td>
                                    <td width="300" align="center">
                                        <s:property value="detail"/>
                                    </td>
                                    <td width="50" align="center">
                                        <s:url id="url" method="add">
                                            <s:param name="key" value="#tuple.key"/>
                                        </s:url>
                                        <s:a href="%{url}">选取</s:a>
                                        </td>
                                    </tr>
                                </table>
                        </s:iterator>
                        <s:iterator value="exportList" id="tuple">
                            <table>
                                <tr>
                                    <td width="300" align="center">
                                        <s:property value="achievementName"/>
                                    </td>
                                    <td width="75" align="center">
                                        <s:property value="presonName"/>
                                    </td>
                                    <td width="75" align="center">
                                        <s:property value="date"/>
                                    </td>
                                    <td width="300" align="center">
                                        <s:property value="detail"/>
                                    </td>
                                    <td width="20" align="center">
                                        <s:url id="url" method="remove">
                                            <s:param name="key" value="#tuple.key"/>
                                        </s:url>
                                        <s:a href="%{url}">移除</s:a>
                                        </td>
                                        <td width="15" align="center">
                                        <s:url id="url" method="moveUp">
                                            <s:param name="key" value="#tuple.key"/>
                                        </s:url>
                                        <s:a href="%{url}">上移</s:a>
                                        </td>
                                        <td width="15" align="center">
                                        <s:url id="url" method="moveDown">
                                            <s:param name="key" value="#tuple.key"/>
                                        </s:url>
                                        <s:a href="%{url}">下移</s:a>
                                        </td>
                                    </tr>
                                </table>
                        </s:iterator>
                    </td>
                </tr>
            </table>
            <s:submit value="导出" method="export"/>
        </s:form>
    </body>
</html>