package controllers

import (
	"database/sql"
	"net/http"

	"github.com/evilfactorylabs/gow/api/models"
)

type notFound struct {
	Data []string
}

// GetHitsStatsBySlug — TODO: Create middleware of this
func GetHitsStatsBySlug(db *sql.DB, w http.ResponseWriter, r *http.Request) {
	params := r.URL.Query()
	slug := params.Get(":slug")
	payload := models.GetHitsBySlug(db, slug)

	if payload == nil {
		emptyData := make([]string, 0)

		// FIXME: this is temporary fix to our frontend!
		ResponseJSON(w, http.StatusNoContent, notFound{emptyData})

		return
	}

	ResponseJSON(w, http.StatusOK, payload)
}
