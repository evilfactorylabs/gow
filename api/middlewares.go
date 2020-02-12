package api

import (
	"database/sql"
	"fmt"
	"net/http"
)

type requestHandlerFunction func(db *sql.DB, w http.ResponseWriter, r *http.Request)

func errorHandler(w http.ResponseWriter, r *http.Request, err error) {
	w.WriteHeader(http.StatusInternalServerError)
	w.Header().Set("Content-Type", "application/json")
	w.Write([]byte("false"))
}

func (API *API) commonRequestHandler(handler requestHandlerFunction) http.HandlerFunc {
	return func(w http.ResponseWriter, r *http.Request) {
		handler(API.DB, w, r)
	}
}

// AuthorizationMiddleware —
func (API *API) AuthorizationMiddleware(handler http.Handler) http.Handler {
	return http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		var token = r.Header.Get("X-TOKEN")

		if token != API.Token {
			w.WriteHeader(http.StatusUnauthorized)

			fmt.Fprintf(w, "Unauthorized")

			return
		}

		handler.ServeHTTP(w, r)
	})
}
