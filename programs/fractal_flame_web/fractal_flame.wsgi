import sys
sys.path.insert(0, '/home/vova/applications/fractal_flame')

import logging, sys
logging.basicConfig(stream=sys.stderr)

from fractal_flame import app as application

