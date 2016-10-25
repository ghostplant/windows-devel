<%-- 
    Document   : documentsmanager
    Created on : 2011-9-18, 21:20:58
    Author     : Ghostplant
--%>

<%@page contentType="text/html" pageEncoding="UTF-8"%>
<%@taglib prefix="s" uri="/struts-tags" %>

<body>
    <s:form action="documentsmanager">
        <table border="1" cellspacing="0" cellpadding="0" align="center">
            <tbody>
                <tr style="background-color:skyblue" width="690">
                    <td width="500">文档名称</td>
                    <td width="92">操作</td>
                </tr>
                <s:iterator value="%{recordList}" status="st" id="id">
                    <s:iterator value="recordList[#st.index]">
                        <tr align="center" <s:if test="#st.odd">style="background-color:gainsboro"</s:if> >
                            <td width="500">
                                <s:url id="url" method="download">
                                    <s:param name="selectedFile" value='#id.name'/>
                                </s:url>
                                <s:a href="%{url}"><s:property value="decode(#id.name)"/></s:a>
                            </td>
                            <td width="92">
                                <s:url id="url" method="delete">
                                    <s:param name="selectedFile" value='#id.name'/>
                                </s:url>
                                <s:a href="%{url}">删除</s:a>
                            </td>
                        </tr>
                    </s:iterator>
                </s:iterator>
            </tbody>
        </table>
    </s:form>
    <div align="center">
        <s:form action="documentsmanager" method="POST" enctype="multipart/form-data">
            <s:file name="uploadFile" theme="simple"/>
            <s:submit value="上传文件" method="upload" theme="simple"/>
        </s:form>
    </div>
</body>
