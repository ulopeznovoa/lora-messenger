from flask import Flask, request
from flask import render_template, send_from_directory
from flask_restful import Resource, Api, reqparse
from json import dumps
import os

app = Flask(__name__, static_url_path='')
api = Api(app)

configfile = "../counter.conf"
sendbuffer = "../send.dat"
receivebuffer = "../receive.dat"
sendflag = "../sendflag.dat"

#lora_binary = "../lora_interface/cooking/code/LoRa/main-tx-rx.cpp_exe"

parser = reqparse.RequestParser()

class Transmit(Resource):
    def post(self):
        message = request.form['message']
        print 'Transmit message: '+message
        file = open(sendbuffer, 'w')
        file.write(message)
        file.close()

        file = open(sendflag, 'w')
        file.write('1')
        file.close()

        return {'status': 1}

class Receive(Resource):
    def get(self):
        file = open(receivebuffer, 'r')
        msg = file.read()
        file.close()

        return {'message': msg}

#Routes
api.add_resource(Transmit, '/transmit') 
api.add_resource(Receive, '/receive') 

@app.route('/')
def home():
    file = open(configfile, 'r')
    rpi_name = file.read()
    file.close()
 

    file = open('home.html','r')
    html_string = file.read()
    file.close()
    html_string = html_string.replace('rpi-name',rpi_name)

    return html_string

@app.route('/js/<path:path>')
def send_js(path):
    return send_from_directory('js',path)

@app.route('/css/<path:path>')
def send_css(path):
    return send_from_directory('css',path)

if __name__ == '__main__':
    app.run(port=5002, host='0.0.0.0', debug=True)
