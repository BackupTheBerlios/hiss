#legacy
#import email.mime.image
import imghdr
import os.path
import random
import string
import urllib2
__version__='20091214_1'
allchars = string.letters + string.digits
def download(url,dirToDownload,header={}):
  #TODO: Exceptionhandling,return
  req = urllib2.Request(url,header)
  response = urllib2.urlopen(req)
  open('%s/%s' % (dirToDownload,os.path.basename(url)),'w').write(response.read())
def fetchDoc(url, body=None, header={}):
  #TODO: Move UA-Faker in here
  req=urllib2.Request(url,data=body,headers=header)
  return urllib2.urlopen(req)
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
    'Mozilla/4.0 (Mozilla/4.0; MSIE 7.0; Windows NT 5.1; FDM; SV1)'
  ))
def makeMIME(localfile,imagefield,fields):
  bound=generateBound()
  header = { 'Content-Type' : 'multipart/form-data; boundary="{0}"'.format(bound),
  'Mime-Version' : '1.0'}
  body=[]
# For Servers may change their opinion again. Heres MIME & B64:
#  mimeImage=email.mime.image.MIMEImage(open(localfile).read())
#  mimeImage['Content-Disposition'] = 'form-data; name="{iname}"; filename="{fname}"'.format(iname=imagefield,fname=os.path.basename(localfile))
#  body.append('--'+bound)
#  body.append(mimeImage.as_string())
  body.append('--'+bound)
#NOMIME
  body.append('Content-Disposition: form-data; name="{iname}"; filename="{fname}"'.format(iname=imagefield,fname=os.path.basename(localfile)))
  body.append('Content-Type: image/{0}'.format(imghdr.what(localfile)))
  body.append('')
  body.append(open(localfile).read())
#ENDNOMIME
  for name,content in fields.iteritems():
    body.append('--'+bound)
    body.append('Content-Disposition: form-data; name="{0}"'.format(name))
    body.append('')
    body.append(content)
  body.append('--'+bound+'--')
  return header, body
def upload(url,imagepath,imagefield,header={},fields={}):
  #TODO: Exceptionhandling,return
  mheader,body=makeMIME(imagepath,imagefield,fields)
  mheader.update(header)
  req = urllib2.Request(url,'\r\n'.join(body),mheader)
  return urllib2.urlopen(req).read()