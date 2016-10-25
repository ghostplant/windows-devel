#!/usr/bin/python

# sudo apt-get install python-gevent-websocket python-flask

import time
from flask import Flask, request
from gevent.pywsgi import WSGIServer
from geventwebsocket.handler import WebSocketHandler

app = Flask(__name__)

@app.route('/')
def index():
	return app.send_static_file("index.html")

@app.route('/websocket')
def websocket():
	_socket = request.environ['wsgi.websocket']
	try:
		while True:
			data = _socket.receive() # receive a piece of message from web
			if not data:
				return
			
			print('server got: '+data)
			_socket.send('okay, I got it.') # send a piece of message to web
			
	finally:
		_socket.close()
	return ''

if __name__ == '__main__':
	try:
		WSGIServer(('0.0.0.0', 8080), app, handler_class=WebSocketHandler).serve_forever()
	except KeyboardInterrupt:
		pass

