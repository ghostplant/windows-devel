<%-- 
    Document   : download
    Created on : 2011-9-19, 16:30:55
    Author     : Administrator
--%>

<%@page contentType="text/html" pageEncoding="UTF-8"%>
<%@taglib prefix="s" uri="/struts-tags" %>


<body>

    <s:form action="researcher" theme="simple">

        <table align="center" valign="center" border="1" cellspacing="0" cellpadding="0">
            <tr >
                <td valign="center" colspan="18"align="center" style="background-color: rgb(255, 255, 51)"><p ><strong>本一级学科点现有在编人员结构 </strong></p></td>
            </tr>
            <tr style="background-color:antiquewhite">
                <td valign="center" colspan="3" align="center"><p >专业技术职务 </p></td>
                <td valign="center" colspan="2" align="center"><p >35岁 </p>
                    <p >以下 </p></td>
                <td valign="center" colspan="2" align="center"><p >36至 </p>
                    <p >45岁 </p></td>
                <td width="54" valign="center" align="center"><p >46至 </p>
                    <p >55岁 </p></td>
                <td width="68" valign="center" align="center"><p >56至 </p>
                    <p >60岁 </p></td>
                <td valign="center" colspan="2" align="center"><p >61岁 </p>
                    <p >以上 </p></td>
                <td valign="center" colspan="2" align="center"><p >人数合计 </p></td>
                <td valign="center" colspan="2" align="center"><p >具有博士学位人数 </p></td>
                <td valign="center" colspan="3" align="center"><p >具有硕士学位人数 </p></td>
            </tr>

            <tr >
                <td valign="center" colspan="3" align="center" style="background-color: rgb(153, 255, 255)"><p >教授（或相当专业技术职务者）</p></td>
                <td valign="center" colspan="2" align="center"><p > <s:property value="a11"/></p></td>
                <td valign="center" colspan="2" align="center"><p > <s:property value="a12"/></p></td>
                <td width="54" valign="center" align="center"><p > <s:property value="a13"/></p></td>
                <td width="68" valign="center" align="center"><p > <s:property value="a14"/></p></td>
                <td valign="center" colspan="2" align="center"><p > <s:property value="a15"/></p></td>
                <td valign="center" colspan="2" align="center"><p ><s:property value="a11+a12+a13+a14+a15"/> </p></td>
                <td valign="center" colspan="2" align="center" rowspan="3" ><p ><s:property value="doctorNumber"/></p></td>
                <td valign="center" colspan="3" align="center" rowspan="3" ><p ><s:property value="masterNumber"/> </p></td>
            </tr>
            <tr >
                <td valign="center" colspan="3" align="center"style="background-color: rgb(153, 255, 255)" ><p >副教授（或相当专业技术职务者） </p></td>
                <td valign="center" colspan="2" align="center"><p ><s:property value="a21"/> </p></td>
                <td valign="center" colspan="2" align="center"><p ><s:property value="a22"/> </p></td>
                <td width="54" valign="center" align="center"><p ><s:property value="a23"/> </p></td>
                <td width="68" valign="center" align="center"><p ><s:property value="a24"/> </p></td>
                <td valign="center" colspan="2" align="center"><p ><s:property value="a25"/> </p></td>
                <td valign="center" colspan="2" align="center"><p ><s:property value="a21+a22+a23+a24+a25"/></p></td>
            </tr>
            <tr >
                <td valign="center" colspan="3" align="center"style="background-color: rgb(153, 255, 255)"><p >讲师（或相当专业技术职务者） </p></td>
                <td valign="center" colspan="2" align="center"><p ><s:property value="a31"/> </p></td>
                <td valign="center" colspan="2" align="center"><p ><s:property value="a32"/> </p></td>
                <td width="54" valign="center" align="center"><p ><s:property value="a33"/> </p></td>
                <td width="68" valign="center" align="center"><p ><s:property value="a34"/> </p></td>
                <td valign="center" colspan="2" align="center"><p ><s:property value="a35"/> </p></td>
                <td valign="center" colspan="2" align="center"><p ><s:property value="a31+a32+a33+a34+a35"/> </p></td>
            </tr>
            <tr >
                <td valign="center" colspan="18" align="center" style="background-color: rgb(255, 255, 51)"><p ><strong>本一级学科点的学科方向及其学术带头人、主要学术骨干 </strong></p></td>
            </tr>
            <tr style="background-color:antiquewhite">
                <td width="36" valign="center" rowspan="2" align="center"><p >学科方向 </p></td>
                <td width="44" valign="center" rowspan="2" align="center"><p >姓名 </p></td>
                <td valign="center" colspan="2" rowspan="2" align="center"><p >出生 </p>
                    <p >年月 </p></td>
                <td valign="center" colspan="2" rowspan="2" align="center"><p >获 </p>
                    <p >博士学 </p>
                    <p >位年月 </p></td>
                <td valign="center"  colspan="4" rowspan="2" align="center"><p >专业技术职务 </p>
                    <p >及专家称谓 </p></td>
                <td valign="center" colspan="4" align="center"><p >培养博士生 </p></td>
                <td valign="center" colspan="3" align="center"><p >培养硕士生 </p></td>
            </tr>
            <tr style="background-color:antiquewhite">
                <td valign="center" colspan="2" align="center"><p >近五年获 </p>
                    <p >学位人数 </p></td>
                <td valign="center" colspan="2" align="center"><p >在学人数 </p></td>
                <td valign="center" colspan="2" align="center"><p >近五年获 </p>
                    <p >学位人数 </p></td>
                <td width="136" valign="center" align="center"><p >在学人数 </p></td>
            </tr>

            <s:iterator value="getMajors()" id="key">

                
                
                    <s:iterator value="getResearcherSet(#key) " id="teacher">
                    <tr >
                        <td width="16" valign="center" align="center" style="background-color: rgb(153, 255, 255)"><p ><s:property value="#key"/></p></td>
                        <td width="64" valign="center" align="center" style="background-color: rgb(153, 255, 255)"><p ><s:property value="#teacher.getName()"/></p></td>
                        <td valign="center" colspan="2" align="center"><p ><s:property value="getBirthDate(#teacher)"/></p></td>
                        <td valign="center" colspan="2" align="center"><p ><s:property value="getGraduationDate(#teacher)"/></p></td>
                        <td valign="center" colspan="4" align="center"><p ><s:property value="#teacher.getExpertTitle()"/></p></td>
                        <td valign="center" colspan="2" align="center"><p ><s:property value="getGraDocNum(#teacher.getId())"/></p></td>
                        <td valign="center" colspan="2" align="center"><p ><s:property value="getStuDocNum(#teacher.getId())"/></p></td>
                        <td valign="center" colspan="2" align="center"><p ><s:property value="getGraMasNum(#teacher.getId())"/> </p></td>
                        <td width="136" valign="center" align="center"><p ><s:property value="getStuMasNum(#teacher.getId())"/></p></td>
                    </tr>
                </s:iterator>
            

        </s:iterator>
    </table>
    <p align="right"><s:submit theme="simple" value="download" method="export"/></p>

</s:form>
</body>
