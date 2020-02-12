package main

import (
	"github.com/evilfactorylabs/gow/api"
	"github.com/evilfactorylabs/gow/config"
)

func main() {
	config := config.GetConfig()
	server := &api.API{}

	server.Init(config)

	RunMigration(server.DB)

	server.Run(config.Server.Host, config.Server.Port)
}
