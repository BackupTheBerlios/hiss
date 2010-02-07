#legacy
from HTMLParser import HTMLParser
tehlink='nuffin'
class NoisyParser(HTMLParser):
  def __init__(self):
    HTMLParser.__init__(self)
    self.tehtree={}
  def handle_starttag(self, tag, attrs):
    print("Starting tag ({0}), Value ({1})".format(tag,attrs))
    global tehlink
    try:
      self.tehtree[tag]=self.tehtree[tag]+1
      if tag=='a':
        for attr,value in attrs:
          if attr=='href':
            print ">>>>>>>>>>>>> A AHEAD:" + attr,value
            if value.find('index.php?view=')==0:
              tehlink='http://imagecabin.com/'+value
              print(tehlink)
    except KeyError:
      self.tehtree[tag]=1
    print(self.tehtree)
  def handle_endtag(self, tag):
    try:
      self.tehtree[tag]=self.tehtree[tag]-1
      print("Ending tag ({0})".format(tag))
      print(self.tehtree)
    except KeyError:
      self.tehtree[tag]=-1
pa=NoisyParser()
pa.feed(open('sample_xml_imagecabin.htm').read())
print(tehlink)