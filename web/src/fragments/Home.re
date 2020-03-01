open Utils;

[@bs.val] external date: string => string = "Date";

type url = {
  id: int,
  slug: string,
  destination: string,
  ts: string,
};

type state =
  | LoadingData
  | ErrorFetch
  | URLs(array(url));

[@react.component]
let make = () => {
  let (state, setState) = React.useState(() => LoadingData);

  React.useEffect0(() => {
    Js.Promise.(
      fetch("/api/url/list")
      |> then_(response => response##json())
      |> then_(responseJSON => {
           setState(_previousState => URLs(responseJSON));
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

  <div>
    {switch (state) {
     | ErrorFetch => React.string("Something went wrong")
     | LoadingData => <Loading />
     | URLs(url) =>
       <table>
         <tr>
           <th> {React.string("Slug")} </th>
           <th
             className="_width-40"
             style={ReactDOMRe.Style.make(
               ~maxWidth="66.6px",
               ~overflow="hidden",
               (),
             )}>
             {React.string("Destination")}
           </th>
           <th> {React.string("Date")} </th>
           <th className="_text-align-center"> {React.string("Action")} </th>
         </tr>
         {url
          ->Belt.Array.map(url => {
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
                <td> {React.string(parseDate(url.ts))} </td>
                <td className="_text-align-center">
                  <Link href={Router.Stats(url.slug)}>
                    {React.string("Stats")}
                  </Link>
                </td>
              </tr>
            })
          ->React.array}
       </table>
     }}
  </div>;
};