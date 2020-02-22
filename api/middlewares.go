package api

import (
	"bytes"
	"database/sql"
	"encoding/json"
	"fmt"
	"io/ioutil"
	"log"
	"net/http"
	"os"
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

func healthCheck(w http.ResponseWriter, r *http.Request) {
	fmt.Fprintf(w, "OK")

	return
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

func (API *API) SendToSlackMiddleware(handler http.Handler) http.Handler {
	return http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		var slackWebhook = os.Getenv("SLACK_WEBHOOK")

		if slackWebhook != "" {

			type slackPayload struct {
				Text string `json:"text"`
			}

			path := r.URL.Path
			ua := r.Header.Get("User-Agent")
			referer := r.Header.Get("Referer")

			text := fmt.Sprintf("UA: %s \n Referer: %s \n Path: %s \n", ua, referer, path)

			body, _ := json.Marshal(slackPayload{Text: text})
			req, err := http.Post(slackWebhook, "application/json", bytes.NewBuffer(body))

			if err != nil {
				log.Println(err)
			}

			defer req.Body.Close()

			body, err = ioutil.ReadAll(req.Body)

			if err != nil {
				log.Printf("Error sending to Slack: %s \n", err)
			}

		}

		handler.ServeHTTP(w, r)
	})
}
