package api

import (
	"net/http"

	"github.com/gobuffalo/packr"
)

// StaticHandler —
type StaticHandler func(w http.ResponseWriter, r *http.Request) error

// ServeFileHandler —
func ServeFileHandler(box *packr.Box, filename string) http.Handler {
	return handlerFunc(ServeFile(box, filename))
}

// ServeFile —
func ServeFile(box *packr.Box, filename string) StaticHandler {
	return func(w http.ResponseWriter, r *http.Request) error {
		f, err := box.Open(filename)

		if err != nil {
			return err
		}

		defer f.Close()

		d, err := f.Stat()

		if err != nil {
			return err
		}

		w.Header().Set("X-Content-Type-Options", "nosniff")
		w.Header().Set("X-Xss-Protection", "1; mode=block")
		w.Header().Set("Cache-Control", "max-age=259200") // 3 days

		http.ServeContent(w, r, filename, d.ModTime(), f)
		return nil
	}
}

func (h StaticHandler) ServeHTTP(w http.ResponseWriter, r *http.Request) {
	if err := h(w, r); err != nil {
		errorHandler(w, r, err)
	}
}

func handlerFunc(fn StaticHandler) http.HandlerFunc {
	return http.HandlerFunc(StaticHandler(fn).ServeHTTP)
}
