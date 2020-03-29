open Relude.Globals;

module Error = {
  type t = ReludeFetch.Error.t(string);
  let show = error =>
    switch (error) {
    | _ => "Gagal memuat data"
    };
  module Type = {
    type nonrec t = t;
  };
};

module Endpoint = {
  type t =
    | URLs;

  let unwrap =
    fun
    | URLs => "/api/url/list";
};

module URLs = {
  [@decco]
  type url = {
    id: int,
    slug: string,
    destination: string,
    ts: string,
  };

  [@decco]
  type t = array(url);

  let decode: Js.Json.t => Belt.Result.t(t, string) =
    json => json->t_decode |> Relude.Result.mapError(_ => "Decode failed");
};

module IOE = IO.WithError(Error.Type);
open IOE.Infix;

let getUrls = (): IO.t(URLs.t, Error.t) =>
  Endpoint.(URLs->unwrap)->ReludeFetch.fetch
  >>= URLs.decode->ReludeFetch.Response.Json.decode;
