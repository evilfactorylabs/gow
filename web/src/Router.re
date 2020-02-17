type t =
  | Home
  | New
  | Stats(string)
  | Notfound;

let unwrap =
  fun
  | Home => "home"
  | New => "new"
  | Stats(_) => "stats"
  | _ => "404";

let parseUrl = (url: ReasonReactRouter.url) =>
  switch (url.path) {
  | ["/"] => Home
  | ["/new"] => New
  | ["/stats", slug] => Stats(slug)
  | _ => Notfound
  };

/*
  use for handle button or ahref click
 */
let linkTo = (path, event) => {
  event->ReactEvent.Mouse.preventDefault;
  path->ReasonReactRouter.push;
};
