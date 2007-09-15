<?xml version="1.0" encoding="UTF-8"?>

<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#">

	<xsl:output method="text"/>

	<xsl:template match="rdf:RDF">
		<xsl:for-each select="*//@rdf:ID">
			<xsl:sort select="."/>
			<xsl:value-of select="."/>
			<xsl:text>&#10;</xsl:text>
		</xsl:for-each>
		<xsl:text>&#10;</xsl:text>
		<xsl:for-each select="*//@rdf:about">
			<xsl:sort select="."/>
			<xsl:value-of select="."/>
			<xsl:text>&#10;</xsl:text>
		</xsl:for-each>
	</xsl:template>

</xsl:stylesheet>
