#legacy
#Beispiel zu "FindNSlice"
tehstr=open('sample_xml_imagecabin.htm').read()
p1=tehstr.find('index.php?view=')
if p1>=0:
  p2=tehstr.find('"',p1)
  print('http://imagecabin.com/'+tehstr[p1:p2])