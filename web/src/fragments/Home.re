type state = {urls: Relude.Globals.AsyncResult.t(API.URLs.t, API.Error.t)};

let initialState = {urls: Relude.Globals.AsyncResult.init};

type action =
  | FetchUrl
  | FetchUrlSuccess(API.URLs.t)
  | FetchUrlError(API.Error.t)
  | NoOp;

let reducer = (state, action): ReludeReact.Reducer.update(action, state) =>
  switch (action) {
  | FetchUrl =>
    UpdateWithIO(
      {urls: state.urls |> Relude.Globals.AsyncResult.toBusy},
      API.getUrls()
      |> Relude.IO.bimap(
           res => FetchUrlSuccess(res),
           error => FetchUrlError(error),
         ),
    )
  | FetchUrlSuccess(urls) =>
    Update({...state, urls: Relude.Globals.AsyncResult.completeOk(urls)})
  | FetchUrlError(error) =>
    Update({...state, urls: Relude.Globals.AsyncResult.completeError(error)})
  | NoOp => NoUpdate
  };

module UrlList = {
  [@react.component]
  let make = (~urls: API.URLs.t) => {
    Js.log(urls);
    <table>
      <tr>
        <th> {React.string("Slug")} </th>
        <th className="_width-40"> {React.string("Destination")} </th>
        <th> {React.string("Date")} </th>
        <th className="_text-align-center"> {React.string("Action")} </th>
      </tr>
      {
        switch (urls.data) {
        | None => <p> "No Data"->React.string </p>
        | Some(d) =>
          d
          ->Belt.List.map(url =>
              <tr key={string_of_int(url.id)}>
                <td>
                  <a href={url.slug} target="_blank" rel="noopener noreferer">
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
          ->Array.of_list
          ->React.array
        }
      }
    </table>;
  };
};

module UrlError = {
  [@react.component]
  let make = (~error: API.Error.t) =>
    <div> {React.string(API.Error.show(error))} </div>;
};

module UrlResults = {
  [@react.component]
  let make =
      (~result: Relude.Globals.AsyncResult.t(API.URLs.t, API.Error.t), ~send) =>
    result
    |> ReludeReact.Render.asyncResultByValueLazy(
         _ => <p> "Loading"->React.string </p>,
         urls => <UrlList urls />,
         error => <UrlError error />,
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
