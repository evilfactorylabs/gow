package models

import (
	"database/sql"
	"log"

	"github.com/lucsky/cuid"
)

/**

- table: url
- version: 1.0.0

----------------------------------------------0--------------+
| id 										| slug         | ts   | destination  |
-------------------------------------------------------------+
| integer, primary key  | text, unique | text | text         |
+------------------------------------------------------------+

*/

// URL —
type URL struct {
	ID          int    `json:"id"`
	Slug        string `json:"slug"`
	TS          string `json:"ts"`
	Destination string `json:"destination"`
}

// CheckIsSlugExists —
func CheckIsSlugExists(db *sql.DB, slug string) bool {
	var url URL

	err := db.QueryRow(`select slug from url where slug = ?`, slug).Scan(&url.Slug)

	return err != sql.ErrNoRows
}

// GetURLDestBySlug —
func GetURLDestBySlug(db *sql.DB, slug string) URL {
	var url URL

	query, err := db.Query(`select destination from url where slug = ?`, slug)

	if err != nil {
		log.Fatal(err)
	}

	for query.Next() {
		err = query.Scan(&url.Destination)

		if err != nil {
			log.Fatal(err)
		}
	}

	return url
}

// GetAllURL —
func GetAllURL(db *sql.DB) []URL {
	var urls []URL
	var url URL

	getAllURL, err := db.Query(`select * from url`)

	if err != nil {
		log.Fatal(err)
	}

	for getAllURL.Next() {
		err = getAllURL.Scan(&url.ID, &url.Slug, &url.TS, &url.Destination)

		if err != nil {
			log.Fatal(err)
		}

		urls = append(urls, url)
	}

	return urls
}

// CreateURL -
func CreateURL(db *sql.DB, slug string, dest string) error {
	if slug == "" {
		slug = cuid.Slug()
	}

	createNewURL, err := db.Prepare("insert into url (slug, destination) values (?, ?)")

	_, err = createNewURL.Exec(slug, dest)

	if err != nil {
		log.Fatal(err)
	}

	return err
}
