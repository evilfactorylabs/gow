package api

import (
	"database/sql"
	"log"
)

// ConnectDB — create db instance
func (API *API) ConnectDB(dialect string, dbPath string) *sql.DB {
	db, err := sql.Open(dialect, dbPath)

	if err != nil {
		log.Fatal(err)
	}

	return db
}
