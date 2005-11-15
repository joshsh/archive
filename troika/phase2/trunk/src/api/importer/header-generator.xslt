<?xml version="1.0" encoding="utf-8"?>

<!--  -->
<xsl:stylesheet version="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:xslAlt="urn:dummy:namespace">

    <xsl:output method="text" />

    <xsl:template match="/doxygen">
        <xsl:for-each select="compounddef">
            <xsl:choose>
                <xsl:when test="count(sectiondef) > 0">
<xsl:text />/** \note  This is a generated file. */

extern &quot;C&quot;
{
    #include &quot;<xsl:value-of select="compoundname/text()" />&quot;<xsl:text />

                    <xsl:for-each select="sectiondef">
                        <xsl:for-each select="memberdef">
                            <xsl:call-template name="function-stub" />
                        </xsl:for-each>
                    </xsl:for-each>

}  // extern &quot;C&quot;

<xsl:text />
                </xsl:when>
            </xsl:choose>
        </xsl:for-each>
    </xsl:template>


    <!--<xsl:template match="//memberdef">-->
    <xsl:template name="function-stub">

        <xsl:variable name="function-name" select="./name/text()" />
        <xsl:variable name="return-type" select="./type/text()" />

    void *<xsl:call-template name="mangle-function-name">
         <xsl:with-param name="original-name" select="$function-name" />
    </xsl:call-template>( void **args )
    {
        <xsl:value-of select="$return-type" /> *ret;

        ret = <xsl:value-of select="$function-name" />(<xsl:text />

        <xsl:variable name="number-of-parameters" select="count(param)" />
        <xsl:for-each select="param"><xsl:text />
            <xsl:call-template name="parameter-cast">
                 <xsl:with-param name="type" select="type" />
                 <xsl:with-param name="array-index">
                     <xsl:value-of select="position() - 1" />
                 </xsl:with-param>
            </xsl:call-template>
            <xsl:choose>
                <xsl:when test="position() = $number-of-parameters">
                    <xsl:text />);  <xsl:text />
                </xsl:when>
                <xsl:otherwise>
                    <xsl:text />,   <xsl:text />
                </xsl:otherwise>
            </xsl:choose>
            <xsl:text />// <xsl:value-of select="type" />
            <xsl:value-of select="' '" />
            <xsl:value-of select="declname" />
        </xsl:for-each>

        return ( void* ) ret;
    }<xsl:text />


    </xsl:template>


    <!-- Create a (hopefully) unique function name. -->
    <xsl:template name="mangle-function-name">

        <xsl:param name="original-name" />
        <xsl:text />p2_imported__<xsl:value-of select="$original-name" />

    </xsl:template>

    <!-- Cast a parameter from an array element. -->
    <xsl:template name="parameter-cast">

        <xsl:param name="type" />
        <xsl:param name="array-index" />
            *(( <xsl:value-of select="$type" />* ) args[<xsl:value-of select="$array-index" />] )<xsl:text />

    </xsl:template>

</xsl:stylesheet>