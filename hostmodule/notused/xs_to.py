from core import base
__version__='20100125'
__flsb__  = True
__fexif__ = True
def upload(localfile):
  strtoparse=base.upload('http://www.xs.to/upload.php',localfile,'f_single',fields={'typ':'s'})
  p1=strtoparse.find('URL=')
  if p1>0:
    p1+=4
    p2=strtoparse.find('">',p1)
    strtoparse=base.fetchDoc(strtoparse[p1:p2])
    #Ye, theyve no clue of html, single ' is correct in here
    p1=strtoparse.find("'http://xs.to/image")
    if p1>0:
      p1+=1
      p2=strtoparse.find("'",p1)
      return strtoparse[p1:p2]
def download(url,dirToDownload,referrer=''):
  return base.download(url, dirToDownload,{'Referer': referrer})