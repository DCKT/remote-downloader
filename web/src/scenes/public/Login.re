let component = "Login" |> ReasonReact.statelessComponent;

module Styles = {
  open Css;
  let root =
    style([
      display(`flex),
      justifyContent(center),
      alignItems(center),
      background(hex("0288D1")),
      height(pct(100.)),
    ]);
  let box =
    style([
      background(hex("fff")),
      width(px(500)),
      padding(px(40)),
      boxShadow(~x=px(0), ~y=px(3), ~blur=px(9), rgba(24, 24, 24, 0.3)),
    ]);
};

let make = (~isLogged: bool, ~connectUser, _children) => {
  ...component,
  didMount: _self => {
    if (isLogged) {
      ReasonReact.Router.push(Router.Config.routeToUrl(Admin));
    };
    ReasonReact.NoUpdate;
  },
  render: _self =>
    <div className=Styles.root>
      <div className=Styles.box>
        <h1> (ReasonReact.stringToElement("Login")) </h1>
        <form>
          <button onClick=connectUser>
            (ReasonReact.stringToElement("Connect me"))
          </button>
        </form>
      </div>
    </div>,
};