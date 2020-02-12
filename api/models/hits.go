package models

import (
	"database/sql"
	"log"
)

/**

- table: hits
- version: 1.0.0

-----------------------------------------------------------------------------------------+
| id 										| slug | ts                                   | referer  | ua    |
-----------------------------------------------------------------------------------------+
| integer, primary key  | text | datetime, default: CURRENT_TIMESTAMP | text     | text  |
+----------------------------------------------------------------------------------------+

*/

// Hits —
type Hits struct {
	ID      int    `json:"id"`
	Slug    string `json:"slug"`
	TS      string `json:"ts"`
	Referer string `json:"referer"`
	UA      string `json:"ua"`
}

// SaveToHits — Save data to hits
func SaveToHits(db *sql.DB, slug string, referer string, userAgent string) (string, error) {
	saveHitsQuery, err := db.Prepare(`insert into hits (slug, referer, ua) values (?, ?, ?)`)

	_, err = saveHitsQuery.Exec(slug, referer, userAgent)

	return slug, err
}

// GetHitsBySlug — Get hits detail by slug
func GetHitsBySlug(db *sql.DB, slug string) []Hits {
	var listHits []Hits
	var hits Hits

	getHitsBySlug, err := db.Query(`select * from hits where slug = ?`, slug)

	if err != nil {
		log.Fatal(err)
	}

	for getHitsBySlug.Next() {
		err = getHitsBySlug.Scan(&hits.ID, &hits.Slug, &hits.TS, &hits.Referer, &hits.UA)

		if err != nil {
			log.Fatal(err)
		}

		listHits = append(listHits, hits)
	}

	return listHits
}
