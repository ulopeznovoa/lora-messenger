from flask import Flask, request
from flask import render_template, send_from_directory
from flask_restful import Resource, Api
from json import dumps
import os

app = Flask(__name__, static_url_path='')
api = Api(app)

filename = "../counterFile.dat"
configfile = "../counter.conf"
lora_binary = "../lora_interface/cooking/code/LoRa/main-tx-rx.cpp_exe"

class Increment(Resource):
    def get(self):
        file = open(filename, 'r')
        val = int(file.read())
        file.close()

        file = open(filename, 'w')
        file.write(str(val+1))
        file.close()

        return {'value': val+1}

class Decrement(Resource):
    def get(self):
        file = open(filename, 'r')
        val = int(file.read())
        file.close()

        file = open(filename, 'w')
        file.write(str(val-1))
        file.close()

        return {'value': val-1}

class Refresh(Resource):
    def get(self):
        file = open(filename, 'r')
        val = int(file.read())
        file.close()

        return {'value': val}

class Transmit(Resource):
    def get(self):
        cmd = lora_binary + ' t'
        os.system(cmd)

        return {'error': 0}

class Receive(Resource):
    def get(self):
        cmd = lora_binary + ' r'
        os.system(cmd)

        return {'error': 0}

#Routes

api.add_resource(Increment, '/increment') 
api.add_resource(Decrement, '/decrement') 
api.add_resource(Refresh, '/refresh') 
api.add_resource(Transmit, '/transmit') 
api.add_resource(Receive, '/receive') 

@app.route('/')
def home():
    file = open(filename, 'r')
    counter_val = file.read()
    file.close()
    
    file = open(configfile, 'r')
    rpi_name = file.read()
    file.close()
 

    file = open('home.html','r')
    html_string = file.read()
    file.close()
    html_string = html_string.replace('counter-to-replace',counter_val)
    html_string = html_string.replace('rpi-name',rpi_name)

    return html_string

@app.route('/js/<path:path>')
def send_js(path):
    return send_from_directory('js',path)

if __name__ == '__main__':
    app.run(port=5002, host='0.0.0.0', debug=True)
