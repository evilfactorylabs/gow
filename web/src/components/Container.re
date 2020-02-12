[@react.component]
let make = (~children) => {
  let container =
    ReactDOMRe.Style.make(
      ~backgroundColor="#fff",
      ~border="1px solid #eee",
      ~borderRadius="3px",
      ~width="1020px",
      ~maxWidth="100%",
      ~margin="auto",
      ~marginTop="2rem",
      (),
    );

  <div style=container> children </div>;
};