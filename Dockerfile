FROM node:alpine as frontendBuilder
WORKDIR /app

COPY web ./

RUN apk add --update g++ make python
RUN npm install
RUN npm run build

FROM golang:alpine AS apiBuilder
WORKDIR /go/src/github.com/evilfactorylabs/gow

RUN apk add --update git gcc musl-dev
RUN go get -u github.com/gobuffalo/packr/packr

COPY . ./
COPY --from=frontendBuilder /app/dist ./web/dist

RUN GOOS=linux GOARCH=amd64 packr build -v -ldflags "-s"

FROM alpine:latest
EXPOSE 3030
HEALTHCHECK --retries=10 CMD ["wget", "-qO-", "http://localhost:3030/healthcheck"]
RUN apk add --no-cache bash
WORKDIR /app
COPY --from=apiBuilder /go/src/github.com/evilfactorylabs/gow/gow .
CMD ["./gow"]
