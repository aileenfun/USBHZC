<?xml version="1.0" encoding="UTF-8"?><!-- DWXMLSource="NEW_PROD-REV1-DEV.ldat" -->
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:template match="/">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
<title>ON Semiconductor <xsl:value-of select="sensor/@part_number"/>-<xsl:value-of select="sensor/@version_name"/></title>
</head>
<body>
  <div style="float:left;display:block;font-weight:normal;font-family:Geneva, Arial, Helvetica, sans-serif;">
	<h1 style="font-size:20px;line-height:20px;">
	<span style="vertical-align:super">
       <xsl:text>&#160;</xsl:text>ON Semiconductor<xsl:text>&#160;&#160;&#160;&#160;</xsl:text><xsl:value-of select="concat(sensor/@part_number,'&#160;&#160;')"/><xsl:value-of select="concat('&#160;&#160;',sensor/@version_name)"/>
	</span></h1>
   	
    <h2 style="font-size:16px;line-height:16px;">&#160;Register/Bitfield Long Descriptions</h2>
    <p style="font-size:11px;line-height:11px;">&#160;Note that items displayed in <font color="#FF0000"><b>Red</b></font> indicate their contents are Confidential, items displayed in <font color="#000000"><b>Black</b></font> indicate their contents are NDA, and items displayed in <font color="#009933"><b>Green</b></font> indicate their contents are Public.</p>
	<table border="0" cellspacing="0" cellpadding="2" style="font-size:11px;">
 	<xsl:call-template name="Display_Reg_Header"/>
 	<xsl:call-template name="Display_Reg_Row">
		<xsl:with-param name="Reg_Nodes" select="sensor/registers/reg"/>
    </xsl:call-template>
    </table><br /><br />
  	  
<br /></div>
</body>
</html>
</xsl:template>


<xsl:template name="Display_Reg_Header">
      <tr bgcolor="#9acd32" align="left">
        <th style="border:solid; border-color:#E0E0E0; border-width:1px">Register Name</th>
        <th style="border:solid; border-color:#E0E0E0; border-width:1px">Long Description</th>
		<xsl:if test="sensor/registers/reg/conf_notes or sensor/registers/reg/bitfield/conf_notes">
			<th style="border:solid; border-color:#E0E0E0; border-width:1px">Confidential Notes</th>
		</xsl:if>
      </tr>
</xsl:template>

<xsl:template name="Display_Reg_Row">
   <xsl:param name="Reg_Nodes"/>
   <xsl:for-each select="$Reg_Nodes">
      <xsl:choose>
         <xsl:when test="@confidential != ''">
			<xsl:choose>
				<xsl:when test="@confidential = 'Y'">
					<xsl:call-template name="Format_Red_Row"/>
				</xsl:when>
				<xsl:otherwise>
				   <xsl:call-template name="Format_Green_Row"/>
				</xsl:otherwise>
			</xsl:choose>			
         </xsl:when>
         <xsl:otherwise>
           <xsl:call-template name="Format_Black_Row"/>
         </xsl:otherwise>
      </xsl:choose>
      <xsl:call-template name="Display_Bit_Row">
	     <xsl:with-param name="Bit_Nodes" select="bitfield"/>
      </xsl:call-template>
   </xsl:for-each>
</xsl:template>

<xsl:template name="Display_Bit_Row">
   <xsl:param name="Bit_Nodes"/>  
   <xsl:for-each select="$Bit_Nodes">
      <xsl:choose>
         <xsl:when test="@confidential != ''">
			<xsl:choose>
				<xsl:when test="@confidential = 'Y'">
					<xsl:call-template name="Format_Red_Bit_Row"/>
				</xsl:when>
				<xsl:otherwise>
				   <xsl:call-template name="Format_Green_Bit_Row"/>
				</xsl:otherwise>
			</xsl:choose>			
         </xsl:when>
         <xsl:otherwise>
           <xsl:call-template name="Format_Black_Bit_Row"/>
         </xsl:otherwise>
      </xsl:choose>
   </xsl:for-each>
</xsl:template>

<xsl:template name="Format_Red_Row">
  <tr style="color:#FF0000;">
    <xsl:call-template name="Display_Reg_Cells"/>
  </tr>
</xsl:template>

