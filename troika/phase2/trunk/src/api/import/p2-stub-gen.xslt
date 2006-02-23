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

#ifdef __cplusplus
extern &quot;C&quot;
{
#endif

#include &lt;stdlib.h&gt;  /* malloc */

<xsl:text />
        <xsl:for-each select="compounddef">
            <xsl:if test="count(sectiondef) > 0">
/******************************************************************************/

#include &quot;<xsl:text />
        <xsl:value-of select="compoundname/text()" />
        <xsl:text />&quot;<xsl:text />

        <xsl:text>&#xa;</xsl:text>

        <!-- Add any includes from the source file. -->
        <xsl:for-each select="includes">
/* #include <xsl:text />
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
            </xsl:choose> */<xsl:text/>
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
/******************************************************************************/


#ifdef __cplusplus
}  /* extern &quot;C&quot; */
#endif

<xsl:text />

    </xsl:template>


    <!-- ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: -->


    <xsl:template name="p2__import_primitives">

/* #include &quot;import-aux.h&quot; */

enum function_marker
{
    unmarked = 0,
    encoder,
    decoder,
    destructor
};

extern void *p2_primdef__head(
    void *( *cstub )( void** ),
    char *name,
    char *return_type_id,
    int parameters );

extern void *p2_primdef__parameter(
    char *param_name,
    char *param_type_id,
    char param_transparency );

extern void *p2_primdef__tail( enum function_marker marker );


#define RT  ( char ) 1  /* Referentially transparent. */
#define RO  ( char ) 0  /* Referentially opaque. */


int p2__import_primitives()
{
    return ( 1
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
    );
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
        <xsl:if test="not($return-type = 'void')">
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
        </xsl:if>

        <xsl:if test="not($return-type = 'void')">
            <xsl:if test="not($returns-pointer-type)">*</xsl:if>
    <xsl:text />ret = <xsl:text />
        </xsl:if>
        <xsl:value-of select="$function-name" />(<xsl:text />

        <xsl:variable name="number-of-parameters" select="count(param)" />
        <xsl:choose>
            <xsl:when test="$number-of-parameters = 0">
                <xsl:text> );</xsl:text>
            </xsl:when>
            <xsl:otherwise>
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
                     <xsl:text />/* <xsl:value-of select="type" />
                     <xsl:value-of select="' '" />
                     <xsl:value-of select="declname" /> */<xsl:text/>
                 </xsl:for-each>
            </xsl:otherwise>
        </xsl:choose>

        <xsl:choose>
            <xsl:when test="$return-type = 'void'">
<xsl:text />

    return 0;<xsl:text />
            </xsl:when>
            <xsl:otherwise>
<xsl:text />

    return ( void* ) ret;<xsl:text />
            </xsl:otherwise>
        </xsl:choose>
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
        /* <xsl:value-of select="$function-name" /> */
        &amp;&amp; p2_primdef__head( <xsl:text />
        <xsl:call-template name="mangle-function-name">
            <xsl:with-param name="original-name" select="$function-name" />
        </xsl:call-template>, &quot;<xsl:text />
        <xsl:value-of select="$function-name" />&quot;, &quot;<xsl:text />
        <xsl:call-template name="remove-qualifiers">
            <xsl:with-param name="type">
                <xsl:call-template name="depointerize">
                    <xsl:with-param name="type" select="$return-type" />
                </xsl:call-template>
            </xsl:with-param>
        </xsl:call-template>&quot;, <xsl:text />
        <xsl:value-of select="count(param)" /> )<xsl:text />

        <!-- Define each parameter. -->
        <xsl:for-each select="param">
            <xsl:variable name="pos" select="position()" />
            <xsl:variable name="name" select="declname/text()" />
<xsl:text />
        &amp;&amp; p2_primdef__parameter( &quot;<xsl:text />
            <xsl:value-of select="declname/text()" />&quot;, &quot;<xsl:text />
            <xsl:call-template name="remove-qualifiers">
                <xsl:with-param name="type">
                    <xsl:call-template name="depointerize">
                        <xsl:with-param name="type" select="type" />
                    </xsl:call-template>
                </xsl:with-param>
            </xsl:call-template>&quot;, <xsl:text />
            <xsl:call-template name="boolean-as-transparency">
                <xsl:with-param name="arg" select="not(contains($this/detaileddescription/para/parameterlist/parameteritem[parameternamelist/parametername/text() = $name]/parameterdescription/para/text(), '$opaque'))" />
            </xsl:call-template> )<xsl:text />

        </xsl:for-each>

        <!-- Register the primitive. -->
<xsl:text />
        &amp;&amp; p2_primdef__tail( <xsl:text />
        <xsl:variable name="marker">
            <xsl:for-each select="detaileddescription/para">
                <xsl:call-template name="function-marker" />
            </xsl:for-each>
        </xsl:variable>
        <xsl:choose>
            <xsl:when test="string-length($marker) > 0">
                <xsl:value-of select="$marker" />
            </xsl:when>
            <xsl:otherwise>
                <xsl:text>unmarked</xsl:text>
            </xsl:otherwise>
        </xsl:choose>
        <xsl:text/> )
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
        <xsl:text />p2_cstub__<xsl:value-of select="$original-name" />

    </xsl:template>


    <!-- ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: -->


    <!-- Cast a parameter from an array element. -->
    <xsl:template name="parameter-cast">

        <xsl:param name="type" />
        <xsl:param name="array-index" />
        <xsl:variable name="depointerized-type">
            <xsl:call-template name="depointerize">
                <xsl:with-param name="type" select="type" />
            </xsl:call-template>
        </xsl:variable>

        <xsl:choose>
            <xsl:when test="contains($type, '*')">
        ( <xsl:value-of select="$depointerized-type" />* ) args[<xsl:value-of select="$array-index" />]<xsl:text />
            </xsl:when>
            <xsl:otherwise>
        *(( <xsl:value-of select="$depointerized-type" />* ) args[<xsl:value-of select="$array-index" />] )<xsl:text />
            </xsl:otherwise>
        </xsl:choose>

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


    <!-- Remove any type qualifiers. -->
    <xsl:template name="remove-qualifiers">

        <xsl:param name="type" />

        <xsl:choose>
            <xsl:when test="contains($type, 'const')">
                <xsl:value-of select="normalize-space(substring-after($type, 'const'))" />
            </xsl:when>
            <xsl:otherwise>
                <xsl:value-of select="$type" />
            </xsl:otherwise>
        </xsl:choose>

    </xsl:template>


    <!-- ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: -->


    <xsl:template name="function-marker">

        <xsl:choose>
            <xsl:when test="contains(text(), '$encoder')">
                <xsl:text>encoder</xsl:text>
            </xsl:when>
            <xsl:when test="contains(text(), '$decoder')">
                <xsl:text>decoder</xsl:text>
            </xsl:when>
            <xsl:when test="contains(text(), '$destructor')">
                <xsl:text>destructor</xsl:text>
            </xsl:when>
        </xsl:choose>

    </xsl:template>


    <!-- ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::: -->


</xsl:stylesheet>