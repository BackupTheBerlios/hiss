from core import base
__version__='20100111_2'
__flsb__  = '?'
__fexif__ = '?'
def upload(localfile):
  strtoparse=base.upload('http://www.7pics.info',localfile,'uploadfile[]',fields={'MAX_FILE_SIZE':'8388608','upload':'1'})
  p1=strtoparse.find('Direktzugriff Vollbild<br />')
  if p1>=0:
    p1+=72
    p2=strtoparse.find('"',p1)
    return ''+strtoparse[p1:p2]
def download(url,dirToDownload,referrer=''):
  return base.download(url, dirToDownload,{'Referer': referrer})
