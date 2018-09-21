from flask import Flask, request
from flask import render_template, send_from_directory
from flask_restful import Resource, Api
from json import dumps

app = Flask(__name__, static_url_path='')
api = Api(app)

filename = "counter_value.txt"

class Increment(Resource):
    def get(self):
        file = open(filename, 'r')
        val = int(file.read())
        file.close()

        file = open(filename, 'w')
        file.write(str(val+1))
        file.close()

        return {'value': val+1}

class Refresh(Resource):
    def get(self):
        file = open(filename, 'r')
        val = int(file.read())
        file.close()

        return {'value': val}
       
api.add_resource(Increment, '/increment') # Route_1
api.add_resource(Refresh, '/refresh') # Route_2

@app.route('/')
def home():
    file = open(filename, 'r')
    counter_val = file.read()
    file.close()
    
    file = open('home.html','r')
    html_string = file.read()
    file.close()
    html_string = html_string.replace('counter-to-replace',counter_val)

    return html_string
#    return render_template(html_string)
#    return render_template('home.html')

@app.route('/js/<path:path>')
def send_js(path):
    return send_from_directory('js',path)

if __name__ == '__main__':
    app.run(port=5002, host='0.0.0.0', debug=True)
