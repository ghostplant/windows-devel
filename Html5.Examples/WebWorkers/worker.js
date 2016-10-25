onmessage = function (evt) {
	var d = evt.data;
	postMessage('got: ' + d);
}

