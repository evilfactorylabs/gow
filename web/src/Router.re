type t =
  | Home
  | New
  | Stats(string)
  | Notfound;

let unwrap =
  fun
  | Home => "/#/"
  | New => "/#/new"
  | Stats(slug) => {j|/#/$slug|j}
  | _ => "404";

let parseUrl = (url: ReasonReactRouter.url) => {
  switch (url.hash) {
  | "" => Home 
  | "/" => Home
  | "/new" => New
  | slug => Stats(slug)
  | _ => Notfound
  };
}

/*
  use for handle button or ahref click
 */
let linkTo = (path, event) => {
  event->ReactEvent.Mouse.preventDefault;
  path->ReasonReactRouter.push;
};
