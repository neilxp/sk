import web

db = web.database(dbn='sqlite', db="data.rdb")

def getAllAvailable():
	return db.select('news', order='ts', where='status=0')

def markNotSend(IDs):
	whereclause = "id in (" + ','.join(IDs) + ")"
	db.update("news", where = whereclause, status= 1)