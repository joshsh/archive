<?xml version="1.0" encoding="utf-8"?>

<!--  -->
<xsl:stylesheet version="1.0"
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:xslAlt="urn:dummy:namespace">

    <xsl:output method="text" />


    <!-- ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: -->


    <xsl:template match="/doxygen">

        <xsl:call-template name="stub-definitions" />
        <xsl:call-template name="p2__import_primitives" />

    </xsl:template>


    <!-- ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: -->


    <!-- Generate stub definitions section. -->
    <xsl:template name="stub-definitions">

<xsl:text />/** \note  This is an automatically generated file. */

extern &quot;C&quot;
{

<xsl:text />
        <xsl:for-each select="compounddef">
            <xsl:if test="count(sectiondef) > 0">
#include &quot;<xsl:text />
        <xsl:value-of select="compoundname/text()" />
        <xsl:text />&quot; <xsl:text />
        <xsl:call-template name="comment-bar">
            <xsl:with-param name="length" select="68 - string-length(compoundname/text())" />
        </xsl:call-template>

        <xsl:text>&#xa;</xsl:text>

        <!-- Add any includes from the source file. -->
        <xsl:for-each select="includes">
#include <xsl:text />
            <xsl:choose>
                <xsl:when test="@local = 'yes'">
                    <xsl:call-template name="enclose-in-quotes">
                        <xsl:with-param name="arg" select="text()" />
                    </xsl:call-template>
                </xsl:when>
                <xsl:otherwise>
                    <xsl:call-template name="enclose-in-angle-braces">
                        <xsl:with-param name="arg" select="text()" />
                    </xsl:call-template>
                </xsl:otherwise>
            </xsl:choose>
        </xsl:for-each>
        <xsl:if test="count(includes) > 0">
            <xsl:text>&#xa;</xsl:text>
        </xsl:if>


        <xsl:text>&#xa;</xsl:text>

        <!-- Create function stubs. -->
        <xsl:for-each select="sectiondef">
            <xsl:for-each select="memberdef">
                <xsl:call-template name="function-stub" />
            </xsl:for-each>
        </xsl:for-each>

            </xsl:if>
        </xsl:for-each>
////////////////////////////////////////////////////////////////////////////////


}  // extern &quot;C&quot;
<xsl:text />

    </xsl:template>


    <!-- ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: -->


    <xsl:template name="p2__import_primitives">

#include &quot;import-aux.h&quot;

#define RT  ( char ) 1  // Referentially transparent.
#define RO  ( char ) 0  // Referentially opaque.


int p2__import_primitives()
{
    p2_primitive *prim;
<xsl:text />

        <xsl:for-each select="compounddef">
            <xsl:if test="count(sectiondef) > 0">

        <!-- Construct and add p2_primitives. -->
        <xsl:for-each select="sectiondef">
            <xsl:for-each select="memberdef">
                <xsl:call-template name="register-primitive" />
            </xsl:for-each>
        </xsl:for-each>

            </xsl:if>
        </xsl:for-each>

<xsl:text />
    return 1;
}

<xsl:text />

    </xsl:template>


    <!-- ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: -->


    <!--<xsl:template match="//memberdef">-->
    <xsl:template name="function-stub">

        <xsl:variable name="function-name" select="./name/text()" />
        <xsl:variable name="return-type" select="./type/text()" />
        <xsl:variable name="is-struct" select="contains(./definition/text(), 'struct')" />
        <xsl:variable name="returns-pointer-type" select="contains($return-type, '*')" />

    <!-- Add original definition as a comment. -->
/** <xsl:text />
        <xsl:value-of select="definition" />
        <xsl:value-of select="argsstring" /> */<xsl:text />
void *<xsl:text />
        <xsl:call-template name="mangle-function-name">
            <xsl:with-param name="original-name" select="$function-name" />
        </xsl:call-template>( void **args )
{
    <xsl:text />
        <xsl:choose>
            <xsl:when test="$is-struct">struct <xsl:text /></xsl:when>
        </xsl:choose>
        <xsl:call-template name="depointerize">
            <xsl:with-param name="type" select="$return-type" />
        </xsl:call-template> *ret<xsl:text />
        <xsl:choose>
            <xsl:when test="$returns-pointer-type">;<xsl:text /></xsl:when>
            <xsl:otherwise> = ( <xsl:text />
                <xsl:value-of select="$return-type" />* ) malloc( sizeof( <xsl:text />
                <xsl:value-of select="$return-type" /> ));<xsl:text />
            </xsl:otherwise>
        </xsl:choose>

    <xsl:text>

    </xsl:text>

        <xsl:if test="not($returns-pointer-type)">*</xsl:if>

    <xsl:text />ret = <xsl:value-of select="$function-name" />(<xsl:text />

        <xsl:variable name="number-of-parameters" select="count(param)" />
        <xsl:choose>
            <xsl:when test="$number-of-parameters = 0">
                <xsl:text> );</xsl:text>
            </xsl:when>
            <xsl:otherwise>
                 <xsl:for-each select="param"><xsl:text />
                     <xsl:call-template name="parameter-cast">
                          <xsl:with-param name="type">
                              <xsl:call-template name="depointerize">
                                  <xsl:with-param name="type" select="type" />
                              </xsl:call-template>
                          </xsl:with-param>
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
            </xsl:otherwise>
        </xsl:choose>

    return ( void* ) ret;
}