<xsl:template name="Format_Red_Bit_Row">
  <tr style="color:#FF0000; background-color:#FFFFFF">
    <xsl:call-template name="Display_Bit_Cells"/>
  </tr>
</xsl:template>

<xsl:template name="Format_Black_Row">
  <tr style="color:#000000">
    <xsl:call-template name="Display_Reg_Cells"/>
  </tr>
</xsl:template>

<xsl:template name="Format_Black_Bit_Row">
  <tr style="color:#000000">
    <xsl:call-template name="Display_Bit_Cells"/>
  </tr>
</xsl:template>

<xsl:template name="Format_Green_Row">
  <tr style=" color:#009933; background-color:#FFFFFF">
    <xsl:call-template name="Display_Reg_Cells"/>
  </tr>
</xsl:template>

<xsl:template name="Format_Green_Bit_Row">
  <tr style=" color:#009933; background-color:#FFFFFF">
    <xsl:call-template name="Display_Bit_Cells"/>
  </tr>
</xsl:template>

<xsl:template name="print_Words">
	<xsl:param name="word"/>
		<xsl:choose>
				<xsl:when test="contains($word, 'http://') or contains($word, 'https://') or contains($word, 'ftp://')">
						<a href="{$word}"><xsl:value-of select="$word"/></a>
						<xsl:text>&#160;</xsl:text>
				</xsl:when>
				<xsl:otherwise>
					<xsl:value-of select="$word"/>
					<xsl:text>&#160;</xsl:text>
				</xsl:otherwise>
			</xsl:choose>
</xsl:template>

<xsl:template name="tokenizeString">
	<xsl:param name="list"/>
	<xsl:param name="delimiter"/>
	
	<xsl:choose>
		<xsl:when test="contains($list, $delimiter)">
			<xsl:call-template name="print_Words">
				<xsl:with-param name="word" select="substring-before($list,$delimiter)"/>
			</xsl:call-template>
			<xsl:call-template name="tokenizeString">
				<xsl:with-param name="list" select="substring-after($list,$delimiter)"/>
				<xsl:with-param name="delimiter" select="$delimiter"/>
			</xsl:call-template>
		</xsl:when>
		<xsl:otherwise>
			<xsl:choose>
				<xsl:when test="$list = ''">
					<xsl:call-template name="print_Words">
						<xsl:with-param name="word" select="$list"/>
					</xsl:call-template>
				</xsl:when>
				<xsl:otherwise>
					<xsl:call-template name="print_Words">
						<xsl:with-param name="word" select="$list"/>
					</xsl:call-template>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:otherwise>
	</xsl:choose>
</xsl:template> 

<xsl:template name="print_WordsByAnchor">
	<xsl:param name="word"/>
		
		<xsl:choose>
				
				<xsl:when test="contains($word, '&quot;&gt;') ">
						
						<a href="{substring-before($word,'&quot;&gt;')}"><xsl:value-of select="substring-after($word,'&quot;&gt;')"/></a>
						<xsl:text>&#160;</xsl:text>
				</xsl:when>
				<xsl:otherwise>
					<xsl:value-of select="$word"/>
					<xsl:text>&#160;</xsl:text>
				</xsl:otherwise>
			</xsl:choose>
</xsl:template>

<xsl:template name="tokenizeStringByAnchor">
	<xsl:param name="list"/>
	<xsl:param name="delimiter1"/>
	<xsl:param name="delimiter2"/>
	<xsl:param name="delimiter3"/>
	<xsl:choose>
		<xsl:when test="contains($list, $delimiter1) and contains($list, $delimiter2) and contains($list, $delimiter3)">
			<xsl:value-of select="substring-before($list,$delimiter1)"/>
			<xsl:call-template name="print_WordsByAnchor">
				<xsl:with-param name="word" select="substring-before(substring-after($list,$delimiter1),$delimiter3)"/>
			</xsl:call-template>
			<xsl:call-template name="tokenizeStringByAnchor">
				<xsl:with-param name="list" select="substring-after(substring-after($list,$delimiter1),$delimiter3)"/>
				
				<xsl:with-param name="delimiter1" select="$delimiter1"/>
				<xsl:with-param name="delimiter2" select="$delimiter2"/>
				<xsl:with-param name="delimiter3" select="$delimiter3 "/>
			</xsl:call-template>
		</xsl:when>
		<xsl:otherwise>
			<xsl:choose>
				<xsl:when test="$list = ''">
					<xsl:text>&#160;</xsl:text>
				</xsl:when>
				<xsl:otherwise>
					<xsl:value-of select="$list"/>
				</xsl:otherwise>
			</xsl:choose>
		</xsl:otherwise>
	</xsl:choose>
	
