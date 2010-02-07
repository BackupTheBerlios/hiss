from core import base
__version__='20100125'
__flsb__  = '?'
__fexif__ = '?'
def upload(localfile):
  strtoparse=base.upload('http://www.hellameke.com/basic.php',localfile,'userfile')
  p1=strtoparse.find('hellameke.com/images/')
  if p1>=0:
    p2=strtoparse.find('"',p1)
    return 'http://www.'+strtoparse[p1:p2]
def download(url,dirToDownload,referrer=''):
  return base.download(url, dirToDownload,{'Referer': referrer})