<xsl:text />

    </xsl:template>

    <!-- ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: -->


    <xsl:template name="register-primitive">

        <xsl:variable name="function-name" select="./name/text()" />
        <xsl:variable name="return-type" select="./type/text()" />
        <xsl:variable name="is-struct" select="contains(./definition/text(), 'struct')" />
        <xsl:variable name="returns-pointer-type" select="contains($return-type, '*')" />
        <xsl:variable name="this" select="." />

        <!-- Define the p2_primitive. -->
    if ( !( prim = prim__new( <xsl:text />
        <xsl:call-template name="mangle-function-name">
            <xsl:with-param name="original-name" select="$function-name" />
        </xsl:call-template>, &quot;<xsl:text />
        <xsl:value-of select="$function-name" />&quot;, &quot;<xsl:text />
        <xsl:call-template name="depointerize">
            <xsl:with-param name="type" select="$return-type" />
        </xsl:call-template>&quot;, <xsl:text />
        <xsl:value-of select="count(param)" /> ))) return 0;<xsl:text />

        <!-- Define each parameter. -->
        <xsl:for-each select="param">
            <xsl:variable name="pos" select="position()" />
            <xsl:variable name="name" select="declname/text()" />
<xsl:text />
    if ( !( prim__set_parameter( prim, &quot;<xsl:text />
            <xsl:value-of select="declname/text()" />&quot;, &quot;<xsl:text />
            <xsl:call-template name="depointerize">
                <xsl:with-param name="type" select="type" />
            </xsl:call-template>&quot;, <xsl:text />
            <xsl:call-template name="boolean-as-transparency">
                <xsl:with-param name="arg" select="not(contains($this/detaileddescription/para/parameterlist/parameteritem[parameternamelist/parametername/text() = $name]/parameterdescription/para/text(), '$opaque'))" />
            </xsl:call-template>, <xsl:text />
            <xsl:value-of select="position() - 1" /> ))) return 0;<xsl:text />

        </xsl:for-each>

<xsl:text />
    p2_primitive__register( prim );
<xsl:text />
    </xsl:template>


    <!-- ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: -->


    <xsl:template name="enclose-in-angle-braces">

        <xsl:param name="arg" />
        <xsl:text />&lt;<xsl:value-of select="$arg" />&gt;<xsl:text />

    </xsl:template>


    <xsl:template name="enclose-in-quotes">

        <xsl:param name="arg" />
        <xsl:text />&quot;<xsl:value-of select="$arg" />&quot;<xsl:text />

    </xsl:template>


    <xsl:template name="comment-bar">

        <xsl:param name="length" />
        <xsl:value-of select="substring('////////////////////////////////////////////////////////////////////////////////', 1, $length)" />

    </xsl:template>


    <xsl:template name="boolean-as-number">

        <xsl:param name="arg" />
        <xsl:choose>
            <xsl:when test="$arg">1</xsl:when>
            <xsl:otherwise>0</xsl:otherwise>
        </xsl:choose>

    </xsl:template>


    <xsl:template name="boolean-as-transparency">

        <xsl:param name="arg" />
        <xsl:choose>
            <xsl:when test="$arg">RT</xsl:when>
            <xsl:otherwise>RO</xsl:otherwise>
        </xsl:choose>

    </xsl:template>


    <!-- ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: -->


    <!-- Define an include directive for a series of function stubs. -->
    <xsl:template name="include-directive">

        <xsl:text />
            <xsl:choose>
                <xsl:when test="@local = 'yes'">
                    <xsl:call-template name="enclose-in-quotes">
                        <xsl:with-param name="arg" select="text()" />
                    </xsl:call-template>
                </xsl:when>
                <xsl:otherwise>
                    <xsl:call-template name="enclose-in-angle-braces">
                        <xsl:with-param name="arg" select="text()" />
                    </xsl:call-template>
                </xsl:otherwise>
            </xsl:choose>
        <xsl:text />

    </xsl:template>


    <!-- ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: -->


    <!-- Create a (hopefully) unique function name. -->
    <xsl:template name="mangle-function-name">

        <xsl:param name="original-name" />
        <xsl:text />p2_imported__<xsl:value-of select="$original-name" />

    </xsl:template>


    <!-- ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: -->


    <!-- Cast a parameter from an array element. -->
    <xsl:template name="parameter-cast">

        <xsl:param name="type" />
        <xsl:param name="array-index" />
        *(( <xsl:value-of select="$type" />* ) args[<xsl:value-of select="$array-index" />] )<xsl:text />

    </xsl:template>


    <!-- ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: -->


    <!-- Prefixes a '*' to a non-pointer type reference.  Leaves pointer type
         references unchanged. -->
    <xsl:template name="dereference">

        <xsl:param name="type" />

        <xsl:choose>
            <xsl:when test="contains($type, '*')">
                <xsl:value-of select="$type" />
            </xsl:when>
            <xsl:otherwise>
                <xsl:value-of select="$type" />*<xsl:text />
            </xsl:otherwise>
        </xsl:choose>

    </xsl:template>


    <!-- Remove the '*' (if any) from a pointer type reference. -->
    <xsl:template name="depointerize">

        <xsl:param name="type" />

        <xsl:choose>
            <xsl:when test="contains($type, '*')">
                <xsl:value-of select="normalize-space(substring-before($type, '*'))" />
            </xsl:when>
            <xsl:otherwise>
                <xsl:value-of select="$type" />
            </xsl:otherwise>
        </xsl:choose>

    </xsl:template>


    <!-- ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: -->


</xsl:stylesheet>