open Relude.Globals;
type state = {urls: AsyncResult.t(API.URLs.t, API.Error.t)};

let initialState = {urls: AsyncResult.init};

type action =
  | FetchUrl
  | FetchUrlSuccess(API.URLs.t)
  | FetchUrlError(API.Error.t)
  | NoOp;

let reducer = (state, action): ReludeReact.Reducer.update(action, state) =>
  switch (action) {
  | FetchUrl =>
    UpdateWithIO(
      {urls: state.urls |> AsyncResult.toBusy},
      API.getUrls()
      |> Relude.IO.bimap(
           res => FetchUrlSuccess(res),
           error => FetchUrlError(error),
         ),
    )
  | FetchUrlSuccess(urls) => Update({urls: AsyncResult.completeOk(urls)})
  | FetchUrlError(error) => Update({urls: AsyncResult.completeError(error)})
  | NoOp => NoUpdate
  };

module UrlList = {
  [@react.component]
  let make = (~urls: API.URLs.t) => {
    <table>
      <tr>
        <th> {React.string("Slug")} </th>
        <th className="_width-40"> {React.string("Destination")} </th>
        <th> {React.string("Date")} </th>
        <th className="_text-align-center"> {React.string("Action")} </th>
      </tr>
      {urls
       |> Array.map((url: API.URLs.url) =>
            <tr key={string_of_int(url.id)}>
              <td>
                <a
                  href=Router.(url.slug->Stats->unwrap)
                  target="_blank"
                  rel="noopener noreferer">
                  {React.string("/" ++ url.slug)}
                </a>
              </td>
              <td>
                <a
                  href={url.destination}
                  target="_blank"
                  rel="noopener noreferer">
                  {React.string(url.destination)}
                </a>
              </td>
              <td> {React.string(url.ts)} </td>
              <td className="_text-align-center">
                <Link href={Router.Stats(url.slug)}>
                  {React.string("Stats")}
                </Link>
              </td>
            </tr>
          )
       |> React.array}
    </table>;
  };
};

module UrlError = {
  [@react.component]
  let make = (~error: API.Error.t, ~send) =>
    <section
      style={ReactDOMRe.Style.make(
        ~backgroundColor="#000000",
        ~color="#D01C22",
        ~fontSize="large",
        ~padding="1em",
        (),
      )}>
      {React.string(API.Error.show(error))}
      <button
        style={ReactDOMRe.Style.make(
          ~marginLeft="1em",
          ~border="0",
          ~borderRadius="5px",
          ~padding="5px",
          ~background="#D01C22",
          ~color="#fff",
          ~outline="0",
          ~cursor="pointer",
          (),
        )}
        onClick={_ => FetchUrl->send}>
        "Coba, Lagi"->React.string
      </button>
    </section>;
};

module UrlResults = {
  [@react.component]
  let make = (~result: AsyncResult.t(API.URLs.t, API.Error.t), ~send) =>
    result
    |> ReludeReact.Render.asyncResultByValueLazy(
         _ => <p> "Loading"->React.string </p>,
         urls => <UrlList urls />,
         error => <UrlError error send />,
       );
};

module Main = {
  [@react.component]
  let make = (~state, ~send) => <UrlResults result={state.urls} send />;
};

[@react.component]
let make = () => {
  let (state, send) = ReludeReact.Reducer.useReducer(reducer, initialState);
  ReludeReact.Effect.useOnMount(() => send(FetchUrl));
  <Main state send />;
};
