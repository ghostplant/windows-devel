function glTextureShaderLatest() {

	this.loadWebGL = function(id) {
		var canvas = document.getElementById(id);
		var gl = canvas.getContext('experimental-webgl');

		var prog = gl.createProgram();
		var vShader = gl.createShader(gl.VERTEX_SHADER), fShader = gl
				.createShader(gl.FRAGMENT_SHADER);
		gl
				.shaderSource(
						vShader,
						'attribute vec3 vtxPosition;attribute vec4 vtxColor;uniform mat4 matObject, matProjection;varying vec4 unionColor;'
								+ 'void main() { gl_Position = matProjection * matObject * vec4(vtxPosition, 1.0);unionColor = vtxColor; }');
		gl.compileShader(vShader), gl.attachShader(prog, vShader);
		gl
				.shaderSource(
						fShader,
						'precision mediump float;uniform sampler2D sampleTexture;uniform bool useTexture;varying vec4 unionColor;void main() { gl_FragColor = (useTexture ? texture2D(sampleTexture, vec2(unionColor.s,unionColor.t)): unionColor); }');
		gl.compileShader(fShader), gl.attachShader(prog, fShader);
		gl.linkProgram(prog), gl.useProgram(prog);

		prog.vtxPosition = gl.getAttribLocation(prog, "vtxPosition");
		prog.vtxColor = gl.getAttribLocation(prog, "vtxColor");
		prog.matProjection = gl.getUniformLocation(prog, "matProjection");
		prog.matObject = gl.getUniformLocation(prog, "matObject");
		prog.sampleTexture = gl.getUniformLocation(prog, "sampleTexture");
		prog.useTexture = gl.getUniformLocation(prog, "useTexture");
		gl.enableVertexAttribArray(prog.vtxPosition);
		gl.enableVertexAttribArray(prog.vtxColor);

		gl.program = prog;
		gl.matProjection = mat4.create(), gl.matObject = mat4.create();
		mat4.identity(gl.matProjection), mat4.identity(gl.matObject);

		gl.prepareObject = function(vecItems) {
			var buffer = gl.createBuffer();
			gl.bindBuffer(gl.ARRAY_BUFFER, buffer);
			gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(vecItems),
					gl.STATIC_DRAW);
			return buffer;
		};

		gl.prepareTexture = function(image) {
			var texture = gl.createTexture();
			gl.bindTexture(gl.TEXTURE_2D, texture);
			gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGB, gl.RGB, gl.UNSIGNED_BYTE,
					image);
			gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.NEAREST);
			gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.NEAREST);
			gl.activeTexture(gl.TEXTURE0);
			gl.uniform1i(prog.sampleTexture, 0);
			return texture;
		};

		gl.displayObject = function(vtxPosition, vtxColor, texture, mode, count) {
			gl.uniformMatrix4fv(gl.program.matProjection, false,
					gl.matProjection);
			gl.uniformMatrix4fv(gl.program.matObject, false, gl.matObject);
			gl.bindBuffer(gl.ARRAY_BUFFER, vtxPosition);
			gl.vertexAttribPointer(gl.program.vtxPosition, 3, gl.FLOAT, false,
					0, 0);
			gl.bindBuffer(gl.ARRAY_BUFFER, vtxColor);
			if (texture) {
				gl.uniform1i(prog.useTexture, true);
				gl.vertexAttribPointer(gl.program.vtxColor, 2, gl.FLOAT, false,
						0, 0);
				gl.bindTexture(gl.TEXTURE_2D, texture);
			} else {
				gl.uniform1i(prog.useTexture, false);
				gl.vertexAttribPointer(gl.program.vtxColor, 4, gl.FLOAT, false,
						0, 0);
			}
			gl.drawArrays(mode, 0, count);
		};

		gl.ortho = function(left, right, bottom, top, near, far) {
			mat4.ortho(left, right, bottom, top, near, far, gl.matProjection);
		};

		gl.perspective = function(fov, aspect, near, far) {
			mat4.perspective(fov, aspect, near, far, gl.matProjection);
		};

		gl.lookAt = function(vecEyes, vecTarjet, vecHead) {
			var pMatrix = mat4.create();
			mat4.lookAt(vecEyes, vecTarjet, vecHead, pMatrix);
			mat4.multiply(gl.matProjection, pMatrix);
		};

		gl.loadIdentity = function() {
			mat4.identity(gl.matObject);
			return gl.matObject;
		};

		gl.matrixStack=new Array();

		gl.pushMatrix = function() {
			var obj=mat4.create(gl.matObject);
			gl.matrixStack.push(obj);
		};

		gl.popMatrix = function() {
			gl.matObject=gl.matrixStack.pop();
		};

		gl.translate = function(vecMove) {
			mat4.translate(gl.matObject, vecMove);
			return gl.matObject;
		};

		gl.scale = function(vecRate) {
			mat4.scale(gl.matObject, vecRate);
			return gl.matObject;
		};

		gl.rotate = function(radian, vecAxis) {
			mat4.rotate(gl.matObject, radian, vecAxis);
			return gl.matObject;
		};

		return gl;
	};
}

var glTextureShader_latest = new glTextureShaderLatest();