</xsl:template> 


<xsl:template name="Split_Words">
	<xsl:param name="description_value"/>
	<td wrap="wrap">
	<xsl:call-template name="tokenizeString">
		<xsl:with-param name="list" select="$description_value"/>
		<xsl:with-param name="delimiter" select="' '"/>										
	</xsl:call-template>
	</td>
	<!--<td wrap="wrap"><xsl:value-of select="$description_value"/></td>-->
</xsl:template>
	
<xsl:template name="Display_Reg_Cells">  
	<td><xsl:value-of select="@name"/></td>		
	<xsl:choose>
		<xsl:when test="long_desc != ''"> <!-- detail is a tag, not an attribute -->
			<td wrap="wrap"><xsl:value-of select="long_desc"/></td>
		</xsl:when>
		<xsl:otherwise>
			<td><xsl:text>&#160;&#160;</xsl:text></td>
		</xsl:otherwise>
	</xsl:choose>
	<xsl:choose>
		<xsl:when test="conf_notes != ''"> <!-- detail is a tag, not an attribute -->
			
			
		<xsl:choose>
				<xsl:when test="contains(conf_notes, '&lt;a href=&quot;') and contains(conf_notes, '&quot;&gt;') and contains(conf_notes, '&lt;/a&gt;')">
					<xsl:call-template name="tokenizeStringByAnchor">
						<xsl:with-param name="list" select="conf_notes"/>
						<xsl:with-param name="delimiter1" select="'&lt;a href=&quot;'"/>
						<xsl:with-param name="delimiter2" select="'&quot;&gt;'"/>
						<xsl:with-param name="delimiter3" select="'&lt;/a&gt;'"/>							
				</xsl:call-template>
				</xsl:when>
				<xsl:otherwise>
					<td wrap="wrap"><font color="red"><xsl:value-of select="conf_notes"/></font></td>
				</xsl:otherwise>
				
		</xsl:choose>
			
		</xsl:when>
		<xsl:otherwise>
			<td><xsl:text>&#160;&#160;</xsl:text></td>
		</xsl:otherwise>
	</xsl:choose>
</xsl:template>

<xsl:template name="Display_Bit_Cells">
	<td><xsl:text>&#160;&#160;</xsl:text><xsl:value-of select="concat('&#160;&#160;&#160;&#160;&#160;&#160;',@name)"/></td>
	<xsl:choose>
		<xsl:when test="long_desc != ''">
			<td wrap="wrap"><xsl:value-of select="long_desc"/></td>
		</xsl:when>
		<xsl:otherwise>
			<td><xsl:text>&#160;&#160;</xsl:text></td>
		</xsl:otherwise>
	</xsl:choose>
	<xsl:choose>
		<xsl:when test="conf_notes != ''"> <!-- detail is a tag, not an attribute -->
		<xsl:choose>
				<xsl:when test="contains(conf_notes, '&lt;a href=&quot;') and contains(conf_notes, '&quot;&gt;') and contains(conf_notes, '&lt;/a&gt;')">
					<xsl:call-template name="tokenizeStringByAnchor">
						<xsl:with-param name="list" select="conf_notes"/>
						<xsl:with-param name="delimiter1" select="'&lt;a href=&quot;'"/>
						<xsl:with-param name="delimiter2" select="'&quot;&gt;'"/>
						<xsl:with-param name="delimiter3" select="'&lt;/a&gt;'"/>		
					</xsl:call-template>
				</xsl:when>
				<xsl:otherwise>
					<td wrap="wrap"><font color="red"><xsl:value-of select="conf_notes"/></font></td>
				</xsl:otherwise>
				
		</xsl:choose>
			
		</xsl:when>
		<xsl:otherwise>
			<td><xsl:text>&#160;&#160;</xsl:text></td>
		</xsl:otherwise>
	</xsl:choose>
</xsl:template>

</xsl:stylesheet>


