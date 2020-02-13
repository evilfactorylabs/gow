# gow

Simple URL shortener with simple analytics.

## Motivation

This program is powering our internal url shortener (evlfctry.pro) which is to
track link distribution without tracking any personal data.

## Techonologies

- Go
- SQLite
- Reason React (Web)

## Goals

- Minimal dependencies
- Minimal memory footprint
- No personal data tracking

## Development

Make sure you have Go & SQLite installed (and it's dependencies), if not, you know what's next.

- Build the application: `make`
- Run the server: `GOW_TOKEN=<token> ./gow`

If you want to develop the web as well, you need to open new terminal session and
run `npm run dev` in `web` directory.

## Environment variables

- `GOW_TOKEN` - Used for creating via REST_API, default: (empty)
- `HOST` - Helpful when you are using reverse proxy, default: `localhost`
- `PORT` - Helpful in case you don't use container technology, default: `3030`

For now (v0.2.0), authorization is done in Layer 7 (Load Balancer), but I will fix this.

## Deployment

There are 2 way to run this program in Production

### Quick way

- Pull the image: `docker pull evilfactorylabs/gow:<version>`
- Run it: `docker run -d -e TOKEN="<token>" -v ~/data:/app/data evilfactorylabs/gow:<version>`

### As Binary

- Clone repo
- Build code: `make`
- Run program: `GOW_TOKEN=<token> ./gow`

### As Docker Container

- Build image: `docker build -t . evilfactorylabs/gow:latest`
- Run image: `docker run -d -e TOKEN="<token>" -v ~/data:/app/data evilfactorylabs/gow:<version>`

## Roadmap

- [ ] v1.0.0 — stable basic functionality [api, web]
- [ ] v1.1.0 — show total hits on index & stats page [api, web]
- [ ] v1.2.0 — functionality to delete existing slug [api]
- [ ] v1.3.0 — authentication functionality, not rely on token (like now) [api, db]
- [ ] v1.4.0 — make migration more easier and reliable [db]
- [ ] v1.5.0 — fancy chart! [web]

## Maintainer(s)

- [Fariz Rizaldy, evilfactorylabs](https://twitter.com/faultable)

## License

(c) 2020 evilfactorylabs, under MIT License.
