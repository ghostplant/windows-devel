<%--
    Document   : index
    Created on : 2011-8-30, 11:10:42
    Author     : zyz
--%>

<%@page contentType="text/html" pageEncoding="UTF-8"%>
<%@taglib prefix="s" uri="/struts-tags" %>
<!DOCTYPE html>
<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <title>教学科研成果管理系统</title>

        <style type="text/css">
            .nav a{text-decoration:none;color:white;}
            .nav a:hover{text-decoration:none;color:red;}
            .nav a:link,a:actived,a:visited{text-decoration:none;}
            .lst a{text-decoration:none;color:blue;}
            .lst a:hover{text-decoration:none;color:red;}
            .lst a:link,a:actived,a:visited{text-decoration:none;}
            .com a{text-decoration:none;color:black;}
            .com a:hover{text-decoration:none;color:black;}
            .com a:link,a:actived,a:visited{text-decoration:none;}
        </style>
    </head>

    <body class="lst"  style=" font-size: 12px">
        <div width="1000" align="center">

            <table width="1000"  border="0" cellspacing="0">
                <tbody>
                    <tr>
                        <td height="100" align="center" valign="center">教学科研成果管理系统 - 图片</td>
                    </tr>
                    <tr>
                        <td height="20" valign="top">
                            <table width="998" border="0" cellspacing="0">
                                <tbody>
                                    <tr>
                                        <td valign="top">
                                            <s:url id="userdoc" action="index" method="toUserdoc"/>
                                            <s:a href="%{userdoc}">用户管理</s:a>
                                        </td>
                                        <td valign="top">
                                            <s:url id="roleauth" action="index" method="toRoleauth" />
                                            <s:a href="%{roleauth}">角色权限管理</s:a>
                                        </td>
                                        <td valign="top">
                                            <s:url id="achievement" action="index" method="toAchievement" />
                                            <s:a href="%{achievement}">成果管理</s:a>
                                        </td>
                                        <td valign="top">
                                            <s:url id="research" action="index" method="toResearch" />
                                            <s:a href="%{research}">教学改革与科研训练管理</s:a>
                                        </td>
                                        <td valign="top">
                                            <s:url id="course" action="index" method="toCourse" />
                                            <s:a href="%{course}">课程管理</s:a>
                                        </td>
                                        <td valign="top">
                                            <s:url id="talents" action="index" method="toTalents" />
                                            <s:a href="%{talents}">人才培养</s:a>
                                        </td>
                                        <td valign="top">
                                            <s:url id="academic" action="index" method="toAcademic" />
                                            <s:a href="%{academic}">学术交流</s:a>
                                        </td>
                                        <td valign="top">
                                            <s:url id="document" action="index" method="toDocument" />
                                            <s:a href="%{document}">文件文档管理</s:a>
                                        </td>
                                    </tr>
                                </tbody>
                            </table>
                        </td>
                    </tr>
                </tbody>
            </table>
        </div>
        <table width="1000" align="center">
            <tbody>
                <tr height="50"><td></td></tr>
                <tr>
                    <td width="300" valign="top">
                        <div>
                            <s:form action="index">
                                <s:if test="#session.user==null">
                                    <s:textfield name="userId" label="用户"/>
                                    <s:password name="password" label="密码"/>
                                    <s:submit value="登录" method="login"/>
                                </s:if>
                                <s:else>
                                    <div>
                                        欢迎您，<s:property value="#session.user.name"/>！<br/><br/>
                                        <s:url id="url" action="message" method="diliver" />
                                        <s:a href="%{url}">收件箱(<s:property value="unreadCount"/>)</s:a> 
                                        <s:url id="url" action="message" method="send" />
                                        <s:a href="%{url}">发件箱</s:a><br/><br/>
                                        <s:submit theme="simple" value="注销" method="logoff"/>
                                        <br/><br/><br/>
                                        <s:if test="#session.user!=null && #session.menuName!='message' && !listMap.keySet().isEmpty()">
                                            <img src='_images/dir.jpg' border=0/>菜单目录<br/>
                                            <s:iterator value="listMap.keySet()">
                                                <s:set name="key"/>
                                                <s:url id="link" action="%{listMap.get(#key)}" method="init">
                                                    <s:param name="urlMenuName" value="menuName"/>
                                                    <s:param name="urlMenuItem" value="listMap.get(#key)"/>
                                                </s:url>
                                                <s:a href="%{link}">&nbsp;<img src='_images/item.jpg' border=0/><s:property value="#key"/></s:a><br/>
                                            </s:iterator>
                                        </s:if>
                                    </div>
                                </s:else>
                            </s:form>
                        </div>
                    </td>
                    <td width="692" valign="top">
                        <div style="color:tomato">
                            <s:property value="msgTitle"/><br/>
                            <s:property value="msgInformation"/><br/>
                        </div><br/>
                        <s:if test="#session.user!=null">
                            <s:if test="#session.menuName=='message'">
                                <s:include value="message.jsp"/>
                            </s:if><s:elseif test="#session.menuName!=null && #session.menuItem!=null">
                                <s:set name="url" value='#session.menuName+"/"+#session.menuItem+".jsp"'/>
                                <s:include value="%{url}"/>
                            </s:elseif>
                        </s:if>
                    </td>
                </tr>
            </tbody>
        </table>
    </body>
</html>