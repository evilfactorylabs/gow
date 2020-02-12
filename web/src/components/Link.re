[@react.component]
let make = (~href, ~className="", ~style: ReactDOMRe.Style.t=?, ~children) => {
  let handleClick = (href, e) => {
    ReactEvent.Mouse.preventDefault(e);
    ReasonReactRouter.push("/#" ++ href);
  };

  <a href={"/#" ++ href} style className onClick={e => handleClick(href, e)}>
    children
  </a>;
};