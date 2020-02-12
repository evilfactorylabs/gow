open Utils;

[@react.component]
type hits = {
  id: int,
  ts: string,
  referer: string,
  ua: string,
};

type state =
  | LoadingData
  | ErrorFetch
  | Hits(array(hits));

[@react.component]
let make = (~slug) => {
  let (state, setState) = React.useState(() => LoadingData);

  React.useEffect0(() => {
    Js.Promise.(
      fetch("/api/stats" ++ slug)
      |> then_(response => response##json())
      |> then_(responseJSON => {
           setState(_previousState => Hits(responseJSON));
           Js.Promise.resolve();
         })
      |> catch(_err => {
           setState(_previousState => ErrorFetch);
           Js.Promise.resolve();
         })
      |> ignore
    );

    None;
  });

  let title =
    ReactDOMRe.Style.make(~fontSize="1.8rem", ~marginBottom="2rem", ());

  <div>
    <h2 style=title> {React.string("Stats for: " ++ slug)} </h2>
    {switch (state) {
     | ErrorFetch => React.string("Data is empty") // FIXME: lol like seriously
     | LoadingData => <Loading />
     | Hits(hits) =>
       <table>
         <tr>
           <th> {React.string("#")} </th>
           <th> {React.string("Date")} </th>
           <th> {React.string("Referer")} </th>
           <th className="_width-40"> {React.string("User Agent")} </th>
         </tr>
         {hits
          ->Belt.Array.mapWithIndex((i, hits) => {
              <tr key={string_of_int(hits.id)}>
                <td> {React.string(string_of_int(i + 1))} </td>
                <td> {React.string(parseDate(hits.ts))} </td>
                <td>
                  {React.string(hits.referer == "" ? "-" : hits.referer)}
                </td>
                <td> {React.string(hits.ua)} </td>
              </tr>
            })
          ->React.array}
       </table>
     }}
  </div>;
};