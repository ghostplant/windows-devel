<%-- 
    Document   : adduser
    Created on : 2011-9-1, 16:43:16
    Author     : Ghostplant
--%>
<%@page contentType="text/html" pageEncoding="UTF-8"%>
<%@taglib  prefix="s" uri="/struts-tags" %>

<body>

    <form action="uploaduser" method="POST" enctype="multipart/form-data">
        &nbsp;&nbsp;第一步：下载模板 ：<s:submit theme="simple" value="下载模板" method="template"/>；<br/><br/>
        &nbsp;&nbsp;第二步：填写完整空余信息；<br/><br/>
        &nbsp;&nbsp;第三步：将一个或者多个填写完整的Excel文档打包成为zip压缩文档；<br/><br/>
        &nbsp;&nbsp;第四步：上传压缩文档：<s:file name="upload" theme="simple" /><s:submit theme="simple" value="开始上传"/>。
    </form>

    <s:if test="finished">
        <br/>
        <div align="center"><strong>批量注册结果：</strong></div><br/>
        <table border="1"  cellspacing="0" cellpadding="0" width="600" align="center">
            <tr>
                <td align="center"><strong>索引</strong></td>
                <td align="center"><strong>处理结果</strong></td>
            </tr>
            <s:iterator value="results" status="s">
                <tr bgcolor="<s:if test="#s.odd">pink</s:if>">
                    <td><s:property value="#s.count"/></td>
                    <td><s:property/></td>
                </tr>
            </s:iterator>
        </table>
        <br/>
        <div align="center" style=" color: mediumslateblue">
            <strong>
                成功注册统计:<s:label name="successCount" theme="simple"/>；
                注册冲突统计:<s:label name="failedCount" theme="simple"/>；
                文件错误统计:<s:label name="errorCount" theme="simple"/>。
            </strong>
        </div>
        <br/>
    </s:if>
</body>
