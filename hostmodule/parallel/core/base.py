import imghdr
import os.path
import random
import string
import urllib2

__version__='20100125'
#Super-Mario-Allchars, dealing win to bound-generator
allchars = string.letters + string.digits
def download(url,dirToDownload,header={}):
  """Downloads a previously uploaded file.
  
Parameters:
-----------
  :url:
    *string*

    URL of the image.
  :dirToDownload:
    *string*

    Folder to put the image in.
  :*header*:
    *dictionary*

    Additional headers to use"""
  ##NOES:
  #if header['Referer']==None:
  #  header['Referer']='http://'+url.split('/')[2]
  fname='%s/%s' % (dirToDownload,os.path.basename(url))
  with open(fname,'w') as f:
    f.write(fetchDoc(url,header=header))
  return fname
def fetchDoc(url, body=None, header={}):
  """Fetches a single document from the web.

Parameters:
-----------
  :url:
    *string*

    URL of the document to get.
  :*body*:
    *string*

    *default*: None

    Bodydata to be sent.
  :*header*:
    *dictionary*

    *default*: {}

    Headers to be sent"""
  header.update({'User-Agent': getUA()})
  req=urllib2.Request(url,data=body,headers=header)
  return urllib2.urlopen(req).read()
def generateBound():
  """Generates a boundary for MIME-Use.

"""
  return random.randint(8,19)*'-'+''.join([random.choice(allchars) for i in range(random.randint(12,18))])
def getUA():
  """Get a fake User-Agent-String.

Picks and returns a random string from a given list of UA-Strings"""
  #TODO: moar!
  #http://www.useragentstring.com/pages/useragentstring.php
  return random.choice((
    'Mozilla/5.0 (X11; U; Linux i686; pl-PL; rv:1.9.0.2) Gecko/20121223 Ubuntu/9.25 (jaunty) Firefox/3.8',
    'Mozilla/5.0 (Windows; U; Windows NT 5.2; zh-CN; rv:1.9.2) Gecko/20100101 Firefox/3.6',
    'Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 6.0;)',
    'Mozilla/4.0 (Mozilla/4.0; MSIE 7.0; Windows NT 5.1; FDM; SV1)',
    'Mozilla/5.0 (Windows; U; Windows NT 6.1; en-US; rv:1.8.1.21) Gecko/20090331 K-Meleon/1.5.3',
    'Mozilla/5.0 (compatible; Konqueror/4.3; Linux 2.6.31-16-generic; X11) KHTML/4.3.2 (like Gecko)',
    'Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 6.1; WOW64; Trident/4.0; SLCC2; .NET CLR 2.0.50727; .NET CLR 3.5.30729; .NET CLR 3.0.30729)'
  ))
def makeMIME(fields,localfile=None,imagefield=None):
  """Generates a MIME-Message from given data.

Parameters:
-----------
  :fields:
    *dictionary*

    HTML-Fields to fill out
  :*localfile*:
    *string*

    *default*: None

    Path to local file, that should be converted to MIME
  :*imagefield*:
    *string*

    *default*: None

    HTML-Field, that contains the image."""
  bound=generateBound()
  header = { 'Content-Type' : 'multipart/form-data; boundary="{0}"'.format(bound),
  'Mime-Version' : '1.0'}
  body=[]
  if localfile!=None and imagefield!=None:
    body.append('--'+bound)
    body.append('Content-Disposition: form-data; name="{iname}"; filename="{fname}"'.format(iname=imagefield,fname=os.path.basename(localfile)))
    body.append('Content-Type: image/{0}'.format(imghdr.what(localfile)))
    body.append('')
    body.append(open(localfile).read())
  for name,content in fields.iteritems():
    body.append('--'+bound)
    body.append('Content-Disposition: form-data; name="{0}"'.format(name))
    body.append('')
    body.append(content)
  body.append('--'+bound+'--')
  return '\r\n'.join(body), header

def upload(url,imagepath,imagefield,header={},fields={}):
  """Uploads an image

Parameters:
-----------
  :url:
    *string*

    URL to the script, accepting uploads
  :imagepath:
    *string*

    Path to local file, that should be uploaded.
  :imagefield:
    *string*

    HTML-Field containing the image
  :*header*:
    *dictionary*

    *default*: {}

    Headers to use
  :*fields*:
    *dictionary*

    *default*: {}

    HTML-Fields to fake"""
  body,mheader=makeMIME(fields,imagepath,imagefield)
  mheader.update(header)
  return fetchDoc(url,body,mheader)