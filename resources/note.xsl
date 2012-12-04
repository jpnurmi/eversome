<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="2.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:output method="html" encoding="utf-8" omit-xml-declaration="yes" media-type="text/html" doctype-public="" doctype-system=""/>

<xsl:template match="/">
    <html>
        <head>
            <meta http-equiv="Content-Type" content="text/html; charset=UTF-8"/>
        </head>
        <xsl:apply-templates/>
    </html>
</xsl:template>

<xsl:template match="en-note">
    <body class="en-note">
        <xsl:attribute name="style">
            <xsl:value-of select="@style" />
        </xsl:attribute>
        <xsl:apply-templates/>
    </body>
</xsl:template>

<xsl:template match="en-media">
    <div class="en-media">
        <xsl:attribute name="style">
            <xsl:value-of select="@style" />
        </xsl:attribute>
        <xsl:attribute name="type">
            <xsl:value-of select="@type" />
        </xsl:attribute>
        <xsl:variable name="hash" select="@hash" />
        <img src="{$dataPath}/{$hash}/thumb.png" />
    </div>
</xsl:template>

<xsl:template match="en-todo">
    <input class="en-todo" type="checkbox">
        <xsl:if test="@checked='true'">
            <xsl:attribute name="checked">
                <xsl:value-of select="@checked" />
            </xsl:attribute>
        </xsl:if>
    </input>
</xsl:template>

<xsl:template match="en-crypt">
    <select class="en-crypt">
        <option value="decrypt">&#9679;&#9679;&#9679;&#9679;&#9679;&#9679;&#9679;&#9679;</option>
        <option value="decrypt">Decrypt...</option>
    </select>
</xsl:template>

<xsl:template match="*">
    <xsl:copy-of select="."/>
</xsl:template>

</xsl:stylesheet>
