from wsgiref.handlers import CGIHandler
from fractal_flame import app

app.debug = True
CGIHandler().run(app)

