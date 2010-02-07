from core import base
__version__='always too old'
def upload(localfile):
"""Uploads a File.

Parameters:
===========
  :localfile:
    *string*

    The local file to be uploaded.

This method is called by the C-Core. *localfile* is a string, containing a qualified path to an image to upload. Absolute Paths would be the safest way to go, but relative would work as well with all pathes set properly. It returns a String-URL, pointing to the image online. Only if needed, the required referrer is given back aswell in the form *URL<Space>Referrer*

Workflow:
=========


Get HTML:
~~~~~~~~~
  The usual way to go is to call *base.upload()* and save the result somewhere.

Example:
--------
::

  s=base.upload('<URLToScript>',localfile,'<FileField>',fields={<Additional Fields>})

Fields to edit:
---------------
  :URLToScript:
    URL to the serverside script, handling the imageupload.
  :FileField:
    The HTML (File-)-Field, the image belongs to
  :Additional Fields:
    Dictionary with additional input-elements filled out.
    f.ex.: fields={'comment':'i like it'}

Parse it:
~~~~~~~~~
  The resulting string can be parsed for some unique element, containing the image.

Example:
--------
::

  p1=s.find('viewimage.php?image=')
  if p1>=0:
    p2=s.find('"',p1)
    theurl 'http://example.com/'+s[p1:p2]

Return
~~~~~~
  Return a String. If a referrer is necessary, its *<URL>+' '+<Referer>*, else only *<URL>*

Example:
--------
::

  return theurl+' http://example.com'
"""
  pass
def download(url,dirToDownload,referrer=''):
'''Downloads an image.

Parameters:
===========
  :url:
    *string*

    The images url.
  :dirToDownload:
    *string*

    Directory to put the image in.
  :*referrer*:
    *string*

    *default*: ''

    Sets a specific referrer to fake.'''
  pass
def urlUpload(url):
'''Uploads an image from another hoster via the url-upload-function.

  Saves time and ressources mirroring images to several places in the world.

Parameters:
===========
  :url:
    *string*
    
    The Hoster(already containing the image)s url

Usage:
======

Workflow like upload.

::

  body,header=base.makeMIME({'source':url,'submit':'Upload','resize':'320x240'})
  stp=base.fetchDoc(<this hoster>,body,header)
  p1=stp.find('value="http://pixuria.com/imgs/')
  if p1>0:
    p1+=7
    p2=stp.find('"',p1)
    print(stp[p1:p2])'''
  pass