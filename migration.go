package main

import (
	"database/sql"
	"log"
)

func setupTable(db *sql.DB) {
	createURLTable := `create table if not exists url (id integer not null primary key, slug text unique not null, ts datetime default CURRENT_TIMESTAMP, destination text not null)`

	_, err := db.Exec(createURLTable)

	if err != nil {
		log.Fatal(err)
	}

	createHitsTable := `create table if not exists hits (id integer not null primary key, slug text not null, ts datetime default CURRENT_TIMESTAMP, referer text, ua text)`

	_, err = db.Exec(createHitsTable)

	if err != nil {
		log.Fatal(err)
	}
}

func setupIndex(db *sql.DB) {
	createHitsSlugIndex := `create index if not exists hits_slug on hits (slug)`

	_, err := db.Exec(createHitsSlugIndex)

	if err != nil {
		log.Fatal(err)
	}

	createURLSlugIndex := `create index if not exists url_slug on url (slug)`

	_, err = db.Exec(createURLSlugIndex)

	if err != nil {
		log.Fatal(err)
	}
}

// RunMigration —
func RunMigration(db *sql.DB) {
	setupTable(db)
	setupIndex(db)
}
