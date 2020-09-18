FROM node:10.16.3 as frontendBuilder

# RUN apt install g++ make python

ADD web/bsconfig.json web/package.json web/package-lock.json /app/

WORKDIR /app
RUN npm install

COPY web ./
COPY web/.env.example ./.env
RUN npm run build

FROM golang:alpine AS apiBuilder
WORKDIR /go/src/github.com/evilfactorylabs/gow

RUN apk add --update git gcc musl-dev --no-cache
RUN go get -u github.com/gobuffalo/packr/packr

COPY . ./
COPY --from=frontendBuilder /app/build ./web/build

RUN GOOS=linux GOARCH=amd64 packr build -v -ldflags "-s"

FROM alpine:latest
EXPOSE 3030
HEALTHCHECK --retries=10 CMD ["wget", "-qO-", "http://localhost:3030/healthcheck"]
RUN apk add --no-cache bash
WORKDIR /app
COPY --from=apiBuilder /go/src/github.com/evilfactorylabs/gow/gow .
CMD ["./gow"]
