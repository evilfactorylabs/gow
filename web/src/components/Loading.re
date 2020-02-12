[@react.component]
let make = () => {
  let loader =
    ReactDOMRe.Style.make(
      ~width="100%",
      ~textAlign="center",
      ~fontWeight="bold",
      ~padding="2rem",
      (),
    );

  <div style=loader />;
};