package hellompp

import (
	"bytes"
	"html/template"
	"net/http"
)

func init() {
	http.HandleFunc("/", func (w http.ResponseWriter, r *http.Request) {
		b := &bytes.Buffer{}
		template.Must(template.ParseFiles("index.html",)).ExecuteTemplate(b, "index.html", 0)
		b.WriteTo(w)
	})
}


