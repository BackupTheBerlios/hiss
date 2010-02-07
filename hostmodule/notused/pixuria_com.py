from core import base
__version__='20100125'
__flsb__  = True
__fexif__ = True

def upload(localfile):
  strtoparse=base.upload('http://www.pixuria.com',localfile,'userfile',fields={'MAX_FILE_SIZE':'12582912','SendenButton':'Upload gestartet','rezise':'320x240'})
  p1=strtoparse.find('Hotlink')
  if p1>=0:
    p1+=229
    p2=strtoparse.find('&',p1)
    return strtoparse[p1:p2]

def urlUpload(url):
  body,header=base.makeMIME({'source':url,'submit':'Upload','resize':'320x240'})
  stp=base.fetchDoc('http://pixuria.com/index.php?mod=remote',body,header)
  p1=stp.find('value="http://pixuria.com/imgs/')
  if p1>0:
    p1+=7
    p2=stp.find('"',p1)
    print(stp[p1:p2])
