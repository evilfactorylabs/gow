build: build-api install-web build-web

build-api:
	go build -ldflags "-s"

build-web:
	cd web && npm run build

install-web:
	cd web && npm install
