package api

import (
	"database/sql"
	"log"
	"net/http"

	"github.com/bmizerany/pat"
	"github.com/evilfactorylabs/gow/config"
	_ "github.com/mattn/go-sqlite3" // sqlite3
)

// API —
type API struct {
	Router *pat.PatternServeMux
	DB     *sql.DB
	Token  string
}

// Init — Initialize api, setting up router & db
func (API *API) Init(config *config.Config) {
	API.Router = pat.New()
	API.Token = config.Token
	API.DB = API.ConnectDB(config.DB.Dialect, config.DB.Path)

	API.MountRouters()
}

// Run — Run server based on selected port
func (API *API) Run(host string, port string) {
	log.Printf("Server run at http://%s%s", host, port)
	log.Fatal(http.ListenAndServe(port, API.Router))
}
