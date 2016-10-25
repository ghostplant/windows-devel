<%@ page language="java" import="java.util.*" pageEncoding="GBK"%>
<%@taglib prefix="s" uri="/struts-tags"%>
<%-- --%>
<html>
<body>
	<s:form action="myAction">
		<s:label name="message" />
		<s:textfield name="textbox" />
		<s:submit method="submit" value="Ìá½»" />
	</s:form>
</body>
</html>