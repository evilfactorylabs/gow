package api

import (
	"context"
	"database/sql"
	"log"
	"net/http"
	"os"
	"os/signal"
	"syscall"
	"time"

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
	srv := &http.Server{
		Addr:    port,
		Handler: API.Router,
	}

	stop := make(chan os.Signal, 1)
	signal.Notify(stop, os.Interrupt, syscall.SIGINT, syscall.SIGTERM)

	go func() {
		if err := srv.ListenAndServe(); err != nil && err != http.ErrServerClosed {
			log.Fatalf("Server stopped but: %s", err)
		}
	}()

	log.Printf("Server run at http://%s%s", host, port)

	<-stop

	log.Printf("Server stopped")

	ctx, cancel := context.WithTimeout(context.Background(), 5*time.Second)

	defer func() {
		API.DB.Close()
		cancel()
	}()

	if err := srv.Shutdown(ctx); err != nil {
		log.Fatalf("Server shutdown failed: %v", err)
	}

	log.Printf("Server shutdown properly")
}
