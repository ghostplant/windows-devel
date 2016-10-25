	# Auth 上传
	def do_PUT(self):
			try:
					raise Exception("not correct upload/download port")
					"""username = self.authenticate_with_headers()
					[null, cmd, filename] = self.path.split('/', 2)
					filename=filename.split('?', 1)[0]
					if cmd=='upload' or cmd=='upload-force':
							if filename.find('/')!=-1 or filename.find('*')!=-1:
									raise Exception('unsupported delimiter "/", "*" !')
							if filename=='' or filename=='.' or filename=='..':
									raise Exception('illegal filename "", ".", ".." !')
							target = "/home/docklet/global/users/%s/home/%s" % (username, filename)
							if os.path.exists(target) and cmd!='upload-force':
									raise Exception('file with name %s already exists in your nfs directory, uploading cancelled (try upload-force)' % filename)
							length = int(self.headers['content-length'])
							host = open(target, "w")
							read = 0
							while read < length:
									data = self.rfile.read(min(1000000, length - read))
									host.write(data)
									read += len(data)
							host.close()
							obj = {'success':True, 'file-location': "/nfs/%s" % filename}
					else:
							raise Exception('unsupported request!')"""
			except Exception as e:
					sys.stderr.write(traceback.format_exc())
					obj = {'success':False, 'message': str(e)}

			self.send_response(200)
			self.send_header("Content-type", "application/json")
			self.end_headers()
			self.wfile.write(json.dumps(obj).encode())
			self.wfile.write('\n'.encode())
			#self.wfile.close()
			return


	# Auth 下载
	def do_GET(self):
			try:
					username = self.authenticate_with_headers()
					context = self.path
					if context.startswith("/download/"):
							raise Exception("not correct upload/download port")
							"""filename = context[10:].strip()
							if filename.find('/')!=-1 or filename.find('*')!=-1 or filename == "" or filename == "." or filename =="..":
									raise Exception("Bad filename given!")
							target = "/home/docklet/global/users/%s/home/%s" % (username, filename)
							self.send_response(200)
							self.end_headers()
							host = open(target, "r")
							BLOCK = 1024000
							data = host.read(BLOCK)
							while len(data)!=0:
									self.wfile.write(data)
									data = host.read(BLOCK)
							host.close()
							self.wfile.close()
							return"""
					if not context.endswith("/"):
							context = context + "/"
					obj = {'success':True, 'data': self.on_get_request(context, username)}
			except Exception as e:
					sys.stderr.write(traceback.format_exc())
					obj = {'success':False, 'message': str(e)}

			self.send_response(200)
			self.send_header("Content-type", "application/json")
			self.end_headers()
			self.wfile.write(json.dumps(obj).encode())
			self.wfile.write('\n'.encode())
			#self.wfile.close()
			return

		# 迭代显示POST字段
			""" for field in form.keys():
					field_item = form[field]
					if field_item.filename:
							file_data = field_item.file.read()
							file_len = len(file_data)
							del file_data
							self.wfile.write('\tUploaded %s as "%s" (%d bytes)\n' % (field, field_item.filename, file_len))
					else:
							self.wfile.write('\t%s=%s\n' % (field, form[field].value)) """


		# 提交作业
		if self.path=='/submit':
			raise Exception("not correct upload/download port")
			ctype, pdict = cgi.parse_header(self.headers.getheader('content-type'))
			if ctype != 'multipart/form-data':
				raise Exception("data format not supported")
			postvars = cgi.parse_multipart(self.rfile, pdict)
			provider = ["\n".join(postvars['user']), "\n".join("\n".join(postvars['key']).split('\\n'))]
			username = self.authenticate_with_headers(provider)
				
			filename = "".join(postvars['name'])
			if filename.find('/')!=-1 or filename.find('*')!=-1:
				raise Exception('unsupported delimiter "/", "*" !')
			if filename=='' or filename=='.' or filename=='..':
				raise Exception('illegal filename "", ".", ".." !')
				
			if os.system('mkdir -p /home/docklet/global/users/%s/home/submit' % username)!=0:
				raise Exception('cannot locate "/nfs/submit" directory!')
			target = "/home/docklet/global/users/%s/home/submit/%s" % (username, filename)
				
			host = open(target, "w")
			host.write("\n".join(postvars['upload']))
			host.close()
			# obj = {'success':True, 'file-location': "/nfs/submit/%s" % filename}
				
			self.send_response(200)
			self.end_headers()
			#self.wfile.close()
			return
		else:
			form = cgi.FieldStorage(fp=self.rfile, headers=self.headers,environ={'REQUEST_METHOD':'POST','CONTENT_TYPE': "text/html"})
