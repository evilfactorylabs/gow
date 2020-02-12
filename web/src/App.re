[@react.component]
let make = () => {
  let url = ReasonReactRouter.useUrl();

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
      {switch (url.hash) {
       | "" => <Home />
       | "/" => <Home />
       | "/new" => <New />
       | _ => <Stats slug={url.hash} /> // FIXME: lol wtf how to handle url globbing
       }}
    </main>
  </Container>;
};