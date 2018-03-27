let component = ReasonReact.statelessComponent("Admin");

module Styles = {
  open Css;
  let root = style([background(hex("F7F8FB")), height(pct(100.))]);
};

let make = (~isLogged: bool, _children) => {
  ...component,
  didMount: _self => {
    if (! isLogged) {
      ReasonReact.Router.push(Router.Config.routeToUrl(Login));
    };
    ReasonReact.NoUpdate;
  },
  render: _self =>
    <div className=Styles.root>
      <Navbar />
      <h1> (ReasonReact.stringToElement("Admin")) </h1>
    </div>,
};