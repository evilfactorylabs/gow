package config

import (
	"log"
	"os"
)

// Config —
type Config struct {
	Server *server
	DB     *dbConfig
	Token  string
}

type server struct {
	Host string
	Port string
}

type dbConfig struct {
	Dialect string
	Path    string
}

func getHost() string {
	host := os.Getenv("HOST")

	if host == "" {
		host = "localhost"
	}

	return host
}

func getPort() string {
	port := os.Getenv("port")

	if port == "" {
		port = ":3030"
	}

	return port
}

// GetConfig —
func GetConfig() *Config {
	var token = os.Getenv("TOKEN")

	if token == "" {
		log.Fatalln("[config] `TOKEN` is not set")
	}

	return &Config{
		Server: &server{
			Host: getHost(),
			Port: getPort(),
		},
		DB: &dbConfig{
			Dialect: "sqlite3",
			Path:    "./data/gow.db",
		},
		Token: token,
	}
}
