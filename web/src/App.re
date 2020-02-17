[@react.component]
let make = () => {
  let url = ReasonReactRouter.useUrl();
  let route = Router.parseUrl(url);

  let fragmentContainer =
    ReactDOMRe.Style.make(
      ~padding="2rem",
      ~height="800px",
      ~overflowY="auto",
      (),
    );

  <Container>
    <Navbar path={url.hash} />
    <main style=fragmentContainer>
      {
        switch (route) {
        | Home => <Home />
        | New => <New />
        | Stats(slug) => <Stats slug />
        | _ => <Notfound />
        }
      }
    </main>
  </Container>;
};
