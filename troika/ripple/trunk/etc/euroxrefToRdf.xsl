<?xml version="1.0" encoding="UTF-8"?>

<!-- Apply this to the daily currency conversion table from:
         http://www.ecb.int/stats/eurofxref/eurofxref-daily.xml -->
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#" xmlns:currency="http://troika.dnsdojo.net/rdf/currency.ttl#" xmlns:gesmes="http://www.gesmes.org/xml/2002-08-01" xmlns:euroxref="http://www.ecb.int/vocabulary/2002-08-01/eurofxref">

	<xsl:output method="xml" indent="yes"/>

	<xsl:template match="/gesmes:Envelope/euroxref:Cube/euroxref:Cube">
		<rdf:RDF>
			<currency:Context rdf:ID="now">
				<xsl:attribute name="currency:date">
					<xsl:value-of select="@time"/>
				</xsl:attribute>
				<xsl:for-each select="euroxref:Cube">
					<xsl:element namespace="currency" name="{concat('currentRate_', @currency)}">
						<xsl:value-of select="@rate"/>
					</xsl:element>
				</xsl:for-each>
			</currency:Context>
		</rdf:RDF>
	</xsl:template>

</xsl:stylesheet>
