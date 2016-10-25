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
        <LINK rel=stylesheet type="text/css" href="index.css" />
    </head>

    <body>
        <div class="container">
            <div class="header">
                <table width="1150"  border="0" cellspacing="0">
                    <tr>
                        <td align="center" valign="center"><img src="_images/logo_cool.png" width="1100" height="130" border="0" /></td>
                    </tr>
                    <tr>
                        <td height="20" valign="top">
                            <table width="1150" border="0" cellspacing="0">

                                <tr>
                                    <td valign="top"><span class="headerspan">
                                            <s:url id="userdoc" action="index" method="toUserdoc"/>
                                            <s:a href="%{userdoc}">用户管理</s:a>
                                            </span></td>
                                        <td valign="top"><span class="headerspan">
                                            <s:url id="roleauth" action="index" method="toRoleauth" />
                                            <s:a href="%{roleauth}">角色权限管理</s:a>
                                            </span></td>
                                        <td valign="top"><span class="headerspan">
                                            <s:url id="achievement" action="index" method="toAchievement" />
                                            <s:a href="%{achievement}">成果管理</s:a>
                                            </span></td>
                                        <td valign="top"><span class="headerspan">
                                            <s:url id="research" action="index" method="toResearch" />
                                            <s:a href="%{research}">教学改革与科研训练管理</s:a>
                                            </span></td>
                                        <td valign="top"><span class="headerspan">
                                            <s:url id="course" action="index" method="toCourse" />
                                            <s:a href="%{course}">课程管理</s:a>
                                            </span></td>
                                        <td valign="top"><span class="headerspan">
                                            <s:url id="talents" action="index" method="toTalents" />
                                            <s:a href="%{talents}">人才培养</s:a>
                                            </span></td>
                                        <td valign="top"><span class="headerspan">
                                            <s:url id="academic" action="index" method="toAcademic" />
                                            <s:a href="%{academic}">学术交流</s:a>
                                            </span></td>
                                        <td valign="top"><span class="headerspan">
                                            <s:url id="document" action="index" method="toDocument" />
                                            <s:a href="%{document}">文件文档管理</s:a>
                                            </span></td>
                                    </tr>
                                </table>
                            </td>
                        </tr>
                    </table>

                </div>

                <div class="sidebar1">
                    <ul class="nav">
                        <form action="index">
                        <s:if test="#session.user==null">
                            <div align="center" style="color:yellow; font-size: small">
                                <s:property value="userInformation"/><br/>
                            </div><br/>
                            <div align="center">
                                用户:<s:textfield name="userId" theme="simple"/><br />
                                密码:<s:password name="password" theme="simple"/><br /><br />
                                <s:submit value="登录" method="login" theme="simple"/><br />
                            </div>
                            <hr/>
                        </s:if>
                        <s:else>
                            <br />
                            <li><strong>
                                    欢迎您，<s:property value="#session.user.name"/>！
                                </strong></li>
                            <li>
                                <s:url id="url" action="message" method="diliver" />
                                <s:a href="%{url}">收件箱(<s:property value="unreadCount"/>)</s:a>
                                </li>
                                <li>
                                <s:url id="url" action="message" method="send" />
                                <s:a href="%{url}">发件箱</s:a>
                                </li>

                            <s:if test="#session.user!=null && #session.menuName!='message' && !listView.isEmpty()">
                                <s:set name="exist" value="'no'"/>
                                <li><strong>菜单目录</strong></li>
                                <s:iterator value="listView" id="id">
                                    <s:set name="linker" value="getListViewLink(#id)"/>
                                    <s:if test="allowAccess(getMenuName(),#linker)=='yes'">
                                        <s:set name="exist" value="'yes'"/>
                                        <li>
                                            <strong>
                                                <s:url id="url" action="%{#linker}" method="init">
                                                    <s:param name="urlMenuName" value="menuName"/>
                                                    <s:param name="urlMenuItem" value="#linker"/>
                                                </s:url>
                                                <s:a href="%{url}">&nbsp;&nbsp;<s:property value="getListViewName(#id)"/><s:if test="isRecommended(#id)">&nbsp;<img src="_images/click.gif" width="30" height="15" valign="middle" border="0" /></s:if></s:a>
                                            </strong>
                                        </li>
                                    </s:if>
                                </s:iterator>
                                <s:if test="#exist=='no'">
                                    <li>
                                        <无可以授权使用的功能>
                                    </li>
                                </s:if>
                            </s:if>

                            <br />
                            <li>
                                <strong>
                                    <s:url id="url" action="index" method="logoff" />
                                    <s:a href="%{url}">注销</s:a>
                                    </strong>
                                </li>

                        </s:else>

                    </form>
                </ul>
            </div>

            <div class="content">
                <s:if test="#session.user!=null">
                    <div style="color:red" align="center">
                        <s:if test="msgTitle.length()!=0">
                            <s:property value="msgTitle"/><br/>
                        </s:if>
                        <s:if test="msgInformation.length()!=0">
                            <s:property value="msgInformation"/><br/>
                        </s:if>
                    </div>
                    <s:if test="#session.menuName=='message'">
                        <s:include value="message.jsp"/>
                    </s:if><s:elseif test="#session.menuName!=null && #session.menuItem!=null">
                        <s:set name="url" value='#session.menuName+"/"+#session.menuItem+".jsp"'/>
                        <s:include value="%{url}"/>
                    </s:elseif><s:else>
                        <s:include value="welcome.jsp"/>
                    </s:else>
                </s:if><s:else>
                    <s:include value="welcome.jsp"/>
                </s:else>
            </div>

            <div class="footer">
                <p>版权所有<span class="footerchar">南京理工大学计算机科学与技术学院</span>2009级软件工程 崔嵬、周宇哲、刘铁俊、娄明明</p>
            </div>
        </div>
    </body>
</html>