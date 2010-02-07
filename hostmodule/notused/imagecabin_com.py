from core import base
__version__='20100111_2'
__flsb__  = True
__fexif__ = True
def upload(localfile):
  strtoparse=base.upload('http://www.imagecabin.com/index.php',localfile,'file[]', fields={'MAX_FILE_SIZE':'3000000','filetype':'images','numupl':'1','action':'now','typesel':'images','privacy1':'1','filetitle1':'','cat1':'1'})
  p1=strtoparse.find('index.php?view=')
  if p1>=0:
    p2=strtoparse.find('"',p1)
    return 'http://imagecabin.com/'+strtoparse[p1:p2]
def download(url,dirToDownload,referrer=''):
  return base.download(url, dirToDownload,{'Referer': referrer})