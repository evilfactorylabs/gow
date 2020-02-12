[@react.component]
let make = (~path) => {
  let navStyle =
    ReactDOMRe.Style.make(
      ~backgroundColor="#06102a",
      ~color="#fff",
      ~padding="1rem",
      (),
    );

  let navBrand =
    ReactDOMRe.Style.make(
      ~fontSize="1.6rem",
      ~fontWeight="bold",
      ~color="inherit",
      ~textDecoration="none",
      ~lineHeight="0",
      (),
    );

  let navAction =
    ReactDOMRe.Style.make(
      ~float="right",
      ~fontWeight="bold",
      ~textDecoration="none",
      (),
    );

  let navTitle = path == "/" || path == "" ? "gow" : "back";

  <nav style=navStyle>
    <Link href="/" style=navBrand> {React.string(navTitle)} </Link>
    <Link href="/new" style=navAction> {React.string("new link")} </Link>
  </nav>;
};