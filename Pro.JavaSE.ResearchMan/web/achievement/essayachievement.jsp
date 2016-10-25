<%--
    Document   : essayachievement
    Created on : 2011-9-9, 11:22:33
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
            var o=document.getElementById("essayachievement_style");
            if(o.value==""){
                traget1.style.display="none";
                traget2.style.display="none";
            }else if (o.value=="论文"){
                traget1.style.display="";
                traget2.style.display="none";
            }else{
                traget1.style.display="none";
                traget2.style.display="";
            }
            //o=document.getElementById("essayachievement_firstAuthorName");
        }
    </script>
    <body onload="ChangeTable()">
        <s:form action="essayachievement" theme="simple">
            <s:div align="center">
                <h1>论文专著录入</h1>
            </s:div>
            <table width="850" cellpadding="0" cellspacing="0" border="1" align="center">
                <col width="56" span="12" />
                <tr>
                    <td colspan="3" width="168">
                        *论文（著）题目<br/>
                    </td>
                    <td colspan="5" width="30">
                        <s:textfield name="title" size="50" maxLength="150"/>
                    </td>
                    <td width="56">*文体</td>
                    <td width="56">
                        <s:select list="{'','论文','论著','教材'}" name="style" onChange="ChangeTable()"/>
                    </td>
                    <td width="56">类别</td>
                    <td width="56">
                        <s:select list="{'','社会科学','自然科学','工程应用','其他'}" name="type"/>
                    </td>
                </tr>
                <tr>
                    <td colspan="2" rowspan="2" width="112">论文（著）第一作者</td>
                    <td width="56" align="right">姓名</td>
                    <td colspan="2" width="112">
                        <s:select list="persons" name="firstAuthorName0" value="2"/>
                    </td>
                    <td colspan="2" width="112">
                        <s:submit value="确定" method="submitName"/>
                    </td>
                    <td width="320">
                        *姓名：<s:textfield name="firstAuthorName" readonly="true" size="10"/>
                        &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
                        &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
                        &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
                        &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
                        职别:
                    </td>
                    <td width="56">
                        <s:textfield name="job" readonly="true" size="10"/>
                    </td>
                    <td width="56">电话</td>
                    <td colspan="2" width="112">
                        <s:textfield name="mobilePhone"  readonly="true" size="15"/>
                    </td>
                </tr>
                <tr>
                    <td colspan="10" width="560">
                        单位：<s:textfield name="firstWorkingSection" readonly="true" size="35"/>
                    </td>
                </tr>
                <tr>
                    <td colspan="2" rowspan="3" width="112">论文其他作者情况</td>
                    <td colspan="2" width="112">姓名</td>
                    <td colspan="4" width="208">单位</td>
                    <td colspan="2" width="168">职别</td>
                    <td colspan="2" width="112">
                        <s:submit method="delAuthor" value="重置"/>
                    </td>
                </tr>
                <tr>
                    <td colspan="10" width="600">
                        <s:iterator value="authorList">
                            <table width="650" align="center">
                                <tr>
                                    <td colspan="2" width="150">
                                        <s:property value="name"/>
                                    </td>
                                    <td colspan="7" width="600">
                                        <s:property value="workDep"/>
                                    </td>
                                    <td colspan="1" width="100">
                                        <s:property value="job"/>
                                    </td>
                                </tr>
                            </table>
                        </s:iterator>
                    </td>
                </tr>
                <tr>
                    <td colspan="2" width="112">添加作者：</td>
                    <td colspan="3" width="168">
                        <s:select list="persons" name="authorName"/>
                    </td>
                    <td colspan="3" width="168"></td>
                    <td colspan="2" width="112">
                        <s:submit value="添加" method="addAuthor"/>
                    </td>
                </tr>
            </table>
            <table width="850" cellpadding="0" cellspacing="0" border="1" id="table1" align="center">
                <col width="56" span="12" />
                <tr>
                    <td rowspan="13" width="56">论文发表情况</td>
                    <td rowspan="8" width="97">刊物发表</td>
                    <td colspan="5" rowspan="2">
                        刊物名称<s:textfield name="periodicalName" size="40" maxLength="150"/>
                    </td>
                    <td colspan="5" rowspan="2">
                        <s:select list="{'','国际刊物','国内刊物'}" name="foreignPeriodical"/>
                    </td>
                </tr>
                <tr> </tr>
                <tr>
                    <td colspan="2" rowspan="2">主办单位：</td>
                    <td colspan="3" rowspan="2">
                        <s:textfield name="periodicalHolder" size="35" maxLength="150"/>
                    </td>
                    <td colspan="2" rowspan="2">刊 号</td>
                    <td width="97">ISSN</td>
                    <td colspan="2" width="184">
                        <s:textfield name="periodicalIssnNo" size="15" maxLength="150"/>
                    </td>
                </tr>
                <tr>
                    <td width="97">CN</td>
                    <td colspan="2" width="184">
                        <s:textfield name="periodicalCnNo" size="15" maxLength="150"/>
                    </td>
                </tr>
                <tr>
                    <td colspan="5">基金（计划）资助项目</td>
                    <td colspan="2" rowspan="4">发表<br/>时间</td>
                    <td colspan="3" rowspan="4">
                        <s:textfield name="periodicalPublish" size="10" maxLength="150"/>
                    </td>
                </tr>
                <tr>
                    <td colspan="2">项目名称</td>
                    <td colspan="2">项目号</td>
                    <td width="146">
                        <s:submit method="delFund" value="重置"/>
                    </td>
                </tr>
                <tr>
                    <td colspan="5" width="450">
                        <s:iterator value="fundList">
                            <table  align="center">
                                <tr>
                                     <td colspan="2"  width="166">
                                        <s:property value="name"/>
                                    </td>
                                    <td colspan="2"  width="146">
                                        <s:property value="id"/>
                                    </td>
                                    <td width="146"></td>
                                </tr>
                            </table>
                        </s:iterator>
                    </td>
                </tr>
                <tr>
                    <td colspan="2">
                        <s:textfield name="fundName" size="5" maxLength="150"/>
                    </td>
                    <td colspan="2">
                        <s:textfield name="fundNo" size="5" maxLength="150"/>
                    </td>
                    <td width="146">
                        <s:submit method="addFund" value="添加"/>
                    </td>
                </tr>
                <tr>
                    <td rowspan="3" width="97">会议发表</td>
                    <td width="126">会议名称：</td>
                    <td colspan="3">
                        <s:select list="meetings" name="meetingName"/>
                        <%--s:textfield name="meetingName"/--%>
                    </td>
                    <td colspan="1">
                        <s:select list="{'','国际','国内'}" name="foreignMeeting"/>
                    </td>
                    <td colspan="2">时间</td>
                    <td colspan="3">
                        <jscalendar:head/>
                        <jscalendar:jscalendar name="meetingTime" format="%Y-%m-%d" showstime="true" size="6"/>
                    </td>
                </tr>
                <tr>
                    <td rowspan="2" width="126">主办单位</td>
                    <td colspan="3" rowspan="2">
                        <s:textfield name="meetingHolder" size="5" maxLength="150"/>
                    </td>
                    <td colspan="1" rowspan="2"></td>
                    <td colspan="2">地点</td>
                    <td colspan="3">
                        <s:textfield name="meetingPlace" size="5" maxLength="150"/>
                    </td>
                </tr>
                <tr>
                    <td colspan="3">论文集正式出版号</td>
                    <td colspan="2">
                        <s:textfield name="meetingIssnNo" size="5" maxLength="150"/>
                    </td>
                </tr>
                <tr>
                    <td colspan="11">*摘要：（限200字以内，此栏很重要请务必填写，否则不予登记）</td>
                </tr>
                <tr>
                    <td colspan="11">
                        <s:textarea name="memo" cols="100" maxLength="200"/>
                    </td>
                </tr>
            </table>
            <table width="850" height="65" cellpadding="0" cellspacing="0" border="1" id="table2"  align="center">
                <col width="56" span="12" />
                <tr>
                    <td rowspan="4" width="56">论著（教材）发表情况</td>
                    <td colspan="3" rowspan="2">*论著（教材）出版社名称：</td>
                    <td colspan="3" rowspan="2">
                        <s:textfield name="treatisePublisher" size="25" maxLength="150"/>
                    </td>
                    <td colspan="2" rowspan="2">论著（教材）总字数（千字）</td>
                    <td colspan="3" rowspan="2" width="167">
                        <s:textfield name="treatiseTotalKWord" size="25" maxLength="150"/>
                    </td>
                </tr>
                <tr> </tr>
                <tr>
                    <td colspan="3" rowspan="2">*书号：</td>
                    <td colspan="3" rowspan="2">
                        <s:textfield name="treatiseBookNo" size="25" maxLength="150"/>
                    </td>
                    <td colspan="2" rowspan="2">本人完成字数（千字）</td>
                    <td colspan="3" rowspan="2" width="167">
                        <s:textfield name="treatiseSelfKWord" size="25" maxLength="150"/>
                    </td>
                </tr>
                <tr> </tr>
            </table>
            <s:div align="center">
                <s:submit value="提交" method="submit"/>
            </s:div>
        </s:form>
    </body>
</html>