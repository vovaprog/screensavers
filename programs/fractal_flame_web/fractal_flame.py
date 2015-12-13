from flask import Flask
from flask import render_template
from flask import request
import urllib
import os
import uuid

from settings import settings

if __name__ == "__main__":
    app = Flask(__name__, static_folder='data', static_url_path='/data')
else:
    app = Flask(__name__)

def flame_fortune():
    output_file="./data/fortune"
    
    if os.system(str.format("{0} --mode=fortune --output-file={1} --width={2} --height={3}",settings["fractal_flame_program"],output_file,settings["width"],settings["height"]))==0:
        image_url = create_image_url(output_file)
    else:
        image_url = None

    return {
        "image_url" : image_url
    }

def create_image_url(file_name):
    return str.format("/data/fortune.png?{1}",file_name,uuid.uuid1())
    

@app.route("/flame")
def flame_page():
    flame_data = flame_fortune()
    
    return render_template('flame.html', 
        data=flame_data, settings=settings)

if __name__ == "__main__":
    app.debug = True
    app.run(host='0.0.0.0')    

