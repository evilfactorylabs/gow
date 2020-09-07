package api

import (
	"net/http"

	"github.com/evilfactorylabs/gow/api/controllers"
	"github.com/gobuffalo/packr"
)

// MountRouters —
func (API *API) MountRouters() {

	box := packr.NewBox("./../web/build")

	API.Router.Get("/", http.FileServer(box))
	API.Router.Get("/healthcheck", http.HandlerFunc(healthCheck))

	// FIXME: lol
	API.Router.Get("/app.js", ServeFileHandler(&box, "app.js"))

	API.Router.Get("/api/url/list", API.commonRequestHandler(controllers.GetURLs))
	API.Router.Post("/api/url/create", API.commonRequestHandler(controllers.CreateURL))
	API.Router.Get("/api/stats/:slug", API.commonRequestHandler(controllers.GetHitsStatsBySlug))

	API.Router.Del("/api/:slug", API.AuthorizationMiddleware(API.commonRequestHandler(controllers.DeleteURL)))
	API.Router.Get("/:slug", API.SendToSlackMiddleware(API.commonRequestHandler(controllers.GetURL)))
}
