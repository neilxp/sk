# -*- coding: utf-8 -*-


""" Basic todo list using webpy 0.3 """
import web
import model

### Url mappings

urls = (
    '/', 'Index',
    '/del/(\d+)', 'Delete'
)


### Templates
render = web.template.render('templates', base='base')


class Index:
    def GET(self):
        """ Show page """
        news = [
            {
                "city":"hangzhou", 
                "news":[
                    {"id": 1, "title": u"坚实的龙卷风"},
                    {"id": 2, "title": u"撒发生地方"},
                    {"id": 3, "title": u"后脑勺撒娇的发链接阿萨德浪费阿萨德，了解阿萨德。的分公司的风格"},
                    {"id": 4, "title": u"爱的撒发生地方"},
                    {"id": 5, "title": u"同样艰难地"},
                    {"id": 6, "title": u"说分公司的风格"},
                ]
            }
        ]
        return render.index(news)

    def POST(self):
        """ Add new entry """
        if not form.validates():
            todos = model.get_todos()
            raise web.seeother('/')
        model.new_todo(form.d.title)
        raise web.seeother('/')



class Delete:

    def POST(self, id):
        """ Delete based on ID """
        id = int(id)
        model.del_todo(id)
        raise web.seeother('/')


app = web.application(urls, globals())

if __name__ == '__main__':
    app.run()
