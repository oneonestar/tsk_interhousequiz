xmllint --xinclude --output big.xml book.xml
xsltproc /usr/share/xml/docbook/stylesheet/docbook-xsl-ns/fo/docbook.xsl big.xml > book.fo
fop -fo book.fo -pdf book.pdf
#xmlstarlet val --err --xsd /usr/share/xml/docbook/schema/xsd/5.0/docbook.xsd book.xml
