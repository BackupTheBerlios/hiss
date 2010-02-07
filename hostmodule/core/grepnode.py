#legacy
# Mal ein bisschen Gespiele im DOM-Baum
# sample_xml_imagecabin.htm
from xml.etree import cElementTree as ET
doc=ET.ElementTree()
doc.parse('sample_xml_imagecabin.htm')
tabs=doc.findall('body/table')
print(tabs)