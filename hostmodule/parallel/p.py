from multiprocessing import Pool
def k():
  pool = Pool(processes=4)
  n=3
  pa='gcle.jpg'
  rrr=[]
  for i in range(n):
    rrr.append(pa)
  print pool.map(upload, rrr)
  print('-------------')
  for i in range(n):
    print(upload(pa))
def t(s,images):
  exec('import '+s+' as q')
  pool = Pool(processes=4)
  return pool.map(q.upload, images)
def t2(s,images):
  exec('import '+s+' as q')
  for i in images:
    q.upload(i)
def m():
  ps=[]
  for i in range(3):
    print i
    ps.append(Process(target=upload, args=('core/test.gif',)))
    #ps.append(Process(target=upload, args=('core/test.gif',)))
    ps[i].start()
    ps[i].join()
  print ps