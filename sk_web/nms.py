# -*- coding: utf-8 -*-
import web, re, sqlite3
import model

### Url mappings

urls = (
    '/', 'Index',
    '/news/action/delete', 'Delete'
)


### Templates
render = web.template.render('templates', base='base')
checkBoxMatch = re.compile(r'cb-(\d+)$')

class Index:
    def GET(self):
        """ Show page """
        news = model.getAllAvailable()
        newsByCity={}
        for entry in news:
            if not newsByCity.has_key(entry['city']):
                newsByCity[entry['city']] = []
            value = {'id': entry['id'], 'title': entry['title']}
            newsByCity[entry['city']].append(value)
        # from pprint import pformat
        # print(pformat(newsByCity))
        return render.index(newsByCity)

class Delete:

    def POST(self):
        """ Delete based on ID """
        IDs=[]
        i = web.input()
        for k in i.keys():
            matchResult = checkBoxMatch.match(k)
            if matchResult:
                IDs.append(matchResult.group(1))

        model.markNotSend(IDs)
        raise web.seeother('/')


app = web.application(urls, globals())

if __name__ == '__main__':
    app.run()
