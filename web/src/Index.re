/* use Webapi for communicate DOM (non-react) in Reason */
open Webapi.Dom;

/* our gow react app ID */
let appId = "g0W";

/*
 cause HTMLBodyElement not implemented in Webapi.Dom
 and our need to create new Element in `document.body`.
 unsafe_document##body##appendChild(dom)
 */
[@bs.val] external unsafe_document: Js.t({..}) = "document";

/*
 checking appId in DOM, when exists our `hydrate` VDOM to DOM
 when not exist, we create new element and `render` VDOM to DOM
 */
switch (document |> Document.getElementById(appId)) {
| Some(_) => ReactDOMRe.hydrateToElementWithId(<App />, appId)
| None =>
  let dom = document |> Document.createElement("div");
  dom |> Element.setAttribute("id", appId);
  unsafe_document##body##appendChild(dom)->ignore;
  ReactDOMRe.renderToElementWithId(<App />, appId);
};
