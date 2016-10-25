<%-- 
    Document   : message
    Created on : 2011-9-13, 23:02:02
    Author     : Ghostplant
--%>

<%@page contentType="text/html" pageEncoding="UTF-8"%>
<%@taglib prefix="s" uri="/struts-tags" %>


<body>
    <div align="center">
        <s:form action="message">
            <s:hidden name="diliver"/>

            <s:if test="diliver==1">
                <table width="690" border="1" cellspacing="0" cellpadding="0">
                    <tr>
                        <td colspan="6"><p align="center"><strong>收件箱</strong></p></td>
                    </tr>
                    <tr>
                        <td><p align="center" >发件人 </p></td>
                        <td><p align="center">发件时间 </p></td>
                        <td><p align="center">状态 </p></td>
                        <td><p align="center">内容 </p></td>
                        <td><p align="center">回执 </p></td>
                    </tr>
                    <s:iterator value="%{delievedLists}" id="mail">
                        <tr>
                            <td><p align="center"><s:property value="getTeacherName(#mail.srcTeacherId)"/></p></td>
                            <td><p align="center"><s:property value="castToString(#mail.date)"/></p></td>
                            <td><p align="center"><s:property value="castToString(#mail.readed)"/></p></td>
                            <td><p align="center"><s:property value="#mail.message"/></p></td>
                            <s:if test="#mail.readed==false">
                                <td><p align="center">
                                        <s:url id="url" method="hasBeenRead">
                                            <s:param name="readId" value="#mail.id"/>
                                        </s:url>
                                        <s:a href="%{url}" disabled='#mail.readed==false'>已阅</s:a>

                                        <s:url id="url" method="reply">
                                            <s:param name="replyId" value="#mail.id"/>
                                        </s:url>
                                        <s:a href="%{url}" disabled='#mail.readed==false'>回复</s:a>
                                        </p>
                                    </td>
                            </s:if>
                            <s:else><td></td></s:else>

                            </tr>
                    </s:iterator>
                </table>
            </s:if>
            <s:if test="diliver==2">
                <table width="690" border="1" cellspacing="0" cellpadding="0">
                    <tr>
                        <td colspan="4"><p align="center"><strong>发件箱</strong></p></td>
                    </tr>
                    <tr>
                        <td><p align="center">收件人 </p></td>
                        <td><p align="center">发件时间 </p></td>
                        <td><p align="center">状态 </p></td>
                        <td><p align="center">内容 </p></td>
                    </tr>
                    <s:iterator value="%{sendedLists}" id="mail">
                        <tr>
                            <td><p align="center"><s:property value="getTeacherName(#mail.dstTeacherId)"/></p></td>
                            <td><p align="center"><s:property value="castToString(#mail.date)"/></p></td>
                            <td><p align="center"><s:property value="castToString(#mail.readed)"/></p></td>
                            <td><p align="center"><s:property value="#mail.message"/></p></td>
                        </tr>
                    </s:iterator>
                </table>
                <p align="center">
                    <s:url id="url" method="write"></s:url>
                    <s:a href="%{url}">写新站内信</s:a>
                    </p>
            </s:if>
            <s:if test="diliver==3">
                <s:form theme="simple" action="message">
                    <table>
                        <tr><td>收件人：</td><td><s:select list="users" name="selectedUser"/></td></tr>
                        <tr><td>内容(限255字以内)：</td><td><s:textarea  name="content" theme="simple" rows="20" cols="60"/></td></tr>
                        <tr><td></td><td><s:submit value="发送" method="sendMessages" theme="simple"/></td></tr>
                    </table>
                </s:form>
            </s:if>
        </s:form>
    </div>
</body>

