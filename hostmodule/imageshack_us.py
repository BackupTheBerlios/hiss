from core import base
__version__='20100124'
__flsb__  = True
__fexif__ = True
def upload(localfile):
  strtoparse=base.upload('http://www.imageshack.us/',localfile,'fileupload',fields={'uploadtype':'on','url':'','email':'','MAX_FILE_SIZE':'13145728','refer':'','brand':'','optiimage':'resample','optisize':'resample','rembar':'1'})
  p1=strtoparse.find('<input onClick="pageTracker._trackEvent(\'new-done-click\',\'direct-click\');" readonly="readonly" class="readonly" value="')
  if p1>=0:
    p1+=119
    p2=strtoparse.find('"',p1)
    return strtoparse[p1:p2]
def download(url,dirToDownload,referrer=''):
  return base.download(url, dirToDownload,{'Referer': referrer})
