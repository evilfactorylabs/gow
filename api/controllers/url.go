package controllers

import (
	"database/sql"
	"encoding/json"
	"fmt"
	"log"
	"net/http"

	"github.com/evilfactorylabs/gow/api/models"
)

// GetURL — get destination by slug & track to hits
func GetURL(db *sql.DB, w http.ResponseWriter, r *http.Request) {

	params := r.URL.Query()

	slug := params.Get(":slug")
	referer := r.Header.Get("referer")
	userAgent := r.Header.Get("user-agent")

	_, err := models.SaveToHits(db, slug, referer, userAgent)

	if err != nil {
		log.Fatal(err)
	}

	url := models.GetURLDestBySlug(db, slug)

	if url.Destination == "" {
		w.WriteHeader(http.StatusNotFound)
		fmt.Fprintf(w, "url not found")

		return
	}

	w.Header().Set("Location", url.Destination)
	w.WriteHeader(http.StatusFound)
}

// GetURLs — get all available URL
func GetURLs(db *sql.DB, w http.ResponseWriter, r *http.Request) {

	payload := models.GetAllURL(db)

	ResponseJSON(w, http.StatusOK, payload)
}

// DeleteURL —
func DeleteURL(db *sql.DB, w http.ResponseWriter, r *http.Request) {
	w.WriteHeader(http.StatusOK)
	fmt.Fprintf(w, "OK")
}

// CreateURL —
func CreateURL(db *sql.DB, w http.ResponseWriter, r *http.Request) {
	var url *models.URL

	err := json.NewDecoder(r.Body).Decode(&url)

	if err != nil {
		log.Fatal(err)
	}

	if url.Destination == "" {
		ResponseError(w, http.StatusBadRequest, "destination is null")

		return
	}

	isSlugExists := models.CheckIsSlugExists(db, url.Slug)

	if isSlugExists {
		ResponseError(w, http.StatusConflict, "Slug exists")
		return
	}

	err = models.CreateURL(db, url.Slug, url.Destination)

	if err != nil {
		log.Fatal(err)
	}

	fmt.Fprintf(w, "OK")
}
