[@react.component]
let make = () => {
  let (url, setUrl) = React.useState(() => "");
  let (slug, setSlug) = React.useState(() => "");
  let (message, setMessage) = React.useState(() => "");

  let handleInput = (e, context) => {
    let value = e->ReactEvent.Form.target##value;

    if (context == "url") {
      setUrl(value);
    } else {
      setSlug(value);
    };
  };

  let clearInput = status => {
    setUrl(_prevState => "");
    setSlug(_prevState => "");

    if (status >= 400) {
      setMessage(_prevState => "failed to create url!");
    } else {
      setMessage(_prevState => "succesfully saved!");
    };

    Js.Global.setTimeout(() => {setMessage(_prevState => "")}, 3000);
  };

  let handleSubmit = e => {
    ReactEvent.Form.preventDefault(e);

    let payload = Js.Dict.empty();

    Js.Dict.set(payload, "destination", Js.Json.string(url));
    Js.Dict.set(payload, "slug", Js.Json.string(slug));
    let _ = {
      Js.Promise.(
        Fetch.fetchWithInit(
          "/api/url/create",
          Fetch.RequestInit.make(
            ~method_=Post,
            ~body=
              Fetch.BodyInit.make(
                Js.Json.stringify(Js.Json.object_(payload)),
              ),
            ~headers=
              Fetch.HeadersInit.make({"content-type": "application/json"}),
            (),
          ),
        )
        |> then_(r => clearInput(Fetch.Response.status(r)) |> resolve)
      );
    };
    ();
  };

  let inputStyle =
    ReactDOMRe.Style.make(
      ~width="100%",
      ~padding="0.4rem 0.5rem",
      ~color="inherit",
      ~fontFamily="inherit",
      ~fontSize="inherit",
      ~display="block",
      (),
    );

  let labelStyle =
    ReactDOMRe.Style.make(
      ~marginTop="1rem",
      ~lineHeight="2.5rem",
      ~color="#555555",
      ~display="block",
      (),
    );

  let buttonStyle =
    ReactDOMRe.Style.make(
      ~fontSize="inherit",
      ~padding="5px 15px",
      ~borderRadius="2px",
      ~border="1px solid #ccc",
      ~color="inherit",
      ~display="block",
      ~marginTop="1rem",
      ~fontFamily="inherit",
      ~cursor="pointer",
      (),
    );

  <form onSubmit={e => handleSubmit(e)}>
    <h2> {React.string("new url")} </h2>
    <div>
      <label htmlFor="url" style=labelStyle> {React.string("URL")} </label>
      <input
        id="url"
        style=inputStyle
        type_="url"
        required=true
        onChange={e => handleInput(e, "url")}
        placeholder="https://evlfctry.pro"
        value=url
      />
    </div>
    <div>
      <label htmlFor="slug" style=labelStyle>
        {React.string("slug (optional)")}
      </label>
      <input
        id="slug"
        style=inputStyle
        type_="text"
        onChange={e => handleInput(e, "slug")}
        placeholder="gow (optional)"
        value=slug
      />
      <button style=buttonStyle> {React.string("save")} </button>
    </div>
    <br />
    <p> {React.string(message)} </p>
  </form>;
};
