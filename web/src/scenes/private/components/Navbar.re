let component = "Navbar" |> ReasonReact.statelessComponent;

module Styles = {
  open Css;
  let root =
    style([
      display(`flex),
      justifyContent(center),
      alignItems(center),
      height(px(60)),
      background(hex("fff")),
      borderBottom(px(1), solid, hex("eee")),
    ]);
  let title =
    style([
      fontSize(px(22)),
      textTransform(uppercase),
      fontWeight(700),
      color(hex("69798F")),
    ]);
};

let make = _children => {
  ...component,
  render: _self =>
    <nav className=Styles.root>
      <h1 className=Styles.title>
        (ReasonReact.stringToElement("Downloads"))
      </h1>
    </nav>,
};