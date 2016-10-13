from flask import Flask, render_template, request
import os
import uuid
import tempfile
import time
from datetime import datetime

from settings import settings


if __name__ == "__main__":
    app = Flask(__name__, static_folder='data', static_url_path='/data')
else:
    app = Flask(__name__)


def cleanup_data_folder():
    dtnow = datetime.now()
    for fl in os.listdir(settings["data_folder"]):
        file_name = os.path.join(settings["data_folder"],fl)
        dt = datetime.fromtimestamp(os.path.getmtime(file_name))
        tdif = dtnow - dt
        if tdif.total_seconds() > 5 * 60:
            os.remove(file_name)
     

def create_image_file_name():
    return os.path.join(settings["data_folder"], str(uuid.uuid1()))    


def create_image_url(file_name):
    return (
        str.format("{0}/{1}.png?{2}", settings["data_url"], file_name, uuid.uuid1()),
        str.format("{0}/{1}.xml?{2}", settings["data_url"], file_name, uuid.uuid1())
        )


def flame_fortune():
    cleanup_data_folder()
    
    output_file = create_image_file_name()
    
    if os.system(str.format("{0} --mode=fortune --output-file={1} --width={2} --height={3}",
        settings["fractal_flame_program"], output_file, settings["width"], settings["height"]))==0:
        (image_url,xml_url) = create_image_url(os.path.split(output_file)[1])
    else:
        image_url = None

    return {
        "image_url" : image_url,
        "xml_url" : xml_url,
        "next_flame_link" : settings["application_url"]
    }

root_url = settings["route_url"]
if root_url == "":
    root_url = "/"

@app.route(root_url)
def flame_page():
    flame_data = flame_fortune()
    
    return render_template('flame.html', 
        data=flame_data, settings=settings)


if __name__ == "__main__":
    app.debug = True
    app.run(host='0.0.0.0')    

